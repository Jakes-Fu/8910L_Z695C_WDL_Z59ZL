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
#ifdef GAME_BOXMAN_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmigame_boxman_export.h"

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
#define BOXMAN_MAX_MAP_ROW              9   //��ͼ�������
#define BOXMAN_MAX_MAP_COL              9   //��ͼ�������
#define BOXMAN_MAX_MOVE_STEP            30  //����֮�������ƶ�����
#define BOXMAN_MAX_GO_BACK_STEPS        5   //�����˲���
#define BOXMAN_MAP_NUMS                 15  //�ܵĵ�ͼ����
#define BOXMAN_MAX_MAP_AIM              15  //��ͼ�е����Ŀ�����Ŀ
#define BOXMAN_MAX_SEARCH_STEP          64  //���·����������
#define BOXMAN_MAX_SELECT_MAP_NUM_LEN   2   //���ѡ�����ֳ���

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//�ƶ�����
typedef enum
{
    TOWARDS_LEFT=1, //����
    TOWARDS_RIGHT,  //����
    TOWARDS_UP,     //����
    TOWARDS_DOWN    //����
} BOXMAN_MOVE_DIRECTION_E;
//��ͼ����ֵ
typedef enum
{
    BOXMAN_WALLOUT=0,   //��ʾǽ��
    BOXMAN_SPACE,   //��ʾ�յ�
    BOXMAN_WALL,    //��ʾǽ
    BOXMAN_MAN, //��ʾС��
    BOXMAN_BOX, //��ʾ����
    BOXMAN_AIM, //��ʾĿ��
    BOXMAN_MAN_IN_AIM,  //��ʾС����Ŀ��
    BOXMAN_BOX_IN_AIM,  //��ʾ������Ŀ��
    BOXMAN_SUB_MAP_NUMS //��ͼ����
} BOXMAN_MAP_DATA_VALUE_E;
//һ����ͼ���������ṹ
typedef struct
{
    uint8 row;  //����
    uint8 col;  //����
} BOXMAN_MAP_ROW_AND_COL_T;
//��ͼ�е�һ����Ľṹ
typedef struct
{
    uint16 x;   //���ڵ�ͼ�е�x����ֵ
    uint16 y;   //���ڵ�ͼ�е�y����ֵ
} BOXMAN_POINT_T;//��ͼ���Ͻ�Ϊ����ԭ�㣬ԭ������Ϊ(0,0),���ң�xֵ���ӣ����£�yֵ��С��
//һ�����ӵ��ƶ�����,���ڻ��˶�����
typedef struct
{
    BOXMAN_POINT_T box_before_move; //�ƶ�ǰ���ӵ�λ��
    BOXMAN_MAP_DATA_VALUE_E aim_value_before_move;  //Ŀ��λ�õ��ƶ�ǰ��ͼ����ֵ
    BOXMAN_MAP_DATA_VALUE_E box_value_before_move;  //����λ�õ��ƶ�ǰ��ͼ����ֵ
    BOXMAN_MAP_DATA_VALUE_E man_value_before_move;  //С��λ�õ��ƶ�ǰ��ͼ����ֵ
    BOXMAN_MOVE_DIRECTION_E move_direc; //�ƶ�����
    BOXMAN_MAP_DATA_VALUE_E aim_value_after_move;   //Ŀ��λ�õ��ƶ����ͼ����ֵ
    BOXMAN_MAP_DATA_VALUE_E box_value_after_move;   //����λ�õ��ƶ����ͼ����ֵ
    BOXMAN_MAP_DATA_VALUE_E man_value_after_move;   //С��λ�õ��ƶ����ͼ����ֵ
} BOXMAN_BOX_MOVE_STEP_T;
//���˶��м�¼�ṹ
typedef struct
{
    uint8 then_current_map[BOXMAN_MAX_MAP_ROW][BOXMAN_MAX_MAP_COL]; //��ʱ�ĵ�ͼ����
    BOXMAN_POINT_T then_current_man;    //��ʱ��С�˵�λ��
} BOXMAN_GO_BACK_T;
//�ƶ����нṹ
typedef struct
{
    uint8 move_num; //�����е��ƶ�����
    BOXMAN_POINT_T queue[BOXMAN_MAX_MOVE_STEP]; //�ƶ�����
} MOVE_QUEUE_T;


/*****************************************************************************/
//   FUNCTION:     MMIGAME_AddTwoString
//  Description :   add two string 
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMBOXMAN_AddTwoString(
                MMI_STRING_T first, 
                MMI_STRING_T second
                );
/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMBOXMAN_InitModule(void);
/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMBOXMAN_Init(void);

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIGAME_BOXMAN_RegWinIdNameArr(void);

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register box game module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIGMBOX_RegAppletInfo(void);

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif  

