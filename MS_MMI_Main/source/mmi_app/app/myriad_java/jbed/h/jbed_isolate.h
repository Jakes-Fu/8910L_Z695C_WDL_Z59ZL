/*
 * Copyright 2005-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: Isolate support.
 *
 * This file contains prototypes of functions for managing the lifecycle of
 * isolates as well as setting and querying runtime parameters of isolates. A
 * call to any of these functions represents an upcall from native code into
 * the VM. <b>These functions are designed to be called only from the context
 * of the Jbed task</b>, i.e. the task running the VM. Calling any of these
 * functions from any other task may interrupt the Jbed task in a critical
 * section and yield unpredictable results.
 */

#ifndef __JBED_JPL_CONC_H__
#define __JBED_JPL_CONC_H__

#include <jbed_jbni.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name Isolate management
 * Functions for creating isolates, setting and querying their runtime
 * characteristics and killing isolates.
 */
/*@{*/

/**
 * Create and start a new isolate.
 *
 * @param argc The number of arguments.
 * @param argv The argument list.
 * Each <code>char *argv[n]</code> entry is expected to reference a null
 * terminated UTF8 encoded string.
 * @param priority The priority of the new isolate.
 * @return The ID of the new isolate, or a negative value if creating the new
 * isolate failed.
 */
extern jint JBNI_IMPORT Jbed_newIsolate(jint argc, char JPTR JPTR argv, jint priority);

/**
 * Set the priority of an Isolate.
 *
 * @param id The ID of the isolate in question.
 * @param priority The new priority value. Valid priority values are
 * from 1 to 10 inclusive.
 */
extern void JBNI_IMPORT Jbed_setIsolatePriority(jint id, jint priority);

/**
 * Kill an isolate.
 *
 * @param id The ID of the isolate to kill.
 */
extern jboolean JBNI_IMPORT Jbed_killIsolate(jint id);

/**
 * Kill an isolate, ignoring async finalizers which did not complete yet.
 *
 * @param id The ID of the isolate to kill.
 */
extern jboolean JBNI_IMPORT Jbed_forceKillIsolate(jint id);

/**
 * Check if an isolate is alive.
 *
 * @param id The ID of the isolate to check.
 *
 * @return JBNI_TRUE if the isolate is still alive.
 */
extern jboolean JBNI_IMPORT Jbed_isolateAlive(jint id);

/**
 * Return the current heap usage of an isolate.
 * The returned value is a worst-case estimate based on the exact value
 * determined at the last run of the garbage collector and any allocations made
 * by the isolate since then. Running the garbage collector before calling this
 * function will yield more precise values.
 *
 * @param id The ID of the isolate to check.
 *
 * @return The heap usage of this isolate in bytes.
 */
extern jint JBNI_IMPORT Jbed_isolateHeapUsage(jint id);

/**
 * Return the CPU time given to an isolate.
 *
 * @param id The ID of the isolate to check.
 *
 * @return The total CPU time spent in this isolate is milliseconds.
 */
extern jlong JBNI_IMPORT Jbed_isolateCpuTime(jint id);

/**
 * Return the absolute time until this isolate is idle.
 *
 * @param isolate The ID of the isolate to check.
 *
 * @return The time in milliseconds when this isolate wakes up (timebase is
 *         OPL_os_getElapsedMicros()). If the isolate is not idle, -1 is
 *         returned. If the isolate is not found, or concurrentIsolates is
 *         not enabled, -2 is returned.
 */
extern jlong JBNI_IMPORT Jbed_isolateIdleUntil(jint id);

/**
 * Return the number of threads running within an isolate.
 *
 * @param id The ID of the isolate to check.
 *
 * @return The number of threads running in this isolate.
 */
extern jint JBNI_IMPORT Jbed_isolateThreadCount(jint id);

/**
 * Fetch the ID of the currently running isolate, and as a special case in a
 * finalizer, the ID of the isolate that allocated the object being finalized.
 *
 * @return The ID of the currently running isolate, or a negative value if no
 * isolate is running or another error occurred.
 */
extern jint JBNI_IMPORT Jbed_currentIsolate(void);

/*@}*/

/** @name Heap quota management
 * Manipulate and query quota parameters of a single isolate or assign an
 * isolate to an existing quota group.
 */
/*@{*/

/**
 * Return the current heap quota given to an isolate.
 *
 * @param id The ID of the isolate to check.
 *
 * @return The heap quota of this isolate in bytes.
 */
extern jint JBNI_IMPORT Jbed_isolateHeapQuota(jint id);

/**
 * Set the heap quota for the given isolate.
 *
 * @param id The ID of the isolate to change.
 * @param quota The new heap quota for that isolate in bytes.
 *
 * @return 0 on success, a negative value on error.
 */
extern jint JBNI_IMPORT Jbed_setIsolateHeapQuota(jint id, jint quota);

/**
 * Give a pariticular isolate a fixed-size, reserved heap space.
 *
 * @param id The ID of the isolate to check.
 * @param reservedSpace The heap space to reserve for the isolate in bytes.
 *
 * @return 0 If the allocation succeeded. If there wasn't enough room, no heap
 * space is reserved, but this method returns the maximum amount of space which
 * can currently be reserved. If this method failed for any reason, e.g. if the
 * ID is invalid, or reserved heap quotas aren't supported, a negative value is
 * returned.
 */
extern jint JBNI_IMPORT Jbed_setIsolateReservedHeapSpace(jint id, jint reservedSpace);

/**
 * Return the ID of the quota group this isolate belongs to.
 *
 * @param id The ID of the isolate to check.
 *
 * @return The ID of the quota group the isolate belongs to, or a negative
 * value if the isolate does not belong to a quota group.
 */
extern jint JBNI_IMPORT Jbed_isolateQuotaGroup(jint id);

/**
 * Set the quota group for the given isolate.
 *
 * Assignment of a quota group is only valid if a quota group hasn't
 * already been assigned for the same isolate. An error is returned if a
 * group change is attempted.
 *
 * @param id The ID of the isolate to add to the group.
 * @param gid The ID of the quota group.
 *
 * @return 0 on success, a negative value on error.
 */
extern jint JBNI_IMPORT Jbed_setIsolateQuotaGroup(jint id, jint gid);

/*@}*/

/** @name Quota group management
 * Create and delete quota groups and modify their quota parameters.
 */
/*@{*/

/**
 * Create a new quota group and return its ID.
 *
 * @return The ID of the newly created group, or a negative value on error.
 */
extern jint JBNI_IMPORT Jbed_newQuotaGroup(void);

/**
 * Return the current heap usage of a quota group.
 * The returned value represents the Java heap usage of the given quota group
 * only. It does not include native allocations reported for that group.
 *
 * The returned value is a worst-case estimate based on the exact value
 * determined at the last run of the garbage collector and any allocations made
 * by the group since then. Running the garbage collector before calling this
 * function will yield more precise values.
 *
 * @param id The ID of the quota group to check.
 *
 * @return The heap usage of this quota group in bytes.
 */
extern jint JBNI_IMPORT Jbed_groupHeapUsage(jint gid);

/**
 * Return the current heap quota given to a quota group.
 * The returned value is the quota value that has been set for the given group.
 * This may be different from the effective quota used for that group if native
 * allocation tracking is used.
 *
 * @param gid The ID of the quota group to check.
 *
 * @return The heap quota in bytes, a negative value if the given ID is
 * invalid.
 */
extern jint JBNI_IMPORT Jbed_groupHeapQuota(jint gid);

/**
 * Set the quota of a quota group.
 *
 * @param gid The ID of the quota group to modify.
 * @param quota The new quota for this group, which is the amount of heap
 * memory this group may not exceed.
 *
 * @return 0 on success, a negative value if the given ID or quota value are
 * invalid.
 */
extern jint JBNI_IMPORT Jbed_setGroupHeapQuota(jint gid, jint quota);

/**
 * Set the reserved heap space for a quota group.
 * Heap memory reservations are effective immediately. If the requested amount
 * of memory isn't available for reservation, an error is returned.  Use
 * <code>deleteQuotaGroup()</code> when the group is no longer needed.  This
 * will cancel the heap reservation.
 *
 * @param gid The ID of the group to modify.
 * @param reserved The reserved space to give to the quota group.
 *
 * @return 0 on success, a negative value if the given ID or amount are
 * invalid. A positive number indicates that the reservation was not
 * successful. The returned value indicates the amount of heap memory available
 * for reservation at this time.
 */
extern jint JBNI_IMPORT Jbed_setGroupReservedHeapSpace(jint gid, jint reserved);

/**
 * Delete a quota group and cancel its heap reservation.
 *
 * @param gid The ID of the quota group to delete.
 *
 * @return 0 on success, a negative value if the given ID is invalid.
 */
extern jint JBNI_IMPORT Jbed_deleteQuotaGroup(jint gid);

/*@}*/

/** @name Native allocation tracking
 * Functions to report and query the amount of natively allocated memory that
 * can be attributed to a quota group. When native memory usage is reported,
 * the amount of natively allocated memory for that group is deducted from the
 * heap quota for the respective quota group.
 *
 * The use of this feature is optional. It is most useful on platforms where
 * the Java heap is allocated in chunks from the native heap. Using these
 * functions then effectively transforms the Java heap quota into a quota for
 * the native heap. Note, however, that the resulting native heap quota
 * management is not entirely accurate. It does not account for free space on
 * the Java heap, Java heap chunk size and objects that can not be attributed
 * to any quota group.
 *
 * When using native allocation tracking, the quota parameter for each quota
 * group must be set to include any natively allocated memory for that group.
 * Native code should also check the sum of Jbed_groupHeapUsage() and
 * Jbed_groupNativeUsage() against Jbed_groupHeapQuota() before allowing a
 * native allocation.
 */
/*@{*/

/**
 * Report native memory usage for a quota group.
 * Use this to incrementally report native memory usage for this quota group.
 * An internal counter accumulates reported values and decreases the effective
 * quota of the group by the amount of currently allocated native memory.
 *
 * @param gid The ID of the group responsible for the native allocation.
 * @param delta The size of the native allocation in bytes.  Negative values
 * are used to report an amount of memory that has been freed.
 *
 * @return 0 on success, a negative value if the given delta would result in a
 * negative value for this group's total native allocation.
 */
extern jint JBNI_IMPORT Jbed_groupAddNativeUsage(jint gid, jint delta);

/**
 * Return the amount of memory natively allocated by a quota group.
 *
 * @return The amount of natively allocated memory in bytes, or a negative
 * value if the given ID is invalid.
 */
extern jint JBNI_IMPORT Jbed_groupNativeUsage(jint gid);

/*@}*/

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_JPL_CONC_H__ */
