/*****************************************************************************
** File Name:      qfile.h                                                  *
** Author:                                                                   *
** Date:             11/05/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare QQ filesystem function and define global               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010       fen.xie       Create
******************************************************************************/
#ifndef _QQ_FILE_H
#define _QQ_FILE_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sfs.h"

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
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef  SFS_HANDLE    IFile;
typedef struct _IFileMgr IFileMgr; 


enum 
{
   _OFM_READ      = 0x0001,
   _OFM_READWRITE = 0x0002,
   _OFM_CREATE    = 0x0004
};

enum 
{
	_SEEK_START,
	_SEEK_END,
	_SEEK_CURRENT
};	

enum
{
	_FS_ERROR_NONE  = 0,
	_FS_ERROR
};


typedef struct
{
	int dwSize;
}FileInfo;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: IFILE_GetInfo
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void  IFILE_GetInfo(IFile* file, FileInfo* info);

/*****************************************************************************/
//  Description: IFILE_Read
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int IFILE_Read(IFile* file, void* data_ptr, int length);
/*****************************************************************************/
//  Description: IFILE_Write
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int IFILE_Write(IFile* file, void *data_ptr, int length);
/*****************************************************************************/
//  Description: IFILE_Seek
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int IFILE_Seek(IFile* file, int whence, int offset);
/*****************************************************************************/
//  Description: IFILE_Release
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int IFILE_Release(IFile* file);

/*****************************************************************************/
//  Description: IFILEMGR_OpenFile
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
IFile* IFILEMGR_OpenFile(IFileMgr* pFileMgr, const char *file_name, int access_mode);
/*****************************************************************************/
//  Description: IFILEMGR_Remove
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int IFILEMGR_Remove(IFileMgr* pFileMgr, const char * file_name);
/*****************************************************************************/
//  Description: IFILEMGR_MkDir
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int IFILEMGR_MkDir(IFileMgr* pFileMgr, const char *dir_name);
/*****************************************************************************/
//  Description: IFILEMGR_RmDir
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int IFILEMGR_RmDir(IFileMgr* pFileMgr,const char *dir_name);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
