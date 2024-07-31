/******************************************************************************/
//File Name:        McfFs.c
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide simple Fs interface for mcare
//Modifed:          nicklei 26/09/2011
//Modifed:          wilsonzhu 01/10/2011
/******************************************************************************/
#ifndef _MCFFS_H_
#define _MCFFS_H_

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif
/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mcfcommon.h"
#include "mcftime.h"

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//FUNCTION: McfFile_Init
//Descrption:文件系统初始化
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfFile_Init(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfFile_Deinit
//Descrption:文件系统反初始化
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfFile_Deinit(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_InsertHandle
//Descrption:将展讯文件句柄转化为mcare句柄
//Param:
//  IN: sfs_handle:展讯句柄  
//  OUT:MFd:mcare句柄
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MFd McfFile_InsertHandle(MUINT sfs_handle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_RemoveHandle
//Descrption:mcare句柄移除
//Param:
//  IN: file_handle:mcare句柄
//  OUT:MBOOL
//Return:MTRUE 成功
//      MFALSE 失败
//Note:
//----------------------------------------------------------------------------
MBOOL McfFile_RemoveHandle(MFd file_handle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetSFSHandle
//Descrption:通过mcare句柄获得展讯文件句柄
//Param:
//  IN: file_handle:mcare句柄
//      sfs_handle:展讯文件句柄
//  OUT:MBOOL
//Return:MTRUE 成功
//      MFALSE 失败
//Note:
//----------------------------------------------------------------------------
MBOOL McfFile_GetSFSHandle(MFd file_handle, MUINT* sfs_handle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Open
//Descrption:打开文件
//Param:
//  IN: pwcszFileName:文件名
//      iOpenFlag:打开类型,MFILE_OPEN_RO,MFILE_OPEN_RW,MFILE_OPEN_CW
//  OUT:MFd
//Return:文件句柄
//Note:
//----------------------------------------------------------------------------
MFd McfFile_Open(const MWCHAR* pwcszFileName, MUINT iOpenFlag);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Close
//Descrption:关闭文件
//Param:
//  IN: FileHandle: 文件句柄,Mcare的
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Close(MFd FileHandle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Read
//Descrption:读取文件
//Param:
//  IN: FileHandle: 文件句柄,Mcare的
//      DataPtr:读取文件的数据
//      iLength:读取长度
//  OUT:MINT
//Return:读取的大小
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Read(MFd FileHandle, MVOID* DataPtr, MUINT iLength);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Write
//Descrption:写文件
//Param:
//  IN: FileHandle: 文件句柄,Mcare的
//      DataPtr:写文件的数据
//      iLength:写长度
//  OUT:MINT
//Return:写的大小
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Write(MFd FileHandle, const MVOID* DataPtr, MUINT iLength);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Flush
//Descrption:立即写文件
//Param:
//  IN: FileHandle: 文件句柄,Mcare的
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Flush(MFd FileHandle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Seek
//Descrption:指定到文件的具体位置
//Param:
//  IN: FileHandle: 文件句柄,Mcare的
//      iOffset:偏移量
//      iSeekType:指定类型，MFILE_SEEK_SET，MFILE_SEEK_CUR，MFILE_SEEK_END
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Seek(MFd FileHandle, MINT iOffset, MINT iSeekType);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_CheckFile
//Descrption:判断文件或者文件夹是否存在
//Param:
//  IN: pwcFileName: 文件名称
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_CheckFile(const MWCHAR* pwcFileName);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Delete
//Descrption:删除文件
//Param:
//  IN: pwcFileName: 文件名称
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Delete(const MWCHAR* pwcFileName);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Move
//Descrption:移动文件
//Param:
//  IN: pwcSrcFullPath: 原 文件路径
//      pwcDstFullPath:目的文件路径
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Move(const MWCHAR* pwcSrcFullPath, const MWCHAR* pwcDstFullPath);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_FindFirst
//Descrption:在指定路径下查找对应类型的文件
//Param:
//  IN: pwcnamePattern: 指定路径
//      pwcFileName:对应类型
//      iMaxLen:最大长度
//  OUT:MFd
//Return:文件句柄
//Note:
//----------------------------------------------------------------------------
MFd McfFile_FindFirst(const MWCHAR* pwcnamePattern, MWCHAR* pwcFileName, MUINT iMaxLen);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_FindNext
//Descrption:查找下一个文件
//Param:
//  IN: FileHandle: 查找到的文件句柄
//      pwcFileName:对应类型
//      iMaxLen:最大长度
//  OUT:MFd
//Return:文件句柄
//Note:
//----------------------------------------------------------------------------
MINT McfFile_FindNext(MFd FileHandle, MWCHAR* pwcFileName, MUINT iMaxLen);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_FindClose
//Descrption:关闭查找到的文件
//Param:
//  IN: FileHandle: 查找到的文件句柄
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_FindClose(MFd FileHandle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetFileInfo
//Descrption:取得文件属性
//Param:
//  IN: FileHandle: 查找到的文件句柄
//      psFileInfo:接收文件属性
//  OUT:psFileInfo，MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_GetFileInfo(MFd FileHandle, MFileInfo* psFileInfo);

MUINT McfFile_GetFileSize(MFd fd);
//----------------------------------------------------------------------------
//FUNCTION: McfFile_SetFileAttribute
//Descrption:设置文件属性
//Param:
//  IN: pwcFileName:文件名
//      byAttr:文件属性
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_SetFileAttribute(const MWCHAR* pwcFileName, MBYTE byAttr);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetFilePosition
//Descrption:获取文件句柄的位置
//Param:
//  IN: FileHandle:文件句柄
//      piPosition:位置
//  OUT:MINT， piPosition
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_GetFilePosition(MFd FileHandle, MUINT *piPosition);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetTCardFreeSpace
//Descrption:获取T卡的剩余空间大小
//Param:
//  IN: MVOID
//  OUT:MUINT
//Return:T卡的剩余空间大小
//Note:
//----------------------------------------------------------------------------
MUINT McfFile_GetTCardFreeSpace(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetPublicDrvFreeSpace
//Descrption:获取手机盘的剩余空间大小
//Param:
//  IN: MVOID
//  OUT:MINT
//Return:获取手机盘的剩余空间大小
//Note:
//----------------------------------------------------------------------------
MUINT McfFile_GetPublicDrvFreeSpace(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetCardDrv
//Descrption:获取T卡路径
//Param:
//  IN: MVOID
//  OUT:MWCHAR
//Return:T卡路径
//Note:
//----------------------------------------------------------------------------
MWCHAR McfFile_GetCardDrv(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetCardDrv
//Descrption:获取手机盘路径
//Param:
//  IN: MVOID
//  OUT:MWCHAR
//Return:手机盘路径
//Note:
//----------------------------------------------------------------------------
MWCHAR McfFile_GetPublicDrv(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_IsTCardReady
//Descrption:判断T卡是否就绪
//Param:
//  IN: MVOID
//  OUT:MBOOL
//Return:MTRUE:就绪
//      MFALSE:未就绪
//Note:
//----------------------------------------------------------------------------
MBOOL McfFile_IsTCardReady(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_CreateDir
//Descrption:创建目录
//Param:
//  IN: pwcDirName:目录名称
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_CreateDir(const MWCHAR* pwcDirName);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_RemoveDir
//Descrption:删除目录
//Param:
//  IN: pwcDirName:目录名称
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_RemoveDir(const MWCHAR* pwcDirName);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Rename
//Descrption:删除目录
//Param:
//  IN: pwcFileName:原名
//      pwcNewName:新名
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Rename(const MWCHAR* pwcFileName, const MWCHAR* pwcNewName);

//----------------------------------------------------------------------------
//FUNCTION: McfFmgr_SelectPathAndEnter
//Descrption:打开平台的文件系统，选择文件后执行回调
//Param:
//  IN: callback
//  OUT:MINT
//Return:error code 
//Note:
//----------------------------------------------------------------------------
MINT McfFmgr_SelectPathAndEnter(pfnMcfSelPathCb callback);


//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetCardDrv
//Descrption:手机存储区域是否就绪
//Param:
//  IN: wcdrv:存储区域
//  OUT:MBOOL
//Return:是否就绪
//Note:
//----------------------------------------------------------------------------
MBOOL McfFile_CheckDrvReady(MWCHAR wcdrv);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetCardDrv
//Descrption:获得手机存储区域的剩余空间
//Param:
//  IN: wcdrv:存储区域
//  OUT:MINT
//Return:存储区域的剩余空间
//Note:
//----------------------------------------------------------------------------
MINT McfFile_GetDrvFreeSpace(MWCHAR wcdrv);

MWCHAR McfFile_GetStorageDrv(MVOID);

MUINT McfFile_GetStorageFreeSpace(MVOID);

MUINT McfFile_GetFsSizeReq(MVOID);

//fixme: we should make sure: 
// 1.当t卡拔出时，如果存储区使用的是t卡该函数需要返回MFALSE
MBOOL McfFile_IsStorageReady(MVOID);
	
//增加单位为KB的api
MUINT McfFile_GetTCardFreeSpaceKB(MVOID);
MWCHAR* McfFile_GetSysPath(MVOID);

MWCHAR* McfFile_GetAppPath(MVOID);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus   
}   
#endif // __cplusplus

#endif//_MCFFS_H_
