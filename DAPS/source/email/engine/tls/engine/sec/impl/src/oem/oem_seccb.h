/*=====================================================================================
    FILE NAME : oem_seccb.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This file define the macros about callback function.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                            Tracking
    Date                    Author         Number              Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-10-19    Zhanghuanqing                            create oem_seccb.h
    
=====================================================================================*/
/*--------------------------START-----------------------------*/


/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/

#ifndef _OEM_SECCB_H
#define _OEM_SECCB_H

/*******************************************************************************
*   Include File Section
*******************************************************************************/


/*******************************************************************************
*   Macro Define Section
*******************************************************************************/

#define SEC_REGCALLBACK(shell,cb,pfn,data)\
{\
    pfn(data);\
}

/*Cancel Callback*/
/*
#define SEC_CANCELCALLBACK(cb) CALLBACK_Cancel(&cb)
*/
#endif

/*--------------------------END-----------------------------*/
