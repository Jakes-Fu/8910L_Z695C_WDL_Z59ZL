/*****************************************************************************
 *  ģ����     ��Java��ϵͳWMAģ�鱾�������
 *  �ļ���     ��appsms_4Spread.c
 *  �ļ���ʶ��
 *  ����ļ���appsms_4Spread.h
 *  ʵ�ֹ��ܣ�ʵ����չѶƽ̨��صĽӿں�����7API ��
 *  ����	      ��
 *  �汾	      ��
 *  ������ڣ�
 *  ����˵����
 *****************************************************************************/

/**************************************************************************
 *                        ͷ�ļ�����                                      *
 **************************************************************************/
/* ��׼��ͷ�ļ� */


/* �Ǳ�׼��ͷ�ļ� */
#include "mmi_sun_java_trc.h"
#include "appsms_4spread.h"
#include "os_api.h"
#include "javacall_logging.h"
#include "appsms_thread.h"
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
/*����Ϣ����״̬*/
extern BOOLEAN g_bappsms_smsAvailable;

/*������ָ��*/
void * g_pvMutex;

/**************************************************************************
 *                     ȫ�ֺ���ʵ��                                      *
 **************************************************************************/
/*****************************************************************************
 *	������		��APPSMS_ExistPortNum
 *	����		       �����ö���Ϣ����״̬Ϊ���������Ҵ���������
 *	�������	����
 *	�������	����
 *	����ֵ˵����
 *	����˵��	��
 *****************************************************************************/
void APPSMS_SmsReady_CallBack(){

    /*��������������ɹ������ö���Ϣ����Ϊ����*/
    if(SCI_NULL != g_pvMutex)
    {
        SCI_DeleteMutex((SCI_MUTEX_PTR)g_pvMutex);
    }

    g_pvMutex = (SCI_MUTEX_PTR)SCI_CreateMutex( "myMutex",SCI_INHERIT);

    if( SCI_NULL != g_pvMutex)
    {
        g_bappsms_smsAvailable = TRUE;
    }
}
/*****************************************************************************
 *	������		��APPSMS_SendSmsNet
 *	����		       �����Ͷ���Ϣ������
 *	�������	��sendSms��Ҫ���͵Ķ���Ϣ
 *	�������	����
 *	����ֵ˵����
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_SendSmsNet(const T_APPSMS_MsgForList *msg,T_APPSMS_MsgForNet *sendSms){

   MN_SMS_MO_SMS_T tMMISms = {0};
   ERR_MNSMS_CODE_E tResult = ERR_MNSMS_NONE;
   uint8 nOffset=0;

   /*  ����Ŀ������BCD����  */
   uint8 nBcdCode;
   uint8 nAscCode;
   uint8 i;

   /*��μ��*/
    if(NULL == sendSms){
        return FALSE;
    }
    if(APPSMS_MAX_ADD_LEN < strlen((int8*)(sendSms->address))) {
        return FALSE;
    }
    if(APPSMS_8BIT_SMS_LEN+1 < sendSms->msgBufferLen){
        return FALSE;
    }
   
   SCI_MEMSET(&tMMISms,0,sizeof(MN_SMS_MO_SMS_T));
    if ((0 == msg->destPort)&&(msg->max==1)) /* �� �˿ں��ҷǳ�����Ϣ*/
    {
         tMMISms.user_head_is_exist = 0;/*  ���û�����ͷ*/
    }
    else
    {
   	  tMMISms.user_head_is_exist = 1;/*  ���û�����ͷ*/
    }
	
   tMMISms.reply_path_is_request = 0;              /* ��ָ���ظ�״̬�����·��*/
   tMMISms.status_report_is_request = 0;           /*  ����Ҫ״̬����*/
   tMMISms.time_format_e = MN_SMS_TP_VPF_RELATIVE_FORMAT;  /*  ʹ�����ʱ��*/
   tMMISms.time_valid_period.time_second=50000000; /*  ����Ϣ��Чʱ��Ϊһ��*/

   /*start  Ϊ�˺�չѶ����һ�£�Ҳ��������  */
   tMMISms.time_valid_period.time_stamp_t.year=128;
   tMMISms.time_valid_period.time_stamp_t.month=240;
   tMMISms.time_valid_period.time_stamp_t.day=250;
   tMMISms.time_valid_period.time_stamp_t.hour=2;
   tMMISms.time_valid_period.time_stamp_t.minute=0;
   tMMISms.time_valid_period.time_stamp_t.second=0;
   tMMISms.time_valid_period.time_stamp_t.timezone=0;
   tMMISms.tp_mr = 0;
   /*end*/
   tMMISms.pid_present = 1;          /*  ָ������Э��*/
   tMMISms.pid_e = MN_SMS_PID_DEFAULT_TYPE;  /* Ĭ�ϵ�����Э��*/
   tMMISms.dest_addr_present = 1;  /*  ָ��Ŀ���ַ*/

   /*  ����Ŀ���ַ*/
   tMMISms.dest_addr_t.number_plan = MN_NUM_PLAN_ISDN_TELE; /*  ISDNҵ��*/
   if(sendSms->address[0] == '+'){
       nOffset = 1;
       tMMISms.dest_addr_t.number_type = MN_NUM_TYPE_INTERNATIONAL;
   }else{
       tMMISms.dest_addr_t.number_type = MN_NUM_TYPE_UNKNOW;
   }

   SCI_MEMSET(tMMISms.dest_addr_t.party_num,0,MN_MAX_ADDR_BCD_LEN);

   tMMISms.dest_addr_t.num_len = (strlen((char *)sendSms->address) + 1 ) /2;

   /*START BCD*/
   for(i=0;i < strlen((char *)sendSms->address)-nOffset;i++){
        nAscCode = sendSms->address[i+nOffset];
        /*bcdCode = 0xFF;*/
        if(!((nAscCode >= '0' && nAscCode <= '9')||nAscCode == '*'||
              nAscCode == '#'||(nAscCode == 'P'||nAscCode == 'p')||
              nAscCode == 'W'||nAscCode == 'w'))
            break;
        switch( nAscCode ){
            case '*':
                nBcdCode = 0xA;
                break;
            case '#':
                nBcdCode = 0xB;
                break;
            case 'P':
            case 'p':
                nBcdCode = 0xC;
                break;
            case 'W':
            case 'w':
                nBcdCode = 0xD;
                break;
            default:
                nBcdCode = nAscCode - '0';
                break;
        }
        tMMISms.dest_addr_t.party_num[i/2] = ((i & 1) ? tMMISms.dest_addr_t.party_num[i/2] : 0) +
                    (nBcdCode << ((i & 1) ? 4 : 0));
    }
    if(i&1){
       tMMISms.dest_addr_t.party_num[(i-1)/2] |= 0xf0;
    }
    /*END BCD*/	
   
   /*  ���뷽ʽ*/
   switch(sendSms->msgType){
      case  APPSMS_SMSENCODE_7_BIT:
       	tMMISms.dcs.alphabet_type = MN_SMS_DEFAULT_ALPHABET;
       	break;
       	
      case  APPSMS_SMSENCODE_8_BIT:
       	tMMISms.dcs.alphabet_type = MN_SMS_8_BIT_ALPHBET;
       	break;
       	
      case  APPSMS_SMSENCODE_UCS2:
       	tMMISms.dcs.alphabet_type = MN_SMS_UCS2_ALPHABET;
       	break;
       	
       default:
       	tMMISms.dcs.alphabet_type = MN_SMS_DEFAULT_ALPHABET;
       	break;
    }
    tMMISms.dcs.class_is_present = 0;

    /*�û�����*/
/*    if ((MN_SMS_APP_PORT_16_BIT_ADDR == sendSms->msgBuffer[2])
        && (0 == ((sendSms->msgBuffer[4]<<8)|(sendSms->msgBuffer[5])))) {
            //�ж϶˿ں�Ϊ0���������ȥ��
            tMMISms.user_head_is_exist = FALSE;
            tMMISms.user_data_t.length = sendSms->msgBufferLen - sendSms->msgBuffer[1] - 1;
            tMMISms.user_data_t.user_data_arr[0] = sendSms->msgBuffer[0] - sendSms->msgBuffer[1] - 1;
            SCI_MEMCPY(&tMMISms.user_data_t.user_data_arr[1],&sendSms->msgBuffer[sendSms->msgBuffer[1] + 1+1],tMMISms.user_data_t.length);
    } else {*/
            tMMISms.user_data_t.length = sendSms->msgBufferLen;
            SCI_MEMCPY(tMMISms.user_data_t.user_data_arr,sendSms->msgBuffer,sendSms->msgBufferLen);
//    }

   //tResult = MNSMS_AppSendSms(&tMMISms,MN_SMS_NO_STORED,MN_SMS_GSM_PATH,0);
    tResult = MNSMS_AppSendSmsEx(MN_DUAL_SYS_1,&tMMISms,MN_SMS_NO_STORED,MN_SMS_GSM_PATH,0);

    switch(tResult){
        case ERR_MNSMS_NONE:
            //SCI_TRACE_LOW:"SMS SEND SUCCESS"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,APPSMS_4SPREAD_200_112_2_18_3_26_38_13,(uint8*)"");
	     return TRUE;
        default:
            //SCI_TRACE_LOW:"SMS SEND FAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,APPSMS_4SPREAD_203_112_2_18_3_26_38_14,(uint8*)"");
	    return FALSE;
    }
}

/*****************************************************************************
 *	������		��APPSMS_SendResult_CallBack
 *	����		  ���������Ϣ���ͽ��
 *	�������	��result������Ϣ���ͽ��,tp_mr:����ϢЭ��ο���
 *	�������	����
 *	����ֵ˵����
 *	����˵��	��
 *****************************************************************************/
void APPSMS_SendResult_CallBack(MN_SMS_CAUSE_E result,MN_SMS_TP_MR_T tp_mr){
    T_APPSMS_SendResult  tSmsSendResult;

    //javacall_printf("[appsms_4Spread][APPSMS_SendResult_CallBack]>> sendResult:%d",result);

    switch(result){
        case MN_SMS_OPERATE_SUCCESS:
            tSmsSendResult = APPSMS_SMSSEND_SUCCESS;
            break;
        case MN_SMS_SEND_FAILURE:
            tSmsSendResult = APPSMS_SMSSEND_FAIL;
            break;
        case MN_SMS_MN_BUSY:
            tSmsSendResult = APPSMS_SMSSEND_NETBUSY;
            break;
        default:
            tSmsSendResult = APPSMS_SMSSEND_UNKNOW;
	     break;
    }

    APPSMS_ReceiveSendResult(tSmsSendResult);    
}

/*****************************************************************************
 *	������		��APPSMS_RecvSms_CallBack
 *	����		       �����ն���Ϣ
 *	�������	��recvSms  �յ��Ķ���Ϣ
 *	�������	����
 *	����ֵ˵����
 *	����˵��	��
 *****************************************************************************/
void APPSMS_RecvSms_CallBack(const MN_SMS_MT_SMS_T *recvSms){
    uint8 i;	
    T_APPSMS_MsgForNet tNetSms;
	
	/*bcd start*/
    uint8 nBcdCode;
    uint8 nAscCode;
	uint8 nOffSet = 0;
	/*bcd end*/

	/*��μ��*/
	if(NULL == recvSms){
	    return;
	}
    if(APPSMS_MAX_ADD_LEN < recvSms->origin_addr_t.num_len) {
        return;
    }
    if(APPSMS_8BIT_SMS_LEN+1 < recvSms->user_data_t.length){
        return;
    }

   //javacall_printf ("[appsms_4Spread][APPSMS_RecvSms_CallBack]>> APPSMS_RecvSms_CallBack recvSms->user_data_t.length:%d",recvSms->user_data_t.length);

	/*û���û�ͷ*/
    if(recvSms->user_head_is_exist!=1){
        return;
    }
	
	SCI_MEMSET(&tNetSms,'\0',sizeof(T_APPSMS_MsgForNet));
    
	/*������ת���ɵ�2000��1��1��0��0��0�������*/
    tNetSms.timeStamp = APPSMS_ConvertTime2Long(recvSms->time_stamp_t.year, 
		                                        recvSms->time_stamp_t.month, 
    	                                        recvSms->time_stamp_t.day, 
												recvSms->time_stamp_t.hour, 
												recvSms->time_stamp_t.minute,
												recvSms->time_stamp_t.second);
    if(tNetSms.timeStamp == 0)
    {
        SCI_TRACE_LOW("\ntNetSms.timeStamp error");/*���յĶ���Ϣ��ʱ���쳣*/
    }

    if(recvSms->origin_addr_t.number_type==MN_NUM_TYPE_INTERNATIONAL){
        nOffSet = 1;
        tNetSms.address[0] = '+';
    }

    /*Դ��ַ��BCD����*/
    for(i = 0;i < recvSms->origin_addr_t.num_len*2; i++){
        nBcdCode = (recvSms->origin_addr_t.party_num[i/2] >> (((i+1) & 1) ? 0 : 4)) & 0x0F;
        if(nBcdCode == 0xF){
            break;
        }
        nAscCode = (nBcdCode == 0xA) ? '*':
                  (nBcdCode == 0xB) ? '#':
                  (nBcdCode == 0xC)? 'P':
                  (nBcdCode == 0xD) ? 'w': (nBcdCode + '0');


        if(!((nAscCode >= '0' && nAscCode <= '9')||nAscCode == '*'||
              nAscCode == '#'||(nAscCode == 'P'||nAscCode == 'p')||
              nAscCode == 'W'||nAscCode == 'w'))
           nAscCode = 0;

        tNetSms.address[i+nOffSet] = nAscCode;
    }
    tNetSms.address[i+nOffSet] = 0;
	/*BCD�������*/

	/*ת����������*/
    switch(recvSms->dcs.alphabet_type){
        case MN_SMS_DEFAULT_ALPHABET:
            tNetSms.msgType = APPSMS_SMSENCODE_7_BIT;
            break;
        case MN_SMS_UCS2_ALPHABET:
            tNetSms.msgType = APPSMS_SMSENCODE_UCS2;
            break;
        default:
            tNetSms.msgType = APPSMS_SMSENCODE_8_BIT;
            break;
    }

    /*�����û�����*/
    tNetSms.msgBufferLen = recvSms->user_data_t.length;
        
    SCI_MEMCPY(tNetSms.msgBuffer,recvSms->user_data_t.user_data_arr,tNetSms.msgBufferLen);

    APPSMS_ReceiveRecvSms(&tNetSms);    
}

/*****************************************************************************
 *	������		��APPSMS_GetMutex
 *	����		       ����ȡ������
 *	�������	��mutexPtr  ������ָ�룬timeOut�ȴ�ʱ�䣨��λ�룩
 *	�������	���ɹ�����ʧ��
 *	����ֵ˵����
 *	����˵��	��
 *****************************************************************************/
BOOLEAN  APPSMS_GetMutex(uint32 timeOut){
    uint32 uiResult = SCI_GetMutex(g_pvMutex , timeOut);
    javacall_printf("[appsms_4Spread][APPSMS_GetMutex]>> WMA  SCI_GetMutex:%d",uiResult);
    if(uiResult == SCI_SUCCESS){
        return TRUE;
    }else{
        return FALSE;
    }
}

/*****************************************************************************
 *	������		��APPSMS_PutMutex
 *	����		       ���ͷŻ�����
 *	�������	��mutexPtr  ������ָ��
 *	�������	���ɹ�����ʧ��
 *	����ֵ˵����
 *	����˵��	��
 *****************************************************************************/
BOOLEAN  APPSMS_PutMutex(){
    uint32 uiResult = SCI_PutMutex(g_pvMutex);
    javacall_printf("[appsms_4Spread][APPSMS_GetMutex]>> SCI_PutMutex:%d",uiResult);
    if(uiResult == SCI_SUCCESS){
        return TRUE;
    }else{
        return FALSE;
    }
}

/*****************************************************************************
 *	������		��APPSMS_GetCurrentTime
 *	����		       ����ȡ��ǰʱ��
 *	�������	����
 *	�������	����
 *	����ֵ˵����2000 ��1 ��1 ��0 ��0 ��0 �뵽��ǰ������
 *	����˵��	��
 *****************************************************************************/
int32 APPSMS_GetCurrentTime(void)
{
	return TM_GetTotalSeconds();//1970��2000�������946684800
}

/*****************************************************************************
 *	������		��APPSMS_IsAppSms
 *	����		       ���ж��Ƿ���JAVA  ����Ϣ
 *	�������	����
 *	�������	����
 *	����ֵ˵����2000 ��1 ��1 ��0 ��0 ��0 �뵽��ǰ������
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_IsAppSms_CallBack(MN_DUAL_SYS_E dual_sys, APP_SMS_USER_DATA_T *user_data_ptr){
    BOOLEAN is_java_sms = FALSE;
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
    int32 des_port = 0,org_port = 0;

    if(NULL == user_data_ptr){
        return FALSE;
    }

    MNSMS_DecodeUserDataHeadEx(dual_sys,&user_data_ptr->user_data_head_t,&user_head_ptr_t);

    // �ж��Ƿ�Ϊjava sms
    is_java_sms = APPSMS_IsSmsForJava(user_head_ptr_t,&des_port,&org_port);

    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &user_head_ptr_t);

    return is_java_sms;
}

BOOLEAN APPSMS_IsSmsForJava(
                            MN_SMS_USER_HEAD_T         user_head_ptr_t,
                            int32                    *des_port_ptr,
                            int32                    *org_port_ptr
                            ){
    uint8 iei_num = 0;
    uint8 i = 0;
    int32 index= -1;
    int32 des_port_num = 0,org_port_num = 0;
    BOOLEAN is_for_java = FALSE;

    iei_num = user_head_ptr_t.iei_num;
    for (i = 0; i<iei_num;i++){
        javacall_printf(LOG_STR("i value is = %d  .iei=%d"),i,user_head_ptr_t.iei_arr[i].iei);
        if((user_head_ptr_t.iei_arr[i].iei == MMISMS_HEAD_IEI_FOR_PORT)
            ||(user_head_ptr_t.iei_arr[i].iei == MMISMS_HEAD_IEL_FOR_PORT)){
            index = i;
            break;
        }else{
            index = -1;
        }
    }

    if (-1 == index){
        javacall_printf(LOG_STR("There is no java port number!"));
        is_for_java = FALSE;
    }else{
        if (user_head_ptr_t.iei_arr[index].iei == MN_SMS_APP_PORT_8_BIT_ADDR){
            des_port_num = user_head_ptr_t.iei_arr[index].iei_data_t[0];
            org_port_num = user_head_ptr_t.iei_arr[index].iei_data_t[1];
        }else if (user_head_ptr_t.iei_arr[index].iei == MN_SMS_APP_PORT_16_BIT_ADDR){
            des_port_num = (user_head_ptr_t.iei_arr[index].iei_data_t[0] << 8) | (user_head_ptr_t.iei_arr[index].iei_data_t[1]);
            org_port_num = (user_head_ptr_t.iei_arr[index].iei_data_t[2] << 8) |(user_head_ptr_t.iei_arr[index].iei_data_t[3]);
        }

        *des_port_ptr = des_port_num;
        *org_port_ptr = org_port_num;
        is_for_java =  APPSMS_ExistPortNum(des_port_num);
    }
    return is_for_java;
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


