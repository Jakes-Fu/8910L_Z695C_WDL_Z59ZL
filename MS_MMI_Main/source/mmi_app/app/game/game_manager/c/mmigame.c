/****************************************************************************
** File Name:      game.c                            
** Author:                                                                  
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define game module outside API .
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie.an          Create
** 
****************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_SUPPORT
#define _GAME_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmigame_menutable.h"
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "tb_dal.h"
//#include "mmiacc_winmain.h"
#include "mmigame_link_internal.h"
#include "mmigame_boxman_internal.h" 
#ifdef GAME_BUBBLEBOBBLE_SUPPORT 
#include "mmigame_bubblebobble_internal.h"
#endif
#ifdef GAME_SNAKE_SUPPORT
#include "mmigame_snake_internal.h" 
#endif
#ifdef GAME_TETRIS_SUPPORT
#include "mmigame_square_internal.h" 
#endif
#include "mmialarm_export.h"
#include "mmi_textfun.h"
#include "mmiphone_export.h"
//#include "mmigame_text.h"
//#include "mmigame_link_nv.h"
//#include "mmigame_square_nv.h"
#include "mmigame_internal.h"
//michael wang for5 
#include "mmigame_quintet_internal.h"
//michael wang
#ifdef GAME_GOLDMINER_SUPPORT
#include "mmigame_goldminer_internal.h"
#endif


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                   FUNCTION DEFINITION                                                                               */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description :  GAME_InitModule
//	Global resource dependence : 
//  Author:ycd
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGAME_InitModule(void)
{  
    MMIGAME_RegMenuGroup();
    MMIGAME_RegWinIdNameArr();
    //MMIGMQUT_InitModule();
#ifdef GAME_LINK_SUPPORT
    MMIGMLINK_InitModule();
#endif
#ifdef GAME_BOXMAN_SUPPORT
    MMIGMBOXMAN_InitModule();
#endif

#ifdef GAME_BUBBLEBOBBLE_SUPPORT
    MMIGMBUBBLEBOBBLE_InitModule();
#endif

#ifdef GAME_GOLDMINER_SUPPORT
    MMIGMGOLDMINER_InitModule();
#endif

#ifdef GAME_SNAKE_SUPPORT
    MMIGMSNAKE_InitModule();
#endif

#ifdef GAME_TETRIS_SUPPORT
    MMIGMSQU_InitModule();
#endif

//michael wang for5
#ifdef GAME_QUINTET_SUPPORT
	MMIGMQUT_InitModule();
#endif
}

/*****************************************************************************/
// 	Description :  MMIAPIGAME_InitSetting
//	Global resource dependence : 
//  Author:ycd
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIGAME_InitSetting(void)
{
#ifdef GAME_LINK_SUPPORT
    MMIGMLINK_Init();
#endif
#ifdef GAME_BOXMAN_SUPPORT
    MMIGMBOXMAN_Init();   
#endif
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
    MMIGMBUBBLEBOBBLE_Init();   
#endif
//michael wang for5
#ifdef GAME_QUINTET_SUPPORT
    MMIGMQUT_Init();
#endif
    return TRUE;
}

/*****************************************************************************/
//   FUNCTION:     MMIAPIGAME_ClearScreenColor
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIGAME_ClearScreenColor( uint16 color)
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    LCD_FillRect(&lcd_dev_info, rect, color);
} 

#endif


