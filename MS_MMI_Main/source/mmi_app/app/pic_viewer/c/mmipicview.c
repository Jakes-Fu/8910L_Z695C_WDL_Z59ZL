/*****************************************************************************
** File Name:      mmipicview.c                                              *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIPICVIEW_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pic_viewer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmipicview_id.h"
#include "mmipicview_nv.h"
#include "mmifilearray_export.h"
#include "mmifmm_export.h"
#include "mmi_appmsg.h"
//#include "mmipicview_text.h"
#include "window_parse.h"
#include "mmipicview_internal.h"
#include "mmisd_export.h"
#include "mmipicview_menutable.h"
#include "mmipicview_position.h"
#include "mmi_nv.h"
#include "mmisrv_cameraroll_export.h"
#ifdef MSA_SUPPORT
#include "mmimsa_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 //zhuyongjing add for gallary ui
#define MMI_PICVIEW_MINI_SUPPORT TRUE
#define UPLOADING_MAX_NUM 1024
LOCAL wchar *s_gallery_list_file_folder = L"D:\\System\\Gallery";
LOCAL wchar *s_gallery_list_file_temp_path = L"D:\\System\\Gallery\\temp_filelist.txt";
LOCAL wchar *s_gallery_list_file_main_path = L"D:\\System\\Gallery\\main_filelist.txt";
LOCAL wchar *s_gallery_list_file_local_path = L"D:\\System\\Gallery\\local_filelist.txt";
LOCAL wchar *s_gallery_list_file_capture_path = L"D:\\System\\Gallery\\capture_filelist.txt";
LOCAL wchar *s_gallery_list_file_video_path = L"D:\\System\\Gallery\\video_filelist.txt";
LOCAL wchar *s_gallery_list_file_download_path = L"D:\\System\\Gallery\\download_filelist.txt";
LOCAL wchar *s_gallery_list_file_onedrive_path = L"D:\\System\\Gallery\\onedrive_filelist.txt";
LOCAL GALLERY_FILEARRAY_SIG_TYPE_E s_current_own_task = GALLERY_FILEARRAY_SIG_NONE;
//add end


LOCAL FILEARRAY     s_mmipicview_pic_array = PNULL; //current path all picture files array
#ifndef MMI_PICVIEW_MINI_SUPPORT
LOCAL FILEARRAY     s_mmipicview_pic_lib      = PNULL; //picture files array include sub-folder
LOCAL uint32            cur_folder_index              = 0;
LOCAL BOOLEAN       is_array_from_lib                       = FALSE;
#ifndef PIC_PREVIEW_U200_SUPPORT
LOCAL uint32            s_cur_index              = 0;
#endif
#endif

LOCAL FILEARRAY     s_mmipicview_uploading_pic_array = PNULL; 
LOCAL FILEARRAY     s_mmipicview_thumbnail_pic_array = PNULL; 


LOCAL FILEARRAY     s_mmipicview_search_pic_array = PNULL;
LOCAL FILEARRAY     s_mmipicview_backup_main_pic_array = PNULL;
LOCAL FILEARRAY     s_mmipicview_previous_pic_array = PNULL;

LOCAL MMIPICVIEW_EFFECT_TYPE_E  s_mmipicview_slide_effect = MMIPICVIEW_EFFECT_ORDER;
LOCAL MMIPICVIEW_TIME_TYPE_E    s_mmipicview_slide_time = MMIPICVIEW_TIME_3S;
LOCAL MMIPICVIEW_VIEW_TYPE_E    s_mmipicview_view_type = MMIPICVIEW_VIEW_ICONLIST;

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GetDefaultPath(
    MMIPICVIEW_DEFAULT_PATH_T* path_ptr
);
LOCAL FILEARRAY MMIAPIPICVIEW_AllLocalFileArrayForFile(FILEARRAY main_array,BOOLEAN isforUI,BOOLEAN isDelFile, uint16 limitsize);
LOCAL void MMIAPIPICVIEW_SaveArrayToFileLimit(
                                     MMIPICVIEW_FILEARRAY_TYPE_E filearray_type, 
                                     uint16     *file_name, 
                                     uint16     file_name_len);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init pic viewer module
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_InitModule(void)
{
    //register menu
    MMIPICVIEW_RegMenuGroup();

    //register nv
    MMIPICVIEW_RegNv();

    //register win id name
    MMIPICVIEW_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : init pic viewer nv
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_Init(void)
{
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;
    MMIPICVIEW_TIME_TYPE_E      slide_time = MMIPICVIEW_TIME_2S;
    MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIPICVIEW_EFFECT_ORDER;
    MMIPICVIEW_VIEW_TYPE_E      view_type = MMIPICVIEW_VIEW_ICONLIST;
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};

    //init slide effect
    MMINV_READ(MMINV_PICVIEW_SLIDE_EFFECT, &slide_effect, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_EFFECT, &slide_effect);
    }
    s_mmipicview_slide_effect = slide_effect;

    //init slide time
    MMINV_READ(MMINV_PICVIEW_SLIDE_TIME, &slide_time, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_TIME, &slide_time);
    }
    //zhuyongjing add for default val
    if(slide_time != MMIPICVIEW_TIME_3S){
        slide_time = MMIPICVIEW_TIME_3S;
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_TIME, &slide_time);
    }
    //add end
    s_mmipicview_slide_time = slide_time;

    //init view type
    MMINV_READ(MMINV_PICVIEW_VIEW_TYPE, &view_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_PICVIEW_VIEW_TYPE, &view_type);
    }
    //zhuyongjing add for default val
    if(view_type != MMIPICVIEW_VIEW_ICONLIST){
        view_type = MMIPICVIEW_VIEW_ICONLIST;
        MMINV_WRITE(MMINV_PICVIEW_VIEW_TYPE, &view_type);
    }
    //add end
    s_mmipicview_view_type = view_type;

    //init default path
    MMIAPIPICVIEW_GetDefaultPath(&default_path);
}

/*****************************************************************************/
//  Description : resset pic viewer factory settings
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ResetFactorySettings(void)
{
    MMIPICVIEW_TIME_TYPE_E      slide_time = MMIPICVIEW_TIME_2S;
    MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIPICVIEW_EFFECT_ORDER;
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};

    //reset slide effect
    MMIPICVIEW_SetSlideEffect(slide_effect);

    //reset slide time
    MMIPICVIEW_SetSlideTime(slide_time);

    //reset default path
    GetDefaultPath(&default_path);
    MMIAPIPICVIEW_SetDefaultPath(&default_path);
}

/*****************************************************************************/
// 	Description : get slide effect
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_EFFECT_TYPE_E MMIAPIPICVIEW_GetSlideEffect(void)
{
    return (s_mmipicview_slide_effect);
}

/*****************************************************************************/
// 	Description : get slide effect
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetSlideEffect(
    MMIPICVIEW_EFFECT_TYPE_E  slide_effect
)
{
    if (slide_effect != s_mmipicview_slide_effect)
    {
        s_mmipicview_slide_effect = slide_effect;

        //write slide effect
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_EFFECT, &slide_effect);
    }
}

/*****************************************************************************/
// 	Description : get slide time
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_TIME_TYPE_E MMIPICVIEW_GetSlideTime(void)
{
    return (s_mmipicview_slide_time);
}

/*****************************************************************************/
// 	Description : set slide time
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetSlideTime(
    MMIPICVIEW_TIME_TYPE_E  slide_time
)
{
    if ((MMIPICVIEW_TIME_2S > slide_time)
            || (MMIPICVIEW_TIME_TYPE_MAX <= slide_time)
       )
    {
        //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetSlideTime slide_time=%d, overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_200_112_2_18_2_46_18_0,(uint8*)"d", slide_time);
        return ;
    }

    if (slide_time != s_mmipicview_slide_time)
    {
        s_mmipicview_slide_time = slide_time;

        //write slide effect
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_TIME, &slide_time);
    }
}

/*****************************************************************************/
// 	Description : get view type
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_VIEW_TYPE_E MMIPICVIEW_GetViewType(void)
{
    return (s_mmipicview_view_type);
}

/*****************************************************************************/
// 	Description : set view type
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetViewType(
    MMIPICVIEW_VIEW_TYPE_E    view_type
)
{
    BOOLEAN     result = FALSE;

    if (MMIPICVIEW_VIEW_MAX <= view_type)
    {
        result = FALSE;
        //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetViewType view_type=%d, overflow!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_237_112_2_18_2_46_18_1,(uint8*)"d", view_type);
        return result;
    }

    if (view_type != s_mmipicview_view_type)
    {
        s_mmipicview_view_type = view_type;

        //write view type
        MMINV_WRITE(MMINV_PICVIEW_VIEW_TYPE, &view_type);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GetDefaultPath(
    MMIPICVIEW_DEFAULT_PATH_T* path_ptr
)
{
    MMIFILE_DEVICE_E fmm_dev =  0;
//    BOOLEAN      is_sd   = MMIAPISD_IsSDExist(&fmm_dev);
    const wchar* dev_ptr = PNULL;//is_sd ? ((MMI_DEVICE_SDCARD == fmm_dev) ? MMIFILE_DEVICE_SDCARD : MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1)) : MMIFILE_DEVICE_UDISK;
    uint16       dev_len = 0;//(uint16)(is_sd ? ((MMI_DEVICE_SDCARD == fmm_dev) ? MMIFILE_DEVICE_SDCARD_LEN : MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1)) : MMIFILE_DEVICE_UDISK_LEN);

    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW GetDefaultPath path_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_273_112_2_18_2_46_18_2,(uint8*)"");
        return ;
    }
   fmm_dev = MMIAPIFMM_GetDefaultDisk();

    dev_ptr = MMIAPIFMM_GetDevicePath(fmm_dev);
    dev_len = MMIAPIFMM_GetDevicePathLen(fmm_dev);
    path_ptr->path_len = MMIFMM_PATHNAME_LEN;

#ifdef CMCC_UI_STYLE
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PHOTOS, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PHOTOS),
                            PNULL, 0,
                            path_ptr->path, &path_ptr->path_len);
#else
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PICTURE, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
                            PNULL, 0,
                            path_ptr->path, &path_ptr->path_len);
#endif
}

/*****************************************************************************/
// 	Description : get my photos pic path
//	Global resource dependence : none
//  Author: kevin.lou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_GetMyPhotosPath(
    MMIPICVIEW_DEFAULT_PATH_T* path_ptr
)
{
    //MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    GetDefaultPath(path_ptr);
}

/*****************************************************************************/
// 	Description : get default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_GetDefaultPath(
    MMIPICVIEW_DEFAULT_PATH_T* path_ptr
)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW MMIAPIPICVIEW_GetDefaultPath path_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_323_112_2_18_2_46_18_3,(uint8*)"");
        return ;
    }

    MMINV_READ(MMINV_PICVIEW_DEFAULT_PATH, path_ptr, return_value);

    if ((MN_RETURN_SUCCESS != return_value) ||
            (!MMIAPIFMM_IsFolderExist(path_ptr->path, path_ptr->path_len)))
    {
        GetDefaultPath(path_ptr);

        MMIAPIPICVIEW_SetDefaultPath(path_ptr);
    }
}

/*****************************************************************************/
// 	Description : set default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_SetDefaultPath(
    const MMIPICVIEW_DEFAULT_PATH_T* path_ptr
)
{
    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW MMIAPIPICVIEW_SetDefaultPath path_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_348_112_2_18_2_46_18_4,(uint8*)"");
        return ;
    }

    MMINV_WRITE(MMINV_PICVIEW_DEFAULT_PATH, (MMIPICVIEW_DEFAULT_PATH_T *)path_ptr);
}

/*****************************************************************************/
//  Description : get all picture files number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNum(void)
{
    uint32  pic_num = 0;
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
        if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
            pic_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(s_mmipicview_pic_lib, cur_folder_index) - MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index);
        } 
    }
    else
#endif
    {
        pic_num = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_pic_array);
    }
    return (pic_num);
}

/*****************************************************************************/
//  Description : create all picture file array
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_CreateAllPicArray(void)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE !=  is_array_from_lib)
#endif
    {
        if(PNULL == s_mmipicview_pic_array)
        {
            s_mmipicview_pic_array = MMIAPIFILEARRAY_Create();
        }
    }
}

/*****************************************************************************/
//  Description : destroy all picture file array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyAllPicArray(void)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE !=  is_array_from_lib)
#endif
    {
    	//MMIAPIFMM_SearchFileStop();
        MMIAPIFILEARRAY_Destroy(&s_mmipicview_pic_array);
    }
#ifndef MMI_PICVIEW_MINI_SUPPORT
    else
    {
        is_array_from_lib = FALSE;
    }
#endif
}
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : check the array is  from lib or not
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsPicArrayFromArrayLib(void)   
{
  return   is_array_from_lib;
}

#endif
/*****************************************************************************/
//  Description : get all picture file array
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetAllPicArray(void)
{
    return s_mmipicview_pic_array;
}

/*****************************************************************************/
//  Description : get cur picture file array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetCurPicArray(void)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
        return s_mmipicview_pic_lib;
    }
    else
#endif
    {
        return s_mmipicview_pic_array;
    }
}

/*****************************************************************************/
//  Description : get the current preview  picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPICVIEW_GetFirstPos(FILEARRAY filearray)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
        return MMIAPIFILEARRAY_GetFirstFilePos(filearray, cur_folder_index);
    }
    else
#endif
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : get the current preview  picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_GetCurPicFileInfo(BOOLEAN  is_piclib,FILEARRAY_DATA_T   *file_info_ptr)
{
    BOOLEAN     result = FALSE;

#ifndef MMI_PICVIEW_MINI_SUPPORT
    if(is_piclib)
    {
       if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
           result = MMIAPIFILEARRAY_Read(s_mmipicview_pic_lib, (MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index)), file_info_ptr);
        }
    }
    else
#endif
    {
        result = MMIAPIFILEARRAY_Read(s_mmipicview_pic_array, 0, file_info_ptr);
    }

    //SCI_TRACE_LOW:"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_446_112_2_18_2_46_18_5,(uint8*)"dd", file_info_ptr->size, file_info_ptr->name_len);
    if (0 == file_info_ptr->name_len)
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : add  picture file to pic array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_AddFileToPicArray(FILEARRAY   file_array,     
                                                                    const wchar *fullpathname)
{
    SFS_FIND_DATA_T   find_data={0};
    FILEARRAY_DATA_T    f_array_data={0};
    MMIFILE_HANDLE         handle_result =   0;   
    BOOLEAN result=FALSE;
    
    if(NULL == file_array
        || fullpathname==PNULL)
    {
        return FALSE;
    }
    handle_result=MMIAPIFMM_FindFirstFile(fullpathname, MMIAPICOM_Wstrlen(fullpathname), &find_data);
    if(SFS_INVALID_HANDLE != handle_result)
    {
        MMIAPICOM_Wstrcpy(f_array_data.filename, fullpathname);
        f_array_data.name_len=MMIAPICOM_Wstrlen(fullpathname);

        f_array_data.type = FILEARRAY_TYPE_FILE;
        f_array_data.size = find_data.length;
        f_array_data.modify_date.year = find_data.modify_Date.year;
        f_array_data.modify_date.mon = find_data.modify_Date.mon;
        f_array_data.modify_date.mday = find_data.modify_Date.mday;

        f_array_data.modify_time.hour = find_data.modify_time.hour;
        f_array_data.modify_time.min = find_data.modify_time.min;
        f_array_data.modify_time.sec = find_data.modify_time.sec;
        result = MMIAPIFILEARRAY_Add(file_array, &f_array_data);
    }
    SFS_FindClose (handle_result);
    return result;
       
}




/*****************************************************************************/
//  Description : get picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetPicFileInfo(
    uint32             pic_index,      //in
    FILEARRAY_DATA_T   *file_info_ptr  //in/out
)
{
    BOOLEAN     result = FALSE;
    
    if(PNULL == file_info_ptr)
    {
        return FALSE;
    }
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
        if  (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
            result = MMIAPIFILEARRAY_Read(s_mmipicview_pic_lib, (pic_index + MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index)), file_info_ptr);
        }
    }
    else
#endif
    {
        result = MMIAPIFILEARRAY_Read(s_mmipicview_pic_array, pic_index, file_info_ptr);
    }
    //SCI_TRACE_LOW:"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_523_112_2_18_2_46_18_6,(uint8*)"dd", file_info_ptr->size, file_info_ptr->name_len);
    if (0 == file_info_ptr->name_len)
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : delete picture file info by index from array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_DeletePicFileInfo(
    uint32  pic_index   //in
)
{
    BOOLEAN     result = FALSE;
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
         if  (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
                result = MMIAPIFILEARRAY_Delete(s_mmipicview_pic_lib, (pic_index + MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index)));
        }
    }
    else
#endif
    {
        result = MMIAPIFILEARRAY_Delete(s_mmipicview_pic_array, pic_index);
    }

    return (result);
}

/*****************************************************************************/
//  Description : delete picture file info by index from array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyPicFileInfo(
    uint32                  pic_index,  //in
    const FILEARRAY_DATA_T  *data_ptr   //in
)
{
    BOOLEAN     result = FALSE;
    
    if(PNULL == data_ptr)
    {
        return FALSE;
    }
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
            if  (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index))
            {
                result = MMIAPIFILEARRAY_Modify(s_mmipicview_pic_lib, \
                                                       (pic_index + MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index)),\
                                                        (FILEARRAY_DATA_T    *)data_ptr\
                                                        );
            }
    }
    else
#endif
    {
        result = MMIAPIFILEARRAY_Modify(s_mmipicview_pic_array, pic_index, (FILEARRAY_DATA_T *)data_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : check the default path
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsDefaultPath(MMI_STRING_T string)
{
    MMIPICVIEW_DEFAULT_PATH_T  default_path = {0};

    if (0 == string.wstr_len)
    {
        return FALSE;
    }

    //get default path
    MMIAPIPICVIEW_GetDefaultPath(&default_path);

    if (default_path.path_len != string.wstr_len)
    {
        return FALSE;
    }

    if (0 != MMIAPICOM_Wstrncmp(default_path.path, string.wstr_ptr, default_path.path_len))
    {
        return FALSE;
    }

    return TRUE;
}
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : create picture files lib array include sub-folder
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_CreatePicLibArray(void)
{
    if(PNULL == s_mmipicview_pic_lib)
    {
        s_mmipicview_pic_lib = MMIAPIFILEARRAY_Create();
    }
}

/*****************************************************************************/
//  Description : destroy  picture files lib array
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyPicLibArray(void)
{
	MMIAPIFMM_SearchFileStop();
    MMIAPIFILEARRAY_Destroy(&s_mmipicview_pic_lib);
}

/*****************************************************************************/
//  Description : get picture files  array of total pic lib
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIPICVIEW_GetPicLibArray(void)
{
    return s_mmipicview_pic_lib;
}
/*****************************************************************************/
//  Description : Create picture file array from pic lib
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_InitPicArrayFromPicLib(uint32 cur_index_folder)
{
    cur_folder_index            = cur_index_folder;
#ifndef PIC_PREVIEW_U200_SUPPORT
	s_cur_index					= cur_index_folder + 1;
#endif
    is_array_from_lib          = TRUE;
    return;
}
/*****************************************************************************/
//  Description : release picture file array from pic lib
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_ReleasePicArrayFromPicLib(void)
{
    is_array_from_lib   = FALSE;
#ifdef PIC_PREVIEW_U200_SUPPORT
    return cur_folder_index;
#else
	return s_cur_index;
#endif
}
/*****************************************************************************/
//  Description : get all picture files number in pic lib
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNumInLib(void)
{
    uint32  pic_num = 0;

    pic_num = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_pic_lib);

    return (pic_num);
}
#endif
#if 0//norflashcut_pic
/*****************************************************************************/
//  Description : get picture files number in cur Folder
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNumInCurFolder(void)
{
    uint32  pic_num = 0;
    //FILEARRAY_DATA_T  data_t;

    //MMIAPIFILEARRAY_ReadFolder(s_mmipicview_pic_lib, cur_folder_index, &data_t);
    if (TRUE ==  is_array_from_lib)
    {
        if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
            pic_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(s_mmipicview_pic_lib, cur_folder_index) - MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index);
        } 
    }

    
    return (pic_num);
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get all picture files number in pic lib
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetCurFolderIndex(void)//NEWMS00145771
{
#ifdef PIC_PREVIEW_U200_SUPPORT
    return cur_folder_index;
#else
	return s_cur_index;
#endif
}
#endif
/*****************************************************************************/
// 	Description : get photo path of special udisk
//	Global resource dependence : none
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_GetFixeddiskPhotoPath(const wchar* dev_ptr, uint16 dev_len, MMIPICVIEW_DEFAULT_PATH_T* search_path)
{
    SCI_MEMSET(search_path->path, 0, (MMIFMM_PATHNAME_LEN+1) * sizeof(wchar));
    search_path->path_len = MMIFMM_PATHNAME_LEN;

#ifdef CMCC_UI_STYLE
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PHOTOS, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PHOTOS),
                            PNULL, 0,
                            search_path->path, &search_path->path_len);
#else
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PICTURE, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
                            PNULL, 0,
                            search_path->path, &search_path->path_len);
#endif
}
//zhuyongjing add for gallary UI 20150707
/*****************************************************************************/
// 	Description : get photo path of special folder
//	Global resource dependence : none
//  Author: zhuyongjing 
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_GetFixedFolderPhotoPath(const wchar* dev_ptr, uint16 dev_len, PIC_SOURCE_TYPE type,MMIPICVIEW_DEFAULT_PATH_T* search_path)
{
    SCI_MEMSET(search_path->path, 0, (MMIFMM_PATHNAME_LEN+1) * sizeof(wchar));
    search_path->path_len = MMIFMM_PATHNAME_LEN;

#ifdef MMI_SHOW_MSA
    if(type == CAMERA_ROLL_SOURCE_DONWLOADED){
        MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PIC_ONEDRIVE_DOWNLOAD, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PIC_ONEDRIVE_DOWNLOAD),
                            PNULL, 0,
                            search_path->path, &search_path->path_len);
    }else if(type == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
        MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PIC_ONEDRIVE_THUMBNAILS),
                            PNULL, 0,
                            search_path->path, &search_path->path_len);
    }else if(type == CAMERA_ROLL_SOURCE_VIDEO){
        MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_MOVIE, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE),
                            PNULL, 0,
                            search_path->path, &search_path->path_len);
    }else if(type == CAMERA_ROLL_SOURCE_CAPTURED){
        MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PIC_CAMERA_ROLL, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PIC_CAMERA_ROLL),
                            PNULL, 0,
                            search_path->path, &search_path->path_len);
    }else{
        SCI_TRACE_LOW("MMIPICVIEW_GetFixedFolderPhotoPath err type=%d",type);
    }
#endif
     if(type == CAMERA_ROLL_SOURCE_CAPTURED){
            MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                                MMIMULTIM_DIR_PIC_CAMERA, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PIC_CAMERA),
                                PNULL, 0,
                                search_path->path, &search_path->path_len);
        }else{
            SCI_TRACE_LOW("MMIPICVIEW_GetFixedFolderPhotoPath err type=%d",type);
        }
}
/*****************************************************************************/
//  Description : Search all the Photo and video in e:/video & e:/photos/camera roll
//  Global resource dependence :
//  Author:  zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIPICVIEW_SearchAllCaptured(MMI_WIN_ID_T win_id,MMIFMM_FILTER_T* file_filter, PIC_SOURCE_TYPE* path_type,MMIFILE_DEVICE_E* fmm_dev)
{
    //BOOLEAN						return_val=FALSE;
    int32						return_val=0;
    MMIPICVIEW_DEFAULT_PATH_T 	search_path = {0};
    wchar* 						dev_ptr = PNULL;
    uint16 						dev_len = 0;
    PIC_SOURCE_TYPE     *type = path_type;
    MMIFILE_DEVICE_E*			search_dev  = fmm_dev;
    
    if(PNULL == fmm_dev)
    {
        SCI_TRACE_LOW("MMIPICVIEW_SearchAllCaptured &searching_device = pnull !!");
        return return_val;
    }


    while(*search_dev < MMI_DEVICE_NUM)
    {
            dev_ptr = MMIAPIFMM_GetDevicePath(*search_dev);
            dev_len = MMIAPIFMM_GetDevicePathLen(*search_dev);
        if(*type == CAMERA_ROLL_SOURCE_CAPTURED ||*type == CAMERA_ROLL_SOURCE_VIDEO)
        {
            if(MMIAPIFMM_GetDeviceStatus(dev_ptr, dev_len)){
                MMIPICVIEW_GetFixedFolderPhotoPath(dev_ptr, dev_len, *type,&search_path);
                MMIAPIFMM_SearchFileInPathAndSort(search_path.path,
                                    search_path.path_len,
                                    file_filter,
                                    FALSE,
                                    FUNC_FIND_FILE,
                                    MMIAPIPICVIEW_GetAllPicArray(),
                                    win_id,
                                    MSG_PICVIEWER_LOAD_VIDEO_CONTINUE,
                                    FILEARRAY_SORT_TIME_CREATE_DESCEND);
                
                if(*type == CAMERA_ROLL_SOURCE_CAPTURED){
                    *type = CAMERA_ROLL_SOURCE_VIDEO;                   
                }else if(*type == CAMERA_ROLL_SOURCE_VIDEO){
                    *type = CAMERA_ROLL_SOURCE_CAPTURED;//CAMERA_ROLL_SOURCE_NONE;
                    (*search_dev)++;
                }
                return_val=1;
                break;
            }
            else{
                (*search_dev)++;
                SCI_TRACE_LOW("MMIPICVIEW_SearchAllCaptured &searching_device not exist!");
            }
        }
        else{
            (*search_dev)++;
            SCI_TRACE_LOW("MMIPICVIEW_SearchAllCaptured &searching_device type error");
        }
    }
    return return_val;
}
//add end
/*****************************************************************************/
//  Description : Search all the Photo path of all disks 
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SearchAllPhotos(MMI_WIN_ID_T win_id, MMIFMM_FILTER_T* file_filter, MMIFILE_DEVICE_E* fmm_dev)
{
	BOOLEAN						return_val=FALSE;
	MMIFILE_DEVICE_E*			search_dev  = fmm_dev;
	MMIPICVIEW_DEFAULT_PATH_T 	search_path = {0};
	wchar* 						dev_ptr = PNULL;
    uint16 						dev_len = 0;
	

	if(PNULL == fmm_dev)
	{
		SCI_TRACE_LOW("MMIPICVIEW_SearchAllPhotos &searching_device = pnull !!");
		return return_val;
	}
    for(; *search_dev < MMI_DEVICE_NUM; (*search_dev)++)
    {
    	dev_ptr = MMIAPIFMM_GetDevicePath(*search_dev);
		dev_len = MMIAPIFMM_GetDevicePathLen(*search_dev);

        if(MMIAPIFMM_GetDeviceStatus(dev_ptr, dev_len))
        {
			MMIPICVIEW_GetFixeddiskPhotoPath(dev_ptr, dev_len, &search_path);

			MMIAPIFMM_SearchFileInPathAndSort(search_path.path,
		                        search_path.path_len,
		                        file_filter,
		                        FALSE,
		                        FUNC_FIND_FILE,
		                        MMIAPIPICVIEW_GetAllPicArray(),
		                        win_id,
		                        MSG_PICVIEWER_LOAD_CONTINUE,
		                        FILEARRAY_SORT_TIME_CREATE_DESCEND);
			(*search_dev)++;
			return_val=TRUE;
			break;
        }
	}

	return return_val;
}

/*****************************************************************************/
//  Discription: append one line text toggle list item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMIPICVIEW_AppenOneLineTextToggleListItem(	
												MMI_CTRL_ID_T    ctrl_id,
												MMI_TEXT_ID_T    text_id,
 												uint16           pos
												)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
	
    item_t.item_style = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
// 	item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

// 	item_data.softkey_id[0] = TXT_NULL;
// 	item_data.softkey_id[1] = TXT_COMMON_OK;
//     item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
	
    GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Discription: append one line text radio list item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMIPICVIEW_AppenOneLineTextRadioListItem(	
													 MMI_CTRL_ID_T    ctrl_id,
													 MMI_TEXT_ID_T    text_id,
													 uint16           pos
													 )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
	
    item_t.item_style = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
	
// 	item_data.softkey_id[0] = TXT_NULL;
// 	item_data.softkey_id[1] = TXT_COMMON_OK;
//     item_data.softkey_id[2] = STXT_RETURN;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[0].item_data.text_id = text_id;
	
    GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Discription: append one line text radio list item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMIPICVIEW_AppenOneLineTextListItem(	
													MMI_CTRL_ID_T    ctrl_id,
													MMI_TEXT_ID_T    text_id,
													uint16           pos
													)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
	
    item_t.item_style = GUIITEM_STYLE_DEFAULT;
	// 	item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;
	
// 	item_data.softkey_id[0] = TXT_NULL;
// 	item_data.softkey_id[1] = TXT_COMMON_OK;
//     item_data.softkey_id[2] = STXT_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[0].item_data.text_id = text_id;
	
    GUILIST_AppendItem( ctrl_id, &item_t );
}

#ifdef MMI_PIC_ONE_DRIVE_SUPPORT
/*****************************************************************************/
//  Description : Append One Drive List Item 
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetOneDriveItemGray(BOOLEAN is_gray)
{
	int     i = 0;
	
	for(i=ONEDRIVE_INDEX_UPLOAD; i<ONEDRIVE_INDEX_MAX; i++)
	{
        GUILIST_SetItemGray(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, i, is_gray);
        GUILIST_SetItemInvalid(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, i, is_gray);
    }
}

/*****************************************************************************/
//  Description : Append One Drive List Item 
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_AppendOneDriveUploadOptItem (void)
{
	GUILIST_SetMaxItem(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID, 2, FALSE);
	MMIPICVIEW_AppenOneLineTextRadioListItem(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID, STR_ONEDRIVE_TITLE_AUTO_EXT01, ONEDRIVE_INDEX_AUTO_UPLOAD);
	MMIPICVIEW_AppenOneLineTextRadioListItem(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID, STR_ONEDRIVE_TITLE_MANUAL_EXT01, ONEDRIVE_INDEX_MANUAL_UPLOAD);
#if defined(CAMERAROLL_SUPPORT)	
	GUILIST_SetCurItemIndex(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID, MMISRV_CAMERAROLL_Get_Sync_State());
	GUILIST_SetItemStateById(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID , MMISRV_CAMERAROLL_Get_Sync_State() , GUIITEM_STATE_SELECTED , TRUE );
#endif
	return;
}

/*****************************************************************************/
//  Description : Append One Drive List Item 
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_AppendOneDriveMenuItem ( BOOLEAN	is_sync_on)
{
// 	uint16	ver_space = 0;
//  uint16	hor_space = 0;

// 	GUIFORM_SetType(MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID,GUIFORM_TYPE_TP);
// 	GUIFORM_SetMargin(MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID, 0);
// 	GUIFORM_SetSpace(MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID,&ver_space,&hor_space);
// 	GUIFORM_PermitChildBg(MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID, FALSE);
// 	GUIFORM_PermitChildBorder(MMIPICVIEW_ONEDRIVE_LIST_FORM_CTRL_ID, FALSE);

	GUILIST_SetMaxItem(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, 3, FALSE);
	MMIPICVIEW_AppenOneLineTextToggleListItem(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, STR_ONEDRIVE_ILIST_CONNECT_EXT01, ONEDRIVE_INDEX_CONNECT);
	MMIPICVIEW_AppenOneLineTextListItem(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, STR_ONEDRIVE_HDR_UPLOAD_EXT01, ONEDRIVE_INDEX_UPLOAD);
//	MMIPICVIEW_AppenOneLineTextListItem(MMIPICVIEW_ONEDRIVE_MENU_CTRL_ID, TXT_ONEDRIVE_SYNC, ONEDRIVE_INDEX_SYNC);
	
// 	GUILIST_SetMaxItem(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID, 2, FALSE);
// 	MMIPICVIEW_AppenOneLineTextRadioListItem(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID, TXT_ONEDRIVE_UPLOAD_OPTIONS, ONEDRIVE_INDEX_UPLOAD);
// 	MMIPICVIEW_AppenOneLineTextRadioListItem(MMIPICVIEW_ONEDRIVE_LIST_UPLOAD_CTRL_ID, TXT_ONEDRIVE_SYNC, ONEDRIVE_INDEX_SYNC);

	if (is_sync_on == TRUE)
	{
		MMIPICVIEW_SetOneDriveItemGray(FALSE);
	}
	else
	{
 		MMIPICVIEW_SetOneDriveItemGray(TRUE);
	}

	return;
}
#endif
//zhuyongjing add for mutil uploading file array 
/*****************************************************************************/
//  Description : get all picture files number-in uploading array
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllUploadingPicNum(void)
{
    uint32  pic_num = 0;

    pic_num = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_uploading_pic_array);

    return (pic_num);
}

/*****************************************************************************/
//  Description : create all Uploading picture file array
//  Global resource dependence :
//  Author: zhuyongjing 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_CreateAllUploadingPicArray(void)
{
    if(PNULL == s_mmipicview_uploading_pic_array)
    {
        s_mmipicview_uploading_pic_array = MMIAPIFILEARRAY_Create();
    }
}

/*****************************************************************************/
//  Description : destroy allUploading picture file array
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyAllUploadingPicArray(void)
{
    uint32 size =0;
    uint32 i =0;
    //MMIAPIFMM_SearchFileStop();
    size = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_uploading_pic_array);
    MMIAPIFILEARRAY_Destroy(&s_mmipicview_uploading_pic_array);
}

/*****************************************************************************/
//  Description : get all Uploading picture file array
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIPICVIEW_GetAllUploadingPicArray(void)
{
    return s_mmipicview_uploading_pic_array;
}

/*****************************************************************************/
//  Description : get the current preview  picture file info by index from Uploading array
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetCurUploadingPicFileInfo(FILEARRAY_DATA_T   *file_info_ptr)
{
    BOOLEAN     result = FALSE;

    result = MMIAPIFILEARRAY_Read(s_mmipicview_uploading_pic_array, 0, file_info_ptr);
    if (0 == file_info_ptr->name_len)
    {
        result = FALSE;
    }

    return (result);
}



/*****************************************************************************/
//  Description : delete picture file info  from array
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_DeleteCurUploadingPicFileInfo(void)

{
    BOOLEAN     result = FALSE;
    uint8 i =0;
    uint8 size =0;
    
    result = MMIAPIFILEARRAY_Delete(s_mmipicview_uploading_pic_array, 0);
    size = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_uploading_pic_array);
    
    return (result);
}

/*****************************************************************************/
//  Description : delete picture file info by index from array
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_DeleteUploadingPicFileInfoByIndex(uint32 real_index)

{
    BOOLEAN     result = FALSE;
    uint8 size =0;

    SCI_TRACE_LOW("MMIPICVIEW_DeleteUploadingPicFileInfoByIndex  real_index=%d",real_index);
  
    if(real_index == MMI_PICVIEW_UPLOADING_INVALID_INDEX)
        return result;
    result = MMIAPIFILEARRAY_Delete(s_mmipicview_uploading_pic_array, real_index);
    size = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_uploading_pic_array);
    return (result);
}
/*****************************************************************************/
//  Description : add  picture file to uploading pic array
//  Global resource dependence :
//  Author: zhuyongjing
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_AddFileToUploadingPicArray(FILEARRAY   file_array,     
                                                                    const wchar *fullpathname)
{
    SFS_FIND_DATA_T   find_data={0};
    FILEARRAY_DATA_T    f_array_data={0};
    MMIFILE_HANDLE         handle_result =   0;   
    BOOLEAN result=FALSE;
     uint8 i = 0;
    
    if(NULL == file_array
        || fullpathname==PNULL)
    {
        return FALSE;
    }
 
    handle_result=MMIAPIFMM_FindFirstFile(fullpathname, MMIAPICOM_Wstrlen(fullpathname), &find_data);
    if(SFS_INVALID_HANDLE != handle_result)
    {
        MMIAPICOM_Wstrcpy(f_array_data.filename, fullpathname);
        f_array_data.name_len=MMIAPICOM_Wstrlen(fullpathname);

        f_array_data.type = FILEARRAY_TYPE_FILE;
        f_array_data.size = find_data.length;
        f_array_data.modify_date.year = find_data.modify_Date.year;
        f_array_data.modify_date.mon = find_data.modify_Date.mon;
        f_array_data.modify_date.mday = find_data.modify_Date.mday;

        f_array_data.modify_time.hour = find_data.modify_time.hour;
        f_array_data.modify_time.min = find_data.modify_time.min;
        f_array_data.modify_time.sec = find_data.modify_time.sec;
        result = MMIAPIFILEARRAY_Add(file_array, &f_array_data);
    }
    SFS_FindClose (handle_result);
    return result;
       
}
PUBLIC BOOLEAN MMIPICVIEW_IsFileExistUploadingPicArray(const wchar *fullpathname,uint16 namelen,uint32 *index_ptr)
{
    BOOLEAN result=FALSE;
    uint32              photo_index = 0;
    uint32              pic_num =MMIAPIFILEARRAY_GetArraySize(MMIPICVIEW_GetAllUploadingPicArray());
    FILEARRAY_DATA_T    file_info = {0};

    if (PNULL == fullpathname)
    {
        SCI_TRACE_LOW("MMIPICVIEW_IsFileExistUploadingPicArray fullpathname=null!");
        return result;
    }

    for (photo_index = 0; photo_index < pic_num; photo_index ++)
    {
        if(!MMIAPIFILEARRAY_Read(MMIPICVIEW_GetAllUploadingPicArray(),photo_index, &file_info))
        {
            SCI_TRACE_LOW("MMIPICVIEW_IsFileExistUploadingPicArray READ ERROR!");
            break;
        }
        if(0 == MMIAPICOM_CompareTwoWstr(file_info.filename, file_info.name_len, fullpathname, namelen))
        {
            result = TRUE;
            *index_ptr = photo_index;
            break;
        }
    }
    if(!result){
        *index_ptr = MMI_PICVIEW_UPLOADING_INVALID_INDEX;
    }
    SCI_TRACE_LOW("MMIPICVIEW_IsFileExistUploadingPicArray result=%d",result);
    return result;
       
}

PUBLIC uint32 MMIPICVIEW_GetIndexInFileArray(const wchar *fullpathname,FILEARRAY filearray)
{
    uint32  photo_index = MMI_PICVIEW_UPLOADING_INVALID_INDEX;
    uint32  index = 0;
    uint32  pic_num =MMIAPIFILEARRAY_GetArraySize(filearray);
    FILEARRAY_DATA_T    file_info = {0};
    if (PNULL == fullpathname)
    {
        SCI_TRACE_LOW("MMIPICVIEW_GetIndexInFileArray fullpathname=null!");
        return photo_index;
    }
    for (index = 0; index < pic_num; index ++)
    {
        if(!MMIAPIFILEARRAY_Read(filearray,index, &file_info))
        {
            SCI_TRACE_LOW("MMIPICVIEW_GetIndexInFileArray READ ERROR!");
            photo_index = MMI_PICVIEW_UPLOADING_INVALID_INDEX;
            break;
        }
        if(0 == MMIAPICOM_CompareTwoWstr(file_info.filename, file_info.name_len, fullpathname, MMIAPICOM_Wstrlen(fullpathname)))
        {
            photo_index = index;
            break;
        }
    }
    SCI_TRACE_LOW("MMIPICVIEW_GetIndexInFileArray photo_index=%d",photo_index);
    return photo_index;
}
/*****************************************************************************/
//  Description : get all thumbnail file array
//  Global resource dependence :
//  Author: zhuyongjing 
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetThumbnailArray(void)
{
    if(PNULL == s_mmipicview_thumbnail_pic_array)
    {
        s_mmipicview_thumbnail_pic_array = MMIAPIFILEARRAY_Create();
    }
    return s_mmipicview_thumbnail_pic_array;
}
PUBLIC BOOLEAN MMIAPIPICVIEW_ThumbnailArrayIsNull(void)
{
    BOOLEAN res = FALSE;
    if(PNULL == s_mmipicview_thumbnail_pic_array)
    {
        res = TRUE;
    }
    return res;
}
PUBLIC BOOLEAN MMIPICVIEW_GetThumbnailFileInfo(
    uint32             pic_index,      //in
    FILEARRAY_DATA_T   *file_info_ptr  //in/out
)
{
    BOOLEAN     result = FALSE;
    if(PNULL == file_info_ptr)
    {
        return FALSE;
    }
    result = MMIAPIFILEARRAY_Read(s_mmipicview_thumbnail_pic_array, pic_index, file_info_ptr);
    if (0 == file_info_ptr->name_len)
    {
        result = FALSE;
         SCI_TRACE_LOW("MMIPICVIEW_GetThumbnailFileInfo--ERR");
    }
    return (result);
}
PUBLIC void MMIPICVIEW_SearchThumbnails(MMI_WIN_ID_T win_id)
{
    wchar                       *path_ptr = PNULL;
    uint16                      path_len = 0;
    MMIFMM_FILTER_T             file_filter = {0};
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
    const wchar* dev_path = PNULL;
    int dev_len = MMIAPIFMM_GetDevicePathLen(MMISet_GetPreferDisk());
    PIC_SOURCE_TYPE type =CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY;


  MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(FALSE);
    
    if(PNULL == s_mmipicview_thumbnail_pic_array)
    {
        s_mmipicview_thumbnail_pic_array = MMIAPIFILEARRAY_Create();
    }
    dev_path = MMIAPIFMM_GetDevicePath(MMISet_GetPreferDisk());

    if(MMIAPIFMM_GetDeviceStatus(dev_path, dev_len))
    {
        MMIPICVIEW_GetFixedFolderPhotoPath(dev_path, dev_len, type,&default_path);
        path_ptr = default_path.path;
        path_len = default_path.path_len;
    }else{
        SCI_TRACE_LOW("MMIPICVIEW error sd --onedrive not exist!");
    }
    GetAllPicSuffix(&file_filter);
    win_id = 0;//backgroud win_id=0
     MMIAPIFMM_SearchFileInPathAndSort(path_ptr,
                            path_len,
                            &file_filter,
                            FALSE,
                            FUNC_FIND_FILE,
                            MMIAPIPICVIEW_GetThumbnailArray(),
                            win_id,
                            MSG_PICVIEWER_LOAD_FINISH,
                            FILEARRAY_SORT_TIME_MODIFY_ASCEND);
#if defined(CAMERAROLL_SUPPORT)
    if(!(MMISRV_CAMERAROLL_Get_Connect_State() 
    && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)))
#endif
    {
        MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(TRUE);
    }
}
PUBLIC uint32 MMIPICVIEW_GetAllThumbnailsNum(void)
{
    uint32  pic_num = 0;
    pic_num = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_thumbnail_pic_array);
    return (pic_num);
}
PUBLIC void MMIPICVIEW_DestroyThumbnailsArray(void)
{
    MMIAPIFILEARRAY_Destroy(&s_mmipicview_thumbnail_pic_array);
}

//for main array backup when go to all local/capture/download/video/onedrive
PUBLIC void MMIAPIPICVIEW_DestroyBackupAllPicArray(void)
{
    MMIAPIFILEARRAY_Destroy(&s_mmipicview_backup_main_pic_array);

}
PUBLIC FILEARRAY MMIAPIPICVIEW_GetBackupAllPicArray(void)
{
    if(s_mmipicview_backup_main_pic_array == PNULL){
        s_mmipicview_backup_main_pic_array = MMIAPIFILEARRAY_Create();
    }
    return s_mmipicview_backup_main_pic_array;
}

//for main array really search
PUBLIC FILEARRAY MMIAPIPICVIEW_GetSearchAllPicArray(void)
{
    if(s_mmipicview_search_pic_array == PNULL){
        s_mmipicview_search_pic_array = MMIAPIFILEARRAY_Create();
    }
    return s_mmipicview_search_pic_array;
}
//for reload filearray 
PUBLIC void MMIAPIPICVIEW_DestroyPreviousPicArray(void)
{
    MMIAPIFILEARRAY_Destroy(&s_mmipicview_previous_pic_array);

}
PUBLIC FILEARRAY MMIPICVIEW_GetPreviousPicArray(void)
{
    if(s_mmipicview_previous_pic_array == PNULL){
        s_mmipicview_previous_pic_array = MMIAPIFILEARRAY_Create();
    }
    return s_mmipicview_previous_pic_array;
}
//
PUBLIC void MMIAPIPICVIEW_DestroySearchAllPicArray(void)
{
    	//MMIAPIFMM_SearchFileStop();
        MMIAPIFILEARRAY_Destroy(&s_mmipicview_search_pic_array);
}
PUBLIC void MMIPICVIEW_SearchMainViewFile(MMI_WIN_ID_T win_id)
{
    MMIFMM_FILTER_T             file_filter = {0};
    
#if defined(CAMERAROLL_SUPPORT)
    if(MMISRV_CAMERAROLL_Get_Connect_State()
#ifndef WIN32
        && MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL)
#endif
    ){
    MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(FALSE);
    }else
#endif    
    {
        MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(TRUE);
    }

    GetAllPicAndVideoSuffix(&file_filter);
    MMIAPIPICVIEW_DestroySearchAllPicArray();
    
#if 1//modify by qin.he for only search specially dir not all dir
    {
        MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
        wchar      **abs_path_array = NULL;
        uint8       i = 0, j = 0;
        wchar   *mmimultim_picture_dir_array[MMIMULTIM_PIC_DIR_NUM] = {MMIMULTIM_DIR_PICTURE, MMIMULTIM_DIR_REVFILE, NULL};
        uint16      path_array_len = 0;

        abs_path_array = SCI_MallocApp(4*MMI_DEVICE_NUM*MMIMULTIM_PIC_DIR_NUM, _D_FILE_NAME, _D_FILE_LINE);
        if (abs_path_array == NULL)
        {
            SCI_TRACE_LOW("[qin.he %s:%d] malloc fail \n",__FUNCTION__,__LINE__);
            MMK_PostMsg(win_id,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
            return;  
        }

        SCI_MEMSET(abs_path_array,0x00,4*MMI_DEVICE_NUM*MMIMULTIM_PIC_DIR_NUM);
        SCI_TRACE_LOW("[qin.he %s:%d] alloc memory path_array %p \n",__FUNCTION__,__LINE__,abs_path_array);

        for (device = MMI_DEVICE_UDISK, j= 0; device < MMI_DEVICE_NUM; device++,j++)
        {
            if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
            {
                for (i=0; i< MMIMULTIM_PIC_DIR_NUM && abs_path_array != NULL; i++)
                {
                    abs_path_array[path_array_len] = SCI_MallocApp((MMIFMM_FILE_FILE_NAME_MAX_LEN+3)*2,_D_FILE_NAME,_D_FILE_LINE);
                    if (abs_path_array[path_array_len])
                    {
                        SCI_TRACE_LOW("[qin.he %s:%d] alloc memory path_array %p \n",__FUNCTION__,__LINE__,abs_path_array[path_array_len]);
                        SCI_MEMSET(abs_path_array[path_array_len],0x00,(MMIFMM_FILE_FILE_NAME_MAX_LEN+3)*2);
                        if (mmimultim_picture_dir_array[i])
                        {
                            abs_path_array[path_array_len][0] = TRUE;
                            abs_path_array[path_array_len][1] = MMIFMM_FILE_FILE_NAME_MAX_LEN;
                            if (MMIAPIFMM_CombineFullPath(
                                MMIAPIFMM_GetDevicePath(device), 
                                MMIAPIFMM_GetDevicePathLen(device),
                                mmimultim_picture_dir_array[i], 
                                MMIAPICOM_Wstrlen(mmimultim_picture_dir_array[i]),
                                PNULL, 
                                PNULL,
                                &(abs_path_array[path_array_len][2]),
                                &(abs_path_array[path_array_len][1])
                                ))
                            {
                                uint8 pass_word[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
                                MMIAPICOM_WstrToStr(&(abs_path_array[path_array_len][2]),  (uint8 *)pass_word);
                                SCI_TRACE_LOW("[qin.he %s:%d]index:%d,  path len: %d, path: %s\n",__FUNCTION__,__LINE__, path_array_len, abs_path_array[path_array_len][1], pass_word);
                                path_array_len++;
                            }
                            else
                            {
                                SCI_TRACE_LOW("[qin.he %s:%d] free path_array , path_array_len: %d, abs_path_array[path_array_len]: %p\n",__FUNCTION__,__LINE__,path_array_len, abs_path_array[path_array_len]);
                                SCI_FREE (abs_path_array[path_array_len]);
                                abs_path_array[path_array_len] = NULL;
                            }
                        }
                        else
                        {
                            uint8 pass_word[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
                            abs_path_array[path_array_len][0] = FALSE;
                            abs_path_array[path_array_len][1] = MMIAPIFMM_GetDevicePathLen(device);
                            MMI_WSTRNCPY (&(abs_path_array[path_array_len][2]), MMIFMM_FILE_FILE_NAME_MAX_LEN, MMIAPIFMM_GetDevicePath(device), abs_path_array[path_array_len][1], abs_path_array[path_array_len][1]);
                            abs_path_array[path_array_len][2+(abs_path_array[path_array_len][1])] = MMIFILE_COLON;
                            abs_path_array[path_array_len][1] += 1;
                            MMIAPICOM_WstrToStr(&(abs_path_array[path_array_len][2]),  (uint8 *)pass_word);
                            SCI_TRACE_LOW("[qin.he %s:%d]index:%d,   path len: %d, path: %s\n",__FUNCTION__,__LINE__, path_array_len, abs_path_array[path_array_len][1], pass_word);
                            path_array_len++;
                        }
                    }
                }
            } 
        }

        if (path_array_len ==  0)
        {
            SCI_TRACE_LOW("[qin.he %s:%d] free path_array , path_array_len: %d, abs_path_array: %p\n",__FUNCTION__,__LINE__,path_array_len, abs_path_array);
            SCI_FREE (abs_path_array);
            MMK_PostMsg(win_id,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
            return;  
        }
        else
        {
            MMIAPIFMM_SearchFileInPathsAndSort(
                                abs_path_array, 
                                path_array_len,
                                &file_filter,
                                TRUE,
                                FUNC_FIND_FILE,
                                MMIAPIPICVIEW_GetSearchAllPicArray(),
                                win_id,
                                MSG_PICVIEWER_LOAD_FINISH,
                                FILEARRAY_SORT_NULL/*FILEARRAY_SORT_TIME_CREATE_DESCEND*/);
        }
    }
#else
    MMIAPIFMM_SearchFileInPathAndSort(PNULL,
                            0,
                            &file_filter,
                            TRUE,
                            FUNC_FIND_FILE,
                            MMIAPIPICVIEW_GetSearchAllPicArray(),
                            win_id,
                            MSG_PICVIEWER_LOAD_FINISH,
                            FILEARRAY_SORT_NULL/*FILEARRAY_SORT_TIME_CREATE_DESCEND*/);
#endif
}

PUBLIC void MMIPICVIEW_SearchMainViewPicFile(MMI_WIN_ID_T win_id)
{
    MMIFMM_FILTER_T             file_filter = {0};

    MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(TRUE);
    MMIAPIPICVIEW_DestroySearchAllPicArray();
    GetAllPicSuffix(&file_filter);

#if 1//modify by qin.he for only search specially dir not all dir
    {
        MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
        wchar      **abs_path_array = NULL;
        uint8       i = 0, j = 0;
        wchar   *mmimultim_picture_dir_array[MMIMULTIM_PIC_DIR_NUM] = {MMIMULTIM_DIR_PICTURE, MMIMULTIM_DIR_REVFILE, NULL};
        uint16      path_array_len = 0;
        
        abs_path_array = SCI_MallocApp(4*MMI_DEVICE_NUM*MMIMULTIM_PIC_DIR_NUM, _D_FILE_NAME, _D_FILE_LINE);
        if (abs_path_array == NULL)
        {
            SCI_TRACE_LOW("[qin.he %s:%d] malloc fail \n",__FUNCTION__,__LINE__);
            MMK_PostMsg(win_id,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
            return;  
        }

        SCI_MEMSET(abs_path_array,0x00,4*MMI_DEVICE_NUM*MMIMULTIM_PIC_DIR_NUM);
        SCI_TRACE_LOW("[qin.he %s:%d] alloc memory path_array %p, filter_str: %s \n",__FUNCTION__,__LINE__,abs_path_array,file_filter.filter_str);
        
        for (device = MMI_DEVICE_UDISK, j= 0; device < MMI_DEVICE_NUM; device++,j++)
        {
            if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
            {
                for (i=0; i< MMIMULTIM_PIC_DIR_NUM && abs_path_array != NULL; i++)
                {
                    abs_path_array[path_array_len] = SCI_MallocApp((MMIFMM_FILE_FILE_NAME_MAX_LEN+3)*2,_D_FILE_NAME,_D_FILE_LINE);
                    if (abs_path_array[path_array_len])
                    {
                        SCI_TRACE_LOW("[qin.he %s:%d] alloc memory path_array %p \n",__FUNCTION__,__LINE__,abs_path_array[path_array_len]);
                        SCI_MEMSET(abs_path_array[path_array_len],0x00,(MMIFMM_FILE_FILE_NAME_MAX_LEN+3)*2);
                        if (mmimultim_picture_dir_array[i])
                        {
                            abs_path_array[path_array_len][0] = TRUE;
                            abs_path_array[path_array_len][1] = MMIFMM_FILE_FILE_NAME_MAX_LEN;
                            if (MMIAPIFMM_CombineFullPath(
                                MMIAPIFMM_GetDevicePath(device), 
                                MMIAPIFMM_GetDevicePathLen(device),
                                mmimultim_picture_dir_array[i], 
                                MMIAPICOM_Wstrlen(mmimultim_picture_dir_array[i]),
                                PNULL, 
                                PNULL,
                                &(abs_path_array[path_array_len][2]),
                                &(abs_path_array[path_array_len][1])
                                ))
                            {
                                uint8 pass_word[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
                                MMIAPICOM_WstrToStr(&(abs_path_array[path_array_len][2]),  (uint8 *)pass_word);
                                SCI_TRACE_LOW("[qin.he %s:%d]index:%d,  path len: %d, path: %s\n",__FUNCTION__,__LINE__, path_array_len, abs_path_array[path_array_len][1], pass_word);
                                path_array_len++;
                            }
                            else
                            {
                                SCI_TRACE_LOW("[qin.he %s:%d] free path_array , path_array_len: %d, abs_path_array[path_array_len]: %p\n",__FUNCTION__,__LINE__,path_array_len, abs_path_array[path_array_len]);
                                SCI_FREE (abs_path_array[path_array_len]);
                                abs_path_array[path_array_len] = NULL;
                            }
                        }
                        else
                        {
                            uint8 pass_word[MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
                            abs_path_array[path_array_len][0] = FALSE;
                            abs_path_array[path_array_len][1] = MMIAPIFMM_GetDevicePathLen(device);
                            MMI_WSTRNCPY (&(abs_path_array[path_array_len][2]), MMIFMM_FILE_FILE_NAME_MAX_LEN, MMIAPIFMM_GetDevicePath(device), abs_path_array[path_array_len][1], abs_path_array[path_array_len][1]);
                            abs_path_array[path_array_len][2+(abs_path_array[path_array_len][1])] = MMIFILE_COLON;
                            abs_path_array[path_array_len][1] += 1;
                            MMIAPICOM_WstrToStr(&(abs_path_array[path_array_len][2]),  (uint8 *)pass_word);
                            SCI_TRACE_LOW("[qin.he %s:%d]index:%d,   path len: %d, path: %s\n",__FUNCTION__,__LINE__, path_array_len, abs_path_array[path_array_len][1], pass_word);
                            path_array_len++;
                        }
                    }
                }
            } 
        }

        if (path_array_len ==  0)
        {
            SCI_TRACE_LOW("[qin.he %s:%d] free path_array , path_array_len: %d, abs_path_array: %p\n",__FUNCTION__,__LINE__,path_array_len, abs_path_array);
            SCI_FREE (abs_path_array);
            MMK_PostMsg(win_id,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
        }
        else
        {
            MMIAPIFMM_SearchFileInPathsAndSort(
                                abs_path_array, 
                                path_array_len,
                                &file_filter,
                                TRUE,
                                FUNC_FIND_FILE,
                                MMIAPIPICVIEW_GetSearchAllPicArray(),
                                win_id,
                                MSG_PICVIEWER_LOAD_FINISH,
                                FILEARRAY_SORT_NULL/*FILEARRAY_SORT_TIME_CREATE_DESCEND*/);
        }
    }
#else
    MMIAPIFMM_SearchFileInPathAndSort(PNULL,
                       0,
                       &file_filter,
                       TRUE,
                       FUNC_FIND_FILE,
                       MMIAPIPICVIEW_GetSearchAllPicArray(),
                       win_id,
                       MSG_PICVIEWER_LOAD_FINISH,
                       FILEARRAY_SORT_NULL/*FILEARRAY_SORT_TIME_CREATE_DESCEND*/);
#endif
}
PUBLIC void MMIPICVIEW_SearchMainViewPicWithoutGifFile(MMI_WIN_ID_T win_id)
{
    MMIFMM_FILTER_T             file_filter = {0};
    MMIAPIPICVIEW_SetThumbnailsFolderAttrHide(TRUE);
    MMIAPIPICVIEW_DestroySearchAllPicArray();
    GetAllPicWithoutGifSuffix(&file_filter);
     MMIAPIFMM_SearchFileInPathAndSort(PNULL,
                            0,
                            &file_filter,
                            TRUE,
                            FUNC_FIND_FILE,
                            MMIAPIPICVIEW_GetSearchAllPicArray(),
                            win_id,
                            MSG_PICVIEWER_LOAD_FINISH,
                            FILEARRAY_SORT_NULL/*FILEARRAY_SORT_TIME_CREATE_DESCEND*/);
}
PUBLIC void MMIPICVIEW_SearchMainViewVideoFile(MMI_WIN_ID_T win_id)
{

    MMIFMM_FILTER_T             file_filter = {0};

    GetAllVideoSuffix(&file_filter);
    MMIAPIPICVIEW_DestroySearchAllPicArray();
     MMIAPIFMM_SearchFileInPathAndSort(PNULL,
                            0,
                            &file_filter,
                            TRUE,
                            FUNC_FIND_FILE,
                            MMIAPIPICVIEW_GetSearchAllPicArray(),
                            win_id,
                            MSG_PICVIEWER_LOAD_FINISH,
                            FILEARRAY_SORT_NULL/*FILEARRAY_SORT_TIME_CREATE_DESCEND*/);
     
}
LOCAL void MMIPICVIEW_FilterToGetExistFile(FILEARRAY  file_array)
{
    uint32 pic_num = MMIAPIFILEARRAY_GetArraySize(file_array);
    uint16 index =0;
    FILEARRAY_DATA_T file_info={0};
    
    while(index < pic_num){
        if(MMIPICVIEW_GetPicFileInfo(index,&file_info)){
            if(!MMIAPIFMM_IsFileExist(file_info.filename,file_info.name_len)){
                MMIAPIFILEARRAY_Delete(file_array, index);
                pic_num--;
                continue;
            }
            if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_WALLPAPER){              
                if(ONEDRIVE_THUMBNAIL == GetFileSource(file_info.filename,file_info.name_len)
                    ||MMIPICVIEW_IsGifSuffix(&file_info)||MMIPICVIEW_IsVideoSuffix(&file_info)){
                MMIAPIFILEARRAY_Delete(file_array, index);
                pic_num--;
                    continue;
                }        
            }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS
                ||MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_PB){
                if(ONEDRIVE_THUMBNAIL == GetFileSource(file_info.filename,file_info.name_len)
                    ||MMIPICVIEW_IsVideoSuffix(&file_info)){
                    MMIAPIFILEARRAY_Delete(file_array, index);
                    pic_num--;
                    continue;
                }
            }else if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS){
                if(!MMIPICVIEW_IsVideoSuffix(&file_info)){
                    MMIAPIFILEARRAY_Delete(file_array, index);
                    pic_num--;
                    continue;
                }
            }
            index++;
        }else{
            break;
         }
    }    
}
PUBLIC BOOLEAN MMIPICVIEW_IfGetFileArrayFromListFile(PIC_SOURCE_TYPE type)
{
    MMIFILE_HANDLE file_handle = 0;
    BOOLEAN result = FALSE;
    
     SCI_TRACE_LOW("MMIPICVIEW MMIPICVIEW_IfGetFileArrayFromListFile type=0x%x", type);
     if (!MMIAPIFMM_IsFolderExist (s_gallery_list_file_folder, MMIAPICOM_Wstrlen(s_gallery_list_file_folder))){
            MMIAPIFMM_CreateDir (s_gallery_list_file_folder,  MMIAPICOM_Wstrlen(s_gallery_list_file_folder));
        }
     if(type == CAMERA_ROLL_SOURCE_NONE){
        if(PNULL == s_mmipicview_pic_array){
            s_mmipicview_pic_array = MMIAPIFILEARRAY_Create();
            if(MMIAPIFMM_IsFileExist(s_gallery_list_file_main_path , MMIAPICOM_Wstrlen(s_gallery_list_file_main_path) ))
            {
                MMIAPIFILEARRAY_LoadArrayFromFile(s_mmipicview_pic_array, s_gallery_list_file_main_path ,MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
                //MMIPICVIEW_FilterToGetExistFile(s_mmipicview_pic_array);
                //result = TRUE;
                if(0 < MMIPICVIEW_GetAllPicNum())
                {
                    result = TRUE;
                }
                else
                {
                    SCI_TRACE_LOW("MMIPICVIEW MMIPICVIEW_IfGetFileArrayFromListFile PicNum is 0");
                    result = FALSE;
                }
            }	
            else
            {
                file_handle = MMIAPIFMM_CreateFile(s_gallery_list_file_main_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
                MMIAPIFMM_CloseFile(file_handle);
                result = FALSE;
            }
        }
        else{
            if(MMIAPIFMM_IsFileExist(s_gallery_list_file_main_path , MMIAPICOM_Wstrlen(s_gallery_list_file_main_path) ))
            {
                if (0 == MMIPICVIEW_GetAllPicNum())
                {
                    MMIAPIFILEARRAY_LoadArrayFromFile(s_mmipicview_pic_array, s_gallery_list_file_main_path ,MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
                }
                else
                {
                    MMIVIRTUALARRAY_ReadFileToBuffer(s_mmipicview_pic_array);
                }
                
                //MMIPICVIEW_FilterToGetExistFile(s_mmipicview_pic_array);
                //result = TRUE;
                if(0 < MMIPICVIEW_GetAllPicNum())
                {
                    result = TRUE;
                }
                else
                {
                    SCI_TRACE_LOW("MMIPICVIEW MMIPICVIEW_IfGetFileArrayFromListFile PicNum is 00");
                    result = FALSE;
                }
            }	
            else
            {
                file_handle = MMIAPIFMM_CreateFile(s_gallery_list_file_main_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
                MMIAPIFMM_CloseFile(file_handle);
                result = FALSE;
            }
        }
    }
     else{
        result = TRUE;
     }
     /*else if(type == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){

    }else if(type == CAMERA_ROLL_SOURCE_ALL_LOCAL){
        if(PNULL == s_mmipicview_alllocal_pic_array){
            s_mmipicview_alllocal_pic_array = MMIAPIFILEARRAY_Create();
            if(MMIAPIFMM_IsFileExist(s_gallery_list_file_local_path , MMIAPICOM_Wstrlen(s_gallery_list_file_local_path) ))
            {
                MMIAPIFILEARRAY_LoadArrayFromFile(s_mmipicview_alllocal_pic_array, s_gallery_list_file_local_path ,MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
                result = TRUE;
            }	
            else
            {
                file_handle = MMIAPIFMM_CreateFile(s_gallery_list_file_local_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
                MMIAPIFMM_CloseFile(file_handle);
                result = FALSE;
            }
        }else{
            result = TRUE;
        }
    }else if(type == CAMERA_ROLL_SOURCE_CAPTURED){
        if(PNULL == s_mmipicview_capture_pic_array){
            s_mmipicview_capture_pic_array = MMIAPIFILEARRAY_Create();
            if(MMIAPIFMM_IsFileExist(s_gallery_list_file_capture_path , MMIAPICOM_Wstrlen(s_gallery_list_file_capture_path) ))
            {
                MMIAPIFILEARRAY_LoadArrayFromFile(s_mmipicview_capture_pic_array, s_gallery_list_file_capture_path ,MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
                result = TRUE;
            }	
            else
            {
                file_handle = MMIAPIFMM_CreateFile(s_gallery_list_file_capture_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
                MMIAPIFMM_CloseFile(file_handle);
                result = FALSE;
            }
        }else{
            result = TRUE;
        }
    }else if(type == CAMERA_ROLL_SOURCE_DONWLOADED){
        if(PNULL == s_mmipicview_download_pic_array){
            s_mmipicview_download_pic_array = MMIAPIFILEARRAY_Create();
            if(MMIAPIFMM_IsFileExist(s_gallery_list_file_download_path , MMIAPICOM_Wstrlen(s_gallery_list_file_download_path) ))
            {
                MMIAPIFILEARRAY_LoadArrayFromFile(s_mmipicview_download_pic_array, s_gallery_list_file_download_path ,MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
                result = TRUE;
            }	
            else
            {
                file_handle = MMIAPIFMM_CreateFile(s_gallery_list_file_download_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
                MMIAPIFMM_CloseFile(file_handle);
                result = FALSE;
            }
        }else{
            result = TRUE;
        }
    }else if(type == CAMERA_ROLL_SOURCE_VIDEO){
        if(PNULL == s_mmipicview_video_pic_array){
            s_mmipicview_video_pic_array = MMIAPIFILEARRAY_Create();
            if(MMIAPIFMM_IsFileExist(s_gallery_list_file_video_path , MMIAPICOM_Wstrlen(s_gallery_list_file_video_path) ))
            {
                MMIAPIFILEARRAY_LoadArrayFromFile(s_mmipicview_video_pic_array, s_gallery_list_file_video_path ,MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
                result = TRUE;
            }	
            else
            {
                file_handle = MMIAPIFMM_CreateFile(s_gallery_list_file_video_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
                MMIAPIFMM_CloseFile(file_handle);
                result = FALSE;
            }
        }else{
            result = TRUE;
        }
    }*/
    
    SCI_TRACE_LOW(" MMIPICVIEW_IfGetFileArrayFromListFile result=%d",result);

    return result;
}
/*PUBLIC FILEARRAY MMIPICVIEW_GetSubFolderFileArray(PIC_SOURCE_TYPE type){
    if(type == CAMERA_ROLL_SOURCE_ALL_LOCAL){
        return s_mmipicview_alllocal_pic_array;
    }else if(type == CAMERA_ROLL_SOURCE_CAPTURED){
        return s_mmipicview_capture_pic_array;
    }else if(type == CAMERA_ROLL_SOURCE_DONWLOADED){
        return s_mmipicview_download_pic_array;
    }else if(type == CAMERA_ROLL_SOURCE_VIDEO){
        return s_mmipicview_video_pic_array;
    }
}*/

PUBLIC void MMIPICVIEW_SaveFileArrayToListFile(PIC_SOURCE_TYPE type){
  
    if(type == CAMERA_ROLL_SOURCE_NONE){
#ifdef CAMERAROLL_SUPPORT 			
        if(MMISRV_CAMERAROLL_Get_Connect_State()){
            SCI_TRACE_LOW("[mmipic]MMIPICVIEW_SaveFileArrayToListFile filter thumbnail timestart = %d", SCI_GetTickCount());
           //MMIAPIPICVIEW_AllLocalFileArrayForFile(s_mmipicview_pic_array,FALSE,FALSE,VIRTUALARRAY_SIZE_SAVE_TO_FILE);
            MMIAPIPICVIEW_SaveArrayToFileLimit(MMIPICVIEW_CURRENT_FILEARRAY, s_gallery_list_file_main_path, MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
        }else
#endif        
        {
        SCI_TRACE_LOW("[mmipic]MMIPICVIEW_SaveFileArrayToListFile save to file timestart = %d", SCI_GetTickCount());
            MMIAPIPICVIEW_SaveArrayToFileLimit(MMIPICVIEW_CURRENT_FILEARRAY, s_gallery_list_file_main_path, MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
        }
    }else{
#ifdef CAMERAROLL_SUPPORT    
        if(MMISRV_CAMERAROLL_Get_Connect_State()){
            SCI_TRACE_LOW("[mmipic]MMIPICVIEW_SaveFileArrayToListFile filter thumbnail timestart = %d", SCI_GetTickCount());
           //MMIAPIPICVIEW_AllLocalFileArrayForFile(s_mmipicview_backup_main_pic_array,FALSE,FALSE,VIRTUALARRAY_SIZE_SAVE_TO_FILE);
            MMIAPIPICVIEW_SaveArrayToFileLimit(MMIPICVIEW_BACKUP_FILEARRAY, s_gallery_list_file_main_path, MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
        }else
#endif        
        {
            MMIAPIPICVIEW_SaveArrayToFileLimit(MMIPICVIEW_BACKUP_FILEARRAY, s_gallery_list_file_main_path, MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
        }
    }
    SCI_TRACE_LOW("[mmipic] type=%d MMIPICVIEW_SaveFileArrayToListFile end time=%d ",type,SCI_GetTickCount());
}

LOCAL FILEARRAY FilterThumbnailFileArray(FILEARRAY main_array,
                                    const wchar *folder_path1,
                                    const wchar *folder_path2,
                                    const wchar *sd_folder_path1,
                                    const wchar *sd_folder_path2,
                                    BOOLEAN isforUI,
                                    BOOLEAN isDelFile,
                                    BOOLEAN keep_video,
                                    uint16 limit_size){
    FILEARRAY_DATA_T    file_info = {0};
    uint16 photo_index = 0;
    uint16 old_photo_index = 0;
    uint32 total_num= MMIAPIFILEARRAY_GetArraySize(main_array);
    FILEARRAY limit_array = PNULL;

    if(folder_path1 == PNULL){
        return PNULL;
    }
    if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_UDISK)
        &&!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD)){
        return PNULL;
    }
    while(photo_index < total_num && !MMIPICVIEW_IsClose() && !MMIAPIPICVIEW_GetTaskCancelFlag()){
        if(!MMIAPIFILEARRAY_Read(main_array,photo_index, &file_info)){
            SCI_TRACE_LOW("[mmipic] FilterThumbnailFileArray local file read err!");
            break;
        }
        if(!keep_video){
            wchar suffix_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
            uint16 suffix_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
            MMIAPIFMM_SplitFullPathExt(file_info.filename,MMIAPICOM_Wstrlen(file_info.filename),PNULL,PNULL, PNULL, PNULL, suffix_path, &suffix_path_length);
            if(MMIAPICOM_Wstrcmp(suffix_path,L".3gp")==0
                ||MMIAPICOM_Wstrcmp(suffix_path,L".mp4")==0
                ||MMIAPICOM_Wstrcmp(suffix_path,L".flv")==0
                ||MMIAPICOM_Wstrcmp(suffix_path,L".rmvb")==0
                ||MMIAPICOM_Wstrcmp(suffix_path,L".avi")==0){
                if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                    total_num--;
                    if(isforUI){
                        MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,photo_index);
                    }
                    continue;
                }else{
                    SCI_TRACE_LOW("[mmipic] FilterFileArrayByAttri delete video fail!");
                }
            }
        }
        if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD)){
            if(folder_path2 != PNULL){ 
                if(MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path1, MMIAPICOM_Wstrlen(folder_path1))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path2, MMIAPICOM_Wstrlen(folder_path2))){ 
                    if(isDelFile){
                        MMIAPIFMM_DeleteFileSyn(file_info.filename, file_info.name_len);
                    }
                    if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                        if(isforUI){
                            MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,photo_index);
                        }
                        total_num--;
                    }else{
                        SCI_TRACE_LOW("[mmipic] FilterThumbnailFileArray udisk delete fail!2tring photo_index=%d",photo_index);
                        break;
                    }
                }else{
                    photo_index++;
                }
            }else{
                if(MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path1, MMIAPICOM_Wstrlen(folder_path1))){ 
                    if(isDelFile){
                        MMIAPIFMM_DeleteFileSyn(file_info.filename, file_info.name_len);
                    }
                    if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                        if(isforUI){
                            MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,photo_index);
                        }
                        total_num--; 
                    }else{
                        SCI_TRACE_LOW("[mmipic] FilterThumbnailFileArray udisk delete fail!1tring photo_index=%d",photo_index);
                        break;
                    }
                }else{
                    photo_index++;
                }
            }
        }else{
            if(folder_path2 != PNULL){ 

                if(MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path1, MMIAPICOM_Wstrlen(folder_path1))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path2, MMIAPICOM_Wstrlen(folder_path2))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, sd_folder_path1, MMIAPICOM_Wstrlen(sd_folder_path1))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, sd_folder_path2, MMIAPICOM_Wstrlen(sd_folder_path2))){ 
                    if(isDelFile){
                        MMIAPIFMM_DeleteFileSyn(file_info.filename, file_info.name_len);
                    }
                    if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                        if(isforUI){
                            MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,photo_index);
                        }
                        total_num--;
                    }else{
                        SCI_TRACE_LOW("[mmipic] FilterThumbnailFileArray sd delete fail!2tring photo_index=%d",photo_index);
                        break;
                    }
                }else{
                    photo_index++;
                }
            }else{
                if(MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path1, MMIAPICOM_Wstrlen(folder_path1))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, sd_folder_path1, MMIAPICOM_Wstrlen(sd_folder_path1))){     
                    if(isDelFile){
                        MMIAPIFMM_DeleteFileSyn(file_info.filename, file_info.name_len);
                    }
                    if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                        if(isforUI){
                            MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,photo_index);
                        }
                        total_num--;
                    }else{
                        SCI_TRACE_LOW("[mmipic] FilterThumbnailFileArray sd delete fail!1tring photo_index=%d",photo_index);
                        break;
                    }
                }else{
                    photo_index++;
                }
            }
        }
        if(limit_size > 0&&old_photo_index!= photo_index){
            old_photo_index = photo_index;
            if(limit_array == PNULL){
                limit_array = MMIAPIFILEARRAY_Create();
            }
            MMIAPIPICVIEW_AddFileToPicArray(limit_array,file_info.filename);
            if(photo_index== limit_size){
                SCI_TRACE_LOW("[mmipic] FilterThumbnailFileArray reach limit size =%d",limit_size);
                break;
            }
        }
    }
    if(photo_index == total_num){
        SCI_TRACE_LOW("[mmipic] FilterThumbnailFileArray finish arraysize=%d",total_num);
        }

    if(limit_array != PNULL){
        MMIAPIFILEARRAY_SaveArrayToFile(limit_array, s_gallery_list_file_main_path, MMIAPICOM_Wstrlen(s_gallery_list_file_main_path));
        MMIAPIFILEARRAY_Destroy(&limit_array);
    }
    return main_array;
}

LOCAL void FilterSpecialFolderFileArray(FILEARRAY main_array,const wchar *folder_path1,const wchar *folder_path2,const wchar *sd_folder_path1,const wchar *sd_folder_path2){
    FILEARRAY_DATA_T    file_info = {0};
    uint16 photo_index = 0;
    uint32 total_num= MMIAPIFILEARRAY_GetArraySize(main_array);


    if(folder_path1 == PNULL){
        return;
    }
    if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_UDISK)
        &&!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD)){
        return;
    }
    while(photo_index < total_num && !MMIPICVIEW_IsClose() && !MMIAPIPICVIEW_GetTaskCancelFlag()){
        if(!MMIAPIFILEARRAY_Read(main_array,photo_index, &file_info)){
            SCI_TRACE_LOW("[mmipic] FilterSpecialFolderFileArray local file read err!");
            break;
        }

        if(!MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_SDCARD)){
            if(folder_path2 != PNULL){ 
                if(MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path1, MMIAPICOM_Wstrlen(folder_path1))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path2, MMIAPICOM_Wstrlen(folder_path2))){
                    photo_index++;   
                }else{
                    if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                        total_num--;
                    }else{
                        SCI_TRACE_LOW("[mmipic] FilterSpecialFolderFileArray delete fail!udisk 2string photo_index=%d",photo_index);
                        break;
                    }
                }
            }else{
                if(MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path1, MMIAPICOM_Wstrlen(folder_path1))){     
                    photo_index++;
                }else{
                    if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                        total_num--;
                    }else{
                        SCI_TRACE_LOW("[mmipic] FilterSpecialFolderFileArray delete fail!udisk 1string photo_index=%d",photo_index);
                        break;
                    }
                }
            }
        }else{
            if(folder_path2 != PNULL){ 

                if(MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path1, MMIAPICOM_Wstrlen(folder_path1))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path2, MMIAPICOM_Wstrlen(folder_path2))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, sd_folder_path1, MMIAPICOM_Wstrlen(sd_folder_path1))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, sd_folder_path2, MMIAPICOM_Wstrlen(sd_folder_path2))){                   
                    photo_index++;
                }else{
                    if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                        total_num--;
                    }else{
                        SCI_TRACE_LOW("[mmipic] FilterSpecialFolderFileArray sd delete fail!2string photo_index=%d",photo_index);
                        break;
                    }
                }
            }else{
                if(MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, folder_path1, MMIAPICOM_Wstrlen(folder_path1))
                    ||MMIAPICOM_Wstrnstr(file_info.filename, file_info.name_len, sd_folder_path1, MMIAPICOM_Wstrlen(sd_folder_path1))){     
                    photo_index++;
                }else{
                    if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                        total_num--;
                    }else{
                        SCI_TRACE_LOW("[mmipic] FilterSpecialFolderFileArray sd delete fail!1string photo_index=%d",photo_index);
                        break;
                    }
                }
            }

        }
    }
    if(photo_index == total_num){
        SCI_TRACE_LOW("[mmipic] FilterSpecialFolderFileArray finish arraysize=%d",total_num);
    }
}
PUBLIC void FilterFileArrayByAttri(FILEARRAY main_array,BOOLEAN keepVideo,BOOLEAN isforUI){
    FILEARRAY_DATA_T    file_info = {0};
    uint16 photo_index = 0;
    uint32 total_num= MMIAPIFILEARRAY_GetArraySize(main_array);
    wchar suffix_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
    uint16 suffix_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
    
    while(photo_index < total_num && !MMIPICVIEW_IsClose() && !MMIAPIPICVIEW_GetTaskCancelFlag()){
        if(!MMIAPIFILEARRAY_Read(main_array,photo_index, &file_info)){
            SCI_TRACE_LOW("[mmipic] FilterFileArrayByAttri local file read err!");
            break;
        }

        MMIAPIFMM_SplitFullPathExt(file_info.filename,MMIAPICOM_Wstrlen(file_info.filename),PNULL,PNULL, PNULL, PNULL, suffix_path, &suffix_path_length);
        if(MMIAPICOM_Wstrcmp(suffix_path,L".3gp")==0
            ||MMIAPICOM_Wstrcmp(suffix_path,L".mp4")==0
            ||MMIAPICOM_Wstrcmp(suffix_path,L".flv")==0
            ||MMIAPICOM_Wstrcmp(suffix_path,L".rmvb")==0
            ||MMIAPICOM_Wstrcmp(suffix_path,L".avi")==0){
            if(!keepVideo){
                if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                    total_num--;
                    if(isforUI){
                        SCI_TRACE_LOW("[mmipic] FilterFileArrayByAttri local delete pic icon");
                        MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,photo_index);
                    }
                }else{
                    SCI_TRACE_LOW("[mmipic] FilterFileArrayByAttri delete video fail!");
                }
            }else{
                photo_index++;
            }
        }else{
            if(keepVideo){
                if(MMIAPIFILEARRAY_Delete(main_array, photo_index)){
                    total_num--;
                    if(isforUI){
                        SCI_TRACE_LOW("[mmipic] FilterFileArrayByAttri delete video icon");
                        MMIPICVIEW_RemoveItem(MMIPICVIEW_LIST_CTRL_ID,photo_index);
                    }
                }else{
                    SCI_TRACE_LOW("[mmipic] FilterFileArrayByAttri delete pic fail!");
                }
            }else{
                photo_index++;
            }
        }

    }
    if(photo_index == total_num){
        SCI_TRACE_LOW("[mmipic] FilterFileArrayByAttri finish arraysize=%d,keepvideo%d",total_num,keepVideo);
    }
}
PUBLIC FILEARRAY MMIAPIPICVIEW_AllLocalPicFileArray(FILEARRAY main_array){
    return FilterThumbnailFileArray(main_array,L"D:\\Photos\\OneDrive\\Thumbnails",PNULL,L"E:\\Photos\\OneDrive\\Thumbnails",PNULL,FALSE,FALSE,FALSE,0);
}
PUBLIC FILEARRAY MMIAPIPICVIEW_AllLocalFileArray(FILEARRAY main_array,BOOLEAN isforUI,BOOLEAN isDelFile){
    return FilterThumbnailFileArray(main_array,L"D:\\Photos\\OneDrive\\Thumbnails",PNULL,L"E:\\Photos\\OneDrive\\Thumbnails",PNULL,isforUI,isDelFile,TRUE,0);
}
LOCAL FILEARRAY MMIAPIPICVIEW_AllLocalFileArrayForFile(FILEARRAY main_array,BOOLEAN isforUI,BOOLEAN isDelFile, uint16 limitsize){
    return FilterThumbnailFileArray(main_array,L"D:\\Photos\\OneDrive\\Thumbnails",PNULL,L"E:\\Photos\\OneDrive\\Thumbnails",PNULL,isforUI,isDelFile,TRUE,limitsize);
}
PUBLIC void MMIAPIPICVIEW_SaveArrayToFileLimit(
                                     MMIPICVIEW_FILEARRAY_TYPE_E filearray_type,
                                     uint16     *file_name,
                                     uint16     file_name_len
                                     ){
    BOOLEAN result = FALSE;
    uint32 array_size = 0;
    uint8 i=0;
    FILEARRAY main_array = PNULL;
    if(filearray_type == MMIPICVIEW_CURRENT_FILEARRAY){
        array_size = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_pic_array);
        main_array = s_mmipicview_pic_array;
    }else if(filearray_type == MMIPICVIEW_BACKUP_FILEARRAY){
        array_size = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_backup_main_pic_array);
        main_array = s_mmipicview_backup_main_pic_array;
    }else{
        return;
    }

#if	0 // jun.luo marked @ 2019.4.2 // fix issue:1033441
    if(array_size >VIRTUALARRAY_SIZE_SAVE_TO_FILE){
        FILEARRAY  limit_array = PNULL;
        FILEARRAY_DATA_T    file_info = {0};
        if(limit_array == PNULL){
            limit_array = MMIAPIFILEARRAY_Create();
        }
        for(i=0;i<VIRTUALARRAY_SIZE_SAVE_TO_FILE;i++){
            if(!MMIAPIFILEARRAY_Read(main_array,i, &file_info))
            {
                SCI_TRACE_LOW("[mmipic] MMIAPIPICVIEW_SaveArrayToFileLimit read err");
                break;
            }
            if(!MMIAPIFILEARRAY_Add(limit_array, &file_info)){
                SCI_TRACE_LOW("[mmipic] MMIAPIPICVIEW_SaveArrayToFileLimit add err");
            }
        }
        result = MMIAPIFILEARRAY_SaveArrayToFile(limit_array, file_name, file_name_len);
        MMIAPIFILEARRAY_Destroy(&limit_array);
    }else
#endif
    {
        result = MMIAPIFILEARRAY_SaveArrayToFile(main_array, file_name, file_name_len);
    }
    if(!result){
        SCI_TRACE_LOW("[mmipic] MMIAPIPICVIEW_SaveArrayToFileLimit error");
    }
}
PUBLIC void MMIAPIPICVIEW_FilterPicArrayByType(FILEARRAY main_array,PIC_SOURCE_TYPE type){
    SCI_TRACE_LOW("[mmipic] MMIAPIPICVIEW_FilterPicArrayByType type=%d",type);

    if(type == CAMERA_ROLL_SOURCE_ALL_LOCAL){
#if defined(CAMERAROLL_SUPPORT)
        if(MMISRV_CAMERAROLL_Get_Connect_State()&& MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
        {
               MMIAPIPICVIEW_AllLocalFileArray(main_array,FALSE,FALSE);

        }
#endif
    }else if(type == CAMERA_ROLL_SOURCE_CAPTURED){
        FilterSpecialFolderFileArray(main_array,L"D:\\Photos\\Camera Roll",L"D:\\Video",L"E:\\Photos\\Camera Roll",L"E:\\Video");
    }else if(type == CAMERA_ROLL_SOURCE_DONWLOADED){
        FilterSpecialFolderFileArray(main_array,L"D:\\Photos\\OneDrive",L"D:\\Photos\\Saved Pictures",L"E:\\Photos\\OneDrive",L"E:\\Photos\\Saved Pictures");
        FilterThumbnailFileArray(main_array,L"D:\\Photos\\OneDrive\\Thumbnails",PNULL,L"E:\\Photos\\OneDrive\\Thumbnails",PNULL,FALSE,FALSE,TRUE,0);
    }else if(type == CAMERA_ROLL_SOURCE_VIDEO){
        FilterFileArrayByAttri(main_array,TRUE,FALSE);
    }else if(type == CAMERA_ROLL_SOURCE_ONEDRIVE_ONLY){
        FilterSpecialFolderFileArray(main_array,L"D:\\Photos\\OneDrive\\Thumbnails",PNULL,L"E:\\Photos\\OneDrive\\Thumbnails",PNULL);
        //MMIPICVIEW_DestroyThumbnailsArray();
        //MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetThumbnailArray(),main_array);
     //   MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_THUMBNAIL_FILEARRAY,main_array);

    }
}

PUBLIC void MMIAPIPICVIEW_DelPicArrayItem(FILEARRAY main_array,const wchar *fullpathname,uint16 namelen){
    uint16 photo_index = 0;
    uint32 total_num= MMIAPIFILEARRAY_GetArraySize(main_array);
    FILEARRAY_DATA_T    file_info = {0};
        
    for (photo_index = 0; photo_index < total_num; photo_index ++)
    {
        if(!MMIAPIFILEARRAY_Read(main_array,photo_index, &file_info))
        {
            SCI_TRACE_LOW("MMIPICVIEW_IsFileExistUploadingPicArray READ ERROR!");
            break;
        }
        if(0 == MMIAPICOM_CompareTwoWstr(file_info.filename, file_info.name_len, fullpathname, namelen))
        {
            MMIAPIFILEARRAY_Delete(main_array, photo_index);
            break;
        }
    }
}

PUBLIC void MMIPICVIEW_GetThumbnailsByMainArray(FILEARRAY main_array){
    SCI_TRACE_LOW("[mmipic] MMIPICVIEW_GetThumbnailsByMainArray ");
    FilterSpecialFolderFileArray(main_array,L"D:\\Photos\\OneDrive\\Thumbnails",PNULL,L"E:\\Photos\\OneDrive\\Thumbnails",PNULL);
   // MMIPICVIEW_DestroyThumbnailsArray();
    //MMIAPIFILEARRAY_Combine(MMIAPIPICVIEW_GetThumbnailArray(),main_array);
    MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_THUMBNAIL_FILEARRAY,main_array);
}

PUBLIC void MMIPICVIEW_GetFileArrayFromOtherForArray(FILEARRAY dest_array,FILEARRAY source_array){
    MMIFILE_HANDLE file_handle = 0;
    if(MMIPICVIEW_IsClose()){
        return;
    }
    if(MMIAPIFMM_IsFileExist(s_gallery_list_file_temp_path , MMIAPICOM_Wstrlen(s_gallery_list_file_temp_path) ))
    {
        MMIAPIFILEARRAY_SaveArrayToFile(source_array, s_gallery_list_file_temp_path, MMIAPICOM_Wstrlen(s_gallery_list_file_temp_path));

    }	
    else
    {
        file_handle = MMIAPIFMM_CreateFile(s_gallery_list_file_temp_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
        MMIAPIFMM_CloseFile(file_handle);
        if(file_handle!=PNULL){
            MMIAPIFILEARRAY_SaveArrayToFile(source_array, s_gallery_list_file_temp_path, MMIAPICOM_Wstrlen(s_gallery_list_file_temp_path));
        }
    }
    MMIAPIFILEARRAY_LoadArrayFromFile(dest_array, s_gallery_list_file_temp_path ,MMIAPICOM_Wstrlen(s_gallery_list_file_temp_path));

   
}

PUBLIC void MMIPICVIEW_GetFileArrayFromOther(MMIPICVIEW_FILEARRAY_TYPE_E filearray_type,FILEARRAY source_array){
    MMIFILE_HANDLE file_handle = 0;
    FILEARRAY dest_array = PNULL;

    if(MMIPICVIEW_IsClose()){
        return;
    }
    if(MMIAPIFMM_IsFileExist(s_gallery_list_file_temp_path , MMIAPICOM_Wstrlen(s_gallery_list_file_temp_path) ))
    {
        MMIAPIFILEARRAY_SaveArrayToFile(source_array, s_gallery_list_file_temp_path, MMIAPICOM_Wstrlen(s_gallery_list_file_temp_path));
    }	
    else
    {
        file_handle = MMIAPIFMM_CreateFile(s_gallery_list_file_temp_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
        MMIAPIFMM_CloseFile(file_handle);
        if(file_handle!=PNULL){
            MMIAPIFILEARRAY_SaveArrayToFile(source_array, s_gallery_list_file_temp_path, MMIAPICOM_Wstrlen(s_gallery_list_file_temp_path));
        }
    }
    if(filearray_type == MMIPICVIEW_CURRENT_FILEARRAY){
        MMIPICVIEW_DestroyAllPicArray();
        MMIPICVIEW_CreateAllPicArray();
        dest_array = s_mmipicview_pic_array;
    }else if(filearray_type == MMIPICVIEW_THUMBNAIL_FILEARRAY){
        MMIPICVIEW_DestroyThumbnailsArray();
        dest_array = MMIAPIPICVIEW_GetThumbnailArray();
    }else if(filearray_type == MMIPICVIEW_BACKUP_FILEARRAY){
        MMIAPIPICVIEW_DestroyBackupAllPicArray();
        dest_array = MMIAPIPICVIEW_GetBackupAllPicArray();  
    }else if(filearray_type == MMIPICVIEW_SEARCH_FILEARRAY){
        MMIAPIPICVIEW_DestroySearchAllPicArray();
        dest_array = MMIAPIPICVIEW_GetSearchAllPicArray();  
    }else if(filearray_type == MMIPICVIEW_UPLOADING_FILEARRAY){
        MMIPICVIEW_DestroyAllUploadingPicArray();
        MMIPICVIEW_CreateAllUploadingPicArray();
        dest_array = MMIPICVIEW_GetAllUploadingPicArray();
    }else if(filearray_type == MMIPICVIEW_PREVIOUS_FILEARRAY){
        MMIAPIPICVIEW_DestroyPreviousPicArray();
        dest_array = MMIPICVIEW_GetPreviousPicArray();
    }
    MMIAPIFILEARRAY_LoadArrayFromFile(dest_array, s_gallery_list_file_temp_path ,MMIAPICOM_Wstrlen(s_gallery_list_file_temp_path));
}

PUBLIC GALLERY_FILEARRAY_SIG_TYPE_E MMIPICVIEW_GetCurrentOwnTaskId(void){
    return s_current_own_task;
}
PUBLIC void MMIPICVIEW_SetCurrentOwnTaskId(GALLERY_FILEARRAY_SIG_TYPE_E task_signal){
    s_current_own_task = task_signal;
}

PUBLIC void MMIPICVIEW_ReloadAfterSearchEx(void)
{
    FILEARRAY temp_array = PNULL;
    
    if(MMIPICVIEW_IsClose()){
        return;
    }
    
    temp_array = s_mmipicview_previous_pic_array;
    s_mmipicview_previous_pic_array = s_mmipicview_pic_array;
    s_mmipicview_pic_array = s_mmipicview_search_pic_array;
    s_mmipicview_search_pic_array = NULL;
    MMIAPIFILEARRAY_Destroy(&temp_array);
    
    if(MMIPICVIEW_GetListEmpty() 
        && MMIPICVIEW_GetAllPicNum()>0
        &&!MMK_IsActiveCtrl(MMIPICVIEW_LIST_CTRL_ID)){
        CreatePicListCtrl(MMIPICVIEW_LIST_WIN_ID, MMIPICVIEW_LIST_CTRL_ID);
    }
}

//zhuyongjing add end

#endif //#ifdef PIC_VIEWER_SUPPORT
