/*****************************************************************************
 *  模块名  ：Java子系统WMA模块本地适配层
 *  文件名	：appsms_Core.c
 *  文件标识：
 *  相关文件：appsms_Core.h
 *  实现功能：定义APPSMS适配层内部使用的数据结构和函数接口
 *  作者		：
 *  版本		：
 *  完成日期：
 *  其它说明：
 *
 *  修改记录1：
 *    日  期：
 *    修改记录号:	
 *    修改人：              
 *    修改内容：
 *  修改记录2：
 *****************************************************************************/

/**************************************************************************
 *                        头文件包含                                      *
 **************************************************************************/
/* 标准库头文件 */


/* 非标准库头文件 */
#include "appsms_core.h"
#include "appsms_thread.h"
#include "appsms_4spread.h"
#include "appsms_4java.h"
#include "os_api.h"
#include "javacall_logging.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*PUSH串接工作数据区*/
//extern T_PUSH_CONCATENATE_CONTEXT	g_tPushConcatenateContext;

/**************************************************************************
 *                         全局变量声明                                    *
 **************************************************************************/
 /*限制使用的端口号*/
 uint16 g_wRestrictPort[APPSMS_RESTRICT_PORT_NUM]={2805,2923,2948,2949,5502,5503,
                                           5508,5511,5512,9200,9201,9202,9203,9207,49996,49999};



/*接收短消息的缓存队列*/
T_APPSMS_RecvSmsNode g_tappsms_recvSmsList[APPSMS_MAX_SMS_NUM];

/*短消息句柄计数器*/
int32 g_nHandleNum = 1;

/*长短消息参考码计数器*/
int32 g_nReferenceNum = 0;

/*最近短消息发送时间*/
int32 g_NewSendTime = 0;
/**************************************************************************
 *                     全局函数定义                                      *
 **************************************************************************/
/*设置存贮区中的某个7-bit 数据*/
static BOOLEAN APPSMS_Set7BitBuff(uint8* buff , int32 buffLen , int32 pos , int8 data7Bit);

/*读取存贮区中的某个7-bit 数据*/
static BOOLEAN APPSMS_Get7BitBuff(const uint8* buff , int32 buffLen , int32 pos , int8* data7Bit);
/**************************************************************************
 *                     全局函数实现                                      *
 **************************************************************************/

/*****************************************************************************
 *	函数名		：APPSMS_AddPortNum
 *	功能		       ：注册端口号
 *	输入参数	：portNum端口号
 *	输出参数	：无
 *	返回值说明：成功或者失败
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_AddPortNum(uint16 portNum){
    int32 i =0;

    /*入参检查*/
    if(portNum <=0){/*lint !e775 */
        return FALSE;
    }

    /*是否是限制端口*/
    for(i=0;i<APPSMS_RESTRICT_PORT_NUM;i++){
        if(portNum == g_wRestrictPort[i]){
            return FALSE;
        }
    }

    /*是否已经存在*/
    if(APPSMS_ExistPortNum(portNum)){
        return FALSE;
    }else{
        return APPSMS_AddPortNumToList(portNum);
    }
}


/*****************************************************************************
 *	函数名		：APPSMS_SendSms
 *	功能		       ：发送短消息
 *	输入参数	：javaSms待发送短消息
 *	输出参数	：无
 *	返回值说明：短消息句柄，错误返回0
 *	其他说明	：
 *****************************************************************************/
int32 APPSMS_SendSms(const T_APPSMS_MsgForJava *javaSms){

    T_APPSMS_MsgForList tSendSms;
    T_APPSMS_MsgForList* ptSendSms;
    T_APPSMS_MsgForNet netSms;
    BOOLEAN  bTemp;
    BOOLEAN bNetState;
    int32 nTemp;
    int32 nHandle;
    int32 i;
    uint8 nSegNum;

    int32 ref=0;
    BOOLEAN hasPort = TRUE;
    //javacall_printf("[appsms_Core][APPSMS_SendSms]>>  javaSms=%d &javaSms msgBuffer=%d",javaSms,&(javaSms->msgBuffer));

    javacall_printf("[appsms_Core][APPSMS_SendSms]>>  javaSms=%d javaSms->destPort=%d",javaSms,javaSms->destPort);

    if (0 == javaSms->destPort)
    {
		hasPort = FALSE;
    }
    /*计算短消息分段*/
    nSegNum = APPSMS_GetSmsSegments(javaSms->msgType,javaSms->msgBufferLen,hasPort);
    javacall_printf("[appsms_Core][APPSMS_SendSms]>>APPSMS_GetSmsSegments  nSegNum=%d",nSegNum);
    
    /*如果分段数为0，返回错误*/
    if(nSegNum == 0){
        return 0;
    }
    /*长短消息太长*/
    if(nSegNum > APPSMS_MAX_SMS_NUM){
        return 0;
    }
    /*  为长短消息生成参考码  */
    if(nSegNum>1){
        ref = APPSMS_GenReference();
    }
    /*生成短消息句柄*/
    nHandle = APPSMS_GenHandle();

    /*  把短消息拆分并保存到待发送列表中 */
     /* 如果拆分失败，删除已经拆分的短消息，返回错误 */
     /* 如果拆分成功，把物理短消息保存到待发送列表中 */
    for(i=1 ; i<=nSegNum ; i++){
        bTemp = APPSMS_MsgSegment(javaSms , i ,nSegNum,ref, &tSendSms, hasPort);            
        if(bTemp==0){
            APPSMS_DelSendSmsByHandle(nHandle);
            return 0;
        }else{
            bTemp = APPSMS_AddSendSmsMsg(&tSendSms ,nHandle);
            if(bTemp==0){
                APPSMS_DelSendSmsByHandle(nHandle);
                return 0;
            }
        }
    }
    /*判断最近短消息发送时间是否为5分钟之前*/
    /*如果最近短消息发送时间为5分钟之前,则视为上一条短消息的发送结果丢失*/
    /*重新发送短消息*/
    if(0!=g_NewSendTime&&((APPSMS_GetCurrentTime()-g_NewSendTime)>=5*60)){
        bTemp = APPSMS_GetFirstSendSms(&ptSendSms, &nTemp);
        if(bTemp == 0){
            APPSMS_DelSendSmsByHandle(nHandle);
            return 0;
        }else{
            bTemp = APPSMS_EncodeSms(ptSendSms , &netSms);
            if(bTemp == 0){
                APPSMS_DelSendSmsByHandle(nHandle);
               return 0;
            }else{
                bTemp = APPSMS_SendSmsNet(ptSendSms,&netSms);
                if(bTemp == 0){
                    APPSMS_DelSendSmsByHandle(nHandle);
                    return 0;
                }else{
                     g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling 修改最近发送时间为当前时间
                    APPSMS_SetNetState(1);
                    return nHandle;
                }
            }
        }
    }else{
        /* 申请同步信号量 */
        /* 如果申请失败，则函数返回 */
        /* 如果申请成功，则检查网络层状态，选择是否发送短消息 */
        
        bTemp =  APPSMS_GetMutex(1000);
        if(bTemp == 0){
            APPSMS_PutMutex();
            return nHandle;
        }else{
            bNetState = APPSMS_CheckNetState();
            APPSMS_PutMutex();
        }
        

        /* 如果网络层状态为忙，则函数返回短消息句柄 */
        /* 如果网络层状态为闲，则取出待发送短消息列表中的第一条短消息 */
        /* 对短消息进行编码发送 */
        /* 如果发送失败，删除本条长短消息的所有物理短消息，返回错误 */
        /* 如果发送成功，把网络状态置为忙，返回短消息句柄 */
        if(bNetState == 1){
            return nHandle;
        }else{
            bTemp = APPSMS_GetFirstSendSms(&ptSendSms, &nTemp);
            if(bTemp == 0){
                APPSMS_DelSendSmsByHandle(nHandle);
                return 0;
            }
            else{
                bTemp = APPSMS_EncodeSms(ptSendSms , &netSms);
                if(bTemp == 0){
                    APPSMS_DelSendSmsByHandle(nHandle);
                   return 0;
                }else{
                    bTemp = APPSMS_SendSmsNet(ptSendSms,&netSms);
                    if(bTemp == 0){
                        APPSMS_DelSendSmsByHandle(nHandle);
                        return 0;
                    }else{
                         g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling 修改最近发送时间为当前时间
                        APPSMS_SetNetState(1);
                        return nHandle;
                    }
                }
            }
        }
    }
}

/*****************************************************************************
 *	函数名		：APPSMS_getSmsSegments
 *	功能		       ：计算长短消息的分段数
 *	输入参数	：msgType短消息编码类型，msgBufferLen短消息数据长度，
 *                                     hasPort有无端口号
 *	输出参数	：无
 *	返回值说明：分段数
 *	其他说明	：
 *****************************************************************************/
int32 APPSMS_GetSmsSegments (  T_APPSMS_Encoding   msgType,
                             int32              msgBufferLen,
							 BOOLEAN    hasPort){
   javacall_printf("---Core APPSMS_GetSmsSegments msgType:%d",msgType);
    /*有端口号*/
    if(hasPort == TRUE){
        /*7-bit编码*/
        if(msgType == APPSMS_SMSENCODE_7_BIT ){
            if(msgBufferLen <= 152){
                return 1;
            }else{
                if(msgBufferLen%146 == 0){
                    return msgBufferLen/146;
                }
                else{
                    return (msgBufferLen/146+1);
                }
            }
        }
        
        /*8-bit编码*/
        else if(msgType == APPSMS_SMSENCODE_8_BIT){
            if(msgBufferLen <= 133){
                return 1;
            }else{
                if(msgBufferLen%128== 0){
                    return msgBufferLen/128;
                }else{
                    return (msgBufferLen/128+1);
                }
            }
        }
        
        /*16-bit编码*/
        else if(msgType == APPSMS_SMSENCODE_UCS2){
            if(msgBufferLen <= 133){
                return 1;
            }else{
                if(msgBufferLen%128 == 0){
                    return msgBufferLen/128;
                }else{
                    return (msgBufferLen/128+1);
                }
            }
        }
        
        /*未知编码*/
        else
        {
            return 0;
        }
        	
    }
    /*无端口号*/
    else{
        /*7-bit编码*/
        if(msgType == APPSMS_SMSENCODE_7_BIT ){
            if(msgBufferLen <= 160){
                return 1;
            }else{
                if(msgBufferLen%153 == 0){
                    return msgBufferLen/153;
                }else{
                    return (msgBufferLen/153+1);
                }
            }
        }
        
        /*8-bit编码*/
        else if(msgType == APPSMS_SMSENCODE_8_BIT){
            if(msgBufferLen <= 140){
                return 1;
            }else{
                if(msgBufferLen%134== 0){
                    return msgBufferLen/134;
                }else{
                    return (msgBufferLen/134+1);
                }
            }
        }
        
        /*16-bit编码*/
        else if(msgType == APPSMS_SMSENCODE_UCS2){
            if(msgBufferLen <= 140){
                return 1;
            }else{
                if(msgBufferLen%134 == 0){
                    return msgBufferLen/134;
                }else{
                    return (msgBufferLen/134+1);
                }
            }
        }
        
        /*未知编码*/
        else{
            return 0;
        }
    }
}

/*****************************************************************************
 *	函数名		：APPSMS_MsgSegment
 *	功能		       ：折分短消息
 *	输入参数	：msg长短消息，pos欲抽取的物理短消息位置
 *                                      max长短消息分段数，hasPort有无端口
 *	输出参数	：sendSms抽取出的物理短消息
 *	返回值说明：成功或者失败
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_MsgSegment(const T_APPSMS_MsgForJava *msg , uint8 pos ,
                     uint8 max,uint8 ref,T_APPSMS_MsgForList *sendSms,BOOLEAN hasPort){
    int32 nStartPos;
    int32 nEndPos;

    /* 计算物理短消息在长短消息中的起始位置和结束位置
    * 和计算长短消息的分段数类似 */
    if(hasPort == TRUE){
        if(msg->msgType == APPSMS_SMSENCODE_7_BIT){
            nStartPos = (pos-1) * 146;
            if(msg->msgBufferLen <= 152){
                nEndPos = msg->msgBufferLen;
            }else{
                if(msg->msgBufferLen < pos * 146){
                    nEndPos = msg->msgBufferLen;
                }else{
                    nEndPos = pos * 146;
                }
            }
        }else if(msg->msgType == APPSMS_SMSENCODE_8_BIT){
            nStartPos = (pos-1) * 128;
            if(msg->msgBufferLen <= 133){
                nEndPos = msg->msgBufferLen;
            }else{
                if(msg->msgBufferLen < pos * 128){
                    nEndPos = msg->msgBufferLen;
                }else{
                    nEndPos = pos * 128;
                }
            }
        }else if(msg->msgType == APPSMS_SMSENCODE_UCS2){
            nStartPos = (pos-1) * 128;
            if(msg->msgBufferLen <= 132){
                nEndPos = msg->msgBufferLen;
            }else{
                if(msg->msgBufferLen < pos * 128){
                    nEndPos = msg->msgBufferLen;
                }else{
                    nEndPos = pos * 128;
                }
            }
        }else{
            return FALSE;
        }
    }else{
        if(msg->msgType == APPSMS_SMSENCODE_7_BIT){
            nStartPos = (pos-1) * 153;
            if(msg->msgBufferLen <= 160){
                nEndPos = msg->msgBufferLen;
            }else{
                if(msg->msgBufferLen < pos * 153){
                    nEndPos = msg->msgBufferLen;
                }else{
                    nEndPos = pos * 153;
                }
            }
        }else if(msg->msgType == APPSMS_SMSENCODE_8_BIT){
            nStartPos = (pos-1) * 134;
            if(msg->msgBufferLen <= 140){
                nEndPos = msg->msgBufferLen;
            }else{
                if(msg->msgBufferLen < pos * 134){
                    nEndPos = msg->msgBufferLen;
                }else{
                    nEndPos = pos * 134;
                }
            }
        }else if(msg->msgType == APPSMS_SMSENCODE_UCS2){
            nStartPos = (pos-1) * 134;
            if(msg->msgBufferLen <= 140){
                nEndPos = msg->msgBufferLen;
            }else{
                if(msg->msgBufferLen < pos * 134){
                    nEndPos = msg->msgBufferLen;
                }else{
                    nEndPos = pos * 134;
                }
            }
        }else{
            return FALSE;
        }
        if(nStartPos <0 || nStartPos >= nEndPos){
            return FALSE;
        }
    }
        
    /* 提取物理短消息 */
    sendSms->msgType = msg->msgType;
    //SCI_MEMCPY(sendSms->address,msg->address,APPSMS_MAX_ADD_LEN);
    strcpy((char*)sendSms->address,(char*)msg->address);
    sendSms->destPort = msg->destPort;
    sendSms->msgBufferLen=nEndPos-nStartPos;
    if(sendSms->msgBufferLen > APPSMS_7BIT_SMS_LEN){
        return FALSE;
    }
    SCI_MEMCPY(sendSms->msgBuffer,msg->msgBuffer+nStartPos,sendSms->msgBufferLen);
    sendSms->sourcePort = msg->sourcePort;
    sendSms->timeStamp = msg->timeStamp;
    sendSms->seq = (unsigned char)pos;
    sendSms->max = (unsigned char)max;
    sendSms->ref = (unsigned char)ref;
    return TRUE;
}

/*****************************************************************************
 *	函数名		：APPSMS_GenHandle
 *	功能		       ：生成短消息句柄
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：句柄
 *	其他说明	：
 *****************************************************************************/
int32 APPSMS_GenHandle(){
   int32 lHandle;
   lHandle = APPSMS_GetCurrentTime();
   lHandle =  g_nHandleNum*1000000+lHandle%1000000;
  
   g_nHandleNum++;
   if(g_nHandleNum == 4293){
       g_nHandleNum=1;
   }
   javacall_printf("---Core APPSMS_GenHandle lHandle:%d",lHandle);
   return (int32)lHandle;
}

/*****************************************************************************
 *	函数名		：APPSMS_GenReference
 *	功能		       ：生成长短消息参考码
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：参考码
 *	其他说明	：
 *****************************************************************************/
int32 APPSMS_GenReference(){
    int32 lRef;
    lRef= APPSMS_GetCurrentTime();
    lRef = g_nReferenceNum*10+lRef%10;

    g_nReferenceNum++;
    if(g_nReferenceNum == 24){
        g_nReferenceNum=0;
    }
   javacall_printf("---Core APPSMS_GenReference lRef:%d",lRef);
   return (int32)lRef;
}

/*****************************************************************************
 *	函数名		：APPSMS_EncodeSms
 *	功能		       ：对待发送短消息进行编码
 *	输入参数	：msg编码前短消息
 *	输出参数	：sendSms编码后短消息
 *	返回值说明：成功或者失败
 *	其他说明	：认为短消息有16-bit的端口号
 *****************************************************************************/
BOOLEAN APPSMS_EncodeSms(const T_APPSMS_MsgForList *msg,T_APPSMS_MsgForNet *sendSms){
    int32 nStartPos=0;
    int32 i=0;
    
    //SCI_MEMCPY(sendSms->address,msg->address,APPSMS_MAX_ADD_LEN);
    strcpy((char*)sendSms->address,(char*)msg->address);
    sendSms->msgType = msg->msgType;
    sendSms->timeStamp = msg->timeStamp;
    SCI_MEMSET(sendSms->msgBuffer,'\0',APPSMS_8BIT_SMS_LEN);

    /*这里固定使用16-bit的端口号*/
    /*IEI*/
    sendSms->msgBuffer[2] = 5;
    /*IEDL*/
    sendSms->msgBuffer[3] = 4;
    /*Destination port*/
    sendSms->msgBuffer[4] = msg->destPort/256;
    sendSms->msgBuffer[5] = msg->destPort%256;
    /*Originator port*/
    sendSms->msgBuffer[6] = msg->sourcePort/256;
    sendSms->msgBuffer[7] = msg->sourcePort%256;

    if (0 == msg->destPort)
    {
        /*非长短消息*/
        if(msg->max==1){
            nStartPos = 1;
        }
        /*长短消息*/
        else{
            /*UDHL*/
            sendSms->msgBuffer[1] = 5;
            /*IEI*/
            sendSms->msgBuffer[2]= 0;
            /*IEDL*/
            sendSms->msgBuffer[3]= 3;
            /*ref*/
            sendSms->msgBuffer[4]= (uint8)msg->ref;
            /*max*/
            sendSms->msgBuffer[5]= (uint8)msg->max;
            /*seq*/
            sendSms->msgBuffer[6]= (uint8)msg->seq;
            
            nStartPos = 7;		
        }
    }
    else
    {
        /*非长短消息*/
        if(msg->max==1){
            /*UDHL*/
            sendSms->msgBuffer[1] = 6;
            nStartPos = 8;
        }
        /*长短消息*/
        else{
            /*UDHL*/
            sendSms->msgBuffer[1] = 11;
            /*IEI*/
            sendSms->msgBuffer[8]= 0;
            /*IEDL*/
            sendSms->msgBuffer[9]= 3;
            /*ref*/
            sendSms->msgBuffer[10]= (uint8)msg->ref;
            /*max*/
            sendSms->msgBuffer[11]= (uint8)msg->max;
            /*seq*/
            sendSms->msgBuffer[12]= (uint8)msg->seq;
            
            nStartPos = 13;
        }
    }

	/*计算编码后的消息长度*/
    if(msg->msgType == APPSMS_SMSENCODE_7_BIT){
        sendSms->msgBuffer[0] = msg->msgBufferLen+(((nStartPos-1)*8%7==0)?
			((nStartPos-1)*8/7):((nStartPos-1)*8/7+1));

	sendSms->msgBufferLen = ((sendSms->msgBuffer[0])*7%8==0)?
		((sendSms->msgBuffer[0])*7/8+1):((sendSms->msgBuffer[0])*7/8+2);    
    }else if(msg->msgType == APPSMS_SMSENCODE_8_BIT){
        sendSms->msgBufferLen = nStartPos+msg->msgBufferLen;
        sendSms->msgBuffer[0]=sendSms->msgBufferLen-1;
    }else if(msg->msgType == APPSMS_SMSENCODE_UCS2){
        sendSms->msgBufferLen = nStartPos+msg->msgBufferLen;
        sendSms->msgBuffer[0]=sendSms->msgBufferLen-1;
    }else{
        return FALSE;
    }

    /*payload+udh>141*/
    if(sendSms->msgBufferLen >APPSMS_8BIT_SMS_LEN+1){
        return FALSE;
    }

    /*  7-bit 编码，需要特殊设置  */
    /*  8-bit 与16-bit，直接内存拷贝  */
    if(msg->msgType == APPSMS_SMSENCODE_7_BIT){
        nStartPos--; /*  第一个字节UDL  不参与编码  */
		/*test*/
		//nStartPos=0;
		/*test*/
        nStartPos = (nStartPos*8%7==0)?(nStartPos*8/7):(nStartPos*8/7+1);
        for(i=0;i<msg->msgBufferLen;i++){
           APPSMS_Set7BitBuff(sendSms->msgBuffer+1, msg->msgBufferLen, i+nStartPos, msg->msgBuffer[i]);
        }
    }else{
        SCI_MEMCPY(sendSms->msgBuffer+nStartPos,msg->msgBuffer,msg->msgBufferLen);
    }
    return TRUE;
}


/*****************************************************************************
 *	函数名		：APPSMS_DecodeSms
 *	功能		       ：对接收的短消息进行解码
 *	输入参数	：recvSms解码前短消息
 *	输出参数	：msg解码后短消息
 *	返回值说明：成功或者失败
 *	其他说明	：认为短消息一定有用户数据头
 *****************************************************************************/
BOOLEAN APPSMS_DecodeSms(const T_APPSMS_MsgForNet *recvSms, T_APPSMS_MsgForList *msg){
    uint8 nUDHL = 0;
    uint8 nIEI;
    uint8 nIEDL;
    uint8 n7BitUDHL=0;
    int32 i;
    
    /*设置短消息的默认值*/
    msg->destPort = 0;
    msg->sourcePort = 0;
    msg->max = 1;
    msg->ref = 1;
    msg->seq = 1;

    /*短消息无用户数据，返回错误*/
    if(recvSms->msgBufferLen < 1){
        return FALSE;
    }
    /*无用户数据头，返回错误*/
    nUDHL = recvSms->msgBuffer[1];
    if(nUDHL < 6){
        return FALSE;
    }

   msg->timeStamp = recvSms->timeStamp;
   msg->msgType = recvSms->msgType;
   //SCI_MEMCPY(msg->address , recvSms->address , APPSMS_MAX_ADD_LEN);
   strcpy((char*)msg->address, (char*)recvSms->address);

    /*解析用户数据头*/
    for(i=2;i<nUDHL+1;){
        nIEI = recvSms->msgBuffer[i];
        nIEDL = recvSms->msgBuffer[i+1];

        /*处理IEI*/
        switch(nIEI){
            case 0:  /*长短消息*/
            	msg->ref = recvSms->msgBuffer[i+2];
            	msg->max = recvSms->msgBuffer[i+3];
            	msg->seq = recvSms->msgBuffer[i+4];
            	break;
            case 4:  /*8-bit 的端口号*/
            	msg->sourcePort = recvSms->msgBuffer[i+3];
            	msg->destPort = recvSms->msgBuffer[i+2];
            	break;
            case 5:  /*16-bit 的端口号*/
            	msg->destPort = recvSms->msgBuffer[i+2]*256+recvSms->msgBuffer[i+3];
            	msg->sourcePort = recvSms->msgBuffer[i+4]*256+recvSms->msgBuffer[i+5];
            	break;
            case 8:  /*16-bit ref 的长短消息*/
            	msg->ref = recvSms->msgBuffer[i+2]*256+recvSms->msgBuffer[i+3];
            	msg->max = recvSms->msgBuffer[i+4];
            	msg->seq = recvSms->msgBuffer[i+5];
            	break;
            default :
            	break;
        }        
        /*i 指向下一个IEI*/
        i=i+2+nIEDL;
    }

    /*解析有效承载*/
    /*对于7-bit 编码类型，需要特殊的解码*/
    /*8-bit 和16-bit 直接内存拷贝*/
    if(recvSms->msgType == APPSMS_SMSENCODE_7_BIT){
        //modify by zhouyang 2008-08-10 和解码相对应
        //msg->msgBufferLen = (recvSms->msgBufferLen -nUDHL-2)*8/7;
        n7BitUDHL = ((nUDHL+1)*8%7==0)?((nUDHL+1)*8/7):((nUDHL+1)*8/7+1);
        msg->msgBufferLen = (recvSms->msgBufferLen-1)*8/7-n7BitUDHL;
        for(i=0;i<msg->msgBufferLen;i++){
            /*test */
            APPSMS_Get7BitBuff((const uint8*)recvSms->msgBuffer+1 , recvSms->msgBufferLen, i+n7BitUDHL, (int8*)((msg->msgBuffer)+i));
        }
    }
    else{
        msg->msgBufferLen = recvSms->msgBufferLen -nUDHL-2;
        SCI_MEMCPY(msg->msgBuffer,recvSms->msgBuffer+nUDHL+2,msg->msgBufferLen);
    }
    return TRUE;
}

/*****************************************************************************
 *	函数名		：APPSMS_ReceiveSendResult
 *	功能		       ：处理短消息发送结果
 *	输入参数	：result：短消息发送结果
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：请参考程序流程图，包含三大分支。其中分支“长短消息
 *                                     没有全部发送完成”有点改变，增加了检查有无下一条短消息
 *                                     业务上说，一定有下一条待发送短消息，这里再检查一次主要
 *                                     是为了共享代码，也可以提高代码的容错能力
 *****************************************************************************/
void APPSMS_ReceiveSendResult(T_APPSMS_SendResult result)
{
    BOOLEAN bTemp;
    T_APPSMS_MsgForNet tNetSms;
    
    /*获取最近发送的短消息*/
    T_APPSMS_MsgForList * ptSendSms;
    int32 nHandle;
	
    bTemp = APPSMS_GetFirstSendSms(&ptSendSms,  &nHandle);
    if(bTemp == FALSE){
        return;
    }

    /*判断长短消息是否全部发送完成或者为非长短消息*/
    if(ptSendSms->max == ptSendSms->seq){
        APPSMS_SendResultToVM(result , nHandle);
        APPSMS_DelFirstSendSms();
        while(1){/*lint !e716*/
            bTemp =  APPSMS_GetMutex(1000);

            if(bTemp == FALSE){
                APPSMS_PutMutex();
                return;
            }else{

                /*获取下一条待发送短消息，如果没有，返回FALSE*/
                bTemp = APPSMS_GetFirstSendSms(&ptSendSms,  &nHandle);
                if(bTemp == FALSE){
                    APPSMS_SetNetState(FALSE);
                    APPSMS_PutMutex();
                    return;
                }else  /*发送下一条短消息*/{
                    APPSMS_PutMutex();

                    bTemp = APPSMS_EncodeSms(ptSendSms , &tNetSms);
                    if(bTemp == FALSE){
                        APPSMS_DelSendSmsByHandle(nHandle);
                        APPSMS_SendResultToVM(APPSMS_SMSSEND_FAILENCODE , nHandle);
                        return;
                    }else{
                        bTemp = APPSMS_SendSmsNet(ptSendSms,&tNetSms);
                        if(bTemp == FALSE){ 
                            APPSMS_SendResultToVM(APPSMS_SMSSEND_FAIL , nHandle);
                            APPSMS_DelSendSmsByHandle(nHandle);
                        }else{
                    	    g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling 修改最近发送时间为当前时间
                    	     return;
                    	  }
                    }
                }
            }
        }
    }else{
        if(result == APPSMS_SMSSEND_SUCCESS)  /*发送成功*/{
            APPSMS_DelFirstSendSms();
            while(1){/*lint !e716*/
                bTemp =  APPSMS_GetMutex(1000);
                if(bTemp == FALSE){
                    APPSMS_PutMutex();
                    return;
                }else{
                    /*获取下一条待发送短消息，如果没有，返回FALSE*/
                    bTemp = APPSMS_GetFirstSendSms(&ptSendSms,  &nHandle);
                    if(bTemp == FALSE){
                        APPSMS_SetNetState(FALSE);
                        APPSMS_PutMutex();
                        return;
                    }else{
                        /*发送下一条短消息*/
                        APPSMS_PutMutex();

                        bTemp = APPSMS_EncodeSms(ptSendSms , &tNetSms);
                        if(bTemp == FALSE){
                            APPSMS_DelSendSmsByHandle(nHandle);
                            APPSMS_SendResultToVM(APPSMS_SMSSEND_FAILENCODE , nHandle);
                            return;
                        }else{
                            bTemp = APPSMS_SendSmsNet(ptSendSms,&tNetSms);
                            if(bTemp == FALSE){ 
                                APPSMS_SendResultToVM(APPSMS_SMSSEND_FAIL , nHandle);
                                APPSMS_DelSendSmsByHandle(nHandle);
                            }else{
                    	           g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling 修改最近发送时间为当前时间
                    	           return;
                    	      }
                        }
                    }
                }
            }
        }
        else    /*发送失败*/
        {
            APPSMS_SendResultToVM(result , nHandle);
            APPSMS_DelSendSmsByHandle(nHandle);
            while(1){/*lint !e716*/
                bTemp =  APPSMS_GetMutex(1000);
                if(bTemp == FALSE){
                    APPSMS_PutMutex();
                    return;
                }else{
                    /*获取下一条待发送短消息，如果没有，返回FALSE*/
                    bTemp = APPSMS_GetFirstSendSms(&ptSendSms,  &nHandle);
                    if(bTemp == FALSE){ 
                        APPSMS_SetNetState(FALSE);
                        APPSMS_PutMutex();
                        return;
                    }else{
                        /*发送下一条短消息*/
                        APPSMS_PutMutex();

                        bTemp = APPSMS_EncodeSms(ptSendSms , &tNetSms);
                        if(bTemp == FALSE){
                            APPSMS_DelSendSmsByHandle(nHandle);
                            APPSMS_SendResultToVM(APPSMS_SMSSEND_FAILENCODE , nHandle);
                            return;
                        }else{
                            bTemp = APPSMS_SendSmsNet(ptSendSms,&tNetSms);
                            if(bTemp == FALSE){
                                APPSMS_SendResultToVM(APPSMS_SMSSEND_FAIL , nHandle);
                                APPSMS_DelSendSmsByHandle(nHandle);
                            }else{
                    	           g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling 修改最近发送时间为当前时间
                    	           return;
                    	    }
                    	 }
                    }
                }
            }
        }
    }
}

/*****************************************************************************
 *	函数名		：APPSMS_ReceiveRecvSms
 *	功能		       ：处理接收到的短消息
 *	输入参数	：recvSms：接收到的短消息
 *	输出参数	：无
 *	返回值说明：0表示不是Java短消息，非0表示是Java短消息
 *	其他说明	：无
 *****************************************************************************/
void  APPSMS_ReceiveRecvSms(const T_APPSMS_MsgForNet *recvSms)
{
    T_APPSMS_MsgForList tListSms;
    T_APPSMS_MsgForJava tJavaSms;
    BOOLEAN bTemp;

    bTemp = APPSMS_DecodeSms(recvSms, &tListSms);
    if(bTemp == FALSE){
        return;
    }

    javacall_printf(LOG_STR("receive message port=%d"), tListSms.destPort);
    /*如果为WAP PUSH 短消息或者其端口号在目标列表中*/
    if(tListSms.destPort == APPSMS_PORT_2948 || tListSms.destPort == APPSMS_PORT_2949 
        || APPSMS_ExistPortNum(tListSms.destPort)){
        /*长短消息*/
        if(tListSms.max > 1){
            bTemp = APPSMS_CheckLongRecvSms(&tListSms);
            /*长短消息接收未完成*/
            if(bTemp == FALSE){
                APPSMS_DelRecvSmsByDate();
                bTemp = APPSMS_AddRecvSmsToList(&tListSms);
                if(bTemp == FALSE){
                    ;/*缓存已满*/
                }
                return;
            }
            else  /*长短消息接收完成，串接长短消息，并从列表中删除*/
            {
                 APPSMS_MsgConcatenation(&tListSms, &tJavaSms);
                 APPSMS_DelRecvSmsByRef(&tListSms);
            }
        }
        else  /*非长短消息，此处非串接，仅仅是改变数据结构*/
        {
            APPSMS_MsgConcatenation(&tListSms, &tJavaSms);
        }

        /*处理WAP PUSH 短消息*/
        if(tListSms.destPort == APPSMS_PORT_2948 || tListSms.destPort == APPSMS_PORT_2949){
/*   
            //P900 中的代码
            g_tPushConcatenateContext.wDestinationPort = tJavaSms.destPort;
	        g_tPushConcatenateContext.wOriginatorPort = tJavaSms.sourcePort;
            g_tPushConcatenateContext.wPushDataLen = tJavaSms.msgBufferLen;
            SCI_MEMCPY(g_tPushConcatenateContext.abPushData,tJavaSms.msgBuffer,tJavaSms.msgBufferLen);
            mmiPushEntry(0,NULL);
*/
            //javacall_printf("-------appsms_Core.APPSMS_ReceiveRecvSms().APPSMS_PORT_2948-------");
            return;
        }
        else /*向Java 组件发送短消息*/
        {
            APPSMS_SendMtToVM(&tJavaSms);
            return;
        }
        
    }
    else
    {
        return;
    }
}


/*****************************************************************************
 *	函数名		：APPSMS_CheckLongRecvSms
 *	功能		       ：检查指定短消息所在的长短消息是否全部接收完成
 *	输入参数	：接收短消息
 *	输出参数	：无
 *	返回值说明：TRUE 为完成；FALSE 为未完成
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_CheckLongRecvSms(const T_APPSMS_MsgForList *recvSms){
    int32 i;
    uint8 num=1;

    for(i=0;i<APPSMS_MAX_SMS_NUM;i++){
        if(g_tappsms_recvSmsList[i].useFlag == TRUE  
        	&& (strcmp((char *)g_tappsms_recvSmsList[i].recvSms.address,(char *)recvSms->address))==0
        	&& g_tappsms_recvSmsList[i].recvSms.ref == recvSms->ref
        	&& g_tappsms_recvSmsList[i].recvSms.max == recvSms->max){
            if(g_tappsms_recvSmsList[i].recvSms.seq == recvSms->seq){
                 return FALSE;
            }else{
                num++;
            }
        }
    }
    if(num==recvSms->max){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

/*****************************************************************************
 *	函数名		：APPSMS_AddRecvSmsToList
 *	功能		       ：添加接收短消息到链表中
 *	输入参数	：要添加的短消息
 *	输出参数	：无
 *	返回值说明：FALSE 为缓存已满
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_AddRecvSmsToList(const T_APPSMS_MsgForList *recvSms){
    int32 i;
	
    for(i=0;i<APPSMS_MAX_SMS_NUM;i++){
        if(g_tappsms_recvSmsList[i].useFlag == FALSE){
            SCI_MEMCPY(&(g_tappsms_recvSmsList[i].recvSms),recvSms,sizeof(T_APPSMS_MsgForList));
            g_tappsms_recvSmsList[i].useFlag = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************
 *	函数名		：APPSMS_MsgConcatenation
 *	功能		       ：串解长短消息
 *	输入参数	：recvSms要串接的短消息
 *	输出参数	：msg串接后的短消息
 *	返回值说明  ：无
 *	其他说明	：当要串接的短消息为非长短消息时，
 *                其功能仅仅是数据结构转换
 *****************************************************************************/
void APPSMS_MsgConcatenation (const T_APPSMS_MsgForList *recvSms,T_APPSMS_MsgForJava *msg){
     /* 对于同一条长短消息，每个物理短消息的有效承载数据长度
     * 应该是相同的（最后一条除外）*/
    int32 nSegmentLen = 0; /* 每一物理短消息的有效承载数据长度 */
    int32 i =0;

    SCI_MEMSET(msg,0,sizeof(T_APPSMS_MsgForJava));
    //SCI_MEMCPY(msg->address , recvSms->address,APPSMS_MAX_ADD_LEN);
    strcpy((char*)msg->address, (char*)recvSms->address);
    msg->destPort = recvSms->destPort;
    msg->sourcePort = recvSms->sourcePort;
    msg->msgType = recvSms->msgType;
    msg->timeStamp = recvSms->timeStamp;
    msg->msgBufferLen = 0;

    /* 长短消息 */
    if(recvSms->max >1){
        if(recvSms->seq != recvSms->max){
            nSegmentLen = recvSms->msgBufferLen;
        }
        for(i=0;i<APPSMS_MAX_SMS_NUM;i++){
            if(g_tappsms_recvSmsList[i].useFlag == 1
        	    && strcmp((char *)g_tappsms_recvSmsList[i].recvSms.address,(char *)recvSms->address)==0
        	    && g_tappsms_recvSmsList[i].recvSms.ref == recvSms->ref
            	    && g_tappsms_recvSmsList[i].recvSms.max == recvSms->max){
                if(nSegmentLen == 0){
                    nSegmentLen = g_tappsms_recvSmsList[i].recvSms.msgBufferLen;
                }
                SCI_MEMCPY(msg->msgBuffer+(g_tappsms_recvSmsList[i].recvSms.seq-1)*nSegmentLen,
            	                 g_tappsms_recvSmsList[i].recvSms.msgBuffer,
            	                 g_tappsms_recvSmsList[i].recvSms.msgBufferLen);
                msg->msgBufferLen = msg->msgBufferLen+g_tappsms_recvSmsList[i].recvSms.msgBufferLen;
            }
        }
    }
    /*  对于非长短消息，(recvSms.seq-1)XnSegmentLen 为0X0 */
    SCI_MEMCPY(msg->msgBuffer+(recvSms->seq-1)*nSegmentLen,recvSms->msgBuffer,recvSms->msgBufferLen);
    msg->msgBufferLen = msg->msgBufferLen + recvSms->msgBufferLen;

    return;
}

/*****************************************************************************
 *	函数名		：APPSMS_DelRecvSmsByRef
 *	功能		       ：删除短消息列表中与指定的短消息为同一条
 *                                长短消息的所有短消息
 *	输入参数	：recvSms 指定的接收到的短消息
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：
 *****************************************************************************/
void APPSMS_DelRecvSmsByRef (const T_APPSMS_MsgForList *recvSms){
    int32 i;
	
    for(i=0;i<APPSMS_MAX_SMS_NUM;i++){
        if(g_tappsms_recvSmsList[i].useFlag == TRUE
        	&& strcmp((char *)g_tappsms_recvSmsList[i].recvSms.address,(char *)recvSms->address)==0
        	&& g_tappsms_recvSmsList[i].recvSms.ref == recvSms->ref
        	&& g_tappsms_recvSmsList[i].recvSms.max == recvSms->max){
            g_tappsms_recvSmsList[i].useFlag = FALSE;
        }
    }
}

/*****************************************************************************
 *	函数名		：APPSMS_DelRecvSmsByDate
 *	功能		       ：删除接收短消息链表中五天前的短消息
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：
 *****************************************************************************/
void APPSMS_DelRecvSmsByDate(){
    int32 i;
    int32 lTime = APPSMS_GetCurrentTime() - 5*24*60*60;
	
    for(i=0;i<APPSMS_MAX_SMS_NUM;i++){
        if (g_tappsms_recvSmsList[i].useFlag == TRUE && (int32)g_tappsms_recvSmsList[i].recvSms.timeStamp < lTime){
            g_tappsms_recvSmsList[i].useFlag = FALSE;
        }
    }
}

/*****************************************************************************
 *	函数名		：APPSMS_Set7BitBuff
 *	功能		       ：设置存贮区中的某个7-bit 数据
 *	输入参数	：buff 7-bit 编码方式下的数据 存贮空间，
 *                                     buffLen 7-bit 数据存贮空间的大小(byte 数)
 *                                     pos septet 的序列号，从0 开始。如果第10 个septet
 *                                     data7Bit  需要保存的7-bit 数据，ASCII 码
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：
 *****************************************************************************/
static BOOLEAN APPSMS_Set7BitBuff(uint8* buff , int32 buffLen , int32 pos , int8 data7Bit){
    /*把data7Bit 保存以7-bit 编码方式保存到buff 中，
    *最多可能会跨两个byte，也可能只一个byte*/
    int32 nFirstBytePos;    /*第一个byte 位置*/
    int32 nSecondBytePos;    /*第二个byte 位置*/
    
    uint8 nFirstBitNum;    /*在第一个byte 中保存的字位数*/
    uint8 nSecondBitNum;    /*在第二个byte 中保存的字位数*/

    /*test*/
    int32 nTest;
    /*test*/
    BOOLEAN bTemp;

    nSecondBitNum = (pos+1)*7%8;
    nFirstBitNum = 7 - (pos+1)*7%8;

    /*因为nFirstBytePos  从0  开始，所有结果需要-1 ，pos  需要+1*/
    nFirstBytePos = (pos+1)*7/8-1;    
    nSecondBytePos = nFirstBytePos+1;

    /*test*/
    nTest = buff[nFirstBytePos];
    nTest = buff[nSecondBytePos];
    nTest = data7Bit;
    /*test*/

    /*保存第一个byte*/
    if( nFirstBitNum != 0 ){
        /*把相应的位置0，前nFirstBytePos位*/
        bTemp = 0xFF;
        bTemp = bTemp>>nFirstBitNum;
        buff[nFirstBytePos]=buff[nFirstBytePos]&bTemp;

        /*设置相应的位*/
        /*nFirstBytePos+nSecondBitNum=7, bTemp是八位*/
        /*保留后nFirstBytePos位*/
        bTemp=data7Bit<<nSecondBitNum+1;/*现在bTemp 前nSecondBitNum+1 位都为0*/   /*lint !e504*/     
        buff[nFirstBytePos]=buff[nFirstBytePos]|bTemp;
    }
    /*保存第二个byte*/
    if(nSecondBitNum != 0){
         /*把相应的位置0，后nSecondBytePos位*/
        bTemp = 0xFF;
        bTemp = bTemp<<nSecondBitNum;       /*lint !e504*/
        buff[nSecondBytePos]=buff[nSecondBytePos]&bTemp;

        /*设置相应的位*/
        bTemp = data7Bit>>nFirstBitNum;/*char 类型第一位是无用的位，需要移走*/
        buff[nSecondBytePos]=buff[nSecondBytePos]|bTemp;
    }
    /*test*/
    nTest = buff[nFirstBytePos];
    nTest = buff[nSecondBytePos];
    /*test*/
    return TRUE;
}

/*****************************************************************************
 *	函数名		：APPSMS_Get7BitBuff
 *	功能		       ：读取存贮区中的某个7-bit 数据
 *	输入参数	：buff 7-bit 编码方式下的数据 ，
 *                                     buffLen 7-bit 数据存贮空间的大小(byte 数)
 *                                     pos septet 的序列号，从0 开始。如果第10 个septet
 *                                     data7Bit  用户保存的提取出的7-bit 数据，ASCII 码
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：可以参考函数APPSMS_Set7BitBuff
 *****************************************************************************/
static BOOLEAN APPSMS_Get7BitBuff(const uint8* buff , int32 buffLen , int32 pos , int8* data7Bit){
    /*把data7Bit 保存以7-bit 编码方式保存到buff 中，
    *最多可能会跨两个byte，也可能只一个byte*/
    int32 nFirstBytePos;    /*第一个byte 位置*/
    int32 nSecondBytePos;    /*第二个byte 位置*/
    
    uint8 nFirstBitNum;    /*在第一个byte 中保存的字位数*/
    uint8 nSecondBitNum;    /*在第二个byte 中保存的字位数*/

    BOOLEAN bTemp;

    nSecondBitNum = (pos+1)*7%8;
    nFirstBitNum = 7 - (pos+1)*7%8;

    /*因为nFirstBytePos  从0  开始，所有结果需要-1 ，pos  需要+1*/
    nFirstBytePos = (pos+1)*7/8-1;
    
    nSecondBytePos = nFirstBytePos+1;

    *data7Bit = 0;
    /*提取第一个byte*/
    if( nFirstBitNum != 0 ){
        /*设置相应的位*/
        bTemp=buff[nFirstBytePos]>>nSecondBitNum+1;    /*lint !e504*/  
        *data7Bit = *data7Bit|bTemp;
    }
    /*保存第二个byte*/
    if(nSecondBitNum != 0){
        /*设置相应的位*/
        bTemp = buff[nSecondBytePos]<<nFirstBitNum;
        *data7Bit=*data7Bit|bTemp;
    }

    /*最后需要把第一位去掉*/
    
    *data7Bit = *data7Bit & 0x7F;/*0x7F:0111 1111*/
    return TRUE;
}

/*****************************************************************************
 *	函数名		：APPSMS_ConvertTime2Long
 *	功能		       ：把年月日的日期格式转换成到2000 年1 月1 日的秒数
 *	输入参数	：year年，month月，day日，hour时，minute分，second秒
 *	输出参数	：到2000年1月1日0点0分0秒的秒数
 *	返回值说明：无
 *	其他说明	：计算闰年时，没有考虑100  和400  问题
 *****************************************************************************/
int32 APPSMS_ConvertTime2Long(uint8 year , uint8 month , uint8 day , uint8 hour , uint8 minute , uint8 second)
{
    /*一天86400  秒*/
    /*一年是(365)  天31536000*/
    /*一年是(366)  天31622400*/
    /*四年是 天126230400*/
    /*一月是(28)  天2419200*/
    /*一月是(29)  天2505600*/
    /*一月是(30)  天2592000*/
    /*一月是(31)  天2678400*/
    /*一小时3600  秒*/
    /*一分钟60  秒*/
    int32 lTime=0;
    /*检查日期数据的有效性，day  检查不是很全面*/
    if(year>=100){
        return lTime;
    }
    if(month>12 || month<1){
        return lTime;
    }
    if(day>31 || day<1){
        return lTime;
    }
    if(hour >23 ){
        return lTime;
    }
    if(minute>59){
        return lTime;
    }
    if(second>59){
        return lTime;
    }
    /*处理年*/
    lTime = (year)/4*126230400;
    switch((year)%4){
        case 3:
            lTime = lTime +  94694400;
            break;
        case 2:
            lTime = lTime +  63158400;
            break;
        case 1:
            lTime = lTime + 31622400;
            break;
        default:
            break;
    }
    /*处理月*/
    lTime  = lTime + (month-1)*2592000;
    if(month<9){
        lTime  = lTime + month/2*86400;
    }else{
        lTime  = lTime + (month+1)/2*86400;
    }
    if(month>2){
        if(year%4 == 0){
            lTime  = lTime - 86400;
        }else{
            lTime  = lTime - 172800;
       }
    }

    /*处理日*/
    lTime  = lTime + (day-1)*86400;

    /*处理时*/
    lTime  = lTime + hour*3600;

    /*处理分*/
    lTime  = lTime + minute*60;

    /*处理秒*/
    lTime  = lTime + second;

    return lTime;
}
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


