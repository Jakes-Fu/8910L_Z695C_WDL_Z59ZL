#ifndef _MMIGAMEBUBBLE_POSITION_H_ /*perl robot add*/
#define _MMIGAMEBUBBLE_POSITION_H_ /*perl robot add*/
/*****************************************************************************
** File Name:      mmigamebubble_position.h                                       *
** Author:                                                                   *
** Date:           13/04/2007                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007     maggie,ren              Creat
******************************************************************************/
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*********************  全局常量  ***************************/
#if defined MAINLCD_SIZE_240X320

#define X16			11
#define X305		228

#define Y86			45

#define MAX_X		240
#define MAX_Y		320

#define RADIUS		12
#define DISY		2
#define DIAMETER	24

#define OFFSETX		120
#define OFFSETY		299

#define SMALL_BUBBLE_X 62
#define SMALL_BUBBLE_Y 298

#define SHOOT_SPEED	25 //16
#define DROP_SPEED	30
#define DEAD_LINE	248

#define SCHEDULE_SPACE_WITH	5
#define PERCENT_SPACE_WITH	6

#define BUBBLE_SPACE	0.2
#endif

#endif
#endif  /*perl robot add*/

