/*
 * Copyright 2006-2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */

#ifndef __JBED_FILE_EVENTS_H__
#define __JBED_FILE_EVENTS_H__

#include <jbed_types.h>
#include <jbed_vm_common.h>
#include <jbedcoreinstance.h>
#include <jbed_upcall.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @file
 * Jbed Porting Layer: Optional helper functions to help implement the \link jpl_file_events
 * JPL interface (platform-independent, VM-independent).
 *
 */

/**
 * @name Convenience functions to set the initial roots.
 * @{
 */

/**
 * Creates the initial root mappings, given the provided root UTF8-encoded root mappings.
 * WARNING: This function will cause allocation on the JAVA heap.
 * See also #JPL_file_events_nativeFsListenerUp.
 *
 * @param initialRoots An array of 2 JAVA <code>String[]</code>. <code>initialRoots[0]</code>
 *        will be set with the content of <code>rootNames</code> and <code>initialRoots[1]</code>
 *        will be set with the content of <code>rootPaths</code>.
 * @param rootNames The initial set of root names, UTF8-encoded.
 * @param rootPaths The initial set of root paths, UTF8-encoded. MAY be <code>NULL</code> if
 *        <code>com.jbed.io.IoToolkit.mapRoots()</code> is configured to return
 *        <code>false</code>, in which case <code>initialRoots[1]</code> will be <code>NULL</code>.
 * @param length The number of initial roots to set. -1 means that the
 *        <code>rootNames</code> and <code>rootPaths</code> are
          <code>NULL</code>-terminated arrays of UTF8-encoded strings.
 * @return #CPL_FILE_SUCCESS if the initial roots could be set, #CPL_FILE_FAILURE otherwise.
 */
jint Jbed_file_events_initialRootsUtf8(REFARRAY initialRoots,
                                       char** rootNames,
                                       char** rootPaths,
                                       jint length);

/* @} */

/**
 * @name Convenience functions to manage the file event queue.
 * @{
 */

/**
 * The queue where file events are to be posted. Can only be used after Jbed_file_events_openQueue()
 * which sets it when the queue is first opened. It is set to <code>NULL</code> when
 *  Jbed_file_events_closeQueue() is called and the count reaches 0.
 */
extern Jbed_upcall_queue* jbed_fc_upcallQueue;

/**
 * Opens the queue for sending file events. The calls to this function are counted.
 * The open count is increased by 1 for each call to this function.
 * See also #JPL_file_events_nativeFsListenerUp.
 *
 * @param words The size of the queue to be created. Only has effect the first time
 *              that this function is called.
 * @param priority The priority of the queue. Only has effect the first time
 *              that this function is called.
 * @return #CPL_FILE_SUCCESS if the queue could be created, #CPL_FILE_FAILURE otherwise.
 */
jint Jbed_file_events_openQueue(int32_t words, int32_t priority);

/**
 * Closes the file event queue. Each call to this function decreases the open count by 1.
 * When the open count reaches 0, the queue is closed.
 * See also #JPL_file_events_nativeFsListenerDown.
 *
 * @return #CPL_FILE_SUCCESS or #CPL_FILE_FAILURE
 */
jint Jbed_file_events_closeQueue(void);

/**
 * Notifies Java world that a file system root have been added or removed.
 *
 * @param rootAdded TRUE if a root have been added, FALSE if removed.
 * @param rootName name of the root, UTF8-encoded
 * @param rootPath platform path of the root added, UTF8-encoded
 *
 * @return #CPL_FILE_SUCCESS or #CPL_FILE_FAILURE
 */
jint Jbed_file_events_notifyRootChanged(jint rootAdded, char *rootName, char *rootPath);

#ifdef IASSERTS
/**
 * Asserts that the jbed_file_events module has been properly cleaned up. MAY be called
 * after the VM has shut down to verify that all resources used by this module have been
 * cleaned up properly.
 */
void Jbed_file_events_assertCleanedUp(void);
#endif

/* @} */

#ifdef __cplusplus
}
#endif

#endif
