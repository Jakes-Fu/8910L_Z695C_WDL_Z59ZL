/*****************************************************************************
 *  ģ����  ��Java��ϵͳWMAģ�鱾�������
 *  �ļ���	��appsms_Thread.h
 *  �ļ���ʶ��
 *  ����ļ���appsms_Thread.c
 *  ʵ�ֹ��ܣ�����ȫ�ֱ����˿ں�������������Ϣ����
                                ���Ӳ�ͬ���̶߳����ǽ��в�����
 *  ����		��
 *  �汾		��
 *  ������ڣ�
 *  ����˵����
 *****************************************************************************/


#ifndef	APPSMS_THREAD_H
#define	APPSMS_THREAD_H
/**************************************************************************
 *                        ͷ�ļ�����                                      *
 **************************************************************************/
/* ��׼��ͷ�ļ� */


/* �Ǳ�׼��ͷ�ļ� */

#include "appsms_core.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**************************************************************************
 *                         ȫ���������Ͷ���                                       *
 **************************************************************************/
/*�����Ͷ���Ϣ�б��ж���Ϣ�ڵ�*/
typedef struct
{
    T_APPSMS_MsgForList sendSms;           /*�����͵Ķ���Ϣ*/
    int32              handle;            /*����Ϣ���*/
    BOOLEAN             useFlag;           /*�Ժ���չʹ��*/
}T_APPSMS_SendSmsNode; 

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
/*���ָ���Ķ˿ں��Ƿ�����ڶ˿ں�������*/
BOOLEAN APPSMS_ExistPortNum(uint16 portNum);

 /*��˿ں�����������µĶ˿ں�*/
BOOLEAN APPSMS_AddPortNumToList(uint16 portNum);

/*�Ӷ˿ں�������ɾ��ָ���Ķ˿ں�*/
BOOLEAN APPSMS_RemovePortNum(uint16 portNum);

/*������Ϣ�����״̬*/
BOOLEAN APPSMS_CheckSmsService(void);

/*���ö���Ϣ�����״̬*/
void APPSMS_SetSmsService(BOOLEAN state);

 /*������Ͷ���Ϣ���������Ӵ����Ͷ���Ϣ*/
BOOLEAN APPSMS_AddSendSmsMsg(const T_APPSMS_MsgForList *sendSms ,int32 handle);

/*��������״̬��æ����*/
BOOLEAN APPSMS_CheckNetState(void);

/*���������״̬*/
void APPSMS_SetNetState(BOOLEAN state);

/*��ȡ��һ�������Ͷ���Ϣ*/
BOOLEAN APPSMS_GetFirstSendSms(T_APPSMS_MsgForList * *sendSms, int32 *handle);

/*ͨ����Ϣ���ɾ����������Ϣ�����е���Ϣ*/
void APPSMS_DelSendSmsByHandle(int32 handle);

/*ɾ�������Ͷ���Ϣ�����еĵ�һ������Ϣ*/
void APPSMS_DelFirstSendSms(void);

void APPSMS_InitPortNumList(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /* APPSMS_THREAD_H */

