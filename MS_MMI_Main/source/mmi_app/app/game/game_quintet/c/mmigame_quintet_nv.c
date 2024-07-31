/*************************************************************************
 ** File Name:      mmigame_quintet_nv.c                                             *
 ** Author:         wancan.you                                           *
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
#define _MMIGAME_QUINTET_NV_C_
#ifdef GAME_QUINTET_SUPPORT
#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"

/*the length of game nv*/
const uint16 game_quintet_nv_len[]=
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
PUBLIC void MMIGMQUT_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_GAME_QUINTET, game_quintet_nv_len, sizeof(game_quintet_nv_len) / sizeof(uint16));
}

#endif











