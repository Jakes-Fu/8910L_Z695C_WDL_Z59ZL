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

#include "jktypes.h"
#include "jkmediacontrol.h"
#include "ammediacontrol.h"
#include "ammemory.h"
#include "amtone.h"
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
LOCAL JKSint32 mp3Ctrl_getMIMEType( JKWChar **pType )
{    
    static const JKWChar mimeType[] = {'a', 'u', 'd', 'i', 'o', '/', 'm', 'p', 'e', 'g'};
    JWE_LOG(mp3Ctrl_getMIMEType, ("enter!"));
    
    *pType = ( JKWChar* )mimeType;
    JWE_LOG(mp3Ctrl_getMIMEType, ("leave!sizeof mimetype str=%d",sizeof( mimeType ) / sizeof( JKWChar )));
    return sizeof( mimeType ) / sizeof( JKWChar );
}


/**
 * Check signature in data.
 */
static JKBool mp3Ctrl_isData( const JKUint8* pData, const JKSint32 length ) 
{	
       JWE_LOG(mp3Ctrl_isData, ("enter!"));
	if ( ( pData[ 0 ] == 0xFF  &&  pData[ 1 ] == 0xFB ) ||  /* FFF =>MP3 sync word , B=>version , layer and no error correction */
	 ( pData[ 0 ] == 0xFF  &&  pData[ 1 ] == 0xFA ) ||  /* FFF =>MP3 sync word , A=>version , layer and with error correction */
	 ( pData[ 0 ] == 0x49  &&  pData[ 1 ] == 0x44 && pData[ 2 ] == 0x33 )	/* 'ID3' , tag for MP3 files */
	) 
	{
		JWE_LOG(mp3Ctrl_isData,("MP3 file"))		
		return JK_TRUE;
	}
	else
	{
		JWE_LOG(mp3Ctrl_isData,("is not MP3 file"))
		return JK_FALSE;
	}	    
}

LOCAL JKSint32 mp3Ctrl_create( void )
{
    return mediaCtrl_create(JKMEDIA_TYPE_MP3);
}

LOCAL JKSint32 mp3Ctrl_getIncludedContents( JKSint32 id )
{
    JWE_LOG(mp3Ctrl_getIncludedContents, ("enter"))
    return JKMEDIA_CONTENT_AUDIO;
}

static const JKT_MediaOption mp3Ctrl_OptionCtrl ={
    NULL,		    /* JKT_Media_TempoControl const * pTempoFuncs;  */
    NULL,  /* JKT_Media_SpeedControl const * PSpeedFuncs;  */
    &mediaOpt_StopCtrl,	    /* JKT_Media_StopControl const * pStopFuncs;    */
    NULL,		    /* JKT_Media_PitchControl const * pPitchFuncs;  */
    NULL, /* JKT_Media_MeteDataControl const * pMetaDataFuncs;*/
    NULL,		    /* JKT_Media_MIDIControl const * pMIDIFuncs;    */
    NULL,		    /* JKT_Media_SyncControl const * pSyncFuncs;    */
    NULL,		    /* JKT_Media_VideoControl const * pVideoFuncs;  */
    NULL,		    /* JKT_Media_FrameControl const * pFrameFuncs;  */
    NULL,		    /* void const * pSnapShotFuncs;		    */
    NULL,		    /* void const * pInputDeviceFuncs;		    */
    NULL,		    /* void const * pRecordFuncs;		    */
    NULL,		    /* void const * pExtFuncs;			    */
};

const JKT_MediaControl mp3Control = {
    JKMEDIA_TYPE_MP3,           /* JKSint32 mediaType */
        mp3Ctrl_getMIMEType,        /* JKSint32 ( *getMIMEType )( JKWChar** pType ); */
        mp3Ctrl_isData,             /* JKBool ( *isData )( const JKUint8* pData, const JKSint32 length ); */
        mediaCtrl_checkOpen,  /*lint !e64*/        /* JKSint32 ( *checkOpen )( const JKUint8* pData, const JKSint32 length ); */
        mediaCtrl_check,              /* JKSint32 ( *check )( JKSint32 id, JKSint32* pExtraID ); */
        mediaCtrl_checkClose,         /* JKSint32 ( *checkClose )( JKSint32 id ); */
        mediaCtrl_getMediaSize,       /* JKSint32 ( *getMediaSize )( JKSint32 id, JKSint32* pWidth, JKSint32* pHeight ); */
        mp3Ctrl_create,             /* JKSint32 ( *create )( void ); */
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
        mp3Ctrl_getIncludedContents,/* JKSint32 ( *getIncludedContents )( JKSint32 id ); */
        mediaCtrl_suspendVM,          /* JKSint32 ( *suspend )( JKSint32 id ); */
        mediaCtrl_resumeVM,           /* JKSint32 ( *resume )( JKSint32 id ); */
        JK_NULL,                     /* JKT_Media_EventControl */
        &mediaCtrl_VolumeCtrl,             /* JKT_Media_VolumeControl */
        JK_NULL,                     /* JKT_Media_UserControl */
        &mediaCtrl_ExtensionCtrl,                     /* void const* pStreamFuncs */
        &mp3Ctrl_OptionCtrl,                     /* void const* pOptionFuncs */
};




#endif
