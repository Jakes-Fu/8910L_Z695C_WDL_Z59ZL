/*
 * Copyright 2004-2006 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Wireless Messaging API (VM-independent)
 *
 * This interface provides some of the wireless messaging support for
 * the Jbed VM. These functions are the CPL level interfaces that are
 * supported by the VM.
 *
 * Note that this interface mostly comprises the shared subset of the
 * interfaces between the push registry and the WMA send/receive
 * interfaces. It is therefor largely agnostic of the actual structure
 * of the messages. All it requires is that the message structure has
 * field for chaining in a linked list. The bulk of the actual sending
 * and receiving of messages in normal use is done by the JPL of
 * <ul>
 *    <li>javax.microedition.io.SmsConnectionThread</li>
 *    <li>javax.microedition.io.sms_Protocol</li>
 * </ul>
 * The terms "message" and "ports" refer here to the SMS messages as
 * defined in GSM 03.40 and the 8-bit or 16-ports defined in the user
 * header payload. CBS broadcast message are currently unsorted by
 * this interface.
 *
 * @see base/src/wma.c
 */

#ifndef _CPL_WMA_H
#define _CPL_WMA_H

#include <jbed_global.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define CPL_WMA_SUCCESS 0
#define CPL_WMA_FAILURE -1
#define CPL_WMA_WOULDBLOCK -100

/**
 * @name WMA specific info flags
 * @ingroup midletinfoflags
 *
 * These flags are passed to indicate attributes about the port being opened
 * for SMS.
 */
/*@{*/
/* start at bit 16 since the lower int16_t is reserved for generic midlet info
 * bits */
/** Presence indicates port registration is for push \hideinitializer */
#define CPL_WMA_INFO_PUSH ((int32_t)1 << 16)
/** Presence indicates CBS port registration */
#define CPL_WMA_INFO_CBS ((int32_t)0x20000)
/*@}*/

/**
 * @name Shared functions
 *
 * Functions that are intented to shared between the push registry
 * interface and the normal WMA interface functions. This exposes
 * very little information about the content of the SMS payload.
 *
 * When called from the push registry these functions are called
 * synchronously so use of the AsyncIO functions is not allowed.
 *
 * At the moment only the push registry functions call this interface
 * directly.  Using the functions from the WMA JPL functions is up to
 * the port implementer.
 *
 * @{
 */
/**
 * Opaque SMS message structure.
 *
 * The first entry must be a pointer sized field, to allow for chaining
 * a queue. This field is read and written by the PushRegistry.
 */
typedef struct CPL_SmsMessage_s CPL_SmsMessage;

/**
 * @internal
 * check CPL_SmsMessages_s is defined appropriately.
 */
#define CPL_sms_assertNext() Iassert(&(((CPL_SmsMessage *)NULL)->next) == NULL)

/**
 * Register an SMS port.
 *
 * @param port Port number.
 * @param info flag word with context info (one of JBED_WMA_INFO_* or JBED_MIDLET_INFO_*)
 * @return CPL_WMA_SUCCESS on success, CPL_WMA_FAILURE on failure.
 */
int32_t CPL_sms_register(int32_t port, int32_t info);

/**
 * Deregister an SMS or CBS port.
 * If (info & CPL_WMA_INFO_PUSH == 0) CPL_sms_deregister can
 * be asynchronous.
 *
 * @param port Port number.
 * @param info flag word with context info (one of JBED_WMA_INFO_* or JBED_MIDLET_INFO_*)
 */
void CPL_sms_deregister(int32_t port, int32_t info);

/**
 * Poll an SMS or CBS port.
 *
 * The returned message will either be discarded via #CPL_sms_free or
 * queued inside the push registry for later retrieval by the
 * application. In that case it can be retrieved from the push
 * registry using push_checkout_sms as an opaque pointer.
 *
 * @see  JbniV_javax_microedition_io_SmsConnectionThread_nReadMessage
 * @see  push_checkout_sms
 *
 * @param port Port number.
 * @return an SMS message or NULL.
 */
CPL_SmsMessage *CPL_sms_poll(int port);

/**
 * Wake up SMS reader thread
 *
 * This should notify the notifier (if present) given in nReadMessage
 * as part of \ref wmajpl. It is used by the push registry
 * to notify it might have new messages queued for a checked out entry.
 *
 * N.B. This function should be safe to call at any time and not cause
 * double notifications.
 */
void CPL_sms_notifyReader(void);
/**
 * Get the source or destination number for a message.
 *
 * @param message SMS message.
 * @return the msisdn string.
 */
const char *CPL_sms_msisdn(CPL_SmsMessage *message);

/**
 * Discard an SMS message.
 *
 * @param message SMS message.
 */
void CPL_sms_free(CPL_SmsMessage *message);
/*@}*/

/**
 * Initializes wma:
 *    - sets up wireless.messaging.sms.smsc property
 */
void CPL_wma_init(void);

/**
 * Shutdown the sms device if it is up and running.
 *
 * This is called from the VM finalization code.
 */
void CPL_wma_shutdown(void);

#ifdef JBED_SMS_PERSISTENCE
/**
 * Turn the SMS into an array of bytes.
 *
 * This should be such that the message can be recreated
 * using CPL_sms_deserialize at any time, including after reboot.
 *
 * @param message Message to be serialized
 * @param buf Buffer to serialize to
 * @param len size of 'buf' in bytes
 *
 * @return length of the serialized form (or -1 on failure)
 */
int CPL_sms_serialize(CPL_SmsMessage *message,unsigned char *buf, int len);

/**
 * Recreate an SMS message from an array of bytes
 *
 * The input is a copy of the bytes previously created from
 *
 * @param buf Buffer containing serialized form of message
 * @param len lenght of the serialized data
 *
 * @return Deserlized SMS message (as if returned from CPL_sms_poll).
 *         NULL on failure.
 */
CPL_SmsMessage *CPL_sms_deserialize(const unsigned char *buf,int len);

/**
 * Get maximum size of serialized form of a message.
 *
 * @return maximum size required for CPL_sms_serialize in bytes;
 */
int CPL_sms_maxSerializedLength(void);
#endif /* JBED_SMS_PERSISTENCE */

#if defined(__cplusplus)
}
#endif

#endif /* _CPL_WMA_H */

