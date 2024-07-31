/*==================================================================================================
    FILE NAME : sec_ast.h
    MODULE NAME : SEC
     
    GENERAL DESCRIPTION
        In this file,define the string function prototype.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                             Tracking
    Date              Author               Number           Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07   Kevin Yang                            create sec_ast.h
    2007-06-19   bluefire                              four bytes align for dragonfly
==================================================================================================*/

/*******************************************************************************
*   multi-Include-Prevent Section
*******************************************************************************/
#ifndef _SEC_AST_H
#define _SEC_AST_H

#include "we_cfg.h"

#ifdef WE_CFG_BREW_PLATFORM
#include "AEEStdlib.h"
#else
#ifdef WIN32_DEBUG
#include "stdlib.h"
#include "time.h"
#else
#include "tf3rd_for_HZ.h"
#endif
#endif
/*******************************************************************************
*   macro Define Section
*******************************************************************************/
#define WE_SIGNAL_DESTRUCT      Sec_FreeMsg
#define SEC_CURRENT_TIME        Sec_TimeGetCurrent

#ifdef WE_CFG_BREW_PLATFORM


#define SEC_SPRINTF     SPRINTF
#define SEC_STRCAT      STRCAT
#define SEC_STRRCHR     STRRCHR
#define SEC_STRCHR      STRCHR
#define SEC_STRCPY      STRCPY
#define SEC_STRDUP      STRDUP
#define SEC_STRLEN      STRLEN
#define SEC_ATOI        ATOI
#define SEC_STRSTR      STRSTR
#define SEC_STRTOL      STRTOUL
#define SEC_STRCMP      STRCMP
#define SEC_STRNCMP     STRNCMP
#define SEC_STRNCPY     STRNCPY

#else  
#ifdef WIN32_DEBUG

#define SEC_SPRINTF     sprintf
#define SEC_STRCAT      strcat
#define SEC_STRRCHR     strrchr
#define SEC_STRCHR      strchr
#define SEC_STRCPY      strcpy
#define SEC_STRDUP      strdup
#define SEC_STRLEN      strlen
#define SEC_ATOI        atoi
#define SEC_STRSTR      strstr
#define SEC_STRTOL      strtol
#define SEC_STRCMP      strcmp
#define SEC_STRNCMP     strncmp
#define SEC_STRNCPY     strncpy
#else
#define SEC_SPRINTF  TF3RD_SPRINTF
#define SEC_STRCAT   TF3RD_STRCAT 
#define SEC_STRRCHR  TF3RD_STRRCHR
#define SEC_STRCHR   TF3RD_STRCHR
#define SEC_STRCPY   TF3RD_STRCPY
#define SEC_STRDUP   TF3RD_STRDUP
#define SEC_STRLEN   TF3RD_STRLEN
#define SEC_ATOI     TF3RD_STRATOI
#define SEC_STRSTR   TF3RD_STRSTR
#define SEC_STRTOL   TF3RD_STRTOL
#define SEC_STRCMP   TF3RD_STRCMP
#define SEC_STRNCMP  TF3RD_STRNCMP
#define SEC_STRNCPY  TF3RD_STRNCPY
#endif

#endif

/*******************************************************************************
*   Type Define Section
*******************************************************************************/

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
void Sec_FreeMsg
(
    WE_UINT8    ucID, 
    WE_INT32    iMsg, 
    void        *pData
);

WE_UINT32 Sec_TimeGetCurrent(void);



#endif
