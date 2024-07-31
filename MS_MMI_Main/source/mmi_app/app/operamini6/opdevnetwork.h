/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#ifndef OPDEV_NETWORK_H
#define OPDEV_NETWORK_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Max number of sim cards supported by opera. */
#define OD_SIM_MAX			(16)
/* We treat WIFI as a special SIM card, it's index is OD_SIM_INDEX_WIFI. */
#define OD_SIM_INDEX_WIFI	(OD_SIM_MAX - 1)

typedef enum _OD_NetState
{
	OD_NET_STATE_NONE,
	OD_NET_STATE_DOWN,
	OD_NET_STATE_UP,
	OD_NET_STATE_CONNECTING,
	OD_NET_STATE_DISCONNECTING,

	OD_NET_STATE_UNKNOWN,
	OD_NET_STATE_MAX
} OD_NetState;

typedef enum _OD_NetType
{
	OD_NET_TYPE_NONE,
	OD_NET_TYPE_CSD,
	OD_NET_TYPE_GPRS,
	OD_NET_TYPE_WIFI,

	OD_NET_TYPE_UNKNOWN,
	OD_NET_TYPE_MAX
} OD_NetType;

/**
* Apn selected callback
* @param apn_index: the apn index, from 0, <0 means user cancel selecting.
*/
typedef OD_MMI_TASK void (*OD_ApnSelectedCallback)(int apn_index);

/**
* Net state changed callback
* @param context the opera context
* @param previous previous net state
* @param current current net state
*/
typedef OD_MMI_TASK void (*OD_NetLinkStateChangedCallback)(OD_Context *context, OD_NetState previous, OD_NetState current);

/**
* Net type changed callback
* @param context the opera context
* @param previous previous net type
* @param current current net type
*/
typedef OD_MMI_TASK void (*OD_NetBearerChangedCallback)(OD_Context *context, OD_NetType previous, OD_NetType current);

/* max length of proxy IP, tailing zero not included */
#define OD_MAX_PROXY_IP_LEN			(15)
/* max length of username, tailing zero not included */
#define OD_MAX_PROXY_USERNAME_LEN	(31)
/* max length of password, tailing zero not included */
#define OD_MAX_PROXY_PASSWORD_LEN	(31)

/* Proxy structure used by opera. */
typedef struct _OD_Proxy
{
	/* IP, eg. "10.0.0.172" */
	char ip[OD_MAX_PROXY_IP_LEN + 1];
	/* port, host order */
	unsigned short port;
	/* username and password, can be "" means no authentication. */
	char username[OD_MAX_PROXY_USERNAME_LEN + 1];
	char password[OD_MAX_PROXY_PASSWORD_LEN + 1];
} OD_Proxy;

typedef struct _OD_NetContext
{
	/* network account(eg. CMWAP, CMNET) index for each sim */
	unsigned char profile_index[OD_SIM_MAX];
	/* currently selected SIM card index */
	int selected_sim;
	/* current net state */
	OD_NetState net_state;
	/* current net type */
	OD_NetType net_type;
	/* current net id, used in soc_create/soc_gethostbyname */
	OD_NetID net_id;
	/* if WIFI is enabled for this session, for MMIBROWSER_Entry API. */
	OD_BOOL wifi_enabled;

	/* internal global values for some platform, not used in some platforms now
	need to refactor later. */
	unsigned int cbm_application_id;
	OD_BOOL waiting_for_account_id;
	OD_NetType last_seen_bearer;
	unsigned int last_seen_account_id;
	OD_BOOL bearer_held;
	unsigned int network_account_id;
	char *network_account_name;
	OD_BOOL network_valid;
	OD_BOOL bearer_opened;
	OD_NetState last_seen_link_state;
	OD_BOOL use_proxy;
	OD_Proxy proxy_settings;

} OD_NetContext;

typedef struct _OD_NetCallbacks
{
	/* Net type changed callback, see #OD_NetBearerChangedCallback */
	OD_NetBearerChangedCallback bearer_changed_cb;
	/* Net state changed callback, see #OD_NetLinkStateChangedCallback */
	OD_NetLinkStateChangedCallback link_state_changed;

	/* APN selected callback. Not used in PNGLAI now. In PNGLAI, the callback will be
	stored in add data of window. */
	OD_ApnSelectedCallback apn_selected;
} OD_NetCallbacks;

/*
* Get current net ID
* @param context the opera context
* @return net ID, there is no invalid net ID, see definition of OD_DEFAULT_NET_ID
*/
OD_ALL_TASK OD_NetID OD_NetGetID(const OD_Context *context);

/*
* Get current net proxy.
* @param context the opera context
* @param[out] use_proxy a pointer to the value indicating if we use proxy
* @param[out] proxy a pointer to the proxy structure
* @return OD_SUCCESS if succeed, other OD_ERROR
*/
OD_ALL_TASK OD_STATUS OD_NetGetProxy(const OD_Context *context, OD_BOOL *use_proxy, OD_Proxy *proxy);

/*
* Init opdev network system
* @param context the opera context
* @return OD_SUCCESS if succeed, other OD_ERROR
*/
OD_MMI_TASK OD_STATUS OD_NetInit(OD_Context *context);

/*
* Deinit opdev network system
* @param context the opera context
* @return OD_SUCCESS if succeed, other OD_ERROR
*/
OD_MMI_TASK OD_STATUS OD_NetDeinit(OD_Context *context);

/*
* Activate bearer
* @param context the opera context
* @return
*  OD_SUCCESS: activate success. Now network state is up. If network is alreally up before this function,
*   callback won't be called. Otherwise, callback should be called during the function.
*  OD_ERROR_PENDING: in activating. The callback will be called when activated.
*  OD_ERROR: it refused to activate.
* @warning:
*   During this function, context->bearer_changed_cb() may be called
*/
OD_MMI_TASK OD_STATUS OD_NetActivateBearer(OD_Context *context);

/*
* Deactivate bearer
* @param context the opera context
* @return OD_SUCCESS if succeed
*  OD_ERROR_PENDING: in deactivating
*  OD_ERROR: it refused to activate
*/
OD_MMI_TASK OD_STATUS OD_NetDeactivateBearer(OD_Context *context);

/*
* Get sim status
* @param context the opera context
* @param[out] sim_slots a pointer to the array containing sim status. If sim_slots[i] is
*  TRUE, the sim i is active, otherwise inactive.
*  sim_slots length is OD_SIM_MAX
* @return OD_SUCCESS if succeed, otherwise OD_ERROR
*/
OD_MMI_TASK OD_STATUS OD_NetGetActiveSimSlots(OD_Context *context, OD_BOOL sim_slots[]);

/**
* Sim selected callback.
*  @param sim: the sim index from 0,  WIFI's index is OD_SIM_INDEX_WIFI,
*               <0 means user cancel selecting.
*/
typedef void (OD_MMI_TASK *OD_SimSelectedCallback)(int sim);

/**
* Select a sim card to connect network. It will check WIFI and SIM status.
* If only one choice, select it. If not, show sim selection UI to let user select.
*
* @param context the opera context
* @param callback the callback called when sim is selected
* @return OD_SUCCESS if succeed, otherwise OD_ERROR
*/
OD_MMI_TASK OD_STATUS OD_NetSelectSim(OD_Context *context, OD_SimSelectedCallback callback);

/**
* Select a sim card to connect network. It will check WIFI and SIM status.
* If only one choice, select it. If not, show sim selection UI to let user select.
*
* @param context the opera context
* @param callback the callback called when sim is selected
* @return OD_SUCCESS if succeed, otherwise OD_ERROR
*/
OD_MMI_TASK OD_STATUS OD_NetSelectApn(OD_Context *context, int sim, OD_ApnSelectedCallback callback);

/**
* Check if the current bearer is wifi.
*
* @param context the opera context
* @return OD_TRUE if current bearer is wifi, OD_FALSE if not
* @note:
*   Return OD_TRUE does NOT mean the network is connected.
*/
OD_MMI_TASK OD_BOOL OD_BearerIsWifi(OD_Context *context);

/**
* Close SIM card select window and APN select window related to network.
*/
OD_MMI_TASK OD_STATUS OD_NetCloseWin(void);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* OPDEV_NETWORK_H */
