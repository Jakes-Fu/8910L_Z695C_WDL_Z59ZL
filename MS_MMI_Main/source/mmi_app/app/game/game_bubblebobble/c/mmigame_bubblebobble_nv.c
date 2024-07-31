/*************************************************************************
** File Name:      mmigame_bubblebobble_nv.c                                             *
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
#define _MMIGAME_BUBBLEBOBBLE_NV_C_
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
#include "mmi_modu_main.h"


/*the length of game nv*/
const uint16 game_bubblebobble_nv_len[]=
{
    6,                         // MMINV_GAME_LEVER_SET,    
        6                          // MMINV_GAME_HIGH_SCORE  
};

/*****************************************************************************/
// 	Description : register bubble module nv len and max item
//	Global resource dependence : none
//    Author: junxue.zhu
//	Note:
/*****************************************************************************/
PUBLIC void MMIGMBUBBLEBOBBLE_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_GAME_BUBBLEBOBBLE, game_bubblebobble_nv_len, sizeof(game_bubblebobble_nv_len) / sizeof(uint16));
}

#endif











