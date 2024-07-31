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
#include "mediacontrol.h"
#include "ammemory.h"
#include "audioclient.h"
#include "jwe_log.h"
#include "jkmediastream.h"
#include "jkmediaextension.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern JKT_Media_VolumeControl mediaCtrl_VolumeCtrl;
extern JKT_Media_StopControl mediaOpt_StopCtrl;
extern JKT_MediaExtension mediaCtrl_ExtensionCtrl;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL JKSint32 midiCtrl_getMIMEType( JKWChar **pType )
{
    /* "audio/midi" */
    static const JKWChar mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'm', 'i', 'd', 'i'
    };
    JWE_LOG(midiCtrl_getMIMEType, ("enter"));
    
    *pType = ( JKWChar* )mimeType;
    JWE_LOG(midiCtrl_getMIMEType, ("leave!sizeof mimetype str=%d",sizeof( mimeType ) / sizeof( JKWChar )));
    return sizeof( mimeType ) / sizeof( JKWChar );
}

LOCAL JKSint32 spmidiCtrl_getMIMEType( JKWChar **pType )
{
    /* "audio/midi" */
    static const JKWChar mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 's','p','-','m', 'i', 'd', 'i'
    };
    JWE_LOG(spmidiCtrl_getMIMEType, ("enter"));
    
    *pType = ( JKWChar* )mimeType;
    JWE_LOG(spmidiCtrl_getMIMEType, ("leave!sizeof mimetype str=%d",sizeof( mimeType ) / sizeof( JKWChar )));
    return sizeof( mimeType ) / sizeof( JKWChar );
}

LOCAL JKBool midiCtrl_isData( const JKUint8 *pData, const JKSint32 length )
{
    JWE_LOG(midiCtrl_isData, ("enter!, length = %d", length ));
    
    if ( length < 4 ) 
    {
        return JK_FALSE;
    }
    
    if ( ( pData[ 0 ] != 0x4d ) /* 'M' */
        || ( pData[ 1 ] != 0x54 ) /* 'T' */
        || ( pData[ 2 ] != 0x68 ) /* 'h' */
        || ( pData[ 3 ] != 0x64 ) /* 'd' */
        ) 
    {
        JWE_LOG(midiCtrl_isData, ("leave false"));
        return JK_FALSE;
    }
    JWE_LOG(midiCtrl_isData, ("leave true"));
    return JK_TRUE;
}


LOCAL JKSint32 midiCtrl_checkOpen( const JKUint8 *pData, const JKSint32 length )
{
    JWE_LOG(midiCtrl_checkOpen,("enter"));
    return mediaCtrl_checkOpen(pData,length,JKMEDIA_TYPE_MIDI);
}

JKSint32 spmidiCtrl_checkOpen( const JKUint8* pData, const JKSint32 length ) 
{
    JWE_LOG(spmidiCtrl_checkOpen,("enter"))
    return mediaCtrl_checkOpen(pData,length,JKMEDIA_TYPE_SPMIDI);
}

LOCAL JKSint32 midiCtrl_create( void )
{
    return mediaCtrl_create(JKMEDIA_TYPE_MIDI);
}

LOCAL JKSint32 spmidiCtrl_create( void )
{
    return mediaCtrl_create(JKMEDIA_TYPE_SPMIDI);
}

LOCAL JKSint32 midiCtrl_getIncludedContents( JKSint32 id )
{
    JWE_LOG(midiCtrl_getIncludedContents, ("enter"));
    return JKMEDIA_CONTENT_AUDIO;
}

LOCAL JKSint32 spmidiCtrl_getIncludedContents( JKSint32 id )
{
    JWE_LOG(spmidiCtrl_getIncludedContents, ("enter"));
    return JKMEDIA_CONTENT_AUDIO;
}

static const JKT_MediaOption midiCtrl_OptionCtrl ={
    NULL,	    /* JKT_Media_TempoControl const * pTempoFuncs;  */
    NULL,	    /* JKT_Media_SpeedControl const * PSpeedFuncs;  */
    &mediaOpt_StopCtrl,		    /* JKT_Media_StopControl const * pStopFuncs;    */
    NULL,	    /* JKT_Media_PitchControl const * pPitchFuncs;  */
    NULL,	    /* JKT_Media_MeteDataControl const * pMetaDataFuncs;*/
    NULL,		    /* JKT_Media_MIDIControl const * pMIDIFuncs;    */
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
const JKT_MediaControl midiControl = {
    JKMEDIA_TYPE_MIDI,           //ok/* JKSint32 mediaType */
        midiCtrl_getMIMEType,       //ok /* JKSint32 ( *getMIMEType )( JKWChar** pType ); */
        midiCtrl_isData,            //ok /* JKBool ( *isData )( const JKUint8* pData, const JKSint32 length ); */
        midiCtrl_checkOpen,          //ok/* JKSint32 ( *checkOpen )( const JKUint8* pData, const JKSint32 length ); */
        mediaCtrl_check,              //ok/* JKSint32 ( *check )( JKSint32 id, JKSint32* pExtraID ); */
        mediaCtrl_checkClose,         //ok/* JKSint32 ( *checkClose )( JKSint32 id ); */
        mediaCtrl_getMediaSize,       //ok/* JKSint32 ( *getMediaSize )( JKSint32 id, JKSint32* pWidth, JKSint32* pHeight ); */
        midiCtrl_create,             //ok/* JKSint32 ( *create )( void ); */
        mediaCtrl_destroy,            //ok/* JKSint32 ( *destroy )( JKSint32 id ); */
        mediaCtrl_set,                //ok/* JKSint32 ( *set )( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID ); */
        mediaCtrl_load,               //ok/* JKSint32 ( *load )( JKSint32 id ); */
        mediaCtrl_start,              //ok/* JKSint32 ( *start )( JKSint32 id, JKSint32 loop ); */
        mediaCtrl_stop,               //ok/* JKSint32 ( *stop )( JKSint32 id ); */
        mediaCtrl_pause,             //ok /* JKSint32 ( *pause )( JKSint32 id ); */
        mediaCtrl_restart,            /* JKSint32 ( *restart )( JKSint32 id ); */
        mediaCtrl_unload,            //ok /* JKSint32 ( *audio_unload )( JKSint32 id ); */
        mediaCtrl_getState,           //ok/* JKSint32 ( *getState )( JKSint32 id ); */
        mediaCtrl_getDuration,        //spreadtrum/* JKSint32 ( *getDuration )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
        mediaCtrl_setTime,            //spreadtrum/* JKSint32 ( *setTime )( JKSint32 id, JKUint32 usecH, JKUint32 usecL ); */
        mediaCtrl_getTime,            //spreadtrum/* JKSint32 ( *getTime )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
        midiCtrl_getIncludedContents,//ok/* JKSint32 ( *getIncludedContents )( JKSint32 id ); */
        mediaCtrl_suspendVM,          /* JKSint32 ( *suspend )( JKSint32 id ); */
        mediaCtrl_resumeVM,           /* JKSint32 ( *resume )( JKSint32 id ); */
        JK_NULL,                     /* JKT_Media_EventControl */
        &mediaCtrl_VolumeCtrl,             /* JKT_Media_VolumeControl */
        JK_NULL,                     /* JKT_Media_UserControl */
        JK_NULL,                     /* void const* pStreamFuncs */
        &midiCtrl_OptionCtrl,                     /* void const* pOptionFuncs */
};

const JKT_MediaControl spmidiControl = {
    JKMEDIA_TYPE_SPMIDI,		    /* JKSint32 mediaType */
    spmidiCtrl_getMIMEType,	    /* JKSint32 ( *getMIMEType )( JKWChar** pType ); */
    midiCtrl_isData,		    /* JKBool ( *isData )( const JKUint8* pData, const JKSint32 length ); */
    spmidiCtrl_checkOpen,		    /* JKSint32 ( *checkOpen )( const JKUint8* pData, const JKSint32 length ); */
    mediaCtrl_check,		    /* JKSint32 ( *check )( JKSint32 id, JKSint32* pExtraID ); */
    mediaCtrl_checkClose,	    /* JKSint32 ( *checkClose )( JKSint32 id ); */
    mediaCtrl_getMediaSize,	    /* JKSint32 ( *getMediaSize )( JKSint32 id, JKSint32* pWidth, JKSint32* pHeight ); */
    spmidiCtrl_create,		    /* JKSint32 ( *create )( void ); */
    mediaCtrl_destroy,		    /* JKSint32 ( *destroy )( JKSint32 id ); */
    mediaCtrl_set,		    /* JKSint32 ( *set )( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID ); */
    mediaCtrl_load,		    /* JKSint32 ( *load )( JKSint32 id ); */
    mediaCtrl_start,		    /* JKSint32 ( *start )( JKSint32 id, JKSint32 loop ); */
    mediaCtrl_stop,		    /* JKSint32 ( *stop )( JKSint32 id ); */
    mediaCtrl_pause,		    /* JKSint32 ( *pause )( JKSint32 id ); */
    mediaCtrl_restart,		    /* JKSint32 ( *restart )( JKSint32 id ); */
    mediaCtrl_unload,		    /* JKSint32 ( *unload )( JKSint32 id ); */
    mediaCtrl_getState,		    /* JKSint32 ( *getState )( JKSint32 id ); */
    mediaCtrl_getDuration,	    /* JKSint32 ( *getDuration )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
    mediaCtrl_setTime,		    /* JKSint32 ( *setTime )( JKSint32 id, JKUint32 usecH, JKUint32 usecL ); */
    mediaCtrl_getTime,		    /* JKSint32 ( *getTime )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
    spmidiCtrl_getIncludedContents,  /* JKSint32 ( *getIncludedContents )( JKSint32 id ); */
    mediaCtrl_suspendVM,	    /* JKSint32 ( *suspend )( JKSint32 id ); */
    mediaCtrl_resumeVM,		    /* JKSint32 ( *resume )( JKSint32 id ); */
    JK_NULL,			    /* JKT_Media_EventControl */
    &mediaCtrl_VolumeCtrl,       /* JKT_Media_Volume */
    JK_NULL,			    /* JKT_Media_UserControl */
    &mediaCtrl_ExtensionCtrl,				/* void const* pStreamFuncs */
    &midiCtrl_OptionCtrl,				 /* void const* pOptionFuncs */
};


#endif
