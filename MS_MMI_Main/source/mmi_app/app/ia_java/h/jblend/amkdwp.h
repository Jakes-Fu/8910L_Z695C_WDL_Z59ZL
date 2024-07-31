/*
 * Copyright 2002-2004 Aplix Corporation. All rights reserved.
 */

/**
 * @file AMKdwp.h
 *    Debugging Features JBI
 */
/**
 * @addtogroup Module_AMKDWP
 * @{
 */

#ifndef ___AMKDWP_H
#define ___AMKDWP_H

#include "jktypes.h"

#ifdef __cplusplus
extern "C" {
#endif



/**
 * Outputs a character to the Debug Agent for debugging use.
 *
 * @param c
 *     A Unicode character to be sent to the Debug Agent.
 *
 * @return
 *     The character designated in argument c: succeeded <br>
 *     Negative value: failed
 *
 * <p>
 * Sends the designated Unicode character to the Debug Agent.
 * The character sent by this function is output to the Debug Agent console.
 * <p>
 * This function can be called when the Java application to be debugged is running on the target device, the Debug Agent is running on a PC, and the target device and PC are linked by KDWP connection. If this function is called while a KDWP connection is not active, a negative value is returned.
 * </p>
 * 
 */
JKSint32 AmPutcToKdp(JKSint32 c);

/**
 * Switches between writing and not writing standard output. 
 *
 * @param flag
 *    A flag governing whether or not to write standard output. 
 *    Set either of the following values.
 *    <ul>
 *    <li>TRUE: Write standard output.<br>
 *    <li>FALSE: Do not write standard output.
 *    </ul>
 * <p>
 * This function switches between writing and not writing to the standard
 * output stream (<code>System.out</code>) and standard error stream
 *(<code>System.err</code>). 
 * In initial state, writing standard output is enabled. 
 * </p>
 * 
 * When #AmKdwpSystemOut() was not called, or when it was called designating <i>TRUE</i>, 
 * standard output is sent to the Debug Agent console or debugger output window.
 * If <i>FALSE</i> is designated, no output is made. 
 * 
 * The destination is determined based on whether UEI program is used.
 * <ul>
 * <li>UEI program is used.<br>
 *     The standard output is written to the debugger output window. 
 *     Debug Agent console is not displayed in this case.
 * <li>UEI program is not used.<br>
 *     The standard output is written to the Debug Agent console.
 * </ul>
 * 
 * This function must be called after the JVM event JK_VMEVENT_VMSTARTED is notified 
 * when the JVM has started normally.
 * If it is called before this point, then when the JVM is initialized internally,
 * the #AmKdwpSystemOut() call itself will end up being initialized. 
 * Once JK_VMEVENT_VMSTARTED has been notified, this function may be called as often
 * as necessary. 
 * 
 * Since writing standard output is enabled in initial state, as long as
 * standard output has not been disabled by #AmKdwpSystemOut(), note carefully that
 * when debugging starts, standard output will be displayed in the Debug Agent console
 * or debugger window.
 * To control the writing of standard output when debugging starts, 
 * call this function in the callback function to which JK_VMEVENT_VMSTARTED is
 * passed.
 */
void AmKdwpSystemOut(JKBool flag);



/**
 *  A structure type for containing information about the Java package for which 
 *  debugging is disabled.
 *
 *  This structure is used as parameter to #AmKdwpAddHiddenPackages()
 *  for passing information about the Java packages to be excluded from 
 *  the list of debugging targets.
 *
 *  This structure instance should not be declared as const because
 *  JBlend may set a value in the <i>next</i> member.
 */
typedef struct T_KDWP_HiddenPackage {
    /**
     *  A pointer to the memory area holding the name of the Java package for which 
     *  debugging disabled.
     *
     *  The Java package with exactly the same name as this member and 
     *  its sub-packages are excluded from the list of debugging targets.
     *
     *  Specify the package name as follows.
     *  <ul>
     *  <li>Use only ASCII characters.
     *  <li>Terminate the string by '\0'.
     *  <li>Concatenate sub-pacages by '/' not by '.'.
     *  <li>The last character must not be '/'.
     *  </ul>
     */
    const JKUint8             *packageName;

    /**
     *  This is reserved for future use.  Set JK_FALSE.
     */
    JKBool                    reserved1;

    /**
     *  This is reserved for future use.  Set JK_NULL.
     */
    void                      *reserved2;

    /**
     *  A pointer to the memory area holding the next T_KDWP_HiddenPackage structure.
     * 
     *  To pass multiple Java packages to #AmKdwpAddHiddenPackages() as the 
     *  packages to be excluded from the debugging target list, set in <i>next</i> 
     *  a pointer to the next structure. Set JK_NULL in the last structure.
     */
    struct T_KDWP_HiddenPackage *next;
} T_KDWP_HiddenPackage;

/**
 *        Disables debugging for Java package(s).
 * @param object
 *        A pointer to the #T_KDWP_HiddenPackage structure holding the 
 *        information about Java package(s) to be excluded from debugging 
 *        target list. This pointer must remain valid until JVM terminates.
 *
 * This function disables debugging for the Java package(s) indicated by 
 * the <i>object</i> parameter and its sub-packages.
 * 
 * This function must be called after starting JVM before debugging is started.
 * The following places are therefore suitable for calling this function.
 * <ul> 
 * <li>In the call back function that receives the JVM event #JK_VMEVENT_VMSTARTED,
 *     by which the native system is notified of completion of JVM start-up.
 * <li>In the function #JkTOpenDebugStream(), by which JBlend initializes the
 *     communication with the Debug Agent.
 * </ul> 
 */
void AmKdwpAddHiddenPackages(T_KDWP_HiddenPackage *object);



#ifdef __cplusplus
}
#endif

#endif

/**
 * @} end of group
 */
