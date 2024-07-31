/*************************************************************************
** File Name:      mmigame_bubblebobble_nv.h                                             *
** Author:           junxue.zhu                                          *
** Date:             01/06/2012                                           *
** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.    *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE                 NAME             DESCRIPTION                          *
** 01/06/2012     junxue.zhu        Create.                              *
*************************************************************************/

#ifndef _MMIGAME_BUBBLEBOBBLE_NV_H_
#define _MMIGAME_BUBBLEBOBBLE_NV_H_
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
       {  
        MMINV_BUBBLEBOBBLE_LEVER_SET = MMI_MODULE_GAME_BUBBLEBOBBLE<< 16,  
        MMINV_BUBBLEBOBBLE_HIGH_SCORE,
		MMINV_BUBBLEBOBBLE_NV_ITEM_NUM
       }BUBBLEBOBBLE_NV_ITEM_E;

/*****************************************************************************/
// 	Description : register game module nv len and max item
//	Global resource dependence : none
//    Author:  junxue.zhu   
//	Note:
/*****************************************************************************/
PUBLIC void MMIGMBUBBLEBOBBLE_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif





