/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#include "kjava_sys_fs.h"
#include "kjava_sys_fs_dev.h"
#include "sfs.h"
#include "sci_types.h"
#include "os_api.h"
#include "jwe_log.h"
#include "gui_ucs2b_converter.h"
#include "mmifmm_export.h"

#ifndef _AJSC_DEBUG
    #undef  JWE_LOG
    #define JWE_LOG(function,parameters)
#endif


KJavaDevFileHandle kjava_dfs_open(const char *fileName, int* err )
{
    KJavaFileHandle handle;
    JWE_LOG(kjava_dfs_open,("fileName = %s", fileName))
	handle=kjava_fs_open( fileName, KJAVA_FILE_OPEN_READ|KJAVA_FILE_OPEN_EXCL ,0, err);
	return handle;
}

int kjava_dfs_close(KJavaDevFileHandle hFile)
{
    int ret;//the return value of the function's executing condition
    JWE_LOG(kjava_dfs_close,("hFile = %d", hFile))
	ret= kjava_fs_close(hFile,0);
	return ret;
}

int kjava_dfs_read(void* ptr, 
                  signed long offset,
                  signed long size, 
                  signed long* read,
                  KJavaDevFileHandle hFile)
{
    int ret = KJAVA_FILE_ERR_NONE;			//the return value of the function's executing condition
	KJavaLongSize K_offset;
    JWE_LOG(kjava_dfs_read,("kjava read is being called size = %d", size))
	
    if(ptr == 0)
	{
        return KJAVA_FILE_ERR_IO_ERR;
    }
    //if you want to read zero byte,only the end of file should be returned.
    
    if(size == 0)
    {
        return KJAVA_FILE_ERR_END_OF_FILE;
    }
    //of course,null value of file's handle is not allowed
    
    if(hFile == 0)
	{
    	return KJAVA_FILE_ERR_IO_ERR;
	}
    
	K_offset.high=(unsigned long )0;
	K_offset.low=(unsigned long )(offset);
	
	ret=kjava_fs_seek((KJavaFileHandle *)hFile,&K_offset,SFS_SEEK_BEGIN,0);
    
	if( ret<0 )
	{
		return KJAVA_FILE_ERR_NOT_EXIST;

	}
	else
	{
		ret=kjava_fs_read( ptr, size, read, (KJavaFileHandle*)hFile ,0);
	}
    
	return ret;
}
                  
int kjava_dfs_stat2(const char *fileName, signed long* size)
{
    //int         ret = 0;
    SFS_HANDLE filehandle = NULL;
    int         wlength = 0;  
    uint16      wfilename[255] = {0};
    uint32      file_Size;

	if(fileName == 0)
	{
        return KJAVA_FILE_ERR_IO_ERR;
	}
	
    JWE_LOG(kjava_dfs_stat2,("kjava_dfs_stat2 is being called"));
    JWE_LOG(kjava_dfs_stat2,("filename is %s",fileName));
    SCI_MEMSET(wfilename,0,sizeof(wfilename));
    wlength = GUI_UTF8ToWstr(wfilename,255, (uint8 *)fileName,strlen(fileName));
    
    JWE_LOG(kjava_dfs_stat2,("kjava_dfs_stat2 is running"));
    filehandle = MMIAPIFMM_CreateFile(wfilename,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);

    if (PNULL == filehandle)
    {
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    file_Size = MMIAPIFMM_GetFileSize(filehandle);
    
    if(file_Size == 0)
    {
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    *size = file_Size;
    
    MMIAPIFMM_CloseFile(filehandle);           
    return KJAVA_FILE_ERR_NONE;
}
