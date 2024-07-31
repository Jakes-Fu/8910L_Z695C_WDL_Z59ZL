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
#define PBSYNC_MAX_EMAIL_NUM      1//先定义为1，联系人存储结构体发生改变时切记同步

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef enum PbSyncHttp_RequestID {	
	PBSYNC_CONTACT_NO_OPERATION, 
	PBSYNC_CONTACT_LISTGET,	//O365 all contacts获取
	PBSYNC_CONTACT_INFO_GET,  //O365 a contact获取
	PBSYNC_CONTACT_MODIFYTIME_GET, //O365 modify time获取
	PBSYNC_CONTACT_ADD,		//添加联系人
	PBSYNC_CONTACT_DELETE,		//删除联系人
	PBSYNC_CONTACT_MODIFY,	//修改联系人
	PBSYNC_CONTACT_ADDED_CONTACT_GET_BYTIME,  //查询Office365某个时间以后添加的联系人
	PBSYNC_CONTACT_UPDATED_CONTACT_GET_BYTIME, //查询Office365某个时间以后修改的联系人
	PBSYNC_CONTACT_COUNT_GET, //查询Office365当前联系人总数
	PBSYNC_CONTACT_ADDED_CONTACTS_COUNT_GET, //查询Office365某个时间以后添加联系人的总数
	PBSYNC_CONTACT_UPDATED_CONTACTS_COUNT_GET, //查询Office365某个时间以后修改联系人的总数
	PBSYNC_CONTACT_MAX
} PbSyncHttp_RequestID;

typedef MMIPB_CONTACT_T MMISRV_CONTACT_T;

//contacts modify time list
typedef struct _MMISRV_CONTACT_MODIFYTIME_T
{
    char  o365_id[MMIPB_SYNC_O365_ID_LEN+1];//Office365 分配的联系人id (新增）
    uint32		o365_modifiedTime;//Office365 联系人的修改时间(新增）
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
//  Description : 请求获取联系人列表数据
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllContacts(void);

/*****************************************************************************/
//  Description : 请求获取指定联系人数据
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetContactById(char *O365_id);
/*****************************************************************************/
//  Description : 请求获取联系人的最近一次修改时间
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllIdAndModiTime(void);
/*****************************************************************************/
//  Description : 查询Office365某个时间以后添加的联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTime指定的时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllAddedContacts(uint32 uiTime);
/*****************************************************************************/
//  Description : 查询Office365某个时间以后修改的联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTime指定的时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllUpdatedContacts(uint32 uiTime);
/*****************************************************************************/
//  Description : 查询Office365当前联系人总数
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetTotalCount(void);
/*****************************************************************************/
//  Description : 查询Office365某个时间以后添加联系人的总数
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:uiTime指定的时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_GetAllAddedContactsCount(uint32 uiTime);
/*****************************************************************************/
//  Description : 请求添加联系人到O365服务器
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_AddContact(uint32 request_id, MMIPB_CONTACT_T *contact_ptr);

/*****************************************************************************/
//  Description : 请求从O365服务器上删除一条联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_DeteteContact(uint32 request_id, char* O365_id);

/*****************************************************************************/
//  Description : 请求从O365服务器上修改联系人
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_SVR_ModifyContact(uint32 request_id, char* O365_id, MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : 将整型格式的时间转化成Office365字符串格式时间
//  Global resource dependence : none
//  Author: shiwei.zhang
//  Note: 2015-7-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UintTimeToO365Str(char *pO365tm/*[out]*/, 
                                                        uint32 uiTm/*[in]*/);
/*****************************************************************************/
//  Description : 将Office365字符串格式时间转化成整型格式的时间
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
