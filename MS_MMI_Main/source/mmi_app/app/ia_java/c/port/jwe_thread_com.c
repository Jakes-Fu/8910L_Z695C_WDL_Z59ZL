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
#include "ajms_common.h"
#include "jktypes.h"
#include "sci_types.h"
#include "jblend_platform.h"
#include "kjava_deprecated.h"
#include "ajutil.h"
#include "sig_code.h"
#include "jwe_log.h"
#include "mmijava_export.h"
#include "amcommconnectionevent.h"
#include "jkcommconnection.h"
#include "mmijava_internal.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define ASSERT_LOG(_x)  JWE_LOG_ERR(assert,_x)



/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/***************************************************************************/
// Description: comm read thread's main
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************/ 
void AJAVA_COMMain(uint32 argc, void* argv)
{
#if 0
    BLOCK_ID current_task_id = SCI_IdentifyThread();
    JAVA_SIGNAL *pSignal = PNULL;
    
    JKSint32 readLen = 0;
    BOOLEAN ret = FALSE;
    JKSint32 total = 0;
    JKSint32 result = 0;
    ReadFuncArg *param = PNULL;
    
    for(;;)
    {
        pSignal = (JAVA_SIGNAL *)SCI_GetSignal(current_task_id);
        //SCI_ASSERT(NULL != pSignal);
        
        if(pSignal->signalCode == KJAVA_COMMREAD_SINGALCODE)
        {
            total = 0;
            readLen = 0;
            param = (ReadFuncArg *)(pSignal->pMsg);
            do
            {
                JWE_LOG(AJAVA_COMMain, ("AJAVA_COMMain"));
				ret = MMIJAVA_CommReadData((uint8 *)(param->buf + total),  param->bufLen - total, (uint32 *)&readLen);
                if(!ret)
                {
                    result = JK_COMM_RESULT_FAILURE;
                    goto DONE;
                }
                total += readLen;

            }while(*param->used==1 && (JKSint32)total < param->bufLen);
            result = total;
DONE:
#ifndef WIN32
            AmNotifyCommEvent( param->handle, JK_COMM_EVENT_READ, result );
#endif

            SCI_FREE(pSignal->pMsg);
        }
        
    }
    
    // SCI_ThreadExit();   
#endif
}                                                                                                                                                

#endif
