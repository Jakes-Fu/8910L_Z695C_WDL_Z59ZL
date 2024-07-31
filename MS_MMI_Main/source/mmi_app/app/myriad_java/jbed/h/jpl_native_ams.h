/*
 * Copyright 2004-2012 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: TPAL - Support for native AMS.
 * @ingroup jpl_native_ams
 */

#ifndef __JBED_JPL_NATIVE_AMS_H__
#define __JBED_JPL_NATIVE_AMS_H__

#include <jbed_global.h>
#include <jbed_jbni.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name Convenience constants for the TPAL events.
 * @{
 */

#define EVENT_NONE                   STATIC_com_jbed_ams_NativeAms_EVENT_NONE
#define EVENT_LIST                   STATIC_com_jbed_ams_NativeAms_EVENT_LIST
#define EVENT_RUN                    STATIC_com_jbed_ams_NativeAms_EVENT_RUN
#define EVENT_UPDATE                 STATIC_com_jbed_ams_NativeAms_EVENT_UPDATE
#define EVENT_INSTALL_PROGRESS       STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_PROGRESS
#define EVENT_INSTALL                STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL
#define EVENT_INSTALL_CANCEL         STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_CANCEL
#define EVENT_REMOVE                 STATIC_com_jbed_ams_NativeAms_EVENT_REMOVE
#define EVENT_PAUSE                  STATIC_com_jbed_ams_NativeAms_EVENT_PAUSE
#define EVENT_RESUME                 STATIC_com_jbed_ams_NativeAms_EVENT_RESUME
#define EVENT_DESTROY                STATIC_com_jbed_ams_NativeAms_EVENT_DESTROY
#define EVENT_DESTROYED              STATIC_com_jbed_ams_NativeAms_EVENT_DESTROYED
#define EVENT_INSTALL_STATUS         STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_STATUS
#define EVENT_INSTALL_QUERY          STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_QUERY
#define EVENT_INSTALL_QUERY_YES_NO   STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_QUERY_YES_NO
#define EVENT_INSTALL_UNSTOPPABLE    STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_UNSTOPPABLE
#define EVENT_INSTALL_PREVIEW_JAD    STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_PREVIEW_JAD
#define EVENT_INSTALL_AUTHENTICATION STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_AUTHENTICATION
#define EVENT_PAUSED                 STATIC_com_jbed_ams_NativeAms_EVENT_PAUSED
#define EVENT_RUNNING                STATIC_com_jbed_ams_NativeAms_EVENT_RUNNING
#define EVENT_INSTALL_STEP           STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_STEP
#define EVENT_INFO                   STATIC_com_jbed_ams_NativeAms_EVENT_INFO
#define EVENT_RENAME                 STATIC_com_jbed_ams_NativeAms_EVENT_RENAME
#define EVENT_GET_ICONS              STATIC_com_jbed_ams_NativeAms_EVENT_GET_ICONS
#define EVENT_PERMISSIONS            STATIC_com_jbed_ams_NativeAms_EVENT_PERMISSIONS
#define EVENT_PERMISSIONS_ANSWERS    STATIC_com_jbed_ams_NativeAms_EVENT_PERMISSIONS_ANSWERS
#define EVENT_QUERY                  STATIC_com_jbed_ams_NativeAms_EVENT_QUERY
#define EVENT_REMOVE_ALL             STATIC_com_jbed_ams_NativeAms_EVENT_REMOVE_ALL
#define EVENT_SET                    STATIC_com_jbed_ams_NativeAms_EVENT_SET
#define EVENT_INSTALL_STEP_COMPLETE  STATIC_com_jbed_ams_NativeAms_EVENT_INSTALL_STEP_COMPLETE
#define EVENT_SELECT_STORAGE         STATIC_com_jbed_ams_NativeAms_EVENT_SELECT_STORAGE
#define EVENT_REMOVABLE_MEDIA_CHANGED   STATIC_com_jbed_ams_NativeAms_EVENT_REMOVABLE_MEDIA_CHANGED
#define EVENT_MOVE                   STATIC_com_jbed_ams_NativeAms_EVENT_MOVE
#define EVENT_LIST_STORAGE           STATIC_com_jbed_ams_NativeAms_EVENT_LIST_STORAGE
#define EVENT_REQUEST_INSTALL        STATIC_com_jbed_ams_NativeAms_EVENT_REQUEST_INSTALL
#define EVENT_LIST_RUNNING_MIDLETS   STATIC_com_jbed_ams_NativeAms_EVENT_LIST_RUNNING_MIDLETS
#define EVENT_JAR_DOWNLOAD_PROGRESS  STATIC_com_jbed_ams_NativeAms_EVENT_JAR_DOWNLOAD_PROGRESS
#define EVENT_JAR_DOWNLOAD           STATIC_com_jbed_ams_NativeAms_EVENT_JAR_DOWNLOAD
#define EVENT_JAR_DOWNLOAD_COMPLETE  STATIC_com_jbed_ams_NativeAms_EVENT_JAR_DOWNLOAD_COMPLETE
#define EVENT_DRM_ACTIVATE           STATIC_com_jbed_ams_NativeAms_EVENT_DRM_ACTIVATE
#define EVENT_DRM_REINSTALL            STATIC_com_jbed_ams_NativeAms_EVENT_DRM_REINSTALL
#define EVENT_RUN_DEBUG              STATIC_com_jbed_ams_NativeAms_EVENT_RUN_DEBUG
#define EVENT_PERMISSIONS_RESET_ANSWERS \
    STATIC_com_jbed_ams_NativeAms_EVENT_PERMISSIONS_RESET_ANSWERS
#define EVENT_PLATFORM_RESET         STATIC_com_jbed_ams_NativeAms_EVENT_PLATFORM_RESET
#define EVENT_REQUEST_PUSH           STATIC_com_jbed_ams_NativeAms_EVENT_REQUEST_PUSH
#define EVENT_REQUEST_SATSA          STATIC_com_jbed_ams_NativeAms_EVENT_REQUEST_SATSA
#define EVENT_REQUEST_CHAPI          STATIC_com_jbed_ams_NativeAms_EVENT_REQUEST_CHAPI
#define EVENT_PREINSTALL_START       STATIC_com_jbed_ams_NativeAms_EVENT_PREINSTALL_START
#define EVENT_PREINSTALL_FINISH      STATIC_com_jbed_ams_NativeAms_EVENT_PREINSTALL_FINISH
#define EVENT_RUNNING_MIDLET_MANAGER STATIC_com_jbed_ams_NativeAms_EVENT_RUNNING_MIDLET_MANAGER
#define EVENT_NOTIFY_LAUNCH          STATIC_com_jbed_ams_NativeAms_EVENT_NOTIFY_LAUNCH
#define EVENT_SANITY_REPORT          STATIC_com_jbed_ams_NativeAms_EVENT_SANITY_REPORT
#define EVENT_REQUEST_PERMISSION     STATIC_com_jbed_ams_NativeAms_EVENT_REQUEST_PERMISSION
#define EVENT_LIST_CERTIFICATES      STATIC_com_jbed_ams_NativeAms_EVENT_LIST_CERTIFICATES
#define EVENT_CHANGE_CERTIFICATE_STATE  STATIC_com_jbed_ams_NativeAms_EVENT_CHANGE_CERTIFICATE_STATE
#define EVENT_MIDLET_RUNTIME_ERROR   STATIC_com_jbed_ams_NativeAms_EVENT_RUNTIME_ERROR
#define EVENT_LIST_FOLDER_CONTENTS   STATIC_com_jbed_ams_NativeAms_EVENT_LIST_FOLDER_CONTENTS
#define EVENT_CREATE_FOLDER          STATIC_com_jbed_ams_NativeAms_EVENT_CREATE_FOLDER
#define EVENT_MOVE_FOLDER            STATIC_com_jbed_ams_NativeAms_EVENT_MOVE_FOLDER
#define EVENT_RENAME_FOLDER          STATIC_com_jbed_ams_NativeAms_EVENT_RENAME_FOLDER
#define EVENT_REMOVE_FOLDER          STATIC_com_jbed_ams_NativeAms_EVENT_REMOVE_FOLDER
#define EVENT_REMOVE_FOLDER_CONTENTS    STATIC_com_jbed_ams_NativeAms_EVENT_REMOVE_FOLDER_CONTENTS
#define EVENT_SELECT_INSTALL_FOLDER  STATIC_com_jbed_ams_NativeAms_EVENT_SELECT_INSTALL_FOLDER
#define EVENT_DRM_CHECK_RIGHTS       STATIC_com_jbed_ams_NativeAms_EVENT_DRM_CHECK_RIGHTS
#define EVENT_DRM_CONSTRAINT         STATIC_com_jbed_ams_NativeAms_EVENT_DRM_CONSTRAINT
#define EVENT_NOPAUSE_IN_BG          STATIC_com_jbed_ams_NativeAms_EVENT_NOPAUSE_IN_BG
#define EVENT_SHOW_JAR_DOWNLOAD_SUCCESS        STATIC_com_jbed_ams_NativeAms_EVENT_SHOW_JAR_DOWNLOAD_SUCCESS
#define EVENT_COMMAND_INSTALL                  STATIC_com_jbed_ams_NativeAms_EVENT_COMMAND_INSTALL
#define EVENT_COMMAND_INSTALL_STATUS           STATIC_com_jbed_ams_NativeAms_EVENT_COMMAND_INSTALL_STATUS
#define EVENT_COMMAND_INSTALL_PROGRESS         STATIC_com_jbed_ams_NativeAms_EVENT_COMMAND_INSTALL_PROGRESS

#define RES_NONE                     STATIC_com_jbed_ams_NativeAms_RES_NONE
#define RES_OK                       STATIC_com_jbed_ams_NativeAms_RES_OK
#define RES_JAVA_HANDLE              STATIC_com_jbed_ams_NativeAms_RES_JAVA_HANDLE
#define RES_NO_INSTALL_RUNNING       STATIC_com_jbed_ams_NativeAms_RES_NO_INSTALL_RUNNING
#define RES_TOO_LATE_FOR_CANCEL      STATIC_com_jbed_ams_NativeAms_RES_TOO_LATE_FOR_CANCEL
#define RES_CONTINUE                 STATIC_com_jbed_ams_NativeAms_RES_CONTINUE
#define RES_CANCEL                   STATIC_com_jbed_ams_NativeAms_RES_CANCEL
#define RES_JAD_URL                  STATIC_com_jbed_ams_NativeAms_RES_JAD_URL
#define RES_NOT_FOUND                STATIC_com_jbed_ams_NativeAms_RES_NOT_FOUND
#define RES_DOWNLOADING              STATIC_com_jbed_ams_NativeAms_RES_DOWNLOADING
#define RES_INSTALLING               STATIC_com_jbed_ams_NativeAms_RES_INSTALLING
#define RES_RUNNING                  STATIC_com_jbed_ams_NativeAms_RES_RUNNING
#define RES_NOT_RUNNING              STATIC_com_jbed_ams_NativeAms_RES_NOT_RUNNING
#define RES_NO_ICON                  STATIC_com_jbed_ams_NativeAms_RES_NO_ICON
#define RES_NOT_SET                  STATIC_com_jbed_ams_NativeAms_RES_NOT_SET
#define RES_ERROR                    STATIC_com_jbed_ams_NativeAms_RES_ERROR
#define RES_NOT_ENOUGH_SPACE         STATIC_com_jbed_ams_NativeAms_RES_NOT_ENOUGH_SPACE
#define RES_COPY_ERROR               STATIC_com_jbed_ams_NativeAms_RES_COPY_ERROR
#define RES_CANT_CREATE_DIRECTORY    STATIC_com_jbed_ams_NativeAms_RES_CANT_CREATE_DIRECTORY
#define RES_NO_STORAGES              STATIC_com_jbed_ams_NativeAms_RES_NO_STORAGES
#define RES_NO_RUNNING_MIDLETS       STATIC_com_jbed_ams_NativeAms_RES_NO_RUNNING_MIDLETS
#define RES_RUNNING_MIDLETS          STATIC_com_jbed_ams_NativeAms_RES_RUNNING_MIDLETS
#define RES_CANCELLING               STATIC_com_jbed_ams_NativeAms_RES_CANCELLING
#define RES_SIZE_MISMATCH            STATIC_com_jbed_ams_NativeAms_RES_SIZE_MISMATCH
#define RES_DRM_INSTALL              STATIC_com_jbed_ams_NativeAms_RES_DRM_INSTALL
#define RES_YES                      STATIC_com_jbed_ams_NativeAms_RES_YES
#define RES_NO                       STATIC_com_jbed_ams_NativeAms_RES_NO
#define RES_DRM_ERROR                STATIC_com_jbed_ams_NativeAms_RES_DRM_ERROR
#define RES_NOT_COMPLETELY_REMOVED   STATIC_com_jbed_ams_NativeAms_RES_NOT_COMPLETELY_REMOVED

#define SET_REMOVABLE                STATIC_com_jbed_ams_NativeAms_SET_REMOVABLE

#define MIDLET_LIFECYCLE_EXCEPTION   STATIC_com_jbed_ams_NativeAms_MIDLET_LIFECYCLE_EXCEPTION

#define IQ_ASK_USER                  STATIC_com_jbed_ams_NativeAms_IQ_ASK_USER
#define IQ_INFORM_USER               STATIC_com_jbed_ams_NativeAms_IQ_INFORM_USER
#define IQ_OLD_VERSION               STATIC_com_jbed_ams_NativeAms_IQ_OLD_VERSION
#define IQ_NEW_VERSION               STATIC_com_jbed_ams_NativeAms_IQ_NEW_VERSION
#define IQ_ALREADY_INSTALLED         STATIC_com_jbed_ams_NativeAms_IQ_ALREADY_INSTALLED
#define IQ_JAD_MOVED                 STATIC_com_jbed_ams_NativeAms_IQ_JAD_MOVED
#define IQ_UNAUTHORIZED              STATIC_com_jbed_ams_NativeAms_IQ_UNAUTHORIZED
#define IQ_INSTALL_CONFIRM           STATIC_com_jbed_ams_NativeAms_IQ_INSTALL_CONFIRM
#define IQ_MIDLET_UNSIGNED           STATIC_com_jbed_ams_NativeAms_IQ_MIDLET_UNSIGNED
#define IQ_MIDLET_SIGNED             STATIC_com_jbed_ams_NativeAms_IQ_MIDLET_SIGNED

#define EPA_CHECK                    STATIC_com_jbed_ams_NativeAms_EPA_CHECK
#define EPA_SET                      STATIC_com_jbed_ams_NativeAms_EPA_SET
#define PERM_ALWAYS                  STATIC_com_jbed_ams_NativeAms_PERM_ALWAYS
#define PERM_NEVER                   STATIC_com_jbed_ams_NativeAms_PERM_NEVER
#define PERM_ASK                     STATIC_com_jbed_ams_NativeAms_PERM_ASK
#define PERM_SESSION                 STATIC_com_jbed_ams_NativeAms_PERM_SESSION
#define PERM_ONESHOT                 STATIC_com_jbed_ams_NativeAms_PERM_ONESHOT

#define RESET_PREINSTALLED           STATIC_com_jbed_ams_NativeAms_RESET_PREINSTALLED
#define RESET_PROTECTED              STATIC_com_jbed_ams_NativeAms_RESET_PROTECTED

#define PERM_ANSWER_NEVER            STATIC_com_jbed_ams_NativeAms_NEVER
#define PERM_ANSWER_NOT_SESSION      STATIC_com_jbed_ams_NativeAms_NOT_SESSION
#define PERM_ANSWER_NOT_USE          STATIC_com_jbed_ams_NativeAms_NOT_USE
#define PERM_ANSWER_USE              STATIC_com_jbed_ams_NativeAms_USE
#define PERM_ANSWER_SESSION          STATIC_com_jbed_ams_NativeAms_SESSION
#define PERM_ANSWER_ALWAYS           STATIC_com_jbed_ams_NativeAms_ALWAYS

#define SATSA_TYPE_PIN               STATIC_com_jbed_ams_NativeAms_SATSA_TYPE_PIN
#define SATSA_TYPE_CONFIRMATION      STATIC_com_jbed_ams_NativeAms_SATSA_TYPE_CONFIRMATION
#define SATSA_TYPE_LIST              STATIC_com_jbed_ams_NativeAms_SATSA_TYPE_LIST
#define PIN_CHANGE                   STATIC_com_jbed_ams_NativeAms_PIN_CHANGE
#define PIN_ENABLE                   STATIC_com_jbed_ams_NativeAms_PIN_ENABLE
#define PIN_DISABLE                  STATIC_com_jbed_ams_NativeAms_PIN_DISABLE
#define PIN_ENTER                    STATIC_com_jbed_ams_NativeAms_PIN_ENTER
#define PIN_UNBLOCK                  STATIC_com_jbed_ams_NativeAms_PIN_UNBLOCK
#define CONFIRMATION_USED            STATIC_com_jbed_ams_NativeAms_CONFIRMATION_USED
#define CONFIRMATION_ADDED           STATIC_com_jbed_ams_NativeAms_CONFIRMATION_ADDED
#define CONFIRMATION_REMOVED         STATIC_com_jbed_ams_NativeAms_CONFIRMATION_REMOVED
#define CONFIRMATION_SIGN            STATIC_com_jbed_ams_NativeAms_CONFIRMATION_SIGN
#define CONFIRMATION_GENERATE        STATIC_com_jbed_ams_NativeAms_CONFIRMATION_GENERATE

#define LIST_ALL                     STATIC_com_jbed_ams_NativeAms_LIST_ALL
#define LIST_FOLDER                  STATIC_com_jbed_ams_NativeAms_LIST_FOLDER
#define LIST_SUITE                   STATIC_com_jbed_ams_NativeAms_LIST_SUITE

#define TYPE_FOLDER                  STATIC_com_jbed_ams_NativeAms_TYPE_FOLDER
#define TYPE_SUITE                   STATIC_com_jbed_ams_NativeAms_TYPE_SUITE

#define TARGET_STORAGE               STATIC_com_jbed_ams_NativeAms_TARGET_STORAGE
#define TARGET_FOLDER                STATIC_com_jbed_ams_NativeAms_TARGET_FOLDER

/** @} */

/**
 * @name Convenience short form macros for the variables being accessed by native.
 * @{
 */

#define JPL_ams_event       STATIC_com_jbed_ams_NativeAms_event
#define JPL_ams_eventInt    STATIC_com_jbed_ams_NativeAms_eventInt
#define JPL_ams_eventObject STATIC_com_jbed_ams_NativeAms_eventObject

/** @} */

/**
 * @name Convenience short form macros for the native methods.
 * @{
 */

#define JPL_ams_getEvent       JbniS_com_jbed_ams_NativeAms_nativeGetEvent
#define JPL_ams_postEvent      JbniS_com_jbed_ams_NativeAms_nativePostEvent

/** @} */

#if defined(__cplusplus)
}
#endif

#endif  /* __JBED_JPL_NATIVE_AMS_H__ */
