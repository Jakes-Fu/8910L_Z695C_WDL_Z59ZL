/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

/*****************************************************************************
** File Name:      mmijava_wintable.c                                        *
** Description:    This file is used to describe digit camera                *
******************************************************************************/

#ifndef _MMIJAVA_WINTAB_C_
#define _MMIJAVA_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "jblend_config.h"

#include "os_api.h"
#include "mn_type.h"
#include "ajms_ota.h"
#include "ajms_ab.h"
#include "ajms_mss.h"

#include "mmi_modu_main.h"

#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiudisk_export.h"
#include "mmipub.h"
#include "mmk_app.h"
#include "guiwin.h"
#include "guirichtext.h"
#include "mmisms_text.h"
#include "mmitv_out.h"
#include "caf.h"
#include "block_mem_def.h"
#include "block_mem.h"
#include "kjava_sys_gfx_font_intra.h"
#include "amuievent.h"
#include "mmijava_internal.h"
#include "mmifmm_export.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmijava_nv.h"
#include "jwe_log.h"
#include "ajms_cfg.h"
#include "ajms_arc.h"
#include "ajms_cfg_eime.h"
#include "kjava_sys_fs_dev.h"
#include "ajms_ab.h"
#include "ajsc_httplib.h"
#include "mmk_timer.h"

#include "jblend_platform.h"
#include "mediacontrol.h"

#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef BROWSER_SUPPORT_NETFRONT
#include "mmiwww_text.h"
#include "mmiwww_export.h"
#include "mmidl_export.h"
#endif
#ifdef BROWSER_SUPPORT_DORADO
#include "browser_text.h"
#endif
#include "jkvmbackground.h"
#include "jcutf8.h" //for  jcutf8touni
#include  "mmijava_export.h"
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#include "kjava_sys_serial.h" //for kjava_serial_init
#include "mmiidle_export.h" //for status icon

#include "guiform.h"
#include "guilabel.h"
#include "guidropdownlist.h"

#include "aamsvmctl.h" //for aamsvmkill
#include "guires.h" //for guires_getimgwidthheight
#include "mmicc_export.h"

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

#if defined(DYNAMIC_MAINMENU_SUPPORT)
#include "mmi_mainmenu_synchronize.h"
#endif

#if defined MMI_GPRS_SUPPORT
#include "mmiconnection_export.h"   
#endif
#include "guibutton.h" 
#include "mmi_nv.h" 
#ifdef JAVA_SUPPORT_SHRINK_RAM
#include "kjava_shrink_ram.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MIDLET_ATTR_NUM 29
#define MMIJAVA_MIDLET_ARR 5
#define MMIJAVA_SUFFIX "*.jad"
#define MMIJAVA_MEMSTR_MAX_LEN       12
#define MMIJAVA_DISPLAY_DATE_LEN     20
#define MMIJAVA_INSTALL_JAR_MAX_SIZE (2*1024*1024 )

//OTA
#define AJMS_IGNORE_BUFFER_SIZE     (5*1024)
#define AJMS_NOTIFY_PARAM            9998
#define OTA_NOTIFY_TIMEOUT           4000

//MORE SPACE  FOR INSTALL
//#define  INSTALL_MORE_SAPCE_SIZE    (5*1024)
#define  JAVA_DOWNLOAD_DEFAULT_URL_LEN    20

#define MMIJAVA_EXIT_WAIT_COUNT             20

#ifdef MMI_TASK_MANAGER
#define MMK_CreateJavaWin( _WIN_TABLE_, _ADD_DATA_ )  OpenJavaWin( _WIN_TABLE_, _ADD_DATA_ )
#else
#define MMK_CreateJavaWin   MMK_CreateWin
#endif

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

extern  MMIJAVA_WAP_INFO    g_java_wap_info;
extern  BOOLEAN             is_platform_request;
extern  uint8*              g_java_call_info_ptr;
	
//+CR247820
#ifdef  JAVA_SUPPORT_MEDIA_CAMERA
extern JKSint32 visialbe_id ;
#endif
//-CR247820

#ifdef KVM_ENABLE_GAMECANVAS_OPT
extern uint8 m_Gamecanvas_timer;
extern void JVM_UpdateGameCanvasScreenTimer(void * param)	;
#endif

#ifdef USE_LOGFILE
extern SFS_HANDLE yuyulog_ptr;
#endif
#ifdef CMCC_JAVA_V41_SUPPORT
extern int current_running_midlet_domain;
#endif

#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
BOOLEAN g_jvm_in_background_by_mmi = FALSE;
#endif

BOOLEAN g_jvm_is_auto_tck_mode = FALSE;

BOOLEAN g_jvm_is_running_by_mmi = FALSE; //20100907
JAVA_INSTALL_INFO_T  s_install_info = {0};

BOOLEAN g_jvm_hold_lcd = FALSE;//java if hold lcd

PUBLIC BOOLEAN g_b_needContinueLocalInstall = FALSE;

//extern BOOLEAN MMIAPIFMM_GetDeviceTypeStatus(MMIFILE_DEVICE_E memory_type);

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN g_jvm_is_running = FALSE;
BOOLEAN s_b_force_killed = FALSE;//20100907
LOCAL BOOLEAN s_ota_notify_is_success = TRUE; //用来标志，ota notify是否已结束

LOCAL AJMS_InstallResult s_local_install_result = {0};
LOCAL AJMS_CheckResult s_local_check_result = {0};
LOCAL AJMS_Suite*  s_suiteList = PNULL;   // MIDlet suite
LOCAL MMIJAVA_INSTALL_DATA_INFO_T *s_install_data_info_ptr = PNULL;   
//LOCAL MN_DUAL_SYS_E s_java_dual_sys = MN_DUAL_SYS_1;
LOCAL BOOLEAN s_b_cancel_install = FALSE;
LOCAL BOOLEAN s_is_display_logo = TRUE;
LOCAL BOOLEAN s_is_still_display_logo =TRUE;
LOCAL BOOLEAN s_b_enable_pasue = TRUE;
LOCAL BOOLEAN s_is_close_browser = FALSE;//表示启动浏览器后，是否由java 模块关闭浏览器

LOCAL  MMIJAVA_CONTINUE_OTA_INSTALL_T s_needContinueOTAInstall= {0};
//LOCAL  wchar g_wchar_ContinueOTAInstall_jad_backup_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
LOCAL  wchar* g_wchar_ContinueOTAInstall_jad_backup_path_ptr = PNULL;

LOCAL  int32 s_lastkeypressed = -1;

#ifdef JAVA_OTA_SUPPORT
//wchar OTA_JAD_FileName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
//wchar OTA_JAR_FileName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
wchar* OTA_JAD_FileName_ptr = PNULL;
wchar* OTA_JAR_FileName_ptr = PNULL;

int OTA_DELETE_JAD =0;
int OTA_DELETE_JAR =0;
#endif	
#ifdef SELECT_INSTALL_DISK_SUPPORT
//#define MMIJAVA_DISK_NUM 3
LOCAL MMIFILE_DEVICE_E s_device_arr[MMI_DEVICE_NUM]={0} ;
LOCAL int32 current_item =0;
LOCAL BOOLEAN SysDiskHiddenOn = FALSE;
#endif

#ifdef PREINSTALL_JAVA_SUPPORT
uint32  s_preinstall_id = 0;  //保存预安装应用的id号
#endif
#ifdef MMI_JAVA_NW_V2
int32 g_active_suite_id = 0;
#endif
#ifdef FS_PROFILE
int  g_vm_exit = 3 ;
#endif
int g_runningTckLevel = 0;
#ifdef JAVA_DM_SUPPORT
LOCAL int installfromdm =0;
#endif

#ifdef VIRTUAL_KEY_BOARD
LOCAL  BOOLEAN s_form1_display_flag = FALSE;
LOCAL  BOOLEAN s_form2_display_flag = FALSE;
#endif

LOCAL AJ_U8  s_java_installtoPath[20] = "gfs://java";

LOCAL uint8 s_sim_selected_account_index[MMI_DUAL_SYS_MAX] = {0};    //the selected wap account index of sim1/2/3/4

#ifdef MMI_TASK_MANAGER	
/*****************************************************************************/
//  Description : the process message function of the java
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  JavaApplet_HandleEvent(    
            IAPPLET_T*          iapplet_ptr,
            MMI_MESSAGE_ID_E    msg_id, 
            DPARAM              param
            );

typedef struct 
{
    CAF_APPLET_T caf_applet;
    uint32  user_data;
}MMIJAVA_APPLET_T;   //vt applet定义

LOCAL MMIJAVA_APPLET_T *s_java_applet_instance = PNULL; 
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmijava_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_JAVA_APPLET_ID), sizeof(MMIJAVA_APPLET_T), JavaApplet_HandleEvent , 
         IMAGE_SECMENU_ICON_PASTIME_JAVA, TXT_JAVA},
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmijava_applet_info = 
{
    s_mmijava_applet_array,
    ARR_SIZE(s_mmijava_applet_array)
};
#endif

/**--------------------------------------------------------------------------*
**                         CONST DEFINITION                                *
**--------------------------------------------------------------------------*/
const char *s_midlet_attr[MIDLET_ATTR_NUM] =
{
	"MIDlet-1",
	"MIDlet-2",
	"MIDlet-3",
	"MIDlet-4",
	"MIDlet-5",
	"MIDlet-6",
	"MIDlet-7",
	"MIDlet-8",
	"MIDlet-9",
	"MIDlet-10",
	"MIDlet-Name",
	"MIDlet-Version",
	"MIDlet-Vendor",
	"MIDlet-Jar-Size",
	"MIDlet-Jar-URL",
	"MIDlet-Description",
	"MIDlet-Icon",
	"MicroEdition-Configuration",
	"MicroEdition-Profile",
	"MIDlet-Data-Size",
	"MIDlet-Delete-Confirm",
	"MIDlet-Info-URL",
	"MIDlet-Install-Notify",
	"MIDlet-Delete-Notify",
	"MIDlet-Permissions",
	"MIDlet-Permissions-Opt",
	"MIDlet-Permissions-opt",
	"MIDlet-Jar-RSA-SHA1",
	"MIDlet-Certificate"
};
const char *s_midlet_5_attr[MMIJAVA_MIDLET_ARR] =
{
	"MIDlet-Name",
	"MIDlet-Version",
	"MIDlet-Vendor",
	"MIDlet-Jar-Size",
	"MIDlet-Jar-URL",
};
const OTANotifyStatusCode  s_StatusCode[STATUS_CODE_ARRAY_SIZE] = 
{
    {900,"900 Success"},
    {901,"901 Insufficient Memory"},
    {902,"902 User Cancelled"},
    {903,"903 Loss of Service"},
    {904,"904 JAR size mismatch"},
    {905,"905 Attribute Mismatch"},
    {906,"906 Invalid Descriptor"},
    {907,"907 Invalid JAR"},
    {908,"908 Incompatible Configuration or Profile"},
    {909,"909 Application authentication failure"},
    {910,"910 Application authorization failure"},
    {911,"911 Push registration failure"},
    {912,"912 Deletion Notification"} 
};

LOCAL char*  ignoreBuf = PNULL;
LOCAL char*  JARBuffer = PNULL;
LOCAL uint8 s_ota_notify_timer_id = 0;

#ifdef CMCC_JAVA_V41_SUPPORT 
static BOOLEAN jad_keep_background_running =FALSE;
BOOLEAN keep_background_running = FALSE;
static const AJ_U8* CKB_KEY = (AJ_U8*)"CMCC-Keep-Background";
#endif
LOCAL       int ams_delet_ok=0;
LOCAL       const char* s_default_download_url = "http://wap.baidu.com";

extern int32 SCI_MEMCMP(void * _BUF1, void * _BUF2, uint32 _SIZE);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void ResumeDialg_OnBTN(RESUME_DIALG_BTN_TYPE btn_type);

#ifdef DYNAMIC_MAINMENU_SUPPORT

/*****************************************************************************/
//  Description : MMIJAVA_AddSuiteToMainmenu
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void MMIJAVA_AddSuiteToMainmenu(int32 suite_id,GUIMENU_FILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : MMIJAVA_DeleteSuiteRecordInMainmenu
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void MMIJAVA_DeleteSuiteRecordInMainmenu(int32 suite_id);

/*****************************************************************************/
//  Description : MMIJAVA_OpenJAVAFromMainMenu
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void MMIJAVA_OpenJAVAFromMainMenu(void* param1_ptr, void* param2_ptr);

/*****************************************************************************/
//  Description : MMIJAVA_DeleteSuiteFromMainmenu
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIJAVA_DeleteSuiteFromMainmenu(void* param1_ptr, void* param2_ptr);
#endif
/*****************************************************************************/
//  Description : local install jad reader for java install
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL AJ_S32 InstallJadDataReader(
                                            void *buffer, 
                                            AJ_S32 offset, 
                                            AJ_S32 size
                                            );                                        
/*****************************************************************************/
//  Description : local install jar reader for java install
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL AJ_S32 InstallJarDataReader(
                                            void *buffer, 
                                            AJ_S32 offset, 
                                            AJ_S32 size
                                            );

LOCAL BOOLEAN MMIJAVA_SetNetSettingInfo(int32 curSuiteId,MMIJAVA_LINK_SETTING_T setting_info);
LOCAL MMIJAVA_LINK_SETTING_T MMIJAVA_GetNetSettingInfo(int32 curSuiteId);
#ifdef JAVA_DM_SUPPORT

LOCAL void jblendia_dm_init_id(void);
LOCAL void DM_JAVAHandleScomoCallback(int result);
#endif

#ifdef CMCC_JAVA_V41_SUPPORT
LOCAL void get_CMCC_extention_info(int midlet_suite_id);

#ifdef JAVA_OTA_DELETE_REPORT_SUPPORT
    /* Begin: send MIDlet-Delete-Notify if this property exists in jad or MANIFEST.MF */
LOCAL void jblendia_send_delete_report(void);
    /* End: send MIDlet-Delete-Notify if this property exists in jad or MANIFEST.MF */
#endif
#endif

/**************************************************************************************/
// Description:List all of the midlet suite in MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID ListBox
// Date:2007-06-08
// Author:huangjc
// Note:
//************************************************************************************/
LOCAL BOOLEAN CreateSuitListBox(int32 item_index);
/**************************************************************************************/
// Description:Set item text with midlet name in MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID ListBox
// Date:2010-07-08
// Author:ivy.Su
// Note:
//************************************************************************************/
LOCAL BOOLEAN AppendOneListItemWithSuiteName(
                                       uint16           item_index,     //in:item索引
                                       MMI_CTRL_ID_T    ctrl_id         //in:control id
                                       );
/**************************************************************************************/
// Description:Set item icon raw data buffer in MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID ListBox
// Date:2010-07-08
// Author:ivy.Su
// Note:
//************************************************************************************/
LOCAL BOOLEAN DrawListItemWithIconData(
                                      uint16           item_index,     //in:item索引
                                      uint16           content_index,  //in:item内容索引,text or anim etc.
                                      MMI_CTRL_ID_T    ctrl_id         //in:control id
                                      );

/**************************************************************************************/
// Description: Get Midlet suite icon data buffer size
// Date:2010-07-08
// Author:ivy.Su
// Note:
//************************************************************************************/
LOCAL int GetMidletSuiteIconSize(AJ_S32 suiteId, AJ_S32 midletId);
/**************************************************************************************************/
// Description:List all of the midlets of a suite in MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID ListBox
// Date:2007-06-08
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL void CreateMidletListBox(int32 suitid);
/**************************************************************************************************/
// Description:jblendia_setMidletWinTitle
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL void jblendia_setMidletWinTitle(int32 cursuitid);
/**************************************************************************************************/
// Description:HandleJavaAMSWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA 应用列表窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

/**************************************************************************************************/
// Description:HandleJavaAMSOptionWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA 应用选项窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSOptionWinMsg(
                                             MMI_WIN_ID_T        win_id,    
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             );



/**************************************************************************************************/
// Description:HandleJavaAMSMidletListWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA MIDlet列表窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSMidletListWinMsg(
                                                 MMI_WIN_ID_T        win_id,    
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 ); 

/**************************************************************************************************/
// Description:SetJavaInfoDetail
// Date:2007-06-02
// Author:huangjc
// Note:设置JAVA 应用信息，获得JAVA应用的名称、版本、服务提供商、大小、
//         域等信息
//*************************************************************************************************/
LOCAL void SetJavaInfoDetail(MMI_CTRL_ID_T ctrl_id, int32   cursuitid);
/*****************************************************************************/
//  Description : add one detail item
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void AddOneDetailItem(
                            MMI_CTRL_ID_T   ctrl_id,
                            MMI_TEXT_ID_T   label_title,
                            MMI_TEXT_ID_T   label_name,
                            uint16          *index_ptr,
                            wchar           *detail_ptr,
                            uint32          detail_len
                            );
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:jian.ma
// Note: 
/*****************************************************************************/
LOCAL void SetCommonCharacter( GUIRICHTEXT_ITEM_T* p_item );

#if defined MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description : set sim network account label text
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void SetSimNetAccountLabelText(MMI_WIN_ID_T win_id, int32 cursuiteid);

/*****************************************************************************/
//  Description : set sim's network accout index setting for wap setting
//  Global resource dependence : s_nv_setting
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void JavaSetNetIndexSettingCallback(MN_DUAL_SYS_E dual_sys, uint32 index);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void JavaHandleSettingAccount(int32 curSuiteId, MN_DUAL_SYS_E dual_sys);
#endif

/**************************************************************************************************/
// Description:HandleJavaAMSOptionInfoWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA 应用信息窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E  HandleJavaAMSOptionInfoWinMsg(
                                                  MMI_WIN_ID_T   win_id, 
                                                  MMI_MESSAGE_ID_E  msg_id, 
                                                  DPARAM                param
                                                  );

/**************************************************************************************************/
// Description:HandleJavaNetSelectWinMsg
// Date:20100321
// Author:murphy.xie
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaNetSelectWinMsg(
                                                MMI_WIN_ID_T	win_id, 
                                                MMI_MESSAGE_ID_E	msg_id, 
                                                DPARAM				param
                                                );
/**************************************************************************************************/
// Description:HandleJavaPauseDialgWinMsg
// Date:2010-05-25
// Author:siyuan
// Note:JAVA 暂停窗口
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaPauseDialgWinMsg(
                                                 MMI_WIN_ID_T        win_id,    
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 );

/**************************************************************************************************/
// Description:HandleJavaPauseDialgWinMsg
// Date:2010-05-25
// Author:siyuan
// Note:JAVA恢复窗口
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaResumeDialgWinMsg(
                                                 MMI_WIN_ID_T        win_id,    
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 );

/**************************************************************************************************/
// Description:HandleCheckWinMsg
// Date:2007-06-08
// Author:huangjc
// Note:Jad/Jar解析窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleCheckWinMsg(
                                     MMI_WIN_ID_T            win_id, 
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM                      param
                                     );

/**************************************************************************************************/
// Description:OpenJavaFile
// Date:2007-06-08
// Author:huangjc
// Note:尝试打开JAVA应用安装文件，判断该文件是否完好无损
//*************************************************************************************************/
LOCAL MMI_JAVA_OPEN_RESULT_E OpenJavaFile(wchar *full_name, MMIJAVA_JAR_FILE_INFO_T  *file_info_ptr);


/**************************************************************************************************/
// Description:SetJavaInstallInfo
// Date:2007-06-08
// Author:huangjc
// Note:设置JAVA安装详细信息(JAD描述信息窗口信息的获取)
//*************************************************************************************************/
LOCAL void SetJavaInstallInfo(AJMS_CheckResult check_result, MMI_CTRL_ID_T ctrl_id);


/**************************************************************************************************/
// Description:HandleInstallWinMsg
// Date:2007-06-08
// Author:huangjc
// Note:本地安装窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleInstallWinMsg(
                                       MMI_WIN_ID_T            win_id, 
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM                      param
                                       );

/**************************************************************************************************/
// Description:HandleInstallInfoWinMsg
// Date:2007-06-08
// Author:huangjc
// Note:本地安装信息处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E  HandleInstallInfoWinMsg(
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );

/**************************************************************************************************/
// Description:HandleLocalCheckResult
// Date:2007-06-08
// Author:huangjc
// Note:本地安装解析结果信息处理函数
//*************************************************************************************************/
LOCAL void HandleLocalCheckResult(MMIJAVA_INSTALL_DATA_INFO_T * install_data_ptr);


/**************************************************************************************************/
// Description:HandleJavaSubWinMsg
// Date:2007-06-08
// Author:huangjc
// Note:JAVA应用运行窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaSubWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

/**************************************************************************************************/
// Description:JavaDeleteSuite
// Date:2007-06-02
// Author:huangjc
// Note:删除JAVA应用，参数suite_id为要删除应用的ID号
//*************************************************************************************************/
LOCAL void JavaDeleteSuite(int32 suite_id);

/**************************************************************************************************/
// Description:HandleJavaPermitSetWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA应用权限设置窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaPermitSetWinMsg(
                                             MMI_WIN_ID_T            win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM                      param
                                             );

/**************************************************************************************************/
// Description:HandleJavaPermitOptWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA应用权限设置菜单选项窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaPermitOptWinMsg(
                                             MMI_WIN_ID_T            win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM                      param
                                             );

/**************************************************************************************************/
// Description:PermitOptAppendListItem
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL void PermitOptAppendListItem (
                                    MMI_CTRL_ID_T ctrl_id,  /* list box control ID*/
                                    MMI_TEXT_ID_T *plist,   /* pointer of list items*/
                                    uint8 count                     /* list max item num */
                                    );
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT

/*****************************************************************************/
//  Description :MMIJAVA_OTADeleteFile
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJAVA_OTADeleteFile(void);

/**************************************************************************************************/
// Description:MMIJAVA_OTADeleteDLFile
// Author:murphy.xie
// Date:2010.11.22
// Note:MMIJAVA_OTADeleteDLFile when init
//*************************************************************************************************/
LOCAL void MMIJAVA_OTADeleteDLFile(void);
/**************************************************************************************************/
// Description:MMIJAVA_OTASaveJavaFileInfo
// Author:murphy.xie
// Date:2010.11.22
// Note:sava ota download  for delete when install ok or fail
//*************************************************************************************************/
LOCAL MMI_RESULT_E MMIJAVA_OTASaveJavaFileInfo(MMIBROWSER_MIME_TYPE_E type);
/**************************************************************************************************/
// Description:MMIJAVA_OTAInstallJad_CallBackFunction
// Author:murphy.xie
// Date:2010.11.22
// Note:jad ota install call back
//*************************************************************************************************/
LOCAL void MMIJAVA_OTAInstallJad_CallBackFunction(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr);
/**************************************************************************************************/
// Description:MMIJAVA_OTAInstallJar_CallBackFunction
// Author:murphy.xie
// Date:2010.11.22
// Note:jar ota install call back
//*************************************************************************************************/
LOCAL void MMIJAVA_OTAInstallJar_CallBackFunction(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr);
/**************************************************************************************************/
// Description:HandleJavaDownLoadSetWinMsg
// Author:murphy.xie
// Date:2010.11.22
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaDownLoadSetWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

/*****************************************************************************/
//  Description :SetSelectNetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIJAVA_Download_Init(void);

/*****************************************************************************/
//  Description : MMIJAVA_SuiteUpdate
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIJAVA_SuiteUpdate(int32 suite_id);

#endif
/*****************************************************************************/
//  Description : Set JAVA net setting index 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
#ifdef MMI_JAVA_NW_V2
LOCAL void MMIJAVA_CreateNetworkSetting(int suitId);
LOCAL void MMIJAVA_DeleteNetworkSetting(int suitId);
#endif
/*****************************************************************************/
//  Description :append one line icon text list item
//  Global resource dependence : none
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(
                                     MMI_CTRL_ID_T       ctrl_id,
                                     GUIITEM_STYLE_E    item_style,
                                     MMI_TEXT_ID_T       left_softkey_id,
                                     MMI_IMAGE_ID_T      icon_id,
                                     const wchar*        wstr_ptr,
                                     uint16              wstr_len,                                 
                                     uint16              pos,
                                     BOOLEAN             is_update
                                     );



/****************************************************************************
*函数定义:LocalCheck
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL BOOLEAN LocalCheck(
                               const int32 data_size,
                               const uint8 *data_addr,
                               BOOLEAN is_jad
                               );
/****************************************************************************
*函数定义:LocalCheck
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL AJ_S32 Java_LocalCheckCallBack( 
                AJ_S32 total, 
                AJ_S32 step, 
                AJMS_CheckResult *result 
        );
/****************************************************************************
*函数定义:Java_LocalInstall
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL BOOLEAN Java_LocalInstall(
                                 int32 jad_data_size,
                                 const uint8 *jad_data_addr,      
                                 int32 jar_data_size,
                                 const uint8 *jar_data_addr, 
                                 const uint8 *jar_file_path,
                                 int32 old_id,
                                 int32 ams
                                 );
/****************************************************************************
*函数定义:Java_LocalInstall
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL AJ_S32 Java_LocalInstallCallBack( 
                AJ_S32 total, 
                AJ_S32 step, 
                AJMS_InstallResult *result);
/*****************************************************************************/
//  Description :  JavaCheckNeededProperty
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JavaCheckNeededProperty(AJ_U8 *jad_content, int32 jad_contenr_len);
/*****************************************************************************/
//  Description :  JavaCheck5MidletProperty
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JavaCheck5MidletProperty(AJ_U8 *jad_content, int32 jad_contenr_len);
/*****************************************************************************/
//  Description : to open java icon window
//  Global resource dependence : none
//  Author: louis 
//  Note:
/*****************************************************************************/
LOCAL void OpenInstallListWin(BOOLEAN is_open_again);

/*****************************************************************************/
//  Description : handle java select win
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectWinMsg(
                                      MMI_WIN_ID_T          win_id,     
                                      MMI_MESSAGE_ID_E      msg_id, 
                                      DPARAM                param
                                      );
/****************************************************************************
*函数定义:nofity server
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL BOOLEAN jwe_ota_notify(int32 statusCode, char* url, int32 urlLength);
/****************************************************************************
*函数定义:jwe_ota_download_callback
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
AJ_S32 jwe_ota_download_callback( AJ_S32 param, AJ_S32 status,  AJ_S32 value );
#ifdef JAVA_EVENT_ERROR_PROMPT_WIN
LOCAL void JAVA_Install_ErrorHandle(AJ_S32 otaCode);
#endif
#ifdef SELECT_INSTALL_DISK_SUPPORT
/**************************************************************************************************/
// Description:HandleJavaSelectInstallDiskWinMsg
// Author:hanker
// Note:JAVA 应用列表窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaSelectInstallDiskWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

/**************************************************************************************************/
// Description:List all of the midlet suite in CreateInstallDiskListBox ListBox
// Author:hanker
// Note:
//*************************************************************************************************/
LOCAL BOOLEAN CreateInstallDiskListBox(void);
/****************************************************************************
*函数定义:Java_LocalInstall
*函数说明:
*参数说明:
*返回值:
*hanker 
*****************************************************************************/
LOCAL BOOLEAN Java_LocalInstall_To(
					const char *installto,
                                 int32 jad_data_size,
                                 const uint8 *jad_data_addr,      
                                 int32 jar_data_size,
                                 const uint8 *jar_data_addr, 
                                 const uint8 *jar_file_path,
                                 int32 old_id,
                                 int32 ams
                                 );
#endif

#ifdef MMI_PDA_SUPPORT
/**************************************************************************************************/
// Description:HandleJavaAMSPopUpOptionWinMsg
// Date:
// Author:maryxiao
// Note:JAVA 应用选项弹出menu处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSPopUpOptionWinMsg(
                                             MMI_WIN_ID_T        win_id,    
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             );

#endif/*MMI_PDA_SUPPORT*/

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : Get JAVA Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIJAVA_APPLET_T *MMIJAVA_GetAppletInstance(void);

/*****************************************************************************/
//  Description : Create java reference win
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenJavaWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  );

/*****************************************************************************/
//  Description : start java applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StartApplet(void);

/**************************************************************************************************/
// Description:applet window message handler
// Date:
// Author:fen.xie
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAppletWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

#endif/*MMI_TASK_MANAGER*/


LOCAL MMI_RESULT_E  JavaHandleProcessWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

 MMI_TEXT_ID_T label_str[] = {
        TXT_SIM_SEL_SIM1,
        TXT_SIM_SEL_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        TXT_SIM_SEL_SIM3,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SIM_SEL_SIM4,
#endif
#endif
        };
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/

WINDOW_TABLE( MMIJAVA_AMS_WIN_TAB ) = 
{ 
    WIN_FUNC( (uint32)HandleJavaAMSWinMsg), 
    WIN_ID( MMIJAVA_AMS_WIN_ID ),
    WIN_TITLE(TXT_APP_MANAGER),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
WINDOW_TABLE( MMIJAVA_AMS_OPTION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleJavaAMSOptionWinMsg), 
    WIN_ID( MMIJAVA_AMS_OPTION_WIN_ID ),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_JAVA_AMS_OPTION, MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMIJAVA_AMS_POPUP_OPTION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleJavaAMSPopUpOptionWinMsg), 
    WIN_ID( MMIJAVA_AMS_POPUP_OPTION_WIN_ID ),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    END_WIN
};
#endif
WINDOW_TABLE( MMIJAVA_AMS_OPTION_INFO_WIN_TAB ) = 
{
 
    WIN_FUNC( (uint32)HandleJavaAMSOptionInfoWinMsg ),   
    WIN_ID( MMIJAVA_AMS_OPTION_INFO_WIN_ID ),      
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIJAVA_DETAIL_NAME_CTRL_ID),     
    END_WIN
};

//权限设置
//the menu of permit setting window
WINDOW_TABLE(MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_TAB) = 
{  
    WIN_FUNC((uint32)HandleJavaPermitSetWinMsg),    
    WIN_ID(MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_ID),    
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_JAVA_AMS_OPTION_PERMITION_SET, MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIJAVA_PERMISSION_OPT_WIN_TAB) = 
{
  
    WIN_FUNC((uint32)HandleJavaPermitOptWinMsg), 
    WIN_ID(MMIJAVA_PERMISSION_OPT_WIN_ID),   
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_JAVA_AMS_OPTION_PERMITION_SET, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIJAVA_PERMIT_OPT_LIST_CTRL_ID),
    END_WIN
};
WINDOW_TABLE( MMIJAVA_CHECK_WIN_TAB ) = 
{    
    WIN_FUNC( (uint32)HandleCheckWinMsg), 
    WIN_TITLE(TXT_JAVA_LOCAL_CHECK),
    WIN_ID( MMIJAVA_CHECK_WIN_ID ), 
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),//no full paint, just an empty window,add not full flag for 3D.
    //WIN_STYLE(WS_HAS_TRANSPARENT),
    END_WIN
};
WINDOW_TABLE( MMIJAVA_INSTALL_WIN_TAB ) = 
{   
    WIN_FUNC( (uint32)HandleInstallWinMsg),
    WIN_TITLE(TXT_COM_LOCALINSTALL),        
    WIN_ID( MMIJAVA_INSTALL_WIN_ID ),
    //WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),//no full paint, just an empty window,add not full flag for 3D.    
    END_WIN
};
WINDOW_TABLE( MMIJAVA_INSTALL_INFO_WIN_TAB ) = 
{    
    WIN_FUNC( (uint32)HandleInstallInfoWinMsg ),    
    WIN_ID( MMIJAVA_INSTALL_INFO_WIN_ID ),
    WIN_TITLE(STXT_DETAIL),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COM_LOCALINSTALL, TXT_NULL, STXT_RETURN),

    CREATE_RICHTEXT_CTRL(MMIJAVA_JAD_INFO_CTRL_ID),
    WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),//no full paint, just an empty window,add not full flag for 3D.
    END_WIN
};

WINDOW_TABLE( MMIJAVA_AMS_MIDLETLIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleJavaAMSMidletListWinMsg), 
    WIN_ID( MMIJAVA_AMS_MIDLETLIST_WIN_ID ), 
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


WINDOW_TABLE( MMIJAVA_MIDLET_WIN_TAB ) = 
{
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC( (uint32)HandleJavaSubWinMsg),
    WIN_HIDE_STATUS,
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    //WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),//no full paint, just an empty window,add not full flag for 3D.

    #ifdef  VIRTUAL_KEY_BOARD
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_KEYBOARD1_FORM),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_KEYBOARD1_LINE_ONE_FORM,MMIJAVA_KEYBOARD1_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_LS_OFF,MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY,MMIJAVA_KEYBOARD1_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_UP_OFF,MMIJAVA_KEYBOARD1_BOTTON_UP_KEY,MMIJAVA_KEYBOARD1_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_RS_OFF,MMIJAVA_KEYBOARD1_BOTTON_RIGHT_SOFTKEY,MMIJAVA_KEYBOARD1_LINE_ONE_FORM),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_KEYBOARD1_LINE_TWO_FORM,MMIJAVA_KEYBOARD1_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_NUM_OFF,MMIJAVA_KEYBOARD1_BOTTON_NUM_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_LEFT_OFF,MMIJAVA_KEYBOARD1_BOTTON_LEFT_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_DOWN_OFF,MMIJAVA_KEYBOARD1_BOTTON_DOWN_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_RIGHT_OFF,MMIJAVA_KEYBOARD1_BOTTON_RIGHT_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_OK_OFF,MMIJAVA_KEYBOARD1_BOTTON_OK_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_KEYBOARD2_FORM),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_KEYBOARD2_LINE_ONE_FORM,MMIJAVA_KEYBOARD2_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_LS_OFF,MMIJAVA_KEYBOARD2_BOTTON_LFFT_SOFTKEY,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM1_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_1,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM2_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_2,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM3_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_3,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM4_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_4,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM5_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_5,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM6_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_6,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_RS_OFF,MMIJAVA_KEYBOARD2_BOTTON_RIGHT_SOFTKEY,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_KEYBOARD2_LINE_TWO_FORM,MMIJAVA_KEYBOARD2_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_RETURN_OFF,MMIJAVA_KEYBOARD2_BOTTON_RETURN_KEY,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM7_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_7,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM8_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_8,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM9_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_9,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM0_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_0,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_ASTERISK_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_ASTERISK,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_POUND_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_POUND,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_OK_OFF,MMIJAVA_KEYBOARD2_BOTTON_OK_KEY,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
    #endif
    WIN_ID( MMIJAVA_MIDLET_WIN_ID ),
    END_WIN
};


WINDOW_TABLE( MMIJAVA_SELECT_NET_ENTER_POINT ) =
{
    WIN_FUNC((uint32)HandleJavaNetSelectWinMsg),
    WIN_ID(MMIJAVA_SELECT_NET_WIN_ID),
    WIN_TITLE(TXT_COMMON_LINKSETTING),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_SELECT_NET_FORM_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_SELECT_NET_FORM1_ID,MMIJAVA_SELECT_NET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_SELECT_NET_LABEL_NET_ID,MMIJAVA_SELECT_NET_FORM1_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIJAVA_SELECT_NET_DROPDOWNLIST_NET_ID,MMIJAVA_SELECT_NET_FORM1_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_SELECT_NET_FORM2_ID,MMIJAVA_SELECT_NET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_SELECT_NET_LABEL_SIM1_ID,MMIJAVA_SELECT_NET_FORM2_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIJAVA_SELECT_NET_LABEL2_SIM1_ID,MMIJAVA_SELECT_NET_FORM2_ID),
            //CHILD_DROPDOWNLIST_CTRL(TRUE,MMIJAVA_SELECT_NET_DROPDOWNLIST_SIM1_ID,MMIJAVA_SELECT_NET_FORM2_ID),
        #if defined(MMI_MULTI_SIM_SYS_DUAL)||defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_SELECT_NET_FORM3_ID,MMIJAVA_SELECT_NET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_SELECT_NET_LABEL_SIM2_ID,MMIJAVA_SELECT_NET_FORM3_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIJAVA_SELECT_NET_LABEL2_SIM2_ID,MMIJAVA_SELECT_NET_FORM3_ID),
            //CHILD_DROPDOWNLIST_CTRL(TRUE,MMIJAVA_SELECT_NET_DROPDOWNLIST_SIM2_ID,MMIJAVA_SELECT_NET_FORM3_ID),
        #endif
        #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_SELECT_NET_FORM4_ID,MMIJAVA_SELECT_NET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_SELECT_NET_LABEL_SIM3_ID,MMIJAVA_SELECT_NET_FORM4_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIJAVA_SELECT_NET_LABEL2_SIM3_ID,MMIJAVA_SELECT_NET_FORM4_ID),
            //CHILD_DROPDOWNLIST_CTRL(TRUE,MMIJAVA_SELECT_NET_DROPDOWNLIST_SIM3_ID,MMIJAVA_SELECT_NET_FORM4_ID),
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_SELECT_NET_FORM5_ID,MMIJAVA_SELECT_NET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_SELECT_NET_LABEL_SIM4_ID,MMIJAVA_SELECT_NET_FORM5_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIJAVA_SELECT_NET_LABEL2_SIM4_ID,MMIJAVA_SELECT_NET_FORM5_ID),
            //CHILD_DROPDOWNLIST_CTRL(TRUE,MMIJAVA_SELECT_NET_DROPDOWNLIST_SIM4_ID,MMIJAVA_SELECT_NET_FORM5_ID),
        #endif
    END_WIN    
};


WINDOW_TABLE( MMIJAVA_PAUSE_DIALOG_WIN_TAB ) = 
{
  
    WIN_FUNC( (uint32)HandleJavaPauseDialgWinMsg), 
    WIN_ID( MMIJAVA_PAUSE_DIALOG_WIN_ID ),
    WIN_TITLE(TXT_PAUSE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIJAVA_RESUME_DIALOG_WIN_TAB ) = 
{
  
    WIN_FUNC( (uint32)HandleJavaResumeDialgWinMsg), 
    WIN_ID( MMIJAVA_RESUME_DIALOG_WIN_ID ),
    WIN_TITLE(TXT_JAVA_RESUME_WIN_TITLE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#ifdef SELECT_INSTALL_DISK_SUPPORT
WINDOW_TABLE( MMIJAVA_APP_INSTALL_SELECT_WIN_TAB ) = 
{ 
    WIN_FUNC( (uint32)HandleJavaSelectInstallDiskWinMsg), 
    WIN_ID( MMIJAVA_APP_INSTALL_SELECT_WIN_ID ),
    WIN_TITLE(TXT_JAVA_INSTALL_SELECT_DISK),
#ifdef MMI_PDA_SUPPORT
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID),
#else
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID),
#endif
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
WINDOW_TABLE( MMIJAVA_APP_DOWNLOAD_SET_WIN_TAB ) = 
{ 
    WIN_FUNC( (uint32)HandleJavaDownLoadSetWinMsg), 
    WIN_ID( MMIJAVA_DOWNLOAD_WIN_ID ),
    WIN_TITLE(TXT_DOWNLOAD_SETTINGS),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_DOWNLOAD_SET_FORM_ID),
	    //GAME
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_DOWNLOAD_SET_GAME_LABEL_ID,MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIJAVA_MAX_DOWNLOAD_URL_LEN,MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID,MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID),
    	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
    	    CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_DOWNLOAD_ENTER,MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID,MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID),
    	    CHILD_OWNDRAW_CTRL(FALSE,MMIJAVA_DOWNLOAD_SET_GAME_ENTER_SPACE_ID,MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID,PNULL),
	    //EBOOK
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_DOWNLOAD_SET_EBOOK_LABEL_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIJAVA_MAX_DOWNLOAD_URL_LEN,MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID),
    	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_DOWNLOAD_ENTER,MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID),
    	    CHILD_OWNDRAW_CTRL(FALSE,MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_SPACE_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID,PNULL),
        //OTHER
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIJAVA_DOWNLOAD_SET_OTHER_LABEL_ID,MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIJAVA_MAX_DOWNLOAD_URL_LEN,MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID,MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID),
    	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID,MMIJAVA_DOWNLOAD_SET_FORM_ID),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_DOWNLOAD_ENTER,MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID,MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID),
    	    CHILD_OWNDRAW_CTRL(FALSE,MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_SPACE_ID,MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID,PNULL),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

#ifdef MMI_TASK_MANAGER
WINDOW_TABLE( MMIJAVA_APPLET_WIN_TAB ) = 
{
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC( (uint32)HandleJavaAppletWinMsg),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    WIN_ID( MMIJAVA_APPLET_WIN_ID ),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
 **                        LOCAL FUNCTION DEFINITION                         *
 **--------------------------------------------------------------------------*/
#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : MMIJAVA_AddSuiteToMainmenu
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void MMIJAVA_AddSuiteToMainmenu(int32 suite_id,GUIMENU_FILE_DEVICE_E device_type)
{
    GUIMAINMENU_DYNAMIC_INFO_T      dynamic_item_info = {0};
    AJ_U8*                          data_info_ptr = PNULL;  
    int32                           iNumOfSuite = 0;
    int32                           item_index = 0;
    int32                           suite_icon_size=  0;
    int32                           ret_value =  0;   
    uint16                          flag = 0;
    AJMS_Suite*                     suiteList_ptr = PNULL;
    uint16                          suite_name_len = 0;
    
    //SCI_TRACE_LOW:"MMIJAVA_AddSuiteToMainmenu enter suite id=%d,device_type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1251_112_2_18_2_22_10_13,(uint8*)"dd",suite_id,device_type);
    
    iNumOfSuite = ajms_ab_getSutieListCount( -1 );
    if (iNumOfSuite < 0 || iNumOfSuite > MAXJAVALISTSIZE)
    {
        return;//没有读取到item个数
    }
    
    suiteList_ptr = SCI_ALLOCA((uint32)iNumOfSuite*sizeof(AJMS_Suite));
    if(PNULL == suiteList_ptr)
    {
        //SCI_TRACE_LOW:"MMIJAVA_AddSuiteToMainmenu suiteList_ptr ALLOC FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1262_112_2_18_2_22_10_14,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(suiteList_ptr, 0, (uint32)iNumOfSuite*sizeof(AJMS_Suite));
    // get the information of all java applications
    if(ajms_ab_getSuiteList(-1 , suiteList_ptr, 0) < 0)
    {
        //SCI_TRACE_LOW:"MMIJAVA_AddSuiteToMainmenu ajms_ab_getSuiteList FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1270_112_2_18_2_22_10_15,(uint8*)"");
        return;
    }    

       
    for(item_index = 0; item_index < iNumOfSuite; item_index++)
    {
        if(suite_id == suiteList_ptr[item_index].id)
        {
            suite_name_len = strlen((char*)suiteList_ptr[item_index].suiteName);
            dynamic_item_info.dynamic_menu_info.text_len= GUI_UTF8ToWstr(dynamic_item_info.dynamic_menu_info.text,
                    GUIMENU_DYMAINMENU_STR_MAX_NUM,
                    (const uint8 *)suiteList_ptr[item_index].suiteName,
                    MIN(suite_name_len, GUIMENU_DYMAINMENU_STR_MAX_NUM));

            suite_icon_size = GetMidletSuiteIconSize(suiteList_ptr[item_index].id, -1);
            if(0 < suite_icon_size)
            {
                data_info_ptr = SCI_ALLOCA(suite_icon_size*sizeof(AJ_U8));/*lint !e737*/
                if(PNULL == data_info_ptr)
                {
                    //SCI_TRACE_LOW:"MMIJAVA_AddSuiteToMainmenu data_info_ptr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1291_112_2_18_2_22_10_16,(uint8*)"");
                    break;
                }
                ret_value = ajms_ab_getIcon(suiteList_ptr[item_index].id, AJMS_SUITE_ICON_ID, (AJ_U8 *)data_info_ptr, suite_icon_size);
                if (ret_value <= 0)
                {
                    //SCI_TRACE_LOW:"Test_Add_Java_item get the first midlet icons_suiteList"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1297_112_2_18_2_22_10_17,(uint8*)"");
                    ret_value = ajms_ab_getIcon(suiteList_ptr[item_index].id, 0, (AJ_U8 *)data_info_ptr, suite_icon_size);
                }

                if(ret_value > 0)
                {
                    dynamic_item_info.dynamic_menu_info.icon_ptr = data_info_ptr;
                    dynamic_item_info.dynamic_menu_info.icon_datasize = suite_icon_size;
                    dynamic_item_info.dynamic_menu_info.has_icon = TRUE;
                }
            }
            else
            {
                dynamic_item_info.dynamic_menu_info.has_icon = 0;
                
            }
            dynamic_item_info.dynamic_menu_info.link_function_ptr = MMIJAVA_OpenJAVAFromMainMenu;
            dynamic_item_info.dynamic_menu_info.delete_function_ptr = MMIJAVA_DeleteSuiteFromMainmenu;
            dynamic_item_info.dynamic_menu_info.param1ptr = suiteList_ptr[item_index].suiteName;
            dynamic_item_info.dynamic_menu_info.param2ptr = &flag;
            dynamic_item_info.dynamic_menu_info.parm1_size = suite_name_len*sizeof(AJ_U8);
            dynamic_item_info.dynamic_menu_info.parm2_size = sizeof(flag);
            MMIMENU_DyAppendItem(&dynamic_item_info, device_type, TRUE);

            if(PNULL != data_info_ptr)
            {
                SCI_FREE(data_info_ptr);
            }    
            
            break;
        }
   }

   SCI_FREE(suiteList_ptr);
}

/*****************************************************************************/
//  Description : MMIJAVA_DeleteSuiteInfoInMainmenu
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void MMIJAVA_DeleteSuiteRecordInMainmenu(int32 suite_id)
{
    GUIMAINMENU_DYNAMIC_INFO_T      dynamic_item_info = {0};
    int32                           iNumOfSuite = 0;
    int32                           item_index = 0;
    AJMS_Suite*                     suiteList_ptr = PNULL;
    uint16                          suite_name_len = 0;

    //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteRecordInMainmenu suite_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1345_112_2_18_2_22_10_18,(uint8*)"d",suite_id);

    if(suite_id < 0 )
    {
        return;
    }
    
    iNumOfSuite = ajms_ab_getSutieListCount( -1 );
    if (iNumOfSuite < 0 || iNumOfSuite > MAXJAVALISTSIZE)
    {
        return;//没有读取到item个数
    }
    suiteList_ptr = SCI_ALLOCA((uint32)iNumOfSuite*sizeof(AJMS_Suite));
    if(PNULL == suiteList_ptr)
    {
        //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteRecordInMainmenu suiteList_ptr ALLOC FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1360_112_2_18_2_22_10_19,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(suiteList_ptr, 0, (uint32)iNumOfSuite*sizeof(AJMS_Suite));
    // get the information of all java applications
    if(ajms_ab_getSuiteList(-1 , suiteList_ptr, 0) < 0)
    {
        //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteRecordInMainmenu ajms_ab_getSuiteList FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1368_112_2_18_2_22_10_20,(uint8*)"");
        return;
    }    

    for(item_index = 0; item_index < iNumOfSuite; item_index++)
    {
        if((suite_id) == suiteList_ptr[item_index].id)
        {
            suite_name_len = strlen((char*)suiteList_ptr[item_index].suiteName);
            dynamic_item_info.dynamic_menu_info.text_len
            = GUI_UTF8ToWstr(dynamic_item_info.dynamic_menu_info.text,
                    GUIMENU_DYMAINMENU_STR_MAX_NUM,
                    (const uint8 *)suiteList_ptr[item_index].suiteName,
                    MIN(suite_name_len, GUIMENU_DYMAINMENU_STR_MAX_NUM));

            dynamic_item_info.dynamic_menu_info.link_function_ptr = MMIJAVA_OpenJAVAFromMainMenu;
            MMIMENU_DyDeleteItem(&dynamic_item_info);
            break;
        }
    }
            
    SCI_FREE(suiteList_ptr);
}

/*****************************************************************************/
//  Description : MMIJAVA_OpenJAVAFromMainMenu
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void MMIJAVA_OpenJAVAFromMainMenu(void* param1_ptr, void* param2_ptr)
{ 
    static int32    cursuitid  = 0;
    AJMS_Suite*     suiteList_ptr = PNULL;
    int32           i  = 0;
    int32           iNumOfSuite     = 0;  
    BOOLEAN         find_flag = FALSE;
    int32           active_suite_index = -1;
    int32           run_suite_index = -1;

    if(param1_ptr == PNULL)
    {
        return;
    }
    //cursuitid =  *((int32 *)suit_id_ptr);
    //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu:param1_ptr g_active_suite_id=%s,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1411_112_2_18_2_22_10_21,(uint8*)"sd",param1_ptr,g_active_suite_id);

    //重启java 系统 
    Ajava_Initiliztion_Special();

    iNumOfSuite = ajms_ab_getSutieListCount( -1 );
    if(iNumOfSuite < 0 || iNumOfSuite > (MAXJAVALISTSIZE + 1) )
    {
        //提示
        //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu iNumOfSuite= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1420_112_2_18_2_22_10_22,(uint8*)"d",iNumOfSuite);
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_SUITE_NOT_EXIST, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        return;
    }

    
    suiteList_ptr = SCI_ALLOCA((uint32)iNumOfSuite*sizeof(AJMS_Suite));
    if(PNULL == suiteList_ptr)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_COMMON_NO_MEMORY, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu suiteList_ptr ALLOC FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1444_112_2_18_2_22_10_23,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(suiteList_ptr, 0, (uint32)iNumOfSuite*sizeof(AJMS_Suite));
    // get the information of all java applications
    if(ajms_ab_getSuiteList(-1 , suiteList_ptr, 0) < 0)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_SUITE_NOT_EXIST, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu ajms_ab_getSuiteList FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1460_112_2_18_2_22_11_24,(uint8*)"");
        SCI_FREE(suiteList_ptr);
        return;
    }

    for(i = 0;i < iNumOfSuite;i++)
    {
        if(0 == strncmp((char *)suiteList_ptr[i].suiteName,param1_ptr,strlen((char *)suiteList_ptr[i].suiteName)))
        {
            run_suite_index = i;
            find_flag = TRUE;
            //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu run_suite_index= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1471_112_2_18_2_22_11_25,(uint8*)"d",run_suite_index);
        }

        if(suiteList_ptr[i].id == g_active_suite_id)
        {
           active_suite_index = i;
           //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu active_suite_index = %d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1477_112_2_18_2_22_11_26,(uint8*)"d",active_suite_index);
           if(find_flag)
           {
               break;
           }
        }
    }
    
    if(!find_flag)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_SUITE_NOT_EXIST, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu ajms_ab_getSuiteList FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1495_112_2_18_2_22_11_27,(uint8*)"");
        SCI_FREE(suiteList_ptr);
        return;
    }
    
    if(MMIAPIJAVA_IsJavaRuning())
    {
        if(run_suite_index == active_suite_index)//如果是相同的应用
        {
            #ifdef MMI_TASK_MANAGER
            MMIAPIJAVA_StopApplet(FALSE);
            #endif
			//ResumeDialg_OnBTN(RESUME_DIALG_BTN_RESUME_TYPE);
            #ifdef UI_P3D_SUPPORT
            MMI_Disable3DEffect(MMI_3D_EFFECT_JAVA);
            #endif 		
            
            GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,GUILCD_GetInitAngle(GUI_MAIN_LCD_ID));

            MMK_CreateWin((uint32*)MMIJAVA_MIDLET_WIN_TAB, PNULL);      
            MMIJAVA_resumeVM_external();
        
        }
        else //prompt haS application runing,can't run this app
        {
            if(active_suite_index < 0)
            {
                SCI_FREE(suiteList_ptr);
                //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu active_suite_index < 0"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1523_112_2_18_2_22_11_28,(uint8*)"");
                return;
            }

            //for 变量声明
            {
            	MMI_STRING_T    totalcontent = {0};
            	MMI_STRING_T    textcontent = {0};

                wchar*          prompt_content_ptr = PNULL;
                wchar           suite_name[AJMS_MAX_ENTRY_NAME_LENGTH] = { 0 };
                uint32          suite_name_len = 0; 
                suite_name_len = GUI_UTF8ToWstr(suite_name,AJMS_MAX_ENTRY_NAME_LENGTH,
                (const uint8 *)suiteList_ptr[active_suite_index].suiteName,
                strlen((char*)suiteList_ptr[active_suite_index].suiteName)); 
                
                MMI_GetLabelTextByLang(TXT_JAVA_SUITE_IS_RUNING_PLEASE_STOP, &textcontent);
                prompt_content_ptr = SCI_ALLOCA((suite_name_len+textcontent.wstr_len+1+1)*sizeof(wchar));
                //+1 for 空格
                if(PNULL == prompt_content_ptr)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, 
                    TXT_COMMON_NO_MEMORY, 
                    TXT_NULL, 
                    IMAGE_PUBWIN_WARNING,
                    PNULL, 
                    PNULL,
                    MMIPUB_SOFTKEY_ONE,
                    PNULL);
                    //SCI_TRACE_LOW:"MMIJAVA_OpenJAVAFromMainMenu prompt_content_ptr alloc FAILED"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1552_112_2_18_2_22_11_29,(uint8*)"");

                    SCI_FREE(suiteList_ptr);

                    return;
                }
                
                SCI_MEMSET(prompt_content_ptr,0,(suite_name_len+textcontent.wstr_len+2)*sizeof(wchar));

                MMIAPICOM_Wstrncpy(prompt_content_ptr,suite_name,suite_name_len);
                MMIAPICOM_Wstrncpy(prompt_content_ptr+suite_name_len," ",1);/*lint !e64*/
                MMIAPICOM_Wstrncpy(prompt_content_ptr+suite_name_len+1,textcontent.wstr_ptr,textcontent.wstr_len);

                totalcontent.wstr_ptr = prompt_content_ptr;
                totalcontent.wstr_len = suite_name_len+textcontent.wstr_len+1;//+1 增加一个空格

                MMIPUB_OpenAlertWinByTextPtr(0,
                &totalcontent,
                PNULL,
                IMAGE_PUBWIN_WARNING, 
                PNULL,
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL
                );  
                SCI_FREE(suiteList_ptr);
                SCI_FREE(prompt_content_ptr);
                return;
            }
         }
    }
    else
    {
	    cursuitid = suiteList_ptr[run_suite_index].id;
        MMK_CreateWin((uint32*)MMIJAVA_AMS_MIDLETLIST_WIN_TAB, (ADD_DATA)cursuitid);
    }
    
    SCI_FREE(suiteList_ptr);
}

/*****************************************************************************/
//  Description : MMIJAVA_DeleteSuiteFromMainmenu
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIJAVA_DeleteSuiteFromMainmenu(void* param1_ptr, void* param2_ptr)
{
    AJMS_Suite*     suiteList_ptr = PNULL;
    int32           i  = 0;
    int32           iNumOfSuite     = 0;  
    int32           suite_index = -1;

    if(MMIAPIJAVA_IsJavaRuning())//有应用在运行，无法进行删除动作
    {

        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JVM_IS_RUNNING_CANNOT_DELETE_SUITE, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        return FALSE;
    }

    if(param1_ptr == PNULL)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteFromMainmenu:param1_ptr =%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1622_112_2_18_2_22_11_30,(uint8*)"s",param1_ptr);

    
    //重启java 系统 
    Ajava_Initiliztion_Special();

    iNumOfSuite = ajms_ab_getSutieListCount( -1 );
    if(iNumOfSuite < 0 || iNumOfSuite > (MAXJAVALISTSIZE + 1) )
    {
        //提示
        //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteFromMainmenu iNumOfSuite= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1632_112_2_18_2_22_11_31,(uint8*)"d",iNumOfSuite);
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_SUITE_NOT_EXIST, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        return FALSE;
    }

    
    suiteList_ptr = SCI_ALLOCA((uint32)iNumOfSuite*sizeof(AJMS_Suite));
    if(PNULL == suiteList_ptr)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_COMMON_NO_MEMORY, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteFromMainmenu suiteList_ptr ALLOC FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1656_112_2_18_2_22_11_32,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMSET(suiteList_ptr, 0, (uint32)iNumOfSuite*sizeof(AJMS_Suite));
    // get the information of all java applications
    if(ajms_ab_getSuiteList(-1 , suiteList_ptr, 0) < 0)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_SUITE_NOT_EXIST, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteFromMainmenu ajms_ab_getSuiteList FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1672_112_2_18_2_22_11_33,(uint8*)"");
        SCI_FREE(suiteList_ptr);
        return FALSE;
    }

    for(i = 0;i < iNumOfSuite;i++)
    {
        if(0 == strncmp((char *)suiteList_ptr[i].suiteName,param1_ptr,strlen((char *)suiteList_ptr[i].suiteName)))
        {
            suite_index = i;
            //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteFromMainmenu suite_index= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1682_112_2_18_2_22_11_34,(uint8*)"d",suite_index);
        }
 
    }
    
    if(suite_index < 0)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_SUITE_NOT_EXIST, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
        //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteFromMainmenu suite not exist"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1697_112_2_18_2_22_11_35,(uint8*)"");
        SCI_FREE(suiteList_ptr);
        return FALSE;
    }

    
    if(AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED == suiteList_ptr[suite_index].categoryId)
    {
        //提示有预置应用无法删除 
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_PREINSTALL_SUITE_CANNOT_DELETE, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING,
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL); 
 
        //SCI_TRACE_LOW:"MMIJAVA_DeleteSuiteFromMainmenu suite AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1715_112_2_18_2_22_11_36,(uint8*)"");
        SCI_FREE(suiteList_ptr);
        return FALSE;
    }

    //最终删除
    JavaDeleteSuite(suiteList_ptr[suite_index].id);
    
    SCI_FREE(suiteList_ptr);

    return TRUE;
}
#endif

#ifdef JAVA_EVENT_ERROR_PROMPT_WIN
PUBLIC void JAVA_Install_ErrorHandle(AJ_S32 otaCode)
{
	
	MMI_TEXT_ID_T     error_text_id=TXT_NULL;

    switch ( otaCode )
    {
    case AJMS_ERR_OTA_NO_ENOUGH_SPACE:
	 error_text_id = TXT_COMMON_NO_MEMORY;
        break;     
    case AJMS_ERR_OTA_USER_CANCEL:
	 error_text_id = TXT_USER_CANCEL;
        break;     
    case AJMS_ERR_OTA_LOSS_OF_SERVICE:
	 error_text_id = TXT_JAVA_ERR_OTA_LOSS_OF_SERVICE;
        break;     
    case AJMS_ERR_OTA_JAR_SIZE_MISMATCH:
  	 error_text_id = TXT_JAVA_ERR_OTA_JAR_SIZE_MISMATCH;
        break;    
    case AJMS_ERR_OTA_ATTRIBUTE_MISMATCH: 
  	 error_text_id = TXT_JAVA_ERR_OTA_ATTRIBUTE_MISMATCH;
        break;  		
    case AJMS_ERR_OTA_INVALID_JAD:
  	 error_text_id = TXT_JAVA_ERR_OTA_INVALID_JAD;
        break;  
    case AJMS_ERR_OTA_INVALID_JAR:
  	 error_text_id = TXT_JAVA_ERR_OTA_INVALID_JAR;
        break;  
    case AJMS_ERR_OTA_CLDC_MIDP_VERSION:
  	 error_text_id = TXT_JAVA_ERR_OTA_CLDC_MIDP_VERSION;
        break;  	
    case AJMS_ERR_OTA_AUTHENTICATION_FAILURE:
  	 error_text_id = TXT_JAVA_ERR_OTA_AUTHENTICATION_FAILURE;
        break;  		
    case AJMS_ERR_OTA_AUTHORIZATION_FAILURE:
  	 error_text_id = TXT_JAVA_ERR_OTA_AUTHORIZATION_FAILURE;
        break;  
    case AJMS_ERR_OTA_PUSH_REGISTRATION_FALIURE:
  	 error_text_id = TXT_JAVA_ERR_OTA_PUSH_REGISTRATION_FALIURE;
        break;  
	default:
	 error_text_id = TXT_JAVA_ERR_OTA_UNDEFINED;
        break;
    }

        //SCI_TRACE_LOW:"JAVA_Install_ErrorHandle otaCode=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_1775_112_2_18_2_22_11_37,(uint8*)"d", otaCode);
       
	if(TXT_NULL != error_text_id)
	{
           MMIPUB_OpenAlertWinByTextId(PNULL,error_text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
	}

	
}
#endif
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:jian.ma
// Note: 
/*****************************************************************************/
LOCAL void SetCommonCharacter( GUIRICHTEXT_ITEM_T* p_item )
{
    //SCI_PASSERT(PNULL != p_item,("SetCommonCharacter,param error"));   
    if(PNULL != p_item)
    {
        p_item->img_type = GUIRICHTEXT_IMAGE_NONE;
        p_item->text_type = GUIRICHTEXT_TEXT_BUF;
    }
#if 0
    p_item->type = GUIRICHTEXT_ITEMTYPE_TEXT;
    p_item->need_frame = FALSE;
    p_item->data.text.has_icon = FALSE;
    p_item->data.text.icon_id = NULL;
    p_item->data.text.recogn_type = GUIRICHTEXT_RECOGCHAR_TYPE_NULL;
#endif
}

#ifdef CMCC_JAVA_V41_SUPPORT
/*****************************************************************************/
//  Description :check_midlet_can_keepBgRunning
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void check_midlet_can_keepBgRunning(int midlet_suite_id)
{
    get_CMCC_extention_info( midlet_suite_id );

    if( jad_keep_background_running == TRUE && 
        current_running_midlet_domain == AJMS_PCS_DOMAIN_OPERATOR){
        JWE_LOG(check_midlet_can_keepBgRunning,("CMCC trusted midlet"));
        keep_background_running = TRUE;
    }
    else{
        keep_background_running = FALSE;
    }
}

/*****************************************************************************/
//  Description :get_CMCC_extention_info
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void get_CMCC_extention_info(int midlet_suite_id)
{
	AJ_S32 propertyLen = 0;
	AJ_S32 jadSize;
	AJ_U8* jadBuffer=PNULL;
	AJ_S32  ret;
	AJ_U8* keep_bg_value=PNULL; 
	
	JWE_LOG(get_CMCC_extention_info,("enter, midlet_suite_id: %d", midlet_suite_id));

	jad_keep_background_running = FALSE;
	jadSize = ajms_ab_getJadFileContent (midlet_suite_id, NULL, 0);
	
	if(jadSize > 0)
	{
		jadBuffer = (AJ_U8*)SCI_ALLOC(jadSize);        
		SCI_MEMSET(jadBuffer, 0, jadSize);

		ret = ajms_ab_getJadFileContent (midlet_suite_id, jadBuffer, jadSize);
		if (ret <= 0)
		{
			JWE_LOG(get_CMCC_extention_info,("can't get jad buf, return %d", ret)); 
			goto error_return;
		}
		else
		{
			propertyLen = ajms_ab_getAppDescProperty (jadBuffer, jadSize, AJMS_DESC_TYPE_JAD, CKB_KEY, NULL, 0);
			if(propertyLen <= 0)
			{
				  goto error_return;
			}
			else
			{
				keep_bg_value = (AJ_U8*)SCI_ALLOC(propertyLen);        
				SCI_MEMSET(keep_bg_value, 0, propertyLen);
				ret = ajms_ab_getAppDescProperty (jadBuffer, jadSize, AJMS_DESC_TYPE_JAD, CKB_KEY, keep_bg_value, propertyLen);
				if (ret <= 0)
				{
					  JWE_LOG(get_CMCC_extention_info,("can't get property \"CMCC-Keep-Background\"")); 
				    	  goto error_return;
				}
			}
		}
	}
	else
	{
		goto error_return;
	}
    
	propertyLen=ajms_ab_getAppDescProperty( jadBuffer, jadSize, AJMS_DESC_TYPE_JAD, 
	                      CKB_KEY, keep_bg_value, sizeof(keep_bg_value) );

	if( propertyLen <=0 ){
	    JWE_LOG(get_CMCC_extention_info,("%s not found in JAD.", CKB_KEY));
	    goto error_return;
	}

	JWE_LOG(get_CMCC_extention_info,("%s: %s",CKB_KEY, keep_bg_value));

	if( (memcmp("yes",keep_bg_value, 3 ) == 0) ||(memcmp("YES",keep_bg_value, 3 )==0)){    
	    jad_keep_background_running = TRUE;
	}

error_return:

	if( jadBuffer != PNULL){
	    SCI_FREE(jadBuffer);
	}
	
	if( keep_bg_value != PNULL){
	    SCI_FREE(keep_bg_value);
	}

	JWE_LOG(get_CMCC_extention_info,("exit, midlet_suite_id: %d", midlet_suite_id));
}
#endif

LOCAL MMI_RESULT_E  HandleJavaAlertForceQuitWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    int32 err = 0;
    JWE_LOG(HandleJavaAlertForceQuitWinMsg, ("enter"));
    switch(msg_id)
    {
    case MSG_APP_WEB:
    case MSG_APP_OK:
        JWE_LOG(HandleJavaAlertForceQuitWinMsg, ("msg_id = %x Force Kill OK,", msg_id));
        err = AamsVmKill( 1 );
        s_b_force_killed = TRUE;
        JWE_LOG(HandleJavaAlertForceQuitWinMsg, ("err = %d", err));
        break;

    case MSG_APP_CANCEL:
        JWE_LOG(HandleJavaAlertForceQuitWinMsg, ("msg_id = %x  Force Kill Cancel", msg_id));
        break;

    default:
        break;
    }

    recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    
    return recode;
}


/*****************************************************************************/
//  Description :MMIJAVA_OpenMidletWindow
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenMidletWindow(JAVA_INSTALL_INFO_T *install_info_ptr)
{       
    MMIJAVA_initConnPool();
    GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,GUILCD_GetInitAngle(GUI_MAIN_LCD_ID));
    MMK_CreateWin((uint32*)MMIJAVA_MIDLET_WIN_TAB, (ADD_DATA)install_info_ptr);
}

/*****************************************************************************/
//  Description :SetSelectNetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetSelectNetFormParam(MMI_WIN_ID_T	win_id,int32 cursuiteid)
{

    uint16			                    form_space =    0;
	MMI_STRING_T*                       dropdownlist_item_ptr = PNULL;
    uint16                              list_num  = 0;
#if defined MMI_GPRS_SUPPORT
    //MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
#endif
    //uint8                               linkset_num = 0;
    MMIJAVA_LINK_SETTING_T              link_setting_info = {0};
    int32                               i = MN_DUAL_SYS_1;
    //uint16                              index  = 0;
    
    MMI_TEXT_ID_T java_user[] =
    {
        TXT_COMMON_SET_NET_SIM1,          
        TXT_COMMON_SET_NET_SIM2,
        #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_COMMON_SET_NET_SIM3,
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
         TXT_COMMON_SET_NET_SIM4
        #endif      
    };
    MMI_CTRL_ID_T   lable_id[] =
    {
        MMIJAVA_SELECT_NET_LABEL_SIM1_ID,
        MMIJAVA_SELECT_NET_LABEL_SIM2_ID,
        MMIJAVA_SELECT_NET_LABEL_SIM3_ID,
        MMIJAVA_SELECT_NET_LABEL_SIM4_ID
    };
    MMI_CTRL_ID_T   form_id[] =
    {
        MMIJAVA_SELECT_NET_FORM2_ID,
        MMIJAVA_SELECT_NET_FORM3_ID,
        MMIJAVA_SELECT_NET_FORM4_ID,
        MMIJAVA_SELECT_NET_FORM5_ID
    };
    
    GUIFORM_SetStyle(MMIJAVA_SELECT_NET_FORM1_ID,GUIFORM_STYLE_UNIT);

    link_setting_info  = MMIJAVA_GetNetSettingInfo(s_suiteList[cursuiteid].id);
    //set label text content
    GUILABEL_SetTextById(MMIJAVA_SELECT_NET_LABEL_NET_ID,TXT_JAVA_USER,FALSE);
    
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        GUIFORM_SetStyle(form_id[i],GUIFORM_STYLE_UNIT);
        GUILABEL_SetTextById(lable_id[i],java_user[i],FALSE);
    }
#if defined MMI_GPRS_SUPPORT
    SetSimNetAccountLabelText(win_id, cursuiteid);
#endif
    MMK_SetAtvCtrl(win_id, MMIJAVA_SELECT_NET_LABEL_SIM1_ID);

    //for net type dropdown list 
    list_num = MMI_DUAL_SYS_MAX;
#ifdef WIFI_SUPPORT
    list_num++;
#endif

    dropdownlist_item_ptr = SCI_ALLOCA(list_num*sizeof(MMI_STRING_T));

    if(PNULL == dropdownlist_item_ptr)
    {
        //SCI_TRACE_LOW:"SetSelectNetFormParam ALLOCA FAILED 2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2020_112_2_18_2_22_12_38,(uint8*)"");
        return;
    }
        
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMI_GetLabelTextByLang(label_str[i], &dropdownlist_item_ptr[i]);
    }
    
#ifdef WIFI_SUPPORT
    MMI_GetLabelTextByLang(TXT_WIFI, &dropdownlist_item_ptr[i]);
#endif

    if(list_num > 1)/*lint !e774*/
    {
        GUIDROPDOWNLIST_AppendItemArray(MMIJAVA_SELECT_NET_DROPDOWNLIST_NET_ID, dropdownlist_item_ptr, list_num);
        GUIDROPDOWNLIST_SetCurItemIndex(MMIJAVA_SELECT_NET_DROPDOWNLIST_NET_ID,link_setting_info.network_type);
        MMK_SetAtvCtrl(win_id, MMIJAVA_SELECT_NET_DROPDOWNLIST_NET_ID);
    }
    else
    {
        GUIFORM_SetChildDisplay(MMIJAVA_SELECT_NET_FORM_ID,MMIJAVA_SELECT_NET_FORM1_ID,GUIFORM_CHILD_DISP_HIDE);
    }

    GUIFORM_SetSpace(MMIJAVA_SELECT_NET_FORM_ID,&form_space,&form_space);
    GUIFORM_SetMargin(MMIJAVA_SELECT_NET_FORM_ID,0);

    SCI_FREE(dropdownlist_item_ptr);
}

#if defined MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description : set/update sim network account label text
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void SetSimNetAccountLabelText(MMI_WIN_ID_T win_id, int32 cursuiteid)
{
    uint8   i = 0;    
    MMICONNECTION_LINKSETTING_DETAIL_T  *link_setting_ptr = PNULL;
    MMIJAVA_LINK_SETTING_T              link_setting_info = {0};
    MMI_CTRL_ID_T sim_label_ctrl_id[] = {
            MMIJAVA_SELECT_NET_LABEL2_SIM1_ID,
            MMIJAVA_SELECT_NET_LABEL2_SIM2_ID,
            MMIJAVA_SELECT_NET_LABEL2_SIM3_ID,
            MMIJAVA_SELECT_NET_LABEL2_SIM4_ID
        };
    MMI_STRING_T    string = {0};//[MMI_DUAL_SYS_MAX]
    uint8          select_index = 0;

    link_setting_info  = MMIJAVA_GetNetSettingInfo(s_suiteList[cursuiteid].id);
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {      
        /*set SIM label*/
        select_index = link_setting_info.sim_index[i];
        link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex((MN_DUAL_SYS_E)i,select_index);
        string.wstr_len = link_setting_ptr->name_len;
        string.wstr_ptr = (wchar *)link_setting_ptr->name;
        GUILABEL_SetText(sim_label_ctrl_id[i], &string, FALSE); // text
        s_sim_selected_account_index[i] = select_index;
    }
}

/*****************************************************************************/
//  Description : set sim's network accout index setting for wap setting
//  Global resource dependence : s_nv_setting
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void JavaSetNetIndexSettingCallback(MN_DUAL_SYS_E dual_sys, uint32 index)
{
    MMICONNECTION_LINKSETTING_DETAIL_T  *link_setting_ptr = PNULL;
    MMI_CTRL_ID_T sim_label_ctrl_id[] = {
            MMIJAVA_SELECT_NET_LABEL2_SIM1_ID,
            MMIJAVA_SELECT_NET_LABEL2_SIM2_ID,
            MMIJAVA_SELECT_NET_LABEL2_SIM3_ID,
            MMIJAVA_SELECT_NET_LABEL2_SIM4_ID
        };
    MMI_STRING_T    string = {0};
    
    link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex((MN_DUAL_SYS_E)dual_sys, index);
	
	string.wstr_ptr = (wchar *)link_setting_ptr->name;
	string.wstr_len = link_setting_ptr->name_len;

    GUILABEL_SetText(sim_label_ctrl_id[dual_sys], &string, FALSE); // text
    s_sim_selected_account_index[dual_sys] = index;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void JavaHandleSettingAccount(int32 curSuiteId, MN_DUAL_SYS_E dual_sys)
{
    MMIJAVA_LINK_SETTING_T   link_setting_info = {0};

    link_setting_info  = MMIJAVA_GetNetSettingInfo(curSuiteId);
    MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_JAVA,
        MMIAPICONNECTION_GetListIndexByAPNIndex(dual_sys, link_setting_info.sim_index[(MN_DUAL_SYS_E)dual_sys]),
        dual_sys, JavaSetNetIndexSettingCallback);
}
#endif

/**************************************************************************************************/
// Description:HandleJavaNetSelectWinMsg
// Date:20100321
// Author:murphy.xie
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaNetSelectWinMsg(
                                                MMI_WIN_ID_T	win_id, 
                                                MMI_MESSAGE_ID_E	msg_id, 
                                                DPARAM				param
                                                )
{
    // uint16                      selected_item = 0;
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    int32                       curSuiteId = (int32)MMK_GetWinAddDataPtr(win_id);
    // MN_DUAL_SYS_E               sim_card_record  = MN_DUAL_SYS_1;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
			//SCI_TRACE_LOW:"HandleJavaNetSelectWinMsg"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2140_112_2_18_2_22_12_39,(uint8*)"");
            SetSelectNetFormParam(win_id,curSuiteId);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        }
        break;

    case MSG_FULL_PAINT:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    #ifdef WIFI_SUPPORT
    case MSG_CTL_DROPDOWNLIST_SELECTED:
	    if(MMK_GetActiveCtrlId(win_id) == MMIJAVA_SELECT_NET_DROPDOWNLIST_NET_ID)
	    {
	        MMI_STRING_T item_info = {0};
            MMI_STRING_T           wifi = {0};
            MMI_GetLabelTextByLang(TXT_WIFI, &wifi);
            item_info = GUIDROPDOWNLIST_GetCurItem(MMIJAVA_SELECT_NET_DROPDOWNLIST_NET_ID);
            MMIAPICOM_WstrTraceOut(item_info.wstr_ptr,item_info.wstr_len);
            if(0 == MMIAPICOM_Wstrncmp(wifi.wstr_ptr,item_info.wstr_ptr,wifi.wstr_len))
            {
                if(MMIWIFI_STATUS_CONNECTED != MMIAPIWIFI_GetStatus())
                {
                    MMIAPIWIFI_OpenListWin();
                }
            }
        }
        break;
    #endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch (src_id)
            {
#ifdef MMI_PDA_SUPPORT
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
                
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
                
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
#endif
#if defined MMI_GPRS_SUPPORT
            case MMIJAVA_SELECT_NET_LABEL2_SIM1_ID:
			    JavaHandleSettingAccount(s_suiteList[curSuiteId].id, MN_DUAL_SYS_1);
				break;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            case MMIJAVA_SELECT_NET_LABEL2_SIM2_ID:
				JavaHandleSettingAccount(s_suiteList[curSuiteId].id, MN_DUAL_SYS_2);
				break;
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
            case MMIJAVA_SELECT_NET_LABEL2_SIM3_ID:
				JavaHandleSettingAccount(s_suiteList[curSuiteId].id, MN_DUAL_SYS_3);
				break;
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            case MMIJAVA_SELECT_NET_LABEL2_SIM4_ID:
            JavaHandleSettingAccount(s_suiteList[curSuiteId].id, MN_DUAL_SYS_4);
            break;
#endif 
#endif
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
//#ifndef MMI_PDA_SUPPORT
    case MSG_CTL_MIDSK:
        switch (MMK_GetActiveCtrlId(win_id))
        {
#if defined MMI_GPRS_SUPPORT
        case MMIJAVA_SELECT_NET_LABEL2_SIM1_ID:
        JavaHandleSettingAccount(s_suiteList[curSuiteId].id, MN_DUAL_SYS_1);
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMIJAVA_SELECT_NET_LABEL2_SIM2_ID:
        JavaHandleSettingAccount(s_suiteList[curSuiteId].id, MN_DUAL_SYS_2);
        break;

#endif
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIJAVA_SELECT_NET_LABEL2_SIM3_ID:
        JavaHandleSettingAccount(s_suiteList[curSuiteId].id, MN_DUAL_SYS_3);
        break;
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIJAVA_SELECT_NET_LABEL2_SIM4_ID:
        JavaHandleSettingAccount(s_suiteList[curSuiteId].id, (MN_DUAL_SYS_E)(MMK_GetActiveCtrlId(win_id) - MMIJAVA_SELECT_NET_LABEL2_SIM1_ID));
        break;
#endif 
#endif
        default:
            break;
        }
        break;
//#endif
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    //case MSG_APP_MENU:
   // case MSG_CTL_PENOK:
        {
	        MMI_STRING_T item_info = {0};
            MMI_STRING_T           wifi = {0};
            MMI_STRING_T           sim = {0};
            MMIJAVA_NETWORK_TYPE_E net_index  = 0;
            // uint8                  sim1_setting = 0;
            // uint8                  sim2_setting = 0;
            MMIJAVA_LINK_SETTING_T java_setting = {0};
            BOOLEAN                valid_flag = TRUE;
            uint32 i = MN_DUAL_SYS_1;           
            MMI_TEXT_ID_T sim_not_ok_str[] = {
                                                            TXT_JAVA_SELECT_NET_SIM1_NOT_OK,
                                                            TXT_JAVA_SELECT_NET_SIM2_NOT_OK,
                                                            #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
                                                            TXT_JAVA_SELECT_NET_SIM3_NOT_OK,
                                                            #if defined(MMI_MULTI_SIM_SYS_QUAD)
                                                            TXT_JAVA_SELECT_NET_SIM4_NOT_OK,
                                                            #endif
                                                            #endif
                                                            };           
            MMI_GetLabelTextByLang(TXT_WIFI, &wifi);           
            item_info = GUIDROPDOWNLIST_GetCurItem(MMIJAVA_SELECT_NET_DROPDOWNLIST_NET_ID);

            if(0 == MMIAPICOM_Wstrncmp(wifi.wstr_ptr,item_info.wstr_ptr,wifi.wstr_len))
            {
                net_index = JAVA_NETWORK_TYPE_WIFI;

            }
            else 
            {
                
                for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                {
                     MMI_GetLabelTextByLang(label_str[i], &sim);
                     if(0 == MMIAPICOM_Wstrncmp(sim.wstr_ptr,item_info.wstr_ptr,sim.wstr_len))
                     {
                        if(MMIAPIPHONE_IsSimAvailable(i))
                        {
                            net_index = JAVA_NETWORK_TYPE_GPRS_SIM1+(i - MN_DUAL_SYS_1);

                        }
                        else
                        {
                            valid_flag = FALSE;
                            MMIPUB_OpenAlertWarningWin(sim_not_ok_str[i]);
                        }
                        break;
                     }
                }                
            }
           
            //write info to file
            if(valid_flag)
            {
                for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
                {
                    java_setting.sim_index[i] = s_sim_selected_account_index[i];//GUIDROPDOWNLIST_GetCurItemIndex(drop_down_id[i]);
                }         
                java_setting.network_type = net_index;

                if(MMIJAVA_SetNetSettingInfo(s_suiteList[curSuiteId].id,java_setting))
                {
                   // MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                }
            }
        }

        break;
        
    case MSG_CLOSE_WINDOW:
        break;
                
    default:
        recode = MMI_RESULT_FALSE;
        break; 
    }
    return(recode);
}

//  Description : to open java icon window
//  Global resource dependence : none
//  Author: louis 
//  Note:
/*****************************************************************************/
LOCAL void OpenInstallListWin(BOOLEAN is_open_again)
{   
    MMIFMM_FILTER_T         filter     = {0};
    MMIFMM_FUNC_TYPE_E      func_type  = 0;
    uint32                  suffix_filter = MMIFMM_JAVA_ALL;
	
    if (!MMIAPIUDISK_IsDeviceReady())
    {
        return;
    }
	
#ifdef DRM_SUPPORT
    suffix_filter = suffix_filter|MMIFMM_DM_FILE|MMIFMM_DCF_FILE;
#endif
    
	MMIAPIFMM_GetFilerInfo(&filter,suffix_filter);
    func_type = FUNC_FIND_FILE_AND_ALLFOLER;  
    MMIAPIFMM_OpenFileWinByLayer(PNULL, 0,&filter, func_type, FALSE,HandleSelectWinMsg, NULL, NULL,FALSE);
}


/*****************************************************************************/
//  Description : handle java select win
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectWinMsg(
                                      MMI_WIN_ID_T          win_id,     
                                      MMI_MESSAGE_ID_E      msg_id, 
                                      DPARAM                param
                                      )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    FILEARRAY_DATA_T   file_data = {0};
	//BOOLEAN             is_ret =  TRUE;
	
    //SCI_TRACE_LOW:"HandleSelectWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2362_112_2_18_2_22_12_40,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:          
        break;

    case MSG_FULL_PAINT:
        GUIWIN_SetTitleTextId(win_id, TXT_COM_LOCALINSTALL, FALSE);  
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_APP_WEB:   
    case MSG_CTL_MIDSK: 
#ifdef MMI_PDA_SUPPORT
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            if(src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID || src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID || src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                result = MMI_RESULT_FALSE; 
                break;
            }
        }
#endif
        MMIAPIFMM_GetOpenFileWinCurFile(&file_data);
        //SCI_TRACE_LOW:"HandleSelectWinMsg len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2390_112_2_18_2_22_12_41,(uint8*)"d",file_data.name_len);
        if (FILEARRAY_TYPE_FILE == file_data.type)
        {          
            MMIAPIJAVA_StartLocalInstall(file_data.filename,file_data.name_len,0);         
            
            result = MMI_RESULT_TRUE;
        } 
        else
        {        
        
            result = MMI_RESULT_FALSE;   
        }
        break;

   

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

   // if (MMI_RESULT_FALSE == result)
    //{
        //选择文件窗口默认处理函数
   //     result = MMIFMM_HandleOpenFileByLayerWin(win_id, msg_id, param);
   // }

    return result;
}


/**************************************************************************************************/
// Description:HandleJavaAMSMidletListWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA MIDlet列表窗口处理函数，如果应用是包含多个MIDlet的应用
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSMidletListWinMsg(
                                                 MMI_WIN_ID_T        win_id,    
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E               result  = MMI_RESULT_TRUE;
    uint16                     curmidletid = 0;
    int32                      cursuitid = (int32)MMK_GetWinAddDataPtr(win_id);
    JAVA_INSTALL_INFO_T*       install_info_ptr = MMIJAVA_GetIntallInfo();
    //SCI_TRACE_LOW:"HandleJavaAMSMidletListWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2436_112_2_18_2_22_12_42,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID, MAXJAVALISTSIZE, FALSE);  //10
        CreateMidletListBox(cursuitid);        
        //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id,MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:    
    case MSG_KEYDOWN_WEB:        
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        curmidletid = GUILIST_GetCurItemIndex(MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID); 
        //SCI_TRACE_LOW:"HandleJavaAMSMidletListWinMsg GUILIST_GetCurItemIndex %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2453_112_2_18_2_22_13_43,(uint8*)"d", curmidletid);
        SCI_MEMSET(install_info_ptr,0,sizeof(JAVA_INSTALL_INFO_T));
        install_info_ptr->curmidletid = curmidletid;
        install_info_ptr->cursuitid = cursuitid;
        install_info_ptr->is_flag = 0;
        // create a new window to start the selected java application
		MMIJAVA_OpenMidletWindow(install_info_ptr);
        break;  
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        
        break;
    case MSG_CLOSE_WINDOW: 
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

#ifdef MMI_PDA_SUPPORT
LOCAL uint32 s_java_popup_menu_item[][2] =
{
	{ID_JAVA_AMS_OPTION_PLAY, TXT_JAVA_AMS_OPTION_PLAY},//play
	{ID_JAVA_AMS_OPTION_DELETE, TXT_DELETE},//delete
    {ID_JAVA_AMS_OPTION_NETWORK_PROFILE, TXT_COMMON_LINKSETTING},
    {ID_JAVA_AMS_OPTION_PERMIT_SET, TXT_JAVA_AMS_OPTION_PERMITION_SET},
    {ID_JAVA_UPDATE, TXT_COMMON_UPDATE},
	{ID_JAVA_AMS_OPTION_INFO, STXT_DETAIL},//info
};
#endif
/**************************************************************************************************/
// Description:HandleJavaAMSWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA 应用列表窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E                result      = MMI_RESULT_TRUE;
    static int32                       cursuitid   = -1;
   
    //SCI_TRACE_LOW:"HandleJavaAMSWinMsg, msg_id = 0x%02x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2503_112_2_18_2_22_13_44,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {    
    case MSG_OPEN_WINDOW: 
        s_suiteList = SCI_ALLOCA((MAXJAVALISTSIZE +1)*sizeof(AJMS_Suite));
        if(PNULL == s_suiteList)
        {
            //SCI_TRACE_LOW:"HandleJavaAMSWinMsg, s_suiteList ALLOC failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2511_112_2_18_2_22_13_45,(uint8*)"");
            MMK_CloseWin(win_id);
        }
        cursuitid = 0;
        GUILIST_SetSyncDecodeAnim(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, TRUE); 
        GUILIST_SetMaxItem(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, MAXJAVALISTSIZE, TRUE);

        GUILIST_SetDefaultIcon(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, JAVA_ICON, JAVA_ICON);

        // List all of the midlet suite
        CreateSuitListBox(cursuitid);//无论正确与否都可以进入主窗口
        
        GUILIST_SetTitleIndexType( MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, GUILIST_TITLE_INDEX_DEFAULT );
        MMK_SetAtvCtrl(win_id,MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);      
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;
    case MSG_LCD_SWITCH://record the focus 
        cursuitid = GUILIST_GetCurItemIndex(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);   
        //SCI_TRACE_LOW:"HandleJavaAMSWinMsg cursuitid = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2532_112_2_18_2_22_13_46,(uint8*)"d",cursuitid);
        break;
    case MSG_GET_FOCUS:
        if(CreateSuitListBox(cursuitid))
        {        
            GUILIST_SetTitleIndexType( MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, GUILIST_TITLE_INDEX_DEFAULT );            
        }
        break;
        
    case MSG_FULL_PAINT:
        if (s_suiteList[0].size != 0)
        {
            GUILIST_SetCurItemIndex(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, cursuitid);
            GUILIST_SetSelectedItem(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, cursuitid, FALSE);     

        }
        break;
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            //add text into list item
            GUILIST_NEED_ITEM_CONTENT_T * need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            //SCI_ASSERT( PNULL != need_item_content_ptr );
            if(PNULL != need_item_content_ptr)
            {
                AppendOneListItemWithSuiteName(need_item_content_ptr->item_index, MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);
            }
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {            
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            //SCI_ASSERT( PNULL != need_item_content_ptr );     
            if(PNULL != need_item_content_ptr)
            {
                DrawListItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);
            }
        }
        break;    
    case MSG_CTL_MIDSK:  
    case MSG_KEYDOWN_WEB:
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif
        
        if(!s_ota_notify_is_success )
	    {
    		MMIPUB_OpenAlertWarningWin(TXT_JAVA_OTA_NOTIFY_SENDING);
    		break;
        }	
        cursuitid = GUILIST_GetCurItemIndex(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);
        //SCI_TRACE_LOW:"HandleJavaAMSWinMsg cursuitid = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2584_112_2_18_2_22_13_47,(uint8*)"d",cursuitid);
        MMK_CreateWin((uint32*)MMIJAVA_AMS_MIDLETLIST_WIN_TAB, (ADD_DATA)(s_suiteList[cursuitid].id));
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        {
            GUIMENU_DYNA_ITEM_T node_item = {0};
            MMI_STRING_T        menu_str = {0};
            MMI_STRING_T        title_str = {0};
            uint16              item_num = 0;
            uint16              i = 0;
            uint16              nodex_index = 0;
            const GUILIST_ITEM_T *cur_list_ptr = {0};
            
            cursuitid = GUILIST_GetCurItemIndex(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);   

            MMK_CreateWin((uint32 *)MMIJAVA_AMS_POPUP_OPTION_WIN_TAB, (ADD_DATA)cursuitid);
            GUIMENU_CreatDynamic(PNULL,MMIJAVA_AMS_POPUP_OPTION_WIN_ID, MMIJAVA_LONGPRESS_OPT_CTRL_ID,GUIMENU_STYLE_POPUP_AUTO);
            item_num = sizeof(s_java_popup_menu_item)/sizeof(*s_java_popup_menu_item);

            for(i=0; i<item_num; i++)
            {
                if(AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED == s_suiteList[cursuitid].categoryId)
                {
                    if(ID_JAVA_AMS_OPTION_DELETE == s_java_popup_menu_item[i][0])
                    {
                        continue;
                    }
                    
                    #ifdef MMI_MULTI_SIM_SYS_SINGLE
                    if(ID_JAVA_AMS_OPTION_APPLICATION_SET == s_java_popup_menu_item[i][0])
                    {
                        continue;
                    }
                    #endif
                }

                MMI_GetLabelTextByLang(s_java_popup_menu_item[i][1], &menu_str);
                node_item.item_text_ptr = &menu_str;
                GUIMENU_InsertNode(nodex_index, s_java_popup_menu_item[i][0], 0, &node_item, MMIJAVA_LONGPRESS_OPT_CTRL_ID);
                nodex_index++;
            }
            
            cursuitid = GUILIST_GetCurItemIndex(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);   
            cur_list_ptr = GUILIST_GetItemPtrByIndex(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, cursuitid);

            if(PNULL != cur_list_ptr)
            {
                title_str.wstr_len = cur_list_ptr->item_data_ptr->item_content[1].item_data.text_buffer.wstr_len;
                title_str.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[1].item_data.text_buffer.wstr_ptr;
                GUIMENU_SetMenuTitle(&title_str, MMIJAVA_LONGPRESS_OPT_CTRL_ID);
            }
        }
        break;
#endif
#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#else        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:        
    case MSG_CTL_OK:
#endif
        cursuitid = GUILIST_GetCurItemIndex(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);   
        //SCI_TRACE_LOW:"HandleJavaAMSWinMsg cursuitid = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2647_112_2_18_2_22_13_48,(uint8*)"d",cursuitid);
        MMK_CreateWin((uint32 *)MMIJAVA_AMS_OPTION_WIN_TAB, (ADD_DATA)cursuitid);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
    case MSG_CLOSE_WINDOW:  
        SCI_FREE(s_suiteList);
        break;        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
/**************************************************************************************************/
// Description:HandleJavaDownLoadEnter
// Date:2010.11.22
// Author:muprhy.xie
// Note:JAVA download enter browser interface
//*************************************************************************************************/
LOCAL void HandleJavaDownLoadEnter(int32 url_type)
{   
    MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MN_RETURN_RESULT_E              nv_result = MN_RETURN_FAILURE;
    MMIBROWSER_ENTRY_PARAM          brw_parm = {0};
    MMIJAVA_DOWNLOAD_ITEM_DETAIL_T  item_detail = {0};

    //SCI_TRACE_LOW:"HandleJavaDownLoadEnter Enter type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2678_112_2_18_2_22_13_49,(uint8*)"d",url_type);
    
    MMINV_READ(MMINV_JAVA_DOWNLOAD_INFO, &download_detail, nv_result);

    switch(url_type)
    {
     case ID_JAVA_DOWNLOAD_GAME:
        MMINV_READ(MMINV_JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
        item_detail = download_detail.game_info;
        break;
     case ID_JAVA_DOWNLOAD_EBOOK:
        MMINV_READ(MMINV_JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
        item_detail = download_detail.ebook_info;
        
        break;
     case ID_JAVA_DOWNLOAD_OTHER:
        MMINV_READ(MMINV_JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
        item_detail = download_detail.other_info;
        break;
      default:
        break;
    }
    
    if(item_detail.url_len == 0)
    {
        item_detail.url_len = JAVA_DOWNLOAD_DEFAULT_URL_LEN;
        SCI_MEMCPY(item_detail.url,s_default_download_url,JAVA_DOWNLOAD_DEFAULT_URL_LEN+1);
    }

    //brw param set
    brw_parm.url_ptr = (char *)item_detail.url;
    brw_parm.type = MMIBROWSER_ACCESS_URL;
    brw_parm.dual_sys = MN_DUAL_SYS_MAX;     
    brw_parm.is_connection_customized = FALSE;

    MMIAPIBROWSER_Entry(&brw_parm);
    //SCI_TRACE_LOW:"HandleJavaDownLoadEnter exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2714_112_2_18_2_22_13_50,(uint8*)"");
}
#endif

#ifdef JAVA_SUPPORT_SYSTEM_RESET_FUNCTION
/*****************************************************************************/
// 	Description :reset the ia java system,will back to init state
//	Global resource dependence : 
//  Author: murphy.xie
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIJAVA_ResetSystem(void)
{    
#ifdef IA_JAVA_SUPPORT_LOW_MEMORY
    wchar                   path_sytem_e_nor[] = L"E:\\System";
    #ifdef DUAL_TCARD_SUPPORT
    wchar                   path_sytem_f_nor[] = L"F:\\System";
    #endif
#else
    wchar                   path_sytem_c[] = L"C:";
    wchar                   path_sytem_d[] = L"D:";
    wchar                   path_sytem_e[] = L"E:";
    
    #ifdef DUAL_TCARD_SUPPORT
    wchar                   path_sytem_f[] = L"F:";
    #endif

#endif
    MMIFILE_ERROR_E         error = SFS_NO_ERROR;
    BOOLEAN                 result = TRUE;
    
    //SCI_TRACE_LOW:"MMIJAVA_ResetSystem enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2743_112_2_18_2_22_13_51,(uint8*)"");
    MMIAPIJAVA_Finalize_Special();

    #ifdef IA_JAVA_SUPPORT_LOW_MEMORY
    error = MMIAPIFMM_DeleteTree(path_sytem_e_nor,JAVA_DIR);
    //SCI_TRACE_LOW:"path_sytem_e_nor error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2748_112_2_18_2_22_13_52,(uint8*)"d",error);
    #ifdef DUAL_TCARD_SUPPORT
    error = MMIAPIFMM_DeleteTree(path_sytem_f_nor,JAVA_DIR);
    //SCI_TRACE_LOW:"path_sytem_f_nor error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2751_112_2_18_2_22_13_53,(uint8*)"d",error);

    #endif
    #else
        //please keep this sequece,don't change
        #ifdef DUAL_TCARD_SUPPORT
        error = MMIAPIFMM_DeleteTree(path_sytem_f,JAVA_DIR);
        //SCI_TRACE_LOW:"path_sytem_f error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2758_112_2_18_2_22_13_54,(uint8*)"d",error);
        #endif

        error = MMIAPIFMM_DeleteTree(path_sytem_e,JAVA_DIR);
        //SCI_TRACE_LOW:"path_sytem_e error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2762_112_2_18_2_22_13_55,(uint8*)"d",error);
        
        error = MMIAPIFMM_DeleteTree(path_sytem_d,JAVA_DIR);
        //SCI_TRACE_LOW:"path_sytem_d error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2765_112_2_18_2_22_13_56,(uint8*)"d",error);
        #ifndef WIN32
        error = MMIAPIFMM_DeleteTree(path_sytem_c,JAVA_DIR);
        //SCI_TRACE_LOW:"path_sytem_c error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2768_112_2_18_2_22_13_57,(uint8*)"d",error);
        #endif
    #endif
    #ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIMENU_DyDeleteItemByFunc(MMIJAVA_OpenJAVAFromMainMenu);
    #endif
    Ajava_Initiliztion_Special(); //init system
    //SCI_TRACE_LOW:"MMIJAVA_ResetSystem success!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2775_112_2_18_2_22_13_58,(uint8*)"");
    
    return result;
    
}

/*****************************************************************************/
// 	Description : reset system to init msg prompt msg win
//	Global resource dependence : 
//  Author: murphy.xie
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJavaResetSystemPromptWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"HandleJavaResetSystemPromptWinMsg,msg_id =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2793_112_2_18_2_22_13_59,(uint8*)"d",msg_id);

    switch (msg_id)
    {
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            if(MMIJAVA_ResetSystem())
            {
             //   MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);           
            break;

        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return (result);    
}
#endif

/*****************************************************************************/
// 	Description : handle hardware icon prompt msg
//	Global resource dependence : 
//  Author: Allen.Zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJavaDeletePromptWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{

    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T*      win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    int32               cursuitid = 0;//( int32 )win_info_ptr->user_data;
    MMI_STRING_T        prompt_str = {0};


    if(PNULL == win_info_ptr)
	{
		//SCI_TRACE_LOW:"HandleJavaDeletePromptWinMsg win_info_ptr == PNULL !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2845_112_2_18_2_22_13_60,(uint8*)"");
        return result;
	}
    
    cursuitid =( int32 )win_info_ptr->user_data;
    
    switch (msg_id)
    {
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            ams_delet_ok++;
    		if(ams_delet_ok==1)
    		{

                if (ajms_ab_getMidletListCount( s_suiteList[cursuitid].id )>1) 
                {
                    MMI_GetLabelTextByLang(TXT_JAVA_DELETE_MIDLET_SUIT, &prompt_str);                    

                }
		        else
		        {
                    MMI_GetLabelTextByLang(TXT_JAVA_DELETE_MIDLET_RMS, &prompt_str);     
	  	        }
             	MMIPUB_SetAlertWinTextByPtr(win_id,&prompt_str,PNULL,TRUE);
    		}
    		else
    		{

                JavaDeleteSuite(s_suiteList[cursuitid].id); 
                MMK_CloseWin(win_id);
    		}
            
            break;

        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return (result);    
}

#ifdef MMI_PDA_SUPPORT
/**************************************************************************************************/
// Description:HandleJavaAMSPopUpOptionWinMsg
// Date:
// Author:maryxiao
// Note:JAVA 应用选项弹出menu处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSPopUpOptionWinMsg(
                                             MMI_WIN_ID_T        win_id,    
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             )
{
   
    MMI_RESULT_E                result  = MMI_RESULT_TRUE;
    int32                       cursuitid = (int32)MMK_GetWinAddDataPtr(win_id);
     uint32                      node_id       = 0;


    
    //SCI_TRACE_LOW:"HandleJavaAMSOptionWinMsg,msg_id =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_2913_112_2_18_2_22_14_61,(uint8*)"d",msg_id);
    //判断应用是否是内置应用，是的话是删除菜单选项变灰
    //need modify
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED == s_suiteList[cursuitid].categoryId)
        {
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_DELETE, TRUE);
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_APPLICATION_SET, TRUE);
        }
        
        if(ajms_ab_getSutieListCount(-1) <= 0)
        {
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_DELETE, TRUE);
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_PLAY, TRUE);
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_INFO, TRUE);
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_APPLICATION_SET, TRUE);

        }         
        break;
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
       node_id = GUIMENU_GetCurNodeId(MMIJAVA_LONGPRESS_OPT_CTRL_ID);
        switch(node_id)
        {
        case ID_JAVA_AMS_OPTION_PLAY:
            MMK_CreateWin((uint32*)MMIJAVA_AMS_MIDLETLIST_WIN_TAB, (ADD_DATA)(s_suiteList[cursuitid].id));
            break;

        case ID_JAVA_AMS_OPTION_DELETE:
            //隐藏pop menu
		    ams_delet_ok=0;
            MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),
                                            TXT_DELETEQUERY, 
                                            IMAGE_PUBWIN_QUERY,
                                            PNULL,
                                            HandleJavaDeletePromptWinMsg,
                                            (uint32)cursuitid
                                            );
            break;

        case ID_JAVA_AMS_OPTION_INFO:
            MMK_CreateWin((uint32 *)MMIJAVA_AMS_OPTION_INFO_WIN_TAB, (ADD_DATA)cursuitid);
            break;
        case ID_JAVA_APP_INSTALL:
            OpenInstallListWin(FALSE);
            break;
	    case ID_JAVA_AMS_OPTION_NETWORK_PROFILE:
             MMK_CreateWin((uint32*)MMIJAVA_SELECT_NET_ENTER_POINT, (ADD_DATA)cursuitid);
             break;         
        case ID_JAVA_AMS_OPTION_PERMIT_SET:
            MMK_CreateWin((uint32 *)MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_TAB, (ADD_DATA)cursuitid);
            break;
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
        case ID_JAVA_UPDATE: 
            if(!MMIJAVA_SuiteUpdate(s_suiteList[cursuitid].id))
            {
                MMIPUB_OpenAlertWarningWin(TXT_JAVA_CANNOT_UPDATE);
            }
            break;
#endif

        default: 
            break;
        }
        MMK_CloseWin(win_id);             
        break;
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:      
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif
/**************************************************************************************************/
// Description:HandleJavaAMSOptionWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA 应用选项窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAMSOptionWinMsg(
                                             MMI_WIN_ID_T        win_id,    
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             )
{
   
    MMI_RESULT_E                result  = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    int32                       cursuitid = (int32)MMK_GetWinAddDataPtr(win_id);
   
    
    //SCI_TRACE_LOW:"HandleJavaAMSOptionWinMsg,msg_id =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3019_112_2_18_2_22_14_62,(uint8*)"d",msg_id);
    //判断应用是否是内置应用，是的话是删除菜单选项变灰
    //need modify
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED == s_suiteList[cursuitid].categoryId)
        {
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_DELETE, TRUE);
            #ifdef MMI_MULTI_SIM_SYS_SINGLE
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_APPLICATION_SET, TRUE);
            #endif
        }
        
        if(ajms_ab_getSutieListCount(-1) <= 0)
        {
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_DELETE, TRUE);
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_PLAY, TRUE);
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_INFO, TRUE);
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_AMS_OPTION_APPLICATION_SET, TRUE);
            #ifdef JAVA_OTA_SUPPORT
            GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,MENU_JAVA_AMS_OPTION, ID_JAVA_UPDATE, TRUE);
            #endif
        }
        MMK_SetAtvCtrl(win_id,MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID);          
        break;
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
        GUIMENU_GetId(MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,&group_id,&menu_id);
        switch(menu_id)
        {
        case ID_JAVA_AMS_OPTION_PLAY:
            MMK_CreateWin((uint32*)MMIJAVA_AMS_MIDLETLIST_WIN_TAB, (ADD_DATA)(s_suiteList[cursuitid].id));
            break;

        case ID_JAVA_AMS_OPTION_DELETE:
            //隐藏pop menu
		    ams_delet_ok=0;
            MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),
                                            TXT_DELETEQUERY, 
                                            IMAGE_PUBWIN_QUERY,
                                            PNULL,
                                            HandleJavaDeletePromptWinMsg,
                                            (uint32)cursuitid
                                            );
            break;

        case ID_JAVA_AMS_OPTION_INFO:
            MMK_CreateWin((uint32 *)MMIJAVA_AMS_OPTION_INFO_WIN_TAB, (ADD_DATA)cursuitid);
            break;
        case ID_JAVA_APP_INSTALL:
            OpenInstallListWin(FALSE);
            break;
	    case ID_JAVA_AMS_OPTION_NETWORK_PROFILE:
             MMK_CreateWin((uint32*)MMIJAVA_SELECT_NET_ENTER_POINT, (ADD_DATA)cursuitid);
             break;         
        case ID_JAVA_AMS_OPTION_PERMIT_SET:
            MMK_CreateWin((uint32 *)MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_TAB, (ADD_DATA)cursuitid);
            break;
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
         case ID_JAVA_DOWNLOAD_GAME:
            HandleJavaDownLoadEnter(ID_JAVA_DOWNLOAD_GAME);
            break;
         case ID_JAVA_DOWNLOAD_EBOOK:
            HandleJavaDownLoadEnter(ID_JAVA_DOWNLOAD_EBOOK);
            break;
         case ID_JAVA_DOWNLOAD_OTHER:
            HandleJavaDownLoadEnter(ID_JAVA_DOWNLOAD_OTHER);
            break;
         case ID_JAVA_DOWNLOAD_SET:            
            MMK_CreateWin((uint32 *)MMIJAVA_APP_DOWNLOAD_SET_WIN_TAB, (ADD_DATA)cursuitid);
            break;            
         case ID_JAVA_UPDATE: 
            if(!MMIJAVA_SuiteUpdate(s_suiteList[cursuitid].id))
            {
                MMIPUB_OpenAlertWarningWin(TXT_JAVA_CANNOT_UPDATE);
            }
            break;
#endif
#ifdef JAVA_SUPPORT_SYSTEM_RESET_FUNCTION
       case ID_JAVA_RESET:
            MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),
                                TXT_JAVA_APP_RESET_SYSTEM_PROMPT, 
                                IMAGE_PUBWIN_QUERY,
                                PNULL,
                                HandleJavaResetSystemPromptWinMsg,
                                (uint32)cursuitid
                                );

            break;
#endif
        default: 
            break;
        }
        MMK_CloseWin(win_id);             
        break;
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:      
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/**************************************************************************************************/
// Description:HandleJavaAMSOptionInfoWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA 应用信息窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E  HandleJavaAMSOptionInfoWinMsg(
                                                  MMI_WIN_ID_T   win_id, 
                                                  MMI_MESSAGE_ID_E  msg_id, 
                                                  DPARAM                param
                                                  )
{

    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    int32                      cursuitid = (int32)MMK_GetWinAddDataPtr(win_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetTitleTextId(win_id,STXT_DETAIL,FALSE);
        SetJavaInfoDetail(MMIJAVA_DETAIL_NAME_CTRL_ID,cursuitid);
//        GUIRICHTEXT_SetSwitchFocusMode(MMIJAVA_DETAIL_NAME_CTRL_ID, GUIRICHTEXT_FOCUS_MODE_NULL);
        //set softkey        
        GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
        MMK_SetAtvCtrl(win_id,MMIJAVA_DETAIL_NAME_CTRL_ID);
        break;      

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : add one detail item
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void AddOneDetailItem(
                            MMI_CTRL_ID_T   ctrl_id,
                            MMI_TEXT_ID_T   label_title,
                            MMI_TEXT_ID_T   label_name,
                            uint16          *index_ptr,
                            wchar           *detail_ptr,
                            uint32          detail_len
                            )
{
    GUIRICHTEXT_ITEM_T item_data = {0};
    MMI_STRING_T detail_str = {0};
	//SCI_TRACE_LOW:"AddOneDetailItem enter "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3188_112_2_18_2_22_14_63,(uint8*)"");

	//set XXX title
	SetCommonCharacter(&item_data);
	MMI_GetLabelTextByLang( label_title, &detail_str );
	item_data.text_data.buf.len = detail_str.wstr_len;
	item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
	GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);	

    //set XXX name
    if ( PNULL == label_name && PNULL!=detail_ptr )
    {
        SCI_MEMSET(&item_data,0,sizeof(item_data));
        SetCommonCharacter(&item_data);
        item_data.text_data.buf.len = (uint16)detail_len;   
        item_data.text_data.buf.str_ptr = detail_ptr;
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);
    }
	else if (PNULL != label_name)
	{
	    SCI_MEMSET(&item_data,0,sizeof(item_data));
	    SCI_MEMSET(&detail_str,0,sizeof(detail_str));
	    SetCommonCharacter(&item_data);
	    MMI_GetLabelTextByLang( label_name, &detail_str );
	    item_data.text_data.buf.len = detail_str.wstr_len;
	    item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
	    GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);
	}	
    else
    {
        //SCI_TRACE_LOW:"AddOneDetailItem fail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3218_112_2_18_2_22_14_64,(uint8*)"");
    }

}
/**************************************************************************************************/
// Description:SetJavaInfoDetail
// Date:2007-06-02
// Author:huangjc
// Note:设置JAVA 应用信息，获得JAVA应用的名称、版本、服务提供商、大小、
//         域等信息
//*************************************************************************************************/
LOCAL void SetJavaInfoDetail(MMI_CTRL_ID_T ctrl_id, int32   cursuitid)
{
    uint8           version[MMIJAVA_VERSTR_MAX_LEN] = {0};
    uint8           size[MMIJAVA_SIZESTR_MAX_LEN]   = {0};
    // uint8           domain[2]                       = {0};
    MMI_STRING_T    name_str                        = {0};
    MMI_STRING_T    version_str                     = {0};
    MMI_STRING_T    vendor_str                      = {0};
    MMI_STRING_T    size_str                        = {0};
    MMI_STRING_T    domain_str                      = {0};  
    uint16          nIndex                          = 0;      
  	#ifdef  DRM_SUPPORT
    const AJ_U8 *		m_jarPath = AJ_NULL;     // merlin
    AJ_S32          	error_code;
    int32 				wlength = 0;
    wchar 				wfilename[MMIFILE_FILE_NAME_MAX_LEN] = {0};
	#endif
    //set name    
    name_str.wstr_ptr = SCI_ALLOC_APP((MMIJAVA_NAME_MAX_LEN + 1)*sizeof(wchar));
    SCI_MEMSET(name_str.wstr_ptr, 0, ((MMIJAVA_NAME_MAX_LEN + 1)*sizeof(wchar))); 
    name_str.wstr_len = GUI_UTF8ToWstr(name_str.wstr_ptr,
                                        MMIJAVA_NAME_MAX_LEN,
                                        s_suiteList[cursuitid].suiteName, 
                                        SCI_STRLEN((char *)s_suiteList[cursuitid].suiteName)
                                        );
    AddOneDetailItem(ctrl_id,TXT_INPUT_NAME,PNULL,&nIndex,name_str.wstr_ptr,name_str.wstr_len);
    if(PNULL != name_str.wstr_ptr)
    {
        SCI_FREE(name_str.wstr_ptr);
    }

    //set vision    
    SCI_MEMSET(&version, 0, sizeof(version));
    sprintf((char *)version, "%d.%d.%d", (uint8)s_suiteList[cursuitid].majorVersion, 
        (uint8)s_suiteList[cursuitid].minorVersion, (uint8)s_suiteList[cursuitid].microVersion);    
    version_str.wstr_len = strlen((char *)version);
    version_str.wstr_ptr = SCI_ALLOC_APP((version_str.wstr_len+1)*sizeof(wchar));   
    SCI_MEMSET(version_str.wstr_ptr,0,((version_str.wstr_len+1)*sizeof(wchar)));      
    MMIAPICOM_StrToWstr((uint8 *)version,version_str.wstr_ptr);
    AddOneDetailItem(ctrl_id,TXT_COMM_VERSION_1,PNULL,&nIndex,version_str.wstr_ptr,version_str.wstr_len);
    if(PNULL != version_str.wstr_ptr)
    {
        SCI_FREE(version_str.wstr_ptr);
    }

     // 3.set vendor
    vendor_str.wstr_ptr = SCI_ALLOCA((MMIJAVA_NAME_MAX_LEN + 1)*sizeof(wchar));
    //SCI_ASSERT(PNULL != vendor_str.wstr_ptr);
    if(PNULL != vendor_str.wstr_ptr)
    {
        SCI_MEMSET(vendor_str.wstr_ptr, 0, ((MMIJAVA_NAME_MAX_LEN + 1)*sizeof(wchar)));
        vendor_str.wstr_len = GUI_UTF8ToWstr(vendor_str.wstr_ptr,MMIJAVA_NAME_MAX_LEN,s_suiteList[cursuitid].vendor,SCI_STRLEN((char *)s_suiteList[cursuitid].vendor));
         
        AddOneDetailItem(ctrl_id,TXT_JAVA_DETAIL_VENDOR,PNULL,&nIndex,vendor_str.wstr_ptr,vendor_str.wstr_len);
        if(PNULL != vendor_str.wstr_ptr)
        {
            SCI_FREE(vendor_str.wstr_ptr);
        }
    }
     // 4.set size
    SCI_MEMSET(&size, 0, sizeof(size));
    sprintf((char *)size, "%ld.%03ldK", (s_suiteList[cursuitid].size /1024),((s_suiteList[cursuitid].size%1024)*1000/1024));

    //SCI_ASSERT(strlen((const char *)size) < sizeof(size));
    if(strlen((const char *)size) < sizeof(size))
    {
        size_str.wstr_len = strlen((char *)size);
        size_str.wstr_ptr = SCI_ALLOC_APP((size_str.wstr_len+1)*sizeof(wchar));
        SCI_MEMSET(size_str.wstr_ptr,0,((size_str.wstr_len+1)*sizeof(wchar)));
        MMIAPICOM_StrToWstr((uint8 *)size,size_str.wstr_ptr);    
        
        AddOneDetailItem(ctrl_id,TXT_DETAIL_SIZE,PNULL,&nIndex,size_str.wstr_ptr,size_str.wstr_len);
        if(PNULL != size_str.wstr_ptr)
        {
            SCI_FREE(size_str.wstr_ptr);
        }
    }
    
   // 5.set domain
   switch(s_suiteList[cursuitid].domainId)
   {
        case AJMS_PCS_DOMAIN_UNSPECIFIED:
    	 MMI_GetLabelTextByLang( TXT_JAVA_DOMAIN_UNSPECIFIED, &domain_str );
       	 break;
        case AJMS_PCS_DOMAIN_MANUFACTURER:
    	 MMI_GetLabelTextByLang( TXT_JAVA_DOMAIN_MANUFACTURER, &domain_str );
            break;
        case AJMS_PCS_DOMAIN_OPERATOR:
    	 MMI_GetLabelTextByLang( TXT_JAVA_DOMAIN_OPERATOR, &domain_str );
    	 break;
        case AJMS_PCS_DOMAIN_THIRD_PARTY:
    	 MMI_GetLabelTextByLang( TXT_JAVA_DOMAIN_THIRD_PARTY, &domain_str );
            break;
        default:
    	//SCI_TRACE_LOW:"SetJavaInfoDetail s_suiteList[cursuitid].domainId = %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3322_112_2_18_2_22_15_65,(uint8*)"d",s_suiteList[cursuitid].domainId);
            return;
   }    
    AddOneDetailItem(ctrl_id,TXT_JAVA_DETAIL_DOMAIN,PNULL,&nIndex,domain_str.wstr_ptr,domain_str.wstr_len);

#ifdef DRM_SUPPORT
	m_jarPath = ajms_ab_getJarPath(s_suiteList[cursuitid].id,&error_code);
	if(PNULL == m_jarPath)
	{
		return;
	}

	wlength = GUI_UTF8ToWstr(wfilename,MMIFILE_FILE_NAME_MAX_LEN,m_jarPath,strlen(m_jarPath));/*lint !e64*/
	if(MMIAPIDRM_IsDRMFile(NULL,wfilename))
	{
		MMIAPIFMM_AddDrmFileDetail(wfilename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0, ctrl_id);
	}
#endif

}
                                  
/*****************************************************************************/
//  Description :append one line icon text list item
//  Global resource dependence : none
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(
                               MMI_CTRL_ID_T       ctrl_id,
                               GUIITEM_STYLE_E    item_style,
                               MMI_TEXT_ID_T       left_softkey_id,
                               MMI_IMAGE_ID_T      icon_id,
                               const wchar*        wstr_ptr,
                               uint16              wstr_len,
                               uint16              pos,
                               BOOLEAN             is_update
                               )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16                  i         = 0;

    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;

    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    if( 0 != icon_id )
    {
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = icon_id;
        i++;
    }

    item_data.item_content[i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[i].item_data.text_buffer.wstr_len = MIN( wstr_len, GUILIST_STRING_MAX_NUM );
    item_data.item_content[i].item_data.text_buffer.wstr_ptr = (wchar*)wstr_ptr;
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }

}
#ifdef MMI_JAVA_NW_V2
void MMIJAVA_CreateNetworkSetting(int suitId)
{
    char                    idStr[20] = {0};//*dnw_setting.bin
    wchar                   setting_file[MMIFILE_FULL_PATH_MAX_LEN + 1]={0};
    char                    setting_file_ascii[MMIFILE_FULL_PATH_MAX_LEN + 1]={0};
    SFS_HANDLE              setting_handle = PNULL;
    MMIJAVA_LINK_SETTING_T  link_setting= {0};
    uint32                  bytes_written=0;      
    KJavaPathMappingEntry   javafolderpathmap ={0};
    //SCI_TRACE_LOW:"enter MMIJAVA_CreateNetworkSetting, suitId= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3399_112_2_18_2_22_15_66,(uint8*)"d", suitId);
    
    kjava_fs_getPath(0, &javafolderpathmap);

    sprintf(idStr,"%dnw_setting.bin",suitId);
    sprintf(setting_file_ascii,"%s%s",javafolderpathmap.path,idStr);

    //SCI_TRACE_LOW:"MMIJAVA_CreateNetworkSetting setting_file_ascii= %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3406_112_2_18_2_22_15_67,(uint8*)"s", setting_file_ascii);
    GUI_UTF8ToWstr(setting_file, MMIFILE_FULL_PATH_MAX_LEN + 1, (uint8*)setting_file_ascii, strlen(setting_file_ascii));
    if(!MMIAPIFMM_IsFileExist(setting_file, MMIAPICOM_Wstrlen(setting_file)))
    {
        setting_handle = MMIAPIFMM_CreateFile(setting_file, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, 0, 0);
    }
    else
    {
        setting_handle = MMIAPIFMM_CreateFile(setting_file, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, 0, 0);
    }
    //SCI_TRACE_LOW:"MMIJAVA_CreateNetworkSetting setting_handle= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3416_112_2_18_2_22_15_68,(uint8*)"d", setting_handle);
    if(setting_handle)
    {
        SCI_MEMSET(&link_setting,0,sizeof(MMIJAVA_LINK_SETTING_T));
        MMIAPIFMM_WriteFile(setting_handle, &link_setting, sizeof(MMIJAVA_LINK_SETTING_T),&bytes_written,0);
	    //SCI_TRACE_LOW:"MMIJAVA_CreateNetworkSetting bytes_written= %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3421_112_2_18_2_22_15_69,(uint8*)"d", bytes_written);
    }
    MMIAPIFMM_CloseFile(setting_handle);
}

void MMIJAVA_DeleteNetworkSetting(int suitId)
{
    char                    idStr[20] = {0};//Xdnw_setting.bin
    wchar                   network_setting_file[MMIFILE_FULL_PATH_MAX_LEN + 1]={0};
    char                    setting_file_ascii[MMIFILE_FULL_PATH_MAX_LEN + 1]={0};
    KJavaPathMappingEntry   javafolderpathmap ={0};
    
    kjava_fs_getPath(0, &javafolderpathmap);
    
    sprintf(idStr,"%dnw_setting.bin",suitId);
    sprintf(setting_file_ascii,"%s%s",javafolderpathmap.path,idStr);

    GUI_UTF8ToWstr(network_setting_file, MMIFILE_FULL_PATH_MAX_LEN + 1,(uint8*)setting_file_ascii, strlen(setting_file_ascii));
    MMIAPIFMM_DeleteFile(network_setting_file, PNULL);
}
#endif
/*****************************************************************************/
//  Description : Get JAVA net setting index 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
LOCAL MMIJAVA_LINK_SETTING_T MMIJAVA_GetNetSettingInfo(int32 curSuiteId)
{
    char                    idStr[20] = {0};//Xdnw_setting.bin
    char                    setting_file_ascii[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    wchar                   setting_file[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    SFS_HANDLE              setting_handle = PNULL;
    uint32                  bytes_read = 0;
    MMIJAVA_LINK_SETTING_T  link_setting = {0};
    KJavaPathMappingEntry   javafolderpathmap ={0};
    
    kjava_fs_getPath(0, &javafolderpathmap);
    
    sprintf(idStr,"%dnw_setting.bin",curSuiteId);
    
    sprintf(setting_file_ascii,"%s%s",javafolderpathmap.path,idStr);
    
    //SCI_TRACE_LOW:"MMIJAVA_GetNetSettingInfo file name = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3461_112_2_18_2_22_15_70,(uint8*)"s", setting_file_ascii);
    GUI_UTF8ToWstr(setting_file, MMIFILE_FULL_PATH_MAX_LEN+1, (uint8*)setting_file_ascii,strlen(setting_file_ascii));
    setting_handle = MMIAPIFMM_CreateFile(setting_file, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
    //SCI_TRACE_LOW:"MMIJAVA_GetNetSettingInfo setting file handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3464_112_2_18_2_22_15_71,(uint8*)"d", setting_handle);
    if(setting_handle)
    {
        MMIAPIFMM_ReadFile(setting_handle, &link_setting, sizeof(MMIJAVA_LINK_SETTING_T), &bytes_read, 0);
	    MMIAPIFMM_CloseFile(setting_handle);
    }

    return link_setting;
}

/*****************************************************************************/
//  Description : MMIJAVA_GetNetSettingIndex
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIJAVA_GetNetSettingIndex(void)
{
    uint8                   index = 0;
    MMIJAVA_LINK_SETTING_T link_setting = {0};
#ifdef MMI_JAVA_NW_V2
    char                    idStr[20] = {0};//Xdnw_setting.bin
    char                    setting_file_ascii[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    wchar                   setting_file[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    SFS_HANDLE              setting_handle = PNULL;
    uint32                  bytes_read = 0;
    KJavaPathMappingEntry   javafolderpathmap ={0};
    
    if(-1 == g_active_suite_id)
    {
    #ifdef BROWSER_SUPPORT
        index = MMIAPIBROWSER_GetNetSettingIndex(MN_DUAL_SYS_1);		
    #endif

        return index;
    }
#endif
#ifndef MMI_JAVA_NW_V2
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"MMIJAVA_GetNetSettingIndex enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3501_112_2_18_2_22_15_72,(uint8*)"");

    MMINV_READ(MMINV_JAVA_LINKSETTING_INFO, &link_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        return 0;
    }
#else
    kjava_fs_getPath(0, &javafolderpathmap);
    
    sprintf(idStr,"%dnw_setting.bin",g_active_suite_id);
    sprintf(setting_file_ascii,"%s%s",javafolderpathmap.path,idStr);


    //SCI_TRACE_LOW:"MMIJAVA_GetNetSettingIndex file name = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3515_112_2_18_2_22_15_73,(uint8*)"s", setting_file_ascii);
    GUI_UTF8ToWstr(setting_file, MMIFILE_FULL_PATH_MAX_LEN+1, (uint8*)setting_file_ascii,strlen(setting_file_ascii));
    setting_handle = MMIAPIFMM_CreateFile(setting_file, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
    //SCI_TRACE_LOW:"MMIJAVA_GetNetSettingIndex setting file handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3518_112_2_18_2_22_15_74,(uint8*)"d", setting_handle);
    if(setting_handle)
    {
        MMIAPIFMM_ReadFile(setting_handle, &link_setting, sizeof(MMIJAVA_LINK_SETTING_T), &bytes_read, 0);
	    MMIAPIFMM_CloseFile(setting_handle);
    }
    
#endif 	
    if(MN_DUAL_SYS_1 <= (MN_DUAL_SYS_E)link_setting.network_type
        && (MN_DUAL_SYS_1 + MMI_DUAL_SYS_MAX) > (MN_DUAL_SYS_E)link_setting.network_type)
    {
        index = link_setting.sim_index[link_setting.network_type];
    }
    
    return index;
}


LOCAL BOOLEAN MMIJAVA_SetNetSettingInfo(int32 curSuiteId,MMIJAVA_LINK_SETTING_T setting_info)
{
    char                    idStr[20] = {0};//Xdnw_setting.bin    
    char                    setting_file_ascii[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    wchar                   setting_file[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    SFS_HANDLE              setting_handle = PNULL;
    uint32                  bytes_written = 0; 
    BOOLEAN                 result = FALSE;
    KJavaPathMappingEntry   javafolderpathmap ={0};
    
    kjava_fs_getPath(0, &javafolderpathmap);
    
    sprintf(idStr,"%dnw_setting.bin",curSuiteId);
    sprintf(setting_file_ascii,"%s%s",javafolderpathmap.path,idStr);

    //SCI_TRACE_LOW:"MMIJAVA_SetNetSettingInfo setting file:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3551_112_2_18_2_22_15_75,(uint8*)"s",setting_file_ascii);
    GUI_UTF8ToWstr(setting_file, MMIFILE_FULL_PATH_MAX_LEN+1, (uint8*)setting_file_ascii,strlen(setting_file_ascii));
    setting_handle = MMIAPIFMM_CreateFile(setting_file, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS,0,0);
    //SCI_TRACE_LOW:"MMIJAVA_SetNetSettingInfo setting file handle:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3554_112_2_18_2_22_15_76,(uint8*)"d",setting_handle);
    if(setting_handle)
    {  
        MMIAPIFMM_SetFilePointer(setting_handle, 0, SFS_SEEK_BEGIN);
        MMIAPIFMM_WriteFile(setting_handle, &setting_info, sizeof(MMIJAVA_LINK_SETTING_T), &bytes_written, 0);
        //SCI_TRACE_LOW:"MMIJAVA_SetNetSettingInfo bytes_written=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3559_112_2_18_2_22_15_77,(uint8*)"d", bytes_written);
	    MMIAPIFMM_CloseFile(setting_handle);
        result = TRUE;
    }
    
    return result;
}

/**************************************************************************************************/
// Description:SetJavaInstallInfo
// Date:2007-06-08
// Author:huangjc
// Note:设置JAVA安装详细信息(JAD描述信息窗口信息的获取)
//*************************************************************************************************/
LOCAL void SetJavaInstallInfo(AJMS_CheckResult check_result, MMI_CTRL_ID_T ctrl_id)
{
    uint8           version[MMIJAVA_VERSTR_MAX_LEN] = {0};
    uint8           size[MMIJAVA_SIZESTR_MAX_LEN]= {0};
    MMI_STRING_T    name_str= {0};
    MMI_STRING_T    version_str= {0};
    MMI_STRING_T    vendor_str= {0};
    MMI_STRING_T    size_str= {0};
    uint16          nIndex = 0;
#ifdef  DRM_SUPPORT
    const AJ_U8 *		m_jarPath = AJ_NULL;    
    //AJ_S32          	error_code;
    int32 				wlength = 0;
    uint16 				wfilename[MMIFILE_FILE_NAME_MAX_LEN] = {0};
#endif

    // 1.set name
    name_str.wstr_ptr = SCI_ALLOCA((MMIJAVA_NAME_MAX_LEN + 1)*sizeof(wchar));
    //SCI_ASSERT(PNULL != name_str.wstr_ptr);
    if(PNULL != name_str.wstr_ptr)
    {
        SCI_MEMSET(name_str.wstr_ptr, 0, ((MMIJAVA_NAME_MAX_LEN + 1)*sizeof(wchar)));
        name_str.wstr_len = GUI_UTF8ToWstr(
            name_str.wstr_ptr, 
            MMIJAVA_NAME_MAX_LEN,
            (uint8*)check_result.name, 
            SCI_STRLEN((char *)check_result.name)
            );    
        AddOneDetailItem(
            ctrl_id,
            TXT_INPUT_NAME,
            PNULL,
            &nIndex,
            name_str.wstr_ptr,
            name_str.wstr_len);
        //free memory
        if(PNULL != name_str.wstr_ptr)
        {
            SCI_FREE(name_str.wstr_ptr);
            name_str.wstr_ptr= PNULL;
        }
    }
    
    // 2.set version
    SCI_MEMSET(&version, 0, sizeof(version));
    sprintf((char *)version, "%d.%d.%d", (uint8)check_result.majorVersion, 
        (uint8)check_result.minorVersion, 
        (uint8)check_result.microVersion);
    if(strlen((const char *)version) < sizeof(version))
    {
        //SCI_ASSERT(strlen((const char *)version) < sizeof(version));    
        version_str.wstr_len = strlen((char *)version);
        version_str.wstr_ptr = SCI_ALLOC_APP((version_str.wstr_len+1) * sizeof(wchar));
        SCI_MEMSET(version_str.wstr_ptr,0,((version_str.wstr_len+1)*sizeof(wchar)));
        MMIAPICOM_StrToWstr((const uint8 *)version,version_str.wstr_ptr);
        AddOneDetailItem(
            ctrl_id,
            TXT_COMM_VERSION_1,
            PNULL,
            &nIndex,
            version_str.wstr_ptr,
            version_str.wstr_len);
        SCI_FREE(version_str.wstr_ptr);
    }
    
    // 3.set vendor
    vendor_str.wstr_ptr = SCI_ALLOCA((MMIJAVA_NAME_MAX_LEN + 1)*sizeof(wchar));
    if(PNULL != vendor_str.wstr_ptr)
    {
        //SCI_ASSERT(PNULL != vendor_str.wstr_ptr);
        SCI_MEMSET(vendor_str.wstr_ptr, 0, ((MMIJAVA_NAME_MAX_LEN + 1)*sizeof(wchar)));
        vendor_str.wstr_len = GUI_UTF8ToWstr(
            vendor_str.wstr_ptr, 
            MMIJAVA_NAME_MAX_LEN,
            (uint8*)check_result.vendor, 
            SCI_STRLEN((char *)check_result.vendor)
            ); 
        AddOneDetailItem(
            ctrl_id,
            TXT_JAVA_DETAIL_VENDOR,
            PNULL,
            &nIndex,
            vendor_str.wstr_ptr,
            vendor_str.wstr_len);
        //free memory
        if(PNULL != vendor_str.wstr_ptr)
        {
            SCI_FREE(vendor_str.wstr_ptr);
            vendor_str.wstr_ptr= PNULL;
        }
    }
    // 4.set size
    SCI_MEMSET(&size, 0, sizeof(size));
    sprintf((char *)size, "%ld.%03ldK", (check_result.jarSize /1024),((check_result.jarSize %1024)*1000/1024));
    
    if(strlen((const char *)size) < sizeof(size))
    {
        //SCI_ASSERT(strlen((const char *)size) < sizeof(size));   
        size_str.wstr_len = strlen((char *)size);
        size_str.wstr_ptr = SCI_ALLOC_APP((size_str.wstr_len+1)*sizeof(wchar));
        SCI_MEMSET(size_str.wstr_ptr,0,((size_str.wstr_len+1)*sizeof(wchar)));
        MMIAPICOM_StrToWstr((const uint8 *)size,size_str.wstr_ptr);    
        AddOneDetailItem(
            ctrl_id,
            TXT_DETAIL_SIZE,
            PNULL,
            &nIndex,
            size_str.wstr_ptr,
            size_str.wstr_len);
        SCI_FREE(size_str.wstr_ptr)
    }
    
    #ifdef DRM_SUPPORT
	m_jarPath = s_install_data_info_ptr->jar_file_path;
	if(PNULL == m_jarPath)
	{
		return;
	}

	wlength = GUI_UTF8ToWstr(wfilename,MMIFILE_FILE_NAME_MAX_LEN,(uint8 *)m_jarPath,strlen(m_jarPath));/*lint !e64*/		

	MMIAPIFMM_AddDrmFileDetail(wfilename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0, ctrl_id);
    #endif
}


#ifdef SELECT_INSTALL_DISK_SUPPORT
/*2010-8-24 hanker.zhou for folder hidden begin*/	
/*****************************************************************************/
// Description : 创建java目录
// Global resource dependence :
// Author:hanker.zhou
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitJavaFolder(MMIFILE_DEVICE_E disk)
{
    wchar       full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16      full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
	wchar	    *file_dev_ptr = PNULL;
	uint16	    file_dev_len = 0;
    BOOLEAN     result = FALSE;
	const wchar javafolder[]      = { '@','J', 'a' ,'v','a', 0 };	
	uint16 javafolder_len=5;
    //MMIFILE_DEVICE_E dev      = MMI_DEVICE_UDISK;
    //MMIFILE_DEVICE_E     file_dev = MMI_DEVICE_UDISK;
    //BOOLEAN          dev_ok   = FALSE;

	//SCI_TRACE_LOW:"InitPBFolder enter"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3717_112_2_18_2_22_15_78,(uint8*)"");
    
    if(disk >= MMI_DEVICE_NUM)
    {
        return FALSE;
    }
    file_dev_ptr = MMIAPIFMM_GetDevicePath(disk);	/*lint !e605 */	
    file_dev_len = MMIAPIFMM_GetDevicePathLen(disk); 
    
    if(MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
		javafolder, javafolder_len, 
		NULL, 0, 
		full_path_name, &full_path_len))
    {
		if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
		{
			result = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
			
		}
		else
		{
			result = TRUE;
		}
        
        if(result)
        {
	        //SCI_TRACE_LOW:"MMIAPIFMM_SetAttr result %d"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3743_112_2_18_2_22_15_79,(uint8*)"d",result);
			MMIAPIFMM_SetAttr(full_path_name, full_path_len, TRUE, FALSE, FALSE, TRUE);			
        }
    }
    
	//SCI_TRACE_LOW:"InitPBFolder result %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3748_112_2_18_2_22_15_80,(uint8*)"d",result);
    return result;
}
/*2010-8-24 hanker.zhou for folder hidden end*/	
/****************************************************************************
*函数定义:Java_LocalInstall_to
*函数说明:
*参数说明:
*返回值:
*hanker zhou
*****************************************************************************/
LOCAL BOOLEAN Java_LocalInstall_To(
					            const char *installto,
                                 int32 jad_data_size,
                                 const uint8 *jad_data_addr,      
                                 int32 jar_data_size,
                                 const uint8 *jar_data_addr, 
                                 const uint8 *jar_file_path,
                                 int32 old_id,
                                 int32 ams
                                 )
{
    int32           result = 0;    
    uint8*          url = PNULL;  
    AJ_S32          categoryId = AJMS_DEFAULT_CATEGORY_ID_UNFILED;
    uint16          device_len = 0;
    uint32          free_high = 0;
    uint32          free_low = 0;	
    const AJ_U8*    jarStorageName = AJ_NULL;     // merlin
    wchar           w_jarStorageName[MMIFILE_FULL_PATH_MAX_LEN+1] ={0};
    AJ_S32          errorCode;
    MMIFILE_DEVICE_E   device_type = s_device_arr[current_item];
    const wchar*    device_ptr = PNULL;
    char            c_device_path[MMIFILE_DEVICE_LEN +1] = {0};

    //SCI_TRACE_LOW:"Java_LocalInstall_To current_item=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3782_112_2_18_2_22_15_81,(uint8*)"d",current_item);
    //SCI_TRACE_LOW:"Java_LocalInstall_To current_item=%d %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3783_112_2_18_2_22_15_82,(uint8*)"dd",s_device_arr[0],s_device_arr[1]);
    //SCI_TRACE_LOW:"Java_LocalInstall_To jar_data_size = %d  cur_item=%d,disk_name=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3784_112_2_18_2_22_15_83,(uint8*)"ddd",jar_data_size,current_item,s_device_arr[current_item]);

	if(PNULL != jar_file_path ) //for  drm java install
    {
     	if(NULL !=s_install_data_info_ptr->jar_file_handler)
		{
			MMIAPIFMM_CloseFile(s_install_data_info_ptr->jar_file_handler );
            s_install_data_info_ptr->jar_file_handler = NULL;
		}
        
	    //SCI_TRACE_LOW:"Java_LocalInstall_To drm = %s"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3794_112_2_18_2_22_16_84,(uint8*)"s",jar_file_path);
        //reset cancel flag 
        s_b_cancel_install = FALSE;
        result = ajms_ota_installEx( AJMS_DEFAULT_CATEGORY_ID_UNFILED,
                         PNULL,
                         PNULL,
                         jar_file_path,
                         ams,
                         old_id,
                        &s_local_install_result, 
                        Java_LocalInstallCallBack );
    }
    else //for normal install
    {
        if(old_id >= 0)
        {
            jarStorageName = ajms_ab_getJarPath(old_id,&errorCode);
            if(PNULL == jarStorageName)
            {
                return FALSE;
            }
            
            MMI_STRNTOWSTR(w_jarStorageName, MMIFILE_FULL_PATH_MAX_LEN, jarStorageName, strlen((char*)jarStorageName), strlen((char*)jarStorageName));
            device_type = MMIAPIFMM_GetDeviceTypeByPath(w_jarStorageName, MMIFILE_DEVICE_LEN);
            s_device_arr[current_item] = device_type;
            if(device_type < MMI_DEVICE_NUM)
            {
                device_ptr = MMIAPIFMM_GetDevicePath(device_type);
                if(PNULL == device_ptr)
                {
                    return FALSE;
                }

                device_len = MMIAPIFMM_GetDevicePathLen(device_type);

                MMI_WSTRNTOSTR((uint8*)c_device_path,MMIFILE_DEVICE_LEN,device_ptr,device_len,device_len);
                MMIAPICOM_Strlower(c_device_path);
                sprintf((char*)s_java_installtoPath,"gfs://java%s/",c_device_path);
            }        
        }
    	else if(device_type < MMI_DEVICE_NUM)
        {
    		//SCI_TRACE_LOW:"Java_LocalInstall_To  current_item=%d"
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3836_112_2_18_2_22_16_85,(uint8*)"d",current_item);
            if(device_type == MMI_DEVICE_SYSTEM)
            {
    	        categoryId=AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED;
            }
            
            device_ptr = MMIAPIFMM_GetDevicePath(device_type);
            if(PNULL == device_ptr)
            {
                return FALSE;
            }
            
            device_len = MMIAPIFMM_GetDevicePathLen(device_type);
            
            MMI_WSTRNTOSTR((uint8 *)c_device_path,MMIFILE_DEVICE_LEN,device_ptr,device_len,device_len);
            MMIAPICOM_Strlower(c_device_path);
            SCI_MEMSET(s_java_installtoPath,0,sizeof(s_java_installtoPath));
            sprintf((char*)s_java_installtoPath,"gfs://java%s/",c_device_path);

        }
        
        if(PNULL != device_ptr)
        {
            MMIAPIFMM_GetDeviceFreeSpace(device_ptr, device_len, & free_high, & free_low);
            if(((s_install_data_info_ptr->jar_len + s_local_check_result.minimumInstallSize) >= free_low)&&(0 == free_high))
            {
                MMIPUB_OpenAlertWarningWin(TXT_FMM_SPACE_IS_NOT_ENOUGH);
                return FALSE;
            }
        }
        //check @java folder   
        #ifndef IA_JAVA_SUPPORT_LOW_MEMORY
        InitJavaFolder(device_type);	
        #endif
        
        if ((PNULL != s_install_data_info_ptr) &&s_install_data_info_ptr->is_ota)
        {
            url = s_install_data_info_ptr->url;
            //SCI_TRACE_LOW:"Java_LocalInstall_To url=%s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3874_112_2_18_2_22_16_86,(uint8*)"s",url);
        }    
        //SCI_TRACE_LOW:"Java_LocalInstall_To jadinstalltoPath = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3876_112_2_18_2_22_16_87,(uint8*)"s",s_java_installtoPath);
    	
    	//AJMS_INSTALL_KEEP_RMS
    	if(old_id>=0)
    	{
    		AJMS_Suite  p_suiteList[MAXJAVALISTSIZE];   
    		int32       iNumOfSuite;
    		int32       j;
    		iNumOfSuite= ajms_ab_getSutieListCount(AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED);   

            if ( ajms_ab_getSuiteList( AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED , p_suiteList, 0 ) < 0 )	
            {
                //have no preinstall midlet
                //do some process here.
                categoryId=AJMS_DEFAULT_CATEGORY_ID_UNFILED;
            }
            
            for(j=0;j<iNumOfSuite;j++)
            {
                if(p_suiteList[j].id==old_id)
                {
                	categoryId=AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED;
                }
            }
    		
    	}
        //reset cancel flag 
        s_b_cancel_install = FALSE;

        
        #ifdef JAVA_INSTALL_FILE_READER
        if(jad_data_size > 0)
        {
            result=ajms_ota_install_to( categoryId,
                                    AJMS_APPLICATION_TYPE_2 ,
                                    s_java_installtoPath,
                                    s_java_installtoPath,
                                    jad_data_size,
                                    AJ_NULL,
                                    InstallJadDataReader,
                                    url,  // this is for uptate url where midlet to get the newest midlet
                                    jar_data_size,
                                    AJ_NULL,
                                    InstallJarDataReader,
                                    ams,
                                    old_id,
                                    &s_local_install_result, 
                                    Java_LocalInstallCallBack);	
        }
        else
        {
            result=ajms_ota_install_to( categoryId,
                                AJMS_APPLICATION_TYPE_2 ,
                                s_java_installtoPath,
                                s_java_installtoPath,
                                jad_data_size,
                                AJ_NULL,
                                AJ_NULL,
                                url,  // this is for uptate url where midlet to get the newest midlet
                                jar_data_size,
                                AJ_NULL,
                                InstallJarDataReader,
                                ams,
                                old_id,
                                &s_local_install_result, 
                                Java_LocalInstallCallBack);	
        }
        #else
        result=ajms_ota_install_to( categoryId,
                         AJMS_APPLICATION_TYPE_2 ,
                            s_java_installtoPath,
                            s_java_installtoPath,
                            jad_data_size,
                            jad_data_addr,
                            AJ_NULL,
                            url,  // this is for uptate url where midlet to get the newest midlet
                            jar_data_size,
                            jar_data_addr,
                            AJ_NULL,
                            ams,
                            old_id,
                           &s_local_install_result, 
                            Java_LocalInstallCallBack);	
        #endif
    }

    if( AJMS_ERR_NONE == result )
    {
        //SCI_TRACE_LOW:"Java_LocalInstall_To TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3964_112_2_18_2_22_16_88,(uint8*)"");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"Java_LocalInstall_To FLASE result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3969_112_2_18_2_22_16_89,(uint8*)"d",result);
        return FALSE;
    }

    
}


/**************************************************************************************************/
// Description:HandleJavaSelectInstallDiskWinMsg
// Author:hanekr 
// Note:JAVA 应用列表窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaSelectInstallDiskWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E                result      = MMI_RESULT_TRUE;
    static int32                       cursuitid   = -1;
    // int8 closeresult=0;
    //SCI_TRACE_LOW:"HandleJavaSelectInstallDiskWinMsg, msg_id = 0x%02x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_3990_112_2_18_2_22_16_90,(uint8*)"d", msg_id);


    switch (msg_id)
    {    
    case MSG_OPEN_WINDOW:    
        cursuitid   = 0;
        GUIWIN_SetTitleTextId(win_id,TXT_JAVA_INSTALL_SELECT_DISK,FALSE);
        GUILIST_SetMaxItem(MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, 4, FALSE);
        // List all of the midlet suite
        if(CreateInstallDiskListBox())
        {        
            GUILIST_SetTitleIndexType( MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, GUILIST_TITLE_INDEX_DEFAULT );
#ifndef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);    	
#endif            
            MMK_SetAtvCtrl(win_id,MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID);
  
        }
        else
        {
            MMK_CloseWin(win_id);
        }          
        break;

    case MSG_GET_FOCUS:
        if(CreateInstallDiskListBox())
        {        
            GUILIST_SetTitleIndexType( MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, GUILIST_TITLE_INDEX_DEFAULT );            
        }               
        break;
        
    case MSG_FULL_PAINT:
        GUILIST_SetCurItemIndex(MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, cursuitid);
        GUILIST_SetSelectedItem(MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, cursuitid, FALSE);     
        break;

    case MSG_CTL_MIDSK:  
    case MSG_KEYDOWN_WEB:
        cursuitid = GUILIST_GetCurItemIndex(MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID);
        //SCI_TRACE_LOW:"HandleJavaSelectInstallDiskWinMsg cursuitid = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4030_112_2_18_2_22_16_91,(uint8*)"d",cursuitid);
	    current_item=cursuitid;
        MMK_CreateWin((uint32*)MMIJAVA_INSTALL_WIN_TAB, (ADD_DATA)AJMS_INSTALL_DEFAULT);
        MMK_CloseWin(win_id);
        break;
      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:        
    case MSG_CTL_OK:
        cursuitid = GUILIST_GetCurItemIndex(MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID);   
        //SCI_TRACE_LOW:"HandleJavaSelectInstallDiskWinMsg cursuitid = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4040_112_2_18_2_22_16_92,(uint8*)"d",cursuitid);
	    current_item=cursuitid;	
        MMK_CreateWin((uint32 *)MMIJAVA_INSTALL_WIN_TAB, (ADD_DATA)AJMS_INSTALL_DEFAULT);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );        
		MMK_CloseWin( win_id);
        break;  
        
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif
/**************************************************************************************************/
// Description:HandleInstallInfoWinMsg
// Date:2007-06-08
// Author:huangjc
// Note:本地安装信息处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E  HandleInstallInfoWinMsg(
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
  
    MMI_RESULT_E        result = MMI_RESULT_TRUE;      
    MMI_WIN_ID_T        query_win_id = MMIJAVA_QUERY_WIN_ID;
    static  BOOLEAN     s_is_install = FALSE;  

    //SCI_TRACE_LOW:"HandleInstallInfoWinMsg msg_id =%x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4078_112_2_18_2_22_16_93,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:       
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        SetJavaInstallInfo(s_local_check_result, MMIJAVA_JAD_INFO_CTRL_ID);
        //set softkey
        MMK_SetAtvCtrl(win_id,MMIJAVA_JAD_INFO_CTRL_ID);
        //set softkey
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_PDA_SUPPORT
     case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                 MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#else
    case MSG_CTL_PENOK:
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:  
        s_is_install = FALSE; 
	    current_item=0;
        SCI_MEMSET(s_device_arr,MMI_DEVICE_NUM,sizeof(s_device_arr));//reset disk record info
        //SCI_TRACE_LOW:"HandleInstallInfoWinMsg jar_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4121_112_2_18_2_22_16_94,(uint8*)"d",s_local_check_result.jarSize);
        switch( s_local_check_result.update)
        {
        case AJMS_UPDATE_NEWER_VERSION:     
            //应用管理器中存在此应用，但不是最新的
            MMIPUB_OpenQueryWinByTextId(TXT_JAVA_APP_INSTALL_UPDATE, 
                                        IMAGE_PUBWIN_QUERY, 
                                        &query_win_id, 
                                        PNULL);
            
            break;           
            
        case AJMS_UPDATE_SAME_VERSION:           
            //应用管理器中存在此应用，而且版本一致
            MMIPUB_OpenQueryWinByTextId(TXT_JAVA_APP_INSTALL_REPLACE, 
                                        IMAGE_PUBWIN_QUERY, 
                                        &query_win_id, 
                                        PNULL);        
            
            break;          
            
        case AJMS_UPDTAE_OLDER_VERSION:          
            //应用管理器中存在此应用，但比较旧
            MMIPUB_OpenQueryWinByTextId(TXT_JAVA_APP_INSTALL_ROLLBACK, 
                                        IMAGE_PUBWIN_QUERY, 
                                        &query_win_id, 
                                        PNULL);
            
            break;            
            
        case AJMS_UPDATE_MIDLET_SUITE_NOT_EXIST:
#ifdef SELECT_INSTALL_DISK_SUPPORT
#ifdef MMI_PDA_SUPPORT
             MMK_CreatePubListWin((uint32* )MMIJAVA_APP_INSTALL_SELECT_WIN_TAB, (ADD_DATA)AJMS_INSTALL_DEFAULT); 
#else
            MMK_CreateWin((uint32* )MMIJAVA_APP_INSTALL_SELECT_WIN_TAB, (ADD_DATA)AJMS_INSTALL_DEFAULT); 
#endif            
#else
            MMK_CreateWin((uint32* )MMIJAVA_INSTALL_WIN_TAB, (ADD_DATA)AJMS_INSTALL_DEFAULT);  

#endif			
            MMK_CloseWin(win_id);
            break;

        default:          
            MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_COMM_INSTALL_FAILED, 
                TXT_NULL, 
                IMAGE_PUBWIN_FAIL, 
                PNULL, 
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL);  
            MMK_CloseWin(win_id);
            MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );        
            break;                       
           
        }
        break;
        
    case MSG_JAVA_CHECK_AMS:
        MMIPUB_OpenQueryWinByTextId(TXT_JAVA_CHECKRMS,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        s_is_install = TRUE;
        break; 

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(&query_win_id);
        if (!s_is_install)
        {            
            MMK_PostMsg( ( MMI_WIN_ID_T )win_id, MSG_JAVA_CHECK_AMS, PNULL, 0 );   
            break;
        }    
        else
        {
            MMK_CreateWin((uint32* )MMIJAVA_INSTALL_WIN_TAB, (ADD_DATA)AJMS_INSTALL_KEEP_RMS);  
            MMK_CloseWin(win_id);
            break;
        }
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id); 
        if (s_is_install)
        {  
            MMK_CreateWin((uint32* )MMIJAVA_INSTALL_WIN_TAB, (ADD_DATA)AJMS_INSTALL_DEFAULT);  
            MMK_CloseWin(win_id);
            break;
           
        }
        MMK_CloseWin(win_id);
        MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );   
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:        
        MMK_CloseWin(win_id);
        MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );       
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}


/**************************************************************************************************/
// Description:OpenJavaFile
// Date:2007-06-08
// Author:huangjc
// Note:尝试打开JAVA应用安装文件，判断该文件是否完好无损,需要释放BLOCK_MEM_POOL_JAVA
//*************************************************************************************************/
LOCAL MMI_JAVA_OPEN_RESULT_E OpenJavaFile(wchar *full_name, MMIJAVA_JAR_FILE_INFO_T  *file_info_ptr)
{
  //  BOOLEAN     result = FALSE;
    // uint32       lyric_len_readed = 0;
    // MMIFILE_ERROR_E lyric_error = SFS_NO_ERROR;
    SFS_HANDLE    java_file_handle = 0; /*java文件的指针*/

    //SCI_TRACE_LOW:"mmijava_wintab.c, OpenJarFile() enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4241_112_2_18_2_22_16_95,(uint8*)"");

    java_file_handle = MMIAPIFMM_CreateFile(
                                        full_name,
                                        SFS_MODE_OPEN_EXISTING | SFS_MODE_READ,
                                        NULL,
                                        NULL);

    if (SFS_INVALID_HANDLE == java_file_handle)
    {
        //SCI_TRACE_LOW:"mmijava_wintab.c, OpenJarFile(), FALSE 1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4251_112_2_18_2_22_16_96,(uint8*)"");
        return MMI_JAVA_OPEN_RESULT_FALSE;
    }

    file_info_ptr->file_len = MMIAPIFMM_GetFileSize(java_file_handle);
    //SCI_TRACE_LOW:"mmijava_wintab.c, OpenJarFile(), file_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4256_112_2_18_2_22_16_97,(uint8*)"d", file_info_ptr->file_len);

    if (0 == file_info_ptr->file_len)
    {
        MMIAPIFMM_CloseFile(java_file_handle);
        //SCI_TRACE_LOW:"mmijava_wintab.c, OpenJarFile(), FALSE 2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4261_112_2_18_2_22_16_98,(uint8*)"");
        return MMI_JAVA_OPEN_RESULT_FALSE;
    }
//buffer方式执行这些代码
#ifndef JAVA_INSTALL_FILE_READER
    if (file_info_ptr->file_len > MMIJAVA_INSTALL_JAR_MAX_SIZE)
    {
        MMIAPIFMM_CloseFile(java_file_handle);
        //SCI_TRACE_LOW:"mmijava_wintab.c, OpenJarFile(), FALSE 2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4269_112_2_18_2_22_16_99,(uint8*)"");
        return MMI_JAVA_OPEN_RESULT_TOO_BIG_FAIL;
    }

    /*注意! 安装完需要释放该指针*/

    file_info_ptr->file_content_ptr = SCI_ALLOC_APP(file_info_ptr->file_len+1);

    SCI_MEMSET(file_info_ptr->file_content_ptr,0,(file_info_ptr->file_len+1));

    //SCI_TRACE_LOW:"OpenJarFile file_info_ptr->file_content_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4279_112_2_18_2_22_16_100,(uint8*)"d",file_info_ptr->file_content_ptr);
    if (PNULL == file_info_ptr->file_content_ptr)
    {
        MMIAPIFMM_CloseFile(java_file_handle);
        //SCI_TRACE_LOW:"mmijava_wintab.c, OpenJarFile(), FALSE 3"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4283_112_2_18_2_22_16_101,(uint8*)"");
        return MMI_JAVA_OPEN_RESULT_FALSE;
    }

    //init local variable
    file_info_ptr->file_pos = 0;

    lyric_error = MMIAPIFMM_ReadFile(
        java_file_handle,
        file_info_ptr->file_content_ptr,
        file_info_ptr->file_len,
        &lyric_len_readed,
        PNULL
        );

    MMIAPIFMM_CloseFile(java_file_handle);

    if (SFS_NO_ERROR != lyric_error )
    {

        SCI_FREE(file_info_ptr->file_content_ptr);
        //SCI_TRACE_LOW:"mmijava_wintab.c, OpenJarFile(), FALSE 4, error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4304_112_2_18_2_22_17_102,(uint8*)"d", lyric_error);
        return MMI_JAVA_OPEN_RESULT_FALSE;
    }
    else
#endif
    {
        file_info_ptr->file_handler = java_file_handle;
        //SCI_TRACE_LOW:"mmijava_wintab.c, OpenJarFile(), TRUE 5"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4311_112_2_18_2_22_17_103,(uint8*)"");
        return MMI_JAVA_OPEN_RESULT_TRUE;
    }
}
/**************************************************************************************************/
// Description:ReleaseInstallDataInfo
// Date:20110113
// Author:murphyxie
// Note:s_install_data_info_ptr release function
//*************************************************************************************************/
LOCAL void ReleaseInstallDataInfo()
{
    if (PNULL != s_install_data_info_ptr)
    {
        if (PNULL != s_install_data_info_ptr->jar_ptr)
        {
            SCI_FREE(s_install_data_info_ptr->jar_ptr);
        }
        if (PNULL != s_install_data_info_ptr->jad_ptr)
        {
            SCI_FREE(s_install_data_info_ptr->jad_ptr);
        }
        
        if (PNULL != s_install_data_info_ptr->url)
        {
            SCI_FREE(s_install_data_info_ptr->url);
        }
        
        if (NULL != s_install_data_info_ptr->jar_file_handler)
        {
            MMIAPIFMM_CloseFile(s_install_data_info_ptr->jar_file_handler);
            s_install_data_info_ptr->jar_file_handler = NULL;
        }
        if (NULL != s_install_data_info_ptr->jad_file_handler)
        {
            MMIAPIFMM_CloseFile(s_install_data_info_ptr->jad_file_handler);
            s_install_data_info_ptr->jad_file_handler = NULL;
        }

        if(PNULL != s_install_data_info_ptr->jar_file_path)
        {
            SCI_FREE(s_install_data_info_ptr->jar_file_path);
        }

        s_install_data_info_ptr->is_ota = FALSE;
        SCI_FREE(s_install_data_info_ptr);
        s_install_data_info_ptr = PNULL;
    }
}
/**************************************************************************************************/
// Description:HandleCheckWinMsg
// Date:2007-06-08
// Author:huangjc
// Note:Jad/Jar解析窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleCheckWinMsg(
                                     MMI_WIN_ID_T            win_id, 
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM                      param
                                     )
{
    
    MMI_RESULT_E    recode = MMI_RESULT_TRUE; 
    uint32          check_data_len = 0; 
    uint8*          check_data_ptr = PNULL;    
    BOOLEAN         is_jad = FALSE;   
    
    //SCI_TRACE_LOW:"HandleCheckWinMsg msg_id = %x, "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4376_112_2_18_2_22_17_104,(uint8*)"d",msg_id);
    
    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:  

#ifdef UI_P3D_SUPPORT
        MMI_Disable3DEffect(MMI_3D_EFFECT_JAVA);
#endif     

        #ifdef JAVA_INSTALL_FILE_READER
        if( NULL != s_install_data_info_ptr->jad_file_handler)
        {            
            check_data_len = s_install_data_info_ptr->jad_len;
            is_jad = TRUE;
        }
        else if( NULL != s_install_data_info_ptr->jar_file_handler )
        {            
            check_data_len = s_install_data_info_ptr->jar_len;
            is_jad = FALSE;
        }
        #else
        if( PNULL != s_install_data_info_ptr->jad_ptr )
        {            
            check_data_len = s_install_data_info_ptr->jad_len;
            check_data_ptr = s_install_data_info_ptr->jad_ptr;
            is_jad = TRUE;
        }
        else if( PNULL != s_install_data_info_ptr->jar_ptr )
        {            
            check_data_len = s_install_data_info_ptr->jar_len;
            check_data_ptr = s_install_data_info_ptr->jar_ptr;
            is_jad = FALSE;
        }
        #endif
        else
        {            
            MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_COMM_INSTALL_FAILED, 
                TXT_NULL, 
                IMAGE_PUBWIN_FAIL, 
                PNULL, 
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL);  

            MMK_CloseWin(win_id);
        }
      
        if( !LocalCheck( check_data_len,check_data_ptr,is_jad ))
        {
            if(PNULL != s_install_data_info_ptr && s_install_data_info_ptr->is_ota)
		    {
			    if(0 != strlen((char*)s_local_check_result.installNotifyUrl))
                {
				    if(jwe_ota_notify(	s_local_check_result.otaCode, 
								    (char*) s_local_check_result.installNotifyUrl,
								    strlen((char*)s_local_check_result.installNotifyUrl)
								    ))
				    {
                        s_ota_notify_is_success = FALSE;
                    }
			    }									
		    }
            MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_COMM_INSTALL_FAILED, 
                TXT_NULL, 
                IMAGE_PUBWIN_FAIL, 
                PNULL, 
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL);  

            MMK_CloseWin(win_id);
        }    
 
        
        break;        
        
    case MSG_JAVA_CHECK_FAIL:
    case MSG_JAVA_CHECK_OK:        
        HandleLocalCheckResult(s_install_data_info_ptr);
        break;

    case MSG_CLOSE_WINDOW:
	#ifdef JAVA_DM_SUPPORT
		DM_JAVAHandleScomoCallback(0);	
	#endif
        ReleaseInstallDataInfo();
        check_data_ptr = PNULL;

        //如果ota check 失败
        s_is_close_browser = FALSE;//reset flag
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
        MMIJAVA_OTADeleteFile();
#endif
        if(PNULL != OTA_JAD_FileName_ptr)
        {
            SCI_FREE(OTA_JAD_FileName_ptr);
        }
        
        if(PNULL != OTA_JAR_FileName_ptr)
        {
            SCI_FREE(OTA_JAR_FileName_ptr);
        }
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_JAVA);
#endif
        break;         
        
    default:    
        recode = MMI_RESULT_FALSE;            
        break;
        
    }   
    return recode;   
}


/**************************************************************************************************/
// Description:HandleLocalCheckResult
// Date:2007-06-08
// Author:huangjc
// Note:本地安装解析结果信息处理函数
//*************************************************************************************************/
LOCAL void HandleLocalCheckResult(MMIJAVA_INSTALL_DATA_INFO_T*  install_data_ptr)
{ 
   //SCI_TRACE_LOW:"HandleLocalCheckResult s_local_check_result.oldId = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4502_112_2_18_2_22_17_105,(uint8*)"d",s_local_check_result.oldId);
   
    if((ajms_ab_getSutieListCount(-1) >= MAXJAVALISTSIZE) && (s_local_check_result.oldId < 0))
    {
        if(PNULL != s_install_data_info_ptr && s_install_data_info_ptr->is_ota)
		{
		
			if(0 != strlen((char*)s_local_check_result.installNotifyUrl))
            {
				if(jwe_ota_notify(	s_local_check_result.otaCode, 
								(char*) s_local_check_result.installNotifyUrl,
								strlen((char*)s_local_check_result.installNotifyUrl)
								))
				{
                    s_ota_notify_is_success= FALSE;
                }
			}									
		}
        
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_JAVA_FILE_FULL, 
            TXT_NULL, 
            IMAGE_PUBWIN_WARNING, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);         
        
        MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );
	
        return;
    }
    //SCI_TRACE_LOW:"HandleLocalCheckResult otacode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4534_112_2_18_2_22_17_106,(uint8*)"d",s_local_check_result.otaCode);
    switch(s_local_check_result.otaCode )
    {
    case AJMS_ERR_OTA_OK:  
        install_data_ptr->old_id = s_local_check_result.oldId;
        MMK_CreateWin( ( uint32* )MMIJAVA_INSTALL_INFO_WIN_TAB, PNULL);
        break;
       
        
    case AJMS_ERR_OTA_NO_ENOUGH_SPACE:   
		//SCI_ASSERT(0);
        if(PNULL != s_install_data_info_ptr && s_install_data_info_ptr->is_ota)
		{
			if(0 != strlen((char*)s_local_check_result.installNotifyUrl))
            {
				if(jwe_ota_notify(	s_local_check_result.otaCode, 
								(char*) s_local_check_result.installNotifyUrl,
								strlen((char*)s_local_check_result.installNotifyUrl)
								))
				{
		            s_ota_notify_is_success= FALSE;
                }
			}									
		}

        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_NO_SPACE, 
            TXT_NULL, 
            IMAGE_PUBWIN_WARNING, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);         
        
        MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );
        break;
              
        
    default:                  
        if(PNULL != s_install_data_info_ptr && s_install_data_info_ptr->is_ota)
		{
			if(0 != strlen((char*)s_local_check_result.installNotifyUrl))
            {
				if(jwe_ota_notify(	s_local_check_result.otaCode, 
								(char*) s_local_check_result.installNotifyUrl,
								strlen((char*)s_local_check_result.installNotifyUrl)
								))

				{
		            s_ota_notify_is_success= FALSE;
                }
			}									
		}
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);  
        
        MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );
        break;
                   
    }        
}


PUBLIC BOOLEAN MMIJAVA_jblendia_start(JAVA_INSTALL_INFO_T *install_info_ptr  ,MMI_WIN_ID_T win_id)
{
     AJ_S32    ret = AJMS_ERR_NONE;
     BOOLEAN   run_result = TRUE;

	 //SCI_TRACE_LOW:"\"\"\"\"\" MMIJAVA_jblendia_start\"\"\"\"\""
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4608_112_2_18_2_22_17_107,(uint8*)"");
#ifdef MMI_JAVA_NW_V2
     g_active_suite_id = install_info_ptr->cursuitid;
#endif
	 MMIJAVA_Start_initializeResource();
	 
//	 GUILCD_SetBrushMode(GUI_MAIN_LCD_ID, LCD_BRUSH_MODE_ASYNC);

#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(),  ARM_CLK_LVL5);   
#endif
#ifndef JAVA_LOW_POWER_ENABLE
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);  
#endif
        UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
        is_platform_request = FALSE;
        

        if(install_info_ptr->is_flag == 1)
        {
            #ifdef CMCC_JAVA_V41_SUPPORT 
		    current_running_midlet_domain = AJMS_PCS_DOMAIN_UNSPECIFIED;
	        #endif
            if(!((install_info_ptr->jad_filename_ptr != PNULL) && (install_info_ptr->jar_filename_ptr != PNULL) && MMIJAVA_jwe_launchJad(install_info_ptr->jad_filename_ptr,install_info_ptr->jar_filename_ptr)))
            {
                //SCI_TRACE_LOW:"HandleJavaSubWinMsg launch error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4631_112_2_18_2_22_17_108,(uint8*)"");
                 MMIPUB_OpenAlertWinByTextId(PNULL, 
			                                 TXT_COMM_INSTALL_FAILED, 
			                                 TXT_NULL, 
			                                 IMAGE_PUBWIN_FAIL, 
			                                 PNULL, 
			                                 PNULL,
			                                 MMIPUB_SOFTKEY_ONE,
			                                 PNULL);             
                MMK_CloseWin( win_id ); 
                run_result = FALSE;
            }
        }
        else if(install_info_ptr->is_flag == 2)
        {
            #ifdef CMCC_JAVA_V41_SUPPORT 
		    current_running_midlet_domain = AJMS_PCS_DOMAIN_UNSPECIFIED;
	        #endif
            if(!((install_info_ptr->jar_filename_ptr != PNULL) && MMIJAVA_jwe_launchJar(install_info_ptr->jar_filename_ptr)))
            {
                //SCI_TRACE_LOW:"HandleJavaSubWinMsg launch error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4651_112_2_18_2_22_17_109,(uint8*)"");
                MMIPUB_OpenAlertWinByTextId(PNULL, 
			                                 TXT_COMM_INSTALL_FAILED, 
			                                 TXT_NULL, 
			                                 IMAGE_PUBWIN_FAIL, 
			                                 PNULL, 
			                                 PNULL,
			                                 MMIPUB_SOFTKEY_ONE,
			                                 PNULL);         
                MMK_CloseWin(win_id);
                run_result = FALSE;
            }
        }
        else 
        {
            #ifdef CMCC_JAVA_V41_SUPPORT 
            current_running_midlet_domain = ajms_mss_getDomainId(install_info_ptr->cursuitid);
            check_midlet_can_keepBgRunning(install_info_ptr->cursuitid);
            #endif
            ret =  MMIJAVA_jwe_run(install_info_ptr->cursuitid, install_info_ptr->curmidletid);//by suit
			//SCI_TRACE_LOW:"HandleJavaSubWinMsg MMIJAVA_jwe_run ret =%d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4671_112_2_18_2_22_17_110,(uint8*)"d",ret);
            if(AJMS_ERR_FILE_NOT_EXIST == ret)
            {

                MMIPUB_OpenAlertFailWin(TXT_JAVA_FILE_DAMAGED);
                MMK_CloseWin(win_id); 
                run_result = FALSE;
            }
            else if (AJMS_ERR_NONE != ret)
			{
				MMK_CloseWin(win_id);
                run_result = FALSE;
			}
       } 
        
      //SCI_TRACE_LOW:"HandleJavaSubWinMsg MMIJAVA_jwe_run leave"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4686_112_2_18_2_22_17_111,(uint8*)"");
      return    run_result;
}


PUBLIC BOOLEAN MMIJAVA_jblendia_tck_start(JAVA_INSTALL_INFO_T *install_info_ptr ,MMI_WIN_ID_T win_id)
{
     BOOLEAN                 run_result = TRUE;
	 int err;
	 char url[100];
	 KJavaDevFileHandle fp;
	 signed long read;

	 MMIJAVA_Start_initializeResource();
//	 GUILCD_SetBrushMode(GUI_MAIN_LCD_ID, LCD_BRUSH_MODE_ASYNC);
	 
#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(),  ARM_CLK_LVL5);   
#endif
#ifndef JAVA_LOW_POWER_ENABLE //for cr NEWMS00161837
    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#endif
    UILAYER_SetMainLayerType(DATA_TYPE_RGB565);

    is_platform_request = FALSE;

	//SCI_TRACE_LOW:"\"\"\"\"\" tck test start \"\"\"\"\""
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4710_112_2_18_2_22_17_112,(uint8*)"");
	JWE_LOG(HandleJavaSubWinMsg, ("########## tck test start ##########"));
	SCI_MEMSET(url, 0, sizeof(url));
	fp = kjava_dfs_open("E:\\tck.txt", &err);
    if(fp)
    {
		kjava_dfs_read(url, 0, 100, &read, fp);
		kjava_dfs_close(fp);
		//SCI_TRACE_LOW:"\"\"\"\"\"\"\"\"# tck test url: %s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4718_112_2_18_2_22_17_113,(uint8*)"s", url);
		JWE_LOG(HandleJavaSubWinMsg, ("################# tck test url: %s", url));
		if( strstr(url, "127.0.0.1") )
        {
			kjava_serial_init();
			ajms_cfg_setHttpImpl(AJSC_HTTP_PROFILE_SERIAL);
		}
		else
        {
			ajms_cfg_setHttpImpl(AJSC_HTTP_PROFILE_DEFAULT);
		}
		g_runningTckLevel = 1;
       		ajms_cfg_registerEimeHandler(MMIJAVA_jwe_eimeStartupImpl, MMIJAVA_jwe_eimeTerminateImpl);
		ajms_arc_startTck( url, (AJMS_VmEventHandler)MMIJAVA_TCK_vmEventHandler);
	}
    else
    {
        if(install_info_ptr->is_flag == 1)
        {
            if(!((install_info_ptr->jad_filename_ptr != PNULL) && (install_info_ptr->jar_filename_ptr != PNULL) && MMIJAVA_jwe_launchJad(install_info_ptr->jad_filename_ptr,install_info_ptr->jar_filename_ptr)))
            {
                //SCI_TRACE_LOW:"HandleJavaSubWinMsg launch error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4739_112_2_18_2_22_17_114,(uint8*)"");
                 MMIPUB_OpenAlertWinByTextId(PNULL, 
		                                     TXT_COMM_INSTALL_FAILED, 
		                                     TXT_NULL, 
		                                     IMAGE_PUBWIN_FAIL, 
		                                     PNULL, 
		                                     PNULL,
		                                     MMIPUB_SOFTKEY_ONE,
		                                     PNULL);             
                MMK_CloseWin( win_id );
                run_result = FALSE;
            }
        }
        else if(install_info_ptr->is_flag == 2)
        {
            if(!((install_info_ptr->jar_filename_ptr != PNULL) && MMIJAVA_jwe_launchJar(install_info_ptr->jar_filename_ptr)))
            {
                //SCI_TRACE_LOW:"HandleJavaSubWinMsg launch error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4756_112_2_18_2_22_17_115,(uint8*)"");
                MMIPUB_OpenAlertWinByTextId(PNULL, 
		                                     TXT_COMM_INSTALL_FAILED, 
		                                     TXT_NULL, 
		                                     IMAGE_PUBWIN_FAIL, 
		                                     PNULL, 
		                                     PNULL,
		                                     MMIPUB_SOFTKEY_ONE,
		                                     PNULL);         
                MMK_CloseWin(win_id);
                run_result = FALSE;
            }
        }
        else 
        {
           run_result =  MMIJAVA_jwe_run(install_info_ptr->cursuitid, install_info_ptr->curmidletid);
        } 
    
    }
    return run_result;
}

#ifdef VIRTUAL_KEY_BOARD
/*****************************************************************************/
//  Description : the callback of num botton in keyboard1
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void MMIJAVA_VirtualKeyboardTPToKJKey(uint32 ctr_id,int32 key_type)
{
    int32  key_code =-1;
    //SCI_TRACE_LOW:"MMIJAVA_VirtualKeyboardTPToKJKey:active_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4786_112_2_18_2_22_17_116,(uint8*)"d",ctr_id);

    switch(ctr_id)
    {
        case MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY:
        case MMIJAVA_KEYBOARD2_BOTTON_LFFT_SOFTKEY:
            key_code = AJSC_KEYCODE_SOFT1;
            break;
        case MMIJAVA_KEYBOARD1_BOTTON_RIGHT_SOFTKEY:
        case MMIJAVA_KEYBOARD2_BOTTON_RIGHT_SOFTKEY:
            key_code = AJSC_KEYCODE_SOFT2;
            break;
        case MMIJAVA_KEYBOARD1_BOTTON_UP_KEY:
            key_code = AJSC_KEYCODE_UP;
            break;
        case MMIJAVA_KEYBOARD1_BOTTON_LEFT_KEY:
            key_code = AJSC_KEYCODE_LEFT;
            break;
        case MMIJAVA_KEYBOARD1_BOTTON_DOWN_KEY:
            key_code = AJSC_KEYCODE_DOWN;
            break;
        case MMIJAVA_KEYBOARD1_BOTTON_RIGHT_KEY:
            key_code = AJSC_KEYCODE_RIGHT;
            break;
        case MMIJAVA_KEYBOARD1_BOTTON_OK_KEY:
        case MMIJAVA_KEYBOARD2_BOTTON_OK_KEY:
            key_code = AJSC_KEYCODE_SELECT;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_1:
            key_code = AJSC_KEYCODE_NUM1;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_2:
            key_code = AJSC_KEYCODE_NUM2;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_3:
            key_code = AJSC_KEYCODE_NUM3;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_4:
            key_code = AJSC_KEYCODE_NUM4;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_5:
            key_code = AJSC_KEYCODE_NUM5;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_6:
            key_code = AJSC_KEYCODE_NUM6;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_7:
            key_code = AJSC_KEYCODE_NUM7;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_8:
            key_code = AJSC_KEYCODE_NUM8;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_9:
            key_code = AJSC_KEYCODE_NUM9;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_0:
            key_code = AJSC_KEYCODE_NUM0;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_ASTERISK:
            key_code = AJSC_KEYCODE_ASTERISK;
            break;
        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_POUND:
            key_code = AJSC_KEYCODE_POUND;
            break;
         default:
            break;
    }

    if(key_type >= 0 && key_code >= 0)
    {
        //SCI_TRACE_LOW:"MMIJAVA_VirtualKeyboardTPToKJKey:key_type,key_code =%d,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4856_112_2_18_2_22_18_117,(uint8*)"dd",key_type,key_code);
        MMIJAVA_sendKeyEvent(key_type, key_code);
    }

}

/*****************************************************************************/
//  Description : the callback of num botton in keyboard1
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E NumberButtonCallBack(void)
{
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD2_FORM,TRUE,TRUE);
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD1_FORM,FALSE,TRUE);

    s_form1_display_flag = FALSE;
    s_form2_display_flag = TRUE;
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : the callback of return botton in keyboard2
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E ReturnButtonCallBack(void)
{
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD2_FORM,FALSE,TRUE);
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD1_FORM,TRUE,TRUE);
    s_form1_display_flag = TRUE;
    s_form2_display_flag = FALSE;

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  FUNCTION:       SetWCFormParam
// 	Description :   set the from parameter
//	Global resource dependence : 
//  Author:         murphy.xie
//	Note:   
/*****************************************************************************/
LOCAL void SetJavaKeyboardFormparam(void)
{
    GUI_BG_T                    bg_info = {0}; 
    uint16			            form_space = 1;
    uint16                      form_margin = 4;
    GUI_RECT_T                  rect= MMITHEME_GetFullScreenRect();
    int                         i = 0;
    uint16                      image_width = 0,image_height = 0;
    GUIFORM_CHILD_WIDTH_T       child_width = {0};

    //not slide
    GUIFORM_IsSlide(MMIJAVA_KEYBOARD1_FORM,FALSE);
    GUIFORM_IsSlide(MMIJAVA_KEYBOARD1_FORM,FALSE);
    
    //set rect
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD_BG,MMIJAVA_MIDLET_WIN_ID);
    rect.top = rect.bottom+1- image_height;
	GUIFORM_SetRect(MMIJAVA_KEYBOARD1_FORM,&rect);
	GUIFORM_SetRect(MMIJAVA_KEYBOARD2_FORM,&rect);
    //SCI_TRACE_LOW:"rect(%d,%d,%d,%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_4914_112_2_18_2_22_18_118,(uint8*)"dddd",rect.left,rect.top,rect.right,rect.bottom);
    
    //set bg
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_JAVA_KEYBOARD_BG;
    GUIFORM_SetBg(MMIJAVA_KEYBOARD1_FORM, &bg_info);
    GUIFORM_SetBg(MMIJAVA_KEYBOARD2_FORM, &bg_info);

    //set keyboard1 rect info
    //left+right softkey icon
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD1_LS_ON,MMIJAVA_MIDLET_WIN_ID);
    child_width.add_data = image_width;
    GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD1_LINE_ONE_FORM,MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY,&child_width);
    GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD1_LINE_ONE_FORM,MMIJAVA_KEYBOARD1_BOTTON_RIGHT_SOFTKEY,&child_width);


    //set keyboard2 rect info
    //left+right softkey icon
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD2_LS_OFF,MMIJAVA_MIDLET_WIN_ID);
    child_width.add_data = image_width;
    GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD2_LINE_ONE_FORM,MMIJAVA_KEYBOARD2_BOTTON_RIGHT_SOFTKEY,&child_width);

    //line1  num
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD2_NUM0_OFF,MMIJAVA_MIDLET_WIN_ID);
    child_width.add_data = image_width;
    for(i = MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_1; i <= MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_6; i++)
    {
        GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD2_LINE_ONE_FORM,i,&child_width);
    }


    //return icon
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD2_RETURN_OFF,MMIJAVA_MIDLET_WIN_ID);
    child_width.add_data = image_width;
    GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD2_LINE_TWO_FORM,MMIJAVA_KEYBOARD2_BOTTON_RETURN_KEY,&child_width);


    //line2 num
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD2_NUM0_OFF,MMIJAVA_MIDLET_WIN_ID);
    child_width.add_data = image_width;
    for(i = MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_7; i <= MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_POUND; i++)
    {
        GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD2_LINE_TWO_FORM,i,&child_width);
    }

    
    //set from margin + space
    GUIFORM_SetSpace(MMIJAVA_KEYBOARD1_FORM,&form_space,&form_space);
    #if defined (MAINLCD_SIZE_240X400)||(MAINLCD_SIZE_240X320)
    form_space = 3;
    #endif
    GUIFORM_SetSpace(MMIJAVA_KEYBOARD2_FORM,&form_space,&form_space);

    GUIFORM_SetMargin(MMIJAVA_KEYBOARD1_FORM,form_margin);
    GUIFORM_SetMargin(MMIJAVA_KEYBOARD2_FORM,form_margin);

    //set button
    for(i = MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY; i <= MMIJAVA_KEYBOARD2_BOTTON_OK_KEY; i++)
    {
        GUIBUTTON_PermitBorder(i, FALSE);
        GUIBUTTON_SetRunSheen(i,FALSE);
        if(i < MMIJAVA_KEYBOARD2_BOTTON_OK_KEY)
        {
            bg_info.img_id = IMAGE_JAVA_KEYBOARD1_LS_ON + (i - MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY) *2;
            GUIBUTTON_SetPressedFg(i, &bg_info);
        }
    }

    //ok botton
    bg_info.img_id = IMAGE_JAVA_KEYBOARD1_OK_ON;
    GUIBUTTON_SetPressedFg(MMIJAVA_KEYBOARD2_BOTTON_OK_KEY, &bg_info);


    //set callback
    GUIBUTTON_SetCallBackFunc(MMIJAVA_KEYBOARD1_BOTTON_NUM_KEY, NumberButtonCallBack);
    GUIBUTTON_SetCallBackFunc(MMIJAVA_KEYBOARD2_BOTTON_RETURN_KEY, ReturnButtonCallBack);

    // hide keyboard2 
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD2_FORM,s_form2_display_flag,FALSE);
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD1_FORM,s_form1_display_flag,FALSE);
    if(s_form2_display_flag)
    {
        MMK_SetAtvCtrl(MMIJAVA_MIDLET_WIN_ID, MMIJAVA_KEYBOARD2_BOTTON_LFFT_SOFTKEY);
    }
    else
    {
        MMK_SetAtvCtrl(MMIJAVA_MIDLET_WIN_ID, MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY);
    }
}
#endif


/**************************************************************************************************/
// Description:midlet window message handler
// Date:2007-06-08
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaSubWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    BOOLEAN                 run_result = TRUE;
    MMI_RESULT_E            result    = MMI_RESULT_TRUE;
    int32                   ajscKeyType = 0, ajscKeyCode = 0;
    // int32                   i = 0;
    // int32                   java_state = 0;
    JAVA_INSTALL_INFO_T *   install_info_ptr = (JAVA_INSTALL_INFO_T *)MMK_GetWinAddDataPtr(win_id);    
    JWE_LOG(HandleJavaSubWinMsg, ("msg_id = %x", msg_id));
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        
        g_jvm_hold_lcd = TRUE;
#ifndef JAVA_LOW_POWER_ENABLE //for cr NEWMS00161837
	    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#endif
        UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
#ifdef UI_P3D_SUPPORT
        MMI_Disable3DEffect(MMI_3D_EFFECT_JAVA);
#endif
		if(MMIAPIJAVA_IsJavaRuning())
		{
		    //SCI_TRACE_LOW:"JVM has been runing...."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5040_112_2_18_2_22_18_119,(uint8*)"");
		}
		else
		{
		    char* java_mem_ptr = PNULL;
            BLKMEM_GarbageClean();
            UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
            
		    //尝试提前分配内存
            java_mem_ptr = MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_JAVA);
		    //SCI_TRACE_LOW:"JVM start step1,try to allocate memory ahead"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5046_112_2_18_2_22_18_120,(uint8*)"");
            
			//如果失败就退出
            if(PNULL == java_mem_ptr)
            {
		        //SCI_TRACE_LOW:"JVM start step1,allocate failed, exit"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5051_112_2_18_2_22_18_121,(uint8*)"");
                MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
                UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
                MMK_CloseWin(win_id);
                break;
            }
            
            UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
            
            //RESET FORM DISPLAY FLAG
#ifdef VIRTUAL_KEY_BOARD
            s_form1_display_flag = FALSE;
            s_form2_display_flag = FALSE;
#endif	
            
            if(g_jvm_is_auto_tck_mode) 
                run_result = MMIJAVA_jblendia_tck_start(install_info_ptr,win_id);
            else 
                run_result = MMIJAVA_jblendia_start(install_info_ptr,win_id);
            
            if(run_result)
            {
                g_jvm_is_running_by_mmi = TRUE;
            }
            else
            {
                MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_JAVA); 
            }
        }
        
        #ifdef VIRTUAL_KEY_BOARD
        SetJavaKeyboardFormparam();
        #endif
		break;
	
    case MSG_KEYDOWN_RED:
    case MSG_KEYLONG_RED:
    case MSG_KEYPRESSUP_RED:
    case MSG_KEYREPEAT_RED:
	 //SCI_TRACE_LOW:"native key(red) is 0x%x"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5086_112_2_18_2_22_18_122,(uint8*)"d", msg_id);

        if(s_lastkeypressed != -1)
        {
            // deal with key event
            MMIJAVA_sendKeyEvent(AJSC_EVENT_KEY_RELEASED, s_lastkeypressed);
            s_lastkeypressed = -1;
        }
#ifndef REMOVE_PAUSE_RESUME_DIALOG	
        s_b_enable_pasue = FALSE;
        g_jvm_hold_lcd = FALSE;
        SCI_SLEEP(150);//don't remove it,用作进程切换!!!
		MMK_CreateWin((uint32 *)MMIJAVA_PAUSE_DIALOG_WIN_TAB, PNULL);//PNULL will indicate bge info
#else
	    MMIJAVA_stopVM_external(); 
#endif		
		break;
    case MSG_KEYUP_UP:
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_LEFT:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_WEB:
    case MSG_KEYUP_OK:
    case MSG_KEYUP_CANCEL:
        
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
    case MSG_KEYUP_GREEN:
#else        
    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
#endif
    case MSG_KEYUP_1:
    case MSG_KEYUP_2:
    case MSG_KEYUP_3:
    case MSG_KEYUP_4:
    case MSG_KEYUP_5:
    case MSG_KEYUP_6:
    case MSG_KEYUP_7:
    case MSG_KEYUP_8:
    case MSG_KEYUP_9:
    case MSG_KEYUP_STAR:
    case MSG_KEYUP_0:
    case MSG_KEYUP_HASH:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_HASH:
    
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
    case MSG_KEYREPEAT_WEB:
    case MSG_KEYREPEAT_OK:
    case MSG_KEYREPEAT_CANCEL:
    case MSG_KEYREPEAT_GREEN:
    case MSG_KEYREPEAT_1:
    case MSG_KEYREPEAT_2:
    case MSG_KEYREPEAT_3:
    case MSG_KEYREPEAT_4:
    case MSG_KEYREPEAT_5:
    case MSG_KEYREPEAT_6:
    case MSG_KEYREPEAT_7:
    case MSG_KEYREPEAT_8:
    case MSG_KEYREPEAT_9:
    case MSG_KEYREPEAT_0:
    case MSG_KEYREPEAT_HASH:

    case MSG_KEYLONG_UP:
    case MSG_KEYLONG_DOWN:
    case MSG_KEYLONG_LEFT:
    case MSG_KEYLONG_RIGHT:
    case MSG_KEYLONG_WEB:
    case MSG_KEYLONG_OK:
    case MSG_KEYLONG_CANCEL:
    case MSG_KEYLONG_GREEN:
    case MSG_KEYLONG_1:
    case MSG_KEYLONG_2:
    case MSG_KEYLONG_3:
    case MSG_KEYLONG_4:
    case MSG_KEYLONG_5:
    case MSG_KEYLONG_6:
    case MSG_KEYLONG_7:
    case MSG_KEYLONG_8:
    case MSG_KEYLONG_9:
    case MSG_KEYLONG_0:
    case MSG_KEYLONG_HASH:

    case MSG_KEYPRESSUP_UP:
    case MSG_KEYPRESSUP_DOWN:
    case MSG_KEYPRESSUP_LEFT:
    case MSG_KEYPRESSUP_RIGHT:
    case MSG_KEYPRESSUP_WEB:
    case MSG_KEYPRESSUP_OK:
    case MSG_KEYPRESSUP_CANCEL:
    case MSG_KEYPRESSUP_GREEN:
    case MSG_KEYPRESSUP_1:
    case MSG_KEYPRESSUP_2:
    case MSG_KEYPRESSUP_3:
    case MSG_KEYPRESSUP_4:
    case MSG_KEYPRESSUP_5:
    case MSG_KEYPRESSUP_6:
    case MSG_KEYPRESSUP_7:
    case MSG_KEYPRESSUP_8:
    case MSG_KEYPRESSUP_9:
    case MSG_KEYPRESSUP_0:
    case MSG_KEYPRESSUP_HASH:
        //SCI_TRACE_LOW:"native key(no red) is 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5211_112_2_18_2_22_18_123,(uint8*)"d", msg_id);
        if (msg_id > KEY_REPEATED)
        {
            ajscKeyType = AJSC_EVENT_KEY_REPEATED;
            ajscKeyCode = MMIJAVA_NativeKeyToKJKey(msg_id - KEY_REPEATED);
        }
        else if(msg_id > KEY_LONG_RELEASED)
        {
            ajscKeyType = AJSC_EVENT_KEY_RELEASED;
            ajscKeyCode = MMIJAVA_NativeKeyToKJKey(msg_id - KEY_LONG_RELEASED);
        }
        else if(msg_id > KEY_LONG_PRESSED)
        {
            ajscKeyType = AJSC_EVENT_KEY_PRESSED;
            ajscKeyCode = MMIJAVA_NativeKeyToKJKey(msg_id - KEY_LONG_PRESSED);
        }
        else if(msg_id > KEY_RELEASED)
        {
            ajscKeyType = AJSC_EVENT_KEY_RELEASED;
            ajscKeyCode = MMIJAVA_NativeKeyToKJKey(msg_id - KEY_RELEASED);
            s_lastkeypressed = -1;
        }
        else if(msg_id > KEY_PRESSED)
        {
            ajscKeyType = AJSC_EVENT_KEY_PRESSED;
            ajscKeyCode = MMIJAVA_NativeKeyToKJKey(msg_id - KEY_PRESSED);
            s_lastkeypressed = ajscKeyCode;
        }
        
        // map native key code to ajsc key code
        if(ajscKeyCode != -1)
        {
            // deal with key event
            MMIJAVA_sendKeyEvent(ajscKeyType, ajscKeyCode);
        }
        else
        {
            //SCI_TRACE_LOW:"HandleJavaSubWinMsg this is no acordding key code"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5248_112_2_18_2_22_18_124,(uint8*)"");
        }
        break;

     #ifdef AUDIO_SIDEKEY_SUPPORT
     case MSG_KEYDOWN_UPSIDE:
        MMIJAVA_AdjustVolume(+1);
        break;
     case MSG_KEYDOWN_DOWNSIDE:
        MMIJAVA_AdjustVolume(-1);
        break;
     #endif     
     case MSG_CLOSE_WINDOW:
        #ifdef REMOVE_PAUSE_RESUME_DIALOG
        MMIAPIJAVA_ExitJAVA();
        #endif
        g_jvm_hold_lcd = FALSE;       
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_JAVA);
#endif   

#ifndef JAVA_LOW_POWER_ENABLE //for cr NEWMS00161837
	    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#endif
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
        //SCI_TRACE_LOW:"HandleJavaSubWinMsg  -- MSG_CLOSE_WINDOW!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5270_112_2_18_2_22_18_125,(uint8*)"");
        break; 

    case MSG_TIMER: 
	    /* BM_MonitorAnyAddr(0x043c08a4, 0xffffffff, 0, wav_BmCallback);*/
#ifdef KVM_ENABLE_GAMECANVAS_OPT
    	 if (*(uint8*)param == m_Gamecanvas_timer)
    	 {
            JVM_UpdateGameCanvasScreenTimer(param);
    	 }
    	 else
#endif	 	
    	 {
            jblendia_gfx_timer(param);
    	 }
        break;        

#ifdef FLIP_PHONE_SUPPORT             
    case MSG_KEYDOWN_FLIP:
#endif    
    case MSG_LOSE_FOCUS:
        //SCI_TRACE_LOW:"HandleJavaSubWinMsg  -- MSG_LOSE_FOCUS!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5291_112_2_18_2_22_18_126,(uint8*)"");
        g_jvm_hold_lcd = FALSE;
    	 if(is_platform_request)//platform request 特殊处理
         {
    	 	 JWE_LOG(HandleJavaSubWinMsg,("is_platform_request=%d",is_platform_request)); 
             #ifdef CMCC_JAVA_V41_SUPPORT
    		 if(!keep_background_running){
    		    JWE_LOG(HandleJavaSubWinMsg,("keep_background_running=%d current_running_midlet_domain=%d jad_keep_background_running=%d",keep_background_running,current_running_midlet_domain,jad_keep_background_running)); 
    		    MMIJAVA_pauseVM_external(MMIJAVA_LONG_EVENT_STATE);
    		 }
    		 else{
    		    JWE_LOG(HandleJavaSubWinMsg,("keep_background_running=%d current_running_midlet_domain=%d jad_keep_background_running=%d  is_platform_request=%d",keep_background_running,current_running_midlet_domain,jad_keep_background_running,is_platform_request)); 
    		    MMIJAVA_pauseVM_external(MMIJAVA_SHORT_EVENT_STATE);
    		 }
    		 #else
    		 MMIJAVA_pauseVM_external(MMIJAVA_LONG_EVENT_STATE);
             #endif
    		 is_platform_request = FALSE;
    	 }
    	 else
         {//其他情况，如电话、短消息、手动按RED键等
    	 	 JWE_LOG(HandleJavaSubWinMsg,("is_platform_request=%d",is_platform_request)); 
    		 if(s_b_enable_pasue){
    		 	JWE_LOG(HandleJavaSubWinMsg,("s_b_enable_pasue=TRUE")); 
    			MMIJAVA_pauseVM_external(MMIJAVA_LONG_EVENT_STATE);
    		 }
    		 else{
    			JWE_LOG(HandleJavaSubWinMsg,("s_b_enable_pasue=FALSE")); 
    			MMIJAVA_pauseVM_external(MMIJAVA_SHORT_EVENT_STATE);
    		 }
    	}
         
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_JAVA);
#endif 
#ifndef JAVA_LOW_POWER_ENABLE //for cr NEWMS00161837
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#endif
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYUP_FLIP:
#endif    
    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"HandleJavaSubWinMsg  -- MSG_GET_FOCUS!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5334_112_2_18_2_22_19_127,(uint8*)"");
        if(s_b_enable_pasue)
        {
            MMIJAVA_resumeVM_external();
        }
        
#ifdef UI_P3D_SUPPORT
        MMI_Disable3DEffect(MMI_3D_EFFECT_JAVA);
#endif
#ifndef JAVA_LOW_POWER_ENABLE //for cr NEWMS00161837
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#endif
        UILAYER_SetMainLayerType(DATA_TYPE_RGB565);

        break;

    case MSG_FULL_PAINT:
        MMIJAVA_gfx_update();
        g_jvm_hold_lcd = TRUE;
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T point = {0};
            KJavaGfxInfo kjavaGfxInfo = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);      
            
            kjava_gfx_getInfo(&kjavaGfxInfo);
            if(point.y <= (kjavaGfxInfo.softKeyRegion.y + kjavaGfxInfo.softKeyRegion.height))
            {
                if(kjavaGfxInfo.numSoftkeys && point.y >= kjavaGfxInfo.softKeyRegion.y) 
                {  
                    if( point.x <= kjavaGfxInfo.softKeyRegion.width/2 )
                    {
                        MMIJAVA_sendKeyEvent(AJSC_EVENT_KEY_RELEASED, AJSC_KEYCODE_SOFT1);
                    }
                    else 
                    {
                        MMIJAVA_sendKeyEvent(AJSC_EVENT_KEY_RELEASED, AJSC_KEYCODE_SOFT2);
                    }
                }
                else
                {
                    AmNotifyPointerEvent( AJSC_EVENT_POINTER_RELEASED, 0, point.x, point.y );
                    AmNotifyPointerEvent( AJSC_EVENT_POINTER_CLICKED, 0, point.x, point.y );

                }
            }
            #ifdef VIRTUAL_KEY_BOARD
            else
            {
                MMIJAVA_VirtualKeyboardTPToKJKey(MMK_GetActiveCtrlId(MMIJAVA_MIDLET_WIN_ID),AJSC_EVENT_KEY_RELEASED);
            }
            #endif
       
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            KJavaGfxInfo kjavaGfxInfo = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
           
            kjava_gfx_getInfo(&kjavaGfxInfo);
            if(point.y <= (kjavaGfxInfo.softKeyRegion.y + kjavaGfxInfo.softKeyRegion.height))
            {
                if(kjavaGfxInfo.numSoftkeys && point.y >= kjavaGfxInfo.softKeyRegion.y) 
                {  
                    if( point.x <= kjavaGfxInfo.softKeyRegion.width/2 )
                    {
                        MMIJAVA_sendKeyEvent(AJSC_EVENT_KEY_PRESSED, AJSC_KEYCODE_SOFT1);
                    }
                    else 
                    {
                        MMIJAVA_sendKeyEvent(AJSC_EVENT_KEY_PRESSED, AJSC_KEYCODE_SOFT2);
                    }
                }
                else
                {
                    AmNotifyPointerEvent( AJSC_EVENT_POINTER_PRESSED, 0, point.x, point.y );
                }
            }
            #ifdef VIRTUAL_KEY_BOARD
            else
            {
                MMIJAVA_VirtualKeyboardTPToKJKey(MMK_GetActiveCtrlId(MMIJAVA_MIDLET_WIN_ID),AJSC_EVENT_KEY_PRESSED);
            }
            #endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#if 0//def VIRTUAL_KEY_BOARD
    case MSG_CTL_BUTTON_DOWN:
        if (PNULL != param)
        {
            uint32 ctr_id = ((MMI_NOTIFY_T*)param)->src_id;
            MMIJAVA_VirtualKeyboardTPToKJKey(ctr_id,AJSC_EVENT_KEY_PRESSED);

        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 ctr_id = ((MMI_NOTIFY_T*)param)->src_id;
            MMIJAVA_VirtualKeyboardTPToKJKey(ctr_id,AJSC_EVENT_KEY_RELEASED);

        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            AmNotifyPointerEvent( AJSC_EVENT_POINTER_MOVED, 0, point.x, point.y );
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYUP_POWER:
    case MSG_KEYDOWN_POWER:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}


#ifdef SELECT_INSTALL_DISK_SUPPORT
/**************************************************************************************************/
// Description:Set sys Disk C to on
// Date:2010-07-13
// Author:hanker
// Note:
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_SetSysDiskHiddenOn(void)
{
	SysDiskHiddenOn=TRUE;
}
/**************************************************************************************************/
// Description:Set sys Disk C to on
// Date:2010-07-13
// Author:hanker
// Note:
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_SetSysDiskHiddenOff(void)
{
	SysDiskHiddenOn= FALSE;
}

// Description:List all of the midlet suite in MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID ListBox
// Date:2007-06-08
// Author:hanker
// Note:
//*************************************************************************************************/
LOCAL BOOLEAN CreateInstallDiskListBox(void)
{
    GUILIST_ITEM_T          listbox_item = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    //const wchar*            dev_d_name = MMIFILE_DEVICE_UDISK;
    //uint16                  dev_d_name_len = MMIFILE_DEVICE_UDISK_LEN;
    //const wchar*            dev_e_name = MMIFILE_DEVICE_SDCARD;
    //uint16                  dev_e_name_len = MMIFILE_DEVICE_SDCARD_LEN;	
    uint16                  disk_num = 0;
	MMIFILE_DEVICE_E        dev    = MMI_DEVICE_UDISK;
	
    //SCI_TRACE_LOW:"CreateInstallDiskListBox "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5499_112_2_18_2_22_19_128,(uint8*)"");
	GUILIST_RemoveAllItems(MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID);

    if (SysDiskHiddenOn)
    {
        //SCI_TRACE_LOW:"CreateInstallDiskListBox C"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5504_112_2_18_2_22_19_129,(uint8*)"");
        SCI_MEMSET(&listbox_item, 0, sizeof(listbox_item));  
#ifdef MMI_PDA_SUPPORT
        listbox_item.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
#else
        listbox_item.item_style = GUIITEM_STYLE_ONE_LINE_TEXT; 
#endif
        SCI_MEMSET(&item_data, 0, sizeof(item_data));
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_JAVA_INSTALL_SELECT_DISK_C;      
#ifndef MMI_PDA_SUPPORT
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
#endif
        listbox_item.item_data_ptr = &item_data;
        // Add the item to MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID listbox. 
        GUILIST_AppendItem (MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, &listbox_item);    
        s_device_arr[disk_num]= MMI_DEVICE_SYSTEM;
        disk_num++;
    }  
    
    do
    {
        if(!MMIAPIFMM_GetDeviceTypeStatus(dev))
        {
            continue;
        }

        s_device_arr[disk_num] = dev;//MMIFILE_GetFileFilterByDevice(MMIFILE_GetFileDeviceType(dev));
        disk_num++;
        
        SCI_MEMSET(&listbox_item, 0, sizeof(listbox_item)); 
#ifdef MMI_PDA_SUPPORT
        listbox_item.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
#else        
        listbox_item.item_style = GUIITEM_STYLE_ONE_LINE_TEXT; 
#endif
        SCI_MEMSET(&item_data, 0, sizeof(item_data));
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = MMIAPIFMM_GetDeviceName(dev);
#ifndef MMI_PDA_SUPPORT
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
#endif
        listbox_item.item_data_ptr = &item_data;
        GUILIST_AppendItem (MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, &listbox_item);    
    }while(++dev < MMI_DEVICE_NUM);

    // Update listbox
    GUILIST_SetCurItemIndex(MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, 0);
    GUILIST_SetSelectedItem(MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID, 0, TRUE);       
    return TRUE;
}
#endif
/**************************************************************************************************/
// Description:List all of the midlet suite in MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID ListBox
// Date:2007-06-08
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL BOOLEAN CreateSuitListBox(int32 item_index)
{
    int32                   j               = 0;
    GUILIST_ITEM_T          listbox_item    = {0};
    GUILIST_ITEM_DATA_T     item_data       = {0};
    int32                   iNumOfSuite     = 0;  
    
#ifdef PREINSTALL_JAVA_SUPPORT
    AJMS_Suite              suiteList_tmp = {0};
#endif
    // int32                   preinstall_total_num               = 0;

    // initialize the ListBox, delet all items here
    GUILIST_RemoveAllItems(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID);

    // get the number of all java applications
#ifdef HIDE_PREINSTALL_IN_ITEMLIST_HUAWEI
    iNumOfSuite = ajms_ab_getSutieListCount(AJMS_DEFAULT_CATEGORY_ID_UNFILED);   
#else
    iNumOfSuite = ajms_ab_getSutieListCount( -1 );   
#endif
    //SCI_TRACE_LOW:"ACJ->ajms_ab_getSutieListCount return applet = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5586_112_2_18_2_22_19_130,(uint8*)"d",iNumOfSuite);
    
    if ( iNumOfSuite <= 0 || iNumOfSuite > (MAXJAVALISTSIZE + 1))
    {
        #if 0
        MMIPUB_OpenAlertWinByTextId(PNULL, 
			                         TXT_COMMON_COMMON_EMPTY_LIST, 
			                         TXT_NULL, 
			                         IMAGE_PUBWIN_WARNING, 
			                         PNULL, 
			                         PNULL,
			                         MMIPUB_SOFTKEY_ONE,
			                         PNULL); 
        MMK_CloseWin(MMIJAVA_AMS_OPTION_WIN_ID);
        MMK_CloseWin(MMIJAVA_AMS_WIN_ID); 
        #endif
        //SCI_TRACE_LOW:"no midlet or more size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5602_112_2_18_2_22_19_131,(uint8*)"");
        return FALSE;
    }
    else
    {
        SCI_MEMSET(s_suiteList, 0, (MAXJAVALISTSIZE + 1)*sizeof(AJMS_Suite));
        // get the information of all java applications
#ifdef HIDE_PREINSTALL_IN_ITEMLIST_HUAWEI
        if ( ajms_ab_getSuiteList( AJMS_DEFAULT_CATEGORY_ID_UNFILED , s_suiteList, 0 ) < 0 )	
#else
        if ( ajms_ab_getSuiteList( -1 , s_suiteList, 0 ) < 0 )
#endif
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, 
			                         TXT_ERROR, 
			                         TXT_NULL, 
			                         IMAGE_PUBWIN_WARNING, 
			                         PNULL, 
			                         PNULL,
			                         MMIPUB_SOFTKEY_ONE,
			                         PNULL);  
            MMK_CloseWin(MMIJAVA_AMS_OPTION_WIN_ID);
            MMK_CloseWin(MMIJAVA_AMS_WIN_ID);        
            //SCI_TRACE_LOW:"--error: could not get the installed midlet list."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5625_112_2_18_2_22_19_132,(uint8*)"" );
            return FALSE;
        }
#ifdef PREINSTALL_JAVA_SUPPORT //将预安装应用调整到列表的顶端
        //SCI_TRACE_LOW:"CreateSuitListBox s_preinstall_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5629_112_2_18_2_22_19_133,(uint8*)"d",s_preinstall_id);
        if (0 != s_preinstall_id)
        {
            SCI_MEMCPY(&suiteList_tmp,&s_suiteList[0], sizeof(AJMS_Suite));
            for ( j = 0; j < iNumOfSuite && j < MAXJAVALISTSIZE; j ++ )
            {
                if (s_suiteList[j].id == s_preinstall_id)
                {
                    //SCI_TRACE_LOW:"CreateSuitListBox j = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5637_112_2_18_2_22_19_134,(uint8*)"d",j);
                    break;
                }
            }
            SCI_MEMCPY(&s_suiteList[0],&s_suiteList[j],sizeof(AJMS_Suite));
            SCI_MEMCPY(&s_suiteList[j],&suiteList_tmp,sizeof(AJMS_Suite));
            s_suiteList[0].categoryId = AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED;
        }        
      
#endif

        // the limit of amoi listbox item number is 20, so ignore the overfull applications
        for ( j = 0; j < iNumOfSuite && j < MAXJAVALISTSIZE; j ++ )
        {           

            SCI_MEMSET(&listbox_item, 0, sizeof(listbox_item));           
               
            //{ chenjin, 08.08.15
            listbox_item.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT; 
            //图标+ 文本文字
            SCI_MEMSET(&item_data, 0, sizeof(item_data));

              
               listbox_item.item_data_ptr = &item_data;
               // Add the item to MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID listbox. 
               GUILIST_AppendItem (MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, &listbox_item); 
            
        }
        // Update listbox
        GUILIST_SetCurItemIndex(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, item_index);
        GUILIST_SetSelectedItem(MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID, item_index, TRUE);
		  
        return TRUE;
    }
   
    
}


LOCAL BOOLEAN AppendOneListItemWithSuiteName(
                                       uint16           item_index,     //in:item索引
                                       MMI_CTRL_ID_T    ctrl_id         //in:control id
                                       )
{
    GUILIST_ITEM_DATA_T     item_data = {0};     /*lint !e64*/
    wchar                   suite_name_wstr[GUILIST_STRING_MAX_NUM +1]   = {0};     
    uint16                  suite_name_len = 0;
    BOOLEAN                 result = FALSE;   
    
    //set softkey 
     item_data.softkey_id[0] = STXT_OPTION;
     item_data.softkey_id[1] = TXT_NULL;
     item_data.softkey_id[2] = STXT_RETURN;       
            
     if(0 < GetMidletSuiteIconSize(s_suiteList[item_index].id, -1))
     {
         item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
     }
     else
     {
         // 先用默认的图片
         item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
         item_data.item_content[0].item_data.image_id = JAVA_ICON;
     }            
    
     item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
     suite_name_len = strlen((char*)s_suiteList[item_index].suiteName);
     item_data.item_content[1].item_data.text_buffer.wstr_len = GUI_UTF8ToWstr(suite_name_wstr,
                                                        GUILIST_STRING_MAX_NUM,
                                                        (const uint8 *)s_suiteList[item_index].suiteName,
                                                        (uint16)MIN(GUILIST_STRING_MAX_NUM,suite_name_len)
                                                        );            
     item_data.item_content[1].item_data.text_buffer.wstr_ptr = suite_name_wstr;            
     result = GUILIST_SetItemData(ctrl_id,&item_data,item_index); 
     return (result);
}

LOCAL int GetMidletSuiteIconSize(AJ_S32 suiteId, AJ_S32 midletId)
{
      int retval;
      if(midletId == -1 )
      {
        retval = ajms_ab_getIcon( suiteId,
                    AJMS_SUITE_ICON_ID,
                    AJ_NULL, // set the icon_data
                    0 );
          switch (retval)
          {
            case AJMS_ERR_ID_NOT_EXIST:
            case AJMS_ERR_IO_ERROR:
            case AJMS_ERR_NONE:
                retval = ajms_ab_getIcon( suiteId, 0, AJ_NULL, 0 );
                break;
            default:
                break;
          }

       }   /* end of switch (iconBufLen) */
      else
      {
          retval = ajms_ab_getIcon( suiteId, midletId, AJ_NULL, 0 );
      }
      return retval;
}

LOCAL BOOLEAN DrawListItemWithIconData(
                                      uint16           item_index,     //in:item索引
                                      uint16           content_index,  //in:item内容索引,text or anim etc.
                                      MMI_CTRL_ID_T    ctrl_id         //in:control id
                                      )
{
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    GUIANIM_DATA_INFO_T     anim_data    = {0};      /*lint !e64*/
    uint8 *                 data_info_ptr = PNULL;   
    int                     suite_icon_size=  0;
    int                     ret_value     =  0;    
    BOOLEAN                 result      = TRUE;
    
    suite_icon_size = GetMidletSuiteIconSize(s_suiteList[item_index].id, -1);
    //SCI_TRACE_LOW:"DrawListItemWithIconData item_index:%d, content_index: %d, icon size:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5756_112_2_18_2_22_19_135,(uint8*)"ddd", item_index, content_index,suite_icon_size);
    if(0 < suite_icon_size)
    {
        data_info_ptr = SCI_ALLOCA(suite_icon_size*sizeof(AJ_U8));/*lint !e737*/
        if(PNULL == data_info_ptr)
        {
            //SCI_TRACE_LOW:"DrawListItemWithIconData data_info_ptr  failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5762_112_2_18_2_22_19_136,(uint8*)"");
            return FALSE;
        }

        item_content.item_data_type = GUIITEM_DATA_ANIM_DATA; 
        item_content.item_data.anim_data_ptr = &anim_data;
        
        ret_value = ajms_ab_getIcon(s_suiteList[item_index].id, AJMS_SUITE_ICON_ID, (AJ_U8 *)data_info_ptr, suite_icon_size);
        if (ret_value <= 0)
        {
            //SCI_TRACE_LOW:"DrawListItemWithIconData get the first midlet icon"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5772_112_2_18_2_22_19_137,(uint8*)"");
            ret_value = ajms_ab_getIcon(s_suiteList[item_index].id, 0, (AJ_U8 *)data_info_ptr, suite_icon_size);
        }
        
        if(ret_value > 0)
        {
            item_content.item_data.anim_data_ptr->is_save_data = TRUE;
            item_content.item_data.anim_data_ptr->data_ptr = data_info_ptr;
            item_content.item_data.anim_data_ptr->data_size = suite_icon_size;
            result = GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);                 
        }
    }
   
    if(PNULL != data_info_ptr)
    {
        SCI_FREE(data_info_ptr);
        data_info_ptr = PNULL;
    }  
     
    return (result);
}

LOCAL void CreatePauseDialogListBox()
{
	// GUILIST_ITEM_T          listbox_item	= {0};
	// GUILIST_ITEM_DATA_T     item_data		= {0};
	// int32                   iNumOfSuite	= 0;  
	// wchar  					wstr_ptr_tmp[GUILIST_STRING_MAX_NUM +1]   = {0};
	
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
	//**********最小化
	MMIAPISET_AppendListItemByTextId(TXT_MINIMIZE, TXT_COMMON_OK, MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif

	//**********暂停
	MMIAPISET_AppendListItemByTextId(TXT_PAUSE, TXT_COMMON_OK, MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
	//**********退出
	MMIAPISET_AppendListItemByTextId(STXT_EXIT, TXT_COMMON_OK, MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
	// Update listbox
	GUILIST_SetCurItemIndex(MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID, 0);
	GUILIST_SetSelectedItem(MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID, 0, TRUE);       
}

LOCAL void CreateResumeDialogListBox()
{

    // GUILIST_ITEM_T          listbox_item		= {0};
    // GUILIST_ITEM_DATA_T     item_data			= {0};
    // int32                   iNumOfSuite		= 0;  
    // wchar  					wstr_ptr_tmp[GUILIST_STRING_MAX_NUM +1]   = {0};
	
	//**********继续
	MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_CONTINUE, STXT_OK, MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);

	//**********退出
	MMIAPISET_AppendListItemByTextId(STXT_EXIT, TXT_COMMON_OK, MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);

	// Update listbox
	GUILIST_SetCurItemIndex(MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID, 0);
	GUILIST_SetSelectedItem(MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID, 0, TRUE); 
}


/**************************************************************************************************/
// Description:List all of the midlets of a suite in MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID ListBox
// Date:2007-06-08
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL void CreateMidletListBox(int32 suitid)
{
    int32                   k = 0;    
    GUILIST_ITEM_T          listbox_item = {0};
    GUILIST_ITEM_DATA_T     item_data = { 0 };
    int32                   iNumOfMidlet = 0;    // total number of suite and midlets
    JAVA_INSTALL_INFO_T*    install_info_ptr = MMIJAVA_GetIntallInfo();
    AJMS_Midlet*            midletList_ptr = PNULL;
	wchar                   wstr_ptr_tmp[GUILIST_STRING_MAX_NUM +1]   = {0};
	
    //SCI_TRACE_LOW:"CreateMidletListBox suit id is %d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5850_112_2_18_2_22_20_138,(uint8*)"d", suitid);
    
    // Gets the number of midlets of a selected midlet suite.
    iNumOfMidlet = ajms_ab_getMidletListCount(suitid);        
    
    //SCI_TRACE_LOW:"-- number of midlet(s) is %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5855_112_2_18_2_22_20_139,(uint8*)"d", iNumOfMidlet );
    
    if ( iNumOfMidlet <= 0 )
    {   
        MMIPUB_OpenAlertWinByTextId(PNULL, 
			                         TXT_COMMON_COMMON_EMPTY_LIST, 
			                         TXT_NULL, 
			                         IMAGE_PUBWIN_WARNING, 
			                         PNULL, 
			                         PNULL,
			                         MMIPUB_SOFTKEY_ONE,
			                         PNULL);  
        MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);
        return;
    }

    midletList_ptr = SCI_ALLOC_APP(iNumOfMidlet * sizeof(AJMS_Midlet));/*lint !e737*/
    SCI_MEMSET(midletList_ptr, 0, (iNumOfMidlet * sizeof(AJMS_Midlet)));/*lint !e737*/

    // gets all MIDlet infomation of a selected midlet suite.
    if ( ajms_ab_getMidletList( suitid, midletList_ptr ) < 0 )
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
			                         TXT_ERROR, 
			                         TXT_NULL, 
			                         IMAGE_PUBWIN_WARNING, 
			                         PNULL, 
			                         PNULL,
			                         MMIPUB_SOFTKEY_ONE,
			                         PNULL);  
        MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);
        SCI_FREE(midletList_ptr);
        //SCI_TRACE_LOW:"--error: failed to get midlet list."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5887_112_2_18_2_22_20_140,(uint8*)"" );
        return;
    }
    
    // start the midlet directy if the selected midlet suite has only one midlet
    if(iNumOfMidlet == 1)
    {        
        SCI_MEMSET(install_info_ptr,0,sizeof(JAVA_INSTALL_INFO_T));
        install_info_ptr->curmidletid = 0;
        install_info_ptr->cursuitid = suitid;
        install_info_ptr->is_flag = 0;
        SCI_FREE(midletList_ptr);
		MMIJAVA_OpenMidletWindow(install_info_ptr);     
        MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);
        return;
    }
    
    // If the selected midlet suite has more than one midlets, 
    // We will delete all of the current item(midlet suite) of the ListBox
	// GUILIST_RemoveAllItems(MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID);
    // List all of the midlets for the selected midlet suite
    for ( k = 0; k < iNumOfMidlet; k ++ )
    {     
       
        //{ chenjin, 08.08.15
        SCI_MEMSET(&listbox_item, 0, sizeof(listbox_item));
        //SCI_MEMSET(wstr_ptr_tmp, 0, GUILIST_STRING_MAX_NUM);
        listbox_item.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT; 
        //图标+ 文本文字
        SCI_MEMSET(&item_data, 0, sizeof(item_data));
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
  

        item_data.item_content[1].item_data.text_buffer.wstr_len = GUI_UTF8ToWstr(
            wstr_ptr_tmp,
            GUILIST_STRING_MAX_NUM,
            (const uint8 *)midletList_ptr[k].name,
            strlen((char*)midletList_ptr[k].name)
            );
		item_data.item_content[1].item_data.text_buffer.wstr_ptr = 	wstr_ptr_tmp;
        //SCI_TRACE_LOW("CreateMidletListBox len = %d ",strlen((char*)midletList_ptr[k].name));
        //SCI_TRACE_LOW("CreateMidletListBox item_data.item_content[1].item_data.text_buffer.wstr_len = %d",item_data.item_content[1].item_data.text_buffer.wstr_len);
        item_data.item_content[0].item_data.image_id = JAVA_ICON;      
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        listbox_item.item_data_ptr = &item_data;
        GUILIST_AppendItem (MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID, &listbox_item);
    }
    // update listbox   
    GUILIST_SetCurItemIndex(MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID, 0);
    GUILIST_SetSelectedItem(MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID, 0, TRUE);

    jblendia_setMidletWinTitle(suitid);        

    SCI_FREE(midletList_ptr);
    
}

/**************************************************************************************************/
// Description:jblendia_setMidletWinTitle
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL void jblendia_setMidletWinTitle(int32 cursuitid)
{
    AJMS_Suite*     suiteList_ptr = PNULL;
    int32           i  = 0;
    int32           iNumOfSuite     = 0;  
    
    iNumOfSuite = ajms_ab_getSutieListCount( -1 );   
    
    //SCI_TRACE_LOW:"jblendia_setMidletWinTitle iNumOfSuite = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5960_112_2_18_2_22_20_141,(uint8*)"d",iNumOfSuite);
    
    if(iNumOfSuite < 0 || iNumOfSuite > (MAXJAVALISTSIZE + 1) )
    {
        //SCI_TRACE_LOW:"jblendia_setMidletWinTitle iNumOfSuite < 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5964_112_2_18_2_22_20_142,(uint8*)"");
        return;
    }
    
    suiteList_ptr = SCI_ALLOCA((uint32)iNumOfSuite*sizeof(AJMS_Suite));
    if(PNULL == suiteList_ptr)
    {
        //SCI_TRACE_LOW:"jblendia_setMidletWinTitle suiteList_ptr ALLOC FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5971_112_2_18_2_22_20_143,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(suiteList_ptr, 0, (uint32)iNumOfSuite*sizeof(AJMS_Suite));
    // get the information of all java applications
    if(ajms_ab_getSuiteList(-1 , suiteList_ptr, 0) < 0)
    {
        //SCI_TRACE_LOW:"jblendia_setMidletWinTitle ajms_ab_getSuiteList FAILED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_5979_112_2_18_2_22_20_144,(uint8*)"");
        return;
    }

    for(i = 0;i < iNumOfSuite;i++)
    {
        if(suiteList_ptr[i].id == cursuitid)
        {
            wchar           title[AJMS_MAX_ENTRY_NAME_LENGTH] = { 0 };
            int             titlelen = 0; 
            titlelen = GUI_UTF8ToWstr(title,AJMS_MAX_ENTRY_NAME_LENGTH,
                                (const uint8 *)suiteList_ptr[i].suiteName,
                                strlen((char*)suiteList_ptr[i].suiteName)); 
            GUIWIN_SetTitleText(MMIJAVA_AMS_MIDLETLIST_WIN_ID, title, MIN(titlelen, AJMS_MAX_ENTRY_NAME_LENGTH),  SCI_FALSE);      
            break;
        }
    }
    
    SCI_FREE(suiteList_ptr);
}

/**************************************************************************************************/
#ifdef JAVA_OTA_DELETE_REPORT_SUPPORT
//support only one delete url
uint8 ota_delrpt_url[AJMS_MAX_ENTRY_URL_LENGTH + 1]={0};

void jblendia_send_delete_report(void)
{
	// int err=0,sys_drv_letter;
	int req_num,i;
	int32 file_size;
	int urlSize;
	char *sys_drv_path="C:\\delrpt.txt";
	KJavaStat jstat;
	SFS_HANDLE handle = PNULL;
	signed long readLen=0;
	int ret = 0;
	wchar wchar_sys_drv_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
	
	JWE_LOG(jblendia_send_delete_report,("sys_drv_path:%s len=%d len=%d",sys_drv_path,strlen(sys_drv_path),	MMIAPICOM_Wstrlen(sys_drv_path)));	/*lint !e64*/

   if( kjava_fs_stat2(sys_drv_path, &jstat) == KJAVA_FILE_ERR_NOT_EXIST )
	{
		//SCI_TRACE_LOW:"C:\\delrpt.txt not exist"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6021_112_2_18_2_22_20_145,(uint8*)"");
		return;
	}
	else
  	{
	    //SCI_TRACE_LOW:"jblendia_send_delete_report file name = %s"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6026_112_2_18_2_22_20_146,(uint8*)"s", sys_drv_path);
	    GUI_UTF8ToWstr(wchar_sys_drv_path, MMIFILE_FULL_PATH_MAX_LEN+1, (uint8*)sys_drv_path,strlen(sys_drv_path));
	    handle = MMIAPIFMM_CreateFile(wchar_sys_drv_path, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
	    //SCI_TRACE_LOW:"jblendia_send_delete_report setting file handle = %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6029_112_2_18_2_22_20_147,(uint8*)"d", handle);

		file_size=jstat.size.low-jstat.size.high;
		//file_size=MMIAPIFMM_GetFileSize((SFS_HANDLE)handle);
		req_num = file_size/256;
		JWE_LOG(jblendia_send_delete_report,("yuyu open success req_num:%d %d %x",req_num,file_size,handle));

		for(i=0;i<req_num;i++)
		{
			if(i>=1){
				JWE_LOG(jblendia_send_delete_report,("delete url num could not > %d",i));
				break;
			}
			
			SCI_MEMSET(ota_delrpt_url,0x00,256);

			if(handle)
			{
				ret = MMIAPIFMM_ReadFile(handle, ota_delrpt_url, 256, &readLen, 0);/*lint !e64*/
				urlSize=strlen((char*)ota_delrpt_url);
				JWE_LOG(jblendia_send_delete_report,("send nofify:%s  urlSize=%d kjava_fs_read's ret=%d readLen=%d",ota_delrpt_url,urlSize,ret,readLen));
 			 	if(0 != urlSize)
				{
    			    if(jwe_ota_notify(AJMS_ERR_OTA_DELETION_NOTIFICATION, 
    								(char*)ota_delrpt_url, 
    								(int)urlSize ))
    			    {
    				    s_ota_notify_is_success = FALSE;
                    }
				}

				JWE_LOG(jblendia_send_delete_report,("yuyu- ret:%d handle:%d",ret,handle));
			}


		}

		if(handle)
			MMIAPIFMM_CloseFile(handle);
		
		MMIAPIFMM_DeleteFile(wchar_sys_drv_path,PNULL);
		JWE_LOG(jblendia_send_delete_report,("leave"));
	} 
}
#endif

#ifdef JAVA_DM_SUPPORT
#define MAX_DM_LEN 63
int jblendia_dm_id[MAX_DM_LEN+1];
static char dmidfile[]="C:\\dm.bin";
void jblendia_dm_save_id()
{
  wchar path[16];
  // uint32 count=0;
  uint32 j;
  KJavaFileHandle handle;
    int err;
  sprintf(path,dmidfile);/*lint !e64*/

	handle = kjava_fs_open(path,KJAVA_FILE_OPEN_RW|KJAVA_FILE_OPEN_CREATE,0,&err);/*lint !e64*/
	  for(j=0;j<9;j++)
	{
	   JWE_LOG(jblendia_dm_save_id,("dmidfile name %c",path[j]));
	}
	  JWE_LOG(jblendia_dm_save_id,("dmidfile create (%d)",handle));
							
	if(handle == NULL)
	{
		   return;	
	}
	else
	{
		kjava_fs_write(jblendia_dm_id,sizeof(jblendia_dm_id),handle,0);
		kjava_fs_close(handle,0);
	}

}

void jblendia_dm_init_id(void)
{
    int32 count=0;
    int n,i,j;
    KJavaFileHandle handle = 0; 
    int err = 0;
    char sys_drv_letter = 'C';
    dmidfile[0]=sys_drv_letter;
    JWE_LOG(jblendia_dm_init_id,("jblendia_dm_init_id enter "));
    for(j=0;j<9;j++)
    {
        JWE_LOG(jblendia_dm_init_id,("dmidfile name %c",dmidfile[j]));
    }

    handle = kjava_fs_open(dmidfile,KJAVA_FILE_OPEN_EXCL|KJAVA_FILE_OPEN_READ,0,&err);

    if(handle == NULL)
    {
        JWE_LOG(jblendia_dm_init_id,("dmidfile open failed(%d)",handle));
        memset(jblendia_dm_id,0,sizeof(jblendia_dm_id));
        n = ajms_ab_getSutieListCount(AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED);
        for(i=0;i<n;i++) jblendia_dm_id[i]=i;
        jblendia_dm_id[MAX_DM_LEN]=n;
        if(jblendia_dm_id[MAX_DM_LEN]<1)
        jblendia_dm_id[MAX_DM_LEN]=1;
        jblendia_dm_save_id();
    }
    else
    {
        JWE_LOG(jblendia_dm_init_id,("dmidfile open (%d)",handle));
        kjava_fs_read(jblendia_dm_id, sizeof(jblendia_dm_id), &count, handle, 0);
        kjava_fs_close(handle, 0);
        if(count!=sizeof(jblendia_dm_id))/*lint !e737*/
        {
            JWE_LOG(jblendia_dm_init_id,("dmidfile invalid"));
            kjava_fs_remove(dmidfile);
            jblendia_dm_init_id();
        }
    }
}

void jblendia_dm_remove(int suiteId)
{
   int DM_suit_id=0;	
   int thissuitid;
   int i;
   thissuitid=suiteId;
  JWE_LOG(jblendia_dm_remove,("del %d",thissuitid));

  	for(i=0;i<MAX_DM_LEN+1;i++)
	{
		if(jblendia_dm_id[i]!=0)
			JWE_LOG(jblendia_dm_remove,("all dmid [suiteid:%d][dmid:%d]",i,jblendia_dm_id[i]));
	}
	
  if(thissuitid>=0 && thissuitid<MAX_DM_LEN)
  {
  	  JWE_LOG(jblendia_dm_remove,("del 22: %d",thissuitid));
  	if(jblendia_dm_id[thissuitid]!=0)
  	{
  		  JWE_LOG(jblendia_dm_remove,("del dmid: %d",jblendia_dm_id[thissuitid]));
		   DM_suit_id=jblendia_dm_id[thissuitid];
		   jblendia_dm_id[thissuitid]=0;
		   jblendia_dm_save_id();
		   
		   DM_suit_id=-1*DM_suit_id;
		  JWE_LOG(jblendia_dm_remove,("-dmid: %d",DM_suit_id));		   
		   MMIAPIDM_HandleScomoCallback(DM_suit_id,PNULL,0,PNULL,0);		   
  	}
  }

}

static void jblendia_set_dm_midlet_suiteID(int suiteid,int oldsuiteid)
{
	int i;
	int dmid;
    	JWE_LOG(jblendia_set_dm_midlet_suiteID,("enter,suiteid=%d,old=%d",suiteid,oldsuiteid)); 

	  if(oldsuiteid<0 || oldsuiteid>=MAX_DM_LEN)
	  {
		   dmid=++jblendia_dm_id[MAX_DM_LEN];
	   }
	  else
	  {
		   dmid=jblendia_dm_id[oldsuiteid];
		   jblendia_dm_id[oldsuiteid]=0;
		   if(dmid==0)
		   {
			JWE_LOG(jblendia_set_dm_midlet_suiteID,("warning:dmid[%d]=0",oldsuiteid));
		     dmid=++jblendia_dm_id[MAX_DM_LEN];
		   }
	  }
	  jblendia_dm_id[suiteid]=dmid;
	  jblendia_dm_save_id();
	for(i=0;i<MAX_DM_LEN+1;i++)
	{
		if(jblendia_dm_id[i]!=0)
			JWE_LOG(jblendia_set_dm_midlet_suiteID,("all [suiteid:%d][dmid:%d]",i,jblendia_dm_id[i]));
	}
	  
	  
}

LOCAL int jblendia_get_dmid_from_storageid(int suiteid)
{
	  int dmid=0;
	  // wchar c;
	  if(suiteid>=0 && suiteid<MAX_DM_LEN)
	   dmid=jblendia_dm_id[suiteid];
	  JWE_LOG(jblendia_get_dmid_from_storageid,("DMID=%d" ,dmid));
	  return dmid;
}
                               
LOCAL void DM_JAVAHandleScomoCallback(int result)
{
    AJ_S32  ret = 0, flag = 0;
	AJ_U8   *jarmanifestBuffer = PNULL;
	// AJ_U8	*urlBuf = PNULL;
	AJ_S32  jarmanifestSize = 0;
	// AJ_S32	urlSize = 0;

    /* Begin: send MIDlet-Delete-Notify if this property exists in jad or MANIFEST.MF */
    // SFS_HANDLE           jadHandle = 0;
    // unsigned int         readCount = 0;
  //  AJ_S32    jadSize = 0;            /* jad buffer size */

    AJ_U8   *VersionProperty = AJ_NULL; /* property buffer */
    AJ_S32   VersionPropertySize = 0;   /* property buffer size */

    AJ_U8   *NameProperty = AJ_NULL; /* property buffer */
    AJ_S32   NamePropertySize = 0;   /* property buffer size */
	
    const AJ_U8   Versionkey[] = "MIDlet-Version";
	const AJ_U8   Namekey[]  ="MIDlet-Name";

    wchar   Version[80]={0};
    uint16   Version_len = 0;

    wchar    Name[80]={0};
    uint16   Name_len = 0;
    uint16   thissuitid = 0;
    uint16   thisoldsuitid = 0;	 
    uint16   dmid = 0;		

    JWE_LOG(DM_JAVAHandleScomoCallback,("result =%d  installfromdm= %d",result,installfromdm));

    if(installfromdm==1)
    {
    	if(result==0)
    	{
    		MMIAPIDM_HandleScomoCallback(1, PNULL, 0, PNULL, 0);
    	}
    	else if(result==2)
    	{
    			installfromdm=0;
    			thissuitid=s_local_install_result.suiteId;
    			thisoldsuitid=s_local_check_result.oldId;
       			JWE_LOG(DM_JAVAHandleScomoCallback,(" suiteId= %d  oldid=%d  installfromdm=%d",thissuitid,thisoldsuitid,installfromdm));
    			
    //install OK, then save DMID

    			jblendia_set_dm_midlet_suiteID(thissuitid,thisoldsuitid);
    			dmid=jblendia_get_dmid_from_storageid(thissuitid);

    			flag = 1;	
    			jarmanifestSize = ajms_ab_getManifestFileContent (thissuitid, NULL, 0);
    		    	JWE_LOG(DM_JAVAHandleScomoCallback,(" jadSize = %d thissuitid=%d",jarmanifestSize,thissuitid));
    			if(jarmanifestSize > 0)
    			{
    				jarmanifestBuffer = (AJ_U8*)SCI_ALLOC(jarmanifestSize);        
    				SCI_MEMSET(jarmanifestBuffer, 0, jarmanifestSize);

    				ret = ajms_ab_getManifestFileContent (thissuitid, jarmanifestBuffer, jarmanifestSize);
    				if (ret <= 0)
    				{
    					JWE_LOG(DM_JAVAHandleScomoCallback,( "can't get jad file, return %d.\n", ret ));   
    					flag = 0;
    				}
    				else
    				{
    					VersionPropertySize=ajms_ab_getAppDescProperty (jarmanifestBuffer, jarmanifestSize, AJMS_DESC_TYPE_MANIFEST, (AJ_U8* )Versionkey, AJ_NULL, 0);
    					JWE_LOG(DM_JAVAHandleScomoCallback,( " 1111 VersionPropertySize=%d", VersionPropertySize ));
    					if(VersionPropertySize <= 0)
    					{
    						flag = 0;
    					}
    					else
    					{
    						VersionProperty = (AJ_U8*)SCI_ALLOC(VersionPropertySize+1);        
    						SCI_MEMSET(VersionProperty, 0, VersionPropertySize);
    						ret = ajms_ab_getAppDescProperty (jarmanifestBuffer, jarmanifestSize, AJMS_DESC_TYPE_MANIFEST, (AJ_U8* )Versionkey, VersionProperty, VersionPropertySize);
    						VersionProperty[VersionPropertySize] = 0;
    						JWE_LOG(DM_JAVAHandleScomoCallback,( " VersionPropertySize= %d ret=%d.\n", VersionPropertySize,ret ));
    						if (ret <= 0)
    						{
    						    	JWE_LOG(DM_JAVAHandleScomoCallback,( " return %d.\n", ret ));   
    						    	flag = 0;
    						}
    						//utr-8 to wchar
    						Version_len = GUI_UTF8ToWstr(Version,
                                                           70,
                                                            (const uint8 *)VersionProperty,
                                                            VersionPropertySize);  
    						
    					}
    					NamePropertySize=ajms_ab_getAppDescProperty (jarmanifestBuffer, jarmanifestSize, AJMS_DESC_TYPE_MANIFEST, (AJ_U8* )Namekey, NULL, 0);
    					JWE_LOG(DM_JAVAHandleScomoCallback,( " 222 NamePropertySize=%d", NamePropertySize ));
    					if(NamePropertySize <= 0)
    					{
    						flag = 0;
    					}
    					else
    					{
    						NameProperty = (AJ_U8*)SCI_ALLOC(NamePropertySize+1);        
    						SCI_MEMSET(NameProperty, 0, NamePropertySize);
    						ret = ajms_ab_getAppDescProperty (jarmanifestBuffer, jarmanifestSize, AJMS_DESC_TYPE_MANIFEST, (AJ_U8* )Namekey, NameProperty, NamePropertySize);
    						NameProperty[NamePropertySize] = 0;
    						JWE_LOG(DM_JAVAHandleScomoCallback,( "VersionPropertySize= %d ret=%d.\n", NamePropertySize,ret ));						
    						if (ret <= 0)
    						{
    						    	JWE_LOG(DM_JAVAHandleScomoCallback,( "return %d.\n", ret ));   
    						    	flag = 0;
    						}
    						Name_len = GUI_UTF8ToWstr(Name,
                                                            70,
                                                            (const uint8 *)NameProperty,
                                                            NamePropertySize);
    						
    					}
    				}
    			}
    		    //update push database

    		JWE_LOG(DM_JAVAHandleScomoCallback,( "dmid=%d VersionProperty=%s NameProperty=%s",dmid, VersionProperty,NameProperty ));  
    		{
    			int index_ptr;
    			JWE_LOG(DM_JAVAHandleScomoCallback,( " Version_len=%d Name_len=%d", Version_len ,Name_len)); 
    			for(index_ptr=0;index_ptr<Version_len;index_ptr++)
    			{
    			 	JWE_LOG(DM_JAVAHandleScomoCallback,( " version=%c", Version[index_ptr] )); 
    			}

    			for(index_ptr=0;index_ptr<Name_len;index_ptr++)
    			{
    			 	JWE_LOG(DM_JAVAHandleScomoCallback,( "Name=%c", Name[index_ptr]) ); 
    			}			
    		}

    			
    		JWE_LOG(DM_JAVAHandleScomoCallback,( " out" ));   		
    		MMIAPIDM_HandleScomoCallback(dmid, Version, Version_len, Name, Name_len);

    		//error_return:
    		if( jarmanifestBuffer != PNULL){
    		    SCI_FREE(jarmanifestBuffer);
    		}
    		if( VersionProperty != PNULL){
    		    SCI_FREE(VersionProperty);
    		}
    		if( NameProperty != PNULL){
    		    SCI_FREE(NameProperty);
    		}

    	}

    }
	return;
}


PUBLIC void MMIAPIJAVA_InstallFromFilesystemForDM(wchar *pathname, int pathname_len, wchar *filename, int filename_len)
{
wchar ia_full_filename[MMIFMM_FULL_FILENAME_LEN+16] = {0,};
 int ia_full_filename_len = 0;

	int i=0;
        JWE_LOG(MMIJAVA_InstallFromFilesystemForDM,(" enter"));	
	JWE_LOG(MMIJAVA_InstallFromFilesystemForDM,(" Ia_Prepare_Fileinstaller_Parameter: enter! pathname=%s filename=%s",pathname,filename));

    SCI_MEMSET(ia_full_filename,0,sizeof(ia_full_filename));
    ia_full_filename_len = 0;
  //  SCI_MEMCPY(ia_full_filename_len,L"file:///",8*sizeof(wchar));
    //ia_full_filename_len += 8;
    SCI_MEMCPY(ia_full_filename+ia_full_filename_len,pathname,pathname_len*sizeof(wchar));/*lint !e737 */
    ia_full_filename_len += pathname_len;
    SCI_MEMCPY(ia_full_filename+ia_full_filename_len, L"\\", sizeof(wchar));
    ia_full_filename_len ++;
    SCI_MEMCPY(ia_full_filename+ia_full_filename_len, filename, filename_len*sizeof(wchar));/*lint !e737 */
    ia_full_filename_len += filename_len;
	for(i=0;i<ia_full_filename_len;i++)
	JWE_LOG(MMIJAVA_InstallFromFilesystemForDM,("full file name=%c",ia_full_filename[i]));	


	 //if( ajms_arc_getState() == AJSC_VMSTATE_NONE )
	 //{
	 	installfromdm=1;
	 	MMIAPIJAVA_StartLocalInstall(ia_full_filename,ia_full_filename_len,0);
	 //}
	// else
 	

}
#else
PUBLIC void MMIAPIJAVA_InstallFromFilesystemForDM(wchar *pathname, int pathname_len, wchar *filename, int filename_len)
{
	return;
}
#endif

/**************************************************************************************************/
// Description:JavaDeleteSuite
// Date:2007-06-02
// Author:huangjc
// Note:删除JAVA应用，参数suite_id为要删除应用的ID号
//*************************************************************************************************/
LOCAL void JavaDeleteSuite(int32 suite_id)
{
    AJ_S32              ret, flag;
    AJ_U8*              delete_jadBuffer = PNULL;
    AJ_S32              jadSize = 0;
    const AJ_U8*        jadStorageName = AJ_NULL;     // merlin
    AJ_U8*              property = AJ_NULL; /* property buffer */
    AJ_S32              propertySize = 0;   /* property buffer size */
    AJ_S32              getURLFlag;
    const AJ_U8         key[] = "MIDlet-Delete-Notify";
	KJavaFileHandle     handle=NULL;
    AJ_S32              errorCode;

    getURLFlag = 0;
    jadStorageName=ajms_ab_getJadPath(suite_id,&errorCode);
    flag = 1;	
    jadSize = ajms_ab_getJadFileContent (suite_id, NULL, 0);
    //SCI_TRACE_LOW:"JavaDeleteSuite jadSize = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6439_112_2_18_2_22_21_148,(uint8*)"d",jadSize);
	if(jadSize > 0)
	{
		delete_jadBuffer = (AJ_U8*)SCI_ALLOC(jadSize);        
		SCI_MEMSET(delete_jadBuffer, 0, jadSize);

		ret = ajms_ab_getJadFileContent (suite_id, delete_jadBuffer, jadSize);
		if (ret <= 0)
		{
			//SCI_TRACE_LOW:"delete notify failed, can't get jad file, return %d.\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6448_112_2_18_2_22_21_149,(uint8*)"d", ret );
			flag = 0;
		}
		else
		{
			propertySize = ajms_ab_getAppDescProperty (delete_jadBuffer, jadSize, AJMS_DESC_TYPE_JAD, key, NULL, 0);
			if(propertySize <= 0)
			{
				flag = 0;
			}
			else
			{
				property = (AJ_U8*)SCI_ALLOC(propertySize);
                if(PNULL != property)
                {
        			SCI_MEMSET(property, 0, propertySize);
        			ret = ajms_ab_getAppDescProperty (delete_jadBuffer, jadSize, AJMS_DESC_TYPE_JAD, key, property, propertySize);
        			property[propertySize - 1] = 0;////CR209539
                }
                
				if (ret <= 0)
				{
			    	//SCI_TRACE_LOW:"delete notify failed, can't get property \"MIDlet-Delete-Notify\", return %d.\n"
			    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6470_112_2_18_2_22_21_150,(uint8*)"d", ret );
			    	flag = 0;
				}
			}
		}
	}
    //update push database
    
#ifdef DYNAMIC_MAINMENU_SUPPORT
    MMIJAVA_DeleteSuiteRecordInMainmenu(suite_id);//要在java 删除之前调用这个函数
#endif

/* End: send MIDlet-Delete-Notify if this property exists in jad or MANIFEST.MF */ 
    if(AJMS_ERR_NONE == ajms_ota_delete(suite_id))
    {
        //SCI_TRACE_LOW:"JavaDeleteSuite flag = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6484_112_2_18_2_22_21_151,(uint8*)"d",flag);

	    /* Begin: send MIDlet-Delete-Notify if this property exists in jad or MANIFEST.MF */
	    //find the delete-notification. send it before delete midlet
	    if (flag == 1)
	    {
            int ret_value= -1;
            uint32 urlLen=0;
            wchar *url=NULL;
	        /* installResult->installNotifyUrl is utf-8 */
            /* jam_request_http_post need unicode */
            if(PNULL != property)
            {
                urlLen=SCI_STRLEN((char *)property)+1;            
                url= (wchar *)SCI_ALLOC( urlLen*sizeof(wchar) );
                if(PNULL != url)
                {
                    JcUtf8ToUni((const JKUint8*)property, urlLen, (JKWChar*)url, urlLen*sizeof(wchar), NULL, NULL, 0);
                }
            }
			{
				int err=0,req_num;
				uint32 file_size;
				KJavaLongSize skip_size;
				char my_url[256];
                // wchar sys_drv_letter;
                char sys_drv_path[50];
                SCI_MEMSET(sys_drv_path,0,sizeof(sys_drv_path));
				SCI_MEMSET(my_url,0,sizeof(my_url));
                sprintf(sys_drv_path,"C:\\delrpt.txt");
                handle = kjava_fs_open(sys_drv_path,KJAVA_FILE_OPEN_RW|KJAVA_FILE_OPEN_CREATE,0,&err);
				if(handle == NULL)
				{
					
				}
				else
				{
					file_size=MMIAPIFMM_GetFileSize((SFS_HANDLE)handle);
					req_num = file_size/256;
					JWE_LOG(JavaDeleteSuite,(" open success:%d  %d",file_size,req_num));
					skip_size.high = 0;
					skip_size.low = req_num*256;
					kjava_fs_seek(handle,&skip_size,KJAVA_FILE_SEEK_SET,0);
                    if(PNULL != property)
                    {
				        strncpy(my_url,(void *)property,urlLen);
                    }
					ret_value = kjava_fs_write(my_url,256,handle,0);
					JWE_LOG(JavaDeleteSuite,(" write:%d %s",ret_value,my_url));
					ret_value = kjava_fs_close(handle,0);
					JWE_LOG(JavaDeleteSuite,("close:%d",ret_value));
					//ret = jam_request_http_post( url, gm_jblendia_notifyMsg[12], &response );
                    #ifdef JAVA_OTA_DELETE_REPORT_SUPPORT
					jblendia_send_delete_report();
					#endif
				}
			}
	        if ( ret_value !=AJMS_ERR_NONE  ) {
	            JWE_LOG(JavaDeleteSuite,("notify fail. retval = %d", ret_value))
	        }

		  if ( url != NULL ) SCI_FREE( url ); url=NULL;
	    }

	    // free memory:
	    if ( property != NULL )
        {
                SCI_FREE( property );
                property=NULL;
        }
	   /* End: send MIDlet-Delete-Notify if this property exists in jad or MANIFEST.MF */

#ifdef MMI_JAVA_NW_V2
       MMIJAVA_DeleteNetworkSetting(suite_id);
#endif
#ifdef  JAVA_DM_SUPPORT
	    jblendia_dm_remove(suite_id);
#endif		
        //弹出删除成功的提示窗口
    /*    MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_DELETED, 
            TXT_NULL, 
            IMAGE_PUBWIN_SUCCESS, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);*/
    }     
    else
    {
        //弹出删除失败的提示窗口
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMMON_DEL_FAIL, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
    }

    if(jadSize > 0)
	{
		if(delete_jadBuffer != PNULL)	
        {
            SCI_FREE(delete_jadBuffer);
        }

	}
}

/**************************************************************************************************/
// Description:HandleJavaPermitSetWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA应用权限设置窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaPermitSetWinMsg(
                                             MMI_WIN_ID_T            win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM                      param
                                             )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;   
    MMI_MENU_ID_T       menu_id = ID_JAVA_AMS_OPTION_SET_CALL;
    int32    group_num = 0;/*权限设置的数量*/    
    AJMS_DomainGroup   *group_info = PNULL;
    AJ_S32  group_id = 0;/*权限设置的组别*/
    int32  cursuitid = (int32)MMK_GetWinAddDataPtr(win_id);
    //SCI_TRACE_LOW:"HandleJavaPermitSetWinMsg: msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6612_112_2_18_2_22_21_152,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        group_num = ajms_pcs_getGroupCount(s_suiteList[cursuitid].domainId);
        group_num = (group_num > MMIJAVA_PERMIT_MAX_NUM) ? MMIJAVA_PERMIT_MAX_NUM: group_num;
        group_info = SCI_ALLOC_APP((uint32)group_num * sizeof(AJMS_DomainGroup));
        SCI_MEMSET(group_info, 0, ((uint32)group_num * sizeof(AJMS_DomainGroup)));
        
        for(group_id = 0; group_id < group_num; group_id++)
        {
            ajms_pcs_getGroup(s_suiteList[cursuitid].domainId, group_id, &(group_info[group_id]));
            if ((( group_info[group_id].defaultMode & AJMS_PCS_ISPRIVATE ) != 0 ) 
                || (( group_info[group_id].defaultMode & AJMS_PCS_STATE_ALLOWED) != 0 )
                || (0 == group_info[group_id].interactiveModes)
                )
            {
                GUIMENU_SetItemGrayed(MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_CTRL_ID,MENU_JAVA_AMS_OPTION_PERMITION_SET, (MMI_MENU_ID_T)(menu_id + (uint32)group_id), TRUE);
				
            }
        }
        SCI_FREE(group_info);
        GUIWIN_SetTitleTextId(win_id,TXT_JAVA_AMS_OPTION_PERMITION_SET,FALSE);
        MMK_SetAtvCtrl(win_id, MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_CTRL_ID);
        break;
        
        
    case MSG_APP_OK:
    case MSG_APP_WEB:  
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        MMK_CreateWin((uint32*)MMIJAVA_PERMISSION_OPT_WIN_TAB, (ADD_DATA)cursuitid);
        break;
        
        
    case MSG_APP_CANCEL:        
    case MSG_CTL_CANCEL:
        //返回上一级
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/**************************************************************************************************/
// Description:HandleJavaPermitOptWinMsg
// Date:2007-06-02
// Author:huangjc
// Note:JAVA应用权限设置菜单选项窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaPermitOptWinMsg(
                                             MMI_WIN_ID_T            win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM                      param
                                             )
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    MMI_MENU_ID_T               menu_id = 0;
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_CTRL_ID_T               ctrl_id = MMIJAVA_PERMIT_OPT_LIST_CTRL_ID;
    uint16                      cur_item = 0;
    AJMS_DomainGroup            group_info = {0};
    static AJ_S32               cur_group = 0;
    static MMIJAVA_PERMIT_OPT_INFO opt_list_info = {0};
    const JAVA_MMI_TEXT_ID_E          title_text[] = {TXT_COMM_CALL,
                                                TXT_JAVA_AMS_OPTION_SET_LINK,
                                                TXT_MSG_SETTINGS, 
                                                TXT_JAVA_AMS_OPTION_SET_AUTOPLAY, 
                                                TXT_JAVA_AMS_OPTION_SET_LOCATION, 
                                                TXT_JAVA_AMS_OPTION_SET_MULTIM, 
                                                TXT_JAVA_AMS_OPTION_SET_USERDATAREAD, 
                                                TXT_JAVA_AMS_OPTION_SET_USERDATAWRITE
                                                }; 
    int32  cursuitid = (int32)MMK_GetWinAddDataPtr(win_id);
    //SCI_TRACE_LOW:"HandleJavaPermitOptWinMsg: msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6694_112_2_18_2_22_21_153,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //case MSG_FULL_PAINT:
        GUIMENU_GetId(MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_CTRL_ID, &group_id, &menu_id);
        cur_group = (AJ_S32)(menu_id - ID_JAVA_AMS_OPTION_SET_CALL);
        //SCI_TRACE_LOW:"cur_group = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6702_112_2_18_2_22_21_154,(uint8*)"d", cur_group);
        SCI_MEMSET(&group_info, 0, sizeof(AJMS_DomainGroup));
        SCI_MEMSET(&opt_list_info, 0, sizeof(MMIJAVA_PERMIT_OPT_INFO));
        ajms_pcs_getGroup(s_suiteList[cursuitid].domainId, cur_group, &group_info);
        ajms_mss_getGroupMode(s_suiteList[cursuitid].id, cur_group, &(opt_list_info.cur_mode));/*lint !e64*/
        if((group_info.interactiveModes & AJMS_PCS_MODE_BLANKET) != 0)
        {
            //SCI_ASSERT(opt_list_info.total < MMIJAVA_PERMIT_OPT_MAX_NUM);
            if(opt_list_info.total < MMIJAVA_PERMIT_OPT_MAX_NUM)
            {
                opt_list_info.item_text_id[opt_list_info.total] = TXT_MMIJAVA_AMS_OPTION_SET_3;//从不询问
                opt_list_info.mode[opt_list_info.total] = AJMS_PCS_MODE_BLANKET;
                if(AJMS_PCS_MODE_BLANKET == opt_list_info.cur_mode)
                {
                    cur_item = opt_list_info.total;
                }
                opt_list_info.total ++;
            }
        }
        if((group_info.interactiveModes & AJMS_PCS_MODE_SESSION) != 0)
        {
            //SCI_ASSERT(opt_list_info.total < MMIJAVA_PERMIT_OPT_MAX_NUM);
            if(opt_list_info.total < MMIJAVA_PERMIT_OPT_MAX_NUM)
            {
                opt_list_info.item_text_id[opt_list_info.total] = TXT_MMIJAVA_AMS_OPTION_SET_1;//访问一次
                opt_list_info.mode[opt_list_info.total] = AJMS_PCS_MODE_SESSION;
                if(AJMS_PCS_MODE_SESSION == opt_list_info.cur_mode)
                {
                    cur_item = opt_list_info.total;
                }
                opt_list_info.total ++;
            }
        }
        if((group_info.interactiveModes & AJMS_PCS_MODE_ONESHOT) != 0)
        {
            //SCI_ASSERT(opt_list_info.total < MMIJAVA_PERMIT_OPT_MAX_NUM);
            if(opt_list_info.total < MMIJAVA_PERMIT_OPT_MAX_NUM)
            {
                opt_list_info.item_text_id[opt_list_info.total] = TXT_COMMON_ASK_EVERYTIME;//每次询问
                opt_list_info.mode[opt_list_info.total] = AJMS_PCS_MODE_ONESHOT;
                if(AJMS_PCS_MODE_ONESHOT == opt_list_info.cur_mode)
                {
                    cur_item = opt_list_info.total;
                }
                opt_list_info.total ++;
            }
        }
        if((group_info.interactiveModes & AJMS_PCS_MODE_NO) != 0)
        {
            //SCI_ASSERT(opt_list_info.total < MMIJAVA_PERMIT_OPT_MAX_NUM);
            if(opt_list_info.total < MMIJAVA_PERMIT_OPT_MAX_NUM)
            {
                opt_list_info.item_text_id[opt_list_info.total] = TXT_COMM_NOT_ALLOWED;//不允许
                opt_list_info.mode[opt_list_info.total] = AJMS_PCS_MODE_NO;
                if(AJMS_PCS_MODE_NO == opt_list_info.cur_mode)
                {
                    cur_item = opt_list_info.total;
                }
                opt_list_info.total ++;
            }
        }      
        if ( opt_list_info.total>0)
        {
            PermitOptAppendListItem(ctrl_id, opt_list_info.item_text_id, opt_list_info.total);
        }
        else
        {
            MMK_CloseWin(win_id);
            break;
        }
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
        GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
        GUIWIN_SetTitleTextId(win_id,title_text[cur_group],FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        /*save the setting*/
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
       
        //SCI_TRACE_LOW:"HandleJavaPermitOptWinMsg:result =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_6788_112_2_18_2_22_21_155,(uint8*)"d",ajms_mss_setGroupMode(s_suiteList[cursuitid].id, cur_group, opt_list_info.mode[cur_item], PNULL));
        GUILIST_SetSelectedItem(ctrl_id, cur_item, TRUE);
        /*MMIPUB_OpenAlertWinByTextId( PNULL, 
            TXT_SUCCESS, 
            TXT_NULL, 
            IMAGE_PUBWIN_SUCCESS,
            PNULL,
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL );*/         
        MMK_CloseWin(win_id);
        break;
        
        
    case MSG_APP_CANCEL:        
    case MSG_CTL_CANCEL:
        //返回上一级
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        cur_group = 0;
        SCI_MEMSET(&opt_list_info, 0, sizeof(MMIJAVA_PERMIT_OPT_INFO));
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/**************************************************************************************************/
// Description:PermitOptAppendListItem
// Date:2007-06-02
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL void PermitOptAppendListItem (
                                    MMI_CTRL_ID_T ctrl_id,  /* list box control ID*/
                                    MMI_TEXT_ID_T *plist,   /* pointer of list items*/
                                    uint8 count                     /* list max item num */
                                    )
{
    int32 i = 0;
    MMI_STRING_T  str = {0};
    
    //SCI_ASSERT(plist != PNULL && count != 0);
    if(plist != PNULL && count != 0)
    {
        GUILIST_SetMaxItem(ctrl_id, count, FALSE);  

        for (i = 0; i < count; i++)
        {
            MMI_GetLabelTextByLang(plist[i], &str);     
            AppendOneLineTextListItem(ctrl_id, 
                GUIITEM_STYLE_ONE_LINE_RADIO,
                TXT_COMMON_OK,
                0,
                str.wstr_ptr,
                str.wstr_len,               
                i,
                FALSE
                );         
        }
    }
}

#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
/**************************************************************************************************/
// Description:MMIJAVA_ContinueOTAInstall
// Date:20110124
// Author:huangjc
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_ContinueOTAInstall(void)
{
	JWE_LOG(MMIJAVA_ContinueOTAInstall,("enter g_b_needContinueOTAInstall == %d",s_needContinueOTAInstall.is_needContinueOTAInstall));
	if(s_needContinueOTAInstall.is_needContinueOTAInstall)
	{
		JWE_LOG(MMIJAVA_ContinueOTAInstall,("s_needContinueOTAInstall.is_jad == %d",s_needContinueOTAInstall.is_jad ));
#if defined DRM_SUPPORT
        if(s_needContinueOTAInstall.is_drm)
        {
            MMIAPIJAVA_OTAInstallDrm(s_needContinueOTAInstall.fileName,s_needContinueOTAInstall.url);
        }
        else
#endif
        {
    		if(s_needContinueOTAInstall.is_jad)
            {
    			MMIJAVA_OTAInstallJad(s_needContinueOTAInstall.fileName,s_needContinueOTAInstall.mime_type, s_needContinueOTAInstall.url);
    		}
    		else
            {
    			MMIJAVA_OTAInstallJar(s_needContinueOTAInstall.fileName,s_needContinueOTAInstall.mime_type, s_needContinueOTAInstall.url);
    		}
        }
        
        if(s_needContinueOTAInstall.url != PNULL)
        {
            SCI_FREE(s_needContinueOTAInstall.url);
        }
        
		SCI_MEMSET(&s_needContinueOTAInstall,0,sizeof(MMIJAVA_CONTINUE_OTA_INSTALL_T));
		JWE_LOG(MMIJAVA_ContinueOTAInstall,("leave memset 0 s_needContinueOTAInstall" ));
	}
	
}
#endif
/**************************************************************************************************/
// Description:MMIJAVA_ContinueLocalInstall
// Date:20110124
// Author:huangjc
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_ContinueLocalInstall(void)
{
	JWE_LOG(MMIJAVA_ContinueLocalInstall,("enter g_b_needContinueLocalInstall == %d",g_b_needContinueLocalInstall ));
	if(g_b_needContinueLocalInstall)
	{
		g_b_needContinueLocalInstall = FALSE;
		MMK_CreateWin( ( uint32* )MMIJAVA_CHECK_WIN_TAB, PNULL);  
	}
	
}

/**************************************************************************************************/
// Description:MMIJAVA_ContinueLocalInstall
// Date:20110124
// Author:huangjc
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_ContinueInstall(void)
{
	JWE_LOG(MMIJAVA_ContinueInstall,("enter" ));
	MMIJAVA_ContinueLocalInstall();
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
	MMIJAVA_ContinueOTAInstall();
#endif
}

/**************************************************************************************************/
// Description:HandleJavaQueryLocalInstallInRunningWinMsg
// Date:20110124
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E  HandleJavaQueryLocalInstallInRunningWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
	MMI_RESULT_E    recode = MMI_RESULT_TRUE;
	//int32 err = 0;
	static BOOLEAN b_terminateVmToInstallFlag = FALSE;
	JWE_LOG(HandleJavaQueryLocalInstallInRunningWinMsg, ("enter msg_id=%x",msg_id));
	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:
			b_terminateVmToInstallFlag = FALSE;
			break;
		
		case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_APP_OK:
		case MSG_APP_WEB:
		case MSG_CTL_MIDSK:
			JWE_LOG(HandleJavaQueryLocalInstallInRunningWinMsg, ("OK, Quit Java and install"));
			g_b_needContinueLocalInstall = TRUE;
			b_terminateVmToInstallFlag = TRUE;
			MMK_CloseWin(win_id);
			MMIJAVA_stopVM_external(); 	
			break;

		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			JWE_LOG(HandleJavaQueryLocalInstallInRunningWinMsg, ("Cancel, Dont Quit Java and not install"));
			MMK_CloseWin(win_id);
			break;

		case MSG_CLOSE_WINDOW:
			JWE_LOG(HandleJavaQueryLocalInstallInRunningWinMsg, ("MSG_CLOSE_WINDOW b_terminateVmToInstallFlag=%d",b_terminateVmToInstallFlag));
			if(!b_terminateVmToInstallFlag)
			{
				if (PNULL != s_install_data_info_ptr)
				{
				    if (PNULL != s_install_data_info_ptr->jar_ptr)
				    {
				        SCI_FREE(s_install_data_info_ptr->jar_ptr);
				    }
				    if (PNULL != s_install_data_info_ptr->jad_ptr)
				    {
				        SCI_FREE(s_install_data_info_ptr->jad_ptr);
				    }    
                    if (NULL != s_install_data_info_ptr->jar_file_handler)
                    {
                        MMIAPIFMM_CloseFile(s_install_data_info_ptr->jar_file_handler);
                        s_install_data_info_ptr->jar_file_handler = NULL;
                    }
                    if (NULL != s_install_data_info_ptr->jad_file_handler)
                    {
                        MMIAPIFMM_CloseFile(s_install_data_info_ptr->jad_file_handler);
                        s_install_data_info_ptr->jad_file_handler = NULL;
                    }
				    s_install_data_info_ptr->is_ota = FALSE;
				    SCI_FREE(s_install_data_info_ptr);
				}


			}
			break;
			
		case MSG_KEYDOWN_RED:
			JWE_LOG(HandleJavaQueryLocalInstallInRunningWinMsg, ("MSG_KEYDOWN_RED"));
			MMK_CloseWin(win_id);
			break;
		
		default:
			recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
			break;
	}

	return recode;
}

/**************************************************************************************************/
// Description:HandleJavaQueryOTAInstallJadInRunningWinMsg
// Date:20110124
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E  HandleJavaQueryOTAInstallJadInRunningWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    //int32           err = 0;
    static BOOLEAN  b_terminateVmToInstallFlag = FALSE;
	
    JWE_LOG(HandleJavaQueryOTAInstallJadInRunningWinMsg, ("enter"));
    switch(msg_id)
    {
	case MSG_OPEN_WINDOW:
		b_terminateVmToInstallFlag = FALSE;
		break;
		
	case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
		JWE_LOG(HandleJavaQueryOTAInstallJadInRunningWinMsg, ("OK, Quit Java and install"));
		s_needContinueOTAInstall.is_needContinueOTAInstall= TRUE;
		s_needContinueOTAInstall.is_jad = TRUE;
		b_terminateVmToInstallFlag = TRUE;
		MMK_CloseWin(win_id);
		MMIJAVA_stopVM_external(); 	//g_jvm_is_running_by_mmi will be FALSE
		break;

	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		JWE_LOG(HandleJavaQueryOTAInstallJadInRunningWinMsg, ("Cancel, Dont Quit Java and not install"));
		MMK_CloseWin(win_id);
		break;
	
	case MSG_CLOSE_WINDOW:
		JWE_LOG(HandleJavaQueryOTAInstallJadInRunningWinMsg, ("MSG_CLOSE_WINDOW b_terminateVmToInstallFlag=%d s_needContinueOTAInstall.is_needContinueOTAInstall=%d",b_terminateVmToInstallFlag,s_needContinueOTAInstall.is_needContinueOTAInstall));
		if(!b_terminateVmToInstallFlag)
		{
			//Delete jad file
			int ret = 0;
			if( MMIAPIFMM_IsFileExist(s_needContinueOTAInstall.fileName,MMIAPICOM_Wstrlen(s_needContinueOTAInstall.fileName)))
			{
				ret = MMIAPIFMM_DeleteFile(s_needContinueOTAInstall.fileName,0);
			}
            
            ret = 0;
            if(g_wchar_ContinueOTAInstall_jad_backup_path_ptr != PNULL)
            {
    			if( MMIAPIFMM_IsFileExist(g_wchar_ContinueOTAInstall_jad_backup_path_ptr,MMIAPICOM_Wstrlen(g_wchar_ContinueOTAInstall_jad_backup_path_ptr)) )
    			{
    				ret = MMIAPIFMM_DeleteFile(g_wchar_ContinueOTAInstall_jad_backup_path_ptr,0);
    			}
                
                SCI_FREE(g_wchar_ContinueOTAInstall_jad_backup_path_ptr);
            }
            
			JWE_LOG(HandleJavaQueryOTAInstallJadInRunningWinMsg, ("MMIAPIFMM_DeleteFile ret = %d",ret));	

            if(s_needContinueOTAInstall.url != PNULL)
            {
                SCI_FREE(s_needContinueOTAInstall.url);
            }

            SCI_MEMSET(s_needContinueOTAInstall.fileName,0,sizeof(s_needContinueOTAInstall.fileName));
		}

		break;
		
	case MSG_KEYDOWN_RED:
		JWE_LOG(HandleJavaQueryOTAInstallJadInRunningWinMsg, ("MSG_KEYDOWN_RED"));
		MMK_CloseWin(win_id);
		break;
			
	default:
		recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
		break;
    }
    
    return recode;

}

/**************************************************************************************************/
// Description:HandleJavaQueryOTAInstallJarInRunningWinMsg
// Date:20110124
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E  HandleJavaQueryOTAInstallJarInRunningWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    //int32 err = 0;
    static BOOLEAN b_terminateVmToInstallFlag = FALSE;
	
    JWE_LOG(HandleJavaQueryOTAInstallJarInRunningWinMsg, ("enter"));
    switch(msg_id)
    {
	case MSG_OPEN_WINDOW:
		b_terminateVmToInstallFlag = FALSE;
		break;
		
	case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
		JWE_LOG(HandleJavaQueryOTAInstallJarInRunningWinMsg, ("OK, Quit Java and install"));
		s_needContinueOTAInstall.is_needContinueOTAInstall= TRUE;
		s_needContinueOTAInstall.is_jad = FALSE;
		b_terminateVmToInstallFlag = TRUE;
		MMK_CloseWin(win_id);
		MMIJAVA_stopVM_external(); 	
		break;

	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		JWE_LOG(HandleJavaQueryOTAInstallJarInRunningWinMsg, ("Cancel, Dont Quit Java and not install"));
		MMK_CloseWin(win_id);
		break;
	
	case MSG_CLOSE_WINDOW:
		JWE_LOG(HandleJavaQueryOTAInstallJarInRunningWinMsg, ("MSG_CLOSE_WINDOW b_terminateVmToInstallFlag=%d s_needContinueOTAInstall.is_needContinueOTAInstall=%d",b_terminateVmToInstallFlag,s_needContinueOTAInstall.is_needContinueOTAInstall));
		if(!b_terminateVmToInstallFlag)
		{
			//Delete jar file
			int ret = 0;
			if( MMIAPIFMM_IsFileExist(s_needContinueOTAInstall.fileName,MMIAPICOM_Wstrlen(s_needContinueOTAInstall.fileName)) )
			{	
				ret = MMIAPIFMM_DeleteFile(s_needContinueOTAInstall.fileName,0);
			}
			JWE_LOG(HandleJavaQueryOTAInstallJarInRunningWinMsg, ("MMIAPIFMM_DeleteFile jar ret = %d",ret));	

			//Delete jad file
			ret = 0;
            if(g_wchar_ContinueOTAInstall_jad_backup_path_ptr != PNULL)
            {
    			if( MMIAPIFMM_IsFileExist(g_wchar_ContinueOTAInstall_jad_backup_path_ptr,MMIAPICOM_Wstrlen(g_wchar_ContinueOTAInstall_jad_backup_path_ptr)) )
    			{
    				ret = MMIAPIFMM_DeleteFile(g_wchar_ContinueOTAInstall_jad_backup_path_ptr,0);
    			}
                
                SCI_FREE(g_wchar_ContinueOTAInstall_jad_backup_path_ptr);
            }
			JWE_LOG(HandleJavaQueryOTAInstallJarInRunningWinMsg, ("MMIAPIFMM_DeleteFile jad ret = %d",ret));	
            if(s_needContinueOTAInstall.url != PNULL)
            {
                SCI_FREE(s_needContinueOTAInstall.url);
            }
			SCI_MEMSET(s_needContinueOTAInstall.fileName,0,sizeof(s_needContinueOTAInstall.fileName));
		}
		break;
		
	case MSG_KEYDOWN_RED:
		JWE_LOG(HandleJavaQueryOTAInstallJarInRunningWinMsg, ("MSG_KEYDOWN_RED"));
		MMK_CloseWin(win_id);
		break;
			
	default:
		recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
		break;
    }
    
    return recode;

}


/**************************************************************************************************/
// Description:MMIAPIJAVA_StartLocalInstall
// Date:2007-06-08
// Author:huangjc
// Note:本地安装入口函数，供"资源管理器"调用
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_StartLocalInstall(const wchar *full_name,uint16 full_name_len, uint16 index)
{
    wchar                   suffix[MMIJAVA_DEFAULT_SUFFIX_LEN] = {0};
    
    wchar                   jar_full_name[MMIFMM_FILE_FILE_NAME_MAX_LEN] = {0};
    wchar                   jad_full_name[MMIFMM_FILE_FILE_NAME_MAX_LEN] = {0};
    wchar                   path_name[MMIFMM_FILE_FILE_NAME_MAX_LEN] = {0};
    MMI_JAVA_OPEN_RESULT_E  open_result = MMI_JAVA_OPEN_RESULT_TRUE;
    MMI_TEXT_ID_T           error_info_id =  TXT_COMM_INSTALL_FAILED;
    MMIJAVA_JAR_FILE_INFO_T jar_file_info = {0};
    MMIJAVA_JAD_FILE_INFO_T jad_file_info={0};
    uint16                  suffix_len = MMIJAVA_DEFAULT_SUFFIX_LEN;
    uint16                  path_len = MMIFMM_FILE_FILE_NAME_MAX_LEN;    
    
    MMI_IMAGE_ID_T          error_info_image_id = IMAGE_PUBWIN_FAIL;
    uint32                  readcount = 0;
    AJ_S32                  jar_file_name_len = 0;
    BOOLEAN                 is_jar_exist_pre = TRUE;
 #ifdef DRM_SUPPORT
    BOOLEAN                 is_drm = FALSE;
 #endif
    BOOLEAN                 free_flag = FALSE;
    #if 0
	if(MMIAPIJAVA_IsJavaRuning())
	{
	 	MMK_CreateWin((uint32 *)MMIJAVA_RESUME_DIALOG_WIN_TAB, PNULL);
		return;
	}
    #endif
	
    Ajava_Initiliztion_Special();
    MMIAPIFMM_SplitFullPathExt(full_name, 
        full_name_len,
        path_name,
        &path_len,
        PNULL,
        PNULL,
        suffix, //include dot 
        &suffix_len);     
 
    suffix_len = (suffix_len > MMIJAVA_DEFAULT_SUFFIX_LEN/2)? suffix_len: MMIJAVA_DEFAULT_SUFFIX_LEN/2;    
    MMIAPICOM_Wstrlower(suffix);   
    do
    {
        if (0 == MMIAPICOM_Wstrncmp(suffix, MMIJAVA_JAD_SUFFIX, suffix_len))//include dot 
        {
            //SCI_TRACE_LOW:"MMIAPIJAVA_StartLocalInstall(): jad"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7238_112_2_18_2_22_22_156,(uint8*)"");
            MMIAPICOM_Wstrcpy(jad_full_name,full_name); 
            MMIAPICOM_Wstrcpy(jar_full_name,full_name);
            MMIAPICOM_Wstrncpy(jar_full_name + full_name_len - suffix_len , MMIJAVA_JAR_SUFFIX, suffix_len);  
            
            if (!MMIAPIFMM_IsFileExist(jar_full_name,full_name_len))    // only jad, can't install!
            {
                is_jar_exist_pre = FALSE;
            }
            //open jad 
            {
                open_result = OpenJavaFile(jad_full_name, &jad_file_info);

                if (MMI_JAVA_OPEN_RESULT_TRUE != open_result )
                {
                    error_info_id = TXT_COMM_INSTALL_FAILED;
                    if( MMI_JAVA_OPEN_RESULT_TOO_BIG_FAIL == open_result)
                    {
                        error_info_id = TXT_JAVA_FILE_TOO_BIG;
                        error_info_image_id = IMAGE_PUBWIN_WARNING;
                    }
#if 0                                      
                    MMIPUB_OpenAlertWinByTextId(PNULL,
                        error_info_id,
                        TXT_NULL,
                        error_info_image_id,
                        PNULL,
                        PNULL,
                        MMIPUB_SOFTKEY_ONE,
                        PNULL);
#endif
                    MMIPUB_OpenAlertWarningWin(error_info_id);
                    free_flag = TRUE;
                    
                    
                    break; 
                }
                //get jar path from jad description
                if(!is_jar_exist_pre)
                {
                    uint8*                  jad_data = PNULL;
                    uint8*                  jar_file_name = PNULL;
                    //reset full path name
                    SCI_MEMSET(jar_full_name, 0, sizeof(jar_full_name));
                    //copy path name first
                    MMIAPICOM_Wstrncpy(jar_full_name, path_name, path_len); 

                    //get file name from jad description
                    jad_data = jad_file_info.file_content_ptr; //初始，如果是非filereader的方式
                    
                    #ifdef JAVA_INSTALL_FILE_READER
                    jad_data = SCI_ALLOCA(jad_file_info.file_len+1);

                    if(PNULL == jad_data)
                    {
                        #if 0
                        MMIPUB_OpenAlertWinByTextId(PNULL, 
                        TXT_COMM_INSTALL_FAILED, 
                        TXT_NULL, 
                        IMAGE_PUBWIN_FAIL, 
                        PNULL, 
                        PNULL,
                        MMIPUB_SOFTKEY_ONE,
                        PNULL); 
                        #endif
                        MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);
                        free_flag = TRUE;
                        break;
                    }
                    
                    SCI_MEMSET(jad_data,0,(jad_file_info.file_len+1));
                    
                    if (SFS_ERROR_NONE != MMIAPIFMM_ReadFile(jad_file_info.file_handler, jad_data, jad_file_info.file_len,&readcount, PNULL))
                    {
                        #if 0
                        MMIPUB_OpenAlertWinByTextId(PNULL, 
                        TXT_COMM_INSTALL_FAILED, 
                        TXT_NULL, 
                        IMAGE_PUBWIN_FAIL, 
                        PNULL, 
                        PNULL,
                        MMIPUB_SOFTKEY_ONE,
                        PNULL); 
                        #endif
                        MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);

                        SCI_FREE(jad_data);
                        free_flag = TRUE;
                        break;
                    }
                    #endif
            		jar_file_name_len = ajms_ab_getAppDescProperty(jad_data,jad_file_info.file_len,AJMS_DESC_TYPE_JAD,(const AJ_U8*)"MIDlet-Jar-URL",AJ_NULL,0);
                    
                    if(jar_file_name_len <= 0)
                    {
                        #if 0
                        MMIPUB_OpenAlertWinByTextId(PNULL, 
                        TXT_COMM_INSTALL_FAILED, 
                        TXT_NULL, 
                        IMAGE_PUBWIN_FAIL, 
                        PNULL, 
                        PNULL,
                        MMIPUB_SOFTKEY_ONE,
                        PNULL); 
                        #endif
                        MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);

                        #ifdef JAVA_INSTALL_FILE_READER
                        SCI_FREE(jad_data);
                        #endif
                        free_flag = TRUE;
                        break;
                    }
                    
            		jar_file_name = (AJ_U8*)SCI_ALLOCA(jar_file_name_len+1);  
                    
                    if(PNULL == jar_file_name)
                    {
                        #if 0
                        MMIPUB_OpenAlertWinByTextId(PNULL, 
                        TXT_COMM_INSTALL_FAILED, 
                        TXT_NULL, 
                        IMAGE_PUBWIN_FAIL, 
                        PNULL, 
                        PNULL,
                        MMIPUB_SOFTKEY_ONE,
                        PNULL);
                        #endif
                        MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);

                        #ifdef JAVA_INSTALL_FILE_READER
                        SCI_FREE(jad_data);
                        #endif
                        free_flag = TRUE;
                        break;
                    }

                    SCI_MEMSET(jar_file_name, 0, jar_file_name_len);
            		jar_file_name_len = ajms_ab_getAppDescProperty(jad_data,jad_file_info.file_len,AJMS_DESC_TYPE_JAD,(const AJ_U8*)"MIDlet-Jar-URL",jar_file_name,jar_file_name_len);

                    if(PNULL == jar_file_name)/*lint !e774*/
                    {
                        #if 0
                        MMIPUB_OpenAlertWinByTextId(PNULL, 
                        TXT_COMM_INSTALL_FAILED, 
                        TXT_NULL, 
                        IMAGE_PUBWIN_FAIL, 
                        PNULL, 
                        PNULL,
                        MMIPUB_SOFTKEY_ONE,
                        PNULL);
                        #endif
                        MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);

                        #ifdef JAVA_INSTALL_FILE_READER
                        SCI_FREE(jad_data);
                        #endif
                        SCI_FREE(jar_file_name);

                        free_flag = TRUE;
                        break;
                    }
                    
                    SCI_MEMSET(path_name, 0, sizeof(path_name));//use path name temp for strtowstr
                    MMIAPICOM_StrToWstr(jar_file_name,path_name);
                    MMIAPICOM_Wstrncpy(jar_full_name+path_len,path_name,jar_file_name_len);
                    MMIAPICOM_WstrTraceOut(jar_full_name,jar_file_name_len+path_len);
                    
                    #ifdef JAVA_INSTALL_FILE_READER
                    SCI_FREE(jad_data);
                    #endif
                    SCI_FREE(jar_file_name);
                }
                
                open_result = OpenJavaFile(jar_full_name, &jar_file_info);
                if (MMI_JAVA_OPEN_RESULT_TRUE != open_result )
                {
                    error_info_id = TXT_COMM_INSTALL_FAILED;
                    if( MMI_JAVA_OPEN_RESULT_TOO_BIG_FAIL == open_result)
                    {
                        error_info_id = TXT_JAVA_FILE_TOO_BIG;
                        error_info_image_id = IMAGE_PUBWIN_WARNING;
                    }

                    MMIPUB_OpenAlertWinByTextId(PNULL,
                        error_info_id,
                        TXT_NULL,
                        error_info_image_id,
                        PNULL,
                        PNULL,
                        MMIPUB_SOFTKEY_ONE,
                        PNULL); 

                    free_flag = TRUE;
                    break;
                }
            }
        }
        // lanuch jar file 
        else if (!MMIAPICOM_Wstrncmp(suffix, MMIJAVA_JAR_SUFFIX, suffix_len))
        {
            //SCI_TRACE_LOW:"MMIAPIJAVA_JavaGameDirectStart(): jar"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7416_112_2_18_2_22_23_157,(uint8*)"");
            MMIAPICOM_Wstrcpy(jar_full_name,full_name);    
           
            jad_file_info.file_content_ptr = PNULL;
            open_result =  OpenJavaFile(jar_full_name, &jar_file_info);
            if (MMI_JAVA_OPEN_RESULT_TRUE != open_result )
            {
                error_info_id = TXT_COMM_INSTALL_FAILED;
                if( MMI_JAVA_OPEN_RESULT_TOO_BIG_FAIL == open_result)
                {
                    error_info_id = TXT_JAVA_FILE_TOO_BIG;
                    error_info_image_id = IMAGE_PUBWIN_WARNING;
                }

                MMIPUB_OpenAlertWinByTextId(PNULL,
                    error_info_id,
                    TXT_NULL,
                    error_info_image_id,
                    PNULL,
                    PNULL,
                    MMIPUB_SOFTKEY_ONE,
                    PNULL);

                //SCI_TRACE_LOW:"MMIAPIJAVA_StartLocalInstall(): OPEN jar error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7439_112_2_18_2_22_23_158,(uint8*)"");
                free_flag = TRUE;
                break;
            }
        } 
     #ifdef DRM_SUPPORT
        else 
        {
            SFS_HANDLE file_handle;
            file_handle = MMIAPIFMM_CreateFile(full_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
            if(MMIFMM_FILE_TYPE_JAVA != MMIAPIDRM_GetMediaFileType(file_handle, PNULL))
            {
                //提示不是java文件
                MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_JAVA_DRM_NOT_JAVA, 
                TXT_NULL, 
                IMAGE_PUBWIN_WARNING, 
                PNULL, 
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL);
                MMIAPIFMM_CloseFile(file_handle);
                free_flag = TRUE;
                break;
            }
            
            if(MMIAPIDRM_IsRightsExpired(file_handle, PNULL, DRM_PERMISSION_EXECUTE))
            {
                //prompt 过期
                MMIAPIDRM_PreCheckFileStatus(full_name,DRM_PERMISSION_PLAY);/*lint !e605*/
                MMIAPIFMM_CloseFile(file_handle);
                free_flag = TRUE;
                break;
            }
            
            MMIAPIFMM_CloseFile(file_handle);           

            if (!OpenJavaFile((wchar*)full_name, &jar_file_info) )
            {
        		MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);
                //SCI_TRACE_LOW:"MMIAPIJAVA_StartLocalInstall(): OPEN jar error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7479_112_2_18_2_22_23_159,(uint8*)"");
                free_flag = TRUE;
                break;
            }
            is_drm  = TRUE;
        }
#endif

        s_install_data_info_ptr = SCI_ALLOCA(sizeof(MMIJAVA_INSTALL_DATA_INFO_T));
        if(PNULL == s_install_data_info_ptr)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL); 

            //SCI_TRACE_LOW:"s_install_data_info_ptr alloc failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7499_112_2_18_2_22_23_160,(uint8*)"");
            free_flag = TRUE;
            break;
        }
        
        SCI_MEMSET(s_install_data_info_ptr,0,sizeof(MMIJAVA_INSTALL_DATA_INFO_T));
        s_install_data_info_ptr->jar_len = jar_file_info.file_len;
        s_install_data_info_ptr->jad_len = jad_file_info.file_len;
#ifdef JAVA_INSTALL_FILE_READER
        s_install_data_info_ptr->jar_file_handler = jar_file_info.file_handler;
        s_install_data_info_ptr->jad_file_handler = jad_file_info.file_handler;
#else
        s_install_data_info_ptr->jar_ptr = jar_file_info.file_content_ptr;
        s_install_data_info_ptr->jad_ptr = jad_file_info.file_content_ptr;
#endif
        s_install_data_info_ptr->is_ota = FALSE;
        s_install_data_info_ptr->url = PNULL;
        
     #ifdef DRM_SUPPORT
        if(is_drm)
        {
            s_install_data_info_ptr->jar_file_path = SCI_ALLOCA((full_name_len+1)*sizeof(uint8));
            if(PNULL ==s_install_data_info_ptr->jar_file_path )
            {
                free_flag = TRUE;
                break;
            }
            
            SCI_MEMSET(s_install_data_info_ptr->jar_file_path,0,(full_name_len+1)*sizeof(uint8));
            MMIAPICOM_WstrToStr(full_name,s_install_data_info_ptr->jar_file_path);
        }
     #endif
     
        //SCI_TRACE_LOW:"jar_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7532_112_2_18_2_22_23_161,(uint8*)"d",s_install_data_info_ptr->jar_len);
        //SCI_TRACE_LOW:"jad_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7533_112_2_18_2_22_23_162,(uint8*)"d",s_install_data_info_ptr->jad_len);
        //SCI_TRACE_LOW:"jar_file_path=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7534_112_2_18_2_22_23_163,(uint8*)"d",s_install_data_info_ptr->jar_file_path);
        
    	if(g_jvm_is_running_by_mmi)
    	{
    		JWE_LOG(MMIAPIJAVA_StartLocalInstall,("g_jvm_is_running_by_mmi==TRUE (g_b_needContinueLocalInstall=%d s_needContinueOTAInstall.is_needContinueOTAInstall=%d) -> HandleJavaAlertInstallInRunningWinMsg",g_b_needContinueLocalInstall,s_needContinueOTAInstall.is_needContinueOTAInstall));		
    		if((!g_b_needContinueLocalInstall) && (!s_needContinueOTAInstall.is_needContinueOTAInstall))
            {
    			JWE_LOG(MMIAPIJAVA_StartLocalInstall,("No previous install step"));		
    			MMIPUB_OpenQueryWinByTextId( TXT_JAVA_INSTALL_IN_RUNNING, IMAGE_PUBWIN_QUERY,PNULL,HandleJavaQueryLocalInstallInRunningWinMsg );         		
    		}
    		else	
            {
    			JWE_LOG(MMIAPIJAVA_StartLocalInstall,("Has a previous install step"));		
    			MMIPUB_OpenAlertFailWin( TXT_JAVA_PREVIOUS_INSTALL_NOT_COMPLETED); 
    		}
            //no need set free_flag 20111013 by murphy
    		break;
    	}
    	else
    	{
    		JWE_LOG(MMIAPIJAVA_StartLocalInstall,("g_jvm_is_running_by_mmi==FALSE -> MMK_CreateWin MMIJAVA_CHECK_WIN_TAB"));		
            #ifdef UI_P3D_SUPPORT
            MMI_Disable3DEffect(MMI_3D_EFFECT_JAVA);
            #endif     
            MMK_CreateWin( ( uint32* )MMIJAVA_CHECK_WIN_TAB, PNULL);  
    	}
    }while(0);
    //MMK_CreateWin( ( uint32* )MMIJAVA_CHECK_WIN_TAB, PNULL); 

    if(free_flag)
    {
#ifdef JAVA_DM_SUPPORT
        DM_JAVAHandleScomoCallback(0);
#endif

        if (PNULL != s_install_data_info_ptr) //防止重复释放 
        {
            ReleaseInstallDataInfo();
        }
        else
        {
            #ifdef JAVA_INSTALL_FILE_READER
            if (NULL != jar_file_info.file_handler)
            {
                MMIAPIFMM_CloseFile(jar_file_info.file_handler);
                jar_file_info.file_handler = NULL;
            }
            if (NULL != jad_file_info.file_handler)
            {
                MMIAPIFMM_CloseFile(jad_file_info.file_handler);
                jad_file_info.file_handler = NULL;
            }
            #else
            if(PNULL != jar_file_info.file_content_ptr)
            {
                SCI_FREE(jar_file_info.file_content_ptr);
            }
            
            if (PNULL != jad_file_info.file_content_ptr)
            {
                SCI_FREE(jad_file_info.file_content_ptr);
            }       
            #endif
        }

    }
    //SCI_TRACE_LOW:"StartLocalInstall(): out!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7600_112_2_18_2_22_23_164,(uint8*)"");
}


/**************************************************************************************************/
// Description:MMIJAVA_ReturnToApplicationMenu
// Date:2010-12-1
// Author:murphy.xie
// Note:回到应用菜单
//*************************************************************************************************/
LOCAL void MMIJAVA_ReturnToApplicationMenu(void)
{
    //SCI_TRACE_LOW:"MMIJAVA_ReturnToApplicationMenu ENTER"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7611_112_2_18_2_22_23_165,(uint8*)"");
    MMK_CloseWin(MMIJAVA_AMS_WIN_ID);
    MMK_CloseWin(MMIJAVA_AMS_OPTION_WIN_ID);
    MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);
    MMK_CloseWin(MMIJAVA_MIDLET_WIN_ID);
    MMK_CloseWin(MMIJAVA_PAUSE_DIALOG_WIN_ID);
}

/**************************************************************************************************/
// Description:PauseDialg_OnBTN
// Date:2010-05-25
// Author:siyuan
// Note:JAVA 暂停
//*************************************************************************************************/
LOCAL void PauseDialg_OnBTN(PAUSE_DIALG_BTN_TYPE btn_type)
{
    //SCI_TRACE_LOW:"PauseDialg_OnBTN, btn_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7626_112_2_18_2_22_23_166,(uint8*)"d", btn_type);
    switch (btn_type)
    {
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
    case PAUSE_DIALG_BTN_MINIMIZE_TYPE:
    #ifdef MMI_TASK_MANAGER
        MMIAPIJAVA_StartApplet();
    #endif
        MMIJAVA_backgroundVM_external();
        MMIJAVA_ReturnToApplicationMenu();	   
        break;
#endif			
    case PAUSE_DIALG_BTN_PAUSE_TYPE:	
    
    #ifdef MMI_TASK_MANAGER
        MMIAPIJAVA_StartApplet();
    #endif	
        MMIJAVA_pauseVM_external(MMIJAVA_LONG_EVENT_STATE);
        MMIJAVA_ReturnToApplicationMenu();		
        break;
		
    case PAUSE_DIALG_BTN_TERMINATE_TYPE:
        MMIJAVA_stopVM_external();  
        //MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID); //不关闭为了和exit保持ui上的统一
        //MMK_CloseWin(MMIJAVA_MIDLET_WIN_ID);
        //MMK_CloseWin(MMIJAVA_PAUSE_DIALOG_WIN_ID);
        break;

    case PAUSE_DIALG_BTN_RETURN_TYPE:
        MMK_CloseWin(MMIJAVA_PAUSE_DIALOG_WIN_ID);
        break;
    }

}


/**************************************************************************************************/
// Description:HandleJavaPauseDialgWinMsg
// Date:2010-05-25
// Author:siyuan
// Note:JAVA 暂停窗口
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaPauseDialgWinMsg(
                                                 MMI_WIN_ID_T        win_id,    
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E               result  = MMI_RESULT_TRUE;
    uint16                     action_item_tid = 0;

    //SCI_TRACE_LOW:"HandleJavaPauseDialgWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7676_112_2_18_2_22_23_167,(uint8*)"d", msg_id);
	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
	        //GUIWIN_SetTitleTextId(win_id,TXT_APP_MANAGER,FALSE);
			//MMI_Enable3DMMI(FALSE);
			GUILIST_SetMaxItem(MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID, 3, FALSE);  //10
			CreatePauseDialogListBox(); 
            MMK_SetAtvCtrl(win_id,MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID);
            //GUIWIN_SetSoftkeyTextId(win_id, STXT_OK, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
			break;
        case MSG_FULL_PAINT:
            break;

		case MSG_CTL_MIDSK:    
		case MSG_KEYDOWN_WEB:        
		case MSG_CTL_OK:
		case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
			action_item_tid = GUILIST_GetCurItemIndex(MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID); 
			//SCI_TRACE_LOW:"HandleJavaPauseDialgWinMsg GUILIST_GetCurItemIndex %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7696_112_2_18_2_22_23_168,(uint8*)"d", action_item_tid);
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
			if(action_item_tid == 0)
			{//最小化
		        MAIN_UpdateIdleJavaState(TRUE);
				PauseDialg_OnBTN(PAUSE_DIALG_BTN_MINIMIZE_TYPE);
			}
			else if(action_item_tid == 1)
			{//暂停
				PauseDialg_OnBTN(PAUSE_DIALG_BTN_PAUSE_TYPE);
			}	
			else 
			{//结束
				PauseDialg_OnBTN(PAUSE_DIALG_BTN_TERMINATE_TYPE);
			}
#else
			if(action_item_tid == 0)
			{//暂停
				PauseDialg_OnBTN(PAUSE_DIALG_BTN_PAUSE_TYPE);
			}	
			else 
			{//结束
				PauseDialg_OnBTN(PAUSE_DIALG_BTN_TERMINATE_TYPE);
			}
#endif
			break;  

		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:		
		//返回
			PauseDialg_OnBTN(PAUSE_DIALG_BTN_RETURN_TYPE);
			break;

		case MSG_KEYDOWN_RED:
		//case MSG_KEYLONG_RED:
		//case MSG_KEYPRESSUP_RED:
		//case MSG_KEYREPEAT_RED:  for cr220176
		//暂停
			PauseDialg_OnBTN(PAUSE_DIALG_BTN_TERMINATE_TYPE);
			break;

		case MSG_CLOSE_WINDOW:  
            s_b_enable_pasue = TRUE;
			//MMI_Enable3DMMI(TRUE);
			break;

		default:
			result = MMI_RESULT_FALSE;
			break;
	}
    
    return (result);
}




LOCAL void ResumeDialg_OnBTN(RESUME_DIALG_BTN_TYPE btn_type)
{
    //SCI_TRACE_LOW:"ResumeDialg_OnBTN, btn_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7755_112_2_18_2_22_23_169,(uint8*)"d", btn_type);
    switch (btn_type)
    {
    case RESUME_DIALG_BTN_RESUME_TYPE:
 		
#ifdef MMI_TASK_MANAGER
        MMIAPIJAVA_StopApplet(FALSE);
#endif
        GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,GUILCD_GetInitAngle(GUI_MAIN_LCD_ID));

        MMK_CreateWin((uint32*)MMIJAVA_MIDLET_WIN_TAB, PNULL);      
        MMK_CloseWin(MMIJAVA_RESUME_DIALOG_WIN_ID);
        MMIJAVA_resumeVM_external();

        break;

    case RESUME_DIALG_BTN_TERMINATE_TYPE:	

        MMIJAVA_stopVM_external();   
        MMK_CloseWin(MMIJAVA_RESUME_DIALOG_WIN_ID);
        break;

    case RESUME_DIALG_BTN_RETURN_TYPE:
        MMK_CloseWin(MMIJAVA_RESUME_DIALOG_WIN_ID);
        break;

    }

}
/**************************************************************************************************/
// Description:HandleJavaPauseDialgWinMsg
// Date:2010-05-25
// Author:siyuan
// Note:JAVA恢复窗口
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaResumeDialgWinMsg(
                                                 MMI_WIN_ID_T        win_id,    
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 )
{// will be modified
    MMI_RESULT_E               result  = MMI_RESULT_TRUE;
    uint16                     action_item_tid = 0;
    //SCI_TRACE_LOW:"HandleJavaResumeDialgWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7797_112_2_18_2_22_23_170,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
	    //GUIWIN_SetTitleTextId(win_id,TXT_APP_MANAGER,FALSE);	
		//MMI_Enable3DMMI(FALSE);
		GUILIST_SetMaxItem(MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID, 2, FALSE);  //10
		CreateResumeDialogListBox();   
        //GUIWIN_SetSoftkeyTextId(win_id, STXT_OK, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
		MMK_SetAtvCtrl(win_id,MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:    
    case MSG_KEYDOWN_WEB:        
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		action_item_tid = GUILIST_GetCurItemIndex(MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID); 
		//SCI_TRACE_LOW:"HandleJavaAMSMidletListWinMsg GUILIST_GetCurItemIndex %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7816_112_2_18_2_22_24_171,(uint8*)"d", action_item_tid);
		if(action_item_tid == 0)
		{//继续
			ResumeDialg_OnBTN(RESUME_DIALG_BTN_RESUME_TYPE);
		}
		else
		{//结束
			ResumeDialg_OnBTN(RESUME_DIALG_BTN_TERMINATE_TYPE);
		}
        break;  
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
	 	ResumeDialg_OnBTN(RESUME_DIALG_BTN_RETURN_TYPE);
        break;

    case MSG_KEYDOWN_RED:
    case MSG_KEYLONG_RED:
    case MSG_KEYPRESSUP_RED:
    case MSG_KEYREPEAT_RED:
		//暂停
	 	ResumeDialg_OnBTN(RESUME_DIALG_BTN_RETURN_TYPE);
        break;
		
    case MSG_CLOSE_WINDOW: 
		//MMI_Enable3DMMI(TRUE);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}


/*****************************************************************************/
//  Description : local install jad reader for java install
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL AJ_S32 InstallJadDataReader(
                                            void*  buffer, 
                                            AJ_S32 offset, 
                                            AJ_S32 size
                                            )                                         
{
    uint32 readcount;
    
    if(PNULL ==  s_install_data_info_ptr)
    {
        return -1;
    }
    
    //SCI_TRACE_LOW:"mmijava_wintab.c InstallJadDataReader, size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7870_112_2_18_2_22_24_172,(uint8*)"d", size);
    
    if (SFS_ERROR_NONE != MMIAPIFMM_SetFilePointer(s_install_data_info_ptr->jad_file_handler, offset, SFS_SEEK_BEGIN))
    {
        return -1;
    }
    
    if (SFS_ERROR_NONE != MMIAPIFMM_ReadFile(s_install_data_info_ptr->jad_file_handler, buffer, size,&readcount, PNULL))
    {
        return -1;
    }


   return (int32)readcount;
}

/*****************************************************************************/
//  Description : local install jar reader for java install
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL  AJ_S32 InstallJarDataReader(
                                            void *buffer, 
                                            AJ_S32 offset, 
                                            AJ_S32 size
                                            )
{
    uint32 readcount;

    if(PNULL ==  s_install_data_info_ptr)
    {
        return -1;
    }
    
    //SCI_TRACE_LOW:"mmijava_wintab.c InstallJarDataReader, size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7903_112_2_18_2_22_24_173,(uint8*)"d", size);
     
    if (SFS_ERROR_NONE != MMIAPIFMM_SetFilePointer(s_install_data_info_ptr->jar_file_handler, offset, SFS_SEEK_BEGIN))
    {
        return -1;
    }
    
    if (SFS_ERROR_NONE != MMIAPIFMM_ReadFile(s_install_data_info_ptr->jar_file_handler, buffer, size,&readcount, PNULL))
    {
        return -1;
    }

    //SCI_TRACE_LOW:"mmijava_wintab.c InstallJarDataReader, readcount = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7915_112_2_18_2_22_24_174,(uint8*)"d", readcount);
    return (int32)readcount;
}


/*****************************************************************************/
//  Description :  JavaCheckNeededProperty
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JavaCheckNeededProperty(AJ_U8 *jad_content, int32 jad_contenr_len)
{
	int8 *str_start = PNULL;
    int8 *str_end = PNULL;
    int8  *temp = PNULL;
	int32 i = 0;
    int32 match = 0;

	str_start = (int8 *)jad_content;
	str_end = strstr((void *)str_start,":");	
	for(;str_end !=NULL;)
	{	
		for(i=0;i<MIDLET_ATTR_NUM;i++)
		{
			if(!strncmp((void *)str_start,s_midlet_attr[i],str_end-str_start))
			{	
				//SCI_TRACE_LOW:"point 0"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7940_112_2_18_2_22_24_175,(uint8*)"");
				match = 1;	
			}
		}
		if(match == 0)
		{
			if(!strncmp((void *)str_start,"MIDlet-",strlen("MIDlet-")))
			for(temp=str_start+7;temp<str_end;temp++)
			{
				if(((*(uint8 *)temp)>(int)'9')||((*(uint8 *)temp)<(int)'0'))
					break;
				if(temp == str_end - 1)
				{	
					//SCI_TRACE_LOW:"point 1 %d"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7953_112_2_18_2_22_24_176,(uint8*)"d",(*(uint8 *)temp));
					match = 1;
				}
			}
			if(!strncmp((void *)str_start,"MIDlet-Push-",strlen("MIDlet-Push-")))
			for(temp=str_start+12;temp<str_end;temp++)
			{
				if(((*(uint8 *)temp)>(int)'9')||((*(uint8 *)temp)<(int)'0'))
					break;
				if(temp == str_end - 1)
				{	
					//SCI_TRACE_LOW:"point 2 %d"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7964_112_2_18_2_22_24_177,(uint8*)"d",(*(uint8 *)temp));
					match = 1;
				}
			}
		}
		if((strncmp((void *)str_start,"MIDlet-",strlen("MIDlet-")) != 0) &&(strncmp((void *)str_start,"MicroEdition-",strlen("MicroEdition-")) != 0))
		{
			match = 1;
			//SCI_TRACE_LOW:"point 3"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_7972_112_2_18_2_22_24_178,(uint8*)"");
		}
		if(match == 0)
		{
			JWE_LOG(JavaCheckNeededProperty,("return FALSE"));		
			return FALSE;
		}
		match = 0;
		str_start = (AJ_U8 *)strstr((void *)str_start,"\n")+1;/*lint !e64 !e613*/
		str_end = (AJ_U8 *)strstr((void *)str_start,":");/*lint !e64*/
	}
	JWE_LOG(JavaCheckNeededProperty,("return TRUE"));		
	return TRUE;
} 

/*****************************************************************************/
//  Description :  JavaCheck5MidletProperty
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JavaCheck5MidletProperty(AJ_U8 *jad_content, int32 jad_contenr_len)
{
	int32 jad_data_size_len = 0;
	int32 jad_mandatory = 0;
	int32 i = 0;
	
	for(i=0;i<MMIJAVA_MIDLET_ARR;i++)
	{
		jad_data_size_len = ajms_ab_getAppDescProperty(jad_content,jad_contenr_len,AJMS_DESC_TYPE_JAD,
				(const AJ_U8*)s_midlet_5_attr[i],AJ_NULL,0);
		if(jad_data_size_len <= 0)
		{
			jad_mandatory = 1;
			break;
		}
			
	}
	
	if(jad_mandatory)
		return FALSE;
	else
		return TRUE;
}
/****************************************************************************
*函数定义:LocalCheck
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL BOOLEAN LocalCheck(
                               const int32 data_size,
                               const uint8 *data_addr,
                               BOOLEAN is_jad
                               )
{
    
    int32           result = 0;
    int32           data_type = 0;
    int8            jad_data_size[512] = {0};
    int32           jad_data_size_len = 0;
    uint8*          jad_url = PNULL;
    uint8*          jad_data =(uint8 *)data_addr;

 
    //SCI_TRACE_LOW:"LocalCheck data_size = %d,data_addr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8035_112_2_18_2_22_24_179,(uint8*)"dd",data_size,jad_data);
    if( is_jad )
    {
        #ifdef JAVA_INSTALL_FILE_READER
        jad_data = SCI_ALLOCA(data_size+1);
        
        if(PNULL == jad_data)
        {
           return FALSE;
        }
        SCI_MEMSET(jad_data,0,(data_size+1));
        
        if (data_size != InstallJadDataReader(jad_data, 0, data_size))
        {
            JWE_LOG(LocalCheck,("read jad file failed!"));    
            SCI_FREE(jad_data);
            return FALSE;  
        }
        #endif
        data_type = AJMS_CHECK_TYPE_JAD;
        
		if((data_size == 0) && (JavaCheckNeededProperty((AJ_U8 *)jad_data,data_size) == 0))
		{
			JWE_LOG(LocalCheck,("jad file is mismatch! data_size=%d",data_size));		
			return FALSE;								
		}	

		jad_data_size_len = ajms_ab_getAppDescProperty(jad_data,data_size,AJMS_DESC_TYPE_JAD,
							(const AJ_U8*)"MIDlet-Data-Size",jad_data_size,512);/*lint !e64*/
		
	
		if(!JavaCheck5MidletProperty((AJ_U8 *)jad_data,data_size))/* check jad file has 5 file necessary property */
		{
			JWE_LOG(LocalCheck,("jad file miss 5 necessary property!"));		
			return FALSE;							
		}
        #ifdef JAVA_INSTALL_FILE_READER
		SCI_FREE(jad_data);
        #endif
        //SCI_TRACE_LOW:"Java file type is jad"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8074_112_2_18_2_22_24_180,(uint8*)"");
    }
    else
    {
        data_type = AJMS_CHECK_TYPE_JAR;
        //SCI_TRACE_LOW:"Java file type is jar"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8079_112_2_18_2_22_24_181,(uint8*)"");
    }
    
    if ((PNULL != s_install_data_info_ptr) && s_install_data_info_ptr->is_ota)
    {
        jad_url = s_install_data_info_ptr->url;
    }  
    
#ifdef JAVA_INSTALL_FILE_READER
    if (is_jad)
    {
        result = ajms_ota_check( data_size, 
            AJ_NULL, 
            InstallJadDataReader,
            data_type,
            jad_url,
            &s_local_check_result,
            Java_LocalCheckCallBack ); 
    }
    else
    {
        result = ajms_ota_check( data_size, 
            AJ_NULL, 
            InstallJarDataReader,
            data_type,
            jad_url,
            &s_local_check_result,
            Java_LocalCheckCallBack ); 
    }

#else
    result = ajms_ota_check( data_size, 
        jad_data, 
        AJ_NULL,
        data_type,
        jad_url,
        &s_local_check_result,
        Java_LocalCheckCallBack );
#endif

    //SCI_TRACE_LOW:"LocalCheck result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8119_112_2_18_2_22_24_182,(uint8*)"d",result);
    if( AJMS_ERR_NONE == result )
    {

        //SCI_TRACE_LOW:"LocalCheck TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8123_112_2_18_2_22_24_183,(uint8*)"");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"LocalCheck FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8128_112_2_18_2_22_24_184,(uint8*)"");
          return FALSE;
    }
    
}

/****************************************************************************
*函数定义:LocalCheck
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL AJ_S32 Java_LocalCheckCallBack( 
                AJ_S32 total, 
                AJ_S32 step, 
                AJMS_CheckResult *result 
        )
{    
    
    //SCI_TRACE_LOW:"Java_LocalCheckCallBack step = %d,total = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8147_112_2_18_2_22_24_185,(uint8*)"dd",step,total );
    if( ( total >= 0 ) && ( total == step ) )
    {  
        MMK_PostMsg( ( MMI_WIN_ID_T )MMIJAVA_CHECK_WIN_ID,
            MSG_JAVA_CHECK_OK,
            PNULL,
            0 );
        MMI_TriggerMMITask(); 
        //SCI_TRACE_LOW:"LocalCheck Success"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8155_112_2_18_2_22_24_186,(uint8*)"");
        return 1; 
    }
    else if( total >= 0 ) 
    {
        //SCI_TRACE_LOW:"LocalCheck Waiting...."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8160_112_2_18_2_22_24_187,(uint8*)"");
        return 1; /* return 1 for continue MIDlet check */
    }
    else
    {
        //JBlendiaAjmsFini();
        MMK_PostMsg( ( MMI_WIN_ID_T )MMIJAVA_CHECK_WIN_ID,
            MSG_JAVA_CHECK_FAIL,
            PNULL,
            0 );
        MMI_TriggerMMITask(); 
        //SCI_TRACE_LOW:"MMIJAVA_LocalCheck Fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8171_112_2_18_2_22_24_188,(uint8*)"");
        return 0; /* return 0 for cancel MIDlet check */         
    } 
    
}

/****************************************************************************
*函数定义:Java_LocalInstall
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL BOOLEAN Java_LocalInstall(
                                 int32 jad_data_size,
                                 const uint8 *jad_data_addr,      
                                 int32 jar_data_size,
                                 const uint8 *jar_data_addr, 
                                 const uint8 *jar_file_path,
                                 int32 old_id,
                                 int32 ams
                                 )
{
    
    int32 result = 0;    
    uint8 *url = PNULL;  
  
    
    //SCI_TRACE_LOW:"Java_LocalInstall jar_data_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8198_112_2_18_2_22_24_189,(uint8*)"d",jar_data_size);

    if ((PNULL != s_install_data_info_ptr) && s_install_data_info_ptr->is_ota)
    {
        url = s_install_data_info_ptr->url;
    }
    //reset cancel flag 
    s_b_cancel_install = FALSE;
	if(PNULL != jar_file_path ) //for  drm java install
    {
     	if(NULL !=s_install_data_info_ptr->jar_file_handler)
		{
			MMIAPIFMM_CloseFile(s_install_data_info_ptr->jar_file_handler );
            s_install_data_info_ptr->jar_file_handler = NULL;
		}
 		
        result = ajms_ota_installEx( AJMS_DEFAULT_CATEGORY_ID_UNFILED,
                         PNULL,
                         PNULL,
                         jar_file_path,
                         ams,
                         old_id,
                        &s_local_install_result, 
                        Java_LocalInstallCallBack );
    }
    else
    {
  		result = ajms_ota_install( 0, 
        jad_data_size,
        jad_data_addr, 
        AJ_NULL, 
        AJ_NULL, 
        jar_data_size, 
        jar_data_addr,
        AJ_NULL, 
        ams, 
        old_id, 
        &s_local_install_result, 
        Java_LocalInstallCallBack ); 
    }
    if( AJMS_ERR_NONE == result )
    {
        //SCI_TRACE_LOW:"Java_LocalInstall TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8240_112_2_18_2_22_24_190,(uint8*)"");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"Java_LocalInstall FLASE result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8245_112_2_18_2_22_24_191,(uint8*)"d",result);

        return FALSE;
    }
    
}


/**************************************************************************************************/
// Description:MMIJAVA_LaunchMidletDirectAfterInstallOk
// Date:2010-11-30
// Author:huangjc
// Note:安装后直接运行
//*************************************************************************************************/
LOCAL void MMIJAVA_LaunchMidletDirectAfterInstallOk(AJMS_InstallResult install_result)
{
    
    if(!s_ota_notify_is_success)
    {
        MMIPUB_OpenAlertWarningWin(TXT_JAVA_OTA_NOTIFY_SENDING);
        return;
    }  

    //SCI_TRACE_LOW:"MMIJAVA_LaunchMidletDirectAfterInstallOk install_result.suiteId = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8267_112_2_18_2_22_24_192,(uint8*)"d",install_result.suiteId);
    MMK_CreateWin((uint32*)MMIJAVA_AMS_MIDLETLIST_WIN_TAB, (ADD_DATA)(install_result.suiteId));  
}

/*****************************************************************************/
// 	Description : launch midlet query window message handle
// Date:2010-11-30
//	Global resource dependence : 
//  Author:murphy.xie
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E JavaLaunchMidletQueryCallback(
                                        MMI_WIN_ID_T win_id, 	
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;    

    switch (msg_id)
    {
        case MSG_APP_OK:
        case MSG_CTL_OK:
            MMK_CloseWin(win_id);
            MMIJAVA_LaunchMidletDirectAfterInstallOk(s_local_install_result);	        
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:       
            MMK_CloseWin(win_id);
            break;
            
        default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
    }
    return result;
}


/**************************************************************************************************/
// Description:HandleInstallWinMsg
// Date:2007-06-08
// Author:huangjc
// Note:本地安装窗口处理函数
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleInstallWinMsg(
                                       MMI_WIN_ID_T            win_id, 
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM                      param
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;    
    int32        ams = (int32)MMK_GetWinAddDataPtr(win_id);
    MMI_STRING_T wait_text= {0};
#ifdef SELECT_INSTALL_DISK_SUPPORT
    char*       installto = PNULL;
#endif    
    SCI_MEMSET(&wait_text, 0, sizeof(wait_text));
    MMI_GetLabelTextByLang(TXT_COMMON_INSTALL, &wait_text); 
   
   //SCI_TRACE_LOW:"HandleInstallWinMsg msg_id =%x "
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8325_112_2_18_2_22_25_193,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:  
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        #ifdef JAVA_INSTALL_FILE_READER
        if((PNULL != s_install_data_info_ptr)&&(s_install_data_info_ptr->is_ota)&&(PNULL == s_install_data_info_ptr->jar_file_handler))
        #else
        if((PNULL != s_install_data_info_ptr)&&(s_install_data_info_ptr->is_ota)&&(PNULL == s_install_data_info_ptr->jar_ptr))
        #endif
        {
            MMK_PostMsg(VIRTUAL_WIN_ID,MSG_JAVA_OTA_DOWNLOAD_START,PNULL,PNULL);
					 
        }
        else
        {                
            //应用管理器中没有此应用
            //SCI_TRACE_LOW:"Java_LocalInstall jar_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8342_112_2_18_2_22_25_194,(uint8*)"d",s_install_data_info_ptr->jar_len);
            //SCI_TRACE_LOW:"s_local_check_result minimumInstallSize = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8343_112_2_18_2_22_25_195,(uint8*)"d",s_local_check_result.minimumInstallSize);

		    #ifdef SELECT_INSTALL_DISK_SUPPORT
	            if(!Java_LocalInstall_To(installto,
			  s_install_data_info_ptr->jad_len, 
	                s_install_data_info_ptr->jad_ptr, 
	                s_install_data_info_ptr->jar_len, 
	                s_install_data_info_ptr->jar_ptr, 
	                s_install_data_info_ptr->jar_file_path, 
	                s_local_check_result.oldId,
	                ams))    
		    #else
            if(!Java_LocalInstall( s_install_data_info_ptr->jad_len, 
                s_install_data_info_ptr->jad_ptr, 
                s_install_data_info_ptr->jar_len, 
                s_install_data_info_ptr->jar_ptr, 
                s_install_data_info_ptr->jar_file_path, 
                s_local_check_result.oldId,
                ams))
            #endif
            {                
                //SCI_TRACE_LOW:"Java_LocalInstall Fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8364_112_2_18_2_22_25_196,(uint8*)"");
                MMIPUB_OpenAlertWinByTextId(PNULL, 
                    TXT_COMM_INSTALL_FAILED, 
                    TXT_NULL, 
                    IMAGE_PUBWIN_FAIL, 
                    PNULL, 
                    PNULL,
                    MMIPUB_SOFTKEY_ONE,
                    PNULL);          
                MMK_CloseWin( win_id );                   
            }
        }
        break;
          
    case MSG_JAVA_INSTALL_OK:
        {
            //SCI_TRACE_LOW:"HandleInstallWinMsg s_install_data_info_ptr = %x, s_install_data_info_ptr->is_ota = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8380_112_2_18_2_22_25_197,(uint8*)"dd",s_install_data_info_ptr,s_install_data_info_ptr->is_ota);
            if(PNULL != s_install_data_info_ptr && s_install_data_info_ptr->is_ota)
		    {
                //SCI_TRACE_LOW:"HandleInstallWinMsg s_local_check_result.installNotifyUrl = %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8383_112_2_18_2_22_25_198,(uint8*)"s",s_local_check_result.installNotifyUrl);
			    if(0 != strlen((char*)s_local_check_result.installNotifyUrl))
                {
				    if(jwe_ota_notify(s_local_install_result.otaCode, 
                                                (char*) s_local_check_result.installNotifyUrl,
                                                strlen((char*)s_local_check_result.installNotifyUrl)
                                                ))
                    {
                        s_ota_notify_is_success= FALSE;
                    }                                               
			
                }
		    }
            #ifndef JAVA_SUPPORT_DIRECTLY_RUN
           /* MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_COMM_INSTALL_COMPLETE, 
                TXT_NULL, 
                IMAGE_PUBWIN_SUCCESS, 
                PNULL, 
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL);  */
            #else
			if(!(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) || MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE)))  //CR:NEWMS00110124
			{
            	MMIPUB_OpenQueryWinByTextId(TXT_JAVA_LAUNCH_MIDLET_SUITE, IMAGE_PUBWIN_QUERY, PNULL, JavaLaunchMidletQueryCallback);
           	}
		    #endif
            #ifdef MMI_JAVA_NW_V2
            MMIJAVA_CreateNetworkSetting(s_local_install_result.suiteId);
            #endif
            #ifdef JAVA_DM_SUPPORT
			DM_JAVAHandleScomoCallback(2);
            #endif
            
            //SCI_TRACE_LOW:"s_local_check_result.oldId = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8418_112_2_18_2_22_25_199,(uint8*)"d",s_local_check_result.oldId);
            #ifdef DYNAMIC_MAINMENU_SUPPORT
            if(s_local_check_result.oldId >= 0)
            {
                MMIJAVA_DeleteSuiteRecordInMainmenu(s_local_install_result.suiteId);
            }
            #endif

            #ifdef DYNAMIC_MAINMENU_SUPPORT
            MMIJAVA_AddSuiteToMainmenu(s_local_install_result.suiteId,(GUIMENU_FILE_DEVICE_E)s_device_arr[current_item]);
            #endif
            MMK_CloseWin( win_id );

            break;
        }
        
    case MSG_JAVA_INSTALL_FAIL:
        {
            if(PNULL != s_install_data_info_ptr && s_install_data_info_ptr->is_ota)
		    {
                if(0 != strlen((char*)s_local_check_result.installNotifyUrl))
                {
				    if(jwe_ota_notify(s_local_install_result.otaCode, 
                                                (char*) s_local_check_result.installNotifyUrl,
                                                strlen((char*)s_local_check_result.installNotifyUrl)
                                                ))
                    {
                        s_ota_notify_is_success= FALSE;
                    }
                }
		    }
            #ifdef JAVA_EVENT_ERROR_PROMPT_WIN
    	    JAVA_Install_ErrorHandle(s_local_install_result.otaCode);	
            #endif
            MMK_CloseWin( win_id );
            break;
        }
    case MSG_JAVA_INSTALL_START:
		#ifdef SELECT_INSTALL_DISK_SUPPORT

	            if(!Java_LocalInstall_To(installto,
			  s_install_data_info_ptr->jad_len, 
	                s_install_data_info_ptr->jad_ptr, 
	                s_install_data_info_ptr->jar_len, 
	                s_install_data_info_ptr->jar_ptr, 
                    s_install_data_info_ptr->jar_file_path, 
	                s_local_check_result.oldId,
	                ams))    
		#else
        if(!Java_LocalInstall( s_install_data_info_ptr->jad_len, 
                    s_install_data_info_ptr->jad_ptr, 
                    s_install_data_info_ptr->jar_len, 
                    s_install_data_info_ptr->jar_ptr, 
                    s_install_data_info_ptr->jar_file_path, 
                    s_local_check_result.oldId,
                    ams))
        #endif
        {                
            //SCI_TRACE_LOW:"Java_LocalInstall Fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8476_112_2_18_2_22_25_200,(uint8*)"");
            if(PNULL != s_install_data_info_ptr && s_install_data_info_ptr->is_ota)
		    {
			    if(0 != strlen((char*)s_local_check_result.installNotifyUrl))
                {
				    if(jwe_ota_notify(	s_local_check_result.otaCode, 
								    (char*) s_local_check_result.installNotifyUrl,
								    strlen((char*)s_local_check_result.installNotifyUrl)
								    ))
                    {
                        s_ota_notify_is_success= FALSE;
                    }
			    }									
		    }
            MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_COMM_INSTALL_FAILED, 
                TXT_NULL, 
                IMAGE_PUBWIN_FAIL, 
                PNULL, 
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL);          
            MMK_CloseWin( win_id );
        }
        break;        

    case MSG_JAVA_INSTALL_WAIT:  
        {
            MMI_WIN_ID_T    progress_win_id = MMIJAVA_PROGRESS_WIN_ID;
            AJ_S32          step= -1;
            
            if(PNULL != param)
            {
                step = *(AJ_S32 *)param;

                if(s_b_cancel_install)//用户已取消安装
                {
                    break;
                }

                if(!MMK_IsOpenWin(MMIJAVA_PROGRESS_WIN_ID))
                {
                    MMIPUB_OpenProgressWinByTextId(TXT_COMMON_INSTALL, &progress_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, JavaHandleProcessWinMsg);
                    MMIPUB_SetProgressTotalSize(&progress_win_id, 100);
                }
                else
                {                    
                    if(step > 1 && MMK_IsFocusWin(MMIJAVA_PROGRESS_WIN_ID))
                    {
                        MMIPUB_UpdateProgressBarEx(&progress_win_id, (step-1)*10, TRUE);
                    }
                }
            }
        }
        break;

    case MSG_JAVA_OTA_STOP:
        if(PNULL != s_install_data_info_ptr && s_install_data_info_ptr->is_ota)
        {
		    if(0 != strlen((char*)s_local_check_result.installNotifyUrl))
            {
                s_local_check_result.otaCode = AJMS_ERR_OTA_USER_CANCEL;
			    if(jwe_ota_notify(s_local_check_result.otaCode, 
							    (char*) s_local_check_result.installNotifyUrl,
							    strlen((char*)s_local_check_result.installNotifyUrl)
							    ))
                {
                    s_ota_notify_is_success= FALSE;
                }
		    }									
            MMIPUB_OpenAlertWinByTextId(PNULL, 
                            TXT_COMM_INSTALL_FAILED, 
                            TXT_NULL, 
                            IMAGE_PUBWIN_FAIL, 
                            PNULL, 
                            PNULL,
                            MMIPUB_SOFTKEY_ONE,
                            PNULL);          
            MMK_CloseWin(win_id);
        }        
        break;
    case MSG_CLOSE_WINDOW:
        s_b_cancel_install= TRUE; //非用户操作的取消安装

        MMIPUB_CloseWaitWin(MMIJAVA_PROGRESS_WIN_ID);	
        MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );  
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);

        break;        
        
    default:         
        recode = MMI_RESULT_FALSE;        
        break;
        
    }
    
    return recode;
    
}

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:Hanker.Zhou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  JavaHandleProcessWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    //SCI_TRACE_LOW:"HandleProcessWinMsg  msg_id %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8587_112_2_18_2_22_25_201,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);	
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        s_b_cancel_install = TRUE;
        //MMK_CloseWin(win_id); //会有installwin 统一关闭
        break;
    case MSG_CLOSE_WINDOW:
        s_b_cancel_install = TRUE;//非用户按键操作的取消
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/****************************************************************************
*函数定义:Java_LocalInstall
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL AJ_S32 Java_LocalInstallCallBack( 
                AJ_S32 total, 
                AJ_S32 step, 
                AJMS_InstallResult *result)
{  
    SCI_TRACE_LOW("Java_LocalInstallCallBack total = %d, step = %d, oldMicroVersion = %d, result = %d s_b_cancel_install =%d",
        total,step,s_local_check_result.oldMicroVersion,result?result->otaCode:0,s_b_cancel_install);
    if (s_b_cancel_install && ( total >= 0 ) && ( step < total )) //user cancel the install process
    {
        return 0; /* return 0 for cancel MIDlet install */        
    }
    
    if( ( total >= 0 ) && ( total == step ) )
    {  
        
#ifdef PREINSTALL_JAVA_SUPPORT //记录预安装应用的suit id
        //SCI_TRACE_LOW:"Java_LocalInstallCallBack s_preinstall_id = %d, s_local_check_result.oldId = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8632_112_2_18_2_22_25_202,(uint8*)"dd",s_preinstall_id,s_local_check_result.oldId);
        if (s_preinstall_id == s_local_check_result.oldId)
        {
            s_preinstall_id = result->suiteId;
            MMI_WriteNVItem(MMINV_PREINSTALL_INFO, &s_preinstall_id);
            //SCI_TRACE_LOW:"Java_LocalInstallCallBack s_preinstall_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8637_112_2_18_2_22_25_203,(uint8*)"d",s_preinstall_id);
        }
#endif
        MMIAPICOM_OtherTaskToMMI(( MMI_WIN_ID_T )MMIJAVA_INSTALL_WIN_ID,MSG_JAVA_INSTALL_OK,PNULL,0);
#ifdef MMI_JAVA_NW_V2
        if(s_local_check_result.oldId>=0)
        {
            MMIJAVA_DeleteNetworkSetting(s_local_check_result.oldId);            
        }
#endif
        return 1; /* return 1 for continue MIDlet install */
    }
    else if( total >= 0 ) 
    { 

        if(step > 0) //保证signal被同步处理，不滞后
        {
            SCI_SLEEP(100);
        }
        
        MMIAPICOM_OtherTaskToMMI(( MMI_WIN_ID_T )MMIJAVA_INSTALL_WIN_ID,MSG_JAVA_INSTALL_WAIT, &step, sizeof(AJ_S32));
        return 1; /* return 1 for continue MIDlet install */
    }
    else
    {       
        MMIAPICOM_OtherTaskToMMI(( MMI_WIN_ID_T )MMIJAVA_INSTALL_WIN_ID,MSG_JAVA_INSTALL_FAIL,PNULL,0);
        return 0; /* return 0 for cancel MIDlet install */         
    } 
    
}
/****************************************************************************
*函数定义:jwe_ota_download_callback
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
AJ_S32 jwe_ota_download_callback( AJ_S32 param, AJ_S32 status,  AJ_S32 value )
{   
    
    switch(status)
    {
        case AJSC_HTTPLIB_SUCCESS :
        {
            //SCI_TRACE_LOW:"[param = %d], function success, read %d bytes data in buffer\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8680_112_2_18_2_22_25_204,(uint8*)"dd", param, value);
            MMIJAVA_NET_fini();
		    s_ota_notify_is_success= TRUE;
		    if(PNULL != JARBuffer)
            {
		        SCI_FREE(JARBuffer);
		    }

            if(PNULL != ignoreBuf)
            {
                SCI_FREE(ignoreBuf);
            }
            break;
        }
        case AJSC_HTTPLIB_DATA_READ :
        {
            //SCI_TRACE_LOW:"[param = %d], reading... %d bytes read\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8696_112_2_18_2_22_25_205,(uint8*)"dd", param, value);
            break;
        }
        case AJSC_HTTPLIB_DATA_WRITTEN :
        {
            //SCI_TRACE_LOW:"[param = %d], writing... %d bytes write\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8701_112_2_18_2_22_25_206,(uint8*)"dd", param, value);
            break;
        }
        case AJSC_HTTPLIB_ERROR :
        {
            //SCI_TRACE_LOW:"[param = %d], error occurred in download files, error value = %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8706_112_2_18_2_22_25_207,(uint8*)"dd", param, value);
            MMIJAVA_NET_fini();
		    s_ota_notify_is_success= TRUE;

		    if(JARBuffer)
            {
		        SCI_FREE(JARBuffer);
		    }	
            if(PNULL != ignoreBuf)
            {
                SCI_FREE(ignoreBuf);
            }
            break;
        }
        case AJSC_HTTPLIB_GET_AUTH :
        {            
            
            //SCI_TRACE_LOW:"[param = %d], Authentication failed, set name / password.\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8723_112_2_18_2_22_25_208,(uint8*)"d", param);
            
            break;
        }
        default:
            break;
    }
    return 1;   //return 0 to cancel ota
}

/****************************************************************************
*函数定义:nofity server
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
LOCAL BOOLEAN jwe_ota_notify(int32 statusCode, char* url, int32 urlLength)
{
  	BOOLEAN ret = FALSE;
    int32  i = 0;
    char*  text = PNULL;
    
	//SCI_TRACE_LOW:"jwe_ota_notify enter statusCode = %d, urlLength = %d "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8745_112_2_18_2_22_26_209,(uint8*)"dd",statusCode,urlLength);

#ifdef MMI_JAVA_NW_V2
    g_active_suite_id = -1; //for active pdp purpose, use browser net setting
#endif
     MMIJAVA_NET_init();
       

    //SCI_TRACE_LOW:"jwe_ota_notify.., url = %s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8753_112_2_18_2_22_26_210,(uint8*)"s", url);
    for(i=0; i<STATUS_CODE_ARRAY_SIZE; i++)
    {
	   	//SCI_TRACE_LOW:"StatusCode[i].code: %d.\n"
	   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8756_112_2_18_2_22_26_211,(uint8*)"d",s_StatusCode[i].code );
        if (s_StatusCode[i].code == statusCode)
        {
            text = s_StatusCode[i].text;
        }
    }

	//SCI_TRACE_LOW:"t: %s.\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8763_112_2_18_2_22_26_212,(uint8*)"s",text );
    if (text == NULL)
    {
        //SCI_TRACE_LOW:"error: t==NULL.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8766_112_2_18_2_22_26_213,(uint8*)"" );
        return ret;  //no status code
    }
    
    JARBuffer = SCI_ALLOCA(1024 * 100);
    if( PNULL == JARBuffer)
    {
        //SCI_TRACE_LOW:"JARBuffer alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8773_112_2_18_2_22_26_214,(uint8*)"" );
        return ret;
    }
    SCI_MEMSET(JARBuffer, 0, 1024 * 100);
    
    ignoreBuf = SCI_ALLOCA(AJMS_IGNORE_BUFFER_SIZE*sizeof(char));
    if( PNULL == ignoreBuf)
    {
	    SCI_FREE(JARBuffer);
        //SCI_TRACE_LOW:"ignoreBuf alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8782_112_2_18_2_22_26_215,(uint8*)"" );
        return ret;
    }
    
    SCI_MEMSET(ignoreBuf, 0, AJMS_IGNORE_BUFFER_SIZE);
    
    if ( ajsc_http_sendRequest((AJ_U8*) url, (AJ_S32)urlLength,     //the url
                            AJSC_HTTPLIB_METHOD_POST,       //request mothod
                            AJ_NULL, 0,                     //headers
                            (AJ_U8*)text, (AJ_S32)strlen(text),                   //body to be send
                            AJMS_NOTIFY_PARAM,              //id
                            (AJ_U8*)ignoreBuf, AJMS_IGNORE_BUFFER_SIZE, //return buffer ptr and size
                            (AJ_U8*)JARBuffer, (AJ_S32)1024 * 100,               //heap addr and size
                            (AJSC_HTTPLIB_CALLBACK) jwe_ota_download_callback ) != 0 )
    { 
        //SCI_TRACE_LOW:"error: ajsc_http_sendRequest.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8797_112_2_18_2_22_26_216,(uint8*)"" );
        MMIJAVA_NET_fini();

		if(PNULL != JARBuffer)
        {
		    SCI_FREE(JARBuffer);
		}  
        
        if(PNULL != ignoreBuf)
        {
            SCI_FREE(ignoreBuf);
        }
    }
    else
    {   
        if (0 != s_ota_notify_timer_id)
        {
            MMK_StopTimer(s_ota_notify_timer_id);
            s_ota_notify_timer_id = 0;
        }
       s_ota_notify_timer_id = MMK_CreateWinTimer(VIRTUAL_WIN_ID, OTA_NOTIFY_TIMEOUT, SCI_FALSE);    
       //SCI_TRACE_LOW:"notify start.., status = %d.\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8818_112_2_18_2_22_26_217,(uint8*)"d", statusCode);
       ret = TRUE;
    }
    return ret;
} 



/****************************************************************************
*函数定义:MMIJAVA_HandleOTANotifyTimer
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_HandleOTANotifyTimer(void * param)
{
    if (s_ota_notify_timer_id == *(uint8*)param)
    {
        MMIJAVA_NET_fini();

		if(JARBuffer)
        {
		    SCI_FREE(JARBuffer);
		}  

        if(PNULL != ignoreBuf)
        {
            SCI_FREE(ignoreBuf);
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
// Description:HandleJavaQueryOTAInstallJarInRunningWinMsg
// Date:20110124
// Author:huangjc
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E  HandleJavaQueryOTAInstallDrmInRunningWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    //int32 err = 0;
    static BOOLEAN b_terminateVmToInstallFlag = FALSE;
	
    JWE_LOG(HandleJavaQueryOTAInstallDrmInRunningWinMsg, ("enter"));
    switch(msg_id)
    {
	case MSG_OPEN_WINDOW:
		b_terminateVmToInstallFlag = FALSE;
		break;
		
	case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
		JWE_LOG(HandleJavaQueryOTAInstallDrmInRunningWinMsg, ("OK, Quit Java and install"));
		s_needContinueOTAInstall.is_needContinueOTAInstall= TRUE;
		s_needContinueOTAInstall.is_drm = TRUE;
		b_terminateVmToInstallFlag = TRUE;
		MMK_CloseWin(win_id);
		MMIJAVA_stopVM_external(); 	
		break;

	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		JWE_LOG(HandleJavaQueryOTAInstallDrmInRunningWinMsg, ("Cancel, Dont Quit Java and not install"));
		MMK_CloseWin(win_id);
		break;
	
	case MSG_CLOSE_WINDOW:
		JWE_LOG(HandleJavaQueryOTAInstallDrmInRunningWinMsg, ("MSG_CLOSE_WINDOW b_terminateVmToInstallFlag=%d s_needContinueOTAInstall.is_needContinueOTAInstall=%d",b_terminateVmToInstallFlag,s_needContinueOTAInstall.is_needContinueOTAInstall));
		if(!b_terminateVmToInstallFlag)
		{
			SCI_MEMSET(s_needContinueOTAInstall.fileName,0,sizeof(s_needContinueOTAInstall.fileName));
		}
		break;
		
	case MSG_KEYDOWN_RED:
		JWE_LOG(HandleJavaQueryOTAInstallDrmInRunningWinMsg, ("MSG_KEYDOWN_RED"));
		MMK_CloseWin(win_id);
		break;
			
	default:
		recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
		break;
    }
    
    return recode;

}

#if defined(DRM_SUPPORT) && defined(JAVA_OTA_SUPPORT)
/*****************************************************************************/
//  Description : MMIAPIJAVA_OTAInstallDrm
//  Global resource dependence : none
//  Author: murphy.xie
//  Note: for ota drm download auto install callback function
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OTAInstallDrm(wchar *fileName, int8 *url)
{
    uint16                  url_len = 0;
    int32                   i = 0;
    MMIJAVA_JAR_FILE_INFO_T jar_file_info = {0};
    uint16                  file_name_len = 0;//(MMIAPICOM_Wstrlen(fileName);

    if(PNULL == fileName || PNULL == url)
    {
        //SCI_TRACE_LOW:"MMIAPIJAVA_OTAInstallDrm in param error null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8932_112_2_18_2_22_26_218,(uint8*)"");
        return;
    }
    url_len = strlen(url);
    file_name_len = MMIAPICOM_Wstrlen(fileName);
    if(0 == file_name_len ||0 == url_len)
    {
        //SCI_TRACE_LOW:"MMIAPIJAVA_OTAInstallDrm in param error len is 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8939_112_2_18_2_22_26_219,(uint8*)"");
    }
    
    if(!s_ota_notify_is_success)
    {
        MMIPUB_OpenAlertWarningWin(TXT_JAVA_OTA_NOTIFY_SENDING);
        return;
    }

    if(g_jvm_is_running_by_mmi)
    {
		JWE_LOG(MMIAPIJAVA_OTAInstallDrm,("g_jvm_is_running_by_mmi==TRUE (g_b_needContinueLocalInstall=%d s_needContinueOTAInstall.needContinueOTAInstall=%d) -> HandleJavaQueryOTAInstallInRunningWinMsg",g_b_needContinueLocalInstall,s_needContinueOTAInstall.is_needContinueOTAInstall));		
		if((!g_b_needContinueLocalInstall) && (!s_needContinueOTAInstall.is_needContinueOTAInstall))
        {
			JWE_LOG(MMIAPIJAVA_OTAInstallDrm,("No previous install step"));	
			SCI_MEMSET(s_needContinueOTAInstall.fileName,0,sizeof(s_needContinueOTAInstall.fileName));
			MMIAPICOM_Wstrcpy(s_needContinueOTAInstall.fileName, fileName);
            s_needContinueOTAInstall.url  = SCI_ALLOCA(url_len+1);
            if(PNULL == s_needContinueOTAInstall.url)
            {
                return;
            }
            SCI_MEMSET(s_needContinueOTAInstall.url,0,url_len+1);
			SCI_STRCPY(s_needContinueOTAInstall.url,url);
			MMIPUB_OpenQueryWinByTextId( TXT_JAVA_INSTALL_IN_RUNNING, IMAGE_PUBWIN_QUERY,PNULL,HandleJavaQueryOTAInstallDrmInRunningWinMsg );         		
		}
		else	
        {
			JWE_LOG(MMIAPIJAVA_OTAInstallDrm,("Has a previous install step"));		
			MMIPUB_OpenAlertFailWin( TXT_JAVA_PREVIOUS_INSTALL_NOT_COMPLETED); 
		}
        return;
    }

    if ((PNULL != s_install_data_info_ptr)&&s_install_data_info_ptr->is_ota)
    {
	    //SCI_TRACE_LOW:"MMIAPIJAVA_OTAInstallDrm s_install_data_info_ptr is not null"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_8975_112_2_18_2_22_26_220,(uint8*)"");
        return;
    }
    else
    {    
        SFS_HANDLE file_handle;

        JWE_LOG(MMIJAVA_OTAInstall,("url = %s",url));	
        MMIAPICOM_WstrTraceOut(fileName,file_name_len);
    
        file_handle = MMIAPIFMM_CreateFile(fileName, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
        if(MMIFMM_FILE_TYPE_JAVA != MMIAPIDRM_GetMediaFileType(file_handle, PNULL))
        {
            //提示不是java文件
            MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_JAVA_DRM_NOT_JAVA, 
            TXT_NULL, 
            IMAGE_PUBWIN_WARNING, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
            MMIAPIFMM_CloseFile(file_handle);
            return;
        }
        
        if(MMIAPIDRM_IsRightsExpired(file_handle, PNULL, DRM_PERMISSION_EXECUTE))
        {
            //prompt 过期
            MMIAPIDRM_PreCheckFileStatus(fileName,DRM_PERMISSION_PLAY);/*lint !e605*/
            MMIAPIFMM_CloseFile(file_handle);  
            return;
        }
        
        MMIAPIFMM_CloseFile(file_handle);           
        //记录文件handle 用于local check
        if (!OpenJavaFile((wchar*)fileName, &jar_file_info) )
        {
    		MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);
            //SCI_TRACE_LOW:"MMIAPIJAVA_StartLocalInstall(): OPEN jar error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9014_112_2_18_2_22_26_221,(uint8*)"");
            return;
        }

        s_install_data_info_ptr = SCI_ALLOCA(sizeof(MMIJAVA_INSTALL_DATA_INFO_T));
        if(PNULL == s_install_data_info_ptr)
        {
            JWE_LOG(MMIJAVA_OTAInstall,("s_install_data_info_ptr alloc error"));
            MMIPUB_OpenAlertWarningWin(TXT_COMM_INSTALL_FAILED);
    	#ifdef JAVA_DM_SUPPORT
    		DM_JAVAHandleScomoCallback(0);
    	#endif	

            if(NULL != jar_file_info.file_handler)
            {
                MMIAPIFMM_CloseFile(jar_file_info.file_handler);
                jar_file_info.file_handler = NULL;
            }
            return; 
        }
        
        SCI_MEMSET(s_install_data_info_ptr,0,sizeof(MMIJAVA_INSTALL_DATA_INFO_T));

        s_install_data_info_ptr->jar_len = jar_file_info.file_len;
        s_install_data_info_ptr->jar_file_handler = jar_file_info.file_handler;

        //保存url 
        s_install_data_info_ptr->is_ota = TRUE; 
        JWE_LOG(MMIJAVA_OTAInstall,("line = %d, s_install_data_info_ptr->is_ota = %d",__LINE__,s_install_data_info_ptr->is_ota));			  
        {
            url_len = strlen(url);
            JWE_LOG(MMIJAVA_OTAInstall,("line = %d, s_install_data_info_ptr->is_ota = %d",__LINE__,s_install_data_info_ptr->is_ota));		
            JWE_LOG(MMIJAVA_OTAInstall,("apple ots url_len = %d",url_len));	
            s_install_data_info_ptr->url = SCI_ALLOC_APP(url_len+1);
            SCI_MEMSET(s_install_data_info_ptr->url,0,url_len+1);
            JWE_LOG(MMIJAVA_OTAInstall,("line = %d, s_install_data_info_ptr->is_ota = %d",__LINE__,s_install_data_info_ptr->is_ota));	
            for (i = 0; i < url_len; i++) 
            {
                s_install_data_info_ptr->url[i] = url[i];
            }
            s_install_data_info_ptr->url[i] = 0;
            JWE_LOG(MMIJAVA_OTAInstall,("line = %d, s_install_data_info_ptr->is_ota = %d",__LINE__,s_install_data_info_ptr->is_ota));		
        }

        //保存文件路径，用于drm安装
        s_install_data_info_ptr->jar_file_path = SCI_ALLOCA((file_name_len+1)*sizeof(uint8));
        if(PNULL ==s_install_data_info_ptr->jar_file_path )
        {
            ReleaseInstallDataInfo();
            return;
        }

        SCI_MEMSET(s_install_data_info_ptr->jar_file_path,0,(file_name_len+1)*sizeof(uint8));
        MMIAPICOM_WstrToStr(fileName,s_install_data_info_ptr->jar_file_path);

    }

    JWE_LOG(MMIJAVA_OTAInstall,("jad_len=%d, s_install_data_info_ptr->is_ota = %d",s_install_data_info_ptr->jad_len,s_install_data_info_ptr->is_ota));	

    MMK_CreateWin( ( uint32* )MMIJAVA_CHECK_WIN_TAB, PNULL);
    
    return;   
}

#endif

#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
#ifdef BROWSER_SUPPORT_NETFRONT
/*****************************************************************************/
//  Description :MMIJAVA_OTADownloadFailHandle
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIJAVA_OTADownloadFailHandle(MMIDL_DOWNLOAD_RESULT_E downresult)
{
	int otacode=AJMS_ERR_OTA_USER_CANCEL;
	//SCI_TRACE_LOW:"MMIJAVA_OTADownloadFailHandle  downresult =%d, s_local_check_result.installNotifyUrl lenth = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9089_112_2_18_2_22_27_222,(uint8*)"dd",downresult, strlen((char*)s_local_check_result.installNotifyUrl));

	switch(downresult)
	{
		case MMIDL_DOWNLOAD_ERROR_NO_SPACE:
	       	otacode=AJMS_ERR_OTA_NO_ENOUGH_SPACE;//901
	       	break;
       	case MMIDL_DOWNLOAD_ERROR_STOP:
			otacode=AJMS_ERR_OTA_USER_CANCEL;//902
       		break;
		default:
			otacode = -1;//the flag of not send ota notify
			break;
	}
	
	 if(0 !=strlen((char*)s_local_check_result.installNotifyUrl) && (-1 != otacode))
     {
    		
    	    if(jwe_ota_notify(otacode, 
    					    (char*) s_local_check_result.installNotifyUrl,
    					    strlen((char*)s_local_check_result.installNotifyUrl)
    					    ))
            {
                s_ota_notify_is_success= FALSE;
            }
    }
     
	if(MMK_IsOpenWin(MMIJAVA_INSTALL_WIN_ID))
    {
       MMK_CloseWin( MMIJAVA_INSTALL_WIN_ID );  
    }	
     if(MMK_IsOpenWin(MMIJAVA_CHECK_WIN_ID))
    {
       	MMK_CloseWin( MMIJAVA_CHECK_WIN_ID );  
	}
}
#endif
/*****************************************************************************/
//  Description :MMIJAVA_OTADeleteDLFile
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIJAVA_OTADeleteDLFile(void)
{
	MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
	int ret;
	wchar OTA_JAD_Temp_FileName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	wchar OTA_JAR_Temp_FileName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};	
	return_value = MMI_ReadNVItem(MMINV_OTA_TEMP_JADFILE_INFO,OTA_JAD_Temp_FileName);
	return_value = MMI_ReadNVItem(MMINV_OTA_TEMP_JARFILE_INFO,OTA_JAR_Temp_FileName);

	ret=MMIAPIFMM_DeleteFile(OTA_JAD_Temp_FileName,PNULL);
	ret=MMIAPIFMM_DeleteFile(OTA_JAR_Temp_FileName,PNULL);	
	
}

/*****************************************************************************/
//  Description :MMIJAVA_OTASaveJavaFileInfo
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJAVA_OTASaveJavaFileInfo(MMIBROWSER_MIME_TYPE_E type)
{
	if(MMIBROWSER_MIME_TYPE_JAD==type)
	{
		if(OTA_DELETE_JAD==1 &&PNULL != OTA_JAD_FileName_ptr)
	    {
		    MMI_WriteNVItem(MMINV_OTA_TEMP_JADFILE_INFO, OTA_JAD_FileName_ptr);
	    }
	}
    
	if(MMIBROWSER_MIME_TYPE_JAR==type)
	{
		if(OTA_DELETE_JAR==1&&PNULL != OTA_JAR_FileName_ptr)
	    {
		    MMI_WriteNVItem(MMINV_OTA_TEMP_JARFILE_INFO, OTA_JAR_FileName_ptr);
        }
	}
    return TRUE;
}


/*****************************************************************************/
//  Description :MMIJAVA_OTADeleteFile
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIJAVA_OTADeleteFile(void)
{
    int result=FALSE;
	//SCI_TRACE_LOW:"OTA_DELETE_JAD=%d OTA_DELETE_JAR=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9176_112_2_18_2_22_27_223,(uint8*)"dd",OTA_DELETE_JAD,OTA_DELETE_JAR);

    if(OTA_DELETE_JAD==1 && PNULL != OTA_JAD_FileName_ptr)
	{
		int fileNameLen = 0;
		int i;
		int ret;
		ret=MMIAPIFMM_DeleteFile(OTA_JAD_FileName_ptr,PNULL);
		fileNameLen = MMIAPICOM_Wstrlen(OTA_JAD_FileName_ptr);
	    for (i=0; i<fileNameLen; i++) 
        {
			OTA_JAD_FileName_ptr[i]=0;
		}
		//SCI_TRACE_LOW:"OTA_DELETE_JAD ret=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9189_112_2_18_2_22_27_224,(uint8*)"d",ret);
		result=TRUE;
	}
    
	if(OTA_DELETE_JAR==1 && PNULL != OTA_JAR_FileName_ptr)
	{
		int fileNameLen = 0;
		int i;
		int ret;
		ret =MMIAPIFMM_DeleteFile(OTA_JAR_FileName_ptr,PNULL);
		fileNameLen = MMIAPICOM_Wstrlen(OTA_JAR_FileName_ptr);
	    for (i=0; i<fileNameLen; i++)
        {
			OTA_JAR_FileName_ptr[i]=0;
		}
		//SCI_TRACE_LOW:"OTA_JAR_FileName_ptr ret=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9204_112_2_18_2_22_27_225,(uint8*)"d",ret);
		result=TRUE;
	}

    return result;
}

/*****************************************************************************/
//  Description :SetSelectNetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIJAVA_Download_Init(void)
{

    MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MN_RETURN_RESULT_E              nv_result = MN_RETURN_FAILURE;
    // wchar                           wchar_url[JAVA_DOWNLOAD_DEFAULT_URL_LEN+1] = {0};

    MMINV_READ(MMINV_JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
    
    if(download_detail.game_info.url_len == 0)
    {
        download_detail.game_info.url_len = JAVA_DOWNLOAD_DEFAULT_URL_LEN;
        SCI_MEMCPY(download_detail.game_info.url,s_default_download_url,JAVA_DOWNLOAD_DEFAULT_URL_LEN+1);
    }
    
    if(download_detail.ebook_info.url_len == 0)
    {
        download_detail.ebook_info.url_len = JAVA_DOWNLOAD_DEFAULT_URL_LEN;
        SCI_MEMCPY(download_detail.ebook_info.url,s_default_download_url,JAVA_DOWNLOAD_DEFAULT_URL_LEN+1);
    }
    
    if(download_detail.other_info.url_len == 0)
    {
        download_detail.other_info.url_len = JAVA_DOWNLOAD_DEFAULT_URL_LEN;
        SCI_MEMCPY(download_detail.other_info.url,s_default_download_url,JAVA_DOWNLOAD_DEFAULT_URL_LEN+1);
    }
    
    MMINV_WRITE(MMINV_JAVA_DOWNLOAD_INFO, &download_detail);

}

/*****************************************************************************/
//  Description : GameBottonProc
//  Global resource dependence : none
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E GameBottonProc(void)
{
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : EbookBottonProc
//  Global resource dependence : none
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E EbookBottonProc(void)
{
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : OtherBottonProc
//  Global resource dependence : none
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E OtherBottonProc(void)
{
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description :SetSelectNetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetJavaDownLoadFormParam(MMI_WIN_ID_T	win_id)
{
    GUIFORM_CHILD_WIDTH_T           width = {0};
    uint16                          image_width = 0;
    uint16                          image_height = 0;
    MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MN_RETURN_RESULT_E              nv_result = MN_RETURN_FAILURE;
    wchar                           wchar_url[MMIJAVA_MAX_DOWNLOAD_URL_LEN+1] = {0};
    GUIIM_TYPE_T                    allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T                    init_im = GUIIM_TYPE_NONE;

    GUIFORM_SetStyle(MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID,GUIFORM_STYLE_UNIT);

    //设置botton rect
	GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_DOWNLOAD_ENTER,win_id);
    width.type = GUIFORM_CHILD_WIDTH_FIXED;
    width.add_data = image_width+4;//info add 4,because of botton border
    
    GUIFORM_SetChildWidth(MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID,MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID,&width);
    GUIFORM_SetChildWidth(MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID,MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID,&width);
    GUIFORM_SetChildWidth(MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID,MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID,&width);

    GUILABEL_SetTextById(MMIJAVA_DOWNLOAD_SET_GAME_LABEL_ID,TXT_JAVA_DOWNLOAD_GAME_URL,FALSE);
    GUILABEL_SetTextById(MMIJAVA_DOWNLOAD_SET_EBOOK_LABEL_ID,TXT_JAVA_DOWNLOAD_EBOOK_URL,FALSE);
    GUILABEL_SetTextById(MMIJAVA_DOWNLOAD_SET_OTHER_LABEL_ID,TXT_JAVA_DOWNLOAD_OTHER_URL,FALSE);

    GUIEDIT_SetFont(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID,MMI_DEFAULT_NORMAL_FONT);
    GUIEDIT_SetFont(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID,MMI_DEFAULT_NORMAL_FONT);
    GUIEDIT_SetFont(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID,MMI_DEFAULT_NORMAL_FONT);

    //设置初始url
    MMINV_READ(MMINV_JAVA_DOWNLOAD_INFO, &download_detail, nv_result);
    if(download_detail.game_info.url_len > 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)download_detail.game_info.url, wchar_url);    
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, wchar_url, download_detail.game_info.url_len);
    }
    
    if(download_detail.ebook_info.url_len > 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)download_detail.ebook_info.url, wchar_url);    
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, wchar_url, download_detail.ebook_info.url_len);
    }
    
    if(download_detail.other_info.url_len > 0)
    {
        MMIAPICOM_StrToWstr((uint8 *)download_detail.other_info.url, wchar_url);    
        GUIEDIT_SetString(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, wchar_url, download_detail.other_info.url_len);
    }
    
    //设置url text edit im
    allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
    init_im = GUIIM_TYPE_ABC;
    
    GUIEDIT_SetIm(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, allow_im, init_im);
    GUIEDIT_SetIm(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, allow_im, init_im);
    GUIEDIT_SetIm(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, allow_im, init_im);

    //set active ctrl
    MMK_SetAtvCtrl(win_id, MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID);
}



/*****************************************************************************/
//  Description :IfJavaDownLoadSetInfoChanged
//  Global resource dependence : none
//  Author: mary,xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E IfJavaDownLoadSetInfoChanged(void)
{
    MMI_RESULT_E                    result = MMI_RESULT_FALSE;
	MN_RETURN_RESULT_E              return_value = MN_RETURN_FAILURE;
    char                            char_url[MMIJAVA_MAX_DOWNLOAD_URL_LEN+1] = {0};
    MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MMI_STRING_T                    url_wstr = {0};
    
    MMINV_READ(MMINV_JAVA_DOWNLOAD_INFO, &download_detail, return_value);
	if (MN_RETURN_SUCCESS != return_value)
        return  (MMI_RESULT_TRUE);

   	GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, &url_wstr);
	SCI_MEMSET(char_url,0,sizeof(char_url));
    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
    
    if(SCI_MEMCMP(download_detail.game_info.url,char_url, MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN)))
  	{
  		return MMI_RESULT_TRUE;
  	}

    GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, &url_wstr);
	SCI_MEMSET(char_url,0,sizeof(char_url));
    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
    if(SCI_MEMCMP(download_detail.ebook_info.url,char_url, MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN)))
  	{
  		return MMI_RESULT_TRUE;
  	}

    GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, &url_wstr);
	SCI_MEMSET(char_url,0,sizeof(char_url));
    MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
    if(SCI_MEMCMP(download_detail.other_info.url,char_url, MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN)))
  	{
  		return  MMI_RESULT_TRUE;
  	}    

    return (result);  	
}
/**************************************************************************************************/
// Description:HandleJavaDownLoadSetWinMsg
// Author:murphy.xie
// Date:2010.11.22
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaDownLoadSetWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{

    MMI_RESULT_E                    result = MMI_RESULT_TRUE;
	MMI_STRING_T                    url_wstr = {0};
    char                            char_url[MMIJAVA_MAX_DOWNLOAD_URL_LEN+1] = {0};
    MMIJAVA_DOWNLOAD_DETAIL_T       download_detail = {0};
    MMIBROWSER_ENTRY_PARAM          brw_parm = {0};
    MMI_CTRL_ID_T                   active_id = 0;
	uint16	                        min_url_len = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetJavaDownLoadFormParam(win_id);
        break;      
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        active_id = MMK_GetActiveCtrlId(win_id);
        if((MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID == active_id)
        ||(MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID == active_id)
        ||(MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID == active_id))
        {
            GUIEDIT_GetString(active_id - 2, &url_wstr);//-2 for get input url control id
            if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
            {
                MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                brw_parm.type = MMIBROWSER_ACCESS_URL;
                brw_parm.dual_sys = MN_DUAL_SYS_MAX;     
                brw_parm.is_connection_customized = FALSE;  
                brw_parm.url_ptr = char_url;
        	    MMIAPIBROWSER_Entry(&brw_parm);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
            }
        }
        else
        {
            GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID, &url_wstr);
            if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
            {
                min_url_len = MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN);
                MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                SCI_MEMCPY(download_detail.game_info.url,char_url,min_url_len);
                download_detail.game_info.url_len = min_url_len;
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
                break;
            }


            GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID, &url_wstr);
            if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
            {
                SCI_MEMSET(char_url,0,sizeof(char_url));
                min_url_len = MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN);
                MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                SCI_MEMCPY(download_detail.ebook_info.url,char_url,min_url_len);
                download_detail.ebook_info.url_len = min_url_len;
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
                break;
            }
            
            GUIEDIT_GetString(MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID, &url_wstr);
            if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
            {
                SCI_MEMSET(char_url,0,sizeof(char_url));
                min_url_len = MIN(url_wstr.wstr_len,MMIJAVA_MAX_DOWNLOAD_URL_LEN);
                MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                SCI_MEMCPY(download_detail.other_info.url,char_url,min_url_len);
                download_detail.other_info.url_len = min_url_len;
            }    
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
                break;
            }
            
            MMINV_WRITE(MMINV_JAVA_DOWNLOAD_INFO, &download_detail);
          //  MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
          MMK_CloseWin(win_id);
        }

        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        active_id = ((MMI_NOTIFY_T*)param)->src_id ;
        if((MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID == active_id)
            ||(MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID == active_id)
            ||(MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID == active_id))
        {
            GUIEDIT_GetString(active_id - 2, &url_wstr);//-2 for get input url control id
            if(url_wstr.wstr_len > 0 && (PNULL != url_wstr.wstr_ptr))
            {
                MMIAPICOM_WstrToStr(url_wstr.wstr_ptr,(uint8 *)char_url);
                brw_parm.type = MMIBROWSER_ACCESS_URL;
                brw_parm.dual_sys = MN_DUAL_SYS_MAX;     
                brw_parm.is_connection_customized = FALSE;  
                brw_parm.url_ptr = char_url;
                MMIAPIBROWSER_Entry(&brw_parm);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_INVALID_URL);
            }
        }
        #ifdef MMI_PDA_SUPPORT
        else
        {
            switch (active_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        #endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
    		if(IfJavaDownLoadSetInfoChanged())
    		{
				MMIPUB_OpenQueryWinByTextId(STXT_EXIT, IMAGE_PUBWIN_QUERY, PNULL, PNULL);			
    		}
    		else
    		{
    		    MMK_CloseWin(win_id);	
			}
        break;

		case MSG_PROMPTWIN_OK: 
			MMIPUB_CloseQuerytWin(PNULL);
			MMK_CloseWin(win_id);
			break;
			
	 case MSG_PROMPTWIN_CANCEL: 
			MMIPUB_CloseQuerytWin(PNULL);
			break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : MMIAPIJAVA_IsJavaRuning
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:jvm if running
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsJavaInstalling( void )
{
    BOOLEAN result = FALSE;
    
    if(MMK_IsOpenWin(MMIJAVA_CHECK_WIN_ID))
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"MMIJAVA_IsJavaRuning result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9567_112_2_18_2_22_28_226,(uint8*)"d",result);
    return result;
}

/**************************************************************************************************/
// Description:MMIJAVA_OTAInstallJad_CallBackFunction
// Author:murphy.xie
// Date:2010.11.22
// Note:OTA  下载注册回调函数--JAD
//*************************************************************************************************/
LOCAL void MMIJAVA_OTAInstallJad_CallBackFunction(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{
    wchar                   fileName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0,};
    int                     fileNameLen = 0;
    int                     i;
    uint16                  url_len = 0;
    
    //SCI_TRACE_LOW:"MMIJAVA_OTAInstallJad_CallBackFunction enter "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9583_112_2_18_2_22_28_227,(uint8*)"");
    if(MMIAPIJAVA_IsJavaInstalling())
	{
        MMIAPIFMM_DeleteFile(param_ptr->file_name, PNULL);
        //maybe some prompt
        //SCI_TRACE_LOW:"MMIJAVA_OTAInstallJad_CallBackFunction,MMIAPIJAVA_IsJavaInstalling true exit "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9588_112_2_18_2_22_28_228,(uint8*)"");
        return;
	}
    
    if(param_ptr->download_result)
    {
    	OTA_DELETE_JAD=1;
    	
        MMIAPICOM_Wstrcpy(fileName, param_ptr->file_name);
        if(OTA_JAD_FileName_ptr != PNULL)
        {
            return;
        }
        
        OTA_JAD_FileName_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
        if(OTA_JAD_FileName_ptr == PNULL)
        {
           return;
        }

        SCI_MEMSET(OTA_JAD_FileName_ptr,0,sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
        MMIAPICOM_Wstrcpy(OTA_JAD_FileName_ptr, param_ptr->file_name);
    	
        fileNameLen = MMIAPICOM_Wstrlen(fileName);

        for (i=0; i<fileNameLen; i++)
         {
            if (fileName[i] == '/') 
            {
                fileName[i] = '\\';
            }
            if (OTA_JAD_FileName_ptr[i] == '/') 
            {
    	     OTA_JAD_FileName_ptr[i]='\\';	
            }		
        }
        
        //MMIJAVA_OTAInstallJad(fileName,param_ptr->mime_type, (unsigned char*)param_ptr->url_ptr);
        
    	MMIJAVA_OTASaveJavaFileInfo(param_ptr->mime_type);
        
        if(g_jvm_is_running_by_mmi)
        {
            JWE_LOG(MMIJAVA_OTAInstallJad_CallBackFunction,("g_jvm_is_running_by_mmi==TRUE (g_b_needContinueLocalInstall=%d s_needContinueOTAInstall.is_needContinueOTAInstall=%d) -> HandleJavaQueryOTAInstallInRunningWinMsg",g_b_needContinueLocalInstall,s_needContinueOTAInstall.is_needContinueOTAInstall));        
            if((!g_b_needContinueLocalInstall) && (!s_needContinueOTAInstall.is_needContinueOTAInstall)){
                JWE_LOG(MMIJAVA_OTAInstallJad_CallBackFunction,("No previous install step"));   
                SCI_MEMSET(s_needContinueOTAInstall.fileName,0,sizeof(s_needContinueOTAInstall.fileName));
                MMIAPICOM_Wstrcpy(s_needContinueOTAInstall.fileName, fileName);
                if(g_wchar_ContinueOTAInstall_jad_backup_path_ptr != PNULL)
                {
                    SCI_FREE(OTA_JAD_FileName_ptr);
                    return;  
                }
                
                g_wchar_ContinueOTAInstall_jad_backup_path_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
                if(g_wchar_ContinueOTAInstall_jad_backup_path_ptr == PNULL)
                {
                    SCI_FREE(OTA_JAD_FileName_ptr);
                    return; 
                }
                SCI_MEMSET(g_wchar_ContinueOTAInstall_jad_backup_path_ptr,0,sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
                MMIAPICOM_Wstrcpy(g_wchar_ContinueOTAInstall_jad_backup_path_ptr, fileName);    
                s_needContinueOTAInstall.mime_type = param_ptr->mime_type;
 
                url_len = SCI_STRLEN(param_ptr->url_ptr);
                s_needContinueOTAInstall.url  = SCI_ALLOCA(url_len+1);
                
                if(PNULL == s_needContinueOTAInstall.url)
                {
                    SCI_FREE(OTA_JAD_FileName_ptr);
                    SCI_FREE(g_wchar_ContinueOTAInstall_jad_backup_path_ptr);
                    return;
                }
                SCI_MEMSET(s_needContinueOTAInstall.url,0,url_len+1);
    			SCI_STRCPY(s_needContinueOTAInstall.url,param_ptr->url_ptr);
            
                MMIPUB_OpenQueryWinByTextId( TXT_JAVA_INSTALL_IN_RUNNING, IMAGE_PUBWIN_QUERY,PNULL,HandleJavaQueryOTAInstallJadInRunningWinMsg );               
            }
            else    
            {
                JWE_LOG(MMIJAVA_OTAInstallJad_CallBackFunction,("Has a previous install step"));        
                MMIPUB_OpenAlertFailWin( TXT_JAVA_PREVIOUS_INSTALL_NOT_COMPLETED); 
            }
        }
        else
        {
            JWE_LOG(MMIJAVA_OTAInstallJad_CallBackFunction,("g_jvm_is_running_by_mmi==FALSE -> MMIJAVA_OTAInstallJad"));        
            MMIJAVA_OTAInstallJad(fileName,param_ptr->mime_type,param_ptr->url_ptr);
        }

    }
    else
    {
#ifdef BROWSER_SUPPORT_NETFRONT
    	MMIJAVA_OTADownloadFailHandle(param_ptr->result_code);
#endif
        MMIAPIJAVA_StopOTAInstall();
    }

}

/**************************************************************************************************/
// Description:MMIJAVA_OTAInstallJar_CallBackFunction
// Author:murphy.xie
// Date:2010.11.22
// Note:OTA  下载注册回调函数--JAR
//*************************************************************************************************/
LOCAL void MMIJAVA_OTAInstallJar_CallBackFunction(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{

    wchar                   fileName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0,};
    int                     fileNameLen = 0;
    int                     i;
    uint16                  url_len = 0;
    
    if((PNULL!= param_ptr->url_ptr) && (0 != strcmp(param_ptr->url_ptr,(char *)s_local_check_result.jarUrl)))
    {
        if(MMIAPIJAVA_IsJavaInstalling())
    	{
            MMIAPIFMM_DeleteFile(param_ptr->file_name, PNULL);
            //maybe some prompt
            //SCI_TRACE_LOW:"MMIJAVA_OTAInstallJar_CallBackFunction,MMIAPIJAVA_IsJavaInstalling true exit "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9708_112_2_18_2_22_28_229,(uint8*)"");
            return;
    	}
    }
    
    if(param_ptr->download_result)
    {
    	OTA_DELETE_JAR=1;
        if(OTA_JAR_FileName_ptr != PNULL)
        {
            return;
        }
        
        OTA_JAR_FileName_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));
        if(OTA_JAR_FileName_ptr == PNULL)
        {
           return;
        }
        MMIAPICOM_Wstrcpy(fileName, param_ptr->file_name);
    	MMIAPICOM_Wstrcpy(OTA_JAR_FileName_ptr, param_ptr->file_name);
        fileNameLen = MMIAPICOM_Wstrlen(fileName);
        for (i=0; i<fileNameLen; i++) 
        {
            if (fileName[i] == '/')
            {
                fileName[i] = '\\';
            }
            
            if (OTA_JAR_FileName_ptr[i] == '/')
            {
    	        OTA_JAR_FileName_ptr[i]='\\';	
            }			
        }

        
    	MMIJAVA_OTASaveJavaFileInfo(param_ptr->mime_type);
    	if(g_jvm_is_running_by_mmi)
    	{
    		JWE_LOG(MMIJAVA_OTAInstallJar_CallBackFunction,("g_jvm_is_running_by_mmi==TRUE (g_b_needContinueLocalInstall=%d s_needContinueOTAInstall.needContinueOTAInstall=%d) -> HandleJavaQueryOTAInstallInRunningWinMsg",g_b_needContinueLocalInstall,s_needContinueOTAInstall.is_needContinueOTAInstall));		
    		if((!g_b_needContinueLocalInstall) && (!s_needContinueOTAInstall.is_needContinueOTAInstall))
            {
    			JWE_LOG(MMIJAVA_OTAInstallJar_CallBackFunction,("No previous install step"));	
    			SCI_MEMSET(s_needContinueOTAInstall.fileName,0,sizeof(s_needContinueOTAInstall.fileName));
    			MMIAPICOM_Wstrcpy(s_needContinueOTAInstall.fileName, fileName);
    			s_needContinueOTAInstall.mime_type = param_ptr->mime_type;

                url_len = SCI_STRLEN(param_ptr->url_ptr);
                s_needContinueOTAInstall.url  = SCI_ALLOCA(url_len+1);
                
                if(PNULL == s_needContinueOTAInstall.url)
                {
                    SCI_FREE(OTA_JAR_FileName_ptr);
                    return;
                }
                
                SCI_MEMSET(s_needContinueOTAInstall.url,0,url_len+1);
    			SCI_STRCPY(s_needContinueOTAInstall.url,param_ptr->url_ptr);
                
    			MMIPUB_OpenQueryWinByTextId( TXT_JAVA_INSTALL_IN_RUNNING, IMAGE_PUBWIN_QUERY,PNULL,HandleJavaQueryOTAInstallJarInRunningWinMsg );         		
    		}
    		else	
            {
    			JWE_LOG(MMIJAVA_OTAInstallJar_CallBackFunction,("Has a previous install step"));		
    			MMIPUB_OpenAlertFailWin( TXT_JAVA_PREVIOUS_INSTALL_NOT_COMPLETED); 
    		}
    	}
    	else
    	{
    		JWE_LOG(MMIJAVA_OTAInstallJar_CallBackFunction,("g_jvm_is_running_by_mmi==FALSE -> MMIJAVA_OTAInstallJar"));
    		MMIJAVA_OTAInstallJar(fileName,param_ptr->mime_type, param_ptr->url_ptr);
    	}
    }
    else
    {
#ifdef BROWSER_SUPPORT_NETFRONT
    	MMIJAVA_OTADownloadFailHandle(param_ptr->result_code);    
#endif
        MMIAPIJAVA_StopOTAInstall();      
    }

}

/**************************************************************************************************/
// Description:MMIJAVA_OTAInstallJad
// Author:murphy.xie
// Date:2010.11.22
// Note:OTA 下载安装，准备工作
//*************************************************************************************************/
PUBLIC void MMIJAVA_OTAInstallJad( wchar * jadName, uint32  mime_type,char *url)
{
	uint8*      JadData = PNULL;
    uint32      jadSize = 0;
    
    JWE_LOG(MMIJAVA_OTAInstallJad,("jarname,mime_typem,url = %s,%d,%s",jadName,mime_type,url));	
    #ifndef JAVA_INSTALL_FILE_READER
    uint32          readedsize = 0;
    SFS_HANDLE      filehandle = 0;
    filehandle = MMIAPIFMM_CreateFile(jadName,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
    if(filehandle == 0)
    {
        //SCI_TRACE_LOW:"MMIJAVA_OTAInstallJad  11"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_9807_112_2_18_2_22_29_230,(uint8*)"");
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return ;
    }    
    jadSize = MMIAPIFMM_GetFileSize(filehandle);      
    
    if( jadSize == 0)
    {        
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        MMIAPIFMM_CloseFile(filehandle);
        return ;
    }
    
    JadData = (char *)SCI_ALLOC_APP(sizeof(char)*jadSize +1);
    
    SCI_MEMSET(JadData,0,(sizeof(char)*jadSize +1));/*lint !e831*/
    if(MMIAPIFMM_ReadFile(filehandle,JadData,jadSize,&readedsize,0) != SFS_NO_ERROR)
    {
	    if( JadData != NULL ) /*lint !e774 !e831*/
        {
            SCI_FREE( JadData );
            JadData = NULL;
        }    
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        MMIAPIFMM_CloseFile(filehandle);
        return ;
    }    
    
    if(MMIAPIFMM_CloseFile(filehandle) != SFS_NO_ERROR)
    {
	    if( JadData != NULL )/*lint !e774 !e831*/
        {
            SCI_FREE( JadData );
            JadData = NULL;
        }
   
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return ;
    }
    #endif
    MMIJAVA_OTAInstall(jadName,JadData,jadSize,(int8 *)url,TRUE);
}

/**************************************************************************************************/
// Description:MMIJAVA_OTAInstallJar
// Author:murphy.xie
// Date:2010.11.22
// Note:OTA 下载安装，准备工作
//*************************************************************************************************/
PUBLIC void MMIJAVA_OTAInstallJar(wchar *jarName, uint32  mime_type,char *url)
{
	uint8*          JarData = PNULL;
    uint32          jarSize = 0;
    
    JWE_LOG(MMIJAVA_OTAInstallJar,("jarname,mime_typem,url = %s,%d,%s",jarName,mime_type,url));	
    #ifndef JAVA_INSTALL_FILE_READER
    uint32          readedsize = 0;
    SFS_HANDLE      filehandle = 0;
    filehandle = MMIAPIFMM_CreateFile(jarName,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
    if(filehandle == 0)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return ;
    }    
    jarSize = MMIAPIFMM_GetFileSize(filehandle);      
    
    if( jarSize == 0)
    {        
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        MMIAPIFMM_CloseFile(filehandle);
        return ;
    }
    JarData = (char *)SCI_ALLOC_APP(sizeof(char)*jarSize +1);
    
    SCI_MEMSET(JarData,0,(sizeof(char)*jarSize +1)); /*lint !e831*/
    if(MMIAPIFMM_ReadFile(filehandle,JarData,jarSize,&readedsize,0) != SFS_NO_ERROR)
    {
	    if( JarData != NULL )/*lint !e774 !e831*/
        {
            SCI_FREE( JarData );
            JarData = NULL;
        }    
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        MMIAPIFMM_CloseFile(filehandle);
        return ;
    }    
    
    if(MMIAPIFMM_CloseFile(filehandle) != SFS_NO_ERROR)
    {
	    if( JarData != NULL )/*lint !e774 !e831*/
        {
            SCI_FREE( JarData );
            JarData = NULL;
        }
   
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return ;
    }
    #endif
    MMIJAVA_OTAInstall(jarName,JarData,jarSize,url,FALSE);
}

/****************************************************************************
*函数定义:MMIJAVA_OTAInstall
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
PUBLIC void MMIJAVA_OTAInstall(wchar * fileName,uint8 *jaddata, int32 jadlen, int8 *url, BOOLEAN is_jad)
{
    MMIJAVA_JAR_FILE_INFO_T file_info = {0};   
    BOOLEAN                 is_open_check = FALSE;
    uint16                  url_len = 0;
    int32                   i = 0;
    MMI_JAVA_OPEN_RESULT_E  open_result = MMI_JAVA_OPEN_RESULT_TRUE;
    #if 0
    if(is_jad && PNULL != s_install_data_info_ptr&&s_install_data_info_ptr->is_ota)
	{
		//取消上一次的安装
        MMK_CloseWin(MMIJAVA_INSTALL_WIN_ID);
	}
	#endif
    #ifndef JAVA_INSTALL_FILE_READER
    file_info.file_content_ptr = SCI_ALLOC_APP(jadlen+1);  
    SCI_MEMSET(file_info.file_content_ptr,0,(jadlen+1));
    file_info.file_len = jadlen;
    SCI_MEMCPY(file_info.file_content_ptr,jaddata,jadlen);
    //what is the meaning,to do some change in future
    if(jaddata != PNULL)
    {
        JWE_LOG(MMIJAVA_OTAInstall,("jaddata copy to file_info.file_content_ptr,jaddata should free  AND  is_jad = %d",is_jad));		
        SCI_FREE(jaddata);
    }
    #endif

    g_java_wap_info.is_start_wap = FALSE;
    JWE_LOG(MMIJAVA_OTAInstall,("url = %s,s_install_data_info_ptr = %x",url,s_install_data_info_ptr));	

    if(!s_ota_notify_is_success)
    {
        MMIPUB_OpenAlertWarningWin(TXT_JAVA_OTA_NOTIFY_SENDING);
        return;
    }
    
    if ((PNULL != s_install_data_info_ptr)&&s_install_data_info_ptr->is_ota)
    {
        is_open_check = FALSE;

        #ifdef JAVA_INSTALL_FILE_READER
        open_result =  OpenJavaFile(fileName, &file_info);
        if (MMI_JAVA_OPEN_RESULT_TRUE != open_result )
        {

            MMIPUB_OpenAlertWarningWin(TXT_COMM_INSTALL_FAILED);

            JWE_LOG(MMIJAVA_OTAInstall,("OpenJavaFile fail 1"));
  
    	    #ifdef JAVA_DM_SUPPORT
    		DM_JAVAHandleScomoCallback(0);
    	    #endif	

            ReleaseInstallDataInfo();            
            MMK_CloseWin(MMIJAVA_INSTALL_WIN_ID);
            return; 
        }    
        #endif
        if(file_info.file_len != (uint32)s_local_check_result.jarSize)
        {
            JWE_LOG(MMIJAVA_OTAInstall,("MMIJAVA_WapCheck jar_file_info.file_len = %d,s_local_check_result.jarSize = %d",file_info.file_len,s_local_check_result.jarSize));		
            if(s_install_data_info_ptr->is_ota)
		    {
			    if(0 !=strlen((char*)s_local_check_result.installNotifyUrl))
                {
					
				    if(jwe_ota_notify(s_local_check_result.otaCode, 
								    (char*) s_local_check_result.installNotifyUrl,
								    strlen((char*)s_local_check_result.installNotifyUrl)
								    ))
                    {
                        s_ota_notify_is_success= FALSE;
                    }
			    }									
		    }     
            MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_COMM_INSTALL_FAILED, 
                TXT_NULL, 
                IMAGE_PUBWIN_FAIL, 
                PNULL, 
                PNULL,
                MMIPUB_SOFTKEY_ONE,
                PNULL);  
            
            ReleaseInstallDataInfo();
            MMK_CloseWin(MMIJAVA_INSTALL_WIN_ID);
            return;
        }

    }
    else
    {    
        s_install_data_info_ptr = SCI_ALLOCA(sizeof(MMIJAVA_INSTALL_DATA_INFO_T));
        if(PNULL == s_install_data_info_ptr)
        {
            JWE_LOG(MMIJAVA_OTAInstall,("s_install_data_info_ptr alloc error"));
            MMIPUB_OpenAlertWarningWin(TXT_COMM_INSTALL_FAILED);
    	#ifdef JAVA_DM_SUPPORT
    		DM_JAVAHandleScomoCallback(0);
    	#endif			
            return; 
        }
        
        SCI_MEMSET(s_install_data_info_ptr,0,sizeof(MMIJAVA_INSTALL_DATA_INFO_T));
        s_install_data_info_ptr->is_ota = TRUE; 
        JWE_LOG(MMIJAVA_OTAInstall,("line = %d, s_install_data_info_ptr->is_ota = %d",__LINE__,s_install_data_info_ptr->is_ota));			  
        if (PNULL != url)
        {
            url_len = strlen(url);
            JWE_LOG(MMIJAVA_OTAInstall,("line = %d, s_install_data_info_ptr->is_ota = %d",__LINE__,s_install_data_info_ptr->is_ota));		
            JWE_LOG(MMIJAVA_OTAInstall,("apple ots url_len = %d",url_len));	
            s_install_data_info_ptr->url = SCI_ALLOC_APP(url_len+1);
            SCI_MEMSET(s_install_data_info_ptr->url,0,url_len+1);
            JWE_LOG(MMIJAVA_OTAInstall,("line = %d, s_install_data_info_ptr->is_ota = %d",__LINE__,s_install_data_info_ptr->is_ota));	
            for (i = 0; i < url_len; i++) 
            {
                s_install_data_info_ptr->url[i] = url[i];
            }
            s_install_data_info_ptr->url[i] = 0;
            JWE_LOG(MMIJAVA_OTAInstall,("line = %d, s_install_data_info_ptr->is_ota = %d",__LINE__,s_install_data_info_ptr->is_ota));		
        }
        #ifdef JAVA_INSTALL_FILE_READER
        open_result =  OpenJavaFile(fileName, &file_info);
        if (MMI_JAVA_OPEN_RESULT_TRUE != open_result )
        {

            MMIPUB_OpenAlertWarningWin(TXT_COMM_INSTALL_FAILED);

            JWE_LOG(MMIJAVA_OTAInstall,("OpenJavaFile fail 2"));
    	#ifdef JAVA_DM_SUPPORT
    		DM_JAVAHandleScomoCallback(0);
    	#endif		
            ReleaseInstallDataInfo();
            return; 
        }    
        #endif
        is_open_check = TRUE;
        
    }
    
    JWE_LOG(MMIJAVA_OTAInstall,("is_jad = %d",is_jad));
    
    if (is_jad)
    {
        s_install_data_info_ptr->jad_len = file_info.file_len;
        #ifdef JAVA_INSTALL_FILE_READER
        s_install_data_info_ptr->jad_file_handler= file_info.file_handler;
        #else
        s_install_data_info_ptr->jad_ptr = file_info.file_content_ptr;
        #endif
        //for crNEWMS00108329
        MMIJAVA_OTASaveJavaFileInfo(MMIBROWSER_MIME_TYPE_JAD);
    }
    else
    {  
        s_install_data_info_ptr->jar_len = file_info.file_len;
        #ifdef JAVA_INSTALL_FILE_READER
        s_install_data_info_ptr->jar_file_handler = file_info.file_handler;
        #else
        s_install_data_info_ptr->jar_ptr = file_info.file_content_ptr;
        #endif
        //for crNEWMS00108329
        MMIJAVA_OTASaveJavaFileInfo(MMIBROWSER_MIME_TYPE_JAR);  
        if(s_is_close_browser)
        {
            s_is_close_browser = FALSE;//reset flag
            //关闭web 
            MMIAPIBROWSER_Close();
        }
    }    
    
    JWE_LOG(MMIJAVA_OTAInstall,("jad_len=%d, s_install_data_info_ptr->is_ota = %d",s_install_data_info_ptr->jad_len,s_install_data_info_ptr->is_ota));	

    if (is_open_check)
    {   
        //初始化系统 
        Ajava_Initiliztion_Special();
	    MMIJAVA_OpenCheckWin();
    }
    else
    {
        MMK_PostMsg(MMIJAVA_INSTALL_WIN_ID,MSG_JAVA_INSTALL_START,PNULL,PNULL);
    }
    
    return;   

}

/*****************************************************************************/
//  Description : MMIJAVA_OpenCheckWin
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenCheckWin(void)
{
    if ((PNULL != s_install_data_info_ptr) && s_install_data_info_ptr->is_ota)
    {
        MMK_CreateWin( ( uint32* )MMIJAVA_CHECK_WIN_TAB, PNULL);     
    }    
}

/*****************************************************************************/
//  Description : open waiting window
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenWapWin(void)
{    
#ifdef BROWSER_SUPPORT	
	MMIBROWSER_ENTRY_PARAM brw_parm = {0};/*lint !e64*/

    //for total set
    brw_parm.type = MMIBROWSER_ACCESS_URL;
    brw_parm.user_agent_ptr = PNULL;
    
    //brw para set
    //SCI_TRACE_LOW:"hanker MMIAPIJAVA_OpenWapWin enter "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10181_112_2_18_2_22_30_231,(uint8*)"");
    
    if (MMK_IsOpenWin(MMIJAVA_CHECK_WIN_ID))
    {
        //SCI_TRACE_LOW:"MMIAPIJAVA_OpenWapWin hanker enter s_local_check_result.url=%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10185_112_2_18_2_22_30_232,(uint8*)"s",s_local_check_result.jarUrl);
        //第二次进入， 使用浏览器之前的设置
        #ifdef BROWSER_SUPPORT_NETFRONT
        brw_parm.type = MMIBROWSER_ACCESS_DOWNLOAD_BY_URL;
        #endif /* BROWSER_SUPPORT_NETFRONT */
        brw_parm.url_ptr = (char *)s_local_check_result.jarUrl;
	    MMIAPIBROWSER_Entry(&brw_parm);
    }
    else
    {
        if (PNULL != g_java_wap_info.url)
        {
            brw_parm.dual_sys = MMIJAVA_GetAcitveSim();     
            brw_parm.is_connection_customized = TRUE;
            brw_parm.connection_index = 0;

            if(MMIJAVA_GetNetworkType() == JAVA_NETWORK_TYPE_WIFI)
            {
                brw_parm.is_use_wifi =  TRUE;
            }
            
			//SCI_TRACE_LOW:"MMIAPIJAVA_OpenWapWin hanker enter g_java_wap_info.url=%s"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10206_112_2_18_2_22_30_233,(uint8*)"s",g_java_wap_info.url);
            brw_parm.url_ptr =(char *)g_java_wap_info.url;
            
            if(is_platform_request)
            {
                s_is_close_browser = TRUE;//表示启动浏览器后，要由java 模块关闭浏览器
                is_platform_request = FALSE;
            }
            //第一次调用，使用java 的网络设置
            MMIAPIBROWSER_Entry(&brw_parm);
            SCI_FREE(g_java_wap_info.url);
            g_java_wap_info.url = PNULL;            
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIJAVA_OpenWapWin url is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10221_112_2_18_2_22_30_234,(uint8*)"");
        }
       
    } 
#endif  
}
/*****************************************************************************/
//  Description : MMIAPIJAVA_StopOTAInstall
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOTAInstall(void)
{
    if (MMK_IsOpenWin(MMIJAVA_INSTALL_WIN_ID))
    {
        MMK_PostMsg(MMIJAVA_INSTALL_WIN_ID,MSG_JAVA_OTA_STOP,PNULL,PNULL);
    }
    else if (g_java_wap_info.is_start_wap)
    {
        #if 0
        MMIPUB_OpenAlertWinByTextId(PNULL, 
                            TXT_COMM_INSTALL_FAILED, 
                            TXT_NULL, 
                            IMAGE_PUBWIN_FAIL, 
                            PNULL, 
                            PNULL,
                            MMIPUB_SOFTKEY_ONE,
                            PNULL);
        #endif
        g_java_wap_info.is_start_wap = FALSE;
    }
    
    //reset flag,用户取消
    s_is_close_browser = FALSE;
}

/*****************************************************************************/
//  Description : MMIJAVA_SuiteUpdate
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIJAVA_SuiteUpdate(int32 suite_id)
{
    const AJ_U8*    jadurl = AJ_NULL;     // merlin
    AJ_S32          errorCode;
    int32           url_len = 0;
    
    //SCI_TRACE_LOW:"MMIJAVA_SuiteUpdate,suite_id =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10266_112_2_18_2_22_30_235,(uint8*)"d",suite_id);
    g_active_suite_id = suite_id;
    jadurl = ajms_ab_getJadUrl(suite_id,&errorCode);
    if(AJ_NULL == jadurl)
    {
        //SCI_TRACE_LOW:"MMIJAVA_SuiteUpdate,erorcode =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10271_112_2_18_2_22_30_236,(uint8*)"d",errorCode);
        return FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIJAVA_SuiteUpdate,jadurl =%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10276_112_2_18_2_22_30_237,(uint8*)"s",jadurl);
        url_len = strlen((char *)jadurl);
		if(0 == url_len)
		{
			return FALSE;
		}

        g_java_wap_info.is_start_wap = TRUE;
        g_java_wap_info.url = SCI_ALLOCA(url_len + 1);
        
        if(PNULL == g_java_wap_info.url)
        {
            return FALSE;
        }
        
        SCI_MEMSET(g_java_wap_info.url,0,(url_len + 1));
        SCI_MEMCPY(g_java_wap_info.url,jadurl,url_len); 
        MMIAPIJAVA_OpenWapWin(); 
        return TRUE;
    }    
    
}
#endif
/*****************************************************************************/
//  Description : set preinstall midlet suit id
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetPreinstallId(uint32 suit_id)
{
#ifdef PREINSTALL_JAVA_SUPPORT 
    s_preinstall_id = suit_id;
    MMI_WriteNVItem(MMINV_PREINSTALL_INFO, &s_preinstall_id);
#endif
}

/*****************************************************************************/
//  Description : ini preinstall midlet suit id
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_initPreinstallID(void)
{
#ifdef PREINSTALL_JAVA_SUPPORT 
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    int32                   iNumOfSuite     = 0;  
    int32                   j               = 0;
    AJMS_Suite*             suiteList_ptr = PNULL;

    iNumOfSuite = ajms_ab_getSutieListCount( -1 );
    suiteList_ptr = SCI_ALLOCA((uint32)iNumOfSuite*sizeof(AJMS_Suite));
    //SCI_TRACE_LOW:"MMIJAVA_initPreinstallID return applet = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10325_112_2_18_2_22_30_238,(uint8*)"d",iNumOfSuite);
    
    if((PNULL != suiteList_ptr)&&(iNumOfSuite > 0 && iNumOfSuite <= MAXJAVALISTSIZE))
    {
        SCI_MEMSET(suiteList_ptr, 0, (uint32)iNumOfSuite*sizeof(AJMS_Suite));
        // get the information of all java applications
        if ( ajms_ab_getSuiteList( -1 , suiteList_ptr, 0 ) >= 0 )
        {
            for ( j = 0; j < iNumOfSuite && j < MAXJAVALISTSIZE; j ++ )
            {
                if (AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED == suiteList_ptr[j].categoryId)
                {
                    MMIJAVA_SetPreinstallId(j);
                    #ifdef DYNAMIC_MAINMENU_SUPPORT
                    MMIJAVA_AddSuiteToMainmenu(suiteList_ptr[j].id,GUIMENU_DEVICE_SYSTEM);
                    #endif
                    return;
                }
            }
           
        } 
    }

    return_value = MMI_ReadNVItem(MMINV_PREINSTALL_INFO,&s_preinstall_id);

    //SCI_TRACE_LOW:"MMIJAVA_initPreinstallID s_preinstall_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10350_112_2_18_2_22_30_239,(uint8*)"d",s_preinstall_id);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIJAVA_SetPreinstallId(0);      
    }
    
    if(PNULL != suiteList_ptr)
    {
        SCI_FREE(suiteList_ptr);
    }
#else
    #ifdef DYNAMIC_MAINMENU_SUPPORT
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    int32                   j               = 0;
    AJMS_Suite*             suiteList_ptr = PNULL;
    int32                   iNumOfSuite     = 0;  
    uint32                  pre_id = 0;
    return_value = MMI_ReadNVItem(MMINV_PREINSTALL_INFO,&pre_id);
    if (MN_RETURN_SUCCESS != return_value)//只有download版本后，第一次开机才做这个动作
    {
        MMI_WriteNVItem(MMINV_PREINSTALL_INFO, &pre_id);
        
#ifdef HIDE_PREINSTALL_IN_ITEMLIST_HUAWEI
        iNumOfSuite = ajms_ab_getSutieListCount(AJMS_DEFAULT_CATEGORY_ID_UNFILED);   
#else
        iNumOfSuite = ajms_ab_getSutieListCount( -1 );   
#endif
        suiteList_ptr = SCI_ALLOCA((uint32)iNumOfSuite*sizeof(AJMS_Suite));
        //SCI_TRACE_LOW:"MMIJAVA_initPreinstallID return applet = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10378_112_2_18_2_22_30_240,(uint8*)"d",iNumOfSuite);
        
        if((PNULL != suiteList_ptr)&&(iNumOfSuite > 0 && iNumOfSuite <= MAXJAVALISTSIZE))
        {
            SCI_MEMSET(suiteList_ptr, 0, (uint32)iNumOfSuite*sizeof(AJMS_Suite));
            // get the information of all java applications
            if ( ajms_ab_getSuiteList( -1 , suiteList_ptr, 0 ) >= 0 )
            {
                for ( j = 0; j < iNumOfSuite && j < MAXJAVALISTSIZE; j ++ )
                {
                    if (AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED == suiteList_ptr[j].categoryId)
                    {
                        MMIJAVA_AddSuiteToMainmenu(suiteList_ptr[j].id,GUIMENU_DEVICE_SYSTEM);
                        return;
                    }
                }
               
            } 
        }
        
        if(PNULL != suiteList_ptr)
        {
            SCI_FREE(suiteList_ptr);
        }
    }
    #endif
#endif
}

/*****************************************************************************/
//  Description : MMIJAVA_GetNetworkTypeBySuiteID
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:get network type
/*****************************************************************************/
PUBLIC MMIJAVA_NETWORK_TYPE_E MMIJAVA_GetNetworkType(void)
{
    MMIJAVA_LINK_SETTING_T              link_setting_info = {0};
    int32                               suite_id = g_active_suite_id;
    
    link_setting_info =  MMIJAVA_GetNetSettingInfo(suite_id);
    return (MMIJAVA_NETWORK_TYPE_E)(link_setting_info.network_type);
}

/*---------------------------------------------------------------------------*/
/*                          API FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : to install form file system
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystem(
                                             wchar *fullpathname, 
                                             int fullpathname_len)
{

    MMI_RESULT_E		result	= MMI_RESULT_TRUE;
    MMIPUB_CloseQuerytWin(PNULL); 
    if( MMIAPITVOUT_GetStartingStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_TVOUT_FORBID_JAVA);
        result=MMI_RESULT_FALSE;
    }
    else 
    {
        MMIAPIJAVA_StartLocalInstall(fullpathname, fullpathname_len, 0);
    }
}
PUBLIC void MMIJAVA_SetJavaRuning(BOOLEAN  b_running)
{
    g_jvm_is_running = b_running;
}

/*****************************************************************************/
//  Description :MMIJAVA_IsDisplayLogo
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsDisplayLogo(void)
{
	return s_is_display_logo;
}

/*****************************************************************************/
//  Description : MMIJAVA_SetIsDisplayLogo
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsDisplayLogo(BOOLEAN is_display)
{
	s_is_display_logo = is_display;
}

/*****************************************************************************/
//  Description :MMIJAVA_IsStillDisplayLogo
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsStillDisplayLogo(void)
{
	return s_is_still_display_logo;
}

/*****************************************************************************/
//  Description : MMIJAVA_SetIsStillDisplayLogo
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsStillDisplayLogo(BOOLEAN is_still_display)
{
	s_is_still_display_logo = is_still_display;
}


/*****************************************************************************/
//  Description :MMIAPIJAVA_OpenMainMenu
//  Global resource dependence : none
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenMainMenu(void)
{
	AJ_S32 java_state = 0;
	
#ifdef USE_LOGFILE
	if(yuyulog_ptr!=0){
		SFS_CloseFile(yuyulog_ptr);
	}
#endif

	if(g_jvm_is_running_by_mmi==FALSE)
	{
		JWE_LOG(MMIAPIJAVA_OpenMainMenu, ("start vm in a clean environment"));
		s_b_force_killed = FALSE;
		if(MMIAPIUDISK_UdiskIsRun())
		{
			JWE_LOG(MMIAPIJAVA_OpenMainMenu, ("UDisk is used"));
			MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
			return ;
		}       
		Ajava_Initiliztion_Special();
		MMK_CreateWin((uint32 *)MMIJAVA_AMS_WIN_TAB, PNULL);
	}
	else
	{
		if(g_jvm_is_running==TRUE)
		{
			java_state = ajms_arc_getState();
            if( java_state == AJSC_VMSTATE_DESTROYING || java_state == AJSC_VMSTATE_KILLING || java_state == AJSC_VMSTATE_RESUMING)
            {
                if(!s_b_force_killed)
                {
                    JWE_LOG(MMIAPIJAVA_OpenMainMenu, ("kve not quit,and java_state is in -ing (=%d) ,require user choose force quit or wait?",java_state));
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_JAVA_FORCEQUIT,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleJavaAlertForceQuitWinMsg);
                }
                else
                {
                    JWE_LOG(MMIAPIJAVA_OpenMainMenu, ("kve not quit,and java_state is in -ing (=%d) ,note:it has been force killed! Wait->( g_jvm_is_running=FALSE && g_jvm_is_running_by_mmi=FALSE)",java_state));
                    AamsVmKill( 1 );
                    MMIPUB_OpenAlertWarningWin(TXT_JAVA_WAITQUIT);
                }	
            }
			else if(java_state == AJSC_VMSTATE_NONE)
            {
				JWE_LOG(MMIAPIJAVA_OpenMainMenu, ("kve not quit,and java_state is normal (=AJSC_VMSTATE_NONE),user will wait few second->( g_jvm_is_running=FALSE && g_jvm_is_running_by_mmi=FALSE)"));
				MMIPUB_OpenAlertWarningWin(TXT_JAVA_WAITQUIT);
			}
			else
            {
				JWE_LOG(MMIAPIJAVA_OpenMainMenu, ("kve not quit,and java_state is normal (=%d),user will enter resume dialog",java_state));
		 		MMK_CreateWin((uint32 *)MMIJAVA_RESUME_DIALOG_WIN_TAB, PNULL);
			}
		}
		else
		{
			JWE_LOG(MMIAPIJAVA_OpenMainMenu, ("kve has quit,waiting mmi release resource"));
			MMIPUB_OpenAlertWarningWin(TXT_JAVA_WAITQUIT);
		}

	}
	
}

/*****************************************************************************/
// 	Description : IsFocusJAVARunWin
//	Global resource dependence : 
//  Author:aoke.hu
//	Note:2007.8.15
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsFocusJAVARunWin(void)
{
    return MMK_IsFocusWin(MMIJAVA_MIDLET_WIN_ID);
}

/*****************************************************************************/
// 	Description : MMIAPIJAVA_IsJavaBackgroundRuning
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:jvm if running
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsJavaBackgroundRuning( void )
{
    BOOLEAN result = FALSE;
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
    if (g_jvm_in_background_by_mmi)
    {
        result = TRUE;
    }
#endif

    //SCI_TRACE_LOW:"MMIAPIJAVA_IsJavaBackgroundRuning result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10574_112_2_18_2_22_31_241,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
// 	Description : MMIAPIJAVA_IsJavaRuning
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:jvm if running
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsJavaRuning( void )
{
    BOOLEAN result = FALSE;
    
    if (g_jvm_is_running)
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"MMIJAVA_IsJavaRuning result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10591_112_2_18_2_22_31_242,(uint8*)"d",result);
    return result;
}

/**
 * \brief starts specified midlet with given suite name and vendor name.
 * @param suiteName
 *        A pointer to the UTF-8 string indicating the application suite name.
 * @param vendorName
 *        A pointer to the UTF-8 string indicating the application vendor name.
 * @return 
 *        - A result value: 
 *        - #0 non error
 *        - #-1 has error
 *        .

  
 *Author:hanker.zhou
 *Date:2010-6-30
 * The function is used to start the midlet by suitename and vendorname
 * application. According to the MIDP2.0 spec, the vendor name plus suite name
 * should be unique for each different midlet suite.
 */
int MMIAPIJAVA_StartMIDletByNameVendor( const char* suiteName, 
                                       const char* vendorName )
{
    static int32  cursuitid   = -1;
	int           result=0;
	AJ_S32        java_state = 0;
       Ajava_Initiliztion_Special();
#ifdef USE_LOGFILE
	if(yuyulog_ptr!=0)
    {
		SFS_CloseFile(yuyulog_ptr);
	}
	
#endif
	if(!g_jvm_is_running_by_mmi)
	{
		JWE_LOG(MMIJAVA_StartMIDletByNameVendor, ("start vm in a clean environment"));
		s_b_force_killed = FALSE;
		if(MMIAPIUDISK_UdiskIsRun())
		{
			JWE_LOG(MMIJAVA_StartMIDletByNameVendor, ("UDisk is used"));
			MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
			return result;
		}       
	}
	else
	{
		if(g_jvm_is_running)
		{
			java_state = ajms_arc_getState();
			if( java_state == AJSC_VMSTATE_DESTROYING || java_state == AJSC_VMSTATE_KILLING || java_state == AJSC_VMSTATE_RESUMING)
            {
				if(!s_b_force_killed)
                {
					JWE_LOG(MMIJAVA_StartMIDletByNameVendor, ("kve not quit,and java_state is in -ing (=%d) ,require user choose force quit or wait?",java_state));
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_JAVA_WAITQUIT,TXT_JAVA_FORCEQUIT,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleJavaAlertForceQuitWinMsg);
				}
				else
                {
					JWE_LOG(MMIJAVA_StartMIDletByNameVendor, ("kve not quit,and java_state is in -ing (=%d) ,note:it has been force killed! Wait->( g_jvm_is_running=FALSE && g_jvm_is_running_by_mmi=FALSE)",java_state));
					MMIPUB_OpenAlertWarningWin(TXT_JAVA_WAITQUIT);
				}	
			}
			else if(java_state == AJSC_VMSTATE_NONE)
            {
				JWE_LOG(MMIJAVA_StartMIDletByNameVendor, ("kve not quit,and java_state is normal (=AJSC_VMSTATE_NONE),user will wait few second->( g_jvm_is_running=FALSE && g_jvm_is_running_by_mmi=FALSE)"));
				MMIPUB_OpenAlertWarningWin(TXT_JAVA_WAITQUIT);
			}
			else{
				JWE_LOG(MMIJAVA_StartMIDletByNameVendor, ("kve not quit,and java_state is normal (=%d),user will enter resume dialog",java_state));
		 		MMK_CreateWin((uint32 *)MMIJAVA_RESUME_DIALOG_WIN_TAB, PNULL);
			}
		}
		else
		{
			JWE_LOG(MMIJAVA_StartMIDletByNameVendor, ("kve has quit,waiting mmi release resource"));
			MMIPUB_OpenAlertWarningWin(TXT_JAVA_WAITQUIT);
		}
        return result;

	}

	if(!MMIAPIPB_IsPbReady())
	{
        return result;
	}
	if(!s_ota_notify_is_success)
	{
		MMIPUB_OpenAlertWarningWin(TXT_JAVA_OTA_NOTIFY_SENDING);
        return result;
	}
    
    if(g_jvm_is_running)
	{
	 	MMK_CreateWin((uint32 *)MMIJAVA_RESUME_DIALOG_WIN_TAB, PNULL);
	}
	else
	{
		if(MMIAPIUDISK_UdiskIsRun())
    	{
	        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            return result;
    	}
		else
		{

            cursuitid =ajms_ab_getSuiteIdByNameVendor((AJ_U8 *) suiteName, (const AJ_U8 * )vendorName);
            if(cursuitid == AJMS_ERR_INVALID_PARAMS||cursuitid == AJMS_ERR_ID_NOT_EXIST)
            {
            	result=-1;
            	return result;
            }

            //SCI_TRACE_LOW:"MMIJAVA_StartMIDletByNameVendor cursuitid = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10704_112_2_18_2_22_31_243,(uint8*)"d",cursuitid);
            MMK_CreateWin((uint32*)MMIJAVA_AMS_MIDLETLIST_WIN_TAB, (ADD_DATA)cursuitid); 
		}
		
	}	
	return result;
}


//*********************************************************************************
//Description:JavaGameDirectStart
//Author:huangjc
//Date:2007-5-17
//Note:for java direct start
//        modify at 2007.7.1 for direct start bug
//在U盘上直接运行JAVA应用，而且应用退出时，会把安装记录删除掉
//*********************************************************************************
PUBLIC void MMIAPIJAVA_JavaGameDirectStart(const wchar *full_name,uint16 full_name_len, uint16 index)
{
    wchar               suffix[MMIJAVA_DEFAULT_SUFFIX_LEN] = {0};   
    uint16              suffix_len = MMIJAVA_DEFAULT_SUFFIX_LEN;
    wchar               jar_full_name[MMIFMM_FILE_FILE_NAME_MAX_LEN] = {0};
    wchar                   jad_full_name[MMIFMM_FILE_FILE_NAME_MAX_LEN] = {0};
    JAVA_INSTALL_INFO_T*    install_info_ptr = PNULL;

    //SCI_TRACE_LOW:"JavaGameDirectStart(): enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10729_112_2_18_2_22_31_244,(uint8*)"");
    install_info_ptr = MMIJAVA_GetIntallInfo();
    SCI_MEMSET(install_info_ptr,0,sizeof(JAVA_INSTALL_INFO_T));
    Ajava_Initiliztion_Special();

    MMIAPIFMM_SplitFullPathExt(full_name, 
    full_name_len,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    suffix, //include dot 
    &suffix_len);
    
    suffix_len = (suffix_len > MMIJAVA_DEFAULT_SUFFIX_LEN/2)? suffix_len: MMIJAVA_DEFAULT_SUFFIX_LEN/2;    

    MMIAPICOM_Wstrlower(suffix);
   
    // launch jad file
    if (0 == MMIAPICOM_Wstrncmp(suffix, MMIJAVA_JAD_SUFFIX, suffix_len))
    {
        //SCI_TRACE_LOW:"JavaGameDirectStart(): jad"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10750_112_2_18_2_22_31_245,(uint8*)"");
   		MMIAPICOM_Wstrcpy(jad_full_name,full_name);
        MMIAPICOM_Wstrcpy(jar_full_name,full_name);
        MMIAPICOM_Wstrncpy(jar_full_name + full_name_len - suffix_len , MMIJAVA_JAR_SUFFIX, suffix_len);    
        
            
        if (!MMIAPIFMM_IsFileExist(jar_full_name,full_name_len))	// only jad, can't install!
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, 
			                                 TXT_COMM_INSTALL_FAILED, 
			                                 TXT_NULL, 
			                                 IMAGE_PUBWIN_FAIL, 
			                                 PNULL, 
			                                 PNULL,
			                                 MMIPUB_SOFTKEY_ONE,
			                                 PNULL);  
            
            return; 
        }                   
        install_info_ptr->is_flag = 1;		// lanuch jad and jar file        
    }
    // lanuch jar file 
    else if (0 == MMIAPICOM_Wstrncmp(suffix, MMIJAVA_JAR_SUFFIX, suffix_len))
    {
        //SCI_TRACE_LOW:"JavaGameDirectStart(): jar"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10774_112_2_18_2_22_31_246,(uint8*)"");
        MMIAPICOM_Wstrcpy(jar_full_name,full_name);   
        install_info_ptr->is_flag = 2;		// lanuch jar file only
        
    }
    else
    {
#ifdef DRM_SUPPORT
        MMIAPICOM_Wstrcpy(jar_full_name,full_name);
        install_info_ptr->is_flag = 2;		// lanuch jar file only
 #else
        SCI_FREE(install_info_ptr);
        //SCI_TRACE_LOW:"JavaGameDirectStart: ignore this file"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10786_112_2_18_2_22_31_247,(uint8*)"");
        return;
#endif
    }
	install_info_ptr->curmidletid = 0;
    install_info_ptr->cursuitid = 0;
    
    install_info_ptr->jad_filename_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
    if(install_info_ptr->jad_filename_ptr == PNULL)
    {
        return;
    }
    
    SCI_MEMSET(install_info_ptr->jad_filename_ptr,0,sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)); 
    MMIAPICOM_Wstrcpy(install_info_ptr->jad_filename_ptr,jad_full_name);
    
    install_info_ptr->jar_filename_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
    if(install_info_ptr->jar_filename_ptr == PNULL)
    {
        SCI_FREE(install_info_ptr->jad_filename_ptr);
        return;
    }
    SCI_MEMSET(install_info_ptr->jar_filename_ptr,0,sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)); 
    MMIAPICOM_Wstrcpy(install_info_ptr->jar_filename_ptr,jar_full_name);
    MMIJAVA_OpenMidletWindow(install_info_ptr);	// start the app directly
    //SCI_TRACE_LOW:"JavaGameDirectStart(): out!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10811_112_2_18_2_22_31_248,(uint8*)"");
}


/**************************************************************************************************/
// Description:MMIJAVA_Init
// Date:2007-06-08
// Author:huangjc
// Note:JAVA 模块初始化(NV与及其它相关数据结构)
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_Init(void)
{  
#ifdef BROWSER_SUPPORT
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T jar_otacallback = {0,PNULL};/*lint !e64*/
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T jad_otacallback = {0,PNULL};/*lint !e64*/
#endif
#ifdef JAVA_SUPPORT_SHRINK_RAM
       ajms_ShrinkRamInit();
#endif
#ifdef BROWSER_SUPPORT    
	jad_otacallback.mime_type = MMIBROWSER_MIME_TYPE_JAD;
	jad_otacallback.call_back = MMIJAVA_OTAInstallJad_CallBackFunction;
	jar_otacallback.mime_type = MMIBROWSER_MIME_TYPE_JAR;
	jar_otacallback.call_back = MMIJAVA_OTAInstallJar_CallBackFunction;
	MMIAPIBROWSER_RegMimeFunc(&jar_otacallback);
	MMIAPIBROWSER_RegMimeFunc(&jad_otacallback);
#endif	
#ifdef JAVA_DM_SUPPORT
	jblendia_dm_init_id();
#endif	

#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
    MMIJAVA_Download_Init();
    MMIJAVA_OTADeleteDLFile();
#endif
    s_install_data_info_ptr = PNULL;    
}

/*****************************************************************************/
//  Description:   to pause jbed installing processing when mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Hanker.Zhou 2010/06
//  Param
//       module: [in] module tag to pause java
//  Return:
//       TRUE : paused normally.
//       FALSE: don't need pause or has been paused.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Pause(MMIJAVA_PAUSE_TYPE_E module)
{
	if(MMK_IsOpenWin(MMIJAVA_PAUSE_DIALOG_WIN_ID)||MMK_IsOpenWin(MMIJAVA_MIDLET_WIN_ID)||MMK_IsOpenWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID)||MMK_IsOpenWin(MMIJAVA_AMS_WIN_ID))
	{
        	MMIJAVA_pauseVM_external(MMIJAVA_LONG_EVENT_STATE);
	}
    
	return TRUE;
}

/*****************************************************************************/
//  Description:   to resume jbed installing processing when after mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Hanker.Zhou 2010/06
//  Param
//       module: [in] module tag to resume java
//  Return:
//       TRUE : resuming complete.
//       FALSE: don't need to resume or has been resumed.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Resume(MMIJAVA_PAUSE_TYPE_E module)
{
#if 1
	if(MMK_IsOpenWin(MMIJAVA_PAUSE_DIALOG_WIN_ID)||MMK_IsOpenWin(MMIJAVA_MIDLET_WIN_ID)||MMK_IsOpenWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID)||MMK_IsOpenWin(MMIJAVA_AMS_WIN_ID))
    	{
        	MMIJAVA_resumeVM_external();
    	}
#endif
    return TRUE;

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_ResumeBackgroundJVM( void )
{
    #ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
    //SCI_TRACE_LOW:"MMIAPIJAVA_ResumeBackgroundJVM g_jvm_is_running_by_mmi =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10895_112_2_18_2_22_31_249,(uint8*)"d",g_jvm_in_background_by_mmi);
    if(g_jvm_in_background_by_mmi)
    {
        MMIJAVA_ResumeBackgroundJVM();
    }
    else if(MMK_IsOpenWin(MMIJAVA_PAUSE_DIALOG_WIN_ID))//yes use open,not focus
    {
        if ( ajms_arc_resumeApp() != AJMS_ERR_NONE )
        {
            //SCI_TRACE_LOW:"MMIAPIJAVA_ResumeBackgroundJVM ajms_arc_resumeApp fail!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10904_112_2_18_2_22_31_250,(uint8*)"" );
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIJAVA_ResumeBackgroundJVM do nothing exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10909_112_2_18_2_22_31_251,(uint8*)"");
    }
    #endif
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PauseBackgroundJVM( void )
{

#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
    //SCI_TRACE_LOW:"MMIAPIJAVA_PauseBackgroundJVM g_jvm_is_running_by_mmi =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10921_112_2_18_2_22_31_252,(uint8*)"d",g_jvm_in_background_by_mmi);
    if(g_jvm_in_background_by_mmi)
    {
        MMIJAVA_PauseBackgroundJVM();
    }
    else if(MMK_IsOpenWin(MMIJAVA_PAUSE_DIALOG_WIN_ID))
    {
		MMIJAVA_PauseAllMediaPlayer();
        if( ajms_arc_suspendApp() != AJMS_ERR_NONE )
        {
            //SCI_TRACE_LOW:"MMIAPIJAVA_PauseBackgroundJVM ajms_arc_suspendApp fail!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10931_112_2_18_2_22_31_253,(uint8*)"" );
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIJAVA_PauseBackgroundJVM do nothing exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_10936_112_2_18_2_22_31_254,(uint8*)"");
    }
#endif

}
/*****************************************************************************/
//  Description :  get current active sim
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIJAVA_GetAcitveSim(void)
{
    MN_DUAL_SYS_E dual_sys  = MN_DUAL_SYS_1;
    MMIJAVA_NETWORK_TYPE_E work_type = MMIJAVA_GetNetworkType();
    
    if(work_type < JAVA_NETWORK_TYPE_WIFI)
    {
        dual_sys = (MN_DUAL_SYS_E)work_type;
    }
    
    return dual_sys;
}

/*****************************************************************************/
// 	Description : MMIAPIJAVA_SetJavaAutoTckOn
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:set tck flag on
/*****************************************************************************/

void MMIAPIJAVA_SetJavaAutoTckOn(void)
{
	JWE_LOG(MMIAPIJAVA_SetJavaAutoTckOn,("enter g_jvm_is_auto_tck_mode=%d",g_jvm_is_auto_tck_mode));	
	g_jvm_is_auto_tck_mode = TRUE;
	JWE_LOG(MMIAPIJAVA_SetJavaAutoTckOn,("leave g_jvm_is_auto_tck_mode=%d",g_jvm_is_auto_tck_mode));	

}

/*****************************************************************************/
// 	Description : MMIAPIJAVA_SetJavaAutoTckOff
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:set tck flag off
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SetJavaAutoTckOff(void)
{
	JWE_LOG(MMIAPIJAVA_SetJavaAutoTckOff,("enter g_jvm_is_auto_tck_mode=%d",g_jvm_is_auto_tck_mode));	
	g_jvm_is_auto_tck_mode = FALSE;
	JWE_LOG(MMIAPIJAVA_SetJavaAutoTckOff,("leave g_jvm_is_auto_tck_mode=%d",g_jvm_is_auto_tck_mode));	

}

#ifdef VIRTUAL_KEY_BOARD
/*****************************************************************************/
// 	Description : MMIJAVA_SetForm1ShowHide
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetForm1ShowOrHide(BOOLEAN  flag)
{
	s_form1_display_flag = flag;

}

#endif

/*****************************************************************************/
// 	Description : MMIAPIJAVA_GetJavaAutoTckState
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:set tck flag off
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_GetJavaAutoTckState(void)
{
	return g_jvm_is_auto_tck_mode;	
}

/*****************************************************************************/
// 	Description : MMIJAVA_GetIntallInfo
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:get global install info
/*****************************************************************************/
PUBLIC JAVA_INSTALL_INFO_T*  MMIJAVA_GetIntallInfo(void)
{
    return  &s_install_info;

}

/*****************************************************************************/
// 	Description : MMIJAVA_SetIsJavaEnablePause
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetJavaIsEnablePause(BOOLEAN is_enable)
{
	s_b_enable_pasue = is_enable;

}

/*****************************************************************************/
// 	Description : MMIJAVA_GetJavaIsEnablePause
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_GetJavaIsEnablePause(void)
{
	return s_b_enable_pasue;	
}
/*****************************************************************************/
// 	Description : MMIJAVA_MakeCall
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_MakeCall(void)
{
    //SCI_TRACE_LOW:"MMIAPIJAVA_PlatformRequest enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_11039_112_2_18_2_22_32_255,(uint8*)"");

    if(PNULL != g_java_call_info_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIJAVA_PlatformRequest enter g_java_call_info_ptr = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_11043_112_2_18_2_22_32_256,(uint8*)"s",(char *)g_java_call_info_ptr);
        MMIAPICC_MakeCall(MMIJAVA_GetAcitveSim(),g_java_call_info_ptr,strlen((char *)g_java_call_info_ptr),PNULL,PNULL,CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,PNULL);  	  

        SCI_FREE(g_java_call_info_ptr)
    }
}

/*****************************************************************************/
// 	Description : MMIAPIJAVA_PlatformRequest
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PlatformRequest(uint32 type)
{
    PLATFORMREQUEST_TYPE request_type = (PLATFORMREQUEST_TYPE)type;

    //SCI_TRACE_LOW:"MMIAPIJAVA_PlatformRequest type = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_11058_112_2_18_2_22_32_257,(uint8*)"d",request_type);

    switch(request_type)
    {
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
        case MMI_PLATFORM_REQUEST_HTTP:
            MMIAPIJAVA_OpenWapWin();
            break;
#endif
        case MMI_PLATFORM_REQUEST_TEL:
            MMIJAVA_MakeCall();
            break;
        default:
            break;
    }
}
/*****************************************************************************/
//  Description :exit java
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_ExitJAVA(void)
{
    int32 ret = 0;
    int32 i = 0;
    
    JWE_LOG(MMIAPIJAVA_ExitJAVA, ("enter ajms_arc_getState=%d",ajms_arc_getState()));
    s_b_cancel_install = TRUE; //取消安装
     
    if (ajms_arc_getState() == AJSC_VMSTATE_NONE)
    {
        while(MMK_IsOpenWin(MMIJAVA_INSTALL_WIN_ID))//如果没有中止，等待下
        {
            SCI_Sleep(100);
            i++;
            if(i > MMIJAVA_EXIT_WAIT_COUNT)//最多等待 MMIJAVA_EXIT_WAIT_COUNT
            {
                break;

            }
        }
        JWE_LOG(MMIAPIJAVA_ExitJAVA, ("ajms_arc_getState=AJSC_VMSTATE_NONE. jvm has been ended and into none state."));
        return;
    }
    else if (ajms_arc_getState() >= AJSC_VMSTATE_STARTING) 
    {
        JWE_LOG(MMIAPIJAVA_ExitJAVA, ("ajms_arc_terminateApp"));
        ret = ajms_arc_terminateApp();	
        #ifdef MMI_TASK_MANAGER
        MMIAPIJAVA_StopApplet(FALSE);
        #endif
        if(ret == AJMS_ERR_NONE)
        {
            while(ajms_arc_getState() != AJSC_VMSTATE_NONE)//如果没有中止，等待下
            {
                SCI_Sleep(100);
                i++;
                if(i > MMIJAVA_EXIT_WAIT_COUNT)//最多等待 MMIJAVA_EXIT_WAIT_COUNT
                {
                    AamsVmKill( 1 );//否则强制关闭
                    break;
                }
            }
        }
        else
        {
            AamsVmKill( 1 );
        }
    } 
    else 
    {    
        MMK_CloseWin( MMIJAVA_AMS_MIDLETLIST_WIN_ID ); 
        MMK_CloseWin( MMIJAVA_MIDLET_WIN_ID );                                
    }

}

/*****************************************************************************/
// 	Description : MMIJAVA_SetIsJavaEnablePause
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_GetJavaIfRunningByMMI(void)
{
    return g_jvm_is_running_by_mmi;
}

/*****************************************************************************/
// 	Description : MMIJAVA_SetIsJavaEnablePause
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetJavaIfRunningByMMI(BOOLEAN is_flag)
{
    g_jvm_is_running_by_mmi = is_flag;
}

/*****************************************************************************/
// 	Description : MMIJAVA_SetJavaIfContinueInstall
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetJavaIfContinueInstall(BOOLEAN is_flag)
{
    g_b_needContinueLocalInstall = is_flag;
}

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register java module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_JAVA, &g_mmijava_applet_info);
}

/*****************************************************************************/
//  Description : the process message function of the JAVA
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  JavaApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIJAVA_APPLET_T* applet_ptr = (MMIJAVA_APPLET_T*)iapplet_ptr;

    //SCI_TRACE_LOW:"mmi JavaApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_11184_112_2_18_2_22_32_258,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_java_applet_instance = applet_ptr;
        MMK_CreateJavaWin( ( uint32* )MMIJAVA_APPLET_WIN_TAB, PNULL);  
        break;
        
    case MSG_APPLET_STOP: 
        if (MMK_IsOpenWin(MMIAPPLET_MANAGER_LIST_WIN_ID))
        {
            MMIJAVA_stopVM_external();  
        }
        else
        {
            //do nothing
        }
        s_java_applet_instance = PNULL;
        break;

    case MSG_APPLET_RESUME: 
        MMIAPIJAVA_OpenMainMenu();
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Get JAVA Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIJAVA_APPLET_T *MMIJAVA_GetAppletInstance(void)
{
    return s_java_applet_instance;
}

/*****************************************************************************/
//  Description : Create java  reference window for MMI_APPLET_STATE_BG_HIDE_WIN
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenJavaWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  )
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = SPRD_JAVA_APPLET_ID;
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;
    
    return MMK_CreateWinTable(&win_table_create);
}

/*****************************************************************************/
//  Description : Stop java applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopApplet(BOOLEAN is_minimize)
{
    if (is_minimize)
    {
        //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
        MMK_HideWinByApplet(SPRD_JAVA_APPLET_ID);
    }
    else
    {
        MMK_CloseApplet(SPRD_JAVA_APPLET_ID);
    }
}

/*****************************************************************************/
//  Description : start java applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StartApplet(void)
{
    MMI_APPLET_START_T start ={0};
    
    //SCI_TRACE_LOW:"MMIAPIJAVA_StartApplet"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_11268_112_2_18_2_22_32_259,(uint8*)"");
    start.guid = SPRD_JAVA_APPLET_ID;
    start.state = MMI_APPLET_STATE_BG_HIDE_WIN|MMI_APPLET_STATE_TASK_MANAGER;
    MMK_StartApplet( &start );
}

/**************************************************************************************************/
// Description:applet window message handler
// Date:
// Author:fen.xie
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E HandleJavaAppletWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E            result    = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"HandleJavaAppletWinMsg:msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_WINTAB_11287_112_2_18_2_22_32_260,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        break;

    case MSG_FULL_PAINT:
        MMK_PostMsg(win_id, MSG_LOSE_FOCUS , PNULL, 0);
        break;
        
    case MSG_CLOSE_WINDOW: 
        MMIAPIJAVA_StopApplet(FALSE);
        break;
        
    case MSG_GET_FOCUS: 
        break;
        
    case MSG_LOSE_FOCUS: 
        MMIAPIJAVA_StopApplet(TRUE);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif/*MMI_TASK_MANAGER*/


#endif //_MMIJAVA_WINTAB_C_

#endif


/*Edit by script, ignore 3 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527


/*Edit by script, ignore 22 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
