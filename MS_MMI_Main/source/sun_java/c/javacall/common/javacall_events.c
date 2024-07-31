/*
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#ifdef WIN32
//#include "tx_api_win32.h"      
#else
//#include "tx_api_thumb.h"    /*lint !e766*/
#endif
#include "os_api.h"
#include "javacall_logging.h"
#include "javacall_time.h"
#include "javacall_events.h"
#include "javacall_memory.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MaxEventNumber 32
#ifdef _MSC_VER
#define USE_SCI_SEMA 1
#else
#undef USE_SCI_SEMA
#endif
#ifndef USE_SCI_SEMA
#define USE_SCI_SEMA 1
#endif
//#define JAVA_EVENT_POOL_MUTEX_TIMEOUT 50
typedef struct {
    int size;
    char* data;
} Event;
static Event eventPool[MaxEventNumber];
static javacall_bool initialized = JAVACALL_FALSE;
static int head, tail;

#ifdef USE_SCI_SEMA
static SCI_SEMAPHORE_PTR sema;
#else
static TX_SEMAPHORE txSema, *sema = &txSema;
#endif
static void * g_eventPoolMutex=NULL;

/*
 * Mandatory event functions:
 * - javacall_events_init()
 * - javacall_events_finalize()
 *
 * Functions specific for CLDC-based implementations:
 * - javacall_event_receive()
 * - javacall_event_send()
 *
 * Functions specific for CDC-based implementations:
 * - javacall_event_receive_cvm()
 * - javacall_event_send_cvm()
 */
    
/**
 * CLDC-specific function.
 * Waits for an incoming event message and copies it to user supplied
 * data buffer
 * @param waitForever indicate if the function should block forever
 * @param timeTowaitInMillisec max number of seconds to wait
 *              if waitForever is false
 * @param binaryBuffer user-supplied buffer to copy event to
 * @param binaryBufferMaxLen maximum buffer size that an event can be 
 *              copied to.
 *              If an event exceeds the binaryBufferMaxLen, then the first
 *              binaryBufferMaxLen bytes of the events will be copied
 *              to user-supplied binaryBuffer, and JAVACALL_OUT_OF_MEMORY will 
 *              be returned
 * @param outEventLen user-supplied pointer to variable that will hold actual 
 *              event size received
 *              Platform is responsible to set this value on success to the 
 *              size of the event received, or 0 on failure.
 *              If outEventLen is NULL, the event size is not returned.
 * @return <tt>JAVACALL_OK</tt> if an event successfully received, 
 *         <tt>JAVACALL_FAIL</tt> or if failed or no messages are avaialable
 *         <tt>JAVACALL_OUT_OF_MEMORY</tt> If an event's size exceeds the 
 *         binaryBufferMaxLen 
 */
javacall_result javacall_event_receive(
                            long                     timeTowaitInMillisec,
                            /*OUT*/ unsigned char*  binaryBuffer,
                            /*IN*/  int             binaryBufferMaxLen,
                            /*OUT*/ int*            outEventLen){
    uint32 result;
    uint32 uiResult;
    
    if (NULL == binaryBuffer || 0 >= binaryBufferMaxLen) {
        return JAVACALL_FAIL;
    }

    if (JAVACALL_FALSE == initialized) {
        if (JAVACALL_FAIL == javacall_events_init()) {
            return JAVACALL_FAIL;
        }
    }
        
    #ifdef USE_SCI_SEMA
//    javacall_printf(LOG_STR("after get 1"));
    result = SCI_GetSemaphore(sema, (uint32)timeTowaitInMillisec);
    if (SCI_SUCCESS != result) {
    #else
    result = tx_semaphore_get(sema, timeTowaitInMillisec);
//    javacall_printf(LOG_STR("after get 2"));
    if (TX_SUCCESS != result) {
    #endif
//        javacall_printf("javacall_event_receive:failed 3, result is %d\n",result);
        return JAVACALL_FAIL;
    }

    uiResult = SCI_GetMutex(g_eventPoolMutex, SCI_WAIT_FOREVER);
    if(uiResult != SCI_SUCCESS) {
        javacall_printf(LOG_STR_LEAVE("fail, to get event mutex"));
        return JAVACALL_FAIL;
    }
    if (eventPool[tail].size > binaryBufferMaxLen) {
        javacall_print("[WARNING]javacall_event_receive: Event's discarded for too long\n");
        if (eventPool[tail].data)
        {
            javacall_free(eventPool[tail].data);
            eventPool[tail].data = NULL;
        }
        if (MaxEventNumber - 1 == tail)
            tail = 0;
        else
            tail++;
        SCI_PutMutex(g_eventPoolMutex);
        javacall_printf(LOG_STR_LEAVE("fail, out of memory"));
        return JAVACALL_OUT_OF_MEMORY;
    }

    if (outEventLen != NULL) {
        *outEventLen = eventPool[tail].size;
    }
    SCI_MEMCPY(binaryBuffer, eventPool[tail].data, eventPool[tail].size);
    if (eventPool[tail].data)
    {
        javacall_free(eventPool[tail].data);
        eventPool[tail].data = NULL;
    }
    if (MaxEventNumber - 1 == tail)
        tail = 0;
    else
        tail++;
    //javacall_print("javacall_event_receive:ok 3");
    SCI_PutMutex(g_eventPoolMutex);
    return JAVACALL_OK;
}

/**
 * CLDC-specific function.
 * copies a user supplied event message to a queue of messages
 *
 * @param binaryBuffer a pointer to binary event buffer to send
 *        The platform should make a private copy of this buffer as
 *        access to it is not allowed after the function call.
 * @param binaryBufferLen size of binary event buffer to send
 * @return <tt>JAVACALL_OK</tt> if an event successfully sent, 
 *         <tt>JAVACALL_FAIL</tt> or negative value if failed
 */
javacall_result javacall_event_send(unsigned char* binaryBuffer, int binaryBufferLen)
{
    int next;
    uint32 uiResult;
    
    if (NULL == binaryBuffer || 0 >= binaryBufferLen)
    {
        //javacall_print("failed 1");
        return JAVACALL_FAIL;
    }

    if (JAVACALL_FALSE == initialized) {
        if (JAVACALL_FAIL == javacall_events_init()) {
            javacall_printf(LOG_STR_LEAVE("fail, to init javacall events"));
            return JAVACALL_FAIL;
        }
    }

    uiResult = SCI_GetMutex(g_eventPoolMutex, SCI_WAIT_FOREVER);
    if(uiResult != SCI_SUCCESS) {
        javacall_printf(LOG_STR_LEAVE("fail, to get event mutex"));
            return JAVACALL_FAIL;
        }

    if (MaxEventNumber - 1 == head)
        next = 0;
    else
        next = head + 1;    
    if (tail == next) {
        SCI_PutMutex(g_eventPoolMutex);
        javacall_printf(LOG_STR_LEAVE("fail. ******** ERROR *******, event pool full!"));
        return JAVACALL_FAIL;
    }

    eventPool[head].size = binaryBufferLen;
    eventPool[head].data = NULL;
    eventPool[head].data = javacall_malloc(binaryBufferLen);
    if (!eventPool[head].data) {
        SCI_PutMutex(g_eventPoolMutex);
        javacall_printf(LOG_STR("[WARNING]javacall_event_send failed for not enough memory\n"));
        return JAVACALL_FAIL;
    }
    SCI_MEMCPY(eventPool[head].data, binaryBuffer, eventPool[head].size);
    head = next;
    SCI_PutMutex(g_eventPoolMutex);
    #ifdef USE_SCI_SEMA
    //javacall_printf(LOG_STR("javacall_event_send before put"));
    SCI_PutSemaphore(sema);
    #else
    //javacall_printf(LOG_STR("javacall_event_send before put"));
    tx_semaphore_put(sema);
    #endif
    // SCI_TRACE_LOW("Semaphore set returns %d\n", result);
    
    return JAVACALL_OK;
}    

/**
 * CDC-specific function.
 * Waits for an incoming event in the queue with the given ID and copies it to
 * a user supplied data buffer.
 *
 * @param queueId identifier of an event queue, typically a JSR number.
 * @param binaryBuffer user-supplied buffer to copy event to.
 * @param binaryBufferMaxLen maximum buffer size that an event can be
 *              copied to.
 * @param outEventLen user-supplied pointer to variable that will hold actual 
 *              event size received, or desired buffer size if
 *              binaryBufferMaxLen is insufficient.
 *              If outEventLen is NULL, the event size is not returned.
 * @return <tt>JAVACALL_OK</tt> if an event successfully received,
 *         <tt>JAVACALL_OUT_OF_MEMORY</tt> if event size exceeds
 *         binaryBufferMaxLen,
 *         <tt>JAVACALL_FAIL</tt> on any other error.
 */
javacall_result javacall_event_receive_cvm(int queueId,
    /*OUT*/ unsigned char *binaryBuffer, int binaryBufferMaxLen,
    /*OUT*/ int *outEventLen) {
    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * CDC-specific function.
 * Copies a user supplied event message to a queue of messages and wakes up the
 * thread that is waiting for events on the queue with the given id.
 *
 * @param queueId identifier of an event queue, typically a JSR number.
 * @param binaryBuffer a pointer to binary event buffer to send. The first int
 *        is the event id.
 * @param binaryBufferLen size of binary event buffer to send.
 * @return <tt>JAVACALL_OK</tt> if an event has been successfully sent,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result javacall_event_send_cvm(int queueId,
    unsigned char* binaryBuffer, int binaryBufferLen) {
    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * The function is called during Java VM startup, allowing the
 * platform to perform specific initializations. It is called in the same
 * process as javacall_event_receive() and javacall_events_finalize().
 *
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise
 */
javacall_result javacall_events_init(void) {  
    if (JAVACALL_TRUE == initialized)
    {
        javacall_printf(LOG_STR_LEAVE("ok, already initialezed"));
        return JAVACALL_OK;
    }
    #ifdef USE_SCI_SEMA
    if (SCI_NULL == (sema = SCI_CreateSemaphore("java event", 0)))
    #else
    if (TX_SUCCESS != tx_semaphore_create(sema, "java event", 0))
    #endif
    {
        javacall_printf(LOG_STR_LEAVE("fail, to create semaphore"));
        return JAVACALL_FAIL;
    }
    g_eventPoolMutex = (SCI_MUTEX_PTR)SCI_CreateMutex( "java event", SCI_INHERIT);
    if (g_eventPoolMutex == SCI_NULL)
    {
        javacall_printf(LOG_STR_LEAVE("fail, to create mutex for event pool"));
        return JAVACALL_FAIL;
    }
    head = 0;
    tail = 0;
    initialized = JAVACALL_TRUE;
    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/**
 * The function is called during Java VM shutdown, allowing the platform to
 * perform specific events-related shutdown operations. It is called in the same
 * process as javacall_events_init() and javacall_event_receive().
 *
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise
 */
javacall_result javacall_events_finalize(void) {
    if (JAVACALL_TRUE == initialized) {
        #ifdef USE_SCI_SEMA
        SCI_DeleteSemaphore(sema);
        #else
        tx_semaphore_delete(sema);
        #endif
        if (g_eventPoolMutex != SCI_NULL)
            SCI_DeleteMutex((SCI_MUTEX_PTR)g_eventPoolMutex);
        initialized = JAVACALL_FALSE;
    }
    
    return JAVACALL_OK;
}

#ifdef __cplusplus
}
#endif

