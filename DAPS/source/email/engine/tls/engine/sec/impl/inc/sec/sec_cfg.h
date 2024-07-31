/*==================================================================================================
    FILE NAME : sec_cfg.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file,define the macro used in the sec module, and these macros can control
        the features of sec module.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                            Tracking
    Date              Author               Number        Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-06-29   Kevin Yang                            create sec_cfg.h
    
==================================================================================================*/

/***************************************************************************************************
*   multi-Include-Prevent Section
***************************************************************************************************/
#ifndef SEC_CFG_H
#define SEC_CFG_H

/*************************************************
*  Global Macro
**************************************************/
/*************************************************************************
The following macro is used to control the common features of the sec 
packet,and can be modified based on requirements.
*************************************************************************/
#define M_SEC_CFG_WTLS_CLASS_2
/*#define M_SEC_CFG_WTLS_CLASS_3*/

#define M_SEC_CFG_TLS 
//#define M_SEC_CFG_WTLS

/*add by tang 070628 for no platform os:no file operation,no session*/
#define M_SEC_CFG_NO_PLATFORM  

/*************************************************************************
Define  whether to send message to wtls& signtext  
*************************************************************************/
/* without UE */
#define M_SEC_CFG_CONN_WITHOUT_CERT_UE

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
/* no confirm */
#define M_SEC_WTLS_NO_CONFIRM_UE
 
/*************************************************************************
Define whether to send warnings event to the application.
*************************************************************************/
/*#define M_SEC_CFG_SHOW_WARNINGS*/

#endif

/*************************************************************************
Define the number of sessions&peer that can be saved data to.
*************************************************************************/
#define M_SEC_SESSION_CACHE_SIZE    10
#define M_SEC_PEER_CACHE_SIZE          10

/*************************************************************************
Define some algorithm used in sec packet
*************************************************************************/
#define M_SEC_CFG_RC5_CBC
#define M_SEC_CFG_3DES_CBC_EDE
#define M_SEC_CFG_MD2
#define M_SEC_CFG_3DES_CBC_EDE_INTERN
#ifdef M_SEC_CFG_TLS
#define M_SEC_CFG_RC4
#define M_SEC_CFG_RC2 
#define M_SEC_CFG_DES40
#define Test_M_SEC_CFG_3DES_CBC_EDE
#define M_SEC_CFG_DHE_RSA 

#endif 
/*************************************************
*  Module Macro
**************************************************/

/*************************************************************************
Define the maximum number of certificates to be sent in TrustedKeyIds 
in wtls. used in iwim.c
*************************************************************************/
#define M_SEC_MAX_NBR_OF_CERTS_IN_TRUSTED_KEY_IDS 60

#endif /* end of SEC_CFG_H */





















