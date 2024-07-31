/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opdevinternal.h"

#if defined _WIN32 && defined OD_STACK_TEST
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#define OD_MAX_STACK_FRAMES 1000

typedef struct _OD_StackTestStruct
{
    char * base_addr;
    int max_stack_depth;
    int stack_frame;
    int sf[OD_MAX_STACK_FRAMES];
    OD_TaskID taskid;
} OD_StackTestStruct;

static OD_StackTestStruct stss[OD_STACKTEST_TASK_NUMBER];

OD_StackTestStruct * _findSts(OD_TaskID taskid)
{
    int i = 0;
    for(; i< OD_STACKTEST_TASK_NUMBER; ++i)
    {
        if(stss[i].taskid == taskid)
            return &stss[i];
    }
    return NULL;
}

void OD_StackTest_init()
{
    od_memset(stss, 0, sizeof(stss));
}

void OD_StackTest_initTaskInfo(OD_STACKTEST_TASKID st_tid)
{
    char c;
    OD_ASSERT(st_tid < OD_STACKTEST_TASK_NUMBER);
    stss[st_tid].base_addr = &c;
    stss[st_tid].taskid = OD_GetCurrentTaskID();
}

void OD_StackTest_printMaxStackDepth()
{
    int i = 0;
    for(; i< OD_STACKTEST_TASK_NUMBER; ++i)
    {
        printf("task index %d, base_address:0x%x, max_stack_depth:%d \n",
            i, stss[i].base_addr, stss[i].max_stack_depth);
    }
}

void _do_enter( void ) 
{
    char c;
    int n;
    OD_StackTestStruct * st = _findSts(OD_GetCurrentTaskID());
    if(!st)
        return;

    n = st->base_addr - &c;
    st->sf[OD_MAX_STACK_FRAMES-1-(st->stack_frame++)] = n;
    if( n > st->max_stack_depth)
    {
        st->max_stack_depth= n;
    }
}

void _do_exit( void )
{
    OD_StackTestStruct * st = _findSts(OD_GetCurrentTaskID());
    if(st && st->stack_frame > 0)
        --st->stack_frame;

}

void __declspec(naked) _cdecl _penter( void ) {
    _asm {
        push eax
            push ebx
            push ecx
            push edx
            push ebp
            push edi
            push esi
    }

    _do_enter();

    _asm {
        pop esi
            pop edi
            pop ebp
            pop edx
            pop ecx
            pop ebx
            pop eax
            ret
    }
}

void __declspec(naked) _cdecl _pexit( void )
{
    _asm {
        push eax
            push ebx
            push ecx
            push edx
            push ebp
            push edi
            push esi
    }

    _do_exit();

    _asm {
        pop esi
            pop edi
            pop ebp
            pop edx
            pop ecx
            pop ebx
            pop eax
            ret
    }
}

#endif /* _WIN32 && OD_STACK_TEST */
