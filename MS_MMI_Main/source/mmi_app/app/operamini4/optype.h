/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_OPTYPE_H
#define OPDEV_OPTYPE_H

/**
 * NOTE:
 * 1) This file will only define the primary data types for OpDev, and those which
 *    will be used in more than one classified OpDev header.
 * 2) The data type and const values used in OpDev MUST be compatible for all SDKs.
 *  2.1) If there's no good data type for a certain data, then use int or void*.
 *  2.2) If there's no consistent value for a const, then declare a function for it.
 *       The function should return proper value in different SDKs.
 * 3) The other types should be defined in the relevant classified OpDev headers.
 * 4) This file MUST be included as the beginning of any classified OpDev header.
 */

#ifdef __cplusplus
extern "C" {
#endif


/* Markers to indicate where a certain OpDev function can be used. */
#define OD_MMI_TASK
#define OD_CORE_TASK
#define OD_ALL_TASK


/************************************************************************/
/* Data                                                                 */
/************************************************************************/
typedef int				OD_BOOL;
typedef unsigned short	OD_WCHAR;
#if defined(_MSC_VER) && (_MSC_VER < 1300)
typedef __int64		OD_INT64;
#else
typedef long long		OD_INT64;
#endif

#define OD_FALSE		(0)
#define OD_TRUE			(1)
#define OD_NULL			(0)


/************************************************************************/
/* File                                                                 */
/************************************************************************/
typedef int				OD_FileHandle;
typedef int				OD_FileFindHandle;
typedef unsigned short	OD_FileName;

#define OD_INVALID_FILE_HANDLE		((OD_FileHandle)-1)
#define OD_INVALID_FILE_FIND_HANDLE	((OD_FileFindHandle)-1)


/************************************************************************/
/* FONT                                                                 */
/************************************************************************/


/************************************************************************/
/* GDI                                                                  */
/************************************************************************/
typedef unsigned int	OD_GDIHandle;
#define OD_INVALID_GDI_HANDLE		((OD_GDIHandle)0)


/************************************************************************/
/* Thread                                                               */
/************************************************************************/
typedef void*			OD_SemID;
typedef unsigned int	OD_TaskID;
typedef unsigned int	OD_TaskIndex;
typedef unsigned int	OD_ModuleID;

typedef struct _OD_Mutex
{
	void* m_mutex;

	volatile int m_counter;			/* reserved for compatibility */
	volatile OD_TaskID m_task_id;	/* reserved for compatibility */
} OD_Mutex;
typedef OD_Mutex		*OD_MutexID;

#define OD_INVALID_TASKID			((OD_TaskID)-1)


/************************************************************************/
/* Async API Callback                                                   */
/************************************************************************/
struct _OD_Async_Call;
typedef void (*OD_ASYNC_CALL)(struct _OD_Async_Call*);
typedef struct _OD_AsyncCallback
{
	OD_TaskID sender;
	OD_TaskID receiver;
	void* param;
	OD_ASYNC_CALL call;
} OD_AsyncCallback;

typedef struct _OD_Async_Call
{
	int event;
	OD_AsyncCallback req;
	OD_AsyncCallback ret;
} OD_AsyncCall;

typedef struct _OD_TaskMsg	OD_TaskMsg;


/************************************************************************/
/* Socket                                                               */
/************************************************************************/
typedef int				OD_Socket;
typedef unsigned int	OD_HostResolveHandle;
#define OD_INVALID_SOCKET			((OD_Socket)-1)
#define OD_INVALID_RESOLVE_HANDLE	((OD_HostResolveHandle)0)
typedef void*			OD_Socket_Event;
typedef void*			OD_HostResolve_Event;


/************************************************************************/
/* Network                                                              */
/************************************************************************/
typedef unsigned int	OD_NetID;


/************************************************************************/
/* System                                                               */
/************************************************************************/
typedef void*			OD_TimerID;
typedef void*			OD_TimerArg;
typedef void			(*OD_TIMERFUN)(OD_TimerArg);
#define OD_INVALID_TIMER	((OD_TimerID)-1)

typedef struct _OD_SYS_Event	OD_SYS_Event;

typedef struct _OD_Context		OD_Context;

typedef enum _OD_STATUS
{
	OD_SUCCESS                  = 0,

	OD_GENERAL_ERROR_UPPER      = -1,
	OD_ERROR                    = OD_GENERAL_ERROR_UPPER,
	OD_NO_MEMORY                = -2,					/* Can't alloc enough memory for operation, aka. OOM */
	OD_NULL_POINTER             = -3,
	OD_ERROR_PENDING            = -4,					/* Pending for result, indicating callee is an async api */
	OD_INVALID_PARAM            = -5,					/* One of the param is invalid */
	OD_OUT_OF_RANGE             = -6,					/* Parameter or operation is out of permitted range */
	OD_NOT_SUPPORTED            = -7,					/* The call is not supported for this module/object/platform */
	OD_NOT_DELAYED              = -8,					/* Posted message was not delayed */
	OD_PARSING_FAILED           = -9,					/* A parsing operation failed */
	OD_NO_SUCH_RESOURCE         = -10,					/* Resource not available (like not assigned listener) */
	OD_NOT_READY                = -11,
	OD_INSUFFICIENT_BUFFER      = -12,					/* Any given buffer size is not sufficient for operation */
	OD_GENERAL_ERROR_LOWER      = -31,

	OD_FILE_ERROR_UPPER         = -32,
	OD_FILE_NO_ACCESS           = OD_FILE_ERROR_UPPER,  /* Attempting to write a read-only entity */
	OD_FILE_NOT_FOUND           = -33,                  /* File not found or could not be opened */
	OD_FILE_NO_DISK             = -34,                  /* Disk is full */
	OD_FILE_BAD_FILE_NUMBER     = -35,                  /* Bad file number, operations on resource illegal (wrong socket id/file id) */
	OD_FILE_NO_MORE_FILES       = -36,
	OD_FILE_EXISTS              = -37,
	OD_FILE_NOT_EMPTY           = -38,
	OD_FILE_ERROR_LOWER         = -63,

	OD_SOCKET_ERROR_UPPER       = -64,
	OD_SOCKET_WOULDBLOCK        = OD_SOCKET_ERROR_UPPER,
	OD_SOCKET_LIMIT_RESOURCE    = -65,
	OD_SOCKET_INVALID_SOCKET    = -66,					/* invalid socket */
	OD_SOCKET_INVALID_ACCOUNT   = -67,					/* invalid account id */
	OD_SOCKET_ALREADY           = -68,					/* operation already in progress */
	OD_SOCKET_INVALID_ARGUMENT  = -69,					/* invalid argument */
	OD_SOCKET_BEARER_FAIL       = -70,					/* bearer is broken */
	OD_SOCKET_CONNRESET         = -71,					/* TCP half-write close, i.e., FINED */
	OD_SOCKET_CLOSED            = -72,
	OD_SOCKET_ERROR_LOWER       = -95,

	OD_TIMER_ERROR_UPPER        = -96,
	OD_TIMER_ALREADY_RUN        = OD_TIMER_ERROR_UPPER,	/* timer already runs/activated */
	OD_TIMER_ERROR_LOWER        = -127,

	OD_SCHEME_ERROR_UPPER       = -128,
	OD_SCHEME_ALREADY_REGISTERED = OD_SCHEME_ERROR_UPPER, /** Scheme already registered */
	OD_SCHEME_PENDING           = -129,                   /** Request pending */
	OD_SCHEME_FAILED            = -130,                   /** Request failed */
	OD_SCHEME_ERROR_LOWER       = -159,

} OD_STATUS;


#ifdef __cplusplus
};
#endif

#endif /* OPDEV_OPTYPE_H */
