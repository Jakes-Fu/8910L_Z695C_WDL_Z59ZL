/***************************************************************************
** File Name:      sample_export.h                                             *
** Author:                                                                 *
** Date:           4/01/2009                                               *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2009         Xiaoqing.Lu      Create                                  *
**																		   *
****************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _PBSYNC_EXPORT_H_
#define _PBSYNC_EXPORT_H_


//#define YYA_TEST 1

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"
#include "mmipb_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

#define PBSYNC_MAX_PHONE_NUM      MMIPB_MAX_NV_PHONE_NUM
#define PBSYNC_MAX_EMAIL_NUM      1//�ȶ���Ϊ1����ϵ�˴洢�ṹ�巢���ı�ʱ�м�ͬ��

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef enum PbSyncHttp_RequestID {	
	PBSYNC_CONTACT_NO_OPERATION, 
	PBSYNC_CONTACT_LISTGET,	//O365 all contacts��ȡ
	PBSYNC_CONTACT_INFO_GET,  //O365 a contact��ȡ
	PBSYNC_CONTACT_MODIFYTIME_GET, //O365 modify time��ȡ
	PBSYNC_CONTACT_ADD,		//�����ϵ��
	PBSYNC_CONTACT_DELETE,		//ɾ����ϵ��
	PBSYNC_CONTACT_MODIFY,	//�޸���ϵ��
	PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME,  //��ѯOffice365ĳ��ʱ���Ժ���ӵ���ϵ��
	PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME, //��ѯOffice365ĳ��ʱ���Ժ��޸ĵ���ϵ��
	PBSYNC_CONTACT_COUNT_GET, //��ѯOffice365��ǰ��ϵ������
	PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET, //��ѯOffice365ĳ��ʱ���Ժ������ϵ�˵�����
	PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET, //��ѯOffice365ĳ��ʱ���Ժ��޸���ϵ�˵�����
	PBSYNC_CONTACT_MAX
} PbSyncHttp_RequestID;

typedef MMIPB_CONTACT_T MMISRV_CONTACT_T;

//contacts modify time list
typedef struct _MMISRV_CONTACT_MODIFYTIME_T
{
    char  o365_id[MMIPB_SYNC_O365_ID_LEN+1];//Office365 �������ϵ��id (������
    uint32		o365_modifiedTime;//Office365 ��ϵ�˵��޸�ʱ��(������
} MMISRV_CONTACT_MODIFYTIME_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open hello sprd windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_InitModule(void);

/*****************************************************************************/
// 	Description :MMIAPIPBSYNC_Init
//	Global resource dependence : none
//   Author: Yanyan.an
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_Init(void);

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_StartHttpRequest(void);

/*****************************************************************************/
//  Description : disconnected http and deactive pdp
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_StopHttp(void);

/*****************************************************************************/
//  Description : active pdp and start init http
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_StartInitHttp(void);
/*****************************************************************************/
//  Description : �����ȡ��ϵ���б�����
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllContacts(void);

/*****************************************************************************/
//  Description : �����ȡָ����ϵ������
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetContactById(char *O365_id);
/*****************************************************************************/
//  Description : �����ȡ��ϵ�˵����һ���޸�ʱ��
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllIdAndModiTime(void);
/*****************************************************************************/
//  Description : ��ѯOffice365ĳ��ʱ���Ժ���ӵ���ϵ��
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTimeָ����ʱ��
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllAddedContacts(uint32 uiTime);
/*****************************************************************************/
//  Description : ��ѯOffice365ĳ��ʱ���Ժ��޸ĵ���ϵ��
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTimeָ����ʱ��
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllUpdatedContacts(uint32 uiTime);
/*****************************************************************************/
//  Description : ��ѯOffice365��ǰ��ϵ������
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetTotalCount(void);
/*****************************************************************************/
//  Description : ��ѯOffice365ĳ��ʱ���Ժ������ϵ�˵�����
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTimeָ����ʱ��
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllAddedContactsCount(uint32 uiTime);
/*****************************************************************************/
//  Description : ���������ϵ�˵�O365������
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_AddContact(uint32 request_id, MMIPB_CONTACT_T *contact_ptr);

/*****************************************************************************/
//  Description : �����O365��������ɾ��һ����ϵ��
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_DeteteContact(uint32 request_id, char* O365_id);

/*****************************************************************************/
//  Description : �����O365���������޸���ϵ��
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_ModifyContact(uint32 request_id, char* O365_id, MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : �����͸�ʽ��ʱ��ת����Office365�ַ�����ʽʱ��
//  Global resource dependence : none
//  Author: shiwei.zhang
//  Note: 2015-7-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UintTimeToO365Str(char *pO365tm/*[out]*/, 
                                                        uint32 uiTm/*[in]*/);
/*****************************************************************************/
//  Description : ��Office365�ַ�����ʽʱ��ת�������͸�ʽ��ʱ��
//  Global resource dependence : none
//  Author: shiwei.zhang
//  Note: O365tm : 2015-07-21T06:50:41Z, uiTm = ?
//  2015-7-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_O365StrTimeToUint(uint32 *uiTm/*[out]*/, 
                                                        char *pO365tm/*[in]*/);
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_PBSYNC_EXPORT_H_

#endif
