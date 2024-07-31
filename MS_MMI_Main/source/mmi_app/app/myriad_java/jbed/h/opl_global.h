/*
 * Copyright 2012 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

#ifndef __OPL_GLOBAL_H__
#define __OPL_GLOBAL_H__

#include <jbed_porting_define_config.h>
#include <guiimg.h>
#include <mmk_app.h>
#include <mmk_type.h>
#include <mmifmm_export.h>
#ifdef BROWSER_SUPPORT
#include <mmibrowser_export.h>
#endif
#include <jbed_global.h>
#ifdef SUPPORT_CAMERA_CAPTURE
#include <cpl_tpal.h>
#endif


#define MAX_ARGUMENTS_NUM  (16)

#if !defined(MMI_SDCARD_MAX_NUM)
typedef enum
{
    MMI_DEVICE_SYSTEM = 0,
    MMI_DEVICE_UDISK,
    MMI_DEVICE_SDCARD,
#ifdef DUAL_TCARD_SUPPORT
    MMI_DEVICE_SDCARD_1,
#endif
    MMI_DEVICE_NUM
} MMIFILE_DEVICE_E;
#endif

#define NUM_OF_ROOTS  (MMI_DEVICE_NUM)
#define JAVA_COMMON_SYSTEM      "System/"
#define JAVA_COMMON_UDISK       "Phone/"

#ifdef DUAL_TCARD_SUPPORT
#define JAVA_COMMON_TFCARD1     "TFCard1/"
#define JAVA_COMMON_TFCARD2     "TFCard2/"
#else
#define JAVA_COMMON_TFCARD      "TFCard/"
#endif

#ifdef AMS_NETWORK_PARAMETERS_SUPPORT

#define Jbed_net_getSettings \
            JbniV_com_jbed_ams_NetworkParameters_nGetSettings


#define Jbed_net_saveSettings \
            JbniV_com_jbed_ams_NetworkParameters_nSaveSettings


#endif //#ifdef AMS_NETWORK_PARAMETERS_SUPPORT

typedef struct _JbedGlobalArguments
{
    int    argc;
    char * argv[MAX_ARGUMENTS_NUM];
}JbedGlobalArguments;

typedef struct _JbedGfxInfo
{
    GUIIMG_BITMAP_T bitmap;
}JbedGfxInfo;

/* ----------------------------------------- */
/* -------------- GLOBAL DATA -------------- */
/* ----------------------------------------- */

extern int64_t  g_timeBase;

/* ----------------------------------------- */
/* ------------ External Data -------------- */
/* ----------------------------------------- */

extern GLOBAL struct INSTANCE_com_jbed_runtime_SignalQueue_s *eventsHandle;

/* ----------------------------------------- */
/* ------------ GLOBAL APIs ---------------- */
/* ----------------------------------------- */
PUBLIC void CPL_os_init(void);

PUBLIC void CPL_os_final(void);

PUBLIC void finializeGlobalArgs(void);

PUBLIC void * Jbed_getCmdLineArgs(void);

PUBLIC bool_t Jbed_startVM(void);

//bool_t Jbed_conn_activeNetwowrk(int run_id,void * pCB);

PUBLIC bool_t Jbed_conn_getIsUseWifi(void);

PUBLIC int MMIJAVA_updateRunningWinState(int state, BOOLEAN needUpdate);

PUBLIC void resetCertsPolicyIfNotExist(void);

PUBLIC void refreshPaintWindowFull(void);

PUBLIC int32_t Jbed_getJbedState(void);

PUBLIC void Jbed_setJbedCurState(int32_t newstate);

PUBLIC char * Jbed_getRootPath(void);

PUBLIC bool_t JavaEntry(int argc, char * argv[]);

PUBLIC int32_t getAmsRootInfo(char *ams_root_name[],char *ams_root_paths[]);

PUBLIC int32_t getRootInfo(char *root_names[],char *root_paths[]);

PUBLIC bool_t Jbed_is_javalist_open(void);

PUBLIC void cpl_gfx_getInfo(JbedGfxInfo *info);

PUBLIC BOOL Jbed_checkRootAvailable(void);

PUBLIC void setRootProperty(void);

PUBLIC void Jbed_Get_VritualKeyBoardBitmap(GUIIMG_BITMAP_T *virtualkey_bitmap_ptr);


PUBLIC void Jbed_vmWantForeground(bool_t foreground);

PUBLIC void Jbed_vmWantToIdle(void);

PUBLIC void Jbed_nAms_gotoBackground(void);

PUBLIC int32_t Jbed_tm_getRunningAppNum(void);

PUBLIC void initializeConfig(char *midp_home);

#ifdef JAVA_SUPPORT_TYPE_MVM
PUBLIC void Cpl_media_launchAppCheckMedia(int suite_id);

PUBLIC void Cpl_media_exitAppCloseMedia(int suite_id);
#endif

PUBLIC bool_t Jbed_HandleEvents(MMI_MESSAGE_ID_E msgId,DPARAM param,BOOL is_virtual_key);

PUBLIC void Jbed_nAms_handleMMIInstallEvent(int mmi_event_id);

PUBLIC MMIFILE_DEVICE_E Jbed_getRootTypeByIndex(int index);

PUBLIC char* Jbed_getRootByType(MMIFILE_DEVICE_E device_type);

PUBLIC char* Jbed_getJavaRootName(void);

PUBLIC MMIFILE_DEVICE_E Jbed_getJavaRootType(void);

PUBLIC void Jbed_nAms_actionPendingEvt(void);

PUBLIC void Jbed_nAms_setTransitionFlag(BOOL flag);

PUBLIC BOOL Jbed_nAms_getTransitionFlag(void);

PUBLIC void Jbed_I18N_init(void);

PUBLIC void Jbed_updateLanguageInfo(void);

PUBLIC void Jbed_I18N_final(void);

PUBLIC void JPL_net_shutdown(void);

PUBLIC void Jbed_nAms_finalize(void);

PUBLIC int Jbed_nAms_checkDelApp(int is_del);

PUBLIC const char* Jbed_getVersion(void);

PUBLIC bool_t isJbedThreadExist(void);

PUBLIC bool_t Jbed_clrPendEvt(void);

PUBLIC void Jbed_startNextTimeScheduling(void);

PUBLIC void OPL_os_createTimerCallback(void);

PUBLIC void refreshPaintWindow(int x1, int y1, int x2, int y2);

PUBLIC int32_t CPL_gfx_paint_logo(void);

PUBLIC void Jbed_exitVM(void);

PUBLIC void Jbed_startNextTimePush(void);

PUBLIC void Jbed_requireStopTimer(void);

PUBLIC void Jbed_ClearFileInitSignal(void);

PUBLIC bool_t Jbed_needRightAlign(void);

#if !defined(SUPPORT_SELECT_STORAGE) || defined(AMS_TCK_RUNNER)
PUBLIC wchar * Jbed_getDefaultInstallDiskName(void);
#endif

#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
bool_t Jbed_isInRunState(void);
bool_t Jbed_isMIDletInRunning(void);
#endif

#ifdef USE_JBEDLIB_MEMORY_POOL
PUBLIC BOOLEAN Jbed_initLibMemPool(void);
PUBLIC void Jbed_finalLibMemPool(void);
#endif

#if defined(BROWSER_SUPPORT)
MMIBROWSER_ENTRY_PARAM * Jpl_getOrset_BrowserStartParam(int operate,int * be_start);
#endif

#if defined(SUPPORTS_PUSH_SMS) || defined(SUPPORTS_PUSH_ALARM)

void Jbed_nAms_setLaunchMIDletFlag(BOOLEAN flag);

void Jbed_nAms_pushMIDletInProgress(void);

#endif //SUPPORTS_PUSH_SMS || SUPPORTS_PUSH_ALARM

void CPL_wma_initialize(void);

void CPL_file_delMutex(void);

#ifdef SUPPORT_CAMERA_CAPTURE
PUBLIC BOOLEAN Jbed_IsCameraOpened(void);
PUBLIC void Jbed_set_previewStatus(MMINAMS_SNAPSHOT_OPERATION_TYPE reqOptType);
PUBLIC void Jbed_fill_previewRect(GUI_RECT_T* preview_rect);
PUBLIC BOOLEAN Jbed_preview_isFullScreen(void);
#endif

typedef void (*rstFactoryCb)(void);

void MMIJAVA_regJbedFactoryCB(rstFactoryCb cb);

void MMIJAVA_resetJbedFactoryCB(void);

BOOLEAN MMIJAVA_getJbedResetState(void);

typedef enum
{
    REPO_TYPE_PUSH,
    REPO_TYPE_PULL
}REPO_OP_E;  /*定义仓库动作*/

#define REPO_MAX_SIZE 256

/*
 * jad_url - push's input; return - pull's output
 */
char * MMIJAVA_JadUrlRepo(REPO_OP_E opt,char * jad_url);

PUBLIC void MMIJAVA_HandleJbedExitEvent(void);

void Jbed_nAms_pushMIDletHandled(void);

#endif  /*__OPL_GLOBAL_H__*/


