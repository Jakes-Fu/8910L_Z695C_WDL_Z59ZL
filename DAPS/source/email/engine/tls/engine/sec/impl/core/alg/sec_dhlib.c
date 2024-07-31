/*==================================================================================================
HEADER NAME : sec_dhlib.c
MODULE NAME : SEC

  PRE-INCLUDE FILES DESCRIPTION
  
    
  GENERAL DESCRIPTION
  In this file,define the initial function prototype ,and will be update later.
  
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
Revision History

  Modification                   Tracking
  Date          Author            Number      Description of changes
  ----------   ------------    ---------   -------------------------------------------------------
  2007-04-11   tang            None         Draft
==================================================================================================*/

/***************************************************************************************
*   Include File Section
****************************************************************************************/


#include "sec_comm.h"
#ifdef M_SEC_CFG_DHE_RSA
#include "sec_dhlib.h"
/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
#define DH_FREE_SELF(x) {WE_FREE(x);x = NULL ;}

/***************************************************************************************************
*   Prototype Declare Section
***************************************************************************************************/
static WE_VOID   SEClib_DHUtDisposeMblk(St_UtMblk *pstPtr);
static WE_INT32  SEClib_DHSizeofKey2BerStringPkcs3(WE_VOID * pvKey);
static WE_UCHAR* SEClib_DHKey2BerStringPkcs3(WE_VOID * key, WE_UCHAR *dest);
static WE_INT32  SEClib_DHUtMpmGenerateDlKeys(WE_HANDLE   hSecHandle,WE_VOID * pvMachine,
                                             WE_VOID * pvP,  WE_VOID * pvQ, WE_VOID * pvG,
                                             WE_VOID * *ppPriv, WE_VOID * *ppPub);
static WE_VOID * SEClib_DHMakeX(WE_HANDLE   hSecHandle,WE_VOID * pvMachine, WE_VOID * pvQ);
static WE_VOID * SEClib_DHMakeY(WE_VOID * pvMachine,WE_VOID * pvP,WE_VOID * pvG, WE_VOID * x);
static WE_INT32  SEClib_DHGenerateKeyPkcs3(WE_HANDLE   hSecHandle,St_UtMblk *param,
                                          St_UtMblk *pstPrivBlk, St_UtMblk *pstPubBlk);
static WE_UCHAR *SEClib_DHBerString2ParametersPkcs3(WE_UCHAR *pucSrc, WE_VOID * *ppvP, 
                                                    WE_VOID * *g, WE_UINT32 *puiPrivateKeyLength);
static WE_INT32  SEClib_DHDeriveSharedPkcs3(St_UtMblk *param, St_UtMblk *pstYourPubKey, 
                                           St_UtMblk *myprivkey,St_UtMblk *pstOutput);
static WE_UCHAR *SEClib_DHBerstring2keyPkcs3(WE_UCHAR *pucSrc, WE_VOID * *ppvKey);
static WE_INT32  SEClib_DHConvertToStUtMblk(WE_UCHAR *pucInData, WE_UINT16 usIndataLen, St_UtMblk *key);
static WE_INT32  SEClib_DHConvertFromStUtMblk(WE_UCHAR **ppucOutData, WE_INT32 *piOutDataLen, St_UtMblk key);
static WE_INT32  SEClib_DHConvertParams(WE_UCHAR *pucDhP, WE_INT32 iDhPLen, 
                                       WE_UCHAR *pucDhG, WE_INT32 iDhGLen, St_UtMblk *pstParam);

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
        SEClib_DHConvertParams
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
        Convert dh parameter.
ARGUMENTS PASSED:
        WE_UCHAR *pucDhP[IN]:Pointer to the dhP.
        WE_INT32 iDhPLen[IN]:length of dhP.
        WE_UCHAR *pucDhG[IN]:Pointer to the dhG.
        WE_INT32 iDhGLen[IN]:length of dhG.
        St_UtMblk *pstParam[OUT]:the result of convert dh parameter.
RETURN VALUE:
        M_SEC_ERR_OK: success
        other: error.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_INT32 SEClib_DHConvertParams(WE_UCHAR *pucDhP, WE_INT32 iDhPLen, WE_UCHAR *pucDhG, WE_INT32 iDhGLen, St_UtMblk *pstParam)
{
    WE_UINT16       usMsgLen = 0;
    WE_UINT16       usHdrLen = 0;
    WE_UINT16       usPLen = 0; 
    WE_UINT16       usGLen = 0; 
    WE_UCHAR        ucPrependModZero = 0;
    WE_UCHAR        ucPrependExpZero = 0;
    WE_UINT16       usPDh = (WE_UINT16) iDhPLen;  
    WE_UINT16       usGDh = (WE_UINT16) iDhGLen; 
    WE_UCHAR        *pucP = NULL;
    WE_INT32        iLoop = 0 ;
    WE_UCHAR        ucBigEndian;
    WE_UINT16       usU = 0x0102;
    
    if (0x01 == *((WE_UCHAR*)&(usU))) /* Check how bytes are positioned on the current platform */
    {
        ucBigEndian = 1;
    }
    else
    {
        ucBigEndian = 0;
    }
    
    /* Calculate length of DER representation of the public key */
    /* usMLen is the length of the modulus (integer) header */
    /* eLen is the length of the exponent (integer) header */
    /* usHdrLen is the length of the SEQUENCE (encapsulating) header */
    if (((WE_UCHAR)*(pucDhP)) > 127)
    {
        ucPrependModZero = 1;
        usPDh++;
    }
    usPLen = 2;
    if (usPDh > 127)
    {
        usPLen++;
    }
    if (usPDh > 255)
    {
        usPLen++;
    }
    
    if (((WE_UCHAR)*(pucDhG)) > 127)
    {
        ucPrependExpZero = 1;
        usGDh++;
    }
    usGLen = 2;
    if (usGDh > 127)
    {
        usGLen++;
    }
    if (usGDh > 255)
    {
        usGLen++;
    }
    
    usMsgLen = (WE_UINT16)(usPLen + usPDh + usGLen + usGDh);
    
    usHdrLen = 2;
    if (usMsgLen > 127)
    {
        usHdrLen++;
    }
    if (usMsgLen > 255)
    {
        usHdrLen++;
    }
    (*pstParam).pucData = (WE_UCHAR *)WE_MALLOC((WE_UINT32)(usHdrLen + usMsgLen));
    if (!((*pstParam).pucData))
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (*pstParam).uiSize =(WE_UINT32)(usHdrLen + usMsgLen );
    /* DER encode params (ASN.1) */
    pucP = (*pstParam).pucData;
    *pucP++ = 0x30; /* SEQUENCE */
    switch (usHdrLen)
    {
    case 2:
        if (ucBigEndian)
        {
            *pucP++ = *(((WE_UCHAR*)&(usMsgLen))+1); /* write message length */
        }
        else
        {
            *pucP++ = *((WE_UCHAR*)&(usMsgLen)); /* write message length (1 byte) */
        }
        break;
    case 3: 
        *pucP++ = 0x81; /* LENGTH SPECIFIER - 1 Byte */
        if (ucBigEndian)
        {
            *pucP++ = *(((WE_UCHAR*)&(usMsgLen))+1); /* write message length */
        }
        else
        {
            *pucP++ = *((WE_UCHAR*)&(usMsgLen)); /* write message length */
        }
        break;
    case 4:
        *pucP++ = 0x82; /* LENGTH SPECIFIER - 2 Bytes */
        if (ucBigEndian)
        {
            *pucP++ = *((WE_UCHAR*)&(usMsgLen));
            *pucP++ = *(((WE_UCHAR*)&(usMsgLen))+1); /* write message length */
        }
        else
        {
            *pucP++ = *(((WE_UCHAR*)&(usMsgLen))+1); /* write message length */
            *pucP++ = *((WE_UCHAR*)&(usMsgLen));
        }
    } /* This provides for message lengths up to 65535 bits */
    
    *pucP++ = 0x02; /* INTEGER */
    switch (usPLen)
    {
    case 2:
        if (ucBigEndian)
        {
            *pucP++ = *(((WE_UCHAR*)&(usPDh))+1); /* write modulus length (1 byte) */
        }
        else
        {
            *pucP++ = *((WE_UCHAR*)&(usPDh));
        }
        break;
    case 3:
        *pucP++ = 0x81; /* LENGTH SPECIFIER */
        if (ucBigEndian)
        {
            *pucP++ = *(((WE_UCHAR*)&(usPDh))+1); /* write modulus length (1 byte) */
        }
        else
        {
            *pucP++ = *((WE_UCHAR*)&(usPDh));
        }
        break;
    case 4:
        *pucP++ = 0x82; /* LENGTH SPECIFIER */
        if (ucBigEndian)
        {
            *pucP++ = *((WE_UCHAR*)&(usPDh));
            *pucP++ = *(((WE_UCHAR*)&(usPDh))+1); /* write modulus length (2 bytes) */
        }
        else
        {
            *pucP++ = *(((WE_UCHAR*)&(usPDh))+1); /* write modulus length (2 bytes) */
            *pucP++ = *((WE_UCHAR*)&(usPDh));
        }
    }
    if (ucPrependModZero)
    {
        *pucP++ = 0x00;
    }
    for (iLoop=0; iLoop<iDhPLen; iLoop++) /* write modulus (original length  only) */
    {
        *pucP++ = pucDhP[iLoop];
    }
    *pucP++ = 0x02; /* INTEGER */
    switch (usGLen)
    {
    case 2:
        if (ucBigEndian)
        {
            *pucP++ = *(((WE_UCHAR*)&(usGDh))+1); /* write exponent length (1 byte) */
        }
        else
        {
            *pucP++ = *((WE_UCHAR*)&(usGDh));
        }
        break;
    case 3:
        *pucP++ = 0x81; /* LENGTH SPECIFIER */
        if (ucBigEndian)
        {
            *pucP++ = *(((WE_UCHAR*)&(usGDh))+1); /* write exponent length (1 byte) */
        }
        else
        {
            *pucP++ = *((WE_UCHAR*)&(usGDh));
        }
        break;
    case 4:
        *pucP++ = 0x82; /* LENGTH SPECIFIER */
        if (ucBigEndian)
        {
            *pucP++ = *((WE_UCHAR*)&(usGDh));
            *pucP++ = *(((WE_UCHAR*)&(usGDh))+1); /* write exponent length (2 bytes) */
        }
        else
        {
            *pucP++ = *(((WE_UCHAR*)&(usGDh))+1); /* write exponent length (2 bytes) */
            *pucP++ = *((WE_UCHAR*)&(usGDh));
        }
    }
    if (ucPrependExpZero)
    {
        *pucP++ = 0x00;
    }
    for (iLoop=0; iLoop<iDhGLen;iLoop++) /* write exponent (original length  only) */
    {
        *pucP++ = pucDhG[iLoop];
    }
    
    (*pstParam).uiUsed = usHdrLen + usMsgLen;
    return M_SEC_ERR_OK;
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHConvertToStUtMblk
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
        Convert structure.
ARGUMENTS PASSED:
        WE_UCHAR *pucInData[IN]:input data.
        WE_UINT16 usIndataLen[IN]:length of input data.
        St_UtMblk *pstKey[OUT]:Pointer the the key.
RETURN VALUE:
        error code.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_INT32 SEClib_DHConvertToStUtMblk(WE_UCHAR *pucInData, WE_UINT16 usIndataLen, St_UtMblk *pstKey)
{
    WE_UINT16       usMsgLen = 0;
    WE_UINT16       usMLen = 0;
    WE_UCHAR        ucPrependZero = 0;
    WE_UINT16       usM = usIndataLen;
    WE_UCHAR        *pucP = NULL;
    WE_INT32        iLoop = 0;
    WE_UCHAR        ucBigEndian = 0;
    WE_UINT16       usU = 0x0102;
    
    if (0x01 == *((WE_UCHAR*)&(usU))) /* Check how bytes are positioned on the current platform */
    {
        ucBigEndian = 1;
    }
    else
    {
        ucBigEndian = 0;
    }
    /* Calculate length of DER representation of the public key */
    /* usMLen is the length of the modulus (integer) header */
    if (((WE_UCHAR)*(pucInData)) > 127)
    {
        ucPrependZero = 1;
        usM++;
    }
    usMLen = 2;
    if (usM > 127)
    {
        usMLen++;
    }
    if (usM > 255)
    {
        usMLen++;
    }
    
    usMsgLen = (WE_UINT16)(usMLen + usM);
    
    (*pstKey).pucData = (WE_UINT8 *)WE_MALLOC((WE_UINT32)usMsgLen);
    if (!((*pstKey).pucData))
    {
        return M_SEC_ERR_INSUFFICIENT_MEMORY;
    }
    (*pstKey).uiSize = (WE_UINT32)usMsgLen ;
    /* DER encode key (ASN.1) */
    pucP = (*pstKey).pucData;
    *pucP++ = 0x02; /* INTEGER */
    switch (usMLen)
    {
    case 2:
        if (ucBigEndian)
        {
            *pucP++ = *(((WE_UCHAR*)&(usM))+1); /* write modulus length (1 byte) */
        }
        else
        {
            *pucP++ = *((WE_UCHAR*)&(usM));
        }
        break;
    case 3:
        *pucP++ = 0x81; /* LENGTH SPECIFIER */
        if (ucBigEndian)
        {
            *pucP++ = *(((WE_UCHAR*)&(usM))+1); /* write modulus length (1 byte) */
        }
        else
        {
            *pucP++ = *((WE_UCHAR*)&(usM));
        }
        break;
    case 4:
        *pucP++ = 0x82; /* LENGTH SPECIFIER */
        if (ucBigEndian)
        {
            *pucP++ = *((WE_UCHAR*)&(usM));
            *pucP++ = *(((WE_UCHAR*)&(usM))+1); /* write modulus length (2 bytes) */
        }
        else
        {
            *pucP++ = *(((WE_UCHAR*)&(usM))+1); /* write modulus length (2 bytes) */
            *pucP++ = *((WE_UCHAR*)&(usM));
        }
    }
    if (ucPrependZero)
    {
        *pucP++ = 0x00;
    }
    for (iLoop=0; iLoop<usIndataLen; iLoop++) /* write modulus (original length  only) */
    {
        *pucP++ = pucInData[iLoop];
    }
    (*pstKey).uiUsed = usMsgLen;
    (*pstKey).uiSize = usMsgLen;
    return SEC_ERR_OK;
    
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHUtDisposeMblk
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
        free memory.
ARGUMENTS PASSED:
        St_UtMblk *pstPtr[IN]:Pointer to the stucture.
RETURN VALUE:
        error code.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_VOID SEClib_DHUtDisposeMblk(St_UtMblk *pstPtr)
{
    if(pstPtr->pucData)
    {
        WE_FREE(pstPtr->pucData);
        pstPtr->pucData = NULL;
    }
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHGenerateKeyPkcs3
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
       generate key pkcs3.
ARGUMENTS PASSED:
        WE_HANDLE   hSecHandle[IN]::globle handle.
        St_UtMblk   *pstParam[IN]:dh parameter.
        St_UtMblk   *pstPrivBlk[OUT]:private key.
        St_UtMblk   *pstPubBlk[OUT]:public key.
RETURN VALUE:
        error code.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_INT32 SEClib_DHGenerateKeyPkcs3(WE_HANDLE   hSecHandle,St_UtMblk *pstParam, St_UtMblk *pstPrivBlk, St_UtMblk *pstPubBlk)
{
    
    WE_VOID *       pvP = NULL;         
    WE_VOID *       pvQ = NULL;     
    WE_VOID *       pvG = NULL;         
    WE_UINT32       uiPrivateKeyLength = 0; 
    WE_VOID *       pvMach;      
    WE_VOID *       pvPriv;      
    WE_VOID *       pvPub;       
    WE_UCHAR        *pucEnd;       
    WE_INT32        iRval = -1;   
    
    if(!(SEClib_DHBerString2ParametersPkcs3(pstParam->pucData, &pvP, &pvG, &uiPrivateKeyLength)))
    {
        return iRval;
    }    
    pvMach = We_MpmNewMach();
    if(!pvMach)
    {
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
    }
    
    if(uiPrivateKeyLength) 
    {
        We_Mpm2Power(pvMach, E_MPM_R0, uiPrivateKeyLength-1);
        pvQ = We_MpmBackUp(pvMach, E_MPM_R0, E_FALSE);
    }    
    if (SEClib_DHUtMpmGenerateDlKeys(  hSecHandle,pvMach,pvP, pvQ, pvG, &pvPriv, &pvPub)) 
    {
        if (pvQ)
        {
            We_MpmKillArc (pvQ);
        }
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
    }
    if (pvQ)
    {
        We_MpmKillArc (pvQ);
    }  
    pstPrivBlk->uiSize =(WE_UINT32)SEClib_DHSizeofKey2BerStringPkcs3(pvPriv);
    pstPrivBlk->pucData =(WE_UCHAR *)WE_MALLOC(pstPrivBlk->uiSize);
    if(! pstPrivBlk->pucData)
    {
        We_MpmKillArc(pvPriv);
        We_MpmDisposeMach(pvMach);
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
    }
    pucEnd = SEClib_DHKey2BerStringPkcs3(pvPriv, pstPrivBlk->pucData);
    if(!pucEnd)
    {
        We_MpmKillArc(pvPriv);
        We_MpmDisposeMach(pvMach);
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;        
    }
    pstPrivBlk->uiUsed = (WE_UINT32)(pucEnd-pstPrivBlk->pucData);
    pstPubBlk->uiSize = (WE_UINT32)SEClib_DHSizeofKey2BerStringPkcs3(pvPub);
    pstPubBlk->pucData = WE_MALLOC(pstPubBlk->uiSize);
    if(!(pstPubBlk->pucData))
    {
        We_MpmKillArc(pvPub);
        We_MpmKillArc(pvPriv);
        We_MpmDisposeMach(pvMach);
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;        
    }
    pucEnd = SEClib_DHKey2BerStringPkcs3(pvPub, pstPubBlk->pucData);
    if(!pucEnd)
    {
        We_MpmKillArc(pvPub);
        We_MpmKillArc(pvPriv);
        We_MpmDisposeMach(pvMach);
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
    }
    pstPubBlk->uiUsed = (WE_UINT32)(pucEnd-pstPubBlk->pucData);    
    iRval = 0;    
    We_MpmKillArc(pvPub);
    We_MpmKillArc(pvPriv);
    We_MpmDisposeMach(pvMach);
    We_MpmKillArc(pvP);
    We_MpmKillArc(pvG);
    return iRval;
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHUtMpmGenerateDlKeys
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
       generate dl key .
ARGUMENTS PASSED:
        WE_HANDLE   hSecHandle[IN]::globle handle.
        WE_VOID     * pvMachine[IN]:machine.
        WE_VOID     * pvP[IN]:Pointer to the dhP.
        WE_VOID     * pvQ[IN]:Pointer to the dhQ.
        WE_VOID     * pvG[IN]:Pointer to the dhG.
        WE_VOID     * *ppvPriv[OUT]:Pointer to the private key.
        WE_VOID     * *ppvPub[OUT]:Ponter to the public key.
RETURN VALUE:
        error code.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_INT32 SEClib_DHUtMpmGenerateDlKeys(WE_HANDLE   hSecHandle,WE_VOID * pvMachine, WE_VOID * pvP,
                                      WE_VOID * pvQ, WE_VOID * pvG, WE_VOID * *ppvPriv, WE_VOID * *ppvPub)
{
    if (!pvMachine || !pvP || !pvG || !ppvPriv || !ppvPub)
    {
        return -2;
    }   
    *ppvPriv = SEClib_DHMakeX( hSecHandle,pvMachine, pvQ ? pvQ : pvP);
    if(!(*ppvPriv))
    {
        return -1;
    }
    *ppvPub = SEClib_DHMakeY(pvMachine, pvP, pvG, *ppvPriv);
    if(!(*ppvPub))
    {
        We_MpmKillArc(*ppvPriv);
        return -1;
    }    
    return 0;
    
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHSizeofKey2BerStringPkcs3
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
       get the size of key to ber string .
ARGUMENTS PASSED:
        WE_VOID * pvKey[IN]:Pointer to the key.
RETURN VALUE:
        size of key.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_INT32 SEClib_DHSizeofKey2BerStringPkcs3(WE_VOID * pvKey)
{
    return (We_MpmSizeOfArc2BerInteger(pvKey));
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHKey2BerStringPkcs3
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
       convert key to ber string .
ARGUMENTS PASSED:
        WE_VOID * pvKey[IN]:Pointer to the key.
        WE_UCHAR *pucDest[OUT]:Pointer to the converted key.
RETURN VALUE:
        size of key.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_UCHAR* SEClib_DHKey2BerStringPkcs3(WE_VOID * pvKey, WE_UCHAR *pucDest)
{
    return (We_MpmArc2BerInteger(pvKey, pucDest));  
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHMakeX
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
       make x.
ARGUMENTS PASSED:
        WE_HANDLE   hSecHandle[IN]:globle handle.
        WE_VOID     * pvMachine[IN]:machine.
        WE_VOID     * pvQ[IN]:Pointer to the dhQ.
RETURN VALUE:
        x.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_VOID * SEClib_DHMakeX(WE_HANDLE   hSecHandle,WE_VOID * pvMachine, WE_VOID * pvQ)
{
    if(!pvMachine) 
    {
        return NULL;
    }
    if(!pvQ) 
    {
        return NULL;
    }    
    
    We_MpmRestore(pvMachine,E_MPM_R0,pvQ,E_TRUE);  
    if( We_MpmFlagSet(pvMachine,E_MPM_OVERFLOW) )
    {
        return NULL;
    }
    We_MpmSqrt(pvMachine,E_MPM_R1,E_MPM_R2,E_MPM_R0); 
    if( We_MpmFlagSet(pvMachine,E_MPM_OVERFLOW) )
    {
        return NULL;
    }
    We_MpmLToMpm(pvMachine, E_MPM_R3, 2);         
    if( We_MpmFlagSet(pvMachine,E_MPM_OVERFLOW) )
    {
        return NULL;
    }
    We_MpmSub(pvMachine, E_MPM_R4, E_MPM_R0, E_MPM_R3); 
    if(We_MpmPrng2Mpm(  hSecHandle,pvMachine, (WE_INT32)E_MPM_R3, (WE_INT32)E_MPM_R1,(WE_INT32)E_MPM_R4 ))
    {
        return NULL;
    }
    return We_MpmBackUp(pvMachine,E_MPM_R3,E_FALSE);
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHMakeY
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
       make Ys.
ARGUMENTS PASSED:
        WE_HANDLE   hSecHandle[IN]:globle handle.
        WE_VOID     * pvMachine[IN]:machine.
        WE_VOID     * pvG[IN]:Pointer to the dhG.
        WE_VOID     * pvX[IN]:Pointer to the x.
RETURN VALUE:
        Ys.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_VOID * SEClib_DHMakeY(WE_VOID * pvMachine,WE_VOID * pvP,WE_VOID * pvG, WE_VOID * pvX)
{
    if(!pvMachine) 
    {
        return NULL;
    }
    if(!pvP) 
    {
        return NULL;
    }
    if(!pvG) 
    {
        return NULL;
    }
    if(!pvX) 
    {
        return NULL;
    }   
    We_MpmRestore(pvMachine,E_MPM_R0,pvG,E_TRUE);  
    if( We_MpmFlagSet(pvMachine,E_MPM_OVERFLOW) )
    {
        return NULL;
    }
    We_MpmRestore(pvMachine,E_MPM_R1,pvX,E_TRUE);  
    if( We_MpmFlagSet(pvMachine,E_MPM_OVERFLOW) )
    {
        return NULL;
    }
    We_MpmRestore(pvMachine,E_MPM_RMOD,pvP,E_TRUE);  
    if( We_MpmFlagSet(pvMachine,E_MPM_OVERFLOW) )
    {
        return NULL;
    }          
    We_MpmExpm(pvMachine,E_MPM_R2,E_MPM_R0,E_MPM_R1);
    if( We_MpmFlagSet(pvMachine,E_MPM_OVERFLOW) )
    {
        return NULL;
    }
    return We_MpmBackUp(pvMachine,E_MPM_R2,E_FALSE);
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHBerString2ParametersPkcs3
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
      Conert ber string to pkcs3 parameter.
ARGUMENTS PASSED:
        WE_UCHAR *pucSrc[IN]:Pointer to the Src.
        WE_VOID * *ppvP[OUT]:Pointer to the dhP.
        WE_VOID * *ppvG[OUT]:Pointer to the dhG.
        WE_UINT32 *puiPrivateKeyLength[OUT]:length of Private key.
RETURN VALUE:
        private key.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_UCHAR *SEClib_DHBerString2ParametersPkcs3(WE_UCHAR *pucSrc, WE_VOID * *ppvP, WE_VOID * *ppvG, WE_UINT32 *puiPrivateKeyLength)
{
    WE_UINT32       uiType = 0;  
    WE_LONG         lSize;  
    WE_UCHAR        *pucEnd;   
    WE_INT32        iTmp = 0;   
    
    pucSrc = We_MbkAsn1BerTag2Type(pucSrc, &uiType);
    if(!pucSrc) 
    {
        return NULL;
    }
    if (uiType != ASN1_SEQUENCE) 
    {
        return NULL;
    }   
    pucSrc = WE_MBK_ASN1_BERLEN2SIZE(pucSrc, &lSize);
    if(!pucSrc)
    {
        return NULL;
    }
    pucEnd = pucSrc+lSize;
    pucSrc = We_MpmBerInteger2Arc(pucSrc, ppvP);
    if (!pucSrc) 
    {
        return NULL;
    } 
    pucSrc = We_MpmBerInteger2Arc(pucSrc, ppvG);
    if (!pucSrc) 
    {
        We_MpmKillArc(*ppvP);
        return NULL;
    }  
    if (pucSrc == pucEnd) 
    {
        *puiPrivateKeyLength = 0;
        return pucSrc;
    }    
    pucSrc = We_MbkAsn1Berinteger2s32(pucSrc, &iTmp);
    if (!pucSrc) 
    {
        We_MpmKillArc(*ppvP);
        We_MpmKillArc(*ppvG); 
        return NULL;
    }
    *puiPrivateKeyLength = (WE_UINT32)iTmp;
    if(pucSrc != pucEnd) 
    {
        We_MpmKillArc(*ppvP);
        We_MpmKillArc(*ppvG);
        return NULL;
    }
    return pucSrc;  
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHDeriveSharedPkcs3
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
      Derive Pkcs3 Shared key .
ARGUMENTS PASSED:
        St_UtMblk *pstParam[IN]:Pointer to the parameter.
        St_UtMblk *pstYourPubKey[OUT]:Pointer to the public key.
        St_UtMblk *myprivkey[OUT]:Pointer to the private key.
        St_UtMblk *pstOutput[OUT]:Pointer to the shared key.
RETURN VALUE:
        error code.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_INT32 SEClib_DHDeriveSharedPkcs3(St_UtMblk *pstParam, St_UtMblk *pstYourPubKey, 
                                           St_UtMblk *myprivkey, St_UtMblk *pstOutput)
{
    WE_VOID     *pvP = NULL;         
    WE_VOID     *pvG = NULL;         
    WE_VOID     *pvPriv = NULL;      
    WE_VOID     *pvPub = NULL;       
    WE_VOID     *pvShared = NULL;    
    WE_VOID     *pvMach= NULL;      
    WE_UINT32    uiTmp = 0;       
    WE_UCHAR     *pucEnd;       
    WE_INT32     iRval = -1; 
    
    if(!(SEClib_DHBerString2ParametersPkcs3(pstParam->pucData, &pvP, &pvG, &uiTmp)))
    {
        /*couldn't get parameters */
        return iRval;
    }
    if(!(SEClib_DHBerstring2keyPkcs3(myprivkey->pucData, &pvPriv)))
    {
        /*couldn't get private key */
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
        
    }
    if(!(SEClib_DHBerstring2keyPkcs3(pstYourPubKey->pucData, &pvPub)))
    {
        /*couldn't get public key */
        We_MpmKillArc(pvPriv);
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
        
    }
    pvMach = We_MpmNewMach();
    if(!pvMach) 
    {
        /*couldn't start mpm session */
        We_MpmKillArc(pvPub);
        We_MpmKillArc(pvPriv);
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
    }
    We_MpmRestore(pvMach, E_MPM_RMOD, pvP, E_TRUE);
    We_MpmRestore(pvMach, E_MPM_R0, pvPriv, E_TRUE);
    We_MpmRestore(pvMach, E_MPM_R1, pvPub, E_TRUE);
    We_MpmExpm(pvMach, E_MPM_R2, E_MPM_R1, E_MPM_R0);
    
    pvShared = We_MpmBackUp(pvMach, E_MPM_R2, E_TRUE);
    if(!pvShared)
    {
        /*couldn't get pvShared key*/
        We_MpmDisposeMach(pvMach);
        We_MpmKillArc(pvPub);
        We_MpmKillArc(pvPriv);
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
    }       
    pstOutput->uiSize =(WE_UINT32)SEClib_DHSizeofKey2BerStringPkcs3(pvShared);
    pstOutput->pucData = WE_MALLOC(pstOutput->uiSize);
    if(!(pstOutput->pucData))
    {
        We_MpmKillArc(pvShared);
        We_MpmDisposeMach(pvMach);
        We_MpmKillArc(pvPub);
        We_MpmKillArc(pvPriv);
        We_MpmKillArc(pvP);
        We_MpmKillArc(pvG);
        return iRval;
    }
    pucEnd = SEClib_DHKey2BerStringPkcs3(pvShared, pstOutput->pucData);
    if(!pucEnd) 
    {
        /*ouldn't store pvShared key*/
        DH_FREE_SELF(pstOutput->pucData);
    } 
    else 
    {
        iRval = 0;
        pstOutput->uiUsed = (WE_UINT32)(pucEnd-pstOutput->pucData);
    }    
    We_MpmKillArc(pvShared);
    We_MpmDisposeMach(pvMach);
    We_MpmKillArc(pvPub);
    We_MpmKillArc(pvPriv);
    We_MpmKillArc(pvP);
    We_MpmKillArc(pvG);
    return iRval;
}                       
/*=====================================================================================
FUNCTION: 
        SEClib_DHBerstring2keyPkcs3
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
      convert ber string to pkcs3 key.
ARGUMENTS PASSED:
        WE_UCHAR *pucSrc[IN]:Pointer to the src.
        WE_VOID * *ppvKey[OUT]:Pointer to the pkcs3 key.
RETURN VALUE:
        error code.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_UCHAR *SEClib_DHBerstring2keyPkcs3(WE_UCHAR *pucSrc, WE_VOID * *ppvKey)
{
    return(We_MpmBerInteger2Arc(pucSrc, ppvKey));
}
/*=====================================================================================
FUNCTION: 
        SEClib_DHConvertFromStUtMblk
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
      convert structure.
ARGUMENTS PASSED:
        WE_UCHAR **ppucOutData[OUT]:Pointer to the output data.
        WE_INT32 *piOutDataLen[OUT]:Pointer to the length of Output Data.
        St_UtMblk stKey[IN]:key.
RETURN VALUE:
        error code.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
static WE_INT32 SEClib_DHConvertFromStUtMblk(WE_UCHAR **ppucOutData, WE_INT32 *piOutDataLen, St_UtMblk stKey)
{
    WE_INT32    iLoop = 0;
    WE_UCHAR    *pucP = NULL;
    WE_UINT16   usSize = 0, usTmpStep = 0;
    
    pucP = (WE_UCHAR *)stKey.pucData;
    *piOutDataLen = (WE_INT32)stKey.uiSize;
    if(*pucP != 0x02)						/*Check that it is an INTEGER*/
    {
        return M_SEC_ERR_GENERAL_ERROR;
    }
    pucP++;
    *piOutDataLen-=1;
    We_X509DERtoSize(pucP, &usTmpStep, &usSize);/*Get the length of INTEGER */
    pucP +=usTmpStep;
    *piOutDataLen-= usTmpStep;
    
    if (*pucP==0)									/*If Modulus starts with a "\x00" */
    {
        pucP++;
        *piOutDataLen-=1;
    }
    /*
    *ppucOutData = (WE_UCHAR *)WE_MALLOC((WE_UINT32)(*piOutDataLen));
    if(*ppucOutData == NULL)
    {       
        return M_SEC_ERR_GENERAL_ERROR;
    }
*/    
    for (iLoop=0; iLoop<*piOutDataLen; iLoop++)
    {
        (*ppucOutData)[iLoop] = *pucP++;
    }
    
    return SEC_ERR_OK;
}
/*=====================================================================================
FUNCTION: 
        SEClib_generateDHKeyPair
CREATE DATE: 
        2007-04-10
AUTHOR: 
        tang
DESCRIPTION:
        generate dh key.
ARGUMENTS PASSED:
        WE_HANDLE   hSecHandle[IN]:gloable handle.
                WE_UCHAR *pucDhP[IN]:Pointer to the dhP.
        WE_INT32 iDhPLen[IN]:length of dhP.
        WE_UCHAR *pucDhG[IN]:Pointer to the dhG.
        WE_INT32 iDhGLen[IN]:length of dhG.
        WE_UCHAR *pucDhYs[IN]:Public key from sever.
        WE_INT32 iDhYsLen[IN]:length of dhYs.
        WE_UCHAR **ppucClientPubKey[OUT]:the client public key.
        WE_INT32 *piClientPubkeyLen[OUT]:pointer to the length of client public key.
        WE_UCHAR **ppucSharedKey[OUT]:pointer to the shared key.
        WE_INT32 *piSharedKeyLen[OUT]:length of shared key. 
RETURN VALUE:
        M_SEC_ERR_OK: success
        other: error.
USED GLOBAL VARIABLES:
        none.    
USED STATIC VARIABLES:
        none.
CALL BY:
        Omit.
IMPORTANT NOTES:
        Omit.
=====================================================================================*/
WE_INT32 SEClib_generateDHKeyPair(WE_HANDLE   hSecHandle,WE_UCHAR *pucDhP, WE_INT32 iDhPLen,
                                  WE_UCHAR *pucDhG, WE_INT32 iDhGLen, WE_UCHAR *pucDhYs, WE_INT32 iDhYsLen, 
                                  WE_UCHAR **ppucClientPubKey, WE_INT32 *piClientPubkeyLen,
                                  WE_UCHAR **ppucSharedKey, WE_INT32 *piSharedKeyLen )
{
    WE_INT32        iRes = 0;
    St_UtMblk       stParam ={0,0,0};
    St_UtMblk       stClientPrivblk={0,0,0};
    St_UtMblk       stClientPubBlk={0,0,0}; 
    St_UtMblk       stServerPubBlk={0,0,0}; 
    St_UtMblk       stShared={0,0,0};
    
    /*convert the parameters ppvP (params.pucDhP) and g (params.pucDhG) as a BER-encoded string within param*/ 
    iRes= SEClib_DHConvertParams(pucDhP, iDhPLen, pucDhG, iDhGLen, &stParam);
    if (iRes != SEC_ERR_OK)
    {
        return iRes;
    }
    /*convert (params.pucDhYs) to Baltimore's "keyType"*/
    iRes= SEClib_DHConvertToStUtMblk(pucDhYs, (WE_UINT16) iDhYsLen, &stServerPubBlk);
    if (iRes != SEC_ERR_OK)
    {
        if(stParam.pucData)
        {
            WE_FREE(stParam.pucData);
        }
        return iRes;
    }
    
    /* generate client DH key-pair*/
    if (SEClib_DHGenerateKeyPkcs3( hSecHandle, &stParam, &stClientPrivblk, &stClientPubBlk))
    {
        SEClib_DHUtDisposeMblk (&stServerPubBlk);
        SEClib_DHUtDisposeMblk (&stParam);
        return M_SEC_ERR_GENERAL_ERROR;
    }
    
    /* derive PKCS#3-style Diffie-Hellman stShared public key*/
    if (SEClib_DHDeriveSharedPkcs3 (&stParam, &stServerPubBlk, &stClientPrivblk, &stShared))
    {
        SEClib_DHUtDisposeMblk (&stServerPubBlk);
        SEClib_DHUtDisposeMblk (&stParam);
        WE_FREE (stClientPrivblk.pucData);
        WE_FREE (stClientPubBlk.pucData);
        return M_SEC_ERR_GENERAL_ERROR;
    }
    
    SEClib_DHUtDisposeMblk (&stParam);
    SEClib_DHUtDisposeMblk (&stServerPubBlk);
    WE_FREE (stClientPrivblk.pucData);
    
    iRes = SEClib_DHConvertFromStUtMblk(ppucClientPubKey, piClientPubkeyLen, stClientPubBlk);
    if (iRes != M_SEC_ERR_OK)
    {
        WE_FREE (stClientPubBlk.pucData);
        WE_FREE (stShared.pucData);
        return M_SEC_ERR_GENERAL_ERROR;
    }
    
    WE_FREE (stClientPubBlk.pucData);
    
    iRes = SEClib_DHConvertFromStUtMblk(ppucSharedKey, piSharedKeyLen, stShared);
    if (iRes != M_SEC_ERR_OK)
    {
        WE_FREE (stShared.pucData);
        return M_SEC_ERR_GENERAL_ERROR;
    }
    
    WE_FREE (stShared.pucData);
    return M_SEC_ERR_OK;
    
}
#endif

