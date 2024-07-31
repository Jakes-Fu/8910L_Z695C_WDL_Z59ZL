/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_THREAD_H
#define OPDEV_THREAD_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OD_MAX_MUTEX_COUNT 24
#define OD_MAX_SEMAPHORE_COUNT 4

typedef struct _OD_Thread_Context
{
	OD_Mutex atomicMutex;
	OD_TaskID mmiTaskID;
	void* mmiTaskData;
	OD_TaskID gogiTaskID;
	void* gogiTaskData;
} OD_ThreadContext;


/************************************************************************/
/* Atomic                                                               */
/************************************************************************/

/** Init Atomic */
OD_ALL_TASK OD_STATUS OD_AtomicInit(OD_Context *context);

/** Destory Actomic */
OD_ALL_TASK OD_STATUS OD_AtomicDestroy(OD_Context *context);

/** Atomiclly increase the value of var */
OD_ALL_TASK int OD_AtomicIncrement(OD_Context *context, int volatile* var);

/** Atomiclly decrease the value of var */
OD_ALL_TASK int OD_AtomicDecrement(OD_Context *context, int volatile* var);


/************************************************************************/
/* Mutex                                                                */
/************************************************************************/

/**
 * Crate mutex after the given name
 * @param mutex_name the name of the mutex
 * @param mutex the mutex struct to hold the handler of system mutex
 * @return OD_SUCCESS if created, otherwise OD_ERROR
 */
OD_ALL_TASK OD_STATUS OD_CreateMutex(const char* mutex_name, OD_MutexID* mutex);

/**
 * Release Mutex
 * @param context Opera context
 * @mutex_id the ID of the mutex
 * @return OD_SUCCESS if released, otherwise OD_ERROR
 */
OD_ALL_TASK OD_STATUS OD_ReleaseMutex(OD_Context *context, OD_MutexID mutex_id);

/**
 * Take the mutex, it will waiting for ever.
 * @param mutex_id the mutex_id
 * @return OD_SUCCESS if taken, otherwise OD_ERROR
 */
OD_ALL_TASK OD_STATUS OD_EnterMutex(OD_MutexID mutex_id);

/**
 * Take the mutex, it will waiting for ever.
 * @param mutex_id the mutex_id
 * @return OD_SUCCESS if leaved, otherwise OD_ERROR
 */
OD_ALL_TASK OD_STATUS OD_LeaveMutex(OD_MutexID mutex_id);


/************************************************************************/
/* Semaphore                                                            */
/************************************************************************/
typedef enum
{
	OD_SEMAPHORE_NO_WAIT,
	OD_SEMAPHORE_WAIT_INIFITE
} OD_WaitMode;

/**
 * Create semaphore
 * @param context Opera context
 * @param sem_name semaphore name
 * @param initial_count, the initial value
 * return OD_SemID the ID created.
 */
OD_ALL_TASK OD_SemID OD_CreateSemaphore(OD_Context *context, const char* sem_name, unsigned int initial_count);

/**
 * Release the semaphore
 * @param context opera context
 * @semid the semaphore ID
 * @param count the remain count of the semaphore
 * @return OD_SUCCESS if released, otherwise OD_ERROR
 */
OD_ALL_TASK OD_STATUS OD_ReleaseSemaphore(OD_Context *context, OD_SemID semid, int count);

/**
 * Take the semaphore
 * @param semid
 * @param wait_mode
 */
OD_ALL_TASK OD_STATUS OD_TakeSemaphore(OD_SemID semid, OD_WaitMode wait_mode);

/** Give the semaphore */
OD_ALL_TASK OD_STATUS OD_GiveSemaphore(OD_SemID semid);


/************************************************************************/
/* Task                                                                 */
/************************************************************************/

/**
 * Get the current thread ID.
 * Note: if the thread was create by user, return thread ID,
 * otherwise OD_INVALID_TASKID is returned.
 */
OD_ALL_TASK OD_TaskID OD_GetCurrentTaskID(void);


/**
 * Get the thread ID of MMI task.
 */
OD_ALL_TASK OD_TaskID OD_GetMMITaskID(OD_Context *context);


/**
 * Get the thread ID of Gogi task.
 */
OD_ALL_TASK OD_TaskID OD_GetGogiTaskID(OD_Context* context);


/**
 * init MMI task context
 * @note: please call it in MMI task
 */
OD_MMI_TASK void OD_InitMMITaskContext(OD_Context *context);

/**
 * init MMI task context
 * @note: please call it in MMI task
 */
OD_MMI_TASK void OD_InitGogiTaskContext(OD_Context *context);


/**
 * Create gogi task
 * @context [in], global context
 * @taskname [in], task name
 * @queuename [in], system message queue name in this task.
 * @stacksize [in], task stack size in bytes
 * @queuesize [in], number of messages could be queued
 * @priority [in], task priority
 * @return task id
 */
OD_MMI_TASK OD_TaskID OD_CreateGogiTask(OD_Context *context, const char* taskname, const char* queuename, 
										int stacksize, int queuesize, int priority);

/**
 * Create gogi task
 * @Note dynamic thread is not support by some platforms
 */
OD_MMI_TASK void OD_DestroyGogiTask(OD_Context* context);

/**
 * Request a function call in specific task in the next timing.
 * @param task, the function call will be called in this task
 * @param call the function will be called.
 * @param param, the parameter passed to the OD_ASYNC_CALL
 * @Note the caller need need free the OD_AsyncCall memory after post.
 *  but the memory in the param must be freed in the call function.
 */
OD_ALL_TASK void OD_PostCall(OD_AsyncCall* call);

/**
 * Force trigger task context switch.
 */
OD_ALL_TASK void OD_Yield(OD_Context* context);

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_THREAD_H */
