
/*=====================================================================================
    FILE NAME : sec_evtdcvt.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This File will gather functions that encode and decode the event message.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                    Author         Number                  Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-11-24          Bird                                           create sec_evtdcvt.c
    2007-05-17     tang                                 Delete cert manage and signtext
=====================================================================================*/

#include "sec_comm.h"

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE


/************************************************************
 * Macro definitions
 ************************************************************/

#define M_SEC_DCVT_GET_BYTE(pstObj)    ((pstObj)->pucData[(pstObj)->lPos++])
#define M_SEC_DCVT_PUT_BYTE(pstObj, b) ((pstObj)->pucData[(pstObj)->lPos++] = (WE_UCHAR)(b))

#define M_SEC_DCVT_REM_LENGTH(str) ((str)->lLen - (str)->lPos)

/*constant*/
#define M_SEC_DCVT_DECODE             1
#define M_SEC_DCVT_ENCODE             2
#define M_SEC_DCVT_ENCODE_SIZE    3
#define M_SEC_DCVT_FREE                  4

typedef struct tagSt_Dcvt
{
  WE_LONG           lPos;
  WE_LONG           lLen;
  WE_INT16          sOperation;
  WE_UCHAR       *pucData;
} St_Dcvt;

/*==================================================================================================
FUNCTION: 
    Sec_DcvtInit
CREATE DATE:
    2006-11-24
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    Prepare a conversion object for a specific operation, using the indicated buffer of given length.    
ARGUMENTS PASSED:
    St_Dcvt *pstObj[OUT]:conversion object
    WE_INT16 sOperation[IN]:operation 
    void *pvBuf[IN]:buffer
    WE_LONG lLen[IN]:lenght of buffer
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

static void
Sec_DcvtInit (St_Dcvt *pstObj, WE_INT16 sOperation,
              void *pvBuf, WE_LONG lLen)
{
    pstObj->sOperation = sOperation;
    pstObj->pucData = (WE_UCHAR *)pvBuf;
    pstObj->lLen = lLen;
    pstObj->lPos = 0;
}
/*==================================================================================================
FUNCTION: 
    Sec_DcvtInt32
CREATE DATE:
    2006-11-24
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    Convert a 4-byte signed integer in big-endian
ARGUMENTS PASSED:
    St_Dcvt *pstObj[IN/OUT]:conversion object
    WE_INT32 *piData[IN/OUT]:4-byte signed integer
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_INT32
Sec_DcvtInt32 (St_Dcvt *pstObj, WE_INT32 *piData)
{
    WE_UCHAR     ucA = 0; 
    WE_UCHAR     ucB = 0; 
    WE_UCHAR     ucC = 0; 
    WE_UCHAR     ucD = 0;
    WE_UINT32     uiN = 0;
    
    switch (pstObj->sOperation) 
    {
    case M_SEC_DCVT_DECODE:
        if (M_SEC_DCVT_REM_LENGTH (pstObj) < 4)
            return FALSE;
        ucA = M_SEC_DCVT_GET_BYTE (pstObj);
        ucB = M_SEC_DCVT_GET_BYTE (pstObj);
        ucC = M_SEC_DCVT_GET_BYTE (pstObj);
        ucD = M_SEC_DCVT_GET_BYTE (pstObj);
        uiN = ((WE_UINT32)ucA << 24) | ((WE_UINT32)ucB << 16) | ((WE_UINT32)ucC  << 8) | (WE_UINT32)ucD;
        if (uiN & 0x80000000) 
        {
            /* We have a negative number */
            uiN = (WE_UINT32)(uiN & 0x7fffffff);
            if (uiN == 0) 
            {
                *piData = -0x7fffffff - 1;
            }
            else 
            {
                uiN = (0x7fffffff - uiN) + 1;
                *piData = -(WE_INT32)uiN;
            }
        }
        else 
        {
            *piData = (WE_INT32)uiN;
        }
        break;
        
    case M_SEC_DCVT_ENCODE:
        if (M_SEC_DCVT_REM_LENGTH (pstObj) < 4)
            return FALSE;
        if (*piData < 0) 
        {
            uiN = (WE_UINT32)((0x7fffffff + *piData) + 1);
            uiN |= 0x80000000;
        }
        else 
        {
            uiN = (WE_UINT32)(*piData);
        }
        M_SEC_DCVT_PUT_BYTE (pstObj, ((uiN >> 24) & 0xff));
        M_SEC_DCVT_PUT_BYTE (pstObj, ((uiN >> 16) & 0xff));
        M_SEC_DCVT_PUT_BYTE (pstObj, ((uiN >> 8) & 0xff));
        M_SEC_DCVT_PUT_BYTE (pstObj, (uiN & 0xff));
        break;
        
    case M_SEC_DCVT_ENCODE_SIZE:
        pstObj->lPos += 4;
        break;
        
    case M_SEC_DCVT_FREE:
        break;
        
    default:
        return FALSE;
    }
    
    return TRUE;
}
/*==================================================================================================
FUNCTION: 
    Sec_DcvtBool
CREATE DATE:
    2006-11-24
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    Convert a Boolean,  a WE_BOOL will always be encoded as one unsigned byte. 
ARGUMENTS PASSED:
    St_Dcvt *pstObj[IN/OUT]:conversion object
    WE_BOOL *pbData[IN/OUT]:boolean
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

static WE_INT32
Sec_DcvtBool (St_Dcvt *pstObj, WE_BOOL *pbData)
{
    WE_UINT8 ucTmp = 0;
    
    switch (pstObj->sOperation) 
    {
    case M_SEC_DCVT_DECODE:
        if (M_SEC_DCVT_REM_LENGTH (pstObj) < 1)
            return FALSE;
        ucTmp = M_SEC_DCVT_GET_BYTE (pstObj);
        *pbData = (WE_BOOL)ucTmp;
        break;
        
    case M_SEC_DCVT_ENCODE:
        if (M_SEC_DCVT_REM_LENGTH (pstObj) < 1)
            return FALSE;
        ucTmp = (WE_UINT8)*pbData;
        M_SEC_DCVT_PUT_BYTE (pstObj, ucTmp);
        break;
        
    case M_SEC_DCVT_ENCODE_SIZE:
        pstObj->lPos += 1;
        break;
        
    case M_SEC_DCVT_FREE:
        break;
        
    default:
        return FALSE;
    }
    
    return TRUE;
}

/*==================================================================================================
FUNCTION: 
    Sec_CvtConfirmAction
CREATE DATE:
    2006-11-24
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    Convert a structure St_ConfirmAction
ARGUMENTS PASSED:
    St_Dcvt *pstObj[IN/OUT]:conversion object
    St_ConfirmAction* pstConfirmAction[IN/OUT]:structure want to be convert
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

/*St_ConfirmAction*/
static WE_INT32 
Sec_CvtConfirmAction(St_Dcvt *pstObj, St_ConfirmAction* pstConfirmAction)
{
    if((NULL == pstObj)||(NULL == pstConfirmAction))
    {
        return FALSE;
    }
    if(!Sec_DcvtBool(pstObj,&(pstConfirmAction->bIsOk)))
    {
        return FALSE;
        
    }
    return TRUE;
}

/*==================================================================================================
FUNCTION: 
    Sec_CvtConfirm
CREATE DATE:
    2006-11-24
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    Convert a structure St_Confirm
ARGUMENTS PASSED:
    St_Dcvt *pstObj[IN/OUT]:conversion object
    St_Confirm* pstConfirm[IN/OUT]:structure want to be convert
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

/*St_Confirm*/
static WE_INT32 
Sec_CvtConfirm(St_Dcvt *pstObj, St_Confirm* pstConfirm)
{
    if((NULL == pstObj)||(NULL == pstConfirm))
    {
        return FALSE;
    }
    if(!Sec_DcvtInt32(pstObj,&(pstConfirm->iConfirmStrId)))
    {
        return FALSE;
    }
    return TRUE;
    
}

/*==================================================================================================
FUNCTION: 
    Sec_CvtWarning
CREATE DATE:
    2006-11-24
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    Convert a structure St_Warning
ARGUMENTS PASSED:
    St_Dcvt *pstObj[IN/OUT]:conversion object
    St_Warning* pstWarning[IN/OUT]:structure want to be convert
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

/*St_Warning*/
static WE_INT32 
Sec_CvtWarning(St_Dcvt *pstObj, St_Warning* pstWarning)
{
    if((NULL == pstObj)||(NULL == pstWarning))
    {
        return FALSE;
    }
    if(!Sec_DcvtInt32(pstObj,&(pstWarning->iWarningContent)))
    {
        return FALSE;
    }
    return TRUE;
    
}

/*==================================================================================================
FUNCTION: 
    Sec_EncodeEvt
CREATE DATE:
    2006-11-24
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    encode evt 
ARGUMENTS PASSED:
    WE_INT32 iEvent[IN]:event
    WE_VOID* pvSrcData[IN]:structure want to be encode
    WE_VOID** ppvDesData[OUT]:output buffer
    WE_UINT32* puiLen[OUT]:buffer length
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_INT32 
Sec_EncodeEvt(WE_INT32 iEvent,WE_VOID* pvSrcData,WE_VOID** ppvDesData,WE_UINT32* puiLen )
{
    St_Dcvt stObj = {0};
    /*1.encode*/
    switch(iEvent)
    {
    case G_Evt_CONFIRM:
        {
            /*calculate encode size*/
            Sec_DcvtInit(&stObj,M_SEC_DCVT_ENCODE_SIZE,NULL,0);
            if(!Sec_CvtConfirm(&stObj,(St_Confirm*)pvSrcData))
            {
                return FALSE;
            }
            *puiLen = (WE_UINT32)(stObj.lPos +4);
            *ppvDesData = (WE_VOID*)WE_MALLOC(*puiLen);
            if(NULL == *ppvDesData )
            {
                return FALSE;
            }
            (void)WE_MEMSET(*ppvDesData,0,*puiLen );
            /*encode*/
            Sec_DcvtInit(&stObj,M_SEC_DCVT_ENCODE,*ppvDesData,(WE_LONG)(*puiLen));
            /*event*/
            if(!(Sec_DcvtInt32(&stObj,&iEvent)))
            {
                return FALSE;
            }
            if(!Sec_CvtConfirm(&stObj,(St_Confirm*)pvSrcData))
            {
                if(NULL != *ppvDesData)
                {
                    WE_FREE( *ppvDesData);
                    *ppvDesData= NULL;     
                }
                return FALSE;
            }
        }
        break;
     case G_Evt_WARNING:/*St_Warning*/
         {
            /*calculate encode size*/
            Sec_DcvtInit(&stObj,M_SEC_DCVT_ENCODE_SIZE,NULL,0);
            if(!Sec_CvtWarning(&stObj,(St_Warning*)pvSrcData))
            {
                return FALSE;
            }
            *puiLen = (WE_UINT32)(stObj.lPos +4);
            *ppvDesData = (WE_VOID*)WE_MALLOC(*puiLen);
            if(NULL == *ppvDesData )
            {
                return FALSE;
            }
            (void)WE_MEMSET(*ppvDesData,0,*puiLen );
            /*encode*/
            Sec_DcvtInit(&stObj,M_SEC_DCVT_ENCODE,*ppvDesData,(WE_LONG)(*puiLen));
            /*event*/
            if(!(Sec_DcvtInt32(&stObj,&iEvent)))
            {
                return FALSE;
            }
            if(!Sec_CvtWarning(&stObj,(St_Warning*)pvSrcData))
            {
                if(NULL != *ppvDesData)
                {
                    WE_FREE( *ppvDesData);
                    *ppvDesData= NULL;     
                }
                return FALSE;
            }
         }
     break;
        
    default:
        *ppvDesData= NULL;
        return FALSE;
    }
    return TRUE;
    
}
/*==================================================================================================
FUNCTION: 
    Sec_DecodeEvtAction
CREATE DATE:
    2006-11-24
AUTHOR:
    Bird Zhang
DESCRIPTION:    
    decode evt action
ARGUMENTS PASSED:
    WE_VOID* pvSrcData[IN]:input buffer
    WE_UINT32 uiLen[IN]:buffer length
    WE_INT32 *piEvent[OUT]:output event
    WE_VOID** pvDesData[OUT]:structure want to be decode
RETURN VALUE:
    TRUE: success
    FALSE: fail
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_INT32 
Sec_DecodeEvtAction(WE_VOID* pvSrcData,WE_UINT32 uiLen,WE_INT32 *piEvent,WE_VOID** pvDesData)
{
    St_Dcvt stObj = {0};
    WE_INT32 iEvent = 0;
    if((NULL == pvSrcData) || (NULL == piEvent) || (NULL == pvDesData))
    {
        return FALSE;
    }
    /*init*/
    Sec_DcvtInit(&stObj,M_SEC_DCVT_DECODE,pvSrcData,(WE_LONG)uiLen);
    /*decode*/
    if(!(Sec_DcvtInt32(&stObj,&iEvent)))
    {
        return FALSE;
    }
    *piEvent = iEvent;
    switch(iEvent)
    {    
    case G_Evt_CONFIRM:
        {
            St_ConfirmAction* pstConfirm = (St_ConfirmAction*)WE_MALLOC(sizeof(St_ConfirmAction));
            if(NULL == pstConfirm)
            {
                return FALSE;
            }
            (void)WE_MEMSET(pstConfirm,0,sizeof(St_ConfirmAction) );
            if(!Sec_CvtConfirmAction(&stObj,pstConfirm))
            {
                WE_FREE(pstConfirm);
                pstConfirm = NULL;
                return FALSE;
            }
            *pvDesData = pstConfirm;
        }
        break;
    default:
        *pvDesData = NULL;
        return FALSE;
    }
    return TRUE;
    
}

#endif

