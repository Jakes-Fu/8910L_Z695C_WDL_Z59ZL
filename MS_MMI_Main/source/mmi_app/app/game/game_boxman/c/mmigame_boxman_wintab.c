/****************************************************************************
** File Name:      mmigame_boxman_wintab.c                            
** Author:             Annie.an                                                     
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of game
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie         Create
** 
****************************************************************************/
#define _MMIGAME_BOXMAN_WINTAB_C_


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_game_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BOXMAN_SUPPORT
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
#include "mmigame_boxman_id.h"  
#include "tb_dal.h"
//#include "mmiacc_winmain.h"
//#include "mmigame_link_nv.h"
#include "mmialarm_export.h"
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmi_textfun.h"
#include "mmiphone_export.h"
#include "mmigame_boxman_internal.h"
#include "mmigame_text.h"
#include "mmigame_boxman_image.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipub.h"
#include "mmigame_position.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "guires.h"
#include "guiedit.h"
#include "guistring.h"
#include "mmi_resource.h"
#include "guiform.h"
#ifdef MMI_TASK_MANAGER
#include "mmi_applet_table.h"
#endif


/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA  DECLARE                           */
/*---------------------------------------------------------------------------*/
//所有地图数据
LOCAL  const uint8 boxman_all_maps[BOXMAN_MAP_NUMS][BOXMAN_MAX_MAP_ROW][BOXMAN_MAX_MAP_COL+1] = 
{
    {
        {"00222000"},
        {"00252000"},
        {"00212222"},
        {"22241452"},
        {"25143222"},
        {"22224200"},
        {"00025200"},
        {"00022200"},
        {0}
    },
    {
        {"222220000"},
        {"211320000"},
        {"214420222"},
        {"214120252"},
        {"222122252"},
        {"022111152"},
        {"021112112"},
        {"021112222"},
        {"022222000"}
    },
    {
        {"022220"},
        {"221120"},
        {"213420"},
        {"224122"},
        {"221412"},
        {"254112"},
        {"255752"},
        {"222222"},
        {0}
    },
    {
        {"02222000"},
        {"02312220"},
        {"02141120"},
        {"22212122"},
        {"25212112"},
        {"25411212"},
        {"25111412"},
        {"22222222"},
        {0}
    },
    {
        {"00222222"},
        {"00211112"},
        {"22244412"},
        {"23145512"},
        {"21455522"},
        {"22221120"},
        {"00022220"},
        {0},
        {0}
    },
    {
        {"00222220"},
        {"22211320"},
        {"21145122"},
        {"21154512"},
        {"22217412"},
        {"00211122"},
        {"00222220"},
        {0},
        {0}
    },
    {
        {"00222200"},
        {"00255200"},
        {"02215220"},
        {"02114520"},
        {"22141122"},
        {"21124412"},
        {"21131112"},
        {"22222222"},
        {0}
    },
    {
        {"22222222"},
        {"21121112"},
        {"21455412"},
        {"23457122"},
        {"21455412"},
        {"21121112"},
        {"22222222"},
        {0},
        {0}
    },
    {
        {"222222000"},
        {"211112000"},
        {"214442200"},
        {"211255222"},
        {"221155412"},
        {"021311112"},
        {"022222222"},
        {0},
        {0}
    },
    {
        {"2222222"},
        {"2554552"},
        {"2552552"},
        {"2144412"},
        {"2114112"},
        {"2144412"},
        {"2112312"},
        {"2222222"},
        {0}
    },
    {
        {"02222200"},
        {"02131222"},
        {"22124112"},
        {"21751512"},
        {"21144122"},
        {"22212520"},
        {"00211120"},
        {"00222220"},
        {0}
    },
    {
        {"222222"},
        {"211112"},
        {"214132"},
        {"227112"},
        {"217122"},
        {"217120"},
        {"217120"},
        {"215120"},
        {"222220"}
    },
    {
        {"0022220"},
        {"0021120"},
        {"2224122"},
        {"2117132"},
        {"2117112"},
        {"2117122"},
        {"2227120"},
        {"0025220"},
        {"0022200"}
    },
    {
        {"222220000"},
        {"211122222"},
        {"212121112"},
        {"214111412"},
        {"255242422"},
        {"253411120"},
        {"255112220"},
        {"222222000"},
        {0}
    },
    {
        {"02222220"},
        {"02111122"},
        {"22522412"},
        {"21554112"},
        {"21124112"},
        {"21131222"},
        {"22222200"},
        {0},
        {0}
    }
};
//所有地图的行列数,每行代表一幅地图,第一列表示每幅地图的行数,
//第二列代表每幅地图的列数
//LOCAL const uint8 boxman_all_maps_row_and_col[BOXMAN_MAP_NUMS][3] =
//{
//    {"88"},{"99"},{"86"},{"88"},{"78"},{"78"},{"88"},{"78"},{"79"},{"87"},{"88"},{"96"},{"97"},{"89"},{"78"}
//};
//子图图片名字
//LOCAL const MMI_IMAGE_ID_T boxman_all_maps_name[BOXMAN_SUB_MAP_NUMS] =
//{
//    IMAGE_BOXMAN_WALLOUT,
//        IMAGE_BOXMAN_SPACE,
//        IMAGE_BOXMAN_WALL,
//        IMAGE_BOXMAN_MAN,
//        IMAGE_BOXMAN_BOX,
//        IMAGE_BOXMAN_AIM,
//        IMAGE_BOXMAN_MAN_IN_AIM,
//        IMAGE_BOXMAN_BOX_IN_AIM
//};
LOCAL const int8 s_x_change[4]={1,0,-1,0};
LOCAL const int8 s_y_change[4]={0,-1,0,1};
LOCAL uint8 boxman_current_map_num; //游戏的当前关数
LOCAL BOXMAN_MAP_ROW_AND_COL_T boxman_current_map_row_and_col;  //当前关的行列数
LOCAL uint8 boxman_current_map[BOXMAN_MAX_MAP_ROW][BOXMAN_MAX_MAP_COL]; //当前关的地图数据
LOCAL uint8 boxman_current_map_aim_nums;    //当前关的目标点数目
LOCAL BOXMAN_POINT_T all_aim[BOXMAN_MAX_MAP_AIM];   //当前关的目标点位置队列
LOCAL BOXMAN_POINT_T boxman_current_man;    //当前小人的位置
LOCAL uint8 boxman_current_map_go_back_steps;   //当前关的可回退步数
LOCAL uint8 boxman_current_map_go_back_pointer; //当前关的回退指针
LOCAL BOXMAN_GO_BACK_T go_back_queue[BOXMAN_MAX_GO_BACK_STEPS]; //回退队列
LOCAL BOOLEAN mmigame_boxman_may_continue=FALSE;

LOCAL uint8 s_input_map_num = 0;
LOCAL uint8 s_game_pass_timer = 0;   //通关提示计时器

#ifdef MMI_TASK_MANAGER
#define MMK_CreateGameBoxWin( _WIN_TABLE_, _ADD_DATA_ )  OpenGameBoxWin( _WIN_TABLE_, _ADD_DATA_ )
#else
#define MMK_CreateGameBoxWin   MMK_CreateWin
#endif

#if defined MAINLCD_SIZE_240X320
#define BOXMAN_LABEL_LEFT_FIT   2
#endif

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : the process message function of the box game
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  GameBoxApplet_HandleEvent(    
            IAPPLET_T*          iapplet_ptr,
            MMI_MESSAGE_ID_E    msg_id, 
            DPARAM              param
            );

typedef struct 
{
    CAF_APPLET_T caf_applet;
    uint32  user_data;
}MMIGAMEBOX_APPLET_T;   //vt applet定义

LOCAL MMIGAMEBOX_APPLET_T *s_gamebox_applet_instance = PNULL; 
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmigamebox_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_GAME_BOX_APPLET_ID), sizeof(MMIGAMEBOX_APPLET_T), GameBoxApplet_HandleEvent , 
        IMAGE_SECMENU_ICON_PASTIME_BOXMAN, TXT_BOXMAN},
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmigamebox_applet_info = 
{
    s_mmigamebox_applet_array,
    ARR_SIZE(s_mmigamebox_applet_array)
};

//#define MMIGMBOX_CAFEAPPLET(applet) ((applet)->caf_applet)
//#define MMIGMBOX_HANDLE(applet) ((MMIGMBOX_CAFEAPPLET(applet)).app_handle)
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/            

//*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : go last map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Boxman_Last_Map(void);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : calculate the coordinates of the press point in map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL BOXMAN_POINT_T PressAt(DPARAM param);
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : find move path
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MOVE_QUEUE_T FindMovePath(
                                BOXMAN_POINT_T press_point
                                );
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : deal with press event
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void HandlePress(DPARAM param);
#endif
/*****************************************************************************/
//  Description : creat the boxman game
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxmanWinMsg( 
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      );

/*****************************************************************************/
//  Description : HandleQuintetMainMenuWindow
//       
//  Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//  Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleGMBOXMainMenuWindow(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param);
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : clear move queue
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void ClearMoveQueue(MOVE_QUEUE_T *move_queue);
#endif
/*****************************************************************************/
//  Description : set back color
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void SetBackColor(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : go next map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Boxman_Next_Map(void);
/*****************************************************************************/
//  Description : go back
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Boxman_Go_Back(void);
/*****************************************************************************/
//  Description : the window for boxman help information
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxmanHelpWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );
/*****************************************************************************/
//  Description : refresh current map data
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void RefreshCurrentMapData(void);
/*****************************************************************************/
//  Description : do select map cancel button
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void Mmigame_Boxman_Select_Cancel(void);
/*****************************************************************************/
//  Description : show current map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void ShowCurrentMap(BOOLEAN refresh);
/*****************************************************************************/
//  Description : do select map confirm button
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void Mmigame_Boxman_Select_Confirm(void);

/*****************************************************************************/
//  Description : 朝某个方向移动的处理
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void PressTowards(
                        int8 x_change,  //x坐标的变化值
                        int8 y_change   //y坐标的变化值
                        );

/*****************************************************************************/
//  Description : ShowSubMap
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void ShowSubMap(
                      BOXMAN_MAP_DATA_VALUE_E sub_map_num,
                      BOXMAN_POINT_T position_point
                      );

/*****************************************************************************/
//  Description : deal with map error
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void MapError(void);

/*****************************************************************************/
//  Description : if have win
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void IfHaveWin(void);

/*****************************************************************************/
//  Description : 朝某个方向移动箱子的处理
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void MoveBoxTo(
                     uint8 box_x,       //箱子的x坐标值
                     uint8 box_y,       //箱子的y坐标值
                     int8 x_change, //x坐标的变化值
                     int8 y_change, //y坐标的变化值
                     BOXMAN_MAP_DATA_VALUE_E aim_box_primary_value      //目标箱子位置原来的地图数据值
                     );

/*****************************************************************************/
//  Description : deal with map error
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
//LOCAL void MapError(void);

/*****************************************************************************/
//  Description : save scene for go back
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void SaveSceneForGoback(void);

/*****************************************************************************/
//  Description : play sorry voice
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void PlaySorryVoice(void);

/*****************************************************************************/
//  Description : 朝某个方向移动小人的处理
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void MoveManTo(
                     int8 x_change, //x坐标的变化值
                     int8 y_change, //y坐标的变化值
                     BOXMAN_MAP_DATA_VALUE_E aim_man_primary_value      //目标小人位置原来的地图数据值
                     );

/*****************************************************************************/
//  Description : the boxman select map window
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxmanSelectWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : if press in current map,not include the border of the current map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IfPressInMap(DPARAM param);
#endif
/*****************************************************************************/
//  Description : play sorry voice
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
//LOCAL void PlaySorryVoice(void);

/*****************************************************************************/
//  Description : play move man voice
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void PlayMoveManVoice(void);

/*****************************************************************************/
//  FUNCTION:     HandleBoxmanPassWinMsg
//  Description :   win which dispaly game finish screen
//  Global resource dependence : 
//  Author:       zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxmanPassWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );

/*****************************************************************************/
//  FUNCTION:     DrawGameFinishScreen
//  Description :   dispaly game finish screen
//  Global resource dependence : 
//  Author:       zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void DrawBoxmanFinishScreen(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  FUNCTION:     InitBoxmanWin
//  Description :   
//  Global resource dependence : 
//  Author:       ying.xu
//  Note:   
/*****************************************************************************/
LOCAL void InitBoxmanWin(MMI_WIN_ID_T win_id);
LOCAL  void LcdSwitch( MMI_WIN_ID_T win_id);
LOCAL  MMI_RESULT_E HandleReturnButtonCallback(void);

/*****************************************************************************/
//  FUNCTION:     Boxman_ConvertMsg
//  Description :转横屏的2，4，6，8按键消息  
//  Global resource dependence : 
//  Author:juan.wu
//  Note:   
/*****************************************************************************/
LOCAL  MMI_MESSAGE_ID_E Boxman_ConvertMsg(MMI_MESSAGE_ID_E msg_id);

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : Get game box Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIGAMEBOX_APPLET_T *MMIGMBOX_GetAppletInstance(void);

/*****************************************************************************/
//  Description : Create box setting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenGameBoxWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  );

#endif

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE( MMIGMBOXMAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleGMBOXMainMenuWindow ),    
    WIN_ID( MMIGMBOXMAN_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_BOXMAN, MMIGMBOXMAN_MAIN_MENU_CTRL_ID),
    END_WIN
};

// the window for new boxman game
WINDOW_TABLE( BOXMAN_NEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32) HandleBoxmanWinMsg ),    
        WIN_ID( BOXMAN_NEW_WIN_ID),
//#if (defined MMI_PDA_SUPPORT) ||( defined TOUCHPANEL_TYPE)
        WIN_HIDE_STATUS,
//#endif
#ifdef TOUCH_PANEL_SUPPORT        
        CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_LBTN_UN, MMIGAME_BOXMAN_LAST_MAP_BUT_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_RBTN_UN, MMIGAME_BOXMAN_NEXT_MAP_BUT_CTRL_ID),
        CREATE_BUTTON_CTRL(IMAGE_BOXMAN_GO_BACK_NORMAL_ICON, MMIGAME_BOXMAN_GO_BACK_BUT_CTRL_ID),         
#endif     
	 CREATE_BUTTON_CTRL(IMAGE_BOXMAN_BUTTON, MMIGAME_BOXMAN_BUTTON_CTRL_ID),  
        CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID),         
    //   WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        END_WIN
}; 

//the boxman select map window
WINDOW_TABLE( BOXMAN_SELECT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleBoxmanSelectWinMsg ),    
    WIN_ID( BOXMAN_SELECT_WIN_ID),
    WIN_TITLE(TXT_GMBOXMAN_SELECT),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIGAME_BOXMAN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIGAME_BOXMAN_FORM1_CTRL_ID,MMIGAME_BOXMAN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIGAME_BOXMAN_LABEL1_CTRL_ID, MMIGAME_BOXMAN_FORM1_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 2,MMIGAME_BOXMAN_EDIT1_CTRL_ID, MMIGAME_BOXMAN_FORM1_CTRL_ID),	
#ifdef MMI_PDA_SUPPORT
        CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_COMMON_OK, TXT_NULL,MMICOMMON_SOFTKEY_CTRL_ID,MMIGAME_BOXMAN_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
}; 
//the window for boxman help information
WINDOW_TABLE( BOXMAN_INFO_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleBoxmanHelpWinMsg ),    
    WIN_ID( BOXMAN_INFO_WIN_ID),
    WIN_TITLE(TXT_GAME_INFO),
    CREATE_TEXT_CTRL(MMIGAME_STATIC_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

// the window for continue boxman game
WINDOW_TABLE( BOXMAN_PASS_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32) HandleBoxmanPassWinMsg ),    
        WIN_ID( BOXMAN_PASS_WIN_ID ),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
  
        END_WIN
}; 
/**---------------------------------------------------------------------------*
**                       function bodies 
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_OpenGameWin
//  Description :   
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGMBOXMAN_OpenGameWin(void)
{
#ifndef MMI_TASK_MANAGER
    return (MMK_CreateGameBoxWin((uint32*)MMIGMBOXMAIN_MENU_WIN_TAB,PNULL));
#else
    MMI_APPLET_START_T start ={0};
    
    //SCI_TRACE_LOW:"MMIAPIGMBOXMAN_OpenGameWin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_BOXMAN_WINTAB_656_112_2_18_2_21_19_0,(uint8*)"");
    start.guid = SPRD_GAME_BOX_APPLET_ID;
    start.state = /*MMI_APPLET_STATE_BG_HIDE_WIN|*/MMI_APPLET_STATE_TASK_MANAGER;
    MMK_StartApplet( &start );
    return TRUE;
#endif    
}

/*****************************************************************************/
//  Description : creat the boxman game
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxmanWinMsg( 
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_FONT_ALL_T      font_all   ={0};

//@CR NEWMS00182739
#if defined KEYPAD_TYPE_QWERTY_KEYPAD   
    uint8 keycode= (uint8)(msg_id & 0x000000ff); 
    keycode = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(keycode);
    msg_id = (msg_id & 0xffffff00) | (keycode & 0x000000ff);
#endif
//end
    msg_id = Boxman_ConvertMsg(msg_id);//处理横屏时数字按键消息转换
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        InitBoxmanWin(win_id);
        font_all.font = MMI_DEFAULT_NORMAL_FONT;
        font_all.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);

        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        // GUIWIN_SetSoftkeyTextId(win_id, COMMON_TXT_NULL,COMMON_TXT_NULL,STXT_RETURN,FALSE);
#ifdef TOUCH_PANEL_SUPPORT        
        GUIBUTTON_SetCallBackFunc(MMIGAME_BOXMAN_LAST_MAP_BUT_CTRL_ID, Boxman_Last_Map);
        GUIBUTTON_SetCallBackFunc(MMIGAME_BOXMAN_NEXT_MAP_BUT_CTRL_ID, Boxman_Next_Map);
        GUIBUTTON_SetCallBackFunc(MMIGAME_BOXMAN_GO_BACK_BUT_CTRL_ID, Boxman_Go_Back);    
#endif        
        GUIBUTTON_SetCallBackFunc(MMIGAME_BOXMAN_BUTTON_CTRL_ID , HandleReturnButtonCallback );
        GUIBUTTON_SetFont(MMIGAME_BOXMAN_BUTTON_CTRL_ID, &font_all);
        GUIBUTTON_SetTextAlign( MMIGAME_BOXMAN_BUTTON_CTRL_ID,   ALIGN_HVMIDDLE) ;
        GUIBUTTON_SetTextId(MMIGAME_BOXMAN_BUTTON_CTRL_ID,  STXT_RETURN);
        LcdSwitch(win_id);    
        if( ID_GMBOXMAN_NEW == (uint32)MMK_GetWinAddDataPtr(MMK_ConvertIdToHandle(win_id)))
        {
            boxman_current_map_num=1;
            RefreshCurrentMapData();
            mmigame_boxman_may_continue=TRUE;
        }
        else
        {
            if(!mmigame_boxman_may_continue)
            {
                boxman_current_map_num=1;
                RefreshCurrentMapData();
                mmigame_boxman_may_continue=TRUE;
            }	  
        }      
        break;

   case MSG_LCD_SWITCH:
       InitBoxmanWin(win_id);
	   LcdSwitch(win_id);
   	break;
	
    case MSG_FULL_PAINT:
      SetBackColor(win_id);	
#ifdef TOUCH_PANEL_SUPPORT 
	 GUIBUTTON_Update(MMIGAME_BOXMAN_LAST_MAP_BUT_CTRL_ID);
      GUIBUTTON_Update(MMIGAME_BOXMAN_NEXT_MAP_BUT_CTRL_ID);
      GUIBUTTON_Update(MMIGAME_BOXMAN_GO_BACK_BUT_CTRL_ID);
#endif
      if(MMITHEME_IsMainScreenLandscape())
      {
        GUIBUTTON_Update(MMIGAME_BOXMAN_BUTTON_CTRL_ID);
      }	 
      else
      {
          GUIWIN_UpdateSoftkey(win_id);
      }
	 GUILABEL_SetVisible(MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID, TRUE, TRUE);
        ShowCurrentMap(TRUE);       
        break;
        
    case MSG_KEYDOWN_CANCEL:    
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_KEYDOWN_WEB:	
    case MSG_CTL_MIDSK:
        break;
        
    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        // 背光被允许关闭
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        break;
        
    case MSG_GET_FOCUS:
        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);     
        break;
        
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_4:
        PressTowards(-1,0);
        break;
        
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_6:
        PressTowards(1,0);
        break;
        
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_2:
        PressTowards(0,-1);
        break;
        
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_8:
        PressTowards(0,1);
        break;
        
    case MSG_KEYDOWN_1:
        Boxman_Last_Map();
        break;
		
	case MSG_KEYDOWN_3:
        Boxman_Next_Map();
        break;
    case MSG_KEYDOWN_HASH:
        Boxman_Go_Back();
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        HandlePress(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : refresh current map data
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void RefreshCurrentMapData(void)
{
    uint8 i = 0; 
    uint8 j = 0;
    const uint8 boxman_all_maps_row_and_col[BOXMAN_MAP_NUMS][3] =
    {
        {"88"},{"99"},{"86"},{"88"},{"78"},{"78"},{"88"},{"78"},{"79"},{"87"},{"88"},{"96"},{"97"},{"89"},{"78"}
    };

    boxman_current_map_aim_nums = 0;
    boxman_current_map_go_back_steps = 0;
    boxman_current_map_go_back_pointer = 0;

    if(boxman_current_map_num < 1)
    {
       //SCI_TRACE_LOW:"RefreshCurrentMapData error boxman_current_map_num = %d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_BOXMAN_WINTAB_815_112_2_18_2_21_19_1,(uint8*)"d",boxman_current_map_num);
    	boxman_current_map_num = 1;
    }
    boxman_current_map_row_and_col.row = boxman_all_maps_row_and_col[boxman_current_map_num-1][0]-0x30;
    boxman_current_map_row_and_col.col = boxman_all_maps_row_and_col[boxman_current_map_num-1][1]-0x30;
    SCI_MEMSET(boxman_current_map,0,sizeof(boxman_current_map));
    SCI_MEMSET(all_aim,0,sizeof(all_aim));
    SCI_MEMSET(go_back_queue,0,sizeof(go_back_queue));
    for(i=0;i<boxman_current_map_row_and_col.row;i++)
    {
      for(j=0;j<boxman_current_map_row_and_col.col;j++)
      {
          boxman_current_map[i][j]=boxman_all_maps[boxman_current_map_num-1][i][j]-0x30;
          if(BOXMAN_AIM==boxman_current_map[i][j] ||BOXMAN_MAN_IN_AIM==boxman_current_map[i][j] ||BOXMAN_BOX_IN_AIM==boxman_current_map[i][j])
          {
              all_aim[boxman_current_map_aim_nums].y=i;
              all_aim[boxman_current_map_aim_nums].x=j;
              boxman_current_map_aim_nums++;
          }
          if(BOXMAN_MAN==boxman_current_map[i][j])
          {
              boxman_current_man.y=i;
              boxman_current_man.x=j;
          }
      }
    }
}

/*****************************************************************************/
//  Description : go last map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Boxman_Last_Map( void)
{
    if(1==boxman_current_map_num)
    {
      boxman_current_map_num=BOXMAN_MAP_NUMS+1;
    }
    boxman_current_map_num--;
    RefreshCurrentMapData();
    MMK_WinGetActive(BOXMAN_NEW_WIN_ID);
    //ShowCurrentMap(TRUE);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : go next map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Boxman_Next_Map(void)
{
    if(BOXMAN_MAP_NUMS==boxman_current_map_num)
    {
      boxman_current_map_num=0;
    }
    boxman_current_map_num++;
    RefreshCurrentMapData();

    MMK_WinGetActive(BOXMAN_NEW_WIN_ID);
    //ShowCurrentMap(TRUE);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : go back
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Boxman_Go_Back(void)
{
    if(0==boxman_current_map_go_back_steps)
    {
      return MMI_RESULT_TRUE;
    }
    if(0==boxman_current_map_go_back_pointer)
    {
      SCI_MEMCPY(boxman_current_map,go_back_queue[BOXMAN_MAX_GO_BACK_STEPS-1].then_current_map,sizeof(boxman_current_map));
      SCI_MEMCPY(&boxman_current_man,&go_back_queue[BOXMAN_MAX_GO_BACK_STEPS-1].then_current_man,sizeof(boxman_current_man));
    }
    else
    {
      SCI_MEMCPY(boxman_current_map,go_back_queue[boxman_current_map_go_back_pointer-1].then_current_map,sizeof(boxman_current_map));
      SCI_MEMCPY(&boxman_current_man,&go_back_queue[boxman_current_map_go_back_pointer-1].then_current_man,sizeof(boxman_current_man));
    }
    boxman_current_map_go_back_steps--;
    if(0==boxman_current_map_go_back_pointer)
    {
      boxman_current_map_go_back_pointer=BOXMAN_MAX_GO_BACK_STEPS-1;
    }
    else
    {
      boxman_current_map_go_back_pointer--;
    }
    MMK_WinGetActive(BOXMAN_NEW_WIN_ID);
    //ShowCurrentMap(TRUE);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : show current map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void ShowCurrentMap(BOOLEAN refresh)
{
    uint8 i = 0;
    uint8 j = 0;
    BOXMAN_POINT_T temp_point = {0};
    MMI_STRING_T num_str= {0};
    uint8 num_buf[3]      = {0};
    wchar num_wstr[3]      = {0};

    for(i=0;i<BOXMAN_MAX_MAP_ROW;i++)
    {
      temp_point.y=i;
      for(j=0;j<BOXMAN_MAX_MAP_COL;j++)
      {
          temp_point.x=j;
          ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[i][j],temp_point);
      }
    }

    sprintf((char*)num_buf,"%d",boxman_current_map_num);
    num_str.wstr_len = strlen((char*)num_buf);
    num_str.wstr_ptr = num_wstr;
    MMI_STRNTOWSTR( num_wstr, 3, num_buf, 3, num_str.wstr_len );
    
#ifdef MAINLCD_SIZE_240X400//NEWMS00123856
    GUILABEL_SetFont(MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
#endif
#if defined (MAINLCD_SIZE_176X220)||defined (MAINLCD_SIZE_128X160)&& defined (MMI_GAME_BOXMAN_MINI_SUPPORT)
    if(! MMITHEME_IsMainScreenLandscape())
    {
     GUILABEL_SetText(MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID, &num_str,refresh);
    }
    else
    {
     GUILABEL_SetFont(MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);
     GUILABEL_SetBackgroundColor(MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID,MMI_DARK_GRAY_COLOR);
     GUILABEL_SetText(MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID, &num_str,refresh);
    }
    
#else
    GUILABEL_SetText(MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID, &num_str,refresh);
#endif   
    //Rui.Zhang 2006-03-22 for avoid memory waste
  //  SCI_FREE(all_str.wstr_ptr);
  //  SCI_FREE(temp_str.wstr_ptr);
 //  GUIWIN_SetSoftkeyTextId(this_win_id, COMMON_TXT_NULL,COMMON_TXT_NULL,STXT_RETURN,TRUE);

    if (refresh)
    {
    }
}

/*****************************************************************************/
//  Description : 朝某个方向移动的处理
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void PressTowards(
int8 x_change,  //x坐标的变化值
int8 y_change   //y坐标的变化值
)
{
    int8 next_x = 0;    //隔壁点的x坐标值
    int8 next_y = 0;    //隔壁点的y坐标值
    int8 next_next_x = 0;   //隔壁的隔壁点的x坐标值
    int8 next_next_y = 0;   //隔壁的隔壁点的y坐标值

    if(-1!=x_change && 0!=x_change && 1!=x_change)
    {
      return;
    }
    if(-1!=y_change && 0!=y_change && 1!=y_change)
    {
      return;
    }
    next_x=boxman_current_man.x +x_change;
    next_y=boxman_current_man.y +y_change;
    next_next_x=boxman_current_man.x +x_change+x_change;
    next_next_y=boxman_current_man.y +y_change+y_change;
    switch(boxman_current_map[next_y][next_x])
    {
    case BOXMAN_SPACE:
      MoveManTo(x_change,y_change,BOXMAN_SPACE);
      break;
    case BOXMAN_WALL:
      PlaySorryVoice();
      break;
    case BOXMAN_BOX:
      switch(boxman_current_map[next_next_y][next_next_x])
      {
      case BOXMAN_SPACE:
          SaveSceneForGoback();
          MoveBoxTo(next_x,next_y,x_change,y_change,BOXMAN_SPACE);
          MoveManTo(x_change,y_change,BOXMAN_SPACE);
          break;
      case BOXMAN_WALL:
          PlaySorryVoice();
          break;
      case BOXMAN_BOX:
          PlaySorryVoice();
          break;
      case BOXMAN_AIM:
          SaveSceneForGoback();
          MoveBoxTo(next_x,next_y,x_change,y_change,BOXMAN_AIM);
          MoveManTo(x_change,y_change,BOXMAN_SPACE);
          IfHaveWin();
          break;
      case BOXMAN_BOX_IN_AIM:
          PlaySorryVoice();
          break;
      default:
          MapError();
          break;
      }
      break;
      case BOXMAN_AIM:
          MoveManTo(x_change,y_change,BOXMAN_AIM);
          break;
      case BOXMAN_BOX_IN_AIM:
          switch(boxman_current_map[next_next_y][next_next_x])
          {
          case BOXMAN_SPACE:
              SaveSceneForGoback();
              MoveBoxTo(next_x,next_y,x_change,y_change,BOXMAN_SPACE);
              MoveManTo(x_change,y_change,BOXMAN_AIM);
              break;
          case BOXMAN_WALL:
              PlaySorryVoice();
              break;
          case BOXMAN_BOX:
              PlaySorryVoice();
              break;
          case BOXMAN_AIM:
              SaveSceneForGoback();
              MoveBoxTo(next_x,next_y,x_change,y_change,BOXMAN_AIM);
              MoveManTo(x_change,y_change,BOXMAN_AIM);
              IfHaveWin();
              break;
          case BOXMAN_BOX_IN_AIM:
              PlaySorryVoice();
              break;
          default:
              MapError();
              break;
          }
          break;
          default:
              MapError();
              break;
    }
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : deal with press event
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void HandlePress(DPARAM param)
{
    int8 i = 0;
    int8 x_difference = 0;
    int8 y_difference = 0;
    MOVE_QUEUE_T move_queue = {0};  //移动队列
    BOXMAN_POINT_T press_point = {0};//触笔点在地图中的坐标

    if (IfPressInMap(param))
    {
      press_point=PressAt(param);
      x_difference=press_point.x-boxman_current_man.x;
      y_difference=press_point.y-boxman_current_man.y;
      if((-1==x_difference && 0==y_difference) || (1==x_difference && 0==y_difference) \
          || (0==x_difference && -1==y_difference) || (0==x_difference && 1==y_difference)) //等同于按键左右上下
      {
          PressTowards(x_difference,y_difference);
      }
      else
      {
          if(!(0==x_difference && 0==y_difference)) //不是点在当前小人上
          {
              if(BOXMAN_SPACE==boxman_current_map[press_point.y][press_point.x] || BOXMAN_AIM==boxman_current_map[press_point.y][press_point.x])
              {
                  ClearMoveQueue(&move_queue);
                  move_queue=FindMovePath(press_point);
                  if(0!=move_queue.move_num)
                  {
                      for(i=0;i<move_queue.move_num;i++)
                      {
                          switch(boxman_current_map[(move_queue.queue[i].y)][(move_queue.queue[i].x)])
                          {
                          case BOXMAN_SPACE:
                              MoveManTo(move_queue.queue[i].x-boxman_current_man.x,move_queue.queue[i].y-boxman_current_man.y,BOXMAN_SPACE);
                              MMITHEME_UpdateRect();
                              SCI_Sleep(50);
                              break;
                          case BOXMAN_AIM:
                              MoveManTo(move_queue.queue[i].x-boxman_current_man.x,move_queue.queue[i].y-boxman_current_man.y,BOXMAN_AIM);
                              MMITHEME_UpdateRect();
                              SCI_Sleep(50);
                              break;
                          default:
                              MapError();
                              return;
                          }
                      }
                  }
              }
              else
              {
                  return;
              }
          }
      }
    }
}
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : calculate the coordinates of the press point in map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL BOXMAN_POINT_T PressAt(
  DPARAM param
  )
{
    uint16 x = 0;
    uint16 y = 0;
	uint16 point_x = 0;
	uint16 point_y = 0;
    BOXMAN_POINT_T press_point = {0};//触笔点在地图中的坐标

    x = MMK_GET_TP_X(param);
    y = MMK_GET_TP_Y(param);


    if(MMITHEME_IsMainScreenLandscape())//modify by mary
	{
		point_x = BOXMAN_MAP_LEFT_UP_H_X;
		point_y = BOXMAN_MAP_LEFT_UP_H_Y;
	}
    else
    {
        point_x =BOXMAN_MAP_LEFT_UP_X;
        point_y = BOXMAN_MAP_LEFT_UP_Y;		
    }

    press_point.x = (x-point_x)/BOXMAN_SUB_MAP_BORDER_WIDTH;
    press_point.y = (y-point_y)/BOXMAN_SUB_MAP_BORDER_HIGHT;


    return press_point;
}

/*****************************************************************************/
//  Description : clear move queue
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void ClearMoveQueue(
  MOVE_QUEUE_T *move_queue
  )
{
    (*move_queue).move_num=0;
    SCI_MEMSET((*move_queue).queue,0,(BOXMAN_MAX_MOVE_STEP*sizeof(BOXMAN_POINT_T)));
}
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : find move path
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MOVE_QUEUE_T FindMovePath(
  BOXMAN_POINT_T press_point
  )
{
    uint8 search_step = 0;
    BOXMAN_POINT_T pos[BOXMAN_MAX_SEARCH_STEP] = {0};
    BOXMAN_POINT_T temp_point = {0};
    MOVE_QUEUE_T move_queue = {0};  //移动队列
    int8 from[BOXMAN_MAX_SEARCH_STEP] = {0};
    int8 now = 0;
    int8 end = 0;
    int8 temp_x = 0;
    int8 temp_y = 0;
    uint8 i = 0;
    uint8 j = 0;
    BOOLEAN have = FALSE;

    search_step = 0;
    pos[search_step].x=press_point.x;
    pos[search_step].y=press_point.y;
    from[search_step]=-1;
    search_step++;
    now=0;
    end=0;
    while(now<=end)
    {
      temp_point.x=pos[now].x;
      temp_point.y=pos[now].y;
      for(i=0;4>i;i++)
      {
          temp_x=temp_point.x+s_x_change[i];
          temp_y=temp_point.y+s_y_change[i];
          if(temp_x==boxman_current_man.x && temp_y==boxman_current_man.y)
          {
              ClearMoveQueue(&move_queue);
              while(-1!=now)
              {
                  move_queue.queue[move_queue.move_num].x=pos[now].x;
                  move_queue.queue[move_queue.move_num].y=pos[now].y;
                  now=from[now];
                  move_queue.move_num++;
              }
              return move_queue;
          }
          if(BOXMAN_SPACE==boxman_current_map[temp_y][temp_x] || BOXMAN_AIM==boxman_current_map[temp_y][temp_x])
          {
              have=FALSE;
              for(j=0;!have && j<=end;j++)
              {
                  if(pos[j].x==temp_x && pos[j].y==temp_y)
                  {
                      have=TRUE;
                  }
              }
              if(!have)
              {
                  pos[search_step].x=temp_x;
                  pos[search_step].y=temp_y;
                  from[search_step]=now;
                  search_step++;
                  end++;
              }
          }
      }
      now++;
    }
    ClearMoveQueue(&move_queue);

    return move_queue;
}
#endif
/*****************************************************************************/
//  Description : show one sub map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void ShowSubMap(
  BOXMAN_MAP_DATA_VALUE_E sub_map_num,
  BOXMAN_POINT_T position_point
  )
{    
    GUI_POINT_T                 dis_point = {0};
	uint16 point_x = 0;
	uint16 point_y = 0;
	const MMI_IMAGE_ID_T boxman_all_maps_name[BOXMAN_SUB_MAP_NUMS] =
    {
        IMAGE_BOXMAN_WALLOUT,
        IMAGE_BOXMAN_SPACE,
        IMAGE_BOXMAN_WALL,
        IMAGE_BOXMAN_MAN,
        IMAGE_BOXMAN_BOX,
        IMAGE_BOXMAN_AIM,
        IMAGE_BOXMAN_MAN_IN_AIM,
        IMAGE_BOXMAN_BOX_IN_AIM
    };
    const GUI_LCD_DEV_INFO*     lcd_dev_info_ptr = MMITHEME_GetDefaultLcdDev();

    if(MMITHEME_IsMainScreenLandscape())//modify by mary
    {
        point_x = BOXMAN_MAP_LEFT_UP_H_X;
        point_y = BOXMAN_MAP_LEFT_UP_H_Y;
    }
    else
    {
        point_x =BOXMAN_MAP_LEFT_UP_X;
        point_y = BOXMAN_MAP_LEFT_UP_Y;	
    }

    dis_point.x = point_x+BOXMAN_SUB_MAP_BORDER_WIDTH*position_point.x;
    dis_point.y = point_y+BOXMAN_SUB_MAP_BORDER_HIGHT*position_point.y;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        BOXMAN_NEW_WIN_ID,
        boxman_all_maps_name[sub_map_num],
        lcd_dev_info_ptr);
}


/*****************************************************************************/
//  Description : if have win
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void IfHaveWin(void)
{
    uint8           i = 0;
    for(i=0;i<boxman_current_map_aim_nums;i++)
    {
      if(BOXMAN_BOX_IN_AIM!=boxman_current_map[all_aim[i].y][all_aim[i].x])
      {
          return;
      }
    }
    if (BOXMAN_MAP_NUMS == boxman_current_map_num)//最后一关,显示通关
    {
        MMK_CreateGameBoxWin((uint32 *) BOXMAN_PASS_WIN_TAB, PNULL);
        mmigame_boxman_may_continue = FALSE;
    }
    else
    {
        MMITHEME_UpdateRect();
        SCI_Sleep(1000);
        boxman_current_map_num++;
        RefreshCurrentMapData();
        MMK_SendMsg(MMK_ConvertIdToHandle(BOXMAN_NEW_WIN_ID) ,MSG_FULL_PAINT, PNULL) ;           
        //ShowCurrentMap(TRUE);
    }
}

/*****************************************************************************/
// 	Description : 文本风格
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL __inline void SetBoxmanTextStyle(
                                       GUISTR_STYLE_T *style_ptr,
                                       GUI_ALIGN_E align,
                                       GUI_FONT_T  font,
                                       GUI_COLOR_T font_color
                                       )
{
    //SCI_ASSERT(PNULL != style_ptr);
    if(PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"[mmigame_boxman_wintab.c]:SetBoxmanTextStyle(PNULL == style_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_BOXMAN_WINTAB_1320_112_2_18_2_21_21_2,(uint8*)"");
        return;
    }
    style_ptr->align = align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 0;
}

/*****************************************************************************/
//  FUNCTION:     DrawGameFinishScreen
//  Description :   dispaly game finish screen
//  Global resource dependence : 
//  Author:       zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void DrawBoxmanFinishScreen(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          rect         = MMITHEME_GetClientRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};

	rect.top =  0;
#if defined MAINLCD_SIZE_240X400 //modify by mary
		LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);//modify by mary
#else
    GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
#endif
         

    MMIRES_GetText( TXT_GAME_FINISH, win_id, &string );

    SetBoxmanTextStyle(&text_style, ALIGN_HVMIDDLE, MMI_DEFAULT_NORMAL_FONT, MMI_DARK_WHITE_COLOR);
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
//  FUNCTION:     InitBoxmanWin
//  Description :   
//  Global resource dependence : 
//  Author:       ying.xu
//  Note:   
/*****************************************************************************/
LOCAL void InitBoxmanWin(MMI_WIN_ID_T win_id) //modify by mary
{    
#ifdef TOUCH_PANEL_SUPPORT
    GUI_RECT_T    button_last_rect = {0};
    GUI_RECT_T    button_next_rect = {0};
    GUI_RECT_T    button_goback_rect = {0};
#if !defined(MAINLCD_SIZE_240X400)
    GUI_RECT_T    client_rect = MMITHEME_GetClientRect();
#endif
#endif
    GUI_RECT_T    lab_rect =  {0} ;    
    GUI_RECT_T    return_button_rect =  {0} ;   
#if !defined(MAINLCD_SIZE_240X400)
    GUI_RECT_T	  full_rect = MMITHEME_GetFullScreenRect();
#else
#ifndef TOUCH_PANEL_SUPPORT
    GUI_RECT_T	  full_rect = MMITHEME_GetFullScreenRect();
#endif
#endif
    uint16        button_height = 0;
    uint16        button_width = 0;	
//	uint16        font_height = GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT);
//	uint16        button_space = 0;
	 
	 
    if(! MMITHEME_IsMainScreenLandscape())
    {	
#ifdef TOUCH_PANEL_SUPPORT        
        {
#if defined MAINLCD_SIZE_240X400
            button_last_rect.left = BOXMAN_BUTTON_LAST_RECT_LEFT;
            button_last_rect.top =  BOXMAN_BUTTON_LAST_RECT_TOP ;
            GUIRES_GetImgWidthHeight(&button_width, &button_height, 
                IMAGE_COMMON_COUNT_LBTN_UN , win_id);
            button_last_rect.bottom = button_last_rect.top+button_height;		  
            button_last_rect.right =  button_last_rect.left + button_width  ;

            button_goback_rect.left = BOXMAN_BUTTON_GOBACK_RECT_LEFT ;
            button_goback_rect.top = BOXMAN_BUTTON_GOBACK_RECT_TOP;
            GUIRES_GetImgWidthHeight(&button_width, &button_height,
                IMAGE_BOXMAN_GO_BACK_NORMAL_ICON , win_id);
            button_goback_rect.right=  button_goback_rect.left+button_width;
            button_goback_rect.bottom= button_goback_rect.top+button_height;

            button_next_rect.left =  BOXMAN_BUTTON_NEXT_RECT_LEFT;
            button_next_rect.top =BOXMAN_BUTTON_NEXT_RECT_TOP; 
            GUIRES_GetImgWidthHeight(&button_width, &button_height,
                IMAGE_COMMON_COUNT_RBTN_UN , win_id);	
            button_next_rect.right =  button_next_rect.left + button_width;
            button_next_rect.bottom = button_next_rect.top+button_height;
#else
            button_last_rect.left = 4;
            button_last_rect.bottom = client_rect.bottom - 1;		  
            GUIRES_GetImgWidthHeight(&button_width, &button_height, 
                IMAGE_COMMON_COUNT_LBTN_UN , win_id);
            button_last_rect.right =  button_last_rect.left + button_width;
            button_last_rect.top =  button_last_rect.bottom -button_height;           

            button_goback_rect.right = full_rect.right - 4;
            button_goback_rect.bottom = client_rect.bottom - 1;	
            GUIRES_GetImgWidthHeight(&button_width, &button_height,
                IMAGE_BOXMAN_GO_BACK_NORMAL_ICON , win_id);
            button_goback_rect.left = button_goback_rect.right -button_width;
            button_goback_rect.top = button_goback_rect.bottom -button_height;

            button_next_rect.right =  button_goback_rect.left - 4;
            button_next_rect.bottom = client_rect.bottom - 1;	
            GUIRES_GetImgWidthHeight(&button_width, &button_height,
                IMAGE_COMMON_COUNT_RBTN_UN , win_id);		 
            button_next_rect.left =  button_next_rect.right - button_width;
            button_next_rect.top = button_next_rect.bottom - button_height;        
#endif       
            lab_rect.left = button_last_rect.right + 4;
            lab_rect.right =  button_next_rect.left - 4;	  
            lab_rect.top =    button_next_rect.top;
            lab_rect.bottom =   button_next_rect.bottom;
        }        
#else
        {
            lab_rect = MMITHEME_GetClientRect();
#ifdef MAINLCD_SIZE_128X160
            lab_rect.top =     lab_rect.bottom - 12;
#elif defined(MAINLCD_SIZE_176X220)
            lab_rect.top =     lab_rect.bottom - 30;
#else
            lab_rect.top =     lab_rect.bottom - 40;
#endif
        }
#endif
    }
    else
    {
#if defined MAINLCD_SIZE_240X400	
        return_button_rect.left =  BOXMAN_BUTTON_RETURN_RECT_LEFT;
        return_button_rect.top =  BOXMAN_BUTTON_RETURN_RECT_TOP;
        GUIRES_GetImgWidthHeight(&button_width, &button_height, IMAGE_BOXMAN_BUTTON , win_id); 
        return_button_rect.right=return_button_rect.left+button_width;
        return_button_rect.bottom= return_button_rect.top+button_height;
#else

        GUIRES_GetImgWidthHeight(&button_width, &button_height, IMAGE_BOXMAN_BUTTON , win_id);
        return_button_rect.right  = full_rect.right - (BOXMAN_LABEL_WIDTH >> 1) + (button_width >>1) ;
#if defined (MAINLCD_SIZE_176X220)
        return_button_rect.bottom =  full_rect.bottom - 7;  //juexue zhu
#elif defined (MAINLCD_SIZE_128X160)
        return_button_rect.bottom =  full_rect.bottom - 1;
#else
        return_button_rect.bottom =  full_rect.bottom - 4;
#endif
#if defined (MAINLCD_SIZE_128X160)
        return_button_rect.left =  return_button_rect.right - button_width-12;
#else
        return_button_rect.left =  return_button_rect.right - button_width;
#endif
        return_button_rect.top =  return_button_rect.bottom -  button_height; 
#endif  	

        GUIBUTTON_SetRect(MMIGAME_BOXMAN_BUTTON_CTRL_ID, &return_button_rect);

#ifdef TOUCH_PANEL_SUPPORT  
#if defined MAINLCD_SIZE_240X400

        button_last_rect.left = BOXMAN_BUTTON_LAST_RECT_H_LEFT ;
        button_last_rect.top = BOXMAN_BUTTON_LAST_RECT_H_TOP; 
        GUIRES_GetImgWidthHeight(&button_width, &button_height,
            IMAGE_COMMON_COUNT_LBTN_UN , win_id); 
        button_last_rect.right = button_last_rect.left+ button_width;          
        button_last_rect.bottom = button_last_rect.top + button_height;


        lab_rect.left = BOXMAN_BUTTON_LAST_RECT_H_LEFT;
        lab_rect.right =   lab_rect.left + 30;
        lab_rect.top =   button_last_rect.bottom + 8;           
        lab_rect.bottom =   lab_rect.top + 40 ;

        button_next_rect.left = BOXMAN_BUTTON_NEXT_RECT_H_LEFT;
        button_next_rect.top =     BOXMAN_BUTTON_NEXT_RECT_H_TOP;			
        button_next_rect.bottom =  button_next_rect.top +  button_height;
        button_next_rect.right = button_next_rect.left+button_width;

        button_goback_rect.left = BOXMAN_BUTTON_GOBACK_RECT_H_LEFT;
        button_goback_rect.top =  BOXMAN_BUTTON_GOBACK_RECT_H_TOP; 
        GUIRES_GetImgWidthHeight(&button_width, &button_height,
            IMAGE_BOXMAN_GO_BACK_NORMAL_ICON , win_id); 
        button_goback_rect.right =  button_goback_rect.left + button_width;         
        button_goback_rect.bottom =  button_goback_rect.top + button_height ;
#else  
        GUIRES_GetImgWidthHeight(&button_width, &button_height,
            IMAGE_COMMON_COUNT_LBTN_UN , win_id);     
        button_last_rect.right = full_rect.right - (BOXMAN_LABEL_WIDTH >> 1) + (button_width >>1) ;
        button_last_rect.left = button_last_rect.right - button_width ;
        button_last_rect.top = full_rect.top + 8;            
        button_last_rect.bottom = button_last_rect.top + button_height;

        lab_rect.right =   full_rect.right - 4;
#if defined MAINLCD_SIZE_240X320
        lab_rect.left = lab_rect.right  - BOXMAN_LABEL_WIDTH + BOXMAN_LABEL_LEFT_FIT;
#else
        lab_rect.left = lab_rect.right  - BOXMAN_LABEL_WIDTH;
#endif
        lab_rect.top =   button_last_rect.bottom + 8 ;           
        lab_rect.bottom =   lab_rect.top + 40 ;

        GUIRES_GetImgWidthHeight(&button_width, &button_height,
            IMAGE_COMMON_COUNT_RBTN_UN , win_id);		 
        button_next_rect.right = full_rect.right - (BOXMAN_LABEL_WIDTH >> 1) + (button_width >>1) ;
        button_next_rect.left = button_next_rect.right - button_width ;
        button_next_rect.top =     lab_rect.bottom + 8 ;			
        button_next_rect.bottom = button_next_rect.top +button_height ;

        GUIRES_GetImgWidthHeight(&button_width, &button_height,
            IMAGE_BOXMAN_GO_BACK_NORMAL_ICON , win_id);  
        button_goback_rect.right =  full_rect.right - (BOXMAN_LABEL_WIDTH >> 1) + (button_width >>1) ;
        button_goback_rect.left = button_goback_rect.right - button_width;
    #ifdef MMI_PDA_SUPPORT
        button_goback_rect.bottom  = client_rect.bottom - 1;
        button_goback_rect.top     = button_goback_rect.bottom - button_height;
    #else
        button_goback_rect.top =  button_next_rect.bottom + 8;          
        button_goback_rect.bottom =  button_goback_rect.top + button_height ;
    #endif
#endif
#else

#if defined (MAINLCD_SIZE_176X220) && defined (MMI_GAME_BOXMAN_MINI_SUPPORT)
        lab_rect.top =   6;
        lab_rect.right =  214;
        lab_rect.left = 173;
        lab_rect.bottom =   148;
#elif defined (MAINLCD_SIZE_128X160) && defined (MMI_GAME_BOXMAN_MINI_SUPPORT)
		lab_rect.top =   2;
        lab_rect.right =  157;
        lab_rect.left = 128;
        lab_rect.bottom = 108;

#else
        lab_rect.top =   full_rect.top + 8;
        lab_rect.right =   full_rect.right-4;
        lab_rect.left = full_rect.right - BOXMAN_LABEL_WIDTH;
        lab_rect.bottom =   lab_rect.top +40;
#endif
#endif
    }    	
#ifdef TOUCH_PANEL_SUPPORT    	
    GUIBUTTON_SetRect(MMIGAME_BOXMAN_LAST_MAP_BUT_CTRL_ID,&button_last_rect);
    GUIBUTTON_SetRect(MMIGAME_BOXMAN_NEXT_MAP_BUT_CTRL_ID,&button_next_rect);
    GUIBUTTON_SetRect(MMIGAME_BOXMAN_GO_BACK_BUT_CTRL_ID,&button_goback_rect);   
#endif
    GUILABEL_SetRect(MMIGAME_BOXMAN_MAP_NUM_LABEL_CTRL_ID, &lab_rect, FALSE);
}

/*****************************************************************************/
//  FUNCTION:     HandleBoxmanPassWinMsg
//  Description :   win which dispaly game finish screen
//  Global resource dependence : 
//  Author:       zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxmanPassWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
        if (s_game_pass_timer != 0)
        {
            MMK_StopTimer(s_game_pass_timer);
            s_game_pass_timer = 0;
        }
        s_game_pass_timer = MMK_CreateTimer(MMI_2SECONDS,FALSE);
        break;

    case MSG_FULL_PAINT:
        DrawBoxmanFinishScreen(win_id);
    break;
    
    case MSG_TIMER:
		if (s_game_pass_timer ==  *( GUI_TIMER_ID_T*)param)
        {
            MMK_StopTimer(s_game_pass_timer);
            s_game_pass_timer = 0;
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        if (s_game_pass_timer > 0)
        {
            MMK_StopTimer(s_game_pass_timer);
            s_game_pass_timer = 0;
        }
        MMK_CloseWin(BOXMAN_NEW_WIN_ID);
        MMK_CloseWin(BOXMAN_RES_WIN_ID);
        break;

    case MSG_LOSE_FOCUS:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;
      
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 朝某个方向移动箱子的处理
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void MoveBoxTo(
  uint8 box_x,      //箱子的x坐标值
  uint8 box_y,      //箱子的y坐标值
  int8 x_change,    //x坐标的变化值
  int8 y_change,    //y坐标的变化值
  BOXMAN_MAP_DATA_VALUE_E aim_box_primary_value     //目标箱子位置原来的地图数据值
  )
{
    uint8 aim_box_x = 0;        //目标箱子位置的x坐标值
    uint8 aim_box_y = 0;        //目标箱子位置的y坐标值
    BOXMAN_MAP_DATA_VALUE_E aim_box_new_value = BOXMAN_WALLOUT; //目标箱子位置的将要变为的地图数据值
    BOXMAN_POINT_T temp_point = {0};

    aim_box_x=box_x+x_change;
    aim_box_y=box_y+y_change;
    switch(aim_box_primary_value)
    {
    case BOXMAN_SPACE:
      aim_box_new_value=BOXMAN_BOX;
      break;
      
    case BOXMAN_AIM:
      aim_box_new_value=BOXMAN_BOX_IN_AIM;
      break;
      
    default:
      return;
    }

    switch(boxman_current_map[box_y][box_x])
    {
    case BOXMAN_BOX:
      boxman_current_map[box_y][box_x]=BOXMAN_SPACE;
      temp_point.x=box_x;
      temp_point.y=box_y;
      ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[box_y][box_x],temp_point);
      boxman_current_map[aim_box_y][aim_box_x]=aim_box_new_value;
      temp_point.x=aim_box_x;
      temp_point.y=aim_box_y;
      ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[aim_box_y][aim_box_x],temp_point);
      break;
      
    case BOXMAN_BOX_IN_AIM:
      boxman_current_map[box_y][box_x]=BOXMAN_AIM;
      temp_point.x=box_x;
      temp_point.y=box_y;
      ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[box_y][box_x],temp_point);
      boxman_current_map[aim_box_y][aim_box_x]=aim_box_new_value;
      temp_point.x=aim_box_x;
      temp_point.y=aim_box_y;
      ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[aim_box_y][aim_box_x],temp_point);
      break;
      
    default:
      break;
    }
}

/*****************************************************************************/
//  Description : deal with map error
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void MapError(void)
{
}

/*****************************************************************************/
//  Description : save scene for go back
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void SaveSceneForGoback(void)
{
    SCI_MEMCPY(go_back_queue[boxman_current_map_go_back_pointer].then_current_map,boxman_current_map,sizeof(boxman_current_map));
    SCI_MEMCPY(&go_back_queue[boxman_current_map_go_back_pointer].then_current_man,&boxman_current_man,sizeof(boxman_current_man));
    if(BOXMAN_MAX_GO_BACK_STEPS>boxman_current_map_go_back_steps)
    {
      boxman_current_map_go_back_steps++;
    }
    boxman_current_map_go_back_pointer++;
    if(BOXMAN_MAX_GO_BACK_STEPS==boxman_current_map_go_back_pointer)
    {
      boxman_current_map_go_back_pointer=0;
    }
}

/*****************************************************************************/
//  Description : play sorry voice
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void PlaySorryVoice(void)
{
}

/*****************************************************************************/
//  Description : 朝某个方向移动小人的处理
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void MoveManTo(
  int8 x_change,    //x坐标的变化值
  int8 y_change,    //y坐标的变化值
  BOXMAN_MAP_DATA_VALUE_E aim_man_primary_value     //目标小人位置原来的地图数据值
  )
{
    uint8 aim_man_x = 0;    //目标小人位置的x坐标值
    uint8 aim_man_y = 0;    //目标小人位置的y坐标值
    BOXMAN_MAP_DATA_VALUE_E aim_man_new_value = BOXMAN_WALLOUT; //目标小人位置的将要变为的地图数据值

    aim_man_x=boxman_current_man.x+x_change;
    aim_man_y=boxman_current_man.y+y_change;
    switch(aim_man_primary_value)
    {
    case BOXMAN_SPACE:
      aim_man_new_value=BOXMAN_MAN;
      break;
      
    case BOXMAN_AIM:
      aim_man_new_value=BOXMAN_MAN_IN_AIM;
      break;
      
    default:
      return;
    }

    switch(boxman_current_map[boxman_current_man.y][boxman_current_man.x])
    {
    case BOXMAN_MAN:
      boxman_current_map[boxman_current_man.y][boxman_current_man.x]=BOXMAN_SPACE;
      ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[boxman_current_man.y][boxman_current_man.x],boxman_current_man);
      PlayMoveManVoice();
      boxman_current_map[aim_man_y][aim_man_x]=aim_man_new_value;
      boxman_current_man.x=aim_man_x;
      boxman_current_man.y=aim_man_y;
      ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[boxman_current_man.y][boxman_current_man.x],boxman_current_man);
      break;
      
    case BOXMAN_MAN_IN_AIM:
      boxman_current_map[boxman_current_man.y][boxman_current_man.x]=BOXMAN_AIM;
      ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[boxman_current_man.y][boxman_current_man.x],boxman_current_man);
      PlayMoveManVoice();
      boxman_current_map[aim_man_y][aim_man_x]=aim_man_new_value;
      boxman_current_man.x=aim_man_x;
      boxman_current_man.y=aim_man_y;
      ShowSubMap((BOXMAN_MAP_DATA_VALUE_E)boxman_current_map[boxman_current_man.y][boxman_current_man.x],boxman_current_man);
      break;
      
    default:
      break;
    }
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : if press in current map,not include the border of the current map
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IfPressInMap(DPARAM param)
{
    uint16 x = 0;
    uint16 y = 0;
	uint16 point_x = 0;
	uint16 point_y = 0;

    x = MMK_GET_TP_X(param);
    y = MMK_GET_TP_Y(param);

    if(MMITHEME_IsMainScreenLandscape())//modify by mary
    {
        point_x = BOXMAN_MAP_LEFT_UP_H_X;
        point_y = BOXMAN_MAP_LEFT_UP_H_Y;
    }
    else
    {
        point_x =BOXMAN_MAP_LEFT_UP_X;
        point_y = BOXMAN_MAP_LEFT_UP_Y;	
    }

    if((point_x+BOXMAN_SUB_MAP_BORDER_WIDTH)<x && (point_x+(boxman_current_map_row_and_col.col-1)*BOXMAN_SUB_MAP_BORDER_WIDTH)>x \
      && (point_y+BOXMAN_SUB_MAP_BORDER_HIGHT)<y && (point_y+(boxman_current_map_row_and_col.row-1)*BOXMAN_SUB_MAP_BORDER_HIGHT)>y)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }

}
#endif
/*****************************************************************************/
//  Description : play move man voice
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void PlayMoveManVoice(void)
{
}

/*****************************************************************************/
//  Description : HandleQuintetMainMenuWindow
//       
//  Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleGMBOXMainMenuWindow(
  MMI_WIN_ID_T win_id, 
  MMI_MESSAGE_ID_E msg_id, 
  DPARAM param
  )
  
{
    uint32          group_id = 0; 
    uint32          menu_id  = 0;
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
    //MMI_STRING_T    result   = {0};
    //uint16          nv_return = 0;
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
        MMK_SetAtvCtrl(win_id, MMIGMBOXMAN_MAIN_MENU_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        GUIMENU_GetId(MMIGMBOXMAN_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
        switch(menu_id) 
        {
        case ID_GMBOXMAN_NEW:
            MMK_CreateGameBoxWin((uint32 *)BOXMAN_NEW_WIN_TAB,(ADD_DATA)(ID_GMBOXMAN_NEW));
            break;
            
        case ID_GMBOXMAN_RES:
            MMK_CreateGameBoxWin((uint32 *)BOXMAN_NEW_WIN_TAB, (ADD_DATA)(ID_GMBOXMAN_RES));
            break;

        case ID_GMBOXMAN_SELECT:
            MMK_CreateGameBoxWin((uint32 *)BOXMAN_SELECT_WIN_TAB, PNULL);
            break;

        case ID_GMBOXMAN_INFO:
            MMK_CreateGameBoxWin((uint32 *) BOXMAN_INFO_WIN_TAB, PNULL);
            break;

        default:
            break;
        }
    break;

	case MSG_KEYDOWN_1:
	//case MSG_KEYDOWN_R:
			MMK_CreateGameBoxWin((uint32 *)BOXMAN_NEW_WIN_TAB,(ADD_DATA)(ID_GMBOXMAN_NEW));
			break;
	case MSG_KEYDOWN_2:
   //case MSG_KEYDOWN_T:
			MMK_CreateGameBoxWin((uint32 *)BOXMAN_NEW_WIN_TAB, (ADD_DATA)(ID_GMBOXMAN_RES));
			break;
	case MSG_KEYDOWN_3:
	//case MSG_KEYDOWN_Y:	         
			MMK_CreateGameBoxWin((uint32 *)BOXMAN_SELECT_WIN_TAB, PNULL);
			break;
	case MSG_KEYDOWN_4:
	//case MSG_KEYDOWN_F:	         
			MMK_CreateGameBoxWin((uint32 *) BOXMAN_INFO_WIN_TAB, PNULL);
			break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifndef MMI_TASK_MANAGER
        MMK_CloseWin(win_id);
#else
        MMIAPIGMBOX_Stop(FALSE);
#endif
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : set back color
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void SetBackColor(MMI_WIN_ID_T win_id)
{   
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; 
    GUI_RECT_T rect =    MMITHEME_GetFullScreenRect();
#if defined MAINLCD_SIZE_240X400 //modify by mary
    LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);//modifyby mary
#elif defined MMI_PDA_SUPPORT
    LCD_FillRect(&lcd_dev_info, rect, 0X5b2e);//PDA换个颜色 
#else       
    GUIRES_DisplayImg(PNULL,
        &rect,
        &rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
#endif
}

/*****************************************************************************/
//  Description : the boxman select map window
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxmanSelectWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint8 default_map = 1;
    uint8 default_map_num[BOXMAN_MAX_SELECT_MAP_NUM_LEN+1] = {0};
    wchar default_map_num_wstr[BOXMAN_MAX_SELECT_MAP_NUM_LEN+1] = {0};
    MMI_STRING_T select_tip = {0};
    uint16 len = 0;
    BOOLEAN         is_disp_im_icon = FALSE;
	BOOLEAN         is_disp_num_info = FALSE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMIGAME_BOXMAN_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
        
        MMI_GetLabelTextByLang(TXT_GMBOXMAN_SELECT_TIP, &select_tip);      
        GUILABEL_SetText(MMIGAME_BOXMAN_LABEL1_CTRL_ID,&select_tip,FALSE);
        MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), 
        MMK_ConvertIdToHandle(MMIGAME_BOXMAN_EDIT1_CTRL_ID));
		GUIEDIT_SetImTag(MMIGAME_BOXMAN_EDIT1_CTRL_ID, GUIIM_TAG_PURE_NUM);		//needn't use dot
        s_input_map_num = (s_input_map_num==0)?default_map:s_input_map_num;
        sprintf((char*)default_map_num, "%d", s_input_map_num);        
        len = strlen((char*)default_map_num);
        MMI_STRNTOWSTR( default_map_num_wstr, BOXMAN_MAX_SELECT_MAP_NUM_LEN, 
			default_map_num, BOXMAN_MAX_SELECT_MAP_NUM_LEN, len );
        /* -fixed by hongjun.jia for NEWMS00221349 */
        /* -在推箱子游戏选关界面，不再显示“123”输入法图标*/
        GUIEDIT_SetDispImIcon(MMIGAME_BOXMAN_EDIT1_CTRL_ID, &is_disp_im_icon, &is_disp_num_info);
        GUIEDIT_SetString(
            MMIGAME_BOXMAN_EDIT1_CTRL_ID,
            default_map_num_wstr,
            len
            );
        break;
        
    case MSG_LOSE_FOCUS:
        {
            uint8 duar_buf[8] = {0};
            MMI_STRING_T str_info = {0};
            GUIEDIT_GetString(MMIGAME_BOXMAN_EDIT1_CTRL_ID, &str_info);
            if ((0 < str_info.wstr_len) && (PNULL != str_info.wstr_ptr))
            {
                MMI_WSTRNTOSTR( duar_buf, 8, str_info.wstr_ptr, str_info.wstr_len, str_info.wstr_len );
                s_input_map_num = atoi((char *)(duar_buf));
            }
        }
        break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
    if (PNULL != param)
    {
        uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
        switch (src_id)
        {
        case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
            MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            break;
            
        default:
            break;
        }
    }
        break;
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 

     case MSG_CTL_OK:
     case MSG_APP_OK:
     case MSG_CTL_MIDSK:
        Mmigame_Boxman_Select_Confirm();
        break;        
        
    case MSG_CTL_CANCEL: 
    case MSG_KEYDOWN_CANCEL: 
        Mmigame_Boxman_Select_Cancel();
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
//  Description : do select map cancel button
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void Mmigame_Boxman_Select_Cancel(void)
{
    MMK_CloseWin(BOXMAN_SELECT_WIN_ID);
}

/*****************************************************************************/
//  Description : do select map confirm button
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL void Mmigame_Boxman_Select_Confirm(void)
{
    MMI_STRING_T str_info = {PNULL, 0};
    uint8 select_num = 0;
    uint8 duar_buf[8] = {0};
    wchar wstr_buf[8] = {0} ;
    const char* str_class1 =  "1" ;
    uint16 length = 0;
    
    GUIEDIT_GetString(MMIGAME_BOXMAN_EDIT1_CTRL_ID, &str_info);
    if ((0 < str_info.wstr_len) && (PNULL != str_info.wstr_ptr))
    {
        MMI_WSTRNTOSTR( duar_buf, 8, str_info.wstr_ptr, str_info.wstr_len, str_info.wstr_len );
        select_num = atoi((char *)(duar_buf));
        //        select_num=atoi((char*)str_info.str_ptr);
        if(1 <= select_num && BOXMAN_MAP_NUMS >= select_num)
        {
            boxman_current_map_num = select_num;
            RefreshCurrentMapData();
            mmigame_boxman_may_continue = TRUE;
            // MMK_CreateWin((uint32*)BOXMAN_NEW_WIN_TAB, PNULL);
            MMK_CreateGameBoxWin((uint32 *)BOXMAN_NEW_WIN_TAB,(ADD_DATA)(ID_GMBOXMAN_RES));
            MMK_CloseWin(BOXMAN_SELECT_WIN_ID);
        }
        else
        {
#ifndef MMI_TASK_MANAGER
            MMIPUB_OpenAlertWarningWin(TXT_GMBOXMAN_MAP_INVALID);
#else
            MMIPUB_OpenAlertWinByTextIdEx(SPRD_GAME_BOX_APPLET_ID, 
                    PNULL,TXT_GMBOXMAN_MAP_INVALID,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,
                    PNULL);
#endif
            length = 	  SCI_STRLEN(str_class1) ;
            MMI_STRNTOWSTR( wstr_buf ,  7,  (uint8*)str_class1 , length,  length );
            GUIEDIT_SetString( MMIGAME_BOXMAN_EDIT1_CTRL_ID,  wstr_buf ,  length) ;
        }
    }
    else
    {
#ifndef MMI_TASK_MANAGER
        MMIPUB_OpenAlertWarningWin(TXT_GMBOXMAN_MAP_INVALID);
#else
        MMIPUB_OpenAlertWinByTextIdEx(SPRD_GAME_BOX_APPLET_ID, 
                PNULL,TXT_GMBOXMAN_MAP_INVALID,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,
                PNULL);
#endif
        length = 	  SCI_STRLEN(str_class1) ;
        MMI_STRNTOWSTR( wstr_buf ,  7,  (uint8*)str_class1 , length,  length );
        GUIEDIT_SetString( MMIGAME_BOXMAN_EDIT1_CTRL_ID,  wstr_buf ,  length) ;
    }
}

/*****************************************************************************/
//  Description : the window for boxman help information
//  Global resource dependence : none
//  Author: Ming.Song
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBoxmanHelpWinMsg(
  MMI_WIN_ID_T win_id,
  MMI_MESSAGE_ID_E msg_id,
  DPARAM param
  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T string = {0};
    GUI_COLOR_T  text_color = MMI_BLACK_COLOR;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
      MMK_SetAtvCtrl(win_id, MMIGAME_STATIC_TEXT_CTRL_ID);
#if defined (TOUCH_PANEL_SUPPORT)
    #if defined (MMI_PDA_SUPPORT)
        MMI_GetLabelTextByLang(TXT_BOXMAN_HELP_PDA, &string);
    #else   
        MMI_GetLabelTextByLang(TXT_BOXMAN_HELP_EXT, &string);
    #endif
#else
      MMI_GetLabelTextByLang(TXT_BOXMAN_HELP_NO_TOUCH, &string);
#endif
      GUITEXT_SetString(MMIGAME_STATIC_TEXT_CTRL_ID , string.wstr_ptr, string.wstr_len, FALSE);
      GUITEXT_SetFont(MMIGAME_STATIC_TEXT_CTRL_ID, PNULL, &text_color);//@NEWMS00173451,NEWMS00172742
      break;
      
    case MSG_FULL_PAINT:
      break;
      
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
//  FUNCTION:     LcdSwitch
//  Description :   
//  Global resource dependence : 
//  Author:ying.xu
//  Note:   
/*****************************************************************************/
LOCAL  void LcdSwitch( MMI_WIN_ID_T win_id)
{
    if(MMITHEME_IsMainScreenLandscape())
    {
#if (defined(MAINLCD_SIZE_176X220) && defined(TOUCH_PANEL_SUPPORT)) || defined(MMI_PDA_SUPPORT)//176x220+触屏显示不下此button cr106572
        GUIBUTTON_SetVisible(MMIGAME_BOXMAN_BUTTON_CTRL_ID,FALSE, FALSE);
#else
        GUIBUTTON_SetVisible(MMIGAME_BOXMAN_BUTTON_CTRL_ID,TRUE, FALSE);
#endif
#ifndef MMI_PDA_SUPPORT	//pda大多数场景不显示softkey
        MMK_DestroyDynaCtrl( MMK_GetCtrlHandleByWin(win_id , MMITHEME_GetSoftkeyCtrlId()));	
#endif
    }
    else
    {              
        GUIBUTTON_SetVisible(MMIGAME_BOXMAN_BUTTON_CTRL_ID,FALSE, FALSE);	 
#ifndef MMI_PDA_SUPPORT	//pda大多数场景不显示softkey	 
        GUISOFTKEY_CreateCtrlDyn(win_id,  MMITHEME_GetSoftkeyCtrlId(), 
            PNULL,  PNULL,  STXT_RETURN);	
#endif
    }

}

/*****************************************************************************/
//  FUNCTION:     HandleReturnButtonCallback
//  Description :   
//  Global resource dependence : 
//  Author:ying.xu
//  Note:   
/*****************************************************************************/
LOCAL  MMI_RESULT_E  HandleReturnButtonCallback(void)
{
    MMK_CloseWin(BOXMAN_NEW_WIN_ID);
    // MMK_CloseWin(BOXMAN_RES_WIN_ID);
    return MMI_RESULT_TRUE ;
}

/*****************************************************************************/
//  FUNCTION:     Boxman_ConvertMsg
//  Description :转横屏的2，4，6，8按键消息  
//  Global resource dependence : 
//  Author:juan.wu
//  Note:   
/*****************************************************************************/
LOCAL  MMI_MESSAGE_ID_E Boxman_ConvertMsg(MMI_MESSAGE_ID_E msg_id)
{
    LCD_ANGLE_E     rotate_angle = LCD_ANGLE_0;

    rotate_angle = GUILCD_GetRotateAngle(GUI_MAIN_LCD_ID);    
    if(LCD_ANGLE_270 == rotate_angle)//横屏
    {
        switch(msg_id)
        {
        case MSG_KEYDOWN_2:
            msg_id = MSG_KEYDOWN_RIGHT;
            break;

        case MSG_KEYDOWN_4:
            msg_id = MSG_KEYDOWN_UP;
            break;

        case MSG_KEYDOWN_6:
            msg_id = MSG_KEYDOWN_DOWN;
            break;

        case MSG_KEYDOWN_8:
            msg_id = MSG_KEYDOWN_LEFT;
            break;

        default:
            break;
        }
    }
    else if(LCD_ANGLE_90 == rotate_angle)
    {
        switch(msg_id)
        {
        case MSG_KEYDOWN_2:
            msg_id = MSG_KEYDOWN_LEFT;
            break;

        case MSG_KEYDOWN_4:
            msg_id = MSG_KEYDOWN_DOWN;
            break;

        case MSG_KEYDOWN_6:
            msg_id = MSG_KEYDOWN_UP;
            break;

        case MSG_KEYDOWN_8:
            msg_id = MSG_KEYDOWN_RIGHT;
            break;

        default:
            break;
        }
    }
    else if(LCD_ANGLE_180 == rotate_angle)
    {
        switch(msg_id)
        {
        case MSG_KEYDOWN_2:
            msg_id = MSG_KEYDOWN_DOWN;
            break;

        case MSG_KEYDOWN_4:
            msg_id = MSG_KEYDOWN_RIGHT;
            break;

        case MSG_KEYDOWN_6:
            msg_id = MSG_KEYDOWN_LEFT;
            break;

        case MSG_KEYDOWN_8:
            msg_id = MSG_KEYDOWN_UP;
            break;

        default:
            break;
        }
    }
    return msg_id;
}

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register box game module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIGMBOX_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_GAME_BOXMAN, &g_mmigamebox_applet_info);
}

/*****************************************************************************/
//  Description : the process message function of the box game
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  GameBoxApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIGAMEBOX_APPLET_T* applet_ptr = (MMIGAMEBOX_APPLET_T*)iapplet_ptr;

    //SCI_TRACE_LOW:"mmi GameBoxApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_BOXMAN_WINTAB_2257_112_2_18_2_21_22_3,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_gamebox_applet_instance = applet_ptr;
        MMK_CreateGameBoxWin((uint32*)MMIGMBOXMAIN_MENU_WIN_TAB,PNULL);
        break;
        
    case MSG_APPLET_STOP: 
        s_gamebox_applet_instance = PNULL;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Get box Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIGAMEBOX_APPLET_T *MMIGMBOX_GetAppletInstance(void)
{
    return s_gamebox_applet_instance;
}

/*****************************************************************************/
//  Description : Create box reference window for MMI_APPLET_STATE_BG_HIDE_WIN
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenGameBoxWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  )
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = SPRD_GAME_BOX_APPLET_ID;//MMIGMBOX_HANDLE(MMIGMBOX_GetAppletInstance());
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;
    
    return MMK_CreateWinTable(&win_table_create);
}

 /*****************************************************************************/
//  Description : Stop game box
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIGMBOX_Stop(BOOLEAN is_minimize)
{
#if 0
    MMIGAMEBOX_APPLET_T    *applet_ptr = MMIGMBOX_GetAppletInstance();

    if (PNULL != applet_ptr)
    {
        MMI_HANDLE_T applet_handle = MMIGMBOX_HANDLE(applet_ptr);

        if (is_minimize)
        {
            //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
            MMK_HideWinByApplet(applet_handle);
        }
        else
        {
            MMK_CloseApplet(applet_handle);
        }
    }
#endif
    if (is_minimize)
    {
        //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
        MMK_HideWinByApplet(SPRD_GAME_BOX_APPLET_ID);
    }
    else
    {
        MMK_CloseApplet(SPRD_GAME_BOX_APPLET_ID);
    }
}

#endif

#endif


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:54 2012*/ //IGNORE9527


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
