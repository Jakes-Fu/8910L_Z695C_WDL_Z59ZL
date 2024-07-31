/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#include "opcfg.h"
#include "opdevinternal.h"
#include "opdevdownload.h"
#include "opimport.h"

#ifdef DL_SUPPORT
#include "mmipdp_export.h"
#include "mmidl_export.h"
#include "mime_type.h"
#endif



static MMIBROWSER_MIME_TYPE_E _OD_DownloadConvertMIMETypeFromString(const char* type)
{
	MMIBROWSER_MIME_TYPE_E mime = MMIBROWSER_MIME_TYPE_NONE;

	OD_ASSERT(type);

#define CHCKMIME(X) (od_stricmp(X, type) == 0)

	if(CHCKMIME("text/vnd.sun.j2me.app-descriptor"))
	{
		mime = MMIBROWSER_MIME_TYPE_JAD;
	}else if( CHCKMIME("application/x-java-archive") || CHCKMIME("application/java-archive") )
	{
		mime = MMIBROWSER_MIME_TYPE_JAR;
		//FIXME: It seems that CORE don't send out this type string.
	}else if( CHCKMIME("application/vnd.oma.dd+xml") )
	{
		 //type = MMIBROWSER_MIME_TYPE_DD;
	}else if( CHCKMIME("application/sdp") ||  CHCKMIME("application/x-sdp"))
	{
		mime = MMIBROWSER_MIME_TYPE_SDP;
	}
#ifdef DRM_SUPPORT
	else if( CHCKMIME("application/vnd.oma.drm.message") )
	{
		mime = MMIBROWSER_MIME_TYPE_DM;
	}else if( CHCKMIME("application/vnd.oma.drm.rights+xml") )
	{
		mime = MMIBROWSER_MIME_TYPE_DR;
	}else if( CHCKMIME("application/vnd.oma.content") )
	{
		mime = MMIBROWSER_MIME_TYPE_DRC;
	}
#endif

#undef CHCKMIME

	return mime;
}


OD_BOOL OD_DownloadIsMimeTypeSupport(const char* type)
{
	MMIBROWSER_MIME_TYPE_E mime = _OD_DownloadConvertMIMETypeFromString(type);
	return (MMIBROWSER_MIME_TYPE_NONE != mime ? OD_TRUE : OD_FALSE);
}


OD_BOOL OD_DownloadIsDDMimeType(const char* type)
{
	MMIBROWSER_MIME_TYPE_E mime = _OD_DownloadConvertMIMETypeFromString(type);

	if(MMIBROWSER_MIME_TYPE_JAD == mime || MMIBROWSER_MIME_TYPE_SDP == mime)
	{
		return OD_TRUE;
	}
	return OD_FALSE;
}

OD_STATUS OD_DownloadHandleMIMEDownloaded(OD_Context *context, const char* url, const char* mimetype, const OD_FileName *filename)
{
#ifdef DL_SUPPORT
    OD_STATUS status = OD_SUCCESS;
    MMIDL_DOWNLOAD_INFO_T download_info = {0};

    if (PNULL != mimetype)
    {
        download_info.mime_type = mime_to_uint(mimetype);
    }
    //download_info.http_header = PNULL;
    download_info.url = (uint8 *)url;
    //download_info.is_chunk = FALSE;
    //download_info.status_code = 200;
    //download_info.method = 0;
    download_info.buf = PNULL;
    //download_info.file_name = (wchar *)filename;

    if (PNULL != context && context->data.net.selected_sim == OD_SIM_INDEX_WIFI)
    {
        download_info.bearer_type = MMIPDP_INTERFACE_WIFI;
    }
    else
    {
        download_info.bearer_type = MMIPDP_INTERFACE_GPRS;
    }
    if(PNULL != context)
    {
        download_info.dual_sys = context->data.net.selected_sim;
    }
    MMIAPIDL_StartDownload(MMIDL_CREATE_NEW, &download_info);

    return status;
#else
	MMIBROWSER_MIME_CALLBACK_PARAM_T* param = NULL;
	OD_STATUS status = OD_SUCCESS;

	OD_ASSERT(url);
	OD_ASSERT(mimetype);
	OD_ASSERT(filename);

	param = (MMIBROWSER_MIME_CALLBACK_PARAM_T*)SCI_ALLOCA(sizeof(MMIBROWSER_MIME_CALLBACK_PARAM_T));
	
	if(param)
	{
		int len = od_strlen(url);

		param->mime_type = _OD_DownloadConvertMIMETypeFromString(mimetype);
		od_wcsncpy(param->file_name, filename, MMIFILE_FULL_PATH_MAX_LEN);
		param->file_name[MMIFILE_FULL_PATH_MAX_LEN - 1] = 0;

		param->url_ptr = (char*)SCI_ALLOCA(len + 1);
		if(!param->url_ptr) 
		{
			SCI_FREE(param);
			return OD_NO_MEMORY;
		}

		od_strcpy(param->url_ptr, url);

#ifdef BROWSER_SUPPORT_NETFRONT
		/*
		* From spreadtrum people: currently download module is in DORADO and NETFRONT.
		* But NETFRONT download module has a little different with DORADO's. 
		* Then add following code for NETFRONT.
		*/
		param->result_code = MMIDL_DOWNLOAD_SUCCESS;
#endif
#ifndef PLATFORM_SC6600L
		param->download_result = TRUE;
#endif /* PLATFORM_SC6600L */

		/** we don't know the result of this callback */
#if defined(OD_6530_12B) || defined(OD_6530_12C) || defined(OD_8501_MAIN)
		MMIAPIBROWSER_MimeDownlaodCallBack(param);
#else
		MMIBROWSER_MIMEDownlaodCallBack(param);
#endif /* OD_6530_12B || OD_6530_12C || OD_8501_MAIN */

		SCI_FREE(param->url_ptr);
		SCI_FREE(param);
	}else
	{
		status = OD_NO_MEMORY;
	}

	return status;
#endif
}




void OD_DASRequestSavePath(const char* url, const char* filename, const char* mimetype, unsigned int filelength, int session_id, 
											const char *header, unsigned int header_len, const char *data, unsigned int data_len)
{

}

void OD_DASReportProgress(int session_id, unsigned int progress, int seq_num, OD_DownloadStatus status, OD_DownloadCause cause)
{

}

void OD_DASResponseCancelPrefetch(int session_id)
{

}

void OD_DASAbortPrefetch(int session_id, OD_DownloadCause cause)
{

}

void OD_DASStopResponse(int session_id, unsigned int progress, int seq_num, OD_DownloadCause cause)
{

}

void OD_DASShowDownloadList(void)
{

}

void OD_DASStartBrowser(void)
{

}

void OD_DASExitBrowser(void)
{

}

void OD_DASSendAuthenticationData(OD_AuthenticationMethod auth_type,
				OD_AuthenticationMechanism auth_option,
				unsigned char* proxy_ip, unsigned short port, const char* url, 
				const char* realm, const char* username, const char* password,
				const char* nonce, const char* opaque, const char* algorithm,
				const char* qop)
{

}

