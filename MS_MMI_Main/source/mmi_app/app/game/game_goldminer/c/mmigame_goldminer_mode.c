/********************************************************************************************
** File Name:    mmigame_common.h                                                        *
** Author:         michael.wang                                                                     *
** Date:            6/4/2012                                                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is the common part of shooting game framwork *
**********************************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME             DESCRIPTION                               *
** 4/6/2012        michael.wang         Create
******************************************************************************/

#define _MMIGAME_GOLDMINER_MODE_C_
#ifdef GAME_GOLDMINER_SUPPORT

#include "std_header.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmigame_menutable.h"
#include "tb_dal.h"

#include "mmipub.h"
#include "mmigame_quintet_nv.h"
#include "mmialarm_export.h"

#include "mmigame_goldminer_nv.h"

#include "mmigame_internal.h"
#include "mmi_textfun.h"
#include "mmiphone_export.h"
#include "mmigame_id.h"
#include "mmigame_goldminer_text.h"
#include "mmigame_goldminer_view.h"
#include "mmigame_goldminer_mode.h"
#include "mmigame_shooter_common.h"
#include "mmigame_goldminer_image.h"
#include "mmigame_goldminer_position.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GOABLE DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_GAME_INFO_T *s_goldminer_info_ptr = PNULL;  //�ƽ����Ϸ��Ϣ
MMI_GAME_CELL_T **s_gm_cell_ptr = PNULL;    //grid cell info
MMI_GAME_SHOOTER_T *s_gm_shooter_ptr = PNULL;   //��������Ϣ
MMI_GAME_GOLDMINER_TOOLS_T *s_goldminer_tool_info_ptr = PNULL;  //��Ϸ������Ϣ
MMI_GAME_CELL_T s_move_cell_info = {0}; //��������ȡ��object��Ϣ
MMI_GAME_ENTITY_STATE_E s_clipper_state = MMI_GAME_ENTITY_NOT_USE;  //ǯ��״̬

LOCAL uint8  s_goldminer_timer = 0;         //�������ڶ�������ͻ�����Timer
LOCAL uint8  s_goldminer_count_down_timer = 0;      //��Ϸ����ʱTimer
LOCAL uint8  s_goldminer_miner_timer = 0;      //���ƶ�timer
LOCAL uint8  g_miner_count_down_imer_times = 0;
/*
MMI_GAME_ENTITY_NORMAL--->MMI_GAME_ENTITY_READY--->MMI_GAME_ENTITY_MOTION--->MMI_GAME_ENTITY_HIT--->|
                              |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|
*/

//level info of gold miner,use const type
const uint8 s_goldminer_level_info[][MMIGM_GOLDMINER_CELLTYPE_MAX] = {
                                                                                                                               {2,1,2,1,0,0,0}, {2,2,2,2,0,0,0},{3,1,3,1,0,0,0},{3,2,3,2,0,1,0},{3,1,3,3,0,1,1},
                                                                                                                                {3,2,4,2,0,0,1},{3,1,4,3,0,1,0},{4,2,5,3,0,0,1},{4,2,5,3,0,1,0},{3,2,5,3,0,1,1},
                                                                                                                                {3,2,5,4,0,1,0},{3,2,5,4,1,1,1},{4,3,4,3,1,1,1},{4,4,4,4,1,1,1},{4,4,5,5,1,1,1},
                                                                                                                                {4,3,5,3,2,1,1},{4,3,2,4,2,1,1},{4,3,3,3,2,1,1},{4,3,4,3,2,1,1},{4,3,5,4,2,1,1},
                                                                                                                                {5,2,5,3,2,1,1},{5,3,5,3,2,1,1},{5,3,5,4,2,1,1},{5,3,5,4,2,1,1},{5,4,6,4,2,1,1},
                                                                                                                                {6,2,5,3,2,1,1},{6,3,5,4,2,1,1},{6,4,6,3,2,1,1},{6,4,6,4,2,1,1},{6,4,6,5,2,1,1},
                                                                                                                                {7,2,5,2,3,1,1},{7,3,6,3,3,1,1},{7,3,7,4,3,1,1},{7,3,7,5,3,1,1},{7,4,7,5,3,1,1},
                                                                                                                                {8,3,7,3,3,1,1},{8,4,7,4,3,1,1},{8,5,8,5,3,1,1},{8,5,8,7,3,1,1},{8,4,8,7,3,1,1},
                                                                                                                                {9,2,7,2,4,1,1},{9,2,7,3,4,1,1},{9,3,8,4,4,1,1},{9,4,9,6,4,1,1},{9,5,9,8,4,1,1},
                                                                                                                                {10,2,8,3,5,1,1},{10,2,8,5,4,1,1},{10,2,10,5,4,1,1},{10,2,10,6,4,1,1},{10,2,10,7,4,1,1}
                                                                     };


/*****************************************************************************/
//  Description : init param of gold miner
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GOLDMINER_InitParamGM()
{
    uint16 i = 0;
    
    //GoldMiner������Ϣ
    if(PNULL == s_goldminer_tool_info_ptr)
    {
         s_goldminer_tool_info_ptr = SCI_ALLOCA(sizeof(MMI_GAME_GOLDMINER_TOOLS_T));

           if( PNULL == s_goldminer_tool_info_ptr)
            {
                return FALSE;
            }
            else
            {
                SCI_MEMSET(s_goldminer_tool_info_ptr, 0, sizeof(MMI_GAME_GOLDMINER_TOOLS_T));
            }
    }

    //GoldMiner ��Ϸ��Ϣ
    if(PNULL == s_goldminer_info_ptr)
    {
        s_goldminer_info_ptr = SCI_ALLOCA(sizeof(MMI_GAME_INFO_T));
        
        if( PNULL == s_goldminer_info_ptr)
        {
            return FALSE;
        }
        else
        {
            SCI_MEMSET(s_goldminer_info_ptr, 0, sizeof(MMI_GAME_INFO_T));
        }
    }      

    //shooter������Ϣ���˴�Ϊһ����ά����
    if((PNULL == s_gm_cell_ptr))
    {         
        s_gm_cell_ptr = (MMI_GAME_CELL_T **)SCI_ALLOCA(sizeof(MMI_GAME_GRID_T) * MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_V);
        if(PNULL != s_gm_cell_ptr)
        {
            //for(i = 0; i < MMI_GM_GOLDMINER_GRID_NUM_V; i++)
            for(i = 0; i < MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H; i++)
            {    
                s_gm_cell_ptr[i] = (MMI_GAME_CELL_T *)SCI_ALLOCA(sizeof(MMI_GAME_GRID_T) * MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H);
                SCI_MEMSET(s_gm_cell_ptr[i], MMI_GM_GOLDMINER_CELL_INFO_NULL, sizeof(MMI_GAME_GRID_T) * MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H);
            }
        }        
        else
        {
            return FALSE;
        }
    }    
    else    
    {
            //�˴�Ϊ�˵�level ����ʱ�������ٴζ�������г�ʼ�� 
            for(i = 0; i < MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H; i++)
            { 
                SCI_MEMSET(s_gm_cell_ptr[i], MMI_GM_GOLDMINER_CELL_INFO_NULL, sizeof(MMI_GAME_GRID_T) * MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H);
            }
    }

    //GoldMiner ���䲿����Ϣ
    if(PNULL == s_gm_shooter_ptr)
    {
        s_gm_shooter_ptr = SCI_ALLOCA(sizeof(MMI_GAME_SHOOTER_T));
        
        if( PNULL == s_gm_shooter_ptr)
        {
            return FALSE;
        }
    } 
        
    s_clipper_state = MMI_GAME_ENTITY_NORMAL;
    s_move_cell_info.type = MMI_GM_GOLDMINER_CELL_INFO_NULL;    //��ʼ����ǯ��ץס��object type��
        
    //image
    s_gm_shooter_ptr->object.image_id = IMAGE_GOLDMINER_CLIPPERS;
    s_gm_shooter_ptr->image_id = IMAGE_GOLDMINER_1; 
    //line min length
    s_gm_shooter_ptr->object.min_distance = MMI_GM_SHOOTER_LINE_LENGTH;
    s_gm_shooter_ptr->angle = MMI_GM_SHOOTER_DEFAULT_ANGLE; 

}

/*****************************************************************************/
//  Description : init gold miner layout
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_InitLayout()
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //��ʼ��������Ϣ���������Ⱥͱ߽�ֵ
    s_goldminer_info_ptr->grid.cell_height = MMI_GM_GOLDMINER_GRID_INTERVAL;
    s_goldminer_info_ptr->grid.cell_weight = MMI_GM_GOLDMINER_GRID_INTERVAL;
 
    s_goldminer_info_ptr->grid.display_rect.left = (lcd_width - s_goldminer_info_ptr->grid.cell_weight * MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H)/2;
    s_goldminer_info_ptr->grid.display_rect.right = lcd_width - s_goldminer_info_ptr->grid.display_rect.left;
    s_goldminer_info_ptr->grid.display_rect.top = MMI_GM_GOLDMINER_GRID_HORIZONTAL_TOP;  
    s_goldminer_info_ptr->grid.display_rect.bottom = s_goldminer_info_ptr->grid.display_rect.top + s_goldminer_info_ptr->grid.cell_height * MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_V;    

    //goldminer layout 
    s_gm_shooter_ptr->shooter_pos.x = MMI_GM_GOLDMINER_SHOOTER_HORIZONTAL_X;
    s_gm_shooter_ptr->shooter_pos.y = MMI_GM_GOLDMINER_SHOOTER_HORIZONTAL_Y;
    s_gm_shooter_ptr->object.object_pos.x = MMI_GM_GOLDMINER_SHOOTER_HORIZONTAL_X;
    s_gm_shooter_ptr->object.object_pos.y = MMI_GM_GOLDMINER_SHOOTER_HORIZONTAL_Y+MMI_GM_SHOOTER_LINE_LENGTH;
    
}

/*****************************************************************************/
//  Description : ����level ��Ӧ���ܷ�
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL uint16 GOLDMINER_CalcTotalScore(uint8 level)
{
    int i = 0;
    uint16 score;

    score = s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] * MMI_GM_GOLDMINER_MINI_GOLD_SCORE +
                s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD] * MMI_GM_GOLDMINER_BIG_GOLD_SCORE +
                s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE] * MMI_GM_GOLDMINER_MINI_STONE_SCORE +
                s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] * MMI_GM_GOLDMINER_BIG_STONE_SCORE +
                s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_DIAMOND] * MMI_GM_GOLDMINER_DIAMOND_SCORE;
    
    return score;
}

/*****************************************************************************/
//  Description : ��ʼ��GoldMiner ��ʾ��Ϣ����
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: ��������ʱ������Ŀ������͵�ǰ�÷�
/*****************************************************************************/
LOCAL void GOLDMINER_InitPromptAreaInfo()
{
    s_goldminer_info_ptr->time = 60;
    s_goldminer_info_ptr->score.cur_score = 0;
    s_goldminer_info_ptr->score.total_score = GOLDMINER_CalcTotalScore(s_goldminer_info_ptr->level);
    s_goldminer_info_ptr->score.target_score =  7 * (s_goldminer_info_ptr->score.total_score/10);
}

/*****************************************************************************/
//  Description : Adjust: Whether Point is out of the rect range
//  Global resource dependence : 
//  Author: leon.wang
/*****************************************************************************/
LOCAL BOOLEAN GOLDMINER_IsPointOutofRect(GUI_POINT_T point, GUI_RECT_T rect)
{
    BOOLEAN re_code = FALSE;

    if(point.x <= rect.left 
        || point.x >= rect.right 
        || point.y <= rect.top 
        || point.y >= rect.bottom)
    {
        re_code = TRUE;
    }

    return re_code;
}

/*****************************************************************************/
//  Description : stop timer by id
//  Global resource dependence : 
//  Author: michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_StopTimerbyId(uint8  *timer_id) 
{
    if(*timer_id != 0)   
    {   
        MMK_StopTimer(*timer_id);    
        *timer_id = 0;   
    }   
}    

/*****************************************************************************/
//  Description : start timer by id
//  Global resource dependence : 
//  Author: michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_StartTimerById(uint8 *timer_id,uint32 time_out)     
{   
    if(*timer_id != 0)   
    {   
        MMIGOLDMINER_StopTimerM(*timer_id);    
    }   
    *timer_id = MMK_CreateTimer(time_out, TRUE);
}


/*****************************************************************************/
//  Description : cell whether can set value
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GOLDMINER_IsCellCanSetValue(uint16 row,uint16 column,uint8 group_id,uint8 level)
{
    BOOLEAN ret = FALSE;

    if (group_id < s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD])
    {
        //��ͼ��Ҫռ4��cell�����Ե���߽��ʱ����Ҫ�ж�
        if((row == MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_V - 1) ||(column == MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H - 1))
        {
            ret = FALSE;
        }
        else if((MMI_GM_GOLDMINER_CELL_INFO_NULL == s_gm_cell_ptr[row][column].groupid)&&
                (MMI_GM_GOLDMINER_CELL_INFO_NULL == s_gm_cell_ptr[row][column+1].groupid)&&
                (MMI_GM_GOLDMINER_CELL_INFO_NULL == s_gm_cell_ptr[row+1][column].groupid)&&
                (MMI_GM_GOLDMINER_CELL_INFO_NULL == s_gm_cell_ptr[row+1][column+1].groupid)
                )
        {
            ret = TRUE;
        }
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
                        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE]))
    {
        if((row == MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_V - 1) ||(column == MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H - 1))
        {
            ret = FALSE;
        }
        else if((MMI_GM_GOLDMINER_CELL_INFO_NULL == s_gm_cell_ptr[row][column].groupid)&&
            (MMI_GM_GOLDMINER_CELL_INFO_NULL == s_gm_cell_ptr[row][column+1].groupid)&&
            (MMI_GM_GOLDMINER_CELL_INFO_NULL == s_gm_cell_ptr[row+1][column].groupid)&&
            (MMI_GM_GOLDMINER_CELL_INFO_NULL == s_gm_cell_ptr[row+1][column+1].groupid)
            )
        {
            ret = TRUE;
        }
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
            s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
            s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD]))
    {
        if(MMI_GM_GOLDMINER_CELL_INFO_NULL ==s_gm_cell_ptr[row][column].groupid)
        {
            ret = TRUE;
        }
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE]))
    {
        if(MMI_GM_GOLDMINER_CELL_INFO_NULL ==s_gm_cell_ptr[row][column].groupid)
        {
            ret = TRUE;
        }
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_DIAMOND]))
    {        
        if(MMI_GM_GOLDMINER_CELL_INFO_NULL ==s_gm_cell_ptr[row][column].groupid)
        {
            ret = TRUE;
        }
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_DIAMOND] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BOMB]))
    {        
        if(MMI_GM_GOLDMINER_CELL_INFO_NULL ==s_gm_cell_ptr[row][column].groupid)
        {
            ret = TRUE;
        }
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_DIAMOND] +
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BOMB]+
        s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_ENERGY]))
    {        
        if(MMI_GM_GOLDMINER_CELL_INFO_NULL ==s_gm_cell_ptr[row][column].groupid)
        {
            ret = TRUE;
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description : update cell info
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_GridCellUpdate(uint16 row,uint16 column,uint8 group_id,uint8 level)
{
    //�����������й涨����ͼ���λ�ã�Ȼ���ڻ�С��
    if (group_id < s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD])
    {
        s_gm_cell_ptr[row][column].type = MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD;
        s_gm_cell_ptr[row][column].groupid = group_id; 
        s_gm_cell_ptr[row][column+1].groupid = group_id;
        s_gm_cell_ptr[row+1][column].groupid = group_id;
        s_gm_cell_ptr[row+1][column+1].groupid = group_id;
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE]))
    {
        s_gm_cell_ptr[row][column].type = MMIGM_GOLDMINER_CELLTYPE_BIG_STONE;
        s_gm_cell_ptr[row][column].groupid = group_id; 
        s_gm_cell_ptr[row][column+1].groupid = group_id;
        s_gm_cell_ptr[row+1][column].groupid = group_id;
        s_gm_cell_ptr[row+1][column+1].groupid = group_id;
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD]))
    {
        s_gm_cell_ptr[row][column].type = MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD;
        s_gm_cell_ptr[row][column].groupid = group_id;
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE]))
    {
        s_gm_cell_ptr[row][column].type = MMIGM_GOLDMINER_CELLTYPE_MINI_STONE;
        s_gm_cell_ptr[row][column].groupid = group_id;
    }
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_DIAMOND]))
    {        
        s_gm_cell_ptr[row][column].type = MMIGM_GOLDMINER_CELLTYPE_DIAMOND;
        s_gm_cell_ptr[row][column].groupid = group_id;  
    }        
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_DIAMOND] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BOMB]))
    {        
        s_gm_cell_ptr[row][column].type = MMIGM_GOLDMINER_CELLTYPE_BOMB;
        s_gm_cell_ptr[row][column].groupid = group_id;  
    }            
    else if(group_id < (s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD]+
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BIG_STONE] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_MINI_STONE] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_DIAMOND] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_BOMB] +
                                    s_goldminer_level_info[level][MMIGM_GOLDMINER_CELLTYPE_ENERGY]))
    {        
        s_gm_cell_ptr[row][column].type = MMIGM_GOLDMINER_CELLTYPE_ENERGY;
        s_gm_cell_ptr[row][column].groupid = group_id;  
    }        
}

/*****************************************************************************/
//  Description : ����level ����ÿһ����ص�������Ϣ
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINEU_ConfigGridCellInfo(uint8 level)
{
    uint16 *row_array_ptr = PNULL;
    uint16 *column_array_ptr = PNULL;
    uint16 cell_count = 0;
    uint16 row = 0;
    uint16 column = 0;
    uint16 i = 0;
    uint16 j = 0;
    BOOLEAN is_update = FALSE;
    
    //alloc the memory according the level
    for(i = 0; i < MMIGM_GOLDMINER_CELLTYPE_MAX; i++)
    {
        cell_count += s_goldminer_level_info[level][i];
    }
    
    row_array_ptr = (uint16 *)SCI_ALLOCAZ(cell_count * sizeof(uint16));
    column_array_ptr = (uint16 *)SCI_ALLOCAZ(cell_count * sizeof(uint16));
    //alloc end

    for( i = 0; i < cell_count; i ++)
    {            
        is_update = TRUE;
        row = rand() % MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_V; //��ĳ����Χ��ȡ�����
        column = rand() % MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H; 
        
        *(row_array_ptr + i) = row;
        *(column_array_ptr + i) = column;
        //����Ƿ����ظ��������������ظ���������˴η��䣬��i--
        for(j = 0; j < i ; j++)
        {
            if(*(row_array_ptr + j) == row)
            {
                if(*(column_array_ptr + j) == column)
                {
                    is_update = FALSE;
                    break;
                }
            }
        }

        //GOLDMINER_IsCellCanSetValue()�е�i��Ϊgroup_id,��id����ȷ��ÿ�������������Χ��Ϊ���ǯ��ץȡ��׼��
        if(is_update && GOLDMINER_IsCellCanSetValue(row,column,i,level))
        {
            GOLDMINER_GridCellUpdate(row,column,i,level);
        } 
        else
        {
            i--;
        }
    } 
    
    SCI_Free(row_array_ptr);
    SCI_Free(column_array_ptr);
}

/*****************************************************************************/
//  Description : �����ƶ���point�жϵ�ǰ�ƶ����ĸ�������
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_GetCellRowColByPoint(const GUI_POINT_T point, uint16 *row, uint16 *col)
{
    *row = (point.y - MMI_GM_GOLDMINER_GRID_HORIZONTAL_TOP)/(MMI_GM_GOLDMINER_GRID_INTERVAL);
    *col = (point.x - MMI_GM_GOLDMINER_GRID_HORIZONTAL_LEFT)/(MMI_GM_GOLDMINER_GRID_INTERVAL);
}

/*****************************************************************************/
//  Description : get grid info
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_GAME_CELL_T *GOLDMINER_GetMoveCellInfo()
{
    return &s_move_cell_info;
}

/*****************************************************************************/
//  Description : GOLDMINER_IsCellHaveObject
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GOLDMINER_IsCellHaveObject(uint16 row, uint16 col)
{
    if( row<0  
        || row > (MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_V-1) 
        || col < 0 
        || col > (MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H-1))
    {
        return FALSE;
    }

    if(MMI_GM_GOLDMINER_CELL_INFO_NULL!= s_gm_cell_ptr[row][col].groupid)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : ��ץȡ��������Ӿ����м��е���ʱ��s_move_cell_info
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: x:row number  y:column number
/*****************************************************************************/
LOCAL void GOLDMINER_SetCellToCatchedObject(uint16 row, uint16 col) 
{
    uint8 group_id = 0;
    group_id = s_gm_cell_ptr[row][col].groupid;
    
    if((s_gm_cell_ptr[row][col].type == MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD)||
       (s_gm_cell_ptr[row][col].type == MMIGM_GOLDMINER_CELLTYPE_BIG_STONE)||
       ((s_gm_cell_ptr[row][col].type == MMI_GM_GOLDMINER_CELL_INFO_NULL)&&     //big imge������3������
       (s_gm_cell_ptr[row][col].groupid != MMI_GM_GOLDMINER_CELL_INFO_NULL)))
    {
        int i = 0;
        int j = 0;
        //�����object��ռ�������Ϣ�������ƶ���object ����
        for(i = (row - 1); i <= (row + 1); i++)
        {
            for(j = (col - 1); j <= (col + 1); j++)
            {
                if((i < 0) ||(j < 0)||(i > (MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_V - 1))||( j > (MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H)))
                {
                    continue;
                }
                if(group_id == s_gm_cell_ptr[i][j].groupid)
                {
                    if(MMI_GM_GOLDMINER_CELL_INFO_NULL != s_gm_cell_ptr[i][j].type)
                    {
                        s_move_cell_info.type = s_gm_cell_ptr[i][j].type;
                    }
                    s_gm_cell_ptr[i][j].type = MMI_GM_GOLDMINER_CELL_INFO_NULL;
                    s_gm_cell_ptr[i][j].groupid = MMI_GM_GOLDMINER_CELL_INFO_NULL;
                }
            }
        }
    }
    else
    {
        s_move_cell_info.type = s_gm_cell_ptr[row][col].type;
        s_gm_cell_ptr[row][col].type = MMI_GM_GOLDMINER_CELL_INFO_NULL;
        s_gm_cell_ptr[row][col].groupid = MMI_GM_GOLDMINER_CELL_INFO_NULL;
    }   
}

/*****************************************************************************/
//  Description : init gold miner
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_InitGameM(void)
{                   
    
    GOLDMINER_InitParamGM();
    
    GOLDMINER_InitLayout();

    GOLDMINER_InitPromptAreaInfo();
    
    GOLDMINEU_ConfigGridCellInfo(s_goldminer_info_ptr->level);
    
    MMIGOLDMINER_GameDrawV();
}

/*****************************************************************************/
//  Description : init gold miner according to the level
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SetGameInfoByLevelM(uint32 level)
{
    GOLDMINER_InitParamGM();

    s_goldminer_info_ptr->level = level - 1;
    
}

/*****************************************************************************/
//  Description : exit  gold miner
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_ExitGameM(void)
{
    if(s_goldminer_info_ptr != PNULL)
    {
        SCI_FREE(s_goldminer_info_ptr);
        s_goldminer_info_ptr = PNULL;
    }
    
    if(s_gm_cell_ptr != PNULL)
    {
        SCI_FREE(s_gm_cell_ptr);
        s_gm_cell_ptr = PNULL;
    }
    
    if(s_gm_shooter_ptr != PNULL)
    {
        SCI_FREE(s_gm_shooter_ptr);
        s_gm_shooter_ptr = PNULL;
    }
    
    if(s_goldminer_tool_info_ptr != PNULL)
    {
        SCI_FREE(s_goldminer_tool_info_ptr);
        s_goldminer_tool_info_ptr = PNULL;
    }
    
}


/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_StartTimerM(MMIGM_GOLDMINER_TIMER_E timer_type)
{
    MMI_GAME_ENTITY_STATE_E clippers_state = MMIGOLDMINER_GetClipperStateM();

    if(MMIGM_GOLDMINER_TIMER_COUNTED == timer_type)
    {
        GOLDMINER_StartTimerById(&s_goldminer_count_down_timer, MMI_COUNTED_TIME_INTERNAL);
        return;
    }
    
    if(MMI_GAME_ENTITY_READY ==clippers_state) //MMIGM_GOLDMINER_TIMER_SWING
    {
        GOLDMINER_StartTimerById(&s_goldminer_timer, MMI_SWING_TIME_INTERNAL);
    }
    else //��������ʱ��timer��Ҫ������ʱ����s_stretch_timer_internal��һ��һ��
    {
        GOLDMINER_StartTimerById(&s_goldminer_timer, MMI_STRETCH_TIME_INTERNAL);
    }    
}

PUBLIC uint8 MMIGOLDMINER_GetMinerTimesM()
{
   return g_miner_count_down_imer_times;
}
/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_StopTimerM(MMIGM_GOLDMINER_TIMER_E timer_type)
{
    if(MMIGM_GOLDMINER_TIMER_COUNTED == timer_type)
    {
        GOLDMINER_StopTimerbyId(&s_goldminer_count_down_timer);
    }
    else 
    {
        GOLDMINER_StopTimerbyId(&s_goldminer_timer);
    }    
}


/*****************************************************************************/
//  Description : get swing timer
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIGM_GOLDMINER_TIMER_E MMIGOLDMINER_GetTimerTypeM(uint8 timer)
{
   if(s_goldminer_count_down_timer == timer)
   {
   	 return MMIGM_GOLDMINER_TIMER_COUNTED;
   }
   else
   {
   	 return MMIGM_GOLDMINER_TIMER_SWING; //������
   }
}

/*****************************************************************************/
//  Description : set the miner/clippers state
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SetClipperStateM(MMI_GAME_ENTITY_STATE_E state)
{
    s_clipper_state = state;
}

/*****************************************************************************/
//  Description : get the miner/clippers state
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_ENTITY_STATE_E MMIGOLDMINER_GetClipperStateM(void)
{
    return s_clipper_state;
}

/*****************************************************************************/
//  Description : set the game state
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SetGameStateM(MMI_GAME_STATE_E state)
{
    if (PNULL != s_goldminer_info_ptr)
    {
        s_goldminer_info_ptr->status = state;
    }
}

/*****************************************************************************/
//  Description : get the game state
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_STATE_E MMIGOLDMINER_GetGameStateM(void)
{
    if (PNULL == s_goldminer_info_ptr)
    {
        return MMI_GAME_START;  //���ڳ�ʼ��GAME, ����
    }
    return s_goldminer_info_ptr->status;
}

/*****************************************************************************/
//  Description : get current event
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIGOLDMINER_GetCatchedObjectImgM(void)
{
    MMI_GAME_CELL_T *gm_move_cell_ptr = PNULL;
    MMI_IMAGE_ID_T  image_id = 0;
    gm_move_cell_ptr = GOLDMINER_GetMoveCellInfo();

    if(PNULL == gm_move_cell_ptr)
    {
        return IMAGE_NULL;
    }
    switch( gm_move_cell_ptr ->type )
    {
        case MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD:
            image_id = IMAGE_GOLDMINER_BIG_GOLD;
            break;
        case MMIGM_GOLDMINER_CELLTYPE_BIG_STONE:
        
            image_id = IMAGE_GOLDMINER_BIG_STONE;
            break;
        
        case MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD:
            image_id = IMAGE_GOLDMINER_MINI_GOLD;
            break;
        
        case MMIGM_GOLDMINER_CELLTYPE_MINI_STONE:
            image_id = IMAGE_GOLDMINER_MINI_STONE;
            break;
        
        case MMIGM_GOLDMINER_CELLTYPE_DIAMOND:
            image_id = IMAGE_GOLDMINER_DIAMOND;     
            break;
        case MMIGM_GOLDMINER_CELLTYPE_BOMB:
            image_id = IMAGE_GOLDMINER_TOOL;
            break;        
        case MMIGM_GOLDMINER_CELLTYPE_ENERGY:
            image_id = IMAGE_GOLDMINER_TOOL;
            break;
    }

    return image_id;
}

/*****************************************************************************/
//  Description : get current event
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC  MMI_ENTITY_EVENT_E MMIGOLDMINER_GetEventM(void)
{
  //����object_pos������λ��������֮��Ĺ�ϵ���ж��Ƿ��д����¼���OUT_RANGE�¼�
    MMI_ENTITY_EVENT_E event_type = EVENT_NONE;
    MMI_GAME_SHOOTER_T *shooter = MMIGOLDMINER_GetShooterInfoM();
    MMI_GAME_CELL_T  **gm_cell_arr = MMIGOLDMINER_GetGridCellInfoM();
    MMI_GAME_ENTITY_STATE_E clippers_state = MMIGOLDMINER_GetClipperStateM(); 
    GUI_RECT_T         rect = MMI_GM_GOLDMINER_HORIZONTAL_RECT;    
    GUI_POINT_T        point = shooter->object.object_pos;
    
    if(MMI_GAME_ENTITY_MOTION == clippers_state) //|| MMI_GAME_ENTITY_NOCATCH == clippers_state //�����ƶ�����
    {
        MMI_WIN_ID_T    win_id = MMK_GetFocusWinId();
        GUI_POINT_T     check_points[5] = {0};
        MMI_IMAGE_ID_T  image_id = shooter->object.image_id;
        uint16          row_num = 0;
        uint16          col_num = 0;
        uint16          image_width = 0;
        uint16          image_height = 0;
        uint16          i = 0;

        if(GOLDMINER_IsPointOutofRect(point, rect))
        {
            event_type = EVENT_OUTRANGE;
        }
        else
        {
            GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, win_id);
            
            //����5������
            //�м��
            check_points[0].x = point.x;
            check_points[0].y = point.y;
            //���µ�
            check_points[1].x = point.x - image_width/2;
            check_points[1].y = point.y + image_height/2;
            //���µ�
            check_points[2].x = point.x + image_width/2;
            check_points[2].y = point.y + image_height/2;
            //���ϵ�
            check_points[3].x = point.x - image_width/2;
            check_points[3].y = point.y - image_height/2;
            //���ϵ�
            check_points[4].x = point.x + image_width/2;
            check_points[4].y = point.y - image_height/2;

            //�жϼ���
            for(i=0; i<5; i++)
            {
                GOLDMINER_GetCellRowColByPoint(check_points[i], &row_num, &col_num);
                if(GOLDMINER_IsCellHaveObject(row_num, col_num))
                {
                    event_type = EVENT_HITOBJECT;
                    GOLDMINER_SetCellToCatchedObject(row_num, col_num); 
                    break;
                }
            }
        }
    }        
    else 
    {   
        //����󹤾���ӽ�һ������ʱ���ж�Ϊreatchtop
        point.x = ABS(point.x-MMI_GM_GOLDMINER_SHOOTER_HORIZONTAL_X);
        point.y = ABS(point.y-MMI_GM_GOLDMINER_SHOOTER_HORIZONTAL_Y);

        if( point.x<MMI_GM_GOLDMINER_GRID_INTERVAL && point.y<MMI_GM_GOLDMINER_GRID_INTERVAL) 
        {
            event_type = EVENT_REATCHTOP;
        }
    } 

    return event_type;
}

/*****************************************************************************/
//  Description : get shooter info
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_SHOOTER_T *MMIGOLDMINER_GetShooterInfoM()
{
    return s_gm_shooter_ptr;
}

/*****************************************************************************/
//  Description : get cell info
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_CELL_T **MMIGOLDMINER_GetGridCellInfoM()
{
    return s_gm_cell_ptr;
}

/*****************************************************************************/
//  Description : get shooter info
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_INFO_T *MMIGOLDMINER_GetGameInfoM()
{
    return s_goldminer_info_ptr;
}

/*****************************************************************************/
//  Description : get grid info
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_GAME_GRID_T *MMIGOLDMINER_GetGridInfoM()
{
    return &(s_goldminer_info_ptr->grid);
}

/*****************************************************************************/
//  Description : ���õ���ʱ
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: ÿ��timer����һ����
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_UpdateCountDownTimeM()
{
    g_miner_count_down_imer_times++;
    if(100 == g_miner_count_down_imer_times)
    {
        g_miner_count_down_imer_times = 0;
    }
    //��TimerΪ500 ms����Ϊ��ÿ500ms��ʾһ�ο󹤶�����ͼƬ��ÿ����timerΪһ�룬ʵ�ֵ���ʱ����
    if(0 == g_miner_count_down_imer_times%2)
    {
        if(0 != s_goldminer_info_ptr->time)
        {
            s_goldminer_info_ptr->time--;
        }    
    }
    /*
    if(0 != s_goldminer_info_ptr->time)
    {
        s_goldminer_info_ptr->time--;
    }  
    */
}

/*****************************************************************************/
//  Description : ��ȡ����ʱ
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View��������ʾ
/*****************************************************************************/
PUBLIC uint32 MMIGOLDMINER_GetCountDownTimeM()
{
    return  s_goldminer_info_ptr->time;
}


/*****************************************************************************/
//  Description : ��ȡbomb ����
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View��������ʾ
/*****************************************************************************/
PUBLIC uint32 MMIGOLDMINER_GetBombNumM()
{
    return  s_goldminer_tool_info_ptr->bomb_num_cur;
}

/*****************************************************************************/
//  Description : ����bomb ����
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View��������ʾ
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_UpdateBombNumM()
{
    s_goldminer_tool_info_ptr->bomb_num_cur--;
}

/*****************************************************************************/
//  Description : ��ȡenergy ����
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View��������ʾ
/*****************************************************************************/
PUBLIC uint32 MMIGOLDMINER_GetEnergyNumM()
{
    return  s_goldminer_tool_info_ptr->energy_num_cur;
}

/*****************************************************************************/
//  Description : ����energy ����
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View��������ʾ
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_UpdateEnergyNumM()
{
    s_goldminer_tool_info_ptr->energy_num_cur--;
}


/*****************************************************************************/
//  Description : ��ʼ���������object ��Ϣ
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: View��������ʾ
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SetMoveCellNoneM()
{
    s_move_cell_info.type = MMI_GM_GOLDMINER_CELL_INFO_NULL;
}

/*****************************************************************************/
//  Description : �ж�Game�Ƿ�ͨ��
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGOLDMINER_IsGameFinishM()
{
     if(s_goldminer_info_ptr->level < (sizeof(s_goldminer_level_info)/MMIGM_GOLDMINER_CELLTYPE_MAX - 1))
     {
        return FALSE;
     }
     else
     {
        return TRUE;
     }
}

/*****************************************************************************/
//  Description : ͨ�غ�������
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_ReplayWithGameFinishM()
{
       s_goldminer_info_ptr->level = 0;
       s_goldminer_tool_info_ptr->bomb_num_last = s_goldminer_tool_info_ptr->bomb_num_cur;
       s_goldminer_tool_info_ptr->energy_num_last = s_goldminer_tool_info_ptr->energy_num_cur;
       GOLDMINER_InitParamGM();
       GOLDMINER_InitPromptAreaInfo();
       GOLDMINEU_ConfigGridCellInfo(s_goldminer_info_ptr->level);            
}

/*****************************************************************************/
//  Description : �ж��Ƿ���Խ�����һ��
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: �Ƿ����������ɲ�������
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_GoToNextLevelM(BOOLEAN is_next_level)
{
    //�˴���Ҫ��һ���������level �Ĵ���
    /*
        ++++++++++++++++++++++++++++++++++++++
    */
    
    if(is_next_level)
    {     
        s_goldminer_info_ptr->level++;
        //��������£����µ��ߵ���Ϣ   
        s_goldminer_tool_info_ptr->bomb_num_last = s_goldminer_tool_info_ptr->bomb_num_cur;
        s_goldminer_tool_info_ptr->energy_num_last = s_goldminer_tool_info_ptr->energy_num_cur;
    }
    else
    {
        //����ʧ�ܣ�������������˹صĳ�ʼ״̬
        s_goldminer_tool_info_ptr->bomb_num_cur = s_goldminer_tool_info_ptr->bomb_num_last;
        s_goldminer_tool_info_ptr->energy_num_cur = s_goldminer_tool_info_ptr->energy_num_last;
    }

    GOLDMINER_InitParamGM();
    GOLDMINER_InitPromptAreaInfo();
    GOLDMINEU_ConfigGridCellInfo(s_goldminer_info_ptr->level);    
}


/*****************************************************************************/
//  Description : ���㵱ǰ�÷�
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/    
PUBLIC void MMIGOLDMINER_UpdateCurScoreAndToolsM()
{
    uint16 value = 0;
    
    switch(s_move_cell_info.type)
    {
    case MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD:
        {
            value = MMI_GM_GOLDMINER_MINI_GOLD_SCORE;
        }
        break;
        
    case MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD:
        {
            value = MMI_GM_GOLDMINER_BIG_GOLD_SCORE;
        }
        break;
        
    case MMIGM_GOLDMINER_CELLTYPE_MINI_STONE:
        {
            value = MMI_GM_GOLDMINER_MINI_STONE_SCORE;
        }
        break;
        
    case MMIGM_GOLDMINER_CELLTYPE_BIG_STONE:
        {
            value = MMI_GM_GOLDMINER_BIG_STONE_SCORE;
        }
        break;
        
    case MMIGM_GOLDMINER_CELLTYPE_DIAMOND:
        {
            value = MMI_GM_GOLDMINER_DIAMOND_SCORE;
        }
        break;        
    case MMIGM_GOLDMINER_CELLTYPE_BOMB:
        {
            s_goldminer_tool_info_ptr->bomb_num_cur++;
        }
        break;    
        
    case MMIGM_GOLDMINER_CELLTYPE_ENERGY:
        {
            s_goldminer_tool_info_ptr->energy_num_cur++;
        }
        break;
    }

    s_goldminer_info_ptr->score.cur_score += value;
}

/*****************************************************************************/
//  Description : ��⵱ǰlevel �Ƿ�ﵽ�������
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/    
PUBLIC BOOLEAN MMIGOLDMINER_CheckLevelFinishEnableM()
{
    BOOLEAN         ret =  FALSE;

    if((s_goldminer_info_ptr->score.cur_score  > s_goldminer_info_ptr->score.target_score)||(s_goldminer_info_ptr->score.cur_score  == s_goldminer_info_ptr->score.target_score))
    {   
        ret = TRUE;
    }
    return ret;    
}

/*****************************************************************************/
//  Description : swing clippers
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_SwingM(void)
{
    MMI_GAME_SHOOTER_T *shooter = MMIGOLDMINER_GetShooterInfoM();
    static BOOLEAN lefttoright = TRUE;
    int32   line_len = shooter->object.min_distance;
    uint16  angle = 0;
    int32   len_internal = 0;

    if(lefttoright)
    {
        if(MMI_SWING_ANGLE_BOUNDARY_LEFT < shooter->angle)
        {
            shooter->angle -= MMI_SWING_ANGLE_INTERNAL;
        }
        else
        {
            lefttoright = FALSE;
            shooter->angle += MMI_SWING_ANGLE_INTERNAL;
        }
    }
    else
    {
        if(MMI_SWING_ANGLE_BOUNDARY_RIGHT > shooter->angle)
        {
            shooter->angle += MMI_SWING_ANGLE_INTERNAL;
        }
        else
        {
            lefttoright = TRUE;
            shooter->angle -= MMI_SWING_ANGLE_INTERNAL;
        }
    }
    
    //����ץǯλ��
    angle= shooter->angle;
    len_internal = line_len*COS_S(angle);
    shooter->object.object_pos.x= shooter->shooter_pos.x+len_internal;
    len_internal = line_len*SIN_S(angle);
    shooter->object.object_pos.y= shooter->shooter_pos.y+len_internal;
}


/*****************************************************************************/
//  Description : stretch clipper
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_ClippersMoveM(
                               BOOLEAN is_down,
                               BOOLEAN is_with_target
                               )
{
    MMI_GAME_SHOOTER_T *shooter = MMIGOLDMINER_GetShooterInfoM();
    //GUI_RECT_T         rect = MMI_GM_GOLDMINER_RECT;
    GUI_RECT_T         rect = MMI_GM_GOLDMINER_HORIZONTAL_RECT;
    GUI_POINT_T        point = shooter->object.object_pos;
    int32  line_len = MMI_GM_GOLDMINER_SPEED;
    uint16  angle = shooter->angle;
    int32  x_internal  = 0;
    int32  y_internal = 0;
    static float x_internal_left = 0; //������ʾ������С������
    static float y_internal_left = 0;
    static BOOLEAN is_down_state = TRUE;
    int32  sig = 1;

    //���ݹ�ס����������͵���ʹ���������ÿ���ƶ��ľ���Modify by @Michael Wang
    if(MMI_GAME_ENTITY_HIT ==MMIGOLDMINER_GetClipperStateM())
    {        
        if(MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD == s_move_cell_info.type ||
           MMIGM_GOLDMINER_CELLTYPE_BIG_STONE == s_move_cell_info.type)
            {
                 line_len = MMI_GM_GOLDMINER_SPEED_LOW;
            }
        
        if( s_goldminer_tool_info_ptr->tool_speed_up)
        {
            line_len = MMI_GM_GOLDMINER_SPEED_HIGH;
        }
    }
    //Modify over

    if(is_down)
    {
        sig = 1;
    }
    else
    {
        sig =-1;
    }
    //����״̬�仯ʱ��ʣ���С���������
    if(is_down_state != is_down)
    {
        is_down_state = is_down;
        x_internal_left = 0;
        y_internal_left = 0;
    }

    x_internal = sig*line_len*COS_S(angle);
    y_internal = sig*line_len*SIN_S(angle);

    //��ÿ�β�����ʾ������С�������ۼ�
    x_internal_left += sig*line_len*COS_S(angle) - x_internal;
    y_internal_left += sig*line_len*SIN_S(angle) - y_internal;
    
    if(ABS(x_internal_left) > 1)
    {
        x_internal_left += ((x_internal_left >0)?-1:1);
        x_internal += ((x_internal_left >0)? 1:-1);
    }
    if(ABS(y_internal_left) > 1)
    {
        y_internal_left += ((y_internal_left >0)?-1:1);
        y_internal += ((y_internal_left >0)? 1:-1);
    }

    point.x= shooter->object.object_pos.x+ x_internal;
    point.y= shooter->object.object_pos.y+ y_internal;
    
    //�ж��Ƿ�Խ�磬�������Խ�磬����Ϊ�߽�ֵ
    if(is_down && GOLDMINER_IsPointOutofRect(point, rect))
    {
        int32  x_temp = 0;
        int32  y_temp = 0;
        if(point.x < rect.left)
        {
            x_temp = (rect.left-shooter->object.object_pos.x);
            y_temp = (x_temp*y_internal/x_internal);
            point.x= shooter->object.object_pos.x+ x_temp;
            point.y= shooter->object.object_pos.y+ y_temp;
        }
        else if(point.x > rect.right)
        {
            x_temp = (rect.right-shooter->object.object_pos.x);
            y_temp = (x_temp*y_internal/x_internal);
            point.x= shooter->object.object_pos.x+ x_temp;
            point.y= shooter->object.object_pos.y+ y_temp;
        }
    }

    shooter->object.object_pos.x = point.x;
    shooter->object.object_pos.y = point.y;
}


/*****************************************************************************/
//  Description : ����object �ƶ������Ƿ�ʹ��
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: "����ҩˮ"
/*****************************************************************************/    
PUBLIC void MMIGOLDMINER_SetClippersMoveWithEnergyM(BOOLEAN is_speed_up)
{
    s_goldminer_tool_info_ptr->tool_speed_up = is_speed_up;
}

/*****************************************************************************/
//  Description : ��ȡobject �ƶ������Ƿ�ʹ��
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: "����ҩˮ"
/*****************************************************************************/   
PUBLIC BOOLEAN MMIGOLDMINER_GetClippersSpecMoveM()
{
    return s_goldminer_tool_info_ptr->tool_speed_up;
}

/*****************************************************************************/
//  Description : ����GameRecord info
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: Highe level and time
/*****************************************************************************/   
PUBLIC void MMIGOLDMINER_UpdateRecordM()
{    
    uint16              nv_return       = 0;
    MMI_GAME_RECORD_T   game_record = {0};

    MMINV_READ(MMINV_GMGOLDMINER_HIGH_SCORE, &game_record, nv_return);

    if((s_goldminer_info_ptr->level + 1) > game_record.high_level)
    {
        game_record.high_level = s_goldminer_info_ptr->level + 1;
        game_record.time = 60 - s_goldminer_info_ptr->time;
        MMINV_WRITE(MMINV_GMGOLDMINER_HIGH_SCORE,&game_record);
    }
    else if((s_goldminer_info_ptr->level + 1) == game_record.high_level)
    {
        if((60 -s_goldminer_info_ptr->time) < game_record.time)
        {
            game_record.high_level = s_goldminer_info_ptr->level + 1;
            game_record.time = 60 - s_goldminer_info_ptr->time;
            MMINV_WRITE(MMINV_GMGOLDMINER_HIGH_SCORE,&game_record);
        }
    }
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#endif //GAME_GOLDMINER_SUPPORT

