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
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "jblend_config.h"
#ifdef JAVA_SUPPORT_SHRINK_RAM
#include "kjava_shrink_ram.h"
#endif
#include "kjava_sys_fs.h"
#include "kjava_sys_fs_adv.h"
#include "sfs.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmifmm_export.h"
#include "jwe_log.h"
#include "mmk_app.h"
#include "gui_ucs2b_converter.h" //for gui_utf8towstr
 
/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define SUPPORT_JSR75
#define ASSERT_LOG(_x)  JWE_LOG_ERR(assert,_x)
#define KJAVA_MAX_UTF_FILE_NAME_LENGTH  ((KJAVA_MAX_FILE_NAME_LENGTH -1)*3+1)

#ifndef _AJSC_DEBUG
    #undef  JWE_LOG
    #define JWE_LOG(function,parameters)
#endif

typedef struct _kjava_fs_DirentTag
{
    unsigned char name[KJAVA_MAX_UTF_FILE_NAME_LENGTH];
    int32           isHidden;
} kjava_fs_Dirent;

typedef struct _kjava_fs_DirTag
{
    int32 handle;
    kjava_fs_Dirent entry;    
    kjava_fs_Dirent *entries;
    int32 index;
    int32 count;
} kjava_fs_Dir;

/* check if filter is regular express of src */
static int rec_filter( const char* src, const char* filter );
static BOOLEAN is_parantDirExist(const char* dirName);

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef JAVA_SUPPORT_SHRINK_RAM
static char  m_retName[KJAVA_MAX_UTF_FILE_NAME_LENGTH] = {0};
#else
DEFINE_STATIC_SHRINK_VAR_1_DIM(char,m_retName)
#endif

#ifdef JAVA_SUPPORT_SHRINK_RAM
BEGIN_DEFINE_SHRINK_VAR_INIT(kjava_adv_fs)
	SHRINK_VAR_MEMORY_1_DIM(char,m_retName,KJAVA_MAX_UTF_FILE_NAME_LENGTH)
END_DEFINE_SHRINK_VAR_INIT

BEGIN_DEFINE_SHRINK_VAR_FINI(kjava_adv_fs)
	FREE_SHRINK_VAR_MEMORY(m_retName)
END_DEFINE_SHRINK_VAR_FINI	
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//***********************************************************************************************
//  Description:kjava_fs_setAttr
//  Author:huangjc
//  Date:2007-09-03
//  Note:设置文件系统的属性，平台暂无接口支持!
//***********************************************************************************************
int kjava_fs_setAttr( const char* filename, unsigned int mode )
{
#ifdef SUPPORT_JSR75
    wchar       wfilename [KJAVA_MAX_FILE_NAME_LENGTH] = {0};
    uint16      attr = SFS_ATTR_NONE;
    SFS_ERROR_E result;  
    
    //SCI_TRACE_LOW:"kjava_fs_setAttr enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_ADV_FS_68_112_2_18_2_23_7_351,(uint8*)"");
    JWE_LOG(kjava_fs_setAttr,("kjava_fs_setAttr enter"));
    JWE_LOG(kjava_fs_setAttr,("kjava_fs_setAttr mode is %d",mode));

    if ( PNULL == filename)
	{
        return KJAVA_FILE_ERR_NOT_EXIST;
    }

    JWE_LOG(kjava_fs_setAttr,("kjava_fs_setAttr filename is %s",filename));
    
    GUI_UTF8ToWstr(wfilename,KJAVA_MAX_FILE_NAME_LENGTH -1,(const uint8 *)filename,strlen(filename));

    if(!(mode & KJAVA_FILE_ATTR_WRITABLE)) 
    {
        attr |= SFS_ATTR_RO;
    }
    
    if(mode & KJAVA_FILE_ATTR_HIDDEN)
    {
        attr |= SFS_ATTR_HIDDEN;
    }
    
    result = SFS_SetAttr(wfilename,attr);
    
    if(result == SFS_ERROR_NOT_EXIST)
    {
        return  KJAVA_FILE_ERR_NOT_EXIST;
    }
    else if(result != SFS_NO_ERROR)
    {
        return  KJAVA_FILE_ERR_IO_ERR;
    }
    else
    {
        return KJAVA_FILE_ERR_NONE;
    }
#else
    return KJAVA_FILE_ERR_IO_ERR;
#endif
}

//***********************************************************************************************
//  Description:kjava_fs_truncate2
//  Author:huangjc
//  Date:2007-09-03
//  Note:Set the size of a file!
//***********************************************************************************************
int kjava_fs_truncate2(const char *fileName, KJavaLongSize  *size )
{
#ifdef SUPPORT_JSR75
    KJavaFileHandle filehandle;
    int error;
    SFS_ERROR_E result;  
    // SFS_HANDLE  sfsHandle;
    int err;

    JWE_LOG(kjava_fs_truncate,("truncate to size: %d", size->low))
   
    if(PNULL == fileName)
    {
        return KJAVA_FILE_ERR_IO_ERR;
    }
    

	filehandle  = kjava_fs_open( fileName,KJAVA_FILE_OPEN_RW|KJAVA_FILE_OPEN_EXCL ,0, &err);
	if(filehandle == 0)
    {
      return KJAVA_FILE_ERR_NOT_EXIST;
    }

	JWE_LOG(kjava_fs_truncate2,("kjava_fs_truncate2 CreateFile is KO"));

	error = kjava_fs_truncate(filehandle,size);

	result = kjava_fs_close(filehandle,0);
	if(result != KJAVA_FILE_ERR_NONE)
		return KJAVA_FILE_ERR_IO_ERR;

    return error;
#else
    return 0;
#endif
}

//***********************************************************************************************
//  Description:kjava_fs_opendir
//  Author:huangjc
//  Date:2007-09-03
//  Note:Open a dir!
//***********************************************************************************************
KJavaDirHandle kjava_fs_opendir( const char* dirName, int *err )
{
#ifdef SUPPORT_JSR75
    kjava_fs_Dirent *dirContent;
    kjava_fs_Dir    *dirHandle;
    SFS_HANDLE      handle;
    //char            utf8_strbuf[KJAVA_MAX_UTF_FILE_NAME_LENGTH]={0};
    int             wlength;  
    uint16          wfilename[KJAVA_MAX_FILE_NAME_LENGTH] = {0};
    SFS_FIND_DATA_T findData;
    uint16          utf8_path_len = 0;

    if(PNULL == dirName || PNULL == err)
    {
        return (KJavaDirHandle) NULL;
    }
    
    JWE_LOG(kjava_fs_opendir,( "Enter (dirName = %s, err = %x)", dirName, err));
    
    #if 0
    utf8_path_len = SCI_STRLEN(dirName);
    
    SCI_MEMCPY((char*)utf8_strbuf,(const char*)dirName,utf8_path_len);

    for (i = utf8_path_len - 1; i > 0; i--) //get clean path without '\\' in the end
    {
        if (utf8_strbuf[i] == '\\')
        {
            utf8_strbuf[i] = '\0';
        }
        else
        {
            break;
        }
    }
    
    JWE_LOG(kjava_fs_opendir,( "utf8_strbuf = %s", utf8_strbuf))
    #endif
    SCI_MEMSET(wfilename,0,sizeof(wfilename));
    wlength = GUI_UTF8ToWstr((wchar *)wfilename,KJAVA_MAX_FILE_NAME_LENGTH -1, (const uint8 *)dirName,strlen(dirName));

    if(wlength >= (KJAVA_MAX_FILE_NAME_LENGTH -1))
    {
        *err = KJAVA_FILE_ERR_ROOT_TOO_LONG;
        return NULL;
    }
    
    JWE_LOG(kjava_fs_opendir,( "wlength = %d", wlength))

    if(!MMIAPIFMM_IsFolderExist(wfilename, wlength))
    {
        *err = KJAVA_FILE_ERR_IO_ERR;
        return (KJavaDirHandle) NULL;
    }

    if(wlength>2)
    {
        if (wfilename[wlength - 2] != '/' || wfilename[wlength - 2] != '\\' 
        || wfilename[wlength - 1] != '*') 
        {
            if (wfilename[wlength - 1] == L'/' || wfilename[wlength - 1] == L'\\')
            {
                wfilename[wlength++] = '*';
            } 
            else
            {
                wfilename[wlength++] = L'\\';
                wfilename[wlength++] = '*';
            }
        }
    }
    
    handle = MMIAPIFMM_FindFirstFile(wfilename, wlength, &findData);
    JWE_LOG(kjava_fs_opendir,( "SFS_FindFirstFile handle= %d", handle));
    dirHandle = (kjava_fs_Dir *) SCI_ALLOCA(sizeof(kjava_fs_Dir));
    dirContent = (kjava_fs_Dirent *) SCI_ALLOCA(sizeof(kjava_fs_Dirent));
    SCI_MEMSET(dirContent, 0, sizeof(kjava_fs_Dirent));
    utf8_path_len = strlen((char *)dirName);
    SCI_MEMCPY((char*)dirContent->name, (const char*)dirName,utf8_path_len);

    dirHandle->entries = dirContent;
    
    if(handle != NULL)
    {
        if(MMIAPIFMM_FindClose(handle) != SFS_ERROR_NONE)
        {
            JWE_LOG(kjava_fs_opendir,( "SFS_CloseFile error! "))
            *err = KJAVA_FILE_ERR_IO_ERR;
            SCI_FREE(dirContent);
            SCI_FREE(dirHandle);
            return (KJavaDirHandle) NULL;
        }
    }
//  strlength = GUI_UCS2B2UTF8((dirContent->name),sizeof(dirContent->name),(const uint8 *)findData.name,sizeof(findData.name));
    *err = KJAVA_FILE_ERR_NONE;

//    JWE_LOG(kjava_fs_opendir,( "Return (dirContent->name = %s(%x), err = %d)", dirContent->name, dirHandle, *err))

    return (KJavaDirHandle)dirHandle;

#endif
}

//***********************************************************************************************
//  Description:kjava_fs_opendir
//  Author:huangjc
//  Date:2007-09-03
//  Note:Open a dir! 
//           !!Make sure param "filter" passed is a string with '\0' in the end!!!
//***********************************************************************************************
const char* kjava_fs_readdir(KJavaDirHandle hDir, const char* filter, int showHidden, int *err)
{
#ifdef SUPPORT_JSR75
    static int          m_isFirst = 1;
    char                strbuf[KJAVA_MAX_UTF_FILE_NAME_LENGTH] ={0};
    kjava_fs_Dirent     *dirEntry = NULL;
    kjava_fs_Dir        *dirHandle = (kjava_fs_Dir *)hDir;
    int                 wlength;  
    // int                 strlength;
    uint16              wfilename[KJAVA_MAX_FILE_NAME_LENGTH];
    static SFS_HANDLE   m_handle;
    SFS_FIND_DATA_T     findData;
    static int          is_filter = 0;
    uint16              utf8_path_len = 0;
    
    if(PNULL == filter)
    {
        *err = KJAVA_FILE_ERR_IO_ERR;
        return NULL;
    }
    
    dirEntry = &dirHandle->entries[0];
    
    JWE_LOG(kjava_fs_readdir,( "Enter (hDir = %x, filter = %s, showHidden = %d, err = %x)", hDir, ((filter==NULL)?("NULL"):(filter)), showHidden, err))
    JWE_LOG(kjava_fs_readdir,( "Enter (dirEntry->name = %s)", dirEntry->name))
    utf8_path_len = SCI_STRLEN((char *)dirEntry->name);
    
    SCI_MEMCPY((char*)strbuf,(const char*)dirEntry->name,utf8_path_len);
//    SCI_STRCPY((char*)strbuf, (const char*)dirEntry->name );
    
    if(strbuf[strlen(strbuf) - 1] == '\\')
    {
       strcat(strbuf,"*");
    }
    else
    {
        strcat(strbuf, "\\*");              
    }

    SCI_MEMSET(wfilename,0,sizeof(wfilename));
    wlength = GUI_UTF8ToWstr((wchar *)wfilename,KJAVA_MAX_FILE_NAME_LENGTH-1, (uint8 *)strbuf,strlen(strbuf));

    if(wlength >= (KJAVA_MAX_FILE_NAME_LENGTH -2))
    {
        *err = KJAVA_FILE_ERR_NOT_EXIST;
        return NULL;
    }
    if(m_isFirst == 1)
    {
        m_handle = MMIAPIFMM_FindFirstFile(wfilename, wlength, &findData);
        if(m_handle != PNULL)
        {
            do 
            {
                SCI_MEMSET(m_retName, 0, KJAVA_MAX_UTF_FILE_NAME_LENGTH);
                GUI_WstrToUTF8((uint8*)m_retName,KJAVA_MAX_UTF_FILE_NAME_LENGTH -1,(const wchar *)findData.name, MMIAPICOM_Wstrlen(findData.name));
                if ( rec_filter(m_retName, filter ) == 1 )
                {
                   // uint16  file_name_len = 0 ;
                   
                    is_filter = 1;
                    m_isFirst = 0 ;
                    *err = KJAVA_FILE_ERR_NONE;
                    return (const char*)m_retName;              
                }
            }while (SFS_NO_ERROR == MMIAPIFMM_FindNextFile(m_handle, &findData)); 
             
            MMIAPIFMM_FindClose(m_handle);
            m_isFirst = 1 ;
            is_filter = 0;
            *err = KJAVA_FILE_ERR_END_OF_DIRECTORY;
            return NULL ;
            
        }
        else  //the folder is empty ,i don't know the return value as followed can meet to the request or not 
        {           
            *err = KJAVA_FILE_ERR_END_OF_DIRECTORY;
            return NULL ;
        }   
    }
    else
    {
        while (SFS_NO_ERROR == MMIAPIFMM_FindNextFile(m_handle, &findData))
        {
            SCI_MEMSET(m_retName, 0, KJAVA_MAX_UTF_FILE_NAME_LENGTH);
            GUI_WstrToUTF8((uint8*)m_retName,KJAVA_MAX_UTF_FILE_NAME_LENGTH-1,(const wchar *)findData.name,MMIAPICOM_Wstrlen(findData.name));
            if ( rec_filter(m_retName, filter ) == 1 )
            {
                // uint16  file_name_len = 0 ;

                is_filter = 1;
                m_isFirst = 0 ;

                *err = KJAVA_FILE_ERR_NONE;
                return (const char*)m_retName;              
            }
        }
        MMIAPIFMM_FindClose(m_handle);
        m_isFirst = 1 ;
        is_filter = 0;
        *err = KJAVA_FILE_ERR_END_OF_DIRECTORY;
        return NULL ;
    }

#else
    return (const char*)0;
#endif
}

//***********************************************************************************************
//  Description:kjava_fs_sizedir
//  Author:huangjc
//  Date:2007-09-03
//  Note:Calculate size of a dictory!
//***********************************************************************************************
int kjava_fs_sizedir( const char*     dirName, 
                     int             includeSubDirs,
                     KJavaLongSize  *size,
                     signed long     taskId )
{
#ifdef SUPPORT_JSR75
    SFS_ERROR_E     error = 0;
    char            strbuf[KJAVA_MAX_UTF_FILE_NAME_LENGTH] = {0};
    int32           wlength = 0;  
    uint16          wfilename[KJAVA_MAX_FILE_NAME_LENGTH] = {0};
    SFS_HANDLE      hDir = 0;
    SFS_FIND_DATA_T findData = {0};
    int	            strlength;
    KJavaLongSize   subsize;
    uint16  dir_name_len = 0;
    
    JWE_LOG(kjava_fs_sizedir,("kjava_fs_sizedir enter"));
	
    size->high = 0;
    size->low = 0;
    
    if(dirName == NULL)
    {
        JWE_LOG(kjava_fs_sizedir,("kjava_fs_sizedir param is null"));
        size->high = 0;
        size->low = 0;
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_sizedir,("kjava_fs_sizedir dirName is %s",dirName));
    JWE_LOG(kjava_fs_sizedir,("kjava_fs_sizedir includeSubDirs is %d",includeSubDirs));
    JWE_LOG(kjava_fs_sizedir,("kjava_fs_sizedir size->high is %d",size->high));
    JWE_LOG(kjava_fs_sizedir,("kjava_fs_sizedir size->low is %d",size->low));
    JWE_LOG(kjava_fs_sizedir,("kjava_fs_sizedir taskId is %d",taskId));
    
    dir_name_len = strlen(dirName);

    SCI_MEMCPY((char*)strbuf,(const char*)dirName,dir_name_len);
    
    SCI_MEMSET(wfilename,0,sizeof(wfilename));
    wlength = GUI_UTF8ToWstr((wchar *)wfilename,KJAVA_MAX_FILE_NAME_LENGTH-1, (uint8 *)strbuf,dir_name_len);
    if(wlength >= (KJAVA_MAX_FILE_NAME_LENGTH -2))
    {
        size->high = 0;
        size->low = 0;
        return KJAVA_FILE_ERR_NOT_EXIST;
    }

    if(!MMIAPIFMM_IsFolderExist(wfilename, wlength))
    {
        size->high = 0;
        size->low = 0;
        return KJAVA_FILE_ERR_IO_ERR;
    }

    if(strbuf[strlen(strbuf) - 1] == '\\')
    {
        strcat(strbuf, "*");
    }
    else
    {
        strcat(strbuf, "\\*");
    }
    
    SCI_MEMSET(wfilename,0,sizeof(wfilename));
    wlength = GUI_UTF8ToWstr(wfilename,KJAVA_MAX_FILE_NAME_LENGTH-1, (uint8 *)strbuf,strlen(strbuf));
    
    hDir = MMIAPIFMM_FindFirstFile(wfilename, wlength,&findData);   
    if((int)hDir < 0)
    {
    	size->high = 0;
    	size->low = 0;
        return KJAVA_FILE_ERR_NOT_EXIST;
    }
    if (hDir == 0) 
    {
        size->high = 0;
        size->low = 0;
        return KJAVA_FILE_ERR_NONE;
    }
    
    if(includeSubDirs == 1 && (findData.attr & SFS_ATTR_DIR))
    {
    	JWE_LOG(kjava_fs_sizedir,("includeSubDirs = True  && file founded is a Dir"));
        strbuf[strlen(strbuf) - 1] = 0;//路径最后一个元素赋为0=='\0'
    	strlength = GUI_WstrToUTF8((uint8*)m_retName,KJAVA_MAX_UTF_FILE_NAME_LENGTH-1,(const wchar *)findData.name,MMIAPICOM_Wstrlen(findData.name));
    	strcat((char *)strbuf,(char *)m_retName);//把那个找到的Dir放到前面的路径后面去，构成完整路径
    	if(kjava_fs_sizedir(strbuf,1,&subsize,0) == KJAVA_FILE_ERR_NONE)
    	{
    		JWE_LOG(kjava_fs_sizedir,("AAAA size->low=%d",size->low));
    		JWE_LOG(kjava_fs_sizedir,("AAAA subsize->low=%d",subsize.low));
    		size->high = 0;
    		size->low += subsize.low;
    	}
    }
    else
    {
        JWE_LOG(kjava_fs_sizedir,("includeSubDirs = False"));
        JWE_LOG(kjava_fs_sizedir,("includeSubDirs = False  &&  file founded is a file  or  includeSubDirs = True  &&  file founded is a file"));
        size->high = 0;
        size->low += findData.length;
    }
	
    JWE_LOG(kjava_fs_sizedir,("dirsize is = %d",size->low));
	
    while (MMIAPIFMM_FindNextFile(hDir, &findData) == 0)
    {
        JWE_LOG(kjava_fs_sizedir,("FindNextFile findData's size = %d",findData.length));
			
        if(includeSubDirs == 1 && (findData.attr & SFS_ATTR_DIR))
        {
                JWE_LOG(kjava_fs_sizedir,("includeSubDirs = True  && file founded is a Dir"));
                strbuf[strlen(strbuf) - 1] = 0;
                strlength = GUI_WstrToUTF8((uint8*)m_retName,KJAVA_MAX_UTF_FILE_NAME_LENGTH-1,(const wchar *)findData.name,MMIAPICOM_Wstrlen(findData.name));
                strcat((char *)strbuf,(char *)m_retName);
                if(kjava_fs_sizedir(strbuf,1,&subsize,0) == KJAVA_FILE_ERR_NONE)
                {
                    JWE_LOG(kjava_fs_sizedir,("BBBB size->low=%d",size->low));
                    JWE_LOG(kjava_fs_sizedir,("BBBB subsize->low=%d",subsize.low));
                    size->high = 0;
                    size->low +=subsize.low;
                }
        }
        else
        {
            JWE_LOG(kjava_fs_sizedir,("includeSubDirs = False  &&  file founded is a file  or  includeSubDirs = True  &&  file founded is a file"));
            size->high = 0;
            size->low += findData.length;
         }
    }
    
    JWE_LOG(kjava_fs_sizedir,("dirsize is = %d",size->low));

    error = MMIAPIFMM_FindClose(hDir);
    if(error != SFS_ERROR_NONE)
    {
        size->high = 0;
        size->low = 0;
        return KJAVA_FILE_ERR_IO_ERR;
    }
        
    JWE_LOG(kjava_fs_sizedir,("kjava_fs_sizedir KJAVA_FILE_ERR_NONE"));

    return KJAVA_FILE_ERR_NONE;
#else
    return 0;
#endif
}

//***********************************************************************************************
//  Description:kjava_fs_closedir
//  Author:huangjc
//  Date:2007-09-03
//  Note:Close a dictory!
//***********************************************************************************************
int kjava_fs_closedir(KJavaDirHandle hDir)
{
#ifdef SUPPORT_JSR75
    kjava_fs_Dir    *dirHandle = (kjava_fs_Dir*)hDir; 
    JWE_LOG(kjava_fs_closedir,("close dir 0x%08x",hDir)) 
    if (PNULL != dirHandle)
    {
        if (PNULL != dirHandle->entries)
        {
            SCI_FREE(dirHandle->entries);
        }
        if (PNULL != dirHandle->handle)
        {
            MMIAPIFMM_FindClose(dirHandle->handle);
        }
        SCI_FREE(dirHandle);
    }
    JWE_LOG(kjava_fs_closedir,("leave"))
    return KJAVA_FILE_ERR_NONE;
#else
    return 0;
#endif
}

//***********************************************************************************************
//  Description:kjava_fs_mkdir
//  Author:huangjc
//  Date:2007-09-03
//  Note:Create a dictory!
//***********************************************************************************************
int kjava_fs_mkdir(const char* dirName)
{
#ifdef SUPPORT_JSR75
    int32       dirlen = 0;  
    uint16      dirtmp[KJAVA_MAX_FILE_NAME_LENGTH] = {0};
    SFS_ERROR_E error = 0;  
    JWE_LOG(kjava_fs_mkdir,("enter"));   
    if(PNULL == dirName)
    {
        JWE_LOG(kjava_fs_mkdir,("param is null"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    JWE_LOG(kjava_fs_mkdir,("dirName is %s",dirName));

    if (FALSE == is_parantDirExist(dirName))
    {
        JWE_LOG(kjava_fs_mkdir,("parant dir not exist!"));
        return KJAVA_FILE_ERR_IO_ERR;
    }

    SCI_MEMSET(dirtmp,0,sizeof(dirtmp));
    dirlen = GUI_UTF8ToWstr(dirtmp,KJAVA_MAX_FILE_NAME_LENGTH-1,(uint8 *)dirName,strlen(dirName));
    error = MMIAPIFMM_CreateDirectory(dirtmp);
    if(error == SFS_ERROR_HAS_EXIST)
    {
        JWE_LOG(kjava_fs_mkdir,("KJAVA_FILE_ERR_ALREADY_EXIST"));
        return KJAVA_FILE_ERR_ALREADY_EXIST;
    }
    if(error != SFS_NO_ERROR)
    {
        JWE_LOG(kjava_fs_mkdir,("KJAVA_FILE_ERR_IO_ERR"));
        return KJAVA_FILE_ERR_IO_ERR;
    }   

    JWE_LOG(kjava_fs_mkdir,("KJAVA_FILE_ERR_NONE")); 
    return KJAVA_FILE_ERR_NONE;
#else
    return 0;
#endif
}

//***********************************************************************************************
//  Description:kjava_fs_rmdir
//  Author:huangjc
//  Date:2007-09-03
//  Note:Remove a dictory!
//***********************************************************************************************
int kjava_fs_rmdir(const char* dirName)
{
#ifdef SUPPORT_JSR75
    int32           dirlen = 0;  
    uint16          dirtmp[KJAVA_MAX_FILE_NAME_LENGTH] = {0};
    SFS_ERROR_E     error = 0;
    char            strbuf[KJAVA_MAX_UTF_FILE_NAME_LENGTH] = {0};
    int32           wlength = 0;  
    uint16          wfilename[KJAVA_MAX_FILE_NAME_LENGTH] = {0};
    SFS_HANDLE      hDir = 0;
    SFS_FIND_DATA_T findData = {0};
    uint16          dir_name_len = 0;
    
    JWE_LOG(kjava_fs_rmdir,("enter"));
    
    if(PNULL == dirName)
    {
        JWE_LOG(kjava_fs_rmdir,("param is null"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    JWE_LOG(kjava_fs_rmdir,("dirName is %s",dirName));
    dir_name_len = strlen(dirName);
    
    SCI_MEMCPY((char*)strbuf,(const char*)dirName,dir_name_len);
   // SCI_STRCPY((char*)strbuf, (const char*)dirName );
    JWE_LOG(kjava_fs_rmdir,(" strbuf is %s",strbuf));
    if(strbuf[strlen(strbuf) - 1] == '\\')
    {
        strcat(strbuf, "*");
    }
    else
    {
        strcat(strbuf, "\\*");
    }
    
    SCI_MEMSET(wfilename,0,sizeof(wfilename));
    wlength = GUI_UTF8ToWstr(wfilename,KJAVA_MAX_FILE_NAME_LENGTH -1, (uint8 *)strbuf,strlen(strbuf));
    
    hDir = MMIAPIFMM_FindFirstFile(wfilename,wlength, &findData);
    JWE_LOG(kjava_fs_rmdir,("hDir = %d",hDir));
    if (hDir != PNULL) 
    {
        MMIAPIFMM_FindClose(hDir);
        JWE_LOG(kjava_fs_rmdir,("KJAVA_FILE_ERR_DIRECTORY_NOT_EMPTY"));
        return KJAVA_FILE_ERR_DIRECTORY_NOT_EMPTY;
    }    
    
    SCI_MEMSET(dirtmp,0,sizeof(dirtmp));
    dirlen = GUI_UTF8ToWstr(dirtmp,KJAVA_MAX_FILE_NAME_LENGTH -1,(uint8 *)dirName,strlen(dirName));
    
    error = MMIAPIFMM_DeleteDirectory(dirtmp);
    if(error == SFS_ERROR_NOT_EXIST)
    {
        JWE_LOG(kjava_fs_rmdir,("KJAVA_FILE_ERR_NOT_EXIST"));
        return KJAVA_FILE_ERR_NOT_EXIST;
    }
    if(error != SFS_NO_ERROR)
    {
        JWE_LOG(kjava_fs_rmdir,("KJAVA_FILE_ERR_IO_ERR"));
        return KJAVA_FILE_ERR_IO_ERR;
    }
    
    JWE_LOG(kjava_fs_rmdir,("KJAVA_FILE_ERR_NONE"));
    
    return KJAVA_FILE_ERR_NONE;
#else
    return 0;
#endif
}

//***********************************************************/
//  Description:kjava_fs_rmdir
//  Author:huangjc
//  Date:2007-09-03
//  Note:Remove a dictory!
//************************************************************************/
int kjava_fs_setRootChangeNotification( KJavaFsRootChange rootCallback )
{
#ifdef SUPPORT_JSR75
    //SCI_TRACE_LOW:"kjava_fs_setRootChangeNotification enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_ADV_FS_695_112_2_18_2_23_8_352,(uint8*)"");
    
    JWE_LOG(kjava_fs_setRootChangeNotification,("kjava_fs_setRootChangeNotification enter"));
    JWE_LOG(kjava_fs_setRootChangeNotification,("kjava_fs_setRootChangeNotification rootCallback is %d",rootCallback));
    
    JWE_LOG(kjava_fs_setRootChangeNotification,("leave"))
        
    return KJAVA_FILE_ERR_NONE;
#else
    return 0;
#endif
}

static int rec_filter( const char* src, const char* filter )
{
    int i;

    JWE_LOG(rec_filter,("compare \'%s\' and \'%s\'\n", src, filter));

    if ( *filter == '\0' && *src != '\0' ) /*lint !e661*/
    {
        return 0;
    }

    if ( *filter == '*' && strlen(filter) == 1 )
    {
        return 1;
    }

    if ( *filter != '\0' && *src == '\0' )
    {
        return 0;
    }

    if ( *filter == '\0' && *src == '\0' ) /*lint !e661*/
    {
        return 1;
    }

    if ( *filter == '*' )
    {
        /* ignore 1~strlen(src) char */
        for ( i = 0; i < (int)strlen(src); i ++ )
        {
            if ( rec_filter( src+i, filter+1 ) == 1 )
            {
                return 1;
            }
        }
    }
    else if ( *src == *filter )
    {
        if ( rec_filter( src+1, filter+1 ) == 1 ) /*lint !e661*/
        {
            return 1;
        }
    }

    return 0;
}


//***********************************************************************************************
//  Description:is parant dir exist
//  Author:pan.yang
//  Date:2009-12-08
//  Note:Check whether parant dir exist 
//           make sure dirName is valid before call this fuction
//***********************************************************************************************
static BOOLEAN is_parantDirExist(const char* dirName)
{
    uint16  dirname_len = 0;
    uint16 parantdir[KJAVA_MAX_PATH_LENGTH] = {0};
    uint16 parantdir_len = 0;
    int i;
    
    if (PNULL == dirName)
    {
        return FALSE;
    }

    dirname_len = strlen(dirName);
    if (dirname_len < 4)  //less than 2 level path-- eg: "e:\"
    {
        return FALSE;
    }

    if (dirName[dirname_len - 1] == '\\')
    {
        i = dirname_len - 2;
    }
    else 
    {
        i = dirname_len - 1;
    }

    for (; i > 1 && dirName[i] != ':'; i--)
    {
        if (dirName[i] == '\\')
        {
            SCI_MEMSET(parantdir, KJAVA_MAX_PATH_LENGTH * sizeof(uint16), 0);
            parantdir_len = GUI_UTF8ToWstr(parantdir,KJAVA_MAX_PATH_LENGTH-1,(uint8 *)dirName, i);
            if (TRUE == MMIAPIFMM_IsFolderExist(parantdir, parantdir_len))
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }

    return FALSE;
}

#endif

