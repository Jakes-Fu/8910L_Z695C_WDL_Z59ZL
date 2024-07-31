/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

#ifndef AJSC_I_HTTPLIB_HEADER
#define AJSC_I_HTTPLIB_HEADER

#ifdef __cplusplus
extern "C"{
#endif

#include <jktypes.h>
#include <kjava_action.h>
#include <jkhttp.h>
#include <ajsc_httplib.h>

#define AJSC_HTTPLIB_STATE_IDLING						0
#define AJSC_HTTPLIB_STATE_REQUEST						1
#define AJSC_HTTPLIB_STATE_READING						2
#define AJSC_HTTPLIB_STATE_SET_AUTH						3

#define AJSC_HTTPLIB_MAX_NAME_LENGTH					128
#define AJSC_HTTPLIB_MAX_PASSWORD_LENGTH				128

typedef struct _AJSCHttpRequestTag
{
    AJ_S32     param;

    AJ_U8    *ajmmBuf_p;
    AJ_S32     ajmmBufSize;

    AJ_U8    *httpBuf_p;
    AJ_S32     httpBufSize;

    AJ_U8    *http_url;
    AJ_S32     urlLength;

    JKT_Http_Header* 	jkt_http_header;
    AJ_S32     			headerLength;
    
    AJ_S32     method;

	AJ_U8	*httpBodyBuf_p;
	AJ_S32		httpBodyBufSize;
	AJ_S32		httpBodyBufWriteIndex;
	
	AJSC_HTTPLIB_CALLBACK	cb;
	
	AJ_S32		tryAuthorized;
	
}
AJSCHttpRequest;

typedef struct _AJSCHttpNotifyTag
{
    AJ_S32     param;
    AJ_S32     cause;
    AJ_S32     option;
}
AJSCHttpNotify;

AJ_S32 ajsc_http_internal_sendRequest(KJavaActionCommand *action_p);

JKBool ajsc_http_event_cb(JKSint32 param, JKSint32 cause, JKSint32 option);					

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AJSC_HTTPLIB_HEADER */
