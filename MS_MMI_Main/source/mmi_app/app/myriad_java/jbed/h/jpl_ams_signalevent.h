/*
 * Copyright 2005-2009 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: AMS Event Signalling (VM-indendent)
 *
 * This interface provides signals from the AMS to the native world about
 * various events.
 */

#ifndef _JPL_AMS_SIGNALEVENT_H_
#define _JPL_AMS_SIGNALEVENT_H_

#include <jbed_global.h>
#include <jbed_jbni.h>
#include <javastatic.h>

/**
 * @name Constants for AMS events
 *
 * Constants for the \a event parameter of JPL_ams_signalEvent().
 */

#define JPL_ams_signalEvent \
    JbniS_com_jbed_ams_AmsToolkitBase_signalEvent

/*@{*/

/** Event for "-select" command line parameter. */
#define JPL_AMS_EVENT_SELECT   STATIC_com_jbed_ams_AmsToolkitBase_EVENT_SELECT

/** Event for install. */
#define JPL_AMS_EVENT_INSTALL  STATIC_com_jbed_ams_AmsToolkitBase_EVENT_INSTALL

/** Event for preinstall. */
#define JPL_AMS_EVENT_PREINSTALL STATIC_com_jbed_ams_AmsToolkitBase_EVENT_PREINSTALL

/** Event for "-ginstall" or "-linstall" command line parameter. */
#define JPL_AMS_EVENT_EXIT_AFTER_INSTALL \
                              STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_ASK_EXIT_AFTER_INSTALL

/** Event for MIDlet auto launch. */
#define JPL_AMS_AUTO_LAUNCH_MIDLET   STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AUTO_LAUNCH_MIDLET

/** Event for JAD notification posting. */
#define JPL_AMS_EVENT_NOTIFICATION   STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_NOTIFICATION

/** Event sent after installation is aborted and notification sent. */
#define JPL_AMS_EVENT_INSTALLABORTED  STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_INSTALLABORTED

/** Event sent after all downloads completed and notifications sent. */
#define JPL_AMS_EVENT_INSTALLCOMMITTED \
                                    STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_INSTALLCOMMITTED

/** Event sent when foreground MIDlet changes. */
#define JPL_AMS_EVENT_MIDLETFOREGROUND  \
                                    STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_MIDLETFOREGROUND

/** Event sent for platformRequest. */
#define JPL_AMS_EVENT_AMS_PLATFORM_REQUEST \
                                    STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_PLATFORM_REQUEST

/** Event sent for query whether native will handle install notifications */
#define JPL_AMS_EVENT_AMS_NATIVE_NOTIFICATION \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_NATIVE_NOTIFICATION

/** Event sent for asking native to notify the user to open the flip. */
#define JPL_AMS_EVENT_SPRINT_CLAMSHELL_NOTIFICATION \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_SPRINT_CLAMSHELL_NOTIFICATION

/** Event sent when failing to request AMS to run a MIDlet. */
#define JPL_AMS_EVENT_RUN_MIDLET_FAILED \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_RUN_MIDLET_FAILED

/** Event sent when a MIDlet is being launched. */
#define JPL_AMS_EVENT_AMS_LAUNCH_NOTIFICATION \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_LAUNCH_NOTIFICATION

/** Event sent when the VM wants to be the foreground/background. */
#define JPL_AMS_EVENT_VM_WANT_FOREGROUND \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_VM_WANT_FOREGROUND

/** Event send when the VM is out of memory in AMS isolate. */
#define JPL_AMS_EVENT_OUT_OF_MEMORY_AMS \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_OUT_OF_MEMORY_AMS

/** Event send when the VM is out of memory in a MIDlet isolate */
#define JPL_AMS_EVENT_OUT_OF_MEMORY_MIDLET \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_OUT_OF_MEMORY_MIDLET

/** Event send when a Bad MIDlet is detected */
#define JPL_AMS_EVENT_BAD_MIDLET_DETECTED \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_BAD_MIDLET_DETECTED

/** Event send when the TaskManager displays a new screen */
#define JPL_AMS_EVENT_TASK_MANAGER_STATE \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_TASK_MANAGER_STATE

/** Event send for process return state (for Sprint support) */
#define JPL_AMS_EVENT_PROCESS_RETURN_STATE \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_PROCESS_RETURN_STATE

/** Event send when midlet is removed  (for xperia support) */
#define JPL_AMS_EVENT_AMS_REMOVE_NOTIFICATION \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_REMOVE_NOTIFICATION

/** Event send when list running-midlet  (for xperia support) */
#define JPL_AMS_EVENT_LIST_RUNNING_MIDLETS \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_LIST_RUNNING_MIDLETS

/** Event send when kill a midlet  (for xperia support) */
#define JPL_AMS_EVENT_KILL_MIDLET \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_KILL_MIDLET

/** Event send when bring a midlet to foreground  (for xperia support) */
#define JPL_AMS_EVENT_ACTIVE_MIDLET \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_ACTIVE_MIDLET

/** Event send when the amount of ActivityMenu event is changed  (for SEMC Activity Menu support) */
#define JPL_AMS_EVENT_SEMC_AMENU_NOTIFICATION \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_SEMC_AMENU_NOTIFICATION

/** Event send when ActivityMenu is cancelled  (for SEMC Activity Menu support) */
#define JPL_AMS_EVENT_SEMC_AMENU_CANCEL \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_SEMC_AMENU_CANCEL

/** Event sent to native to prompt uncaught exception OutOfMemoryError occurs inside MIDlet
    (The Java layer can not handle the prompt in this situation). */
#define JPL_AMS_EVENT_PROMPT_FOR_OUT_OF_MEMORY_ERROR \
        STATIC_com_jbed_ams_AmsToolkitBase_EVENT_PROMPT_FOR_OUT_OF_MEMORY_ERROR
/*@}*/

/**
 * @name Constants for AMS event results
 *
 * Constants for the \a result parameter of JPL_ams_signalEvent().
 */

/*@{*/

/** Unrecognised. */
#define JPL_AMS_UNRECOGNISED   STATIC_com_jbed_ams_AmsToolkitBase_UNRECOGNISED

/** Success. */
#define JPL_AMS_SUCCESS        STATIC_com_jbed_ams_AmsToolkitBase_SUCCESS


/** Failure. */
#define JPL_AMS_FAILURE        STATIC_com_jbed_ams_AmsToolkitBase_FAILURE

/** No suites are installed. */
#define JPL_AMS_NO_SUITES_INSTALLED STATIC_com_jbed_ams_AmsToolkitBase_NO_SUITES_INSTALLED

/** The user cancelled the operation. */
#define JPL_AMS_USER_CANCELED       STATIC_com_jbed_ams_AmsToolkitBase_USER_CANCELED

/** The JAD server could not be found. */
#define JPL_AMS_JAD_SERVER_NOT_FOUND \
                                   STATIC_com_jbed_ams_AmsToolkitBase_JAD_SERVER_NOT_FOUND

/** The JAD file could not be found. */
#define JPL_AMS_JAD_NOT_FOUND          STATIC_com_jbed_ams_AmsToolkitBase_JAD_NOT_FOUND

/** The MIDlet suite has expired. */
#define JPL_AMS_MIDLET_SUITE_EXPIRED \
                                   STATIC_com_jbed_ams_AmsToolkitBase_MIDLET_SUITE_EXPIRED

/** The JAD file is invalid. */
#define JPL_AMS_INVALID_JAD        STATIC_com_jbed_ams_AmsToolkitBase_INVALID_JAD

/** The native code is handling a platform request now. */
#define JPL_AMS_PLATREQ_NOW        STATIC_com_jbed_ams_AmsToolkitBase_PLATREQ_NOW

/** The native code requires a VM reset to handle a platform request. */
#define JPL_AMS_PLATREQ_LATER      STATIC_com_jbed_ams_AmsToolkitBase_PLATREQ_LATER

/** The native code will handle the install notification */
#define JPL_AMS_NATIVE_NOTIFICATION STATIC_com_jbed_ams_AmsToolkitBase_NATIVE_NOTIFICATION

/** The MIDlet failed to initialize */
#define JPL_AMS_MIDLET_INIT_ERROR   STATIC_com_jbed_ams_AmsToolkitBase_MIDLET_INIT_ERROR

/** The MIDlet is launched from Installer */
#define JPL_AMS_LAUNCH_FROM_INSTALLER   STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FROM_INSTALLER

/** The MIDlet is launched for CHAPI */
#define JPL_AMS_LAUNCH_FOR_CHAPI   STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FOR_CHAPI

/** The MIDlet is launched because of an AMS upcall */
#define JPL_AMS_LAUNCH_FOR_AMS_UPCALL   STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FOR_AMS_UPCALL

/** The MIDlet is launched from native ams */
#define JPL_AMS_LAUNCH_FROM_NATIVE_AMS   STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FROM_NATIVE_AMS

/** The MIDlet is launched for a push registry event */
#define JPL_AMS_LAUNCH_FOR_PUSH_REGISTRY   \
    STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FOR_PUSH_REGISTRY

/** The MIDlet is launched from the AMS MIDlet List */
#define JPL_AMS_LAUNCH_FROM_MIDLET_LIST   STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FROM_MIDLET_LIST

/** The MIDlet is launched from the tck runner */
#define JPL_AMS_LAUNCH_FROM_TCK_RUNNER   STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FROM_TCK_RUNNER

/** The MIDlet is launched for SPRINT content handler */
#define JPL_AMS_LAUNCH_FOR_SPRINT_CONTENT   \
    STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FOR_SPRINT_CONTENT

/** The MIDlet is launched for server MIDlet */
#define JPL_AMS_LAUNCH_FOR_SERVER_MIDLET   \
    STATIC_com_jbed_ams_AmsToolkitBase_LAUNCH_FOR_SERVER_MIDLET

/** Indicate a preinstall start */
#define JPL_AMS_PREINSTALL_START   \
    STATIC_com_jbed_ams_AmsToolkitBase_PREINSTALL_START

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates the AMS comes to foreground.
 */
#define JPL_AMS_AMS_IN_FOREGROUND   STATIC_com_jbed_ams_AmsToolkitBase_AMS_IN_FOREGROUND

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates the RunningMIDletManager comes to foreground.
 * This is used only with MVM.
 */
#define JPL_AMS_RUNNING_MIDLET_MANAGER_IN_FOREGROUND   \
    STATIC_com_jbed_ams_AmsToolkitBase_RUNNING_MIDLET_MANAGER_IN_FOREGROUND

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates the TaskManager comes to foreground.
 * The TaskManager shows screens such as confirmation before termination.
 */
#define JPL_AMS_TASK_MANAGER_PROMPT   STATIC_com_jbed_ams_AmsToolkitBase_TASK_MANAGER_PROMPT

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates the install GUI is handled by Java
 * This will be sent according to RES_JAVA_HANDLE from native AMS.
 */
#define JPL_AMS_INSTALL_JAVA_HANDLE   STATIC_com_jbed_ams_AmsToolkitBase_INSTALL_JAVA_HANDLE

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates the MIDlet comes to foreground.
 */
#define JPL_AMS_MIDLET_IN_FOREGROUND   STATIC_com_jbed_ams_AmsToolkitBase_MIDLET_IN_FOREGROUND

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates Display#setCurrent() is called.
 */
#define JPL_AMS_MIDLET_SET_DISPLAY   STATIC_com_jbed_ams_AmsToolkitBase_MIDLET_SET_DISPLAY

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates MIDlet#startApp() will be called.
 */
#define JPL_AMS_MIDLET_STARTING   STATIC_com_jbed_ams_AmsToolkitBase_MIDLET_STARTING

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates MIDlet#resumeRequest() is called.
 */
#define JPL_AMS_MIDLET_RESUME_REQUEST   STATIC_com_jbed_ams_AmsToolkitBase_MIDLET_RESUME_REQUEST

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This is sent before the static initilizer of the MIDlet is run, the first
 * chance the MIDlet has to run any code.  If a TASK_MANAGER_PROMPT is sent
 * after this and before MIDLET_IN_FOREGROUND it could mean that the MIDlet
 * is (against the MIDlet specification) trying to access protected APIs
 * either in it's static init or constructor.  Such a prompt will block the
 * MIDlet from progressing, so the VM has to be brought to the foreground to
 * let the MIDlet get into a running state.
 */
#define JPL_AMS_MIDLET_PREPARE  STATIC_com_jbed_ams_AmsToolkitBase_MIDLET_PREPARE

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * To debug MIDlet via KDWP, the debugger needs Java to be foreground.
 */
#define JPL_AMS_KDWP_REQUEST  STATIC_com_jbed_ams_AmsToolkitBase_KDWP_REQUEST

/**
 * One of the reasons of JPL_AMS_EVENT_VM_WANT_FOREGROUND.
 * This indicates the end key is pressed and the VM wants to be background.
 */
#define JPL_AMS_END_KEY_PRESSED  STATIC_com_jbed_ams_AmsToolkitBase_END_KEY_PRESSED

/** Task Manager state: idle - no prompt is displayed */
#define JPL_AMS_STATE_IDLE STATIC_com_jbed_ams_AmsToolkitBase_STATE_IDLE

/** Task Manager state: list - displays current running MIDlets  */
#define JPL_AMS_STATE_LIST STATIC_com_jbed_ams_AmsToolkitBase_STATE_LIST

/** Task Manager state: display splash screen */
#define JPL_AMS_STATE_SPLASH STATIC_com_jbed_ams_AmsToolkitBase_STATE_SPLASH

/** Task Manager state: show an alert */
#define JPL_AMS_STATE_ALERT_MESSAGE STATIC_com_jbed_ams_AmsToolkitBase_STATE_ALERT_MESSAGE

/** Task Manager state: uncaught exception prompt */
#define JPL_AMS_STATE_KILL_MIDLET STATIC_com_jbed_ams_AmsToolkitBase_STATE_KILL_MIDLET

/** Task Manager state: life-cycle switching is taking a long time, and no display has been set */
#define JPL_AMS_STATE_LIFECYCLE STATIC_com_jbed_ams_AmsToolkitBase_STATE_LIFECYCLE

/** Task Manager state: permission request */
#define JPL_AMS_STATE_PERMISSION STATIC_com_jbed_ams_AmsToolkitBase_STATE_PERMISSION

/** Task Manager state: payment prompt */
#define JPL_AMS_STATE_PAYREQ STATIC_com_jbed_ams_AmsToolkitBase_STATE_PAYREQ

/** Task Manager state: satsa prompt */
#define JPL_AMS_STATE_SATSA STATIC_com_jbed_ams_AmsToolkitBase_STATE_SATSA

/** Task Manager state: Sprint scheme/content launch prompt */
#define JPL_AMS_STATE_LAUNCH_REQUEST STATIC_com_jbed_ams_AmsToolkitBase_STATE_LAUNCH_REQUEST

/** Task Manager state: chapi launch prompt */
#define JPL_AMS_STATE_CHAPI_INVOKE STATIC_com_jbed_ams_AmsToolkitBase_STATE_CHAPI_INVOKE

/** Task Manager state: push launch prompt */
#define JPL_AMS_STATE_PUSH STATIC_com_jbed_ams_AmsToolkitBase_STATE_PUSH

/** Task Manager state: red key has been pressed, we're exiting the MIDlet */
#define JPL_AMS_STATE_END_EXIT STATIC_com_jbed_ams_AmsToolkitBase_STATE_END_EXIT

/** Task Manager state: install request prompt */
#define JPL_AMS_STATE_INSTALL STATIC_com_jbed_ams_AmsToolkitBase_STATE_INSTALL

/** Task Manager state: red key has been pressed, terminate MIDlet prompt */
#define JPL_AMS_STATE_ASK_TERMINATE_MIDLET \
    STATIC_com_jbed_ams_AmsToolkitBase_STATE_ASK_TERMINATE_MIDLET

/** Task Manager state: Sprint notification for outgoing call */
#define JPL_AMS_STATE_OUTGOING_CALL STATIC_com_jbed_ams_AmsToolkitBase_STATE_OUTGOING_CALL

/** Task Manager state: incoming call notification */
#define JPL_AMS_STATE_CALL STATIC_com_jbed_ams_AmsToolkitBase_STATE_CALL

/** Task Manager state: incoming call has been accepted */
#define JPL_AMS_STATE_ACCEPT STATIC_com_jbed_ams_AmsToolkitBase_STATE_ACCEPT

/*@}*/

/**
 * @name Constants for AMS events
 *
 * Constants for the \a event parameter of JPL_ams_signalAsyncEvent().
 */

#define JPL_ams_signalAsyncEvent \
    JbniS_com_jbed_ams_AmsToolkitBase_nSignalAsyncEvent

/*@{*/

/** Event for posting install/delete notification. */
#define JPL_AMS_EVENT_POST_NOTIFICATION \
    STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_POST_NOTIFICATION

/** Event for launching browser. */
#define JPL_AMS_EVENT_LAUNCH_BROWSER \
    STATIC_com_jbed_ams_AmsToolkitBase_EVENT_AMS_LAUNCH_BROWSER
/*@}*/

#endif /* _JPL_AMS_SIGNALEVENT_H_ */
