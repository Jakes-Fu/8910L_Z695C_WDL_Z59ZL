//pclint
/*lint -e522 */
/*lint -e747 */
/*lint -e775 */
/*lint -e668 */
/*lint -e573 */
/*lint -e533 */
/*lint -e516 */
/*lint -e802 */
/*lint -e553 */
/*lint -e762 */
/*lint -e628 */
   
#include "cafhelp.h"
#include "mmi_common.h"
#include "mmifmm_export.h"
#include "dal_time.h"
#include "sfs.h"

#include "tf3rd_for_hz.h"
#include "tf3rd_for_hz_fun.h"
#include "we_mempool.h"

#include "eme_callback.h"
#include "eme_utils_api.h"

#ifndef E_FALSE
#define E_FALSE      0
#endif

#ifndef E_TRUE
#define E_TRUE       1
#endif

#ifndef E_SUCCESS
#define E_SUCCESS    0
#endif

#ifndef E_FAILED
#define E_FAILED     1
#endif

#ifndef E_BADPARAM
#define E_BADPARAM   2
#endif

#ifndef E_NOMEMORY
#define E_NOMEMORY   3
#endif

typedef struct _St_FileMgr
{
   char *pszDir;
   SFS_HANDLE hFile;
   unsigned char bDir;
}St_FileMgr;

/*check the file path whether is existed*/
int   g_nPathFlag; 		/*E_FASE or E_TRUE*/

/*file name for enum file name*/
char  g_pszFileName[128];

SFS_HANDLE g_hdbgFile = 0;

unsigned int g_mod_id = 0;

//St_TS_Timer m_astTSTimer[TS_TIMER_MAX] = {0};

#ifdef TF3RD_TARGET_DBG_INFO
#define TF3RD_DBG_FILE_INFO	         0    /*write debug info into specific file*/

#define TF3RD_DBG_ROOT_PATH		      L"C:\\ts"
#define TF3RD_DBG_MMS_PATH		         L"C:\\ts\\mms"
#define TF3RD_DBG_MMS_LOG_FILE	      L"C:\\ts\\mms\\mmslog.txt"
#define TF3RD_DBG_MMS_LOG_FILE_BAK	   L"C:\\ts\\mms\\mmslog_bak.txt"
#endif

#ifdef TF3RD_DBGINFO

#define TF3RD_DBGINFO                  Tf3rd2_59DbgPrintf
#define TF3RD_DBGINFO_START(x)         TF3RD_DBGINFO("TF3RD_DBGINFO----------%s %s", x, "start!")
#define TF3RD_DBGINFO_END(x)           TF3RD_DBGINFO("TF3RD_DBGINFO----------%s %s", x, "end!")

#else

#define TF3RD_DBGINFO
#define TF3RD_DBGINFO_START(x) 
#define TF3RD_DBGINFO_END(x) 

#endif


/*Memory Pool Define*/
static void*    g_email_mem_handle = NULL;

#define EMAIL_MAX_MEM_SIZE      0x00100000  /*1024 * 1024 *2*/
#define EMAIL_MEM_START         0x54454348  /*TECH*/
#define EMAIL_MEM_END           0x534F4654  /*SOFT*/          

typedef struct St_EmlMemInfo St_EmlMemInfo;
struct St_EmlMemInfo
{
   uint32  uiMemStart;
   struct St_EmlMemInfo *next;
   struct St_EmlMemInfo *prev; 
   char   filename[32];
   uint32 frommempool;
   uint32 line;
   uint32 uiSize;
   uint32 uiBlockSize;
   void   *pvMem;
};

typedef struct St_EmlMemCtl St_EmlMemCtl;
struct St_EmlMemCtl
{
   uint32 uiMemUsedSize;
   uint32 uiMemUsedNum;
   uint32 uiMemFreeSize;
   uint32 uiMemFreeNum;
   uint32 uiPeakValueSize;
   uint32 uiPeakValueNum;
   St_EmlMemInfo *email_mem_list;
   St_EmlMemInfo *email_mem_free_list;
};

static St_EmlMemCtl gst_email_mem_ctl = {0};


static void Tf3rd2_PrintMemoryCB(void);

static unsigned short* Tf3rd2_Utils_StrToWstr(char *pcStr)
{
   unsigned short *pwcStr = NULL;
   
   if(!pcStr || !Tf3rd2_18StrLen(pcStr))
      return NULL;
   
   pwcStr = TF3RD_MALLOCMEMORY((Tf3rd2_18StrLen(pcStr) + 1) * sizeof(unsigned short));
   if(!pwcStr)
      return NULL;
   
   Tf3rd2_4MemSet((void*)pwcStr, 0x00, (Tf3rd2_18StrLen(pcStr) + 1) * sizeof(unsigned short));

   Tf3rd2_27Str2Wstr((char *)pcStr,pwcStr, (Tf3rd2_18StrLen(pcStr) + 1 )*(sizeof(unsigned short)));

   return pwcStr;
}
/* =================================================================================

  FILE INTERFACE 
  
=================================================================================*/
static int Tf3rd2_Utils_isLittleEdain(void)
{
   int i = 1;
   return *(char*)&i;
}

/* ==================================================================================
Function:		Tf3rd0_1fclose()
Func Type:		Global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   10:41

Description:	close file function.

Parameters:		input:		void* f			file-handle

output:		NULL

Return value:	error code
================================================================================ */
int Tf3rd0_1fclose(void* f)
{
   int iRet = 0;
   SFS_HANDLE hFile = 0;
   
   /*check parameter*/
   if (NULL == f) 
   {
      /*return negative value*/
      return E_BADPARAM;
   }
   
   /*set file handle*/
   hFile = (SFS_HANDLE)f;
   
   /* using mtk's fs_close function, if success, the return value is FS_NO_ERROR,
   * else, return negative value.
   */
   iRet = SFS_CloseFile(hFile);

   return iRet;
}

/* ==================================================================================
Function:		Tf3rd0_2fopen()
Func Type:		Global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   11:08

Description:	open file interface

Parameters:		input:	const char *filename, 
const unsigned char * mode

output: NULL

Return value:	file handle
================================================================================ */
void* Tf3rd0_2fopen(const char *filename, int mode)
{
   void           *phFile = NULL;
   unsigned int   uiOpenMode = 0 ;
   unsigned char  bAppendMode = E_FALSE;
   SFS_HANDLE     hFile = 0 ;
   unsigned short *pwszFileName = NULL;
   
   /*check parameters*/
   if (NULL == filename) 
   {
      return NULL;
   }
   
   switch(mode)
   {
   case TF3RD_OFM_READ:
      uiOpenMode = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;
      break;
   case TF3RD_OFM_READWRITE:
      uiOpenMode = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ | SFS_MODE_WRITE;
      break;
      
   case TF3RD_OFM_CREATE:
      uiOpenMode = SFS_MODE_CREATE_NEW | SFS_MODE_READ | SFS_MODE_WRITE;
      break;
   case TF3RD_OFM_APPEND:
      uiOpenMode = SFS_MODE_APPEND | SFS_MODE_WRITE | SFS_MODE_READ;
      break;
   default:
      return NULL;
   }

   Tf3rd2_59DbgPrintf("open file = %s, mode = %d", filename, mode);
   
   /*transfer file name from string to wstring*/
   pwszFileName = TF3RD_MALLOCMEMORY((Tf3rd2_18StrLen(filename) + 1) * sizeof(wchar));
   if (NULL == pwszFileName)
   {
      return NULL;
   }
   
   Tf3rd2_4MemSet(pwszFileName, 0x00, (Tf3rd2_18StrLen(filename) + 1)* sizeof(wchar));
   Tf3rd2_29Utf8ToWstr(filename, Tf3rd2_18StrLen(filename), pwszFileName, Tf3rd2_18StrLen(filename) * sizeof(wchar));
      
   /* using mtk's fs_open function, if success, the return value is file handle,
   * else, return null.
   */
   hFile = SFS_CreateFile(pwszFileName, uiOpenMode, 0, 0);
   
   if (hFile == 0) 
   {
      TF3RD_DBGINFO("TF3RD_DBGINFO---------- FS_Open failed");
      Tf3rd2_7MemFree(pwszFileName);
      return NULL;
   }
   
   phFile = (void *)hFile;
   
   Tf3rd2_7MemFree(pwszFileName);
   
   return (void*)phFile;
}

/* ==================================================================================
Function:		Tf3rd0_3fread()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   13:07

Description:	read file

Parameters:		input:		void* buf, 
unsigned int size, 
unsigned int count, 
void* f

output:		null

Return value:	read number
================================================================================ */
int Tf3rd0_3fread(void* f, void* buf, unsigned int size)
{
 SFS_HANDLE   hFile = 0;
 unsigned int uiReadSize = 0;
 
 /*check parameters*/
 if ((NULL == buf) ||(NULL == f)) 
 {
    /*if parameter is NULL,return negative value*/
    return E_FAILED;
 }
 
 TF3RD_DBGINFO_START("Tf3rd0_3fread");
 
 /*get file handle*/
 hFile = (SFS_HANDLE)f;
 
 /*if on target, using MTK'S file operation interface*/
 SFS_ReadFile(hFile, buf, size, &uiReadSize, NULL);
 
 TF3RD_DBGINFO_END("Tf3rd0_3fread");
 
 /*return read size.*/
 return uiReadSize;
}
      
/* ==================================================================================
Function:		Tf3rd0_4fseek()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   13:33

Description:    seek file.

Parameters:		input:	void* f, 
int offset, 
int whence

output: NULL

Return value:
================================================================================ */
int Tf3rd0_4fseek(void* f, int offset, int whence)
{
   int iWhence = 0;
   SFS_HANDLE hFile = 0;
   
   /*check prameters*/
   if (NULL == f) 
   {
      return E_BADPARAM;
   }
   
   /*set file handle*/
   hFile = (SFS_HANDLE)f;
   
   /*if on target, transfer framework's whence into mtk's whence*/
   switch(whence) 
   {
   case TF3RD_SEEK_SET:
      iWhence = SFS_SEEK_BEGIN;
      break;
   case TF3RD_SEEK_CUR:
      iWhence = SFS_SEEK_CUR;
      break;
   case TF3RD_SEEK_END:
      iWhence = SFS_SEEK_END;
      break;
   default:
      iWhence = SFS_SEEK_CUR;
   }
   
   return ((SFS_SetFilePointer(hFile,offset,iWhence) >= 0) ? E_SUCCESS : E_FAILED);
}
                            
                            
/* ==================================================================================
Function:		Tf3rd0_5ftell()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   13:52

Description:	tell the cur position of file.

Parameters:		input:	void* f

output:	NULL

Return value:	cur position of file
================================================================================ */
int Tf3rd0_5ftell(void* f)
{
   /* will be implemented later if needed*/
   SFS_HANDLE hFile = 0;
   int nRet = E_FAILED;
   int32 nPos = 0;
   
   /*set file handle*/
   hFile = (SFS_HANDLE)f;
   
   nRet = SFS_GetFilePointer(hFile, SFS_SEEK_BEGIN, &nPos);

   return nPos;
}

/* ==================================================================================
Function:		Tf3rd0_6feof()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   13:56

Description:	test for end-of-file on a stream.

Parameters:		input:	void* f

output:	NULL

Return value:
================================================================================ */
int Tf3rd0_6feof(void* f)
{
   /* will be implemented later if needed*/
   //TODO:
   return 0;
}

/* ==================================================================================
Function:		Tf3rd0_7ferror()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   13:58

Description:	test for an error on a stream

Parameters:		input:	void* f

output:	NULL

Return value:
================================================================================ */
int Tf3rd0_7ferror(void* f)
{
   /* will be implemented later if needed*/
   //TODO:
   return 0;
}
                            
/* ==================================================================================
Function:		Tf3rd0_8fflush()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:00

Description:	flushes a stream.

Parameters:		input:	void* f

output:

Return value:
================================================================================ */
int Tf3rd0_8fflush(void* f)
{
   SFS_HANDLE hFile = 0;
   
   /*check parameters*/
   if (NULL == f) 
   {
      return E_BADPARAM;
   }
   
   /*get file handle*/
   hFile = (SFS_HANDLE)f;

   if(SFS_FlushFile(hFile, NULL) == SFS_NO_ERROR)
      return E_SUCCESS;
   else
      return E_FAILED;
}

/* ==================================================================================
Function:		Tf3rd0_9fwrite()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:01

Description:	writes data to a stream

Parameters:		input:	void* buf,
unsigned int size,
unsigned int count,
void* f

output:	NULL

Return value:	write number.
================================================================================ */
int Tf3rd0_9fwrite(void* f, void* buf, unsigned int size)
{
   unsigned int uiWriteCount = 0;
   int          iRet = 0;
   SFS_HANDLE   hFile = 0;
   
   /*check parameters*/
   if ((NULL == buf) || (NULL == f)) 
   {
      /*if parameters is null, return negative data.*/
      return E_BADPARAM;
   }
   
   TF3RD_DBGINFO_START("Tf3rd0_9fwrite");
   
   /*get file handle*/
   hFile = (SFS_HANDLE)f;
   
   /*if on target, using mtk's file wirte function*/
   SFS_WriteFile(hFile, buf, size, &uiWriteCount, NULL);
   
   TF3RD_DBGINFO_END("Tf3rd0_9fwrite");
   
   return uiWriteCount;
}

/* ==================================================================================
Function:		Tf3rd0_21CheckFileExist()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:13

Description:	check whether file exists or not.

Parameters:		input:   char * f

output:

Return value:	if fail, return negative value.
================================================================================ */
int Tf3rd0_21CheckFileExist(char * f)
{
   SFS_HANDLE     hFile = 0;
   unsigned short *pwszName = NULL;
   SFS_FIND_DATA_T *pstFindData = NULL;

   /*check parameters*/
   if (NULL == f) 
   {
      return E_BADPARAM;
   }

   pstFindData = TF3RD_MALLOCMEMORY(sizeof(SFS_FIND_DATA_T));
   if(NULL == pstFindData)
   {
      return E_FAILED;
   }
   Tf3rd2_4MemSet(pstFindData, 0x00, sizeof(SFS_FIND_DATA_T));

   pwszName = Tf3rd2_Utils_StrToWstr(f);
   if(!pwszName)
   {
      Tf3rd2_7MemFree(pstFindData);
      return E_FAILED;
   }
   
   hFile = SFS_FindFirstFile(pwszName, pstFindData);
   if(hFile == 0)
   {
      Tf3rd2_7MemFree(pwszName);
      Tf3rd2_7MemFree(pstFindData);
      return E_FAILED;
   }
   else
   {
      Tf3rd2_7MemFree(pwszName);
      Tf3rd2_7MemFree(pstFindData);
      SFS_FindClose(hFile);
      return E_SUCCESS;
   }
}
                            
/* ==================================================================================
Function:		Tf3rd0_22CreatFile()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:28

Description:	create file

Parameters:		input:

output:

Return value:
================================================================================ */
int Tf3rd0_22CreatFile(char * f)
{
   SFS_HANDLE     hFile = 0;
   unsigned short *pwszDirName = NULL;
   
   /*check parameters*/
   if (NULL == f) 
   {
      return E_BADPARAM;
   }
   
   TF3RD_DBGINFO_START("Tf3rd0_22CreatFile");
   pwszDirName = Tf3rd2_Utils_StrToWstr(f);
   if (NULL == pwszDirName)
   {
      return E_NOMEMORY;
   }
   
   /*if on target,invoke mtk's FS_Open(),whose mode is "FS_OPEN_DIR"*/
   hFile = SFS_CreateFile(pwszDirName, SFS_MODE_CREATE_NEW | SFS_MODE_READ | SFS_MODE_WRITE, 0, 0);
   if (hFile <= 0) 
   {
      Tf3rd2_7MemFree(pwszDirName);
      return E_FAILED;
   }
   
   SFS_CloseFile(hFile);
   
   Tf3rd2_7MemFree(pwszDirName);
   
   TF3RD_DBGINFO_END("Tf3rd0_22CreatFile");
   
   return E_SUCCESS;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:31

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
int Tf3rd0_10chdir(const char* path)
{
   /* will be implemented later if needed*/
   //TODO:
   return 0;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:32

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
char* Tf3rd0_11getcwd(char* buf, unsigned int size)
{
   /* will be implemented later if needed*/
   //TODO:
   return NULL;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:32

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
int Tf3rd0_12makedir(const char* path)
{
   unsigned short *pwszPath = NULL;
   int iRet = E_FAILED;

   /*check parameters*/
   if (NULL == path) 
   {
      return E_BADPARAM;
   }

   Tf3rd2_59DbgPrintf("make dir = %s", path);
   
   TF3RD_DBGINFO_START("Tf3rd0_12makedir");
   pwszPath = Tf3rd2_Utils_StrToWstr(path);
   if (NULL == pwszPath)
   {
      return E_NOMEMORY;
   }
   
   /*if on target, invoke mtk's FS_CreateDir()*/
   iRet = SFS_CreateDirectory(pwszPath);
//   MMIFILE_SetAttr(pwszPath, Tf3rd2_39WstrLen(pwszPath), TRUE, TRUE, TRUE, FALSE);

   Tf3rd2_7MemFree(pwszPath);
   
   TF3RD_DBGINFO_END("Tf3rd0_12makedir");
   
   return iRet;

}
                            
/* ==================================================================================
Function:		Tf3rd0_13remove()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:41

Description:	remove file

Parameters:		input:	const char* filename

output: NULL

Return value:	if fail, return negative value.
================================================================================ */
int Tf3rd0_13remove(const char* filename)
{
   unsigned short *pwszFileName = NULL;
   
   /*check parameters*/
   if (NULL == filename) 
   {
      return E_BADPARAM;
   }
   
   TF3RD_DBGINFO_START("Tf3rd0_13remove");
   
   pwszFileName = Tf3rd2_Utils_StrToWstr(filename);
   if (NULL == pwszFileName)
   {
      return E_NOMEMORY;
   }

   /*if on target, using MTK'S FS_Delete*/
   if(0 != SFS_DeleteFile(pwszFileName, NULL))
   {
      Tf3rd2_7MemFree(pwszFileName);
      TF3RD_DBGINFO_END("Tf3rd0_13remove failed");
      return E_FAILED;
   }
   else
   {
      Tf3rd2_7MemFree(pwszFileName);
      TF3RD_DBGINFO_END("Tf3rd0_13remove successfully");
      return E_SUCCESS;
   }
}

/* ==================================================================================
Function:		Tf3rd0_14rename()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:48

Description:	rename file

Parameters:		input:	const char* pOldName
const char* pNewName

output: NULL

Return value:	if fail,return negative value
================================================================================ */
int Tf3rd0_14rename(const char* pOldName, const char* pNewName)
{
   unsigned short *pwszOldName = NULL;
   unsigned short *pwszNewName = NULL;
   int nRet = E_FAILED;

   /*check parameters*/
   if ((NULL == pOldName) || (NULL == pNewName)) 
   {
      return E_BADPARAM;
   }
   
   TF3RD_DBGINFO_START("Tf3rd0_14rename");
   do 
   {
      /*transfer file name from string to wstring*/
      pwszOldName = (unsigned short*)Tf3rd2_Utils_StrToWstr(pOldName);
      if (NULL == pwszOldName)
      {
         nRet = E_NOMEMORY;
         break;
      }
      pwszNewName = (unsigned short*)Tf3rd2_Utils_StrToWstr(pNewName);
      if (NULL == pwszNewName)
      {
         nRet = E_NOMEMORY;
         break;
      }
      
      /*if on target, invoke FS_Rename()*/
      if(0 != SFS_RenameFile(pwszOldName, pwszNewName, NULL))
      {
         nRet = E_FAILED;
         break;
      }
      else
      {
         nRet = E_SUCCESS;
         break;
      }
   } while(0);
   
   Tf3rd2_7MemFree(pwszOldName);
   Tf3rd2_7MemFree(pwszNewName);
   
   TF3RD_DBGINFO_END("Tf3rd0_14rename");
   
   return nRet;
}

/* ==================================================================================
Function:		Tf3rd0_15removedir()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:51

Description:	remove dir.

Parameters:		input:	const char* path

output:

Return value:	if fails, return negative value
================================================================================ */
int Tf3rd0_15removedir(const char* path)
{
   unsigned short *pwszPath = NULL;
   int nRet = E_FAILED;

   /*check parameters*/
   if (NULL == path) 
   {
      return E_BADPARAM;
   }
   
   TF3RD_DBGINFO_START("Tf3rd0_15removedir");
   do 
   {
      /*transfer file name from string to wstring*/
      pwszPath = (unsigned short*)Tf3rd2_Utils_StrToWstr(path);
      if (NULL == pwszPath)
      {
         nRet = E_NOMEMORY;
         break;
      }
      
      /*if on target, invoke mtk's FS_RemoveDir()*/
      if(0 != SFS_DeleteDirectory(pwszPath))
      {
         nRet =  E_FAILED;
         break;
      }
      else
      {
         nRet =  E_SUCCESS;
         break;
      }
   } while(0);
   
   Tf3rd2_7MemFree(pwszPath);
   
   TF3RD_DBGINFO_END("Tf3rd0_15removedir");
   
   return nRet;
}
                            
/* ==================================================================================
Function:		Tf3rd0_16stat()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   14:56

Description:	get file size

Parameters:		input:	void* f

output:	unsigned int* dwSize

Return value:	if fail, return negative value.
================================================================================ */
int Tf3rd0_16stat(void* f, unsigned int* dwSize)
{
   SFS_HANDLE hFile = 0;
   int nRet = E_FAILED;

   /*check parameters*/
   if ((NULL == f) || (NULL == dwSize)) 
   {
      return E_BADPARAM;
   }
   
   TF3RD_DBGINFO_END("Tf3rd0_16stat");
   /*get file handle*/
   hFile = (SFS_HANDLE)f;
   
   /*if on target,invoke mtk's FS_GetFileSize()*/
   nRet = SFS_GetFileSize(hFile, (uint32*)dwSize);
   
   TF3RD_DBGINFO_END("Tf3rd0_16stat");
   
   return nRet;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   15:33

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
int Tf3rd0_16statEx(const char* filename,  unsigned int* dwSize)
{
   SFS_HANDLE hFile;
   int nRet = E_FAILED;
   
   if(E_SUCCESS != Tf3rd0_26fAccess((char*)filename))
   {
      return E_FAILED;
   }
   
   TF3RD_DBGINFO_START("Tf3rd0_16statEx");
   
   hFile = (SFS_HANDLE)Tf3rd0_2fopen(filename, (const unsigned char*)"r");
   if (hFile <= 0)
   {
      return E_FAILED;
   }
   
   nRet = Tf3rd0_16stat((void *)hFile, dwSize);
   
   Tf3rd0_1fclose((void *)hFile);
   
   TF3RD_DBGINFO_END("Tf3rd0_16statEx");
   
   return nRet;
}

/* ==================================================================================
Function:		Tf3rd0_16StatX()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   15:39

Description:	get file size, file time, and file mode.

Parameters:		input:	const char* filename,

output:	unsigned int* dwSize,
unsigned int* fileTime,
unsigned char* fileMode

Return value:	if fails, return negative value.
================================================================================ */
int Tf3rd0_16StatX(const char* filename,  unsigned int* dwSize,unsigned int* fileTime, unsigned char* fileMode)
{
   int iRet = 0;
   SFS_HANDLE hFile = 0;
   unsigned short *pwszName = NULL;
   SFS_FIND_DATA_T fileInfo = {0};
   
   /*check parameters*/
   if ((NULL == filename) || (NULL == dwSize) || (NULL == fileTime)
      || (NULL == fileMode)) 
   {
      return E_BADPARAM;
   }
   
   TF3RD_DBGINFO_START("Tf3rd0_16StatX");
   
   TF3RD_DBGINFO("TF3RD_DBGINFO----------Tf3rd0_21CheckFileExist start, file or directory name = %s", filename);
   
   pwszName = (unsigned short *)Tf3rd2_Utils_StrToWstr(filename);
   if(!pwszName)
   {
      return E_NOMEMORY;
   }
      
   hFile = SFS_FindFirstFile(pwszName, &fileInfo);
   if(hFile == 0)
   {
      Tf3rd2_7MemFree(pwszName);
      return E_FAILED;
   }
   else
   {
      Tf3rd2_7MemFree(pwszName);
      *dwSize   = fileInfo.length;
      SFS_FindClose(hFile);

      return E_SUCCESS;
   }

   TF3RD_DBGINFO_END("Tf3rd0_16StatX");
   
   return 0;
}

/* ==================================================================================
Function:		Tf3rd0_17truncate()
Func Type:		global

  Author:			xin.hou@mailhz.techfaith.cn
  
    Created:		4:12:2007   16:21
    
      Description:	truncate a file
      
        Parameters:		input:	const char* filename, int length
        
          output:
          
            Return value:	
================================================================================ */
int Tf3rd0_17truncate(const char* filename, int length)
{
   SFS_HANDLE hFile = 0;
   int nRet = E_FAILED;

   /*check parameters*/
   if (NULL == filename) 
   {
      return E_BADPARAM;
   }
   
   TF3RD_DBGINFO_START("Tf3rd0_17truncate");
   
   /*get file handle*/
   hFile = (SFS_HANDLE)Tf3rd0_2fopen(filename, TF3RD_OFM_READWRITE);
   if (hFile <= 0) 
   {
      return E_FAILED;
   }
   
   nRet = Tf3rd0_17truncateEx(hFile, length);
   
   Tf3rd0_1fclose((void *)hFile);
   
   TF3RD_DBGINFO_END("Tf3rd0_17truncate");
   
   return nRet;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   16:51

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
int Tf3rd0_17truncateEx(void* f,  int length)
{
	SFS_HANDLE hFile = 0;
	uint32 nSize = 0;
	int nErr = SFS_NO_ERROR;
	
	/*check parameters*/
	if (NULL == f) 
	{
		return E_BADPARAM;
	}
	
	/*get file handle*/
	hFile = (SFS_HANDLE)f;
	
	SFS_GetFileSize(f, &nSize);
	
	if((uint32)length == 0)
	{
		nErr += SFS_SetFileSize(f, 0 );
		nErr += SFS_SetFilePointer(f, 0, SFS_SEEK_BEGIN);
	}
	else
	{
		nErr += SFS_SetFileSize(f, length);
		nErr += SFS_SetFilePointer(f, length, SFS_SEEK_BEGIN);
	}
   
   return nErr;
}

/* ==================================================================================
Function:		Tf3rd0_18opendir()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   16:54

Description:	opend directory

Parameters:		input:		const char* pDirName

output:

Return value:	file handle
================================================================================ */
void* Tf3rd0_18opendir(const char* pDirName, unsigned char bDir)
{
   int iRet = E_SUCCESS;
   St_FileMgr *pFileMgr = NULL;
   
   /*check parameters*/
   if (NULL == pDirName) 
   {
      return NULL;
   }

   TF3RD_DBGINFO_START("Tf3rd0_18opendir");
   
   iRet = Tf3rd0_21CheckFileExist((char *)pDirName);
   if (iRet != E_SUCCESS)
   {
      return NULL;
   }
   
   pFileMgr = (St_FileMgr*)TF3RD_MALLOCMEMORY(sizeof(St_FileMgr));
   if (NULL == pFileMgr)
   {
      return NULL;
   }
   
   pFileMgr->pszDir = Tf3rd2_13StrDup(pDirName);
   pFileMgr->hFile  = 0;
   pFileMgr->bDir   = bDir;
   
   TF3RD_DBGINFO_END("Tf3rd0_18opendir");
   
   return pFileMgr;
}

/* ==================================================================================
Function:		Tf3rd0_19readdir()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   17:12

Description:	read directory

Parameters:		input:

output:

Return value:
================================================================================ */
char* Tf3rd0_19readdir(void* pDirPtr)
{
   SFS_HANDLE hFile = 0;
   SFS_FIND_DATA_T *pstFindData = NULL;
   unsigned short *pwszDirName = NULL;
   St_FileMgr *pFileMgr = NULL;
   int nRet = E_SUCCESS;
   int nFindNextFlag = E_FALSE;
   int nLen = 0;

   uint16  ctlStrFile[] = L"/A:+NF-NF";//find file not include folder
   uint16  ctlStrFolder[] = L"/A:+NF-F";//find folder
	uint16  filter[] = L"*";//all files

   char    pszFileName[128] = {0};
   
   /*check parameters*/
   if (NULL == pDirPtr ) 
   {
      return NULL;
   }
   
   pFileMgr = (St_FileMgr *)pDirPtr;
   
   if (pFileMgr->pszDir == NULL)
   {
      return NULL;
   }
   
   pstFindData = TF3RD_MALLOCMEMORY(sizeof(SFS_FIND_DATA_T));
   if(!pstFindData)
   {
      return NULL;
   }
   TF3RD_MEMSET(pstFindData, 0x00, sizeof(SFS_FIND_DATA_T));

   if (pFileMgr->hFile == 0)
   {
      TF3RD_DBGINFO("TF3RD_DBGINFO----------Tf3rd0_19readdir start, directory name = %s", pFileMgr->pszDir);
      pwszDirName = (unsigned short*)Tf3rd2_Utils_StrToWstr(pFileMgr->pszDir);
      if (NULL == pwszDirName)
      {
         Tf3rd2_7MemFree(pstFindData);
         return NULL;
      }
            
      if(!pFileMgr->bDir)
      {
         nRet = SFS_FindFirstFileCtrl((unsigned short*)pwszDirName,
            filter,
            ctlStrFile,
            &hFile,
            pstFindData,
            NULL);
      }
      else
      {
         nRet = SFS_FindFirstFileCtrl((unsigned short*)pwszDirName,
            filter,
            ctlStrFolder,
            &hFile,
            pstFindData,
            NULL);
      }
      
      if (nRet != EFS_ERROR_NONE) 
      {
         Tf3rd2_7MemFree(pwszDirName);
         Tf3rd2_7MemFree(pstFindData);
         return NULL;
      }
      
      Tf3rd2_7MemFree(pwszDirName);
      
      pFileMgr->hFile = hFile;    
   }
   else
   {
      nFindNextFlag = E_TRUE;
   }
   
   if (nFindNextFlag == E_TRUE)
   {
      nRet = SFS_FindNextFile(pFileMgr->hFile, pstFindData);
    }
   
   if (nRet != SFS_ERROR_NONE)
   {
      Tf3rd2_7MemFree(pstFindData);
      return NULL;
   }
   
   nLen = Tf3rd2_39WstrLen(pstFindData->name);
   
   Tf3rd2_42Wstr2Str(pstFindData->name, pszFileName, nLen);
   Tf3rd2_4MemSet(g_pszFileName, 0x00, 128);
   Tf3rd2_61SPrintf(g_pszFileName, "%s\\%s", pFileMgr->pszDir, pszFileName);
   Tf3rd2_7MemFree(pstFindData);
      
   return g_pszFileName;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   17:16

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
int Tf3rd0_20closedir(void* pDirPtr)
{
   St_FileMgr *pstFileMgr = 0;
   SFS_HANDLE hFile;
   
   /*check parameters*/
   if (NULL == pDirPtr) 
   {
      return E_BADPARAM;
   }
   
   pstFileMgr = (St_FileMgr *)pDirPtr;
   
   Tf3rd2_7MemFree(pstFileMgr->pszDir);
   
   hFile = pstFileMgr->hFile;
   
   Tf3rd2_7MemFree(pstFileMgr);
   
   return SFS_FindClose(hFile);
}

int Tf3rd0_37fitfreespace(unsigned int *pdwHighSize, unsigned int *pdwLowSize)
{
   if(E_SUCCESS == Tf3rd0_37sdcard1Freespace(pdwHighSize, pdwLowSize))
   {
      return E_SUCCESS;
   }
   else if(E_SUCCESS == Tf3rd0_37sdcard2Freespace(pdwHighSize, pdwLowSize))
   {
      return E_SUCCESS;
   }
   else
   {
      return Tf3rd0_37udiskFreespace(pdwHighSize, pdwLowSize);
   }
}

int Tf3rd0_36sdcard1Vailed(void)
{
   if(SFS_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD)) == SFS_ERROR_NONE)
      return E_SUCCESS;
   else
      return E_FAILED;
}

int Tf3rd0_36sdcard2Vailed(void)
{
   if(SFS_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD)) == SFS_ERROR_NONE)
      return E_SUCCESS;
   else
      return E_FAILED;
}

int Tf3rd0_36udiskVailed(void)
{
   if(SFS_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK)) == SFS_ERROR_NONE)
      return E_SUCCESS;
   else
      return E_FAILED;
}

int Tf3rd0_37sdcard1Freespace(unsigned int *pdwHighSize, unsigned int *pdwLowSize)
{
   int nErr = E_FAILED;

   if(NULL == pdwLowSize || NULL == pdwHighSize)
   {
      return E_BADPARAM;
   }

   if(E_SUCCESS == Tf3rd0_36sdcard1Vailed())
   {
      nErr = SFS_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), pdwHighSize, pdwLowSize);
   }

   return (SFS_ERROR_NONE == nErr ? E_SUCCESS : E_FAILED);
}

int Tf3rd0_37sdcard2Freespace(unsigned int *pdwHighSize, unsigned int *pdwLowSize)
{
   int nErr = E_FAILED;

   if(NULL == pdwLowSize || NULL == pdwHighSize)
   {
      return E_BADPARAM;
   }

   if(E_SUCCESS == Tf3rd0_36sdcard2Vailed())
   {
      nErr = SFS_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1), pdwHighSize, pdwLowSize);
   }
   
   return (SFS_ERROR_NONE == nErr ? E_SUCCESS : E_FAILED);
}

int Tf3rd0_37udiskFreespace(unsigned int *pdwHighSize, unsigned int *pdwLowSize)
{
   int nErr = SFS_ERROR_NONE;

   if(NULL == pdwLowSize || NULL == pdwHighSize)
   {
      return E_BADPARAM;
   }

   if(E_SUCCESS == Tf3rd0_36udiskVailed())
   {
      nErr = SFS_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), pdwHighSize, pdwLowSize);
   }

   return (SFS_ERROR_NONE == nErr ? E_SUCCESS : E_FAILED);
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   17:21

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
int Tf3rd0_26fAccess(char* filename)
{
   if (filename == NULL)
   {
      return E_BADPARAM;
   }
   
   /*test if the file is exist or not*/
   return Tf3rd0_21CheckFileExist(filename);
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   17:22

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
unsigned int Tf3rd0_33FreeSize(int vVolumeID)
{
   /*it will be implemented later if needed.*/
   //TODO:
   return 0; 
}
            
/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   17:23

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
int Tf3rd0_34fsize(void* f)
{
   unsigned int uiSize = 0;
   
   if(NULL == f)
   {
      return 0;
   }

   SFS_GetFileSize((SFS_HANDLE)f, &uiSize);

   /*it will be implemented later if needed.*/
   return uiSize;
}

/* ==================================================================================
Function:		Tf3rd0_30Rand()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   17:34

Description:	create rend value

Parameters:		input:	NULL

output:	NULL

Return value:	rend value
================================================================================ */
unsigned int Tf3rd0_30Rand(void)
{
   return rand();
}

/* ==================================================================================
Function:		Tf3rd0_31ClockZone()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   17:43

Description:	return clock zone

Parameters:		input:

output:

Return value:
================================================================================ */
unsigned int Tf3rd0_31ClockZone(void)
{
   /*it will be implemented later*/
   return 0;
}
            
/* ==================================================================================
Function:		Tf3rd0_32Time()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   9:39

Description:	get utc time

Parameters:		input:

output:

Return value:
================================================================================ */
unsigned int Tf3rd0_32Time(void)
{
   return TM_GetTotalSeconds();
}

/* ==================================================================================
Function:		Tf3rd0_34MKTime()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   9:49

Description:	create time

Parameters:		input:	void* pTM ------- point of struct tm.

output:

Return value:	UTC TIME.
================================================================================ */
unsigned int Tf3rd0_34MKTime(TimeType* pTM)
{
   if(!pTM)
      return 0;
   
   return MMIAPICOM_Tm2Second((uint32)pTM->nSec,
                                                      (uint32)pTM->nMin,
                                                      (uint32)pTM->nHour,
                                                      (uint32)pTM->nDay,
                                                      (uint32)pTM->nMonth,
                                                      (uint32)pTM->nYear);
}
                                        
/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   10:21

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
void Tf3rd0_50Lock(TF3RD_HZ_SEMAPHORE_TYPE vSemaPhoreID)
{
   /*it will be implemented later if needed.*/
   return;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   10:28

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
void Tf3rd0_51UnLock(TF3RD_HZ_SEMAPHORE_TYPE vSemaPhoreID)
{
   /*it will be implemented later if needed.*/
   return;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   10:29

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
unsigned int Tf3rd0_21SetTimer(unsigned char appId, unsigned int ms)
{
   return E_FAILED;	
}
                                        
/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   10:29

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
unsigned int Tf3rd0_22KillTimer(unsigned int timer_id)
{
   return E_FAILED;
}

/* ==================================================================================
Function:		Tf3rd2_78GetTickCount()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   10:30

Description:	get time million seconds

Parameters:		input:

output:

Return value:
================================================================================ */
unsigned int Tf3rd2_78GetTickCount(void)
{
   
   /*it will be implemented later if needed*/
   return SCI_GetTickCount();
   
}

/* ==================================================================================
    Function:		Tf3rd2_GetDateTime()
	Func Type:		global

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:30
	
	Description:	get time million seconds

	Parameters:		input:
			
					output:
					
					  Return value:
================================================================================ */

void Tf3rd2_GetDateTime( TimeType *pstJulDate)
{
     SCI_DATE_T stSysDate = {0};
     SCI_TIME_T  stSysTime = {0};
     
	if (NULL == pstJulDate)
	{
		return ;
	}

     TM_GetSysDate(&stSysDate);
     TM_GetSysTime(&stSysTime);

     pstJulDate->nYear = (unsigned short)stSysDate.year;
     pstJulDate->nMonth = (unsigned char)stSysDate.mon;
     pstJulDate->nDay = (unsigned char)stSysDate.mday;
     pstJulDate->DayIndex = (unsigned char)stSysDate.wday;
     pstJulDate->nHour = (unsigned char)stSysTime.hour;
     pstJulDate->nMin = (unsigned char)stSysTime.min;
     pstJulDate->nSec = (unsigned char)stSysTime.sec;
	
}

/* ==================================================================================
    Function:		Tf3rd2_GetJulDate()
	Func Type:		global

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:30
	
	Description:	get time million seconds

	Parameters:		input:

			
					output:

	Return value:
  ================================================================================ */

void Tf3rd2_GetJulDate(unsigned long uiSenconds, TimeType *pstJulDate)
{
     MMI_TM_T stTMTime = {0};
     
	if (NULL == pstJulDate)
	{
		return ;
	}

     stTMTime = MMIAPICOM_Second2Tm((uint32)uiSenconds);
     pstJulDate->nYear = (unsigned short)stTMTime.tm_year;
     pstJulDate->nMonth = (unsigned char)stTMTime.tm_mon;
     pstJulDate->nDay = (unsigned char)stTMTime.tm_mday;
     pstJulDate->DayIndex = (unsigned char)stTMTime.tm_wday;
     pstJulDate->nHour = (unsigned char)stTMTime.tm_hour;
     pstJulDate->nMin = (unsigned char)stTMTime.tm_min;
     pstJulDate->nSec = (unsigned char)stTMTime.tm_sec;
}
/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   10:36

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
void Tf3rd2_79ConvertTC2Date(unsigned int vTC,unsigned short* Year, 
                             unsigned char* Month, unsigned char* Day, 
                             unsigned char* Hour, unsigned char* Minute, 
                             unsigned char* Second)
{
   /*it will be implemented later if needed.*/
   
}

/*==================================================================================
OS Interface
==================================================================================*/


/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   18:03

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
#define TF3RD_USED_MEMORY_CATCH                   1
extern PUBLIC void *email_alloc(uint32 size, const char *file, uint32 line) ;
extern PUBLIC void email_free(void *p);
#if TF3RD_USED_MEMORY_CATCH
static void* Tf3rd2_0GetMemFromFreeList(unsigned int allocSize, char *file, unsigned int line)
{   
   St_EmlMemInfo *pMem = gst_email_mem_ctl.email_mem_free_list;
   St_EmlMemInfo *pTmp = NULL;
   uint32        uiLeftSize = 0xffffffff;

   while(NULL != pMem)
   {
      if(pMem->uiBlockSize >= allocSize)
      {
         if(uiLeftSize > pMem->uiBlockSize - allocSize)
         {
            uiLeftSize = pMem->uiBlockSize - allocSize;
            pTmp = pMem;
            if(0 == uiLeftSize)
            {
               break;
            }
         }
      }

      pMem = pMem->next;
   }

   if(NULL == pTmp)
   {
      return email_alloc(allocSize,file,line);
   }
   else
   {
      EME_MEM_GETMUTEX;
      if(NULL == pTmp->prev && NULL == pTmp->next)
      {
         gst_email_mem_ctl.email_mem_free_list = NULL;
      }
      else if(NULL == pTmp->prev)
      {
         gst_email_mem_ctl.email_mem_free_list = pTmp->next;
         gst_email_mem_ctl.email_mem_free_list->prev = NULL;
      }
      else if(NULL == pTmp->next)
      {
         pTmp->prev->next = NULL;
      }
      else
      {
         pTmp->prev->next = pTmp->next;
         pTmp->next->prev = pTmp->prev;
      }

      gst_email_mem_ctl.uiMemFreeSize -= pTmp->uiBlockSize;
      gst_email_mem_ctl.uiMemFreeNum  -= 1;
      Tf3rd2_4MemSet(pTmp, 0x00, pTmp->uiBlockSize);
      EME_MEM_PUTMUTEX;
         
      return pTmp;
   }
}
#endif

static BOOLEAN gs_usedmempool = TRUE;
void* Tf3rd2_0MallocMemory(unsigned int allocSize, char *file, unsigned int line)
{
   St_EmlMemInfo *pMem = NULL;
   char          *pcFileName = NULL;
   uint32        uiMemEnd = EMAIL_MEM_END;
   uint32        uiFromPool = TRUE;

   do 
   {   
       if(gs_usedmempool && g_email_mem_handle == NULL)
       {
          g_email_mem_handle = (MPOOL_HANDLE)WEMemPool_CreateMemPool(EMAIL_MAX_MEM_SIZE);
          if(NULL == g_email_mem_handle)
          {
             gs_usedmempool = FALSE;
          }
       }
   } while (0);
   
   if(gs_usedmempool)
   {
       pMem = WEMemPool_Alloc(g_email_mem_handle, sizeof(St_EmlMemInfo) + allocSize + sizeof(uint32));
       if(pMem == NULL)
       {
           TF3RD_DBGPRINTF("Tf3rd2_0MallocMemory--: allocal memory from memory pool failed then used heap!!!");
           uiFromPool = FALSE;

#if TF3RD_USED_MEMORY_CATCH
           pMem = Tf3rd2_0GetMemFromFreeList(sizeof(St_EmlMemInfo) + allocSize + sizeof(uint32),file,line);
#else
           pMem = email_alloc(sizeof(St_EmlMemInfo) + allocSize + sizeof(uint32),file,line);
#endif
           if(NULL == pMem)
           {
               TF3RD_DBGPRINTF("Tf3rd2_0MallocMemory--: allocal memory from memory heap failed");
               return NULL;
           }
       }
   }
   else
   {
       uiFromPool = FALSE;

#if TF3RD_USED_MEMORY_CATCH
       pMem = Tf3rd2_0GetMemFromFreeList(sizeof(St_EmlMemInfo) + allocSize + sizeof(uint32),file,line);
#else
       pMem = email_alloc(sizeof(St_EmlMemInfo) + allocSize + sizeof(uint32),file,line);
#endif
       if(NULL == pMem)
       {
           TF3RD_DBGPRINTF("Tf3rd2_0MallocMemory--: allocal memory from memory heap failed");
           return NULL;
       }
   }
   Tf3rd2_4MemSet(pMem, 0x00, allocSize + sizeof(St_EmlMemInfo) + sizeof(uint32));

   pcFileName = TF3RD_STRRCHR(file, '\\');
   if(pcFileName == NULL)
   {
      pcFileName = TF3RD_STRRCHR(file, '/');
   }

   if(pcFileName != NULL)
   {
      Tf3rd2_2MemCpy(pMem->filename, pcFileName + 1, TF3RD_STRLEN(pcFileName + 1) > 31 ? 31 : TF3RD_STRLEN(pcFileName));
   }

   Tf3rd2_2MemCpy((char*)pMem + sizeof(St_EmlMemInfo) + allocSize, &uiMemEnd, sizeof(uint32));
   pMem->line   = line;
   pMem->uiSize = allocSize;
   pMem->uiBlockSize = sizeof(St_EmlMemInfo) + allocSize + sizeof(uint32);
   pMem->frommempool = uiFromPool;
   pMem->pvMem  = (char*)pMem + sizeof(St_EmlMemInfo);
   pMem->uiMemStart = EMAIL_MEM_START;

   EME_MEM_GETMUTEX;
   if(gst_email_mem_ctl.email_mem_list == NULL)
   {
      gst_email_mem_ctl.email_mem_list = pMem;
   }
   else
   {
      pMem->next = gst_email_mem_ctl.email_mem_list;
      pMem->prev = NULL;
      gst_email_mem_ctl.email_mem_list->prev = pMem;
      gst_email_mem_ctl.email_mem_list = pMem;
   }

   gst_email_mem_ctl.uiMemUsedSize += pMem->uiBlockSize;
   gst_email_mem_ctl.uiMemUsedNum  += 1;

   if(gst_email_mem_ctl.uiPeakValueSize < gst_email_mem_ctl.uiMemUsedSize)
   {
      gst_email_mem_ctl.uiPeakValueSize = gst_email_mem_ctl.uiMemUsedSize;
   }

   if(gst_email_mem_ctl.uiPeakValueNum < gst_email_mem_ctl.uiMemUsedNum)
   {
      gst_email_mem_ctl.uiPeakValueNum = gst_email_mem_ctl.uiMemUsedNum;
   }
   EME_MEM_PUTMUTEX;;
   return pMem->pvMem;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		4:12:2007   18:10

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
void Tf3rd2_7MemFree(void *memBlock)
{
   St_EmlMemInfo *pMem = (char*)memBlock - sizeof(St_EmlMemInfo); 
   uint32 uiSize = 0;
   uint32 uiPrf1 = 0;
   uint32 uiPrf2 = 0;
   uint32 uiMemEnd = 0;
   uint32 uiMemStart = 0;

   if(!memBlock)
      return;

   Tf3rd2_2MemCpy(&uiMemStart, pMem, sizeof(uint32));
   if(uiMemStart != EMAIL_MEM_START)
   {
      Tf3rd2_59DbgPrintf("Tf3rd2_7MemFree--: bad memory to free");
      return;
   }

   Tf3rd2_2MemCpy(&uiMemEnd, (char*)pMem + sizeof(St_EmlMemInfo) + pMem->uiSize, sizeof(uint32));

   if(uiMemEnd != EMAIL_MEM_END)
   {
      Tf3rd2_59DbgPrintf("Tf3rd2_7MemFree--: Memory OverFlow !!!!!!!!!!!!!!");
      Tf3rd2_59DbgPrintf("Tf3rd2_7MemFree--: Memory OverFlow !!!!!!!!!!!!!!");
      Tf3rd2_59DbgPrintf("Tf3rd2_7MemFree--: file = %s", pMem->filename);
      Tf3rd2_59DbgPrintf("Tf3rd2_7MemFree--: line = %d", pMem->line);
      Tf3rd2_59DbgPrintf("Tf3rd2_7MemFree--: size = %d", pMem->uiSize);
      Tf3rd2_59DbgPrintf("Tf3rd2_7MemFree--: Memory OverFlow !!!!!!!!!!!!!!");
      Tf3rd2_59DbgPrintf("Tf3rd2_7MemFree--: Memory OverFlow !!!!!!!!!!!!!!");
   }

   EME_MEM_GETMUTEX;
   if(pMem->next == NULL && pMem->prev == NULL)
   {
      gst_email_mem_ctl.email_mem_list = NULL;
   }
   else if(pMem->next == NULL)
   {
      pMem->prev->next = NULL;
   }
   else if(pMem->prev == NULL)
   {
      pMem->next->prev = NULL;
      gst_email_mem_ctl.email_mem_list = pMem->next;
   }
   else
   {
      pMem->next->prev = pMem->prev;
      pMem->prev->next = pMem->next;
   }

   gst_email_mem_ctl.uiMemUsedNum  -= 1;
   gst_email_mem_ctl.uiMemUsedSize -= pMem->uiBlockSize;

#if TF3RD_USED_MEMORY_CATCH
   if(!pMem->frommempool)
   {
      if(gst_email_mem_ctl.email_mem_free_list == NULL)
      {
         gst_email_mem_ctl.email_mem_free_list = pMem;
         pMem->next = NULL;
         pMem->prev = NULL;
      }
      else
      {
         pMem->next = gst_email_mem_ctl.email_mem_free_list;
         pMem->prev = NULL;
         gst_email_mem_ctl.email_mem_free_list->prev = pMem;
         gst_email_mem_ctl.email_mem_free_list = pMem;
      }

      gst_email_mem_ctl.uiMemFreeSize += pMem->uiBlockSize;
      gst_email_mem_ctl.uiMemFreeNum  += 1;
   }
   else
   {
      WEMemPool_Free(g_email_mem_handle, pMem);
   }
#else
   if(pMem->frommempool)
   {
      WEMemPool_Free(g_email_mem_handle, pMem);
   }
   else
   {
      email_free(pMem);
   }
#endif

   if(gst_email_mem_ctl.uiMemUsedNum == 0)
   {
      Tf3rd2_PrintMemoryCB();

#if TF3RD_USED_MEMORY_CATCH
      while(gst_email_mem_ctl.email_mem_free_list)
      {
         St_EmlMemInfo *pvMem = gst_email_mem_ctl.email_mem_free_list;
         gst_email_mem_ctl.email_mem_free_list = gst_email_mem_ctl.email_mem_free_list->next;
         gst_email_mem_ctl.uiMemFreeSize -= pvMem->uiBlockSize;
         gst_email_mem_ctl.uiMemFreeNum  -= 1;
         email_free(pvMem);
      }
#endif

      if(g_email_mem_handle != 0)
      {
         WEMemPool_DeleteMemPool(g_email_mem_handle);
         g_email_mem_handle = NULL;
      }

      Tf3rd2_4MemSet(&gst_email_mem_ctl, 0x00, sizeof(St_EmlMemCtl));
   }
   EME_MEM_PUTMUTEX;;
}

static AEECallback stPrintMemorycb = {0};
static void Tf3rd2_PrintMemoryCB(void)
{
   St_EmlMemInfo *pstMemStr = gst_email_mem_ctl.email_mem_list;
   
   if(gst_email_mem_ctl.email_mem_list != NULL)
   {
      while(pstMemStr)
      {
         Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: ---------------------------------");
         Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: file = %s", pstMemStr->filename);
         Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: line = %d", pstMemStr->line);
         Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: size = %d", pstMemStr->uiSize);
         pstMemStr = pstMemStr->next;
      }
   }
   
   Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: #######################################");
   Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: mem used size = %d", gst_email_mem_ctl.uiMemUsedSize);
   Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: mem used numb = %d", gst_email_mem_ctl.uiMemUsedNum);
   Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: mem free size = %d", gst_email_mem_ctl.uiMemFreeSize);
   Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: mem free numb = %d", gst_email_mem_ctl.uiMemFreeNum);
   Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: mem peak size = %d", gst_email_mem_ctl.uiPeakValueSize);
   Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: mem peak numb = %d", gst_email_mem_ctl.uiPeakValueNum);
   Tf3rd2_59DbgPrintf("Tf3rd2_PrintMemory--: #######################################");
}

void Tf3rd2_PrintMemory(void)
{
//    if(!CALLBACK_IsQueued(&stPrintMemorycb))
//    {
//       CALLBACK_Init(&stPrintMemorycb, Tf3rd2_PrintMemoryCB, NULL);
//       IShell_EmailResume(&stPrintMemorycb);
//    }
//   Tf3rd2_PrintMemoryCB();
}

void Tf3rd2_DeleteMemory(void)
{
   if(g_email_mem_handle != 0) 
   {
      WEMemPool_DeleteMemPool(g_email_mem_handle);
      g_email_mem_handle = NULL;
   }

   EME_MEM_GETMUTEX;
#if TF3RD_USED_MEMORY_CATCH
      while(gst_email_mem_ctl.email_mem_free_list)
      {
         St_EmlMemInfo *pvMem = gst_email_mem_ctl.email_mem_free_list;
         gst_email_mem_ctl.email_mem_free_list = gst_email_mem_ctl.email_mem_free_list->next;
         gst_email_mem_ctl.uiMemFreeSize -= pvMem->uiBlockSize;
         gst_email_mem_ctl.uiMemFreeNum  -= 1;
         email_free(pvMem);
      }
#endif

   Tf3rd2_4MemSet(&gst_email_mem_ctl, 0x00, sizeof(St_EmlMemCtl));
   EME_MEM_PUTMUTEX;
}
/* ==================================================================================
Function:	Tf3rd2_1MemCmp	
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_1MemCmp( const void *buf1, const void *buf2, unsigned int  count )
{
   return memcmp(buf1, buf2, count);
}

/* ==================================================================================
Function:	Tf3rd2_2MemCpy	
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
void * Tf3rd2_2MemCpy(void *dst, const void *src, unsigned int len)
{
   return memcpy(dst, src, len);
}

/* ==================================================================================
Function:	Tf3rd2_3MemMove	
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
void * Tf3rd2_3MemMove(void *dst, const void *src, unsigned int count)
{
   return memmove(dst, src, count);
}

/* ==================================================================================
Function:	Tf3rd2_4MemSet	
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:	

input:

output:

Return value:
================================================================================ */
void * Tf3rd2_4MemSet(void *s, int c, unsigned int n)
{
   return memset(s, c, n);
}

/* ==================================================================================
Function:		Tf3rd2_5MemStr
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char* Tf3rd2_5MemStr( void *p1, const void *p2, unsigned int count )
{
   char *s1 = NULL;
   char *s2 = NULL;
   int i = 0;
   char * pFind = NULL;
   
   /*Check parameters*/
   if (NULL == p1 || NULL == p2)
   {
      return NULL;
   }    
   
   if((int)count < Tf3rd2_18StrLen(p2))
   {
      return NULL;
   }
   
   s1 = (char *)p1;
   s2 = (char *)p2;
   
   while (i < (int)count)
   {
      if ((Tf3rd2_22StrniCmp(s1,s2,Tf3rd2_18StrLen (s2))) == 0)
      {
         pFind = (char*)s1;
         break;
      }
      
      i++;
      
      if ((int)(count-i) < Tf3rd2_18StrLen((s2)))
      {
         return NULL;
      }
      
      s1++;
   }
   
   return pFind;
   
}
                                        
/* ==================================================================================
Function:		Tf3rd2_6Realloc
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
void *Tf3rd2_6Realloc(void *ptr, unsigned int size)
{
   St_EmlMemInfo *pMem = (char*)ptr - sizeof(St_EmlMemInfo);
   char         *pcReturn = NULL;
   uint32 uiMemStart = 0;
   
   if(ptr == NULL)
   {
      return TF3RD_MALLOCMEMORY(size);
   }

   Tf3rd2_2MemCpy(&uiMemStart, pMem, sizeof(uint32));
   if(uiMemStart != EMAIL_MEM_START)
   {
      TF3RD_DBGPRINTF("Tf3rd2_6Realloc--: Realloc memory failed !!!!!!!!!");
      return NULL;
   }
   
   if(pMem->uiSize == size)
   {
      return ptr;
   }

   if(pMem->uiSize > size)
   {
      TF3RD_DBGPRINTF("Tf3rd2_6Realloc--: bad size for realloc memory !!!!!!!!!");
      return NULL;
   }

   pcReturn = TF3RD_MALLOCMEMORY(size);
   if(NULL == pcReturn)
   {
      TF3RD_DBGPRINTF("Tf3rd2_6Realloc--: no memory for realloc memory !!!!!!!!!");
      return NULL;
   }
   Tf3rd2_2MemCpy(pcReturn, ptr, pMem->uiSize);

   Tf3rd2_7MemFree(ptr);

   return pcReturn;
}

/* ==================================================================================
Function:		Tf3rd2_8StrAtoi
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_8StrAtoi(const unsigned char *str)
{
   /*Check parameters*/
   if (NULL == str)
   {
      return E_BADPARAM;
   }
   
   return atoi((const char *)str);
   
}

/* ==================================================================================
Function:		Tf3rd2_9StrCat
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char *Tf3rd2_9StrCat(char *dst, const char*src)
{
   return strcat(dst, src);
}

/* ==================================================================================
Function:		Tf3rd2_10StrChr
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char *Tf3rd2_10StrChr(const char *str, char ch)
{
   return strchr(str, ch);
}


/* ==================================================================================
Function:		Tf3rd2_11StrCmp
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_11StrCmp(const char *str1, const char *str2)
{
   return strcmp(str1, str2);
}


/* ==================================================================================
Function:		Tf3rd2_12StrCpy
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char *Tf3rd2_12StrCpy(char *dst, const char *src)
{
   return strcpy(dst, src);
}


/* ==================================================================================
Function:		Tf3rd2_13StrDup
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char *Tf3rd2_13StrDup(const char *stringToDuplicate)
{
   int nRet = E_SUCCESS;
   char *pszRet = NULL;
   
   if (!stringToDuplicate)
   {
      return NULL;
   }
   
   do 
   {
      pszRet = (char *)TF3RD_MALLOCMEMORY(Tf3rd2_18StrLen(stringToDuplicate) +1);
      if (NULL == pszRet)
      {
         nRet = E_NOMEMORY;
         break;
      } 
      
      Tf3rd2_12StrCpy(pszRet, stringToDuplicate);
      
   } while(0);
   
   if (E_SUCCESS != nRet)
   {
      Tf3rd2_7MemFree(pszRet);
      pszRet = NULL;
   }
   
   return pszRet;
   
}
                                        
                                        
/* ==================================================================================
Function:		Tf3rd2_14StriCmp
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description: Compares, case-insensitively, str1 and str2. 

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_14StriCmp( const char *str1, const char *str2)
{
   return MMIAPICOM_Stricmp(str1, str2);
#if 0
   char *string1 = NULL;
   char *string2 = NULL;
   int nRet = 0;
   
   string1 = Tf3rd2_13StrDup(str1);
   string2 = Tf3rd2_13StrDup(str2);
   
   Tf3rd2_19StrLower((char*)string1);
   Tf3rd2_19StrLower((char*)string2);
   
   nRet =  Tf3rd2_11StrCmp(string1, string2);
   
   Tf3rd2_7MemFree(string1);
   Tf3rd2_7MemFree(string2);
   
   return nRet;
#endif
}


/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description: Find a the first occurence of a string in another string, 
case-insensitively.

Parameters:		
input:

output:

Return value:
================================================================================ */
char* Tf3rd2_15StriStr( const char *pszHaystack, const char *pszNeedle)
{
   if (NULL == pszHaystack || NULL == pszNeedle)
   {
      return NULL;
   } 
   
   Tf3rd2_19StrLower((char*)pszHaystack);
   Tf3rd2_19StrLower((char*)pszNeedle);
   
   return Tf3rd2_24StrStr(pszHaystack, pszNeedle);  
}


/* ==================================================================================
Function:		Tf3rd2_16StrlCat
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char* Tf3rd2_16StrlCat(char * dst, const char * src, unsigned int nSize)
{
   char  * sdst = dst;
   char  * ssrc = (char* )src;
   unsigned int i = 0;
   unsigned int l = 0;
   
   /*Check parameters*/
   if (NULL == dst || NULL == src || (int)nSize <= 0)
   {
      return NULL;
   } 
   
   l = Tf3rd2_18StrLen(dst) + Tf3rd2_18StrLen(src);
   
   while (*sdst != '\0')
   {
      sdst++;
      i++;
   }
   
   while (*ssrc != '\0') 
   {
      if (i >= nSize-1)
      {
         *sdst = '\0';
         return dst;
      }
      else
      {
         *sdst = *ssrc;
      }
      
      sdst++;
      ssrc++;
      i++;
   }
   
   *sdst = '\0';
   
   return dst;
   
}

/* ==================================================================================
Function:		Tf3rd2_17StrlCpy
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char* Tf3rd2_17StrlCpy(char * dst, const char * src, unsigned int nSize)
{
   char  * sdst = NULL;
   char  * ssrc = NULL;
   unsigned int  i = 0;
   
   /*Check parameters*/
   if (NULL == dst || NULL == src)
   {
      return NULL;
   } 
   
   sdst = dst;
   ssrc = (char* )src;
   
   while (*ssrc != '\0') 
   {
      if (i == nSize-1)
      {
         *sdst = '\0';
         return dst;
      }
      else
      {
         *sdst = *ssrc;
      }
      
      sdst++;
      ssrc++;
      i++; 
   }
   
   *sdst = '\0';
   
   return dst;
}
                                        
/* ==================================================================================
Function:		Tf3rd2_18StrLen
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_18StrLen(const char * str)
{
   return strlen(str);
}

/* ==================================================================================
Function:		Tf3rd2_19StrLower
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char* Tf3rd2_19StrLower(char *psz)
{
   return MMIAPICOM_Strlower(psz);
#if 0
   char * pszTemp = NULL;
   
   /*Check parameters*/
   if (NULL == psz)
   {
      return NULL;
   } 
   
   pszTemp = psz;
   
   while (*pszTemp != '\0')
   {
      if (*pszTemp >= 'A' && *pszTemp <= 'Z')
      {
         *pszTemp += 32; 
      }    
      
      pszTemp++;
   } 
   
   return psz;
#endif
}

/* ==================================================================================
Function:		Tf3rd2_20StrnCmp
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_20StrnCmp( const char *str1, const char *str2 , unsigned int length)
{
   return strncmp(str1, str2, length);
}

/* ==================================================================================
Function:		Tf3rd2_21StrnCpy
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char* Tf3rd2_21StrnCpy( char *strDest, const char *strSource, unsigned int count )
{
   return strncpy(strDest, strSource, count);
}


/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:Compares, case-insensitively, at most length bytes in str1 and str2. 

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_22StrniCmp( const char *str1, const char *str2 , unsigned int length)
{
   char *string1 = NULL;
   char *string2 = NULL;
   int nRet = E_FAILED;
   
   string1 = Tf3rd2_13StrDup(str1);
   string2 = Tf3rd2_13StrDup(str2);
   
   Tf3rd2_19StrLower((char*)string1);
   Tf3rd2_19StrLower((char*)string2);
   
   nRet = Tf3rd2_20StrnCmp(string1, string2, length);
   
   Tf3rd2_7MemFree(string1);
   Tf3rd2_7MemFree(string2);
   
   return nRet;
}
                                        
                                        
/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char *Tf3rd2_23StrrChr( const char *string, char c)
{
   return strrchr(string, c);
}


/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char *Tf3rd2_24StrStr(const char *str1, const char *str2)
{
   return strstr(str1, str2);
}

/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
double Tf3rd2_25StrTod(const char *pszFloat, char ** ppszEnd)
{
   return 0;
}


/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
unsigned long Tf3rd2_26StrToul( const char *nptr, char **endptr, int base )
{
   return strtoul(nptr,endptr,base);
}
                                        
/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
unsigned short *Tf3rd2_27Str2Wstr(char *p_AscStr, unsigned short *p_Ucs2, int nSize)
{
   return MMIAPICOM_StrToWstr(p_AscStr, p_Ucs2);
#if 0
   unsigned short *pwszTemp = NULL;
   char *pszIn = NULL;
   int nLength = 0;
   
   /*Check parameters*/
   if (NULL == p_AscStr || NULL == p_Ucs2)
   {
      return NULL;
   } 
   
   pwszTemp = p_Ucs2;
   pszIn = p_AscStr;
   
   nLength = Tf3rd2_18StrLen(p_AscStr) ;
   nSize = (nSize > nLength ) ?  nLength : nSize ;
   while( nSize -- > 0)
   {
      if (Tf3rd2_Utils_isLittleEdain())
      {
         *pwszTemp = (unsigned short)*pszIn;
      }
      else
      {
         *pwszTemp = (unsigned short)*pszIn;
         *pwszTemp = *pwszTemp << 8 ;
      }
      
      pwszTemp++;
      pszIn++ ;
   }
   
   *pwszTemp = '\0';
   
   return p_Ucs2 ;
#endif
   
}

/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char *Tf3rd2_28StrUpper(char *psz)
{
   return MMIAPICOM_Strupper(psz);
#if 0   
   char * pszTemp = NULL;
   
   /*Check parameters*/
   if (NULL == psz)
   {
      return NULL;
   } 
   
   pszTemp = psz;
   
   while (*pszTemp != '\0')
   {
      if (*pszTemp >= 'a' && *pszTemp <= 'z')
      {
         *pszTemp -= 32; 
      }    
      
      pszTemp++;
   } 
   
   return psz;
#endif
}

/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
unsigned int Tf3rd2_29Utf8ToWstr(const unsigned char *pSrc,int nLen, unsigned short *pDst, int nSize)
{
   const unsigned char* pDest1 = NULL;
   /*Check parameters*/
   if (NULL == pSrc || NULL == pDst)
   {
      return (unsigned char)'\0';
   } 
   
   return (unsigned int)GUI_UTF8ToWstr(pDst, nSize/sizeof(wchar), pSrc, nLen);
}


/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
unsigned short* Tf3rd2_31WstrCat(unsigned short *pDest, unsigned short *pSrc)
{
   return MMIAPICOM_Wstrcat(pDest, pSrc);
}

/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
unsigned short* Tf3rd2_32WstrChr( unsigned short *s1, unsigned short c )
{
   /*Check parameters*/
   if (NULL == s1)
   {
      return NULL;
   } 

   return NULL;
}
                                        
/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_33WstrCmp(unsigned short *s1, unsigned short *s2)
{
   return MMIAPICOM_Wstrcmp(s1, s2);
}

/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
unsigned short* Tf3rd2_35WstrCpy(unsigned short *pDest, unsigned short *pSrc)
{
   return MMIAPICOM_Wstrcpy(pDest, pSrc);
}

/* ==================================================================================
Function:		Tf3rd2_36WstrDup
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
unsigned short *Tf3rd2_36WstrDup(const unsigned short *pIn)
{
   unsigned short *pTemp = NULL;
   
   /*Check parameters*/
   if (NULL == pIn)
   {
      return NULL;
   }
   
   /*allocates memory required using MALLOC() to store a copy of the incoming string */
   pTemp = (unsigned short *)TF3RD_MALLOCMEMORY(Tf3rd2_39WstrLen((unsigned short*)pIn));
   if (NULL == pTemp)
   {
      return NULL;
   } 
   
   /*copies this string into the newly allocated buffer*/
   if (NULL == Tf3rd2_35WstrCpy(pTemp, (unsigned short *)pIn))
   {
      return NULL;
   }
   
   *(pTemp + Tf3rd2_39WstrLen((unsigned short*)pIn)) = 0;
   
   return pTemp;
}


/* ==================================================================================
Function:		Tf3rd2_39WstrLen
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_39WstrLen(unsigned short * str)
{
   return MMIAPICOM_Wstrlen(str);
}

/* ==================================================================================
Function:		Tf3rd2_40WstrLower
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
void Tf3rd2_40WstrLower(unsigned short *pszDest)
{
   MMIAPICOM_Wstrlower(pszDest);
}

/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
int Tf3rd2_41WstrnCmp(unsigned short *s1, unsigned short *s2, unsigned int length)
{
   return MMIAPICOM_Wstrncmp(s1, s2, length);
}

/* ==================================================================================
Function:		
Func Type:

Author:		junrong.liu@mailhz.techfaith.cn

Created:		5:12:2007   10:10

Description:

Parameters:		
input:

output:

Return value:
================================================================================ */
char *Tf3rd2_42Wstr2Str(unsigned short *pIn, char *pszDest, int nSize)
{
   return MMIAPICOM_WstrToStr(pIn, pszDest);
   
#if 0
   char *pszTemp = NULL;
   unsigned short *pwszIn = NULL;
   int nLength = 0;
   
   if (NULL == pIn || NULL == pszDest)
   {
      return NULL;
   } 
   
   pwszIn = pIn;
   pszTemp = pszDest;
   
   nLength = Tf3rd2_39WstrLen(pIn) ;
   nSize = (nSize > nLength ) ?  nLength : nSize ;
   
   while( nSize -- > 0)
   {
      
      if (Tf3rd2_Utils_isLittleEdain())
      {
         *pszTemp = (char)*pwszIn;
      }
      else
      {
         *pszTemp = *pwszIn >> 8 ;
      }
      
      pszTemp++;
      pwszIn++ ;  
   }
   
   *pszTemp = '\0';
   
   return pszDest ;
#endif
   
}


unsigned int Tf3rd2_43Wstr2Utf8(const unsigned short *pSrc,int nLen, unsigned char *pDst, int nSize)
{
   if(!pSrc || !nLen || !pDst || !nSize)
      return (unsigned char)'\0';
   
   return (unsigned int)GUI_WstrToUTF8(pDst, nSize, pSrc, nLen);
}


void Tf3rd2_44WstrUpper(unsigned short *pszDest)
{
   MMIAPICOM_Wstrupper(pszDest);
}
                                        
/*===================================================================*
No used
*===================================================================*/

unsigned int Tf3rd2_37WstrlCat(unsigned short * dst, const unsigned short * src, unsigned int nSize)
{
   unsigned int nLen = 0;
   unsigned int nStep = 0;
   unsigned short *pwszTemp = NULL;
   
   if (NULL == dst || NULL == src)
   {
      return E_BADPARAM;
   } 
   
   nLen = Tf3rd2_39WstrLen(dst);
   
   nStep = (nLen < nSize) ? (nSize - nLen - 1) : 0;
   
   if ((int)nStep <= 0)
   {
      return -1;
   } 
   
   pwszTemp = (unsigned short *)src;
   
   while (nStep > 0 && *pwszTemp != '\0')
   {
      *(dst + nSize - nStep + 1) = *pwszTemp++;
      
      nStep--;
   } 
   
   *(dst + nSize - nStep + 1) = '\0';
   
   return (nSize - nStep + 1);
}


unsigned int Tf3rd2_38WstrlCpy(unsigned short * dst, const unsigned short * src, unsigned int nSize)
{
   unsigned int nDstLen = 0;
   unsigned int nSrcLen = 0;
   unsigned int nStep = 0;
   unsigned short *pwszDst = NULL;
   unsigned short *pwszSrc = NULL;
   
   if (NULL == dst || NULL == src || (int)nSize <= 0)
   {
      return E_BADPARAM;
   } 
   
   nDstLen = Tf3rd2_39WstrLen(dst);
   nSrcLen = Tf3rd2_39WstrLen((unsigned short *)src);
   
   nStep = (nSize > nDstLen) ? nDstLen : nSize;
   
   nStep = (nSrcLen < nStep) ? nSrcLen : nStep;
   
   pwszDst = dst;
   pwszSrc = (unsigned short *)src;
   
   while (nStep > 0 && *pwszSrc != '\0')
   {
      *pwszDst++ = *pwszSrc++;
      
      nStep--;
   }
   
   *(pwszDst) = '\0';
   
   return Tf3rd2_39WstrLen(dst);
   
}

void Tf3rd2_34WstrCompress(const unsigned short *pSrc, int nLen,unsigned char *pDest, int nSize);

double Tf3rd2_45FCeil(double x)
{
   return 0;
}


double Tf3rd2_47FFloor(double x)
{
   return 0;
}

unsigned char Tf3rd2_48Float2Wstr(double v, unsigned short *psz, int nSize)
{
   return (unsigned char)'\0';
}

unsigned long Tf3rd2_49StrCspn(char* a,char *b)
{
   return 0;
}

char* Tf3rd2_50StrPbrk(char* a,char *b)
{
   return NULL;
}

long Tf3rd2_52StrTol(char* a,char **b,int c)
{
   return strtol(a, b, c);
}

void Tf3rd2_56MSleep(unsigned int msecs)
{
   return;
}

void *Tf3rd2_63MemChr(const void *s, int c, unsigned int n)
{
   return memchr(s, c, n);
}

void* Tf3rd2_64MemRChr(const void *s, int c, unsigned int n)
{
   char *pszTemp = NULL;
   char *pszRet  = NULL;
   //int nsize = 0;
   
   /*Check parameters*/
   if (NULL == s)
   {
      return NULL;
   }    
   
   pszTemp = (char *)s;
   
   while (*pszTemp != '\0' && n > 0)
   {
      if (c == *pszTemp)
      {
         pszRet = pszTemp;
      } 
      
      pszTemp++;
      n--;
   } 
   
   return pszRet;   
   
}
/*===================================================================*/

int Tf3rd2_60SNPrintf(char *s, unsigned int n, const char *format, ...)
{
   va_list args;
   int nBuf = 0;
   va_start(args, format);
   
   nBuf = _vsnprintf((char *)s, n, format, args);
   
   va_end(args);
   
   return nBuf;
}


int Tf3rd2_60VSNPrintf(char *s, unsigned int n, const char *format, void* args)
{
   int nBuf = 0;
   
   nBuf = _vsnprintf((char *)s, n, format, *((va_list*)args));
   
   return nBuf;
}


int Tf3rd2_61VSPrintf(char *s, const char *format, void* args)
{
   int nBuf = 0;
   
   nBuf = vsprintf((char *)s, format, *((va_list*)args));
   
   return nBuf;
}
                                        
#if TF3RD_DBG_FILE_INFO
static void Tf3rd0_CheckDbgFilePath(void)
{
   g_nPathFlag = E_FALSE;
   
   return ;
}
#endif //TF3RD_DBG_FILE_INFO

char  g_DBGbuf[256] = {0};
void Tf3rd2_59DbgPrintf(const char *pszFormat,...)
{
   char *prefix = "[TechSoft] ";
   va_list args;
   
   if(pszFormat == NULL)
   {
      return;
   }

   va_start(args, pszFormat);
   
   Tf3rd2_4MemSet(g_DBGbuf, 0x00, 256);
   Tf3rd2_12StrCpy(g_DBGbuf, prefix);
   
   _vsnprintf(g_DBGbuf+Tf3rd2_18StrLen(prefix), 240, pszFormat, args);
   
//   SCI_TRACE_MID("%s\n", g_DBGbuf);
   MMIEMAIL_DbgPrintf("%s", g_DBGbuf);
   
   va_end(args);
   
   return;
}

int Tf3rd2_61SPrintf(char *s, const char *format, ...)
{
   va_list args;
   int nBuf = 0;
   
   if(format == NULL)
   {
      return 0;
   }
   va_start(args, format);
   
   nBuf = vsprintf((char *)s,  format, args);
   
   va_end(args);
   
   return nBuf;
}


void Tf3rd2_30Wsprintf(unsigned short *pDest, int nSize, unsigned short *pFormat,...)
{
   va_list args;
   int nBuf = 0;

   if(pFormat == NULL)
   {
      return;
   }

   va_start(args, pFormat);
   
   //nBuf = wsprintf(pDest, nSize, pFormat, args);
   
   va_end(args);
   
   return;
}

void Tf3rd2_62CurrentTime(unsigned short* Year, 
                          unsigned char* Month, 
                          unsigned char* Day, 
                          unsigned char* Hour, 
                          unsigned char* Minute, 
                          unsigned char* Second)
{
   SCI_DATE_T stDate = {0};
   SCI_TIME_T stTime = {0};

   TM_GetSysDate(&stDate);
   TM_GetSysTime(&stTime);

   *Year   = (unsigned short)stDate.year;
   *Month  = (unsigned char)stDate.mon;
   *Day    = (unsigned char)stDate.mday;
   *Hour   = (unsigned char)stTime.hour;
   *Minute = (unsigned char)stTime.min;
   *Second = (unsigned char)stTime.sec;

   return;
}


/* ==================================================================================
   
							MATH FUNCTION
						
   =============================================================================== */

   
/* ==================================================================================
   Function:		Tf3rd2_70Abs()
   Func Type:		global

   Author:			xin.hou@mailhz.techfaith.cn

   Created:		5:12:2007   10:38

   Description:	get absolute vlaue.

   Parameters:		input:

   output:

   Return value:
  ================================================================================ */
int Tf3rd2_70Abs(int param)
{
	return abs(param);
}

/* ==================================================================================
   Function:		
   Func Type:

   Author:			xin.hou@mailhz.techfaith.cn

   Created:		5:12:2007   10:41

   Description:

   Parameters:		input:

   output:

   Return value:
  ================================================================================ */
double Tf3rd2_71Fabs(double param)
{
	return fabs(param);
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:49
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
double Tf3rd2_72Sqrt(double param)
{
	return sqrt(param);
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:50
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
double Tf3rd2_73Sin(double param)
{
	return sin(param);
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:50
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
double Tf3rd2_74Cos(double param)
{
	return cos(param);
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:50
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
double Tf3rd2_75Tan(double param)
{
	return tan(param);
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:51
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
double Tf3rd2_76AtoF(const char * param)
{
	return atof(param);
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:51
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
double Tf3rd2_77Pow(double param, double exp)
{
	return pow(param,exp);
}


/* ================================================================================
 
							PLAY SOUND 
   
 =============================================================================== */


/* ==================================================================================
    Function:		Tf3rd3_20PlayMusic()
	Func Type:		global 

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   10:53
	
	Description:	play music

	Parameters:		input:	const char* pFileName
							unsigned char bRepeat
							unsigned int vDuration
			
					output:

	Return value:	NULL
  ================================================================================ */
void Tf3rd3_20PlayMusic(const char* pFileName, unsigned char bRepeat, 
						unsigned int vDuration )
{
   /*it will be implemented later if needed*/
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   11:52

Description:

Parameters:		input:

output:

Return value:
  ================================================================================ */
void Tf3rd3_21StopMusic(void)
{
   /*it will be implemented later if needed*/
}


/* ==================================================================================
 
 							UI INTERFACE
 
 ================================================================================== */



/* =================================================================================
 
							image decode interface

 ==================================================================================*/


/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   12:00

Description:

Parameters:		input:

output:

Return value:
  ================================================================================ */
void Tf3rd3_9CompressPic(unsigned char  *Src_data,unsigned short srcWidth, 
						 unsigned short srcHeight,unsigned short Src_x,
						 unsigned short  Src_y,unsigned short  Srcimg_width,
						 unsigned short  Srcimg_height,unsigned short  Displaywidth,
						 unsigned short Displayheight,unsigned short  Des_x,
						 unsigned short  Des_y,unsigned short desWidth, 
						 unsigned short desHeight,unsigned char  *Des_data)
{
	/*it will be implemented later if needed.*/
	return ;
}

/* ==================================================================================
Function:		Tf3rd3_12GetImageInfo()
Func Type:		global

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   12:01

Description:	get image info: width,height,colors,framecount,srcBufsize

Parameters:		input:	void *pImageBuffer,
int enumImageType

output:	void *pImageInfo

Return value:	if it fails, return negative data.
  ================================================================================ */
unsigned int Tf3rd3_12GetImageInfo(void *pImageBuffer,int enumImageType,
								   void *pImageInfo )
{
   /*it will be implemented later if needed*/
   return 0;
}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   13:21

Description:

Parameters:		input:

output:

Return value:
================================================================================ */
unsigned int Tf3rd3_13DecodeImageToBmp(void *pImageBuffer, 
									   unsigned long BufferSize,
									   int enumImageType,
									   int  FrameIndex, // 如果是动画，这里表示取用第几帧，不是动画，这个值无意义
									   unsigned int pGifHandle,
									   int *pDelayTime,
									   void *pBmpBuffer,
									   int vBmpWidth,
                                       int vBmpHeight)
{
	/*it will be implemented later*/
	return 0;

}

/* ==================================================================================
Function:		
Func Type:

Author:			xin.hou@mailhz.techfaith.cn

Created:		5:12:2007   13:30

Description:

Parameters:		input:
	
			output:

Return value:
  ================================================================================ */
int Tf3rd3_22GetCurFrameInfo(unsigned int pGifHandle , int *dx, int*dy, 
							 int* dWidth,int * dHeight)
{
	/*it will be implemented later*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   13:34
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
int Tf3rd3_23CheckGIFIsLastFrame(unsigned int pGifHandle)
{
   /*it will be implemented later if needed*/

   return 0;
}




/* ================================================================================

  displaying interface
  
================================================================================== */

/* ==================================================================================
Function:		
Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   14:57
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_0DrawPoint(unsigned char *Des_buf, unsigned short vp_Width, 
					   unsigned short vp_Height, unsigned short x , 
					   unsigned short y, unsigned short vp_Color, 
					   unsigned char vp_Mode)
{
	/*it will be implemented later if needed*/
	return ;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:00
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_01DrawLine(unsigned char * Des_buf,unsigned short Area_width,
					   unsigned short Area_height,unsigned short x1, 
					   unsigned short y1, unsigned short x2, unsigned short y2,
					   unsigned short vp_Color,unsigned char vp_Mode)
{
	/*it will be implemented later if needed*/
	return ;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:01
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_02DrawRect( unsigned char * Des_buf,unsigned short Area_width,
					   unsigned short Area_height,unsigned short x1, 
					   unsigned short y1, unsigned short x2, unsigned short y2,
					   const unsigned short v_ColorIdx,const unsigned char v_Mode)
{
	/*it will be implemented later if needed*/
	return ;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:02
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_04FillRect(unsigned char * Des_buf,unsigned short Area_width,
					   unsigned short Area_height,unsigned short vl_x, 
					   unsigned short vl_y,unsigned short  vl_Width, 
					   unsigned short vl_Height,unsigned short v_BackgroudColor)
{
	/*it will be implemented later if needed*/
	return ;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:03
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_05DrawArc(unsigned short cx, unsigned short cy, unsigned short w, 
					  unsigned short h, unsigned short s, unsigned short e, 
					  const unsigned short v_ColorIdx,const unsigned char v_Mode)
{
	/*it will be implemented later if needed*/
	return ;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:05
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_06ReverseZone(unsigned char * Des_buf,unsigned short Area_width,
						  unsigned short Area_height,unsigned short vl_x, 
						  unsigned short vl_y,unsigned short  vl_Width, 
						  unsigned short vl_Height)
{
	/*it will be implemented later if needed*/
	return ;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:05
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_07RefreshAreaScreen(unsigned short v_x1,unsigned short v_y1,
								unsigned short v_x2,unsigned short v_y2)
{
   /*it will be implemented later if needed*/
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:44
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_08DisplayImage(unsigned char * vp_BmpData, unsigned short ImgWidth,
						   unsigned short ImgHeight,unsigned short v_x, 
						   unsigned short v_y, unsigned char v_Mode)
{
	/*it will be implemented later if needed*/
	return ;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:45
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned short *Tf3rd4_09Get_ScreenBuffer(void)
{
     /*it will be implemented later if needed*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:49
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned short Tf3rd4_10Get_ScreenBuffer_Width(void)
{
     /*it will be implemented later if needed*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:51
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned short Tf3rd4_11Get_ScreenBuffer_Height(void)
{
     /*it will be implemented later if needed*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:52
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
void Tf3rd4_12BitBlt(unsigned char * pbmDest,unsigned short desWidth, 
					 unsigned short desHeight, unsigned short xDest,
					 unsigned short yDest,unsigned short cxDest,
					 unsigned short cyDest,unsigned char * pbmSource,
					 unsigned short srcWidth,unsigned short srcHeight, 
					 unsigned short xSrc, unsigned short ySrc, 
					 unsigned short cxSrc,unsigned short cySrc,
					 int  v_mode,unsigned short v_Transpantcolor)
{
	/*it will be implemented later if needed*/
	return ;
}





/* =================================================================================
 
						string displaying
 
 ================================================================================== */

 
 


 /* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   15:59
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned short Tf3rd5_0GetPixelNumofString(unsigned char v_CharSet,
										   unsigned char * p_Str, 
										   unsigned short v_StrLen)
{
     /*it will be implemented later if needed*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   16:35
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned short Tf3rd5_1GetCharNumofCertainPixel(unsigned char v_CharSet, 
												unsigned char * p_Str, 
												unsigned short v_StrLen, 
												unsigned short v_PixelNum)
{
     /*it will be implemented later if needed*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   16:54
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned char   Tf3rd5_2GetCharactorWidth(unsigned short v_CharCode)
{
     /*it will be implemented later if needed*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   16:59
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned char   Tf3rd5_3GetCharactorHeight(void)
{
     /*it will be implemented later if needed*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   17:00
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned short Tf3rd5_4DrawStringinBuffer(unsigned short *p_AreaBuf, 
										  unsigned short v_Width, 
										  unsigned short v_Height, 
								          unsigned short v_x, 
										  unsigned short v_y, 
										  unsigned char *p_String, 
										  unsigned short v_StrLen, 
                                          unsigned char v_CharSet, 
										  unsigned char v_Mode, 
										  unsigned short v_Color)
{
	/*it will be implemented later if needed*/
	return 0;
}

/* ==================================================================================
    Function:		
	Func Type:

	Author:			xin.hou@mailhz.techfaith.cn
	
	Created:		5:12:2007   17:04
	
	Description:

	Parameters:		input:
			
					output:

	Return value:
  ================================================================================ */
unsigned short Tf3rd5_5DrawStringinBufferEx(unsigned short *p_AreaBuf, 
											unsigned short v_Width, 
											unsigned short v_Height, 
											short v_x, short v_y, 
											unsigned char *p_String, 
											unsigned short  v_StrLen, 
											unsigned char v_CharSet,
											unsigned char v_Mode, 
											unsigned short v_Color, 
                                 unsigned short v_clipLeft, 
											unsigned short v_clipTop,   
											unsigned short v_clipRight, 
											unsigned short v_clipBottom)
{
     /*it will be implemented later if needed*/
	return 0;
}


typedef struct _St_PathDetail
{
   E_PathType   eType;
   char         acPath[256];
   unsigned int iRef;
}St_PathDetail;

static St_PathDetail       *g_pstPathDetail = NULL;
static E_ActivePathType    eActivePath = E_ACTIVE_PATH_IS_PHONE;

static int BRWENG10_SetRootDirPath(char* pcRootDir)
{
   if(NULL == pcRootDir)
      return E_FAILED;

   if(NULL == g_pstPathDetail)
   {
      g_pstPathDetail = TF3RD_MALLOCMEMORY(sizeof(St_PathDetail)*E_PATH_TYPE_NUMBER_MAX);
      if(NULL == g_pstPathDetail)
         return E_FAILED;
      TF3RD_MEMSET(g_pstPathDetail, 0x00, sizeof(St_PathDetail)*E_PATH_TYPE_NUMBER_MAX);
   }

   g_pstPathDetail->iRef++;

   g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].eType  = E_PATH_TYPE_ROOT_PATH;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath, pcRootDir);

   /*MMS path*/
//   g_pstPathDetail[E_PATH_TYPE_MMS_PATH].eType  = E_PATH_TYPE_MMS_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_MMS_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_MMS_PATH].acPath, "\\mms\\message");

//   g_pstPathDetail[E_PATH_TYPE_MMS_FOLDER_PATH].eType  = E_PATH_TYPE_MMS_FOLDER_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_MMS_FOLDER_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_MMS_FOLDER_PATH].acPath, "\\mms\\mbox");

//   g_pstPathDetail[E_PATH_TYPE_MMS_TRASH_PATH].eType  = E_PATH_TYPE_MMS_TRASH_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_MMS_TRASH_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_MMS_TRASH_PATH].acPath, "\\mms\\mbox\\trash.dat");

//   g_pstPathDetail[E_PATH_TYPE_MMS_INFO_PATH].eType  = E_PATH_TYPE_MMS_INFO_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_MMS_INFO_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_MMS_INFO_PATH].acPath, "\\mms\\mbox\\boxinfo.dat");

//   g_pstPathDetail[E_PATH_TYPE_MMS_MEDIA_PATH].eType  = E_PATH_TYPE_MMS_MEDIA_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_MMS_MEDIA_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_MMS_MEDIA_PATH].acPath, "\\mms\\message\\mms");

   /*Push path*/
//   g_pstPathDetail[E_PATH_TYPE_PUSH_SMSSTATE_PATH].eType  = E_PATH_TYPE_PUSH_SMSSTATE_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_PUSH_SMSSTATE_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_PUSH_SMSSTATE_PATH].acPath, "\\push\\sms\\sms_status.dat");

//   g_pstPathDetail[E_PATH_TYPE_PUSH_PATH].eType  = E_PATH_TYPE_PUSH_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_PUSH_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_PUSH_PATH].acPath, "\\push");

//   g_pstPathDetail[E_PATH_TYPE_PUSH_SMS_PATH].eType  = E_PATH_TYPE_PUSH_SMS_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_PUSH_SMS_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_PUSH_SMS_PATH].acPath, "\\push\\sms");

//   g_pstPathDetail[E_PATH_TYPE_PUSH_DM_PATH].eType  = E_PATH_TYPE_PUSH_DM_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_PUSH_DM_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_PUSH_DM_PATH].acPath, "\\push\\dm");

//   g_pstPathDetail[E_PATH_TYPE_PUSH_PDU_PATH].eType  = E_PATH_TYPE_PUSH_PDU_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_PUSH_PDU_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_PUSH_PDU_PATH].acPath, "\\push\\sms\\%08x_%08x.sms");

   /*Email path*/
   g_pstPathDetail[E_PATH_TYPE_EMAIL_PATH].eType  = E_PATH_TYPE_EMAIL_PATH;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_EMAIL_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_ROOT_PATH].acPath);
   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_EMAIL_PATH].acPath, "\\email");

   g_pstPathDetail[E_PATH_TYPE_EMAIL_EML_PATH].eType  = E_PATH_TYPE_EMAIL_EML_PATH;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_EMAIL_EML_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_EMAIL_PATH].acPath);
   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_EMAIL_EML_PATH].acPath, "\\eml");

   g_pstPathDetail[E_PATH_TYPE_EMAIL_ACCOUNT_PATH].eType  = E_PATH_TYPE_EMAIL_ACCOUNT_PATH;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_EMAIL_ACCOUNT_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_EMAIL_PATH].acPath);
   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_EMAIL_ACCOUNT_PATH].acPath, "\\account");

   g_pstPathDetail[E_PATH_TYPE_EMAIL_INDEX_PATH].eType  = E_PATH_TYPE_EMAIL_INDEX_PATH;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_EMAIL_INDEX_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_EMAIL_PATH].acPath);
   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_EMAIL_INDEX_PATH].acPath, "\\account\\account.act");

   g_pstPathDetail[E_PATH_TYPE_EMAIL_TEMP_PATH].eType  = E_PATH_TYPE_EMAIL_TEMP_PATH;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_EMAIL_TEMP_PATH].acPath, g_pstPathDetail[E_PATH_TYPE_EMAIL_PATH].acPath);
   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_EMAIL_TEMP_PATH].acPath, "\\received\\");

   g_pstPathDetail[E_PATH_TYPE_EMAIL_ACTIVE_ACCOUNT].eType  = E_PATH_TYPE_EMAIL_ACTIVE_ACCOUNT;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_EMAIL_ACTIVE_ACCOUNT].acPath, g_pstPathDetail[E_PATH_TYPE_EMAIL_PATH].acPath);
   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_EMAIL_ACTIVE_ACCOUNT].acPath, "\\account\\activeaccount.dat");

   return E_SUCCESS;
}

int BRWENG10_SetSaveAsPath(char* pcPath)
{
   if(NULL == pcPath || NULL == g_pstPathDetail)
      return E_FAILED;

   /*MMS path*/
//   Tf3rd2_4MemSet(&g_pstPathDetail[E_PATH_TYPE_MMS_CARD_PATH], 0x00, sizeof(St_PathDetail));
//   g_pstPathDetail[E_PATH_TYPE_MMS_CARD_PATH].eType  = E_PATH_TYPE_MMS_CARD_PATH;
//   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_MMS_CARD_PATH].acPath, pcPath);
//   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_MMS_CARD_PATH].acPath, "\\mms\\message");

   /*Email path*/
   Tf3rd2_4MemSet(&g_pstPathDetail[E_PATH_TYPE_EMAIL_EML_CARD_PATH], 0x00, sizeof(St_PathDetail));
   g_pstPathDetail[E_PATH_TYPE_EMAIL_EML_CARD_PATH].eType  = E_PATH_TYPE_EMAIL_EML_CARD_PATH;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_EMAIL_EML_CARD_PATH].acPath, pcPath);
   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_EMAIL_EML_CARD_PATH].acPath, "\\email\\eml");

   Tf3rd2_4MemSet(&g_pstPathDetail[E_PATH_TYPE_EMAIL_TEMP_CARD_PATH], 0x00, sizeof(St_PathDetail));
   g_pstPathDetail[E_PATH_TYPE_EMAIL_TEMP_CARD_PATH].eType  = E_PATH_TYPE_EMAIL_TEMP_CARD_PATH;
   Tf3rd2_12StrCpy(g_pstPathDetail[E_PATH_TYPE_EMAIL_TEMP_CARD_PATH].acPath, pcPath);
   Tf3rd2_9StrCat(g_pstPathDetail[E_PATH_TYPE_EMAIL_TEMP_CARD_PATH].acPath, "\\email\\received\\");

   return E_SUCCESS;
}

void BREWNG10_SetCardIsActive(void)
{
   eActivePath = E_ACTIVE_PATH_IS_CARD;
}

void BREWNG10_SetPhoneIsActive(void)
{
   eActivePath = E_ACTIVE_PATH_IS_PHONE;
}

E_ActivePathType BRWENG10_GetActivePathType(void)
{
   return eActivePath;
}

static int BRWENG10_SetDefaultDirPath(void)
{
   return BRWENG10_SetRootDirPath(TS_ROOT_PATH);
}

char* BRWENG10_GetPathByType(E_PathType eType)
{
   int iRet = E_SUCCESS;
   int iIndex = 0;
   if(NULL == g_pstPathDetail)
   {
      iRet =BRWENG10_SetDefaultDirPath();
      if(E_SUCCESS != iRet)
         return NULL;
   }

   while(iIndex < E_PATH_TYPE_NUMBER_MAX)
   {
      if(g_pstPathDetail[iIndex].eType == eType)
      {
         return g_pstPathDetail[iIndex].acPath;
      }
      iIndex++;
   }

   return NULL;
}

void BRWENG10_FreeDirPath(void)
{
   if(NULL == g_pstPathDetail)
      return;

   g_pstPathDetail->iRef--;
   if(g_pstPathDetail->iRef == 0)
   {
      Tf3rd2_7MemFree(g_pstPathDetail);
      g_pstPathDetail = NULL;
   }
}

