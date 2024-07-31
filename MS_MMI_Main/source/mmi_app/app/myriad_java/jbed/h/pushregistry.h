/*
 * Copyright 2006-2009 Myriad Group AG. All Rights Reserved.
 * $Id$
 */
#ifndef __PUSHREGISTRY_H__
#define __PUSHREGISTRY_H__

#include <jbed_jbni.h> /* for string_instance */
#include <jbed_async.h>
#include <targ_push.h>

#if defined (__cplusplus)
extern "C" {
#endif

/* state of the connection.
 *
 * Some of the states don't seem to
 * make much sense, but are intended to allow work to be deferred.
 *
 * Transitions go roughly like :
 *
 * On startup, we open the connection. Success => WATCHING, FAILURE => IGNORE
 *
 * If new connection arrives, we test against the filter, and reject any that don't match.
 * FIXME: Even the MIDlet checked out the connection, should we still check the filter?
 *
 *  Otherwise advance conn_state to HIT
 *     if checked out, release thread if async_id set
 *     else notify ams of the connection
 *
 * When the AMS reports back success, NOTIFIED => LAUNCHING
 * When the AMS reports back defer, NOTIFIED => DEFERRED
 * When the AMS reports back rejection, discard and resume WATCHING.
 *
 *
 * When a MIDlet opens the connection, checked_out is set
 *
 * When MIDlet does an accept() / recv()
 *
 *  PRI_HIT   =>  PRI_WATCHING and return the connection
 *
 *  PRI_WATCHING => get async notifier and suspend thread
 *
 *
 * When MIDlet closes the connection (semantics say neither MIDlet nor
 * AMS watches the socket)
 *
 *  if async_id, signal notifier to release blocked thread
 *
 *  PRI_HIT      => discard connection and move to PRI_IGNORE(FIXME: Where are these codes?)
 *  PRI_WATCHING => PRI_IGNORE
 *
 *
 * During remove / install / upgrade, we need to be able to make tentative changes.
 *  - if delete is cancelled, restore state
 *  - if install finds a connection in use, abort(FIXME)
 *  - if update finds a new connection in use, abort and restore state(FIXME)
 *
 * The install_state field is used for this.
 * For upgrade or delete, we set bit 0 on all entries
 * During upgrade or install, we set bit 1 on all new entries.
 * Then on commit(), we actually make the changes.
 *
 * If on reset we find any in a transient state, we assume
 * an inpportune crash and assume an install/upgrade was
 * cancelled.
 */

enum conn_state_t
{
    PRI_CLOSED        = 0,  /* Connection not open */
    PRI_PREOPEN       = 1,  /* Waiting for network stack to initialise */
    PRI_OPENWAIT      = 2,  /* Waiting for the connection to open */
    PRI_IGNORE        = 3,  /* Open, but should not be watched */
    PRI_STARTWATCH    = 4,  /* Start watching */
    PRI_WATCHING      = 5,  /* Watch for incoming connection */
    PRI_HIT           = 6   /* Pending connection  */
#ifdef SUPPORTS_PUSH_SIP
,
    PRI_STARTRECEIVE  = 7
#endif
};

/* Various, orthogonal, state values. */

/* During a MIDlet install, we tentatively mark entries
 * as INSTALL_ADD. During an upgrade, we first mark
 * all connectors as DELETE. Then any ones that are also registered
 * in the updated version are marked UPDATE_KEEP. New connectors
 * are marked INSTALL_ADD.
 *
 * Similarly, during MIDlet runtime, we tentatively mark entries as
 * DYNAMIC_ADD.
 */
enum install_state_t
{
    PRI_DELETE        = 1,  /* Suite is being deleted / updated */
    PRI_INSTALL_ADD   = 2,  /* Suite is being installed / updated */
    PRI_UPDATE_KEEP   = 3,  /* Suite being upgraded */
    PRI_DYNAMIC_ADD   = 4   /* Dynamic registration */
};

/* Interaction with AMS
 *
 * State          Method/condition                                     New State[Action]
 * -----         ----------------                                   -----------------
 * Any                new msg                                                   PRI_INITIAL
 * Any                push_setup                                               PRI_INITIAL
 * PRI_INITIAL     Jbed_push_sendPushEvent & VM is running      PRI_NOTIFIED
 * PRI_NOTIFIED  vm_exit_or_reset                                        PRI_INITIAL
 * PRI_NOTIFIED  Impl_cancel/Ege midlet                                PRI_DEFERRED
 * PRI_NOTIFIED  Impl_cancel/Normal midlet                            PRI_INITIAL[message deleted]
 * PRI_NOTIFIED  Impl_launch0                                              PRI_LAUNCHED
 * PRI_NOTIFIED  Impl_lauchLater                                          PRI_LAUNCH_LATER
 * PRI_LAUNCHED midlet_death/checked out                            PRI_NOTIFIED
 * PRI_LAUNCHED midlet_death/not checked out                      PRI_DEFERRED
 * PRI_DEFERRED midlet_death                        PRI_DEFERRED
 * PRI_DEFERRED push_checkout                       PRI_NOTIFIED
 * PRI_LAUNCH_LATER midlet_death                    PRI_INITIAL
 *
 * NOTE: midlet_death also includes VM exit and VM reset. For MVM
 * build VM reset or exit implies death of all midlets.
 */
enum ams_state_t
{
    PRI_INITIAL       = 0,  /* Initial state */
    PRI_NOTIFIED      = 1,  /* We have told AMS about a timeout / hit */
    PRI_DEFERRED      = 2,  /* AMS / user deferred - renotify next time */
    PRI_LAUNCHED      = 3   /* AMS / user accepted - consumed next VM */
#ifdef SPRINT_SUPPORT
,
PRI_LAUNCH_LATER   = 4      /* relaunch next time when current running MIDlet exits */
#endif // SPRINT_SUPPORT
};

/**
 *@name Type codes for type of push entry.
 */
/*@{*/
#define PRI_DEAD      0 /**< place holder for dead entries */
#define PRI_ALARM     1 /**< entry for waking up at a given time */
#define PRI_SOCKET    2 /**< TCP server socket */
#define PRI_DATAGRAM  3 /**< UDP server socket */
#define PRI_SMS       4 /**< WMA Push to SMS port */
#define PRI_CBS       5 /**< WMA Push to CBS port */
#define PRI_MMS       6 /**< WMA Push to MMS port */
#define PRI_SIP       7 /**< Sip Push */
#define PRI_BLUETOOTH 8 /**< Bluetooth server connection */
#define PRI_EVENT     9 /**< MIDP3 Event system */
#define PRI_AUTOSTART 10/**< AUTO START PUSH */
#define PRI_NFC       11/**< NFC PUSH */
/*@}*/

/**
 *@name the result for a connection
 */
/*@{*/
#define PUSH_CONNECTION_SUCCESS 0
#define PUSH_CONNECTION_ERROR -1
#define PUSH_CONNECTION_WOULDBLOCK -100
/*@}*/

#define PUSH_INVALID_CONNECTION_ID -1
#define PUSH_UNSUPPORT_CONNECTION -1

typedef struct _pushentry
{
    struct _pushentry *next;
    uint8_t conn_state;    /* PRI_HIT etc, */
    uint8_t install_state; /* PRI_DELETE etc */
    uint8_t ams_state;     /* PRI_NOTIFIED etc */
    uint8_t type;          /* PRI_DEAD | PRI_ALARM | PRI_SOCKET | PRI_DATAGRAM |
                              PRI_SMS | PRI_CBS | PRI_MMS | PRI_SIP | PRI_BLUETOOTH */
    uint16_t midletinfo;   /* generic */
    int32_t isolateId;
    uint32_t port;
    int32_t connectionId;
    ASYNC_callbackNotifier callbackNotifier;
    ASYNC_callback *callback;
    bool_t ignoredConnection;
    bool_t alive;
    ASYNC_notifier async_id;
    unsigned checked_out;
    void *connection;

    char value[1];  /* Midlet descriptor string - more allocated as needed */
} PushEntry;

/* Used for port check. */
typedef struct _params
{
    union
    {
        uint32_t port;
        void *param1;
    } u;
    int32_t param2;
} ParamForCheckPort_t;

#ifdef SUPPORTS_PUSH_ALARM
/**
 *@name the push entry
 */
/*@{*/
typedef struct _alarmentry
{
  int64_t wakeup;
} AlarmEntry;
#endif /* SUPPORTS_PUSH_ALARM */
/*@}*/

struct push_connection
{
    unsigned int (*create)(PushEntry *pe);
    unsigned int (*add)(PushEntry *pe, PushEntry **head);
    int (*init)(PushEntry *pe);
    int (*open)(PushEntry *pe);
    int (*watch)(PushEntry *pe);
    void (*discard)(PushEntry *pe);
    void (*close)(PushEntry *pe);
    unsigned int (*checkFilter)(PushEntry *pe);
    unsigned int (*isPortMatch)(PushEntry *pe, ParamForCheckPort_t *params);
    void (*startConnection)(PushEntry *pe);
    void (*storeInfo)(PushEntry *pe, int handle);
    void (*reviveInfo)(PushEntry *pe, const char *info);
    unsigned int (*matchConn)(const char *url1, const char *url2);

    /**
     * Checks if the push entry has pending connections.
     *
     * @param pe the push entry
     * @return TRUE if the push entry has pending connections, otherwise FALSE
     */
    unsigned int (*hasConn)(PushEntry *pe);

    /**
     * Resets the push entry if the VM resets or the midlet exits.
     *
     * @param pe the push entry
     */
    void (*reset)(PushEntry *pe);
};

#ifdef SUPPORTS_PUSH_ALARM
unsigned int push_alarm_create(PushEntry *pe);
unsigned int push_alarm_add(PushEntry *pe, PushEntry **head);
int push_alarm_init(PushEntry *pe);
int push_alarm_open(PushEntry *pe);
int push_alarm_watch(PushEntry *pe);
void push_alarm_discard(PushEntry *pe);
void push_alarm_close(PushEntry *pe);
unsigned int push_alarm_check_filter(PushEntry *pe);
unsigned int push_alarm_check_port(PushEntry *pe, ParamForCheckPort_t *params);
void push_start_alarm_connection(PushEntry *pe);
void push_alarm_store_info(PushEntry *pe, int handle);
void push_alarm_revive_info(PushEntry *pe, const char *info);
#endif /* SUPPORTS_PUSH_ALARM */

#ifdef JBED_MIDP_3
unsigned int push_event_create(PushEntry *pe);
unsigned int push_event_add(PushEntry *pe, PushEntry **head);
int push_event_init(PushEntry *pe);
int push_event_open(PushEntry *pe);
int push_event_watch(PushEntry *pe);
void push_event_discard(PushEntry *pe);
void push_event_close(PushEntry *pe);
unsigned int push_event_check_filter(PushEntry *pe);
unsigned int push_event_check_port(PushEntry *pe, ParamForCheckPort_t *params);
void push_start_event_connection(PushEntry *pe);
void push_event_store_info(PushEntry *pe, int handle);
void push_event_revive_info(PushEntry *pe, const char *info);
#endif /* JBED_MIDP_3 */

void push_entry_add(PushEntry *pe);
void push_entry_remove(PushEntry *pe);
PushEntry *push_get_list(void);
void push_save(void);
unsigned int push_setup(PushEntry *pe);
void start_connection_op(PushEntry *p);
PushEntry *push_checkout_simple_port(uint32_t protocol, uint32_t port);
PushEntry *push_checkout_complex_port(uint32_t protocol, void *param1, int32_t param2);
STRING_INSTANCE getStorageName(void);
PushEntry *push_getPushEntry(uint32_t protocol, uint32_t port);
unsigned int strings_match(STRING_INSTANCE js, const char *cs);
unsigned int push_check_port_isused(PushEntry * pe);
unsigned int push_matchConn(const char *url1, const char *url2);

/**
 * The default implementation of callback function <code>hasConn</code>.
 *
 * @param pe the push entry
 * @return FALSE
 */
unsigned int push_hasConn(PushEntry *pe);

/**
 * The default implementation of callback function <code>reset</code>.
 * Do nothing.
 *
 * @param pe the push entry
 */
void push_resetEntry(PushEntry *pe);

/**
 * Check whether it is a push port.
 *
 * This function must only be called inside a Java native method.
 *
 * @param protocol type code for the protocol
 * @param port
 * @return TRUE if the port is registered.
 */
unsigned int push_checkport(uint32_t protocol, uint32_t port);

const char *push_field(const char *value, uint32_t field);

STRING_INSTANCE Jbed_push_get_field_helper(PushEntry *pe, jint field);

#define PUSH_STRING(pe) ((pe)->value + 1)

/* Bits passed to push_cleanup_entry */

#define PRI_CLEANUP_DISCARD 1 /* discard any pending connections */
#define PRI_CLEANUP_CLOSE   2 /* close listening sockets */
#define PRI_CLEANUP_KILL    4 /* mark entry as dead */
#define PRI_CLEANUP_WATCH   8 /* open listen socket and set to WATCHING */

/* Read a hex number at p into val, moving p */
#define SCAN_HEX_NUMBER_16(p,val) \
        for (; *p && *p != ',' ; ++p)                               \
        {                                                           \
            uint16_t digit = (uint16_t)((*p >= 'a')                 \
                       ? (*p - 'a') + 10 : (*p - '0'));             \
            (val) = (uint16_t)(((val) << 4) + digit);               \
        }

/* Read a hex number at p into val, moving p */
#define SCAN_HEX_NUMBER_64(p,val) \
        for (; *p && *p != ',' && *p != '\n'; ++p)                  \
        {                                                           \
            uint32_t digit = (*p >= 'a') ? (*p - 'a') + 10 : (*p - '0'); \
            int64_t digit64;                                        \
            int64_init(digit64, 0, digit);                          \
            int64_lshl(val, val, 4);                                \
            int64_add(val, val, digit64);                           \
        }

#if defined(__cplusplus)
}
#endif

#endif /* __PUSHREGISTRY_H__ */
