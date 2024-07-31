/*
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/inc/mmijava_common.h#5 $
 */

/**
 * @file
 * Define input method interface for the 6800h build of FBCC
 */

#ifndef  _MMIJAVA_COMMON_H_
#define  _MMIJAVA_COMMON_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include <sci_types.h>
#include <mmk_type.h>
//#include <mmiset_export.h>
//#include <guiim.h>
//#include <guiim_base.h>
//#include <mmitheme_im.h>
#include <cpl_tpal.h>
#include <cpl_tm.h>
#include "mmijava_id.h"
#include "mmijava_menutable.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define IN
#define OUT

#define SCI_FREEIF(X)       \
    do {                    \
        if (X != NULL)      \
        {                   \
            SCI_FREE(X);    \
            X = NULL;       \
        }                   \
    }                       \
    while(0)

#ifndef WIN_DEF
//#define WIN_DEF(_WINTAB_) extern const uint32 _WINTAB_[];
#endif

//#include "mmijava_win.def"

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
//#define     JBED_RUNNING_SEMAPHORE_TIMEOUT                 5000
//for network setting module

//For im porting
#if 0
#ifdef MMI_PDA_SUPPORT
#define     MMIJAVA_ONE_CTRL_TOP         MMI_TITLE_THEME_HEIGHT + 24
#define     MMI_INPUT_EDITBOX_BOTTOM     MMI_MAINSCREEN_HEIGHT - 50
#else
#define     MMIJAVA_ONE_CTRL_TOP         MMI_TITLE_THEME_HEIGHT
#define     MMI_INPUT_EDITBOX_BOTTOM     MMI_MAINSCREEN_HEIGHT - 22
#endif
#endif

#ifdef DYNAMIC_MAINMENU_SUPPORT
#define MAX_INSTALLED_MIDLETS_NUM    32
typedef struct
{
    int used;
    int sid;
}MMIJAVA_INSTALLED_ITEM_T;
#endif


#ifdef MMI_TASK_MANAGER
typedef struct
{
    CAF_APPLET_T caf_applet;
    uint32  user_data;
}MMIJAVA_APPLET_T;   //The definition of vt applet
#endif

//for task to mmi sending message. can add field
typedef struct
{
    uint32 eventId;
    int result;
    void *data;
    int dataLen;
#ifdef SUPPORT_CAMERA_CAPTURE
    MMINAMS_SNAPSHOT_OPT_PARAM snapshotOptParam;
#endif
    uint32 blDuration; // the backlight duration.
}EVENT_TASKTOMMI_T;

typedef enum
{
  STATE_GET_FOCUS = 0,
  STATE_LOSE_FOCUS,
  STATE_DEFAULT
} RunningWinState;

/* Do we really need this static variable? */
extern BOOLEAN g_is_start_running_process;
extern int g_delete_suiteid;
extern int g_destroy_suiteid;
//extern BOOLEAN g_is_install_in_progress;
extern int g_installed_suiteId;
extern RunningApp *g_active_launching_app;
extern RunningApp g_current_launching_app;
//extern BOOLEAN s_can_stop;
extern SCI_MUTEX_PTR g_mutex_post_eventlist_ptr;
extern BOOLEAN g_jbed_mutex_flag;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Create a waiting popup window
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_openWaitingWin(
                                          MMI_STRING_T * user_prompt
                                      );

/*****************************************************************************/
//  Description :Close waiting popup window if exist
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_closeWaitingWin(int test);



/*****************************************************************************/
//  Description : handle comm info java MMI event, such as delete, app details, delete all, runtime error
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_nAms_HandleCommInfoEvent(
                                                 int event_id
                                              );
/*****************************************************************************/
//  Description : handle command install java MMI event, including notify install status, progress and result.
//  Global resource dependence :
//  Author: Sean.Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_nAms_handleMMICommandInstallEvent(EVENT_TASKTOMMI_T *event_ptr);

/*****************************************************************************/
//  Description : handle destroyed event for running Midlet
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_nAms_handleDestroyed(
                                           int event_id
                                        );


/*****************************************************************************/
//  Description : allocate MMI Event
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC MMIEvent * MMIJAVA_allocMMIEvent(void);

/*****************************************************************************/
//  Description :dealloc MMIEvent by event
//  Global resource dependence :
//  Author: nix.long
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_deallocMMIEvent_byEventID(int mmi_event_id);

/*****************************************************************************/
//  Description :dealloc all MMIEvent (some may not be dealloc)
//  Global resource dependence :
//  Author: nix.long
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_deallocAllMMIEvent(void);

/*****************************************************************************/
//  Description : handle the event of callback
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_util_amsCB(
                                  MMIEvent * mEvent
                              );


/*****************************************************************************/
//  Description :find MMIEvent by mmi event ID
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC MMIEvent * MMIJAVA_find_MMIEvent(
                                           int mmi_event_id
                                       );

/*****************************************************************************/
//  Description : MMIJAVA_NotifyJVMExit
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_HandleJVMEvent(void* event_ptr);

PUBLIC void MMIJAVA_NotifyJVMExit(void);

PUBLIC void MMIJAVA_RegMenuGroup(void);

PUBLIC void MMIJAVA_RegWinIdNameArray(void);

PUBLIC void MMIJAVA_nAms_handleMMIInstallEvent(int mmi_event_id);

PUBLIC void MMIJAVA_HandleInfoEvent(int event_id);


#ifdef MMI_TASK_MANAGER
PUBLIC void MMIJAVA_RegAppletInfo(void);

PUBLIC MMIJAVA_APPLET_T *MMIJAVA_GetAppletInstance(void);

PUBLIC void MMIAPIJAVA_StopApplet(BOOLEAN is_minimize);

PUBLIC void MMIAPIJAVA_StartApplet(void);

PUBLIC BOOLEAN MMIJAVA_GetIsDestroyAllMIDlets(void);

PUBLIC void MMIJAVA_SetIsDestroyAllMIDlets(BOOLEAN flag);

#endif //MMI_TASK_MANAGER



PUBLIC void MMIJAVA_DeleteTempFile(void);


#ifndef DRM_SUPPORT_NONE
/*****************************************************************************/
//  Description :for drm install function
//  TRUE if display, otherwise return FALSE
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OTAInstallDrm(unsigned short * name, char * url);
#endif

PUBLIC void MMIJAVA_OTAInstallInit(void);
PUBLIC void MMIJAVA_EnvironmentInit(void);
PUBLIC void DeleteFileIfExist(const uint16* name, int32 nameChars);

/*****************************************************************************/
//  Description : Handle State Conflict
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_HandleStateConflict(void);

/*****************************************************************************/
//  Description : Post the banner of MIDlet is running background.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_PostMIDletRunningBanner(void);

/* Replace the %U in text string with the midlet name by specified suite id. */
BOOLEAN MMIJAVA_GetReplaceText(MMI_TEXT_ID_T text_id, int32 suite_id, MMI_STRING_T *str_ptr);

/*****************************************************************************/
//  Description : update title on status bar with text id.
//  Global resource dependence :
//  Author: sean.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_UpdateTitle(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : Return max sim card number to jbed porting layer.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIJAVA_GetMaxSimNumber(void);

#ifdef   __cplusplus
    }
#endif

#endif /* _MMIJAVA_COMMON_H_  */

