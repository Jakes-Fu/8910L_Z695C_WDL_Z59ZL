/*==================================================================================================
    FILE NAME : sec_main.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION
        In this file,define the main function of sec module.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                             Tracking
    Date             Author                Number        Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07   Kevin Yang                            create sec_main.h
    2007-05-17     tang                          Delete cert manage and signtext
==================================================================================================*/

#ifndef SEC_MAIN_H
#define SEC_MAIN_H

WE_INT32 Sec_StartEx(WE_HANDLE hSecHandle);
WE_UINT32 Sec_StopEx(WE_HANDLE hSecHandle);

void Sec_RunMainFlow(WE_HANDLE hHandle);
WE_INT32 Sec_PostMsg(WE_HANDLE hSecHandle, WE_INT32 iEvent, WE_HANDLE hData);

#endif
