/*
 * Copyright 2004-2011 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

#ifdef  JBED_CDCA
/**
 * @file
 * Jbed Porting Layer: Legacy Networking support (VM-independent)
 */
#else
/**
 * @file
 * Jbed Porting Layer: Networking support (VM-independent)
 */
#endif

/**
 * @file
 * This interface provides socket-based networking support for the Jbed VM.
 *
 * IP addresses and port numbers are always specified in host order.
 * It is up to the port to do any necessary conversions.
 *
 * \section cpl_net_porting Porting comments
 * \subsection cpl_net_midp MIDP 2.0
 * <p>Compliant MIDP 2.0 implementations:
 * <ul>
 * <li>MUST give the user a visual indication of network usage
 * generated when using the mechanisms indicated in the MIDP 2.0
 * specification.
 * <li>MUST provide support for accessing HTTP 1.1 servers and
 * services either directly, or by using gateway services such as
 * provided by WAP or i-mode.
 * <li>MUST provide support for secure HTTP connections (HTTPS) either
 * directly, or by using gateway services such as provided by WAP or
 * i-mode.
 * <li>SHOULD provide support for datagram connections (UDP).
 * <li>SHOULD provide support for socket stream connections (TCP).
 * <li>SHOULD provide support for server socket stream connections
 * (TCP, server sockets).
 * </ul>
 * <p>In other words, HTTP[S] connections are required in MIDP 2.0.
 * They can be implemented either over TCP sockets
 * (option 1) or over a native HTTP client interface (option 2).<p>
 * If the porting contract specified option 1, this cpl_net.h file
 * defines the required functions.  The porting engineer must
 * implement the functions of this file required for socket stream
 * connections support (see function documentation).<p>
 * If the porting contract specified option 2, it must also list the
 * MIDP 2.0 optional protocols requested (if any); this cpl_net.h file
 * then defines the functions for optional MIDP 2.0 protocols.  The
 * porting engineer's job is to implement the corresponding functions
 * required to support a given protocol (see function documentation).
 * \sa cpl_http.h
 *
 * \subsection cpl_net_cdc CDC 1.1
 * <p>Compliant CDC 1.1 implementations:
 * <ul>
 * <li>MUST provide support for datagram connections (UDP).
 * <li>MUST provide support for socket stream connections (TCP).
 * <li>MUST provide support for server socket stream connections
 * (TCP, server sockets).
 * </ul>
 *
 * <p>
 * A port must provide the extra functions and socket options enabled
 * by JBED_EXTENDED_NET_SUPPORT, which includes some function variants
 * that take timeouts. During the early stages of a port, it is possible
 * to ignore the timeouts (treating them as 0, i.e. infinite). But full
 * timeout semantics must be implemented before release.
 *
 * <p>
 * The non-timeout version can be trivially implemented by calling the
 * WithTimeout version,passing a timeout of 0 (infinite).
 *
 * <p>
 * <!-- #interface list begin -->
 * \section cpl_net_interface Interface
 * - Case cldc TCP client sockets (including HTTP[S] connections implemented
 *   over TCP sockets)
 *    - CPL_net_startup()
 *    - CPL_net_shutdown()
 *    - CPL_net_getHostByName()
 *    - CPL_net_clientSocket()
 *    - CPL_net_recv()
 *    - CPL_net_send()
 *    - CPL_net_socketClose()
 *    - CPL_net_socketCloseNow()
 *    - CPL_net_setSockOpt()
 *    - CPL_net_getSockOpt()
 *    - CPL_net_getAvailable()
 *    - CPL_net_getSockName()
 * - Case TCP server sockets: In addition to the same functions as
 *                            for TCP client sockets:
 *    - CPL_net_serverSocket()
 *    - CPL_net_accept()
 * - Case UDP datagram sockets:
 *    - CPL_net_startup()
 *    - CPL_net_shutdown()
 *    - CPL_net_getHostByName()
 *    - CPL_net_datagramSocket()
 *    - CPL_net_socketClose()
 *    - CPL_net_socketCloseNow()
 *    - CPL_net_recvFrom()
 *    - CPL_net_sendTo()
 * - Case native network parameters:
 *    - CPL_net_handleNativeNetworkParams()
 *
 * \subsection extended networking interface
 * - Case CDC with MIDP (all of the above, plus the following)
 *    - CPL_net_getHostByAddr()
 *    - CPL_net_getFullHostByName()
 *    - CPL_net_socket()
 *    - CPL_net_bind()
 *    - CPL_net_listen()
 *    - CPL_net_connect()
 *    - CPL_net_acceptWithTimeout()
 *    - CPL_net_recvWithTimeout()
 *    - CPL_net_recvFromWithTimeout()
 *    - CPL_net_peekDataWithTimeout()
 *    - CPL_net_changeGroups()
 *   In addition, there are several additional socket options that must be
 *   supported.
 *
 * - Case CDC without MIDP
 *    - For the functions that have a WithTimeout version, the non-timeout
 *    version is not required.
 * <!-- #interface list end -->
 *
 */

#ifndef _CPL_NET_H_
#define _CPL_NET_H_

#include <jbed_global.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Maximum hostname length in a DNS query.
 * Also defined in NetworkConnectionBase.java as
 * STATIC_javax_microedition_io_NetworkConnectionBase_MAX_HOST_LENGTH
 */
#define MAX_HOST_LENGTH (256)

/**
 * The number of queued connections to allow for server sockets.
 * An implementation is free to use a more convenient value.
 */
#define CPL_NET_DEFAULT_BACKLOG (3)

/**
 * An invalid socket handle. No socket creation code will ever return
 * this handle. Code should not make any assumption that socket creation
 * functions will set returned socket handles to this value in the case
 * of an error. This value is intended solely for use as a default
 * 'not valid' value.
 *
 * Ports may override this default value with a more suitable one for
 * their platform.
 */
#ifndef CPL_NET_INVALID_SOCKET
#define CPL_NET_INVALID_SOCKET (-1)
#endif

/* =================================================================== */
/*                  SOCKETS API                                        */
/* =================================================================== */

/**
 * @name Socket options
 * @see CPL_net_setSockOpt()
 * @see CPL_net_getSockOpt()
 *
 * @internal
 * NOTE:  These values must match the values in SocketConnection.java
 * We do not use the static finals directly, since they only exist on midp 2
 * builds, and we require the symbols to be available on all variants.
 */
/*@{*/
/** TCP delay. */
#define CPL_NET_TCP_DELAY 0
/** Socket linger. */
#define CPL_NET_SO_LINGER 1
/** TCP Keep Alive. */
#define CPL_NET_SO_KEEPALIVE 2
/** Receive buffer. */
#define CPL_NET_SO_RCVBUF 3
/** Send buffer. */
#define CPL_NET_SO_SNDBUF 4

#ifdef JBED_EXTENDED_NET_SUPPORT
/** Reuse addr. */
#define CPL_NET_SO_REUSEADDR 5
/** Traffic class (type of service). */
#define CPL_NET_SO_TOS 6
/** Broadcast */
#define CPL_NET_SO_BROADCAST 7
/** The interface to send multicast packets from. */
#define CPL_NET_SO_MULTICAST_IF 8
/** Whether multicast packets are also recieved by the sending interface. */
#define CPL_NET_SO_MULTICAST_LOOP 9
/** Receive Out Of Band data inline */
#define CPL_NET_SO_OOBINLINE 10
#endif /* JBED_EXTENDED_NET_SUPPORT */
/*@}*/

/**
 * @name Constant return codes.
 */
/*@{*/
/** Successful operation. */
#define CPL_NET_SUCCESS            0
/** General failure. */
#define CPL_NET_ERROR             (-1)
/** Connection refused. */
#define CPL_NET_CONNREFUSED       (-2)

/** Operation will be handled asynchronously */
#define CPL_NET_WOULDBLOCK        (-100)

#ifdef JBED_EXTENDED_NET_SUPPORT

/**
 * May be returned by some cpl functions (currently peekDataWithTimeout())
 * to ask the base java to implement the functionality a less efficient way.
 */
#define CPL_NET_UNSUPPORTED       (-101)

/** Operation timed out */
#define CPL_NET_TIMEOUT           (-102)

#endif /* JBED_EXTENDED_NET_SUPPORT */

/*@}*/

/**
 * @name parameters for CPL_net_shutdownSocket.
 */
/*@{*/
#define CPL_NET_SHUTDOWN_READ   1
#define CPL_NET_SHUTDOWN_WRITE  2
#define CPL_NET_SHUTDOWN_RDWR   3

/*@}*/

#ifdef JBED_EXTENDED_NET_SUPPORT
/**
 * @name Socket type constants
 * Note that these constants have been arranged to be a bitfield
 * with the bottom nibble containing the primary type and the next
 * nibble containing subtype.
 */
/*@{*/

/** An IPV4 TCP socket */
#define CPL_NET_SOCKTYPE_TCP 0
/** An IPV4 UDP socket */
#define CPL_NET_SOCKTYPE_UDP 1

/** An IPV4 TCP client socket (only used for socket create) */
#define CPL_NET_SOCKTYPE_TCPCLIENT 0x10
/** An IPV4 TCP server socket (only used for socket create) */
#define CPL_NET_SOCKTYPE_TCPSERVER 0x20
/*@}*/

#endif /* JBED_EXTENDED_NET_SUPPORT */

#ifdef NETWORK_PROFILE_IN_JAD
/**
* set Network profile to porting layer,
* which will be used to create network connection later
* @param profile, name of the profile to be used by network connection
* @param profilelen number of the characters in profileName
* @return NULL;
*/
int CPL_net_setProfile(const uint16_t *profile, uint32_t profilelen);
#endif /*NETWORK_PROFILE_IN_JAD*/

/**
 * Initialize the networking module.  This includes initializing the
 * networking library and start up the networking stack if necessary.
 * Therefore, if a network attach procedure, such as a GPRS attach,
 * is required, it is peformed at this time.
 *
 * @return #CPL_NET_SUCCESS on success (or if the initialisation has
 *         already been done);
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_startup(void);


/**
 * Terminate and cleanup the networking module.
 * Any outstanding connection will be closed, as in
 * CPL_net_socketClose().  Therefore, any system resource associated
 * with a connection will be released.
 * Any system resource associated with the networking module itself
 * will also be released.  Moreover, this function also shuts down the
 * networking stack if necessary.  Therefore, if a network detach
 * procedure, such as a GPRS detach, is required, it is performed at
 * this time.
 *
 * NOTE: This function must complete synchronously and must not call
 * any of the ASYNC... functions.
 */
void CPL_net_shutdown(void);

/**
 * Get host by name.
 *
 * @param name Hostname to lookup.
 * @param namelen number of characters in name.
 * @param addr Where to put the IP address.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error (eg invalid address).
 */
int CPL_net_getHostByName(const uint16_t *name, uint32_t namelen,
                          uint32_t *addr);

/**
 * Get name of the local machine.
 * Optional: if supported, CPL_net_gethostname() can be used to
 * provide the "microedition.hostname" System property.

 * @param name where to store the local machine name.
 * @param namelen size of the name array to be filled, including
 * terminating zero. Updated to contain the size written.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_getHostName(uint16_t *name, int32_t *namelen);


/**
 * Create a client socket.
 * Allocate a socket descriptor and establish a TCP connection to the
 * given address and port.
 *
 * NOTE: connections typically take time to complete. If this call returns
 * CPL_NET_WOULDBLOCK, and the VM is reset before the connect completes,
 * the value last stored in *sock will be passed to CPL_net_socketCloseNow().
 * The port might choose to store some marker value such as
 * CPL_NET_INVALID_SOCKET if it needs to detect this case.
 *
 * @param addr IP address to connect to.
 * @param port port to connect to.
 * @param secure the socket will be used for a secure connection.
 * @param sock where to put the socket handle. Any values written will
 *        be preserved across WOULDBLOCK calls.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_CONNREFUSED if the connection is refused;
 *         #CPL_NET_ERROR on any other error.
 */
int CPL_net_clientSocket(uint32_t addr, uint16_t port, bool_t secure,
                         int32_t *sock);

/**
 * Create a datagram socket.
 * Allocate a socket descriptor and, if port is > 0, bind to the given
 * port on all local addresses.
 * The socket should be marked as non-blocking if the implementation supports
 * it.
 *
 * NOTE: If this call returns CPL_NET_WOULDBLOCK, and the VM is reset before
 * the operation completes, the value last stored in *sock will be passed
 * to CPL_net_socketCloseNow(). The port might choose to store some marker
 * value such as CPL_NET_INVALID_SOCKET if it needs to detect this case.
 *
 * @param port port to bind to (0 to allocate a free local port).
 * @param sock where to put the socket handle.  Any values written will
 *        be preserved across WOULDBLOCK calls.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_datagramSocket(uint16_t port, int32_t *sock);

/**
 * Create a server socket.
 * Allocate a socket descriptor and listen for connection to the given port
 * on all local addresses.
 * The socket should be marked as non-blocking if the implementation supports
 * it. It should also be marked as having a reusable address.
 *
 * NOTE: If this call returns CPL_NET_WOULDBLOCK, and the VM is reset before
 * the operation completes, the value last stored in *sock will be passed
 * to CPL_net_socketCloseNow(). The port might choose to store some marker
 * value such as CPL_NET_INVALID_SOCKET if it needs to detect this case.
 *
 * @param port port to listen for connections on (0 to allocate a free local
 * port).
 * @param sock where to put the socket handle. Any values written will
 *        be preserved across WOULDBLOCK calls.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_serverSocket(uint16_t port, int32_t *sock);

/**
 * Send some data.
 *
 * @param sock Socket descriptor.
 * @param buf Data to send.
 * @param num Number of bytes to send.
 * @return Number of bytes sent on success;
 *         #CPL_NET_WOULDBLOCK if no bytes were sent and the operation will
 *                             be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_send(int32_t sock, const char *buf, int num);


/**
 * Receive a network message or some data.
 *
 * @param sock Socket descriptor.
 * @param buf Buffer to store the message.
 * @param num Maximum number of bytes to receive.
 * @return Number of bytes received on success;
 *         0 if EOF has been reached;
 *         #CPL_NET_WOULDBLOCK if no bytes were received and the operation will
 *                             be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_recv(int32_t sock, char *buf, int num);


/**
 * Accept and extract the first connection request on the
 * queue of pending connections, create a new socket with
 * the same properties as sock, and allocate a new file
 * descriptor for the socket.
 * If no pending connections are present on the queue, and the
 * socket is marked non-blocking, accept returns #CPL_NET_WOULDBLOCK,
 * as described below.
 * The socket returned by accept may not be used to accept more connections.
 * The original socket sock remains open.
 *
 * @param sock a socket that has been created with CPL_net_serverSocket().
 * @param addr Where to store the IP address of the connecting entity.
 * @param port Where to store the port of the connecting entity.
 * @param sockOut Where to store the socket descriptor for the new connection.
 * @return #CPL_NET_SUCCESS on success (immediately connected);
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_accept(int32_t sock, uint32_t *addr, uint16_t *port,
                   int32_t *sockOut);

/**
 * Receive a network message.
 *
 * @param sock Socket descriptor.
 * @param buf Buffer to store message.
 * @param buflen Maximum number of bytes to receive.
 * @param fromAddr Where to store the source ip address of the message.
 * @param fromPort Where to store the source port of the message.
 * @return Number of bytes received on success;
 *         0 if EOF has been reached;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_recvFrom(int32_t sock, char *buf, int buflen,
                     uint32_t *fromAddr, uint16_t *fromPort);

/**
 * Transmit a network message to another socket.
 *
 * @param sock Socket descriptor.
 * @param buf Buffer to send.
 * @param buflen Number of bytes to send.
 * @param toAddr the IP address to send the message to.
 * @param toPort the port to send the message to.
 * @return Number of bytes sent on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_sendTo(int32_t sock, const char *buf, int buflen,
                   uint32_t toAddr, uint16_t toPort);


/**
 * Close a socket. Any incoming data available for read will be discarded
 * and any system resource associated with the socket will be released.
 *
 * NOTE: The implementation must ensure that ASYNC_notify() is performed for
 * any outstanding calls which have returned CPL_NET_WOULDBLOCK.
 *
 * @param sock Socket descriptor.
 * @return #CPL_NET_SUCCESS on success (immediately closed);
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_socketClose(int32_t sock);

/**
 * Close a socket immediately.
 * Equivalent to CPL_net_socketClose() but can be called from finalize methods
 * or other places where async completion is not allowed.
 *
 * NOTE: This function must complete synchronously. It must not call any of
 * the ASYNC_callAgain... functions, but may call ASYNC_notify() to
 * unblock any pending operations.
 *
 * NOTE: The parameter may be the value stored in *sock by an incomplete call
 * to one of the functions CPL_net_clientSocket, CPL_net_serverSocket,
 * CPL_net_datagramSocket.
 *
 * @param sock Socket descriptor.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_socketCloseNow(int32_t sock);

/**
 * Set an option for the given socket.
 *
 * @param sock socket descriptor.
 * @param optname the option name.
 * @param optval the option value.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_setSockOpt(int32_t sock, int32_t optname,
                       int32_t optval);

/**
 * Get an option for the given socket.
 *
 * @param sock socket descriptor.
 * @param optname the option name.
 * @param optval where to store the option value.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_getSockOpt(int32_t sock, int32_t optname, int32_t *optval);


/**
 * Get the remote connection address info for the socket.
 *
 * @param sock Socket.
 * @param addr Where to store the IP address of the peer.
 * @param port Where to store the port of the peer.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_getPeerName(int32_t sock, uint32_t *addr, uint16_t *port);

/**
 * Get the number of bytes that can be read from sock without blocking.
 *
 * @param sock socket descriptor.
 * @param avail Where to store the result.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_getAvailable(int32_t sock, int32_t *avail);

/**
 * Shutdown part of the socket.
 *
 * Note: CPL_NET_SUCCESS should be returned if this functionality is currently
 *       unsupported. Returning CPL_NET_ERROR would trigger IOExceptions on
 *       stream closure which generally worse than silently not unblocking any
 *       readers
 *
 * @param sock socket descriptor.
 * @param how How the socket should be shutdown, can be CPL_NET_SHUTDOWN_READ,
 *  CPL_NET_SHUTDOWN_WRITE, or CPL_NET_SHUTDOWN_RDWR.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_shutdownSocket(int32_t sock, int32_t how);

/**
 * Get the connection address and port for the socket.
 *
 * Note: This must always return a real IP address, never 0.
 *       If the socket is listening on multiple local interfaces then pick
 *       one (probably on the path to your default route) and return its IP
 *       address.
 *
 * @param sock Socket to get info about.
 * @param addr Where to store the IP address of the socket.
 * @param port Where to store the port of the socket.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_getSockName(int32_t sock, uint32_t *addr, uint16_t *port);

void Jbed_conn_startPDPActive(void);

void Jbed_conn_startPDPDeactive(void);

void JPL_net_notifyNetworkDown(void);

#ifdef JBED_EXTENDED_NET_SUPPORT

/**
 * Get full list of host addresses for a name.
 *
 * @param name Hostname to lookup.
 * @param namelen number of characters in name.
 * @param addr Array to put IP addresses into or NULL to just read how
 *             many addresses there are.
 * @param addrCount The size (in words) of the addr array.
 * @return number of IP addresses available (which may be more than could be
 *         written to the addr array);
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error (eg invalid address).
 */
int CPL_net_getFullHostByName(const uint16_t *name, uint32_t namelen,
                          uint32_t *addr, int32_t addrCount);


/**
 * Get name of host with the given address.
 *
 * @param name where to store the local machine name.
 * @param namelen size (in bytes) of the name array to be filled. Updated to
 *                contain the number of characters written.
 * @param addr the IP address to look up.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_getHostByAddr(uint16_t *name, uint32_t *namelen, uint32_t addr);

/**
 * Create a raw, unbound, unconnected socket of the given type.
 *
 * @param type CPL_NET_SOCKTYPE_TCPCLIENT, CPL_NET_SOCKTYPE_TCPSERVER or
 *             CPL_NET_SOCKTYPE_UDP.
 * @param sock updated to contain the socket handle
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_socket(int type, int32_t *sock);

/**
 * Bind an unbound and unconnected socket to the given local address.
 *
 * @param sock Socket descriptor.
 * @param type the type of socket being bound.
 * @param addr IP address to bind to (0 will bind to all local addresses).
 * @param port port to bind to (0 to allocate a free local port).
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_bind(int32_t sock, int type, uint32_t addr, uint16_t port);

/**
 * Start a server socket listening.
 *
 * May only be called on a socket that was created as a serversocket.
 *
 * @param sock Socket descriptor.
 * @param count The number of pending connections to allow.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_listen(int32_t sock, int count);

/**
 * Connect an unconnected socket to the given address and port.
 *
 * @param sock Socket descriptor.
 * @param type the type of socket being connected.
 * @param addr IP addreess to connect to.
 * @param port port to connect to.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_connect(int32_t sock, int type, uint32_t addr, uint16_t port);

/**
 * Receive a network message or some data.
 *
 * @param sock Socket descriptor.
 * @param buf Buffer to store the message.
 * @param num Maximum number of bytes to receive.
 * @param timeout Timeout in milliseconds for when to abort this operation,
 *                or 0 for no timeout.
 * @return Number of bytes received on success;
 *         0 if EOF has been reached;
 *         #CPL_NET_WOULDBLOCK if no bytes were received and the operation will
 *                             be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_recvWithTimeout(int32_t sock, char *buf, int num, int32_t timeout);




/**
 * Accept and extract the first connection request on the queue of pending
 * connections, create a new socket with the same properties as sock, and
 * allocate a new file descriptor for the socket. If no pending connections
 * are present on the queue, return #CPL_NET_WOULDBLOCK and watch the
 * queue in the background.
 * The socket returned by accept may not be used to accept more connections.
 * The original socket sock remains open.
 *
 * @param sock a socket that has been created with CPL_net_serverSocket().
 * @param addr Where to store the IP address of the connecting entity.
 * @param port Where to store the port of the connecting entity.
 * @param sockOut Where to store the socket descriptor for the new connection.
 * @param timeout Timeout in milliseconds for when to abort this operation,
 *                or 0 for no timeout.
 * @return #CPL_NET_SUCCESS on success (immediately connected);
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_acceptWithTimeout(int32_t sock, uint32_t *addr, uint16_t *port,
                              int32_t *sockOut, int32_t timeout);

/**
 * Receive a network message.
 *
 * @param sock Socket descriptor.
 * @param buf Buffer to store message.
 * @param buflen Maximum number of bytes to receive.
 * @param fromAddr Where to store the source ip address of the message.
 * @param fromPort Where to store the source port of the message.
 * @param timeout Timeout in milliseconds for when to abort this operation,
 *                or 0 for no timeout.
 * @return Number of bytes received on success;
 *         0 if EOF has been reached;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_recvFromWithTimeout(int32_t sock, char *buf, int buflen,
                                uint32_t *fromAddr, uint16_t *fromPort,
                                int32_t timeout);

/**
 * Receive a network message, but leave it in the message queue.
 *
 * @param sock Socket descriptor.
 * @param buf Buffer to store message.
 * @param buflen Maximum number of bytes to receive.
 * @param fromAddr Where to store the source ip address of the message.
 * @param fromPort Where to store the source port of the message.
 * @param timeout Timeout in milliseconds for when to abort this operation,
 *                or 0 for no timeout.
 * @return Number of bytes received on success;
 *         0 if EOF has been reached;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error;
 *         #CPL_NET_UNSUPPORTED if java should emulate using recvFrom()
 */
int CPL_net_peekDataWithTimeout(int32_t sock, char *buf, int buflen,
                                uint32_t *fromAddr, uint16_t *fromPort,
                                int32_t timeout);

/**
 * Change the multicast group membership for a multicast socket.
 *
 * @param sock socket descriptor.
 * @param addr the address of the group to join or leave.
 * @param join JBNI_TRUE to join the given group, otherwise leave it.
 * @return #CPL_NET_SUCCESS on success;
 *         #CPL_NET_WOULDBLOCK if the operation will be handled asynchronously;
 *         #CPL_NET_ERROR on error.
 */
int CPL_net_changeGroups(int32_t sock, uint32_t addr, bool_t join);


#endif /* JBED_EXTENDED_NET_SUPPORT */


/* ========== END OF SOCKET APIS ========== */

#ifdef AMS_NETWORK_PARAMETERS_SUPPORT
#ifdef AMS_NATIVE_NETWORK_PARAMETERS

/**
 * Handle setting/getting of native network parameters.
 *
 * @return One of the CPL_NET_* error codes.
 */
int CPL_net_handleNativeNetworkParams(void);

#endif /* AMS_NATIVE_NETWORK_PARAMETERS */
#endif /* AMS_NETWORK_PARAMETERS_SUPPORT */

#if defined(__cplusplus)
}
#endif

#endif /* _CPL_NET_H_ */
