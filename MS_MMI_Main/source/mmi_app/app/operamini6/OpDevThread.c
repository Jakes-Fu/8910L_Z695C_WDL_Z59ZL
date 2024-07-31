/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"


/************************************************************************/
/* Atomic                                                               */
/************************************************************************/

OD_STATUS OD_AtomicInit(OD_Context *context)
{
	OD_MutexID id  = OD_NULL;
	char name[32];

	OD_CHECK_ARGS_RETURN_STATUS(context && "Invalid OD_Context");

	od_snprintf(name, sizeof(name), "OD_AtomicMutex_%xd", (unsigned int)context);
	
    id = SCI_CreateMutex(name, SCI_INHERIT);
	if(id)
	{
		context->data.thread.atomicMutex.m_counter = 0;
		context->data.thread.atomicMutex.m_task_id = OD_INVALID_TASKID;
		context->data.thread.atomicMutex.m_mutex = id;
	}
	
	return (id != SCI_NULL) ? OD_SUCCESS : OD_ERROR;
}

OD_STATUS OD_AtomicDestroy(OD_Context *context)
{
	OD_STATUS ret = OD_SUCCESS;
	OD_CHECK_ARGS_RETURN_STATUS(context && "Invalid OD_Context");
	
	ret = OD_ReleaseMutex(context, &context->data.thread.atomicMutex);
	context->data.thread.atomicMutex.m_mutex = 0;
	return ret;
}

int OD_AtomicIncrement(OD_Context *context, int volatile* var)
{
	int ret = 0;
	
	OD_CHECK_ARGS_RETURN_STATUS(context && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS(var && "var should not be null");

	SCI_GetMutex(context->data.thread.atomicMutex.m_mutex, SCI_WAIT_FOREVER);
	ret = ++(*var);
	SCI_PutMutex(context->data.thread.atomicMutex.m_mutex);
	
	return ret;
}

int OD_AtomicDecrement(OD_Context *context, int volatile* var)
{
	int ret = 0;

	OD_CHECK_ARGS_RETURN_STATUS(context && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS(var && "var should not be null");

	SCI_GetMutex(context->data.thread.atomicMutex.m_mutex, SCI_WAIT_FOREVER);
	ret = --(*var);
	SCI_PutMutex(context->data.thread.atomicMutex.m_mutex);
	
	return ret;
}

/************************************************************************/
/* Mutex                                                                */
/************************************************************************/
OD_STATUS OD_CreateMutex(const char* mutex_name, OD_MutexID* mutex_id)
{
	OD_CHECK_ARGS_RETURN_STATUS(mutex_id && "mutex_id should not be null");

	(*mutex_id)->m_counter = 0;
	(*mutex_id)->m_task_id = 0;
	(*mutex_id)->m_mutex = SCI_CreateMutex(mutex_name, SCI_INHERIT);
	return (*mutex_id)->m_mutex ? OD_SUCCESS : OD_ERROR;
}

OD_STATUS OD_ReleaseMutex(OD_Context *context, OD_MutexID mutex_id)
{
	OD_CHECK_ARGS_RETURN_STATUS(context && "Invalid OD_Context");
	return (SCI_DeleteMutex(mutex_id->m_mutex) == SCI_SUCCESS) ? OD_SUCCESS : OD_ERROR;
}

OD_STATUS  OD_EnterMutex(OD_MutexID mutex_id)
{
	return (SCI_GetMutex(mutex_id->m_mutex, SCI_WAIT_FOREVER) == SCI_SUCCESS) ? OD_SUCCESS : OD_ERROR;
}

OD_STATUS  OD_LeaveMutex(OD_MutexID mutex_id)
{
	return (SCI_PutMutex(mutex_id->m_mutex) == SCI_SUCCESS) ? OD_SUCCESS : OD_ERROR;
}


/************************************************************************/
/* Semaphore                                                            */
/************************************************************************/
/* OD_ALL_TASK */
static OD_STATUS OD_ThreadInitContext(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context && "Invalid OD_Context");
	return OD_SUCCESS;
}


OD_SemID OD_CreateSemaphore(OD_Context *context, const char* sem_name, unsigned int initial_count)
{
	OD_CHECK_ARGS(context && "Invalid OD_Context", NULL);
	return SCI_CreateSemaphore(sem_name, initial_count);
}

OD_STATUS OD_ReleaseSemaphore(OD_Context *context, OD_SemID semid, int count)
{
	OD_CHECK_ARGS_RETURN_STATUS(context && "Invalid OD_Context");
	return (SCI_DeleteSemaphore(semid) == SCI_SUCCESS) ? OD_SUCCESS : OD_ERROR;
}

OD_STATUS OD_TakeSemaphore(OD_SemID semid, OD_WaitMode wait_mode)
{
	int option = SCI_NO_WAIT;
	switch(wait_mode)
	{
	case OD_SEMAPHORE_NO_WAIT:
		option = SCI_NO_WAIT;
		break;
	case OD_SEMAPHORE_WAIT_INIFITE:
		option = SCI_WAIT_FOREVER;
		break;
	default:
		OD_ASSERT(!"unknown wait mode");
	}
	return (SCI_GetSemaphore(semid, option) == SCI_SUCCESS) ? OD_SUCCESS : OD_ERROR;
}

OD_STATUS OD_GiveSemaphore(OD_SemID semid)
{
	return (SCI_PutSemaphore(semid) == SCI_SUCCESS) ? OD_SUCCESS : OD_ERROR;
}


/************************************************************************/
/* Task                                                                 */
/************************************************************************/
OD_TaskID OD_GetCurrentTaskID(void)
{
	return SCI_IdentifyThread();
}

OD_TaskID OD_GetMMITaskID(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
    return (context->data.thread).mmiTaskID;
}

OD_TaskID OD_GetGogiTaskID(OD_Context* context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
	return context->data.thread.gogiTaskID;
}

void OD_InitMMITaskContext(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_VOID(context && "Invalid OD_Context");

    context->data.thread.mmiTaskID = OD_GetCurrentTaskID();
	context->data.thread.mmiTaskData = NULL;
}

void OD_InitGogiTaskContext(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_VOID(context && "Invalid OD_Context");
	context->data.thread.gogiTaskID = OD_GetCurrentTaskID();
	context->data.thread.gogiTaskData = NULL;
}

struct _OD_TaskMsg
{
    SIGNAL_VARS
    unsigned int event; //sub event in case of the number of master event limitation.
};

typedef struct _OD_AsyncCallMsg
{
    SIGNAL_VARS
    unsigned int event;
    OD_AsyncCall call;
} OD_AsyncCallMsg;

static void _OD_ExecAsyncCall(OD_AsyncCall* async)
{
	OD_ASSERT(async);
	OD_ASSERT(async->req.call);

	(async->req.call)(async);
	if (async->ret.call)
	{
		OD_AsyncCall request = {0};
		request.req = async->ret;
		OD_PostCall(&request);
	}
}

/* OD_ALL_TASK */
static OD_BOOL OD_TaskMessageHandler(OD_Context* context, OD_TaskMsg* msg)
{
    OD_BOOL handled = OD_TRUE;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(IsValidContext(context) && "Invalid OD_Context");
	OD_CHECK_ARGS_RETURN_STATUS(msg);

    switch(msg->SignalCode)
    {
    case OPERA_ASYNC_CALL_MSG:
        {
            OD_AsyncCall* call = &((OD_AsyncCallMsg* )msg)->call;
            _OD_ExecAsyncCall(call);
        }
        break;
    default:
        {
            handled = OD_FALSE;
        }
        break;
    }

    return handled;
}

static void OD_GogiTaskMain(unsigned long int argc, void* argv)
{
    OD_Context* ctx = (OD_Context*)argv;

    xSignalHeaderRec* sig = 0;
    OD_TaskID taskID = OD_GetCurrentTaskID();

	OD_CHECK_ARGS_RETURN_VOID(ctx);
	OD_CHECK_ARGS_RETURN_VOID(IsValidContext(ctx) && "Invalid OD_Context");

#ifdef OD_STACK_TEST
    OD_StackTest_initTaskInfo(OD_STACKTEST_CORE_TASK);
#endif

    for(;;)
    {
        sig = SCI_GetSignal(taskID);
        OD_ASSERT(sig);

        switch(sig->SignalCode)
        {
		case OPERA_ASYNC_CALL_MSG:
			{
				OD_AsyncCall* call = &((OD_AsyncCallMsg*)sig)->call;
				_OD_ExecAsyncCall(call);
			}
			break;
        default:
            break;
        }

        SCI_FREE(sig);
    }

    ctx->data.thread.gogiTaskID = OD_NULL;
}


OD_TaskID OD_CreateGogiTask(OD_Context *context,
                            const char* taskname, 
                            const char* queuename, 
                            int stacksize, 
                            int queuesize, 
                            int priority)
{
	OD_ThreadContext* tcontext = &(context->data.thread);
	

	OD_TaskID task = SCI_CreateAppThread(taskname,
						queuename,
						&OD_GogiTaskMain,
						0,
						(void*)context,
						stacksize,
						queuesize,
						priority,
						SCI_PREEMPT,
						SCI_DONT_START);

	
	tcontext->gogiTaskID = task;

	if(OD_INVALID_TASKID != task) SCI_ResumeThread(task);

	return task;
}

void OD_DestroyGogiTask(OD_Context* context)
{
	OD_CHECK_ARGS_RETURN_VOID(context->data.thread.gogiTaskID);

    SCI_TerminateThread(context->data.thread.gogiTaskID);

	SCI_DeleteThread(context->data.thread.gogiTaskID);
}

void OD_PostCall(OD_AsyncCall* call)
{
	OD_AsyncCallMsg* msg = NULL;
	OD_CHECK_ARGS_RETURN_VOID(call);

	msg = (OD_AsyncCallMsg*)SCI_ALLOCA(sizeof(OD_AsyncCallMsg));
    if(msg)
    {
        uint32 ret;
        msg->SignalCode = OPERA_ASYNC_CALL_MSG;
        msg->SignalSize = sizeof(*msg);
        msg->Sender = OD_GetCurrentTaskID();
		call->req.sender = msg->Sender;
        SCI_MEMCPY(&(msg->call), call, sizeof(OD_AsyncCall));
        ret = SCI_SendSignal((xSignalHeader)msg, call->req.receiver);
		OD_ASSERT(ret == SCI_SUCCESS);
    }
}

void OD_Yield(OD_Context* context)
{
	/* It assumes that OD_Yield implemented by send message to force the thread switch.
	Spreadtrum people said their system wouldn't meet the MMI Queue-Full problem
	caused by core task run too long slice. So do nothing now.*/
	(context);
}

