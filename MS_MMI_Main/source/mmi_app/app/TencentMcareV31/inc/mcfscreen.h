/******************************************************************************/
//File Name:        McfScreen.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Modifed:          nicklei 09/21/2011
//Modifed:          wiltan 10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide screen for mcare
/******************************************************************************/
#ifndef _MCFSCREEN_H_
#define _MCFSCREEN_H_

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                        Include Files                                       */
/*----------------------------------------------------------------------------*/
#include "mcfcommon.h"
#include "mcfsys.h"
/*----------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                  */
/*----------------------------------------------------------------------------*/
//typedef MVOID (*pfnFunc)(MVOID);
//typedef MBYTE (*pfnDelFunc)(MVOID*);

typedef struct _McfScreenDelFun
{
    MVOID* m_pUser;
    pfnDelFunc m_DelCB;
}McfScreenDelFun;

typedef struct _McfScreenFun
{
    pfnFunc m_CB;
}McfScreenFun;

/*----------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                   */
/*----------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
//	FUNCTION: McfScrn_EntryNewScreen
//	Descrption:�����´���
//	Param:
//	   	    IN  exitFunc:�˳����� 
//	   	    IN  entryFunc:���뺯��
//	   	    IN  flag:������־
//	Return: 
//		-1:fail
//		else :����id
//    Note:
//----------------------------------------------------------------------------
MINT McfScrn_EntryNewScreen(pfnFunc exitFunc, pfnFunc entryFunc, MVOID* flag);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetCurrEntryFunc
//	Descrption:���ý��봰�ڵĺ���
//	Param:
//	   	IN      func:���봰�ڵĺ���
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_SetCurrEntryFunc(pfnFunc func);
//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_GetCurrEntryFunc
//	Descrption:�õ����봰�ڵĺ���
//	Param:
//	   	IN      func:���봰�ڵĺ���
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
McfScreenFun* McfScrn_GetCurrEntryFunc(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetCurrExitFunc
//	Descrption:�����˳����ڵĺ���
//	Param:
//	   	IN      func:�˳����ڵĺ���	   
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_SetCurrExitFunc(pfnFunc func);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_GetCurrExitFunc
//	Descrption:�õ����봰�ڵĺ���
//	Param:
//	   	IN      func:�˳����ڵĺ���
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
McfScreenFun* McfScrn_GetCurrExitFunc(MVOID);


//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_ClearDelScrnCallback
//	Descrption:���ɾ�����ڵ�callback
//	Param:
//	   	IN  scrnId:����id
//	Return: 
//          MTURE: success
//          MFALSE: fail
//    Note:
//----------------------------------------------------------------------------
MBOOL McfScrn_ClearDelScrnCallback(MINT scrnId);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetDelScrnCallback
//	Descrption:����ɾ�����ڵ�callback
//	Param:
//	   	IN  scrnId:����id
//      IN  pfnDelScrnCb:���ڵĻص�����   
//	Return: 
//          MTURE: success
//          MFALSE: fail
//    Note:
//----------------------------------------------------------------------------
MBOOL McfScrn_SetDelScrnCallback(MINT scrnId, pfnDelFunc func);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_GetDelScrnCallback
//	Descrption:����ɾ�����ڵ�callback
//	Param:
//	   	IN  scrnId:����id
//      IN  pfnDelScrnCb:���ڵĻص�����   
//	Return: 
//          MTURE: success
//          MFALSE: fail
//    Note:
//----------------------------------------------------------------------------
McfScreenDelFun* McfScrn_GetDelScrnCallback(MINT scrnId);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_CloseScreen
//	Descrption:�رմ���
//	Param:
//	   	IN  scrnId:����id
//	Return: 
//          MVOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_CloseScreen(MINT scrnId);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_DeleteScreen
//	Descrption:ɾ������
//	Param:
//	   	IN  scrnId:����id
//	Return: 
//          MVOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_DeleteScreen(MINT scrnId);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_HasFocus
//	Descrption:�жϴ����Ƿ�Ϊfocus
//	Param:
//	   	IN  scrnId:����id
//	Return: 
//          MTURE: success
//          MFALSE: fail
//    Note:
//----------------------------------------------------------------------------
MBOOL McfScrn_HasFocus(MINT scrnId);


//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_DisplayIdleScreen
//	Descrption:��ʾidle����
//	Param:
//          MVOID
//	Return: 
//          MVOID	
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_DisplayIdleScreen(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_DisplayWarnning
//	Descrption:��ʾ����
//	Param:
//          pMessage:����ľ�����Ϣ
//	Return: 
//          MVOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_DisplayWarnning(const MUSHORT* pMessage);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_GetCurScreenID
//	Descrption:��õ�ǰ����id
//	Param:
//          MVOID  
//	Return: 
//          ��ǰ����id
//    Note:
//----------------------------------------------------------------------------
MINT McfScrn_GetCurScreenID(MVOID);


//---------------------------------------------------------------------------
//	FUNCTION: McfMcareRotate
//	Descrption:����MCARE��ת��Ϣ
//	Param:
//          MVOID
//	Return: 
//          MVOID	
//    Note:
//----------------------------------------------------------------------------
MVOID McfMcareRotate(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_Deinit
//	Descrption:ƽ̨��������
//	Param:
//          MVOID
//	Return: 
//          MVOID	
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_Deinit(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetEnable3DMMI
//	Descrption:����3D MMI
//	Param:
//          MVOID
//	Return: 
//          MVOID	   
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_SetEnable3DMMI(MBOOL is_enable);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetCurScreenFocus
//	Descrption:����ǰ��������Ϊfocus״̬
//	Param:
//	   	    MVOID
//	Return: 
//          MTRUE: success
//          MFALSE:fail
//    Note:
//----------------------------------------------------------------------------
MBOOL McfScrn_SetCurScreenFocus(MVOID);

//-----------------------------------------------------------------------------
//	FUNCTION: McfScrn_IsOpenWinMcare
//	Descrption:�жϵ�ǰ�����Ƿ�Ϊmcare����
//	Param:
//	   	    MVOID
//	Return: 
//          MTRUE: yes
//          MFALSE:no
//    Note:
//-----------------------------------------------------------------------------
MBOOL McfScrn_IsOpenWinMcare(MVOID);


//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetCurScreenFocus
//	Descrption:����ǰ��������Ϊfocus״̬
//	Param:
//	   	    MVOID
//	Return: 
//          MTRUE: success
//          MFALSE:fail
//    Note:
//----------------------------------------------------------------------------
MBOOL McfScrn_SetCurScreenClose(MVOID);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif//_MCFSCREEN_H_
