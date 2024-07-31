/**
 * Copyright 2003-2005 Aplix Corporation. All rights reserved.
 */

/**
 * @file JkLog.h
 *  Trace Output
 */
/**
 * @addtogroup Module_Debugging_Support
 * @{
 */

#ifndef ___SYS_JK_LOG_H
#define ___SYS_JK_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jktypes.h"

/**
 * Outputs trace data generated by the Java application.
 *
 * @param c
 *        A byte in the character string passed by the Java application in 
 *        java.lang.System.out 
 *
 * When a Java application calls the <code>java.lang.System.out</code> method 
 * and passes a character string, JBlend encodes the passed character string 
 * using the default character encoding and passes it in this function a byte 
 * at a time. 
 * 
 * If JBlend inserts line breaks in the trace information, LF (hexadecimal 0x0a) 
 * is passed in this function as the line feed code.
 * 
 *  @if DOJA_OE
 * This function is called only when the ADF AppTrace key parameter string is "on".
 *  @endif
 */
void JkAppLogPutc( JKSint c );

/**
 * Outputs a character in the trace data generated by the JBlend JVM library.
 * 
 * @param c
 *        The output character, passed as ASCII code (0x20-0x7E).
 * @return
 *   0: succeeded<br>
 *   negative value: failed
 *
 * JBlend currently does not care about the return value of this function.
 * 
 * If JBlend inserts line breaks in the trace information, LF (hexadecimal 0x0a) 
 * is passed in this function as the line feed code.
 */
JKSint JkTraceLogPutc( JKSint c );

#ifdef __cplusplus
}
#endif

#endif

/**
 * @} end of group
 */
