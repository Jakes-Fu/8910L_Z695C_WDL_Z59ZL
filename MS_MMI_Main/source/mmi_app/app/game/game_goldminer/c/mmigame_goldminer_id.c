/************************************************************************
** File Name:      mmigame_goldminer_id.c                               *
** Author:                                                              *
** Date:           06/09/2012                                           *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 06/09/2012                      Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef GAME_GOLDMINER_SUPPORT

#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmigame_goldminer_internal.h" 

#define WIN_ID_DEF(win_id, win_id_name)          win_id_name,

const uint8 mmigame_goldminer_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmigame_goldminer_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_GOLDMINER_RegWinIdNameArr(void)
{
//#ifndef MMI_AUTOTEST_LOW_MEMORY
    MMI_RegWinIdNameArr(MMI_MODULE_GAME_GOLDMINER, (const uint8**)mmigame_goldminer_id_name_arr);
//#endif
}
#endif
