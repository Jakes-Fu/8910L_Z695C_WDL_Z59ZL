/*****************************************************************************
 *  ģ����  ��Java��ϵͳWMAģ�鱾�������
 *  �ļ���	��appsms_Thread.c
 *  �ļ���ʶ��
 *  ����ļ���appsms_Thread.h
 *  ʵ�ֹ��ܣ���ȫ�ֱ����˿ں�������������Ϣ����
                                ���п��̲߳����ӿڵ�ʵ�֡�
 *  ����		��
 *  �汾		��
 *  ������ڣ�
 *  ����˵����
 *
 *  �޸ļ�¼1��
 *    ��  �ڣ�
 *    �޸ļ�¼��:	
 *    �޸��ˣ�              
 *    �޸����ݣ�
 *  �޸ļ�¼2��
 *****************************************************************************/

/**************************************************************************
 *                        ͷ�ļ�����                                      *
 **************************************************************************/
/* ��׼��ͷ�ļ� */


/* �Ǳ�׼��ͷ�ļ� */

#include "appsms_thread.h"
#include "appsms_4spread.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/
/* �˿ں��б� */
uint16 g_wappsms_portNumList[APPSMS_PORT_LIST_LEN];

/*�����Ͷ���Ϣ�б�*/
T_APPSMS_SendSmsNode g_tappsms_sendSmsList[APPSMS_MAX_SMS_NUM];
int32 nStart=-1;
int32 nEnd=0;
 
/* MN��״̬
*  TRUE����ʾæ
*  FALSE����ʾ��
*  Ĭ��Ϊ�� */
BOOLEAN g_bappsms_netIsBusy=0;

/* ����Ϣ�����״��
* TRUE����ʾ����
* FALSE����ʾ�쳣
* Ĭ��Ϊ�쳣 */
BOOLEAN g_bappsms_smsAvailable=0;

/**************************************************************************
 *                     ȫ�ֺ���ʵ��                                      *
 **************************************************************************/
void APPSMS_InitPortNumList() {
    int i;

    for (i=0; i<APPSMS_PORT_LIST_LEN; i++) {
        g_wappsms_portNumList[i] = 0;
    }
}
/*****************************************************************************
 *	������		��APPSMS_ExistPortNum
 *	����		  �����ָ���Ķ˿ں��Ƿ�����ڶ˿ں��б���
 *	�������	��portNum�˿ں�
 *	�������	����
 *	����ֵ˵����ָ���Ķ˿ں��ڶ˿ں��б����Ƿ����
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_ExistPortNum(uint16 portNum){
    uint8 i;

    for(i=0;i<APPSMS_PORT_LIST_LEN;i++){
        if(g_wappsms_portNumList[i]==portNum){
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************
 *	������		��APPSMS_AddPortNumToList
 *	����		  ����˿ں��б������ж˿ں�
 *	�������	��portNum�˿ں�
 *	�������	����
 *	����ֵ˵�����ɹ�����ʧ��
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_AddPortNumToList(uint16 portNum){
    uint8 i;

    for(i=0;i<APPSMS_PORT_LIST_LEN;i++){
        if(g_wappsms_portNumList[i]==0){
            g_wappsms_portNumList[i]=portNum;
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************
 *	������		��APPSMS_RemovePortNum
 *	����		  ���Ӷ˿ں��б���ɾ��ָ���Ķ˿ں�
 *	�������	��portNum�˿ں�
 *	�������	����
 *	����ֵ˵�����ɹ�����ʧ��
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_RemovePortNum(uint16 portNum){
    uint8 i;

    for(i=0;i<APPSMS_PORT_LIST_LEN;i++){
        if(g_wappsms_portNumList[i]==portNum){
            g_wappsms_portNumList[i]=0;
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************
 *	������		��APPSMS_CheckSmsService
 *	����		  ��������Ϣ�����״̬
 *	�������	����
 *	�������	����
 *	����ֵ˵�������������쳣
 *	����˵��	��TRUE��ʾ������FALSE��ʾ�쳣
 *****************************************************************************/
BOOLEAN APPSMS_CheckSmsService(void)
{   
    return g_bappsms_smsAvailable;
}

/*****************************************************************************
 *	������		��APPSMS_SetSmsService
 *	����		  �����ö���Ϣ�����״̬
 *	�������	��state����Ϣ�����״̬
 *	�������	����
 *	����ֵ˵������
 *	����˵��	��TRUE��ʾ������FALSE��ʾ�쳣
 *****************************************************************************/
void APPSMS_SetSmsService(BOOLEAN state)
{
    g_bappsms_smsAvailable = state;
}

/*****************************************************************************
 *	������		��APPSMS_AddSendSmsMsg
 *	����		       ��������Ͷ���Ϣ���������Ӵ����Ͷ���Ϣ
 *	�������	��sendSms�����͵Ķ���Ϣ;handle������Ϣ�ľ��
 *	�������	����
 *	����ֵ˵�����ɹ�����ʧ��
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_AddSendSmsMsg(const T_APPSMS_MsgForList *sendSms ,int32 handle){

    if(nStart == nEnd){/*��������*/
        return FALSE;
    }else{/*δ��*/
        /*����Ϊ��*/
        if(nStart==-1){
            nStart = nEnd;
        }
        
        g_tappsms_sendSmsList[nEnd].handle = handle;
        SCI_MEMCPY(&(g_tappsms_sendSmsList[nEnd].sendSms),sendSms,sizeof(T_APPSMS_MsgForList));
        nEnd++;

        if(nEnd == APPSMS_MAX_SMS_NUM){
            nEnd = 0;
        }

        return TRUE;
    }
}

/*****************************************************************************
 *	������		��APPSMS_CheckNetState
 *	����		       ����������״̬��æ����
 *	�������	����
 *	�������	����
 *	����ֵ˵����æ������
 *	����˵��	��TRUE���У�FALSE��æ
 *****************************************************************************/
BOOLEAN APPSMS_CheckNetState(void){
    return g_bappsms_netIsBusy;
}


/*****************************************************************************
 *	������		��APPSMS_SetMNState
 *	����		       ������MN��״̬
 *	�������	��æ������
 *	�������	����
 *	����ֵ˵������
 *	����˵��	��TRUE��æ��FALSE����//suozhiling�Ĺ���
 *****************************************************************************/
void APPSMS_SetNetState(BOOLEAN state){
    g_bappsms_netIsBusy = state;
    //javacall_printf("---Thread APPSMS_SetNetState g_bappsms_netIsBusy:%d",g_bappsms_netIsBusy);
}

/*****************************************************************************
 *	������		��APPSMS_GetFirstSendSms
 *	����		       ����ȡ��һ�������Ͷ���Ϣ
 *	�������	����
 *	�������	�������Ͷ���Ϣ
 *	����ֵ˵�����ɹ�����ʧ��
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_GetFirstSendSms(T_APPSMS_MsgForList * *sendSms, int32 *handle){
    /*����Ϊ��*/
    if(nStart==-1){
        return FALSE;
    }else{
        *sendSms = &(g_tappsms_sendSmsList[nStart].sendSms);
        *handle = g_tappsms_sendSmsList[nStart].handle;
        return TRUE;
    }
}


/*****************************************************************************
 *	������		��APPSMS_DelSendSmsByHandle
 *	����		       ��ͨ����Ϣ���ɾ����������Ϣ�����е���Ϣ
 *	�������	������Ϣ���
 *	�������	����
 *	����ֵ˵������
 *	����˵��	��
 *****************************************************************************/
void APPSMS_DelSendSmsByHandle(int32 handle){
     while(nStart != -1 && g_tappsms_sendSmsList[nStart].handle == handle){
         APPSMS_DelFirstSendSms();
     }
}

/*****************************************************************************
 *	������		��APPSMS_DelFirstSendSms
 *	����		       ��ɾ�������Ͷ���Ϣ�����еĵ�һ������Ϣ
 *	�������	����
 *	�������	����
 *	����ֵ˵������
 *	����˵��	��
 *****************************************************************************/
void APPSMS_DelFirstSendSms(){
    if(nStart != -1){
        nStart++;
        if(nStart == APPSMS_MAX_SMS_NUM){
            nStart=0;
        }
        if(nStart == nEnd){
            nStart = -1;
        }
    }
    return;
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


