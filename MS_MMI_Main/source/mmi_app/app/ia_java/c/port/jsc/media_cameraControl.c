/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

/*****************************************************************************
** File Name:      media_cameraControl.c                                       *
** Description:    This file is used to implement digital camera function             *
******************************************************************************/

#include "jblend_config.h"

#ifdef JAVA_SUPPORT_MEDIA_CAMERA    

/*-----------------------------------------------------------------------------
 * Include Files
 *-----------------------------------------------------------------------------*/
#include "jkmediacontrol.h"	    /* jkmediaxx */
#include "mediacontrol.h"	    /* for struct player ,jkmedia_type_camera */
#include "mmicamera_manager.h"
#include "jwe_log.h"
#include "guilcd.h"
#include "kjava_sys_gfx.h"

#include "mmijava_export.h"

/*-----------------------------------------------------------------------------
 * Marcos, Types, and Structures
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * External variables
 *----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

LOCAL void *m_snapshot_buff=NULL;
LOCAL JKBool camera_isopen = FALSE;
LOCAL int is_waiting = 1;
LOCAL void* jpeg_buf = PNULL;
LOCAL uint32 jpeg_size= 0;
JKSint32 visialbe_id = -1;


/*-----------------------------------------------------------------------------
 * External Function Prototypes
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Local Function Prototypes
 *----------------------------------------------------------------------------*/
static JKSint32 cameraOpt_Video_stopUpdate( JKSint32 id );

/*-----------------------------------------------------------------------------
 * Global functions
 *----------------------------------------------------------------------------*/
/**
 * Get MIME-type string.
 */
static JKSint32 cameraCtrl_getMIMEType( JKWChar** pType ) 
{
    static const JKWChar mimeType[] = {
        'v', 'i', 'd', 'e', 'o'
    };
    *pType = ( JKWChar* )mimeType;
    JWE_LOG(cameraCtrl_getMIMEType, ("MIME is call"))
    
    return sizeof( mimeType ) / sizeof( JKWChar );
}

/**
 * Check signature in data.
 */
static JKBool cameraCtrl_isData( const JKUint8* pData, const JKSint32 length ) 
{	
    JWE_LOG(cameraCtrl_isData,("enter"))
    return JK_TRUE;
}

/**
 * Open media for data validity check.
 */
JKSint32 cameraCtrl_checkOpen( const JKUint8* pData, const JKSint32 length ) 
{
    JWE_LOG(cameraCtrl_checkOpen,("enter"))
    return mediaCtrl_checkOpen(pData,length,JKMEDIA_TYPE_CAMERA);
}

/**
 * Create a player.
 */
JKSint32 cameraCtrl_create( void ) 
{
    return mediaCtrl_create(JKMEDIA_TYPE_CAMERA);
}

/**
 * Get whether the media have images or sound.
 */
static JKSint32 cameraCtrl_getIncludedContents( JKSint32 id ) 
{
    JWE_LOG(cameraCtrl_getIncludedContents, ("enter"))
    return JKMEDIA_CONTENT_VIDEO;
}


JKSint32 cameraCtrl_getMediaSize( JKSint32 checkID, JKSint32* pWidth, JKSint32* pHeight )
{
    KJavaGfxInfo    kjavaGfxInfo = {0};

	//GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,(uint16 *) pWidth,( uint16 *) pHeight);
	//+CRNEWMS00111166
	#if 0 //PDA工程有问题 
	if(*pWidth<0||*pWidth>240)
	  *pWidth = 240;
	if(*pHeight<0||*pHeight>320)
	  *pHeight = 320;
    #endif
    kjava_gfx_requestScreenMode(KJAVA_VIDEO_SCREEN_FULL);
    kjava_gfx_getInfo(&kjavaGfxInfo);
    *pWidth = kjavaGfxInfo.viewRegion.width;
    *pHeight= kjavaGfxInfo.viewRegion.height;
	JWE_LOG(cameraCtrl_getMediaSize,("222 pWidth=%d, pHeight=%d",*pWidth,*pHeight));
	//-CRNEWMS00111166
//    *pWidth = 240;
//    *pHeight = 320;
    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_set( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID )
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraCtrl_set,("enter,player id=%d,pData=0x%08x length=%d extraID=%d",id,pData,length,extraID));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraCtrl_set,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    if( pPlayer->media_type == JKMEDIA_TYPE_CAMERA) {
        if ( !pData ) {/*set(null) close native mmapi handle.free platform resources*/
            JWE_LOG(cameraCtrl_set,("close native player")); 
            pPlayer->status = JKMEDIA_STATE_NODATA;
            return JKMEDIA_RESULT_OK;
        }
        /* if(!pData) */
        else{
            pPlayer->status = JKMEDIA_STATE_DATA_SET;

            return JKMEDIA_RESULT_OK;
        }
    }
    else
    {
        return JKMEDIA_RESULT_ERROR;
    }
}

JKSint32 cameraCtrl_load( JKSint32 id ) 
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraCtrl_load,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraCtrl_load,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    pPlayer->status = JKMEDIA_STATE_READY;

    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_start( JKSint32 id, JKSint32 loop )
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraCtrl_start,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraCtrl_start,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    pPlayer->status = JKMEDIA_STATE_STARTED;

    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_stop( JKSint32 id ) 
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraCtrl_stop,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraCtrl_stop,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
    pPlayer->status = JKMEDIA_STATE_READY;

    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_pause( JKSint32 id ) 
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraCtrl_pause,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraCtrl_pause,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }

    if(pPlayer->preview_isvisible == TRUE && camera_isopen == TRUE)
    {
        if(!MMICamera_StopDCPreview())
        {
            JWE_LOG(cameraOpt_Video_stopUpdate, ("MMICamera_StopDCPreview error"));
            return JKMEDIA_RESULT_ERROR;
        }
    }

    pPlayer->status = JKMEDIA_STATE_PAUSED;

    JWE_LOG(cameraCtrl_pause,("leave"));

    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_restart( JKSint32 id ) 
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraCtrl_restart,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraCtrl_restart,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    if(camera_isopen == FALSE)
    {
        if(!MMICamera_OpenDC())
        {
            JWE_LOG(cameraCtrl_restart, ("MMICamera_OpenDC error"));
            return JKMEDIA_RESULT_ERROR;
        }

        if(!MMICamera_InitDCPreviewParam())
        {
            JWE_LOG(cameraCtrl_restart, ("MMICamera_InitDCPreviewParam error"));
            return JKMEDIA_RESULT_ERROR;
        }

        camera_isopen = TRUE;
    }

    if(pPlayer->preview_isvisible == TRUE)
    {
        if(!MMICamera_StartDCPreview())
        {
            JWE_LOG(cameraCtrl_restart, ("MMICamera_StartDCPreview error"));
            return JKMEDIA_RESULT_ERROR;
        }
    }

    pPlayer->status = JKMEDIA_STATE_STARTED;
    JWE_LOG(cameraCtrl_restart,("leave"));
    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_unload( JKSint32 id ) 
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraCtrl_unload,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraCtrl_unload,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    pPlayer->status = JKMEDIA_STATE_DATA_SET;

    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_getState( JKSint32 id ) 
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraCtrl_getState,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraCtrl_getState,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    return pPlayer->status;
}

JKSint32 cameraCtrl_getDuration( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL )
{
    *pUsecH = JKMEDIA_TIME_UNKNOWN;
    *pUsecL = JKMEDIA_TIME_UNKNOWN;
    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_setTime( JKSint32 id, JKUint32 usecH, JKUint32 usecL )
{
    return JKMEDIA_RESULT_OK;
}

 JKSint32 cameraCtrl_getRecordTime( JKSint32 id, JKUint32* usecH, JKUint32* usecL )
{
    *usecH = JKMEDIA_TIME_UNKNOWN;// for pclint
    *usecL = JKMEDIA_TIME_UNKNOWN;//for pclint
    return JKMEDIA_RESULT_OK;
}

JKSint32 cameraCtrl_destroy( JKSint32 id )
{
    T_mediaPlayer *pPlayer = getPlayer( id );
    
    JWE_LOG(cameraCtrl_destroy, ("enter player id=%d",id));
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(cameraCtrl_destroy, ("find id error "));
        return JKMEDIA_RESULT_ERROR;
    }

    cameraOpt_Video_stopUpdate(id);

/*
    if(pPlayer->preview_isvisible == TRUE && camera_isopen == TRUE)
    {
        if(!MMICamera_StopDCPreview())
        {
            JWE_LOG(cameraCtrl_destroy, ("MMICamera_StopDCPreview error"));
            return JKMEDIA_RESULT_ERROR;
        }
    }

    if(camera_isopen == TRUE)
    {
        if(!MMICamera_CloseDC())
        {
            JWE_LOG(cameraCtrl_destroy, ("MMICamera_CloseDC error"));
            return JKMEDIA_RESULT_ERROR;
        }
        camera_isopen = FALSE;
    }
*/
    JWE_LOG(cameraCtrl_destroy, ("leave! ok "));      
    return destroyPlayer( pPlayer->id );

}

JKSint32 cameraCtrl_suspendVM( JKSint32 id ) 
{
    //JKSint32 status = 0;

    JKSint32 ret = JKMEDIA_RESULT_ERROR;
    T_mediaPlayer *pPlayer = PNULL;
    JWE_LOG(cameraCtrl_suspendVM, ("enter id=%d",id));
    
    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(cameraCtrl_suspendVM, ("leave! null error "));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    if(camera_isopen == TRUE)
    {
        if(!MMICamera_StopDCPreview())
        {
            JWE_LOG(cameraCtrl_suspendVM, ("MMICamera_StopDCPreview error"));
            return JKMEDIA_RESULT_ERROR;
        }

        if(!MMICamera_CloseDC())
        {
            JWE_LOG(cameraCtrl_suspendVM, ("MMICamera_CloseDC error"));
            return JKMEDIA_RESULT_ERROR;
        }
        camera_isopen = FALSE;
    }
  
    ret = JKMEDIA_RESULT_OK;
    
    JWE_LOG(cameraCtrl_suspendVM, ("leave"));
    return ret;
}

/*****************************************************************************/
//  Description : mediaCtrl_resumeVM
//  Global resource dependence : none
//  Author: 
//  Note: Do the process when JVM is to resumed.
/*****************************************************************************/
JKSint32 cameraCtrl_resumeVM( JKSint32 id ) 
{
   // JKSint32 status = 0; for pclint

    JKSint32 ret = JKMEDIA_RESULT_ERROR;
    T_mediaPlayer *pPlayer = PNULL;
    AJ_S32 state = ajms_arc_getState() ;
    
    JWE_LOG(cameraCtrl_resumeVM, ("enter id=%d, state = %d",id,state));

    if((AJSC_VMSTATE_DESTROYING == state|| AJSC_VMSTATE_KILLING  == state))//如果jvm正在关闭则不恢复播放
    {
        JWE_LOG(cameraCtrl_resumeVM, ("leave! jvm is not resuming, no need restart "));
        return ret;
    }

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(cameraCtrl_resumeVM, ("leave! null error "));
        return ret;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054 
    cameraCtrl_restart(id);

    ret = JKMEDIA_RESULT_OK;
    pPlayer->hasExtPaused = FALSE;
    
    JWE_LOG(cameraCtrl_resumeVM, ("leave"));  
    return ret;
}
/*=========================================================================== 
 *
 * Snapshot Control Function
 *
 ==========================================================================*/ 
static JKSint32 cameraOpt_Snapshot_getEncoderCount(void)
{
    JWE_LOG(cameraOpt_Snapshot_getEncoderCount,("enter"))
    return 1;
}

static JKSint32 cameraOpt_Snapshot_getEncoderType(JKSint32 i , JKWChar **pType)
{
    static const JKWChar mimeType[]= {
	'i','m','a','g','e','/','j','p','e','g',
    };
    JWE_LOG(cameraOpt_Snapshot_getEncoderType,("enter"))
    
    *pType=(JKWChar *)mimeType;
    return sizeof(mimeType)/sizeof(JKWChar);
}

static JKSint32 cameraOpt_Snapshot_prepare(JKSint32 id)
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraOpt_Snapshot_prepare,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraOpt_Snapshot_prepare,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    if(camera_isopen == FALSE)
    {
        if(!MMICamera_OpenDC())
        {
            JWE_LOG(cameraOpt_Snapshot_prepare, ("MMICamera_OpenDC error"));
            return JKMEDIA_RESULT_ERROR;
        }

        if(!MMICamera_InitDCPreviewParam())
        {
            JWE_LOG(cameraOpt_Snapshot_prepare, ("MMICamera_InitDCPreviewParam error"));
            return JKMEDIA_RESULT_ERROR;
        }

        camera_isopen = TRUE;
    }

    if(pPlayer->preview_isvisible == TRUE)
    {
        if(!MMICamera_StartDCPreview())
        {
            JWE_LOG(cameraOpt_Snapshot_prepare, ("MMICamera_StartDCPreview error"));
            return JKMEDIA_RESULT_ERROR;
        }
    }

    JWE_LOG(cameraOpt_Snapshot_prepare,("leave"));

    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_Snapshot_unprepare(JKSint32 id)
{
    JWE_LOG(cameraOpt_Snapshot_unprepare,("enter,id=%d"))
    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_Snapshot_setEncodeParam(JKSint32 id , JKT_Media_Param *pParam , JKSint32 paramLen , JKWChar **pType )
{
    static const JKWChar MIMEtype[]={'i','m','a','g','e','/','j','p','e','g'};
    JWE_LOG(cameraOpt_Snapshot_setEncodeParam,("enter,paramLen=%d",paramLen))
    //将局部数组赋给out参数。在外部使用时会很危险。建议使用strncpy
    *pType=(JKWChar *)MIMEtype;
    return sizeof(MIMEtype)/sizeof(JKWChar);
}

LOCAL uint32 CaptureCallBack(DCAMERA_RETURN_VALUE_E ret_value, void* param_ptr, uint32 param_size)
{
    JWE_LOG(CaptureCallBack,("enter,ret_value=%d",ret_value))

    if(ret_value != DCAMERA_OP_SUCCESS)
    {
        is_waiting = 2;
	return 0;
    }

    is_waiting = 3;

    jpeg_buf = param_ptr;
    jpeg_size = param_size;
    return 0;
}

static JKSint32 cameraOpt_Snapshot_capture(JKSint32 id)
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraOpt_Snapshot_capture,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraOpt_Video_startUpdate,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    if(pPlayer->preview_isvisible == TRUE)
    {
        JWE_LOG(cameraOpt_Snapshot_capture,("isvisible"))
    }

    if(camera_isopen== TRUE)
    {
        JWE_LOG(cameraOpt_Snapshot_capture,("camera opened"))
    }

    if(camera_isopen== FALSE)
    {
        if(!MMICamera_OpenDC())
        {
            JWE_LOG(cameraOpt_Snapshot_capture, ("MMICamera_OpenDC error"));
            return JKMEDIA_RESULT_ERROR;
        }
        //+CRNEWMS00108347            
        /*if(!MMICamera_InitDCPreviewParam())
        {
            JWE_LOG(cameraOpt_Snapshot_capture, ("MMICamera_InitDCPreviewParam error"));
            return JKMEDIA_RESULT_ERROR;
        }*/
        if(!MMICamera_StartDCPreview())
        {
            JWE_LOG(cameraOpt_Snapshot_capture, ("MMICamera_StartDCPreview error"));
            return JKMEDIA_RESULT_ERROR;
        }
        //-CRNEWMS00108347
        camera_isopen = TRUE;
    }

//    if(!MMICamera_SetPhotoSize(MMICAMAERA_PHOTO_SIZE_240X320))
    if(!MMICamera_SetPhotoSize(MMICAMAERA_PHOTO_SIZE_176X220))
    {
        JWE_LOG(cameraOpt_Snapshot_capture, ("MMICamera_SetPhotoSize error"));
        return JKMEDIA_RESULT_ERROR;
    }
    JWE_LOG(cameraOpt_Snapshot_capture,("MMICamera_SetPhotoSize,ok"));

    is_waiting = 1;
    if(!MMICamera_SnapShot(CaptureCallBack))
    {
        JWE_LOG(cameraOpt_Snapshot_capture, ("MMICamera_SnapShot error"));
        is_waiting = 0;
        return JKMEDIA_RESULT_ERROR;
    }    
    JWE_LOG(cameraOpt_Snapshot_capture,("MMICamera_SnapShot,ok"))
    while(is_waiting == 1)
    {
        SCI_Sleep(500);
    }

    JWE_LOG(cameraOpt_Snapshot_capture,("leave"))

    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_Snapshot_encode(JKSint32 id)
{
    JWE_LOG(cameraOpt_Snapshot_encode,("enter"))
    return JKMEDIA_RESULT_OK;
}

static JKUint8 *cameraOpt_Snapshot_getImage(JKSint32 id ,JKSint32 *pLength)
{

    JWE_LOG(cameraOpt_Snapshot_getImage,("enter,id=%d",id))

    if(is_waiting != 3)
    {
        return (JKUint8 *)NULL;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return PNULL;
    }
#endif
//-CR250054
    if(is_waiting == 3)
    {
        *pLength=jpeg_size;
    }

    is_waiting = 0;
    return (JKUint8 *)jpeg_buf;
}

static void cameraOpt_Snapshot_discardTemporary(JKSint32 id)
{
    JWE_LOG(cameraOpt_Snapshot_discardTemporary,("enter,id=%d",id))
}


/*=========================================================================== 
 *
 *	Input Device Control 
 *
 ==========================================================================*/ 
static JKSint32 cameraOpt_InputDevice_getMIMEType(JKWChar** pType)
{
    static const JKWChar MIMEType[] = {
        'i','m','a','g','e','/','j','p','e','g',
    };
    
    JWE_LOG(cameraOpt_InputDevice_getMIMEType,("enter"))

    *pType = ( JKWChar* )MIMEType;
    return sizeof( MIMEType ) / sizeof( JKWChar ); 
}

static JKSint32 cameraOpt_InputDevice_setParam( JKT_Media_Param* pParam, JKSint32 paramLen )
{
    JWE_LOG(cameraOpt_InputDevice_setParam,("enter"))

    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_InputDevice_open( void )
{
    JWE_LOG(cameraOpt_InputDevice_open,("enter"))
    if(camera_isopen== FALSE)
    {
        if(!MMICamera_OpenDC())
        {
            JWE_LOG(cameraOpt_Video_startUpdate, ("MMICamera_OpenDC error"));
            return JKMEDIA_RESULT_ERROR;
        }

        if(!MMICamera_InitDCPreviewParam())
        {
            JWE_LOG(cameraOpt_Video_startUpdate, ("MMICamera_InitDCPreviewParam error"));
            return JKMEDIA_RESULT_ERROR;
        }
        camera_isopen = TRUE;
    }

    JWE_LOG(cameraOpt_InputDevice_open,("leave"));

    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_InputDevice_close( void )
{
    JWE_LOG(cameraOpt_InputDevice_close,("enter"))

    if(camera_isopen == TRUE)
    {
        if(!MMICamera_StopDCPreview())
        {
            JWE_LOG(cameraOpt_InputDevice_close, ("MMICamera_StopDCPreview error"));
            return JKMEDIA_RESULT_ERROR;
        }

        if(!MMICamera_CloseDC())
        {
            JWE_LOG(cameraOpt_InputDevice_close, ("MMICamera_CloseDC error"));
            return JKMEDIA_RESULT_ERROR;
        }
        camera_isopen = FALSE;
    }

    JWE_LOG(cameraOpt_InputDevice_close,("leave"));

    return JKMEDIA_RESULT_OK;
}


/****************************************************************************
 *
 *    Optional Control Function 
 *
 ****************************************************************************/    
/*=========================================================================== 
 *
 *  Video Control Functions
 * 
 ==========================================================================*/ 
static JKSint32 cameraOpt_Video_getScalingMode( JKSint32 id )
{
//     return JKMEDIA_SCALING_FIXED_SIZE;
    return JKMEDIA_SCALING_INDEPENDENT;         // merlin 20070810: fix MAUI_00234205 JBOM-659
}

static JKSint32 cameraOpt_Video_setBounds( JKSint32 id,
                                          JKSint32 x,JKSint32 y,
                                          JKSint32 width,JKSint32 height,
                                          JKSint32 magX,JKSint32 magY )
{
    GUI_RECT_T      camerarect;
    KJavaGfxInfo    kjavaGfxInfo = {0};

    JWE_LOG(cameraOpt_Video_setBounds,("enter,player id=%d",id));

/*
    camerarect.left = x;
    camerarect.top = y;
    camerarect.right = x+width;
    camerarect.bottom = y+height;
*/
    camerarect.left = 0;
    camerarect.top = 0;
 //   camerarect.right = 240;
//    camerarect.bottom = 320;
	//GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,(uint16 *) (&camerarect.right),( uint16 *) (&camerarect.bottom));
    kjava_gfx_requestScreenMode(KJAVA_VIDEO_SCREEN_FULL);
    kjava_gfx_getInfo(&kjavaGfxInfo);
    camerarect.right = kjavaGfxInfo.viewRegion.width-1;
    camerarect.bottom = kjavaGfxInfo.viewRegion.height-1;

/*

    if(!MMICamera_SetFullScreenPreview(TRUE))
    {
        JWE_LOG(cameraOpt_Video_setBounds, ("MMICamera_SetFullScreenPreview error"));
        return JKMEDIA_RESULT_ERROR;
    }
*/
    if(!MMICamera_SetPreviewRect(camerarect))
    {
        JWE_LOG(cameraOpt_Video_setBounds, ("MMICamera_SetPreviewRect error"));
        return JKMEDIA_RESULT_ERROR;
    }
    JWE_LOG(cameraOpt_Video_setBounds,("leave"));
    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_Video_setOffset( JKSint32 id, JKSint32 offset_x, JKSint32 offset_y )
{
    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_Video_repaint( JKSint32 id )
{
    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_Video_startUpdate( JKSint32 id )
{
    T_mediaPlayer *pPlayer;
    KJavaGfxInfo    kjavaGfxInfo = {0};
    
    kjava_gfx_requestScreenMode(KJAVA_VIDEO_SCREEN_FULL);
    kjava_gfx_getInfo(&kjavaGfxInfo);

    JWE_LOG(cameraOpt_Video_startUpdate,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraOpt_Video_startUpdate,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
    return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    if(pPlayer->preview_isvisible == TRUE)
    {
        JWE_LOG(cameraOpt_Video_startUpdate,("isvisible"))
    }

    if(camera_isopen== TRUE)
    {
        JWE_LOG(cameraOpt_Video_startUpdate,("camera opened"))
    }


    if(camera_isopen == FALSE)
    {
        cameraOpt_Video_setBounds(0,0,0,0,0,0,0);//如果不是全屏需要设置 范围

        if(!MMICamera_OpenDC())
        {
            JWE_LOG(cameraOpt_Video_startUpdate, ("MMICamera_OpenDC error"));
            return JKMEDIA_RESULT_ERROR;
        }

        camera_isopen = TRUE;
    }
    
    visialbe_id = id;

    kjava_gfx_paintVirtualBegin();
    kjava_gfx_paintVirtualEnd(1,&kjavaGfxInfo.viewRegion);
    if(!MMICamera_StartDCPreview())
    {
        JWE_LOG(cameraOpt_Video_startUpdate, ("MMICamera_StartDCPreview error"));
        return JKMEDIA_RESULT_ERROR;
    }

    //GUIFORM_SetVisible(MMIJAVA_KEYBOARD1_FORM,FALSE,TRUE);
    //GUIFORM_SetVisible(MMIJAVA_KEYBOARD2_FORM,FALSE,TRUE);
   
    pPlayer->preview_isvisible = TRUE;
    JWE_LOG(cameraOpt_Video_startUpdate,("leave"));

    return JKMEDIA_RESULT_OK;
}

static JKSint32 cameraOpt_Video_stopUpdate( JKSint32 id )
{
    T_mediaPlayer *pPlayer;

    JWE_LOG(cameraOpt_Video_stopUpdate,("enter,player id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraOpt_Video_stopUpdate,("set fail. null player id"));
        return JKMEDIA_RESULT_ERROR;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
    return JKMEDIA_RESULT_ERROR;
    }
#endif
//-CR250054
    if(pPlayer->preview_isvisible == TRUE)
    {
        JWE_LOG(cameraOpt_Video_stopUpdate,("isvisible"))
    }
    
    if(camera_isopen == TRUE)
    {
        JWE_LOG(cameraOpt_Video_stopUpdate,("camera opened"))
    }


    if(camera_isopen == TRUE)
    {
        if(!MMICamera_StopDCPreview())
        {
            JWE_LOG(MMICamera_StopDCPreview, ("MMICamera_StopDCPreview error"));
            return JKMEDIA_RESULT_ERROR;
        }

        if(!MMICamera_CloseDC())
        {
            JWE_LOG(cameraOpt_Video_stopUpdate, ("MMICamera_CloseDC error"));
            return JKMEDIA_RESULT_ERROR;
        }
        camera_isopen = FALSE;
    }
    //GUIFORM_SetVisible(MMIJAVA_KEYBOARD1_FORM,TRUE,TRUE);
    //GUIFORM_SetVisible(MMIJAVA_KEYBOARD2_FORM,TRUE,TRUE);

    visialbe_id = -1;

    pPlayer->preview_isvisible = FALSE;
    JWE_LOG(cameraOpt_Video_stopUpdate,("leave"));

    return JKMEDIA_RESULT_OK;
}

static const JKT_Media_InputDeviceControl cameraOpt_InputDeviceCtrl = {
    cameraOpt_InputDevice_getMIMEType,	    /* JKSint32 ( *getMIMEType )( JKWChar** pType ) */
    cameraOpt_InputDevice_setParam,	    /* JKSint32 ( *setParam )( JKT_Media_Param* pParam, JKSint32 paramLen ) */
    cameraOpt_InputDevice_open,		    /* JKSint32 ( *open )( void ); */
    cameraOpt_InputDevice_close		    /* JKSint32 ( *close )( void ); */
};

static const JKT_Media_SnapshotControl cameraOpt_SnapshotCtrl ={
    cameraOpt_Snapshot_getEncoderCount,	/* JKSint32 (*getEncoderCount)(void);*/
    cameraOpt_Snapshot_getEncoderType,	/* JKSint32 (*getEncoderType)(JKSint32 i , JKWChar **pType);*/
    cameraOpt_Snapshot_prepare,		/* JKSint32 (*prepare)(JKSint32 id);*/
    cameraOpt_Snapshot_unprepare,	/* JKSint32 (*unprepare)(JKSint32 id); */
    cameraOpt_Snapshot_setEncodeParam,	/* JKSint32 (*setEncodeParam)(JKSint32 id, JKT_Media_Param *pParam ,JKSint32 paramLen , JKWChar **pType )*/
    cameraOpt_Snapshot_capture,		/* JKSint32	(*capture)(JKSint32 id);*/
    cameraOpt_Snapshot_encode,		/* JKSint32 (*encode)(JKSint32 id);*/
    cameraOpt_Snapshot_getImage,	/* JKUint8  *(*getImage)(JKSint32 id, JKSint32 *pLength);*/
    cameraOpt_Snapshot_discardTemporary /* void	(*discardTemporary)(JKSint32 id);*/
};

JKT_Media_VideoControl cameraOpt_VideoCtrl = {
    cameraOpt_Video_getScalingMode,    /* JKSint32 ( *getScalingMode )( JKSint32 id ); */
    cameraOpt_Video_setBounds,         /* JKSint32 ( *setBounds )( JKSint32 id, JKSint32 x, JKSint32 y, JKSint32 width, JKSint32 height, JKSint32 magX, JKSint32 magY ); */
    cameraOpt_Video_setOffset,         /* JKSint32 ( *setOffset )( JKSint32 id, JKSint32 offset_x, JKSint32 offset_y ); */
    cameraOpt_Video_repaint,           /* JKSint32 ( *repaint )( JKSint32 id ); */
    cameraOpt_Video_startUpdate,       /* JKSint32 ( *startUpdate )( JKSint32 id ); */
    cameraOpt_Video_stopUpdate         /* JKSint32 ( *stopUpdate )( JKSint32 id ); */
};

static const JKT_MediaOption cameraCtrl_OptionCtrl ={
    NULL,			    /* JKT_Media_TempoControl const * pTempoFuncs; */
    NULL,			    /* JKT_Media_SpeedControl const * PSpeedFuncs; */
    NULL,			    /* JKT_Media_StopControl const * pStopFuncs;   */
    NULL,			    /* JKT_Media_PitchControl const * pPitchFuncs; */
    NULL,			    /* JKT_Media_MeteDataControl const * pMetaDataFuncs; */
    NULL,			    /* JKT_Media_MIDIControl const * pMIDIFuncs;   */
    NULL,			    /* JKT_Media_SyncControl const * pSyncFuncs;   */
    &cameraOpt_VideoCtrl,	    /* JKT_Media_VideoControl const * pVideoFuncs; */
    NULL,			    /* JKT_Media_FrameControl const * pFrameFuncs; */
    &cameraOpt_SnapshotCtrl,	    /* void const * pSnapShotFuncs;*/
    &cameraOpt_InputDeviceCtrl,	    /* void const * pInputDeviceFuncs;		   */
    NULL,			    /* void const * pRecordFuncs;		   */ 
    NULL,			    /* void const * pExtFuncs;			   */ 
};


/**
 * Structure for SMF playback control functions
 */

const JKT_MediaControl cameraControl = {
    JKMEDIA_TYPE_CAMERA,	/* JKSint32 mediaType */
    cameraCtrl_getMIMEType,	/* JKSint32 ( *getMIMEType )( JKWChar** pType ); */
    cameraCtrl_isData,		/* JKBool ( *isData )( const JKUint8* pData, const JKSint32 length ); */
    cameraCtrl_checkOpen,	/* JKSint32 ( *checkOpen )( const JKUint8* pData, const JKSint32 length ); */
    mediaCtrl_check,		/* JKSint32 ( *check )( JKSint32 id, JKSint32* pExtraID ); */
    mediaCtrl_checkClose,	/* JKSint32 ( *checkClose )( JKSint32 id ); */
    cameraCtrl_getMediaSize,	/* JKSint32 ( *getMediaSize )( JKSint32 id, JKSint32* pWidth, JKSint32* pHeight ); */
    cameraCtrl_create,		/* JKSint32 ( *create )( void ); */
    cameraCtrl_destroy,		/* JKSint32 ( *destroy )( JKSint32 id ); */
    cameraCtrl_set,		    /* JKSint32 ( *set )( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID ); */
    cameraCtrl_load,		    /* JKSint32 ( *load )( JKSint32 id ); */
    cameraCtrl_start,		/* JKSint32 ( *start )( JKSint32 id, JKSint32 loop ); */
    cameraCtrl_stop,		    /* JKSint32 ( *stop )( JKSint32 id ); */
    cameraCtrl_pause,		/* JKSint32 ( *pause )( JKSint32 id ); */
    cameraCtrl_restart,		/* JKSint32 ( *restart )( JKSint32 id ); */
    cameraCtrl_unload,		/* JKSint32 ( *unload )( JKSint32 id ); */
    cameraCtrl_getState,		/* JKSint32 ( *getState )( JKSint32 id ); */
    cameraCtrl_getDuration,	/* JKSint32 ( *getDuration )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
    mediaCtrl_setTime,	    /* JKSint32 ( *setTime )( JKSint32 id, JKUint32 usecH, JKUint32 usecL ); */
    cameraCtrl_getRecordTime,	/* JKSint32 ( *getTime )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
    cameraCtrl_getIncludedContents, /* JKSint32 ( *getIncludedContents )( JKSint32 id ); */
    cameraCtrl_suspendVM,	/* JKSint32 ( *suspend )( JKSint32 id ); */
    cameraCtrl_resumeVM,		/* JKSint32 ( *resume )( JKSint32 id ); */

    JK_NULL,			    /* JKT_Media_EventControl */
    JK_NULL,			    /* JKT_Media_VolumeControl */
    JK_NULL,			    /* JKT_Media_UserControl */
    JK_NULL,			    /* void const* pExtFuncs */
    &cameraCtrl_OptionCtrl,	/* void const* pOptionFuncs */
};
/**
 * Structure for option control functions
 */

PUBLIC void cameraOpt_Preview_visialbe( void )
{
    T_mediaPlayer *pPlayer;
    if(visialbe_id == -1)
    {
        JWE_LOG(cameraOpt_Preview_visialbe,("player id -1,no need show video"));
        return;
    }    		
    pPlayer = getPlayer( visialbe_id );
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(cameraOpt_Preview_visialbe,("set fail. null player id"));
        return;
    }
//+CR250054
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if( MMIAPIJAVA_IsJavaBackgroundRuning()){
        return ;
    }
#endif
//-CR250054
    if(camera_isopen == TRUE)
    {
        if(!MMICamera_OpenDC())
        {
            JWE_LOG(cameraOpt_Preview_visialbe, ("MMICamera_OpenDC error"));
            return;
        }


        if(pPlayer->preview_isvisible == TRUE)
        {
            if(!MMICamera_StartDCPreview())
            {
                JWE_LOG(cameraOpt_Preview_visialbe, ("MMICamera_StartDCPreview error"));
                return;
            }
        }
    }

}

PUBLIC void cameraOpt_Preview_Invisialbe( void )
{
    JWE_LOG(cameraOpt_Preview_Invisialbe, ("enter"));
    if(visialbe_id == -1)
    {
        JWE_LOG(cameraOpt_Preview_Invisialbe,("player id -1,no need invisiable video"));
        return;
    }    		

    if(camera_isopen == TRUE)
    {
        if(!MMICamera_StopDCPreview())
        {
            JWE_LOG(cameraOpt_Preview_Invisialbe, ("MMICamera_StopDCPreview error"));
        }

        if(!MMICamera_CloseDC())
        {
            JWE_LOG(cameraOpt_Preview_Invisialbe, ("MMICamera_CloseDC error"));
        }
    }
}



#endif

