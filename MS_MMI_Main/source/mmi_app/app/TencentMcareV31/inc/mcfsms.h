/******************************************************************************/
// File Name:       McfSms.h                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         haijunqiu 09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     �������������ؽӿ�                          
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
//	Descrption: ���Ͷ��ź���
//	Param:  
//	IN:         pUser 
//              simId:SIM����ID
//              pNumber:���͵ĺ���
//              pData:��������
//              dataSize:�������ݳ���
//              encodeType:���ݵı����ʽ
//              sendSmsCb:���ͺ�Ļص�����
//	OUT:        MVOID
//	Return:     0:����ʧ��1:���ͳɹ�
//  Note: 
/******************************************************************************/
MINT McfSms_SendSms(MVOID* pUser, MINT simId, MINT nOption, const MCHAR* pNumber, const MCHAR* pData,
					MINT dataSize, MINT encodeType, pfnMcfSmsSendCb sendSmsCb);


#ifdef SPREAD_NEW_SMS_UPDATE
/******************************************************************************/
//	FUNCTION: McfSms_callback
//	Descrption: :���Ͷ��Żص�����
//	Param:  
//	IN:         result :���ݸ��ص������Ĳ���
//	OUT:        MVOID
//	Return:     
//  Note: 
/******************************************************************************/
MBOOL McfSms_callback(MBOOL is_succss, MVOID* parm);
#else//#ifdef SPREAD_NEW_SMS_UPDATE
/******************************************************************************/
//	FUNCTION: McfSms_callback
//	Descrption: :���Ͷ��Żص�����
//	Param:  
//	IN:         result :���ݸ��ص������Ĳ���
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
