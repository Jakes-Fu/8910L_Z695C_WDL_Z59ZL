/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"

#include "mn_type.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_default.h"
#include "mmi_text.h"
#include "mmi_theme.h"
#include "guilistbox.h"
#include "gui_ucs2b_converter.h"

#define OD_GetMainWindowID(module_id) ((((unsigned int)module_id) << 16) + 1)

#ifdef PLATFORM_SC6600L
/* 6600 have following two functions, but no in header files. */
BOOLEAN MMICONNECTION_RegisterNotifyInfo(MMICONNECTION_NOTIFY_INFO_T *notify_info_ptr);
void MMICONNECTION_UnregisterNotifyInfo(uint32 module_id);
#endif

/* The platform has a value TCPIP_NETID_NULL(0) being invalid net id. But in simulator,
the net id can be 0, and it is right for them. So I think there is no invalid net id then.
So there is no OD_DEFAULT_NET_ID. When network is down, I set id to OD_DEFAULT_NET_ID. */
#define OD_DEFAULT_NET_ID   (0)

#define OD_INVALID_GATEWAY              "0.0.0.0"

static OD_NetType _OD_NetTranslateType(MMIPDP_INTERFACE_E type);

static MN_DUAL_SYS_E _OD_NetGetSelectSIM(const OD_Context *context);
static void _OD_NetSetState(OD_Context *context, OD_NetState state, OD_NetID id, OD_NetType type);
static int _OD_GetNetProfileIndex(const OD_Context *context, int sim);
static OD_STATUS OD_NetResetSimAccount(OD_Context *context);
static void _OD_HandleNetworkPDPMessage(OD_Context* context, MMIPDP_CNF_INFO_T *msg_ptr);
static OD_STATUS _OD_NetUpdateProfile(OD_Context *context, int sim, int apn_index);
static MMICONNECTION_LINKSETTING_DETAIL_T* _OD_GetLinkSettingItemByIndex(MN_DUAL_SYS_E dual_sys, uint8 index);
static uint8 _OD_GetLinkSettingNum(MN_DUAL_SYS_E dual_sys);
static uint8 _OD_GetAccountDefaultIndex(MN_DUAL_SYS_E dual_sys);


OD_MMI_TASK static void _OD_PDPMessageCallback(MMIPDP_CNF_INFO_T *msg_ptr)
{
	/* This function just send(yes, SEND, not POST) message to the module's main window.
	The function _OD_HandleNetworkPDPMessage really handle this message. */
	unsigned int module_id = 0;

	if (!msg_ptr)
	{
		OD_TRACE_LOW("net: _OD_PDPMessageCallback, msg_ptr == NULL");
		return;
	}

	module_id = msg_ptr->app_handler;
	if ( FALSE
#ifdef OPERA_MOBILE_SUPPORT
		|| module_id == MMI_MODULE_OPERA_MOBILE
#endif /* OPERA_MOBILE_SUPPORT */
#ifdef OPERA_MINI_SUPPORT
		|| module_id == MMI_MODULE_OPERA_MINI
#endif /* OPERA_MINI_SUPPORT */
		)
	{
		MMI_HANDLE_T winid = OD_GetMainWindowID(module_id);
		OD_Context* context = NULL;
		if (MMK_IsOpenWin(winid))
		{
			context = MMK_GetWinAddDataPtr(winid);
			if (context && IsValidContext(context))
			{
				_OD_HandleNetworkPDPMessage(context, msg_ptr);
			}
		}
	}
}

OD_NetID OD_NetGetID(const OD_Context *context)
{
	OD_CHECK_ARGS(context, OD_DEFAULT_NET_ID);
	//In modis, the net is 0. we can't know if net_id is valid.
	return context->data.net.net_id;
}

OD_STATUS OD_NetGetProxy(const OD_Context *context, OD_BOOL * use_proxy, OD_Proxy* proxy)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(use_proxy);
	OD_CHECK_ARGS_RETURN_STATUS(proxy);

#if defined(WIFI_SUPPORT) && !defined(WIFI_SUPPORT_UNISOC_RX)
	if (context->data.net.selected_sim == OD_SIM_INDEX_WIFI)
	{
		MMIWLAN_PROXY_SETTING_DETAIL_T setting;   /* Note: this will consume about 1.5K stack. */
		MMIWIFI_GetProxySettingDetail(&setting);
		*use_proxy = setting.is_use_proxy;
		if(*use_proxy)
		{
			int tmp = MIN(OD_MAX_PROXY_IP_LEN, setting.gateway_len);
			od_strncpy(proxy->ip, (const char*)setting.gateway, tmp);
			proxy->ip[tmp] = 0;

			proxy->port = setting.port;

			tmp = MIN(OD_MAX_PROXY_USERNAME_LEN, setting.username_len);
			od_strncpy(proxy->username, (const char*)setting.username, tmp);
			proxy->username[tmp] = 0;

			tmp = MIN(OD_MAX_PROXY_PASSWORD_LEN, setting.password_len);
			od_strncpy(proxy->password, (const char*)setting.password, tmp);
			proxy->password[tmp] = 0;
		}
	}
	else
#endif
	{
		int setting_index = 0;
		int sim_select;
		int tmp = 0;
		MMICONNECTION_LINKSETTING_DETAIL_T* setting_item = PNULL;

		sim_select = _OD_NetGetSelectSIM(context);
		setting_index = _OD_GetNetProfileIndex(context, sim_select);
		setting_item = _OD_GetLinkSettingItemByIndex(sim_select, setting_index);

		*use_proxy = (NULL != setting_item
			&& 0 != od_strlen((const char*)(setting_item->gateway))
			&& 0 != od_strncmp(OD_INVALID_GATEWAY, (const char*)(setting_item->gateway), od_strlen(OD_INVALID_GATEWAY)));

		if(*use_proxy)
		{
			tmp = MIN(OD_MAX_PROXY_IP_LEN, setting_item->gateway_len);
			od_strncpy(proxy->ip, (const char*)setting_item->gateway, tmp);
			proxy->ip[tmp] = 0;

			proxy->port = setting_item->port;

			/* FIXME, how to get proxy username/password. */
			proxy->username[0] = 0;
			proxy->password[0] = 0;
		}
	}
	return OD_SUCCESS;
}

OD_STATUS OD_NetGetActiveSimSlots(OD_Context *context, OD_BOOL sim_slots[])
{
	int counter;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(sim_slots);

	for (counter = 0; counter < OD_SIM_MAX; counter++)
	{
		if (counter >= MMI_DUAL_SYS_MAX)
		{
			sim_slots[counter] = OD_FALSE;
		}
		else
		{
			sim_slots[counter] = MMIPHONE_IsSimAvailable(counter);
		}
	}
	return OD_SUCCESS;
}

OD_STATUS OD_NetActivateBearer(OD_Context *context)
{
	int setting_index = 0;
	int sim_select;
	OD_STATUS ret = OD_ERROR;
	MMIPDP_ACTIVE_INFO_T app_info = {0};
	MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;

	OD_CHECK_ARGS_RETURN_STATUS(context);

	OD_TRACE_LOW("net: OD_NetActivateBearer, net_state = %d, sim = %d",
		context->data.net.net_state , context->data.net.selected_sim);

	if(context->data.net.net_state == OD_NET_STATE_UP)
	{
		return OD_SUCCESS;
	}
	else if(context->data.net.net_state == OD_NET_STATE_CONNECTING)
	{
		return OD_ERROR_PENDING;
	}


#if defined(WIFI_SUPPORT) && !defined(WIFI_SUPPORT_UNISOC_RX)
	if(context->data.net.selected_sim == OD_SIM_INDEX_WIFI)
	{
		app_info.app_handler = context->data.app.module_id;
		app_info.priority = 3;  //All other apps set it to 3!
		app_info.ps_service_type = BROWSER_E;
		app_info.handle_msg_callback = _OD_PDPMessageCallback;
		app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
	}
	else
#endif
	{
		sim_select = _OD_NetGetSelectSIM(context);
		setting_index = _OD_GetNetProfileIndex(context, sim_select);
		setting_item_ptr = _OD_GetLinkSettingItemByIndex(sim_select, setting_index);

		if(setting_item_ptr != PNULL)
		{
			app_info.app_handler = context->data.app.module_id;
			app_info.apn_ptr = (char*)setting_item_ptr->apn;
			app_info.auth_type = setting_item_ptr->auth_type;
			app_info.user_name_ptr = (char*)setting_item_ptr->username;
			app_info.psw_ptr = (char*)setting_item_ptr->password;
			app_info.dual_sys = sim_select;
			app_info.priority = 3;  //All other apps set it to 3!
			app_info.ps_service_type = BROWSER_E;
			app_info.handle_msg_callback = _OD_PDPMessageCallback;
			app_info.ps_interface = MMIPDP_INTERFACE_GPRS;
			MNGPRS_ReadStorageExtReqQosEx(sim_select,MN_GPRS_STORAGE_ALL,BROWSER_E,&app_info.qos_info);
		}
		else
		{
			return OD_ERROR;
		}
	}

	{
		_OD_NetSetState(context, OD_NET_STATE_CONNECTING, OD_DEFAULT_NET_ID, OD_NET_TYPE_NONE);
		if(MMIAPIPDP_Active(&app_info))
		{
			if(context->data.net.net_state == OD_NET_STATE_UP)
			{
				ret = OD_SUCCESS;
			}
			else
			{
				ret = OD_ERROR_PENDING;
			}
		}
		else
		{
			ret = OD_ERROR;
		}
	}
	return ret;
}

OD_STATUS OD_NetDeactivateBearer(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);

	OD_TRACE_LOW("net: OD_NetDeactivateBearer, net_state = %d, sim = %d",
		context->data.net.net_state , context->data.net.selected_sim);

	if(context->data.net.net_state == OD_NET_STATE_DOWN)
	{
		return OD_SUCCESS;
	}
	else if(context->data.net.net_state == OD_NET_STATE_DISCONNECTING)
	{
		return OD_ERROR_PENDING;
	}

	_OD_NetSetState(context, OD_NET_STATE_DISCONNECTING,
		context->data.net.net_id, context->data.net.net_type);
	if(MMIAPIPDP_Deactive(context->data.app.module_id))
	{
		if(context->data.net.net_state == OD_NET_STATE_DOWN)
		{
			return OD_SUCCESS;
		}
		else
		{
			return OD_ERROR_PENDING;
		}
	}
	else
	{
		return OD_ERROR;
	}
}

/* This function will be called in OpDevMain.c */
OD_MMI_TASK void _OD_HandleNetworkPDPMessage(OD_Context* context, MMIPDP_CNF_INFO_T *msg_ptr)
{
	OD_CHECK_ARGS_RETURN_VOID(context);
	OD_CHECK_ARGS_RETURN_VOID(msg_ptr);

	OD_TRACE_LOW("net:_OD_HandleNetworkPDPMessage msg_id = %d, result: %d, \
		net_id = %d, \
		module_id = %d, context = 0x%x",
		msg_ptr->msg_id, msg_ptr->result,
		msg_ptr->nsapi,
		msg_ptr->app_handler, context);

	switch(msg_ptr->msg_id)
	{
	case MMIPDP_ACTIVE_CNF:
		if(MMIPDP_RESULT_SUCC == msg_ptr->result)
		{
			OD_NetType type = OD_NET_TYPE_UNKNOWN;
#ifndef PLATFORM_SC6600L
			type = _OD_NetTranslateType(msg_ptr->ps_interface);
#endif
			_OD_NetSetState(context, OD_NET_STATE_UP, msg_ptr->nsapi, type);
		}
		else /* pdp have retry mechanism, we don't retry again. */
		{
			_OD_NetSetState(context, OD_NET_STATE_DOWN, OD_DEFAULT_NET_ID, OD_NET_TYPE_NONE);
		}
		break;

	case MMIPDP_DEACTIVE_CNF:
		/* We receive this message after we call MMIAPIPDP_Deactive. We don't care what the result(msg_ptr->result) is,
		because if the result is false, the state of PDP is unknown. */
		_OD_NetSetState(context, OD_NET_STATE_DOWN, OD_DEFAULT_NET_ID, OD_NET_TYPE_NONE);
		break;

	case MMIPDP_DEACTIVE_IND:
		/* We receive this message when network is broken from system. */
		_OD_NetSetState(context, OD_NET_STATE_DOWN, OD_DEFAULT_NET_ID, OD_NET_TYPE_NONE);
		break;

	default:
		break;
	}
}

static OD_NetType _OD_NetTranslateType(MMIPDP_INTERFACE_E type)
{
	switch(type)
	{
	case MMIPDP_INTERFACE_GPRS:
		return OD_NET_TYPE_GPRS;
	case MMIPDP_INTERFACE_WIFI:
		return OD_NET_TYPE_WIFI;
	default:
		break;
	}
	return OD_NET_TYPE_NONE;
}

static MN_DUAL_SYS_E _OD_NetGetSelectSIM(const OD_Context *context)
{
	MN_DUAL_SYS_E sim = 0;
	OD_CHECK_ARGS(context, -1);

	sim = (MN_DUAL_SYS_E)context->data.net.selected_sim;
	OD_ASSERT(sim >= MN_DUAL_SYS_1);
	OD_ASSERT(sim < MN_DUAL_SYS_MAX || sim == OD_SIM_INDEX_WIFI); //consider wifi
	return sim;
}

static int _OD_GetNetProfileIndex(const OD_Context *context, int sim)
{
	OD_CHECK_ARGS(context, -1);
	OD_CHECK_ARGS(sim >= 0 && sim < OD_SIM_MAX, -1);
	return context->data.net.profile_index[sim] ;
}

static void _OD_NetSetState(OD_Context *context, OD_NetState state, OD_NetID id, OD_NetType type)
{
	OD_NetState prev_state = context->data.net.net_state;
	OD_NetType prev_type = context->data.net.net_type;

	OD_CHECK_ARGS_RETURN_VOID(context);

	OD_TRACE_LOW("net: _OD_NetSetState: state=%d, type=%d, id=%d; original: state=%d, type=%d, id=%d",
		state, type, id,
		context->data.net.net_state,
		context->data.net.net_type,
		context->data.net.net_id);

	context->data.net.net_type = type;
	context->data.net.net_id = id;
	context->data.net.net_state = state;

	if(state == OD_NET_STATE_UP)
	{
		unsigned int account = 0;
		if(OD_BearerIsWifi(context))
			account = context->data.net.selected_sim<<8;
		else
			account = (context->data.net.selected_sim<<8)|(context->data.net.profile_index[context->data.net.selected_sim] + 1);

		OD_TRACE_LOW("_OD_NetSetState::network account changed from %d to %d", context->data.net.network_account_id, account);

		if(context->data.net.network_account_id != account)
		{
			context->data.net.network_account_id = account;
			context->callback.net.bearer_changed_cb(context, prev_type, type);
		}
	}

	if(prev_state != state)
	{
		OD_TRACE_LOW("_OD_NetSetState::network state changed from %d to %d", (int)prev_state, (int)state);
		if (state != OD_NET_STATE_CONNECTING)
			context->callback.net.link_state_changed(context, prev_state, state);
	}
}

/* called after OD_NetInitMobileContext or OD_NetInitMiniContext */
OD_STATUS OD_NetInit(OD_Context *context)
{
	OD_STATUS ret = OD_SUCCESS;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(context->data.app.module_id > 0);

	context->data.net.network_account_name = NULL;

#if defined(WIFI_SUPPORT) && !defined(WIFI_SUPPORT_UNISOC_RX)
	context->data.net.wifi_enabled = TRUE;
#endif

	/* Set the system default account for each sim. */
	OD_NetResetSimAccount(context);

	return ret;
}

OD_STATUS OD_NetDeinit(OD_Context *context)
{
	OD_STATUS ret = OD_SUCCESS;
	OD_CHECK_ARGS_RETURN_STATUS(context);

	context->data.net.wifi_enabled = FALSE;
	OD_NetDeactivateBearer(context);
	if (context->data.net.network_account_name)
	{
		SCI_FREE(context->data.net.network_account_name);
	}

	return ret;
}

/* Implement account manager */
static OD_STATUS OD_NetResetSimAccount(OD_Context *context)
{
	uint32 i;

	OD_CHECK_ARGS_RETURN_STATUS(context);

	OD_ASSERT(OD_SIM_MAX >= MMI_DUAL_SYS_MAX+1); //For wifi, add 1.
	for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
	{
		context->data.net.profile_index[i] =
#ifdef PLATFORM_SC6600L
			0;
#else
			MMIAPICONNECTION_GetAccountDefaultIndex((MN_DUAL_SYS_E)i, MMICONNECTION_ACCOUNT_BROWSER);
#endif
	}

	return OD_SUCCESS;
}

/* Implement sim selecting window */
static MMI_RESULT_E _OD_HandleSIMSelectWinMsg(
	MMI_WIN_ID_T	win_id,
	MMI_MESSAGE_ID_E	msg_id,
	DPARAM param
	);
static WINDOW_TABLE( OPERA_SIM_SELECTION_WINDOW_TABLE ) =
{
	WIN_TITLE(TXT_SIM_SEL),
	WIN_FUNC((uint32)_OD_HandleSIMSelectWinMsg),
	WIN_ID(OPERA_SIM_SELECTION_WIN_ID),
#ifdef PLATFORM_SC6600L
	CREATE_LISTBOX_CTRL(
	MMI_LISTBOX_LEFT, MMI_CLIENT_RECT_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM,
	GUILIST_RADIOLIST_E, OPERA_SIM_SELECTION_CTRL_ID),
#else
	CREATE_LISTBOX_CTRL(
	GUILIST_RADIOLIST_E, OPERA_SIM_SELECTION_CTRL_ID),
#endif
	WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
	END_WIN
};

/* SIM selection window */
static OD_BOOL OD_NetNeedSelectSim(OD_Context *context)
{
	uint16  sim_ok = 0;
	uint32  sim_num = 0;
	int available_item;

	OD_CHECK_ARGS_RETURN_STATUS(context);

	available_item = context->data.net.wifi_enabled ? 1 : 0;
	sim_num = MMIPHONE_GetSimOkNum(&sim_ok, 1);

	available_item += sim_num;
	return (available_item > 1) ? OD_TRUE : OD_FALSE ;
}

typedef struct _SimWinAddData
{
	OD_BOOL wifi_enabled;
	OD_SimSelectedCallback callback;
} SimWinAddData;

OD_STATUS OD_NetSelectSim(OD_Context *context, OD_SimSelectedCallback callback)
{
	uint16  sim_ok = 0;
	uint32  sim_num = 0;
	int available_item;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(callback);

	available_item = context->data.net.wifi_enabled ? 1 : 0;
	sim_num = MMIPHONE_GetSimOkNum(&sim_ok, 1);

	available_item += sim_num;

	if (available_item > 1)
	{
		MMI_HANDLE_T handle;
		SimWinAddData* win_data = NULL;

        MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
        major_sim_type = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_WAP);
        if ((major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
            && MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1)))
        {
            callback(major_sim_type);
            return OD_SUCCESS;
        }

		win_data = (SimWinAddData*)SCI_ALLOCA(sizeof(SimWinAddData));
		if (!win_data)
		{
			return OD_NO_MEMORY;
		}

		win_data->callback = callback;
		win_data->wifi_enabled = context->data.net.wifi_enabled;

		handle = MMK_CreateWin((uint32*)OPERA_SIM_SELECTION_WINDOW_TABLE, (ADD_DATA)win_data);
		return (MMI_INVALID_ID != handle) ? OD_SUCCESS : OD_ERROR;
	}
	else
	{
		if(context->data.net.wifi_enabled)
		{
			callback(OD_SIM_INDEX_WIFI); //wifi
			return OD_SUCCESS;
		}
		else if(sim_num == 1)
		{
			callback(sim_ok);
			return OD_SUCCESS;
		}
		else
		{
			MMIPHONE_AlertSimNorOKStatus();
			callback(-1);
			return OD_ERROR;
		}
	}
}

#ifdef PLATFORM_SC6600L
static void _OD_CreateGuiItemTextBuffer(GUIITEM_DATA_TEXT_BUFFER_T* text_buffer, wchar* wstr, uint16 wstr_len)
{
	OD_ASSERT(text_buffer);
	OD_ASSERT(wstr);
	text_buffer->wstr_len = MIN(wstr_len, sizeof(text_buffer->wstr)/sizeof(wchar));
	od_wcsncpy(text_buffer->wstr, wstr, text_buffer->wstr_len);
}
#endif

static BOOLEAN _OD_AppendSimListItem(MMI_CTRL_ID_T ctrl_id, BOOLEAN wifi_enabled)
{
	int32 sim_index = 0;
	MMI_STRING_T sim_name = {0};
	MMI_IMAGE_ID_T img_id = 0;
	GUILIST_ITEM_T item = {0};
	GUILIST_ITEM_DATA_T item_data = {0};

#ifndef WIFI_SUPPORT
	/* If WIFI_SUPPORT not defined, force reset wifi_enabled to FALSE.
	Don't need to check both WIFI_SUPPORT and wifi_enabled again */
	wifi_enabled = FALSE;
#endif

#if defined(WIFI_SUPPORT) && defined(WIFI_SUPPORT_UNISOC_RX)
    /* If WIFI_SUPPORT not defined, force reset wifi_enabled to FALSE.
        don't need to check both WIFI_SUPPORT and wifi_enabled again */
        wifi_enabled = FALSE;
#endif

	/* Initialize some shared setting in item */
	item.item_data_ptr = &item_data;

#ifdef OD_7701_SUPPORT
	item_data.softkey_id[0] = STXT_SOFTKEY_MENU_MK;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
#else
	item_data.softkey_id[0] = STXT_SOFTKEY_OK_MK;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = STXT_RETURN;
#endif /* OD_7701_SUPPORT */


	/* consider wifi, then max item number + 1 */
	GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX + 1, FALSE);

	for(sim_index = 0; sim_index < MMI_DUAL_SYS_MAX; sim_index++)
	{
		wchar operator_str_buffer[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
		MMI_STRING_T operator_str = {0};
		operator_str.wstr_ptr = operator_str_buffer;

		if(!MMIPHONE_GetSimExistedStatus(sim_index) || !MMIPHONE_IsSimOk(sim_index))
		{
			continue;
		}

#ifdef MMI_MULTI_SIM_SYS_SINGLE
		img_id = IMAGE_IDLE_PROMPT_SIM;
#else
		img_id = IMAGE_IDLE_PROMPT_SIM1 + (sim_index - MN_DUAL_SYS_1);
#endif

		item.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
		item.user_data = sim_index;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
		{
			MMISET_SIM_NAME_T sim_name_t = MMIAPISET_GetSimName(sim_index);
			sim_name.wstr_len = sim_name_t.wstr_len;
			sim_name.wstr_ptr = sim_name_t.wstr_arr;
		}
#else
		MMI_GetLabelTextByLang(TXT_GPRS, &sim_name);
#endif
		od_memset(&item_data.item_content, 0, sizeof(item_data.item_content));

		item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
		item_data.item_content[0].item_data.image_id = img_id;

		item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
#ifdef PLATFORM_SC6600L
		_OD_CreateGuiItemTextBuffer(&item_data.item_content[1].item_data.text_buffer, sim_name.wstr_ptr, sim_name.wstr_len);
#else
		item_data.item_content[1].item_data.text_buffer = sim_name;
#endif

		MMIAPISET_GetSimServiceString(sim_index, &operator_str);

		item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
#ifdef PLATFORM_SC6600L
		_OD_CreateGuiItemTextBuffer(&item_data.item_content[2].item_data.text_buffer, operator_str.wstr_ptr, operator_str.wstr_len);
#else
		item_data.item_content[2].item_data.text_buffer = operator_str;
#endif

		GUILIST_AppendItem(ctrl_id, &item);
	}

#if defined(WIFI_SUPPORT) && !defined(WIFI_SUPPORT_UNISOC_RX)
	if (wifi_enabled)
	{
		MMI_STRING_T item_str = {0};
		int item_content_index = 0;
#ifdef OD_HAVE_WIFI_ICON
		item.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
#else
		item.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
#endif
		item.user_data = OD_SIM_INDEX_WIFI;
		MMI_GetLabelTextByLang(TXT_WIFI, &item_str);

		od_memset(item_data.item_content, 0, sizeof(item_data.item_content));

#ifdef OD_HAVE_WIFI_ICON
		item_data.item_content[item_content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
		item_data.item_content[item_content_index].item_data.image_id = MAIN_GetIdleWifiIcon();
		++ item_content_index;
#endif

		item_data.item_content[item_content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[item_content_index].item_data.text_buffer.wstr_len = item_str.wstr_len;
		item_data.item_content[item_content_index].item_data.text_buffer.wstr_ptr = item_str.wstr_ptr;

		GUILIST_AppendItem(ctrl_id, &item);
	}
#endif

	GUILIST_SetOwnSofterKey(ctrl_id, TRUE);

	return TRUE;
}

static MMI_RESULT_E _OD_HandleSIMSelectWinMsg(
	MMI_WIN_ID_T	win_id,
	MMI_MESSAGE_ID_E	msg_id,
	DPARAM param
	)
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	uint16          list_index = 0;
	uint32          item_data = 0;
	OD_SimSelectedCallback sim_selected_callback = NULL;
	SimWinAddData*   win_data = NULL;

	OD_CHECK_ARGS(win_id, MMI_RESULT_FALSE);

	win_data = (SimWinAddData*)MMK_GetWinAddDataPtr((MMI_HANDLE_T)win_id);

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		OD_ASSERT(win_data);
		_OD_AppendSimListItem(OPERA_SIM_SELECTION_CTRL_ID, win_data->wifi_enabled);

#if defined(WIFI_SUPPORT) && !defined(WIFI_SUPPORT_UNISOC_RX)
		GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_NETWORK_SELECT,FALSE);
#else
		GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_SIM_SEL,FALSE);
#endif

#ifdef OD_7701_SUPPORT
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MENU_MK, LEFT_BUTTON, FALSE);
#endif /* OD_7701_SUPPORT */

		MMK_SetAtvCtrl(win_id, OPERA_SIM_SELECTION_CTRL_ID);
		break;

	case MSG_CTL_PENOK:
	case MSG_APP_WEB:
	case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_CTL_MIDSK:
		OD_ASSERT(win_data);
		list_index = GUILIST_GetCurItemIndex(OPERA_SIM_SELECTION_CTRL_ID);

		if(GUILIST_IsCurrItemInvalid(OPERA_SIM_SELECTION_CTRL_ID))
		{
			break;
		}

#if defined(OD_6530_12A) || defined(OD_6530_12B) || defined(OD_6530_12C) || defined(OD_8501_MAIN)
		GUILIST_GetItemData(OPERA_SIM_SELECTION_CTRL_ID, list_index, &item_data);
#else
		GUILIST_GetItemDataByCtrlID(OPERA_SIM_SELECTION_CTRL_ID, list_index, &item_data);
#endif /* OD_6530_12A || OD_6530_12B || OD_6530_12C || OD_8501_MAIN */

		sim_selected_callback = win_data->callback;
		/* win_data will be freed when receive MSG_CLOSE_WINDOW, during call following function */
		MMK_CloseWin(win_id);

		if (sim_selected_callback)
		{
			sim_selected_callback(item_data);
		}
		break;

	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		OD_ASSERT(win_data);
		sim_selected_callback = win_data->callback;
		/* win_data will be freed when receive MSG_CLOSE_WINDOW, during call following function */
		MMK_CloseWin(win_id);

		if(sim_selected_callback)
		{
			//Selected nothing.
			sim_selected_callback(-1);
		}
		break;
	case MSG_CLOSE_WINDOW:
		MMK_FreeWinAddData(win_id);
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}

	return result;
}

//Apn selection
static MMI_RESULT_E _OD_HandleApnSelectWinMsg(
	MMI_WIN_ID_T	win_id,
	MMI_MESSAGE_ID_E	msg_id,
	DPARAM param
	);
//the window for connect setting list
static WINDOW_TABLE(OPERA_APN_SELECTION_WIN_TAB) =
{
	WIN_TITLE(STXT_SELECT),
	WIN_FUNC((uint32)_OD_HandleApnSelectWinMsg),
	WIN_ID(OPERA_APN_SELECTION_WIN_ID),
	WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, STXT_RETURN),
#ifdef PLATFORM_SC6600L
	CREATE_LISTBOX_CTRL(
	MMI_LISTBOX_LEFT, MMI_CLIENT_RECT_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM,
	GUILIST_RADIOLIST_E, OPERA_APN_SELECTION_CTRL_ID),
#else
	CREATE_LISTBOX_CTRL(
	GUILIST_RADIOLIST_E, OPERA_APN_SELECTION_CTRL_ID),
#endif
	END_WIN
};

typedef struct _ApnWinAddData
{
	int sim;
	int default_apn_index;
	OD_ApnSelectedCallback callback;
	OD_Context *context;
} ApnWinAddData;

OD_STATUS OD_NetSelectApn(OD_Context *context, int sim, OD_ApnSelectedCallback callback)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(callback);
	OD_CHECK_ARGS_RETURN_STATUS(sim >= 0 && sim < OD_SIM_MAX);

	if(OD_BearerIsWifi(context))
	{
		_OD_NetUpdateProfile(context, sim, 0);
		callback(0);
		return OD_SUCCESS;
	}else
	{
		MMI_HANDLE_T handle;
		ApnWinAddData *data = NULL;
		uint32 num = 0;

		uint8 apn_index = _OD_GetAccountDefaultIndex(sim);
		if (apn_index >= 0)
		{
			callback(apn_index);
			return OD_SUCCESS;
		}

		num = _OD_GetLinkSettingNum(sim);
		if (num == 0 )
		{
			/* NO Apn for this sim */
			callback(-1);
			return OD_ERROR;
		}
		else if (num == 1)
		{
			/* Only one APN for this sim, select it and return */
			callback(0);
			return OD_SUCCESS;
		}

		data = (ApnWinAddData*)SCI_ALLOCA(sizeof(ApnWinAddData));
		if(!data)
		{
			return OD_NO_MEMORY;
		}

		data->sim = sim;
		data->default_apn_index = context->data.net.profile_index[sim];
		data->callback = callback;
		data->context = context;
		handle = MMK_CreateWin((uint32*)OPERA_APN_SELECTION_WIN_TAB, (ADD_DATA)data);
		return (MMI_INVALID_ID != handle) ? OD_SUCCESS : OD_ERROR;
	}
}

static void AppendOneLineTextListItem(
									  MMI_CTRL_ID_T       ctrl_id,
									  const wchar*        wstr_ptr,
									  uint16              wstr_len,
									  uint32              userData
									  )
{
	GUILIST_ITEM_T      item_t    = {0}; /*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0}; /*lint !e64*/

	OD_CHECK_ARGS_RETURN_VOID(ctrl_id);
	OD_CHECK_ARGS_RETURN_VOID(wstr_ptr);

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;

	item_t.item_data_ptr = &item_data;
	item_t.user_data = userData;

	item_data.softkey_id[0] = STXT_SOFTKEY_OK_MK;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = STXT_RETURN;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[0].item_data.text_buffer.wstr_len = wstr_len;

#ifdef PLATFORM_SC6600L
	{
		uint32 buf_size = sizeof(item_data.item_content[0].item_data.text_buffer.wstr)/sizeof(wchar);
		od_wcsncpy(item_data.item_content[0].item_data.text_buffer.wstr, wstr_ptr, buf_size - 1 );
		item_data.item_content[0].item_data.text_buffer.wstr[buf_size - 1 ] = 0;
	}
#else
	item_data.item_content[0].item_data.text_buffer.wstr_ptr = (wchar*)wstr_ptr; /*lint !e605*/
#endif

	GUILIST_AppendItem( ctrl_id, &item_t );
}

static BOOLEAN _OD_AppendApnListItem(int sim, int selected_item_index, MMI_CTRL_ID_T ctrl_id)
{
	uint8 i = 0;
	uint8 num = 0;
	MMICONNECTION_LINKSETTING_DETAIL_T * setting;

	OD_CHECK_ARGS(ctrl_id, FALSE);

	num = _OD_GetLinkSettingNum(sim);
	GUILIST_SetMaxItem( ctrl_id, num, FALSE );//max item 20
	GUILIST_SetMaxSelectedItem(ctrl_id, 1);

	for(i = 0; i < num; i++)
	{
		setting = _OD_GetLinkSettingItemByIndex(sim, i);
		AppendOneLineTextListItem(ctrl_id,
			setting->name,
			setting->name_len,
			i
			);
	}
	GUILIST_SetCurItemIndex(ctrl_id, selected_item_index);
	GUILIST_SetOwnSofterKey(ctrl_id, TRUE);

	return TRUE;
}

static OD_STATUS _OD_NetUpdateProfile(OD_Context *context, int sim, int apn_index)
{
	OD_STATUS status = OD_SUCCESS;
	unsigned char* account_name = NULL;
	int len;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(sim >= 0 && sim < OD_SIM_MAX);

	if(OD_BearerIsWifi(context))
	{
		MMI_STRING_T sim_str = {0};
		MMI_GetLabelTextByLang(TXT_WIFI,&sim_str);
		account_name = (unsigned char*)SCI_ALLOCA(sim_str.wstr_len*4 + 1);
		if(account_name)
		{
			len = GUI_WstrToUTF8(account_name, sim_str.wstr_len*4, sim_str.wstr_ptr, sim_str.wstr_len);
			account_name[len] = 0;
		}else
		{
			status = OD_NO_MEMORY;
		}
	}else
	{
		MMICONNECTION_LINKSETTING_DETAIL_T * setting;

		setting = _OD_GetLinkSettingItemByIndex(sim, apn_index);
		if(setting)
		{
			/** Is it enough? */
			account_name = (unsigned char*)SCI_ALLOCA(setting->name_len*4 + 1);
			if(account_name)
			{
				len = GUI_WstrToUTF8(account_name, setting->name_len*4, setting->name, setting->name_len);
				account_name[len] = 0;
			}else
			{
				status = OD_NO_MEMORY;
			}
		}else
		{
			status = OD_OUT_OF_RANGE;
		}
	}

	if(status == OD_SUCCESS)
	{
		SCI_FREE(context->data.net.network_account_name);
		context->data.net.network_account_name = account_name;

		context->data.net.selected_sim = sim;
		context->data.net.profile_index[sim] = apn_index;
	}

	return status;
}


static MMI_RESULT_E _OD_HandleApnSelectWinMsg(
	MMI_WIN_ID_T	win_id,
	MMI_MESSAGE_ID_E	msg_id,
	DPARAM param
	)
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	uint16          list_index = 0;
	uint32          item_data = 0;
	OD_ApnSelectedCallback  apn_selected_callback = NULL;
	ApnWinAddData *win_data = NULL;

	OD_CHECK_ARGS(win_id, MMI_RESULT_FALSE);

	win_data = (ApnWinAddData*)MMK_GetWinAddDataPtr((MMI_HANDLE_T)win_id);

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		OD_ASSERT(win_data);
		_OD_AppendApnListItem(win_data->sim, win_data->default_apn_index, OPERA_APN_SELECTION_CTRL_ID);

		MMK_SetAtvCtrl(win_id, OPERA_APN_SELECTION_CTRL_ID);
		break;

	case MSG_CTL_PENOK:
	case MSG_APP_WEB:
	case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_CTL_MIDSK:
		OD_ASSERT(win_data);
		list_index = GUILIST_GetCurItemIndex(OPERA_APN_SELECTION_CTRL_ID);

		if(GUILIST_IsCurrItemInvalid(OPERA_APN_SELECTION_CTRL_ID))
		{
			break;
		}
#if defined(OD_6530_12A) || defined(OD_6530_12B) || defined(OD_6530_12C) || defined(OD_8501_MAIN)
		GUILIST_GetItemData(OPERA_APN_SELECTION_CTRL_ID, list_index, &item_data);
#else
		GUILIST_GetItemDataByCtrlID(OPERA_APN_SELECTION_CTRL_ID, list_index, &item_data);
#endif /* OD_6530_12A || OD_6530_12B || OD_6530_12C || OD_8501_MAIN */
		{
			OD_STATUS update_profile_result = _OD_NetUpdateProfile(win_data->context, win_data->sim, list_index);
			OD_Context *context = win_data->context;

			apn_selected_callback = win_data->callback;

			// Make sure the main window get focus after window switch, since some other app(e.g. SMS)
			// may grab focus in process of selecting APN. See also HULA-730.
			if (context && MMK_IsOpenWin(context->data.app.win_id))
			{
				MMK_WinGrabFocus(context->data.app.win_id);
			}

			/* data will be freed when receive MSG_CLOSE_WINDOW, during call following function */
			MMK_CloseWin(win_id);

			if(apn_selected_callback)
			{
				if(update_profile_result == OD_SUCCESS)
				{
					apn_selected_callback(item_data);
				}else
				{
					apn_selected_callback(-1);
				}
			}
		}
		break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		OD_ASSERT(win_data);
		apn_selected_callback = win_data->callback;
		/* data will be freed when receive MSG_CLOSE_WINDOW, during call following function */
		MMK_CloseWin(win_id);

		if(apn_selected_callback)
		{
			//Selected nothing.
			apn_selected_callback(-1);
		}
		break;
	case MSG_CLOSE_WINDOW:
		MMK_FreeWinAddData(win_id);
		break;
	default:
		result = MMI_RESULT_FALSE;
		break;
	}

	return result;
}

OD_BOOL OD_BearerIsWifi(OD_Context *context)
{
	OD_CHECK_ARGS(context, OD_FALSE);
	OD_CHECK_ARGS(IsValidContext(context), OD_FALSE);

	return context->data.net.selected_sim == OD_SIM_INDEX_WIFI;
}

MMICONNECTION_LINKSETTING_DETAIL_T* _OD_GetLinkSettingItemByIndex(MN_DUAL_SYS_E dual_sys, uint8 index)
{
#ifdef OD_NEW_PLATFORM_INTERFACE
#	ifdef NETWORK_ACCOUNT_ADAPTIVE_SUPPORT
		return MMICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
#	else
		return MMICONNECTION_GetLinkSettingItemByIndex(index);
#	endif
#else
	return MMICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
#endif
}

uint8 _OD_GetAccountDefaultIndex(MN_DUAL_SYS_E dual_sys)
{
	return MMIAPICONNECTION_GetAccountDefaultIndex(dual_sys, MMICONNECTION_ACCOUNT_BROWSER);
}

uint8 _OD_GetLinkSettingNum(MN_DUAL_SYS_E dual_sys)
{
#ifdef OD_NEW_PLATFORM_INTERFACE
#	ifdef NETWORK_ACCOUNT_ADAPTIVE_SUPPORT
		return MMICONNECTION_GetLinkSettingNum(dual_sys);
#	else
		return MMICONNECTION_GetLinkSettingNum();
#	endif
#else
	return MMICONNECTION_GetLinkSettingNum(dual_sys);
#endif
}

OD_STATUS OD_NetCloseWin(void)
{
	if(MMK_IsOpenWin((MMI_HANDLE_T)OPERA_SIM_SELECTION_WIN_ID))
		MMK_CloseWin((MMI_HANDLE_T)OPERA_SIM_SELECTION_WIN_ID);

	if(MMK_IsOpenWin((MMI_HANDLE_T)OPERA_APN_SELECTION_WIN_ID))
		MMK_CloseWin((MMI_HANDLE_T)OPERA_APN_SELECTION_WIN_ID);

	return OD_SUCCESS;
}
