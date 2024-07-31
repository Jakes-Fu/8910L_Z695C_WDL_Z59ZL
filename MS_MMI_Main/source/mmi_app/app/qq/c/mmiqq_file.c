/****************************************************************************
** File Name:      mmiqq_file.c(Qfile.c )                                               *
** Author:                                                                 *
** Date:           05/11/2010                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the filesystem  of QQ             *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2010       fen.xie         Create
** 
****************************************************************************/
#include "mmi_app_qq_trc.h"
#ifdef QQ_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "qfile.h"
#include "qbase.h"
#include "sfs.h"
#include "sci_types.h"
#include "os_api.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#ifdef QQ_SUPPORT_2009
LOCAL const wchar s_qq_dir_arr[]   = { 'q', 'q', '2', '0', '0', '9', 0};
#else
LOCAL const wchar s_qq_dir_arr[]   = { 'q', 'q', 0};
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: 得到当前QQ系统文件可存储的盘符的名字信息
//  Global resource dependence :  
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL wchar* GetQQDeviceName(uint16* name_len_ptr);	

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: IFILE_GetInfo
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void  IFILE_GetInfo(IFile* file, FileInfo* info)
{
    uint32 file_size = 0;
    SFS_GetFileSize((SFS_HANDLE)file, &file_size);
    info->dwSize = file_size;
}

/*****************************************************************************/
//  Description: 得到当前QQ系统文件可存储的盘符的名字信息
//  Global resource dependence :  
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL wchar* GetQQDeviceName(uint16* name_len_ptr)	
{
    wchar * device_name_ptr = PNULL;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;

    if (PNULL == name_len_ptr)
    {
        return PNULL;
    }

    file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
    
    *name_len_ptr = 0;

    if (MMI_DEVICE_NUM > file_dev)
    {
        *name_len_ptr = MMIAPIFMM_GetDevicePathLen((file_dev));
        device_name_ptr = MMIAPIFMM_GetDevicePath((file_dev));
    }

    return device_name_ptr;
}

/*****************************************************************************/
//  Description: IFILEMGR_OpenFile
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC IFile* IFILEMGR_OpenFile(IFileMgr* pFileMgr, const char* file_name, int access_mode)
{
    SFS_HANDLE 	handle = 0;
    wchar		ucs2_name_arr[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16		ucs2_name_len = 0;
    wchar    	file_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};     //sd:/music/*.mp3, udisk:/music/*.mp3
    uint16   	file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    wchar*	device_name_ptr = PNULL;
    uint16		device_name_len = 0;

    //SCI_TRACE_LOW:"[MMIQQ]IFILEMGR_OpenFile()*****"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_110_112_2_18_2_49_6_0,(uint8*)"");

    if (PNULL == file_name)
    {
        return PNULL;
    }
    
    /** check device exist **/
    device_name_ptr = GetQQDeviceName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        return PNULL;
    }
    
    //转换文件名字的编码
    ucs2_name_len = GUI_GBToWstr(ucs2_name_arr, (uint8 *)file_name, strlen(file_name));
    if (0 == ucs2_name_len)
    {
        return PNULL;
    }

    MMIAPIFMM_CombineFullPath(
                                            device_name_ptr, device_name_len,
                                            s_qq_dir_arr,  MMIAPICOM_Wstrlen(s_qq_dir_arr),
                                            ucs2_name_arr, 
                                            ucs2_name_len, //in bytes
                                            file_full_path,  //[out]
                                            &file_full_path_len      //[out]
                                            );
	
    switch(access_mode)
    {
    case _OFM_CREATE:
        handle = SFS_CreateFile(file_full_path, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);/*lint !e655 */
        break;
    case _OFM_READWRITE:
        handle = SFS_CreateFile(file_full_path, SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);/*lint !e655 */	
        break;
    case _OFM_READ:
        handle = SFS_CreateFile(file_full_path, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, 0, 0);/*lint !e655 */
        break;
    default: 
        break;
    }
    
    return (IFile*)handle;
}
 
/*****************************************************************************/
//  Description: IFILE_Release
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int IFILE_Release(IFile* file)
{
    SFS_ERROR_E error = SFS_NO_ERROR;

    if(PNULL == file)
    {
        return _FS_ERROR;
    }

    error = SFS_CloseFile((SFS_HANDLE)file);
    //SCI_TRACE_LOW:"[MMIQQ]IFILE_Release()*****error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_172_112_2_18_2_49_6_1,(uint8*)"d", error);

    if(SFS_ERROR_NONE == error)
    {
        return _FS_ERROR_NONE;
    }
    else
    {
        return _FS_ERROR;
    }
}

/*****************************************************************************/
//  Description: IFILE_Read
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int IFILE_Read(IFile* file, void *data_ptr, int length)
{
    SFS_ERROR_E error = SFS_NO_ERROR;
    int readlen = 0;

    if(PNULL == file)
    {
        return _FS_ERROR;
    }
    error = SFS_ReadFile((SFS_HANDLE)file, data_ptr, length, (uint32*)&readlen, NULL);
    //SCI_TRACE_LOW:"[MMIQQ]IFILE_Read()*****error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_198_112_2_18_2_49_6_2,(uint8*)"d", error);

    if(SFS_ERROR_NONE == error)
    {
        return readlen;
    }
    else
    {
        return _FS_ERROR;
    }
}

/*****************************************************************************/
//  Description: IFILE_Write
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int IFILE_Write(IFile* file, void *data_ptr, int length)
{
    SFS_ERROR_E error = SFS_ERROR_NONE;
    int writelen = 0;
    
    if(PNULL == file)
    {
        return 0;
    }
    error = SFS_WriteFile((SFS_HANDLE)file, data_ptr, length, (uint32*)&writelen, NULL);
    //SCI_TRACE_LOW:"[MMIQQ]IFILE_Write()*****error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_224_112_2_18_2_49_6_3,(uint8*)"d", error);

    if(SFS_ERROR_NONE == error)
    {
        return writelen;
    }
    else
    {
        return _FS_ERROR;
    }
}

/*****************************************************************************/
//  Description: IFILEMGR_Remove
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int IFILEMGR_Remove(IFileMgr* pFileMgr, const char* file_name)
{
    SFS_ERROR_E error = SFS_NO_ERROR;
    wchar		ucs2_name_arr[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16		ucs2_name_len = 0;
    wchar    	file_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};     //sd:/music/*.mp3, udisk:/music/*.mp3
    uint16   	file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    wchar*	device_name_ptr = PNULL;
    uint16		device_name_len = 0;

    if (PNULL == file_name)
    {
        return _FS_ERROR;
    }
    
    /** check device exist **/
    device_name_ptr = GetQQDeviceName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        return _FS_ERROR;
    }
    
    //转换文件名字的编码
    ucs2_name_len = GUI_GBToWstr(ucs2_name_arr, (uint8 *)file_name, strlen(file_name));
    if (0 == ucs2_name_len)
    {
        return _FS_ERROR;
    }

    MMIAPIFMM_CombineFullPath(
                                            device_name_ptr, device_name_len,
                                            s_qq_dir_arr,  MMIAPICOM_Wstrlen(s_qq_dir_arr),
                                            ucs2_name_arr, 
                                            ucs2_name_len, //in bytes
                                            file_full_path,  //[out]
                                            &file_full_path_len      //[out]
                                            );
    
    if(MMIAPIFMM_IsFileExist(file_full_path, file_full_path_len))
    {
        error = SFS_DeleteFile(file_full_path, 0);
        //SCI_TRACE_LOW:"[MMIQQ]IFILEMGR_Remove()*****error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_281_112_2_18_2_49_6_4,(uint8*)"d", error);
        
        if(SFS_ERROR_NONE == error)
        {
            return _FS_ERROR_NONE;
        }
    }

    return _FS_ERROR;
}

/* 只能在文件长度范围内操作文件句柄指针，否则移动到文件头，汗*/
/*****************************************************************************/
//  Description: IFILE_Seek
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int IFILE_Seek(IFile* file, int whence, int offset)
{
    int pos = 0;
    SFS_ERROR_E error = SFS_NO_ERROR;
    unsigned int size = 0;

    if (PNULL == file)
    {
        //SCI_TRACE_LOW:"[MMIQQ]IFILE_Seek()*****error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_305_112_2_18_2_49_6_5,(uint8*)"");
        return 0;
    }
    
    switch(whence)
    {
    case _SEEK_START:
        SFS_GetFileSize((SFS_HANDLE)file, (uint32*)&size);
        if(offset > (int)size)
        {
            SFS_SetFileSize((SFS_HANDLE)file, offset + 2);
        }
        error = SFS_SetFilePointer((SFS_HANDLE)file, (__int64)offset,SFS_SEEK_BEGIN);
        break;

    case _SEEK_CURRENT:
        error = SFS_SetFilePointer((SFS_HANDLE)file, (__int64)offset,SFS_SEEK_CUR);
        break;
        
    case _SEEK_END:
        error = SFS_SetFilePointer((SFS_HANDLE)file, (__int64)offset,SFS_SEEK_END);
        break;
        
    default:
        break;
	}
	
//	SFS_GetFilePointer((SFS_HANDLE)file, SFS_SEEK_BEGIN, &pos);
	//SCI_TRACE_LOW:"[MMIQQ]IFILE_Seek()*****error = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_333_112_2_18_2_49_6_6,(uint8*)"d", error);

	return pos;
}

/*****************************************************************************/
//  Description: IFILEMGR_MkDir
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int IFILEMGR_MkDir(IFileMgr* pFileMgr, const char *dir_name)
{
    SFS_ERROR_E error = SFS_NO_ERROR;
    wchar		ucs2_name_arr[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16		ucs2_name_len = 0;
    wchar    	file_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};     //sd:/music/*.mp3, udisk:/music/*.mp3
    uint16   	file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    wchar*	device_name_ptr = PNULL;
    uint16		device_name_len = 0;

    if (PNULL == dir_name)
    {
        return _FS_ERROR;
    }
    
    /** check device exist **/
    device_name_ptr = GetQQDeviceName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        return _FS_ERROR;
    }
    
    //转换文件名字的编码
    ucs2_name_len = GUI_GBToWstr(ucs2_name_arr, (uint8 *)dir_name, strlen(dir_name));
    if (0 == ucs2_name_len)
    {
        return _FS_ERROR;
    }
    else if (MMIFILE_SLASH == ucs2_name_arr[ucs2_name_len - 1])//TENCENT传入的dir_name参数带"\",故需要删除该字符
    {
        ucs2_name_len -= 1;
        ucs2_name_arr[ucs2_name_len] = 0x00;
    }

    MMIAPIFMM_CombineFullPath(
                                            device_name_ptr, device_name_len,
                                            s_qq_dir_arr,  MMIAPICOM_Wstrlen(s_qq_dir_arr),
                                            ucs2_name_arr, 
                                            ucs2_name_len, //in bytes
                                            file_full_path,  //[out]
                                            &file_full_path_len      //[out]
                                            );

    if(!MMIAPIFMM_IsFolderExist(file_full_path, file_full_path_len))//文件夹不存在,则需要创建
    {
        error = SFS_CreateDirectory(file_full_path);

        //SCI_TRACE_LOW:"[MMIQQ]IFILEMGR_MkDir()*****error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_389_112_2_18_2_49_6_7,(uint8*)"d", error);
        
        if(SFS_ERROR_NONE != error)
        {
            return _FS_ERROR;
        }
    }
    
    return _FS_ERROR_NONE;
}



/*****************************************************************************/
//  Description: IFILEMGR_RmDir
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int IFILEMGR_RmDir(IFileMgr* pFileMgr, const char* dir_name)
{
    wchar		ucs2_name_arr[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16		ucs2_name_len = 0;
    wchar    	file_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};     //sd:/music/*.mp3, udisk:/music/*.mp3
    uint16   	file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN+1;
    wchar*	device_name_ptr = PNULL;
    uint16		device_name_len = 0;
    BOOLEAN   result = TRUE;

    //MMIFILE_HANDLE f_handle;
    //MMIFILE_FIND_DATA_T find_data;

    if (PNULL == dir_name)
    {
        return _FS_ERROR;
    }
    
    /** check device exist **/
    device_name_ptr = GetQQDeviceName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        return _FS_ERROR;
    }
    
    //转换文件名字的编码
    ucs2_name_len = GUI_GBToWstr(ucs2_name_arr, (uint8 *)dir_name, strlen(dir_name));
    if (0 == ucs2_name_len)
    {
        return _FS_ERROR;
    }
    else if (MMIFILE_SLASH == ucs2_name_arr[ucs2_name_len - 1])//TENCENT传入的dir_name参数带"\",故需要删除该字符
    {
        ucs2_name_len -= 1;
        ucs2_name_arr[ucs2_name_len] = 0x00;
    }
    
    MMIAPIFMM_CombineFullPath(
                                            device_name_ptr, device_name_len,
                                            s_qq_dir_arr,  MMIAPICOM_Wstrlen(s_qq_dir_arr),
                                            ucs2_name_arr, 
                                            ucs2_name_len, //in bytes
                                            file_full_path,  //[out]
                                            &file_full_path_len      //[out]
                                            );

    if(MMIAPIFMM_IsFolderExist(file_full_path, file_full_path_len))//文件夹存在则需要删除
    {
        result =  MMIAPICOM_DelDirAll(file_full_path, SFS_MAX_PATH);
    }
    
    //SCI_TRACE_LOW:"[MMIQQ]IFILEMGR_RmDir() result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_FILE_457_112_2_18_2_49_6_8,(uint8*)"d", result);

    return ( (result)? _FS_ERROR_NONE: _FS_ERROR);
}

/*****************************************************************************/
//  Description: QWriteFileLog
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QWriteFileLog(const char *psz,...)
{

/*	va_list marker;
	char p[200];
	
	IFile* pFile=IFILEMGR_OpenFile(QGetIFileMgr(), "qqfile.log", _OFM_READWRITE);
	if(pFile==0){
		pFile=IFILEMGR_OpenFile(QGetIFileMgr(), "qqfile.log", _OFM_CREATE);
	}
	
	if(pFile!=0){
		MEMSET(p,0,sizeof(p));
		
		//SPRINTF(p,"[%04u]",QGetTickTime());
		
		va_start(marker, psz);   	
		_vsnprintf(p, sizeof(p), psz, marker);
		p[STRLEN(p)] = 0x0a;
		va_end(marker);
		
		IFILE_Seek(pFile,_SEEK_END,0);
		IFILE_Write(pFile,p,STRLEN(p));
		IFILE_Release(pFile);
	}
*/

}

//#endif

PUBLIC wchar* MMIQQ_GetDeviceName(uint16* name_len_ptr)	
{
    //wchar*	device_name_ptr = PNULL;

    return GetQQDeviceName(name_len_ptr);
    
}

#endif
