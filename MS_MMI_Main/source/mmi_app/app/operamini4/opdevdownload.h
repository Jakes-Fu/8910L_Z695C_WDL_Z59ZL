/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_DOWNLOAD_H
#define OPDEV_DOWNLOAD_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (OD_MMI_TASK *OD_DownloadInfoIndicationCallback)(int total_downloads_count, int active_downloads_count);
typedef void (OD_MMI_TASK *OD_DownloadStartCallback)(int session_id, OD_WCHAR* file_path);
typedef void (OD_MMI_TASK *OD_DownloadCommonCallback)(int session_id);

typedef struct _OD_DownloadCallbacks
{
	OD_DownloadStartCallback start_cb;
	OD_DownloadCommonCallback cancel_cb;
	OD_DownloadCommonCallback need_content_cb;
	OD_DownloadCommonCallback need_download_progress_cb;
	OD_DownloadCommonCallback stop_cb;
	OD_DownloadCommonCallback cancel_prefetch_cb;
	OD_DownloadInfoIndicationCallback report_status_cb;
} OD_DownloadCallbacks;

typedef enum _OD_DownloadStatus
{
	OD_DOWNLOAD_STATUS_DOWNLOADING,
	OD_DOWNLOAD_STATUS_COMPLETE,
	OD_DOWNLOAD_STATUS_ERROR,
} OD_DownloadStatus;

typedef enum _OD_DownloadCause
{
	OD_DOWNLOAD_OK,
	OD_DOWNLOAD_ERROR_INVALID_SESSION_ID,
	OD_DOWNLOAD_ERROR_CANCELLED,
	OD_DOWNLOAD_ERROR_CONNECTION_CLOSED,
	OD_DOWNLOAD_ERROR_DISK_FULL,
	OD_DOWNLOAD_ERROR_FILE_ACCESS,
} OD_DownloadCause;

/**
 * \brief Authentication mechanism to use when sending credentials.
 *
 * This enumeration controls how the credential data is being sent over to
 * the remote end to request access to a resource.
 *
 * The way authentication credentials are sent is also documented by RFC
 * 2617, available <a href="http://tools.ietf.org/html/rfc2617">here</a>.
 */
typedef enum _OD_AuthenticationMechanism
{
	/** Authenticate directly to the server. */
	OD_WPS_HTTP_AUTH_SERVER = 1,

	/** Authenticate via a proxy. */
	OD_WPS_HTTP_AUTH_PROXY
} OD_AuthenticationMechanism;

/**
 * \brief Authentication method to use for requesting protected resources.
 *
 * Right now only two authentication methods are supported, BASIC and
 * DIGEST.  Both methods are defined by RFC 2617, available
 * <a href="http://tools.ietf.org/html/rfc2617">here</a>.
 */
typedef enum _OD_AuthenticationMethod
{
	/** Use BASIC authentication. */
	OD_WPS_HTTP_AUTH_TYPE_BASIC = 1,

	/** Use DIGEST authentication. */
	OD_WPS_HTTP_AUTH_TYPE_DIGEST
} OD_AuthenticationMethod;


OD_ALL_TASK void OD_DASRequestSavePath(const char* url, const char* mimetype, const char* filename, unsigned int filelength, int session_id, 
											const char *header, unsigned int header_len, const char *data, unsigned int data_len);

OD_ALL_TASK void OD_DASReportProgress(int session_id, unsigned int progress, int seq_num, OD_DownloadStatus status, OD_DownloadCause cause);

OD_ALL_TASK void OD_DASResponseCancelPrefetch(int session_id);

OD_ALL_TASK void OD_DASAbortPrefetch(int session_id, OD_DownloadCause cause);

OD_ALL_TASK void OD_DASStopResponse(int session_id, unsigned int progress, int seq_num, OD_DownloadCause cause);

OD_ALL_TASK void OD_DASShowDownloadList(void);

OD_ALL_TASK void OD_DASStartBrowser(void);

OD_ALL_TASK void OD_DASExitBrowser(void);

OD_ALL_TASK void OD_DASSendAuthenticationData(OD_AuthenticationMethod auth_type,
				OD_AuthenticationMechanism auth_option,
				unsigned char* proxy_ip, unsigned short port, const char* url, 
				const char* realm, const char* username, const char* password,
				const char* nonce, const char* opaque, const char* algorithm,
				const char* qop);

/**
 * Query whether the specific mime type is supported or not in this platform
 */
OD_MMI_TASK OD_BOOL OD_DownloadIsMimeTypeSupport(const char* type);

/**
 * Check if it's DD type for OMA or JAD download.
 */
OD_MMI_TASK OD_BOOL OD_DownloadIsDDMimeType(const char* type);

/**
 * Let platform handle the downloaded content
 * @param context, opera context
 * @param url the url where the context download from
 * @param type the mime type of the context
 * @param filename the path name of the context saved.
 * @return OD_SUCCESS if it will be handled
 */
OD_MMI_TASK OD_STATUS OD_DownloadHandleMIMEDownloaded(OD_Context *context, const char* url, const char* mimetype, const OD_FileName *filename);


#ifdef __cplusplus
};
#endif

#endif /* OPDEV_DOWNLOAD_H */
