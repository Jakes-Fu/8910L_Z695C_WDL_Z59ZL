/*
 * Copyright 2002-2004 Aplix Corporation. All rights reserved.
 */

/**
 *  @file kjava_sys_http.h
 *  KSI function prototypes for HTTP and HTTPS communication
 */

#ifndef KJAVA_SYS_HTTP_HEADER
#define KJAVA_SYS_HTTP_HEADER

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup network_ksi
 *  @{
 */

/**
 * @defgroup kjava_sys_http HTTP/HTTPS KSI
 * @{
 */

/**
 * @addtogroup HTTP_Constant Constant and Definition
 * @{
 */

/**
 * \brief An HTTP/HTTPS connection request by #kjava_http_open was accepted.
 *
 * When an HTTP/HTTPS connection to the destination server has been opened in response 
 * to #kjava_http_open, or when preparations for connection opening have been completed, 
 * call #KJavaHttpCallbackProc to make the following event notification to JBlendia.
 * 
 * <table><caption>Event Notified when Succeeding in Required Processing in kjava_http_open</caption>
 *  <tr>
 *   <td><b> #KJavaHttpCallbackProc parameters</b></td>
 *   <td>Designated values</td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td> #kjava_http_open param value </td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td>#KJAVA_HTTP_EVENT_OPEN</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td> 0</td>
 *  </tr>
 * </table>
 */
#define     KJAVA_HTTP_EVENT_OPEN              1

/**
 * \brief SSL session successfully established in HTTPS communication.
 *
 * When an SSL connection is successfully established in HTTPS communication, call
 * #KJavaHttpCallbackProc to make the following event notification to JBlendia.
 *
 * <table><caption>Event Notified when SSL Session Established</caption>
 *  <tr>
 *   <td><b> #KJavaHttpCallbackProc parameters</b></td>
 *   <td>Designated values</td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td> Value of param passed by #kjava_http_open</td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td> #KJAVA_HTTP_EVENT_ESTABLISH_SSL_SESSION</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td>0</td>
 *  </tr>
 * </table>
 *
 */
#define     KJAVA_HTTP_EVENT_ESTABLISH_SSL_SESSION         2

/**
 * \brief An HTTP/HTTPS header send request by #kjava_http_write_header was accepted.
 *
 * When header sending to the connected server was completed in response to 
 * #kjava_http_write_header, or when preparations for header sending have been completed,
 * call #KJavaHttpCallbackProc to make the following event notification to JBlendia.
 *
 * <table><caption>Event Notified when Succeeding in Required Processing in kjava_http_write_header.</caption>
 *  <tr>
 *   <td><b> #KJavaHttpCallbackProc parameters </b></td>
 *   <td>Designated values</td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td> #kjava_http_open param value</td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td> #KJAVA_HTTP_EVENT_WRITE_HEADER</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td> 0</td>
 *  </tr>
 * </table>
 *
 */
#define     KJAVA_HTTP_EVENT_WRITE_HEADER      3

/**
 * \brief An HTTP/HTTPS request body send request by #kjava_http_write was accepted.
 *
 * When request body sending to the connected server has been completed in response 
 * to #kjava_http_write, or when preparations for request body sending have been 
 * completed, call #KJavaHttpCallbackProc to make the following event notification to 
 * JBlendia.
 *
 * <table><caption>Event Notified when Succeeding in Required Processing in kjava_http_write.</caption>
 *  <tr>
 *   <td><b> #KJavaHttpCallbackProc parameters</b></td>
 *   <td>Designated values</td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td> #kjava_http_open param value</td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td> #KJAVA_HTTP_EVENT_WRITE</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td>Size (in octets) of data sent or prepared for sending.</td>
 *  </tr>
 * </table>
 *
 */
#define     KJAVA_HTTP_EVENT_WRITE             4

/**
 *  \brief Send processing was completed in response to an HTTP/HTTPS request body send 
 *  request by #kjava_http_flush.
 *
 *
 * Once all previously unsent HTTP/HTTPS request bodies have been sent in response 
 * to #kjava_http_flush, call #KJavaHttpCallbackProc and notify the event indicated below
 * to JBlendia.
 *
 * <table><caption>
 *    Event Notified when kjava_http_flush Completes Required Processing
 *  </caption>
 *  <tr>
 *   <td><b>
 *    #KJavaHttpCallbackProc parameters
 *   </b></td>
 *   <td>
 *    Designated values
 *   </td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td>
 *    Value of param passed by #kjava_http_open
 *   </td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td> #KJAVA_HTTP_EVENT_FLUSH</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td>0</td>
 *  </tr>
 * </table>
 *
 */
#define     KJAVA_HTTP_EVENT_FLUSH    5

/**
 * \brief Notification of HTTP/HTTPS request completion by #kjava_http_finish_request was accepted.
 *
 * When receipt of a response from the connected server is possible as requested 
 * by #kjava_http_finish_request, call #KJavaHttpCallbackProc to make the following event 
 * notification to JBlendia.
 *
 * <table><caption>Event Notified when Succeeding in Required Processing in kjava_http_finish_request.</caption>
 *  <tr>
 *   <td><b> #KJavaHttpCallbackProc parameters</b></td>
 *   <td>Designated values</td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td> Value of param passed by #kjava_http_open</td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td> #KJAVA_HTTP_EVENT_FINISH_REQUEST</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td>0</td>
 *  </tr>
 * </table>
 *
 */
#define     KJAVA_HTTP_EVENT_FINISH_REQUEST    6

/**
 * \brief An HTTP/HTTPS response requested by #kjava_http_read was received successfully.
 *
 * When received data has been put in the area designated by the data parameter as 
 * requested by #kjava_http_read, call #KJavaHttpCallbackProc to make the following event 
 * notification to JBlendia.
 *
 * <table><caption>Event Notified when Succeeding in Required Processing in kjava_http_read.</caption>
 *  <tr>
 *   <td><b> #KJavaHttpCallbackProc parameters</b></td>
 *   <td>Designated values</td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td> Value of param passed by #kjava_http_open</td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td> #KJAVA_HTTP_EVENT_READ</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td>Size (in octets) of receive data put in the buffer designated by the 
 *       #kjava_http_read data parameter.
 *       Put a value between 1 and the value of the #kjava_http_read dataLength parameter.
 *       When all the response data received from the connected server has been passed to
 *       JBlendia, set 0. </td>
 *  </tr>
 * </table>
 * 
 */
#define     KJAVA_HTTP_EVENT_READ              7

/**
 * \brief An HTTP/HTTPS connection close request by #kjava_http_close was accepted.
 *
 * When the HTTP/HTTPS connection has been closed as requested by #kjava_http_close, or 
 * when preparations for closing have been completed, call #KJavaHttpCallbackProc to make 
 * the following event notification to JBlendia.
 *
 * <table><caption>Event Notified when Succeeding in Required Processing in kjava_http_close.</caption>
 *  <tr>
 *   <td><b> #KJavaHttpCallbackProc parameters</b></td>
 *   <td>Designated values</td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td> Value of param passed by #kjava_http_open</td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td> #KJAVA_HTTP_EVENT_CLOSE</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td>0</td>
 *  </tr>
 * </table>
 *
 */
#define     KJAVA_HTTP_EVENT_CLOSE             8

/**
 * \brief An error occurred.
 *
 * When an error occurs in HTTP/HTTPS communication processing, call 
 * #KJavaHttpCallbackProc to make the following event notification to JBlendia.
 *
 * <table><caption>Event Notified when Error Occurs</caption>
 *  <tr>
 *   <td><b> #KJavaHttpCallbackProc parameters</b></td>
 *   <td>Designated values</td>
 *  </tr>
 *  <tr>
 *   <td><b>param</b></td>
 *   <td> Value of param passed by #kjava_http_open</td>
 *  </tr>
 *  <tr>
 *   <td><b>cause</b></td>
 *   <td> #KJAVA_HTTP_EVENT_ERROR</td>
 *  </tr>
 *  <tr>
 *   <td><b>option</b></td>
 *   <td>Set an appropriate error code, starting with KJAVA_HTTP_ERROR_, based on the type 
 *   of error.
 *   If there is no such error code, set any value above 
 *   #KJAVA_HTTP_ERROR_CUSTOMER_SPEC_MIN.</td>
 *  </tr>
 * </table>
 *
 * If #KJAVA_HTTP_ERROR_TIMEOUT or the value greater than or equal to #KJAVA_HTTP_ERROR_CUSTOMER_SPEC_MIN 
 * is designated to the <i>option</i> parameter of #KJavaHttpCallbackProc, 
 * the behavior of JBlendia depends on the value JBlendia has designated to the <i>timeout</i> parameter 
 * of kjava_http_open. 
 *
 * When #KJAVA_HTTP_ERROR_TIMEOUT is designated:
 * <table>
 *  <tr>
 *   <td><i>Timeout</i><b> Parameter of kjava_http_open</b></td>
 *   <td><b>The Behavior of JBlendia After the Event Notification</b></td>
 *  </tr>
 *  <tr>
 *   <td>1</td>
 *   <td>Throws an <code>InterruptedIOException</code></td>
 *  </tr>
 *  <tr>
 *   <td>0</td>
 *   <td>Throws an <code>IOException</code></td>
 *  </tr>
 * </table>
 *
 * When the value greater than or equal to #KJAVA_HTTP_ERROR_CUSTOMER_SPEC_MIN is designated:
 * <table>
 *  <tr>
 *   <td><i>Timeout</i><b> Parameter of kjava_http_open</b></td>
 *   <td><b>The Behavior of JBlendia After the Event Notification</b></td>
 *  </tr>
 *  <tr>
 *   <td>#TRUE<br>#FALSE</td>
 *   <td>Throws an <code>IOException</code></td>
 *  </tr>
 * </table>
 *
 */
#define     KJAVA_HTTP_EVENT_ERROR             0


/**
 * \brief Connection timeout
 *
 * If a timeout error occurs, with no response from the server after a set period, call
 * #KJavaHttpCallbackProc setting the error code in the option parameter.
 */
#define     KJAVA_HTTP_ERROR_TIMEOUT                                1

/**
 * \brief The certificate contains an unrecognized critical extension.
 */
#define     KJAVA_HTTP_ERROR_CERT_BAD_EXTENSIONS                    101

/**
 * \brief The server certificate chain exceeds the maximum depth specified by the issuer.
 */
#define     KJAVA_HTTP_ERROR_CERT_CHAIN_TOO_LONG                    102

/**
 * \brief The server certificate has expired (it is no longer valid).
 */
#define     KJAVA_HTTP_ERROR_CERT_EXPIRED                           103

/**
 * \brief An intermediate certificate in the chain has not been authorized by the 
 * Certificate Authority.
 */
#define     KJAVA_HTTP_ERROR_CERT_UNAUTHORIZED_INTERMEDIATE_CA      104
    
/**
 * \brief The certificate is unsigned.
 */
#define     KJAVA_HTTP_ERROR_CERT_MISSING_SIGNATURE                 105

/**
 * \brief The certificate is not yet valid.
 */
#define     KJAVA_HTTP_ERROR_CERT_NOT_YET_VALID                     106

/**
 * \brief The site name on the certificate is not correct.
 */
#define     KJAVA_HTTP_ERROR_CERT_SITENAME_MISMATCH                 107

/**
 * \brief The issuer of the certificate is unknown.
 */
#define     KJAVA_HTTP_ERROR_CERT_UNRECOGNIZED_ISSUER               108

/**
 * \brief The certificate signature algorithm is not supported.
 */
#define     KJAVA_HTTP_ERROR_CERT_UNSUPPORTED_SIGALG                109

/**
 * \brief The public key is used inappropriately by the certificate issuer.
 */
#define     KJAVA_HTTP_ERROR_CERT_INAPPROPRIATE_KEY_USAGE           110

/**
 * \brief A certificate in the chain was not issued by the same Certificate Authority as the 
 * next certificate in the chain.
 */
#define     KJAVA_HTTP_ERROR_CERT_BROKEN_CHAIN                      111

/**
 * \brief The public key of the root certificate has expired.
 */
#define     KJAVA_HTTP_ERROR_CERT_ROOT_CA_EXPIRED                   112

/**
 * \brief The certificate public key type is not supported by the device.
 */
#define     KJAVA_HTTP_ERROR_CERT_UNSUPPORTED_PUBLIC_KEY_TYPE       113

/**
 * \brief Certificate verification failed.
 */
#define     KJAVA_HTTP_ERROR_CERT_VERIFICATION_FAILED               114

/**
 * \brief The minimum value in the customer-specified area
 */
#define     KJAVA_HTTP_ERROR_CUSTOMER_SPEC_MIN                      10000


/**
 * \brief HTTP/HTTPS request method GET
 */
#define     KJAVA_HTTP_METHOD_GET      1

/**
 * \brief HTTP/HTTPS request method HEAD
 */
#define     KJAVA_HTTP_METHOD_HEAD     2

/**
 * \brief HTTP/HTTPS request method POST
 */
#define     KJAVA_HTTP_METHOD_POST     3


/**
 * \brief Handle type for HTTP/HTTPS connection
 */
typedef const void* KJavaHttpHandle;

/** \brief Handle value for invalid. */
#define KJAVA_INVALID_HANDLE   ((KJavaHttpHandle)0xFFFFFFFF)

/**
 * \brief Data structure type for storing HTTP/HTTPS header
 */
typedef struct _KJava_Http_HeaderTag {
    /**
     * \brief Field name (ASCII code) (not null-terminated)
     */
    char*      fieldName;

    /**
     * \brief Field name length (in octets)
     */
    int        fieldNameLength;

    /**
     * \brief Field value (ASCII code) (not null-terminated)
     */
    char*      fieldValue;

    /**
     * \brief Field value length (in octets)
     */
    int        fieldValueLength;
} KJava_Http_Header;

/**
 *  \brief The structure type for storing security related information of a secure connection.
 *  Every field in this structure must be filled after a connection is established.
 */
typedef struct _KJavaSecureInfoTag
{
    /**
     * \brief The security protocol used in this secure connection.
     *
     * Set one of following values:
     * - #KJAVA_SECURITY_INFO_PROTOCOL_SSL
     * - #KJAVA_SECURITY_INFO_PROTOCOL_TLS
     * - #KJAVA_SECURITY_INFO_PROTOCOL_WTLS
     * .
     */
    int     securityProtocol;
    /**
     * \brief The cipher suite used in this secure connection.
     *
     * It must be stored in format of (key_exchange)_(cipher)_(hash).
     * Example: "EDH_RSA_SHA1"
     */
    char    cipherSuite[20];

    /**
     * \brief The server certificate type, null-terminated UTF8 string.
     *
     * For example, if the certificate is a X.509 certificate, store "X.509" in this field.
     */
    char    certType[8];
    /**
     * \brief The version number of the server certificate.
     *
     * For example, if the certificate is a X.509 v3 certification, store 3 in this field.
     */
    int     certVersion;
    /**
     *  \brief The serial number of the server certificate, null-terminated string.
     *
     *  Refer to Setion 4.1.2.2 in RFC3280 for the format of serial number.
     */
    char    certSN[21];
    /**
     * \brief The subject of the server certificate, null-terminated UTF8 string.
     *
     * Refer to the JkSecureSocketGetCertSubject part in the document
     * <i>"JBlend[micro] Interface Reference: MIDP2.0 SecureConnection"</i> for detail.
     */
    char    certSubject[400];
    /**
     * \brief The issuer of the server certificate, null-terminated UTF8 string.
     *
     * Refer to the JkSecureSocketGetCertIssuer part in the document
     * <i>"JBlend[micro] Interface Reference: MIDP2.0 SecureConnection"</i> for detail.
     */
    char    certIssuer[400];
    /**
     * \brief The algorithm used in the server certificate signature, null-terminated UTF8 string.
     *
     * The algorithm name format is a combination of the message digest algorithm and
     * public key algorithm, based on the items specified in RFC 3279.
     *
     * For example, for a certificate uses SHA-1 message digest and RSA algorithm, 
     * the signature algorithm woule be "SHA1withRSA".
     */
    char    certSigAlgName[20];
    /**
     * \brief The NotBefore time for the server certificate, null-terminated string.
     *
     * Store NULL if the valid term start time is not specified.
     * Refer to Section 4.1.2.5 "Validity" in RFC3280 for proper time format.
     */
    char    certNotBefore[20];
    /**
     * \brief The time format used in the NotBefore field.
     *
     * Set one of these values:
     * - #KJAVA_CERT_TIME_UTC
     * - #KJAVA_CERT_TIME_GENERALIZED
     * .
     */
    int     certNBTimeType;
    /**
     * \brief The NotAfter time for the server certificate, null-terminated string.
     *
     * Store NULL if the valid term start time is not specified.
     * Please refer to Section 4.1.2.5 "Validity" in RFC3280 for proper time format.
     */
    char    certNotAfter[20];
    /** 
     * \brief The time format used in the NotAfter field.
     *
     * Set one of these values:
     * - #KJAVA_CERT_TIME_UTC
     * - #KJAVA_CERT_TIME_GENERALIZED
     * .
     */
    int     certNATimeType;
}
KJavaSecureInfo;

/**
 * \brief Notifies an HTTP/HTTPS event to JBlendia.
 *
 * @param param
 *    Set the param parameter passed when the HTTP/HTTPS connection for which the event 
 *    occurred was created by #kjava_http_open.
 * @param cause
 *    Set a value appropriate for the event.
 * @param option
 *    Event details.
 *    Set a value appropriate for the event.
 *
 * @return
 *    1 is returned if notification succeeded, or 0 if it failed.
 */
typedef int (* KJavaHttpCallbackProc) ( int param, int cause, int option );

/** @} */

/**
 * @addtogroup HTTP_KSI HTTP/HTTPS Service
 * @{
 */

/**
 * \brief Opens an HTTP/HTTPS connection.
 *
 * @param url
 *      The URL string  (not null-terminated).
 *      This pointer remains valid until the request result event is notified to 
 *      JBlendia.
 * @param urlLength
 *      The size (in octets) of the url parameter.
 * @param method
 *      The request method. 
 *      Either #KJAVA_HTTP_METHOD_GET, #KJAVA_HTTP_METHOD_HEAD, 
 *      or #KJAVA_HTTP_METHOD_POST is designated.
 * @param param
 *      The value that must be passed as a parameter of the event when the event 
 *      is notified with KJavaHttpCallbackProc. 
 * @param timeout
 *      The flag which a MIDlet has designated to the <code>timeouts</code> parameter of 
 *      <code>javax.microedition.io.Connector.open(String name, int mode, boolean timeouts)</code> method.
 * @param callback
 *      The pointer to the callback function of VM, should be called after a 
 *      action is completed.
 *
 * @return
 *      When an HTTP/HTTPS connection is created normally, return a handler 
 *      value indicating the created connection.
 *      If an HTTP/HTTPS connection could not be created, return #KJAVA_INVALID_HANDLE.
 *
 * This function is called in order to create a connection to an HTTP server.
 * The timing for actually connecting to the HTTP server is an implementation 
 * dependent matter.
 * To enable other functions to perform various operations on the created 
 * HTTP/HTTPS connection, issue a handle value that uniquely identifies the 
 * connection and notify this handle as a return value. 
 * The created connection and the handle value issued for it should remain valid
 * until #kjava_http_close is called.
 *
 * JBlendia sets 1 to the <i>timeout</i> parameter only when a MIDlet has designated <code>true</code> to the 
 * <code>timeouts</code> parameter of <code>javax.microedition.io.Connector.open(String name, int mode, boolean timeouts)</code> 
 * method, JBlendia sets 0 to the <i>timeout</i> parameter otherwize.
 * When the native system notifies a MIDlet of the #KJAVA_HTTP_EVENT_ERROR event by #KJavaHttpCallbackProc,
 * if #KJAVA_HTTP_ERROR_TIMEOUT or the value greater than or equal to #KJAVA_HTTP_ERROR_CUSTOMER_SPEC_MIN is designated 
 * to the <i>option</i> parameter, the behavior of JBlendia depends on the value JBlendia has designated to 
 * the <i>timeout</i> parameter of kjava_http_open. 
 * For the details of this behavior, refer to the description of #KJAVA_HTTP_EVENT_ERROR.
 *
 * When the connection or preparations for it are completed, call #KJavaHttpCallbackProc 
 * to notify a #KJAVA_HTTP_EVENT_OPEN event to JBlendia.
 *
 * @see KJavaHttpCallbackProc
 * @see kjava_http_close
 */
KJavaHttpHandle kjava_http_open( const char* url, int urlLength, int method, int param, int timeout, KJavaHttpCallbackProc callback );


/**
 * \brief Sends an HTTP/HTTPS request header.
 *
 * @param connection
 *      The applicable HTTP/HTTPS connection. The value returned by 
 *      #kjava_http_open is designated.
 * @param header
 *      A pointer to the array of KJava_Http_Header data structures holding the 
 *      HTTP/HTTPS requests.
 *      The termination of each header does not include a line break code (CR or LF). <br>
 *      This pointer remains valid until the request result event is notified to 
 *      JBlendia.
 * @param headerCount
 *      The number of KJava_Http_Header data structures pointed to by the header 
 *      parameter.
 *
 * @return
 *      If the operation succeeds, return 0; if it fails (error), return -1.
 *
 * This function is called to send an HTTP/HTTPS header to an HTTP server. 
 * The actual send timing to the HTTP server is implementation dependent.
 *
 * When sending or preparations for sending are complete, call #KJavaHttpCallbackProc
 * to notify a #KJAVA_HTTP_EVENT_WRITE_HEADER event to JBlendia.
 *
 * The sequence of the individual HTTP/HTTPS header pointers indicated by the 
 * header parameter may be changed for sorting or other editing, so long as it 
 * is still possible to free the memory allocated for the HTTP/HTTPS header.
 *
 *  @if JTWI
 * @par For compliance with JTWI 1.0:
 * If the MIDlet currently being executed on a GSM/UMTS (W-CDMA) handset is an untrusted MIDlet,
 * JTWI-compliant JBlendia calls this function appending the following header information 
 * to that designated by the MIDlet.
 *  <br><br>
 * <table border="1">
 * <tr><td><b> Member of KJava_Http_Header Structure </b></td>
 *   <td><b>Value Set </b></td></tr>
 * <tr><td>fieldName</td><td>User-Agent</td></tr>
 * <tr><td>fieldNameLength</td><td>10</td></tr>
 * <tr><td>fieldValue</td><td>UNTRUSTED/1.0</td></tr>
 * <tr><td>fieldValueLength</td><td>13</td></tr>
 * </table>
 *  <br>
 * An HTTP request must be sent containing this header information along with the User-Agent 
 * header designated by the MIDlet.
 *  @endif
 *
 * @see KJavaHttpCallbackProc
 * @see kjava_http_open
 */
int kjava_http_write_header( KJavaHttpHandle connection, KJava_Http_Header header[], int headerCount );


/**
 * \brief Sends an HTTP/HTTPS request body.
 *
 * @param connection
 *      The applicable HTTP/HTTPS connection. The value returned by 
 *      #kjava_http_open is designated.
 * @param data
 *      A pointer to the HTTP/HTTPS request body to be sent.
 *      This pointer remains valid until the request result event is notified to 
 *      JBlendia.
 * @param dataLength
 *      The parameter data size in octets
 *
 * @return
 *      If the operation succeeds, 0 is returned; if it fails (error), -1 is 
 *      returned.
 *
 * This function is called to send an HTTP/HTTPS request body to the HTTP server. 
 * The actual data send timing is implementation dependent.
 *
 * When request body sending or preparations for sending are complete, call 
 * #KJavaHttpCallbackProc to notify a #KJAVA_HTTP_EVENT_WRITE event to JBlendia.
 *
 * It is allowable to process only the amount of data that can be handled, 
 * without completing all the designated request body processing.
 * Designate the size of the processed request body in the option parameter 
 * with #KJavaHttpCallbackProc.
 * JBlendia looks at the processed data size as notified in the 
 * #KJAVA_HTTP_EVENT_WRITE event and continues to call this function until the entire 
 * request body has been processed. 
 *
 * @see KJavaHttpCallbackProc
 * @see kjava_http_open
 */
int kjava_http_write( KJavaHttpHandle connection, const unsigned char* data, int dataLength );

/**
 * \brief Sends request body data buffered in the native system to the HTTP/HTTPS server.
 *
 * @param connection
 *      The applicable HTTP/HTTPS connection. The value returned by 
 *      #kjava_http_open is designated.
 *
 * @return
 *      Return 0 if the operation succeeds, or -1 in case of error.
 *
 * This function is called to complete the sending of HTTP/HTTPS request body 
 * data buffered in the native system. It applies to the HTTP/HTTPS connection 
 * designated by the connection parameter.
 * When this function is called, the native system should immediately send 
 * buffered HTTP/HTTPS request body data that has not yet been sent.
 * If request body sending is not yet possible because a connection to the 
 * HTTP/HTTPS server is not established or the HTTP/HTTPS headers have not been 
 * sent, first connect to the server and send the headers, then send the 
 * HTTP/HTTPS request body data.
 *
 * When the native system has completed sending all the buffered HTTP/HTTPS 
 * request body data, call #KJavaHttpCallbackProc notifying the 
 * #KJAVA_HTTP_EVENT_FLUSH event to JBlendia.
 *
 *
 * @see KJavaHttpCallbackProc
 * @see kjava_http_open
 * @see kjava_http_write
 */
int kjava_http_flush( KJavaHttpHandle connection );


/**
 * \brief Notifies the completion of an HTTP/HTTPS request.
 *
 * @param connection
 *      The applicable HTTP/HTTPS connection. The value returned by 
 *      #kjava_http_open is designated.
 * @return
 *      If the operation succeeds, 0 is returned; if it fails (error), -1 is 
 *      returned.
 *
 * This function is called to notify the HTTP server that the HTTP/HTTPS request has 
 * been completed. After this function is called, #kjava_http_write_header and  
 * #kjava_http_write will not be called for the same HTTP/HTTPS connection.
 *
 * When request sending is completed and preparations for receiving the response can be
 * made, call #KJavaHttpCallbackProc to notify a #KJAVA_HTTP_EVENT_FINISH_REQUEST event to 
 * JBlendia.
 *
 * After the #KJAVA_HTTP_EVENT_FINISH_REQUEST event is notified, #kjava_http_Read is called 
 * requesting receipt of the HTTP/HTTPS response.
 *
 * @see kjava_http_rrite_header
 * @see kjava_http_write
 * @see kjava_http_read
 */
int kjava_http_finish_request( KJavaHttpHandle connection );


/**
 * \brief Gets the HTTP/HTTPS response received from the HTTP server.
 *
 * @param connection
 *      The applicable HTTP/HTTPS connection. The value returned by 
 *      #kjava_http_open is designated.
 * @param data
 *      A pointer to the area holding data received from the HTTP server.
 *      This pointer remains valid until a request result event is notified to 
 *      JBlendia.
 * @param dataLength
 *      The size (in octets) of the area designated by the data parameter.
 *
 * @return
 *      If the operation succeeds, 0 is returned; if it fails (error), -1 is 
 *      returned.
 *
 * This function is called to get the HTTP/HTTPS response received from the HTTP 
 * server. 
 * Note that no flow control of the receive data is performed in JBlendia. 
 * If flow control is necessary, it must be implemented by the device manufacturer.
 *
 * When the received data has been put in the memory area designated by the data 
 * parameter, call #KJavaHttpCallbackProc to notify a #KJAVA_HTTP_EVENT_READ event to 
 * JBlendia.
 * The format of the HTTP response to be passed in the data parameter must 
 * conform to the format specified in RFC 2616.
 * Below is an example of an HTTP response that is in accordance with 
 * RFC 2616, Section 6, "Responce."
 *
 * <pre>
 * HTTP/1.1 200 OK
 * Date: Wed, 24 Mar 2004 08:00:25 GMT
 * Content-Length: 23
 * Connection: close
 * Content-Type: text/plain; charset=ISO-8859-1
 * 
 * HTTP response message
 * </pre>
 * 
 * Since HTTP responses whose status code starts with 1, such as HTTP/1.1 1xx, 
 * are temporary, do not pass them to JBlendia.
 * Pass to JBlendia only the eventual responses following them.
 *
 * @see KJavaHttpCallbackProc
 * @see kjava_http_open
 */
int kjava_http_read( KJavaHttpHandle connection, unsigned char* data, int dataLength );

/**
 * \brief Gets the HTTP/HTTPS response size that can be obtained with no I/O blocking.
 *
 * @param connection
 *      The applicable HTTP/HTTPS connection. The value returned by 
 *      #kjava_http_open is designated.
 *
 * @return
 *      Return the HTTP/HTTPS response size (in octets) that can be obtained without 
 *      I/O blocking, that is, the size buffered in the communication layer by the 
 *      manufacturer-implemented processing at the time this function is called.
 *      In case of error, return -1.
 *
 * Return the HTTP/HTTPS response size already received from the HTTP server, buffered 
 * in the communication layer and not yet passed to JBlendia in response to 
 * #kjava_http_read.
 * 
 * In processing this function it is not necessary to notify an HTTP/HTTPS event to 
 * JBlendia. Return control to JBlendia as soon as possible so that JBlendia
 * processsing will not be blocked by this function for too long.
 *
 * @see kjava_http_open
 */
int kjava_http_available( KJavaHttpHandle connection );

/**
 * \brief Closes an HTTP/HTTPS connection.
 *
 * @param connection
 *      The HTTP/HTTPS connection to be closed. The value returned by 
 *      #kjava_http_open is designated.
 *
 * @return
 *      If the close operation succeeds, return 0; if it fails (error), return -1. 
 *
 * This function is called to close the HTTP/HTTPS connection designated in 
 * the connection parameter. Free the resources allocated for the designated 
 * connection. After this function is called, the designated connection can no 
 * longer be used.
 * When a request to close the HTTP/HTTPS connection is accepted, call
 * #KJavaHttpCallbackProc to notify a #KJAVA_HTTP_EVENT_CLOSE event to JBlendia. 
 * It must be possible to call this function at any point while an HTTP/HTTPS
 * connection is created.
 * After this function is called, no more processing requests will be made for the 
 * HTTP/HTTPS connection designated in the connection parameter.
 *
 * @see KJavaHttpCallbackProc
 * @see kjava_http_open
 */
int kjava_http_close( KJavaHttpHandle connection );


/**
 *  \brief Retrieve security related information for a HTTPS connection.
 *
 *  @param connection
 *      The HTTP/HTTPS connection to be closed. The value returned by 
 *      #kjava_http_open is designated.
 *  @param secureInfo
 *      A pointer to the KJavaSecureInfo structure to store security related 
 *      information. This function will be called once after the 
 *      #KJAVA_HTTP_EVENT_ESTABLISH_SSL_SESSION event is notified to get all
 *      information required by JBlendia.
 *
 *  @return
 *      If all security information are successfully store in the structure,
 *      return 0; if it fails, return -1.
 */
int kjava_http_get_secureinfo( KJavaHttpHandle connection, KJavaSecureInfo *secureInfo );

/** @} */
/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* KJAVA_SYS_HTTP_HEADER */
