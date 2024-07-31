/*************************************************************************
** File Name:      mmigame_snake_id.c                                          *
** Author:         peilong.zhou                                           *
** Date:           26/01/2011                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 26/01/2011  peilong.zhou       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef GAME_SNAKE_SUPPORT

#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmigame_snake_internal.h" 

#define WIN_ID_DEF(win_id, win_id_name)          win_id_name,

const uint8 mmigame_snake_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmigame_snake_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: peilong.zhou
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_SNAKE_RegWinIdNameArr(void)
{
#ifndef MMI_AUTOTEST_LOW_MEMORY
    MMI_RegWinIdNameArr(MMI_MODULE_GAME_SNAKE, (const uint8**)mmigame_snake_id_name_arr);
#endif
}
#endif
