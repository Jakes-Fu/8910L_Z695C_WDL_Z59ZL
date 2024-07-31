/****************************************************************************
** File Name:      nps_http.c                                              *
** Author:         kelly.li                                                *
** Date:           April/06/2006                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** April/06/2006  kelly.li         Create                                  *
**                                                                         *
****************************************************************************/

/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmipdp_export.h"
#include "mn_type.h"
#include "mmiconnection_export.h"
#include "mmi_module.h"
#include "mmi_appmsg.h"
#ifdef ENABLE_HTTPC
#include "httpc_api.h"
#else 
#include "http_api.h"
#endif
#include "in_message.h"
#include "mmi_signal.h"
#include "mmk_type.h"
#include "mmiset_export.h"
#include "xml_sap.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "nps_text.h"
#include "nps_main.h"
#include "nps_id.h"
#include "mmi_common.h"
#include "mmicom_banner.h"
#include "mmifdn_export.h"
#include "guitext.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define NPS_PDP_ACTIVE_TIMEOUT       20000  
//#define PDP_RETRY_TIMEOUT        3000 //重新尝试连接pdp，换卡连接

#define ACCEPT_STRING           "text/html, */*"
#define ACCEPT_LANGUAGE_STRING  "zh-cn, en"
#define USER_AGENT_STRING       "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)"
#define ACCEPT_CHARSET_STRING   "utf-8"
#define HTTP_URI_STRING           "https://test"

#define NPS_HTTP_CONTENT_BUFFER_SIZE    256

//#define PARSER_SEP_CHAR                "$"     //tag name separator
#define XML_TAG_MAX_LEN                64

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum 
{
    XML_TAG_NONE = 0,				//"xml_tag_none",
    XML_TAG_RESPONSE,			    // 1"response",
    XML_TAG_STATUS,                 // 2"status",
    XML_TAG_MESSAGE,                // 3"message",
    XML_TAG_CODE,                   // 4"code",
    XML_TAG_ROWID,                  // 5"rowId",
    XML_TAG_MAX
}XML_TAG_E;
#ifdef ENABLE_HTTPC
typedef struct _MMINPS_HTTP_CONTEXT_T_
{
    HTTPC_APP_MODULE_ID_T module_id;
    HTTPC_CONTEXT_ID_T    context_id;
    HTTPC_REQUEST_ID_T    request_id;
    HTTPC_APP_INSTANCE_T  app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
}MMINPS_HTTP_CONTEXT_T;
#else
typedef struct _MMINPS_HTTP_CONTEXT_T_
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
}MMINPS_HTTP_CONTEXT_T;
#endif
typedef struct _MMINPS_DATA_INFO_T_
{
	wchar                       *status;            // 
    wchar                       *message;           // 
    wchar                       *code;              // 
    wchar                       *rowId;             // 
}MMINPS_DATA_INFO_T;

typedef struct _MMINPS_PARSE_TEMP_T_
{
    XML_TAG_E                   cur_tag;            // 记录正在解析的 xml tag
    BOOLEAN                     is_bad_data;        // 非正常数据。
    MMINPS_DATA_INFO_T          data_info;          // 数据信息
}MMINPS_PARSE_TEMP_T;

LOCAL const char* const s_nps_xml_tag_arr[] = 
{
	"",		       
    "response",
    "status",
    "message",
    "code",    
    "rowId"
};

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint32                      s_net_id            = 0;
LOCAL uint8                       s_nps_pdp_timer_id      = 0;
//LOCAL uint8                       s_pdp_retry_timer_id = 0;
LOCAL MMINPS_HTTP_CONTEXT_T       s_nps_http_context    = {0};
LOCAL char *                      s_nps_content_buffer = PNULL; 
LOCAL uint16                      s_sim_sys_fdn = MN_DUAL_SYS_1;
/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/																	//store if the blue tooth is power off because USB


/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: yanxian.zhou
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpActiveTimer(
								uint8 timer_id,
								uint32 param
								);

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartPdpActiveTimer(void);

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopPdpActiveTimer(void);

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMINPS_HTTP_CONTEXT_T* GetHttpContextPtr(void);

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
#ifdef ENABLE_HTTPC
void StartHttpRequest(void);
#else
LOCAL void StartHttpRequest(void);
#endif
/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpInitCnf(
									 MMINPS_HTTP_CONTEXT_T * http_context_ptr, 
									 DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpRequestIdInd(
										  MMINPS_HTTP_CONTEXT_T *http_context_ptr, 
										  DPARAM param);

/*****************************************************************************/
//  Description : HandleHttpDataInd
//  Global resource dependence : 
//  Author: Glen Li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpDataInd(
									MMINPS_HTTP_CONTEXT_T * http_context_ptr, 
									DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpGetCnf(
									MMINPS_HTTP_CONTEXT_T *http_context_ptr, 
									DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpErrorInd(MMINPS_HTTP_CONTEXT_T *http_context_ptr, 
									  DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpCloseCnf(
								MMINPS_HTTP_CONTEXT_T *http_context_ptr, 
								DPARAM param);

/*****************************************************************************/
// 	Description : 天气信息BUFFER 分配
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_DataInfoAlloc(void);

/*****************************************************************************/
// 	Description : 天气信息BUFFER 释放
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
LOCAL void MMINPS_DataInfoRelease(void);

/*****************************************************************************/
// 	Description : 获得天气信息
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
LOCAL char* MMINPS_GetDataInformation(void);

/*****************************************************************************/
//  Description : Start parse
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_ParseStart(void);

/*****************************************************************************/
//  Description : Do parse data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Xml_ParseData(
                            uint8* buf_ptr,                         //[in] Data buffer to paser
                            int32 buf_len,                         //[in] Data length to paser
                            MMINPS_PARSE_TEMP_T *parse_data_ptr //[out] Data get
                            );

/*****************************************************************************/
//  Description : start element handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Xml_StartElementHandler(
                        void        *user_data_ptr,
                        const char  *localname_ptr,
                        const char  **attributes_pptr
                        );

/*****************************************************************************/
//  Description : end element handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Xml_EndElementHandler(
                        void         *user_data_ptr,
                        const uint8  *localname_ptr
                        );

/*****************************************************************************/
//  Description : character date handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Xml_CharacterDataHandler(
                                 void         *user_data_ptr,
                                 const uint8  *ch_ptr 
                                 );

/*****************************************************************************
// 	Description : Set element's attributes
//	Global resource dependence : 
//  Author:Gaily.Wang
//	Note: 
*****************************************************************************/
LOCAL void Xml_GetElementAttributes(
                                MMINPS_PARSE_TEMP_T *parse_data_ptr,
                                XML_TAG_E           tag
                                );

/*****************************************************************************
// 	Description : Set element's attributes
//	Global resource dependence : 
//  Author:Gaily.Wang
//	Note: 
*****************************************************************************/
LOCAL void Xml_GetElementData(
                                MMINPS_PARSE_TEMP_T *parse_data_ptr,
                                XML_TAG_E          tag, 
                                const uint8        *ch_ptr
                                );

/*****************************************************************************/
//  Description : Handle parsed data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_HandleParsedData(MMINPS_PARSE_TEMP_T *temp_data);

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);

/*****************************************************************************/
//  Description : Open Popup win
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL void MMINPS_SendMsgToApp(uint32 sig_id);

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :FDN开启提醒窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFdnOpenedAlertWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    MMI_HANDLE_T ctrl_handle = MMI_INVALID_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMIPUB_GetDialogTextCtrlId(win_id);
         GUITEXT_SetAlign(ctrl_handle, ALIGN_LEFT); 
        break;
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
        MMISET_OpenFdnHandleWindow(s_sim_sys_fdn);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : 打开FDN开启提醒窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIAPINPS_FDNOpenedAlertWindow(uint16 sim_sys) 
{
     MMI_WIN_PRIORITY_E      win_priority_dialog = WIN_ONE_LEVEL;
     MMI_WIN_ID_T     win_id_query = MMINPS_FDN_OPENED_ALERT_WIN_ID;
     s_sim_sys_fdn  =  sim_sys;
     MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_FDN_FEEDBACK, PNULL, IMAGE_NULL, &win_id_query,&win_priority_dialog, 
            MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleFdnOpenedAlertWindow);
}

/*****************************************************************************/
//  Description :find the available sim with FDN status judge                     
//  Global resource dependence : none
//  Author: zhenxing
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPINPS_GetSIMAndFDNStatusFunction(MN_DUAL_SYS_E *sim_sys)
{
    BOOLEAN  return_value = FALSE;
    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
#ifdef MOBILE_DATA_CONN_USE_PERFER_SIM_ONLY
    MN_DUAL_SYS_E   active_sim=MN_DUAL_SYS_1;

    if(!MMIAPIPHONE_GetDataServiceSIM(&active_sim))
    {
        SCI_TRACE_LOW("[NPS]MMIAPINPS_GetSIMAndFDNStatusFunction Error: MMIAPIPHONE_GetDataServiceSIM return false");
        return return_value;
    }

    *sim_sys = (MN_DUAL_SYS_E)active_sim;
    fdn_context_ptr = MMIAPIFDN_GetFDNContext(active_sim);

    if (fdn_context_ptr->is_fdn_enable)
    {
        MMIAPINPS_FDNOpenedAlertWindow(active_sim);            
        SCI_TRACE_LOW("[NPS]MMIAPINPS_GetSIMAndFDNStatusFunction sim_sys=%d FDN is on", active_sim);
        return return_value;
    }
    
    if (MMIAPIPHONE_IsSimAvailable(active_sim))
    {
        return_value = TRUE;
        SCI_TRACE_LOW("[NPS]MMIAPINPS_GetSIMAndFDNStatusFunction sim_sys=%d", active_sim);
    }else
    {
        SCI_TRACE_LOW("[NPS]MMIAPINPS_GetSIMAndFDNStatusFunction sim_sys=%d is not available", active_sim);
    }

    return return_value;
#else
    uint16   dual_sim = 0;
    
#ifdef MMI_MULTI_SIM_SYS_SINGLE 
    dual_sim = MN_DUAL_SYS_1;
    fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sim);
    if(MMIAPIPHONE_IsSimAvailable(dual_sim) && (fdn_context_ptr->is_fdn_enable))
    {
        MMIAPINPS_FDNOpenedAlertWindow(dual_sim);
        SCI_TRACE_LOW("[NPS]MMIAPINPS_GetSIMAndFDNStatusFunction single sim FDN is on");
    }
    else if(MMIAPIPHONE_IsSimAvailable(dual_sim))
    {
        return_value = TRUE;
    }
    *sim_sys = (MN_DUAL_SYS_E)dual_sim;
    return return_value;
#else   
    uint32 i = 0;
    dual_sim = MN_DUAL_SYS_1;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        fdn_context_ptr = MMIAPIFDN_GetFDNContext(i);
        if (MMIAPIPHONE_IsSimAvailable(i) && (!(fdn_context_ptr->is_fdn_enable)))//find the available sim with fdn is off
        {
            dual_sim = i;            
            return_value = TRUE;
            *sim_sys = (MN_DUAL_SYS_E)dual_sim;
            SCI_TRACE_LOW("[NPS]MMIAPINPS_GetSIMAndFDNStatusFunction sim_sys=%d", dual_sim);
            return return_value;
        }
    }
    //all available sim's fdn are on
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        fdn_context_ptr = MMIAPIFDN_GetFDNContext(i);
        if (MMIAPIPHONE_IsSimAvailable(i) && (fdn_context_ptr->is_fdn_enable))
        {
            dual_sim = i;
            MMIAPINPS_FDNOpenedAlertWindow(dual_sim);            
            *sim_sys = (MN_DUAL_SYS_E)dual_sim;
            SCI_TRACE_LOW("[NPS]MMIAPINPS_GetSIMAndFDNStatusFunction sim_sys=%d FDN is on", dual_sim);
            return return_value;
        }
    }
    
    SCI_TRACE_LOW("[NPS]MMIAPINPS_GetSIMAndFDNStatusFunction no sim available");
    return return_value;
#endif
#endif
}

/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: yanxian.zhou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMINPS_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_1;
    MMI_STRING_T            wait_text = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL; 
    BOOLEAN                 available_send = FALSE;

//    dual_sys = (MN_DUAL_SYS_E)MMIAPISET_GetActiveSim();
    available_send = MMIAPINPS_GetSIMAndFDNStatusFunction(&dual_sys);
    if(!available_send)//all available sim's fdn are on or no availabele sim, could not to send feenback 
     {
        return return_val;
     }
#ifdef BROWSER_SUPPORT
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, MMIAPIBROWSER_GetNetSettingIndex(dual_sys));
#endif
    if(setting_item_ptr)
    {
        active_info.app_handler         = MMI_MODULE_NPS;
        active_info.dual_sys            = dual_sys;
        active_info.apn_ptr             = (char*)setting_item_ptr->apn;
        active_info.user_name_ptr       = (char*)setting_item_ptr->username;
        active_info.psw_ptr             = (char*)setting_item_ptr->password;
        active_info.priority            = 3;
        active_info.ps_service_rat      = MN_UNSPECIFIED;
        active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
        active_info.handle_msg_callback = HandlePdpMsg;
        active_info.ps_service_type = BROWSER_E;
        active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type   = setting_item_ptr->ip_type;
#endif

       if(MMIAPIPDP_Active(&active_info))
       {
#if 0
             MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text); 
             MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMINPS_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,PNULL);
#endif
//             fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);
//             if(fdn_context_ptr->is_fdn_enable)
//             if(!MMIPDP_IsFDNPermitActivePdp(active_info.dual_sys))
//             {
//                   MMIAPINPS_FDNOpenedAlertWindow();
 //                  SCI_TRACE_LOW("[NPS]MMINPS_pdpActie FDN is on");
//             }
//             else
             {
                   MMIAPINPS_OpenFeedbackSendingWindow();
             }
        StartPdpActiveTimer();
        return_val = TRUE;

        }
        
     }

    return return_val;
}

/*****************************************************************************/
//  Description : Deactive Pdp
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMINPS_PdpDeactive(void)
{
    BOOLEAN return_val = FALSE;

	MMINPS_DataInfoRelease();
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_NPS);
	
    return return_val;
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: yanxian.zhou
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    
    if(PNULL == msg_ptr)
    {
        return;
    }
	
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        StopPdpActiveTimer();
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
		{
            s_net_id = msg_ptr->nsapi;
            MMINPS_SendMsgToApp(MSG_NPS_PDP_ACTIVE_CNF);
		}
        else 
        {
            MMINPS_PdpDeactive();
            MMINPS_SendMsgToApp(MSG_NPS_PDP_ACTIVE_FAILED_CNF);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
        s_net_id = 0;
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
        s_net_id = 0;
        MMINPS_PdpDeactive();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpActiveTimer(
								uint8 timer_id,
								uint32 param
								)
{
	StopPdpActiveTimer();
    MMINPS_PdpDeactive();
    MMINPS_SendMsgToApp(MSG_NPS_CONN_FAILED_CNF);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartPdpActiveTimer(void)
{
    if(0 != s_nps_pdp_timer_id)
    {
        MMK_StopTimer(s_nps_pdp_timer_id);
        s_nps_pdp_timer_id = 0;
    }
    
    s_nps_pdp_timer_id = MMK_CreateTimerCallback(NPS_PDP_ACTIVE_TIMEOUT, 
		HandlePdpActiveTimer, 
		PNULL, 
		FALSE);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopPdpActiveTimer(void)
{
	
    if(0 != s_nps_pdp_timer_id)
    {
        MMK_StopTimer(s_nps_pdp_timer_id);
        s_nps_pdp_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleNPSHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMINPS_HTTP_CONTEXT_T *http_context_ptr = PNULL;
    void *original_param = (void*)((uint32)param);
    
	http_context_ptr = GetHttpContextPtr();

	SCI_TRACE_LOW("[NPS] HandleNPSHttpMsg msg_id = %d", msg_id);
	SCI_TRACE_LOW("[NPS] http_context_ptr context_id = %d",http_context_ptr->context_id);

    switch (msg_id)
    {
#ifdef ENABLE_HTTPC
    case HTTPC_SIG_INIT_CNF:
        {
            HTTPC_INIT_CNF_SIG_T * cnf = (HTTPC_INIT_CNF_SIG_T*)original_param;
                      
            SCI_TRACE_LOW("[NPS]:HTTPC_SIG_INIT_CNF app_param = %d,cnf->module_id = %d",cnf->app_param,cnf->module_id);
            if(cnf->module_id != MMI_MODULE_NPS || 1 != cnf->app_param)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpInitCnf(http_context_ptr, param);
        }
        break;

    case HTTPC_SIG_REQUEST_ID_IND:
        {
            HTTPC_REQUEST_ID_IND_SIG_T *cnf = (HTTPC_REQUEST_ID_IND_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTPC_SIG_REQUEST_ID_IND context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpRequestIdInd(http_context_ptr, param);
        }
        break;

	case HTTPC_SIG_DATA_IND:
		{
		    HTTPC_DATA_IND_SIG_T *cnf = (HTTPC_DATA_IND_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTPC_SIG_DATA_IND context_id = %d",cnf->context_id);
		    if(cnf->context_id != http_context_ptr->context_id)
		    {
		        return MMI_RESULT_FALSE;
		    }
		    result = HandleHttpDataInd(http_context_ptr, param);
		}
		break;

    case HTTPC_SIG_GET_CNF:
        {
            HTTPC_GET_CNF_SIG_T *cnf = (HTTPC_GET_CNF_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTPC_SIG_GET_CNF context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpGetCnf(http_context_ptr, param);
        }
        break;

    case HTTPC_SIG_HEADER_IND:
        {
            HTTPC_HEADER_IND_SIG_T *cnf = (HTTPC_HEADER_IND_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTPC_SIG_HEADER_IND context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }

            //result = HandleHttpHeaderInd(http_context_ptr, param);
        }
        break;

	case HTTPC_SIG_ERROR_IND:
        {
            HTTPC_ERROR_IND_SIG_T *cnf = (HTTPC_ERROR_IND_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTPC_SIG_ERROR_IND context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpErrorInd(http_context_ptr, param);
        }
        break;

    case HTTPC_SIG_CLOSE_CNF:
        {
            HTTPC_CLOSE_CNF_SIG_T *cnf = (HTTPC_CLOSE_CNF_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTPC_SIG_CLOSE_CNF context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpCloseCnf(http_context_ptr, param);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
#else
	    case HTTP_SIG_INIT_CNF:
        {
            HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)original_param;
                      
            SCI_TRACE_LOW("[NPS]:HTTP_SIG_INIT_CNF app_param = %d,cnf->module_id = %d",cnf->app_param,cnf->module_id);
            if(cnf->module_id != MMI_MODULE_NPS || 1 != cnf->app_param)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpInitCnf(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_REQUEST_ID_IND:
        {
            HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTP_SIG_REQUEST_ID_IND context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpRequestIdInd(http_context_ptr, param);
        }
        break;

	case HTTP_SIG_DATA_IND:
		{
		    HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTP_SIG_DATA_IND context_id = %d",cnf->context_id);
		    if(cnf->context_id != http_context_ptr->context_id)
		    {
		        return MMI_RESULT_FALSE;
		    }
		    result = HandleHttpDataInd(http_context_ptr, param);
		}
		break;

    case HTTP_SIG_GET_CNF:
        {
            HTTP_GET_CNF_SIG_T *cnf = (HTTP_GET_CNF_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTP_SIG_GET_CNF context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpGetCnf(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_HEADER_IND:
        {
            HTTP_HEADER_IND_SIG_T *cnf = (HTTP_HEADER_IND_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTP_SIG_HEADER_IND context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }

            //result = HandleHttpHeaderInd(http_context_ptr, param);
        }
        break;

	case HTTP_SIG_ERROR_IND:
        {
            HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTP_SIG_ERROR_IND context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpErrorInd(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_CLOSE_CNF:
        {
            HTTP_CLOSE_CNF_SIG_T *cnf = (HTTP_CLOSE_CNF_SIG_T*)original_param;

			SCI_TRACE_LOW("[NPS]:HTTP_SIG_CLOSE_CNF context_id = %d",cnf->context_id);
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpCloseCnf(http_context_ptr, param);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
#endif
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle NPS app Msg
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/

PUBLIC MMI_RESULT_E HandleNPSAppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	SCI_TRACE_LOW("[NPS] HandleNPSAppMsg msg_id = %d", msg_id);

    switch(msg_id)
    {
    case MSG_NPS_PDP_ACTIVE_CNF: 
        {
			MMINPS_DataInfoAlloc();
            StartHttpRequest();
        }
        break;
		
    case MSG_NPS_PDP_ACTIVE_FAILED_CNF:
    case MSG_NPS_CONN_FAILED_CNF:
    case MSG_NPS_PARSE_FAILED_CNF:
        {
//			MMK_CloseWin(MMINPS_WAIT_WIN_ID);
            MMK_CloseWin(MMINPS_FEEDBACK_SENDING_WIN_ID);
//			MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }    		   	
        break; 
		
    case MSG_NPS_PARSE_START_IND:
        {
			MMINPS_ParseStart();
        }
        break;
		
    case MSG_NPS_PARSE_SUCCESS_CNF:
        {
			MMICOM_BANNER_INFO_T banner_info = {0};
	        GUI_POINT_T pin_alert_start_point = {0, 0};
			MMI_WIN_ID_T  win_id = MMK_GetFocusWinHandle();
//			MMK_CloseWin(MMINPS_WAIT_WIN_ID);
            MMK_CloseWin(MMINPS_FEEDBACK_SENDING_WIN_ID);
			MMK_CloseWin(MMINPS_MAIN_MENU_WIN_ID);

			//feedback sent banner prompt
			banner_info.banner_type = MMI_BANNER_TEXT;
 	        banner_info.text1_id = STR_NPS_FEEDBACK_SENT_EXT01;
 	        banner_info.start_point = pin_alert_start_point;
 	        MMIAPICOM_OpenBannerWin(win_id, banner_info);
//			MMIAPINPS_OpenCompleteWindow();
        }
        break;
		
	case MSG_NPS_HTTP_CLOSE_CNF:
        {
//			MMK_CloseWin(MMINPS_WAIT_WIN_ID);
            MMK_CloseWin(MMINPS_FEEDBACK_SENDING_WIN_ID);
			MMINPS_PdpDeactive();
        }
        break;
		
    default:
        {
        }
        break;
    }
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMINPS_HTTP_CONTEXT_T* GetHttpContextPtr(void)
{
    return &s_nps_http_context;
}

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
#ifdef ENABLE_HTTPC
PUBLIC void StartHttpRequest(void)
{
    HTTPC_INIT_PARAM_T* init_param_ptr = PNULL;
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTPC_INIT_PARAM_T));
    
    if(0 != s_net_id)
    {
        init_param_ptr->net_id = s_net_id;
    }
    init_param_ptr->is_cookie_enable = FALSE;
	SCI_TRACE_LOW("[cookie] nps StartHttpRequest init_param_ptr->is_cookie_enable = FALSE; \n");
    init_param_ptr->is_cache_enable = FALSE;
    #ifndef WIN32
    HTTPC_InitRequest(MMI_MODULE_NPS, 1, init_param_ptr);
    #endif
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
}
#else
LOCAL void StartHttpRequest(void)
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    
    if(0 != s_net_id)
    {
        init_param_ptr->net_id = s_net_id;
    }
    init_param_ptr->is_cookie_enable = FALSE;
	SCI_TRACE_LOW("[cookie] nps StartHttpRequest init_param_ptr->is_cookie_enable = FALSE; \n");
    init_param_ptr->is_cache_enable = FALSE;
    HTTP_InitRequest(MMI_MODULE_NPS, 1, init_param_ptr);
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
}
#endif

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpInitCnf(MMINPS_HTTP_CONTEXT_T * http_context_ptr, DPARAM param)
{
#ifdef ENABLE_HTTPC
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    HTTPC_INIT_CNF_SIG_T signal = {0};
    HTTPC_GET_PARAM_T*   get_param_ptr = PNULL;
    //HTTP_APP_PARAM_T app_param = 0;
    uint16 len =0;
	uint16 param_len = 0;
	char*  param_url = PNULL;
#if 0 // for dc_debug	
	static uint16 swisrvcount = 0;
#endif	
    if(PNULL == http_context_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf PNULL == http_context_ptr return false");
        return MMI_RESULT_FALSE;
    }
    
    //---1--- 
    GetSignalStruct(param, &signal, sizeof(HTTPC_INIT_CNF_SIG_T));
    //app_param = signal.app_param;   //可以传app需要的参数过来。暂未用到。

    http_context_ptr->context_id   = signal.context_id;
    http_context_ptr->app_instance = 0;
    http_context_ptr->module_id    = signal.module_id;

    //---2---
    if (MMI_MODULE_NPS != http_context_ptr->module_id)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf MMI_MODULE_NPS != http_context_ptr->module_id return false");
        return MMI_RESULT_FALSE;
    }

    //---3---
    get_param_ptr = SCI_ALLOCA(sizeof(HTTPC_GET_PARAM_T));
    if(PNULL == get_param_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf PNULL == get_param_ptr return false");
        return MMI_RESULT_FALSE;
    }
    SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTPC_GET_PARAM_T));

	//URI
	param_url = MMINPS_GetHttpParamUrl();
	len = strlen(HTTP_URI_STRING) + strlen(param_url);
    get_param_ptr->uri.uri_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->uri.uri_ptr,HTTP_URI_STRING);
	strcat(get_param_ptr->uri.uri_ptr, param_url);
#if 0 // for dc_debug	
	swisrvcount++;
	if(swisrvcount==1);
	else if(swisrvcount==2)
		strcpy(get_param_ptr->uri.uri_ptr, "https://www.baidu.com/\0");
	else{ 
		strcpy(get_param_ptr->uri.uri_ptr, "https://www.sogou.com/\0");
		swisrvcount=0;
	}	
#endif	
#if 0 // for jw_debug
    strcpy(get_param_ptr->uri.uri_ptr, "https://test/\0");
#endif
	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf param_url = %s\n", param_url);
	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf get_param_ptr->uri.uri_ptr = %s\n", get_param_ptr->uri.uri_ptr);
	//accept
    len = strlen(ACCEPT_STRING);
    get_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->accept.accept_ptr,ACCEPT_STRING);
	//accept charset
    len = strlen(ACCEPT_CHARSET_STRING);
    get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
    strcpy(get_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);
    //accept language
    len = strlen(ACCEPT_LANGUAGE_STRING);
    get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
	//Connection
    get_param_ptr->connection = HTTPC_CONNECTION_KEEP_ALIVE;
    //UA
    len = strlen(USER_AGENT_STRING);
    get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->user_agent.user_agent_ptr,USER_AGENT_STRING);
    //buffer store;
    get_param_ptr->get_data.is_header_use_file = FALSE;
    get_param_ptr->get_data.style = HTTPC_DATA_TRANS_STYLE_BUFFER;
    //
    get_param_ptr->need_net_prog_ind = FALSE;

    //---4---
    #ifndef WIN32
    HTTPC_GetRequest(http_context_ptr->context_id, get_param_ptr, http_context_ptr->app_instance);
    #endif
#else
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_GET_PARAM_T*   get_param_ptr = PNULL;
    //HTTP_APP_PARAM_T app_param = 0;
    uint16 len =0;
	uint16 param_len = 0;
	char*  param_url = PNULL;

    if(PNULL == http_context_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf PNULL == http_context_ptr return false");
        return MMI_RESULT_FALSE;
    }
    
    //---1--- 
    GetSignalStruct(param, &signal, sizeof(HTTP_INIT_CNF_SIG_T));
    //app_param = signal.app_param;   //可以传app需要的参数过来。暂未用到。

    http_context_ptr->context_id   = signal.context_id;
    http_context_ptr->app_instance = 0;
    http_context_ptr->module_id    = signal.module_id;

    //---2---
    if (MMI_MODULE_NPS != http_context_ptr->module_id)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf MMI_MODULE_NPS != http_context_ptr->module_id return false");
        return MMI_RESULT_FALSE;
    }

    //---3---
    get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
    if(PNULL == get_param_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf PNULL == get_param_ptr return false");
        return MMI_RESULT_FALSE;
    }
    SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));

	//URI
	param_url = MMINPS_GetHttpParamUrl();
	len = strlen(HTTP_URI_STRING) + strlen(param_url);
    get_param_ptr->uri.uri_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->uri.uri_ptr,HTTP_URI_STRING);
	strcat(get_param_ptr->uri.uri_ptr, param_url);
	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf param_url = %s\n", param_url);
	SCI_TRACE_LOW("[NPS] HandleHttpInitCnf get_param_ptr->uri.uri_ptr = %s\n", get_param_ptr->uri.uri_ptr);
	//accept
    len = strlen(ACCEPT_STRING);
    get_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->accept.accept_ptr,ACCEPT_STRING);
	//accept charset
    len = strlen(ACCEPT_CHARSET_STRING);
    get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
    strcpy(get_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);
    //accept language
    len = strlen(ACCEPT_LANGUAGE_STRING);
    get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
	//Connection
    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
    //UA
    len = strlen(USER_AGENT_STRING);
    get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->user_agent.user_agent_ptr,USER_AGENT_STRING);
    //buffer store;
    get_param_ptr->get_data.is_header_use_file = FALSE;
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
    //
    get_param_ptr->need_net_prog_ind = FALSE;

    //---4---
    HTTP_GetRequest(http_context_ptr->context_id, get_param_ptr, http_context_ptr->app_instance);

#endif
	//---5---
	SCI_FREE(get_param_ptr->uri.uri_ptr);
    SCI_FREE(get_param_ptr->accept.accept_ptr);
    SCI_FREE(get_param_ptr->accept_charset.accept_charset_ptr);
    SCI_FREE(get_param_ptr->accept_language.accept_language_ptr);
    SCI_FREE(get_param_ptr->user_agent.user_agent_ptr);
    SCI_FREE(get_param_ptr);
	MMINPS_HttpParamUrlFree(param_url);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpRequestIdInd(MMINPS_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef ENABLE_HTTPC
    HTTPC_REQUEST_ID_IND_SIG_T signal = {0};

    if(PNULL == http_context_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpRequestIdInd PNULL == http_context_ptr return false");
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTPC_REQUEST_ID_IND_SIG_T));
#else
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};

    if(PNULL == http_context_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpRequestIdInd PNULL == http_context_ptr return false");
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_REQUEST_ID_IND_SIG_T));
#endif
    http_context_ptr->context_id = signal.context_id;
    http_context_ptr->request_id = signal.request_id;

    return result;
}

/*****************************************************************************/
//  Description : HandleHttpDataInd
//  Global resource dependence : 
//  Author: Glen Li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpDataInd(MMINPS_HTTP_CONTEXT_T * http_context_ptr, DPARAM param)
{

    //---1--- 
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
#ifdef ENABLE_HTTPC
    HTTPC_DATA_IND_SIG_T signal = {0};
    uint32 response_code = 0;
    uint32 data_len = 0;
    uint8 *data_ptr = PNULL;

    if(PNULL == http_context_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpDataInd PNULL == http_context_ptr return false");
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTPC_DATA_IND_SIG_T));
#else
	HTTP_DATA_IND_SIG_T signal = {0};
    uint32 response_code = 0;
    uint32 data_len = 0;
    uint8 *data_ptr = PNULL;

    if(PNULL == http_context_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpDataInd PNULL == http_context_ptr return false");
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_DATA_IND_SIG_T));
#endif 
    http_context_ptr->request_id = signal.request_id;
    http_context_ptr->context_id = signal.context_id;
    http_context_ptr->app_instance = signal.app_instance;
    
    response_code = signal.response_code;
        
    data_len = signal.data_len;
    data_ptr = signal.data_ptr;
	//SCI_TRACE_LOW("[ducheng] data_ptr:%s  data_len:%d\n",data_ptr,data_len);
    if(PNULL == s_nps_content_buffer)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpDataInd PNULL == s_nps_content_buffer return false");
        return MMI_RESULT_FALSE;
    }
	else
    {
		SCI_MEMSET(s_nps_content_buffer, 0, strlen(s_nps_content_buffer));
		data_len = MIN(data_len, NPS_HTTP_CONTENT_BUFFER_SIZE);
		SCI_MEMCPY(s_nps_content_buffer, data_ptr, data_len);
    }
    return result;
}

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpGetCnf(MMINPS_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef ENABLE_HTTPC
    HTTPC_GET_CNF_SIG_T signal = {0};

    if(PNULL == http_context_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpGetCnf PNULL == http_context_ptr return false");
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTPC_GET_CNF_SIG_T));
    http_context_ptr->context_id = signal.context_id;

    if(signal.result != HTTPC_SUCCESS)
    {
        MMINPS_SendMsgToApp(MSG_NPS_CONN_FAILED_CNF);
    }
    else
    {
        MMINPS_SendMsgToApp(MSG_NPS_PARSE_START_IND);
    }
#ifndef WIN32
    HTTPC_CloseRequest(http_context_ptr->context_id);
#endif
#else
    HTTP_GET_CNF_SIG_T signal = {0};

    if(PNULL == http_context_ptr)
    {
    	SCI_TRACE_LOW("[NPS] HandleHttpGetCnf PNULL == http_context_ptr return false");
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));
    http_context_ptr->context_id = signal.context_id;

    if(signal.result != HTTP_SUCCESS)
    {
        MMINPS_SendMsgToApp(MSG_NPS_CONN_FAILED_CNF);
    }
    else
    {
        MMINPS_SendMsgToApp(MSG_NPS_PARSE_START_IND);
    }

    HTTP_CloseRequest(http_context_ptr->context_id);

#endif
    return result;
}

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpErrorInd(MMINPS_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E            result      = MMI_RESULT_TRUE;
#ifdef ENABLE_HTTPC
    HTTPC_ERROR_IND_SIG_T    signal      = {0};
    HTTPC_CONTEXT_ID_T       context_id  = 0;
    HTTPC_ERROR_E            http_result = HTTPC_SUCCESS;         //http error
    
    GetSignalStruct(param, &signal, sizeof(HTTPC_ERROR_IND_SIG_T));
    context_id  = signal.context_id;
    http_result = signal.result;
    
    if(signal.result != HTTPC_SUCCESS)
    {
        MMINPS_SendMsgToApp(MSG_NPS_CONN_FAILED_CNF);
    }
    #ifndef WIN32
    HTTPC_CloseRequest(context_id); 
    #endif
#else
    HTTP_ERROR_IND_SIG_T    signal      = {0};
    HTTP_CONTEXT_ID_T       context_id  = 0;
    HTTP_ERROR_E            http_result = HTTP_SUCCESS;         //http error
    
    GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));
    context_id  = signal.context_id;
    http_result = signal.result;
    
    if(signal.result != HTTP_SUCCESS)
    {
        MMINPS_SendMsgToApp(MSG_NPS_CONN_FAILED_CNF);
    }
    
    HTTP_CloseRequest(context_id); 
#endif
    return result;
}

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpCloseCnf(MMINPS_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //成功关闭HTTP
    http_context_ptr->app_instance = 0;
    http_context_ptr->context_id   = 0;
    http_context_ptr->request_id   = 0;
    http_context_ptr->module_id    = 0;

    MMINPS_SendMsgToApp(MSG_NPS_HTTP_CLOSE_CNF);
    return result;
}

/*****************************************************************************/
// 	Description : 获得天气信息
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
LOCAL char* MMINPS_GetDataInformation(void)
{
    return s_nps_content_buffer;
}

/*****************************************************************************/
// 	Description : 天气信息BUFFER 分配
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_DataInfoAlloc(void)
{
    if(PNULL == s_nps_content_buffer)
    {
        s_nps_content_buffer = SCI_ALLOC_APPZ(NPS_HTTP_CONTENT_BUFFER_SIZE);
    }

    if(PNULL != s_nps_content_buffer)
    {
        SCI_MEMSET(s_nps_content_buffer, 0, NPS_HTTP_CONTENT_BUFFER_SIZE);     
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : 天气信息BUFFER 释放
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
LOCAL void MMINPS_DataInfoRelease(void)
{
    if(PNULL != s_nps_content_buffer)
    {
        SCI_FREE(s_nps_content_buffer);
		s_nps_content_buffer = PNULL;  
    }
}

/*****************************************************************************/
//  Description : Start parse
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_ParseStart(void)
{
	BOOLEAN result      = FALSE;
    char* buf_ptr = PNULL;
	int32 buf_len = 0;
    
    buf_ptr = MMINPS_GetDataInformation();
	SCI_TRACE_LOW("[NPS] MMINPS_ParseStart buf_ptr = %s\n ", buf_ptr);

    if(PNULL != buf_ptr)
    {
		MMINPS_PARSE_TEMP_T temp_data = {0};
		SCI_MEMSET(&temp_data, 0, sizeof(MMINPS_PARSE_TEMP_T));
		//is_bad_data默认为TRUE,在解析过程中，
		//只要收到XML_TAG_RESULT，则会置为FALSE，正常情况都为FALSE
		temp_data.is_bad_data = TRUE;
		buf_len = strlen(buf_ptr);
		if(Xml_ParseData((uint8*)buf_ptr, buf_len, &temp_data))
		{
		    result = MMINPS_HandleParsedData(&temp_data);
		}
    }

    if(result)
    {
        MMINPS_SendMsgToApp(MSG_NPS_PARSE_SUCCESS_CNF);
    }
    else
    {
        MMINPS_SendMsgToApp(MSG_NPS_PARSE_FAILED_CNF);
    }
 
    return result;
}

/*****************************************************************************/
//  Description : Do parse data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Xml_ParseData(
                            uint8* buf_ptr,                         //[in] Data buffer to paser
                            int32 buf_len,                         //[in] Data length to paser
                            MMINPS_PARSE_TEMP_T *parse_data_ptr //[out] Data get
                            )
{
#ifdef XML_SUPPORT
    BOOLEAN result = FALSE;
    CFLXML_PARSER_T* parser_ptr = PNULL;
	
    parser_ptr = CFLXML_CreateParserUTF8( Xml_StartElementHandler, 
                                      Xml_EndElementHandler, 
                                      Xml_CharacterDataHandler, 
                                      PNULL,//PARSER_SEP_CHAR, 
                                      (void*)parse_data_ptr);
    
    if(PNULL != parser_ptr )
    {                
        //开始解析       
        if(CFLXML_ERR_OK == CFLXML_Parse( parser_ptr, buf_ptr, buf_len, TRUE))
        {
            //数据解析成功
            result = TRUE;
        }

        //结束解析       
        CFLXML_FreeParser( parser_ptr );
    }
	
	SCI_TRACE_LOW("[NPS] Xml_ParseData return %d", result);
    return result;
#endif
}

/*****************************************************************************/
//  Description : start element handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Xml_StartElementHandler(
                        void        *user_data_ptr,
                        const char  *localname_ptr,
                        const char  **attributes_pptr
                        )
{
    
    int32 tag_id = XML_TAG_MAX;
    MMINPS_PARSE_TEMP_T *parse_data_ptr = (MMINPS_PARSE_TEMP_T *)user_data_ptr;
    
    //MMIAPICOM_WstrTraceOut(localname_ptr, MMIAPICOM_Wstrlen(localname_ptr));

    // 设置解析的属性
    while( --tag_id )
	{
		if( 0 == MMIAPICOM_Stricmp((const uint8*)localname_ptr, (const uint8*)s_nps_xml_tag_arr[tag_id])  )
		{
            break;
		}
	}
    
	parse_data_ptr->cur_tag = tag_id;
    
    Xml_GetElementAttributes( parse_data_ptr, parse_data_ptr->cur_tag);
}

/*****************************************************************************/
//  Description : end element handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Xml_EndElementHandler(
                        void         *user_data_ptr,
                        const uint8  *localname_ptr
                        )
{		
    //CFLWCHAR_StrDump( localname_ptr );	         	    
}

/*****************************************************************************/
//  Description : character date handler
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void Xml_CharacterDataHandler(
                                 void         *user_data_ptr,
                                 const uint8  *ch_ptr 
                                 )
{
	MMINPS_PARSE_TEMP_T *parse_data_ptr = (MMINPS_PARSE_TEMP_T *)user_data_ptr;
    
	Xml_GetElementData( parse_data_ptr, parse_data_ptr->cur_tag, ch_ptr);
}

/*****************************************************************************
// 	Description : Set element's attributes
//	Global resource dependence : 
//  Author:Gaily.Wang
//	Note: 
*****************************************************************************/
LOCAL void Xml_GetElementAttributes(
                                MMINPS_PARSE_TEMP_T *parse_data_ptr,
                                XML_TAG_E           tag
                                )
{
	switch(tag)
	{
	case XML_TAG_RESPONSE:
		{
		    parse_data_ptr->is_bad_data = FALSE;
		}
		break;
    
	default:
		{
		    //unknow or unconcerned tag.
		}
		break;
	}
}

/*****************************************************************************
// 	Description : Set element's attributes
//	Global resource dependence : 
//  Author:Gaily.Wang
//	Note: 
*****************************************************************************/
LOCAL void Xml_GetElementData(
                                MMINPS_PARSE_TEMP_T *parse_data_ptr,
                                XML_TAG_E          tag, 
                                const uint8        *ch_ptr
                                )
{
    wchar** data_ptr = PNULL;
    uint16 str_len = 0;
	
    switch(tag)
    {
    case XML_TAG_STATUS:
        {
            data_ptr = &parse_data_ptr->data_info.status;
        }
        break;
    case XML_TAG_MESSAGE:
        {
            data_ptr = &parse_data_ptr->data_info.message;
        }
        break;
    case XML_TAG_CODE:
        {
            data_ptr = &parse_data_ptr->data_info.code;
        }
        break;
    case XML_TAG_ROWID:
        {
            data_ptr = &parse_data_ptr->data_info.rowId;
        }
    	break;
    default:
        {
            //unknow or unconcerned tag.
            return;
        }
        break;
    }

	if(data_ptr != PNULL)
    {
		SCI_TRACE_LOW("[NPS] Xml_GetElementData cur_tag = %d", tag);

		if(MMIAPICOM_Wstrlen(*data_ptr))
		{
			SCI_TRACE_LOW("[NPS] Xml_GetElementData this tag already parsed and return");
			return;
		}
		
        if (ch_ptr != PNULL)
        {
            str_len = strlen(ch_ptr);
            str_len = MIN(str_len, XML_TAG_MAX_LEN);
            *data_ptr = SCI_ALLOC_APPZ((str_len + 1) * sizeof(wchar));
            MMIAPICOM_StrToWstr(ch_ptr, *data_ptr);
			MMIAPICOM_WstrTraceOut(*data_ptr, str_len);
        }
    }
}

/*****************************************************************************/
//  Description : Handle parsed data
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_HandleParsedData(MMINPS_PARSE_TEMP_T *temp_data)
{
    BOOLEAN result    = TRUE;

	//MMK_CloseWin(MMINPS_WAIT_WIN_ID);
	
    if( PNULL == temp_data )
    {
            result = FALSE;
    }
    else
    {
		SCI_TRACE_LOW("[NPS] MMINPS_HandleParsedData temp_data->is_bad_data = %d", temp_data->is_bad_data);
		MMIAPICOM_WstrTraceOut(temp_data->data_info.status, MMIAPICOM_Wstrlen(temp_data->data_info.status));
		
        if(temp_data->is_bad_data)
        {
			//MMIPUB_OpenAlertFailWin(TXT_COMMON_FORMAT_ERROR);
            result = FALSE;
        }
        else
        {
			wchar  status_str[]=L"OK";
//			MMI_STRING_T status_str = {0};

//			MMI_GetLabelTextByLang(STXT_SOFTKEY_OK_MK, &status_str);status_str.wstr_ptr
			if(0 == MMIAPICOM_Wstrcmp(status_str, temp_data->data_info.status))
			{
				//MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
				result = TRUE;
			}
			else
			{
				//MMIPUB_OpenAlertFailWin(TXT_ERROR);;
			    result = FALSE;
			}
		}
    }
	
	SCI_TRACE_LOW("[NPS] MMINPS_HandleParsedData return %d", result);
    return result;
}

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size));
    }
}

/*****************************************************************************/
//  Description : Open Popup win
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL void MMINPS_SendMsgToApp(uint32 sig_id)
{
    MmiSignalS *signal = PNULL;
    
    //creat signal
    MMI_CREATE_SIGNAL(signal,(uint16)sig_id, sizeof(MmiSignalS), P_APP);

    //send signal
    MMI_SEND_SIGNAL(signal, P_APP);
}
