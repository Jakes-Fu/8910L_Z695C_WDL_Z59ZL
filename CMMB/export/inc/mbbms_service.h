/******************************************************************************
** File Name:      mbbms_service.h                                            *
** Author:                                                                    *
** Date:           2009/06/24                                                  *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.          *
** Description:    This file supply APIs& data structures for MMI application  in MBBMS service layer      *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 2009/6        Hong.Yu           Create                                     *
******************************************************************************/
#ifndef __MBBMS_SERVICE_H__
#define __MBBMS_SERVICE_H__

#define MAX_SG_PATH_LEN 	256
#define MAX_WAP_ADD_LEN 	128
#define MAX_MBBMS_UA_LEN   255

#include "sig_code.h"
#include "mtv_api.h"
#include "mbbms_sub.h"
#include "mbbms_auth.h"
//#include "mmipdp_api.h"
#include "mn_type.h"
#include "dal_player.h"

//MBBMS_SERVICE_FUNC_E: Mbbms service返回的流程
typedef enum   //SIG CODE TO CALLBACK
{
	MBBMS_SERVICE_SUBSCRIBE =MBBMS_SVC_SIG_SUBSCRIBE ,
	MBBMS_SERVICE_STOP_SUBSCRIBE = MBBMS_SVC_SIG_STOP_SUBSCRIBE,
	MBBMS_SERVICE_UNSUBSCRIBE =MBBMS_SVC_SIG_UNSUBSCRIBE,
	MBBMS_SERVICE_STOP_UNSUBSCRIBE =MBBMS_SVC_SIG_STOP_UNSUBSCRIBE,
     	MBBMS_SERVICE_GBA = MBBMS_SVC_SIG_GBA,
 	MBBMS_SERVICE_STOP_GBA =MBBMS_SVC_SIG_STOP_GBA,
   	MBBMS_SERVICE_UPDATE_TABLE =MBBMS_SVC_SIG_UPDATE_TABLE,
	MBBMS_SERVICE_STOP_UPDATE_TABLE =MBBMS_SVC_SIG_STOP_UPDATE_TABLE,
	MBBMS_SERVICE_EXIT=MBBMS_SVC_SIG_EXIT,
	MBBMS_SERVICE_ACTIVE=MBBMS_SVC_SIG_ACTIVE,
	MBBMS_SERVICE_STOP_ACTIVE=MBBMS_SVC_SIG_STOP_ACTIVE,
	MBBMS_SERVICE_UPDATE_SG=MBBMS_SVC_SIG_UPDATE_SG,
	MBBMS_SERVICE_STOP_UPDATE_SG=MBBMS_SVC_SIG_STOP_UPDATE_SG,
	MBBMS_SERVICE_MANUAL_SG=MBBMS_SVC_SIG_MANUAL_SG,
	MBBMS_SERVICE_STOP_MANUAL_SG=MBBMS_SVC_SIG_STOP_MANUAL_SG,
	MBBMS_SERVICE_ACCOUNT_QUIRY=MBBMS_SVC_SIG_ACCOUNT_QUIRY,
	MBBMS_SERVICE_STOP_ACCOUNT_QUIRY=MBBMS_SVC_SIG_STOP_ACCOUNT_QUIRY,
	
	MBBMS_SERVICE_PLAY = MBBMS_SVC_SIG_PLAY,
	MBBMS_SERVICE_STOP = MBBMS_SVC_SIG_STOP,
        MBBMS_SERVICE_STOP_CMMB = MBBMS_SVC_SIG_STOP_CMMB,
        MBBMS_SERVICE_RSSI_CHANGED_IND = MBBMS_SVC_SIG_RSSI_CHANGED_IND,
        MBBMS_SERVICE_PLAY_AV_IND = MBBMS_SVC_SIG_PLAY_AV_IND,
        MBBMS_SERVICE_EMERGENCY_BROADCAST_IND = MBBMS_SVC_SIG_EMERGENCY_BROADCAST_IND,
        MBBMS_SERVICE_CA_NOT_GRANTED_IND = MBBMS_SVC_SIG_CA_NOT_GRANTED_IND,
        MBBMS_SERVICE_PLAY_DRA_IND = MBBMS_SVC_SIG_PLAY_DRA_IND,
        MBBMS_SERVICE_BAD_SIGNAL_IND = MBBMS_SVC_SIG_BAD_SIGNAL_IND,
        MBBMS_SERVICE_SIGNAL_RECOVERED_IND =MBBMS_SVC_SIG_SIGNAL_RECOVERED_IND,
        MBBMS_SERVICE_SIGNAL_RELOCKING_IND =MBBMS_SVC_SIG_SIGNAL_RELOCKING_IND,
       MBBMS_SERVICE_PLAY_ERROR_IND = MBBMS_SVC_SIG_PLAY_ERROR_IND,
       MBBMS_SERVICE_PARSE_ESG = MBBMS_SVC_SIG_PARSE_ESG,
       
	MBBMS_SERVICE_PARSE_SG=MBBMS_SVC_SIG_PARSE_SG,
	MBBMS_SERVICE_STOP_PARSE_SG=MBBMS_SVC_SIG_STOP_PARSE_SG,
       MBBMS_SERVICE_DOWLOAD_PIC =  MBBMS_SVC_SIG_DOWLOAD_PIC,
       MBBMS_SERVICE_STOP_DOWLOAD_PIC = MBBMS_SVC_SIG_STOP_DOWLOAD_PIC,

	MBBMS_SERVICE_GETMSK=MBBMS_SVC_SIG_GETMSK,
	MBBMS_SERVICE_STOP_GETMSK=MBBMS_SVC_SIG_STOPGETMSK,
	MBBMS_SERVICE_PDP_ACTIVE=MBBMS_SVC_SIG_PDP_ACTIVE,
	MBBMS_SERVICE_PDP_DEACTIVE=MBBMS_SVC_SIG_PDP_DEACTIVE,
	MBBMS_SERVICE_IDLE,
	MBBMS_SERVICE_UPDATE_SOFT,   //Update software. Reserved for chinamobile standard..

	CMMB_SEARCH_PROG,
	CMMB_STOP_SEARCH_PROG,
	CMMB_GET_PROG,
	CMMB_STOP_GET_PROG,

	MBBMS_SERVICE_STOP_PARSE_ESG,
	
	MBBMS_SERVICE_MAX
}MBBMS_SERVICE_FUNC_E;

typedef enum
{
	MBBMS_SERVICE_MODE_CMMB,
	MBBMS_SERVICE_MODE_MBBMS,
	MBBMS_SERVICE_MODE_UNKOWN
}MTV_SERVICE_MODE_E;

//MBBMS_SERVICE_RET_E: mbbms service API立即返回枚举类型
typedef enum
{
	MBBMS_SERVICE_FUNC_OK,
	MBBMS_SERVICE_FUNC_BUSY,
	MBBMS_SERVICE_FUNC_INVALID,
	MBBMS_SERVICE_FUNC_NO_INIT,
	MBBMS_SERVICE_FUNC_NO_DATA,
	MBBMS_SERVICE_FUNC_PDP_FAIL,	
	MBBMS_SERVICE_FUNC_INVALID_PARAM

}MBBMS_SERVICE_RET_E;

//MBBMS_SERVICE_RESULT_E: mbbms service 返回结果列表
typedef enum
{
	MTVSERVICE_RST_RET_OK = 0,
	MTVSERVICE_RST_GENERAL_FAIL,
	MTVSERVICE_RST_STOP_OK,
	MTVSERVICE_RST_INVALID,
	MTVSERVICE_RST_NO_INIT,
	MTVSERVICE_RST_GBA_FAIL,
	MTVSERVICE_RST_PDP_FAIL,
      MTVSERVICE_RST_MSK_FAIL,
      MTVSERVICE_RST_WRONG_STATE,

	//SG其他错误类型范围100-199
	MTVSERVICE_RST_SG_NOT_UPDATED = 100,
	MTVSERVICE_RST_SG_RCV_FAIL,
    	MTVSERVICE_RST_INCOMPLETE_DATA,
	MTVSERVICE_RST_NO_DATA,
	MTVSERVICE_RST_SG_SEARCH_ABORT,
    	MTVSERVICE_RST_PARSE_ABORT,
	MTVSERVICE_RST_IS_SEARCHING,
	MTVSERVICE_RST_SERVICE_FAIL,
	MTVSERVICE_RST_NO_MEM,
	MTVSERVICE_RST_NEED_UPDATE_SG,

	//GBA其他错误类型范围200-299
	MTVSERVICE_RST_GBA_SYNC_FAIL = 200,
	MTVSERVICE_RST_SIM_FAIL,
	MTVSERVICE_RST_GBA_AUTH_FAIL,
	MTVSERVICE_RST_GBA_IMPI_FAIL,
	MTVSERVICE_RST_NETWORK_FAIL,

	//SUB其他错误类型范围300-499
	//对应规范的返回值+300: 301<-->431
	MTVSERVICE_RST_STATUS_CODE_SUCC =300,
	MTVSERVICE_RST_DEVICE_AUTH_FAIL = 301,
	MTVSERVICE_RST_USER_AUTH_FAIL =302,
	MTVSERVICE_RST_PURCHASE_UNKNOWN =303,
	MTVSERVICE_RST_DEVICE_AUTHORIZE_FAIL = 304,
	MTVSERVICE_RST_USER_AUTHORIZE_FAIL = 305,
	MTVSERVICE_RST_SERVICE_NOT_REG = 306,
	MTVSERVICE_RST_SERVER_ERR = 307,
	MTVSERVICE_RST_MALFORM_MSG = 308,
	MTVSERVICE_RST_CHARGING_ERR = 309,
	MTVSERVICE_RST_NO_SUBSCRIPTION = 310,
	MTVSERVICE_RST_NOT_PERMITTED = 311,
	MTVSERVICE_RST_NOT_SUPPORT_VER = 312,
	MTVSERVICE_RST_ILLEGAL_DEVICE = 313,
	MTVSERVICE_RST_AREA_NOT_ALLOWED = 314,
	MTVSERVICE_RST_REQUEST_UNAVAILABLE = 315,
	MTVSERVICE_RST_REQUEST_DONE = 316,
	MTVSERVICE_RST_ELEMENT_NOT_EXIST = 317,
	MTVSERVICE_RST_UNDIFINED = 318,
	MTVSERVICE_RST_DELAYED = 319,
	MTVSERVICE_RST_GENERATION_FAIL = 320,
	MTVSERVICE_RST_INVALID_INFO = 321,
	MTVSERVICE_RST_INVALID_REQUEST = 322,
	MTVSERVICE_RST_WRONG_DESTINATION = 323,
	MTVSERVICE_RST_WRONG_KEY_INFO = 324,
	MTVSERVICE_RST_PROVIDER_UNKOWN = 325,
	MTVSERVICE_RST_BSM_ID_UNKOWN = 326,
	MTVSERVICE_RST_ALREADY_USE = 327,
	MTVSERVICE_RST_NO_MATCH_FRAG = 328,
	MTVSERVICE_RST_SUBSCRIBED = 329,
	MTVSERVICE_RST_INSUFF_CHARGE = 431,
	
	MTVSERVICE_RST_SUB_HTTP_FAIL = 460,
	MTVSERVICE_RST_SUB_XML_FAIL,
	MTVSERVICE_RST_SUB_NO_SDCARD,
      
	 //CMMB ERR , 	
	MTVSERVICE_RST_SEARCHING_PRG = 500,
	MTVSERVICE_RST_INVALID_CENTER_FRQ,
	MTVSERVICE_RST_FILE_SYSTEM_ERR,
	MTVSERVICE_RST_NO_SPACE,
	MTVSERVICE_RST_SEARCHING_ESG,
	MTVSERVICE_RST_LACKOF_CONTROL_INFO



} MBBMS_SERVICE_RESULT_E;


typedef enum
{
	MTVSERVICE_SEARCH_PROG_ESG =0,
	MTVSERVICE_SEARCH_PROG,
	MTVSERVICE_SEARCH_ESG,
	MTVSERVICE_SEARCH_PROG_ESG_BY_FRQ,
	MTVSERVICE_SEARCH_PROG_BY_FRQ,
	MTVSERVICE_SEARCH_ESG_BY_NETID
}MTVSERVICE_SEARCH_TYPE_E;





typedef enum
{
	MBMSPDP_APP_MSG_INVALID,
	MBMSPDP_ACTIVE_REQ,			
	MBMSPDP_ACTIVE_CNF,
	MBMSPDP_DEACTIVE_REQ,
	MBMSPDP_DEACTIVE_CNF,
	MBMSPDP_DEACTIVE_IND,
	MBMSPDP_SUSPEND_IND,
	MBMSPDP_RESUME_IND,
	MBMSPDP_ACTIVE_TIMOUT_IND,
	MBMSPDP_REACTIVE_TIMER_IND,
	MBMSPDP_PS_ACTIVE_CNF,
	MBMSPDP_PS_DEACTIVE_CNF,
	MBMSPDP_PS_DEACTIVE_IND
} MBMSPDP_APP_MSG_E;
typedef enum
{
	MBMSPDP_RESULT_SUCC,
	MBMSPDP_RESULT_FAIL,
	MBMSPDP_RESULT_TIMEOUT
}MBMSPDP_RESULT_E;
typedef struct
{
	uint32				app_handler;
	uint8				nsapi;
	MBMSPDP_APP_MSG_E	msg_id;
	MBMSPDP_RESULT_E		result;
}MBMSPDP_CNF_INFO_T;

// MMI callback函数输入的数据结构
typedef struct MBBMS_SERVICE_SIG_Tag
{
       MBBMS_SERVICE_FUNC_E type; //MBBMS_SERVICE_FUNC_E  for MBBMS & CMMB_SERVICE_FUNC_E for CMMB
	MBBMS_SERVICE_FUNC_E subtype; //MBBMS_SERVICE_FUNC_E for MBBMS & CMMB_SERVICE_FUNC_E for cmmb mode.
       MBBMS_SERVICE_RESULT_E result; //MBBMS_SERVICE_RESULT_E for MBBMS & CMMB_SERVICE_RESULT_E for cmmb mode.
  	uint32 param;
	void *data_ptr;
} MBBMS_SERVICE_INFO_T;

//MTV SERVICE Callback prototype
typedef void (*MTVSERVICE_CALLBACK) (MBBMS_SERVICE_INFO_T *service_info_ptr);
typedef void (*MTVSERVICE_PDPCALLBACK)(MBMSPDP_CNF_INFO_T *msg_ptr);

// init function 's input parameters
typedef struct MBBMS_Service_Init_Para_Tag
{
	  MTV_SERVICE_MODE_E mode;
	  MN_DUAL_SYS_E sim_no;
         uint8 sg_path[MAX_SG_PATH_LEN];
	  uint32 sg_path_len; 
	  uint8 wap_addr[MAX_WAP_ADD_LEN];
	  uint32 wap_port;
	  uint8 sg_uri[MAX_WAP_ADD_LEN];
	  uint32 sg_uri_len;
	  uint8 cmmb_esg_path[MAX_SG_PATH_LEN];
	  uint32 cmmb_esg_len;
	  uint8 useragent[MAX_MBBMS_UA_LEN];
	  uint32 useragent_len;
} MBBMS_Service_Init_Para_T;

//SG language type
typedef enum
{
    MTVSERVICE_LANG_UNKNOWN = 0,
    MTVSERVICE_LANG_ZHO,
    MTVSERVICE_LANG_ENG
}MTVSERVICE_LANGUAGE_E;

// PDP activate msg for MMI. data_ptr
typedef struct MTVSERVICE_PDP_ACTIVE_TAG
{
	MN_DUAL_SYS_E dual_id;
	MTVSERVICE_PDPCALLBACK mtvservice_pdpcallback;
}MTVSERVICE_PDP_ACTIVE_T;

// mtv service program 
typedef struct  mtvservice_program_tag
{
anyURI          index;      // service index
languageString   name;   // service name
uint16           	weight;     // 
uint16           	net_id; //network id is effective in cmmb mode
uint16 		service_id; //service id is effective in cmmb mode
uint32   		char_set;   //it is effective in cmmb mode
BOOLEAN       is_forfree;  
BOOLEAN       is_subscribed;
BOOLEAN       is_encryped;
BOOLEAN       is_exist_content;//whether exist current  content
uint32           servicetype_count;
uint32           *servicetype_ptr;
uint32            genretype_count;
contentString     * genretype_ptr;
languageString      curcontentname; 
SG_DATETIME_T   curcontentstartTime; 
SG_DATETIME_T   curcontentendTime;   
} MTVSERVICE_PROGRAM_T;



typedef struct  mtvservice_program_list_tag
{
uint32 total_count;
MTVSERVICE_PROGRAM_T* list_ptr ;
} MTVSERVICE_PROGRAM_LIST_T;


typedef struct
{
    uint32 operate_status;  //operate status ,see MTV_STIMI_ERR_NUM_E
    uint8  *data_ptr; 
    uint32 data_type;       //data type,see  MTV_STIMI_SIG_DATA_TYPE_E
    uint32 data_len;        //when send broadcast msg to mmi,it is broadcast data length
}MTVSERVICE_CMMB_BROADCAST_IND_T;



//MTV SERVICE Callback prototype
//typedef void (*MTVSERVICE_CALLBACK) (MBBMS_SERVICE_INFO_T *service_info_ptr, uint32 para0, uint32 para1, uint32 para2);

BOOLEAN MTVSERVICE_Init(MBBMS_Service_Init_Para_T *init_ptr, MTVSERVICE_CALLBACK callback);
BOOLEAN MTVSERVICE_CMMBClose(void);
BOOLEAN MTVSERVICE_MBMSExit(void);
void MTVSERVICE_SetMskTestMode(BOOLEAN mode_enable );
void MTVSERVICE_SetLogOutput(BOOLEAN log_enable );
void MTVSERVICE_SetCWTestMode(BOOLEAN mode_enable );

MBBMS_SERVICE_RET_E MTVSERVICE_ChangeMode(MTV_SERVICE_MODE_E mode);
MBBMS_SERVICE_RET_E MTVSERVICE_Play(uint32 frequency_num, uint32 service_id);
MBBMS_SERVICE_RET_E MTVSERVICE_Stop(void);
MBBMS_SERVICE_RET_E MTVSERVICE_SetDisplayParam(DPLAYER_DISPLAY_PARAM_T *param_ptr);
MBBMS_SERVICE_RET_E MTVSERVICE_GetChannelStatus(MTV_CHANNEL_STATUS_T *status);
MBBMS_SERVICE_RET_E MTVSERVICE_GetCmmbSn(uint8 buffer_len,uint8  *sn_buffer);
MBBMS_SERVICE_RET_E MTVSERVICE_Pause(void);
MBBMS_SERVICE_RET_E MTVSERVICE_Resume(void);

MBBMS_SERVICE_RET_E MTVSERVICE_ServiceRequest(MBBMS_Service_Request_t *request_ptr);
MBBMS_SERVICE_RET_E MTVSERVICE_MBMSStopServiceRequest (void);

MBBMS_SERVICE_RET_E MTVSERVICE_UnsubRequest(MBBMS_Service_Request_t *request_ptr);
MBBMS_SERVICE_RET_E MTVSERVICE_MBMSStopUnsubRequest (void);

MBBMS_SERVICE_RET_E MTVSERVICE_AccountQuiry(MBBMS_Account_Inquiry_t *request_ptr);
MBBMS_SERVICE_RET_E MTVSERVICE_MBMSStopAccountQuiry(void);

MBBMS_SERVICE_RET_E MTVSERVICE_SubUpdate(void);
MBBMS_SERVICE_RET_E MTVSERVICE_MBMSStopSubUpdate (void);

MBBMS_SERVICE_RET_E MTVSERVICE_StartGBA(void);
MBBMS_SERVICE_RET_E MTVSERVICE_MBMSStopGBA(void);

 MBBMS_SERVICE_RET_E MTVSERVICE_ParseServiceGuide (void);
 MBBMS_SERVICE_RET_E MTVSERVICE_StopParseServiceGuide (void);

 MBBMS_SERVICE_RET_E MTVSERVICE_ActiveMBBMS (void);
 MBBMS_SERVICE_RET_E MTVSERVICE_DeactiveMBBMS (void);

 MBBMS_SERVICE_RET_E MTVSERVICE_GetProgramList (BOOLEAN is_updated,
          MTVSERVICE_LANGUAGE_E language_type, MTVSERVICE_PROGRAM_LIST_T* list_ptr);
 MBBMS_SERVICE_RET_E MTVSERVICE_FreeProgramList (MTVSERVICE_PROGRAM_LIST_T* list_ptr);

MBBMS_SERVICE_RET_E MTVSERVICE_ManualUpdateSG (uint32  search_type,uint32 para);
MBBMS_SERVICE_RET_E MTVSERVICE_StopManualUpdateSG (void);

MBBMS_SERVICE_RET_E MTVSERVICE_SetSGAreaName (uint8 *name_ptr, uint32 name_len);
/***********************************************************************/
// 	Description : This function is used to set sg wap .
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
 MBBMS_SERVICE_RET_E  MTVSERVICE_SGSetWap (
char   *wap_ptr, 
uint16  port
);

/***********************************************************************/
// 	Description : This function is used to set sg URI .
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
 MBBMS_SERVICE_RET_E  MTVSERVICE_SetSGURI(char* uri_ptr);
/***********************************************************************/
// 	Description : This function is used to free pdp active pointer when mmi wont use it anymore
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
void MTVSERVICE_FreePDPActivePtr(MTVSERVICE_PDP_ACTIVE_T *ptr);

/***********************************************************************/
// 	Description : This function is used to parse esg.
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
 MBBMS_SERVICE_RET_E MTVSERVICE_ParseESG (uint16  network_id);

/***********************************************************************/
// 	Description : This function is used to stop parsing esg.
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
 MBBMS_SERVICE_RET_E MTVSERVICE_StopParseESG (void);
#endif  //__MBBMS_SERVICE_H__


