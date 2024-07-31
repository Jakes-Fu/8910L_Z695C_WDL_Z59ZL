/*****************************************************************************
** File Name:      mmisrv_contactsync_datalist.h                                         *
** Author:         shiwei.zhang                                                          *
** Date:           09/04/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 这个文件对HTTP GET 以外的请求数据进行管理.  
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _MMISRV_CONTACTSYNC_DATALIST_H_
#define _MMISRV_CONTACTSYNC_DATALIST_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_custom_define.h"
#include "mmisrv_contactsync_http.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         phone contacts update info                                                                           *
 **--------------------------------------------------------------------------*/

//phone更新记录表节点的基本信息
typedef struct
{
    HTTP_APP_INSTANCE_T        app_instance;
    uint32           request_id;
    PBSYNC_HTTP_REQUEST_PARAM_T pbsync_http_param;//保存请求参数,由app task 统一收发http 数据
}MMIPB_SYNC_HTTP_UPLOAD_REQUEST_T;

typedef struct _MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T_TAG
{
    BOOLEAN isSent;
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_T  request_data;
    struct _MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T_TAG *next;
}MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T;

typedef struct _MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_INFO_TAG
{
    uint16  max_count;//最大个数
    uint16  record_count;//当前记录个数
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *last_addnode;//当前记录节点指针(尾结点)
    struct _MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T_TAG *plist_head;
}MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_INFO;


/****************************************************************************
**  Description : 初始化数据表
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/09/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_HTTP_Request_InitList(void);

/********************************************************************************
** Description: This function is used to check http request list is full                         *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_HTTP_Request_List_IsFull(void);

/********************************************************************************
** Description: This function is used to check http request list is full                         *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_HTTP_Request_List_IsEmpty(void);

#ifdef   __cplusplus
    }
#endif

#endif
#endif

