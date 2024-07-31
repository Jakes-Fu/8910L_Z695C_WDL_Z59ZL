/*************************************************************************
 ** File Name:      mmigame_quintet_nv.h                                             *
 ** Author:         wancan.you                                           *
 ** Date:           11/21/2005                                           *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 11/21/2005     annie.an        Create.                              *
*************************************************************************/

#ifndef _MMIGAME_QUINTET_NV_H_
#define _MMIGAME_QUINTET_NV_H_
#ifdef GAME_QUINTET_SUPPORT
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
        MMINV_GMQUT_LEVER_SET = MMI_MODULE_GAME_QUINTET<< 16,  
        MMINV_GMQUT_HIGH_SCORE,
		MMINV_GMQUT_NV_ITEM_NUM
       }GMQUT_NV_ITEM_E;

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



