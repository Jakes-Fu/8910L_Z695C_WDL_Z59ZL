/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */


#ifdef __cplusplus
extern "C" {
#endif

#include "os_api.h"
#include "mmifmm_export.h"
#include "mmk_app.h"
//#include "mmiset.h"
#include "mmiset_export.h"

#include "javacall_dir.h"
#include "javacall_logging.h"
#include "javacall_memory.h"

#define APPDB_DIR L"appdb"
#define CONFIG_DIR L"lib"

#ifdef WIN32
#define JAVAHOME_DIR L"E:\\javahome"
javacall_utf16 *external_dirs[] = {
    L"D:\\System\\java_installed", L"E:\\System\\java_installed"
};
#else
#ifdef SC6600L_ARM7
#define JAVAHOME_DIR L"E:\\javahome"
javacall_utf16 *external_dirs[] = {
    L"E:\\java", L"!:"
};
#else
#if 1 // USE_HIDDEN_DISK
#define JAVAHOME_DIR L"C:\\System\\javahome"
javacall_utf16 *external_dirs[] = {
    L"D:\\System\\java_installed", L"E:\\System\\java_installed"
};
#else
#define JAVAHOME_DIR L"D:\\javahome"
javacall_utf16 *external_dirs[] = {
    L"D:\\java", L"E:\\java"
};
#endif
#endif
#endif

typedef struct
{
    MMIFILE_HANDLE  tHandle;      // Directory handler
    javacall_utf16 fileName[JAVACALL_MAX_FILE_NAME_LENGTH+1];          // First file name
    int       fileNameLen;              // First file name length
    javacall_bool   firstFileFound;        // To tag whether the file name found from the first call to system API findNext
}_T_DIR_HANDLE;

/**
 *  Returns file separator character used by the underlying file system
 * (usually this function will return '\\';)
 * @return 16-bit Unicode encoded file separator
 */
javacall_utf16 javacall_get_file_separator(void) {
    return (javacall_utf16)L'\\';
}

/**
 * returns a handle to a file list. This handle can be used in
 * subsequent calls to javacall_dir_get_next() to iterate through
 * the file list and get the names of files that match the given string.
 * 
 * @param path the name of a directory, but it can be a
 *             partial file name
 * @param pathLen length of directory name or
 *        JAVACALL_UNKNOWN_LENGTH, which may be used for null terminated string 
 * @return pointer to an opaque filelist structure, that can be used in
 *         javacall_dir_get_next() and javacall_dir_close
 *         NULL returned on error, for example if root directory of the
 *         input 'string' cannot be found.
 */
javacall_handle javacall_dir_open(const javacall_utf16* path, int pathLen) {
    _T_DIR_HANDLE *pHandler = NULL;
    MMIFILE_HANDLE hFind;
    MMIFILE_FIND_DATA_T findData;
    javacall_utf16 wOsPath[JAVACALL_MAX_FILE_NAME_LENGTH+1]={0};

    if (pathLen == JAVACALL_UNKNOWN_LENGTH) {
        pathLen = MMIAPICOM_Wstrlen(path);
    }
    if( pathLen > JAVACALL_MAX_FILE_NAME_LENGTH ) {
        return NULL;
    }
    
    SCI_MEMCPY(wOsPath, path, pathLen * sizeof(javacall_utf16));/*lint !e737 */

    if(pathLen>2)
    {
        if (wOsPath[pathLen - 2] != '/' || wOsPath[pathLen - 2] != '\\' 
        || wOsPath[pathLen - 1] != '*') 
        {
                if (wOsPath[pathLen - 1] == L'/' || wOsPath[pathLen - 1] == L'\\') {
                    wOsPath[pathLen++] = '*';
                } else {
                    wOsPath[pathLen++] = javacall_get_file_separator();
                    wOsPath[pathLen++] = '*';
                }
        }
    }
    wOsPath[pathLen] = '\0';

    hFind = MMIAPIFMM_FindFirstFile(wOsPath, pathLen, &findData);
    if (hFind == SFS_INVALID_HANDLE) {
        javacall_printf("[javacall_dir_open], find first fail\n");
        return NULL;
    }

    pHandler = javacall_malloc(sizeof(_T_DIR_HANDLE));
    if(pHandler == NULL) {
        javacall_printf("[javacall_dir_open], out of memory, momory for dirHandler cannot allocated.\n");
        return NULL;
    }

    MMIAPICOM_Wstrcpy(pHandler->fileName, findData.name);
    pHandler->fileNameLen= MMIAPICOM_Wstrlen(pHandler->fileName);
    pHandler->tHandle = hFind;
    pHandler->firstFileFound = JAVACALL_TRUE;
    return pHandler;
}

/**
 * closes the specified file list. This handle will no longer be
 * associated with this file list.
 * @param handle pointer to opaque filelist struct returned by
 *               javacall_dir_open 
 */
void javacall_dir_close(javacall_handle handle) {
    _T_DIR_HANDLE *pHandle;

    if(NULL == handle)	{
        javacall_printf("[javacall_dir_close] >> input handle is null\n");
        return;
    }

    pHandle = (_T_DIR_HANDLE *)handle;
    MMIAPIFMM_FindClose(pHandle->tHandle);

    javacall_free(pHandle);
}

/**
 * return the next filename in directory path (UNICODE format)
 * The order is defined by the underlying file system.
 * 
 * On success, the resulting file will be copied to user supplied buffer.
 * The filename returned will omit the file's path
 * 
 * @param handle pointer to filelist struct returned by javacall_dir_open
 * @param outFilenameLength will be filled with number of chars written 
 * 
 * 
 * @return pointer to UNICODE string for next file on success, 0 otherwise
 * returned param is a pointer to platform specific memory block
 * platform MUST BE responsible for allocating and freeing it.
 */
javacall_utf16* javacall_dir_get_next(javacall_handle handle, int* /*OUT*/ outFilenameLength) {
    _T_DIR_HANDLE *pHandle;

    if(NULL == handle)	{
        javacall_printf("[javacall_dir_get_next] >> handle is null");
        return NULL;
    }

    pHandle = (_T_DIR_HANDLE *)handle;
    if (JAVACALL_TRUE == pHandle->firstFileFound) {
        pHandle->firstFileFound = JAVACALL_FALSE;
        *outFilenameLength = pHandle->fileNameLen;
        return (javacall_utf16 *)pHandle->fileName;
    } else {
        MMIFILE_ERROR_E  findResult;
        MMIFILE_FIND_DATA_T findData;

        findResult = MMIAPIFMM_FindNextFile(pHandle->tHandle, &findData);

        if(SFS_NO_ERROR != findResult) {
            javacall_printf("[javacall_dir_get_next] >> return fail from MMIAPIFMM_FindNextFile\n");
            return NULL;
        } else {
            MMIAPICOM_Wstrcpy(pHandle->fileName, findData.name);
            *outFilenameLength = MMIAPICOM_Wstrlen(pHandle->fileName);
            return (javacall_utf16 *)pHandle->fileName; 
        }
    }
}
    
/**
 * Checks the size of free space in storage. 
 * @return size of free space
 */
javacall_int64 javacall_dir_get_free_space_for_java(int storageId) {
    uint32 free_high_space;
    uint32 free_low_space;
    BOOLEAN bRet;
    javacall_int64 retValue;

    if (storageId < 2) {
        bRet = MMIAPIFMM_GetDeviceFreeSpace(
            MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK),
            MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK),
            &free_high_space,
            &free_low_space);
    } else {
        bRet = MMIAPIFMM_GetDeviceFreeSpace(
            MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
            MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
            &free_high_space,
            &free_low_space);
    }
    if (bRet == FALSE) {
        javacall_printf("[javacall_dir_get_free_space_for_java] >> Get Device Free Space return fail\n");
        return 0;
    } else {
        retValue = free_high_space;
        retValue << 32;
        retValue += free_low_space;/*lint !e737 */
        return retValue;
    }
}
    
javacall_result helper_dir_get_home_path(javacall_utf16* /* OUT */ rootPath, 
                                                    int* /* IN | OUT */ rootPathLen) {
    MMIAPICOM_Wstrcpy(rootPath, JAVAHOME_DIR); /* java-home dir */
    *rootPathLen = MMIAPICOM_Wstrlen(rootPath);

    return JAVACALL_OK;
}

javacall_result javacall_dir_get_external_path(javacall_utf16*** /* OUT */ rootPaths,    /*lint !e765*/
                                               int* /* IN | OUT */ rootPathLen) {
    *rootPaths = external_dirs;
    *rootPathLen = sizeof(external_dirs) / sizeof(javacall_utf16*);
    javacall_printf("javacall_dir_get_external_path %d", rootPathLen);
    return JAVACALL_OK;
}

/**
 * Returns the root path of java's application directory.
 * 
 * @param rootPath returned value: pointer to unicode buffer, allocated 
 *        by the VM, to be filled with the root path.
 * @param rootPathLen IN  : lenght of max rootPath buffer
 *                    OUT : lenght of set rootPath
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully
 *         <tt>JAVACALL_FAIL</tt> if an error occured
 */
javacall_result javacall_dir_get_root_path(javacall_utf16* /* OUT */ rootPath,
                                           int* /* IN | OUT */ rootPathLen) {
    javacall_utf16 filesep[2];
    javacall_result res;
    int len;

    filesep[0] = javacall_get_file_separator();
    filesep[1] = 0;
    if (rootPathLen == NULL) {
        return JAVACALL_FAIL;
    }
    
    len = *rootPathLen;
    res = helper_dir_get_home_path(rootPath, &len);

    if (res != JAVACALL_OK) {
        return res;
    }

    if (len + (int)MMIAPICOM_Wstrlen(APPDB_DIR) >= *rootPathLen) {
        return JAVACALL_FAIL;
    }

    MMIAPICOM_Wstrcat(rootPath, filesep);
    MMIAPICOM_Wstrcat(rootPath, APPDB_DIR);
    *rootPathLen = MMIAPICOM_Wstrlen(rootPath);

    return JAVACALL_OK;
}  

/**
 * Returns the root path of java's configuration directory.
 * 
 * @param configPath returned value: pointer to unicode buffer, allocated 
 *        by the VM, to be filled with the root path.
 * @param configPathLen IN  : lenght of max rootPath buffer
 *                    OUT : lenght of set rootPath
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully
 *         <tt>JAVACALL_FAIL</tt> if an error occured
 */
javacall_result javacall_dir_get_config_path(javacall_utf16* /* OUT */ configPath,
                                           int* /*IN | OUT*/ configPathLen) {
    javacall_utf16 filesep[2];
    javacall_result res;
    int len;

    filesep[0] = javacall_get_file_separator();
    filesep[1] = 0;

    if (configPathLen == NULL) {
        return JAVACALL_FAIL;
    }
    
    len = *configPathLen;
    res = helper_dir_get_home_path(configPath, &len);

    if (res != JAVACALL_OK) {
        return res;
    }

    if (len + (int)MMIAPICOM_Wstrlen(CONFIG_DIR) >= *configPathLen) {
        return JAVACALL_FAIL;
    }

    MMIAPICOM_Wstrcat(configPath, filesep);
    MMIAPICOM_Wstrcat(configPath, CONFIG_DIR);
    *configPathLen = MMIAPICOM_Wstrlen(configPath);

    return JAVACALL_OK;
}

/**
 * Check if the given path is located on secure storage
 * The function should return JAVACALL_TRUE only in the given path
 * is located on non-removable storage, and cannot be accessed by the 
 * user or overwritten by unsecure applications.
 * @return <tt>JAVACALL_TRUE</tt> if the given path is guaranteed to be on 
 *         secure storage
 *         <tt>JAVACALL_FALSE</tt> otherwise
 */
javacall_bool javacall_dir_is_secure_storage(javacall_utf16* classPath, int pathLen) {
    return JAVACALL_FALSE;
}

void javacall_get_current_theme(javacall_utf16* name)
{
    int theme = MMIAPISET_GetCurrentThemeType();

    if(theme == MMISET_THEME_STATIC_TYPE) 
    {
        MMIAPICOM_Wstrcpy(name, L"static\\");
        return;
    } else {
        MMIAPICOM_Wstrcpy(name, L"default\\");
        return;
    }
}
#ifdef __cplusplus
}
#endif

