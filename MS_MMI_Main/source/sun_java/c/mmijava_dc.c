/*****************************************************************************
** File Name:      mmijava_dc.c                                           *
** Author:         hui.zhao                                               *
** Date:           6/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2009       hui.zhao            Create                                *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_sun_java_trc.h"
#ifdef JAVA_SUPPORT_SUN
#include "javacall_multimedia.h"
#include "javacall_lifecycle.h"
#include "javacall_logging.h"
#include "javacall_lcd.h"
#include "mmijava_multimedia.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiaudio.h"
#include "dal_dcamera.h"
#include "mmicamera_manager.h"
#include "mmi_position.h"
#include "mmidc_setting.h"
#include "sig_code.h"
#include "mmijava_main.h"
#include "mmijava_multimedia.h"
#include "mmidv_manager.h"
#include "mmijava_dc.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DC_DBG javacall_printf
#define DC_INFO javacall_printf

#define PHOTO_SIZE_3MP_WIDTH        2048
#define PHOTO_SIZE_3MP_HEIGHT       1536

#define PHOTO_SIZE_2MP_WIDTH        1600
#define PHOTO_SIZE_2MP_HEIGHT       1200

#define PHOTO_SIZE_1MP_WIDTH        1280
#define PHOTO_SIZE_1MP_HEIGHT       960

#define PHOTO_SIZE_VGA_WIDTH        640
#define PHOTO_SIZE_VGA_HEIGHT       480

#define PHOTO_SIZE_QVGA_WIDTH       320
#define PHOTO_SIZE_QVGA_HEIGHT      240

#define PHOTO_SIZE_240X320_WIDTH    240
#define PHOTO_SIZE_240X320_HEIGHT   320

#define PHOTO_SIZE_176X220_WIDTH    176
#define PHOTO_SIZE_176X220_HEIGHT   220

#define PHOTO_SIZE_220X176_WIDTH    220
#define PHOTO_SIZE_220X176_HEIGHT   176

#define PHOTO_SIZE_240X400_WIDTH    240
#define PHOTO_SIZE_240X400_HEIGHT   400

#define PHOTO_SIZE_400X240_WIDTH    400
#define PHOTO_SIZE_400X240_HEIGHT   240

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
    typedef struct
    {
        BOOLEAN     is_callback_finished;
        char*       buffer;
        uint32      size;
    }MMIJAVA_DC_SNAPSHOT_T;
    
    typedef struct
    {
        DCAMERA_DATA_TYPE_E encoding;
        MMICamera_PHOTO_SIZE_E photosize;
        int32       width;
        int32       height;
    }MMIJAVA_DC_IMAGE_TYPE_T;


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : close audio
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN dc_close(void);

/*****************************************************************************/
//  Description : free resource
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN dc_destroy(void);

/*****************************************************************************/
//  Description : acquive device to start
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result dc_acquire_device(wchar *filename);

/*****************************************************************************/
//  Description : release device
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result dc_release_device(int32 appid, int32 playerid);
LOCAL javacall_result dc_set_rate(MMIJAVA_MEDIA_TYPE_T mediatype, int rate);
/*****************************************************************************/
//  Description : stopped by others
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN dc_stopped_by_other(void);
/*****************************************************************************/
//  Description : start preview
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result dc_start(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype,int32 curtime);

/*****************************************************************************/
//  Description : stop playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result dc_stop(void);

/*****************************************************************************/
//  Description : pause playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result dc_pause(void);

/*****************************************************************************/
//  Description : resume playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result dc_resume(void);

/*****************************************************************************/
//  Description : get current time for the playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return current time for the playing audio
/*****************************************************************************/
LOCAL int32 dc_get_time(void);//dc中不需要

/*****************************************************************************/
//  Description : set current time for the playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return current time for the playing audio   
/*****************************************************************************/
LOCAL int32 dc_set_time(int32 ms);//dc中不需要

/*****************************************************************************/
//  Description : get total time for the playing audio
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL int32 dc_get_duration(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype);//dc中不需要

/*****************************************************************************/
//  Description : snapshot callback fun
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL uint32 dc_snapshot_callback(DCAMERA_RETURN_VALUE_E ret_value, char* buffer, uint32 buffer_size);

/*****************************************************************************/
//  Description : snapshot
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL int8 dc_start_snapshot( const uint16* imageType, int32 length );

/*****************************************************************************/
//  Description : parse image type
//  Global resource dependence : none
//  Author: hui.zhao
//  Note: 
/*****************************************************************************/
LOCAL void dc_parse_image_type(
									const javacall_utf16* imageType,
									int32 length,
									MMIJAVA_DC_IMAGE_TYPE_T* parsedType//OUT
									);

/*****************************************************************************/
//  Description : get snapshot data size
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL BOOLEAN dc_get_snapshot_data_size( int32 *size );

/*****************************************************************************/
//  Description : get snapshot data
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL BOOLEAN dc_get_snapshot_data(char* buffer, int32 size );

/*****************************************************************************/
//  Description : set video location
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL javacall_result dc_set_video_location(int32 x, int32 y, int32 w, int32 h);

/*****************************************************************************/
//  Description : get video preview's width and height
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL BOOLEAN dc_get_video_size(wchar *filename, int32* width,int32* height);

/*****************************************************************************/
//  Description : Show or hide camera capture preview
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_set_video_visible(BOOLEAN visible);

/*****************************************************************************/
//  Description : set full screen mode 
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL javacall_result dc_fullscreen_mode(BOOLEAN full_screen_mode, int32 appid, int32 playerid);

/*****************************************************************************/
//  Description : dc_map_frame_to_time
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_map_frame_to_time(int32 frameNum, int32 *ms);

/*****************************************************************************/
//  Description : dc_map_time_to_frame
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_map_time_to_frame(int32 ms, int32 *frameNum);

/*****************************************************************************/
//  Description : dc_seek_frame
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_seek_frame(int32 frameNum);

/*****************************************************************************/
//  Description : dc skip frame
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_skip_frame(uint32 time_offset);	

/*****************************************************************************/
//  Description : start recording video
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dv_start_recording(wchar *filename);

/*****************************************************************************/
//  Description : stop recording video
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dv_stop_recording(void);

/*****************************************************************************/
//  Description : pause recording video
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dv_pause_recording(void);

/*****************************************************************************/
//  Description : reset recording video
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dv_reset_recording(void);

/*****************************************************************************/
//  Description : get recorded data size
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN dv_get_recorded_data_size(
					wchar *filename,//in
					int32 *size	//out
					);//从文件中获得size
					
/*****************************************************************************/
//  Description : get recorded data
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN dv_get_recorded_data(wchar *filename,//in
				char *buf, 	//out
				int32 offset, 	//in
				int32 size	//in
				);
				
/*****************************************************************************/
//  Description : get record content type length
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL int dv_get_record_content_type_length(void);

/*****************************************************************************/
//  Description : get record content type
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL int dv_get_record_content_type(uint16* content_buf);

/*****************************************************************************/
//  Description : Record End Callback
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL void dv_record_end_callback(int32 i_type);
/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
LOCAL MMIJAVA_DC_SNAPSHOT_T s_snapshot_param = {0};
//LOCAL GUI_RECT_T s_dc_location_rect = {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT}; //remember last settings
LOCAL GUI_RECT_T s_dc_location_rect = {0} ;

LOCAL MMIFILE_HANDLE s_dv_sfs_handle = 0;
LOCAL BOOLEAN s_dc_fullscreen_mode = TRUE;
MMIJAVA_CAMERA_STATE_E s_camera_state = MMIJAVA_CAMERA_UNINIT;
/* avoid re-entry error. Because jvm and app task may operate dc concurrently. */
LOCAL MMIJAVA_CAMERA_STATE_E s_camera_pending_state = MMIJAVA_CAMERA_UNINIT;
LOCAL BOOLEAN s_camera_busy = FALSE;

static MMIJAVA_MEDIA_BASIC_INTERFACE_T s_dc_basic_itf = 
{   
    	dc_close,
    	dc_destroy,    
    	dc_start,
    	dc_stop,
    	dc_pause,
    	dc_resume,    
    	dc_get_time,
    	dc_set_time,
    	dc_get_duration,   
    	dc_acquire_device,
    	dc_release_device,
    	dc_set_rate,
    	dc_stopped_by_other
};

static MMIJAVA_MEDIA_SNAPSHOT_INTERFACE_T s_dc_snapshot_itf = 
{   
	dc_start_snapshot,
	dc_get_snapshot_data_size,
	dc_get_snapshot_data	
};

static MMIJAVA_MEDIA_VIDEO_INTERFACE_T s_dc_video_itf = 
{   
	dc_get_video_size,          /*lint !e64*/
	dc_set_video_location,
	dc_set_video_visible,	
	dc_fullscreen_mode,
	dc_map_frame_to_time,   /*lint !e64*/
	dc_map_time_to_frame,   /*lint !e64*/
	dc_seek_frame,
	dc_skip_frame
};      /*lint !e64*/

 static  MMIJAVA_MEDIA_RECORD_INTERFACE_T g_dv_recording_itf = 
{   
	dv_start_recording,
	dv_pause_recording,
	dv_stop_recording,
	dv_reset_recording,/*lint !e64*/
	dv_get_recorded_data_size,
	dv_get_recorded_data,
	dv_get_record_content_type_length,
	dv_get_record_content_type   
};

PUBLIC MMIJAVA_MEDIA_INTERFACE_T g_dc_dv_itf = 
{
    &s_dc_basic_itf,    
    &g_dv_recording_itf,
    &s_dc_video_itf,
    PNULL,
    &s_dc_snapshot_itf
};

/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN open_dc_preview(GUI_RECT_T* preview_rect, BOOLEAN* fullscreen)
{
    GUI_RECT_T rect = {0};

#ifdef DEBUG_NO_CAMERA
    return TRUE;
#endif      

    if (!MMICamera_OpenDC())
    {
        DC_INFO(LOG_STR("MMICamera_OpenDC fail"));
        return FALSE; 
    }
    
    if (preview_rect != NULL)
    {
        SCI_MEMCPY(&rect, preview_rect, sizeof(GUI_RECT_T));
        DC_DBG(LOG_STR_ENTER("set preview rect-(%d, %d) - (%d, %d)"), rect.left, rect.top, rect.right, rect.bottom);
        MMICamera_SetPreviewRect(*preview_rect);
    } 
    else if (fullscreen != NULL) 
    {
        DC_DBG(LOG_STR_ENTER("set fullscreen  == %d"), (int32)(*fullscreen));      
        MMICamera_SetFullScreenPreview(*fullscreen);
    }
    else
    {
        DC_DBG(LOG_STR_ENTER("NULL, NULL"));
        rect = MMITHEME_GetFullScreenRect();
        MMICamera_SetPreviewRect(rect);
    }
    
    
    if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
    {
        DC_DBG(LOG_STR("vm shutdwon, MMICamera_CloseDC"));
        return MMICamera_CloseDC();            
    }
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_VIDEO_MODE,DCAMERA_NORMAL_MODE,NULL);
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS, BRIGHTNESS_LEVEL_3, PNULL);
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST, CONTRAST_3, PNULL);
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_WB_MODE, WHITE_BALANCE_AUTO, PNULL);
    
    if (!MMICamera_StartDCPreview())
    {
        DC_INFO(LOG_STR("MMICamera_StartDCPreview fail"));
        return FALSE;
    }           
    
    DC_INFO(LOG_STR_LEAVE("ok"));    
    
    return TRUE ;
}

LOCAL BOOLEAN close_dc_preview()
{
#ifdef DEBUG_NO_CAMERA
    return TRUE;
#endif
    if (MMICamera_StopDCPreview())
    {
        if (MMICamera_CloseDC()) {
            DC_INFO(LOG_STR_LEAVE("ok"));
            return TRUE;
        }
    }
    DC_INFO(LOG_STR_LEAVE("fail"));
    return FALSE;
}
LOCAL BOOLEAN dc_handle_pending()
{
    if (s_camera_pending_state != MMIJAVA_CAMERA_UNINIT) {
        DC_DBG(LOG_STR("s_camera_pending_state =%d"), s_camera_pending_state );
        switch (s_camera_pending_state) {
            case MMIJAVA_CAMERA_DC_PAUSED:
                dc_pause();
                break;
            case MMIJAVA_CAMERA_SHUTDOWN:
                dc_close();
                break;
            default:
                break;
        }
    }
    return TRUE;
}
/*****************************************************************************/
//  Description : close audio
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN dc_close(void)
{
    DC_INFO(LOG_STR_ENTER("stat=%d"), s_camera_state);
    if (s_camera_busy == TRUE) {
        DC_DBG(LOG_STR_ENTER("pending stat=%d"), s_camera_state);
        s_camera_pending_state = MMIJAVA_CAMERA_SHUTDOWN;
        return TRUE;
    }
    if (s_camera_state == MMIJAVA_CAMERA_DC_PREVIEWING)
    {
        close_dc_preview();
    }
    if (s_camera_state == MMIJAVA_CAMERA_DV_RECORDING)
    {
        if (MMIDV_StopRecord())
        {
            DC_INFO(LOG_STR("MMIDV_Close"));
        }
    }
    s_camera_state = MMIJAVA_CAMERA_UNINIT;
    s_camera_pending_state = MMIJAVA_CAMERA_UNINIT;
    s_dc_fullscreen_mode = TRUE;
    s_dc_location_rect = MMITHEME_GetFullScreenRect();
    return TRUE;    
}

/*****************************************************************************/
//  Description : free resource
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN dc_destroy(void)
{
    return dc_close();
}

/*****************************************************************************/
//  Description : acquive device to start
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result dc_acquire_device(wchar *filename)
{
    s_dc_location_rect = MMITHEME_GetFullScreenRect();
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : release device
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result dc_release_device(int32 appid, int32 playerid)
{   
    DC_INFO(LOG_STR_ENTER("stat=%d"), s_camera_state);
    if (TRUE == dc_close())
        return JAVACALL_OK;
    return JAVACALL_FAIL;
}

LOCAL javacall_result dc_set_rate(MMIJAVA_MEDIA_TYPE_T mediatype, int rate)
{
    return JAVACALL_NOT_IMPLEMENTED;
}
/*****************************************************************************/
//  Description : stopped by others
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN dc_stopped_by_other(void)
{
    if ((s_camera_state == MMIJAVA_CAMERA_DC_PREVIEWING)
        ||(s_camera_state == MMIJAVA_CAMERA_SNAPSHOT_WITH_PREVIEW))
    {
        DC_INFO(LOG_STR("s_camera_state=%d, close dc"), s_camera_state);
        dc_stop();
        dc_close();
    }
    if ((s_camera_state == MMIJAVA_CAMERA_DV_PAUSED)
        ||(s_camera_state == MMIJAVA_CAMERA_DV_RECORDING))
    {
        DC_INFO(LOG_STR("s_camera_state=%d, close dv"), s_camera_state);
        dv_stop_recording();
    }
    s_dv_sfs_handle = 0;
    s_camera_state = MMIJAVA_CAMERA_UNINIT;
    return TRUE;
}
//  Description : start preview
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result dc_start(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype,int32 curtime)
{
    BOOLEAN result = FALSE;

    DC_INFO(LOG_STR_ENTER("stat=%d"), s_camera_state);
    DC_INFO(LOG_STR("filename = %s"), filename);
    DC_INFO(LOG_STR("mediatype = %d"), mediatype);
    DC_INFO(LOG_STR("curtime = %d"), curtime);
/*    if ((s_camera_state == MMIJAVA_CAMERA_DV_RECORDING)
        || (s_camera_state == MMIJAVA_CAMERA_DV_PAUSED))
    {
        javacall_printf(LOG_STR_LEAVE("ok, dv record start"));
        return TRUE;        
    }
*/
    s_camera_pending_state = MMIJAVA_CAMERA_UNINIT;
    if (s_camera_state != MMIJAVA_CAMERA_UNINIT)
    {
        DC_INFO(LOG_STR("warning!!!, illegal stat=%d"), s_camera_state);
        s_camera_state = MMIJAVA_CAMERA_UNINIT;
    }
    s_camera_busy = TRUE; 
    
    result = open_dc_preview(NULL, NULL);
    if (TRUE == result)
    {
        s_camera_state = MMIJAVA_CAMERA_DC_PREVIEWING;
    }
    s_camera_busy = FALSE;
    dc_handle_pending();
    DC_INFO(LOG_STR_LEAVE("result = %d"),result);
    if (result == TRUE)
        return JAVACALL_OK;
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : stop playing audio
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value:      
/*****************************************************************************/
LOCAL javacall_result dc_stop(void)
{
    DC_INFO(LOG_STR_ENTER("stat=%d"), s_camera_state);
    if (s_camera_busy == TRUE) {
        DC_DBG(LOG_STR_ENTER("pending stat=%d"), s_camera_state);
        s_camera_pending_state = MMIJAVA_CAMERA_SHUTDOWN;
        return JAVACALL_OK;
    }
    switch (s_camera_state)
    {
        case MMIJAVA_CAMERA_DC_PREVIEWING:
            s_camera_state = MMIJAVA_CAMERA_UNINIT;
            if (TRUE == close_dc_preview())
            {
                DC_INFO(LOG_STR_LEAVE("ok"));
                return JAVACALL_OK;
            }
            DC_INFO(LOG_STR_LEAVE("fail"));
            return JAVACALL_FAIL;
        case MMIJAVA_CAMERA_DV_RECORDING:
        case MMIJAVA_CAMERA_DV_PAUSED:
            //dv_stop_recording();
            DC_INFO(LOG_STR("warning, wrong state"));
            break;
        default:
            break;
    }
    DC_DBG(LOG_STR_LEAVE("fail, illegal stat=%d"), s_camera_state);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : pause dc preview
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result dc_pause(void)
{
    if (s_camera_busy == TRUE) {
        DC_DBG(LOG_STR_ENTER("pending stat=%d"), s_camera_state);
        s_camera_pending_state = MMIJAVA_CAMERA_DC_PAUSED;
        return JAVACALL_OK;
    }
    DC_DBG(LOG_STR_ENTER("stat=%d"), s_camera_state);
    if (s_camera_state == MMIJAVA_CAMERA_DC_PREVIEWING)
    {
        s_camera_state=MMIJAVA_CAMERA_DC_PAUSED;
        if (TRUE != close_dc_preview()) {
            s_camera_state=MMIJAVA_CAMERA_UNINIT;
            DC_DBG(LOG_STR_LEAVE("result=fail"));
            return JAVACALL_FAIL;
        }
    }
    DC_DBG(LOG_STR_LEAVE("result=ok"));
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : resume dc
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result dc_resume(void)
{
    BOOLEAN  result = TRUE;

    DC_DBG(LOG_STR_ENTER("stat=%d"), s_camera_state);
    s_camera_busy = TRUE;
    if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
    {
        DC_DBG(LOG_STR("vm shutdwon"));
        s_camera_busy = FALSE;
        return JAVACALL_OK;
    }
    if (s_camera_state == MMIJAVA_CAMERA_DC_PAUSED)
    {
        result=open_dc_preview(&s_dc_location_rect, NULL);
        s_camera_state=MMIJAVA_CAMERA_DC_PREVIEWING;
        if (result != TRUE)
            s_camera_state=MMIJAVA_CAMERA_UNINIT;
    }
    s_camera_busy = FALSE;
    dc_handle_pending();
    if (result == TRUE) {
        DC_DBG(LOG_STR_LEAVE("result=ok"));
        return JAVACALL_OK;
    }
    DC_DBG(LOG_STR_LEAVE("result=fail"));
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : get current time for the playing audio
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return current time for the playing audio
/*****************************************************************************/
LOCAL int32 dc_get_time(void)//dc中不需要
{
    return -1;
}

/*****************************************************************************/
//  Description : set current time for the playing audio
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return current time for the playing audio   
/*****************************************************************************/
LOCAL int32 dc_set_time(int32 ms)//dc中不需要
{ 
    return -1;
}

/*****************************************************************************/
//  Description : get total time for the playing audio
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL int32 dc_get_duration(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype)//dc中不需要
{
    return -1;
}

/*****************************************************************************/
//  Description : snapshot callback fun
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL uint32 dc_snapshot_callback(DCAMERA_RETURN_VALUE_E ret_value, char* buffer, uint32 buffer_size)
{
    DC_DBG(LOG_STR_ENTER("ret_value = %d, buffer=0x%x, size=%d"), ret_value, buffer,buffer_size);

    if(PNULL == buffer || 0 == buffer_size)
    {
        return 2;
    }
    s_snapshot_param.is_callback_finished = TRUE;
    if (s_snapshot_param.buffer != PNULL)
    {
        SCI_FREE(s_snapshot_param.buffer);
        s_snapshot_param.buffer = PNULL;
    }
    s_snapshot_param.buffer = SCI_ALLOCA(buffer_size);
    s_snapshot_param.size = 0;

    if (s_snapshot_param.buffer == PNULL)
    {
        DC_DBG(LOG_STR("s_snapshot_param.buffer ==  NULL"));
        return 1;
    }
    
    SCI_MEMCPY(s_snapshot_param.buffer, buffer, buffer_size);
    s_snapshot_param.size = buffer_size;
    DC_DBG(LOG_STR("save snapshot buffer ok"));    

    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_SNAPSHOT_END_IND, 0);
    DC_DBG(LOG_STR_LEAVE("save buffer=0x%x"), s_snapshot_param.buffer);
    return 0;
}

/*****************************************************************************/
//  Description : snapshot notify处理
//  Global resource dependence :
//  Author: 
//  Note: restart camera
/*****************************************************************************/
PUBLIC void MMIJAVA_CallbackSnapshotEndNotifyProc(void)
{
    DC_DBG(LOG_STR_ENTER("stat=%d"), s_camera_state);
    switch (s_camera_state)
    {
        case MMIJAVA_CAMERA_SNAPSHOT_WITHOUT_PREVIEW:
            MMICamera_CloseDC();
            s_camera_state = MMIJAVA_CAMERA_UNINIT;
            break;
        case MMIJAVA_CAMERA_SNAPSHOT_WITH_PREVIEW:
            MMICamera_CloseDC();
            open_dc_preview(&s_dc_location_rect, NULL);
            s_camera_state = MMIJAVA_CAMERA_DC_PREVIEWING;
            break;
        default:
            break;
    }
	
    MMIJAVA_NotifySnapshotFinished();
    DC_DBG(LOG_STR_LEAVE("stat=%d"), s_camera_state);
}

/*****************************************************************************/
//  Description : snapshot
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL int8 dc_start_snapshot( const uint16* imageType, int32 length )
{
    MMIJAVA_DC_IMAGE_TYPE_T parsedType = {0};/*lint !e64 */
    
    DC_DBG(LOG_STR_ENTER("imageType =%d, length  =%d "), *imageType, length);

    DC_DBG(LOG_STR_ENTER("stat=%d"), s_camera_state);

#ifdef DEBUG_NO_CAMERA
    MMIJAVA_NotifySnapshotFinished();
    return 1;
#endif
    dc_parse_image_type(imageType,length,&parsedType);

    switch (s_camera_state)
    {
        case MMIJAVA_CAMERA_SNAPSHOT_WITH_PREVIEW:
        case MMIJAVA_CAMERA_SNAPSHOT_WITHOUT_PREVIEW:
            DC_INFO(LOG_STR_LEAVE("fail, in snapshoting"));
            return 0;
        case MMIJAVA_CAMERA_UNINIT:
        case MMIJAVA_CAMERA_DC_PAUSED:
            if (open_dc_preview(NULL, NULL) != TRUE) {
                s_camera_state=MMIJAVA_CAMERA_UNINIT;
                DC_INFO(LOG_STR_LEAVE("start camera failed"));
                return 0;
            }
            s_camera_state=MMIJAVA_CAMERA_SNAPSHOT_WITHOUT_PREVIEW;
            break;
        case MMIJAVA_CAMERA_DC_PREVIEWING:
            s_camera_state=MMIJAVA_CAMERA_SNAPSHOT_WITH_PREVIEW;
            break;
        default:
            break;
    }

    if (!MMICamera_StopDCPreview())
    {
        MMICamera_CloseDC();
        s_camera_state=MMIJAVA_CAMERA_UNINIT;
        DC_INFO(LOG_STR("fail, MMICamera_StopDCPreview fail"));	
    	 return 0;
    }

    if(!MMICamera_SetPhotoSize(parsedType.photosize))//调试值,该值可设置
    {
        DC_INFO(LOG_STR("MMICamera_SetPhotoSize fail"));	
         return 0;	
    }
        
    if (MMICamera_SnapShot((DCAMERA_CALL_BACK)dc_snapshot_callback))
    {
        DC_DBG(LOG_STR_LEAVE("ok, stat=%d"), s_camera_state);
        return 1;
    }

    s_camera_state=MMIJAVA_CAMERA_UNINIT;
    DC_INFO(LOG_STR("fail"));
    return 0;
}

/*****************************************************************************/
//  Description : parse image type
//  Global resource dependence : none
//  Author: hui.zhao
//  Note: 
/*****************************************************************************/
LOCAL void dc_parse_image_type(
									const javacall_utf16* imageType,
									int32 length,
									MMIJAVA_DC_IMAGE_TYPE_T* parsedType//OUT
									)
{
	char* pStr = PNULL;
	char* pEncoding = PNULL;
	char* pWidth = PNULL;
	char* pHeight = PNULL;
	
    DC_DBG(LOG_STR_ENTER("imageType =%x, length = %d, parsedType =%x"), imageType, length, parsedType);  
    if(PNULL == imageType)
    {
        return ;
    }
	if (0 == length)
	{	    
		parsedType->encoding = DCAMERA_DATA_TYPE_JPEG; 
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_176X220;
	}
    pStr = SCI_ALLOCA(length + 1); 
    if(NULL == pStr)
    {
        DC_DBG(LOG_STR_LEAVE("SCI_ALLOCA failed"));
        return;
    }
    MMI_WSTRNTOSTR((uint8*)pStr,length,imageType,length,length);
    pStr[length] = 0;
	
    pEncoding = strstr(pStr, "encoding=");
    pWidth = strstr(pStr, "width=");
    pHeight = strstr(pStr, "height=");	

	//平台目前只支持jpeg编码
	parsedType->encoding = DCAMERA_DATA_TYPE_JPEG; 

	//
	if (pWidth) {
	    pWidth += strlen("width=");
	    strtok(pWidth, "&");
	    parsedType->width = atoi(pWidth);
	} else {
	    parsedType->width = MMITHEME_GetClientRectWidth();
	}

	if (pHeight) {
	    pHeight += strlen("height=");
	    strtok(pHeight, "&");
	    parsedType->height = atoi(pHeight);
	} else {
	    parsedType->height = MMITHEME_GetClientRectHeight();
	}

	//平台中只支持如下几种格式像素的照片大小
	if ((PHOTO_SIZE_3MP_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_3MP_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_3MP;
	}
	else if ((PHOTO_SIZE_2MP_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_2MP_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_2MP;
	}
	else if ((PHOTO_SIZE_1MP_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_1MP_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_1MP;
	}
	else if ((PHOTO_SIZE_VGA_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_VGA_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_VGA;
	}
	else if ((PHOTO_SIZE_QVGA_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_QVGA_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_QVGA;
	}
	else if ((PHOTO_SIZE_400X240_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_400X240_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_400X240;
	}
	else if ((PHOTO_SIZE_240X400_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_240X400_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_240X400;
	}
	else if ((PHOTO_SIZE_240X320_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_240X320_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_240X320;
	}
	else if ((PHOTO_SIZE_220X176_WIDTH == parsedType->width)
		&&(PHOTO_SIZE_220X176_HEIGHT == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_220X176;
	}
	else// if ((176 == parsedType->width)&&(220 == parsedType->height))
	{
		parsedType->photosize = MMICAMAERA_PHOTO_SIZE_176X220;
	}

	DC_DBG(LOG_STR_LEAVE("photosize=%d"),parsedType->photosize);
}

/*****************************************************************************/
//  Description : get snapshot data size
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL BOOLEAN dc_get_snapshot_data_size( int32 *size )
{
	BOOLEAN 	result = FALSE;
	
	DC_DBG(LOG_STR_ENTER("size = %x"), size);
	if(PNULL == size )
	{
	    return result;
	}
#ifdef DEBUG_NO_CAMERA
    dv_get_recorded_data_size(L"E:\\media\\camera\\snapshot.jpg", size);
#else
    if (s_snapshot_param.is_callback_finished)
    {
        *size = s_snapshot_param.size;
        result = TRUE;
    }
#endif
    DC_DBG(LOG_STR("size = %d,result = %d"), *size,result);

	return result;
}

/*****************************************************************************/
//  Description : get snapshot data
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL BOOLEAN dc_get_snapshot_data(char* buffer, int32 size )
{
	BOOLEAN 	result = FALSE;
	
	DC_DBG(LOG_STR_ENTER("buffer = %x, size = %d"), buffer, size);
	if(PNULL == buffer )
	{
	    return result;
	}
#ifdef DEBUG_NO_CAMERA
    dv_get_recorded_data(L"E:\\media\\camera\\snapshot.jpg", buffer, 0, size);
#else
    if (s_snapshot_param.is_callback_finished && s_snapshot_param.buffer != PNULL)
    {
        SCI_MEMCPY(buffer,s_snapshot_param.buffer,size);
        SCI_FREE(s_snapshot_param.buffer);
        s_snapshot_param.buffer=PNULL;
        result = TRUE;
    }
#endif
    DC_DBG(LOG_STR_LEAVE("result=%d"),result);	

	return result;
}

/*****************************************************************************/
//  Description : set video location
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL javacall_result dc_set_video_location(int32 x, int32 y, int32 w, int32 h)//目前平台中不支持设置任意preview区域,该接口可能会有问题
{
    BOOLEAN         result = 0;
    GUI_RECT_T 		rect = {0};

    s_camera_busy = TRUE;

    if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
    {
        DC_DBG(LOG_STR("vm shutdwon"));
        dc_close();
        s_camera_busy = FALSE;
        return JAVACALL_OK;
    }
    
    //keep zero  at 2 lowest bits
    rect.left = x >> 2  << 2;                   /*lint !e504*/
    rect.right = (x+w -1) >> 2  << 2 ;       /*lint !e504*/
    rect.top = (y + java_lcd_GetYStartFromLCD()) >> 2  << 2 ;                   /*lint !e504*/
    rect.bottom = (y + h - 1) >> 2  << 2 ;    /*lint !e504*/
    DC_DBG(LOG_STR("input rect: (%d, %d) - (%d, %d)"), rect.left, rect.top,  rect.right, rect.bottom);
    
    if ((s_dc_location_rect.left == rect.left) && (s_dc_location_rect.right == rect.right)
        && (s_dc_location_rect.top == rect.top) && (s_dc_location_rect.bottom == rect.bottom))
    {
        DC_DBG(LOG_STR("ok, same settings"));
        s_camera_busy = FALSE;
        return JAVACALL_OK;
    }

    s_dc_location_rect.left = rect.left;
    s_dc_location_rect.right = rect.right;
    s_dc_location_rect.top = rect.top  ;
    s_dc_location_rect.bottom = rect.bottom ;

    if (s_camera_state==MMIJAVA_CAMERA_DC_PREVIEWING)
    {
        DC_DBG(LOG_STR("restart camera preview"));
        result = close_dc_preview();
        if (TRUE == result)
            result=open_dc_preview(&s_dc_location_rect, NULL);
        DC_DBG(LOG_STR_LEAVE("result = %d"), result);      

        s_camera_busy = FALSE;
        if (result == TRUE) {
            DC_DBG(LOG_STR_LEAVE("result=ok"));
            return JAVACALL_OK;
        }
        DC_DBG(LOG_STR_LEAVE("result=fail"));
        return JAVACALL_FAIL;
    }
    //TODO: do not support set video location of DV
    if (javacall_lifecycle_get_vm_state() == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
    {
        DC_DBG(LOG_STR("vm shutdwon"));
        dc_close();
        s_camera_busy = FALSE;
        return JAVACALL_OK;
    }

    s_camera_busy = FALSE;
    dc_handle_pending();
    DC_DBG(LOG_STR("ok, just save settings"));	
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : get video preview's width and height
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL BOOLEAN dc_get_video_size(wchar *filename, int32* width,int32* height)
{
	GUI_RECT_T preview_rect = {0};
#ifdef DEBUG_NO_CAMERA
    *width=240;
    *height=320;
#else
    preview_rect = MMICamera_GetPreviewRect();

    *width = (int32)(preview_rect.right - preview_rect.left + 1);
    *height = (int32)(preview_rect.bottom - preview_rect.top  + 1);
#endif
    DC_DBG(LOG_STR("width is %d,height is %d,result is %d"),*width,*height,1);	

	return TRUE;	
}

/*****************************************************************************/
//  Description : Show or hide camera capture preview
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_set_video_visible(BOOLEAN visible)
{
	//目前平台不支持该功能
    if (visible)
        return TRUE;

    //MMICamera_StopDCPreview();
    DC_INFO(LOG_STR("invisible not supported,StopDCPreview"));
    return FALSE;
}

/*****************************************************************************/
//  Description : set full screen mode 
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL javacall_result dc_fullscreen_mode(BOOLEAN full_screen_mode, int32 appid, int32 playerid)
{
    BOOLEAN 		result = FALSE;

    DC_DBG(LOG_STR("full_screen_mode = %d"), full_screen_mode);	
    if (s_dc_fullscreen_mode == full_screen_mode)
    {
        DC_DBG(LOG_STR_LEAVE("ok, same settings"));
        return JAVACALL_OK;
    }
    s_dc_fullscreen_mode = full_screen_mode;

    if (s_camera_state==MMIJAVA_CAMERA_DC_PREVIEWING)
    {
        result=FALSE;
        DC_DBG(LOG_STR("restart camera preview"));
        result=close_dc_preview();
        if (result == TRUE)
            result=open_dc_preview(NULL, &s_dc_fullscreen_mode);
        if (result == TRUE) {
            DC_DBG(LOG_STR_LEAVE("result=ok"));
            return JAVACALL_OK;
        }
        DC_DBG(LOG_STR_LEAVE("result=fail"));
        return JAVACALL_FAIL;
    }
#ifndef DEBUG_NO_CAMERA
    MMICamera_SetFullScreenPreview(s_dc_fullscreen_mode);
#endif
    DC_DBG(LOG_STR_LEAVE("ok"));	
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : dc_map_frame_to_time
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_map_frame_to_time(int32 frameNum, int32 *ms)
{
	//dc不支持该功能
	DC_DBG(LOG_STR_LEAVE("not supported, return ms=0"));	
	return FALSE;
}

/*****************************************************************************/
//  Description : dc_map_time_to_frame
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_map_time_to_frame(int32 ms, int32 *frameNum)
{
	//dc不支持该功能
	DC_DBG(LOG_STR_LEAVE("not supported"));	
	return FALSE;
}

/*****************************************************************************/
//  Description : dc_seek_frame
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_seek_frame(int32 frameNum)
{
	//dc不支持该功能
	DC_DBG(LOG_STR_LEAVE("not supported, return num=0"));	
	return FALSE;
}
	
/*****************************************************************************/
//  Description : dc skip frame
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dc_skip_frame(uint32 time_offset)
{
	DC_DBG(LOG_STR_LEAVE("not supported"));	
	return FALSE;	
}
//set snapshot size
/*****************************************************************************/
//  Description : start recording video
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dv_start_recording(wchar *filename)
{
    BOOLEAN result = FALSE;
    
    DC_DBG(LOG_STR_ENTER("s_camera_state=%d, filename=%s"), s_camera_state, javacall_wstr2str(filename));/*lint !e64*/

    if (s_camera_state == MMIJAVA_CAMERA_DV_PAUSED)
    {
//        MMIDV_ResumeRecord();
         s_camera_state = MMIJAVA_CAMERA_DV_RECORDING;
        DC_DBG(LOG_STR_LEAVE("resume record"));
        return TRUE;
    }

    if (filename == PNULL)
    {
        DC_INFO(LOG_STR_LEAVE("fail, no filename"));
        return FALSE;
    }

    if (s_camera_state == MMIJAVA_CAMERA_DC_PREVIEWING)
    {
        if (TRUE != close_dc_preview())
        {
            DC_INFO(LOG_STR_LEAVE("fail, fail to close dc"));
            return FALSE;
        }
    }

    if (MMIDV_Open())
    {
        DC_DBG(LOG_STR("start"));
        if (MMIAPIFMM_IsFileExist(filename,MMIAPICOM_Wstrlen(filename)))
            MMIAPIFMM_DeleteFile(filename,NULL);

        DCAMERA_ParamCtrl(DRECORDER_PARAM_CTRL_BRIGHTNESS, BRIGHTNESS_LEVEL_3, PNULL);/*lint !e64 */
        DCAMERA_ParamCtrl(DRECORDER_PARAM_CTRL_CONTRAST, CONTRAST_3, PNULL);/*lint !e64 */
        
        MMIDV_StartPreview();
      //  MMIDV_StopPreview();
        if (MMIDV_StartRecord(dv_record_end_callback,filename))
        {
            s_camera_state = MMIJAVA_CAMERA_DV_RECORDING;
            result = TRUE;
        }
    }

    DC_DBG(LOG_STR_LEAVE("result = %d"), result);
    return result;
}

/*****************************************************************************/
//  Description : Record End Callback
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL void dv_record_end_callback(int32 i_type)
{
    //SCI_TRACE_MID:"[MMIJAVA]: dv_record_end_callback record end! i_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_DC_1255_112_2_18_3_27_17_17,(uint8*)"d", i_type);
    DC_DBG(LOG_STR(""));
}

LOCAL BOOLEAN close_dv_start_dc()
{
    if (s_camera_state == MMIJAVA_CAMERA_DV_RECORDING)
    {
        s_camera_state = MMIJAVA_CAMERA_UNINIT;
        if (TRUE != MMIDV_StopRecord()) {
            DC_INFO(LOG_STR_LEAVE("fail, MMIDV_StopRecord fail"));
            return FALSE;
        }
        if (TRUE != MMIDV_Close()) {
            DC_INFO(LOG_STR_LEAVE("fail, MMIDV_Close fail"));
            return FALSE;
        }
        if (TRUE != open_dc_preview(&s_dc_location_rect, NULL)) {
            DC_INFO(LOG_STR_LEAVE("fail, open_dc_preview fail"));
            return FALSE;
        }
    }
    s_camera_state = MMIJAVA_CAMERA_DC_PREVIEWING;
    DC_DBG(LOG_STR_LEAVE("ok"));
    return TRUE;
}

/*****************************************************************************/
//  Description : pause recording video
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dv_stop_recording(void)
{    
    DC_DBG(LOG_STR_ENTER("stat=%d"), s_camera_state);
    return close_dv_start_dc();
}

/*****************************************************************************/
//  Description : pause recording video
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dv_pause_recording(void)
{
    DC_DBG(LOG_STR_ENTER("stat=%d"), s_camera_state);
    return close_dv_start_dc();
}

/*****************************************************************************/
//  Description : reset recording video
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN dv_reset_recording(void)
{
    DC_DBG(LOG_STR_ENTER("stat=%d"), s_camera_state);
    return close_dv_start_dc();
}

/*****************************************************************************/
//  Description : get recorded data size
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
LOCAL BOOLEAN dv_get_recorded_data_size(
                                                wchar *filename,//in
                                                int32 *size    //out
                                                )//从文件中获得size
{
    BOOLEAN         result          = TRUE;

    if (!MMIAPIFMM_IsFileExist(filename,MMIAPICOM_Wstrlen(filename)))
    {
        *size = 0;
    }
    else
    {    
        s_dv_sfs_handle = MMIAPIFMM_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        DC_DBG(LOG_STR("s_dv_sfs_handle =%d"),s_dv_sfs_handle);

        if (PNULL != s_dv_sfs_handle)
        {    
            *size = MMIAPIFMM_GetFileSize(s_dv_sfs_handle);
        }
        MMIAPIFMM_CloseFile(s_dv_sfs_handle);
    }

    DC_DBG(LOG_STR_LEAVE("size = %d"),size);
    return result;
    
}

/*****************************************************************************/
//  Description : get recorded data
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
LOCAL BOOLEAN dv_get_recorded_data(wchar *filename,//in
                                            char *buf,     //out
                                            int32 offset,     //in
                                            int32 size    //in
                                            )
{
    uint32                       read_size       = 0;   
    BOOLEAN                     result          = TRUE;
    
    DC_DBG(LOG_STR_ENTER("offset = %d, size = %d"), offset,size);
    
    s_dv_sfs_handle = MMIAPIFMM_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

    
    if (SFS_INVALID_HANDLE != s_dv_sfs_handle)
    {
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(s_dv_sfs_handle, offset, SFS_SEEK_BEGIN))
        {        
            if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(s_dv_sfs_handle,buf,(uint32)size,&read_size,NULL))
            {
                DC_INFO(LOG_STR("read file error"));
                result = FALSE;                
            }       
        }
        else
        {
            DC_INFO(LOG_STR("set file pointer error file_handle = %d"),s_dv_sfs_handle);
            result = FALSE;            
        }
    }
    else
    {    
        DC_INFO(LOG_STR("file error"));
        result = FALSE;
    }

    MMIAPIFMM_CloseFile(s_dv_sfs_handle);
    DC_DBG(LOG_STR_LEAVE("read_size = %d, ret=%d"),read_size, result);

    return result;
}

/*****************************************************************************/
//  Description : get record content type length
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
LOCAL int dv_get_record_content_type_length(void)
{
    wchar contentType[] = L"video/3gpp";

    DC_DBG(LOG_STR(""));

    return sizeof(contentType) / sizeof(*contentType)-1; 
}

/*****************************************************************************/
//  Description : get record content type
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:  return value: 
/*****************************************************************************/
LOCAL int dv_get_record_content_type(uint16* content_buf)
{
    wchar contentType[] = L"video/3gpp";
    
    DC_DBG(LOG_STR(""));
    MMIAPICOM_Wstrcpy(content_buf,contentType);   

    return sizeof(contentType) / sizeof(*contentType)-1;
}

PUBLIC int32 mmijava_dc_get_state(void)
{
    return s_camera_state;
}

PUBLIC int32 mmijava_dc_get_rect(GUI_RECT_T *rect)
{
    if((NULL == rect))
    return -1;
    
    memcpy(rect, &s_dc_location_rect, sizeof(GUI_RECT_T));
    return 0;
}

#endif

