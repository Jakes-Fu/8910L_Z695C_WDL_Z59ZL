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
// 语言类型
typedef enum
{
    MBBMS_LANG_UNKNOWN = 0,
    MBBMS_LANG_ZHO,
    MBBMS_LANG_ENG
}MBBMS_LANGUAGE_E;

// 货币种类
typedef enum
{
    MBBMS_CURRENCY_UNKNOWN = 0,
    MBBMS_CURRENCY_CNY, // 人民币
    MBBMS_CURRENCY_HKD, // 港元
    MBBMS_CURRENCY_TWD, // 台币
    MBBMS_CURRENCY_USD, // 美元
    MBBMS_CURRENCY_EUR, // 欧元
    MBBMS_CURRENCY_GBP, // 英镑
    MBBMS_CURRENCY_AUD, // 澳元
    MBBMS_CURRENCY_JPY, // 日元
    MBBMS_CURRENCY_KRW  // 韩元
}MBBMS_CURRENCY_E;

// 内容分类
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

// 业务类别
typedef enum
{
    MBBMS_SERVICE_TYPE_0 = 0,               // 未规定
    MBBMS_SERVICE_TYPE_TV,                  // 基本电视节目
    MBBMS_SERVICE_TYPE_AUDIO,               // 基本音频广播
    MBBMS_SERVICE_TYPE_CACHECAST = 4,       // Cachecast
    MBBMS_SERVICE_TYPE_FILE_DOWNLOAD,       // 文件下载
    MBBMS_SERVICE_TYPE_SW_MAN,              // 软件管理
    MBBMS_SERVICE_TYPE_INFORM,              // 通告
    MBBMS_SERVICE_TYPE_SG,                  // 业务指南
    MBBMS_SERVICE_TYPE_UE_CONFIG = 9,       // 终端配置
    // 10~127 reserved for future applications
    MBBMS_SERVICE_TYPE_VIDEO_CLIP = 128,    // 短格式视频节目片断
    MBBMS_SERVICE_TYPE_STREAMING_DATA       // 流模式数据服务
    // 130~255 reserved for private use
}MBBMS_SERVICE_TYPE_E;

// 预览分片用途
typedef enum
{
    MBBMS_PREVIEW_0,                        // 未定义
    MBBMS_PREVIEW_CHAN_SWITCH,              // 频道切换
    MBBMS_PREVIEW_SG_BROWSE,                // 业务指南浏览
    MBBMS_PREVIEW_SERVICE_PREVIEW,          // 频道预览
    MBBMS_PREVIEW_BARKER,                   // Barker
    MBBMS_PREVIEW_TOU,                      // Terms of Use
    // 6~127 reserved for future applications
    MBBMS_PREVIEW_BACKGROUND = 128,         // 背景
    MBBMS_PREVIEW_PREVUE,                   // 预告片
    MBBMS_PREVIEW_ADVERTISEMENT             // 广告
    // 131~255 reserved for private use
}MBBMS_PREVIEW_DATA_USAGE_E;

// 订购类型
typedef enum
{
    MBBMS_PURCHASE_DATA_SUBSCRIPTION_CONTENT = 128,     // 节目按次
    MBBMS_PURCHASE_DATA_SUBSCRIPTION_SERVICE = 129,     // 频道包月
    MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM = 130,         // 套餐（包含多于一个的包月频道）
    MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM_MULTI =131//包多月套餐
}MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E;

//底层分发网络类型
typedef enum
{
    MBBMS_ACCESS_BDSTYPE_IPDC,      //IPDC over DVB-H
    MBBMS_ACCESS_BDSTYPE_MBMS,      //3GPP MBMS
    MBBMS_ACCESS_BDSTYPE_BCMCS,     //3GPP2 BCMCS
    // 3~127 reserved for future applications
    MBBMS_ACCESS_BDSTYPE_CMMB       //CMMB
    // 129~255 reserved for private use
}MBBMS_ACCESS_BDSTYPE_E;

//密钥管理系统的类型
typedef enum
{
    MBBMS_ACCESS_KMSTYPE_DRM,       //oma-bcast-drm-pki 表明OMA DRM PKI (Public KeyInfrastructure)
    MBBMS_ACCESS_KMSTYPE_UMBMS,     //oma-bcast-gba_u-mbms表明GBA_U 3GPP MBMS SKI(Symmetric Key Infrastructure)
    MBBMS_ACCESS_KMSTYPE_MEMBMS,    //oma-bcast-gba_me-mbms表明 GBA_ME 3GPP MBMS SKI i.e. GBA_ME can be used
    MBBMS_ACCESS_KMSTYPE_BCMCS      //oma-bcast-prov-bcmcs 表明 provisioned 3GPP2 BCMCS SKI
}MBBMS_ACCESS_KMSTYPE_E;

//密钥管理系统的保护类别
typedef enum
{
    MBBMS_ACCESS_PROTECTION_TYPE_CONTENT,   //仅内容保护 (protection_after_reception in STKM = 0x00 or 0x01
    MBBMS_ACCESS_PROTECTION_TYPE_SERVICE,   //仅业务保护(protection_after_reception in STKM = 0x03
    MBBMS_ACCESS_PROTECTION_TYPE_RECORD     //内容保护，以及对于录制内容重放的保护(protection_after_reception in STKM = 0x02
    // 3~127 reserved for future applications
    // 128~255 reserved for private use
}MBBMS_ACCESS_PROTECTION_TYPE_E;

//终端访问业务所需支持的加密方法
typedef enum
{
    MBBMS_ACCESS_ENCRYPTION_TYPE_IPSEC,
    MBBMS_ACCESS_ENCRYPTION_TYPE_SRTP,
    MBBMS_ACCESS_ENCRYPTION_TYPE_ISMACRYP,
    MBBMS_ACCESS_ENCRYPTION_TYPE_DCF
    // 4~255 reserved for future use
}MBBMS_ACCESS_ENCRYPTION_TYPE_E;

//访问被保护内容的业务密钥标识类型
typedef enum
{
    MBBMS_ACCESS_PROTECTIONKEY_TYPE_ID      //ProtectionKeyID = Key Domain ID + SEK/PEK ID, 用于Smartcard Profile
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
    uint8   lang_type;      // 语言类型, MBBMS_LANGUAGE_E
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

// 预览数据分片的索引
typedef struct
{
    anyURI  idRef;          // 分片标识
    uint8   usage;          // 分片用途, MBBMS_PREVIEW_DATA_USAGE_E
}SG_FRAGMENT_PREVIEWDATA_REF_T;

//预览图像信息
typedef struct
{
    anyURI          PictureURI;     // 图像的URL地址
    contentString   MIMEType;       // 图像的编码类型
    base64Binary    PictureData;    // 图像数据
    uint8 *              file_name_ptr;
    uint16               file_name_len;
}SG_PREVIEW_PICTURE_T;

// 货币单位
typedef struct
{
    float           price;
    uint8           currency;   // 货币种类
}SG_MONETARY_PRICE_T;

// 价格信息
typedef struct
{
    uint8           subscriptionType;   // 订购类型, MBBMS_PURCHASE_DATA_SUBSCRIPTION_TYPE_E
    uint8           period;
    SG_LIST_PTR_T   MonetaryPriceList;  // 节点类型: SG_MONETARY_PRICE_T
}SG_PRICE_INFO_T;

//业务密钥标识
typedef struct
{
    uint8           type;               // MBBMS_ACCESS_PROTECTIONKEY_TYPE_E
    base64Binary    KeyId;
}SG_PROTECTION_KEY_ID_T;

//密钥管理系统标识
typedef struct
{
    uint8   kmsType;                    // 类型, MBBMS_ACCESS_KMSTYPE_E
    uint8   protectionType;             // 保护类别, MBBMS_ACCESS_PROTECTION_TYPE_E
    anyURI  PermissionIssuerURI;        // 发布权限的平台的地址
    BOOLEAN PermissionIssuerURIType;    // PermissionsIssuerURI的类型
    SG_LIST_PTR_T ProtectionKeyIDList;  // 业务密钥标识(节点类型: SG_PROTECTION_KEY_ID_T)
}SG_KEY_MAN_SYS_T;

//CMMB网络参数
typedef struct
{
    uint8   frequencyNo;    // 频率编号
    uint16  serviceid;      // 业务标识
}SG_CMMB_PARA_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create link list head node
//  Global resource dependence : none
//  Author:  
//  Note: 第一个节点为Header指针
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
