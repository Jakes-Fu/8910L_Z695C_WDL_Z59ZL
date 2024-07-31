/**********************
 *      INCLUDES
 *********************/
#include "port_cfg.h"

#include "std_header.h"
#include "mmk_app.h"
#include "mmi_common.h"
#include "mmk_timer.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"

#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#include "lebao_ctrl.h"
#include "lebao_misc.h"
#include "lebao_win_base.h"
#include "lebao_app.h"

#include "lebao_id.h"
#include "lebao_text.h"
#include "lebao_image.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lebao_show_no_network(void);
static MMI_RESULT_E lebao_null_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
static MMI_RESULT_E lebao_hook_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/**********************
 *  STATIC VARIABLES
 **********************/

 // splash page
WINDOW_TABLE(MMI_LEBAO_SPLASH_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_BACKGROUND_ID(IMAGE_LEBAO_SPLASH),
	WIN_ID(MMI_LEBAO_WIN_ID_SPLASH),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_WELCOME),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_LOADING, MMI_LEBAO_WIN_ID_SPLASH),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_HIDE_STATUS,
	END_WIN
};

// menu page
WINDOW_TABLE(MMI_LEBAO_MENU_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_MAINMENU),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_MENU),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

// about page
WINDOW_TABLE(MMI_LEBAO_ABOUT_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_ABOUT),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_ABOUT),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};


// charts page
WINDOW_TABLE(MMI_LEBAO_CHARTS_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_CHARTS),
	//WIN_TITLE(TEXT_LEBAO_MAIN_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_CHARTS),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	//WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_HIDE_STATUS,
	END_WIN
};


// music list page
WINDOW_TABLE(MMI_LEBAO_MUSICLIST_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_MUSICLIST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_LEBAO_CTRL_ID_LIST_MUSICLIST),
	WIN_SOFTKEY(TEXT_LEBAO_PLAY, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

// books list page
WINDOW_TABLE(MMI_LEBAO_BOOKLIST_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_BOOKLIST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_LEBAO_CTRL_ID_LIST_BOOKLIST),
	WIN_SOFTKEY(TEXT_LEBAO_PLAY, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

// option menu page
WINDOW_TABLE(MMI_LEBAO_OPTION_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_OPTION),
	WIN_STYLE(WS_HAS_TRANSPARENT),
	WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

// player page
WINDOW_TABLE(MMI_LEBAO_PLAYER_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_PLAYER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_SONG_NAME),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_SINGER_NAME),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_PREVIOU,	MMI_LEBAO_CTRL_ID_BTN_PREV),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_PLAY,	MMI_LEBAO_CTRL_ID_BTN_PLAY),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_NEXT,	MMI_LEBAO_CTRL_ID_BTN_NEXT),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_RING,	MMI_LEBAO_CTRL_ID_BTN_RING),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_FAVORITE,	MMI_LEBAO_CTRL_ID_BTN_FAVORITE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,		MMI_LEBAO_CTRL_ID_LABEL_CURRENT_TIME),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT,		MMI_LEBAO_CTRL_ID_LABEL_TOTAL_TIME),
	WIN_SOFTKEY(STXT_OPTION, TEXT_LEBAO_PLAY, STXT_RETURN),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_HIDE_STATUS,
	END_WIN
};

// volume page
WINDOW_TABLE(MMI_LEBAO_VOLUME_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_VOLUME),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

// My favorites page
WINDOW_TABLE(MMI_LEBAO_LOCAL_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_LOCAL),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	//CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_DELETE,	MMI_LEBAO_CTRL_ID_BTN_DEL),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_LOCAL),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

// search page
WINDOW_TABLE(MMI_LEBAO_SEARCH_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_SEARCH),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TIMER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TIPS),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_RECORD_GRAY,	MMI_LEBAO_CTRL_ID_BTN_RECORD),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

// waiting page
WINDOW_TABLE(MMI_LEBAO_WAITING_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_WAITING),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TIPS),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TIMER),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_LOADING, MMI_LEBAO_WIN_ID_WAITING),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
	WIN_HIDE_STATUS,
	END_WIN
};

// order page
WINDOW_TABLE(MMI_LEBAO_ORDER_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_ORDER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_QRCODE, 0),
	// CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_SCAN),
	CREATE_TEXT_CTRL(MMI_LEBAO_CTRL_ID_LABEL_SCAN),		/* 允许换行显示 */
	CREATE_BUTTON_CTRL(IMAGE_NULL, MMI_LEBAO_CTRL_ID_BTN_CALL),
	WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_OK, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

WINDOW_TABLE(MMI_LEBAO_ORDER_OPT_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_OPTION),
	WIN_STYLE(WS_HAS_TRANSPARENT),
	WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

WINDOW_TABLE(MMI_LEBAO_VIP_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_BACKGROUND_ID(IMAGE_LEBAO_SPLASH),
	WIN_ID(MMI_LEBAO_WIN_ID_VIP),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_VIP, 0),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TIPS),
	WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

// confirm page
WINDOW_TABLE(MMI_LEBAO_CONFIRM_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_CONFIRM),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	CREATE_BUTTON_CTRL(IMAGE_NULL, MMI_LEBAO_CTRL_ID_BTN_PLAY),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_QRCODE, 0),
	CREATE_TEXT_CTRL(MMI_LEBAO_CTRL_ID_TEXT_CONFIRM),
	WIN_HIDE_STATUS,
	END_WIN
};

// login page
WINDOW_TABLE(MMI_LEBAO_LOGIN_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_LOGIN),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),	// WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_EDIT_DIGITAL_CTRL(16, MMI_LEBAO_CTRL_ID_EDIT_MOBILE),
	CREATE_EDIT_TEXT_CTRL(6, MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA),
	CREATE_TEXT_CTRL(MMI_LEBAO_CTRL_ID_TEXT_CONFIRM),
	WIN_HIDE_STATUS,
	END_WIN
};

WINDOW_TABLE(MMI_LEBAO_HISTORY_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_HISTORY),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_LOCAL),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

static struct
{
	int pageId;
	uint32* tab;
	int winId;
} _resWinId[] = {
	{ LEBAO_PAGE_SRV,		NULL, 0 },
	{ LEBAO_PAGE_CTRL,		NULL, 0 },
	{ LEBAO_PAGE_SPLASH,	MMI_LEBAO_SPLASH_WIN_TAB,	MMI_LEBAO_WIN_ID_SPLASH },
	{ LEBAO_PAGE_MENU,		MMI_LEBAO_MENU_WIN_TAB,		MMI_LEBAO_WIN_ID_MAINMENU },
	{ LEBAO_PAGE_CHART,		MMI_LEBAO_CHARTS_WIN_TAB,	MMI_LEBAO_WIN_ID_CHARTS },
	{ LEBAO_PAGE_MUSICLIST, MMI_LEBAO_MUSICLIST_WIN_TAB, MMI_LEBAO_WIN_ID_MUSICLIST },
	{ LEBAO_PAGE_BOOKLIST,  MMI_LEBAO_BOOKLIST_WIN_TAB, MMI_LEBAO_WIN_ID_BOOKLIST },
	{ LEBAO_PAGE_PLAYER,	MMI_LEBAO_PLAYER_WIN_TAB,	MMI_LEBAO_WIN_ID_PLAYER },
	{ LEBAO_PAGE_VOLUME,	MMI_LEBAO_VOLUME_WIN_TAB,	MMI_LEBAO_WIN_ID_VOLUME },
	{ LEBAO_PAGE_LOCAL,		MMI_LEBAO_LOCAL_WIN_TAB,	MMI_LEBAO_WIN_ID_LOCAL },
	{ LEBAO_PAGE_SEARCH,	MMI_LEBAO_SEARCH_WIN_TAB,	MMI_LEBAO_WIN_ID_SEARCH },
	{ LEBAO_PAGE_RECORD,	NULL, 0 },
	{ LEBAO_PAGE_WAITING,	MMI_LEBAO_WAITING_WIN_TAB,	MMI_LEBAO_WIN_ID_WAITING },
	{ LEBAO_PAGE_ORDER,		MMI_LEBAO_ORDER_WIN_TAB,	MMI_LEBAO_WIN_ID_ORDER },
	{ LEBAO_PAGE_STREAM,	NULL, 0 },
	{ LEBAO_PAGE_ABOUT,		MMI_LEBAO_ABOUT_WIN_TAB,	MMI_LEBAO_WIN_ID_ABOUT },
	{ LEBAO_PAGE_CONFIRM,	MMI_LEBAO_CONFIRM_WIN_TAB,	MMI_LEBAO_WIN_ID_CONFIRM },
	{ LEBAO_PAGE_OPTION,	MMI_LEBAO_OPTION_WIN_TAB,	MMI_LEBAO_WIN_ID_OPTION },
	{ LEBAO_PAGE_VIP,		MMI_LEBAO_VIP_WIN_TAB,		MMI_LEBAO_WIN_ID_VIP },
	{ LEBAO_PAGE_HISTORY,	MMI_LEBAO_HISTORY_WIN_TAB,	MMI_LEBAO_WIN_ID_HISTORY },
	{ LEBAO_PAGE_LOGIN,		MMI_LEBAO_LOGIN_WIN_TAB,	MMI_LEBAO_WIN_ID_LOGIN },
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

MMI_HANDLE_T lebao_create_win(const int pageId)
{
	MMI_HANDLE_T handle = NULL;

	if (pageId <= 0 || pageId > LEBAO_PAGE_LOGIN)
		return NULL;

	if (_resWinId[pageId].tab == NULL || _resWinId[pageId].winId == 0)
		return NULL;

	if (MMK_IsOpenWin(_resWinId[pageId].winId))
		return NULL;

	handle = MMK_CreateWin((uint32*)_resWinId[pageId].tab, PNULL);
	if (handle != NULL)
		MMK_SetWinHookFunc(handle, lebao_hook_win_func);

	helper_trace("pageId=%d, handle=0x%x", pageId, handle);
	return handle;
}

void lebao_close_win(const int pageId)
{
	helper_trace("pageId=%d", pageId);

	if (pageId <= 0 || pageId > LEBAO_PAGE_LOGIN)
		return;

	if (_resWinId[pageId].tab == NULL || _resWinId[pageId].winId < MMI_LEBAO_WIN_ID_SPLASH || _resWinId[pageId].winId > MMI_LEBAO_WIN_ID_LOGIN)
		return;

	if (MMK_IsOpenWin(_resWinId[pageId].winId)) {
		MMK_CloseWin(_resWinId[pageId].winId);
	}
}

BOOLEAN	lebao_is_win_opened(const int pageId)
{
	if (pageId <= 0 || pageId > LEBAO_PAGE_LOGIN)
		return FALSE;

	if (_resWinId[pageId].tab == NULL || _resWinId[pageId].winId == 0)
		return FALSE;

	if (MMK_IsOpenWin(_resWinId[pageId].winId))
		return TRUE;

	return FALSE;
}

int lebao_get_pageid(int win_id)
{
	int  i = 0;

	if(win_id < MMI_LEBAO_WIN_ID_SPLASH || win_id > MMI_LEBAO_WIN_ID_LOGIN)
		return 0;

	for(i = LEBAO_PAGE_SPLASH; i <= LEBAO_PAGE_LOGIN; i++) {
		if(_resWinId[i].winId == win_id)
			return _resWinId[i].pageId;
	}

	return 0;
}

MMI_TEXT_ID_T lebao_softkey_null_button_text_id_get(void)
{
	return TXT_NULL;
}

MMI_TEXT_ID_T lebao_softkey_return_button_text_id_get(void)
{
	return STXT_RETURN;
	// return TXT_ENG_RETURN;
}

int lebao_set_win_softkey(const int page_id, const int sub_id, const uint32 win_id)
{	
	MMI_IMAGE_ID_T left, middle, right;

	left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OK);
	middle = lebao_softkey_null_button_text_id_get();
	right = lebao_softkey_return_button_text_id_get();

	switch (page_id)
	{
		case LEBAO_PAGE_MENU:
		case LEBAO_PAGE_CHART:
		case LEBAO_PAGE_MUSICLIST:
		case LEBAO_PAGE_ABOUT:
		case LEBAO_PAGE_VOLUME:
			break;
		
		case LEBAO_PAGE_PLAYER:
			if (sub_id == 0) {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OPTION);
				middle = LEBAO_ID_TEXT(RES_TEXT_LEBAO_PLAY);
				right = lebao_softkey_return_button_text_id_get();
			}
			else if (sub_id == LEBAO_SUB_PAGE_PLAYER_OPTION) { /* 音乐播放界面, Option 子菜单开启后, 显示的 softkey */
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OK);
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();

				GUIWIN_SetSoftkeyVisible(win_id, TRUE);
			}
			break;
		
		case LEBAO_PAGE_WAITING:
			if (sub_id == LEBAO_SUB_PAGE_WAITING_RETURN) {
				left = lebao_softkey_null_button_text_id_get();
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			else {
				left = lebao_softkey_null_button_text_id_get();
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_null_button_text_id_get();
			}
			break;

		case LEBAO_PAGE_ORDER:
			if (sub_id == LEBAO_SUB_PAGE_ORDER_SHOW_MID_KEY) {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OPTION);
				middle = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OK);
				right = lebao_softkey_return_button_text_id_get();
			}
			else if (sub_id == LEBAO_SUB_PAGE_ORDER_NO_MID_KEY) {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OPTION);
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			else if (sub_id == LEBAO_SUB_PAGE_ORDER_OPTION) { /* 会员服务页面中, option 子菜单显示时, 配置 softkey */
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OK);
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
				GUIWIN_SetSoftkeyVisible(win_id, TRUE);
			}
			break;

		case LEBAO_PAGE_VIP:
			if (sub_id == LEBAO_SUB_PAGE_VIP_SHOW_LEFT_KEY) {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OPTION);
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			else if (sub_id == LEBAO_SUB_PAGE_VIP_NO_LEFT_KEY) {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OPTION);
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			break;
			
		case LEBAO_PAGE_SEARCH:
			left = lebao_softkey_null_button_text_id_get();
			middle = lebao_softkey_null_button_text_id_get();
			right = lebao_softkey_return_button_text_id_get();
			break;

		case LEBAO_PAGE_CONFIRM:
			if (sub_id == LEBAO_SUB_PAGE_CONFIRM_ONE_RETRUN) {
				left = lebao_softkey_null_button_text_id_get();
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			else if (sub_id == LEBAO_SUB_PAGE_CONFIRM_OK_RETRUN) {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OK);
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			else if (sub_id == LEBAO_SUB_PAGE_CONFIRM_AGRESS_RETRUN) {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_AGREE);
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			else if (sub_id == LEBAO_SUB_PAGE_CONFIRM_AGRESS_GUESTLOGIN) {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_AGREE);
				middle = lebao_softkey_null_button_text_id_get();
				right = LEBAO_ID_TEXT(RES_TEXT_LEBAO_GUEST_LOGIN);
			}
			
			break;

		case LEBAO_PAGE_LOCAL:
			if (sub_id == LEBAO_SUB_PAGE_LOCAL_NO_SONG) {
				left = lebao_softkey_null_button_text_id_get();
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			else {
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_DEL);
				middle = LEBAO_ID_TEXT(RES_TEXT_LEBAO_PLAY);
				right = lebao_softkey_return_button_text_id_get();
			}
			break;
		
		case LEBAO_PAGE_HISTORY:
			if (sub_id == LEBAO_SUB_PAGE_LOCAL_NO_SONG) {
				left = lebao_softkey_null_button_text_id_get();
				middle = lebao_softkey_null_button_text_id_get();
				right = lebao_softkey_return_button_text_id_get();
			}
			else {				
				left = LEBAO_ID_TEXT(RES_TEXT_LEBAO_DEL);
				middle = LEBAO_ID_TEXT(RES_TEXT_LEBAO_PLAY);
				right = lebao_softkey_return_button_text_id_get();
			}
			break;

		default:
			return 0;
	}

	GUIWIN_SeSoftkeytButtonTextId(win_id, left,  LEFT_BUTTON,  TRUE);
	GUIWIN_SeSoftkeytButtonTextId(win_id, middle,MIDDLE_BUTTON,TRUE);
	GUIWIN_SeSoftkeytButtonTextId(win_id, right, RIGHT_BUTTON, TRUE);

	return 0;
}

void lebao_player_update_playstatus_icon(const uint32 win_id)
{
	MMI_CTRL_ID_T playId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_BTN_PLAY);
	GUI_BG_T playFg = { GUI_BG_IMG, GUI_SHAPE_RECT, IMAGE_LEBAO_BTN_PLAY, 0, TRUE };

	if (lebao_player_get_status() == LEBAO_STATUS_PLAY) {
		GUIWIN_SeSoftkeytButtonTextId(win_id, LEBAO_ID_TEXT(RES_TEXT_LEBAO_PAUSE), MIDDLE_BUTTON, 0);
		playFg.img_id = LEBAO_ID_IMG(RES_IMAGE_LEBAO_BTN_PAUSE);
	} else if (lebao_player_get_status() == LEBAO_STATUS_PAUSE) {
		GUIWIN_SeSoftkeytButtonTextId(win_id, LEBAO_ID_TEXT(RES_TEXT_LEBAO_PLAY), MIDDLE_BUTTON, 0);
		playFg.img_id = LEBAO_ID_IMG(RES_IMAGE_LEBAO_BTN_PLAY);
	} else {
		GUIWIN_SeSoftkeytButtonTextId(win_id, LEBAO_ID_TEXT(RES_TEXT_LEBAO_PLAY), MIDDLE_BUTTON, 0);
		playFg.img_id = LEBAO_ID_IMG(RES_IMAGE_LEBAO_BTN_PLAY);
	}

	GUIBUTTON_SetFg(playId, &playFg);
}

/* 搜索歌曲时, 设定开始语音录音，以及录音结束的 消息id */
void lebao_search_win_record_handle_msgid_get(MMI_MESSAGE_ID_E *start_msgid, MMI_MESSAGE_ID_E *stop_msgid)
{	/* 默认默认值为 MSG_KEYLONG_WEB, MSG_KEYPRESSUP_WEB. 修改为其它按键值时,需要修改提示字符串提示内容, ID为 TEXT_LEBAO_PUSH_TALK_PHONE, TEXT_LEBAO_OVER_TALK */
	 // *start_msgid = MSG_KEYLONG_WEB;
	 // *stop_msgid = MSG_KEYPRESSUP_WEB;

	// 同步修改字符串资源  TEXT_LEBAO_PUSH_TALK_PHONE
	// *start_msgid = MSG_KEYLONG_OK;
	// *stop_msgid = MSG_KEYPRESSUP_OK;

	*start_msgid = MSG_KEYLONG_WEB;
	*stop_msgid = MSG_KEYPRESSUP_WEB;
}

/* 播放歌曲窗口, 按键触发音量或其它动作时, 消息预处理, 过滤, 或按键消息转换  */
int lebao_player_win_user_handle_msg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM para, int *result)
{
#if 0
	if(msg_id == MSG_APP_UP) {
		int vol = lebao_get_volume() + 1;

		if (vol > lebao_get_max_volume())
			vol = lebao_get_max_volume();

		lebao_set_volume(vol);

		*result = MMI_RESULT_TRUE;
		return 1;	// 消息已处理并终结
	}

	if(msg_id == MSG_APP_DOWN) {
		int vol = lebao_get_volume() - 1;

		if (vol > lebao_get_max_volume())
			vol = lebao_get_max_volume();

		lebao_set_volume(vol);

		*result = MMI_RESULT_TRUE;
		return 1;	// 消息已处理并终结
	}
#endif

	return 0;	// 消息未处理, 需要继续处理
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static MMI_RESULT_E lebao_null_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	(void)win_id;
	(void)msg_id;
	(void)param;

	return MMI_RESULT_FALSE;
}

static MMI_RESULT_E lebao_back_btn_click(void)
{
	MMK_DispMsgToFocusWin(MSG_CTL_CANCEL, NULL);
	return MMI_RESULT_TRUE;
}

static BOOLEAN _keyRing = FALSE;
static BOOLEAN _tpRing = FALSE;

static MMI_RESULT_E lebao_hook_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	// hide softkeybar
	if(msg_id == MSG_OPEN_WINDOW || msg_id == MSG_FULL_PAINT) {
		#if defined(LEBAO_HIDE_SOFTKEYBAR)
		GUIWIN_SetSoftkeyVisible(win_id, FALSE);
		#endif
	}

	// whether to turn off the key tone and turn off the screen
	if(win_id == MMI_LEBAO_WIN_ID_PLAYER) {
		if(msg_id == MSG_OPEN_WINDOW) {
			_keyRing = MMIDEFAULT_IsEnableKeyRing();
			_tpRing  = MMIDEFAULT_IsEnableTpRing();
			
			MMIDEFAULT_EnableKeyTpRing("lebao", FALSE);

			if (lebao_player_is_backend() == 0) {
				// MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
				// MMIDEFAULT_TurnOnBackLight();
				// MMIDEFAULT_AllowTurnOffBackLight(FALSE);

				// MMIDEFAULT_SetDeepSleep(FALSE);
				// MMIDEFAULT_StopAutoKeylockTimer();
				// MMIDEFAULT_AllowOpenKeylocWin
			}
		}
		else if(msg_id == MSG_CLOSE_WINDOW) {
			MMIDEFAULT_EnableKeyRing("lebao", _keyRing);
			MMIDEFAULT_EnableTpRing("lebao", _tpRing);

			if (lebao_player_is_backend() == 0) {
				MMIDEFAULT_AllowTurnOffBackLight(TRUE);
				// MMIDEFAULT_SetAlwaysHalfOnBackLight(TRUE);
			}
		}
	}
	else if(win_id == MMI_LEBAO_WIN_ID_SEARCH) {
		if(msg_id == MSG_OPEN_WINDOW) {
			_keyRing = MMIDEFAULT_IsEnableKeyRing();
			_tpRing  = MMIDEFAULT_IsEnableTpRing();

            MMIDEFAULT_EnableKeyTpRing("lebao", FALSE);
		}
		else if(msg_id == MSG_CLOSE_WINDOW) {
			MMIDEFAULT_EnableKeyRing("lebao", _keyRing);
			MMIDEFAULT_EnableTpRing("lebao", _tpRing);
		}
	}

	// quit app
	if(msg_id == MSG_APP_RED) {
		#if 1
			helper_trace("MSG_APP_RED trig.");
			MMIAPIMENU_QuitLebao();
			return MMI_RESULT_TRUE; // Must be true !!! otherwise, crash
		#else
			MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
			return MMI_RESULT_TRUE;
			// or
			// MMK_CloseWin(win_id);
			// or
			// int pageId = lebao_get_pageid(win_id);
			// if(pageId != 0 && pageId != LEBAO_PAGE_SPLASH)
			//	 lebao_event_send(EVT_CTRL_LEBAO_CLOSE_PAGE, LEBAO_PAGE_CTRL, pageId, 0);
		#endif
	}

	return MMI_RESULT_FALSE;
}

/* lebao 启动播放或录音时的 callback */
void lebao_music_start_playing_callback(int record_flag)
{
/* 在 lebao_hook_win_func 中已处理, 仅需可以选择一处处理按键音
	_keyRing = MMIDEFAULT_IsEnableKeyRing();
	_tpRing  = MMIDEFAULT_IsEnableTpRing();

	MMIDEFAULT_EnableKeyRing("lebao", FALSE);
	MMIDEFAULT_EnableKeyTpRing("lebao", FALSE);
*/
}
/* lebao 退出播放或录音时的 callback */
void lebao_music_stop_playing_callback(int record_flag)
{
/*  在 lebao_hook_win_func 中已处理, 仅需可以选择一处处理按键音
	MMIDEFAULT_EnableKeyRing("lebao", _keyRing);
	MMIDEFAULT_EnableTpRing("lebao", _tpRing);
*/
}
