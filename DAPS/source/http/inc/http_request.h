/*****************************************************************************
** File Name:      http_request.h                                            *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP request            *
*****************************************************************************/

#ifndef _http_request_H_
#define _http_request_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "netmgr_api.h"
#include "http_def.h"
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
//  Description : create http request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC PROTOCOL_REQUEST_HANDLE HTTP_CreateRequest (NETMGR_REQUEST_HANDLE net_request_handle, const NETMGR_URL_T *url_ptr, xSignalHeaderRec* sig_ptr);

/*****************************************************************************/
//  Description : destroy http request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_DestroyRequest(PROTOCOL_CONTEXT_HANDLE request_handle);

/*****************************************************************************/
//  Description : if the http request is security
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpRequestIsSecurity(HTTP_REQUEST_T* request_ptr);

/*****************************************************************************/
//  Description : format http request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpRequestFormat(HTTP_REQUEST_T* request_ptr, char **data_ptr, uint32 *data_len_ptr);

/*****************************************************************************/
//  Description : find http request node by request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpRequestFindNodeByRequest(
                                 CFL_NODE_OBJECT node, 
                                 void* request_ptr    //[IN]
                                 );

/*****************************************************************************/
//  Description : get http request list handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC CFL_LIST_HANDLE HttpGetRequestListHandle(void);

/*****************************************************************************/
//  Description : update http request
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_UpdateRequest(
                        NETMGR_REQUEST_HANDLE	netmgr_request_handle,	//in: netmanager request handler
                        PROTOCOL_REQUEST_HANDLE pro_request_handle,		//in: protocol request
                        NETMGR_URL_T			*new_url_ptr,			//in: url
                        NETMGR_HOST_HANDLE		host_handler			//in: host handler
                        );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif