/***************************************************************************
** File Name:      mmigame_goldminer_func.c                                *                        
** Author:                                                                 *
** Date:           06/09/2012                                              *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     * 
** Description:    This file is used to create windows of quintet          *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                         *                            
** 06/09/2012                          Create                              *
****************************************************************************/
#define _MMIGAME_GOLDMINER_FUNC_C_  

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_GOLDMINER_SUPPORT
#include "window_parse.h"
#include "mmigame_goldminer_internal.h"
#include "mmigame_goldminer_position.h" 
#include "mmigame_goldminer_text.h"   
#include "mmigame_goldminer_nv.h"
#include "mmigame_goldminer_id.h"
#include "mmigame_goldminer_image.h"
#include "guilistbox.h"
#include "guimsgbox.h"
#include "mmidisplay_data.h" 
#include "guilcd.h"
#include "dal_img.h"
#include "tb_dal.h"
#include "mmi_nv.h"
#include "guiref_scale.h"
#include "mmk_timer.h"
#include "guilcd.h"
#include "mmigame_text.h"
#include "guires.h"
#include "mmipub.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
**                       LOCAL DATA DECLARATION
**---------------------------------------------------------------------------*/
// 当前的分数
LOCAL  uint16       s_goldminer_currentscore = 0; 


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                                                           *
**--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                       function bodies  -- local
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  FUNCTION:      MMIGMGOLDMINER_Init(void)
//  Description :   
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMGOLDMINER_Init(void)
{
    
}

/*****************************************************************************/
//  FUNCTION:      MMIGMGOLDMINER_InitModule
//  Description :   
//  Global resource dependence : 
//  Author:   
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMGOLDMINER_InitModule(void)
{
    //初始化menu
    MMIGMGOLDMINER_RegMenuGroup();  
    //初始化nv
    MMIGMGOLDMINER_RegNv();   
    MMIGAME_GOLDMINER_RegWinIdNameArr();
}




/*****************************************************************************/
//  FUNCTION:     GoldMiner_InitGame
//  Description :   
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC void GoldMiner_InitGame(void)
{

}

/*****************************************************************************/
//  FUNCTION:     GoldMiner_SaveGameScore
//  Description :  
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC void GoldMiner_SaveGameScore(void)
{
    uint16 high_score = 0;
    uint16 nv_return = 0;   
    // 读nv
    MMINV_READ(MMINV_GMGOLDMINER_HIGH_SCORE, &high_score, nv_return);    
    if (MN_RETURN_SUCCESS != nv_return)
    {
        high_score = 0;
        MMINV_WRITE(MMINV_GMGOLDMINER_HIGH_SCORE, &high_score);
    }
    // 如果分数超过最高分，则存nv
    if (s_goldminer_currentscore > high_score)  
    {   
        high_score = s_goldminer_currentscore;  
        MMINV_WRITE(MMINV_GMGOLDMINER_HIGH_SCORE,&high_score);  
    }   
}
#endif

