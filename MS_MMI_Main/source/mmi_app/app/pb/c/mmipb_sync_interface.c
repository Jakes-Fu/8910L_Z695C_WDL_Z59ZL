/****************************************************************************
** File Name:      mmipb_sync_interface.c                                        *
** Author:           shiwei.zhang                                                      *
** Date:           07/09/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 这个文件实现联系人同步PB 侧对外接口
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _MMIPB_SYNC_INTERFACE_C_
#define _MMIPB_SYNC_INTERFACE_C_

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mn_type.h"
#include "mmipb_datalist.h"
#include "mmipb_storage.h"
#include "mmipb_sync_interface.h"
#include "mmipb_task.h"
#include "mmipb_sync_datalist.h"
#include "mmipb_view.h"
#include "mmipb_interface.h"
#include "mmipb_set.h"

#define MMIPB_SYNC_GET_COUNT_PARALLEL

/****************************************************************************
*  Description :处理查询Office365返回的一个联系人信息（已转换成phone支持的联系人格式）
*   (1)contact_ptr是已转换成phone支持格式的Office365联系人信息指针
*   (2)chunk_end_flag表示当前查询的数据块结束，可以根据它判断是否需要继续查询余下的联系人信息，如果是，处理完成本条信息后继续调用server提供的MMIAPIPBSYNC_SVR_GetAllContacts()接口进行查询。
*       chunk_end_flag一般会在处理到数据块中第10条时置为TRUE.
*   (3)all_end_flag 表示所有联系人信息都已查询完毕，PB端可以根据这个标识来做下一步的操作，如记录已完成第一次同步。
*       all_end_flag一般是在处理到最后一个数据块的最后一条时置为TRUE.
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag)
{
    MMIPB_CONTACT_LIST_NODE_T * node = NULL;

    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler");

    if(PNULL != contact_ptr)
    {
        if (MMIPB_ERROR_SUCCESS != MMIPB_SYNC_GetListNodeByO365Id(&node, contact_ptr->o365_id))
        {
            MMIPB_SYNC_UpdateCurSyncTime(contact_ptr->o365_modifiedTime);
            contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
            //contact_ptr->contact_id = MMIPB_GetFreeContactId(contact_ptr->storage_id);
            contact_ptr->contact_id = 0;//if contact_id = 0, contactid will be autocreated

            if(MMIPB_ERROR_SUCCESS != MMIAPIPB_AddContact(contact_ptr))
            {
                //SCI_FREE(contact_ptr);//没必要都free，不过暂时保留方便追溯问?
                SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler MMIAPIPB_AddContact fail");//return MMIPB_ERROR_ERROR;保存失败不能轻易return
            }
            else
            {
                //provide contact info to pb_viewer
                //MMIPB_SYNC_Notify_Update_Info(contact_ptr->contact_id, contact_ptr->storage_id, MMIPB_OPC_ADD);
            }
            //SCI_FREE(contact_ptr); it is a struct, do not need free
            //contact_ptr = PNULL;
        }
        else 
        {
            SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler find same o365id=%s", contact_ptr->o365_id);//return MMIPB_ERROR_ERROR;保存失败不能轻易return
        }
    }
    else
    {
        //contact_ptr = PNULL;
        //return MMIPB_ERROR_INVALID_PARAM;
    }

    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SYNC_Notify_Update_Info(0, 0, MMIPB_OPC_ADD);
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }

    if(TRUE == all_end_flag)
    {
        MMIPB_SYNC_Notify_Update_Info(0, 0, MMIPB_OPC_ADD);
        // first sync over.
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        //MMIPB_SYNC_InformUpdateList();
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler,all_end_flag = TRUE");
        return MMIPB_ERROR_SUCCESS;
    }

    if(TRUE == chunk_end_flag)
    {
        MMIPB_SYNC_Notify_Update_Info(0, 0, MMIPB_OPC_ADD);
        //call MMIAPIPBSYNC_SVR_GetAllContacts() to continue;
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_CONTACTS_TO_PHONE, 0);
        //MMIPB_SYNC_InformUpdateList();
    }

    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler");
    return MMIPB_ERROR_SUCCESS;
}

/****************************************************************************
*  Description :处理查询返回的Office365 某个时间以后添加的联系人（已转换成phone支持的联系人格式）
*   (1)contact_ptr  是已转换成phone支持格式的Office365联系人信息指针
*   (2)chunk_end_flag   表示当前查询的数据块结束，可以根据它判断是否需要继续查询余下的联系人信息，如果是，处理完成本条信息后继续调用server提供的MMIAPIPBSYNC_SVR_GetAllContacts()接口进行查询。
*       chunk_end_flag    一般会在处理到数据块中第10条时置为TRUE.
*   (3)all_end_flag 表示所有联系人信息都已查询完毕，PB端可以根据这个标识来做下一步的操作，如记录已完成第一次同步。
*       all_end_flag   一般是在处理到最后一个数据块的最后一条时置为TRUE.
*   2015-07-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllAddedContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag)
{
    MMIPB_CONTACT_LIST_NODE_T * node = NULL;
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsHandler");
    
    if(PNULL != contact_ptr)
    {
        if (MMIPB_ERROR_SUCCESS != MMIPB_SYNC_GetListNodeByO365Id(&node, contact_ptr->o365_id))
        {
            MMIPB_SYNC_UpdateCurSyncTime(contact_ptr->o365_modifiedTime);
            contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
            //contact_ptr->contact_id = MMIPB_GetFreeContactId(contact_ptr->storage_id);
            contact_ptr->contact_id = 0;//if contact_id = 0, contactid will be autocreated
            if(MMIPB_ERROR_SUCCESS != MMIAPIPB_AddContact(contact_ptr))//wirte NV
            {
                //SCI_FREE(contact_ptr);//没必要都free，不过暂时保留方便追溯问题
                SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsHandler MMIAPIPB_AddContact fail");//return MMIPB_ERROR_ERROR;保存失败不能轻易return
            }
            else
            {
                //provide contact info to pb_viewer
                //MMIPB_SYNC_Notify_Update_Info(contact_ptr->contact_id, contact_ptr->storage_id, MMIPB_OPC_ADD);
            }

            //SCI_FREE(contact_ptr);
            //contact_ptr = PNULL;
        }
        else 
        {
            SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsHandler find same o365id=%s", contact_ptr->o365_id);//return MMIPB_ERROR_ERROR;保存失败不能轻易return
        }
    }
    else
    {
        //return MMIPB_ERROR_INVALID_PARAM; it means no contact is added in office365
        //go next step
        //MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_UPDATED_TO_PHONE, 0);
    }

    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SYNC_Notify_Update_Info(0, 0, MMIPB_OPC_ADD);
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }

    if(TRUE == all_end_flag)
    {
        MMIPB_SYNC_Notify_Update_Info(0, 0, MMIPB_OPC_ADD);
        //all added contacts sync over.
        //(2) Update contact on the phone according to Office365 
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_UPDATED_TO_PHONE, 0);
        //MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        //MMIPB_SYNC_InformUpdateList();
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsHandler,all_end_flag = TRUE");
        return MMIPB_ERROR_SUCCESS;
    }

    if(TRUE == chunk_end_flag)
    {
        MMIPB_SYNC_Notify_Update_Info(0, 0, MMIPB_OPC_ADD);
        //call MMIAPIPBSYNC_SVR_GetAllAddedContacts (last_light_sync_time) to continue;
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_ADDED_TO_PHONE, 0);
        //MMIPB_SYNC_InformUpdateList();
    }

    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsHandler");
    return MMIPB_ERROR_SUCCESS;
}

/****************************************************************************
*  Description :    处理查询返回的Office365某个时间以后修改的联系人（已转换成phone支持的联系人格式）
*   (1)contact_ptr  是已转换成phone支持格式的Office365联系人信息指针
*   (2)chunk_end_flag   表示当前查询的数据块结束，可以根据它判断是否需要继续查询余下的联系人信息，如果是，处理完成本条信息后继续调用server提供的MMIAPIPBSYNC_SVR_GetAllContacts()接口进行查询。
*       chunk_end_flag    一般会在处理到数据块中第10条时置为TRUE.
*   (3)all_end_flag 表示所有联系人信息都已查询完毕，PB端可以根据这个标识来做下一步的操作，如记录已完成第一次同步。
*       all_end_flag   一般是在处理到最后一个数据块的最后一条时置为TRUE.
*   2015-07-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag)
{
    BOOLEAN o365IsLate = FALSE;
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler");
    
    if(PNULL != contact_ptr)//it it better to alloc a new contact buffer
    {
        o365IsLate = MMIPB_SYNC_Conflict_With_Phone_UpdateLst_Proc(contact_ptr->o365_modifiedTime, contact_ptr->o365_id);
        if (TRUE == o365IsLate)//add the o365 contact to phone if it is significant.
        {
            /* o365 contact is significant, delete the item in local_update_list */
            MMIPB_SYNC_Phone_Update_List_DeleteByO365Id(contact_ptr->o365_id);
            SCI_TRACE_LOW("[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler 365 update is late.");  
            MMIPB_SYNC_UpdateCurSyncTime(contact_ptr->o365_modifiedTime);
            if (MMIPB_ERROR_SUCCESS == MMIPB_SYNC_GetPhoneIdByO365Id(contact_ptr->o365_id,
                                                                        &contact_ptr->contact_id,
                                                                        &contact_ptr->storage_id))
            {
                SCI_TRACE_LOW("[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler modify contact requestid = %d.", request_id);
                if(MMIPB_ERROR_SUCCESS != MMIPB_ModifyContact(contact_ptr))//update NV
                {
                    //SCI_FREE(contact_ptr);//没必要都free，不过暂时保留方便追溯问题
                    SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler MMIPB_ModifyContact fail");//return MMIPB_ERROR_ERROR;保存失败不能轻易return
                }
                else
                {
                    //provide contact info to pb_viewer
                    MMIPB_SYNC_Notify_Update_Info(contact_ptr->contact_id, contact_ptr->storage_id, MMIPB_OPC_EDIT);
                }
            }
            else
            {
                SCI_TRACE_LOW("[MMIPBSYNC] !!! error MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler no such item in contact list.");
            }
        }
    }
    else
    {
        //return MMIPB_ERROR_INVALID_PARAM; it means no contact is updated in office365
        //go next step
        //MMIPB_SYNC_UpdateO365DeletedOrContinue();
    }

    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }
        
    if(TRUE == all_end_flag)
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_PHONE_LIST_ITEM_TO_O365, 0);
        SCI_TRACE_LOW("[MMIPBSYNC] send MMIPB_SYNC_PHONE_LIST_ITEM_TO_O365!");
        //MMIPB_SYNC_InformUpdateList();
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler3");
        return MMIPB_ERROR_SUCCESS;
    }

    if(TRUE == chunk_end_flag)
    {
        //call MMIAPIPBSYNC_SVR_GetAllUpdatedContacts (last_light_sync_time) to continue;
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_UPDATED_TO_PHONE, 0);
        //MMIPB_SYNC_InformUpdateList();
    }
        
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler");
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 处理从Office365查询到的一个联系人的contactId
//  (1) o365_id : Office365联系人Id指针
//  (2) uiTime 修改时间
//  (3) chunk_end_flag : 表示当前查询的数据块结束，可以根据它判断是否需要继续查询余下的联系人信息（id和修改时间），如果是，处理完成本条信息后继续调用server提供的MMIAPIPBSYNC_SVR_GetAllIdAndModiTime()接口进行查询。
//      chunk_end_flag 一般会在处理到数据块中第10条时置为TRUE.
//  (4) all_end_flag 表示所有联系人id 都已查询完毕，PB 端可以根据这个标识来做下一步的操作，如开始与phont_update_list做冲突检查。
//      all_end_flag一般是在处理到最后一个数据块的最后一条信息时置为TRUE.
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler (uint32 request_id, char* o365_id, uint32 uiTime, BOOLEAN chunk_end_flag , BOOLEAN all_end_flag)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler");
    
    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }
        
    if(PNULL != o365_id)
    {
        MMIPB_SYNC_SetPhoneContactCheckedByO365Id(o365_id);
        MMIPB_SYNC_UpdateCurSyncTime(uiTime);
    }
    /*else if((PNULL == o365_id)&&(PNULL == o365_id)&&(PNULL == o365_id)&&(PNULL == o365_id))
    {//need not to do o365 deleted contacts checking, go complete
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        return MMIPB_ERROR_SUCCESS;
    }*/

    if(TRUE == all_end_flag)
    {
        //start find the deleted contacts by o365, checked is FALSE.
        //MMIPB_SYNC_UpdateO365DeletedContactsToPB
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_DELETED_TO_PHONE, 0);
        //MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler2");
        return MMIPB_ERROR_SUCCESS;
    }

    if(TRUE == chunk_end_flag)
    {
        //call MMIAPIPBSYNC_SVR_GetAllIdAndModiTime() to continue;
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_ID_AND_MODIFIED_TIME, 0);
        //MMISRV_PBSYNC_OpenWaitingWindow();
    }
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler");
    return MMIPB_ERROR_SUCCESS;
    
}

/*****************************************************************************/
//  Description : 处理查询返回的Office365当前联系人总数
//  Global resource dependence : 
//  Note: total_count Office365当前联系人总数
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetTotalCountHandler(uint32 request_id, uint16 total_count)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_GetTotalCountHandler, total_count = %d", total_count);
    
    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetTotalCountHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }
        
    MMIPB_SYNC_UpdateCurO365ContactsCount(total_count);
    if (MMIPB_SYNC_TYPE_FIRST_TIME == MMIPB_SYNC_GetContactsSyncType())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_CONTACTS_TO_PHONE, 0); //-----first sync step 2, get all ---log
    }
    else
    {
        //heavy sync
#ifdef MMIPB_SYNC_GET_COUNT_PARALLEL
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_CHECK_IF_HEAVY_SYNC_POSSIBLE, 0); //-----heavy sync Step 4, check if heavy sync possilbe and show user a dialog to confirm sync
#else
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_ADDED_CONTACTS_COUNT, 0); //-----heavy sync check Step 2,get all added count
#endif
    }
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetTotalCountHandler");
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 处理查询返回的Office365某个时间以后添加联系人的总数
//  Global resource dependence : 
//  Note: total_count Office365某个时间以后添加联系人的总数
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllAddedContactsCountHandler(uint32 request_id, uint16 total_count)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsCountHandler, total_count = %d", total_count);
    
    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsCountHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }
        
    //MMIPB_SYNC_StartHeavyWeightContactsSync(total_count);//-----heavy sync check Step 3---log
    MMIPB_SYNC_UpdateCurO365AddedContactsCount(total_count);

#ifdef MMIPB_SYNC_GET_COUNT_PARALLEL
    MMIPB_SendSignalToPBTask(MMIPB_SYNC_CHECK_IF_HEAVY_SYNC_POSSIBLE, 0); //-----heavy sync Step 4, check if heavy sync possilbe and show user a dialog to confirm sync
#else
    // MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_ADDED_TO_PHONE, 0); //-----heavy sync Step 3, go to light sync step 1---log
    MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_UPDATED_CONTACTS_COUNT, 0); //-----heavy sync Step 3, get all updated count
#endif  
    //test wk
    //MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_UPDATED_TO_PHONE, 0);
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsCountHandler");
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 处理查询返回的Office365某个时间以后修改联系人的总数
//  Global resource dependence : 
//  Note: total_count Office365某个时间以后添加联系人的总数
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllUpdatedContactsCountHandler(uint32 request_id, uint16 total_count)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsCountHandler, total_count = %d", total_count);
    
    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsCountHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }

    MMIPB_SYNC_UpdateCurO365UpdatedContactsCount(total_count);  
    MMIPB_SendSignalToPBTask(MMIPB_SYNC_CHECK_IF_HEAVY_SYNC_POSSIBLE, 0); //-----heavy sync Step 4, check if heavy sync possilbe and show user a dialog to confirm sync
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_GetAllUpdatedContactsCountHandler");
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 处理添加一个联系人到Office365之后返回的联系人信息（contactid和修改时间）
//  Global resource dependence : 
//  Note: o365_id Office365联系人Id指针，uiTime uint32类型Office365联系人最后修改时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_AddContactHandler(uint32 request_id, char* o365_id, uint32 uiTime)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_AddContactHandler,request_id=0X%X", request_id);
    
    if (PNULL != o365_id)
    {
        if (MMIPB_ERROR_SUCCESS == MMIPB_SYNC_UpdateAddedContactFromO365((MMIPB_SYNC_PHONE_UPDATE_LIST_T *)request_id, o365_id, uiTime))
        {
            MMIPB_SYNC_UpdateCurSyncTime(uiTime);
        }
        //MMIPB_SYNC_InformUpdateList();//20150819
    }

    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_AddContactHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }
        
    MMIPB_SendSignalToPBTask(MMIPB_SYNC_PHONE_LIST_ITEM_TO_O365, 0);
    
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_AddContactHandler");
    
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 处理修改Office365上一个联系人之后返回的联系人信息（contactid和修改时间）
//  Global resource dependence : 
//  Note: o365_id Office365联系人Id指针，uiTime uint32类型Office365联系人最后修改时间
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_ModifyContactHandler(uint32 request_id, char* o365_id, uint32 uiTime, BOOLEAN bDeletedOnServer)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_ModifyContactHandler,request_id=0X%X, bDeletedOnServer=%d", request_id, bDeletedOnServer);
    
    if (bDeletedOnServer)
    {
        MMIPB_SYNC_ReAddLocalPhoneModifyItem((MMIPB_SYNC_PHONE_UPDATE_LIST_T *)request_id);
        SCI_TRACE_LOW("[MMIPBSYNC] MMIAPIPBSYNC_PB_ModifyContactHandler,re-add case");
    }
    else if (PNULL != o365_id)
    {
        SCI_TRACE_LOW("[PBSYNC] O --->P MMIAPIPBSYNC_PB_ModifyContactHandler.");
        if (MMIPB_ERROR_SUCCESS == MMIPB_SYNC_UpdateModifiedContactFromO365((MMIPB_SYNC_PHONE_UPDATE_LIST_T *)request_id, o365_id, uiTime))
        {
            MMIPB_SYNC_UpdateCurSyncTime(uiTime);
        }
        //MMIPB_SYNC_InformUpdateList();//20150819
    }

    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_ModifyContactHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }

    MMIPB_SendSignalToPBTask(MMIPB_SYNC_PHONE_LIST_ITEM_TO_O365, 0);
    
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_ModifyContactHandler");
    
    return MMIPB_ERROR_SUCCESS;
}

PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_DeleteContactHandler(uint32 request_id, BOOLEAN isDeleted)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_DeleteContactHandler,request_id=0X%X, isDeleted=%d", request_id, isDeleted);

    /* check if it is delete on server successfully? */
    if (isDeleted)
    {
        /* phone_delete_item is deleted on serve successfully, clear this phone_delete_item */
        MMIPB_SYNC_ClearLocalPhoneDeleteItem((MMIPB_SYNC_PHONE_UPDATE_LIST_T *)request_id);

        //MMIPB_SYNC_InformUpdateList();
    }

    if(TRUE == MMIPB_SYNC_IsStopSync())
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_DeleteContactHandler, sync stopped,skip");
        return MMIPB_ERROR_SUCCESS;
    }   

    MMIPB_SendSignalToPBTask(MMIPB_SYNC_PHONE_LIST_ITEM_TO_O365, 0);
    
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_DeleteContactHandler");
    
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 错误处理函数，Office365应答异常时，server调用此接口
//  Global resource dependence : 
//  Note: uint32 error_code，如果是网络问题直接填入错误码
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_RequestFailHandler(uint32 request_id, uint32 error_code)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_RequestFailHandler,error_code=%d", error_code);
    
    MMIPB_SYNC_SetSyncErrorCode(error_code);
#if 0
	//zouruoqi add for prompting error windows
	if(FALSE == MMIPB_SYNC_IsHeartbeatStart() && TRUE == MMIPB_SYNC_IsShowEorr())
    {
		if(error_code == 64 || error_code == 6 )//error_code is RESPONSE_NOT_PERMITTED
		{
		MMIPB_OpenSyncErrorPermissionDialogWin();
		MMIPB_SYNC_NV_Set_HeartBeat(FALSE);
		}
		else{
		MMIPB_OpenSyncErrorNetworkDialogWin();
		}
		MMIPB_SYNC_SetShowEorr(FALSE);
	}
    //end zouruoqi*/
#endif
    MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
    SCI_TRACE_LOW("[PBSYNC] send MMIPB_SYNC_COMPLETE");
    
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_RequestFailHandler");
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : Callback hander  when login is done
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_LoginHandler(uint32 request_id)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_LoginHandler");
    
    MMIPB_SendSignalToPBTask(MMIPBSYNC_SIG_LOGIN_CNF, 0);
    SCI_TRACE_LOW("[PBSYNC] send MMIPBSYNC_SIG_LOGIN_CNF");
    
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_LoginHandler");
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : Callback hander  when logout is done
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_LogoutHandler(uint32 request_id)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIAPIPBSYNC_PB_LogoutHandler");
    
    MMIPB_SendSignalToPBTask(MMIPBSYNC_SIG_LOGOUT_CNF, 0);
    SCI_TRACE_LOW("[PBSYNC] send MMIAPIPBSYNC_PB_LogoutHandler");
    
    SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_PB_LogoutHandler");
    return MMIPB_ERROR_SUCCESS;
}
#endif
#endif
