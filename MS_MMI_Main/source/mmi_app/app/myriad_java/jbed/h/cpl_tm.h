/**
 * Copyright 2011-2015 Myriad Group AG. All Rights Reserved.
 * $Id$
 */


#ifndef __CPL_TM_H__
#define __CPL_TM_H__

#include <guianim.h>
#include <mmifmm_export.h>

#ifdef JBED_MVM
#define MAX_RUNNING_MIDLETS (16)
#else
#define MAX_RUNNING_MIDLETS (1)
#endif
typedef struct _RunningApp
{
    int used;
    int sid;
    int mid;
    MMI_STRING_T text_buffer;
    GUIANIM_FILE_INFO_T pngPtr;
}RunningApp;

/**
 * Task manager module initalize.
 */
void Jbed_tm_initialize();

/**
 * Task manager module finalize.
 */
void Jbed_tm_finalize();

int Jbed_tm_getRunningAppNum(void);

void Jbed_wakeupVM(void);

/**
 * If user launch an app, set the launched APP information
 * to tm module.
 * @param sid
 *      \ suite id.
 * @param mid
 *      \ MIDlet number.
 * @return current launching app information.
 */
void Jbed_tm_setRunningAppInfo(int sid, int mid);

/**
 * Set current active app.
 * to tm module.
 * @param sid
 *      \ suite id.
 * @param mid
 *      \ MIDlet number.
 * @return current active launching app information.
 */
RunningApp* Jbed_tm_setActiveRunningApp(int sid, int mid);
/**
 * To simply check if a MIDlet is running.
 * @param sid
 *      \ suite id.
 * @param mid
 *      \ MIDlet number.
 * @return BOOLEAN
 *      \ TRUE if the MIDlet is running.
 */
BOOLEAN Jbed_tm_queryRunningAppByID(int sid, int mid);

/**
 * If user destropy a running app, destroy the registerred
 * information which is in task manager module.
 * @param sid
 *      \ suite id.
 * @param mid
 *      \ MIDlet number.
 * @param winHandle
 *      \ The corresponding window handle.
 */
void Jbed_tm_destroyRunningAppInfo(int sid, int mid);

void Jbed_createJbedDownTimer();

/**
 * Query current running MIDlets informations.
 * @param raInfo
 *      \ running application information. The value is passed
 *        out, need to be freed by caller.
 * @return the total running app number.
 */
int Jbed_tm_queryRunningAppInfo(RunningApp **raInfo);

/**
 * Query the specific suite id whether is running.
 * @return true means in running state, otherwise not running.
 */
BOOLEAN Jbed_tm_queryRunningAppSuite(int sid);


#endif //#ifndef __CPL_TM_H__
