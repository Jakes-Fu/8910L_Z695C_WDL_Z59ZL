// -- tag_jbom2_20100108_RC
/**
 * Copyright 2002-2006 Aplix Corporation. All rights reserved.
 */

/*************************************************************************
 *  TODO:    
 *  
 *
 *
 *************************************************************************/
/******************************************************************************
 * FILE: recordAmrControl.c
 * 
 * DESCRIPTION:
 *
 * UPDATED DATE: 
 *
 * REVISION:			    
 *	2007/04/14	    alexh		add due to modification of JBlend
 ******************************************************************************/
#include "jblend_config.h"



#ifdef SUPPORT_MMAPI
//#define MODULE_NAME     "recordWaveControl.c" for pclint
/*-----------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include "mmijava_internal.h"

/* header files for jblendia */
#include "jkmediacontrol.h"	    /* jkmediaxx */
#include "jkmediaoption.h"	    /* for option control */
#include "ammediacontrol.h"	    /* midp2.0 jbi funcs */

#include "mediacontrol.h"	    /* for struct player ,jkmedia_type_record_amr */

/* header files for ANSI C */
//#include <string.h>		    /* for strncmp ,strcpy */ for pclint
/*-----------------------------------------------------------------------------
 * Marcos, Types, and Structures
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * External variables
 *----------------------------------------------------------------------------*/
extern JKT_Media_RecordControl mediaOpt_RecordCtrl;
extern JKT_Media_EventControl mediaCtrl_EventCtrl;
/*-----------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * External Function Prototypes
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Local Function Prototypes
 *----------------------------------------------------------------------------*/
static JKSint32 recordWaveOpt_InputDevice_getMIMEType(JKWChar** pType);
static JKSint32 recordWaveOpt_InputDevice_setParam( JKT_Media_Param* pParam, JKSint32 paramLen );
static JKSint32 recordWaveOpt_InputDevice_open( void );
static JKSint32 recordWaveOpt_InputDevice_close( void );
/*-----------------------------------------------------------------------------
 * Global functions
 *----------------------------------------------------------------------------*/
/*===========================================================================
 *
 *  Input Device Control
 *
 ==========================================================================*/
static JKSint32 recordWaveOpt_InputDevice_getMIMEType(JKWChar** pType)
{
    static const JKWChar MIMEType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'x','-','w','a','v',
    };
    
    JWE_LOG(recordWaveOpt_InputDevice_getMIMEType,("enter"))
    *pType = ( JKWChar* )MIMEType;

    return sizeof( MIMEType ) / sizeof( JKWChar );
}
static JKSint32 recordWaveOpt_InputDevice_setParam( JKT_Media_Param* pParam, JKSint32 paramLen )
{	    
    return JKMEDIA_RESULT_OK;
}
static JKSint32 recordWaveOpt_InputDevice_open( void )
{
    JWE_LOG(recordWaveOpt_InputDevice_open,("enter"))
    /*
     *  jma_audio_recorder_open needs java_recorder id as arg
     *  postpone it until load() is called
     */
    return JKMEDIA_RESULT_OK;
}
static JKSint32 recordWaveOpt_InputDevice_close( void )
{
    JWE_LOG(recordWaveOpt_InputDevice_close,("enter"))
    return JKMEDIA_RESULT_OK;
}

 
static const JKT_Media_InputDeviceControl recordWaveOpt_InputDeviceCtrl = {
    recordWaveOpt_InputDevice_getMIMEType, /* JKSint32 ( *getMIMEType )( JKWChar** pType ) */
    recordWaveOpt_InputDevice_setParam,    /* JKSint32 ( *setParam )( JKT_Media_Param* pParam, JKSint32 paramLen ) */
    recordWaveOpt_InputDevice_open,	    /* JKSint32 ( *open )( void ); */
    recordWaveOpt_InputDevice_close	    /* JKSint32 ( *close )( void ); */
};


/**
 * Structure for option control functions
 */

static const JKT_MediaOption recordWaveCtrl_OptionCtrl ={
    NULL,				/* JKT_Media_TempoControl const * pTempoFuncs; */
    NULL,				/* JKT_Media_SpeedControl const * PSpeedFuncs; */
    NULL,				/* JKT_Media_StopControl const * pStopFuncs;   */
    NULL,				/* JKT_Media_PitchControl const * pPitchFuncs; */
    NULL,				/* JKT_Media_MeteDataControl const * pMetaDataFuncs; */
    NULL,				/* JKT_Media_MIDIControl const * pMIDIFuncs;   */
    NULL,				/* JKT_Media_SyncControl const * pSyncFuncs;   */
    NULL,				/* JKT_Media_VideoControl const * pVideoFuncs; */
    NULL,				/* JKT_Media_FrameControl const * pFrameFuncs; */
    NULL,				/* void const * pSnapShotFuncs;*/
    &recordWaveOpt_InputDeviceCtrl,	/* void const * pInputDeviceFuncs;         */
    &mediaOpt_RecordCtrl,		/* void const * pRecordFuncs;          */
    NULL,				/* void const * pExtFuncs;             */
};
//-CR248439

/* JKT_Media_VolumeControl */
/**
 * Set current volume.
 */
static JKSint32 recordWaveVolCtrl_setVolume( JKSint32 id, JKSint32 value )
{
    return JKMEDIA_RESULT_OK;
}

/**
 * Get current volume.
 */

static JKSint32 recordWaveVolCtrl_getVolume( JKSint32 id )
{
    return  0;
}

/**
 * Structure for volume control functions
 */

static const JKT_Media_VolumeControl volumeControl = {
    recordWaveVolCtrl_setVolume,
    recordWaveVolCtrl_getVolume
};

/**
 * Get MIME-type string.
 */
static JKSint32 recordWaveCtrl_getMIMEType( JKWChar** pType )
{
    static const JKWChar mimeType[] = {
        'a', 'u', 'd', 'i', 'o',   /* for capture://audio */
    };
    *pType = ( JKWChar* )mimeType;
    JWE_LOG(recordWaveCtrl_getMIMEType, ("MIME is call"))
    return sizeof( mimeType ) / sizeof( JKWChar );
}

/**
 * Check signature in data.
 */
static JKBool recordWaveCtrl_isData( const JKUint8* pData, const JKSint32 length )
{
    JWE_LOG(recordWaveCtrl_isData,("enter"))
    return JK_TRUE;
}

/**
 * Open media for data validity check.
 */
JKSint32 recordWaveCtrl_checkOpen( const JKUint8* pData, const JKSint32 length )
{
    return mediaCtrl_checkOpen(pData,length,JKMEDIA_TYPE_RECORD_WAVE);
}


/**
 * Create a player.
 */
JKSint32 recordWaveCtrl_create( void )
{
    JWE_LOG(recordAmrCtrl_create,("enter JKMEDIA_TYPE_RECORD_AMR=%d",JKMEDIA_TYPE_RECORD_AMR))
    return mediaCtrl_create(JKMEDIA_TYPE_RECORD_WAVE);
}


/**
 * Structure for SMF playback control functions
 */

const JKT_MediaControl recordWaveControl = {
    JKMEDIA_TYPE_RECORD_WAVE,		/* JKSint32 mediaType */
    recordWaveCtrl_getMIMEType,		/* JKSint32 ( *getMIMEType )( JKWChar** pType ); */
    recordWaveCtrl_isData,		    /* JKBool ( *isData )( const JKUint8* pData, const JKSint32 length ); */
    recordWaveCtrl_checkOpen,		/* JKSint32 ( *checkOpen )( const JKUint8* pData, const JKSint32 length ); */
    mediaCtrl_check,			    /* JKSint32 ( *check )( JKSint32 id, JKSint32* pExtraID ); */
    mediaCtrl_checkClose,		    /* JKSint32 ( *checkClose )( JKSint32 id ); */
    mediaCtrl_getMediaSize,		    /* JKSint32 ( *getMediaSize )( JKSint32 id, JKSint32* pWidth, JKSint32* pHeight ); */
    recordWaveCtrl_create,		    /* JKSint32 ( *create )( void ); */
    mediaCtrl_destroy,			    /* JKSint32 ( *destroy )( JKSint32 id ); */
    mediaCtrl_set,			        /* JKSint32 ( *set )( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID ); */
    mediaCtrl_load,			        /* JKSint32 ( *load )( JKSint32 id ); */
    mediaCtrl_start,			    /* JKSint32 ( *start )( JKSint32 id, JKSint32 loop ); */
    mediaCtrl_stop,			        /* JKSint32 ( *stop )( JKSint32 id ); */
    mediaCtrl_pause,			    /* JKSint32 ( *pause )( JKSint32 id ); */
    mediaCtrl_restart,			    /* JKSint32 ( *restart )( JKSint32 id ); */
    mediaCtrl_unload,			    /* JKSint32 ( *unload )( JKSint32 id ); */
    mediaCtrl_getState,			    /* JKSint32 ( *getState )( JKSint32 id ); */
    mediaCtrl_getDuration,	        /* JKSint32 ( *getDuration )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
    mediaCtrl_setTime,	            /* JKSint32 ( *setTime )( JKSint32 id, JKUint32 usecH, JKUint32 usecL ); */
    mediaCtrl_getRecordTime,		/* JKSint32 ( *getTime )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
    mediaCtrl_getIncludedContents,	/* JKSint32 ( *getIncludedContents )( JKSint32 id ); */
    mediaCtrl_suspendVM,		    /* JKSint32 ( *suspend )( JKSint32 id ); */
    mediaCtrl_resumeVM,			    /* JKSint32 ( *resume )( JKSint32 id ); */

    &mediaCtrl_EventCtrl,		    /* JKT_Media_EventControl */
    JK_NULL,				        /* JKT_Media_VolumeControl */
    JK_NULL,				        /* JKT_Media_UserControl */
    JK_NULL,				        /* void const* pExtFuncs */
    &recordWaveCtrl_OptionCtrl,	    /* void const* pOptionFuncs */
};
#endif	/* SUPPORT_MMAPI */


