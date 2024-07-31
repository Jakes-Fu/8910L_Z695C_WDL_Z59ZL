/******************************************************************************
** File Name:      mbbms_sub.h                                            *
** Author:                                                                    *
** Date:           2009/06/24                                                  *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.          *
** Description:    This file handles subscription and MSK update in MBBMS project                *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 2009/6        Hong.Yu           Create                                     *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 History:
 2009-7-12: 	typedef struct MBBMS_Subscript_Update_Response_Type_tag
 uint8 response_code;  //128 opened, 129: Paused, 130: Closed {Type is changed from string to BYTE}
 2009-7-12:  Function Definition changed
 Old:
 PUBLIC BOOLEAN MBBMS_Subscription_Update(MBBMS_Subscription_Update_t *request_ptr);
 New:
 PUBLIC BOOLEAN MBBMS_Subscription_Update(void)
 **--------------------------------------------------------------------------*/
 

#ifndef __MBBMS_SUB_MSK_H__
#define __MBBMS_SUB_MSK_H__

#include "sci_types.h"
#include "sfs.h"
#include "os_api.h"
#include "user_tasks_id.h"
#include "mbbms_auth.h"
#include "mbbms_sg_common.h"

#define MBBMS_IDREF_LEN		32
#define MBBMS_LTKM_LEN			256
#define MBBMS_KEY_DOMAIN_LEN	18
#define MBBMS_INFO_LEN			256


#define MBBMS_SUBSTATE_SUBSCRIBED          128
#define MBBMS_SUBSTATE_UNSUBSCRIBED     130
#define MBBMS_SUBSTATE_PAUSED                 129
#ifdef __cplusplus
extern   "C"
{
#endif

typedef enum{
MBBMS_USER_ID_TYPE_IMSI			=1,
MBBMS_USER_ID_TYPE_URI			=2,
MBBMS_USER_ID_TYPE_IMPI			=3,	
MBBMS_USER_ID_TYPE_CMMBSN		=128
}USER_ID_TYPE_E;

//MBBMS 开通，取消，暂停，恢复操作
typedef enum
{
	MBBMS_OPEN,
	MBBMS_CANCEL,
	MBBMS_PAUSE,
	MBBMS_RESUME,
	MBBMS_UPATE_USER,
	MBBMS_UNKOWN
}MBBMS_OPERATION_E;

typedef enum{
	MBBMS_ACCOUNT_UNKOWN  = 0,
	MBBMS_ACCOUNT_PURCHASEITEM = 1,
	MBBMS_ACCOUNT_SERVICEGUIDE = 2,
	MBBMS_ACCOUNT_BILLINGINFO = 3,
	MBBMS_ACCOUNT_PURCHASE = 4,
	MBBMS_ACCOUNT_PURCHASE_FRAG = 5 //use this item.
}MBBMS_ACCOUNT_INQUIRY_E;

typedef enum {
	MBBMS_ACCEPT = 0,
	MBBMS_SUB_SERVICE_REQUEST,
	MBBMS_SUB_SERVICE_UNSUB,
	MBBMS_SUB_UPDATE,
	MBBMS_SUB_QUIRY_ACCOUNT,
	MBBMS_MSK_GET_MSK,
	MBBMS_MTV_OPEN,
	MBBMS_MTV_CANCEL,
	MBBMS_MTV_SUSPEND,
	MBBMS_MTV_RESUME,
	MBBMS_MTV_IDLE,
	MBBMS_MTV_STOP_OPERATION,
	MBBMS_NOT_INIT
}MBBMS_MSK_SUBSCRIBER_TYPE_E;

typedef enum{
	
	MBBMS_MSK_SUB_SUCCESS = 0,
		
	MBBMS_MSK_SUB_DEVICE_AUTH_FAIL = 301,
	MBBMS_MSK_SUB_USER_AUTH_FAIL =302,
	MBBMS_MSK_SUB_PURCHASE_UNKNOWN =303,
	MBBMS_MSK_SUB_DEVICE_AUTHORIZE_FAIL = 304,
	MBBMS_MSK_SUB_USER_AUTHORIZE_FAIL = 305,
	MBBMS_MSK_SUB_SERVICE_NOT_REG = 306,
	MBBMS_MSK_SUB_SERVER_ERR = 307,
	MBBMS_MSK_SUB_MALFORM_MSG = 308,
	MBBMS_MSK_SUB_CHARGING_ERR = 309,
	MBBMS_MSK_SUB_NO_SUBSCRIPTION = 310,
	MBBMS_MSK_SUB_NOT_PERMITTED = 311,
	MBBMS_MSK_SUB_NOT_SUPPORT_VER = 312,
	MBBMS_MSK_SUB_ILLEGAL_DEVICE = 313,
	MBBMS_MSK_SUB_AREA_NOT_ALLOWED = 314,
	MBBMS_MSK_SUB_REQUEST_UNAVAILABLE = 315,
	MBBMS_MSK_SUB_REQUEST_DONE = 316,
	MBBMS_MSK_SUB_ELEMENT_NOT_EXIST = 317,
	MBBMS_MSK_SUB_UNDIFINED = 318,
	MBBMS_MSK_SUB_DELAYED = 319,
	MBBMS_MSK_SUB_GENERATION_FAIL = 320,
	MBBMS_MSK_SUB_INVALID_INFO = 321,
	MBBMS_MSK_SUB_INVALID_REQUEST = 322,
	MBBMS_MSK_SUB_WRONG_DESTINATION = 323,
	MBBMS_MSK_SUB_WRONG_KEY_INFO = 324,
	MBBMS_MSK_SUB_PROVIDER_UNKOWN = 325,
	MBBMS_MSK_SUB_BSM_ID_UNKOWN = 326,
	MBBMS_MSK_SUB_ALREADY_USE = 327,
	MBBMS_MSK_SUB_NO_MATCH_FRAG = 328,
	MBBMS_MSK_SUB_SUBSCRIBED = 329,
	MBBMS_MSK_SUB_INSUFF_CHARGE = 431,
	
	MBBMS_MSK_SUB_HTTP_FAIL = 560,
	MBBMS_MSK_SUB_XML_FAIL,
	MBBMS_MSK_SUB_NO_SDCARD,
	MBBMS_MSK_SUB_UNCLEAR,

//	MBBMS_MSK_SUB_HTTP_FAIL,
	MBBMS_MSK_SUB_GBA_FAIL,
//	MBBMS_MSK_SUB_NO_SDCARD,
//	MBBMS_MSK_SUB_XML_FAIL,
//	MBBMS_MSK_SUB_FAIL,
	MBBMS_MSK_SUB_STOP_OK          //200910@yuhong

}MBBMS_MSK_SUBSCRIBER_RESULT_E;

typedef struct MBBMS_Purchase_Data_Reference_Service_tag
{
	uint8 gloabl_id_ref[MBBMS_IDREF_LEN];  //SG GlobalPurchaseItemID in PurchaseItem segment.
	
	BOOLEAN	have_id_ref;  	//退订业务无效
	uint8 id_ref[MBBMS_IDREF_LEN];       
			//退订业务无效 "urn:allServices" to Cancel our services in the cancel service request.

	BOOLEAN	have_price;  //业务退订无效
	double	price; 		// 业务退订该项无效。not exist in the cancelserviceRequest.

	struct MBBMS_Purchase_Data_Reference_Service_tag *next;
}MBBMS_Purchase_Data_Reference_Service_t;

typedef struct MBBMS_Service_Request_tag
{
	BOOLEAN request_id_valid;
	unsigned int request_id;
	//unsigned char user_id[32];
	//USER_ID_TYPE_E  user_id_type;
	MBBMS_Purchase_Data_Reference_Service_t *data_ref_ptr;
}MBBMS_Service_Request_t;

typedef struct MBBMS_Purchase_Item_tag
{
	uint16 global_id_ref_len; //addlen
	uint8 global_id_ref[MBBMS_IDREF_LEN];
	uint8 itemwiseStatusCode;
	struct MBBMS_Purchase_Item_tag *next;
}MBBMS_Purchase_Item_t;

typedef struct MBBMS_LTKM_List_tag
{
	uint16 ltkm_len;
	uint8 ltkm[MBBMS_LTKM_LEN];
	struct MBBMS_LTKM_List_tag *next;
}MBBMS_LTKM_List_t;

typedef struct MBBMS_Service_Request_Res_tag
{
	uint32 request_id;
	uint8 global_status_code;
	uint16 description_len; //addlen
	uint8 description[MBBMS_INFO_LEN];   //@version2.0
	
	MBBMS_Purchase_Item_t *purchase_item_ptr;
	MBBMS_LTKM_List_t *smartcard_profile_ptr;
}MBBMS_Service_Request_Res_t;

typedef struct MBBMS_MSK_ID_List_tag
{
	uint32 msk_id;
	uint32 valid_from;
	uint32 valid_to;
	struct MBBMS_MSK_ID_List_tag *next;
}MBBMS_MSK_ID_List_t;

typedef struct MBBMS_MSK_domain_tag
{
	uint8 key_domain_id_len;
	uint8 key_domain_id[MBBMS_KEY_DOMAIN_LEN];
	uint32 msk_id;
	
	BOOLEAN valid_valid;
	uint32 valid_from;
	uint32 valid_to;
	struct MBBMS_MSK_domain_tag *next;
}MBBMS_MSK_domain_t;

typedef struct MBBMS_Msk_Request_tag
{
	//uint8 imsi[32];
	//uint8 cmmbsn[32];

	MBBMS_MSK_domain_t *msk_domain_ptr;   //More MSK request is possible.
}MBBMS_Msk_Request_t;

// Invalid structure. Same as MBBMS_LTKM_List_t
typedef struct MBBMS_MSK_LTKM_tag
{
	uint16 ltkm_len;
	uint8 ltkm[MBBMS_LTKM_LEN];
	struct MBBMS_MSK_LTKM_tag *next;
}MBBMS_MSK_LTKM_t;

typedef struct MBBMS_Msk_Response_tag
{
	uint16 description_len; //addlen
	uint8 description[MBBMS_INFO_LEN]; //@version2.0
	uint8 key_domain_id_len;
	uint8 key_domain_id[MBBMS_KEY_DOMAIN_LEN];
	uint32 msk_id;
	uint8 global_status_code;
	BOOLEAN ltkm_version_valid;
	uint8 ltkm_version;
	MBBMS_LTKM_List_t *ltkm_ptr; //LTKMResponseMSKExtensionType
	struct MBBMS_Msk_Response_tag *next;  //More MSK response is possible.
}MBBMS_Msk_Response_t;

typedef struct MBBMS_Operation_Res_tag
{
	uint32 result;
	uint8 description[MBBMS_INFO_LEN];
	MBBMS_LTKM_List_t *ltkm_ptr; 
}MBBMS_Operation_Res_t;  //RETURN @data_ptr

/* 订购关系表,但在xml schema中无体现.
typedef struct MBBMS_SubscribeInfo_List_tag
{
	uint32 subscribe_info;
	MBBMS_MSK_ID_List_t *msk_id;
	MBBMS_SubscribeInfo_List_t *next;
}MBBMS_SubscribeInfo_List_t;

typedef struct MBBMS_Subscriber_Table_tag
{
	uint8 activation_status;  //128:OPENED, 129: NOT OPENED, 130:PAUSED.
	MBBMS_MSK_ID_List_t *mskid_list;
	MBBMS_SubscribeInfo_List_t *subinfo_list;
}MBBMS_Subscriber_Table_t;
*/

typedef struct MBBMS_Account_Inquiry_tag
{
	BOOLEAN request_id_valid;
	uint32 request_id; 			//订购信息查询请求消息的标识 
		
	//BOOLEAN user_id_valid;
	//USER_ID_TYPE_E user_id_type;
	//unsigned char user_id[64];
	
	MBBMS_ACCOUNT_INQUIRY_E inquiry_type;
}MBBMS_Account_Inquiry_t;

typedef struct MBBMS_Account_Inquiry_Billinginfo_Tag
{
	uint16 billinginfo_len;  //addlen
	uint8 billinginfo[MBBMS_INFO_LEN];
	struct MBBMS_Account_Inquiry_Billinginfo_Tag *next;
	
}MBBMS_Account_Inquiry_Billinginfo_T;


typedef struct MBBMS_Purchase_Table_Info_tag
{
	MBBMS_Account_Inquiry_Billinginfo_T *descriptions_ptr;
	uint16 global_purchase_item_id_len; //addlen
	uint8 global_purchase_item_id[MBBMS_IDREF_LEN];
//       PurchaseItemFragment;  @version2.0
	SG_LIST_PTR_T                        purchase_item; // (节点类型: MBBMSSG_PURCHASE_ITEM_T)
	BOOLEAN purchase_data_idref_valid;
	uint16 purchase_data_idref_len; //addlen
	uint8 purchase_data_idref[MBBMS_IDREF_LEN];
    SG_LIST_PTR_T                        purchase_data; // (节点类型: MBBMSSG_PURCHASE_DETAIL_T)

//       PurchaseDataFragment;  @version2.0	
	struct MBBMS_Purchase_Table_Info_tag *next;
}MBBMS_Purchase_Table_Info_t;

typedef struct MBBMS_Service_Content_Reference_tag
{
	uint8 id_ref[MBBMS_IDREF_LEN];
	struct MBBMS_Service_Content_Reference_tag *next;
}MBBMS_Service_Content_Reference_t;


typedef struct MBBMS_Account_Inquiry_Res_tag
{
	BOOLEAN request_id_valid;
	uint32 request_id; 	//订购信息查询请求消息的标识 

	uint8	global_status_code;
	uint16 description_len; //addlen
	uint8 description[MBBMS_INFO_LEN];  //@version2.0
	
       MBBMS_Account_Inquiry_Billinginfo_T *billinginfo_ptr;

	MBBMS_Purchase_Table_Info_t *purchase_info_ptr;
}MBBMS_Account_Inquiry_Res_t;   //Return dataptr@Test AccountResponse

typedef struct MBBMS_Subscript_Update_ServiceID_tag
{
	uint8 purchaseitem_id[MBBMS_IDREF_LEN];
	struct MBBMS_Subscript_Update_ServiceID_tag *next;
}MBBMS_Subscript_Update_ServiceID_t;

typedef struct MBBMS_Subscript_Update_Ext_tag
{
//Empty for further extension usage.
	uint8 gard;
}MBBMS_Subscript_Update_Ext_t;

typedef struct MBBMS_Subscription_Update_t
{
	MBBMS_Subscript_Update_ServiceID_t *purchaseitem_ptr;
	MBBMS_Subscript_Update_Ext_t *ext_ptr;
}MBBMS_Subscription_Update_t;


typedef struct MBBMS_Subscript_Update_Response_Type_tag
{
	uint16 purchaseitem_id_len; //addlen
	uint8 purchaseitem_id[MBBMS_IDREF_LEN];
	uint16 description_len;//addlen
	uint8 description[MBBMS_INFO_LEN];
	uint8 response_code;  //128 opened, 129: Paused, 130: Closed {Changed to BYTE}
	MBBMS_MSK_ID_List_t *msk_ptr;
	
	BOOLEAN version_valid;
	uint8 version;
	struct MBBMS_Subscript_Update_Response_Type_tag *next;
}MBBMS_Subscript_Update_Response_Type_t;

typedef struct MBBMS_Subscript_Update_Response_Type_tag_V2
{
	MBBMS_Subscript_Update_Response_Type_t update_resp;
}MBBMS_Subscript_Update_Response_Type_V2_t;

typedef struct
{
	uint8 activation_status;
	uint8 register_extension_version;
	uint8 delivery_type;    //0 or 2 only.	uint8 delivery_mode; //useless data for further extension.
}MBBMS_Subscript_Update_Response_Ext_t;

typedef struct MBBMS_Subscription_Update_Res_tag
{
	MBBMS_Subscript_Update_Response_Type_t *response_ptr;
	MBBMS_Subscript_Update_Response_Ext_t *extend_ptr;
}MBBMS_Subscription_Update_Res_t;  //@version2.0

//signal code is defined as MBBMS_MSK_SUB_SIGNAL.
typedef struct MBBMS_MSK_SERVICE_REQUEST_SIG_Tag
{
      _SIGNAL_VARS
	MBBMS_MSK_SUBSCRIBER_TYPE_E type;
       MBBMS_MSK_SUBSCRIBER_RESULT_E result;
	void *data_ptr;
}MBBMS_MSK_SERVICE_REQUEST_SIG_T;

// init and exit
PUBLIC BOOLEAN MBBMS_Sub_MSK_Init(void);
PUBLIC BOOLEAN MBBMS_Sub_MSK_Exit(void);

PUBLIC void MBBMS_Set_SMD_Counter_Mode(BOOLEAN smdcounter);
//free data structure
PUBLIC void MBBMS_Free_Service_Request_Ptr(MBBMS_Service_Request_t *ptr);
PUBLIC void MBBMS_Free_Service_Request_Res_Ptr(MBBMS_Service_Request_Res_t *ptr);
PUBLIC void MBBMS_Free_SubUpdate_Request_Ptr(MBBMS_Subscription_Update_t *ptr);
PUBLIC void MBBMS_Free_Subupdate_Res_Ptr(MBBMS_Subscription_Update_Res_t *ptr);
PUBLIC void MBBMS_Free_Account_Query_Ptr(MBBMS_Account_Inquiry_t *ptr);
PUBLIC void MBBMS_Free_Account_Query_Res_Ptr(MBBMS_Account_Inquiry_Res_t *ptr);
PUBLIC void MBBMS_Free_MSK_Ptr(MBBMS_Msk_Request_t *ptr);
PUBLIC void MBBMS_Free_MSK_Res_Ptr(MBBMS_Msk_Response_t *ptr);
PUBLIC void MBBMS_Free_Operation_Res_Ptr(MBBMS_Operation_Res_t *ptr);

//Entry to Open/Cancel/Suspend/Resume the MBBMS MTV. 异步函数
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Operation(MBBMS_OPERATION_E  Op_ptr );
//Get the MBBMS MTV state: Open, cancel, suspending. 异步函数
PUBLIC MBBMS_OPERATION_E MBBMS_Operation_Status(void);

//业务订购请求,退订,查询和信息更新.异步函数
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Service_Request( MBBMS_Service_Request_t *request_ptr);
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Unsubscribe_Request( MBBMS_Service_Request_t *request_ptr);
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Subscription_Update(void);
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Account_Inquiry(MBBMS_Account_Inquiry_t *request_ptr);

//MBBMS_Get_Account_Info is not supported now.
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Get_Account_Info(MBBMS_Account_Inquiry_Res_t *ptr);

//Stop the HTTP procedure if http communication is in middle of processing.
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_MSK_SUB_Stop(void);
//业务密钥获取和播放时设置
//the process entry of retrieve the serivce keys from network.
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_MSK_Request(MBBMS_Msk_Request_t *data_ptr);
//Set the msk_id's service keys before playing the encrypted service.
PUBLIC BOOLEAN MBBMS_Set_MSK(uint32 keydomainid, uint32 msk_id);
//check if msk_id's service key is available.
PUBLIC BOOLEAN MBBMS_Check_MSK(uint32 keydomainid, uint32 msk_id);

PUBLIC void  MBBMS_SUBSetCallback(MBMSMIDWARE_CALLBACK sub_callback );
PUBLIC void  MBBMS_SUBCleanCallback(void);

PUBLIC BOOLEAN MBBMS_Check_GBA_Ready(void);
PUBLIC BOOLEAN MBBMS_Load_UpdateTable(void);

#ifdef __cplusplus
}
#endif
#endif






