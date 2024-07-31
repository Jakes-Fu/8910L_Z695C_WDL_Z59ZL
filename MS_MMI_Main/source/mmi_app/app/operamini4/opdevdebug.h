/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_DEBUG_H
#define OPDEV_DEBUG_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define OD_TRACE       OD_Trace

#ifdef _DEBUG
#define OD_ASSERT(x)   ((x) ? (void) 0 : OD_Assert(#x, __FILE__, __LINE__))
#else /* _DEBUG */
#define OD_ASSERT(x)   ((void)0)
#endif /* !_DEBUG */

#ifndef __FUNCTION__
	#ifdef __arm
	#define __FUNCTION__ __func__
	#else
		#if defined(_MSC_VER) && (_MSC_VER > 1300)
		/* __FUNCTION__ should be present */
		#else
		#define __STRINGIFY(x) #x
		#define __STRINGIFY_2(x) __STRINGIFY(x)
		#define __FUNCTION__ "a function@" __FILE__ " (" __STRINGIFY_2(__LINE__) ")"
		#endif
	#endif
#endif

/**
 * Log importance levels.
 */
typedef enum
{
	/** Fatal error, showstopper. */
	OD_TRACE_LEVEL_FATAL,

	/** High importance, worth reporting back. */
	OD_TRACE_LEVEL_HIGH,

	/** Important debugging information, not critical. */
	OD_TRACE_LEVEL_MID,

	/** Debugging information, only used during development. */
	OD_TRACE_LEVEL_LOW
} OD_TRACE_LEVEL;

#if defined(_MSC_VER) && _MSC_VER < 1300
/* INTERNAL */

/**
 * Placeholder for disabled tracing.
 *
 * @param[in] level the trace level.
 * @param[in] format the format string, same as printf().
 */
void OD_TraceDummy(OD_TRACE_LEVEL level, const char *format, ...);

/**
 * Writes a trace entry with FATAL importance.
 *
 * @param[in] format the format string, same as printf().
 */
void OD_TRACE_FATAL(const char *format, ...);

/**
 * Writes a trace entry with HIGH importance.
 *
 * @param[in] format the format string, same as printf().
 */
void OD_TRACE_HIGH(const char *format, ...);

/**
 * Writes a trace entry with MID importance.
 *
 * @param[in] format the format string, same as printf().
 */
void OD_TRACE_MID(const char *format, ...);

/**
 * Writes a trace entry with LOW importance.
 *
 * @param[in] format the format string, same as printf().
 */
void OD_TRACE_LOW(const char *format, ...);

#else /* defined(_MSC_VER) && _MSC_VER < 1300 */

/* Macros for Non-VS6... */

#define OD_TRACE_FATAL(...) OD_TRACE(OD_TRACE_LEVEL_FATAL, __VA_ARGS__)
#define OD_TRACE_HIGH(...)  OD_TRACE(OD_TRACE_LEVEL_HIGH, __VA_ARGS__)
#define OD_TRACE_MID(...)   OD_TRACE(OD_TRACE_LEVEL_MID, __VA_ARGS__)
#define OD_TRACE_LOW(...)   OD_TRACE(OD_TRACE_LEVEL_LOW, __VA_ARGS__)

#endif /* ! (defined(_MSC_VER) && _MSC_VER < 1300) */

#define OD_TRACE_W_LOW(wstr)  OD_TraceWString(OD_TRACE_LEVEL_LOW, (wstr))

/**
 * Initialises the debugging subsystem.
 */
OD_MMI_TASK void OD_InitDebugger(void);

/**
 * Deinitialises the debugging subsystem.
 */
OD_MMI_TASK void OD_DeinitDebugger(void);

/**
 * Writes a string to a log file.
 *
 * The log file the string is going to be written to is being chosen
 * depending on which OS task this function is being invoked from.
 *
 * Right now there are three possible choices, namely om-mmi.log,
 * om-core.log, and om-other.log - respectively for the MMI, CORE, and
 * (eventually) DISPLAY tasks.
 *
 * @param context the context.
 * @param format the format string, same as printf().
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_WriteLogFile(OD_Context* context, const char *format, ...);

/**
 * Writes a trace message using the platform log mechanism.
 *
 * @param[in] level the trace level.
 * @param[in] format the format string, same as printf(), shouldn't be NULL
 *
 * @return OD_SUCCESS if the operation succeed, OD_ERROR if failed
 */
OD_ALL_TASK OD_STATUS OD_Trace(OD_TRACE_LEVEL level, const char *format, ...);

/**
* Writes a trace message(encoding in UCS2) using the platform log mechanism.
*
* @param[in] level the trace level.
* @param[in] wstr the ucs2 string to be print, shouldn't be NULL.
*
* @return OD_SUCCESS if the operation succeed, OD_ERROR if failed.
*/
OD_ALL_TASK OD_STATUS OD_TraceWString(OD_TRACE_LEVEL level, const OD_WCHAR *wstr);

/**
 * Prints the current stack trace using the platform log mechanism.
 *
 * @param[in] print_locals flag indicating whether to print each local
 *            variable on each stack frame, OD_TRUE if so, otherwise
 *            OD_FALSE.
 *
 * @return OD_SUCCESS if the operation succeeded, or the appropriate error
 *         code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_PrintStackTrace(OD_BOOL print_locals);

/**
 * Callback for data breakpoints set with OD_SetDataBreakPoint.
 *
 * @see OD_SetDataBreakPoint
 *
 * @param[in] data user data for the breakpoint.
 * @param[in] pc_before_fiq the program counter value before the fast irq
 *            was triggered.
 */
typedef void (OD_ALL_TASK *OD_DataBreakPointRaiseCallback)(unsigned long data, unsigned long pc_before_fiq);

/**
 * Data breakpoint action.
 */
typedef enum
{
	/** Breakpoint on Read. */
	OD_BKPT_READ = 0,

	/** Breakpoint on Write. */
	OD_BKPT_WRITE
} OD_DataBreakpointAction;

/**
 * Sets a breakpoint callback when a memory range is being accessed.
 *
 * @param[in] start_address the memory range start address.
 * @param[in] end_address the memory range end address.
 * @param[in] data user data to pass to the callback (can't be 0xFFFFFFFF).
 * @param[in] mode the action to trigger the breakpoint.
 * @param[in] callback the callback to invoke when the breakpoint is hit.
 *
 * @return OD_SUCCESS if the operation succeeded, or the appropriate error
 *         code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_SetDataBreakPoint(unsigned int start_addr,
		unsigned int end_addr, unsigned int data,
		OD_DataBreakpointAction mode,
		OD_DataBreakPointRaiseCallback callback);

/*
 * Trace buffer size, different between target and simulator to
 * prevent stack overflows and stack overruns.
 */
#ifdef WIN32
#define OD_TRACE_BUF_SIZE  1000
#else
#define OD_TRACE_BUF_SIZE  200
#endif

/* INTERNAL */ 

/**
 * Reports an assertion with the given data.
 *
 * @warn NO checks are being done on the content of the parameters,
 *       hence no guarantees are being made regarding the output of
 *       function calls to OD_Assert.
 *
 * @param[in] exp the expression that triggered the assertion.
 * @param[in] file the file in which the expression was found.
 * @param[in] line the line in the file that contains the expression.
 */
OD_ALL_TASK void OD_Assert(const char *exp, const char *file, int line);


#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* OPDEV_DEBUG_H */
