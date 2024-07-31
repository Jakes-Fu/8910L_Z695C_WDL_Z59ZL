/*****************************************************************************
 *  ģ����      ��Java��ϵͳWMAģ�鱾�������
 *  �ļ���      ��appsms_4Spread.h
 *  �ļ���ʶ��
 *  ����ļ���appsms_4Spread.c
 *  ʵ�ֹ��ܣ�������չѶƽ̨��صĽӿں�����7API ��
 *  ����	      ��
 *  �汾	      ��
 *  ������ڣ�
 *  ����˵����
 *****************************************************************************/


#ifndef	APPSMS_4SPREAD_H
#define	APPSMS_4SPREAD_H
/**************************************************************************
 *                        ͷ�ļ�����                                      *
 **************************************************************************/
/* ��׼��ͷ�ļ� */


/* �Ǳ�׼��ͷ�ļ� */
#include "appsms_core.h"
#include "mn_type.h"
#include "mmisms_app.h"
#include "mn_api.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
 /*����MN��APP_MN_SMS_READY_IND��Ϣ*/
void APPSMS_SmsReady_CallBack();

BOOLEAN APPSMS_SendSmsNet(const T_APPSMS_MsgForList *msg,T_APPSMS_MsgForNet *sendSms);

/*�������Ϣ�ķ��ͽ��*/
void APPSMS_SendResult_CallBack(MN_SMS_CAUSE_E result,MN_SMS_TP_MR_T tp_mr);

/*����MT����Ϣ*/
void APPSMS_RecvSms_CallBack(const MN_SMS_MT_SMS_T *recvSms);

/*��ȡ������*/
BOOLEAN  APPSMS_GetMutex(uint32 timeOut);

/*�ͷŻ�����*/
BOOLEAN  APPSMS_PutMutex(void);

/*��ȡ��ǰʱ��*/
int32 APPSMS_GetCurrentTime(void);

/*�ж��Ƿ���JAVA  ����Ϣ*/
BOOLEAN APPSMS_IsAppSms_CallBack(MN_DUAL_SYS_E dual_sys,APP_SMS_USER_DATA_T    *user_data_ptr);

BOOLEAN APPSMS_IsSmsForJava(
                            MN_SMS_USER_HEAD_T         user_head_ptr_t,
                            int32                    *des_port_ptr,
                            int32                    *org_port_ptr
                            );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /* APPSMS_4SPREAD_H */
