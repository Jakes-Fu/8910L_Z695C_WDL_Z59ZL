/*****************************************************************************
** File Name:      qbase.h                                                  *
** Author:                                                                   *
** Date:             11/05/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare QQ                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010       fen.xie       Create
******************************************************************************/
#ifndef _QQ_BASE_H_
#define _QQ_BASE_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "os_api.h"
#include "mmk_type.h"
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
//¹«ÓÃº¯Êý
#define MEMSET                  SCI_MEMSET
#define MEMCPY                  SCI_MEMCPY
#define SPRINTF					sprintf
#define STRSTR                  strstr
#define STRCMP                  strcmp
#define STRNCMP                 strncmp
#define STRNCPY                 strncpy 
#define STRCPY                  SCI_STRCPY
#define STRLEN                  SCI_STRLEN
#define STRCAT                  strcat
#define ATOI                    atoi

#define MEMCMP		memcmp 
#define WSTRLEN		MMIAPICOM_Wstrlen
#define WSTRCAT		MMIAPICOM_Wstrcat
#define WSTRCPY		MMIAPICOM_Wstrcpy
#define WSTRNCPY	MMIAPICOM_Wstrncpy
#define WSTRLCPY	MMIAPICOM_Wstrncpy
#define WSTRNCOPY(des, n, src)	MMIAPICOM_Wstrncpy(des, src, n)
#define WSTRCMP     MMIAPICOM_Wstrcmp
#define WSTRNCMP	MMIAPICOM_Wstrncmp
#define WSTRNCAT    MMIAPICOM_Wstrncat
#define STRTOWSTR(src, des, n)	MMIAPICOM_StrToWstr(src, des)
#define WSTRTOSTR(src, des, n)	MMIAPICOM_WstrToStr(src, des)


#define DBGPRINTF       
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/

typedef  unsigned int	RGBVAL;
typedef  unsigned short AECHAR;

enum{
	AVK_0 = 57377,                           // hex 0xE021; dec 57377
		AVK_1,                               // hex 0xE022; dec 57378
		AVK_2,                               // hex 0xE023; dec 57379
		AVK_3,                               // hex 0xE024; dec 57380
		AVK_4,                               // hex 0xE025; dec 57381
		AVK_5,                               // hex 0xE026; dec 57382
		AVK_6,                               // hex 0xE027; dec 57383
		AVK_7,                               // hex 0xE028; dec 57384
		AVK_8,                               // hex 0xE029; dec 57385
		AVK_9,                               // hex 0xE02A; dec 57386
		AVK_STAR,                            // hex 0xE02B; dec 57387
		AVK_POUND,                           // hex 0xE02C; dec 57388
		
		AVK_POWER,                           // hex 0xE02D; dec 57389
		AVK_END,                             // hex 0xE02E; dec 57390
		AVK_SEND,                            // hex 0xE02F; dec 57391
		AVK_CLR,                             // hex 0xE030; dec 57392
		
		AVK_UP,                              // hex 0xE031; dec 57393
		AVK_DOWN,                            // hex 0xE032; dec 57394
		AVK_LEFT,                            // hex 0xE033; dec 57395
		AVK_RIGHT,                           // hex 0xE034; dec 57396
		AVK_SELECT,                          // hex 0xE035; dec 57397
		
		AVK_SOFT1,
		AVK_SOFT2,
		
		EVT_KEY,
		EVT_KEY_RELEASE,
		EVT_PEN_UP,
		EVT_PEN_MOVE,
		EVT_PEN_DOWN
};

        

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif