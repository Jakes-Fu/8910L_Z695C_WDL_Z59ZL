/*****************************************************************************
** File Name:      http_header.h                                             *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP header             *
*****************************************************************************/

#ifndef _http_header_H_
#define _http_header_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "http_request.h"
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
#define HTTP_HEADER_NODE_MAX_NUM 50
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: li.li
//  Note: 
/*****************************************************************************/

/*****************************************************************************/
//  Description : set accept header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetAcceptHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set accept charset header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetAcceptCharsetHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set accept encoding header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetAcceptEncodingHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set accept language header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetAcceptLanguageHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set connection header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetConnectionHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set referer header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetRefererHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set user agent header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetUserAgentHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set ua profile header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetUAProfileHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set If-Match header
//  Global resource dependence : 
//  Author: mingzhe.jin
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetIfMatchHeader(HTTP_REQUEST_T* request_ptr,char* value_ptr);

/*****************************************************************************/
//  Description : set header field
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetHeaderField(HTTP_REQUEST_T* request_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : set content type header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetContentTypeHeader(HTTP_REQUEST_T* request_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : destroy header list
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpDestroyHeaderList(HTTP_HEADER_FIELD_T* header_list_ptr);

/*****************************************************************************/
//  Description : get header field
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpRequestGetHeaderField(HTTP_REQUEST_T* request_ptr, char* token_ptr, char** value_ptr);

/*****************************************************************************/
//  Description : http save data to file
//  Global resource dependence : 
//  Author: sally.he
//  Note: 
/*****************************************************************************/
PUBLIC void HttpSaveDataToFile(char* buff_ptr, int32 len);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif