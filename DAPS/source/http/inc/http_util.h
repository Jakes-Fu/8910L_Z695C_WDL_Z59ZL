/*****************************************************************************
** File Name:      http_util.h				                                 *
** Author:                                                                   *
** Date:           01/16/2008                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    HTTP v1.0: util function				            		 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/16/2008     li.li             Create									 *	
******************************************************************************/

#ifndef _HTTP_UTIL_H_
#define _HTTP_UTIL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
#ifdef HTTP_MEM_DEBUG
#include "cfl_mem_debug.h"
#endif
#include "http_def.h"
#include "cfl_char.h"
#include "cfl_string.h"
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

/*when open this macro, there is memory leak for CFLMEM_Malloc.*/
//#define HTTP_MEM_DEBUG // for http cookie memory debug


#define HTTP_STRNICMP    CFLCHAR_StrNICmp
#define HTTP_STRICMP     CFLCHAR_StrICmp
#define HTTP_CREATE_FILE SFS_CreateFile
#define HTTP_WRITE_FILE  SFS_WriteFile 
#define HTTP_CLOSE_FILE  SFS_CloseFile 
#define HTTP_FIND_FIRST_FILE SFS_FindFirstFile
#define HTTP_FIND_CLOSE  SFS_FindClose
#define HTTP_DELETE_FILE SFS_DeleteFile
#define HTTP_DELETE_DIRECTORY SFS_DeleteDirectory

#ifndef HTTP_MEM_DEBUG
	/*Because free memory is not enough, cookie need use CFL_ALLOC() to malloc memory.*/
    #define HTTP_ALLOC(s) CFL_ALLOC(s)
    #define HTTP_COOKIEALLOC(s) CFL_ALLOC(s)//HttpCookie_dlMalloc(s)
    #define HTTP_FREE(p) CFL_FREE(p)
    #define HTTP_COOKIEFREE(p) CFL_FREE(p)//HttpCookie_dlFree(p)
	#define HttpCookieStrclone(p) CFL_StrClone(p)//HttpCookie_dlStrclone(p)
	#define HttpCookieRealloc(p, s, b) CFL_ReAlloc(p, s, b)//HttpCookie_dlRealloc(p, s, b)
#else
    #define HTTP_ALLOC(s) CFLMEM_Malloc(s, __FILE__,__LINE__)
    #define HTTP_COOKIEALLOC(s) CFLMEM_Malloc(s, __FILE__,__LINE__)
    #define HTTP_FREE(p) CFLMEM_Free(p)     
    #define HTTP_COOKIEFREE(p) CFLMEM_Free(p)
	#define HttpCookieStrclone(p) CFL_StrClone(p)
	#define HttpCookieRealloc(p, s, b) CFL_ReAlloc(p, s, b)
#endif
#define HTTP_MEMSET     memset
#define HTTP_MEMCPY     memcpy
#define HTTP_MEMNCPY    CFLCHAR_MEMCPY
#define HTTP_STRCPY     strcpy
#define HTTP_STRLEN     CFL_StrLen
#define HTTP_STRNCPY    CFLCHAR_StrNCpy
#define HTTP_MEMCMP     memcmp
#define HTTP_MEMMOVE    memmove
#define HTTP_STRSTR     strstr
#define HTTP_STRCAT     strcat
#define HTTP_STRNCAT    strncat
#define HTTP_STRCMP     strcmp
#define HTTP_STRNCMP    strncmp
#define HTTP_STRCHR     strchr
#define HTTP_STRRCHR    strrchr
#define HTTP_SSCANF     sscanf
#define HTTP_SPRINTF    sprintf
#define HTTP_SNPRINTF   CFL_Snprintf
#define HttpStrclone    CFL_StrClone
#define STR_IS_NOT_EMPTY(x)   (x!=PNULL && x[0]!=0)

#define _U      0x01    /* upper */
#define _L      0x02    /* lower */
#define _D      0x04    /* digit */
#define _C      0x08    /* cntrl */
#define _P      0x10    /* punct */
#define _B      0x20    /* blank (space/tab/cr/lf) */
#define _X      0x40    /* hex digit */
#define _S      0x80    /* space (0x20) */    
extern const uint8 g_ascii_table[];
#define HTTP_ISASCII(c)     (((unsigned int) c) <= 0x7f)
#define HTTP_TOASCII(c)     (((unsigned int) c) & 0x7f)
#define HTTP_ISCR(c)        ('\r' == (c))
#define HTTP_ISLF(c)        ('\n' == (c))

#define HTTP_ISCRORLF(c)    (HTTP_ISCR(c) || HTTP_ISLF(c) )
#define HTTP_ISCRLF(p)      (HTTP_ISCR(*(p)) && HTTP_ISLF(*((p)+1)) )
#define HTTP_ISLFLF(p)      (HTTP_ISLF(*(p)) && HTTP_ISLF(*((p)+1)) )
#define HTTP_ISCRCR(p)      (HTTP_ISCR(*(p)) && HTTP_ISCR(*((p)+1)) )
#define HTTP_ISCRLFCRLF(p)  (HTTP_ISCRLF((p)) && HTTP_ISCRLF((p)+2))

#define HTTP_TOUPPER(c)    (( (c) >= 'a' && (c) <= 'z') ? ((c)+'A'-'a') : (c))
#define HTTP_TOLOWER(c)    (( (c) >= 'A' && (c) <= 'Z') ? ((c)+'a'-'A') : (c))     

#define HTTP_ISALNUM(c)  ((g_ascii_table)[(uint8)(c)] & (_U|_L|_D)       )        
#define HTTP_ISALPHA(c)  ((g_ascii_table)[(uint8)(c)] & (_U|_L)          )
#define HTTP_ISCNTRL(c)  ((g_ascii_table)[(uint8)(c)] & (_C)             )
#define HTTP_ISDIGIT(c)  ((g_ascii_table)[(uint8)(c)] & (_D)             )
#define HTTP_ISGRAPH(c)  ((g_ascii_table)[(uint8)(c)] & (_P|_U|_L|_D)    )
#define HTTP_ISLOWER(c)  ((g_ascii_table)[(uint8)(c)] & (_L)             )
#define HTTP_ISPRINT(c)  ((g_ascii_table)[(uint8)(c)] & (_P|_U|_L|_D|_S) )
#define HTTP_ISPUNCT(c)  ((g_ascii_table)[(uint8)(c)] & (_P)             )
#define HTTP_ISBLANK(c)  ((g_ascii_table)[(uint8)(c)] & (_B)             )
#define HTTP_ISSPACE(c)  ((g_ascii_table)[(uint8)(c)] & (_S)             )
#define HTTP_ISUPPER(c)  ((g_ascii_table)[(uint8)(c)] & (_U)             )
#define HTTP_ISXDIGIT(c) ((g_ascii_table)[(uint8)(c)] & (_D|_X)          )

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : read one line
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC int32 HttpReadLine(char* buf_ptr,int32 length,char** line_buf_ptr);

/*****************************************************************************/
//  Description : trim quot
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpTrimQuot(char *str_ptr);

/*****************************************************************************/
//  Description : Find the next token in a string.
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpStrtoken(uint32 handle, char *str_token_ptr, char *str_delimit_ptr);

/*****************************************************************************/
//  Description : create token handle.
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 HttpCreateStrtokenHandle(void);

/*****************************************************************************/
//  Description : destroy token handle.
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpDestroyStrtokenHandle(uint32 handle);

/*****************************************************************************/
//  Description : gets the number of contiguous characters from the beginning
//  of string that are in charset
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 HttpStrspn(char *string_ptr, char *str_charset_ptr);

/*****************************************************************************/
//  Description : clone strings
//  of string that are in charset
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
// PUBLIC char* HttpStrclone(const char *str_source_ptr);

/*****************************************************************************/
//  Description : trim string
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpStrtrim(char* string_ptr);

/*****************************************************************************/
//  Description : trim string
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
char *HttpStrReplace(char *str, char old_ch, char new_ch);

/*****************************************************************************/
//  Description : separate the string into two part,the return is the first, 
//  and the rear_ptr is the second part
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpStrSeparate(char* buff_ptr, const char* separate_ptr, char **rear_ptr);

/*****************************************************************************/
//  Description : match two string from tail
//  and the rear_ptr is the second part
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpStrTailMatch(const char* domain1_ptr, const char* domain2_ptr);

/*****************************************************************************/
// 	Description : Same as strstr() only case insensitive
//	Global resource dependence :
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC char *HttpStrCaseStr(char *src_ptr,uint32 src_len,char *find_ptr);

/*****************************************************************************/
// 	Description : Find a match buffer
//	Global resource dependence :
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC void* HttpMemMatch(const void* text_ptr, uint32 text_len, void* match_ptr, uint32 match_len);

/*****************************************************************************/
//  Description : gets the number of contiguous characters from the beginning
//  of string that are not in strCharSet 
//  of string that are in charset
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 HttpStrcspn(char* string_ptr,char* str_charset_ptr);

/*****************************************************************************/
// 	Description : Convert an integer to a string.
//	Global resource dependence :
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC char* HttpItoa(int32 value, char* string_ptr, uint32 radix);

/*****************************************************************************/
// 	Description : move to host start.
//	Global resource dependence :
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC char* HttpMovetoHostStart(char* absolute_url_ptr);

/*****************************************************************************/
// 	Description : move to host end.
//	Global resource dependence :
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC char* HttpMovetoHostEnd(char *absolute_url_ptr);

/*****************************************************************************/
// 	Description : move to host end.
//	Global resource dependence :
//  Author: li.li
//	Note:      
/*****************************************************************************/
PUBLIC void HttpConvertToLowerCase(char *src_str_ptr,uint32 len);

/*****************************************************************************/
//  Description : Find the next token in a string.
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpStrtokenCookieValue(
                          uint32 handle,
                          char *str_token_ptr,  //[IN]
                          char *str_delimit_ptr //[IN]
                          );
#if 0//#ifndef HTTP_MEM_DEBUG
/*****************************************************************************/
//  Description : cookie mem malloc from dlmalloc
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC void* HttpCookie_dlMalloc(uint32 size);

/*****************************************************************************/
//  Description : cookie mem free from dlmalloc
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC void  HttpCookie_dlFree(void* ptr);
/*****************************************************************************/
//  Description : cookie strclone from dlmalloc
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpCookie_dlStrclone(void* ptr);
/*****************************************************************************/
//  Description : cookie strclone from dlmalloc
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC void* HttpCookie_dlRealloc(void   *old_mem_ptr, 
                                                             uint32 old_size,
                                                             uint32 new_size);
#endif
/*****************************************************************************/
//  Description : is cookie mem init
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpCookie_IsMemInit();
/****************************************************************************/
//Description : 如果host后面是?或者#，在host后加/
//Global resource dependence : 
//Author: li.li
//Note:	如果host后面是?或者#，在host后加/
/****************************************************************************/
PUBLIC char* HTTP_UrlFormat(char *string_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_HTTP_UTIL_H_
