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
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "kjava_sys.h"
#include "jblend_platform.h"
#include "jwe_log.h"
#include "kjava_deprecated.h"



static KJavaActionCommand kveActionCommand;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern BLOCK_ID   g_AKveId;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : kjava_sys_sendAction
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void kjava_sys_sendAction(int target, KJavaActionCommand *actionCommand_p)
{
    JAVA_SIGNAL *message = PNULL;    
    JWE_LOG(kjava_sys_sendAction, ("kjava_sys_sendAction -- target = %d, actionCommand_p = 0x%08X",target, actionCommand_p));
    
    switch (target)
    {       
        /** KJava VM Execution thread */
    case KJAVA_KVE_THREAD:
        message = (JAVA_SIGNAL *)SCI_ALLOC_APP(sizeof(JAVA_SIGNAL));
        SCI_MEMSET(message,0,sizeof(JAVA_SIGNAL));
        message->pMsg = (KJavaActionCommand*)SCI_ALLOC_APP(sizeof(KJavaActionCommand));
        SCI_MEMSET(message->pMsg,0,sizeof(KJavaActionCommand));
        
        message->signalCode = JBLENDIA_ACTION_COMMAND;
        message->signalSize = sizeof(JAVA_SIGNAL);
        message->pre = SCI_NULL;
        message->succ = SCI_NULL;
	JWE_LOG(kjava_sys_sendAction, ("SCI_SendSignal 111"));
        SCI_MEMCPY(message->pMsg, actionCommand_p, sizeof(KJavaActionCommand));
        if (g_AKveId != SCI_INVALID_BLOCK_ID)
        {
			JWE_LOG(kjava_sys_sendAction, ("SCI_SendSignal enter"));
			SCI_SendSignal((xSignalHeader)message, g_AKveId);
			JWE_LOG(kjava_sys_sendAction, ("SCI_SendSignal leave"));
        }
	 	JWE_LOG(kjava_sys_sendAction, ("SCI_SendSignal 2222"));
        return;      
      
   
    default:
        break;
    }   

    return;
}

/*****************************************************************************/
//  Description : kjava_sys_receiveAction
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
int kjava_sys_receiveAction(KJavaActionCommand **actionCommand_pp)
{
    JAVA_SIGNAL* msg = PNULL;
    int32 ret = 0;
    
    JWE_LOG(kjava_sys_receiveAction, ("kjava_sys_receiveAction -- actionCommand_pp = (0x%08X)", actionCommand_pp));
    JWE_LOG(kjava_sys_receiveAction, ("kjava_sys_receiveAction -- kveId = %d, ",g_AKveId));   

    SCI_MEMSET(&kveActionCommand, 0, sizeof(KJavaActionCommand));
    
    msg =(JAVA_SIGNAL *)SCI_GetSignal(g_AKveId);
    JWE_LOG(kjava_sys_receiveAction, ("msg is 0x%08x", msg));
    JWE_LOG(kjava_sys_receiveAction, ("msg->pMsg is 0x%08x", msg->pMsg));
    
    if (msg->signalCode == JBLENDIA_ACTION_COMMAND) 
    {
        JWE_LOG(kjava_sys_receiveAction, ("JBLENDIA_ACTION_COMMAND"));
        SCI_MEMCPY(&kveActionCommand, (KJavaActionCommand *)(msg->pMsg), sizeof(KJavaActionCommand));
		if(msg->pMsg)
		{
			SCI_FREE(msg->pMsg);
			msg->pMsg = NULL;
		}
        ret = 1;
    }
    
    // when terminate the mmapi midelt, we will catch a ilegal sigal here,
    // do not free the msg->pMsg when singalCode is not JBLENDIA_ACTION_COMMAND
    else 
    {
        JWE_LOG(kjava_sys_receiveAction, ("signalCode = %d", msg->signalCode));
    }   
    
    if(PNULL != msg->pMsg)
    {
        SCI_FREE(msg->pMsg);
        msg->pMsg = NULL;
    }
    SCI_FREE(msg);
    *actionCommand_pp = &kveActionCommand;
    JWE_LOG(kjava_sys_receiveAction, ("Leave"));
    return ret;
}

#endif
