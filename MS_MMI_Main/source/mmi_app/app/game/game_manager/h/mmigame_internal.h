/*****************************************************************************
** File Name:      mmigame_internal.h                                                   *
** Author:                                                                   *
** Date:           04/03/2006                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the internal api of game              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/03/2006      Annie.an      Create
******************************************************************************/

#ifndef _MMIGAME_INTERNAL_H_
#define _MMIGAME_INTERNAL_H_
#ifdef GAME_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmigame_export.h"
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

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIGAME_RegWinIdNameArr(void);

/*****************************************************************************/
// 	Description :  Register menu group
//	Global resource dependence : 
//  Author:KELLY.LI
//	Note: 
/*****************************************************************************/
PUBLIC void MMIGAME_RegMenuGroup(void);


#ifdef   __cplusplus
    }
#endif
#endif
#endif  

