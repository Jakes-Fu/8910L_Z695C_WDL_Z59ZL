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
#include "jkmediacontrol.h"
#include "ammediacontrol.h"
#include "jwe_log.h"

#include "mediacontrol.h"

#include "jkmediastream.h"
#include "jkmediaextension.h"
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern JKT_Media_VolumeControl mediaCtrl_VolumeCtrl;
extern JKT_Media_StopControl mediaOpt_StopCtrl;
extern JKT_MediaExtension mediaCtrl_ExtensionCtrl;


static JKSint32 waveCtrl_getMIMEType( JKWChar** pType ) 
{
    static const JKWChar mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'x', '-', 'w', 'a' , 'v',
    };
    JWE_LOG(waveCtrl_getMIMEType, ("enter"))
    *pType = ( JKWChar* )mimeType;
    JWE_LOG(waveCtrl_getMIMEType, ("leave!sizeof mimetype str=%d",sizeof( mimeType ) / sizeof( JKWChar )));
    return sizeof( mimeType ) / sizeof( JKWChar );
}

static JKBool waveCtrl_isData( const JKUint8* pData, const JKSint32 length ) 
{	
    JWE_LOG(waveCtrl_isData,("enter"))
    if ( pData[0]  == (JKUint8)'R' && pData[1]  == (JKUint8)'I' && 
         pData[2]  == (JKUint8)'F' && pData[3]  == (JKUint8)'F' && 
         pData[8]  == (JKUint8)'W' && pData[9]  == (JKUint8)'A' && 
         pData[10] == (JKUint8)'V' && pData[11] == (JKUint8)'E' && 
         pData[12] == (JKUint8)'f' && pData[13] == (JKUint8)'m' &&
         pData[14] == (JKUint8)'t' ){
	    JWE_LOG(waveCtrl_isData,("is Wave File"))
	    return JK_TRUE;
    }

    else{
	    JWE_LOG(waveCtrl_isData,("not Wave File"))
	    return JK_FALSE;			    
    }
}

LOCAL JKSint32 waveCtrl_create( void )
{
    return mediaCtrl_create(JKMEDIA_TYPE_WAVE);
}



LOCAL JKSint32 waveCtrl_getIncludedContents( JKSint32 id )
{
    JWE_LOG(mp3Ctrl_getIncludedContents, ("enter"))
    return JKMEDIA_CONTENT_AUDIO;
}


static const JKT_MediaOption waveCtrl_OptionCtrl={
    NULL,			        /* JKT_Media_TempoControl const * pTempoFuncs;  */
    NULL,			        /* JKT_Media_SpeedControl const * PSpeedFuncs;  */
    &mediaOpt_StopCtrl,		/* JKT_Media_StopControl const * pStopFuncs;    */
    NULL,			        /* JKT_Media_PitchControl const * pPitchFuncs;  */
    NULL,	/* JKT_Media_MeteDataControl const * pMetaDataFuncs;*/
    NULL,			        /* JKT_Media_MIDIControl const * pMIDIFuncs;    */
    NULL,			        /* JKT_Media_SyncControl const * pSyncFuncs;    */
    NULL,			        /* JKT_Media_VideoControl const * pVideoFuncs;  */
    NULL,			        /* JKT_Media_FrameControl const * pFrameFuncs;  */
    NULL,			        /* void const * pSnapShotFuncs;		    */
    NULL,			        /* void const * pInputDeviceFuncs;		    */
    NULL,			        /* void const * pRecordFuncs;		    */
    NULL,			        /* void const * pExtFuncs;			    */
};

const JKT_MediaControl waveControl = {
    JKMEDIA_TYPE_WAVE,           /* JKSint32 mediaType */
        waveCtrl_getMIMEType,        /* JKSint32 ( *getMIMEType )( JKWChar** pType ); */
        waveCtrl_isData,             /* JKBool ( *isData )( const JKUint8* pData, const JKSint32 length ); */
        mediaCtrl_checkOpen, /*lint !e64*/         /* JKSint32 ( *checkOpen )( const JKUint8* pData, const JKSint32 length ); */
        mediaCtrl_check,              /* JKSint32 ( *check )( JKSint32 id, JKSint32* pExtraID ); */
        mediaCtrl_checkClose,         /* JKSint32 ( *checkClose )( JKSint32 id ); */
        mediaCtrl_getMediaSize,       /* JKSint32 ( *getMediaSize )( JKSint32 id, JKSint32* pWidth, JKSint32* pHeight ); */
        waveCtrl_create,             /* JKSint32 ( *create )( void ); */
        mediaCtrl_destroy,            /* JKSint32 ( *destroy )( JKSint32 id ); */
        mediaCtrl_set,                /* JKSint32 ( *set )( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID ); */
        mediaCtrl_load,               /* JKSint32 ( *load )( JKSint32 id ); */
        mediaCtrl_start,              /* JKSint32 ( *start )( JKSint32 id, JKSint32 loop ); */
        mediaCtrl_stop,               /* JKSint32 ( *stop )( JKSint32 id ); */
        mediaCtrl_pause,              /* JKSint32 ( *pause )( JKSint32 id ); */
        mediaCtrl_restart,            /* JKSint32 ( *restart )( JKSint32 id ); */
        mediaCtrl_unload,             /* JKSint32 ( *audio_unload )( JKSint32 id ); */
        mediaCtrl_getState,           /* JKSint32 ( *getState )( JKSint32 id ); */
        mediaCtrl_getDuration,        /* JKSint32 ( *getDuration )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
        mediaCtrl_setTime,            /* JKSint32 ( *setTime )( JKSint32 id, JKUint32 usecH, JKUint32 usecL ); */
        mediaCtrl_getTime,            /* JKSint32 ( *getTime )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
        waveCtrl_getIncludedContents,/* JKSint32 ( *getIncludedContents )( JKSint32 id ); */
        mediaCtrl_suspendVM,          /* JKSint32 ( *suspend )( JKSint32 id ); */
        mediaCtrl_resumeVM,           /* JKSint32 ( *resume )( JKSint32 id ); */
        JK_NULL,                     /* JKT_Media_EventControl */
        &mediaCtrl_VolumeCtrl,             /* JKT_Media_VolumeControl */
        JK_NULL,                     /* JKT_Media_UserControl */
        &mediaCtrl_ExtensionCtrl,                     /* void const* pStreamFuncs */
        &waveCtrl_OptionCtrl,                     /* void const* pOptionFuncs */
};


#endif
