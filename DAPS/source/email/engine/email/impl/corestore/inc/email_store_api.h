/*==============================================================================
    FILE NAME   : email_store_api.h
    MODULE NAME : CoreStore


    GENERAL DESCRIPTION:
        Provide a set of API for tm store message.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-10-08   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EMAIL_STORE_API_H
#define EMAIL_STORE_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "we_def.h"
#include "eme_error.h"
#include "eme_constant_api.h"

 /*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef WE_VOID* HEmailStore;

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
WE_INT32 EmailStore_InitHandle(HEmailStore *phStore, 
                               EmailOpt *pstOpt, 
                               WE_VOID *pvFileMgr, 
                               WE_VOID *pvMemMgr);

WE_VOID EmailStore_DeleteHandle(HEmailStore hStore);

WE_INT32 EmailStore_NewMessage(HEmailStore hStore, 
                               WE_UINT32 *pdwMsgId, 
                               WE_CHAR **ppcPath);

WE_INT32 EmailStore_AddMsgHeaders(HEmailStore hStore, 
                                  WE_UINT32 dwMsgId, 
                                  EmailOpt *pstOpt);

WE_INT32 EmailStore_AddMsgPart(HEmailStore hStore, 
                               WE_UINT32 dwMsgId, 
                               EmailOpt *pstOpt);

WE_INT32 EmailStore_UpdateMsgPart(HEmailStore hStore, 
                                  WE_UINT32 dwMsgId, 
                                  WE_UINT32 dwPartId, 
                                  EmailOpt *pstOpt);

WE_INT32 EmailStore_WindUp(HEmailStore hStore, 
                           WE_UINT32 dwMsgId);

WE_INT32 EmailStore_DeleteMsgPart(HEmailStore hStore, 
                                  WE_UINT32 dwMsgId, 
                                  WE_UINT32 dwPartId);

WE_INT32 EmailStore_GetMsgHeaders(HEmailStore hStore, 
                                  WE_UINT32 dwMsgId, 
                                  EmailOpt **ppstOpt);

WE_INT32 EmailStore_GetMsgPartCount(HEmailStore hStore, 
                                    WE_UINT32 dwMsgId, 
                                    WE_UINT32 *pdwPartNum);

WE_INT32 EmailStore_GetMsgPart(HEmailStore hStore, 
                               WE_UINT32 dwMsgId, 
                               WE_UINT32 dwPartId, 
                               EmailOpt **ppstOpt);

WE_INT32 EmailStore_SetMsgDownStatus(HEmailStore hStore, 
                                     WE_UINT32 dwMsgId, 
                                     WE_BOOL bDown);

WE_INT32 EmailStore_GetMsgPath(HEmailStore hStore, 
                               WE_UINT32 dwMsgId, 
                               WE_CHAR **ppcPath);

WE_INT32 EmailStore_DeleteMsg(HEmailStore hStore, 
                              WE_UINT32 dwMsgId);


/*------------------------------------------------------------------------------
*    Function Documentation
*----------------------------------------------------------------------------*/
/*=====================================================================================
FUNCTION: 
    EmailStore_InitHandle
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Init the email store handle so that the caller can use the handle to call email store
    interface.
ARGUMENTS PASSED:
    pHandle[in/out] :   The pointer of the handle to be init.
    pstOpt [in]     :   The pointer of Email Opts.
    pvFileMgr [in]  :   The pointer of File Manager.
    pvMemMgr [in]   :   The pointer of Memory Manager.

RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_INVALID_DATA,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
   The caller should call EmailStore_DeleteHandle to free the memory of 
   the store handle.
=====================================================================================*/


/*=====================================================================================
FUNCTION: 
    EmailStore_DeleteHandle
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Init the email store handle so that the caller can use the handle to call email store
    interface.
ARGUMENTS PASSED:
    pHandle[in/out] :   The pointer of the handle to be deleted.
RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_FAILED,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
   
=====================================================================================*/


/*=====================================================================================
FUNCTION: 
    EmailStore_AddMsgHeaders
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Add header part into database or file. And return the message Id.
ARGUMENTS PASSED:
    pHandle[in]      :   The handle of the email store lib.
    pstHdrOpt[in]    :   The opt contain the data of header to store in file or database.
    ppcMsgId[out]    :   The message Id which to be get.
   
RETURN VALUE:
    If the operation is SUCCESS, return the char type of message Id.
    Else return NULL.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
    If the operation is SUCCESS, the value will be EME_SUCCESS,
    else the value will be EME_FAILED or EME_NOMEMORY.
=====================================================================================*/



/*=====================================================================================
FUNCTION: 
    EmailStore_GetMsgHeaders
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Get header  from database or file.
ARGUMENTS PASSED:
    pHandle[in]      :   The handle of the mmse store lib.
    pcMsgId[in]      :   The Id of the message to get.  
    ppstHdrOpt[out]  :   The pointer to opt to get the data of header stored in file or database.
    
RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_FAILED,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
    The caller must free the memory allocated by this function(ppstOpt).
=====================================================================================*/


/*=====================================================================================
FUNCTION: 
    EmailStore_AddMsgPart
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Add all body part to database or file.
ARGUMENTS PASSED:
    pHandle[in/out]    :   The handle of the mmse store lib.
    pcMsgId[in]        :   The message Id which body part to be add.
    pstPartOpt[in]     :   The opt contain the data of body to be stored in file or database.
    
RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_FAILED,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
   
=====================================================================================*/


/*=====================================================================================
FUNCTION: 
    EmailStore_GetMsgPartCount
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Get the number of body part in database or file.
ARGUMENTS PASSED:
    pHandle[in]        :   The handle of the mmse store lib.
    pcMsgId[in]        :   The message Id which body part to be get.
    puiPartNum[out]     :   The pointer to the number of part .
    
RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_FAILED,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
   
=====================================================================================*/



/*=====================================================================================
FUNCTION: 
    EmailStore_GetMsgPart
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Get one body part to database or file.
ARGUMENTS PASSED:
    pHandle[in]      :   The handle of the mmse store lib.
    pcMsgId[in]      :   The message Id which body part to be get.
    uiPartId[in]     :   The Id of the part to get from storage.
    ppstPartOpt[out] :   The pointer to get data of body stored in file or database.
    
RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_FAILED,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
   The caller must free the memory allocated by this function(ppstOpt).
=====================================================================================*/

/*=====================================================================================
FUNCTION: 
    EmailStore_DeleteMsgPart
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Delete all body part and header in database or file.
    When we reject a message not use it again.
ARGUMENTS PASSED:
    pHandle[in]      :   The handle of the mmse store lib.
    pcMsgId[in]      :   The message Id which body part to be delete.
    uiPartId[in]     :   The Id of the part to get from storage.
    
RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_FAILED,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
   
=====================================================================================*/


/*=====================================================================================
FUNCTION: 
    EmailStore_SetMsgDownStatus
CREATE DATE: 
    2006-12-25
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    Set the down status to determine where download email data or  not.
ARGUMENTS PASSED:
    pHandle[in]      :   The handle of the mmse store lib.
    pcMsgId[in]      :   The message Id which email to be download.
    bDown[in]        :   conitune to download the email or not.
RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_FAILED,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
   
=====================================================================================*/


/*=====================================================================================
FUNCTION: 
    EmailStore_UpdateMsgPart
CREATE DATE: 
    2007-01-27
AUTHOR: 
    chenhaiwan
DESCRIPTION:
    To update the  message part in the storage.
ARGUMENTS PASSED:
    pHandle[in]          :   The handle of the mmse store lib.
    pcMsgId[in]          :   The message Id of the Email which  to be update.
    uiPartId[in]         :   The part id of the Email which  to be update.
    pstUpdatePartOpt[out]:   The Email part opts  to be update.

RETURN VALUE:
    If the operation is SUCCESS, return EME_SUCCESS.
    Else return EME_FAILED,EME_NOMEMORY.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    ...
IMPORTANT NOTES:   
   
=====================================================================================*/
#endif /* EMAIL_STORE_API_H */
