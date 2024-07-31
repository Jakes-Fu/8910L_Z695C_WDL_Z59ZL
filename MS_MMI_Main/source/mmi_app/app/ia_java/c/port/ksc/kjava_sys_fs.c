/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "kjava_sys_fs.h"
#include "os_api.h"
#include "mmifmm_export.h"
#include "mmijava_internal.h"
#include "sfs.h"
 
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define ASSERT_LOG(_x)  JWE_LOG_ERR(assert,_x)

#ifndef _AJSC_DEBUG
    #undef  JWE_LOG
    #define JWE_LOG(function,parameters)
#endif

#define JBLENDIA_FS_D	0
#define SUPPORT_JSR75

#define CONST_KJAVA_FILE_PATHSEP        '\\'
#define CONST_MAX_FILENAME_LEN      (256)

#ifdef IA_JAVA_SUPPORT_LOW_MEMORY

#ifdef SUPPORT_JSR75
    #ifdef UMEM_SUPPORT
    #define CONST_NUM_AUTHORTIES        (1 + (MMI_SDCARD_MAX_NUM+1)*2)
    #else
    #define CONST_NUM_AUTHORTIES        (1 + MMI_SDCARD_MAX_NUM*2)
    #endif
#else
    #ifdef UMEM_SUPPORT
    #define CONST_NUM_AUTHORTIES        (1 + MMI_SDCARD_MAX_NUM+1)       /* only \java\ is allowed */
    #else
    #define CONST_NUM_AUTHORTIES        (1 + MMI_SDCARD_MAX_NUM)       /* only \java\ is allowed */
    #endif
#endif

#else

#ifdef SUPPORT_JSR75
#define CONST_NUM_AUTHORTIES        (MMI_DEVICE_NUM*2)
#else
#define CONST_NUM_AUTHORTIES        (MMI_DEVICE_NUM+1)       /* only \java\ is allowed */
#endif

#endif

#define CONST_MAX_NUM_FILE_HANDLES  (20)      /* max file handles count */
#define CONST_MAX_NUM_FILES         (7)       /* max files count */
#define CONST_MAX_FILE_SIZE         (5*1024)  /* we reserve 5K for each file(s) */


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
const KJavaPathMappingEntry PathMappingEntry[CONST_NUM_AUTHORTIES] = 
{
#ifdef IA_JAVA_SUPPORT_LOW_MEMORY
    { "java", "E:\\System\\@Java\\", KJAVA_FILE_ROOT_PRIVATE}, 
    #ifdef UMEM_SUPPORT
    { "javad", "D:\\System\\@Java\\", KJAVA_FILE_ROOT_PRIVATE}, 	
    #endif
    { "javae","E:\\System\\@Java\\",  KJAVA_FILE_ROOT_PRIVATE},
    #ifdef DUAL_TCARD_SUPPORT
    { "javaf", "F:\\System\\@Java\\", KJAVA_FILE_ROOT_PRIVATE},
    #endif
    #ifdef SUPPORT_JSR75
    #ifdef UMEM_SUPPORT
    { "D:",   "D:\\",       KJAVA_FILE_ROOT_PUBLIC},  
    #endif
    { "E:",   "E:\\",       KJAVA_FILE_ROOT_PUBLIC},  
    #ifdef DUAL_TCARD_SUPPORT
    { "F:",   "F:\\",       KJAVA_FILE_ROOT_PUBLIC},
    #endif
    #endif

#else
    #ifdef WIN32
    { "java", "D:\\@Java\\", KJAVA_FILE_ROOT_PRIVATE},
    #else
    { "java", "C:\\@Java\\", KJAVA_FILE_ROOT_PRIVATE},
    #endif
    { "javac", "C:\\@Java\\",  KJAVA_FILE_ROOT_PRIVATE},  
    { "javad", "D:\\@Java\\",  KJAVA_FILE_ROOT_PRIVATE},  		
    { "javae", "E:\\@Java\\",  KJAVA_FILE_ROOT_PRIVATE},
#ifdef DUAL_TCARD_SUPPORT
    { "javaf", "F:\\@Java\\",  KJAVA_FILE_ROOT_PRIVATE},
#endif
    #ifdef SUPPORT_JSR75
    { "D:",   "D:\\",       KJAVA_FILE_ROOT_PUBLIC},
    { "E:",   "E:\\",       KJAVA_FILE_ROOT_PUBLIC},
#ifdef DUAL_TCARD_SUPPORT
    { "F:",   "F:\\",       KJAVA_FILE_ROOT_PUBLIC},  
#endif
    #endif
#endif
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
//***********************************************************************************************
//  Description:  getlastModify
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL KJavaLongSize getlastModify(SFS_DATE_T modify_Date, SFS_TIME_T modify_time);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//***********************************************************************************************
//  Description:Initializes the native file system component
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_fs_init( KJavaFsNotifyEvent callback )
{
    JWE_LOG(kjava_fs_init,("kjava fs init success\n"))    
}

//***********************************************************************************************
//  Description:  Finalizes the native file system component. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void kjava_fs_fini(void)
{
    
}

//***********************************************************************************************
//  Description:  Opens a file
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
KJavaFileHandle kjava_fs_open(const char *fileName, const int mode, signed long taskId, int* err )
{
    uint32          mymode = 0;//the open mode
    KJavaFileHandle handle = PNULL;//the return value of opened file's handle
    int32           wlength = 0; 
    uint16          filename_len = 0;
    uint16          wfilename[MMIFILE_FILE_NAME_MAX_LEN] = {0};
    
    if(PNULL == fileName)
    {
        JWE_LOG(kjava_fs_open,("fileName is PNULL"));
        *err = KJAVA_FILE_ERR_NOT_EXIST;
        return 0;
    }
    
    JWE_LOG(kjava_fs_open,("fileName: %s, mode: %d", fileName, mode)) 
    filename_len = strlen(fileName);
    
    if( filename_len > MMIFILE_FILE_NAME_MAX_LEN)
    {
        JWE_LOG(kjava_fs_open,("fileName is too long"));
        *err = KJAVA_FILE_ERR_ROOT_TOO_LONG;
        return 0;
    }
    
    //transfer the code of the filename
    SCI_MEMSET(wfilename,0,sizeof(wfilename));
    wlength = GUI_UTF8ToWstr(wfilename,MMIFILE_FILE_NAME_MAX_LEN,(uint8 *)fileName,filename_len);
    if(wlength > MMIFILE_FILE_NAME_MAX_LEN)
    {
        JWE_LOG(kjava_fs_open,("wfileName is too long"));
        *err = KJAVA_FILE_ERR_ROOT_TOO_LONG;
        return 0;
    }
    
    mymode = 0;
    //fill the open mode according to 'mode'
    if((mode & KJAVA_FILE_OPEN_READ)&& !(mode & KJAVA_FILE_OPEN_WRITE)&& !(mode & KJAVA_FILE_OPEN_RW))
    {
        mymode |= SFS_MODE_READ;
    }
    else if(!(mode & KJAVA_FILE_OPEN_READ) && (mode & KJAVA_FILE_OPEN_WRITE) &&!(mode & KJAVA_FILE_OPEN_RW))
    {
        mymode |= SFS_MODE_WRITE;
    }
    else if(!(mode & KJAVA_FILE_OPEN_READ)&& !(mode & KJAVA_FILE_OPEN_WRITE) &&(mode & KJAVA_FILE_OPEN_RW))
    {
        mymode |= (SFS_MODE_WRITE | SFS_MODE_READ);
    }
    else 
    {
        JWE_LOG(kjava_fs_open,("fs:mode invalid\n"))
        *err = KJAVA_FILE_ERR_IO_ERR;
        return 0;
    }
    if(((mode & KJAVA_FILE_OPEN_EXCL) != 0)&&((mode & KJAVA_FILE_OPEN_CREATE) == 0))
    {
        mymode |= SFS_MODE_OPEN_EXISTING;
    }
    else if(((mode & KJAVA_FILE_OPEN_EXCL) == 0)&&((mode & KJAVA_FILE_OPEN_CREATE) != 0))
    {        
       if (MMIAPIFMM_IsFileExist(wfilename,wlength))
       {
           MMIAPIFMM_DeleteFile(wfilename,PNULL);
       } 
       mymode |= SFS_MODE_OPEN_ALWAYS;
    }
    else
    {
       JWE_LOG(kjava_fs_open,("fs:mode invalid\n"))
       *err = KJAVA_FILE_ERR_IO_ERR;
       return 0;
    }

    handle = (KJavaFileHandle)MMIAPIFMM_CreateFile(wfilename, mymode, 0, 0);
    if(handle == 0)
    {
    	*err = KJAVA_FILE_ERR_IO_ERR;
  	    JWE_LOG(kjava_fs_open,("handle: 0x%08x, err: %d", handle, *err)) 
        return 0;
    }

    *err = KJAVA_FILE_ERR_NONE;
    JWE_LOG(kjava_fs_open,("handle: 0x%08x, err: %d", handle, *err)) 
    return handle;   
}

//***********************************************************************************************
//  Description:  Closes the opened file handle.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_close(KJavaFileHandle hFile, signed long taskId)
{ 
    JWE_LOG(kjava_fs_close,("hFile: 0x%08x", hFile));       
        
	if(SFS_NO_ERROR != MMIAPIFMM_CloseFile((SFS_HANDLE)hFile))
    {
        JWE_LOG(kjava_fs_close,("MMIAPIFMM_CloseFile fail"));  
        return KJAVA_FILE_ERR_IO_ERR;
    }

    return KJAVA_FILE_ERR_NONE;  
}

//***********************************************************************************************
//  Description:  Reads from the opened file.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_read(void* ptr, signed long size, signed long* read,
                             KJavaFileHandle hFile, signed long taskId)
{
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    
    if(PNULL == ptr ||NULL == hFile)
    {
        JWE_LOG(kjava_fs_read,("ptr is null"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_read,("size %d, hFile: 0x%08x", size, hFile))
    *read = 0;
    
    sfs_error = MMIAPIFMM_ReadFile((SFS_HANDLE)hFile, ptr, size, (uint32 *)read, PNULL);

    JWE_LOG(kjava_fs_read,("read %d bytes", *read)) 
        
    if (SFS_NO_ERROR == sfs_error)
    {
        if(*read <= 0)
        {
            return KJAVA_FILE_ERR_END_OF_FILE;
        }
    }
    else
    {
        return KJAVA_FILE_ERR_IO_ERR;
    }

    
    return KJAVA_FILE_ERR_NONE;
}

//***********************************************************************************************
//  Description:  Writes to the opened file. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_write(void* ptr, 
                   signed long length,                   
                   KJavaFileHandle hFile,
                   signed long   taskId )
{
    int32       written = 0;//written is not used 
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    
    if(PNULL == ptr ||NULL == hFile)
    {
        JWE_LOG(kjava_fs_write,("ptr or hFile is null"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_write,("hFile: 0x%08x, length:%d",hFile, length))
   
	sfs_error = MMIAPIFMM_WriteFile((SFS_HANDLE)hFile, ptr, (uint32)length, (uint32 *)&written, 0);
    
    if (SFS_NO_ERROR != sfs_error)
    {
        JWE_LOG(kjava_fs_write,("write fail"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_write,("write %d bytes", written))
    return KJAVA_FILE_ERR_NONE;
}

//***********************************************************************************************
//  Description:  Moves the file pointer to a designated location
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_seek(KJavaFileHandle hFile, KJavaLongSize* offset, int whence, signed long taskId)
{
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    
    if(PNULL == offset ||NULL == hFile)
    {
        JWE_LOG(kjava_fs_seek,("ptr or hFile is null"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_seek,("offset: %d, whence: %d", offset->low, whence))
	
	sfs_error = MMIAPIFMM_SetFilePointer((SFS_HANDLE)hFile, offset->low, whence);
    
    if (SFS_NO_ERROR != sfs_error)
    {
        JWE_LOG(kjava_fs_seek,("seek fail"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    return KJAVA_FILE_ERR_NONE;
}

//***********************************************************************************************
//  Description:  Query the position of the file pointer. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_tell(KJavaFileHandle hFile, KJavaLongSize* offset, signed long taskId)
{	
    uint32      cur_size = 0;
    int32       oper_result = -1;
    if(PNULL == offset ||NULL == hFile)
    {
        JWE_LOG(kjava_fs_tell,("offset or hFile is null"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_tell,("hFile: 0x%08x, offset: %d", hFile, offset->low));
    //reset offset value
    offset->low = 0;
	offset->high = 0;
    
	oper_result = MMIAPIFMM_GetFilePointer((SFS_HANDLE)hFile,SFS_SEEK_BEGIN);
    JWE_LOG(kjava_fs_tell,("MMIAPIFMM_GetFilePointer: oper_result%d", oper_result));
    
    if(oper_result < 0)
    {
        JWE_LOG(kjava_fs_tell,("MMIAPIFMM_GetFilePointer error"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    offset->low  = oper_result;
    
	if(SFS_NO_ERROR != SFS_GetFileSize((SFS_HANDLE)hFile,&cur_size))
    {
        JWE_LOG(kjava_fs_tell,("SFS_GetFileSize error "));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_tell,("SFS_GetFileSize: %d", cur_size))

	if(offset->low > cur_size) 
    {
        offset->low = cur_size;
    }
    
	offset->high = 0;
    JWE_LOG(kjava_fs_tell,("hFile: 0x%08x, offset: %d", hFile, offset->low));
    return KJAVA_FILE_ERR_NONE;
}

//***********************************************************************************************
//  Description:  Flushes cached data of the opened file. . 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_flush(KJavaFileHandle hFile, signed long taskId)
{
    SFS_FlushFile(hFile, PNULL);/*lint !e64*/
    JWE_LOG(kjava_fs_flush,("**********leave***********"))
    return KJAVA_FILE_ERR_NONE;
}

//truncate the file by given size
int kjava_fs_truncate(KJavaFileHandle hFile, KJavaLongSize* size )
{
    int32           written = 0, ret = 0;
    char            zero = 0, temp_char = 0;
    KJavaLongSize   cur_size = {0};
    uint32          file_size = 0;

    if(hFile == 0 ||PNULL == size)
    {
        JWE_LOG(kjava_fs_truncate,("size or hFile is null"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_truncate,("truncate to size: %d", size->low)) ;
    
	if(SFS_NO_ERROR != SFS_GetFileSize((SFS_HANDLE)hFile,&file_size))
    {
        JWE_LOG(kjava_fs_truncate,("SFS_GetFileSize error "));
        return KJAVA_FILE_ERR_IO_ERR;
    }

    cur_size.low = file_size;
    JWE_LOG(kjava_fs_truncate,("origianl size: %d", cur_size.low))
    
    zero = 0;  
    if(size->low == 0)
    {
    	ret = MMIAPIFMM_SetFileSize((SFS_HANDLE)hFile, 0);
    	if(ret != SFS_ERROR_NONE)
    	{
        	return KJAVA_FILE_ERR_IO_ERR;   
    	}
        MMIAPIFMM_SetFilePointer((SFS_HANDLE)hFile,0,0);	
        MMIAPIFMM_WriteFile((SFS_HANDLE)hFile,(void *)&zero,1,(uint32 *)&written,0);
    }

    if(size->low > cur_size.low)
    {
    	ret = MMIAPIFMM_SetFileSize((SFS_HANDLE)hFile, size->low);
    	if(ret != SFS_ERROR_NONE)
    	{
        	return KJAVA_FILE_ERR_IO_ERR;
    	}
    	MMIAPIFMM_SetFilePointer((SFS_HANDLE)hFile,size->low-1,0);
    	MMIAPIFMM_WriteFile((SFS_HANDLE)hFile,(void *)&zero,1,(uint32 *)&written,0);
    }
    if(size->low < cur_size.low)
    {
    	ret = MMIAPIFMM_SetFileSize((SFS_HANDLE)hFile, size->low);   
    	if(ret != SFS_ERROR_NONE)
    	{
        	return KJAVA_FILE_ERR_IO_ERR;
    	}
        MMIAPIFMM_SetFilePointer((SFS_HANDLE)hFile,size->low-1,0);
        MMIAPIFMM_ReadFile((SFS_HANDLE)hFile,&temp_char,1,(uint32 *)&written,0);
        MMIAPIFMM_SetFilePointer((SFS_HANDLE)hFile,size->low-1,0);
        MMIAPIFMM_WriteFile((SFS_HANDLE)hFile,(void *)&temp_char,1,(uint32 *)&written,0);
	}    
 
    JWE_LOG(kjava_fs_truncate,("Leave"))
    return KJAVA_FILE_ERR_NONE;
}
//***********************************************************************************************
//  Description:  Removes a file.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_remove(const char* fileName) 
{
    int32   ret = 0;
    int32   wlength = 0;  
    uint16  filename_len = 0;
    uint16  wfilename[MMIFILE_FILE_NAME_MAX_LEN] = {0};
    
    if(PNULL == fileName)
    {
        JWE_LOG(kjava_fs_remove,("fileName is null"));
        return KJAVA_FILE_ERR_NOT_EXIST;
    } 
    
    JWE_LOG(kjava_fs_remove,("remove %s", fileName))
    filename_len = strlen(fileName);
    
    if( filename_len > MMIFILE_FILE_NAME_MAX_LEN)
    {
        JWE_LOG(kjava_fs_remove,("fileName is too long"));
        return KJAVA_FILE_ERR_ROOT_TOO_LONG;
    }
    
    SCI_MEMSET( wfilename, 0, sizeof( wfilename ) );           
    wlength = GUI_UTF8ToWstr(wfilename,CONST_MAX_FILENAME_LEN,(uint8 *)fileName,filename_len); 
    ret = MMIAPIFMM_DeleteFile(wfilename,PNULL);
    JWE_LOG(kjava_fs_remove,("ret: %d", ret))        

    if((ret == SFS_ERROR_NONE)||(ret == SFS_ERROR_NOT_EXIST))
    {
        return KJAVA_FILE_ERR_NONE;
    }
    else if(ret == SFS_ERROR_IO_PENDING)
    {
        return KJAVA_FILE_ERR_NONE;
    }
    return KJAVA_FILE_ERR_IO_ERR;
}

//***********************************************************************************************
//  Description:  getlastModify
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL KJavaLongSize getlastModify(SFS_DATE_T modify_Date, SFS_TIME_T modify_time)
{
	uint32 sec = 0;
	__int64 lastmodify = 0;
    KJavaLongSize size = {0};
	uint32 date = 0;
	uint32 ret = 0;
    SCI_DATE_T modify_date = {0};
	
	modify_date.mday = modify_Date.mday;
	modify_date.mon  = modify_Date.mon;
	modify_date.year = modify_Date.year;

	JWE_LOG(getlastModify, ("enter, %d-%d-%d, %d:%d:%d", modify_date.year,modify_date.mon,modify_date.mday,modify_time.hour,modify_time.min,modify_time.sec))
    //lint -esym(628, TM_GetDayNumOfDate)
    ret = TM_GetDayNumOfDate(modify_date, &date); /*lint !e718  !e746  !e628*/
	JWE_LOG(getlastModify, ("M_GetDayNumOfDate ret: %d", ret))
	date += 365 * 10 + 2;//phone start time is 1980.1.1, but system time is 1970.1.1, so you must add days from 1970.1.1 to  1980.1.1
	JWE_LOG(getlastModify, ("date is %d", date))
	sec = date * 24 * 3600 + modify_time.hour * 3600 + modify_time.min * 60 + modify_time.sec;
    sec -= 3600 * 8; //sync with fuction kjava_core_getCurrentTime in kjava_sys_core.c   
	JWE_LOG(getlastModify, ("sec is %d", sec))
	lastmodify = ((int64_t)sec) * 1000;//((int64_t)sec + 946656000) * 1000;

	size.high = (unsigned long)(lastmodify >> 32);
	size.low = (unsigned long)(lastmodify);
	
	JWE_LOG(getlastModify, ("last modify time:high is 0x%08x, low is 0x%08x", size.high, size.low))	
	return size;
}

//***********************************************************************************************
//  Description:  Gets information about a file indicated by file name. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_stat2(const char *fileName, KJavaStat* stat) 
{
    MMIFILE_ERROR_E ret = 0;
    SFS_FIND_DATA_T file_sts = {0};
    SFS_HANDLE      file_ptr = PNULL;
    // MMIFILE_HANDLE handle = PNULL;
    int32           wlength = 0;
    uint16          wfilename[CONST_MAX_FILENAME_LEN] = {0};
    uint16          filename_len = 0;
    
    if(PNULL == fileName || PNULL == stat)
    {
        JWE_LOG(kjava_fs_stat2,("fileName is null"));
        return KJAVA_FILE_ERR_NOT_EXIST;
    }
    
    JWE_LOG(kjava_fs_stat2,("fileName: %s", fileName));
    filename_len = strlen(fileName);
        
    if( filename_len > MMIFILE_FILE_NAME_MAX_LEN)
    {
        JWE_LOG(kjava_fs_stat2,("fileName is too long"));
        return KJAVA_FILE_ERR_ROOT_TOO_LONG;
    }


    if(fileName[strlen(fileName)-1] == ':')
    {
        if(fileName[0] == 'E' || fileName[0] == 'D' 
            || fileName[0] == 'e' || fileName[0] == 'd' )
        {
            JWE_LOG(kjava_fs_stat2,("this file maybe is D: ore E: root"));
            stat->attr = KJAVA_FILE_ATTR_READABLE | KJAVA_FILE_ATTR_WRITABLE | KJAVA_FILE_ATTR_DIRECTORY;
            stat->size.high = 0;
            stat->size.low = 0; 
            return KJAVA_FILE_ERR_NONE;
        }
    }	

    SCI_MEMSET(wfilename,0,sizeof(wfilename));
    wlength = GUI_UTF8ToWstr(wfilename,CONST_MAX_FILENAME_LEN, (uint8 *)fileName,strlen(fileName));
   
    JWE_LOG(kjava_fs_stat2,("wfilename: %s", wfilename));
    
    file_ptr = MMIAPIFMM_FindFirstFile(wfilename,wlength,&file_sts);
    if(PNULL == file_ptr)
    {
        return KJAVA_FILE_ERR_NOT_EXIST;
    }

    JWE_LOG(kjava_fs_stat2,("file_sts.attr: %d, file_ptr = 0x%x", file_sts.attr, file_ptr));
    stat->attr &= 0x0;   //赋值前先清零 防止保留上次的结果
    stat->attr |= KJAVA_FILE_ATTR_READABLE;
    if(!(file_sts.attr & SFS_ATTR_RO))
    {
        stat->attr |= KJAVA_FILE_ATTR_WRITABLE;
    }
    if(file_sts.attr & SFS_ATTR_HIDDEN)
    {
        stat->attr |= KJAVA_FILE_ATTR_HIDDEN;
    }
    if(file_sts.attr & SFS_ATTR_DIR)
    {
        stat->attr |= KJAVA_FILE_ATTR_DIRECTORY;
    }
    if(file_sts.attr & SFS_ATTR_ARCH)
    {
    	stat->attr |= KJAVA_FILE_ATTR_READABLE + KJAVA_FILE_ATTR_WRITABLE;
    }
    
    if(file_sts.attr == 0)
    {
    	stat->attr = KJAVA_FILE_ATTR_READABLE + KJAVA_FILE_ATTR_WRITABLE;
    }
  
    stat->size.high = 0;
    stat->size.low = file_sts.length;
    
    stat->lastModify = getlastModify(file_sts.modify_Date , file_sts.modify_time);                 
    ret = MMIAPIFMM_FindClose(file_ptr);  
    if (SFS_NO_ERROR != ret)
    {
        return KJAVA_FILE_ERR_IO_ERR;
    }
        
    JWE_LOG(kjava_fs_stat2,("stat->attr %d, stat->size.high: %d, stat->size.low: %d", stat->attr, stat->size.high, stat->size.low))

    return KJAVA_FILE_ERR_NONE;
}

//***********************************************************************************************
//  Description:  Gets the available space for the designated path.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_available(const char* path, KJavaLongSize* total,
                                         KJavaLongSize* free_size )
{
	int32 wlength = 0;
    uint16 wpathname[CONST_MAX_FILENAME_LEN] = {0};
    KJavaLongSize used = {0},frees = {0};
   // int32 ret = 0;
    wchar device_name [MMIFILE_DEVICE_NAME_MAX_LEN+1] = {0};
    uint16 device_name_len = 0;
	MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    
    if(PNULL == path || PNULL == total ||PNULL == free_size)
    {
        JWE_LOG(kjava_fs_available,("fileName is null"));
        return KJAVA_FILE_ERR_ROOT_NOT_EXIST;
    }
    
    SCI_MEMSET(total,0,sizeof(KJavaLongSize));
	SCI_MEMSET(free_size,0,sizeof(KJavaLongSize));
	SCI_MEMSET(&used,0,sizeof(KJavaLongSize));
	SCI_MEMSET(&frees,0,sizeof(KJavaLongSize));
    JWE_LOG(kjava_fs_available,("path: %s", path))
    wlength = GUI_UTF8ToWstr(wpathname,CONST_MAX_FILENAME_LEN,(uint8 *)path,strlen(path));
    if (!MMIAPIFMM_SplitFullPath(
                wpathname, wlength,
                device_name,&device_name_len, 
                PNULL, PNULL, PNULL, PNULL
                ))
    {
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
	device_type =  MMIAPIFMM_GetDeviceTypeByPath(device_name, MMIFILE_DEVICE_LEN);
    if(MMI_DEVICE_NUM > device_type)
    {
        MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(device_type),MMIAPIFMM_GetDevicePathLen(device_type),&frees.high, &frees.low);
        MMIAPIFMM_GetDeviceUsedSpace(MMIAPIFMM_GetDevicePath(device_type),MMIAPIFMM_GetDevicePathLen(device_type),&used.high, &used.low);
    }
	else
    {
        MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM),MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM),&frees.high, &frees.low);
        MMIAPIFMM_GetDeviceUsedSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM),MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM),&used.high, &used.low);
    }
	
    total->low = used.low + frees.low;
    total->high = 0;
    free_size->low = frees.low;
    free_size->high = 0;
    
    JWE_LOG(kjava_fs_available,("total->low: %d, free->low %d", total->low, free_size->low))

	return KJAVA_FILE_ERR_NONE;
}

//***********************************************************************************************
//  Description:  Renames a file
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_rename(const char* oldName, const char* newName)
{
    int32       ret = 0;			//return value
    uint16      woldname[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};//old file's name
    uint16      wnewname[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};//new file's name
    uint16      new_file_name[MMIFILE_FILE_NAME_MAX_LEN+1]={0}; 
    uint16      old_wlength = 0;		//file length
    uint16      new_wlength = 0;
    uint16      new_file_name_len=0;
    
    if(PNULL == oldName || PNULL == newName)
    {
        JWE_LOG(kjava_fs_rename,("fileName is null"));
        return KJAVA_FILE_ERR_NOT_EXIST;
    }  
    
    JWE_LOG(kjava_fs_rename,("oldName: %s, newName: %s", oldName, newName))
    //transfer the code of the file name(full path)
    SCI_MEMSET(wnewname,0,sizeof(wnewname));
    SCI_MEMSET(woldname,0,sizeof(woldname));
    old_wlength = GUI_UTF8ToWstr(woldname,CONST_MAX_FILENAME_LEN,(uint8 *)oldName,strlen(oldName));   
    new_wlength = GUI_UTF8ToWstr(wnewname,CONST_MAX_FILENAME_LEN,(uint8 *)newName,strlen(newName));

    if(!MMIAPIFMM_SplitFullPath(wnewname, new_wlength, NULL, NULL, NULL, NULL, new_file_name, &new_file_name_len))
    {
        JWE_LOG(kjava_fs_rename,("MMIAPIFMM_SplitFullPath fail"))
        return KJAVA_FILE_ERR_IO_ERR;
    }
    //call the SFS interface to rename the file    
    //ret = MMIAPIFMM_RenameFile(woldname, old_wlength,wnewname, new_wlength);
    ret = MMIAPIFMM_RenameFile(woldname, old_wlength,new_file_name, new_file_name_len);
    
    JWE_LOG(kjava_fs_rename,("ret %d", ret))

    if(FALSE == ret)
    {
        return KJAVA_FILE_ERR_IO_ERR;
    }

    return KJAVA_FILE_ERR_NONE;
}

//***********************************************************************************************
//  Description:  Gets the path separator character of the native file system.
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
unsigned char kjava_fs_getPathSeparator(void)
{ 
	JWE_LOG(kjava_fs_getPathSeparator,("kjava getPathSeparator is being called"))   
    return CONST_KJAVA_FILE_PATHSEP;
}

//***********************************************************************************************
//  Description:  Gets the number of path mappings for the platform
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int32 kjava_fs_getPathCount(void)
{
  	JWE_LOG(kjava_fs_getPathCount,("kjava getPathCount is being called"))
    return CONST_NUM_AUTHORTIES;
}

//***********************************************************************************************
//  Description: Gets a path mapping for the platform. 
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_fs_getPath(int index, KJavaPathMappingEntry* map)
{
    JWE_LOG(kjava_fs_getPath,("kjava getPath is being called"))

    if (index >= 0 && index < CONST_NUM_AUTHORTIES) 
    {
            
   	    JWE_LOG(kjava_fs_getPath,("kjava getPath is getpath"))
        MMI_MEMCPY(map->name,KJAVA_MAX_AUTHORITY_NAME_LENGTH, PathMappingEntry[index].name,KJAVA_MAX_AUTHORITY_NAME_LENGTH,KJAVA_MAX_AUTHORITY_NAME_LENGTH);
        MMI_MEMCPY(map->path,KJAVA_MAX_PATH_LENGTH, PathMappingEntry[index].path,KJAVA_MAX_PATH_LENGTH,KJAVA_MAX_PATH_LENGTH);
	    JWE_LOG(kjava_fs_getPath,("kjava getPath map->name is %s",map->name));

       	map->attr = PathMappingEntry[index].attr;

        #ifdef IA_JAVA_SUPPORT_LOW_MEMORY
        if(0 == index)
        {
            char            device_path[MMIFILE_DEVICE_LEN +1] = {0};
            MMIFILE_DEVICE_E device_type = MMIAPIFMM_GetFirstValidSD();
            if(device_type > MMI_DEVICE_SDCARD)
            {
                const wchar*     device_ptr = MMIAPIFMM_GetDevicePath(device_type);
                MMI_WSTRNTOSTR(device_path,MMIFILE_DEVICE_LEN,device_ptr,MMIFILE_DEVICE_LEN,MMIFILE_DEVICE_LEN);
                map->path[0] = device_path[0];
            }
        }
        #endif
       	return KJAVA_FILE_ERR_NONE;
	}
    JWE_LOG(kjava_fs_getPath,("kjava getPath is KJAVA_FILE_ERR_IO_ERR"));
	return KJAVA_FILE_ERR_IO_ERR;
}

#endif
