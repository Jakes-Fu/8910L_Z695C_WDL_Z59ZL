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
//	Descrption:     ����һ���༭��
//	Param:
//	   	 IN         pUser:������ָ�룬�ص�ʱ����
//	   	 IN         inputType:���뷨����
//	   	 IN         nmaxNum:���Ա��������ַ���Ŀ
//	   	 IN         defstr:Ĭ����ʾ���ַ�
//	   	 IN         handler:�༭���ص�(���ȷ��)
//	Return:         
//                  >0 : success(return account id)
//                  <=0 : fail
//  Note:           ����༭���棬�༭��󱣴��������ݵ����� 
//------------------------------------------------------------------------------
MINT McfEditor_EnterEditor(MVOID *pUser, MINT inputType, MUSHORT nmaxNum, MWCHAR* defstr, pfnMcfInputCb handler);

//------------------------------------------------------------------------------
//	FUNCTION:       McfEditor_ExitEditor
//	Descrption:     �˳��༭��
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

