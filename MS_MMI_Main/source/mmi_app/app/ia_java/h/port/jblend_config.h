#ifndef JBLEND_CONFIG_HEADER
#define JBLEND_CONFIG_HEADER

#ifndef IA_JAVA_SUPPORT_LOW_MEMORY
#define JBLENDIA_SUPPORT_BACKGROUND_VM
#endif

/*AUDIO_USEFILE must be defined in Spreadtrum 09B w1020. Avoid VM crash for null pointer in GC*/
#define AUDIO_USEFILE

/*MEDIA_USE_ONE_AUDIO_HANDLE must be defined in Spreadtrum 09B w1020.  TONE_USE_JBLEND_PARSER Use Tone Sequence Parser Library Service Functions in MJB2 (tone->midi)*/
#define TONE_USE_JBLEND_PARSER

/*MEDIA_USE_ONE_AUDIO_HANDLE must be defined in Spreadtrum 09B w1020. Only one  mmi audio handle(HAUDIO) for "MMIAUDIO_JAVA" in spreadtrum ,please see Mmiaudio.c
PS:Spreadtrum not support audio mixing.
*/
#define MEDIA_USE_ONE_AUDIO_HANDLE

/*JWE_LOG use file trace*/
//#define USE_LOGFILE
#define SELECT_INSTALL_DISK_SUPPORT

#if defined(PLATFORM_SC8800G)		
#define CMCC_JAVA_V41_SUPPORT
//降低飞信待机功耗使用
#define JAVA_LOW_POWER_ENABLE  
#endif

/*2010-07-16 hanker.zhou modify for CETS error handle win issue begin*/			
//java event error prompt win
#define JAVA_EVENT_ERROR_PROMPT_WIN
/*2010-07-16 hanker.zhou modify for CETS error handle win issue end*/	

//#define  HIDE_PREINSTALL_IN_ITEMLIST_HUAWEI
/*GFX Image Decode Switch,depend on ajsc option*/
#define GFX_USE_EXTERNAL_JPEG_DECODER
#define GFX_USE_EXTERNAL_GIF_DECODER
#define GFX_USE_EXTERNAL_PNG_DECODER
#define  JAVA_DM_SUPPORT
#define JAVA_SUPPORT_SHRINK_RAM

#define TRUSTED_MIDLET_OVER_MINISSL

#define MMI_JAVA_NW_V2   //Ivy
/*****************************************************************************/
//  Description:   the following enum is used for pause and resume by other module, 
//                      like cc alarm CT UDISK SD and so on.
//  Author     :   Hanker.Zhou 2010/06
//  Param
//       module: [in] module tag to pause java
/*****************************************************************************/
typedef enum
{
    JAVA_PAUSE_NONE=0,   // none module paused java
    JAVA_PAUSE_BY_CC,    // cc mt processing
    JAVA_PAUSE_BY_ALARM, // alarm incoming
    JAVA_PAUSE_BY_CT,//countedtime arrive        
    JAVA_PAUSE_BY_UDISK, // udisk plus in
    JAVA_PAUSE_BY_SD,    //
    JAVA_PAUSE_MAX
}MMIJAVA_PAUSE_TYPE_E;

#define JAVA_SUPPORT_DIRECTLY_RUN //murphy.xie  是否支持安装完成后直接提示运行
#define JAVA_INSTALL_FILE_READER //murphy.xie  是否采用file reader的方式进行安装
#define JAVA_SUPPORT_SYSTEM_RESET_FUNCTION //murphy.xie  是否支持重置系统

#ifdef IA_JAVA_SUPPORT_LOW_MEMORY //128x32 norflash情况下，使用5425png优化
#define SUPPORT_5425
#endif

#ifdef IA_JAVA_SUPPORT_LOW_MEMORY
#define REMOVE_PAUSE_RESUME_DIALOG
#endif

//#define JAVA_SUPPORT_MEDIA_RECORD //FOR CETS4.1
//#define JAVA_SUPPORT_MEDIA_CAMERA //FOR CETS4.1


#ifdef MMI_PDA_SUPPORT
#if defined (MAINLCD_SIZE_320X480) ||defined (MAINLCD_SIZE_240X400)||defined (MAINLCD_SIZE_240X320)
#define VIRTUAL_KEY_BOARD 
#endif
#endif

#endif

