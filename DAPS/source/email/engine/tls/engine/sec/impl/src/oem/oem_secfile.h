/*=====================================================================================
    FILE NAME: oem_secfile.h
    MODULE NAME: SEC
    
    GENERAL DESCRIPTION:    
        This file declare APIs of file operation.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification                             Tracking
    Date                 Author             Number                    Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-06-01       Alan                                                create oem_secfile.h
    
=====================================================================================*/
#ifndef __OEM_SECFILE_H__
#define __OEM_SECFILE_H__

#include "we_def.h"
#include "oem_secerr.h"
#include "oem_secevent.h"

#ifndef M_SEC_CFG_NO_PLATFORM

#define WE_FILE_OPEN                    SecFile_Open
#define WE_FILE_CLOSE                   SecFile_Close
#define WE_FILE_WRITE                   SecFile_Write
#define WE_FILE_READ                     SecFile_Read
#define WE_FILE_SEEK                      SecFile_Seek
#define WE_FILE_REMOVE                 SecFile_Remove
#define WE_FILE_MKDIR                    SecFile_MakeDir
#define WE_FILE_GETSIZE                 SecFile_GetSize
#define WE_FILE_CHECK_EXIST         SecFile_CheckFileExist

typedef enum{
    E_WE_FILE_SET_RDONLY,
    E_WE_FILE_SET_WRONLY,
    E_WE_FILE_SET_RDWR,
    E_WE_FILE_SET_APPEND,
    E_WE_FILE_SET_CREATE
}E_WE_FILE_MODE;

typedef enum{
    E_WE_FILE_SEEK_SET  ,
    E_WE_FILE_SEEK_CUR,
    E_WE_FILE_SEEK_END
}E_WE_FILE_SEEK_MODE;

typedef enum{
    E_WE_FILE_DIRTYPE,
    E_WE_FILE_FILETYPE
}E_WE_FILE_TYPE;


/*==================================================================================================
    FUNCTION: 
        SecFile_Open
    DESCRIPTION:
        Creates and/or opens a file with the name pszFileName. 
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeHandle is not a valid handle, or name is not a valid path name for a file. 
            or Errror in parameters e.g. mode specifies both E_WE_FILE_SET_RDONLY 
            and E_WE_FILE_SET_WRONLY. 
        E_WE_FILE_ERROR_PATH
            The named file does not exist. For example, the path is null or a component 
            of path does not exist, or the file itself does not exist and E_WE_FILE_SET_CREATE 
            is not set. 
        E_WE_FILE_ERROR_SIZE 
        E_WE_FILE_ERROR_FULL 
        E_WE_FILE_ERROR_EXIST 
            E_WE_FILE_SET_EXCL and E_WE_FILE_SET_CREATE are set and the named file exists. 
    REMARK:
        The open mode must be one E_WE_FILE_MODE type value.
        The file be opened or created is in the WE's directory, its' name should be like
        "file", "/file", "dir/file", "/dir/file", etc, and the name will be converted to lower case.
==================================================================================================*/
E_WE_ERROR  
SecFile_Open
(
    WE_HANDLE hWeHandle,
    WE_CHAR *pszFileName,
    E_WE_FILE_MODE eMode,
    WE_HANDLE *phWeFileHandle
);


/*==================================================================================================
    FUNCTION: 
        SecFile_Close
    DESCRIPTION:
        This function closes the file. 
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
    REMARK:
==================================================================================================*/
E_WE_ERROR 
SecFile_Close 
(
    WE_HANDLE hWeFileHandle
);


/*==================================================================================================
    FUNCTION: 
        SecFile_Write
    DESCRIPTION:
        This function attempts to write size bytes from the buffer. 
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
        E_WE_FILE_ERROR_ACCESS 
            E.g. attempt to write in a file opened as read only. 
        E_WE_ FILE_ERROR_SIZE 
        E_WE_ FILE_ERROR_FULL 
    REMARK:
        This function writes the specified number of bytes to an open file, the actual writen size
        is return by piWriteNum.
==================================================================================================*/
E_WE_ERROR 
SecFile_Write 
(
    WE_HANDLE hWeFileHandle,
    void *pvDataBuf, 
    WE_INT32 iBufSize,
    WE_INT32 *piWriteNum
);


/*==================================================================================================
    FUNCTION: 
        SecFile_Read
    DESCRIPTION:
        This function attempts to read a specified number of bytes from an open file.
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
    REMARK:
        The actual read size is return by piReadNum.

==================================================================================================*/
E_WE_ERROR
SecFile_Read
(
    WE_HANDLE hWeFileHandle,
    void *pvDataBuf,
    WE_INT32 iBufSize,
    WE_INT32 *piReadNum
);

/*==================================================================================================
    FUNCTION: 
        SecFile_Seek
    DESCRIPTION:
        Moves the file pointer of an open file, allowing random access read and write operations.  
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
    REMARK:
        Moves the file pointer of an open file, allowing random access read and write operations. 
        If the seek operation exceeds the limits of the file an error is returned. Offset may be negative. 
==================================================================================================*/
E_WE_ERROR 
SecFile_Seek 
(
    WE_HANDLE hWeFileHandle, 
    WE_INT32 iOffset, 
    E_WE_FILE_SEEK_MODE eSeekMode,
    WE_INT32 *piNewPos
);


/*==================================================================================================
    FUNCTION: 
        SecFile_Remove
    DESCRIPTION:
        This function removes the file identified by fileName.   
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
    REMARK:
        This function removes the file identified by fileName. 
==================================================================================================*/
E_WE_ERROR 
SecFile_Remove 
(
    WE_HANDLE hWeHandle, 
    const WE_CHAR *pszFileName
);

/*==================================================================================================
    FUNCTION: 
        SecFile_MakeDir
    DESCRIPTION:
        This function create a directory name by dirName.   
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
        E_WE_FILE_ERROR_FULL 
        E_WE_FILE_ERROR_PATH 
        E_WE_FILE_ERROR_EXIST

    REMARK:
        This function creates a directory named by dirName. 
==================================================================================================*/
E_WE_ERROR 
SecFile_MakeDir 
(
    WE_HANDLE hWeHandle, 
    const WE_CHAR *pszDirName
);



/*==================================================================================================
    FUNCTION: 
        SecFile_GetSize
    DESCRIPTION:
        This function gets the file size.
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
        E_WE_FILE_ERROR_PATH 
    REMARK:
==================================================================================================*/
E_WE_ERROR
SecFile_GetSize 
(
    WE_HANDLE hWeHandle, 
    const WE_CHAR *pszFileName,
    WE_INT32* piSize
);


/*==================================================================================================
    FUNCTION: 
        SecFile_CheckFileExist
    DESCRIPTION:
        This function check if the file exists.
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
        E_WE_FILE_ERROR_PATH 
    REMARK:
==================================================================================================*/

E_WE_ERROR
SecFile_CheckFileExist(WE_HANDLE hWeHandle, 
                      const WE_CHAR *pszFileName);


/*==================================================================================================
    FUNCTION: 
        SecFile_CloseAll
    DESCRIPTION:
        This function closes all files associated with hWeHandle.
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
        E_WE_FILE_ERROR_PATH 
    REMARK:
==================================================================================================*/
E_WE_ERROR
SecFile_CloseAll 
(
    WE_HANDLE hWeHandle
);
#else

#endif/*#ifdef M_SEC_CFG_NO_PLATFORM*/

#endif 

