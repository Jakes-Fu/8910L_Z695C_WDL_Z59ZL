/*****************************************************************************
** File Name:      netproxy_text.h                                            *
** Author:         Jiaoyou.wu                                                    *
** Date:           03/27/2015                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser text resource   *
*****************************************************************************/
#ifndef NET_PROXY_TEXT_H_
#define NET_PROXY_TEXT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

typedef enum NETPROXY_MMI_TEXT_ID_E
{
    NETPROXY_TXT_NULL = ( MMI_MODULE_NET_PROXY << 16 ) | MMI_SYS_RES_FLAG,
#include "netproxy_mdu_def.h"
    NETPROXY_TXT_MAX
} NETPROXY_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // BROWSER_TEXT_H_
