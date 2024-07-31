/********************************************************************************************
** File Name:    mmigame_common.h                                                        *
** Author:         michael.wang                                                                     *
** Date:            6/4/2012                                                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is the common part of shooting game framwork *
**********************************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME             DESCRIPTION                               *
** 4/6/2012        michael.wang         Create
******************************************************************************/

#ifndef _MMIGAME_COMMON_H_
#ifdef GAME_GOLDMINER_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "std_header.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmigame_quintet_menutable.h"
#include "mmigame_menutable.h"
#include "mmigame_quintet_id.h"  
#include "tb_dal.h"

#include "mmipub.h"
#include "mmigame_quintet_nv.h"
#include "mmialarm_export.h"
#include "mmigame_internal.h"
#include "mmi_textfun.h"
#include "mmiphone_export.h"
//#include "mmigame_quintet_internal.h"
//#include "mmigame_quintet_text.h"
#include "mmigame_id.h"
#include "mmigame_goldminer_text.h"
#include "mmigame_goldminer_control.h"
#include "mmigame_goldminer_mode.h"
#include "mmigame_goldminer_view.h"

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN is_tool_in_use = FALSE;   //游戏中道具使用状态
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 游戏time out 弹出提示框的回调函数
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerLevelFailWaitWinMsg(
                                                        MMI_WIN_ID_T        win_id,   //in
                                                        MMI_MESSAGE_ID_E    msg_id,  //in  
                                                        DPARAM              param    //in
                                                        )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch( msg_id)
    {    
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMIGOLDMINER_GoToNextLevelM(FALSE);
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_APP_CANCEL:
        {
            MMK_CloseWin(MMK_GetPrevWinId(win_id));
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_APP_RED:
        break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 游戏Level Done 的提示框回调函数
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerLevelFinishWaitWinMsg(
                                       MMI_WIN_ID_T        win_id,   //in
                                       MMI_MESSAGE_ID_E    msg_id,  //in  
                                       DPARAM              param    //in
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch( msg_id)
    {    
    case MSG_APP_OK:
    case MSG_APP_WEB:
            {
                 MMIGOLDMINER_GoToNextLevelM(TRUE);
                 MMK_CloseWin(win_id);
            }
    break;
    case MSG_APP_CANCEL:
            {
                MMK_CloseWin(MMK_GetPrevWinId(win_id));
                MMK_CloseWin(win_id);
            }
            break;
    case MSG_APP_RED:
        break;
    default:
       result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 游戏Level Done 的提示框回调函数
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGoldMinerGameFinishWaitWinMsg(
                                       MMI_WIN_ID_T        win_id,   //in
                                       MMI_MESSAGE_ID_E    msg_id,  //in  
                                       DPARAM              param    //in
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch( msg_id)
    {    
    case MSG_APP_OK:
    case MSG_APP_WEB:
            {                
                 MMIGOLDMINER_ReplayWithGameFinishM();
                 MMK_CloseWin(win_id);
            }
    break;
    case MSG_APP_CANCEL:
            {
                MMK_CloseWin(MMK_GetPrevWinId(win_id));
                MMK_CloseWin(win_id);
            }
            break;
    case MSG_APP_RED:
        break;
    default:
       result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : start gold miner
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_StartGameC(void) 
{
    MMIGOLDMINER_SetGameStateM(MMI_GAME_START);
    MMIGOLDMINER_GameStateMachineC(GAME_EVENT_NONE);
    MMIGOLDMINER_ClipperStateMachineC(EVENT_NONE);
}

/*****************************************************************************/
//  Description : exit gold miner
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_ExitGameC(void) 
{
    MMIGOLDMINER_ExitGameM();
}

/*****************************************************************************/
//  Description : game state machine
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_GameStateMachineC(MMI_GAME_EVENT_E game_event)  //参数
{
    //游戏状态：start-->pause--catch-->上升--got-->卸载-->摇摆；
    MMI_GAME_STATE_E game_state =  MMIGOLDMINER_GetGameStateM();

    switch(game_state)
    {
    case MMI_GAME_START:    //初始状态
        MMIGOLDMINER_InitGameM();
        //启动timer
        MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_COUNTED);
        MMIGOLDMINER_SetGameStateM(MMI_GAME_PLAY);            
        break;
    case MMI_GAME_PLAY:     //游戏进行中
        {
            if(GAME_EVENT_LOSE_FOCUS == game_event)  //LOSE_FOCUS
            {
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_COUNTED);
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_SWING);
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_STRETCH);
                MMIGOLDMINER_SetGameStateM(MMI_GAME_PAUSE);//暂停
            }
            else if(GAME_EVENT_TIME_OUT == game_event)
            {
                MMI_STRING_T        wait_text = {0};
                
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_COUNTED);
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_SWING);
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_STRETCH);
                MMIGOLDMINER_SetGameStateM(MMI_GAME_OVER);//关卡失败   
                //pop up 过关失败提示框，Modify by Michael.Wang
                MMI_GetLabelTextByLang(TXT_GOLDMINER_GAME_OVER, &wait_text);
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,NULL,IMAGE_PUBWIN_SUCCESS,
                            NULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_OKCANCEL,HandleGoldMinerLevelFailWaitWinMsg);  
                //Modify over
            }
            else if(MMIGOLDMINER_CheckLevelFinishEnableM())
            {
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_COUNTED);
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_SWING);
                MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_STRETCH);
                MMIGOLDMINER_GameDrawV();
                MMIGOLDMINER_UpdateRecordM();
                
                if(!MMIGOLDMINER_IsGameFinishM())
                {
                    MMI_STRING_T  wait_text = {0};
                    MMIGOLDMINER_SetGameStateM(MMI_GAME_LEVEL_DONE);//过关               
                    MMI_GetLabelTextByLang(TXT_GOLDMINER_LEVEL_DONE, &wait_text);
                    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,NULL,IMAGE_PUBWIN_SUCCESS,
                        NULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_OKCANCEL,HandleGoldMinerLevelFinishWaitWinMsg); 
                }
                else
                {
                    MMI_STRING_T  wait_text = {0};
                    MMIGOLDMINER_SetGameStateM(MMI_GAME_FINISH);//过关  
                    MMI_GetLabelTextByLang(TXT_GOLDMINER_GAME_FINISH, &wait_text);
                    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,NULL,IMAGE_PUBWIN_SUCCESS,
                        NULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_OKCANCEL,HandleGoldMinerGameFinishWaitWinMsg); 
                }     
            }
        }
        break;
        
    case MMI_GAME_PAUSE:    //暂停
        {
            if(GAME_EVENT_GET_FOCUS == game_event)
            {
                MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_COUNTED);
                MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_SWING);
                MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_STRETCH);
                MMIGOLDMINER_SetGameStateM(MMI_GAME_PLAY);
            }
        }
        break;
    //过关成功和失败，在此处处理是一样的，不一样的逻辑在pubwin的时候已经处理过,Modify by Michael.Wang
    case MMI_GAME_LEVEL_DONE:   //过关
    case MMI_GAME_OVER:         //关卡失败
        {
            MMIGOLDMINER_SetGameStateM(MMI_GAME_PLAY);
            MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_NORMAL);//重置
            MMIGOLDMINER_ClipperStateMachineC(EVENT_NONE);     //重置
            MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_COUNTED);
        }
        break;
        //Modify over

    case MMI_GAME_FINISH:       //所有关通过
        {
            //退出游戏，回到游戏菜单
            MMIGOLDMINER_SetGameStateM(MMI_GAME_PLAY);
            MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_NORMAL);//重置
            MMIGOLDMINER_ClipperStateMachineC(EVENT_NONE);     //重置
            MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_COUNTED);
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : clippers state machine
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_ClipperStateMachineC(MMI_ENTITY_EVENT_E event_type)  //参数为事件
{
    //钳子具有三种状态：摇摆--press-->下降--catch-->上升--got-->卸载-->摇摆；
    //MMI_GAME_ENTITY_READY->MMI_GAME_ENTITY_MOTION->MMI_GAME_ENTITY_HIT->MMI_GAME_ENTITY_END-->MMI_GAME_ENTITY_READY
    MMI_GAME_ENTITY_STATE_E clipper_state =  MMIGOLDMINER_GetClipperStateM();

    switch(clipper_state)
    {
    case MMI_GAME_ENTITY_NORMAL:    //初始状态
        MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_SWING);
        MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_READY);            
        break;
    case MMI_GAME_ENTITY_READY:     //摇摆状态
        {
            if(EVENT_START != event_type)  //无启动事件,则摇摆
            {
                MMIGOLDMINER_SwingM();//定时更新angle\object_pos,长度固定
            }
            else
            {
                MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_STRETCH);
                MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_MOTION);                   
            }
        }
        break;
        
    case MMI_GAME_ENTITY_MOTION:    //下降
        {
            if(EVENT_OUTRANGE == event_type)
            {
                MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_NOCATCH);  
            }
            else if(EVENT_HITOBJECT == event_type)  //无启动事件,则摇摆
            {
                MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_HIT);    
            }
            else
            {
                MMIGOLDMINER_ClippersMoveM(TRUE, FALSE);//(DOWN, NO_CATCH); 
            } 
        }
        break;
        
    case MMI_GAME_ENTITY_HIT:       //携带物体上升
        {
            if(EVENT_REATCHTOP == event_type)     //到达顶部
            {
                //加分处理
                MMIGOLDMINER_UpdateCurScoreAndToolsM();
                MMIGOLDMINER_SetMoveCellNoneM();
                //过关判断
                
                MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_SWING);
                MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_READY);

                if(MMIGOLDMINER_GetClippersSpecMoveM())
                {
                    MMIGOLDMINER_SetToolStateC(FALSE);
                    MMIGOLDMINER_SetClippersMoveWithEnergyM(FALSE);
                }
            }
            else
            {
                MMIGOLDMINER_ClippersMoveM(FALSE, TRUE);//(UP, HAVE_CATCH); 
            } 
        }
        break;
    case MMI_GAME_ENTITY_NOCATCH:    //空载
        {
            if(EVENT_REATCHTOP == event_type)      //到达顶部
            {
                MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_SWING);
                MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_READY); 
            }
            else
            {
                MMIGOLDMINER_ClippersMoveM(FALSE, FALSE);//(UP, NO_CATCH);
            } 
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : 获取道具使用状态
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGOLDMINER_GetToolStateC()
{
    return is_tool_in_use;
}

/*****************************************************************************/
//  Description : 设置道具使用状态
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 以防两个道具同时使用
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SetToolStateC(BOOLEAN is_tool_use)
{
    is_tool_in_use = is_tool_use;   
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif //GAME_GOLDMINER_SUPPORT
