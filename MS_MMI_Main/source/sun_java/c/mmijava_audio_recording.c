/*****************************************************************************
** File Name:      mmijava_auido_recording.c                                 *
** Author:         apple.zhang                                               *
** Date:           6/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2009       apple.zhang            Create                                *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#ifdef JAVA_SUPPORT_SUN
#include "javacall_multimedia.h"
#include "mmijava_multimedia.h"
#include "mmiaudio.h"
#include "javacall_logging.h"
#include "mmk_app.h" 

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
 //java audio state
typedef enum
{    
    MMIJAVA_AUDIO_RECORDING, //play
    MMIJAVA_AUDIO_RECORDING_PAUSED,  //pause
    MMIJAVA_AUDIO_RECORDING_STOPPED, //stop
    MMIJAVA_AUDIO_RECORDING_STATE_MAX/*lint !e749*/
} MMIJAVA_AUDIO_RECORDING_STATE_E;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : stopped by others
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN stopped_by_other(void);

/*****************************************************************************/
//  Description : start recording
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN start_recording(wchar *filename);

/*****************************************************************************/
//  Description : pause recording
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN pause_recording(void);

/*****************************************************************************/
//  Description : stop recording
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN stop_recording(void);

/*****************************************************************************/
//  Description : reset recording
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN reset_recording(wchar *filename);

/*****************************************************************************/
//  Description : get recorded data size
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN get_recorded_data_size(wchar *filename,int32 *size);

/*****************************************************************************/
//  Description : get recorded data
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN get_recorded_data(wchar *filename,char *buf, int32 offset, int32 size);

/*****************************************************************************/
//  Description : get record content type length
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL int get_record_content_type_length(void);

/*****************************************************************************/
//  Description : get record content type
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL int get_record_content_type(uint16* content_buf);

/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
LOCAL MMIFILE_HANDLE s_sfs_handle = 0;
LOCAL MMIJAVA_AUDIO_RECORDING_STATE_E s_cur_recording_state = MMIJAVA_AUDIO_RECORDING_STOPPED;
static MMIJAVA_MEDIA_BASIC_INTERFACE_T s_audio_recording_basic_itf = 
{   
    NULL,
    NULL,    
    NULL,
    NULL,
    NULL,
    NULL,    
    NULL,
    NULL,
    NULL,   
    NULL,
    NULL,
    NULL,
    stopped_by_other
};
static MMIJAVA_MEDIA_RECORD_INTERFACE_T s_audio_recording_itf = 
{   
    start_recording,
    pause_recording,
    stop_recording,
    reset_recording,
    get_recorded_data_size,
    get_recorded_data,
    get_record_content_type_length,
    get_record_content_type   
};

PUBLIC MMIJAVA_MEDIA_INTERFACE_T g_audio_recording_itf = 
{
    NULL,    
    &s_audio_recording_itf,
    NULL,
    NULL,
    PNULL
};
/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : stopped by others
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN stopped_by_other(void)
{
    if ((MMIJAVA_AUDIO_RECORDING_PAUSED ==  s_cur_recording_state)
        || (MMIJAVA_AUDIO_RECORDING_PAUSED ==  s_cur_recording_state))
    {
        javacall_printf(LOG_STR("stop recodrding, s_cur_recording_state=%d"), s_cur_recording_state);
        stop_recording();
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : start recording
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN start_recording(wchar *filename)
{
    BOOLEAN             ret         = FALSE;
    javacall_printf(LOG_STR_ENTER("start_recording s_cur_recording_state = %d, file=%s"),s_cur_recording_state, javacall_wstr2str(filename));/*lint !e64*/

    if (MMIJAVA_AUDIO_RECORDING_PAUSED ==  s_cur_recording_state)
    {
        ret = MMIAUDIO_ResumeRecord();
    }
    else
    {
        if (MMIAPIFMM_IsFileExist(filename, MMIAPICOM_Wstrlen(filename)) == TRUE)
                MMIAPIFMM_DeleteFile(filename, PNULL);
        s_sfs_handle = MMIAPIFMM_CreateFile(filename, SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, NULL, NULL);/*lint !e655*/

        if (0 != s_sfs_handle)
        {
            if(MMIAUDIO_StartRecord(filename,s_sfs_handle,MMIAUDIO_RECORD_FORMAT_AMR,MMIAUDIO_RECORD_DEFAULT_FRAME_LEN,FALSE,NULL))        
            {
                s_cur_recording_state = MMIJAVA_AUDIO_RECORDING;
                ret = TRUE;
            }
		    else
		    {
                javacall_printf(LOG_STR("start record failed"));
   	 		    MMIAPIFMM_CloseFile(s_sfs_handle);
			    ret = FALSE;
		    }
        } else {javacall_printf(LOG_STR("create file failed"));}
    }
    

    javacall_printf(LOG_STR_LEAVE("start_recording ret = %d, s_sfs_handle = %d"), ret,s_sfs_handle);
    return ret;
    
}

/*****************************************************************************/
//  Description : pause recording
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN pause_recording(void)
{
    BOOLEAN         result      = FALSE;
    
    if(MMIAUDIO_PauseRecord())
    {    
        s_cur_recording_state = MMIJAVA_AUDIO_RECORDING_PAUSED;
        result = TRUE;
    }
    javacall_printf(LOG_STR_LEAVE("pause_recording result = %d"),result);
    return result;
}

/*****************************************************************************/
//  Description : stop recording
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN stop_recording(void)
{    
    javacall_printf(LOG_STR_ENTER("stop_recording"));
    MMIAUDIO_StopRecord();
    MMIAUDIO_EndRecord();
    MMIAPIFMM_CloseFile(s_sfs_handle);
    s_sfs_handle = 0;
    s_cur_recording_state = MMIJAVA_AUDIO_RECORDING_STOPPED;
    return TRUE;
}

/*****************************************************************************/
//  Description : reset recording
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN reset_recording(wchar *filename)
{
    javacall_printf(LOG_STR_ENTER("reset_recording"));
    MMIAUDIO_StopRecord();
    MMIAUDIO_EndRecord();
    MMIAPIFMM_CloseFile(s_sfs_handle);
    s_cur_recording_state = MMIJAVA_AUDIO_RECORDING_STOPPED;
//    if(SFS_NO_ERROR == MMIAPIFMM_DeleteFile(filename,PNULL))
//    {
//        javacall_printf(LOG_STR_LEAVE("reset_recording delete file success"));
        return TRUE;
//    }
//    else
//    {
//        return FALSE;
//    }

}

/*****************************************************************************/
//  Description : get recorded data size
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN get_recorded_data_size(wchar *filename,int32 *size)
{
    BOOLEAN         result          = TRUE;

    if (!MMIAPIFMM_IsFileExist(filename,MMIAPICOM_Wstrlen(filename)))
    {
        *size = -1;
	    result = FALSE;
    }
    else
    {    
        s_sfs_handle = MMIAPIFMM_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        javacall_printf(LOG_STR("get_recorded_data_size s_sfs_handle =%d"),s_sfs_handle);

        if (PNULL != s_sfs_handle)
        {    
            *size = MMIAPIFMM_GetFileSize(s_sfs_handle);
        	MMIAPIFMM_CloseFile(s_sfs_handle);
        }
        else
        {
            *size = -1;
            result = FALSE;
        }

    }

    javacall_printf(LOG_STR_LEAVE("get_recorded_data_size size = %d"), *size);
    return result;
    

}

/*****************************************************************************/
//  Description : get recorded data
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN get_recorded_data(wchar *filename,char *buf, int32 offset, int32 size)
{
    uint32                      read_size       = 0;   
    BOOLEAN                     result          = TRUE;
    
    javacall_printf(LOG_STR_ENTER("get_recorded_data offset = %d, size = %d"), offset,size);
    
    s_sfs_handle = MMIAPIFMM_CreateFile(filename, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

    
    if (SFS_INVALID_HANDLE != s_sfs_handle)
    {
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(s_sfs_handle, offset, SFS_SEEK_BEGIN))
        {        
            if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(s_sfs_handle,buf,(uint32)size,&read_size,NULL))
            {
                javacall_printf(LOG_STR("get_recorded_data read file error"));
                result = FALSE;                
            }       
        }
        else
        {
            javacall_printf(LOG_STR("get_recorded_data set file pointer error file_handle = %d"),s_sfs_handle);
            result = FALSE;            
        }
    }
    else
    {    
        javacall_printf(LOG_STR("get_recorded_data create file error"));
        result = FALSE;
    }

    javacall_printf(LOG_STR_LEAVE("get_recorded_data read_size = %d"),read_size);
    MMIAPIFMM_CloseFile(s_sfs_handle);

    return result;
   
}

/*****************************************************************************/
//  Description : get record content type length
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL int get_record_content_type_length(void)
{
    wchar contentType[] = L"audio/amr";

    javacall_printf(LOG_STR_LEAVE("length=%d"), sizeof(contentType) / sizeof(*contentType)-1);

    return sizeof(contentType) / sizeof(*contentType) - 1; 
}

/*****************************************************************************/
//  Description : get record content type
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL int get_record_content_type(uint16* content_buf)
{
    wchar contentType[] = L"audio/amr";
    
    javacall_printf(LOG_STR(""));
    MMIAPICOM_Wstrcpy(content_buf,contentType);   

    return sizeof(contentType) / sizeof(*contentType) - 1;
}

#endif
