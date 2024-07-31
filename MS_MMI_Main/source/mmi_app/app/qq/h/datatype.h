/*****************************************************************************
** File Name:      qq2009.h                                                  *
** Author:                                                                   *
** Date:             11/05/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare QQ datatype               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010       fen.xie       Create
******************************************************************************/
#ifndef _DATATYPE_HEAD_H
#define _DATATYPE_HEAD_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "qbase.h"
#include "qfile.h"
#include "qsocket.h"
#include "qsystem.h"
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
#define QFALSE  	0
#define QTRUE  		1
#define QNULL		0

#define QMalloc(size)        QQMEMMGR_Malloc(size)
#define QFree(p)             QQMEMMGR_Free(p)



#define GETUPTIMEMS           SCI_GetTickCount


#define CHANGEIP(x)			    ((x>>24)&0xff)|(((x>>16)&0xff)<<8)|(((x>>8)&0xff)<<16)|((x&0xff)<<24)
#define MAX_FILEPATH_LEN        100

#ifndef SUCCESS
#define SUCCESS 0
#endif

#define RGB_NONE              (0xffffffffL)
#define RGB_WHITE             (MAKE_RGB(0xff, 0xff, 0xff))
#define RGB_BLACK             (MAKE_RGB(0,0,0))

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//数据类型重定义
typedef char 				QS8;
typedef unsigned char		QU8;
typedef signed short		QS16;
typedef unsigned short		QU16;
typedef signed long 		QS32;
typedef unsigned long 		QU32;
typedef unsigned char 		Qbool;


//方法重定义
typedef void (*TIMER_NOTIFY)(void* pUser);

extern Qbool	 QQTimer_AddTask(void* pUser, TIMER_NOTIFY pfn, QU32 nInterval, Qbool bLoop);
extern void 	 QQTimer_CancelTask(void * pUser,TIMER_NOTIFY pfn);

//方法重定义

extern void*    QQMEMMGR_Create(void);
extern void     QQMEMMGR_Release(void* p);
extern void*	QQMEMMGR_Malloc(QS32 nSize);
extern void		QQMEMMGR_Free(void* p);


typedef struct _IShell	 IShell;
typedef struct _IDisplay IDisplay;


//公用函数
IShell*		    QGetIShell(void);
IDisplay*		QGetIDisplay(void);
IFileMgr*		QGetIFileMgr(void);
void*       	QGetMqq(void);
int			    QGetDeviceW(void);
int			    QGetDeviceH(void);

//通用全局变量（移植暂时用，需要修改）
extern int SCR_W;
extern int SCR_H;

                         
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif

