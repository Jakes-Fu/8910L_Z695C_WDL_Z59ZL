/************************************************************************
** File Name:      mmigame_goldminer_nv.c                               *
** Author:                                                              *
** Date:           06/09/2012                                           *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file defines the function about nv              *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 06/09/2012                      Create.                              *
*************************************************************************/
#define _MMIGAME_GOLDMINER_NV_C_

#include "std_header.h"
#ifdef GAME_GOLDMINER_SUPPORT
#include "sci_types.h"

#include "mmi_modu_main.h"
#include "mmigame_goldminer_nv.h"

/*the length of game nv*/
const uint16 game_goldminer_nv_len[]=
{
    2,                         // MMINV_GOLDMINER_LEVEL_SET,    等级设置
    4                          // MMINV_GOLDMINER_HIGH_SCORE，最高分
};

/*****************************************************************************/
//  Description : register game module nv len and max item
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIGMGOLDMINER_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_GAME_GOLDMINER, game_goldminer_nv_len, sizeof(game_goldminer_nv_len) / sizeof(uint16));
}
#endif

