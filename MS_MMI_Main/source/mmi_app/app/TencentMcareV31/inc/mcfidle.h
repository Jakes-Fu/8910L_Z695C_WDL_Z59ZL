/******************************************************************************/
//File Name:        McfIdle.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide simple ilde interface for mcare
//Modifed:          wilsonzhu 01/10/2011
/******************************************************************************/
#ifndef _MCFIDLE_H_
#define _MCFIDLE_H_

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif
/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mcfcommon.h"

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_SetStatusIcon
//Descrption:设置StatusBar的图标
//Param:
//  IN: iconId:MIDLE_ICON_MCARE等
//      flag:MIDLE_ICON_SHOW等
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_SetStatusIcon(MSHORT iconId, MINT flag);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_UpdateStatusIcons
//Descrption:更新StatusBar的图标。描画
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_UpdateStatusIcons(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_RegIconPenEventHdlr
//Descrption:注册StatusBar的图标的回调函数
//Param:
//  IN: iconId:图标
//  evtType:事件类型
//  func:回调函数
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_RegIconPenEventHdlr(pfnIconFunc func);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_HandlePenIconFunc
//Descrption:Icon的触屏相应函数
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_HandlePenIconFunc(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_RunPenIconFunc
//Descrption:Icon的触屏相应函数
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MINT McfIdle_RunPenIconFunc(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_IsStatusIconBlink
//Descrption:Icon是否闪烁
//Param:
//  IN: icon
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MBOOL McfIdle_IsStatusIconBlink(MSHORT nIconId);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_InitIdleIcon
//Descrption:初始化StatusBar的ID
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_InitIdleIcon(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_DeinitIdle
//Descrption:Idle数据的反初始化
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MBOOL McfIdle_DeinitIdle(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfCmn_GetIdleIconImageID
//Descrption:获得相应的状态下的图标
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MUINT McfCmn_GetIdleIconImageID(MVOID);
//---------------------------------------------------------------------------
//FUNCTION: McfIdle_IdleSatuBarInit
//Descrption:IDLE状态图标的初始化
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_IdleSatuBarInit(MVOID);

MBOOL McfIdle_IsStatusIconShow(MSHORT nIconId);

MVOID McfIdle_HandleIconFunc_QQ(MVOID);

MBOOL McfIdle_IsQQBg(MVOID);

#endif//_MCFIDLE_H_
