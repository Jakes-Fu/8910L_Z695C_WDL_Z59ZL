/*
 * Copyright 2015 Myriad Group AG. All Rights Reserved.
 * $Id: $
 */

 #ifndef __MS_MSA_H__
 #define __MS_MSA_H__

#include <jbed_upcall.h>
#include <javastatic.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define MSA_OP_GET     (0)
#define MSA_OP_REFRESH (1)
#define MSA_OP_SIGNOFF (2)


/* CPL MSA result for create event queue */
#define CPL_MSA_FAILURE (-1)
#define CPL_MSA_SUCCESS (0)

/* the maximum size of a token buffer, refer to MAX_TOKEN_STR_SIZE */
#define MSA_TOKEN_MAX_SIZE  (1600)

/* Assume there is only one event once */
#define MSA_EVENTS_NUM     (1)

#define MSA_EVENT_SIZE     (JBED_UPCALL_MINEVENTSIZE+JBED_UPCALL_OBJECTHEADERSIZE+JBED_UPCALL_STRINGSIZE(MSA_TOKEN_MAX_SIZE))

#define MSA_EVENT_QUEUE_SIZE (JBED_UPCALL_SPAREQUEUESPACE + (MSA_EVENTS_NUM * MSA_EVENT_SIZE))

/* MSA event Id */
#define MSA_UPCALL_EVENT STATIC_com_ms_service_msa_MSAManagerImpl_MSA_UPCALL_EVENT

/**
 * Initialize msa upcall event queue.
 */
int Jbed_msa_createEventQueue();

/**
 * Delete msa upcall event queue.
 */
void Jbed_msa_deleteEventQueue();

/**
 * Notify running application MSA is signed of by other applications.
 */
void Jbed_msa_notifySignOff();



#if defined(__cplusplus)
}
#endif

 #endif//__MS_MSA_H__
