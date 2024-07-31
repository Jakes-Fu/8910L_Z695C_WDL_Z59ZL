/*
 * Copyright 2002-2004 Aplix Corporation. All rights reserved.
 */

/**
 *  @file kjava_sys_sock.h
 *  Socket KSI
 */

#ifndef KJAVA_SYS_SOCK_HEADER
#define KJAVA_SYS_SOCK_HEADER

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup network_ksi
 *  @{
 */

/** 
 * @addtogroup kjava_sys_sock Socket/Secure Socket KSI
 * @{
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Socket_KSI Socket/Secure Socket KSI
 * @{
 */

/**
 * \brief Socket connect result notification event.
 *
 * This is used for notifying of the result of #kjava_sock_connect processing.
 */
#define KJAVA_SOCKET_EVENT_TYPE_CONNECT     1
/**
 * \brief Socket send result notification event.
 *
 * This is used for notifying of the result of #kjava_sock_send processing.
 */
#define KJAVA_SOCKET_EVENT_TYPE_SEND        2
/**
 * \brief Socket receive result notification event.
 *
 * This is used for notifying of the result of #kjava_sock_receive processing.
 */
#define KJAVA_SOCKET_EVENT_TYPE_RECEIVE     3
/**
 * \brief Socket shutdown result notification event.
 *
 * This is used for notifying of the result of #kjava_sock_shutdown processing.
 */
#define KJAVA_SOCKET_EVENT_TYPE_SHUTDOWN    4
/**
 * \brief Socket close/destroy result notification event.
 *
 * This is used for notifying of the result of #kjava_sock_close processing.
 */
#define KJAVA_SOCKET_EVENT_TYPE_CLOSE       5

/* Socket error codes */
/** 
 * \brief Socket operation succeeded. 
 */
#define KJAVA_SOCKET_SUCCESS                                        0
/** 
 * \brief Socket operation interrupted due to a time out. 
 */
#define KJAVA_SOCKET_ERROR_TIMEOUT                                  -20000
/** 
 * \brief An internal error occurred. 
 */
#define KJAVA_SOCKET_ERROR_OTHER                                    -30000
/** 
 * \brief The certificate contains an unrecognized critical extension. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_BAD_EXTENSIONS               -101
/** 
 * \brief The server certificate chain exceeds the maximum depth specified by the issuer. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_CHAIN_TOO_LONG               -102
/** 
 * \brief The server certificate has expired (it is no longer valid). 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_EXPIRED                      -103
/** 
 * \brief An intermediate certificate in the chain has not been authorized by the Certificate Authority. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_UNAUTHORIZED_INTERMEDIATE_CA -104
/** 
 * \brief The certificate is unsigned. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_MISSING_SIGNATURE            -105
/** 
 * \brief The certificate is not yet valid. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_NOT_YET_VALID                -106
/** 
 * \brief The site name on the certificate is not correct. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_SITENAME_MISMATCH            -107
/** 
 * \brief The issuer of the certificate is unknown. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_UNRECOGNIZED_ISSUER          -108
/** 
 * \brief The certificate signature algorithm is not supported. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_UNSUPPORTED_SIGALG           -109
/** 
 * \brief The public key is used inappropriately by the certificate issuer. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_INAPPROPRIATE_KEY_USAGE      -110
/** 
 * \brief A certificate in the chain was not issued by the same Certificate 
 * Authority as the next certificate in the chain. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_BROKEN_CHAIN                 -111
/** 
 * \brief The public key of the root certificate has expired. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_ROOT_CA_EXPIRED              -112
/** 
 * \brief The certificate public key type is not supported by the device. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_UNSUPPORTED_PUBLIC_KEY_TYPE  -113
/** 
 * \brief Certificate verification failed. 
 */
#define KJAVA_SOCKET_SECURE_ERROR_CERT_VERIFICATION_FAILED          -114

/* Shutdown methods */
/** 
 * \brief Shutdown receiving direction only. 
 */
#define KJAVA_SOCKET_SHUTDOWN_RECEIVE       1
/** 
 * \brief Shutdown sending direction only. 
 */
#define KJAVA_SOCKET_SHUTDOWN_SEND          2
/** 
 * \brief Shutdown both sending and receiving directions. 
 */
#define KJAVA_SOCKET_SHUTDOWN_BOTH          3

/* Socket options */
/**
 * \brief Socket option: Wether to use Nagle algorithm. 
 */
#define KJAVA_SOCKET_OPTION_DELAY           1
/** 
 * \brief Socket option: Wether to send KEEPALIVE signal. 
 */
#define KJAVA_SOCKET_OPTION_KEEPALIVE       2
/** 
 * \brief Socket option: Linger time (wait time before disconnecting socket). 
 */
#define KJAVA_SOCKET_OPTION_LINGER          3
/** 
 * \brief Socket option: Receive buffer size in bytes. 
 */
#define KJAVA_SOCKET_OPTION_RCVBUF          4
/** 
 * \brief Socket option: Send buffer size in bytes. 
 */
#define KJAVA_SOCKET_OPTION_SNDBUF          5

/* Security protocol used in secure socket */
/** 
 * \brief SSL Protocol Version 3.0. 
 */
#define KJAVA_SECURITY_INFO_PROTOCOL_SSL    0
/** 
 * \brief TLS Protocol Version 1.0, or WAP TLS Profile and Tunneling. 
 */
#define KJAVA_SECURITY_INFO_PROTOCOL_TLS    1
/** 
 * \brief Wireless Transport Layer Security. 
 */
#define KJAVA_SECURITY_INFO_PROTOCOL_WTLS   2

/* Time of certificate */
/** 
 * \brief UTC Time type. 
 */
#define KJAVA_CERT_TIME_UTC                 (int)0
/** 
 * \brief Generalized Time type. 
 */
#define KJAVA_CERT_TIME_GENERALIZED         (int)1

/**
 * \brief The function type of the result notificatin callback for Socket KSI functions.
 * @param connectionID
 *        The ID of the connection.
 * @param type
 *        The type of event. Designate one of the following
 *        types based on the KSI function causing the event
 *        notification:
 *        - #kjava_sock_connect<br>
 *            #KJAVA_SOCKET_EVENT_TYPE_CONNECT
 *        - #kjava_sock_send<br>
 *            #KJAVA_SOCKET_EVENT_TYPE_SEND
 *        - #kjava_sock_receive<br>
 *            #KJAVA_SOCKET_EVENT_TYPE_RECEIVE
 *        - #kjava_sock_shutdown<br>
 *            #KJAVA_SOCKET_EVENT_TYPE_SHUTDOWN
 *        - #kjava_sock_close<br>
 *            #KJAVA_SOCKET_EVENT_TYPE_CLOSE
 *        .
 * @param result
 *        The result of this event type. Designate one of the
 *        following types based on the KSI function causing the
 *        event notification:
 *        - #kjava_sock_connect<br>
 *            Designate 0 (success) or an appropriate error
 *            code if it failed.
 *        - #kjava_sock_send<br>
 *            Designate the size (in bytes) of data actually
 *            sent or an appropriate error code if it failed.
 *        - #kjava_sock_receive<br>
 *            Designate the size (in bytes) of data actually
 *            received or an appropriate error code if it failed.
 *        - #kjava_sock_shutdown<br>
 *            Designate 0 (success) or an appropriate error
 *            code if it failed.
 *        - #kjava_sock_close<br>
 *            Designate 0 (success) or an appropriate error
 *            code if it failed.
 *        .
 */
typedef void (*KJavaNetworkCallbackProc) (int connectionID, int type, int result);

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
 * \brief The structure type for containing local connection information.
 * Every field in this structure must be filled after a connection is established.
 */
typedef struct _KJavaLocalInfoTag
{
    /** 
     * \brief The local IP address of the connection, null-terminated string. 
     */
    char                localAddress[16];
    /** 
     * \brief The local port of the connection. 
     */
    int                 localPort;

    /** 
     * \brief Security related information for a secure connection.
     *
     * Set NULL when the <i>secure</i> parameter passed to #kjava_sock_open is 0.
     */
    KJavaSecureInfo     *secureInfo;
}
KJavaLocalInfo;


/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/**
 * \brief Gets the number of maximum concurrent connections.
 *
 * @return The number of the maximum concurrent connections.
 */
int kjava_sock_getMaxSessions(void);

/**
 * \brief Opens a new connection handle.
 * @param addr
 *        A pointer to the string indicating the address of a remote host.
 * @param port
 *        The port number of the remote host.
 * @param secure
 *        A flag indicating whetehr or not the socket is secure.
 * @param connectionID_p
 *        A pointer to the buffer for putting the allocated connection ID.
 * @param callback
 *        The address of the callback function which notifies JBlendia of 
 *        socket-related processing results. The address remains valid 
 *        until #kjava_sock_close is called for the specified socket 
 *        if #kjava_sock_open returns 0. 
 * @return 
 *        Return 0 if the operation succeeds, otherwise return an error code.
 *
 * The native system must create a socket with the passed parameters and 
 * store the identifier assgined to the socket in the buffer indicated by 
 * <i>connectionID_p</i>.
 * 
 * The function address passed in <i>callback</i> must be held in the native 
 * system to notify JBlendia of results of other Socket KSI functions.
 * See description of #KJavaNetworkCallbackProc for details.
 * 
 * @see KJavaNetworkCallbackProc
 */
int kjava_sock_open(const unsigned char *addr, int port, int secure,
                    int *connectionID_p, KJavaNetworkCallbackProc callback);

/**
 * \brief Establish a connection using a specified connection handle.
 * @param connectionID
 *        The ID of the socket connection. The value returned by #kjava_sock_open 
 *        is designated.
 * @param localInfo_p
 *        A pointer to the structure holding the local connection information.
 * @return 
 *        Return 0 if the operation succeeds, otherwise return an error code.
 *
 * Perform the connection establishment in other thread, e.g. in the #KJAVA_NET_THREAD
 * thread. This function should forward the request to the thread and return to the caller.
 *
 * @see KJavaNetworkCallbackProc
 */
int kjava_sock_connect(int connectionID, KJavaLocalInfo *localInfo_p);

/**
 * \brief Sends data over a connection.
 * @param connectionID
 *        The ID of the socket connection. The value returned by #kjava_sock_open 
 *        is designated.
 * @param data
 *        A pointer to the buffer holding the data to be sent.
 * @param datalen
 *        The length in bytes of the data indicated by the <i>data</i> parameter.
 * @return 
 *        Return 0 if the operation succeeds, otherwise return an error code.
 *
 * @see KJavaNetworkCallbackProc
 */
int kjava_sock_send(int connectionID, const unsigned char *data, int datalen);

/**
 * \brief Receives data from a connection.
 * @param connectionID
 *        The ID of the socket connection. The value returned by #kjava_sock_open 
 *        is designated.
 * @param data
 *        A pointer to the buffer for putting the received data.
 * @param datalen
 *        The size of the buffer indicated by the <i>data</i> parameter.
 * @return 
 *        Return 0 if the operation succeeds, otherwise return an error code.
 *
 * @see KJavaNetworkCallbackProc
 */
int kjava_sock_receive(int connectionID, unsigned char *data, int datalen);

/**
 * \brief Gets the size of available data in the buffer of the protocol stack.
 * @param connectionID 
 *        The ID of the socket connection. The value returned by #kjava_sock_open 
 *        is designated.
 * @return 
 *        Return the size of the data that can be read 
 *        without IO processing. Return an error code if an error occurrs.
 */
int kjava_sock_available(int connectionID);

/**
 * \brief Shuts down a connection.
 *
 * @param connectionID
 *        The ID of the socket connection. The value returned by #kjava_sock_open 
 *        is designated.
 * @param method
 *        The direction for shutting down the connection.
 *        One of the follwoing values is designated.
 *        - #KJAVA_SOCKET_SHUTDOWN_RECEIVE
 *        - #KJAVA_SOCKET_SHUTDOWN_SEND
 *        - #KJAVA_SOCKET_SHUTDOWN_BOTH
 *        .
 * @return
 *        Return 0 if the operation succeeds, otherwise return an error code.
 *
 * @see KJavaNetworkCallbackProc
 */
int kjava_sock_shutdown(int connectionID, int method);

/**
 * \brief Closes the connection.
 * @param connectionID
 *        The ID of the socket connection. The value returned by #kjava_sock_open 
 *        is designated.
 * @return
 *        Return 0 if the operation succeeds, otherwise return an error code.
 *
 * When the native system has finished with the callback function passed to 
 * #kjava_sock_open to notify JBlendia of this function result, the native 
 * system should not call the callback address for the closed socket.
 *
 * @see KJavaNetworkCallbackProc
 */
int kjava_sock_close(int connectionID);

/**
 * \brief Sets a socket option of the connection.
 * @param connectionID
 *        The ID of the socket connection. The value returned by #kjava_sock_open 
 *        is designated.
 * @param option
 *        The type of option to set its value. One of the following is designated.
 *        - #KJAVA_SOCKET_OPTION_DELAY
 *        - #KJAVA_SOCKET_OPTION_KEEPALIVE
 *        - #KJAVA_SOCKET_OPTION_LINGER
 *        - #KJAVA_SOCKET_OPTION_RCVBUF
 *        - #KJAVA_SOCKET_OPTION_SNDBUF
 *        .
 * @param value
 *        The value of the option designated by the <i>option</i> parameter.
 * @return 
 *        Return 0 if the operation succeeds, otherwise return an error code.
 */
int kjava_sock_setoption(int connectionID, int option, int value);

/**
 * \brief Gets the socket option of a connection.
 * @param connectionID
 *        The ID of the socket connection. The value returned by #kjava_sock_open 
 *        is designated.
 * @param option
 *        The type of option to obtain its value. One of the following is designated.
 *        - #KJAVA_SOCKET_OPTION_DELAY
 *        - #KJAVA_SOCKET_OPTION_KEEPALIVE
 *        - #KJAVA_SOCKET_OPTION_LINGER
 *        - #KJAVA_SOCKET_OPTION_RCVBUF
 *        - #KJAVA_SOCKET_OPTION_SNDBUF
 *        .
 * @return 
 *        Return the value retrieved from native layer for the option designated.
 *        Return KJAVA_SOCKET_ERROR_OTHER if an error happened.

 */
int kjava_sock_getoption(int connectionID, int option);

/** @} */
/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* KJAVA_SYS_SOCK_HEADER */
