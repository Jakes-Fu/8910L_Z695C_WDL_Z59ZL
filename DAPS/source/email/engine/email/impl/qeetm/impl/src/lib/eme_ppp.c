/*=====================================================================================
FILE NAME   : eme_ppp.c
MODULE NAME : Email engine

     GENERAL DESCRIPTION
       Provide some function for managing transaction.
        
     TechFaith Software Confidential Proprietary
     Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
     =======================================================================================
     Revision History
     
     Modification                        Tracking
     Date                  Author        Number        Description of changes
     ----------    --------------    ---------    --------------------------------------
     2007-05-11      Rachel                            create eme_ppp.c
             
=====================================================================================*/


/*===================================================================
 * 
 *                           Include Files
 * 
====================================================================*/
/*Include System head file*/
#include "oem_ppp_api.h"
#include "brweng_tf2hz_email.h"
//#include "We_GprsAccountProfile.h"
#include  "oem_soc_api.h"
#include "eme_utils_api.h"

/*Include Program Global head file*/ 

/* Define Constant Macro Start */
#define PPP_CONTEXT_ID  0x0a 
#define PPP_PROFILE_ID  0x0f

/* Define Constant Macro End */

/*============================================================================
 *	Private/Local Function Declarations
 *===========================================================================*/
static void EME_GPRSSetAccountRsp(void* para);
static void EME_GPRSGetAccountRsp(void* para);

typedef struct _StPPPData
{
   WE_INT32     iProfileId;
   WE_VOID      *pvMemMgr;
   FnCallBackPn pFn;
   WE_VOID      *pvUser;
   WE_INT32     iDialId;
}St_PPPData;

St_PPPData *g_pstData = NULL;

/*====================================================================
DESCRIPTION
     OEM_DialByProfile
         
PARAMETERS
    

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 
WE_INT32 OEM_DialByProfile(WE_VOID *pvMemMgr, WE_INT32 iProfileId, FnCallBackPn pFn, WE_VOID *pvUser)
{
   pFn(pvUser);
   return EME_SUCCESS;
}

/*====================================================================
DESCRIPTION
     EME_GPRSSetAccountRsp
         
PARAMETERS
    

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 
static void EME_GPRSSetAccountRsp(void* para)
{
   
#ifdef __MTK_TARGET__  
   mmi_ps_set_gprs_data_account_rsp_struct *local_data = (mmi_ps_set_gprs_data_account_rsp_struct*) para;
   mmi_ps_get_gprs_data_account_req_struct *myMsgPtr;
   ilm_struct *msg;
   
   EME_UTILS_LOG_INFO(("EME_GPRSSetAccountRsp: begin para = 0x%x\n")); 
   
   if(local_data != NULL && local_data->result == KAL_TRUE)
   {
      EME_UTILS_LOG_INFO(("EME_GPRSSetAccountRsp: set account req success resp\n")); 
      
      myMsgPtr = (mmi_ps_get_gprs_data_account_req_struct*)construct_local_para(
         sizeof(mmi_ps_get_gprs_data_account_req_struct), 
         TD_CTRL);     
      if(myMsgPtr == NULL)
      {
         EME_UTILS_LOG_INFO(("EME_GPRSSetAccountRsp: malloc myMsgPtr fail\n")); 
         //call cb to notify PPP link fail
         
         EME_UTILS_LOG_INFO(("EME_GPRSSetAccountRsp: construct_local_para fail\n")); 
         return;
      }
      
      myMsgPtr->profile_id = PPP_PROFILE_ID; 
      
      EME_UTILS_LOG_INFO(("EME_GPRSSetAccountRsp: profile_id = %d\n",myMsgPtr->profile_id)); 
      
      msg = allocate_ilm(MOD_TS_EMA_ENG);
      msg->src_mod_id = MOD_TS_EMA_ENG;
      msg->dest_mod_id = MOD_L4C;
      msg->msg_id = PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ;
      msg->local_para_ptr = (local_para_struct *)myMsgPtr;
      msg->peer_buff_ptr = NULL;
      msg->sap_id = 0;
      
      SetProtocolEventHandler(EME_GPRSGetAccountRsp,PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP);
      msg_send_ext_queue(msg);
   }
   else
   {
      //call cb to notify PPP link fail
      EME_UTILS_LOG_INFO(("EME_GPRSGetAccountRsp: set account req fail resp\n")); 
      
      return;
   }
#else
   return;
#endif
   
}




/*====================================================================
DESCRIPTION
     EME_GPRSGetAccountRsp
         
PARAMETERS
    

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 
static void EME_GPRSGetAccountRsp(void* para)
{
#ifdef  __MTK_TARGET__  
   
   mmi_ps_get_gprs_data_account_rsp_struct *local_data = (mmi_ps_get_gprs_data_account_rsp_struct*) para;
   
   EME_UTILS_LOG_INFO(("EME_GPRSGetAccountRsp: begin para = 0x%x\n")); 
   
   if( local_data != NULL&&local_data->result == KAL_TRUE)
   {
      //call cb to notify PPP link success
      EME_UTILS_LOG_INFO(("EME_GPRSGetAccountRsp: get account req success resp\n")); 
      /*
      step1:get the host name by id 
      */ 
      if (g_pstData != NULL && g_pstData->pFn != NULL)
      {
         g_pstData->pFn(g_pstData->pvUser);
      }
   }
   else
   {
      //call cb to notify PPP link fail
      EME_UTILS_LOG_INFO(("EME_GPRSGetAccountRsp: get account req fail resp\n")); 
      return;
   }
   
#else
   
   return;
   
#endif
}



/*====================================================================
DESCRIPTION
     EME_DisconnectPPP
PARAMETERS
    

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 

WE_VOID  EME_DisconnectPPP(WE_INT32 nDialId)
{
#ifdef __MTK_TARGET__
   
   EME_UTILS_LOG_INFO(("EME_DisconnectPPP====START!"));
   soc_close_nwk_account(MOD_TS_EMA_ENG);
   EME_UTILS_LOG_INFO(("EME_DisconnectPPP====END!"));
   return;
   
#endif     
}



/*====================================================================
DESCRIPTION
     EME_GetDialId
PARAMETERS
    

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 

WE_VOID  EME_GetDialId(WE_INT32 *pnDialId)
{
   if (NULL != g_pstData)
   {
      *pnDialId = g_pstData->iDialId;
   }
   return;
}


/*====================================================================
DESCRIPTION
     EME_FreePPPData
PARAMETERS
    

RETURN VALUE
   TRUE: If operation has been processed.
   FALSE: If operation has not been processed.
=====================================================================*/ 

WE_VOID  EME_FreePPPData(WE_VOID)
{
   EME_UTILS_LOG_INFO(("EME_FreePPPData====START!"));
   if (NULL != g_pstData)
   {
      EME_UTILS_LOG_INFO(("EME_FreePPPData====g_pstData !"));
      g_pstData->pFn = NULL;
   }    
   EME_UTILS_FREEIF(g_pstData->pvMemMgr, g_pstData);
   EME_UTILS_LOG_INFO(("EME_FreePPPData==== END !"));
   return;
}
