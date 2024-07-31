/****************************************************************************
** File Name:      mmipb_sync_interface.c                                        *
** Author:           shiwei.zhang                                                      *
** Date:           07/09/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 ����ļ�ʵ����ϵ��ͬ��PB �����ӿ�
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
*  Description :�����ѯOffice365���ص�һ����ϵ����Ϣ����ת����phone֧�ֵ���ϵ�˸�ʽ��
*   (1)contact_ptr����ת����phone֧�ָ�ʽ��Office365��ϵ����Ϣָ��
*   (2)chunk_end_flag��ʾ��ǰ��ѯ�����ݿ���������Ը������ж��Ƿ���Ҫ������ѯ���µ���ϵ����Ϣ������ǣ�������ɱ�����Ϣ���������server�ṩ��MMIAPIPBSYNC_SVR_GetAllContacts()�ӿڽ��в�ѯ��
*       chunk_end_flagһ����ڴ������ݿ��е�10��ʱ��ΪTRUE.
*   (3)all_end_flag ��ʾ������ϵ����Ϣ���Ѳ�ѯ��ϣ�PB�˿��Ը��������ʶ������һ���Ĳ��������¼����ɵ�һ��ͬ����
*       all_end_flagһ�����ڴ������һ�����ݿ�����һ��ʱ��ΪTRUE.
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
                //SCI_FREE(contact_ptr);//û��Ҫ��free��������ʱ��������׷����?
                SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler MMIAPIPB_AddContact fail");//return MMIPB_ERROR_ERROR;����ʧ�ܲ�������return
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
            SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler find same o365id=%s", contact_ptr->o365_id);//return MMIPB_ERROR_ERROR;����ʧ�ܲ�������return
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
*  Description :�����ѯ���ص�Office365 ĳ��ʱ���Ժ���ӵ���ϵ�ˣ���ת����phone֧�ֵ���ϵ�˸�ʽ��
*   (1)contact_ptr  ����ת����phone֧�ָ�ʽ��Office365��ϵ����Ϣָ��
*   (2)chunk_end_flag   ��ʾ��ǰ��ѯ�����ݿ���������Ը������ж��Ƿ���Ҫ������ѯ���µ���ϵ����Ϣ������ǣ�������ɱ�����Ϣ���������server�ṩ��MMIAPIPBSYNC_SVR_GetAllContacts()�ӿڽ��в�ѯ��
*       chunk_end_flag    һ����ڴ������ݿ��е�10��ʱ��ΪTRUE.
*   (3)all_end_flag ��ʾ������ϵ����Ϣ���Ѳ�ѯ��ϣ�PB�˿��Ը��������ʶ������һ���Ĳ��������¼����ɵ�һ��ͬ����
*       all_end_flag   һ�����ڴ������һ�����ݿ�����һ��ʱ��ΪTRUE.
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
                //SCI_FREE(contact_ptr);//û��Ҫ��free��������ʱ��������׷������
                SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsHandler MMIAPIPB_AddContact fail");//return MMIPB_ERROR_ERROR;����ʧ�ܲ�������return
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
            SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllAddedContactsHandler find same o365id=%s", contact_ptr->o365_id);//return MMIPB_ERROR_ERROR;����ʧ�ܲ�������return
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
*  Description :    �����ѯ���ص�Office365ĳ��ʱ���Ժ��޸ĵ���ϵ�ˣ���ת����phone֧�ֵ���ϵ�˸�ʽ��
*   (1)contact_ptr  ����ת����phone֧�ָ�ʽ��Office365��ϵ����Ϣָ��
*   (2)chunk_end_flag   ��ʾ��ǰ��ѯ�����ݿ���������Ը������ж��Ƿ���Ҫ������ѯ���µ���ϵ����Ϣ������ǣ�������ɱ�����Ϣ���������server�ṩ��MMIAPIPBSYNC_SVR_GetAllContacts()�ӿڽ��в�ѯ��
*       chunk_end_flag    һ����ڴ������ݿ��е�10��ʱ��ΪTRUE.
*   (3)all_end_flag ��ʾ������ϵ����Ϣ���Ѳ�ѯ��ϣ�PB�˿��Ը��������ʶ������һ���Ĳ��������¼����ɵ�һ��ͬ����
*       all_end_flag   һ�����ڴ������һ�����ݿ�����һ��ʱ��ΪTRUE.
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
                    //SCI_FREE(contact_ptr);//û��Ҫ��free��������ʱ��������׷������
                    SCI_TRACE_LOW("[PBSYNC] MMIAPIPBSYNC_PB_GetAllContactsHandler MMIPB_ModifyContact fail");//return MMIPB_ERROR_ERROR;����ʧ�ܲ�������return
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
//  Description : �����Office365��ѯ����һ����ϵ�˵�contactId
//  (1) o365_id : Office365��ϵ��Idָ��
//  (2) uiTime �޸�ʱ��
//  (3) chunk_end_flag : ��ʾ��ǰ��ѯ�����ݿ���������Ը������ж��Ƿ���Ҫ������ѯ���µ���ϵ����Ϣ��id���޸�ʱ�䣩������ǣ�������ɱ�����Ϣ���������server�ṩ��MMIAPIPBSYNC_SVR_GetAllIdAndModiTime()�ӿڽ��в�ѯ��
//      chunk_end_flag һ����ڴ������ݿ��е�10��ʱ��ΪTRUE.
//  (4) all_end_flag ��ʾ������ϵ��id ���Ѳ�ѯ��ϣ�PB �˿��Ը��������ʶ������һ���Ĳ������翪ʼ��phont_update_list����ͻ��顣
//      all_end_flagһ�����ڴ������һ�����ݿ�����һ����Ϣʱ��ΪTRUE.
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
//  Description : �����ѯ���ص�Office365��ǰ��ϵ������
//  Global resource dependence : 
//  Note: total_count Office365��ǰ��ϵ������
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
//  Description : �����ѯ���ص�Office365ĳ��ʱ���Ժ������ϵ�˵�����
//  Global resource dependence : 
//  Note: total_count Office365ĳ��ʱ���Ժ������ϵ�˵�����
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
//  Description : �����ѯ���ص�Office365ĳ��ʱ���Ժ��޸���ϵ�˵�����
//  Global resource dependence : 
//  Note: total_count Office365ĳ��ʱ���Ժ������ϵ�˵�����
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
//  Description : �������һ����ϵ�˵�Office365֮�󷵻ص���ϵ����Ϣ��contactid���޸�ʱ�䣩
//  Global resource dependence : 
//  Note: o365_id Office365��ϵ��Idָ�룬uiTime uint32����Office365��ϵ������޸�ʱ��
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
//  Description : �����޸�Office365��һ����ϵ��֮�󷵻ص���ϵ����Ϣ��contactid���޸�ʱ�䣩
//  Global resource dependence : 
//  Note: o365_id Office365��ϵ��Idָ�룬uiTime uint32����Office365��ϵ������޸�ʱ��
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
//  Description : ����������Office365Ӧ���쳣ʱ��server���ô˽ӿ�
//  Global resource dependence : 
//  Note: uint32 error_code���������������ֱ�����������
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
