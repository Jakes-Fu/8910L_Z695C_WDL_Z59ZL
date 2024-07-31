/******************************************************************************/
//File Name:        McfTrace.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide simple timer interface for mcare
//Modifed:          nicklei 26/09/2011
//Modifed:          wilsonzhu 01/10/2011
/******************************************************************************/
#ifndef _MCFTRACE_H_
#define _MCFTRACE_H_
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/ 
#include "madpconfig.h"
#include "mcfcommon.h"

 /**---------------------------------------------------------------------------*
 **                         DEFINE                                    *
 **---------------------------------------------------------------------------*/

#define LOG_FILE_PATH	L"mcarelog\\log.txt" //mcare所在存储卡根目录下
#define LOG_DIR_NAME	L"mcarelog"
#define LOG_FILE_NAME	L"log.txt"
#define LOG_STR_Slash	L"\\"

#ifdef MCR_TRACE_DEBUG
#define USE_TRACE  (1) // (0) or (1)
#else
#define USE_TRACE  (0) // (0) or (1)
#endif



extern MINT _vsnwprintf_MCR(MWCHAR *buf, MUINT cnt, const MWCHAR *fmt, va_list args);
#define McfVsnwprintf _vsnwprintf_MCR
/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
 
//---------------------------------------------------------------------------
//FUNCTION: McfTrace_TraceFormatA
//Descrption:log MCAHR型,以log输出，需要展讯的工具支持
//Param:
//  IN:format, ...格式化参数
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTrace_TraceFormatA(const MCHAR* format, ...);//平台trace实现，支持ANSI 字符串数据输入,如 McfTrace_TraceFormatA("%s","log me");

//---------------------------------------------------------------------------
//FUNCTION: McfTrace_TraceFormat
//Descrption:log MWCHAR型,以log输出，需要展讯的工具支持
//Param:
//  IN:format, ...格式化参数
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTrace_TraceFormat(const MWCHAR* format, ...);//平台trace实现，支持UCS2字符串数据输入 如 McfTrace_TraceFormat("%s",L"log me");

//---------------------------------------------------------------------------
//FUNCTION: McfTrace_LogFormatA
//Descrption:MCHAR型,以文件输出，非常降低效率
//Param:
//  IN:format, ...格式化参数
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTrace_LogFormatA(const MCHAR *format, ...);//log记录到文件，ANSI

//---------------------------------------------------------------------------
//FUNCTION: McfTrace_LogFormat
//Descrption:MWCHAR型,以文件输出，非常降低效率
//Param:
//  IN:format, ...格式化参数
//  OUT:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfTrace_LogFormat(const MWCHAR* format, ...);//log记录到文件，UCS2
MVOID McfTrace_AssertFail(MCHAR* pExpr, MCHAR* pFileName, MINT nLine);

MVOID McfTraceInit(MVOID);
MVOID McfTraceDeinit(MVOID);


/*****************************************************************************
*通道log的通道(channel)
*****************************************************************************/
typedef enum 
{
	MLOG_NONE = 0,
	MLOG_SDKMEMORY,// 1
	MLOG_SDKFILE,// 2
	MLOG_SDKAPP,// 3
	MLOG_SDKGDI,//4
	MLOG_SDKAUDIO,//5
	MLOG_SDKSOCKET,//6
	MLOG_SDKSMS,//7
	MLOG_SDKTELEPHONE,//8
	MLOG_SDKPHONEBOOK,//9
	MLOG_SDKMISC,//10
	MLOG_SDKLOG,//11
	MLOG_SDKINPUT,//12
	MLOG_SDKUNZIP,//13
	MLOG_MGRKERN,//14
	MLOG_MGRNET,//15
	MLOG_MGRTMC,//16
	MLOG_MGRUI,//17
	MLOG_APPLICATION  ,//扩展，应用使用//18
	MLOG_FIRMFLOW,//扩展，应用使用//19
	MLOG_FIRMHTTP,//固化层http协议//20
	MLOG_SDKKEY,
	MLOG_SDKPEN,
	MLOG_SDKSCREEN,	
	MLOG_SDKTIMER,
	MLOG_SDKINTERFACE,
	MLOG_SDKIDLE,
	MLOG_CHANNEL_MAX
}TraceChannelEnum;


#define MLOG_INFO   ALOG_INFO
#define MLOG_WARN   ALOG_WARN
#define MLOG_ERROR  ALOG_ERROR
#define MLOG_LEVEL_MAX  ALOG_LEVEL_MAX


#define TRACE_CHANNEL_NUM					MLOG_CHANNEL_MAX
#define TRACE_MAX_LOG_LENGTH				256
#define TRACE_MAX_NAME_LENGTH				15
#define ATRACE_MAX_CHANNEL_INFO_STR_LENGTH  (32)


typedef struct  
{
	MBOOL m_bIsInitialized;    
	MCHAR m_ChannelControl[TRACE_CHANNEL_NUM];
	MCHAR m_ChannelLevel[TRACE_CHANNEL_NUM];
	MCHAR m_pTmpBuf[TRACE_MAX_LOG_LENGTH];
}MTrace;


#define TRACE8		McfTrace_TraceFormatA
#define LOG8		McfTrace_LogFormatA

#ifdef __MCARE_SEPERATE_NET_TASK__
#define MCF_TRACE_MOD   MOD_MCARE
#else
#define MCF_TRACE_MOD   MOD_SMS
#endif

#if (USE_TRACE == 1)
#define MTRACEANDLOG8(_x_)	TRACE8 _x_
#else
#define MTRACEANDLOG8(_x_)	\
{							\
	TRACE8 _x_;				\
	LOG8 _x_;				\
}
#endif

MVOID McfLog_PrintMethod(MCHAR level, MCHAR channel, const MCHAR *pFormat, ...);


//API LOG控制
#ifdef MFIRM_DEBUG
#define MCR_DEBUG_PORTING_APIAUDIO
#define MCR_DEBUG_PORTING_APISOCKET
#define MCR_DEBUG_PORTING_APIMEMORY
//#define MCR_DEBUG_PORTING_APIGDI
#define MCR_DEBUG_PORTING_APIFILE
#define MCR_DEBUG_PORTING_APIMISC
#define MCR_DEBUG_PORTING_APISMS
#define MCR_DEBUG_PORTING_TIMER
#define MCR_DEBUG_LOGIC_FLOW
#define MCR_DEBUG_LOGIC_HTTP
#define MCR_DEBUG_PORTING_KEY
#define MCR_DEBUG_PORTING_PEN
#define MCR_DEBUG_PORTING_SCREEN
#define MCR_DEBUG_PORTING_TELEPHONE
#define MCR_DEBUG_PORTING_INTERFACE
#define MCR_DEBUG_PORTING_APIIDLE
#define MCR_DEBUG_PORTING_PHONEBOOK
#endif


//IDLE
#ifdef MCR_DEBUG_PORTING_APIIDLE
#define MCFAPI_IDLE_PRINT   McfLog_Print
#define MCFIDLE_TRACE    McfTrace_TraceFormatA
#else
#ifdef WIN32
#define MCFAPI_IDLE_PRINT
#define MCFIDLE_TRACE    
#else
#define MCFAPI_IDLE_PRINT(...)
#define MCFIDLE_TRACE(...)
#endif
#endif


//AUDIO
#ifdef MCR_DEBUG_PORTING_APIAUDIO
#define MCFAPI_AUDIO_PRINT   McfLog_Print
#define MCFAPI_TRACE McfTrace_TraceFormatA
#else
#ifdef WIN32
#define MCFAPI_AUDIO_PRINT
#else
#define MCFAPI_AUDIO_PRINT(...)
#endif
#endif

//SOCKET
#ifdef MCR_DEBUG_PORTING_APISOCKET
#define MCFAPI_SOCKET_PRINT   McfLog_Print
#define MCFSOC_TRACE    McfTrace_TraceFormatA
#else
#ifdef WIN32
#define MCFAPI_SOCKET_PRINT
#define MCFSOC_TRACE    
#else
#define MCFAPI_SOCKET_PRINT(...)
#define MCFSOC_TRACE(...)
#endif
#endif

//MEMORY
#ifdef MCR_DEBUG_PORTING_APIMEMORY
#define MCFAPI_MEMORY_PRINT   McfLog_Print
#else
#ifdef WIN32
#define MCFAPI_MEMORY_PRINT
#else
#define MCFAPI_MEMORY_PRINT(...)
#endif
#endif

//GDI
#ifdef MCR_DEBUG_PORTING_APIGDI
#define MCFAPI_GDI_PRINT   McfLog_Print
#else
#ifdef WIN32
#define MCFAPI_GDI_PRINT
#else
#define MCFAPI_GDI_PRINT(...)
#endif
#endif

//FILE
#ifdef MCR_DEBUG_PORTING_APIFILE
#define MCFAPI_FILE_PRINT   McfLog_Print
#define MCFAPI_FILE_TRACE   McfTrace_TraceFormatA
#else
#ifdef WIN32
#define MCFAPI_FILE_PRINT
#define MCFAPI_FILE_TRACE
#else
#define MCFAPI_FILE_PRINT(...)
#define MCFAPI_FILE_TRACE(...)
#endif
#endif

//MISC
#ifdef MCR_DEBUG_PORTING_APIMISC
#define MCFAPI_MISC_PRINT   McfLog_Print
#else
#ifdef WIN32
#define MCFAPI_MISC_PRINT
#else
#define MCFAPI_MISC_PRINT(...)
#endif
#endif

//SMS
#ifdef MCR_DEBUG_PORTING_APISMS
#define MCFAPI_SMS_PRINT	McfLog_Print
#else
#ifdef WIN32
#define MCFAPI_SMS_PRINT
#else
#define MCFAPI_SMS_PRINT(...)
#endif
#endif

//TIMER
#ifdef MCR_DEBUG_PORTING_TIMER
#define MCFAPI_TIMER_PRINT		McfLog_Print
#else
#ifdef WIN32
#define MCFAPI_TIMER_PRINT
#else
#define MCFAPI_TIMER_PRINT(...)
#endif
#endif


//LOGIC
#ifdef MCR_DEBUG_LOGIC_FLOW
#define MCFLOGIC_FLOW_PRINT		McfLog_PrintMethod
#else
#ifdef WIN32
#define MCFLOGIC_FLOW_PRINT     
#else
#define MCFLOGIC_FLOW_PRINT(...)     
#endif
#endif


#if 0
//HTTP
#ifdef MCR_DEBUG_LOGIC_HTTP
#define MCFLOGIC_HTTP_PRINT		McfLog_PrintMethod
#else
#ifdef WIN32
#define MCFLOGIC_HTTP_PRINT     
#else
#define MCFLOGIC_HTTP_PRINT(...)     
#endif
#endif
#endif//#if 0

//KEY
#ifdef MCR_DEBUG_PORTING_KEY
#define MCFAPI_KEY_PRINT		McfLog_PrintMethod
#else
#ifdef WIN32
#define MCFAPI_KEY_PRINT     
#else
#define MCFAPI_KEY_PRINT(...)     
#endif
#endif

//PEN
#ifdef MCR_DEBUG_PORTING_PEN
#define MCFAPI_PEN_PRINT		McfLog_PrintMethod
#else
#ifdef WIN32
#define MCFAPI_PEN_PRINT     
#else
#define MCFAPI_PEN_PRINT(...)     
#endif
#endif

//SCREEN
#ifdef MCR_DEBUG_PORTING_SCREEN
#define MCFAPI_SCREEN_PRINT		McfLog_PrintMethod
#else
#ifdef WIN32
#define MCFAPI_SCREEN_PRINT     
#else
#define MCFAPI_SCREEN_PRINT(...)     
#endif
#endif

//INTERFACE
#ifdef MCR_DEBUG_PORTING_INTERFACE
#define MCFAPI_INTERFACE_PRINT		McfLog_PrintMethod
#define MCFAPI_INTERFACE_TRACE   McfTrace_TraceFormatA
#else
#ifdef WIN32
#define MCFAPI_INTERFACE_PRINT
#define MCFAPI_INTERFACE_TRACE
#else
#define MCFAPI_INTERFACE_PRINT(...)
#define MCFAPI_INTERFACE_TRACE(...)
#endif
#endif


//PHONEBOOK
#ifdef MCR_DEBUG_PORTING_PHONEBOOK
#define MCFAPI_PB_PRINT		McfLog_PrintMethod
#else
#ifdef WIN32
#define MCFAPI_PB_PRINT     
#else
#define MCFAPI_PB_PRINT(...)     
#endif
#endif

//TELEPHONE
#ifdef MCR_DEBUG_PORTING_TELEPHONE
#define MCFAPI_CALL_PRINT		McfLog_PrintMethod
#else
#ifdef WIN32
#define MCFAPI_CALL_PRINT     
#else
#define MCFAPI_CALL_PRINT(...)     
#endif
#endif

#ifdef MCR_LOG_DEBUG
#define McfLog_Print	McfLog_PrintMethod
#else
#ifdef WIN32
#define McfLog_Print
#else
#define McfLog_Print(...)
#endif
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif//_MCFTRACE_H_
