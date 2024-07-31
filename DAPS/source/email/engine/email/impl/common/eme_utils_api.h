/*=====================================================================================
FILE NAME   : eme_utils_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some api for transaction.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                 create eme_utils_api.h
          
=====================================================================================*/
#ifndef _EME_UTILS_API_H
#define _EME_UTILS_API_H
#include "we_cfg.h"
/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/
//#ifndef WE_DEBUG_ERROR
//#define WE_DEBUG_ERROR
//#endif

//#ifndef WE_DEBUG_INFO
//#define WE_DEBUG_INFO
//#endif

#include "os_api.h"
#include "we_scl.h"
#include "we_def.h"
#include "eme_error.h"
#include "we_log.h"

/*Include Program Global head file*/
#include "tf3rd_for_hz.h"
/*Include module head file*/

typedef  unsigned short AECHAR;
/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
/************************************************************************/
/* switch Macros                                                        */
/************************************************************************/
 #ifndef EME_UTILS_MEMORY_MGR
 //#define EME_UTILS_MEMORY_MGR
 #endif

#ifndef EME_UTILS_MEMMGR_FOR_FREE
#define EME_UTILS_MEMMGR_FOR_FREE   0xAAAAAAAA
#endif
/************************************************************************/
/* log related implementations                                          */
/************************************************************************/

#ifndef EME_PROCESS_AS_SYNC
#define EME_PROCESS_AS_SYNC
#endif

#ifndef EME_UTILS_LOG_INFO
#define EME_UTILS_LOG_INFO(x)    WE_SCL_PRINTF(x)
#endif

#ifndef EME_UTILS_LOG_ERROR
#define EME_UTILS_LOG_ERROR(x)   WE_SCL_PRINTF(x)
#endif

/************************************************************************/
/* memory related implementations                                       */                                                                
/************************************************************************/
struct St_Mms_Mem_Status
{
   unsigned long dwLLibMemObj;//num of memory blocks allocated
   unsigned long dwLLibMemUsed;//memory used for data content
   unsigned long dwLLibMemMax;
};

struct St_EME_Mem_Link
{
   unsigned long     dwCC1;// check sum , used to check if it is a valid node[10/6/2006 Samuel]
   struct St_EME_Mem_Link  *next;// reverse list [10/6/2006 Samuel]
   struct St_EME_Mem_Link  *last;
   
   unsigned long             dwLength;// memory length allocated [10/6/2006 Samuel]
   const char       *pcFileName;// file where the memory was allocated [10/6/2006 Samuel]
   unsigned long             dwLine ;// line where memory was allocated [10/6/2006 Samuel]
   unsigned long             dwID;// times memory allocated [10/6/2006 Samuel]
   unsigned long             dwCC2;//checksum   ,used to check if it is a valid node
   void              *pvMemMgr;
};

typedef struct St_EME_MemHandle
{
   struct St_EME_Mem_Link  stHeader;
   struct St_EME_Mem_Link * pTail;

   struct St_Mms_Mem_Status stStatus;
}St_EME_MemHandle;

void EME_UTILS_OutputMemUsed(St_EME_MemHandle* pMemHdr);
void* EME_Utils_Malloc(St_EME_MemHandle* pMemHdr, unsigned long nSize,const char * pFileName,int dwLine);
void* EME_Utils_Realloc(St_EME_MemHandle* pMemHdr,void *pSrc, unsigned long nSize,const char * pFileName,int dwLine);
int EME_Utils_Free(St_EME_MemHandle* pMemHdr, void * pMem,const char * pFileName,int dwLine);
WE_UINT32 EME_UTILS_GetTimems(void);

#ifndef EME_UTILS_MALLOC
   #ifdef EME_UTILS_MEMORY_MGR
      #define EME_UTILS_MALLOC(p,n) EME_Utils_Malloc(p, n, __FILE__, __LINE__) 
   #else
      #define EME_UTILS_MALLOC(p,n) WE_SCL_MALLOC(n)
   #endif
#endif


#ifndef EME_UTILS_REALLOC
   #ifdef EME_UTILS_MEMORY_MGR
       #define EME_UTILS_REALLOC(p,pSrc,n) EME_Utils_Realloc(p,pSrc, n, __FILE__, __LINE__) 
   #else
       #define EME_UTILS_REALLOC(p,pSrc,n)  TF3RD_REALLOC(pSrc,n)
   #endif
#endif


#ifndef EME_UTILS_FREE
   #ifdef EME_UTILS_MEMORY_MGR
      #define EME_UTILS_FREE(pMemHdr, p) EME_Utils_Free(pMemHdr, (void*)p, __FILE__, __LINE__)
   #else 
      #define EME_UTILS_FREE(pMemHdr, p) WE_SCL_FREE(p)
   #endif
#endif

/*Define Constant Macro end*/
#ifndef EME_UTILS_FREEIF
#define EME_UTILS_FREEIF(pMemHdr,p)    if (NULL != p) {EME_UTILS_FREE((void*)pMemHdr, p); (p) = 0; }
#endif

#ifndef EME_UTILS_MEMCPY
#define EME_UTILS_MEMCPY     (void)WE_SCL_MEMCPY
#endif 

#ifndef EME_UTILS_MEMSET
#define EME_UTILS_MEMSET     (void)WE_SCL_MEMSET
#endif 

#ifndef EME_UTILS_MEMCMP
#define EME_UTILS_MEMCMP     WE_SCL_MEMCMP
#endif

#ifndef EME_UTILS_MEMMOVE
#define EME_UTILS_MEMMOVE    WE_SCL_MEMMOVE
#endif
/************************************************************************/
/* time and second                                       */
/************************************************************************/
//#ifndef JULIANTYPE
//#define JULIANTYPE JulianType
//#endif

#ifndef EME_UTILS_GETJULIANDATE
#define EME_UTILS_GETJULIANDATE   TF3RD_GETJULDATE 
#endif

#ifndef EME_UTILS_GETTIMEMS
#define EME_UTILS_GETTIMEMS           WE_SCL_GETTIMES()
#endif

#ifndef EME_UTILS_GETLOCALSECONDS
#define EME_UTILS_GETLOCALSECONDS WE_SCL_GETSECONDS()
#endif

#ifndef EME_UTILS_JULIANTOSECONDS
#define EME_UTILS_JULIANTOSECONDS TF3RD_MKTIME
#endif



#ifndef EME_UTILS_GETLOCALOFFSET
#define EME_UTILS_GETLOCALOFFSET   (8*3600) //TF3RD_TIME()//LOCALTIMEOFFSET(NULL)
#endif

#ifndef EME_UTILS_GETSECONDS
#define EME_UTILS_GETSECONDS    (WE_SCL_GETSECONDS()/* + 315964800*/) /*31594800 = (1980-1-6) - (1970-1-1)*/
#endif


#ifndef  EME_UTILS_STRSTR
#define  EME_UTILS_STRSTR            TF3RD_STRSTR
#endif

extern SCI_MUTEX_PTR g_email_file_mutex;
#define EME_UTILS_GETMUTEX        SCI_GetMutex(g_email_file_mutex, SCI_WAIT_FOREVER);
#define EME_UTILS_PUTMUTEX        SCI_PutMutex(g_email_file_mutex);

extern SCI_MUTEX_PTR g_email_mem_mutex;
#define EME_MEM_GETMUTEX        SCI_GetMutex(g_email_mem_mutex, SCI_WAIT_FOREVER);
#define EME_MEM_PUTMUTEX        SCI_PutMutex(g_email_mem_mutex);

/************************************************************************/
/* string related implementations                                       */                          
/************************************************************************/
unsigned int EME_Utils_Atox(char *pcStr);
char *EME_UTILS_WstrToUtf8(St_EME_MemHandle* pMemHdr, AECHAR *pwcFrom, const char* pFileName, int dwLine);
AECHAR *EME_UTILS_Utf8ToWstr(St_EME_MemHandle* pMemHdr, char *pcFrom, const char* pFileName, int dwLine);
AECHAR *EME_UTILS_Utf8NToWstr(St_EME_MemHandle* pMemHdr, char *pcFrom, unsigned int uiOrigSize, unsigned int *puiDesSize);

char *EME_UTILS_WstrToStr(St_EME_MemHandle* pMemHdr, AECHAR *pwcFrom, const char* pFileName, int dwLine);
AECHAR *EME_UTILS_StrToWstr(St_EME_MemHandle* pMemHdr, char *pcFrom, const char* pFileName, int dwLine);
char *EME_UTILS_WstrNToUtf8(St_EME_MemHandle* pMemHdr, AECHAR *pwcFrom, unsigned int uiOrigSize, unsigned int *puiDesSize);
char* EME_UTILS_StrStrNC(char *pcToSearch, const char *pcSearchFor);
char *MMA_UTILS_GetExt(char  *pcFileName);
char* EME_UTILS_STRDUPREF(void* pMemHdr, char* p, const char* pFileName, int dwLine);
AECHAR* EME_UTILS_WSTRDUPREF(void* pMemHdr, AECHAR* p, const char* pFileName, int dwLine);
void* EME_Utils_Realloc(St_EME_MemHandle* pMemHdr,void *pSrc, unsigned long nSize,const char * pFileName,int dwLine);
void *EME_Utils_StrRealloc(St_EME_MemHandle *pMemHdr, void *pStr, unsigned long nSize, const char* pFileName, int dwLine);
/*BASE64 encode*/
int EME_UTILS_Base64Encode
(
    const char   *pcInBuf, 
    int        iInLen, 
    char         *pcOutBuf, 
    int        *piOutLen
);

#ifndef EME_UTILS_Base64EncodeLen   
#define  EME_UTILS_Base64EncodeLen(c)  ((c == 0) ? 0 : ((c + 2) / 3) * 4)
#endif

#ifndef EME_UTILS_GETEXT
#define EME_UTILS_GETEXT   MMA_UTILS_GetExt
#endif

#ifndef EME_UTILS_BASE64LEN
#define EME_UTILS_BASE64LEN   WE_SCL_BASE64ENCODELEN
#endif

#ifndef EME_UTILS_WSTRTOSTR
#define EME_UTILS_WSTRTOSTR(pMemHdr, p)     EME_UTILS_WstrToStr(pMemHdr, p, __FILE__, __LINE__)
#endif

#ifndef EME_UTILS_STRTOWSTR
#define EME_UTILS_STRTOWSTR(pMemHdr, p)     EME_UTILS_StrToWstr(pMemHdr, p, __FILE__, __LINE__)
#endif

#ifndef EME_UTILS_WSTRTOUTF8
#define EME_UTILS_WSTRTOUTF8(pMemHdr, p)    EME_UTILS_WstrToUtf8(pMemHdr, p, __FILE__, __LINE__)
#endif

#ifndef EME_UTILS_UTF8TOWSTR
#define EME_UTILS_UTF8TOWSTR(pMemHdr, p)    EME_UTILS_Utf8ToWstr(pMemHdr, p, __FILE__, __LINE__)
#endif

#ifndef EME_UTILS_ATOX
#define EME_UTILS_ATOX          EME_Utils_Atox
#endif

#ifndef EME_UTILS_STRCMPNC
#define EME_UTILS_STRCMPNC      TF3RD_STRICMP
#endif
 
#ifndef EME_UTILS_STRSTRNC
#define EME_UTILS_STRSTRNC       EME_UTILS_StrStrNC
#endif
#ifndef EME_UTILS_STRLEN
#define EME_UTILS_STRLEN         WE_SCL_STRLEN
#endif 

#ifndef EME_UTILS_STRCPY
#define EME_UTILS_STRCPY        WE_SCL_STRCPY
#endif 

#ifndef EME_UTILS_STRNCPY
#define EME_UTILS_STRNCPY        TF3RD_STRNCPY
#endif 

#ifndef EME_UTILS_STRCMP
#define EME_UTILS_STRCMP        WE_SCL_STRCMP
#endif 

#ifndef EME_UTILS_STRNCMP
#define EME_UTILS_STRNCMP       WE_SCL_STRNCMP
#endif 

#ifndef EME_UTILS_SPRINTF
#define EME_UTILS_SPRINTF       WE_SCL_SPRINTF
#endif 

#ifndef EME_UTILS_SNPRINTF
#define EME_UTILS_SNPRINTF       TF3RD_SNPRINTF
#endif 

#ifndef EME_UTILS_STRCAT
#define EME_UTILS_STRCAT        WE_SCL_STRCAT
#endif

#ifndef EME_UTILS_STRSTR
#define EME_UTILS_STRSTR        WE_SCL_STRSTR
#endif

#ifndef EME_UTILS_ATOI
#define EME_UTILS_ATOI          WE_SCL_ATOI
#endif

#ifndef EME_UTILS_WSPRINTF
#define EME_UTILS_WSPRINTF     TF3RD_WSPRINTF // WE_SCL_WSPRINTF
#endif

#ifndef EME_UTILS_WSTRCMP
#define EME_UTILS_WSTRCMP      TF3RD_WSTRCMP //WE_SCL_WSTRCMP
#endif

#ifndef EME_UTILS_WSTRICMP
#define EME_UTILS_WSTRICMP      TF3RD_WSTRCMP//TF3RD_WSTRICMP
#endif

#ifndef EME_UTILS_STRCHR
#define EME_UTILS_STRCHR       WE_SCL_STRCHR
#endif


#ifndef EME_UTILS_STRRCHR
#define EME_UTILS_STRRCHR      WE_SCL_STRRCHR
#endif

#ifndef EME_UTILS_WSTRLEN
#define EME_UTILS_WSTRLEN      WE_SCL_WSTRLEN
#endif

#ifndef EME_UTILS_STRLOWER
#define EME_UTILS_STRLOWER      TF3RD_STRLOWER
#endif

#ifndef EME_UTILS_STRUPPER
#define EME_UTILS_STRUPPER      TF3RD_STRUPPER
#endif

#ifndef EME_UTILS_STRUPPER
#define EME_UTILS_STRUPPER      TF3RD_STRUPPER
#endif

#ifndef EME_UTILS_SETTIMER      
#define EME_UTILS_SETTIMER      TF3RD_SETTIMER
#endif

#ifndef EME_UTILS_CANCELTIMER
#define EME_UTILS_CANCELTIMER   TF3RD_KILLTIMER
#endif

#ifndef EME_UTILS_STRTOUL 
#define EME_UTILS_STRTOUL       TF3RD_STRTOUL
#endif 
                               
#ifndef EME_UTILS_WSTRDUP
#define EME_UTILS_WSTRDUP(pMemHdr,p) EME_UTILS_WSTRDUPREF(pMemHdr, p, __FILE__, __LINE__)
#endif

#ifndef EME_UTILS_STRDUP
#define EME_UTILS_STRDUP(pMemHdr, p) EME_UTILS_STRDUPREF(pMemHdr, p, __FILE__, __LINE__)
#endif

#ifndef EME_UTILS_STRREALLOC
#define EME_UTILS_STRREALLOC(p,pSrc,n) EME_Utils_StrRealloc(p,pSrc, n, __FILE__, __LINE__) 
#endif
/************************************************************************/
/* used in do while(0) function structure for break                     */                                            
/************************************************************************/
#ifndef EME_UTILS_BREAKIF
#define EME_UTILS_BREAKIF(ret, desc)  {\
   if(SUCCESS != ret)\
                                       {\
                                       EME_UTILS_LOG_ERROR(("%s,ECode:%d(%s:%dL)", desc, (int)ret, __FILE__, __LINE__));\
                                       break;\
                                       }\
                                    }   
#endif //EME_UTILS_BREAKIF

#ifndef EME_UTILS_BREAKNULL
#define EME_UTILS_BREAKNULL(p, ret, retVal, desc) {\
                                                      if(NULL == p)\
                                                      {\
                                                         ret = retVal;\
                                                         EME_UTILS_LOG_ERROR(("%s,(%s:%dL)", desc,__FILE__, __LINE__));\
                                                         break;\
                                                      }\
                                                   }
#endif //EME_UTILS_BREAKNULL

#ifndef EME_UTILS_BREAK
#define EME_UTILS_BREAK(ret, desc)    EME_UTILS_LOG_ERROR(("%s,ECode:%d(%s:%dL)", desc, (int)ret, __FILE__, __LINE__));\
break;
#endif //EME_UTILS_BREAK


/************************************************************************/
/* platform related functions                                           */                      
/************************************************************************/
//#ifndef EME_UTILS_ADDREFIF
//#define EME_UTILS_ADDREFIF(p)         EME_UTILS_ADDREFPPIF((IBase**) (void *) &p)
//
//static __inline unsigned int EME_UTILS_ADDREFPPIF(IBase **p) 
//{
//   unsigned int uiCount = 0;
//   if (*p) 
//   {
//      uiCount = IBASE_AddRef(*p);
//   }
//   
//   return uiCount;
//}
//#endif


//#ifndef EME_UTILS_RELEASEIF
//#define EME_UTILS_RELEASEIF(p)       EME_UTILS_RELEASEPPIF((IBase**) (void *) &p)
//
//static __inline unsigned int EME_UTILS_RELEASEPPIF(IBase **p) 
//{
//   unsigned int uiCount = 0;
//   if (*p) 
//   {
//      uiCount = IBASE_Release(*p);
//      *p = 0;      
//   }
//   
//   return uiCount;
//}
//#endif

/*
　msgid = 0xFFFFFFFF
　0,第８位0xF为保留值
　1,第７位0xF代表存储位置，0x0表示手机， 0x1代表TFLASH卡
　2,第６位0xF代表信息对应的邮件账号
　3,第５位0xF代表对应账号的随机码
  4,后４位表示邮件对应的ID值
*/

#define EME_SET_PLACE_TO_MSGID(msgid,place)           ((((place)<<(3*4))&0x0000F000)|(msgid))
#define EME_SET_ACTID_TO_MSGID(msgid,actid)           ((((actid)<<(6*4))&0x0F000000)|(msgid))
#define EME_SET_RAND_TO_MSGID(msgid,randx)            ((((randx)<<(4*4))&0x00FF0000)|(msgid))

#define EME_GET_PLACE_FROM_MSGID(msgid)               (((msgid)>>(3*4))&0x0F)
#define EME_GET_ACTID_FROM_MSGID(msgid)               (((msgid)>>(6*4))&0x0F)
#define EME_GET_RAND_FROM_MSGID(msgid)                (((msgid)>>(4*4))&0xFF)

#define EME_UTILS_GETRAND                             eme_utils_getrand
static __inline eme_utils_getrand()
{
   uint8 uiRand = 0;
   uiRand = (uint8)rand();
   return (uiRand&0xFF);
}

//#include "we_precfgdef.h"

#endif             /* #ifndef EME_UTILS_API_H */
