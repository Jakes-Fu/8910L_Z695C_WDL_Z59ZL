/*==================================================================================================
    FILE NAME : sec_pubdata.h
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION
        In this file,define the global data structure , and all the member is used as global 
        data, and is initialized when sec module is created.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                            Tracking
    Date              Author              Number          Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07   Kevin Yang                            create sec_pubdata.h
    2007-05-17     tang                          Delete cert manage and signtext
    2007-06-19    bluefire                         four bytes align for dragonfly
==================================================================================================*/
/***************************************************************************************************
*   multi-Include-Prevent Section
***************************************************************************************************/
#ifndef _SECPUBDATA_H_
#define _SECPUBDATA_H_

/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/
typedef struct tagSt_PublicData
{
    WE_HANDLE                   hWeHandle;

    WE_VOID*                   pvRespCBData;
    WE_VOID*                   pvUeCBData;
     /*modified  by bird 060118,for malloc failed improve*/
    WE_INT32                    iRespEvent;
    WE_INT32                    iUeEvent;
    St_SecMessage *             pstMsgQueueHead;    
    St_SecMessage *             pstMsgQueueTail;    
    St_SecMessage *             pstMsgFreeList;     
    WE_INT32                    iMsgFreeListLen;                          
    Fn_SecMessageFunc *         apfnSecMsgFunc[M_SEC_MSG_MAX_REGS_FUNC_POINTER];
    WE_INT32                    iTooOldCertId;
    WE_INT32                    iUserReq;
    WE_INT32                    iUeShown;
    WE_INT32                    iNocertUeShown;
    
    St_SecInfo                  *pstInfo;
    St_SecSessionRec            *pstSessionCache;    
    St_SecPeerRec               *pstPeerCache;  
    WE_CHAR                     *pcHaveLoad;  
    
    WE_HANDLE                 hCACert;
    WE_HANDLE                 hIX509Chain;    
    
    St_SecWimElement *            pstHeadElement;                                
#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
    WE_VOID*                    pvCurUeResp;
#endif
    WE_INT32                    iStates;
    WE_INT32                    iWantsToRun;
    WE_VOID*                    pvCurFnc;
    St_SecDpParameter           stQueueHead; 
    St_SecDpParameter           stQueueTail; 
/*for tls,add by tang 070108*/
    WE_HANDLE                 pvAlgHandle;
    WE_UINT32                auiCertChainIds[3];
    WE_UCHAR                 *pucKeyId;
    WE_INT32                 iSignAlg;    

    /*modify linux by Bird 070313*/
    WE_UINT8                  ucChainDepth;
    WE_UINT8                  aucReserved[3];/* four bytes align */

}St_PublicData;

#endif
