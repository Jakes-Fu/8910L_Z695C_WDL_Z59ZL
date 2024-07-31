/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/*****************************************************************************
** File Name:      mmijava_func.c                                    *
** Description:    This file is used to describe digit camera                *
******************************************************************************/

#ifndef _MMIJAVA_FUNC_C_
#define _MMIJAVA_FUNC_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "ajms_common.h"

#include "mmijava_internal.h"
#include "mmijava_export.h"

#include "mmi_modu_main.h"
#include "mmipub.h"
#include "ajms_cfg_eime.h"
#include "mmi_image.h"

#include "mediacontrol.h"
#include "block_mem.h"
#include "mmiidle_export.h"
#include "aamsvmctl.h"
#include "mmiudisk_export.h"
#include "mmimtv_export.h"
#include "mmiatv_export.h"
#include "mmidc_export.h"
#ifdef VIDEO_PLAYER_SUPPORT       
#include "mmivp_export.h"
#endif
#include "jkvmbackground.h"
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif

#ifdef IA_JAVA_SUPPORT_LOW_MEMORY
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#endif

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif


// define
#define MMIJAVA_DIR_RUN_SIZE (300*1024)
#define MMIJAVA_VMSTATE_TIMER_PEROID   	    500
#define MMIJAVA_VMSTATE_TIMER_COUNT  	    20
#define MMIJAVA_JWE_RUN_ERROR_CHECK 
#define MMIJAVA_STOPVM_TIMER_PEROID      1000
#define MMIJAVA_PLATFORM_VOLUME_MAX              MMISET_VOL_SIX
#define MMIJAVAV_PLATFORM_VOLUME_DEFAULT        MMISET_VOL_THREE




/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct _KeyCodeMapping
{
    int ajscCode;    
    int nativeCode;
} KeyCodeMapping;

typedef struct FLASH_BACKLIGHT 
{ 
	BOOLEAN is_backlighton; 
	SCI_TIMER_PTR flash_timer_id_ptr;
	uint32 flash_count; 
	uint32 flash_delay; 
}FLASH_BACKLIGHT_T; 
LOCAL FLASH_BACKLIGHT_T s_flash_backlight = {0};
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

const KeyCodeMapping keyCodeMapping[] =
{
    {AJSC_KEYCODE_NUM0,      KEY_0},
    {AJSC_KEYCODE_NUM1,      KEY_1},
    {AJSC_KEYCODE_NUM2,      KEY_2},
    {AJSC_KEYCODE_NUM3,      KEY_3},
    {AJSC_KEYCODE_NUM4,      KEY_4},
    {AJSC_KEYCODE_NUM5,      KEY_5},
    {AJSC_KEYCODE_NUM6,      KEY_6},
    {AJSC_KEYCODE_NUM7,      KEY_7},
    {AJSC_KEYCODE_NUM8,      KEY_8},
    {AJSC_KEYCODE_NUM9,      KEY_9},
    {AJSC_KEYCODE_ASTERISK,  KEY_STAR},
    {AJSC_KEYCODE_POUND,     KEY_HASH},
    {AJSC_KEYCODE_LEFT,      KEY_LEFT},
    {AJSC_KEYCODE_UP,        KEY_UP},
    {AJSC_KEYCODE_RIGHT,     KEY_RIGHT},
    {AJSC_KEYCODE_DOWN,      KEY_DOWN},
    {AJSC_KEYCODE_SELECT,    KEY_WEB},
    {AJSC_KEYCODE_SOFT1,     KEY_OK},
    {AJSC_KEYCODE_SOFT2,     KEY_CANCEL},
    {AJSC_KEYCODE_POWER,     KEY_RED},
};

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#ifdef FS_PROFILE	
extern int	 g_vm_exit;
#endif

#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
extern BOOLEAN g_jvm_in_background_by_mmi;
#endif

#ifdef USE_LOGFILE
extern SFS_HANDLE yuyulog_ptr;//
#endif

#ifndef WIN32
extern void LCD_SetBackLightBrightness( uint32  brightless );
#endif
extern BOOLEAN g_jvm_is_running_by_mmi;

extern const char bldTime_cldc[];
extern BOOLEAN s_b_force_killed;
#ifdef DRM_SUPPORT
extern int32 g_active_suite_id;
#endif

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
char  *pAppEx1Jad = PNULL;
char  *pAppEx1Jar = PNULL;

LOCAL  int32     g_java_platform_Volume = MMIJAVAV_PLATFORM_VOLUME_DEFAULT;//(g_java_platform_Volume is from 0-6)
const  uint8     g_actualVolumeMappingTable[MMIJAVA_PLATFORM_VOLUME_MAX][MMIJAVA_PLATFORM_VOLUME_MAX+1] = {
/*x-coordinate is master volume 0 - 6*/
/*y-coordinate is java volume level 0 - 5*/
/*map value is actualVolume  0 - 7, 7 is MMI_AUDIO_VOLUME_MAX*/
{0, 0, 0, 0, 0, 0, 0},
{0, 1, 2, 3, 4, 5, 6},
{0, 2, 3, 4, 5, 6, 7},
{0, 3, 4, 5, 6, 7, 7},
{0, 4, 5, 6, 7, 7, 7},
{0, 5, 6, 7, 7, 7, 7}
};


/*---------------------------------------------------------------------------*/
/*                          LOCAL VARIABLE DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL unsigned long s_lKeyState = 0; 
LOCAL int32         VMState_timer_id=0;
LOCAL char          s_java_version[100] = { 0};
LOCAL BOOLEAN       s_b_bgResume  = FALSE;
LOCAL BOOLEAN       s_b_needStopSuspendStateCheck = FALSE;
LOCAL int32         VMState_timer_count=0;
LOCAL int32         StopVM_timer_id=0;
LOCAL int32         StopVM_timer_Count=0;


/**--------------------------------------------------------------------------*
**                      INTERNAL CALL  FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

PUBLIC void jwe_vmstart_begin()                                                                 
{
	JWE_LOG(jwe_vmstart_begin, ("enter"))
	//g_jvm_is_running = TRUE;
    MMIJAVA_SetJavaRuning(TRUE);
}

PUBLIC void jwe_vmstart_end()                                                                 
{
	JWE_LOG(jwe_vmstart_end, ("enter"))

	MMIAPIJAVA_SendSignal(MSG_JAVA_VM_END_IND,MMIJAVA_MIDLET_WIN_ID);	
    MMIJAVA_SetJavaRuning(FALSE);
	
	#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
	JWE_LOG(jwe_vmstart_end, ("g_jvm_in_background_by_mmi = FALSE"));
	g_jvm_in_background_by_mmi = FALSE;
	#endif	
	
    MMIJAVA_SetIsDisplayLogo(TRUE);
    MMIJAVA_SetIsStillDisplayLogo(TRUE);
	
	#ifdef USE_LOGFILE
	JWE_LOG(jwe_vmstart_end, ("File log will Over"));
 	SCI_Sleep(5000);
	if(yuyulog_ptr!=0){
	SFS_CloseFile(yuyulog_ptr);
	}
	#endif
}                                                  


/*****************************************************************************/
//  Description :map native key to ajsc key code
//  Global resource dependence : none
//  Author: 
//  Note: nativeCode     native key code
/*****************************************************************************/
PUBLIC int32 MMIJAVA_NativeKeyToKJKey(int32 nativeCode)
{
    int32 i = 0;
    int32 n = ARR_SIZE(keyCodeMapping);
    for(i = 0; i < n; ++i)
    {
        if(keyCodeMapping[i].nativeCode == nativeCode)
        {
            
            return keyCodeMapping[i].ajscCode;
        }
    }
    return -1;
}

#if 0
/*****************************************************************************/
//  Description :start a java application
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_jwe_run(int32 suiteId, int32 midletId )
{

    AJ_S32  err_code = AJMS_ERR_NONE;
    const  AJ_U8*   m_jarPA
	JWE_LOG(MMIJAVA_jwe_run, ("suiteId %d, midletId %d", suiteId, midletId));
    /*device IME register callback*/
    ajms_cfg_registerEimeHandler(MMIJAVA_jwe_eimeStartupImpl, MMIJAVA_jwe_eimeTerminateImpl);
    // start the selected java application
    ajms_arc_runApp(suiteId, midletId, MMIJAVA_jwe_vmEventHandler);

    JWE_LOG(MMIJAVA_jwe_run, ("MMIJAVA_jwe_run leave"));
}
#endif
/*****************************************************************************/
//  Description :start a java application
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC AJ_S32 MMIJAVA_jwe_run(int32 suiteId, int32 midletId )
{
    AJ_S32          err_code = AJMS_ERR_NONE;   
#ifdef MMIJAVA_JWE_RUN_ERROR_CHECK
    const AJ_U8 *   m_jarPath = AJ_NULL;     // merlin
    AJ_S32          error_code;
    int32           wlength = 0;
    wchar           wfilename[MMIFILE_FILE_NAME_MAX_LEN] = {0};
    SFS_HANDLE      filehandle = 0;
    uint32          jarSize = 0;
    #ifndef IA_JAVA_SUPPORT_LOW_MEMORY
    uint32          readedsize = 0;
    #endif
    AJ_S32          iNumOfSuite = -1;
    
    JWE_LOG(MMIJAVA_jwe_run, ("suiteId %d, midletId %d", suiteId, midletId));
    /*device IME register callback*/
    ajms_cfg_registerEimeHandler(MMIJAVA_jwe_eimeStartupImpl, MMIJAVA_jwe_eimeTerminateImpl);
    // start the selected java application
    iNumOfSuite = ajms_ab_getSutieListCount(AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED ); 
    if ((iNumOfSuite > 0) && (suiteId <= (iNumOfSuite - 1))) //start preinstall midlet suite
    {
        err_code = ajms_arc_runApp(suiteId, midletId, MMIJAVA_jwe_vmEventHandler);
    }
    else
    {    
        m_jarPath = ajms_ab_getJarPath(suiteId,&error_code);
        if(PNULL == m_jarPath)
        {
            return AJMS_ERR_FILE_NOT_EXIST;
        }
        SCI_MEMSET(wfilename,0,sizeof(wfilename));
        wlength = GUI_UTF8ToWstr(wfilename,MMIFILE_FILE_NAME_MAX_LEN,(uint8 *)m_jarPath,strlen(m_jarPath));/*lint !e64*/
        filehandle = MMIAPIFMM_CreateFile(wfilename,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
        if (PNULL == filehandle)
        {
            return AJMS_ERR_FILE_NOT_EXIST;
        }

#ifdef DRM_SUPPORT
        if((MMIFMM_FILE_TYPE_JAVA == MMIAPIDRM_GetMediaFileType(filehandle, PNULL))
            && (!MMIAPIDRM_IsRightsValid(filehandle, PNULL, TRUE, DRM_PERMISSION_EXECUTE)))
        {
            //prompt error:版权无效
            #if 0
            MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_JAVA_DRM_INVALID_COPYRIGHTS, 
            TXT_NULL, 
            IMAGE_PUBWIN_WARNING, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
            #endif
            MMIAPIDRM_PreCheckFileStatus(wfilename,DRM_PERMISSION_PLAY);
            MMIAPIFMM_CloseFile(filehandle);           
            return AJMS_ERR_INVALID_PARAMS;
        }
 #endif
 
        jarSize = MMIAPIFMM_GetFileSize(filehandle);
        #ifndef IA_JAVA_SUPPORT_LOW_MEMORY
        if(jarSize <= 700*1024)
        {
            pAppEx1Jar = (char *)SCI_ALLOCA(sizeof(char)*jarSize +1);
            if(PNULL != pAppEx1Jar)
            {
                SCI_MEMSET(pAppEx1Jar,0,(sizeof(char)*jarSize +1));
                MMIAPIFMM_ReadFile(filehandle,pAppEx1Jar,jarSize,&readedsize,0);
                err_code = ajms_arc_runAppEx2(suiteId, midletId,(AJ_U8*) pAppEx1Jar,MMIJAVA_jwe_vmEventHandler);
            }
            else
            {
                err_code = ajms_arc_runApp(suiteId, midletId, MMIJAVA_jwe_vmEventHandler);
            }
        }
        else
        #endif
        {
            err_code = ajms_arc_runApp(suiteId, midletId, MMIJAVA_jwe_vmEventHandler);
        }
        MMIAPIFMM_CloseFile(filehandle);
    }
#else
  ajms_cfg_registerEimeHandler(MMIJAVA_jwe_eimeStartupImpl, MMIJAVA_jwe_eimeTerminateImpl);
  err_code = ajms_arc_runApp(suiteId, midletId, MMIJAVA_jwe_vmEventHandler);
#endif

    return err_code;
}

/*****************************************************************************/
//  Description :MMIJAVA_CheckIfPermitAutoRun
//  Global resource dependence : none
//  Author: murphy.xie
//  Note: check if permit auto run java midlet
/*****************************************************************************/
LOCAL BOOLEAN MMIJAVA_CheckIfPermitAutoRun(void)
{

    JWE_LOG(MMIJAVA_CheckIfPermitAutoRun,("enter"));

    if(MMIAPIUDISK_UdiskIsRun()
        ||MMIAPIATV_IsATVOpen()
        ||MMIAPIDC_IsOpened()
#ifdef VIDEO_PLAYER_SUPPORT       
        || MMIAPIVP_IsOpenPlayerWin()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsOpenMobileVideo()
#endif
#ifdef IA_JAVA_SUPPORT_LOW_MEMORY
#ifdef BROWSER_SUPPORT
        ||MMIAPIBROWSER_IsRunning()
#endif
#endif
    )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
  
}

/*****************************************************************************/
//  Description :auto run midlet
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
PUBLIC int32 MMIJAVA_ASA_autorunApp(int32 suiteId, int32 midletId)
{
    int32                     java_state = 0;
    int32                     i = 0;
    int                       suitelist_index = 0;
    JAVA_INSTALL_INFO_T*      install_info_ptr = MMIJAVA_GetIntallInfo();

    if(!MMIJAVA_CheckIfPermitAutoRun())
    {
        JWE_LOG(MMIJAVA_ASA_autorunApp, ("not permit run auto push"));
        return -1;
    }
    
    if(suiteId < 0)
    {
        JWE_LOG(MMIJAVA_ASA_autorunApp, ("suit id: %d", suiteId));
        return -1;
    }
    
    java_state = ajms_arc_getState();
    
    if( ( AJSC_VMSTATE_STARTING == java_state ) || ( AJSC_VMSTATE_STARTED == java_state ) )
    {
        JWE_LOG(MMIJAVA_ASA_autorunApp, ("ajms_arc_terminateApp"));
        ajms_arc_terminateApp();
        #ifdef MMI_TASK_MANAGER
        MMIAPIJAVA_StopApplet(FALSE);
        #endif	
        while( ajms_arc_getState() != AJMS_VMEXIT_TERMINATED )
        {
            SCI_Sleep(100);
            i++;
            if(i > 5)
            {
                break;
            }
        }
    }
    
    if(MMK_IsOpenWin(MMIJAVA_MIDLET_WIN_ID))
    {
        JWE_LOG(ASA_autorun, ("close MMIJAVA_MIDLET_WIN_ID"));
        MMK_CloseWin(MMIJAVA_MIDLET_WIN_ID);       
    }
    if(MMK_IsOpenWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID))
    {
        JWE_LOG(ASA_autorun, ("close MMIJAVA_AMS_MIDLETLIST_WIN_ID"));      
        MMK_CloseWin(MMIJAVA_AMS_MIDLETLIST_WIN_ID);       
    }
    
    SCI_MEMSET(install_info_ptr,0,sizeof(JAVA_INSTALL_INFO_T));
    install_info_ptr->curmidletid = midletId;
    install_info_ptr->cursuitid = suiteId;
    install_info_ptr->is_flag = 0;
    JWE_LOG(MMIJAVA_ASA_autorunApp, ("create MMIJAVA_AMS_MIDLETLIST_WIN_TAB suite id =%d,midletid=%d",suitelist_index,midletId));   
	MMIJAVA_OpenMidletWindow(install_info_ptr);   
    return 0;
}

/*****************************************************************************/
//  Description :直接从jad文件运行
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_jwe_launchJad(const wchar* jadName, const wchar* jarName )
{
    SFS_HANDLE filehandle = 0;
    uint32 jadSize = 0;
    uint32 jarSize = 0;
    uint32 readedsize = 0;   
    int32  result = 0;    
    filehandle = MMIAPIFMM_CreateFile(jadName,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
    if(filehandle == 0)
    {
        JWE_LOG(MMIJAVA_jwe_launchJad,("jwe_launch log failed"));        
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return FALSE;
    }    

#ifdef DRM_SUPPORT
    if((MMIFMM_FILE_TYPE_JAVA == MMIAPIDRM_GetMediaFileType(filehandle, PNULL))
        && (!MMIAPIDRM_IsRightsValid(filehandle, PNULL, TRUE, DRM_PERMISSION_EXECUTE)))
    {
        //prompt error:版权无效
        #if 0
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_DRM_INVALID_COPYRIGHTS, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING, 
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL);
        #endif
        
        MMIAPIDRM_PreCheckFileStatus((wchar *)jarName,DRM_PERMISSION_PLAY);
        MMIAPIFMM_CloseFile(filehandle);           
        return FALSE;
    }
#endif
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
        return FALSE;
    }
    
    pAppEx1Jad = (char *)SCI_ALLOCA(sizeof(char)*jadSize +1);
    if(PNULL == pAppEx1Jad)
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
    	JWE_LOG(MMIJAVA_jwe_launchJad,(" alloc pAppEx1Jad failed"));
        return FALSE;
    }
    
    SCI_MEMSET(pAppEx1Jad,0,(sizeof(char)*jadSize +1));
    if(MMIAPIFMM_ReadFile(filehandle,pAppEx1Jad,jadSize,&readedsize,0) != SFS_NO_ERROR)
    {
        JWE_LOG(MMIJAVA_jwe_launchJad,( "SFS_ReadFile is error" ));
        if( pAppEx1Jad != NULL )
        {
            SCI_FREE( pAppEx1Jad );
            pAppEx1Jad = NULL;
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
        return FALSE;
    }    
    
    if(MMIAPIFMM_CloseFile(filehandle) != SFS_NO_ERROR)
    {
        JWE_LOG(MMIJAVA_jwe_launchJad,( "SFS_CloseFile is error" ));
        if( pAppEx1Jad != NULL )
        {
            SCI_FREE( pAppEx1Jad );
            pAppEx1Jad = NULL;
        }
   
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return FALSE;
    }       
    
    filehandle = MMIAPIFMM_CreateFile(jarName,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
    if(filehandle == 0)
    {
        JWE_LOG(MMIJAVA_jwe_launchJad,("yuyu log failed"));
        if( pAppEx1Jad != NULL )
        {
            SCI_FREE( pAppEx1Jad );
            pAppEx1Jad = NULL;
        }
   
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return FALSE;
    }    
    jarSize = MMIAPIFMM_GetFileSize(filehandle);    
    
    if( jarSize == 0)
    {
        if( pAppEx1Jad != NULL )
        {
            SCI_FREE( pAppEx1Jad );
            pAppEx1Jad = NULL;
        }
	    JWE_LOG(MMIJAVA_jwe_launchJad,("get file size is zero"));	
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        MMIAPIFMM_CloseFile(filehandle);
        return FALSE;
    }

    if( jarSize > MMIJAVA_DIR_RUN_SIZE)
    {
        if( pAppEx1Jad != NULL )
        {
            SCI_FREE( pAppEx1Jad );
            pAppEx1Jad = NULL;
        }
	    JWE_LOG(MMIJAVA_jwe_launchJad,("get file size is zero"));	
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_NO_SPACE, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        MMIAPIFMM_CloseFile(filehandle);
        return FALSE;
    }
    
	pAppEx1Jar = (char *)SCI_ALLOCA(sizeof(char)*jarSize+1);
    
    if(PNULL == pAppEx1Jar)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_COMM_INSTALL_FAILED, 
        TXT_NULL, 
        IMAGE_PUBWIN_FAIL, 
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL);
        
        if( pAppEx1Jad != NULL )
        {
            SCI_FREE( pAppEx1Jad );
            pAppEx1Jad = NULL;
        }
        MMIAPIFMM_CloseFile(filehandle);
    	JWE_LOG(MMIJAVA_jwe_launchJad,(" alloc pAppEx1Jar failed"));
        return FALSE;
    }
    
    SCI_MEMSET(pAppEx1Jar,0,(sizeof(char)*jarSize+1));
    
    if(MMIAPIFMM_ReadFile(filehandle,pAppEx1Jar,jarSize,&readedsize,0) != SFS_NO_ERROR)
    {
        JWE_LOG(MMIJAVA_jwe_launchJad,( "SFS_ReadFile is error" ));
        if( pAppEx1Jad != NULL )
        {
            SCI_FREE( pAppEx1Jad );
            pAppEx1Jad = NULL;
        }
        if( pAppEx1Jar != NULL )
        {
            SCI_FREE(pAppEx1Jar);          
            pAppEx1Jar = NULL;
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
        return FALSE;
    }    
    
    if(MMIAPIFMM_CloseFile(filehandle) != SFS_NO_ERROR)
    {
        JWE_LOG(MMIJAVA_jwe_launchJad,( "SFS_CloseFile is error" ));
        if( pAppEx1Jad != NULL )
        {
            SCI_FREE( pAppEx1Jad );
            pAppEx1Jad = NULL;
        }
        if( pAppEx1Jar != NULL )
        {
		    SCI_FREE( pAppEx1Jar );
            pAppEx1Jar = NULL;
        }
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return FALSE;
    }
    
    
    /*device IME register callback*/
    ajms_cfg_registerEimeHandler(MMIJAVA_jwe_eimeStartupImpl, MMIJAVA_jwe_eimeTerminateImpl);

    result = ajms_arc_runAppEx1( jadSize, (const AJ_U8 *)pAppEx1Jad, NULL,
        jarSize, (const AJ_U8 *)pAppEx1Jar, NULL,
        MMIJAVA_jwe_vmEventHandler );
    return TRUE;

}

/*****************************************************************************/
//  Description :直接从jar文件运行
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_jwe_launchJar( const wchar* jarName )
{    

    SFS_HANDLE filehandle = 0;
    uint32 jarSize = 0;
    uint32 readedsize = 0;
    int32 result = 0;
    
    
    filehandle = MMIAPIFMM_CreateFile(jarName,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
    if(SFS_INVALID_HANDLE == filehandle )
    {
        JWE_LOG(MMIJAVA_jwe_launchJar,("yuyu log failed"));        
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return FALSE;
    }
#ifdef DRM_SUPPORT
    if((MMIFMM_FILE_TYPE_JAVA == MMIAPIDRM_GetMediaFileType(filehandle, PNULL))
        && (!MMIAPIDRM_IsRightsValid(filehandle, PNULL, TRUE, DRM_PERMISSION_EXECUTE)))
    {
        //prompt error:版权无效
        #if 0
        MMIPUB_OpenAlertWinByTextId(PNULL, 
        TXT_JAVA_DRM_INVALID_COPYRIGHTS, 
        TXT_NULL, 
        IMAGE_PUBWIN_WARNING, 
        PNULL, 
        PNULL,
        MMIPUB_SOFTKEY_ONE,
        PNULL);
        #endif
        MMIAPIDRM_PreCheckFileStatus((wchar *)jarName,DRM_PERMISSION_PLAY);
        MMIAPIFMM_CloseFile(filehandle);           
        return FALSE;
    }
#endif
    jarSize = MMIAPIFMM_GetFileSize(filehandle);    
    
    if((jarSize == 0))        
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
        return FALSE;
    }

    if(jarSize > MMIJAVA_DIR_RUN_SIZE)        
    {
         
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_NO_SPACE, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        MMIAPIFMM_CloseFile(filehandle);
        return FALSE;
    }
    
    
	pAppEx1Jar = (char *)SCI_ALLOC_APP(sizeof(char)*jarSize+1);
    
    SCI_MEMSET(pAppEx1Jar,0,(sizeof(char)*jarSize+1));
    
    if(MMIAPIFMM_ReadFile(filehandle,pAppEx1Jar,jarSize,&readedsize,0) != SFS_NO_ERROR)
    {
        JWE_LOG(MMIJAVA_jwe_launchJar,( "SFS_ReadFile is error" ));   
        if( pAppEx1Jar != PNULL )
        {
		    SCI_FREE( pAppEx1Jar );
            pAppEx1Jar = NULL;
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
        return FALSE;
    }
    
    
    if(MMIAPIFMM_CloseFile(filehandle) != SFS_NO_ERROR)
    {
        JWE_LOG(MMIJAVA_jwe_launchJar,( "SFS_CloseFile is error" ));
        if( pAppEx1Jar != PNULL )
        {
		    SCI_FREE( pAppEx1Jar );
            pAppEx1Jar = NULL;
        }        
        MMIPUB_OpenAlertWinByTextId(PNULL, 
            TXT_COMM_INSTALL_FAILED, 
            TXT_NULL, 
            IMAGE_PUBWIN_FAIL, 
            PNULL, 
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);
        return FALSE;
    }
    
    
    /*device IME register callback*/
    ajms_cfg_registerEimeHandler(MMIJAVA_jwe_eimeStartupImpl, MMIJAVA_jwe_eimeTerminateImpl);

    result = ajms_arc_runAppEx1( 0, AJ_NULL, AJ_NULL,
        jarSize, (const AJ_U8 *)pAppEx1Jar, AJ_NULL,
        MMIJAVA_jwe_vmEventHandler );   
        
    return TRUE;

}

/*****************************************************************************/
//  Description :ajsc key event handler
//  Global resource dependence : none
//  Author: 
//  Note: type     key event type     keyCode  key code id
/*****************************************************************************/
PUBLIC void MMIJAVA_sendKeyEvent(int32 type, int32 keyCode) 
{
    int32 i = 0;
    int32 ret = 0;    
   
    if ((keyCode == AJSC_KEYCODE_POWER))
    {
        JWE_LOG(MMIJAVA_sendKeyEvent, ("keyCode == AJSC_KEYCODE_POWER,We do not need deal with AJSC_KEYCODE_POWER"));
        return;
    }
    if ((keyCode == 80) && (type == 1))
    {
        return;
    }
    
    if (ajms_arc_getState() >= AJSC_VMSTATE_STARTING) 
    {
        if ((keyCode == AJSC_KEYCODE_POWER) && (type == 0)) 
        {
            JWE_LOG(MMIJAVA_sendKeyEvent, ("ajms_arc_terminateApp"));
            ret = ajms_arc_terminateApp();
            #ifdef MMI_TASK_MANAGER
            MMIAPIJAVA_StopApplet(FALSE);
            #endif
            if(ret == AJMS_ERR_NONE)
            {
                while( ajms_arc_getState() != AJMS_VMEXIT_TERMINATED )
                {
                    SCI_Sleep(100);
                    i++;
                    if(i > 5) break;
                }
            }
            else
            {
                JWE_LOG(sendKeyEvent, ("ajms_arc_terminateApp failed, close window"));
                MMK_CloseWin( MMIJAVA_AMS_MIDLETLIST_WIN_ID ); 
                MMK_CloseWin( MMIJAVA_MIDLET_WIN_ID ); 
            }
            
        } 
        else if ((keyCode == 80) && (type == 0) &&
            (ajms_arc_getState() == AJSC_VMSTATE_STARTED)) 
        {
            ajms_arc_suspendApp();
            
        } else if ((keyCode == 80) && (type == 0) &&
            (ajms_arc_getState() == AJSC_VMSTATE_SUSPENDED)) 
        {
            ajms_arc_resumeApp();
            
        } 
        else 
        {
            ajsc_keyEventHandler(type, keyCode, 0);
            
            switch( type )
            {
            case AJSC_EVENT_KEY_PRESSED:
                if ( keyCode == AJSC_KEYCODE_UP )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_UP;
                }
                else if ( keyCode == AJSC_KEYCODE_DOWN )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_DOWN;
                }
                else if ( keyCode == AJSC_KEYCODE_LEFT )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_LEFT;
                }
                else if ( keyCode == AJSC_KEYCODE_RIGHT )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_RIGHT;
                }
                else if ( keyCode == AJSC_KEYCODE_SELECT )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_FIRE;
                }
                else if ( keyCode == AJSC_KEYCODE_NUM0 )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_A;
                }
                else if ( keyCode == AJSC_KEYCODE_NUM1 )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_B;
                }
                else if ( keyCode == AJSC_KEYCODE_NUM2 )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_C;
                }
                else if ( keyCode == AJSC_KEYCODE_NUM3 )
                {
                    s_lKeyState |= KJAVA_DEVICE_GAME_KEY_D;
                }
                break;
                
            case AJSC_EVENT_KEY_RELEASED:
                if ( keyCode == AJSC_KEYCODE_UP )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_UP);
                else if ( keyCode == AJSC_KEYCODE_DOWN )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_DOWN);
                else if ( keyCode == AJSC_KEYCODE_LEFT )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_LEFT);
                else if ( keyCode == AJSC_KEYCODE_RIGHT )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_RIGHT);
                else if ( keyCode == AJSC_KEYCODE_SELECT )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_FIRE);
                else if ( keyCode == AJSC_KEYCODE_NUM0 )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_A);
                else if ( keyCode == AJSC_KEYCODE_NUM1 )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_B);
                else if ( keyCode == AJSC_KEYCODE_NUM2 )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_C);
                else if ( keyCode == AJSC_KEYCODE_NUM3 )
                    s_lKeyState &= (~KJAVA_DEVICE_GAME_KEY_D);
                break;
                
            default:
                break;
            }
        }
    } 
    else 
    {
        if ((keyCode == AJSC_KEYCODE_POWER) && (type == 0)) 
        {
            MMK_CloseWin( MMIJAVA_AMS_MIDLETLIST_WIN_ID ); 
            MMK_CloseWin( MMIJAVA_MIDLET_WIN_ID ); 
        } 
        else 
        {
            ajsc_keyEventHandler(type, keyCode, 0);
        }
    }
}
/*****************************************************************************/
//  Description :HandleJavaWaitWinMsg
//  Global resource dependence : none
//  Author: 
//  Note: java exit waiting win message handler
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleJavaWaitWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    )
{
    MMI_RESULT_E      recode = MMI_RESULT_TRUE;   
    MMI_STRING_T      waitquit_text = {0};
    
    MMI_GetLabelTextByLang(TXT_JAVA_WAITQUIT, &waitquit_text);// TXT_JAVA_WAITQUIT
    JWE_LOG(HandleJavaWaitWinMsg, ("msg_id = %x param=%d", msg_id,param));
    
    switch(msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:     
    case MSG_APP_CANCEL:
        JWE_LOG(HandleJavaWaitWinMsg,("MSG_APP_CANCEL MSG_APP_RED MSG_APP_CANCEL ajms_arc_terminateApp" ));
        if ( ajms_arc_terminateApp() != AJMS_ERR_NONE )
        {
            JWE_LOG( HandleJavaWaitWinMsg, ( "ajms_arc_terminateApp fail!!!" ) )
        }	
        
        MMIPUB_SetWaitWinText(TRUE,1,&waitquit_text,PNULL,PNULL,win_id);  
        
	    break;
    case MSG_JAVA_WAITWIN_END:
        JWE_LOG(HandleJavaWaitWinMsg,("MSG_JAVA_WAITWIN_END" ));
        if(param == (DPARAM)AJSC_VMSTATE_SUSPENDED)
        {
            JWE_LOG(HandleJavaWaitWinMsg,("MSG_JAVA_WAITWIN_END AJSC_VMSTATE_SUSPENDED" ));
            if ( ajms_arc_resumeApp() != AJMS_ERR_NONE )
            {
                JWE_LOG(HandleJavaWaitWinMsg,("ajms_arc_resumeApp fail!!!" ))	
            }
        }
        else
        {
            JWE_LOG(HandleJavaWaitWinMsg,("MSG_JAVA_WAITWIN_END !AJSC_VMSTATE_SUSPENDED" ));
        }	

        MMIPUB_CloseWaitWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
// 	Description : MMIAPIJAVA_StartCheckVmState
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:check jvm state time out function 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StartCheckVmState(int32 msgVMState,DPARAM param)
{
	int32 java_state = 0;
	
	JWE_LOG(MMIAPIJAVA_StartCheckVmState,(" enter"));

	if(s_b_needStopSuspendStateCheck)
	{
		JWE_LOG(MMIAPIJAVA_StartCheckVmState,("s_b_needStopSuspendStateCheck==TRUE no need check suspend and suspending state"));
		s_b_needStopSuspendStateCheck = FALSE;
		return;
	}	

	switch ( msgVMState )
	{
		case MSG_JAVA_VM_SUSPEND_IND:
		if(s_b_force_killed)
        {
            JWE_LOG(MMIAPIJAVA_StartCheckVmState,("VM has been force killed!"));
            if(VMState_timer_id!=0)
            {
                MMK_StopTimer(VMState_timer_id);
                VMState_timer_id = 0;
            }	
            break;
        }
		
		java_state = ajms_arc_getState();
		if(java_state == AJSC_VMSTATE_SUSPENDED)
		{
			JWE_LOG(MMIAPIJAVA_StartCheckVmState,("java_state is AJSC_VMSTATE_SUSPENDED. No need create CheckVmStateTimer"));
			if(MMK_IsOpenWin(MMIJAVA_WAIT_WIN_ID)){
				JWE_LOG(MMIAPIJAVA_StartCheckVmState,("MMIJAVA_WAIT_WIN_ID is open. Close it."));
				MMK_SendMsg(MMIJAVA_WAIT_WIN_ID, MSG_JAVA_WAITWIN_END, (DPARAM)AJSC_VMSTATE_SUSPENDED);
			}
			else
            {
                if(s_b_bgResume)
                {
                    /*corresponding to MMIJAVA_ResumeBackgroundJVM when java_state == AJSC_VMSTATE_SUSPENDING*/
                    if(JkGetVmBgFgState() == JK_BG_VM_STATE_BACKGROUND)/*lint !e746*/			
                    {
                        if ( ajms_arc_resumeApp() != AJMS_ERR_NONE )
                        {
                            JWE_LOG(MMIAPIJAVA_StartCheckVmState,("ajms_arc_resumeApp fail!!!" ))	
                        }

                        if ( ajms_arc_backgroundApp() != AJMS_ERR_NONE )
                        {
                            JWE_LOG( MMIAPIJAVA_StartCheckVmState, ( "ajms_arc_backgroundApp fail!!!" ) )
                        }
                    }
                    s_b_bgResume = FALSE;
                }

            }	
		}
		else
		{
			JWE_LOG(MMIAPIJAVA_StartCheckVmState,("java_state is not in AJSC_VMSTATE_SUSPENDED (java_state is %d). Create CheckVmStateTimer",java_state));
			if(VMState_timer_id!=0)
			{
				JWE_LOG(MMIAPIJAVA_StartCheckVmState,("not create 2 timers",java_state));
				MMK_StopTimer(VMState_timer_id);
				VMState_timer_id = 0;
			}
            VMState_timer_count = 0;
            VMState_timer_id = MMK_CreateTimerCallback( MMIJAVA_VMSTATE_TIMER_PEROID, MMIAPIJAVA_CheckVmStateTimer_Process, NULL, SCI_TRUE);
            MMK_StartTimerCallback(VMState_timer_id, MMIJAVA_VMSTATE_TIMER_PEROID, MMIAPIJAVA_CheckVmStateTimer_Process, NULL, SCI_TRUE);

		}
		break;     

		default:
		    break;
	}  

}
/*****************************************************************************/
// 	Description : MMIAPIJAVA_CheckVmStateTimer_Process
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:check jvm state time out function 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_CheckVmStateTimer_Process(uint8 timerID, uint32 param)
{
	int32 java_state = 0;
	int32 err;

	if (timerID == VMState_timer_id)
	{
		VMState_timer_count++;
		JWE_LOG(MMIAPIJAVA_CheckVmStateTimer_Process,("VMState_timer_id active enter VMState_timer_count=%d",VMState_timer_count));
		
		if(s_b_needStopSuspendStateCheck)
		{
			JWE_LOG(MMIAPIJAVA_CheckVmStateTimer_Process,("s_b_needStopSuspendStateCheck==TRUE"));
			MMK_StopTimer(VMState_timer_id);
			VMState_timer_id = 0;
			s_b_needStopSuspendStateCheck=FALSE;
			return ;	
		}		
		
		if(VMState_timer_count == MMIJAVA_VMSTATE_TIMER_COUNT)
		{
			err = AamsVmKill( 1 );
			JWE_LOG(MMIAPIJAVA_CheckVmStateTimer_Process,("VMState_timer_count == MMIJAVA_VMSTATE_TIMER_COUNT force kill VM err=%d",VMState_timer_count,err));
			s_b_force_killed = TRUE;
		}
		
		if(s_b_force_killed)
        {
			JWE_LOG(MMIAPIJAVA_CheckVmStateTimer_Process,("VM has been force killed!"));
			MMK_StopTimer(VMState_timer_id);
			VMState_timer_id = 0;
			return;
		}
		
		java_state = ajms_arc_getState();
		if(java_state == AJSC_VMSTATE_SUSPENDED)
		{
			JWE_LOG(MMIAPIJAVA_CheckVmStateTimer_Process,("java state==AJSC_VMSTATE_SUSPENDED stop VMState_timer_id=%d",VMState_timer_id));
			MMK_StopTimer(VMState_timer_id);
			VMState_timer_id = 0;
			if(MMK_IsOpenWin(MMIJAVA_WAIT_WIN_ID))
            {
                MMK_SendMsg(MMIJAVA_WAIT_WIN_ID, MSG_JAVA_WAITWIN_END, (DPARAM)AJSC_VMSTATE_SUSPENDED);
            }
			else
			{
				if(s_b_bgResume)
				{
					/*corresponding to MMIJAVA_ResumeBackgroundJVM when java_state == AJSC_VMSTATE_SUSPENDING*/
					if(JkGetVmBgFgState()==JK_BG_VM_STATE_BACKGROUND)					
					{
						if ( ajms_arc_resumeApp() != AJMS_ERR_NONE )
						{
							JWE_LOG(MMIAPIJAVA_CheckVmStateTimer_Process,("ajms_arc_resumeApp fail!!!" ))	
						}

						if ( ajms_arc_backgroundApp() != AJMS_ERR_NONE )
                        {
                            JWE_LOG( MMIAPIJAVA_CheckVmStateTimer_Process, ( "ajms_arc_backgroundApp fail!!!" ) )
                        }
					}
					
					s_b_bgResume =FALSE;
				}

			}
			
		}
		else
		{
			JWE_LOG(MMIAPIJAVA_CheckVmStateTimer_Process,("ajms_arc_getState is not in AJSC_VMSTATE_SUSPENDED(java_state is %d). wait next period",java_state));
		}
	}
	
}

/*****************************************************************************/
//  Description :MMIJAVA_resumeVM_external
//  Global resource dependence : none
//  Author: 
//  Note: make jvm to running state
/*****************************************************************************/
PUBLIC void MMIJAVA_resumeVM_external(void)
{
	int32 java_state = 0;
	MMI_STRING_T            wait_text = {0};
	MMI_GetLabelTextByLang(TXT_JAVA_WAIT, &wait_text);
	JWE_LOG(MMIJAVA_resumeVM_external,("enter" ))	         

    java_state = ajms_arc_getState();
    JWE_LOG(MMIJAVA_resumeVM_external,("The KVM Resume java_state = %d",java_state ));

    
    if (java_state == AJSC_VMSTATE_NONE)
    {
        JWE_LOG(MMIJAVA_resumeVM_external, ("ajms_arc_getState=AJSC_VMSTATE_NONE. jvm has been ended and into none state."));
        return;
    }
    else if( ( AJSC_VMSTATE_RESUMING == java_state )  || ( AJSC_VMSTATE_STARTED == java_state ) )
    {
        JWE_LOG(MMIJAVA_resumeVM_external,("VM is already in AJSC_VMSTATE_RESUMING or AJSC_VMSTATE_STARTED (%d),return.",java_state));
        return;
    }
    else if( AJSC_VMSTATE_SUSPENDING == java_state )
    {
        JWE_LOG(MMIJAVA_resumeVM_external,("java_state = AJSC_VMSTATE_SUSPENDING. , OpenWaitWin "));	
        MMIPUB_OpenWaitWin(1,
        &wait_text,
        PNULL,
        PNULL,
        MMIJAVA_WAIT_WIN_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_ONE,
        HandleJavaWaitWinMsg);
    }   
    else if( AJSC_VMSTATE_SUSPENDED == java_state )
    {
        JWE_LOG(MMIJAVA_resumeVM_external,("java_state = AJSC_VMSTATE_SUSPENDED , call ajms_arc_resumeApp" ))	;
        if ( ajms_arc_resumeApp() != AJMS_ERR_NONE )
        {
            JWE_LOG(MMIJAVA_resumeVM_external,("ajms_arc_resumeApp fail!!!" ))	
        }
		if(JkGetVmBgFgState()==JK_BG_VM_STATE_BACKGROUND)
		{
			JWE_LOG(MMIJAVA_resumeVM_external,("jvm is inJK_BG_VM_STATE_BACKGROUND need ajms_arc_foregroundApp" ))	;
			if ( ajms_arc_foregroundApp() != AJMS_ERR_NONE )
			{
			JWE_LOG(MMIJAVA_resumeVM_external,("ajms_arc_foregroundApp fail!!!" ));	
			}
			/* Notify VM to foreground.  */
			AamsVmForeground( 1 );
		}

	}	            		
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
	else if (java_state == AJSC_VMSTATE_BACKGROUND)
	{
		JWE_LOG(MMIJAVA_resumeVM_external,("java_state =  AJSC_VMSTATE_BACKGROUND"));	
		if ( ajms_arc_foregroundApp() != AJMS_ERR_NONE )
		{
			JWE_LOG(MMIJAVA_resumeVM_external,("ajms_arc_foregroundApp fail!!!" ));	
		}
		/* Notify VM to foreground.  */
		AamsVmForeground( 1 );
	}

	JWE_LOG(MMIJAVA_resumeVM_external,("set g_jvm_in_background_by_mmi=FALSE"));
	g_jvm_in_background_by_mmi = FALSE;
    MAIN_UpdateIdleJavaState(FALSE);

#endif	
}    


/*****************************************************************************/
//  Description :MMIJAVA_pauseVM_external,stop jvm by external
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_pauseVM_external(mmijava_state_enum pause_level)
{
	int32 java_state = ajms_arc_getState();
	
 	JWE_LOG(MMIJAVA_pauseVM_external,("enter" ))	

	switch ( pause_level )
	{
		case MMIJAVA_LONG_EVENT_STATE:
		 JWE_LOG(MMIJAVA_pauseVM_external,("pause_level = JVM_LONG_EVENT_STATE" ))	
            if (java_state == AJSC_VMSTATE_NONE)
            {
                JWE_LOG(MMIJAVA_pauseVM_external, ("ajms_arc_getState=AJSC_VMSTATE_NONE. jvm has been ended and into none state."));
                return;
            }
            else if ( java_state == AJSC_VMSTATE_SUSPENDED || java_state == AJSC_VMSTATE_SUSPENDING )
            {
                JWE_LOG(MMIJAVA_pauseVM_external,("VM is already suspended or suspending,return." ));
                return;
            }
            else
            {
                s_b_needStopSuspendStateCheck = FALSE;
                MMIJAVA_PauseAllMediaPlayer();
                MMIJAVA_unlock_device_multimedia();
                JWE_LOG(MMIJAVA_pauseVM_external,("Because something hapen,The KVM Suspend java_state = %d",java_state ))
                if ( ajms_arc_suspendApp() != AJMS_ERR_NONE )
                {
                    JWE_LOG( MMIJAVA_pauseVM_external, ( "ajms_arc_suspendApp fail!!!" ) );
                }
                
            }
			break;
		case MMIJAVA_SHORT_EVENT_STATE:
            JWE_LOG(MMIJAVA_pauseVM_external,("pause_level = MMIJAVA_SHORT_EVENT_STATE" ));	
			break;			
		default:
			break;
	}
}

/*****************************************************************************/
//  Description :MMIJAVA_backgroundVM_external
//  Global resource dependence : none
//  Author: 
//  Note: make jvm to background state
/*****************************************************************************/
PUBLIC void MMIJAVA_backgroundVM_external(void)
{
    #ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
	int32 java_state = 0;
 	JWE_LOG(MMIJAVA_backgroundVM_external,("enter" ));
	g_jvm_in_background_by_mmi = TRUE;
	java_state = ajms_arc_getState();
	if ( java_state == AJSC_VMSTATE_BACKGROUND){
		JWE_LOG( MMIJAVA_backgroundVM_external, ( "VM is already in  or background, return." ) );
		return;
	}
    //先resume,然后进入后台
	if(java_state == AJSC_VMSTATE_SUSPENDED)
    {
        if ( ajms_arc_resumeApp() != AJMS_ERR_NONE )
        {
            JWE_LOG(MMIJAVA_backgroundVM_external,("ajms_arc_resumeApp fail!!!" ))	
        }
    }
    
	if ( ajms_arc_backgroundApp() != AJMS_ERR_NONE ){
		JWE_LOG( MMIJAVA_backgroundVM_external, ( "ajms_arc_backgroundApp fail!!!" ) )
	}

	AamsVmBackground(1);
    MAIN_UpdateIdleJavaState(TRUE);
    
    #endif
}


/*****************************************************************************/
//  Description :MMIAPIJAVA_StopVmTimer_Process
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIAPIJAVA_StopVmTimer_Process(uint8 timerID, uint32 param)
{
	if (timerID != StopVM_timer_id)
    {
        return ;
    }
	JWE_LOG(MMIAPIJAVA_CheckVmStateTimer_Process,("enter count=%d", StopVM_timer_Count));

    if(++StopVM_timer_Count == 5)
	{
		MMK_StopTimer(StopVM_timer_id);
		StopVM_timer_id = 0;
		ajms_arc_terminateApp();
	}
}

/*****************************************************************************/
//  Description : waiting popup window messaging hanlder
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIJAVA_handleWaitingWinMsg(
                                         MMI_WIN_ID_T     win_id,
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM           param
                                     )
{
    MMI_RESULT_E    result   = MMI_RESULT_TRUE;  
    switch (msg_id)
    {
        case MSG_KEYDOWN_RED:
        case MSG_KEYUP_RED:
        case MSG_KEYLONG_POWER:
        case MSG_KEYUP_POWER:
        case MSG_KEYDOWN_POWER:
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
    }

    return result;
}

/*****************************************************************************/
//  Description :MMIJAVA_stopVM_external,stop jvm by external
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_stopVM_external(void)
{
	
	int32 java_state = 0;

    MMI_STRING_T            wait_text = {0};
    SCI_MEMSET(&wait_text, 0, sizeof(wait_text));	
    MMI_GetLabelTextByLang(TXT_JAVA_WAITQUIT, &wait_text);

	java_state = ajms_arc_getState();
	
	JWE_LOG(MMIJAVA_stopVM_external,("enter java_state=%d", java_state))
		
	if (ajms_arc_getState() == AJSC_VMSTATE_NONE)
	{
		 JWE_LOG(MMIJAVA_stopVM_external, ("ajms_arc_getState=AJSC_VMSTATE_NONE. jvm has been ended and into none state."));
	   	 return;
	}
	else if (ajms_arc_getState() >= AJSC_VMSTATE_STARTING) 
	{
		JWE_LOG(MMIJAVA_stopVM_external, ("ajms_arc_terminateApp"));
		if(StopVM_timer_id) 
	    {
            MMK_StopTimer(StopVM_timer_id);
	    }
		StopVM_timer_id = MMK_CreateTimerCallback(MMIJAVA_STOPVM_TIMER_PEROID,MMIAPIJAVA_StopVmTimer_Process,NULL, SCI_TRUE);
		MMK_StartTimerCallback(StopVM_timer_id, MMIJAVA_STOPVM_TIMER_PEROID, MMIAPIJAVA_StopVmTimer_Process,NULL, SCI_TRUE);
		StopVM_timer_Count = 0;
	    if ( ajms_arc_terminateApp() != AJMS_ERR_NONE ){
		JWE_LOG( MMIJAVA_stopVM_external, ( "ajms_arc_terminateApp fail!!!" ) )
	    }
	    MMIPUB_OpenWaitWin(1,
	        &wait_text,
	        PNULL,
	        PNULL,
	        MMIJAVA_WAIT_WIN_ID,
	        IMAGE_NULL,
	        ANIM_PUBWIN_WAIT,
	        WIN_ONE_LEVEL,
	        MMIPUB_SOFTKEY_NONE,
	        MMIJAVA_handleWaitingWinMsg);
#ifdef MMI_TASK_MANAGER
        MMIAPIJAVA_StopApplet(FALSE);
#endif	

	} 
	else 
	{            
	    JWE_LOG(MMIJAVA_stopVM_external, ("other impossible ! condition")); 
	}

}

/*****************************************************************************/
//  Description :MMIAPIJAVA_stopVM_sync
//  Global resource dependence : none
//  Author: 
//  Note: make jvm to background state
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_stopVM_sync(void)
{
	int32 java_state = 0;

	java_state = ajms_arc_getState();
	
	JWE_LOG(MMIAPIJAVA_stopVM_sync,("enter java_state=%d", java_state))
		
	if (ajms_arc_getState() == AJSC_VMSTATE_NONE)
	{
		 JWE_LOG(MMIAPIJAVA_stopVM_sync, ("ajms_arc_getState=AJSC_VMSTATE_NONE. jvm has been ended and into none state."));
	   	 return;
	}
	else if (ajms_arc_getState() >= AJSC_VMSTATE_STARTING) 
	{		
	    #ifdef MMI_TASK_MANAGER
        MMIAPIJAVA_StopApplet(FALSE);
        #endif
		if ( ajms_arc_terminateApp() != AJMS_ERR_NONE ){
			JWE_LOG( MMIAPIJAVA_stopVM_sync, ( "ajms_arc_terminateApp fail!!!" ) )
		}		
		while(ajms_arc_getState() >= AJSC_VMSTATE_STARTING)
		{
		      SCI_Sleep(1000);
		}		
	 
	} 
	else 
	{            
	    JWE_LOG(MMIAPIJAVA_stopVM_sync, ("other impossible ! condition")); 
	}

}


/*****************************************************************************/
//  Description :MMIJAVA_PauseBackgroundJVM
//  Global resource dependence : none
//  Author: 
//  Note: make jvm to pause in background state
/*****************************************************************************/
PUBLIC void MMIJAVA_PauseBackgroundJVM( void )
{
	int32 gound_state = JkGetVmBgFgState();
	int32 java_state = 0;
	int32 err = 0;
	JWE_LOG(MMIJAVA_PauseBackgroundJVM,("gound_state=%d",gound_state));

	/*设计目的:如果JAVA处于BGE状态,suspend虚拟机,增强前台使用媒体的应用的表现*/
	if (gound_state == JK_BG_VM_STATE_BACKGROUND)
	{
		java_state = ajms_arc_getState();
		JWE_LOG(MMIJAVA_PauseBackgroundJVM, ("ajms_arc_getState = %d",java_state));
        if (java_state == AJSC_VMSTATE_NONE)
        {
			JWE_LOG(MMIJAVA_PauseBackgroundJVM, ("java_state= AJSC_VMSTATE_NONE do nothing" ));
        }
		else if ( java_state == AJSC_VMSTATE_SUSPENDED || java_state == AJSC_VMSTATE_SUSPENDING )
		{
			JWE_LOG(MMIJAVA_PauseBackgroundJVM, ("java has been AJSC_VMSTATE_SUSPENDED or AJSC_VMSTATE_SUSPENDED,do nothing"));
			//JWE_LOG(MMIJAVA_PauseBackgroundJVM,("In BGE mode,customer switch bgPlay PauseBackgroundJVM->ResumeBackgroundJVM->PauseBackgroundJVM very quickly!Kill VM!(current AJSC_VMSTATE_SUSPENDING)" ));
			//err = AamsVmKill( 1 );
			//s_b_force_killed = TRUE;
			JWE_LOG(MMIJAVA_PauseBackgroundJVM, ("err = %d", err));
		}        
		else if ( java_state == AJSC_VMSTATE_RESUMING ||java_state == AJSC_VMSTATE_STARTING ||java_state == AJSC_VMSTATE_DESTROYING || java_state == AJSC_VMSTATE_KILLING )
		{
			JWE_LOG(MMIJAVA_PauseBackgroundJVM,("In BGE mode,customer switch bgPlay PauseBackgroundJVM->ResumeBackgroundJVM->PauseBackgroundJVM very quickly!Kill VM!(current AJSC_VMSTATE_RESUMING)" ));
			err = AamsVmKill( 1 );
			s_b_force_killed = TRUE;
			JWE_LOG(MMIJAVA_PauseBackgroundJVM, ("err = %d", err));
		}     				
		else
		{
			JWE_LOG( MMIJAVA_PauseBackgroundJVM, ( "java is in %d. ",java_state) );//Note:may be in AJSC_VMSTATE_RESUMING
			MMIJAVA_PauseAllMediaPlayer();
			s_b_needStopSuspendStateCheck = FALSE;
			if( ajms_arc_suspendApp() != AJMS_ERR_NONE )
            {
                JWE_LOG( MMIJAVA_PauseBackgroundJVM, ( "ajms_arc_suspendApp fail!!!" ) )
            }
		} 
	}
	

}

/*****************************************************************************/
//  Description :MMIJAVA_ResumeBackgroundJVM
//  Global resource dependence : none
//  Author: 
//  Note: make jvm resume in background state
/*****************************************************************************/
PUBLIC void MMIJAVA_ResumeBackgroundJVM( void )
{
	int32 java_state  = ajms_arc_getState();
	int32 gound_state  = JkGetVmBgFgState();
	JWE_LOG(MMIJAVA_ResumeBackgroundJVM,("java_state = %d  gound_state=%d", java_state,gound_state));
    
	/*设计目的:如果JAVA处于BGE状态,前台应用的媒体资源使用完之后,继续resume JAVA*/	
	if (java_state == AJSC_VMSTATE_SUSPENDED && (gound_state == JK_BG_VM_STATE_BACKGROUND) )
	{
        JWE_LOG(MMIJAVA_ResumeBackgroundJVM,("java is in JK_BG_VM_STATE_BACKGROUND and java_state =  AJSC_VMSTATE_SUSPENDED.  BGPlay resume java"));
        s_b_needStopSuspendStateCheck = TRUE;
        if ( ajms_arc_resumeApp() != AJMS_ERR_NONE )
        {
            JWE_LOG(MMIJAVA_ResumeBackgroundJVM,("ajms_arc_resumeApp fail!!!" ))	
        }

        if ( ajms_arc_backgroundApp() != AJMS_ERR_NONE )
        {//Note:maybe del,and use JkGetVmBgFgState in the all situation(ex: ajms_arc_getState==AJSC_VMSTATE_BACKGROUND)
            JWE_LOG( MMIJAVA_ResumeBackgroundJVM, ( "ajms_arc_backgroundApp fail!!!" ) )
        }
        //AamsVmBackground(1);
	}
	else if (java_state == AJSC_VMSTATE_SUSPENDING && (gound_state == JK_BG_VM_STATE_BACKGROUND) )
	{
		 JWE_LOG(MMIJAVA_ResumeBackgroundJVM,("java is in JK_BG_VM_STATE_BACKGROUND and java_state =  AJSC_VMSTATE_SUSPENDING.  waiting MMIAPIJAVA_StartCheckVmState -> AJSC_VMSTATE_SUSPENDED"));
		 s_b_needStopSuspendStateCheck = FALSE;
		 s_b_bgResume = TRUE;
	}
}

/*****************************************************************************/
//  Description :MMIJAVA_Start_initializeResource,初始化
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_Start_initializeResource(void)
{
   
    MMISET_LANGUAGE_TYPE_E java_language_type = 0;
    JWE_LOG(MMIJAVA_Start_initializeResource,("enter" ))

    MMIAPISET_GetLanguageType(&java_language_type);
    if(java_language_type == MMISET_LANGUAGE_ENGLISH)
    {
        ajms_pcs_setStringTable(1);
    }
    else if(java_language_type == MMISET_LANGUAGE_SIMP_CHINESE)
    {
        ajms_pcs_setStringTable(2);
    }
		
    MMIJAVA_MMAPI_init();

    MMIJAVA_NET_init();
    s_b_bgResume = FALSE;
    s_b_needStopSuspendStateCheck = FALSE;

    //SCI_TRACE_LOW:" MMIJAVA_Start_initializeResource leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_FUNC_1574_112_2_18_2_22_5_6,(uint8*)"");

}



/*****************************************************************************/
//  Description :get current key state
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
PUBLIC uint32 MMIJAVA_GetKeyState(void)
{
    return s_lKeyState;
}
/*****************************************************************************/
//  Description :JVM state notification callback function.
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
LOCAL char* getCurrentVmState() 
{
    int stateCode = ajms_arc_getState();
    switch ( stateCode )
    {
      case AJSC_VMSTATE_NONE:
        return "VMSTATE_NONE"; 
      case AJSC_VMSTATE_STARTING:
        return "VMSTATE_STARTING";
      case AJSC_VMSTATE_RESERVED:
        return "VMSTATE_RESERVED";
      case AJSC_VMSTATE_STARTED:
        return "VMSTATE_STARTED";
      case AJSC_VMSTATE_SUSPENDING:
        return "VMSTATE_SUSPENDING";
      case AJSC_VMSTATE_SUSPENDED:
        return "VMSTATE_SUSPENDED";
      case AJSC_VMSTATE_RESUMING:
        return "VMSTATE_RESUMING";
      case AJSC_VMSTATE_DESTROYING:
        return "VMSTATE_DESTROYING";
      case AJSC_VMSTATE_KILLING:
        return "VMSTATE_KILLING";
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
      case AJSC_VMSTATE_BACKGROUND:
        return "VMSTATE_BACKGROUND";
      case AJSC_VMSTATE_FOREGROUND:
        return "VMSTATE_FOREGROUND";
#endif
      default:
        JWE_LOG(getCurrentVmState,("error state\n" ))
        return "UNKNOWN STATE";
    }
}

LOCAL char* printCurrentVmEvent( int event )
{
    switch ( event )
	{
		case AJMS_VMEVENT_VM_STARTED:
			return "VMEVENT_VM_STARTED";
		case AJMS_VMEVENT_VM_SUSPENDED:
			return "VMEVENT_VM_SUSPENDED";
		case AJMS_VMEVENT_VM_RESUMED:
			return "VMEVENT_VM_RESUMED";
		case AJMS_VMEVENT_VM_STOPPED:
			return "VMEVENT_VM_STOPPED";
		case AJMS_VMEVENT_VM_PAINTREADY:
			return "VMEVENT_VM_PAINTREADY";
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
		case AJMS_VMEVENT_VM_BACKGROUND:
			return "VMEVENT_VM_BACKGROUND";
		case AJMS_VMEVENT_VM_FOREGROUND:
			return "VMEVENT_VM_FOREGROUND";
#endif
		case AJMS_VMEVENT_APP_STARTED:
			return "VMEVENT_APP_STARTED";
		case AJMS_VMEVENT_APP_PAUSED:
			return "VMEVENT_APP_PAUSED";
		case AJMS_VMEVENT_APP_RESUMED:
			return "VMEVENT_APP_RESUMED";
		case AJMS_VMEVENT_APP_DESTROYED:
			return "VMEVENT_APP_DESTROYED";
		case AJMS_VMEXIT_TERMINATED:
			return "AJMS_VMEXIT_TERMINATED";
		default:
		JWE_LOG(printCurrentVmEvent,("special event: %d\n", event ))
			return "SPECIAL EVENT";
 	}
}

#ifdef JAVA_EVENT_ERROR_PROMPT_WIN
PUBLIC void MMIAPIJAVA_jwe_vmErrorHandle(int32 eventType)
{
	
    MMI_TEXT_ID_T      exception_text_id=TXT_NULL;

    switch ( eventType )
    {
    case AJMS_VMEXIT_EXCEPTION_NULL_POINT:
        exception_text_id = TXT_JAVA_VMEXIT_EXCEPTION_NULL_POINT;
        break;     
    case AJMS_VMEXIT_EXCEPTION_ARITHMETIC:
        exception_text_id = TXT_JAVA_VMEXIT_EXCEPTION_ARITHMETIC;
        break;     
    case AJMS_VMEXIT_EXCEPTION_ARRRYINDEX_OUTOFBOUNDS:
        exception_text_id = TXT_JAVA_VMEXIT_EXCEPTION_ARRRYINDEX_OUTOFBOUNDS; 
        break;     
    case AJMS_VMEXIT_EXCEPTION_NO_CLASS_DEF_FOUND:
        exception_text_id = TXT_JAVA_VMEXIT_EXCEPTION_NO_CLASS_DEF_FOUND;
        break;    
    case AJMS_VMEXIT_ERROR_OUTOFMEMORY: 
    case AJMS_VMEXIT_EXCEPTION_OUT_OF_MEMORY:
        exception_text_id = TXT_COMMON_NO_MEMORY;
        break;    		
    case AJMS_VMEXIT_INTERNALERROR_FATAL_ERROR:
        exception_text_id = TXT_JAVA_VMEXIT_INTERNALERROR_FATAL_ERROR;
        break;  
    default:
        exception_text_id = TXT_ERROR_OCCUR;
        break;
    }  

    if(TXT_NULL != exception_text_id)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,exception_text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
    }	
}
#endif

/*****************************************************************************/
//  Description :JVM state notification callback function,jvm用来出来各种jvm级别的事件和消息.
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
PUBLIC void MMIJAVA_jwe_vmEventHandler(AJ_S32 eventType)
{
 #ifndef WIN32   
    const JKT_StdKeyTable stdKeyTable = 
    {
        { 48,  (JKUTF8Char *)"NUM0",   4},
        { 49,  (JKUTF8Char *)"NUM1",   4},
        { 50,  (JKUTF8Char *)"NUM2",   4},
        { 51,  (JKUTF8Char *)"NUM3",   4},
        { 52,  (JKUTF8Char *)"NUM4",   4},
        { 53,  (JKUTF8Char *)"NUM5",   4},
        { 54,  (JKUTF8Char *)"NUM6",   4},
        { 55,  (JKUTF8Char *)"NUM7",   4},
        { 56,  (JKUTF8Char *)"NUM8",   4},
        { 57,  (JKUTF8Char *)"NUM9",   4},
        { 42,  (JKUTF8Char *)"START",  5},
        { 35,  (JKUTF8Char *)"POUND",  5},
        {  0,  JK_NULL,  0},
        {  0,  JK_NULL,  0},
        {  0,  JK_NULL,  0},
        {  0,  JK_NULL,  0},
        { -3,  (JKUTF8Char *)"LEFT",   4},
        { -1,  (JKUTF8Char *)"UP",     2},
        { -4,  (JKUTF8Char *)"RIGHT",  5},
        { -2,  (JKUTF8Char *)"DOWN",   4},
        { -5,  (JKUTF8Char *)"SELECT",  6},
        { -6,  (JKUTF8Char *)"SOFT1",   5},
        { -7,  (JKUTF8Char *)"SOFT2",   5},
        { -23, (JKUTF8Char *)"SOFT3",  5},
        {  0,  JK_NULL,  0},
        { -10, (JKUTF8Char *)"SEND",  4},
        { -8,  (JKUTF8Char *)"CLEAR", 5}
    };
#endif 
    JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[MMIJAVA_jwe_vmEventHandler] VM event %s.\n", printCurrentVmEvent(eventType)))
    JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[MMIJAVA_jwe_vmEventHandler] VM current state  = %s\n", getCurrentVmState()))
        
    switch (eventType) 
    {        
    /* normal JBlend events */
    case AJMS_VMEVENT_VM_STARTED:
        {
            #ifdef DRM_SUPPORT
            const AJ_U8 *       m_jarPath = AJ_NULL;     // merlin
            AJ_S32              error_code;
            uint32              wlength = 0;
            wchar               wfilename[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
            m_jarPath = ajms_ab_getJarPath(g_active_suite_id,&error_code);
            
            if(PNULL != m_jarPath)
            {
                wlength = GUI_UTF8ToWstr(wfilename,MMIFILE_FILE_NAME_MAX_LEN,m_jarPath,strlen(m_jarPath));/*lint !e64*/			
                MMIAPIDRM_ConsumeRights(NULL,wfilename);
            }
            
            #endif

            #ifndef WIN32
            AmSetKeyCodeTable( &stdKeyTable, JK_NULL);
            #endif        

            JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_VM_STARTED.\n"))
        }
        break;

	/*
	After MMIJAVA_jwe_vmEventHandler(AJMS_VMEVENT_VM_SUSPENDED) called:
	1. change ajsc vmstate -> AJSC_VMSTATE_SUSPENDED
	2. call kjava_sys_received to yield kve-task cpu.

	note:
	When ajms_arc_resumeApp called in MMI task:
	1. kjava_sys_send to active kve-task cpu
	2. call AamsVmResume to enqueue the jblend internal event queue
	*/		
    case AJMS_VMEVENT_VM_SUSPENDED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_VM_SUSPENDED.\n"))            
	 	MMIAPIJAVA_SendSignal(MSG_JAVA_VM_SUSPEND_IND,MMIJAVA_WAIT_WIN_ID);	
        break;
        
    case AJMS_VMEVENT_VM_RESUMED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_VM_RESUMED.\n"))                
        break;
		
    case AJMS_VMEVENT_VM_BACKGROUND:
        //jvm is in background = TRUE;
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_VM_BACKGROUND.\n"))
        break;

    case AJMS_VMEVENT_VM_FOREGROUND:
    	//jvm is in background = FALSE;
    	JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_VM_FOREGROUND.\n"))
	    break;
		
    case AJMS_VMEVENT_VM_STOPPED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_VM_STOPPED.\n"))
#ifdef FS_PROFILE
	    if(g_vm_exit <2)
	    {
		    kjava_profiler_endCount(0);
	    }	
		kjava_profiler_dump(0);
		kjava_profiler_dump(1);
		kjava_profiler_dump(2);
		kjava_profiler_dump(3);
		kjava_profiler_dump(4);
		kjava_profiler_dump(5);
		kjava_profiler_dump(6);
       	g_vm_exit = 3;
#endif		
         break;
        
    case AJMS_VMEVENT_VM_PAINTREADY:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_VM_PAINTREADY.\n"))
         break;
        
    case AJMS_VMEVENT_APP_STARTED:
         
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_APP_STARTED.\n"))
         break;
        
    case AJMS_VMEVENT_APP_PAUSED: 
       
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_APP_PAUSED.\n"))
        break;
        
    case AJMS_VMEVENT_APP_RESUMED: 
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_APP_RESUMED.\n"))
        break;
        
    case AJMS_VMEVENT_APP_DESTROYED:  
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_APP_DESTROYED.\n"))
        break;
        
        /* normal AamsVmStartMIDP return values */    
    case AJMS_VMEXIT_OK_VOLUNTARY:  
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEXIT_OK_VOLUNTARY.\n"))     
        break;

    case AJMS_VMEXIT_OK_REQUESTED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEXIT_OK_REQUESTED.\n"))
        break;
        
    case AJMS_VMEXIT_OK_TCKCONTINUE:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEXIT_OK_TCKCONTINUE.\n"))
        break;
        
        /* special VM events in JBlendia, serves as the final VM events be sent */
    case AJMS_VMEXIT_TERMINATED:
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEXIT_TERMINATED.\n"))
        break;
    case AJMS_VMEXIT_ERROR_VMKILLED: //强制关闭
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEXIT_ERROR_VMKILLED.\n"))        
        break;
        /* exceptional AamsVmStartMIDP return values */
    case AJMS_VMEXIT_EXCEPTION_NULL_POINT:
    case AJMS_VMEXIT_EXCEPTION_ARITHMETIC:
    case AJMS_VMEXIT_EXCEPTION_ARRRYINDEX_OUTOFBOUNDS:
    case AJMS_VMEXIT_EXCEPTION_NO_CLASS_DEF_FOUND:
    case AJMS_VMEXIT_ERROR_OUTOFMEMORY: 
    case AJMS_VMEXIT_EXCEPTION_OUT_OF_MEMORY:        
    case AJMS_VMEXIT_INTERNALERROR_FATAL_ERROR: 		
    case AJMS_VMEXIT_ERROR_ILLEGAL_ARGUMENT:
    case AJMS_VMEXIT_ERROR_BAD_STATE:
		
#ifdef JAVA_EVENT_ERROR_PROMPT_WIN		
        //MMIAPIJAVA_jwe_vmErrorHandle(eventType);
        MMIAPIJAVA_SendSignal(MSG_JAVA_OUT_OF_MEMORY,eventType);
#endif
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEXIT_ERROR_VMKILLED.\n"))
        break;
        
    case AJMS_VMEXIT_INTERNALERROR_RUNNING_ALREADY:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEXIT_INTERNALERROR_RUNNING_ALREADY.\n"))
        break;
    case AJMS_TCKEVENT_STARTED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_TCKEVENT_STARTED.\n"))
        break;
        
    case AJMS_TCKEVENT_JAD_DOWNLOADED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_TCKEVENT_JAD_DOWNLOADED.\n"))
        break;
        
    case AJMS_TCKEVENT_JAD_CHECKED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_TCKEVENT_JAD_CHECKED.\n"))
        break;
        
    case AJMS_TCKEVENT_JAR_DOWNLOADED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_TCKEVENT_JAR_DOWNLOADED.\n"))
        break;
        
    case AJMS_TCKEVENT_INSTALLED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_TCKEVENT_INSTALLED.\n"))
       break;
        
    case AJMS_TCKEVENT_VM_TERMINATED:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_TCKEVENT_VM_TERMINATED.\n"))
        break;
        
    case AJMS_TCKEVENT_EXITED_NORMALLY:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_TCKEVENT_EXITED_NORMALLY.\n"))
        break;
        
    case AJMS_TCKEVENT_EXITED_WITH_ERROR:            
#ifdef JAVA_EVENT_ERROR_PROMPT_WIN			
        //MMIAPIJAVA_jwe_vmErrorHandle(eventType);
        MMIAPIJAVA_SendSignal(MSG_JAVA_OUT_OF_MEMORY,PNULL);
#endif
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_TCKEVENT_EXITED_WITH_ERROR.\n"))
        break;
        
    default:            
        JWE_LOG(MMIJAVA_jwe_vmEventHandler,("[jwe_vmEventHandler] unknown VM event %d.\n", eventType))
         break;
    }
}

/*****************************************************************************/
//  Description :JVM state notification callback function.
//  Global resource dependence : none
//  Author: 
//  Note: eventType  The JVM state
/*****************************************************************************/
PUBLIC void MMIJAVA_TCK_vmEventHandler(int32 eventType)
{
#ifndef WIN32    
    const JKT_StdKeyTable stdKeyTable = 
    {
        { 48,  (JKUTF8Char *)"NUM0",   4},
        { 49,  (JKUTF8Char *)"NUM1",   4},
        { 50,  (JKUTF8Char *)"NUM2",   4},
        { 51,  (JKUTF8Char *)"NUM3",   4},
        { 52,  (JKUTF8Char *)"NUM4",   4},
        { 53,  (JKUTF8Char *)"NUM5",   4},
        { 54,  (JKUTF8Char *)"NUM6",   4},
        { 55,  (JKUTF8Char *)"NUM7",   4},
        { 56,  (JKUTF8Char *)"NUM8",   4},
        { 57,  (JKUTF8Char *)"NUM9",   4},
        { 42,  (JKUTF8Char *)"START",  5},
        { 35,  (JKUTF8Char *)"POUND",  5},
        {  0,  JK_NULL,  0},
        {  0,  JK_NULL,  0},
        {  0,  JK_NULL,  0},
        {  0,  JK_NULL,  0},
        { -3,  (JKUTF8Char *)"LEFT",   4},
        { -1,  (JKUTF8Char *)"UP",     2},
        { -4,  (JKUTF8Char *)"RIGHT",  5},
        { -2,  (JKUTF8Char *)"DOWN",   4},
        { -5,  (JKUTF8Char *)"SELECT",  6},
        { -6,  (JKUTF8Char *)"SOFT1",   5},
        { -7,  (JKUTF8Char *)"SOFT2",   5},
        { -23, (JKUTF8Char *)"SOFT3",  5},
        {  0,  JK_NULL,  0},
        { -10, (JKUTF8Char *)"SEND",  4},
        { -8,  (JKUTF8Char *)"CLEAR", 5}
    };
#endif      
    JWE_LOG(jwe_vmEventHandler,("[jwe_vmEventHandler] VM event %d.\n", eventType))
        
    switch (eventType) 
    {        
    /* normal JBlend events */
    case AJMS_VMEVENT_VM_STARTED:

#ifndef WIN32
        AmSetKeyCodeTable( &stdKeyTable, JK_NULL);
#endif        
      
        JWE_LOG(jwe_vmEventHandler,("[jwe_vmEventHandler] AJMS_VMEVENT_VM_STARTED.\n"))                
        break;
        
    
    default:            
        JWE_LOG(jwe_vmEventHandler,("[jwe_vmEventHandler] unknown VM event %d.\n", eventType))
         break;
            
    }
}

PUBLIC uint32 MMIJAVA_Tm2Second( 
                      uint32 tm_sec, 
                      uint32 tm_min, 
                      uint32 tm_hour,
                      uint32 tm_mday,
                      uint32 tm_mon, 
                      uint32 tm_year)
{
    uint32 second_common = 0;

    second_common = MMIAPICOM_Tm2Second(tm_sec, tm_min, tm_hour,
                      tm_mday, tm_mon, tm_year);
    
    /* 取到的秒数为1980年以来的秒数，还需要加上70~80的秒数 */
    return (uint32)(second_common + M_SECOND_1970_TO_1980);
}

PUBLIC uint32 MMIJAVA_GetCurTime(void)
{
    SCI_DATE_T	sys_date = {0};
    SCI_TIME_T	sys_time = {0};
    uint32      second = 0;
    
    
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    second = MMIJAVA_Tm2Second( sys_time.sec, 
        sys_time.min, 
        sys_time.hour,
        sys_date.mday,
        sys_date.mon, 
        sys_date.year);
    
    /*1970年1月1日零点到现在经过的秒数*/ 
    return ( second);
}

/*****************************************************************************/
//  Description :HandleFlashBacklightTimerOut.
//  Global resource dependence : none
//  Author: 
//  Note:handle backlight time out signal
/*****************************************************************************/
LOCAL void HandleFlashBacklightTimerOut( uint32 praram )
{	

    int32           lcd_contrast = 0;
    // int32           current_step = 0;
    uint32          real_contrast  = MMIAPISET_GetRealBacklightValue();

    SCI_DeactiveTimer(s_flash_backlight.flash_timer_id_ptr);
    if(s_flash_backlight.is_backlighton)
    {
    	lcd_contrast = real_contrast;		
    	s_flash_backlight.is_backlighton = FALSE;		
    }
    else
    {		
    	lcd_contrast = real_contrast - 20;
    	s_flash_backlight.is_backlighton = TRUE;
    	s_flash_backlight.flash_count--;
    }	

    if(lcd_contrast>0)
    {
#ifndef WIN32
        LCD_SetBackLightBrightness(lcd_contrast);
#endif
    }
        
    if(s_flash_backlight.flash_count > 0)
    {
        SCI_ChangeTimer(s_flash_backlight.flash_timer_id_ptr, 
                             	     HandleFlashBacklightTimerOut, 
                             	     s_flash_backlight.flash_delay);
        SCI_ActiveTimer(s_flash_backlight.flash_timer_id_ptr);			
    }
    else
    {
        LCD_SetBackLightBrightness(real_contrast);
        if(!MMK_IsTimerActive(MMI_LCD_LIGHT_TIMER_ID))
        {
            MMK_ResumeTimer(MMI_LCD_LIGHT_TIMER_ID);
        }
    }
	
}

/*****************************************************************************/
//  Description :MMIJAVA_FlashBackLightStart.
//  Global resource dependence : none
//  Author: 
//  Note:start backlight timer
/*****************************************************************************/
PUBLIC void MMIJAVA_FlashBackLightStart( uint32 count, uint32 delay )
{
    if(count == 0 || delay == 0)//入参合法性检查
    {
		//SCI_TRACE_LOW:"MMIDEFAULT_FlashBackLight:input param is invalid"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_FUNC_2078_112_2_18_2_22_6_7,(uint8*)"");
        return;
    }
			
	if(!MMIDEFAULT_IsBacklightOn())		
	{			
		MMIDEFAULT_TurnOnBackLight();
	}
	
	s_flash_backlight.flash_count = count;
	s_flash_backlight.flash_delay = delay;
	s_flash_backlight.is_backlighton = TRUE;
	if(NULL == s_flash_backlight.flash_timer_id_ptr)
	{
		s_flash_backlight.flash_timer_id_ptr = SCI_CreatePeriodTimer("FLASH_LCD", 
    															 HandleFlashBacklightTimerOut,
    															 0, 
    															 s_flash_backlight.flash_delay, 
    															 SCI_AUTO_ACTIVATE);
	}
	else
	{
		SCI_ChangeTimer(s_flash_backlight.flash_timer_id_ptr, 
                           			HandleFlashBacklightTimerOut,
                           			s_flash_backlight.flash_delay);
		SCI_ActiveTimer(s_flash_backlight.flash_timer_id_ptr);
	}
	
	if(NULL == s_flash_backlight.flash_timer_id_ptr)
	{
		//SCI_TRACE_LOW:"MMIDEFAULT_FlashBackLight:create timer failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_FUNC_2108_112_2_18_2_22_6_8,(uint8*)"");
		return;
	}
	
	MMK_PauseTimer(MMI_LCD_LIGHT_TIMER_ID);

}

/*****************************************************************************/
//  Description :MMIJAVA_FlashBackLightStop.
//  Global resource dependence : none
//  Author: 
//  Note:stop backlight timer
/*****************************************************************************/
PUBLIC void MMIJAVA_FlashBackLightStop(void)
{
    uint32          real_contrast  = MMIAPISET_GetRealBacklightValue();

    if(NULL != s_flash_backlight.flash_timer_id_ptr)
    {
        if(SCI_IsTimerActive(s_flash_backlight.flash_timer_id_ptr))
        {		
            SCI_DeactiveTimer(s_flash_backlight.flash_timer_id_ptr);
        }

        SCI_DeleteTimer(s_flash_backlight.flash_timer_id_ptr);
        SCI_MEMSET(&s_flash_backlight, 0, sizeof(FLASH_BACKLIGHT_T));

        LCD_SetBackLightBrightness(real_contrast);
        if(!MMK_IsTimerActive(MMI_LCD_LIGHT_TIMER_ID))
        {
            MMK_ResumeTimer(MMI_LCD_LIGHT_TIMER_ID);
        }
    }
}

/*****************************************************************************/
//  Description : JkGetMjb2Version
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL const char* JkGetMjb2Version(void)
{
    sprintf(s_java_version,"%s\nbuildtime:%s\n",ajsc_getJBlendVersion(),bldTime_cldc);

    return s_java_version;
}

/*****************************************************************************/
// 	Description : MMIJAVA_CalculateActualVolume
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_CalculateActualVolume(int32 mastervolume ,int32 playervolume)
{
	int32 playervolume_level = 0;
	uint8 actual_volume = 0;

	JWE_LOG(MMIJAVA_CalculateActualVolume,("enter mastervolume = %d playervolume = %d", mastervolume,playervolume));
	if( mastervolume>6 || mastervolume<0 || playervolume>127 || playervolume<0 )
	{
		JWE_LOG(MMIJAVA_CalculateActualVolume,("input volume error return 0!"));
		return 0;
	}
	
	if(playervolume ==126 || playervolume==127)
	{
		playervolume_level = 5;
	}
	else
	{
		if(playervolume%25 == 0)
			playervolume_level = playervolume/25;
		else
			playervolume_level = playervolume/25+1;
	}
	
	JWE_LOG(MMIJAVA_CalculateActualVolume,("mastervolume=%d playervolume_level=%d",mastervolume,playervolume_level));
	actual_volume = g_actualVolumeMappingTable[playervolume_level][mastervolume];
	JWE_LOG(MMIJAVA_CalculateActualVolume,("leave return actual_volume=%d",actual_volume));
	return actual_volume;

}

/*****************************************************************************/
// 	Description : MMIJAVA_AdjustVolume
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_AdjustVolume(int32 step)
{
	//int32   actual_volume = 0;
	int32   cur_player_volume = 0;


	JWE_LOG(MMIJAVA_AdjustVolume,("MMIJAVA_AdjustVolume step = %d",step ));
	g_java_platform_Volume = g_java_platform_Volume + step;

    if(g_java_platform_Volume >= MMIJAVA_PLATFORM_VOLUME_MAX)
    {
        g_java_platform_Volume = MMIJAVA_PLATFORM_VOLUME_MAX;
    }

    if(g_java_platform_Volume <= 0)
    {
        g_java_platform_Volume = 0;
    }
    
	JWE_LOG(MMIJAVA_AdjustVolume,("g_masterVolume now is  = %d",g_java_platform_Volume ));
	if(MMIJAVA_getPlayingPlayerVolume(&cur_player_volume))
	{
		JWE_LOG(MMIJAVA_AdjustVolume,("current playing player's volume is cur_player_volume=%d",cur_player_volume ));
		AudioClient_setActualVolume(MMIJAVA_CalculateActualVolume(g_java_platform_Volume,cur_player_volume));
		
	}
	else
	{
		JWE_LOG(MMIJAVA_AdjustVolume,("No java player is in playing" ));
	}
		
}
/*****************************************************************************/
//  Description : MMIJAVA_GetPlatformVolume
//  Global resource dependence : none
//  Author: 
//  Note: MMIAUDIO_PauseOtherBgPlay,when java play music
/*****************************************************************************/
PUBLIC int32 MMIJAVA_GetPlatformVolume(void)
{

    return g_java_platform_Volume;
}
/*****************************************************************************/
//  Description : mediaCtrl_lock_device_multimedia
//  Global resource dependence : none
//  Author: 
//  Note: MMIAUDIO_PauseOtherBgPlay,when java play music
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_lock_device_multimedia(void)
{
    JWE_LOG(MMIJAVA_lock_device_multimedia, ("enter")); 

//    MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_JAVA, FALSE);
// 	MMIAUDIO_PauseOtherBgPlay(MMIBGPLAY_TYPE_JAVA_BG);
    
    return TRUE;
}


/*****************************************************************************/
//  Description : mediaCtrl_unlock_device_multimedia
//  Global resource dependence : none
//  Author: 
//  Note: MMIAUDIO_ResumeOtherBgPlay,when java play music
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_unlock_device_multimedia(void)
{
    JWE_LOG(MMIJAVA_unlock_device_multimedia, ("enter")); 

//    MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_JAVA, TRUE);
//    MMIAUDIO_ResumeOtherBgPlay(MMIBGPLAY_TYPE_JAVA_BG);
    
    return TRUE;    
}

/**--------------------------------------------------------------------------*
**                      EXTERNAL CALL  FUNCTION DEFINITION                  **
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : MMIAPIJAVA_GetVersion
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC const  char* MMIAPIJAVA_GetVersion( void )
{
	return JkGetMjb2Version();
}

/****************************************************************************
*函数定义:MMIAPIJAVA_JBlendiaAjmsFinalize
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
PUBLIC void MMIAPIJAVA_JBlendiaAjmsFinalize( void )
{   
    Ajava_Finalize_ia();           
}

/****************************************************************************
*函数定义:MMIAPIJAVA_JBlendiaAjmsInit
*函数说明:
*参数说明:
*返回值:
*****************************************************************************/
PUBLIC void MMIAPIJAVA_JBlendiaAjmsInit( void )
{ 
	JWE_LOG(MMIAPIJAVA_JBlendiaAjmsInit, ("enter"));
    Ajava_Initiliztion_ia();      
    MMIJAVA_initPreinstallID();
	JWE_LOG(MMIAPIJAVA_JBlendiaAjmsInit, ("leave"));
}

/****************************************************************************
*函数定义:MMIAPIJAVA_InitModule
*函数说明:
*参数说明:
*返回值:
*****************************************************************************/
PUBLIC void MMIAPIJAVA_InitModule(void)
{
    MMIJAVA_RegMenuGroup();  
    MMIJAVA_RegJavaNv();   
    MMIJAVA_RegWinIdNameArr();
#ifdef MMI_TASK_MANAGER
    MMIJAVA_RegAppletInfo();
#endif    
}

/*****************************************************************************/
//  Description :MMIAPIJAVA_Exit_FinalizeResource
//  Global resource dependence : none
//  Author: 
//  Note: finizie resource of java
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_Exit_FinalizeResource(void)
{
    JAVA_INSTALL_INFO_T*      install_info_ptr = MMIJAVA_GetIntallInfo();
    
	JWE_LOG(MMIAPIJAVA_Exit_FinalizeResource,("enter" ));
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
	if (ajms_arc_getState() != AJSC_VMSTATE_NONE)//如果jvm还有状态，不释放资源
        return;
#endif

    if(StopVM_timer_id != 0)
    {
        MMK_StopTimer(StopVM_timer_id);
        StopVM_timer_id = 0;        
    }
    
    if(VMState_timer_id != 0)
    {
        MMK_StopTimer(VMState_timer_id);
        VMState_timer_id = 0;
    }
    
#ifndef WIN32 
        CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif

	MMIJAVA_NET_fini();

	MMIJAVA_MMAPI_fini();

	Ajava_Finalize_UDP();    

	Ajava_Finalize_COM();   

    MMK_CloseWin( MMIJAVA_AMS_MIDLETLIST_WIN_ID ); 
    MMK_CloseWin(MMIJAVA_MIDLET_WIN_ID);
    MMK_CloseWin(MMIJAVA_RESUME_DIALOG_WIN_ID);
    MMK_CloseWin(MMIJAVA_PAUSE_DIALOG_WIN_ID);
    MMK_CloseWin(MMIJAVA_WAIT_WIN_ID);
    
    g_jvm_is_running_by_mmi = FALSE;

    if (PNULL != pAppEx1Jad)
    {
        SCI_FREE(pAppEx1Jad);
    }
    
    if (PNULL != pAppEx1Jar)
    {
        SCI_FREE(pAppEx1Jar);
    }
    
    if(install_info_ptr->jad_filename_ptr)
    {
        SCI_FREE(install_info_ptr->jad_filename_ptr);
    }
    
    if(install_info_ptr->jar_filename_ptr)
    {
        SCI_FREE(install_info_ptr->jar_filename_ptr);
    }
    MAIN_UpdateIdleJavaState(FALSE);
    MMIJAVA_unlock_device_multimedia();//release media control

	MMIJAVA_ContinueInstall();
	MMIJAVA_ContinuePush();
	//SCI_TRACE_LOW:" MMIJAVA_Exit_FinalizeResource leave"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_FUNC_2381_112_2_18_2_22_6_9,(uint8*)"");
		
}


/*****************************************************************************/
//  Description : java task send msg to app task
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SendSignal(uint32 sig_id, uint32 param)
{
	JAVA_APP_SIGNAL *signal = PNULL;

	MmiCreateSignal(sig_id, sizeof(JAVA_APP_SIGNAL), (MmiSignalS**)&signal);
	signal->Sender = P_APP;
	signal->content = param;
	//SCI_TRACE_LOW:"MMIAPIJAVA_SendSignal sig_id = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_FUNC_2397_112_2_18_2_22_6_10,(uint8*)"d",  sig_id);
	MmiSendSignal(P_APP, (MmiSignalS*)signal);
}
/*****************************************************************************/
//  Description : MMIAPIJAVA_Finalize_Special
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Finalize_Special(void)
{
   return Ajava_Finalize_Special();
}
/***************************************************************************/
// Description:free MMIAPIJAVA_socket_polling_timeOut_process
// Author:huangjc
// Note:
//*************************************************************************/
PUBLIC int32  MMIAPIJAVA_socket_polling_timeOut_process(int32 timerID)
{
   kjava_socket_polling_timeOut_process((uint8)timerID, NULL);
   return 1;
}

/*****************************************************************************/
//  Description:  stop current operation when start udisk 
//  Author     :  murphy.xie
//  Param :     void
//  Return:     void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOperaStartUDisk(void)
{
    MMIAPIJAVA_ExitJAVA();
    MMIAPIJAVA_Finalize_Special();
}


/*****************************************************************************/
//  Description:  stop current operation when sd plug
//  Author     :  murphy.xie
//  Param :     void
//  Return:     void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOperaSDPlug(void)
{
    MMIAPIJAVA_ExitJAVA();
}

/*****************************************************************************/
//     Description : if the port is for java
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_CheckIfJavaPort(int32 port, SMS_CALLBACK* javasms_func_ptr, BOOLEAN callback_flag)
{

    MMISMS_JAVA_GLOBAL_T*   java_data_ptr = MMIAPIJAVA_GetSmsJavaData();
    uint16                  index         = 0;
    BOOLEAN                 result        = FALSE;

    for (index = 0; index < MMISMS_JAVACBF_MAX_NUM; index++)
    {
    	//SCI_TRACE_LOW:"IsJavaPort() port is %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_FUNC_2453_112_2_18_2_22_6_11,(uint8*)"d", java_data_ptr->java_cbf[index].port );
        if (java_data_ptr->java_cbf[index].port == port && java_data_ptr->java_cbf[index].jsCBF != PNULL)
        {
            if (callback_flag == TRUE)
            {
                *javasms_func_ptr = java_data_ptr->java_cbf[index].jsCBF;
                 //SCI_TRACE_LOW:"IsJavaPort() port is %d, cb is %d"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_FUNC_2459_112_2_18_2_22_6_12,(uint8*)"dd", port, (int32)(*javasms_func_ptr));
            }

            result = TRUE;
            break;
        }
    }
    return result;

}

/*****************************************************************************/
//  Description : MMIAPIJAVA_StopAudio
//  Global resource dependence :stop java audio and also stop jvm
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_StopAudioAndJVM(void)
{
    MMIJAVA_PauseAllMediaPlayer();
    MMIAPIJAVA_ExitJAVA();
    MMIJAVA_unlock_device_multimedia();
    return TRUE;
}

/*****************************************************************************/
//  Description : MMIAPIJAVA_IfUseAudioDevice
//  Global resource dependence :MMIAPIJAVA_IfUseAudioDevice
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IfUseAudioDevice(void)
{
    BOOLEAN result = FALSE;
    
    if(MMIJAVA_AudioClientGetActivePlayerID() > 0)
    {
        result = TRUE;
    }
    
    return result;
}
#endif
#endif

