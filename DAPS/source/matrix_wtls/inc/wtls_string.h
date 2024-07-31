
/*****************************************************************************
** File Name:      wtlsstring.h                                                *
** Author:         undiso.cheng                                                *
** Date:           2009.2                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    list functions                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.2     undiso.cheng         Create                                 *
******************************************************************************/

#ifndef _WTLS_STRING_H_
#define _WTLS_STRING_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "cfl_mem.h"
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
#define WTLS_OctstrGetChar CFL_OctstrGetChar
#define WTLS_OctstrSetChar CFL_OctstrSetChar
#define WTLS_OctstrDelete CFL_OctstrDelete
#define WTLS_OctstrGetCstr CFL_OctstrGetCstr
#define WTLS_OctstrRetrieveToData CFL_OctstrRetrieveToData
#define WTLS_OctstrReset CFL_OctstrReset
#define WTLS_OctstrCompare CFL_OctstrCompare
#define WTLS_UnpackInt24 CFL_UnpackInt24
#define WTLS_UnpackInt32 CFL_UnpackInt32
#define WTLS_OctstrDestroy CFL_OctstrDestroy
#define WTLS_OctstrCreateEmpty CFL_OctstrCreateEmpty
#define WTLS_OctstrGrow CFL_OctstrGrow
#define WTLS_OctstrCreateFromData CFL_OctstrCreateFromData
#define WTLS_OctstrCreate CFL_OctstrCreate
#define WTLS_OctstrCopy CFL_OctstrCopy
#define WTLS_OctstrDuplicate  CFL_OctstrDuplicate  
#define WTLS_OctstrAppendChar CFL_OctstrAppendChar
#define WTLS_OctstrAppendCstr CFL_OctstrAppendCstr
#define WTLS_OctstrAppendData CFL_OctstrAppendData
#define WTLS_OctstrInsertData CFL_OctstrInsertData
#define WTLS_OctstrCat CFL_OctstrCat
#define WTLS_OctstrInsert CFL_OctstrInsert
#define WTLS_OctstrAppend CFL_OctstrAppend
#define WTLS_PackInt16 CFL_PackInt16
#define WTLS_PackInt32 CFL_PackInt32
#define WTLS_UnpackOctstr CFL_UnpackOctstr
#define WTLS_UnpackOctstrFixed CFL_UnpackOctstrFixed
#define WTLS_PackOctstrFixed CFL_PackOctstrFixed
#define WTLS_PackOctstr CFL_PackOctstr
#define WTLS_ListCreate  CFL_ListCreat
#define WTLS_ListInsertNode CFL_ListInsertNode

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif //#ifdef _WTLS_STRING_H
