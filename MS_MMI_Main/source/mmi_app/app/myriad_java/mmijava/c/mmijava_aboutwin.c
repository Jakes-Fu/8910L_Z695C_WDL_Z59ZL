/**
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_aboutwin.c#1 $
 */

/**
 * @file
 * implemented relevant about information for the 6800h build of FBCC
 */
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_common.h"
#include "guitext.h"
#include "java_mmi_text.h"
#include "mmijava_id.h"


/*****************************************************************************/
//  Description : Jbed_HandleAboutWinMsg.
//  Global resource dependence : none
//  Author: martin zhao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleAboutWinMsg
(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(JBED_ABOUT_MENU_WIN_TAB) =
{
    WIN_TITLE(TXT_JAVA_ABOUT),
    WIN_FUNC((uint32)Jbed_HandleAboutWinMsg),
    WIN_ID(JBED_ABOUT_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIJAVA_ABOUT_CRTL_ID),
    END_WIN
};

/*****************************************************************************/
//  Description : Handle about window message.
//  Global resource dependence : none
//  Author: martin zhao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleAboutWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_STRING_T    string0 = {0};
    MMI_STRING_T    string1 = {0};
    MMI_STRING_T    string2 = {0};
    MMI_STRING_T    string3 = {0};
    MMI_STRING_T    string4 = {0};
    MMI_STRING_T    string5 = {0};
    MMI_STRING_T    string6 = {0};
    uint16          u16lf[] = {0x0A};
    uint32          u16lf_len = 0;
    uint32          len = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIJAVA_ABOUT_CRTL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMI_GetLabelTextByLang(TXT_JAVA_ABOUT_TXT_LINE_1, &string0);
            MMI_GetLabelTextByLang(TXT_JAVA_ABOUT_TXT_LINE_2, &string1);
            MMI_GetLabelTextByLang(TXT_JAVA_ABOUT_TXT_LINE_3, &string2);
            MMI_GetLabelTextByLang(TXT_JAVA_ABOUT_TXT_LINE_4, &string3);
            MMI_GetLabelTextByLang(TXT_JAVA_ABOUT_TXT_LINE_5, &string4);
            MMI_GetLabelTextByLang(TXT_JAVA_ABOUT_TXT_LINE_6, &string5);
            u16lf_len = 1;
            string6.wstr_len = string0.wstr_len + string1.wstr_len + string2.wstr_len + string3.wstr_len
                               + string4.wstr_len + string5.wstr_len + string5.wstr_len + (u16lf_len+1)*6;
            string6.wstr_ptr = (wchar *)SCI_ALLOCA(string6.wstr_len*sizeof(wchar));
            if (PNULL == string6.wstr_ptr)
            {
              break;
            }

            SCI_MEMSET(string6.wstr_ptr, 0x00, (string6.wstr_len*sizeof(wchar)));
            MMIAPICOM_Wstrncpy(string6.wstr_ptr,string0.wstr_ptr,string0.wstr_len);
            len += string0.wstr_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,u16lf,u16lf_len);
            len += u16lf_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,string1.wstr_ptr,string1.wstr_len);
            len += string1.wstr_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,u16lf,u16lf_len);
            len += u16lf_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,string2.wstr_ptr,string2.wstr_len);
            len += string2.wstr_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,u16lf,u16lf_len);
            len += u16lf_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,string3.wstr_ptr,string3.wstr_len);
            len += string3.wstr_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,u16lf,u16lf_len);
            len += u16lf_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,string4.wstr_ptr,string4.wstr_len);
            len += string4.wstr_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,u16lf,u16lf_len);
            len += u16lf_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,string5.wstr_ptr,string5.wstr_len);
            len += string5.wstr_len;
            MMIAPICOM_Wstrncpy(string6.wstr_ptr+len,u16lf,u16lf_len);
            string6.wstr_len = len;
            (void)GUITEXT_SetString(ctrl_id, string6.wstr_ptr, string6.wstr_len, FALSE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            SCI_FREE(string6.wstr_ptr);
            string6.wstr_ptr = PNULL;
            break;

        case MSG_KEYLONG_POWER:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Open about content window
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void  MMIJAVA_OpenAboutWin(void)
{
   MMK_CreateWin((uint32*)JBED_ABOUT_MENU_WIN_TAB,PNULL);
}

