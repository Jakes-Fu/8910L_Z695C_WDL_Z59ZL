/*
 * Copyright 2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */
#ifndef __TARGPUSH_H__
#define __TARGPUSH_H__

#if defined(SUPPORTS_PUSH_SMS) || defined(SUPPORTS_PUSH_CBS)

/*
 * The maximum size of the queue which determines how many messages can be
 * pending before the launched MIDlet reads these messages.
 */
#define PUSH_SMS_MAX_QUEUE_SIZE 20

/*
 * in sprint ams v2.2.2 section 4.3.3 specify that the queue for midlets
 * must be exactly 10.
 */
#define PUSH_SPRINT_SMS_MAX_QUEUE_SIZE 10

/* If there is a pending SMS message, it is stored in a target defined
 * structure (CPL_SmsMessage) and attached to the pushentry. The first field
 * must be 'next' but the target will normally define more entries in this
 * structure.
 */

#endif /* defined(SUPPORTS_PUSH_SMS) || defined(SUPPORTS_PUSH_CBS) */

#ifdef SUPPORTS_PUSH_MMS
/*
 * The maximum size of the queue which determines how many messages can be
 * pending before the launched MIDlet reads these messages.
 */
#define PUSH_MMS_MAX_QUEUE_SIZE 20
#endif

#endif /* __TARGPUSH_H__ */
