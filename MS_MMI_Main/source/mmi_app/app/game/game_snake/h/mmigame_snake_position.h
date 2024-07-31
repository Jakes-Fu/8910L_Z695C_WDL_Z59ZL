/****************************************************************************
** File Name:      mmigame_snake_position.h                                               *
** Author:                                                                 *
** Date:           5/10/2011                                             *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 5/10/2011     leon.wang         Create
** 
****************************************************************************/
#ifndef  _MMIGAME_SNAKE_POSITION_H_    
#define  _MMIGAME_SNAKE_POSITION_H_  

 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef GAME_SNAKE_SUPPORT
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmigame_snake_menutable.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
   extern   "C"
   {
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
//关于位置的宏定义
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
#define MMIGAME_SNAKE_LOW_MEMORY  //用于决定是否禁用大背景图片等资源
                                  //可根据具体情况，定义到各个分辨率中
#endif

#ifdef MAINLCD_SIZE_128X160
#ifdef MAINLCD_DEV_SIZE_128X160     //竖屏
		#define SNAKE_START_X       4   //格子的起点x坐标
		#define SNAKE_START_Y       20  //格子的起点y坐标
		#define SNAKE_XGRIDS        20  //每行几个格子
		#define SNAKE_YGRIDS        20  //总共几行
		#define SNAKE_GRID_WIDTH    6   //每个格子的宽度
		#define SNAKE_GRID_HEIGHT   6   //每个格子的高度
		#define SNAKE_SCORE_STRING_RECT       {4, 2, 64, 18}            //分数字串显示
		#define SNAKE_LEVEL_STRING_RECT       {64, 2, 124, 18}    //难度字串显示
#else   //MAINLCD_DEV_SIZE_160X128  //横屏
		#define SNAKE_START_X       11   //格子的起点x坐标
		#define SNAKE_START_Y       16  //格子的起点y坐标
		#define SNAKE_XGRIDS        23  //每行几个格子
		#define SNAKE_YGRIDS        16  //总共几行
		#define SNAKE_GRID_WIDTH    6   //每个格子的宽度
		#define SNAKE_GRID_HEIGHT   6   //每个格子的高度
		#define SNAKE_SCORE_STRING_RECT       {11, 2, 80, 16}            //分数字串显示
		#define SNAKE_LEVEL_STRING_RECT       {80, 2, 146, 16}    //难度字串显示 
#endif      
#elif MAINLCD_SIZE_176X220
#ifdef MAINLCD_DEV_SIZE_176X220     //竖屏
    #define SNAKE_START_X       8   //格子的起点x坐标
    #define SNAKE_START_Y       26  //格子的起点y坐标 
    #define SNAKE_XGRIDS        20  //每行几个格子
    #define SNAKE_YGRIDS        21  //总共几行
    #define SNAKE_GRID_WIDTH    8   //每个格子的宽度
    #define SNAKE_GRID_HEIGHT   8   //每个格子的高度
    #define SNAKE_SCORE_STRING_RECT       {8, 10, 108, 26}          //分数字串显示
    #define SNAKE_LEVEL_STRING_RECT       {78,10, 176-8, 26}   //难度字串显示
#else   //MAINLCD_DEV_SIZE_220X176  //横屏
    #define SNAKE_START_X       18  //格子的起点x坐标
    #define SNAKE_START_Y       24  //格子的起点y坐标
    #define SNAKE_XGRIDS        23  //每行几个格子
    #define SNAKE_YGRIDS        16  //总共几行
    #define SNAKE_GRID_WIDTH    8   //每个格子的宽度
    #define SNAKE_GRID_HEIGHT   8   //每个格子的高度
    #define SNAKE_SCORE_STRING_RECT     {18, 6, 118, 22}            //分数字串显示
    #define SNAKE_LEVEL_STRING_RECT     {152, 6,202, 22}     //难度字串显示
#endif

#elif defined MAINLCD_SIZE_240X400

#define SNAKE_START_X       20  //格子的起点x坐标
#define SNAKE_START_Y       40  //格子的起点y坐标
#define SNAKE_XGRIDS        20  //每行几个格子
#define SNAKE_YGRIDS        30  //总共几行
#define SNAKE_GRID_WIDTH    10  //每个格子的宽度
#define SNAKE_GRID_HEIGHT   10  //每个格子的高度
#define SNAKE_SCORE_STRING_RECT     {20, 22, 20+70, 22+16}      //分数字串显示
#define SNAKE_LEVEL_STRING_RECT     {220-70, 22, 220, 22+16}    //难度字串显示


#elif defined MAINLCD_SIZE_240X320
#ifdef MAINLCD_DEV_SIZE_240X320     //竖屏
    #define SNAKE_START_X       20  //格子的起点x坐标
    #define SNAKE_START_Y       40  //格子的起点y坐标
    #define SNAKE_XGRIDS        20  //每行几个格子
    #define SNAKE_YGRIDS        24  //总共几行
    #define SNAKE_GRID_WIDTH    10  //每个格子的宽度
    #define SNAKE_GRID_HEIGHT   10  //每个格子的高度
    #define SNAKE_SCORE_STRING_RECT     {20, 22, 120, 38}          //分数字串显示
    #define SNAKE_LEVEL_STRING_RECT     {130, 22, 220, 38}     //难度字串显示
#else   //MAINLCD_DEV_SIZE_320X240  //横屏
    #define SNAKE_START_X       30  //格子的起点x坐标
    #define SNAKE_START_Y       40  //格子的起点y坐标
    #define SNAKE_XGRIDS        26  //每行几个格子
    #define SNAKE_YGRIDS        16  //总共几行
    #define SNAKE_GRID_WIDTH    10  //每个格子的宽度
    #define SNAKE_GRID_HEIGHT   10  //每个格子的高度
    #define SNAKE_SCORE_STRING_RECT       {30, 20, 130, 38}     //分数字串显示
    #define SNAKE_LEVEL_STRING_RECT       {220, 20, 290, 38}    //难度字串显示
#endif

#elif defined MAINLCD_SIZE_320X480
    #define SNAKE_START_X       20  //格子的起点x坐标
    #define SNAKE_START_Y       40  //格子的起点y坐标
    #define SNAKE_XGRIDS        20  //每行几个格子
    #define SNAKE_YGRIDS        24  //总共几行
    #define SNAKE_GRID_WIDTH    10  //每个格子的宽度
    #define SNAKE_GRID_HEIGHT   10  //每个格子的高度
    #define SNAKE_SCORE_STRING_RECT     {20, 22, 20+70, 22+16}          //分数字串显示
    #define SNAKE_LEVEL_STRING_RECT     {220-70, 22, 220, 22+16}        //难度字串显示

#else   //  其他分辨率可按需求添加,此处仅用于保证编译通过
#define SNAKE_START_X       8       //格子的起点x坐标
#define SNAKE_START_Y       26      //格子的起点y坐标 
#define SNAKE_XGRIDS        20      //每行几个格子
#define SNAKE_YGRIDS        21      //总共几行
#define SNAKE_GRID_WIDTH    8       //每个格子的宽度
#define SNAKE_GRID_HEIGHT   8       //每个格子的高度
#define SNAKE_SCORE_STRING_RECT     {8, 10, 8+70, 10+16}          //分数字串显示
#define SNAKE_LEVEL_STRING_RECT     {176-8-70, 2, 176-8, 10+16}   //难度字串显示
#endif


#ifdef   __cplusplus
   }
#endif
#endif
#endif// _MMIGAME_SNAKE_POSITION_H_

