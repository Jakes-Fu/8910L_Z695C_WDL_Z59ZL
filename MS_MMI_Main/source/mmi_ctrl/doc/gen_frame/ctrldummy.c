/*****************************************************************************
** File Name:      ctrl[ctrl].c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"

#include "ctrl[ctrl].h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

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
//  Description : init [ctrl] class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlInitVtbl(
    CTRL[CTRL]_VTBL_T        *[ctrl]_vtbl_ptr
);

/*****************************************************************************/
//  Description : [ctrl] construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRL[CTRL]_INIT_PARAM_T   *[ctrl]_param_ptr
);

/*****************************************************************************/
//  Description : [ctrl] destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void [Ctrl]CtrlPackInitParam (
    GUI[CTRL]_INIT_DATA_T     *[ctrl]_init_ptr,   //in
    CTRL[CTRL]_INIT_PARAM_T   *[ctrl]_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle [ctrl] msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E [Ctrl]CtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : display [ctrl] control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void [Ctrl]Draw (
    CTRL[CTRL]_OBJ_T     *[ctrl]_ctrl_ptr
);

/*****************************************************************************/
//  Description : get [ctrl] pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRL[CTRL]_OBJ_T* Get[Ctrl]Ptr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]TypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : set control whether display progress
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlSetScrollbarEnabled (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
);

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
);

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get [ctrl] type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T [CTRL]_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_[PARENT]_TYPE,                                  // parent class type
                       "[ctrl]",                                         // control name
                       sizeof (CTRL[CTRL]_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) [Ctrl]CtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) [Ctrl]CtrlDestruct,      // control destructor function
                       sizeof (CTRL[CTRL]_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) [Ctrl]CtrlPackInitParam,     // convert init param
                       sizeof (CTRL[CTRL]_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) [Ctrl]CtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init [ctrl] class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlInitVtbl (
    CTRL[CTRL]_VTBL_T        *[ctrl]_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) [ctrl]_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = [Ctrl]CtrlHandleMsg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = [Ctrl]CtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = [Ctrl]CtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetScrollbarEnabled = [Ctrl]CtrlSetScrollbarEnabled; //有些控件不必实现
    base_ctrl_vtbl_ptr->SetUpDownLoop = [Ctrl]CtrlSetUpDownLoop; //有些控件不必实现
    base_ctrl_vtbl_ptr->GetSelectRect = [Ctrl]CtrlGetSelectRect; //有些控件不必实现
    base_ctrl_vtbl_ptr->GetSelectIndex = [Ctrl]CtrlGetSelectIndex; //有些控件不必实现
    base_ctrl_vtbl_ptr->SetSelectIndex = [Ctrl]CtrlSetSelectIndex; //有些控件不必实现

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void [Ctrl]CtrlPackInitParam (
    GUI[CTRL]_INIT_DATA_T     *[ctrl]_init_ptr,   //in
    CTRL[CTRL]_INIT_PARAM_T   *[ctrl]_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) [ctrl]_param_ptr;

    if (PNULL == [ctrl]_init_ptr || PNULL == [ctrl]_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = [ctrl]_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : [ctrl] construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRL[CTRL]_INIT_PARAM_T   *[ctrl]_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRL[CTRL]_OBJ_T     *[ctrl]_ctrl_ptr = (CTRL[CTRL]_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == [ctrl]_param_ptr)
    {
        return FALSE;
    }

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
//  Description : [ctrl] destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRL[CTRL]_OBJ_T     *[ctrl]_ctrl_ptr = (CTRL[CTRL]_OBJ_T*) base_ctrl_ptr;

    if (PNULL == [ctrl]_ctrl_ptr)
    {
        return FALSE;
    }

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
//  Description : handle [ctrl] msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E [Ctrl]CtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRL[CTRL]_OBJ_T     *[ctrl]_ctrl_ptr = (CTRL[CTRL]_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL == [ctrl]_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        [Ctrl]Draw ([ctrl]_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_LOSE_FOCUS:
    case MSG_CTL_CLOSE:
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        break;

    case MSG_TIMER:
        break;

    case MSG_APP_OK:
        break;

    case MSG_APP_WEB:
        break;

    case MSG_APP_CANCEL:
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_UP:
        break;
#endif

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_[CTRL]_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display [ctrl] control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void [Ctrl]Draw (
    CTRL[CTRL]_OBJ_T     *[ctrl]_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) [ctrl]_ctrl_ptr;

    if (PNULL == [ctrl]_ctrl_ptr)
    {
        return;
    }

    // TODO
    // ...
}

/*****************************************************************************/
//  Description : get [ctrl] pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRL[CTRL]_OBJ_T* Get[Ctrl]Ptr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT ([Ctrl]TypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRL[CTRL]_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]TypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_[CTRL]_TYPE);
}

/*****************************************************************************/
// 	Description : set control whether display progress
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlSetScrollbarEnabled (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_display
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 有滚动条功能的控件需要实现这个接口
    // ...

    return result;
}

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlSetUpDownLoop (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    BOOLEAN              is_circular
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口
    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlGetSelectRect (
    CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
    GUI_RECT_T             *rect_ptr       //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口
    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN [Ctrl]CtrlGetSelectIndex (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    BOOLEAN               *is_first_ptr,  //in/out:
    BOOLEAN               *is_last_ptr    //in/out:
)
{
    BOOLEAN     result = FALSE;

    // TODO
    // 注意: 像list等这种有多个item项的控件，需要实现这个接口
    // 其他单项控件，如button，不需要实现
    // ...

    return (result);
}

