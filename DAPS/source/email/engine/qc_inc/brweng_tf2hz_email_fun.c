//pclint
/*lint -e766 */

#include "sci_api.h"
#include "brweng_tf2hz_email.h"
#include "tf3rd_for_HZ.h"
#include "mmi_osbridge.h"

BLOCK_ID g_email_task_id = 0;
static SCI_MUTEX_PTR g_email_mutex = (void*)0;
SCI_MUTEX_PTR g_email_file_mutex = (void*)0;
SCI_MUTEX_PTR g_email_mem_mutex = (void*)0;

#define BRWENG_SUCCESS 0
#define BRWENG_FAILED  1

extern PUBLIC uint32 MMIEMAIL_GetEmailTaskPriority(void);
extern PUBLIC void *email_alloc(uint32 size, const char *file, uint32 line) ;
extern PUBLIC void email_free(void *p);

extern void  CallCB(void);
static void BRWENG20_EmailCallback(uint32 wParam)
{
   CallCB();
}

extern int OEM_Socket_HandleEvent(xSignalHeaderRec *sig_ptr);

static void BREWENG20_EmailProc(uint32 argc, void *argv)
{
   MMIEMAIL_SIG_T *pstSig = NULL;
   
   while(TRUE)
   {
      pstSig = SCI_GetSignal(g_email_task_id);
      if(pstSig)
      {
//         TF3RD_DBGPRINTF("****************BREWENG20_EmailProc ************");

         SCI_GetMutex(g_email_mutex, SCI_WAIT_FOREVER);

         if(OEM_Socket_HandleEvent((xSignalHeaderRec*)pstSig) < 0)
         {
            switch(pstSig->SignalCode)
            {
            case BRWENG_EVT_RESUME:
//               TF3RD_DBGPRINTF("BREWENG20_EmailProc--: process sig code = %x", pstSig->SignalCode);
               BRWENG20_EmailCallback((uint32)pstSig->pMsg);
               break;
            default:
//               TF3RD_DBGPRINTF("BREWENG20_EmailProc--: bad sig code = %x", pstSig->SignalCode);
               break;
            }
         }
         email_free(pstSig);
         SCI_PutMutex(g_email_mutex);
      }
   }
   
   return;
}

unsigned int BRWENG20_CreateEmailTaskThread(void)
{
   uint32 uiPriority=MMIEMAIL_GetEmailTaskPriority();
   
   if(0 == g_email_task_id)
   {  
      g_email_task_id = SCI_CreateThread("TS_Email_Task",
         "TS_Email_Queue",
         BREWENG20_EmailProc,
         g_email_task_id,
         0,
         20480,
         32,
         uiPriority,/*SCI_PRIORITY_IDLE*/
         SCI_PREEMPT,
         SCI_DONT_START);
      
      if(SCI_INVALID_BLOCK_ID == g_email_task_id || !g_email_task_id)
      {
         return BRWENG_FAILED;
      }

	  SCI_ResumeThread(g_email_task_id);
   }
   g_email_mem_mutex = SCI_CreateMutex("TS_Email_Mem_Mutex", SCI_INHERIT);
   g_email_file_mutex = SCI_CreateMutex("TS_Email_File_Mutex", SCI_INHERIT);
   g_email_mutex = SCI_CreateMutex("TS_Email_Mutex", SCI_INHERIT);

   return BRWENG_SUCCESS;
}

unsigned int BRWENG20_DeleteEmailTaskThread(void)
{
   if(g_email_task_id)
   {
      SCI_DeleteThread(g_email_task_id);
      g_email_task_id = 0;
   }

   if(g_email_mutex)
   {
      SCI_DeleteMutex(g_email_mutex);
      g_email_mutex = 0;
   }

   if(g_email_file_mutex)
   {
      SCI_DeleteMutex(g_email_file_mutex);
      g_email_file_mutex = 0;
   }

   if(g_email_mem_mutex)
   {
      SCI_DeleteMutex(g_email_mem_mutex);
      g_email_mem_mutex = 0;
   }
   
   return BRWENG_SUCCESS;
}

unsigned int BRWENG20_0EmailEventInterface(int vAppType , int vEventType, void* pMsg, void *hWnd)
{
   
   if(0 == g_email_task_id)
   {
      return FALSE;
   }

//   TF3RD_DBGPRINTF("BRWENG20_0EmailEventInterface--: EventType = %d", vEventType);
   
   switch (vAppType)
   {
   case BRWENG_EMAIL_MMI_APP:
      {
         MMIEMAIL_SIG_T *pstSig = email_alloc(sizeof(MMIEMAIL_SIG_T),__FILE__,__LINE__);
         if(!pstSig)
            return FALSE;
         SCI_MEMSET(pstSig, 0x00, sizeof(MMIEMAIL_SIG_T));

         pstSig->SignalSize  = sizeof(MMIEMAIL_SIG_T);
         pstSig->SignalCode  = vEventType;
         pstSig->pMsg = pMsg;
         
//         TF3RD_DBGPRINTF("MMI SIGNAL VALUE = 0x%08x, 0x%08x", &pstSig->sig, pMsg);
//         TF3RD_DBGPRINTF("****************POST EVENT TO APPLET ************");
         if(SCI_SendSignal(pstSig, P_APP) == SCI_SUCCESS)
            return TRUE;
         else
            return FALSE;
      }
   case BRWENG_EMAIL_MMS_ENG:
      {
         MMIEMAIL_SIG_T *pstSig = email_alloc(sizeof(MMIEMAIL_SIG_T),__FILE__,__LINE__);
         if(!pstSig)
            return FALSE;

         SCI_MEMSET(pstSig, 0x00, sizeof(MMIEMAIL_SIG_T));
         
         pstSig->SignalSize  = sizeof(MMIEMAIL_SIG_T);
         pstSig->SignalCode  = vEventType;
         pstSig->pMsg        = pMsg;

//         TF3RD_DBGPRINTF("****************POST EVENT TO MMSEngine ************");
         if(SCI_SendSignal(pstSig, g_email_task_id) == SCI_SUCCESS)
         {
//            TF3RD_DBGPRINTF("****************POST EVENT TO MMSEngine TRUE END************");
            return TRUE;
         }
         else
         {
//            TF3RD_DBGPRINTF("****************POST EVENT TO MMSEngine FALSE END************");
            return FALSE;
         }
      }
   default:
      return FALSE;
   }
   
   return FALSE;
}

