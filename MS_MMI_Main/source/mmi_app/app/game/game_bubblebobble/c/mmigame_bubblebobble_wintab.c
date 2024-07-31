/****************************************************************************
** File Name:      mmigame_bubblebobble_wintab.c                            
** Author:           junxue.zhu                                                   
** Date:             2012/06/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of game
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 01/06/2012      junxue zhu            Create
** 
****************************************************************************/
#define _MMIGAME_BUBBLEBOBBLE_WINTAB_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmigame_bubblebobble_nv.h"
#include "mmilivewallpaper_export.h"
#include "ui_layer.h"
#include "mmk_type.h"
#include "graphics_draw.h"
#include "mmi_app_game_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
#include "window_parse.h"
#include "mmk_app.h"
#include "guibutton.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmigame_link_menutable.h"
#include "mmigame_menutable.h"
#include "mmigame_bubblebobble_id.h"  
#include "tb_dal.h"
#include "mmialarm_export.h"
#include "mmigame_internal.h"
#include "mmi_textfun.h"
#include "mmiphone_export.h"
#include "mmigame_bubblebobble_internal.h"
#include "mmigame_text.h"
#include "mmigame_bubblebobble_image.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipub.h"
#include "mmigamebubble_position.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "guires.h"
#include "guiedit.h"
#include "guistring.h"
#include "mmi_resource.h"
#include "guiform.h"

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA  DECLARE                           */
/*---------------------------------------------------------------------------*/
//LOCAL MMI_GAME_BUBBLE_T  bubbleArray[105] = {0};//泡泡数组
LOCAL MMI_GAME_BUBBLE_T  *bubbleArray = PNULL;
//LOCAL MMI_GAME_BUBBLE_T  prepareBubble[4] = {0};//预备泡泡
LOCAL MMI_GAME_BUBBLE_T  *prepareBubble = PNULL;
//LOCAL MMI_GAME_BUBBLE_T  movingBubble[1] = {0};//移动泡泡
LOCAL MMI_GAME_BUBBLE_T  *movingBubble = PNULL;
//LOCAL MMI_GAME_BUBBLE_T  dropArray[30] = {0}; //下落泡泡
//LOCAL MMI_GAME_BUBBLE_T  *dropArray = PNULL;
//LOCAL MMI_GAME_BUBBLE_T  popArray[30] = {0};  //爆炸泡泡
LOCAL MMI_GAME_BUBBLE_T  *popArray = PNULL;

LOCAL  GUI_LCD_DEV_INFO s_lcd_dev_info = { 0,UILAYER_NULL_HANDLE };
LOCAL  GUI_LCD_DEV_INFO s_needle_dev_info = { 0,UILAYER_NULL_HANDLE };
LOCAL  GUI_LCD_DEV_INFO s_main_lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

LOCAL const float _sin_0_90_table[91] = 
{
    0,
    0.017, 0.034, 0.052, 0.069, 0.087, 0.104, 0.121, 0.139, 0.156, 0.173,
    0.190, 0.207, 0.224, 0.241, 0.258, 0.275, 0.292, 0.309, 0.325, 0.342,
    0.358, 0.374, 0.390, 0.406, 0.422, 0.438, 0.453, 0.469, 0.484, 0.499,
    0.515, 0.529, 0.544, 0.559, 0.573, 0.587, 0.601, 0.615, 0.629, 0.642,
    0.656, 0.669, 0.681, 0.694, 0.707, 0.719, 0.731, 0.743, 0.754, 0.766,
    0.777, 0.788, 0.798, 0.809, 0.819, 0.829, 0.838, 0.848, 0.857, 0.866,
    0.874, 0.882, 0.891, 0.898, 0.906, 0.913, 0.920, 0.927, 0.933, 0.939,
    0.945, 0.951, 0.956, 0.961, 0.965, 0.970, 0.974, 0.978, 0.981, 0.984,
    0.987, 0.990, 0.992, 0.994, 0.996, 0.997, 0.998, 0.999, 0.999, 1.000,
};

LOCAL uint16 curScore = 0;//当前分数
LOCAL uint8 curLevel = 1;//当前等级
LOCAL uint8 addScore = 0;//增加分数
LOCAL uint8 row0[9] = {0}; //第一行泡泡的横坐标，在泡泡停靠最顶端时，用来确定泡泡的停位
LOCAL uint8 zero = 0;//顶行标志
LOCAL uint8 Y = 0;//挡板下降距离
LOCAL uint8 numOfColumns = 0;//最顶行的列数
LOCAL uint8 maxColumnNum = 0;//当前行最大列
LOCAL uint8 maxRowsNum = 0;//最大行
LOCAL uint8 bubbleNum = 0;
LOCAL uint8 curColorNum = 0;//当前颜色数量
LOCAL uint8 curLevel_LineCount = 0;//当前关卡，需要下降的泡泡的行数
LOCAL uint8 fall_LineCount = 0;//泡泡下降标志
LOCAL uint8 popingNum = 0;
LOCAL uint8 dropNum = 0;
LOCAL uint16 scoreOf_one_bubble = 0;//本次消去，每个泡泡的分数
LOCAL int16 shootRotation = 0;//发射台角度
LOCAL uint16 countCallBack = 0;//多少个单位时间
LOCAL uint8   creat_num = 0;   //每关新生成的泡泡的行数
LOCAL GUI_POINT_T         board_start_point = {0};
LOCAL uint8 s_input_map_num = 0;
LOCAL BOOLEAN isNewLine = TRUE;//装载游戏控制
LOCAL BOOLEAN flag_stop = TRUE;//计算速度只一次
LOCAL BOOLEAN isShoot = FALSE;//很重要，在泡泡移动过程中一直为true，只有当它为false时，才可以发射泡泡
LOCAL BOOLEAN isGo = FALSE;//与ishoot相似，在键盘的时候用到它控制泡泡的发射条件
LOCAL BOOLEAN btrace = FALSE;//标志发射出去的泡泡是否已经碰撞
LOCAL BOOLEAN flag_Win_or_Lose = FALSE;//进入胜利或者失败状态后不能发射球

/*--------------------------------------------------------------------------*/
/*                    LOCAL FUNCTION DECLARE                                */
/*--------------------------------------------------------------------------*/
/******************************************************************************/
//  Description :sin
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL float SIN_S(int16 angle);

/******************************************************************************/
//  Description : cos
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL float COS_S(int16 angle);

/******************************************************************************/
//  Description : 初始化游戏的状态
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E InitDate(void);
/******************************************************************************/
//  Description : 获得随机数
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/
LOCAL int32 GetRand(int32 min,int32 max);

/******************************************************************************/
//  Description : 得到绝对值
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/
LOCAL int32 _Abs(int32 value);

/******************************************************************************/
//  Description : 根据行列号，得到泡泡屏幕坐标
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_GetBubbleCenter(int32 c, int32 r, int32 * x, int32 * y)  ;

/******************************************************************************/
//  Description : 根据屏幕坐标，得到泡泡行列号
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_GetBubblePosition(int32 * c, int32 * r, int32  x, int32  y);

/******************************************************************************/
//  Description : 标记爆炸的泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL int32 MMIBUB_CountSameBubble(int32 c,int32 r,int32 color);

/******************************************************************************/
//  Description : 标记下坠泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL BOOLEAN MMIBUB_MarkDrop(int32 c,int32 r);  

/******************************************************************************/
//  Description : 根据新发射上来的泡泡，标记坠落的泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_DropBubble();

/******************************************************************************/
//  Description : 生成新泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL MMI_GAME_BUBBLE_T MMIBUB_NewBubble(int32 color, int32 x, int32 y, int32 bubbleState);

/******************************************************************************/
//  Description :新的一个随机行
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_AddBubbleToArray(void);

/******************************************************************************/
//  Description :重置泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_ReSetBubble(MMI_GAME_BUBBLE_T * bubble);

/******************************************************************************/
//  Description :重置dropArray，并返回当前是否还有正在下降的泡泡，
//                     如果有正在下降的泡泡，返回TRUE，否则返回FALSE
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
//LOCAL BOOLEAN MMIBUB_IsHaveDroping(void);

/******************************************************************************/
//  Description :预备泡泡的生成
//                    向moving泡泡的转换，以及大小两种预备泡泡的交替
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_ReadyBubbleBuffer(void) ;

/************************************************************************/
/*               碰撞检测                                               */
/************************************************************************/
/******************************************************************************/
//  Description :判定移动泡泡碰撞后停靠的位置是否合法
//                     其中，x,y 是移动泡泡碰撞到的那个普通泡泡的坐标
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL BOOLEAN MMIBUB_IsRightPosition(int32 x,int32 y);

/******************************************************************************/
//  Description :和泡泡碰撞停下后的位置
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL BOOLEAN MMIBUB_BubbleStop(MMI_GAME_BUBBLE_T bubble) ;

/******************************************************************************/
//  Description :泡泡停在最顶行的位置确?
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_FirstRowPosition();

/******************************************************************************/
//  Description :碰撞检测
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL BOOLEAN MMIBUB_Collide(void);

/******************************************************************************/
//  Description :无挂点泡泡下降(新buffer实现)
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
//LOCAL void MMIBUB_DrawDroping(uint16 dropCnt) ;

/******************************************************************************/
//  Description :移动准备好的泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_MoveBubble();

/******************************************************************************/
//  Description :使泡泡爆炸
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 

LOCAL uint16 MMIBUB_PopBubble(void);

/******************************************************************************/
//  Description :装载游戏
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_InitGame(void);

/*****************************************************************************/
//  Description : InitBubblebobbleWin
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void InitBubblebobbleWin(MMI_WIN_ID_T win_id,BOOLEAN is_refresh);

/*****************************************************************************/
//  Description : DrawBubble
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void DrawBigBubble(
                     MMI_WIN_ID_T win_id,
                     MMI_GAME_BUBBLE_T *BubbleArray,
                     uint16 BubbleNum,
                     MMI_GAME_BUBBLESTATE State
               //      GUI_LCD_DEV_INFO lcd_dev_info
                     );

/*****************************************************************************/
//  Description : DrawLittleBubble
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void DrawLittleBubble(
                          MMI_WIN_ID_T win_id,
                          MMI_GAME_BUBBLE_T *BubbleArray,
                          uint16 BubbleNum
                          );

/*****************************************************************************/
//  Description : change the angle of the shooter
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void ChangeShooterAngle(MMI_WIN_ID_T win_id, uint16 new_freq_angle);

/*****************************************************************************/
//  Description : DrawBombBubble
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void DrawBombBubble(
                         MMI_WIN_ID_T win_id,
                         MMI_GAME_BUBBLE_T *BubbleArray,
                         uint16 BubbleNum,
                         MMI_GAME_BUBBLESTATE State
                         );

/*****************************************************************************/
//  Description : ShowScore
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void ShowScore(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : ShowLevel
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void ShowLevel(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  FUNCTION:     Bubble_ViewScore
//  Description :   显示分数
//  Global resource dependence : 
//  Author:       junxue.zhu
//  Note:   
/*****************************************************************************/
LOCAL void Bubble_ViewScore(
                      MMI_WIN_ID_T win_id,
                      MMI_CTRL_ID_T ctrl_id, 
                      MMI_TEXT_ID_T  title_id,
                      uint16 score);

/*****************************************************************************/
//  Description :open autodemo finished window by text_id 
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void OpenThisLevelFinishedWin(void);

/*****************************************************************************/
//  Description :open autodemo read script error
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void OpenBubbleErrorWin(MMI_TEXT_ID_T text_id);

/******************************************************************************/
//  Description :HandleGMBUBBLEMainMenuWindow
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:   i  
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGMBUBBLEMainMenuWindow(
                       MMI_WIN_ID_T win_id, 
                       MMI_MESSAGE_ID_E msg_id, 
                       DPARAM param
                       );

/*****************************************************************************/
//  FUNCTION:     HandleBubbleScoreViewWinMsg
//  Description :   查看最高分数
//  Global resource dependence : 
//  Author:       junxue.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBubbleScoreViewWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );

/******************************************************************************/
//  Description :HandleBubblebobbleWinMsg
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:   i  
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBubblebobbleWinMsg( 
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      );

/*****************************************************************************/
//  Description : the bubblebobble select level window
//  Global resource dependence : none
//  Author: Junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGMBUBBLESelectMenuWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

/*****************************************************************************/
//  Description : malloc the memory
//  Global resource dependence : none
//  Author: Junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GameAllocMem(void);

LOCAL void CreateBUBBLENeedleLayer(MMI_WIN_ID_T win_id);

LOCAL void BUBBLECreateLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle);

LOCAL void BUBBLEAppendLayer(GUI_LCD_DEV_INFO *dev_info_ptr);


LOCAL void BUBBLECreateLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle);

LOCAL void BUBBLEReleaseLayer(GUI_LCD_DEV_INFO *dev_info_ptr);

/******************************************************************************/
//  Description: decompressed the ARGB image
//  Author: 
//  Input:   
//          src_img_ptr:	pointer of input image information
//			dst_buf_ptr:	pointer to the target buffer
//			dst_buf_size:	target buffer size
//  Output: 
//			dst_img_ptr:	pointer to the decompressed image information
//  Return: 
//			TRUE:			successful
//          FALSE:			failed
//  Note:   
/******************************************************************************/
LOCAL BOOLEAN DecompressARGB(const MMI_IMAGE_BUBBLE_T *src_img_ptr, MMI_IMAGE_BUBBLE_T *dst_img_ptr, 
							  uint8 *dst_buf_ptr, uint32 dst_buf_size);


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE( MMIGMBUBBLEMAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleGMBUBBLEMainMenuWindow ),    
    WIN_ID( MMIGMBUBBLEBOBBLE_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_BUBBLEBOBBLE, MMIGMBUBBLEBOBBLE_MAIN_MENU_CTRL_ID),
    END_WIN
};

// the window for new boxman game
WINDOW_TABLE( BUBBLEBOBBLE_NEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32) HandleBubblebobbleWinMsg ),    
        WIN_ID( BUBBLEBOBBLE_NEW_WIN_ID),
//#if (defined MMI_PDA_SUPPORT) ||( defined TOUCHPANEL_TYPE)
        WIN_HIDE_STATUS,
//#endif
	 //   CREATE_BUTTON_CTRL(IMAGE_BOXMAN_BUTTON, MMIGAME_BOXMAN_BUTTON_CTRL_ID),  
        CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMIGAME_BUBBLEBOBBLE_LABEL_SCORE_CTRL_ID),
        CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMIGAME_BUBBLEBOBBLE_LABEL_LEVEL_CTRL_ID),
    //   WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
        END_WIN
};

WINDOW_TABLE( BUBBLEBOBBLE_SELECT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleGMBUBBLESelectMenuWindow),    
    WIN_ID(BUBBLEBOBBLE_SELECT_WIN_ID),
    WIN_TITLE(TXT_GMBUBBLE_SELECT),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIGAME_BUBBLEBOBBLE_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIGAME_BUBBLEBOBBLE_FORM1_CTRL_ID,MMIGAME_BUBBLEBOBBLE_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIGAME_BUBBLEBOBBLE_LABEL1_CTRL_ID, MMIGAME_BUBBLEBOBBLE_FORM1_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 2,MMIGAME_BUBBLEBOBBLE_EDIT1_CTRL_ID, MMIGAME_BUBBLEBOBBLE_FORM1_CTRL_ID),	
// #ifdef MMI_PDA_SUPPORT
//         CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_COMMON_OK, TXT_NULL,MMICOMMON_SOFTKEY_CTRL_ID,MMIGAME_BUBBLEBOBBLE_FORM_CTRL_ID),
// #else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//#endif
    END_WIN
}; 

// 泡泡龙最高分查看
WINDOW_TABLE( BUBBLEBOBBLE_SCOREVIEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleBubbleScoreViewWinMsg ),    
    WIN_ID( BUBBLE_SCOREVIEW_WIN_ID ),
    WIN_TITLE( TXT_BUBBLE_HIGHSCORE ),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIGMBUBBLEBOBBLE_SCORE_CTRL_ID), 
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/******************************************************************************/
//  Description : sin
//  Global resource dependence : none
//  Author:  junxue.zhu 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL float SIN_S(int16 angle)
{
    angle %= 360;

    if(angle <= 90)
    {
        return _sin_0_90_table[angle];
    }
    else if (angle <= 180)
    {
        return _sin_0_90_table[180 - angle];
    }
    else if (angle <= 270)
    {
        return -_sin_0_90_table[angle - 180];
    }
    else if (angle <= 360)
    {
        return -_sin_0_90_table[360 - angle];
    }
    else 
    {
        return 0;
    }
}

/******************************************************************************/
//  Description : cos
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL float COS_S(int16 angle)
{
    angle %= 360;

    if (angle <= 90)
    {
        return _sin_0_90_table[90 - angle];
    }
    else if (angle <= 180)
    {
        return -_sin_0_90_table[angle - 90];
    }
    else if (angle <= 270)
    {
        return -_sin_0_90_table[270 - angle];
    }
    else if (angle <= 360)
    {
        return _sin_0_90_table[angle - 270];
    }
    else
    {
        return 0;
    }
    
}

/******************************************************************************/
//  Description : cos
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL MMI_RESULT_E InitDate(void)
{
    uint16 i = 0;
    for (i = 0;i<105;i++)
    {
        bubbleArray[i].myColor = 0;
        bubbleArray[i].myState = 0;
        bubbleArray[i].my_X = 0;
        bubbleArray[i].my_Y = 0;
        bubbleArray[i].row = 0;
        bubbleArray[i].column = 0;
        bubbleArray[i].speedX = 0;
        bubbleArray[i].speedY = 0;
    }

     for (i = 0;i<2;i++)
     {
        prepareBubble[i].myColor = 0;
        prepareBubble[i].myState = 0;
        prepareBubble[i].my_X = 0;
        prepareBubble[i].my_Y = 0;
        prepareBubble[i].row = 0;
        prepareBubble[i].column = 0;
        prepareBubble[i].speedX = 0;
        prepareBubble[i].speedY = 0;
     }
     prepareBubble[2] = prepareBubble[0];
     prepareBubble[3] = prepareBubble[1];

     movingBubble[0].myColor = 0;
     movingBubble[0].myState = 0;
     movingBubble[0].my_X = 0;
     movingBubble[0].my_Y = 0;
     movingBubble[0].row = 0;
     movingBubble[0].column = 0;
     movingBubble[0].speedX = 0;
     movingBubble[0].speedY = 0;

//     for (i = 0;i<30;i++)
//      {
//         dropArray[i].myColor = 0;
//         dropArray[i].myState = 0;
//         dropArray[i].my_X = 0;
//         dropArray[i].my_Y = 0;
//         dropArray[i].row = 0;
//         dropArray[i].column = 0;
//         dropArray[i].speedX = 0;
//         dropArray[i].speedY = 0;
//      }

    for (i = 0;i<30;i++)
     {
        popArray[i].myColor = 0;
        popArray[i].myState = 0;
        popArray[i].my_X = 0;
        popArray[i].my_Y = 0;
        popArray[i].row = 0;
        popArray[i].column = 0;
        popArray[i].speedX = 0;
        popArray[i].speedY = 0;
     }

    for (i = 0;i<9;i++)
    {
        row0[i] = 0;
    }
     creat_num = 0;
     //curScore = 0;//当前分数
     //curLevel = 1;//当前等级
     addScore = 0;//增加分数
     zero = 0;//顶行标志
     Y = 0;//挡板下降距离
     numOfColumns = 0;//最顶行的列数
     maxColumnNum = 0;//当前行最大列
     maxRowsNum = 0;//最大行
     bubbleNum = 0;
     curColorNum = 0;//当前颜色数量
     curLevel_LineCount = 0;//当前关卡，需要下降的泡泡的行数
     fall_LineCount = 0;//泡泡下降标志
     popingNum = 0;
     dropNum = 0;
     isNewLine = TRUE;//装载游戏控制
     scoreOf_one_bubble = 0;//本次消去，每个泡泡的分数
     shootRotation = 0;//发射台角度
     countCallBack = 0;//多少个单位时间
     flag_stop = TRUE;//计算速度只一次
     isShoot = FALSE;//很重要，在泡泡移动过程中一直为true，只有当它为false时，才可以发射泡泡
     isGo = FALSE;//与ishoot相似，在键盘的时候用到它控制泡泡的发射条件
     btrace = FALSE;//标志发射出去的泡泡是否已经碰撞
     flag_Win_or_Lose = FALSE;//进入胜利或者失败状态后不能发射球
}
/**********************************泡泡龙相关算法*********************************************/
/******************************************************************************/
//  Description : 获得随机数
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/
LOCAL int32 GetRand(int32 min,int32 max)
{
    int32  temp;
    if(min == max)
        return min;
    if(min > max)
    {
        int tempnum = min;
        min = max;
        max = tempnum;
    } 
    temp = rand();
    if(temp < 0)
        temp = temp * -1;
    return min + (temp%(max - min + 1));
}

/******************************************************************************/
//  Description : 得到绝对值
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/
LOCAL int32 _Abs(int32 value)
{
    if (value < 0)
        return -value;
    return value;
}
/******************************************************************************/
//  Description : 根据行列号，得到泡泡屏幕坐标
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_GetBubbleCenter(int32 c, int32 r, int32 * x, int32 * y)  
{
	if (numOfColumns == 9)//根据最顶行的列数，计算当前行的列数
	{
		if (r%2 == 0)
		{
			maxColumnNum = 9;
			*x = c*DIAMETER + RADIUS +X16;
		}
		else if (r%2 == 1)
		{
			maxColumnNum = 8;
			*x = (c+1)*DIAMETER +X16;
		}
	}
	else if (numOfColumns == 8)
	{
		if (r%2 == 0)
		{
			maxColumnNum = 8;
			*x = (c+1)*DIAMETER +X16;
		} 
		else if(r%2 == 1)
		{
			maxColumnNum = 9;
			*x = c*DIAMETER + RADIUS +X16;
		}
	}
    *y = r*(DIAMETER-DISY) + RADIUS + Y86+zero*(DIAMETER-DISY);
}

/******************************************************************************/
//  Description : 根据屏幕坐标，得到泡泡行列号
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_GetBubblePosition(int32 * c, int32 * r, int32  x, int32  y)    
{
	*r = (y - RADIUS - (Y86+zero*(DIAMETER-DISY)))/(DIAMETER - DISY);

	if ((x - X16)%DIAMETER == 0)//这一行有8个泡泡
	{
		*c = (x - X16)/DIAMETER - 1;
	}
	else if ((x - X16)%DIAMETER == RADIUS) //这一行有9个泡泡
	{
		*c = (x - X16 - RADIUS)/DIAMETER;
	}
}

/******************************************************************************/
//  Description : 标记爆炸的泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL int32 MMIBUB_CountSameBubble(int32 c,int32 r,int32 color)
{
	int32 x = 0;
	int32 y = 0;
	uint16 i = 0;
	int32 count = 0;//爆炸泡泡数量

	for (i=0; i<bubbleNum; i++)
	{
		MMIBUB_GetBubblePosition(&bubbleArray[i].column, &bubbleArray[i].row, bubbleArray[i].my_X, bubbleArray[i].my_Y);

		if (bubbleArray[i].row > maxRowsNum)
		{
			maxRowsNum = bubbleArray[i].row;		//获取当前所有泡泡中，行数的最大值
		}
	}

	MMIBUB_GetBubbleCenter(c, r, &x, &y);//获取当前行的列数(maxColumnNum)，以及(c,r)位置泡泡的坐标

	if (c<0 || r<0 || r> maxRowsNum || c>= maxColumnNum)//所计算的泡泡出界
	{
	 	return 0 ;
	}

	for (i=0; i<bubbleNum; i++)
	{
		if (x==bubbleArray[i].my_X && y==bubbleArray[i].my_Y)//计算当前泡泡是泡泡数组中哪一个(合法)
		{
			if (bubbleArray[i].myState != BS_Normal)
			{
				return 0;
			}
			if (bubbleArray[i].myColor == color)
			{
				count++;
				bubbleArray[i].myState = BS_CanPop;
				MMIBUB_GetBubbleCenter(bubbleArray[i].column, bubbleArray[i].row, &bubbleArray[i].my_X, &bubbleArray[i].my_Y);

				if (maxColumnNum == 9)
				{
					count += MMIBUB_CountSameBubble(c-1, r-1, color);//左上
					count += MMIBUB_CountSameBubble(c  , r-1, color);//右上
					count += MMIBUB_CountSameBubble(c-1, r  , color);//左
					count += MMIBUB_CountSameBubble(c+1, r  , color);//右
					count += MMIBUB_CountSameBubble(c-1, r+1, color);//左下
					count += MMIBUB_CountSameBubble(c	, r+1, color);//右下
				}
				else if (maxColumnNum == 8)
				{
					count += MMIBUB_CountSameBubble(c	, r-1, color);//左上
					count += MMIBUB_CountSameBubble(c+1, r-1, color);//右上
					count += MMIBUB_CountSameBubble(c-1, r  , color);//左
					count += MMIBUB_CountSameBubble(c+1, r  , color);//右
					count += MMIBUB_CountSameBubble(c	, r+1, color);//左下
					count += MMIBUB_CountSameBubble(c+1, r+1, color);//右下
				}
			}
		}		
	}

	return count;
	
}

/******************************************************************************/
//  Description : 标记下坠泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL BOOLEAN MMIBUB_MarkDrop(int32 c,int32 r)     //markDrop
{
	int32 x = 0;
	int32 y = 0;
	uint16 i = 0;

	for (i=0; i<bubbleNum; i++)
	{
		MMIBUB_GetBubblePosition(&bubbleArray[i].column, &bubbleArray[i].row, bubbleArray[i].my_X, bubbleArray[i].my_Y);

		if (bubbleArray[i].row > maxRowsNum)
		{
			maxRowsNum = bubbleArray[i].row;	//获取当前所有泡泡中，行数的最大值
		}
	}
	MMIBUB_GetBubbleCenter(c, r, &x, &y);//获取当前行的列数，以及(c,r)位置泡泡的坐标

	if (r < 0)	//到顶部了，不会掉落，终止递归
	{
		return TRUE;
	}
	if (c<0 || r> maxRowsNum || c>= maxColumnNum)	//泡泡出界
	{
		return FALSE ;
	}
	for (i=0; i<bubbleNum; i++)
	{
		if (x==bubbleArray[i].my_X && y==bubbleArray[i].my_Y)
		{
			if (bubbleArray[i].myState == BS_CanNotDrop)//说明该区域已经检查过了
			{
				return TRUE;
			}
			if (bubbleArray[i].myState != BS_Normal)//这个泡泡是Poping状态
			{
				return FALSE;
			}

			bubbleArray[i].myState = BS_CanDrop;

			if (maxColumnNum == 8)// 8个泡泡
			{
				if( MMIBUB_MarkDrop( c, r-1 ) )//左上
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}				
				if( MMIBUB_MarkDrop( c+1, r-1 ) )//右上
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				if( MMIBUB_MarkDrop( c-1, r ) )//左
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				if( MMIBUB_MarkDrop( c+1, r ) )//右
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				if( MMIBUB_MarkDrop( c, r+1 ) )//左下
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				if( MMIBUB_MarkDrop( c+1, r+1 ) )//右下
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
			} //if (maxColumnNum == 8)// 8个泡泡
			else if(maxColumnNum == 9)
			{
				if( MMIBUB_MarkDrop( c-1, r-1 ) )//左上
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				
				if( MMIBUB_MarkDrop( c, r-1 ) )//右上
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				if( MMIBUB_MarkDrop( c-1, r ) )//左
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				if( MMIBUB_MarkDrop( c+1, r ) )//右
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				if( MMIBUB_MarkDrop( c-1, r+1 ) )//左下
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
				if( MMIBUB_MarkDrop( c, r+1 ) )//右下
				{
					bubbleArray[i].myState = BS_CanNotDrop;
					return TRUE;
				}
			}//else if(maxColumnNum == 9)
		}//if (x==bubbleArray[i].my_X && y==bubbleArray[i].my_Y)
	}

	return FALSE;
}

/******************************************************************************/
//  Description : 根据新发射上来的泡泡，标记坠落的泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_DropBubble()     //dropBubble
{
	uint16 i = 0;

	for (i=0; i<bubbleNum; i++)
	{
		if (bubbleArray[i].myState == BS_CanPop)
		{
			//测试会爆破的泡泡周围的泡泡是否有挂点

			MMIBUB_GetBubblePosition(&bubbleArray[i].column, &bubbleArray[i].row, bubbleArray[i].my_X, bubbleArray[i].my_Y);
			MMIBUB_GetBubbleCenter(bubbleArray[i].column, bubbleArray[i].row, &bubbleArray[i].my_X, &bubbleArray[i].my_Y);

			if (maxColumnNum == 8)
			{
				MMIBUB_MarkDrop(bubbleArray[i].column	, bubbleArray[i].row-1);//左上
				MMIBUB_MarkDrop(bubbleArray[i].column+1, bubbleArray[i].row-1);//右上
				MMIBUB_MarkDrop(bubbleArray[i].column-1, bubbleArray[i].row);//左
				MMIBUB_MarkDrop(bubbleArray[i].column+1, bubbleArray[i].row);//右
				MMIBUB_MarkDrop(bubbleArray[i].column	, bubbleArray[i].row+1);//左下
				MMIBUB_MarkDrop(bubbleArray[i].column+1, bubbleArray[i].row+1);//右下
			}
			else if (maxColumnNum == 9)
			{
				MMIBUB_MarkDrop(bubbleArray[i].column-1, bubbleArray[i].row-1);//左上
				MMIBUB_MarkDrop(bubbleArray[i].column	, bubbleArray[i].row-1);//右上
				MMIBUB_MarkDrop(bubbleArray[i].column-1, bubbleArray[i].row);//左
				MMIBUB_MarkDrop(bubbleArray[i].column+1, bubbleArray[i].row);//右
				MMIBUB_MarkDrop(bubbleArray[i].column-1, bubbleArray[i].row+1);//左下
				MMIBUB_MarkDrop(bubbleArray[i].column	, bubbleArray[i].row+1);//右下
			}
		}
	}

}
/******************************************************************************/
//  Description : 生成新泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL MMI_GAME_BUBBLE_T MMIBUB_NewBubble(int32 color, int32 x, int32 y, int32 bubbleState)    //newBubble
{
	
	MMI_GAME_BUBBLE_T bubble;

	bubble.myColor = color;
	bubble.my_X = x +X16;
	bubble.my_Y = y + Y86;
	bubble.speedX = 0;
	bubble.speedY = 0;
	bubble.row = 0;
	bubble.column = 0;
	bubble.myState = bubbleState;
	
	return bubble;	
}

/******************************************************************************/
//  Description :新的一个随机行
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_AddBubbleToArray(void)     //addBubbleToArray
{
	int32 c = 0;
	uint16 i = 0;
    uint8 curColorMaxNum = 7;
    
	if (numOfColumns == 8)
	{
		numOfColumns = 9;
	}
	else
	{
		numOfColumns = 8;
	}
	for (i=0; i<bubbleNum; i++)//之前已经存在的所有泡泡都要下降一行
	{
		bubbleArray[i].my_Y += (DIAMETER-DISY);
	}
	for (c=0; c<numOfColumns; c++)//生成的新的一行，放在最顶端，放到bubbleArray里面
	{
		if (numOfColumns == 8)
		{
			bubbleArray[bubbleNum+c] = MMIBUB_NewBubble(GetRand(0, curColorNum), (c+1)*DIAMETER, RADIUS,BS_Normal);

		} 
		else if(numOfColumns ==9)
		{
			bubbleArray[bubbleNum+c] = MMIBUB_NewBubble(GetRand(0, curColorNum), RADIUS+c*DIAMETER, RADIUS,BS_Normal);	
		}
	}

	bubbleNum += numOfColumns;
	
}

/******************************************************************************/
//  Description :重置泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_ReSetBubble(MMI_GAME_BUBBLE_T * bubble)     //reSetBubble
{
	bubble->column = 0;
	bubble->row = 0;
	bubble->my_X = 0;
	bubble->my_Y = 0;
	bubble->myColor = 0;
	bubble->myState = BS_NotUse;
	bubble->speedX = 0;
	bubble->speedY = 0;
}

/******************************************************************************/
//  Description :重置dropArray，并返回当前是否还有正在下降的泡泡，
//                     如果有正在下降的泡泡，返回TRUE，否则返回FALSE
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
// LOCAL BOOLEAN MMIBUB_IsHaveDroping(void)      //isHaveDroping
// {
// 	uint16 i = 0;
// 	BOOLEAN isHaveDrop = FALSE;
// 
// 	for (i=0; i<dropNum; i++)
// 	{
// 		if(dropArray[i].my_Y >= MAX_Y+2*RADIUS)
// 		{
// 			MMIBUB_ReSetBubble(&dropArray[i]);
// 		}
// 		if (dropArray[i].my_Y < MAX_Y+RADIUS)
// 		{
// 			if (dropArray[i].myState == BS_Droping)
// 			{
// 				isHaveDrop = TRUE;
// 				break;
// 			}
// 		}		
// 	}
// 
// 	return isHaveDrop;
// 	
// }

/******************************************************************************/
//  Description :预备泡泡的生成
//                    向moving泡泡的转换，以及大小两种预备泡泡的交替
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_ReadyBubbleBuffer(void)     //readyBubbleBuffer
{
    uint8 curColorMaxNum = 7;
	if (prepareBubble[1].myState == BS_NotUse)//最小预备泡泡为空，进行初始化
	{
		prepareBubble[1] = MMIBUB_NewBubble(GetRand(0, curColorNum),SMALL_BUBBLE_X-X16,SMALL_BUBBLE_Y-Y86,BS_Ready);
		prepareBubble[0] = MMIBUB_NewBubble(GetRand(0, curColorNum),OFFSETX-X16,OFFSETY-Y86,BS_Ready);
		movingBubble[0] = MMIBUB_NewBubble(prepareBubble[0].myColor,OFFSETX-X16,OFFSETY-Y86,BS_Normal);
	}
	else if (prepareBubble[1].myState != BS_NotUse )//游戏中，泡泡的交替变化
	{
		if (movingBubble[0].myState == BS_Move)//
		{
			movingBubble[0] = MMIBUB_NewBubble(prepareBubble[0].myColor,OFFSETX-X16,OFFSETY-Y86,BS_Normal);
			prepareBubble[0] = MMIBUB_NewBubble(prepareBubble[1].myColor,OFFSETX-X16,OFFSETY-Y86,BS_Ready);
			prepareBubble[1] = MMIBUB_NewBubble(GetRand(0, curColorNum),SMALL_BUBBLE_X-X16,SMALL_BUBBLE_Y-Y86,BS_Ready);
			movingBubble[0].myState = BS_Moving;//状态改变，以免两个预备泡泡一直变换颜色
		}
	}

}

/******************************************************************************/
//  Description :泡泡消去后重新排序
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_ResortBubbleArray(void)    //resortBubbleArray
{
	uint16 i = 0;
	uint16 j = 0;
	uint8 count = 0;

	//清除泡泡数组中的无用元素

	for (i=0; i<bubbleNum; i++)
	{
		if (bubbleArray[i].my_X < 0 || bubbleArray[i].my_X > MAX_X 
			|| bubbleArray[i].my_Y < 0 || bubbleArray[i].my_Y >MAX_Y
			|| bubbleArray[i].myState != BS_Normal)
		{
			MMIBUB_ReSetBubble(&bubbleArray[i]);
		}
		if (bubbleArray[i].myState == BS_NotUse)//查找数组中为空的元素
		{
			for (j=i+1; j<bubbleNum; j++)
			{
				if (bubbleArray[j].myState == BS_Normal)//将空白元素后边第一个不为空的元素赋给他
				{
					bubbleArray[i] = bubbleArray[j];
					MMIBUB_ReSetBubble(&bubbleArray[j]);
					break;
				}
			}
		}
	}

	//清除空元素后，重新计算泡泡数量

	for (i=0; i<bubbleNum; i++)
	{
		if (bubbleArray[i].myState == BS_Normal)
		{
			count++;
		}
	}

	bubbleNum = count;
	
}
/************************************************************************/
/*               碰撞检测                                               */
/************************************************************************/
/******************************************************************************/
//  Description :判定移动泡泡碰撞后停靠的位置是否合法
//                     其中，x,y 是移动泡泡碰撞到的那个普通泡泡的坐标
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL BOOLEAN MMIBUB_IsRightPosition(int32 x,int32 y)     //isRightPosition
{
	uint16 i = 0;

	if(x < X16+RADIUS || x > X16 + 8*DIAMETER + RADIUS || y < RADIUS + Y86+zero*(DIAMETER-DISY))//停靠位置出界
	{
		return FALSE;
	}

	for (i=0; i<bubbleNum; i++)
	{
		if (bubbleArray[i].my_X == x && bubbleArray[i].my_Y == y)//停靠位置已有泡泡
		{
			return FALSE;
		}
	}

	return TRUE;
}
/******************************************************************************/
//  Description :和泡泡碰撞停下后的位置
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL BOOLEAN MMIBUB_BubbleStop(MMI_GAME_BUBBLE_T bubble)      //bubbleStop                              
{															
	if (_Abs(movingBubble[0].my_Y - bubble.my_Y) <= RADIUS/2-1)	//停在左右的情况			   
	{
		//x坐标
		if (movingBubble[0].my_X <= bubble.my_X)
		{
			if (MMIBUB_IsRightPosition(bubble.my_X-DIAMETER,bubble.my_Y))//左边是否可以停
			{
				movingBubble[0].my_X = bubble.my_X - DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-RADIUS,bubble.my_Y+DIAMETER-DISY))//左下是否可以停
			{
				//停左下
				movingBubble[0].my_X = bubble.my_X - RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+RADIUS,bubble.my_Y+DIAMETER-DISY))//右下是否可以停靠
			{
				//停右下
				movingBubble[0].my_X = bubble.my_X + RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+DIAMETER,bubble.my_Y))//右边是否可以停
			{
				movingBubble[0].my_X = bubble.my_X + DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}
		}
		else if (movingBubble[0].my_X > bubble.my_X)
		{
			if (MMIBUB_IsRightPosition(bubble.my_X+DIAMETER,bubble.my_Y))//右边是否有球
			{
				movingBubble[0].my_X = bubble.my_X + DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+RADIUS,bubble.my_Y+DIAMETER-DISY))//右下是否有球
			{
				//停右下
				movingBubble[0].my_X = bubble.my_X + RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-RADIUS,bubble.my_Y+DIAMETER-DISY))//左下是否可以停
			{
				//停左下
				movingBubble[0].my_X = bubble.my_X - RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-DIAMETER,bubble.my_Y))//左边是否可以停
			{
				movingBubble[0].my_X = bubble.my_X - DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}

		}
	} 
	else if(movingBubble[0].my_Y - bubble.my_Y > RADIUS/2-1)//左下和右下的情况
	{
		//x坐标
		if (movingBubble[0].my_X <= bubble.my_X)
		{
			if (MMIBUB_IsRightPosition(bubble.my_X-RADIUS,bubble.my_Y+DIAMETER-DISY))//左下是否可以停
			{
				//停左下
				movingBubble[0].my_X = bubble.my_X - RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+RADIUS,bubble.my_Y+DIAMETER-DISY))//右下是否可以停
			{
				//停右下
				movingBubble[0].my_X = bubble.my_X + RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-DIAMETER,bubble.my_Y))//左边是否可以停
			{
				movingBubble[0].my_X = bubble.my_X - DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+DIAMETER,bubble.my_Y))//右边是否有球
			{
				movingBubble[0].my_X = bubble.my_X + DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}
		}
		else if (movingBubble[0].my_X > bubble.my_X)
		{
			if (MMIBUB_IsRightPosition(bubble.my_X+RADIUS,bubble.my_Y+DIAMETER-DISY))//右下是否可以停
			{
				//停右下
				movingBubble[0].my_X = bubble.my_X + RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-RADIUS,bubble.my_Y+DIAMETER-DISY))
			{
				//停左下
				movingBubble[0].my_X = bubble.my_X - RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+DIAMETER,bubble.my_Y))//右边是否有球
			{
				movingBubble[0].my_X = bubble.my_X + DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-DIAMETER,bubble.my_Y))//左边是否可以停
			{
				movingBubble[0].my_X = bubble.my_X - DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}

		}
	}
	else if (movingBubble[0].my_Y - bubble.my_Y < 1-RADIUS/2)//左上和右上的情况
	{
		//x坐标
		if (movingBubble[0].my_X <= bubble.my_X)
		{
			if (MMIBUB_IsRightPosition(bubble.my_X-RADIUS,bubble.my_Y-DIAMETER+DISY))//左上是否可以停
			{
				//停左上
				movingBubble[0].my_X = bubble.my_X - RADIUS;
				movingBubble[0].my_Y = bubble.my_Y-DIAMETER+DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-DIAMETER,bubble.my_Y))//左边是否可以停
			{
				//停左边
				movingBubble[0].my_X = bubble.my_X - DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-RADIUS,bubble.my_Y+DIAMETER-DISY))
			{
				//停左下
				movingBubble[0].my_X = bubble.my_X - RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+RADIUS,bubble.my_Y+DIAMETER-DISY))
			{
				//停右下
				movingBubble[0].my_X = bubble.my_X + RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+DIAMETER,bubble.my_Y))//右边是否有球
			{
				movingBubble[0].my_X = bubble.my_X + DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}

		}
		else if (movingBubble[0].my_X > bubble.my_X)
		{
			// ************mark*************************

			if (MMIBUB_IsRightPosition(bubble.my_X+RADIUS,bubble.my_Y-DIAMETER+DISY))//右上是否可以停
			{
				//停右上
				movingBubble[0].my_X = bubble.my_X + RADIUS;
				movingBubble[0].my_Y = bubble.my_Y-DIAMETER+DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+DIAMETER,bubble.my_Y))//右边是否可以停
			{
				//停右边
				movingBubble[0].my_X = bubble.my_X + DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X+RADIUS,bubble.my_Y+DIAMETER-DISY))
			{
				//停右下
				movingBubble[0].my_X = bubble.my_X + RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-RADIUS,bubble.my_Y+DIAMETER-DISY))
			{
				//停左下
				movingBubble[0].my_X = bubble.my_X - RADIUS;
				movingBubble[0].my_Y = bubble.my_Y + DIAMETER - DISY;
				return TRUE;
			}
			else if (MMIBUB_IsRightPosition(bubble.my_X-DIAMETER,bubble.my_Y))//左边是否可以停
			{
				movingBubble[0].my_X = bubble.my_X - DIAMETER;
				movingBubble[0].my_Y = bubble.my_Y;
				return TRUE;
			}

		}
	}
	return FALSE;
}

/******************************************************************************/
//  Description :泡泡停在最顶行的位置
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_FirstRowPosition()     //firstRowPosition
{
	int32 dist = 0;
	int32 minDist = 10000;
	uint16 i = 0;

	if (numOfColumns == 8)
	{
		for (i=0; i<numOfColumns; i++)
		{
			row0[i] = (i+1)*DIAMETER + X16;//由最顶行的泡泡数目，来确定最顶行泡泡的坐标
		}	
	} 
	else if(numOfColumns == 9)
	{
		for (i=0; i<numOfColumns; i++)
		{
			row0[i] = i*DIAMETER + X16 + RADIUS;
		}
	}

	for (i=0; i<numOfColumns; i++)
	{
		dist = (movingBubble[0].my_X - row0[i])*(movingBubble[0].my_X - row0[i])
			 + (movingBubble[0].my_Y - (Y86+zero*(DIAMETER-DISY) + RADIUS))*(movingBubble[0].my_Y - (Y86+zero*(DIAMETER-DISY) + RADIUS));

		if (dist < minDist)
		{
			minDist = dist;//找出最小距离
		}
	}
	for (i=0; i<numOfColumns; i++)
	{
		dist = (movingBubble[0].my_X - row0[i])*(movingBubble[0].my_X - row0[i])
			+ (movingBubble[0].my_Y - (Y86+zero*(DIAMETER-DISY) + RADIUS))*(movingBubble[0].my_Y - (Y86+zero*(DIAMETER-DISY) + RADIUS));

		if (dist == minDist)
		{
			movingBubble[0].my_X = row0[i];				//由最小距离，来给movingBubble[0]确定坐标
			movingBubble[0].my_Y = (RADIUS + Y86+zero*(DIAMETER-DISY));
			break;
		}
	}
}

/******************************************************************************/
//  Description :碰撞检测
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN MMIBUB_Collide(void)    //collide
{
	if (movingBubble[0].myState == BS_Moving)
	{
/******************************************* 泡泡之间的碰撞 *********************************/
		uint16 i = 0;

		for (i=0; i<bubbleNum; i++)
		{			
			int dis = (movingBubble[0].my_X - bubbleArray[i].my_X)*(movingBubble[0].my_X - bubbleArray[i].my_X) 
					+ (movingBubble[0].my_Y - bubbleArray[i].my_Y)*(movingBubble[0].my_Y - bubbleArray[i].my_Y);

			if (dis <= (DIAMETER-BUBBLE_SPACE)*(DIAMETER-BUBBLE_SPACE))
			{
				movingBubble[0].speedX = movingBubble[0].speedY = 0;
				flag_stop = TRUE;
				btrace = TRUE;
				isShoot = FALSE;
				isGo = FALSE;

				if (MMIBUB_BubbleStop(bubbleArray[i]) && movingBubble[0].myState == BS_Moving)
				{
					movingBubble[0].myState = BS_Normal;
					MMIBUB_GetBubblePosition(&movingBubble[0].column, &movingBubble[0].row, movingBubble[0].my_X, movingBubble[0].my_Y);
					/* 这一段老出问题 **/
					bubbleArray[bubbleNum] = movingBubble[0];
					bubbleNum += 1;

					movingBubble[0].myState = BS_Move;
					movingBubble[0].my_X = -200;
					movingBubble[0].my_Y = -200;
					
					return btrace;
				}

			}			
		}
/*********** ********************************和墙的碰撞 ***********************************************/
		//到达最高处,
		if (movingBubble[0].my_Y - RADIUS <= Y86+zero*(DIAMETER-DISY))
		{
			movingBubble[0].speedX = movingBubble[0].speedY = 0;
			flag_stop = TRUE;
			btrace = TRUE;
			isShoot = FALSE;
			isGo = FALSE;
			
			if (movingBubble[0].myState == BS_Moving)
			{
				movingBubble[0].myState = BS_Normal;
				MMIBUB_FirstRowPosition();
				MMIBUB_GetBubblePosition(&movingBubble[0].column, &movingBubble[0].row, movingBubble[0].my_X, movingBubble[0].my_Y);
				bubbleArray[bubbleNum] = movingBubble[0];
				bubbleNum += 1;
				
				movingBubble[0].myState = BS_Move;
				movingBubble[0].my_X = -200;
				movingBubble[0].my_Y = -200;
				
				return btrace;
			}
		}
		//左边墙
		if (movingBubble[0].my_X - RADIUS <= X16)
		{
			//X方向速度反向
			movingBubble[0].speedX = -1.0*(int)movingBubble[0].speedX;
		}
		//右边墙
		if (movingBubble[0].my_X + RADIUS >= X305)
		{
			//X方向速度反向
			movingBubble[0].speedX = -1.0*(int)movingBubble[0].speedX ;
		}
	}//if (movingBubble[0].myState == BS_Moving)

	return btrace;	
}

/******************************************************************************/
//  Description :无挂点泡泡下降(新buffer实现)
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
// LOCAL void MMIBUB_DrawDroping(uint16 dropCnt)    //drawDroping
// {
// 	uint16 i = 0;
// 
// 	for (i=0; i<dropCnt; i++)
// 	{
// 		if (dropArray[i].myState == BS_Droping)
// 		{
// 			if (dropArray[i].my_Y <= DEAD_LINE)
// 			{
// 				drawBigBubble(dropArray[i]);
// 			}
// 			dropArray[i].my_Y += DROP_SPEED;
// 
// 			if (dropArray[i].my_Y >= MAX_Y+RADIUS)
// 			{
// 				MMIBUB_ReSetBubble(&dropArray[i]);
// 			}
// 		}
// 	}
// }

/******************************************************************************/
//  Description :移动准备好的泡泡
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_MoveBubble()       //moveBubble
{
    if (isGo)
    {
        isGo = FALSE;			
        movingBubble[0].myState = BS_Moving;		

        if (shootRotation == 0)//根据发射角，计算泡泡x,y 方向的移动速度
        {
            movingBubble[0].speedX = 0;
            movingBubble[0].speedY = -SHOOT_SPEED;
        }
        else if (shootRotation < 0)
        {
				
            movingBubble[0].speedX = (int16)SHOOT_SPEED*COS_S(90-shootRotation);
				
            movingBubble[0].speedY = -(int16)SHOOT_SPEED*SIN_S(90-shootRotation);
        }
        else if (shootRotation > 0)
        {
            movingBubble[0].speedX = (int16)SHOOT_SPEED*COS_S(90-shootRotation);	
            movingBubble[0].speedY = -(int16)SHOOT_SPEED*SIN_S(90-shootRotation);
        }
    }

}

/******************************************************************************/
//  Description :使泡泡爆炸
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL uint16 MMIBUB_PopBubble(void)     //popBubble
{
	uint16 i = 0;
	uint16 count = 0;
	if (btrace)		//发射并与球碰撞后才统计消去与下坠泡泡
	{		
	
		btrace = FALSE;	
		popingNum = 0;
		dropNum = 0;

		//碰撞后标志泡泡数组中所有泡泡状态
        
		count = MMIBUB_CountSameBubble(bubbleArray[bubbleNum-1].column, bubbleArray[bubbleNum-1].row, bubbleArray[bubbleNum-1].myColor);

		if (count < 3)//碰撞的泡泡周围，相同颜色泡泡小于3
		{
			for (i=0; i<bubbleNum; i++)
			{
				bubbleArray[i].myState = BS_Normal;
			}
			return;
		}
		else if(count >= 3)
		{
			MMIBUB_DropBubble();//标记需要坠落的泡泡
			addScore = 0;
			
			for (i=0; i<bubbleNum; i++)						/*碰撞后达到消去条件后的动作*/
			{
				if (bubbleArray[i].myState == BS_CanPop)
				{
					bubbleArray[i].myState = BS_Poping;
					//popArray[popingNum] = bubbleArray[i];//爆炸泡泡添加到爆炸数组
                    popArray[popingNum].myColor = bubbleArray[i].myColor;
                //    bubbleArray[i].myState = 0;
                    popArray[popingNum].myState = bubbleArray[i].myState;
                //    bubbleArray[i].my_X = 0;
                    popArray[popingNum].my_X = bubbleArray[i].my_X;
                //    bubbleArray[i].my_Y = 0;
                    popArray[popingNum].my_Y = bubbleArray[i].my_Y;
                //    bubbleArray[i].row = 0;
                    popArray[popingNum].row = bubbleArray[i].row;
                //    bubbleArray[i].column = 0;
                    popArray[popingNum].column = bubbleArray[i].column;
                //    bubbleArray[i].speedX = 0;
                    popArray[popingNum].speedX = bubbleArray[i].speedX;
                //    bubbleArray[i].speedY = 0;
                    popArray[popingNum].speedY = bubbleArray[i].speedY;
					popingNum ++;
					MMIBUB_ReSetBubble(&bubbleArray[i]);
				}
// 				if (bubbleArray[i].myState == BS_CanDrop)
// 				{
// 					bubbleArray[i].myState = BS_Droping;
// 					dropArray[dropNum] = bubbleArray[i];//坠落泡泡 添加到坠落数组
// 					dropNum++;
// 					MMIBUB_ReSetBubble(&bubbleArray[i]);
// 				}
				if (bubbleArray[i].myState == BS_CanNotDrop)
				{
					bubbleArray[i].myState = BS_Normal;
				}    
			}
			
			if (count+dropNum <= 4)							/*			积分		*/
			{
				scoreOf_one_bubble = 10;
			}
			else if (count+dropNum > 4 && count+dropNum <= 10)
			{
				scoreOf_one_bubble = 20;
			}
			else if (count+dropNum > 10 && count+dropNum <= 16)
			{
				scoreOf_one_bubble = 30;
			}

			else if (count+dropNum > 16 && count+dropNum <= 22)
			{
				scoreOf_one_bubble = 40;
			}

			else if (count+dropNum >22 && count+dropNum <= 28)
			{
				scoreOf_one_bubble = 50;
			}

			else if (count+dropNum > 28)
			{
				scoreOf_one_bubble = 60;
			}
			addScore = (count+dropNum)*scoreOf_one_bubble;
			curScore += addScore;//当前得分
		}
	}
    MMIBUB_ResortBubbleArray();
    return count;
}

/******************************************************************************/
//  Description :装载游戏
//  Global resource dependence : none
//  Author:  junxue.zhu
//  Note:  
/*****************************************************************************/ 
LOCAL void MMIBUB_InitGame(void)    //initGame
{
	uint16 i = 0;
	if (isNewLine)
	{
		isNewLine = FALSE;

		switch(curLevel)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			curColorNum = 2;

			switch(curLevel)
			{
				case 1:
					curLevel_LineCount = 5;
					break;
				case 2:
					curLevel_LineCount = 10;
					break;
				case 3:
					curLevel_LineCount = 15;
					break;
				case 4:
					curLevel_LineCount = 20;
					break;
				default:
					break;
			}

			for (i=0; i<3; i++)
			{
				MMIBUB_AddBubbleToArray(i);
			}
            MMIBUB_ReadyBubbleBuffer();
            prepareBubble[2] = prepareBubble[0];
            prepareBubble[3] = prepareBubble[1];
			break;
		case 5:
		case 6:
		case 7:
		case 8:
			curColorNum = 3;

			switch(curLevel)
			{
				case 5:
					curLevel_LineCount = 5;
					break;
				case 6:
					curLevel_LineCount = 10;
					break;
				case 7:
					curLevel_LineCount = 15;
					break;
				case 8:
					curLevel_LineCount = 20;
					break;					
				default:
					break;
			}

			for (i=0; i<3; i++)
			{
				MMIBUB_AddBubbleToArray(i);
			}
            MMIBUB_ReadyBubbleBuffer();
            prepareBubble[2] = prepareBubble[0];
            prepareBubble[3] = prepareBubble[1];
			break;
		case 9:
		case 10:
		case 11:
		case 12:
			curColorNum = 4;

			switch(curLevel)
			{
				case 9:
					curLevel_LineCount = 5;
					break;
				case 10:
					curLevel_LineCount = 10;
					break;
				case 11:
					curLevel_LineCount = 15;
					break;
				case 12:
					curLevel_LineCount = 20;
					break;					
				default:
					break;
			}

			for (i=0; i<3; i++)
			{
				MMIBUB_AddBubbleToArray(i);
			}
            MMIBUB_ReadyBubbleBuffer();
            prepareBubble[2] = prepareBubble[0];
            prepareBubble[3] = prepareBubble[1];
			break;
		case 13:
		case 14:
		case 15:
		case 16:
			curColorNum = 5;

			switch(curLevel)
			{
				case 13:
					curLevel_LineCount = 5;
					break;
				case 14:
					curLevel_LineCount = 10;
					break;
				case 15:
					curLevel_LineCount = 15;
					break;
				case 16:
					curLevel_LineCount = 20;
					break;					
				default:
					break;
			}

			for (i=0; i<3; i++)
			{
				MMIBUB_AddBubbleToArray(i);
			}
            MMIBUB_ReadyBubbleBuffer();
            prepareBubble[2] = prepareBubble[0];
            prepareBubble[3] = prepareBubble[1];
			break;
		case 17:
                curColorNum = 6;
                curLevel_LineCount = 45;
                for (i=0; i<3; i++)
                {
                    MMIBUB_AddBubbleToArray(i);
                }
                MMIBUB_ReadyBubbleBuffer();
                prepareBubble[2] = prepareBubble[0];
                prepareBubble[3] = prepareBubble[1];
                break;
		default:
			break;
			
		}

	}
	
}

/*****************************************************************************/
//  FUNCTION:     HandleBubbleScoreViewWinMsg
//  Description :   查看最高分数
//  Global resource dependence : 
//  Author:       junxue.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBubbleScoreViewWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E        recode          = MMI_RESULT_TRUE;
    static uint16       high_score      = 0;
    uint16              nv_return       = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
     
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                // 读nv
                MMINV_READ(MMINV_BUBBLEBOBBLE_HIGH_SCORE, &high_score, nv_return);
                if (MN_RETURN_SUCCESS != nv_return)
                {
                    high_score = 0;
                    MMINV_WRITE(MMINV_BUBBLEBOBBLE_HIGH_SCORE,&high_score);
                }
                // 设置softkey
                GUIWIN_SetSoftkeyTextId(win_id, TXT_BUBBLE_RESET,TXT_NULL, STXT_RETURN,FALSE);
            }
            break;
        
        case MSG_FULL_PAINT:
            {
                // 画背景
                GUI_POINT_T startpoint = {0};
                startpoint.y = MMI_TITLE_HEIGHT;

                GUIRES_DisplayImg(&startpoint,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_COMMON_BG,
                    &lcd_dev_info);
                // 画分数
                Bubble_ViewScore(win_id, MMIGMBUBBLEBOBBLE_SCORE_CTRL_ID, TXT_BUBBLE_HIGHSCORE, high_score);
            }
            break;
            
        case MSG_GET_FOCUS:
            break;
        
        case MSG_TIMER:
            break;
        
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            {
                // 提示分数重置
                MMIPUB_OpenQueryWinByTextId(TXT_BUBBLE_RESET,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            {
                MMK_CloseWin(win_id);
            }
            break;
        
        case MSG_PROMPTWIN_OK:
            {
                // 分数重置为0，并显示
                MMIPUB_CloseQuerytWin(PNULL);
                high_score = 0;
                MMINV_WRITE(MMINV_BUBBLEBOBBLE_HIGH_SCORE, ( int16*)&high_score);
                Bubble_ViewScore(win_id, MMIGMBUBBLEBOBBLE_SCORE_CTRL_ID, TXT_BUBBLE_HIGHSCORE, high_score);
            }
        break;
        
        case MSG_PROMPTWIN_CANCEL:
            {
                MMIPUB_CloseQuerytWin(PNULL);
            }
            break;
        
        default:
            {
                recode = MMI_RESULT_FALSE;
            }
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : the bubblebobble select level window
//  Global resource dependence : none
//  Author: Junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGMBUBBLESelectMenuWindow(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint8 default_map = 1;
    uint8 default_map_num[2+1] = {0};
    wchar default_map_num_wstr[2+1] = {0};
    MMI_STRING_T select_tip = {0};
    uint16 len = 0;
    uint8 duar_buf[8] = {0};
    MMI_STRING_T str_info = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMIGAME_BUBBLEBOBBLE_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
        
        MMI_GetLabelTextByLang(TXT_GMBUBBLEBOBBLE_SELECT_TIP, &select_tip);      
        GUILABEL_SetText(MMIGAME_BUBBLEBOBBLE_LABEL1_CTRL_ID,&select_tip,FALSE);
        MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), MMK_ConvertIdToHandle(MMIGAME_BUBBLEBOBBLE_EDIT1_CTRL_ID));
        s_input_map_num = (s_input_map_num==0)?default_map:s_input_map_num;
        sprintf((char*)default_map_num, "%d", s_input_map_num);        
        len = strlen((char*)default_map_num);
        MMI_STRNTOWSTR( default_map_num_wstr, 2, 
			default_map_num, 2, len );
        GUIEDIT_SetString(
            MMIGAME_BUBBLEBOBBLE_EDIT1_CTRL_ID,
            default_map_num_wstr,
            len
            );
        break;
        
    case MSG_LOSE_FOCUS:
        {
            GUIEDIT_GetString(MMIGAME_BUBBLEBOBBLE_EDIT1_CTRL_ID, &str_info);
            if ((0 < str_info.wstr_len) && (PNULL != str_info.wstr_ptr))
            {
                MMI_WSTRNTOSTR( duar_buf, 8, str_info.wstr_ptr, str_info.wstr_len, str_info.wstr_len );
                curLevel = atoi((char *)(duar_buf));
            }
        }
        break;
     case MSG_CTL_OK:
     case MSG_APP_OK:
     case MSG_CTL_MIDSK:
        GUIEDIT_GetString(MMIGAME_BUBBLEBOBBLE_EDIT1_CTRL_ID, &str_info);
        if ((0 < str_info.wstr_len) && (PNULL != str_info.wstr_ptr))
        {
            MMI_WSTRNTOSTR( duar_buf, 8, str_info.wstr_ptr, str_info.wstr_len, str_info.wstr_len );
            curLevel = atoi((char *)(duar_buf));
        }
        if(curLevel <= 17 && curLevel > 0)
        {
            MMK_CreateWin((uint32 *)BUBBLEBOBBLE_NEW_WIN_TAB,PNULL);
        }
        else
        {
            OpenBubbleErrorWin(TXT_NULL);
        }

        break;        
        
    case MSG_CTL_CANCEL: 
    case MSG_KEYDOWN_CANCEL:
        curLevel = 1;
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        s_input_map_num = 0;
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : malloc the memory
//  Global resource dependence : none
//  Author: Junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GameAllocMem(void)
{
        BOOLEAN ret = TRUE;
        bubbleArray=(MMI_GAME_BUBBLE_T *)SCI_ALLOC_APP(sizeof(MMI_GAME_BUBBLE_T)*105);
	    if(PNULL!=bubbleArray)
        {
           SCI_MEMSET((MMI_GAME_BUBBLE_T*)bubbleArray, 0,sizeof(MMI_GAME_BUBBLE_T)*105);
        }
        else
        {
           return FALSE;
        }

        prepareBubble=(MMI_GAME_BUBBLE_T *)SCI_ALLOC_APP(sizeof(MMI_GAME_BUBBLE_T)*4);
	    if(PNULL!=prepareBubble)
        {
           SCI_MEMSET((MMI_GAME_BUBBLE_T*)prepareBubble, 0,sizeof(MMI_GAME_BUBBLE_T)*4);
        }
        else
        {
           return FALSE;
        }

        movingBubble=(MMI_GAME_BUBBLE_T *)SCI_ALLOC_APP(sizeof(MMI_GAME_BUBBLE_T));
	    if(PNULL!=movingBubble)
        {
           SCI_MEMSET((MMI_GAME_BUBBLE_T*)movingBubble, 0,sizeof(MMI_GAME_BUBBLE_T));
        }
        else
        {
           return FALSE;
        }

//         dropArray=(MMI_GAME_BUBBLE_T *)SCI_ALLOC_APP(sizeof(MMI_GAME_BUBBLE_T)*30);
// 	    if(PNULL!=dropArray)
//         {
//            SCI_MEMSET((MMI_GAME_BUBBLE_T*)dropArray, 0,sizeof(MMI_GAME_BUBBLE_T)*30);
//         }
//         else
//         {
//            return FALSE;
//         }

        popArray=(MMI_GAME_BUBBLE_T *)SCI_ALLOC_APP(sizeof(MMI_GAME_BUBBLE_T)*30);
	    if(PNULL!=popArray)
        {
           SCI_MEMSET((MMI_GAME_BUBBLE_T*)popArray, 0,sizeof(MMI_GAME_BUBBLE_T)*30);
        }
        else
        {
           return FALSE;
        }

        return ret;
}
/*****************************************************************************/
//  Description :the bubblebobble window
//  Global resource dependence : none
//  Author: Junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBubblebobbleWinMsg( 
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_FONT_ALL_T      font_all   ={0};
    static uint8        game_timer_new_id = 0;
    static uint8        game_timer_fly_id = 0;
    static uint8        game_timer_process_id = 0;
    //GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16              i = 0;
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    uint16              image_width = 0;
    uint16              image_height = 0;
    GUI_RECT_T          disp_rect = {0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT};
    GUI_ALIGN_E         align_type = ALIGN_HVMIDDLE;
    uint16              font_color = MMI_BLACK_COLOR;
    GUI_FONT_T          s_FONT_SIZE = 0;
    GUISTR_STYLE_T      str_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T          rect={0};
    uint8               game_over = 0;
    MMI_STRING_T        string = {0};
    uint16 score = 0;
    uint16 nv_return = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_shooter, win_id);

//@CR NEWMS00182739
#if defined KEYPAD_TYPE_QWERTY_KEYPAD   
    uint8 keycode= (uint8)(msg_id & 0x000000ff); 
    keycode = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(keycode);
    msg_id = (msg_id & 0xffffff00) | (keycode & 0x000000ff);
#endif
//end

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
        GUI_RECT_T rect = {0};

        rect.left = 10;
        rect.top = 10;
        rect.right = 100;
        rect.bottom = 30;
        GUILABEL_SetRect(MMIGAME_BUBBLEBOBBLE_LABEL_SCORE_CTRL_ID, &rect, FALSE);
        
        rect.left = 180;
        rect.top = 10;
        rect.right = 220;
        rect.bottom = 30;
        GUILABEL_SetRect(MMIGAME_BUBBLEBOBBLE_LABEL_LEVEL_CTRL_ID, &rect, FALSE);
 
        curScore = 0;
        GameAllocMem();
        
        CreateBUBBLENeedleLayer(win_id);
        BUBBLEAppendLayer(&s_needle_dev_info);
        BUBBLECreateLayer(&s_lcd_dev_info,win_id);
       // CreateBUBBLENeedleLayer(win_id,s_lcd_dev_info);
        BUBBLEAppendLayer(&s_lcd_dev_info);
        
        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        game_timer_new_id=MMK_CreateWinTimer(win_id, 10000, TRUE);
        }
        break;
	
    case MSG_FULL_PAINT:
        InitDate();
        MMIBUB_InitGame();
        InitBubblebobbleWin(win_id, TRUE);
        break;

//按方向键左键，调整角度
    case MSG_APP_LEFT:
 //   case MSG_KEYDOWN_LEFT:
        if (shootRotation > -82)
			{
				shootRotation -= 5;
                //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL); 
                InitBubblebobbleWin(win_id, FALSE);
			}
        break;
//按方向键右键，调整角度
    case MSG_APP_RIGHT:
 //   case MSG_KEYDOWN_RIGHT:
        if (shootRotation < 82)
			{
				shootRotation += 5;
               // MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL); 
                InitBubblebobbleWin(win_id, FALSE);
			}
        break;
        
    case MSG_KEYDOWN_CANCEL:    
    case MSG_CTL_CANCEL:
  
        if(game_timer_fly_id != 0)
        {
          MMK_StopTimer(game_timer_fly_id);
          game_timer_fly_id = 0;
        }
         if(game_timer_new_id != 0)
        {
          MMK_StopTimer(game_timer_new_id);
          game_timer_new_id = 0;
        }
          if(game_timer_process_id != 0)
        {
          MMK_StopTimer(game_timer_process_id);
          game_timer_process_id = 0;
        }
        //InitDate();
        curLevel = 1;

        MMINV_READ(MMINV_BUBBLEBOBBLE_HIGH_SCORE, &score, nv_return);
        if (MN_RETURN_SUCCESS == nv_return)
        {
          if (score < curScore)
          MMINV_WRITE(MMINV_BUBBLEBOBBLE_HIGH_SCORE,&curScore);
        }

	    if(PNULL!=bubbleArray)
        {
           SCI_FREE(bubbleArray);
           bubbleArray = PNULL;
        }

        if(PNULL!=prepareBubble)
        {
           SCI_FREE(prepareBubble);
           prepareBubble = PNULL;
        }

        if(PNULL!=movingBubble)
        {
           SCI_FREE(movingBubble);
           movingBubble = PNULL;
        }

//         if(PNULL!=dropArray)
//         {
//            SCI_FREE(dropArray);
//            dropArray = PNULL;
//         }

        if(PNULL!=popArray)
        {
           SCI_FREE(popArray);
           popArray = PNULL;
        }

        MMK_CloseWin(win_id);
        break;

//按中键发射泡泡
    case MSG_KEYDOWN_WEB:	
    case MSG_CTL_MIDSK:
        {
          isGo = TRUE;
          btrace = FALSE;
          MMIBUB_MoveBubble();
         // shootRotation = 0;   
         game_timer_fly_id=MMK_CreateWinTimer(win_id, 100, TRUE);
        }
        break;
        
    case MSG_TIMER:
        if(game_timer_fly_id == *(uint8*)param)
        {
           movingBubble[0].my_X += movingBubble[0].speedX;
           movingBubble[0].my_Y += movingBubble[0].speedY;
           InitBubblebobbleWin(win_id, TRUE);
           MMIBUB_Collide();
           if(!btrace)
           {
            DrawBigBubble(win_id,movingBubble,1,BS_Moving);
           }
           else
           {
            MMK_StopTimer(game_timer_fly_id);
            if(MMIBUB_PopBubble()>=3);
            {
              game_timer_process_id=MMK_CreateWinTimer(win_id, 200, TRUE);
            }
            InitBubblebobbleWin(win_id, TRUE);
            DrawBombBubble(win_id,popArray,popingNum,BS_Poping);
            prepareBubble[2] = prepareBubble[0];
            prepareBubble[3] = prepareBubble[1];
            //btrace = FALSE;
            MMIBUB_ReadyBubbleBuffer();
           }         
        }
        else if((game_timer_process_id == *(uint8*)param))
        {
           InitBubblebobbleWin(win_id, FALSE);
           if(game_timer_process_id != 0)
           {
             MMK_StopTimer(game_timer_process_id);
             game_timer_process_id = 0;
           }
        }

        else if(game_timer_new_id == *(uint8*)param)
        {
           if (creat_num < curLevel_LineCount)
           {
                 MMIBUB_AddBubbleToArray();
                 InitBubblebobbleWin(win_id, FALSE); 
           }
           else
           {

					for (i=0; i<bubbleNum; i++)
					{
						bubbleArray[i].my_Y += (DIAMETER-DISY);//泡泡下降
					}
					zero++;//最顶行标志
					Y +=  (DIAMETER - DISY);//挡板下降

           }

            if (zero>=1)
           {
             board_start_point.x = X16;
             board_start_point.y = Y+DIAMETER;
             InitBubblebobbleWin(win_id, FALSE);
           }
           creat_num++;
        }
//判断游戏结束的条件
        for (i=0; i<bubbleNum; i++)
        {
            if (bubbleArray[i].my_Y >= lcd_height-image_height)
            {
                game_over = 1;
            }
        }

        if (0 != game_over)
        {
             MMIPUB_OpenQueryWinByTextId(TXT_BUBBLE_RESTART,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        }

//判断升级条件
        else if(0 == game_over && 0 == bubbleNum)
        {
             // uint8 i = 0;
              curLevel++;
             // i = curLevel;
              if (17 < curLevel)
              {
                 LCD_FillRect(&s_main_lcd_dev_info, disp_rect, MMI_GRAY_WHITE_COLOR);
                 s_FONT_SIZE = SONG_FONT_30;
                 MMI_GetLabelTextByLang(TXT_GAME_FINISH,&string);
                 rect.top =100 ;
                 rect.bottom = 200;
                 rect.left =20 ;
                 rect.right =220 ;
                 str_style.align = align_type;
                 str_style.font = s_FONT_SIZE;
                 str_style.font_color = font_color;
                 GUISTR_DrawTextToLCDInRect(&s_main_lcd_dev_info,
                                            &rect,      
                                            &rect,       
                                            (const MMI_STRING_T    *)&string,
                                            &str_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO);
                 MMK_CloseWin(win_id);
              }
              else
              {
              OpenThisLevelFinishedWin();
              MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
              }
        }

        break;

    case MSG_PROMPTWIN_OK:
         {
            // 提示框，按左键重新开始游戏
            MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
           // 背光常亮
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            MMIPUB_CloseQuerytWin(PNULL);
         }
         break;

    case MSG_PROMPTWIN_CANCEL:
         {
            // 按右键退出游戏
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
         }
         break;

    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        // 背光被允许关闭
        BUBBLEReleaseLayer(&s_lcd_dev_info);
        BUBBLEReleaseLayer(&s_needle_dev_info);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        break;
        
    case MSG_GET_FOCUS:
        // 背光常亮设置
         BUBBLEAppendLayer(&s_lcd_dev_info);
         BUBBLEAppendLayer(&s_needle_dev_info);
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);     
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/******************************************************************************/
//  Description :HandleGMBUBBLEMainMenuWindow
//  Global resource dependence : none
//  Author: junxue.zhu
//  Note:   i  
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGMBUBBLEMainMenuWindow(
  MMI_WIN_ID_T win_id, 
  MMI_MESSAGE_ID_E msg_id, 
  DPARAM param
  )
  
{
    uint32          group_id = 0; 
    uint32          menu_id  = 0;
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
#if defined KEYPAD_TYPE_QWERTY_KEYPAD
    if(MSG_KEYDOWN_UP <= msg_id && msg_id <= MSG_KEYDOWN_MENU)
    {
        uint8 keycode= (uint8)(msg_id & 0x000000ff); 
        keycode = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(keycode);
        msg_id = (msg_id & 0xffffff00) | (keycode & 0x000000ff);
    }
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
      //  MMK_SetAtvCtrl(win_id, MMIGMBUBBLEBOBBLE_MAIN_MENU_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIGMBUBBLEBOBBLE_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
        switch(menu_id) 
        {
        case ID_GMBUBBLEBOBBLE_NEW:
            MMK_CreateWin((uint32 *)BUBBLEBOBBLE_NEW_WIN_TAB,(ADD_DATA)(ID_GMBUBBLEBOBBLE_NEW));
            break;
            
        case ID_GMBUBBLEBOBBLE_SELECT:
            MMK_CreateWin((uint32 *)BUBBLEBOBBLE_SELECT_WIN_TAB, (ADD_DATA)(ID_GMBUBBLEBOBBLE_SELECT));
             break;
             
        case ID_GMBUBBLEBOBBLE_HIGHSCORE:
            MMK_CreateWin((uint32 *)BUBBLEBOBBLE_SCOREVIEW_WIN_TAB, (ADD_DATA)(ID_GMBUBBLEBOBBLE_HIGHSCORE));
             break;

        default:
            break;
        }
    break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : InitBubblebobbleWin
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void InitBubblebobbleWin(MMI_WIN_ID_T win_id, BOOLEAN is_refresh)
{
  //  GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint16 image_width = 0;
    uint16 image_height = 0;
    uint16 angle = 0;
  
    GUI_RECT_T          disp_rect = {0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT};

    LCD_FillRect(&s_main_lcd_dev_info, disp_rect, MMI_GRAY_WHITE_COLOR);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_shooter, win_id);
    
    DrawBigBubble(win_id,bubbleArray,bubbleNum,BS_Normal);  //画网格区的球
    if(is_refresh)
    {
        DrawBigBubble(win_id,&prepareBubble[0],1,BS_Ready);   //画发射器上的球
        DrawLittleBubble(win_id,&prepareBubble[1],1);
    }
    else
    {
        DrawBigBubble(win_id,&prepareBubble[2],1,BS_Ready);   //画发射器上的球
        DrawLittleBubble(win_id,&prepareBubble[3],1);
    }
    ShowScore(win_id);
    ShowLevel(win_id);
    if (zero>=1)
    {
      GUIRES_DisplayImg(&board_start_point,NULL,NULL,win_id,IMAGE_dangban,&s_main_lcd_dev_info);
    }
    if (shootRotation < 0)
    {
          angle = 360 - _Abs(shootRotation);
    }
    else
    {
          angle = shootRotation;

    }
      ChangeShooterAngle(win_id,angle);
}

/*****************************************************************************/
//  FUNCTION:     Bubble_ViewScore
//  Description :   显示分数
//  Global resource dependence : 
//  Author:       junxue.zhu
//  Note:   
/*****************************************************************************/
LOCAL void Bubble_ViewScore(
                      MMI_WIN_ID_T win_id,
                      MMI_CTRL_ID_T ctrl_id, 
                      MMI_TEXT_ID_T  title_id,
                      uint16 score)
{
    uint8 asc_str[12] = {0};
    wchar wstr[12] = {0};
    MMI_STRING_T  str2 = {0};
    MMI_STRING_T  temp_str = {0};
    MMI_STRING_T  result = {0};

    MMI_GetLabelTextByLang(title_id, &temp_str);
    sprintf((char*)asc_str,":%d",score);
    str2.wstr_len = strlen((char*)asc_str);     
    str2.wstr_ptr = wstr;
    MMI_STRNTOWSTR( wstr, 12, asc_str, 12, str2.wstr_len );
    result = MMIGMBUBBLE_AddTwoString(temp_str,str2);
    if (PNULL == result.wstr_ptr || 0 == result.wstr_len)
    {
        return;
    }
    GUILABEL_SetFont(ctrl_id, MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
    GUILABEL_SetText(ctrl_id, &result, FALSE);

    SCI_FREE(result.wstr_ptr);

}

/*****************************************************************************/
//  Description : DrawBigBubble
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void DrawBigBubble(MMI_WIN_ID_T win_id,MMI_GAME_BUBBLE_T *BubbleArray,uint16 BubbleNum,MMI_GAME_BUBBLESTATE State)
{
    if(NULL==BubbleArray)

        return;

    else
    {
    uint16 i=0;
    GUI_POINT_T point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    for (i=0; i<BubbleNum; i++)
	{
       if (State==(BubbleArray+i)->myState)
       {
        if (BC_BLUE==(BubbleArray+i)->myColor)
        {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_blue_big, 
                              &lcd_dev_info);
        }

        else if (BC_GRAY==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_gray_big, 
                              &lcd_dev_info);
        }

        else if (BC_GREEN==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_green_big, 
                              &lcd_dev_info);
        }

        else if (BC_ORANGE==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_orange_big, 
                              &lcd_dev_info);
        }

        else if (BC_PINK==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_pink_big, 
                              &lcd_dev_info);
        }

        else if (BC_RED==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_red_big, 
                              &lcd_dev_info);
        }

        else if (BC_YELLOW==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_yellow_big, 
                              &lcd_dev_info);
        }
       }	
	}
    }

}

/*****************************************************************************/
//  Description : DrawLittleBubble
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void DrawLittleBubble(MMI_WIN_ID_T win_id,MMI_GAME_BUBBLE_T *BubbleArray,uint16 BubbleNum)
{
    if(NULL==BubbleArray)

        return;

    else
    {
    uint16 i=0;
    GUI_POINT_T point = {0};
   // GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    for (i=0; i<BubbleNum; i++)
	{

        if (BC_BLUE==(BubbleArray+i)->myColor)
        {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_blue_little, 
                              &s_main_lcd_dev_info);
        }

        else if (BC_GRAY==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_gray_little, 
                              &s_main_lcd_dev_info);
        }

        else if (BC_GREEN==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_green_little, 
                              &s_main_lcd_dev_info);
        }

        else if (BC_ORANGE==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_orange_little, 
                              &s_main_lcd_dev_info);
        }

        else if (BC_PINK==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_pink_little, 
                              &s_main_lcd_dev_info);
        }

        else if (BC_RED==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_red_little, 
                              &s_main_lcd_dev_info);
        }

        else if (BC_YELLOW==(BubbleArray+i)->myColor)
         {

           point.x = (BubbleArray+i)->my_X - RADIUS;
           point.y = (BubbleArray+i)->my_Y - RADIUS;
           GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_yellow_little, 
                              &s_main_lcd_dev_info);
        }
		
	}
    }

}

/*****************************************************************************/
//  Description : DrawBombBubble
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void DrawBombBubble(MMI_WIN_ID_T win_id,MMI_GAME_BUBBLE_T *BubbleArray,uint16 BubbleNum,MMI_GAME_BUBBLESTATE State)
{
    if(NULL==BubbleArray)

        return;

    else
    {
    uint16 i=0;
    GUI_POINT_T point = {0};
   // GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    for (i=0; i<BubbleNum; i++)
    {
        if (State==(BubbleArray+i)->myState)
        {
          point.x = (BubbleArray+i)->my_X - RADIUS;
          point.y = (BubbleArray+i)->my_Y - RADIUS;
          GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              IMAGE_Bubble_boom, 
                              &s_main_lcd_dev_info);
        }
    }
    }
}

/*****************************************************************************/
//  Description : ShowScore
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void ShowScore(MMI_WIN_ID_T win_id)
{
     uint16              score_str[10]   = {0};
     MMI_STRING_T        text            = {0};
     //GUILABEL_SetTextById(MMIGAME_BUBBLEBOBBLE_LABEL_SCORE_CTRL_ID,TXT_NULL,FALSE);
     GUILABEL_SetBackgroundColor(MMIGAME_BUBBLEBOBBLE_LABEL_SCORE_CTRL_ID,MMI_GRAY_WHITE_COLOR);
     sprintf( (char*)score_str,"%d",curScore);
     text.wstr_len = 16;
     text.wstr_ptr = SCI_ALLOC_APP((text.wstr_len +1)* sizeof(wchar));
     SCI_MEMSET(text.wstr_ptr,0,((text.wstr_len +1)* sizeof(wchar)));
     MMI_STRNTOWSTR(text.wstr_ptr,10,(uint16*)score_str,10,10);
     //MMK_DestroyControl(MMIGAME_BUBBLEBOBBLE_LABEL_SCORE_CTRL_ID);
     GUILABEL_SetText(MMIGAME_BUBBLEBOBBLE_LABEL_SCORE_CTRL_ID, &text, TRUE);
     if (PNULL != text.wstr_ptr)
     {
         SCI_FREE(text.wstr_ptr);
         text.wstr_ptr = PNULL;
     }
}

/*****************************************************************************/
//  Description : ShowLevel
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void ShowLevel(MMI_WIN_ID_T win_id)
{
     uint16              level_str[3]   = {0};
     MMI_STRING_T        text            = {0};
     //GUILABEL_SetTextById(MMIGAME_BUBBLEBOBBLE_LABEL_LEVEL_CTRL_ID,TXT_NULL,FALSE);
     GUILABEL_SetBackgroundColor(MMIGAME_BUBBLEBOBBLE_LABEL_LEVEL_CTRL_ID,MMI_GRAY_WHITE_COLOR);
     sprintf( (char*)level_str,"%d",curLevel);
     text.wstr_len = 3;
     text.wstr_ptr = SCI_ALLOC_APP((text.wstr_len +1)* sizeof(wchar));
     SCI_MEMSET(text.wstr_ptr,0,((text.wstr_len +1)* sizeof(wchar)));
     MMI_STRNTOWSTR(text.wstr_ptr,3,(uint16*)level_str,3,3);
     //MMK_DestroyControl(MMIGAME_BUBBLEBOBBLE_LABEL_LEVEL_CTRL_ID);
     GUILABEL_SetText(MMIGAME_BUBBLEBOBBLE_LABEL_LEVEL_CTRL_ID, &text, TRUE);
     if (PNULL != text.wstr_ptr)
     {
         SCI_FREE(text.wstr_ptr);
         text.wstr_ptr = PNULL;
     }
}

LOCAL void CreateBUBBLENeedleLayer(MMI_WIN_ID_T win_id)
{
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        uint16 img_width = 0;
        uint16 img_height = 0;
        GUI_POINT_T center_point = {0};

        center_point.x = OFFSETX;//prepareBubble[0].my_X;
        center_point.y = OFFSETY;//prepareBubble[0].my_Y;

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_shooter, win_id);        
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = center_point.x- (img_width + img_height)/2;
        create_info.offset_y = center_point.y- (img_width + img_height)/2;
        create_info.width = img_width + img_height;
        create_info.height = img_width + img_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(&create_info, &s_needle_dev_info);
    }
}

LOCAL void BUBBLECreateLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle)
{
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
        
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle =handle;
        create_info.offset_x = 0;
        create_info.offset_y = 28;
        create_info.width = logic_lcd_width;
        if(MMITHEME_IsMainScreenLandscape())
        {
            create_info.height = logic_lcd_height;
        }
        else
        {
            create_info.height = 167;
        }
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;        
        create_info.format = UILAYER_MEM_DOUBLE_COPY;
        UILAYER_CreateLayer(&create_info, dev_info_ptr);
    }
}

LOCAL void BUBBLEAppendLayer(GUI_LCD_DEV_INFO *dev_info_ptr)
{
    UILAYER_APPEND_BLT_T append_layer={0};
    
    append_layer.layer_level=UILAYER_LEVEL_NORMAL;
    append_layer.lcd_dev_info=*dev_info_ptr;
    UILAYER_AppendBltLayer(&append_layer);
}

/*****************************************************************************/
//  Description : change the angle of the shooter
//  Global resource dependence : none
//  Author: junxue.Zhu
//  Note:
/*****************************************************************************/
LOCAL void ChangeShooterAngle(MMI_WIN_ID_T win_id, int16 new_freq_angle)
{
    //GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    uint16 img_width = 0;
    uint16 img_height = 0;
    //GUI_RECT_T rect = {0};
    ROTATE_ARBITRARY_IN_T	rot_in		= {0};
    ROTATE_ARBITRARY_OUT_T	rot_out		= {0};
    uint32              img_size       = 0;
    uint8*              img_ptr         = PNULL;
    MMI_IMAGE_BUBBLE_T src_img = {0};
    MMI_IMAGE_BUBBLE_T dst_img = {0};  
    uint32  rotated_buf_size  = 0;
    uint8*  rotated_buf_ptr   = PNULL;
    uint8* decomp_buf_ptr = PNULL;
    uint32 decomp_buf_size = 0; 
    GUI_LCD_DEV_INFO tmp_dev_info = { 0,UILAYER_NULL_HANDLE };

    if(UILAYER_IsLayerActive(&s_needle_dev_info))
    {
        UILAYER_Clear(&s_needle_dev_info);
    }
    else
    {
        if(UILAYER_IsMultiLayerEnable())
        {
            UILAYER_CREATE_T    create_info = {0};
            GUI_POINT_T center_point = {0};
            center_point.x = OFFSETX;
            center_point.y = OFFSETY;
            
            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_shooter, win_id);        
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = win_id;
            create_info.offset_x = center_point.x- (img_width + img_height)/2;
            create_info.offset_y = center_point.y- (img_width + img_height)/2;
            create_info.width = img_width + img_height;
            create_info.height = img_width + img_height;
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = TRUE;
            UILAYER_CreateLayer(&create_info, &tmp_dev_info);
        }
    }
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_shooter, win_id);
    
    rotated_buf_size = (img_width + img_height) * (img_width + img_height) * sizeof(uint32);
    if(UILAYER_IsLayerActive(&s_needle_dev_info))
    {
        rotated_buf_ptr = UILAYER_GetLayerBufferPtr(&s_needle_dev_info);
    }
    else if(UILAYER_IsLayerActive(&tmp_dev_info))
    {
        rotated_buf_ptr = UILAYER_GetLayerBufferPtr(&tmp_dev_info);
    }
    else
    {
        SCI_TRACE_LOW("DisplayFreqPointerByAngle:no memery 1!");
        return; 
    }
    
    decomp_buf_size = img_width * img_height * sizeof(uint32);
    decomp_buf_ptr = SCI_ALLOCA((decomp_buf_size) * sizeof(uint8));
    if(PNULL == decomp_buf_ptr)
    {
        SCI_TRACE_LOW("DisplayFreqPointerByAngle:no memery 2!");
        if(UILAYER_IsLayerActive(&tmp_dev_info))
        {
            BUBBLEReleaseLayer(&tmp_dev_info);
        }
        return;
    }
    
    img_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_shooter,win_id,&img_size);
    src_img.data_ptr = img_ptr;
    src_img.data_size = img_size;
    src_img.data_type = IMGREF_FORMAT_RGB565;
    src_img.width = img_width;
    src_img.height = img_height;
    SCI_MEMSET(decomp_buf_ptr, 0, decomp_buf_size);
    if (!DecompressARGB(&src_img, &dst_img, decomp_buf_ptr, decomp_buf_size))
    {
        SCI_TRACE_LOW("DisplayFreqPointerByAngle:DecompressARGB error!");
        if(PNULL != decomp_buf_ptr) /*lint !e774*/
        {
            SCI_FREE(decomp_buf_ptr);
        }
        if(UILAYER_IsLayerActive(&tmp_dev_info))
        {
            BUBBLEReleaseLayer(&tmp_dev_info);
        }
        return;
    }
    rot_in.angle = new_freq_angle;
    rot_in.src_format = IMGREF_FORMAT_ARGB888;
    rot_in.src_size.w = img_width;
    rot_in.src_size.h = img_height;
    rot_in.src_center.x = img_width/2;
    rot_in.src_center.y = img_height;
    rot_in.src_chn.chn0.ptr = (void *)dst_img.data_ptr; 
    rot_in.src_chn.chn0.size = img_width * img_height * sizeof(uint32); 

    rot_in.target_format = IMGREF_FORMAT_ARGB888;
    rot_in.target_size.w = img_width + img_height;
    rot_in.target_size.h = img_width + img_height;
    rot_in.target_chn.chn0.ptr = (void *)rotated_buf_ptr;
    rot_in.target_chn.chn0.size = rotated_buf_size;
    rot_in.target_center.x = (img_width + img_height)/2;
    rot_in.target_center.y = (img_width + img_height)/2;
    
    rot_in.mode = ROTATE_ARBITRARY_AUTO_CROP;

    SCI_MEMSET(rotated_buf_ptr, 0, rotated_buf_size);
    if (SCI_SUCCESS != GRAPH_RotateArbitrary(&rot_in, &rot_out))
    {
        SCI_TRACE_LOW("DisplayFreqPointerByAngle:GRAPH_RotateArbitrary error!");
        if(PNULL != decomp_buf_ptr) /*lint !e774*/
        {
            SCI_FREE(decomp_buf_ptr);
        }
        if(UILAYER_IsLayerActive(&tmp_dev_info))
        {
            BUBBLEReleaseLayer(&tmp_dev_info);
        }
    	return;
    }
    if(UILAYER_IsLayerActive(&tmp_dev_info))
    {
        GUI_RECT_T rect = UILAYER_GetLayerRect(&tmp_dev_info);
        UILAYER_BltLayerToLayer(&s_main_lcd_dev_info,&tmp_dev_info , &rect,TRUE);
        BUBBLEReleaseLayer(&tmp_dev_info);
    }
    
    SCI_FREE(decomp_buf_ptr); 
}

LOCAL void BUBBLEReleaseLayer(GUI_LCD_DEV_INFO *dev_info_ptr)
{
    if(PNULL!=dev_info_ptr)
    {
        if(dev_info_ptr->block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(dev_info_ptr);
        }
    }
}

/******************************************************************************/
//  Description: decompressed the ARGB image
//  Author: 
//  Input:   
//          src_img_ptr:	pointer of input image information
//			dst_buf_ptr:	pointer to the target buffer
//			dst_buf_size:	target buffer size
//  Output: 
//			dst_img_ptr:	pointer to the decompressed image information
//  Return: 
//			TRUE:			successful
//          FALSE:			failed
//  Note:   
/******************************************************************************/
LOCAL BOOLEAN DecompressARGB(const MMI_IMAGE_BUBBLE_T *src_img_ptr, MMI_IMAGE_BUBBLE_T *dst_img_ptr, 
							  uint8 *dst_buf_ptr, uint32 dst_buf_size)
{
	uint32			img_bytes	= 0;
	IMG_RES_SRC_T	img_src		= {0};
	IMG_RES_DST_T	img_dst		= {0};
	IMG_RES_INFO_T	img_info	= {0};
	IMG_RES_ERROR_E error		= 0;
	IMG_RES_DST_CTL_T dstctl   = {0};

    //get the compressed ARGB info, width and height
    //calc img_bytes
    if(PNULL == src_img_ptr)
            return FALSE;
	img_src.imgstream_ptr = (uint8 *)src_img_ptr->data_ptr;
	img_src.imgstreamsize = src_img_ptr->data_size;

    error = IMG_RES_GetImageInfo(&img_src, &img_info);
	if (IMG_RES_SUCCESS != error)
	{
		return FALSE;
	}

	img_bytes = img_info.width * img_info.height * 4;	

	if (PNULL == dst_buf_ptr || dst_buf_size < img_bytes)
	{
		return FALSE;
	}

	img_src.imgrect.left = 0;
	img_src.imgrect.top = 0;
	img_src.imgrect.right = img_info.width - 1;
	img_src.imgrect.bottom = img_info.height - 1;
	
	//decompress ARGB image
	img_dst.dstctl = &dstctl;
	img_dst.dstctl->bg_type = IMG_BG_NORMAL;
	img_dst.dstctl->format = IMGREF_FORMAT_ARGB888;
	img_dst.dstctl->dstmem = dst_buf_ptr;
	img_dst.dstctl->width = img_info.width;
	img_dst.dstctl->height = img_info.height;
	img_dst.dstrect.top = 0;
	img_dst.dstrect.left = 0;
	img_dst.dstrect.right = img_info.width - 1;
	img_dst.dstrect.bottom = img_info.height - 1;

	SCI_MEMSET(dst_buf_ptr, 0, dst_buf_size);
       
       error = IMG_RES_Display_Img(&img_src, &img_dst);	
	if (IMG_RES_SUCCESS != error)
	{
		return FALSE;
	}
	
	*dst_img_ptr = *src_img_ptr;

    dst_img_ptr->data_ptr = dst_buf_ptr;
    dst_img_ptr->data_size = img_bytes;
    dst_img_ptr->data_type = IMGREF_FORMAT_ARGB888;
    
	return TRUE;
}
/*****************************************************************************/
//  Description :open autodemo read script error
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void OpenBubbleErrorWin(MMI_TEXT_ID_T text_id)
{
    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,
                                TXT_NULL,IMAGE_PUBWIN_WARNING,
                                PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}

/*****************************************************************************/
//  Description :open autodemo finished window by text_id 
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void OpenThisLevelFinishedWin(void)
{
   MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NULL,
                               TXT_NULL,IMAGE_PUBWIN_SUCCESS,
                               PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}

/*****************************************************************************/
//  FUNCTION:     MMIAPIGMBUBBLEBOBBLE_OpenGameWin
//  Description :   open bubble win
//  Global resource dependence : 
//  Author:       junxue.zhu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGMBUBBLEBOBBLE_OpenGameWin(void)
{
    return (MMK_CreateWin((uint32*)MMIGMBUBBLEMAIN_MENU_WIN_TAB,PNULL));
}

#endif
