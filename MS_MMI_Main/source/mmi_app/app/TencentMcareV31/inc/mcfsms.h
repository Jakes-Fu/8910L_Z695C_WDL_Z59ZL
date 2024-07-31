/******************************************************************************/
// File Name:       McfSms.h                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         haijunqiu 09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     用来定义短信相关接口                          
/******************************************************************************/

#ifndef _MCFSMS_H_
#define _MCFSMS_H_
#ifdef __cplusplus
extern  "C"
{
#endif
/******************************************************************************/
//                      include files
/******************************************************************************/
#include "mcfcommon.h"

/******************************************************************************/
//                      Macro Declaration
/******************************************************************************/
#define MCFSMS_CONTENT_MAX          (256)

/******************************************************************************/
//                      Type and constant
/******************************************************************************/
typedef struct _M_SMS_PARAM
{
    MVOID* pUser;
    MINT result;
}MSMS_PARAM;

typedef struct _M_SMS_CALLBACK
{
    pfnMcfSmsSendCb sms_callback;
    MSMS_PARAM sms_param;
}MSMS_CALLBACK;

/******************************************************************************/
//                      Local variables Declaration
/******************************************************************************/


/******************************************************************************/
//                      Local function extern
/******************************************************************************/
MVOID McfSms_SetMcareStatus(MBOOL is_mcare);

MBOOL McfSms_GetMcareStatus(MVOID);

/******************************************************************************/
//	FUNCTION: McfSms_SendSms
//	Descrption: 发送短信函数
//	Param:  
//	IN:         pUser 
//              simId:SIM卡的ID
//              pNumber:发送的号码
//              pData:发送内容
//              dataSize:发送内容长度
//              encodeType:内容的编码格式
//              sendSmsCb:发送后的回调函数
//	OUT:        MVOID
//	Return:     0:发送失败1:发送成功
//  Note: 
/******************************************************************************/
MINT McfSms_SendSms(MVOID* pUser, MINT simId, MINT nOption, const MCHAR* pNumber, const MCHAR* pData,
					MINT dataSize, MINT encodeType, pfnMcfSmsSendCb sendSmsCb);


#ifdef SPREAD_NEW_SMS_UPDATE
/******************************************************************************/
//	FUNCTION: McfSms_callback
//	Descrption: :发送短信回调函数
//	Param:  
//	IN:         result :传递给回调函数的参数
//	OUT:        MVOID
//	Return:     
//  Note: 
/******************************************************************************/
MBOOL McfSms_callback(MBOOL is_succss, MVOID* parm);
#else//#ifdef SPREAD_NEW_SMS_UPDATE
/******************************************************************************/
//	FUNCTION: McfSms_callback
//	Descrption: :发送短信回调函数
//	Param:  
//	IN:         result :传递给回调函数的参数
//	OUT:        MVOID
//	Return:     
//  Note: 
/******************************************************************************/
MVOID McfSms_callback(MINT result);
#endif//#ifdef SPREAD_NEW_SMS_UPDATE

/*************************************
* McfSms_WriteMsg
* @pNumber --[in]destination number
* @pContent --[in]msg default content,not support now@0909,arlo
* @nSize --[in]size of msg default content,not support now
* #return none
*/
MVOID McfSms_WriteMsg(MCHAR* pNumber, MINT nNumberCnt, MWCHAR* pContent, MINT nSize);

/******************************************************************************/

MVOID McfSms_Deinit(MVOID);

#ifdef __cplusplus
    }
#endif

#endif //_MCFSMS_H_
