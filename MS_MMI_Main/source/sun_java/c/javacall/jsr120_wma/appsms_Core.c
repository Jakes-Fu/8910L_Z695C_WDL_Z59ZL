/*****************************************************************************
 *  ģ����  ��Java��ϵͳWMAģ�鱾�������
 *  �ļ���	��appsms_Core.c
 *  �ļ���ʶ��
 *  ����ļ���appsms_Core.h
 *  ʵ�ֹ��ܣ�����APPSMS������ڲ�ʹ�õ����ݽṹ�ͺ����ӿ�
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

/*PUSH���ӹ���������*/
//extern T_PUSH_CONCATENATE_CONTEXT	g_tPushConcatenateContext;

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/
 /*����ʹ�õĶ˿ں�*/
 uint16 g_wRestrictPort[APPSMS_RESTRICT_PORT_NUM]={2805,2923,2948,2949,5502,5503,
                                           5508,5511,5512,9200,9201,9202,9203,9207,49996,49999};



/*���ն���Ϣ�Ļ������*/
T_APPSMS_RecvSmsNode g_tappsms_recvSmsList[APPSMS_MAX_SMS_NUM];

/*����Ϣ���������*/
int32 g_nHandleNum = 1;

/*������Ϣ�ο��������*/
int32 g_nReferenceNum = 0;

/*�������Ϣ����ʱ��*/
int32 g_NewSendTime = 0;
/**************************************************************************
 *                     ȫ�ֺ�������                                      *
 **************************************************************************/
/*���ô������е�ĳ��7-bit ����*/
static BOOLEAN APPSMS_Set7BitBuff(uint8* buff , int32 buffLen , int32 pos , int8 data7Bit);

/*��ȡ�������е�ĳ��7-bit ����*/
static BOOLEAN APPSMS_Get7BitBuff(const uint8* buff , int32 buffLen , int32 pos , int8* data7Bit);
/**************************************************************************
 *                     ȫ�ֺ���ʵ��                                      *
 **************************************************************************/

/*****************************************************************************
 *	������		��APPSMS_AddPortNum
 *	����		       ��ע��˿ں�
 *	�������	��portNum�˿ں�
 *	�������	����
 *	����ֵ˵�����ɹ�����ʧ��
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_AddPortNum(uint16 portNum){
    int32 i =0;

    /*��μ��*/
    if(portNum <=0){/*lint !e775 */
        return FALSE;
    }

    /*�Ƿ������ƶ˿�*/
    for(i=0;i<APPSMS_RESTRICT_PORT_NUM;i++){
        if(portNum == g_wRestrictPort[i]){
            return FALSE;
        }
    }

    /*�Ƿ��Ѿ�����*/
    if(APPSMS_ExistPortNum(portNum)){
        return FALSE;
    }else{
        return APPSMS_AddPortNumToList(portNum);
    }
}


/*****************************************************************************
 *	������		��APPSMS_SendSms
 *	����		       �����Ͷ���Ϣ
 *	�������	��javaSms�����Ͷ���Ϣ
 *	�������	����
 *	����ֵ˵��������Ϣ��������󷵻�0
 *	����˵��	��
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
    /*�������Ϣ�ֶ�*/
    nSegNum = APPSMS_GetSmsSegments(javaSms->msgType,javaSms->msgBufferLen,hasPort);
    javacall_printf("[appsms_Core][APPSMS_SendSms]>>APPSMS_GetSmsSegments  nSegNum=%d",nSegNum);
    
    /*����ֶ���Ϊ0�����ش���*/
    if(nSegNum == 0){
        return 0;
    }
    /*������Ϣ̫��*/
    if(nSegNum > APPSMS_MAX_SMS_NUM){
        return 0;
    }
    /*  Ϊ������Ϣ���ɲο���  */
    if(nSegNum>1){
        ref = APPSMS_GenReference();
    }
    /*���ɶ���Ϣ���*/
    nHandle = APPSMS_GenHandle();

    /*  �Ѷ���Ϣ��ֲ����浽�������б��� */
     /* ������ʧ�ܣ�ɾ���Ѿ���ֵĶ���Ϣ�����ش��� */
     /* �����ֳɹ������������Ϣ���浽�������б��� */
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
    /*�ж��������Ϣ����ʱ���Ƿ�Ϊ5����֮ǰ*/
    /*����������Ϣ����ʱ��Ϊ5����֮ǰ,����Ϊ��һ������Ϣ�ķ��ͽ����ʧ*/
    /*���·��Ͷ���Ϣ*/
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
                     g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling �޸��������ʱ��Ϊ��ǰʱ��
                    APPSMS_SetNetState(1);
                    return nHandle;
                }
            }
        }
    }else{
        /* ����ͬ���ź��� */
        /* �������ʧ�ܣ��������� */
        /* �������ɹ������������״̬��ѡ���Ƿ��Ͷ���Ϣ */
        
        bTemp =  APPSMS_GetMutex(1000);
        if(bTemp == 0){
            APPSMS_PutMutex();
            return nHandle;
        }else{
            bNetState = APPSMS_CheckNetState();
            APPSMS_PutMutex();
        }
        

        /* ��������״̬Ϊæ���������ض���Ϣ��� */
        /* ��������״̬Ϊ�У���ȡ�������Ͷ���Ϣ�б��еĵ�һ������Ϣ */
        /* �Զ���Ϣ���б��뷢�� */
        /* �������ʧ�ܣ�ɾ������������Ϣ�������������Ϣ�����ش��� */
        /* ������ͳɹ���������״̬��Ϊæ�����ض���Ϣ��� */
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
                         g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling �޸��������ʱ��Ϊ��ǰʱ��
                        APPSMS_SetNetState(1);
                        return nHandle;
                    }
                }
            }
        }
    }
}

/*****************************************************************************
 *	������		��APPSMS_getSmsSegments
 *	����		       �����㳤����Ϣ�ķֶ���
 *	�������	��msgType����Ϣ�������ͣ�msgBufferLen����Ϣ���ݳ��ȣ�
 *                                     hasPort���޶˿ں�
 *	�������	����
 *	����ֵ˵�����ֶ���
 *	����˵��	��
 *****************************************************************************/
int32 APPSMS_GetSmsSegments (  T_APPSMS_Encoding   msgType,
                             int32              msgBufferLen,
							 BOOLEAN    hasPort){
   javacall_printf("---Core APPSMS_GetSmsSegments msgType:%d",msgType);
    /*�ж˿ں�*/
    if(hasPort == TRUE){
        /*7-bit����*/
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
        
        /*8-bit����*/
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
        
        /*16-bit����*/
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
        
        /*δ֪����*/
        else
        {
            return 0;
        }
        	
    }
    /*�޶˿ں�*/
    else{
        /*7-bit����*/
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
        
        /*8-bit����*/
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
        
        /*16-bit����*/
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
        
        /*δ֪����*/
        else{
            return 0;
        }
    }
}

/*****************************************************************************
 *	������		��APPSMS_MsgSegment
 *	����		       ���۷ֶ���Ϣ
 *	�������	��msg������Ϣ��pos����ȡ���������Ϣλ��
 *                                      max������Ϣ�ֶ�����hasPort���޶˿�
 *	�������	��sendSms��ȡ�����������Ϣ
 *	����ֵ˵�����ɹ�����ʧ��
 *	����˵��	��
 *****************************************************************************/
BOOLEAN APPSMS_MsgSegment(const T_APPSMS_MsgForJava *msg , uint8 pos ,
                     uint8 max,uint8 ref,T_APPSMS_MsgForList *sendSms,BOOLEAN hasPort){
    int32 nStartPos;
    int32 nEndPos;

    /* �����������Ϣ�ڳ�����Ϣ�е���ʼλ�úͽ���λ��
    * �ͼ��㳤����Ϣ�ķֶ������� */
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
        
    /* ��ȡ�������Ϣ */
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
 *	������		��APPSMS_GenHandle
 *	����		       �����ɶ���Ϣ���
 *	�������	����
 *	�������	����
 *	����ֵ˵�������
 *	����˵��	��
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
 *	������		��APPSMS_GenReference
 *	����		       �����ɳ�����Ϣ�ο���
 *	�������	����
 *	�������	����
 *	����ֵ˵�����ο���
 *	����˵��	��
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
 *	������		��APPSMS_EncodeSms
 *	����		       ���Դ����Ͷ���Ϣ���б���
 *	�������	��msg����ǰ����Ϣ
 *	�������	��sendSms��������Ϣ
 *	����ֵ˵�����ɹ�����ʧ��
 *	����˵��	����Ϊ����Ϣ��16-bit�Ķ˿ں�
 *****************************************************************************/
BOOLEAN APPSMS_EncodeSms(const T_APPSMS_MsgForList *msg,T_APPSMS_MsgForNet *sendSms){
    int32 nStartPos=0;
    int32 i=0;
    
    //SCI_MEMCPY(sendSms->address,msg->address,APPSMS_MAX_ADD_LEN);
    strcpy((char*)sendSms->address,(char*)msg->address);
    sendSms->msgType = msg->msgType;
    sendSms->timeStamp = msg->timeStamp;
    SCI_MEMSET(sendSms->msgBuffer,'\0',APPSMS_8BIT_SMS_LEN);

    /*����̶�ʹ��16-bit�Ķ˿ں�*/
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
        /*�ǳ�����Ϣ*/
        if(msg->max==1){
            nStartPos = 1;
        }
        /*������Ϣ*/
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
        /*�ǳ�����Ϣ*/
        if(msg->max==1){
            /*UDHL*/
            sendSms->msgBuffer[1] = 6;
            nStartPos = 8;
        }
        /*������Ϣ*/
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

	/*�����������Ϣ����*/
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

    /*  7-bit ���룬��Ҫ��������  */
    /*  8-bit ��16-bit��ֱ���ڴ濽��  */
    if(msg->msgType == APPSMS_SMSENCODE_7_BIT){
        nStartPos--; /*  ��һ���ֽ�UDL  ���������  */
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
 *	������		��APPSMS_DecodeSms
 *	����		       ���Խ��յĶ���Ϣ���н���
 *	�������	��recvSms����ǰ����Ϣ
 *	�������	��msg��������Ϣ
 *	����ֵ˵�����ɹ�����ʧ��
 *	����˵��	����Ϊ����Ϣһ�����û�����ͷ
 *****************************************************************************/
BOOLEAN APPSMS_DecodeSms(const T_APPSMS_MsgForNet *recvSms, T_APPSMS_MsgForList *msg){
    uint8 nUDHL = 0;
    uint8 nIEI;
    uint8 nIEDL;
    uint8 n7BitUDHL=0;
    int32 i;
    
    /*���ö���Ϣ��Ĭ��ֵ*/
    msg->destPort = 0;
    msg->sourcePort = 0;
    msg->max = 1;
    msg->ref = 1;
    msg->seq = 1;

    /*����Ϣ���û����ݣ����ش���*/
    if(recvSms->msgBufferLen < 1){
        return FALSE;
    }
    /*���û�����ͷ�����ش���*/
    nUDHL = recvSms->msgBuffer[1];
    if(nUDHL < 6){
        return FALSE;
    }

   msg->timeStamp = recvSms->timeStamp;
   msg->msgType = recvSms->msgType;
   //SCI_MEMCPY(msg->address , recvSms->address , APPSMS_MAX_ADD_LEN);
   strcpy((char*)msg->address, (char*)recvSms->address);

    /*�����û�����ͷ*/
    for(i=2;i<nUDHL+1;){
        nIEI = recvSms->msgBuffer[i];
        nIEDL = recvSms->msgBuffer[i+1];

        /*����IEI*/
        switch(nIEI){
            case 0:  /*������Ϣ*/
            	msg->ref = recvSms->msgBuffer[i+2];
            	msg->max = recvSms->msgBuffer[i+3];
            	msg->seq = recvSms->msgBuffer[i+4];
            	break;
            case 4:  /*8-bit �Ķ˿ں�*/
            	msg->sourcePort = recvSms->msgBuffer[i+3];
            	msg->destPort = recvSms->msgBuffer[i+2];
            	break;
            case 5:  /*16-bit �Ķ˿ں�*/
            	msg->destPort = recvSms->msgBuffer[i+2]*256+recvSms->msgBuffer[i+3];
            	msg->sourcePort = recvSms->msgBuffer[i+4]*256+recvSms->msgBuffer[i+5];
            	break;
            case 8:  /*16-bit ref �ĳ�����Ϣ*/
            	msg->ref = recvSms->msgBuffer[i+2]*256+recvSms->msgBuffer[i+3];
            	msg->max = recvSms->msgBuffer[i+4];
            	msg->seq = recvSms->msgBuffer[i+5];
            	break;
            default :
            	break;
        }        
        /*i ָ����һ��IEI*/
        i=i+2+nIEDL;
    }

    /*������Ч����*/
    /*����7-bit �������ͣ���Ҫ����Ľ���*/
    /*8-bit ��16-bit ֱ���ڴ濽��*/
    if(recvSms->msgType == APPSMS_SMSENCODE_7_BIT){
        //modify by zhouyang 2008-08-10 �ͽ������Ӧ
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
 *	������		��APPSMS_ReceiveSendResult
 *	����		       ���������Ϣ���ͽ��
 *	�������	��result������Ϣ���ͽ��
 *	�������	����
 *	����ֵ˵������
 *	����˵��	����ο���������ͼ�����������֧�����з�֧��������Ϣ
 *                                     û��ȫ��������ɡ��е�ı䣬�����˼��������һ������Ϣ
 *                                     ҵ����˵��һ������һ�������Ͷ���Ϣ�������ټ��һ����Ҫ
 *                                     ��Ϊ�˹�����룬Ҳ������ߴ�����ݴ�����
 *****************************************************************************/
void APPSMS_ReceiveSendResult(T_APPSMS_SendResult result)
{
    BOOLEAN bTemp;
    T_APPSMS_MsgForNet tNetSms;
    
    /*��ȡ������͵Ķ���Ϣ*/
    T_APPSMS_MsgForList * ptSendSms;
    int32 nHandle;
	
    bTemp = APPSMS_GetFirstSendSms(&ptSendSms,  &nHandle);
    if(bTemp == FALSE){
        return;
    }

    /*�жϳ�����Ϣ�Ƿ�ȫ��������ɻ���Ϊ�ǳ�����Ϣ*/
    if(ptSendSms->max == ptSendSms->seq){
        APPSMS_SendResultToVM(result , nHandle);
        APPSMS_DelFirstSendSms();
        while(1){/*lint !e716*/
            bTemp =  APPSMS_GetMutex(1000);

            if(bTemp == FALSE){
                APPSMS_PutMutex();
                return;
            }else{

                /*��ȡ��һ�������Ͷ���Ϣ�����û�У�����FALSE*/
                bTemp = APPSMS_GetFirstSendSms(&ptSendSms,  &nHandle);
                if(bTemp == FALSE){
                    APPSMS_SetNetState(FALSE);
                    APPSMS_PutMutex();
                    return;
                }else  /*������һ������Ϣ*/{
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
                    	    g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling �޸��������ʱ��Ϊ��ǰʱ��
                    	     return;
                    	  }
                    }
                }
            }
        }
    }else{
        if(result == APPSMS_SMSSEND_SUCCESS)  /*���ͳɹ�*/{
            APPSMS_DelFirstSendSms();
            while(1){/*lint !e716*/
                bTemp =  APPSMS_GetMutex(1000);
                if(bTemp == FALSE){
                    APPSMS_PutMutex();
                    return;
                }else{
                    /*��ȡ��һ�������Ͷ���Ϣ�����û�У�����FALSE*/
                    bTemp = APPSMS_GetFirstSendSms(&ptSendSms,  &nHandle);
                    if(bTemp == FALSE){
                        APPSMS_SetNetState(FALSE);
                        APPSMS_PutMutex();
                        return;
                    }else{
                        /*������һ������Ϣ*/
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
                    	           g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling �޸��������ʱ��Ϊ��ǰʱ��
                    	           return;
                    	      }
                        }
                    }
                }
            }
        }
        else    /*����ʧ��*/
        {
            APPSMS_SendResultToVM(result , nHandle);
            APPSMS_DelSendSmsByHandle(nHandle);
            while(1){/*lint !e716*/
                bTemp =  APPSMS_GetMutex(1000);
                if(bTemp == FALSE){
                    APPSMS_PutMutex();
                    return;
                }else{
                    /*��ȡ��һ�������Ͷ���Ϣ�����û�У�����FALSE*/
                    bTemp = APPSMS_GetFirstSendSms(&ptSendSms,  &nHandle);
                    if(bTemp == FALSE){ 
                        APPSMS_SetNetState(FALSE);
                        APPSMS_PutMutex();
                        return;
                    }else{
                        /*������һ������Ϣ*/
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
                    	           g_NewSendTime = APPSMS_GetCurrentTime(); //suozhiling �޸��������ʱ��Ϊ��ǰʱ��
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
 *	������		��APPSMS_ReceiveRecvSms
 *	����		       ��������յ��Ķ���Ϣ
 *	�������	��recvSms�����յ��Ķ���Ϣ
 *	�������	����
 *	����ֵ˵����0��ʾ����Java����Ϣ����0��ʾ��Java����Ϣ
 *	����˵��	����
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
    /*���ΪWAP PUSH ����Ϣ������˿ں���Ŀ���б���*/
    if(tListSms.destPort == APPSMS_PORT_2948 || tListSms.destPort == APPSMS_PORT_2949 
        || APPSMS_ExistPortNum(tListSms.destPort)){
        /*������Ϣ*/
        if(tListSms.max > 1){
            bTemp = APPSMS_CheckLongRecvSms(&tListSms);
            /*������Ϣ����δ���*/
            if(bTemp == FALSE){
                APPSMS_DelRecvSmsByDate();
                bTemp = APPSMS_AddRecvSmsToList(&tListSms);
                if(bTemp == FALSE){
                    ;/*��������*/
                }
                return;
            }
            else  /*������Ϣ������ɣ����ӳ�����Ϣ�������б���ɾ��*/
            {
                 APPSMS_MsgConcatenation(&tListSms, &tJavaSms);
                 APPSMS_DelRecvSmsByRef(&tListSms);
            }
        }
        else  /*�ǳ�����Ϣ���˴��Ǵ��ӣ������Ǹı����ݽṹ*/
        {
            APPSMS_MsgConcatenation(&tListSms, &tJavaSms);
        }

        /*����WAP PUSH ����Ϣ*/
        if(tListSms.destPort == APPSMS_PORT_2948 || tListSms.destPort == APPSMS_PORT_2949){
/*   
            //P900 �еĴ���
            g_tPushConcatenateContext.wDestinationPort = tJavaSms.destPort;
	        g_tPushConcatenateContext.wOriginatorPort = tJavaSms.sourcePort;
            g_tPushConcatenateContext.wPushDataLen = tJavaSms.msgBufferLen;
            SCI_MEMCPY(g_tPushConcatenateContext.abPushData,tJavaSms.msgBuffer,tJavaSms.msgBufferLen);
            mmiPushEntry(0,NULL);
*/
            //javacall_printf("-------appsms_Core.APPSMS_ReceiveRecvSms().APPSMS_PORT_2948-------");
            return;
        }
        else /*��Java ������Ͷ���Ϣ*/
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
 *	������		��APPSMS_CheckLongRecvSms
 *	����		       �����ָ������Ϣ���ڵĳ�����Ϣ�Ƿ�ȫ���������
 *	�������	�����ն���Ϣ
 *	�������	����
 *	����ֵ˵����TRUE Ϊ��ɣ�FALSE Ϊδ���
 *	����˵��	��
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
 *	������		��APPSMS_AddRecvSmsToList
 *	����		       ����ӽ��ն���Ϣ��������
 *	�������	��Ҫ��ӵĶ���Ϣ
 *	�������	����
 *	����ֵ˵����FALSE Ϊ��������
 *	����˵��	��
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
 *	������		��APPSMS_MsgConcatenation
 *	����		       �����ⳤ����Ϣ
 *	�������	��recvSmsҪ���ӵĶ���Ϣ
 *	�������	��msg���Ӻ�Ķ���Ϣ
 *	����ֵ˵��  ����
 *	����˵��	����Ҫ���ӵĶ���ϢΪ�ǳ�����Ϣʱ��
 *                �书�ܽ��������ݽṹת��
 *****************************************************************************/
void APPSMS_MsgConcatenation (const T_APPSMS_MsgForList *recvSms,T_APPSMS_MsgForJava *msg){
     /* ����ͬһ��������Ϣ��ÿ���������Ϣ����Ч�������ݳ���
     * Ӧ������ͬ�ģ����һ�����⣩*/
    int32 nSegmentLen = 0; /* ÿһ�������Ϣ����Ч�������ݳ��� */
    int32 i =0;

    SCI_MEMSET(msg,0,sizeof(T_APPSMS_MsgForJava));
    //SCI_MEMCPY(msg->address , recvSms->address,APPSMS_MAX_ADD_LEN);
    strcpy((char*)msg->address, (char*)recvSms->address);
    msg->destPort = recvSms->destPort;
    msg->sourcePort = recvSms->sourcePort;
    msg->msgType = recvSms->msgType;
    msg->timeStamp = recvSms->timeStamp;
    msg->msgBufferLen = 0;

    /* ������Ϣ */
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
    /*  ���ڷǳ�����Ϣ��(recvSms.seq-1)XnSegmentLen Ϊ0X0 */
    SCI_MEMCPY(msg->msgBuffer+(recvSms->seq-1)*nSegmentLen,recvSms->msgBuffer,recvSms->msgBufferLen);
    msg->msgBufferLen = msg->msgBufferLen + recvSms->msgBufferLen;

    return;
}

/*****************************************************************************
 *	������		��APPSMS_DelRecvSmsByRef
 *	����		       ��ɾ������Ϣ�б�����ָ���Ķ���ϢΪͬһ��
 *                                ������Ϣ�����ж���Ϣ
 *	�������	��recvSms ָ���Ľ��յ��Ķ���Ϣ
 *	�������	����
 *	����ֵ˵������
 *	����˵��	��
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
 *	������		��APPSMS_DelRecvSmsByDate
 *	����		       ��ɾ�����ն���Ϣ����������ǰ�Ķ���Ϣ
 *	�������	����
 *	�������	����
 *	����ֵ˵������
 *	����˵��	��
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
 *	������		��APPSMS_Set7BitBuff
 *	����		       �����ô������е�ĳ��7-bit ����
 *	�������	��buff 7-bit ���뷽ʽ�µ����� �����ռ䣬
 *                                     buffLen 7-bit ���ݴ����ռ�Ĵ�С(byte ��)
 *                                     pos septet �����кţ���0 ��ʼ�������10 ��septet
 *                                     data7Bit  ��Ҫ�����7-bit ���ݣ�ASCII ��
 *	�������	����
 *	����ֵ˵������
 *	����˵��	��
 *****************************************************************************/
static BOOLEAN APPSMS_Set7BitBuff(uint8* buff , int32 buffLen , int32 pos , int8 data7Bit){
    /*��data7Bit ������7-bit ���뷽ʽ���浽buff �У�
    *�����ܻ������byte��Ҳ����ֻһ��byte*/
    int32 nFirstBytePos;    /*��һ��byte λ��*/
    int32 nSecondBytePos;    /*�ڶ���byte λ��*/
    
    uint8 nFirstBitNum;    /*�ڵ�һ��byte �б������λ��*/
    uint8 nSecondBitNum;    /*�ڵڶ���byte �б������λ��*/

    /*test*/
    int32 nTest;
    /*test*/
    BOOLEAN bTemp;

    nSecondBitNum = (pos+1)*7%8;
    nFirstBitNum = 7 - (pos+1)*7%8;

    /*��ΪnFirstBytePos  ��0  ��ʼ�����н����Ҫ-1 ��pos  ��Ҫ+1*/
    nFirstBytePos = (pos+1)*7/8-1;    
    nSecondBytePos = nFirstBytePos+1;

    /*test*/
    nTest = buff[nFirstBytePos];
    nTest = buff[nSecondBytePos];
    nTest = data7Bit;
    /*test*/

    /*�����һ��byte*/
    if( nFirstBitNum != 0 ){
        /*����Ӧ��λ��0��ǰnFirstBytePosλ*/
        bTemp = 0xFF;
        bTemp = bTemp>>nFirstBitNum;
        buff[nFirstBytePos]=buff[nFirstBytePos]&bTemp;

        /*������Ӧ��λ*/
        /*nFirstBytePos+nSecondBitNum=7, bTemp�ǰ�λ*/
        /*������nFirstBytePosλ*/
        bTemp=data7Bit<<nSecondBitNum+1;/*����bTemp ǰnSecondBitNum+1 λ��Ϊ0*/   /*lint !e504*/     
        buff[nFirstBytePos]=buff[nFirstBytePos]|bTemp;
    }
    /*����ڶ���byte*/
    if(nSecondBitNum != 0){
         /*����Ӧ��λ��0����nSecondBytePosλ*/
        bTemp = 0xFF;
        bTemp = bTemp<<nSecondBitNum;       /*lint !e504*/
        buff[nSecondBytePos]=buff[nSecondBytePos]&bTemp;

        /*������Ӧ��λ*/
        bTemp = data7Bit>>nFirstBitNum;/*char ���͵�һλ�����õ�λ����Ҫ����*/
        buff[nSecondBytePos]=buff[nSecondBytePos]|bTemp;
    }
    /*test*/
    nTest = buff[nFirstBytePos];
    nTest = buff[nSecondBytePos];
    /*test*/
    return TRUE;
}

/*****************************************************************************
 *	������		��APPSMS_Get7BitBuff
 *	����		       ����ȡ�������е�ĳ��7-bit ����
 *	�������	��buff 7-bit ���뷽ʽ�µ����� ��
 *                                     buffLen 7-bit ���ݴ����ռ�Ĵ�С(byte ��)
 *                                     pos septet �����кţ���0 ��ʼ�������10 ��septet
 *                                     data7Bit  �û��������ȡ����7-bit ���ݣ�ASCII ��
 *	�������	����
 *	����ֵ˵������
 *	����˵��	�����Բο�����APPSMS_Set7BitBuff
 *****************************************************************************/
static BOOLEAN APPSMS_Get7BitBuff(const uint8* buff , int32 buffLen , int32 pos , int8* data7Bit){
    /*��data7Bit ������7-bit ���뷽ʽ���浽buff �У�
    *�����ܻ������byte��Ҳ����ֻһ��byte*/
    int32 nFirstBytePos;    /*��һ��byte λ��*/
    int32 nSecondBytePos;    /*�ڶ���byte λ��*/
    
    uint8 nFirstBitNum;    /*�ڵ�һ��byte �б������λ��*/
    uint8 nSecondBitNum;    /*�ڵڶ���byte �б������λ��*/

    BOOLEAN bTemp;

    nSecondBitNum = (pos+1)*7%8;
    nFirstBitNum = 7 - (pos+1)*7%8;

    /*��ΪnFirstBytePos  ��0  ��ʼ�����н����Ҫ-1 ��pos  ��Ҫ+1*/
    nFirstBytePos = (pos+1)*7/8-1;
    
    nSecondBytePos = nFirstBytePos+1;

    *data7Bit = 0;
    /*��ȡ��һ��byte*/
    if( nFirstBitNum != 0 ){
        /*������Ӧ��λ*/
        bTemp=buff[nFirstBytePos]>>nSecondBitNum+1;    /*lint !e504*/  
        *data7Bit = *data7Bit|bTemp;
    }
    /*����ڶ���byte*/
    if(nSecondBitNum != 0){
        /*������Ӧ��λ*/
        bTemp = buff[nSecondBytePos]<<nFirstBitNum;
        *data7Bit=*data7Bit|bTemp;
    }

    /*�����Ҫ�ѵ�һλȥ��*/
    
    *data7Bit = *data7Bit & 0x7F;/*0x7F:0111 1111*/
    return TRUE;
}

/*****************************************************************************
 *	������		��APPSMS_ConvertTime2Long
 *	����		       ���������յ����ڸ�ʽת���ɵ�2000 ��1 ��1 �յ�����
 *	�������	��year�꣬month�£�day�գ�hourʱ��minute�֣�second��
 *	�������	����2000��1��1��0��0��0�������
 *	����ֵ˵������
 *	����˵��	����������ʱ��û�п���100  ��400  ����
 *****************************************************************************/
int32 APPSMS_ConvertTime2Long(uint8 year , uint8 month , uint8 day , uint8 hour , uint8 minute , uint8 second)
{
    /*һ��86400  ��*/
    /*һ����(365)  ��31536000*/
    /*һ����(366)  ��31622400*/
    /*������ ��126230400*/
    /*һ����(28)  ��2419200*/
    /*һ����(29)  ��2505600*/
    /*һ����(30)  ��2592000*/
    /*һ����(31)  ��2678400*/
    /*һСʱ3600  ��*/
    /*һ����60  ��*/
    int32 lTime=0;
    /*����������ݵ���Ч�ԣ�day  ��鲻�Ǻ�ȫ��*/
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
    /*������*/
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
    /*������*/
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

    /*������*/
    lTime  = lTime + (day-1)*86400;

    /*����ʱ*/
    lTime  = lTime + hour*3600;

    /*�����*/
    lTime  = lTime + minute*60;

    /*������*/
    lTime  = lTime + second;

    return lTime;
}
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


