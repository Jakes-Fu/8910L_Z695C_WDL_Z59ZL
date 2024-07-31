/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

#ifndef AJSC_HTTPLIB_HEADER
#define AJSC_HTTPLIB_HEADER

/**
 * @file ajsc_httplib.h
 * JBlendia HTTP Service
 */

#ifdef __cplusplus
extern "C"{
#endif

#include "aj_type.h"

/** @addtogroup network_aci
 *  @{ 
 */

/*
 *  Interfaces listed in the group are used for HTTP service, 
 *  including these functionalities:
 *   - Sending HTTP request and receive the HTTP respond.
 *   - Set id and password for proxy.
 *   - Set id and password for server.
 *   .
 */

/** 
 * @addtogroup ajsc_httplib_aci_main HTTP Service ACI
 * @{
 */

/** \brief HTTP/HTTPS request method GET. */
#define	AJSC_HTTPLIB_METHOD_GET     (1)
/** \brief HTTP/HTTPS request method HEAD. */
#define	AJSC_HTTPLIB_METHOD_HEAD    (2)
/** \brief HTTP/HTTPS request method POST. */
#define	AJSC_HTTPLIB_METHOD_POST    (3)

/** \brief HTTP/HTTPS status: Connection is closed normally. */
#define AJSC_HTTPLIB_SUCCESS        (1)
/** \brief HTTP/HTTPS status: Getting server authenticate. */
#define AJSC_HTTPLIB_GET_AUTH       (2)
/** \brief HTTP/HTTPS status: Read data. */
#define AJSC_HTTPLIB_DATA_READ      (3)
/** \brief HTTP/HTTPS status: Write data. */
#define AJSC_HTTPLIB_DATA_WRITTEN   (4)
/** \brief HTTP/HTTPS status: Error happened during HTTP/HTTPS communication. */
#define AJSC_HTTPLIB_ERROR          (5)

/** \brief HTTP/HTTPS error code: Network is busy. */
#define AJSC_HTTPLIB_ERROR_BUSY                        (0x1001)
/** \brief HTTP/HTTPS error code: Failed to open HTTP/HTTPS connection. */
#define AJSC_HTTPLIB_ERROR_INVALID_HANDLE              (0x1002)
/** \brief HTTP/HTTPS error code: Failed during HTTP/HTTPS IO. */
#define AJSC_HTTPLIB_ERROR_PROCEED                     (0x1003)
/** \brief HTTP/HTTPS error code: IO buffer is not enough. */
#define AJSC_HTTPLIB_ERROR_BUFF_TOO_SMALL              (0x1004)
/** \brief HTTP/HTTPS error code: The content length does not 
 *         match HTTP/HTTPS header information. 
 */
#define AJSC_HTTPLIB_ERROR_WRONG_CONTENT_LENGTH        (0x1005)
/** \brief HTTP/HTTPS error code: The HTTP/HTTPS url is not valid. */
#define AJSC_HTTPLIB_ERROR_WRONG_URL                   (0x1006)
/** \brief HTTP/HTTPS error code: Invalid HTTP/HTTPS header format. */
#define AJSC_HTTPLIB_ERROR_HEADER_OFFSET               (0x1007)
/** \brief HTTP/HTTPS error code: No content length inside HTTP/HTTPS header. */
#define AJSC_HTTPLIB_ERROR_CONTENT_LENGTH_NOT_SPECIFY  (0x1008)
/** \brief HTTP/HTTPS error code: Invalid HTTP/HTTPS chunk. */
#define AJSC_HTTPLIB_ERROR_DECODE_CHUNKED              (0x1009)
/** \brief HTTP/HTTPS error code: Failed to get HTTP/HTTPS server authentication. */
#define AJSC_HTTPLIB_ERROR_NO_AUTHENTICATE_INFO        (0x1010)

/** \brief Data structure type for storing HTTP/HTTPS header. */
typedef struct _AJSC_Http_Header
{
    /** \brief Field name (ASCII code) (no need terminated by null-character) */
    AJ_S8*   fieldName;
    /** \brief Field name length (in octets) */
    AJ_S32   fieldNameLength;
    /** \brief Field value (ASCII code) (not terminated by null-character) */
    AJ_S8*   fieldValue;
    /** \brief Field value length (in octets) */
    AJ_S32   fieldValueLength;
} AJSC_Http_Header;

/**
 * \brief The function type of the state notification callback for
 *        HTTP/HTTPS communication. The callback function must be
 *        assigned to #ajsc_http_sendRequest before start a HTTP/HTTPS
 *        request.
 *
 *       When the callback state is #AJSC_HTTPLIB_GET_AUTH, the callback
 *       function should use #ajsc_http_setServerNamePasswd and 
 *       #ajsc_http_setProxyNamePasswd to provide the id and password, and
 *       then return <b>1</b> to retry connection. When no id and password
 *       could provide, return <b>0</b> indicates failed.
 *
 * @param param  The identifier that sent to HTTP/HTTPS request via the 
 *               #ajsc_http_sendRequest parameter <i>param</i>.
 * @param states The current HTTP/HTTPS communication state, could be 
 *               one of the follows:
 *               - #AJSC_HTTPLIB_SUCCESS
 *               - #AJSC_HTTPLIB_GET_AUTH
 *               - #AJSC_HTTPLIB_DATA_READ
 *               - #AJSC_HTTPLIB_DATA_WRITTEN
 *               - #AJSC_HTTPLIB_ERROR
 *               .
 * @param value
 *               - When <i>states</i> is #AJSC_HTTPLIB_SUCCESS, <i>value</i> always be <b>0</b>.
 *               - When <i>states</i> is #AJSC_HTTPLIB_GET_AUTH, <i>value</i> always be <b>0</b>.
 *               - When <i>states</i> is #AJSC_HTTPLIB_DATA_READ, <i>value</i> is the total
 *                 read data during the HTTP/HTTPS communication. 
 *               - When <i>states</i> is #AJSC_HTTPLIB_DATA_WRITTEN, <i>value</i> is the total
 *                 written data during the HTTP/HTTPS communication. 
 *               - When <i>states</i> is #AJSC_HTTPLIB_ERROR, <i>value</i> one of the follows:
 *                       - #AJSC_HTTPLIB_ERROR_BUSY
 *                       - #AJSC_HTTPLIB_ERROR_INVALID_HANDLE
 *                       - #AJSC_HTTPLIB_ERROR_PROCEED
 *                       - #AJSC_HTTPLIB_ERROR_BUFF_TOO_SMALL
 *                       .
 *               .
 * @return 
 *              - <b>1</b> for succeed.
 *              - <b>0</b> for failed.
 *              .
 */
typedef AJ_S32 (*AJSC_HTTPLIB_CALLBACK) (AJ_S32 param, AJ_S32 states, AJ_S32 value);

/** 
 * \brief Send HTTP/HTTPS request and retrieve the respond via callback function.
 *
 * @param url       The URL in UTF-8 encoding that the HTTP/HTTPS should send to.
 * @param urlLength The length of <i>url</i>.
 * @param method    The HTTP/HTTPS method, could be one of the follows:
 *                  - #AJSC_HTTPLIB_METHOD_GET
 *                  - #AJSC_HTTPLIB_METHOD_POST
 *                  - #AJSC_HTTPLIB_METHOD_HEAD
 *                  .
 * @param headers   A pointer to the array of AJSC_Http_Heade data structures holding the 
 *                  HTTP/HTTPS requests.
 * @param headersLength The number of JKT_Http_Header data structures pointed to by the 
 *                      <i>headers</i> parameter.
 * @param body      A poniter to the buffer that content the POST data. If the <i>method</i>
 *                  is #AJSC_HTTPLIB_METHOD_GET, this field should be AJ_NULL.
 * @param bodyLength The number of bytes in <i>body</i>.
 * @param param      The connection identifier that will be the <i>param</i> in callback
 *                   function.
 * @param buf        A pointer to the buffer that will contain the read/write data
 *                   during HTTP/HTTPS communication.
 * @param bufSize    The number of bytes of data that <i>buf</i> can contain.
 * @param heapAddr   The huge heap for Java VM usage.
 * @param heapSize   The size <i>heapAddr</i> in byte.
 * @param callback   A pointer of function #AJSC_HTTPLIB_CALLBACK for returning the
 *                   HTTP/HTTPS states.
 *
 * @see #AJSC_HTTPLIB_CALLBACK
 */
AJ_S32 ajsc_http_sendRequest
(
            AJ_U8           *url,
            AJ_S32 	         urlLength,
            AJ_S32 	         method,
            AJSC_Http_Header headers[],
            AJ_S32           headersLength,
            AJ_U8           *body,
            AJ_S32           bodyLength,
            AJ_S32           param,
            AJ_U8           *buf,
            AJ_S32           bufSize,
            void            *heapAddr,
            AJ_S32 	         heapSize,
            AJSC_HTTPLIB_CALLBACK 	callback
);

/** 
 *  \brief Set the user id and password for HTTP/HTTPS server.
 *
 * @param name       The user id.
 * @param nameLength The length of <i>name</i> in bytes.
 * @param passwd     The passwd data.
 * @param passwdLength The length of <i>passwd</i> in bytes.
 *
 * @return <b>0</b> for succeed.
 *         <b>-1</b> for failed.
 */
AJ_S32 ajsc_http_setServerNamePasswd(AJ_U8* name, AJ_S32 nameLength, AJ_U8* passwd, AJ_S32 passwdLength);							

/** 
 *  \brief Set the user id and password for proxy server.
 *
 * @param name       The user id.
 * @param nameLength The length of <i>name</i> in bytes.
 * @param passwd     The passwd data.
 * @param passwdLength The length of <i>passwd</i> in bytes.
 *
 * @return <b>0</b> for succeed.
 *         <b>-1</b> for failed.
 */
AJ_S32 ajsc_http_setProxyNamePasswd(AJ_U8* name, AJ_S32 nameLength, AJ_U8* passwd, AJ_S32 passwdLength);							

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AJSC_HTTPLIB_HEADER */
