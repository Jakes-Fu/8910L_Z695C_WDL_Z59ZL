/*****************************************************************************
** File Name:      mmigame_quintet_internal.h                                                   *
** Author:                                                                   *
** Date:           04/03/2006                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the internal api of game              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/03/2006      Annie.an      Create
******************************************************************************/

#ifndef _MMIGAME_QUINTET_INTERNAL_H_
#define _MMIGAME_QUINTET_INTERNAL_H_
#ifdef GAME_QUINTET_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmidisplay_data.h"




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
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define  BOARD_LINE_NUMBER		15

//For 320X240!此处用dev 控制，因为main lcd 仍为240X320,而且此分辨率
//默认的是landscape，所以注意横竖分辨率的变化.
#ifdef MAINLCD_DEV_SIZE_320X240
#define  LINE_INTERVAL			15
#define  TOP_LINE_POS_X		15 // (屏幕宽度 - 棋盘宽度(14 * 间距))/2
#define  TOP_LINE_POS_Y		60
#define  BOTTOM_LINE_POS_Y	270
#define  POINT_VALID_OFFSET	4
#define  QUENTET_INFO_RECT         {20,280,150,320}
#define  CURSOR_RADIUS                 ((LINE_INTERVAL - 1)/2)
#define  CIRCLE_R				6
//for horniz
#define  TOP_LINE_POS_H_X		15
#define  TOP_LINE_POS_H_Y		15
#define  BOTTOM_LINE_POS_H_Y	225
#define  QUENTET_INFO_H_RECT_LEFT           230
#define  QUENTET_INFO_H_RECT_TOP    	30
#define  QUENTET_INFO_H_RECT_RIGHT 	320
#define  QUENTET_INFO_H_RECT_BOTTOM   60

//michael wang for5 320_480
#elif defined MAINLCD_SIZE_320X480
#define  LINE_INTERVAL			20
#define  TOP_LINE_POS_X		20 // (屏幕宽度 - 棋盘宽度(14 * 间距))/2
#define  TOP_LINE_POS_Y		110
#define  BOTTOM_LINE_POS_Y	390
#define  POINT_VALID_OFFSET	5
#define  BOARD_VALID_RECT		{15,105,305,395}
#define  QUENTET_INFO_RECT         { 25,425, 200, 450}
#define  CURSOR_RADIUS                 (LINE_INTERVAL/2)
#define  CIRCLE_R				8
//for horniz
#define  TOP_LINE_POS_H_X		20
#define  TOP_LINE_POS_H_Y		30
#define  BOTTOM_LINE_POS_H_Y	310
#define  BOARD_VALID_H_RECT_LEFT		15
#define  BOARD_VALID_H_RECT_TOP		25
#define  BOARD_VALID_H_RECT_RIGHT		305
#define  BOARD_VALID_H_RECT_BOTTOM	315
#define  QUENTET_INFO_H_RECT_LEFT   	320
#define  QUENTET_INFO_H_RECT_TOP    	50
#define  QUENTET_INFO_H_RECT_RIGHT 	480
#define  QUENTET_INFO_H_RECT_BOTTOM    100

#elif defined MAINLCD_SIZE_240X400
#define  LINE_INTERVAL			15
#define  TOP_LINE_POS_X		15 // (屏幕宽度 - 棋盘宽度(14 * 间距))/2
#define  TOP_LINE_POS_Y		100
#define  BOTTOM_LINE_POS_Y	310
#define  POINT_VALID_OFFSET	4
#define  BOARD_VALID_RECT		{11,96,229,314}
#define  QUENTET_INFO_RECT         {20,340, 200, 380}
#define  CURSOR_RADIUS                 ((LINE_INTERVAL - 1)/2)
#define  CIRCLE_R				5
//for horniz
#define  TOP_LINE_POS_H_X		30
#define  TOP_LINE_POS_H_Y		25
#define  BOTTOM_LINE_POS_H_Y	235
#define  BOARD_VALID_H_RECT_LEFT		26
#define  BOARD_VALID_H_RECT_TOP		21
#define  BOARD_VALID_H_RECT_RIGHT		244
#define  BOARD_VALID_H_RECT_BOTTOM	400
#define  QUENTET_INFO_H_RECT_LEFT   	250
#define  QUENTET_INFO_H_RECT_TOP    	30
#define  QUENTET_INFO_H_RECT_RIGHT 	400
#define  QUENTET_INFO_H_RECT_BOTTOM    50

#elif defined MAINLCD_SIZE_240X320
#define  LINE_INTERVAL			15
#define  TOP_LINE_POS_X		15 // (屏幕宽度 - 棋盘宽度(14 * 间距))/2
#define  TOP_LINE_POS_Y		60
#define  BOTTOM_LINE_POS_Y	270
#define  POINT_VALID_OFFSET	4
#define  BOARD_VALID_RECT		{11,56,229,274}
#define  QUENTET_INFO_RECT         { 20,280, 190, 310}
#define  CURSOR_RADIUS                 ((LINE_INTERVAL - 1)/2)
#define  CIRCLE_R				6
//for horniz
#define  TOP_LINE_POS_H_X		30
#define  TOP_LINE_POS_H_Y		25
#define  BOTTOM_LINE_POS_H_Y	235
#define  BOARD_VALID_H_RECT_LEFT		26
#define  BOARD_VALID_H_RECT_TOP		21
#define  BOARD_VALID_H_RECT_RIGHT		244
#define  BOARD_VALID_H_RECT_BOTTOM	320
#define  QUENTET_INFO_H_RECT_LEFT   	250
#define  QUENTET_INFO_H_RECT_TOP    	30
#define  QUENTET_INFO_H_RECT_RIGHT 	320
#define  QUENTET_INFO_H_RECT_BOTTOM    50
#endif
//michael wang

#define  STONE_BKW                      11      // stone background width
#define  STONE_BKB                      121      // STONE BacKground Buffer

typedef enum{
        STONE_NULL,
        STONE_WHITE,
        STONE_BLACK
}QUINTET_STONE_T;



#define GM_COLOR_BLACK		0  //0x20E4
#define COLOR_RED		0xF800
#define COLOR_GREEN		0x07E0
#define COLOR_BLUE		0x001F
#define COLOR_WHITE		0xFFFF
#define COLOR_TAN		0xc527
#define COLOR_LIGHTGRAY	0xc618
#define COLOR_YELLOW        0XFFF0
/*
#define COLOR_RUSSIA_BACK   0xcebe
#define COLOR_RUSSIA_BLOCK  0x8000  //0xc618      //  18c6 ?
#define COLOR_RUSSIA_BLOCK_LINE   0x8410  // 1084
*/
#define COLOR_QUINTET_LINE   0x04
#define COLOR_QUINTET_CURSOR  COLOR_GREEN

#define GAME_TIME_DUR  s_timer_level[s_game_level]
/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_Init(void)
// 	Description :   
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMQUT_Init(void);

/*****************************************************************************/
//   FUNCTION:    MMIGMQUT_RegNv(void)
// 	Description :   
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMQUT_RegNv(void);

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
// 	Description :   
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMQUT_InitModule(void);

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_Get_Renstatus
// 	Description :   
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIGMQUT_Get_Renstatus(void);

/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_ResumeQuintet
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIGMQUT_ResumeQuintet(void);

/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_DisplayLastView
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIGMQUT_DisplayLastView(void);

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_StartQuintet   ( 五子棋 )
// 	Description :   game start
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIGMQUT_StartQuintet(void);

/*****************************************************************************/
//   FUNCTION:      reg quintet nenu group
// 	Description :   
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMQUT_RegMenuGroup(void);

/*****************************************************************************/
// 	Description : MMIGMQUT_OpenGameWin
//       
//	Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIGMQUT_OpenGameWin(void);

/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_ProcessQuintetKeyWinMsg
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC  MMI_RESULT_E MMIGMQUT_ProcessQuintetKeyWinMsg(
													MMI_WIN_ID_T win_id,
													MMI_MESSAGE_ID_E  msg_id
													);

/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_SetQuintetHand
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIGMQUT_SetQuintetHand( uint8 chand);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  
#endif

