/*
 * Copyright 2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */
#ifndef __PUSH_WMA_H__
#define __PUSH_WMA_H__

#include <jbed_global.h>
#include <pushregistry.h>

#if defined(SUPPORTS_PUSH_SMS) || defined(SUPPORTS_PUSH_CBS)
#include <cpl_wma.h>
#endif

#ifdef SUPPORTS_PUSH_MMS
#include <cpl_wma_mms.h>
#endif

#if defined (__cplusplus)
extern "C" {
#endif

#if defined(SUPPORTS_PUSH_SMS) || defined(SUPPORTS_PUSH_CBS)
typedef struct _smsentry
{
  struct CPL_SmsMessage_s *next;
  struct CPL_SmsMessage_s *newMessage; /* Point to a sms message that hasn't been checked the filter. */
} SmsEntry;
#endif /* defined(SUPPORTS_PUSH_SMS) || defined(SUPPORTS_PUSH_CBS) */

#ifdef SUPPORTS_PUSH_MMS
typedef struct _mmsentry
{
  CPL_mms_pushMmsInfo_t *next; /* pending MMS message queue */
  CPL_mms_pushMmsInfo_t *newMessage; /* Point to a mms message that hasn't been checked the filter. */
  jchar appID[JBED_MMS_MAX_APP_ID_LEN];
  int16_t appIDlen;
} MmsEntry;
#endif /* SUPPORTS_PUSH_MMS */

/**
 * @name WMA push related functions
 */
/*@{*/
#ifdef SUPPORTS_PUSH_SMS
/**
 * Use an existing SMS port opened by the push registry for a real connection.
 *
 * When an SMS port is opened the native code implementing the
 * connection (e.g. \ref wmajpl) should call this function to retrieve
 * the handle for the already opened port from the push registry.
 *
 * Get a handle for an SMS port that is already open for push:
 *    - If this exists and the current midlet has access the
 *       SMS read notifier is notified and
 *       corresponding entry i is returned.
 *    - If the port is opened for push but the midlet has no access
 *      a pending JBNI exception is set the current native method is
 *      exited abrubtly. <em>This function does not return</em>.
 *    - If the port is not for opened push, NULL is returned
 *      In this case WMA code should open the port directly
 *
 * When the MIDlet closes a connection retrieved in this way
 * in this way, it must be handed back to push registry with push_checkin
 * (rather than closed). Additionally any queued messages be retrieved
 * using the push_checkout_sms (rather than directly).
 *
 * The handle obtained should be returned to the push registry
 * by calling Jbed_push_hand_back() when the MIDLet is done with it.
 *
 * @param  port SMS port check if opened push
 * @return handle to corresponding push entry or NULL if no such entry exists
 * @throws if an exception occured accessing the port this function does not exit normally.
 *
 */
PushEntry *Jbed_push_sms_get_open_port(int32_t port);

/**
 * Check if a SMS port has been opened by the push registry.
 *
 * @param  port SMS port check if opened push
 * @return TRUE to corresponding push entry or FALSE if no such entry exists
 *
 */
bool_t Jbed_push_sms_is_push_port(int32_t port);

#endif

#ifdef SUPPORTS_PUSH_CBS
/**
 * Use an existing CBS port opened by the push registry for a real connection.
 *
 * When an CBS port is opened the native code implementing the
 * connection (e.g. \ref wmajpl) should call this function to retrieve
 * the handle for the already opened port from the push registry.
 *
 * Get a handle for an CBS port that is already open for push:
 *    - If this exists and the current midlet has access the
 *       CBS read notifier is notified and
 *       corresponding entry i is returned.
 *    - If the port is opened for push but the midlet has no access
 *      a pending JBNI exception is set the current native method is
 *      exited abrubtly. <em>This function does not return</em>.
 *    - If the port is not for opened push, NULL is returned
 *      In this case WMA code should open the port directly
 *
 * When the MIDlet closes a connection retrieved in this way
 * in this way, it must be handed back to push registry with push_checkin
 * (rather than closed). Additionally any queued messages be retrieved
 * using the push_checkout_sms (rather than directly).
 *
 * The handle obtained should be returned to the push registry
 * by calling Jbed_push_hand_back() when the MIDLet is done with it.
 *
 * @param  port CBS port check if opened push
 * @return handle to corresponding push entry or NULL if no such entry exists
 * @throws if an exception occured accessing the port this function does
 * notexit normally.
 *
 */
PushEntry *Jbed_push_cbs_get_open_port(int32_t port);

/**
 * Check if a CBS port has been opened by the push registry.
 *
 * @param  port CBS port check if opened push
 * @return TRUE to corresponding push entry or FALSE if no such entry exists
 *
 */
bool_t Jbed_push_cbs_is_push_port(int32_t port);
#endif

#ifdef SUPPORTS_PUSH_MMS
/**
 * Use an existing MMS application ID opened by the push registry for a real connection.
 *
 * When an MMS application ID is registeres the native code implementing the
 * connection (e.g. \ref wmajpl) should call this function to retrieve
 * the handle for the already opened application ID from the push registry.
 *
 * Get a handle for an MMS application ID that is already open for push:
 *    - If this exists and the current midlet has access the
 *       MMS read notifier is notified and
 *       corresponding entry i is returned.
 *    - If the application ID is registered for push but the midlet has no access
 *      a pending JBNI exception is set the current native method is
 *      exited abrubtly. <em>This function does not return</em>.
 *    - If the application ID is not for opened push, NULL is returned
 *      In this case WMA code should open the port directly
 *
 * When the MIDlet closes a connection retrieved in this way
 * in this way, it must be handed back to push registry with push_checkin
 * (rather than closed). Additionally any queued messages be retrieved.
 *
 * The handle obtained should be returned to the push registry
 * by calling Jbed_push_hand_back() when the MIDLet is done with it.
 *
 * @param  appID MMS application ID check if opened push (UCS2)
 * @param  appIDlen MMS application ID lenght in chars
 * @return handle to corresponding push entry or NULL if no such entry exists
 * @throws if an exception occured accessing the appID this function does not exit normally.
 *
 */
PushEntry *Jbed_push_mms_get_open_port(const jchar *appID, int32_t appIDlen);
#endif

#if defined(SUPPORTS_PUSH_CBS) || defined(SUPPORTS_PUSH_SMS)
/**
 * Check for any queued inbound messages on the port associated
 * with the push entry.
 *
 * The messages returned are those previously returned from CPL_sms_poll
 * @param pe Push entry as previously returned from Jbed_push_sms_get_open_port()
 * @return Queued SMS message or NULL if none.
 */
extern CPL_SmsMessage *Jbed_push_sms_get_queued(PushEntry *pe);
#endif

#ifdef SUPPORTS_PUSH_MMS
/**
 * Check for any queued inbound messages on the application ID associated
 * with the push entry.
 *
 * The messages returned are those previously returned from CPL_mms_poll
 * @param pe Push entry as previously returned from Jbed_push_mms_get_open_port()
 * @param remove Indicate whether remove the mms in the pe from the queue.
 * @return Queued MMS message or NULL if none.
 */
extern CPL_mms_pushMmsInfo_t *Jbed_push_mms_get_queued(PushEntry *pe, jboolean remove);
#endif
/*@}*/


#ifdef SUPPORTS_PUSH_SMS
unsigned int push_sms_create(PushEntry *pe);
unsigned int push_sms_add(PushEntry *pe, PushEntry **head);
int push_sms_init(PushEntry *pe);
int push_sms_open(PushEntry *pe);
int push_sms_watch(PushEntry *pe);
void push_sms_discard(PushEntry *pe);
void push_sms_close(PushEntry *pe);
unsigned int push_sms_check_filter(PushEntry *pe);
unsigned int push_sms_check_port(PushEntry *pe, ParamForCheckPort_t *params);
void push_start_sms_connection(PushEntry *pe);
void push_sms_store_info(PushEntry *pe, int handle);
void push_sms_revive_info(PushEntry *pe, const char *info);
unsigned int push_sms_hasConn(PushEntry *pe);
#endif /* SUPPORTS_PUSH_SMS */

#ifdef SUPPORTS_PUSH_CBS
unsigned int push_cbs_create(PushEntry *pe);
unsigned int push_cbs_add(PushEntry *pe, PushEntry **head);
int push_cbs_init(PushEntry *pe);
int push_cbs_open(PushEntry *pe);
int push_cbs_watch(PushEntry *pe);
void push_cbs_discard(PushEntry *pe);
void push_cbs_close(PushEntry *pe);
unsigned int push_cbs_check_filter(PushEntry *pe);
unsigned int push_cbs_check_port(PushEntry *pe, ParamForCheckPort_t *params);
void push_start_cbs_connection(PushEntry *pe);
void push_cbs_store_info(PushEntry *pe, int handle);
void push_cbs_revive_info(PushEntry *pe, const char *info);
unsigned int push_cbs_hasConn(PushEntry *pe);
#endif /* SUPPORTS_PUSH_CBS */

#ifdef SUPPORTS_PUSH_MMS
unsigned int push_mms_create(PushEntry *pe);
unsigned int push_mms_add(PushEntry *pe, PushEntry **head);
int push_mms_init(PushEntry *pe);
int push_mms_open(PushEntry *pe);
int push_mms_watch(PushEntry *pe);
void push_mms_discard(PushEntry *pe);
void push_mms_close(PushEntry *pe);
unsigned int push_mms_check_filter(PushEntry *pe);
unsigned int push_mms_check_port(PushEntry *pe, ParamForCheckPort_t *params);
void push_start_mms_connection(PushEntry *pe);
void push_mms_store_info(PushEntry *pe, int handle);
void push_mms_revive_info(PushEntry *pe, const char *info);
unsigned int push_mms_hasConn(PushEntry *pe);
#endif /* SUPPORTS_PUSH_MMS */

#if defined(__cplusplus)
}
#endif

#endif /* __PUSH_WMA_H__ */
