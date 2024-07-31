/*****************************************************************************
** File Name:      mbbms_sg_api.h                                            *
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

#ifndef _MBBMS_SG_API_H_
#define _MBBMS_SG_API_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mbbms_sg_common.h"
#include "os_api.h"

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
#define BIT0        (uint8)(0x01)
#define BIT1        (uint8)(0x01) << 1
#define BIT2        (uint8)(0x01) << 2
#define BIT3        (uint8)(0x01) << 3
#define BIT4        (uint8)(0x01) << 4
#define BIT5        (uint8)(0x01) << 5
#define BIT6        (uint8)(0x01) << 6
#define BIT7        (uint8)(0x01) << 7

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
        
//****************************************************************************
//
//  !!!!!!!   NOTE: SG_LIST_PTR 表示的指针都有Head节点    !!!!!!!!
//
//****************************************************************************

typedef enum
{
    SG_ERROR_NONE,
    SG_ERROR_INCOMPLETE_DATA,
    SG_ERROR_NO_DATA,
    SG_ERROR_PARSE_ABORT,
    SG_ERROR_IS_SEARCHING,    
    SG_ERROR_FAIL,
    SG_ERROR_NO_MEM
    
}SG_ERROR_E;

//the result of search SG
typedef enum
{
    MBBMS_SG_RECEIVE_FAIL,
    MBBMS_SG_RECEIVE_TIMEOUT,
    MBBMS_SG_RECEIVE_OK,
    MBBMS_SG_RECEIVE_OK_NOT_UPDATED,
    MBBMS_SG_STOP_SUCCESS,
    MBBMS_SG_STOP_FAIL,
    MBBMS_SG_RECEIVE_SIG_MAX
}MBBMS_SG_RECEIVE_SIG_TYPE_E;

//error type
typedef enum
{
    MBBMS_SG_RETURN_ERR = 0,//mbbms sg operation error   
    MBBMS_SG_HTTP_ERR,//http operation error
    MBBMS_SG_FILE_ERR,//file operation error
    MBBMS_SG_STATUS_ERR,//status from sg response
    MBBMS_SG_COMPARE_ERR,//fail to compare
    MBBMS_SG_COMPARE_SAME,//new sg is same as old 
    MBBMS_SG_NO_MEM,//new sg is same as old
    MBBMS_SG_NEED_UPDATE,   
    MBBMS_SG_ERR_MAX
}MBBMS_SG_ERR_TYPE_E;


typedef enum{
	
	QUERY_COMPLEX_INFO,
		QUERY_CONTENT_INDEX,
		QUERY_CONTENT,
		QUERY_CONTENT_ID,
		QUERY_GLOBAL_CONTENT_ID,
		QUERY_CONTENT_PREVIEW_DATA
		
		
}MBBMS_SG_QUERY_TYPE_E;

typedef enum{

   MBBMS_SG_GET_COMPLEX =0,
   MBBMS_SG_GET_DATA_MAX


}MBBMS_SGAPI_GET_DATA_TYPE_E;


typedef struct mbbms_sgapi_info_tag
{
    uint32 data_type;// MBBMS_SGAPI_GET_DATA_TYPE_E
    uint32 err_code;//SG_ERROR_E
    void * data_ptr;
    uint32 is_finish;

}MBBMS_SGAPI_INFO_T;
//infomation to mmi
typedef struct mbbms_sg_info_tag
{
         
    MBBMS_SG_RECEIVE_SIG_TYPE_E type;    //result type
    MBBMS_SG_ERR_TYPE_E err_type;//error type
    uint32 err_code;            //error code
}MBBMS_SG_INFO_T;

//signal structure
typedef struct mbbms_sg_sig_tag
{
    SIGNAL_VARS//system sig
  	MBBMS_SG_INFO_T info ;//infomation to mmi
}MBBMS_SG_SIG_T;

typedef struct
{
    uint32  type;//MBBMS_SG_ERR_TYPE_E	
    uint32  update_err;	
    uint16  err_code;

}SG_PARSE_RET_T;

typedef struct
{
    SIGNAL_VARS
    SG_PARSE_RET_T rt;
}SG_PARSE_SIG_T;


//signal structure
typedef struct sgapi_sig_tag
{
    SIGNAL_VARS//system sig
  	MBBMS_SGAPI_INFO_T info ;//infomation to mmi
  	
}MBBMS_SGAPI_GET_DATA_SIG_T;

typedef struct mbbms_sg_process_sig_tag
{
    SIGNAL_VARS//system sig

}MBBMS_SG_PROCESS_SIG_T;


// 综合数据查询类型
typedef enum
{
    MBBMS_CONTEXT_SEARCH_DATE      = BIT1,  // 日期
    MBBMS_CONTEXT_SEARCH_SERVICE   = BIT2,  // 频道
    MBBMS_CONTEXT_SEARCH_GENRE     = BIT3,  // 节目类型
    MBBMS_CONTEXT_SEARCH_KEYWORD   = BIT4   // 关键字
}MBBMS_CONTEXT_COMPLEX_SEARCH_E;

// 预览数据所属分片
typedef enum
{
    MBBMS_PREVIEWDATA_RANGE_PURCHASEITEM,
    MBBMS_PREVIEWDATA_RANGE_SERVICE,
    MBBMS_PREVIEWDATA_RANGE_CONTENT
}MBBMS_PREVIEWDATA_RANGE_E;

// 预览数据查找条件
typedef struct
{
    anyURI  index;      // 预览分片所属分片标识
    uint8   range;      // 预览分片所属分片, MBBMS_PREVIEWDATA_RANGE_E
    uint8   usage;      // 预览分片用途, MBBMS_PREVIEW_DATA_USAGE_E
}MBBMSSG_PREVIEWDATA_SEARCH_T;

// 套餐索引信息
typedef struct
{
    anyURI          index;      // 套餐分片标识
    SG_LIST_PTR_T   NameList;   // 套餐分片名称(节点类型: languageString)
}MBBMSSG_PURCHASEITEM_INDEX_T;

// 频道索引信息
typedef struct
{
    anyURI          index;      // 频道分片标识
    SG_LIST_PTR_T   NameList;   // 频道名称(节点类型: languageString)
    uint16          weight;     // 频道相对显示次序
    BOOLEAN         forfree;    // 免费指示(免费:TRUE, 收费:FALSE)
}MBBMSSG_SERVICE_INDEX_T;

// 节目索引信息
typedef struct
{
    anyURI          index;      // 节目分片标识
    SG_LIST_PTR_T   NameList;   // 节目名称(节点类型: languageString)
    SG_DATETIME_T   startTime;  // 节目启始时间
    SG_DATETIME_T   endTime;    // 节目结束时间
    BOOLEAN         forfree;    // 免费指示(免费:TRUE, 收费:FALSE)
    SG_LIST_PTR_T   GenreList;  // 频道的类别描述(节点类型: contentString)
}MBBMSSG_CONTENT_INDEX_T;

// 业务指南过滤器
typedef struct
{
    SG_DATETIME_T startTime;    // 节目启始时间
    SG_DATETIME_T endTime;      // 节目结束时间
}MBBMSSG_FILTER_T;

// 综合数据查询条件
typedef struct
{
    SG_DATETIME_T   startTime;          // 日期
    anyURI          serviceId;          // 频道分片标识
    contentString   genre;              // 节目类型
    languageString  keyword;            // 关键字
    uint8           condition_enable;   // 查询条件设置, MBBMS_CONTEXT_COMPLEX_SEARCH_E
}MBBMSSG_CONTEXT_COMPLEX_SEARCH_T;

// 综合数据查询结果
typedef struct
{

    //anyURI          serviceId;          // 频道分片标识
    //MBBMSSG_CONTENT_INDEX_T contextIdx; // 节目索引信息
    anyURI          *serviceId_ptr;          // 频道分片标识
    MBBMSSG_CONTENT_INDEX_T *contextIdx_ptr; // 节目索引信息
}MBBMSSG_CONTEXT_COMPLEX_DATA_T;

// 频道分片结构
typedef struct
{
    SG_LIST_PTR_T   servicetypeList;    // 业务类别(节点类型: uint8), MBBMS_SERVICE_TYPE_E
    SG_LIST_PTR_T   NameList;           // 频道名称(节点类型: languageString)
    SG_LIST_PTR_T   DescriptionList;    // 频道描述(节点类型: languageString)
    SG_LIST_PTR_T   GenreList;          // 频道的类别描述(节点类型: contentString)
    SG_LIST_PTR_T   ServiceProviderList;// 业务提供商名称(节点类型: languageString)
}MBBMSSG_SERVICE_T;

// 节目分片结构
typedef struct
{
    BOOLEAN         forfree;            // 免费指示(免费:TRUE, 收费:FALSE)
    BOOLEAN         live;               // 直播指示(直播:TRUE, 录像:FALSE)
    BOOLEAN         repeat;             // 回放指示(重播:TRUE, 首播:FALSE)

    SG_LIST_PTR_T   KeywordList;        // 关键词(节点类型: contentString)
    SG_LIST_PTR_T   ContentClassList;   // 内容媒体类型(节点类型: uint8)
    anyURI          serviceIdRef;       // 节目分片归属的频道分片标识

    SG_LIST_PTR_T   NameList;           // 节目名称(节点类型: languageString)
    SG_LIST_PTR_T   DescriptionList;    // 节目描述(节点类型: languageString)
    SG_DATETIME_T   StartTime;          // 节目开始时间
    SG_DATETIME_T   EndTime;            // 节目结束时间

    SG_LIST_PTR_T   AudioLanguageList;  // 内容伴音语言指示(节点类型: contentString)
    SG_LIST_PTR_T   TextLanguageList;   // 字幕语言指示(节点类型: contentString)
    SG_LIST_PTR_T   GenreList;          // 内容类别(节点类型: contentString)
}MBBMSSG_CONTENT_T;

// 预览数据分片
typedef struct
{
    SG_PREVIEW_PICTURE_T    *picture_ptr;   // 显示图像
    contentString           text;           // 预览用途的文字说明
}MBBMSSG_PREVIEW_DATA_T;

// 节目套餐分片
typedef struct
{
    SG_LIST_PTR_T   ServiceIdRefList;       // 关联的频道分片索引(节点类型: anyURI)
    SG_LIST_PTR_T   ContentIdRefList;       // 关联的节目分片索引(节点类型: anyURI)
    SG_LIST_PTR_T   NameList;               // 套餐名称(节点类型: languageString)
    SG_LIST_PTR_T   DescriptionList;        // 套餐描述(节点类型: languageString)
}MBBMSSG_PURCHASE_ITEM_T;

// 节目套餐详细信息
typedef struct
{
    SG_LIST_PTR_T   DescriptionList;        // 订购数据描述(节点类型: languageString)
    SG_PRICE_INFO_T PriceInfo;              // 价格信息
    anyURI          PurchaseItemIdRef;      // 关联的套餐分片
    SG_LIST_PTR_T   PurchaseChannelList;    // 关联的套餐订购渠道分片(节点类型: MBBMSSG_PURCHASE_CHANNEL_T)
}MBBMSSG_PURCHASE_DETAIL_T;

// 订购渠道分片
typedef struct
{
    SG_LIST_PTR_T   NameList;               // 订购渠道名称(节点类型: languageString)
    SG_LIST_PTR_T   DescriptionList;        // 订购渠道描述(节点类型: languageString)
    SG_LIST_PTR_T   PurchaseURLList;        // 订购请求的目的URL(节点类型: anyURI)
}MBBMSSG_PURCHASE_CHANNEL_T;

//接入分片
typedef struct
{
    SG_LIST_PTR_T   KeyManagementSystemList; // 密钥管理系统标识(节点类型: SG_KEY_MAN_SYS_T)
    SG_CMMB_PARA_T  CMMBPara;                // CMMB网络的参数
}MBBMSSG_ACCESS_T;
#define DSX_ISMACRYP_SALT_SIZE 8


typedef signed char           TSignedInt8;

/**
 *  @brief
 *    Signed 16-bit type.
*/
typedef signed short int      TSignedInt16;

/**
 *  @brief
 *    Signed 32-bit type.
*/
typedef signed long int       TSignedInt32;


/**
 *  @brief
 *    Unsigned 8-bit type.
*/
typedef unsigned char         TUnsignedInt8;

/**
 *  @brief
 *    Unsigned 16-bit type.
*/
typedef unsigned short int    TUnsignedInt16;

/**
 *  @brief
 *    Unsigned 32-bit type.
*/
typedef unsigned long int     TUnsignedInt32;



typedef TUnsignedInt8         TBoolean;

typedef enum
{
  DSX_ENCRYPTION_UNKNOWN,
   /**<  unknown */
  DSX_ENCRYPTION_ISMACRYP,
   /**<  ISMACryp */
  DSX_ENCRYPTION_SRTP,
   /**<  SRTP */
  DSX_ENCRYPTION_IPSEC,
   /**<  IPSec */
  DSX_ENCRYPTION_CLEAR
   /**<  no encryption */
} TDsxEncryption;

/**
 *  @brief
 *    Transports used by the DSX
*/
typedef enum
{
  DSX_TRANSPORT_UNKNOWN,
   /**<  RTP */
  DSX_TRANSPORT_RTP
   /**<  RTP */
} TDsxTransport;

/**
 *  @brief
 *    ISMACryp encryption modes
*/
typedef enum
{
  DSX_ISMACRYP_MODE_AES_CTR_128
   /**<  AES_CTR_128 mode */
} TDsxIsmacrypEncryptionMode;


typedef struct
{
  TDsxIsmacrypEncryptionMode  mode;
    /**< ISMACryp encryption mode */
  TUnsignedInt8               initializationVectorLength;
    /**< length in bytes of the initial initialization vector*/
  TUnsignedInt8               deltaInitializationVectorLength;
    /**< length in bytes of the delta initialization vector, 0 if unused */
  TBoolean                    useSelectiveEncryption;
    /**< TRUE if selective encryption is used, FALSE otherwise */
  TUnsignedInt8               keyIndicatorLength;
    /**< Length in bytes of the key indicators */
  TBoolean                    useKeyIndicatorPerAu;
    /**< TRUE if a Key indicator appears per Access Unit (AU) basis,
     * FALSE otherwise
    */
  TUnsignedInt8         salt[DSX_ISMACRYP_SALT_SIZE];
    /**< salt value (MSBF) */
} TDsxIsmacrypInitialData;

/**
 *  @brief
 *    Data describing the used encryption
*/
typedef struct
{
  TDsxEncryption   encryption;
  /**< used encryption */
  union 
  {
    TDsxIsmacrypInitialData  ismacrypData;
    /**< used for encryption DSX_ENCRYPTION_ISMACRYP */
  } initialData;
  /**< initial data for the descrambler object */
} TDsxEncryptionData;

/**
 *  @brief
 *    Possible access unit size length types used by RTP.
*/
typedef enum
{
  DSX_RTP_AU_CONSTANT_SIZE_LENGTH,
   /**<  auSizeLength is the constant size in octets of each Access Unit 
         for this stream. */
  DSX_RTP_AU_VARIABLE_SIZE_LENGTH,
   /**<  auSizeLength is the number of bits on which the AU-size field 
         is encoded in the AU-header. */
  DSX_RTP_AU_ABSENT_SIZE_LENGTH
   /**<  no size of the AU is present in the transport layer. */
} TDsxRtpAuSizeType;

/**
 *  @brief
 *    Initial data used for the RTP transport layer handling
*/
typedef struct
{
  TDsxRtpAuSizeType	auSizeLengthType;
    /**< Access unit size length type */
  TUnsignedInt32	  auSizeLength;
    /**< Access unit size length */
  TUnsignedInt32	  indexLength;
    /**< Number of bits on which the AU-Index is encoded in the first
     *   AU-header
    */
  TUnsignedInt32	  indexDeltaLength;
    /**< Number of bits on which the AU-Index-delta field is encoded in any
         non-first AU-header. */
  TUnsignedInt32	  ctsDeltaLength;
    /**< Number of bits on which the CTS-delta field is encoded in 
         the AU-header. */
  TUnsignedInt32  	dtsDeltaLength;
    /**< Number of bits on which the DTS-delta field is encoded in 
         the AU-header. */
  TBoolean		      useRandomAccessFlag;
    /**< If TRUE, the RAP-flag is present in the AU-header, if FALSE,
         the RAP-flag is absent. */
  TUnsignedInt32	  streamStateLength;
    /**< The number of bits on which the Stream-state field is encoded in the 
         AU-header. */
  TUnsignedInt32	  auxiliaryDataSizeLength;
    /**< The number of bits that is used to encode the 
         auxiliary-data-size field. */
} TDsxRtpInitialData;

/**
 *  @brief
 *    Data describing the used transport layer
*/
typedef struct
{
  TDsxTransport         transport;
  /**< used transport */
  union 
  {
    TDsxRtpInitialData  rtpData;
    /**< used for the RTP transport layer */
  } initialData;
  /**< initial data regarding the transport layer handling */
} TDsxTransportData;


typedef struct dsx_save_param_tag{
	
	TDsxEncryptionData xEncryptionData;
	TDsxTransportData xTransportData;
	
}DSX_SAVE_PARAM_T;


typedef struct dsx_isma_param_tag{

	DSX_SAVE_PARAM_T audio;
	
    DSX_SAVE_PARAM_T video;

	DSX_SAVE_PARAM_T data;

}DSX_ISMA_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 业务指南服务初始化
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E  MBBMSSG_Init(uint8* full_path_ptr,uint16 full_path_len);

/*****************************************************************************/
// Description : 退出业务指南服务
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC void MBBMSSG_Exit(void);

/*****************************************************************************/
// Description : 设置业务指南过滤器
// Global resource dependence :
// Author:Hui.Zhang
// Note: 必须在搜索业务指南业务启动前调用
/*****************************************************************************/
PUBLIC void MBBMSSG_SetFilter(
                              MBBMSSG_FILTER_T filter
                              );

/*****************************************************************************/
// Description : 清除业务指南过滤器
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC void MBBMSSG_ClearFilter(void);

/***********************************************************************/
//  Description : 搜索业务指南
//  Global resource dependence :
//  Author:Hui.Zhang
//  Note:
/***********************************************************************/
PUBLIC void MBBMSSG_SearchServiceGuide(void);

/***********************************************************************/
//  Description :  停止搜索业务指南
//  Global resource dependence :
//  Author:Hui.Zhang
//  Note:
/***********************************************************************/
PUBLIC void MBBMSSG_StopSearchingSG(void);

/***********************************************************************/
//  Description : 设置WAP
//  Global resource dependence :
//  Author:Hui.Zhang
//  Note:
/***********************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_SGSetWap(
                             char   *wap_ptr,   // [IN]
                             uint16 port        // [IN]
                             );

/***********************************************************************/
//  Description : 设置URI
//  Global resource dependence :
//  Author:Hui.Zhang
//  Note:
/***********************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_SetSGURI(
                             char* uri_ptr      // [IN]
                             );
/***********************************************************************/
// 	Description : process service guide
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_ProcessServiceGuide(void);

/*****************************************************************************/
// Description : 解析业务指南信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_ParseServiceGuide(void);

/*****************************************************************************/
//  Description : 停止解析业务指南信息
//  Global resource dependence : none
//  Author: Hui.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MBBMSSG_StopParseSG(void);

/*****************************************************************************/
// Description : 获取套餐索引信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetPurchaseItemIndex(
                                               SG_LIST_PTR_T *itemIdList_head_ptr    // [OUT] (节点类型: MBBMSSG_PURCHASEITEM_INDEX_T)
                                               );

/*****************************************************************************/
// Description : 释放套餐索引信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreePurchaseItemIndex(
                                                SG_LIST_PTR_T *itemIdList_head_ptr  // (节点类型: MBBMSSG_PURCHASEITEM_INDEX_T)
                                                );

/*****************************************************************************/
// Description : 获取套餐信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetPurchaseItem(
                                          anyURI itemId,    // [IN]
                                          MBBMSSG_PURCHASE_ITEM_T *item_ptr // [OUT]
                                          );

/*****************************************************************************/
// Description : 释放套餐信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreePurchaseItem(
                                           MBBMSSG_PURCHASE_ITEM_T *item_ptr
                                           );

/*****************************************************************************/
// Description : 获取套餐详细信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetPurchaseDetail(
                                            anyURI itemId,  // [IN]
                                            MBBMSSG_PURCHASE_DETAIL_T *detailData_ptr   // [OUT]
                                          );

/*****************************************************************************/
// Description : 释放套餐详细信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreePurchaseDetail(
                                             MBBMSSG_PURCHASE_DETAIL_T *detailData_ptr
                                             );

/*****************************************************************************/
// Description : 获取频道索引信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetServiceIndex(
                                          SG_LIST_PTR_T *serviceIdList_head_ptr  // [OUT] (节点类型: MBBMSSG_SERVICE_INDEX_T)
                                          );

/*****************************************************************************/
// Description : 释放频道索引信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreeServiceIndex(
                                           SG_LIST_PTR_T *serviceIdList_head_ptr    // (节点类型: MBBMSSG_SERVICE_INDEX_T)
                                           );

/*****************************************************************************/
// Description : 获取频道信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetService(
                                     anyURI serviceId,  // [IN]
                                     MBBMSSG_SERVICE_T *service_ptr     // [OUT]
                                     );

/*****************************************************************************/
// Description : 释放频道信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreeService(
                                      MBBMSSG_SERVICE_T *service_ptr
                                      );

/*****************************************************************************/
// Description : 获取节目索引信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetContentIndex(
                                          anyURI serviceId,     // [IN]
                                          SG_DATETIME_T date,   // [IN]
                                          SG_LIST_PTR_T *contentIdList_head_ptr  // [OUT] (节点类型: MBBMSSG_CONTENT_INDEX_T)
                                          );

/*****************************************************************************/
// Description : 释放节目索引信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreeContentIndex(
                                           SG_LIST_PTR_T *contentIdList_head_ptr    // (节点类型: anyURI)
                                           );

/*****************************************************************************/
// Description : 获取节目信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetContent(
                                     anyURI contentId,  // [IN]
                                     MBBMSSG_CONTENT_T *content_ptr     // [OUT]
                                     );

/*****************************************************************************/
// Description : 释放节目信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreeContent(
                                      MBBMSSG_CONTENT_T *content_ptr
                                      );

/*****************************************************************************/
// Description : 获取综合信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetComplexInfo(
                                         MBBMSSG_CONTEXT_COMPLEX_SEARCH_T search_info,  // [IN]
                                         SG_LIST_PTR_T *complexDataList_head_ptr       // [OUT] (节点类型: MBBMSSG_CONTEXT_COMPLEX_DATA_T)
                                         );

/*****************************************************************************/
// Description : 释放综合信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreeComplexInfo(
                                          SG_LIST_PTR_T *complexDataList_head_ptr   // (节点类型: MBBMSSG_CONTEXT_COMPLEX_DATA_T)
                                          );

/*****************************************************************************/
// Description : 通过频道分片标识获取套餐信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E  MBBMSSG_GetPurchaseItemByService(
                                                    anyURI serviceId,   // [IN]
                                                    SG_LIST_PTR_T *itemList_head_ptr    // [OUT] (节点类型: MBBMSSG_PURCHASEITEM_INDEX_T)
                                                    );

/*****************************************************************************/
// Description : 通过节目分片标识获取套餐信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E  MBBMSSG_GetPurchaseItemByContent(
                                                    anyURI contentId,   // [IN]
                                                    SG_LIST_PTR_T *itemList_head_ptr    // [OUT] (节点类型: MBBMSSG_PURCHASEITEM_INDEX_T)
                                                    );

/*****************************************************************************/
// Description : 获取预览数据
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E  MBBMSSG_GetPreviewdata(
                                          MBBMSSG_PREVIEWDATA_SEARCH_T search_info, // [IN]
                                          MBBMSSG_PREVIEW_DATA_T *data_ptr  // [OUT]
                                          );

/*****************************************************************************/
// Description : 释放预览数据
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E  MBBMSSG_FreePreviewData(
                                           MBBMSSG_PREVIEW_DATA_T *data_ptr
                                           );

/*****************************************************************************/
// Description : 获取频道标识
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetGlobalServiceId(
                                             anyURI serviceId,  // [IN]
                                             anyURI *globalServiceId_ptr    // [OUT]
                                             );

/*****************************************************************************/
// Description : 获取频道分片标识
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetServiceId(
                                       anyURI globalServiceId,  // [IN]
                                       anyURI *serviceId        // [OUT]
                                       );

/*****************************************************************************/
// Description : 获取节目标识
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetGlobalContentId(
                                             anyURI contentId,  // [IN]
                                             anyURI *globalContentId_ptr        // [OUT]
                                             );

/*****************************************************************************/
// Description : 获取节目分片标识
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetContentId(
                                       anyURI globalContentId,  // [IN]
                                       anyURI *contentId_ptr    // [OUT]
                                       );

/*****************************************************************************/
// Description : 获取套餐标识
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetGlobalPurchaseItemtId(
                                                   anyURI itemId,       // [IN]
                                                   anyURI *globalPurchaseItemId_ptr     // [OUT]
                                                   );

/*****************************************************************************/
// Description : 获取套餐分片标识
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetPurchaseItemId(
                                            anyURI globalPurchaseItemId,    // [IN]
                                            anyURI *itemId_ptr              // [OUT]
                                            );

/*****************************************************************************/
// Description : 释放AnyURI类型数据
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreeTypeAnyURI(
                                         anyURI *uri_ptr
                                         );

/*****************************************************************************/
// Description : 频道是否被加密
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_IsServiceEncrypted(
                                             anyURI serviceId,
                                             BOOLEAN *is_encrypted // [OUT]
                                             );

/*****************************************************************************/
// Description : 套餐是否被订购
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_IsPurchaseItemSubscribed(
                                                   anyURI itemId,
                                                   BOOLEAN *is_subscribed // [OUT]
                                                   );

/*****************************************************************************/
// Description : 获取接入信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetAccess(
                                    anyURI serviceId,  // [IN]
                                    MBBMSSG_ACCESS_T *access_ptr   // [OUT]
                                    );

/*****************************************************************************/
// Description : 释放接入信息
// Global resource dependence :
// Author:Hui.Zhang
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_FreeAccess(
                                     MBBMSSG_ACCESS_T *access_ptr
                                     );
/*****************************************************************************/
// Description : get access fragement
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetAccessFragment(
                                 anyURI serviceId,  // [IN]
                                 void *fragment_ptr   //SG_FRAGMENT_ACCESS_T [OUT]
                                 );                                     
/*****************************************************************************/
// Description : get mbbms service id by cmmb param
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetServiceIdByCmmbPara(
                                       
                                       uint8 cmmb_frqNo,  // [IN]
                                       uint16 cmmb_serviceId,  // [IN]
									                     anyURI *serviceId_ptr        // [OUT]
                                       );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSDP_GetIsma(

                        uint8 cmmb_frqNo,  // [IN]
                        uint16 cmmb_serviceId,  // [IN]                       
                        DSX_ISMA_PARAM_T *isma_ptr//[OUT]
                        );                                       
/*****************************************************************************/
//  Description : Create ISMA data
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSDP_CreateIsma(uint8  cmmb_frqNo, uint16  cmmb_serviceId);
// 	Description : get welcome information
//	Global resource dependence : 
//  Author:  
//	Note: App needs   to release memory 
//          
/*****************************************************************************/ 
uint8*  MBBMSSG_GetWelcomeInfo(uint32 *data_len_ptr);
/*****************************************************************************/
// Description : get free status id by service fragment index 
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_GetFreeStatusByServiceId(
                                             anyURI serviceId,  // [IN]
                                             BOOLEAN *is_free_ptr   // [OUT]
                                             );
/***********************************************************************/
// 	Description :  search service guide
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
PUBLIC SG_ERROR_E MBBMSSG_ManualSearchServiceGuide(void);
/*****************************************************************************/
// Description : free purchase item by service fragment id
// Global resource dependence :
// Author:
// Note: data structure of itemList_head_ptr node  is MBBMSSG_PURCHASEITEM_INDEX_T
/*****************************************************************************/
PUBLIC SG_ERROR_E  MBBMSSG_FreePurchaseItemByService(
                                                    
                                                    SG_LIST_PTR_T *itemList_head_ptr    // [OUT] 
                                                    );
/*****************************************************************************/
// Description : free purchase item by service fragment id
// Global resource dependence :
// Author:
// Note: data structure of itemList_head_ptr node  is MBBMSSG_PURCHASEITEM_INDEX_T
/*****************************************************************************/
PUBLIC SG_ERROR_E  MBBMSSG_FreePurchaseItemByContent(
                                                    
                                                    SG_LIST_PTR_T *itemList_head_ptr    // [OUT] 
                                                    );

/*****************************************************************************/
// Description : Compare DateTime type
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC int MBBMSSG_CompareDateTime(SG_DATETIME_T datetime1,SG_DATETIME_T datetime2);
/*****************************************************************************/
// Description : stop searching complex info
//               
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MBBMSSG_StopGetComplexInfo(
                                         void 
                                         );      
/*****************************************************************************/
// 	Description : get first bsmselector
//	Global resource dependence : 
//  Author:  
//	Note:structure of BSMSelectorIdList is anyURI.
//           App needs to call MBBMS_FreeBSMSelector to release memory 
//          
/*****************************************************************************/ 
PUBLIC uint32  MBBMS_GetFirstBSMSelector(void);
/***********************************************************************/
// 	Description : get sgdd id from sgdd file
//	Global resource dependence : none
//  Author:
//	Note:
/***********************************************************************/
PUBLIC BOOLEAN MBBMSSG_GetSGDDId(  
					
					anyURI *id_ptr
					);
/***********************************************************************/
// 	Description : free sgdd id from sgdd file
//	Global resource dependence : none
//  Author:
//	Note:
/***********************************************************************/
PUBLIC void MBBMSSG_FreeSGDDId(  
					
					anyURI *id_ptr
					);					
/***********************************************************************/
// 	Description : This function is used to set sg area name .
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/

PUBLIC SG_ERROR_E MBBMSSG_SetSGAreaName (uint8 *name_ptr,uint32 name_len);
/*****************************************************************************/
// Description : get genre list of content
// Global resource dependence :
// Author:
// Note: data structure of node is contentstring
/*****************************************************************************/
PUBLIC SG_LIST_PTR_T MBBMSSG_GetContentGenreIndex(void);

/*****************************************************************************/
// 	Description : download preview data
//	Global resource dependence : 
//  Author:  
//	Note:
/*****************************************************************************/ 
PUBLIC SG_ERROR_E MBBMSSG_DownloadPreviewData (void);
/***********************************************************************/
// 	Description :  test for parsing
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
PUBLIC void MBBMS_TestParse(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
