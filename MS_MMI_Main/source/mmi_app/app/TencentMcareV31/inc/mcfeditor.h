/******************************************************************************/
//File Name:        McfEditor.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Modifed:          nicklei 09/21/2011
//Modifed:          wiltan 10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide editor box for mcare
/******************************************************************************/
#ifndef _MCFEDITOR_H_
#define _MCFEDITOR_H_

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "mcfcommon.h"
/*----------------------------------------------------------------------------*/
/*                       Macro Declaration    							      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                  */
/*----------------------------------------------------------------------------*/


typedef struct edit_callback_param
{
    MINT len;
    MVOID* pdata;
    MVOID* pUser;
}EDIT_CALLBACK_PARAM;

typedef struct edit_callback
{
    pfnMcfInputCb callback;
    EDIT_CALLBACK_PARAM callback_param;
}EDIT_CALLBACK;

/*----------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                   */
/*----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
//	FUNCTION:       McfEditor_EnterEditor
//	Descrption:     创建一个编辑框
//	Param:
//	   	 IN         pUser:调用者指针，回调时传回
//	   	 IN         inputType:输入法类型
//	   	 IN         nmaxNum:可以保存的最大字符数目
//	   	 IN         defstr:默认显示的字符
//	   	 IN         handler:编辑完后回调(点击确定)
//	Return:         
//                  >0 : success(return account id)
//                  <=0 : fail
//  Note:           进入编辑界面，编辑完后保存输入内容到缓存 
//------------------------------------------------------------------------------
MINT McfEditor_EnterEditor(MVOID *pUser, MINT inputType, MUSHORT nmaxNum, MWCHAR* defstr, pfnMcfInputCb handler);

//------------------------------------------------------------------------------
//	FUNCTION:       McfEditor_ExitEditor
//	Descrption:     退出编辑框
//	Param:
//                  VOID
//	Return:         
//                  VOID
//  Note: 
//------------------------------------------------------------------------------
MVOID McfEditor_ExitEditor(MVOID);

MINT McfEditor_EnterEditorEx(MVOID* pUser, MINT nInputType, MINT nPorperty, MUSHORT nMaxNum, MWCHAR* pDefstr, pfnMcfInputCb handler);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_MCFEDITOR_H_

