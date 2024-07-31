/*****************************************************************************
** File Name:      http_response.h                                           *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP response           *
*****************************************************************************/

#ifndef _http_response_H_
#define _http_response_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
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
//  Description : create http response
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_RESPONSE_T* HTTP_CreateResponse(HTTP_HSM_T* hsm_ptr);

/*****************************************************************************/
//  Description : destroy http response
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HTTP_DestroyResponse(HTTP_RESPONSE_T* response_ptr);

/*****************************************************************************/
//  Description : parse http response header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpResponseParseHead(HTTP_RESPONSE_T* response_ptr);

/*****************************************************************************/
//  Description : parse http response authentication proxy header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseAuthecticationProxyHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response authentication www header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseAuthenticationWWWHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response cache header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseCacheHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response content encoding header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseContentEncodingHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response content length header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseContentLengthHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response content type header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseContentTypeHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response location header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseLocationHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response set cookie header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseSetCookieHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response transfer encoding header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseTransferEncodingHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response x sp result header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseXSpResultHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response connection header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseConnectionHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response etag header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseEtagHeader(void* data_ptr, char* token_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : parse http response last-modofied header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseLastModifiedHeader(void* data_ptr, char* token_ptr, char* value_ptr);

PUBLIC HTTP_ERROR_E HttpParseResponseRefreshHeader(
                                                      void* data_ptr,   //[IN][OUT]
                                                      char* token_ptr,                 //[IN]
                                                      char* value_ptr                  //[IN]
                                                      );
/*****************************************************************************/
//  Description : parse http response expires header
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseExpiresHeader(
                                                      void* data_ptr,   //[IN][OUT]
                                                      char* token_ptr,                 //[IN]
                                                      char* value_ptr                  //[IN]
                                                      );
                                                      
/*****************************************************************************/
//  Description : parse http response BITS-Session-Id header
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpParseResponseSessionIdHeader(void* data_ptr, char* token_ptr, char* value_ptr);                                                      
                                                      
PUBLIC void HTTP_FormatUrl(char **dst_pptr, const char *src_ptr);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif