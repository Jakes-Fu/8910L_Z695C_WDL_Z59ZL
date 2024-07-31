#ifndef _JBLENDPLATFORM_H_
#define _JBLENDPLATFORM_H_
/*------------------------------------Include--------------------------------------------*/

#include "os_api.h"
#include "kjava_deprecated.h"

#define JBLENDIA_ACTION_COMMAND 1
#define PLATFORM_AUDIO_THREAD	(KJAVA_USER_THREAD+5)
#define KJAVA_ACTION_TYPE_AUDIO_REQ          (KJAVA_ACTION_TYPE_USER + 17)

#define KJAVA_LINKUP_SINGALCODE			2
#define KJAVA_LINKDOWN_SINGALCODE		3
#define KJAVA_PUSHALARM_SINGALCODE		4	
#define KJAVA_COMMREAD_SINGALCODE		5
#define KJAVA_COMMWRITE_SINGALCODE      6


//#define KVM_ENABLE_GAMECANVAS_OPT
//#define FS_PROFILE
/*-----------------------------Type Declaration------------------------------------------*/

typedef struct _JAVA_SIGNAL
{
    uint16 					signalCode;
    uint16 					signalSize;
    uint32 					pre;
    uint32 					succ;
    uint32                  receiver;
    void					*pMsg;
}JAVA_SIGNAL;

#endif
