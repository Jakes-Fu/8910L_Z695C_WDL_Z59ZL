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
    MUINT nIntervel;			//ִ������
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
//Descrption:ƽ̨timer��ʼ��
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTimer_Initialize(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfTimer_DeInitialize
//Descrption:ƽ̨timer����ʼ��
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTimer_DeInitialize(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfTm_StartTimer
//Descrption:�ļ�ϵͳ��ʼ��
//Param:
//  IN: pUser:������ָ�룬�ص�ʱ����
//  interval:��ʱ���������Ϊ��λ
//  expireFunc:��ʱ�ص�����
//  OUT:McfTmHandle
//Return: ����timer id
//			>=0  -- timer handle
//			<0  -- error	
//Note:������ʱ������ʱһ�� 
//----------------------------------------------------------------------------
MTmHandle McfTm_StartTimer(MVOID *pUser,MUINT nInterval,pfnTimerFuncPtr expireFunc);

//---------------------------------------------------------------------------
//FUNCTION: McfTm_StopTimer
//Descrption:ֹͣƽ̨timer
//Param:
//  IN: timerHd:timer���
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTm_StopTimer(MTmHandle timerHd);

//---------------------------------------------------------------------------
//FUNCTION: McfTm_StopAllTimer
//Descrption:ֹͣƽ̨���е�timer
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTm_StopAllTimer(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfTimer_HandleTimer
//Descrption:timer �ص�
//Param:
//  IN:timer_id:timer id 
//  param:��������
//  OUT:MBOOL
//Return: MFALSE: ƽ̨����
//		MTRUE:ƽ̨������
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

