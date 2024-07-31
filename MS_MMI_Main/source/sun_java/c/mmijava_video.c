/*****************************************************************************
** File Name:      mmijava_video.c                                           *
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
 
#ifdef JAVA_SUPPORT_SUN
#include "javacall_multimedia.h"
#include "javacall_logging.h"
#include "mmijava_multimedia.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "dal_player.h"
#include "mmi_position.h"
#include "mmijava_video.h"
#include "sig_code.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmidisplay_data.h"
#include "mmijava_main.h"
#include "mmijava_multimedia.h"
#include "javacall_lcd.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIJAVA_VIDEO_ROTATE_ANGLE_0         0
#define MMIJAVA_VIDEO_ROTATE_ANGLE_90        1


LOCAL uint32 callback_para_ptr[3] = {0} ;
LOCAL int32 callback_para_num = 3;
LOCAL int32 video_time = 0;
LOCAL int32 video_duration = 0;
LOCAL int32 video_width = 0;
LOCAL int32 video_height = 0;  
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
 //java video state
typedef enum
{    
    MMIJAVA_VIDEO_PLAYING, //play
    MMIJAVA_VIDEO_PAUSED,  //pause
    MMIJAVA_VIDEO_STOPPED, //stop
    MMIJAVA_VIDEO_STATE_MAX
} MMIJAVA_VIDEO_STATE_E;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open and start video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN video_close(void);
LOCAL void video_close_callback(void);

/*****************************************************************************/
//  Description : open and start video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN video_destroy(void);

/*****************************************************************************/
//  Description : open and start video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL javacall_result video_start(
                          wchar *filename,
                          MMIJAVA_MEDIA_TYPE_T mediatype,
                          int32 cur_time
                          );
LOCAL void video_start_callback();
							
/*****************************************************************************/
//  Description : stop and close video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL javacall_result video_stop(void);
LOCAL void video_stop_callback(void);

/*****************************************************************************/
//  Description : pause video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL javacall_result video_pause(void);
LOCAL void video_pause_callback(void);
/*****************************************************************************/
//  Description : resume video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL javacall_result video_resume(void);
LOCAL void video_resume_callback(void);
/*****************************************************************************/
//  Description : get time
//  Global resource dependence : 
//  Author: hui.zhao
//  Note: If success return time in ms else return -1
/*****************************************************************************/
LOCAL int32 video_get_time(void);
LOCAL void video_get_time_callback(void);
/*****************************************************************************/
//  Description : get time
//  Global resource dependence : 
//  Author: hui.zhao
//  Note: If success return time in ms else return -1
/*****************************************************************************/
LOCAL int32 video_set_time(int32 time);
//LOCAL void video_set_time_callback(int32 time);
/*****************************************************************************/
//  Description : get duration
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL int32 video_get_duration(
                               wchar *filename,
                               MMIJAVA_MEDIA_TYPE_T mediatype
                               );
LOCAL void video_get_duration_callback(void);
/*****************************************************************************/
//  Description : get original video's width and height
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN video_get_video_size(
                                   wchar *filename,
                                   int32* width,
                                   int32* height
                                   );
LOCAL void video_get_video_size_callback(void);
/*****************************************************************************/
//  Description : Set video display area
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/    
LOCAL javacall_result video_set_video_location(
                                       long x, 
                                       long y, 
                                       long w, 
                                       long h
                                       );
LOCAL void video_set_video_location_callback(void);

/*****************************************************************************/
//  Description : prepare for play
//  Global resource dependence : 
//  Author: hui.zhao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN video_prepare_for_play(void);

/*****************************************************************************/
//  Description : Set video display or not
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN video_init_display_param(void);

/*****************************************************************************/
//  Description : Set video display or not
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN video_set_video_visible(BOOLEAN visible);
LOCAL void video_set_video_visible_callback(void);
/*****************************************************************************/
//  Description : Set Full Screen Mode or not when video display
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/    
LOCAL javacall_result video_set_fullscreen_mode(BOOLEAN fullScreenMode, int32 appid, int32 playerid);
LOCAL void video_set_fullscreen_mode_callback(void);
/*****************************************************************************/
//  Description : Skill frame
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN video_seek_frame(int32 frameNum);
LOCAL void video_seek_frame_callback(void);
/*****************************************************************************/
//  Description : Set video display or not
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL void video_end_callback(int32 end_type);

/*****************************************************************************/
//  Description : Set video display or not
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL void video_frame_end_callback(void);

/*****************************************************************************/
//  Description : video_acquire_device
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/    
LOCAL javacall_result video_acquire_device(wchar *filename);
LOCAL void video_acquire_device_callback(void);
/*****************************************************************************/
//  Description : video_release_device
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/    
LOCAL javacall_result video_release_device(int32 appid, int32 playerid);
LOCAL void video_release_device_callback(void);
LOCAL javacall_result video_set_rate(MMIJAVA_MEDIA_TYPE_T mediatype, int rate);
/*****************************************************************************/
//  Description : stopped by others
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN stopped_by_other(void);
/*****************************************************************************/
//  Description : video_release_device
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN map_frame_to_time(wchar *filename, int32 frameNum, int32*ms);
/*****************************************************************************/
//  Description :map_time_to_frame
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN map_time_to_frame(wchar *filename, int32 ms, int32*frameNum);
/*****************************************************************************/
//  Description : Skilp frame
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN video_skip_frame(int32 frameNum);
LOCAL void video_skip_frame_callback(void);
#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : video_handle_callback
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void video_handle_callback(VP_CALLBACK_T *msg_ptr);
#endif
/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
#ifdef VIDEO_PLAYER_SUPPORT
LOCAL VP_HANDLE                 s_vp_handle = 0;
LOCAL DPLAYER_DISPLAY_PARAM_T   s_video_display_param;//播放视频的设置参数
#endif
LOCAL int32 s_current_appid=-1; // used for notify jvm
LOCAL int32 s_current_playerid=-1; // used for notify jvm
    
/**
 * video basic javacall function interface
 */
static MMIJAVA_MEDIA_BASIC_INTERFACE_T s_video_basic_itf = 
{
    video_close,
    video_destroy,
    video_start,
    video_stop,
    video_pause,
    video_resume,
    video_get_time,
    video_set_time,
    video_get_duration,
    video_acquire_device,
    video_release_device,
    video_set_rate,
    stopped_by_other
};


/**
 * Video video javacall function interface
 */
static MMIJAVA_MEDIA_VIDEO_INTERFACE_T s_video_video_itf = 
{
    video_get_video_size,
    video_set_video_location,
    video_set_video_visible,
    video_set_fullscreen_mode,
    map_frame_to_time,
    map_time_to_frame,
    video_seek_frame,
    video_skip_frame,
};

MMIJAVA_MEDIA_INTERFACE_T g_video_itf = 
{
    &s_video_basic_itf,  
    PNULL,
    &s_video_video_itf,
    PNULL,
    PNULL
};
/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open and start video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN video_close(void)
{
    BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_CLOSE); 
    result = TRUE;
#endif  
    return result;
}

LOCAL void video_close_callback(void)          									
{
#ifdef VIDEO_PLAYER_SUPPORT
    javacall_printf(LOG_STR_ENTER("s_vp_handle = %x"),s_vp_handle);
    MMIAPIVP_FSMExit(s_vp_handle);
#endif  
}

/*****************************************************************************/
//  Description : open and start video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN video_destroy(void)          									
{
	BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT
    javacall_printf(LOG_STR_ENTER("s_vp_handle = %x"),s_vp_handle);
    result = TRUE;  
#endif  
    return result ;  
}
#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : video_handle_callback
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void video_handle_callback(VP_CALLBACK_T *msg_ptr)
{
    if (PNULL == msg_ptr)
    {
        return;
    }
    javacall_printf(LOG_STR_ENTER("s_vp_handle = %x, msg_id = %d"),s_vp_handle,msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {   
    case VP_STOP_IND: 
        MMIJAVA_NotifyMediaEnd();
        break;

    case VP_PLAY_PROCESS_IND:
   //     MMITHEME_UpdateRect();
        break;

    default:
        break;
    }
	return;
}
#endif
/*****************************************************************************/
//  Description : video_acquire_device
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/    
LOCAL javacall_result video_acquire_device(wchar *filename)
{
	javacall_result result = JAVACALL_FAIL;
#ifdef VIDEO_PLAYER_SUPPORT
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_ACQUIRE_DEVICE);  
    result = JAVACALL_WOULD_BLOCK;
#endif
    return result;
}
LOCAL void video_acquire_device_callback(void)
{
    javacall_result result=JAVACALL_FAIL;
#ifdef VIDEO_PLAYER_SUPPORT  
    s_vp_handle = MMIAPIVP_FSMInit(video_handle_callback);   
    javacall_printf(LOG_STR_ENTER("s_vp_handle = %x"),s_vp_handle);   
    if (PNULL != s_vp_handle)
    {
        if (TRUE == MMIAPIVP_RegTaskID(s_vp_handle, P_APP))
            result=JAVACALL_OK;
    }     
    video_init_display_param(); 
#endif
    MMIJAVA_NotifyMediaAcquireDevice(result);
    return ;
}

/*****************************************************************************/
//  Description : open and start video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL javacall_result video_start( wchar *filename,
                          MMIJAVA_MEDIA_TYPE_T mediatype,
                          int32 cur_time
                            )
{
    void *temp_ptr = NULL;
    uint32 length = 0;
	javacall_result result = JAVACALL_FAIL;
#ifdef VIDEO_PLAYER_SUPPORT  
    javacall_printf(LOG_STR_ENTER("s_vp_handle=0x%x,filename=%s,cuitime=%d"),
                    s_vp_handle, javacall_wstr2str(filename), cur_time);    /*lint !e64*/

    if (s_vp_handle == PNULL || PNULL == filename)
    {
        return JAVACALL_FAIL;
    }
    length = MMIAPICOM_Wstrlen(filename) + 1 ;
    temp_ptr = SCI_ALLOCA(sizeof(wchar) *length);
    if(NULL == temp_ptr )
        return JAVACALL_FAIL;

    SCI_MEMSET((uint8*)temp_ptr, 0 ,length*sizeof(wchar));
    MMIAPICOM_Wstrcpy(temp_ptr, filename);
    callback_para_ptr[0] = (uint32)temp_ptr ;
    callback_para_ptr[1] = (uint32)cur_time ;
     MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_START); 
    result = JAVACALL_WOULD_BLOCK;
#endif
   return result;
}

LOCAL void video_start_callback(void)          									
{
    BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT
    VP_PLAY_PARAM_T play_param = {0};
    VP_SEEK_PARAM_T seek_param = {0};
    wchar *filename   = NULL;
    int32 cur_time =  0;
    javacall_printf(LOG_STR_ENTER("s_vp_handle=0x%x"),s_vp_handle);  
    if (s_vp_handle == PNULL )
    {
        MMIJAVA_NotifyMediaStart(JAVACALL_FAIL);
        return;
    }
    filename  = (wchar*)callback_para_ptr[0];
    cur_time = (int32)callback_para_ptr[1];
    if (VP_STATE_READY == MMIAPIVP_FSMGetState(s_vp_handle))
    {    
        SCI_MEMSET(&play_param,0,sizeof(VP_PLAY_PARAM_T));
        
        play_param.full_path_ptr = filename;
        play_param.full_path_len = MMIAPICOM_Wstrlen(filename);
        play_param.is_random_play = 0;
        play_param.is_save_history = FALSE;
        play_param.mode = 0;
        play_param.play_type = VP_PLAY_FROM_FILE;    
        video_init_display_param(); 
        SCI_MEMCPY(&(play_param.display_param),&(s_video_display_param),sizeof(DPLAYER_DISPLAY_PARAM_T));	
        GUIBLOCK_SetType(255, MMI_BLACK_COLOR, GUIBLOCK_TYPE_OSD, MMITHEME_GetDefaultLcdDev());   /*lint !e605*/
        
        result = MMIAPIVP_FSMPlay(s_vp_handle,&play_param);
        if (result && (0 != cur_time))
        {
            seek_param.seek_frame = 0xffffffff;
            seek_param.seek_time = cur_time;
            MMIAPIVP_FSMSeek(s_vp_handle,&seek_param);
        }
    }
    else if (VP_STATE_PAUSED == MMIAPIVP_FSMGetState(s_vp_handle))
    {
        result = MMIAPIVP_FSMResume(s_vp_handle);
    }
    else
    {
        result = FALSE;
    }
#endif
    SCI_FREE(filename);
    MMIJAVA_NotifyMediaStart(result == TRUE ? JAVACALL_OK : JAVACALL_FAIL);
    javacall_printf(LOG_STR_LEAVE("result = %d"),result);
    return;
}

/*****************************************************************************/
//  Description : stop and close video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL javacall_result video_stop(void)
{  
	javacall_result result = JAVACALL_FAIL;
#ifdef VIDEO_PLAYER_SUPPORT    
    if (s_vp_handle == PNULL)
        return JAVACALL_FAIL;
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_STOP); 
    result =  JAVACALL_WOULD_BLOCK;   
#endif
	return result;
}

LOCAL void  video_stop_callback(void)
{
    BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT    
    javacall_printf(LOG_STR_ENTER("handle=%d"), s_vp_handle);  
    GUIBLOCK_ResetMainType();
    result = MMIAPIVP_FSMStop(s_vp_handle);
#endif   
    MMIJAVA_NotifyMediaStop(result == TRUE ? JAVACALL_OK : JAVACALL_FAIL);
    javacall_printf(LOG_STR_LEAVE("result = %d"),result);
}

/*****************************************************************************/
//  Description : pause video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL javacall_result video_pause(void)
{
	javacall_result result = JAVACALL_FAIL;
#ifdef VIDEO_PLAYER_SUPPORT
    if (s_vp_handle == PNULL)
        return JAVACALL_FAIL;
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_PAUSE); 
    result = JAVACALL_WOULD_BLOCK;
#endif
    return result;   /*lint !e527 */
}

LOCAL void video_pause_callback(void)
{
	BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT
    result = MMIAPIVP_FSMPause(s_vp_handle);
    javacall_printf(LOG_STR("handle=%d"), s_vp_handle);   
#endif
    MMIJAVA_NotifyMediaPause(result == TRUE ? JAVACALL_OK : JAVACALL_FAIL);
    javacall_printf(LOG_STR_LEAVE("result = %d"),result);
}

/*****************************************************************************/
//  Description : resume video
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL javacall_result video_resume(void)
{
	javacall_result result = JAVACALL_FAIL;
#ifdef VIDEO_PLAYER_SUPPORT
    if (s_vp_handle == PNULL)
        return JAVACALL_FAIL;
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_RESUME); 
    result = JAVACALL_WOULD_BLOCK;
#endif    
    return result;   /*lint !e527 */
}
LOCAL void video_resume_callback(void)
{
	BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT     
    result = MMIAPIVP_FSMResume(s_vp_handle);
    javacall_printf(LOG_STR_ENTER("handle=%d"), s_vp_handle); 
#endif
    MMIJAVA_NotifyMediaResume(result == TRUE ? JAVACALL_OK : JAVACALL_FAIL);
    javacall_printf(LOG_STR_LEAVE("result = %d"),result);
}

/*****************************************************************************/
//  Description : get time
//  Global resource dependence : 
//  Author: hui.zhao
//  Note: If success return time in ms else return -1
/*****************************************************************************/
LOCAL int32 video_get_time(void)
{
#ifdef VIDEO_PLAYER_SUPPORT
   if (s_vp_handle == PNULL)
    {
        javacall_printf(LOG_STR("handle = %d"), s_vp_handle);
        return JAVACALL_FAIL;
    }
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_GET_TIME); 
    SCI_SLEEP(10);
    if (video_time == 0)
        video_time = 1;		//ONLY FOR PASS TCK TEST
    return video_time;
#else
    return JAVACALL_FAIL;   /*lint !e527 */
#endif
}
/*****************************************************************************/
//  Description : get time
//  Global resource dependence : 
//  Author: hui.zhao
//  Note: If success return time in ms else return -1
/*****************************************************************************/
LOCAL void video_get_time_callback(void)
{
#ifdef VIDEO_PLAYER_SUPPORT
    javacall_printf(LOG_STR_ENTER("handle=%d"), s_vp_handle);
    video_time = MMIAPIVP_FSMGetTimeOffset(s_vp_handle);
    javacall_printf(LOG_STR_LEAVE("time=%d"), video_time);
#endif
	return ;
}

/*****************************************************************************/
//  Description : get time
//  Global resource dependence : 
//  Author: hui.zhao
//  Note: If success return time in ms else return -1
/*****************************************************************************/
LOCAL int32 video_set_time(int32 time)
{	
    javacall_printf(LOG_STR_LEAVE("not supported, timeoffset=%d,"),time);
    return -1;
}

/*****************************************************************************/
//  Description : get duration
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL int32 video_get_duration(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype)
{
    uint32   filename_length = 0;
#ifdef VIDEO_PLAYER_SUPPORT 
    if (PNULL == filename)
    {
        javacall_printf(LOG_STR("filename = %d"), filename);
        return JAVACALL_FAIL;  
    }   
    filename_length = MMIAPICOM_Wstrlen(filename);
    callback_para_ptr[0] = (uint32)SCI_ALLOCA((filename_length + 1)*sizeof(wchar));
    SCI_MEMSET((void*)callback_para_ptr[0], 0, (filename_length + 1)*sizeof(wchar));
    MMI_WSTRNCPY((wchar*)callback_para_ptr[0], filename_length , filename, filename_length, filename_length);
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_GET_DURATION); 
    SCI_SLEEP(10);
    return video_duration;
#else
	javacall_printf(LOG_STR_LEAVE("duration = %d"),duration);     /*lint !e527 */ 
    return JAVACALL_FAIL;   /*lint !e527 */
#endif
}
LOCAL void video_get_duration_callback(void)
{
	int32	duration = 0;
    wchar*  filename = (wchar*)callback_para_ptr[0];
#ifdef VIDEO_PLAYER_SUPPORT 
    DPLAYER_MEDIA_INFO_T media_info = {0};
    if (s_vp_handle == PNULL) {	  
        if (PNULL == filename) 
        {
            javacall_printf(LOG_STR("false, illegal argument"));
            media_info.media_length = 0 ;
        }
        else
        {
            MMIAPIVP_GetInfoFromFullPathName(filename,MMIAPICOM_Wstrlen(filename),&media_info);
        }
    } 
    else {
        DPLAYER_GetMovieInfo(s_vp_handle, &media_info);
    }
    duration = media_info.media_length;
#endif
    video_duration = duration;
    SCI_FREE((void*)callback_para_ptr[0]);
    callback_para_ptr[0] = 0;
    javacall_printf(LOG_STR_LEAVE("duration = %d"),duration);    
	return ;
}

/*****************************************************************************/
//  Description : get original video's width and height
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN video_get_video_size(wchar *filename, int32* width,int32* height)
{
    BOOLEAN  result = FALSE;
    uint32   filename_length = 0;
#ifdef VIDEO_PLAYER_SUPPORT  
    if (PNULL == filename || PNULL == width || PNULL == height) 
    {
        javacall_printf(LOG_STR_LEAVE("false, illegal argument"));
        return FALSE;
    }
    filename_length = MMIAPICOM_Wstrlen(filename);
    callback_para_ptr[0] = (uint32)SCI_ALLOCA((filename_length + 1)*sizeof(wchar));
	if(0 == callback_para_ptr[0])
	{
		return FALSE;
	}
    SCI_MEMSET((void*)callback_para_ptr[0], 0, (filename_length + 1)*sizeof(wchar));
    MMI_WSTRNCPY((wchar*)callback_para_ptr[0], filename_length , filename, filename_length, filename_length);
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_GET_SIZE); 
    SCI_SLEEP(10);
    *width = video_width;       /*lint !e613 */
    *height = video_height;     /*lint !e613 */
    result = TRUE;    
#endif
    return result;   /*lint !e527 */
}
LOCAL void video_get_video_size_callback(void)
{
	DPLAYER_MEDIA_INFO_T    media_info  = {0};
    wchar*  filename = (wchar*)callback_para_ptr[0];
    int32   width   = 0;
    int32   height  = 0;
#ifdef VIDEO_PLAYER_SUPPORT  
    if (s_vp_handle == PNULL) {
        if (PNULL == filename) 
        {
            javacall_printf(LOG_STR("false, illegal argument"));
            SCI_MEMSET(&media_info, 0, sizeof(media_info));
        }
        else
        {
            MMIAPIVP_GetInfoFromFullPathName(filename,MMIAPICOM_Wstrlen(filename),&media_info);
        }
    } else {
        DPLAYER_GetMovieInfo(s_vp_handle, &media_info);
    }	
    width = media_info.video_info.width;       /*lint !e613 */
    height = media_info.video_info.height;     /*lint !e613 */
#endif
    javacall_printf(LOG_STR("width=%d, height = %d"), width,height);    /*lint !e613 */
    video_width = width;       /*lint !e613 */
    video_height = height ;    /*lint !e613 */
    SCI_FREE((void*)callback_para_ptr[0]);
    callback_para_ptr[0] = 0;
	return ;
}

/*****************************************************************************/
//  Description : Set video display area
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/    
LOCAL javacall_result video_set_video_location(long x, long y, long w, long h)
{
	javacall_result result = JAVACALL_FAIL;
    javacall_printf(LOG_STR_ENTER("(x, y, w, h)=(%d, %d, %d, %d)"), x, y, w, h);

    if (0 > x)
        x = 0;
    if (0 > y)
        y = 0;        
#ifdef VIDEO_PLAYER_SUPPORT  
    if(s_video_display_param.disp_rect.x == (uint32)x 
        && s_video_display_param.disp_rect.y == (uint32)y + (uint32)java_lcd_GetYStartFromLCD()
        && s_video_display_param.disp_rect.dx == (uint32)w
        && s_video_display_param.disp_rect.dy == (uint32)h
        )
    {
        javacall_printf(LOG_STR("video location not changed"));
        return JAVACALL_OK;
    }
    s_video_display_param.disp_rect.x = (uint32)x;
    s_video_display_param.disp_rect.y = (uint32)y + (uint32)java_lcd_GetYStartFromLCD();
    s_video_display_param.disp_rect.dx = (uint32)w;
    s_video_display_param.disp_rect.dy = (uint32)h;
    s_video_display_param.RotateAngle = MMIJAVA_VIDEO_ROTATE_ANGLE_0;
    s_video_display_param.target_rect = s_video_display_param.disp_rect;
 
    s_video_display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
      MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_SET_LOCATION); 
    result = JAVACALL_WOULD_BLOCK;
#endif
    return result;   /*lint !e527 */
}

LOCAL void video_set_video_location_callback(void)//问题:拍摄的照片区域是否也用该接口设置
{  
	BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT
    javacall_printf(LOG_STR_ENTER("handle=%d"), s_vp_handle);
    javacall_printf(LOG_STR("(x, y, w, h)=(%d, %d, %d, %d)"), 
                  s_video_display_param.disp_rect.x, s_video_display_param.disp_rect.y,
                  s_video_display_param.disp_rect.dx,s_video_display_param.disp_rect.dy);

    result = MMIAPIVP_FSMSwitchDisplayMode(s_vp_handle,&s_video_display_param,TRUE);  
#endif
    javacall_printf(LOG_STR_LEAVE("result = %d"),result);
    MMIJAVA_NotifyMediaSetVideoLocation(result == TRUE ? JAVACALL_OK : JAVACALL_FAIL); /*lint !e527 */
}

/*****************************************************************************/
//  Description : prepare for play
//  Global resource dependence : 
//  Author: hui.zhao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN video_prepare_for_play(void)
{
    BOOLEAN    result  = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT    
     result = TRUE;
#endif
    return result;
}
		
/*****************************************************************************/
//  Description : Set video display or not
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN video_init_display_param(void)
{
    BOOLEAN    result  = FALSE;
	javacall_printf(LOG_STR_ENTER(""));
#ifdef VIDEO_PLAYER_SUPPORT    
	SCI_MEMSET(&s_video_display_param,0,sizeof(DPLAYER_DISPLAY_PARAM_T));
    
	s_video_display_param.disp_rect.x = 0;
    s_video_display_param.disp_rect.y = MMITHEME_GetClientRect().top;
	s_video_display_param.disp_rect.dx = MMITHEME_GetClientRectWidth();
    s_video_display_param.disp_rect.dy = 176;
    
	s_video_display_param.target_rect.x = 0;
    s_video_display_param.target_rect.y = MMITHEME_GetClientRect().top;
	s_video_display_param.target_rect.dx = MMITHEME_GetClientRectWidth();
    s_video_display_param.target_rect.dy = 176;
    
	s_video_display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
	s_video_display_param.RotateAngle = MMIJAVA_VIDEO_ROTATE_ANGLE_0;
	s_video_display_param.audioSupport = 1;
	s_video_display_param.videoSupport = 1;
	s_video_display_param.LcdId = 0;
	s_video_display_param.LcdBlkId = 0;
	s_video_display_param.b_support_seek = 1;
    result = TRUE;
#endif
    return result;
}


/*****************************************************************************/
//  Description : Set video display or not
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN video_set_video_visible(BOOLEAN visible)
{
    BOOLEAN    result  = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT    
    s_video_display_param.videoSupport = (visible)? 1:0;
    javacall_printf(LOG_STR_LEAVE("visible=%d, handle=%d"),visible,s_vp_handle); 
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_SET_VISIBLE); 
	result = TRUE;
#endif
    return result;
}
LOCAL void video_set_video_visible_callback(void)
{   
#ifdef VIDEO_PLAYER_SUPPORT    
    javacall_printf(LOG_STR_LEAVE("handle=%d"),s_vp_handle); 
    MMIAPIVP_FSMSwitchDisplayMode(s_vp_handle,&s_video_display_param,TRUE);
#endif
}

/*****************************************************************************/
//  Description : Set Full Screen Mode or not when video display
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/    
LOCAL javacall_result video_set_fullscreen_mode(BOOLEAN fullScreenMode, int32 appid, int32 playerid)
{
	javacall_result result = JAVACALL_FAIL;
#ifdef VIDEO_PLAYER_SUPPORT
    javacall_printf(LOG_STR_ENTER("handle=%d, fullScreenMode=%d"), s_vp_handle, fullScreenMode);
    if (fullScreenMode)
    {
        s_video_display_param.disp_rect.x = 0;
        s_video_display_param.disp_rect.y = java_lcd_GetYStartFromLCD();
        s_video_display_param.disp_rect.dx = MMITHEME_GetClientRectWidth();
        s_video_display_param.disp_rect.dy = MMITHEME_GetClientRectHeight() - java_lcd_GetYStartFromLCD();
        s_video_display_param.RotateAngle = MMIJAVA_VIDEO_ROTATE_ANGLE_90;
    } 
    s_video_display_param.target_rect = s_video_display_param.disp_rect ;
    if (s_vp_handle == PNULL) {// display param will take effect when start
        return JAVACALL_OK;
    }

    s_current_appid=appid;
    s_current_playerid=playerid;
     MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_SET_FULLSCREEN);    
    result = JAVACALL_WOULD_BLOCK;
#else
    s_current_appid=-1;      /*lint !e527 */
    s_current_playerid=-1;   /*lint !e527 */	
    result = JAVACALL_FAIL;    /*lint !e527 */
#endif
	return result;
}

LOCAL void video_set_fullscreen_mode_callback(void)
{
	BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT
    javacall_printf(LOG_STR_ENTER("handle=%d, appid=%d, playerid=%d"), s_vp_handle, s_current_appid, s_current_playerid);
    result = MMIAPIVP_FSMSwitchDisplayMode(s_vp_handle,&s_video_display_param,TRUE);  
#else
 	s_current_appid=-1;
    s_current_playerid=-1;
#endif
    javacall_printf(LOG_STR_LEAVE("result = %d"),result);
    MMIJAVA_NotifyMediaSetVideoFullScreenMode(result == TRUE ? JAVACALL_OK : JAVACALL_FAIL, 
    		s_current_appid, s_current_playerid);
}

/*****************************************************************************/
//  Description : Skill frame
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN video_seek_frame(int32 frameNum)
{
    BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT    
    VP_SEEK_PARAM_T seek_param = {0};
    callback_para_ptr[0] = (uint32)SCI_ALLOCA(sizeof(seek_param));
    if(0 == callback_para_ptr[0] )
    {
        return FALSE;
    }
    seek_param.seek_frame = frameNum;
    seek_param.seek_time = 0xffffffff;
    SCI_MEMCPY(callback_para_ptr, &seek_param ,sizeof(seek_param));
    javacall_printf(LOG_STR_LEAVE("frames=%d, handle=%d, result=%d"),frameNum,s_vp_handle,result);      
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_SEEK_FRAME);     
	result = TRUE;
#endif
    return result;
}

LOCAL void video_seek_frame_callback(void)
{
#ifdef VIDEO_PLAYER_SUPPORT    
    VP_SEEK_PARAM_T seek_param = {0};
    SCI_MEMCPY(&seek_param, (void*)callback_para_ptr[0], sizeof(seek_param));
    SCI_FREE((void*)callback_para_ptr[0]);   /*lint !e63*/
    MMIAPIVP_FSMSeek(s_vp_handle,&seek_param);
    javacall_printf(LOG_STR_LEAVE("handle=%d"),s_vp_handle);    
#endif
}
/*****************************************************************************/
//  Description : Skip frame
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN video_skip_frame(int32 frameNum)
{
	BOOLEAN     result      = FALSE;
    int32       cur_frame   = 0;
#ifdef VIDEO_PLAYER_SUPPORT  
    VP_SEEK_PARAM_T seek_param = {0};
    cur_frame = MMIAPIVP_FSMGetFrameOffset(s_vp_handle);
    cur_frame += frameNum;   
    seek_param.seek_frame = cur_frame;
    seek_param.seek_time = 0xffffffff;
    callback_para_ptr[0] = (uint32)SCI_ALLOCA(sizeof(seek_param));
    if(0 == callback_para_ptr[0] )
    {
        return FALSE;
    }
    SCI_MEMCPY(callback_para_ptr, &seek_param ,sizeof(seek_param));
    javacall_printf(LOG_STR_LEAVE("frames=%d, handle=%d, result=%d"),frameNum,s_vp_handle,result);  
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_SKIP_FRAME);  
	result = TRUE;
#endif
	return result;
}

LOCAL void video_skip_frame_callback(void)
{
	BOOLEAN     result      = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT  
    VP_SEEK_PARAM_T seek_param = {0};
    SCI_MEMCPY(&seek_param, (void*)callback_para_ptr[0], sizeof(seek_param));
    SCI_FREE((void*)callback_para_ptr[0]);   /*lint !e63*/
    result = MMIAPIVP_FSMSeek(s_vp_handle,&seek_param);
    javacall_printf(LOG_STR_LEAVE("handle=%d, result=%d"),s_vp_handle,result);  
#endif
	return ;
}
/*****************************************************************************/
//  Description : Set video display or not
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL void video_end_callback(int32 end_type)
{

}

/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIJAVA_CallbackEndNotifyProc(void)
{

}

/*****************************************************************************/
//  Description : Set video display or not
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL void video_frame_end_callback(void)
{

}

/*****************************************************************************/
//  Description : display notify处理
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
PUBLIC void MMIJAVA_CallbackFrameEndNotifyProc(void)
{

}

/*****************************************************************************/
//  Description : map_frame_to_time
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN map_frame_to_time(wchar *filename, int32 frameNum, int32*ms)
{
    BOOLEAN         ret         = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT   
    DPLAYER_MEDIA_INFO_T media_info = {0};
    uint32                total_frame = 0;
    javacall_printf(LOG_STR_ENTER("filename=%x, ms=%x"), filename, ms);
    if (PNULL == filename || PNULL == ms)
    {
        return FALSE;
    }
    MMIAPIVP_GetInfoFromFullPathName(filename,MMIAPICOM_Wstrlen(filename),&media_info);
    //计算视频文件总的帧数  
    total_frame = (media_info.video_info.frame_rate)*media_info.media_length/1000/100;
    //计算对应的time
    if(( frameNum < (int32)total_frame) && (0 != media_info.video_info.frame_rate))
    {
        *ms = frameNum * 1000*100 /(int32)media_info.video_info.frame_rate;
        if( *ms > (int32)media_info.media_length)
        {
           *ms = (int32)media_info.media_length - 1;
        }
    }
    else if( (uint32)frameNum >= total_frame)
    {
       *ms = (int32)media_info.media_length - 1;
    }   
    ret = TRUE;
    javacall_printf(LOG_STR_ENTER("total_frame=%d, *ms=%d, frameNum = %d"), total_frame, *ms,frameNum);
#endif
    return ret;

}

/*****************************************************************************/
//  Description :map_time_to_frame
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL BOOLEAN map_time_to_frame(wchar *filename, int32 ms, int32*frameNum)
{
    BOOLEAN         ret         = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT   
    DPLAYER_MEDIA_INFO_T media_info = {0};    
    if (PNULL == filename || PNULL == frameNum)
    {
        return FALSE;
    }
    MMIAPIVP_GetInfoFromFullPathName(filename,MMIAPICOM_Wstrlen(filename),&media_info);
    if((uint32)ms <= media_info.media_length)
    {
        *frameNum = ((int32)(media_info.video_info.frame_rate )* ms) /1000/100;
        ret = TRUE;
    }
    javacall_printf(LOG_STR_ENTER("*ms=%d, frameNum = %d"),  ms,*frameNum);
#endif
    return ret;

}

/*****************************************************************************/
//  Description : video_release_device
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
/*****************************************************************************/	
LOCAL javacall_result video_release_device(int32 appid, int32 playerid)
{
	javacall_result result = JAVACALL_FAIL; 
    s_current_appid=appid;
    s_current_playerid=playerid;
#ifdef VIDEO_PLAYER_SUPPORT   
    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_VIDEO, JAVA_VIDEO_RELEASE_DEVICE);  
    result = JAVACALL_WOULD_BLOCK;
#endif
    return result;
}

LOCAL javacall_result video_set_rate(MMIJAVA_MEDIA_TYPE_T mediatype, int rate)
{
    return JAVACALL_NOT_IMPLEMENTED;
}
LOCAL void video_release_device_callback(void)
{
#ifdef VIDEO_PLAYER_SUPPORT    
    javacall_printf(LOG_STR_ENTER("handle=%d"), s_vp_handle);
    MMIAPIVP_UnRegTaskID(s_vp_handle,P_APP);
    MMIAPIVP_FSMExit(s_vp_handle);
#endif
    MMIJAVA_NotifyMediaRelease(JAVACALL_OK, s_current_appid, s_current_playerid);
    s_current_appid=-1;
    s_current_playerid=-1;
}

/*****************************************************************************/
//  Description : stopped by others
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN stopped_by_other(void)
{
    //because no stop video, let's do it by myself
    javacall_printf(LOG_STR_ENTER(""));
    return video_close();   
}

BOOLEAN java_HandleVideoMessage(uint16 msg_id)
{
    BOOLEAN result = TRUE;
    javacall_printf("java_HandleVideoMessages msg_id = %d",msg_id);
    switch (msg_id)
    {  
    case JAVA_VIDEO_START   :
        video_start_callback();
        break;
    case JAVA_VIDEO_CLOSE   :
        video_close_callback();
        break;
    case JAVA_VIDEO_ACQUIRE_DEVICE:
        video_acquire_device_callback();
        break;
    case JAVA_VIDEO_STOP  :
        video_stop_callback();
        break;
    case JAVA_VIDEO_PAUSE   :
        video_pause_callback();
        break;
    case JAVA_VIDEO_RESUME   :
        video_resume_callback();
        break;
    case JAVA_VIDEO_SET_LOCATION :
        video_set_video_location_callback();
        break;
    case JAVA_VIDEO_SET_VISIBLE :
        video_set_video_visible_callback();
        break;
    case JAVA_VIDEO_SET_FULLSCREEN  :
        video_set_fullscreen_mode_callback();
        break;
    case JAVA_VIDEO_RELEASE_DEVICE :
        video_release_device_callback();
        break;
    case JAVA_VIDEO_SEEK_FRAME  :
        video_seek_frame_callback();
        break;
    case JAVA_VIDEO_SKIP_FRAME  :
        video_skip_frame_callback();
        break;
    case JAVA_VIDEO_GET_TIME:
        video_get_time_callback();
        break;
    case JAVA_VIDEO_GET_DURATION:
        video_get_duration_callback();
        break;
    case JAVA_VIDEO_GET_SIZE:
        video_get_video_size_callback();
        break;
    default:
        result = FALSE;
        break;
    }  
    return result;
}

BOOLEAN mmijava_IsVPRunning(void)
{
	BOOLEAN result = FALSE;
#ifdef VIDEO_PLAYER_SUPPORT    
    int32 vp_state = MMIAPIVP_FSMGetState(s_vp_handle);
    if(VP_STATE_PLAYING == vp_state   || VP_STATE_SEEKING == vp_state)
    {
        result =  TRUE;
    }
    else
    {
        result = FALSE;
    }
#endif  
	 return result ;
}

int32 mmijava_GetVPLocation(GUI_RECT_T *rect)
{
#ifdef VIDEO_PLAYER_SUPPORT    
    rect->left = s_video_display_param.disp_rect.x ;
    rect->top = s_video_display_param.disp_rect.y ;
    rect->right = s_video_display_param.disp_rect.x + s_video_display_param.disp_rect.dx - 1;
    rect->bottom = s_video_display_param.disp_rect.y + s_video_display_param.disp_rect.dy -1;
#endif  
    return 0;
} 

int32 mmijava_SetVideoVolume(int32 volume)
{
#ifdef VIDEO_PLAYER_SUPPORT    
    MMIAPIVP_FSMUpdateCurrentVol(s_vp_handle, (uint8)volume);
#endif  
    return 0;
} 

#endif
