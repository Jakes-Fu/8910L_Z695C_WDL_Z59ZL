/********************************************************************************************
** File Name:    mmigame_common.h                                                           *
** Author:         michael.wang                                                             *
** Date:            6/4/2012                                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.                       *
** Description:    This file is the common part of shooting game framwork                   *
**********************************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME             DESCRIPTION                         *
** 4/6/2012        michael.wang         Create
******************************************************************************/

#ifndef _MMIGAME_COMMON_H_
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


#include "mmigame_goldminer_mode.h"
#include "mmigame_shooter_common.h"
#include "mmigame_goldminer_view.h"
#include "mmigame_goldminer_image.h"
#include "mmigame_goldminer_position.h"
#include "mmigame_goldminer_id.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define GM_COLOR_BLACK      0  //0x20E4
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_WHITE     0xFFFF
#define COLOR_TAN       0xc527
#define COLOR_LIGHTGRAY 0xc618
#define COLOR_YELLOW        0XFFF0


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description :显示 Target 分数
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
 LOCAL void GOLDMINER_DrawTargetScore(uint16 target_score)
{ 
    GUI_RECT_T target_score_rect = MMI_GOLDMINER_TARGETSCORE_HORIZONTAL_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T text_style =  {0};
    MMI_STRING_T target_score_string = {0};
    uint8    str_len = 0;
    char     target_score_char[10] = {0};
    wchar  target_score_wstr[10] = {0};
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128) 
    MMI_STRING_T  result_str_target = {0};
    MMI_STRING_T  temp_str_target = {0};
#endif
    
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_SMALL_FONT;
    text_style.align = ALIGN_LEFT;// ALIGN_HMIDDLE;
    
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)
    MMI_GetLabelTextByLang(TXT_GOLDMINER_VIEW_TARGETSCORE, &temp_str_target);
#endif

    sprintf(target_score_char,"%d ",target_score);
    str_len = strlen(target_score_char);
    MMI_STRNTOWSTR(target_score_wstr, 10, (const uint8*)target_score_char, 10, str_len);
    target_score_string.wstr_ptr = target_score_wstr;
    target_score_string.wstr_len = str_len;
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)
    MMIAPICOM_CatTwoString(&result_str_target, &temp_str_target, &target_score_string);
#endif
    
    GUISTR_DrawTextToLCDInRect( 
                            (const GUI_LCD_DEV_INFO *)(&lcd_dev_info),
                            (const GUI_RECT_T *)&target_score_rect,
                            (const GUI_RECT_T *)&target_score_rect,
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)                
                            (const MMI_STRING_T *)&result_str_target,
#else
                            (const MMI_STRING_T *)&target_score_string,
#endif
                            &text_style,
                            GUISTR_STATE_ALIGN,
                            GUISTR_TEXT_DIR_AUTO
                            );
}

 /*****************************************************************************/
//  Description :显示 cur 分数
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_DrawCurScore(uint16 cur_score)
{ 
    GUI_RECT_T scroe_rect = MMI_GOLDMINER_SCORE_HORIZONTAL_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T text_style =  {0};
    MMI_STRING_T cur_score_string = {0};
    uint8    str_len = 0;
    char     cur_score_char[10] = {0};
    wchar  cur_score_wstr[10] = {0};
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)
    MMI_STRING_T  result_str_current = {0};
    MMI_STRING_T  temp_str_current = {0};
#endif

    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_SMALL_FONT;
    text_style.align = ALIGN_LEFT;// ALIGN_HMIDDLE;

#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128) 
    MMI_GetLabelTextByLang(TXT_GOLDMINER_VIEW_CURRENTSCORE, &temp_str_current);
#endif    

    sprintf(cur_score_char,"%d ",cur_score);
    str_len = strlen(cur_score_char);
    MMI_STRNTOWSTR(cur_score_wstr, 10, (const uint8*)cur_score_char, 10, str_len);
    cur_score_string.wstr_ptr = cur_score_wstr;
    cur_score_string.wstr_len = str_len;
    
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)
    MMIAPICOM_CatTwoString(&result_str_current, &temp_str_current, &cur_score_string);
#endif

    GUISTR_DrawTextToLCDInRect( 
                            (const GUI_LCD_DEV_INFO *)(&lcd_dev_info),
                            (const GUI_RECT_T *)&scroe_rect,
                            (const GUI_RECT_T *)&scroe_rect,
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)                      
                            (const MMI_STRING_T *)&result_str_current,
#else
                            (const MMI_STRING_T *)&cur_score_string,
#endif
                            &text_style,
                            GUISTR_STATE_ALIGN,
                            GUISTR_TEXT_DIR_AUTO
                            );
}

 /*****************************************************************************/
//  Description :显示 倒计时
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_DrawTime(uint32 time)
{ 
    GUI_RECT_T time_rect = MMI_GOLDMINER_TIME_HORIZONTAL_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T text_style =  {0};
    MMI_STRING_T time_string = {0};
    uint8    str_len = 0;
    char     time_char[10] = {0};
    wchar  time_wstr[10] = {0};
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)  
    MMI_STRING_T  result_str_time = {0};
    MMI_STRING_T  temp_str_time = {0};
#endif    

    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_SMALL_FONT;
    text_style.align = ALIGN_RIGHT;// ALIGN_HMIDDLE;

#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)
    MMI_GetLabelTextByLang(TXT_GOLDMINER_VIEW_TIME, &temp_str_time);
#endif    

    sprintf(time_char,"%d ",time);
    str_len = strlen(time_char);
    MMI_STRNTOWSTR(time_wstr, 10, (const uint8*)time_char, 10, str_len);
    time_string.wstr_ptr = time_wstr;
    time_string.wstr_len = str_len;

#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)
    MMIAPICOM_CatTwoString(&result_str_time, &temp_str_time, &time_string);
#endif    

    GUISTR_DrawTextToLCDInRect( 
                            (const GUI_LCD_DEV_INFO *)(&lcd_dev_info),
                            (const GUI_RECT_T *)&time_rect,
                            (const GUI_RECT_T *)&time_rect,
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)                        
                            (const MMI_STRING_T *)&result_str_time,
#else
                            (const MMI_STRING_T *)&time_string,
#endif
                            &text_style,
                            GUISTR_STATE_ALIGN,
                            GUISTR_TEXT_DIR_AUTO
                            );
}

 /*****************************************************************************/
//  Description :显示 level
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_DrawLevel(uint8 level)
{ 
    GUI_RECT_T level_rect = MMI_GOLDMINER_LEVEL_HORIZONTAL_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T text_style =  {0};
    MMI_STRING_T level_string = {0};
    uint8    str_len = 0;
    char     level_char[10] = {0};
    wchar  level_wstr[10] = {0};
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)    
    MMI_STRING_T  result_str_level = {0};
    MMI_STRING_T  temp_str_level = {0};
#endif


    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_SMALL_FONT;
    text_style.align = ALIGN_RIGHT;// ALIGN_HMIDDLE;

#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128) 
    MMI_GetLabelTextByLang(TXT_GOLDMINER_VIEW_LEVEL, &temp_str_level);
#endif

    sprintf(level_char,"%d ",(level+1));
    str_len = strlen(level_char);
    MMI_STRNTOWSTR(level_wstr, 10, (const uint8*)level_char, 10, str_len);
    level_string.wstr_ptr = level_wstr;
    level_string.wstr_len = str_len;

#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128) 
    MMIAPICOM_CatTwoString(&result_str_level, &temp_str_level, &level_string);
#endif

    GUISTR_DrawTextToLCDInRect( 
                            (const GUI_LCD_DEV_INFO *)(&lcd_dev_info),
                            (const GUI_RECT_T *)&level_rect,
                            (const GUI_RECT_T *)&level_rect,
#if (!defined MAINLCD_SIZE_128X160) ||(defined MAINLCD_DEV_SIZE_160X128)                             
                            (const MMI_STRING_T *)&result_str_level,
#else
                            (const MMI_STRING_T *)&level_string,
#endif
                            &text_style,
                            GUISTR_STATE_ALIGN,
                            GUISTR_TEXT_DIR_AUTO
                            );
}

/*****************************************************************************/
//  Description :显示 道具炸弹数量
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_DrawBombNum()
{ 
    GUI_RECT_T bomb_num_rect = MMI_GOLDMINER_BOMB_NUM_HORIZONTAL_RECT;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T text_style =  {0};
    MMI_STRING_T bomb_num_string = {0};
    uint8    str_len = 0;
    char     bomb_num_char[10] = {0};
    wchar  bomb_num_wstr[10] = {0};
    uint8 bomb_num = 0;

    
    bomb_num = MMIGOLDMINER_GetBombNumM();
    
    if(bomb_num > 0)
    {
            text_style.font_color = MMI_WHITE_COLOR;
            text_style.font = MMI_DEFAULT_SMALL_FONT;
            text_style.align = ALIGN_HMIDDLE;// ALIGN_HMIDDLE;

            sprintf(bomb_num_char,"%d ",bomb_num);
            str_len = strlen(bomb_num_char);
            MMI_STRNTOWSTR(bomb_num_wstr, 10, (const uint8*)bomb_num_char, 10, str_len);
            bomb_num_string.wstr_ptr = bomb_num_wstr;
            bomb_num_string.wstr_len = str_len;
            
            //LCD_FillRect(&lcd_dev_info,bomb_num_rect,COLOR_LIGHTGRAY);
            GUIRES_DisplayImg(PNULL, &(bomb_num_rect), PNULL, GOLDMINER_NEW_WIN_ID, IMAGE_GOLDMINER_BOMB, &(lcd_dev_info)); 

            bomb_num_rect.top = bomb_num_rect.top - MMI_GOLDMINER_TOOL_NUM_TOP_OFFSET;
            bomb_num_rect.bottom = bomb_num_rect.bottom - MMI_GOLDMINER_TOOL_NUM_TOP_OFFSET;
            bomb_num_rect.right = bomb_num_rect.right + MMI_GOLDMINER_TOOL_NUM_RIGHT_OFFSET;
            
            GUISTR_DrawTextToLCDInRect( 
                                    (const GUI_LCD_DEV_INFO *)(&lcd_dev_info),
                                    (const GUI_RECT_T *)&bomb_num_rect,
                                    (const GUI_RECT_T *)&bomb_num_rect,
                                    (const MMI_STRING_T *)&bomb_num_string,
                                    &text_style,
                                    GUISTR_STATE_ALIGN,
                                    GUISTR_TEXT_DIR_AUTO
                                    );
    }
}

/*****************************************************************************/
//  Description :显示 道具加速数量
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_DrawEnergyNum()
{ 
    GUI_RECT_T energy_num_rect = MMI_GOLDMINER_ENERGY_NUM_HORIZONTAL_RECT;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T text_style =  {0};
    MMI_STRING_T energy_num_string = {0};
    uint8    str_len = 0;
    char     energy_num_char[10] = {0};
    wchar  energy_num_wstr[10] = {0};
    uint8 energy_num = 0;    
    
    energy_num = MMIGOLDMINER_GetEnergyNumM();
    
    if(energy_num > 0)
    {
            text_style.font_color = MMI_WHITE_COLOR;
            text_style.font = MMI_DEFAULT_SMALL_FONT;
            text_style.align = ALIGN_HMIDDLE;// ALIGN_HMIDDLE;

            sprintf(energy_num_char,"%d ",energy_num);
            str_len = strlen(energy_num_char);
            MMI_STRNTOWSTR(energy_num_wstr, 10, (const uint8*)energy_num_char, 10, str_len);
            energy_num_string.wstr_ptr = energy_num_wstr;
            energy_num_string.wstr_len = str_len;

            //LCD_FillRect(&lcd_dev_info,energy_num_rect,COLOR_GREEN);
            GUIRES_DisplayImg(PNULL, &(energy_num_rect), PNULL, GOLDMINER_NEW_WIN_ID, IMAGE_GOLDMINER_ENERGY, &(lcd_dev_info)); 

            energy_num_rect.top = energy_num_rect.top - MMI_GOLDMINER_TOOL_NUM_TOP_OFFSET;
            energy_num_rect.bottom = energy_num_rect.bottom - MMI_GOLDMINER_TOOL_NUM_TOP_OFFSET;
            energy_num_rect.right = energy_num_rect.right + MMI_GOLDMINER_TOOL_NUM_RIGHT_OFFSET;
            
            GUISTR_DrawTextToLCDInRect( 
                                    (const GUI_LCD_DEV_INFO *)(&lcd_dev_info),
                                    (const GUI_RECT_T *)&energy_num_rect,
                                    (const GUI_RECT_T *)&energy_num_rect,
                                    (const MMI_STRING_T *)&energy_num_string,
                                    &text_style,
                                    GUISTR_STATE_ALIGN,
                                    GUISTR_TEXT_DIR_AUTO
                                    );
    }
}


/*****************************************************************************/
//  Description :display the mining area(Include drawing ore)  
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_DisplayMiningArea(void)
{
    uint32  i = 0;
    uint32  j = 0;
    GUI_RECT_T rect = {0}; 
    MMI_GAME_CELL_T **gm_cell_ptr = PNULL;
    MMI_GAME_GRID_T *gm_grid_ptr = PNULL; 
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_WIN_ID_T    win_id = GOLDMINER_NEW_WIN_ID;
    
    gm_cell_ptr = (MMI_GAME_CELL_T **)MMIGOLDMINER_GetGridCellInfoM();
    gm_grid_ptr = (MMI_GAME_GRID_T *)MMIGOLDMINER_GetGridInfoM();
    
    for(i = 0; i < MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_V; i++)
    {
        for(j = 0; j < MMI_GM_GOLDMINER_GRID_NUM_HORIZONTAL_H; j++)
        {
            switch(gm_cell_ptr[i][j].type)
            {
            case MMIGM_GOLDMINER_CELLTYPE_MINI_GOLD:
                {
                    rect.left = gm_grid_ptr->display_rect.left + (gm_grid_ptr->cell_weight * j);
                    rect.right = rect.left + gm_grid_ptr->cell_weight;
                    rect.top = gm_grid_ptr->display_rect.top + (gm_grid_ptr->cell_height* i);
                    rect.bottom = rect.top +  gm_grid_ptr->cell_height;
                    GUIRES_DisplayImg(PNULL, &(rect), PNULL, win_id, IMAGE_GOLDMINER_MINI_GOLD, &(lcd_dev_info));
                }
                break;

            case MMIGM_GOLDMINER_CELLTYPE_MINI_STONE:
                {
                    rect.left = gm_grid_ptr->display_rect.left + (gm_grid_ptr->cell_weight * j);
                    rect.right = rect.left + gm_grid_ptr->cell_weight;
                    rect.top = gm_grid_ptr->display_rect.top + (gm_grid_ptr->cell_height* i);
                    rect.bottom = rect.top +  gm_grid_ptr->cell_height;
                    GUIRES_DisplayImg(PNULL, &(rect), PNULL, win_id, IMAGE_GOLDMINER_MINI_STONE, &(lcd_dev_info));                    
                }
                break;

            case MMIGM_GOLDMINER_CELLTYPE_DIAMOND:
                {
                    rect.left = gm_grid_ptr->display_rect.left + (gm_grid_ptr->cell_weight * j);
                    rect.right = rect.left + gm_grid_ptr->cell_weight;
                    rect.top = gm_grid_ptr->display_rect.top + (gm_grid_ptr->cell_height* i);
                    rect.bottom = rect.top +  gm_grid_ptr->cell_height;
                    GUIRES_DisplayImg(PNULL, &(rect), PNULL, win_id, IMAGE_GOLDMINER_DIAMOND, &(lcd_dev_info));      
                }
                break;

            case MMIGM_GOLDMINER_CELLTYPE_BOMB:
                {
                    rect.left = gm_grid_ptr->display_rect.left + (gm_grid_ptr->cell_weight * j);
                    rect.right = rect.left + gm_grid_ptr->cell_weight;
                    rect.top = gm_grid_ptr->display_rect.top + (gm_grid_ptr->cell_height* i);
                    rect.bottom = rect.top +  gm_grid_ptr->cell_height;
                    GUIRES_DisplayImg(PNULL, &(rect), PNULL, win_id, IMAGE_GOLDMINER_TOOL, &(lcd_dev_info));                          
                    //LCD_FillRect(&lcd_dev_info,rect,COLOR_RED);
                }
                break;

            case MMIGM_GOLDMINER_CELLTYPE_ENERGY:
                {
                    rect.left = gm_grid_ptr->display_rect.left + (gm_grid_ptr->cell_weight * j);
                    rect.right = rect.left + gm_grid_ptr->cell_weight;
                    rect.top = gm_grid_ptr->display_rect.top + (gm_grid_ptr->cell_height* i);
                    rect.bottom = rect.top +  gm_grid_ptr->cell_height;
                    GUIRES_DisplayImg(PNULL, &(rect), PNULL, win_id, IMAGE_GOLDMINER_TOOL, &(lcd_dev_info));      
                    //LCD_FillRect(&lcd_dev_info,rect,COLOR_RED);
                }
                break;
                
            case MMIGM_GOLDMINER_CELLTYPE_BIG_GOLD:
                {
                    rect.left = gm_grid_ptr->display_rect.left + (gm_grid_ptr->cell_weight * j);
                    rect.right = rect.left + gm_grid_ptr->cell_weight * 2;
                    rect.top = gm_grid_ptr->display_rect.top + (gm_grid_ptr->cell_height* i);
                    rect.bottom = rect.top +  gm_grid_ptr->cell_height * 2;
                    GUIRES_DisplayImg(PNULL, &(rect), PNULL, win_id, IMAGE_GOLDMINER_BIG_GOLD, &(lcd_dev_info));
                }
                break;

            case MMIGM_GOLDMINER_CELLTYPE_BIG_STONE:
                {
                    rect.left = gm_grid_ptr->display_rect.left + (gm_grid_ptr->cell_weight * j);
                    rect.right = rect.left + gm_grid_ptr->cell_weight * 2;
                    rect.top = gm_grid_ptr->display_rect.top + (gm_grid_ptr->cell_height* i);
                    rect.bottom = rect.top +  gm_grid_ptr->cell_height * 2;
                    GUIRES_DisplayImg(PNULL, &(rect), PNULL, win_id, IMAGE_GOLDMINER_BIG_STONE, &(lcd_dev_info));
                }
                break;

            default:
                break;
            }
        }
    }    
}

 
/*****************************************************************************/
//  Description :display miner 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_DisplayMiner()
{
    //定时播放矿工动画，钳子的显示不在该函数内进行
    MMI_GAME_SHOOTER_T *gm_shooter_ptr = MMIGOLDMINER_GetShooterInfoM();
    GUI_POINT_T     shooter_pos = gm_shooter_ptr->shooter_pos;
    MMI_IMAGE_ID_T  image_id = gm_shooter_ptr->image_id; 
    GUI_POINT_T     image_point = {0};
    uint16          image_width = 0;
    uint16          image_height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_WIN_ID_T    win_id = GOLDMINER_NEW_WIN_ID;
    uint8 miner_times = MMIGOLDMINER_GetMinerTimesM();

    GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, win_id);
    image_point.x = shooter_pos.x-image_width/2;
    image_point.y = shooter_pos.y-image_height;
    //====================================
    if(miner_times%2)
    {
        image_id = IMAGE_GOLDMINER_1;
    }
    else
    {
         image_id = IMAGE_GOLDMINER_2;
    }
    //====================================

    GUIRES_DisplayImg(
                      &image_point,
                      PNULL,
                      PNULL,
                      win_id,
                      image_id,
                      &lcd_dev_info
                      );
}


/*****************************************************************************/
//  Description :display clippers 
//  Global resource dependence : 
//  Author: leon.wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_DisplayClippers()
{
    //仅根据状态做显示
    MMI_GAME_ENTITY_STATE_E entity_state =  MMIGOLDMINER_GetClipperStateM();
    GUI_LCD_DEV_INFO        lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_GAME_SHOOTER_T      *gm_shooter_ptr = MMIGOLDMINER_GetShooterInfoM();
    GUI_POINT_T     shooter_pos = gm_shooter_ptr->shooter_pos;
    GUI_POINT_T     object_pos = (gm_shooter_ptr->object).object_pos;
    MMI_IMAGE_ID_T  image_id = (gm_shooter_ptr->object).image_id;  //默认使用钳子
    uint16          image_width = 0;
    uint16          image_height = 0;
    uint16          angle = gm_shooter_ptr->angle; 
    GUI_POINT_T     image_point = {0};
    MMI_WIN_ID_T    win_id = GOLDMINER_NEW_WIN_ID;
    GUI_RECT_T      rect_area = {0};
    GUI_RECT_T      line_rect[4] = {0};
    GUI_POINT_T     point1 = {0};
    GUI_POINT_T     point2 = {0};
    uint32          i = 0;

    //画绳索
    LCD_DrawLine(&lcd_dev_info, shooter_pos.x, shooter_pos.y, object_pos.x, object_pos.y, MMI_BLACK_COLOR);
    
    //if(entity_state!= MMI_GAME_ENTITY_HIT)
    {
        GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, win_id); //此处image_width/image_height可以更换为宏
        image_point.x = object_pos.x-image_width/2;
        image_point.y = object_pos.y-image_height/4; 
               

        rect_area.left = -image_width/2;
        rect_area.right = image_width/2;
        rect_area.top = 0;
        rect_area.bottom = image_height;
        
        line_rect[0].top = 0;
        line_rect[0].left = 0;
        line_rect[0].right = rect_area.left;
        line_rect[0].bottom = image_height/2;

        
        line_rect[1].top = 0;
        line_rect[1].left = 0;
        line_rect[1].right = rect_area.right;
        line_rect[1].bottom = image_height/2;
        
        line_rect[2].top = rect_area.bottom;
        line_rect[2].left = rect_area.right/2;
        line_rect[2].right = rect_area.right;
        line_rect[2].bottom = image_height/2;
       
        line_rect[3].top = rect_area.bottom;
        line_rect[3].left = -rect_area.right/2;
        line_rect[3].right = -rect_area.right;
        line_rect[3].bottom = image_height/2;
        angle = 90 - angle ; //变化角度
        for(i=0; i<4 ; i++)
        {
            point1.x = line_rect[i].left* COS_S(angle) + line_rect[i].top* SIN_S(angle);
            point1.y = line_rect[i].top* COS_S(angle) - line_rect[i].left* SIN_S(angle);
            point2.x = line_rect[i].right* COS_S(angle) + line_rect[i].bottom* SIN_S(angle);
            point2.y = line_rect[i].bottom* COS_S(angle) - line_rect[i].right* SIN_S(angle);

            LCD_DrawLine(&lcd_dev_info, (uint16)(point1.x+object_pos.x), (uint16)point1.y+object_pos.y, (uint16)(point2.x+object_pos.x), (uint16)(point2.y+object_pos.y), MMI_BLACK_COLOR);
        }
    }
    
    //当处于上拉状态时，画被抓物体
    if(MMI_GAME_ENTITY_HIT == entity_state)
    {
        //==============================================================
        image_point.x = object_pos.x + SIN_S(angle)*image_width/2; //此时使用的是clippers的image_width
        image_point.y = object_pos.y + COS_S(angle)*image_height/2;

        image_id = MMIGOLDMINER_GetCatchedObjectImgM();
        GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, win_id);
        image_point.x -= image_width/2;
        image_point.y -= image_height/2;

        GUIRES_DisplayImg(
                      &image_point,
                      PNULL,
                      PNULL,
                      win_id,
                      image_id,
                      &lcd_dev_info
                      );
    }

    //当处于上拉到顶端时时，画替换图片
    if(MMI_GAME_ENTITY_END == entity_state)
    {
        image_id = IMAGE_NULL;  //积分替换图片
        GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, win_id);
        image_point.x = object_pos.x-image_width/2;
        image_point.y = object_pos.y-image_height/2;

        GUIRES_DisplayImg(
                      &image_point,
                      PNULL,
                      PNULL,
                      win_id,
                      image_id,
                      &lcd_dev_info
                      );
    }
}

/*****************************************************************************/
//  Description :display current game info, include score\level\time 
//  Global resource dependence : 
//  Author: Michael.Wang
//  Note: 
/*****************************************************************************/
LOCAL void GOLDMINER_GameDrawPromptArea()
{
    MMI_GAME_INFO_T *gm_info_ptr = PNULL;

    gm_info_ptr = MMIGOLDMINER_GetGameInfoM();
    
    GOLDMINER_DrawTargetScore(gm_info_ptr->score.target_score);
    GOLDMINER_DrawCurScore(gm_info_ptr->score.cur_score);
    GOLDMINER_DrawTime(gm_info_ptr->time);
    GOLDMINER_DrawLevel(gm_info_ptr->level);
    GOLDMINER_DrawBombNum();
    GOLDMINER_DrawEnergyNum();
}

/*****************************************************************************/
//  Description : draw game
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGOLDMINER_GameDrawV()
{   
    GUI_RECT_T rect_fullscreen = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
                 
    rect_fullscreen =  MMITHEME_GetFullScreenRect();    
    LCD_FillRect(&lcd_dev_info, rect_fullscreen, COLOR_TAN);

    GOLDMINER_GameDrawPromptArea();
    GOLDMINER_DisplayMiningArea(); 
    GOLDMINER_DisplayClippers();
    GOLDMINER_DisplayMiner();    
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif //GAME_GOLDMINER_SUPPORT

