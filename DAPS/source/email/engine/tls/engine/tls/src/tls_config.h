/*=====================================================================================
    FILE NAME   : wap_config.h
    MODULE NAME : WAP
    
    PRE-INCLUDE FILES DESCRIPTION
        we_def.h
        
    GENERAL DESCRIPTION
        In this file,define the function config. 
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification Tracking
    Date       Author       Number    Description of changes
    ---------- ------------ --------- --------------------------------------------
    2006-06-05 ChenZhiFeng  none      Init
    2007-03-08 HouGuanhua   none      G_WAP_CFG_MAX_SOCKETS,G_WAP_CFG_MAX_CONNECTIONS = 10
    2007-03-13 HouGuanhua   none      add feature macro define for customize wap
    
=====================================================================================*/

/**************************************************************************************
*   Multi-Include-Prevent Section
**************************************************************************************/
#ifndef WAP_CONFIG_H
#define WAP_CONFIG_H
#include "we_cfg.h"
#define G_WAP_CFG_MAX_CONNECTIONS                   10         /* The maximum number of connections supported. */

#define WAP_CONFIG_APP_MANAGE_CERTIFICATE
#define WAP_TLS_SUPPORT_VIEW_SVR_CERT
#define WAP_TLS_SUPPORT_USER_ASSIGN_PRO_VERSION

#define WAP_CONFIG_TLS
#define WAP_CONFIG_SSL

/* If defined, WAP stack will report error code to application.2006-6-5 */
//#define G_WAP_CFG_REPORT_DATA_ERRORS
#ifdef WAP_CONFIG_TLS
#define G_WAP_CFG_TLS_MAX_RECORD_LENGTH   5192
#endif
#define MAX_SERVER_CERT_NUM  60



#endif  /* WAP_CONFIG_H */
