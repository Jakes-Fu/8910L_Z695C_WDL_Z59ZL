/*****************************************************************************
** File Name:      http_auth.h                                               *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP auth               *
*****************************************************************************/

#ifndef _http_auth_H_
#define _http_auth_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "http_def.h"
#include "cfl_md5.h"
#include "cfl_base64.h"
#include "http_util.h"
#include "http_api.h"
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
#define HTTP_AUTH_INT_STRING "auth-int"
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//auth
#define HTTP_HASH_HEX_LEN 32
#define HTTP_HASHLEN 16
typedef char HTTP_HASH[HTTP_HASHLEN];
#define HTTP_HASHHEXLEN 32
typedef char HTTP_HASHHEX[HTTP_HASHHEXLEN +1];
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create response authen digest
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_RESPONSE_DIGEST_T* HttpAuthenDigestResponseCreate(void);

/*****************************************************************************/
//  Description : destroy response authen digest
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpAuthenDigestResponseDestroy(HTTP_RESPONSE_DIGEST_T* digest_ptr);

/*****************************************************************************/
//  Description : create request authen digest
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_REQUEST_DIGEST_T* HttpAuthenDigestRequestCreate(void);

/*****************************************************************************/
//  Description : destroy request authen digest
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpAuthenDigestRequestDestory(HTTP_REQUEST_DIGEST_T* digest_ptr);

/*****************************************************************************/
//  Description : deal basic encode
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpAuthenBasicEncode(char* user_name_ptr,char* password_ptr);

/*****************************************************************************/
//  Description : deal digest encode
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpAuthenDigestEncode(HTTP_REQUEST_T* request_ptr,char* user_name_ptr,char* password_ptr,uint8* entity_ptr,uint32 entity_len);

/*****************************************************************************/
//  Description : digest calc ha1
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpAuthenDigestCalcHA1(HTTP_DIGEST_CALC_A1 calc_a1, 
                                    HTTP_HASHHEX HA1
                                    );

/*****************************************************************************/
//  Description : digest calc response
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpAuthenDigestCalcResult(
    HTTP_HASHHEX HA1,
    HTTP_DIGEST_CALC_RESPONSE calc_response,
    HTTP_HASHHEX entity,
    HTTP_HASHHEX digest_result
);

/*****************************************************************************/
//  Description : set digest request param
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpAuthenDigestRequsetSetParam(HTTP_REQUEST_T *request_ptr, char *name_ptr, char *password_ptr);

/*****************************************************************************/
//  Description : parse the information of the digest authentication
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpAuthenDigestResponseParse(HTTP_RESPONSE_T* response_ptr, char* value_ptr);

/*****************************************************************************/
//  Description : save the information of digest authentication 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpAuthenDigestResponseSaveParam(HTTP_RESPONSE_T* response_ptr, char* param_ptr);

/*****************************************************************************/
//  Description : get the authentication value of the request 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpAuthenGetInfo(HTTP_REQUEST_T* request_ptr, HTTP_AUTH_TYPE_E type);

/*****************************************************************************/
//  Description : convert 4 bytes to HEX
//  Global resource dependence :
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpCvtHex(HTTP_HASH Bin,HTTP_HASHHEX Hex);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
