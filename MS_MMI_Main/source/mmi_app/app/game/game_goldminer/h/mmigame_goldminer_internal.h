/*****************************************************************************
** File Name:      mmigame_goldminer_internal.h                              *
** Author:                                                                   *
** Date:           06/09/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe the internal api of game    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/09/2012                      Create                                    *
******************************************************************************/

#ifndef _MMIGAME_GOLDMINER_INTERNAL_H_
#define _MMIGAME_GOLDMINER_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef GAME_GOLDMINER_SUPPORT
#include "os_api.h"
#include "mmk_type.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmi_position.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
//游戏的难度
// typedef enum
// {
//     GOLDMINER_LEVEL_EASY,
//     GOLDMINER_MAX_LEVEL
// }GOLDMINER_LEVEL_E;
// 
// //游戏状态


/*****************************************************************************/
//  FUNCTION:      MMIGMGOLDMINER_Init(void)
//  Description :   
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMGOLDMINER_Init(void);

/*****************************************************************************/
//  FUNCTION:    MMIGMGOLDMINER_RegNv(void)
//  Description :   
//  Global resource dependence : 
//  Author:       
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMGOLDMINER_RegNv(void);

/*****************************************************************************/
//  FUNCTION:      MMIGMGOLDMINER_InitModule
//  Description :   
//  Global resource dependence : 
//  Author:   
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMGOLDMINER_InitModule(void);

/*****************************************************************************/
//  FUNCTION:      reg quintet nenu group
//  Description :   
//  Global resource dependence : 
//  Author:     
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMGOLDMINER_RegMenuGroup(void);

/*****************************************************************************/
//  FUNCTION:     GoldMiner_InitGame
//  Description :   
//  Global resource dependence : 
//  Author:
//  Note:   
/*****************************************************************************/
PUBLIC void GoldMiner_InitGame(void);

/*****************************************************************************/
//  FUNCTION:     MMIGMGOLDMINER_OpenGameWin
//  Description :      
//  Global resource dependence : 
//  Author:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIGMGOLDMINER_OpenGameWin(void);

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_GOLDMINER_RegWinIdNameArr(void);

/*****************************************************************************/
//  FUNCTION:     GoldMiner_SaveGameScore
//  Description :  
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC void GoldMiner_SaveGameScore(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif//_MMIGAME_GOLDMINER_INTERNAL_H_

