/**
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */


#ifndef ___JK_VM_BACKGROUND_H
#define ___JK_VM_BACKGROUND_H

#ifdef __cplusplus
extern "C" {
#endif

  
#include "jktypes.h" 

#define JK_VMBGE_EVENT_TYPE_FG_TO_BG  0

#define JK_VMBGE_EVENT_TYPE_BG_TO_FG  1

#define JK_BG_VM_STATE_FOREGROUND  0

#define JK_BG_VM_STATE_BACKGROUND  1

/**
 *
 * Inform native system/MMI that application wants to switch from FG to BG.
 * The function need be returned as soon as possible. MMI can handle user confirmation and 
 * resource conflict checking. If MMI is ready, please invoke AamsVmBgeNotifyEvent to change VM status.
 * 
 * @param vmId
 *            Identity of vm if multi-vm supported
 * 
 */
void JkSetVmBackground(JKSint32 vmId);

/**
 *
 * Inform native system/MMI that application wants to switch from BG to FG.
 * The function need be returned as soon as possible. MMI can handle user confirmation and 
 * resource conflict checking. If MMI is ready, please invoke AamsVmBgeNotifyEvent to change VM status.
 * 
 * @param vmId
 *            Identity of vm if multi-vm supported
 * 
 */
void JkSetVmForeground(JKSint32 vmId);

void  JkNotifyPopupDialog(const JKUint8* message);

JKUint8 JkGetVmBgFgState();

JKBool JkIsVmOnRealScreen(void);

#ifdef __cplusplus
}
#endif

#endif /* ___JK_VM_BACKGROUND_H */

