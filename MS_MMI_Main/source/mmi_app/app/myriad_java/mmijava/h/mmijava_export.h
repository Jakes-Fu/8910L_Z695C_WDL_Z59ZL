/*
 *  Copyright 2011-2015 Myriad Group AG. All Rights Reserved.
 *  $Id:
 */


#ifndef __MMIJAVA_EXPORT_H__
#define __MMIJAVA_EXPORT_H__

#include <java_mmi_text.h>
#include <sci_types.h>
#include <jbed_porting_define_config.h>

#ifdef JBED_CHUNKED_HEAP
    #define MMIJAVA_TASK_STACK_SPACE_SIZE  (0)
    #define MMIJAVA_TASK_QUEUE_SPACE_SIZE  (0)
    #define JBED_THREAD_STACK_SIZE         (8*1024)
    #define JBED_THREAD_QUEUE_SIZE         (20*sizeof(uint32)*SCI_QUEUE_ITEM_SIZE)
#else
    #define MMIJAVA_TASK_STACK_SPACE_SIZE  (8*1024)
    #define MMIJAVA_TASK_QUEUE_SPACE_SIZE  (20*sizeof(uint32)*SCI_QUEUE_ITEM_SIZE)
#endif /*JBED_CHUNKED_HEAP*/

#ifdef JAVA_SUPPORT_TYPE_SVM
    #if defined(NANDBOOT_SUPPORT) && !defined (PLATFORM_SC7702)
        #define MMIJAVA_HEAP_SPACE_SIZE    (5 * 512 *1024)
        #ifdef JBED_CHUNKED_HEAP
            // LIMITED_CHUNK_SIZE is used to define the limited JBED total memory alloc size when suppotted JBED_CHUNKED_HEAP.
            #define LIMITED_CHUNK_SIZE     (3 * 1024 *1024)
        #endif
    #else
        #ifndef MEMORY_CUT
            #define MMIJAVA_HEAP_SPACE_SIZE  (4 * 1024 *1024 + 512 * 1024)
        #else
            #define MMIJAVA_HEAP_SPACE_SIZE  (3 * 512 *1024)
        #endif
        #ifdef JBED_CHUNKED_HEAP
            // LIMITED_CHUNK_SIZE is used to define the limited JBED total memory alloc size when suppotted JBED_CHUNKED_HEAP.
            #define LIMITED_CHUNK_SIZE   (9 * 512 *1024)
        #endif
    #endif
#else
    /* for further mvm project definition.  */
#endif

#define MMIJAVA_HEAP_DEL_SIZE  (800 * 1024)

#define MMIJAVA_MAX_DEL_SIZE   (MMIJAVA_HEAP_DEL_SIZE+MMIJAVA_TASK_STACK_SPACE_SIZE+MMIJAVA_TASK_QUEUE_SPACE_SIZE*2)

#define MMIJAVA_MAX_SIZE       (MMIJAVA_HEAP_SPACE_SIZE+MMIJAVA_TASK_STACK_SPACE_SIZE+MMIJAVA_TASK_QUEUE_SPACE_SIZE*2)

typedef struct FLASH_BACKLIGHT
{
    BOOLEAN is_backlighton;
    SCI_TIMER_PTR flash_timer_id_ptr;
    uint32 flash_count;
}JAVA_FLASH_BACKLIGHT_T;

PUBLIC void MMIAPIJAVA_Init(void);

PUBLIC void MMIAPIJAVA_OpenMainMenu(void);

PUBLIC void MMIAPIJAVA_InitModule(void);

PUBLIC BOOLEAN MMIAPIJAVA_IsFocusJAVARunWin(void);

PUBLIC void MMIAPIJAVA_PauseBackgroundJVM(void);

PUBLIC void MMIAPIJAVA_ResumeBackgroundJVM(void);

PUBLIC BOOLEAN MMIAPIJAVA_StopAudioAndJVM(void);

PUBLIC BOOLEAN MMIAPIJAVA_IsJavaRuning(void);

PUBLIC BOOLEAN MMIAPIJAVA_IfUseAudioDevice(void);  //check if audio is occupied by java

PUBLIC const char* MMIAPIJAVA_GetVersion(void);

PUBLIC void MMIAPIJAVA_StopOperaSDPlug(void);

PUBLIC void MMIAPIJAVA_HandSDCardChanged(int sd_type,BOOLEAN is_rootadded);

/*****************************************************************************/
// Description: handle all java MMI Events
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_HandleJbedMMIEvent(void* event_ptr);

/**************************************************************************************************/
// Description: Interface for enter taksmanager from status bar.
// Date:2011-08-30
// Author:Martin.Zhao
// Note:
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_OpenTaskMgr(void);

/*****************************************************************************/
//  Description :This API is used to install a Java application which is
//               located by the path. It can be a filesytem path, or a network
//               resource path, such as http or https.
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
//       @param path
//              the java application located.
//       @param pathSize
//              The character number of the Path(Not bytes).
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_Install(
                            uint16 * path,
                            int32    path_size
                       );

PUBLIC BOOLEAN MMIAPIJAVA_CheckSmsIsForJava(int port);

PUBLIC BOOLEAN MMIAPIJAVA_RetrieveSmsContent(void* msg);

PUBLIC BOOLEAN MMIAPIJAVA_IsWaitingWinOpen(void);
/*****************************************************************************/
//  Description : to install form file system
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystemForDM(
                                             wchar *pathname,
                                             int pathname_len,
                                             wchar *filename,
                                             int filename_len);

PUBLIC void MMIAPIJAVA_UdiskNotifyJavaRunning(void);

PUBLIC void MMIAPIJAVA_resetJbedFactorySetting(void);

/*****************************************************************************/
//  Description : to start flash backlight
//  Global resource dependence : none
//  Author:minghui.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StartJbedFlashBacklight(uint32 duration);
/*****************************************************************************/
//  Description : to stop flash backlight
//  Global resource dependence : none
//  Author:minghui.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopJbedFlashBacklight(void);

/*****************************************************************************/
//  Description : To see if the java app is running on foreground.
//      1) The midlet is running on foreground.
//      2) It also regards as running on foreground when installing midlet.
//  Global resource dependence:
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsForegroundRuning(void);

/*****************************************************************************/
//  Description : Command install a midlet suite with specified jad or jar path.
//  Global resource dependence : none
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_CommandInstall(char *path);


/*****************************************************************************/
//  Description : check whether the MIDlet suite is available or not.
//  Global resource dependence : none
//  Author:kula
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_isMIDletSuiteAvailable(int32 suiteId);

/*****************************************************************************/
//  Description : Get available suiteIDs.
//        If ids is NULL or idsSize is 0, only return the available ids count;
//      Otherwise, avaialbe suite Id will be stored in param ids.
//        This API can be called twice, first-time with (ids == null, idsSize == 0)
//      to get the availabe suite id count. second-time with ids buffer and the size
//      of ids buffer(idsSize) to get avaialbe suite ids.
//      @param ids, the buffer used to store available suite Id.
//      @param idsSize, the size of buffer ids;
//      @return the available suite Id count;
//  Global resource dependence : none
//  Author:kula
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIJAVA_getSDCardAvailableSuites(int32 *ids, int32 idsSize);

/*****************************************************************************/
//  Description : Start a MIDlet suite by specifying the suite id.
//  @param suite_id, the suite id of midlet suite which you want to run.
//                   the suite id must be set a meaningful value.
//  @return TRUE if it is successful to start the suite.
//          FASLE if it is failed to start the suite.
//  Global resource dependence:
//  Author: Sean Xu
// CAUTION: 1) We create the MMIJAVA_StartSuiteEx function with conflict check
//             instead of MMIJAVA_StartSuite to be called by external modules.
//             Please do not call MMIJAVA_StartSuite anymore from outside module.
//          2) The caller(SPRD) should specify the parameter of suite id
//             with meaningful value. You've better let Myriad know if
//             you are not sure how to get correct suite id.
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_StartSuiteEx(int suite_id);

#ifdef MICROSOFT_WNS_SUPPORT
/*****************************************************************************/
//  Description : launch a MIDlet suite by wns push message identifier
//  @param wnsIdentifier, the identifer of WNS applicaions.
//  @return the MIDlet suite id which is equal or greater than zero;
//         -1 means MIDlet suite not found.
//  Global resource dependence :
//  Author: Kula
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_LaunchMIDletSuiteByWnsPushMessage(char *wnsIdentifier);

/*****************************************************************************/
//  Description : Get icon by WNS push message identifier which is specified in JAD
//  or MANIFEST file with property: MIDlet-WNS-AppName;
//  Usage: You probably need to call the function twice:
//      The first time, you specify the parameter of buffer as NULL, the function will
//      return the size of icon buffer which you need to malloc. After you mallocated
//      the buffer passed as the parameter and call MMIJAVA_GetIconByWnsIdentifier
//      again, the icon data will be filled in this buffer. You are also responsible
//      for freeing the buffer.
//  @param wnsIdentifier, the identifer of WNS applicaions.
//  @param buffer, the buffer is used for filling icon data.
//  @param bufLen, the length of buffer, it is of no effect when buffer is passed as NULL.
//  @return the icon buffer size; return -1 means no responding icon or getting icon failed.
//  Global resource dependence:
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_GetIconByWnsIdentifier(char *wnsIdentifier, uint8 *buffer, int bufLen);

/*****************************************************************************/
//  Description : Get icon path by WNS push message identifier which is specified in JAD
//  or MANIFEST file with property: MIDlet-WNS-AppName;
//  @param wnsIdentifier, the identifer of WNS applicaions.
//  @param iconPath, the pass out value of icon path in unicode string format.
//  @return the icon path length caculated by unicode unit;
//          <= 0 means no responding icon in WNS MIDlet or getting icon path failed.
//  Example code:
//      int path_len = 0;
//      uint16 *icon_path = NULL;
//      path_len = MMIJAVA_GetIconPathByWnsIdentifier(identifier, &icon_path);
//      if (path_len > 0)
//      {
//          // Use icon_path value as your wish.
//      }
//      SCI_FREEIF(icon_path);  //remember to free the memory.
//  Global resource dependence:
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_GetIconPathByWnsIdentifier(char *identifier,  uint16 **iconPath);


typedef enum APP_STATE_E {
    /* the specific application is not found or not installed */
    STATE_APP_NOT_FOUND = 0,
    /* no application is running */
    STATE_NO_APP_RUNNING,
    /* other applicaiton is running, but specific application is in down state */
    STATE_RUNNING_OTHER_APP,
    /* the specific application is running in background */
    STATE_RUNNING_IN_BG,
    /* the specific application is running in foreground*/
    STATE_RUNNING_IN_FG,
    STATE_MAX
} APP_STATE;


/*****************************************************************************/
//  Description : Query the specific application status. return values refers to
//                enumeration APP_STATE.
//  @return, application status.
//
//  Global resource dependence:
//  Author:
/*****************************************************************************/
PUBLIC APP_STATE MMIJAVA_QueryWnsAppState(char *wnsIdentifier);

#endif

/*****************************************************************************/
//  Description : Is Can Stop
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsCanStop(void);

PUBLIC void MMIJAVA_OpenAlertByTextId(uint32 text_id);

/**
 * When native application sign off MSA, call this API to notify Java application
 * that MSA is signed off by other apps.
 */
PUBLIC void MMIJAVA_MSASignOffNotify();


/*****************************************************************************/
//  Description : Set Application running properties. It can set application
//                properties. And application can use System.getPropert to
//                get this value.
//  Global resource dependence:
//  Author:
//  Note: If the property is set before launching an application, it must be called
//        before calling MMIJAVA_StartSuite().
/*****************************************************************************/
PUBLIC void MMIJAVA_SetAppProperty(const char *key, const char *val);

#endif
