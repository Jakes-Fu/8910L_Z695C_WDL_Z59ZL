/******************************************************************************/
// File Name:       McfSim.h                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         haijunqiu 09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     用来定义SIM卡相关接口                      
/******************************************************************************/
#ifndef __MCFPB_H__
#define __MCFPB_H__

#ifdef __cplusplus   
extern "C"{   
#endif // __cplusplus

/******************************************************************************/
//                      include files
/******************************************************************************/
#include "mcfcommon.h"

/******************************************************************************/
//                      Macro Declaration
/******************************************************************************/

/******************************************************************************/
//                      Type and constant
/******************************************************************************/
typedef enum _NUM_SEQUENCE
{
	MOBILE_NUMBER,
	HOME_NUMBER,
	OFFICE_NUMBER,
	PHONE_NUMBER,
	FAX_NUMBER
}NUM_SEQUENCE;//胡定义有问题，需要重新整理


typedef struct
{
	MINT m_nStorage;
	MUINT m_nIndex;
	MINT m_PBHandle;//0 代表未使用，>0代表使用的合法handle
}McfPhbHandle;

typedef struct
{
	MVOID* m_pUser;
	MINT m_nResult;
	MINT m_nId;
	MINT m_nStorage;
	pfnPhbNotifyCb m_pCallBack;
}McfPhbCallBack;
/******************************************************************************/
//                      Local variables Declaration
/******************************************************************************/

/******************************************************************************/
//                      Local function extern
/******************************************************************************/

/******************************************************************************/
//                      function declare
/******************************************************************************/

// 遍历接口
MINT McfPhb_FindFirst(MINT nStorage, McfPhbEntry* pEntry);

MINT McfPhb_FindNext(MINT nPhbHandle, McfPhbEntry* pEntry);

MINT McfPhb_FindClose(MINT nPhbHandle);

// 获取指定nStorage的，nID位置的Entry
MINT McfPhb_GetEntry(McfPhbEntry* pEntry);

// 注销回调，应对此时异步调用，回调未返回时，本体被干掉
MINT McfPhb_CancelOperation(MVOID * pUser, pfnPhbNotifyCb func);

MINT McfPhb_QueryStatus(MINT nStorage, MUSHORT* pTotalCount, MUSHORT* pUsedCount);

//***********************************************************************************************************

// 注意如果传入的是汉字，就吐拼音；如果传入的是非汉字，直接把原来的再吐出来；这点与MStar是有区别的
MINT McfMisc_GetCharPinyin(MUSHORT ucs2Char, MCHAR * pPinyin, MINT nSize);

//***********************************************************************************************************


/*************************************************
* McfMisc_RequestCellInfo
* @fnRequestCb --[in]callback function for request
* #return MTRUE --request ok
        MFALE --request fail
*/
MINT McfMisc_RequestCellInfo(MINT nSimId, MVOID* pUser, pfnCellInfoFunc fnRequestCb);

MVOID McfMisc_CancelOperation(MVOID* pUser, pfnCellInfoFunc fnRequestCb);

//***********************************************************************************************************

// 注意这个接口是不管有多少个图标，仅仅显示最近来的图标，且当最上层图标不做显示时，还需要显示第二最近显示的图标。可能有点绕，其实就是需要有个图标的栈，每次只限制栈顶的图标。
/*************************************************
* McfIdle_SetStatusIcon
* @nIconId --[in]given icon id
* @nFlag --[in]MIDLE_ICON_SHOW
            MIDLE_ICON_HIDE
            MIDLE_ICON_BLINK
* #return
*/
//MVOID McfIdle_SetStatusIcon(MSHORT nIconId,MINT nFlag);

//***********************************************************************************************************

MVOID McfRecord_SetRcordStatue(MBOOL is_recording);

MBOOL McfRecord_GetRcordStatue(MVOID);

#ifdef SPREAD_NEW_AUDIO_UPDATE
MBOOL McfRecord_callback(MUINT mocor_hanlde, MVOID* pParam);
#else
MVOID Mcfrecord_callback(MINT nresult, MVOID* pParam);
#endif
/***********************************
* McfAud_StartRecord
*@pUser --[in]
*@pRecord --[in] record info,such as target format,filename, size ,time limit
*@fnAudRecd --[in]callback function to notify user
* #return >=0 audio handle  
        MAUD_FAIL 

*回调告知录音过程的状态结果，如录音完成或中断错误等
*/
MINT McfAud_StartRecord(MVOID *pUser,McfAudRecord *pRecord,pfnAudFunc fnAudRecd);

/***********************************
* McfAud_StopRecord
* @nHd --[in]related handle
* #return MAUD_SUCCESS 
        MAUD_FAIL 
*/
MINT McfAud_StopRecord(MINT nHd);


MVOID McfRecord_StopRcordException(MVOID);

/*************************************************
* MAud_GetDuration
* @pDuration --[out]media duration,in seconds
* @nType --[in] MAUD_SRC_FILE-source is filename,MAUD_SRC_BUFFER--source is audio data
* @pSource --[in]maybe source filename or data,depends on nType
* @nFormat --[in]source audio format,valid when nType is MAUD_SRC_BUFFER
* @nLen --[in]source audio data len,valid when nType is MAUD_SRC_BUFFER 
* #return MAUD_SUCCESS --get the media duration
        MAUD_FAIL       --cannot get 
*/
MINT McfAud_GetDuration(MUINT *pDuration,MINT nType,MVOID * pSource, MINT nFormat, MINT nLen);


//***********************************************************************************************************


/*************************************************
* McfCam_Open,打开照相机
* @pUser --[in]
* @nCamId --[in]logcial camera id
* @nMode --[in]open mode
* @fnCamNotify --[in]callback function 
* #return MCAM_SUCCESS
        MCAM_ERROR

* MCAM_MODE_LOCAL   --直接拉起本地的camera应用
* MCAM_MODE_USER    --启动camera，支持preview/capture功能    ，支持图片
                        大小等基本参数的设置
*/



MINT McfCam_Open(MVOID *pUser,MINT nCamId,MINT nMode,pfnCamNotify fnCamNotify);
MINT McfCam_Close(MINT nCamId,MINT nMode);
MINT McfCam_StartPreview(McfCamParam *pParam);
MINT McfCam_StopPreview(MVOID);
MINT McfCam_CaptureToFile(MWCHAR *pFilepath,MINT *pImgFmt);
MINT McfCam_CaptureToMem(MBYTE **ppBuffer, MUINT *pSize,MINT *pImgFmt);
MINT McfCam_QueryProperty(McfCamProp *pProp);//


MVOID McfMisc_CancelCellInfo(MVOID * pUser, pfnCellInfoFunc fnRequestCb);
MINT McfMisc_DeliverMessage(MINT nAppId,MINT nEvtId,MVOID *pData,MINT nSize);
MVOID McfPhb_AddCallBack(MVOID* pUser);
MVOID McfPhb_UpdateCallBack(MVOID* pUser);
MVOID McfPhb_DelCallBack(MVOID* pUser);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_SetAddCallBack
//	Descrption:用来设置添加记录成功后执行的回调函数
//	Param:
//      IN   fnAddCb:待添加的回调函数
//      IN   pUser:回调函数需要传入的参数
//      IN   storage:记录操作的记录的所在存储类型
//      IN   entry_id:记录操作的记录的所在存储ID
//      IN   result:记录操作的记录的操作相关结果
//	Return: 
//      MVOID
//  Note:
//------------------------------------------------------------------------------
MVOID McfPhb_SetAddCallBack(pfnPhbNotifyCb fnAddCb,
                                MVOID* pUser, 
                                MINT storage, 
                                MINT entry_id,
                                MINT result);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_SetUpdateCallBack
//	Descrption:用来设置更新成功后执行的回调函数
//	Param:
//      IN   fnAddCb:待添加的回调函数
//      IN   pUser:回调函数需要传入的参数
//      IN   storage:记录操作的记录的所在存储类型
//      IN   entry_id:记录操作的记录的所在存储ID
//      IN   result:记录操作的记录的操作相关结果
//	Return: 
//      MVOID
//  Note:
//------------------------------------------------------------------------------

MVOID McfPhb_SetUpdateCallBack(pfnPhbNotifyCb fnUpdateCb,
                                MVOID* pUser, 
                                MINT storage, 
                                MINT entry_id,
                                MINT result);    
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_SetDelCallBack
//	Descrption:用来设置删除成功后执行的回调函数
//	Param:
//      IN   fnAddCb:待添加的回调函数
//      IN   pUser:回调函数需要传入的参数
//      IN   storage:记录操作的记录的所在存储类型
//      IN   entry_id:记录操作的记录的所在存储ID
//      IN   result:记录操作的记录的操作相关结果
//	Return: 
//      MVOID
//  Note:
//------------------------------------------------------------------------------

MVOID McfPhb_SetDelCallBack(pfnPhbNotifyCb fnDelCb,
                                MVOID* pUser, 
                                MINT storage, 
                                MINT entry_id,
                                MINT result);                                
#endif//__MCFPB_H__

