/*****************************************************************************
 *  模块名  ：Java子系统WMA模块本地适配层
 *  文件名	：javacall_sms.c
 *  文件标识 ：
 *  相关文件 ：javacall_sms.h
 *  实现功能 ：实现与Java  组件相关的接口函数（8API ）
 *  作者		：
 *  版本		：
 *  完成日期 ：
 *  其它说明 ：
 *****************************************************************************/

/**************************************************************************
 *                        头文件包含                                      *
 **************************************************************************/
/* 标准库头文件 */

#include <stdio.h>

/* 非标准库头文件 */
#include "javacall_sms.h"
#include "appsms_core.h"
#include "appsms_thread.h"
#include "appsms_4java.h"
#include "mmisms_export.h"
#include "os_api.h"
#include "javacall_logging.h"
#include "mmijava_main.h"
#include "javacall_memory.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

extern void javanotify_get_push_entry(char *);

static T_APPSMS_MsgForJava * s_external_pending_sms=PNULL;

extern BOOLEAN g_bappsms_smsAvailable;
#ifdef WIN32
extern const __int64 deltaTime;
#else
extern const long long deltaTime;
#endif

/**
 * check if the SMS service is available, and SMS messages can be sent and received
 *
 * @return <tt>JAVACALL_OK</tt> if SMS service is avaialble 
 *         <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
javacall_result javacall_sms_is_service_available(void) {
    BOOLEAN bServiceStat = APPSMS_CheckSmsService();

    if(bServiceStat ==FALSE){
        return JAVACALL_FAIL;
    }else{
        return JAVACALL_OK;
    }
}

/**
 * send an SMS message
 *
 * @param msgType message string type: Text or Binary.
 *                The target device should decide the DCS (Data Coding Scheme)  
 *                in the PDU according to this parameter and the  message contents.   
 *                If the target device is compliant with GSM 3.40, then for a Binary 
 *                Message,  the DCS in PDU should be 8-bit binary. 
 *                For a  Text Message, the target device should decide the DCS  according to  
 *                the  message contents. When all characters in the message contents are in 
 *                the GSM 7-bit alphabet, the DCS should be GSM 7-bit; otherwise, it should  
 *                be  UCS-2.
 * @param destAddress the target SMS address for the message.  The format of the address  parameter  
 *                is  expected to be compliant with MSIDN, for example,. +123456789 
 * @param msgBuffer the message body (payload) to be sent
 * @param msgBufferLen the message body (payload) len
 * @param sourcePort source port of SMS message
 * @param destPort destination port of SMS message where 0 is default destination port 
 * @return handle of sent sms or <tt>0</tt> if unsuccessful
 * 
 * Note: javacall_callback_on_complete_sms_send() needs to be called to notify
 *       completion of sending operation.
 *       The returned handle will be passed to javacall_callback_on_complete_sms_send( ) upon completion
 */
javacall_result javacall_sms_send(  javacall_sms_encoding   msgType, 
                        const unsigned char*    destAddress, 
                        const unsigned char*    msgBuffer, 
                        int                     msgBufferLen, 
                        unsigned short          sourcePort, 
                        unsigned short          destPort,
                        int*                     nativeHandle){

    T_APPSMS_MsgForJava javaSms;


    if(g_bappsms_smsAvailable == 0){
        return JAVACALL_FAIL;
    }

    /*入参检查*/
    if(NULL == destAddress){
        return JAVACALL_FAIL;
    }
    if(NULL == msgBuffer){
        return JAVACALL_FAIL;
    }
    if(APPSMS_7BIT_SMS_LEN* APPSMS_MAX_SMS_NUM < msgBufferLen){
        return JAVACALL_FAIL;
    }

    switch(msgType){
        case JAVACALL_SMS_MSG_TYPE_ASCII:
            javaSms.msgType = APPSMS_SMSENCODE_7_BIT;
            break;
        case JAVACALL_SMS_MSG_TYPE_UNICODE_UCS2:
            javaSms.msgType = APPSMS_SMSENCODE_UCS2;
            break;
        case JAVACALL_SMS_MSG_TYPE_BINARY:
        default:
            javaSms.msgType = APPSMS_SMSENCODE_8_BIT;
	     break;
    }

    //SCI_MEMCPY(javaSms.address,destAddress,APPSMS_MAX_ADD_LEN);
    strcpy((char*)javaSms.address,(char*)destAddress);
    SCI_MEMCPY(javaSms.msgBuffer,msgBuffer,msgBufferLen);
    javaSms.msgBufferLen = msgBufferLen;
    javaSms.sourcePort = sourcePort;
    javaSms.destPort = destPort;

    //javacall_printf("[javacall_sms][javacall_sms_send]>>&javaSms=%d &javaSms msgBuffer=%d",&javaSms,&(javaSms.msgBuffer));
    *nativeHandle = APPSMS_SendSms(&javaSms);
    if(*nativeHandle == 0){
        return JAVACALL_FAIL;
    }else{
        return JAVACALL_WOULD_BLOCK;
    }
}
    
    
/**
 * The platform must have the ability to identify the port number of incoming 
 * SMS messages, and deliver messages with port numbers registered to the WMA 
 * implementation.
 * If this port number has already been registered either by a native application 
 * or by another WMA application, then the API should return an error code.
 * 
 * @param portNum port to start listening to
 * @return <tt>JAVACALL_OK</tt> if started listening to port, or 
 *         <tt>JAVACALL_FAIL</tt> or negative value if unsuccessful
 */
javacall_result javacall_sms_add_listening_port(unsigned short portNum){
    BOOLEAN bResult = APPSMS_AddPortNum(portNum);

    javacall_printf(LOG_STR("add listening port=%d result=%d"),portNum,bResult);
	
    if(bResult == TRUE){
        return JAVACALL_OK;
    }else{
        return JAVACALL_FAIL;
    }
}
    
/**
 * unregisters a message port number. 
 * After unregistering a port number, SMS messages received by the device for 
 * the specfied port should not be delivered tothe WMA implementation.  
 * If this API specifies a port number which is not registered, then it should 
 * return an error code.
 *
 * @param portNum port to stop listening to
 * @return <tt>JAVACALL_OK </tt> if stopped listening to port, 
 *          or <tt>0</tt> if failed, or port not registered
 */
javacall_result javacall_sms_remove_listening_port(unsigned short portNum){
    BOOLEAN bResult = APPSMS_RemovePortNum(portNum);

    javacall_printf(LOG_STR("remove listening port=%d result=%d"),portNum,bResult);
	
    if(bResult ==TRUE){
        return JAVACALL_OK;
    }else{
        return JAVACALL_FAIL;
    }
}
    
    
/**
 * returns the number of segments (individual SMS messages) that would 
 * be needed in the underlying protocol to send a specified message. 
 *
 * The specified message is included as a parameter to this API.
 * Note that this method does not actually send the message. 
 * It will only calculate the number of protocol segments needed for sending 
 * the message. This API returns a count of the message segments that would be 
 * sent for the provided Message.
 *
 * @param msgType message string type: Text or Binary.
 *                The target device should decide the DCS (Data Coding Scheme)  
 *                in the PDU according to this parameter and the  message contents.   
 *                If the target device is compliant with GSM 3.40, then for a Binary 
 *                Message,  the DCS in PDU should be 8-bit binary. 
 *                For a  Text Message, the target device should decide the DCS  according to  
 *                the  message contents. When all characters in the message contents are in 
 *                the GSM 7-bit alphabet, the DCS should be GSM 7-bit; otherwise, it should  
 *                be  UCS-2.
 * @param msgBuffer the message body (payload) to be sent
 * @param msgBufferLen the message body (payload) len
 * @param hasPort indicates if the message includes source or destination port number 
 * @return number of segments, or 0 value on error
 */
int javacall_sms_get_number_of_segments(
        javacall_sms_encoding   msgType, 
        char*                   msgBuffer, 
        int                     msgBufferLen, 
        javacall_bool           hasPort){
    T_APPSMS_Encoding tSmsType;
    BOOLEAN bHasPort;

    switch(msgType){
        case JAVACALL_SMS_MSG_TYPE_ASCII:
            tSmsType = APPSMS_SMSENCODE_7_BIT;
            break;
        case JAVACALL_SMS_MSG_TYPE_UNICODE_UCS2:
            tSmsType = APPSMS_SMSENCODE_UCS2;
            break;
        case JAVACALL_SMS_MSG_TYPE_BINARY:
        default:
            tSmsType = APPSMS_SMSENCODE_8_BIT;
    }

    if(hasPort == JAVACALL_FALSE){
        bHasPort = FALSE;
    }else{
        bHasPort = TRUE;
    }

    return APPSMS_GetSmsSegments(tSmsType, msgBufferLen, bHasPort);
}

void javacall_sms_init() {
    APPSMS_InitPortNumList();
}

/*****************************************************************************
 *	函数名		：APPSMS_SendResultToVM
 *	功能		       ：上报短消息发送结果给虚拟机
 *	输入参数	：result  短消息发送结果，handle  短消息的句柄
 *	输出参数	：无
 *	返回值说明：
 *	其他说明	：
 *****************************************************************************/
void APPSMS_SendResultToVM(T_APPSMS_SendResult result ,int32 handle){
    if(result == APPSMS_SMSSEND_SUCCESS){
        javanotify_sms_send_completed  ( JAVACALL_OK, handle );
    }else{
        javanotify_sms_send_completed  ( JAVACALL_FAIL, handle );
    }
}

/*****************************************************************************
 *	函数名		：APPSMS_SendMtToVM
 *	功能		       ：上报mt短消息给虚拟机
 *	输入参数	：recvSms  接收到的短消息
 *	输出参数	：无
 *	返回值说明：
 *	其他说明	：
 *****************************************************************************/
void APPSMS_SendMtToVM(T_APPSMS_MsgForJava *recvSms){
    javacall_sms_encoding t_smsType;
    static char smsentry[24];
	
    switch(recvSms->msgType){
        case APPSMS_SMSENCODE_7_BIT:
            t_smsType = JAVACALL_SMS_MSG_TYPE_ASCII;
            break;
        case APPSMS_SMSENCODE_UCS2:
            t_smsType = JAVACALL_SMS_MSG_TYPE_UNICODE_UCS2;
            break;
        case APPSMS_SMSENCODE_8_BIT:
        default:
            t_smsType = JAVACALL_SMS_MSG_TYPE_BINARY;
            break;
    }

    javacall_printf(LOG_STR("recv new sms sourcePort=%d destPort=%d t_smsType=%d"),recvSms->sourcePort,recvSms->destPort,t_smsType);
    if (MMIJAVA_IsJavaShutdown() == TRUE) {
        if (s_external_pending_sms == NULL) {
            s_external_pending_sms = (T_APPSMS_MsgForJava*)javacall_malloc(sizeof(T_APPSMS_MsgForJava));
            if (s_external_pending_sms == NULL) {
                javacall_printf(LOG_STR_LEAVE("fail, no enough memory"));
                return;
            }
        }
        SCI_MEMCPY(s_external_pending_sms, recvSms, sizeof(T_APPSMS_MsgForJava));
        sprintf(smsentry, "sms://:%d", recvSms->destPort);
        javanotify_get_push_entry(smsentry);
    } else {
	/*向虚拟机发送消息*/
    javanotify_incoming_sms(t_smsType, (char *)recvSms->address, recvSms->msgBuffer , 
    	      recvSms->msgBufferLen , recvSms->sourcePort , recvSms->destPort , recvSms->timeStamp+deltaTime);
}
}

/**
 * Clean up pending sms used by the external push.
 */
void javacall_sms_cleanup_pending_sms(void)
{
    if (s_external_pending_sms != NULL) {
        javacall_free(s_external_pending_sms);
        s_external_pending_sms=NULL;
    }
}

/**
 * Notify the JVM of pending external push sms.
 */
javacall_result javacall_sms_notify_pending_sms(void) {
    javacall_printf(LOG_STR_ENTER(""));
    if (s_external_pending_sms != PNULL) {
        APPSMS_SendMtToVM(s_external_pending_sms);
        javacall_sms_cleanup_pending_sms();
    return JAVACALL_OK;
    }
    return JAVACALL_FAIL;
}
#ifdef __cplusplus
}
#endif


