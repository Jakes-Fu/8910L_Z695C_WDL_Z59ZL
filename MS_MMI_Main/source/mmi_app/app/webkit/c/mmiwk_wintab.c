#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))

#include "std_header.h"
#include "window_parse.h"
#include "in_message.h"
#include "guifont.h"
#include "guistring.h"
#include "ctrlform_export.h"
#include "guilcd.h"
#include "guilabel.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "mmi_common.h"
#include "mmibrowser_export.h"
#include "mmiwk_id.h"
#include "mmiwk_text.h"
#include "wkapi.h"
#include "mmiwk_export.h"
#include "mmi_appmsg.h"
#include "http_api.h"
#include "mmimsa_id.h"
#include "mmimsa_export.h"
#include "mmiwk_menutable.h"
#include "mmi_position.h"
#include "mmiwk_export.h"
#include "mmiwk.h"
#include "mmimsa_nv.h"

#define MMI_WK_PROGRESS_BG_BAR_HEIGHT      10
#define MMI_WK_PROGRESS_BAR_HEIGHT      6
//offset should be (10-6)/2
#define MMI_WK_PROGRESS_TOP_OFFSET      2   
#define MMI_WK_PROGRESS_BAR_LEFT            20
#define MMI_WK_PROGRESS_BAR_RIGHT           220

#define MMI_WK_CLIENT_TOP               MMI_STATUSBAR_HEIGHT
#define MMI_WK_CLIENT_BOTTOM        (320-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)
#define MMI_WK_CLIENT_WIDTH 240

#define MMI_WK_LOADING_MSG_TOP               70
#define MMI_WK_LOADING_MSG_BOTTOM        150
#define MMI_WK_LOADING_MSG_WIDTH 220


#define MMI_WK_EDITOR_MAX_NUM 100
typedef struct
{
BOOLEAN is_anim_on;
BOOLEAN is_loading;
BOOLEAN is_stored_sms;
MMI_DIALOG_EDITOR_TYPE_E  input_type;
wchar input_text[MMI_WK_EDITOR_MAX_NUM];
uint32 input_text_len;
MMI_STRING_T prompt_str;
char*  enter_url;
uint16 cur_progress;
uint8   sms_display_timer;
uint8   progress_timer;
BOOLEAN  is_sign_in;
MMI_TEXT_ID_T query_text_id;
GUI_COLOR_T* bg_guiBuf;
BOOLEAN is_running;
BOOLEAN is_need_repaintBg;
}
MMIWK_CONTROL_CONTEXT_T;

MMIWK_CONTROL_CONTEXT_T g_mmiwk_context ={0};



/*---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E MMIWK_HandleWinMsg(
    MMI_WIN_ID_T  win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM  param);

LOCAL MMI_RESULT_E MMIWK_HandleEditorMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);
LOCAL MMI_RESULT_E MMIWK_HandlePopupMenuWinMsg(
    MMI_WIN_ID_T  win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM  param);

LOCAL MMI_RESULT_E MMIWK_HandleQueryMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param);

LOCAL BOOLEAN MMIWK_isRunningWebkit(void);
LOCAL void WK_DisplayProgressBarbyTimer(void);

LOCAL BOOLEAN MMIWK_InitWin(int32 url_len);
LOCAL void MMIWK_ExitWin(void);

LOCAL void MMIWK_SaveBg(void);
LOCAL void MMIWK_RestoreBg(void);
LOCAL void MMIWK_BgExit(void);
LOCAL void MMIWK_DeleteCookie(void);

LOCAL BOOLEAN MMIWK_IsNeedAddLoadingMsg(void);
LOCAL void WK_DisplayProgressMsg(void);

/*---------------------------------------------------------------------------*/
//webkit window
WINDOW_TABLE( MMIWK_WIN_TAB ) =
{
    WIN_ID(MMIWK_WIN_ID),
    WIN_FUNC((uint32)MMIWK_HandleWinMsg),
    WIN_TITLE(STR_MSA_HDR_ACCOUNT),
    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

    //CREATE_ANIM_CTRL(MMIWK_ANIMATE_SEARCHING_CTRL_ID, MMIWK_WIN_ID),
    END_WIN
};

//text editbox
WINDOW_TABLE( MMIWK_TEXT_EDITBOX_WIN_TAB ) =
{
    WIN_ID(MMIWK_TEXT_EDITBOX_WIN_ID),
    WIN_TITLE(TXT_MMIWK_STXT_EDITBOX),
    WIN_FUNC((uint32)MMIWK_HandleEditorMsg),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(100, MMIWK_TEXT_EDITBOX_CTRL_ID),
    END_WIN
};

//MMIWK_POPMENU_WIN_ID
//text editbox
WINDOW_TABLE( MMIWK_POPMENU_WIN_TAB ) =
{
    WIN_ID(MMIWK_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_HIDE_STATUS,
    CREATE_POPMENU_CTRL(MENU_MMIWK_POPUP,MMIWK_POPUP_MENU_CTRL_ID),
    WIN_FUNC((uint32)MMIWK_HandlePopupMenuWinMsg),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};



/*---------------------------------------------------------------------------*/

LOCAL MMI_MESSAGE_ID_E KeyTransform(MMI_MESSAGE_ID_E key)
{
    switch( key )
    {
    case MSG_KEYDOWN_UP:
    case MSG_KEYUP_UP:
        return VK_UP;
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYUP_DOWN:
        return VK_DOWN;
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYUP_LEFT:
        return VK_LEFT;
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYUP_RIGHT:
        return VK_RIGHT;
    case MSG_KEYDOWN_0:
    case MSG_KEYUP_0:
        return VK_0;
    case MSG_KEYDOWN_1:
    case MSG_KEYUP_1:
        return VK_1;
    case MSG_KEYDOWN_2:
    case MSG_KEYUP_2:
        return VK_2;
    case MSG_KEYDOWN_3:
    case MSG_KEYUP_3:
        return VK_3;
    case MSG_KEYDOWN_4:
    case MSG_KEYUP_4:
        return VK_4;
    case MSG_KEYDOWN_5:
    case MSG_KEYUP_5:
        return VK_5;
    case MSG_KEYDOWN_6:
    case MSG_KEYUP_6:
        return VK_6;
    case MSG_KEYDOWN_7:
    case MSG_KEYUP_7:
        return VK_7;
    case MSG_KEYDOWN_8:
    case MSG_KEYUP_8:
        return VK_8;
    case MSG_KEYDOWN_9:
        return VK_TAB; //VK_9;
    case MSG_KEYUP_9:
        return VK_9;
    case MSG_KEYDOWN_CANCEL:
        return VK_BACK;
	case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_OK:
	case MSG_KEYUP_WEB:
	case MSG_KEYUP_OK:
        return VK_RETURN;
    case MSG_TP_PRESS_DOWN:
        return VK_MOUSE_DOWN;
    case MSG_TP_PRESS_UP:
        return VK_MOUSE_UP;
    case MSG_TP_PRESS_DOUBLE:
        return VK_MOUSE_DOUBLE;
    case MSG_TP_PRESS_MOVE:
        return VK_MOUSE_MOVE;
    default:
        return key;
    }
}

LOCAL void WK_DisplayProgressMsg(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T show_rect={20,MMI_WK_LOADING_MSG_TOP,MMI_WK_LOADING_MSG_WIDTH,MMI_WK_LOADING_MSG_BOTTOM};
    GUI_COLOR_T* guiBuf = GUIBLOCK_GetBuf(&lcd_dev_info);

    MMI_STRING_T      text_str = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T     state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK;  
    
    if (!MMK_IsFocusWin(MMIWK_WIN_ID)) 
        return;
    
    if(g_mmiwk_context.is_loading == FALSE)
    {
        return;
    }
    if (g_mmiwk_context.cur_progress < 20)
    {
        return;
    }    

    MMI_GetLabelTextByLang(STR_MSA_BROWSER_LOADING, &text_str);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_16;
    text_style.line_space = 4;

    GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&show_rect,       //the fixed display area
            (const GUI_RECT_T      *)&show_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    return;
}



LOCAL void WK_DisplayProgressBar(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T bg_rect ={0,0,240,0};
    GUI_RECT_T percent_rect ={MMI_WK_PROGRESS_BAR_LEFT,0,MMI_WK_PROGRESS_BAR_RIGHT,0};

#if 0    
    MMI_TEXT_ID_T  label = TXT_NULL;
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T     state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;  
    GUI_RECT_T		rect = {0};
    MMI_STRING_T      text_str = {0};
    wchar temp_wstr[10] = {0};
    uint8 temp_str[10] = {0};
#endif
    if (!MMK_IsFocusWin(MMIWK_WIN_ID)) return;
    if(g_mmiwk_context.is_loading == FALSE)
    {
        //GUI_COLOR_T bg_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
        //LCD_FillRect(&lcd_dev_info,bg_rect,bg_color); //MMI_DARK_YELLOW_COLOR
        return;
    }
    if (g_mmiwk_context.cur_progress == 0)
    {
        return;
    }    

    if(g_mmiwk_context.is_need_repaintBg == TRUE)
        {
        MMK_PostMsg(MMIWK_WIN_ID, MSG_FULL_PAINT, 0,0);
        MMIWK_RestoreBg();
        g_mmiwk_context.is_need_repaintBg = FALSE;
    }
    
    if(MMIWK_IsNeedAddLoadingMsg())
        {
            WK_DisplayProgressMsg();
    }
    
    //Draw Bar Background
    bg_rect.top =MMI_WK_CLIENT_BOTTOM- MMI_WK_PROGRESS_BG_BAR_HEIGHT;
    bg_rect.bottom=MMI_WK_CLIENT_BOTTOM;
    LCD_FillRect(&lcd_dev_info,bg_rect,0x0090); //MMI_DARK_YELLOW_COLOR

    //Draw Progress BG
    percent_rect.top =MMI_WK_CLIENT_BOTTOM- MMI_WK_PROGRESS_BG_BAR_HEIGHT+MMI_WK_PROGRESS_TOP_OFFSET;
    percent_rect.bottom=MMI_WK_CLIENT_BOTTOM-MMI_WK_PROGRESS_TOP_OFFSET;    
    LCD_FillRect(&lcd_dev_info,percent_rect,0x111b); //MMI_RED_COLOR

    //Draw Progress
    percent_rect.right = percent_rect.left + (percent_rect.right-percent_rect.left)*g_mmiwk_context.cur_progress/100;
    LCD_FillRect(&lcd_dev_info,percent_rect,0x03ba); 


#if 0       //Needn't display percent
    //Get percent display rect
    if (bg_rect.right >200)
    {
        percent_rect.left = bg_rect.right-40;
        percent_rect.right = bg_rect.right;
        text_style.font_color = 0x0;    //BLACK
    }
    else
    {
        percent_rect.left = bg_rect.right;
        percent_rect.right = bg_rect.right+40;
        text_style.font_color = 0xffff;  //White
    }
    //Display Percent
    SCI_MEMSET(temp_wstr, 0x00, sizeof(temp_wstr));
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = 0 ;

    SCI_MEMSET(temp_str, 0x00, sizeof(temp_str));
    if (g_mmiwk_context.cur_progress ==0)   //don't display 0%
        g_mmiwk_context.cur_progress  = 1;   
    sprintf((char *)temp_str, "%d", g_mmiwk_context.cur_progress);

    MMI_STRNTOWSTR(temp_wstr, 10, temp_str, 10, 10);
    text_str.wstr_ptr = temp_wstr;
    text_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr) ;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_14;
    text_style.line_space = 4;

    GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&percent_rect,       //the fixed display area
            (const GUI_RECT_T      *)&percent_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    return;
#endif    
}


LOCAL void WK_DisplayProgressBarbyTimer(void)
{
    if (g_mmiwk_context.is_loading== FALSE)
    {
        MMK_StopTimer(g_mmiwk_context.progress_timer);
        g_mmiwk_context.progress_timer = 0;
        return;
    }    
    if (g_mmiwk_context.cur_progress<95)
        {
        g_mmiwk_context.cur_progress++;
        WK_DisplayProgressBar();
        }
    return;
}



LOCAL void  WK_InitWKWinBG()
{	
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
       GUI_RECT_T   bg_rect={0};
       
//       GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&bg_rect.right,&bg_rect.bottom);
//
//	LCD_FillRect(&lcd_dev_info,bg_rect,0xffff); //MMI_WHITE_COLOR

        g_mmiwk_context.is_need_repaintBg = FALSE;

}

LOCAL void  MMIAPIWK_HandleDisplayTimer(uint8 timer_id, uint32 param)
{
    if ((timer_id ==g_mmiwk_context.sms_display_timer) && g_mmiwk_context.is_stored_sms== TRUE)
    {
        MMK_PostMsg(MMIWK_WIN_ID, MSG_MMI_WEBKIT_SHOW_SMS, 0,0);
    }     
}


PUBLIC void MMIWK_UpdateProgressInd( WK_PROGRESS_TYPE_E type,double progress)
{
    int64 progress_int =(int64)(progress*100);	//progress in 0.1~0.9

    switch (type)
    {
    case WK_PROGRESS_START:	//Begin
        if (g_mmiwk_context.is_loading == FALSE)	//
        {
            g_mmiwk_context.is_loading = TRUE;
            g_mmiwk_context.cur_progress = 0;
            if(g_mmiwk_context.progress_timer != 0)
            {
                MMK_StopTimer(g_mmiwk_context.progress_timer);
            }
            g_mmiwk_context.progress_timer= MMK_CreateWinTimer(MMIWK_WIN_ID, 1000, TRUE);

            WK_DisplayProgressBar();
        }        
        break;
    case WK_PROGRESS_CHANGED: //Update or begin
        if (g_mmiwk_context.is_loading == FALSE)	//
        {
            g_mmiwk_context.is_loading = TRUE;
        }
        if (g_mmiwk_context.cur_progress<progress_int)
            g_mmiwk_context.cur_progress = progress_int; 
        WK_DisplayProgressBar();
        break;
    case WK_PROGRESS_FINISHED: //Stop
        if (g_mmiwk_context.is_loading == TRUE)	//
        {
            g_mmiwk_context.is_loading = FALSE;
            g_mmiwk_context.cur_progress = 0;        	
            WK_DisplayProgressBar();			
        }
        if(g_mmiwk_context.progress_timer != 0)
        {
            MMK_StopTimer(g_mmiwk_context.progress_timer);
            g_mmiwk_context.progress_timer = 0;
        }
        //check there is stored SMS
        if (g_mmiwk_context.is_stored_sms== TRUE)
        {
            if (0 < g_mmiwk_context.sms_display_timer)
            {
                MMK_StopTimer(g_mmiwk_context.sms_display_timer);
            }        
            g_mmiwk_context.sms_display_timer = MMK_CreateTimerCallback(500,MMIAPIWK_HandleDisplayTimer,NULL, FALSE);
            
        }
        break;
    default:
        break;
    }
    return;
}

/*======================================
* when 
1   backgroud is White(0xFFFF)
2   cur_progress >20

======================================*/
LOCAL BOOLEAN MMIWK_IsNeedAddLoadingMsg(void)
{
    //check message rect
    GUI_RECT_T show_rect={0,MMI_WK_LOADING_MSG_TOP,240,MMI_WK_LOADING_MSG_BOTTOM};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_COLOR_T* guiBuf = GUIBLOCK_GetBuf(&lcd_dev_info);
    uint16  i,j;

    guiBuf = guiBuf+MMI_WK_LOADING_MSG_TOP*MMI_WK_LOADING_MSG_WIDTH; 
    //check current
    for(i=0;i<MMI_WK_LOADING_MSG_BOTTOM-MMI_WK_LOADING_MSG_TOP;i++)
        {
        for(j=0;j<MMI_WK_LOADING_MSG_WIDTH;j++)
            {
            if(*guiBuf !=0xFFFF)
                return FALSE;
            guiBuf++;
        }
    }
    if(g_mmiwk_context.cur_progress>20)
        return TRUE;
    return FALSE;
}



void MMIWK_HandPaintInd(GUI_RECT_T *invalidateRect, GUIIMG_BITMAP_T *img_ptr)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    GUI_POINT_T dis_point = {0};
    GUI_COLOR_T* guiBuf = GUIBLOCK_GetBuf(&lcd_dev_info);
    GUI_RECT_T show_rect={0};

   GUI_RECT_T test_rect={0,0,240,258};

	if (invalidateRect ==PNULL )
	{
    	SCI_TRACE_LOW("mmiwk: MMIWK_HandPaintInd rect_ptr = null");
	    return;
	}
	if(!MMK_IsFocusWin(MMIWK_WIN_ID))
	{
    	SCI_TRACE_LOW("mmiwk: MMIWK_HandPaintInd not focused!! ");
	    return;
	}

  SCI_TRACE_LOW("webkit-paint: -------- GUILCD_InvalidateRect{%d, %d, %d, %d} -------",
         invalidateRect->left, invalidateRect->top,
         (invalidateRect->right - invalidateRect->left),
         (invalidateRect->bottom - invalidateRect->top));

   invalidateRect =  &test_rect;


    dis_point.x = invalidateRect->left;
    dis_point.y = invalidateRect->top+MMI_WK_CLIENT_TOP;
    //Status Bar should left.

    show_rect.left = invalidateRect->left;
    show_rect.right = invalidateRect->right;
    show_rect.top = invalidateRect->top+MMI_WK_CLIENT_TOP;
    show_rect.bottom = invalidateRect->bottom+MMI_WK_CLIENT_TOP;
    if (show_rect.bottom >MMI_WK_CLIENT_BOTTOM) 
    {
        show_rect.bottom = MMI_WK_CLIENT_BOTTOM;
    }
    if (g_mmiwk_context.is_loading == TRUE)
    {
        if (show_rect.bottom >MMI_WK_CLIENT_BOTTOM-MMI_WK_PROGRESS_BG_BAR_HEIGHT) 
        {
            show_rect.bottom = MMI_WK_CLIENT_BOTTOM-MMI_WK_PROGRESS_BG_BAR_HEIGHT;
        }
    }


     SCI_TRACE_LOW("webkit-paint: --");    
    GUIIMG_DisplayBmp(FALSE,
        &show_rect,
        &dis_point,
        img_ptr,
        &lcd_dev_info);

   GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, show_rect, 0);

   g_mmiwk_context.is_need_repaintBg = FALSE; //Updated
   
   WK_DisplayProgressBar();
}



LOCAL MMI_RESULT_E MMIWK_HandleQueryMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param)
{
    MMI_RESULT_E  result  = MMI_RESULT_TRUE;

    switch(msg_id)
    {

    case MSG_LOSE_FOCUS:
        //MMK_CloseWin(win_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_WEB:
            SCI_TRACE_LOW("mmiwk: MMIWK_HandleQueryMsg MSG_APP_WEB EXIT !!");
            MMK_CloseWin(win_id);
            MMIAPIMSA_Abort();
            MMIWK_Exit(1);
            break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;            
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}

LOCAL MMI_RESULT_E MMIWK_HandleWinMsg(
    MMI_WIN_ID_T  win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM  param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T sms_win_id= MMIWK_TEXT_SMS_WIN_ID;
    MMI_WIN_ID_T query_win_id= MMIWK_QUERY_WIN_ID;

    //SCI_TRACE_LOW("webkit: func:%s", __FUNCTION__);
    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_SELECT_MK, MIDDLE_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);  
        WK_InitWKWinBG();
        SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg type MSG_OPEN_WINDOW");
        WK_SetViewFocuse(WK_FOCUSE_GET);
        break;
    case MSG_TIMER:
        if (g_mmiwk_context.progress_timer  == *((uint8 *)param))
        {
                SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg type MSG_TIMER");
                WK_DisplayProgressBarbyTimer();
        }
        break;
    case MSG_MMI_WEBKIT_ENTER_URL:
        SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg type MSG_MMI_WEBKIT_ENTER_URL");
        WK_LoadUrlRequest(g_mmiwk_context.enter_url);
	 break;
     case MSG_MMI_WEBKIT_SHOW_SMS:
        SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg type MSG_MMI_WEBKIT_SHOW_SMS");
        MMIPUB_OpenAlertWinByTextPtr(PNULL,&g_mmiwk_context.prompt_str,PNULL,IMAGE_PUBWIN_WARNING,&sms_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        g_mmiwk_context.is_stored_sms= FALSE;        
        break;
     case MSG_FULL_PAINT:
	if(!MMK_IsFocusWin(MMIWK_WIN_ID))   //used for background update
    	    {
        	MMIWK_RestoreBg();
	    return result;
	    }        
        SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg type MSG_FULL_PAINT");
        break;
#if 0
    case MSG_KEYLONG_0:
        WK_LoadUrlRequest( login_url);
        break;
    case MSG_KEYLONG_1:
        WK_LoadUrlRequest( signup_url);
        break;
    case MSG_KEYLONG_2:
        WK_LoadUrlRequest( "file:///e:\\webkit_res\\test\\test.html");
        break;
    case MSG_KEYLONG_3:
//     WK_CloseHistoy();
        break;
    case MSG_KEYLONG_4:
        WK_NavigationRequest( WK_NAVIGATION_GO_BACK);
        break;
    case MSG_KEYLONG_5:
        WK_NavigationRequest( WK_NAVIGATION_GO_FORWARD);
        break;
    case MSG_KEYLONG_6:
        WK_EditRequest( WK_EDIT_SELECT_ALL);
        break;
    case MSG_KEYLONG_7:
        WK_EditRequest( WK_EDIT_CUT_SELECTION);
        break;
    case MSG_KEYLONG_8:
        break;
#endif        
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_UP:   //key event
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_WEB:
       // if (g_mmiwk_context.is_loading == FALSE)
        {
            SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg KeyTransform WK_KEY_EVENT_DOWN %d",msg_id);
            WK_KeyEventRequest( KeyTransform(msg_id), WK_KEY_EVENT_DOWN);
        }
        break;
    case MSG_KEYUP_0:
    case MSG_KEYUP_1:
    case MSG_KEYUP_2:
    case MSG_KEYUP_3:
    case MSG_KEYUP_4:
    case MSG_KEYUP_5:
    case MSG_KEYUP_6:
    case MSG_KEYUP_7:
    case MSG_KEYUP_8:
    case MSG_KEYUP_9:
    case MSG_KEYUP_UP:
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_LEFT:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_WEB:
       // if (g_mmiwk_context.is_loading == FALSE)
        {
            SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg KeyTransform WK_KEY_EVENT_UP %d",msg_id);
            WK_KeyEventRequest( KeyTransform(msg_id), WK_KEY_EVENT_UP);
        }    
        break;

    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_DOUBLE:
    case MSG_TP_PRESS_MOVE:
    {
        GUI_POINT_T tp_point;
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);
        WK_MouseEventRequest( KeyTransform(msg_id), tp_point);
    }
    break;
    case MSG_GET_FOCUS:
        WK_SetViewFocuse( WK_FOCUSE_GET);
        g_mmiwk_context.is_need_repaintBg = TRUE;
        break;
    case MSG_LOSE_FOCUS:
        MMIWK_SaveBg();
        WK_SetViewFocuse( WK_FOCUSE_KILL);
        break;
    case MSG_APP_CANCEL:    //stop?
        //if (g_mmiwk_context.is_loading == TRUE)
        {
            //g_mmiwk_context.is_loading = FALSE;
            SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg WK_NAVIGATION_GO_BACK");

            WK_NavigationRequest(WK_NAVIGATION_GO_BACK);

        }
        break;
    case MSG_APP_OK:    //stop?
        SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg type MSG_APP_MENU");
        MMK_CreateWin(MMIWK_POPMENU_WIN_TAB,PNULL);
        break;

    case MSG_KEYUP_RED:
        MMIPUB_OpenConfirmationDialogByTextId(PNULL, g_mmiwk_context.query_text_id,TXT_NULL, IMAGE_PUBWIN_QUERY, &query_win_id, 0,
            MMIPUB_SOFTKEY_OKCANCEL,MMIWK_HandleQueryMsg);
        break;
    case MSG_CLOSE_WINDOW:
        if(g_mmiwk_context.is_running)
            {
                MMIWK_Exit(1);
            }
        SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg MSG_CLOSE_WINDOW!!");
        break;        
    default:
        result = MMI_RESULT_TRUE;
        break;
    }

    return (result);
}

LOCAL void MMIWK_SaveBg(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_COLOR_T* guiBuf = GUIBLOCK_GetBuf(&lcd_dev_info);

    if( g_mmiwk_context.bg_guiBuf==PNULL)
    {
        g_mmiwk_context.bg_guiBuf = SCI_ALLOCA(240*MMI_WK_CLIENT_BOTTOM*2);
        SCI_MEMCPY(g_mmiwk_context.bg_guiBuf,guiBuf,240*MMI_WK_CLIENT_BOTTOM*2);
    }
    if(g_mmiwk_context.bg_guiBuf !=PNULL)
    {
        SCI_MEMCPY(g_mmiwk_context.bg_guiBuf,guiBuf,240*MMI_WK_CLIENT_BOTTOM*2);
    }

}
LOCAL void MMIWK_RestoreBg(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   bg_rect={0,MMI_WK_CLIENT_TOP,240,MMI_WK_CLIENT_BOTTOM};
    GUI_COLOR_T* guiBuf = GUIBLOCK_GetBuf(&lcd_dev_info);

    if( g_mmiwk_context.bg_guiBuf==PNULL)   //can't alloc
    {
        SCI_TRACE_LOW("mmiwk: bg_guiBuf ==PNULL !!!!");
        //GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&bg_rect.right,&bg_rect.bottom);
        if (g_mmiwk_context.is_loading == TRUE)
            {
            bg_rect.bottom = bg_rect.bottom- MMI_WK_PROGRESS_BG_BAR_HEIGHT;
        }
        LCD_FillRect(&lcd_dev_info,bg_rect,0xffff); //MMI_WHITE_COLOR
    }
    else
    {
        if (g_mmiwk_context.is_loading == TRUE)
        {
            SCI_MEMCPY(guiBuf,g_mmiwk_context.bg_guiBuf,240*(MMI_WK_CLIENT_BOTTOM-10)*2);
        }
        else
        {
            SCI_MEMCPY(guiBuf,g_mmiwk_context.bg_guiBuf,240*MMI_WK_CLIENT_BOTTOM*2);
        }
    }
}

LOCAL void MMIWK_BgExit(void)
{
    if( g_mmiwk_context.bg_guiBuf!=PNULL)
    {
        SCI_FREE(g_mmiwk_context.bg_guiBuf);
        g_mmiwk_context.bg_guiBuf = PNULL;
    }
}

LOCAL MMI_RESULT_E MMIWK_HandlePopupMenuWinMsg(
    MMI_WIN_ID_T  win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM  param)
{
    MMI_RESULT_E    recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id = MMIWK_POPUP_MENU_CTRL_ID;   
    MMI_WIN_ID_T query_win_id= MMIWK_QUERY_WIN_ID;


    switch(msg_id)
    {
	case MSG_OPEN_WINDOW:
        {
            if (g_mmiwk_context.is_sign_in)
                {
                CTRLMENU_SetItemVisible(ctrl_id,MENU_MMIWK_POPUP,ID_POPUP_MENU_WK_MAIN_SIGNUP,FALSE);
                }
            else
                {
                CTRLMENU_SetItemVisible(ctrl_id,MENU_MMIWK_POPUP,ID_POPUP_MENU_WK_MAIN_SIGNIN,FALSE);

            }
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, TRUE);        
            MMK_SetAtvCtrl( win_id, ctrl_id );
        }
    	break;

    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
            MMIPUB_OpenConfirmationDialogByTextId(PNULL, g_mmiwk_context.query_text_id,TXT_NULL, IMAGE_PUBWIN_QUERY, &query_win_id, 0,
                MMIPUB_SOFTKEY_OKCANCEL,MMIWK_HandleQueryMsg);
            MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
       // MMK_CloseWin(win_id);
        break;
     
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

LOCAL MMI_RESULT_E MMIWK_HandleEditorMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param)
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
//    MMI_CTRL_ID_T ctrl_id = MMIWK_TEXT_EDITBOX_CTRL_ID;
    MMI_CTRL_ID_T       ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPUB_INPUTFIELD_CTRL_ID ); 
    MMI_STRING_T  string  = {0};

    switch(msg_id)
    {
	case MSG_OPEN_WINDOW:
        {
#if 0			
        	static int flag = 1;
        	wchar_t* str = NULL;
        	if(1 == flag) {
            	flag = 2;
                str = L"0001000";
            }
     	    else if(2 == flag) {
            	flag = 3;
                str = L"abc12345";
            }
            else if(3 == flag) {
            	flag = 4;
                str = L"0";
            }
        	else if(4 == flag){
            	flag = 1;
            	str = L"sprd_test@outlook.com";
            }

        	GUIEDIT_SetString( ctrl_id, str, wcslen(str) );
#endif			
            //GUIEDIT_SetString(ctrl_id,string,100);
		MMIPUB_SetInputDialogEditorType(win_id,g_mmiwk_context.input_type); 
		MMIPUB_SetDialogBgLayerAlpha(win_id , 0);
            if (g_mmiwk_context.input_text_len!=0)
            {
                SCI_TRACE_LOW("mmiwk: MMIWK_HandleEditorMsg GUIEDIT_SetString");
                GUIEDIT_SetString(ctrl_id,g_mmiwk_context.input_text,g_mmiwk_context.input_text_len) ;
            }
            if (DIALOG_PASSWORD_EDITOR == g_mmiwk_context.input_type)
                {
                GUIIM_TYPE_T    allow_im        = 0;
                GUIIM_TYPE_T    init_im         = 0;

                allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;/*lint !e655*/
                init_im = GUIIM_TYPE_ABC;
                
                GUIEDIT_SetIm(ctrl_id,allow_im,init_im);
            }
            
            MMK_SetAtvCtrl( win_id, ctrl_id );
        }
    	break;

    case MSG_FULL_PAINT:
        MMIPUB_HandleDialogInputFieldWinMsg(win_id,msg_id,param); 
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
        GUIEDIT_GetString(ctrl_id, &string );

	SCI_TRACE_LOW("mmiwk: MMIWK_HandleEditorMsg g_mmiwk_context.input_type %d" , g_mmiwk_context.input_type);			

        if( 0 != string.wstr_len)
        {
            if(g_mmiwk_context.input_type == DIALOG_EMAIL_EDITOR)
            {
                MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
                MMIMSA_DATA_T msa_data = {0};
                MSA_OPERATE_TYPE_E msa_action = MMIAPIMSA_Get_Current_Action();
                
                uint8 email_str[MAX_MSA_USER_INFO_STR_SIZE+1] = {0};
                
                MMINV_READ(MMINV_MSA_DATA , &msa_data , return_value);
               
                if(MSA_APP_SIGNUP == msa_action)
                {
                    MMIAPICOM_WstrToStr(string.wstr_ptr, (uint8*)email_str);                
                    sprintf(msa_data.user_info.email , "%s%s" , email_str , "@outlook.com");
                }
                else //sign in
                {
                    MMIAPICOM_WstrToStr(string.wstr_ptr, (uint8*)msa_data.user_info.email);                
                }
               
                MMINV_WRITE(MMINV_MSA_DATA , &msa_data);
                SCI_TRACE_LOW("mmiwk: MMIWK_HandleEditorMsg save email info %s" , msa_data.user_info.email);			
            }
            WK_InputStringRequest( string.wstr_ptr, string.wstr_len);
            WK_SetViewFocuse( WK_FOCUSE_GET);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_OK:
    //case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_RED:
        MMIWK_BgExit();	
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMIWK_BgExit();	
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

PUBLIC void MMIWK_OpenEditor(WK_INPUT_TYPE_E input_type,uint16* input_text,uint32 input_text_len)
{
    MMI_WIN_ID_T edit_win_id = MMIWK_TEXT_EDITBOX_WIN_ID;
    WK_SetViewFocuse( WK_FOCUSE_KILL);

    g_mmiwk_context.input_type = input_type;

    //We should save it in global ,now MMIPUB_OpenDialogInputFieldWinByTextPtr  can't set default edit string
    if((input_text == PNULL) ||( input_text_len == 0))
    {
        g_mmiwk_context.input_text_len = 0;        
    }
    else
    {
        g_mmiwk_context.input_text_len = input_text_len;
        if(g_mmiwk_context.input_text_len >MMI_WK_EDITOR_MAX_NUM)
            g_mmiwk_context.input_text_len = MMI_WK_EDITOR_MAX_NUM;   //Set Max

        SCI_MEMCPY(g_mmiwk_context.input_text,input_text,g_mmiwk_context.input_text_len*sizeof(wchar));
    }

    SCI_TRACE_LOW("mmiwk: MMIWK_OpenEditor g_mmiwk_context.input_text_len =%d",g_mmiwk_context.input_text_len);


    if (DIALOG_PASSWORD_EDITOR == g_mmiwk_context.input_type)
        {
        MMIPUB_OpenDialogPasswordWinByTextPtr(PNULL,&edit_win_id,PNULL,PNULL,MMIWK_HandleEditorMsg);
        }
    else
        {
            MMIPUB_OpenDialogInputFieldWinByTextPtr(PNULL,&edit_win_id,PNULL,PNULL,MMIWK_HandleEditorMsg);
        }
        
    
    
}

PUBLIC int MMIWK_OutOfMemory(void)
{
    SCI_TRACE_LOW("mmiwk: MMIWK_OutOfMemory !!!!");
/*
    //WK_SetViewFocuse(WK_FOCUSE_KILL);
    //MMK_CloseWin(MMIWK_WIN_ID);
    MMK_ReturnIdleWin();

    MMIWK_ExitWin();
*/    
    return MMIBROWSER_SUCC;
}


LOCAL void MMIWK_DeleteCookie(void)
{
    HTTP_ERROR_E error_code;
    
    error_code = HttpDeleteCookieFile();
    if(error_code == 0)
    {
        SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg delete cookie file success");
    }
    else
    {
        SCI_TRACE_LOW("mmiwk: MMIWK_HandleWinMsg delete cookie file fail");
    }	
}


PUBLIC int MMIWK_Exit(uint16 exit_type)
{
    SCI_TRACE_LOW("mmiwk: MMIWK_Exit");
    WK_SetViewFocuse( WK_FOCUSE_KILL);

    if (exit_type == 1)
        {
            MMIWK_DeleteCookie();
        }
    
    WK_ExitRequest();
    MMIWK_ExitWin();

    MMK_CloseWin(MMIWK_WIN_ID);
    MMK_CloseWin(MMIWK_POPMENU_WIN_ID);
    MMK_CloseWin(MMIWK_TEXT_EDITBOX_WIN_ID);
    MMK_CloseWin(MMIWK_TEXT_SMS_WIN_ID);    
    MMK_CloseWin(MMIWK_QUERY_WIN_ID);    
    
    return MMIBROWSER_SUCC;
}


LOCAL void MMIWK_ExitWin(void)
{
    //Stop Timer
    if (0 < g_mmiwk_context.sms_display_timer)
    {
        MMK_StopTimer(g_mmiwk_context.sms_display_timer);
        g_mmiwk_context.sms_display_timer = 0;
    }
    //Free ptr
    if (g_mmiwk_context.enter_url !=PNULL)
    {
        SCI_FREE(g_mmiwk_context.enter_url);
        g_mmiwk_context.enter_url =PNULL;
    }
    if (g_mmiwk_context.prompt_str.wstr_ptr !=PNULL)
    {
        SCI_FREE(g_mmiwk_context.prompt_str.wstr_ptr);
        g_mmiwk_context.prompt_str.wstr_ptr =PNULL;
    }

    if( g_mmiwk_context.bg_guiBuf!=PNULL)
    {
        SCI_FREE(g_mmiwk_context.bg_guiBuf);
        g_mmiwk_context.bg_guiBuf = PNULL;
    }
    g_mmiwk_context.is_running = FALSE;
    return;
}

/*
PUBLIC int MMIWK_Minimize(void)
{
    WK_SetViewFocuse( WK_FOCUSE_KILL);
    MMK_CloseWin(MMIWK_WIN_ID);

    return MMIBROWSER_SUCC;
}
*/
LOCAL BOOLEAN MMIWK_InitWin(int32 url_len)
{

    MSA_OPERATE_TYPE_E msa_action = MMIAPIMSA_Get_Current_Action();
    //check original value,free ptr
    if (g_mmiwk_context.enter_url !=PNULL)
    {
        SCI_TRACE_LOW("mmiwk: MMIWK_Entry:Free enter_url, it shouldn't happend");
        SCI_FREE(g_mmiwk_context.enter_url);
        g_mmiwk_context.enter_url =PNULL;
    }
    if (g_mmiwk_context.prompt_str.wstr_ptr !=PNULL)
    {
        SCI_TRACE_LOW("mmiwk: MMIWK_Entry:prompt_str.wstr_pt!=PNULL, it shouldn't happend");
        SCI_FREE(g_mmiwk_context.prompt_str.wstr_ptr);
        g_mmiwk_context.prompt_str.wstr_ptr =PNULL;
    }
    SCI_MEMSET(&g_mmiwk_context,0,sizeof(MMIWK_CONTROL_CONTEXT_T));

    //alloc url memory
    g_mmiwk_context.enter_url = SCI_ALLOCAZ(url_len+1);
    if (PNULL == g_mmiwk_context.enter_url)
    {
        SCI_TRACE_LOW("mmiwk: MMIWK_Entry:g_enter_url ALLOC FAIL");
        return FALSE;
    }
    //alloc sms promt memory
    g_mmiwk_context.prompt_str.wstr_ptr = SCI_ALLOCAZ(160*sizeof(wchar));
    if (PNULL == g_mmiwk_context.prompt_str.wstr_ptr)
    {
        SCI_FREE(g_mmiwk_context.enter_url);
        SCI_TRACE_LOW("mmiwk: prompt_str.wstr_ptl ALLOC FAIL");
        return FALSE;
    }
    g_mmiwk_context.is_running = TRUE;
    switch(msa_action)        
    {
        case MSA_APP_SIGNIN:
            g_mmiwk_context.query_text_id= STR_MSA_EXIT_SIGN_IN_EXT01;
            g_mmiwk_context.is_sign_in = TRUE;
            break;
        case 	MSA_APP_SIGNUP:
            g_mmiwk_context.query_text_id= STR_MSA_EXIT_CREATION_EXT01;
            g_mmiwk_context.is_sign_in = FALSE;
            break;
        default:
            g_mmiwk_context.query_text_id= STR_MSA_EXIT_CREATION_EXT01;
            g_mmiwk_context.is_sign_in = FALSE;
            break;
    }    
    
    return TRUE;
}

PUBLIC int MMIWK_Entry(const MMIBROWSER_ENTRY_PARAM *entry_param_ptr)
{
#if 0
    uint32 url_len = 0;
    WK_SETTINGS_T wk_settings = {0};
    SCI_TRACE_LOW("mmiwk: MMIWK_Entry");

    if (PNULL == entry_param_ptr 
        || MMIBROWSER_ACCESS_URL != entry_param_ptr->type
        || PNULL == entry_param_ptr->url_ptr) {
        return MMIBROWSER_ERROR_INPUT;
    }

    url_len = strlen(entry_param_ptr->url_ptr);
    if(url_len < 4)
        return MMIBROWSER_ERROR_INPUT;

    if (!MMIWK_InitWin(url_len))    //init
    {
        MMIWK_ExitWin();
        return MMIBROWSER_FAIL;
    }
    
    //Save input URL
    SCI_MEMCPY(g_mmiwk_context.enter_url,(char *)entry_param_ptr->url_ptr,url_len);


    // webkit init
    if(!WK_Initialize(SCI_IdentifyThread()))
        {
        MMIWK_ExitWin();
        return MMIBROWSER_FAIL;
        }

    //create web window
    MMK_CreateWin((uint32 *)MMIWK_WIN_TAB, PNULL);


    wk_settings.net_id = entry_param_ptr->net_id;
    WK_InitRequest( &wk_settings);

    MMK_PostMsg(MMIWK_WIN_ID, MSG_MMI_WEBKIT_ENTER_URL, 0,0);
#endif
    uint32 url_len = 0;
    WK_SETTINGS_T wk_settings = {0};
    SCI_TRACE_LOW("mmiwk: MMIWK_Entry");

    if (PNULL == entry_param_ptr) {
        return MMIBROWSER_ERROR_INPUT;
    }

    url_len = strlen(entry_param_ptr->url_ptr);
    if(url_len < 4)
        return MMIBROWSER_ERROR_INPUT;

    //check entry_param_ptr->type
    switch (entry_param_ptr->type)
    {
        case MMIBROWSER_ACCESS_URL:         //Only load URL
            if(PNULL == entry_param_ptr->url_ptr)
            {
                 return MMIBROWSER_ERROR_INPUT;
            }
            if(!MMIWK_isRunningWebkit())       //only accept when mmiwk is running
                {
                 return MMIBROWSER_FAIL;
            }

	     //SCI_MEMCPY(g_mmiwk_context.enter_url,(char *)entry_param_ptr->url_ptr,url_len);
	     SCI_TRACE_LOW("mmiwk: MMIWK_Entry WK_LoadUrlRequest MMIBROWSER_ACCESS_URL entry_param_ptr->url_ptr = %s" , entry_param_ptr->url_ptr);			
            WK_LoadUrlRequest(entry_param_ptr->url_ptr);
            return MMIBROWSER_SUCC;
        case MMIBROWSER_ACCESS_MAINMENU:        //Normal Enter
            
            break;
        default:
            return MMIBROWSER_ERROR_INPUT;
    }



    if (!MMIWK_InitWin(url_len))    //init
    {
        MMIWK_ExitWin();
        return MMIBROWSER_FAIL;
    }
    
    //Save input URL
    SCI_MEMCPY(g_mmiwk_context.enter_url,(char *)entry_param_ptr->url_ptr,url_len);


    // webkit init
    if(!WK_Initialize(SCI_IdentifyThread()))
        {
        MMIWK_ExitWin();
        return MMIBROWSER_FAIL;
        }

    //create web window
    MMK_CreateWin((uint32 *)MMIWK_WIN_TAB, PNULL);


    wk_settings.net_id = entry_param_ptr->net_id;
    WK_InitRequest( &wk_settings);

    MMK_PostMsg(MMIWK_WIN_ID, MSG_MMI_WEBKIT_ENTER_URL, 0,0);
    return MMIBROWSER_SUCC;
}

LOCAL BOOLEAN MMIWK_isRunningWebkit(void)
{
    if (MMK_IsOpenWin(MMIWK_WIN_ID) ||MMK_IsFocusWin(MMIWK_TEXT_EDITBOX_WIN_ID))
        return TRUE;
    return FALSE;
}

#include "mmisms_export.h"
#include "mmisms_app.h"

PUBLIC void MMIAPIWK_HandleNewSMSInd(APP_MN_SMS_IND_T *param)
{
    APP_MN_SMS_IND_T *sig_ptr = (APP_MN_SMS_IND_T *)param;
    APP_SMS_USER_DATA_T sms_user_data_t = {0};  
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
    MN_SMS_ALPHABET_TYPE_E alpha = MN_SMS_DEFAULT_ALPHABET;
    BOOLEAN is_lock_shift = FALSE;
    BOOLEAN is_single_shift = FALSE;
    MN_SMS_USER_HEAD_T          user_head_ptr_t = {0};
    MMI_WIN_ID_T sms_win_id= MMIWK_TEXT_SMS_WIN_ID;
    BOOLEAN is_new_mms    = FALSE;
    ERR_MNSMS_CODE_E err_code = ERR_MNSMS_NONE;
    

    if(!MMIWK_isRunningWebkit())
    {
        return;
    }

    //init display string;
    g_mmiwk_context.prompt_str.wstr_len = 0;

    SCI_MEMSET( &sms_user_data_t, 0x00, sizeof( APP_SMS_USER_DATA_T ) );
    alpha = sig_ptr->sms_t.dcs.alphabet_type;
    // need call MN API to decode user head and data.
    MMISMS_DecodeUserHeadAndContent(
        sig_ptr->dual_sys,
        sig_ptr->sms_t.user_head_is_exist,
        &(sig_ptr->sms_t.dcs),
        &(sig_ptr->sms_t.user_data_t),
        &sms_user_data_t
        );

    if (MN_SMS_DEFAULT_ALPHABET == alpha)
    {
        MMISMS_DecodeUserDataHead(&(sms_user_data_t.user_data_head_t), &user_head_ptr_t);
        language = MMISMS_JudgeR8Language(&user_head_ptr_t);
        MMISMS_DecodeR8Header(&user_head_ptr_t,&is_lock_shift,&is_single_shift);
    }

    MMISMS_ConvertValidDataToDispStr(g_mmiwk_context.prompt_str.wstr_ptr,
                                     (uint16*)&(g_mmiwk_context.prompt_str.wstr_len),
                                     sms_user_data_t.user_valid_data_t.user_valid_data_arr,
                                     sms_user_data_t.user_valid_data_t.length,
                                     alpha,
                                     language,
                                     is_lock_shift,
                                     is_single_shift
                                     );

    if (MN_SMS_DEFAULT_ALPHABET == alpha)
    {
        MMISMS_FreeUserDataHeadSpace(&user_head_ptr_t);
    }

    //check it is SMS or not
    err_code = MNSMS_IsSupportMMSEx(sig_ptr->dual_sys,
        sms_user_data_t.user_data_head_t.length,
        sms_user_data_t.user_data_head_t.user_header_arr,
        &is_new_mms
        );	

     if (( ERR_MNSMS_NONE == err_code) && (is_new_mms) )
     	{
        	return;
     	}


            
    if (g_mmiwk_context.is_loading == TRUE)
    {
        //store it
        g_mmiwk_context.is_stored_sms = TRUE;
        if (0 < g_mmiwk_context.sms_display_timer)
        {
            MMK_StopTimer(g_mmiwk_context.sms_display_timer);
            g_mmiwk_context.sms_display_timer = 0;
        }
    }
    else
    {
        if(MMK_IsOpenWin(sms_win_id))
        {
            MMIPUB_SetAlertWinTextByPtr(sms_win_id,&g_mmiwk_context.prompt_str,PNULL,TRUE);
        }
        else
        {
            //show msg content directly
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&g_mmiwk_context.prompt_str,PNULL,IMAGE_PUBWIN_WARNING,&sms_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        g_mmiwk_context.is_stored_sms =FALSE;
    }
   
}


#endif //_MSC_VER >= 1400
