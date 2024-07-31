/***********************************************************************
**Author:    huangjc
**Date:      2007-4-18
**CopyRight:AMOI 2007
**Description:This file define the function of java nv
***********************************************************************/
/***********************************************************************
**                                                Edit History
**      Date                    Name               Description
***2007-4-18              huangjc                create
***********************************************************************/
#ifndef  _MMIJAVA_INTERNAL_H_    
#define  _MMIJAVA_INTERNAL_H_   

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_appmsg.h"  //cocmmon 
#include "mmi_default.h"
#include "jblend_config.h"
#include "mmk_type.h"
#include "ajms_ota.h"
#include "guifont.h"
#include "gui_ucs2b_converter.h"
#include "mmk_app.h"
#include "mmk_timer.h"



#include "mmiaudio.h" //
#include "mmipdp_export.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "jwe_log.h" //
#include "java_id.h"
#include "java_mmi_text.h"
#include "java_menutable.h"
#include "java_mmi_image.h"
#include "mmijava_nv.h"



#include "kjava_sys_gfx.h"//
#include "ajms_cfg_eime.h"

//#include "mmijava_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/        
#define     STATUS_CODE_ARRAY_SIZE 13
#define     M_SECOND_1970_TO_1980    ((365*8 +366*2)*24*3600)

#define     SPREADTRUM_LCD_SOFTKEY_WIDTH 			MMI_MAINSCREEN_WIDTH
#define     SPREADTRUM_LCD_SOFTKEY_HEIGHT 			25//MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT
#define     NCI_TCK   //20110124 -murphy.xie
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   
#ifdef WIN32
typedef __int64 int64_t;
#else
typedef long long int64_t;
#endif

typedef struct _InfoCheck
{
	uint8 name[255];
	int32 majorVersion;
	int32 minorVersion;
	int32 microVersion;
	uint8 vendor[255];
	int32 jarSize;
	int32 update;
	int32 oldId;
}InfoCheck;

typedef struct _OTANotifyStatusCode 
{
    int     code;
    char*   text;
} OTANotifyStatusCode;

typedef enum
{
	JAVA_NETWORK_TYPE_GPRS_SIM1 = 0,
	JAVA_NETWORK_TYPE_GPRS_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD) 
	JAVA_NETWORK_TYPE_GPRS_SIM3, 
#if defined(MMI_MULTI_SIM_SYS_QUAD)
	JAVA_NETWORK_TYPE_GPRS_SIM4,
#endif
#endif
	JAVA_NETWORK_TYPE_WIFI,
    JAVA_NETWORK_TYPE_MAX
}MMIJAVA_NETWORK_TYPE_E;


typedef struct _MMIJAVA_LINK_SETTING_T_
{
    uint8 sim_index[MMI_DUAL_SYS_MAX];    
    MMIJAVA_NETWORK_TYPE_E network_type;
}MMIJAVA_LINK_SETTING_T;    //Ivy

typedef struct
{
	uint8	url[MMIJAVA_MAX_DOWNLOAD_URL_LEN +1];		//那谷??赤??﹞
	uint16	url_len;								//那谷??赤??﹞3∟?豕
}MMIJAVA_DOWNLOAD_ITEM_DETAIL_T;

typedef struct
{
    MMIJAVA_DOWNLOAD_ITEM_DETAIL_T game_info;
    MMIJAVA_DOWNLOAD_ITEM_DETAIL_T ebook_info;
    MMIJAVA_DOWNLOAD_ITEM_DETAIL_T other_info;
}MMIJAVA_DOWNLOAD_DETAIL_T;

typedef struct
{
	BOOLEAN     is_needContinueOTAInstall;
    BOOLEAN     is_drm;
	BOOLEAN     is_jad;
	wchar       fileName[MMIFILE_FULL_PATH_MAX_LEN + 1];
	uint32      mime_type;
	char*      url;
}MMIJAVA_CONTINUE_OTA_INSTALL_T;

typedef enum
{
	INSTALL_NONE =0,
	JAD_LOCALL_INSTALL,
	JAR_LOCALL_INSTALL,
	JAD_OTA_INSTALL,
	JAR_OTA_INSTALL
}INSTALL_TYPE;


typedef enum
{
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
    MMIJAVA_FG_RUNNING_STATE = 50,
    MMIJAVA_BG_RUNNING_STATE =  40,
#endif
    MMIJAVA_SHORT_EVENT_STATE = 30,
    MMIJAVA_LONG_EVENT_STATE = 20,
    MMIJAVA_PAUSED_STATE = 10,
    MMIJAVA_TERMINATE_STATE = 0
} mmijava_state_enum;


typedef enum
{
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
	PAUSE_DIALG_BTN_MINIMIZE_TYPE,
#endif
	PAUSE_DIALG_BTN_PAUSE_TYPE,
	PAUSE_DIALG_BTN_TERMINATE_TYPE,
	//...
	PAUSE_DIALG_BTN_RETURN_TYPE,
} PAUSE_DIALG_BTN_TYPE;

typedef enum
{
	RESUME_DIALG_BTN_RESUME_TYPE,
	RESUME_DIALG_BTN_TERMINATE_TYPE,
	//...
	RESUME_DIALG_BTN_RETURN_TYPE,
} RESUME_DIALG_BTN_TYPE;
//!define different open java file result
typedef enum
{
	MMI_JAVA_OPEN_RESULT_FALSE = 0,
	MMI_JAVA_OPEN_RESULT_TRUE,
	MMI_JAVA_OPEN_RESULT_TOO_BIG_FAIL
	//please add other fail condition if you need

}MMI_JAVA_OPEN_RESULT_E;


typedef enum
{
	MMI_PLATFORM_REQUEST_HTTP = 0,
	MMI_PLATFORM_REQUEST_TEL,
	MMI_PLATFORM_REQUEST_MAX,
}PLATFORMREQUEST_TYPE;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :  get current active sim
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIJAVA_GetAcitveSim(void);

/*****************************************************************************/
//  Description :map native key to ajsc key code
//  Global resource dependence : none
//  Author: 
//  Note: nativeCode     native key code
/*****************************************************************************/
PUBLIC int32 MMIJAVA_NativeKeyToKJKey(int32 nativeCode);
/*****************************************************************************/
//  Description :start a java application
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC AJ_S32 MMIJAVA_jwe_run(int32 suiteId, int32 midletId );
/*****************************************************************************/
//  Description :auto run midlet
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
PUBLIC int32 MMIJAVA_ASA_autorunApp(int32 suiteId, int32 midletId);
/*****************************************************************************/
//  Description :?㊣?車∩車jad???t??DD
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_jwe_launchJad(const wchar* jadName, const wchar* jarName );
/*****************************************************************************/
//  Description :?㊣?車∩車jar???t??DD
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_jwe_launchJar( const wchar* jarName );
/*****************************************************************************/
//  Description :get current key state
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
PUBLIC uint32 MMIJAVA_GetKeyState(void);
/*****************************************************************************/
//  Description :ajsc key event handler
//  Global resource dependence : none
//  Author: 
//  Note: type     key event type     keyCode  key code id
/*****************************************************************************/
PUBLIC void MMIJAVA_sendKeyEvent(int32 type, int32 keyCode) ;
/*****************************************************************************/
//  Description :JVM state notification callback function.
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
PUBLIC void MMIJAVA_jwe_vmEventHandler(AJ_S32 eventType);
/***************************************************************************/
// Description:MMIJAVA_NET_init  
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
PUBLIC BOOLEAN MMIJAVA_NET_init(void);
/***************************************************************************/
// Description:free allocated resources function.
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
PUBLIC void MMIJAVA_NET_fini(void);
/*****************************************************************************/
//  Description : Get java net setting index 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIJAVA_GetNetSettingIndex(void);
/***************************************************************************/
// Description:free kjava_socket_polling_timeOut_process
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************/
PUBLIC void kjava_socket_polling_timeOut_process(uint8 timerID, uint32 param);

/*****************************************************************************/
//  Description : active pdp for JAVA 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_PDPActive(void);


/*****************************************************************************/
//  Description : active pdp for java 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVAHandlePDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : MMIJAVA_IsInitialize
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsInitialize(void);
/*****************************************************************************/
//  Description : Ajava_Initiliztion_ia
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Ajava_Initiliztion_Special(void);
/*****************************************************************************/
//  Description : Ajava_Finalize_ia
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Ajava_Finalize_Special(void);
/*****************************************************************************/
//  Description : Ajava_Finalize_ia
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Ajava_Finalize_ia(void);
/*****************************************************************************/
//  Description : Ajava_Initiliztion_ia
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Ajava_Initiliztion_ia(void);
/*****************************************************************************/
//  Description :the callback function for starting the native input method
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_jwe_eimeStartupImpl(AJMS_EimeData *data);
/*****************************************************************************/
// 	Description : the callback function for finishing the native input method. 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_jwe_eimeTerminateImpl(void);
/*****************************************************************************/
//  Description : Ajava_Initiliztion_UDP
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
BOOLEAN Ajava_Initiliztion_UDP(void);
/*****************************************************************************/
//  Description : Ajava_Finalize_UDP
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
BOOLEAN Ajava_Finalize_UDP(void);
/*****************************************************************************/
//  Description : Ajava_Initiliztion_COM
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
BOOLEAN Ajava_Initiliztion_COM(void);
/*****************************************************************************/
//  Description : Ajava_Finalize_COM
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
BOOLEAN Ajava_Finalize_COM(void);
/***************************************************************************/
// Description: AJAVA_KVEMain
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************/
PUBLIC void AJAVA_KVEMain(uint32 argc, void* argv);      

/***************************************************************************/
// Description: comm read thread's main
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************/  
void AJAVA_COMMain(uint32 argc, void* argv);  
/*****************************************************************************/
//  Description : set preinstall midlet suit id
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetPreinstallId(uint32 suit_id);
/*****************************************************************************/
//  Description : ini preinstall midlet suit id
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_initPreinstallID(void);
//***********************************************************************************************
//  Description:update screen
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
PUBLIC void MMIJAVA_gfx_update(void);
/*****************************************************************************/
//  Description :JVM state notification callback function.
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
PUBLIC void MMIJAVA_TCK_vmEventHandler(int32 eventType);

PUBLIC void MMIJAVA_MMAPI_init(void);

PUBLIC void MMIJAVA_MMAPI_fini(void);

PUBLIC void MMIJAVA_Start_initializeResource();

PUBLIC void MMIJAVA_pauseVM_external(mmijava_state_enum pause_level);

PUBLIC void MMIJAVA_resumeVM_external(void);

PUBLIC void MMIJAVA_stopVM_external(void);
/*****************************************************************************/
//  Description :MMIJAVA_backgroundVM_external
//  Global resource dependence : none
//  Author: 
//  Note: make jvm to background state
/*****************************************************************************/
PUBLIC void MMIJAVA_backgroundVM_external();

/*****************************************************************************/
//  Description :MMIJAVA_PauseBackgroundJVM
//  Global resource dependence : none
//  Author: 
//  Note: make jvm to pause in background state
/*****************************************************************************/
PUBLIC void MMIJAVA_PauseBackgroundJVM( void );

/*****************************************************************************/
//  Description :MMIJAVA_ResumeBackgroundJVM
//  Global resource dependence : none
//  Author: 
//  Note: make jvm resume in background state
/*****************************************************************************/
PUBLIC void MMIJAVA_ResumeBackgroundJVM( void );

PUBLIC void MMIJAVA_initConnPool(void);
PUBLIC int  MMIJAVA_Add2SMSReceiveQue(SMS_Message *argv);

PUBLIC uint32 MMIAPIJAVA_Tm2Second( 
                      uint32 tm_sec, 
                      uint32 tm_min, 
                      uint32 tm_hour,
                      uint32 tm_mday,
                      uint32 tm_mon, 
                      uint32 tm_year);

PUBLIC uint32 MMIAPIJAVA_GetCurTime(void);

/*****************************************************************************/
//  Description :MMIJAVA_GetSuspendJAVAFlag
//  Global resource dependence : none
//  Author: murphy.xie
//  Note: get the s_b_enableSuspendJAVA
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_GetSuspendJAVAFlag(void);

PUBLIC void MMIJAVA_FlashBackLightStart(
                uint32 count, 
                uint32 delay
        );

PUBLIC void MMIJAVA_FlashBackLightStop(void);

PUBLIC uint32 MMIJAVA_GetCurTime(void);


/*****************************************************************************/
//  Description : MMIJAVA_GetNetworkType
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:get network type
/*****************************************************************************/
PUBLIC MMIJAVA_NETWORK_TYPE_E MMIJAVA_GetNetworkType(void);
PUBLIC BOOLEAN MMIJAVA_RegSMSPortCallback(int32 port, SMS_CALLBACK JavaSMSCallback);
PUBLIC BOOLEAN MMIJAVA_UnRegSMSPortCallback(int port);
PUBLIC void MMIJAVA_SetJavaRuning(BOOLEAN  b_running);

/*****************************************************************************/
//  Description : MMIJAVA_SetIsDisplayLogo
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsDisplayLogo(BOOLEAN is_display);
/*****************************************************************************/
//  Description : MMIJAVA_SetIsStillDisplayLogo
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsStillDisplayLogo(BOOLEAN is_still_display);
/*****************************************************************************/
//  Description :MMIJAVA_IsStillDisplayLogo
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsStillDisplayLogo(void);
/*****************************************************************************/
//  Description :MMIJAVA_IsDisplayLogo
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsDisplayLogo(void);

/*****************************************************************************/
// 	Description : MMIJAVA_SetSuspendJAVAFlag
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:set suspend flag
/*****************************************************************************/
PUBLIC void MMIJAVA_SetSuspendJAVAFlag(BOOLEAN b_enable);

/*****************************************************************************/
// 	Description : MMIJAVA_GetIntallInfo
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:get global install info
/*****************************************************************************/
PUBLIC JAVA_INSTALL_INFO_T*  MMIJAVA_GetIntallInfo(void);
/*****************************************************************************/
// 	Description : MMIJAVA_SetIsJavaEnablePause
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetJavaIsEnablePause(BOOLEAN is_enable);
/*****************************************************************************/
// 	Description : MMIJAVA_GetJavaIsEnablePause
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_GetJavaIsEnablePause(void);

/*****************************************************************************/
//  Description : mediaCtrl_lock_device_multimedia
//  Global resource dependence : none
//  Author: 
//  Note: MMIAUDIO_PauseOtherBgPlay,when java play music
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_lock_device_multimedia(void);

/*****************************************************************************/
//  Description : mediaCtrl_unlock_device_multimedia
//  Global resource dependence : none
//  Author: 
//  Note: MMIAUDIO_ResumeOtherBgPlay,when java play music
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_unlock_device_multimedia(void);
/*****************************************************************************/
// 	Description : MMIJAVA_MakeCall
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_MakeCall(void);
/*****************************************************************************/
// 	Description : MMIJAVA_GetPlatformVolume
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_GetPlatformVolume(void);

// 	Description : MMIJAVA_AdjustVolume
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AdjustVolume(int32 step);

/*****************************************************************************/
// 	Description : MMIJAVA_SetIsJavaEnablePause
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_GetJavaIfRunningByMMI(void);
/*****************************************************************************/
// 	Description : MMIJAVA_SetJavaIfContinueInstall
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetJavaIfContinueInstall(BOOLEAN is_flag);
// 	Description : MMIJAVA_CalculateActualVolume
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_CalculateActualVolume(int32 mastervolume ,int32 playervolume);
/**************************************************************************************************/
// Description:MMIJAVA_ContinueLocalInstall
// Date:20110124
// Author:huangjc
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_ContinueInstall(void);
/*****************************************************************************/
//  Description :MMIJAVA_ContinuePush
//  Global resource dependence : none
//  Author: 
//  Note: continue push after jvm exit 
/*****************************************************************************/
PUBLIC void MMIJAVA_ContinuePush(void);
/*****************************************************************************/
// 	Description : MMIJAVA_SetIsJavaEnablePause
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetJavaIfRunningByMMI(BOOLEAN is_flag);

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register java module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_RegAppletInfo(void);

/*****************************************************************************/
//  Description : Stop java applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopApplet(BOOLEAN is_minimize);

#endif

#ifdef VIRTUAL_KEY_BOARD
PUBLIC void MMIJAVA_SetForm1ShowOrHide(BOOLEAN  flag);
#endif


/*****************************************************************************/
//  Description :MMIJAVA_OpenMidletWindow
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenMidletWindow(JAVA_INSTALL_INFO_T *install_info_ptr);


#ifdef JAVA_OTA_SUPPORT
/*****************************************************************************/
//  Description :MMIJAVA_OTAInstallJad
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
void MMIJAVA_OTAInstallJad( wchar * jadName, uint32  mime_type,char *url);

/*****************************************************************************/
//  Description :MMIJAVA_OTAInstallJar
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
void MMIJAVA_OTAInstallJar(wchar *file_name, uint32  mime_type,char *url);

/*****************************************************************************/
//  Description : MMIJAVA_OpenCheckWin
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenCheckWin(void);

/****************************************************************************
*o‘那y?“辰?:MMIJAVA_OTAInstall
*o‘那y?米?¯:
*2?那y?米?¯:
*﹞米???米:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
PUBLIC void MMIJAVA_OTAInstall(wchar * fileName,uint8 *jaddata, int32 jadlen, int8 *url, BOOLEAN is_jad);
#endif

/*****************************************************************************/
//  Description : MMIJAVA_SetIsDelJava
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsDelJava(BOOLEAN is_deljava);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif 

