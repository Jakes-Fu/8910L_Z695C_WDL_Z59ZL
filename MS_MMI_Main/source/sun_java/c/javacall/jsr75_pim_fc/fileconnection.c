/*
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

/*
 * Stubs implementation for JSR-75 FileConnection API.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include "mmk_app.h"
//#pragma warning( disable : 4005)  // Disable warning messages "macro redefinition"
#endif

#include <javacall_fileconnection.h>
#include "javacall_time.h"
#include "javacall_logging.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmicom_time.h"
#include "javacall_dir.h"
#include "javacall_file.h"
#include "javacall_memory.h"
#include "mmijava_multimedia.h" //jc_convert_fc2native_path() prototype
#define FC_DEBUG //javacall_printf

/**
 * @defgroup JSR75 JSR75 File Connection API
 *
 * The following API definitions are required by JSR-75.
 * These APIs are not required by standard JTWI implementations.
 *
 * @{
 */

/*
 * 
 *             JSR075's FileConnection API
 *            =============================
 * 
 * 
 * The following API definitions are required by JSR-75.
 * This API is not required by standard JTWI implementations.
 * 
 * Mandatory API for JSR-75:
 * - javacall_fileconnection_init()
 * - javacall_fileconnection_finalize()
 * - javacall_fileconnection_set_hidden()
 * - javacall_fileconnection_set_readable()
 * - javacall_fileconnection_set_writable()
 * - javacall_fileconnection_get_illegal_filename_chars()
 * - javacall_fileconnection_get_free_size()
 * - javacall_fileconnection_get_total_size()
 * - javacall_fileconnection_get_mounted_roots()
 * - javacall_fileconnection_get_photos_dir()
 * - javacall_fileconnection_get_videos_dir()
 * - javacall_fileconnection_get_graphics_dir()
 * - javacall_fileconnection_get_tones_dir()
 * - javacall_fileconnection_get_music_dir()
 * - javacall_fileconnection_get_recordings_dir()
 * - javacall_fileconnection_get_private_dir()
 * - javacall_fileconnection_get_localized_mounted_roots()
 * - javacall_fileconnection_get_localized_photos_dir()
 * - javacall_fileconnection_get_localized_videos_dir()
 * - javacall_fileconnection_get_localized_graphics_dir()
 * - javacall_fileconnection_get_localized_tones_dir()
 * - javacall_fileconnection_get_localized_music_dir()
 * - javacall_fileconnection_get_localized_recordings_dir()
 * - javacall_fileconnection_get_localized_private_dir()
 * - javacall_fileconnection_get_path_for_root()
 * - javacall_fileconnection_is_hidden()
 * - javacall_fileconnection_is_readable()
 * - javacall_fileconnection_is_writable()
 * - javacall_fileconnection_get_last_modified()
 * - javacall_fileconnection_is_directory()
 * - javacall_fileconnection_create_dir()
 * - javacall_fileconnection_delete_dir()
 * - javacall_fileconnection_dir_exists()
 * - javacall_fileconnection_rename_dir()
 * - javacall_fileconnection_dir_content_size()
 *
 * Functions specific for CDC-based implementations:
 * - javacall_fileconnection_cache_properties()
 * - javacall_fileconnection_activate_notifications()
 * - javacall_fileconnection_deactivate_notifications()
 */

typedef struct {
    int ready;
    const javacall_utf16* root;
    const javacall_utf16* path;
} jsr75fc_rootVSpath;

extern PUBLIC float MMIAPIACC_GetLocalTimeZone(void);
static const javacall_utf16 root1[] = L"SDCard/";
static const javacall_utf16 root2[] = L"Phone/";
//static const javacall_utf16 path1[] =  L"E:\\";
//static  javacall_utf16 root_str[MMI_DEVICE_NUM][12] = {0, L"Phone/", L"SDCard/"};
//static  javacall_utf16 root_str[MMI_DEVICE_NUM][12] = {0};
static  javacall_utf16 path[MMI_DEVICE_NUM][4] = {0};

static  jsr75fc_rootVSpath jsr75fc_rootVSpath_tbl[MMI_DEVICE_NUM -1] = {0};

#ifdef WIN32
extern const __int64 deltaTime;
#else
extern const long long deltaTime;/*lint !e526*/
#endif
void utf16print(char* prompt, const javacall_utf16_string utf16 )
{
#define MAX_SIZE 256    
    unsigned char tmpchar[MAX_SIZE+1];
    int i;
    
    if(!utf16)
    {
	javacall_printf("%s> NULL", prompt);
	return;
    }
    
    for (i=0; utf16[i] && (i<MAX_SIZE); i++)
	tmpchar[i] = utf16[i];
    tmpchar[i] = 0;
    
    javacall_printf("%s> %s||", prompt, tmpchar);

#undef MAX_SIZE
}

/** 
 * @defgroup jsrMandatoryFileConnection Mandatory FC API
 * @ingroup JSR75
 *
 * @{
 */ 

static void update_root_tbl()
{
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_NUM;
    uint32           index = 0;
    
    for(device_type = MMI_DEVICE_UDISK; device_type < MMI_DEVICE_NUM; device_type++)
    {
        if(MMIAPIFMM_GetDeviceStatus((uint16 *)MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
        {
            jsr75fc_rootVSpath_tbl[index].ready = 1;            
        }
        else
        {
            jsr75fc_rootVSpath_tbl[index].ready = 0;
        }
        MMIAPICOM_Wstrncpy(&path[index][0], MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type));
        path[index][1] = MMIFILE_COLON;
        path[index][2] = MMIFILE_SLASH;
        jsr75fc_rootVSpath_tbl[index].path = path[index];
//        javacall_printf(LOG_STR("path[%d] = %s"),  index,   javacall_wstr2str(path[index]));            

        if(MMI_DEVICE_UDISK == device_type)  
        {
            jsr75fc_rootVSpath_tbl[index].root = root2;         
        }
        else if(MMI_DEVICE_SDCARD == device_type) 
        {
            jsr75fc_rootVSpath_tbl[index].root = root1;         
        }
        else
        {
            jsr75fc_rootVSpath_tbl[index].root = 0;         
        }
	
//        javacall_printf(LOG_STR("index = %d, ready =%d"), index, jsr75fc_rootVSpath_tbl[index].ready);
//        javacall_printf(LOG_STR("root = %s"),javacall_wstr2str(jsr75fc_rootVSpath_tbl[index].root));

       index++;
    }
}
/**
 * Makes all the required initializations for JSR-75 FileConnection.
 *
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> if an error occured or feature is not supported.
 */
javacall_result javacall_fileconnection_init(void) {
    update_root_tbl();

    return JAVACALL_OK;
}

/**
 * Cleans up resources used by FileConnection.
 *
 * @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result javacall_fileconnection_finalize(void) {

    return JAVACALL_OK;
}

/**
 * Sets the HIDDEN attribute for the specified file or directory.
 *
 * @param fileName      name of file or directory.
 * @param value         <tt>JAVACALL_TRUE</tt> to set file as hidden,
 *                      <tt>JAVACALL_FALSE</tt> to set file as not hidden.
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> if an error occured.
 */
javacall_result
javacall_fileconnection_set_hidden(javacall_const_utf16_string fileName,
                                   javacall_bool value) {    
    if((javacall_file_exist(fileName, MMIAPICOM_Wstrlen(fileName)) == JAVACALL_OK) || 
       (javacall_fileconnection_dir_exists(fileName) == JAVACALL_OK))
        return JAVACALL_OK;
    else 
        return JAVACALL_FAIL;

    /* javacall_utf16 tmpPath[MMIFILE_FULL_PATH_MAX_LEN]; */
    /* int len; */
    /* unsigned short attr; */
    /* SFS_ERROR_E ret; */
    /* javacall_utf16 sep = javacall_get_file_separator(); */

    /* if(fileName == NULL) */
    /*     return JAVACALL_FAIL; */

    /* utf16print("javacall_fileconnection_set_hidden", fileName); */

    /* len = MMIAPICOM_Wstrlen(fileName); */
    /* MMIAPICOM_Wstrcpy(tmpPath, fileName); */
    /* if(fileName[len - 1] == sep) { */
    /*     tmpPath[len - 1] = NULL; */
    /* } */

    /* ret = SFS_GetAttr(tmpPath, &attr); */
    /* javacall_printf("javacall_fileconnection_set_hidden %x", attr); */

    /* if (ret == SFS_NO_ERROR) { */
    /*     attr = attr & ~SFS_ATTR_DIR; */
    /*     attr = value? (attr | SFS_ATTR_HIDDEN) : (attr & ~SFS_ATTR_HIDDEN);  */
    /*     ret = SFS_SetAttr(tmpPath, attr | value); */
    /*     if(ret == SFS_NO_ERROR) */
    /*         return JAVACALL_OK; */
    /*     else { */
    /*         javacall_printf("javacall_fileconnection_set_hidden SFS_SetAttr got %d", ret); */
    /*         return JAVACALL_FAIL; */
    /*     } */
    /* } else { */
    /*     javacall_printf("javacall_fileconnection_set_hidden SFS_GetAttr got %d", ret); */
    /*     return JAVACALL_FAIL; */
    /* } */

    /* return JAVACALL_FAIL; */
}

/**
 * Sets the READABLE attribute for the specified file or directory.
 *
 * @param pathName      name of file or directory.
 * @param value         <tt>JAVACALL_TRUE</tt> to set file as readable,
 *                      <tt>JAVACALL_FALSE</tt> to set file as not readable.
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> if an error occured.
 */
javacall_result
javacall_fileconnection_set_readable(javacall_const_utf16_string pathName,
                                     javacall_bool value) {
    if((javacall_file_exist(pathName, MMIAPICOM_Wstrlen(pathName)) == JAVACALL_OK) || 
       (javacall_fileconnection_dir_exists(pathName) == JAVACALL_OK))
        return JAVACALL_OK;
    else 
        return JAVACALL_FAIL;

}

/**
 * Sets the WRITABLE attribute for the specified file or directory.
 *
 * @param pathName      name of file or directory.
 * @param value         <tt>JAVACALL_TRUE</tt> to set file as writable,
 *                      <tt>JAVACALL_FALSE</tt> to set file as not writable.
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> if an error occured.
 */
javacall_result
javacall_fileconnection_set_writable(javacall_const_utf16_string pathName,
                                     javacall_bool value) {    
    if((javacall_file_exist(pathName, MMIAPICOM_Wstrlen(pathName)) == JAVACALL_OK) || 
       (javacall_fileconnection_dir_exists(pathName) == JAVACALL_OK))
        return JAVACALL_OK;
    else 
        return JAVACALL_FAIL;

    /* javacall_utf16 tmpPath[MMIFILE_FULL_PATH_MAX_LEN]; */
    /* int len; */
    /* unsigned short attr; */
    /* SFS_ERROR_E ret; */
    /* javacall_utf16 sep = javacall_get_file_separator(); */

    /* if(pathName == NULL) */
    /*     return JAVACALL_FAIL; */

    /* utf16print("javacall_fileconnection_set_writable", pathName); */

    /* len = MMIAPICOM_Wstrlen(pathName); */
    /* MMIAPICOM_Wstrcpy(tmpPath, pathName); */
    /* if(pathName[len - 1] == sep) { */
    /*     tmpPath[len - 1] = NULL; */
    /* } */

    /* ret = SFS_GetAttr(tmpPath, &attr); */
    /* javacall_printf("javacall_fileconnection_set_writable %x", attr); */

    /* if (ret == SFS_NO_ERROR) { */
    /*     attr = attr & ~SFS_ATTR_DIR; */
    /*     attr = (value==JAVACALL_TRUE)? (attr & ~ SFS_ATTR_RO) : (attr | SFS_ATTR_RO);  */
    /*     ret = SFS_SetAttr(tmpPath, attr | value); */
    /*     if(ret == SFS_NO_ERROR) */
    /*         return JAVACALL_OK; */
    /*     else { */
    /*         javacall_printf("javacall_fileconnection_set_writable SFS_SetAttr got %d", ret); */
    /*         return JAVACALL_FAIL; */
    /*     } */
    /* } else { */
    /*     javacall_printf("javacall_fileconnection_set_writable SFS_GetAttr got %d", ret); */
    /*     return JAVACALL_FAIL; */
    /* } */

    /* return JAVACALL_FAIL; */
}

/**
 * Returns the list of illegal characters in file names. The list must not
 * include '/', but must include native file separator, if it is different
 * from '/' character.
 *
 * @param illegalChars returned value: pointer to string, allocated
 *                     by the VM, to be filled with the characters that are
 *                     not allowed inside file names.
 * @param illegalCharsMaxLen available size, in characters,
 *                              of the buffer provided.
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_illegal_filename_chars(javacall_utf16_string /* OUT */ illegalChars,
                                                   int illegalCharsMaxLen) {
    int i;
    char str[] = "?<>:\"\\|";

    if(illegalCharsMaxLen < 9) {
        FC_DEBUG(LOG_STR("length too small"));
        return JAVACALL_FAIL;
    }

    for(i = 0; i <8; i++) { // all chars, including trailing zero
        illegalChars[i] = (unsigned short) str[i];/*lint !e571*/
    }

    return JAVACALL_OK;
}

/**
 * Determines the free memory in bytes that is available on the 
 * file system the file or directory resides on.
 *
 * @param pathName path name of any file within the file system.
 * @param result returned value: on success, size of available storage space (bytes).
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_free_size(javacall_const_utf16_string pathName, 
                                      javacall_int64* /* OUT */ result) {
    unsigned int free_high, free_low;
    int ret;
    javacall_utf16 device[MMIFILE_DEVICE_NAME_MAX_LEN];
    unsigned short devLen = MMIFILE_DEVICE_NAME_MAX_LEN;
    int i;

    FC_DEBUG(LOG_STR("path=%s"), javacall_wstr2str(pathName));/*lint !e64 */
    if(pathName == NULL)
        return JAVACALL_FAIL;


    if(TRUE != MMIAPIFMM_SplitFullPath(pathName, MMIAPICOM_Wstrlen(pathName),
                                     device, &devLen,
                                     NULL, NULL, NULL, NULL)) {
        javacall_printf(LOG_STR_LEAVE("fail, to split path"));
        return JAVACALL_FAIL;
    }

    FC_DEBUG(LOG_STR("len=%d"), devLen);
    device[devLen] = NULL;

    for(i=0; i<7; i++)
    {
        FC_DEBUG(LOG_STR("%d %x"), i, device[i]);
    }
    FC_DEBUG(LOG_STR("device=%s"), javacall_wstr2str(device));/*lint !e64 */

    ret = MMIAPIFMM_GetDeviceFreeSpace(device, MMIAPICOM_Wstrlen(device), &free_high, &free_low);/*lint !e64 */
    if(ret == TRUE)
    {
        *result = free_low;
        FC_DEBUG(LOG_STR_LEAVE("ok, size=%d"), free_low);
        return JAVACALL_OK;
    } else {
        FC_DEBUG(LOG_STR_LEAVE("return failed"));
        return JAVACALL_FAIL;
    }
}

/**
 * Determines the total size in bytes of the file system the file 
 * or directory resides on.
 *
 * @param pathName file name of any file within the file system.
 * @param pathNameLen length of path name.
 * @param result returned value: on success, total size of storage space (bytes).
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_total_size(javacall_const_utf16_string pathName,
                                       javacall_int64* /* OUT */ result) {
    unsigned int free_high, free_low;
    int ret;
    javacall_utf16 device[MMIFILE_DEVICE_NAME_MAX_LEN];
    unsigned short devLen = MMIFILE_DEVICE_NAME_MAX_LEN;

    if(pathName == NULL)
         return JAVACALL_FAIL;

    FC_DEBUG(LOG_STR("path=%s"), javacall_wstr2str(pathName));/*lint !e64 */

    if(TRUE != MMIAPIFMM_SplitFullPath(pathName, MMIAPICOM_Wstrlen(pathName),
                                     device, &devLen,
                                     NULL, NULL, NULL, NULL))
        return JAVACALL_FAIL;

    device[devLen] = NULL;

    FC_DEBUG(LOG_STR("device=%s"), javacall_wstr2str(device));/*lint !e64 */

    ret = MMIAPIFMM_GetDeviceFreeSpace(device, MMIAPICOM_Wstrlen(device), &free_high, &free_low);/*lint !e64 */
    if(ret == TRUE) {
        *result = free_low;
    } else {
        FC_DEBUG(LOG_STR_LEAVE("return failed"));
        return JAVACALL_FAIL;
    }

    ret = MMIAPIFMM_GetDeviceUsedSpace(device, MMIAPICOM_Wstrlen(device), &free_high, &free_low);/*lint !e64 */
    if(ret == TRUE) {
        *result += free_low;/*lint !e737 */
        return JAVACALL_OK;
    } else {
        FC_DEBUG(LOG_STR_LEAVE("return failed"));
        return JAVACALL_FAIL;
    }
}

/** 
 * Returns the mounted root file systems. Each root must end
 * with '/' character.
 *
 * @param roots buffer to store the string containing
 *              currently mounted roots separated by '\n' character.
 * @param rootsLen available buffer size (maximum number of
 *                 characters to be stored).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_mounted_roots(javacall_utf16_string /* OUT */ roots,
                                          int rootsLen) {
    uint i = 0;
    int len = 0;
    int first = 1;
    
    javacall_printf(LOG_STR_ENTER(""));    
    if(roots == NULL)
        return JAVACALL_FAIL;

    update_root_tbl();

    roots[0] = NULL;

    for(i = 0; i < sizeof(jsr75fc_rootVSpath_tbl) / sizeof(jsr75fc_rootVSpath); i++) {/*lint !e737 */
        if(jsr75fc_rootVSpath_tbl[i].ready == 1) {
            len += MMIAPICOM_Wstrlen(jsr75fc_rootVSpath_tbl[i].root);/*lint !e737 */
            if( len > rootsLen) {
                javacall_printf(LOG_STR_LEAVE("fail"));
                return JAVACALL_FAIL;
            }
            if(!first)
                MMIAPICOM_Wstrcat(roots, L"\n");
            else 
                first = 0;
            MMIAPICOM_Wstrcat(roots, jsr75fc_rootVSpath_tbl[i].root);
        }

    }
//    javacall_printf(LOG_STR_LEAVE("ok, %s"), javacall_wstr2str(roots));
    return JAVACALL_OK;
}

/** 
 * Returns the path to photo and other images storage, using '/' as
 * file separator. The path must end with this separator as well.
 *
 * @param dir buffer to store the string containing path to
 *            directory with photos.
 * @param dirLen available buffer size (maximum number of
 *               characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_photos_dir(javacall_utf16_string /* OUT */ dir,
                                       int dirLen, javacall_bool fromCache) {
    (void)dir;
    (void)dirLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/** 
 * Returns the path to video clips storage, using '/' as file separator.
 * The path must end with this separator as well.
 *
 * @param dir buffer to store the string containing path to
 *            directory with video clips.
 * @param dirLen available buffer size (maximum number of
 *               characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_videos_dir(javacall_utf16_string /* OUT */ dir,
                                       int dirLen, javacall_bool fromCache) {
    (void)dir;
    (void)dirLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/** 
 * Returns the path to clip art graphics storage, using '/' as file separator.
 * The path must end with this separator as well.
 *
 * @param dir buffer to store the string containing path to
 *            directory with graphics.
 * @param dirLen available buffer size (maximum number of
 *               characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_graphics_dir(javacall_utf16_string /* OUT */ dir,
                                         int dirLen, javacall_bool fromCache) {
    (void)dir;
    (void)dirLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/** 
 * Returns the path to ring tones and other related audio files storage,
 * using '/' as file separator. The path must end with this separator as well.
 *
 * @param dir buffer to store the string containing path to
 *            directory with ring tones.
 * @param dirLen available buffer size (maximum number of
 *               characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_tones_dir(javacall_utf16_string /* OUT */ dir,
                                      int dirLen, javacall_bool fromCache) {
    (void)dir;
    (void)dirLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/** 
 * Returns the path to music files storage, using '/' as file separator.
 * The path must end with this separator as well
 *
 * @param dir buffer to store the string containing path to
 *            directory with music.
 * @param dirLen available buffer size (maximum number of
 *               characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_music_dir(javacall_utf16_string /* OUT */ dir,
                                      int dirLen, javacall_bool fromCache) {
    (void)dir;
    (void)dirLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/** 
 * Returns the path to voice recordings storage, using '/' as file separator.
 * The path must end with this separator as well.
 *
 * @param dir buffer to store the string containing path to
 *            directory with voice recordings.
 * @param dirLen available buffer size (maximum number of
 *               characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_recordings_dir(javacall_utf16_string /* OUT */ dir,
                                           int dirLen, javacall_bool fromCache) {
    (void)dir;
    (void)dirLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/** 
 * Returns the path to directory, that is used to store private directories
 * for all applications (accessed via "fileconn.dir.private" system property).
 * The returned path must use '/' as file separator and have this separator at
 * the end.
 *
 * @param dir buffer to store the string containing path to
 *            location of private directories for all applications.
 * @param dirLen available buffer size (maximum number of
 *               characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_private_dir(javacall_utf16_string /* OUT */ dir,
                                        int dirLen, javacall_bool fromCache) {
    (void)dir;
    (void)dirLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/** 
 * Returns the path to memory card files storage, using '/' as file separator.
 * The path must end with this separator as well
 *
 * @param dir buffer to store the string containing path to
 *            directory with memory card.
 * @param dirLen available buffer size (maximum number of
 *               characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_memorycard_dir(javacall_utf16_string /* OUT */ dir,
                                      int dirLen, javacall_bool fromCache) {
    
    if(dir == NULL)
        return JAVACALL_FAIL;

    FC_DEBUG(LOG_STR(""));
    dirLen = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD);
    MMIAPICOM_Wstrncpy(dir, MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), dirLen);
    dirLen++;
    dir[dirLen] = MMIFILE_COLON;
    dirLen++;
    dir[dirLen] = MMIFILE_SLASH;
    //dirLen++
    fromCache=JAVACALL_FALSE;
    return JAVACALL_OK;
}

/** 
 * Returns the localized names for mounted root file systems.
 *
 * @param roots buffer to store the string containing localized names
 *              of currently mounted roots separated by ';' character.
 * @param rootsLen available buffer size (maximum number of
 *                 characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_localized_mounted_roots(javacall_utf16_string /* OUT */ names,
                                                    int namesLen,
                                                    javacall_bool fromCache) {
    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Returns localized name for directory with photos and other images, corresponding to
 * the path returned by <tt>System.getProperty("fileconn.dir.photos")</tt>.
 *
 * @param name buffer to store the string containing localized name.
 * @param nameLen available buffer size (maximum number of
 *                characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_localized_photos_dir(javacall_utf16_string /* OUT */ name,
                                                 int nameLen,
                                                 javacall_bool fromCache) {
    (void)name;
    (void)nameLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Returns localized name for video clips location, corresponding to
 * the path returned by <tt>System.getProperty("fileconn.dir.videos")</tt>.
 *
 * @param name buffer to store the string containing localized name.
 * @param nameLen available buffer size (maximum number of
 *                characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_localized_videos_dir(javacall_utf16_string /* OUT */ name,
                                                 int nameLen,
                                                 javacall_bool fromCache) {
    (void)name;
    (void)nameLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Returns localized name for directory containing clip art graphics, corresponding
 * to the path returned by <tt>System.getProperty("fileconn.dir.graphics")</tt>.
 *
 * @param name buffer to store the string containing localized name.
 * @param nameLen available buffer size (maximum number of
 *                characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_localized_graphics_dir(javacall_utf16_string /* OUT */ name,
                                                   int nameLen,
                                                   javacall_bool fromCache) {
    (void)name;
    (void)nameLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Returns localized name for directory with ring tones and other related audio files,
 * corresponding to the path returned by
 * <tt>System.getProperty("fileconn.dir.tones")</tt>.
 *
 * @param name buffer to store the string containing localized name.
 * @param nameLen available buffer size (maximum number of
 *                characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_localized_tones_dir(javacall_utf16_string /* OUT */ name,
                                                int nameLen,
                                                javacall_bool fromCache) {
    (void)name;
    (void)nameLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Returns localized name for music files storage, corresponding to
 * the path returned by <tt>System.getProperty("fileconn.dir.music")</tt>.
 *
 * @param name buffer to store the string containing localized name.
 * @param nameLen available buffer size (maximum number of
 *                characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_localized_music_dir(javacall_utf16_string /* OUT */ name,
                                                int nameLen,
                                                javacall_bool fromCache) {
    (void)name;
    (void)nameLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Returns localized name for voice recordings storage, corresponding to
 * the path returned by <tt>System.getProperty("fileconn.dir.recordings")</tt>.
 *
 * @param name buffer to store the string containing localized name.
 * @param nameLen available buffer size (maximum number of
 *                characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_localized_recordings_dir(javacall_utf16_string /* OUT */ name,
                                                     int nameLen,
                                                     javacall_bool fromCache) {
    (void)name;
    (void)nameLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Returns localized private directory name corresponding to the path returned by
 * <tt>System.getProperty("fileconn.dir.private")</tt>.
 *
 * @param name buffer to store the string containing localized name.
 * @param nameLen available buffer size (maximum number of
 *                characters to be stored).
 * @param fromCache indicates whether the returned value should be taken from
 *                  internal cache (this parameter can be ignored if properties
 *                  caching is not supported by underlying implementation).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_localized_private_dir(javacall_utf16_string /* OUT */ name,
                                                  int nameLen,
                                                  javacall_bool fromCache) {
    (void)name;
    (void)nameLen;
    (void)fromCache;

    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Returns OS-specific path for the specified file system root.
 *
 * @param rootName root name.
 * @param pathName buffer to store the string containing 
 *                 the system-dependent path to access the specified 
 *                 root.
 * @param pathNameLen available buffer size (maximum number of
 *                    characters to be stored).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_path_for_root(javacall_const_utf16_string rootName,
                                          javacall_utf16_string /* OUT */ pathName,
                                          int pathNameLen) {
    uint i = 0;
    int len;
    
    if(rootName == NULL)
        return JAVACALL_FAIL;

    FC_DEBUG(LOG_STR_ENTER("root=%s"), javacall_wstr2str(rootName));/*lint !e64 */

    for(i = 0; i < sizeof(jsr75fc_rootVSpath_tbl) / sizeof(jsr75fc_rootVSpath); i++) {/*lint !e737 */
        FC_DEBUG(LOG_STR("i=%d, ready=%d, root=%s"), i, jsr75fc_rootVSpath_tbl[i].ready, javacall_wstr2str(jsr75fc_rootVSpath_tbl[i].root));/*lint !e64 */

        if((jsr75fc_rootVSpath_tbl[i].ready == 1) && 
           (MMIAPICOM_Wstrcmp(rootName, jsr75fc_rootVSpath_tbl[i].root) == 0)) {
            len = MMIAPICOM_Wstrlen(jsr75fc_rootVSpath_tbl[i].path);
            if( len < pathNameLen) {
                MMIAPICOM_Wstrcpy(pathName, jsr75fc_rootVSpath_tbl[i].path);
                pathName[len] = 0;
                return JAVACALL_OK;
            } else
                return JAVACALL_FAIL;
        }
    }
    return JAVACALL_FAIL;
}

/** @} */


/** 
 * @defgroup jsrCLDCFileConnection CLDC-specific FC API
 * @ingroup JSR75
 *
 * @{
 */ 

/**
 * Returns the HIDDEN attribute for the specified file or directory.
 * If hidden files are not supported, the function should 
 * return <tt>JAVACALL_FALSE</tt>.
 *
 * @param fileName      name of file or directory.
 * @param result        returned value: <tt>JAVACALL_TRUE</tt> if file is hidden,
 *                      <tt>JAVACALL_FALSE</tt> if file is not hidden or 
 *                      feature is not supported.
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> if an error occured.
 */
javacall_result
javacall_fileconnection_is_hidden(javacall_const_utf16_string fileName,
                                  javacall_bool* /* OUT */ result) {
    *result = JAVACALL_FALSE;
    return JAVACALL_OK;

    /* javacall_utf16 tmpPath[MMIFILE_FULL_PATH_MAX_LEN]; */
    /* int len; */
    /* unsigned short attr; */
    /* SFS_ERROR_E ret; */
    /* javacall_utf16 sep = javacall_get_file_separator(); */

    /* if(fileName == NULL) */
    /*     return JAVACALL_FAIL; */

    /* utf16print("javacall_fileconnection_is_hidden", fileName); */

    /* len = MMIAPICOM_Wstrlen(fileName); */
    /* MMIAPICOM_Wstrcpy(tmpPath, fileName); */
    /* if(fileName[len - 1] == sep) { */
    /*     tmpPath[len - 1] = NULL; */
    /* } */

    /* ret = SFS_GetAttr(tmpPath, &attr); */

    /* javacall_printf("javacall_fileconnection_is_hidden %d", attr ); */

    /* if (ret == SFS_NO_ERROR) { */
    /*     *result = (attr & SFS_ATTR_HIDDEN)? JAVACALL_TRUE : JAVACALL_FALSE; */
    /*     return JAVACALL_OK; */
    /* } else { */
    /*     javacall_printf("javacall_fileconnection_is_hidden SFS_GetAttr got %d", ret); */
    /*     return JAVACALL_FAIL; */
    /* } */
}

/**
 * Returns the READABLE attribute for the specified file or directory.
 *
 * @param pathName      name of file or directory.
 * @param result        returned value: <tt>JAVACALL_TRUE</tt> if file/dir is readable
 *                      <tt>JAVACALL_FALSE</tt> if file/dir is not readable.
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> if an error occured.
 */
javacall_result
javacall_fileconnection_is_readable(javacall_const_utf16_string pathName,
                                    javacall_bool* /* OUT */ result) {
    if((javacall_file_exist(pathName, MMIAPICOM_Wstrlen(pathName)) == JAVACALL_OK) || 
       (javacall_fileconnection_dir_exists(pathName) == JAVACALL_OK))
        *result = JAVACALL_TRUE;
    else 
        *result = JAVACALL_FALSE;

    return JAVACALL_OK;
}

/**
 * Returns the WRITABLE attribute for the specified file or directory.
 *
 * @param pathName      name of file or directory.
 * @param result        returned value: <tt>JAVACALL_TRUE</tt> if file/dir is writable,
 *                      <tt>JAVACALL_FALSE</tt> if file/dir is not writable.
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> if an error occured.
 */
javacall_result
javacall_fileconnection_is_writable(javacall_const_utf16_string pathName,
                                    javacall_bool* /* OUT */ result) {
    if((javacall_file_exist(pathName, MMIAPICOM_Wstrlen(pathName)) == JAVACALL_OK) || 
       (javacall_fileconnection_dir_exists(pathName) == JAVACALL_OK))
        *result = JAVACALL_TRUE;
    else 
        *result = JAVACALL_FALSE;

    return JAVACALL_OK;

    /* javacall_utf16 tmpPath[MMIFILE_FULL_PATH_MAX_LEN]; */
    /* int len; */
    /* unsigned short attr; */
    /* SFS_ERROR_E ret; */
    /* javacall_utf16 sep = javacall_get_file_separator(); */

    /* if(pathName == NULL) */
    /*     return JAVACALL_FAIL; */

    /* utf16print("javacall_fileconnection_is_writable", pathName); */

    /* len = MMIAPICOM_Wstrlen(pathName); */
    /* MMIAPICOM_Wstrcpy(tmpPath, pathName); */
    /* if(pathName[len - 1] == sep) { */
    /*     tmpPath[len - 1] = NULL; */
    /* } */

    /* ret = SFS_GetAttr(tmpPath, &attr); */

    /* javacall_printf("javacall_fileconnection_is_writable %d", attr ); */

    /* if (ret == SFS_NO_ERROR) { */
    /*     *result = !(attr & SFS_ATTR_RO)? JAVACALL_TRUE : JAVACALL_FALSE; */
    /*     return JAVACALL_OK; */
    /* } else { */
    /*     javacall_printf("javacall_fileconnection_is_writable SFS_GetAttr got %d", ret); */
    /*     return JAVACALL_FAIL; */
    /* } */

}

/**
 * Returns the time when the file or directory was last modified.
 *
 * @param fileName      name of file or directory.
 * @param result        A javacall_int64 value representing the time the file was 
 *                      last modified, measured in seconds since the epoch (00:00:00 GMT, 
 *                      January 1, 1970).
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_get_last_modified(javacall_const_utf16_string fileName, 
                                          javacall_int64* /* OUT */ result) {
    uint32 size = 0;
    SFS_DATE_T modify_date;
    SFS_TIME_T modify_time;

    if(TRUE == MMIAPIFMM_GetFileInfo(fileName, MMIAPICOM_Wstrlen(fileName),
                                   &size,
                                   &modify_date, &modify_time))
    {
        *result = MMIAPICOM_Tm2Second(modify_time.sec,
                                      modify_time.min,
                                      modify_time.hour,
                                      modify_date.mday,
                                      modify_date.mon,
                                      modify_date.year) + deltaTime
                                      - (uint32)(MMIAPIACC_GetLocalTimeZone()*60*60); //subtract time zone offset
        FC_DEBUG(LOG_STR_LEAVE("MMIAPICOM_TansferSFSTime %lld"), *result);
        return JAVACALL_OK;
    } else {
        FC_DEBUG(LOG_STR_LEAVE("MMIAPIFMM_GetFileInfo return fail"));
        return JAVACALL_FAIL;
    }
}

static javacall_bool is_root(javacall_const_utf16_string pathName)
{
    uint i;
    FC_DEBUG(LOG_STR_ENTER("path=%s"), javacall_wstr2str(pathName));/*lint !e64 */
    update_root_tbl();

    for(i = 0; i < sizeof(jsr75fc_rootVSpath_tbl) / sizeof(jsr75fc_rootVSpath); i++) {/*lint !e737 */
        if(jsr75fc_rootVSpath_tbl[i].ready == 1) {
            int len = MMIAPICOM_Wstrlen(pathName);
            if( (len == 2 || len == 3)
                && (MMIAPICOM_Wstrncmp(pathName, jsr75fc_rootVSpath_tbl[i].path, MMIAPICOM_Wstrlen(jsr75fc_rootVSpath_tbl[i].path) - 1) == 0)) {
                return JAVACALL_TRUE;
            }
        }
    }
    return JAVACALL_FALSE;
}

/**
 * Checks if the path exists in the file system storage and if 
 * it is a directory.
 *
 * @param pathName name of file or directory.
 * @param result returned value: <tt>JAVACALL_TRUE</tt> if path is a directory, 
 *                               <tt>JAVACALL_FALSE</tt> otherwise.
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully,
 *         <tt>JAVACALL_FAIL</tt> if an error occured.
 */
javacall_result
javacall_fileconnection_is_directory(javacall_const_utf16_string pathName, 
                                     javacall_bool* /* OUT */ result) {

    javacall_utf16 tmpPath[MMIFILE_FULL_PATH_MAX_LEN];
    int len;
    unsigned short attr;
    SFS_ERROR_E ret;
    javacall_utf16 sep = javacall_get_file_separator();

    if(pathName == NULL)
        return JAVACALL_FAIL;

    FC_DEBUG(LOG_STR("path=%s"), javacall_wstr2str(pathName));/*lint !e64 */


    len = MMIAPICOM_Wstrlen(pathName);
    MMIAPICOM_Wstrcpy(tmpPath, pathName);

    if(pathName[len - 1] == sep) {
        tmpPath[len - 1] = NULL;
    }

    if(is_root(pathName) == JAVACALL_TRUE) {
        *result = JAVACALL_TRUE;
        return JAVACALL_OK;
    }

    ret = SFS_GetAttr(tmpPath, &attr);

    if (ret == SFS_NO_ERROR) {
        *result = (attr & SFS_ATTR_DIR)? JAVACALL_TRUE : JAVACALL_FALSE;
        return JAVACALL_OK;
    } else {
        return JAVACALL_FAIL;
    }
}

/**
 * Creates a directory.
 *
 * @param dirName path name of directory.
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> on failure.
 */
javacall_result
javacall_fileconnection_create_dir(javacall_const_utf16_string dirName) {

    int ret;
    javacall_utf16 tmpName[MMIFILE_FULL_PATH_MAX_LEN];
    int i;
    javacall_utf16 sep = javacall_get_file_separator();

    FC_DEBUG(LOG_STR("dir=%s"), javacall_wstr2str(dirName));/*lint !e64 */

    if(is_root(dirName) != JAVACALL_TRUE)
    {
        MMIAPICOM_Wstrcpy(tmpName, dirName);

        for(i = MMIAPICOM_Wstrlen(tmpName)-1; i>0; i--)
            if(tmpName[i] == sep) {
                tmpName[i] = NULL;
                break;
            }

        if(javacall_fileconnection_dir_exists(tmpName) != JAVACALL_OK)
        {
            FC_DEBUG(LOG_STR_LEAVE("parent don't exists"));
            return JAVACALL_FAIL;
        }
    }
        
    ret = MMIAPIFMM_CreateDirectory(dirName);
    if( ret == SFS_NO_ERROR)
        return JAVACALL_OK;
    else {
        FC_DEBUG(LOG_STR_LEAVE("MMIAPIFMM_CreateDirectory returns %d"), ret);
        return JAVACALL_FAIL;
    }
}

javacall_result
javacall_fileconnection_create_hide_dir(javacall_const_utf16_string dirName) {
    javacall_result ret;

    ret = javacall_fileconnection_create_dir(dirName);
    if (JAVACALL_OK == ret) {
        // De-comment this part of codes after platform bug fixed
        BOOLEAN ret2 = TRUE;
        ret2 = MMIAPIFMM_SetAttr((wchar*)dirName, MMIAPICOM_Wstrlen(dirName), TRUE, FALSE, TRUE, FALSE);
        if (TRUE == ret2) {
            return JAVACALL_OK; 
        } else {
            return JAVACALL_FAIL;
        }
    } else {
        return JAVACALL_FAIL;
    }
}
/**
 * Deletes an empty directory from the persistent storage.
 * If directory is not empty this function must fail.
 *
 * @param dirName path name of directory.
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> on failure.
 */
javacall_result
javacall_fileconnection_delete_dir(javacall_const_utf16_string dirName) {

    int ret = MMIAPIFMM_DeleteDirectory(dirName);
    FC_DEBUG(LOG_STR_ENTER("dir=%s"), javacall_wstr2str(dirName));/*lint !e64 */
    if( ret == SFS_NO_ERROR)
        return JAVACALL_OK;
    else {
        FC_DEBUG(LOG_STR_LEAVE("MMIAPIFMM_DeleteDirectory returns %d"), ret);
        return JAVACALL_FAIL;
    }
}

/**
 * Check if the directory exists in file system storage.
 *
 * @param pathName name of directory in unicode format.
 * @return <tt>JAVACALL_OK </tt> if it exists and it is a regular directory, 
 *         <tt>JAVACALL_FAIL</tt> if directory does not exist, or any error
 *         has occured.
 */
javacall_result
javacall_fileconnection_dir_exists(javacall_const_utf16_string pathName) {

    BOOLEAN ret;
    //javacall_utf16 sep = javacall_get_file_separator();

    if(pathName == NULL)
        return JAVACALL_FAIL;

    FC_DEBUG(LOG_STR_ENTER("path=%s"), javacall_wstr2str(pathName));/*lint !e64 */

    if(is_root(pathName) == JAVACALL_TRUE)
        return JAVACALL_OK;
    ret=MMIAPIFMM_IsFolderExist(pathName, MMIAPICOM_Wstrlen(pathName));
    if (ret == TRUE) {
        javacall_printf(LOG_STR("ok, %s"), javacall_wstr2str(pathName));/*lint !e64 */
        return JAVACALL_OK;
    }
    javacall_printf(LOG_STR("fail, %s"), javacall_wstr2str(pathName));/*lint !e64 */
    return JAVACALL_FAIL;
}


/**
 * Renames the specified directory.
 *
 * @param oldDirName current name of directory.
 * @param newDirName new name of directory.
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_rename_dir(javacall_const_utf16_string oldDirName,
                                   javacall_const_utf16_string newDirName) {    
    wchar           new_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16          new_name_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    javacall_printf(LOG_STR_ENTER("old=%s"), javacall_wstr2str(oldDirName));/*lint !e64 */
    javacall_printf(LOG_STR("new=%s"), javacall_wstr2str(newDirName));/*lint !e64 */

    if(MMIAPIFMM_SplitFullPath(newDirName, MMIAPICOM_Wstrlen(newDirName),
                             NULL, NULL, NULL, NULL,
                             new_name, &new_name_len))
    {
    if(TRUE == MMIAPIFMM_RenameFile(oldDirName, MMIAPICOM_Wstrlen(oldDirName),
                                  new_name, new_name_len))
        return JAVACALL_OK;
    }

        return JAVACALL_FAIL;
}

/**
 * Get size in bytes of all files and possibly subdirectories contained 
 * in the specified dir.
 *
 * @param pathName          path name of directory.
 * @param includeSubdirs    if <tt>JAVACALL_TRUE</tt>, include subdirectories size too;
 *                          if <tt>JAVACALL_FALSE</tt>, do not include subdirectories.
 * @param result            returned value: size in bytes of all files contained in 
 *                          the specified directory and possibly its subdirectories.
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_dir_content_size(javacall_const_utf16_string pathName,
                                         javacall_bool includeSubdirs,
                                         javacall_int64* /* OUT */ result) {

    volatile int queue_count;
    int count;
    javacall_result ret = JAVACALL_OK;
    javacall_handle handle;
    javacall_utf16* next;
    javacall_utf16 pwd[MMIFILE_FULL_PATH_MAX_LEN];
    int pwd_len;
    int i;
    #define max_queue 256
    javacall_utf16* queue[max_queue+1];

    *result = 0;
    queue_count = 0;

    if(pathName == NULL)
    {
        FC_DEBUG(LOG_STR_LEAVE("fail, got NULL"));
        return JAVACALL_FAIL;
    }

    FC_DEBUG(LOG_STR("path=%s"), javacall_wstr2str(pathName));/*lint !e64 */

    if(javacall_fileconnection_dir_exists(pathName) != JAVACALL_OK)
    {
        FC_DEBUG(LOG_STR_LEAVE("fail, got non-dir"));
        return JAVACALL_FAIL;
    }

    MMIAPICOM_Wstrcpy(pwd, pathName);
    pwd_len = MMIAPICOM_Wstrlen(pwd);
    if(pwd[pwd_len-1] == javacall_get_file_separator())
    {
        pwd_len --;
    }

    handle = javacall_dir_open(pathName, MMIAPICOM_Wstrlen(pathName));
    if(handle == NULL) {
        if (javacall_fileconnection_dir_exists(pathName) != JAVACALL_OK){
            FC_DEBUG(LOG_STR_LEAVE("fail, can't get handle"));
            return JAVACALL_FAIL;
        } else {
            *result = 0;
            FC_DEBUG(LOG_STR_LEAVE("ok, got %d %d"), ret, *result);
            return JAVACALL_OK;
        }
    }
    
    do {        
        int nextLen;

        next = javacall_dir_get_next(handle, &nextLen); 
        if(next == NULL)
            break;

        if(nextLen + pwd_len + 1 > MMIFILE_FULL_PATH_MAX_LEN) {
            ret = JAVACALL_FAIL;
            break;
        }

        pwd[pwd_len] = javacall_get_file_separator();
        pwd[pwd_len+1] = NULL;
        MMIAPICOM_Wstrcat(pwd, next);

        if(javacall_fileconnection_dir_exists(pwd) == JAVACALL_OK)
        {
            if(includeSubdirs)
            {
                queue[queue_count] = (javacall_utf16*) javacall_malloc(MMIAPICOM_Wstrlen(pwd) + 1);
                MMIAPICOM_Wstrcpy(queue[queue_count], pwd);
                queue_count ++;
                if(queue_count >= max_queue)
                {
                    FC_DEBUG(LOG_STR("fail, queue full"));
                    ret =  JAVACALL_FAIL;
                }
            }
        } else {
            count = (int32)javacall_file_sizeof(pwd, MMIAPICOM_Wstrlen(pwd));
            if(count == -1)
            {
                FC_DEBUG(LOG_STR("fail"));
                ret = JAVACALL_FAIL;
                break;
            }
            else 
                *result += count;
        }
    } while (1);/*lint !e506 */

    javacall_dir_close(handle);

    queue[queue_count] = NULL;
    for(i=0; i<queue_count; i++)
    {
        javacall_int64 sub_result;

        if(javacall_fileconnection_dir_content_size (queue[i], includeSubdirs, &sub_result) == JAVACALL_OK)
            *result += sub_result;
        else
        {
            FC_DEBUG(LOG_STR("fail"));
            ret = JAVACALL_FAIL;
            break;
        }
    }

    for(i=0; i<queue_count; i++)
        javacall_free(queue[queue_count]);
                
    FC_DEBUG(LOG_STR_LEAVE("ret=%d, result=%d"), ret, *result);
    return ret;
}

/** @} */


/** 
 * @defgroup jsrCDCFileConnection CDC-specific FC API
 * @ingroup JSR75
 *
 * @{
 */ 

/** 
 * Tells the implementation to cache values of all FileConnection
 * dynamic properties (this call can be ignored if property caching is not
 * supported by the implementation).
 * @note This function is only used by CDC-based Java stack.
 *
 * @return <tt>JAVACALL_OK</tt> on success,
 *         <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result
javacall_fileconnection_cache_properties(void) {
    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Activates notifications for file system roots being mounted or unmounted.
 * After calling this function, javanotify_fileconnection_root_changed() must
 * be called by the platform every time the event occurs.
 * @note This function is only used by CDC-based Java stack.
 *
 * @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result javacall_fileconnection_activate_notifications(void) {
    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * Deactivates notifications for file system roots being mounted or unmounted.
 * After calling this function, javanotify_fileconnection_root_changed() must
 * not be called by the platform.
 * @note This function is only used by CDC-based Java stack.
 *
 * @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> otherwise.
 */
javacall_result javacall_fileconnection_deactivate_notifications(void) {
    return JAVACALL_NOT_IMPLEMENTED;
}


/** @} */

/** @} */

PUBLIC BOOLEAN jc_convert_fc2native_path(javacall_const_utf16_string src, int srcLen, javacall_utf16_string dst, int dstLen)
{
    int len;
    uint i;
    int j;

    javacall_printf(LOG_STR_ENTER("len=%d, src=%s"), srcLen, javacall_nwstr2str(src, srcLen));/*lint !e64 */
    for (i = 0; i < sizeof(jsr75fc_rootVSpath_tbl) / sizeof(jsr75fc_rootVSpath); i++) {/*lint !e737 */
        //if (jsr75fc_rootVSpath_tbl[i].ready == 1) {
            len=MMIAPICOM_Wstrlen(jsr75fc_rootVSpath_tbl[i].root);
            //javacall_printf(LOG_STR("i=%d, len=%d, ready=%d, root=%s"), i, len, jsr75fc_rootVSpath_tbl[i].ready, javacall_wstr2str(jsr75fc_rootVSpath_tbl[i].root));
            if (memcmp(src, jsr75fc_rootVSpath_tbl[i].root, len*2) == 0) {
                //javacall_printf(LOG_STR("match"));
                len = MMIAPICOM_Wstrlen(jsr75fc_rootVSpath_tbl[i].root);
                src+=len;
                srcLen-=len;
                //javacall_printf(LOG_STR("len=%d"), len);
                SCI_MEMSET(dst, 0, dstLen*2);
                len = MMIAPICOM_Wstrlen(jsr75fc_rootVSpath_tbl[i].path);
                SCI_MEMCPY(dst, jsr75fc_rootVSpath_tbl[i].path, len*2);
                SCI_MEMCPY(dst+len, src, srcLen*2);
                //javacall_printf(LOG_STR("len=%d, srcLen=%d"), len, srcLen);
                dst[len+srcLen]=0;

                for (j=0; j<len+srcLen; j++)
                    if (dst[j] == '/')
                        dst[j]='\\';
                javacall_printf(LOG_STR_LEAVE("ok, native path=%s"), javacall_wstr2str(dst));/*lint !e64 */
                return TRUE;
            }
        //}
    }
    javacall_printf(LOG_STR_LEAVE("false"));
    return FALSE;
}

#ifdef __cplusplus
}
#endif
