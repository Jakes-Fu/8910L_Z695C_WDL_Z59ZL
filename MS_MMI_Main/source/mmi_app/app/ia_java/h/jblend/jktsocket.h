/*
 * Copyright 2002,2003 Aplix Corporation. All rights reserved.
 */

/**
 * @file JkTSocket.h
 *  KDWP Communication JSI for JVM 
 */
/**
 * @addtogroup Module_JkT
 * @{
 */

#ifndef ___SYS_JKTSOCKET_H
#define ___SYS_JKTSOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jktypes.h"

/**
 * Handle type for identifying a KDWP connection from the JVM to the Debug Agent
 */
typedef void *JkTHandle_t;

/**
 *                Waits for data to arrive from the Debug Agent.
 *
 * @param handle  The KDWP connection handle created by #JkTAcceptDebugStream()
 * @param timeout Wait timeout in milliseconds. 
 *                The value '-1' means wait until data is received.
 * @return        Return 0 if data arrives before the request times out; 
 *                else return a value other than 0.
 * 
 * If a native system task calls AamsVmKill() while this function is executing, 
 * #JkTCloseDebugStream() is called from the AamsVmKill() processing, designating
 * the same handle as this function.
 * In this case, abort the processing for this function and return a value other than 0. 
 * If the processing cannot be aborted, it may be continued, but #JkTCloseDebugStream()
 * must return control to its caller.
 */
JKSint32 JkTWaitDebugStream(JkTHandle_t handle, JKSint32 timeout);

/**
 *                Sends data to the Debug Agent.
 * @param handle  The KDWP connection handle created by #JkTAcceptDebugStream()
 * @param buffer  A pointer to the buffer holding the data to be sent
 * @param length  The length (in bytes) of the data to be sent
 * @param written A pointer to the buffer for storing the length of sent data
 * @return        Return 0 if the operation succeeds; 
 *                else return a value other than 0.
 *
 * Send processing must not be interrupted until all the data has been sent.
 * 
 * If a native system task calls AamsVmKill() while this function is executing, 
 * #JkTCloseDebugStream() is called from the AamsVmKill() processing, designating
 * the same handle as this function.
 * In this case, abort the processing for this function and return a value other than 0. 
 * If the processing cannot be aborted, it may be continued, but #JkTCloseDebugStream()
 * must return control to its caller.
 */
JKSint32 JkTWriteDebugStream(JkTHandle_t handle, const void *buffer,
    JKUint32 length, JKUint32 *written);

/**
 *                Receives data from the Debug Agent.
 * @param handle  The KDWP connection handle created by #JkTAcceptDebugStream()
 * @param buffer  A pointer to the buffer for storing the received data
 * @param length  The length (in bytes) of the data to be received
 * @param read    A pointer to the buffer for storing the length of received data
 * @return        Return 0 if the operation succeeds; 
 *                else return a value other than 0.
 *
 * After this function is called, receive processing must not be interrupted until
 * at least one byte of data has been received; after that, receipt may be interrupted.
 * 
 * If a native system task calls AamsVmKill() while this function is executing, 
 * #JkTCloseDebugStream() is called from the AamsVmKill() processing, designating
 * the same handle as this function.
 * In this case, abort the processing for this function and return a value other than 0. 
 * If the processing cannot be aborted, it may be continued, but #JkTCloseDebugStream()
 * must return control to its caller.
 */
JKSint32 JkTReadDebugStream(JkTHandle_t handle, void *buffer, JKUint32 length,
    JKUint32 *read);

/**
 *                Initializes a connection with the Debug Agent.
 * @param port    The JVM port used by the Debug Agent for the connection
 * @param handle  A pointer to the buffer for storing the KDWP connection handle. 
 *                The handle must not be zero.
 * @return        Return 0 if the operation succeeds; 
 *                else return a value other than 0.
 *
 * This function is called when the JVM starts debugging.
 */
JKSint32 JkTOpenDebugStream(JKSint32 port, JkTHandle_t *handle);

/**
 *                 Accepts a connection request by the Debug Agent.
 * @param handle   The KDWP connection handle created by #JkTOpenDebugStream()
 * @param timeout  Wait timeout in seconds
 * @param accepted A pointer to the buffer for storing the KDWP connection handle. 
 *                 The handle must not be zero.
 * @return         Return 0 if the operation succeeds; 
 *                 else return a value other than 0.
 *
 * This function is called only once, right after #JkTOpenDebugStream() is called.
 * When this function is executed, communication with the Debug Agent is enabled.
 * 
 * If a native system task calls AamsVmKill() while this function is executing, 
 * #JkTCloseDebugStream() is called from the AamsVmKill() processing, designating
 * the same handle as this function.
 * In this case, abort the processing for this function and return a value other than 0. 
 * If the processing cannot be aborted, it may be continued, but #JkTCloseDebugStream()
 * must return control to its caller.
 */
JKSint32 JkTAcceptDebugStream(JkTHandle_t handle, JKSint32 timeout,
    JkTHandle_t *accepted);

/**
 *               Closes a connection to the Debug Agent.
 * @param handle The KDWP connection handle created by 
 *               #JkTOpenDebugStream() or #JkTAcceptDebugStream()
 * @return       Return 0 if the operation succeeds; 
 *               else return a value other than 0.
 *
 * This function is called once each for the handles created by #JkTOpenDebugStream()
 * and #JkTAcceptDebugStream(), respectively.
 * 
  * If a native system task calls AamsVmKill() to forcibly terminate the
 * Java application while #JkTWaitDebugStream(), #JkTWriteDebugStream(),
 * #JkTReadDebugStream(), or  #JkTAcceptDebugStream() is executing,
 * this function is called from the AamsVmKill() processing.
 * It is possible to return failure in this case without completing the
 * close processing, but control must be returned to the caller of 
 * #JkTCloseDebugStream().
 */
JKSint32 JkTCloseDebugStream(JkTHandle_t handle);

#ifdef __cplusplus
}
#endif

#endif

/**
 * @} end of group
 */
