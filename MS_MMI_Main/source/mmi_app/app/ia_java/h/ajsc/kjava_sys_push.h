/*
 * Copyright 2005 Aplix Corporation. All rights reserved.
 */

/***************************************************************************
 * FILE: kjava_sys_push.h
 *
 * DESCRIPTION:
 *      The Push registry interface of KSI.
 *
 * UPDATED DATE:
 *      2005/01/11
 *
 * REVISION:
 *  2005/01/11, Jack Chung, First draft.
 *
 *
 ***************************************************************************/

#ifndef KJAVA_SYS_PUSH_HEADER
#define KJAVA_SYS_PUSH_HEADER
#ifdef JAVA_SUPPORT_IA
#include "sci_types.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file kjava_sys_push.h
 */
 
/** @addtogroup push_registry_ksi
 *  @{
 */ 
 
/**
 *  @addtogroup Push_Registry_Alarm Alarm
 *  @{
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/**
 * \brief Start an alarm.
 *
 * The function informs the native system to start an alarm at the given time.
 *
 * The time is stored in the structure as elapsed milliseconds from
 * 00:00:00.000 GMT, January 1, 1970.
 *
 * The native system should be informed when the alarm expires, and uses AJMS
 * functions to launch the MIDlet associated with the alarm.
 *
 * @param suiteId  The AJMS midlet suite identifier.
 * @param midletId The AJMS midlet index for the <i>suiteId</i>.
 * @param high     The higher 32 bits of the time information.
 * @param low      The lower 32 bits of the time information.
 *
 * @return If the operation is successful, return 1. Otherwise, return 0.
 */
int kjava_push_startAlarm(int suiteId, int midletId, unsigned long high, unsigned long low);

/** @} */

/**
 *  @addtogroup Push_Registry_Connection Connection
 *  @{
 */

/**
 * \brief Stop a push connection.
 *
 * The function informs the native system to stop the Push connection of the
 * given protocol and port.
 *
 * @param protocol       The connection protocol name in utf-8 string with 
 *                       null-terminated character, such as 
 *                       <i>sms://</i>, <i>socket://</i> or <i>datagram://</i>.
 * @param port           The local port number associated with the protocol.
 *
 * @return If the operation is successful, return 1. Otherwise, return 0.
 */
int kjava_push_stopConn(unsigned char* protocol, int port);

/**
 * \brief Bind a push connection with an installed midlet suite.
 *
 * The function informs the native system to start a Push connection
 * monitoring of the given protocol and port number.
 *
 * The native system should be informed when an incoming connection is created
 * from the specified protocol and port, and uses AJMS functions to launch 
 * the MIDlet associated with the connection.
 *
 * A filter string is also specified so native system could use it to filter
 * wanted inbound connections. The syntax and semantic is protocol depedent. 
 * But wildcard characters should be supported. Please refer to the
 * spec of <code>javax.microedition.io.PushRegistry</code> for detail
 * information.
 *
 * @param suiteId  The AJMS midlet suite identifier.
 * @param midletId The AJMS midlet index for the <i>suiteId</i>.
 * @param protocol       The connection protocol name in utf-8 string with 
 *                       null-terminated character, such as 
 *                       <i>sms://</i>, <i>socket://</i> or <i>datagram://</i>.
 * @param port     The local port number associated with the protocol type.
 * @param filter   The URL filter string, in UTF-8 characters. 
 *
 * @return If the operation is successful, return 1. Otherwise, return 0.
 */
int kjava_push_startConn(int            suiteId, 
                         int            midletId, 
                         unsigned char* protocol, 
                         int            port, 
                         unsigned char* filter);

/**
 * \brief Check if the specified protocol and port is allowed to be used by 
 *        connection.
 *
 * @param protocol       The connection protocol name in utf-8 string with 
 *                       null-terminated character, such as 
 *                       <i>sms://</i>, <i>socket://</i> or <i>datagram://</i>.
 * @param port     The local port number associated with the protocol type
 *
 * @return 1 for available and 0 for not allowed.
 */
int kjava_push_isAvailable(unsigned char* protocol, unsigned int port);

/*****************************************************************************/
//  Description : LoadPushDataBase
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_getConnByPort(int32 port);
#ifdef __cplusplus
}
#endif
#endif
#endif
