/*
 * Copyright 2004 Aplix Corporation. All rights reserved.
 */
/***************************************************************************
 * FILE: kjava_sys_fs_dev.h
 *
 * DESCRIPTION:
 *      The module is one of KJava interface modules. It is for device's protected 
 *      file system handling.
 *
 * UPDATED DATE:
 *      2006/01/28
 *
 * REVISION:
 *                 yih
 *
 ***************************************************************************/
 
 
#ifndef KJAVA_DRM_FS_HEADER
#define KJAVA_DRM_FS_HEADER

/**
 * @file kjava_sys_fs_dev.h
 * File System KSI
 */

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup file_system_ksi
 *  @{
 */
 
/*
 *  Interfaces listed in the group are used for JAD/JAR file IO of the
 *  #AJMS_APPLICATION_TYPE_3 application. In other words, if device's AMS
 *  does not support such Java application type. All the KSI should not be
 *  implemented. Please refer to 
 *  JBlendia document for the detail of #AJMS_APPLICATION_TYPE_3 application.
 */

/** 
 * @addtogroup kjava_dev_fs_io Device Protected File KSI
 * @{
 */

/** \brief KSI device protected file handle type */
typedef const void* KJavaDevFileHandle;

/** \brief Error code: none */
#define KJAVA_DEVFILE_ERR_NONE          (0)

/** \brief Error code: File does not exist */
#define KJAVA_DEVFILE_ERR_NOT_EXIST     (-2)

/** \brief Error code: IO Error */
#define KJAVA_DEVFILE_ERR_IO_ERR        (-3)

/** \brief Error code: reach end of file */
#define KJAVA_DEVFILE_ERR_END_OF_FILE   (-6)

/**
 * \brief Open a device protected file
 *
 *        The file opened by this KSI is only for read. In order words, 
 *        AJMS assumes the file exist and could be read.
 *
 * \param fileName
 *        A pointer to the string containing the name of the file to be opened.
 * \param err
 *        A pointer to the buffer to contain one of the error code:
 *        - #KJAVA_DEVFILE_ERR_NONE
 *          No error, the return value must not be NULL.
 *        - #KJAVA_DEVFILE_ERR_NOT_EXIST
 *          Indicate the file does not exist or could not be acceed right now.
 *        . 
 * @return 
 *        Return the file handle if the operation succeeds; <br>
 *        return NULL if it fails.
 */
KJavaDevFileHandle kjava_dfs_open(const char *fileName, int* err );

/**
 * \brief Closes the opened, device-protected file handle.
 * @param hFile
 *        The file handle.
 *        The value returned by #kjava_dfs_open() is designated.
 * @return
 *        - #KJAVA_DEVFILE_ERR_NONE
 *        - #KJAVA_DEVFILE_ERR_IO_ERR
 *        .
 */
int kjava_dfs_close(KJavaDevFileHandle hFile);

/** \brief Read the file with the offset and size.
 *  @param ptr
 *         A pointer to the buffer to store read data
 *  @param offset
 *         The position to start reading. It is designated as the offset (in bytes) from
 *         the head.
 *  @param size
 *         The size (in bytes) to be read.
 *  @param hFile
 *        The file handle.
 *        The value returned by #kjava_dfs_open() is designated.
 *  @param read
 *         If the read operation succeeds, return the number of bytes (0 or more)
 *         actually read.
 *  @return 
 * @return 
 *        - #KJAVA_DEVFILE_ERR_NONE
 *        - #KJAVA_DEVFILE_ERR_IO_ERR
 *        - #KJAVA_DEVFILE_ERR_END_OF_FILE
 *        .
 *
 *  This is the type of the callback function to read JAD and JAR.
 *
 *  If the size of read data is less than the requested size, the function retries
 *  reading the remaining data.
 *  If the size of read data returned by this function is greater than the requested
 *  size, AJMS treats this result as an error.
 *  If the read data exceeds the buffer size, the behavior of AJMS is not guaranteed.
 */
int kjava_dfs_read(void* ptr, 
                  signed long offset,
                  signed long size, 
                  signed long* read,
                  KJavaDevFileHandle hFile);

/**
 * \brief Gets information about a file indicated by file name.
 *
 * This function is only to query the file size as well as the 
 * file existence. The <i>lastModify</i> field inside <i>stat</i>
 * will not be used anywhere.
 *
 * @param fileName
 *        A pointer to the string containing the file name.
 * @param size
 *        A pointer to the buffer get the file size.
 *        JBlendia assume the Jar/Jad size could not more than
 *        0x7FFFFFFF bytes.
 * @return
 *        - #KJAVA_DEVFILE_ERR_NONE
 *        - #KJAVA_DEVFILE_ERR_NOT_EXIST
 *        - #KJAVA_DEVFILE_ERR_IO_ERR
 *        .
 */
int kjava_dfs_stat2(const char *fileName, signed long* size );

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_FS_HEADER */
