/*==============================================================================
    FILE NAME : oem_secfile.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION:
        This File will gather functions that handle files.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History
    Modification Tracking
    Date               Author               Number      Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-10-24    HouGuanhua        none         Initialization
    2007-06-28              bluefire             after malloc,memset it
   
==================================================================================================*/


/*==================================================================================================
    Include File Section
==================================================================================================*/

// #include "oem_seclog.h"
#include "we_cfg.h"
#include "we_def.h"
#include "oem_sechandle.h"
#include "oem_secfile.h"
#include "sec_ast.h"
#if 0
#include "stdio.h"
#include <string.h>
#include "sec_cfg.h"
#ifndef M_SEC_CFG_NO_PLATFORM

#if 1

#include <stdlib.h>
#include "we_def.h"
#include "we_cfg.h"
#include "we_log.h"
#include "we_mem.h"
#include "tf3rd_for_HZ.h"

#else

#include "io.h"
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include "direct.h"


#endif
#endif

#if 0
       
//#include <errno.h>
//#include <dirent.h> /* readdir */

/*==================================================================================================
* Macro Define Section
==================================================================================================*/
#define WE_FILE_PATH "../shared/"
//#define WE_FILE_PATH "file:/root/workspace/We_File/Debug/shared/we/"

/*==================================================================================================
    FUNCTION: 
            WeFile_ConvertPathWeToBrew
    DESCRIPTION:
        Add WE and BREW prefix string, and convert to lower case.
    RETURN VALUE:
        Zero terminated WE_CHAR string. 
        NULL, if error.     
    REMARK:
        The parameter pcFileName should be like "file", "/file", "dir/file", "/dir/file", etc.
        The string returned must be freed by user.
==================================================================================================*/
WE_CHAR * SecFile_ConvertPath
(
 const WE_CHAR *pcWeFileName
 )
{
    WE_UINT8 ucFileNameLen = 0;
    WE_CHAR *pcWeName = NULL;
    WE_CHAR *pcEntireName = NULL;
    
    if ( NULL == pcWeFileName )
    {
        return NULL;
    }
    pcWeName = (WE_CHAR*)pcWeFileName;      /* save name pointer */
    
    if ('/' == pcWeName[0])
    {
        pcWeName++;     /*skip "/" */
        
    }
    if ('\0' == pcWeName[0])
    {
        ucFileNameLen = strlen(WE_FILE_PATH);
    }
    else 
    {
        ucFileNameLen = (WE_UINT8)( strlen(WE_FILE_PATH)+strlen(pcWeName) );
    }
    
    pcEntireName = (WE_CHAR *)malloc(ucFileNameLen+1);
    if(NULL == pcEntireName)
    {
        return NULL;
    }
    WE_MEMSET(pcEntireName, 0, ucFileNameLen+1);
    WE_STRCPY(pcEntireName, WE_FILE_PATH);
    {
        WE_STRCAT( pcEntireName, pcWeName );
    }
    
    return pcEntireName;
} 



/*==================================================================================================
    FUNCTION: 
        static FileSeekType SecFile_ConvertSeekMode(E_WE_FILE_SEEK_MODE iSeekMode)
    DESCRIPTION:
    RETURN VALUE:
    REMARK:
==================================================================================================*/
static WE_INT32 SecFile_ConvertSeekMode
(
 E_WE_FILE_SEEK_MODE eSeekMode
)
{
    WE_INT32 iSeekType = SEEK_SET;
    
    switch(eSeekMode)
    {
    case E_WE_FILE_SEEK_SET:
        iSeekType = SEEK_SET;
        break;
    case E_WE_FILE_SEEK_CUR:
        iSeekType = SEEK_CUR;
        break;
    case E_WE_FILE_SEEK_END:
        iSeekType = SEEK_END;
        break;
    default:
        break;
    }
    return iSeekType;
}



/*==================================================================================================
    FUNCTION: 
        static E_WE_ERROR WeFile_ConvertResult(WE_INT32 iResult)
    DESCRIPTION:
    RETURN VALUE:
    REMARK:
==================================================================================================*/
static E_WE_ERROR WeFile_ConvertResult
(
 WE_INT32 iResult
)
{
#if 0
    E_WE_ERROR eReturn = (E_WE_ERROR)-1;
    
    switch(iResult)
    {
    case EACCES: /*The requested access to the file is not allowed, or 
                 search permission is denied for one of the directories in the path 
                 prefix of pathname, or the file did not exist yet and write access 
        to the parent directory is not allowed.*/
    case EROFS: /*pathname refers to a file on a read-only filesystem 
            and write access was requested.*/
    case ETXTBSY: /*pathname refers to an executable image which is currently being executed 
                and write access was requested.*/                                  
        eReturn = E_WE_FILE_ERROR_ACCESS;
        break;
                
    case EEXIST: /*pathname already exists and O_CREAT and O_EXCL were used*/
        
        eReturn = E_WE_FILE_ERROR_EXIST;
        break;
        
    case ENAMETOOLONG: /*pathname was too long.*/
    case EFAULT: /*pathname points outside your accessible address space.*/
    case EBADF: /*fd isn't a valid _open file descriptor*/
        //        case EINTR: /*The close() call was interrupted by a signal.*/
    case EIO: /*An I/O error occurred.*/
    case EINVAL: /*invalid option parameter*/
    case EPERM: /*The filesystem does not allow unlinking of files.*/
        eReturn = E_WE_FILE_ERROR_INVALID;
    break;

    case ENFILE: /*The process already has the maximum number of files _open*/
    case ENOMEM: /*Insufficient kernel memory was available.*/
    case ELOOP: /*Too many symbolic links were encountered in resolving pathname, 
        or O_NOFOLLOW was specified but pathname was a symbolic link.*/
    case ENOSPC: /*pathname was to be created 
                         but the device containing pathname has no room for the new file.*/   
    case EFBIG: /*An attempt was made to write a file that exceeds the implementation-defined maximum file size 
                 or the process' file size limit, or to write at a position past than the maximum allowed offset.*/
    case EOVERFLOW: /*The resulting file offset cannot be represented in an off_t.*/                              
         eReturn = E_WE_FILE_ERROR_FULL;
         break; 
                             
    case ENOENT: /*O_CREAT is not set and the named file does not exist. 
        Or, a directory component in pathname does not exist or is a dangling symbolic link.*/
    case ENXIO: /*the file is a device special file and no corresponding device exists.*/  
    case EISDIR: /*fd refers to a directory.*/  
    case ENOTDIR: /*A component used as a directory in pathname is not, in fact, a directory.*/        
        eReturn = E_WE_FILE_ERROR_PATH;
        break;
                                 
    default:
        eReturn = E_WE_ERROR_OTHER;
        break;
    }
    
    return eReturn;
#else
    return iResult;
#endif
}



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
        The _open mode must be one E_WE_FILE_MODE type value.
        The file be opened or created is in the WE's directory, its' name should be like
        "file", "/file", "dir/file", "/dir/file", etc, and the name will be converted to lower case.
==================================================================================================*/
E_WE_ERROR  
SecFile_Open
(
 WE_HANDLE hWeHandle,
 WE_CHAR *pszWeFileName,
 E_WE_FILE_MODE eMode,
 WE_HANDLE *phWeFileHandle
 )
{
    St_WeFileHandle *pstFileHandle = NULL;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_CHAR *pszFileName = NULL;       
    WE_INT32 i=0;
    WE_INT32 iError = 0;
    
    
    if((NULL == hWeHandle)
        ||(NULL == pszWeFileName)
        ||(NULL == phWeFileHandle))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    if(WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    /*find an empty file handle */
    for(i=0; i<OPEN_FILE_MAX_NUM;i++)
    {
        if(NULL == ((St_WeHandle*) hWeHandle)->aphFileList[i])
        {
            break;
        }
    }
    if(OPEN_FILE_MAX_NUM == i)
    {
        return E_WE_FILE_ERROR_FULL;
    }
    
    pstFileHandle = (St_WeFileHandle*)malloc(sizeof(St_WeFileHandle));
    if(NULL == pstFileHandle)
    {
        return E_WE_ERROR_RESOURCE_LIMIT;
    }
    pstFileHandle->iMagicNum = WE_FILE_HANDLE_MAGIC_NUM;
    pstFileHandle->hWeHandle = hWeHandle;
    pstFileHandle->iFileHandleID = i;
    pstFileHandle->iFd = -1;
    
    /* convert file name */
    pszFileName = SecFile_ConvertPath(pszWeFileName);
    if(NULL == pszFileName)
    {
        free(pstFileHandle);
        return E_WE_ERROR_RESOURCE_LIMIT;
    }      
    
    switch(eMode)
    {
    case E_WE_FILE_SET_RDONLY:
        pstFileHandle->iFd = (WE_INT32)_open( pszFileName, _O_RDONLY );
        break;
        
    case E_WE_FILE_SET_WRONLY:
        pstFileHandle->iFd = (WE_INT32)_open( pszFileName, _O_WRONLY );
        break;
        
    case E_WE_FILE_SET_RDWR:
        pstFileHandle->iFd = (WE_INT32)_open( pszFileName, _O_RDWR );
        break;
        
    case E_WE_FILE_SET_CREATE:
        pstFileHandle->iFd = (WE_INT32)_open( pszFileName, _O_CREAT | _O_RDWR, 0777 );
        break;
        
    case E_WE_FILE_SET_APPEND:
        pstFileHandle->iFd = (WE_INT32)_open( pszFileName, _O_APPEND );
        break;
        
    default:
        pstFileHandle->iFd = (WE_INT32)NULL;
        break;
    }
    
    if (-1 == pstFileHandle->iFd)
    {
        iError = errno;
        eResult = WeFile_ConvertResult( iError );
        free(pstFileHandle);
    }
    else
    {
        /* save WeFileHandle in WeHandle */
        ((St_WeHandle*) hWeHandle)->aphFileList[i] = pstFileHandle;
        *phWeFileHandle = pstFileHandle;
        eResult = E_WE_OK;
    }

    free(pszFileName);    
    return eResult ;
}



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
 )
{
    St_WeHandle* hWeHandle = NULL;
    WE_INT32 iFileHandleID = 0;
    WE_INT32 iFd = (WE_INT32)NULL;
    WE_INT32 iError = 0;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iRet = 0;
    
    if(NULL == hWeFileHandle)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    if(WE_FILE_HANDLE_MAGIC_NUM  != ((St_WeFileHandle*)hWeFileHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    hWeHandle = (St_WeHandle*)((St_WeFileHandle*)hWeFileHandle)->hWeHandle;
    iFileHandleID = ((St_WeFileHandle*)hWeFileHandle)->iFileHandleID;
    iFd = ((St_WeFileHandle*)hWeFileHandle)->iFd;
    
    ((St_WeFileHandle*)hWeFileHandle)->iMagicNum = 0;
    ((St_WeFileHandle*)hWeFileHandle)->hWeHandle = NULL;
    ((St_WeFileHandle*)hWeFileHandle)->iFileHandleID = -1;
    
    if (0 != iFd)
    {
        iRet = close( iFd );
        if (0 == iRet)
        {
            eResult = E_WE_OK;
        }
        else
        {
            iError = errno;
            eResult = WeFile_ConvertResult( iError );
        }
    }
    free( hWeFileHandle );
    hWeFileHandle = NULL;
    /* clear WeFileHandle record in WeHandle */
    hWeHandle->aphFileList[iFileHandleID] = NULL;
    
    return eResult;
}



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
        This function writes the specified number of bytes to an _open file, the actual writen size
        is return by piWriteNum.
==================================================================================================*/
E_WE_ERROR
SecFile_Write
(
 WE_HANDLE hWeFileHandle,
 void *pvDataBuf,
 WE_INT32 iBufSize,
 WE_INT32 *piWriteNum
 )
{
    WE_INT32 iFd = (WE_INT32)NULL;
    WE_INT32 iError = 0;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;    
    
    if((NULL == hWeFileHandle)
        ||(NULL == pvDataBuf)
        ||(iBufSize<= 0 )
        ||(NULL ==piWriteNum))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    
    if (WE_FILE_HANDLE_MAGIC_NUM  != ((St_WeFileHandle*)hWeFileHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    iFd = ((St_WeFileHandle*)hWeFileHandle)->iFd;
    
    *piWriteNum = (WE_INT32)write( iFd, pvDataBuf, (size_t)iBufSize );
    if (-1 == (*piWriteNum))
    {    
        iError = errno;
        eResult = WeFile_ConvertResult( iError );
    }
    else
    {
        eResult = E_WE_OK;
    }
    
    return eResult;
}



/*==================================================================================================
    FUNCTION:
        SecFile_Read
    DESCRIPTION:
        This function attempts to read a specified number of bytes from an _open file.
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
 )
{
    WE_INT32 iFd = (WE_INT32)NULL;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iError = 0;
    
    if((NULL == hWeFileHandle)
        ||(NULL == pvDataBuf)
        ||(iBufSize <= 0 )
        ||(NULL == piReadNum))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    
    if(WE_FILE_HANDLE_MAGIC_NUM  != ((St_WeFileHandle*)hWeFileHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    iFd = ((St_WeFileHandle*)hWeFileHandle)->iFd;
    
    *piReadNum = read( iFd, pvDataBuf, iBufSize );
    if( -1 == *piReadNum )
    {
        iError = errno;
        eResult = WeFile_ConvertResult( iError );      
    }
    else if (0 == *piReadNum)
    {
        eResult =  E_WE_FILE_ERROR_EOF;
    }
    else
    {
        eResult =  E_WE_OK;
    }
    
    return eResult;
}



/*==================================================================================================
    FUNCTION:
        SecFile_Seek
    DESCRIPTION:
        Moves the file pointer of an _open file, allowing random access read and write operations.
        RETURN VALUE:
    Possible error codes are:
        E_WE_OK
        successful
        E_WE_ERROR_BAD_HANDLE
        hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
        hWeFileHandle is not a valid handle, or name is not a valid path name for a file.
    REMARK:
        Moves the file pointer of an _open file, allowing random access read and write operations.
        If the seek operation exceeds the limits of the file an error is returned. Offset may be negative.
==================================================================================================*/
E_WE_ERROR
SecFile_Seek
(
 WE_HANDLE hWeFileHandle,
 WE_INT32 iOffset,
 E_WE_FILE_SEEK_MODE eSeekMode,
 WE_INT32 *piNewPos
 )
{
    WE_INT32 iFd = (WE_INT32)NULL;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iSeekType = 0;
    WE_INT32 iRet = 0;
    WE_INT32 iError = 0;
    
    if ((NULL == hWeFileHandle)
        ||(NULL == piNewPos))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    
    if(WE_FILE_HANDLE_MAGIC_NUM  != ((St_WeFileHandle*)hWeFileHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    iFd = ((St_WeFileHandle*)hWeFileHandle)->iFd;
    iSeekType = SecFile_ConvertSeekMode(eSeekMode);
    
    iRet = lseek( iFd, iOffset, SEEK_SET );
    if( -1 == iRet )
    {
        iError = errno;
        eResult = WeFile_ConvertResult( iError );
    }
    else        /* eResult equal to SUCCESS or file pointer position*/
    {
        eResult = E_WE_OK;      
        *piNewPos = iRet;     
    }
    return eResult;
}



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
 const WE_CHAR *pszWeFileName
 )
{
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT iRet = 0;
    WE_INT32 iError = 0;
    WE_CHAR *pszBrewDirName = NULL;
    
    if ((NULL == hWeHandle)
        ||(NULL == pszWeFileName))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    if(WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    pszBrewDirName = SecFile_ConvertPath(pszWeFileName);
    iRet = unlink(pszBrewDirName);
    if(0 == iRet)
    {
        eResult = E_WE_OK;
    }
    else
    {
        iError = errno;
        eResult = WeFile_ConvertResult( iError );
    }
    free(pszBrewDirName);        
    return eResult;
}



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
 const WE_CHAR *pszWeDirName
 )
{
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iRet = 0;
    WE_INT32 iError = 0;
    const WE_CHAR *pcTail = NULL;
    const WE_CHAR *pcTmpDir = NULL;
    WE_CHAR acTmpDir[200] = {0};
    WE_CHAR *pszBrewDirName = NULL;
    if ((NULL == hWeHandle)
        ||(NULL == pszWeDirName))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    if(WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    pszBrewDirName = SecFile_ConvertPath(pszWeDirName);
    pcTail = pcTmpDir = '/' == pszBrewDirName[0] ? pszBrewDirName + 1 : pszBrewDirName;

    while ((pcTail = strchr( pcTail, '/' )) != NULL)
    {
        pcTail++;
        WE_MEMCPY( acTmpDir, pcTmpDir, pcTail - pcTmpDir );
        acTmpDir[pcTail - pcTmpDir] = '\0';
        iRet = _mkdir( acTmpDir); /* mode ?*/
        WE_LOG_MSG((0,0,"acTmpDir is %s iRet is %d\n",acTmpDir,iRet));
        if (-1 == iRet)    
        {
            iError = errno;
            eResult = WeFile_ConvertResult( iError );
            if (eResult != E_WE_FILE_ERROR_EXIST)
            {
                free(pszBrewDirName);    
                return eResult;
            }
        }
    } 
    /*added by Bird 070323*/
    iRet = _mkdir( pcTmpDir); /* mode ?*/
    WE_LOG_MSG((0,0,"_mkdir path is %s ,ret is %d\n",pcTmpDir,iRet));
    if (-1 == iRet)    
    {
        iError = errno;
        eResult = WeFile_ConvertResult( iError );
        if (eResult != E_WE_FILE_ERROR_EXIST)
        {
            free(pszBrewDirName);    
            return eResult;
        }
    }

    eResult = E_WE_OK;
    free(pszBrewDirName);    
    return eResult;
}


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
 )
{
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iError = 0;
    WE_INT32 iRet = 0;
    struct stat stStat = {0};
    WE_CHAR *pszBrewDirName = NULL;
    
    if ((NULL == hWeHandle)
        ||(NULL == pszFileName)
        ||(NULL == piSize))
    {
        return eResult;
    }
    
    if (WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        eResult = E_WE_ERROR_BAD_HANDLE;
        return eResult;
    }
    /* convert file name */
    pszBrewDirName = SecFile_ConvertPath(pszFileName);
    iRet = stat( pszBrewDirName, &stStat );
    if (0 == iRet)
    {
        *piSize = stStat.st_size;
        eResult = E_WE_OK;
    }
    else
    {
        iError = errno;
        eResult = WeFile_ConvertResult( iError );
    }
    
    free(pszBrewDirName);     
    return eResult;
}



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
 )
{
    St_WeFileHandle* hWeFileHandle = NULL;
    WE_INT32 i;
    
    if(NULL == hWeHandle)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    if(WE_HANDLE_MAGIC_NUM  != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    for(i = 0; i < OPEN_FILE_MAX_NUM; i++)
    {
        hWeFileHandle = (St_WeFileHandle*)((St_WeHandle*)hWeHandle)->aphFileList[i];
        SecFile_Close(hWeFileHandle);
    }
    
    return E_WE_OK;
}

/*==================================================================================================
    FUNCTION: 
        SecFile_CheckFileExist
    CREATE DATE:
        2006-07-22
    AUTHOR:
        Bird
    DESCRIPTION: 
        This function check that the file is exist or not.
    ARGUMENTS PASSED:
        WE_HANDLE hWeHandle: Pointer to the IFileMgr Interface object
        WE_CHAR *pszFileName: file name
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
        successful
        E_WE_ERROR_BAD_HANDLE
        hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
        hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
        E_WE_FILE_ERROR_PATH 
    USED GLOBAL VARIABLES:
        None
    USED STATIC VARIABLES:
        None
    CALL BY:
        Omit
    IMPORTANT NOTES:
        None
==================================================================================================*/

E_WE_ERROR
SecFile_CheckFileExist(WE_HANDLE hWeHandle, 
                      const WE_CHAR *pszFileName)
{   
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iRes = 0;
    WE_CHAR *pcEntireFileName = NULL;
    struct stat statBuf;
    
    if((NULL == pszFileName)||NULL == hWeHandle)
    {
        return eResult;
    }
    
    if(WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        eResult = E_WE_ERROR_BAD_HANDLE;
        return eResult;
    }
    pcEntireFileName = SecFile_ConvertPath( pszFileName );
    iRes = stat(pcEntireFileName,&statBuf);
    free(pcEntireFileName);
    if(E_WE_OK != iRes)
    {
        eResult = WeFile_ConvertResult( iRes );
        return eResult;
    }
    
    return E_WE_OK; 
    
}
#else

#define WE_FILE_PATH "we/"

/*==================================================================================================
    FUNCTION: 
            WeFile_ConvertPathWeToBrew
    DESCRIPTION:
        Add WE and BREW prefix string, and convert to lower case.
    RETURN VALUE:
        Zero terminated WE_CHAR string. 
        NULL, if error.     
    REMARK:
        The parameter pcFileName should be like "file", "/file", "dir/file", "/dir/file", etc.
        The string returned must be freed by user.
==================================================================================================*/
WE_CHAR * SecFile_ConvertPath
(
    const WE_CHAR *pcWeFileName
)
{
    WE_UINT8 ucFileNameLen = 0;
    WE_CHAR *pcWeName = NULL;
    WE_CHAR *pcEntireName = NULL;

    if ( NULL == pcWeFileName )
    {
        return NULL;
    }
    pcWeName = (WE_CHAR*)pcWeFileName;      /* save name pointer */

    if ('/' == pcWeName[0])
    {
        pcWeName++;     /*skip "/" */
        
    }
    if ('\0' == pcWeName[0])
    {
        ucFileNameLen = SEC_STRLEN(WE_FILE_PATH);
    }
    else 
    {
        ucFileNameLen = (WE_UINT8)( SEC_STRLEN(WE_FILE_PATH)+SEC_STRLEN(pcWeName) );
    }
    
    pcEntireName = (WE_CHAR *)WE_MALLOC(ucFileNameLen+1);
    if(NULL == pcEntireName)
    {
        return NULL;
    }
    WE_MEMSET(pcEntireName, 0, ucFileNameLen+1);
    WE_STRCPY(pcEntireName, WE_FILE_PATH);
    if ('\0' != pcWeName[0])
    {
        WE_STRCAT( pcEntireName, pcWeName );
    }

    return pcEntireName;
} 



/*==================================================================================================
    FUNCTION: 
            static FileSeekType SecFile_ConvertSeekMode(E_WE_FILE_SEEK_MODE iSeekMode)
    DESCRIPTION:
    RETURN VALUE:
    REMARK:
==================================================================================================*/
static WE_INT32 SecFile_ConvertSeekMode
(
    E_WE_FILE_SEEK_MODE eSeekMode
)
{
    WE_INT32 iSeekType = SEEK_SET;

    switch(eSeekMode)
    {
        case E_WE_FILE_SEEK_SET:
            iSeekType = SEEK_SET;
            break;
        case E_WE_FILE_SEEK_CUR:
            iSeekType = SEEK_CUR;
            break;
        case E_WE_FILE_SEEK_END:
            iSeekType = SEEK_END;
            break;
        default:
            break;
    }
    return iSeekType;
}



/*==================================================================================================
    FUNCTION: 
            static E_WE_ERROR WeFile_ConvertResult(WE_INT32 iResult)
    DESCRIPTION:
    RETURN VALUE:
    REMARK:
==================================================================================================*/

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
    WE_CHAR *pszWeFileName,
    E_WE_FILE_MODE eMode,
    WE_HANDLE *phWeFileHandle
)
{
    St_WeFileHandle *pstFileHandle = NULL;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_CHAR *pszFileName = NULL;    
    WE_CHAR *pcName = NULL;
    WE_CHAR *pcPath = NULL;    
    WE_INT32 i=0;
    
    if((NULL == hWeHandle)
      ||(NULL == pszWeFileName)
      ||(NULL == phWeFileHandle))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    if(WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }

    /*find an empty file handle */
    for(i=0; i<OPEN_FILE_MAX_NUM;i++)
    {
        if(NULL == ((St_WeHandle*) hWeHandle)->aphFileList[i])
        {
          break;
        }
    }
    if(OPEN_FILE_MAX_NUM == i)
    {
        return E_WE_FILE_ERROR_FULL;
    }

    pstFileHandle = (St_WeFileHandle*)WE_MALLOC(sizeof(St_WeFileHandle));
    if(NULL == pstFileHandle)
    {
        return E_WE_ERROR_RESOURCE_LIMIT;
    }
    (void)WE_MEMSET(pstFileHandle,0,sizeof(St_WeFileHandle));
    pstFileHandle->iMagicNum = WE_FILE_HANDLE_MAGIC_NUM;
    pstFileHandle->hWeHandle = hWeHandle;
    pstFileHandle->iFileHandleID = i;
    pstFileHandle->hFd = NULL;

    /* convert file name */
    pszFileName = SecFile_ConvertPath(pszWeFileName);
    if(NULL == pszFileName)
    {
        WE_FREE(pstFileHandle);
        return E_WE_ERROR_RESOURCE_LIMIT;
    }

    switch(eMode)
    {
      case E_WE_FILE_SET_RDONLY:
         pstFileHandle->hFd = (WE_HANDLE)TF3RD_FOPEN( (const WE_CHAR * )pszFileName, (const WE_UCHAR * )"r" );
         break;

      case E_WE_FILE_SET_WRONLY:
         pstFileHandle->hFd = (WE_HANDLE)TF3RD_FOPEN( (const WE_CHAR * )pszFileName, (const WE_UCHAR * )"w" );
         break;

      case E_WE_FILE_SET_RDWR:
         pstFileHandle->hFd = (WE_HANDLE)TF3RD_FOPEN( (const WE_CHAR * )pszFileName, (const WE_UCHAR * )"r+" );
         break;

      case E_WE_FILE_SET_CREATE:
         pcName = WE_STRRCHR( pszFileName, '/' );
         if (NULL == pcName)
         {
            return E_WE_FILE_ERROR_PATH;
         }
         pcName++;
         pcPath = (WE_CHAR *)WE_MALLOC( pcName - pszFileName + 1 );
         if (NULL == pcPath)
         {
             return E_WE_ERROR_RESOURCE_LIMIT;
         }
         WE_MEMCPY( pcPath, pszFileName, pcName - pszFileName );
         pcPath[pcName - pszFileName] = '\0';
         eResult = SecFile_MakeDir( hWeHandle, pcPath );
         WE_FREE( pcPath );
         if (E_WE_OK != eResult && eResult != E_WE_FILE_ERROR_EXIST)
         {
             return eResult;
         }
         
         pstFileHandle->hFd = (WE_HANDLE)TF3RD_FOPEN( (const WE_CHAR * )pszFileName, (const WE_UCHAR * )"w+" );
         /*
         if (pstFileHandle->iFd != -1)
         {
             iRet = fchmod(pstFileHandle->iFd, 0777);
             if (-1 == iRet)
             {
                close(pstFileHandle->iFd);
                unlink(pszFileName);
                pstFileHandle->iFd = -1;                                    
             }
         }*/
         break;

      case E_WE_FILE_SET_APPEND:
         WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM,0,"SecFile_Open()___ append"));
         pstFileHandle->hFd = (WE_HANDLE)TF3RD_FOPEN( (const WE_CHAR * )pszFileName, (const WE_UCHAR * )"a" );
         break;

      default:
         pstFileHandle->hFd = NULL;
         break;
    }

    if (NULL == pstFileHandle->hFd)
    {
        WE_LOGERROR(("SecFile_Open()__________open error!"));
        WE_FREE(pstFileHandle);
    }
    else
    {
        /* save WeFileHandle in WeHandle */
        ((St_WeHandle*) hWeHandle)->aphFileList[i] = pstFileHandle;
        *phWeFileHandle = pstFileHandle;
        eResult = E_WE_OK;
    }

    WE_FREE(pszFileName);
    return eResult ;
}

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
)
{
    St_WeHandle* hWeHandle = NULL;
    WE_INT32 iFileHandleID = 0;
    WE_HANDLE hFd = NULL;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iRet = 0;

    if(NULL == hWeFileHandle)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }

    if(WE_FILE_HANDLE_MAGIC_NUM  != ((St_WeFileHandle*)hWeFileHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }

    hWeHandle = (St_WeHandle*)((St_WeFileHandle*)hWeFileHandle)->hWeHandle;
    iFileHandleID = ((St_WeFileHandle*)hWeFileHandle)->iFileHandleID;
    hFd = ((St_WeFileHandle*)hWeFileHandle)->hFd;

    ((St_WeFileHandle*)hWeFileHandle)->iMagicNum = 0;
    ((St_WeFileHandle*)hWeFileHandle)->hWeHandle = NULL;
    ((St_WeFileHandle*)hWeFileHandle)->iFileHandleID = -1;

    if (0 != hFd)
    {
        iRet = TF3RD_FCLOSE( hFd );
        if (0 == iRet)
        {
            eResult = E_WE_OK;
        }
        else
        {
            eResult = E_WE_ERROR_OTHER;
        }
    }
    WE_FREE( hWeFileHandle );
    hWeFileHandle = NULL;
    /* clear WeFileHandle record in WeHandle */
    hWeHandle->aphFileList[iFileHandleID] = NULL;

    return eResult;
}

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
)
{
    WE_HANDLE hFd = NULL;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;    

    if((NULL == hWeFileHandle)
      ||(NULL == pvDataBuf)
      ||(iBufSize<= 0 )
      ||(NULL ==piWriteNum))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }

    if (WE_FILE_HANDLE_MAGIC_NUM  != ((St_WeFileHandle*)hWeFileHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }

    hFd = ((St_WeFileHandle*)hWeFileHandle)->hFd;

    *piWriteNum = (WE_INT32)TF3RD_FWRITE( pvDataBuf, 1, iBufSize, hFd);
    if (-1 == (*piWriteNum))
    {       
        eResult = E_WE_ERROR_OTHER;
    }
    else
    {
        eResult = E_WE_OK;
    }
    
    return eResult;
}


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
)
{
    WE_HANDLE hFd = NULL;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;

    if((NULL == hWeFileHandle)
      ||(NULL == pvDataBuf)
      ||(iBufSize <= 0 )
      ||(NULL == piReadNum))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }

    if(WE_FILE_HANDLE_MAGIC_NUM  != ((St_WeFileHandle*)hWeFileHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }

    hFd = ((St_WeFileHandle*)hWeFileHandle)->hFd;

    *piReadNum = TF3RD_FREAD( pvDataBuf, 1, iBufSize, hFd );
    if( -1 == *piReadNum )
    {
        eResult = E_WE_ERROR_OTHER;
        WE_LOGERROR(( "SecFile_Read()_____read error! "));        
    }
    else if (0 == *piReadNum)
    {
        eResult =  E_WE_FILE_ERROR_EOF;
    }
    else
    {
        eResult =  E_WE_OK;
    }
    
    return eResult;
}


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
)
{
    WE_HANDLE hFd = NULL;
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iSeekType = 0;
    WE_INT32 iRet = 0;

    if ((NULL == hWeFileHandle)
      ||(NULL == piNewPos))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }

    if(WE_FILE_HANDLE_MAGIC_NUM  != ((St_WeFileHandle*)hWeFileHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }

    hFd = ((St_WeFileHandle*)hWeFileHandle)->hFd;
    iSeekType = SecFile_ConvertSeekMode(eSeekMode);

    iRet = TF3RD_FSEEK( hFd, iOffset, iSeekType );
    if( -1 == iRet )
    {
        eResult = E_WE_ERROR_OTHER;
    }
    else        /* eResult equal to SUCCESS or file pointer position*/
    {
        eResult = E_WE_OK;      
        *piNewPos = iRet;     
    }
    return eResult;
}

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
    const WE_CHAR *pszWeFileName
)
{
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT iRet = 0;

    if ((NULL == hWeHandle)
      ||(NULL == pszWeFileName))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    if(WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
      return E_WE_ERROR_BAD_HANDLE;
    }

    iRet = TF3RD_REMOVE(pszWeFileName);
    if(0 == iRet)
    {
        eResult = E_WE_OK;
    }
    else
    {
        eResult = E_WE_ERROR_OTHER;
    }

    return eResult;
}



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
    const WE_CHAR *pszWeDirName
)
{
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iRet = 0;
    const WE_CHAR *pcTail = NULL;
    const WE_CHAR *pcTmpDir = NULL;
    WE_CHAR acTmpDir[200] = {0};

    if ((NULL == hWeHandle)
      ||(NULL == pszWeDirName))
    {
        return E_WE_ERROR_INVALID_PARAM;
    }
    if(WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM,0, "SecFile_MakeDir()___ pszWeDirName = %s", pszWeDirName));
    //pcTail = pcTmpDir = '/' == pszWeDirName[0] ? pszWeDirName + 1 : pszWeDirName;
    pcTail = pcTmpDir = pszWeDirName;
    /*add by zhangyin*/
    if (NULL != TF3RD_OPENDIR(pcTmpDir))
    {
        eResult = E_WE_FILE_ERROR_EXIST;
        return eResult;
    }
    
    if ('/' == pszWeDirName[0])
    {
        pcTail++;
        pcTail = WeScl_Strchr( pcTail, '/' );
        pcTail++;
        WE_MEMCPY( acTmpDir, pcTmpDir, pcTail - pcTmpDir );
        acTmpDir[pcTail - pcTmpDir] = '\0';
        
        iRet = TF3RD_MAKEDIR( acTmpDir); /* mode ?*/
        /*if (-1 == iRet)
        {
            
            WAP_LOG_PRINT((E_WAP_DBG_FILE,E_WAP_DBG_LEVEL_LOW)("WeFile_MakeDir()_____error!");)
            eResult = E_WE_ERROR_BASE;
            return eResult;
            //}
        }*/
    }
    while ((pcTail = WeScl_Strchr( pcTail, '/' )) != NULL)
    {
        pcTail++;
        WE_MEMCPY( acTmpDir, pcTmpDir, pcTail - pcTmpDir );
        acTmpDir[pcTail - pcTmpDir] = '\0';
        
        iRet = TF3RD_MAKEDIR( acTmpDir); /* mode ?*/
        /*if (-1 == iRet)
        {
            WAP_LOG_PRINT((E_WAP_DBG_FILE,E_WAP_DBG_LEVEL_LOW)("WeFile_MakeDir()_____error!");)
            eResult = E_WE_ERROR_BASE;
            return eResult;
        }*/
        
    }

    if (-1 == iRet)
    {
        eResult = E_WE_ERROR_OTHER;
        return eResult;
    }    

    eResult = E_WE_OK;
    return eResult;
}


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
    const WE_CHAR *pszWeFileName,
    WE_INT32* piSize
)
{
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_INT32 iRet = 0;
    WE_CHAR *pszFileName = NULL;
    WE_HANDLE hFd = NULL;
    WE_UINT32 uiSize = 0;

    if ((NULL == hWeHandle)
        ||(NULL == pszWeFileName)
        ||(NULL == piSize))
    {
        return eResult;
    }
    
    if (WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        eResult = E_WE_ERROR_BAD_HANDLE;
        return eResult;
    }

    WE_LOG_MSG((WE_LOG_DETAIL_MEDIUM,0, "SecFile_Open()___ pszWeFileName = %s", pszWeFileName));
    pszFileName = SecFile_ConvertPath(pszWeFileName);
    if(NULL == pszFileName)
    {
        return E_WE_ERROR_RESOURCE_LIMIT;
    }

    hFd = (WE_HANDLE)TF3RD_FOPEN( (const WE_CHAR * )pszFileName, (const WE_UCHAR * )"r" );
    if (NULL == hFd)
    {
        WE_FREE(pszFileName);
        return E_WE_ERROR_RESOURCE_LIMIT;
    }

    iRet = TF3RD_STAT( hFd, &uiSize);
    if (0 == iRet)
    {
        *piSize = (WE_INT32)uiSize;
        eResult = E_WE_OK;
        TF3RD_FCLOSE( hFd );
    }
    else
    {
        eResult = E_WE_ERROR_OTHER;
    }
    
    WE_FREE(pszFileName);
    return eResult;
}



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
)
{
    St_WeFileHandle* hWeFileHandle = NULL;
    WE_INT32 i;
    
    if(NULL == hWeHandle)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }
    
    if(WE_HANDLE_MAGIC_NUM  != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        return E_WE_ERROR_BAD_HANDLE;
    }

    for(i = 0; i < OPEN_FILE_MAX_NUM; i++)
    {
        hWeFileHandle = (St_WeFileHandle*)((St_WeHandle*)hWeHandle)->aphFileList[i];
        SecFile_Close(hWeFileHandle);
    }
    
    return E_WE_OK;
}

/*==================================================================================================
    FUNCTION: 
        SecFile_CheckFileExist
    DESCRIPTION:
        test the file exist or not.   
    RETURN VALUE:
        Possible error codes are: 
        E_WE_OK
            successful
        E_WE_ERROR_BAD_HANDLE
            hWeFileHandle is a disable handle.
        E_WE_ERROR_INVALID_PARAM
            hWeFileHandle is not a valid handle, or name is not a valid path name for a file. 
    REMARK:
        none. 
==================================================================================================*/
E_WE_ERROR
SecFile_CheckFileExist 
(
    WE_HANDLE hWeHandle, 
    const WE_CHAR *pszWeFileName
)
{
    E_WE_ERROR eResult = E_WE_ERROR_INVALID_PARAM;
    WE_CHAR *pszFileName = NULL;
    WE_INT32 iRet = 0;

    if ((NULL == hWeHandle)
        ||(NULL == pszWeFileName))
    {
        return eResult;
    }

    if (WE_HANDLE_MAGIC_NUM != ((St_WeHandle*)hWeHandle)->iMagicNum)
    {
        eResult = E_WE_ERROR_BAD_HANDLE;
        return eResult;
    }

    pszFileName = SecFile_ConvertPath(pszWeFileName);
    if(NULL == pszFileName)
    {
        return E_WE_ERROR_RESOURCE_LIMIT;
    }

    iRet = TF3RD_FACCESS(pszFileName);

    if (0 == iRet)
    {
        eResult = E_WE_OK;
    }
    else
    {
        eResult = E_WE_ERROR_OTHER;
    }

    return eResult;
}

#endif

#else

#endif /*#ifdef M_SEC_CFG_NO_PLATFORM*/

