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
/*SSL ģ���ʼ������������ȫ�ֱ�������SSLһЩ��������ϵͳ����ʱ����һ��*/
void SSL_Init(void);
/*����SSL��Ϣ��ȫ�ֱ�������ϵͳ����ʱ����һ��*/
void SSL_Destroy(void);
/*����SSL��״̬����������״̬��ָ��
  request: ��Ҫʹ��SSL״̬����Ӧ��ģ��
  fd:socket id
  is_sync: ����ͬ�������첽���
 */
SSL_HANDLER SSL_Create(void* request,long fd,bool is_sync);
/*����SSL����� ���Ϊͬ���ӿڣ� socket Ҳ��������������٣��첽�ӿ�ʱ�� socket ���ϲ�Э������*/
void SSL_Close(SSL_HANDLER ssl,bool is_sync);

void SSL_ProtocolChoose(SSL_HANDLER ssl,SSL_PROTOVER_E  pro,bool is_sync);

SSL_RESULT_E  SSL_HandShake(SSL_HANDLER ssl,uint8* host,uint16 port,bool is_sync);

SSL_RESULT_E  SSL_GetCertInfo(SSL_HANDLER ssl,SSL_CERTINFO_T* cert,bool is_sync);

SSL_RESULT_E  SSL_FreeCurrentCertInfo(SSL_CERTINFO_T* cert);

/*�����ܵ�����ͨ��Callback  ����Ӧ��Э�鷢�ͺ󣬵����ͳɹ����ϲ�Э����Ҫ����SSL_SEND_SUCC ��SSL���ͷ�buffer*/
void SSL_EncryptPasser(SSL_HANDLER ssl,uint8* buf,uint32 len);
/*�����ݽ��ܺ�ͨ��RecvDataCallback ���غ�Ӧ��Э�齫���ݱ��ݺò�����SSL_RECV_SUCC ֪ͨSSL�ͷŽ������ݵĿռ�*/
void SSL_DecryptPasser(SSL_HANDLER ssl,uint8* buf,uint32 len);

/*ע��SSL��Callback��*/
void SSL_AsyncRegMessageCb(SSL_HANDLER ssl,SecurityPostMessage cb);

/*ע��SSL��Callback�y���ڷ��ؽ��ܺõ�����
  ��Ӧ��Э���յ����ܺõ�����ʱ�������ݱ��ݣ���֪ͨSSL�ͷŴ�����*/
void SSL_AsyncRegRecvDataCb(SSL_HANDLER ssl,SecurityDecryptDataOutput cb);

/*ע��SSL��Callback�y���ڷ��ؼ��ܺõ�����
  ��Ӧ��Э���յ����ܺõ�����ʱ�������ݱ��ݣ���֪ͨSSL�ͷŴ�����*/
void SSL_AsyncRegSendDataCb(SSL_HANDLER ssl,SecurityEncryptDataOutput cb);

void SSL_AsyncRegShowCertCb(SSL_HANDLER ssl,SecurityShowCertInfo cb);

void SSL_AsyncUserConfirm(SSL_HANDLER ssl,bool flag);


void SSL_AsyncMessageProc(SSL_HANDLER ssl,SSL_MESSAGE_E message,uint32 length);
#endif