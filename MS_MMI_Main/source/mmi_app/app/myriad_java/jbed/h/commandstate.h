/*[
 * Copyright 2001-2009 Myriad Group AG. All Rights Reserved.
 * $Id$
]*/

/*
 * @(#)commandState.h   1.8 01/08/30
 * Copyright (c) 1999,2001 Sun Microsystems, Inc. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of Sun
 * Microsystems, Inc. ("Confidential Information").  You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered into
 * with Sun.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 */
#ifndef _COMMAND_STATE_H
#define _COMMAND_STATE_H

#include <javastatic.h>  /* krom-generated static final int values */

#define MAIN_EXIT STATIC_com_jbed_ams_CommandProcessor_MAIN_EXIT

typedef enum {
    JAM_EXIT          = STATIC_com_jbed_ams_CommandProcessor_EXIT,
    JAM_INSTALL       = STATIC_com_jbed_ams_CommandProcessor_INSTALL,
    JAM_REMOVE        = STATIC_com_jbed_ams_CommandProcessor_REMOVE,
    JAM_LIST          = STATIC_com_jbed_ams_CommandProcessor_LIST,
    JAM_STORAGE_NAMES = STATIC_com_jbed_ams_CommandProcessor_STORAGE_NAMES,
    JAM_RENAME        = STATIC_com_jbed_ams_CommandProcessor_RENAME,
    JAM_INSTALL_RUN   = STATIC_com_jbed_ams_CommandProcessor_INSTALL_RUN,
    JAM_MANAGE        = STATIC_com_jbed_ams_CommandProcessor_MANAGE,
    JAM_GUI_INSTALL   = STATIC_com_jbed_ams_CommandProcessor_GUI_INSTALL,
    JAM_PREINSTALL    = STATIC_com_jbed_ams_CommandProcessor_PREINSTALL,
#ifdef ALLOW_LOCAL_INSTALLATION
    JAM_LOCAL_INSTALL = STATIC_com_jbed_ams_CommandProcessor_LOCAL_INSTALL,
#endif
    JAM_SELECT        = STATIC_com_jbed_ams_CommandProcessor_SELECT,
    JAM_RUN           = STATIC_com_jbed_ams_CommandProcessor_RUN,
    JAM_RUN_CLASS     = STATIC_com_jbed_ams_CommandProcessor_RUN_CLASS,
    JAM_GUI_INSTALL_RUN = STATIC_com_jbed_ams_CommandProcessor_GUI_INSTALL_RUN,
    JAM_CHAPI_AMS     = STATIC_com_jbed_ams_CommandProcessor_CHAPI_AMS
} jam_state;

#define INITIAL_COMMANDS_NEEDING_GRAPHICS \
   STATIC_com_jbed_ams_CommandProcessor_INITIAL_COMMANDS_NEEDING_GRAPHICS

#define INITIAL_COMMANDS_NEEDING_PUSH \
   STATIC_com_jbed_ams_CommandProcessor_INITIAL_COMMANDS_NEEDING_PUSH

#define COMMANDS_NEEDING_PUSH \
   STATIC_com_jbed_ams_CommandProcessor_COMMANDS_NEEDING_PUSH

#ifdef JUNIT_TESTS
#define JUNIT_STDOUT_COLLECTOR \
   STATIC_com_jbed_ams_CommandState_JUNIT_STDOUT_COLLECTOR

#define JUNIT_SMOKETEST_COLLECTOR \
   STATIC_com_jbed_ams_CommandState_JUNIT_SMOKETEST_COLLECTOR

#define JUNIT_UI_COLLECTOR \
   STATIC_com_jbed_ams_CommandState_JUNIT_UI_COLLECTOR
#endif /* JUNIT_TESTS */

typedef enum StateFlags_e {
    cmdStateLogoDisplayed           = 0x01
    ,cmdStateAccessNetwork          = 0x02
    ,cmdStateRunOnce                = 0x04
    ,cmdStateAutotest               = 0x08
    ,cmdStateForceOverwrite         = 0x10
    ,cmdStateTimeStartup            = 0x20
    ,cmdStatePushSession            = 0x40
    ,cmdStateManage                 = 0x80
#if defined(MIDP2) && !defined(STANDALONE_TARGET)
    ,cmdStateMultiMIDlets           = 0x100
#endif
    ,cmdStateMidletStartScreenDisplayed = 0x200
#ifdef LIGHTWEIGHT_INTERNALISATION
    ,cmdStateDoInternalisation      = 0x400
#endif /* LIGHTWEIGHT_INTERNALISATION */
    ,cmdStateShowPreinstalled       = 0x800
    ,cmdStateHidden                 = 0x1000
    ,cmdStateMIDletDebugging        = 0x2000
    ,cmdStateMIDletProfiling        = 0x4000
} StateFlags;

typedef struct _COMMAND_STATE {
    IU32 status;
    jam_state command;
    char* suiteURL;
    char* suiteStorageName;
    char* descriptorName;
    char* midletName;
    int   midletNum;
    char* midletClassName;
    char* nameToChange;
#ifdef ALLOW_LOCAL_INSTALLATION
    char* jarName;
#ifdef SAVE_ORIGINAL_URL
    char* originalSuiteURL;
#endif
#endif
#ifdef MIDP2
    char* domainOverride;
#endif
    char* tckUrl;
    IU32 flags;
#ifdef AMS_FOLDERS
    char* currentFolder;
#endif /* AMS_FOLDERS */
#ifdef AMS_REMOVABLE_MEDIA
    char* installDir;
#endif
    IS32 currentMIDlet;
#ifdef JUNIT_TESTS
    IS32 junitResultCollector;
    char* junitTestsToRun;
    char* junitExcludeList;
    char* smoketestHost;
    char* smoketestPort;
#endif
    char* preinstallFolder;

} COMMAND_STATE;

extern COMMAND_STATE MidpCommandState;

extern void discardCommandState(void);

#endif /* _COMMAND_STATE_H */

/* ***** EDITOR CONTROL STRINGS *****
 * Local Variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 * vi:set ts=4 sw=4:
 * **********************************/
