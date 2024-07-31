/*
 * Copyright 2003,2004 Aplix Corporation. All rights reserved.
 */


/**
 * @file AMWirelessMessagingMms.h
 *  MMS Communication JBI Header File
 */

/**
 * @addtogroup WMA_MMS_JBI
 * @{
 */
#ifndef __AM_WIRELESSMESSAGING_MMS_H__
#define __AM_WIRELESSMESSAGING_MMS_H__

#include "jkwirelessmessagingtypedef.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 *  Event type definitions
 */

/**
 * Message sending was completed.\ 
 * This is used for notifying JBlend of the processing result of JkWmaMmsSendMessage().
 */
#define	AM_WMA_MMS_EVENT_MESSAGE_SENT       1

/**
 * Message sending timed out.\ 
 * This is used for notifying JBlend of the processing result of JkWmaMmsSendMessage().
 */
#define	AM_WMA_MMS_EVENT_TIMEOUT  2

/**
 * A message arrived.
 */
#define	AM_WMA_MMS_EVENT_MESSAGE_ARRIVED  3

/**
 * A communication error occurred.\ 
 * This is used for notifying JBlend of the processing result of JkWmaMmsSendMessage(), and of the
 * communication error occurred in the native system.
 */
#define	AM_WMA_MMS_EVENT_IO_ERROR       4

/**
 * Message writing was completed.\ 
 * This is used for notifying JBlend of the processing result of JkWmaMmsReceiveMessage().
 */
#define	AM_WMA_MMS_EVENT_MESSAGE_WRITING_COMPLETE   5

/**
 * Message writing was failed.\ 
 * This is used for notifying JBlend of the processing result of JkWmaMmsReceiveMessage().
 */
#define	AM_WMA_MMS_EVENT_MESSAGE_WRITING_ERROR      6


/**
 *        Notifies JBlend of an event occurring on a Wireless Messaging API connection.
 * @param connectionHandle
 *        Handle of the connection on which the event occurred.
 *        Set the handle value issued by #JkWmaMmsOpen().
 * @param event
 *        The event identifier for notification.<br>
 *        When a message arrives from outside, designate
 *        #AM_WMA_MMS_EVENT_MESSAGE_ARRIVED.<br>
 *        When a communication error occurred in the native system, 
 *        designate #AM_WMA_MMS_EVENT_IO_ERROR.<br>
 *        For other cases, set one of the following depending on the JSI function 
 *        which triggered event notification.
 *        <dl>
 *         <dt>JkWmaMmsSendMessage()</dt>
 *          <dd><dl>
 *           <dt>#AM_WMA_MMS_EVENT_MESSAGE_SENT</dt>
 *            <dd>Message sending was completed.</dd>      
 *           <dt>#AM_WMA_MMS_EVENT_TIMEOUT</dt>
 *            <dd>Message sending timed out.</dd>      
 *           <dt>#AM_WMA_MMS_EVENT_IO_ERROR</dt>
 *            <dd>A communication error occurred.</dd>   
 *          </dl></dd>
 *         <dt>JkWmaMmsReceiveMessage()</dt>
 *          <dd><dl>
 *           <dt>#AM_WMA_MMS_EVENT_MESSAGE_WRITING_COMPLETE</dt>
 *            <dd>Message writing was completed</dd>      
 *           <dt>#AM_WMA_MMS_EVENT_MESSAGE_WRITING_ERROR</dt>
 *            <dd>Message writing was failed</dd>     
 *          </dl></dd>
 *        </dl>
 * @param option
 *        Optional parameters for each notification event.
 *        This is reserved for future use. JBlend currently does not refer to this value.
 *
 * Use this function to notify JBlend of the following.
 * - Result of the multipart message sending started by JkWmaMmsSendMessage().
 * - Result of the received message transfer started by JkWmaMmsReceiveMessage().
 * - Arrival of a message.
 * - Communication error occurring in the native system.
 * .
 *
 */
void AMWmaMmsEventPost( JKHandle connectionHandle, int event, JKSint32 option);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__AM_WIRELESSMESSAGING_MMS_H__ */

/** @} end of group */

