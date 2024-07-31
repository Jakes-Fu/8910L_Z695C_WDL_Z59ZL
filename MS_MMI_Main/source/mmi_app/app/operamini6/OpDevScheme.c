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
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif

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
#ifdef DL_SUPPORT
            {
                MMIDL_DOWNLOAD_INFO_T download_info = {0};
                char *temp_url1 = PNULL;
                char *temp_url2 = PNULL;
                const char * opera_mark1 = "opera-mini.net";
                const char * opera_mark2 = "&url=";

                temp_url1 = strstr(uri, opera_mark1);

                if (PNULL != temp_url1)
                {
                    temp_url2 = strstr(temp_url1, opera_mark2);

                    if (PNULL != temp_url2)
                    {
                        temp_url2 += strlen(opera_mark2);

                        if (strnicmp(temp_url2, OD_HTTP_SCHEME, strlen(OD_HTTP_SCHEME)) != 0 && strnicmp(temp_url2, OD_HTTPS_SCHEME, strlen(OD_HTTPS_SCHEME)) != 0)
                        {
                            if (MMIAPIBROWSER_HandleExScheme((uint8*)temp_url2))
                            {
                                return OD_SUCCESS;
                            }
                            else
                            {
                                return OD_ERROR;
                            }
                        }
                    }
                }

                download_info.mime_type = 0;//_OD_DownloadConvertMIMETypeFromString(mimetype);
                //download_info.http_header = PNULL;
                download_info.url = (uint8 *)uri;
                //download_info.is_chunk = FALSE;
                //download_info.status_code = 200;
                //download_info.method = 0;
                download_info.buf = PNULL;
                //download_info.file_name = PNULL;

                if (PNULL != context && context->data.net.selected_sim == OD_SIM_INDEX_WIFI)
                {
                    download_info.bearer_type = MMIPDP_INTERFACE_WIFI;
                }
                else
                {
                    download_info.bearer_type = MMIPDP_INTERFACE_GPRS;
                }
                download_info.dual_sys = context->data.net.selected_sim;

                MMIAPIDL_StartDownload(MMIDL_CREATE_NEW, &download_info);

                return OD_SUCCESS;

            }
#else//DL_SUPPORT
            /*  No other browser found, just return error. */
            return OD_ERROR;
#endif

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
		OD_TRACE_LOW("OD_SchemeHandle: MMIAPIBROWSER_Entry");
		MMIAPIBROWSER_Entry(&browser_entry);
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
