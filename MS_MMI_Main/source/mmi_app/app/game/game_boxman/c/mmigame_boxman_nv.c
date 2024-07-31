/*************************************************************************
** File Name:      mmigame_boxman_nv.c                                             *
** Author:         annie.an                                           *
** Date:           11/21/2005                                           *
** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.    *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 11/21/2005     annie.an        Create.                              *
*************************************************************************/
#define _MMIGAME_BOXMAN_NV_C_
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BOXMAN_SUPPORT

//#include "sci_types.h"

#include "mmi_modu_main.h"
//#include "mmigame_link_nv.h"

/*the length of game nv*/
const uint16 game_boxman_nv_len[]=
{
    6,                         // MMINV_GAME_LEVER_SET,    
        6                          // MMINV_GAME_HIGH_SCORE  
};

/*****************************************************************************/
// 	Description : register game module nv len and max item
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIGMBOXMAN_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_GAME_BOXMAN, game_boxman_nv_len, sizeof(game_boxman_nv_len) / sizeof(uint16));
}

#endif











