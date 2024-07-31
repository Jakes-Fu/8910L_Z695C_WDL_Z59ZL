/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "mmi_app_opera4_trc.h"
#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"
#include "sfs.h"
#include "stdarg.h"
#include "gui_ucs2b_converter.h"
#ifdef PLATFORM_SC6600L
#include "bus_monitor.h"
#else
#include "busmonitor_drvapi.h"
#endif

static void OD_InitLogFile(void);
static void OD_CloseLogFile(void);

/** Please dont' put it into the global context*/
static OD_TaskID s_MMI_TaskID = OD_INVALID_TASKID;

void OD_InitDebugger(void)
{
	if(s_MMI_TaskID == OD_INVALID_TASKID)
	{
		s_MMI_TaskID = OD_GetCurrentTaskID();
	}

#ifdef OD_TWEAK_LOG_TO_FILE
	OD_InitLogFile();
#endif /* OD_TWEAK_LOG_TO_FILE */
}

void OD_DeinitDebugger(void)
{
	s_MMI_TaskID = OD_INVALID_TASKID;

#ifdef OD_TWEAK_LOG_TO_FILE
	OD_CloseLogFile();
#endif /* OD_TWEAK_LOG_TO_FILE */
}

void OD_Assert(const char * exp, const char * file, int line)
{
	/* SPRD document suggest we use SCI_ASSERT,  but we can't use this MACRO for core code,
	the filename and line are passed in this function, so we can use SCI_Assert instead.
	Following macro structure is copied from os_api.h.
	*/
#ifdef _WIN_LOG_TOOL
	assert(!exp);
#else
	SCI_Assert((void*)exp, (void*)file, (uint32)line);
#endif  // End of _WIN_LOG_TOOL
}

static void OD_TraceV(OD_TRACE_LEVEL level, const char *fmt, va_list ap)
{
	//Trace format: "op:<task id>:<tick, optional>";
	int prefix_len = 5;
	char buf[OD_TRACE_BUF_SIZE];    // ARRAY OK 2011-05-18 pingany
	char task_name = 'c';           // Task name: Core/MMI
	OD_TaskID task_id = OD_GetCurrentTaskID();
	/* FIXME, currently, no way to get core task id, so I print 'm' if in mmi,
	otherwise, I print 'c'
	*/
	if(task_id == s_MMI_TaskID)
		task_name = 'm';

	OD_ASSERT(prefix_len < OD_TRACE_BUF_SIZE);
	od_snprintf(buf, OD_TRACE_BUF_SIZE -1, "op:%c:0000", task_name);

	//ADS and WIN both has _vsnprintf.
	od_vsnprintf(buf + prefix_len, OD_TRACE_BUF_SIZE - prefix_len - 1, fmt, ap);
	buf[OD_TRACE_BUF_SIZE-1] = 0;

#ifdef WIN32
	/* The spreadtrum simulator will crash when call SCI_TRACE_XXX("%s", "%s"). */
	OutputDebugStringA(buf);
	OutputDebugStringA("\n");
#else
	switch(level)
	{
	case OD_TRACE_LEVEL_FATAL:
		SCI_TRACE_ERROR("%s", buf);
		break;
	case OD_TRACE_LEVEL_HIGH:
		SCI_TRACE_HIGH("%s", buf);
		break;
	case OD_TRACE_LEVEL_MID:
		//SCI_TRACE_MID:"%s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OPDEVDEBUG_93_112_2_18_12_21_28_0,(uint8*)"s", buf);
		break;
	case OD_TRACE_LEVEL_LOW:
	default:
		//If level is not anyone, treat it as LOW, but not error, since the caller want to trace.
		//SCI_TRACE_LOW:"%s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,OPDEVDEBUG_98_112_2_18_12_21_28_1,(uint8*)"s", buf);
		break;
	}
#endif
}

OD_STATUS OD_Trace(OD_TRACE_LEVEL level, const char *fmt, ...)
{
#ifdef _DEBUG
	va_list ap;
	va_start(ap, fmt);
	OD_TraceV(level, fmt, ap);
	va_end(ap);
#endif
	return OD_SUCCESS;
}

OD_STATUS OD_TraceWString(OD_TRACE_LEVEL level, const OD_WCHAR* wstr)
{
#ifdef _DEBUG
	char buf[OD_TRACE_BUF_SIZE];
	int utf8_len;
	OD_ASSERT(wstr);
	utf8_len = GUI_WstrToUTF8(buf, OD_TRACE_BUF_SIZE - 1, wstr, od_wcslen(wstr));
	OD_ASSERT(utf8_len >= 0 && utf8_len < OD_TRACE_BUF_SIZE);
	buf[utf8_len] = 0;
	return OD_Trace(level, "%s", buf);
#else
	return OD_SUCCESS;
#endif /* _DEBUG */
}

#if defined(_MSC_VER) && _MSC_VER < 1300

void OD_TraceDummy(OD_TRACE_LEVEL level, const char *format, ...)
{
	(level);
	(format);
}

void OD_TRACE_FATAL(const char *format, ...)
{
#ifdef _DEBUG
	va_list ap;
	va_start(ap, format);
	OD_TraceV(OD_TRACE_LEVEL_FATAL, format, ap);
	va_end(ap);
#endif /* _DEBUG */
}

void OD_TRACE_HIGH(const char *format, ...)
{
#ifdef _DEBUG
	va_list ap;
	va_start(ap, format);
	OD_TraceV(OD_TRACE_LEVEL_HIGH, format, ap);
	va_end(ap);
#endif /* _DEBUG */
}

void OD_TRACE_MID(const char *format, ...)
{
#ifdef _DEBUG
	va_list ap;
	va_start(ap, format);
	OD_TraceV(OD_TRACE_LEVEL_MID, format, ap);
	va_end(ap);
#endif /* _DEBUG */
}

void OD_TRACE_LOW(const char *format, ...)
{
#ifdef _DEBUG
	va_list ap;
	va_start(ap, format);
	OD_TraceV(OD_TRACE_LEVEL_LOW, format, ap);
	va_end(ap);
#endif /* _DEBUG */
}

#endif /* ! (defined(_MSC_VER) && _MSC_VER < 1300) */

#ifdef OD_TWEAK_LOG_TO_FILE
static OD_FileHandle _OD_GetLogFileHandle(int logfile_id, const OD_WCHAR* file)
{
#define OD_MAX_LOG_FILE_NAME 40
	OD_WCHAR path[OD_MAX_LOG_FILE_NAME] = L"C:\\";
	OD_FileHandle handle;
	SFS_HANDLE sfs_handle;
	/* Transfer the logid to wchars. */
	OD_WCHAR logid_str[3] = {0};
	logid_str[0] = logfile_id/10 + '0';
	logid_str[1] = logfile_id%10 + '0';
	logfile_id++;

	path[0] = OD_GetPublicDriveLetter(NULL);
	if(path[0] == -1)
	{
		OD_TRACE_LOW("In _OD_GetLogFileHandle, can't find public drive");
		return OD_INVALID_FILE_HANDLE;
	}
	od_wcsncat(path, logid_str, OD_MAX_LOG_FILE_NAME -3);
	od_wcsncat(path, file, OD_MAX_LOG_FILE_NAME - 3 - 2);
	path[OD_MAX_LOG_FILE_NAME - 1] = 0;
	sfs_handle = SFS_CreateFile(path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, 0, 0);
	handle = sfs_handle ? (OD_FileHandle)sfs_handle : OD_INVALID_FILE_HANDLE;
	return handle;
}

static OD_FileHandle flog_core = -1,
flog_mmi = -1,
flog_other = -1;

static void OD_InitLogFile()
{
	static int logfile_id = 0;
	logfile_id ++;
	flog_mmi = _OD_GetLogFileHandle(logfile_id, L"om-mmi.log");
	if(flog_mmi == OD_INVALID_FILE_HANDLE)
	{
		OD_TRACE_LOW("Open log file om-mmi.log failed");
	}

	flog_core = _OD_GetLogFileHandle(logfile_id, L"om-core.log");
	if(flog_core == OD_INVALID_FILE_HANDLE)
	{
		OD_TRACE_LOW("Open log file om-core.log failed");
	}

	flog_other = _OD_GetLogFileHandle(logfile_id, L"om-other.log");
	if(flog_other == OD_INVALID_FILE_HANDLE)
	{
		OD_TRACE_LOW("Open log file om-other.log failed");
	}
}

static void OD_CloseLogFile()
{
	if(flog_mmi != OD_INVALID_FILE_HANDLE)
	{
		SFS_CloseFile((SFS_HANDLE)flog_mmi);
		flog_mmi = OD_INVALID_FILE_HANDLE;
	}
	if(flog_core != OD_INVALID_FILE_HANDLE)
	{
		SFS_CloseFile((SFS_HANDLE)flog_core);
		flog_core = OD_INVALID_FILE_HANDLE;
	}
	if(flog_other != OD_INVALID_FILE_HANDLE)
	{
		SFS_CloseFile((SFS_HANDLE)flog_other);
		flog_other = OD_INVALID_FILE_HANDLE;
	}
}
#endif /* OD_TWEAK_LOG_TO_FILE*/

OD_STATUS OD_WriteLogFile(OD_Context* context, const char * format, ...)
{
#ifdef OD_TWEAK_LOG_TO_FILE
	OD_FileHandle flog = OD_INVALID_FILE_HANDLE;
	OD_TaskID module = OD_GetCurrentTaskID();
	OD_STATUS ret = OD_SUCCESS;
	if(module == context->data.thread.mmiTaskID)
	{
		flog = flog_mmi;
	}
	else if(module == context->data.thread.gogiTaskID)
	{
		flog = flog_core;
	}
	else
	{
		flog = flog_other;
	}

	if(flog != OD_INVALID_FILE_HANDLE)
	{
		char buf[OD_TRACE_BUF_SIZE];
		char *pbuf = buf;
		va_list ap;

		od_snprintf(pbuf, 10, "%09d:", OD_GetTickCount());
		pbuf[10] = 0;
		pbuf += od_strlen(pbuf);

		va_start(ap, format);
		od_vsnprintf(pbuf, buf + OD_TRACE_BUF_SIZE - pbuf - 1, format, ap);
		va_end(ap);

		buf[OD_TRACE_BUF_SIZE - 1] = 0;

		{
			unsigned int len = od_strlen(buf), written = 0, tmp = 0;
			while (ret == OD_SUCCESS && written < len)
			{
				ret = OD_FileWrite(context, flog, buf + written, len - written, &tmp);
				if (ret == OD_SUCCESS)
				{
					written += tmp;
				}
			}
			/* To debug crash issue, we must flush log file everytime we write. */
			if (ret == OD_SUCCESS)
			{
				ret = OD_FileCommit(context, flog);
			}
		}
	}
	return ret;
#else /* OD_TWEAK_LOG_TO_FILE */
	return OD_NOT_SUPPORTED;
#endif /* ndef OD_TWEAK_LOG_TO_FILE */
}

OD_STATUS OD_PrintStackTrace(OD_BOOL printLocalValue)
{
	//Not implemented yet
	return OD_NOT_SUPPORTED;
}

OD_STATUS OD_SetDataBreakPoint(
							   unsigned int bm_start_addr,       // Start address
							   unsigned int bm_end_addr,         // End address
							   unsigned int bm_data,             // 0xffffffff is a invalid value
							   OD_DataBreakpointAction bm_mode,     // 0: read; 1: write
							   OD_DataBreakPointRaiseCallback bm_callback
							   )
{
#ifndef WIN32   //not avaliable on simulator
#ifdef PLATFORM_SC6600L
	BM_MonitorAnyArea(bm_start_addr, bm_end_addr, bm_data, bm_mode, bm_callback);
#else
	bm_monitor_set(bm_start_addr, bm_end_addr, bm_data, bm_mode, bm_callback);
#endif

#else /* def WIN32 */
	(bm_start_addr);
	(bm_end_addr);
	(bm_data);
	(bm_mode);
	(bm_callback);
#endif /* ndef WIN32 */
	return OD_SUCCESS;
}

#include <setjmp.h>
static jmp_buf jmpbuf;
static void testSetJmp(int intervalFrames);
static void testLongJmp(int intervalFrames);
static void testSetJmp(int intervalFrames)
{
	int ret = setjmp(jmpbuf);
	if(0 == ret)
	{
		OD_TRACE_LOW("setjmp return 0");
		testLongJmp(intervalFrames);
	}
	else if (0xcc == ret)
	{
		OD_TRACE_LOW("setjmp return 0xcc");
	}
	else
	{
		OD_TRACE_LOW("setjmp return unknown error %d", ret);
		OD_ASSERT(!"shouldn't come here");
	}
}

static void testLongJmp(int intervalFrames)
{
	if(intervalFrames > 0)
	{
		testLongJmp(intervalFrames - 1);
	}
	else
	{
		longjmp(jmpbuf, 0xcc);
	}

	OD_ASSERT(!"shouldn't come here");
}

static void OD_TestJmp(int testTimes, int intervalFrames)
{
	OD_TRACE_LOW("test jmp start");
	while (testTimes --)
	{
		testSetJmp(intervalFrames);
	}
	OD_TRACE_LOW("test jmp end");
}
