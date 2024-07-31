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
//����λ�õĺ궨��
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
#define MMIGAME_SNAKE_LOW_MEMORY  //���ھ����Ƿ���ô󱳾�ͼƬ����Դ
                                  //�ɸ��ݾ�����������嵽�����ֱ�����
#endif

#ifdef MAINLCD_SIZE_128X160
#ifdef MAINLCD_DEV_SIZE_128X160     //����
		#define SNAKE_START_X       4   //���ӵ����x����
		#define SNAKE_START_Y       20  //���ӵ����y����
		#define SNAKE_XGRIDS        20  //ÿ�м�������
		#define SNAKE_YGRIDS        20  //�ܹ�����
		#define SNAKE_GRID_WIDTH    6   //ÿ�����ӵĿ��
		#define SNAKE_GRID_HEIGHT   6   //ÿ�����ӵĸ߶�
		#define SNAKE_SCORE_STRING_RECT       {4, 2, 64, 18}            //�����ִ���ʾ
		#define SNAKE_LEVEL_STRING_RECT       {64, 2, 124, 18}    //�Ѷ��ִ���ʾ
#else   //MAINLCD_DEV_SIZE_160X128  //����
		#define SNAKE_START_X       11   //���ӵ����x����
		#define SNAKE_START_Y       16  //���ӵ����y����
		#define SNAKE_XGRIDS        23  //ÿ�м�������
		#define SNAKE_YGRIDS        16  //�ܹ�����
		#define SNAKE_GRID_WIDTH    6   //ÿ�����ӵĿ��
		#define SNAKE_GRID_HEIGHT   6   //ÿ�����ӵĸ߶�
		#define SNAKE_SCORE_STRING_RECT       {11, 2, 80, 16}            //�����ִ���ʾ
		#define SNAKE_LEVEL_STRING_RECT       {80, 2, 146, 16}    //�Ѷ��ִ���ʾ 
#endif      
#elif MAINLCD_SIZE_176X220
#ifdef MAINLCD_DEV_SIZE_176X220     //����
    #define SNAKE_START_X       8   //���ӵ����x����
    #define SNAKE_START_Y       26  //���ӵ����y���� 
    #define SNAKE_XGRIDS        20  //ÿ�м�������
    #define SNAKE_YGRIDS        21  //�ܹ�����
    #define SNAKE_GRID_WIDTH    8   //ÿ�����ӵĿ��
    #define SNAKE_GRID_HEIGHT   8   //ÿ�����ӵĸ߶�
    #define SNAKE_SCORE_STRING_RECT       {8, 10, 108, 26}          //�����ִ���ʾ
    #define SNAKE_LEVEL_STRING_RECT       {78,10, 176-8, 26}   //�Ѷ��ִ���ʾ
#else   //MAINLCD_DEV_SIZE_220X176  //����
    #define SNAKE_START_X       18  //���ӵ����x����
    #define SNAKE_START_Y       24  //���ӵ����y����
    #define SNAKE_XGRIDS        23  //ÿ�м�������
    #define SNAKE_YGRIDS        16  //�ܹ�����
    #define SNAKE_GRID_WIDTH    8   //ÿ�����ӵĿ��
    #define SNAKE_GRID_HEIGHT   8   //ÿ�����ӵĸ߶�
    #define SNAKE_SCORE_STRING_RECT     {18, 6, 118, 22}            //�����ִ���ʾ
    #define SNAKE_LEVEL_STRING_RECT     {152, 6,202, 22}     //�Ѷ��ִ���ʾ
#endif

#elif defined MAINLCD_SIZE_240X400

#define SNAKE_START_X       20  //���ӵ����x����
#define SNAKE_START_Y       40  //���ӵ����y����
#define SNAKE_XGRIDS        20  //ÿ�м�������
#define SNAKE_YGRIDS        30  //�ܹ�����
#define SNAKE_GRID_WIDTH    10  //ÿ�����ӵĿ��
#define SNAKE_GRID_HEIGHT   10  //ÿ�����ӵĸ߶�
#define SNAKE_SCORE_STRING_RECT     {20, 22, 20+70, 22+16}      //�����ִ���ʾ
#define SNAKE_LEVEL_STRING_RECT     {220-70, 22, 220, 22+16}    //�Ѷ��ִ���ʾ


#elif defined MAINLCD_SIZE_240X320
#ifdef MAINLCD_DEV_SIZE_240X320     //����
    #define SNAKE_START_X       20  //���ӵ����x����
    #define SNAKE_START_Y       40  //���ӵ����y����
    #define SNAKE_XGRIDS        20  //ÿ�м�������
    #define SNAKE_YGRIDS        24  //�ܹ�����
    #define SNAKE_GRID_WIDTH    10  //ÿ�����ӵĿ��
    #define SNAKE_GRID_HEIGHT   10  //ÿ�����ӵĸ߶�
    #define SNAKE_SCORE_STRING_RECT     {20, 22, 120, 38}          //�����ִ���ʾ
    #define SNAKE_LEVEL_STRING_RECT     {130, 22, 220, 38}     //�Ѷ��ִ���ʾ
#else   //MAINLCD_DEV_SIZE_320X240  //����
    #define SNAKE_START_X       30  //���ӵ����x����
    #define SNAKE_START_Y       40  //���ӵ����y����
    #define SNAKE_XGRIDS        26  //ÿ�м�������
    #define SNAKE_YGRIDS        16  //�ܹ�����
    #define SNAKE_GRID_WIDTH    10  //ÿ�����ӵĿ��
    #define SNAKE_GRID_HEIGHT   10  //ÿ�����ӵĸ߶�
    #define SNAKE_SCORE_STRING_RECT       {30, 20, 130, 38}     //�����ִ���ʾ
    #define SNAKE_LEVEL_STRING_RECT       {220, 20, 290, 38}    //�Ѷ��ִ���ʾ
#endif

#elif defined MAINLCD_SIZE_320X480
    #define SNAKE_START_X       20  //���ӵ����x����
    #define SNAKE_START_Y       40  //���ӵ����y����
    #define SNAKE_XGRIDS        20  //ÿ�м�������
    #define SNAKE_YGRIDS        24  //�ܹ�����
    #define SNAKE_GRID_WIDTH    10  //ÿ�����ӵĿ��
    #define SNAKE_GRID_HEIGHT   10  //ÿ�����ӵĸ߶�
    #define SNAKE_SCORE_STRING_RECT     {20, 22, 20+70, 22+16}          //�����ִ���ʾ
    #define SNAKE_LEVEL_STRING_RECT     {220-70, 22, 220, 22+16}        //�Ѷ��ִ���ʾ

#else   //  �����ֱ��ʿɰ��������,�˴������ڱ�֤����ͨ��
#define SNAKE_START_X       8       //���ӵ����x����
#define SNAKE_START_Y       26      //���ӵ����y���� 
#define SNAKE_XGRIDS        20      //ÿ�м�������
#define SNAKE_YGRIDS        21      //�ܹ�����
#define SNAKE_GRID_WIDTH    8       //ÿ�����ӵĿ��
#define SNAKE_GRID_HEIGHT   8       //ÿ�����ӵĸ߶�
#define SNAKE_SCORE_STRING_RECT     {8, 10, 8+70, 10+16}          //�����ִ���ʾ
#define SNAKE_LEVEL_STRING_RECT     {176-8-70, 2, 176-8, 10+16}   //�Ѷ��ִ���ʾ
#endif


#ifdef   __cplusplus
   }
#endif
#endif
#endif// _MMIGAME_SNAKE_POSITION_H_

