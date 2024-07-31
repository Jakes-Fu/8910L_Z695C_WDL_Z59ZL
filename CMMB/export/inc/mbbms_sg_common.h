/*****************************************************************************
** File Name:      mbbms_sg_common.h                                         *
** Author:                                                                   *
** Date:           26/06/2009                                                *
** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe STiMi ESG data              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/2         Hui.Zhang        Create                                    *
******************************************************************************/

#ifndef _MBBMS_SG_COMMON_H_
#define _MBBMS_SG_COMMON_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

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
#define SG_GLOBAL_PURCHASEITEMID_LEN    24
#define SG_GLOBAL_SERVICEID_LEN         19
#define SG_GLOBAL_CONTENTID_LEN         5

#define SG_MAX_URI_LEN                  32
#define SG_MAX_CONTENT_LEN              80
#define SG_MAX_LANGUAGE_CONTENT_LEN     20
#define SG_MAX_BASE64BIN_LEN            200

typedef void (*DATA_CLEAN_FUNC)(void *data);

// Return Note: 0: same; 1: data1>data2; -1: data1<data2
typedef int (*DATA_COMPARE_FUNC)(void *data1,void *data2); 

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
// ��������
typedef enum
{
    MBBMS_LANG_UNKNOWN = 0,
    MBBMS_LANG_ZHO,
    MBBMS_LANG_ENG
}MBBMS_LANGUAGE_E;

// ��������
typedef enum
{
    MBBMS_CURRENCY_UNKNOWN = 0,
    MBBMS_CURRENCY_CNY, // �����
    MBBMS_CURRENCY_HKD, // ��Ԫ
    MBBMS_CURRENCY_TWD, // ̨��
    MBBMS_CURRENCY_USD, // ��Ԫ
    MBBMS_CURRENCY_EUR, // ŷԪ
    MBBMS_CURRENCY_GBP, // Ӣ��
    MBBMS_CURRENCY_AUD, // ��Ԫ
    MBBMS_CURRENCY_JPY, // ��Ԫ
    MBBMS_CURRENCY_KRW  // ��Ԫ
}MBBMS_CURRENCY_E;

// ���ݷ���
typedef enum
{
    MBBMS_CONTENT_UNKNOWN = 0,
    MBBMS_CONTENT_TEXT,
    MBBMS_CONTENT_IMAGE,
    MBBMS_CONTENT_AUDIO,
    MBBMS_CONTENT_VIDEO,
    MBBMS_CONTENT_APPLICATION,
    MBBMS_CONTENT_MULTIPART
}MBBMS_CONTENT_E;

// ҵ�����
typedef enum
{
    MBBMS_SERVICE_TYPE_0 = 0,               // δ�涨
    MBBMS_SERVICE_TYPE_TV,                  // �������ӽ�Ŀ
    MBBMS_SERVICE_TYPE_AUDIO,               // ������Ƶ�㲥
    MBBMS_SERVICE_TYPE_CACHECAST = 4,       // Cachecast
    MBBMS_SERVICE_TYPE_FILE_DOWNLOAD,       // �ļ�����
    MBBMS_SERVICE_TYPE_SW_MAN,              // �������
    MBBMS_SERVICE_TYPE_INFORM,              // ͨ��
    MBBMS_SERVICE_TYPE_SG,                  // ҵ��ָ��
    MBBMS_SERVICE_TYPE_UE_CONFIG = 9,       // �ն�����
    // 10~127 reserved for future applications
    MBBMS_SERVICE_TYPE_VIDEO_CLIP = 128,    // �̸�ʽ��Ƶ��ĿƬ��
    MBBMS_SERVICE_TYPE_STREAMING_DATA       // ��ģʽ���ݷ���
    // 130~255 reserved for private use
}MBBMS_SERVICE_TYPE_E;

// Ԥ����Ƭ��;
typedef enum
{
    MBBMS_PREVIEW_0,                        // δ����
    MBBMS_PREVIEW_CHAN_SWITCH,              // Ƶ���л�
    MBBMS_PREVIEW_SG_BROWSE,                // ҵ��ָ�����
    MBBMS_PREVIEW_SERVICE_PREVIEW,          // Ƶ��Ԥ��
    MBBMS_PREVIEW_BARKER,                   // Barker
    MBBMS_PREVIEW_TOU,                      // Terms of Use
    // 6~127 reserved for future applications
    MBBMS_PREVIEW_BACKGROUND = 128,         // ����
    MBBMS_PREVIEW_PREVUE,                   // Ԥ��Ƭ
    MBBMS_PREVIEW_ADVERTISEMENT             // ���
    // 131~255 reserved for private use
}MBBMS_PREVIEW_DATA_USAGE_E;

// ��������
typedef enum
{
    MBBMS_PURCHASE_DATA_SUBSCRIPTION_CONTENT = 128,     // ��Ŀ����
    MBBMS_PURCHASE_DATA_SUBSCRIPTION_SERVICE = 129,     // Ƶ������
    MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM = 130,         // �ײͣ���������һ���İ���Ƶ����
    MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM_MULTI =131//�������ײ�
}MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E;

//�ײ�ַ���������
typedef enum
{
    MBBMS_ACCESS_BDSTYPE_IPDC,      //IPDC over DVB-H
    MBBMS_ACCESS_BDSTYPE_MBMS,      //3GPP MBMS
    MBBMS_ACCESS_BDSTYPE_BCMCS,     //3GPP2 BCMCS
    // 3~127 reserved for future applications
    MBBMS_ACCESS_BDSTYPE_CMMB       //CMMB
    // 129~255 reserved for private use
}MBBMS_ACCESS_BDSTYPE_E;

//��Կ����ϵͳ������
typedef enum
{
    MBBMS_ACCESS_KMSTYPE_DRM,       //oma-bcast-drm-pki ����OMA DRM PKI (Public KeyInfrastructure)
    MBBMS_ACCESS_KMSTYPE_UMBMS,     //oma-bcast-gba_u-mbms����GBA_U 3GPP MBMS SKI(Symmetric Key Infrastructure)
    MBBMS_ACCESS_KMSTYPE_MEMBMS,    //oma-bcast-gba_me-mbms���� GBA_ME 3GPP MBMS SKI i.e. GBA_ME can be used
    MBBMS_ACCESS_KMSTYPE_BCMCS      //oma-bcast-prov-bcmcs ���� provisioned 3GPP2 BCMCS SKI
}MBBMS_ACCESS_KMSTYPE_E;

//��Կ����ϵͳ�ı������
typedef enum
{
    MBBMS_ACCESS_PROTECTION_TYPE_CONTENT,   //�����ݱ��� (protection_after_reception in STKM = 0x00 or 0x01
    MBBMS_ACCESS_PROTECTION_TYPE_SERVICE,   //��ҵ�񱣻�(protection_after_reception in STKM = 0x03
    MBBMS_ACCESS_PROTECTION_TYPE_RECORD     //���ݱ������Լ�����¼�������طŵı���(protection_after_reception in STKM = 0x02
    // 3~127 reserved for future applications
    // 128~255 reserved for private use
}MBBMS_ACCESS_PROTECTION_TYPE_E;

//�ն˷���ҵ������֧�ֵļ��ܷ���
typedef enum
{
    MBBMS_ACCESS_ENCRYPTION_TYPE_IPSEC,
    MBBMS_ACCESS_ENCRYPTION_TYPE_SRTP,
    MBBMS_ACCESS_ENCRYPTION_TYPE_ISMACRYP,
    MBBMS_ACCESS_ENCRYPTION_TYPE_DCF
    // 4~255 reserved for future use
}MBBMS_ACCESS_ENCRYPTION_TYPE_E;

//���ʱ��������ݵ�ҵ����Կ��ʶ����
typedef enum
{
    MBBMS_ACCESS_PROTECTIONKEY_TYPE_ID      //ProtectionKeyID = Key Domain ID + SEK/PEK ID, ����Smartcard Profile
    // 1~127 reserved for future applications
    // 128~255 reserved for private use
}MBBMS_ACCESS_PROTECTIONKEY_TYPE_E;

typedef struct sg_node_tag
{
    void    *data;      // data is null in head node
    uint32  node_cnt;   //valid in head node
    struct sg_node_tag *next_ptr;
}SG_NODE_T, *SG_LIST_PTR_T;

typedef struct
{
    uint8   anyURI_arr[SG_MAX_URI_LEN];
    uint8   *anyURI_ptr;    // allocate if longer than SG_MAX_URI_LEN
    uint32  string_len;
}anyURI;

typedef struct
{
    uint8   string_arr[SG_MAX_CONTENT_LEN];
    uint8   *content_string_ptr;    // allocate if longer than SG_MAX_CONTENT_LEN
    uint32  string_len;
}contentString;

typedef struct
{
    uint8   string_arr[SG_MAX_LANGUAGE_CONTENT_LEN];
    uint8   *content_string_ptr;    // allocate if longer than SG_MAX_CONTENT_LEN
    uint32  string_len;
    uint8   lang_type;      // ��������, MBBMS_LANGUAGE_E
}languageString;

typedef struct
{
    uint8   data_arr[SG_MAX_BASE64BIN_LEN];
    uint8   *data_ptr;      // allocate if longer than SG_MAX_BASE64BIN_LEN
    uint32  data_len;
}base64Binary;

typedef struct
{
    uint32    years;
    uint32    months;
    uint32    days;
    uint32    hours;
    uint32    minutes;
    uint32    seconds;
}SG_DATETIME_T;

// Ԥ�����ݷ�Ƭ������
typedef struct
{
    anyURI  idRef;          // ��Ƭ��ʶ
    uint8   usage;          // ��Ƭ��;, MBBMS_PREVIEW_DATA_USAGE_E
}SG_FRAGMENT_PREVIEWDATA_REF_T;

//Ԥ��ͼ����Ϣ
typedef struct
{
    anyURI          PictureURI;     // ͼ���URL��ַ
    contentString   MIMEType;       // ͼ��ı�������
    base64Binary    PictureData;    // ͼ������
    uint8 *              file_name_ptr;
    uint16               file_name_len;
}SG_PREVIEW_PICTURE_T;

// ���ҵ�λ
typedef struct
{
    float           price;
    uint8           currency;   // ��������
}SG_MONETARY_PRICE_T;

// �۸���Ϣ
typedef struct
{
    uint8           subscriptionType;   // ��������, MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E
    uint8           period;
    SG_LIST_PTR_T   MonetaryPriceList;  // �ڵ�����: SG_MONETARY_PRICE_T
}SG_PRICE_INFO_T;

//ҵ����Կ��ʶ
typedef struct
{
    uint8           type;               // MBBMS_ACCESS_PROTECTIONKEY_TYPE_E
    base64Binary    KeyId;
}SG_PROTECTION_KEY_ID_T;

//��Կ����ϵͳ��ʶ
typedef struct
{
    uint8   kmsType;                    // ����, MBBMS_ACCESS_KMSTYPE_E
    uint8   protectionType;             // �������, MBBMS_ACCESS_PROTECTION_TYPE_E
    anyURI  PermissionIssuerURI;        // ����Ȩ�޵�ƽ̨�ĵ�ַ
    BOOLEAN PermissionIssuerURIType;    // PermissionsIssuerURI������
    SG_LIST_PTR_T ProtectionKeyIDList;  // ҵ����Կ��ʶ(�ڵ�����: SG_PROTECTION_KEY_ID_T)
}SG_KEY_MAN_SYS_T;

//CMMB�������
typedef struct
{
    uint8   frequencyNo;    // Ƶ�ʱ��
    uint16  serviceid;      // ҵ���ʶ
}SG_CMMB_PARA_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create link list head node
//  Global resource dependence : none
//  Author:  
//  Note: ��һ���ڵ�ΪHeaderָ��
/*****************************************************************************/
PUBLIC BOOLEAN STL_LinkListCreate(
                                  SG_LIST_PTR_T *link_list_head_ptr
                                  );

/*****************************************************************************/
//  Description : release link list
//  Global resource dependence : none
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN STL_LinkListClean(
                                 SG_LIST_PTR_T *link_list_head_ptr,
                                 DATA_CLEAN_FUNC clean_data
                                 );

/*****************************************************************************/
//  Description : release reference link list that node data is not alloc by link
//                list itself but refence from other place
//  Global resource dependence : none
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN STL_LinkListCleanExceptNodeData(
                                               SG_LIST_PTR_T *link_list_head_ptr
                                               );

/*****************************************************************************/
//  Description : insert node to the tail of link list
//  Global resource dependence : none
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN STL_LinkListAddTail(
                                   SG_LIST_PTR_T link_list_head,
                                   void *node_data_ptr
                                   );

/*****************************************************************************/
//  Description : get tail(the newest) node data of link list
//  Global resource dependence : none
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC void *STL_LinkListGetTailData(
                                     SG_LIST_PTR_T link_list_head
                                     );

/*****************************************************************************/
//  Description : get node data from link list by node index
//  Global resource dependence : none
//  Author:  
//  Note: index start from 0
/*****************************************************************************/
PUBLIC void *STL_LinkListGetNodeDataByIdx(
                                          SG_LIST_PTR_T link_list_head,
                                          uint32 index
                                          );

/*****************************************************************************/
//  Description : get node data count from link list
//  Global resource dependence : none
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC uint32 STL_LinkListGetNodeDataCount(
                                           SG_LIST_PTR_T link_list_head
                                           );

/*****************************************************************************/
//  Description : insert node to link list by compare func
//  Global resource dependence : none
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN STL_LinkListSortInsert(SG_LIST_PTR_T      link_list_head,
                                      void              *node_data_ptr,
                                      DATA_COMPARE_FUNC  compare_func);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
