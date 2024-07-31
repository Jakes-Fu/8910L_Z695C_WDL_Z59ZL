
#include "Tlscust.h" 
#include "itls.h"
#include "ssl_api.h"
#include "eme_utils_api.h"

#define WAP_TLS_OPT_CANCEL_INDEX   0
#define WAP_TLS_OPT_USER_CERT_INDEX 1
#define WAP_TLS_OPT_ERR_RESP_INDEX  2
#define WAP_TLS_OPT_PRIVATE_KEY_INDEX 3
#define WAP_TLS_OPT_ROOT_CERT_INDEX   4
#define WAP_TLS_OPT_VERSION_INDEX     5
#define WAP_TLS_OPT_USER_CERTS_INDEX  6
#define WAP_TLS_OPT_COUNT             7

#define MAX_INSTANCE_NUM  10
typedef struct tagSt_TlsInstance
{
	SSL_HANDLE  hSSl;
    WE_HANDLE  hTls;
	WE_INT32 fd;
	SSL_CERTINFO_T* pstCert;
	WE_CHAR* pcHost;
	WE_UINT16 usPort;
	
}St_TlsInstance;
typedef struct tagSt_TlsHandle
{
	St_TlsInstance* apstTlsInst[MAX_INSTANCE_NUM];
    WE_UINT8     ucProtoVer;
    WE_HANDLE   hPrivData; /*for app callback*/
    WE_HANDLE   hcbRespunc;/*app callback*/	
}St_TlsHandle;

static void _TlsCust_SecurityPostMessage(void* pvInst,WE_UINT32 uiMsg)
{
    St_TlsRespData stResp = {0};
	St_TlsInstance* pstInst = (St_TlsInstance*)pvInst;
	St_TlsHandle* pstTls = (St_TlsHandle*)pstInst->hTls;
	stResp.iConnId = pstInst->fd;

   switch(uiMsg)
   	{
	   	case SSL_SEND_MESSAGE_HANDSHAKE_SUCC:
			{
			  	stResp.iErrCode = G_WAP_ERR_TLS_FINISH_HANDSHAKE_OK;
			  	break;
			}			
		case SSL_SEND_MESSAGE_FAIL:
			{
			  	stResp.iErrCode = G_WAP_ERR_TLS_HANDSHAKE_FAILURE;
			  	break;
			}
		case SSL_SEND_MESSAGE_CLOSE_BY_SERVER:
			{
			  	stResp.iErrCode = G_WAP_ERR_TLS_SERVER_CLOSE;
			  	break;
			}			
		case SSL_SEND_MESSAGE_CANCLED_BY_USER:
			{
			  	stResp.iErrCode = G_WAP_ERR_TLS_CANCEL;
			  	break;
			}
   	}
    if(NULL != pstTls->hcbRespunc)
    {
        ((Fn_ITlsRespCb)(pstTls->hcbRespunc))(E_TLS_INFO,
                            (WE_VOID*)&stResp,
                            pstTls->hPrivData);
    }
    else
    {
        EME_UTILS_LOG_INFO(("_TlsCust_SecurityPostMessage hcbRespunc is NULL"));
    }
    
}
static void _TlsCust_SecurityDecrypostDataOutput(void* pvInst,WE_UINT8* pucData,WE_UINT32 uiLen)
{
    St_TlsRespData stResp = {0};
	St_TlsInstance* pstInst = (St_TlsInstance*)pvInst;
	St_TlsHandle* pstTls = (St_TlsHandle*)pstInst->hTls;

	stResp.iConnId = pstInst->fd;	
    stResp.iRespLen = uiLen;
    stResp.pvRespData = pucData;
    if(NULL != pstTls->hcbRespunc)
    {
        ((Fn_ITlsRespCb)(pstTls->hcbRespunc))(E_TLS_DATA_TO_APP,
                                (WE_VOID*)&stResp,
                                pstTls->hPrivData); 
	    /*notify ssl free*/
	    SSL_AsyncMessageProc(pstInst->hSSl,SSL_RECV_MESSAGE_RECV_SUCC,uiLen);
    }
    else
    {
        EME_UTILS_LOG_INFO(("_TlsCust_SecurityDecrypostDataOutput hcbRespunc is NULL"));
    }
}
static void _TlsCust_SecurityEncryptDataOutput(void* pvInst,WE_UINT8* pucData,WE_UINT32 uiLen)
{
    St_TlsRespData stResp = {0};
    St_TlsInstance* pstInst = (St_TlsInstance*)pvInst;
	St_TlsHandle* pstTls = (St_TlsHandle*)pstInst->hTls;
	
	stResp.iConnId = pstInst->fd;
    stResp.iRespLen = uiLen;
    stResp.pvRespData = pucData; //data encrypted
    if(NULL != pstTls->hcbRespunc)
    {
        ((Fn_ITlsRespCb)(pstTls->hcbRespunc))(E_TLS_DATA_TO_SVR,
                                (WE_VOID*)&stResp,
                                pstTls->hPrivData); 
	    /*notify ssl free*/
	    SSL_AsyncMessageProc(pstInst->hSSl,SSL_RECV_MESSAGE_SEND_SUCC,uiLen);
    }
    else
    {
        EME_UTILS_LOG_INFO(("_TlsCust_SecurityEncryptDataOutput hcbRespunc is NULL"));
    }
}
/*通知上层证书无法验证*/
static void _TlsCust_SecurityShowCert(void* pvInst,WE_UINT8* pucCertName,WE_UINT8*pucCert)
{
    St_TlsRespData stResp = {0};
    St_TlsInstance* pstInst = (St_TlsInstance*)pvInst;
    St_TlsHandle* pstTls = (St_TlsHandle*)pstInst->hTls;
	
    stResp.iConnId = pstInst->fd;
    stResp.iErrCode = G_WAP_ERR_TLS_VERIFY_CERT_ERR;////////证书验证失败
    if(NULL != pstTls->hcbRespunc)
    {
        ((Fn_ITlsRespCb)(pstTls->hcbRespunc))(E_TLS_INFO,
                                (WE_VOID*)&stResp,
                                pstTls->hPrivData); 
    }
    else
    {
        EME_UTILS_LOG_INFO(("_TlsCust_SecurityShowCert hcbRespunc is NULL"));
    }
}
static WE_INT32 _TlsCust_GetOptIndex(WE_INT32 iId)
{
    switch (iId)
    {
        case E_TLS_OPT_CANCEL:
            return WAP_TLS_OPT_CANCEL_INDEX;
        case E_TLS_OPT_ERR_RESP:
            return WAP_TLS_OPT_ERR_RESP_INDEX;                
        case E_TLS_OPT_PRIVATE_KEY:
            return WAP_TLS_OPT_PRIVATE_KEY_INDEX;
        case E_TLS_OPT_ROOT_CERT:
            return WAP_TLS_OPT_ROOT_CERT_INDEX;
        case E_TLS_OPT_USER_CERT:
            return WAP_TLS_OPT_USER_CERT_INDEX;
        case E_TLS_OPT_USER_CERTS:
            return WAP_TLS_OPT_USER_CERTS_INDEX;
        case E_TLS_OPT_VERSION:
            return WAP_TLS_OPT_VERSION_INDEX;
        default:
            break;
    }
    return -1;
}
static WE_HANDLE _TlsCust_CrateInstance(WE_INT32 fd,WE_HANDLE hTls)
{
   St_TlsInstance* pstInstance = NULL;
   St_TlsHandle * pstTls = (St_TlsHandle*)hTls;;
   
   pstInstance = (St_TlsInstance*)TLS_MALLOC(TLS_SIZEOF(St_TlsInstance));
   if (!pstInstance)
   {
   	  return NULL;
   }
   TLS_MEMSET(pstInstance,0,TLS_SIZEOF(St_TlsInstance));
   pstInstance->fd = fd;
   pstInstance->hTls = hTls;
   pstInstance->hSSl = SSL_Create(pstInstance,fd,SSL_ASYNC);

   pstTls->apstTlsInst[fd] = pstInstance;
   return pstInstance;
}
WE_INT32 TlsCust_Init(void** ppHandle)
{
	St_TlsHandle* pstTls = NULL;
    pstTls = TLS_MALLOC(TLS_SIZEOF(St_TlsHandle));
    if (!pstTls)
    {
    	return -1;
    }
    TLS_MEMSET(pstTls,0,TLS_SIZEOF(St_TlsHandle));
    pstTls->ucProtoVer = SSL_PROTOCOLSSL;
   *ppHandle = pstTls;
   SSL_Init();
   return 0;
}
WE_INT32 TlsCust_Release(ITLS *pITls)
{
    St_TlsHandle * pstTls = NULL;
    WE_INT32 i = 0;

    pstTls = (St_TlsHandle*)pITls;
    if (!pstTls)
    {
        return -1;
    }
    for (i = 0;i<MAX_INSTANCE_NUM;i++)
	{
		if (pstTls->apstTlsInst[i])
		{
			TlsCust_Close(pITls, i);
		}
	}
    TLS_FREE(pstTls);
	SSL_Destroy();
    return 0;
}

WE_INT32 TlsCust_Close(ITLS* pvTls,WE_INT32 iId)
{
	St_TlsHandle* pstTls = (St_TlsHandle*)pvTls;
    St_TlsInstance *pstInst = NULL;

	pstInst = pstTls->apstTlsInst[iId];
	if (!pstInst)
	{
		return -1;
	}
	if (pstInst->pcHost)
	{
	   TLS_FREE(pstInst->pcHost);
	   pstInst->pcHost = NULL;
	}
	if (pstInst->pstCert)
	{
		SSL_FreeCurrentCertInfo(pstInst->pstCert);
		pstInst->pstCert = NULL;
	}
    SSL_Close(pstInst->hSSl,SSL_ASYNC);
	TLS_FREE(pstInst);
	pstTls->apstTlsInst[iId] = NULL;
    return 0;
}
WE_INT32 TlsCust_StartHandShake
(
    ITLS *pvTls,
    WE_INT32 iConnId,
    WE_UCHAR* pucHost,
    WE_BOOL bUseProxy,
    WE_UCHAR *pucAddr,
    WE_UINT16 usPort
)
{
	St_TlsHandle* pstTls = (St_TlsHandle*)pvTls;
    St_TlsInstance *pstInst = NULL;
    SSL_CALLBACK_T  astCbFun = {0};
	pstInst = _TlsCust_CrateInstance(iConnId,pvTls);
    pstInst->pcHost = TLS_STRDUP(pucHost);
	pstInst->usPort = usPort;
	
   astCbFun.decryptout_cb = _TlsCust_SecurityDecrypostDataOutput;
   astCbFun.encryptout_cb = _TlsCust_SecurityEncryptDataOutput;
   astCbFun.postmessage_cb = _TlsCust_SecurityPostMessage;
   astCbFun.showcert_cb = _TlsCust_SecurityShowCert;

   SSL_AsyncRegCallback(pstInst->hSSl,&astCbFun);
   /*
    SSL_AsyncRegMessageCb(pstInst,(SecurityPostMessage)_TlsCust_SecurityPostMessage);
	SSL_AsyncRegRecvDataCb(pstInst, (SecurityDecrypostDataOutput)_TlsCust_SecurityDecrypostDataOutput);
    SSL_AsyncRegSendDataCb(pstInst, (SecurityEncryptDataOutput)_TlsCust_SecurityEncryptDataOutput);
    SSL_AsyncRegShowCertCb(pstInst, (SecurityShowCertInfo) _TlsCust_SecurityShowCert);
    */
	SSL_ProtocolChoose(pstInst->hSSl,pstTls->ucProtoVer,SSL_ASYNC);	
	SSL_HandShake(pstInst->hSSl,pucHost,usPort,SSL_ASYNC);
	return 0;
}
WE_INT32 TlsCust_ViewSvrCert
(
    ITLS *pITls,
    WE_CHAR* pcHost,
    WE_VOID **ppvContent
)
{
	St_TlsHandle* pstTls = (St_TlsHandle*)pITls;
	WE_INT32 i = 0;
	St_TlsInstance* pstInst = NULL;

    if ((NULL == pITls) || (NULL == ppvContent))
    {
        return -1;
    }
    for (i = 0;i<MAX_INSTANCE_NUM;i++)
	{
	    pstInst = pstTls->apstTlsInst[i];
		if (TLS_MEMCMP(pstInst->pcHost, pcHost,TLS_STRLEN(pcHost))== 0)
		{
			break;
		}
	}
	if (pstInst)
    {
    	SSL_GetCurrentCertInfo(pstInst->hSSl,&pstInst->pstCert);
		*ppvContent = pstInst->pstCert;
        /*don't free here
		if (pstInst->pstCert)
		{
			SSL_FreeCurrentCertInfo(pstInst->pstCert);
			pstInst->pstCert = NULL;
		}		*/
	}
    return 0;
}

WE_INT32 TlsCust_RegRespCB
(
    ITLS *pITls,
    Fn_ITlsRespCb pcb,
    WE_HANDLE hPrivData
)
{
	St_TlsHandle* pstTls = (St_TlsHandle*)pITls;
	if (!pstTls)
	{
		return -1;
	}
	pstTls->hcbRespunc = (WE_HANDLE)pcb;
	pstTls->hPrivData = hPrivData;
	return 0;
}

WE_INT32 TlsCust_ProcData
(
    ITLS *pITls,
    WE_INT32 iConnId,
    E_TLS_DATA_TYPE eDataType,
    WE_VOID* pvData,
    WE_INT32 iDataLen
)
{
    WE_INT32 iResult = -1;
	St_TlsInstance* pstInst = NULL;
	St_TlsHandle* pstTls = (St_TlsHandle*)pITls;
    if (!pstTls)
    {
        return -1;
    }
    pstInst = pstTls->apstTlsInst[iConnId];
    if (!pstInst)
    {
    	return -1;
    }
    switch (eDataType)
    {
        case E_TLS_DATA_FROM_APP:// APP DATA
  			SSL_EncryptPasser(pstInst->hSSl,pvData,iDataLen);
            break;
        case E_TLS_DATA_FROM_SVR:   
            SSL_DecryptPasser(pstInst->hSSl,pvData,iDataLen);
            break; 
        
        default:
            break;
            
    }
    return 0;
}
WE_INT32 TlsCust_ResumeHandShake
(
    ITLS *pITls,
    WE_INT32 iConnId,
    St_TlsOpt *pstOpt
)
{
    St_TlsHandle *pstTls = NULL;
	St_TlsInstance* pstInst = NULL;	
    St_TlsOpt stOpt[WAP_TLS_OPT_COUNT + 1] = {0};
    St_TlsOpt *pstOptTmp = NULL;
    WE_INT32 iId = 0;
    WE_INT32  iIndex = 0;

    if ((!pstOpt) || (!pITls))
    {
        return -1;
    }
    pstOptTmp = pstOpt;
    pstTls = (St_TlsHandle*)pITls;
    while(1)
    {
        iId = pstOptTmp->eOptId;
        iIndex = _TlsCust_GetOptIndex(iId);
		if (iIndex != -1)
		{
			stOpt[iIndex].eOptId = iId;
			stOpt[iIndex].pvOpt = pstOptTmp->pvOpt;
		}
  
        if (iId == E_TLS_OPT_END)
        {
            break;
        }
        else
        {
            pstOptTmp ++;
        }
    }
    pstInst = pstTls->apstTlsInst[iConnId];
    if (stOpt[WAP_TLS_OPT_ERR_RESP_INDEX].eOptId == E_TLS_OPT_ERR_RESP)
    {
       SSL_UserCnfCert(pstInst->hSSl, *(WE_INT32*)stOpt[WAP_TLS_OPT_ERR_RESP_INDEX].pvOpt);
    }     

    return 0;
  }
WE_INT32 TlsCust_SetPara
(
    ITLS *pITls,
    St_TlsOpt *pstOpt
)
{
 	St_TlsHandle *pstTls = NULL;
    St_TlsOpt stOpt[WAP_TLS_OPT_COUNT + 1] = {0};
    St_TlsOpt *pstOptTmp = NULL;
    WE_INT32 iId = 0;
    WE_INT32  iIndex = 0;
    WE_INT32 iVersion = 0;
    if ((NULL == pstOpt) || (NULL == pITls))
    {
        return -1;
    }
    pstOptTmp = pstOpt;
    pstTls = (St_TlsHandle*)pITls;
    while(1)
    {
        iId = pstOptTmp->eOptId;
        iIndex = _TlsCust_GetOptIndex(iId);
		if (iIndex != -1)
		{
			stOpt[iIndex].eOptId = iId;
			stOpt[iIndex].pvOpt = pstOptTmp->pvOpt;
		}
  
        if (iId == E_TLS_OPT_END)
        {
            break;
        }
        else
        {
            pstOptTmp ++;
        }
    }

    /*user assign the protocol version*/
    if (stOpt[WAP_TLS_OPT_VERSION_INDEX].eOptId == E_TLS_OPT_VERSION)
    {
        iVersion = *(WE_INT32*)stOpt[WAP_TLS_OPT_VERSION_INDEX].pvOpt;
        if (iVersion == G_WAP_TLS_PRO_TLS_VERSION)
        {
            pstTls->ucProtoVer = SSL_PROTOCOLTLS;
        }
        else if (iVersion == G_WAP_TLS_PRO_SSL_VERSION)
        {
            pstTls->ucProtoVer = SSL_PROTOCOLSSL;
        }
        else
        {
            pstTls->ucProtoVer = SSL_PROTOCOLBOTH;
        }
    }
  
    return 0;
}