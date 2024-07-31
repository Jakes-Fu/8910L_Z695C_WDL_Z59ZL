/**
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_dlwin.c#2 $
 */

/**
 * @file
 * implemented relevant install from network window for the 6800h build of FBCC
 */
#include <window_parse.h>
//#include <guirichtext.h>
#include "guiform.h"
#include "guilabel.h"
#include "mmipub.h"
#ifdef BROWSER_SUPPORT
#include <mmibrowser_export.h>
#endif
#include "mmijava_id.h"
#include "java_mmi_image.h"
#include "mmijava_dlwin.h"
#include "mmijava_nv.h"
#include "guires.h"


#ifdef  SUPPORT_DOWNLOAD
//Default download url.
LOCAL    const char* s_default_download_url = "http://wap.baidu.com";

/**************************************************************************************************/
// Description:Handle download window message.
// Author:Martin.Zhao
// Date:2010.11.22
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaDownLoadSetWinMsg(
                                       MMI_WIN_ID_T      win_id,
                                       MMI_MESSAGE_ID_E  msg_id,
                                       DPARAM            param
                                       );

/**************************************************************************************************/
// Description:Download window table.
// Author:Martin.Zhao
// Date:2010.11.22
// Note:
//*************************************************************************************************/
WINDOW_TABLE( JBED_DOWNLOAD_SET_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleJavaDownLoadSetWinMsg),
    WIN_ID( MMIJAVA_DOWNLOAD_SET_WIN_ID ),
    WIN_TITLE(TXT_DOWNLOAD_SETTINGS),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_DOWNLOAD_SET_FORM_ID),
        //GAME
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_DOWNLOAD_SET_GAME_LABEL_ID,MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIJAVA_MAX_DOWNLOAD_URL_LEN,MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID,MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_DOWNLOAD_ENTER,MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID,MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID),
            CHILD_OWNDRAW_CTRL(FALSE,MMIJAVA_DOWNLOAD_SET_GAME_ENTER_SPACE_ID,MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID,PNULL),
        //EBOOK
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_DOWNLOAD_SET_EBOOK_LABEL_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIJAVA_MAX_DOWNLOAD_URL_LEN,MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_DOWNLOAD_ENTER,MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID),
            CHILD_OWNDRAW_CTRL(FALSE,MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_SPACE_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID,PNULL),
        //OTHER
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_DOWNLOAD_SET_OTHER_LABEL_ID,MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIJAVA_MAX_DOWNLOAD_URL_LEN,MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID,MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_DOWNLOAD_ENTER,MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID,MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID),
            CHILD_OWNDRAW_CTRL(FALSE,MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_SPACE_ID,MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID,PNULL),
#ifdef MMI_PDA_SUPPORT
        CHILD_SOFTKEY_CTRL(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};



/*****************************************************************************/
//  Description :Set parameter for download form.
//  Global resource dependence : none
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL void SetJavaDownLoadFormParam(MMI_WIN_ID_T  win_id)
{
    GUIFORM_CHILD_WIDTH_T           width = {0};
    uint16                          image_width = 0;
    uint16                          image_height = 0;
    MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MN_RETURN_RESULT_E              nv_result = MN_RETURN_FAILURE;
    wchar                           wchar_url[MMIJAVA_MAX_DOWNLOAD_URL_LEN+1] = {0};
    GUIIM_TYPE_T                    allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T                    init_im = GUIIM_TYPE_NONE;

    GUIFORM_SetStyle(MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID,GUIFORM_STYLE_UNIT);

    //botton rect
    if (!GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_DOWNLOAD_ENTER,win_id))
    {
        SCI_TRACE_LOW("call GUIRES_GetImgWidthHeight failed");
    }
    width.type = GUIFORM_CHILD_WIDTH_FIXED;
    width.add_data = image_width+4;//info add 4,because of botton border

    GUIFORM_SetChildWidth(MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID,MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID,&width);
    GUIFORM_SetChildWidth(MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID,&width);
    GUIFORM_SetChildWidth(MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID,MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID,&width);

    GUILABEL_SetTextById(MMIJAVA_DOWNLOAD_SET_GAME_LABEL_ID,TXT_JAVA_DOWNLOAD_GAME_URL,FALSE);
    GUILABEL_SetTextById(MMIJAVA_DOWNLOAD_SET_EBOOK_LABEL_ID,TXT_JAVA_DOWNLOAD_EBOOK_URL,FALSE);
    GUILABEL_SetTextById(MMIJAVA_DOWNLOAD_SET_OTHER_LABEL_ID,TXT_JAVA_DOWNLOAD_OTHER_URL,FALSE);

    GUIEDIT_SetFont(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID,MMI_DEFAULT_NORMAL_FONT);
    GUIEDIT_SetFont(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID,MMI_DEFAULT_NORMAL_FONT);
    GUIEDIT_SetFont(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID,MMI_DEFAULT_NORMAL_FONT);

    //read url from mv
    MMINV_READ(JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
    if(download_detail.game_info.url_len == 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)s_default_download_url, wchar_url);
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, wchar_url, MMIJAVA_DOWNLOAD_DEFAULT_URL_LEN);
    }
    else if(download_detail.game_info.url_len > 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)download_detail.game_info.url, wchar_url);
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, wchar_url, download_detail.game_info.url_len);
    }

    if(download_detail.ebook_info.url_len == 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)s_default_download_url, wchar_url);
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, wchar_url, MMIJAVA_DOWNLOAD_DEFAULT_URL_LEN);
    }
    else if(download_detail.ebook_info.url_len > 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)download_detail.ebook_info.url, wchar_url);
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, wchar_url, download_detail.ebook_info.url_len);
    }

    if(download_detail.other_info.url_len == 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)s_default_download_url, wchar_url);
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, wchar_url, MMIJAVA_DOWNLOAD_DEFAULT_URL_LEN);
    }
    else if(download_detail.other_info.url_len > 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)download_detail.other_info.url, wchar_url);
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, wchar_url, download_detail.other_info.url_len);
    }

    //url text edit im
    allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;
    init_im = GUIIM_TYPE_ABC;

    GUIEDIT_SetIm(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, allow_im, init_im);
    GUIEDIT_SetIm(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, allow_im, init_im);
    GUIEDIT_SetIm(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, allow_im, init_im);

    //set active ctrl
    MMK_SetAtvCtrl(win_id, MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID);
}

/*****************************************************************************/
//  Description :Handle download setting info change.
//  Global resource dependence : none
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IfJavaDownLoadSetInfoChanged(void)
{
    MMI_RESULT_E                    result = MMI_RESULT_FALSE;
    MN_RETURN_RESULT_E              return_value = MN_RETURN_FAILURE;
    char                            char_url[MMIJAVA_MAX_DOWNLOAD_URL_LEN+1] = {0};
    MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MMI_STRING_T                    url_wstr = {0};

    MMINV_READ(JAVA_DOWNLOAD_INFO, &download_detail, return_value);
    if (MN_RETURN_SUCCESS != return_value)
        return  (MMI_RESULT_TRUE);

    GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, &url_wstr);
    SCI_MEMSET(char_url,0,sizeof(char_url));
    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);

    if(0 != MMIAPICOM_Stricmp(download_detail.game_info.url, (const uint8*)char_url))
    {
        return MMI_RESULT_TRUE;
    }

    GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, &url_wstr);
    SCI_MEMSET(char_url,0,sizeof(char_url));
    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
    if(0 != MMIAPICOM_Stricmp(download_detail.ebook_info.url, (const uint8*)char_url))
    {
        return MMI_RESULT_TRUE;
    }

    GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, &url_wstr);
    SCI_MEMSET(char_url,0,sizeof(char_url));
    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
    if(0 != MMIAPICOM_Stricmp(download_detail.other_info.url,(const uint8*)char_url))
    {
        return  MMI_RESULT_TRUE;
    }

    return (result);
}
/**************************************************************************************************/
// Description:Handle download window message.
// Author:Martin.Zhao
// Date:2010.11.22
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaDownLoadSetWinMsg(
                                       MMI_WIN_ID_T      win_id,
                                       MMI_MESSAGE_ID_E  msg_id,
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E                    result = MMI_RESULT_TRUE;
    MMI_STRING_T                    url_wstr = {0};
    char                            char_url[MMIJAVA_MAX_DOWNLOAD_URL_LEN+1] = {0};
    MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MMIBROWSER_ENTRY_PARAM          brw_parm = {0};
    MMI_CTRL_ID_T                   active_id = 0;
    uint16                          min_url_len = 0;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            SetJavaDownLoadFormParam(win_id);
            break;

        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
            active_id = MMK_GetActiveCtrlId(win_id);
            if((MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID == active_id)
            ||(MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID == active_id)
            ||(MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID == active_id))
            {
                GUIEDIT_GetString(active_id - 2, &url_wstr);//-2 for get input url control id
                if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
                {
                    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                    brw_parm.type = MMIBROWSER_ACCESS_URL;
                    brw_parm.dual_sys = MN_DUAL_SYS_MAX;
                    brw_parm.is_connection_customized = FALSE;
                    brw_parm.url_ptr = char_url;
                  MMIAPIBROWSER_Entry(&brw_parm);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
                }
            }
            else
            {
                BOOLEAN isInvalidUrl = TRUE;
                GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, &url_wstr);
                if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
                {
                    min_url_len = MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN);
                    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                    SCI_MEMCPY(download_detail.game_info.url,char_url,min_url_len);
                    download_detail.game_info.url_len = min_url_len;
                    isInvalidUrl = FALSE;
                }

                GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, &url_wstr);
                if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
                {
                    SCI_MEMSET(char_url,0,sizeof(char_url));
                    min_url_len = MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN);
                    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                    SCI_MEMCPY(download_detail.ebook_info.url,char_url,min_url_len);
                    download_detail.ebook_info.url_len = min_url_len;
                    isInvalidUrl = FALSE;
                }

                GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, &url_wstr);
                if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
                {
                    SCI_MEMSET(char_url,0,sizeof(char_url));
                    min_url_len = MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN);
                    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                    SCI_MEMCPY(download_detail.other_info.url,char_url,min_url_len);
                    download_detail.other_info.url_len = min_url_len;
                    isInvalidUrl = FALSE;
                }
                if(isInvalidUrl)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
                    break;
                }

                MMINV_WRITE(JAVA_DOWNLOAD_INFO, &download_detail);
                MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                MMK_CloseWin(win_id);
            }
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
            active_id = ((MMI_NOTIFY_T*)param)->src_id ;
            if((MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID == active_id)
                ||(MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID == active_id)
                ||(MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID == active_id))
            {
                GUIEDIT_GetString(active_id - 2, &url_wstr);//-2 for get input url control id
                if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
                {
                    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                    brw_parm.type = MMIBROWSER_ACCESS_URL;
                    brw_parm.dual_sys = MN_DUAL_SYS_MAX;
                    brw_parm.is_connection_customized = FALSE;
                    brw_parm.url_ptr = char_url;
                    MMIAPIBROWSER_Entry(&brw_parm);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
                }
            }
#ifdef MMI_PDA_SUPPORT
            switch(active_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            if(IfJavaDownLoadSetInfoChanged())
            {
                MMIPUB_OpenQueryWinByTextId(STXT_EXIT, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
            }
            else
            {
                MMK_CloseWin(win_id);
            }
            break;

        case MSG_PROMPTWIN_OK:
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;

       case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
        }

        return (result);
}

/**************************************************************************************************/
// Description:Download entry for called in other module.
// Author:Martin.Zhao
// Date:2010.11.22
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_EnterDownload(int32 url_type)
{ 
    //MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MN_RETURN_RESULT_E              nv_result = MN_RETURN_FAILURE;
    MMIBROWSER_ENTRY_PARAM          brw_parm = {0};
    MMIJAVA_DOWNLOAD_ITEM_DETAIL_T  item_detail = {0};
	MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};

    SCI_TRACE_LOW("HandleJavaDownLoadEnter Enter type =%d",url_type);

    MMINV_READ(JAVA_DOWNLOAD_INFO, &download_detail, nv_result);

    switch(url_type)
    {
        case JAVA_DOWNLOAD_ID_GAME:
            MMINV_READ(JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
            item_detail = download_detail.game_info;
            break;

        case JAVA_DOWNLOAD_ID_EBOOK:
            MMINV_READ(JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
            item_detail = download_detail.ebook_info;
            break;

        case JAVA_DOWNLOAD_ID_OTHER:
            MMINV_READ(JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
            item_detail = download_detail.other_info;
            break;

        default:
            break;
    }

    if(item_detail.url_len == 0)
    {
        item_detail.url_len = MMIJAVA_DOWNLOAD_DEFAULT_URL_LEN;
        SCI_MEMCPY(item_detail.url,s_default_download_url,MMIJAVA_DOWNLOAD_DEFAULT_URL_LEN+1);
    }

    //brw param set
    brw_parm.url_ptr = (char *)item_detail.url;
    brw_parm.type = MMIBROWSER_ACCESS_URL;
    brw_parm.dual_sys = MN_DUAL_SYS_MAX;
    brw_parm.is_connection_customized = FALSE;

    MMIAPIBROWSER_Entry(&brw_parm);
    SCI_TRACE_LOW("HandleJavaDownLoadEnter exit");
}
/**************************************************************************************************
  * Description: MMIJAVA_EnterMidletListWin
  * Input Paras:
  * Output Paras: 
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_EnterDownLoadSettingWin(uint32 suiteid)
{
	SCI_TRACE_LOW("MMIJAVA_EnterDownLoadSettingWin suiteid = %d",suiteid);
    MMK_CreateWin((uint32*)JBED_DOWNLOAD_SET_WIN_TAB, (ADD_DATA)suiteid);
}

#endif //SUPPORT_DOWNLOAD


