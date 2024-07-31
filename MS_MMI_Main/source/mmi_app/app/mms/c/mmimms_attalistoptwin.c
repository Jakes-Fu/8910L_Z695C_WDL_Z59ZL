/****************************************************************************
** File Name:      mmimms_attalistoptwin.c                                     *
** Author:         arvin.zhang                                              *
** Date:           08/16/2015                                              *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME             DESCRIPTION                             *
** 08/2015    arvin.zhang               create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmimms_id.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmicc_export.h"
#include "mmk_timer.h"
#include "guires.h"
#include "guilcd.h"
#include "mmitheme_pos.h"

#include "mmimms_text.h"
#include "mms_image.h"
#include "mmimms_export.h"
#include "mmimms_internal.h"
#include "mmimms_net.h"
#include "mmicountedtime_export.h"
#include "mmialarm_export.h"
#include "mmi_default.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_menutable.h"
#include "mmisms_id.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void DrawAttachmentListItemWithPicture(
                                      uint16           item_index,     //in:item索引
                                      uint16           content_index,  //in:item内容索引,text or anim etc.
                                      MMI_CTRL_ID_T    ctrl_id         //in:control id                                     
                                      );

/*****************************************************************************/
//  Description :remove the current selected attachment
//  Global resource dependence : 
//  Author:arvin
//  Note: 
/*****************************************************************************/
LOCAL void  RemoveSelectedAttachment(void);
/*****************************************************************************/
//  Description :mms attachment list opt win handle fun
//  Global resource dependence : 
//  Author:arvin.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMessageAttachmentListOptWinMsg(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                              );

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
WINDOW_TABLE( MMIMMS_ATTA_OPT_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMessageAttachmentListOptWinMsg),
		WIN_ID(MMIMMS_ATTACHMENT_LIST_OPT_WIN_ID),
		WIN_STATUSBAR,
		CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMMS_MMS_ATTACHMENT_LIST_CTRL_ID),
		WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_REMOVE_EXT01, TXT_COMMON_COMMON_BACK),//should be modified to ms string id 'Remove'
		END_WIN
};

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description :remove the current selected attachment
//  Global resource dependence : 
//  Author:arvin
//  Note: 
/*****************************************************************************/
LOCAL void  RemoveSelectedAttachment(void)
{
    uint32 atta_type = 0;
    uint16 current_index = GUILIST_GetCurItemIndex(MMIMMS_MMS_ATTACHMENT_LIST_CTRL_ID);

    GUILIST_GetItemData(MMIMMS_MMS_ATTACHMENT_LIST_CTRL_ID, current_index, &atta_type);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void DrawAttachmentListItemWithPicture(
                                      uint16           item_index,     //in:item索引
                                      uint16           content_index,  //in:item内容索引,text or anim etc.
                                      MMI_CTRL_ID_T    ctrl_id         //in:control id
                                      )
{
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    GUIANIM_DATA_INFO_T     anim_data    = {0};      /*lint !e64*/
    uint32 atta_index_code = 0;
    uint32 slide_number = 0;
    uint32 accessory_index = 0;
    
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    P_MMS_SLIDE_T slide_ptr = PNULL;
    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    uint32 specific_file_type = 0;
    uint8 *multim_buf_ptr                  = PNULL;
    uint32 multim_buf_len                 = 0;
    uint8 is_accessory = 0;
    
    MMS_ACCESSORY_PARAM_T*	accessory_ptr = PNULL;	
    GUILIST_GetItemData(ctrl_id,item_index, &atta_index_code);
    slide_number = (atta_index_code>>8) & 0xff;
    accessory_index = (atta_index_code>>16) & 0xff;
    file_type = (atta_index_code & 0xf);
    is_accessory = (atta_index_code >> 24 ) & 0xf;    
    edit_doc_ptr = MMIMMS_GetAttachmentListEditDoc();//GetAttachmentListEditDoc();    
    
    if(MMIFMM_FILE_TYPE_PICTURE == file_type)    
    {  
        if(is_accessory)
        {
            accessory_ptr = MMIMMS_GetAccessoryPtrByIndex(edit_doc_ptr,accessory_index);
            sysVFGetConstFileData(accessory_ptr->filename,&multim_buf_ptr,(int32 *)&multim_buf_len);
        }
        else
        {
            slide_ptr = MMIMMS_GetSlidePtrByIndex(edit_doc_ptr,slide_number);
            MMIMMS_GetMultimediaBufferInSlide(MMIFMM_FILE_TYPE_PICTURE, &multim_buf_len, &multim_buf_ptr, slide_ptr);        
        }
        if (PNULL  == multim_buf_ptr)
        {
            return;
        }
    }    
    else
    {
        return;
    }
    item_content.item_data_type = GUIITEM_DATA_ANIM_DATA;
    anim_data.data_ptr = multim_buf_ptr;
    anim_data.data_size = multim_buf_len;
    item_content.item_data.anim_data_ptr = &anim_data;
    GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);           
}

LOCAL void HandleMMSAttachmentAllDelWindowInit( MMIPUB_MENU_NOTIFY_T *param )
{
    GUIMENU_BUTTON_INFO_T button_info = {0};
    MMI_MENU_GROUP_ID_T group_id = (MMI_MENU_GROUP_ID_T)MMK_GetWinUserData(param->win_id);
    button_info.is_static = TRUE;
    button_info.group_id = group_id;
    button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
    GUIMENU_SetButtonStyle(param->ctrl_id, &button_info);
    MMK_SetAtvCtrl(param->win_id, param->ctrl_id);
    GUIWIN_SetSoftkeyTextId(param->win_id,  PNULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK, FALSE);
    return ;
}

LOCAL void HandleMMSAttachmentAllDelWindowOK( MMIPUB_MENU_NOTIFY_T *param )
{
//    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 index = 0;
    MMI_MENU_GROUP_ID_T group_id= param->group_id;
    MMI_MENU_ID_T menu_id = param->menu_id;
    MMI_HANDLE_T win_handle = param->win_handle;
    MMIMMS_WINDOW_ID_E query_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;

    GUIMENU_GetId(param->ctrl_id, &group_id, &menu_id);
#ifdef 	MMIMMS_SMS_IN_1_SUPPORT    
    switch (menu_id)
    {        
    case MMISMS_MENU_ATTACH_ALL_DEL:
        SCI_TRACE_LOW("MMISMS_MENU_ATTACH_ALL_DEL");
//         MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_NOTE_REMOVE_ATTACHMENTS_EXT01,PNULL,PNULL,PNULL/*&query_win_id*/,PNULL,MMIPUB_SOFTKEY_OKCANCEL,PNULL);
        MMIPUB_OpenQueryWinByTextId(STR_NOTE_REMOVE_ATTACHMENT_EXT01,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        break;
            
    default:
        break;
    }
#endif    
    return;
}


/*****************************************************************************/
//  Description :mms attachment list opt win handle fun
//  Global resource dependence : 
//  Author:arvin.zhang
//  Note: 
/*****************************************************************************/
 LOCAL MMI_RESULT_E HandleMessageAttachmentListOptWinMsg(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                              )
{
    MMI_RESULT_E err_code                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl_id 			     = MMIMMS_MMS_ATTACHMENT_LIST_CTRL_ID;
    MMIMMS_INIT_ATTA_LIST_DATA_T *init_param = PNULL;
    ATTALISTINIT_CALLBACKFUNC init_callback_fun_ptr = PNULL;
    ATTALISTOPT_CALLBACKFUNC opt_callback_fun_ptr = PNULL;
    MMIMMS_ATTACHMENT_LIST_OPT_TYPE_E cur_opt_type =MMIMMS_ATTACHMENT_LIST_OPT_TYPE_MAX ;
    MMIMMS_WINDOW_ID_E query_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMIPUB_MENU_PARAM_T menuparam = {0};
    init_param = (MMIMMS_INIT_ATTA_LIST_DATA_T*)MMK_GetWinAddDataPtr(win_id);


//    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_ATTALISTOPTWIN_100_112_2_18_2_39_3_437,(uint8*)"d", msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        if(PNULL != init_param && PNULL != init_param->init_callback_fun_ptr)
        {
            cur_opt_type = init_param->opt_type;
            init_callback_fun_ptr = init_param->init_callback_fun_ptr;
            if(init_callback_fun_ptr)
            {
                init_callback_fun_ptr(list_ctrl_id);
            }
#if 0
            if(MMIMMS_ATTACHMENT_LIST_OPT_TYPE_REMOVE == cur_opt_type)
            {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_DELETEQUERY, TXT_NULL, FALSE);//should be modified to ms string id 'Remove'
            }
            else if(MMIMMS_ATTACHMENT_LIST_OPT_TYPE_VIEW == cur_opt_type)
            {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_OPEN_COMN, TXT_NULL, FALSE);//should be replaced to ms string id
            }
#endif

            GUILIST_SetOwnSofterKey(list_ctrl_id, TRUE);
            GUILIST_SetCurItemIndex(list_ctrl_id, 0);
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {            
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            uint16                      item_index;
            uint16                      item_content_index;
            if( PNULL != need_item_content_ptr )
            {
                item_index = need_item_content_ptr->item_index;
                item_content_index = need_item_content_ptr->item_content_index;
                DrawAttachmentListItemWithPicture(item_index,item_content_index,list_ctrl_id);
            }
        }
        break;  
        case MSG_PROMPTWIN_OK:
            if(PNULL != init_param && PNULL != init_param->opt_callback_fun_ptr &&
            (MMIMMS_ATTACHMENT_LIST_OPT_TYPE_REMOVE == init_param->opt_type))
            {
                //init_param->is_allitems = TRUE;
                //add confirm dialog later...
                (init_param->opt_callback_fun_ptr)(win_id, list_ctrl_id, init_param->is_allitems);
            }            
            MMK_CloseWin(query_win_id);
        break;
        
        case MSG_PROMPTWIN_CANCEL:
            MMK_CloseWin(query_win_id);
        break;
        
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        if(PNULL != init_param && PNULL != init_param->opt_callback_fun_ptr)
        {
        init_param->is_allitems = FALSE;
        //add confirm dialog later...
        //			(init_param->opt_callback_fun_ptr)(win_id,list_ctrl_id, FALSE);
        //MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_NOTE_REMOVE_ATTACHMENT_EXT01,PNULL,PNULL,PNULL/*&query_win_id*/,PNULL,MMIPUB_SOFTKEY_OKCANCEL,PNULL);
        MMIPUB_OpenQueryWinByTextId(STR_NOTE_REMOVE_ATTACHMENT_EXT01,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        }
        break;

        case MSG_APP_OK:
        case MSG_CTL_OK:
#if 0
        if(PNULL != init_param && PNULL != init_param->opt_callback_fun_ptr &&
        (MMIMMS_ATTACHMENT_LIST_OPT_TYPE_REMOVE == init_param->opt_type))
        {
        init_param->is_allitems = TRUE;
        //add confirm dialog later...
        //			(init_param->opt_callback_fun_ptr)(win_id, list_ctrl_id, TRUE);
        MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_NOTE_REMOVE_ATTACHMENTS_EXT01,PNULL,PNULL,&query_win_id,PNULL,MMIPUB_SOFTKEY_OKCANCEL,PNULL);
        //MMIPUB_OpenQueryWinByTextId(STR_NOTE_REMOVE_ATTACHMENTS_EXT01,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        }
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
      if(PNULL != init_param && PNULL != init_param->opt_callback_fun_ptr &&
        (MMIMMS_ATTACHMENT_LIST_OPT_TYPE_REMOVE == init_param->opt_type))
      {
            init_param->is_allitems = TRUE;
            menuparam.group_id = MENU_SMS_ATTACHMENT_ALL_DEL_OPT_MENU;
            menuparam.win_id =   MMISMS_ATTACH_ALL_DEL_WIN_ID;
            menuparam.ctrl_id =  MMISMS_SMS_ATTACH_ALL_DEL_CTL_ID;    
            menuparam.dst_handle = win_id;
            menuparam.func.OnMenuInit = HandleMMSAttachmentAllDelWindowInit;
            menuparam.func.OnMenuOk = HandleMMSAttachmentAllDelWindowOK;                    
            MMIPUB_OpenPubMenuWin( &menuparam ); 
        }
#endif      
        break;

        case MSG_FULL_PAINT:
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        
        case MSG_CLOSE_WINDOW:
            if (PNULL != init_param)   //488010 fixed init_param is always PNULL 
            {
                SCI_FREE(init_param);
                init_param = PNULL;
            }
        break;
        
        default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code;
}
/*****************************************************************************/
//  Description : Create the mms attachments list based on different operation type
//  Global resource dependence : none
//  Author: arvin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_CreateATTAListOptWin(
                                           ADD_DATA init_param
                                           )
{
    BOOLEAN result = FALSE;

    if(PNULL != init_param)
    {
        MMK_CreateWin((uint32*)MMIMMS_ATTA_OPT_LIST_WIN_TAB, (ADD_DATA)init_param);
        result = TRUE;
    }

    return result;
}
