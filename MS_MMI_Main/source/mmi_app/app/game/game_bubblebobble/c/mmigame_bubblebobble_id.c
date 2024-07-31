/*************************************************************************
** File Name:      mmigame_bubblebobble_id.c                                          *
** Author:          junxue.zhu                                         *
** Date:             2012/06/01                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE                   NAME             DESCRIPTION                          *
**2012/06/01        junxue.zhu          Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
//#include "sci_types.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmigame_bubblebobble_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "mmigame_bubblebobble_id.def"    
};

#undef WIN_ID_DEF


/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_BUBBLEBOBBLE_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_GAME_BUBBLEBOBBLE, mmigame_bubblebobble_id_name_arr);
}
#endif
