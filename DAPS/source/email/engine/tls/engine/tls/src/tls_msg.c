/*=====================================================================================
    FILE NAME   : wap_msg.c
    MODULE NAME : WAP_MSG
    
    GENERAL DESCRIPTION
        This file implement a message queue. 
        The maxium number of module which can be attached to message queue is define 
    WAP_MSG_MAX_MOD_NUM.
            
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification Tracking
    Date       Author         Number    Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-05-25 ChenZhifeng    none      Init
    2006-06-01 Alan           none      Modify according to discuess
    2006-08-07 bluefire       none      Message---->Msg
    2006-12-21 HouGuanhua     none      add error info struct
=====================================================================================*/

/**************************************************************************************
*   Include File Section
**************************************************************************************/
#include "tls_def.h"
#include "tls_msg.h"
#include "tls_dbg.h"
#include "tls_config.h"
#include "tls_ast.h"

/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
/* Define the module number ID */
#define WAP_MSG_MAX_MOD_NUM E_WAP_MSG_MODULEID_COUNT

/* To check the validate of the handle, the magic number is used */
#define WAP_MSG_MAGIC_NUM 0x4d534751    /* MSGQ */

/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/
/* Node definition of msg queue link list */
typedef struct tagSt_TlsMsgNode{
    WE_UINT32 uiTgtModID;
    struct tagSt_TlsMsgNode *pstNext;
    struct tagSt_TlsMsg *pstWapMsg;
}St_TlsMsgNode;

/* The definition of wap msg handle */
typedef struct tagSt_TlsMsgHandle{
    WE_UINT32 uiMagicNum;
    WE_HANDLE hWeaHandle;
    Fn_TlsMsgPorcess *apcbMsgProcFunc[WAP_MSG_MAX_MOD_NUM];
    void *apvPrivData[WAP_MSG_MAX_MOD_NUM];
    St_TlsMsgNode *pstMsgNodeHead;
    St_TlsMsgNode *pstMsgNodeTail;
}St_TlsMsgHandle;


/*==================================================================================================
FUNCTION: 
    WapMsg_Initialize
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    Create Initialize Msg Handle and Msg List
ARGUMENTS PASSED:
    WE_HANDLE hWeaHandle :   
    WE_HANDLE *phHandle  :   
RETURN VALUE:
    WE_INT32:
    if success,return 0
    if parameter is null,return -1
    if momory is nenough,return -1
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 TlsMsg_Init
(
    WE_HANDLE *phHandle
)
{
    St_TlsMsgHandle *pstWapMsgHandle = NULL;

    /* Check parameter first */
    if(NULL == phHandle)
    {
        return -1;
    }
    /* Create Msg Handle */
    pstWapMsgHandle = (St_TlsMsgHandle *)WAP_MALLOC((WE_ULONG)sizeof(St_TlsMsgHandle));
    if(NULL == pstWapMsgHandle )
    {
        return -1;
    }
    (WE_VOID)WAP_MEMSET( (void *)pstWapMsgHandle, 0, sizeof(St_TlsMsgHandle));

    /* Initialize Msg Handle and Msg List */
    pstWapMsgHandle->uiMagicNum = WAP_MSG_MAGIC_NUM;

    /* Return St_TlsMsgHandle as Msg Handle */
    *phHandle = (void*)pstWapMsgHandle;

    return 0;
}

/*==================================================================================================
FUNCTION: 
    WapMsg_Terminate
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    Detory WapMsgHandle and it's message
ARGUMENTS PASSED:
    WE_HANDLE hMsgHandle    :   Msg handle
RETURN VALUE:
    WE_INT32:
    if success,return 0
    if parameter is null,return -1
    if bad handle,return -1
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 TlsMsg_Terminate(WE_HANDLE hMsgHandle)
{
    St_TlsMsgHandle *pstWapMsgHandle = (St_TlsMsgHandle *)hMsgHandle;
    St_TlsMsgNode *pstWapMsgNode = NULL;

    /* Check the parameter first */
    if(NULL == pstWapMsgHandle)
    {
        return -1;
    }
    if(pstWapMsgHandle->uiMagicNum != (WE_UINT32)WAP_MSG_MAGIC_NUM)
    {
        return -1;
    }
    /* Detory all message Msg Queue */
    while(pstWapMsgHandle->pstMsgNodeHead)
    {
        pstWapMsgNode = pstWapMsgHandle->pstMsgNodeHead;
        pstWapMsgHandle->pstMsgNodeHead = pstWapMsgNode->pstNext;
        WAP_FREE(pstWapMsgNode->pstWapMsg);
        WAP_FREE(pstWapMsgNode);
    }

    /* Detory WapMsgHandle */
    WAP_FREE(pstWapMsgHandle);
    
    return 0;
}

/*==================================================================================================
FUNCTION: 
    TlsMsg_DispatchMsg
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    Disapatch A message to its message process
ARGUMENTS PASSED:
    WE_HANDLE hMsgHandle    :   Msg Handle
RETURN VALUE:
    WE_INT32 :
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 TlsMsg_DispatchMsg( WE_HANDLE hMsgHandle )
{
    St_TlsMsgHandle *pstWapMsgHandle = (St_TlsMsgHandle *)hMsgHandle;
    St_TlsMsg *pstWapMsg = NULL;
    St_TlsMsgNode *pstWapMsgNode = NULL;
    WE_INT32 iRet = 0;
    
    /* Check whether the head of message queue is NULL */
    if (NULL == pstWapMsgHandle)
    {
        return -1;
    }
    if(pstWapMsgHandle->uiMagicNum != (WE_UINT32)WAP_MSG_MAGIC_NUM)
    {
        return -1;
    }
    /* Get a Msg from message queue */
    if (NULL == pstWapMsgHandle->pstMsgNodeHead)
    {
        return 0;
    }
    pstWapMsgNode = pstWapMsgHandle->pstMsgNodeHead;
    pstWapMsgHandle->pstMsgNodeHead = pstWapMsgNode->pstNext;

    if (NULL == pstWapMsgHandle->pstMsgNodeHead)
    {    
        pstWapMsgHandle->pstMsgNodeTail = NULL;
    }
    /* Send the message to corresponding to message process function */
    pstWapMsg = pstWapMsgNode->pstWapMsg;

    if (NULL == pstWapMsgHandle->apcbMsgProcFunc[pstWapMsgNode->uiTgtModID])
    {
        WAP_FREE(pstWapMsgNode);
        WAP_FREE(pstWapMsg);
        return -1;
    }

    iRet = (pstWapMsgHandle->apcbMsgProcFunc[pstWapMsgNode->uiTgtModID])(
            pstWapMsgHandle->apvPrivData[pstWapMsgNode->uiTgtModID],
            pstWapMsg                       /* Msg Data */
           );  /* Private Data of Module */
    iRet = iRet;
    WAP_FREE(pstWapMsgNode);
    WAP_FREE(pstWapMsg);


    return 0;
 
}

/*==================================================================================================
FUNCTION: 
    TlsMsg_HasMsg
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    Check whether there is a message message queue
ARGUMENTS PASSED:
    WE_HANDLE hMsgHandle    :   Msg Handle
RETURN VALUE:
    FALSE   :   there is no message message queue
    TRUE    :
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_BOOL TlsMsg_HasMsg(WE_HANDLE hMsgHandle)
{
    St_TlsMsgHandle *pstWapMsgHandle = (St_TlsMsgHandle *)hMsgHandle;
    
    /* Check whether the head of message queue is NULL */
    if(NULL == pstWapMsgHandle)
    {
        return FALSE;
    }
    if(pstWapMsgHandle->uiMagicNum != (WE_UINT32)WAP_MSG_MAGIC_NUM)
    {
        return FALSE;
    }
    /* check the head pointer */
    if(NULL == pstWapMsgHandle->pstMsgNodeHead)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*==================================================================================================
FUNCTION: 
    TlsMsg_RegisterProcess
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    register callback and privdata to correspoding module ID
ARGUMENTS PASSED:
    WE_HANDLE hMsgHandle             :   Handle of Msg Queue 
    E_TlsMsgModuleId eModuleID      :   Module ID 
    Fn_TlsMsgPorcess *pcbMsgProcFunc :   Msg Process Function 
    WE_HANDLE pvPrivData             :   Private Data 
RETURN VALUE:
    WE_INT32:
    if success,return 0
    if parameter is null,return -1
    if bad handle,return -1
    if bad Module ID,return -1
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 TlsMsg_RegisterProcess
(
    WE_HANDLE hMsgHandle,                /* Handle of Msg Queue */
    E_TlsMsgModuleId eModuleID,                 /* Module ID */
    Fn_TlsMsgPorcess *pcbMsgProcFunc,    /* Msg Process Function */
    WE_HANDLE pvPrivData                     /* Private Data */
)
{
    St_TlsMsgHandle *pstWapMsgHandle = (St_TlsMsgHandle *)hMsgHandle;

    /* Check the parameter first */
    if(NULL == pstWapMsgHandle ||NULL == pcbMsgProcFunc || NULL == pvPrivData )
    {
        return -1;
    }
    if(pstWapMsgHandle->uiMagicNum != (WE_UINT32)WAP_MSG_MAGIC_NUM)
    {
        return -1;
    }
    /* register callback and privdata to correspoding module ID */
    pstWapMsgHandle->apcbMsgProcFunc[eModuleID] = pcbMsgProcFunc;
    pstWapMsgHandle->apvPrivData[eModuleID] = pvPrivData;

    return 0;
}

/*==================================================================================================
FUNCTION: 
    TlsMsg_PrependMsg
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    deregister callback and free the module ID
ARGUMENTS PASSED:
    WE_HANDLE hMsgHandle         :   Handle of Msg Queue
    E_TlsMsgModuleId eModuleID  :   Module ID
RETURN VALUE:
    WE_INT32:
    if success,return 0
    if parameter is null,return -1
    if bad handle,return -1
    if bad Module ID,return -1
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 TlsMsg_DeregisterProcess
(
    WE_HANDLE hMsgHandle,                /* Handle of Msg Queue */
    E_TlsMsgModuleId eModuleID                  /* Module ID */
)
{
    St_TlsMsgHandle *pstWapMsgHandle = (St_TlsMsgHandle *)hMsgHandle;

    /* Check the parameter first */
    if(NULL == pstWapMsgHandle)
    {
        return -1;
    }
    if(pstWapMsgHandle->uiMagicNum != (WE_UINT32)WAP_MSG_MAGIC_NUM)
    {
        return -1;
    }

    /* deregister callback and privdata to correspoding module ID */
    pstWapMsgHandle->apcbMsgProcFunc[eModuleID] = NULL;
    pstWapMsgHandle->apvPrivData[eModuleID] = NULL;

    return 0;
}

/*==================================================================================================
FUNCTION: 
    TlsMsg_PostMsg
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    Msg is put into message queue only. It maybe processed while TlsMsg_DispatchMsg is called
ARGUMENTS PASSED:
    WE_HANDLE hMsgHandle,            Handle of Msg Queue
    E_TlsMsgModuleId eTgtModID,     Target Module ID 
    WE_INT32 iMsgID,                 Msg ID 
    WE_INT32 iParam1,                        
    WE_INT32 iParam2,
    WE_UINT32 uiParam1,
    WE_UINT32 uiParam2,
    void *pvParam1,
    void *pvParam2
RETURN VALUE:
    WE_INT32
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 TlsMsg_PostMsg
(
    WE_HANDLE hMsgHandle,                /* Handle of Msg Queue */
    E_TlsMsgModuleId eTgtModID,             /* Target Module ID  */
    WE_INT32 iMsgID,                   /* Msg ID */
    WE_INT32 iParam1,                        
    WE_INT32 iParam2,
    WE_UINT32 uiParam1,
    WE_UINT32 uiParam2,
    void *pvParam1,
    void *pvParam2
)
{
    St_TlsMsgHandle *pstWapMsgHandle = (St_TlsMsgHandle *)hMsgHandle;
    St_TlsMsgNode *pstWapMsgNod = NULL;
    St_TlsMsg *pstWapMsg = NULL;

    WAP_LOG_PRINT((E_TLS_DBG_MSG,E_WAP_DBG_LEVEL_LOW)("TlsMsg_PostMsg()___hMsgHandle=0x%x", pstWapMsgHandle);)

    /* Check the parameter first */
    if(NULL == pstWapMsgHandle)
    {
        WAP_LOG_PRINT((E_TLS_DBG_MSG,E_WAP_DBG_LEVEL_LOW)("TlsMsg_PostMsg()___null handle");)
        return -1;
    }
    if(pstWapMsgHandle->uiMagicNum != (WE_UINT32)WAP_MSG_MAGIC_NUM)
    {
        WAP_LOG_PRINT((E_TLS_DBG_MSG,E_WAP_DBG_LEVEL_LOW)("TlsMsg_PostMsg()___bad handle");)
        return -1;
    }

    /* Malloc a message structure */
    pstWapMsgNod = (St_TlsMsgNode *)WAP_MALLOC((WE_LONG)sizeof(St_TlsMsgNode));
    if(NULL == pstWapMsgNod)
    {
        return -1;
    }
    pstWapMsg = (St_TlsMsg *)WAP_MALLOC((WE_LONG)sizeof(St_TlsMsg));
    if(NULL == pstWapMsg)
    {
        WAP_FREE(pstWapMsgNod);
        return -1;
    }

    /* Fill message data into message structure */
    pstWapMsg->iParam1 = iParam1;
    pstWapMsg->iParam2 = iParam2;
    pstWapMsg->uiParam1 = uiParam1;
    pstWapMsg->uiParam2 = uiParam2;
    pstWapMsg->pvParam1 = pvParam1;
    pstWapMsg->pvParam2 = pvParam2;
    pstWapMsg->uiMsgType = (WE_UINT32)iMsgID;

    pstWapMsgNod->pstWapMsg = pstWapMsg;
    pstWapMsgNod->uiTgtModID = (WE_UINT32)eTgtModID;
    pstWapMsgNod->pstNext = NULL;
    
    /* Add message to the tail of message queue */
    if(pstWapMsgHandle->pstMsgNodeTail == NULL)
    {
        pstWapMsgHandle->pstMsgNodeHead = pstWapMsgNod;
        pstWapMsgHandle->pstMsgNodeTail = pstWapMsgNod;
    }
    else
    {
        pstWapMsgHandle->pstMsgNodeTail->pstNext = pstWapMsgNod;
        pstWapMsgHandle->pstMsgNodeTail = pstWapMsgNod;
    }

    WAP_LOG_PRINT((E_TLS_DBG_MSG,E_WAP_DBG_LEVEL_LOW)("WAP_MSG: somebody post a message(%08X) to %08X\n", iMsgID, eTgtModID);)

    return 0;
}

/*==================================================================================================
FUNCTION: 
    TlsMsg_PrependMsg
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    Msg was inserted into message queue header,when this message should be priority
ARGUMENTS PASSED:
    WE_HANDLE hMsgHandle,            Handle of Msg Queue
    E_TlsMsgModuleId eTgtModID,     Target Module ID 
    WE_INT32 iMsgID,                 Msg ID 
    WE_INT32 iParam1,                        
    WE_INT32 iParam2,
    WE_UINT32 uiParam1,
    WE_UINT32 uiParam2,
    void *pvParam1,
    void *pvParam2
RETURN VALUE:
    WE_INT32
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 TlsMsg_PrependMsg
(
    WE_HANDLE hMsgHandle,         /* Handle of Msg Queue */
    E_TlsMsgModuleId eTgtModID,  /* Target Module ID  */
    WE_UINT32 uiMsgID,            /* Msg ID */
    WE_INT32 iParam1,             
    WE_INT32 iParam2,
    WE_UINT32 uiParam1,
    WE_UINT32 uiParam2,
    void *pvParam1,
    void *pvParam2
)
{
    St_TlsMsgHandle *pstWapMsgHandle = (St_TlsMsgHandle *)hMsgHandle;
    St_TlsMsgNode *pstWapMsgNod = NULL;
    St_TlsMsg *pstWapMsg = NULL;

    
  
    /* Check the parameter first */
    if(NULL == pstWapMsgHandle)
    {
        return -1;
    }

    if(pstWapMsgHandle->uiMagicNum != (WE_UINT32)WAP_MSG_MAGIC_NUM)
    {
        return -1;
    }

    /* Malloc a message structure */
    pstWapMsgNod = (St_TlsMsgNode *)WAP_MALLOC((WE_LONG)sizeof(St_TlsMsgNode));
    if (NULL == pstWapMsgNod)
    {
        return -1;
    }
    pstWapMsg = (St_TlsMsg *)WAP_MALLOC((WE_LONG)sizeof(St_TlsMsg));
    if (NULL == pstWapMsg)
    {
        WAP_FREE(pstWapMsgNod);
        return -1;
    }

    /* Fill message data into message structure */
    pstWapMsg->iParam1   = iParam1;
    pstWapMsg->iParam2   = iParam2;
    pstWapMsg->uiParam1  = uiParam1;
    pstWapMsg->uiParam2  = uiParam2;
    pstWapMsg->pvParam1  = pvParam1;
    pstWapMsg->pvParam2  = pvParam2;
    pstWapMsg->uiMsgType = uiMsgID;

    pstWapMsgNod->pstWapMsg = pstWapMsg;
    pstWapMsgNod->uiTgtModID = (WE_UINT32)eTgtModID;
    pstWapMsgNod->pstNext = NULL;


    /* Add message to the head of message queue */
    if(pstWapMsgHandle->pstMsgNodeTail == NULL)
    {
        pstWapMsgHandle->pstMsgNodeHead = pstWapMsgNod;
        pstWapMsgHandle->pstMsgNodeTail = pstWapMsgNod;
    }
    else
    {   pstWapMsgNod->pstNext = pstWapMsgHandle->pstMsgNodeHead;
        pstWapMsgHandle->pstMsgNodeHead = pstWapMsgNod;
    }

    WAP_LOG_PRINT((E_TLS_DBG_MSG,E_WAP_DBG_LEVEL_LOW)("WAP_MSG: somebody prepend a message(%08X) to %08X\n", uiMsgID, eTgtModID);)

    return 0;
}

/*==================================================================================================
FUNCTION: 
    TlsMsg_SendMsg
CREATE DATE:
    2006-05-25
AUTHOR:
    ChenZhifeng
DESCRIPTION:
    Msg is send to message process of target module at once
ARGUMENTS PASSED:
    WE_HANDLE hMsgHandle,            Handle of Msg Queue
    E_TlsMsgModuleId eTgtModID,     Target Module ID 
    WE_INT32 iMsgID,                 Msg ID 
    WE_INT32 iParam1,                        
    WE_INT32 iParam2,
    WE_UINT32 uiParam1,
    WE_UINT32 uiParam2,
    void *pvParam1,
    void *pvParam2
RETURN VALUE:
    WE_INT32
USED GLOBAL VARIABLES:   
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 TlsMsg_SendMsg
(
    WE_HANDLE hMsgHandle,                /* Handle of Msg Queue */
    E_TlsMsgModuleId eTgtModID,             /* Target Module ID  */
    WE_INT32 iMsgID,                   /* Msg ID */
    WE_INT32 iParam1,
    WE_INT32 iParam2,
    WE_UINT32 uiParam1,
    WE_UINT32 uiParam2,
    void *pvParam1,
    void *pvParam2
)
{
    St_TlsMsgHandle *pstWapMsgHandle = (St_TlsMsgHandle *)hMsgHandle;
    St_TlsMsg stWapMsg;
    
    /* Check the parameter first */
    if(NULL == pstWapMsgHandle)
    {
        return -1;
    }
    if(pstWapMsgHandle->uiMagicNum != (WE_UINT32)WAP_MSG_MAGIC_NUM)
    {
        return -1;
    }

    /* Fill message data into message structure */
    stWapMsg.uiMsgType = (WE_UINT32)iMsgID;
    stWapMsg.iParam1 = iParam1;
    stWapMsg.iParam2 = iParam2;
    stWapMsg.uiParam1 = uiParam1;
    stWapMsg.uiParam2 = uiParam2;
    stWapMsg.pvParam1 = pvParam1;
    stWapMsg.pvParam2 = pvParam2;
    
    /* Call the message process directly */
    if(NULL == pstWapMsgHandle->apcbMsgProcFunc[eTgtModID])
    {        
        return -1;
    }
    (WE_VOID)(pstWapMsgHandle->apcbMsgProcFunc[eTgtModID])(
            pstWapMsgHandle->apvPrivData[eTgtModID],
            &stWapMsg 
            );
    return 0;
}




