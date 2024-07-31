/****************************************************************************
** File Name:      mmipb_sync_datalist.c                                        *
** Author:           shiwei.zhang                                                      *
** Date:           07/09/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 ?a?????t??¨¢a?¦Ì¨¨?¨ª?2?¨ºy?Y??DD1¨¹¨¤¨ª.  
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _MMIPB_SYNC_DATALIST_C_
#define _MMIPB_SYNC_DATALIST_C_

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mn_type.h"
#include "mmipb_datalist.h"
#include "mmipb_storage.h"
#include "mmipb_sync_datalist.h"


/**--------------------------------------------------------------------------*
**                                                                                                                              *
**--------------------------------------------------------------------------*/

LOCAL MMIPB_SYNC_PHONE_UPDATE_LIST_INFO phone_update_list_info = {0};

LOCAL MMIPB_SYNC_O365_UPDATE_LIST_INFO o365_update_list_info = {0};

LOCAL SCI_SEMAPHORE_PTR s_pb_sync_datalist_semphore_p = PNULL;

LOCAL MMIPB_SYNC_PHONE_UPDATE_LIST_T *phone_update_list_cursor = PNULL;

PUBLIC MMIPB_ERROR_E MMIPB_SYNC_O365_Update_List_Destroy();
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_O365_Update_List_DeleteByO365Id(char *o365id);
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_DeleteByO365Id(char *o365id);
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_Destroy();


/****************************************************************************
**  Description : 3?¨º??¡¥¨ºy?Y¡À¨ª
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/09/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_InitDataList(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_InitDataList create s_pb_sync_datalist_semphore_p");
	s_pb_sync_datalist_semphore_p = SCI_CreateSemaphore("s_pb_sync_datalist_semphore_p", 1);

    phone_update_list_info.plist_head = PNULL;
    phone_update_list_info.max_count = MMINV_MAX_PHONEBOOK_RECORDS*2;//bug482109 append patch as post.
    phone_update_list_info.record_count = 0;
    phone_update_list_info.last_addnode = PNULL;
#if 0
    if (PNULL != phone_update_list_info.plist_head)
    {
        SCI_TRACE_LOW("[PBSYNC] MMIPB_SYNC_InitDataList destroy former list");
        MMIPB_SYNC_Phone_Update_List_Destroy();
    }
    phone_update_list_info.plist_head = PNULL;
    phone_update_list_info.max_count = 500;
    phone_update_list_info.record_count = 0;
    phone_update_list_info.last_addnode = PNULL;
#endif
#if 0
  o365_update_list_info.plist_head = PNULL;
  o365_update_list_info.max_count = 500;
  o365_update_list_info.record_count = 0;
  o365_update_list_info.last_addnode = PNULL;
#endif
}

/********************************************************************************
** Description: This function is used to test            *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/10/2015                                                                     *
*********************************************************************************/
PUBLIC void Test_MMIPB_SYNC_PHONE_List(void)
{
    char stro365id[MMIPB_SYNC_O365_ID_LEN] = "AAAAAA";
    uint32 o365modiTime = 0x11111111;
    uint32 phomodiTime = 0x11111111;
    MMIPB_OPC_E modify_type = 0x1;
    //MMIPB_SYNC_InitDataList();
    phone_update_list_info.max_count = 2000;
    MMIPB_SYNC_Phone_Update_List_DeleteByO365Id("EEEEEE");
    MMIPB_SYNC_Phone_Update_List_DeleteByO365Id("FFFFFF");
    MMIPB_SYNC_Phone_Update_List_DeleteByO365Id("GGGGG");
    MMIPB_SYNC_Phone_Update_List_Append(&stro365id, o365modiTime, phomodiTime, modify_type);
    MMIPB_SYNC_Phone_Update_List_Append("BBBBBB",0x22222222, 0x22222222, 0x2);
    MMIPB_SYNC_Phone_Update_List_Append("CCCCCC",0x33333333, 0x33333333, 0x3);
    MMIPB_SYNC_Phone_Update_List_Append("DDDDDD",0x44444444, 0x44444444, 0x4);


    
    MMIPB_SYNC_Phone_Update_List_Append("EEEEEE",0x55555555, 0x55555555, 0x5);
    MMIPB_SYNC_Phone_Update_List_Append("FFFFFF",0x66666666, 0x66666666, 0x6);
    MMIPB_SYNC_Phone_Update_List_Append("GGGGG",0x77777777, 0x77777777, 0x7);
    
    MMIPB_SYNC_Phone_Update_List_DeleteByO365Id("DDDDDD");
    MMIPB_SYNC_Phone_Update_List_DeleteByO365Id("BBBBBB");
    MMIPB_SYNC_Phone_Update_List_DeleteByO365Id("CCCCCC");
    MMIPB_SYNC_Phone_Update_List_DeleteByO365Id("AAAAAA");

    
    MMIPB_SYNC_Phone_Update_List_Destroy();

}

/********************************************************************************
** Description: This function is used to test            *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/14/2015                                                                     *
*********************************************************************************/
PUBLIC void Test_MMIPB_SYNC_O365_List(void)
{
    char stro365id[MMIPB_SYNC_O365_ID_LEN] = "aaaaaaaa";
    uint32 o365modiTime = 0x11111111;
    uint32 phomodiTime = 0x11111111;
    MMIPB_OPC_E modify_type = 0x1;
    //MMIPB_SYNC_InitDataList();
    o365_update_list_info.max_count = 2000;
    
    MMIPB_SYNC_O365_Update_List_Append(&stro365id, o365modiTime, phomodiTime, modify_type);
    MMIPB_SYNC_O365_Update_List_Append("bbbbbbbb",0x22222222, 0x22222222, 0x2);
    MMIPB_SYNC_O365_Update_List_Append("cccccccc",0x33333333, 0x33333333, 0x3);
    MMIPB_SYNC_O365_Update_List_Append("dddddddd",0x44444444, 0x44444444, 0x4);

    MMIPB_SYNC_O365_Update_List_Append("eeeeeeee",0x55555555, 0x55555555, 0x5);
    MMIPB_SYNC_O365_Update_List_Append("ffffffff",0x66666666, 0x66666666, 0x6);
    MMIPB_SYNC_O365_Update_List_Append("gggggggg",0x77777777, 0x77777777, 0x7);
    
    MMIPB_SYNC_O365_Update_List_DeleteByO365Id("dddddddd");
    MMIPB_SYNC_O365_Update_List_DeleteByO365Id("bbbbbbbb");
    MMIPB_SYNC_O365_Update_List_DeleteByO365Id("cccccccc");
    MMIPB_SYNC_O365_Update_List_DeleteByO365Id("aaaaaaaa");
    MMIPB_SYNC_O365_Update_List_DeleteByO365Id("eeeeeeee");
    MMIPB_SYNC_O365_Update_List_DeleteByO365Id("ffffffff");
    MMIPB_SYNC_O365_Update_List_DeleteByO365Id("gggggggg");

    
    MMIPB_SYNC_O365_Update_List_Destroy();

}

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_SYNC_PHONE_UPDATE_LIST_T *MMIPB_SYNC_Get_Phone_Update_List_Head(void)
{
	MMIPB_SYNC_PHONE_UPDATE_LIST_T * head = PNULL;

	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

	head = phone_update_list_info.plist_head;

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

    return head;
}

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_Is_Phone_Update_List_Empty(void)
{
	BOOLEAN bRet = FALSE;

	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

    if (PNULL == phone_update_list_info.plist_head)
    {
    	bRet = TRUE;
    }
    else if (0 == phone_update_list_info.record_count)
    {
    	bRet = TRUE;
    }

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

	return bRet;
}

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_Is_Phone_Update_List_Full(void)
{
	BOOLEAN bRet = FALSE;
	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

    if (phone_update_list_info.record_count >= phone_update_list_info.max_count)
    {
    	bRet = TRUE;
    }

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

	return bRet;
}

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_SYNC_PHONE_UPDATE_LIST_T *MMIPB_SYNC_Phone_Update_List_Cursor_Goto_Head(void)
{
	MMIPB_SYNC_PHONE_UPDATE_LIST_T * ptr = PNULL;
	
	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

	phone_update_list_cursor = phone_update_list_info.plist_head;
	ptr = phone_update_list_cursor;

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

    return ptr;
}

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_SYNC_PHONE_UPDATE_LIST_T *MMIPB_SYNC_Phone_Update_List_Cursor_Goto_Next(void)
{
	MMIPB_SYNC_PHONE_UPDATE_LIST_T * ptr = PNULL;
	
	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

	if (phone_update_list_cursor)
	{
		phone_update_list_cursor = phone_update_list_cursor->next;
	}

	ptr = phone_update_list_cursor;

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

    return ptr;
}

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_SYNC_PHONE_UPDATE_LIST_T *MMIPB_SYNC_Phone_Update_List_Get_Cursor(void)
{
	MMIPB_SYNC_PHONE_UPDATE_LIST_T * ptr = PNULL;
	
	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

	ptr = phone_update_list_cursor;

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

    return ptr;
}

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_Phone_Update_List_Is_Cursor_Tail(void)
{
	BOOLEAN ret = FALSE;
	
	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

	if (PNULL == phone_update_list_cursor) 
	{
		ret = TRUE;
	}
	else if (PNULL == phone_update_list_cursor->next)
	{
		ret = TRUE;
	}

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

    return TRUE;
}

/********************************************************************************
** Description: This function is used to insert element(deleted and modified) in phone update list  list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: o365_id, o365_modifiedTime, phone_modifiedTime, modify_type   *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_Append(char *o365id, 
                                                                uint32 o365_mdTime,
                                                                uint32 phone_mdTime,
                                                                MMIPB_OPC_E opc)
{
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *p = NULL;
    if ((PNULL == o365id) || (0 == phone_mdTime))//o365_mdTime can be 0
    {
        return MMIPB_ERROR_ERROR;
    }

    if (MMIPB_SYNC_Is_Phone_Update_List_Full())
    {
        return MMIPB_ERROR_ERROR;//list is full
    }

	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);
	
    p =  (MMIPB_SYNC_PHONE_UPDATE_LIST_T*)SCI_ALLOCA(sizeof(MMIPB_SYNC_PHONE_UPDATE_LIST_T));
    SCI_MEMSET(p, 0x00, sizeof(MMIPB_SYNC_PHONE_UPDATE_LIST_T)); 
    SCI_MEMCPY(p->pb_data_change.o365_id, o365id, sizeof(p->pb_data_change.o365_id));
    p->pb_data_change.o365_modifiedTime = o365_mdTime;
    p->pb_data_change.phone_modifiedTime = phone_mdTime;
    p->pb_data_change.modify_type = opc;

    if (PNULL == phone_update_list_info.plist_head)
    {
        
        phone_update_list_info.plist_head = p;
        phone_update_list_info.last_addnode = p;
    }
    else
    {
        phone_update_list_info.last_addnode->next = p;
        phone_update_list_info.last_addnode = phone_update_list_info.last_addnode->next;
    }
    phone_update_list_info.record_count++;

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);
	
    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to insert element  in phone update list  list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/21/2015 did not test                                                                    *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_Append_With_Info(MMIPB_PHONE_UPDATE_INFO_T * p_update_info)
{
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *p = NULL;
    if (NULL == p_update_info)
    {
        return MMIPB_ERROR_ERROR;
    }

    if (MMIPB_SYNC_Is_Phone_Update_List_Full())
    {
        return MMIPB_ERROR_ERROR;//list is full
    }

	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

    p =  (MMIPB_SYNC_PHONE_UPDATE_LIST_T*)SCI_ALLOCA(sizeof(MMIPB_SYNC_PHONE_UPDATE_LIST_T));
    SCI_MEMSET(p, 0x00, sizeof(MMIPB_SYNC_PHONE_UPDATE_LIST_T));
	SCI_MEMCPY(&p->pb_data_change, p_update_info, sizeof(MMIPB_PHONE_UPDATE_INFO_T));

    if (PNULL == phone_update_list_info.plist_head)
    {
        
        phone_update_list_info.plist_head = p;
        phone_update_list_info.last_addnode = p;
    }
    else
    {
        phone_update_list_info.last_addnode->next = p;
        phone_update_list_info.last_addnode = phone_update_list_info.last_addnode->next;
    }
    phone_update_list_info.record_count++;

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to insert element (added) in phone update list  list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/21/2015 did not test                                                                    *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_Append_A(uint16 contactId, 
                                                                uint16 storageId,
                                                                uint32 phone_mdTime)
{
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *p = NULL;

	SCI_TRACE_LOW("+[MMIPBSYNC]MMIPB_SYNC_Phone_Update_List_Append_A");

    if (0 == phone_mdTime)
    {
    	SCI_TRACE_LOW("-[MMIPBSYNC]MMIPB_SYNC_Phone_Update_List_Append_A, zero time!");
        return MMIPB_ERROR_ERROR;
    }

    if (MMIPB_SYNC_Is_Phone_Update_List_Full())
    {
    	SCI_TRACE_LOW("-[MMIPBSYNC]MMIPB_SYNC_Phone_Update_List_Append_A, full!");
        return MMIPB_ERROR_ERROR;//list is full
    }

	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

    p =  (MMIPB_SYNC_PHONE_UPDATE_LIST_T*)SCI_ALLOCA(sizeof(MMIPB_SYNC_PHONE_UPDATE_LIST_T));
    SCI_MEMSET(p, 0x00, sizeof(MMIPB_SYNC_PHONE_UPDATE_LIST_T)); 
    p->pb_data_change.contact_id= contactId;
    p->pb_data_change.storage_id= storageId;
    p->pb_data_change.phone_modifiedTime = phone_mdTime;
    p->pb_data_change.modify_type = MMIPB_OPC_ADD;

    if (PNULL == phone_update_list_info.plist_head)
    {
        
        phone_update_list_info.plist_head = p;
        phone_update_list_info.last_addnode = p;
    }
    else
    {
        phone_update_list_info.last_addnode->next = p;
        phone_update_list_info.last_addnode = phone_update_list_info.last_addnode->next;
    }
    phone_update_list_info.record_count++;

	SCI_TRACE_LOW("[MMIPBSYNC] phone_update_list_cursor =0X%X", phone_update_list_cursor);
	
	if (PNULL == phone_update_list_cursor)
	{
		phone_update_list_cursor = p;
		SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Phone_Update_List_Append_A re-create case,phone_update_list_cursor =0X%X", phone_update_list_cursor);
	}

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

	SCI_TRACE_LOW("-[MMIPBSYNC]MMIPB_SYNC_Phone_Update_List_Append_A");
    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to delete element in phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/10/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_DeleteByO365Id(char *o365id)
{
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *p = PNULL;
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *q = PNULL;
	
    if (PNULL == o365id)
    {
        return MMIPB_ERROR_ERROR;
    }

    if (MMIPB_SYNC_Is_Phone_Update_List_Empty())
    {
        return MMIPB_ERROR_ERROR;//list is empty
    }

	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);
	
    p = phone_update_list_info.plist_head;

    if (0 == MMIAPICOM_StrCmp((uint8 *)o365id, SCI_STRLEN(o365id), (uint8 *)&p->pb_data_change.o365_id, SCI_STRLEN(p->pb_data_change.o365_id)))
    {
        phone_update_list_info.plist_head = p->next;//delete head node
        if (phone_update_list_info.last_addnode == p)//phone_update_list_info.last_addnode is plist_head
        {
            phone_update_list_info.last_addnode = PNULL;
        }
	    if (p == phone_update_list_cursor)
	   {
				phone_update_list_cursor = PNULL;
				SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Phone_Update_List_DeleteByO365Id remove case 0,phone_update_list_cursor =0X%X", phone_update_list_cursor);
	   }

        SCI_FREE(p);
        phone_update_list_info.record_count--;
        p = PNULL;
    }
    else
    {
        q = p;
        p = p->next;
        while(PNULL != p)
        {
            if (0 == MMIAPICOM_StrCmp((uint8 *)o365id, SCI_STRLEN(o365id), (uint8 *)&p->pb_data_change.o365_id, SCI_STRLEN(p->pb_data_change.o365_id)))
            {
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Phone_Update_List_DeleteByO365Id Delete  %d  record.", p->pb_data_change.modify_type);
                if (phone_update_list_info.last_addnode == p)//phone_update_list_info.last_addnode point to the last one
                {
                    phone_update_list_info.last_addnode = q;
                }
                q->next = p->next;
				if (p == phone_update_list_cursor)
			 {
					   phone_update_list_cursor = p->next;
					   SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Phone_Update_List_DeleteByO365Id remove case 1,phone_update_list_cursor =0X%X", phone_update_list_cursor);
			 }                                    

                SCI_FREE(p);
                p = PNULL;
                phone_update_list_info.record_count--;
                break;//if delete all same o365id don't break here, but needn't
            }
            else
            {
                q = p;
                p = p->next;
            }
        }
    }

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);
	
    return MMIPB_ERROR_SUCCESS;
}

/******************************************************************************************************
** Description: Find the record by o365id and return modifiedtime comparison result, remove the record from the list if necessary*
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id, o365 modified time
** Return: 
** TRUE, o365 modified time is late, delete the corresponding record from phone update list; the update change to phone.
** FALSE, phone modifed time is late or it isn't be found; the caller need do nothing.
** Note: 07/22/2015                                                                     *
*****************************************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_Conflict_With_Phone_UpdateLst_Proc(uint32 o365mdTime, char *o365id)
{
    BOOLEAN bRet = TRUE;
	
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *p = PNULL;

    if (MMIPB_SYNC_Is_Phone_Update_List_Empty())
    {
        return TRUE;//o365 contact is significant
    }

    SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

    p = phone_update_list_info.plist_head;

    while(PNULL != p)//or while(p)
    {
        if(0 == MMIAPICOM_StrCmp((uint8 *)o365id, SCI_STRLEN(o365id), (uint8 *)&p->pb_data_change.o365_id, SCI_STRLEN(p->pb_data_change.o365_id)))
        {//find the record
        	if (MMIPB_OPC_DELETE == p->pb_data_change.modify_type)
    		{
    			/* phone delete and server update case ,wangkun*/
    			bRet = FALSE;
    		}
            else if (p->pb_data_change.phone_modifiedTime >= o365mdTime)//if eq or small, phone change is significant
            {
                bRet = FALSE;
            }
            else
            {
                bRet = TRUE;
            }			
            break;
        }
        p = p->next;
    }
    
    SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);

    return bRet;
}

/********************************************************************************
** Description: This function is used to destroy the phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_Destroy()
{
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *p = PNULL;
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *q = PNULL;
	MMIPB_ERROR_E bRet = MMIPB_ERROR_SUCCESS;
	
    if (MMIPB_SYNC_Is_Phone_Update_List_Empty())
    {
        return MMIPB_ERROR_SUCCESS;
    }

	SCI_GetSemaphore(s_pb_sync_datalist_semphore_p, SCI_WAIT_FOREVER);

    p = phone_update_list_info.plist_head;
	
    while(PNULL != p)//or while(p)
    {
        q = p->next;
        SCI_FREE(p);
        p = PNULL;
        phone_update_list_info.record_count--;
        p = q;
    }
    
    phone_update_list_info.plist_head = PNULL;
    phone_update_list_info.last_addnode = PNULL;

    if (0 != phone_update_list_info.record_count)
    {
    	bRet = MMIPB_ERROR_SUCCESS;
    }

	SCI_PutSemaphore(s_pb_sync_datalist_semphore_p);
	
    return bRet;
}

/********************************************************************************
** Description: This function is used to insert element in O365 update list  list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: o365_id, o365_modifiedTime, phone_modifiedTime, modify_type   *
** Note: 07/14/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_O365_Update_List_Append(char *o365id, 
                                                                uint32 o365_mdTime,
                                                                uint32 phone_mdTime,
                                                                MMIPB_OPC_E opc)
{
    MMIPB_SYNC_O365_UPDATE_LIST_T *p = NULL;
    if ((PNULL == o365id) || (0 == phone_mdTime))//o365_mdTime can be 0
    {
        return MMIPB_ERROR_ERROR;
    }

    if (o365_update_list_info.record_count >= o365_update_list_info.max_count)
    {
        return MMIPB_ERROR_ERROR;//list is full
    }

    p =  (MMIPB_SYNC_PHONE_UPDATE_LIST_T*)SCI_ALLOCA(sizeof(MMIPB_SYNC_PHONE_UPDATE_LIST_T));
    SCI_MEMSET(p, 0x00, sizeof(MMIPB_SYNC_PHONE_UPDATE_LIST_T)); 
    SCI_MEMCPY(p->pb_data_change.o365_id, o365id, sizeof(p->pb_data_change.o365_id));
    p->pb_data_change.o365_modifiedTime = o365_mdTime;
    p->pb_data_change.phone_modifiedTime = phone_mdTime;
    p->pb_data_change.modify_type = opc;

    if (PNULL == o365_update_list_info.plist_head)
    {
        
        o365_update_list_info.plist_head = p;
        o365_update_list_info.last_addnode = p;
    }
    else
    {
        o365_update_list_info.last_addnode->next = p;
        o365_update_list_info.last_addnode = o365_update_list_info.last_addnode->next;
    }
    o365_update_list_info.record_count++;
    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to delete element in O365 update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/14/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_O365_Update_List_DeleteByO365Id(char *o365id)
{
    MMIPB_SYNC_O365_UPDATE_LIST_T *p = o365_update_list_info.plist_head;
    MMIPB_SYNC_O365_UPDATE_LIST_T *q = PNULL;
    if (PNULL == o365id)
    {
        return MMIPB_ERROR_ERROR;
    }

    if (0 == o365_update_list_info.record_count)
    {
        return MMIPB_ERROR_ERROR;//list is empty
    }
    if (0 == MMIAPICOM_StrCmp((uint8 *)o365id, SCI_STRLEN(o365id), (uint8 *)&p->pb_data_change.o365_id, SCI_STRLEN(p->pb_data_change.o365_id)))
    {
        o365_update_list_info.plist_head = p->next;//delete head node
        if (o365_update_list_info.last_addnode == p)//o365_update_list_info.last_addnode is plist_head
        {
            o365_update_list_info.last_addnode = PNULL;
        }
        SCI_FREE(p);
        o365_update_list_info.record_count--;
        p = PNULL;
    }
    else
    {
        q = p;
        p = p->next;
        while(PNULL != p)
        {
            if (0 == MMIAPICOM_StrCmp((uint8 *)o365id, SCI_STRLEN(o365id), (uint8 *)&p->pb_data_change.o365_id, SCI_STRLEN(p->pb_data_change.o365_id)))
            {
                if (o365_update_list_info.last_addnode == p)//o365_update_list_info.last_addnode point to the last one
                {
                    o365_update_list_info.last_addnode = q;
                }
                q->next = p->next;
                SCI_FREE(p);
                p = PNULL;
                o365_update_list_info.record_count--;
                break;//if delete all same o365id don't break here, but needn't
            }
            else
            {
                q = p;
                p = p->next;
            }
        }
    }
    
    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to destroy the O365 update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/14/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_O365_Update_List_Destroy()
{
    MMIPB_SYNC_O365_UPDATE_LIST_T *p = o365_update_list_info.plist_head;
    MMIPB_SYNC_O365_UPDATE_LIST_T *q = PNULL;
    if (PNULL == o365_update_list_info.plist_head)
    {
        return MMIPB_ERROR_SUCCESS;
    }

    if (0 == o365_update_list_info.record_count)
    {
        return MMIPB_ERROR_SUCCESS;//list is empty
    }

    while(PNULL != p)//or while(p)
    {
        q = p->next;
        SCI_FREE(p);
        p = PNULL;
        o365_update_list_info.record_count--;
        p = q;
    }
    
    o365_update_list_info.plist_head = PNULL;
    o365_update_list_info.last_addnode = PNULL;
    
    if (0 != o365_update_list_info.record_count)
    {
        return MMIPB_ERROR_ERROR;
    }
    return MMIPB_ERROR_SUCCESS;
}

#endif
#endif
