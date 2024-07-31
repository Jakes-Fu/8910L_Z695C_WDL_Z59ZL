/****************************************************************************
** File Name:      mmiqq_main.c(qq2009.c)                                                *
** Author:                                                                 *
** Date:           05/11/2010                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the system  of QQ             *
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
#include "sci_types.h"
#include "window_parse.h"
#include "datatype.h"
#include "qq2009.h"
#include "mqq.h"
#include "mmk_kbd.h"
#include "mmi_position.h"
#include "qsystem.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "qq_text.h"
//#include "qqmemmanager.h"

//#include "mmiphone_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//QU8 GBUF[200*1024];

//MMI_APPLICATION_T	    g_qq_app = {0};							//g_qq_app的实体定义

#define  MMIQQ_TIMER        100//s_qq_timer_id定时器的时间

#define MMIQQ_SPACE_MIN_SIZE    (100*1024)//100k: 提示QQ的最小存储空间

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern void Timer_Notify(void);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
//第三方直接调用,不可修改名字
int SCR_W = 0;//MMI_MAINSCREEN_WIDTH;
int SCR_H = 0;//MMI_MAINSCREEN_HEIGHT;
void*  pMemMgr = PNULL;//供QQ 使用的memory

 //管理QQ的指针
LOCAL Mqq* s_qq_manage_ptr = PNULL;
 
//QQ运行时,为管理QQ启动的一个周期性定时器
LOCAL uint8            s_qq_timer_id = 0;

//记录MSG_KEYDOWN_WEB
LOCAL BOOLEAN      s_is_web_key_pressed = FALSE;
//MN_DUAL_SYS_E	g_qq_sim_select = MN_DUAL_SYS_1;

//QQ是否需后台运行
LOCAL BOOLEAN s_is_allow_run_bg = FALSE;
//QQ是否处于运行中
LOCAL BOOLEAN  s_is_qq_active = FALSE;

//QQ后台运行前的win id (目前暂未使用)
//static MMIQQ_WINDOW_ID_E s_lv_win_id = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: HandleQQMainWinMsg
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQQMainWinMsg(MMIQQ_WINDOW_ID_E , MMI_MESSAGE_ID_E , DPARAM );

/*****************************************************************************/
//  Description: 打开QQ
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void EntryQQ(void);

/*****************************************************************************/
//  Description :QQ所使用的存储设备空间是否不足
//  Global resource dependence : none
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN QQIsHaveEnoughSpace(int8 *device_type_ptr);

WINDOW_TABLE(MMIQQ_MAIN_WIN_TAB) = 
{
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_ALL),
    WIN_FUNC((uint32)HandleQQMainWinMsg),    
    WIN_ID(MMIQQ_WIN_ID_QQMAIN),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#if 0
/*****************************************************************************/
//  Description: IS_QQRUN
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN IS_QQRUN(void)

{
 // return (MMK_GetFocusWinId() == MMIQQ_WIN_ID_QQMAIN);
  return(MMK_IsOpenWin(MMK_ConvertIdToHandle(MMIQQ_WIN_ID_QQMAIN)));
}

/*****************************************************************************/
//  Description: Prewin_Winrunbg
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void Prewin_Winrunbg(void)
{
    // return (MMK_GetFocusWinId() == MMIQQ_WIN_ID_QQMAIN);
    s_is_allow_run_bg = 1;
}

/*****************************************************************************/
//  Description: IS_timer_id
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 IS_timer_id(void)
{
    // return (MMK_GetFocusWinId() == MMIQQ_WIN_ID_QQMAIN);
    return 0;//s_qq_timer_id;
}
#endif

//#endif

/*****************************************************************************/
//  Description: HandleQQMainWinMsg
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQQMainWinMsg(MMIQQ_WINDOW_ID_E win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    LOCAL BOOLEAN  s_is_full_paint_for_open = FALSE;
    
    //SCI_TRACE_LOW:"[MMIQQ]HandleQQMainWinMsg() msg_id == 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_MAIN_151_112_2_18_2_49_7_10,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_is_full_paint_for_open = TRUE;
        if(s_qq_manage_ptr != PNULL)
        {
            Mqq_Resume(s_qq_manage_ptr);
            break;
        }
        else
        {
            s_qq_manage_ptr= Mqq_Create();
            Mqq_Start(s_qq_manage_ptr);
        }
        s_is_web_key_pressed = FALSE;//init
        break;

    case MSG_FULL_PAINT:
        if(s_qq_manage_ptr != PNULL)
        {
            Mqq_Resume(s_qq_manage_ptr);
        }
        
        if(s_qq_manage_ptr != PNULL)
        {
            Mqq_Redraw(s_qq_manage_ptr);
        }

        if (s_is_full_paint_for_open)
        {
            MMK_PostMsg(win_id, MSG_QQ_ACTIVE_GPRS, PNULL, 0);
            s_is_full_paint_for_open = FALSE;
        }
        //SCI_TRACE_LOW:"[MMIQQ]HandleQQMainWinMsg() MSG_FULL_PAINT--s_is_full_paint_for_open == %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_MAIN_186_112_2_18_2_49_8_11,(uint8*)"d", s_is_full_paint_for_open);
        break;
        
    case MSG_TIMER:
        //	Mqq_TimerNotify(s_qq_manage_ptr);
        break;
        
    case MSG_CLOSE_WINDOW:
        if(s_is_allow_run_bg)/*QQ需后台运行*/
        {
            if(s_qq_manage_ptr != PNULL)
            {
                Mqq_Suspend(s_qq_manage_ptr);
            }
            GUIWIN_UpdateStb();
            break;
        }
        else
        {
        
        }
        break;
        
    case MSG_LOSE_FOCUS:
        if(s_qq_manage_ptr != PNULL)
        {
            Mqq_Suspend(s_qq_manage_ptr);
        }
        break;

    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYREPEAT_UP:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_UP, EVT_KEY);
        break;

    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_DOWN:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_DOWN, EVT_KEY);
        break;

    case MSG_KEYDOWN_LEFT:
    case MSG_KEYREPEAT_LEFT:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_LEFT, 0);
        break;

    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_RIGHT, 0);
        break;
        
    case MSG_KEYDOWN_1:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_1, 0);
        break;
        
    case MSG_KEYDOWN_2:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_2, 0);
        break;
        
    case MSG_KEYDOWN_3:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_3, 0);
        break;
        
    case MSG_KEYDOWN_4:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_4, 0);
        break;
        
    case MSG_KEYDOWN_5:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_5, 0);
        break;
        
    case MSG_KEYDOWN_6:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_6, 0);
        break;
        
    case MSG_KEYDOWN_7:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_7, 0);
        break;
        
    case MSG_KEYDOWN_8:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_8, 0);
        break;
        
    case MSG_KEYDOWN_9:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_9, 0);
        break;
        
    case MSG_KEYDOWN_0:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_0, 0);
        break;
        
    case MSG_KEYDOWN_OK:
        if(!s_is_web_key_pressed)
        {
            Mqq_HandleKey(s_qq_manage_ptr, AVK_SOFT1, 0);
        }
        break;
        
    case MSG_KEYDOWN_HASH :
        Mqq_HandleKey(s_qq_manage_ptr, AVK_POUND, 0);
        break;
        
    case MSG_KEYDOWN_STAR:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_STAR, 0);
        break;
        
    case MSG_KEYDOWN_CANCEL:
        Mqq_HandleKey(s_qq_manage_ptr, AVK_SOFT2, 0);
        break;
        
   case MSG_KEYDOWN_WEB:
        s_is_web_key_pressed = TRUE;
        break;
        
   case MSG_KEYUP_WEB:
        if(s_is_web_key_pressed)
        {
            s_is_web_key_pressed = FALSE;
            Mqq_HandleKey(s_qq_manage_ptr, AVK_SELECT, 0);
        }
        break;
 
    case MSG_KEYLONG_WEB:
    case MSG_KEYPRESSUP_WEB:
    case MSG_KEYREPEAT_WEB:
        s_is_web_key_pressed = FALSE;
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        Mqq_HandlePen(s_qq_manage_ptr, EVT_PEN_DOWN, MMK_GET_TP_X(param), MMK_GET_TP_Y(param));
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        Mqq_HandlePen(s_qq_manage_ptr, EVT_PEN_MOVE, MMK_GET_TP_X(param), MMK_GET_TP_Y(param));
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        Mqq_HandlePen(s_qq_manage_ptr, EVT_PEN_UP, MMK_GET_TP_X(param), MMK_GET_TP_Y(param));
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_QQ_ACTIVE_GPRS:
#ifndef WIN32
    QSOCKET_ActiveGprs(PNULL);//激活GPRS
#endif
         break;
        
    default :
        result = MMI_RESULT_FALSE;
        break;
	}
	
    return result;
}
/*****************************************************************************/
// 	Description : 提示窗口消息处理
//	Global resource dependence : 
//  Author:fen.xie
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQQAlertPromptWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CLOSE_WINDOW:
        EntryQQ();
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description: 打开QQ
//  Global resource dependence :  
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQQ_EntryQQ(void)
{
    int8    device_type = 0;
    MMI_TEXT_ID_T   alert_text_id = TXT_NULL;
    
    if (QQIsHaveEnoughSpace(&device_type))
    {
        EntryQQ();
    }
    else
    {
        alert_text_id = TXT_QQ_NO_SPACE;
        MMIPUB_OpenAlertWinByTextId(PNULL,alert_text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleQQAlertPromptWindow);
    }
}

/*****************************************************************************/
//  Description: 释放QQ(第三方)相关的memory
//  Global resource dependence :  
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQQ_ReleaseMemory(void)
{
    if(PNULL != pMemMgr) 
    {
        QQMEMMGR_Release(pMemMgr);
        pMemMgr = PNULL;
    }
}
/*****************************************************************************/
//  Description: 申请QQ(第三方)相关的memory
//  Global resource dependence :  
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQQ_CreateMemory(void)
{
    if(PNULL == pMemMgr) 
    {
        pMemMgr = (void*)QQMEMMGR_Create();
        if(PNULL == pMemMgr) 
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description: 处理QQ 的timer id
//  Global resource dependence :  
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIQQ_HandleTimer(uint8 timer_id, uint32 param)
{
    if(timer_id == s_qq_timer_id)
    {
        Timer_Notify();//QQ 第三方处理timer
    }
}

/*****************************************************************************/
//  Description: 打开QQ
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
LOCAL void EntryQQ(void)
{
    BOOLEAN mem_result = MMIQQ_CreateMemory();/*开启QQ需要的memory*/
    WINDOW_SUPPORT_ANGLE    support_angle = WIN_SUPPORT_ANGLE_0;

    //SCI_TRACE_LOW:"[QQ] EntryQQ is_use_wifi = %d, dual_sys =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_MAIN_435_112_2_18_2_49_8_12,(uint8*)"dd",MMIQQ_GetIsUseWifi(), MMIQQ_GetSimSelect());
  
    //为窗口启动一周期性定时器
    if(0 == s_qq_timer_id)
    {
        s_qq_timer_id  = MMK_CreateTimerCallback( MMIQQ_TIMER,  MMIAPIQQ_HandleTimer, NULL, TRUE);
    }

    if((0 == s_qq_timer_id) || !mem_result)/*发生error，关闭QQ*/
    {
        ExitQQ();
        return;
    }

    MMK_CreateWin((uint32*)MMIQQ_MAIN_WIN_TAB,  PNULL);
    
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
    MMK_SetWinSupportAngle(MMIQQ_WIN_ID_QQMAIN, support_angle);
    MMK_SetWinAngle(MMIQQ_WIN_ID_QQMAIN, MMIQQ_GetLogicAngle(), FALSE);

    //important:QQ lcd size
    SCR_W = QGetSCR_W();//MMI_MAINSCREEN_WIDTH;
    SCR_H = QGetSCR_H();//MMI_MAINSCREEN_HEIGHT;    
	
    s_is_qq_active = TRUE;
    s_is_allow_run_bg = TRUE; /*允许QQ后台运行*/
}

//*****************************************************************************

/*****************************************************************************/
//  Description: 关闭QQ
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void ExitQQ( void )
{	
    if(s_qq_manage_ptr != PNULL)
    {
        Mqq_Free(s_qq_manage_ptr);
        s_qq_manage_ptr = PNULL;
    }

    MMIQQ_ReleaseMemory();
    
#ifndef WIN32
    QSOCKET_DeActiveGprs();
#endif

    s_is_qq_active = FALSE;   // qq unactive
    MMIQQ_SetLoginSelect(FALSE); //未选卡
    //	lv_qq_state = BUDDY_STAT_MAX; //状态需要设置
    if(s_qq_timer_id > 0)
    {
        MMK_StopTimer(s_qq_timer_id);
        s_qq_timer_id = 0;
    }

    s_is_allow_run_bg = FALSE;/*QQ已关闭，无需后台运行*/
    if (MMK_IsOpenWin(MMIQQ_WIN_ID_QQMAIN))
    {
        MMK_CloseWin(MMIQQ_WIN_ID_QQMAIN);
    }
    
}

/*****************************************************************************/
//  Description: QGetMqq
//  Global resource dependence :  s_qq_manage_ptr
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void* QGetMqq(void)
{
    return s_qq_manage_ptr;
}

/*****************************************************************************/
//  Description: QGetSCR_W
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int QGetSCR_W(void)
{
    uint16      lcd_width = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,PNULL);
    
    return (int)(lcd_width);
}

/*****************************************************************************/
//  Description: QGetSCR_H
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int QGetSCR_H(void)
{
    uint16      lcd_height = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,PNULL,&lcd_height);
   return (int)(lcd_height);
}

/*****************************************************************************/
//  Description: QQ_IsRunning
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN QQ_IsRunning(void)
{
    return s_is_qq_active;
}

/*****************************************************************************/
//  Description :QQ所使用的存储设备空间是否不足
//  Global resource dependence : none
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN QQIsHaveEnoughSpace(int8 *device_type_ptr)
{
    wchar*	device_name_ptr = PNULL;
    uint16		device_name_len = 0;
    uint32 free_high_word = 0;
    uint32 free_low_word = 0;

    device_name_ptr = MMIQQ_GetDeviceName(&device_name_len);


    MMIAPIFMM_GetDeviceFreeSpace(
                (uint16 *)device_name_ptr,
                device_name_len,
                &free_high_word,
                &free_low_word
                );
    //SCI_TRACE_LOW:"[QQ]:QQIsHaveEnoughSpace free_low_word=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_MAIN_583_112_2_18_2_49_8_13,(uint8*)"d",free_low_word);
    
    if(0 ==free_high_word && free_low_word < MMIQQ_SPACE_MIN_SIZE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

#endif


/*Edit by script, ignore 3 case. Thu Apr 26 19:01:04 2012*/ //IGNORE9527
