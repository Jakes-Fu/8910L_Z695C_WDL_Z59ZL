/*==================================================================================================
    FILE NAME : sec_comm.h
    MODULE NAME : SEC
    
    
    GENERAL DESCRIPTION
        In this file,define the initial function prototype ,and will be update later.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                              Tracking
    Date               Author                Number        Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
      2006-07-07   Jabber Wu                           create sec_comm.h
      2006-08-20   Kevin Yang                          update
    
==================================================================================================*/

/***************************************************************************************************
*   Multi-Include-Prevent Section
***************************************************************************************************/
#ifndef _SEC_COMM_H
#define _SEC_COMM_H
/***************************************************************************************************
*   include File Section
*******************************************************************************/

#include "we_def.h"
#include "we_scl.h"
#include "we_mem.h"
#include "we_libalg.h"

#include "oem_secerr.h"
#include "oem_secevent.h"
#include "oem_seclog.h"
#include "oem_secfile.h"

#include "sec_cfg.h"
#include "sec_asn1.h" 

/* Global head file */
#include "oem_seccb.h"
#include "sec.h"
#include "isecw.h"

#include "sec_ast.h"
/*#include "sec_cfg.h"*/
#include "sec_msg.h"
#include "sec_iwap.h"
#include "sec_ibrs.h"
#include "sec_lib.h"
#include "sec_main.h"
/*
#include "oem_secpkc.h"*/
#include "sec_rsacomm.h"//add by tang 070202

/* Public head file */
#include "sec_uecst.h"
#include "sec_wimsi.h"
#include "sec_cd.h"

/* Certificate related */
#include "sec_cm.h"
#include "sec_x509.h" /*add by tang 070205*/
#include "sec_wtlsparser.h"
#include "oem_secx509parser.h"
#include "sec_iwapim.h"

/* WIM related */
#include "sec_wimpkcresp.h"
#include "sec_iwim.h"
#include "sec_wim.h"
#include "sec_wimresp.h"
#include "sec_wimsp.h"

/* User Event */
#include "sec_ue.h"
#include "sec_ueh.h"
#include "sec_iue.h"
#include "sec_ueresp.h"

/* Sec other */
#include "sec_resp.h"
#include "sec_dp.h"
#include "sec_tl.h"
#include "sec_pubdata.h"
#include "sec_evtdcvt.h"


/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/
/*common I interface*/
#if 1
//typedef struct ISec ISec;
typedef struct _ISec
{ 
    WE_HANDLE   hPrivateData;
    /*added by Bird 061122*/
    WE_INT32     iIFType;
   /* St_WimUCertKeyPairInfo      *pstUserCertKeyPair;    */
    WE_HANDLE   hSecPrivDataCB;
    WE_HANDLE   hcbSecEvtFunc;
}ISec;
#endif
#endif
