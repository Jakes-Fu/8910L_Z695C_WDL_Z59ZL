/*****************************************************************************
** File Name:      mmipb_sync_datalist.h                                         *
** Author:         shiwei.zhang                                                          *
** Date:           07/09/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 这个文件对联系人同步数据进行管理.  
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365
#ifndef _MMIPB_SYNC_INTERFACE_H_
#define _MMIPB_SYNC_INTERFACE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_custom_define.h"
#include "mmipb_common.h"
#include "mmipb_adapt.h"
#include "mmipb_interface.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/****************************************************************************
*  Description :处理查询Office365返回的一个联系人信息（已转换成phone支持的联系人格式）
*  Global resource dependence : 
*  Note: 参数说明：
*   (1)contact_ptr是已转换成phone支持格式的Office365联系人信息指针
*   (2)chunk_end_flag表示当前查询的数据块结束，可以根据它判断是否需要继续查询余下的联系人信息，如果是，处理完成本条信息后继续调用server提供的MMIAPIPBSYNC_SVR_GetAllContacts()接口进行查询。
*    (chunk_end_flag一般会在处理到数据块中第10条时置为TRUE)
*   (3)all_end_flag 表示所有联系人信息都已查询完毕，PB端可以根据这个标识来做下一步的操作，如记录已完成第一次同步。
*   (all_end_flag一般是在处理到最后一个数据块的最后一条时置为TRUE)
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag);

/****************************************************************************
*  Description :处理查询返回的Office365 某个时间以后添加的联系人（已转换成phone支持的联系人格式）
*   (1)contact_ptr  是已转换成phone支持格式的Office365联系人信息指针
*   (2)chunk_end_flag   表示当前查询的数据块结束，可以根据它判断是否需要继续查询余下的联系人信息，如果是，处理完成本条信息后继续调用server提供的MMIAPIPBSYNC_SVR_GetAllContacts()接口进行查询。
*       chunk_end_flag    一般会在处理到数据块中第10条时置为TRUE.
*   (3)all_end_flag 表示所有联系人信息都已查询完毕，PB端可以根据这个标识来做下一步的操作，如记录已完成第一次同步。
*       all_end_flag   一般是在处理到最后一个数据块的最后一条时置为TRUE.
*   2015-07-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllAddedContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag);

/****************************************************************************
*  Description :    处理查询返回的Office365某个时间以后修改的联系人（已转换成phone支持的联系人格式）
*   (1)contact_ptr  是已转换成phone支持格式的Office365联系人信息指针
*   (2)chunk_end_flag   表示当前查询的数据块结束，可以根据它判断是否需要继续查询余下的联系人信息，如果是，处理完成本条信息后继续调用server提供的MMIAPIPBSYNC_SVR_GetAllContacts()接口进行查询。
*       chunk_end_flag    一般会在处理到数据块中第10条时置为TRUE.
*   (3)all_end_flag 表示所有联系人信息都已查询完毕，PB端可以根据这个标识来做下一步的操作，如记录已完成第一次同步。
*       all_end_flag   一般是在处理到最后一个数据块的最后一条时置为TRUE.
*   2015-07-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag);

/*****************************************************************************
*  Description : 处理查询指定contactId的联系人数据（已转换成phone支持的联系人格式）
*  Global resource dependence : 
*  Note: contact_ptr 已转换成phone支持格式的Office365联系人信息指针
*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetContactByIdHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr);

/*****************************************************************************/
//  Description : 处理从Office365查询到的一个联系人的contactId
//  (1) o365_id : Office365联系人Id指针
//  (2) uiTime 修改时间
//  (3) chunk_end_flag : 表示当前查询的数据块结束，可以根据它判断是否需要继续查询余下的联系人信息（id和修改时间），如果是，处理完成本条信息后继续调用server提供的MMIAPIPBSYNC_SVR_GetAllIdAndModiTime()接口进行查询。
//      chunk_end_flag 一般会在处理到数据块中第10条时置为TRUE.
//  (4) all_end_flag 表示所有联系人id 都已查询完毕，PB 端可以根据这个标识来做下一步的操作，如开始与phont_update_list做冲突检查。
//      all_end_flag一般是在处理到最后一个数据块的最后一条信息时置为TRUE.
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler (uint32 request_id, char* o365_id, uint32 uiTime, BOOLEAN chunk_end_flag , BOOLEAN all_end_flag);

/*****************************************************************************/
//  Description : 处理查询返回的Office365当前联系人总数
//  Global resource dependence : 
//  Note: total_count Office365当前联系人总数
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetTotalCountHandler(uint32 request_id, uint16 total_count);

/*****************************************************************************/
//  Description : 处理查询返回的Office365某个时间以后添加联系人的总数
//  Global resource dependence : 
//  Note: total_count Office365某个时间以后添加联系人的总数
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllAddedContactsCountHandler(uint32 request_id, uint16 total_count);

/*****************************************************************************/
//  Description : 处理查询返回的Office365某个时间以后修改联系人的总数
//  Global resource dependence : 
//  Note: total_count Office365某个时间以后添加联系人的总数
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllUpdatedContactsCountHandler(uint32 request_id, uint16 total_count);

/*****************************************************************************/
//  Description : 处理添加一个联系人到Office365之后返回的联系人信息（contactid和修改时间）
//  Global resource dependence : 
//  Note: o365_id Office365联系人Id指针，uiTime uint32类型Office365联系人最后修改时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_AddContactHandler(uint32 request_id, char* o365_id, uint32 uiTime);

/*****************************************************************************/
//  Description : 处理修改Office365上一个联系人之后返回的联系人信息（contactid和修改时间）
//  Global resource dependence : 
//  Note: o365_id Office365联系人Id指针，uiTime uint32类型Office365联系人最后修改时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_ModifyContactHandler(uint32 request_id, char* o365_id, uint32 uiTime, BOOLEAN bDeletedOnServer);

/*****************************************************************************/
//  Description : 处理Delete Office365上一个联系人call back
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_DeleteContactHandler(uint32 request_id, BOOLEAN isDeleted);

/*****************************************************************************/
//  Description : 错误处理函数，Office365应答异常时，server调用此接口
//  Global resource dependence : 
//  Note: uint32 error_code，如果是网络问题直接填入错误码
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_RequestFailHandler(uint32 request_id, uint32 error_code);

/*****************************************************************************/
//  Description : Callback hander  when login is done
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_LoginHandler(uint32 request_id);

/*****************************************************************************/
//  Description : Callback hander  when logout is done
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_LogoutHandler(uint32 request_id);

#ifdef   __cplusplus
    }
#endif
#endif

#endif


