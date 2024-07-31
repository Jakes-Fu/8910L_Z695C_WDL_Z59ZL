/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

#ifndef KJAVA_SYS_HEADER
#define KJAVA_SYS_HEADER

/**
 * @file kjava_sys.h
 * Inter Thread Communication
 */

#ifdef __cplusplus
extern "C"{
#endif

#include "kjava_action.h"

/** @addtogroup task_control_ksi
 *  @{
 */

/*
 *  Interfaces listed in the group is used for inter-thread communication,
 *  including:
 *      - Send an message to another thread/task.
 *      - Receive an message from the queue of thread itself.
 *      .
 */

/** 
 * @addtogroup Recv_Send_Msg_KSI Receive and Send Message
 * @{
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/** \brief AMS(MMI) thread */
#define KJAVA_AMS_THREAD (1)
/** \brief KVE (KJava VM Execution) thread */
#define KJAVA_KVE_THREAD (2)
/** \brief Reserved thread for network */
#define KJAVA_NET_THREAD (3)
/** \brief The offset of user-defined thread.
 *
 *         The #KJAVA_AMS_THREAD, #KJAVA_KVE_THREAD and 
 *         #KJAVA_NET_THREAD are the three and the only
 *         three assumption thread/task that must exist. However, when
 *         user want to define other thread/task such as SMS
 *         thread, they can use all the thread ID that equal to or greater 
 *         than #KJAVA_USER_THREAD.
 */
#define KJAVA_USER_THREAD (100)

/** \brief A request for starting the JBlend JVM */
#define KJAVA_ACTION_TYPE_START_VM        (1)
/** \brief A request for resuming the JBlend JVM */
#define KJAVA_ACTION_TYPE_RESUME_VM       (2)
/** \brief AJMS action that would use memory allocation */
#define KJAVA_ACTION_TYPE_AJMS_MEM        (3)
/** \brief AJMS action that will not use memory allocation */
#define KJAVA_ACTION_TYPE_AMS_NOMEM       (4)
/** \brief HTTP request action */
#define KJAVA_ACTION_TYPE_HTTPLIB         (5)
/** \brief TCK request action */
#define KJAVA_ACTION_TYPE_TCK             (6)
/** \brief The offset of user-defined action type.
 *
 *         When user want to define action types other than the prefined
 *         action type, they can define action type ID that equal to or 
 *         greater than #KJAVA_USER_THREAD.
 */
#define KJAVA_ACTION_TYPE_USER            (100)

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/**
 * \brief Sends an action command to the target thread, with the given
 *        whole action command.
 * @param target
 *        The identifier of the destination thread. The possible values are 
 *        as follows:
 *                  - #KJAVA_AMS_THREAD
 *                  - #KJAVA_KVE_THREAD
 *                  - #KJAVA_AMS_THREAD
 *                  - Other user defined thread ID that greater than or
 *                    equal to #KJAVA_USER_THREAD.
 *                  .
 * @param actionCommand_p
 *        A pointer to the #KJavaActionCommand structure holding the 
 *        command information to be sent.
 *
 * JBlendia calls this function to send an action command to the destination thread.
 *
 * The function should create a carrier used for the native inter-thread communication,
 * copy the content of given #KJavaActionCommand into the carrier, and send the
 * carrier to the destination thread indicated in <i>target</i>. 
 */
void kjava_sys_sendAction(int target, KJavaActionCommand *actionCommand_p);

/**
 * \brief Retrieves an action command from the KVE thread queue.
 *
 * NOTE: This function is deprecated and only be linked by special AJSC library
 *       with compatible reason or customer request.
 *
 * @param actionCommand_pp
 *        A pointer to the buffer for putting a pointer to the #KJavaActionCommand 
 *        structure that contains the retrieved action command.
 * @return
 *        Return 1 if the function succeeds;
 *        return 0 if it fails.
 *
 * JBlendia calls this function to retrieve an action command from the 
 * message recieve queue of the KVE thread.
 *
 * This function should not return until an action command is retrieved or 
 * an error occurrs.
 *
 * When this function succeeds, the pointer put in the buffer indicated by 
 * <i>actionCommand_pp</i> must remain valid until next time this function is called.
 *
 * This function is called only in the KVE thread context.
 *
 * This function usually be called when VM needs to be in idle. In order words, when
 * KVE has been blocked inside this function, platform should make sure the KVE is not
 * in busy loop for waiting the message.
 */
int kjava_sys_receiveAction(KJavaActionCommand **actionCommand_pp);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_HEADER */

