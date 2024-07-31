/*==============================================================================
    FILE NAME   : eme_file_api.h
    MODULE NAME : 


    GENERAL DESCRIPTION:
        This file wraps the file operation for upper layer.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-03-02   Daniel           01         Create
    2007-08-29   Wenkai Xu        02         Add and modify some interface

==============================================================================*/
#ifndef EME_FILE_API_H
#define EME_FILE_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
/* Include System head file */
#include "tf3rd_for_hz.h"

/* Include Program Global head file */
#include "we_def.h"

/* Include module head file */
#include "eme_error.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
/*
 *	Seek Origin
 */
#define EME_SEEK_START     TF3RD_SEEK_SET
#define EME_SEEK_END       TF3RD_SEEK_END
#define EME_SEEK_CURRENT   TF3RD_SEEK_CUR

/*
 *	File Open Mode
 */
#define EME_OFM_READ       TF3RD_OFM_READ
#define EME_OFM_READWRITE  TF3RD_OFM_READWRITE
#define EME_OFM_APPEND     TF3RD_OFM_APPEND
#define EME_OFM_CREATE     TF3RD_OFM_CREATE

/*
 *	Define the file handler
 */
typedef WE_VOID* FILE_HANDLE;

/*
 *	Some platform may have file manager handler
 */
typedef WE_VOID* FILEMGR_HANDLE;


/*
 *	Define the buffer size for file read and write
 * 8192 or 2048 is the most efficient
 */
#define EME_IO_BUF_SIZE    2048

/*
 *	Define file path's max length
 */
#define EME_MAXPATH        256
#define EME_MAXDIR         256
#define EME_MAXFILE        256
#define EME_MAXEXT         5

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
/*
 *	File I/O function
 */
FILE_HANDLE EME_FileOpen(FILEMGR_HANDLE hMgr, 
                         const WE_CHAR *pPath, 
                         WE_INT32 nMode);

WE_INT32 EME_CreateFile(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath);

WE_INT32 EME_FileClose(FILE_HANDLE hFile);

WE_UINT32 EME_FileRead(FILE_HANDLE hFile, WE_VOID *pBuf, WE_UINT32 dwSize);

WE_UINT32 EME_FileWrite(FILE_HANDLE hFile, WE_VOID *pBuf, WE_UINT32 dwSize);

WE_INT32 EME_FileSeek(FILE_HANDLE hFile, WE_INT32 offset, WE_INT32 nOrigin);

WE_UINT32 EME_FileTell(FILE_HANDLE hFile);

WE_UINT32 EME_FileLength(FILE_HANDLE hFile);

WE_INT32 EME_FileTruncate(FILE_HANDLE hFile, WE_UINT32 dwPos);


/*
 *	File management
 */
WE_INT32 EME_CreateDir(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath);
WE_INT32 EME_RemoveDir(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath);

WE_INT32 EME_DeleteFile(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath);

WE_BOOL EME_DirectoryExists(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath);
WE_BOOL EME_FileExists(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath);

WE_BOOL EME_CheckFitFreeSpace(FILEMGR_HANDLE hMgr, WE_UINT32 uiSpaceSize);
WE_BOOL EME_CheckFitFreeSpaceEx(FILEMGR_HANDLE hMgr, WE_UINT32 uiMsgId, WE_UINT32 uiSpaceSize);
WE_BOOL EME_CheckUDiskFreeSpace(FILEMGR_HANDLE hMgr, WE_UINT32 uiSpaceSize);
WE_BOOL EME_CheckSDCard1FreeSpace(FILEMGR_HANDLE hMgr, WE_UINT32 uiSpaceSize);
WE_BOOL EME_CheckSDCard2FreeSpace(FILEMGR_HANDLE hMgr, WE_UINT32 uiSpaceSize);

WE_INT32 EME_EnumInit(const WE_CHAR *pDir, WE_BOOL bDirs);
WE_CHAR* EME_EnumNext(FILEMGR_HANDLE hMgr);
WE_VOID EME_EnumClose(FILEMGR_HANDLE hMgr);

#define EME_Rename TF3RD_RENAME
/*------------------------------------------------------------------------------
 *    Function Documentation
 *----------------------------------------------------------------------------*/
/*==============================================================================
 *    Function:      EME_FileOpen
 * 
 *    Description:   This function is used to open a file in specified mode.
 * 
 *    Prototype:
 *       FILE_HANDLE EME_FileOpen(FILEMGR_HANDLE hMgr, 
 *                                WE_CHAR *pPath, 
 *                                WE_INT32 nMode);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 *       pPath    [in]: Null terminated string containing the file path.
 *       nMode    [in]: File open mode.
 *                      The file open mode takes the following values:
 *                      EME_OFM_READ:        Open for reading
 *                      EME_OFM_READWRITE:   Open in read/write mode 
 *                      EME_OFM_APPEND:      Open for appending
 *                      EME_OFM_CREATE:      Create
 * 
 *    Return Value:
 *       Pointer to the FILE_HANDLE handler of the file opened if successful, 
 *       or NULL if failed.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_CreateFile
 * 
 *    Description:   This function is used to create a new file.
 * 
 *    Prototype:
 *       WE_INT32 EME_CreateFile(FILEMGR_HANDLE hMgr, const WE_CHAR *pPath);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 *       pPath    [in]: Null terminated string containing the file path.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the file is successfully created or 
 *                      if file is already exists.
 *       EME_EFAILED:   If unsuccessful.
 *  	   
 *    Comments: 
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_FileClose
 * 
 *    Description:   Close the file by file handler.
 * 
 *    Prototype:
 *       WE_INT32 EME_FileClose(FILE_HANDLE hFile);
 * 
 *    Parameters:
 *       hFile    [in]: File handler return by EME_FileOpen or EME_FileCreate.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If close the file successful.
 *       EME_FAILED:    If Unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_FileRead
 * 
 *    Description:   This function reads a specified number of bytes 
 *                   from an open file.
 * 
 *    Prototype:
 *       WE_UINT32 EME_FileRead(FILE_HANDLE hFile, 
 *                              WE_VOID *pBuf, 
 *                              WE_UINT32 dwSize);
 * 
 *    Parameters:
 *       hFile    [in]: File handler return by EME_FileOpen.
 *       pBuf    [out]: Buffer into which the file data is read.
 *       dwSize   [in]: Number of bytes to be read.
 * 
 *    Return Value:
 *       Number of bytes read: If successful.
 *       0(zero): If unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_FileWrite
 * 
 *    Description:   This function writes the specified number of bytes 
 *                   to an open file.
 * 
 *    Prototype:
 *       WE_UINT32 EME_FileWrite(FILE_HANDLE hFile, 
 *                               WE_VOID *pBuf, 
 *                               WE_UINT32 dwSize);
 * 
 *    Parameters:
 *       hFile    [in]: File handler return by EME_FileOpen or EME_FileCreate.
 *       pBuf     [in]: Buffer from which the data is written.
 *       dwSize   [in]: Number of bytes to be written.
 * 
 *    Return Value:
 *       Number of bytes written: If successful.
 *       0(zero): If unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_FileSeek
 * 
 *    Description:   This function moves the file pointer of the file 
 *                   handler a given distance from a specified origin. 
 * 
 *    Prototype:
 *       WE_INT32 EME_FileSeek(FILE_HANDLE hFile, 
 *                             WE_UINT32 offset, 
 *                             WE_INT32 nOrigin);
 * 
 *    Parameters:
 *       hFile    [in]: File handler return by EME_FileOpen or EME_FileCreate.
 *       offset   [in]: Distance to move.
 *       nOrigin  [in]: EME_SEEK_START or EME_SEEK_END or EME_SEEK_CURRENT.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If successful.
 *       EME_EFAILED:   If unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_FileTell
 * 
 *    Description:   Get the position of the current file pointer .
 * 
 *    Prototype:
 *       WE_UINT32 EME_FileTell(FILE_HANDLE hFile);
 * 
 *    Parameters:
 *       hFile    [in]: File handler return by EME_FileOpen or EME_FileCreate.
 * 
 *    Return Value:
 *       The position of the current file pointer.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_FileLength
 * 
 *    Description:   This function is used to get the file size.
 * 
 *    Prototype:
 *       WE_UINT32 EME_FileLength(FILE_HANDLE hFile);
 * 
 *    Parameters:
 *       hFile    [in]: File handler return by EME_FileOpen or EME_FileCreate.
 * 
 *    Return Value:
 *       The size of the current file.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_FileTruncate
 * 
 *    Description:   This function truncates the file specified by the file 
 *                   handler to position specified by the dwPos parameter. 
 * 
 *    Prototype:
 *       WE_INT32 EME_FileTruncate(FILE_HANDLE hFile, WE_UINT32 dwPos);
 * 
 *    Parameters:
 *       hFile    [in]: File handler return by EME_FileOpen or EME_FileCreate.
 *       dwPos    [in]: Truncate position 
 * 
 *    Return Value:
 *       EME_SUCCESS:   If successful. 
 *       EME_EFAILED:   If unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/

/*==============================================================================
 *    Function:      EME_CreateDir
 * 
 *    Description:   This function is used to create a directory, 
 *                   specified by a directory name. 
 * 
 *    Prototype:
 *       WE_INT32 EME_CreateDir(FILEMGR_HANDLE hMgr, WE_CHAR *pPath);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 *       pPath    [in]: NULL terminated string containing a directory name.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the directory is successfully created or 
 *                      if directory already exists.
 *       EME_EFAILED:   If unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_RemoveDir
 * 
 *    Description:   This function is used to remove a directory 
 *                   identified by a given directory name.
 * 
 *    Prototype:
 *       WE_INT32 EME_RemoveDir(FILEMGR_HANDLE hMgr, WE_CHAR *pPath);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 *       pPath    [in]: NULL terminated string containing a directory name.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If successful. 
 *       EME_EFAILED:   If unsuccessful.
 *  	   
 *    Comments:   This function can remove files and directories in the 
 *                directory.
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_DeleteFile
 * 
 *    Description:   This function is used to remove a file 
 *                   identified by a given filename.
 * 
 *    Prototype:
 *       WE_INT32 EME_DeleteFile(FILEMGR_HANDLE hMgr, WE_CHAR *pPath);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 *       pPath    [in]: Null terminated string containing the file path.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If successful. 
 *       EME_EFAILED:   If unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_DirectoryExists/EME_FileExists
 * 
 *    Description:   These to functions are used to test 
 *                   if the specified directory/file exists.
 * 
 *    Prototype:
 *       WE_BOOL EME_DirectoryExists(FILEMGR_HANDLE hMgr, WE_CHAR *pPath);
 *       WE_BOOL EME_FileExists(FILEMGR_HANDLE hMgr, WE_CHAR *pPath);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 *       pPath    [in]: Null terminated string containing the 
 *                      directory/file path.
 * 
 *    Return Value:
 *       TRUE:    If specified directory/file exists.
 *       FALSE:   If otherwise.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_FreeSpace
 * 
 *    Description:   This function gets the number of free bytes 
 *                   currently available on file system.
 * 
 *    Prototype:
 *       WE_UINT32 EME_FreeSpace(FILEMGR_HANDLE hMgr);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 * 
 *    Return Value:
 *       Number of bytes of file system space currently available: If successful.
 *       0(zero): If fails.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_EnumInit
 * 
 *    Description:   This function do initialization for interactively 
 *                   using the EME_EnumNext() operation on the files 
 *                   or directories in the specified directory.
 * 
 *    Prototype:
 *       WE_INT32 EME_EnumInit(FILEMGR_HANDLE hMgr, 
 *                             WE_CHAR *pDir, 
 *                             WE_BOOL bDirs);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 *       pDir     [in]: NULL terminated string containing the root directory name.
 *       bDirs    [in]: Enumeration type:
 *                      TRUE:    Directories only.
 *                      FALSE:   Files only.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If successful. 
 *  	   EME_EFAILED:   If unsuccessful.
 *
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      EME_EnumNext
 * 
 *    Description:   This function returns file path for the next 
 *                   file/directory within the specified root directory.
 * 
 *    Prototype:
 *       WE_BOOL EME_EnumNext(FILEMGR_HANDLE hMgr, WE_CHAR *pcPath);
 * 
 *    Parameters:
 *       hMgr     [in]: Some platform may need file manager handle, 
 *                      this parameter point to the file manager handler.
 *       pcPath  [out]: The path return, call must allocate the memory.
 * 
 *    Return Value:
 *       TRUE : If successful 
 *       FALSE : If unsuccessful.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
#endif /* EME_FILE_API_H */