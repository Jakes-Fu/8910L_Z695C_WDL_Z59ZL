/*
 * Copyright 2004 Aplix Corporation. All rights reserved.
 */
/***************************************************************************
 * FILE: kjava_sys_fs.h
 *
 * DESCRIPTION:
 *      The module is one of KJava interface modules. It is for java protected
 *      file system handling.
 *
 * REVISION:
 *      2006/02/07,    yih,           seperate different level ksi
 *
 ***************************************************************************/

#ifndef KJAVA_ADV_FS_HEADER
#define KJAVA_ADV_FS_HEADER

/**
 * @file kjava_sys_fs_adv.h
 * File System KSI
 */

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup file_system_ksi
 *  @{
 */

#include "kjava_sys_fs.h"

/** 
 * @addtogroup kjava_sys_fs_advance Advance File KSI
 * @{
 *
 *  "Advance File KSI" need not be implemented in common
 *  JBlendia product. All these KSI are only be called when
 *  JSR75 file connection is supported.
 */

/** \brief A constant indicating that a file system root has been added to the device. */
#define KJAVA_FILE_ROOT_ADDED        (0)
/** \brief A constant indicating that a file system root has been removed to the device. */
#define KJAVA_FILE_ROOT_REMOVED      (1)

/** \brief Data type for directory handle */
typedef const void* KJavaDirHandle;

/** \brief Function type for root change notification 
 *
 *  \param type
 *         - #KJAVA_FILE_ROOT_ADDED : when add new <i>root</i>.
 *         - #KJAVA_FILE_ROOT_REMOVED : when remove exist <i>root</i>.
 *         .
 *  \return One of the follows:
 *         - #KJAVA_FILE_ERR_NONE
 *         - #KJAVA_FILE_ERR_ROOT_FULL
 *         - #KJAVA_FILE_ERR_ROOT_ALREADY_EXIST
 *         - #KJAVA_FILE_ERR_ROOT_TOO_LONG
 *         - #KJAVA_FILE_ERR_ROOT_NOT_EXIST
 *         - #KJAVA_FILE_ERR_IO_ERR
 *         .
 */
typedef int (*KJavaFsRootChange)(KJavaPathMappingEntry *root, int type );


/**
 * \brief Flushes cached data of the opened file.
 * @param hFile
 *        The file handle.
 *        The value returned by kjava_fs_open() is designated.
 * @param taskId
 *        The taskId should be passed to asynchronous callback function assigned
 *        in #kjava_fs_init(). Ignore this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 * \return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_flush(KJavaFileHandle hFile, signed long taskId );

/** 
 *  \brief Change one file's attribute.
 *
 *  \param filename
 *         A pointer to the string containing the file name. The file
 *         could also be a directory.
 *  \param attr
 *         Must be the combination of: 
 *             - #KJAVA_FILE_ATTR_READABLE
 *             - #KJAVA_FILE_ATTR_WRITABLE
 *             - #KJAVA_FILE_ATTR_HIDDEN
 *             .
 * \return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 *
 *
 * When device does not support readable, writable or hidden, the <i>attr</i>
 * that indicate that attribute should be ignored.
 * Please refer to \ref page_file_system_jsr75 for details.
 */ 
int kjava_fs_setAttr( const char* filename, unsigned int attr );

/**
 * \brief Changes the size of a file indicated by file name
 * @param fileName
 *        A pointer to the string containing the file name.
 * @param size
 *        The 64bits long variable for the truncate size.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 *
 * If <i>fileName</i> does not exist, return #KJAVA_FILE_ERR_NOT_EXIST.
 */
int kjava_fs_truncate2(const char *fileName, KJavaLongSize  *size );

/**
 * \brief Opens a directory.
 * @param dirName
 *        A pointer to the directory name string.
 * @param err
 *        A pointer to the buffer to contain one of the error code:
 *        - #KJAVA_FILE_ERR_NONE
 *          No error, the return value must not be NULL.
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *          Indicate <i>dirName</i> does not already exist.
 *        - #KJAVA_FILE_ERR_IO_ERR
 *          Indicate IO error. 
 *        .
 * @return 
 *        Return the directory handle if the operation succeeds; 
 *        return NULL if it fails.
 *
 * When this function succeeds, a value indicating the position 
 * of current directory entry must index the first firectory entry. 
 * This value is called as "directory entry index" in this specification.
 */
KJavaDirHandle kjava_fs_opendir( const char* dirName, int *err );

/**
 * \brief  Reads a directory entry.
 * @param hDir
 *        The directory handle.
 *        The value returned by #kjava_fs_open() is designated.
 *
 * @param filter
 *        The filter string, which can contain a wildcard character "*". 
 *        The string passed in the filter parameter of the FileConnection class 
 *        list(java.lang.String filter, boolean includeHidden) method is passed 
 *        in this parameter. When a Java application calls the list() method, 
 *        "*" is passed.
 * @param showHidden 
 *        When value is 1, read all items inside directory include hidden item.
 *        Otherwise ignore the hidden item.
 * @param err
 *        The pointer to the buffer to get the error code from file system.
 *        Must assign one of the follows if return NULL:
 *        - #KJAVA_FILE_ERR_END_OF_DIRECTORY
 *          Indicates the end of directory is reached.
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 * @return 
 *        A pointer to the read directory entry that corresponds 
 *        to the current directory entry index.
 *
 * Return the directory entry information that corresponds to the currect 
 * directory entry index and proceed the index by 1.
 * If the index has already reached the end of directory, return NULL.
 *
 * If the function succeeds, the return value must remain valid until 
 * next time this function is called or the directory is closed by 
 * #kjava_fs_closedir().
 *
 * When device does not support hidden, the <i>showHidden</i>
 * should be ignored.
 * Please refer to \ref page_file_system_jsr75 for details.
 */
const char* kjava_fs_readdir(KJavaDirHandle hDir, const char* filter, int showHidden, int *err);

/**
 * \brief Closes a directory
 * @param hDir
 *        The directory handle of the open directory.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_closedir(KJavaDirHandle hDir);

/**
 * \brief Creates a new directory
 * @param dirName
 *        A pointer to the string holding the name of the direcotry to be created.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        - #KJAVA_FILE_ERR_ALREADY_EXIST
 *        .
 */
int kjava_fs_mkdir(const char* dirName);

/**
 * \brief Removes an existing directory
 * @param dirName
 *        A pointer to the string holding the name of the direcotry to be removed.
 * @return 
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *        - #KJAVA_FILE_ERR_DIRECTORY_NOT_EMPTY
 *        .
 */
int kjava_fs_rmdir(const char* dirName);

/**
 * \brief Calculate the total size of a directory
 * @param dirName
 *        A pointer to the string holding the name of the direcotry to be calculated.
 * @param includeSubDirs
 *        0 for not to include subdirectories size, 1 for include all subdirectories size.
 * @param size
 *        The 64bits long variable for the total directory size.
 * @param taskId
 *        The taskId should be passed to asynchronous callback function assigned
 *        in #kjava_fs_init(). Ignore this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 * @return 
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *        - #KJAVA_FILE_ERR_ASYNC_FUNCTION
 *        .
 */
 int kjava_fs_sizedir( const char*     dirName, 
                       int             includeSubDirs,
                       KJavaLongSize  *size,
                       signed long     taskId );

/**
 *  \brief Set and cancel the callback function for notifying the change of the root path(s).
 *
 *  \param rootCallback
 *         When this parameter is non-NULL function pointer, device should invoke that function
 *         pointer to let JVM knows the change of the root directory. When this parameter is NULL,
 *         the device should cancel the previous root-notification function pointer and stop
 *         to listen the changing of the root directories.
 *
 */
int kjava_fs_setRootChangeNotification( KJavaFsRootChange rootCallback );

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_ADV_FS_HEADER */
