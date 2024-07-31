/*****************************************************************************
** File Name:      mmisrv_contactsync_datalist.h                                         *
** Author:         shiwei.zhang                                                          *
** Date:           09/04/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 ����ļ���HTTP GET ������������ݽ��й���.  
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

//phone���¼�¼��ڵ�Ļ�����Ϣ
typedef struct
{
    HTTP_APP_INSTANCE_T        app_instance;
    uint32           request_id;
    PBSYNC_HTTP_REQUEST_PARAM_T pbsync_http_param;//�����������,��app task ͳһ�շ�http ����
}MMIPB_SYNC_HTTP_UPLOAD_REQUEST_T;

typedef struct _MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T_TAG
{
    BOOLEAN isSent;
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_T  request_data;
    struct _MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T_TAG *next;
}MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T;

typedef struct _MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_INFO_TAG
{
    uint16  max_count;//������
    uint16  record_count;//��ǰ��¼����
    MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T *last_addnode;//��ǰ��¼�ڵ�ָ��(β���)
    struct _MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_T_TAG *plist_head;
}MMIPB_SYNC_HTTP_UPLOAD_REQUEST_LIST_INFO;


/****************************************************************************
**  Description : ��ʼ�����ݱ�
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

