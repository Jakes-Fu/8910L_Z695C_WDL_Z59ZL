/*****************************************************************************
** File Name:      http_hsm.h                                                *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP state machine      *
*****************************************************************************/

#ifndef _http_hsm_H_
#define _http_hsm_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "http_event.h"
#include "netmgr_api.h"
#include "sci_types.h"
#include "http_api.h"
#include "http_def.h"
#include "ssl_api.h"
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

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create http hsm
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC PROTOCOL_HOST_HANDLE HTTPHSM_Create(NETMGR_HOST_HANDLE host_handle);

/*****************************************************************************/
//  Description : destroy http hsm
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTPHSM_Destroy(PROTOCOL_HOST_HANDLE hsm_handle);

/*****************************************************************************/
//  Description : dispatch signal to http hsm
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTPHSM_DispatchSignal(HTTP_HSM_T* hsm_ptr, HSMEvent_T* sig_ptr);

/*****************************************************************************/
//  Description : http hsm deal http request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTPHSM_DealRequest(PROTOCOL_REQUEST_HANDLE request_handle);

/*****************************************************************************/
//  Description : http trans callback
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_TransCallback(
                    NETTRANS_EVENT_E                event_id,
                    NETMGR_HOST_HANDLE              host_handle,
                    const NETMGR_TRANS_RESULT_T*    result_ptr
                    );

/*****************************************************************************/
//  Description : http trans send callback
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_TransSendCallback(uint32 send_len,NETMGR_HOST_HANDLE host_handle);

/*****************************************************************************/
//  Description : the HTTP idle state 
//  Global resource dependence :
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HSM_STATE_T Http_Idle (HTTP_HSM_T* this_ptr, HSMEvent_T const* e_ptr);

PUBLIC HSM_STATE_T Http_ReceivingAuthPending (HTTP_HSM_T* this_ptr, HSMEvent_T const* e_ptr);

/*****************************************************************************/
//  Description : the call back fun of security module's message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSecurityPostMessage(HTTP_HSM_T* this_ptr, uint32 message_id);

/*****************************************************************************/
//  Description : handle the receive data which has been encripty
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSecurityRecvDataOut(HTTP_HSM_T* this_ptr, char* data_ptr, uint32 data_len);

/*****************************************************************************/
//  Description : handle the receive data which has been encripty
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSecuritySendDataOut(HTTP_HSM_T* this_ptr, char* data_ptr, uint32 data_len);

/*****************************************************************************/
//  Description : handle the show cert info
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSecurityShowCertInfo(HTTP_HSM_T* this_ptr);

/*****************************************************************************/
//  Description : init http
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_Init(void);

/*****************************************************************************/
//  Description : get http cancel info
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_GetCancelInfo(xSignalHeaderRec *sig_ptr,
								NETMGR_HOST_HANDLE *host_handle_ptr,
								PROTOCOL_REQUEST_HANDLE *pro_request_handle_ptr
								);

/*****************************************************************************/
//  Description : get http close info
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_GetCloseInfo(xSignalHeaderRec *sig_ptr,NETMGR_INSTANCE_HANDLE *netmgr_handle_ptr);

/*****************************************************************************/
//  Description : get http init info
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_GetInitInfo(
							  xSignalHeaderRec* sig_ptr,
								NETMGR_PROXY_INFO_T* proxy_info_ptr,
								uint32					*net_id_ptr //[OUT]
								);

/*****************************************************************************/
//  Description : get http request info
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_GetRequestInfo(
								 	xSignalHeaderRec* sig_ptr,                  //[IN]
									char** url_ptr,                             //[OUT]
									NETMGR_INSTANCE_HANDLE* netmgr_handle_ptr  //[OUT]
								 );

/*****************************************************************************/
//  Description : send sig error cnf
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_SigErrCnf(xSignalHeaderRec* sig_ptr);

/*****************************************************************************/
//  Description : disconncet current  socket connection
//  Global resource dependence : 
//  Author: sally.he
//  Note: 
/*****************************************************************************/

PUBLIC void HttpSendCnfToApp(HTTP_REQUEST_T* request_ptr, HTTP_RSP_HEADER_INFO_T rsp_header_info, HTTP_ERROR_E error_code);

PUBLIC BOOLEAN    HTTP_DisconnectSocket(HTTP_HSM_T* hsm_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
