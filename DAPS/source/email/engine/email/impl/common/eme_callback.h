#ifndef  _EMAIL_CALLBACK_H_
#define  _EMAIL_CALLBACK_H_

#include "we_def.h"

typedef struct _AEECallback  AEECallback;
typedef void (*PFNNOTIFY)(void * pUser);
typedef void (*PFNCBCANCEL)(AEECallback *pstCB);


struct _AEECallback
{
   void                     *pmc;              /* RESERVED */
   void                     *pCancelData;      /* Filled by callback handler */
   void                     *pNotifyData;      /* Filled by caller */
   void                     *pReserved;        /* RESERVED - Used by handler */
   PFNCBCANCEL              pfnCancel;         /* Filled by callback handler */
   PFNNOTIFY                pfnNotify;         /* Filled by caller */
   struct _AEECallback      *pstNext;
};



#define CALLBACK_Init(pcb,pcf,pcx) {(pcb)->pfnNotify = (PFNNOTIFY)(pcf); \
                                    (pcb)->pNotifyData = (pcx);}
#define CALLBACK_Cancel(pcb)    if (0 != (pcb)->pfnCancel) (pcb)->pfnCancel(pcb)
#define CALLBACK_IsQueued(pcb)  (0 != (pcb)->pfnCancel)
#define ISHELL_EmailResume(p)  IShell_EmailResume(p);

void	 IShell_EmailResume(AEECallback *pstCB);
#endif



