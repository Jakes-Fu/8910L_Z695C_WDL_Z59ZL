/*****************************************************************************
** File Name:      http_cookie_api.h                                         *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP cookie api         *
*****************************************************************************/

#ifndef _http_cookie_api_H_
#define _http_cookie_api_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "netmgr_api.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define HTTP_SDCARD_MAX_NUM  1

//HTTP MM DEVICE 
typedef enum
{
    HTTP_DEVICE_SYSTEM,   
    HTTP_DEVICE_UDISK,    
    HTTP_DEVICE_SDCARD,   
    HTTP_DEVICE_SDCARD_1, 
    HTTP_DEVICE_NUM = HTTP_SDCARD_MAX_NUM + 2
} HTTPMM_DEVICE_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC BOOLEAN HttpCookie_Init();

PUBLIC BOOLEAN HttpCookie_Exit();

  /*****************************************************************************/
//  Description : add http cookie header
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC void  HttpAddCookieHead( HTTP_REQUEST_T* request,  char**buf, int32* buf_size);

/*****************************************************************************/
//  Description : create http cookie node
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_COOKIE_NODE_T* HttpCookieNodeCreate(HTTP_COOKIE_PARAM_T * param);

/*****************************************************************************/
//  Description : destroy cookie node
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpCookieNodeDestroy(HTTP_COOKIE_NODE_T *node);

/*****************************************************************************/
//  Description : parse http cookie
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_COOKIE_NODE_T* HttpCookieParse(char* line, char* domain, NETMGR_PORT_T port, char* path);

/*****************************************************************************/
//  Description : initialize http cookie
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCookieLoad();

/*****************************************************************************/
//  Description : finalize http cookie
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCookieSave();

/*****************************************************************************/
//  Description : initialize http cookie
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpCookieDump();

/*****************************************************************************/
//  Description : if cookie is enable
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpCookieIsEnable();

/*****************************************************************************/
//  Description : set cookie enable
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCookieSetEnable(BOOLEAN is_enable);

/*****************************************************************************/
//  Description : set cookie file
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetCookieFile(HTTP_UNICODE_T* file_name_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif