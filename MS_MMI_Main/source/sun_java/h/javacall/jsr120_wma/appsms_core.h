/*****************************************************************************
 *  ģ����  ��Java��ϵͳWMAģ�鱾�������
 *  �ļ���	��appsms_Core.h
 *  �ļ���ʶ��
 *  ����ļ���appsms_Core.c
 *  ʵ�ֹ��ܣ�����APPSMS������ڲ�ʹ�õ����ݽṹ�ͺ����ӿ�
                               
 *  ����		��
 *  �汾		��
 *  ������ڣ�
 *  ����˵����
 *****************************************************************************/
#include "javacall_defs.h"
#include "sci_types.h"

#ifndef	APPSMS_CORE_H
#define	APPSMS_CORE_H
/**************************************************************************
 *                        ͷ�ļ�����                                      *
 **************************************************************************/
/* ��׼��ͷ�ļ� */


/* �Ǳ�׼��ͷ�ļ� */
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**************************************************************************
 *                         ��������                                           *
 **************************************************************************/
 
/**************************************************************************
 *                         ȫ�ֺ궨��                                         *
 **************************************************************************/

/*���޵Ķ˿���*/
#define  APPSMS_RESTRICT_PORT_NUM   16

/*�˿ں��б����󳤶�*/
#define  APPSMS_PORT_LIST_LEN         20

/*�绰����ĳ���*/
#define  APPSMS_MAX_ADD_LEN         20

/*����Ϣ������֧�ֵ������������*/
#define  APPSMS_MAX_SMS_NUM         10

/*7-bit�������Ͷ���ϢUD����󳤶�*/
#define  APPSMS_7BIT_SMS_LEN           160

/*8-bit�������Ͷ���ϢUD����󳤶�*/
#define  APPSMS_8BIT_SMS_LEN           140

/*��ͨ���ӵ�WAP PUSH����Ϣ�Ķ˿ں�*/
#define  APPSMS_PORT_2948              (uint16)2948

/*��ȫ���ӵ�WAP PUSH����Ϣ�Ķ˿ں�*/
#define  APPSMS_PORT_2949              (uint16)2949


/**************************************************************************
 *                         ȫ���������Ͷ���                                       *
 **************************************************************************/
 /*����Ϣ��������*/
typedef enum
{
    APPSMS_SMSENCODE_7_BIT = 0 ,   /*7-bit*/
    APPSMS_SMSENCODE_8_BIT,        /*8-bit*/
    APPSMS_SMSENCODE_UCS2          /*16-bit*/
} T_APPSMS_Encoding;


/*�������紫��Ķ���Ϣ*/
typedef struct
{
    T_APPSMS_Encoding msgType;             /*��������*/
    uint8   address[APPSMS_MAX_ADD_LEN+1];    /*��ַ����Ϊû������mt��mo��������Դ��ַ��
                                                   Ҳ������Ŀ���ַ��*/
    uint8   msgBuffer[APPSMS_8BIT_SMS_LEN+1];  /*�û�����ͷ+��Ч�غ�*/
    int32   msgBufferLen;                       /*�û�����ͷ+��Ч�غɵĳ���*/
    uint32 timeStamp;                         /*ʱ���������mo����ϢΪ0*/
}T_APPSMS_MsgForNet;

/*����APPSMSʹ�õĶ���Ϣ*/
typedef struct
{
    T_APPSMS_Encoding msgType;                /*��������*/
    uint8  address[APPSMS_MAX_ADD_LEN+1];       /*��ַ����Ϊû������mt��mo��������Դ��ַ��
                                                    Ҳ������Ŀ���ַ��*/
    uint16 sourcePort;                          /*Դ�˿ں�*/
    uint16  destPort;                            /*Ŀ��˿ں�*/
    
    /*����Ϣ���ݣ����Ӻ���۷�ǰ�����ݣ�*/
    uint8   msgBuffer[APPSMS_7BIT_SMS_LEN* APPSMS_MAX_SMS_NUM];  
    int32   msgBufferLen;                         /*����Ϣ���ݵĳ���*/
    uint32 timeStamp;                          /*ʱ���������mo����ϢΪ0*/ 
}T_APPSMS_MsgForJava;

/*������������Ϣ����ʹ�õĶ���Ϣ*/
typedef struct
{
    T_APPSMS_Encoding msgType;                  /*��������*/
    uint8 address[APPSMS_MAX_ADD_LEN+1];         /*��ַ����Ϊû������mt��mo��������Դ��ַ��
                                                      Ҳ������Ŀ���ַ��*/
    uint16 sourcePort;                            /*Դ�˿ں�*/
    uint16 destPort;                              /*Ŀ��˿ں�*/
    uint8   msgBuffer[APPSMS_7BIT_SMS_LEN];        /*��Ч�غ�*/
    int32 msgBufferLen;                           /*��Ч�غɳ���*/
    uint16   ref;                                   /*������Ϣ�Ĳο��룬���ܻ���16-bit*/
    uint8   max;                                   /*������Ϣ�۷ֵĶ���Ϣ����*/
    uint8   seq;                                   /*����Ϣ�����к�*/
    uint32  timeStamp;                            /*ʱ���������mo����ϢΪ0*/
}T_APPSMS_MsgForList;

/*����Ϣ�ķ��ͽ��*/
typedef enum
{
    APPSMS_SMSSEND_SUCCESS = 0,       /*���ͳɹ�*/
    APPSMS_SMSSEND_FAIL,              /*����ʧ��*/
    APPSMS_SMSSEND_FAILENCODE,        /*����Ϣ�������*/
    APPSMS_SMSSEND_UNKNOW,            /*δ֪����*/
    APPSMS_SMSSEND_NETBUSY            /*����æ*/
} T_APPSMS_SendResult;


/*���յĶ���Ϣ�����б�*/
typedef struct
{
    T_APPSMS_MsgForList  recvSms;
    BOOLEAN              useFlag;
}T_APPSMS_RecvSmsNode;



/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
/*ע��˿ں�*/
BOOLEAN APPSMS_AddPortNum(uint16 portNum);

/*���㳤����Ϣ�ķֶ���*/
int32 APPSMS_GetSmsSegments (T_APPSMS_Encoding msgType,int32 msgBufferLen, BOOLEAN hasPort);


/*���Ͷ���Ϣ*/
int32 APPSMS_SendSms(const T_APPSMS_MsgForJava *sendSms);

/*�۷ֶ���Ϣ*/
BOOLEAN APPSMS_MsgSegment(const T_APPSMS_MsgForJava *msg , 
                           uint8 pos ,
						   uint8 max,
						   uint8 ref,
						   T_APPSMS_MsgForList *sendSms,
						   BOOLEAN hasPort);


/*���ɾ��*/
int32 APPSMS_GenHandle(void);

/*���ɳ�����Ϣ�ο���*/
int32 APPSMS_GenReference(void);

/*����Ϣ����*/
BOOLEAN APPSMS_EncodeSms(const T_APPSMS_MsgForList *msg,T_APPSMS_MsgForNet *sendSms);

/*����Ϣ����*/
BOOLEAN APPSMS_DecodeSms(const T_APPSMS_MsgForNet *recvSms, T_APPSMS_MsgForList *msg);

/*�ϱ�����Ϣ���ͽ��*/
void APPSMS_ReceiveSendResult(T_APPSMS_SendResult result);

/*���ն���Ϣ*/
void  APPSMS_ReceiveRecvSms(const T_APPSMS_MsgForNet *recvSms);


/*��鳤����Ϣ�Ƿ�ȫ���������*/
BOOLEAN APPSMS_CheckLongRecvSms(const T_APPSMS_MsgForList *recvSms);

/*��Ӷ���Ϣ��������*/
BOOLEAN APPSMS_AddRecvSmsToList(const T_APPSMS_MsgForList *recvSms);

/*���ⳤ����Ϣ*/
void APPSMS_MsgConcatenation (const T_APPSMS_MsgForList *recvSms,T_APPSMS_MsgForJava *msg);

/*���ݳ�����Ϣ�Ĳο���ɾ�����ն���Ϣ�����еĶ���Ϣ*/
void APPSMS_DelRecvSmsByRef (const T_APPSMS_MsgForList *recvSms);

/*ɾ�����ն���Ϣ����������ǰ�Ķ���Ϣ*/
void APPSMS_DelRecvSmsByDate(void);

/*�������յ����ڸ�ʽת���ɵ�2000 ��1 ��1 �յ�����*/
int32 APPSMS_ConvertTime2Long(uint8 year , uint8 month , uint8 day , uint8 hour , uint8 minute , uint8 second);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /* APPSMS_CORE_H */
