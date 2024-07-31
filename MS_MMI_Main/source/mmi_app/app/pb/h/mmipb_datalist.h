/*****************************************************************************
** File Name:      mmipb_datalist.h                                         *
** Author:                                                                   *
** Date:           11/09/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file deal with the contact list and qsort list       *
**                 ����ļ��Լ�¼���������ٲ��ҵĹ���.                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_DATALIST_H_
#define _MMIPB_DATALIST_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_custom_define.h"
#include "mmipb_common.h"
#include "mmipb_adapt.h"
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
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


#ifdef CMCC_VCARD_SUPPORT

typedef struct _MMIPB_BCD_NUMBER_LIST_T_TAG
{
    MMIPB_BCD_NUMBER_T number;
    uint8              num_type;
    struct _MMIPB_BCD_NUMBER_LIST_T_TAG *next_ptr;
}MMIPB_BCD_NUMBER_LIST_T;

typedef struct
{
    MMIPB_BCD_NUMBER_LIST_T *number_list;
    MMI_STRING_T mail_str;
}MMIPB_NUM_MAIL_LIST_T;

#endif
//������ڵ�Ļ�����Ϣ
typedef struct
{
    uint32 group;//
    uint16 index;//���������е�λ��
    uint16 contact_id;//��ϵ�˼�¼id
    uint16 storage_id;//�ýڵ��ڶ�Ӧ��ϵ�˱��е�λ�ã��ֻ���ϵ�˺�sim����ϵ�˻��л�����Ϣ��sns��ϵ��ֱ�Ӵ����ݿ��ж�ȡ��
    uint16 first_char;//����ĸ
    uint8  num_type[MMIPB_MAX_NV_PHONE_NUM];//ÿ����������ͣ�������ڶ����ͬ���͵ĺ���
    uint8  mail_exist;//�Ƿ����email
    uint8  visible_flag;//�ɼ��Ա�־��ÿһλ����һ������Ƿ�ɼ�            
    uint8  photo_id_flag;
}MMIPB_CONTACT_BASE_INFO_T;

//��ϵ��������ڵ�
typedef struct
{
    uint32 group;     //
    
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_NUM_MAIL_LIST_T *num_mail_list;
    BOOLEAN	is_read_ext;//��չ��Ϣ�Ƿ����
#endif
     uint16 index;   //�ýڵ��ڶ�Ӧ��ϵ�˱��е�λ�ã��ֻ���ϵ�˻��л�����Ϣ��sim����ϵ��ֱ�Ӵ�sim����ȡ,sns��ϵ��ֱ�Ӵ����ݿ��ж�ȡ��
    uint16 contact_id;//��ϵ�˼�¼id
#ifdef MMIPB_UID_SUPPORT
    uint32 uid;
#endif
    uint16 first_char;//����ĸ
    uint8  short_storage_id;//��ϵ�˼�¼�洢��id����4λ��PHONEBOOK_STORAGE_E����4λ��MMIPB_STORAGE_ID_E��
    uint8  visible_flag;//�ɼ��Ա�־��ÿһλ����һ������Ƿ�ɼ�
    uint8  photo_id_flag;
#ifdef MMIPB_SYNC_WITH_O365
    char  o365_id[MMIPB_SYNC_O365_ID_LEN];//Office365 �������ϵ��id 
    uint32  o365_modifiedTime;//Office365 ��ϵ�˵��޸�ʱ��
    uint32  phone_modifiedTime;// ��ϵ�˵��ֻ��޸�ʱ��
    BOOLEAN  checked;	// Office365���¼���־
#endif
}MMIPB_CONTACT_LIST_NODE_T;

#ifdef MMIPB_SYNC_WITH_O365
typedef enum _MMIPB_SYNC_TYPE_E
{
    MMIPB_SYNC_TYPE_SUSPEND,
    MMIPB_SYNC_TYPE_FIRST_TIME,
    MMIPB_SYNC_TYPE_LIGHT,
    MMIPB_SYNC_TYPE_HEAVY
} MMIPB_SYNC_TYPE_E;

//��ϵ��ͬ��ʱ����Ϣ
typedef struct
{
    uint32 last_light_sync_time;
    uint32 last_heavy_sync_time;
    uint32 cur_sync_time;
    uint16 last_total_count;
    uint16 cur_total_count;
    uint16 cur_added_count;
    uint16 cur_updated_count;	
}MMIPB_SYNC_TIME_INFO_T;
    
#endif

//��ϵ�������������Ϣ
typedef struct
{
    uint16  count; //ȫ����ϵ�˵ĸ���     
#ifdef TCARD_PB_SUPPORT
   uint16 tcontact_count;//t����ϵ�˸���
#endif
    
    uint16  mail_count;//ȫ������ĸ���
    uint16  number_count;//ȫ������ĸ���
    uint16  first_char_count;//����ĸ�ĸ���
    uint16	group_count[MMIPB_MAX_GROUP_NUM];//ÿ�������еļ�¼����
    uint16	storage_count[MMIPB_STORAGE_MAX];//ÿ���洢�豸�ļ�������
    uint16	group_number_count[MMIPB_MAX_GROUP_NUM]; //ÿ�������еĵ绰����ĸ���
    uint16	group_mail_count[MMIPB_MAX_GROUP_NUM];  //ÿ�������е�����ĸ���  
}MMIPB_CONTACT_LIST_INFO_T;

//��ϵ��������
typedef struct
{
    uint16  max_count;
    MMIPB_CONTACT_LIST_INFO_T list_info;
    MMIPB_CONTACT_LIST_NODE_T *list;
}MMIPB_CONTACT_LIST_T;
#ifdef SNS_SUPPORT
//sns��ϵ�˸�Ҫ��Ϣ
typedef struct
{
    uint32       group;   //����
    MMI_STRING_T name_str;//��ϵ������        
}MMIPB_SNS_CONTACT_NODE_T;

//��ϵ��������
typedef struct 
{
    uint16                    max_count;//sns��ϵ�˵�������
    uint16                    current_count;//sns��ϵ�˵�ǰ����
    MMIPB_SNS_CONTACT_NODE_T  *contact_table;//ÿ����ϵ�˵�������Ϣ��
}MMIPB_SNS_CONTACT_LIST_T;
#endif
//�ֻ���ϵ��������Ľڵ�ṹ
typedef struct
{
    //uint16             contact_id;//��ϵ�˼�¼id
    //uint16             storage_id;//��ϵ�˼�¼�洢��id����8λ��PHONEBOOK_STORAGE_E����8λ��MMIPB_STORAGE_ID_E��
    //uint32             group;//��ϵ��������group, (uint32��ÿһλ����һ����������)
    uint8              num_type[MMIPB_MAX_NV_PHONE_NUM];//ÿ����������ͣ�������ڶ����ͬ���͵ĺ���
    MMI_STRING_T       name_str;//��ϵ������
#ifdef MMI_PB_SURNAME_SUPPORT
    MMI_STRING_T       surname_str;//��ϵ����
#endif /*MMI_PB_SURNAME_SUPPORT*/
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint8 preferred_sim;// 0->none 1->sim1 2->sim2
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
    //uint8              mail_exist;//�Ƿ����email
    MMI_STRING_T       mail_str;//email
    MMIPB_BCD_NUMBER_T number[MMIPB_MAX_NV_PHONE_NUM];//��ϵ�˼�¼�����к��� 
}MMIPB_PHONE_CONTACT_NODE_T;


//��ϵ��������
typedef struct 
{
    uint16                    max_count;//��ϵ�˵�������
    uint16                    count_count;
    MMIPB_PHONE_CONTACT_NODE_T  *contact_table;//ÿ����ϵ�˵�������Ϣ��
}MMIPB_PHONE_CONTACT_LIST_T;

#ifdef MMIPB_MOST_USED_SUPPORT
//��ϵ��������Ľڵ�ṹ
typedef struct
{
    uint16             entry_id;//��ϵ�˼�¼id
    uint16             storage_id;//��ϵ�˼�¼�洢��id����8λ��PHONEBOOK_STORAGE_E����8λ��MMIPB_STORAGE_ID_E��
    uint32             used_freq;//ʹ��Ƶ��
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_BCD_NUMBER_T number;//ʹ�õĺ���
#endif
}MMIPB_MOSTUSED_CONTACT_NODE_T;

//������ϵ��������
typedef struct 
{
    uint16  count;//��ϵ�˸���  
    MMIPB_MOSTUSED_CONTACT_NODE_T  *contact_table;//ÿ����ϵ�˵�������Ϣ��
}MMIPB_MOSTUSED_CONTACT_LIST_T;
#endif

//���ٲ��
typedef struct
{
    uint16   max_count;//
    uint16	 count;//���¼����
    uint16   *list;//���ٲ��
}MMIPB_QSORT_LIST_T;

//qsort cmp data
typedef struct
{
    MMI_STRING_T *str_ptr;//�Ƚ��ַ���
    BOOLEAN           is_char;//�Ƿ�Ƚ��ַ���������
}MMIPB_QSORT_CMP_DATA_T;

//�����������email�Ļ���
typedef struct  
{
    MMIPB_NAME_T name;
#ifdef MMI_PB_SURNAME_SUPPORT
    MMIPB_NAME_T surname;
#endif /*MMI_PB_SURNAME_SUPPORT*/
    uint8 num_type[MMIPB_MAX_NV_PHONE_NUM];//��ϵ�˸������������
    MMIPB_BCD_NUMBER_T number_t[MMIPB_MAX_NV_PHONE_NUM];
    MMIPB_MAIL_T mail;
    uint32       group;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint8 preferred_sim;// 0->none 1->sim1 2->sim2
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
}MMIPB_NAME_NUMBER_MAIL_T;

//һ����ϵ����Ч�������������Чmail����
typedef struct
{
    uint8 visible_num_index[MMIPB_MAX_NV_PHONE_NUM];//�ɼ��ĺ������������, 1��ʾ�ɼ���0��ʾ���ɼ�
    BOOLEAN is_mail_visible;
}MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T;

// ��¼�ڴ���洢����Ӧ��entry_idʱ��λ����Ϣ
typedef struct _MMIPB_ENTRY_ID_FLAG_T
{
    uint16   array_pos;  // ������±ꡣ
    uint8   byte_pos;   // ��ʾ��BYTE�еĵڼ���BIT��
} MMIPB_ENTRY_ID_FLAG_T;

//contact id��ʹ�������valid_flag��ÿһλ��ʾһ��id��ʹ������� 1��ʾ��ռ�ã�0��ʾ����
//used conditon of contact id, each bits represents usage of each contact id, 1 means occupied, 0 means spared. 
typedef struct _MMIPB_ENTRY_ID_T
{
    uint16  max_record_num; //   ĳһ�洢���������Ա���ļ�¼����
    uint16  spare_id;                         // ��δ��ʹ�õ�entry_id
    uint8   *valid_flag;                      //contact id��ʹ�������ָ��һ��Ԫ��Ϊuint8������.��dual sim�еļ�¼id���ظ��ģ�
} MMIPB_ENTRY_ID_T;

#ifdef MMI_PB_MULTILANG_SORT_SUPPORT
typedef struct
{
    uint16 unicode;
    uint16 sort_num;
}MMIPB_MULTILANG_SORT_T;

typedef struct
{
    MMIPB_MULTILANG_SORT_T* array_ptr;
    uint16 array_len;
}MMIPB_MULTILANG_SORT_ARRAY_T;

typedef struct
{
    uint16 unicode;
    uint16 first_char;
}MMIPB_MULTILANG_FIRST_CHAR_T;

#define MMIPB_MAX_SORT_NUM 0xffff

#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
 /*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_ReleasePhoneContactListCache();

/*****************************************************************************/
//  Description : 
//  search_str: 
// 
/*****************************************************************************/
PUBLIC void MMIPB_InitDataList(void);

/*****************************************************************************/
//  Description : �������ݱ�ķ��ʿ����ź���
//  
// 
/*****************************************************************************/
PUBLIC void MMIPB_CreateDataListSemphore(void);

/*****************************************************************************/
//  Description : get a free contact
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFreeContactId(uint16  storage_id);

/*****************************************************************************/
//  Description : ��ȡNV contact id�ķ��������
//  Global resource dependence :  
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetContactIdUsedInfoOfNV(uint8 *valid_buf, 
					                        uint8 buf_len
					                        );

/*****************************************************************************/
//  Description : ��ȡsim contact id�ķ��������
//  Global resource dependence :  
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetContactIdUsedInfoOfSIM(
                                            MN_DUAL_SYS_E dual_sys,
                                            uint8 *valid_buf, 
                                            uint16 max_count
                                           );

/*****************************************************************************/
//  Description : ����������ռ�Ϳ��ٲ��ұ�ռ�
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_ALLOCContactList(void);

/*****************************************************************************/
//  Description : Delete one group's contacts from list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOneGroupContactList(uint32 group);

/*****************************************************************************/
//  Description : Find visiable contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindContact(
                            uint16 start_index,//��IN��   
                            uint32 group_mask,//[IN]����Ҫ���ҵ�Ⱥ�� 
                            uint16 storage_id, //Ҫ���ҵ���ϵ������                           
                            uint16 *find_index,//[OUT]
                            MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                            MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                            );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : Find visiable mostused contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindMostUsedContact(
                            uint16 start_index,
                            uint16 *find_index,//[OUT]                            
                            MMIPB_MOSTUSED_CONTACT_NODE_T *find_data_ptr//[OUT]
                            );
#endif
/*****************************************************************************/
//  Description : ����¼������Ļ�����Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactList(
                            MMIPB_CONTACT_LIST_INFO_T *data_ptr//[OUT]
                            );

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : check current dail number contact can be empty
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DailNumberContactCanbeEmpty(uint16 storage_id);
#endif

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : FDN��Ч��¼�ĸ���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFdnContactCount(uint16 storage_id);
#endif
/*****************************************************************************/
//  Description SIM���������ĸ���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetSIMDNContactCount(uint16 storage_id);

/*****************************************************************************/
//  Description : ��ȡ����ϵ�˵ļ���Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactSimpleInfo(
                            uint16 index, 
                            MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                            );

/*****************************************************************************/
//  Description : ��ȡ�ֻ���ϵ�˵ļ���Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadPhoneContactBaseInfo(
                                               uint16 index, 
                                               MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                                               );

/*****************************************************************************/
//  Description : ����һ���ڵ㵽��¼��
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertContactListNode(
                            MMIPB_CONTACT_T *contact_ptr//[IN]
                            );

/*****************************************************************************/
//  Description : �Ӽ�¼����ɾ��һ���ڵ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactListNode(
                            uint16 contact_id,
                            uint16 storage_id
                            );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : �ӳ�����ϵ����������ɾ��һ���ڵ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllMostUsedContactListNode(void);
#endif
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : ����ϵ����������ɾ������sns����
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllSNSContactListNode(void);
#endif

#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : ���ݼ�¼uid ������Ӧ��¼��entry_id��storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetcontactIdByUid(uint32 uid, uint16* entry_id_ptr, uint16* storage_id_ptr);

/*****************************************************************************/
//  Description : ���ݼ�¼uid ������Ӧ��¼��entry_id��storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetUidByContactId(uint16 entry_id, uint16 storage_id, uint32* uid);
#endif
/*****************************************************************************/
//  Description : ���ݼ�¼id��storage id��ȷ���ü�¼�������������
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactListIndex(
                            uint16 contact_id,
                            uint16 stoage_id
                            );

/*****************************************************************************/
//  Description : ����ϵ���������ж�ȡ��ϵ������
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: MMIPB_ERROR_SUCCESS:���ɹ�; ��������ֵ :����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetContactNameNumberByIndex(
                                                 MMIPB_NAME_NUMBER_MAIL_T *cmp_str,//[OUT]: �Ƚ��ַ���
                                                 uint16        pos     //IN: �ڴ洢���е�λ��
                                                 );

/*****************************************************************************/
//  Description : ��ȡ��Ӧ��ϵ�˵����������email��Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactNameNumberMail(
                                                         uint16 contact_id,
                                                         uint16 storage_id,
                                                         MMIPB_NAME_NUMBER_MAIL_T *string_ptr
                                                         );
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ���ݼ�¼id��storage id��ȷ���ü�¼�ڳ�����ϵ�������������
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMostUsedContactListIndex(
                            uint16 contact_id,
                            uint16 stoage_id
                            );
/*****************************************************************************/
//  Description : ��������ϵ�˼�¼������Ļ�����Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContactCount(
                            uint16  *count_ptr//[OUT]
                            );

/*****************************************************************************/
//  Description : ��������ϵ��������Ľڵ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContactListNode(
                            uint16 index, 
                            MMIPB_MOSTUSED_CONTACT_NODE_T *data_ptr//[OUT]
                            );

/*****************************************************************************/
//  Description : ����һ���ڵ㵽������ϵ��������
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertMostUsedContactListNode(
                            MMIPB_CONTACT_T *contact_ptr//[IN]
                            );

/****************************************************************************/
//  Description : �ӳ�����ϵ����������ɾ��һ���ڵ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteMostUsedContactListNode(
                                                         uint16 contact_id,
                                                         uint16 storage_id
                                                         );
#endif
/*****************************************************************************/
//  Description : �������ְ������ַ����ļ�¼
//  search_str: searching string
// group_id��Ҫ���ҵļ�¼���ڵ�group id;
// exclude_group:��Ҫ�ų���group���飬���Ҫ���ҵļ�¼�����group�����У�����ʾ 
/*****************************************************************************/
PUBLIC uint16 MMIPB_SearchSubStringInList(wchar *search_str, 
                                          MMIPB_SEARCH_TYPE_T search_type, 
                                          BOOLEAN                      is_char,
                                          uint32 group,
                                          uint32 exclude_group);

/*****************************************************************************/
//  Description : �������ְ������ַ����ļ�¼
//  search_str: searching string
/*****************************************************************************/
PUBLIC uint16 MMIPB_SearchSubStringInList2
(
 wchar *search_str, 
 MMIPB_SEARCH_TYPE_T search_type,
 BOOLEAN is_char,
 uint16  max_count
 );

/*****************************************************************************/
//  Description : 
//  search_str: email string
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetContactWithEmail
(
 wchar *search_str, //in
 uint16* entry_id_ptr, //out
 uint16* storage_id_ptr //out
 );

/*****************************************************************************/
//  Description : �����ϵ�˵Ŀɼ��Ա�
// 
/*****************************************************************************/
PUBLIC void MMIPB_CleanVisibleTable(uint32 group);

/*****************************************************************************/
//  Description : �����е���ϵ�˿ɼ�
// 
/*****************************************************************************/
PUBLIC void MMIPB_EnableAllContactVisible(void);

/*****************************************************************************/
//  Description : ����һ���ڵ㵽���ٲ��ұ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertQListNode(
                                           uint16       contact_index, //��¼�����������
                                           wchar       *wchar_ptr,
                                           uint16       wchar_len,
                                           wchar       *Key_wchar_ptr,
                                           uint16       Key_wchar_len
                                           );

/*****************************************************************************/
//  Description : �ӿ��ٲ��ұ���ɾ��һ���ڵ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteQListNode(
                            uint16 contact_index
                            );

/*****************************************************************************/
//  Description : �ڿ��ٲ��ұ��в���
// str_input���û����ַ���
// group��Ҫ���ҵļ�¼���ڵ�group;
// exclude_group:��Ҫ�ų���group���飬���Ҫ���ҵļ�¼�����group�����У�����ʾ
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SearchQSortList
(
     MMI_STRING_T *str_input,//IN
     uint32        group,
     uint32       exclude_group,
	 BOOLEAN is_char//TRUE:�ַ����ң�FALSE�����ֲ���
);

/*****************************************************************************/
//  Description : set mempool handle
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetMempoolHandle(MPOOL_HANDLE handle);

#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : set mempool handle
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetOnlySimMempoolHandle(MPOOL_HANDLE handle);
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ��g_mmipb_entry_list�в��ҵ�һ����û�ж�ȡ��չ��Ϣ�ļ�¼ID
//  Global resource
//  Author: 
//  Parameter: entry_id_ptr��[OUT] g_mmipb_entry_list�в��ҵ�һ����û�ж�ȡ��չ��Ϣ�ļ�¼ID
//             group_id_ptr��[OUT] g_mmipb_entry_list�в��ҵ�һ����û�ж�ȡ��չ��Ϣ�ļ�¼�ķ���ID
//  Note: 
//  Return: TURE: ����û�ж�ȡ��չ��Ϣ�ļ�¼��
//          FALSE: ������û�ж�ȡ��չ��Ϣ�ļ�¼��
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNextNotInitEntryID(
    uint16 *entry_id_ptr,
    uint16 *storage_id_ptr
    );

/*****************************************************************************/
//  Description : ���ڴ��ж�ȡUSIM ��չ��Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAddtionalFromContactList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           );

/*****************************************************************************/
//  Description : USIM ��չ��Ϣ���浽�������кͺ�����ұ���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveAddtionToList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           );


#endif
#ifdef MCARE_V31_SUPPORT
/*****************************************************************************/
//  Description :��ȡ��Ӧ��ĵ�ĳ������ֵ��ʼ�ĵ�һ��entry_id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetFirstIdWithIndex(uint32       group_id, uint32* m_index,int *entry_id);

#endif//MCARE_V31_SUPPORT

#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsTCardContactExist(void);

#endif

#ifdef MMIPB_SYNC_WITH_O365
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  RETURN: MMIPB_ERROR_SUCCESS:
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetContactOffice365IdByIndex
(
 uint16* contact_id,//out
 uint16* storage_id,//out
 char*  o365_id,//out
 uint16        pos     //IN: �ڴ洢���е�λ��
 );
 #endif /*MMIPB_SYNC_WITH_O365*/

#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitOnlySimDataList(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_AllocOnlySimcontactList(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_FreeOnlySimcontactList(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateOnlySimDataListSemphore(void);

/*****************************************************************************/
//  Description : ����һ���ڵ㵽���ٲ��ұ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertOnlySimQListNode(
                                           uint16       contact_index, //��¼�����������
                                           wchar       *wchar_ptr,
                                           uint16       wchar_len,
                                           wchar       *Key_wchar_ptr,
                                           uint16       Key_wchar_len
                                           );

/*****************************************************************************/
//  Description : ����һ���ڵ㵽��¼��
//  Global 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertOnlySimContactListNode(
                                                 MMIPB_CONTACT_T *contact_ptr//[IN]
                                                 );

/*****************************************************************************/
//  Description : �������ְ������ַ����ļ�¼
//  search_str: searching string
// group��Ҫ���ҵļ�¼���ڵ�group;
// exclude_group:��Ҫ�ų���group���飬���Ҫ���ҵļ�¼�����group�����У�����ʾ 
/*****************************************************************************/
PUBLIC uint16 MMIPB_SearchSubStringInOnlySimList(wchar *search_str, 
                                          MMIPB_SEARCH_TYPE_T search_type,
                                          BOOLEAN                      is_char,
                                          uint32 group,
                                          uint32 exclude_group);

/*****************************************************************************/
//  Description : ����¼������Ļ�����Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadOnlySimContactList(
                                           MMIPB_CONTACT_LIST_INFO_T *data_ptr//[OUT]
                                           );


/*****************************************************************************/
//  Description : Find visiable contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindOnlySimContact(
                                       uint16 start_index,  
                                       uint32 group_mask,//[IN]����Ҫ���ҵ�Ⱥ��
                                       uint16 storage_id, //Ҫ���ҵ���ϵ������                                        
                                       uint16 *find_index,//[OUT]
                                       MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                       MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                                       );

/*****************************************************************************/
//  Description : ��ȡ����ϵ�˵ļ���Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadOnlySimContactSimpleInfo(
                                               uint16 index, 
                                               MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                                               );

/*****************************************************************************/
//  Description : �ӿ��ٲ��ұ���ɾ��һ���ڵ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOnlySimQListNode(
                                           uint16 contact_index
                                           );

/*****************************************************************************/
//  Description : �Ӽ�¼����ɾ��һ���ڵ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOnlySimContactListNode(
                                                 uint16 contact_id,
                                                 uint16 storage_id
                                                 );

/*****************************************************************************/
//  Description : Delete one group's contacts from list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOneGroupOnlySimContactList(uint32 group);

/*****************************************************************************/
//  Description : ���ݼ�¼id��storage id��ȷ���ü�¼�������������
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetOnlySimContactListIndex(
                                        uint16 contact_id,
                                        uint16 stoage_id
                                        );

/*****************************************************************************/
//  Description : �ڿ��ٲ��ұ��в���
// str_input���û����ַ���
// group��Ҫ���ҵļ�¼���ڵ�group;
// exclude_group:��Ҫ�ų���group���飬���Ҫ���ҵļ�¼�����group�����У�����ʾ 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SearchOnlySimQSortList
(
 MMI_STRING_T *str_input,//IN
 uint32        group,
 uint32       exclude_group,
 BOOLEAN is_char//TRUE:�ַ����ң�FALSE�����ֲ���
 );

/*****************************************************************************/
//  Description : �����ϵ�˵Ŀɼ��Ա�
// 
/*****************************************************************************/
PUBLIC void MMIPB_CleanOnlySimVisibleTable(uint32 group);

/*****************************************************************************/
//  Description : �����е���ϵ�˿ɼ�
// 
/*****************************************************************************/
PUBLIC void MMIPB_EnableOnlySimAllContactVisible(void);

/*****************************************************************************/
//  Description : ����ϵ���������ж�ȡ��ϵ������
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: MMIPB_ERROR_SUCCESS:���ɹ�; ��������ֵ :����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetOnlySimContactNameNumberByIndex(
                                                 MMIPB_NAME_NUMBER_MAIL_T *cmp_str,//[OUT]: �Ƚ��ַ���
                                                 uint16        pos     //IN: �ڴ洢���е�λ��
                                                 );


#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ��g_mmipb_entry_list�в��ҵ�һ����û�ж�ȡ��չ��Ϣ�ļ�¼ID
//  Global resource
//  Author: 
//  Parameter: entry_id_ptr��[OUT] g_mmipb_entry_list�в��ҵ�һ����û�ж�ȡ��չ��Ϣ�ļ�¼ID
//             group_id_ptr��[OUT] g_mmipb_entry_list�в��ҵ�һ����û�ж�ȡ��չ��Ϣ�ļ�¼�ķ���ID
//  Note: 
//  Return: TURE: ����û�ж�ȡ��չ��Ϣ�ļ�¼��
//          FALSE: ������û�ж�ȡ��չ��Ϣ�ļ�¼��
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetOnlySimNextNotInitEntryID(
    uint16 *entry_id_ptr,
    uint16 *storage_id_ptr
    );

/*****************************************************************************/
//  Description : ���ڴ��ж�ȡUSIM ��չ��Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAddtionalFromOnlySimContactList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           );

/*****************************************************************************/
//  Description : USIM ��չ��Ϣ���浽�������кͺ�����ұ���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveAddtionToOnlySimList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           );
#endif /*CMCC_VCARD_SUPPORT*/

#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/

#ifdef MMIPB_SYNC_WITH_O365
#if 0
/*****************************************************************************/
//  Description : ���ݸ��ݼ�¼id��storage id����ȡO365Id
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : TRUE: find this record and o365id exist, FALSE: not find the record or no o365id in this record    
//  Note: 2015-7-8
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_GetO365IdByPhoneId(
                                                uint16 contact_id,
                                                uint16 stoage_id,
                                                char *O365Id//[OUT]
                                                );
#endif

PUBLIC void MMIPB_SYNC_HeavySyncConfirmed(void);
PUBLIC void MMIPB_SYNC_HeavySyncCanceled(void);

PUBLIC BOOLEAN MMIPB_SYNC_IsSyncing(void);

PUBLIC BOOLEAN MMIPB_SYNC_Start_FirstSync(void);
PUBLIC BOOLEAN MMIPB_SYNC_Start_LightSync(void);
PUBLIC BOOLEAN MMIPB_SYNC_Start_HeavySync(BOOLEAN isBackGround);

typedef void (* MMIPB_SYNC_STOP_CALLBACK_FUNC)(void); 

PUBLIC BOOLEAN MMIPB_SYNC_Stop_Sync(MMIPB_SYNC_STOP_CALLBACK_FUNC stop_call_back);
PUBLIC BOOLEAN MMIPB_SYNC_IsStopSync(void);
PUBLIC BOOLEAN MMIPB_SYNC_Remove_All_AccountData(void);
PUBLIC MMIPB_SYNC_TYPE_E MMIPB_SYNC_GetContactsSyncType(void);

#endif



#ifdef   __cplusplus
    }
#endif

#endif


