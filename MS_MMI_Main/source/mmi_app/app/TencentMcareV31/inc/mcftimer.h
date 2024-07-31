/******************************************************************************/
//File Name:        McfTimer.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide simple timer interface for mcare
//Modifed:          nicklei 26/09/2011
//Modifed:          wilsonzhu 01/10/2011
/******************************************************************************/
#ifndef _MCFTIMER_H_
#define _MCFTIMER_H_
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/ 
#include "mcfcommon.h"

 /**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                    *
 **---------------------------------------------------------------------------*/
typedef struct _McfTimer 
{
    MVOID* pUser;
    pfnTimerFuncPtr callbcak;
    MUINT nIntervel;			//执行周期
    MUINT m_PassedTime;
    MTmHandle Mcare_timerID;
	struct _McfTimer* m_pNext;
    //MBYTE Mocor_timerID;
    MBOOL m_IsUsed;
}McfTimer;


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
 
//---------------------------------------------------------------------------
//FUNCTION: McfTimer_Initialize
//Descrption:平台timer初始化
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTimer_Initialize(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfTimer_DeInitialize
//Descrption:平台timer反初始化
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTimer_DeInitialize(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfTm_StartTimer
//Descrption:文件系统初始化
//Param:
//  IN: pUser:调用者指针，回调时传回
//  interval:超时间隔，毫秒为单位
//  expireFunc:超时回调函数
//  OUT:McfTmHandle
//Return: 返回timer id
//			>=0  -- timer handle
//			<0  -- error	
//Note:启动定时器，定时一次 
//----------------------------------------------------------------------------
MTmHandle McfTm_StartTimer(MVOID *pUser,MUINT nInterval,pfnTimerFuncPtr expireFunc);

//---------------------------------------------------------------------------
//FUNCTION: McfTm_StopTimer
//Descrption:停止平台timer
//Param:
//  IN: timerHd:timer句柄
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTm_StopTimer(MTmHandle timerHd);

//---------------------------------------------------------------------------
//FUNCTION: McfTm_StopAllTimer
//Descrption:停止平台所有的timer
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTm_StopAllTimer(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfTimer_HandleTimer
//Descrption:timer 回调
//Param:
//  IN:timer_id:timer id 
//  param:附带数据
//  OUT:MBOOL
//Return: MFALSE: 平台处理
//		MTRUE:平台不处理
//Note:
//----------------------------------------------------------------------------
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MVOID McfTimer_HandleTimer(MBYTE timer_id, MUINT param);
#else
MBOOL McfTimer_HandleTimer(MBYTE timer_id, MUINT param);
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif//_MCFTIMER_H_

