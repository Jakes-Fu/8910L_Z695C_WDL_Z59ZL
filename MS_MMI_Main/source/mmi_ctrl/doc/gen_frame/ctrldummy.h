/*****************************************************************************
** File Name:      ctrl[ctrl].h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRL[CTRL]_H_
#define _CTRL[CTRL]_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrl[parent].h"
#include "ctrl[ctrl]_export.h"

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
// [ctrl] type info
#define CTRL_[CTRL]_TYPE     ([CTRL]_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// [ctrl] init param
typedef struct
{
    CTRL[PARENT]_INIT_PARAM_T       [parent]_ctrl_param;

    // self inif param ...
} CTRL[CTRL]_INIT_PARAM_T;

//[ctrl] control
typedef struct
{
    CTRL[PARENT]_OBJ_T              [parent]_ctrl;

    // self member ...
} CTRL[CTRL]_OBJ_T;

// [ctrl] class for public or protected method
typedef struct
{
    CTRL[PARENT]_VTBL_T            [parent]_ctrl_vtbl;

    // self vtbl ...
} CTRL[CTRL]_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get [ctrl] type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T [CTRL]_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
