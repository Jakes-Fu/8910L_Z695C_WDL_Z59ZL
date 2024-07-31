/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

#ifndef KJAVA_ACTION_HEADER
#define KJAVA_ACTION_HEADER

/**
 *  @file kjava_action.h
 *  Action Command
 */

#ifdef __cplusplus
extern "C"{
#endif

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/
 
/** 
 * @addtogroup task_control_ksi
 * @{
 */

/** \brief The length in bytes of the <i>action</i> buffer of #KJavaActionCommand structure. */
#define KJAVA_ACTION_BUFFER_LENGTH  200

/** \brief The structure type for representing an action command which is 
 *  carried by the carrier of platform inter-thread communication.
 */
typedef struct KJavaActionCommandTag
{
    /** \brief The source thread that sends the action command.
     * 
     *        The identifier of the destination thread. The possible values are 
     *        as follows:
     *            - #KJAVA_AMS_THREAD
     *            - #KJAVA_KVE_THREAD
     *            - #KJAVA_NET_THREAD
     *            - Other user defined thread ID that greater than or
     *              equal to #KJAVA_USER_THREAD.
     *            .
     */
    int     sourceThread;
    
    /** \brief The action type.
     * 
     *         The identifier of action type. The possible values are 
     *         as follows:
     *        - #KJAVA_ACTION_TYPE_START_VM
     *        - #KJAVA_ACTION_TYPE_RESUME_VM
     *        - #KJAVA_ACTION_TYPE_AJMS_MEM
     *        - #KJAVA_ACTION_TYPE_AJMS_NOMEM
     *        - #KJAVA_ACTION_TYPE_HTTPLIB
     *        - #KJAVA_ACTION_TYPE_TCK
     *        - Other user defined action type ID that greater than or
     *          equal to #KJAVA_ACTION_TYPE_USER.
     *        .
     */
    int     type;
    
    /** \brief The payload of the action. */
    unsigned char action[KJAVA_ACTION_BUFFER_LENGTH];
}
KJavaActionCommand;

/** @} */

#ifdef __cplusplus
}
#endif 

#endif /* KJAVA_SYS_HEADER */

