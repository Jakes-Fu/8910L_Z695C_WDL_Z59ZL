/*****************************************************************************
** File Name:      mmi_filemgr.h                                             *
** Author:                                                                   *
** Date:           08/07/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mmi file manage             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2006       liqing.peng       Create                                    *
** 2006-09-21    liqing.peng       Modify   '/' --> '\'                      *
** 2007-09-21    liqing.peng       Ãû×ÖÖ¸Õë uint8* -> uin16*                 *
**                                 Ãû×Ö³¤¶Èµ¥Î» µ¥×Ö½Ú -> Ë«×Ö½Ú             *
******************************************************************************
** Full Path Format: 
**      full path = device + ':\' + directory + '\' + filename
**      eg:  "D:\picture\abc.jpg"
**      "D" is device name of Udisk, SD card is represented by "E"
**      "picture" is directory name
**      "abc.jpg" is filename
**      use ":", "\" to seperate device, directory, filename (don't use '/')
**
** ×¢:
** 1. ÎÄ¼þÃû³¤¶È¡¢ÎÄ¼þ¼ÐÃû³¤¶È¡¢Éè±¸Ãû³¤¶È¡¢È«Â·¾¶Ãû³¤¶È¶¼ÒÔunicode¸öÊý
******************************************************************************/


#ifndef _MMI_FILEMGR_H_
#define _MMI_FILEMGR_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "mmk_type.h"
#include "ffs.h"
#include "mmi_custom_define.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/

/*********************ffs interface begin************************/

//typedef uint32               HFS;

//ÎÄ¼þÉè±¸ÀàÐÍ¶¨Òå£º
//ÔÝÊ±ÆÁ±Îµô£¬µÈffs.hÈ¥µôÔÙ¼ÓÉÏ
/*
typedef enum
{
 FS_HS,
 FS_UDISK,
 FS_MMC,
 FS_INVALID 
} FILE_DEV_E_T;
*/
/**********************ffs interface end***********************/
//UDISK represented by
//SD Card Type: E:, F:, G:...
#ifdef DUAL_TCARD_SUPPORT
	#define MMI_SDCARD_MAX_NUM 2
#else
	#define MMI_SDCARD_MAX_NUM 1
#endif

//#define here

#define MMIFILE_DEVICE_LEN                 1
//full path = device + ':\' + dir + '\' + filename
#define MMIFILE_FULL_PATH_MAX_LEN           MMIFMM_FILE_FULL_PATH_MAX_LEN  //Ë«×Ö½ÚÎªµ¥Î»
#define MMIFILE_FILE_NAME_MAX_LEN           MMIFMM_FILE_FILE_NAME_MAX_LEN  //Ë«×Ö½ÚÎªµ¥Î»
#define MMIFILE_DEVICE_NAME_MAX_LEN         7   //Ë«×Ö½ÚÎªµ¥Î»
#define MMIFILE_AUTO_RENAME_SUFIX_MAX_LEN   7   //×Ô¶¯ÖØÃüÃûµÄºó×º³¤¶È
#define MMIFILE_DIR_NAME_MAX_LEN            MMIFMM_FILE_FULL_PATH_MAX_LEN  //Ë«×Ö½ÚÎªµ¥Î»

#define MMIFILE_LONG_FULL_PATH_MAX_LEN      MMIFMM_FILE_FULL_PATH_MAX_LEN  //Ë«×Ö½ÚÎªµ¥Î»
#define MMIFILE_LONG_FILE_NAME_MAX_LEN      MMIFMM_FILE_FILE_NAME_MAX_LEN  //Ë«×Ö½ÚÎªµ¥Î»

#define MMIFILE_FILE_ATTR_LEN               20
#define MMIFILE_SUFFIX_MAX_LEN              10

#define SFS_INVALID_HANDLE                  0
#define MMIFILE_NEW_DEVICE_NAME_MAX_LEN     3
#ifdef WIN32
#define MMIFILE_SEEK_BEGIN      SEEK_SET
#define MMIFILE_SEEK_CUR        SEEK_CUR
#define MMIFILE_SEEK_END        SEEK_END
#define MMIFILE_HANDLE          SFS_HANDLE
#define MMIFILE_ERROR_E         SFS_ERROR_E
#define MMIFILE_OVERLAPPED_T    SFS_OVERLAPPED_T
#define MMIFILE_FIND_DATA_T     SFS_FIND_DATA_T
#else
#define MMIFILE_SEEK_BEGIN      SFS_SEEK_BEGIN
#define MMIFILE_SEEK_CUR        SFS_SEEK_CUR
#define MMIFILE_SEEK_END        SFS_SEEK_END
#define MMIFILE_HANDLE          SFS_HANDLE
#define MMIFILE_ERROR_E         SFS_ERROR_E
#define MMIFILE_OVERLAPPED_T    SFS_OVERLAPPED_T
#define MMIFILE_FIND_DATA_T     SFS_FIND_DATA_T
#endif

// Éè±¸ÀàÐÍ
typedef enum
{
    MMI_DEVICE_SYSTEM,    //CÅÌ£¬²»ÏdeeÔÊ¾¸øÓÃ»§£¬Ö»¹©Ó¦ÓÃ±£´æÅäÖÃÎÄ¼þ
    MMI_DEVICE_UDISK,    // UÅÌ
    MMI_DEVICE_SDCARD,   // ¿¨1
    MMI_DEVICE_SDCARD_1, //¿¨2
    MMI_DEVICE_NUM = MMI_SDCARD_MAX_NUM + 2
} MMIFILE_DEVICE_E;

//#define MMIFILE_CreateFile						 MMIAPIFMM_CreateFile
//#define MMIFILE_DeleteFile						 MMIAPIFMM_DeleteFile
//#define MMIFILE_DeleteDirectory					 MMIAPIFMM_DeleteDirectory
//#define MMIFILE_FindFirstFile					 MMIAPIFMM_FindFirstFile
//#define MMIFILE_FindNextFile					 MMIAPIFMM_FindNextFile
//#define MMIFILE_FindClose						 MMIAPIFMM_FindClose
//#define MMIFILE_GetFileSize						 MMIAPIFMM_GetFileSize
//#define MMIFILE_ReadFile						 MMIAPIFMM_ReadFile
//#define MMIFILE_WriteFile						 MMIAPIFMM_WriteFile
//#define MMIFILE_CloseFile						 MMIAPIFMM_CloseFile
//these functions are defined for PUSH_MAIL
/*****************************************************************************/
//  Description : find nezt file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_ERROR_E MMIFILE_FindNextFile(
                                        MMIFILE_HANDLE file_handle, 
                                        MMIFILE_FIND_DATA_T *fd
                                        );

/*****************************************************************************/
//  Description : create file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_HANDLE MMIFILE_CreateFile(
                                    const wchar *file_name, 
                                    uint32 access_mode,
                                    uint32 share_mode,  //Must be NULL,File System have not realize it 
                                    uint32 file_attri       //Must be NULL,File System have not realize it 
                                    );


/*****************************************************************************/
//  Description : delete file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_ERROR_E MMIFILE_DeleteFile(
                                    const wchar *name,
                                    MMIFILE_OVERLAPPED_T *overlapped_ptr
                                    );

/*****************************************************************************/
//  Description : delete diretory
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_ERROR_E MMIFILE_DeleteDirectory(
                                            const wchar *path
                                            );
/*****************************************************************************/
//  Description : find first file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_HANDLE MMIFILE_FindFirstFile(
                                        const wchar *find, 
                                        uint16 find_len,
                                        MMIFILE_FIND_DATA_T *fd
                                        );

/*****************************************************************************/
//  Description : find close
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_ERROR_E MMIFILE_FindClose(
                                    MMIFILE_HANDLE file_handle
                                    );

/*****************************************************************************/
//  Description : get file size
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
uint32 MMIFILE_GetFileSize(
                            MMIFILE_HANDLE file_handle
                            );

/*****************************************************************************/
//  Description : read file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_ERROR_E MMIFILE_ReadFile(
                                MMIFILE_HANDLE file_handle,
                                void *buffer,
                                uint32 bytes_to_read,
                                uint32 *bytes_read,
                                MMIFILE_OVERLAPPED_T *overlapped
                                );

/*****************************************************************************/
//  Description : write file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_ERROR_E MMIFILE_WriteFile(
                                MMIFILE_HANDLE file_handle,
                                const void *buffer,
                                uint32 bytes_to_write,
                                uint32 *bytes_written,
                                MMIFILE_OVERLAPPED_T *overlapped
                                );

/*****************************************************************************/
//  Description : close file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
MMIFILE_ERROR_E MMIFILE_CloseFile(
                                  MMIFILE_HANDLE file_handle
                                  );

/*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
BOOLEAN MMIFILE_IsFileExist(
                            const wchar *full_path_ptr,     //in
                            uint16       full_path_len       //in, Ë«×Ö½ÚÎªµ¥Î»
                            );

/*****************************************************************************/
//  Description : check whether folder is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
BOOLEAN MMIFILE_IsFolderExist(
                            const wchar     *full_path_ptr,     //in
                            uint16              full_path_len       //in, Ë«×Ö½ÚÎªµ¥Î»
                            );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif //_MMI_FILEMGR_H_
