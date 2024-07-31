/*
 * Copyright 2002 Aplix Corporation. All rights reserved.
 */

/**
 * @file
 * API functions for modifying date field properties.
 */

#ifndef ___GUI_USER_DATEFIELD_H
#define ___GUI_USER_DATEFIELD_H

#include <jktypes.h>

#include <gui/common/jgstring.h>
#include <gui/common/jgdatefield.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup GUI_DateField
 * @{
 */

/**
 * Activates date input control.
 * 
 * @param datefield
 *        The date field
 */
void JgurDateFieldLaunchDateModule(JGDateField datefield);

/**
 * Terminates date input control.
 * 
 * @param datefield
 *        The date field
 */
void JgurDateFieldCancelDateModule(JGDateField datefield);

/**
 *  Gets the character string indicating the date (year-month-day, day 
 *  of the week) or time (hour-minute) set in the date field.
 *
 *   @param datefield The date field 
 *   @param part      The type of string to be acquired.
 *                    One of the following values is designated.
 *                    <ul>
 *                     <li>#JG_DFIELD_INPUTMODE_DATE
 *                     <li>#JG_DFIELD_INPUTMODE_TIME
 *                    </ul>
 *   @param strBuf    The handle of the string buffer for putting the acquired
 *                    string. The buffer is cleared when this function is called.
 */
void JgurDateFieldGetString(JGDateField datefield, JKUint32 part, JGStrBuf strBuf);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !___GUI_USER_DATEFIELD_H */

