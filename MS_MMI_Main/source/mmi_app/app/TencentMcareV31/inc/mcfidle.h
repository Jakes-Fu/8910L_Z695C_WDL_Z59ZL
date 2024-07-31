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
//Descrption:����StatusBar��ͼ��
//Param:
//  IN: iconId:MIDLE_ICON_MCARE��
//      flag:MIDLE_ICON_SHOW��
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_SetStatusIcon(MSHORT iconId, MINT flag);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_UpdateStatusIcons
//Descrption:����StatusBar��ͼ�ꡣ�軭
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_UpdateStatusIcons(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_RegIconPenEventHdlr
//Descrption:ע��StatusBar��ͼ��Ļص�����
//Param:
//  IN: iconId:ͼ��
//  evtType:�¼�����
//  func:�ص�����
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_RegIconPenEventHdlr(pfnIconFunc func);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_HandlePenIconFunc
//Descrption:Icon�Ĵ�����Ӧ����
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_HandlePenIconFunc(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_RunPenIconFunc
//Descrption:Icon�Ĵ�����Ӧ����
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MINT McfIdle_RunPenIconFunc(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_IsStatusIconBlink
//Descrption:Icon�Ƿ���˸
//Param:
//  IN: icon
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MBOOL McfIdle_IsStatusIconBlink(MSHORT nIconId);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_InitIdleIcon
//Descrption:��ʼ��StatusBar��ID
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfIdle_InitIdleIcon(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfIdle_DeinitIdle
//Descrption:Idle���ݵķ���ʼ��
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MBOOL McfIdle_DeinitIdle(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfCmn_GetIdleIconImageID
//Descrption:�����Ӧ��״̬�µ�ͼ��
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MUINT McfCmn_GetIdleIconImageID(MVOID);
//---------------------------------------------------------------------------
//FUNCTION: McfIdle_IdleSatuBarInit
//Descrption:IDLE״̬ͼ��ĳ�ʼ��
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
