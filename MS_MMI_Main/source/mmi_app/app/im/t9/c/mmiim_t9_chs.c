/*****************************************************************************
 ** File Name:      mmiim_t9_chs.c                                                *
 ** Author:         haiwu.chen                                                *
 ** Date:           4/6/2012                                                *
 ** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file is used to process t9 text input                *
 ******************************************************************************
 **                         Important Edit History                            *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/2012        haiwu.chen            Create                               *
 ** only used for chinese language                                              *
 ******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)

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
#include "mmiim_t9_chs.h"
#include "mmiim_t9_func.h"

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
        MMIIM_T9_CHS_DATA_T *data_ptr
        );

/******************************************************************************/
// Description: 初始化内核
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL T9STATUS InitImKernel(
        MMIIM_T9_CHS_DATA_T *data_ptr
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
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr,
        MMIIM_SP_MSG_T *msg_ptr
        );

/******************************************************************************/
// Description: ChangeRect
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN ChangeRect(MMIIM_T9_CHS_DATA_T *data_ptr);

/******************************************************************************/
// Description: DrawUI
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void DrawUI(MMIIM_T9_CHS_DATA_T *data_ptr);

/******************************************************************************/
// Description: ReDrawUI
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void ReDrawUI(MMIIM_T9_CHS_DATA_T *data_ptr);

/******************************************************************************/
// Description: HandleSysMsgNoInput
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgNoInput(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        );

/******************************************************************************/
// Description: HandleSysMsgInputing
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgInputing(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        );

/******************************************************************************/
// Description: HandleSysMsgSelecting
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgSelecting(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        );

/******************************************************************************/
// Description: HandleSysMsgInputSelecting
// Global resource dependence: NONE
// Author: arvin
// Note: Special test for Stroke Mode
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgInputSelecting(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        );

/******************************************************************************/
// Description: HandleSysMsgAssociate
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgAssociate(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        );

/*****************************************************************************/
//  Description : set to no input state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToNoInput(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : set to samsung inputing state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToInputing(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : set to selecting state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToSelecting(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : set to associate state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToAssociate(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : sychronization t9 engine content and curson with editbox 
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL T9STATUS SychT9WithEdit(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : Commit char
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void CommitChar(MMIIM_T9_CHS_DATA_T *data_ptr, wchar c);

/*****************************************************************************/
//  Description : set the cursor position from editbox to IME
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL T9STATUS SetIMECursorPos(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle press left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLeftKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_MOVE_BY_TYPE move_type
        );

/*****************************************************************************/
//  Description : handle press right key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleRightKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_MOVE_BY_TYPE move_type
        );

/*****************************************************************************/
//  Description : handle t9 horizontal direction key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: not relate to the cursor in editbox, just handle by input method with smart mode
/*****************************************************************************/
PUBLIC BOOLEAN HandleHorDirectKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_SP_VKEY_T key
        );

/*****************************************************************************/
//  Description : handle t9 vertical direction key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: not relate to the cursor in editbox, just handle by input method with smart mode
/*****************************************************************************/
PUBLIC BOOLEAN HandleVerDirectKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_SP_VKEY_T key
        );

/*****************************************************************************/
//  Description : handle long left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: only work in noinput state
/*****************************************************************************/
LOCAL BOOLEAN HandleLongLeftKeyMsg(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle long left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: only work in noinput state
/*****************************************************************************/
LOCAL BOOLEAN HandleLongRightKeyMsg(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle long number(0~9) key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLongNumberKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_SP_VKEY_T key
        );

/*****************************************************************************/
//  Description : handle cancel key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleCancelKeyMsg(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : move hot char in candidate bar
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: direction 0: left, 1:right
/*****************************************************************************/
LOCAL BOOLEAN MoveStepsOnCandBar(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        uint16 direction
        );

/*****************************************************************************/
//  Description : get the identify char from engin
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSelectCharFromEngin(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        uint16 key_index,
        T9SYMB *cmt_char_ptr
        );

/******************************************************************************/
// Description:
// Global resource dependence: NONE
// Author: arvin.zhang
// Note: only consider the single textbar case and user should modify the rect
// if the editbar and candbar exists at the same time
/******************************************************************************/
LOCAL void AdjustTextBarRect(
        GUI_RECT_T *cursor_rect_ptr,
        MMIIM_TEXT_BAR_T *textbar_ptr,
        BOOLEAN is_need_border
        );

/******************************************************************************/
// Description: Config the position of EditBar and CandBar on different state
// Global resource dependence: none
// Author: arvin.zhang
// Note: EditBar's width is dynamic changing and CandBar's width is fixed!!!
/******************************************************************************/
LOCAL void UpdateTextBarsPosition(MMIIM_T9_CHS_DATA_T *data_ptr, BOOLEAN is_need_border);

/******************************************************************************/
// Description: reset to no input state and clean the underline string if needed
// Global resource dependence: NONE
// Author: arvin.zhang
// Note: called under the scenes : LOSE_FOCUS,HASH_KEY(to switch im)
/******************************************************************************/
LOCAL void ResetToNoInput(MMIIM_T9_CHS_DATA_T *data_ptr);

/******************************************************************************/
// Description: ConfigCandBarPosition
// Global resource dependence: NONE
// Author: cheney.wu
// Note:
/******************************************************************************/
PUBLIC void MMIIM_UpdateCandBarPosition(MMIIM_T9_CHS_DATA_T *data_ptr);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
GUIIM_KERNEL_APIS_T const g_guiim_t9_chs_apis =
{
    Initialize,
    Terminate,
    HandleMsg,
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
    MMIIM_T9_CHS_DATA_T *data_ptr = PNULL;

    SCI_TRACE_LOW("[MMIIM] Initialize chs.");

    if(PNULL == param_ptr)
    {
        return data_ptr;
    }

    SCI_TRACE_LOW("[MMIIM] Initialize T9: win_id=%d. s_t9_ldb_max_size=%d", param_ptr->win_id,MMIIM_T9GetMaxLdbSize());

    data_ptr = (MMIIM_T9_CHS_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_T9_CHS_DATA_T));
    SCI_MEMSET(data_ptr, 0, sizeof(MMIIM_T9_CHS_DATA_T));

    //data
    InitData(param_ptr, data_ptr);

    //kernel
    sStatus = InitImKernel(data_ptr);

    if (T9STATNONE != sStatus)
    {
        SCI_TRACE_LOW("MMIIM Initialize InitT9Core fail!");
        MMIIM_T9_Ter_Chs(data_ptr);
        SCI_FREE(data_ptr);
        return PNULL;
    }
    else
    {
        SetMethod(data_ptr, data_ptr->init_param.method_param_ptr->im_set.method);
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
    MMIIM_T9_CHS_DATA_T *data_ptr = (MMIIM_T9_CHS_DATA_T*)handle;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] Terminate.");
        return FALSE;
    }

    SCI_TRACE_LOW("[MMIIM] Terminate, method=%04X", data_ptr->init_param.method_param_ptr->im_set.method);

    MMIIM_T9_Ter_Chs(data_ptr);
    MMIIM_SetImKerState(0);
    MMIIM_ResetMultiLineStyle();
#ifdef UI_MULTILAYER_SUPPORT		
    MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
    MMIIM_PopupListReleaseLayer(&data_ptr->bar_cand);
#endif
    SCI_FREE(data_ptr);
    data_ptr = PNULL;
#ifndef UI_MULTILAYER_SUPPORT	
    MMK_DelayUpdateScreen();
#endif
    return TRUE;
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
    MMIIM_T9_CHS_DATA_T *data_ptr = (MMIIM_T9_CHS_DATA_T*)handle;
    BOOLEAN is_dealt = FALSE;
    BOOLEAN result = FALSE;
    GUI_BOTH_RECT_T pop_menu_rect = {0};
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
                    is_dealt = HandleSysMsgNoInput(data_ptr, event_data_ptr);
                    break;

                case MMIIM_SP_STATE_INPUTING:
                    // Handle the Stroke mode specially for there's no editbar
                    if (GUIIM_TYPE_SMART == mode_type)
                    {
                        is_dealt = HandleSysMsgInputing(data_ptr, event_data_ptr);
                        break;
                    }
                case MMIIM_SP_STATE_SELECTING:
                    if (GUIIM_TYPE_SMART == mode_type)
                    {
                        is_dealt = HandleSysMsgSelecting(data_ptr, event_data_ptr);
                    }
                    else if (GUIIM_TYPE_STROKE == mode_type)
                    {
                        is_dealt = HandleSysMsgInputSelecting(data_ptr, event_data_ptr);
                    }
                    break;

                case MMIIM_SP_STATE_ASSOCIATE:
                    is_dealt = HandleSysMsgAssociate(data_ptr, event_data_ptr);
                    break;

                default:
                    break;
            }

            if (is_dealt)
            {
                ReDrawUI(data_ptr);
                result = is_dealt;
            }
#if 1
#if 0//UNISOC_MMI_Delete
            else if ((PNULL != event_data_ptr)&&(MSG_APP_STAR == event_data_ptr->sys_msg.msg_id)&&(mode_type != GUIIM_TYPE_DIGITAL))
            {
                if (GUIIM_TYPE_DIGITAL != mode_type)
                {
                    MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_SP_SYMB_TYPE_CHS);
                    result = TRUE;
                }
            }
#endif
            else if ((PNULL != event_data_ptr) && (MSG_APP_HASH == event_data_ptr->sys_msg.msg_id))
            {
                ResetToNoInput(data_ptr);
                MMIIM_SwitchingIM(&(data_ptr->init_param)); // for [MS]
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

        case GUIIM_EVENT_SLEEP:
            {
                //ResetToNoInput(data_ptr);
                result = TRUE;
            }
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
                    event_data_ptr->edit_state = GUIIM_EDIT_INPUTING;
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
// Description: init chinese data_ptr value
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL void InitData(
        GUIIM_INIT_KERNEL_T *param_ptr,
        MMIIM_T9_CHS_DATA_T *data_ptr
        )
{
    uint8 width_label = 0;
    GUI_RECT_T win_rect = {0};
    GUI_RECT_T rect_null = {0}; 

    if(PNULL == param_ptr || PNULL == data_ptr)
    {
        return;
    }

    data_ptr->ctrl_handle = param_ptr->im_handle;
    data_ptr->init_param = *param_ptr;
    data_ptr->state = MMIIM_SP_STATE_NOINPUT;

    data_ptr->lcd_info_ptr = data_ptr->init_param.lcd_info_ptr;

    SetToNoInput(data_ptr);

    win_rect = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle);

    width_label = GUI_GetFontWidth(g_mmiim_ui_config.font_lable, UNICODE_ASCII);

    //rect0
    data_ptr->bar_rect_0 = win_rect;
    data_ptr->bar_rect_0.top = (int16)(data_ptr->bar_rect_0.bottom - MMIIM_TEXT_BAR_CAND_HEIGHT + 1);

    //rect1
    data_ptr->bar_rect_1 = win_rect;
    data_ptr->bar_rect_1.bottom = (int16)(data_ptr->bar_rect_0.top  - 1);
    data_ptr->bar_rect_1.top = (int16)(data_ptr->bar_rect_1.bottom - MMIIM_TEXT_BAR_EDIT_HEIGHT + 1);

    //cand bar
    MMIIM_TextBarInit(&(data_ptr->bar_cand));
    data_ptr->bar_cand.win_id = data_ptr->init_param.win_id;
    //data_ptr->bar_cand.lcd_info_ptr = data_ptr->lcd_info_ptr;
    data_ptr->bar_cand.rect = data_ptr->bar_rect_0;
    data_ptr->bar_cand.width_label = width_label;
    data_ptr->bar_cand.width_space = MMIIM_TEXT_BAR_SPACE;
    data_ptr->bar_cand.has_hot = 1;

    //edit bar
    MMIIM_TextBarInit(&(data_ptr->bar_edit));
    data_ptr->bar_edit.win_id = data_ptr->init_param.win_id;
    data_ptr->bar_edit.lcd_dev_info.block_id = data_ptr->lcd_info_ptr->block_id;
    data_ptr->bar_edit.lcd_dev_info.lcd_id = data_ptr->lcd_info_ptr->lcd_id;
    data_ptr->bar_edit.width_space = MMIIM_TEXT_EDIT_BAR_SPACE;
    data_ptr->bar_edit.rect = data_ptr->bar_rect_1;
    MMIIM_InitIdcRect(&data_ptr->bar_edit);

    data_ptr->rect = win_rect;

    return;
}

/******************************************************************************/
// Description: init t9 engine
// Global resource dependence: NONE
// Author: haiwu.chen
// Note: 
/******************************************************************************/
LOCAL T9STATUS InitImKernel(
        MMIIM_T9_CHS_DATA_T *data_ptr
        )
{    
    T9STATUS sStatus = T9STATERROR;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] InitImKernel : data_ptr is null.");
        return sStatus;
    }

    sStatus = MMIIM_T9_Init_Chs(data_ptr);
    /*Assert if t9 return error.For monkey test ctrlim assert issue in trad-chinese mode.----temp added*/
    if(T9STATNONE != sStatus)
    {
        SCI_Sleep(2000);
        SCI_PASSERT(0, ("InitImKernel sStatus:%d", sStatus));
    } 

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
    MMIIM_T9_CHS_DATA_T *data_ptr = (MMIIM_T9_CHS_DATA_T*)handle;    
    T9STATUS sStatus = T9STATNONE;

    if(PNULL == handle || GUIIM_M_NONE == method)
    {
        SCI_TRACE_LOW("[MMIIM] SetMethod: handle is null, method=%04X.",method);
        return FALSE;
    }

    data_ptr->method = method;

    SCI_TRACE_LOW("[MMIIM] SetMethod: method=%04X.", method);

    // set mode
    if (sStatus == T9STATNONE)/*lint !e774*/
    {
        sStatus = MMIIM_T9_SetMode_Chs(data_ptr);
    }

    // insert string to t9core which is already exist in editbox.
    if (sStatus == T9STATNONE)
    {
        GUIIM_CONTEXT_T im_context = {0};

        MMIIM_GetEditContent(&data_ptr->init_param, &im_context);

        if (PNULL != im_context.str_ptr && 0 != im_context.str_len)
        {
            MMIIM_T9_ResetTextBuf(&data_ptr->cc_field_info.G);
            MMIIM_T9_AddTextString(
                    &data_ptr->cc_field_info.G, im_context.str_ptr, im_context.str_len);
        }
    }

    SCI_TRACE_LOW("[MMIIM] SetMethod sStatus=%d", sStatus);

    return (sStatus == T9STATNONE);
}

/******************************************************************************/
// Description: TransMsg
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void TransMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
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
// Description: ChangeRect
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN ChangeRect(MMIIM_T9_CHS_DATA_T *data_ptr)
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
                data_ptr->rect.left = data_ptr->bar_edit.rect.left;
                data_ptr->rect.top = data_ptr->bar_edit.rect.top;
                data_ptr->rect.right = data_ptr->bar_cand.rect.right;
                data_ptr->rect.bottom = MAX(data_ptr->bar_edit.rect.bottom, data_ptr->bar_cand.rect.bottom);
                break;
            case MMIIM_SP_STATE_ASSOCIATE:
                data_ptr->rect = data_ptr->bar_cand.rect;
                //data_ptr->rect.top = data_ptr->bar_edit.rect.top;
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
// Description: DrawUI
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void DrawUI(MMIIM_T9_CHS_DATA_T *data_ptr)
{
#if (1)	//def UI_MULTILAYER_SUPPORT	
    BOOLEAN is_need_border = FALSE;
    GUI_RECT_T textbar_rect = {0};
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;

    if (PNULL == data_ptr)
    {
        return;
    }

    data_ptr->is_rect_change = FALSE;
    data_ptr->is_bar_edit_dirty = FALSE;
    data_ptr->is_bar_cand_dirty = FALSE;

    if (MMIIM_SP_STATE_INPUTING == data_ptr->state
            || MMIIM_SP_STATE_SELECTING == data_ptr->state
            || MMIIM_SP_STATE_ASSOCIATE == data_ptr->state)
    {
        //samsung style. background image(two line edit and cand) 
        //is always exist when is not on noinput state

        is_need_border = MMIIM_GetMultiLineStyle(data_ptr->ctrl_handle);

        // modify all bars position
        UpdateTextBarsPosition(data_ptr, is_need_border);

        // draw all bars
        mode_type = data_ptr->init_param.method_param_ptr->im_set.type;
        if (mode_type == GUIIM_TYPE_SMART)
        {
            MMIIM_SpellBarDraw(&(data_ptr->bar_edit), is_need_border);
        }
        // Don't need to draw editbar for Stroke input mode
        //else if (mode_type == GUIIM_TYPE_STROKE)
        //{
        //    MMIIM_EditBarDraw(&(data_ptr->bar_edit));
        //}

        MMIIM_CandBarDraw(&(data_ptr->bar_cand), (data_ptr->bar_edit.rect), is_need_border);
    }
#endif
    return;
}

/******************************************************************************/
// Description: ReDrawUI
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL void ReDrawUI(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    BOOLEAN is_need_border = FALSE;
    GUI_RECT_T textbar_rect = {0};
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;

    if (PNULL == data_ptr || PNULL == data_ptr->init_param.method_param_ptr)
    {
        return;
    }

#if 0// the following check should be removed so that the refresh operation will be done synchronously.
    if (ChangeRect(data_ptr))
    {
        return;
    }
#endif

    if (MMIIM_SP_STATE_INPUTING == data_ptr->state
            || MMIIM_SP_STATE_SELECTING == data_ptr->state
            || MMIIM_SP_STATE_ASSOCIATE == data_ptr->state
       )
    {
        is_need_border = MMIIM_GetMultiLineStyle(data_ptr->ctrl_handle);
        // modify all bars position
        UpdateTextBarsPosition(data_ptr, is_need_border);

        //if (data_ptr->is_bar_edit_dirty)
        {
            // draw all bars
            mode_type = data_ptr->init_param.method_param_ptr->im_set.type;
            if (mode_type == GUIIM_TYPE_SMART)
            {
                MMIIM_SpellBarDraw(&(data_ptr->bar_edit), is_need_border);
            }
            // Don't need to draw editbar for Stroke input mode
            //else if (mode_type == GUIIM_TYPE_STROKE)
            //{
            //    MMIIM_EditBarDraw(&(data_ptr->bar_edit));
            //}
        }
#ifdef UI_MULTILAYER_SUPPORT	
        if (data_ptr->is_bar_cand_dirty)
#endif
        {
            MMIIM_CandBarDraw(&(data_ptr->bar_cand), (data_ptr->bar_edit.rect), is_need_border);
        }
    }

    data_ptr->is_bar_edit_dirty = FALSE;
    data_ptr->is_bar_cand_dirty = FALSE;

    return;
}

/******************************************************************************/
// Description: HandleSysMsgNoInput
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgNoInput(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        )
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    T9KEY eKeyID = T9KEYNONE;
    T9STATUS sStatus = T9STATERROR;
    MMIIM_TEXT_BAR_SEG_INFO_T underline_seg_info = {0};
#if 0//UNISOC_MMI_Delete for compile error  
    MMI_CTRL_ID_T ctrl_id = 0;
    ctrl_id = MMIPB_CheckAndModifyEditCtrlId(data_ptr->init_param.win_id,
            MMK_GetActiveCtrlId(MMK_GetFocusWinId()));
#endif
    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgNoInput: dataptr=%d, event=%d.", 
                data_ptr, event_data_ptr);
        return FALSE;
    }

    TransMsg(data_ptr, event_data_ptr, &msg);

    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        SychT9WithEdit(data_ptr);
        is_dealt = TRUE;
        return is_dealt;
    }

    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgNoInput: msg_id = null.");
        return FALSE;
    }

    if (!MMIIM_ConvertMsgToT9KeyID(&msg, &eKeyID))
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgNoInput: invalid im_key=%d.",
                msg.para.key);

        return is_dealt;
    }

    if((GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_set.lang) && (T9KEYAMBIGA == eKeyID))
    {
        eKeyID = T9KEYAMBIGB;
    }

    SCI_TRACE_LOW("[MMIIM_T9_CHS504445] HandleSysMsgNoInput: msg.id: %d, msg.para.key: %d.", msg.id, msg.para.key);

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
                    data_ptr->last_key = msg.para.key;
                    break;

                case MMIIM_SP_KEY_LEFT:
                    is_dealt = HandleLeftKeyMsg(data_ptr, MMIIM_MOVE_CHAR_BY_CHAR);
                    break;

                case MMIIM_SP_KEY_RIGHT:
                    is_dealt = HandleRightKeyMsg(data_ptr, MMIIM_MOVE_CHAR_BY_CHAR);
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

            if(MMIIM_SP_KEY_HASH == msg.para.key)
            {
#if defined T9_7_4_4
                BOOLEAN is_smart_type = FALSE;
                is_smart_type = MMIAPIIM_GetWordPredictiveState() 
                    && MMIAPIIM_IsSmartModeLimit(data_ptr->init_param.method_param_ptr->type_set);
#endif
#if 0//UNISOC_MMI_Delete for compile error
                MMIIM_SwitchMethod(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang,
                        data_ptr->init_param.method_param_ptr->im_set.method,
                        is_smart_type);
#endif               
                is_dealt = TRUE;
                break;
            }

            if (MMIIM_SP_KEY_NONE == data_ptr->last_key
                    || MMIIM_SP_KEY_MAX == data_ptr->last_key)
            {
                if (MMIIM_SP_KEY_STAR == msg.para.key)
                {
                    MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));

                    is_dealt = TRUE;
                }
                else
                {
                    is_dealt = FALSE;
                }
                break;
            }
            data_ptr->last_key = MMIIM_SP_KEY_NONE;
            switch (msg.para.key)
            {
                case MMIIM_SP_KEY_0:
                    if(GUIIM_LANG_CHT != data_ptr->init_param.method_param_ptr->im_set.lang)
                    {
                        CommitChar(data_ptr, ' ');
                        break;
                    }
                case MMIIM_SP_KEY_1:
                case MMIIM_SP_KEY_2:
                case MMIIM_SP_KEY_3:
                case MMIIM_SP_KEY_4:
                case MMIIM_SP_KEY_5:
                case MMIIM_SP_KEY_6:
                case MMIIM_SP_KEY_7:
                case MMIIM_SP_KEY_8:
                case MMIIM_SP_KEY_9:
                    sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

                    if (T9STATNONE == sStatus)
                    {
                        MMIIM_T9_GetChsCoreData(data_ptr);

                        SetToInputing(data_ptr);
                        //data_ptr->is_rect_change = TRUE;
                        data_ptr->is_bar_edit_dirty = TRUE;
                        data_ptr->is_bar_cand_dirty = TRUE;

                        // commit underline char
                        if (data_ptr->bar_edit.has_hot && ((data_ptr->bar_edit.seg_cnt - 1) >= data_ptr->bar_edit.hot_index))
                        {
                            underline_seg_info = data_ptr->bar_edit.seg[data_ptr->bar_edit.hot_index];
                            MMIIM_ReplaceChars(
                                    &data_ptr->init_param,
                                    underline_seg_info.word_ptr,
                                    0,
                                    underline_seg_info.len);
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
                    is_dealt = HandleLongLeftKeyMsg(data_ptr);
                    break;

                case MMIIM_SP_KEY_RIGHT:
                    is_dealt = HandleLongRightKeyMsg(data_ptr);
                    break;

                case MMIIM_SP_KEY_HASH:
#if defined T9_7_4_4
                    MMIAPIIM_T9ModeSwitch(data_ptr->init_param.im_handle);
#endif
                    break;

                case MMIIM_SP_KEY_STAR:
                    //if (MMIIM_SP_KEY_STAR == msg.para.key)
                    {
                        MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));

                        is_dealt = TRUE;
                    }
#if 0 //UNISOC_MMI_Delete for compile error
                    MMIAPIWriting_options_OpenMainWin(ctrl_id);
#endif
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
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        )
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    T9KEY eKeyID = T9KEYNONE;
    T9STATUS sStatus = T9STATERROR;
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;
    int32 cmt_idx = 0;
    T9SYMB cmt_chr = 0;
    BOOLEAN is_ret = FALSE;
    MMIIM_TEXT_BAR_SEG_INFO_T underline_seg_info = {0};
#if 0//UNISOC_MMI_Delete for compile error    
    MMI_CTRL_ID_T ctrl_id = 0;
    ctrl_id = MMIPB_CheckAndModifyEditCtrlId(data_ptr->init_param.win_id,
            MMK_GetActiveCtrlId(MMK_GetFocusWinId()));
#endif
    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputing: dataptr=%d, event=%d.", 
                data_ptr, event_data_ptr);
        return FALSE;
    }

    TransMsg(data_ptr, event_data_ptr, &msg);

    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        SetToNoInput(data_ptr);
        SychT9WithEdit(data_ptr);
        is_dealt = TRUE;
        return is_dealt;
    }

    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id || MSG_CTL_LOSE_ACTIVE == event_data_ptr->sys_msg.msg_id)
    {
        ResetToNoInput(data_ptr);
        return TRUE;
    }

    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputing: msg_id = null.");
        return FALSE;
    }

    if (!MMIIM_ConvertMsgToT9KeyID(&msg, &eKeyID))
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputing: invalid im_key=%d.",
                msg.para.key);

        return is_dealt;
    }

    if((GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_set.lang) && (T9KEYAMBIGA == eKeyID))
    {
        eKeyID = T9KEYAMBIGB;
    }

    mode_type = data_ptr->init_param.method_param_ptr->im_set.type;

    SCI_TRACE_LOW("[MMIIM_T9_CHS504445] HandleSysMsgInputing: msg.id: %d, msg.para.key: %d, mode_type: %d.", msg.id, msg.para.key, mode_type);

    switch (msg.id)
    {
        case MMIIM_SP_MSG_KEY_PRESS:
            is_dealt = TRUE;

            switch (msg.para.key)
            {
                //change the current selected spell pinyin
                case MMIIM_SP_KEY_LEFT:
                case MMIIM_SP_KEY_RIGHT:
                    // Handle both GUIIM_TYPE_SMART and GUIIM_TYPE_STROKE in the same way
                    is_dealt = HandleHorDirectKeyMsg(data_ptr, msg.para.key);
                    break;

                    //page up or page down of the cand bar in current selected spell pinyin
                case MMIIM_SP_KEY_UP:
                case MMIIM_SP_KEY_DOWN:
                    // Handle both GUIIM_TYPE_SMART and GUIIM_TYPE_STROKE in the same way
                    is_dealt = HandleVerDirectKeyMsg(data_ptr, msg.para.key);
                    break;

                    //delete the spell pinyin chars
                case MMIIM_SP_KEY_CANCEL:
                    is_dealt = HandleCancelKeyMsg(data_ptr);
                    break;

                    //enter to selecting state
                case MMIIM_SP_KEY_OK:
                    //convert to selecting state or commit the first cand into the editor directly???
                    SetToSelecting(data_ptr);
                    //data_ptr->is_bar_edit_dirty = TRUE;
                    data_ptr->is_bar_cand_dirty = TRUE;
                    break;

                default:
                    is_dealt = FALSE;
                    break;
            }

            break;

        case MMIIM_SP_MSG_KEY_RELEASE:
            is_dealt = TRUE;

            if(MMIIM_SP_KEY_HASH == msg.para.key)
            {
#if defined T9_7_4_4
                BOOLEAN is_smart_type = FALSE;
                is_smart_type = MMIAPIIM_GetWordPredictiveState() 
                    && MMIAPIIM_IsSmartModeLimit(data_ptr->init_param.method_param_ptr->type_set);
#endif
#if 0 //UNISOC_MMI_Delete for compile error
                MMIIM_SwitchMethod(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang,
                        data_ptr->init_param.method_param_ptr->im_set.method,
                        is_smart_type);
#endif                
                is_dealt = TRUE;
                break;
            }
#if 1//UNISOC_MMI_Delete  Cheney TODO   
            if (MMIIM_SP_KEY_STAR == msg.para.key)
            {
                MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                is_dealt = TRUE;
            }
#endif        
            switch (msg.para.key)
            {
                //don't handle key0, do key1 by submit the first select one char
                case MMIIM_SP_KEY_0:
                    if(GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_set.lang)
                    {
                        sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

                        if (T9STATNONE == sStatus)
                        {
                            MMIIM_T9_GetChsCoreData(data_ptr);
                            //data_ptr->is_rect_change = TRUE;
                            data_ptr->is_bar_edit_dirty = TRUE;
                            data_ptr->is_bar_cand_dirty = TRUE;

                            // commit underline char
                            if (data_ptr->bar_edit.has_hot && ((data_ptr->bar_edit.seg_cnt - 1) >= data_ptr->bar_edit.hot_index))
                            {
                                underline_seg_info = data_ptr->bar_edit.seg[data_ptr->bar_edit.hot_index];
                                MMIIM_ReplaceChars(
                                        &data_ptr->init_param,
                                        underline_seg_info.word_ptr,
                                        0,
                                        underline_seg_info.len);
                            }
                        }
                    }
                    break;
                case MMIIM_SP_KEY_1:
                    if(GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_set.lang)
                    {
                        sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

                        if (T9STATNONE == sStatus)
                        {
                            MMIIM_T9_GetChsCoreData(data_ptr);
                            //data_ptr->is_rect_change = TRUE;
                            data_ptr->is_bar_edit_dirty = TRUE;
                            data_ptr->is_bar_cand_dirty = TRUE;

                            // commit underline char
                            if (data_ptr->bar_edit.has_hot && ((data_ptr->bar_edit.seg_cnt - 1) >= data_ptr->bar_edit.hot_index))
                            {
                                underline_seg_info = data_ptr->bar_edit.seg[data_ptr->bar_edit.hot_index];
                                MMIIM_ReplaceChars(
                                        &data_ptr->init_param,
                                        underline_seg_info.word_ptr,
                                        0,
                                        underline_seg_info.len);
                            }
                        }
                    }
                    else
                    {
                        cmt_idx = 0;
                        // Don't convert to associate state automatically on key_1 when current
                        // mode is STROKE because it is used as one stroke --- 'horizontal,-'
                        if (mode_type == GUIIM_TYPE_SMART && (cmt_idx < data_ptr->bar_cand.seg_cnt))
                        {
                            is_ret = GetSelectCharFromEngin(data_ptr, cmt_idx, &cmt_chr);

                            if (is_ret)
                            {
                                sStatus = MMIIM_T9_AddExplicitChar(&data_ptr->cc_field_info.G, cmt_chr);

                                if (T9STATNONE == sStatus)
                                {
                                    CommitChar(data_ptr, cmt_chr);
                                    MMIIM_T9_GetChsCoreData(data_ptr);
                                    SetToAssociate(data_ptr);
#ifdef UI_MULTILAYER_SUPPORT	
                                    data_ptr->is_bar_cand_dirty = TRUE;
                                    MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
                                    MMK_UpdateScreen();
                                    data_ptr->is_bar_cand_dirty = TRUE;
                                    is_dealt = FALSE;
#endif
                                }
                            }
                            break;
                        }
                    }
                case MMIIM_SP_KEY_2:
                case MMIIM_SP_KEY_3:
                case MMIIM_SP_KEY_4:
                case MMIIM_SP_KEY_5:
                case MMIIM_SP_KEY_6:
                case MMIIM_SP_KEY_7:
                case MMIIM_SP_KEY_8:
                case MMIIM_SP_KEY_9:
                    sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

                    if (T9STATNONE == sStatus)
                    {
                        MMIIM_T9_GetChsCoreData(data_ptr);
                        //data_ptr->is_rect_change = TRUE;
                        data_ptr->is_bar_edit_dirty = TRUE;
                        data_ptr->is_bar_cand_dirty = TRUE;

                        // commit underline char
                        if (data_ptr->bar_edit.has_hot && ((data_ptr->bar_edit.seg_cnt - 1) >= data_ptr->bar_edit.hot_index))
                        {
                            underline_seg_info = data_ptr->bar_edit.seg[data_ptr->bar_edit.hot_index];
                            MMIIM_ReplaceChars(
                                    &data_ptr->init_param,
                                    underline_seg_info.word_ptr,
                                    0,
                                    underline_seg_info.len);
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

                case MMIIM_SP_KEY_HASH:
#if defined T9_7_4_4
                    MMIAPIIM_T9ModeSwitch(data_ptr->init_param.im_handle);
#endif
                    break;

                case MMIIM_SP_KEY_STAR:
                    {
                        MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                        is_dealt = TRUE;
                    }
#if 0 //UNISOC_MMI_Delete for compile error
                    MMIAPIWriting_options_OpenMainWin(ctrl_id);
#endif
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
                //change the current selected spell pinyin
                case MMIIM_SP_KEY_LEFT:
                case MMIIM_SP_KEY_RIGHT:
                    // Handle both GUIIM_TYPE_SMART and GUIIM_TYPE_STROKE in the same way
                    is_dealt = HandleHorDirectKeyMsg(data_ptr, msg.para.key);
                    break;

                    //page up or page down of the cand bar in current selected spell pinyin
                case MMIIM_SP_KEY_UP:
                case MMIIM_SP_KEY_DOWN:
                    // Handle both GUIIM_TYPE_SMART and GUIIM_TYPE_STROKE in the same way
                    is_dealt = HandleVerDirectKeyMsg(data_ptr, msg.para.key);
                    break;

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
// Description: HandleSysMsgSelecting
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgSelecting(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        )
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    T9KEY eKeyID = T9KEYNONE;
    T9STATUS sStatus = T9STATERROR;
    int32 cmt_idx = 0;
    T9SYMB cmt_chr = 0;
    BOOLEAN is_ret = FALSE;
#if 0//UNISOC_MMI_Delete for compile error
    MMI_CTRL_ID_T ctrl_id = 0;
    ctrl_id = MMIPB_CheckAndModifyEditCtrlId(data_ptr->init_param.win_id,
            MMK_GetActiveCtrlId(MMK_GetFocusWinId()));
#endif
    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgSelecting: dataptr=%d, event=%d.", 
                data_ptr, event_data_ptr);
        return FALSE;
    }

    TransMsg(data_ptr, event_data_ptr, &msg);

    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        SetToNoInput(data_ptr);
        SychT9WithEdit(data_ptr);
        is_dealt = TRUE;
        return is_dealt;
    }

    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id || MSG_CTL_LOSE_ACTIVE == event_data_ptr->sys_msg.msg_id)
    {
        ResetToNoInput(data_ptr);
        return TRUE;
    }

    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgSelecting: msg_id = null.");
        return FALSE;
    }

    if (!MMIIM_ConvertMsgToT9KeyID(&msg, &eKeyID))
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgSelecting: invalid im_key=%d.",
                msg.para.key);

        return is_dealt;
    }

    if((GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_set.lang) && (T9KEYAMBIGA == eKeyID))
    {
        eKeyID = T9KEYAMBIGB;
    }

    SCI_TRACE_LOW("[MMIIM_T9_CHS504445] HandleSysMsgSelecting: msg.id: %d, msg.para.key: %d.", msg.id, msg.para.key);

    switch (msg.id)
    {
        case MMIIM_SP_MSG_KEY_PRESS:
            is_dealt = TRUE;

            switch (msg.para.key)
            {
                //change the current selected cand char
                case MMIIM_SP_KEY_UP:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 0);
                    break;
                case MMIIM_SP_KEY_DOWN:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 1);
                    break;

                    //page up or page down of the cand bar in current selected spell pinyin
                case MMIIM_SP_KEY_LEFT:
                case MMIIM_SP_KEY_RIGHT:
                    is_dealt = HandleHorDirectKeyMsg(data_ptr, msg.para.key);
                    break;

                    //backu to inputing state
                case MMIIM_SP_KEY_CANCEL:
                    is_dealt = HandleCancelKeyMsg(data_ptr);
                    break;

                    //commit the current focus char to edit
                case MMIIM_SP_KEY_OK:
                    is_ret = GetSelectCharFromEngin(data_ptr, data_ptr->bar_cand.hot_index, &cmt_chr);

                    if (is_ret)
                    {
                        T9SYMB objtt[2] = {0};
                        sStatus = MMIIM_T9_AddExplicitChar(&data_ptr->cc_field_info.G, cmt_chr);
                        objtt[0] = cmt_chr;
                        T9CCAudbAddObj(&data_ptr->cc_field_info, objtt);
                        if (T9STATNONE == sStatus)
                        {
                            CommitChar(data_ptr, cmt_chr);
                            MMIIM_T9_GetChsCoreData(data_ptr);
                            SetToAssociate(data_ptr);
#ifdef UI_MULTILAYER_SUPPORT	
                            data_ptr->is_bar_cand_dirty = TRUE;
                            MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
                            MMK_UpdateScreen();
                            data_ptr->is_bar_cand_dirty = TRUE;
#endif
                        }
                    }
                    break;

                default:
                    is_dealt = FALSE;
                    break;
            }

            break;

        case MMIIM_SP_MSG_KEY_RELEASE:
            is_dealt = TRUE;

            if(MMIIM_SP_KEY_HASH == msg.para.key)
            {
#if defined T9_7_4_4
                BOOLEAN is_smart_type = FALSE;
                is_smart_type = MMIAPIIM_GetWordPredictiveState() 
                    && MMIAPIIM_IsSmartModeLimit(data_ptr->init_param.method_param_ptr->type_set);
#endif
#if 0 //UNISOC_MMI_Delete for compile error
                MMIIM_SwitchMethod(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang,
                        data_ptr->init_param.method_param_ptr->im_set.method,
                        is_smart_type);
#endif                
                is_dealt = TRUE;
                break;
            }
#if 1  //UNISOC_MMI_Delete  Cheney TODO       
            if (MMIIM_SP_KEY_STAR == msg.para.key)
            {
                MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                is_dealt = TRUE;
            }
#endif        
            switch (msg.para.key)
            {
                case MMIIM_SP_KEY_0:
                    if(GUIIM_LANG_CHT != data_ptr->init_param.method_param_ptr->im_set.lang)
                    {
                        break;
                    }

                case MMIIM_SP_KEY_1:
                case MMIIM_SP_KEY_2:
                case MMIIM_SP_KEY_3:
                case MMIIM_SP_KEY_4:
                case MMIIM_SP_KEY_5:
                case MMIIM_SP_KEY_6:
                case MMIIM_SP_KEY_7:
                case MMIIM_SP_KEY_8:
                case MMIIM_SP_KEY_9:

                    cmt_idx = msg.para.key - MMIIM_SP_KEY_0 - 1;
                    if (cmt_idx < data_ptr->bar_cand.seg_cnt)
                    {
                        is_ret = GetSelectCharFromEngin(data_ptr, cmt_idx, &cmt_chr);

                        if (is_ret)
                        {
                            sStatus = MMIIM_T9_AddExplicitChar(&data_ptr->cc_field_info.G, cmt_chr);

                            if (T9STATNONE == sStatus)
                            {
                                CommitChar(data_ptr, cmt_chr);                        
                                MMIIM_T9_GetChsCoreData(data_ptr);
                                SetToAssociate(data_ptr);
                                //data_ptr->is_rect_change = TRUE;
                                //data_ptr->is_bar_edit_dirty = TRUE;
                                data_ptr->is_bar_cand_dirty = TRUE;
#ifdef UI_MULTILAYER_SUPPORT	
                                MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
                                MMK_UpdateScreen();
                                is_dealt = FALSE;
#endif
                                //???????what shall we do now???
                            }
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

                case MMIIM_SP_KEY_HASH:
#if defined T9_7_4_4
                    MMIAPIIM_T9ModeSwitch(data_ptr->init_param.im_handle);
#endif
                    break;

                case MMIIM_SP_KEY_STAR:
                    {
                        MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                        is_dealt = TRUE;
                    }
#if 0 //UNISOC_MMI_Delete for compile error
                    MMIAPIWriting_options_OpenMainWin(ctrl_id);
#endif
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
                //change the current selected cand char
                case MMIIM_SP_KEY_UP:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 0);
                    break;
                case MMIIM_SP_KEY_DOWN:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 1);
                    break;

                    //page up or page down of the cand bar in current selected spell pinyin
                case MMIIM_SP_KEY_LEFT:
                case MMIIM_SP_KEY_RIGHT:
                    is_dealt = HandleHorDirectKeyMsg(data_ptr, msg.para.key);
                    break;

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
// Description: HandleSysMsgInputSelecting
// Global resource dependence: NONE
// Author: arvin
// Note: Special test for Stroke Mode
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgInputSelecting(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        )
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    T9KEY eKeyID = T9KEYNONE;
    T9STATUS sStatus = T9STATERROR;
    int32 cmt_idx = 0;
    T9SYMB cmt_chr = 0;
    BOOLEAN is_ret = FALSE;
    MMIIM_TEXT_BAR_SEG_INFO_T underline_seg_info = {0};

    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputSelecting: dataptr=%d, event=%d.", 
                data_ptr, event_data_ptr);
        return FALSE;
    }

    TransMsg(data_ptr, event_data_ptr, &msg);

    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        SetToNoInput(data_ptr);
        SychT9WithEdit(data_ptr);
        is_dealt = TRUE;
        return is_dealt;
    }

    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id || MSG_CTL_LOSE_ACTIVE == event_data_ptr->sys_msg.msg_id)
    {
        ResetToNoInput(data_ptr);
        return TRUE;
    }

    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputSelecting: msg_id = null.");
        return FALSE;
    }

    if (!MMIIM_ConvertMsgToT9KeyID(&msg, &eKeyID))
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgInputSelecting: invalid im_key=%d.",
                msg.para.key);

        return is_dealt;
    }

    if((GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_set.lang) && (T9KEYAMBIGA == eKeyID))
    {
        eKeyID = T9KEYAMBIGB;
    }

    SCI_TRACE_LOW("[MMIIM_T9_CHS504445] HandleSysMsgInputSelecting: msg.id: %d, msg.para.key: %d.", msg.id, msg.para.key);

    switch (msg.id)
    {
        case MMIIM_SP_MSG_KEY_PRESS:
            is_dealt = TRUE;

            switch (msg.para.key)
            {
                //change the current selected cand char
                case MMIIM_SP_KEY_UP:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 0);
                    break;
                case MMIIM_SP_KEY_DOWN:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 1);
                    break;

                    //page up or page down of the cand bar in current selected spell pinyin
                case MMIIM_SP_KEY_LEFT:
                case MMIIM_SP_KEY_RIGHT:
                    is_dealt = HandleHorDirectKeyMsg(data_ptr, msg.para.key);
                    break;

                    //backu to inputing state
                case MMIIM_SP_KEY_CANCEL:
                    is_dealt = HandleCancelKeyMsg(data_ptr);
                    break;

                    //commit the current focus char to edit
                case MMIIM_SP_KEY_OK:
                    is_ret = GetSelectCharFromEngin(data_ptr, data_ptr->bar_cand.hot_index, &cmt_chr);

                    if (is_ret)
                    {
                        T9SYMB objtt[2] = {0};
                        sStatus = MMIIM_T9_AddExplicitChar(&data_ptr->cc_field_info.G, cmt_chr);
                        objtt[0] = cmt_chr;
                        T9CCAudbAddObj(&data_ptr->cc_field_info, objtt);
                        if (T9STATNONE == sStatus)
                        {
                            CommitChar(data_ptr, cmt_chr);
                            MMIIM_T9_GetChsCoreData(data_ptr);
                            SetToAssociate(data_ptr);
#ifdef UI_MULTILAYER_SUPPORT	
                            data_ptr->is_bar_cand_dirty = TRUE;
                            MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
                            MMK_UpdateScreen();
                            data_ptr->is_bar_cand_dirty = TRUE;
#endif
                        }
                    }
                    break;

                default:
                    is_dealt = FALSE;
                    break;
            }

            break;

        case MMIIM_SP_MSG_KEY_RELEASE:
            is_dealt = TRUE;

            if(MMIIM_SP_KEY_HASH == msg.para.key)
            {
#if defined T9_7_4_4
                BOOLEAN is_smart_type = FALSE;
                is_smart_type = MMIAPIIM_GetWordPredictiveState() 
                    && MMIAPIIM_IsSmartModeLimit(data_ptr->init_param.method_param_ptr->type_set);
#endif
#if 0 //UNISOC_MMI_Delete for compile error
                MMIIM_SwitchMethod(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang,
                        data_ptr->init_param.method_param_ptr->im_set.method,
                        is_smart_type);
#endif                
                is_dealt = TRUE;
                break;
            }
#if 1//UNISOC_MMI_Delete Cheney TODO
            if (MMIIM_SP_KEY_STAR == msg.para.key)
            {
                MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                is_dealt = TRUE;
            }
#endif        
            switch (msg.para.key)
            {
                case MMIIM_SP_KEY_0:
                    if(GUIIM_LANG_CHT != data_ptr->init_param.method_param_ptr->im_set.lang)
                    {
                        break;
                    }

                case MMIIM_SP_KEY_1:
                case MMIIM_SP_KEY_2:
                case MMIIM_SP_KEY_3:
                case MMIIM_SP_KEY_4:
                case MMIIM_SP_KEY_5:
                case MMIIM_SP_KEY_6:
                case MMIIM_SP_KEY_7:
                case MMIIM_SP_KEY_8:
                case MMIIM_SP_KEY_9:

                    sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

                    if (T9STATNONE == sStatus)
                    {
                        MMIIM_T9_GetChsCoreData(data_ptr);
                        //data_ptr->is_rect_change = TRUE;
                        data_ptr->is_bar_edit_dirty = TRUE;
                        data_ptr->is_bar_cand_dirty = TRUE;

                        // commit underline char
                        if (data_ptr->bar_edit.has_hot && ((data_ptr->bar_edit.seg_cnt - 1) >= data_ptr->bar_edit.hot_index))
                        {
                            underline_seg_info = data_ptr->bar_edit.seg[data_ptr->bar_edit.hot_index];
                            MMIIM_ReplaceChars(
                                    &data_ptr->init_param,
                                    underline_seg_info.word_ptr,
                                    0,
                                    underline_seg_info.len);
                        }
                        // force to draw first hot cand word in inputing state
                        //data_ptr->bar_cand.has_hot = 1;
                        //data_ptr->bar_cand.hot_index = 0;
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

                case MMIIM_SP_KEY_HASH:
#if defined T9_7_4_4
                    MMIAPIIM_T9ModeSwitch(data_ptr->init_param.im_handle);
#endif
                    break;

                case MMIIM_SP_KEY_STAR:
                    {
                        MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                        is_dealt = TRUE;
                    }
#if 0 //UNISOC_MMI_Delete for compile error
                    MMIAPIWriting_options_OpenMainWin(ctrl_id);
#endif
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
                //change the current selected cand char
                case MMIIM_SP_KEY_UP:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 0);
                    break;

                case MMIIM_SP_KEY_DOWN:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 1);
                    break;

                    //page up or page down of the cand bar in current selected spell pinyin
                case MMIIM_SP_KEY_LEFT:
                case MMIIM_SP_KEY_RIGHT:
                    is_dealt = HandleHorDirectKeyMsg(data_ptr, msg.para.key);
                    break;

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
// Description: HandleSysMsgAssociate
// Global resource dependence: NONE
// Author: haiwu.chen
// Note:
/******************************************************************************/
LOCAL BOOLEAN HandleSysMsgAssociate(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        GUIIM_EVENT_DATA_U const *event_data_ptr
        )
{
    BOOLEAN is_dealt = FALSE;
    MMIIM_SP_MSG_T msg = {0};
    T9KEY eKeyID = T9KEYNONE;
    T9STATUS sStatus = T9STATERROR;
    int32 cmt_idx = 0;
    T9SYMB cmt_chr = 0;
    BOOLEAN is_ret = FALSE;
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;
#if 0//UNISOC_MMI_Delete for compile error    
    MMI_CTRL_ID_T ctrl_id = 0;
    ctrl_id = MMIPB_CheckAndModifyEditCtrlId(data_ptr->init_param.win_id,
            MMK_GetActiveCtrlId(MMK_GetFocusWinId()));
#endif
    if (PNULL == data_ptr || PNULL == event_data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] HandleMsgAssociate: dataptr=%d, event=%d.", 
                data_ptr, event_data_ptr);
        return FALSE;
    }

    TransMsg(data_ptr, event_data_ptr, &msg);

    if (MSG_CTL_GET_FOCUS == event_data_ptr->sys_msg.msg_id)
    {
        SetToNoInput(data_ptr);
        SychT9WithEdit(data_ptr);
        is_dealt = TRUE;
        return is_dealt;
    }

    if (MSG_CTL_LOSE_FOCUS == event_data_ptr->sys_msg.msg_id || MSG_CTL_LOSE_ACTIVE == event_data_ptr->sys_msg.msg_id)
    {
        ResetToNoInput(data_ptr);
        return TRUE;
    }

    if (MMIIM_SP_MSG_NONE == msg.id)
    {
        SCI_TRACE_LOW("[MMIIM] HandleMsgAssociate: msg_id = null.");
        return FALSE;
    }

    if (!MMIIM_ConvertMsgToT9KeyID(&msg, &eKeyID))
    {
        SCI_TRACE_LOW("[MMIIM] HandleSysMsgAssociate: invalid im_key=%d.",
                msg.para.key);

        return is_dealt;
    }

    if((GUIIM_LANG_CHT == data_ptr->init_param.method_param_ptr->im_set.lang) && (T9KEYAMBIGA == eKeyID))
    {
        eKeyID = T9KEYAMBIGB;
    }

    SCI_TRACE_LOW("[MMIIM_T9_CHS504445] HandleSysMsgAssociate: msg.id: %d, msg.para.key: %d.", msg.id, msg.para.key);

    switch (msg.id)
    {
        case MMIIM_SP_MSG_KEY_PRESS:
            is_dealt = TRUE;

            switch (msg.para.key)
            {
                //change the current selected cand char
                case MMIIM_SP_KEY_UP:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 0);
                    break;

                    //change the current selected cand char
                case MMIIM_SP_KEY_DOWN:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 1);
                    break;

                    //page up or page down of the cand bar in current assosiate chars
                case MMIIM_SP_KEY_LEFT:
                case MMIIM_SP_KEY_RIGHT:
                    is_dealt = HandleHorDirectKeyMsg(data_ptr, msg.para.key);
                    break;

                    //backu to inputing state
                case MMIIM_SP_KEY_CANCEL:
                    is_dealt = HandleCancelKeyMsg(data_ptr);
                    break;

                    //if cand don't have label, set lable. otherwise commit the highlight char to edit
                case MMIIM_SP_KEY_OK:
                    //if (data_ptr->bar_cand.has_label)
                    {
                        is_ret = GetSelectCharFromEngin(data_ptr, data_ptr->bar_cand.hot_index, &cmt_chr);

                        if (is_ret)
                        {
                            sStatus = MMIIM_T9_AddExplicitChar(&data_ptr->cc_field_info.G, cmt_chr);
                            if (T9STATNONE == sStatus)
                            {
                                CommitChar(data_ptr, cmt_chr);
                                MMIIM_T9_GetChsCoreData(data_ptr);
                                data_ptr->bar_cand.has_label = 0;
                                data_ptr->bar_cand.hot_index = 0;
#ifndef UI_MULTILAYER_SUPPORT	
                                MMK_UpdateScreen();
#endif
                                data_ptr->is_bar_cand_dirty = TRUE;
                            }
                        }
                    }
#if 0//MMI_UNISOC_DELETE
                    else
                    {
                        data_ptr->bar_cand.has_label = 1;
                        data_ptr->is_bar_cand_dirty = TRUE;
                    }
#endif
                    break;

                default:
                    is_dealt = FALSE;
                    break;
            }

            break;

        case MMIIM_SP_MSG_KEY_RELEASE:
            is_dealt = TRUE;

            if(MMIIM_SP_KEY_HASH == msg.para.key)
            {
#if defined T9_7_4_4
                BOOLEAN is_smart_type = FALSE;
                is_smart_type = MMIAPIIM_GetWordPredictiveState() 
                    && MMIAPIIM_IsSmartModeLimit(data_ptr->init_param.method_param_ptr->type_set);
#endif
#if 0//UNISOC_MMI_Delete for compile error
                MMIIM_SwitchMethod(data_ptr->init_param.im_handle,
                        data_ptr->init_param.method_param_ptr->im_set.lang,
                        data_ptr->init_param.method_param_ptr->im_set.method,
                        is_smart_type);
#endif                
                is_dealt = TRUE;
                break;
            }
#if 1//UNISOC_MMI_Delete Cheney TODO         
            if (MMIIM_SP_KEY_STAR == msg.para.key)
            {
                MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                is_dealt = TRUE;
            }
#endif        
            switch (msg.para.key)
            {
                //samsung style in associate state and don't has labe, handle key0 the same as noinput.
                case MMIIM_SP_KEY_0:
                    if(GUIIM_LANG_CHT != data_ptr->init_param.method_param_ptr->im_set.lang)
                    {
                        if (!data_ptr->bar_cand.has_label)
                        {
                            SetToNoInput(data_ptr);

                            data_ptr->last_key = msg.para.key;
                            is_dealt = HandleSysMsgNoInput(data_ptr, event_data_ptr);
#ifdef UI_MULTILAYER_SUPPORT	
                            MMIIM_PopupListReleaseLayer(&data_ptr->bar_cand);
                            MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
                            MMK_UpdateScreen();
#endif
                        }
                        break;
                    }

                case MMIIM_SP_KEY_1:
                case MMIIM_SP_KEY_2:
                case MMIIM_SP_KEY_3:
                case MMIIM_SP_KEY_4:
                case MMIIM_SP_KEY_5:
                case MMIIM_SP_KEY_6:
                case MMIIM_SP_KEY_7:
                case MMIIM_SP_KEY_8:
                case MMIIM_SP_KEY_9:
                    mode_type = data_ptr->init_param.method_param_ptr->im_set.type;
                    if ((MMIIM_SP_KEY_1 == msg.para.key) && (GUIIM_TYPE_STROKE != mode_type))
                    {
                        cmt_idx = msg.para.key - MMIIM_SP_KEY_1;

                        if (cmt_idx < data_ptr->bar_cand.seg_cnt)
                        {
                            is_ret = GetSelectCharFromEngin(data_ptr, cmt_idx, &cmt_chr);

                            if (is_ret)
                            {
                                sStatus = MMIIM_T9_AddExplicitChar(&data_ptr->cc_field_info.G, cmt_chr);
                                if (T9STATNONE == sStatus)
                                {
                                    CommitChar(data_ptr, cmt_chr);
                                    MMIIM_T9_GetChsCoreData(data_ptr);
                                    data_ptr->bar_cand.has_label = 0;
                                    data_ptr->bar_cand.hot_index = 0;
                                    data_ptr->is_bar_cand_dirty = TRUE;
                                }
                            }
                        }
                    }
                    else
                    {
                        SetToNoInput(data_ptr); 
#ifdef UI_MULTILAYER_SUPPORT	
                        MMIIM_PopupListReleaseLayer(&data_ptr->bar_cand);
#else
                        MMK_UpdateScreen();
#endif
                        data_ptr->last_key = msg.para.key;
                        is_dealt = HandleSysMsgNoInput(data_ptr, event_data_ptr);
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

                case MMIIM_SP_KEY_HASH:
#if defined T9_7_4_4
                    MMIAPIIM_T9ModeSwitch(data_ptr->init_param.im_handle);
#endif
                    break;

                case MMIIM_SP_KEY_STAR:
                    {
                        MMIIM_SP_SymbCreateWin(data_ptr->init_param.im_handle, MMIIM_GetSymbTypeByLang(data_ptr->init_param.method_param_ptr->im_set.lang));
                        is_dealt = TRUE;
                    }
#if 0 //UNISOC_MMI_Delete for compile error
                    MMIAPIWriting_options_OpenMainWin(ctrl_id);
#endif
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
                //change the current selected cand char
                case MMIIM_SP_KEY_UP:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 0);
                    break;

                    //change the current selected cand char
                case MMIIM_SP_KEY_DOWN:
                    is_dealt = MoveStepsOnCandBar(data_ptr, 1);
                    break;

                    //page up or page down of the cand bar in current assosiate chars
                case MMIIM_SP_KEY_LEFT:
                case MMIIM_SP_KEY_RIGHT:
                    is_dealt = HandleHorDirectKeyMsg(data_ptr, msg.para.key);
                    break;
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
LOCAL void SetToNoInput(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SetToNoInput: data_ptr is null.");
        return;
    }
    data_ptr->state = MMIIM_SP_STATE_NOINPUT;

    MMIIM_TextBarClear(&(data_ptr->bar_edit));
    MMIIM_TextBarClear(&(data_ptr->bar_cand));
    //data_ptr->is_rect_change = TRUE;

    return; 
}

/*****************************************************************************/
//  Description : set to samsung inputing state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToInputing(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SetToInputing: data_ptr is null.");
        return;
    }

    mode_type = data_ptr->init_param.method_param_ptr->im_set.type;
    if ((GUIIM_TYPE_STROKE == mode_type) && (MMIIM_SP_STATE_NOINPUT == data_ptr->state))
    {
        data_ptr->bar_cand.has_hot = 1;
        data_ptr->bar_cand.hot_index = 0;
    }
    else
    {
        data_ptr->bar_cand.has_label = 0;
        data_ptr->bar_cand.has_hot = 0;
    }

    data_ptr->bar_edit.has_hot = 1;
    data_ptr->state = MMIIM_SP_STATE_INPUTING;
    return;
}

/*****************************************************************************/
//  Description : set to selecting state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToSelecting(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SetToSelecting: data_ptr is null.");
        return;
    }

    data_ptr->state = MMIIM_SP_STATE_SELECTING;

    data_ptr->bar_cand.has_hot = 1;
    data_ptr->bar_cand.hot_index = 0;
    data_ptr->bar_cand.has_label = 1;

    data_ptr->bar_edit.has_hot = 0;//MMI_UNISOC_DELETE

    return;
}

/*****************************************************************************/
//  Description : set to associate state
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetToAssociate(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SetToAssociate: data_ptr is null.");
        return;
    }

    data_ptr->state = MMIIM_SP_STATE_ASSOCIATE;

    data_ptr->bar_cand.has_hot = 1;
    data_ptr->bar_cand.hot_index = 0;
    data_ptr->bar_cand.has_label = 0;

    MMIIM_TextBarClear(&data_ptr->bar_edit);
    return;
}

/*****************************************************************************/
//  Description : sychronization t9 engine content and curson with editbox 
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL T9STATUS SychT9WithEdit(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATNONE;
    GUIIM_CONTEXT_T im_context = {0};

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SychT9WithEdit: data_ptr is null.");
        return sStatus;
    }

    MMIIM_GetEditContent(&data_ptr->init_param, &im_context);

    //if (PNULL != im_context.str_ptr && 0 != im_context.str_len)
    // Force to sync T9 engine even if current im_context length
    // is 0 or else mistaken cancel key will be handled by T9 Engine.
    if (PNULL != im_context.str_ptr)
    {
        sStatus = MMIIM_T9_ResetTextBuf(&data_ptr->cc_field_info.G);

        if (T9STATNONE == sStatus)
        {
            sStatus = MMIIM_T9_AddTextString(
                    &data_ptr->cc_field_info.G, im_context.str_ptr, im_context.str_len);
        }

        if (T9STATNONE == sStatus)
        {
            sStatus = SetIMECursorPos(data_ptr);
        }
    }

    return sStatus;
}

/*****************************************************************************/
//  Description : Commit char
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void CommitChar(MMIIM_T9_CHS_DATA_T *data_ptr, wchar c)
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

    return;
}

/*****************************************************************************/
//  Description : set the cursor position from editbox to IME
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL T9STATUS SetIMECursorPos(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    uint16 nCurEditPos = 0;

    nCurEditPos = (uint16)MMIIM_GetEditCursor(&data_ptr->init_param);

    //2: update IME cursor position
    sStatus = MMIIM_T9_Cursor(&data_ptr->cc_field_info.G, T9CA_FROMBEG, nCurEditPos);

    return sStatus;
}

/*****************************************************************************/
//  Description : handle press left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleLeftKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_MOVE_BY_TYPE move_type
        )
{
    BOOLEAN is_dealt = FALSE;
    T9FieldInfo *filed_info_ptr = PNULL;
    T9UINT nPreCursor = 0;
    T9UINT nCursor = 0;
    T9STATUS sStatus = T9STATERROR;
    int16 nStep = 0;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

    filed_info_ptr = &data_ptr->cc_field_info.G;
    if (PNULL == filed_info_ptr)/*lint !e774*/
    {
        return is_dealt;
    }

    //1: get editbox cursor position
    SetIMECursorPos(data_ptr);

    //2: get last position
    nPreCursor = filed_info_ptr->nCursor;

    //3: handle left key
    sStatus = MMIIM_T9_HandleKey(filed_info_ptr, T9KEYLEFT);

    /*
    // No need notify editor move the cursor but just pass the left key to Editor
    //4: get current position
    nCursor = filed_info_ptr->nCursor;

    //reset cursor position
    if (nPreCursor - nCursor > 0)
    {
    if (MMIIM_MOVE_CHAR_BY_CHAR == move_type)
    {
    nStep = (nPreCursor - nCursor);
    MMIIM_SetEditCursor(&data_ptr->init_param, GUIIM_SETCURSOR_TYPE_CUR, -nStep);
    }

    is_dealt = TRUE;
    }
     */
    return is_dealt;
}

/*****************************************************************************/
//  Description : handle press right key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleRightKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_MOVE_BY_TYPE move_type
        )
{
    BOOLEAN is_dealt = FALSE;
    T9FieldInfo *filed_info_ptr = PNULL;
    T9UINT nPreCursor = 0;
    T9UINT nCursor = 0;
    int16 nStep = 0;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

    //1: get editbox cursor position
    SetIMECursorPos(data_ptr);

    if (MMIIM_IsCursorAtTheEnd(&data_ptr->init_param))
    {
        //chinese at the end not doing anything
        //is_dealt = TRUE;
    }
    else
    {
        filed_info_ptr = &data_ptr->cc_field_info.G;
        if (PNULL == filed_info_ptr)/*lint !e774*/
        {
            return is_dealt;
        }
        nPreCursor = filed_info_ptr->nCursor;

        MMIIM_T9_HandleKey(filed_info_ptr, T9KEYRIGHT);

        /*        
        // No need notify editor move the cursor but just pass the right key to Editor
        nCursor = filed_info_ptr->nCursor;

        //reset cursor position
        if (nCursor - nPreCursor > 0)
        {
        nStep = (nCursor - nPreCursor);

        MMIIM_SetEditCursor(&data_ptr->init_param, GUIIM_SETCURSOR_TYPE_CUR, nStep);
        }

        is_dealt = TRUE;
         */
    }
    return is_dealt;
}

/*****************************************************************************/
//  Description : handle t9 horizontal direction key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: not relate to the cursor in editbox, just handle by input method with smart mode
/*****************************************************************************/
PUBLIC BOOLEAN HandleHorDirectKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_SP_VKEY_T key
        )
{
    BOOLEAN is_dealt = FALSE;
    T9STATUS sStatus = T9STATERROR;
    T9UINT nValue = 0;
    T9KEY eKeyID = 0;
    MMIIM_TEXT_BAR_T *bar_ptr = PNULL;
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;

    if (PNULL == data_ptr || ((MMIIM_SP_KEY_LEFT != key) && (MMIIM_SP_KEY_RIGHT != key)))
    {
        SCI_TRACE_LOW("[MMIIM] HandleVerDirectKeyMsg: wrong param, msg_id=%d.", key);
        return is_dealt;
    }

    if(PNULL != data_ptr->init_param.method_param_ptr)
    {
        mode_type = data_ptr->init_param.method_param_ptr->im_set.type;
    }


    //needed???    
    if (MMIIM_SP_KEY_LEFT == key)
    {
        eKeyID = T9KEYPREV;
        if ((MMIIM_SP_STATE_SELECTING == data_ptr->state) && (GUIIM_TYPE_STROKE != mode_type))
        {
            is_dealt = TRUE;
            SetToInputing(data_ptr);
            //data_ptr->is_rect_change = TRUE;
            //data_ptr->is_bar_edit_dirty = TRUE;
            data_ptr->is_bar_cand_dirty = TRUE;
        }

    }
    else if (MMIIM_SP_KEY_RIGHT == key)
    {
        eKeyID = T9KEYNEXT;
        if ((MMIIM_SP_STATE_INPUTING == data_ptr->state) && (GUIIM_TYPE_STROKE != mode_type))
        {
            is_dealt = TRUE;
            SetToSelecting(data_ptr);
            //data_ptr->is_bar_edit_dirty = TRUE;
            data_ptr->is_bar_cand_dirty = TRUE;
        }
    }
    //end needed???
    if (!is_dealt)
    {
        is_dealt = TRUE;
        // Do nothing on the inputing state if the left direction key pressed
        /*
           if (MMIIM_SP_STATE_INPUTING == data_ptr->state)
           {
           sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

           if (T9STATNONE == sStatus)
           {
           MMIIM_T9_GetChsCoreData(data_ptr);
        //data_ptr->bar_cand.has_label = 0;
        //data_ptr->bar_cand.has_hot = 0;
        data_ptr->is_bar_cand_dirty = TRUE;
        }
        }
        else
         */
        if (MMIIM_SP_STATE_SELECTING == data_ptr->state
                || MMIIM_SP_STATE_ASSOCIATE == data_ptr->state
                || GUIIM_TYPE_STROKE == mode_type)
        {
            sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

            if (T9STATNONE == sStatus)
            {
                MMIIM_T9_GetChsCoreData(data_ptr);
                data_ptr->is_bar_cand_dirty = TRUE;
            }
        }
        /*
           else if (MMIIM_SP_STATE_ASSOCIATE == data_ptr->state)
           {
           sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

           if (T9STATNONE == sStatus)
           {
           MMIIM_T9_GetChsCoreData(data_ptr);
           data_ptr->is_bar_cand_dirty = TRUE;
           }
           }
         */
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : handle t9 vertical direction key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: not relate to the cursor in editbox, just handle by input method with smart mode
/*****************************************************************************/
PUBLIC BOOLEAN HandleVerDirectKeyMsg(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_SP_VKEY_T key
        )
{
    BOOLEAN is_dealt = FALSE;
    T9STATUS sStatus = T9STATERROR;
    T9UINT nValue = 0;
    MMIIM_TEXT_BAR_T *bar_ptr = PNULL;


    if (PNULL == data_ptr || ((MMIIM_SP_KEY_UP != key) && (MMIIM_SP_KEY_DOWN != key)))
    {
        SCI_TRACE_LOW("[MMIIM] HandleHorDirectKeyMsg: wrong param, msg_id=%d.", key);
        return is_dealt;
    }

    is_dealt = TRUE;
    if (MMIIM_SP_STATE_INPUTING == data_ptr->state)
    {
        uint16 nIdx = 0;
        uint16 nUnderLen = 0;
        T9SYMB *text_ptr = PNULL;

        bar_ptr = &data_ptr->bar_edit;

        if (bar_ptr->seg_cnt <= 1)
        {
            SCI_TRACE_LOW("[MMIIM] HandleHorDirectKeyMsg: there is only %d phonetic spelling.", 
                    bar_ptr->seg_cnt);
            return is_dealt;
        }

        nValue = bar_ptr->hot_index + 1;  //T9 begin with 1

        if (MMIIM_SP_KEY_UP == key)
        {
            if (1 != nValue)
            {
                nValue--;
            }
        }
        else if (MMIIM_SP_KEY_DOWN == key)
        {
            if (nValue < bar_ptr->seg_cnt)
            {
                nValue++;
            }
        }

        // if changes the nvalue, it meanings that can change the select choice pinyin
        if (nValue != bar_ptr->hot_index + 1)
        {
            sStatus = MMIIM_T9_SpecifySpell(data_ptr, nValue);

            if (T9STATNONE != sStatus)
            {
                //not succeed, than print
                SCI_TRACE_LOW("[MMIIM] HandleHorDirectKeyMsg: return status=%d", sStatus);
            }

            if (T9STATNONE == sStatus)
            {
                MMIIM_T9_GetChsCoreData(data_ptr);
                //data_ptr->is_rect_change = TRUE;
                data_ptr->is_bar_edit_dirty = TRUE;
                data_ptr->is_bar_cand_dirty = TRUE;

                nIdx = data_ptr->bar_edit.hot_index;
                nUnderLen = data_ptr->bar_edit.seg[nIdx].len;
                text_ptr = data_ptr->bar_edit.seg[nIdx].word_ptr;
                MMIIM_ReplaceChars(&data_ptr->init_param, text_ptr, 0, nUnderLen);
            }
        }        
    }
    else if (MMIIM_SP_STATE_SELECTING == data_ptr->state)
    {
        if (MMIIM_SP_KEY_UP == key)
        {
            MoveStepsOnCandBar(&data_ptr->bar_cand, 0);
        }
        else
        {
            MoveStepsOnCandBar(&data_ptr->bar_cand, 1);
        }
        data_ptr->is_bar_cand_dirty = TRUE;
    }
    else if (MMIIM_SP_STATE_ASSOCIATE == data_ptr->state)
    {
        //may be we should do something now ... change between words
    }


    return is_dealt;
}

/*****************************************************************************/
//  Description : handle long left key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: only work in noinput state
/*****************************************************************************/
LOCAL BOOLEAN HandleLongLeftKeyMsg(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    BOOLEAN is_dealt = FALSE;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

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
//  Note: only work in noinput state
/*****************************************************************************/
LOCAL BOOLEAN HandleLongRightKeyMsg(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    BOOLEAN is_dealt = FALSE;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

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
        MMIIM_T9_CHS_DATA_T *data_ptr,
        MMIIM_SP_VKEY_T key
        )
{
    BOOLEAN is_dealt = FALSE;
    wchar commit_char = 0;
    T9CCFieldInfo *cc_field_ptr = PNULL;

    if (PNULL == data_ptr)
    {
        return is_dealt;
    }

    if (MMIIM_SP_KEY_0 > key
            || MMIIM_SP_KEY_9 < key)
    {
        return is_dealt;
    }

    is_dealt = TRUE;

    cc_field_ptr = &data_ptr->cc_field_info;

    //just commit the number whenever state.
    commit_char = key - MMIIM_SP_KEY_0 + '0';

    MMIIM_T9_AddExplicitChar(&data_ptr->cc_field_info.G, commit_char);
    CommitChar(data_ptr, commit_char);

    data_ptr->last_key = MMIIM_SP_KEY_NONE;

    // set to NoInput state after valid number key long pressed
    SetToNoInput(data_ptr);
#ifdef UI_MULTILAYER_SUPPORT	
    MMIIM_PopupListReleaseLayer(&data_ptr->bar_cand);
    MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
    MMK_UpdateScreen();
#endif

    return is_dealt;
}

/*****************************************************************************/
//  Description : handle cancel key message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleCancelKeyMsg(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    BOOLEAN is_dealt = FALSE;
    T9FieldInfo *filed_info_ptr = PNULL;
    T9STATUS sStatus = T9STATERROR;
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;

    if (PNULL == data_ptr ||
            PNULL == data_ptr->init_param.method_param_ptr)
    {
        return is_dealt;
    }

    filed_info_ptr = &data_ptr->cc_field_info.G;
    mode_type = data_ptr->init_param.method_param_ptr->im_set.type;

    if (MMIIM_SP_STATE_NOINPUT == data_ptr->state)
    {
        if (T9STATNONE != sStatus)/*lint !e774*/
        {
            //force to adjust current IME cursor pos(for Up/Down key might had been pressed before)
            SetIMECursorPos(data_ptr);
            //delete a char from engine			
            sStatus = MMIIM_T9_HandleKey(filed_info_ptr, T9KEYCLEAR);
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
    }
    else if (MMIIM_SP_STATE_INPUTING == data_ptr->state)
    {
        sStatus = MMIIM_T9_HandleKey(filed_info_ptr, T9KEYCLEAR);

        if (T9STATNONE == sStatus)
        {
            uint16 nIdx = 0;
            uint16 nUnderLen = 0;
            T9SYMB *text_ptr = PNULL;

            MMIIM_T9_GetChsCoreData(data_ptr);

            // the same handle with smart and stroke mode            
            if (0 == data_ptr->bar_edit.seg_cnt)
            {
                SetToNoInput(data_ptr);
                CommitChar(data_ptr, '\b');
#ifdef UI_MULTILAYER_SUPPORT	
                MMIIM_PopupListReleaseLayer(&data_ptr->bar_cand);
                MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
                MMK_UpdateScreen();
#endif
            }
            else
            {
                nIdx = data_ptr->bar_edit.hot_index;
                nUnderLen = data_ptr->bar_edit.seg[nIdx].len;
                text_ptr = data_ptr->bar_edit.seg[nIdx].word_ptr;

                data_ptr->is_bar_edit_dirty = TRUE;
                data_ptr->is_bar_cand_dirty = TRUE;
            }
            MMIIM_ReplaceChars(&data_ptr->init_param, text_ptr, 0, nUnderLen);
        }
        is_dealt = TRUE;
    }
    else if (MMIIM_SP_STATE_SELECTING == data_ptr->state)
    {
        SetToInputing(data_ptr);
        //data_ptr->is_rect_change = TRUE;
        //data_ptr->is_bar_edit_dirty = TRUE;
        data_ptr->is_bar_cand_dirty = TRUE;
        is_dealt = TRUE;
    }
    else if (MMIIM_SP_STATE_ASSOCIATE == data_ptr->state)
    {
#if 0//no label needed
        if (data_ptr->bar_cand.has_label)
        {
            data_ptr->bar_cand.has_label = 0;
            data_ptr->bar_cand.has_hot = 1;
            data_ptr->bar_cand.hot_index = 0;
            data_ptr->is_bar_cand_dirty = TRUE;
        }
        else
#endif
        {
            SetToNoInput(data_ptr);
            //CommitChar(data_ptr, '\b');//don't execute the backspace operation twice
#ifdef UI_MULTILAYER_SUPPORT	
            MMIIM_PopupListReleaseLayer(&data_ptr->bar_cand);
            MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
            MMK_UpdateScreen();
#endif
        }
        is_dealt = TRUE;
    }

    return is_dealt;
}

/*****************************************************************************/
//  Description : move hot char in candidate bar
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: direction 0: left, 1:right
/*****************************************************************************/
LOCAL BOOLEAN MoveStepsOnCandBar(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        uint16 direction
        )
{
    uint16 cur_hot_index = 0;
    BOOLEAN is_ret = FALSE;
    T9KEY eKeyID = 0;
    T9STATUS sStatus = T9STATERROR;
    MMIIM_TEXT_BAR_T *bar_ptr = PNULL;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MoveStepsOnCandBar: data_ptr is null.");
        return FALSE;
    }

    bar_ptr = &data_ptr->bar_cand;
    /*MMI_UNISOC_DELETE
      if (bar_ptr->seg_cnt <= 1)
      {
      SCI_TRACE_LOW("[MMIIM] MoveStepsOnCandBar: there is only %d seg_cnt.",
      bar_ptr->seg_cnt);
      return TRUE;
      }
     */
    cur_hot_index = bar_ptr->hot_index;

    if (0 == direction)
    {
        eKeyID = T9KEYPREV;
        if (cur_hot_index > 0)
        {
            cur_hot_index--;
        }
    }
    else if (1 == direction)
    {
        eKeyID = T9KEYNEXT;
        if (cur_hot_index <  bar_ptr->seg_cnt - 1)
        {
            cur_hot_index++;
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIIM] MoveStepsOnCandBar: direction is %d ,this is error.",
                direction);
    }

    //we should support to change page for cand and modify the hot index
    //as 0 or (seg_cnt-1) according to the different direction
    if (cur_hot_index == bar_ptr->hot_index)
    {
        T9UINT cur_page_index = data_ptr->cc_field_info.nSelectPageNum;
        sStatus = MMIIM_T9_HandleKey(&data_ptr->cc_field_info.G, eKeyID);

        if (T9STATNONE == sStatus)
        {
            MMIIM_T9_GetChsCoreData(data_ptr);
            //hot index can only be modified when the page changed!!!
            if (cur_page_index > data_ptr->cc_field_info.nSelectPageNum)
            {
                cur_hot_index = bar_ptr->seg_cnt - 1;
            } else if (cur_page_index < data_ptr->cc_field_info.nSelectPageNum)
            {
                cur_hot_index = 0;
            } else
            {
                //do nothing...
            }
            //data_ptr->is_bar_cand_dirty = TRUE;
        }
    }

    bar_ptr->hot_index = cur_hot_index;
    data_ptr->is_bar_cand_dirty = TRUE;

    is_ret = TRUE;
    return is_ret;
}

/*****************************************************************************/
//  Description : get the identify char from engin
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSelectCharFromEngin(
        MMIIM_T9_CHS_DATA_T *data_ptr,
        uint16 key_index,
        T9SYMB *cmt_char_ptr
        )
{
    BOOLEAN is_ret = FALSE;

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] GetSelectCharFromEngin: data_ptr is null.");
        return FALSE;
    }

    if (key_index >= data_ptr->cc_field_info.nSelectPageLen)
    {
        SCI_TRACE_LOW("[MMIIM] T9_AddExplicitChar: invalid key_index=%d, pagelen=%d", 
                key_index, data_ptr->cc_field_info.nSelectPageLen);
        return is_ret;
    }

    *cmt_char_ptr = data_ptr->cc_field_info.pwSelectPage[key_index];

    is_ret = TRUE;
    return is_ret;
}

/******************************************************************************/
// Description:
// Global resource dependence: NONE
// Author: arvin.zhang
// Note: only consider the single textbar rect
/******************************************************************************/
LOCAL void AdjustTextBarRect(GUI_RECT_T *cursor_rect_ptr, MMIIM_TEXT_BAR_T *textbar_ptr, BOOLEAN is_need_border)
{
    uint8 seg_idx = 0;
    uint16 delta_height = 0;
    uint16 textbar_width = 0;
    uint16 textbar_height = 0;

    if (PNULL == textbar_ptr || PNULL == cursor_rect_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] AdjustTextBarRect: textbar_ptr or cursor_rect_ptr is null.");
        return;
    }

    // calculate the max width of the bar_edit currently
    for (seg_idx = 0; seg_idx < textbar_ptr->seg_cnt; seg_idx++)
    {
        if (0 == textbar_ptr->seg[seg_idx].len) continue;
        textbar_width = MAX(textbar_width, textbar_ptr->seg[seg_idx].width);
    }
    // add bar space and margin
    textbar_width += (2*MMIIM_TEXT_EDIT_BAR_SPACE + 2*MMIIM_TEXT_BAR_MARGIN + 2*MMIIM_TEXT_BAR_BORDER_WIDTH);

    // just top and bottom border plus if border needed and each word's height is 24
    // plus top and bottom border into each word to enable its height is 30
    delta_height = MMIIM_TEXT_BAR_BORDER_WIDTH * (is_need_border ? 2 : 2*textbar_ptr->seg_cnt);

    // Should calculate the current page to set the correct display rect based on hot_index 
    if (0 != textbar_ptr->hot_index)
    {
        uint16 start_idx = ((textbar_ptr->hot_index)/MMIIM_T9_CC_SELECTPAGE_NUM)*MMIIM_T9_CC_SELECTPAGE_NUM;
        uint16 end_idx = MIN((start_idx + MMIIM_T9_CC_SELECTPAGE_NUM - 1), (textbar_ptr->seg_cnt));
        textbar_height = MIN((end_idx - start_idx + 1), (textbar_ptr->seg_cnt - start_idx)) * MMIIM_TEXT_BAR_EDIT_HEIGHT + delta_height;
    }
    else
    {
        textbar_height = MIN(textbar_ptr->seg_cnt, MMIIM_T9_CC_SELECTPAGE_NUM) * MMIIM_TEXT_BAR_EDIT_HEIGHT + delta_height;
    }

    // calculate e-rect.left and e-rect.top according to the MMIIM_UpdatePredictBarPosition
    if(cursor_rect_ptr->left >= (MMI_MAINSCREEN_WIDTH - textbar_width - 4*MMIIM_TEXT_EDIT_BAR_SPACE))
    {
        textbar_ptr->rect.left = MMI_MAINSCREEN_WIDTH - textbar_width - 4*MMIIM_TEXT_EDIT_BAR_SPACE;
    }
    else
    {
        textbar_ptr->rect.left = (int16)(cursor_rect_ptr->left);
    }
    // do not modify anything on height temp for it's same as english
    if(cursor_rect_ptr->bottom < (MMI_MAINSCREEN_HEIGHT/2 + MMIIM_TEXT_BAR_CAND_HEIGHT))
    {
        textbar_ptr->rect.top = (int16)(cursor_rect_ptr->bottom + 1);
        textbar_ptr->is_display_upper_side = 0;////important!!!
    }
    else
    {
        textbar_ptr->rect.top = (int16)(cursor_rect_ptr->top - textbar_height - MMIIM_TEXT_BAR_BORDER_WIDTH);
        textbar_ptr->is_display_upper_side = 1;////important!!!
    }

    textbar_ptr->rect.right = textbar_ptr->rect.left + textbar_width;
    textbar_ptr->rect.bottom = textbar_ptr->rect.top + textbar_height - 1;
}
/******************************************************************************/
// Description: Config the rect of EditBar and CandBar for chs based on different
// 				state
// Global resource dependence: NONE
// Author: arvin.zhang
// Note: EditBar's width is dynamic changing and CandBar's width is fixed!!!
/******************************************************************************/
LOCAL void UpdateTextBarsPosition(MMIIM_T9_CHS_DATA_T *data_ptr, BOOLEAN is_need_border)
{
    uint16 candbar_width = 0;
    GUI_RECT_T   cursor_rect = {0};
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;
    MMIIM_TEXT_BAR_T *editbar_ptr = PNULL;
    MMIIM_TEXT_BAR_T *candbar_ptr = PNULL;
    GUI_RECT_T   old_editbar_rect = {0};
    GUI_RECT_T   old_candbar_rect = {0};

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] UpdateTextBarPosition: data_ptr is null.");
        return;
    }

    GUICTRL_SendNotifyEx(data_ptr->ctrl_handle, MSG_NOTIFY_IM_GET_CURSOR_RECT, &cursor_rect);

    editbar_ptr = &(data_ptr->bar_edit);
    candbar_ptr = &(data_ptr->bar_cand);
    old_editbar_rect = editbar_ptr->rect;
    old_candbar_rect = candbar_ptr->rect;

    mode_type = data_ptr->init_param.method_param_ptr->im_set.type;

    // There's no need to calculate the editbar rect if it's Stroke mode or associate state
    if ((MMIIM_SP_STATE_ASSOCIATE == data_ptr->state) || (GUIIM_TYPE_STROKE == mode_type))
    {
        editbar_ptr->rect.left = editbar_ptr->rect.right = 0;
        editbar_ptr->rect.top = editbar_ptr->rect.bottom = 0;

        AdjustTextBarRect(&cursor_rect, candbar_ptr, is_need_border);

        if(candbar_ptr->rect.left >= (MMI_MAINSCREEN_WIDTH - MMIIM_TEXT_POPUP_WIDTH - 2*MMIIM_TEXT_EDIT_BAR_SPACE))
        {
            candbar_ptr->rect.left = MMI_MAINSCREEN_WIDTH - MMIIM_TEXT_POPUP_WIDTH - 2*MMIIM_TEXT_EDIT_BAR_SPACE;
        }
        candbar_ptr->rect.right = candbar_ptr->rect.left + MMIIM_TEXT_POPUP_WIDTH + 2*MMIIM_TEXT_POPUP_MARGIN;
    }
    else
    {
        uint8 seg_idx = 0;
        uint16 textbar_width = 0;

        //we should handle the editBar first if exists because it will affect the candBar position
        AdjustTextBarRect(&cursor_rect, editbar_ptr, is_need_border);
        editbar_ptr->rect.right -= MMIIM_TEXT_BAR_BORDER_WIDTH;//no right border width for spellbar if not associate state

        AdjustTextBarRect(&cursor_rect, candbar_ptr, is_need_border);
        candbar_ptr->rect.right -= MMIIM_TEXT_BAR_BORDER_WIDTH;//no left border width for candbar if spellbar exists

        if((MMI_MAINSCREEN_WIDTH - editbar_ptr->rect.right - 1) >= (candbar_ptr->rect.right - candbar_ptr->rect.left + 1))
        {
            // the left rect is enough to fit candbar, just move candbar rect next the editbar's
            textbar_width = candbar_ptr->rect.right - candbar_ptr->rect.left + 1;
            candbar_ptr->rect.left = editbar_ptr->rect.right + 1;
            candbar_ptr->rect.right = candbar_ptr->rect.left + textbar_width + 1;
        }
        else
        {
            // the left space is not enough to fix candbar, first fix the candbar rect
            // and then move the editbar to its left direction accordingly
            textbar_width = candbar_ptr->rect.right - candbar_ptr->rect.left + 1;
            candbar_ptr->rect.right = MMI_MAINSCREEN_WIDTH - MMIIM_TEXT_BAR_BORDER_WIDTH;// maybe -1...
            candbar_ptr->rect.left = editbar_ptr->rect.right - textbar_width + 1;

            textbar_width = editbar_ptr->rect.right - editbar_ptr->rect.left + 1;//stored
            editbar_ptr->rect.right = candbar_ptr->rect.left - 1;
            editbar_ptr->rect.left = editbar_ptr->rect.right - textbar_width + 1;
        }

        // should adjust the editbar rect if the mode is is_display_upper_side according to the rect of
        // candbar so that we can avoid to display the candbar...
        if (editbar_ptr->is_display_upper_side != candbar_ptr->is_display_upper_side)
        {
            // the two rect does not display on the same side of the editor
            // should adjust the top & bottom value of the lower rect
        }

        if (editbar_ptr->is_display_upper_side && candbar_ptr->rect.top != editbar_ptr->rect.top)
        {
            uint16 textbar_height = 0;
            //if (candbar_ptr->rect.bottom == editbar_ptr->rect.bottom)
            {
                if (editbar_ptr->rect.top < candbar_ptr->rect.top)
                {
                    data_ptr->is_bar_cand_dirty = TRUE;
                    textbar_height = candbar_ptr->rect.bottom - candbar_ptr->rect.top + 1;
                    candbar_ptr->rect.top = editbar_ptr->rect.top;
                    candbar_ptr->rect.bottom = candbar_ptr->rect.top + textbar_height;
                } 
                else
                {
                    //data_ptr->is_bar_edit_dirty = TRUE;
                    textbar_height = editbar_ptr->rect.bottom - editbar_ptr->rect.top + 1;
                    editbar_ptr->rect.top = candbar_ptr->rect.top;
                    editbar_ptr->rect.bottom = editbar_ptr->rect.top + textbar_height;
                }
            }
#if 0//MMI_DELETE_UNISOC
            else
            {
                uint16 editbar_height = editbar_ptr->rect.bottom - editbar_ptr->rect.top + 1;
                uint16 candbar_height = candbar_ptr->rect.bottom - candbar_ptr->rect.top + 1;
                uint16 based_bottom = MAX(editbar_ptr->rect.bottom, candbar_ptr->rect.bottom);

                if (editbar_height >= candbar_height)
                {
                    editbar_ptr->rect.bottom = based_bottom;
                    editbar_ptr->rect.top = based_bottom - editbar_height - 1;
                    candbar_ptr->rect.top = editbar_ptr->rect.top;
                    candbar_ptr->rect.bottom = candbar_ptr->rect.top + candbar_height - 1;
                } 
                else
                {
                    candbar_ptr->rect.bottom = based_bottom;
                    candbar_ptr->rect.top = based_bottom - candbar_height - 1;
                    editbar_ptr->rect.top = candbar_ptr->rect.top;
                    editbar_ptr->rect.bottom = editbar_ptr->rect.top + editbar_height - 1;
                }
                data_ptr->is_bar_edit_dirty = TRUE;
            }
#endif//MMI_DELETE_UNISOC
        }
    }
#ifndef UI_MULTILAYER_SUPPORT	
    if ((!GUI_EqualRect(old_editbar_rect, editbar_ptr->rect)) ||( !GUI_EqualRect(old_candbar_rect, candbar_ptr->rect)))
    {
        MMK_UpdateScreen();
    }
#endif
}

/******************************************************************************/
// Description: reset to no input state and clean the underline string if needed
// Global resource dependence: NONE
// Author: arvin.zhang
// Note: called under the scenes : LOSE_FOCUS,HASH_KEY(to switch im)
/******************************************************************************/
LOCAL void ResetToNoInput(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    /***
     * To fix the auto-commit issue on LOSE_FOCUS msg, we had to adopt the following
     * ugly way: commit an specific char ' ' to editor and then delete it immediately. This
     * is because the length of the underline string is not fixed and there's no API to delete
     * the highlight string in editor available currently. Hence, it seems that the following
     * approach is very simple and no risk to commit an specific char to replace the underline
     * string, and then the added char will be deleted at once with the fixed length 1.
     * NOTE: the way must be matched with the patch on ctrlim handler under the same msg.
     ***/

    GUIIM_CONTEXT_T im_context = {0};

    if (PNULL == data_ptr)
    {
        return;
    }

    MMIIM_GetEditContent(&data_ptr->init_param, &im_context);

    if (PNULL != im_context.str_ptr && im_context.is_underline)
    {	
        CommitChar(data_ptr, ' ');
        CommitChar(data_ptr, '\b');
    }

    SetToNoInput(data_ptr);
    //MMIIM_PopupListClearLayer(&data_ptr->bar_cand);
    //MMIIM_PopupListClearLayer(&data_ptr->bar_edit);
#ifdef UI_MULTILAYER_SUPPORT	
    MMIIM_PopupListReleaseLayer(&data_ptr->bar_cand);
    MMIIM_PopupListReleaseLayer(&data_ptr->bar_edit);
#else
    //MMK_UpdateScreen(); // 去掉刷新屏幕操作（Bug-1019108，Bug-1025078）
#endif
}

//#endif //#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)
