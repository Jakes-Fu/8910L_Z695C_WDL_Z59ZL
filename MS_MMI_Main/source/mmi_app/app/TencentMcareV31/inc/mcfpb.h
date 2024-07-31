/******************************************************************************/
// File Name:       McfSim.h                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         haijunqiu 09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     ��������SIM����ؽӿ�                      
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
}NUM_SEQUENCE;//�����������⣬��Ҫ��������


typedef struct
{
	MINT m_nStorage;
	MUINT m_nIndex;
	MINT m_PBHandle;//0 ����δʹ�ã�>0����ʹ�õĺϷ�handle
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

// �����ӿ�
MINT McfPhb_FindFirst(MINT nStorage, McfPhbEntry* pEntry);

MINT McfPhb_FindNext(MINT nPhbHandle, McfPhbEntry* pEntry);

MINT McfPhb_FindClose(MINT nPhbHandle);

// ��ȡָ��nStorage�ģ�nIDλ�õ�Entry
MINT McfPhb_GetEntry(McfPhbEntry* pEntry);

// ע���ص���Ӧ�Դ�ʱ�첽���ã��ص�δ����ʱ�����屻�ɵ�
MINT McfPhb_CancelOperation(MVOID * pUser, pfnPhbNotifyCb func);

MINT McfPhb_QueryStatus(MINT nStorage, MUSHORT* pTotalCount, MUSHORT* pUsedCount);

//***********************************************************************************************************

// ע�����������Ǻ��֣�����ƴ�������������ǷǺ��֣�ֱ�Ӱ�ԭ�������³����������MStar���������
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

// ע������ӿ��ǲ����ж��ٸ�ͼ�꣬������ʾ�������ͼ�꣬�ҵ����ϲ�ͼ�겻����ʾʱ������Ҫ��ʾ�ڶ������ʾ��ͼ�ꡣ�����е��ƣ���ʵ������Ҫ�и�ͼ���ջ��ÿ��ֻ����ջ����ͼ�ꡣ
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

*�ص���֪¼�����̵�״̬�������¼����ɻ��жϴ����
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
* McfCam_Open,�������
* @pUser --[in]
* @nCamId --[in]logcial camera id
* @nMode --[in]open mode
* @fnCamNotify --[in]callback function 
* #return MCAM_SUCCESS
        MCAM_ERROR

* MCAM_MODE_LOCAL   --ֱ�����𱾵ص�cameraӦ��
* MCAM_MODE_USER    --����camera��֧��preview/capture����    ��֧��ͼƬ
                        ��С�Ȼ�������������
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
//	Descrption:����������Ӽ�¼�ɹ���ִ�еĻص�����
//	Param:
//      IN   fnAddCb:����ӵĻص�����
//      IN   pUser:�ص�������Ҫ����Ĳ���
//      IN   storage:��¼�����ļ�¼�����ڴ洢����
//      IN   entry_id:��¼�����ļ�¼�����ڴ洢ID
//      IN   result:��¼�����ļ�¼�Ĳ�����ؽ��
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
//	Descrption:�������ø��³ɹ���ִ�еĻص�����
//	Param:
//      IN   fnAddCb:����ӵĻص�����
//      IN   pUser:�ص�������Ҫ����Ĳ���
//      IN   storage:��¼�����ļ�¼�����ڴ洢����
//      IN   entry_id:��¼�����ļ�¼�����ڴ洢ID
//      IN   result:��¼�����ļ�¼�Ĳ�����ؽ��
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
//	Descrption:��������ɾ���ɹ���ִ�еĻص�����
//	Param:
//      IN   fnAddCb:����ӵĻص�����
//      IN   pUser:�ص�������Ҫ����Ĳ���
//      IN   storage:��¼�����ļ�¼�����ڴ洢����
//      IN   entry_id:��¼�����ļ�¼�����ڴ洢ID
//      IN   result:��¼�����ļ�¼�Ĳ�����ؽ��
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

