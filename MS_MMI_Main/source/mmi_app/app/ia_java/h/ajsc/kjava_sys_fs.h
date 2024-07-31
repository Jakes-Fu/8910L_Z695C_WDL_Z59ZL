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
 * UPDATED DATE:
 *      2004/11/04
 *
 * REVISION:
 *      2004/10/23,    Jack Chung,    Adopted from MMM code.
 *      2004/11/04,    Jack Chung,    Revised documentation.
 *      2005/12/29,    yih,           clean useless code/KSI, revised docs
 *
 ***************************************************************************/

#ifndef KJAVA_SYS_FS_HEADER
#define KJAVA_SYS_FS_HEADER

/**
 * @file kjava_sys_fs.h
 * File System KSI
 */
#include "sci_types.h"
#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup file_system_ksi
 *  @{
 */

/** 
 * @addtogroup kjava_sys_fs_io Common File KSI
 * @{
 */


/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/
 
/** \brief Root Attribute: Public */
#define KJAVA_FILE_ROOT_PUBLIC            (0x01)
/** \brief Root Attribute: Private, which cannot be accessed by Java application */
#define KJAVA_FILE_ROOT_PRIVATE           (0x02)

/** \brief File Attribute: Readable */
#define KJAVA_FILE_ATTR_READABLE           (0x04)
/** \brief File Attribute: Writable */
#define KJAVA_FILE_ATTR_WRITABLE           (0x08) 
/** \brief File Attribute: Hidden from user */
#define KJAVA_FILE_ATTR_HIDDEN             (0x01)
/** \brief File Attribute: Target is a directory */
#define KJAVA_FILE_ATTR_DIRECTORY          (0x02)

/** \brief File open mode: Read operation enabled */
#define KJAVA_FILE_OPEN_READ               (0x01)
/** \brief File open mode: Write operation enabled */
#define KJAVA_FILE_OPEN_WRITE              (0x02)
/** \brief File open mode: Read and write operation enabled */
#define KJAVA_FILE_OPEN_RW                 (0x04)
/** \brief File open mode: Create a file, should fail if file already exist */
#define KJAVA_FILE_OPEN_CREATE             (0x10)
/** \brief File open mode: Open a file, should fail if file does not exist. */
#define KJAVA_FILE_OPEN_EXCL               (0x40)

/** \brief Move file position to an absolute position. */
#define KJAVA_FILE_SEEK_SET                (0)
/** \brief Move file position to a position relative to the current file position. */
#define KJAVA_FILE_SEEK_CUR                (1)
/** \brief Move file position to a position relative to the end of file.*/
#define KJAVA_FILE_SEEK_END                (2)

/** \brief Error code: none */
#define KJAVA_FILE_ERR_NONE                (0)
/** \brief Error code: File already exist */
#define KJAVA_FILE_ERR_ALREADY_EXIST       (-1)
/** \brief Error code: File does not exist */
#define KJAVA_FILE_ERR_NOT_EXIST           (-2)
/** \brief Error code: IO Error */
#define KJAVA_FILE_ERR_IO_ERR              (-3)
/** \brief Error code: Directory is not empty */
#define KJAVA_FILE_ERR_DIRECTORY_NOT_EMPTY (-4)
/** \brief Error code: target is not a directory */
#define KJAVA_FILE_ERR_NOT_DIRECTORY       (-5)
/** \brief Error code: reach end of file */
#define KJAVA_FILE_ERR_END_OF_FILE         (-6)
/** \brief Error code: invalid position */
#define KJAVA_FILE_ERR_INVALID_POSITION    (-7)
/** \brief Error code: reach end of directory */
#define KJAVA_FILE_ERR_END_OF_DIRECTORY    (-8)
/** \brief Error code: the result will be passed by asynchronous callback */
#define KJAVA_FILE_ERR_ASYNC_FUNCTION      (-9)
/** \brief Error code: no free root entry could be set */
#define KJAVA_FILE_ERR_ROOT_FULL           (-20)
/** \brief Error code: root already exist */
#define KJAVA_FILE_ERR_ROOT_ALREADY_EXIST  (-21)
/** \brief Error code: root does not exist */
#define KJAVA_FILE_ERR_ROOT_NOT_EXIST      (-22)
/** \brief Error code: root length too long */
#define KJAVA_FILE_ERR_ROOT_TOO_LONG       (-23)

/** \brief The maximum length of an authority's name */
#define KJAVA_MAX_AUTHORITY_NAME_LENGTH    (32)
/** \brief The maximum length of an authority's path */
#define KJAVA_MAX_AUTHORITY_PATH_LENGTH    (128)
/** \brief The maximum length of the path of a file in the platform. */
#define KJAVA_MAX_PATH_LENGTH              (256)
/** \brief The maximum length of the name of a file in the platform. */
#define KJAVA_MAX_FILE_NAME_LENGTH         (256)



/** \brief KSI file handle type */

typedef const void* KJavaFileHandle;

/** \brief Function type for asynchronous IO callback. 
 *
 *   While #kjava_fs_init() is called, this callback function is passed 
 *   to native for asychronous IO usage.
 *   By default, the asynchronous file IO is disabled and could be
 *   available by special JBlendia library customization request.
 */
typedef int (*KJavaFsNotifyEvent)( signed long taskId, unsigned long* event );

/** @} */

/** 
 * @addtogroup kjava_sys_fs_dir Directories Authetication KSI
 * @{
 */

/** \brief The strucnture type for storing a path mapping */
typedef struct _KJavaPathMappingTag
{
    /** \brief The authority name */
    char    name[KJAVA_MAX_AUTHORITY_NAME_LENGTH];

    /** \brief The real path for the authority.
     *
     * The path must end with the value returned by #kjava_fs_getPathSeparator.
     */
    char    path[KJAVA_MAX_PATH_LENGTH];
    
    /** \brief The root attribute. 
     *
     *  It must be onle on #KJAVA_FILE_ROOT_PUBLIC or #KJAVA_FILE_ROOT_PRIVATE.
     *  For all other value, JVM treats it as #KJAVA_FILE_ROOT_PRIVATE. When
     *  #KJavaFsRootChange() is called, this field will be ignored while
     *  <i>type</i> is #KJAVA_FILE_ROOT_REMOVE.
     */
    int    attr;
}
KJavaPathMappingEntry;

/** @} */

/** 
 * @addtogroup kjava_sys_fs_io
 * @{
 */
/** \brief The strucnture type for storing 64bit long variable. */
typedef struct _KJavaLongSizeTag
{
    /** \brief The high order 32-bit for the long variable. */
    unsigned long high;
    /** \brief The low order 32-bit for the long variable. */
    unsigned long low;
} KJavaLongSize;

/** \brief The strucnture type for storing detailed information about a file. */
typedef struct _KJavaStatTag
{
    /** \brief The file attribute.
     *
     *         If target does not support file connection (JSR75), the value
     *         in <i>mode</i> is ignored by JBlendia. If target supports JSR75,
     *         the <i>mode</i> should be a combination of:
     *             - #KJAVA_FILE_ATTR_READABLE
     *             - #KJAVA_FILE_ATTR_WRITABLE
     *             - #KJAVA_FILE_ATTR_HIDDEN
     *             - #KJAVA_FILE_ATTR_DIRECTORY
     *             .
     */
    int     attr;

    /** \brief The 64 bits long type value for last modified date.
     *
     *         If target does not support file connection (JSR75), the value
     *         in <i>lastModify</i> is ignored by JBlendia. If target 
     *         supports JSR75, the <i>lastModify</i> should be the 
     *         modified date and time of the file or directory. Store as 
     *         elapsed milliseconds from 0:00:00.000, January 1, 1970 (GMT). 
     */
    KJavaLongSize lastModify;
    
    /** \brief The 64 bits long type value for file size.
     *
     *         When target is a directory, this field is ignored and
     *         JBlendia will use #kjava_fs_sizedir() or #kjava_fs_available()
     *         to query the directory size.
     */
    KJavaLongSize size;
    
}
KJavaStat;

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/**
 * \brief Initializes the native file system component.
 *
 * The function needs to:
 *  - prepare path mappings.
 *  - allocate necessary resource.
 *  - create a directory for the MIDlet suites repository if the
 *    directory doesn't exist.
 *
 * \param callback
 *        The callback function to notify the asynchronous IO result. Ignore 
 *        this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 */
void kjava_fs_init( KJavaFsNotifyEvent callback );

/** 
 * \brief Finalizes the native file system component.
 *
 * The function needs to release file system resources allocated after #kjava_fs_init is called.
 */
void kjava_fs_fini(void);

/**
 * \brief Opens a file.
 * @param fileName
 *        A pointer to the string containing the name of the file to be opened.
 * @param mode
 *        The file open mode.
 *        A combination of the following values is designated.
 *        - #KJAVA_FILE_OPEN_READ
 *        - #KJAVA_FILE_OPEN_WRITE
 *        - #KJAVA_FILE_OPEN_RW
 *        - #KJAVA_FILE_OPEN_CREATE
 *        - #KJAVA_FILE_OPEN_EXCL
 *        .
 *        One and only one of #KJAVA_FILE_OPEN_READ, #KJAVA_FILE_OPEN_WRITE or
 *        #KJAVA_FILE_OPEN_RW will be assigned to mode. One and only one of
 *        #KJAVA_FILE_OPEN_CREATE or #KJAVA_FILE_OPEN_EXCL will be assigned to 
 *        mode. In other words, it is not possible to have <i>mode</i> such as:
 *        <pre>
 *        ( KJAVA_FILE_OPEN_READ | KJAVA_FILE_OPEN_RW )
 *        ( KJAVA_FILE_OPEN_CREATE | KJAVA_FILE_OPEN_EXCL )
 *        ( KJAVA_FILE_OPEN_WRITE )
 *        </pre>
 * @param taskId
 *        The taskId should be passed to asynchronous callback function assigned
 *        in #kjava_fs_init(). Ignore this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 * @param err
 *        A pointer to the buffer to contain one of the error code:
 *        - #KJAVA_FILE_ERR_NONE
 *          No error, the return value must not be NULL.
 *        - #KJAVA_FILE_ERR_ALREADY_EXIST
 *          Indicate the open mode is #KJAVA_FILE_OPEN_CREATE but the file already exist.
 *        - #KJAVA_FILE_ERR_IO_ERR
 *          Indicate IO error. 
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *          Indicate the open mode is #KJAVA_FILE_OPEN_EXCL but the file does not exist.
 *        . 
 * @return 
 *        Return the file handle if the operation succeeds; <br>
 *        return NULL if it fails.
 */
KJavaFileHandle kjava_fs_open(const char *fileName, const int mode, signed long taskId, int* err );

/**
 * \brief Closes the opened file handle.
 * @param hFile
 *        The file handle.
 *        The value returned by #kjava_fs_open() is designated.
 * @param taskId
 *        The taskId should be passed to asynchronous callback function assigned
 *        in #kjava_fs_init(). Ignore this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_close(KJavaFileHandle hFile, signed long taskId);

/**
 * \brief Reads from the opened file
 * @param ptr
 *        A pointer to the buffer for putting the data read.
 * @param size
 *        The size of the buffer indicated by <i>ptr</i>.
 * @param read
 *        The actual read bytes in <i>ptr</i>
 * @param taskId
 *        The taskId should be passed to asynchronous callback function assigned
 *        in #kjava_fs_init(). Ignore this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 * @param hFile
 *        The file handle.
 *        The value returned by #kjava_fs_open is designated.
 * @return 
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        - #KJAVA_FILE_ERR_END_OF_FILE
 *        .
 */
int kjava_fs_read(void* ptr, signed long size, signed long* read,
                             KJavaFileHandle hFile, signed long taskId);

/**
 * \brief Writes to the opened file.
 * @param ptr
 *        A pointer to the buffer holding the data to be written.
 * @param size
 *        The size of the data indicated by <i>ptr</i>.
 * @param hFile
 *        The file handle.
 *        The value returned by kjava_fs_open is designated.
 * @param taskId
 *        The taskId should be passed to asynchronous callback function assigned
 *        in #kjava_fs_init(). Ignore this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 * @return 
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 *
 *        The AJSC expects #kjava_fs_write to fully write all the data at once. If the
 *        return value is not equal to <i>size</i>, AJSC will treat as error happened.
 */
int kjava_fs_write(void* ptr, signed long size, KJavaFileHandle hFile, signed long taskId);

/**
 * \brief Moves the file pointer to a designated location
 * @param hFile
 *        The file handle.
 *        The value returned by #kjava_fs_open is designated.
 * @param offset
 *        The 64bits long variable for the file position.
 * @param whence
 *        The starting point for the file pointer move.
 *        One of the following values is designated.
 *        - #KJAVA_FILE_SEEK_SET<br>
 *          <i>offset</i> is the absolute file position (an offset from the
 *          beginning of the file) desired. <i>offset</i> is a positive value.
 *        - #KJAVA_FILE_SEEK_CUR <br>
 *          <i>offset</i> is a position relative to the current file position.
 *          <i>offset</i> can be meaningfully be either positive or
 *          negative.
 *        - #KJAVA_FILE_SEEK_END <br>
 *          <i>offset</i> is a position relative to the end of file. <i>offset</i>
 *          can meaningfully be either positive (to increase the size of the file) or
 *          negative.
 *        .
 * @param taskId
 *        The taskId should be passed to asynchronous callback function assigned
 *        in #kjava_fs_init(). Ignore this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 * @return 
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        - #KJAVA_FILE_ERR_INVALID_POSITION
 *        .
 *
 * This function is used to set the file position for the file indicated by
 * <i>hFile</i>. The value of <i>offset</i> determines the new
 * position, in one of three ways selected by the value of <i>whence</i>.
 */
int kjava_fs_seek(KJavaFileHandle hFile, KJavaLongSize* offset, int whence, signed long taskId);

/**
 * \brief Query the position of the file pointer.
 * @param hFile
 *        The file handle.
 *        The value returned by #kjava_fs_open() is designated.
 * @param offset
 *        The 64bits long variable for the file position.
 * @param taskId
 *        The taskId should be passed to asynchronous callback function assigned
 *        in #kjava_fs_init(). Ignore this field if not support asynchronous file
 *        IO. By default, the asynchronous file IO is disabled and could be
 *        available by special JBlendia library customization request.
 * @return 
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_tell(KJavaFileHandle hFile, KJavaLongSize* offset, signed long taskId);

/**
 * \brief Truncate the size of a file indicated by file handle
 * @param hFile
 *        The file handle.
 *        The value returned by #kjava_fs_open() is designated.
 * @param size
 *        The 64bits long variable for the truncated file size.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_truncate(KJavaFileHandle hFile, KJavaLongSize* size );

/**
 * \brief Removes a file
 * @param fileName
 *        A pointer to the string containing the name of the file to be removed.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_remove(const char* fileName);

/**
 * \brief Renames a file
 * @param oldName
 *        A pointer to the string containing the original file name.
 * @param newName
 *        A pointer to the string containing the new file name.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *          The <i>oldName</i> does not exist.
 *        - #KJAVA_FILE_ERR_ALREADY_EXIST
 *          The <i>newName</i> already exist.
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_rename(const char* oldName, const char* newName);

/**
 * \brief Gets information about a file indicated by file name.
 *
 * If not support JSR-75, file connection, this function 
 * does not need to fill <i>lastModify</i> and <i>attr</i> field inside <i>stat</i>.
 *
 * If support JSR-75, file connection, when file system does not support
 * "last modified date" attribute, the high and low order date should be
 * set to 0xFFFFFFFF. For <i>attr</i> field inside <i>stat</i>, please
 * provide the combination of #KJAVA_FILE_ATTR_READABLE, #KJAVA_FILE_ATTR_WRITABLE
 * #KJAVA_FILE_ATTR_HIDDEN and #KJAVA_FILE_ATTR_DIRECTORY. To detemine
 * if the <i>fileName</i> is a directory is mandatory for JSR75, others 
 * could be ignored if file system does not support such attribute.
 * Please refer to \ref page_file_system_jsr75 for details.
 *
 * @param fileName
 *        A pointer to the string containing the file name.
 * @param stat
 *        A pointer to the buffer for putting the obtained inoformation.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_stat2(const char *fileName, KJavaStat* stat);

/** @} */

/** 
 * @addtogroup kjava_sys_fs_dir
 * @{
 */

/**
 * \brief Gets the path separator character of the native file system.
 * @return 
 *        Return the path separator character.
 */
unsigned char kjava_fs_getPathSeparator(void);

/**
 * \brief Gets the number of path mappings for the platform.
 * @return 
 *        Return the number of path mappings. It must be 1 or more.
 *
 * At least, there is one path mapping for java. The <i>name</i> field
 * of the path mapping for java is "java". AJMS uses the path mapping for
 * java as the repository to store MIDlet-suites. 
 */
int32 kjava_fs_getPathCount(void);

/**
 * \brief Gets a path mapping for the platform.
 * @param index
 *        The index of path mapping. Its value is in the range between
 *        0 and N-1. N is the return value of #kjava_fs_getPathCount.
 * @param map
 *        A pointer to the #KJavaPathMappingEntry structure for putting the path mapping.
 *        The function should fill fields of the designated structure.
 * @return
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 */
int kjava_fs_getPath(int index, KJavaPathMappingEntry* map);

/**
 * \brief Gets the available space for the designated path
 * @param path
 *        A pointer to the string containing the path.
 * @param total
 *        The 64bits long variable for the total size of the path.
 *        This field is not used when file connection is not supported.
 * @param free
 *        The 64bits long variable for the free size of the path.
 * @return 
 *        - #KJAVA_FILE_ERR_NONE
 *        - #KJAVA_FILE_ERR_NOT_EXIST
 *        - #KJAVA_FILE_ERR_IO_ERR
 *        .
 *
 * If <i>path</i> is a file, return #KJAVA_FILE_ERR_NOT_EXIST.
 */
int kjava_fs_available(const char* path, KJavaLongSize* total,
                                         KJavaLongSize* free_size );

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_FS_HEADER */

