/*
 * Copyright 2004-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: File support (VM-independent)
 *
 * This interface provides file support for the Jbed VM.
 *
 * \section cpl_file_naming File Names
 *
 * File names passed to the cpl_file.h implementation correspond to the path part of a
 * RFC1738-compliant file URL, which corresponds to the "abs_path" definition
 * in the BNF of RFC2396. However we extend it by allowing any Unicode Version 2.1
 * characters to be used in the "path_segments". Please refer to the Application
 * Management Software (AMS) documentation for details on the file names used.
 * The file names are UCS-2 encoded. Their maximum length is set by #MAX_FILENAME_LEN.
 * <br>
 *
 * References:
 * <ul>
 * <li><a href="http://www.unicode.org/reports/tr8/index.html">
 *      Unicode Technical Report #8, The Unicode Standard, Version 2.1
 * </a></li>
 * <li><a href="http://www.unicode.org/reports/tr17/index.html">
 *      Unicode Technical Report #17, Character Encoding Model.
 * </a></li>
 * <li><a href="http://www.rfc-archive.org/getrfc?rfc=1738">
 *      RFC1738: Uniform Resource Locators (URL)
 * </a></li>
 * <li><a href="http://www.rfc-archive.org/getrfc?rfc=2396">
 *      RFC2396: Uniform Resource Identifiers (URI): Generic Syntax.
 * </a></li>
 * </ul>
 *
 * \section cpl_file_interface Interface
 *
 * \subsection cpl_file_async Asynchronous Operation
 *
 * Most of the functions take an #ASYNC_notifier notifier argument. If this
 * argument is NOT NULL, then the implementation MAY implement the function
 * asynchronously, using the functionality provided by the async.h module.
 * If the notifier argument is NULL, then the implementation MUST complete
 * synchronously. The \ref cpl_file_usage section indicates this requirement
 * for each of the CPL_file functions. If the host file system is
 * asynchronous, please read the
 * <a href="../../../manuals/porting/fileSystem.html#asyncFSPort">
 * file system porting guide</a> for more information.
 *
 * When a CPL_file_XXX function calls ASYNC_callAgainWhenSignalled(), it MUST
 * return #CPL_FILE_WOULDBLOCK. The CPL_file_XXX function will always be called
 * again after the asynchronous implementation has called ASYNC_notify().
 * The last call to the CPL function will therefore return the results
 * synchronously. This allows:
 *
 * - The implementation to perform the operation in several asynchronous steps
 * - The implementation to use the async state buffer as it wishes, it is not
  *  even required to use it
 * - The implementation to free resources associated with the asynchronous operation
 *   easily on the last call to the CPL function
 * - The implementation to choose which flavor of ASYNC_callAgain() it wants to
 *   call before returning #CPL_FILE_WOULDBLOCK
 *
 * \subsection cpl_file_appid Application identifiers
 *
 * Some of the functions of this API take an application-identifier as parameter. The
 * identifier may be used by a port if it wants to impose a quota on particular applications,
 * or if it wants to restrict access to certain files by certain applications.
 * The identifier will always be equal to #JBED_APP_JVM unless native applications are also using
 * this interface. JVM ports may add an assertion that the identifier is equal to #JBED_APP_JVM.
 *
 * \subsection cpl_file_usage Interface usage
 *
 * <!-- #interface list begin -->
 * - Case CLDC. Hard to say what is needed since the usage is driven by the port's Application
 * Management System (AMS) requirements. However the following would typically be needed for
 * class loaders:
 *    - CPL_file_startup() (synchronous on both FastBCC and FastDAC)
 *    - CPL_file_getFileLength() (synchronous on FastDAC)
 *    - CPL_file_open() (synchronous on FastDAC)
 *    - CPL_file_setPosition() (synchronous on FastDAC)
 *    - CPL_file_read() (synchronous on FastDAC)
 *    - CPL_file_close() (synchronous on FastDAC, and when called from the finalizer on FastBCC)
 *    - CPL_file_shutdown() (synchronous on both FastBCC and FastDAC)
 * - Case MIDP 2.0. Same as CLDC with the following additions to support the MIDP 2.0 AMS and RMS:
 *    - CPL_file_totalSize64() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_availableSize64() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_listOpen() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_listNextEntry() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_listClose() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_rename() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_exists() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_delete() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_write() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_truncate() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_setPosition() (asynchronous on both FastBCC and FastDAC)
 *    - CPL_file_flush() (asynchronous on both FastBCC and FastDAC)
 * - Case MIDP 2.0 with default push registry implementation. Same as MIDP 2.0 scenario,
 * but the following MUST be implemented synchronously:
 *    - CPL_file_open() (synchronous on both FastBCC and FastDAC)
 *    - CPL_file_read() (synchronous on both FastBCC and FastDAC)
 *    - CPL_file_write() (synchronous on both FastBCC and FastDAC)
 *    - CPL_file_truncate() (synchronous on both FastBCC and FastDAC)
 *    - CPL_file_close() (synchronous on both FastBCC and FastDAC)
 * - Case CDC. The following MUST be implemented synchronously:
 *    - CPL_file_delete()
 *    - CPL_file_flush_buffers()
 *    - CPL_file_sync
 *
 * \subsubsection cpl_file_extended Extended file support
 *
 * This interface provides extended file support for the Jbed VM.
 * This should be implemented in ports requring the FileConnection Optional
 * Package of JSR-75 or the Connected Device Configuration JSR-36.
 *
 * - The required functionality MUST be implemented in the following functions:
 *    - CPL_file_mkdir()
 *    - CPL_file_directorySize64()
 *    - CPL_file_validFilename()
 *
 * - The following functions MAY be stubs as described below if the underlying
 *   file-system does not support the required functionality:
 *    - CPL_file_setHidden()
 *    - CPL_file_setReadable()
 *    - CPL_file_setWritable()
 *    - CPL_file_isHidden()
 *    - CPL_file_canRead()
 *    - CPL_file_canWrite()
 *    - CPL_file_lastModified()
 *    - CPL_file_setLastModified()
 *
 * \subsubsection cpl_file_uapps Support for u@APPS
 *
 * Native applications such as u\@WAP or u\@MMS require only the following subset
 * of the functionality provided by this API. All APIs are synchronous:
 *    - CPL_file_startup()
 *    - CPL_file_getFileLength()
 *    - CPL_file_open()
 *    - CPL_file_setPosition()
 *    - CPL_file_read()
 *    - CPL_file_close()
 *    - CPL_file_shutdown()
 *    - CPL_file_delete()
 *    - CPL_file_write()
 *    - CPL_file_truncate()
 *    - CPL_file_mkdir()
 *    - CPL_file_lastModified()
 * <!-- #interface list end -->
 */

#ifndef __CPL_FILE_H__
#define __CPL_FILE_H__

/* For the ASYNC_notifier type. */
#include <jbed_async.h>

/* For the application ids */
#include <jbed_apps.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name File system entity types.
 * @{
 */

/** Type value of a regular file or file name. */
#define CPL_FILE_ISREG 1
/** Type value of a directory or directory name. */
#define CPL_FILE_ISDIR 2
/** Type value of a filter name */
#define CPL_FILE_ISFILTER 3

/*@}*/

/**
 * @name Return Values for the CPL_file_XXX functions.
 */

/*@{*/

/** Return code that indicates successful completion of an operation. */
#define CPL_FILE_SUCCESS 0
/** Indicates that an operation failed. */
#define CPL_FILE_FAILURE -1
/** Indicates that an operation is implemented asynchronously, and the caller
 *  will be notified by an ASYNC_notify() when the results are made available
 *  in the asynchronous IO buffers.
 */
#define CPL_FILE_WOULDBLOCK -2
/**
 * Indicates that the a CPL_file_read() call reached the end of the file.
 */
#define CPL_FILE_EOF -3

/*@}*/

/**
 * @name CPL_asyncfile_open() mode flags
 */

/*@{*/

/** Open for read access. */
#define CPL_FILE_MODE_READ 1
/** Open for write access. */
#define CPL_FILE_MODE_WRITE 2
/** Open for synchronised writes, every write operation
 * must proceed through to persistent storage. */
#define CPL_FILE_MODE_WRITE_SYNC 4

/*@}*/

/**
 * @name CPL_file_permission() permission flags
 */

/*@{*/

/** The read access permission for the file or directory. An application has the
 * read access to the file or direcotry means it can get its attributes
 * (e.g check if the file exists).
 * NOTE: The application has the read access for the file or directory doesn't
 * imply the file or direcotry is existent or readable. For a Read-Only file or
 * directory, the application can't read data from it but still has read access
 * right to get its attribute. */
#define CPL_FILE_PERMISSION_READ 1

/** The write access permission for the file or directory. An application has the
 * write access to the file or direcotry means it can change its attributes
 * (e.g set the file to readable attribute).
 * NOTE: The application has the write access for the file or directory doesn't
 * imply the file or direcotry is existent or writable. For a Read-Only file or
 * directory, the application can't write data to it but still has write access
 * right to change its attribute. */
#define CPL_FILE_PERMISSION_WRITE 2

/*@}*/


#ifndef MAX_FILENAME_LEN
/** Maximum number of characters that a filename may have. By default assumes
 *  that the entry results of CPL_file_listNextEntry() are returned in the async state
 *  buffer, after the #CPL_file_result_s, and calculates the maximum name
 *  from that.
 */
#define MAX_FILENAME_LEN ((ASYNC_STATE_SIZE - sizeof(CPL_file_result_t)) / 2)
#endif

/**
 * @deprecated Structured type that defines how the asynchronous state buffer
 * MAY optionally be used to store the results until the CPL_file function
 * is called again.
 */
typedef struct CPL_file_result_s {
    /** Return value. */
    int32_t retVal;
    union {
        /**
         *  Handle returned by CPL_file_open(). If retVal is #CPL_FILE_SUCCESS,
         *  it MUST be a valid platform-specific handle.
         */
        int32_t handle;
        /**
         *  Structure used by the CPL_file_listOpen() and CPL_file_listNextEntry().
         */
        struct {
            /**
             *  List session identifier value returned by CPL_file_listOpen()
             *  and/or CPL_file_listNextEntry(). If the implementation chooses
             *  to use list session identifiers, the value MUST be set even
             *  if it did not change from the previous call.
             */
            int32_t session;
            /**
             *  List iteration identifier value returned by CPL_file_listOpen()
             *  and/or CPL_file_listNextEntry(). If the implementation chooses
             *  to use list session identifiers, the value MUST be set even
             *  if it did not change from the previous call.
             */
            int32_t iteration;
            /**
             *  List entry returned by CPL_file_listNextEntry().
             */
            uint16_t* entry;
        } list;
    } u;
} CPL_file_result_t;

/**
 * Performs one-time initialization of the File System (FS).
 * This function is called once during the lifetime of an application,
 * and before any call to <code>CPL_file_*</code> functions by this application.
 * When several applications are using the file interface, this function may be called
 * several times, once per application.
 *
 * @return #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_startup(void);

/**
 * Returns the total size (in bytes) of the file system on which a given
 * file is stored.
 *
 * @param name Name of the file, UCS-2 encoded.
 * @param nameChars Number characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_FAILURE or the total size.
 * <code>CPL_FILE_FAILURE</code> is returned when the <code>name</code> parameter is invalid.
 */
int64_t CPL_file_totalSize64(const uint16_t* name,
                             int32_t nameChars,
                             int32_t appId,
                             ASYNC_notifier notifier);

/**
 * Returns the best effort approximation of the available size
 * on the file system for a given file in bytes.
 * For many implementations, this will correspond to the
 * available space in the file system where all the files are stored.
 * Other platforms may place files on different volumes/filesystem and/or
 * have various file size limitations. These platforms can use the name and
 * nameChars parameters to return a file-specific value.
 *
 * If the port needs to ensure that a specific application does not exceed a given storage
 * space quota, the port SHOULD return a value consistent with that quota.
 *
 * @param name Name of the file, UCS-2 encoded.
 * @param nameChars Number characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_FAILURE or the available size.
 * <code>CPL_FILE_FAILURE</code> is returned when the <code>name</code> parameter is invalid.
 */
int64_t CPL_file_availableSize64(const uint16_t* name,
                                 int32_t nameChars,
                                 int32_t appId,
                                 ASYNC_notifier notifier);

/**
 * Returns the length of a file (by name, opened or unopened).
 *
 * @param name Name of the file, UCS-2 encoded.
 * @param nameChars Number characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_FAILURE or the file length.
 */
int32_t CPL_file_getFileLength(const uint16_t* name,
                               int32_t nameChars,
                               int32_t appId,
                               ASYNC_notifier notifier);

/**
 * Initializes a list iteration session. When Jbed needs to fetch a list
 * of file names, it will first call CPL_file_listOpen() to initialize
 * the list \a session and \a iteration parameters. Jbed will then call
 * CPL_file_listNextEntry() 0 or more times to query the file names
 * from the list. Finally, it will call CPL_file_listClose() to tell
 * the implementation that it no longer needs the list.
 *
 * @param prefix Prefix that must be matched, UCS-2 encoded. This prefix is an indication given
 * to the implementation. It is tolerated that an implementation returns
 * non-matching entries, however the names of all the files created by Jbed that
 * match the prefix MUST be returned before the implementation is allowed to
 * return 0. In particular, if an implementation supports directories, it is tolerated
 * that the implementation returns the results to the nearest directory. Directories are
 * denoted by a trailing '/' in their returned name. Directories MUST be returned in platforms
 * that are implementing the FileConnection Optional Package of JSR75. They may be neglected
 * in all other cases as they will be filtered out by the VM. Any current directory
 * indication (".") and any parent directory indication ("..") is not included in the
 * list of files and directories returned. If hidden files are supported on the
 * referenced file system, these should be included in the list of returned values.
 * At a minimum, the implementation MUST be able to return all the files created by
 * Jbed.
 *
 * @param prefixChars Number characters encoded in prefix.
 *
 * @param appId The \ref cpl_file_appid "application-id".
 *
 * @param session List session identifier. This parameter is used by the
 * implementation to keep track of state of the iterative loop. Points to a
 * 32-bit value. Jbed does not use the information placed there. This is intended
 * to be used by the implementation to identify the list session.
 *
 * @param iteration List iteration identifier. This parameter is used by the
 * implementation to keep track of the progress of the listing session. Points to a
 * 32-bit value. This is intended to be used as a list iteration identifier.
 * Jbed does not use the information placed there.
 *
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 *
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_listOpen(const uint16_t* prefix,
                          int32_t prefixChars,
                          int32_t appId,
                          int32_t* session,
                          int32_t* iteration,
                          ASYNC_notifier notifier);

/**
 * Used to fetch a list of file names that match a given name prefix.
 * Jbed will call this function iteratively  in a loop until either
 * the function returns #CPL_FILE_FAILURE or returns a zero-length entry.
 *
 * The implementation MUST maintain the list consistent in the face of
 * file deletions, i.e. if Jbed deletes a file whose name was returned
 * by a call to CPL_file_listNextEntry() in a session, the implemen-
 * tation must still be able to retrieve the remaining entries of that
 * session.
 *
 * @param prefix See CPL_file_listOpen(). This does not change during the
 * iteration session.
 * @param prefixChars See CPL_file_listOpen(). This does not change during
 * the iteration session.
 * @param entry Buffer parameter to return the next file name that matches the
 * #prefix parameter, if any, when the function returns a positive number of
 * characters.
 * @param entryBytes Size of entry in bytes.
 * @param session See CPL_file_listOpen().
 * @param iteration See CPL_file_listOpen().
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_FAILURE or the number of
 * characters encoded in entry. Returns 0 when the end of the list is reached.
 */
int32_t CPL_file_listNextEntry(const uint16_t* prefix,
                               int32_t prefixChars,
                               uint16_t* entry,
                               int32_t entryBytes,
                               int32_t* session,
                               int32_t* iteration,
                               ASYNC_notifier notifier);

/**
 * Ends a list iteration session. Notifies the implementation
 * that the list session is over and that any CPL-side session resources
 * can be released.
 *
 * @param session See CPL_file_listOpen().
 * @param iteration See CPL_file_listOpen().
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_listClose(int32_t session, int32_t iteration, ASYNC_notifier notifier);

/**
 * Renames a file, given its old name. The file or directory is renamed
 * immediately on the actual file system upon invocation of this method.
 * Any open handles on the file specified by oldName become invalid after
 * this method has been called.
 *
 * @param oldName Current file name (unopened), UCS-2 encoded.
 * @param oldNameChars Number of characters encoded on oldName.
 * @param newName New name for the file (unopened), UCS-2 encoded.
 * @param newNameChars Number of characters encoded on newName.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE. In particular,
 * #CPL_FILE_FAILURE if a file or directory already exists with the new name.
 */
int32_t CPL_file_rename(const uint16_t* oldName,
                        int32_t oldNameChars,
                        const uint16_t* newName,
                        int32_t newNameChars,
                        int32_t appId,
                        ASYNC_notifier notifier);

/**
 * Tests if a file exists given its name.
 *
 * @param name Name of the file, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_ISREG, #CPL_FILE_ISDIR, #CPL_FILE_FAILURE. If name
 * exists, returns #CPL_FILE_ISREG if it is a regular file and #CPL_FILE_ISDIR if it is a directory.
 * Returns #CPL_FILE_FAILURE in all other cases, including those where name exists but is neither
 * a regular file nor a directory. Platforms that do not support directories MUST NOT return
 * #CPL_FILE_ISDIR.
 */
int32_t CPL_file_exists(const uint16_t* name,
                        int32_t nameChars,
                        int32_t appId,
                        ASYNC_notifier notifier);

/**
 * Opens a file with the given name and returns its file handle.
 *
 * @param name Name of the file, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param mode Any combination of the #CPL_FILE_MODE_READ and
 * #CPL_FILE_MODE_WRITE flags. If the file does not exist and mode contains the
 * #CPL_FILE_MODE_WRITE flag, then the file is automatically created. If the
 * file exists and the mode contains the #CPL_FILE_MODE_WRITE flag, the file is
 * opened so it can be modified, but the data is not modified by the open call.
 * In all cases the current position is set to the start of the file.
 * The following table shows how to map the mode semantics above to UNIX
 * fopen-style modes.  For brevity in the table, R=#CPL_FILE_MODE_READ,
 * W=#CPL_FILE_MODE_WRITE, E=File exists:
 * <table>
 * <tr><td>RW</td><td>E</td><td>Maps-to</td></tr>
 * <tr><td>00</td><td>0</td><td>Return #CPL_FILE_FAILURE</td></tr>
 * <tr><td>00</td><td>1</td><td>Return #CPL_FILE_FAILURE</td></tr>
 * <tr><td>01</td><td>0</td><td>Use fopen mode "w"</td></tr>
 * <tr><td>01</td><td>1</td><td>Use fopen mode "a" and fseek to the start</td></tr>
 * <tr><td>10</td><td>0</td><td>Return #CPL_FILE_FAILURE</td></tr>
 * <tr><td>10</td><td>1</td><td>Use fopen mode "r"</td></tr>
 * <tr><td>11</td><td>0</td><td>Use fopen mode "w+"</td></tr>
 * <tr><td>11</td><td>1</td><td>Use fopen mode "r+"</td></tr>
 * </table>
 *
 * For Connected Device Configuration JSR-36, and additional flag
 * #CPL_FILE_MODE_WRITE_SYNC is defined for the mode.
 * Implementations may test for the presence of this flag and open the file
 * in a mode which is compatible with synchronised writes to the file.
 * Alternatively ports may ignore this flag if sync style operations are supported
 * on the target platform, see \ref CPL_file_sync.
 *
 * @param appId The \ref cpl_file_appid "application-id".
 * @param handle Pointer where the result handle value is placed when the function
 * is called synchronously. See also \ref cpl_file_async.
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_open(const uint16_t* name,
                      int32_t nameChars,
                      int32_t mode,
                      int32_t appId,
                      int32_t* handle,
                      ASYNC_notifier notifier);

/**
 * Deletes a file given its name, UCS-2 encoded. The file or directory is
 * deleted immediately on the actual file system upon invocation of this
 * method. Any open handles on the file specified by name become invalid
 * after this method has been called.
 *
 * If the port needs to ensure that a specific application does not exceed a given storage
 * space quota, then the bytes freed by the deletion must be added to the available space for
 * that application.
 *
 * @param name Name of the file, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_delete(const uint16_t* name,
                        int32_t nameChars,
                        int32_t appId,
                        ASYNC_notifier notifier);

/**
 * Read bytes from a file at the current position to a buffer. Afterwards the
 * new file position is the byte after the last byte read.
 * CPL_FILE_FAILURE is returned if the handle is invalid (e.g., as a
 * consequence of CPL_file_delete, CPL_file_rename, or CPL_file_close).
 *
 * @param handle File handle as returned by CPL_file_open().
 * @param dst Buffer where the data is to be copied.
 * @param length Number of bytes to be copied.
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE, #CPL_FILE_EOF
 *         or the number of bytes that were read, i.e. in the range 0..length.
 */
int32_t CPL_file_read(int32_t handle,
                      uint8_t* dst,
                      int32_t length,
                      ASYNC_notifier notifier);

/**
 * Write bytes from a buffer to the file at the current position.  If the
 * current position + number of bytes written > current size of the file,
 * then the file is grown.  Afterwards the new file position is the byte
 * after the last byte written.
 * CPL_FILE_FAILURE is returned if the handle is invalid (e.g., as a
 * consequence of CPL_file_delete, CPL_file_rename, or CPL_file_close).
 *
 * @param handle File handle as returned by CPL_file_open().
 * @param src Buffer that contains the bytes to be written.
 * @param length Number of bytes to be written.
 * If the port needs to ensure that a specific application does not exceed a given storage
 * space quota, the implementation must make sure the call does not violate that invariant.
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_FAILURE or the number of bytes
 *         that were written. This number must be in the range 0..length.
 *         Returns #CPL_FILE_FAILURE when storage is full or exceeds quota.
 */
int32_t CPL_file_write(int32_t handle,
                       const uint8_t* src,
                       int32_t length,
                       ASYNC_notifier notifier);

/**
 * Closes a file.
 * CPL_FILE_SUCCESS is returned if the handle is invalid (e.g., as a
 * consequence of CPL_file_delete or CPL_file_rename).
 *
 * @param handle File handle as returned by CPL_file_open().
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_close(int32_t handle, ASYNC_notifier notifier);

/**
 * Truncates a file.  Only called from a file that was opened with the
 * #CPL_FILE_MODE_WRITE flag set. If the new length is smaller that the current
 * position, the current position is set to the end of the truncated file.
 * Otherwise, the current position is not changed.
 * CPL_FILE_FAILURE is returned if the handle is invalid (e.g., as a
 * consequence of CPL_file_delete, CPL_file_rename, or CPL_file_close).
 *
 * If the port needs to ensure that a specific application does not exceed a given storage
 * space quota, then the bytes freed by the truncation must be added to the available space for
 * that application.
 *
 * @param handle File handle as returned by CPL_file_open().
 * @param value The new length of the file.
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_truncate(int32_t handle, int32_t value, ASYNC_notifier notifier);

/**
 * Sets the current position in an opened file.
 * CPL_FILE_FAILURE is returned if the handle is invalid (e.g., as a
 * consequence of CPL_file_delete, CPL_file_rename, or CPL_file_close).
 *
 * @param handle File handle as returned by CPL_file_open().
 * @param value The new current position of the file. If value is greater
 * than the length of the file then the file should be extended. The contents
 * of the newly extended portion of the file is undefined.
 * If the port needs to ensure that a specific application does not exceed a given storage
 * space quota, the implementation must make sure the call does not violate that invariant.
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 *         Returns #CPL_FILE_FAILURE when storage is full or exceeds quota.
 */
int32_t CPL_file_setPosition(int32_t handle, int32_t value, ASYNC_notifier notifier);

#ifdef  JBED_CDCA
/**
 * Flushes the write buffers of an opened file.
 *
 * This function is called when the file in opened with the mode flag
 * #CPL_FILE_MODE_WRITE or #CPL_FILE_MODE_WRITE_SYNC. See \ref CPL_file_open.
 *
 * This function may be a no-op, if the port opens the file in a mode where
 * data written to the file is not buffered.
 *
 * @param handle File handle as returned by CPL_file_open().
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_flush_buffers(int32_t handle, ASYNC_notifier notifier);

/**
 * Commit a file to the persistent storage.
 * Once this returns all data that has been written to the file must be stored
 * persistently. The function is always called immediately after calling
 * CPL_file_flush_buffers.
 *
 * This function is only called when the file in opened with the mode flag
 * #CPL_FILE_MODE_WRITE_SYNC. See \ref CPL_file_open.
 *
 * If sync style operations are supported on the target platform,
 * this function should be implemented using those operations.
 *
 * This function may be a no-op, if the port opens the file in a mode where
 * data written to the file is immediately committed to persistent storage.
 *
 * @param handle File handle as returned by CPL_file_open().
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_sync(int32_t handle, ASYNC_notifier notifier);

#else   /* JBED_CDCA */

/**
 * Flushes the write buffers of an opened file.  Once this returns all data that
 * has been written to the file must be stored persistently.  It is usually
 * only called in cases where it is critical that the data is indeed persistent
 * at the given time. It can be implemented using a "sync"-style operation.
 * CPL_FILE_FAILURE is returned if the handle is invalid (e.g., as a
 * consequence of CPL_file_delete, CPL_file_rename, or CPL_file_close).
 *
 * @param handle File handle as returned by CPL_file_open().
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_flush(int32_t handle, ASYNC_notifier notifier);

#endif  /* JBED_CDCA */

/**
 * Called by the native world after the VM has shut down to perform one-time
 * cleanup. The implementation should make a best effort attempt to release any
 * resources held by opened files, e.g. by closing any left-over open files.
 */
void CPL_file_shutdown(void);

#ifdef JBED_EXTENDED_FILE_SUPPORT
/**
 * Creates a directory with the assigned name and full file permissions on
 * the file system. The full path to the new directory must already exist.
 * The directory is created immediately on the actual file system upon
 * invocation of this method.
 *
 * @param name Name of the directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK.
 * See also \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_mkdir( const uint16_t* name,
                        int32_t nameChars,
                        int32_t appId,
                        ASYNC_notifier notifier );

/**
 * The size in bytes occupied by the files included in the directory, or -1 if
 * the directory does not exist or is not accessible.
 *
 * @param name Name of the directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param includeSubDirs If JBNI_FALSE, the size does not need to be a recursive
 *                      sum of all bytes occupied by the files included in
 *                      subdirectories of the directory indicated by name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_FAILURE or the size in bytes.
 */
int64_t CPL_file_directorySize64( const uint16_t* name,
                                  int32_t nameChars,
                                  bool_t includeSubDirs,
                                  int32_t appId,
                                  ASYNC_notifier notifier );

/**
 * Set the hidden attribute of this file or directory.
 *
 * The attribute is applied to the file on the actual file system immediately upon
 * invocation of this method if the file system and platform support it. If the file
 * system doesn't support a hidden attribute, this method is ignored and
 * CPL_file_isHidden always returns CPL_FILE_FAILURE.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param hidden JBNI_TRUE if the hidden flag has to be set for this file.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_setHidden( const uint16_t* name,
                            int32_t nameChars,
                            bool_t hidden,
                            int32_t appId,
                            ASYNC_notifier notifier );
/**
 * Set the readable attribute of this file.
 *
 * The readable attribute for the file on the actual file system is set
 * immediately upon invocation of this method. If the file system doesn't
 * support a settable read attribute, this method is ignored and
 * CPL_file_canRead() always returns CPL_FILE_SUCCESS.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param readable JBNI_TRUE if the readable flag has to be set for this file.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_setReadable( const uint16_t* name,
                              int32_t nameChars,
                              bool_t readable,
                              int32_t appId,
                              ASYNC_notifier notifier );

/**
 * Set the writable attribute of this file.
 *
 * The writable attribute for the file on the actual file system is set
 * immediately upon invocation of this method. If the file system doesn't
 * support a settable write attribute, this method is ignored and
 * CPL_file_canWrite() always returns CPL_FILE_SUCCESS.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param writable JBNI_TRUE if the writable flag has to be set for this file.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_setWritable( const uint16_t* name,
                              int32_t nameChars,
                              bool_t writable,
                              int32_t appId,
                              ASYNC_notifier notifier );

/**
 * Checks to see if the given file is hidden.
 *
 * If hidden files are not supported on the referenced file system, this method always
 * returns CPL_FILE_FAILURE.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_isHidden( const uint16_t* name,
                           int32_t nameChars,
                           int32_t appId,
                           ASYNC_notifier notifier );

/**
 * Checks to see if the given file is readable.
 *
 * If the file system doesn't support a settable read attribute, this method always
 * returns CPL_FILE_SUCCESS.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_canRead( const uint16_t* name,
                          int32_t nameChars,
                          int32_t appId,
                          ASYNC_notifier notifier );

/**
 * Checks to see if the given file is writable.
 *
 * If the file system doesn't support a settable read attribute, this method always
 * returns CPL_FILE_SUCCESS.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_canWrite( const uint16_t* name,
                           int32_t nameChars,
                           int32_t appId,
                           ASYNC_notifier notifier );

/**
 * Returns the time the given file was last modified.
 *
 * The time is measured in milliseconds since the epoch (00:00:00 GMT, January 1,
 * 1970).
 * #CPL_FILE_FAILURE is returned if an I/O error occurs.
 * If modification date is not supported by the underlying platform and/or
 * file system, then zero is returned.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_FAILURE, zero or the time in
 *         milliseconds.
 */
int64_t CPL_file_lastModified( const uint16_t* name,
                               int32_t nameChars,
                               int32_t appId,
                               ASYNC_notifier notifier );

/**
 * Checks the validity of filename.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param type The type of the name. One of #CPL_FILE_ISREG, #CPL_FILE_ISDIR or #CPL_FILE_ISFILTER.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_validFilename( const uint16_t* name,
                                int32_t nameChars,
                                int32_t type,
                                int32_t appId,
                                ASYNC_notifier notifier );

/**
 * Returns the access mode granted to this file or directory.
 *
 * This function may be used to restrict access to files that are deemed
 * as private or sensitive. Restrictions are intended to protect the device's
 * and other users' files and data from both malicious and unintentional access.
 *
 * Access to RMS databases managed by the VM have to be restricted by this
 * function.
 *
 * Access to files and directories that are private to another application, files
 * and directories that are private to a different user than the current user,
 * system configuration files, and device and OS specific files and directories
 * may also be restricted.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return Any combination of the #CPL_FILE_PERMISSION_READ and
 * #CPL_FILE_PERMISSION_WRITE flags.
 */
int32_t CPL_file_permission( const uint16_t* name,
                             int32_t nameChars,
                             int32_t appId,
                             ASYNC_notifier notifier );

/**
 * Sets the modification time of a filename.
 *
 * The time is measured in milliseconds since the epoch (00:00:00 GMT, January 1,
 * 1970).
 * If modification date is not supported by the underlying platform and/or
 * file system, then #CPL_FILE_FAILURE is returned.
 *
 * @param name Name of the file or directory, UCS-2 encoded.
 * @param nameChars Number of characters encoded in name.
 * @param time New modification time.
 * @param appId The \ref cpl_file_appid "application-id".
 * @param notifier If not NULL, notifier that MUST be notified when the
 * asynchronous operation returns #CPL_FILE_WOULDBLOCK. See also
 * \ref cpl_file_async.
 * @return #CPL_FILE_WOULDBLOCK, #CPL_FILE_SUCCESS, #CPL_FILE_FAILURE.
 */
int32_t CPL_file_setLastModified( const uint16_t* name,
                                  int32_t nameChars,
                                  int64_t time,
                                  int32_t appId,
                                  ASYNC_notifier notifier );
#endif /* JBED_EXTENDED_FILE_SUPPORT */

#if defined(__cplusplus)
}
#endif

#endif /* __CPL_FILE_H__ */
