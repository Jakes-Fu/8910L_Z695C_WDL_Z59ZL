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
//	Descrption:进入新窗口
//	Param:
//	   	    IN  exitFunc:退出函数 
//	   	    IN  entryFunc:进入函数
//	   	    IN  flag:保留标志
//	Return: 
//		-1:fail
//		else :窗口id
//    Note:
//----------------------------------------------------------------------------
MINT McfScrn_EntryNewScreen(pfnFunc exitFunc, pfnFunc entryFunc, MVOID* flag);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetCurrEntryFunc
//	Descrption:设置进入窗口的函数
//	Param:
//	   	IN      func:进入窗口的函数
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_SetCurrEntryFunc(pfnFunc func);
//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_GetCurrEntryFunc
//	Descrption:得到进入窗口的函数
//	Param:
//	   	IN      func:进入窗口的函数
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
McfScreenFun* McfScrn_GetCurrEntryFunc(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetCurrExitFunc
//	Descrption:设置退出窗口的函数
//	Param:
//	   	IN      func:退出窗口的函数	   
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_SetCurrExitFunc(pfnFunc func);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_GetCurrExitFunc
//	Descrption:得到进入窗口的函数
//	Param:
//	   	IN      func:退出窗口的函数
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
McfScreenFun* McfScrn_GetCurrExitFunc(MVOID);


//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_ClearDelScrnCallback
//	Descrption:清除删除窗口的callback
//	Param:
//	   	IN  scrnId:窗口id
//	Return: 
//          MTURE: success
//          MFALSE: fail
//    Note:
//----------------------------------------------------------------------------
MBOOL McfScrn_ClearDelScrnCallback(MINT scrnId);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetDelScrnCallback
//	Descrption:设置删除窗口的callback
//	Param:
//	   	IN  scrnId:窗口id
//      IN  pfnDelScrnCb:窗口的回调函数   
//	Return: 
//          MTURE: success
//          MFALSE: fail
//    Note:
//----------------------------------------------------------------------------
MBOOL McfScrn_SetDelScrnCallback(MINT scrnId, pfnDelFunc func);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_GetDelScrnCallback
//	Descrption:设置删除窗口的callback
//	Param:
//	   	IN  scrnId:窗口id
//      IN  pfnDelScrnCb:窗口的回调函数   
//	Return: 
//          MTURE: success
//          MFALSE: fail
//    Note:
//----------------------------------------------------------------------------
McfScreenDelFun* McfScrn_GetDelScrnCallback(MINT scrnId);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_CloseScreen
//	Descrption:关闭窗口
//	Param:
//	   	IN  scrnId:窗口id
//	Return: 
//          MVOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_CloseScreen(MINT scrnId);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_DeleteScreen
//	Descrption:删除窗口
//	Param:
//	   	IN  scrnId:窗口id
//	Return: 
//          MVOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_DeleteScreen(MINT scrnId);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_HasFocus
//	Descrption:判断窗口是否为focus
//	Param:
//	   	IN  scrnId:窗口id
//	Return: 
//          MTURE: success
//          MFALSE: fail
//    Note:
//----------------------------------------------------------------------------
MBOOL McfScrn_HasFocus(MINT scrnId);


//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_DisplayIdleScreen
//	Descrption:显示idle界面
//	Param:
//          MVOID
//	Return: 
//          MVOID	
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_DisplayIdleScreen(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_DisplayWarnning
//	Descrption:显示警告
//	Param:
//          pMessage:输入的警告信息
//	Return: 
//          MVOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_DisplayWarnning(const MUSHORT* pMessage);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_GetCurScreenID
//	Descrption:获得当前窗口id
//	Param:
//          MVOID  
//	Return: 
//          当前窗口id
//    Note:
//----------------------------------------------------------------------------
MINT McfScrn_GetCurScreenID(MVOID);


//---------------------------------------------------------------------------
//	FUNCTION: McfMcareRotate
//	Descrption:处理MCARE旋转消息
//	Param:
//          MVOID
//	Return: 
//          MVOID	
//    Note:
//----------------------------------------------------------------------------
MVOID McfMcareRotate(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_Deinit
//	Descrption:平台窗口消除
//	Param:
//          MVOID
//	Return: 
//          MVOID	
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_Deinit(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetEnable3DMMI
//	Descrption:设置3D MMI
//	Param:
//          MVOID
//	Return: 
//          MVOID	   
//    Note:
//----------------------------------------------------------------------------
MVOID McfScrn_SetEnable3DMMI(MBOOL is_enable);

//---------------------------------------------------------------------------
//	FUNCTION: McfScrn_SetCurScreenFocus
//	Descrption:将当前窗口设置为focus状态
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
//	Descrption:判断当前窗口是否为mcare窗口
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
//	Descrption:将当前窗口设置为focus状态
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
