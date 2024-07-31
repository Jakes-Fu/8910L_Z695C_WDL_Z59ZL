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
//#include "jblend_config.h"

#include "jwe_log.h"
#include "jkmediacontrol.h"
#include "media_tonecontrol.h"
#include "mediacontrol.h"
#include "os_api.h"

/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern const JKT_MediaControl toneControl;
#ifdef MIDI_SUPPORT
extern const JKT_MediaControl midiControl;
extern const JKT_MediaControl spmidiControl;
#endif
#ifdef WAV_SUPPORT
extern const JKT_MediaControl waveControl;
#endif
#ifdef MP3_SUPPORT
extern const JKT_MediaControl mp3Control;
#endif
#ifdef AMR_SUPPORT
extern const JKT_MediaControl amrControl;    
#endif
#ifdef AAC_SUPPORT
extern const JKT_MediaControl aacControl;
#endif
//extern const JKT_MediaControl m4aControl;  
//extern const JKT_MediaControl mpegAudioControl;
//extern const JKT_MediaControl h263AudioControl;
//extern const JKT_MediaControl mpegControl;
//extern const JKT_MediaControl h263Control;
#ifdef AVI_DEC_SUPPORT
//extern const JKT_MediaControl aviControl;    
#endif
//extern const JKT_MediaControl gifControl;
#ifdef JAVA_SUPPORT_MEDIA_CAMERA    
extern const JKT_MediaControl cameraControl;  
#endif
//extern const JKT_MediaControl recordWaveControl;   
#ifdef JAVA_SUPPORT_MEDIA_RECORD  
extern const JKT_MediaControl recordWaveControl;  
extern const JKT_MediaControl recordAmrControl;
#endif
//extern const JKT_MediaControl recordH263Control;  

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/* List of all supported media types */
static const JKSint32 m_supportTypes[] = {            
       JKMEDIA_TYPE_TONE,    /* ToneControl .jts*/
#ifdef MIDI_SUPPORT
       JKMEDIA_TYPE_MIDI,    /* MidiControl .mid*/
       JKMEDIA_TYPE_SPMIDI,
#endif
#ifdef WAV_SUPPORT
       JKMEDIA_TYPE_WAVE,     /* WaveControl .wav*/
#endif
#ifdef MP3_SUPPORT
	JKMEDIA_TYPE_MP3,
#endif
#ifdef AMR_SUPPORT
	JKMEDIA_TYPE_AMR,
#endif
#ifdef AAC_SUPPORT
	JKMEDIA_TYPE_AAC,
#endif
//	JKMEDIA_TYPE_M4A,
	
	//JKMEDIA_TYPE_MPEG_AUDIO,
	//JKMEDIA_TYPE_H263_AUDIO,
	
//	JKMEDIA_TYPE_MPEG,	/* MpegControl .mp4*/
//	JKMEDIA_TYPE_H263,	/* H263Control .3gp*/
#ifdef AVI_DEC_SUPPORT
//	JKMEDIA_TYPE_AVI,	/* AviControl .avi*/
#endif
//	JKMEDIA_TYPE_GIF,	/* GifControl .gif*/
#ifdef JAVA_SUPPORT_MEDIA_CAMERA    
	JKMEDIA_TYPE_CAMERA,	/* CameraControl .jpeg*/
#endif

//	JKMEDIA_TYPE_RECORD_WAVE,		/* recordWaveControl .wav*/
#ifdef JAVA_SUPPORT_MEDIA_RECORD
	JKMEDIA_TYPE_RECORD_WAVE,
	JKMEDIA_TYPE_RECORD_AMR,		/* recordAmrControl .amr*/
#endif
//	JKMEDIA_TYPE_RECORD_H263,   	/* recordH263Control .3gp*/
	0                    /* dummy terminator*/
};


/* List of function structures for all suported media types */
static const JKT_MediaControl *m_funcs[] = {
       &toneControl,
#ifdef MIDI_SUPPORT
       &midiControl,
       &spmidiControl,
#endif
#ifdef WAV_SUPPORT
       &waveControl,
#endif
#ifdef MP3_SUPPORT
       &mp3Control,
#endif
#ifdef AMR_SUPPORT
	&amrControl,
#endif
#ifdef AAC_SUPPORT
	&aacControl,
#endif
//	&m4aControl,
	
	//&mpegAudioControl,
	//&h263AudioControl,
	
//	&mpegControl,
//	&h263Control,
#ifdef AVI_DEC_SUPPORT
//	&aviControl,
#endif
//	&gifControl,
#ifdef JAVA_SUPPORT_MEDIA_CAMERA    
	&cameraControl,
#endif
//	&recordWaveControl,
#ifdef JAVA_SUPPORT_MEDIA_RECORD  
	&recordWaveControl,
    &recordAmrControl,
#endif
//	&recordH263Control,

    JK_NULL                    /*dummy terminator */
};
	

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : JkMediaControlGetSupportedTypes
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
JKSint32* JkMediaControlGetSupportedTypes( JKSint32* pNumTypes )
{
    JWE_LOG(JkMediaControlGetSupportedTypes, ("enter"));
    *pNumTypes = ( sizeof(m_supportTypes)/sizeof(JKSint32) ) - 1;
    JWE_LOG(JkMediaControlGetSupportedTypes, ("leave NumTypes=%d",*pNumTypes));
    return ( JKSint32* )m_supportTypes;
}

/*****************************************************************************/
//  Description : JkMediaControlGetSupportedProfiles
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
JKSint32 JkMediaControlGetSupportedProfiles( JKSint32 mediaType )
{
    JKSint32 profiles = 0;
    JWE_LOG(JkMediaControlGetSupportedProfiles, ("enter mediaType = %d",mediaType));
    profiles = JKMEDIA_PROFILE_MMAPI;
    return profiles;
}

/*****************************************************************************/
//  Description : JkMediaControlGetController
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
JKT_MediaControl* JkMediaControlGetController( JKSint32 mediaType )
{
    int32 i = 0;
    JWE_LOG(JkMediaControlGetController, ("enter mediaType = %d",mediaType));
    for ( i = 0; i < (int32)( sizeof(m_supportTypes)/sizeof(JKSint32) ) - 1 ; i++ ) 
    {
        if ( m_funcs[i]->mediaType == mediaType ) 
        {
           
            return (JKT_MediaControl *)( m_funcs[i] );
        }
    }
    JWE_LOG(JkMediaControlGetController, ("no mediaControl for this mediaType,return null"));
    return ( JKT_MediaControl* )JK_NULL;
}

/*****************************************************************************/
//  Description : JkMediaControlDestroyAdditionalInfo
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void JkMediaControlDestroyAdditionalInfo( JKSint32 mediaType, JKSint32 extraID )
{
    JWE_LOG(JkMediaControlDestroyAdditionalInfo, ("enter mediaType=%d extraID=%d",mediaType,extraID));
    return;
}
/*****************************************************************************/
//  Description : JkMediaControlNotifyDeviceReleased
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void JkMediaControlNotifyDeviceReleased( JKSint32 mediaType )
{
    JWE_LOG(JkMediaControlNotifyDeviceReleased, ("enter mediaType=%d",mediaType));
    return;
}

#endif