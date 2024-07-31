/*************************************************************************
** File Name:      mmigame_snake_nv.c                                             *
** Author:          peilong.zhou                                           *
** Date:            26/01/2011                                           *
** Copyright:     2001 Spreadtrum, Incorporated. All Rights Reserved.    *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 26/01/2011  peilong.zhou        Create.                              *
*************************************************************************/
#define _MMIGAME_SNAKE_NV_C_

#include "std_header.h"
#ifdef GAME_SNAKE_SUPPORT
#include "sci_types.h"

#include "mmi_modu_main.h"
#include "mmigame_snake_nv.h"

/*the length of game nv*/
const uint16 game_snake_nv_len[]=
{
    2,                         // MMINV_GAME_LEVER_SET,    等级设置
    2                          // MMINV_GAME_HIGH_SCORE  ，最高分
};

/*****************************************************************************/
//  Description : register game module nv len and max item
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_GAME_SNAKE, game_snake_nv_len, sizeof(game_snake_nv_len) / sizeof(uint16));
}
#endif

