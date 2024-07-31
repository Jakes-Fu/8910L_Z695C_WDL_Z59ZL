/*****************************************************************************
** File Name:      http_signal.h                                             *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP state machine      *
*****************************************************************************/
 
#ifndef _HTTP_SIGNAL_H
#define _HTTP_SIGNAL_H

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "http_util.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 ** SIGNAL MACRO: 
 **         HTTP_GET_SIGCODE
 **         HTTP_GET_SIGSENDER
 **         HTTP_CREATE_SIGNAL
 **         HTTP_SEND_SIGNAL
 **         HTTP_RECEIVE_SIGNAL
 **         HTTP_PEEK_SIGNAL
 **         HTTP_FREE_SIGNAL
 **---------------------------------------------------------------------------*/

/******************************************************************************/
/*! \brief Send a signal.
 *  \author lili
 *  \param[in]	_SIG_PTR        Pointer to the signal which will be sent
 *  \param[in]	_RECEIVER       ID of thread whihc receives this signal.
 *  \return None.
 */
/******************************************************************************/

/******************************************************************************/
/*! \brief Receive a signal.
 *  \author lili
 *  \param[in]	_SIG_PTR        Pointer to a signal to save received signal.
 *  \param[in]	_SELF_ID        ID of thread which receives this signal.
 *  \return None.
 */
/******************************************************************************/
#define HTTP_RECEIVE_SIGNAL(_SIG_PTR, _SELF_ID) \
		_SIG_PTR = SCI_GetSignal(_SELF_ID); 

/******************************************************************************/
/*! \brief Free a signal.
 *  \author lili
 *  \param[in]	_SIG_PTR        Pointer to the signal which will be freed.
 *  \return None   
 */
/******************************************************************************/
#define HTTP_FREE_SIGNAL(_SIG_PTR) \
        HTTP_FREE(_SIG_PTR)
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC HTTP_ERROR_E HttpSendSignal(xSignalHeader signal_ptr,BLOCK_ID thread_id);

/*****************************************************************************/
//  Description : create and send net indication to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendNetIndToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id, NET_PROGRESS_E progress,uint32 total_len,uint32 curr_len);

/*****************************************************************************/
//  Description : create and send data indication to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendDataIndToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id,uint32 len,char* data_ptr,uint32 response_code);

/*****************************************************************************/
//  Description : create and send header indication to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendHeaderIndToApp(char* header_ptr,HTTP_REQUEST_PARAM_T request_param,uint32 request_id, HTTP_RSP_HEADER_INFO_T rsp_header_info);

/*****************************************************************************/
//  Description : create and send http read cache result to net manager 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendReadCacheResult(HTTP_REQUEST_PARAM_T request_param,uint32 request_id, uint16 signal_code);

/*****************************************************************************/
//  Description : create and send header indication to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendHeadCnfToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id);

/*****************************************************************************/
//  Description : create and send http error to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendErrorIndToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id,HTTP_ERROR_E error_code);

/*****************************************************************************/
//  Description : create and send http cancel cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendCancelCnfToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id,HTTP_ERROR_E error_code);

/*****************************************************************************/
//  Description : create and send http get cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendGetCnfToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id,HTTP_RSP_HEADER_INFO_T rsp_header_info,HTTP_ERROR_E error_code);

/*****************************************************************************/
//  Description : create and send http get cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendPostCnfToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id,HTTP_RSP_HEADER_INFO_T rsp_header_info,HTTP_ERROR_E error_code);

/*****************************************************************************/
//  Description : create and send http need auth to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendNeedAuthToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id,HTTP_AUTH_TYPE_E auth_type);

/*****************************************************************************/
//  Description : create and send http init cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendInitCnfToApp(xSignalHeaderRec* e_ptr, HTTP_CONTEXT_ID_T context_id,HTTP_ERROR_E error_code);

/*****************************************************************************/
//  Description : create and send http close cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendCloseCnfToApp(BLOCK_ID task_id, HTTP_CONTEXT_ID_T context_id,HTTP_ERROR_E error_code);

/*****************************************************************************/
//  Description : create and send http clear cookie cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendCookieClearCnfToApp(BLOCK_ID task_id, HTTP_CONTEXT_ID_T context_id,HTTP_ERROR_E error_code);

/*****************************************************************************/
//  Description : create and send http close cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendRequestIndToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id);

/*****************************************************************************/
//  Description : create and send http ssl cert ind to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendSslCertIndToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id);

/*****************************************************************************/
//  Description : create and send http redirect ind to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendRedirectIndToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id, char* redirect_ptr);

/*****************************************************************************/
//  Description : create and send http cache delete cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendCacheDeleteCnfToApp(BLOCK_ID task_id, HTTP_CONTEXT_ID_T context_id,HTTP_ERROR_E error_code);
/*****************************************************************************/
//  Description : create and send http cookie set cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendCookieSetCnfToApp(HTTP_REQUEST_PARAM_T request_param,HTTP_ERROR_E error_code);
/*****************************************************************************/
//  Description : create and send clear cache to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendClearCacheCnfToApp(HTTP_ERROR_E result,BLOCK_ID task_id);
/*****************************************************************************/
//  Description : create and send options cnf to app 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendOptionsCnfToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id,HTTP_RSP_HEADER_INFO_T rsp_header_info,HTTP_ERROR_E error_code);
/*****************************************************************************/
//  Description : create and send delete cnf to app 
//  Global resource dependence : 
//  Author: mingzhe.jin
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSendDeleteCnfToApp(HTTP_REQUEST_PARAM_T request_param,uint32 request_id,HTTP_RSP_HEADER_INFO_T rsp_header_info,HTTP_ERROR_E error_code);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _MMI_SIGNAL_H