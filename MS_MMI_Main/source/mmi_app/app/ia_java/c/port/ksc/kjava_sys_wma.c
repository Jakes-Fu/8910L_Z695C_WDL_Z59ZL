/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#include "jblend_config.h"
#ifdef JAVA_SUPPORT_IA
#ifdef JAVA_SUPPORT_SHRINK_RAM
#include "kjava_shrink_ram.h"
#endif
#include "keypad.h"
#include "dal_keypad.h"
#include "jwe_log.h"
#include "kjava_sys_wma.h"
#include "mmisms_export.h"
#include "mmijava_export.h"
#include "java_id.h"
#include "mmisms_app.h"
#include "mmijava_internal.h"
#include "kjava_sys_push.h"

#include "ajutil.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MAX_KJAVA_WMA_ADDRESS_LENGTH    128
#define MAX_WMA_CONN_NUM                8

/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct WmaConnectionTag
{
/*
* 1 This connection is occupied
* 0 This connection is not occupied
    */
    BOOLEAN  isOccupied;
    
    /* 
       KJAVA_WMA_CONNECTION_MODE_SERVER or KJAVA_WMA_CONNECTION_MODE_CLIENT 
       if mode == KJAVA_WMA_CONNECTION_MODE_SERVER, no address but port, e.g. sms://:3381
    */
    int32   mode; 
    /* Connection Protocol */
    int32 protocol;
    /* sms address and length of address*/  
    unsigned char  address[MAX_KJAVA_WMA_ADDRESS_LENGTH];   
    /* sms port */
    int32   port; 
    /* 
    inMessage stores number of incomming message. When there's incomming
       message, MMI will call ajms_wma_postEvent() and inMessage should 
       increases by 1.
    */
    int32  inMessage;
    KJavaWmaHandle handle;
    KJavaWmaPostEvent callback;            
} WmaConnection;

typedef struct _smsRecvQueue
{
    int isOccupied;
    int port;
    KJavaWmaMessage recvMessage;
}smsRecvQueue;

PUBLIC void MMIAPIJAVA_StartWMAReceiveTimer(void);
PUBLIC void MMIAPIJAVA_StopWMAReceiveTimer(void);
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifndef JAVA_SUPPORT_SHRINK_RAM
static WmaConnection s_connList[MAX_WMA_CONN_NUM] = {0};
static smsRecvQueue recvQue[MAX_WMA_CONN_NUM];
#else
DEFINE_STATIC_SHRINK_VAR_1_DIM(WmaConnection,s_connList)
DEFINE_STATIC_SHRINK_VAR_1_DIM(smsRecvQueue,recvQue)
#endif
uint8 g_java_sms_timer_id = 0;
#ifdef NCI_TCK
int32 isWmaOpen=0;
#endif

LOCAL MMISMS_JAVA_GLOBAL_T     g_mmisms_javadata = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/***************************************************************************/
// Description:AppsmsCallback
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
static/*LOCAL*/ void AppsmsCallback(int32 event, SMS_Message *argv);
/***************************************************************************/
// Description:getUrl
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
static/*LOCAL*/ void getUrl(uint8 *phonenum,  int32 urlLength, uint8 *url);
LOCAL void init_sms_msg_queue(void);
static void dumpbuf_wma(void *buf, int len);

/*****************************************************************************/
//  Description : ia java send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IAJavaSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm);

#ifdef JAVA_SUPPORT_SHRINK_RAM
BEGIN_DEFINE_SHRINK_VAR_INIT(kjava_sys_wma)
	SHRINK_VAR_MEMORY_1_DIM(WmaConnection,s_connList,MAX_WMA_CONN_NUM)
	SHRINK_VAR_MEMORY_1_DIM(smsRecvQueue,recvQue,MAX_WMA_CONN_NUM)
END_DEFINE_SHRINK_VAR_INIT	

BEGIN_DEFINE_SHRINK_VAR_FINI(kjava_sys_wma)
	FREE_SHRINK_VAR_MEMORY(s_connList)
	FREE_SHRINK_VAR_MEMORY(recvQue)
END_DEFINE_SHRINK_VAR_FINI
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/**
* \brief  To open a sms connection                                  -+
*
* \param  [In]connection
*         Connection information
* \param  [Out]handle
*          Handler for the sms connection to be opened
* \param  [In]callback
*          Callback function for MMI/SMS thread to notify event
* \return
*     Return  KJAVA_WMA_OK if the operation succeeds.
*     In case of failure, return one of the following error codes.
*     KJAVA_WMA_ERROR_PROTOCOL_DOESNOT_EXIST
*           The protocol designated in the URL is not supported.
*     KJAVA_WMA_ERROR_CONNECTION_CANT_CREATE
*           The port number designated in the URL is in use or reserved.
*     KJAVA_WMA_ERROR_IO_ERROR
*           An I/O error occurred.
*/

int kjava_wma_open (KJavaWmaConnection* request, KJavaWmaHandle* handle, KJavaWmaPostEvent callback)
{
     
    int32 i = 0;    
    KJavaWmaHandle  hanlde_ptr = PNULL;
    
    JWE_LOG(kjava_wma_open, ("kjava_wma_open"))
    if(request == NULL)
    {
        JWE_LOG(kjava_wma_open, ("request is NULL"))
        return KJAVA_WMA_ERROR_CONNECTION_CANT_CREATE;
    }
    
    if(request->mode != KJAVA_WMA_CONNECTION_MODE_SERVER 
        && request->mode != KJAVA_WMA_CONNECTION_MODE_CLIENT)
    {
        JWE_LOG(kjava_wma_open, ("request->mode is error"))
        return KJAVA_WMA_ERROR_NOT_PERMITTED;
    }
    
    
    if(request->protocol != KJAVA_WMA_PROTOCOL_SMS
        && request->protocol != KJAVA_WMA_PROTOCOL_CBS)
    {
        JWE_LOG(kjava_wma_open, ("request->protocol is error"))
         return KJAVA_WMA_ERROR_PROTOCOL_DOESNOT_EXIST;
    }
    
    /* search if the port is opened */
    if(request->mode == KJAVA_WMA_CONNECTION_MODE_SERVER)
    {
        for(i=0; i<MAX_WMA_CONN_NUM; i++)
        {
            if(s_connList[i].isOccupied == 1 && s_connList[i].mode == KJAVA_WMA_CONNECTION_MODE_SERVER 
                && s_connList[i].port == request->port )
            {   
                JWE_LOG(kjava_wma_open, ("The port %d is opened by other MIDlet.", request->port))
                return KJAVA_WMA_ERROR_CONNECTION_CANT_CREATE;
            }
        }
    }
    //SCI_TRACE_LOW:"kjava_wma_open port = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_166_112_2_18_2_23_29_402,(uint8*)"d",request->port);
    /* get available connection */
    for(i=0; i<MAX_WMA_CONN_NUM; i++)
    {
        if(s_connList[i].isOccupied == 0)
        {
            break;
        }
    }
    
    if(i >= MAX_WMA_CONN_NUM)
    {
        return KJAVA_WMA_ERROR_CONNECTION_CANT_CREATE;              
    }

    JWE_LOG(kjava_wma_open, ("request->port %d", request->port))
    if(request->port < 0)
    {
        request->port = 0;
    }

    if(MMIJAVA_getConnByPort(request->port) == -1)
	{
        //SCI_TRACE_LOW:"kjava_wma_open gen port true"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_189_112_2_18_2_23_29_403,(uint8*)"");
        MMIJAVA_RegSMSPortCallback(request->port,(SMS_CALLBACK)AppsmsCallback);
    }
    else
    {
	  JWE_LOG(kjava_wma_open, ("kjava_wma_open gen port false"))
    }

    hanlde_ptr = (KJavaWmaHandle)SCI_ALLOCA(sizeof(KJavaWmaHandle));
    if(PNULL == hanlde_ptr)
    {
        return KJAVA_WMA_ERROR_CONNECTION_CANT_CREATE;
    }
    
    SCI_MEMSET(hanlde_ptr,0,sizeof(KJavaWmaHandle));
    *handle = hanlde_ptr;
    
    s_connList[i].mode = request->mode;
    s_connList[i].protocol = request->protocol;
    s_connList[i].port =request->port;
    s_connList[i].isOccupied = TRUE;      
    s_connList[i].handle = *handle;
    SCI_MEMCPY(s_connList[i].address, request->address, request->addressLength);
    *(s_connList[i].address+request->addressLength) = '\0';
    s_connList[i].callback = callback;      
#ifdef NCI_TCK
    isWmaOpen = 1;
#endif   
    if(g_java_sms_timer_id == 0 && request->mode == KJAVA_WMA_CONNECTION_MODE_SERVER)//fix wma push case by pan.yang
    {
        JWE_LOG(kjava_wma_open, ("send signal to start receive timer i = %d", i))
        MMIAPIJAVA_StartWMAReceiveTimer();
    }
    /* send back result */
   return KJAVA_WMA_OK;    
 
}


/**
* \brief  To close the designated sms connection 
*       
* \param [In]handle
*       The connection identity got from kjava_wma_open()
* \return KJAVA_WMA_OK:             successed to close a sms connection
*          KJAVA_WMA_ERROR_IO_ERROR:    failed to close a sms connection
*/
int  kjava_wma_close (KJavaWmaHandle handle)
{
    
    int32 port = 0;
    int32 i = 0;   
    JWE_LOG(kjava_wma_close, ("kjava_wma_close()"))   
    
    if(handle == NULL)
    {
        JWE_LOG(kjava_wma_close, ("handle == NULL"))
		return KJAVA_WMA_ERROR_IO_ERROR;   
    }
    JWE_LOG(kjava_wma_close, ("handle = 0x%08X", handle))
#ifdef NCI_TCK
	isWmaOpen = 0;
#endif   
    for (i = 0; i < MAX_WMA_CONN_NUM; i++)
    {
        if (s_connList[i].handle == handle)
        {
            port = s_connList[i].port;
            s_connList[i].isOccupied = FALSE;
            break;
        }       
    }
    if(MMIJAVA_getConnByPort(port) == -1)
	{
        MMIJAVA_UnRegSMSPortCallback(port);
    }
    for (i = 0; i < MAX_WMA_CONN_NUM; i++)
    {
        JWE_LOG(kjava_wma_close, ("s_connList[%d] isOccupied:%d, mode:%d", i, s_connList[i].isOccupied,  s_connList[i].mode))
        if (s_connList[i].isOccupied == 1
            && s_connList[i].mode == KJAVA_WMA_CONNECTION_MODE_SERVER)//fix wma push case by pan.yang
        {
            break;
        }
    }

    if(i == MAX_WMA_CONN_NUM)//fix wma push case by pan.yang
    {
        JWE_LOG(kjava_wma_close, ("Stop WMA receive timer"))
        MMIAPIJAVA_StopWMAReceiveTimer();
    }  

    SCI_FREE(handle); //释放在kjava_wma_open申请的空间
    
    return KJAVA_WMA_OK;
}

/*****************************************************************************/
//  Description : ia java send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IAJavaSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm)
{//dm 不需要做任何处理，直接结束流程
    BOOLEAN result = TRUE;
    SMS_CALLBACK      java_cb     = PNULL;
    MMISMS_JAVA_GLOBAL_T*   java_global_data_ptr = MMIAPIJAVA_GetSmsJavaData();

    //SCI_TRACE_LOW:"IAJavaSMSSendCnfCallback !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_295_112_2_18_2_23_30_404,(uint8*)"");
    if (MMIAPIJAVA_CheckIfJavaPort(java_global_data_ptr->src_port, &java_cb, TRUE))
    {
        SMS_Message java_data = {0};

        if (PNULL == java_cb)
        {
            //SCI_TRACE_LOW:"java:IAJavaSMSSendCnfCallback java_cb = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_302_112_2_18_2_23_30_405,(uint8*)"");
            return result;
        }
        java_data.srcPort = java_global_data_ptr->src_port;
        java_data.dstPort = java_global_data_ptr->des_port;

        if (is_succss)
        {
            java_cb(JAVA_SENDOK_CNF, (int)&java_data);
        }
        else
        {
            java_cb(JAVA_SENDFAIL_CNF, (int)&java_data);
        }
    }
    else
    {
        result = FALSE;
    }
    
    return result;
}

/**
* \brief Send a message by an opened sms connection
*
* \param [In]connectionId
*        Connection identity got from kjava_wma_open
* \param [In]message
*        Message to be sent
* \return KJAVA_WMA_OK:                 successed to send a message
*          KJAVA_WMA_ERROR_IO_ERROR:        failed to send a message due to I/O error
*          KJAVA_WMA_ERROR_PAYLOAD_OVER:    message exceeds system's layload
*          KJAVA_WMA_ERROR_NOT_PERMITTED    sending is not permitted on the connection
*/  
int  kjava_wma_sendMessage (KJavaWmaHandle handle, KJavaWmaMessage* message)
{  
    AJURLInfoR2 urlInfo = {0};
    AJ_BYTE pcAddr[50] = {0};
    AJ_BYTE *p = PNULL;
    uint8 *sms_data_ptr = PNULL;
    int32 sms_data_len = 0;
    int32 i = 0;
    int32 j = 0;
    BOOLEAN sendRet = FALSE, is_exist_port = TRUE;
    int32 type = 0;

//changed by minghu.mao for sms
    MMISMS_SEND_DATA_T send_data = {0};
    MMI_STRING_T    message_content = {0};
    
    if(handle == NULL || message == NULL)
    {
        JWE_LOG(kjava_wma_sendMessage, ("handle is NULL or message is NULL"))
        return KJAVA_WMA_ERROR_IO_ERROR;
    } 

    for (i = 0; i < MAX_WMA_CONN_NUM; i++)
    {
        if (s_connList[i].handle == handle)
        {            
            break;
        }       
    } 

    //SCI_TRACE_LOW:"kjava_wma_sendMessage i = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_366_112_2_18_2_23_30_406,(uint8*)"d",i);
    if (i < MAX_WMA_CONN_NUM)
    {
        SCI_MEMSET(&urlInfo, 0, sizeof(AJURLInfoR2));
        //SCI_TRACE_LOW:"kjava_wma_sendMessage message->url = %s,message->urlLength = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_370_112_2_18_2_23_30_407,(uint8*)"sd",message->url,message->urlLength);
        if (ajutil_parseURL(message->url, message->urlLength, &urlInfo)<0)
        {        // not a url;
            (s_connList[i].callback)(handle, KJAVA_WMA_EVENT_IO_ERROR, NULL);
             //SCI_TRACE_LOW:"kjava_wma_sendMessage parse url 0"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_374_112_2_18_2_23_30_408,(uint8*)"");
            return KJAVA_WMA_OK;
        }
        SCI_MEMSET(pcAddr,0,50);
        for(j=0, p = urlInfo.hostname; *p!=':'; p++, j++)
        {
            pcAddr[j]=*p;
            if(j == urlInfo.hostnameLength)
            {
                is_exist_port = FALSE;
        	    break;
            }
        }
        pcAddr[j]='\0';
        //SCI_TRACE_LOW:"kjava_wma_sendMessage pcAddr = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_388_112_2_18_2_23_30_409,(uint8*)"s", pcAddr);
	    if(message->type == KJAVA_WMA_MESSAGE_TYPE_TEXT)
	    {
		    JWE_LOG(sms_makeAndSendMessage, ("send textData"));       
            sms_data_ptr = (uint8*)message->textData;
            sms_data_len = message->dataLength*2;    
            type = MN_SMS_UCS2_ALPHABET;            
	    }
	    else
	    {
		    JWE_LOG(sms_makeAndSendMessage, ("send binaryData"));		   
            sms_data_len = message->dataLength;            
            sms_data_ptr = message->binaryData;
            type = MN_SMS_8_BIT_ALPHBET;
   	    } 
        message_content.wstr_len = message->dataLength;
        message_content.wstr_ptr = SCI_ALLOC_APP((message_content.wstr_len + 1)*2);
        if(PNULL == message_content.wstr_ptr)
        {
            return KJAVA_WMA_ERROR_IO_ERROR;
        }
        SCI_MEMSET(message_content.wstr_ptr, 0, (message_content.wstr_len + 1)*2);
        if(message->type == KJAVA_WMA_MESSAGE_TYPE_TEXT)
        {
            MMI_WSTRNCPY(message_content.wstr_ptr, message_content.wstr_len, message->textData, message_content.wstr_len, message_content.wstr_len);
        }
        else
        {
            MMI_STRNTOWSTR(message_content.wstr_ptr, message_content.wstr_len, message->binaryData, message_content.wstr_len, message_content.wstr_len);
        }

        send_data.dual_sys = MMIJAVA_GetAcitveSim();
        send_data.sms_content_ptr = &message_content;
        send_data.dst_port = s_connList[i].port;
        send_data.src_port = s_connList[i].port;
        send_data.dest_addr_ptr = (uint8*)pcAddr;
        send_data.dest_addr_len = j;
        send_data.is_need_packet = FALSE;
        send_data.send_callback = IAJavaSMSSendCnfCallback;
        
        sendRet = MMIAPISMS_SendSmsForOther(&send_data);
        
        //SCI_TRACE_LOW:"sms_makeAndSendMessage s_connList[i].port = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_430_112_2_18_2_23_30_410,(uint8*)"d",s_connList[i].port);
        //SCI_TRACE_LOW:"kjava_wma_sendMessage sendRet = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_431_112_2_18_2_23_30_411,(uint8*)"d",sendRet);
	    if(!sendRet)
	    {
		    (s_connList[i].callback)(handle, KJAVA_WMA_EVENT_IO_ERROR, NULL);
            return KJAVA_WMA_ERROR_IO_ERROR;
            
	    }  
        (s_connList[i].callback)(handle, KJAVA_WMA_EVENT_MESSAGE_SENT, NULL);       
        return KJAVA_WMA_OK;
        
    }
    else
    {
        return KJAVA_WMA_ERROR_IO_ERROR;
    }
   
}


/**
* \brief To query the number of segments of designated message to be sent.
* 
* \param [In]message
*        Message Information
* \return 
*        number of segements
*/
int kjava_wma_numberOfSegments(KJavaWmaMessage* message)
{
    JWE_LOG(kjava_wma_numberOfSegments, ("The message length is %d", message->dataLength));
    
    if(message->type == KJAVA_WMA_MESSAGE_TYPE_BINARY)
    {
        return (((message->dataLength)%140 > 0) ? ((message->dataLength)/140 + 1) : (message->dataLength)/140);
    }
    else
    {
        return (((message->dataLength)%70 > 0) ? ((message->dataLength)/70 + 1) : (message->dataLength)/140);
    }
    
    
}


/**
 * \brief  initialize the connections
 *      
 * @return
 *        N/A.
 */
PUBLIC void MMIJAVA_initConnPool(void)
{
    int32 i = 0;

    for(i = 0; i < MAX_WMA_CONN_NUM; i++)
    {
        SCI_MEMSET(&s_connList[i], 0x00, sizeof(WmaConnection));
    }    
}

/***************************************************************************/
// Description:getUrl
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
LOCAL void getUrl(uint8 *phonenum,  int32 urlLength, uint8 *url)
{
    int32 i = 0;
    uint8 *num = phonenum;
    
    SCI_STRCPY((char*)url, (char*)"sms://+");
    
    for(i = 0; i < urlLength; i++)
    {
        url[i+7] = (num[i] & 0x0f) + '0';
        i++;
        if(i == urlLength -1) 
        {
            break;
        }
        url[i+7] = ((num[i] & 0xf0) >> 4) + '0';
    }   
}

/***************************************************************************/
// Description:AppsmsCallback
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
LOCAL void AppsmsCallback(int32 event, SMS_Message *argv)
{	


    int ret;
    int32 i = 0;
   //  KJavaWmaMessage recvMessage = {0};
    // uint8 *buffer = NULL;
    // uint8 url[40] = {0};
	
	
	for (i = 0; i < MAX_WMA_CONN_NUM; i ++)
	{
        if ((s_connList[i].isOccupied == 1) && (s_connList[i].port == argv->dstPort))
        {
            break;
        }
	}
    //SCI_TRACE_LOW:"AppsmsCallback i = %d, event = %d, argv->dstPort = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_536_112_2_18_2_23_30_412,(uint8*)"ddd",i,event, argv->dstPort);
    if (i >= MAX_WMA_CONN_NUM)
    {
        return;
    }
    
	switch ( event )
	{
	case JAVA_SENDOK_CNF: //Result of send	
		(s_connList[i].callback)(s_connList[i].handle, KJAVA_WMA_EVENT_MESSAGE_SENT, NULL);		
		break;

	case JAVA_NEW_MT_IND:    //receive a sms message
        Ajava_Initiliztion_Special();
		if (s_connList[i].mode == KJAVA_WMA_CONNECTION_MODE_CLIENT)
		{
		    //SCI_TRACE_LOW:"AppsmsCallback mode is KJAVA_WMA_CONNECTION_MODE_CLIENT"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_552_112_2_18_2_23_30_413,(uint8*)"");
        	return;
		}	

        ret = MMIJAVA_Add2SMSReceiveQue(argv);
        if(ret == -1)
        {
            (s_connList[i].callback)(s_connList[i].handle, KJAVA_WMA_EVENT_IO_ERROR, NULL);
        }
		break;
		
	case JAVA_SENDFAIL_CNF:
		(s_connList[i].callback)(s_connList[i].handle, KJAVA_WMA_EVENT_IO_ERROR, NULL);
		break;

	default:
		break;
	 }
	return ;
}
//fix wma push case by pan.yang begin
PUBLIC int MMIJAVA_Add2SMSReceiveQue(SMS_Message *argv)
{
    int i;
    unsigned char url[40] = {0};
    KJavaWmaMessage *recvMessage;

    JWE_LOG(Add2SMSReceiveQue, ("Enter"));

    for(i = 0; i < MAX_WMA_CONN_NUM ; i++)
    {
        if(recvQue[i].isOccupied == 0)
        {
            recvQue[i].isOccupied = 1;
            recvQue[i].port = argv->dstPort;
            JWE_LOG(MMIJAVA_Add2SMSReceiveQue, ("recvQue[%d].port = %d, data_type = %d", i, recvQue[i].port, argv->type));
            recvMessage = &recvQue[i].recvMessage;

            getUrl((unsigned char*)argv->url, (int)argv->urlLength, url);

            recvMessage->urlLength = strlen((char*)url);
            recvMessage->url = SCI_ALLOC_APP(recvMessage->urlLength);
            SCI_MEMCPY(recvMessage->url, url, recvMessage->urlLength);

            recvMessage->isTimeStampAvailable = argv->isTimeStampAvailable;
            recvMessage->timeStampHigh = argv->timeStampHigh;
            recvMessage->timeStampLow = argv->timeStampLow;

            switch(argv->type)
            {
            case MN_SMS_8_BIT_ALPHBET:
                recvMessage->type = KJAVA_WMA_MESSAGE_TYPE_BINARY;
                recvMessage->binaryData = SCI_ALLOC_APP(argv->binaryDataLength);
                recvMessage->dataLength = argv->binaryDataLength;
                SCI_MEMCPY(recvMessage->binaryData, argv->binaryData, argv->binaryDataLength);
                //SCI_TRACE_LOW:"Add2SMSReceiveQue Data type MN_SMS_8_BIT_ALPHBET"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_607_112_2_18_2_23_30_414,(uint8*)"");
                dumpbuf_wma(recvMessage->binaryData, argv->binaryDataLength);
                break;
            case MN_SMS_UCS2_ALPHABET:
            case MN_SMS_DEFAULT_ALPHABET:
                recvMessage->type = KJAVA_WMA_MESSAGE_TYPE_TEXT;
                recvMessage->textData = SCI_ALLOC_APP(argv->textDataLength);
                SCI_MEMCPY(recvMessage->textData, argv->textData, argv->textDataLength);
                recvMessage->dataLength = argv->textDataLength/2;
                //SCI_TRACE_LOW:"Add2SMSReceiveQue Data type MN_SMS_UCS2_ALPHABET"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_616_112_2_18_2_23_30_415,(uint8*)"");
                dumpbuf_wma(recvMessage->textData, argv->textDataLength);
                break;
            default:
                JWE_LOG(Add2SMSReceiveQue, ("Not support type and data_type = %d", argv->type));
                return -1;
            }

            JWE_LOG(JavaAppSMSCallback, ("add to sms receive queue"))
            return i;
        }
    }

    JWE_LOG(JavaAppSMSCallback, ("add to sms receive queue faileds"))
    return -1;
}

void sms_recv_timer(uint8 timer_id, uint32 param)
{
    int i, j;
    KJavaWmaMessage *recvMessage;

    JWE_LOG(sms_recv_timer, ("Enter"))
    for(i = 0; i < MAX_WMA_CONN_NUM ; i++)
    {
    	if(recvQue[i].isOccupied == 1)
    	{
            JWE_LOG(sms_recv_timer, ("recvQue[%d].isOccupied, recvQue[%d].port: %d", i, i, recvQue[i].port))
            recvMessage = &recvQue[i].recvMessage;
            for(j = 0; j < MAX_WMA_CONN_NUM; j++)
            {
                JWE_LOG(sms_recv_timer, ("[%d],isOccupied:%d,port:%d,mode:%d", j,s_connList[j].isOccupied,s_connList[j].port,s_connList[j].mode ))
                if(s_connList[j].isOccupied == 1 && s_connList[j].port == recvQue[i].port && s_connList[j].mode == KJAVA_WMA_CONNECTION_MODE_SERVER)
                {
					JWE_LOG(sms_recv_timer, ("calling eventHandler data type is %d", recvMessage->type))

					s_connList[j].callback(s_connList[j].handle, KJAVA_WMA_EVENT_MESSAGE_ARRIVED, recvMessage);
					
					if(recvMessage->url != NULL)
					{
						SCI_FREE(recvMessage->url);
					}
					if(recvMessage->binaryData != NULL)
					{
						SCI_FREE(recvMessage->binaryData);
					}
					if(recvMessage->textData != NULL)	
					{
						SCI_FREE(recvMessage->textData);
					}
					
					SCI_MEMSET(&recvQue[i], 0, sizeof(recvQue[i]));
					SCI_MEMSET(&s_connList[j], 0, sizeof(s_connList[j]));
					return;
                }
            }
    	}
    }
}

LOCAL void init_sms_msg_queue(void)
{
    int i;
    KJavaWmaMessage *recvMessage;

    JWE_LOG(sms_free_msg_queue, ("Enter"))
    for(i = 0; i < MAX_WMA_CONN_NUM ; i++)
    {
        if(1 == recvQue[i].isOccupied)
        {
            JWE_LOG(init_sms_msg_queue, ("recvQue[%d].isOccupied, recvQue[%d].port: %d", i, i, recvQue[i].port))
            recvMessage = &recvQue[i].recvMessage;
			if(recvMessage->url != NULL)
			{
				SCI_FREE(recvMessage->url);
			}
			if(recvMessage->binaryData != NULL)
			{
				SCI_FREE(recvMessage->binaryData);
			}
			if(recvMessage->textData != NULL)	
			{
				SCI_FREE(recvMessage->textData);
			}
            SCI_MEMSET(&recvQue[i], 0, sizeof(recvQue[i]));
        }
    }
}

//******************************************************************************
//  Description:  start wma receive timer
//  Author:pan.yang
//  Date:2009-12-01
//  Note:
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartWMAReceiveTimer(void)
{
    g_java_sms_timer_id = MMK_CreateTimerCallback(1000*5, sms_recv_timer, NULL, TRUE);
    JWE_LOG(MMIAPIJAVA_StartWMAReceiveTimer, ("MMK_CreateWinTimer %d", g_java_sms_timer_id));
    MMK_ResumeTimer(g_java_sms_timer_id);
}

//******************************************************************************
//  Description:  stop wma receive timer
//  Author:pan.yang
//  Date:2009-12-01
//  Note:
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StopWMAReceiveTimer(void)
{
    JWE_LOG(MMIAPIJAVA_StopWMAReceiveTimer, ("g_java_sms_timer_id = %d", g_java_sms_timer_id))
    if (0 != g_java_sms_timer_id)
    {
		MMK_StopTimer(g_java_sms_timer_id);
		g_java_sms_timer_id = 0;
    }
}
//fix wma push case by pan.yang end

void kjava_clean_connList_by_port(int32 port)
{
    int i;

    for (i = 0; i < MAX_WMA_CONN_NUM; i++)
    {
        JWE_LOG(kjava_update_connList, ("s_connList[%d] isOccupied:%d, mode:%d", i, s_connList[i].isOccupied,  s_connList[i].mode))
        if (s_connList[i].port == port
            &&s_connList[i].isOccupied == 1
            && s_connList[i].mode == KJAVA_WMA_CONNECTION_MODE_SERVER)
        {
            SCI_MEMSET(&s_connList[i], 0, sizeof(s_connList[i]));
            break;
        }
    }
}


LOCAL void dumpbuf_wma(void *buf, int len)
{
	int i;
	unsigned char *p = (unsigned char*)buf;
	JWE_LOG(dumpbuf_wma, ("wma dumpbuf_wma %08x, %d\n", buf, len))

	for( i = 0; i < len; i++)
	{
		JWE_LOG(dumpbuf_wma, ("0x%02x ", (unsigned char)p[i]))
	}

}

/*****************************************************************************/
//  Description : Initialization of connection resource control.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_WMA_Timer_Process(int32 timerID)
{
    if (timerID == g_java_sms_timer_id)
    {
    	sms_recv_timer((uint8)timerID, NULL);
    }

    return TRUE;
}

PUBLIC BOOLEAN MMIJAVA_RegSMSPortCallback(int32 port, SMS_CALLBACK JavaSMSCallback)
{
    MMISMS_JAVA_GLOBAL_T* java_data_ptr = &g_mmisms_javadata;
    uint16                index         = 0;
    BOOLEAN               result        = FALSE;
 	//SCI_TRACE_LOW:"MMISMS_RegisterCallback()"
 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_785_112_2_18_2_23_31_416,(uint8*)"" );

    //SCI_TRACE_LOW:"MMISMS_RegisterCallback() cb is %d, port is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_787_112_2_18_2_23_31_417,(uint8*)"dd", JavaSMSCallback, port);
    for (index = 0; index < MMISMS_JAVACBF_MAX_NUM; index++)
    {
        if (java_data_ptr->java_cbf[index].jsCBF == PNULL)
        {
            java_data_ptr->java_cbf[index].jsCBF = JavaSMSCallback;
            java_data_ptr->java_cbf[index].port  = port;
            java_data_ptr->des_port = port;
            java_data_ptr->src_port = port;

            result = TRUE;
            break;
        }
    }
    return result;
}


/*****************************************************************************/
// 	Description : to unregister JAVASMS callback function
//	Global resource dependence : g_mmisms_javadata 
//  Author: 
//      @Params:     (in)port - port bound with func to unregister
//      @Return:    None
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_UnRegSMSPortCallback(int port)
{
    MMISMS_JAVA_GLOBAL_T* java_data_ptr = &g_mmisms_javadata;
    uint16                index         = 0;

    //SCI_TRACE_LOW:"MMISMS_UnRegisterCallback() port is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_816_112_2_18_2_23_31_418,(uint8*)"d", port);

    for (index = 0; index < MMISMS_JAVACBF_MAX_NUM; index++)
    {
        if (java_data_ptr->java_cbf[index].port == port && java_data_ptr->java_cbf[index].jsCBF != PNULL)
        {
            java_data_ptr->java_cbf[index].jsCBF = PNULL;
            java_data_ptr->java_cbf[index].port  = 0;
            java_data_ptr->des_port = 0;
            java_data_ptr->src_port = 0;

            //SCI_TRACE_LOW:"MMISMS_UnRegisterCallback() port found and clear"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_WMA_827_112_2_18_2_23_31_419,(uint8*)"");
            break;
        }
    }
    return TRUE;
}

PUBLIC MMISMS_JAVA_GLOBAL_T* MMIAPIJAVA_GetSmsJavaData(void)
{
    return &g_mmisms_javadata;
}


#endif
