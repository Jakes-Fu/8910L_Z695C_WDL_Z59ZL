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
#ifdef JAVA_SUPPORT_IA
#include "jblend_config.h"
#include "jktypes.h"
#include "jkmediacontrol.h"
#include "ammediacontrol.h"
#include "ammemory.h"
#include "amtone.h"
#include "audioclient.h"
#include "jwe_log.h"

#include "media_tonecontrol.h"
#include "mediacontrol.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TONE_SEQUENCE_RESOLUTION    5000 /* in MIDI header 500,000/100 = 5000     */
#define MIDI_TRACK_SIZE_OFFSET      18   /* offset for MTrk size in header        */
#define MIDI_HEADER_SIZE            22   /* total size - MIDI_HEADER_SIZE = MTrk-1 Size*/

/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern JKT_Media_VolumeControl mediaCtrl_VolumeCtrl;
extern JKT_Media_StopControl mediaOpt_StopCtrl;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void* m_alloc( JKUint32 u32Size )
{
    return (void*)SCI_ALLOC_APP( u32Size );
}


LOCAL void  m_free( void *pBuffer )
{
    SCI_FREE( pBuffer );
}

LOCAL JKSint32 toneCtrl_getMIMEType( JKWChar** pType )
{   
    static const JKWChar mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'x', '-', 't', 'o', 'n', 'e', '-', 's', 'e', 'q'
    };
    JWE_LOG(toneCtrl_getMIMEType, ("enter"));
    *pType = ( JKWChar* )mimeType;
    JWE_LOG(toneCtrl_getMIMEType, ("leave!sizeof mimetype str=%d",sizeof( mimeType ) / sizeof( JKWChar )));
    return sizeof( mimeType ) / sizeof( JKWChar );
}

LOCAL JKBool toneCtrl_isData( const JKUint8 *pData, const JKSint32 length )
{
    if(AmToneIsData((JKUint8 *)pData,length) ==0){
        JWE_LOG(toneCtrl_isData, ("The data  is tone file %d"))
	    return JK_TRUE;
    }
    else{
        JWE_LOG(toneCtrl_isData, ("The data  is NOT tone file %d"))
    	return JK_FALSE;
    }
}


LOCAL JKSint32 toneCtrl_create( void )
{
    return mediaCtrl_create(JKMEDIA_TYPE_TONE);
}

LOCAL JKSint32 toneCtrl_checkOpen( const JKUint8 *pData, const JKSint32 length )
{
    T_mediaPlayer *pPlayer;
    JWE_LOG(toneCtrl_checkOpen,("enter length = %d",length));
		
    pPlayer =  createPlayer(JKMEDIA_TYPE_TONE);
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(toneCtrl_checkOpen, ("leave null"));
        return JKMEDIA_RESULT_ERROR;
    }

    pPlayer->pData = ( JKUint8* )pData;
    pPlayer->length = ( JKSint32 )length;
	
#ifdef TONE_USE_JBLEND_PARSER
    /* analysis tone sequence data */    
    pPlayer->tp->hToneSeqHandle = AmToneCreateSeqData( pPlayer->pData, pPlayer->length,
        (JKSint32)TONE_SEQUENCE_RESOLUTION,
        m_alloc, 
        m_free );
    if ( pPlayer->tp->hToneSeqHandle == JK_NULL )
    {
        JWE_LOG(toneCtrl_checkOpen,("err: could not get tone sequence handle"));
        return (JKSint32)JKMEDIA_RESULT_ERROR;
    }

    return pPlayer->id + JKMEDIA_CHECK_ID_BASE;
#else
    return mediaCtrl_checkOpen(pData,length,JKMEDIA_TYPE_TONE);
#endif

}




LOCAL JKSint32 toneCtrl_check( JKSint32 checkID, JKSint32 *pExtraID )
{
    T_mediaPlayer *pPlayer;
    JWE_LOG(toneCtrl_check,( "checkID=%d",checkID))
    pPlayer = getPlayer( checkID - JKMEDIA_CHECK_ID_BASE );
    if ( pPlayer == JK_NULL )
    {
        JWE_LOG(toneCtrl_check,("checkl fail.null player for check id"))
        return JKMEDIA_RESULT_ERROR;
    }

    JWE_LOG(toneCtrl_check, ("leave")); 
    return JKMEDIA_RESULT_OK;
}

/*****************************************************************************/
//  Description : End data check and close media.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL JKSint32 toneCtrl_checkClose( JKSint32 checkID )
{
    T_mediaPlayer *pPlayer;
#ifdef TONE_USE_JBLEND_PARSER
    int32 ret = 0;
#endif
    JWE_LOG(toneCtrl_checkClose, ("enter checkID=%d",checkID));
	
    pPlayer = getPlayer( checkID - JKMEDIA_CHECK_ID_BASE );
    
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(toneCtrl_checkClose, ("null player"));
        return JKMEDIA_RESULT_ERROR;
    }

#ifdef TONE_USE_JBLEND_PARSER

    if ( pPlayer->tp->hToneSeqHandle )
    {
        ret = AmToneDeleteSeqData(pPlayer->tp->hToneSeqHandle);
        JWE_LOG(toneCtrl_checkClose, ("AmToneDeleteSeqData ret %d", ret));
    }
	
    return destroyPlayer(checkID - JKMEDIA_CHECK_ID_BASE);
#else
	mediaCtrl_checkClose(checkID);
#endif
}

LOCAL JKSint32 toneCtrl_getDuration( JKSint32 id, JKUint32 *pUsecH, JKUint32 *pUsecL )
{
    T_mediaPlayer *pPlayer = PNULL;
#ifdef TONE_USE_JBLEND_PARSER
    ToneParser *tp;
    JKSint32          s32Duration = 0;
#endif
    JWE_LOG(toneCtrl_getDuration, ("enter"));
	
    if ( id >= JKMEDIA_CHECK_ID_BASE ) 
    {
        JWE_LOG(toneCtrl_getDuration, ("id is checkID impossible"));
        id -= JKMEDIA_CHECK_ID_BASE;
    }
	
    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(toneCtrl_getDuration, ("null player")); 
        return JKMEDIA_RESULT_ERROR;
    }
	
#ifdef TONE_USE_JBLEND_PARSER
    tp = pPlayer->tp;
    if ( tp->duration == JKMEDIA_DURATION_UNKNOWN ) 
    {
        
        if(tp->hToneSeqHandle == NULL)
        {
            /* analysis tone sequence data */    
            tp->hToneSeqHandle = AmToneCreateSeqData( pPlayer->pData,
                (JKSint32)pPlayer->length,
                (JKSint32)TONE_SEQUENCE_RESOLUTION,
                m_alloc, 
                m_free );
            if ( tp->hToneSeqHandle == JK_NULL )
            {
                JWE_LOG(toneCtrl_getDuration,("err: could not get tone sequence handle"));
                return (JKSint32)JKMEDIA_RESULT_ERROR;
            }
        }
        
        if (( s32Duration = AmToneGetTotalTime( tp->hToneSeqHandle )) == JKMEDIA_RESULT_ERROR )
        {
            JWE_LOG(toneCtrl_getDuration,("AmToneGetTotalTime err"));
            return JKMEDIA_RESULT_ERROR;
        }
	 tp->duration = ((s32Duration)<<1)>>1;
        pPlayer->duration_low =  tp->duration;
	 pPlayer->duration_high = 0;
	 JWE_LOG(toneCtrl_getDuration,("s32Duration = %d  tp->duration  = %d",s32Duration,tp->duration));
    }
#endif
    *pUsecH = 0;
    *pUsecL = pPlayer->duration_low;
    JWE_LOG(toneCtrl_getDuration, ("Leave3, %d,%d!", *pUsecH, *pUsecL));    
    return JKMEDIA_RESULT_OK;
}


LOCAL JKSint32 toneCtrl_getIncludedContents( JKSint32 id )
{
    JWE_LOG(toneCtrl_getIncludedContents, ("enter"))
    return JKMEDIA_CONTENT_AUDIO;
}

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//  Description : static help function to write SMF format value     
 PUBLIC int32 writeValue( JKUint8 *pBuffer, int32 iOffset, int32 iValue )
{
    JKUint32 ulTmp = iValue & 0x7F;
    while(( iValue >>= 7 ) > 0 )
    {
        ulTmp <<= 8;
        ulTmp |= 0x80;
        ulTmp += ( iValue & 0x7F );
    }
    while(1)/*lint !e716*/
    {
        pBuffer[iOffset++] = (JKUint8)ulTmp;
        
        if ( ulTmp & 0x80 )
        {
            ulTmp >>= 8;
        }
        else
        {
            break;
        }
    }
    return iOffset;
}

PUBLIC JKSint32 toneToMidi(uint8*pData,int length,ToneParser* tp)
{
    int32               iOffset = 0, iDeltaTime = 0;
    AmToneSeqHandle   hToneSeqHandle = 0;
    AmT_ToneEventData *pToneData = PNULL;
    JKSint32          s32TotalTone = 0, s32EstimateDataSize = 0;
    JKSint32          s32Duration = 0;
    
    /* analysis tone sequence data */    
    hToneSeqHandle = AmToneCreateSeqData( pData,
        (JKSint32)length,
        (JKSint32)TONE_SEQUENCE_RESOLUTION,
        m_alloc, 
        m_free );
    if ( hToneSeqHandle == JK_NULL )
    {
        JWE_LOG(toneToMidi,("err: could not get tone sequence handle"));
        return (JKSint32)JKMEDIA_RESULT_ERROR;
    }
    
    JWE_LOG(toneToMidi,("hToneSeqHandle is opened"));
    
    
    pToneData = (AmT_ToneEventData*)SCI_ALLOC_APP((JKSint32)sizeof(AmT_ToneEventData));
    
    if ( pToneData == JK_NULL )
    {
        AmToneDeleteSeqData( hToneSeqHandle );
        JWE_LOG(toneToMidi,("err: allocate pToneData"));
        return (JKSint32)JKMEDIA_RESULT_ERROR;
    }
    
    JWE_LOG(toneToMidi,("pToneData is opened"));
    
    /* count total tone number */
    s32TotalTone = 0;
    
    while( AmToneGetNextToneEvent( hToneSeqHandle, pToneData ) == AMTONE_GOT_NEXT_EVENT )
    {
        s32TotalTone ++;
    }
    
    JWE_LOG(toneToMidi,("total tone=%d",s32TotalTone));
    
    AmToneResetSeqData( hToneSeqHandle );
    
    /* malloc m_u8AudioData buffer, header+instrument+note-on/off+max delta time*2 */
    s32EstimateDataSize = (JKSint32 )sizeof(m_u8MIDIbuffer) + 7 + s32TotalTone*6 + s32TotalTone*4*2;
    
    if (( tp->u8Data = (JKUint8*)SCI_ALLOC_APP(s32EstimateDataSize)) == JK_NULL )
    {
        AmToneDeleteSeqData( hToneSeqHandle );
        SCI_FREE( pToneData );
        JWE_LOG(toneToMidi,("err: allocate m_u8AudioData %d bytes",s32EstimateDataSize));
        return (JKSint32)JKMEDIA_RESULT_ERROR;
    }
    
    JWE_LOG(toneToMidi,("allocate m_u8AudioData size=%d",s32EstimateDataSize));
    
    /* convert tone sequence data into MIDI format and put in m_u8AudioData */
    SCI_MEMCPY( (void*)tp->u8Data, (void*)m_u8MIDIbuffer, sizeof(JKUint8)*sizeof(m_u8MIDIbuffer) );
    iOffset = sizeof(m_u8MIDIbuffer); 
    
    tp->u8Data[iOffset++] = 0x00;
    tp->u8Data[iOffset++] = 0xC0;
    tp->u8Data[iOffset++] = 16; /* flute */
    
    iDeltaTime = 0;
    
    while( AmToneGetNextToneEvent( hToneSeqHandle, pToneData ) == AMTONE_GOT_NEXT_EVENT )
    {
        if ( pToneData->note == AMTONE_SILENCE )
        {
            iDeltaTime += pToneData->count;
        }
        else
        {
            /* note on */
            iOffset = writeValue( tp->u8Data, iOffset, iDeltaTime );
            tp->u8Data[iOffset++] = 0x90;
            tp->u8Data[iOffset++] = pToneData->note;
            tp->u8Data[iOffset++] = 64; /* standard */
            
            /* note-off */
            iOffset = writeValue( tp->u8Data, iOffset, pToneData->count );
            tp->u8Data[iOffset++] = 0x80;
            tp->u8Data[iOffset++] = pToneData->note;
            tp->u8Data[iOffset++] = 64; /* standard */
            
            iDeltaTime = 0;
        }
    }
    
    /* end track */
    tp->u8Data[iOffset++] = 0x00;
    tp->u8Data[iOffset++] = 0xFF;
    tp->u8Data[iOffset++] = 0x2F;
    tp->u8Data[iOffset++] = 0x00;
    
    /* write track size, little endian */
    tp->u8Data[MIDI_TRACK_SIZE_OFFSET] =   (( iOffset - MIDI_HEADER_SIZE ) >> 24 ) & 0xFF; 
    tp->u8Data[MIDI_TRACK_SIZE_OFFSET+1] = (( iOffset - MIDI_HEADER_SIZE ) >> 16 ) & 0xFF; 
    tp->u8Data[MIDI_TRACK_SIZE_OFFSET+2] = (( iOffset - MIDI_HEADER_SIZE ) >> 8  ) & 0xFF; 
    tp->u8Data[MIDI_TRACK_SIZE_OFFSET+3] = (  iOffset - MIDI_HEADER_SIZE ) & 0xFF; 
    
    tp->s32DataLen = iOffset;
    JWE_LOG(toneToMidi,("midi size = %d",tp->s32DataLen));
    
    /* get total time */
    if (( s32Duration = AmToneGetTotalTime( hToneSeqHandle )) == JKMEDIA_RESULT_ERROR)
    {
        JWE_LOG(toneToMidi,("err:duration is unknown"));        
        tp->duration = JKMEDIA_DURATION_UNKNOWN;
    }
    else
    {
	 tp->duration = ((s32Duration)<<1)>>1;
        //tp->duration = (JKUint32)s32Duration;
	JWE_LOG(toneToMidi,("s32Duration = %d  tp->duration = %d",s32Duration, tp->duration));        
    }
    
    /* release all resource */
    AmToneDeleteSeqData( hToneSeqHandle );
    SCI_FREE( pToneData );
    
    JWE_LOG(toneToMidi, ("Leave!"));    
    return (JKSint32)JKMEDIA_RESULT_OK;
}



static const JKT_MediaOption toneCtrl_OptionCtrl ={
    NULL,			    /* JKT_Media_TempoControl const * pTempoFuncs;  */
    NULL,			    /* JKT_Media_SpeedControl const * PSpeedFuncs;  */
    &mediaOpt_StopCtrl,		    /* JKT_Media_StopControl const * pStopFuncs;    */
    NULL,	    /* JKT_Media_PitchControl const * pPitchFuncs;  */
    NULL,			    /* JKT_Media_MeteDataControl const * pMetaDataFuncs;*/
    NULL,			    /* JKT_Media_MIDIControl const * pMIDIFuncs;    */
    NULL,			    /* JKT_Media_SyncControl const * pSyncFuncs;    */
    NULL,			    /* JKT_Media_VideoControl const * pVideoFuncs;  */
    NULL,			    /* JKT_Media_FrameControl const * pFrameFuncs;  */
    NULL,			    /* void const * pSnapShotFuncs;		    */
    NULL,			    /* void const * pInputDeviceFuncs;		    */
    NULL,			    /* void const * pRecordFuncs;		    */
    NULL,			    /* void const * pExtFuncs;			    */
};
    

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/**
* Structure for SMF playback control functions
*/
const JKT_MediaControl toneControl = {
    JKMEDIA_TYPE_TONE,       /* JKSint32 mediaType */
        toneCtrl_getMIMEType,    //ok/* JKSint32 ( *getMIMEType )( JKWChar** pType ); */
        toneCtrl_isData,         //ok/* JKBool ( *isData )( const JKUint8* pData, const JKSint32 length ); */
        toneCtrl_checkOpen,      //ok/* JKSint32 ( *checkOpen )( const JKUint8* pData, const JKSint32 length ); */
        toneCtrl_check,          //ok/* JKSint32 ( *check )( JKSint32 id, JKSint32* pExtraID ); */
        toneCtrl_checkClose,     //ok/* JKSint32 ( *checkClose )( JKSint32 id ); */
        mediaCtrl_getMediaSize,   //ok/* JKSint32 ( *getMediaSize )( JKSint32 id, JKSint32* pWidth, JKSint32* pHeight ); */
        toneCtrl_create,         //ok/* JKSint32 ( *create )( void ); */
        mediaCtrl_destroy,        //ok/* JKSint32 ( *destroy )( JKSint32 id ); */
        mediaCtrl_set,            //ok/* JKSint32 ( *set )( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID ); */
        mediaCtrl_load,           //ok/* JKSint32 ( *load )( JKSint32 id ); */
        mediaCtrl_start,          //ok/* JKSint32 ( *start )( JKSint32 id, JKSint32 loop ); */
        mediaCtrl_stop,           //ok/* JKSint32 ( *stop )( JKSint32 id ); */
        mediaCtrl_pause,          //ok/* JKSint32 ( *pause )( JKSint32 id ); */
        mediaCtrl_restart,        //ok/* JKSint32 ( *restart )( JKSint32 id ); */
        mediaCtrl_unload,         //ok/* JKSint32 ( *audio_unload )( JKSint32 id ); */
        mediaCtrl_getState,       //ok/* JKSint32 ( *getState )( JKSint32 id ); */
        toneCtrl_getDuration,    //ok/* JKSint32 ( *getDuration )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
        mediaCtrl_setTime,        //ok/* JKSint32 ( *setTime )( JKSint32 id, JKUint32 usecH, JKUint32 usecL ); */
        mediaCtrl_getTime,        //ok/* JKSint32 ( *getTime )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
        toneCtrl_getIncludedContents, //ok/* JKSint32 ( *getIncludedContents )( JKSint32 id ); */
        mediaCtrl_suspendVM,      //ok/* JKSint32 ( *suspend )( JKSint32 id ); */
        mediaCtrl_resumeVM,       //ok/* JKSint32 ( *resume )( JKSint32 id ); */
        JK_NULL,                 /* JKT_Media_EventControl */
        &mediaCtrl_VolumeCtrl,         //ok/* JKT_Media_VolumeControl */
        JK_NULL,                 /* JKT_Media_UserControl */
        JK_NULL,                 /* void const* pStreamFuncs */
        &toneCtrl_OptionCtrl,                 /* void const* pOptionFuncs */
};

        
            
#endif
