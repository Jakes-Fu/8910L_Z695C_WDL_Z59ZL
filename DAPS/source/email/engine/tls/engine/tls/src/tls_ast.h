/*=====================================================================================
    FILE NAME   : wap_ast.h
    MODULE NAME : WAP_AST
    
    PRE-INCLUDE FILES DESCRIPTION
        we_def.h
        
    GENERAL DESCRIPTION
        Ast is assistant
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    Modification Tracking
    Date       Author       Number    Description of changes
    ---------- ------------ --------- --------------------------------------------
    2006-05-24 ChenZhiFeng  none      Init
    2006-08-02 ChenZhifeng  none      Add MEM_TEST
    
=====================================================================================*/

/**************************************************************************************
*   Multi-Include-Prevent Section
**************************************************************************************/
#ifndef WAP_AST_H
#define WAP_AST_H
#include "we_cfg.h"
#include "we_scl.h"
#ifdef WE_CFG_BREW_PLATFORM
#include "AEESTDLIB.h"
#else
#ifdef WIN32_DEBUG
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#else
#include "tf3rd_for_HZ.h"
#endif
#endif
#include "tls_def.h"
//#include "we_log.h"

#include "tls_dbg.h"


#ifdef MAX
#undef MAX
#endif
#define MAX(a, b)  ((a) > (b) ? (a) : (b))

#ifdef MIN
#undef MIN
#endif
#define MIN(a, b)  ((a) < (b) ? (a) : (b))

#ifdef WAP_MEM_TEST_DBG_ON
#if WAP_MEM_TEST_DBG_ON==1
#define MEM_TEST     /* If defined, it test for memory. And we must undefine it, when release */
#endif
#endif

#ifdef MEM_TEST   
void *Wap_Malloc
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    WE_INT32 iSize
);
void Wap_Free
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    void *pstMemBlock
);
WE_CHAR *Wap_StrDup
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    const WE_CHAR *pcString
);
void *Wap_MemCpy
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    void    *pvDest,
    void    *pvSrc,
    WE_UINT32 iSize
);
void *Wap_MemSet
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    void       *pvDest,
    WE_INT32    iChar,
    WE_INT32    iSize
);
void *Wap_MemMove
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    void    *pvDest,
    void    *pvSrc,
    WE_INT32 iSize
);
void Wap_StrnCpyLc
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    WE_CHAR *pcStrDest,
    const WE_CHAR *pcStrSource,
    WE_INT32 iLen
);

WE_CHAR * TLS_Strndup   
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    const WE_CHAR *pcStrSource,
    WE_INT32      iLen
);
WE_INT32 Wap_PrintMemInfo( void );
#endif



#ifdef WE_CFG_BREW_PLATFORM
#define WAP_SPRINTF     SPRINTF
#ifndef MEM_TEST
#define WAP_MALLOC      MALLOC
/* #define WAP_FREE        FREEIF */
#define WAP_FREE(x)        if( NULL !=x ) \
    { \
        FREEIF(x); \
        x = NULL; \
    }
WE_CHAR * TLS_Strndup   
(
    const WE_CHAR *pcStrSource,
    WE_INT32      iLen
); 

#define WAP_STRDUP      STRDUP
#define WAP_MEMCPY      MEMCPY
#define WAP_MEMSET      MEMSET
#define WAP_MEMMOVE     MEMMOVE
#define WAP_STRNCPYLC   WE_SCL_STRNCPYLC
#define TLS_STRNDUP     TLS_Strndup
#else
#define WAP_MALLOC(x)           Wap_Malloc(__FILE__, __LINE__, x)
#define WAP_FREE(x)             {Wap_Free(__FILE__, __LINE__, x); x=NULL;}
#define WAP_STRDUP(x)           Wap_StrDup(__FILE__, __LINE__, x)
#define WAP_MEMCPY(d,s,i)       Wap_MemCpy(__FILE__, __LINE__, d, s, i)
#define WAP_MEMSET(d,c,i)       Wap_MemSet(__FILE__, __LINE__, d, c, i)
#define WAP_MEMMOVE(d,s,i)      Wap_MemMove(__FILE__, __LINE__, d, s, i)
#define WAP_STRNCPYLC(d,s,l)    Wap_StrnCpyLc(__FILE__, __LINE__, d, s, l)
#define TLS_STRNDUP(s,l)        TLS_Strndup(__FILE__, __LINE__, s, l)
#endif
#define WAP_REALLOC     REALLOC         /* Not used in WAP */

#define WAP_MEMCHR      MEMCHR
#define WAP_STRCMPNC    WE_SCL_STRCMPNC
#define WAP_STRNCMPNC   WE_SCL_STRNCMPNC
#define WAP_MEMCMP      MEMCMP
#define WAP_STRCAT      STRCAT          /* Not used in WAP */
#define WAP_STRRCHR     STRRCHR
#define WAP_STRCHR      STRCHR
#define WAP_STRCPY      STRCPY          /* Not used in WAP */
#define WAP_STRLEN      STRLEN
#define WAP_ATOI        ATOI
#define WAP_STRSTR      STRSTR
#define WAP_STRTOL      STRTOUL
#define WAP_STRNCPY     STRNCPY         /* Not used in WAP */
#else
#ifdef WIN32_DEBUG
#define WAP_SPRINTF     sprintf
#define WAP_MALLOC(len) memset(malloc(len), 0, len)//Wap_Malloc//malloc
#define WAP_FREE(p)     {free(p),p=NULL;}
#define WAP_MEMCHR      memchr
#define WAP_MEMCPY      memcpy
#define WAP_MEMSET      memset
#define WAP_MEMMOVE     memmove
#define WAP_MEMCMP      memcmp
#define WAP_STRCAT      strcat
#define WAP_STRRCHR     strrchr
#define WAP_STRCHR      strchr
#define WAP_STRCPY      strcpy
#define WAP_STRNCPYLC   strncpy
#define WAP_STRCMPNC    WE_SCL_STRCMPNC
#define WAP_STRNCMPNC   WE_SCL_STRNCMPNC
#define WAP_STRDUP(src) (NULL == src ? NULL : strdup(src))      
#define WAP_STRLEN      strlen
#define WAP_ATOI        atoi
#define WAP_STRSTR      strstr
#define WAP_STRTOL      strtol
#define WAP_STRNCPY     strncpy
#define TLS_STRNDUP     TLS_Strndup
#else
#define WAP_SPRINTF     TF3RD_SPRINTF
#define WAP_MALLOC(len)	TF3RD_MEMSET(TF3RD_MALLOCMEMORY(len), 0, len)
#define WAP_FREE(p)		if(p){TF3RD_MEMFREE(p);p=NULL;}
#define WAP_MEMCHR      TF3RD_MEMCHR
#define WAP_MEMCPY      TF3RD_MEMCPY
#define WAP_MEMSET      TF3RD_MEMSET
#define WAP_MEMMOVE     TF3RD_MEMMOVE
#define WAP_MEMCMP      TF3RD_MEMCMP
#define WAP_STRCAT      TF3RD_STRCAT
#define WAP_STRRCHR     TF3RD_STRRCHR
#define WAP_STRCHR      TF3RD_STRCHR
#define WAP_STRCPY      TF3RD_STRCPY
#define WAP_STRNCPYLC   TF3RD_STRNCPY
#define WAP_STRCMPNC    WE_SCL_STRCMPNC
#define WAP_STRNCMPNC   WE_SCL_STRNCMPNC
#define WAP_STRDUP(src) (NULL == src ? NULL : TF3RD_STRDUP(src))      
#define WAP_STRLEN      TF3RD_STRLEN
#define WAP_ATOI        TF3RD_STRATOI
#define WAP_STRSTR      TF3RD_STRSTR
#define WAP_STRTOL      TF3RD_STRTOUL
#define WAP_STRNCPY     TF3RD_STRNCPY
#define TLS_STRNDUP     TLS_Strndup
#endif
#endif

#define TLS_GET_CURRENT_TIME    TlsAst_GetCurrentTime
WE_UINT32 TlsAst_GetCurrentTime( void );

#if 0


#define CT_UC    0x1u            /* Lower case: a-z */
#define CT_LC    0x2u            /* Upper case: A-Z */
#define CT_D     0x4u            /* Digit: 0-9 */
#define CT_X     0x8u            /* Hex digit: a-f, A-F */
#define CT_M     0x10u           /* Mark: -_.!~*'() */
#define CT_SRES  0x20u           /* Sometimes reserved: :@&=+$, */
#define CT_ARES  0x40u           /* Always reserved: ;/? */
#define CT_DL    0x80u           /* Delimiters: <>#%" */
#define CT_UW    0x100u          /* Unwise: {}|\^[]` */
#define CT_WS    0x200u          /* Whitespace: HT, VT, CR, LF, SP, FF */
#define CT_SP    0x400u          /* Space: ' ' */
#define CT_B     0x800u          /* Blank: SP TAB */
#define CT_C     0x1000u         /* Ctlr character: 0x0-0x1f, 0xff */
#define CT_T     0x2000u         /* Tspecials: ()<>@,;:\"/[]?={}SPHT */
#define CT_ESC   0x4000u         /* Escape character: % */
#define CT_XWS   0x8000u         /* XML whitespace character: HT, CR, LF, SP */


#define WAP_IS_ASCII(c)         ((WE_UINT8)(c) < 0x80)
#define WAP_IS_ALPHA(c)         (Wap_Is(c) & (CT_UC | CT_LC))
#define WAP_IS_ALPHANUM(c)      (Wap_Is(c) & (CT_UC | CT_LC | CT_D))
#define WAP_IS_LOWER(c)         (Wap_Is(c) & CT_LC)
#define WAP_IS_SUPPER(c)         (Wap_Is(c) & CT_UC)
#define WAP_IS_DIGIT(c)         (Wap_Is(c) & CT_D)
#define WAP_IS_HEX(c)           (Wap_Is(c) & (CT_D | CT_X))
#define WAP_IS_HEXLETTER(c)     (Wap_Is(c) & CT_X)
#define WAP_IS_BLANK(c)         (Wap_Is(c) & (CT_B))
#define WAP_IS_WHITESPACE(c)    (Wap_Is(c) & CT_WS)
#define WAP_IS_XMLWHITESPACE(c) (Wap_Is(c) & CT_XWS)
#define WAP_IS_CTRL(c)          (Wap_Is(c) & CT_C)

#define WAP_IS_ESCAPE(c)        (Wap_Is(c) & CT_ESC)
#define WAP_IS_RESERVED(c)      (Wap_Is(c) & (CT_SRES | CT_ARES))
#define WAP_IS_EXCLUDED(c)      (Wap_Is(c) & (CT_C | CT_SP | CT_DL))
#define WAP_IS_DELIMITER(c)     (Wap_Is(c) & CT_DL)
#define WAP_IS_UNWISE(c)        (Wap_Is(c) & CT_UW)
#define WAP_IS_TSPECIAL(c)      (Wap_Is(c) & CT_T)
#define WAP_IS_SPECIAL(c)       (Wap_Is(c) & (CT_C | CT_SP | CT_SRES | CT_ARES \
                                                                 | CT_UW | CT_DL))
#define WAP_IS_PCHAR(c)         (Wap_Is(c) & (CT_UC | CT_LC | CT_D | CT_M | \
                                                                 CT_SRES))
#define WAP_IS_URIC(c)          (Wap_Is(c) & (CT_SRES | CT_ARES | CT_UC | \
                                                              CT_LC | CT_D | CT_M))
#endif                                                                 
/*
WE_CHAR * Wap_Strndup   
(
    const WE_CHAR *pcStrSource,
    WE_INT32      iLen
); 
*/








#endif  /* WAP_AST_H */

