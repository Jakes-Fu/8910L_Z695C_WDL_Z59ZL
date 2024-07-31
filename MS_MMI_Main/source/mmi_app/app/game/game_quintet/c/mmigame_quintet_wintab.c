/****************************************************************************
** File Name:      mmigame_quintet_wintab.c                              
** Author:                                                                  
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
#define _MMIGAME_QUINTET_WINTAB_C_

#include "mmi_app_game_trc.h"
#ifdef GAME_QUINTET_SUPPORT
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "std_header.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmigame_quintet_menutable.h"
#include "mmigame_menutable.h"
#include "mmigame_quintet_id.h"  
#include "tb_dal.h"
//#include "mmiacc_winmain.h"
//michael wang for5
#include "mmipub.h"//#include "mmi_pubwin.h"
#include "mmigame_quintet_nv.h"
#include "mmialarm_export.h"
//michael wang for5
#include "mmigame_internal.h"//#include "mmigame.h"
#include "mmi_textfun.h"
#include "mmiphone_export.h"
#include "mmigame_quintet_internal.h"
#include "mmigame_quintet_text.h"
#include "mmigame_id.h"
/**---------------------------------------------------------------------------*
 **                       LOCAL DATA DECLARATION
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA  DECLARE                           */
/*---------------------------------------------------------------------------*/

//LOCAL int8 	           s_board[BOARD_LINE_NUMBER][BOARD_LINE_NUMBER];	
//LOCAL int16		    s_type;
//LOCAL uint16             s_by_color;
//LOCAL int16               s_stone_num = 0;
//LOCAL BOOLEAN        s_is_over = FALSE;
LOCAL GUI_POINT_T  s_cur_point;
//LOCAL BOOLEAN           s_is_lastview = FALSE;
//LOCAL MMIACC_RTC_TIME_T s_time;
//LOCAL MMIACC_MSGBOX_T s_msgbox = {0};
//LOCAL uint32            s_set_hand_list[2];


typedef struct HORIZANTAL_LINE_TAG{
    int16 x1;
    int16 x2;
    int16 y;
}HORIZANTAL_LINE_T;

LOCAL HORIZANTAL_LINE_T s_circle[7] =
        {
                {-1, 1, -3}, {-2, 2, -2}, {-3, 3, -1}, {-3, 3, 0}, {-3, 3, 1}, {-2, 2, 2}, {-1, 1, 3}
        };


LOCAL uint32            s_set_hand_list[2] ={
    TXT_GMQUT_FIRST_HAND,
    TXT_GMQUT_LAST_HAND
};



     
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/            

//*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : HandleQuintetNewWinMsg
//       
//	Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQuintetNewWinMsg(
                    MMI_WIN_ID_T win_id, 	
                    MMI_MESSAGE_ID_E msg_id, 
                    DPARAM param);

/*****************************************************************************/
// 	Description : HandleQuintetResWinMsg
//       
//	Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQuintetResWinMsg(
                    MMI_WIN_ID_T win_id, 	
                    MMI_MESSAGE_ID_E msg_id, 
                    DPARAM param);


/*****************************************************************************/
// 	Description : HandleQuintetSetHandWinMsg
//        set class 1 to 9 of Russia Square
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleQuintetSetHandWinMsg(
    MMI_WIN_ID_T win_id, 			
    MMI_MESSAGE_ID_E msg_id, 		
    		DPARAM param);

/*****************************************************************************/
// 	Description : display last view
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleQuintetLastViewWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param);

/*****************************************************************************/
// 	Description : HandleQuintetHelpWinMsg
//       
//	Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQuintetHelpWinMsg(
                    MMI_WIN_ID_T win_id, 	
                    MMI_MESSAGE_ID_E msg_id, 
                    DPARAM param);

/*****************************************************************************/
// 	Description : HandleQuintetMainMenuWindow
//       
//	Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleQuintetMainMenuWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param);

/*****************************************************************************/
// 	Description :MMIGMQUT_AppendListBoxItem
//        set class 1 to 9 of Russia Square
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/

PUBLIC void  MMIGMQUT_AppendListBoxItem (
        MMI_CTRL_ID_T ctrl_id,          
        MMI_TEXT_ID_T *plist,                              
        uint16 count                                  
        );
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
WINDOW_TABLE( MMIGAM1_MAIN_MENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleQuintetMainMenuWindow ),    
    WIN_ID( MMIGMQUT_MAIN_MENU_WIN_ID ),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_QUINTET, MMIGMQUT_MAIN_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( QUINTET_NEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleQuintetNewWinMsg ),    
    WIN_ID( MMIGMQUT_NEW_WIN_ID),
    END_WIN
}; 

WINDOW_TABLE( QUINTET_RES_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleQuintetResWinMsg ),    
    WIN_ID( MMIGMQUT_RES_WIN_ID),
    END_WIN
}; 

// QUINTET_SETHAND_WIN_TAB
// set class of Russia Square
WINDOW_TABLE( QUINTET_SETHAND_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleQuintetSetHandWinMsg ),    
    WIN_ID( MMIGMQUT_SETHAND_WIN_ID),
    WIN_TITLE(TXT_GMQUT_SETTING),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIGMQUT_SETHAND_CTRL_ID),
     END_WIN
}; 


WINDOW_TABLE( QUINTET_INFO_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleQuintetHelpWinMsg ),    
    WIN_ID( MMIGMQUT_INFO_WIN_ID),
    WIN_TITLE(TXT_GMQUT_INTRODUCE),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIGAME_STATIC_TEXT_CTRL_ID),
    END_WIN
}; 

WINDOW_TABLE( QUINTET_LAST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleQuintetLastViewWinMsg ),    
    WIN_ID( MMIGMQUT_LAST_WIN_ID),
    END_WIN
}; 




/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                       function bodies 
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_OpenGameWin
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIGMQUT_OpenGameWin(void)
{
    return (MMK_CreateWin((uint32*)MMIGAM1_MAIN_MENU_WIN_TAB,PNULL));
}

#ifdef MAINLCD_DEV_SIZE_320X240
/*****************************************************************************/
// 	Description : To convert the spec the key code
//	Global resource dependence : 
//  Author:michael wang
//	Note:   
/*****************************************************************************/

LOCAL VOID MMIGMQUT_ConvertSpecKeyCode(MMI_MESSAGE_ID_E *msg_id_ptr)
{
	BOOLEAN is_landscape = FALSE;

	if(msg_id_ptr == NULL)
	{
		return;
	}
	
	is_landscape = MMITHEME_IsMainScreenLandscape();

	if(!is_landscape)
	{
		switch(*msg_id_ptr)
        {
        case MSG_KEYDOWN_LEFT:
            *msg_id_ptr = MSG_KEYDOWN_RIGHT;
            break;
            
        case MSG_KEYDOWN_RIGHT:
            *msg_id_ptr = MSG_KEYDOWN_LEFT;
            break;
            
        case MSG_KEYDOWN_UP:
            *msg_id_ptr = MSG_KEYDOWN_DOWN;
            break;
            
        case MSG_KEYDOWN_DOWN:
            *msg_id_ptr = MSG_KEYDOWN_UP;
            break;
            
        default:
            break;
		}
	}
		
}

#endif
/*****************************************************************************/
// 	Description : the process message function of the game menu
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleQuintetNewWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

        
    switch(msg_id)
    {
	 case MSG_OPEN_WINDOW:
            MMIGMQUT_StartQuintet();
            break;
            
        case MSG_FULL_PAINT:
            MMIGMQUT_ResumeQuintet();
            break;
	//michael wang for5
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_UP:
#else		
	//michael wang
        case MSG_KEYDOWN_OK:
        case MSG_KEYDOWN_LEFT:
        case MSG_KEYDOWN_RIGHT:
        case MSG_KEYDOWN_UP:
        case MSG_KEYDOWN_DOWN:          
#ifdef MAINLCD_DEV_SIZE_320X240
            MMIGMQUT_ConvertSpecKeyCode(&msg_id);
#endif
#endif
            MMIGMQUT_ProcessQuintetKeyWinMsg(win_id, msg_id);            
            break;
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_OK:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}


/*****************************************************************************/
// 	Description : the process message function of the game menu
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleQuintetResWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

        
    switch(msg_id)
    {
	 case MSG_OPEN_WINDOW:
          //  if (s_is_continue)
          if(MMIGMQUT_Get_Renstatus())
            {
               s_cur_point.x = 7;
                s_cur_point.y = 7;
                MMIGMQUT_ResumeQuintet();
            }else 
            {
                    MMIGMQUT_StartQuintet();
            }
            break;
            
        case MSG_FULL_PAINT:
	     s_cur_point.x = 7;
            s_cur_point.y = 7;
            MMIGMQUT_ResumeQuintet();
            break;

        case MSG_KEYDOWN_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMIGMQUT_ProcessQuintetKeyWinMsg(win_id, msg_id);
            break;
    }
    
    return recode;
}




/*****************************************************************************/
// 	Description : HandleQuintetSetHandWinMsg
//        set class 1 to 9 of Russia Square
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleQuintetSetHandWinMsg(
    MMI_WIN_ID_T win_id, 			
    MMI_MESSAGE_ID_E msg_id, 		
    		DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16  sel = 0;
    uint8 level[6] = {0};
    uint16 nv_return = 0;
    
        
    switch(msg_id)
    {
	 case MSG_OPEN_WINDOW:      //create radio list box
            MMINV_READ(MMINV_GMQUT_LEVER_SET, (uint16*)level, nv_return);
            if (level[1] >1 ) level[1] = 0;     // default value        
            MMIGMQUT_AppendListBoxItem(MMIGMQUT_SETHAND_CTRL_ID, &s_set_hand_list[0], 2);
	     GUILIST_SetCurItemIndex( MMIGMQUT_SETHAND_CTRL_ID, level[1]);
	     GUILIST_SetSelectedItem(MMIGMQUT_SETHAND_CTRL_ID, level[1], TRUE);
	     MMK_SetAtvCtrl( win_id,  MMIGMQUT_SETHAND_CTRL_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    	case MSG_CTL_OK:		
            sel = GUILIST_GetCurItemIndex(MMIGMQUT_SETHAND_CTRL_ID);
            MMIGMQUT_SetQuintetHand(sel);
          //  MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);	       

	        MMK_CloseWin(win_id);
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
// 	Description : display last view
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleQuintetLastViewWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

        
    switch(msg_id)
    {
	 case MSG_OPEN_WINDOW:
            MMIGMQUT_DisplayLastView();
            break;
            
        case MSG_FULL_PAINT:
            MMIGMQUT_DisplayLastView();
            break;
            
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
// 	Description : the process message function of the game menu
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleQuintetHelpWinMsg( 
								MMI_WIN_ID_T win_id, 
								MMI_MESSAGE_ID_E msg_id, 
								DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T string ;

    switch(msg_id)  
    {
    case MSG_OPEN_WINDOW:
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_GMQUT_HELP, &string);
        GUITEXT_SetString(MMIGAME_STATIC_TEXT_CTRL_ID , string.wstr_ptr, string.wstr_len, FALSE);  
        MMK_SetAtvCtrl(win_id, MMIGAME_STATIC_TEXT_CTRL_ID);
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
// 	Description : ÓÎÏ·Ö÷²Ëµ¥
//	Global resource dependence : 
//  Author:ycd 01
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleQuintetMainMenuWindow(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    uint32          group_id = 0; 
    uint32          menu_id  = 0;
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, MMIGMQUT_MAIN_MENU_CTRL_ID);
        break;
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        GUIMENU_GetId(MMIGMQUT_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
		switch(menu_id) 
		{
		 
		case ID_GMQUT_NEW_ITEM_ID:
			MMK_CreateWin((uint32 *) QUINTET_NEW_WIN_TAB, PNULL);
			break;

		case ID_GMQUT_RES_ITEM_ID:
		              MMK_CreateWin((uint32 *) QUINTET_RES_WIN_TAB, PNULL);
					
			break;
             case ID_GMQUT_SET_ITEM_ID:
		             MMK_CreateWin((uint32 *) QUINTET_SETHAND_WIN_TAB, PNULL);
					
			break;

             case ID_GMQUT_INFO_ITEM_ID:
		            MMK_CreateWin((uint32 *) QUINTET_INFO_WIN_TAB, PNULL);
				
			break;
            case ID_GMQUT_LAST_ITEM_ID:
		            MMK_CreateWin((uint32 *) QUINTET_LAST_WIN_TAB, PNULL);
				
			break;
			
		default:
			
			break;
		}
        
   
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
// 	Description : MMIGMQUT_AppendListBoxItem
//          append list item to a list box control
//   Author:        Annie.an
//	Note:   
/*****************************************************************************/
PUBLIC void  MMIGMQUT_AppendListBoxItem (
        MMI_CTRL_ID_T ctrl_id,           // list box control ID
        MMI_TEXT_ID_T *plist,                                  // pointer of list items
        uint16 count                                    // no. of list items
        )
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    int i = 0;
    MMI_STRING_T  temp_str = {0};
     
    //SCI_ASSERT(plist!=NULL && count !=0);
    if((PNULL == plist) || (count == 0))
    {
        //SCI_TRACE_LOW:"MMIGMQUT_AppendListBoxItem param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGAME_QUINTET_WINTAB_607_112_2_18_2_21_42_25,(uint8*)"");
        return;
    }
    
    GUILIST_SetMaxItem(ctrl_id, count, FALSE );          

      SCI_MEMSET(      
          &item_t,      
          0,      
          sizeof (GUILIST_ITEM_T)      
          );      

    for(i=0; i<count; i++)
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
        item_t.item_data_ptr = &item_data;
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = plist[i];
#ifndef TOUCHPANEL_TYPE		
        item_data.softkey_id[0] = STXT_SOFTKEY_OK_MK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
#endif
         GUILIST_AppendItem(ctrl_id, &item_t);      
    }
}

#endif


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
