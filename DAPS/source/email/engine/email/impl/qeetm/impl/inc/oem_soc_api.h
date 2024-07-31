/*==============================================================================
    FILE NAME   : eme_error.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        Provide basic error definition for email engine.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-03-02   Jiang Dingqing   01         Create
    2007-08-29   Wenkai Xu        02         Add new error code and modify

==============================================================================*/
#ifndef OME_SOC_API_H
#define OME_SOC_API_H

#include "we_def.h"


typedef WE_VOID*  HOEMSOCKETHANDLE;

#define OEMDNSMAXADDRS  4
typedef WE_UINT32 INetAddr;
typedef WE_UINT16 INetPort;

typedef WE_VOID (*PFNCONNECTCB)(WE_VOID * pUser, WE_INT nError);
typedef WE_VOID (*PFNNOTIFY)(WE_VOID *pUser);

typedef enum
{
	OEM_SOCK_EVT_NONE = 0,
	OEM_SOCK_EVT_GETHOSTNAME,
   OEM_SOCK_EVT_CONNECT,
   OEM_SOCK_EVT_READ,
	OEM_SOCK_EVT_WRITE,
	OEM_SOCK_EVT_CLOSE,
	OEM_SOCK_EVT_DISDIAL,
	OEM_SOCK_EVT_MAX
}OemSoketEvtType;

typedef enum
{
    OEM_TIMER_GETHOSTNAME = 0,
    OEM_TIMER_CONNECT,
    OEM_TIMER_RW,
    OEM_TIMER_MAX
}OemTimerType;


typedef enum 
{
   OEM_SOCK_STREAM = 1,
	OEM_SOCK_DGRAM,
	OEM_SOCK_MAX
}NetSocketType;

typedef struct
{  
   WE_INT16 sAddrLen;  
	WE_INT32 nResqId;
	WE_INT   nResult;
	INetAddr addrs[OEMDNSMAXADDRS];
} OEMDNSResult;

 

typedef struct tagSt_OemSockAddr{
    WE_INT16    sAddrLen;   /* IPv6 or IPv4 */
    WE_UINT8    aucAddr[16];   /* IPv6 available */
    WE_UINT16   usPort;     /* Port Number */
}St_OemSockAddr;

#define OEM_SOC_SUCCESS			         (0)
#define OEM_SOC_ERROR 			         (-1)
#define OEM_SOC_WOULDBLOCK		         (-2)
#define OEM_SOC_LIMIT_RESOURCE 	      (-3)	/* limited resource */
#define OEM_SOC_INVALID_SOCKET		   (-4) 	/* invalid socket */
#define OEM_SOC_INVALID_ACCOUNT	      (-5) 	/* invalid account id */
#define OEM_SOC_NAMETOOLONG			   (-6)  /* address too long */
#define OEM_SOC_ALREADY			         (-7)  /* operation already in progress */
#define OEM_SOC_OPNOTSUPP			      (-8)  /* operation not support */
#define OEM_SOC_CONNABORTED			   (-9)  /* Software caused connection abort */
#define OEM_SOC_INVAL			         (-10)	/* invalid argument */
#define OEM_SOC_PIPE			            (-11)	/* broken pipe */
#define OEM_SOC_NOTCONN			         (-12)	/* socket is not connected */
#define OEM_SOC_MSGSIZE			         (-13)	/* msg is too long */
#define OEM_SOC_BEARER_FAIL			   (-14)	/* bearer is broken */
#define OEM_SOC_CONNRESET              (-15)
#define OEM_SOC_TIMEOUT                (-16)

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
WE_INT32   OEM_CreateSocHandle(HOEMSOCKETHANDLE* ppOEMSocketHdl);
WE_VOID OEM_FreeSocHandle(HOEMSOCKETHANDLE* ppOEMSocketHdl);
WE_INT32 OEM_OpenSocket(HOEMSOCKETHANDLE phOEMSocketHdl, NetSocketType Type); 
WE_INT32 OEM_GetHostByName(HOEMSOCKETHANDLE phOEMSocketHdl, const char *pcHostName, OEMDNSResult *pres, PFNNOTIFY pcb, void *pUser);
WE_INT  OEM_Connect(HOEMSOCKETHANDLE phOEMSocketHdl, INetAddr uiIp, INetPort usPort, PFNCONNECTCB pfn, WE_VOID *pUser);
WE_INT  OEM_SocketWrite(HOEMSOCKETHANDLE phOEMSocketHdl, WE_UCHAR * pBuffer, WE_UINT uiBytes);
WE_VOID OEM_SocketWriteable(HOEMSOCKETHANDLE phOEMSocketHdl, PFNNOTIFY pfn, WE_VOID * pUser);
WE_INT  OEM_SocketRead(HOEMSOCKETHANDLE phOEMSocketHdl, WE_UCHAR * pBuffer, WE_UINT uiBytes);
WE_VOID OEM_SocketReadable(HOEMSOCKETHANDLE phOEMSocketHdl, PFNNOTIFY pfn, WE_VOID * pUser);
WE_INT32  OEM_CloseSocket(HOEMSOCKETHANDLE phOEMSocketHdl);
WE_INT32 OEM_SocketProcess(HOEMSOCKETHANDLE phOEMSocketHdl, WE_UINT32 uiEvtType, WE_VOID *pvData);


#endif /*OME_SOC_API_H*/
