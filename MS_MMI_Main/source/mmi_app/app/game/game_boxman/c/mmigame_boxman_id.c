/*************************************************************************
** File Name:      mmigame_boxman_id.c                                          *
** Author:         haiyang.hu                                           *
** Date:           2006/09/18                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2006/09/18     haiyang.hu       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BOXMAN_SUPPORT
//#include "sci_types.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmigame_boxman_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "mmigame_boxman_id.def"    
};

#undef WIN_ID_DEF


/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIGAME_BOXMAN_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_GAME_BOXMAN, mmigame_boxman_id_name_arr);/*lint !e64*/
}
#endif
