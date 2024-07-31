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

static OD_BOOL _OD_SchemeUriHasScheme(const char *uri, const char *scheme);

OD_BOOL _OD_SchemeUriHasScheme(const char *uri, const char *scheme)
{
	unsigned int scheme_len;

	OD_ASSERT(uri);
	OD_ASSERT(scheme);

	scheme_len = od_strlen(scheme);

	return scheme_len > 0 && od_strnicmp(uri, scheme, scheme_len) == 0 && uri[scheme_len] != 0;
}

OD_STATUS OD_SchemeInit(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);

	context->data.scheme.initialized = OD_TRUE;

	return OD_SUCCESS;
}

OD_STATUS OD_SchemeDeinit(OD_Context *context)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);

	context->data.scheme.initialized = OD_FALSE;

	return OD_SUCCESS;
}

/* Implementation provided by customer, begin. */
OD_STATUS OD_SchemeHandle(OD_Context* context, const char * uri)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(uri);

	/* There's no API having parameter for confirming on Spreadtrum. */

	OD_TRACE_LOW("OD_SchemeHandle: uri = %s, confirm = %d", uri, FALSE);

#define OD_HTTP_SCHEME "http://"
#define OD_HTTPS_SCHEME "https://"

	if (_OD_SchemeUriHasScheme(uri, "urn.msa:"))
	{
		char *token = uri + od_strlen("urn.msa:");
		int token_size = od_strlen(token);

		OD_Trace(OD_TRACE_LEVEL_LOW, "Token size: %d", token_size);
		//In sign-off operation, "token" string indicates the status,like:signoff_success or signoff_failure.
		if (od_strstr(token, "signoff_") == token) //for sign off operation
		{
			if (od_strstr(token, "success") == token + od_strlen("signoff_"))
				;//TODO: call platform callback after it's ready in MRE sdk;
				//msa_callback(MSA_OK, NULL , 0);
			else
				;//TODO: call platform callback after it's ready in MRE sdk;
				//msa_callback(MSA_FAILED, NULL , 0);
		}
		else //For sign-in operation.
		{
		if (token_size == 0)
		{
			token = NULL;
			//TODO: call platform callback after it's ready in MRE sdk;
			//msa_callback(MSA_FAILED, token , token_size);
		}
		else
			;//TODO: call platform callback after it's ready in MRE sdk;
			//msa_callback(MSA_OK, token , token_size);
	}
}

#ifdef OD_HAVE_SCHEME_API
	if(strnicmp(uri, OD_HTTP_SCHEME, strlen(OD_HTTP_SCHEME)) == 0 || strnicmp(uri, OD_HTTPS_SCHEME, strlen(OD_HTTPS_SCHEME)) == 0)
	{
		MMIBROWSER_ENTRY_PARAM browser_entry = {0};

		//close opera
#if defined(BROWSER_SUPPORT_DORADO)
		browser_entry.browser_type = MMIBROWSER_TYPE_DORADO;	//dorado,opear mini,NF......
#elif defined(BROWSER_SUPPORT_NETFRONT)
		browser_entry.browser_type = MMIBROWSER_TYPE_NF;	//dorado,opear mini,NF......
#else
		/*  No other browser found, just return error. */
		return OD_ERROR;
#endif
		browser_entry.is_browser_type_customized = TRUE;//used customized browser_type;TRUE, use browser_type
		browser_entry.type = MMIBROWSER_ACCESS_URL;	//mainmenu, homepage, url
		browser_entry.is_use_wifi = (context->data.net.selected_sim == OD_SIM_INDEX_WIFI);
		browser_entry.dual_sys = context->data.net.selected_sim;//sim:dual_sys
		browser_entry.url_ptr = CFL_StrCloneEx(uri, 0, __FILE__, __LINE__);
		if (browser_entry.url_ptr == 0)
		{
			OD_TRACE_LOW("OD_SchemeHandle: browser_entry.url_ptr == 0");
			return OD_ERROR;
		}
		//char *url_title_ptr;//only use for MMIBROWSER_ADD_BOOKMARK
		// char* user_agent_ptr;
		browser_entry.is_connection_customized = TRUE;//used customized connection_index(only netfront now)
		browser_entry.connection_index = context->data.net.profile_index[context->data.net.selected_sim];//(only netfront now)
#ifndef BROWSER_ALL_RUNNING_SUPPORT
#ifdef BROWSER_SUPPORT_DORADO

		OD_TRACE_LOW("OD_SchemeHandle: post MSG_MMI_OPERA_ASYNC_EXIT");
		MMK_PostMsg(VIRTUAL_WIN_ID, MSG_MMI_OPERA_ASYNC_EXIT, &browser_entry, sizeof(browser_entry));

		return OD_SUCCESS;
#endif

#else
		OD_TRACE_LOW("OD_SchemeHandle: MMIBROWSER_Entry");
		MMIBROWSER_Entry(&browser_entry);
		SCI_FREE(browser_entry.url_ptr);
		return OD_SUCCESS;
#endif
		OD_TRACE_LOW("OD_SchemeHandle: OD_NOT_SUPPORTED");
		return OD_NOT_SUPPORTED;
	}
	else
	{
		if(MMIAPIBROWSER_HandleExScheme((uint8*)uri))
		{
			return OD_SUCCESS;
		}
	}

	return OD_ERROR;
#endif /* OD_HAVE_SCHEME_API */
	return OD_NOT_SUPPORTED;
}
/* Implementation provided by customer, end. */
