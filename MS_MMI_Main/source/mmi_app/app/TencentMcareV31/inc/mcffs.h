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
//Descrption:�ļ�ϵͳ��ʼ��
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfFile_Init(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfFile_Deinit
//Descrption:�ļ�ϵͳ����ʼ��
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfFile_Deinit(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_InsertHandle
//Descrption:��չѶ�ļ����ת��Ϊmcare���
//Param:
//  IN: sfs_handle:չѶ���  
//  OUT:MFd:mcare���
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MFd McfFile_InsertHandle(MUINT sfs_handle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_RemoveHandle
//Descrption:mcare����Ƴ�
//Param:
//  IN: file_handle:mcare���
//  OUT:MBOOL
//Return:MTRUE �ɹ�
//      MFALSE ʧ��
//Note:
//----------------------------------------------------------------------------
MBOOL McfFile_RemoveHandle(MFd file_handle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetSFSHandle
//Descrption:ͨ��mcare������չѶ�ļ����
//Param:
//  IN: file_handle:mcare���
//      sfs_handle:չѶ�ļ����
//  OUT:MBOOL
//Return:MTRUE �ɹ�
//      MFALSE ʧ��
//Note:
//----------------------------------------------------------------------------
MBOOL McfFile_GetSFSHandle(MFd file_handle, MUINT* sfs_handle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Open
//Descrption:���ļ�
//Param:
//  IN: pwcszFileName:�ļ���
//      iOpenFlag:������,MFILE_OPEN_RO,MFILE_OPEN_RW,MFILE_OPEN_CW
//  OUT:MFd
//Return:�ļ����
//Note:
//----------------------------------------------------------------------------
MFd McfFile_Open(const MWCHAR* pwcszFileName, MUINT iOpenFlag);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Close
//Descrption:�ر��ļ�
//Param:
//  IN: FileHandle: �ļ����,Mcare��
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Close(MFd FileHandle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Read
//Descrption:��ȡ�ļ�
//Param:
//  IN: FileHandle: �ļ����,Mcare��
//      DataPtr:��ȡ�ļ�������
//      iLength:��ȡ����
//  OUT:MINT
//Return:��ȡ�Ĵ�С
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Read(MFd FileHandle, MVOID* DataPtr, MUINT iLength);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Write
//Descrption:д�ļ�
//Param:
//  IN: FileHandle: �ļ����,Mcare��
//      DataPtr:д�ļ�������
//      iLength:д����
//  OUT:MINT
//Return:д�Ĵ�С
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Write(MFd FileHandle, const MVOID* DataPtr, MUINT iLength);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Flush
//Descrption:����д�ļ�
//Param:
//  IN: FileHandle: �ļ����,Mcare��
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Flush(MFd FileHandle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Seek
//Descrption:ָ�����ļ��ľ���λ��
//Param:
//  IN: FileHandle: �ļ����,Mcare��
//      iOffset:ƫ����
//      iSeekType:ָ�����ͣ�MFILE_SEEK_SET��MFILE_SEEK_CUR��MFILE_SEEK_END
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Seek(MFd FileHandle, MINT iOffset, MINT iSeekType);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_CheckFile
//Descrption:�ж��ļ������ļ����Ƿ����
//Param:
//  IN: pwcFileName: �ļ�����
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_CheckFile(const MWCHAR* pwcFileName);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Delete
//Descrption:ɾ���ļ�
//Param:
//  IN: pwcFileName: �ļ�����
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Delete(const MWCHAR* pwcFileName);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Move
//Descrption:�ƶ��ļ�
//Param:
//  IN: pwcSrcFullPath: ԭ �ļ�·��
//      pwcDstFullPath:Ŀ���ļ�·��
//  OUT:MINT
//Return:error code MFILE_SUCCESS
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Move(const MWCHAR* pwcSrcFullPath, const MWCHAR* pwcDstFullPath);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_FindFirst
//Descrption:��ָ��·���²��Ҷ�Ӧ���͵��ļ�
//Param:
//  IN: pwcnamePattern: ָ��·��
//      pwcFileName:��Ӧ����
//      iMaxLen:��󳤶�
//  OUT:MFd
//Return:�ļ����
//Note:
//----------------------------------------------------------------------------
MFd McfFile_FindFirst(const MWCHAR* pwcnamePattern, MWCHAR* pwcFileName, MUINT iMaxLen);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_FindNext
//Descrption:������һ���ļ�
//Param:
//  IN: FileHandle: ���ҵ����ļ����
//      pwcFileName:��Ӧ����
//      iMaxLen:��󳤶�
//  OUT:MFd
//Return:�ļ����
//Note:
//----------------------------------------------------------------------------
MINT McfFile_FindNext(MFd FileHandle, MWCHAR* pwcFileName, MUINT iMaxLen);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_FindClose
//Descrption:�رղ��ҵ����ļ�
//Param:
//  IN: FileHandle: ���ҵ����ļ����
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_FindClose(MFd FileHandle);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetFileInfo
//Descrption:ȡ���ļ�����
//Param:
//  IN: FileHandle: ���ҵ����ļ����
//      psFileInfo:�����ļ�����
//  OUT:psFileInfo��MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_GetFileInfo(MFd FileHandle, MFileInfo* psFileInfo);

MUINT McfFile_GetFileSize(MFd fd);
//----------------------------------------------------------------------------
//FUNCTION: McfFile_SetFileAttribute
//Descrption:�����ļ�����
//Param:
//  IN: pwcFileName:�ļ���
//      byAttr:�ļ�����
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_SetFileAttribute(const MWCHAR* pwcFileName, MBYTE byAttr);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetFilePosition
//Descrption:��ȡ�ļ������λ��
//Param:
//  IN: FileHandle:�ļ����
//      piPosition:λ��
//  OUT:MINT�� piPosition
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_GetFilePosition(MFd FileHandle, MUINT *piPosition);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetTCardFreeSpace
//Descrption:��ȡT����ʣ��ռ��С
//Param:
//  IN: MVOID
//  OUT:MUINT
//Return:T����ʣ��ռ��С
//Note:
//----------------------------------------------------------------------------
MUINT McfFile_GetTCardFreeSpace(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetPublicDrvFreeSpace
//Descrption:��ȡ�ֻ��̵�ʣ��ռ��С
//Param:
//  IN: MVOID
//  OUT:MINT
//Return:��ȡ�ֻ��̵�ʣ��ռ��С
//Note:
//----------------------------------------------------------------------------
MUINT McfFile_GetPublicDrvFreeSpace(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetCardDrv
//Descrption:��ȡT��·��
//Param:
//  IN: MVOID
//  OUT:MWCHAR
//Return:T��·��
//Note:
//----------------------------------------------------------------------------
MWCHAR McfFile_GetCardDrv(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetCardDrv
//Descrption:��ȡ�ֻ���·��
//Param:
//  IN: MVOID
//  OUT:MWCHAR
//Return:�ֻ���·��
//Note:
//----------------------------------------------------------------------------
MWCHAR McfFile_GetPublicDrv(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_IsTCardReady
//Descrption:�ж�T���Ƿ����
//Param:
//  IN: MVOID
//  OUT:MBOOL
//Return:MTRUE:����
//      MFALSE:δ����
//Note:
//----------------------------------------------------------------------------
MBOOL McfFile_IsTCardReady(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_CreateDir
//Descrption:����Ŀ¼
//Param:
//  IN: pwcDirName:Ŀ¼����
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_CreateDir(const MWCHAR* pwcDirName);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_RemoveDir
//Descrption:ɾ��Ŀ¼
//Param:
//  IN: pwcDirName:Ŀ¼����
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_RemoveDir(const MWCHAR* pwcDirName);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_Rename
//Descrption:ɾ��Ŀ¼
//Param:
//  IN: pwcFileName:ԭ��
//      pwcNewName:����
//  OUT:MINT
//Return:error code
//Note:
//----------------------------------------------------------------------------
MINT McfFile_Rename(const MWCHAR* pwcFileName, const MWCHAR* pwcNewName);

//----------------------------------------------------------------------------
//FUNCTION: McfFmgr_SelectPathAndEnter
//Descrption:��ƽ̨���ļ�ϵͳ��ѡ���ļ���ִ�лص�
//Param:
//  IN: callback
//  OUT:MINT
//Return:error code 
//Note:
//----------------------------------------------------------------------------
MINT McfFmgr_SelectPathAndEnter(pfnMcfSelPathCb callback);


//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetCardDrv
//Descrption:�ֻ��洢�����Ƿ����
//Param:
//  IN: wcdrv:�洢����
//  OUT:MBOOL
//Return:�Ƿ����
//Note:
//----------------------------------------------------------------------------
MBOOL McfFile_CheckDrvReady(MWCHAR wcdrv);

//----------------------------------------------------------------------------
//FUNCTION: McfFile_GetCardDrv
//Descrption:����ֻ��洢�����ʣ��ռ�
//Param:
//  IN: wcdrv:�洢����
//  OUT:MINT
//Return:�洢�����ʣ��ռ�
//Note:
//----------------------------------------------------------------------------
MINT McfFile_GetDrvFreeSpace(MWCHAR wcdrv);

MWCHAR McfFile_GetStorageDrv(MVOID);

MUINT McfFile_GetStorageFreeSpace(MVOID);

MUINT McfFile_GetFsSizeReq(MVOID);

//fixme: we should make sure: 
// 1.��t���γ�ʱ������洢��ʹ�õ���t���ú�����Ҫ����MFALSE
MBOOL McfFile_IsStorageReady(MVOID);
	
//���ӵ�λΪKB��api
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
