/*==================================================================================================
    FILE NAME : sec_cd.h
    MODULE NAME : SEC
            
    GENERAL DESCRIPTION
        In this file,define the function prototype 
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                            Tracking
    Date              Author              Number        Description of changes
    ----------   ------------   ---------   -------------------------------------------------------
    2006-07-07      Sam                                 create sec_cd.h
    2007-06-19    bluefire                         four bytes align for dragonfly
==================================================================================================*/

/***************************************************************************************************
*   Multi-Include-Prevent Section
***************************************************************************************************/
#ifndef SEC_CH_H
#define SEC_CH_H

/***************************************************************************************************
*   include File Section
***************************************************************************************************/

/***************************************************************************************************
*   Marco Define Section
***************************************************************************************************/

/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/
/* this structure used to store the information about certificate transfer information. */
typedef struct tagSt_SecCertTransferInfo 
{
    WE_UCHAR    *pucCertData;        /* store the content data of certificate */    
    WE_HANDLE   hPipeHandle;        /* store the handle of pipe operation */
    WE_CHAR     *pcPipeName;        /* store the pipe name when the pipe operation */
    WE_CHAR     *pcCertMimeType;    /* content type of MIME */
    WE_UINT16   usCertLength;       /* the length of certificate */
    WE_UINT8    ucTransferType;     /* decide which way to transfer data */
    WE_UINT8    ucReserved;
} St_SecCertTransferInfo;

/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/
/*================================================================================================
 * Get the information of certificates which transfer
 *================================================================================================*/
void Sec_GetTransferCertInfo( WE_HANDLE hSecHandle, WE_VOID *pcPointer );

#endif/* endif SEC_CH_H */
