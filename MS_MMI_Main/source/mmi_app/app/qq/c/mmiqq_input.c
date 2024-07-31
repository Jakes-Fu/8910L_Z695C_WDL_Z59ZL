/****************************************************************************
** File Name:      mmiqq_input.c(Qinput.c)                                                *
** Author:                                                                 *
** Date:           05/11/2010                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the input/edit of QQ             *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2010       fen.xie         Create
** 
****************************************************************************/
#include "mmi_app_qq_trc.h"
#ifdef QQ_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "qinput.h"
#include "qq2009.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "guiedit.h"
#include "mmitheme_pos.h"
#include "mmiqq_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIQQ_EDIT_NUMBER_MAX_LEN   11//number类型的QQ编辑框的最大输入长度

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIQQ_EDIT_WIN_INFO_T s_edit_win_info;//QQ中使用的edit窗口的信息

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: HandleEditBoxWinMsg
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditBoxWinMsg(MMIQQ_WINDOW_ID_E win_id, MMI_MESSAGE_ID_E msg_id, DPARAM	param);

WINDOW_TABLE(MMIQQ_EDITBOX_WIN) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_ALL),
    WIN_FUNC((uint32)HandleEditBoxWinMsg),    
    WIN_ID(MMIQQ_WIN_ID_EDITBOX),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_EDIT),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: HandleEditBoxWinMsg
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditBoxWinMsg(MMIQQ_WINDOW_ID_E win_id, MMI_MESSAGE_ID_E msg_id, DPARAM	param)
{
    MMI_RESULT_E			recode = MMI_RESULT_TRUE;
    MMI_STRING_T 			text = {0};
    MMI_HANDLE_T edit_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIQQ_CTRL_ID_EDITBOX);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        //注:editbox的参数设置顺序不可随意调换!!!
        switch(s_edit_win_info.type)
        {
        case MMIQQ_TM_NUMBER:
            GUIEDIT_SetMaxLen(edit_ctrl_handle, MMIQQ_EDIT_NUMBER_MAX_LEN);
            GUIEDIT_SetString(edit_ctrl_handle, s_edit_win_info.buffer, s_edit_win_info.buffer_wlen);
            break;
            
        case MMIQQ_TM_PSWORD:
            {
                BOOLEAN is_disp_im_icon = TRUE; 

                GUIEDIT_SetIm(edit_ctrl_handle, GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
                GUIEDIT_SetMaxLen(edit_ctrl_handle, s_edit_win_info.maxsize/sizeof(QU16));
                GUIEDIT_SetStyle(edit_ctrl_handle, GUIEDIT_STYLE_MULTI);//多行
                GUIEDIT_SetAlign(edit_ctrl_handle, ALIGN_LEFT);//靠左上起
                GUIEDIT_SetDispImIcon(edit_ctrl_handle, &is_disp_im_icon, PNULL);//显示输入法的icon
                GUIEDIT_SetString(edit_ctrl_handle, s_edit_win_info.buffer, s_edit_win_info.buffer_wlen);
            }
            break;
           
        //case MMIQQ_TM_ALL:
        default:
            GUIEDIT_SetIm(edit_ctrl_handle, GUIIM_TYPE_SET_ALL, GUIIM_TYPE_MULTITAP);
            GUIEDIT_SetTextMaxLen(edit_ctrl_handle, s_edit_win_info.maxsize/sizeof(QU16), s_edit_win_info.maxsize/sizeof(QU16));
            GUIEDIT_SetString(edit_ctrl_handle, s_edit_win_info.buffer, s_edit_win_info.buffer_wlen);
            break;
        }
        MMK_SetAtvCtrl(win_id, edit_ctrl_handle);
        break;
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        GUIEDIT_GetString(edit_ctrl_handle,  &text);
        
        SCI_MEMSET(s_edit_win_info.buffer, 0x00, sizeof(s_edit_win_info.buffer));
        if (text.wstr_len > 0)//copy data
        {
            s_edit_win_info.buffer_wlen = (uint16)MIN(QQ_EDIT_WIN_BUFFER_WLEN, text.wstr_len);
            SCI_MEMCPY(s_edit_win_info.buffer, text.wstr_ptr, s_edit_win_info.buffer_wlen*sizeof(QU16));
        }

        //@fen.xie MS00181596:当QQ被强迫下线时,不可再处理输入的数据,比免发生异常
        if ((MMIQQ_TM_NUMBER != s_edit_win_info.type) 
            && (MMIQQ_TM_PSWORD != s_edit_win_info.type) 
            && (MMIQQ_BUDDY_STAT_OFFLINE == MMIAPIQQ_GetLeaveQQState())) 
        {
            s_edit_win_info.handleLeftKey = PNULL;
        }
        
        if(s_edit_win_info.handleLeftKey != PNULL)//处理数据
        {
            s_edit_win_info.handleLeftKey(s_edit_win_info.pUser, s_edit_win_info.buffer);
        }
        MMK_CloseWin(win_id);	
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    case MSG_GET_FOCUS:
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description: 打开编辑窗口
//  Global resource dependence :  
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void QEntryEditWin(void)
{
    MMI_HANDLE_T win_handle = PNULL;
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    MMI_CTRL_ID_T edit_ctrl_id = MMIQQ_CTRL_ID_EDITBOX;
    GUIEDIT_INIT_DATA_T         edit_init = {0};
    MMI_CONTROL_CREATE_T        edit_ctrl = {0};
    GUIEDIT_TYPE_E          editbox_type = GUIEDIT_TYPE_NULL; 
    WINDOW_SUPPORT_ANGLE    support_angle = WIN_SUPPORT_ANGLE_0;
    
    switch(s_edit_win_info.type)
    {
    case MMIQQ_TM_NUMBER:
        editbox_type = GUIEDIT_TYPE_DIGITAL; 
        break;
        
    case MMIQQ_TM_PSWORD:
        editbox_type = GUIEDIT_TYPE_PASSWORD; 
        break;
        
    case MMIQQ_TM_ALL:
        editbox_type = GUIEDIT_TYPE_TEXT; 
        break;
        
    default:
        return;//Quit
    }
    
    //create edit window
    win_table_create.applet_handle = MMK_GetFirstAppletHandle();
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = MMIQQ_EDITBOX_WIN;

    win_handle = MMK_CreateWinTable(&win_table_create);

    //edit init data
    edit_init.both_rect = MMITHEME_GetWinClientBothRect(win_handle);
    edit_init.type        = editbox_type;

    //create edit control
    edit_ctrl.ctrl_id           = edit_ctrl_id;
    edit_ctrl.guid              = SPRD_GUI_EDITBOX_ID;
    edit_ctrl.init_data_ptr     = &edit_init;
    edit_ctrl.parent_win_handle = win_handle;
    MMK_CreateControl(&edit_ctrl);

    //init the QQ lcd angle
    switch (MMIQQ_GetLogicAngle())
    {
    case LCD_ANGLE_0:
        support_angle = WIN_SUPPORT_ANGLE_0;
        break;
    case LCD_ANGLE_90:
        support_angle = WIN_SUPPORT_ANGLE_90;
        break;
    case LCD_ANGLE_180:
        support_angle = WIN_SUPPORT_ANGLE_180;
        break;
    case LCD_ANGLE_270:
        support_angle = WIN_SUPPORT_ANGLE_270;
        break;
    default:
        support_angle = WIN_SUPPORT_ANGLE_0;
        break;
    }
    
    MMK_SetWinSupportAngle(win_handle, support_angle);
    MMK_SetWinAngle(win_handle, MMIQQ_GetLogicAngle(), FALSE);

}


/*****************************************************************************/
//  Description: 初始化编辑窗口
//  Global resource dependence :  s_edit_win_info
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void QInitEditWin(QU16* title, QU16* buffer, QU16 maxsize, QU16 type, void* pUser, pfnLeft handleLKey)
{
    //SCI_TRACE_LOW:"[MMIQQ]QInitEditWin:maxsize = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_INPUT_237_112_2_18_2_49_7_9,(uint8*)"d",maxsize);

    SCI_MEMSET(&s_edit_win_info, 0, sizeof(MMIQQ_EDIT_WIN_INFO_T));

#if 0
    if(title != PNULL)
    {
        s_edit_win_info.title_wlen = (uint16)MIN(QQ_EDIT_WIN_TITLE_WLEN, MMIAPICOM_Wstrlen(title));
        if (s_edit_win_info.title_wlen > 0)
        {
            SCI_MEMCPY(s_edit_win_info.title, title, s_edit_win_info.title_wlen*sizeof(QU16));
        }
    }
#endif
    
    if(buffer != PNULL)
    {
        s_edit_win_info.buffer_wlen = MMIAPICOM_Wstrlen(buffer);
        s_edit_win_info.buffer_wlen = (uint16)MIN(QQ_EDIT_WIN_BUFFER_WLEN, s_edit_win_info.buffer_wlen);
        if (s_edit_win_info.buffer_wlen > 0)
        {
            SCI_MEMCPY(s_edit_win_info.buffer, buffer, s_edit_win_info.buffer_wlen*sizeof(QU16));
        }
    }
    
    s_edit_win_info.type = type;
    s_edit_win_info.maxsize = (uint16)MIN(QQ_EDIT_WIN_BUFFER_WLEN*sizeof(QU16), maxsize);
    s_edit_win_info.pUser = pUser;
    s_edit_win_info.handleLeftKey = handleLKey;
}
#endif 


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
