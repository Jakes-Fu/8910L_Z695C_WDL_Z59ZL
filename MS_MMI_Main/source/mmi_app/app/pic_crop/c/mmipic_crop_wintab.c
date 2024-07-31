/*************************************************************************
 ** File Name:      mmipic_crop_wintab.c                                          *
 ** Author:           chunyou                                           *
 ** Date:             12/25/2011                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about pic crop             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 12/2011       chunyou         Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_pic_crop_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef MMI_PIC_CROP_SUPPORT
#include "os_api.h"
#include "mmipic_crop.h"
//#include "mmipic_crop_menutable.h"
#include "mmifmm_export.h"
#include "guiiconlist.h"
#include "mmipic_crop_id.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "guiiconlist.h"
#if defined(MMI_PDA_SUPPORT)
#include "guibutton.h"
#endif
#include "mmifmm_id.h"
#include "mmipic_crop_image.h"
#include "mmipic_crop_nv.h" 
#include "mmi_appmsg.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmitheme_anim.h"
#include "guilistbox.h"
#include "mmiacc_event.h"
#include "mmischedule_export.h"
#include "guidropdownlist.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_mainmenu_export.h"
#include "mmisd_export.h"
#include "guiedit.h"
#include "mmipicview_export.h"
#include "mmi_default.h"
#include "guires.h"
#include "guiownerdraw.h"
#include "mmipic_crop_text.h"
#include "guisetlist.h"
#include  "mmipicview_id.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

LOCAL MMIPICCROP_INFO_T       s_crop_info = {0};
LOCAL MMIPICCROP_INFO_T*      s_crop_info_ptr = &s_crop_info;
//lijinghua add it for pic crop
LOCAL uint16  s_icon_crop_index=0;         
LOCAL uint16  s_icon_square_crop_index=0; 
LOCAL uint16  s_icon_landscape_crop_index=0; 
LOCAL BOOLEAN s_icon_square_crop=FALSE;
LOCAL BOOLEAN s_icon_landscape_crop=FALSE;
LOCAL BOOLEAN s_icon_is_crop=FALSE;

//lijinghua end
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle edit crop window msg
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicCropMainWinMsg (
                                                          MMI_WIN_ID_T      win_id,
                                                          MMI_MESSAGE_ID_E  msg_id, 
                                                          DPARAM            param
                                                          );
LOCAL MMI_RESULT_E HandleCropSaveWaitingSpinnerWin(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);
/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/
// the window for  pic crop main window
WINDOW_TABLE(MMIPIC_CROP_MAIN_WIN_TAB) = 
{
        //WIN_HIDE_STATUS,                         //lijinghua delete it
        WIN_FUNC( (uint32)HandlePicCropMainWinMsg),
        WIN_ID( MMIPIC_CROP_MAIN_WIN_ID),
		WIN_STATUSBAR,             //lijinghua add it
        WIN_STYLE( WS_DISABLE_COMMON_BG),

        //CREATE_BUTTON_CTRL(IMAGE_PICCROP_OK, PIC_CROP_OK_BUTTON_CTRL_ID),         //lijinghua delete it
        //CREATE_BUTTON_CTRL(IMAGE_PICCROP_CANCEL, PIC_CROP_BACK_BUTTON_CTRL_ID),   //lijinghua delete it
        //CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, PIC_CROP_TITLE_LABEL_CTRL_ID),
    
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
        //WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),          //lijinghua delete it
		WIN_SOFTKEY(STXT_SOFTKEY_MARK_MK, IMAGE_PICCROP_DECREASE, TXT_COMMON_COMMON_BACK), //lijinghua add it
        END_WIN
};
WINDOW_TABLE(MMIPICCROP_WAIT_SPINNER_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleCropSaveWaitingSpinnerWin),
    WIN_ID(MMIPICCROP_WAIT_SPINNER_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_SPINNER_CTRL(PIC_CROP_WAIT_SPINNER_CTRL_ID),
    END_WIN
};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : exit from pic crop
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICCROP_ExitFromPicCrop(void)
{
    MMIPICCROP_ExitFromPicCrop();
}
/*****************************************************************************/
//  Description : init pic crop module
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICCROP_InitModule(void)
{
    MMIPICCROP_RegMenuGroup();
    MMIPICCROP_RegNv();
}
/*****************************************************************************/
//  Description : open pic crop 
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICCROP_OpenPicCrop(
                                              MMIPICCROP_SRC_T* crop_src     //in:
                                              )
{
//enter pic crop disable 3D;exit piceditor enable 3D
    //MMI_Enable3DMMI(FALSE);
#ifdef UI_P3D_SUPPORT
    //MMI_Disable3DEffect(MMI_3D_EFFECT_PIC_EDITOR);
#endif
    return MMIPICCROP_OpenPicEditorMainWin(crop_src);
}

/*****************************************************************************/
//  Description : get crop info ptr
//  Global resource dependence : s_crop_info_ptr
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMIPICCROP_INFO_T* MMIPICCROP_GetCropInfoPtr(void)
{
    return s_crop_info_ptr;
}

/*****************************************************************************/
//  Description : open pic crop 
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICCROP_OpenPicEditorMainWin(
                                           MMIPICCROP_SRC_T* crop_src     //in:
                                           )
{
    BOOLEAN result = FALSE;
    MMI_PICCROP_ERROR_TYPE_E err_type = MMI_PICCROP_ERR_EXIT;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();
    
    err_type = MMIPICCROP_Init(crop_src);
    if(MMI_PICCROP_SUCCESS == err_type)
    {
        result = MMK_CreateWin((uint32*)MMIPIC_CROP_MAIN_WIN_TAB, (ADD_DATA)0);
    }
    else
    {
        MMIPICCROP_ErrorCallback(err_type);
        MMIPICCROP_Exit();
        if(MMI_PICCROP_ERR_NOT_SUPPORT != err_type)
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR_OCCUR);
        }
        //zhuyongjing add for pb crop
        if(crop_info_ptr->crop_type == MMIPIC_CROP_PB
            &&MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)
            &&MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_PB){
            MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
            MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE);
        }
        //add end
    }

    return result;
}

/*****************************************************************************/
//  Description : resert crop info
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL void CropFrameRectInit(void)
{
    MMI_WIN_ID_T    win_id = MMIPIC_CROP_MAIN_WIN_ID;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    uint16 width = 0;
    uint16 height = 0;
    uint16 min_width = 0;
    GUI_RECT_T      client_rect = MMIPICCROP_GetClientRect();
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropFrameRectInit crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_191_112_2_18_2_46_15_40,(uint8*)"");
        return ;
    }

    GUIRES_GetImgWidthHeight(&crop_info_ptr->frame_img_width, &crop_info_ptr->frame_img_height, IMAGE_PICCROP_POINT, win_id);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    min_width = crop_info_ptr->frame_img_width + MMIPICCROP_MOVE_POINT_OFFSET*2;
    //crop_info_ptr->contact_img_min_width = min_width;
    switch(crop_info_ptr->crop_type)
    {
    case MMIPIC_CROP_WALLPAPER:
    case MMIPIC_CROP_DEFAULT_SAVE:
	case MMIPIC_CROP_PB:                          //lijinghua add for PB
        crop_info_ptr->scale_width = crop_info_ptr->min_width;
        crop_info_ptr->scale_height = crop_info_ptr->min_height;
        break;
        
    default:    
        break;
    }
    if(MMIPIC_CROP_WALLPAPER == crop_info_ptr->crop_type)
    {
        if(MMIPICCROP_IsPermitCrop())
        {
            width = (uint16)(crop_info_ptr->display_rect.right - crop_info_ptr->display_rect.left + 1) * crop_info_ptr->scale_width  / crop_info_ptr->original_img.image_width;
            height = (uint16)(crop_info_ptr->display_rect.bottom - crop_info_ptr->display_rect.top + 1) * lcd_height / crop_info_ptr->original_img.image_height;
                
            if(width < min_width)
            {
                width = min_width;
                height = width * crop_info_ptr->scale_height / crop_info_ptr->scale_width;
            }
            else if(height < min_width)
            {
                height = min_width;
                width = height * crop_info_ptr->scale_width / crop_info_ptr->scale_height;
            }

            crop_info_ptr->crop_frame_rect.left = client_rect.left + (client_rect.right - client_rect.left + 1 - width)/2;
            crop_info_ptr->crop_frame_rect.top = client_rect.top + (client_rect.bottom - client_rect.top + 1 - height)/2;
            crop_info_ptr->crop_frame_rect.right = crop_info_ptr->crop_frame_rect.left + width -1 ;
            crop_info_ptr->crop_frame_rect.bottom = crop_info_ptr->crop_frame_rect.top + height - 1;
        }
        else
        {
            crop_info_ptr->crop_frame_rect.left = crop_info_ptr->display_rect.left;
            crop_info_ptr->crop_frame_rect.top = crop_info_ptr->display_rect.top;
            crop_info_ptr->crop_frame_rect.right = crop_info_ptr->display_rect.right;
            crop_info_ptr->crop_frame_rect.bottom = crop_info_ptr->display_rect.bottom;
        }
    }
    else
    {
        if(MMIPICCROP_IsPermitCrop())
        {
            width = MIN(((client_rect.right - client_rect.left + 1)/2),  ((client_rect.bottom - client_rect.top + 1 )/2));
            height = (uint16)(width * crop_info_ptr->scale_height/crop_info_ptr->scale_width);
            if(width > crop_info_ptr->original_img.image_width || height > crop_info_ptr->original_img.image_height)
            {
                width = MIN(crop_info_ptr->original_img.image_width, crop_info_ptr->original_img.image_height) - 2;
                height = (uint16)(width * crop_info_ptr->scale_height/crop_info_ptr->scale_width);
            }
            crop_info_ptr->crop_frame_rect.left = client_rect.left + (client_rect.right - client_rect.left - width + 1)/2;
            crop_info_ptr->crop_frame_rect.top = client_rect.top + (client_rect.bottom - client_rect.top - height + 1)/2;
            crop_info_ptr->crop_frame_rect.right = crop_info_ptr->crop_frame_rect.left + width - 1;
            crop_info_ptr->crop_frame_rect.bottom = crop_info_ptr->crop_frame_rect.top + height - 1;
        }
        else
        {
            crop_info_ptr->crop_frame_rect.left = crop_info_ptr->display_rect.left;
            crop_info_ptr->crop_frame_rect.top = crop_info_ptr->display_rect.top;
            crop_info_ptr->crop_frame_rect.right = crop_info_ptr->display_rect.right;
            crop_info_ptr->crop_frame_rect.bottom = crop_info_ptr->display_rect.bottom;
        }
    }
}

/*****************************************************************************/
//  Description : exit from pic crop
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICCROP_ExitFromPicCrop(void)
{
    if(MMK_IsOpenWin(MMIPIC_CROP_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIPIC_CROP_MAIN_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPICCROP_WAIT_SAVE_PUBWIN_WIN_ID))
    {
        MMK_CloseWin(MMIPICCROP_WAIT_SAVE_PUBWIN_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIPICCROP_WAIT_PUBWIN_WIN_ID))
    {
        MMK_CloseWin(MMIPICCROP_WAIT_PUBWIN_WIN_ID);
    }  
    if(MMK_IsOpenWin(MMIPICCROP_WAIT_SPINNER_WIN_ID))
    {
        MMK_CloseWin(MMIPICCROP_WAIT_SPINNER_WIN_ID);
    } 
  //  MMIPUB_CloseWaitWin(MMIPIC_CROP_MAIN_WIN_ID);
}
LOCAL MMI_RESULT_E HandleCropSaveWaitingSpinnerWin(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_PICCROP_ERROR_TYPE_E err_type = MMI_PICCROP_ERR_EXIT;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();
    MMI_HANDLE_T spinner_ctr_id = NULL;
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_STRING_T            spinner_text = {0};
    static uint8        s_wait_ready_timer_id = 0;
    static BOOLEAN        finish = FALSE;
    SCI_TRACE_LOW(" HandleCropSaveWaitingSpinnerWin ,msg_id=%x",msg_id);
    spinner_ctr_id = MMK_GetCtrlHandleByWin(win_id, PIC_CROP_WAIT_SPINNER_CTRL_ID);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (s_wait_ready_timer_id != 0)
        {
            MMK_StopTimer(s_wait_ready_timer_id);
            s_wait_ready_timer_id = 0;
        }
        s_wait_ready_timer_id = MMK_CreateWinTimer(win_id, 500,TRUE);
        ctrl_info.ctrl_id = spinner_ctr_id;
        ctrl_info.is_ctrl_id = TRUE;
        data_info.img_id = IMAGE_CIRCULAR_LOADER;
        display_info.align_style    = GUIANIM_ALIGN_NONE;
        display_info.is_play_once   = FALSE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
        CTRLANIM_SetSpinnerTextById(spinner_ctr_id,STR_NOTE_WAITING);
        finish = FALSE;
        MMK_PostMsg(win_id, MSG_PIC_CROP_SAVE,(DPARAM)PNULL, (uint32)0);      
        break;
    case MSG_GET_FOCUS:
        if (s_wait_ready_timer_id != 0)
        {
            MMK_StopTimer(s_wait_ready_timer_id);
            s_wait_ready_timer_id = 0;
        }
        s_wait_ready_timer_id = MMK_CreateWinTimer(win_id, 500,TRUE);
        ctrl_info.ctrl_id = spinner_ctr_id;
        ctrl_info.is_ctrl_id = TRUE;
        data_info.img_id = IMAGE_CIRCULAR_LOADER;
        display_info.align_style    = GUIANIM_ALIGN_NONE;
        display_info.is_play_once   = FALSE;
        display_info.is_bg_buf      = TRUE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
        CTRLANIM_SetSpinnerTextById(spinner_ctr_id,STR_NOTE_WAITING);      
        break;
    case MSG_PIC_CROP_SAVE:
        err_type = MMIPICCROP_GetCropImageData();
        if(MMI_PICCROP_SUCCESS != err_type)
        {
            //SCI_TRACE_LOW:"[MMIPICCROP] HandleCropSaveWaitingWin MMIPICCROP_GetCropImageData error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_320_112_2_18_2_46_15_43,(uint8*)"");
            MMIPUB_OpenAlertWarningWin(TXT_ERROR_OCCUR);
            MMIPICCROP_ErrorCallback(err_type);
            MMIPICCROP_ExitFromPicCrop();
            break;
        }
        err_type = MMIPICCROP_Save();
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_327_112_2_18_2_46_15_44,(uint8*)"d", err_type);
        switch(err_type)
        {
        case MMI_PICCROP_SUCCESS:
            if(PNULL != crop_info_ptr->callback_fun)
            {
            //zhuyongjing add for show ui
                MMIAPIPICVIEW_ReloadFile(); 
                MMIPICVIEW_InsertPicArray(crop_info_ptr->random_name_ptr);
            //add end
                crop_info_ptr->callback_t.full_path_ptr = crop_info_ptr->random_name_ptr;
                crop_info_ptr->callback_t.full_path_len = crop_info_ptr->random_name_len;
                crop_info_ptr->callback_t.type = crop_info_ptr->crop_type;
                crop_info_ptr->callback_t.err_type = err_type;
                crop_info_ptr->callback_t.is_pre_wallpaper = crop_info_ptr->is_pre_wallpaper;
                crop_info_ptr->callback_fun(&crop_info_ptr->callback_t);
            }
           MMIPICCROP_ExitFromPicCrop();
            break;
        case MMI_PICCROP_ERR_NO_SPACE:
            MMIPICCROP_ShowMsg(err_type);
            MMIPICCROP_ErrorCallback(err_type);
            MMIPICCROP_ExitFromPicCrop();
            break;
        default:
            MMIPICCROP_ShowMsg(err_type);
            MMIPICCROP_ErrorCallback(err_type);
            MMIPICCROP_ExitFromPicCrop();
            break;
        }
        break;
    case MSG_TIMER:  
        if(MMK_IsFocusWin(win_id))
        {
            SCI_TRACE_LOW("[MMIPICVIEW_list]  HandleLoadPicSpinnerWinMsg finish=%d,s_wait_ready_timer_id=%x,para=%d",finish,s_wait_ready_timer_id,*((uint8 *) param));
            if(s_wait_ready_timer_id == *((uint8 *) param) && finish)
            {              
                MMK_StopTimer(s_wait_ready_timer_id); 
                s_wait_ready_timer_id = 0;
                MMK_CloseWin(win_id);
            }
        }
        break;
    case MSG_APP_FLIP:
    case MSG_APP_CANCEL:
        if(0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
        }
        break;
    case MSG_LOSE_FOCUS:
        if(0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
        }
        break;
    case MSG_CLOSE_WINDOW:
        if(0 != s_wait_ready_timer_id){
            MMK_StopTimer(s_wait_ready_timer_id); 
            s_wait_ready_timer_id = 0;
        }
         //zhuyongjing add for pb crop
        if(crop_info_ptr->crop_type == MMIPIC_CROP_PB
            &&MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)
            &&MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_PB){
            MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
            MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_NONE);
        }
        //add end
        break;
    default:

        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : exit from pic crop
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCropSaveWaitingWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_PICCROP_ERROR_TYPE_E err_type = MMI_PICCROP_ERR_EXIT;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] HandleCropSaveWaitingWin crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_304_112_2_18_2_46_15_41,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMIPICCROP]: HandleCropSaveWaitingWin, msg=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_308_112_2_18_2_46_15_42,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_PostMsg(win_id, MSG_PIC_CROP_SAVE,(DPARAM)PNULL, (uint32)0);        
        MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    case MSG_PIC_CROP_SAVE:
        err_type = MMIPICCROP_GetCropImageData();
        if(MMI_PICCROP_SUCCESS != err_type)
        {
            //SCI_TRACE_LOW:"[MMIPICCROP] HandleCropSaveWaitingWin MMIPICCROP_GetCropImageData error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_320_112_2_18_2_46_15_43,(uint8*)"");
            MMIPUB_OpenAlertWarningWin(TXT_ERROR_OCCUR);
            MMIPICCROP_ErrorCallback(err_type);
            MMIPICCROP_ExitFromPicCrop();
            break;
        }
        err_type = MMIPICCROP_Save();
        //SCI_TRACE_LOW:"[MMIPICCROP]: HandleCropSaveWaitingWin, err_type=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_327_112_2_18_2_46_15_44,(uint8*)"d", err_type);
        switch(err_type)
        {
        case MMI_PICCROP_SUCCESS:
            //MMIPICCROP_ShowMsg(pic_result);
            if(PNULL != crop_info_ptr->callback_fun)
            {
                crop_info_ptr->callback_t.full_path_ptr = crop_info_ptr->random_name_ptr;
                crop_info_ptr->callback_t.full_path_len = crop_info_ptr->random_name_len;
                crop_info_ptr->callback_t.type = crop_info_ptr->crop_type;
                crop_info_ptr->callback_t.err_type = err_type;
                crop_info_ptr->callback_t.is_pre_wallpaper = crop_info_ptr->is_pre_wallpaper;
                crop_info_ptr->callback_fun(&crop_info_ptr->callback_t);
            }
            MMIPICCROP_ExitFromPicCrop();
            break;
        case MMI_PICCROP_ERR_NO_SPACE:
            MMIPICCROP_ShowMsg(err_type);
            MMIPICCROP_ErrorCallback(err_type);
            MMIPICCROP_ExitFromPicCrop();
            break;
        default:
            MMIPICCROP_ShowMsg(err_type);
            MMIPICCROP_ErrorCallback(err_type);
            MMIPICCROP_ExitFromPicCrop();
            break;
        }
        break;

    case MSG_APP_CANCEL:
#ifdef PIC_EDITOR_SUPPORT
        PIC_CROP_SetSquareCropStatus(FALSE);
        PIC_CROP_SetLandscapeCropStatus(FALSE);
#endif
        break;

    case MSG_APP_RED:
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
#ifdef PIC_EDITOR_SUPPORT
        PIC_CROP_SetSquareCropStatus(FALSE);
        PIC_CROP_SetLandscapeCropStatus(FALSE);
#endif
        MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}
/*****************************************************************************/
//  Description : CropOkButtonProc
//  Global resource dependence : 
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CropOkButtonProc(void)
{
    MMI_WIN_ID_T    alert_win_id = MMIPICCROP_WAIT_SAVE_PUBWIN_WIN_ID;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();
    uint32 time_out = 0;

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropOkButtonProc crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_386_112_2_18_2_46_15_45,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
//lijinghua delete it temporary
    /*if(!MMIPICCROP_IsPermitCrop())
    {
        if(PNULL != crop_info_ptr->callback_fun)
        {
            crop_info_ptr->callback_t.full_path_ptr = crop_info_ptr->file_info.full_path_wstr_ptr;
            crop_info_ptr->callback_t.full_path_len = crop_info_ptr->file_info.full_path_wstr_len;
            crop_info_ptr->callback_t.type = crop_info_ptr->crop_type;
            crop_info_ptr->callback_t.err_type = MMI_PICCROP_SUCCESS_NO_CROP;
            crop_info_ptr->callback_t.is_pre_wallpaper = crop_info_ptr->is_pre_wallpaper;
            crop_info_ptr->callback_fun(&crop_info_ptr->callback_t);
        }
        MMIPICCROP_ExitFromPicCrop();
    }*/
    //else
    //{
      //  MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&alert_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleCropSaveWaitingWin);
    //}
    MMK_CreateWin((uint32 *)MMIPICCROP_WAIT_SPINNER_WIN_TAB, PNULL);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : CropBackButtonProc
//  Global resource dependence : 
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CropBackButtonProc(void)
{    
    MMIPICCROP_ErrorCallback(MMI_PICCROP_ERR_CANCEL);
    MMIPICCROP_ExitFromPicCrop();
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : SetCtrlParam
//  Global resource dependence : none
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL void CropSetCtrlParam(void)
{
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    GUI_RECT_T rect = {0};
    //GUI_RECT_T label_rect = {0};
    //MMI_STRING_T cur_file_s = {0};
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropSetCtrlParam crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_436_112_2_18_2_46_15_46,(uint8*)"");
        return ;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);  
    
    GUIBUTTON_SetCallBackFunc(PIC_CROP_OK_BUTTON_CTRL_ID, CropOkButtonProc);
    GUIBUTTON_SetCallBackFunc(PIC_CROP_BACK_BUTTON_CTRL_ID, CropBackButtonProc);

    GUIBUTTON_PermitBorder(PIC_CROP_OK_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(PIC_CROP_BACK_BUTTON_CTRL_ID, FALSE);
    //GUIBUTTON_SetRunSheen(PIC_CROP_OK_BUTTON_CTRL_ID,FALSE);
    //GUIBUTTON_SetRunSheen(PIC_CROP_BACK_BUTTON_CTRL_ID,FALSE);

    //GUILABEL_SetFont(PIC_CROP_TITLE_LABEL_CTRL_ID,  MMIPICCROP_FILENAME_FONT, MMI_WHITE_COLOR);

    rect.left = lcd_width/2;
    rect.top = lcd_height - MMIPICCROP_BOTTOM_HEIGHT - 1;
    rect.right = lcd_width - 1;
    rect.bottom = lcd_height - 1;
    GUIBUTTON_SetRect(PIC_CROP_BACK_BUTTON_CTRL_ID, &rect);

    //label_rect.left = rect.right + 1;
    //label_rect.top = 0;
    
    rect.left = 0;
    rect.top = lcd_height - MMIPICCROP_BOTTOM_HEIGHT - 1;
    rect.right = lcd_width/2;
    rect.bottom = lcd_height - 1;
    GUIBUTTON_SetRect(PIC_CROP_OK_BUTTON_CTRL_ID, &rect);
    
    /*label_rect.right = lcd_width - MMIPICCROP_BUTTON_SPACE - img_width - 1;
    label_rect.bottom = MMIPICCROP_TITLE_HEIGHT - 1;
    GUILABEL_SetRect(PIC_CROP_TITLE_LABEL_CTRL_ID, &label_rect, FALSE);
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_PIC_CROP, &cur_file_s);    
    GUILABEL_SetText(PIC_CROP_TITLE_LABEL_CTRL_ID, &cur_file_s, FALSE);*/
}

/*****************************************************************************/
//  Description : handle edit crop window msg
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePicCropMainWinMsg (
                                                          MMI_WIN_ID_T      win_id,
                                                          MMI_MESSAGE_ID_E  msg_id, 
                                                          DPARAM            param
                                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_PICCROP_ERROR_TYPE_E err_type = MMI_PICCROP_ERR_EXIT;
    //static PICCROP_EDIT_FILE_INFO_T * s_file_info_ptr = PNULL;
    static BOOLEAN s_is_decoded = FALSE;
    MMI_WIN_ID_T    alert_win_id = MMIPICCROP_WAIT_PUBWIN_WIN_ID;
    MMI_WIN_ID_T    nosupport_win_id = MMIPICCROP_NOSUPPORT_PUBWIN_WIN_ID;
    //s_file_info_ptr = (PICCROP_EDIT_FILE_INFO_T*)MMK_GetWinAddDataPtr(win_id); 
#ifdef PIC_EDITOR_SUPPORT
	s_icon_square_crop = PIC_CROP_GetSquareCropStatus();              //lijinghua add it
	s_icon_landscape_crop = PIC_CROP_GetLandscapeCropStatus();        //lijinghua add it
#endif
    SCI_TRACE_LOW("HandlePicCropMainWinMsg: msg_id=%x",msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: MSG_OPEN_WINDOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_496_112_2_18_2_46_15_47,(uint8*)"");
        //zhuyongjing delete for unsupport pic
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&alert_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);        
        //init
        //CropSetCtrlParam();        
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MARK_MK, LEFT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_PICCROP_DECREASE, MIDDLE_BUTTON, FALSE);
        GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,MMI_BLACK_COLOR,0);

        err_type = MMIPICCROP_DecodeJpgImage();
        if(MMI_PICCROP_SUCCESS == err_type)
        {
            s_is_decoded    =TRUE;
        }
        else
        {
            s_is_decoded=FALSE;
            MMIPICCROP_ErrorCallback(err_type);
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_FILE_NOT_SUPPORT,TXT_NULL,IMAGE_PUBWIN_WARNING,&nosupport_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);				
            MMK_CloseWin(win_id);
        }
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: enter pic editor  "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_514_112_2_18_2_46_15_48,(uint8*)"");
        break;
    case MSG_FULL_PAINT:
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: MSG_FULL_PAINT s_is_decoded = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_517_112_2_18_2_46_15_49,(uint8*)"d", s_is_decoded);
        if(TRUE == s_is_decoded)
        {
            MMIPICCROP_DisplayImage(msg_id);
        }
        break;

    case MSG_PIC_GET_DATA_CNF:
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: get file date success then begin decode jpg image "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_525_112_2_18_2_46_15_50,(uint8*)"");
        break;
    case MSG_PIC_DISPLAY_IND:
        if(MMK_IsOpenWin(alert_win_id))
        {
            MMK_CloseWin(alert_win_id);
        }
        
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg:  decode jpg image success "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_533_112_2_18_2_46_16_51,(uint8*)"");
        
        if (!MMIPICCROP_MallocMemForPicProcess())
        {
            MMIPICCROP_ErrorCallback(MMI_PICCROP_ERR_NO_MEMORY);
            MMIPUB_OpenAlertWarningWin(TXT_ERROR_OCCUR);				
            MMK_CloseWin(win_id);
        }
        else
        {
            CropFrameRectInit();
            MMIPICCROP_DisplayImage(msg_id);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_CANCEL_MK, RIGHT_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MARK_MK, LEFT_BUTTON, TRUE);
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_PICCROP_DECREASE, MIDDLE_BUTTON, TRUE);
            GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,MMI_BLACK_COLOR,0);

        }
        break;
        
    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: MSG_GET_FOCUS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_549_112_2_18_2_46_16_52,(uint8*)"");
        s_is_decoded=TRUE;
        break;
        
    case MSG_LOSE_FOCUS:
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: MSG_LOSE_FOCUS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_554_112_2_18_2_46_16_53,(uint8*)"");
        break;
        
    case MSG_TIMER:    
        break;

	case MSG_CTL_OK:
    case MSG_APP_OK:
	{
	//lijinghua modify it for pic crop
              MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();
#ifdef PIC_EDITOR_SUPPORT
		s_icon_is_crop=PIC_GetIsCrop();
		if(s_icon_is_crop)
		{
			MMIAPIPICCROP_OnlySaveOrgImgToUndoBuf();
			MMIPICCROP_GetCropImageData();
			MMIAPIPICCROP_CopyOutputImgDataToOrgImgData();
			MMIAPIPICCROP_CopyOutputRectToOrgRect();
			MMIAPIPICCROP_RecordChangedTimes();
			MMIPICCROP_ExitFromPicCrop();
		}else
#endif
		{
			CropOkButtonProc();
		}
	//lijinghua end
        }
        break;
//lijinghua add it for pic crop
	case MSG_APP_WEB:
		GUIWIN_SetSoftkeyBtnGray(win_id,0,FALSE);
		if(s_icon_square_crop)
		{
                    if(MMIPICCROP_IsPermitCrop()){
			if(s_icon_square_crop_index<2)
			{
				s_icon_square_crop_index++;
			}else{
				s_icon_square_crop_index=0;
			}
                    }else{
                        if(s_icon_square_crop_index == 0){
                            s_icon_square_crop_index = 2;
                        }else{
                            s_icon_square_crop_index = 0;
                        }
                    }
		}else if(s_icon_landscape_crop)
		{
                    if(MMIPICCROP_IsPermitCrop()){
			if(s_icon_landscape_crop_index<2)
			{
				s_icon_landscape_crop_index++;
			}else{
				s_icon_landscape_crop_index=0;
			}
                    }else{
                        if(s_icon_landscape_crop_index == 0){
                            s_icon_landscape_crop_index = 2;
                        }else{
                            s_icon_landscape_crop_index = 0;
                        }
                    }
		}else
		{
                    if(MMIPICCROP_IsPermitCrop()){
			if(s_icon_crop_index<2)
			{
				s_icon_crop_index++;
			}else{
				s_icon_crop_index=0;
			}
                    }else{
                        if(s_icon_crop_index == 0){
                            s_icon_crop_index = 2;
                        }else{
                            s_icon_crop_index = 0;
                        }
                    }
		}
                if(MMIPICCROP_IsPermitCrop()){
		MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
		if(s_icon_crop_index == 2 || s_icon_square_crop_index == 2 || s_icon_landscape_crop_index==2)
		{
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_PICCROP_INCREASE, MIDDLE_BUTTON, TRUE);
		}else{
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_PICCROP_DECREASE, MIDDLE_BUTTON, TRUE);
		}
		break;	
//lijinghua add end
    case MSG_APP_MENU:
        //MMK_CreateWin((uint32 *)MMIPIC_EDIT_MENU_WIN_TAB,PNULL);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        if ((PNULL != param) && ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            || (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
            || (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)))
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {               
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {                
            }
            break;
        }
#endif //MMI_PDA_SUPPORT
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:          
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: MSG_APP_CANCEL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_592_112_2_18_2_46_16_54,(uint8*)"");
#ifdef PIC_EDITOR_SUPPORT
		//lijinghua add it
        PIC_CROP_SetSquareCropStatus(FALSE);
        PIC_CROP_SetLandscapeCropStatus(FALSE);
        PIC_SetIsCrop(FALSE);
		//lijinghua add end
#endif
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: MSG_APP_RED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_597_112_2_18_2_46_16_55,(uint8*)"");
        MMK_CloseWin(win_id);
        MMK_ReturnIdleWin();
        break;
	
    case MSG_PROMPTWIN_OK: 
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_PROMPTWIN_CANCEL: //MSG_ACC_MSGBOX_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_APP_UP:         
    case MSG_APP_DOWN:
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
	//lijinghua add it
	case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
	//lijinghua end
		MMIPICCROP_HandleMsg(msg_id,win_id);
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: MSG_APP_DIRECTION"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_613_112_2_18_2_46_16_56,(uint8*)"");
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(s_is_decoded)
            {
                MMIPICCROP_HandleTpMsg(point, msg_id);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point1 = {0};
            
            point1.x = MMK_GET_TP_X(param);
            point1.y = MMK_GET_TP_Y(param);
            if(s_is_decoded)
            {
                MMIPICCROP_HandleTpMsg(point1, msg_id);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T point1 = {0};
            
            point1.x = MMK_GET_TP_X(param);
            point1.y = MMK_GET_TP_Y(param);
            if(s_is_decoded)
            {
                MMIPICCROP_HandleTpMsg(point1, msg_id);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        //SCI_TRACE_LOW:"HandlePicCropMainWinMsg: MSG_CLOSE_WINDOW"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_WINTAB_654_112_2_18_2_46_16_57,(uint8*)"");
		//lijinghua modify it
#ifdef PIC_EDITOR_SUPPORT
        s_icon_is_crop=PIC_GetIsCrop();
        if(!s_icon_is_crop)
        {
        	MMIPICCROP_Exit();
        }
#endif
		//lijinghua end
        //enter piceditor disable 3D;exit piceditor enable 3D
#ifdef UI_P3D_SUPPORT
        //MMI_Enable3DEffect(MMI_3D_EFFECT_PIC_EDITOR);
#endif
//lijinghua add it for pic crop
		MMIPICCROP_HandleMsg(msg_id,win_id);
		s_icon_crop_index=0;                                  
		s_icon_square_crop_index=0;
		s_icon_landscape_crop_index=0;
		s_icon_square_crop=FALSE;
		s_icon_landscape_crop=FALSE;
		s_icon_is_crop=FALSE;
//lijinghua add end
        s_is_decoded=FALSE;
    if(MMIAPIFMM_IsFileExist(L"D:/photos/PICCROP/temp_crop_file.jpg",MMIAPICOM_Wstrlen(L"D:/photos/PICCROP/temp_crop_file.jpg")))
    {
        MMIAPIFMM_DeleteFileSyn(L"D:/photos/PICCROP/temp_crop_file.jpg",MMIAPICOM_Wstrlen(L"D:/photos/PICCROP/temp_crop_file.jpg"));        
    }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
        
    return (result);
}
//lijinghua add it
PUBLIC uint16 PIC_CROP_GetCropIndex()
{
	return s_icon_crop_index;
}
PUBLIC uint16 PIC_SQUARE_CROP_GetCropIndex()
{
	return s_icon_square_crop_index;
}
PUBLIC uint16 PIC_LANDSCAPE_CROP_GetCropIndex()
{
	return s_icon_landscape_crop_index;
}
//lijinghua end
#endif



/*Edit by script, ignore 3 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
