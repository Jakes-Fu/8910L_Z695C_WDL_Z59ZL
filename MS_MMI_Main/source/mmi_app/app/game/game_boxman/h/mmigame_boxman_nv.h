/*************************************************************************
 ** File Name:      mmigame_boxman_nv.h                                  *
 ** Author:         wancan.you                                           *
 ** Date:           11/21/2005                                           *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 11/21/2005     annie.an        Create.                              *
*************************************************************************/

#ifndef _MMIGAME_BOXMAN_NV_H_
#define _MMIGAME_BOXMAN_NV_H_
#ifdef GAME_BOXMAN_SUPPORT
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
        MMINV_GMBOXMAN_LEVER_SET = MMI_MODULE_GAME_BOXMAN<< 16,  
        MMINV_GMBOXMAN_HIGH_SCORE,
		MMINV_GMBOXMAN_NV_ITEM_NUM
       }GMBOXMAN_NV_ITEM_E;

/*****************************************************************************/
// 	Description : register game module nv len and max item
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIGMBOXMAN_RegNv(void);

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





