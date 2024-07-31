/*****************************************************************************
** File Name:      mmiim_t9.c                                                *
** Author:         haiwu.chen                                                *
** Date:           24/04/2012                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to process t9 text input                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2012        haiwu.chen            Create                               *
** only used for latin language                                              *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_im_trc.h"
#include "guiim_base.h"
#include "mmitheme_pos.h"
#include "mmiim.h"
#include "mmiim_internal.h"
#include "mmk_msg.h"
#include "mmi_custom_define.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "spml_api.h"
#include "mmiim_im_switching.h"
#include "mmiim_sp_symbol.h"
#include "mmi_default.h"
#include "mmiim_t9_base.h"
#include "mmiim_t9.h"
#include "mmiim_id.h"
#include "mmiim_text.h"
#include "mmiim_base.h"
#include "mmiim_t9_func.h"
#include "window_parse.h"
#include "mmipub.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIIM_SPELL_WORD_GRID_HEIGHT        (5)
#define MMIIM_SPELL_WORD_DIALOG_HEIGHT      (25)    //editbox height
#define MMIIM_SPELL_EDIT_BOX_LEFT           (0)
#define MMIIM_SPELL_EDIT_BOX_RIGHT          (MMI_MAINSCREEN_WIDTH-1)
#define MMIIM_SPELL_EDIT_BOX_TOP            (MMI_CLIENT_RECT_BOTTOM-MMIIM_SPELL_WORD_DIALOG_HEIGHT+1)
#define MMIIM_SPELL_EDIT_BOX_BOTTOM         (MMI_CLIENT_RECT_BOTTOM)

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL GUIIM_EVENT_DATA_U s_last_eventdata_autoswitch = {0};
LOCAL BOOLEAN s_is_autoswitch_simu_sych = FALSE;
static BOOLEAN s_is_im_reload = FALSE;
static uint8 s_im_reload_cnt = 0;
static MMIIM_T9_CONVERSION_TABLE_DEF_T *s_conversion_table_ptr = PNULL;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/******************************************************************************/
// Description: Initialize
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr);

/******************************************************************************/
// Description: Terminate
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN Terminate(void *handle);

/******************************************************************************/
// Description: HandleMsg
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleMsg(
    void *handle, 
    GUIIM_EVENT_T event, 
    GUIIM_EVENT_DATA_U *event_data_ptr
);

/******************************************************************************/
// Description: 初始化输入法相关的数据
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL void InitData(
    GUIIM_INIT_KERNEL_T *param_ptr,
    MMIIM_T9_DATA_T *data_ptr
);

/******************************************************************************/
// Description: 初始化内核
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL T9STATUS InitImKernel(
    MMIIM_T9_DATA_T *data_ptr
);

/******************************************************************************/
// Description: SetMethod
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN SetMethod(void *handle, GUIIM_METHOD_T method);

/******************************************************************************/
// Description: TransMsg
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void TransMsg(
    MMIIM_T9_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_MSG_T *msg_ptr
);

/******************************************************************************/
// Description: HandleSysMsgNoInput
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_T9_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/******************************************************************************/
// Description: HandleSysMsgInputing
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_T9_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*****************************************************************************/
//  Description : handle alphabtic multitap char msg of no input state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgMultitap(
    MMIIM_T9_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
);

/*****************************************************************************/
//  Description : set to no input state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToNoInput(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : set to samsung inputing state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToInputing(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : sychronization t9 engine content and curson with editbox 
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL T9STATUS SychT9WithEdit(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : auto switch capital from lead to lower
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AutoSwitchCapital(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : auto switch method from lead to lower
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AutoSwitchMethod(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : Commit char
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void CommitChar(MMIIM_T9_DATA_T *data_ptr, wchar c);

/*****************************************************************************/
//  Description : Commit multitap Last Key value
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 注意本函数只用于multitap输入法,类似abc输入法提交的字母
/*****************************************************************************/
LOCAL BOOLEAN CommitMulLastKey(MMIIM_T9_DATA_T *data_ptr, BOOLEAN *is_commit);

/*****************************************************************************/
//  Description : set the cursor position from editbox to IME
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL T9STATUS SetIMECursorPos(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle press left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLeftKeyMsg(
    MMIIM_T9_DATA_T *data_ptr,
    MMIIM_MOVE_BY_TYPE move_type
);

/*****************************************************************************/
//  Description : handle press right key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleRightKeyMsg(
    MMIIM_T9_DATA_T *data_ptr,
    MMIIM_MOVE_BY_TYPE move_type
);

/*****************************************************************************/
//  Description : handle long left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLongLeftKeyMsg(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle long left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLongRightKeyMsg(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle long number(0~9) key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLongNumberKeyMsg(
    MMIIM_T9_DATA_T *data_ptr,
    MMIIM_SP_VKEY_T key
);

/*****************************************************************************/
//  Description : handle cancel key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleCancelKeyMsg(MMIIM_T9_DATA_T *data_ptr);


/*****************************************************************************/
//  Description : create add new word window
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateAddNewWordWin(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
// Description : HandleAddNewWordWinMsg
// Global resource dependence :
// Author: haiwu.chen
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddNewWordWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
// Description : SendAddWordContent
// Global resource dependence :
// Author: haiwu.chen
// Note:
/*****************************************************************************/
LOCAL void SendAddWordContent(
    MMIIM_T9_ADD_WORD_T *t9_addword_ptr,
    wchar *pBuf,
    uint16 nBufLen
);

/*****************************************************************************/
//  Description : get im reload count
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL uint8 GetImReloadCnt(void);

/*****************************************************************************/
//  Description : set im reload count
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetImReloadCnt(uint8 im_reload_cnt);

/*****************************************************************************/
//  Description : plus reload count
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AddImReloadRefCnt(void);

/*****************************************************************************/
//  Description : minus reload count
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void ReleaseImReloadRefCnt(void);

/*****************************************************************************/
//  Description : is im reload happan
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsImReloadDirty(void);

/*****************************************************************************/
//  Description : set reload flag
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetImReloadDirty(BOOLEAN is_dirty);

/*****************************************************************************/
//  Description : re-entry input method
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReEntryIm(MMIIM_T9_DATA_T *data_ptr);
/*****************************************************************************/
//  Description : Get all predicted words
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPredictWord(MMIIM_T9_DATA_T *data_ptr, MMIIM_SP_VKEY_T key,uint16 nIdx);

/******************************************************************************/
// Description: reset to no input state and clean the underline string if needed
// Global resource dependence: NONE
// Author: arvin.zhang
// Note: called under the scenes : LOSE_FOCUS,HASH_KEY(to switch im)
/******************************************************************************/
LOCAL void ResetToNoInput(MMIIM_T9_DATA_T *data_ptr , BOOLEAN is_need_clear_underline);

/*****************************************************************************
* Function name : UnicodeToGSM
* Description : Convert Unicode character code to GSM supported
* character code
*
* Return : Unicode encoded GSM character
*
* Pass : T9SYMB sUnicode -- Code to be translated
*****************************************************************************/
T9U16 UnicodeToGSM(T9SYMB sUnicode);

/*------------------------------------------------------------------------
* Function name : T9ConvertChar
* Description :   T9 sends a new character to the host to allow the
*                 host to reject it or convert to a different character.
*
* Pass :  T9FieldInfo *pFieldInfo
*         T9SYMB *psConvertChar - character to be converted
*
* Return: T9STATNONE if successful (symbol is okay or it has been replaced)
*         T9STAT_NO_CHAR if symbol cannot be displayed or replaced by another
*         T9STATERROR if another error prevents character conversion
*-----------------------------------------------------------------------*/
T9STATUS T9FARCALL T9ConvertChar(T9FieldInfo *pFieldInfo, T9SYMB *psConvertChar);

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
GUIIM_KERNEL_APIS_T const g_guiim_t9_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
};

WINDOW_TABLE(MMIIM_ADD_WORD_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleAddNewWordWinMsg),
    WIN_ID(MMIIM_ADD_WORD_WIN_ID),
//    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_TITLE(STXT_IM_ADD_WORD),
    CREATE_EDIT_TEXT_CTRL(0, MMIIM_ADD_WORD_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(STXT_SOFTKEY_ADD_MK, TXT_NULL, STXT_RETURN),
    END_WIN
};


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINITION                        */
/*---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: Initialize
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void* Initialize(GUIIM_INIT_KERNEL_T *param_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    MMIIM_T9_DATA_T *data_ptr = PNULL;

    SCI_TRACE_LOW("[MMIIM] Initialize.");
    
    if(PNULL == param_ptr)
    {
        return data_ptr;
    }
    
    data_ptr = (MMIIM_T9_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_T9_DATA_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_T9_DATA_T));

    SCI_TRACE_LOW("[MMIIM] Initialize T9: win_id=%d. s_t9_ldb_max_size=%d", param_ptr->win_id,MMIIM_T9GetMaxLdbSize());
    //data
    InitData(param_ptr, data_ptr);

    //kernel
    sStatus = InitImKernel(data_ptr);

    if (T9STATNONE != sStatus)
    {
        SCI_TRACE_LOW("MMIIM Initialize InitT9Core fail!");
        MMIIM_T9_Ter_Alpha(data_ptr);
        SCI_FREE(data_ptr);
        return PNULL;
    }
    else
    {
        SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_set.method);
        AddImReloadRefCnt();
		/* Turn on Character Conversion */
		sStatus = T9SetCharConvert(&data_ptr->aw_field_info.G, T9ConvertChar);
		SCI_TRACE_LOW("[MMIIM] Initialize T9: T9SetCharConvert result is %d.", (T9STATUS)sStatus);
    }
    
    return data_ptr;
}

/******************************************************************************/
// Description: Terminate
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN Terminate(void *handle)
{
    MMIIM_T9_DATA_T *data_ptr = (MMIIM_T9_DATA_T*)handle;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] Terminate.");
        return FALSE;
    }
    
    SCI_TRACE_LOW("[MMIIM] Terminate, method=%04X", 
        data_ptr->init_param.method_param_ptr->im_set.method);

    MMIIM_T9_Ter_Alpha(data_ptr);
	MMIIM_SetImKerState(0);
#ifdef UI_MULTILAYER_SUPPORT
   MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#endif
    SCI_FREE(data_ptr);
    data_ptr = PNULL;
    ReleaseImReloadRefCnt();
    if (0 != GetImReloadCnt())
    {
        SetImReloadDirty(TRUE);
    }
#ifndef UI_MULTILAYER_SUPPORT
   MMK_DelayUpdateScreen();
#endif
    return TRUE;
}

/******************************************************************************/
// Description: ChangeRect
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN ChangeRect(MMIIM_T9_DATA_T *data_ptr)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUI_RECT_T rect_null = {0};

    if (PNULL == data_ptr )
    {
        return FALSE;
    }
    
    if (data_ptr->is_rect_change)
    {
        switch (data_ptr->state)
        {
        case MMIIM_SP_STATE_NOINPUT:
            data_ptr->rect = rect_null;
            break;

        case MMIIM_SP_STATE_INPUTING:
        case MMIIM_SP_STATE_SELECTING:
        case MMIIM_SP_STATE_ASSOCIATE:
            data_ptr->rect = data_ptr->predict_bar.rect;
#ifndef UI_MULTILAYER_SUPPORT
            MMK_UpdateScreen();
#endif
            break;
            
        default:
            break;
        }

        notify_pack.notify = GUIIM_NOTIFY_CHANGE_RECT;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    }

    return data_ptr->is_rect_change;
}

/******************************************************************************/
// Description: ConfigCandBarPosition
// Global resource dependence: NONE
// Author: cheney.wu
// Note:
/******************************************************************************/
PUBLIC void MMIIM_UpdatePredictBarPosition(MMIIM_T9_DATA_T *data_ptr, BOOLEAN is_need_border)
{
    uint16 start_idx = 0;
    uint16 end_idx = 0;
    uint16 seg_idx = 0;
    uint16 textbar_width = 0;
    uint16 textbar_height = 0;
    GUI_FONT_T font = 0;
    GUI_RECT_T cursor_rect = {0};
    MMI_STRING_T text_info = {0};
    MMIIM_CAND_WIN_T *cand_info_ptr = PNULL;
    GUI_RECT_T old_prdictbar_rect = {0};

    if (PNULL == data_ptr )
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_UpdatePredictBarPosition: data_ptr is PNULL.");
        return;
    }
	
    cand_info_ptr = &data_ptr->cand_win;

    if (PNULL == cand_info_ptr )
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_UpdatePredictBarPosition: cand_info_ptr is PNULL.");
        return;
    }
	
    start_idx = ((cand_info_ptr->cur_sel-1)/MMIIM_T9_CC_SELECTPAGE_NUM)*MMIIM_T9_CC_SELECTPAGE_NUM;
    end_idx = MIN((start_idx+MMIIM_T9_CC_SELECTPAGE_NUM-1), (cand_info_ptr->count));

    // calculate the width and height of the textbar
    font = (is_need_border ? g_mmiim_ui_config.font_candidate : g_mmiim_ui_config.font_default);
    for (seg_idx = start_idx; seg_idx <= end_idx; seg_idx++)
    {
        text_info.wstr_ptr = cand_info_ptr->buffer[seg_idx];
        text_info.wstr_len = MMIAPICOM_Wstrlen(text_info.wstr_ptr);
        textbar_width = MAX(textbar_width, GUI_GetStringWidth(font, text_info.wstr_ptr, text_info.wstr_len));
    }

    textbar_width = MAX(textbar_width, MMIIM_TEXT_POPUP_WIDTH) + 2*MMIIM_TEXT_EDIT_BAR_SPACE;
    textbar_height = MIN(MMIIM_T9_CC_SELECTPAGE_NUM, (end_idx - start_idx + 1)) * (MMIIM_TEXT_BAR_EDIT_HEIGHT + 2*MMIIM_TEXT_BAR_BORDER_WIDTH);
    if (is_need_border)
    {
        textbar_width += 2*MMIIM_TEXT_BAR_BORDER_WIDTH;
        textbar_height -= (end_idx - start_idx)*2*MMIIM_TEXT_BAR_BORDER_WIDTH;//only top&bottom border needed if border exists
    }

    old_prdictbar_rect = data_ptr->predict_bar.rect;

    GUICTRL_SendNotifyEx(data_ptr->ctrl_handle, MSG_NOTIFY_IM_GET_CURSOR_RECT, &cursor_rect);

    if (MMI_DIRECTION_LEFT == data_ptr->dis_direct)
    {
        if(cursor_rect.left >= (MMI_MAINSCREEN_WIDTH - textbar_width))
        {
            data_ptr->predict_bar.rect.left = MMI_MAINSCREEN_WIDTH - textbar_width;
        }
        else
        {
            data_ptr->predict_bar.rect.left = (int16)(cursor_rect.left);//MAX(cursor_rect.left, MMIIM_TEXT_EDIT_BAR_SPACE - 1) y>=5 ---wait for bugfix
        }		
        data_ptr->predict_bar.rect.right = data_ptr->predict_bar.rect.left + textbar_width;
    }
    else//MMI_DIRECTION_RIGHT
    {
        // adjust the cursor rect first [edit ctrl might be modified to fit R-L direction]
        cursor_rect.right -= cursor_rect.left;
        cursor_rect.left = 0;

        // from the SPEC we place the mid-point of the predict bar rect as the cursor_rect->right...
        if (cursor_rect.right > (MMI_MAINSCREEN_WIDTH - textbar_width/2))
        {
            data_ptr->predict_bar.rect.right = MMI_MAINSCREEN_WIDTH - textbar_width/2 - 1;
            data_ptr->predict_bar.rect.left = data_ptr->predict_bar.rect.right - textbar_width;
        } 
        else if (cursor_rect.right < MMIIM_TEXT_POPUP_WIDTH/2)
        {
            data_ptr->predict_bar.rect.left = 0;
            data_ptr->predict_bar.rect.right = textbar_width - 1;
        }
        else
        {
            data_ptr->predict_bar.rect.left = cursor_rect.right - textbar_width/2;
            data_ptr->predict_bar.rect.right = cursor_rect.right + textbar_width/2 - 1;
        }
    }

    if(cursor_rect.bottom < (MMI_MAINSCREEN_HEIGHT/2 + MMIIM_TEXT_BAR_CAND_HEIGHT))
    {
        data_ptr->predict_bar.rect.top = (int16)(cursor_rect.bottom + 1);
        data_ptr->predict_bar.is_display_upper_side = 0;
    }
    else
    {
        data_ptr->predict_bar.rect.top = (int16)(cursor_rect.top - textbar_height - MMIIM_TEXT_BAR_BORDER_WIDTH);
        data_ptr->predict_bar.is_display_upper_side = 1;
    }
	
    data_ptr->predict_bar.rect.bottom = data_ptr->predict_bar.rect.top + textbar_height - 1;

    if (!GUI_EqualRect(old_prdictbar_rect, data_ptr->predict_bar.rect))
    {
        MMK_UpdateScreen();
    }

}

/******************************************************************************/
// Description: DrawUI
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void DrawUI(MMIIM_T9_DATA_T *data_ptr)
{
//    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;
    
    if (PNULL == data_ptr)
    {
        return;
    }
    
    data_ptr->is_rect_change = FALSE;

    if (MMIIM_SP_STATE_INPUTING == data_ptr->state
        || MMIIM_SP_STATE_SELECTING == data_ptr->state
        || MMIIM_SP_STATE_ASSOCIATE == data_ptr->state
        )
    {
#ifdef UI_MULTILAYER_SUPPORT
        if (data_ptr->is_predict_bar_dirty)
#endif
        {
            BOOLEAN is_need_border = MMIIM_GetMultiLineStyle(data_ptr->ctrl_handle);
            MMIIM_UpdatePredictBarPosition(data_ptr, is_need_border);
            MMIIM_PredictBarDraw(&(data_ptr->predict_bar), &data_ptr->cand_win, data_ptr->dis_direct, is_need_border);
        }
    }

    data_ptr->is_predict_bar_dirty = FALSE;

    return;
}

/******************************************************************************/
// Description: ReDrawUI
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void ReDrawUI(MMIIM_T9_DATA_T *data_ptr)
{
//    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;

    if (PNULL == data_ptr)
    {
        return;
    }
    
    if (ChangeRect(data_ptr))
    {
        return;
    }

    if (MMIIM_SP_STATE_INPUTING == data_ptr->state
        || MMIIM_SP_STATE_SELECTING == data_ptr->state
        || MMIIM_SP_STATE_ASSOCIATE == data_ptr->state
        )
    {
        if (data_ptr->is_predict_bar_dirty)
        {
            BOOLEAN is_need_border = MMIIM_GetMultiLineStyle(data_ptr->ctrl_handle);
            MMIIM_UpdatePredictBarPosition(data_ptr, is_need_border);
            MMIIM_PredictBarDraw(&(data_ptr->predict_bar), &data_ptr->cand_win, data_ptr->dis_direct, is_need_border);
        }
    }

    data_ptr->is_predict_bar_dirty = FALSE;
    
    return;
}

/******************************************************************************/
// Description: HandleMsg
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleMsg(
    void *handle, 
    GUIIM_EVENT_T event, 
    GUIIM_EVENT_DATA_U *event_data_ptr
)
{
    MMIIM_T9_DATA_T *data_ptr = (MMIIM_T9_DATA_T*)handle;
    BOOLEAN is_dealt = FALSE;
    BOOLEAN result = FALSE;
//    GUI_BOTH_RECT_T pop_menu_rect = {0};
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleMsg data_ptr is null.");
        return FALSE;
    }

	// Forbid to handle MSG_APP_OK
	if ((PNULL != event_data_ptr) && (MSG_APP_OK == event_data_ptr->sys_msg.msg_id))
	{
		SCI_TRACE_LOW("[MMIIM] HandleMsg do not handle MSG_APP_OK.");
		return FALSE;
	}

    mode_type = data_ptr->init_param.method_param_ptr->im_set.type;
    switch (event)
    {
    case GUIIM_EVENT_SYS_MSG:

        switch (data_ptr->state)
        {
        case MMIIM_SP_STATE_NOINPUT:
            if (mode_type == GUIIM_TYPE_SMART || mode_type == GUIIM_TYPE_ENGLISH)
            {
                is_dealt = HandleSysMsgNoInput(data_ptr, event_data_ptr);
            }
            else if (mode_type == GUIIM_TYPE_MULTITAP || mode_type == GUIIM_TYPE_ABC)
            {
                is_dealt = HandleSysMsgMultitap(data_ptr, event_data_ptr);
            }
            break;

        case MMIIM_SP_STATE_INPUTING:
            is_dealt = HandleSysMsgInputing(data_ptr, event_data_ptr);
            break;

        default:
            break;
        }

        if (is_dealt)
        {
            ReDrawUI(data_ptr);
            result = is_dealt;
           // result = TRUE;
        }
#if 1
/*
        else if ((PNULL != event_data_ptr)&&(MSG_APP_STAR == event_data_ptr->sys_msg.msg_id)&&(mode_type != GUIIM_TYPE_DIGITAL))
        {
		    if (GUIIM_TYPE_DIGITAL != mode_type)
			{
                MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, 
                    MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
                result = TRUE;
			}
        }
*/
        else if ((PNULL != event_data_ptr) && (MSG_APP_HASH == event_data_ptr->sys_msg.msg_id))
        {
            //MMIIM_SetPopMenuRect(data_ptr->init_param.win_handle, &pop_menu_rect);
            //MMIIM_SwitchingCreateWin(&(data_ptr->init_param), pop_menu_rect, MMIIM_EP_TOP_LEFT);
            ResetToNoInput(data_ptr, TRUE);
            MMIIM_SwitchingIM(&(data_ptr->init_param)); // for [MS]
#ifdef UI_MULTILAYER_SUPPORT
            MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
            MMK_UpdateScreen();
#endif
            result = TRUE;
        }
 #endif   
        break;
   
    case GUIIM_EVENT_SET_METHOD:
        if(PNULL != event_data_ptr)
        {
            result = SetMethod(handle, event_data_ptr->set_method);
        }
        break;
        
    case GUIIM_EVENT_GET_METHOD:
        if(PNULL != event_data_ptr)
        {
            event_data_ptr->get_method = data_ptr->method;
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_PAINT:
        DrawUI(data_ptr);
        result = TRUE;
        break;
        
    case GUIIM_EVENT_FORCE_COMMIT:
        {
            MMIIM_T9_TimeOut(&data_ptr->aw_field_info.G, T9_EXPL_TIMER_ID);
            MMIIM_StopTimer(&(data_ptr->is_timer_active));
            data_ptr->last_key = MMIIM_SP_KEY_NONE;
            
            result = TRUE;
            
            break;
        }

    case GUIIM_EVENT_SLEEP:
		//ResetToNoInput(data_ptr);
        result = TRUE;
        break;

    case GUIIM_EVENT_GET_CTRL_RECT:
    case GUIIM_EVENT_GET_RECT:
        if(PNULL != event_data_ptr)
        {
            event_data_ptr->get_rect = data_ptr->rect;
            result = TRUE;
        }
        break;

    case GUIIM_EVENT_GET_EDIT_STATE:
        if(PNULL != event_data_ptr)
        {
            if (MMIIM_SP_STATE_INPUTING == data_ptr->state)
            {
                if (data_ptr->is_spell_state)
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_SPELL;
                }
                else
                {
                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
                }
            }
            else
            {
                event_data_ptr->edit_state = GUIIM_EDIT_NONE;
            }

            result = TRUE;
        }
        break;

    case GUIIM_EVENT_CUSTOM_SWITCH_METHOD:
        MMIIM_SwitchingIM(&(data_ptr->init_param));
        break;

    default:
        break;
    }

    return result;
}

/******************************************************************************/
// Description: 初始化输入法相关的数据
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL void InitData(
    GUIIM_INIT_KERNEL_T *param_ptr,
    MMIIM_T9_DATA_T *data_ptr
)
{
    uint8 width_label = 0;
    GUI_RECT_T win_rect = {0};
    
    if(PNULL == param_ptr || PNULL == data_ptr)
    {
        return;
    }
    
    data_ptr->ctrl_handle = param_ptr->im_handle;
    data_ptr->init_param = *param_ptr;
    data_ptr->state = MMIIM_SP_STATE_NOINPUT;
    
    data_ptr->lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;
    
    SetToNoInput(data_ptr);

//     win_rect = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle); // [MS] for pubwin display fix

    width_label = GUI_GetFontWidth(g_mmiim_ui_config.font_lable, UNICODE_ASCII);

    data_ptr->rect = win_rect;
    
    //cand bar
    MMIIM_TextBarInit(&(data_ptr->predict_bar));
    data_ptr->predict_bar.win_id = data_ptr->init_param.win_id;
    data_ptr->predict_bar.lcd_dev_info.block_id = data_ptr->lcd_info_ptr->block_id;
    data_ptr->predict_bar.lcd_dev_info.lcd_id   = data_ptr->lcd_info_ptr->lcd_id;
    data_ptr->predict_bar.width_label = width_label;
    data_ptr->predict_bar.width_space = MMIIM_TEXT_BAR_SPACE;
    data_ptr->predict_bar.has_hot = 1;
    return;
}

/******************************************************************************/
// Description: 初始化内核
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL T9STATUS InitImKernel(
    MMIIM_T9_DATA_T *data_ptr
)
{    
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] InitImKernel : data_ptr is null.");
        return sStatus;
    }

    sStatus = MMIIM_T9_Init_Alpha(data_ptr);
    
	MMIIM_SetImKerState(1);

    SCI_TRACE_LOW("[MMIIM] InitImKernel : status=%d.", sStatus);
    
    return sStatus;
}

/******************************************************************************/
// Description: SetMethod
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN SetMethod(void *handle, GUIIM_METHOD_T method)
{    
    MMIIM_T9_DATA_T *data_ptr = (MMIIM_T9_DATA_T*)handle;    
    T9STATUS sStatus = T9STATNONE;
    
    if(PNULL == handle || GUIIM_M_NONE == method)
    {
        SCI_TRACE_LOW("[MMIIM] SetMethod: handle is null, method=%04X.",method);
        return FALSE;
    }
        
    data_ptr->method = method;
    data_ptr->dis_direct = MMIIM_GetDisplayDirection(method);
    
    SCI_TRACE_LOW("[MMIIM] SetMethod: method=%04X.", method);
    
    // set mode
    if (sStatus == T9STATNONE)/*lint !e774*/
    {
        sStatus = MMIIM_T9_SetMode(data_ptr);
    }

    // insert string to t9core which is already exist in editbox.
    if (sStatus == T9STATNONE)
    {
        SychT9WithEdit(data_ptr);
    }
        
    // set capital
    if (sStatus == T9STATNONE)
    {
        sStatus = MMIIM_T9_SetCapital(data_ptr);
    }
    
    if (s_is_autoswitch_simu_sych)
    {
        data_ptr->aw_field_info.G.nWordLen = 1;
        data_ptr->aw_field_info.G.dwStateBits |= T9STATEINSERTMASK;
        s_is_autoswitch_simu_sych = FALSE;
    }

    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetMethod sStatus=%d", sStatus);

    // set virtual capital
    switch (data_ptr->init_param.method_param_ptr->im_set.capital)
    {
    case GUIIM_MODE_LEAD_UPPER:
        data_ptr->cap_auto_switch = 1;
        data_ptr->cap_switch_type = MMIIM_T9_CAP_SWITCH_LEAD2LOWER;
        break;        
    case GUIIM_MODE_LOWER:
        data_ptr->cap_auto_switch = 1;//gaurav garg
        data_ptr->cap_switch_type = MMIIM_T9_CAP_SWITCH_LOWER2LEAD;
        break;        
    default:
        data_ptr->cap_auto_switch = 0;
        data_ptr->cap_switch_type = MMIIM_T9_CAP_SWITCH_MAX;
        break;
    }

    return (sStatus == T9STATNONE);
}

/******************************************************************************/
// Description: TransMsg
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void TransMsg(
    MMIIM_T9_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_MSG_T *msg_ptr
)
{
    MMIIM_SP_TP_AREA_INFO_T tp_area_info = {0};
    
    if (PNULL == data_ptr || PNULL == event_data_ptr || PNULL == msg_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] TransMsg: dataptr=%d, event=%d, msg=%d.", 
            data_ptr, event_data_ptr, msg_ptr);
        return;
    }
    
    MMIIM_SP_TransMsg(event_data_ptr, &tp_area_info, msg_ptr);

    return;
}

/******************************************************************************/
// Description: HandleSysMsgNoInput
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgNoInput(
    MMIIM_T9_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
//    wchar cmt_chr = 0;
    T9KEY eKeyID = T9KEYNONE;
//    BOOLEAN is_ret = FALSE;

    T9AWFieldInfo *aw_info_ptr = PNULL;
    T9UINT nPreWordLen = 0;
    T9UINT nPreBufLen = 0;
    T9UINT nWordLen = 0;
    T9UINT nCursor = 0;
    T9UINT nBufLen = 0;

    T9SYMB *text_ptr = PNULL;
    uint16 nCmtLen = 0;
    uint16 nUnderLen = 0;
    uint16 nIdx = 0;
    T9STATUS sStatus = T9STATERROR;
//    MMI_RESULT_E result = MMI_RESULT_TRUE;

    //处理消息
    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgNoInput: dataptr=%d, event=%d.", 
            data_ptr, event_data_ptr);
        return FALSE;
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);

    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id
        || MSG_NOTIFY_EDITBOX_IMSYNC_ADD == event_data_ptr->sys_msg.msg_id
        || MSG_NOTIFY_EDITBOX_IMSYNC_DEL == event_data_ptr->sys_msg.msg_id)
    {
        if (ReEntryIm(data_ptr))
        {
            is_dealt = TRUE;
        }
        else
        {
            SychT9WithEdit(data_ptr);
            //shift is value for only one time, so should set capital at the end of set engine
            MMIIM_T9_SetCapital(data_ptr);
        }
        return is_dealt;
    }

    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
		ResetToNoInput(data_ptr, FALSE);
        return TRUE;
    }

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgNoInput: msg_id = null.");
        return FALSE;
    }
    
    //直接转换按键消息
    if (!MMIIM_ConvertMsgToT9KeyID(&msg, &eKeyID))
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgNoInput: invalid im_key=%d.",
            msg.para.key);

        return is_dealt;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_KEY_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
		case MMIIM_SP_KEY_STAR:
            data_ptr->last_key = msg.para.key;
            break;

		// Adjust to be consistent with the handler of the sp-multitap.
        case MMIIM_SP_KEY_LEFT:
#if 0
            if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
            {
                is_dealt = HandleRightKeyMsg(data_ptr, MMIIM_MOVE_CHAR_BY_CHAR);
            }
            else
#endif
            {
                is_dealt = HandleLeftKeyMsg(data_ptr, MMIIM_MOVE_CHAR_BY_CHAR);
            }
            break;

        case MMIIM_SP_KEY_RIGHT:
#if 0
            if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
            {
                is_dealt = HandleLeftKeyMsg(data_ptr, MMIIM_MOVE_CHAR_BY_CHAR);
            }
            else
#endif
            {
                is_dealt = HandleRightKeyMsg(data_ptr, MMIIM_MOVE_CHAR_BY_CHAR);
            }
            break;

        case MMIIM_SP_KEY_CANCEL:
            is_dealt = HandleCancelKeyMsg(data_ptr);
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    case MMIIM_SP_MSG_KEY_RELEASE:
        is_dealt = TRUE;
        
        if (MMIIM_SP_KEY_CANCEL == msg.para.key)
        {
            AutoSwitchCapital(data_ptr);
            is_dealt = TRUE;
        } 

        if (MMIIM_SP_KEY_NONE == data_ptr->last_key
            || MMIIM_SP_KEY_MAX == data_ptr->last_key)
        {
            is_dealt = FALSE;
            break;
        }
        data_ptr->last_key = MMIIM_SP_KEY_NONE;
        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
		case MMIIM_SP_KEY_STAR:
            if (0x01 == data_ptr->cap_auto_switch)
            {
                //MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G,
                 //   T9KEYSHIFT);
            }

            aw_info_ptr = &data_ptr->aw_field_info;
            nPreBufLen = aw_info_ptr->G.nBufLen;
            nPreWordLen = aw_info_ptr->G.nWordLen;

#if 0
            if((!(aw_info_ptr->G.dwStateBits & T9STATEINSERTMASK) || MMIIM_SP_KEY_0!=msg.para.key) && (aw_info_ptr->G.dwStateBits & T9STATEEXTMULTITAPMASK))
            {
                T9ClrExtMultitapMode(&aw_info_ptr->G);
            }
            if(MMIIM_SP_KEY_0==msg.para.key && (!(aw_info_ptr->G.dwStateBits & T9STATEINSERTMASK) || aw_info_ptr->G.T9Private.eLastKey!=T9KEYAMBIGA) && !(aw_info_ptr->G.dwStateBits & T9STATEEXTMULTITAPMASK))
            {
                T9SetExtMultitapMode(&aw_info_ptr->G);
            }
#endif
            sStatus = MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, eKeyID);

            nWordLen = aw_info_ptr->G.nWordLen;
            nCursor = aw_info_ptr->G.nCursor;
            nBufLen = aw_info_ptr->G.nBufLen;

            if (MMIIM_SP_KEY_0 == msg.para.key && 1 == nWordLen && ('0' == aw_info_ptr->G.psTxtBuf[nCursor - 1] || ' ' == aw_info_ptr->G.psTxtBuf[nCursor - 1]))
            {
                data_ptr->cand_win.count = 0;
                MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYRIGHT);
                MMIIM_ClearImeComp(&data_ptr->init_param);
                CommitChar(data_ptr, ' ');
            }
            else if ((MMIIM_SP_KEY_STAR == msg.para.key) && (T9STATNOWRD == sStatus))
            {
                MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, 
                    MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
#ifdef UI_MULTILAYER_SUPPORT
                MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                MMK_UpdateScreen();
#endif
                is_dealt = TRUE;
            }
            else
            {
                if (nPreBufLen < nBufLen)
                {
                    nUnderLen = nWordLen;
                    nCmtLen = 0;
                    nIdx = nCursor - nWordLen - nCmtLen;

                    // No need get predict word and convert to inputing state if buffer length did not modify...
                    GetPredictWord(data_ptr, msg.para.key,nIdx);
                    SetToInputing(data_ptr);

                    text_ptr = &(aw_info_ptr->G.psTxtBuf[nIdx]);
                    MMIIM_ReplaceChars(&data_ptr->init_param, text_ptr, nCmtLen, nUnderLen);
                }
            }


            if (1 == data_ptr->cap_auto_switch)
            {
                AutoSwitchCapital(data_ptr);
            }
            
            break;

        default:
            is_dealt = FALSE;
            break;
        }
        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
            is_dealt = HandleLongNumberKeyMsg(data_ptr, msg.para.key);
            break;

        case MMIIM_SP_KEY_LEFT:
            if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
            {
                is_dealt = HandleLongRightKeyMsg(data_ptr);
            }
            else
            {
                is_dealt = HandleLongLeftKeyMsg(data_ptr);
            }
            break;

        case MMIIM_SP_KEY_RIGHT:
            if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
            {
                is_dealt = HandleLongLeftKeyMsg(data_ptr);
            }
            else
            {
                is_dealt = HandleLongRightKeyMsg(data_ptr);
            }
            break;

		case MMIIM_SP_KEY_CANCEL:
		case MMIIM_SP_KEY_DEL:
	        {
// Currently, T9 Engine just handle repeat key to delete one by one
// so we should do nothing when long del msg received
#if 0 
				GUIIM_NOTIFY_PACK_T notify_pack = {0};      	
				
	            data_ptr->last_key = MMIIM_SP_KEY_MAX;
	            data_ptr->last_key_index = 0;

	            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
	            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
#endif
	            is_dealt = TRUE;

	        }

            break;

        case MMIIM_SP_KEY_STAR:
            {
                MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, 
                			MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
#ifdef UI_MULTILAYER_SUPPORT
                MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                MMK_UpdateScreen();
#endif
                is_dealt = TRUE;
            }
            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    case MMIIM_SP_MSG_KEY_REPEAT_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_CANCEL:
            is_dealt = HandleCancelKeyMsg(data_ptr);
            break;
        default:
            is_dealt = FALSE;
            break;
        }
        break;

    default:
        break;
    }
    
    return is_dealt;
}

/******************************************************************************/
// Description: HandleSysMsgInputing
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgInputing(
    MMIIM_T9_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};    
    //GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    T9KEY eKeyID = T9KEYNONE;
    //BOOLEAN is_ret = FALSE;
    BOOLEAN is_spell_result = FALSE;
    T9STATUS sStatus = T9STATERROR;

    T9AWFieldInfo *aw_info_ptr = PNULL;
    T9UINT nPreCurSelObj = 0;
    T9UINT nPreWordLen = 0;
    T9UINT nPreCursor = 0;
    T9UINT nPreBufLen = 0;
    T9UINT nCurSelObj = 0;
    T9UINT nWordLen = 0;
    T9UINT nCursor = 0;
    T9UINT nBufLen = 0;

    if (PNULL == data_ptr ||PNULL == event_data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputing: dataptr=%d, event=%d.", 
            data_ptr, event_data_ptr);
        return FALSE;
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);
    
    if ((MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id 
            || MSG_NOTIFY_EDITBOX_IMSYNC_ADD == event_data_ptr->sys_msg.msg_id
            || MSG_NOTIFY_EDITBOX_IMSYNC_DEL == event_data_ptr->sys_msg.msg_id)
        && data_ptr->is_spell_state == FALSE)
    {
        if (ReEntryIm(data_ptr))
        {
            is_dealt = TRUE;
        }
        else
        {
            if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id)
            {
                SetToNoInput(data_ptr);
            }

            SychT9WithEdit(data_ptr);
            //shift is value for only one time, so should set capital at the end of set engine
            MMIIM_T9_SetCapital(data_ptr);
        }
        return is_dealt;
    }

    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        ResetToNoInput(data_ptr, FALSE);
        return TRUE;
    }

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputing: msg_id = null.");
        return FALSE;
    }
    
    //直接转换按键消息
    if (!MMIIM_ConvertMsgToT9KeyID(&msg, &eKeyID))
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputing: invalid im_key=%d.",
            msg.para.key);

        return is_dealt;
    }

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_KEY_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_UP:
        case MMIIM_SP_KEY_RIGHT:
        case MMIIM_SP_KEY_LEFT:
        case MMIIM_SP_KEY_CANCEL:
        case MMIIM_SP_KEY_DOWN:
		/*
		 *	Bug 126806 - [ST][7701]En下输入左软键显示和作用不一致
		 *  注释：在英文输入法下，输入的英文作为一个整体高亮显示；此时点击左软键softkey，应该代表单词确认并上屏；
		 *  而T9没有对msg.para.key = MMIIM_SP_KEY_OK作处理，因此此处将msg.para.key 转化为MMIIM_SP_KEY_RIGHT；
		 *  即将ok键的消息作为右键消息处理。
		 *  而对于data_ptr->is_spell_state的情况下，保持原来的情况，不做修改。
		 */
        case MMIIM_SP_KEY_OK:
            if ( MMIIM_SP_KEY_OK == msg.para.key)
            {			
                if (data_ptr->is_spell_state)
                {
                    SetToNoInput(data_ptr);

#ifdef UI_MULTILAYER_SUPPORT
                    MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                    MMK_UpdateScreen();
#endif
                    CreateAddNewWordWin(data_ptr);
                    break;
                }
                else
                {
                    if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
                    {
                        msg.para.key = MMIIM_SP_KEY_LEFT;
                        eKeyID = T9KEYLEFT;
                    }
                    else
                    {
                        msg.para.key = MMIIM_SP_KEY_RIGHT;
                        eKeyID = T9KEYRIGHT;
                    }
                }
            }

            aw_info_ptr = &data_ptr->aw_field_info;
            nPreCurSelObj = aw_info_ptr->G.nCurSelObj;
            nPreWordLen = aw_info_ptr->G.nWordLen;
            nPreCursor = aw_info_ptr->G.nCursor;
            nPreBufLen = aw_info_ptr->G.nBufLen;

            //处理
            if((1 == data_ptr->cand_win.cur_sel) && (MMIIM_SP_KEY_UP == msg.para.key))
            {
                data_ptr->is_spell_state = TRUE;
                aw_info_ptr->G.nCurSelObj = data_ptr->cand_win.count+1;
                is_spell_result = TRUE;
                sStatus = T9STATNONE;
            }
            else if((data_ptr->cand_win.count+1 == aw_info_ptr->G.nCurSelObj) && (MMIIM_SP_KEY_DOWN == msg.para.key))
            {
                aw_info_ptr->G.nCurSelObj = 1;
                is_spell_result = TRUE;
                sStatus = T9STATNONE;
            }
            else if((data_ptr->cand_win.count == (data_ptr->cand_win.cur_sel)) && (MMIIM_SP_KEY_DOWN == msg.para.key))
            {
                data_ptr->cand_win.cur_sel = data_ptr->cand_win.count;
                aw_info_ptr->G.nCurSelObj = 0;
                sStatus = T9STATNONE;
            }
            else
            {
                if((0 == aw_info_ptr->G.nCurSelObj) && (MMIIM_SP_KEY_UP == msg.para.key))
                {
                    aw_info_ptr->G.nCurSelObj = data_ptr->cand_win.count;
                    sStatus = T9STATNONE;
                }
                else
                {
                    sStatus = MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, eKeyID);
                }
            }
			//sStatus = MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, eKeyID);

            if (T9STATEMPTY == sStatus
                || T9STATFULL == sStatus
                || T9STATBEGIN == sStatus
                || T9STATEND == sStatus
                || T9STATMAXWRD == sStatus
                || T9STATNOWRD == sStatus)
            {
                //set softkey value
                data_ptr->is_spell_state = TRUE;
                is_spell_result = TRUE;
            }

            if (T9STATNONE != sStatus)
            {
                return is_dealt;
            }

            nCurSelObj = aw_info_ptr->G.nCurSelObj;
            nWordLen = aw_info_ptr->G.nWordLen;
            nCursor = aw_info_ptr->G.nCursor;
            nBufLen = aw_info_ptr->G.nBufLen;

            if (MMIIM_SP_KEY_DOWN == msg.para.key)
            {
#ifdef MMI_IME_USER_DATABASE
                if (((nPreCurSelObj == nCurSelObj) && (nCurSelObj != 0))
					|| ((nCurSelObj < nPreCurSelObj) && (nPreCurSelObj != 0)))
                {
                    //set softkey value
                    data_ptr->is_spell_state = TRUE;
                    is_spell_result = TRUE;
                }
#endif
            }

            if (!is_spell_result)
            {
                data_ptr->is_spell_state = FALSE;
            }

            {
                T9SYMB *text_ptr = PNULL;
                uint16 nCmtLen = 0;
                uint16 nUnderLen = 0;
                uint16 nIdx = 0;
                BOOLEAN is_move_cursor = FALSE;
                T9SYMB  backspace_commit = 0;

                if (0 == nWordLen)
                {
                    SetToNoInput(data_ptr);
#ifdef UI_MULTILAYER_SUPPORT
                    MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                    MMK_UpdateScreen();
#endif
                    if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
                    {
                        if (MMIIM_SP_KEY_RIGHT == msg.para.key)
                        {
                            nUnderLen = 0;
                            nCmtLen = nPreWordLen;
                            text_ptr =
                                &(aw_info_ptr->G.psTxtBuf[nCursor-nPreWordLen]);

                            is_move_cursor = TRUE;
                        }

                        if (MMIIM_SP_KEY_LEFT == msg.para.key)
                        {
                            nUnderLen = 0;
                            nCmtLen = nPreWordLen;
                            text_ptr = &(aw_info_ptr->G.psTxtBuf[nCursor]);
                        }
                    }
                    else
                    {
                        if (MMIIM_SP_KEY_RIGHT == msg.para.key)
                        {
                            nUnderLen = 0;
                            nCmtLen = nPreWordLen;
                            text_ptr =
                                &(aw_info_ptr->G.psTxtBuf[nCursor-nPreWordLen]);
                        }

                        if (MMIIM_SP_KEY_LEFT == msg.para.key)
                        {
                            nUnderLen = 0;
                            nCmtLen = nPreWordLen;
                            text_ptr = &(aw_info_ptr->G.psTxtBuf[nCursor]);

                            is_move_cursor = TRUE;
                        }
                    }

                    if (MMIIM_SP_KEY_CANCEL == msg.para.key)
                    {
                        /*
                         *	当underline清空时需要向上传递backspace，否则edit不会处理clear操作
                         */
                        nUnderLen = 0;
                        nCmtLen = 1;
                        backspace_commit = '\b';
                        text_ptr = &backspace_commit;
                    }
                }
                else
                {
                    if (nPreWordLen < nWordLen)
                    {
                        nUnderLen = nWordLen;
                        nCmtLen = 0;
                        nIdx = nCursor-nWordLen;
                    }

                    if (nPreWordLen == nWordLen)
                    {
                        nUnderLen = nWordLen;
                        if (nBufLen > nPreBufLen)
                        {
                            nCmtLen = nBufLen - nPreBufLen;
                        }
                        else
                        {
                            nCmtLen = 0;
                        }
                        nIdx = nCursor-nWordLen-nCmtLen;
                    }

                    if (nPreWordLen > nWordLen)
                    {
                        nUnderLen = nWordLen;
                        if (nBufLen > nPreBufLen)
                        {
                            nCmtLen = nPreWordLen;
                        }
                        else
                        {
                            nCmtLen = 0;
                        }
                        nIdx = nCursor-nWordLen-nCmtLen;
                    }

                    text_ptr = &(aw_info_ptr->G.psTxtBuf[nIdx]);
                    GetPredictWord(data_ptr, msg.para.key,nIdx); 
                }
				
                //no need notify editor change cursor when commit chars
                if (is_move_cursor)
                {
                    int16 nStep = nPreWordLen;
                    
                    MMIIM_ReplaceChars(&data_ptr->init_param, text_ptr, nCmtLen, nUnderLen);
                    
                    MMIIM_SetEditCursor(&data_ptr->init_param, GUIIM_SETCURSOR_TYPE_CUR, -nStep);
                }
                else
                {
                    MMIIM_ReplaceChars(&data_ptr->init_param, text_ptr, nCmtLen, nUnderLen);
                }
            }
            break;

//        case MMIIM_SP_KEY_OK:
//             if (data_ptr->is_spell_state)
//             {
//                 SetToNoInput(data_ptr);
//                 CreateAddNewWordWin(data_ptr);
//             }
//             else
//             {
//                 SetToNoInput(data_ptr);
//                 is_dealt = FALSE;
//             }
//            break;

        default:
            is_dealt = FALSE;
            break;
        }

        break;

    case MMIIM_SP_MSG_KEY_RELEASE:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
        case MMIIM_SP_KEY_STAR:
            aw_info_ptr = &data_ptr->aw_field_info;
            nPreCurSelObj = aw_info_ptr->G.nCurSelObj;
            nPreWordLen = aw_info_ptr->G.nWordLen;
            nPreCursor = aw_info_ptr->G.nCursor;
            nPreBufLen = aw_info_ptr->G.nBufLen;
            
#if 0
            if((!(aw_info_ptr->G.dwStateBits & T9STATEINSERTMASK) || MMIIM_SP_KEY_0!=msg.para.key) && (aw_info_ptr->G.dwStateBits & T9STATEEXTMULTITAPMASK))
            {
                T9ClrExtMultitapMode(&aw_info_ptr->G);
            }
            if(MMIIM_SP_KEY_0==msg.para.key && (!(aw_info_ptr->G.dwStateBits & T9STATEINSERTMASK) || aw_info_ptr->G.T9Private.eLastKey!=T9KEYAMBIGA) && !(aw_info_ptr->G.dwStateBits & T9STATEEXTMULTITAPMASK))
            {
                T9SetExtMultitapMode(&aw_info_ptr->G);
            }
#endif

            //处理
            sStatus = MMIIM_T9_HandleKey(&aw_info_ptr->G, eKeyID);

            if (T9STATEMPTY == sStatus
                || T9STATFULL == sStatus
                || T9STATBEGIN == sStatus
                || T9STATEND == sStatus
                || T9STATMAXWRD == sStatus)
				// @Arvin Disable convert to spell state automatically for no matched word
				// to avoid confusing user because there's special button "+Add" now. 
                //|| T9STATNOWRD == sStatus)
            {
                //set softkey value
                data_ptr->is_spell_state = TRUE;
                is_spell_result = TRUE;
            }

            if (T9STATNONE != sStatus)
            {
                return is_dealt;
            }

            nCurSelObj = aw_info_ptr->G.nCurSelObj;
            nWordLen = aw_info_ptr->G.nWordLen;
            nCursor = aw_info_ptr->G.nCursor;
            nBufLen = aw_info_ptr->G.nBufLen;

            {
                T9SYMB *text_ptr = PNULL;
                uint16 nCmtLen = 0;
                uint16 nUnderLen = 0;
                uint16 nIdx = 0;

                if (0 == nWordLen)
                {
                    SetToNoInput(data_ptr);
#ifdef UI_MULTILAYER_SUPPORT
                    MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                    MMK_UpdateScreen();
#endif
                }
                else
                {
                    if (nPreWordLen < nWordLen)
                    {
                        nUnderLen = nWordLen;
                        nCmtLen = 0;
                    }

                    if (nPreWordLen == nWordLen)
                    {
                        nUnderLen = nWordLen;
                        if (nBufLen > nPreBufLen)
                        {
                            nCmtLen = nBufLen - nPreBufLen;
                        }
                        else
                        {
                            nCmtLen = 0;
                        }
                    }

                    if (nPreWordLen > nWordLen)
                    {
                        nUnderLen = nWordLen;
                        if (nBufLen > nPreBufLen)
                        {
                            nCmtLen = nPreWordLen + (nBufLen - nPreBufLen) - nWordLen;
                        }
                        else
                        {
                            nCmtLen = 0;
                        }
                    }

                    nIdx = nCursor-nWordLen;
                    text_ptr = &(aw_info_ptr->G.psTxtBuf[nIdx - nCmtLen]);
                }

                if (!is_spell_result)
                {
                    data_ptr->is_spell_state = FALSE;
                }
                GetPredictWord(data_ptr, msg.para.key,nIdx);
                if (MMIIM_SP_KEY_0 == msg.para.key && ('0' == aw_info_ptr->G.psTxtBuf[nCursor - 1] || ' ' == aw_info_ptr->G.psTxtBuf[nCursor - 1]))
                {
                    MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYRIGHT);
                    MMIIM_ClearImeComp(&data_ptr->init_param);
                    CommitChar(data_ptr, ' ');
                    SetToNoInput(data_ptr);
#ifdef UI_MULTILAYER_SUPPORT
                    MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                    MMK_UpdateScreen();
#endif
                }
                else if ((MMIIM_SP_KEY_STAR == msg.para.key) && (T9STATNOWRD == sStatus))
                {
                    MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, 
                    MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
#ifdef UI_MULTILAYER_SUPPORT
                    MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                    MMK_UpdateScreen();
#endif
                    is_dealt = TRUE;
                }
                else
                {
                    MMIIM_ReplaceChars(&data_ptr->init_param, text_ptr, nCmtLen, nUnderLen);
                }                
            }
            break;

        default:
            is_dealt = FALSE;
            break;
        }
        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
            is_dealt = HandleLongNumberKeyMsg(data_ptr, msg.para.key);
            break;

        case MMIIM_SP_KEY_LEFT:
            if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
            {
                is_dealt = HandleLongRightKeyMsg(data_ptr);
            }
            else
            {
                is_dealt = HandleLongLeftKeyMsg(data_ptr);
            }
            break;

        case MMIIM_SP_KEY_RIGHT:
            if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
            {
                is_dealt = HandleLongLeftKeyMsg(data_ptr);
            }
            else
            {
                is_dealt = HandleLongRightKeyMsg(data_ptr);
            }
            break;

        case MMIIM_SP_KEY_STAR:
            {
                MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, 
                MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
#ifdef UI_MULTILAYER_SUPPORT
                MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                MMK_UpdateScreen();
#endif
                is_dealt = TRUE;
            }
            break;

        case MMIIM_SP_KEY_UP:
        case MMIIM_SP_KEY_DOWN:
            is_dealt = TRUE;
            break;

        default:
            is_dealt = FALSE;
            break;
        }
        break;

    case MMIIM_SP_MSG_KEY_REPEAT_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_CANCEL:
            is_dealt = HandleCancelKeyMsg(data_ptr);
            break;

        case MMIIM_SP_KEY_UP:
        case MMIIM_SP_KEY_DOWN:
            is_dealt = TRUE;
            break;

        default:
            is_dealt = FALSE;
            break;
        }
        break;

    default:
        break;
    }


    return is_dealt;
}

/*****************************************************************************/
//  Description : handle alphabtic multitap char msg of no input state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleSysMsgMultitap(
    MMIIM_T9_DATA_T *data_ptr,
    GUIIM_EVENT_DATA_U const *event_data_ptr
)
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    T9KEY eKeyID = T9KEYNONE;
    BOOLEAN is_ret = FALSE;
    BOOLEAN is_last_key = FALSE;
    BOOLEAN is_commit = FALSE;
    MMIIM_SP_VKEY_T cur_key = MMIIM_SP_KEY_MAX;
    uint16 ch = 0;
    //GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;

    T9STATUS sStatus = T9STATERROR;
    T9AWFieldInfo *aw_info_ptr = PNULL;
    T9UINT nPreWordLen = 0;
    T9UINT nPreBufLen = 0;
    T9UINT nWordLen = 0;
    T9UINT nCursor = 0;
    T9UINT nBufLen = 0;

    T9SYMB *text_ptr = PNULL;
    uint16 nCmtLen = 0;
    uint16 nUnderLen = 0;
    uint16 nIdx = 0;
    //MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr = PNULL;

    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgMultitap: data_ptr wrong param.");
        return is_dealt;
    }

    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id
        || MSG_NOTIFY_EDITBOX_IMSYNC_ADD == event_data_ptr->sys_msg.msg_id
        || MSG_NOTIFY_EDITBOX_IMSYNC_DEL == event_data_ptr->sys_msg.msg_id)
    {
        if (ReEntryIm(data_ptr))
        {
            is_dealt = TRUE;
        }
        else
        {
            SychT9WithEdit(data_ptr);
            //shift is value for only one time, so should set capital at the end of set engine
            MMIIM_T9_SetCapital(data_ptr);
        }
        return is_dealt;
    }

    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        MMIIM_StopTimer(&(data_ptr->is_timer_active));
        data_ptr->last_key = MMIIM_SP_KEY_MAX;
        data_ptr->last_key_index = 0;

        return is_dealt;
    }

    //特别处理定时器的情况
    //1 定时器触发
    //2 其他按键的发生
    if (MSG_TIMER == event_data_ptr->sys_msg.msg_id
            && MMIIM_EXPL_TIMER_ID == *(uint8*)(event_data_ptr->sys_msg.param))
    {
        return CommitMulLastKey(data_ptr, &is_commit);
    }
    
    TransMsg(data_ptr, event_data_ptr, &msg);

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        SCI_TRACE_LOW("[MMIIM] HandleMsgMultitapAlpha: msg_id = null.");
        return FALSE;
    }

    //直接转换按键消息
    if (!MMIIM_ConvertMsgToT9KeyID(&msg, &eKeyID))
    {
        SCI_TRACE_LOW("[MMIIM] HandleMsgMultitapAlpha: invalid im_key=%d.",
            msg.para.key);

        return is_dealt;
    }

    cur_key = msg.para.key;

    is_last_key = (cur_key == data_ptr->last_key && cur_key < MMIIM_SP_KEY_MAX);
    aw_info_ptr = &data_ptr->aw_field_info;

    //处理消息
    switch (msg.id)
    {
    case MMIIM_SP_MSG_KEY_PRESS:
        is_dealt = TRUE;

        //无论如何先停下定时器
        MMIIM_StopTimer(&(data_ptr->is_timer_active));

        if (MMIIM_SP_KEY_UP == msg.para.key
                || MMIIM_SP_KEY_DOWN == msg.para.key
                || MMIIM_SP_KEY_LEFT == msg.para.key
                || MMIIM_SP_KEY_RIGHT == msg.para.key
                || MMIIM_SP_KEY_OK == msg.para.key
                || MMIIM_SP_KEY_CANCEL == msg.para.key)
        {
            is_ret = CommitMulLastKey(data_ptr, &is_commit);
            if (!is_ret)
            {
                is_dealt = FALSE;
            }

            if (!is_commit)
            {
                //if the key is not used to confirm underline, then move cursor
                if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
                {
                    if (MMIIM_SP_KEY_RIGHT == msg.para.key)
                    {
                        is_dealt = HandleLeftKeyMsg(data_ptr,
                            MMIIM_MOVE_CHAR_BY_CHAR);
                    }
                    else if (MMIIM_SP_KEY_LEFT == msg.para.key)
                    {
                        is_dealt = HandleRightKeyMsg(data_ptr,
                            MMIIM_MOVE_CHAR_BY_CHAR);
                    }
                }
                else
                {
                    if (MMIIM_SP_KEY_RIGHT == msg.para.key)
                    {
                        is_dealt = HandleRightKeyMsg(data_ptr,
                            MMIIM_MOVE_CHAR_BY_CHAR);
                    }
                    else if (MMIIM_SP_KEY_LEFT == msg.para.key)
                    {
                        is_dealt = HandleLeftKeyMsg(data_ptr,
                            MMIIM_MOVE_CHAR_BY_CHAR);
                    }
                }
            }

        if(MMIIM_SP_KEY_OK == msg.para.key 
            ||MMIIM_SP_KEY_UP == msg.para.key
            ||MMIIM_SP_KEY_DOWN == msg.para.key)
            {
                is_dealt = 2*TRUE;  // sanjeev changes
            }
            if (MMIIM_SP_KEY_CANCEL == msg.para.key)
            {
                is_dealt = HandleCancelKeyMsg(data_ptr);
            }
        }
        else
        {
            if (MMIIM_SP_KEY_0 <= cur_key && cur_key <= MMIIM_SP_KEY_STAR)
            {
                data_ptr->last_down_key = cur_key;
            }
            
            is_dealt = FALSE;
        }
        break;

    case MMIIM_SP_MSG_KEY_RELEASE:
		
		//cancel key may cause method switching, here we should check to ensure the capital state 
		//of method is correct, more details pls refer bug135657
		if(MMIIM_SP_KEY_CANCEL == cur_key && MMIIM_NeedSwitchLeadCap(&data_ptr->init_param))
		{			
			AutoSwitchMethod(data_ptr);
			is_dealt = TRUE;		
		}

        if (MMIIM_SP_KEY_0 <= cur_key && cur_key <= MMIIM_SP_KEY_9)
        {
            if (cur_key != data_ptr->last_down_key)
            {
                break;
            }
            
            if (MMIIM_SP_KEY_MAX != data_ptr->last_key
                && MMIIM_SP_KEY_NONE != data_ptr->last_key
                && cur_key != data_ptr->last_key
                && 1 == data_ptr->cap_auto_switch)
            {
                if (AutoSwitchMethod(data_ptr))
                {
                    SCI_MEMCPY(&s_last_eventdata_autoswitch, event_data_ptr, sizeof(GUIIM_EVENT_DATA_U));
                }
            }

            nPreBufLen = aw_info_ptr->G.nBufLen;
            nPreWordLen = aw_info_ptr->G.nWordLen;

#if 1
            if((!(aw_info_ptr->G.dwStateBits & T9STATEINSERTMASK) || MMIIM_SP_KEY_0!=msg.para.key) && (aw_info_ptr->G.dwStateBits & T9STATEEXTMULTITAPMASK))
            {
                T9ClrExtMultitapMode(&aw_info_ptr->G);
            }
            if(MMIIM_SP_KEY_0==msg.para.key && (!(aw_info_ptr->G.dwStateBits & T9STATEINSERTMASK) || aw_info_ptr->G.T9Private.eLastKey!=T9KEYAMBIGA) && !(aw_info_ptr->G.dwStateBits & T9STATEEXTMULTITAPMASK))
            {
                T9SetExtMultitapMode(&aw_info_ptr->G);
            }
#endif

             //处理
            sStatus = MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, eKeyID);

            if (T9STATNONE == sStatus)
            {
                nWordLen = aw_info_ptr->G.nWordLen;
                nCursor = aw_info_ptr->G.nCursor;
                nBufLen = aw_info_ptr->G.nBufLen;

                if (nPreBufLen < nBufLen)
                {
                    nUnderLen = nWordLen;
                    nCmtLen = nPreWordLen;
                }
                else if (nPreBufLen == nBufLen)
                {
                    if (0 != nWordLen)
                    {
                        nUnderLen = nWordLen;
                        nCmtLen = 0;
                    }
                }

                nIdx = nCursor - nWordLen - nCmtLen;
                text_ptr = &(aw_info_ptr->G.psTxtBuf[nIdx]);
                MMIIM_ReplaceChars(&data_ptr->init_param, text_ptr, nCmtLen, nUnderLen);
                
                data_ptr->last_key = cur_key;
                data_ptr->last_char = ch;

                MMIIM_StartTimer(&(data_ptr->is_timer_active), data_ptr->ctrl_handle);
            }

            is_dealt = TRUE;
        }
        break;

    case MMIIM_SP_MSG_KEY_LONG_PRESS:
        is_dealt = TRUE;
        
        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_0:
        case MMIIM_SP_KEY_1:
        case MMIIM_SP_KEY_2:
        case MMIIM_SP_KEY_3:
        case MMIIM_SP_KEY_4:
        case MMIIM_SP_KEY_5:
        case MMIIM_SP_KEY_6:
        case MMIIM_SP_KEY_7:
        case MMIIM_SP_KEY_8:
        case MMIIM_SP_KEY_9:
            is_dealt = HandleLongNumberKeyMsg(data_ptr, msg.para.key);
            break;

        case MMIIM_SP_KEY_LEFT:
            if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
            {
                is_dealt = HandleLongRightKeyMsg(data_ptr);
            }
            else
            {
                is_dealt = HandleLongLeftKeyMsg(data_ptr);
            }
            break;

        case MMIIM_SP_KEY_RIGHT:
            if (data_ptr->dis_direct == MMI_DIRECTION_RIGHT)
            {
                is_dealt = HandleLongLeftKeyMsg(data_ptr);
            }
            else
            {
                is_dealt = HandleLongRightKeyMsg(data_ptr);
            }
            break;

		case MMIIM_SP_KEY_CANCEL:
		case MMIIM_SP_KEY_DEL:
	        {
	        	GUIIM_NOTIFY_PACK_T notify_pack = {0};
				
	            data_ptr->last_key = MMIIM_SP_KEY_MAX;
	            data_ptr->last_key_index = 0;

	            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
	            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

	            is_dealt = TRUE;
	        }

			break;
			
        default:
            is_dealt = FALSE;
            break;
        }
        break;

    case MMIIM_SP_MSG_KEY_REPEAT_PRESS:
        is_dealt = TRUE;

        switch (msg.para.key)
        {
        case MMIIM_SP_KEY_CANCEL:
            is_dealt = HandleCancelKeyMsg(data_ptr);
            break;
        default:
            is_dealt = FALSE;
            break;
        }
        break;

    default:
        break;
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : set to no input state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToNoInput(MMIIM_T9_DATA_T *data_ptr)
{
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SetToNoInput: data_ptr is null.");
        return;
    }
    data_ptr->state = MMIIM_SP_STATE_NOINPUT;
    
    data_ptr->is_spell_state = FALSE;
    //s_ime_editor.editbox_ptr->is_spell = FALSE;

    return;
}

/*****************************************************************************/
//  Description : set to samsung inputing state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToInputing(MMIIM_T9_DATA_T *data_ptr)
{
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SetToInputing: data_ptr is null.");
        return;
    }

    data_ptr->state = MMIIM_SP_STATE_INPUTING;

    return;
}

/*****************************************************************************/
//  Description : sychronization t9 engine content and curson with editbox 
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL T9STATUS SychT9WithEdit(MMIIM_T9_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATNONE;
    GUIIM_CONTEXT_T im_context = {0};
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SychT9WithEdit: data_ptr is null.");
        return sStatus;
    }

    MMIIM_GetEditContent(&data_ptr->init_param, &im_context);

	// It seems that we should not reset Text buffer if current context is underline
	// because this will clear the engine buffer and lead to mismatch with im app
    if (PNULL != im_context.str_ptr && !im_context.is_underline)
    {
        sStatus = MMIIM_T9_ResetTextBuf(&data_ptr->aw_field_info.G);
        
        if (T9STATNONE == sStatus)
        {
            //gaurav changes
            sStatus = MMIIM_T9_AddTextString(
                &data_ptr->aw_field_info.G, im_context.str_ptr, im_context.str_len);
        }

        if (T9STATNONE == sStatus)
        {
            sStatus = SetIMECursorPos(data_ptr);
        }
        
#if 0
        //sychronzation timer if need
        if (T9STATNONE == sStatus && im_context.is_underline
            && (GUIIM_TYPE_MULTITAP == data_ptr->init_param.method_param_ptr->im_set.type
                || GUIIM_TYPE_ABC == data_ptr->init_param.method_param_ptr->im_set.type)
            && im_context.end_pos == im_context.pos
            && im_context.end_pos >= 2)
        {
            //
            //im_context.str_len = im_context.str_len - (im_context.end_pos -im_context.start_pos);
            //data_ptr->aw_field_info.G.nWordLen = im_context.end_pos -im_context.start_pos;
            //data_ptr->aw_field_info.G.dwStateBits |= T9STATEINSERTMASK;

            MMIIM_StartTimer(&(data_ptr->is_timer_active), data_ptr->ctrl_handle);

            //send msg key up to simulator the delete char
            {
                MMIIM_SP_MSG_T msg = {0};
                
                MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYCLEAR);
                TransMsg(data_ptr, &s_last_eventdata_autoswitch, &msg);
                data_ptr->last_down_key = msg.para.key;
                data_ptr->last_key = msg.para.key;
                
                HandleSysMsgMultitap(data_ptr, &s_last_eventdata_autoswitch);
                SCI_MEMSET(&s_last_eventdata_autoswitch, 0x0, sizeof(GUIIM_EVENT_DATA_U));
                s_is_autoswitch_simu_sych = TRUE;
            }
            
        }
#endif
    }

    return sStatus;
}

/*****************************************************************************/
//  Description : auto switch capital from lead to lower
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AutoSwitchCapital(MMIIM_T9_DATA_T *data_ptr)
{
    BOOLEAN is_ret = FALSE;
    GUIIM_CAPITAL_MODE_T cap = GUIIM_MODE_MAX;
    GUIIM_METHOD_PARAM_T * method_param_ptr = PNULL;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] AutoSwitchCapital: data_ptr is null.");
        return is_ret;
    }

    if (1 == data_ptr->cap_auto_switch)
    {
        //need to set method to lower
        //GUIIM_METHOD_T new_method = GUIIM_M_NONE;
        if (MMIIM_T9_CAP_SWITCH_LEAD2LOWER == data_ptr->cap_switch_type)
        {
            if (!(MMIIM_NeedSwitchLeadCap(&data_ptr->init_param)))
            {
                cap = GUIIM_MODE_LOWER;
            }
        }
        else if (MMIIM_T9_CAP_SWITCH_LOWER2LEAD == data_ptr->cap_switch_type)
        {
            //check two chars before the cursor
            if (MMIIM_NeedSwitchLeadCap(&data_ptr->init_param))
            {
                cap = GUIIM_MODE_LEAD_UPPER;
            }
        }
        
        if (GUIIM_MODE_MAX != cap  
            && cap != data_ptr->init_param.method_param_ptr->im_set.capital)
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};
            
            notify_pack.data.switch_capital = cap;
            notify_pack.notify = GUIIM_NOTIFY_SWITCH_CAPITAL;
            GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

            //this is not set method, so app should update value with control
            method_param_ptr = GUIIM_GetImMethodParam(data_ptr->init_param.im_handle);
            if (PNULL != method_param_ptr)
            {
                data_ptr->init_param.method_param_ptr = method_param_ptr; 
            }
 
            //GUIEDIT_DisplayTitleInfo(MMK_GetParentCtrlHandle(data_ptr->init_param.im_handle));
            
            data_ptr->cap_auto_switch = 0;
            MMIIM_T9_SetCapital(data_ptr);	
            
            // set virtual capital
            switch (cap)
            {
            case GUIIM_MODE_LEAD_UPPER:
                data_ptr->cap_auto_switch = 1;
                data_ptr->cap_switch_type = MMIIM_T9_CAP_SWITCH_LEAD2LOWER;
                break;        
            case GUIIM_MODE_LOWER:
                data_ptr->cap_auto_switch = 1;
                data_ptr->cap_switch_type = MMIIM_T9_CAP_SWITCH_LOWER2LEAD;
                break;        
            default:
                data_ptr->cap_auto_switch = 0;
                data_ptr->cap_switch_type = MMIIM_T9_CAP_SWITCH_MAX;
                break;
            }
        }

        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : auto switch method from lead to lower
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AutoSwitchMethod(MMIIM_T9_DATA_T *data_ptr)
{
    BOOLEAN is_ret = FALSE;
    GUIIM_CAPITAL_MODE_T cap = GUIIM_MODE_MAX;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] AutoSwitchMethod: data_ptr is null.");
        return is_ret;
    }

    if (1 == data_ptr->cap_auto_switch)
    {
        //need to set method to lower
        GUIIM_METHOD_T new_method = GUIIM_M_NONE;
        if (MMIIM_T9_CAP_SWITCH_LEAD2LOWER == data_ptr->cap_switch_type)
        {
			//to enable input LeadCap character at the begining of the sentence
			if (!MMIIM_NeedSwitchLeadCap(&data_ptr->init_param))
            {
				cap = GUIIM_MODE_LOWER;
			}
        }
        else if (MMIIM_T9_CAP_SWITCH_LOWER2LEAD == data_ptr->cap_switch_type)
        {
            //check two chars before the cursor
            if (MMIIM_NeedSwitchLeadCap(&data_ptr->init_param))
            {
                cap = GUIIM_MODE_LEAD_UPPER;
            }
        }

        if (GUIIM_MODE_MAX != cap)
        {
            new_method = MMITHEME_IMGetMethodByCapital(
                data_ptr->init_param.method_param_ptr->im_set.method, cap);
        }

        if (GUIIM_M_NONE != new_method 
            && new_method != data_ptr->init_param.method_param_ptr->im_set.method)
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};
            
            notify_pack.data.set_method = new_method;
            notify_pack.notify = GUIIM_NOTIFY_AUTO_SWITCH_METHOD;
            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            
            data_ptr->cap_auto_switch = 0;
        }

        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : Commit char
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void CommitChar(MMIIM_T9_DATA_T *data_ptr, wchar c)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] CommitChar: data_ptr is null.");
        return;
    }

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data.commit[0] = c;
    notify_pack.data.commit_data.commit_len = 1;

    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

    AutoSwitchMethod(data_ptr);

    return;
}

/*****************************************************************************/
//  Description : Commit multitap Last Key value
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 注意本函数只用于multitap输入法,类似abc输入法提交的字母
/*****************************************************************************/
LOCAL BOOLEAN CommitMulLastKey(MMIIM_T9_DATA_T *data_ptr, BOOLEAN *is_commit)
{
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] CommitMulLastKey: data_ptr is null.");
        return FALSE;
    }

    if (data_ptr->last_key >= MMIIM_SP_KEY_MAX || MMIIM_SP_KEY_NONE == data_ptr->last_key)
    {
        SCI_TRACE_LOW("[MMIIM] CommitMulLastKey: data_ptr is null.");
        return FALSE;
    }

    if (MMIIM_SP_KEY_MAX != data_ptr->last_key)
    {
        MMIIM_ClearImeComp(&data_ptr->init_param);

        MMIIM_T9_TimeOut(&data_ptr->aw_field_info.G, T9_EXPL_TIMER_ID);

        AutoSwitchMethod(data_ptr);

        *is_commit = TRUE;
    }

    data_ptr->last_key = MMIIM_SP_KEY_MAX;
    data_ptr->last_key_index = 0;
    data_ptr->last_char = '\0';

    return TRUE;
}

/*****************************************************************************/
//  Description : set the cursor position from editbox to IME
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL T9STATUS SetIMECursorPos(MMIIM_T9_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    uint16 nCurEditPos = 0;

    nCurEditPos = (uint16)MMIIM_GetEditCursor(&data_ptr->init_param);

    //2: update IME cursor position
    sStatus = MMIIM_T9_Cursor(&data_ptr->aw_field_info.G, T9CA_FROMBEG, nCurEditPos);

    return sStatus;
}

/*****************************************************************************/
//  Description : handle press left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLeftKeyMsg(
    MMIIM_T9_DATA_T *data_ptr,
    MMIIM_MOVE_BY_TYPE move_type
)
{
    BOOLEAN is_dealt = FALSE;
    T9FieldInfo *filed_info_ptr = PNULL;
    GUIIM_LANGUAGE_T cur_lang = GUIIM_LANG_NONE;
    T9UINT nPreCursor = 0;
    T9UINT nWordLen = 0;
    T9UINT nCursor = 0;
    T9UINT nBufLen = 0;
    T9STATUS sStatus = T9STATERROR;
    int16 nStep = 0;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

    cur_lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    if (GUIIM_LANG_NONE == cur_lang)
    {
        cur_lang = MMITHEME_IMGetDefaultLang(
            data_ptr->init_param.method_param_ptr->limit);
    }

    filed_info_ptr = &data_ptr->aw_field_info.G;
    if (PNULL == filed_info_ptr)/*lint !e774*/
    {
        return is_dealt;
    }

    //1: get editbox cursor position
    SetIMECursorPos(data_ptr);

    //2: get last position
    nPreCursor = filed_info_ptr->nCursor;

    //3: handle left key
    sStatus = MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYLEFT);

    //4: get current position
    nCursor = filed_info_ptr->nCursor;
    
    //already move to the begining of the buffer, do nothing
    if(0 == nPreCursor && 0 == nCursor)
    {
        //is_dealt = TRUE;
    }
    else if (nPreCursor - nCursor > 0)//reset cursor position
    {
        if (MMIIM_MOVE_WORD_BY_WORD == move_type)
        {
            sStatus = MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYNEXT);

            nWordLen = filed_info_ptr->nWordLen;
            nCursor = filed_info_ptr->nCursor;
            nBufLen = filed_info_ptr->nBufLen;

            if (0 != nWordLen)
            {
                uint16 nIdx = nCursor - nWordLen;

                //commit underline to editbox
                MMIIM_EditorSelectHighlightChars(&data_ptr->init_param, nIdx, nCursor);

                SetToInputing(data_ptr);
            }
            else
            {
                nStep = (nPreCursor - nCursor);
                
                //no word match, just set the cursor
                MMIIM_SetEditCursor(&data_ptr->init_param, GUIIM_SETCURSOR_TYPE_CUR, -nStep);
            }
        }
        else if (MMIIM_MOVE_CHAR_BY_CHAR == move_type)
        {
            nStep = (nPreCursor - nCursor);
            MMIIM_SetEditCursor(&data_ptr->init_param, GUIIM_SETCURSOR_TYPE_CUR, -nStep);
        }

        is_dealt = TRUE;
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : handle press right key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleRightKeyMsg(
    MMIIM_T9_DATA_T *data_ptr,
    MMIIM_MOVE_BY_TYPE move_type
)
{
    BOOLEAN is_dealt = FALSE;
    T9FieldInfo *filed_info_ptr = PNULL;
    GUIIM_LANGUAGE_T cur_lang = GUIIM_LANG_NONE;
    T9UINT nPreCursor = 0;
    T9UINT nCursor = 0;
    int16 nStep = 0;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

    cur_lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    if (GUIIM_LANG_NONE == cur_lang)
    {
        cur_lang = MMITHEME_IMGetDefaultLang(
            data_ptr->init_param.method_param_ptr->limit);
    }

    //1: get editbox cursor position
    SetIMECursorPos(data_ptr);

    if (MMIIM_IsCursorAtTheEnd(&data_ptr->init_param))
    {
        //if (!IM_IsAddWordWinState())
        {
			/***
			 * already move to the end of the buffer, do nothing
             * //MMIIM_T9_AddExplicitChar(&data_ptr->aw_field_info.G, ' ');
             * //CommitChar(data_ptr, ' ');
			***/
            //CheckEditorToSwitchCapital(data_ptr);
        }
        //is_dealt = TRUE;
    }
    else
    {
        //往左移动，不区分按字还是按词，移多少由引擎决定
        filed_info_ptr = &data_ptr->aw_field_info.G;
        if (PNULL == filed_info_ptr)/*lint !e774*/
        {
            return is_dealt;
        }
        nPreCursor = filed_info_ptr->nCursor;

        MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYRIGHT);

        nCursor = filed_info_ptr->nCursor;

        //reset cursor position
        if (nCursor - nPreCursor > 0)
        {
            nStep = (nCursor - nPreCursor);
            
            MMIIM_SetEditCursor(&data_ptr->init_param, GUIIM_SETCURSOR_TYPE_CUR, nStep);
			is_dealt = TRUE;
        }        
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : handle long left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLongLeftKeyMsg(MMIIM_T9_DATA_T *data_ptr)
{
    BOOLEAN is_dealt = FALSE;
    GUIIM_LANGUAGE_T cur_lang = GUIIM_LANG_NONE;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

    cur_lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    if (GUIIM_LANG_NONE == cur_lang)
    {
        cur_lang = MMITHEME_IMGetDefaultLang(
            data_ptr->init_param.method_param_ptr->limit);
    }

    //clear comp
    MMIIM_ClearImeComp(&data_ptr->init_param);

    //set cursor at the begin position
    MMIIM_SetEditCursor(&data_ptr->init_param, GUIIM_SETCURSOR_TYPE_START, 0);
    SetIMECursorPos(data_ptr);

    is_dealt = TRUE;
    return is_dealt;
}

/*****************************************************************************/
//  Description : handle long left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLongRightKeyMsg(MMIIM_T9_DATA_T *data_ptr)
{
    BOOLEAN is_dealt = FALSE;
    GUIIM_LANGUAGE_T cur_lang = GUIIM_LANG_NONE;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

    cur_lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    if (GUIIM_LANG_NONE == cur_lang)
    {
        cur_lang = MMITHEME_IMGetDefaultLang(
            data_ptr->init_param.method_param_ptr->limit);
    }

    //clear comp
    MMIIM_ClearImeComp(&data_ptr->init_param);

    //set cursor at the end position
    MMIIM_SetEditCursor(&data_ptr->init_param, GUIIM_SETCURSOR_TYPE_END, 0);
    SetIMECursorPos(data_ptr);

    is_dealt = TRUE;
    return is_dealt;
}

/*****************************************************************************/
//  Description : handle long number(0~9) key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLongNumberKeyMsg(
    MMIIM_T9_DATA_T *data_ptr,
    MMIIM_SP_VKEY_T key
)
{
    BOOLEAN is_dealt = FALSE;
    wchar commit_char = 0;
    GUIIM_LANGUAGE_T cur_lang = GUIIM_LANG_NONE;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

    if (MMIIM_SP_KEY_0 > key
        || MMIIM_SP_KEY_9 < key)
    {
        return is_dealt;
    }

    cur_lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    if (GUIIM_LANG_NONE == cur_lang)
    {
        cur_lang = MMITHEME_IMGetDefaultLang(
            data_ptr->init_param.method_param_ptr->limit);
    }

    is_dealt = TRUE;

    if (MMIIM_SP_STATE_NOINPUT == data_ptr->state)
    {
        T9AWFieldInfo *aw_info_ptr = &data_ptr->aw_field_info;
        T9UINT nWordLen = 0;

        nWordLen = aw_info_ptr->G.nWordLen;
        if (1 == nWordLen
            && MMIIM_Editor_IsShadeState())
        {
            //之前有阴影的字，则替换成数字
            MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYCLEAR);
            MMIIM_ClearImeComp(&data_ptr->init_param);
            CommitChar(data_ptr, '\b');
        }

        commit_char = key - MMIIM_SP_KEY_0 + '0';

        MMIIM_T9_AddExplicitChar(&data_ptr->aw_field_info.G, commit_char);
        CommitChar(data_ptr, commit_char);
    }
    else if (MMIIM_SP_STATE_INPUTING == data_ptr->state)
    {
        MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYRIGHT);

        MMIIM_ClearImeComp(&data_ptr->init_param);

        commit_char = key - MMIIM_SP_KEY_0 + '0';

        MMIIM_T9_AddExplicitChar(&data_ptr->aw_field_info.G, commit_char);

        CommitChar(data_ptr, commit_char);

        SetToNoInput(data_ptr);

#ifdef UI_MULTILAYER_SUPPORT
        MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
        MMK_UpdateScreen();
#endif
    }

    data_ptr->last_key = MMIIM_SP_KEY_NONE;
    return is_dealt;
}

/*****************************************************************************/
//  Description : handle cancel key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleCancelKeyMsg(MMIIM_T9_DATA_T *data_ptr)
{
    BOOLEAN is_dealt = FALSE;
    T9FieldInfo *filed_info_ptr = PNULL;
    T9STATUS sStatus = T9STATERROR;

    if (PNULL == data_ptr ||
        PNULL == data_ptr->init_param.method_param_ptr)
    {
        return is_dealt;
    }

    filed_info_ptr = &data_ptr->aw_field_info.G;
    if (MMIIM_SP_STATE_NOINPUT == data_ptr->state)
    {
#if 0   //ajay context word should not be highlighted while pressing clear key 1st time.
        GUIIM_TYPE_T mode_type = data_ptr->init_param.method_param_ptr->im_set.type;
        if (mode_type == GUIIM_TYPE_SMART || mode_type == GUIIM_TYPE_ENGLISH)
        {
            //The buf string is not any change, when delete by user in no input
            sStatus = T9AWActivateWordLeft(&data_ptr->aw_field_info);
        }
        else
        {
            sStatus = T9STATERROR;
        }
#endif
        if (T9STATNONE != sStatus)/*lint !e774*/
        {
        	//We shouldn't call T9 engine to handle delete msg if buffer is empty already.
        	if(0 != filed_info_ptr->nBufLen)
        	{
	            //can't active a word succuss, that meanings just delete the char before.
	            sStatus = MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYCLEAR);
	            if (T9STATNONE == sStatus)
	            {
	                CommitChar(data_ptr, '\b');

	                is_dealt = TRUE;
	            }
	            else
	            {
	                is_dealt = FALSE;
	            }
        	}
        	else
        	{
        	    is_dealt = FALSE;
        	}
        }
#if 0   //ajay context word should not be highlighted while pressing clear key 1st time.	
        else
        {
            //active a word don't change the cusor, just set the word len and nCurSelObj
            T9UINT nWordLen = 0;
            T9UINT nCursor = 0;
            uint16 nIdx = 0;

            nWordLen = filed_info_ptr->nWordLen;
            nCursor = filed_info_ptr->nCursor;            
            nIdx = nCursor - nWordLen;

            //commit underline to editbox
            MMIIM_EditorSelectHighlightChars(&data_ptr->init_param, nIdx, nCursor);

            SetToInputing(data_ptr);
            is_dealt = TRUE;
        }
#endif
    }
    else if (MMIIM_SP_STATE_INPUTING == data_ptr->state)/*lint !e774*/
    {
        {
            T9UINT nWordLen = 0;
            T9UINT nCursor = 0;

            if (PNULL == filed_info_ptr)/*lint !e774*/
            {
                return is_dealt;
            }
            nCursor = filed_info_ptr->nCursor;

            MMIIM_T9_HandleKey(&data_ptr->aw_field_info.G, T9KEYCLEAR);

            nWordLen = filed_info_ptr->nWordLen;
            nCursor = filed_info_ptr->nCursor;

            {
                T9SYMB *text_ptr = PNULL;
                uint16 nCmtLen = 0;
                uint16 nUnderLen = 0;
                uint16 nIdx = 0;

                if (0 == nWordLen)
                {
                    SetToNoInput(data_ptr);

#ifdef UI_MULTILAYER_SUPPORT
                    MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
                    MMK_UpdateScreen();
#endif

                    nUnderLen = 0;
                    nCmtLen = 0;
                }
                else
                {
                    nUnderLen = nWordLen;
                    nCmtLen = 0;
                    nIdx = nCursor-nWordLen;

                    text_ptr = &(filed_info_ptr->psTxtBuf[nIdx]);
                }

                MMIIM_ReplaceChars(&data_ptr->init_param, text_ptr, nCmtLen, nUnderLen);
            }

            is_dealt = TRUE;
        }
    }
    

    return is_dealt;
}


/*****************************************************************************/
//  Description : create add new word window
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateAddNewWordWin(MMIIM_T9_DATA_T *data_ptr)
{
	MMI_STRING_T title = {0};
	MMI_STRING_T *content_str_ptr = PNULL;
	MMI_STRING_T edit_str = {0};
    GUIEDIT_HIGHLIGHT_T highlight = {0};
	MMI_HANDLE_T edit_ctrl_handle = PNULL;
    MMI_WIN_ID_T win_id = MMIIM_ADD_WORD_WIN_ID;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] CreateAddNewWordWin: data_ptr is null.");
        return;
    }

	edit_ctrl_handle = MMK_GetParentCtrlHandle(data_ptr->init_param.im_handle);

	GUIEDIT_GetString(edit_ctrl_handle, &edit_str);
	GUIEDIT_GetHighlightData(edit_ctrl_handle, &highlight);

    if (edit_str.wstr_len && highlight.is_underline)
    {		
		content_str_ptr = (MMI_STRING_T *)SCI_ALLOC_APP(sizeof(MMI_STRING_T));
		content_str_ptr->wstr_len = highlight.end_pos - highlight.start_pos;
		content_str_ptr->wstr_ptr = (wchar*)SCI_ALLOC_APP((content_str_ptr->wstr_len + 1) * sizeof(wchar));
		//edit_str.wstr_len = highlight.end_pos - highlight.start_pos;
		
        //edit_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((edit_str.wstr_len + 1) * sizeof(wchar));
        SCI_MEMSET(content_str_ptr->wstr_ptr, 0x0, (content_str_ptr->wstr_len + 1)*sizeof(wchar));
        MMIAPICOM_Wstrncpy(content_str_ptr->wstr_ptr, edit_str.wstr_ptr + highlight.start_pos, content_str_ptr->wstr_len);
		
        GUIEDIT_DelHighlightChars(edit_ctrl_handle);
    }

	if(MMK_IsOpenWin(MMIIM_ADD_WORD_WIN_ID))
	{
		MMK_CloseWin(MMIIM_ADD_WORD_WIN_ID);
	}
	
	MMI_GetLabelTextByLang(STR_INPUT_NEW_WORD_EXT01, &title);
	MMIPUB_OpenDialogInputFieldWinByTextPtrEx(
		MMK_GetFirstAppletHandle(),
		&title , 
		&win_id,
		PNULL,
		MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,
		HandleAddNewWordWinMsg,
		(uint32)(content_str_ptr)
		);
	MMK_SetWinUserData(win_id, (void*)data_ptr);

    return;
}

/*****************************************************************************/
// Description : HandleAddNewWordWinMsg
// Global resource dependence :
// Author: haiwu.chen
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAddNewWordWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T pre_edit_ctrl_handle = PNULL;
    MMI_CTRL_ID_T edit_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID);
    MMI_STRING_T edit_string = {0};
	GUI_RECT_T bg_rect = {0};
	GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	MMIIM_T9_DATA_T *data_ptr = PNULL;
	MMI_STRING_T *content_str_ptr = PNULL;
    MMIPUB_INFO_T   *win_info_ptr = (MMIPUB_INFO_T*)MMK_GetWinAddDataEx(win_id);
    data_ptr = (MMIIM_T9_DATA_T *)MMK_GetWinUserData(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //MMIAPIIM_SetWordPredictiveState(FALSE);

            GUIEDIT_SetTextMaxLen(edit_ctrl_id, T9_MAX_ADD_WORD_LEN, T9_MAX_ADD_WORD_LEN);
			MMIPUB_SetInputDialogImType(win_id,  GUIIM_TYPE_MULTITAP | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_MULTITAP, GUIIM_TAG_NONE);
            GUIEDIT_SetStyle(edit_ctrl_id, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetHandleRedKey(TRUE, edit_ctrl_id);
            GUIEDIT_PermitBorder(edit_ctrl_id,TRUE);

			if (PNULL != data_ptr)
			{
				content_str_ptr = (MMI_STRING_T *)win_info_ptr->user_data;
				if (content_str_ptr)
				{
					if (content_str_ptr->wstr_ptr)
					{
						GUIEDIT_SetString(edit_ctrl_id, content_str_ptr->wstr_ptr, content_str_ptr->wstr_len);
						SCI_FREE(content_str_ptr->wstr_ptr);
						content_str_ptr->wstr_ptr = PNULL;
					}
					SCI_FREE(content_str_ptr);
					content_str_ptr = PNULL;
				}
			}
			
            MMK_SetAtvCtrl(win_id, edit_ctrl_id);
        }
        break;

	case MSG_FULL_PAINT:
		{
			if(MMITHEME_IsMainScreenLandscape())
			{
				bg_rect = both_client_rect.h_rect;
			}
			else
			{
				bg_rect = both_client_rect.v_rect;
			}
			LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
		}
		break;

    case MSG_CTL_PENOK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
		MMK_CloseWin(win_id);
		break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        // to get the content which the user input
        GUIEDIT_GetString(edit_ctrl_id, &edit_string);

        if (0 < edit_string.wstr_len)
        {
            if (PNULL != data_ptr)
            {
                MMIIM_T9_AddUdbCustomWord(data_ptr, edit_string.wstr_ptr, edit_string.wstr_len);
				pre_edit_ctrl_handle = MMK_GetParentCtrlHandle(data_ptr->init_param.im_handle);

				// insert the new content from spell window
				GUIEDIT_InsertString(pre_edit_ctrl_handle, edit_string.wstr_ptr, edit_string.wstr_len);
            }
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        //MMIAPIIM_SetWordPredictiveState(TRUE);
        result = MMI_RESULT_FALSE;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// Description : SendAddWordContent
// Global resource dependence :
// Author: haiwu.chen
// Note:
/*****************************************************************************/
LOCAL void SendAddWordContent(
    MMIIM_T9_ADD_WORD_T *t9_addword_ptr,
    wchar *pBuf,
    uint16 nBufLen
)
{
    wchar *pNewBuf = PNULL;
    if (PNULL == t9_addword_ptr || PNULL == pBuf || 0 == nBufLen)
    {
        return;
    }

    pNewBuf = (wchar*)SCI_ALLOC_APP((nBufLen+1)*sizeof(wchar));
    if (PNULL == pNewBuf)
    {
        return;
    }
    SCI_MEMSET(pNewBuf, 0x0, (nBufLen+1)*sizeof(wchar));

    MMIAPICOM_Wstrncpy(pNewBuf, pBuf, nBufLen);

    GUIEDIT_InsertString(t9_addword_ptr->edit_ctrl_handle, pNewBuf, nBufLen);

    SCI_FREE(pNewBuf);
}

/*****************************************************************************/
//  Description : get im reload count
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL uint8 GetImReloadCnt(void)
{
    return s_im_reload_cnt;
}

/*****************************************************************************/
//  Description : set im reload count
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetImReloadCnt(uint8 im_reload_cnt)
{
    s_im_reload_cnt = im_reload_cnt;
}

/*****************************************************************************/
//  Description : plus reload count
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AddImReloadRefCnt(void)
{
    s_im_reload_cnt++;
}

/*****************************************************************************/
//  Description : minus reload count
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void ReleaseImReloadRefCnt(void)
{
    if (0 != s_im_reload_cnt)
    {
        s_im_reload_cnt--;
    }
}

/*****************************************************************************/
//  Description : is im reload happan
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsImReloadDirty(void)
{
    return s_is_im_reload;
}

/*****************************************************************************/
//  Description : set reload flag
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetImReloadDirty(BOOLEAN is_dirty)
{
    s_is_im_reload = is_dirty;
}

/*****************************************************************************/
//  Description : re-entry input method
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReEntryIm(MMIIM_T9_DATA_T *data_ptr)
{
    GUIIM_METHOD_T cur_method = GUIIM_M_NONE;
    BOOLEAN is_dealt = FALSE;
    T9STATUS sStatus = T9STATERROR;
    
    if (IsImReloadDirty() || 0 == MMIIM_GetImKerState())
    {
        cur_method = data_ptr->init_param.method_param_ptr->im_set.method;
        if (GUIIM_M_123 == cur_method)
        {
            sStatus = T9STATNONE;
        }
        else
        {
            SetImReloadDirty(FALSE);
            sStatus = InitImKernel(data_ptr);
        }

        if (T9STATNONE != sStatus)
        {
            SCI_TRACE_LOW("[MMIIM] Initialize re-entry InitT9Core fail!");
            MMIIM_T9_Ter_Alpha(data_ptr);
            SCI_FREE(data_ptr);
            return is_dealt;
        }
        else
        {
            SetMethod(data_ptr, cur_method);
            SCI_TRACE_LOW("[MMIIM] Initialize re-entry InitT9Core method=%04X", cur_method);
        }
        is_dealt = TRUE;
    }

    return is_dealt;
}
/*****************************************************************************/
//  Description : Get all predicted words
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPredictWord(MMIIM_T9_DATA_T *data_ptr, MMIIM_SP_VKEY_T key,uint16 nIdx)
{
    int i = 0;
    T9UINT nNextSelObj = 0;
    T9STATUS sStatus = T9STATERROR;
    T9UINT nCurSelObj = 0;
    T9FieldInfo *field_info_ptr = PNULL;

    field_info_ptr = &data_ptr->aw_field_info.G;
    nCurSelObj = field_info_ptr->nCurSelObj;

    if(TRUE == data_ptr->is_spell_state)
    {
        data_ptr->cand_win.cur_sel = data_ptr->cand_win.count + 1;
    }
    else
    {
        data_ptr->cand_win.cur_sel = nCurSelObj;
    }

    // Force the candidate bar flash every time.
    data_ptr->is_predict_bar_dirty = TRUE;

    if((MMIIM_SP_KEY_UP == key) || (MMIIM_SP_KEY_DOWN == key) || (MMIIM_SP_KEY_RIGHT == key) || (MMIIM_SP_KEY_OK == key))
    {
        return TRUE;
    }

    data_ptr->cand_win.count = 1;
    memset(data_ptr->cand_win.buffer[0],0x00,MMIIM_WIN_BUFFER_LEN);
    memcpy(data_ptr->cand_win.buffer[0],&field_info_ptr->psTxtBuf[nIdx],field_info_ptr->nWordLen * sizeof(T9SYMB));

    for(i = 1; i < MMIIM_CAND_COUNT_MAX; i++)
    {
        /*Cycle all predicted words*/
        sStatus = MMIIM_T9_HandleKey(field_info_ptr, T9KEYNEXT);
        nNextSelObj = field_info_ptr->nCurSelObj;
        if(nNextSelObj != nCurSelObj)
        {
            memset(data_ptr->cand_win.buffer[i],0x00,MMIIM_WIN_BUFFER_LEN);
            memcpy(data_ptr->cand_win.buffer[i],&field_info_ptr->psTxtBuf[nIdx],field_info_ptr->nWordLen * sizeof(T9SYMB));
            data_ptr->cand_win.count++;
        }
        else
        {
             break;
        }
    }
    data_ptr->is_predict_bar_dirty = TRUE;

    return TRUE;
}

/******************************************************************************/
// Description: reset to no input state and clean the underline string if needed
// Global resource dependence: NONE
// Author: arvin.zhang
// Note: called under the scenes : LOSE_FOCUS,HASH_KEY(to switch im)
/******************************************************************************/
LOCAL void ResetToNoInput(MMIIM_T9_DATA_T *data_ptr , BOOLEAN is_need_clear_underline)
{
    if (PNULL != data_ptr)
    {
    	if (is_need_clear_underline)
    	{
    		GUIIM_CONTEXT_T im_context = {0};
    		MMIIM_GetEditContent(&data_ptr->init_param, &im_context);

    		if (PNULL != im_context.str_ptr && im_context.is_underline)
    		{	
    		    CommitChar(data_ptr, ' ');
    		    CommitChar(data_ptr, '\b');
    		}
    	}

    	SetToNoInput(data_ptr);
    	//MMIIM_PopupListClearLayer(&data_ptr->predict_bar);
#ifdef UI_MULTILAYER_SUPPORT
        MMIIM_PopupListReleaseLayer(&data_ptr->predict_bar);
#else
        MMK_DelayUpdateScreen();
#endif
        return;
    }
}

/*****************************************************************************
* Function name : UnicodeToGSM
* Description : Convert Unicode character code to GSM supported
* character code
*
* Return : Unicode encoded GSM character
*
* Pass : T9SYMB sUnicode -- Code to be translated
*****************************************************************************/
T9U16 UnicodeToGSM(T9SYMB sUnicode)
{
	T9U16 nIndex = 0;
	T9U16 codeConverted = 0x0000;

	if (PNULL != s_conversion_table_ptr)
	{
		while (((*s_conversion_table_ptr)[nIndex][0] != 0xFFFF) && ((*s_conversion_table_ptr)[nIndex][0] != sUnicode))
		{
			nIndex++;
		}
		codeConverted = (*s_conversion_table_ptr)[nIndex][1];
	}

	return codeConverted;

} /* UnicodeToGSM() */

/*------------------------------------------------------------------------
* Function name : T9ConvertChar
* Description :   T9 sends a new character to the host to allow the
*                 host to reject it or convert to a different character.
*
* Pass :  T9FieldInfo *pFieldInfo
*         T9SYMB *psConvertChar - character to be converted
*
* Return: T9STATNONE if successful (symbol is okay or it has been replaced)
*         T9STAT_NO_CHAR if symbol cannot be displayed or replaced by another
*         T9STATERROR if another error prevents character conversion
*-----------------------------------------------------------------------*/
T9STATUS T9FARCALL T9ConvertChar(T9FieldInfo *pFieldInfo, T9SYMB *psConvertChar)
{
	T9STATUS mStatus = T9STATNONE;
	T9SYMB sConverted = (T9SYMB)0xFFFF;
	T9SYMB preCharacter = 0;
	T9UINT langPID = 0;

	if (PNULL == pFieldInfo)
	{
		return T9STATERROR;
	}

	/*------------------------------------------------------------------------------------
	* designed logic: we should only check and adjust the key conversion table(based on
	* the previous char before cursor) on the first typing character of every word. This
	* is mainly because it's very necessary to limit the input on the initial of input so
	* as to avoid to type the independent character alone and then we'd better let engine
	* take over the following input completely to ensure all valid candidates can be found.
	*-------------------------------------------------------------------------------------*/
	s_conversion_table_ptr = PNULL;
	langPID = (T9UINT)(pFieldInfo->nLdbNum & T9PIDMASK);

	if(pFieldInfo->nWordLen == 1)
	{
		/*--------------------------------------------------------------
		* Amharic is a special language in which it lists out all the 
		* combinations of consonant + vowel pairs in its Unicode table.
		* And There's no single vowel character available at all so that
		* we should never show it without a valid consonant previously. 
		* Hence, we should forbid to type any vowel at the beginning of
		* every word even if an consonant had been committed just before!
		*--------------------------------------------------------------*/
		if ((pFieldInfo->nCursor > 1) && (T9PIDAmharic != langPID))
		{
			preCharacter = pFieldInfo->psTxtBuf[pFieldInfo->nCursor - 2];
		}
		s_conversion_table_ptr = MMIIM_CheckConversionTable(langPID, preCharacter);
	}

	sConverted = (T9SYMB)UnicodeToGSM(*psConvertChar);
	/* if the character cannot be displayed/transmitted */
	if (sConverted == 0xFFFF) {
		mStatus = T9STAT_NO_CHAR;
	}
	/* else the character is okay or is replaced */
	else if (0x0000 != sConverted) {
		*psConvertChar = sConverted;
	}

	return mStatus;
}

#ifdef __cplusplus
    }
#endif
