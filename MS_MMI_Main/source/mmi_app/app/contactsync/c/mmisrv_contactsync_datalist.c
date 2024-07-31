/****************************************************************************
** File Name:      mmisrv_contactsync_datalist.c                                        *
** Author:           shiwei.zhang                                                      *
** Date:           09/04/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 这个文件对HTTP GET 以外的请求数据进行管理.  
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _MMISRV_CONTACTSYNC_DATALIST_C_
#define _MMISRV_CONTACTSYNC_DATALIST_C_

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_common.h"
#include "mn_type.h"
#include "mmisrv_contactsync_datalist.h"

#define MMIPB_HTTP_MAX_REQUEST_NUM  10
#define MMIPB_HTTP_MAX_REQUEST_RETRY_TIMES  3

/**--------------------------------------------------------------------------*
**                                                                                                                              *
**--------------------------------------------------------------------------*/

LOCAL MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_INFO http_upload_request_list_info = {0};

LOCAL BOOLEAN request_index_info[MMIPB_HTTP_MAX_REQUEST_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE,
                                                                                                            FALSE, FALSE, FALSE, FALSE, FALSE};

//请求记录表访问信号量
LOCAL SCI_SEMAPHORE_PTR              s_http_upload_list_semphore_p = PNULL;

//请求索引表访问信号量
LOCAL SCI_SEMAPHORE_PTR              s_request_index_semphore_p = PNULL;


/****************************************************************************
**  Description : 初始化数据表
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/09/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_HTTP_Request_InitList(void)
{
    http_upload_request_list_info.plist_head = PNULL;
    http_upload_request_list_info.max_count = MMIPB_HTTP_MAX_REQUEST_NUM;
    http_upload_request_list_info.record_count = 0;
    http_upload_request_list_info.last_addnode = PNULL;
    s_http_upload_list_semphore_p = SCI_CreateSemaphore("s_http_upload_list_semphore_p", 1);
    s_request_index_semphore_p = SCI_CreateSemaphore("s_request_index_semphore_p", 1);
}

PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_GetAvailableIndex(uint16   *request_index)
{
    uint32 i = 0;
    for(i = 0; i < MMIPB_HTTP_MAX_REQUEST_NUM; i++)
    {
        if(FALSE == request_index_info[i])
        {
            SCI_GetSemaphore(s_request_index_semphore_p, SCI_WAIT_FOREVER);
            request_index_info[i] = TRUE;
            *request_index = i;
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_GetAvailableIndex %d.", *request_index);
            SCI_PutSemaphore(s_request_index_semphore_p);
            return MMIPB_ERROR_SUCCESS;
        }
    }
    SCI_TRACE_LOW("[MMIPBSYNC] !!! MMIPB_SYNC_HTTP_Request_GetAvailableIndex %d, but no such item.", *request_index);
    return MMIPB_ERROR_NO_SPACE;
}

PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_ReleaseIndex(uint16   request_index)
{
    if(TRUE == request_index_info[request_index])
    {
        SCI_GetSemaphore(s_request_index_semphore_p, SCI_WAIT_FOREVER);
        request_index_info[request_index] = FALSE;
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_ReleaseIndex %d.", request_index);
        SCI_PutSemaphore(s_request_index_semphore_p);
        return MMIPB_ERROR_SUCCESS;
    }
    else
    {
        SCI_TRACE_LOW("[MMIPBSYNC] !!! MMIPB_SYNC_HTTP_Request_ReleaseIndex %d error.", request_index);
        return MMIPB_ERROR_ERROR;
    }
}

/********************************************************************************
** Description: This function is used to check http request list is full                         *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_HTTP_Request_List_IsFull(void)
{
    if (http_upload_request_list_info.record_count == http_upload_request_list_info.max_count)
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_IsFull now is full.");
        return TRUE;
    }
    return FALSE;
}

/********************************************************************************
** Description: This function is used to check http request list is full                         *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_HTTP_Request_List_IsEmpty(void)
{
	if (0 == http_upload_request_list_info.record_count)
	{
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_IsEmpty now IS empty.");
            return TRUE;
	}
       SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_IsEmpty NOT empty.");
	return FALSE;
}

/********************************************************************************
** Description: This function is used to write response code in http request list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_Set_ResponseCode(HTTP_APP_INSTANCE_T app_instance,//[IN]
                                                                uint32   response_code)//[IN]
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    while(PNULL != p)
    {
        if(app_instance == p->request_data.app_instance)
        {
            p->request_data.pbsync_http_param.http_request_response_code= response_code;
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Set_ResponseCode set app_instance %d by responseCode %d.", app_instance, response_code);
            return MMIPB_ERROR_SUCCESS;
        }
        p = p->next;
    }
    //context ids is full.
    SCI_TRACE_LOW("[MMIPBSYNC] error!!! MMIPB_SYNC_HTTP_Request_List_Set_ResponseCode by app_instance %d ,bug no such item.", app_instance);
    return MMIPB_ERROR_NO_SPACE;
}

/********************************************************************************
** Description: This function is used to insert element in http request list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_Decrease_RetryTimes(HTTP_APP_INSTANCE_T app_instance)
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    while(PNULL != p)
    {
        if(app_instance == p->request_data.app_instance)
        {
            if (p->request_data.pbsync_http_param.http_request_fail_retry_time > 0)//bug482257
            {
                p->request_data.pbsync_http_param.http_request_fail_retry_time--;
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Set_RetryTimes set app_instance %d by retryTimes %d.", app_instance, p->request_data.pbsync_http_param.http_request_fail_retry_time);
            }
            return MMIPB_ERROR_SUCCESS;
        }
        p = p->next;
    }
    //context ids is full.
    SCI_TRACE_LOW("[MMIPBSYNC] error!!! MMIPB_SYNC_HTTP_Request_List_Set_RetryTimes by app_instance %d ,bug no such item.", app_instance);
    return MMIPB_ERROR_NO_SPACE;
}

/********************************************************************************
** Description: This function is used to get element in http request list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_Get_RetryTimes(HTTP_APP_INSTANCE_T app_instance, 
                                                                uint32   *retry_times)//[OUT]
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    while(PNULL != p)
    {
        if(app_instance == p->request_data.app_instance)
        {
            if (p->request_data.pbsync_http_param.http_request_fail_retry_time > 0)
            {
                *retry_times = p->request_data.pbsync_http_param.http_request_fail_retry_time--;
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Get_RetryTimes set app_instance %d by retryTimes %d.", app_instance, *retry_times);
            }
            return MMIPB_ERROR_SUCCESS;
        }
        p = p->next;
    }
    //context ids is full.
    SCI_TRACE_LOW("[MMIPBSYNC] error!!! MMIPB_SYNC_HTTP_Request_List_Get_RetryTimes by app_instance %d ,bug no such item.", app_instance);
    return MMIPB_ERROR_NO_SPACE;
}

/********************************************************************************
** Description: This function is used to insert element in http request list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_Get_ResponseCode(HTTP_APP_INSTANCE_T app_instance,//[IN]
                                                                uint32   *response_code)//[OUT]
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    while(PNULL != p)
    {
        if(app_instance == p->request_data.app_instance)
        {
            *response_code = p->request_data.pbsync_http_param.http_request_response_code;
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Get_ResponseCode get response_code %d by app_instance %d .", *response_code, app_instance);
            return MMIPB_ERROR_SUCCESS;
        }
        p = p->next;
    }
    //context ids is full.
    SCI_TRACE_LOW("[MMIPBSYNC] error!!! MMIPB_SYNC_HTTP_Request_List_Get_ResponseCode by app_instance %d ,bug no such item.", app_instance);
    return MMIPB_ERROR_NO_SPACE;
}

/********************************************************************************
** Description: This function is used to insert element in http request list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_Get_RequestId(HTTP_APP_INSTANCE_T app_instance,//[IN]
                                                                uint32   *request_id)//[OUT]
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    while(PNULL != p)
    {
        if(app_instance == p->request_data.app_instance)
        {
            *request_id = p->request_data.request_id;
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Get_RequestId get requestid %d by app_instance %d .", *request_id, app_instance);
            return MMIPB_ERROR_SUCCESS;
        }
        p = p->next;
    }
    //context ids is full.
    SCI_TRACE_LOW("[MMIPBSYNC] error!!! MMIPB_SYNC_HTTP_Request_List_Get_RequestId by app_instance %d ,bug no such item.", app_instance);
    return MMIPB_ERROR_NO_SPACE;
}

/********************************************************************************
** Description: This function is used to insert element in http request list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC HTTP_APP_INSTANCE_T MMIPB_SYNC_HTTP_Request_List_Get_InstancePart(HTTP_APP_INSTANCE_T app_instance)
{
    return (app_instance>>16);
}

/********************************************************************************
** Description: This function is used to insert element in http request list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_Append_Item(uint32   request_id,
                                                                HTTP_APP_INSTANCE_T        app_instance,
                                                                PBSYNC_HTTP_REQUEST_PARAM_T *requestPbData)
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = NULL;
    uint16   request_index = 0;
    if (PNULL == requestPbData)
    {
        return MMIPB_ERROR_ERROR;
    }

    if (http_upload_request_list_info.record_count >= http_upload_request_list_info.max_count)
    {
        return MMIPB_ERROR_ERROR;//list is full
    }

    if (MMIPB_ERROR_SUCCESS != MMIPB_SYNC_HTTP_Request_GetAvailableIndex(&request_index))
    {
        return MMIPB_ERROR_ERROR;//list is full
    }

    p =  (MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T*)SCI_ALLOCA(sizeof(MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T));
    SCI_MEMSET(p, 0x00, sizeof(MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T)); 
    SCI_MEMCPY(&p->request_data.pbsync_http_param, requestPbData, sizeof(p->request_data.pbsync_http_param));
    p->request_data.request_id = request_id;
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Append_Item SCI_GetSemaphore +++.");
    SCI_GetSemaphore(s_http_upload_list_semphore_p, SCI_WAIT_FOREVER);
    http_upload_request_list_info.record_count++;
    p->request_data.app_instance = (app_instance<<16) + (0xffff&request_index);
    p->isSent = FALSE;
    p->request_data.pbsync_http_param.http_request_fail_retry_time = MMIPB_HTTP_MAX_REQUEST_RETRY_TIMES;
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Append_Item Add app_instance = %d  record.", p->request_data.app_instance);

    if (PNULL == http_upload_request_list_info.plist_head)
    {
        
        http_upload_request_list_info.plist_head = p;
        http_upload_request_list_info.last_addnode = p;
    }
    else
    {
        http_upload_request_list_info.last_addnode->next = p;
        http_upload_request_list_info.last_addnode = http_upload_request_list_info.last_addnode->next;
    }
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Append_Item SCI_PutSemaphore ---.");
    SCI_PutSemaphore(s_http_upload_list_semphore_p);
    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to delete element in http request list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/10/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance(HTTP_APP_INSTANCE_T app_instance)
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *q = PNULL;
    uint16   request_index = 0;

    if (0 == http_upload_request_list_info.record_count)
    {
        return MMIPB_ERROR_ERROR;//list is empty
    }
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance SCI_GetSemaphore +++.");
    SCI_GetSemaphore(s_http_upload_list_semphore_p, SCI_WAIT_FOREVER);
    if((PNULL != p) && (app_instance == p->request_data.app_instance))
    {
        if (FALSE == p->isSent)
        {
            SCI_TRACE_LOW("[MMIPBSYNC] error!!! MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance app_instance = %d  request have Not been sent.", p->request_data.app_instance);
        }
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance Delete app_instance = %d  record.", p->request_data.app_instance);
        http_upload_request_list_info.plist_head = p->next;//delete head node
        if (http_upload_request_list_info.last_addnode == p)//http_upload_request_list_info.last_addnode is plist_head
        {
            http_upload_request_list_info.last_addnode = PNULL;
        }
        SCI_FREE(p);
        http_upload_request_list_info.record_count--;
        p = PNULL;
    }
    else
    {
        q = p;
        if(PNULL != p)
        {
            p = p->next;
        }
        while(PNULL != p)
        {
            if(app_instance == p->request_data.app_instance)
            {
                if (FALSE == p->isSent)
                {
                    SCI_TRACE_LOW("[MMIPBSYNC] error!!! MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance app_instance = %d  request have Not been sent.", p->request_data.app_instance);
                }
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance Delete app_instance = %d  record.", p->request_data.app_instance);
                if (http_upload_request_list_info.last_addnode == p)//http_upload_request_list_info.last_addnode point to the last one
                {
                    http_upload_request_list_info.last_addnode = q;
                }
                q->next = p->next;
                SCI_FREE(p);
                p = PNULL;
                http_upload_request_list_info.record_count--;
                break;//if delete all same o365id don't break here, but needn't
            }
            else
            {
                q = p;
                p = p->next;
            }
        }
    }
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DeleteByAppInstance SCI_PutSemaphore ---.");
    SCI_PutSemaphore(s_http_upload_list_semphore_p);
    request_index = 0xffff&app_instance;
    if (MMIPB_ERROR_SUCCESS != MMIPB_SYNC_HTTP_Request_ReleaseIndex(request_index))
    {
        return MMIPB_ERROR_ERROR;//list is full
    }
    
    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to Deal a http request                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/10/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_DealOneRequest(MMIPB_SYNC_HTTP_UPLOAD_REQUEST_T **request_data_ptr)
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    while(PNULL != p)//or while(p)
    {
        if(FALSE == p->isSent)
        {
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DealOneRequest SCI_GetSemaphore +++.");
            SCI_GetSemaphore(s_http_upload_list_semphore_p, SCI_WAIT_FOREVER);
            *request_data_ptr = &p->request_data;
            p->isSent = TRUE;//set sent flag
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DealOneRequest SCI_PutSemaphore ---.");
            SCI_PutSemaphore(s_http_upload_list_semphore_p);
            return MMIPB_ERROR_SUCCESS;
        }
        p = p->next;
    }
    *request_data_ptr = PNULL;//no request in queue.
    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to Deal a http request                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/10/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_DealOneFailRequest(MMIPB_SYNC_HTTP_UPLOAD_REQUEST_T **request_data_ptr)
{
    MMIPB_ERROR_E error_code = MMIPB_ERROR_NO_ENTRY;
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    *request_data_ptr = PNULL;//no request in queue.
    while(PNULL != p)//or while(p)
    {
        if((FALSE == p->isSent)&&(p->request_data.pbsync_http_param.http_request_response_code > 0))
        {
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DealOneFailRequest SCI_GetSemaphore +++.");
            SCI_GetSemaphore(s_http_upload_list_semphore_p, SCI_WAIT_FOREVER);
            if (p->request_data.pbsync_http_param.http_request_fail_retry_time > 0)
            {
                p->isSent = TRUE;//set sent flag
                error_code = MMIPB_ERROR_SUCCESS;
            }
            else
            {
                //No retry times left. stop sync 
                error_code = MMIPB_ERROR_NO_ENTRY;
            }
            *request_data_ptr = &p->request_data;
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DealOneFailRequest SCI_PutSemaphore ---.");
            SCI_PutSemaphore(s_http_upload_list_semphore_p);
            break;
        }
        p = p->next;
    }
    return error_code;
}

/********************************************************************************
** Description: This function is used to Deal a http request                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 08/10/2015 for bug482257                                                                  *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_DealUnCompletedRequest(uint32   *remain_count)//[OUT]
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    uint32 i = 0;
    while(PNULL != p)//or while(p)
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DealUnCompletedRequest SCI_GetSemaphore +++.");
        SCI_GetSemaphore(s_http_upload_list_semphore_p, SCI_WAIT_FOREVER);
        if(TRUE == p->isSent)
        {
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DealUnCompletedRequest request %d is NOT sent .", p->request_data.app_instance);
        }
        p->isSent = FALSE;
        if(0 == p->request_data.pbsync_http_param.http_request_response_code)
        {
            p->request_data.pbsync_http_param.http_request_response_code = HTTP_ERROR_SEND;
        }

        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_DealUnCompletedRequest SCI_PutSemaphore ---.");
        SCI_PutSemaphore(s_http_upload_list_semphore_p);
        i++;
        p = p->next;
    }
    *remain_count = i;
    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to destroy the http request list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_HTTP_Request_List_Destroy()
{
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *p = http_upload_request_list_info.plist_head;
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *q = PNULL;
    uint32 i = 0;
    if (PNULL == http_upload_request_list_info.plist_head)
    {
        return MMIPB_ERROR_SUCCESS;
    }

    if (0 == http_upload_request_list_info.record_count)
    {
        return MMIPB_ERROR_SUCCESS;//list is empty
    }
    for (i = 0; i < MMIPB_HTTP_MAX_REQUEST_NUM; i++)
    {
        request_index_info[i] = FALSE;
    }
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Destroy SCI_GetSemaphore +++.");
    SCI_GetSemaphore(s_http_upload_list_semphore_p, SCI_WAIT_FOREVER);
    while(PNULL != p)//or while(p)
    {
        q = p->next;
        SCI_FREE(p);
        p = PNULL;
        http_upload_request_list_info.record_count--;
        p = q;
    }
    
    http_upload_request_list_info.plist_head = PNULL;
    http_upload_request_list_info.last_addnode = PNULL;
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_HTTP_Request_List_Destroy SCI_PutSemaphore ---.");
    SCI_PutSemaphore(s_http_upload_list_semphore_p);
    
    if (0 != http_upload_request_list_info.record_count)
    {
        return MMIPB_ERROR_ERROR;
    }
    return MMIPB_ERROR_SUCCESS;
}

#endif
#endif
