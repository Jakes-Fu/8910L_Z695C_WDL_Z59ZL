#include "tls_def.h"
#include "itls.h"

typedef struct _ITLS ITLS; 
WE_INT32 TlsCust_Init(void** ppHandle);
WE_INT32 TlsCust_Release(ITLS *pITls);
WE_INT32 TlsCust_Close(ITLS* pvTls,WE_INT32 iId);
WE_INT32 TlsCust_StartHandShake
(
    ITLS *pvTls,
    WE_INT32 iConnId,
    WE_UCHAR* pucHost,
    WE_BOOL bUseProxy,
    WE_UCHAR *pucAddr,
    WE_UINT16 usPort
);
WE_INT32 TlsCust_ViewSvrCert
(
    ITLS *pITls,
    WE_CHAR* pcHost,
    WE_VOID **ppvContent
);
WE_INT32 TlsCust_RegRespCB
(
    ITLS *pITls,
    Fn_ITlsRespCb pcb,
    WE_HANDLE hPrivData
);
WE_INT32 TlsCust_ProcData
(
    ITLS *pITls,
    WE_INT32 iConnId,
    E_TLS_DATA_TYPE eDataType,
    WE_VOID* pvData,
    WE_INT32 iDataLen
);
WE_INT32 TlsCust_ResumeHandShake
(
    ITLS *pITls,
    WE_INT32 iConnId,
    St_TlsOpt *pstOpt
);
WE_INT32 TlsCust_SetPara
(
    ITLS *pITls,
    St_TlsOpt *pstOpt
);
#if 0
typedef enum
{
	SSL_FAIL =-1,
	SSL_OK = 0,
	SSL_CERT_UNVERIFY= 1,
}SSL_RESULT_E;
typedef enum
{
	SSL_SEND_SUCC = 0x01,
	SSL_RECV_SUCC = 0x03,
}SSL_MESSAGE_E;

#define SSL_ALGORITHM_MAX_SIZE     10
/*SSL 模块初始化函数，创建全局变量保存SSL一些参数，在系统开启时调用一次*/
void SSL_Init(void);
/*保存SSL信息的全局变量，在系统结束时调用一次*/
void SSL_Destroy(void);
/*创建SSL的状态机，并返回状态机指针
  request: 需要使用SSL状态机的应用模块
  fd:socket id
  is_sync: 创建同步或者异步句柄
 */
SSL_HANDLER SSL_Create(void* request,long fd,bool is_sync);
/*销毁SSL句柄， 如果为同步接口， socket 也在这个函数中销毁，异步接口时， socket 由上层协议销毁*/
void SSL_Close(SSL_HANDLER ssl,bool is_sync);

void SSL_ProtocolChoose(SSL_HANDLER ssl,SSL_PROTOVER_E  pro,bool is_sync);

SSL_RESULT_E  SSL_HandShake(SSL_HANDLER ssl,uint8* host,uint16 port,bool is_sync);

SSL_RESULT_E  SSL_GetCertInfo(SSL_HANDLER ssl,SSL_CERTINFO_T* cert,bool is_sync);

SSL_RESULT_E  SSL_FreeCurrentCertInfo(SSL_CERTINFO_T* cert);

/*当加密的数据通过Callback  返回应用协议发送后，当发送成功，上层协议需要发送SSL_SEND_SUCC 给SSL以释放buffer*/
void SSL_EncryptPasser(SSL_HANDLER ssl,uint8* buf,uint32 len);
/*当数据解密好通过RecvDataCallback 返回后，应用协议将数据备份好并发送SSL_RECV_SUCC 通知SSL释放解密数据的空间*/
void SSL_DecryptPasser(SSL_HANDLER ssl,uint8* buf,uint32 len);

/*注册SSL的Callback，*/
void SSL_AsyncRegMessageCb(SSL_HANDLER ssl,SecurityPostMessage cb);

/*注册SSL的Callback用于返回解密好的数据
  当应用协议收到解密好的数据时，将数据备份，并通知SSL释放此数据*/
void SSL_AsyncRegRecvDataCb(SSL_HANDLER ssl,SecurityDecryptDataOutput cb);

/*注册SSL的Callback用于返回加密好的数据
  当应用协议收到加密好的数据时，将数据备份，并通知SSL释放此数据*/
void SSL_AsyncRegSendDataCb(SSL_HANDLER ssl,SecurityEncryptDataOutput cb);

void SSL_AsyncRegShowCertCb(SSL_HANDLER ssl,SecurityShowCertInfo cb);

void SSL_AsyncUserConfirm(SSL_HANDLER ssl,bool flag);


void SSL_AsyncMessageProc(SSL_HANDLER ssl,SSL_MESSAGE_E message,uint32 length);
#endif