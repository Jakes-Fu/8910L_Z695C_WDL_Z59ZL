#ifndef __MMIJAVA_TASKMGRWIN_H__
#define __MMIJAVA_TASKMGRWIN_H__

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : Destroy current all running midlets, the vm will be shutdown
//                when the last midlet is destroyed.
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
void MMIJAVA_Destroy_AllRunningMIDlets(void);

void MMIJAVA_CloseJavaAppletWin(void);

#endif /*#ifdef MMI_TASK_MANAGER*/

#endif /*__MMIJAVA_TASKMGRWIN_H__*/