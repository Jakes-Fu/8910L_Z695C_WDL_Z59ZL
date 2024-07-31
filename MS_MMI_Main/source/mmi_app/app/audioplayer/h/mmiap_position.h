/*****************************************************************************
** File Name:      mmiap_position.h                                          *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _MMIAP_POSITION_H_
#define _MMIAP_POSITION_H_ 

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
#if defined (MAINLCD_SIZE_176X220) 

#define MMIMP3_TIME_X_OFFSET            (1)
#define MMIMP3_TIME_Y_OFFSET            (1)
#define MMIMP3_TIME_WIDTH               (50)  //00:00 width
#define MMIMP3_TIME_HEIGHT              (12)    //00:00 width
		
		//设置为anim使用
#define MMIMP3_MAIN_RECT_HEIGHT         (5)       
#define MMIMP3_MAIN_RECT_HEIGHT_H       (50)
		
#define MMIMP3_MODE_WIDTH_PERCENT_TP    (6)
#define MMIMP3_MODE_WIDTH_PERCENT_TP_H  (5)
#define MMIMP3_MODE_WIDTH_PERCENT       (10)
#define MMIMP3_MODE_WIDTH_PERCENT_H     (7)
// 		
#define MMIMP3_TIME_FONT                SONG_FONT_10
#define MMIMP3_MAIN_TEXT_FONT           SONG_FONT_18
#define MMIMP3_SUB_TEXT_FONT            SONG_FONT_10
#define MMIMP3_TITLE_FONT            SONG_FONT_18
#define MMIMP3_ALBUM_ARTIST_FONT			SONG_FONT_16
		
#define MMIMP3_LIST_BTN_WIDTH_V         (30)    //竖屏列表、音量按钮区域宽度(歌名那一行)
#define MMIMP3_LIST_BTN_WIDTH_H         (66)    //横屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_HEIGHT_H        (55)    //horizontal only
#define MMIMP3_PLAY_BTN_WIDTH_V         (30)   //中间的歌名
#define MMIMP3_ISTYLE_PLAY_BTN_WIDTH_V  (20)
#define MMIMP3_PLAY_BTN_WIDTH_H         (30)
#define MMIMP3_TITLE_MAIN_HEIGHT        (20)
#define MMIMP3_TITLE_SUB_HEIGHT         (9)
#define MMIMP3_TITLE_MAIN_HEIGHT_H      (26)
#define MMIMP3_TITLE_SUB_HEIGHT_H       (22)
		
#define MMIMP3_HOR_ANIM_WIDTH           100     //横屏左右分割线的位置
#define MMIMP3_PLAY_FORM_TOP            (130-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_PLAY_FORM_TOP_H          (100-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_OSD_PANEL_TOP            (75-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) 
#define MMIMP3_OSD_PANEL_TOP_H          (110-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMIMP3_OSD_LABEL_HEIGHT         10
		
#define MMIMP3_OSD_BTN_WIDTH            30    //循环与 AB之间的距离 
#define MMIMP3_OSD_BTN_HEIGHT           20
#define MMIMP3_OSD_PROGRESS_HEIGHT      20  //进度条
#define MMIMP3_OSD_TIME_HEIGHT          10
		
#define MMIMP3_BTN_BG_HEIGHT          29
#define MMIMP3_TITLE_BG_HEIGHT          24
		
#define MP3_LAYOUT_PREV_RECT_V       {0, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-MMIMP3_BTN_BG_HEIGHT, (MMI_MAINSCREEN_WIDTH - 1)/3, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MP3_LAYOUT_PLAY_RECT_V       {(MMI_MAINSCREEN_WIDTH - 1)/3+1, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-MMIMP3_BTN_BG_HEIGHT, (MMI_MAINSCREEN_WIDTH - 1)*2/3, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MP3_LAYOUT_NEXT_RECT_V       {(MMI_MAINSCREEN_WIDTH - 1)*2/3, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-MMIMP3_BTN_BG_HEIGHT, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
		
#define MP3_TITLE_DUMMY_RECT_V       {0, 0, (MMI_MAINSCREEN_WIDTH - 1)/4, MMIMP3_TITLE_BG_HEIGHT}
#define MP3_TITLE_TEXT_RECT_V       {(MMI_MAINSCREEN_WIDTH - 1)/4+1, 0, (MMI_MAINSCREEN_WIDTH - 1)*3/4, MMIMP3_TITLE_BG_HEIGHT}
#define MP3_TITLE_INFO_RECT_V       {(MMI_MAINSCREEN_WIDTH - 1)*3/4+1, 0, MMI_MAINSCREEN_WIDTH - 1, MMIMP3_TITLE_BG_HEIGHT}
		
#define MP3_ARTIST_RECT_V       {4, 93, MMI_MAINSCREEN_WIDTH - 1, 113}
#define MP3_ALBUM_RECT_V       {4, 114, MMI_MAINSCREEN_WIDTH - 1, 130}
		
#define MP3_PROGRESS_RECT_V       {0, 136, MMI_MAINSCREEN_WIDTH - 1, 152}
#define MP3_TIME_RECT_V       {0, 153, MMI_MAINSCREEN_WIDTH - 1, 169}
		
#define MP3_SHUFFLE_RECT_V       {3, MMIMP3_TITLE_BG_HEIGHT+8 ,33 ,MMIMP3_TITLE_BG_HEIGHT+28}
#define MP3_REPEAT_RECT_V       {35, MMIMP3_TITLE_BG_HEIGHT+8, 65, MMIMP3_TITLE_BG_HEIGHT+28}
#define MP3_REV_RECT_V         {67, MMIMP3_TITLE_BG_HEIGHT+8, 97, MMIMP3_TITLE_BG_HEIGHT+28}
#define MP3_PIC_RECT_V       {98,MMIMP3_TITLE_BG_HEIGHT+10 , MMI_MAINSCREEN_WIDTH - 1, MMIMP3_TITLE_BG_HEIGHT+67}
		
		
#define MP3_LAYOUT_PREV_RECT_H       {0, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-22, (MMI_MAINSCREEN_HEIGHT - 1)/3, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MP3_LAYOUT_PLAY_RECT_H       {(MMI_MAINSCREEN_HEIGHT - 1)/3+1, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-22, (MMI_MAINSCREEN_HEIGHT - 1)*2/3, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MP3_LAYOUT_NEXT_RECT_H       {(MMI_MAINSCREEN_HEIGHT - 1)*2/3, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-22, MMI_MAINSCREEN_HEIGHT - 1, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
		
#define MP3_TITLE_DUMMY_RECT_H       {0, 0, (MMI_MAINSCREEN_HEIGHT - 1)/4, MMIMP3_TITLE_BG_HEIGHT}
#define MP3_TITLE_TEXT_RECT_H       {(MMI_MAINSCREEN_HEIGHT - 1)/4+1, 0, (MMI_MAINSCREEN_HEIGHT - 1)*3/4, MMIMP3_TITLE_BG_HEIGHT}
#define MP3_TITLE_INFO_RECT_H       {(MMI_MAINSCREEN_HEIGHT - 1)*3/4+1, 0, MMI_MAINSCREEN_HEIGHT - 1, MMIMP3_TITLE_BG_HEIGHT}
		
#define MP3_ARTIST_RECT_H       {4, 65, 150, 80}
#define MP3_ALBUM_RECT_H       {4, 81, 150, 100}
		
#define MP3_PROGRESS_RECT_H       {0, 105, MMI_MAINSCREEN_HEIGHT - 1, 114}
#define MP3_TIME_RECT_H       {0, 115, MMI_MAINSCREEN_HEIGHT - 1, 128}
		
#define MP3_SHUFFLE_RECT_H       {3, MMIMP3_TITLE_BG_HEIGHT+8 ,33 ,MMIMP3_TITLE_BG_HEIGHT+28}
#define MP3_REPEAT_RECT_H       {38, MMIMP3_TITLE_BG_HEIGHT+8, 63, MMIMP3_TITLE_BG_HEIGHT+28}
#define MP3_REV_RECT_H       {73, MMIMP3_TITLE_BG_HEIGHT+8, 100, MMIMP3_TITLE_BG_HEIGHT+28}
#define MP3_PIC_RECT_H       {157,MMIMP3_TITLE_BG_HEIGHT+5 , MMI_MAINSCREEN_HEIGHT - 1, MMIMP3_TITLE_BG_HEIGHT+57}

#elif (MAINLCD_SIZE_128X160)

#define MMIMP3_TIME_X_OFFSET            (1)
#define MMIMP3_TIME_Y_OFFSET            (1)
#define MMIMP3_TIME_WIDTH               (50)  //00:00 width
#define MMIMP3_TIME_HEIGHT              (12)    //00:00 width
		
		//设置为anim使用
#define MMIMP3_MAIN_RECT_HEIGHT         (5)       
#define MMIMP3_MAIN_RECT_HEIGHT_H       (50)

#define MMIMP3_MODE_WIDTH_PERCENT_TP    (6)
#define MMIMP3_MODE_WIDTH_PERCENT_TP_H  (5)
#define MMIMP3_MODE_WIDTH_PERCENT       (10)
#define MMIMP3_MODE_WIDTH_PERCENT_H     (7)

#define MMIMP3_TIME_FONT                SONG_FONT_10
#define MMIMP3_MAIN_TEXT_FONT           SONG_FONT_18
#define MMIMP3_SUB_TEXT_FONT            SONG_FONT_10
#define MMIMP3_ALBUM_ARTIST_FONT			SONG_FONT_14
#define MMIMP3_TITLE_FONT            SONG_FONT_16

#define MMIMP3_LIST_BTN_WIDTH_V         (30)    //竖屏列表、音量按钮区域宽度(歌名那一行)
#define MMIMP3_LIST_BTN_WIDTH_H         (66)    //横屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_HEIGHT_H        (55)    //horizontal only
#define MMIMP3_PLAY_BTN_WIDTH_V         (30)   //中间的歌名
#define MMIMP3_ISTYLE_PLAY_BTN_WIDTH_V  (20)
#define MMIMP3_PLAY_BTN_WIDTH_H         (30)
#define MMIMP3_TITLE_MAIN_HEIGHT        (20)
#define MMIMP3_TITLE_SUB_HEIGHT         (9)
#define MMIMP3_TITLE_MAIN_HEIGHT_H      (26)
#define MMIMP3_TITLE_SUB_HEIGHT_H       (22)

#define MMIMP3_HOR_ANIM_WIDTH           100     //横屏左右分割线的位置
#define MMIMP3_PLAY_FORM_TOP            (130-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_PLAY_FORM_TOP_H          (100-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_OSD_PANEL_TOP            (75-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) 
#define MMIMP3_OSD_PANEL_TOP_H          (110-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMIMP3_OSD_LABEL_HEIGHT         10

#define MMIMP3_OSD_BTN_WIDTH            30    //循环与 AB之间的距离 
#define MMIMP3_OSD_BTN_HEIGHT           20
#define MMIMP3_OSD_PROGRESS_HEIGHT      10  //进度条
#define MMIMP3_OSD_TIME_HEIGHT          10

#define MMIMP3_BTN_BG_HEIGHT          29
#define MMIMP3_TITLE_BG_HEIGHT          20

#define MP3_LAYOUT_PREV_RECT_V       {0, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-16, (MMI_MAINSCREEN_WIDTH - 1)/3, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MP3_LAYOUT_PLAY_RECT_V       {(MMI_MAINSCREEN_WIDTH - 1)/3+1, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-16, (MMI_MAINSCREEN_WIDTH - 1)*2/3, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MP3_LAYOUT_NEXT_RECT_V       {(MMI_MAINSCREEN_WIDTH - 1)*2/3, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-16, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}

#define MP3_TITLE_DUMMY_RECT_V       {0, 0, (MMI_MAINSCREEN_WIDTH - 1)/4 - 5, 20}
#define MP3_TITLE_TEXT_RECT_V       {(MMI_MAINSCREEN_WIDTH - 1)/4 - 4, 0, (MMI_MAINSCREEN_WIDTH - 1)*3/4 - 6, 20}
#define MP3_TITLE_INFO_RECT_V       {(MMI_MAINSCREEN_WIDTH - 1)*3/4 - 5, 0, MMI_MAINSCREEN_WIDTH - 1, 20}

#define MP3_ARTIST_RECT_V       {0, 64, MMI_MAINSCREEN_WIDTH - 1, 79}
#define MP3_ALBUM_RECT_V       {0, 81, MMI_MAINSCREEN_WIDTH - 1, 96}

#define MP3_PROGRESS_RECT_V       {0, 97, MMI_MAINSCREEN_WIDTH - 1, 108}
#define MP3_TIME_RECT_V       {0, 109, MMI_MAINSCREEN_WIDTH - 1, 122}

#define MP3_SHUFFLE_RECT_V       {0, 22 ,23 ,40}
#define MP3_REPEAT_RECT_V       {24, 22, 48, 40}
#define MP3_REV_RECT_V       {49, 22, 75, 40}
#define MP3_PIC_RECT_V       {81, 21, MMI_MAINSCREEN_WIDTH - 1, 63}
#define MP3_LAYOUT_PREV_RECT_H       {0, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-17, (MMI_MAINSCREEN_HEIGHT - 1)/3, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MP3_LAYOUT_PLAY_RECT_H       {(MMI_MAINSCREEN_HEIGHT - 1)/3+1, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-17, (MMI_MAINSCREEN_HEIGHT - 1)*2/3, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MP3_LAYOUT_NEXT_RECT_H       {(MMI_MAINSCREEN_HEIGHT - 1)*2/3, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-17, MMI_MAINSCREEN_HEIGHT - 1, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}

#define MP3_TITLE_DUMMY_RECT_H       {0, 0, (MMI_MAINSCREEN_HEIGHT - 1)/4, MMIMP3_TITLE_BG_HEIGHT}
#define MP3_TITLE_TEXT_RECT_H       {(MMI_MAINSCREEN_HEIGHT - 1)/4+1, 0, (MMI_MAINSCREEN_HEIGHT - 1)*3/4, MMIMP3_TITLE_BG_HEIGHT}
#define MP3_TITLE_INFO_RECT_H       {(MMI_MAINSCREEN_HEIGHT - 1)*3/4+1, 0, MMI_MAINSCREEN_HEIGHT - 1, MMIMP3_TITLE_BG_HEIGHT}

#define MP3_ARTIST_RECT_H       {4, 35, 105, 50}
#define MP3_ALBUM_RECT_H       {4, 51, 105, 67}

#define MP3_PROGRESS_RECT_H       {0, 68, MMI_MAINSCREEN_HEIGHT - 1, 76}
#define MP3_TIME_RECT_H       {0, 77, MMI_MAINSCREEN_HEIGHT - 1, 89}

#define MP3_SHUFFLE_RECT_H       {3, MMIMP3_TITLE_BG_HEIGHT, 30 ,MMIMP3_TITLE_BG_HEIGHT+14}
#define MP3_REPEAT_RECT_H       {34, MMIMP3_TITLE_BG_HEIGHT, 58, MMIMP3_TITLE_BG_HEIGHT+14}
#define MP3_REV_RECT_H       {62, MMIMP3_TITLE_BG_HEIGHT, 95, MMIMP3_TITLE_BG_HEIGHT+14}
#define MP3_PIC_RECT_H       {108,MMIMP3_TITLE_BG_HEIGHT+2 , MMI_MAINSCREEN_HEIGHT - 1, MMIMP3_TITLE_BG_HEIGHT+46}


//#endif
#elif MAINLCD_SIZE_240X320

//14*80 相对坐标
#define MMIMP3_TIME_X_OFFSET            (1)
#define MMIMP3_TIME_Y_OFFSET            (0)
#define MMIMP3_TIME_WIDTH             (114) //xiongkai  (80)
#define MMIMP3_TIME_HEIGHT             (24) //xiongkai  (14)

//设置为anim使用
#define MMIMP3_MAIN_RECT_HEIGHT         (187)
#define MMIMP3_MAIN_RECT_HEIGHT_H       (143)

#define MMIMP3_MODE_WIDTH_PERCENT_TP    (12)
#define MMIMP3_MODE_WIDTH_PERCENT_TP_H  (9)
#define MMIMP3_MODE_WIDTH_PERCENT       (19)
#define MMIMP3_MODE_WIDTH_PERCENT_H     (14)

#define MMIMP3_TIME_FONT                SONG_FONT_24 // xiongkai SONG_FONT_14
#define MMIMP3_MAIN_TEXT_FONT           SONG_FONT_18
#define MMIMP3_SUB_TEXT_FONT            SONG_FONT_14
#define MMIMP3_MAIN_TITLE_FONT          MMI_DEFAULT_BIG_FONT //zhaijing add for title font

#define MMIMP3_LIST_BTN_WIDTH_V         (50)    //竖屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_WIDTH_H         (56)    //横屏列表、音量按钮区域宽度 
#define MMIMP3_LIST_BTN_HEIGHT_H        (52)    //horizontal only
#define MMIMP3_PLAY_BTN_WIDTH_V         (80)
#define MMIMP3_ISTYLE_PLAY_BTN_WIDTH_V  (47)
#define MMIMP3_PLAY_BTN_WIDTH_H         (40)
#define MMIMP3_TITLE_MAIN_HEIGHT      (33)//  (20)
#define MMIMP3_TITLE_SUB_HEIGHT         16//(18)
#define MMIMP3_TITLE_MAIN_HEIGHT_H      (26)
#define MMIMP3_TITLE_SUB_HEIGHT_H       26//20//(22)

#define MMIMP3_HOR_ANIM_WIDTH           190     //横屏左右分割线的位置
#define MMIMP3_PLAY_FORM_TOP            (280-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_PLAY_FORM_TOP_H          (184-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) 
#define MMIMP3_OSD_PANEL_TOP            (224-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) 
#define MMIMP3_OSD_PANEL_TOP_H          (184-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMIMP3_OSD_LABEL_HEIGHT         22

#define MMIMP3_OSD_BTN_WIDTH            34
#define MMIMP3_OSD_BTN_HEIGHT           22//30
#define MMIMP3_OSD_PROGRESS_HEIGHT      20
#define MMIMP3_OSD_TIME_HEIGHT          24 // xiongkai  14
//zhaijing add for UI
#define MMIMP3_MAIN_WIDTH               240
#define MMIMP3_PROCSS_HEIGHT            4
#define MMIMP3_TIME_GAP_PANEL           5
#define MMIMP3_TIME_ADD_PANEL           20
#define MMIMP3_ABLUM_HEIGHT           208//  240
//End

//#endif
#elif MAINLCD_SIZE_128X64

//14*80 相对坐标
#define MMIMP3_TIME_X_OFFSET            (1)
#define MMIMP3_TIME_Y_OFFSET            (1)
#define MMIMP3_TIME_WIDTH               (80)
#define MMIMP3_TIME_HEIGHT              (14)
 
#define MMIMP3_MAIN_RECT_HEIGHT         (166)
#define MMIMP3_MAIN_RECT_HEIGHT_H       (120)

#define MMIMP3_MODE_WIDTH_PERCENT_TP    (12)
#define MMIMP3_MODE_WIDTH_PERCENT_TP_H  (9)
#define MMIMP3_MODE_WIDTH_PERCENT       (19)
#define MMIMP3_MODE_WIDTH_PERCENT_H     (14)

#define MMIMP3_TIME_FONT                SONG_FONT_12
#define MMIMP3_MAIN_TEXT_FONT           SONG_FONT_18
#define MMIMP3_SUB_TEXT_FONT            SONG_FONT_14

#define MMIMP3_LIST_BTN_WIDTH_V         (60)    //竖屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_WIDTH_H         (66)    //横屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_HEIGHT_H        (55)    //horizontal only
#define MMIMP3_PLAY_BTN_WIDTH_V         (80)
#define MMIMP3_ISTYLE_PLAY_BTN_WIDTH_V  (60)
#define MMIMP3_PLAY_BTN_WIDTH_H         (60)
#define MMIMP3_TITLE_MAIN_HEIGHT        (22)
#define MMIMP3_TITLE_SUB_HEIGHT         (18)
#define MMIMP3_TITLE_MAIN_HEIGHT_H      (26)
#define MMIMP3_TITLE_SUB_HEIGHT_H       (22)

#define MMIMP3_HOR_ANIM_WIDTH           190     //横屏左右分割线的位置
#define MMIMP3_PLAY_FORM_TOP            (280-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_PLAY_FORM_TOP_H          (184-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_OSD_PANEL_TOP            (224-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) 
#define MMIMP3_OSD_PANEL_TOP_H          (184-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMIMP3_OSD_LABEL_HEIGHT         22

#define MMIMP3_OSD_BTN_WIDTH            40
#define MMIMP3_OSD_BTN_HEIGHT           38
#define MMIMP3_OSD_PROGRESS_HEIGHT      34
#define MMIMP3_OSD_TIME_HEIGHT          14

#elif MAINLCD_SIZE_240X400

//14*80 相对坐标
#define MMIMP3_TIME_X_OFFSET            (1)
#define MMIMP3_TIME_Y_OFFSET            (0)
#define MMIMP3_TIME_WIDTH               (80)
#define MMIMP3_TIME_HEIGHT              (14)
     
#define MMIMP3_MAIN_RECT_HEIGHT         (237)
#define MMIMP3_MAIN_RECT_HEIGHT_H       (106)

#define MMIMP3_MODE_WIDTH_PERCENT_TP    (12)
#define MMIMP3_MODE_WIDTH_PERCENT_TP_H  (9)
#define MMIMP3_MODE_WIDTH_PERCENT       (19)
#define MMIMP3_MODE_WIDTH_PERCENT_H     (14)

#define MMIMP3_TIME_FONT                SONG_FONT_14
#define MMIMP3_MAIN_TEXT_FONT           SONG_FONT_18
#define MMIMP3_SUB_TEXT_FONT            SONG_FONT_14

#define MMIMP3_LIST_BTN_WIDTH_V         (50)    //竖屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_WIDTH_H         (62)    //横屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_HEIGHT_H        (52)    //horizontal only
#define MMIMP3_PLAY_BTN_WIDTH_V         (36)
#define MMIMP3_PLAY_BTN_WIDTH_H         (36)
#define MMIMP3_ISTYLE_PLAY_BTN_WIDTH_V  (47)
#define MMIMP3_TITLE_MAIN_HEIGHT        (26)
#define MMIMP3_TITLE_SUB_HEIGHT         (20)
#define MMIMP3_TITLE_MAIN_HEIGHT_H      (28)
#define MMIMP3_TITLE_SUB_HEIGHT_H       (24)

#define MMIMP3_HOR_ANIM_WIDTH           190     //横屏左右分割线的位置
#define MMIMP3_PLAY_FORM_TOP            (324-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_PLAY_FORM_TOP_H          (184-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_OSD_PANEL_TOP            (268-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) 
#define MMIMP3_OSD_PANEL_TOP_H          (184-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMIMP3_OSD_LABEL_HEIGHT         22

#define MMIMP3_OSD_BTN_WIDTH            34//40
#define MMIMP3_OSD_BTN_HEIGHT           22//24
#define MMIMP3_OSD_PROGRESS_HEIGHT      20//34
#define MMIMP3_OSD_TIME_HEIGHT          14//12

#elif MAINLCD_SIZE_320X480

//14*80 相对坐标
#define MMIMP3_TIME_X_OFFSET            (2)
#define MMIMP3_TIME_Y_OFFSET            (0)
#define MMIMP3_TIME_WIDTH               (80)
#define MMIMP3_TIME_HEIGHT              (14)

#define MMIMP3_MAIN_RECT_HEIGHT         (297 + 50)
#define MMIMP3_MAIN_RECT_HEIGHT_H       (176)

#define MMIMP3_MODE_WIDTH_PERCENT_TP    (14)
#define MMIMP3_MODE_WIDTH_PERCENT_TP_H  (9)
#define MMIMP3_MODE_WIDTH_PERCENT       (20)
#define MMIMP3_MODE_WIDTH_PERCENT_H     (14)

#define MMIMP3_TIME_FONT                SONG_FONT_14
#define MMIMP3_MAIN_TEXT_FONT           SONG_FONT_18
#define MMIMP3_SUB_TEXT_FONT            SONG_FONT_14

#define MMIMP3_LIST_BTN_WIDTH_V         (75)    //竖屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_WIDTH_H         (66)    //横屏列表、音量按钮区域宽度
#define MMIMP3_LIST_BTN_HEIGHT_H        (55)    //horizontal only
#define MMIMP3_PLAY_BTN_WIDTH_V         (80)
#define MMIMP3_ISTYLE_PLAY_BTN_WIDTH_V  (63)
#define MMIMP3_PLAY_BTN_WIDTH_H         (60)
#define MMIMP3_TITLE_MAIN_HEIGHT        (24)
#define MMIMP3_TITLE_SUB_HEIGHT         (23)
#define MMIMP3_TITLE_MAIN_HEIGHT_H      (26)
#define MMIMP3_TITLE_SUB_HEIGHT_H       (22)

#define MMIMP3_HOR_ANIM_WIDTH           300     //横屏左右分割线的位置
#define MMIMP3_PLAY_FORM_TOP            (406-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_PLAY_FORM_TOP_H          (244-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)  
#define MMIMP3_OSD_PANEL_TOP            (330-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) 
#define MMIMP3_OSD_PANEL_TOP_H          (244-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMIMP3_OSD_LABEL_HEIGHT         40

#define MMIMP3_OSD_BTN_WIDTH            40
#define MMIMP3_OSD_BTN_HEIGHT           28
#define MMIMP3_OSD_PROGRESS_HEIGHT      30
#define MMIMP3_OSD_TIME_HEIGHT          18

#else
#error
#endif

//common rect
#if defined (MAINLCD_SIZE_128X160)
#define MMIMP3_TITLE_TOTAL_HEIGHT_H     (MMIMP3_TITLE_MAIN_HEIGHT_H + 2*MMIMP3_TITLE_SUB_HEIGHT_H)//70 //(98)
#define MMIMP3_TITLE_BOTTOM             (MMIMP3_TITLE_MAIN_HEIGHT + 2*MMIMP3_TITLE_SUB_HEIGHT + 0 -1)
//#define MMIMP3_TITLE_BOTTOM_H           (MMIMP3_TITLE_TOTAL_HEIGHT_H + MMI_STATUSBAR_HEIGHT -1)


#define MMIMP3_LIST_BTN_UP_SPACE        (MMIMP3_OSD_PANEL_TOP_H-0-MMIMP3_TITLE_TOTAL_HEIGHT_H)/2 //horizontal only
#define MMIMP3_ISTYLE_LIST_BTN_UP_SPACE  MMIMP3_LIST_BTN_UP_SPACE

#define MMIMP3_VER_TITLE_FORM_RECT      {0,   0,  MMI_MAINSCREEN_WIDTH - 1, MMIMP3_TITLE_BOTTOM}   //姓名，文件名等title
#define MMIMP3_HOR_TITLE_FORM_RECT      {MMIMP3_HOR_ANIM_WIDTH, 0,  MMI_MAINSCREEN_HEIGHT - 1, MMIMP3_PLAY_FORM_TOP_H-1}
#define MMIMP3_VER_PLAY_FORM_RECT       {0,MMIMP3_PLAY_FORM_TOP, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MMIMP3_HOR_PLAY_FORM_RECT       {MMIMP3_HOR_ANIM_WIDTH, MMIMP3_PLAY_FORM_TOP_H, MMI_MAINSCREEN_HEIGHT - 1, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}

#define MMIMP3_ALBUM_ANIM_RECT_V        {0, MMIMP3_TITLE_BOTTOM+1, MMI_MAINSCREEN_WIDTH - 1, MMIMP3_PLAY_FORM_TOP-1}
#define MMIMP3_ALBUM_ANIM_RECT_H        {0, 0, MMIMP3_HOR_ANIM_WIDTH-1, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}

#define MMIMP3_OSD_LABEL_RECT_V         {0, MMIMP3_TITLE_BOTTOM+1, MMI_MAINSCREEN_WIDTH - 1, MMIMP3_TITLE_BOTTOM+MMIMP3_OSD_LABEL_HEIGHT}
#define MMIMP3_OSD_LABEL_RECT_H         {0, 0, MMIMP3_HOR_ANIM_WIDTH-1, 0+MMIMP3_OSD_LABEL_HEIGHT-1}
#define MMIMP3_OSD_PANEL_RECT_V         {0, MMIMP3_OSD_PANEL_TOP, MMI_MAINSCREEN_WIDTH - 1, MMIMP3_PLAY_FORM_TOP-1}
#define MMIMP3_OSD_PANEL_RECT_H         {0, MMIMP3_OSD_PANEL_TOP_H, MMIMP3_HOR_ANIM_WIDTH-1, MMI_MAINSCREEN_WIDTH - 1-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}

#else

#define MMIMP3_TITLE_TOTAL_HEIGHT_H     (MMIMP3_TITLE_MAIN_HEIGHT_H + 2*MMIMP3_TITLE_SUB_HEIGHT_H)//70 //(98)
#define MMIMP3_TITLE_BOTTOM             (MMIMP3_TITLE_MAIN_HEIGHT + 2*MMIMP3_TITLE_SUB_HEIGHT + 0 -1)
//#define MMIMP3_TITLE_BOTTOM_H           (MMIMP3_TITLE_TOTAL_HEIGHT_H + MMI_STATUSBAR_HEIGHT -1)


#define MMIMP3_LIST_BTN_UP_SPACE        (MMIMP3_OSD_PANEL_TOP_H-0-MMIMP3_TITLE_TOTAL_HEIGHT_H)/2 //horizontal only
#define MMIMP3_ISTYLE_LIST_BTN_UP_SPACE  MMIMP3_LIST_BTN_UP_SPACE

#define MMIMP3_VER_TITLE_FORM_RECT      {0,   5,  MMI_MAINSCREEN_WIDTH - 1, MMIMP3_TITLE_BOTTOM} //{0,   MMI_STATUSBAR_HEIGHT,  MMI_MAINSCREEN_WIDTH - 1, MMIMP3_TITLE_BOTTOM}   //姓名，文件名等title
#define MMIMP3_HOR_TITLE_FORM_RECT      {MMIMP3_HOR_ANIM_WIDTH, 0,  MMI_MAINSCREEN_HEIGHT - 1, MMIMP3_PLAY_FORM_TOP_H-1}
#define MMIMP3_VER_PLAY_FORM_RECT       {0,MMIMP3_PLAY_FORM_TOP, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
#define MMIMP3_HOR_PLAY_FORM_RECT       {MMIMP3_HOR_ANIM_WIDTH, MMIMP3_PLAY_FORM_TOP_H, MMI_MAINSCREEN_HEIGHT - 1, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}

#define MMIMP3_ALBUM_ANIM_RECT_V       {0, 52, MMI_MAINSCREEN_WIDTH - 1, 208}/// {0, MMIMP3_TITLE_BOTTOM+1, MMI_MAINSCREEN_WIDTH - 1, MMIMP3_PLAY_FORM_TOP-1}
#define MMIMP3_ALBUM_ANIM_RECT_H        {0, 0, MMIMP3_HOR_ANIM_WIDTH-1, MMI_MAINSCREEN_WIDTH - 1 - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}

#define MMIMP3_OSD_LABEL_RECT_V         {0, MMIMP3_TITLE_BOTTOM+1, MMI_MAINSCREEN_WIDTH - 1, MMIMP3_TITLE_BOTTOM+MMIMP3_OSD_LABEL_HEIGHT}
#define MMIMP3_OSD_LABEL_RECT_H         {0, 0, MMIMP3_HOR_ANIM_WIDTH-1, 0+MMIMP3_OSD_LABEL_HEIGHT-1}
#define MMIMP3_OSD_PANEL_RECT_V         {0, MMIMP3_OSD_PANEL_TOP, MMI_MAINSCREEN_WIDTH - 1, MMIMP3_PLAY_FORM_TOP-1}
#define MMIMP3_OSD_PANEL_RECT_H         {0, MMIMP3_OSD_PANEL_TOP_H, MMIMP3_HOR_ANIM_WIDTH-1, MMI_MAINSCREEN_WIDTH - 1-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}
//Begin add by zhaijing for music ui
#define MMIMP3_VER_NAME_FORM_RECT       {0+2,3,  MMI_MAINSCREEN_WIDTH - 2, 52}     //{0+48,MMIMP3_ABLUM_HEIGHT+3,  MMI_MAINSCREEN_WIDTH - 48, MMIMP3_ABLUM_HEIGHT+3+MMIMP3_TIME_ADD_PANEL}                                //zhaijing add for title
#define MMIMP3_VER_NAME_FORM_RECT_MINI       {0+48+8,MMIMP3_ABLUM_HEIGHT+3,  MMI_MAINSCREEN_WIDTH - 48-16, MMIMP3_ABLUM_HEIGHT+3+MMIMP3_TIME_ADD_PANEL}                                   //zhaijing add for title
#endif
//common rect end

#define MMIMP3_ALERT_COLOR              0x94B2
#define MMIMP3_ALERT_FONT               MMI_DEFAULT_NORMAL_FONT

#define MMIMP3_TIME_COLOR               MMI_WHITE_COLOR


#define MMIMP3_LYRIC_DISPLAY_LINE_NUM   15 //Mp3播放面板每次显示歌词的行数
#define MMIMP3_LYRIC_BG_TEXT_COLOR      0x94B2
#define MMIMP3_LYRIC_FOCUS_TEXT_COLOR   MMI_GRAY_WHITE_COLOR
#define MMIMP3_LYRIC_TEXT_FONT          MMI_DEFAULT_SMALL_FONT
#define MMIMP3_LYRIC_CHAR_SPACE         0
#define MMIMP3_LYRIC_LINE_SPACE         3

#define MMIMP3_PROGRESS_HORIZ_MARGIN   (5)
#define MMIMP3_LYRIC_TEXT_MARGIN_H     (1)
#define MMIMP3_LYRIC_TEXT_MARGIN_V     (1)

#define MMIMP3_LYRIC_TEXT_MAX_ALPHA   (100)
#define MMIMP3_LYRIC_TEXT_MIN_ALPHA   (20)

#ifdef   __cplusplus
    }
#endif

#endif
