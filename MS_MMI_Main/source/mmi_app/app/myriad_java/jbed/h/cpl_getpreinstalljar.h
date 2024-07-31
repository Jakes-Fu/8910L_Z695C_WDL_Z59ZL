#ifndef _CPL_GETPREINSTALLJAR_
#define _CPL_GETPREINSTALLJAR_

#include <opl_global.h>
#include "mmi_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************/
/*          zip parsing pre-definitions                                              */
/*************************************************************************************/
#define URL_MAX_LEN 255
#define ZIP_FILE_HEADER 0x04034b50
#define ZIP_FOLDER_HEADER 0x02014b50
#define ZIP_FILE_END 0x06054b50

#define PHONE_FLODER "D:/Java/tmp/"
#define SD_CARD_FLODER "E:/Java/tmp/"

typedef enum
{
    JAVA_UNZIP_OPERATION_SUCCESS = 0,
    JAVA_UNZIP_OPERATION_FAILED,
    JAVA_UNZIP_NOT_ENOUGH_MEMORY,
    JAVA_UNZIP_NO_JAR_ERROR,
    JAVA_UNZIP_DEVICE_STATUS_ERROR
} JavaUnzipOperationResult;

typedef struct
{
    uint8 *startPtr;
    uint32_t  zipLength;
    uint32_t  dirOffset;
} JavaZipRomInfo;

typedef struct
{
    char* name;
    uint8_t* buffer;
    int32_t bufLen;
} JavaPreinstallIconsInfo;

/**
 * Get rom zip infomation with specified resource id.
 * @param resId, the resource id.
 * @param pZipInfo, the rom zip infomation pointer which must pass a valid address,
 *   in this way, it is able to bring back zip information after calling the function.
 * @return the result of JavaUnzipOperationResult, wherein JAVA_UNZIP_OPERATION_SUCCESS means success, other values mean failure.
 */
JavaUnzipOperationResult Jbed_getZipInfo(MMI_DATA_ID_T resId, JavaZipRomInfo *pZipInfo);

/**
 * Get the uncompress data from rom zip information.
 * @pZipInfo, the zip info which is acquired by Jbed_getZipInfo.
 * @confn, the local file name which was compressed in zip package.
 * @dataLen, is a output data which saved the uncompress data size in bytes.
 * @return return the uncompress data pointer.
 * NOTE:
 *      the returned memory pointer must be freed by callee API.
 */
uint8_t* Jbed_getUnzipContentByFileName(JavaZipRomInfo *pZipInfo, uint8_t* confn, int32_t* dataLen);

/**
 * Get jad/jar file from resource id by midlet_name and return operation result
 * file_uri will contain the jad/jar file path,if have jad and jar,set jad path,if only have jar set jar path,otherwise set null;
 * @resId, the mmi resource id;
 * @midletname, the midlet file name not have suffix;
 * @fileuri, the pointer of the jad/jar file path;
 * @return the result of JavaUnzipOperationResult, wherein JAVA_UNZIP_OPERATION_SUCCESS means success,
 *  other values mean failure. Refer to JavaUnzipOperationResult struct for details.
 *
 * NOTE:
 *      the file_uri memory pointer must be freed by callee API.
 */
JavaUnzipOperationResult Jbed_getPreinstallFile(MMI_DATA_ID_T resId, char* midletname, char** fileuri);

/**
 * Get icons info list from the zip by resource id.
 * @resId, the resource id.
 * @iconsCount, is an output data which means the count of list.
 * @return the JavaPreinstallIconsInfo data list pointer which
 *    contains iconsCount JavaPreinstallIconsInfo struct data.
 *    return NULL means getting icon failed or without icon in zip file.
 * NOTE: 1) On your side, you must compress the preinstall
 *          resource(including jad, jar and icon) into standard zip format.
 *       2) After calling Jbed_getPreinstallIcons, you must call
 *          Jbed_freePreinstallIcons for free memory. See below example usage code:
 * {code}
 *     int32_t count = 0, i;
 *     JavaPreinstallIconsInfo *list = Jbed_getPreinstallIcons(DATA_JAVA_APPS, &count);
 *     for (i = 0; i < count; i++)
 *     {
 *         // list[i].name means icon name in utf8 format.
 *         // use list[i].buffer and list[i].bufLen to get(copy) icon buffer data.
 *         SCI_TRACE_LOW("index = %d, name = %s, bufLen = %d \n", i, list[i].name, list[i].bufLen);
 *     }
 *     Jbed_freePreinstallIcons(list, count);
 * {code}
 */
JavaPreinstallIconsInfo* Jbed_getPreinstallIcons(MMI_DATA_ID_T resId, int32_t* iconsCount);

/**
 * Free those memories which are allocated in Jbed_getPreinstallIcons.
 * @iconsList, the JavaPreinstallIconsInfo pointer returned by Jbed_getPreinstallIcons.
 * @count, the icons count getting by iconsCount parameter via Jbed_getPreinstallIcons.
 * @return void(nothing).
 * NOTE: please refer example code in above coments of Jbed_getPreinstallIcons.
 */
void Jbed_freePreinstallIcons(JavaPreinstallIconsInfo *iconsList, int32_t count);
/*************************************************************************************/
/*          zip parsing                                                              */
/*************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
