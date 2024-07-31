/*==================================================================================================
    FILE NAME : sec_ueresp.h
    MODULE NAME : SEC
     
 
    GENERAL DESCRIPTION
        In this file,define the ue response  prototype 
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                            Tracking
    Date              Author               Number           Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-21   Tang                                     create sec_ueresp.h
==================================================================================================*/



/************************************************************************/
/*       Multi-Include-Prevent Section                                                                     */
/************************************************************************/
#ifndef SEC_UERESP_H
#define SEC_UERESP_H 

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE

/************************************************************************/
/*      Prototype Declare Section                                                                     */
/************************************************************************/

void Sec_UeConfirmDialogResp(WE_HANDLE hSecHandle, WE_UINT8 ucAnswer);

#endif

#endif

/*------------------------------------------END--------------------------------------------------*/
