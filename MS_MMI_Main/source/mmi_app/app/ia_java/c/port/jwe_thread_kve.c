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
#include "jwe_log.h"
#include "kjava_sys.h"
#include "ajsc_platform.h"  
#include "kjava_action.h"
#include "ajms_common.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define ASSERT_LOG(_x)  JWE_LOG_ERR(assert,_x)



extern PUBLIC void jwe_vmstart_begin(void);
extern PUBLIC void jwe_vmstart_end(void);                                                          

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/***************************************************************************/
// Description: AJAVA_KVEMain
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************/
int jwe_vmstart_flag;
PUBLIC void AJAVA_KVEMain(uint32 argc, void* argv)                                                                 
{
    
    KJavaActionCommand *m_kveMessage = PNULL;
    int ret;
    JWE_LOG(AJAVA_KVEMain, ("================[KVE] Thread started."))
#ifndef WIN32  
    ajsc_kve_initialize();
#endif
    
    while(1)/*lint !e716*/
	{
		jwe_vmstart_flag=-1;
		if (kjava_sys_receiveAction( &m_kveMessage) == 1) 
		{
			JWE_LOG(AJAVA_KVEMain, ("ajsc_kve_processAction start m_kveMessage->type = %d",m_kveMessage->type))
			if(m_kveMessage->type == KJAVA_ACTION_TYPE_START_VM)
			{
				jwe_vmstart_flag = KJAVA_ACTION_TYPE_START_VM;
				jwe_vmstart_begin();

			}
			ret = ajsc_kve_processAction(m_kveMessage);
			JWE_LOG(AJAVA_KVEMain, ("ajsc_kve_processAction complete ret=%d",ret))
			if(jwe_vmstart_flag==KJAVA_ACTION_TYPE_START_VM)	
			{
				jwe_vmstart_end();
			}
		}
		else 
		{
			JWE_LOG(AJAVA_KVEMain, ("VM Error! Failed to receive message!"))
		}
	}   
    
}


#endif
