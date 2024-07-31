#include "itls.h"
#include "tlscust.h"
E_ITLS_ERROR ITls_Release(ITls * pMe)
{
    WE_INT32 iRet = -1;
    if (NULL == pMe)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }    
    iRet = TlsCust_Release(pMe);
    if (iRet != 0)
    {
        return E_ITLS_ERROR_WAP_TLS;
    }     
    return E_ITLS_ERROR_OK;
}
E_ITLS_ERROR ITls_RegClientEvtCB(ITls * pMe,WE_HANDLE hPrivData,WE_HANDLE pcbSecEvtFunc)
{
    WE_INT32 iRet = -1;

    if (NULL == pMe)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }    
    
    iRet = TlsCust_RegRespCB(pMe,(Fn_ITlsRespCb)pcbSecEvtFunc,hPrivData);
    if (iRet == 0)
    {
        return E_ITLS_ERROR_OK;
    }
    else
    {
        return E_ITLS_ERROR_WAP_TLS;
    }     
}
E_ITLS_ERROR ITls_ProcTlsData(ITls * pMe, WE_INT32 iConnId,E_TLS_DATA_TYPE eDataType,WE_VOID *pvData,WE_INT32 iDataLen)
{
  WE_INT32 iRet = -1;
    if (NULL == pMe)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }
    iRet = TlsCust_ProcData(pMe,
                         iConnId,
                         eDataType,
                         pvData,
                         iDataLen);
    if (iRet >= 0)
    {
        return E_ITLS_ERROR_OK;
    }
    else
    {
        return E_ITLS_ERROR_WAP_TLS;
    }     
}
E_ITLS_ERROR ITls_ResumeTlsHandShake(ITls * pMe, WE_INT32 iConnId,St_TlsOpt *pstOpt)
{
    WE_INT32 iRet = -1;

    if (NULL == pMe)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }    
    iRet = TlsCust_ResumeHandShake(pMe,iConnId,pstOpt);
    if (iRet == 0)
    {
        return E_ITLS_ERROR_OK;
    }
    else
    {
        return E_ITLS_ERROR_WAP_TLS;
    }  
}
E_ITLS_ERROR ITls_SetOptPara(ITls * pMe, St_TlsOpt *pstOpt)
{
    WE_INT32 iRet = -1;
    if (NULL == pMe)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }
    iRet = TlsCust_SetPara(pMe, pstOpt);
    if (iRet == 0)
    {
        return E_ITLS_ERROR_OK;
    }
    else
    {
        return E_ITLS_ERROR_WAP_TLS;
    } 
}
E_ITLS_ERROR ITls_ViewServerCert(ITls * pMe,WE_CHAR * pcHost, WE_VOID **ppvContent)
{
    WE_INT32 iRet = -1;

    if (NULL == pMe)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }
    iRet = TlsCust_ViewSvrCert(pMe,pcHost,ppvContent);
    if (iRet == 0)
    {
        return E_ITLS_ERROR_OK;
    }
    else
    {
        return E_ITLS_ERROR_WAP_TLS;
    } 
}
E_ITLS_ERROR ITls_BeginHandeShake(ITls * pMe, WE_INT32 iConnId,WE_UCHAR *pucHost,
                             WE_BOOL bUseProxy,WE_UCHAR *pucAddr,WE_UINT16 usPort)
{
    WE_INT32 iRet = -1;
    if (NULL == pMe)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }
    iRet = TlsCust_StartHandShake(pMe, iConnId,pucHost,bUseProxy,pucAddr,usPort);
    if (iRet == 0)
    {
        return E_ITLS_ERROR_OK;
    }
    else
    {
        return E_ITLS_ERROR_WAP_TLS;
    } 
}
E_ITLS_ERROR ITls_CloseConn(ITls *pMe,WE_INT32 iConnId)
{
    WE_INT32 iRet = -1;
    if (NULL == pMe)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }
    iRet = TlsCust_Close(pMe,iConnId);
    if (iRet == 0)
    {
        return E_ITLS_ERROR_OK;
    }
    else
    {
        return E_ITLS_ERROR_WAP_TLS;
    }       
}
WE_INT32 ITls_ClsCreateInstance(void **ppMod)
{
    WE_INT32 iRet = -1;
    if (NULL == ppMod)
    {
        return E_ITLS_ERROR_INVALID_PARAM;
    }
    iRet = TlsCust_Init((ITLS**)ppMod);
    if (iRet == 0)
    {
        return E_ITLS_ERROR_OK;
    }
    else
    {
        return E_ITLS_ERROR_WAP_TLS;
    }     
    return 0;
}
