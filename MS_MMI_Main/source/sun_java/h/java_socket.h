#ifndef	JAVA_SOCKET_H
#define	JAVA_SOCKET_H

#include "javacall_defs.h"
#include "ffs.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
//{
#endif

#define SOCK_DBG javacall_printf
#define SOCK_INFO javacall_printf

#define SOCKETNULL  -1

#define DATAGRAM_RECVFROM_COMPLETED      (JAVACALL_EVENT_DATAGRAM_RECVFROM_COMPLETED*10+1)
#define DATAGRAM_SENDTO_COMPLETED        (JAVACALL_EVENT_DATAGRAM_SENDTO_COMPLETED*10+1)

#define SOCKET_OPEN_COMPLETED            (JAVACALL_EVENT_SOCKET_OPEN_COMPLETED*10+2)
#define SOCKET_CLOSE_COMPLETED           (JAVACALL_EVENT_SOCKET_CLOSE_COMPLETED*10+2)
#define SOCKET_RECEIVE                   (JAVACALL_EVENT_SOCKET_RECEIVE*10+2)
#define SOCKET_SEND                      (JAVACALL_EVENT_SOCKET_SEND*10+2)
#define SOCKET_REMOTE_DISCONNECTED       (JAVACALL_EVENT_SOCKET_REMOTE_DISCONNECTED*10+2)
#define NETWORK_GETHOSTBYNAME_COMPLETED  (JAVACALL_EVENT_NETWORK_GETHOSTBYNAME_COMPLETED*10+2)
#define NETWORK_DOWN                     (JAVACALL_NETWORK_DOWN*10+3)
#define NETWORK_UP                       (JAVACALL_NETWORK_UP*10+3)
#define NETWORK_DOWN_REQUEST             (JAVACALL_NETWORK_DOWN_REQUEST*10+3)

typedef enum {
    GOOD,
    BAD,
    REACTIVATING
} E_NET_STATUS; 

typedef enum {
    NONE_BLOCED,
    DATAGRAM_RECVFROM_BLOCED,
    DATAGRAM_SENDTO_BLOCED,
    SOCKET_OPEN_BLOCED,
    SOCKET_CLOSE_BLOCED,
    SOCKET_READ_BLOCED,
    SOCKET_WRITE_BLOCED,
    NETWORK_GETHOSTBYNAME_BLOCED,
} java_socket_blocking_type;

/*最大socket蕐*/
#define MAX_SOCKET_NUM  64


/*socket数据结构*/
typedef struct
{
	WORD nPort;
	long nSocket;
	int nType;       //1: tcp or 2 :udp
    int block;
    int blockResult;
}T_Socket;


int32 java_GetSocketList(int32 s, int32 port,  int32 socket_type);
int32 java_FindSocketList(int32 s);
int32 java_SocketListSetBlock(int32 s, int32 type);
int32 java_SocketListClearBlock(int32 s);
int32 java_DelSocketListBySocket(int32 s);
int32 java_DelSocketListByIndex(int32 index);
int32 java_GetSocketTypeBySocket(int32 s);
uint32 java_SocketListNotifyAll(void);

PUBLIC int java_NetConnect(void);
E_NET_STATUS java_NetStatus(void);
PUBLIC int java_Do_NetConnect(void);
PUBLIC int32 java_Do_NetDisconnect(void);



/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
//}
#endif

#endif  /* JAVA_SOCKET_H */
