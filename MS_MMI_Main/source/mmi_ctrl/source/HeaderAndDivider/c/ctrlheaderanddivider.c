/*****************************************************************************
** File Name:      ctrlheaderanddivider.c                                               *
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

#include "ctrlheaderanddivider.h"
#include "mmi_string.h"
#include "mmitheme_headeranddivider.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define ASCII_CODE_DOT                0x2E  // ascii code /
#define TEXT_TOP_OFFSET               8
#define TEXT_BOTTOM_OFFSET            8

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
// Description : init 
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void HeaderAndDividerInit( 
                     CTRLBASE_OBJ_T        *ctrl_ptr
                     );

/*****************************************************************************/
// Description : get headeranddivider control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLHEADERANDDIVIDER_OBJ_T *GetHeaderAndDividerPtr(
                                   MMI_HANDLE_T  win_handle,
                                   MMI_CTRL_ID_T ctrl_id
                                   );

/*****************************************************************************/
// Description : handle msg of headeranddivider
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HeaderAndDividerHandleMsg( 
                                  CTRLBASE_OBJ_T        *ctrl_ptr,
                                  MMI_MESSAGE_ID_E  msg_id,
                                  DPARAM            param
                                  );

/*****************************************************************************/
// Description : to calc the pix of window headeranddivider string
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL uint16 CalcHeaderAndDividerStrLenByPix(
                                  MMI_HANDLE_T win_handle,
                                  CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                  );

/*****************************************************************************/
// Description : to calc the pix of window headeranddivider string
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL uint16 CalcStrLenByPix( 
                             const MMI_STRING_T    *headeranddivider_str_ptr, 
                             CTRLHEADERANDDIVIDER_OBJ_T        *headeranddivider_ptr
                             );

/*****************************************************************************/
// Description : handle window timer message to update window headeranddivider if headeranddivider  
// string length larger then the headeranddivider's length. scroll as character
// Global resource dependence : 
// Author:Liqing.peng
// Note: 
/*****************************************************************************/
LOCAL void HandleWinHeaderAndDividerTimerAsCharMsg(
                                        CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                        );

/*****************************************************************************/
// Description : handle window timer message to update window headeranddivider if headeranddivider  
// string length larger then the headeranddivider's length. scroll as pixel
// Global resource dependence : 
// Author:Liqing.peng
// Note: 
/*****************************************************************************/
LOCAL void HandleWinHeaderAndDividerTimerAsPixelMsg(
                                         CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                         );

/*****************************************************************************/
// Description : handle window timer message to update window headeranddivider if headeranddivider  
// string length larger then the headeranddivider's length.
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void HandleWinHeaderAndDividerTimerMsg(
                                  CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                  );

/*****************************************************************************/
//  Description : start password timer id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateDividerLayer(
								 CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr,
                                  GUI_RECT_T            *divider_rect
                                  );

/*****************************************************************************/
// Description : display the window headeranddivider, is called by the MMI Kernel
// Global resource dependence : 
// Author:Liqing.peng
// Modify:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDivider(
                        CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                        );

/*****************************************************************************/ 
// Description : display the window headeranddivider, is called by the MMI Kernel 
// Global resource dependence :  
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayScrollHeaderAndDividerByOffset(
                                         const wchar*  str_ptr, //[IN] string pointer
                                         uint16  str_len, //[IN] string length (byte number)
                                         uint16 offset,//[in]scroll offset
                                         CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                         );

/*****************************************************************************/ 
// Description : display the window headeranddivider, is called by the MMI Kernel 
// Global resource dependence :  
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL uint16 GetTextRectWidth(
                              CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                              );

/*****************************************************************************/
// Description : get the headeranddivider text rect
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTextRect(
                             CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                             );

/*****************************************************************************/
// Description : display headeranddivider background
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerBg(
                          CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                          );

/*****************************************************************************/
// Description : display headeranddivider icon
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerLine(
									   CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
									   );

/*****************************************************************************/
// Description : start headeranddivider scroll timer
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void StartHeaderAndDividerScrollTimer(
                                 CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                 );

/*****************************************************************************/
// Description : stop headeranddivider scroll timer
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void StopHeaderAndDividerScrollTimer(
                                CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                );

/*****************************************************************************/
// Description : display headeranddivider text
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerText(
                            CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                            );

/*****************************************************************************/
// Description : 获取字符串
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
LOCAL MMI_STRING_T GetHeaderAndDividerText(
                                CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                );

/*****************************************************************************/
// Description : 获得文本的排版信息
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E GetFontAlign(
                               CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                               );

/*****************************************************************************/
// Description : 显示图片,封装接口的目的是不要重复调用透明色的接口
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerImg(
                           GUI_POINT_T            *display_point_ptr,
                           GUI_RECT_T             *display_rect_ptr,
                           GUI_RECT_T             *image_rect_ptr,
                           MMI_HANDLE_T           win_handle,
                           MMI_IMAGE_ID_T         img_id,       
                           const GUI_LCD_DEV_INFO *dev_info_ptr
                           );

/*****************************************************************************/ 
// Description : 根据文本区域确定背景图片的区域 
// Global resource dependence :  
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetHeaderAndDividerBgImgRect(
                                   CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr, 
                                   GUI_RECT_T text_rect
                                   );

/*****************************************************************************/ 
// Description : 根据文本区域显示背景
// Global resource dependence :  
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerTextBg(
                              CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                              );

/*****************************************************************************/
// Description : get title control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLHEADERANDDIVIDER_OBJ_T *GetDividerPtr(
                                   MMI_HANDLE_T  win_handle,
                                   MMI_CTRL_ID_T ctrl_id
                                   );

/*****************************************************************************/
// Description : headeranddivider set rect
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerSetRect( 
                           CTRLBASE_OBJ_T *iguictrl_ptr, 
                           const GUI_RECT_T *rect_ptr
                           );

/*****************************************************************************/ 
// Description : 获得headeranddivider控件的文本显示风格
// Global resource dependence :  
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void GetHeaderAndDividerTextStyle(
                             CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                             GUISTR_STYLE_T *style_ptr
                             );

/*****************************************************************************/
// Description : 释放字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ReleaseHeaderAndDividerStr(
                           CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                           );
#if 0
/*****************************************************************************/
// Description : headeranddivider set rect
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetHeaderAndDividerContentRect( 
                                    CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                                    );
#endif
/*****************************************************************************/
//  Description : init headeranddivider class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerCtrlInitVtbl(
    CTRLHEADERANDDIVIDER_VTBL_T        *headeranddivider_vtbl_ptr
);

/*****************************************************************************/
//  Description : headeranddivider construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLHEADERANDDIVIDER_INIT_PARAM_T   *headeranddivider_param_ptr
);

/*****************************************************************************/
//  Description : headeranddivider destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void HeaderAndDividerCtrlPackInitParam (
    GUIHEADERANDDIVIDER_INIT_DATA_T     *headeranddivider_init_ptr,   //in
    CTRLHEADERANDDIVIDER_INIT_PARAM_T   *headeranddivider_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle headeranddivider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HeaderAndDividerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get headeranddivider pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLHEADERANDDIVIDER_OBJ_T* GetHeaderAndDividerobjPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get header and divider type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T HEADERANDDIVIDER_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "headeranddivider",                                         // control name
                       sizeof (CTRLHEADERANDDIVIDER_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) HeaderAndDividerCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) HeaderAndDividerCtrlDestruct,      // control destructor function
                       sizeof (CTRLHEADERANDDIVIDER_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) HeaderAndDividerCtrlPackInitParam,     // convert init param
                       sizeof (CTRLHEADERANDDIVIDER_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) HeaderAndDividerCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init headeranddivider class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerCtrlInitVtbl (
    CTRLHEADERANDDIVIDER_VTBL_T        *headeranddivider_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) headeranddivider_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = HeaderAndDividerCtrlHandleMsg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->SetRect = HeaderAndDividerSetRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

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
LOCAL void HeaderAndDividerCtrlPackInitParam (
    GUIHEADERANDDIVIDER_INIT_DATA_T     *headeranddivider_init_ptr,   //in
    CTRLHEADERANDDIVIDER_INIT_PARAM_T   *headeranddivider_param_ptr   //out
)
{
    if (PNULL == headeranddivider_init_ptr || PNULL == headeranddivider_param_ptr)
    {
        return;
    }
}

/*****************************************************************************/
//  Description : headeranddivider construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLHEADERANDDIVIDER_INIT_PARAM_T   *headeranddivider_param_ptr
)
{	
    BOOLEAN              result = TRUE;
        
    if ((PNULL == base_ctrl_ptr)||(PNULL == headeranddivider_param_ptr))
    {
        return FALSE;
    }
    
    HeaderAndDividerInit(base_ctrl_ptr);
    
    IGUICTRL_SetState( (IGUICTRL_T *)base_ctrl_ptr, GUICTRL_STATE_DISABLE_ACTIVE, TRUE );
    
    return (result);
}

/*****************************************************************************/
//  Description : headeranddivider destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    uint32 i = 0;
    uint8 *layer_buf_ptr = PNULL;
    CTRLHEADERANDDIVIDER_OBJ_T        *headeranddivider_ptr = (CTRLHEADERANDDIVIDER_OBJ_T*)base_ctrl_ptr;
    
    if (PNULL == headeranddivider_ptr)
    {
        return TRUE;
    }
    
    ReleaseHeaderAndDividerStr(headeranddivider_ptr);
#ifdef UI_MULTILAYER_SUPPORT	
    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&headeranddivider_ptr->divider_layer_info);
    if(PNULL == layer_buf_ptr)
    {
        UILAYER_ReleaseLayer(&headeranddivider_ptr->divider_layer_info);
    }
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : handle headeranddivider msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HeaderAndDividerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLHEADERANDDIVIDER_OBJ_T     *headeranddivider_ctrl_ptr = (CTRLHEADERANDDIVIDER_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_HEADERANDDIVIDER_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);

    if (PNULL == base_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    headeranddivider_ctrl_ptr = (CTRLHEADERANDDIVIDER_OBJ_T *)base_ctrl_ptr;
        
    switch(msg_id)
    {
    case MSG_CTL_OPEN:
        break;

    case MSG_CTL_GET_FOCUS:
        break;

    case MSG_CTL_LOSE_FOCUS:
        StopHeaderAndDividerScrollTimer(headeranddivider_ctrl_ptr);
        break;

    case MSG_CTL_PAINT:
        DisplayHeaderAndDivider(headeranddivider_ctrl_ptr);
        break;

    case MSG_TIMER:      
        if((*(uint8*)param) == headeranddivider_ctrl_ptr->scroll_timer_id)
        {
            if( 0 != headeranddivider_ctrl_ptr->scroll_timer_id )
            {
                MMK_StopTimer(headeranddivider_ctrl_ptr->scroll_timer_id);
                headeranddivider_ctrl_ptr->scroll_timer_id = 0;
            }
            HandleWinHeaderAndDividerTimerMsg(headeranddivider_ctrl_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT 
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_LONG:    
    case MSG_TP_PRESS_MOVE:
        break;
#endif 

    case MSG_CTL_CLOSE:
        StopHeaderAndDividerScrollTimer(headeranddivider_ctrl_ptr);
        break;

    default:
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get headeranddivider pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLHEADERANDDIVIDER_OBJ_T* GetHeaderAndDividerobjPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (HeaderAndDividerTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLHEADERANDDIVIDER_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_HEADERANDDIVIDER_TYPE);
}

/*****************************************************************************/
// Description : init button
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void HeaderAndDividerInit( 
                     CTRLBASE_OBJ_T		*ctrl_ptr
                     )
{
    CTRLHEADERANDDIVIDER_OBJ_T   *headeranddivider_ptr = (CTRLHEADERANDDIVIDER_OBJ_T*)ctrl_ptr;
    MMI_HEADERANDDIVIDER_STYLE_T headeranddivider_style = {0};

    ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl( ctrl_ptr->handle );
    SCI_ASSERT(NULL != headeranddivider_ptr->base_ctrl.win_handle);/*assert verified*/

    MMITHEME_GetHeaderAndDividerStyle(&headeranddivider_style);
    ctrl_ptr->both_rect = MMITHEME_GetWinHeaderAndDividerBothRect(ctrl_ptr->win_handle);

    headeranddivider_ptr->scroll_type = HEADERANDDIVIDER_SCROLL_LEFT_OUT;

    // 初始化字符串
    headeranddivider_ptr->text_type = GUIHEADERANDDIVIDER_DATA_TEXT_ID;
    headeranddivider_ptr->headeranddivider_str.wstr_ptr = PNULL;
    headeranddivider_ptr->headeranddivider_str.wstr_len = 0;

    headeranddivider_ptr->str_style.align = headeranddivider_style.align_type;
    headeranddivider_ptr->str_style.font = headeranddivider_style.font;
    headeranddivider_ptr->str_style.font_color = headeranddivider_style.font_color;
    headeranddivider_ptr->str_style.char_space = headeranddivider_style.char_space;

    headeranddivider_ptr->bg_color = headeranddivider_style.bg_color;
    headeranddivider_ptr->line_color = headeranddivider_style.line_color;

    headeranddivider_ptr->is_need_bg = TRUE;

    ctrl_ptr->bg = MMITHEME_GetCommonBg();
    return;
}

/*****************************************************************************/
// Description : handle msg of button
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HeaderAndDividerHandleMsg( 
                                  CTRLBASE_OBJ_T        *ctrl_ptr,
                                  MMI_MESSAGE_ID_E  msg_id,
                                  DPARAM            param
                                  )
{
    MMI_RESULT_E      recode = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint	
    GUI_POINT_T       point = {0};
#endif
    CTRLHEADERANDDIVIDER_OBJ_T      *headeranddivider_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != ctrl_ptr); /*assert verified*/
    if (PNULL == ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    headeranddivider_ptr = (CTRLHEADERANDDIVIDER_OBJ_T *)ctrl_ptr;
    
    switch(msg_id)
    {
    case MSG_CTL_OPEN:
        break;

    case MSG_CTL_GET_FOCUS:
        break;

    case MSG_CTL_LOSE_FOCUS:
        StopHeaderAndDividerScrollTimer(headeranddivider_ptr);
        break;

    case MSG_CTL_PAINT:
        DisplayHeaderAndDivider(headeranddivider_ptr);
        break;

    case MSG_TIMER:      // @Rui
        if((*(uint8*)param) == headeranddivider_ptr->scroll_timer_id)
        {
            if( 0 != headeranddivider_ptr->scroll_timer_id )
            {
                MMK_StopTimer(headeranddivider_ptr->scroll_timer_id);
                headeranddivider_ptr->scroll_timer_id = 0;
            }
            HandleWinHeaderAndDividerTimerMsg(headeranddivider_ptr);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_LONG:    
    case MSG_TP_PRESS_MOVE:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_CLOSE:
        StopHeaderAndDividerScrollTimer(headeranddivider_ptr);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
// Description : headeranddivider set rect
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN HeaderAndDividerSetRect( 
                           CTRLBASE_OBJ_T *iguictrl_ptr, 
                           const GUI_RECT_T *rect_ptr
                           )
{
    BOOLEAN           result = FALSE;
    CTRLHEADERANDDIVIDER_OBJ_T   *headeranddivider_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL != iguictrl_ptr && PNULL != rect_ptr ); /*assert verified*/
    if ((PNULL == iguictrl_ptr)||(PNULL == rect_ptr))
    {
        return FALSE;
    }

    headeranddivider_ptr = (CTRLHEADERANDDIVIDER_OBJ_T *)iguictrl_ptr;

    if (!GUI_EqualRect(iguictrl_ptr->rect, *rect_ptr)
#ifdef GUIF_HEADERANDDIVIDER_PDASTYLE
        || MMITHEME_IsIstyle()
#endif
        )
    {
        iguictrl_ptr->rect = *rect_ptr;
        
        GUI_SetVOrHRect( iguictrl_ptr->handle, (GUI_RECT_T *)rect_ptr, &(iguictrl_ptr->both_rect));

        // 重设各项内容的区域
//         ResetHeaderAndDividerContentRect(headeranddivider_ptr);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// Description : 释放字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void ReleaseHeaderAndDividerStr(
                           CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                           )
{
    if (PNULL != headeranddivider_ptr)
    {
        if (PNULL != headeranddivider_ptr->headeranddivider_str.wstr_ptr)
        {
            SCI_FREE(headeranddivider_ptr->headeranddivider_str.wstr_ptr);
        }
        
        headeranddivider_ptr->headeranddivider_str.wstr_ptr = PNULL;
        headeranddivider_ptr->headeranddivider_str.wstr_len = 0;
    }
}

/*****************************************************************************/
// Description : to calc the pix of window headeranddivider string
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL uint16 CalcHeaderAndDividerStrLenByPix(
                                  MMI_HANDLE_T   win_handle,
                                  CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                  )
{

    MMI_STRING_T  headeranddivider_str = {0};
    uint16        length = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return 0;
    }

    headeranddivider_str = GetHeaderAndDividerText(headeranddivider_ptr);
    
    length = CalcStrLenByPix(&headeranddivider_str, headeranddivider_ptr);

    return (length);
}

/*****************************************************************************/
// Description : to calc the pix of window headeranddivider string
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
LOCAL uint16 CalcStrLenByPix(
                             const MMI_STRING_T    *headeranddivider_str_ptr,
                             CTRLHEADERANDDIVIDER_OBJ_T        *headeranddivider_ptr
                             )
{

    uint16          length = 0;
 
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_str_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if ((PNULL == headeranddivider_str_ptr)||(PNULL == headeranddivider_ptr))
    {
        return 0;
    }
    
    length = GUI_CalculateStringPiexlNum(
                        headeranddivider_str_ptr->wstr_ptr,
                        headeranddivider_str_ptr->wstr_len,
                        headeranddivider_ptr->str_style.font,
                        headeranddivider_ptr->str_style.char_space);

    return (length);
}

/*****************************************************************************/
// Description : get headeranddivider control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLHEADERANDDIVIDER_OBJ_T *GetHeaderAndDividerPtr(
                                   MMI_HANDLE_T  win_handle,
                                   MMI_CTRL_ID_T ctrl_id
                                   )
{
    CTRLHEADERANDDIVIDER_OBJ_T* headeranddivider_ptr = PNULL;
    MMI_HANDLE_T ctrl_handle = 0;
    
    if(0 != win_handle)
    {
        win_handle = MMK_ConvertIdToHandle( win_handle );
        
        ctrl_handle = MMK_ConvertIdToHandle( ctrl_id );

        if (0 != ctrl_id && ctrl_handle == ctrl_id)
        {
            headeranddivider_ptr = (CTRLHEADERANDDIVIDER_OBJ_T*)MMK_GetCtrlPtr(ctrl_id);
        }
        else
        {
            headeranddivider_ptr = (CTRLHEADERANDDIVIDER_OBJ_T*)MMK_GetCtrlPtrByWin(win_handle, ctrl_id);
        }

        if(PNULL != headeranddivider_ptr)
        {
            SCI_ASSERT(HeaderAndDividerTypeOf((CTRLBASE_OBJ_T*)headeranddivider_ptr ));/*assert verified*/
        }
    }
    
    return headeranddivider_ptr;
}

/*****************************************************************************/
// Description : handle window timer message to update window headeranddivider if headeranddivider  
// string length larger then the headeranddivider's length.
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void HandleWinHeaderAndDividerTimerMsg(
                                  CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                                  )
{
    if( MMITHEME_GetIsScrollAsPixel() )
    {
        HandleWinHeaderAndDividerTimerAsPixelMsg(headeranddivider_ptr);
    }
    else
    {
        HandleWinHeaderAndDividerTimerAsCharMsg(headeranddivider_ptr);
    }    
}

/*****************************************************************************/
// Description : handle window timer message to update window headeranddivider if headeranddivider  
// string length larger then the headeranddivider's length. scroll as character
// Global resource dependence : 
// Author:Liqing.peng
// Note: 
/*****************************************************************************/
LOCAL void HandleWinHeaderAndDividerTimerAsCharMsg(
                                        CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                        )
{
    uint16          length = 0;
    MMI_STRING_T    headeranddivider_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }

    headeranddivider_str = GetHeaderAndDividerText(headeranddivider_ptr);
    
    //得到当前要显示的窗口标题的长度，以PX记.
    length = CalcStrLenByPix(&headeranddivider_str, headeranddivider_ptr);

    // if headeranddivider string length larger then 10 byte , need to circular display.
    if (length > GetTextRectWidth(headeranddivider_ptr)) 
    {
        headeranddivider_ptr->is_text_overstep = TRUE;
        // disappeared from left byte by byte
        if (HEADERANDDIVIDER_SCROLL_LEFT_OUT == headeranddivider_ptr->scroll_type)   
        {
            if (headeranddivider_ptr->scroll_char_offset < headeranddivider_str.wstr_len)
            {
                headeranddivider_ptr->scroll_char_offset++;
                headeranddivider_str.wstr_ptr += headeranddivider_ptr->scroll_char_offset;
                headeranddivider_str.wstr_len  = (uint16)(headeranddivider_str.wstr_len - headeranddivider_ptr->scroll_char_offset);
            }
            else
            {
                headeranddivider_ptr->scroll_char_offset = 0;
            }
        }

        DisplayScrollHeaderAndDividerByOffset(headeranddivider_str.wstr_ptr, headeranddivider_str.wstr_len, 0, headeranddivider_ptr);

        if (MMK_IsFocusWin(headeranddivider_ptr->base_ctrl.win_handle) && HEADERANDDIVIDER_SCROLL_LEFT_OUT == headeranddivider_ptr->scroll_type)
        {
            headeranddivider_ptr->scroll_timer_id = MMK_CreateWinTimer(headeranddivider_ptr->base_ctrl.handle, headeranddivider_ptr->scroll_time, FALSE);
        }
    }
    else
    {
        headeranddivider_ptr->is_text_overstep = FALSE;
    }
}

/*****************************************************************************/
// Description : handle window timer message to update window headeranddivider if headeranddivider  
// string length larger then the headeranddivider's length. scroll as pixel
// Global resource dependence : 
// Author:Liqing.peng
// Note: 
/*****************************************************************************/
LOCAL void HandleWinHeaderAndDividerTimerAsPixelMsg(
                                         CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                                         )
{
    uint16          pixel_length = 0;
    MMI_STRING_T    headeranddivider_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }

    headeranddivider_str = GetHeaderAndDividerText(headeranddivider_ptr);

    //得到当前要显示的窗口标题的长度，以pixel记.
    pixel_length = CalcStrLenByPix(&headeranddivider_str, headeranddivider_ptr);

    // if headeranddivider string length larger then headeranddivider rect width , need to circular display.
    if (pixel_length > GetTextRectWidth(headeranddivider_ptr)) 
    {
        headeranddivider_ptr->is_text_overstep = TRUE;
        //disappeared from left byte by byte
        if (HEADERANDDIVIDER_SCROLL_LEFT_OUT == headeranddivider_ptr->scroll_type)    
        { 
            if ( headeranddivider_ptr->scroll_pixel_offset < pixel_length - headeranddivider_ptr->scroll_pixel_step)
            {
                headeranddivider_ptr->scroll_pixel_offset += headeranddivider_ptr->scroll_pixel_step;
            }
            else
            {
                headeranddivider_ptr->scroll_pixel_offset = 0;
            }
        }

        DisplayScrollHeaderAndDividerByOffset(headeranddivider_str.wstr_ptr, headeranddivider_str.wstr_len, headeranddivider_ptr->scroll_pixel_offset, headeranddivider_ptr);

        // 启动timer
        if (MMK_IsFocusWin(headeranddivider_ptr->base_ctrl.win_handle) && HEADERANDDIVIDER_SCROLL_LEFT_OUT == headeranddivider_ptr->scroll_type)
        {
            headeranddivider_ptr->scroll_timer_id = MMK_CreateWinTimer(headeranddivider_ptr->base_ctrl.handle, headeranddivider_ptr->scroll_time, FALSE);
        }
    }
    else
    {
        headeranddivider_ptr->is_text_overstep = FALSE;
    }
}

/****************************************************************************/    
//  Description: Create Pop Blink Layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateDividerLayer(
								 CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr,
                                  GUI_RECT_T            *divider_rect
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)headeranddivider_ptr;
	
    if ((PNULL == headeranddivider_ptr)||(PNULL == divider_rect))
    {
        return result;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
		
        //init pop layer info
        create_info.lcd_id       = base_ctrl_ptr->lcd_dev_info.lcd_id;
        create_info.owner_handle = base_ctrl_ptr->handle;
        create_info.offset_x     = divider_rect->left;
        create_info.offset_y     = divider_rect->top;
        create_info.width        = divider_rect->right - divider_rect->left + 1;
        create_info.height       = divider_rect->bottom - divider_rect->top + 1;
		
        //create pop layer
        if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &(headeranddivider_ptr->divider_layer_info)))
        {
            UILAYER_SetLayerColorKey(&(headeranddivider_ptr->divider_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);
            UILAYER_Clear(&(headeranddivider_ptr->divider_layer_info));
            result = TRUE;
        }       
    }
#endif
    return result;
}

/*****************************************************************************/
// Description : display the window headeranddivider, is called by the MMI Kernel
// Global resource dependence : 
// Author:Liqing.peng
// Modify:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDivider(
                        CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                        )
{

    UILAYER_APPEND_BLT_T append_layer_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }

    if (!IGUICTRL_GetState((IGUICTRL_T *)headeranddivider_ptr, GUICTRL_STATE_INVISIBLE))
    {
//         ResetHeaderAndDividerContentRect(headeranddivider_ptr);

        // 显示背景
        DisplayHeaderAndDividerBg(headeranddivider_ptr);
#ifdef UI_MULTILAYER_SUPPORT	
        UILAYER_ClearRect((const GUI_LCD_DEV_INFO*)&headeranddivider_ptr->divider_layer_info,headeranddivider_ptr->base_ctrl.rect);

        CreateDividerLayer(headeranddivider_ptr, &headeranddivider_ptr->base_ctrl.rect);

        append_layer_info.layer_level = UILAYER_LEVEL_NORMAL;
        append_layer_info.lcd_dev_info = headeranddivider_ptr->divider_layer_info;
		
        //append pop layer
        UILAYER_AppendBltLayer(&append_layer_info);
        UILAYER_SetLayerAlpha(&headeranddivider_ptr->divider_layer_info,125);
#endif
        //显示文本
        DisplayHeaderAndDividerText(headeranddivider_ptr);

        DisplayHeaderAndDividerLine(headeranddivider_ptr);

        //启动文本滚动定时器
        StartHeaderAndDividerScrollTimer(headeranddivider_ptr);
    }
}

/*****************************************************************************/ 
// Description : 根据文本区域确定背景图片的区域 
// Global resource dependence :  
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetHeaderAndDividerBgImgRect(
                                   CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr, 
                                   GUI_RECT_T text_rect
                                   )
{
    GUI_RECT_T image_rect = {0};

    image_rect.left = text_rect.left - headeranddivider_ptr->base_ctrl.rect.left;
    image_rect.top = text_rect.top - headeranddivider_ptr->base_ctrl.rect.top;
    image_rect.right = image_rect.left + text_rect.right - text_rect.left;
    image_rect.bottom = image_rect.top + text_rect.bottom - text_rect.top;

    return image_rect;
}

/*****************************************************************************/ 
// Description : 根据文本区域显示背景
// Global resource dependence :  
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerTextBg(
                              CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                              )
{
    GUI_RECT_T       image_rect = {0};
    GUI_RECT_T       text_rect =  GetTextRect(headeranddivider_ptr);
    GUI_BG_DISPLAY_T bg_display = {0};
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }
    
    if (headeranddivider_ptr->is_need_bg)
    {
        // 显示背景
        if (0 != headeranddivider_ptr->bg_image && headeranddivider_ptr->bg_image != MMITHEME_GetInvalidImageID())
        {
            image_rect = GetHeaderAndDividerBgImgRect(headeranddivider_ptr, text_rect);
            
            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect           = headeranddivider_ptr->base_ctrl.rect;
            bg_display.display_rect   = text_rect;
            bg_display.img_rect       = text_rect;
            bg_display.ctrl_handle    = headeranddivider_ptr->base_ctrl.handle;
            bg_display.win_handle     = headeranddivider_ptr->base_ctrl.win_handle;
           
            // common背景
//             GUI_DisplayBg(&headeranddivider_ptr->common_bg,
//                 &bg_display,
//                 &headeranddivider_ptr->lcd_dev_info);
             
            // 显示headeranddivider背景
            GUIRES_DisplaySabmImg(
                &(text_rect),
                &headeranddivider_ptr->base_ctrl.rect,//full rect
                &(image_rect),
                headeranddivider_ptr->base_ctrl.win_handle,
                headeranddivider_ptr->bg_image,
                &headeranddivider_ptr->base_ctrl.lcd_dev_info
                );

        }
        else if (0 != headeranddivider_ptr->bg_color)
        {
            LCD_FillRect(&headeranddivider_ptr->base_ctrl.lcd_dev_info, text_rect, headeranddivider_ptr->bg_color);
        }
    }
}

/*****************************************************************************/ 
// Description : 获得headeranddivider控件的文本显示风格
// Global resource dependence :  
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void GetHeaderAndDividerTextStyle(
                             CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                             GUISTR_STYLE_T *style_ptr
                             )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr && PNULL != style_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr || PNULL == style_ptr)
    {
        return;
    }

    style_ptr->align = GetFontAlign(headeranddivider_ptr);
    style_ptr->font = headeranddivider_ptr->str_style.font;
    style_ptr->font_color = headeranddivider_ptr->str_style.font_color;
    style_ptr->char_space = headeranddivider_ptr->str_style.char_space;
}

/*****************************************************************************/ 
// Description : display the window headeranddivider, is called by the MMI Kernel 
// Global resource dependence :  
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayScrollHeaderAndDividerByOffset(
                                         const wchar*  str_ptr, //[IN] string pointer
                                         uint16  str_len, //[IN] string length (byte number)
                                         uint16 offset,//[in]scroll offset
                                         CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                         )
{
    BOOLEAN recode    = TRUE;
    GUI_RECT_T disp_rect =  GetTextRect(headeranddivider_ptr);
    GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;
    MMI_STRING_T    str_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return FALSE;
    }
    
    GetHeaderAndDividerTextStyle(headeranddivider_ptr, &str_style);
    str_info.wstr_len = str_len;
    str_info.wstr_ptr = (wchar *)str_ptr;

    //显示背景
    DisplayHeaderAndDividerTextBg(headeranddivider_ptr);

    if (HEADERANDDIVIDER_SCROLL_LEFT_OUT != headeranddivider_ptr->scroll_type)    
    {
        str_state |= GUISTR_STATE_ELLIPSIS;
    }

    if ( (PNULL != str_ptr) && (0 < str_len) )
    {
        if( MMITHEME_GetIsScrollAsPixel() && HEADERANDDIVIDER_SCROLL_LEFT_OUT == headeranddivider_ptr->scroll_type)
        {
            GUISTR_DrawTextToLCDByOffset( 
                                         (const GUI_LCD_DEV_INFO*)&headeranddivider_ptr->divider_layer_info,
                                         (const GUI_RECT_T*)&disp_rect,
                                         PNULL,
                                         offset,
                                         0,
                                         (const MMI_STRING_T*)&str_info,
                                         &str_style,
                                         str_state
                                         );
        }
        else
        {
            GUISTR_DrawTextToLCDInRect( 
                                       (const GUI_LCD_DEV_INFO*)&headeranddivider_ptr->divider_layer_info,
                                       (const GUI_RECT_T*)&disp_rect,       //the fixed display area
                                       (const GUI_RECT_T*)&disp_rect,       //用户要剪切的实际区域
                                       (const MMI_STRING_T*)&str_info,
                                       &str_style,
                                       str_state,
                                       GUISTR_TEXT_DIR_AUTO
                                       );
        }
    }
    else
    {
        //SCI_TRACE_LOW:"DisplayScrollHeaderAndDividerByOffset: the param is error!"
//         SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIHEADERANDDIVIDER_1618_112_2_18_3_22_23_370,(uint8*)"");
        recode = FALSE;
    }

    return (recode);
}

/*****************************************************************************/ 
// Description : display the window headeranddivider, is called by the MMI Kernel 
// Global resource dependence :  
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL uint16 GetTextRectWidth(
                              CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                              )
{
    GUI_RECT_T    text_rect = GetTextRect(headeranddivider_ptr);

    return (uint16)(text_rect.right - text_rect.left + 1);
}

/*****************************************************************************/
// Description : get the headeranddivider text rect
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTextRect(
                             CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                             )
{
    GUI_RECT_T          rect = {0};
//     GUI_RECT_T          sub_rect = {0};
    uint16              left_icon_width = 0;
    uint16              right_icon_width = 0;
    MMI_HEADERANDDIVIDER_STYLE_T   headeranddivider_style = {0};
//     MMI_STRING_T    headeranddivider_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return rect;
    }
    //deleted by Tong Yongli. according to Coverity CID 25920
#if 0 
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }
#endif
//     headeranddivider_str = GetHeaderAndDividerText(headeranddivider_ptr);

    rect = headeranddivider_ptr->base_ctrl.rect;

    rect.top += TEXT_TOP_OFFSET;
    rect.bottom -= TEXT_BOTTOM_OFFSET;
	
	rect.left = rect.left + 6;
	rect.right -= headeranddivider_ptr->headeranddivider_text_margin;

    return rect;
}

/*****************************************************************************/
// Description : display headeranddivider background
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerBg(
                          CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                          )
{
    GUI_RECT_T       image_rect = {0};
    GUI_BG_DISPLAY_T bg_display = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }

    if (headeranddivider_ptr->is_need_bg)
    {
        // 显示背景
        if (0 != headeranddivider_ptr->bg_image && headeranddivider_ptr->bg_image != MMITHEME_GetInvalidImageID())
        {
            image_rect.left = 0;
            image_rect.top = 0;
            image_rect.right = headeranddivider_ptr->base_ctrl.rect.right - headeranddivider_ptr->base_ctrl.rect.left;
            image_rect.bottom = headeranddivider_ptr->base_ctrl.rect.bottom - headeranddivider_ptr->base_ctrl.rect.top;
            
            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect           = headeranddivider_ptr->base_ctrl.rect;
            bg_display.display_rect   = headeranddivider_ptr->base_ctrl.rect;
            bg_display.img_rect       = headeranddivider_ptr->base_ctrl.rect;
            bg_display.ctrl_handle    = headeranddivider_ptr->base_ctrl.handle;
            bg_display.win_handle     = headeranddivider_ptr->base_ctrl.win_handle;

            //headeranddivider_ptr->common_bg = MMITHEME_GetCommonBg();

//             GUI_DisplayBg(&headeranddivider_ptr->common_bg,
//                 &bg_display,
//                 &headeranddivider_ptr->lcd_dev_info);

            GUIRES_DisplayImg(
                NULL,
                &(headeranddivider_ptr->base_ctrl.rect),
                &(image_rect),
                headeranddivider_ptr->base_ctrl.win_handle,
                headeranddivider_ptr->bg_image,
                &headeranddivider_ptr->base_ctrl.lcd_dev_info
                );

        }
        else if (0 != headeranddivider_ptr->bg_color)
        {
            LCD_FillRect(&headeranddivider_ptr->base_ctrl.lcd_dev_info, headeranddivider_ptr->base_ctrl.rect, headeranddivider_ptr->bg_color);
        }
    }
}
/*****************************************************************************/
// Description : display headeranddivider icon
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerLine(
									   CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
									   )
{
	GUI_RECT_T			line_rect = {0};
	
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }

	line_rect = headeranddivider_ptr->base_ctrl.rect;
	line_rect.bottom -= 6;
	line_rect.top += 34;

// 	LCD_FillRect(&headeranddivider_ptr->base_ctrl.lcd_dev_info, line_rect, headeranddivider_ptr->line_color);
	LCD_FillRect(&headeranddivider_ptr->divider_layer_info, line_rect, headeranddivider_ptr->line_color);

}

/*****************************************************************************/
// Description : 显示图片,封装接口的目的是不要重复调用透明色的接口
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerImg(
                           GUI_POINT_T            *display_point_ptr,
                           GUI_RECT_T             *display_rect_ptr,
                           GUI_RECT_T             *image_rect_ptr,
                           MMI_HANDLE_T           win_handle,
                           MMI_IMAGE_ID_T         img_id,       
                           const GUI_LCD_DEV_INFO *dev_info_ptr
                           )
{
    IMG_EnableTransparentColor(TRUE);

    GUIRES_DisplayImg(display_point_ptr, 
        display_rect_ptr, 
        image_rect_ptr, 
        win_handle,
        img_id, 
        dev_info_ptr
        );

    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
// Description : start headeranddivider scroll timer
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void StartHeaderAndDividerScrollTimer(
                                 CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                                 )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }

    if( CalcHeaderAndDividerStrLenByPix(headeranddivider_ptr->base_ctrl.win_handle, headeranddivider_ptr) > GetTextRectWidth(headeranddivider_ptr) )
    {
        if (MMK_IsFocusWin(headeranddivider_ptr->base_ctrl.win_handle) && HEADERANDDIVIDER_SCROLL_LEFT_OUT == headeranddivider_ptr->scroll_type
            && 0 == headeranddivider_ptr->scroll_timer_id)
        {
            headeranddivider_ptr->scroll_timer_id = MMK_CreateWinTimer(headeranddivider_ptr->base_ctrl.handle, headeranddivider_ptr->scroll_time, FALSE);
        }
    }
}

/*****************************************************************************/
// Description : stop headeranddivider scroll timer
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void StopHeaderAndDividerScrollTimer(
                                CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }
    
    if( 0 != headeranddivider_ptr->scroll_timer_id )
    {
        MMK_StopTimer(headeranddivider_ptr->scroll_timer_id);
        headeranddivider_ptr->scroll_timer_id = 0;
    }
}

/*****************************************************************************/
// Description : 获得文本的排版信息
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E GetFontAlign(
                               CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                               )
{
    GUI_ALIGN_E align_type = ALIGN_HMIDDLE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return align_type;
    }
    
    switch ( headeranddivider_ptr->str_style.align )
    {
    case ALIGN_HMIDDLE:
        if (headeranddivider_ptr->is_text_overstep)
        {
            align_type = ALIGN_LVMIDDLE;
        }
        else
        {
            align_type = ALIGN_HVMIDDLE;
        }
        break;
        
    case ALIGN_LEFT:
        align_type = ALIGN_LEFT;
        break;
        
    case ALIGN_RIGHT:
        align_type = ALIGN_RIGHT;
        break;

    case ALIGN_LVMIDDLE:
        align_type = ALIGN_LVMIDDLE;
        break;
        
    default:
        align_type = ALIGN_HMIDDLE;
        break;
        
    }

    return align_type;
}

/*****************************************************************************/
// Description : set the window title string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDIVIDER_SetTextByPtr(
                                     CTRLHEADERANDDIVIDER_OBJ_T  *divider_ptr,
                                     const wchar*     wstr_ptr, //[IN] string pointer
                                     uint16           wstr_len, //[IN] string length, <=16
                                     BOOLEAN          is_need_update //[IN] is flash
                                     )
{
    BOOLEAN                result  = TRUE;
    
    if(PNULL == divider_ptr)
    {
        return FALSE;
    }

    StopHeaderAndDividerScrollTimer(divider_ptr);

    // 申请之前必须释放原来的内存，否则会产生内存泄漏
    ReleaseHeaderAndDividerStr(divider_ptr);

    if (0 != wstr_len && NULL != wstr_ptr)
    {
        divider_ptr->headeranddivider_str.wstr_ptr = SCI_ALLOC_APP(((wstr_len + 1)*sizeof(wchar)));
        SCI_MEMSET(divider_ptr->headeranddivider_str.wstr_ptr, 0, ((wstr_len + 1)*sizeof(wchar)));
        divider_ptr->headeranddivider_str.wstr_len = wstr_len;
        
        MMI_WSTRNCPY( divider_ptr->headeranddivider_str.wstr_ptr, divider_ptr->headeranddivider_str.wstr_len, wstr_ptr, wstr_len, wstr_len );

        GUI_ReplaceChar(&divider_ptr->headeranddivider_str, LF_CHAR, BL_CHAR);
        GUI_ReplaceChar(&divider_ptr->headeranddivider_str, CR_CHAR, BL_CHAR);
    }

    divider_ptr->text_type = GUITITLE_DATA_TEXT_BUFFER;

    // 重新设置title的长度和滚动起点
    divider_ptr->scroll_char_offset = 0;
    divider_ptr->scroll_pixel_offset = 0;
    divider_ptr->is_text_overstep = FALSE;
    
    if (is_need_update)
    {
        DisplayHeaderAndDivider(divider_ptr);
    }
    
    return (result);
}


/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN DividerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_HEADERANDDIVIDER_TYPE);
}
/*****************************************************************************/
// Description : get title control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLHEADERANDDIVIDER_OBJ_T *GetDividerPtr(
                                   MMI_HANDLE_T  win_handle,
                                   MMI_CTRL_ID_T ctrl_id
                                   )
{
    CTRLHEADERANDDIVIDER_OBJ_T* divider_ptr = PNULL;
    MMI_HANDLE_T ctrl_handle = 0;
    
    if(0 != win_handle)
    {
        win_handle = MMK_ConvertIdToHandle( win_handle );
        
        ctrl_handle = MMK_ConvertIdToHandle( ctrl_id );

        if (0 != ctrl_id && ctrl_handle == ctrl_id)
        {
            divider_ptr = (CTRLHEADERANDDIVIDER_OBJ_T*)MMK_GetCtrlPtr(ctrl_id);
        }
        else
        {
            divider_ptr = (CTRLHEADERANDDIVIDER_OBJ_T*)MMK_GetCtrlPtrByWin(win_handle, ctrl_id);
        }

        if(PNULL != divider_ptr)
        {
            SCI_ASSERT(DividerTypeOf((CTRLBASE_OBJ_T*)divider_ptr ));/*assert verified*/
        }
    }
    
    return divider_ptr;
}

/*****************************************************************************/
// Description : set the window HEADERANDDIVIDER string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDIVIDER_SetText(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                const wchar*    wstr_ptr, //[IN] string pointer
                                uint16          wstr_len, //[IN] string length, <=16
                                BOOLEAN         is_need_update //[IN] is flash
                                )
{
    return CTRLDIVIDER_SetTextByPtr(GetDividerPtr(win_handle, ctrl_id), wstr_ptr, 
wstr_len, is_need_update);
}

/*****************************************************************************/
// Description : 获取字符串
// Global resource dependence : none
// Author: xiaoqinglu
// Note:
/*****************************************************************************/
LOCAL MMI_STRING_T GetHeaderAndDividerText(
                                CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                                )
{
    MMI_STRING_T    headeranddivider_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return headeranddivider_str;
    }

    if (GUIHEADERANDDIVIDER_DATA_TEXT_ID == headeranddivider_ptr->text_type)
    {
        MMITHEME_GetResText( headeranddivider_ptr->text_id, headeranddivider_ptr->base_ctrl.win_handle, &headeranddivider_str );
    }
    else if (GUIHEADERANDDIVIDER_DATA_TEXT_BUFFER == headeranddivider_ptr->text_type)
    {
        //get string
        headeranddivider_str.wstr_ptr = headeranddivider_ptr->headeranddivider_str.wstr_ptr;
        headeranddivider_str.wstr_len = headeranddivider_ptr->headeranddivider_str.wstr_len;
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

    return headeranddivider_str;
}

/*****************************************************************************/
// Description : display headeranddivider text
// Global resource dependence : none
// Author: Liqing.peng
// Note:
/*****************************************************************************/
LOCAL void DisplayHeaderAndDividerText(
                            CTRLHEADERANDDIVIDER_OBJ_T    *headeranddivider_ptr
                            )
{
    MMI_STRING_T    headeranddivider_str = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != headeranddivider_ptr); /*assert verified*/
    if (PNULL == headeranddivider_ptr)
    {
        return;
    }

    headeranddivider_str = GetHeaderAndDividerText(headeranddivider_ptr);

    //display text
    if ( (PNULL != headeranddivider_str.wstr_ptr) && (0 < headeranddivider_str.wstr_len) )
    {

        // disappeared from left byte by byte
        if( MMITHEME_GetIsScrollAsPixel() )
        {
            DisplayScrollHeaderAndDividerByOffset(headeranddivider_str.wstr_ptr, headeranddivider_str.wstr_len, headeranddivider_ptr->scroll_pixel_offset, headeranddivider_ptr);            
        }
        else
        {
            MMI_STRING_T    str_info = {0};
            GUI_RECT_T      text_rect =  GetTextRect(headeranddivider_ptr);
            GUI_ALIGN_E     align_style = ALIGN_HVMIDDLE;
            GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
            GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;

            align_style = GetFontAlign(headeranddivider_ptr);

            GetHeaderAndDividerTextStyle(headeranddivider_ptr, &str_style);
            if (HEADERANDDIVIDER_SCROLL_LEFT_OUT == headeranddivider_ptr->scroll_type)    
            { 
                if (headeranddivider_ptr->scroll_char_offset < headeranddivider_str.wstr_len)
                {
                    headeranddivider_str.wstr_ptr += headeranddivider_ptr->scroll_char_offset;
                    headeranddivider_str.wstr_len  = (uint16)(headeranddivider_str.wstr_len - headeranddivider_ptr->scroll_char_offset);
                }
                if (headeranddivider_ptr->scroll_char_offset != 0)
                {
                    align_style = ALIGN_LVMIDDLE;
                }
            }
            else if( HEADERANDDIVIDER_NOSCROLL_BY_ELLIPSIS == headeranddivider_ptr->scroll_type )
            {
                str_state |= GUISTR_STATE_ELLIPSIS;
            }

            str_style.align = align_style;
            str_info.wstr_len = headeranddivider_str.wstr_len;
            str_info.wstr_ptr = headeranddivider_str.wstr_ptr;

            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO*)&headeranddivider_ptr->divider_layer_info,
                (const GUI_RECT_T*)&text_rect,       //the fixed display area
                (const GUI_RECT_T*)&text_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T*)&str_info,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO
                );
        }
    }
    else
    {
        //SCI_TRACE_LOW:"DisplayHeaderAndDivider: text str is NULL!"
//         SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIHEADERANDDIVIDER_2592_112_2_18_3_22_25_372,(uint8*)"");
    }
}
#if 0 
/*****************************************************************************/
// Description : headeranddivider set rect
// Global resource dependence : 
// Author: James.Zhang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetHeaderAndDividerContentRect( 
                                    CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr
                                    )
{
    BOOLEAN             result          = FALSE;
    uint16              image_width     = 0;
    uint16              image_height    = 0;
    MMI_TITLE_STYLE_T   headeranddivider_style     = {0};

    // 参数检测
    if (PNULL != headeranddivider_ptr && !GUI_IsInvalidRect(headeranddivider_ptr->base_ctrl.rect))
    {
        MMITHEME_GetHeaderAndDividerStyle(&headeranddivider_style);
        
        GUIRES_GetImgWidthHeight(&image_width, &image_height, headeranddivider_style.left_button_icon_up, headeranddivider_ptr->base_ctrl.win_handle);
        //istyle icon rect
        {
            uint32 i = 0;
            MMI_IMAGE_ID_T istyle_icon_id = 0;
            MMI_STRING_T headeranddivider_str = {0};
            uint16 headeranddivider_str_width[GUIHEADERANDDIVIDER_BUTTON_NUM] = {0};
            uint16 display_width = 0;

                //get icon width/height
                GUIRES_GetImgWidthHeight(&image_width, &image_height, istyle_icon_id, headeranddivider_ptr->base_ctrl.win_handle);

                //get string width
                if ( (PNULL != headeranddivider_str.wstr_ptr) && (0 < headeranddivider_str.wstr_len) )
                {
                    GUISTR_STYLE_T str_style = {0}; /*lint !e64*/
                    GUISTR_STATE_T str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;
                    GUI_ALIGN_E align_style = ALIGN_HVMIDDLE;
                    
                    GetHeaderAndDividerTextStyle(headeranddivider_ptr, &str_style);

                    str_style.align = align_style;
                    str_style.font = headeranddivider_style.button_font_type;

                    headeranddivider_str_width[i] = GUISTR_GetStringWidth(&str_style, &headeranddivider_str, str_state);
                }

                display_width = image_width + headeranddivider_str_width[i];
                
                display_width = MIN(display_width,(headeranddivider_ptr->base_ctrl.rect.right - headeranddivider_ptr->base_ctrl.rect.left + 1)/3);

        result = TRUE;
    }

    return result;
}
#endif 

/*****************************************************************************/
// Description : 动态创建状态栏控件
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CTRLHEADERANDDIVIDER_CreateCtrlDyn(
                                      MMI_HANDLE_T win_handle,
                                      MMI_CTRL_ID_T ctrl_id,
									  MMI_TEXT_ID_T text_id
                                      )
{
    BOOLEAN result = FALSE;
    GUIHEADERANDDIVIDER_INIT_DATA_T   headeranddivider_data   = {0};
    MMI_CONTROL_CREATE_T   create       = {0};
    
    headeranddivider_data.text_id = text_id;
    
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_HEADERANDDIVIDER_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = &headeranddivider_data;
    if (PNULL != MMK_CreateControl( &create ))
    {
        result = TRUE;
//        MMK_SetWinHeaderAndDividerCtrlId( win_handle, create.ctrl_id );
    }

    return result;
}

/*****************************************************************************/
// Description : get headeranddivider control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC CTRLHEADERANDDIVIDER_OBJ_T *CTRLHEADERANDDIVIDER_GetHeaderAndDividerPtr(
                                             MMI_HANDLE_T  win_handle,
                                             MMI_CTRL_ID_T ctrl_id
                                             )
{
    return GetHeaderAndDividerPtr(win_handle, ctrl_id);
}

/*****************************************************************************/
// Description : set the headeranddivider background
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetBackground(
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T  ctrl_id,
                                      MMI_IMAGE_ID_T bg_image,
                                      GUI_COLOR_T    bg_color
                                      )
{
    BOOLEAN           result  = FALSE;
    CTRLHEADERANDDIVIDER_OBJ_T      *headeranddivider_ptr = PNULL;
    
    headeranddivider_ptr = GetHeaderAndDividerPtr(win_handle, ctrl_id);
    
    if(NULL != headeranddivider_ptr)
    {
        headeranddivider_ptr->bg_image = bg_image;
        headeranddivider_ptr->bg_color = bg_color;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : set headeranddivider Theme color
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void CTRLHEADERANDDIVIDER_SetThemeColor(
                                  MMI_HANDLE_T      win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  GUI_COLOR_T       theme_color
                                  )
{
    CTRLHEADERANDDIVIDER_SetThemeColorByPtr(GetHeaderAndDividerPtr(win_handle,ctrl_id), theme_color);
}

/*****************************************************************************/
// Description : set headeranddivider font color
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void CTRLHEADERANDDIVIDER_SetThemeColorByPtr(
                                       CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                                       GUI_COLOR_T       theme_color
                                       )
{
    if(NULL != headeranddivider_ptr && HeaderAndDividerTypeOf((CTRLBASE_OBJ_T*)headeranddivider_ptr ))
    {
        //set font color
        headeranddivider_ptr->str_style.font_color = theme_color;
        headeranddivider_ptr->line_color = theme_color;
    }
}

/*****************************************************************************/
// Description : 设置headeranddivider区域
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetRectByPtr(    
                                     CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                                     GUI_RECT_T        rect
                                     )
{
    BOOLEAN         result = FALSE;
    
    if (PNULL != headeranddivider_ptr 
        && HeaderAndDividerTypeOf((CTRLBASE_OBJ_T*)headeranddivider_ptr ))
    {
        HeaderAndDividerSetRect((CTRLBASE_OBJ_T*)headeranddivider_ptr, &rect);
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : Set HeaderAndDivider Visible
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetIsVisible(    
                                     CTRLHEADERANDDIVIDER_OBJ_T *headeranddivider_ptr,
                                     BOOLEAN is_visible,
                                     BOOLEAN is_update
                                     )
{
    BOOLEAN result = FALSE;

    if (PNULL != headeranddivider_ptr)
    {
        IGUICTRL_SetState((IGUICTRL_T *)headeranddivider_ptr, GUICTRL_STATE_INVISIBLE, (BOOLEAN)(!is_visible));

        if ((is_update) &&
            (MMK_IsFocusWin(headeranddivider_ptr->base_ctrl.win_handle)))
        {
            if (is_visible)
            {
                DisplayHeaderAndDivider(headeranddivider_ptr);
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : update the window headeranddivider
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC void CTRLHEADERANDDIVIDER_Update(
                            MMI_HANDLE_T    win_handle,
                            MMI_CTRL_ID_T    ctrl_id
                            )
{
    CTRLHEADERANDDIVIDER_OBJ_T          *headeranddivider_ptr = PNULL;

    headeranddivider_ptr = GetHeaderAndDividerPtr(win_handle, ctrl_id);
    
    if(PNULL != headeranddivider_ptr)
    {
        DisplayHeaderAndDivider(headeranddivider_ptr);
    }
}

/*****************************************************************************/
// Description : 设置headeranddivider区域
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetRect(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_RECT_T        rect
                                )
{
    BOOLEAN            result = FALSE;
    CTRLHEADERANDDIVIDER_OBJ_T       *headeranddivider_ptr = PNULL;

    headeranddivider_ptr = GetHeaderAndDividerPtr(win_handle, ctrl_id);

    if(PNULL != headeranddivider_ptr)
    {
        result = CTRLHEADERANDDIVIDER_SetRectByPtr( headeranddivider_ptr, rect );
    }
    
    return result;
}
/*****************************************************************************/
// Description : set the window headeranddivider string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextByPtr(
                                     CTRLHEADERANDDIVIDER_OBJ_T  *headeranddivider_ptr,
                                     const wchar*     wstr_ptr, //[IN] string pointer
                                     uint16           wstr_len, //[IN] string length, <=16
                                     BOOLEAN          is_need_update //[IN] is flash
                                     )
{
    BOOLEAN                result  = TRUE;
    
    if(PNULL == headeranddivider_ptr)
    {
        return FALSE;
    }

    StopHeaderAndDividerScrollTimer(headeranddivider_ptr);

    // 申请之前必须释放原来的内存，否则会产生内存泄漏
    ReleaseHeaderAndDividerStr(headeranddivider_ptr);

    if (0 != wstr_len && NULL != wstr_ptr)
    {
        headeranddivider_ptr->headeranddivider_str.wstr_ptr = SCI_ALLOC_APP(((wstr_len + 1)*sizeof(wchar)));
        SCI_MEMSET(headeranddivider_ptr->headeranddivider_str.wstr_ptr, 0, ((wstr_len + 1)*sizeof(wchar)));
        headeranddivider_ptr->headeranddivider_str.wstr_len = wstr_len;
        
        MMI_WSTRNCPY( headeranddivider_ptr->headeranddivider_str.wstr_ptr, headeranddivider_ptr->headeranddivider_str.wstr_len, wstr_ptr, wstr_len, wstr_len );

        GUI_ReplaceChar(&headeranddivider_ptr->headeranddivider_str, LF_CHAR, BL_CHAR);
        GUI_ReplaceChar(&headeranddivider_ptr->headeranddivider_str, CR_CHAR, BL_CHAR);
    }

    headeranddivider_ptr->text_type = GUIHEADERANDDIVIDER_DATA_TEXT_BUFFER;

    // 重新设置headeranddivider的长度和滚动起点
    headeranddivider_ptr->scroll_char_offset = 0;
    headeranddivider_ptr->scroll_pixel_offset = 0;
    headeranddivider_ptr->is_text_overstep = FALSE;
    
    if (is_need_update)
    {
        DisplayHeaderAndDivider(headeranddivider_ptr);
    }
    
    return (result);
}

/*****************************************************************************/
// Description : set the window headeranddivider string, by text id
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextId(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  MMI_TEXT_ID_T        text_id, //[IN] the string id
                                  BOOLEAN            is_need_update //[IN] is flash
                                  )
{
    BOOLEAN                result  = FALSE;
    CTRLHEADERANDDIVIDER_OBJ_T        *headeranddivider_ptr = PNULL;
    
    headeranddivider_ptr = GetHeaderAndDividerPtr(win_handle, ctrl_id);
    
    if(NULL != headeranddivider_ptr)
    {
        StopHeaderAndDividerScrollTimer(headeranddivider_ptr);

        // text type
        headeranddivider_ptr->text_type = GUIHEADERANDDIVIDER_DATA_TEXT_ID;

        //  text id
        headeranddivider_ptr->text_id = text_id;

        // 重新设置headeranddivider的长度和滚动起点
        headeranddivider_ptr->scroll_char_offset = 0;
        headeranddivider_ptr->scroll_pixel_offset = 0;
        headeranddivider_ptr->is_text_overstep = FALSE;
        
        result = TRUE;

        if (is_need_update)
        {
            DisplayHeaderAndDivider(headeranddivider_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
// Description : set the window headeranddivider align type
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextAlignType(
                                         MMI_HANDLE_T                win_handle,
                                         MMI_CTRL_ID_T            ctrl_id,
                                         GUI_ALIGN_E                align
                                         )
{
    BOOLEAN           result  = FALSE;
    CTRLHEADERANDDIVIDER_OBJ_T          *headeranddivider_ptr = PNULL;
    
    headeranddivider_ptr = GetHeaderAndDividerPtr(win_handle, ctrl_id);
    if(PNULL != headeranddivider_ptr)
    {
        headeranddivider_ptr->str_style.align = align;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : set the window headeranddivider scroll type
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetTextScrollType(
                                          MMI_HANDLE_T                win_handle,
                                          MMI_CTRL_ID_T            ctrl_id,
                                          GUIHEADERANDDIVIDER_SCROLL_TYPE_E    scroll_type
                                          )
{
    BOOLEAN                result  = FALSE;
    CTRLHEADERANDDIVIDER_OBJ_T        *headeranddivider_ptr = PNULL;
    
    headeranddivider_ptr = GetHeaderAndDividerPtr(win_handle, ctrl_id);
    
    // check the param
    if ( (HEADERANDDIVIDER_SCROLL_LEFT_OUT != scroll_type)
        && (HEADERANDDIVIDER_NOSCROLL_BY_ELLIPSIS != scroll_type))
    {
        //SCI_TRACE_LOW:"GUIHEADERANDDIVIDER_SetTextScrollType: the type is error!"
 //       SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIHEADERANDDIVIDER_1041_112_2_18_3_22_22_369,(uint8*)"");
        return (result);
    }

    if(PNULL != headeranddivider_ptr)
    {
        
        headeranddivider_ptr->scroll_type = scroll_type;
        result = TRUE;
    }
    
    return (result);
}
/*****************************************************************************/
// Description : set the window headeranddivider string
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLHEADERANDDIVIDER_SetText(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                const wchar*    wstr_ptr, //[IN] string pointer
                                uint16          wstr_len, //[IN] string length, <=16
                                BOOLEAN         is_need_update //[IN] is flash
                                )
{
    return CTRLHEADERANDDIVIDER_SetTextByPtr(GetHeaderAndDividerPtr(win_handle, ctrl_id), wstr_ptr, 
wstr_len, is_need_update);
}


