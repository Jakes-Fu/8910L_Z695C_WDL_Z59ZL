/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "os_api.h"
#ifdef JAVA_SUPPORT_IA
#include "jkmediatone.h"
#include "jkmediacontrol.h"
#include "media_tonecontrol.h"
#include "audioclient.h"
#include "jwe_log.h"

/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 #define _DEBUG_JWE
#ifndef _DEBUG_JWE
#undef  JWE_LOG
#define JWE_LOG(function,parameters)
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINE                                    */
/*---------------------------------------------------------------------------*/
#define TONE_SEQUENCE_RESOLUTION    5000 /* in MIDI header 500,000/100 = 5000     */
#define MIDI_TRACK_SIZE_OFFSET      18   /* offset for MTrk size in header        */
#define MIDI_HEADER_SIZE            22   /* total size - MIDI_HEADER_SIZE = MTrk-1 Size*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL   AudioClient*    s_tone_ac_ptr = PNULL;//for tone play control 
static  JKUint8         m_u8SingleToneMIDI[47] = {0};

/* for single tone */
static const JKUint8 m_u8MIDIbuffer[29] = 
{
    /* SMF, type 1, 1 track, 100 ticks/quarter-note*/
    0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x01, 0x00, 0x01, 0x00, 0x64,

    /* first track, include the MTrk length, tempo is 500,000 usec/quarter-note  */
    0x4D, 0x54, 0x72, 0x6B, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0x51, 0x03, 0x07, 0xA1, 0x20, 
};

extern PUBLIC int32 writeValue( JKUint8 *pBuffer, int32 iOffset, int32 iValue );
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

LOCAL JKSint32 playNote_tonebuf(JKUint8 *data,JKSint32 length,JKSint32 loop,JKSint32 volume )
{
    JWE_LOG(playNote_tonebuf, ("enter data=0x%x length=%d loop=%d volume=%d",data,length,loop,volume));
    
    if(PNULL !=s_tone_ac_ptr)
    {
        JWE_LOG(playNote_tonebuf, ("s_tone_ac_ptr is not null"));
	    return -1;
    }
    
    s_tone_ac_ptr =  ( AudioClient* )SCI_ALLOCA( sizeof( AudioClient ) );
    if(PNULL == s_tone_ac_ptr)
    {
        JWE_LOG(playNote_tonebuf, ("ALLOC ac FAILED"));
	    return -1;
    }
    SCI_MEMSET(s_tone_ac_ptr,0,sizeof(AudioClient));
    
    s_tone_ac_ptr->isFileStream = FALSE;
    s_tone_ac_ptr->wFileStreamUrl = PNULL;
    s_tone_ac_ptr->isplayTone = TRUE;
    s_tone_ac_ptr->need_PlayTimes = loop;
    s_tone_ac_ptr->pData = data;
    s_tone_ac_ptr->length = length;    
    if(!AudioClient_player_play(s_tone_ac_ptr,loop,-1,volume))
    {
     	JWE_LOG(playNote_tonebuf, ("AudioClient_player_play fail"));
        SCI_FREE(s_tone_ac_ptr);
    	return -1;
    }
    
    JWE_LOG(playNote_tonebuf, ("AudioClient_player_play success"));
    return 0;
}   

/*****************************************************************************/
//  Description : toneCtrl_playNote
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/            
PUBLIC JKSint32 toneCtrl_playNote( JKSint32 note, JKSint32 duration, JKSint32 volume )
{   
    int32      iOffset = 0;
    
    /* generate SMF */
    SCI_MEMCPY( (void*)m_u8SingleToneMIDI, (void*)m_u8MIDIbuffer, sizeof(JKUint8)*sizeof(m_u8MIDIbuffer) );
    iOffset = sizeof(m_u8MIDIbuffer); 

    m_u8SingleToneMIDI[iOffset++] = 0x00;
    m_u8SingleToneMIDI[iOffset++] = 0xC0;
    m_u8SingleToneMIDI[iOffset++] = 16; /* flute */

    /* note on */
    m_u8SingleToneMIDI[iOffset++] = 0x00;
    m_u8SingleToneMIDI[iOffset++] = 0x90;
    m_u8SingleToneMIDI[iOffset++] = (JKUint8)note;
    m_u8SingleToneMIDI[iOffset++] = 64; /* standard */

    /* note-off */
    JWE_LOG(toneCtrl_playNote,("duration = %d",duration))
        //    duration *=10;
    iOffset = writeValue( m_u8SingleToneMIDI, iOffset, duration*1000/TONE_SEQUENCE_RESOLUTION );
    m_u8SingleToneMIDI[iOffset++] = 0x80;
    m_u8SingleToneMIDI[iOffset++] = (JKUint8)note;
    m_u8SingleToneMIDI[iOffset++] = 64; /* standard */

    /* end track */
    m_u8SingleToneMIDI[iOffset++] = 0x00;
    m_u8SingleToneMIDI[iOffset++] = 0xFF;
    m_u8SingleToneMIDI[iOffset++] = 0x2F;
    m_u8SingleToneMIDI[iOffset++] = 0x00;

    /* write track size, little endian */
    m_u8SingleToneMIDI[MIDI_TRACK_SIZE_OFFSET] =   (( iOffset - MIDI_HEADER_SIZE ) >> 24 ) & 0xFF; 
    m_u8SingleToneMIDI[MIDI_TRACK_SIZE_OFFSET+1] = (( iOffset - MIDI_HEADER_SIZE ) >> 16 ) & 0xFF; 
    m_u8SingleToneMIDI[MIDI_TRACK_SIZE_OFFSET+2] = (( iOffset - MIDI_HEADER_SIZE ) >> 8  ) & 0xFF; 
    m_u8SingleToneMIDI[MIDI_TRACK_SIZE_OFFSET+3] = (  iOffset - MIDI_HEADER_SIZE ) & 0xFF; 

    if(playNote_tonebuf(m_u8SingleToneMIDI,iOffset,1,volume) != 0)
    {        
        return (JKSint32)JKMEDIA_RESULT_ERROR;
    }
  

    return (JKSint32)JKMEDIA_RESULT_OK;
}
            
          
PUBLIC void toneCtrl_stopAllNotes( void )
{
    JWE_LOG(toneCtrl_stopAllNotes,("enter"))
    if(PNULL !=s_tone_ac_ptr)
    {
        SCI_FREE(s_tone_ac_ptr);
        s_tone_ac_ptr =  PNULL;
    }
}


PUBLIC void toneCtrl_playNote_callback(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    JWE_LOG(toneCtrl_playNote_callback, ("enter"));
    if(PNULL !=s_tone_ac_ptr)
    {
        SCI_FREE(s_tone_ac_ptr);
        s_tone_ac_ptr =  PNULL;
    }
}

JKSint32 JkMediaTonePlayNote( JKSint32 note, JKSint32 duration, JKSint32 volume )
{
    JWE_LOG(JkMediaTonePlayNote, ("note %d, duration %d, volume %d\n", note, duration, volume));
    return toneCtrl_playNote(note, duration, volume);
}

void JkMediaToneStopAllNotes( void )
{
    JWE_LOG(JkMediaToneStopAllNotes, ("enter"));
    toneCtrl_stopAllNotes();
}

#endif
