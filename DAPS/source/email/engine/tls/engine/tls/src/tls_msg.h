/*=====================================================================================
    FILE NAME   : wap_msg.h
    MODULE NAME : WAP_MSG
    
    PRE-INCLUDE FILES DESCRIPTION
        we_def.h      
        
    GENERAL DESCRIPTION
        In this file,define the function prototype.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification Tracking
    Date       Author       Number    Description of changes
    ---------- ------------ --------- --------------------------------------------
    2006-05-25 ChenZhiFeng  none        Init
    2006-06-01 Alan.Ren     none     Modify according to our discuess
    2006-06-05 Bluefire     none     Modify according to our requirement
    2006-08-07 bluefire     none     Message---->Msg
    
    
=====================================================================================*/

/*==================================================================================================
*   Multi-Include-Prevent Section
==================================================================================================*/
#ifndef TLS_MSG_H
#define TLS_MSG_H
/*==================================================================================================
* Macro Define Section
==================================================================================================*/
/*==================================================================================================
*   Type Define Section
==================================================================================================*/

/* the id is also array index */
typedef enum
{
    E_WAP_MSG_MODULEID_BASE = -1,
    E_WAP_MSG_MODULEID_TLS,
    E_WAP_MSG_MODULEID_COUNT        /* The count of module ID */
} E_TlsMsgModuleId;

typedef struct tagSt_TlsMsg
{
    WE_UINT32       uiMsgType;
    WE_INT32        iParam1;
    WE_INT32        iParam2;
    WE_UINT32       uiParam1;
    WE_UINT32       uiParam2;
    void           *pvParam1;
    void           *pvParam2;
} St_TlsMsg;




typedef WE_INT32 Fn_TlsMsgPorcess( WE_HANDLE, St_TlsMsg * );

WE_INT32 TlsMsg_Init
(
    WE_HANDLE *phHandle
);


WE_INT32 TlsMsg_Terminate
(
    WE_HANDLE hMsgHandle             /* the handle returned from WapMsgInitialize */
);

/* Disapatch A message to its message process */
WE_INT32 TlsMsg_DispatchMsg
(
    WE_HANDLE hMsgHandle             
);

WE_BOOL TlsMsg_HasMsg
(
    WE_HANDLE hMsgHandle             /* Check whether there is a message in message queue */
);


WE_INT32 TlsMsg_RegisterProcess
(
    WE_HANDLE hMsgHandle,                /* Handle of Msg Queue */
    E_TlsMsgModuleId eModuleID,                 /* Module ID */
    Fn_TlsMsgPorcess *pcbMsgProcFunc,    /* Msg Process Function */
    WE_HANDLE pvPrivData                     /* Private Data */
);


WE_INT32 TlsMsg_DeregisterProcess
(
    WE_HANDLE               hMsgHandle,
    E_TlsMsgModuleId       eModId
);

/* Msg is put into message queue only. It maybe processed while WapMsg_DispatchMsg is called */

WE_INT32 TlsMsg_PostMsg
(
    WE_HANDLE            hMsgHandle,
    E_TlsMsgModuleId    eDstModId,
    WE_INT32            iMsgType,
    WE_INT32             iParam1,
    WE_INT32             iParam2,
    WE_UINT32            uiParam1,
    WE_UINT32            uiParam2,
    void                *pvParam1,
    void                *pvParam2
);

/*Msg was inserted into message queue header,when this message should be priority*/
WE_INT32 TlsMsg_PrependMsg
(
    WE_HANDLE hMsgHandle,        
    E_TlsMsgModuleId eTgtModID, 
    WE_UINT32 uiMsgID,           
    WE_INT32 iParam1,             
    WE_INT32 iParam2,
    WE_UINT32 uiParam1,
    WE_UINT32 uiParam2,
    void *pvParam1,
    void *pvParam2
);

/* Msg is send to message process of target module at once */

WE_INT32 TlsMsg_SendMsg
(
    WE_HANDLE            hMsgHandle,
    E_TlsMsgModuleId     eDstModId,
    WE_INT32            iMsgType,
    WE_INT32             iParam1,
    WE_INT32             iParam2,
    WE_UINT32            uiParam1,
    WE_UINT32            uiParam2,
    void                *pvParam1,
    void                *pvParam2
);


#endif  /* end WAP_MSG_H */


