/*****************************************************************************
** File Name:      mmipb_sync_datalist.h                                         *
** Author:         shiwei.zhang                                                          *
** Date:           07/09/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 这个文件对联系人同步数据进行管理.  
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _MMIPB_SYNC_DATALIST_H_
#define _MMIPB_SYNC_DATALIST_H_

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

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         phone contacts update info                                                                           *
 **--------------------------------------------------------------------------*/

//phone更新记录表节点的基本信息
typedef struct
{
    uint16      contact_id; //保存phone contact id       
    uint16      storage_id;//保存phone storage id
    uint32        o365_modifiedTime;//Office365 联系人的修改时间,目前看来没有必要保存
    uint32        phone_modifiedTime;// 联系人的手机修改时间
    MMIPB_OPC_E        modify_type;
    char        o365_id[MMIPB_SYNC_O365_ID_LEN];//Office365 分配的联系人id , 
}MMIPB_PHONE_UPDATE_INFO_T;

typedef struct _MMIPB_SYNC_PHONE_UPDATE_LIST_T_TAG
{
    MMIPB_PHONE_UPDATE_INFO_T  pb_data_change;
    struct _MMIPB_SYNC_PHONE_UPDATE_LIST_T_TAG *next;
}MMIPB_SYNC_PHONE_UPDATE_LIST_T;

typedef struct _MMIPB_SYNC_PHONE_UPDATE_LIST_INFO_TAG
{
    uint16  max_count;//最大个数
    uint16  record_count;//当前记录个数
    MMIPB_SYNC_PHONE_UPDATE_LIST_T *last_addnode;//当前记录节点指针
    struct _MMIPB_SYNC_PHONE_UPDATE_LIST_T_TAG *plist_head;
}MMIPB_SYNC_PHONE_UPDATE_LIST_INFO;

/**--------------------------------------------------------------------------*
 **                         office365 contacts update info                                                                           *
 **--------------------------------------------------------------------------*/

//office365更新记录表节点的基本信息
typedef struct
{
    char        o365_id[MMIPB_SYNC_O365_ID_LEN];//Office365 分配的联系人id , modify_type为A时，保存phone contact id
    uint32        o365_modifiedTime;//Office365 联系人的修改时间
    uint32        phone_modifiedTime;// 联系人的手机修改时间
    MMIPB_OPC_E        modify_type; 
}MMIPB_O365_UPDATE_INFO_T;

typedef struct _MMIPB_SYNC_O365_UPDATE_LIST_T_TAG
{
    MMIPB_O365_UPDATE_INFO_T  pb_data_change;
    struct _MMIPB_SYNC_O365_UPDATE_LIST_T_TAG *next;
}MMIPB_SYNC_O365_UPDATE_LIST_T;

typedef struct _MMIPB_SYNC_O365_UPDATE_LIST_INFO_TAG
{
    uint16  max_count;//最大个数
    uint16  record_count;//当前记录个数
    MMIPB_SYNC_O365_UPDATE_LIST_T *last_addnode;//当前记录节点指针
    struct _MMIPB_SYNC_O365_UPDATE_LIST_T_TAG *plist_head;
}MMIPB_SYNC_O365_UPDATE_LIST_INFO;

/****************************************************************************
**  Description : 初始化数据表
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/09/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_InitDataList(void);

/********************************************************************************
** Description: This function is used to insert element in phone update list  list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: o365_id, o365_modifiedTime, phone_modifiedTime, modify_type   *
** Note: 07/09/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_Append(char *o365id, 
                                                                uint32 o365_mdTime,
                                                                uint32 phone_mdTime,
                                                                MMIPB_OPC_E opc);

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
                                                                MMIPB_OPC_E opc);

/********************************************************************************
** Description: This function is used to insert element in O365 update list  list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: o365_id, o365_modifiedTime, phone_modifiedTime, modify_type   *
** Note: 07/14/2015                                                                      *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_Append_With_Info(MMIPB_PHONE_UPDATE_INFO_T * p_update_info);


/********************************************************************************
** Description: This function is used to insert element (added) in phone update list  list                          *
** Global Resource Dependency:                                                 *
** Author: Shiwei.zhang                                                     *
** Input: *
** Note: 07/21/2015 did not test                                                                    *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Phone_Update_List_Append_A(uint16 contactId, 
                                                                uint16 storageId,
                                                                uint32 phone_mdTime);

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
PUBLIC BOOLEAN MMIPB_SYNC_Conflict_With_Phone_UpdateLst_Proc(uint32 o365mdTime, char *o365id);

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_SYNC_PHONE_UPDATE_LIST_T *MMIPB_SYNC_Get_Phone_Update_List_Head(void);


/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_Is_Phone_Update_List_Empty(void);


/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_Is_Phone_Update_List_Full(void);

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_SYNC_PHONE_UPDATE_LIST_T *MMIPB_SYNC_Phone_Update_List_Cursor_Goto_Head(void);

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_SYNC_PHONE_UPDATE_LIST_T *MMIPB_SYNC_Phone_Update_List_Cursor_Goto_Next(void);

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_SYNC_PHONE_UPDATE_LIST_T *MMIPB_SYNC_Phone_Update_List_Get_Cursor(void);

/********************************************************************************
** Description: This function is used to get the head pointer of phone update list                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_Phone_Update_List_Is_Cursor_Tail(void);

/********************************************************************************
** Description: This function is used to test            *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input: o365_id
** Note: 07/10/2015                                                                     *
*********************************************************************************/
PUBLIC void Test_MMIPB_SYNC_PHONE_List(void);
PUBLIC void Test_MMIPB_SYNC_O365_List(void);

#ifdef   __cplusplus
    }
#endif

#endif
#endif
