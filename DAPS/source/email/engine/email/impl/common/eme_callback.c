#include "brweng_tf2hz_email.h"
#include "eme_utils_api.h"
#include "eme_callback.h"

static AEECallback *gst_callback_list = NULL;

static void CancelCB(AEECallback *pstCB)
{
   AEECallback *pstCBTmp = NULL;
   AEECallback *pstCBPre = NULL;

   if(gst_callback_list == NULL || NULL == pstCB)
   {
      return;
   }

   if(pstCB == gst_callback_list)
   {
      gst_callback_list = gst_callback_list->pstNext;
      pstCB->pfnCancel = NULL;
      pstCB->pstNext = NULL;
      return;
   }

   pstCBPre = gst_callback_list;
   pstCBTmp = gst_callback_list->pstNext;
   while(pstCBTmp != NULL)
   {
      if(pstCBTmp == pstCB)
      {
         pstCBPre->pstNext = pstCBTmp->pstNext;
         pstCBTmp->pfnCancel = NULL;
         pstCBTmp->pstNext = NULL;
         pstCBTmp = pstCBPre->pstNext;
      }
      else
      {
         pstCBPre = pstCBTmp;
         pstCBTmp = pstCBTmp->pstNext;
      }
   }

   return;
}

void IShell_EmailResume(AEECallback *pstCB)
{
  
	if(NULL == pstCB)
	{
		return;
	}

   pstCB->pfnCancel = CancelCB;

   if(NULL == gst_callback_list)
   {
      gst_callback_list = pstCB;
   }
   else
   {
      AEECallback *pstCBTmp = gst_callback_list;
      while(pstCBTmp->pstNext != NULL)
      {
         pstCBTmp = pstCBTmp->pstNext;
      }

      pstCBTmp->pstNext = pstCB;
   }
   TS_HZ_EMAIL_EVENT_CALLBACK(BRWENG_EMAIL_MMS_ENG, BRWENG_EVT_RESUME, pstCB, NULL);
}

void  CallCB(void)
{
   AEECallback *pstCBTmp = gst_callback_list;

   if(NULL != gst_callback_list)
   {
      gst_callback_list = gst_callback_list->pstNext;

      pstCBTmp->pCancelData = NULL;
      pstCBTmp->pfnCancel = NULL;
      pstCBTmp->pstNext = NULL;
      if(NULL != pstCBTmp->pfnNotify)
      {
         (*pstCBTmp->pfnNotify)(pstCBTmp->pNotifyData);
      }
   }

   return;
}

WE_INT  Email_HandleTransAndSigCB(AEECallback *pstTransCB, AEECallback *pstSigCB)
{
	AEECallback                stTemp;

	if ((NULL == pstTransCB) || (NULL == pstSigCB))
	{
		return EME_FAILED;
	}

	EME_UTILS_MEMSET(&stTemp, 0x00, sizeof(AEECallback));
	if(CALLBACK_IsQueued(pstSigCB))
	{
		
		stTemp.pfnNotify = pstSigCB->pfnNotify;
		stTemp.pNotifyData = pstSigCB->pNotifyData;
		pstSigCB->pfnCancel(pstSigCB);
		stTemp.pfnNotify(stTemp.pNotifyData);
	}
	else if (CALLBACK_IsQueued(pstTransCB))
	{
		stTemp.pfnNotify = pstTransCB->pfnNotify;
		stTemp.pNotifyData = pstTransCB->pNotifyData;
		pstTransCB->pfnCancel(pstTransCB);
		stTemp.pfnNotify(stTemp.pNotifyData);
	}
	
	return EME_SUCCESS;
}






