
/****************************************************************************
** File Name:      mmigame_quint_func.c                            
** Author:                                                                  
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of quintet
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie.an          Create
** 
****************************************************************************/
#define _MMIGAME_QUINTET_FUNC_C_  
#ifdef GAME_QUINTET_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "window_parse.h"
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmigame_quintet_text.h"   
#include "mmigame_quintet_internal.h"
#include "mmigame_quintet_nv.h"
#include "guilistbox.h"
#include "guimsgbox.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiacc_winmain.h" 
//michael wang for5
#include "mmipub.h" //#include "mmi_pubwin.h" 

#include "mmidisplay_data.h" 

typedef enum MMIGAME_ALIGNMENT_TAG
{
  LCD_LEFT_ALIGNED,
  LCD_RIGHT_ALIGNED,
  LCD_CENTERED_ALIGNED
}MMIGAME_ALIGNMENT_T;


LOCAL int8 	           s_board[BOARD_LINE_NUMBER][BOARD_LINE_NUMBER];	
LOCAL int16		    s_type;
LOCAL uint16             s_by_color;
LOCAL int16               s_stone_num = 0;
LOCAL BOOLEAN        s_is_over = FALSE;
LOCAL GUI_POINT_T  s_cur_point;
LOCAL BOOLEAN           s_is_continue = FALSE; 
LOCAL BOOLEAN           s_is_lastview = FALSE;
LOCAL MMIACC_RTC_TIME_T s_time;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DECLARE                                                                           *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//   FUNCTION:     Ring
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void Ring(void);

/*****************************************************************************/
//   FUNCTION:     UrgentPoint
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  GUI_POINT_T UrgentPoint(
								MMI_WIN_ID_T win_id, 
								uint8 byColor
								);

/*****************************************************************************/
//   FUNCTION:      GetMin
// 	Description :   
//	Global resource dependence :   
//   Author:        
//	Note:   
/*****************************************************************************/
LOCAL  int16 GetMin(int16 n1, int16 n2);

/*****************************************************************************/
//   FUNCTION:     DisplayGroud
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayGroud(void);

/*****************************************************************************/
//   FUNCTION:     Judge
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  int16 Judge(int16 nX, int16 nY, int8 cValue);

/*****************************************************************************/
//   FUNCTION:     DisplayCurrentCross
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayCurrentCross(
								uint16 x_colum, 
								uint16 y_colum, 
								uint16 color, 
								uint16 length,
								LCD_COPY_MODE_E mode
								);

/*****************************************************************************/
//   FUNCTION:     DisplayStones
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayStones(void );

/*****************************************************************************/
//   FUNCTION:     DisplayCornerCross
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  BOOLEAN DisplayCornerCross(
									uint16 x_colum, 
									uint16 y_colum
									);

/*****************************************************************************/
//   FUNCTION:     InitParams
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void InitParams(void);

/*****************************************************************************/
//   FUNCTION:     DisplayCurrentCursor
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayCurrentCursor(void);

/*****************************************************************************/
//   FUNCTION:     ClearCurrentCursor
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void ClearCurrentCursor(void);

/*****************************************************************************/
//   FUNCTION:     DisplayCurrentHand
// 	Description :  
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayCurrentHand(uint8 current);

/*****************************************************************************/
//   FUNCTION:     DisplayStone
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayStone(int16 nx, int16 ny, uint16 color);

/**---------------------------------------------------------------------------*
 **                       function bodies  -- local
 **---------------------------------------------------------------------------*/
PUBLIC  BOOLEAN  MMIGMQUT_Init(void)
{
    uint16 nv_return = 0;
    uint16 high_score[3] = {0};
    uint8 lever[6] = {0};
    
    MMINV_READ(MMINV_GMQUT_LEVER_SET, (uint16*)lever, nv_return);
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&lever[0], 0, 6);
        MMINV_WRITE(MMINV_GMQUT_LEVER_SET, &lever[0]);
    }

   
    MMINV_READ(MMINV_GMQUT_HIGH_SCORE, ( int16*)high_score, nv_return);   
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&high_score[0], 0, 6);
        MMINV_WRITE(MMINV_GMQUT_HIGH_SCORE, &high_score[0]);
    }
   
    return TRUE;

}
/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_StartQuintet   ( 五子棋 )
// 	Description :   game start
//
//	Global resource dependence : 
//   Author:        
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIGMQUT_StartQuintet(void)
{
    uint8 lever[6] = {0};
	MMIACC_RTC_TIME_T *rtc = NULL;
    uint16 nv_return = 0;


	InitParams();
	//rtc = MMIAPICOM_GetRTCTime(&s_time);
	rtc = MMIAPICOM_GetRTCTime();
	srand ( ((*rtc)[0] * 1000 + (*rtc)[1] *100 + (*rtc)[3] * 10 + (*rtc)[4]));
	MMINV_READ(MMINV_GMQUT_LEVER_SET, (uint16*)lever, nv_return);
	if (lever[1] == 1)
	{
		s_by_color = !s_by_color;
		s_board[7][7]=s_by_color+1;
		Judge(7, 7, s_board[7][7]);
		Ring();
	 }

	s_is_continue = TRUE;
	s_is_lastview = FALSE;
	s_cur_point.x = 7;
	s_cur_point.y = 7;

}


/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_Get_Renstatus
// 	Description :   
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIGMQUT_Get_Renstatus( void )
{    
    return s_is_continue;
}
/*****************************************************************************/
//   FUNCTION:     InitParams
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void InitParams(void)
{
	int16 i,j;
	for (i=0; i<BOARD_LINE_NUMBER;++i)
	    for (j=0; j<BOARD_LINE_NUMBER; j++)
	    	s_board[i][j] = STONE_NULL;	

	s_is_over = FALSE;
	s_by_color = 0;            
	s_stone_num = 0;
}

/*****************************************************************************/
//   FUNCTION:     DisplayStone
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayStone(int16 nx, int16 ny, uint16 color)
{
   	int8	i = 0;
	int16 x = 0;
	int16 y = 0;
	BOOLEAN is_landscape = FALSE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {0};
	GUI_RECT_T clip_rect = {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};
    	    
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;

	is_landscape = MMITHEME_IsMainScreenLandscape();

	if(!is_landscape)
	{
		x = TOP_LINE_POS_X+ nx * LINE_INTERVAL;
		y = TOP_LINE_POS_Y+ ny * LINE_INTERVAL;
	}
	else
	{
		clip_rect.left = 0;
		clip_rect.right = MMI_MAINSCREEN_HEIGHT;
		clip_rect.top = 0;
		clip_rect.bottom = MMI_MAINSCREEN_WIDTH;
		x = TOP_LINE_POS_H_X+ nx * LINE_INTERVAL;
		y = TOP_LINE_POS_H_Y+ ny * LINE_INTERVAL;
	}
	

	
	    if (STONE_WHITE == color)  
	    {		
	    	LCD_FillCircle(&lcd_dev_info,
					    &clip_rect,
					    x,
					    y,
					    CIRCLE_R,
					    COLOR_WHITE				    
					   );
	    }
	    else if (STONE_BLACK == color)
	    {
	    	LCD_FillCircle(&lcd_dev_info,
			    &clip_rect,
			    x,
			    y,
			    CIRCLE_R,
			    GM_COLOR_BLACK				    
			   );
	    }

	    /*else
	        DisplayCurrentCross(nx, ny, COLOR_QUINTET_LINE, 4, XOR_MODE);
	        */
}



/*****************************************************************************/
//   FUNCTION:     DisplayStones
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayStones(void )
{
	int16 i,j;

	for (i = 0; i < BOARD_LINE_NUMBER; i ++)
	{
		for ( j = 0; j < BOARD_LINE_NUMBER; j ++)	
		{
			DisplayStone(i,j,s_board[i][j]);
		}
	}

}

/*****************************************************************************/
//   FUNCTION:     DisplayCurrentHand
// 	Description :  
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayCurrentHand(uint8 current)
{
   	GUI_RECT_T  rect =  QUENTET_INFO_RECT;
	GUI_LCD_DEV_INFO	lcd_dev_info = {0};
	GUISTR_STYLE_T text_style = {0};
	 MMI_STRING_T final_string = {0};
	 BOOLEAN is_landscape = FALSE;
    
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;

	 text_style.font_color = GM_COLOR_BLACK;
   	 text_style.font = SONG_FONT_16;
   	 text_style.align = ALIGN_LEFT;

	is_landscape = MMITHEME_IsMainScreenLandscape();	

	if(is_landscape)
	{
		rect.left = QUENTET_INFO_H_RECT_LEFT;
		rect.top = QUENTET_INFO_H_RECT_TOP;
		rect.right = QUENTET_INFO_H_RECT_RIGHT;
		rect.bottom = QUENTET_INFO_H_RECT_BOTTOM;
	}
	
	MMI_GetLabelTextByLang(TXT_GMQUT_CURRENT_HAND, &final_string);
	
	GUISTR_DrawTextToLCDInRect(
		&lcd_dev_info,	
		&rect,
		&rect,
		&final_string, 
		&text_style,
		GUISTR_STATE_ALIGN,
                   GUISTR_TEXT_DIR_AUTO
		);
	
   	 current =!current;
    //michael wang for5   
    	if(!is_landscape)
    	{
#ifdef MAINLCD_SIZE_320X480
   	DisplayStone(10,16,current+1); 		//显示棋盘下的小石子
#elif defined MAINLCD_SIZE_240X400
    	DisplayStone(13,16,current+1); 		//显示棋盘下的小石子
#elif defined MAINLCD_SIZE_240X320
    	DisplayStone(13,15,current+1); 		//显示棋盘下的小石子
#else //MAINLCD_SIZE_320X240
        DisplayStone(17,7,current+1); 		//显示棋盘下的小石子
#endif    
    	}
	else
	{
#ifdef MAINLCD_SIZE_320X480
   	DisplayStone(16,6,current+1); 		//显示棋盘下的小石子
#elif defined MAINLCD_SIZE_240X400
    	DisplayStone(16,6,current+1); 		//显示棋盘下的小石子
#elif defined MAINLCD_SIZE_240X320
    	DisplayStone(16,6,current+1); 		//显示棋盘下的小石子
#else //MAINLCD_SIZE_320X240
        DisplayStone(17,7,current+1); 		//显示棋盘下的小石子
#endif    	
	}

}
/*****************************************************************************/
//   FUNCTION:     DisplayGroud
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayGroud(void)
{
	int16 i = 0;	
    	uint16 lcd_width = 0;
	uint16 lcd_height = 0;
	GUI_LCD_DEV_INFO	lcd_dev_info = {0};
	BOOLEAN is_landscape = FALSE;   
	
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;

	is_landscape = MMITHEME_IsMainScreenLandscape();	

	MMIAPIGAME_ClearScreenColor(COLOR_TAN);

	 GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

	if(!is_landscape)
	{
		for ( i = 0; i < 15; i ++)
		{
		LCD_DrawLine(&lcd_dev_info,(TOP_LINE_POS_X+ i * LINE_INTERVAL),  //花竖线
					TOP_LINE_POS_Y, 
					(TOP_LINE_POS_X+ i * LINE_INTERVAL),
					BOTTOM_LINE_POS_Y, COLOR_QUINTET_LINE);
        
		LCD_DrawLine(&lcd_dev_info, TOP_LINE_POS_X, 				//画横线
					(TOP_LINE_POS_Y+ i * LINE_INTERVAL), 
					//lcd_width - TOP_LINE_POS_X,
                    			14*LINE_INTERVAL + TOP_LINE_POS_X,
					(TOP_LINE_POS_Y + i * LINE_INTERVAL), COLOR_QUINTET_LINE);				
                    
		}		
	}
	else
	{
		for ( i = 0; i < 15; i ++)
		{
		LCD_DrawLine(&lcd_dev_info,(TOP_LINE_POS_H_X+ i * LINE_INTERVAL),  //花竖线
					TOP_LINE_POS_H_Y, 
					(TOP_LINE_POS_H_X+ i * LINE_INTERVAL),
					BOTTOM_LINE_POS_H_Y, COLOR_QUINTET_LINE);
        
		LCD_DrawLine(&lcd_dev_info, TOP_LINE_POS_H_X, 				//画横线
					(TOP_LINE_POS_H_Y+ i * LINE_INTERVAL), 
					//lcd_width - TOP_LINE_POS_X,
                    			14*LINE_INTERVAL + TOP_LINE_POS_H_X,
					(TOP_LINE_POS_H_Y + i * LINE_INTERVAL), COLOR_QUINTET_LINE);				
                    
		}	
	}
	
}
#ifndef TOUCHPANEL_TYPE
/*****************************************************************************/
//   FUNCTION:     DisplayCurrentCross
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayCurrentCross(
								uint16 x_colum, 
								uint16 y_colum, 
								uint16 color, 
								uint16 length,
								LCD_COPY_MODE_E mode
								)
{
	BOOLEAN is_landscape = FALSE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {0};
    
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
	
	is_landscape = MMITHEME_IsMainScreenLandscape();
	
	if(!is_landscape)
	{
		//竖线
	    	LCD_DrawLine(&lcd_dev_info,
	                			TOP_LINE_POS_X + x_colum * LINE_INTERVAL, 		
			  		TOP_LINE_POS_Y+ y_colum * LINE_INTERVAL - length,			
			  		TOP_LINE_POS_X+ x_colum * LINE_INTERVAL,		
			  		TOP_LINE_POS_Y+ y_colum * LINE_INTERVAL + length, 
			  		color);
			
		// horizantal
	    	LCD_DrawLine(&lcd_dev_info, 										
	                			TOP_LINE_POS_X+ x_colum* LINE_INTERVAL-length,		
	    	         			TOP_LINE_POS_Y+ y_colum* LINE_INTERVAL, 		
	    		  		TOP_LINE_POS_X+ x_colum* LINE_INTERVAL+length,	
	    		  		TOP_LINE_POS_Y+ y_colum* LINE_INTERVAL, 
	    		  		color);
	}
	else
	{
		//竖线
	    	LCD_DrawLine(&lcd_dev_info,
	                			TOP_LINE_POS_H_X + x_colum * LINE_INTERVAL, 		
			  		TOP_LINE_POS_H_Y+ y_colum * LINE_INTERVAL - length,			
			  		TOP_LINE_POS_H_X+ x_colum * LINE_INTERVAL,		
			  		TOP_LINE_POS_H_Y+ y_colum * LINE_INTERVAL + length, 
			  		color);
			
		// horizantal
	    	LCD_DrawLine(&lcd_dev_info, 										
	                			TOP_LINE_POS_H_X+ x_colum* LINE_INTERVAL-length,		
	    	         			TOP_LINE_POS_H_Y+ y_colum* LINE_INTERVAL, 		
	    		  		TOP_LINE_POS_H_X+ x_colum* LINE_INTERVAL+length,	
	    		  		TOP_LINE_POS_H_Y+ y_colum* LINE_INTERVAL, 
	    		  		color);
	}
	
}

/*****************************************************************************/
//   FUNCTION:     ClearCurrentCursor
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void ClearCurrentCursor(void)
{
      	// draw a line first to cover cursor line
   	if (!DisplayCornerCross(s_cur_point.x, s_cur_point.y))
   	 {
        		DisplayCurrentCross(s_cur_point.x, s_cur_point.y,  COLOR_QUINTET_LINE, CURSOR_RADIUS, XOR_MODE); 
   	 }

    	DisplayStone(s_cur_point.x, s_cur_point.y,s_board[s_cur_point.x][s_cur_point.y]);
}

/*****************************************************************************/
//   FUNCTION:     DisplayCornerCross
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  BOOLEAN DisplayCornerCross(
									uint16 x_colum, 
									uint16 y_colum
									)
{
    	uint16 x1,x2,y1,y2,cx,cy;
    	BOOLEAN is_corner = TRUE;
	BOOLEAN is_landscape = FALSE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {0};   
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
	
	is_landscape = MMITHEME_IsMainScreenLandscape();

	if(!is_landscape)
	{
		cx = TOP_LINE_POS_X + x_colum * LINE_INTERVAL;
	    	cy = TOP_LINE_POS_Y + y_colum * LINE_INTERVAL;
	}
	else
	{
		cx = TOP_LINE_POS_H_X + x_colum * LINE_INTERVAL;
	    	cy = TOP_LINE_POS_H_Y + y_colum * LINE_INTERVAL;
	}
    

	    
	    x1 = cx - CURSOR_RADIUS;
	    x2 = cx + CURSOR_RADIUS;
	    y1 = cy - CURSOR_RADIUS;
	    y2 = cy + CURSOR_RADIUS;
    
    if (0 == x_colum && (BOARD_LINE_NUMBER-1 != y_colum) && 0 != y_colum)  //  left side      
    {     // LEFT SIDE      
          // draw with background color      
          LCD_DrawHLine(&lcd_dev_info, x1,cy, cx,COLOR_TAN);     
          // draw with line color      
          LCD_DrawHLine(&lcd_dev_info, cx, cy, x2, COLOR_QUINTET_LINE);
          LCD_DrawVLine(&lcd_dev_info, cx, y1, y2, COLOR_QUINTET_LINE);     
    }else if (0 == x_colum && 0 == y_colum)      
      {       // left-top corner      
          // with background color      
          LCD_DrawHLine(&lcd_dev_info, x1,cy,cx, COLOR_TAN);
          LCD_DrawVLine(&lcd_dev_info, cx, y1, cy, COLOR_TAN);
          // with line color      
          LCD_DrawHLine (&lcd_dev_info, cx, cy, x2, COLOR_QUINTET_LINE);
          LCD_DrawVLine( &lcd_dev_info,  cx, cy, y2, COLOR_QUINTET_LINE);
      }else if (0== x_colum && (BOARD_LINE_NUMBER -1 == y_colum))
        {       // left-bottom corner
            // draw line wtih background color
            LCD_DrawHLine(&lcd_dev_info, x1,cy,cx, COLOR_TAN);
            LCD_DrawVLine(&lcd_dev_info, cx, cy, y2, COLOR_TAN);
            // DRAW WITH LINE COLOR
            LCD_DrawHLine(&lcd_dev_info,  cx, cy, x2, COLOR_QUINTET_LINE);
            LCD_DrawVLine(&lcd_dev_info, cx, y1, cy, COLOR_QUINTET_LINE);
        }else if (BOARD_LINE_NUMBER-1 == x_colum && BOARD_LINE_NUMBER-1 != y_colum && 0 != y_colum)
            {       // right  side
                // draw with background color      
                LCD_DrawHLine(&lcd_dev_info, cx, cy, x2,COLOR_TAN);     
                // draw with line color      
                LCD_DrawHLine(&lcd_dev_info, x1, cy, cx, COLOR_QUINTET_LINE);            
                LCD_DrawVLine(&lcd_dev_info, cx, y1, y2, COLOR_QUINTET_LINE);     
            }else if (BOARD_LINE_NUMBER-1== x_colum && 0 == y_colum)      
              {       // right-top corner      
                  // with background color      
                  LCD_DrawHLine(&lcd_dev_info, cx,cy,x2, COLOR_TAN);
                  LCD_DrawVLine(&lcd_dev_info, cx, y1, cy, COLOR_TAN);
                  // with line color      
                  LCD_DrawHLine (&lcd_dev_info, x1, cy, cx, COLOR_QUINTET_LINE);
                  LCD_DrawVLine( &lcd_dev_info,  cx, cy, y2, COLOR_QUINTET_LINE);
              }else if (BOARD_LINE_NUMBER-1== x_colum && BOARD_LINE_NUMBER -1 == y_colum)
                {       // right-bottom corner
                    // draw line wtih background color
                    LCD_DrawHLine(&lcd_dev_info, cx,cy,x2, COLOR_TAN);
                    LCD_DrawVLine(&lcd_dev_info, cx, cy, y2, COLOR_TAN);
                    // DRAW WITH LINE COLOR        
                    LCD_DrawHLine(&lcd_dev_info,  x1, cy, cx, COLOR_QUINTET_LINE);
                    LCD_DrawVLine(&lcd_dev_info, cx, y1, cy, COLOR_QUINTET_LINE);
                }else if (0 == y_colum && 0 != x_colum  && BOARD_LINE_NUMBER -1 != x_colum)
                    {       //  top side
                          // draw with background color      
                          LCD_DrawVLine(&lcd_dev_info, cx,y1, cy,COLOR_TAN);     
                          // draw with line color      
                          LCD_DrawHLine(&lcd_dev_info, x1, cy, x2, COLOR_QUINTET_LINE);
                          LCD_DrawVLine(&lcd_dev_info, cx, cy, y2, COLOR_QUINTET_LINE);     
                    }else if (BOARD_LINE_NUMBER-1 == y_colum && 0 != x_colum  && BOARD_LINE_NUMBER -1 != x_colum)
                        {       // bottom line
                              // draw with background color      
                              LCD_DrawVLine(&lcd_dev_info, cx,cy, y2,COLOR_TAN);     
                              // draw with line color      
                              LCD_DrawHLine(&lcd_dev_info, x1, cy, x2, COLOR_QUINTET_LINE);
                              LCD_DrawVLine(&lcd_dev_info, cx, y1, cy, COLOR_QUINTET_LINE);     
                        }else  
                            is_corner = FALSE;

    return is_corner;                        
}

/*****************************************************************************/
//   FUNCTION:     DisplayCurrentCursor
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void DisplayCurrentCursor(void)
{
    uint16 color = COLOR_WHITE;
    uint16 i,j;
    
    i = s_cur_point.x;
    j = s_cur_point.y;
    if (STONE_WHITE == s_board[i][j]) color = COLOR_RED;
    else if ( STONE_BLACK ==  s_board[i][j] ) color = COLOR_WHITE;
    else color = COLOR_QUINTET_CURSOR;
    
    DisplayCurrentCross(
                s_cur_point.x, 
                s_cur_point.y,  
                color, 
                CURSOR_RADIUS, 
                XOR_MODE);               
 }
#endif  


//michael wang for5
#ifdef TOUCHPANEL_TYPE
/*****************************************************************************/
// FUNCTION:     JudgePointAvail
// Description :   判断touch point是否有效(有效指能画石头)
//Global resource dependence : 
//Author: Michael wang for touch version     
//Note:   在调用此函数之前已经通过GUI_PointIsInRect
//            验证了此Touch point已经在棋盘范围了
/*****************************************************************************/
LOCAL BOOLEAN JudgePointAvail(int16 x,int16 y,BOOLEAN is_landscape)
{
	BOOLEAN retx = FALSE;
	BOOLEAN rety = FALSE;
	BOOLEAN ret = FALSE;

	if(!is_landscape)
	{
			if(( x < TOP_LINE_POS_X) || 											//有效性: x小于棋盘左边缘
			     (((x -TOP_LINE_POS_X)%LINE_INTERVAL)<=POINT_VALID_OFFSET) ||		//有效性:x与棋盘间隔的余< 5或>15，这里以棋盘每个交叉点为原点，
			     (((x -TOP_LINE_POS_X)%LINE_INTERVAL)>=(LINE_INTERVAL - POINT_VALID_OFFSET)))                 //向周围为offset 有效，扩大点击有效性
			{
				retx = TRUE;
			}

			if(( y < TOP_LINE_POS_Y) || 
			     (((y -TOP_LINE_POS_Y)%LINE_INTERVAL)<=POINT_VALID_OFFSET) ||
			     (((y -TOP_LINE_POS_Y)%LINE_INTERVAL)>=(LINE_INTERVAL - POINT_VALID_OFFSET)))
			{
				rety = TRUE;
			}

	}
	else
	{
			if(( x < TOP_LINE_POS_H_X) || 											//有效性: x小于棋盘左边缘
			     (((x -TOP_LINE_POS_H_X)%LINE_INTERVAL)<=POINT_VALID_OFFSET) ||		//有效性:x与棋盘间隔的余< 5或>15，这里以棋盘每个交叉点为原点，
			     (((x -TOP_LINE_POS_H_X)%LINE_INTERVAL)>=(LINE_INTERVAL - POINT_VALID_OFFSET)))                 //向周围为offset 有效，扩大点击有效性
			{
				retx = TRUE;
			}

			if(( y < TOP_LINE_POS_H_Y) || 
			     (((y -TOP_LINE_POS_H_Y)%LINE_INTERVAL)<=POINT_VALID_OFFSET) ||
			     (((y -TOP_LINE_POS_H_Y)%LINE_INTERVAL)>=(LINE_INTERVAL - POINT_VALID_OFFSET)))
			{
				rety = TRUE;
			}

	}
		
	ret = retx&rety;
	
	return ret;
}
/*****************************************************************************/
// FUNCTION:     ConfirmBoardPosition
// Description :   确定touch point属于棋盘上哪个坐标点
//Global resource dependence : 
//Author: Michael wang for touch version     
//Note:   在调用此函数之前已经通过GUI_PointIsInRect
//            验证了此Touch point已经在棋盘范围了
/*****************************************************************************/
LOCAL void ConfirmBoardPosition(int16 x,int16 y,BOOLEAN is_landscape)
{
	int16 x_board = 0;
	int16 y_board = 0;

	if(!is_landscape)
	{
		if(x < TOP_LINE_POS_X )		//小于棋盘边界则为 0
		{
			y_board = 0;
		}
		else 
		{
			y_board = (x -TOP_LINE_POS_X )/LINE_INTERVAL;

			if(((x -TOP_LINE_POS_X)%LINE_INTERVAL)>=((LINE_INTERVAL - POINT_VALID_OFFSET)))	//余数大于等于间(隔减去有效偏移量) 算作下一个坐标点
			{
				y_board = y_board + 1;
			}
		}

		if(y < TOP_LINE_POS_Y)
		{
			x_board = 0;		
		}
		else
		{
			x_board = (y -TOP_LINE_POS_Y )/LINE_INTERVAL;

			if(((y -TOP_LINE_POS_Y)%LINE_INTERVAL)>=(LINE_INTERVAL - POINT_VALID_OFFSET))
			{
				x_board = x_board + 1;
			}
		}
	}
	else
	{
		if(x < TOP_LINE_POS_H_X )		//小于棋盘边界则为 0
		{
			y_board = 0;
		}
		else 
		{
			y_board = (x -TOP_LINE_POS_H_X )/LINE_INTERVAL;

			if(((x -TOP_LINE_POS_H_X)%LINE_INTERVAL)>=((LINE_INTERVAL - POINT_VALID_OFFSET)))	//余数大于等于间(隔减去有效偏移量) 算作下一个坐标点
			{
				y_board = y_board + 1;
			}
		}

		if(y < TOP_LINE_POS_H_Y)
		{
			x_board = 0;		
		}
		else
		{
			x_board = (y -TOP_LINE_POS_H_Y )/LINE_INTERVAL;

			if(((y -TOP_LINE_POS_H_Y)%LINE_INTERVAL)>=(LINE_INTERVAL - POINT_VALID_OFFSET))
			{
				x_board = x_board + 1;
			}
		}

	}

	s_cur_point.x = y_board;
	s_cur_point.y = x_board;
	
}
#endif
//michael wang


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
													)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    int16 nX = 0;
    int16 nY = 0;
    int16 i = 0;
    int16 j = 0;
    int16 bsign = 0;
    GUI_POINT_T ptBest = {0};

	nX = s_cur_point.x;
	nY = s_cur_point.y;
	switch (msg_id)
	{
#ifdef TOUCHPANEL_TYPE	
	case MSG_TP_PRESS_UP:
		{
			BOOLEAN is_landscape = FALSE;
			MMI_TP_STATUS_E tp_status = 0;
			GUI_POINT_T point = {0};
			GUI_RECT_T rect = BOARD_VALID_RECT; //比棋盘范围大5，扩大有效性

			is_landscape = MMITHEME_IsMainScreenLandscape();	

			if(is_landscape)
			{
				rect.left = BOARD_VALID_H_RECT_LEFT;
				rect.top = BOARD_VALID_H_RECT_TOP;
				rect.right = BOARD_VALID_H_RECT_RIGHT;
				rect.bottom = BOARD_VALID_H_RECT_BOTTOM;
			}
			
			MMK_GetLogicTPMsg(&tp_status, &point);

			if(GUI_PointIsInRect(point,rect))
			{
				if(JudgePointAvail(point.x,point.y,is_landscape))
				{
					ConfirmBoardPosition(point.x,point.y,is_landscape);

					nX = s_cur_point.x;
					nY = s_cur_point.y;

					if (s_board[nX][nY]==0 && !s_is_over)
					{					
						s_by_color = !s_by_color;			// 1-Black  0-White
						s_board[nX][nY]=s_by_color+1;
						
						DisplayStone(nX,nY, s_board[nX][nY]);
						Judge(nX, nY, s_board[nX][nY]);
						Ring();

						// Computer                       
						if (!s_is_over)
						{
							s_by_color = !s_by_color;			// 1-Black  0-White
							ptBest = UrgentPoint(win_id, s_by_color);
							
							if (s_is_over) return recode;
							
							s_board[ptBest.x][ptBest.y]=s_by_color+1;
							s_cur_point.x = ptBest.x;
							s_cur_point.y = ptBest.y;
							DisplayStone(s_cur_point.x,s_cur_point.y, s_board[s_cur_point.x][s_cur_point.y]);
							Judge(ptBest.x, ptBest.y, s_board[ptBest.x][ptBest.y]);
							Ring();
						}
						
						if (s_is_over)
						{
							if (s_by_color == 0)
							{//s_white win
			                    			MMIPUB_OpenAlertSuccessWin(TXT_GMQUT_WHITE_WIN);
								MMK_CloseWin(win_id);
							}
							else
							{//s_black win
			                    			MMIPUB_OpenAlertSuccessWin(TXT_GMQUT_BLACK_WIN);
								MMK_CloseWin(win_id);
					                }
						}
						
						//judge for draw
						bsign = 0;
						for (i = 0; i < BOARD_LINE_NUMBER; i++)
						{
							if (bsign != 0) break;
							for (j = 0; j < BOARD_LINE_NUMBER; j++)
							{
								if (s_board[i][j] == 0)
								{
									bsign = 1;
									break;
								}
							}
						}
						
						if (bsign == 0)
						{//draw
							s_is_over = TRUE;
							s_is_continue = FALSE;
							s_is_lastview = TRUE;
			                			MMIPUB_OpenAlertSuccessWin(TXT_GMQUT_CANCEL);
							MMK_CloseWin(win_id);
						}
					}					
				}
			}
			
		}
		break;
#else		
	case MSG_KEYDOWN_OK:
		if (s_board[nX][nY]==0 && !s_is_over)   
		{
			ClearCurrentCursor(); 
			s_by_color = !s_by_color;			// 1-Black  0-White
			s_board[nX][nY]=s_by_color+1;
			DisplayStone(nX,nY, s_board[nX][nY]);
	//		DisplayCurrentHand( s_board[nX][nY]);
			Judge(nX, nY, s_board[nX][nY]);
			Ring();
		//	DisplayStone(nX,nY, s_board[nX][nY]);   DUPLICATE
			// Computer                       
			if (!s_is_over)
			{
				s_by_color = !s_by_color;			// 1-Black  0-White
				ptBest = UrgentPoint(win_id, s_by_color);
				
				if (s_is_over) return recode;
				
				s_board[ptBest.x][ptBest.y]=s_by_color+1;
//				DisplayStone(ptBest.x,ptBest.y);
				s_cur_point.x = ptBest.x;
				s_cur_point.y = ptBest.y;
				DisplayStone(s_cur_point.x,s_cur_point.y, s_board[s_cur_point.x][s_cur_point.y]);
				DisplayCurrentCursor();
				Judge(ptBest.x, ptBest.y, s_board[ptBest.x][ptBest.y]);
				Ring();
			}
			if (s_is_over)
			{
				if (s_by_color == 0)
				{//s_white win
                    			MMIPUB_OpenAlertSuccessWin(TXT_GMQUT_WHITE_WIN);
					MMK_CloseWin(win_id);
				}
				else
				{//s_black win
                    			MMIPUB_OpenAlertSuccessWin(TXT_GMQUT_BLACK_WIN);
					MMK_CloseWin(win_id);
		                }
			}
			//judge for draw
			bsign = 0;
			for (i = 0; i < BOARD_LINE_NUMBER; i++)
			{
				if (bsign != 0) break;
				for (j = 0; j < BOARD_LINE_NUMBER; j++)
				{
					if (s_board[i][j] == 0)
					{
						bsign = 1;
						break;
					}
				}
			}
			if (bsign == 0)
			{//draw
				s_is_over = TRUE;
				s_is_continue = FALSE;
				s_is_lastview = TRUE;
                			MMIPUB_OpenAlertSuccessWin(TXT_GMQUT_CANCEL);
				MMK_CloseWin(win_id);
			}
		}
		break;
	case MSG_KEYDOWN_LEFT:
		if (s_cur_point.x > 0)
		{
			ClearCurrentCursor(); 
			s_cur_point.x --;
			DisplayCurrentCursor();
		}
		break;
	case MSG_KEYDOWN_RIGHT:
		if (s_cur_point.x < BOARD_LINE_NUMBER - 1)
		{
			ClearCurrentCursor(); 
			s_cur_point.x ++;
			DisplayCurrentCursor();
		}
		break;
	case MSG_KEYDOWN_UP:
		if (s_cur_point.y > 0)
		{
			ClearCurrentCursor(); 
			s_cur_point.y --;
			DisplayCurrentCursor();
		}
		break;
	case MSG_KEYDOWN_DOWN:
		if (s_cur_point.y < BOARD_LINE_NUMBER - 1)
		{
			ClearCurrentCursor(); 
			s_cur_point.y ++;
			DisplayCurrentCursor();
		}
		break;
#endif		
	default:
              recode = MMI_RESULT_FALSE;
		break;
	}
    
    return recode;
}

/*****************************************************************************/
//   FUNCTION:     Ring
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  void Ring(void)
{	               
	s_stone_num++;
	if (s_type == 0)
	{		  
		s_is_over = TRUE;
		s_is_continue = FALSE; 
		s_is_lastview = TRUE;
	}                           
}


/*****************************************************************************/
//   FUNCTION:     Judge
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  int16 Judge(int16 nX, int16 nY, int8 cValue)
{
	int16  nGrade = 0;
	int16  i = 0;
    int16  j = 0;
    int16  k = 0;
    int16  l = 0;
	int16  nXStart = 0;
    int16  nXEnd = 0;
	int16  nYStart = 0;
    int16  nYEnd = 0;
	int16  nXYStart = 0;
    int16  nXYEnd = 0;
	int16  nYXStart = 0;
    int16  nYXEnd = 0;
	int16  nXStartAdd = 0;
    int16  nYStartAdd = 0;
    int16  nXYStartAdd = 0;
    int16  nYXStartAdd = 0;            
	int16  nXEndAdd = 0;
    int16  nYEndAdd = 0;
    int16 nXYEndAdd = 0;
    int16 nYXEndAdd = 0;
	BOOLEAN bXStartEmpty = FALSE;
    BOOLEAN bXEndEmpty = FALSE;
    BOOLEAN bXStartEmpty1 = FALSE;
    BOOLEAN bXEndEmpty1 = FALSE;
	BOOLEAN bYStartEmpty = FALSE;
    BOOLEAN bYEndEmpty = FALSE;
    BOOLEAN bYStartEmpty1 = FALSE;
    BOOLEAN bYEndEmpty1 = FALSE;
	BOOLEAN bXYStartEmpty = FALSE;
    BOOLEAN bXYEndEmpty = FALSE;
    BOOLEAN bXYStartEmpty1 = FALSE; 
    BOOLEAN bXYEndEmpty1 = FALSE;
	BOOLEAN bYXStartEmpty = FALSE;
    BOOLEAN bYXEndEmpty = FALSE;
    BOOLEAN bYXStartEmpty1 = FALSE;
    BOOLEAN bYXEndEmpty1 = FALSE;

	int16 nXStep = 0;
    int16 nYStep = 0; 
    int16 nXYStep = 0;
    int16 nYXStep = 0;
	int16 nXAdd = 0;
    int16 nYAdd = 0; 
    int16 nXYAdd = 0;
    int16 nYXAdd = 0;

	BOOLEAN	bX_4 = FALSE; 
    BOOLEAN bY_4 = FALSE ;
    BOOLEAN bXY_4 = FALSE;
    BOOLEAN bYX_4 = FALSE;
    BOOLEAN bX4 = FALSE;
    BOOLEAN bY4 = FALSE;
    BOOLEAN bXY4 = FALSE;
    BOOLEAN bYX4 = FALSE;
    BOOLEAN bX_3 = FALSE;
	BOOLEAN bY_3 = FALSE;
    BOOLEAN bXY_3 = FALSE;
    BOOLEAN bYX_3 = FALSE;
    BOOLEAN bX3 = FALSE;
    BOOLEAN bY3 = FALSE;
    BOOLEAN bXY3 = FALSE;
    BOOLEAN bYX3 = FALSE;
    BOOLEAN bX_2 = FALSE;
    BOOLEAN bY_2 = FALSE;
    BOOLEAN bXY_2 = FALSE;
	BOOLEAN bYX_2 = FALSE;
    BOOLEAN bX2 = FALSE;
    BOOLEAN bY2 = FALSE; 
    BOOLEAN bXY2 = FALSE;
    BOOLEAN bYX2 = FALSE;
    BOOLEAN bX_1 = FALSE; 
    BOOLEAN bY_1 = FALSE;
    BOOLEAN bXY_1 = FALSE;
    BOOLEAN bYX_1 = FALSE;

	BOOLEAN	b1X_4,b1Y_4,b1XY_4,b1YX_4,b1X4,b1Y4,b1XY4,b1YX4,b1X_3,b1Y_3,b1XY_3,b1YX_3;

	nXStart  = nXEnd  = nX;
	nYStart  = nYEnd  = nY;
	nXYStart = nXYEnd = nX;
	nYXStart = nYXEnd = nX;                                           
	nXStartAdd = nYStartAdd = nXYStartAdd = nYXStartAdd = 0;            	
	nXEndAdd = nYEndAdd = nXYEndAdd = nYXEndAdd = 0;            	
	bXStartEmpty = bYStartEmpty = bXYStartEmpty = bYXStartEmpty = FALSE;
	bXEndEmpty = bYEndEmpty = bXYEndEmpty = bYXEndEmpty = FALSE;
	bXStartEmpty1 = bYStartEmpty1 = bXYStartEmpty1 = bYXStartEmpty1 = FALSE;
	bXEndEmpty1 = bYEndEmpty1 = bXYEndEmpty1 = bYXEndEmpty1 = FALSE;
	
	for (i=nX-1; i>=0; i--)            // <-
	{
	    if (s_board[i][nY]==cValue)
	    	nXStart = i;
	    else if (s_board[i][nY]==0) 
	    {
			bXStartEmpty = TRUE;
			for (j=i-1; j>=0; j--)            // <-
			{
			    if (s_board[j][nY]==cValue)
	    			nXStartAdd = i-j;
			    else if (s_board[j][nY]==0) 
			    {
					bXStartEmpty1 = TRUE;
					break;
				}
				else
					break;
			}
            break;
        }                         
        else
        	break;
	}
	for (i=nX+1; i<BOARD_LINE_NUMBER; i++)           // ->
	{
	    if (s_board[i][nY]==cValue)
	    	nXEnd = i;
		else if (s_board[i][nY]==0)
		{
			bXEndEmpty = TRUE;     
			for (j=i+1; j<BOARD_LINE_NUMBER; j++)            // ->
			{
			    if (s_board[j][nY]==cValue)
	    			nXEndAdd = j-i;
			    else if (s_board[j][nY]==0) 
			    {
					bXEndEmpty1 = TRUE;
					break;
				}
				else
					break;
			}
            break;
	    }
	    else
	    	break;
	}
	    	
	for (i=nY-1; i>=0; i--)            // ^|^
	{
	    if (s_board[nX][i]==cValue)
	    	nYStart = i;
		else if (s_board[nX][i]==0)
		{
			bYStartEmpty = TRUE;   
			for (j=i-1; j>=0; j--)            // <-
			{
			    if (s_board[nX][j]==cValue)
	    			nYStartAdd = i-j;
			    else if (s_board[nX][j]==0) 
			    {
					bYStartEmpty1 = TRUE;
					break;
				}
				else
					break;
			}
            break;
		}
	    else
	    	break;
	}
	for (i=nY+1; i<BOARD_LINE_NUMBER; i++)           // v|v
	{
	    if (s_board[nX][i]==cValue)
	    	nYEnd = i;
		else if (s_board[nX][i]==0)
		{
			bYEndEmpty = TRUE;     
			for (j=i+1; j<BOARD_LINE_NUMBER; j++)            // ->
			{
			    if (s_board[nX][j]==cValue)
	    			nYEndAdd = j-i;
			    else if (s_board[nX][j]==0) 
			    {
					bYEndEmpty1 = TRUE;
					break;
				}
				else
					break;
			}
            break;
		}
	    else
	    	break;
	}
	
	//j = nY;
	for (i=nX-1, j=nY+1; i>=0&&j<BOARD_LINE_NUMBER; i--, j++)            // /'
	{
	    //j++;
	    if (s_board[i][j]==cValue)
	    	nXYStart = i;
		else if (s_board[i][j]==0)
		{
			bXYStartEmpty = TRUE; 
			for (k=i-1, l=j+1; k>=0&&l<BOARD_LINE_NUMBER; k--, l++)            // /'
			{
			    if (s_board[k][l]==cValue)
	    			nXYStartAdd = i-k;
				else if (s_board[k][l]==0)
				{
					bXYStartEmpty1 = TRUE; 
                    break;
                }
                else
                	break;
            }
			break;
		}
	    else
	    	break;
	}   
	//j = nY;
	for (i=nX+1, j=nY-1; i<BOARD_LINE_NUMBER&&j>=0; i++, j--)           // ./
	{      
		//j--;
	    if (s_board[i][j]==cValue)
	    	nXYEnd = i;
		else if (s_board[i][j]==0)
		{
			bXYEndEmpty = TRUE;   
			for (k=i+1, l=j-1; l>=0&&k<BOARD_LINE_NUMBER; l--, k++)            // /'
			{
			    if (s_board[k][l]==cValue)
	    			nXYEndAdd = k-i;
				else if (s_board[k][l]==0)
				{
					bXYEndEmpty1 = TRUE; 
                    break;
                }
                else
                	break;
            }
			break;
		}
	    else
	    	break;
	}
	    	
	//j = nY;
	for (i=nX-1, j=nY-1; i>=0&&j>=0; i--, j--)            // '`
	{
	    //j--;
	    if (s_board[i][j]==cValue)
	    	nYXStart = i;
		else if (s_board[i][j]==0)
		{
			bYXStartEmpty = TRUE; 
			for (k=i-1, l=j-1; k>=0&&l>=0; k--, l--)            // /'
			{
			    if (s_board[k][l]==cValue)
	    			nYXStartAdd = i-k;
				else if (s_board[k][l]==0)
				{
					bYXStartEmpty1 = TRUE; 
                    break;
                }
                else
                	break;
            }
			break;
		}
	    else
	    	break;
	}   
	//j = nY;
	for (i=nX+1, j=nY+1; i<BOARD_LINE_NUMBER&&j<BOARD_LINE_NUMBER; i++, j++)           // `.
	{      
		//j++;
	    if (s_board[i][j]==cValue)
	    	nYXEnd = i;
		else if (s_board[i][j]==0)
		{
			bYXEndEmpty = TRUE;   
			for (k=i+1, l=j+1; l<BOARD_LINE_NUMBER&&k<BOARD_LINE_NUMBER; l++, k++)            // /'
			{
			    if (s_board[k][l]==cValue)
	    			nYXEndAdd = k-i;
				else if (s_board[k][l]==0)
				{
					bYXEndEmpty1 = TRUE; 
                    break;
                }
                else
                	break;
            }
			break;
		}
	    else
	    	break;
	}
	        
	nXStep  = nXEnd-nXStart+1;
	nYStep  = nYEnd-nYStart+1;
	nXYStep = nXYEnd-nXYStart+1;
	nYXStep = nYXEnd-nYXStart+1;
	
	bX_4 = ((nXStep==4)&&(bXStartEmpty&&bXEndEmpty));
	bY_4 = ((nYStep==4)&&(bYStartEmpty&&bYEndEmpty));
	bXY_4 = ((nXYStep==4)&&(bXYStartEmpty&&bXYEndEmpty));
	bYX_4 = ((nYXStep==4)&&(bYXStartEmpty&&bYXEndEmpty));
	bX4 = ((nXStep==4)&&(bXStartEmpty||bXEndEmpty));
	bY4 = ((nYStep==4)&&(bYStartEmpty||bYEndEmpty));
	bXY4 = ((nXYStep==4)&&(bXYStartEmpty||bXYEndEmpty));
	bYX4 = ((nYXStep==4)&&(bYXStartEmpty||bYXEndEmpty));

	bX_3 = ((nXStep==3)&&(bXStartEmpty&&bXEndEmpty));
	bY_3 = ((nYStep==3)&&(bYStartEmpty&&bYEndEmpty));
	bXY_3 = ((nXYStep==3)&&(bXYStartEmpty&&bXYEndEmpty));
	bYX_3 = ((nYXStep==3)&&(bYXStartEmpty&&bYXEndEmpty));

	bX3 = ((nXStep==3)&&(bXStartEmpty||bXEndEmpty));
	bY3 = ((nYStep==3)&&(bYStartEmpty||bYEndEmpty));
	bXY3 = ((nXYStep==3)&&(bXYStartEmpty||bXYEndEmpty));
	bYX3 = ((nYXStep==3)&&(bYXStartEmpty||bYXEndEmpty));

	bX_2 = ((nXStep==2)&&(bXStartEmpty&&bXEndEmpty));
	bY_2 = ((nYStep==2)&&(bYStartEmpty&&bYEndEmpty));
	bXY_2 = ((nXYStep==2)&&(bXYStartEmpty&&bXYEndEmpty));
	bYX_2 = ((nYXStep==2)&&(bYXStartEmpty&&bYXEndEmpty));
	bX2 = ((nXStep==2)&&(bXStartEmpty||bXEndEmpty));
	bY2 = ((nYStep==2)&&(bYStartEmpty||bYEndEmpty));
	bXY2 = ((nXYStep==2)&&(bXYStartEmpty||bXYEndEmpty));
	bYX2 = ((nYXStep==2)&&(bYXStartEmpty||bYXEndEmpty));

  	bX_1 = ((nXStep==1)&&(bXStartEmpty&&bXEndEmpty));
  	bY_1 = ((nYStep==1)&&(bYStartEmpty&&bYEndEmpty));
  	bXY_1 = ((nXYStep==1)&&(bXYStartEmpty&&bXYEndEmpty));
  	bYX_1 = ((nYXStep==1)&&(bYXStartEmpty&&bYXEndEmpty));

	nXAdd = nYAdd = nXYAdd = nYXAdd = 0;

	if (nXEndAdd>=nXStartAdd)
	{
		nXAdd = nXEndAdd;
		bXEndEmpty = bXEndEmpty1;
	}
	else                      
	{
		nXAdd = nXStartAdd;
		bXStartEmpty = bXStartEmpty1;
    }
    
	if (nYEndAdd>=nYStartAdd)
	{
		nYAdd = nYEndAdd;
		bYEndEmpty = bYEndEmpty1;
	}
	else                      
	{
		nYAdd = nYStartAdd;
		bYStartEmpty = bYStartEmpty1;
    }

	if (nXYEndAdd>=nXYStartAdd)
	{
		nXYAdd = nXYEndAdd;
		bXYEndEmpty = bXYEndEmpty1;
	}
	else                      
	{
		nXYAdd = nXYStartAdd;
		bXYStartEmpty = bXYStartEmpty1;
    }

	if (nYXEndAdd>=nYXStartAdd)
	{
		nYXAdd = nYXEndAdd;
		bYXEndEmpty = bYXEndEmpty1;
	}
	else                      
	{
		nYXAdd = nYXStartAdd;
		bYXStartEmpty = bYXStartEmpty1;
    }
	
	b1X_4 = ((nXStep+nXAdd>=4)&&(bXStartEmpty&&bXEndEmpty));
	b1Y_4 = ((nYStep+nYAdd>=4)&&(bYStartEmpty&&bYEndEmpty));
	b1XY_4 = ((nXYStep+nXYAdd>=4)&&(bXYStartEmpty&&bXYEndEmpty));
	b1YX_4 = ((nYXStep+nYXAdd>=4)&&(bYXStartEmpty&&bYXEndEmpty));
	b1X4 = ((nXStep+nXAdd>=4)&&(bXStartEmpty||bXEndEmpty));
	b1Y4 = ((nYStep+nYAdd>=4)&&(bYStartEmpty||bYEndEmpty));
	b1XY4 = ((nXYStep+nXYAdd>=4)&&(bXYStartEmpty||bXYEndEmpty));
	b1YX4 = ((nYXStep+nYXAdd>=4)&&(bYXStartEmpty||bYXEndEmpty));

	b1X_3 = ((nXStep+nXAdd==3)&&(bXStartEmpty&&bXEndEmpty));
	b1Y_3 = ((nYStep+nYAdd==3)&&(bYStartEmpty&&bYEndEmpty));
	b1XY_3 = ((nXYStep+nXYAdd==3)&&(bXYStartEmpty&&bXYEndEmpty));
	b1YX_3 = ((nYXStep+nYXAdd==3)&&(bYXStartEmpty&&bYXEndEmpty));
	
	s_type = -1;	                           
	
	////////	
	if (nXStep>=5 || nYStep>=5 || nXYStep>=5 || nYXStep>=5)
	{
		nGrade = 0;
		s_type = 0;
	}
	////////	
	else if (bX_4 || bY_4 || bXY_4 || bYX_4)
	{
		nGrade = 1;             
		s_type = 1;
	}
	////////	
	else if ((bX4 && (bY4 || bXY4 || bYX4 || b1Y4 || b1XY4 || b1YX4))||
			 (bY4 && (bX4 || bXY4 || bYX4 || b1X4 || b1XY4 || b1YX4))||
			 (bXY4 && (bY4 || bX4 || bYX4 || b1Y4 || b1X4 || b1YX4)) ||
			 (bYX4 && (bY4 || bXY4 || bX4 || b1Y4 || b1XY4 || b1X4)) ||
			 (b1X4 && (bY4 || bXY4 || bYX4 || b1Y4 || b1XY4 || b1YX4))||
			 (b1Y4 && (bX4 || bXY4 || bYX4 || b1X4 || b1XY4 || b1YX4))||
			 (b1XY4 && (bY4 || bX4 || bYX4 || b1Y4 || b1X4 || bYX4))  ||
			 (b1YX4 && (bY4 || bXY4 || bX4 || b1Y4 || b1XY4 || b1X4)))
	{
		nGrade = 2;
		s_type = 1;
	}		
    /////////
	else if ((bX4 && (bY_3 || bXY_3 || bYX_3 || b1Y_3 || b1XY_3 || b1YX_3))||
	         (bY4 && (bX_3 || bXY_3 || bYX_3 || b1X_3 || b1XY_3 || b1YX_3))||
			 (bXY4 && (bY_3 || bX_3 || bYX_3 || b1Y_3 || b1X_3 || b1YX_3)) ||
			 (bYX4 && (bY_3 || bXY_3 || bX_3 || b1Y_3 || b1XY_3 || b1X_3)) ||
			 (b1X4 && (bY_3 || bXY_3 || bYX_3 || b1Y_3 || b1XY_3 || b1YX_3))||
			 (b1Y4 && (bX_3 || bXY_3 || bYX_3 || b1X_3 || b1XY_3 || b1YX_3))||
			 (b1XY4 && (bY_3 || bX_3 || bYX_3 || b1Y_3 || b1X_3 || b1YX_3)) ||
			 (b1YX4 && (bY_3 || bXY_3 || bX_3 || b1Y_3 || b1XY_3 || b1X_3)))
	{
		nGrade = 3;
		s_type = 1;
	}		
	////////	
	else if ((bX_3 && (bY_3 || bXY_3 || bYX_3 || b1Y_3 || b1XY_3 || b1YX_3))||
	         (bY_3 && (bX_3 || bXY_3 || bYX_3 || b1X_3 || b1XY_3 || b1YX_3))||
			 (bXY_3 && (bY_3 || bX_3 || bYX_3 || b1Y_3 || b1X_3 || b1YX_3)) ||
			 (bYX_3 && (bY_3 || bXY_3 || bX_3 || b1Y_3 || b1XY_3 || b1X_3)) ||
			 (b1X_3 && (bY_3 || bXY_3 || bYX_3 || b1Y_3 || b1XY_3 || b1YX_3))||
			 (b1Y_3 && (bX_3 || bXY_3 || bYX_3 || b1X_3 || b1XY_3 || b1YX_3))||
			 (b1XY_3 && (bY_3 || bX_3 || bYX_3 || b1Y_3 || b1X_3 || b1YX_3)) ||
			 (b1YX_3 && (bY_3 || bXY_3 || bX_3 || b1Y_3 || b1XY_3 || b1X_3)))
	{
		nGrade = 4;
		s_type = 2;
	}		
	////////
	else if ((bXY4 && (bYX_2 || bY_2 || bX_2))||
	         (bYX4 && (bXY_2 || bY_2 || bX_2))||
	         (bX4 && (bXY_2 || bYX_2 || bY_2))||
	         (bY4 && (bXY_2 || bYX_2 || bX_2)))
	
	{
		nGrade = 5;
		s_type = 1;
	}		                     
	else if ((bXY4 && (bYX3 || bY3 || bX3))||
	         (bYX4 && (bXY3 || bY3 || bX3))||
	         (bX4 && (bXY3 || bYX3 || bY3))||
	         (bY4 && (bXY3 || bYX3 || bX3)))
	{
		nGrade = 6;
		s_type = 1;
	}		
	else if ((bXY4 && (bYX_1 || bY_1 || bX_1))||
	         (bYX4 && (bXY_1 || bY_1 || bX_1))||
	         (bX4 && (bXY_1 || bYX_1 || bY_1))||
	         (bY4 && (bXY_1 || bYX_1 || bX_1)))
	{
		nGrade = 7;
		s_type = 2;
	}		
	else if ((bXY4 && (bYX2 || bY2 || bX2))||
	         (bYX4 && (bXY2 || bY2 || bX2))||
	         (bX4 && (bXY2 || bYX2 || bY2))||
	         (bY4 && (bXY2 || bYX2 || bX2)))
	
	{
		nGrade = 8;
		s_type = 1;
	}		
	else if (bXY4 || bYX4 || bX4 || bY4)
	{
		nGrade = 9;
		s_type = 1;
	}		
	////////
	else if ((bXY_3 && (bYX_2 || bY_2 || bX_2))||
	         (bYX_3 && (bXY_2 || bY_2 || bX_2))||
	         (bX_3 && (bXY_2 || bYX_2 || bY_2))||
	         (bY_3 && (bXY_2 || bYX_2 || bX_2)))
	{
		nGrade = 10;
		s_type = 2;
	}		
	else if ((bXY_3 && (bYX3 || bY3 || bX3))||
	         (bYX_3 && (bXY3 || bY3 || bX3))||
	         (bX_3 && (bXY3 || bYX3 || bY3))||
	         (bY_3 && (bXY3 || bYX3 || bX3)))
	{
		nGrade = 11;
		s_type = 2;
	}		
	else if ((bXY_3 && (bYX_1 || bY_1 || bX_1))||
	         (bYX_3 && (bXY_1 || bY_1 || bX_1))||
	         (bX_3 && (bXY_1 || bYX_1 || bY_1))||
	         (bY_3 && (bXY_1 || bYX_1 || bX_1)))
	{
		nGrade = 12;
		s_type = 2;
	}		
	else if ((bXY_3 && (bYX2 || bY2 || bX2))||
	         (bYX_3 && (bXY2 || bY2 || bX2))||
	         (bX_3 && (bXY2 || bYX2 || bY2))||
	         (bY_3 && (bXY2 || bYX2 || bX2)))
	{
		nGrade = 13;
		s_type = 2;
	}		
	else if (bXY_3 || bYX_3 || bX_3 || bY_3)
	{
		nGrade = 14;
		s_type = 2;
	}		           
	////////		
	else if (bXY_2 || bYX_2 || bX_2 || bY_2)
		nGrade = 16;
    ////////
    else if (bXY3 || bYX3 || bX3 || bY3)
		nGrade = 17;
	else if (bXY2 || bYX2 || bX2 || bY2)
		nGrade = 18;
	else if (bXY_1 || bYX_1 || bX_1 || bY_1)    
		nGrade = 19; 
	else
		nGrade = 20;

	///////	                
	if (nGrade>15)
		if ((nY >= 1&&(s_board[nX][nY-1]==0 || s_board[nX][nY-1]==cValue) )&&
			(s_board[nX+1][nY]==0 || s_board[nX+1][nY]==cValue) &&
			(s_board[nX][nY+1]==0 || s_board[nX][nY+1]==cValue) &&
			((nX >= 1)&&(s_board[nX-1][nY]==0 || s_board[nX-1][nY]==cValue) )&&
			((nX+2<15 && nY-2>=0 && (s_board[nX+2][nY-2]==0 || s_board[nX+2][nY-2]==cValue) &&
			 s_board[nX][nY-2]==cValue && s_board[nX+1][nY-1]==cValue && s_board[nX+2][nY]==cValue) ||
	     	(nX+2<15 && nY+2<15 && (s_board[nX+2][nY+2]==0 || s_board[nX+2][nY+2]==cValue) &&
	     	 s_board[nX][nY+2]==cValue && s_board[nX+1][nY+1]==cValue && s_board[nX+2][nY]==cValue) ||
	     	(nX-2>=0 && nY+2<15 && (s_board[nX-2][nY+2]==0 || s_board[nX-2][nY+2]==cValue) &&
	     	 s_board[nX][nY+2]==cValue && s_board[nX-1][nY+1]==cValue && s_board[nX-2][nY]==cValue) || 
		  	(nX-2>=0 && nY-2>=0 && (s_board[nX-2][nY-2]==0 || s_board[nX-2][nY-2]==cValue) &&
		  	 s_board[nX][nY-2]==cValue && s_board[nX-1][nY-1]==cValue && s_board[nX-2][nY]==cValue)))
			nGrade = 15;
	/////////

	return nGrade;		
}


/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_ResumeQuintet
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIGMQUT_ResumeQuintet(void)
{

	DisplayGroud();             // move to display current cursor for test
	DisplayStones();
#ifndef TOUCHPANEL_TYPE
	DisplayCurrentCursor();
#endif
	DisplayCurrentHand(s_by_color);
}


/*****************************************************************************/
//   FUNCTION:     UrgentPoint
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
LOCAL  GUI_POINT_T UrgentPoint(
								MMI_WIN_ID_T win_id, 
								uint8 byColor
								)
{           
	int16 i = 0, i0 = 0, j = 0, j0 = 0;             

    GUI_POINT_T ptUrgent = {0}, ptInit = {0};
    GUI_POINT_T ptCurrent = {0};
	int16 nGrade1 = 0, nGrade2 = 0;
	int16 nUrgent1 = 0, nUrgent2 = 0, nUrgent = 0,nUrgentmin = 0;

	
	ptInit.x = -1;
	ptInit.y = -1;
	ptUrgent.x = ptInit.x;
	ptUrgent.y = ptInit.y;
	nUrgentmin = 2026;	//set to the max
              
    // Seed the random-number generator with current time so that
    // the numbers will be different every time we run.
	i0=(rand()%2) ? 0:BOARD_LINE_NUMBER-1;               
	
	for (i=i0; !i0 ? i<BOARD_LINE_NUMBER:i>=0; !i0 ? ++i:--i)
	{
	    j0=(rand()%2) ? 0:BOARD_LINE_NUMBER-1;
	    for (j=j0; !j0 ? j<BOARD_LINE_NUMBER:j>=0; !j0 ? ++j:--j)
	    {
	    	if (s_board[i][j]==0)
	    	{
        		ptCurrent.x = i;
        		ptCurrent.y = j;
                
				nGrade1 = Judge(i, j, (int8)(byColor+1));
				nGrade2 = Judge(i, j, (int8)(!byColor+1));
				
		    	switch (nGrade1)
		    	{
					case 0 : nUrgent1 = 0; break;
					case 1 : nUrgent1 = 2; break;					
					case 2 : nUrgent1 = 4; break;					
					case 3 : nUrgent1 = 5; break;					
					case 4 : nUrgent1 = 8; break;					
					
					case 5 : nUrgent1 = 10; break;																									
					case 6 : nUrgent1 = 11; break;					
					case 7 : nUrgent1 = 12; break;					
					case 8 : nUrgent1 = 13; break;					
 					case 9 : nUrgent1 = 14; break;					
					case 10 : nUrgent1 = 15; break;					
					case 11 : nUrgent1 = 16; break;					
					case 12 : nUrgent1 = 17; break;					
					case 13 : nUrgent1 = 18; break;					
					case 14 : nUrgent1 = 19; break;					
					case 15 : nUrgent1 = 20; break;					
					
					case 16 : nUrgent1 = 32; break;					
					case 17 : nUrgent1 = 34; break;					
					case 18 : nUrgent1 = 36; break;					
					case 19 : nUrgent1 = 38; break;					
					case 20 : nUrgent1 = 40; break;					
					default : nUrgent1 = 40; break;					
				}
		    	switch (nGrade2)
		    	{
					case 0 : nUrgent2 = 1; break;
					case 1 : nUrgent2 = 3; break;					
					case 2 : nUrgent2 = 6; break;					
					case 3 : nUrgent2 = 7; break;					
					case 4 : nUrgent2 = 9; break;					
					
					case 5 : nUrgent2 = 21; break;																									
					case 6 : nUrgent2 = 22; break;					
					case 7 : nUrgent2 = 23; break;					
					case 8 : nUrgent2 = 24; break;					
					case 9 : nUrgent2 = 25; break;					
					case 10 : nUrgent2 = 26; break;					
					case 11 : nUrgent2 = 27; break;					
					case 12 : nUrgent2 = 28; break;					
					case 13 : nUrgent2 = 29; break;					
					case 14 : nUrgent2 = 30; break;					
					case 15 : nUrgent2 = 31; break;					
					
					case 16 : nUrgent2 = 33; break;											
					case 17 : nUrgent2 = 35; break;					
					case 18 : nUrgent2 = 37; break;					
					case 19 : nUrgent2 = 39; break;					
					case 20 : nUrgent2 = 41; break;					
					default : nUrgent2 = 41; break;											
				}
                
	            nUrgent = MIN(nUrgent1, nUrgent2)*45 + MAX(nUrgent1, nUrgent2);
				if (nUrgentmin >= nUrgent)
				{
					ptUrgent.x = ptCurrent.x;
					ptUrgent.y = ptCurrent.y;
					nUrgentmin = nUrgent;
				}
            }
		}
    }

/*	for (i=0; i<2025; ++i)   
		if (ptUrgent[i].x != ptInit.x || ptUrgent[i].y != ptInit.y)
			break;*/

    
	if (ptUrgent.x == ptInit.x && ptUrgent.y == ptInit.y)
	{
		//draw!!!!
		s_is_over = TRUE;
		s_is_continue = FALSE;
		s_is_lastview = TRUE;

        MMIPUB_OpenAlertSuccessWin(TXT_GMQUT_TIE);
        MMK_CloseWin(win_id);
  	}
	
	return ptUrgent;
}

/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_DisplayLastView
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIGMQUT_DisplayLastView(void)
{

	DisplayGroud();
	DisplayStones();
}


/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_SetQuintetHand
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIGMQUT_SetQuintetHand( uint8 chand)
{
    uint8 lever[6] = {0};
    uint16 nv_return = 0;
        
	MMINV_READ(MMINV_GMQUT_LEVER_SET, (uint16*)lever, nv_return);
	lever[1] = chand;
	MMINV_WRITE(MMINV_GMQUT_LEVER_SET, (uint16*)lever);

		s_is_continue = FALSE;
}

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
// 	Description :   
//
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIGMQUT_InitModule(void)
{
    MMIGMQUT_RegMenuGroup();  //初始化im menu
    MMIGMQUT_RegNv();   //初始化im nv
   // MMIGAME_QUINTET_RegWinIdNameArr();
}


#endif



/*Edit by script, ignore 1 case. Thu Apr 26 19:00:54 2012*/ //IGNORE9527
