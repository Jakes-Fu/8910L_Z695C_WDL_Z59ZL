/*
 * Copyright 1998-2005 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: PushRegistry support (VM-independent)
 *
 * This interface provides pushed content support for the Jbed VM.
 * This interface allows optionally replacing the persistence
 * implementation for push registrations. A default implementation
 * that works for systems that support synchronous I/O via cpl_file.h is
 * provided so implementing this interface explictly is often not
 * necessary.
 *
 * <!-- #interface list begin -->
 * \section cpl_push_interface Interface
 *    - CPL_push_open()
 *    - CPL_push_write()
 *    - CPL_push_read()
 *    - CPL_push_close()
 * <!-- #interface list end -->
 *
 * PushRegistry support is mandatory for MIDP 2.0 builds of Jbed though no
 * protocol or alarm support is mandated.
 * For MIDP 2.0 builds of Jbed, define the symbol <b>MIDP2_PUSH_SUPPORT</b>
 * if any PushRegistry protocol or alarms are implemented.
 */

#ifndef __JBED_CPL_PUSH_H__
#define __JBED_CPL_PUSH_H__

#include <jbed_global.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name Persistence for the Jbed Push registry
 *
 * This interface provides a persistence layer for the push registry
 * support. Note that a default implementation of these APIs in terms
 * of the standard CPL file API (cpl_file.h) is provided with
 * Jbed. This implementation is normally sufficient and there is no
 * need to implement this interface explicitly for a target. That
 * should only be done if the default implementation is not suitable
 * for a given target, e.g. when there is support for synchronous
 * file access.
 *
 * Note that these interfaces are strictly synchronous and because
 * they are part of the push registry may be called when the VM is not
 * otherwise active.
 *
 * These APIs are used to read or write a series NUL terminated C strings
 * as if they were read or written to a file.
 *
 * The calling sequence is either
 *
 *    - CPL_push_open(CPL_PUSH_READ)
 *    - CPL_push_read
 *    - CPL_push_read
 *    - ...
 *    - CPL_push_close
 *
 * or
 *
 *    - CPL_push_open(CPL_PUSH_WRITE)
 *    - CPL_push_write
 *    - CPL_push_write
 *    - ...
 *    - CPL_push_close
 *
 */
/*@{*/

/**
 * Indicates we are intending to read persistent entries
 */
#define CPL_PUSH_READ 0
/**
 * Indicates we are intending to write persistent entries
 */
#define CPL_PUSH_WRITE 1

/**
 * Operation completed successfully
 */
#define CPL_PUSH_SUCCESS 0
/**
 * Operation failed
 */
#define CPL_PUSH_ERROR -1
/**
 * Operation failed (for CPL_push_open)
 */
#define CPL_PUSH_INVALID_HANDLE -1

#define CPL_PUSH_FILENAME      "pushlist.txt"
#define CPL_PUSH_FILENAME_LEN  12

/**
 * Open the persistent PushRegistry store.
 *
 * @param mode Open mode, one of CPL_PUSH_READ or CPL_PUSH_WRITE.
 * @return An opaque handle or CPL_PUSH_INVALID_HANDLE on error.
 */
int32_t CPL_push_open(int32_t mode);

/**
 * Write an entry to the persistent PushRegistry store.
 *
 * @param handle Store handle.
 * @param value PushRegistry entry string.
 * @return CPL_PUSH_SUCCESS or CPL_PUSH_ERROR.
 */
int32_t CPL_push_write(int32_t handle, const char *value);

/**
 * Read an entry from the persistent PushRegistry store.
 *
 * @param handle Store handle.
 * @return PushRegistry entry string (NUL terminated) or NULL on end of file.
 * Note that this is a volatile pointer in that its value and the data it
 * points to can change on the next call of this function or CPL_push_close().
 */
char *CPL_push_read(int32_t handle);

/**
 * Close the persistent PushRegistry store.
 *
 * @param handle Store handle.
 * @return CPL_PUSH_SUCCESS or CPL_PUSH_ERROR.
 */
int32_t CPL_push_close(int32_t handle);
/*@}*/

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_CPL_PUSH_H__ */

