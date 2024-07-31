
// login page functions========================================================
#ifdef _INCLUDE_LEBAO_WIN_LOGIN_EXT_C_

#include "lebao_win_confirm.h"
#include "lebao_win_order.h"
#include "lebao_misc.h"
#include "lebao_id.h"
#include "lebao_text.h"
#include "lebao_image.h"

static MMI_HANDLE_T _loginPage = 0;
static lebao_confirm_data_t* _loginData = NULL;
static int _timerId = 0;

static void lebao_login_destory(void);
static MMI_RESULT_E lebao_login_handle_msg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
static MMI_RESULT_E lebao_login_handle_hook(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
static void lebao_login_init(MMI_WIN_ID_T win_id);
static void lebao_check_captcha_auto_input(void);
int verifyMsisdn(char* inMsisdn);
void lebao_init_login_input_phone_captcha_softkey(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T edit_id);
void lebao_reset_login_input_phone_captcha_softkey(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T edit_id);

void lebao_login_open(void* data)
{
	MMI_HANDLE_T handle = NULL;
	if (data == NULL || lebao_is_win_opened(LEBAO_PAGE_LOGIN)) {
		if (data != NULL)
			os_free(data);
		return;
	}

	_loginData = (lebao_confirm_data_t*)data;
	handle = lebao_create_win(LEBAO_PAGE_LOGIN);
	if (handle == NULL)
		return;

	_loginPage = handle;
	MMK_SetWinFunc(handle, lebao_login_handle_msg);
	// MMK_SetWinHookFunc(handle, lebao_login_handle_hook);
}

void lebao_login_close(void* data)
{
	if (_timerId != 0) {
		MMK_StopTimer(_timerId);
		_timerId = 0;
	}

	if (_loginPage != 0) {
		_loginPage = 0;
		lebao_close_win(LEBAO_PAGE_LOGIN);
	}
}

int lebao_login_is_opened(void)
{
	return (_loginPage != 0) ? 1 : 0;
}

static void lebao_login_destory(void)
{
	_loginPage = 0;

	if (_timerId != 0) {
		MMK_StopTimer(_timerId);
		_timerId = 0;
	}

	if (_loginData != NULL) {
		os_free(_loginData);
		_loginData = NULL;
	}
}

static void lebao_login_init(MMI_WIN_ID_T win_id)
{
	MMI_CTRL_ID_T titleId	= LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_LABEL_TITLE);
	MMI_CTRL_ID_T textId	= LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_TEXT_CONFIRM);
	MMI_CTRL_ID_T mobileId	= LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_EDIT_MOBILE);
	MMI_CTRL_ID_T captchaId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA);
	GUI_FONT_T      font;

	int padding = 5;

	// 240x320
	GUI_RECT_T captchaRect = { 0 };

	if (_loginData == NULL){
		return;
	}

	if (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_MSISDN) { // 输入手机号码窗口
		lebao_config_t* config = lebao_get_config();;

		captchaRect.left = padding;		
		captchaRect.right = LEBAO_SCREEN_WIDTH - padding;		
		if (LEBAO_SCREEN_HEIGHT > 160) {
			captchaRect.top = LEBAO_TITLE_HEIGHT + LEBAO_SCREEN_HEIGHT / 12;
			captchaRect.bottom = captchaRect.top + LEBAO_TITLE_HEIGHT + 20 + padding;
		}
		else {
			captchaRect.top = LEBAO_TITLE_HEIGHT;
			captchaRect.bottom = captchaRect.top + 24;
		}

		GUIEDIT_SetDividingLine(mobileId, 2, MMI_DARK_GRAY_COLOR);
		GUIEDIT_SetStyle(mobileId, GUIEDIT_STYLE_SINGLE);
		GUIEDIT_SetAlign(mobileId, ALIGN_HVMIDDLE);
		GUIEDIT_SetRect(mobileId, &captchaRect);

		font = LEBAO_BIG_FONT;
		if (LEBAO_SCREEN_HEIGHT <= 160) {
			font = LEBAO_NORMAL_FONT;
		}

		GUIEDIT_SetFont(mobileId, font);

		if (config->msisdn != NULL) {
			MMI_STRING_T	buffer = { 0 };
			wchar			w_text[128 + 1] = { 0 };

			buffer.wstr_ptr = w_text;
			buffer.wstr_len = GUI_GBToWstr(w_text, config->msisdn, os_strlen(config->msisdn));

			GUIEDIT_SetDefaultString(mobileId, buffer.wstr_ptr, buffer.wstr_len);
		}
		else {
			GUIEDIT_ClearAllStr(mobileId);
		}

		lebao_set_title_bar(
			titleId,
			LEBAO_ID_TEXT(RES_TEXT_LEBAO_MOBILE),
			NULL);

		if (LEBAO_SCREEN_HEIGHT > 160) {
			lebao_set_text_default_rect(textId, captchaRect.bottom + 10);
		}
		else {
			lebao_set_text_default_rect(textId, captchaRect.bottom + 2);
		}
		GUITEXT_IsDisplayPrg(TRUE, textId);
		font = LEBAO_NORMAL_FONT;
		if (LEBAO_SCREEN_HEIGHT <= 160) {
			font = LEBAO_SMALL_FONT;
		}
		lebao_set_text_utf8_font(textId, font, LEBAO_FONT_COLOR, _tipServerDescUtf8Arr[2 * CX_SERVER_TIP_STR_LONGIN_PHONE_INPUT + 1]);

		CTRLBASE_SetVisible(captchaId, FALSE);	// GUIEDIT_SetVisible(captchaId, FALSE, FALSE);
		CTRLBASE_SetVisible(mobileId, TRUE);	// GUIEDIT_SetVisible(mobileId, TRUE, FALSE);

		lebao_init_login_input_phone_captcha_softkey(win_id, mobileId);

		return;
	}

	if (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_CAPTCHA) { // 输入验证码窗口
		captchaRect.left = padding;
		captchaRect.right = LEBAO_SCREEN_WIDTH - padding - LEBAO_SCREEN_WIDTH / 4;
		if (LEBAO_SCREEN_HEIGHT > 160) {
			captchaRect.top = LEBAO_TITLE_HEIGHT + LEBAO_SCREEN_HEIGHT / 12;
			captchaRect.bottom = captchaRect.top + LEBAO_TITLE_HEIGHT + 20 + padding;
		}
		else {
			captchaRect.top = LEBAO_TITLE_HEIGHT;
			captchaRect.bottom = captchaRect.top + 24;
		}

		GUIEDIT_SetDividingLine(captchaId, 2, MMI_DARK_GRAY_COLOR);
		GUIEDIT_SetIm(captchaId, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);
		GUIEDIT_SetImTag(captchaId, GUIIM_TAG_PURE_NUM);
		GUIEDIT_SetStyle(captchaId, GUIEDIT_STYLE_SINGLE);
		GUIEDIT_SetAlign(captchaId, ALIGN_HVMIDDLE);
		GUIEDIT_SetRect(captchaId, &captchaRect);

		font = LEBAO_BIG_FONT;
		if (LEBAO_SCREEN_HEIGHT <= 160) {
			font = LEBAO_NORMAL_FONT;
		}

		GUIEDIT_SetFont(captchaId, font);
		GUIEDIT_ClearAllStr(captchaId);

		lebao_set_title_bar(titleId, LEBAO_ID_TEXT(RES_TEXT_LEBAO_CONFIRM),	_tipServerDescUtf8Arr[2 * CX_SERVER_TIP_STR_SNED_CAPTCHA]);

		if (LEBAO_SCREEN_HEIGHT > 160) {
			lebao_set_text_default_rect(textId, captchaRect.bottom + 10);
		}
		else {
			lebao_set_text_default_rect(textId, captchaRect.bottom + 2);
		}
		GUITEXT_IsDisplayPrg(TRUE, textId);

		font = LEBAO_NORMAL_FONT;
		if (LEBAO_SCREEN_HEIGHT <= 160) {
			font = LEBAO_SMALL_FONT;
		}

		lebao_set_text_utf8_font(textId, font, LEBAO_FONT_COLOR, _tipServerDescUtf8Arr[2 * CX_SERVER_TIP_STR_SNED_CAPTCHA + 1]);

		CTRLBASE_SetVisible(mobileId, FALSE); // GUIEDIT_SetVisible(mobileId, FALSE, FALSE);
		CTRLBASE_SetVisible(captchaId, TRUE); // GUIEDIT_SetVisible(captchaId, TRUE, FALSE);

		lebao_init_login_input_phone_captcha_softkey(win_id, captchaId);

		if (_timerId != 0) {
			MMK_StopTimer(_timerId);
			_timerId = 0;
		}
		_timerId = MMK_CreateWinTimer(_loginPage, MMI_300MSECONDS, TRUE);

		return;
	}

	return;
}

static MMI_RESULT_E lebao_login_handle_msg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T captchaId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA);
	MMI_CTRL_ID_T mobileId	= LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_EDIT_MOBILE);

	if ((_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_MSISDN) || (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_CAPTCHA)) {
		if ((msg_id == MSG_APP_WEB) || (msg_id == MSG_KEYUP_WEB)) {
			msg_id = MSG_CTL_OK;
		}
	}

	switch (msg_id)
	{
	case MSG_OPEN_WINDOW: {
		lebao_login_init(win_id);
		MMIAPISMS_CheckAndShowNewMsg();
		break;
	}

	case MSG_FULL_PAINT:
		lebao_set_default_backgroud(win_id, LEBAO_BACKGROUD_COLOR);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL,  LEFT_BUTTON,  TRUE);
		break;

	case MSG_APP_OK:
	case MSG_CTL_OK: {
		MMI_STRING_T w_captcha = { 0 };
		char captcha[20] = { 0 };

		if (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_MSISDN) {
			wchar  		 w_text[19 + 1] = { 0 };

			GUIEDIT_GetStringEx(mobileId, &w_captcha);
			if (w_captcha.wstr_len == 0)
			{
				if (lebao_get_config()->msisdn != NULL) {
					w_captcha.wstr_ptr = w_text;
					w_captcha.wstr_len = GUI_GBToWstr(w_text, lebao_get_config()->msisdn, os_strlen(lebao_get_config()->msisdn));
				}
			}
			if (w_captcha.wstr_len == 0 || w_captcha.wstr_len >= 20) {
				lebao_create_msgbox_index(RES_TEXT_LEBAO_INVALID_NUM, 2000);	// 电话号码长度不正确
				break;
			}

			MMIAPICOM_WstrToStr(w_captcha.wstr_ptr, captcha);
			if (!verifyMsisdn(captcha)) {
				lebao_create_msgbox_index(RES_TEXT_LEBAO_INVALID_NUM, 2000);	// 电话号码不正确
				break;
			}
			if(lebao_get_config()->msisdn) {
				sdsfree(lebao_get_config()->msisdn);
			}
			lebao_get_config()->msisdn = sdsnew(captcha);
		}
		else if (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_CAPTCHA) {
			GUIEDIT_GetString(captchaId, &w_captcha);
			if (w_captcha.wstr_len == 0 || w_captcha.wstr_len >= 10) {
				lebao_create_msgbox_index(RES_TEXT_LEBAO_INVALID_CAPTCHA, 2000);
				break;
			}

			MMIAPICOM_WstrToStr(w_captcha.wstr_ptr, captcha);
			if(lebao_get_config()->captcha) {
				sdsfree(lebao_get_config()->captcha);
			}
			lebao_get_config()->captcha = sdsnew(captcha);
		}
		else {
			GUIEDIT_GetString(captchaId, &w_captcha);
			if (w_captcha.wstr_len == 0 || w_captcha.wstr_len >= 10) {
				lebao_create_msgbox_index(RES_TEXT_LEBAO_EMPTY, 2000);
				break;
			}

			MMIAPICOM_WstrToStr(w_captcha.wstr_ptr, captcha);
			lebao_order_set_captcha(captcha);
		}

		if (_loginData != NULL && _loginData->leftFunc != NULL) {
			_loginData->leftFunc((void *)_loginData);
		}

		lebao_login_close(NULL);
		break;
	}

	case MSG_APP_WEB:
	case MSG_KEYUP_WEB: {
		MMK_SetAtvCtrl(win_id, captchaId);
		break;
	}

	case MSG_TIMER:
		if(_timerId == (*(uint8*)param)) {
			lebao_check_captcha_auto_input();
		}
		break;

	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL: {
		if (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_MSISDN) {
			if (MMK_GetActiveCtrlId(win_id) != mobileId) {
				MMK_SetAtvCtrl(win_id, mobileId);
				MMK_UpdateScreen();
				break;
			}
		}
		else if (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_CAPTCHA) {
			if (MMK_GetActiveCtrlId(win_id) != captchaId) {
				MMK_SetAtvCtrl(win_id, captchaId);
				MMK_UpdateScreen();
				break;
			}
		}

		if (_loginData != NULL && _loginData->rightFunc != NULL) {
			_loginData->rightFunc((void *)_loginData);
		}

		lebao_login_close(NULL);
		break;
	}

	case MSG_CLOSE_WINDOW:
		lebao_login_destory();
		break;

	default:
		result = MMI_RESULT_FALSE;
		break;
	}

/*	if (((msg_id & KEY_PRESSED) == KEY_PRESSED) || ((msg_id & KEY_RELEASED) == KEY_RELEASED))
	{
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL,  LEFT_BUTTON,  TRUE);
	} */

	return (result);
}

static MMI_RESULT_E lebao_login_handle_hook(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_FALSE;
	MMI_CTRL_ID_T captchaId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA);
	MMI_CTRL_ID_T mobileId	= LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_EDIT_MOBILE);

	if ((_loginData->id != LEBAO_ORDER_OPT_NODE_LOGIN_MSISDN) && (_loginData->id != LEBAO_ORDER_OPT_NODE_LOGIN_CAPTCHA)) {
		return result;
	}

	switch (msg_id)
	{
	case MSG_KEYDOWN_0: case MSG_KEYDOWN_1: case MSG_KEYDOWN_2: case MSG_KEYDOWN_3: case MSG_KEYDOWN_4:
	case MSG_KEYDOWN_5: case MSG_KEYDOWN_6: case MSG_KEYDOWN_7: case MSG_KEYDOWN_8: case MSG_KEYDOWN_9:
	case MSG_KEYDOWN_LEFT: case MSG_KEYDOWN_RIGHT:
		if (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_MSISDN) {
			if (MMK_GetActiveCtrlId(win_id) != mobileId) {
				MMK_SetAtvCtrl(win_id, mobileId);
				lebao_reset_login_input_phone_captcha_softkey(win_id, mobileId);
				MMK_UpdateScreen();
			}
		}
		else if (_loginData->id == LEBAO_ORDER_OPT_NODE_LOGIN_CAPTCHA) {
			if (MMK_GetActiveCtrlId(win_id) != captchaId) {
				MMK_SetAtvCtrl(win_id, captchaId);
				lebao_reset_login_input_phone_captcha_softkey(win_id, captchaId);
				MMK_UpdateScreen();
			}
		}
		break;

	default:
		break;
	}

	return (result);
}

static void lebao_check_captcha_auto_input(void)
{
	MMI_CTRL_ID_T captchaId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA);
	char *code;

	if (!_loginData || (_loginData->id != LEBAO_ORDER_OPT_NODE_LOGIN_CAPTCHA)) {
		return;
	}

	code = lebao_caixin_login_sms_captcha_filter_get();

	if (code != NULL) {
		wchar	buffer[10] = { 0 };
		uint16	len = 0;

		len = GUI_UTF8ToWstr(buffer, 8, (const uint8*)code, os_strlen(code));

		GUIEDIT_SetString(captchaId, buffer, len);
		lebao_caixin_login_sms_filter_clear();

		MMK_UpdateScreen();

		if (_timerId != 0) {
			MMK_StopTimer(_timerId);
			_timerId = 0;
		}
	}
}


int verifyMsisdn(char* inMsisdn)
{ // 有效长度 == 11 位
	unsigned char* p;

	if ((inMsisdn == NULL) || (inMsisdn[0] == 0)) {
		return 0;
	}

	p = (unsigned char*)inMsisdn;
	if (strlen(p) < 11) {
		return 0;
	}
	if (*p == '+') {
		p++;
	}
	if (*p == '0') {
		p++;
	}
	if ((p[0] == '8') && (p[1] == '6')) {
		p += 2;
	}

	if (strlen(p) != 11) {
		return 0;
	}
	if ((p[0] != '1') || (p[1] < '3') || (p[1] > '9')) {
		return 0;
	}

	while(*p != '\0')
	{
		if((*p < '0'|| *p > '9'))
			return 0;
		p++;
	}

	return 1;
}

/* 登录时输入电话号码和短信验证码时, 配置显示的 softkey */
void lebao_init_login_input_phone_captcha_softkey(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T edit_id)
{
	// GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL,  LEFT_BUTTON,  FALSE);
	// GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL,  LEFT_BUTTON,  FALSE);
	// GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL,  LEFT_BUTTON,  FALSE);

	GUIWIN_SeSoftkeytButtonTextId(win_id, LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OK),  LEFT_BUTTON,  TRUE);
	GUIWIN_SeSoftkeytButtonTextId(win_id, lebao_softkey_null_button_text_id_get(), MIDDLE_BUTTON,TRUE);
	GUIWIN_SeSoftkeytButtonTextId(win_id, lebao_softkey_return_button_text_id_get(), RIGHT_BUTTON, TRUE);

	GUIEDIT_SetSoftkey(edit_id, LEFT_BUTTON, 0, LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OK), LEBAO_ID_TEXT(RES_TEXT_LEBAO_SOFTKEY_OK), NULL);

	GUIWIN_SetSoftkeyVisible(win_id, TRUE);
	MMK_SetAtvCtrl(win_id, edit_id);
}
/* 登录时输入电话号码和短信验证码时, 键入0-9,左/右键时, 配置显示 softkey */
void lebao_reset_login_input_phone_captcha_softkey(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T edit_id)
{
	// GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL,  LEFT_BUTTON,  TRUE);	// disable left icon key
}


#endif
