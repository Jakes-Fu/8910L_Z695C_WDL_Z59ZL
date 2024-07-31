/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

/**
 *  @file kjava_sys_sock_action.h
 *  Action definition for socket operations
 */

#ifndef KJAVA_SYS_SOCK_ACTION_HEADER
#define KJAVA_SYS_SOCK_ACTION_HEADER

#ifdef __cplusplus
extern "C"{
#endif


enum _KJavaSocketActionTypeEnum
{
    KJAVA_SOCKET_ACTION_NULL = 0,

    KJAVA_SOCKET_ACTION_OPEN,
    KJAVA_SOCKET_ACTION_CONNECT,
    KJAVA_SOCKET_ACTION_SEND,
    KJAVA_SOCKET_ACTION_RECEIVE,
    KJAVA_SOCKET_ACTION_SEN_RCV,
    KJAVA_SOCKET_ACTION_AVAILABLE,
    KJAVA_SOCKET_ACTION_SHUTDOWN,
    KJAVA_SOCKET_ACTION_CLOSE,
    KJAVA_SOCKET_ACTION_SETOPTION,
    KJAVA_SOCKET_ACTION_GETOPTION,

    _TOTAL_KJAVA_SOCKET_ACTION_COUNT
};

/**
 *  Used only for debugging
 */
static char *kjSocketActionName[] =
{
    "KJAVA_SOCKET_ACTION_NULL",

    "KJAVA_SOCKET_ACTION_OPEN",
    "KJAVA_SOCKET_ACTION_CONNECT",
    "KJAVA_SOCKET_ACTION_SEND",
    "KJAVA_SOCKET_ACTION_RECEIVE",
    "KJAVA_SOCKET_ACTION_AVAILABLE",
    "KJAVA_SOCKET_ACTION_SHUTDOWN",
    "KJAVA_SOCKET_ACTION_CLOSE",
    "KJAVA_SOCKET_ACTION_SETOPTION",
    "KJAVA_SOCKET_ACTION_GETOPTION",
};


/**
 *  \brief  Structure for socket action
 *
 */
typedef struct _KJavaSocketCommandTag
{
    int     connectionID;
    int     actionID;
    
    char    params[144];
}
KJavaSocketCommand;


#ifdef __cplusplus
}
#endif 

#endif  /* KJAVA_SYS_SOCK_ACTION_HEADER */
