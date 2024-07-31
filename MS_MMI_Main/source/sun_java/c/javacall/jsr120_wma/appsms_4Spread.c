/*****************************************************************************
 *  模块名     ：Java子系统WMA模块本地适配层
 *  文件名     ：appsms_4Spread.c
 *  文件标识：
 *  相关文件：appsms_4Spread.h
 *  实现功能：实现与展讯平台相关的接口函数（7API ）
 *  作者	      ：
 *  版本	      ：
 *  完成日期：
 *  其它说明：
 *****************************************************************************/

/**************************************************************************
 *                        头文件包含                                      *
 **************************************************************************/
/* 标准库头文件 */


/* 非标准库头文件 */
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
 *                         全局变量声明                                    *
 **************************************************************************/
/*短消息服务状态*/
extern BOOLEAN g_bappsms_smsAvailable;

/*互斥量指针*/
void * g_pvMutex;

/**************************************************************************
 *                     全局函数实现                                      *
 **************************************************************************/
/*****************************************************************************
 *	函数名		：APPSMS_ExistPortNum
 *	功能		       ：设置短消息服务状态为正常，并且创建互斥量
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：
 *	其他说明	：
 *****************************************************************************/
void APPSMS_SmsReady_CallBack(){

    /*如果互斥量创建成功，则置短消息服务为正常*/
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
 *	函数名		：APPSMS_SendSmsNet
 *	功能		       ：发送短消息到网络
 *	输入参数	：sendSms需要发送的短消息
 *	输出参数	：无
 *	返回值说明：
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_SendSmsNet(const T_APPSMS_MsgForList *msg,T_APPSMS_MsgForNet *sendSms){

   MN_SMS_MO_SMS_T tMMISms = {0};
   ERR_MNSMS_CODE_E tResult = ERR_MNSMS_NONE;
   uint8 nOffset=0;

   /*  用于目标号码的BCD编码  */
   uint8 nBcdCode;
   uint8 nAscCode;
   uint8 i;

   /*入参检查*/
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
    if ((0 == msg->destPort)&&(msg->max==1)) /* 无 端口号且非长短消息*/
    {
         tMMISms.user_head_is_exist = 0;/*  无用户数据头*/
    }
    else
    {
   	  tMMISms.user_head_is_exist = 1;/*  有用户数据头*/
    }
	
   tMMISms.reply_path_is_request = 0;              /* 不指定回复状态报告的路径*/
   tMMISms.status_report_is_request = 0;           /*  不需要状态报告*/
   tMMISms.time_format_e = MN_SMS_TP_VPF_RELATIVE_FORMAT;  /*  使用相对时间*/
   tMMISms.time_valid_period.time_second=50000000; /*  短消息有效时间为一天*/

   /*start  为了和展讯保持一致，也许无意义  */
   tMMISms.time_valid_period.time_stamp_t.year=128;
   tMMISms.time_valid_period.time_stamp_t.month=240;
   tMMISms.time_valid_period.time_stamp_t.day=250;
   tMMISms.time_valid_period.time_stamp_t.hour=2;
   tMMISms.time_valid_period.time_stamp_t.minute=0;
   tMMISms.time_valid_period.time_stamp_t.second=0;
   tMMISms.time_valid_period.time_stamp_t.timezone=0;
   tMMISms.tp_mr = 0;
   /*end*/
   tMMISms.pid_present = 1;          /*  指定网络协议*/
   tMMISms.pid_e = MN_SMS_PID_DEFAULT_TYPE;  /* 默认的网络协议*/
   tMMISms.dest_addr_present = 1;  /*  指定目标地址*/

   /*  设置目标地址*/
   tMMISms.dest_addr_t.number_plan = MN_NUM_PLAN_ISDN_TELE; /*  ISDN业务*/
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
   
   /*  编码方式*/
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

    /*用户数据*/
/*    if ((MN_SMS_APP_PORT_16_BIT_ADDR == sendSms->msgBuffer[2])
        && (0 == ((sendSms->msgBuffer[4]<<8)|(sendSms->msgBuffer[5])))) {
            //判断端口号为0的情况处理，去掉
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
 *	函数名		：APPSMS_SendResult_CallBack
 *	功能		  ：处理短消息发送结果
 *	输入参数	：result：短消息发送结果,tp_mr:短消息协议参考码
 *	输出参数	：无
 *	返回值说明：
 *	其他说明	：
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
 *	函数名		：APPSMS_RecvSms_CallBack
 *	功能		       ：接收短消息
 *	输入参数	：recvSms  收到的短消息
 *	输出参数	：无
 *	返回值说明：
 *	其他说明	：
 *****************************************************************************/
void APPSMS_RecvSms_CallBack(const MN_SMS_MT_SMS_T *recvSms){
    uint8 i;	
    T_APPSMS_MsgForNet tNetSms;
	
	/*bcd start*/
    uint8 nBcdCode;
    uint8 nAscCode;
	uint8 nOffSet = 0;
	/*bcd end*/

	/*入参检查*/
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

	/*没有用户头*/
    if(recvSms->user_head_is_exist!=1){
        return;
    }
	
	SCI_MEMSET(&tNetSms,'\0',sizeof(T_APPSMS_MsgForNet));
    
	/*把日期转换成到2000年1月1日0点0分0秒的秒数*/
    tNetSms.timeStamp = APPSMS_ConvertTime2Long(recvSms->time_stamp_t.year, 
		                                        recvSms->time_stamp_t.month, 
    	                                        recvSms->time_stamp_t.day, 
												recvSms->time_stamp_t.hour, 
												recvSms->time_stamp_t.minute,
												recvSms->time_stamp_t.second);
    if(tNetSms.timeStamp == 0)
    {
        SCI_TRACE_LOW("\ntNetSms.timeStamp error");/*接收的短消息中时间异常*/
    }

    if(recvSms->origin_addr_t.number_type==MN_NUM_TYPE_INTERNATIONAL){
        nOffSet = 1;
        tNetSms.address[0] = '+';
    }

    /*源地址的BCD解码*/
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
	/*BCD解码结束*/

	/*转换编码类型*/
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

    /*拷贝用户数据*/
    tNetSms.msgBufferLen = recvSms->user_data_t.length;
        
    SCI_MEMCPY(tNetSms.msgBuffer,recvSms->user_data_t.user_data_arr,tNetSms.msgBufferLen);

    APPSMS_ReceiveRecvSms(&tNetSms);    
}

/*****************************************************************************
 *	函数名		：APPSMS_GetMutex
 *	功能		       ：获取互斥量
 *	输入参数	：mutexPtr  互斥量指针，timeOut等待时间（单位秒）
 *	输出参数	：成功或者失败
 *	返回值说明：
 *	其他说明	：
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
 *	函数名		：APPSMS_PutMutex
 *	功能		       ：释放互斥量
 *	输入参数	：mutexPtr  互斥量指针
 *	输出参数	：成功或者失败
 *	返回值说明：
 *	其他说明	：
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
 *	函数名		：APPSMS_GetCurrentTime
 *	功能		       ：获取当前时间
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：2000 年1 月1 日0 点0 分0 秒到当前的秒数
 *	其他说明	：
 *****************************************************************************/
int32 APPSMS_GetCurrentTime(void)
{
	return TM_GetTotalSeconds();//1970至2000年的秒数946684800
}

/*****************************************************************************
 *	函数名		：APPSMS_IsAppSms
 *	功能		       ：判断是否是JAVA  短消息
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：2000 年1 月1 日0 点0 分0 秒到当前的秒数
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_IsAppSms_CallBack(MN_DUAL_SYS_E dual_sys, APP_SMS_USER_DATA_T *user_data_ptr){
    BOOLEAN is_java_sms = FALSE;
    MN_SMS_USER_HEAD_T user_head_ptr_t = {0};
    int32 des_port = 0,org_port = 0;

    if(NULL == user_data_ptr){
        return FALSE;
    }

    MNSMS_DecodeUserDataHeadEx(dual_sys,&user_data_ptr->user_data_head_t,&user_head_ptr_t);

    // 判断是否为java sms
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


