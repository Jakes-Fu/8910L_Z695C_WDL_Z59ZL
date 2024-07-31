/*==================================================================================================
    HEADER NAME : we_stream.h
    MODULE NAME : WE
    
    PRE-INCLUDE FILES DESCRIPTION    
    
    GENERAL DESCRIPTION
        In this file, define the initial function prototype for WIM module, and will be updated later.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                   Tracking
    Date          Author            Number      Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-01-14    Stone An         None         create we_rc4.h
==================================================================================================*/

/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef WE_STREAM_H
#define WE_STREAM_H

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
WE_INT32 We_LibRC4EncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibRC4EncryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibRC4EncryptFinal(WE_HANDLE handle);

WE_INT32 We_LibRC4DecryptInit
( 
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibRC4DecryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibRC4DecryptFinal(WE_HANDLE handle);

#endif  /*WE_STREAM_H*/


/*--------------------------END-----------------------------*/

