/*=====================================================================================
FILE NAME   : eme_socket_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide api for communicating with server.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02       Daniel                     create eme_socket_api.h
          
=====================================================================================*/

#ifndef _EME_SOCKET_API_H_
#define _EME_SOCKET_API_H_


/* Header Files*/
//#include "AEEShell.h"            /* Shell interface definitions */
//#include "AEENet.h"               /* Socket interface definitions */
//#include "AEEWebOpts.h"
//#include "AEESSL.h"              /* For Supporting SSL */
#include "we_def.h"
/*****************************************************************************/

/* Define */

#define EME_MAX_DATAWRITE_LEN    10240

/*SSL Type*/
#define EME_SSL_TYPE_SSLTLS                  0x00
#define EME_SSL_TYPE_STARTTLS                0x01
#define EME_SSL_TYPE_NONE                    0x02

/*Authenticate type*/
#define EME_AUTHENTICATE_TYPE_NONE           0x00
#define EME_AUTHENTICATE_TYPE_NAME_PWD       0x01
#define EME_AUTHENTICATE_TYPE_SSL            0x02
#define EME_AUTHENTICATE_TYPE_NAME_PWD_SSL   0x03

/*****************************************************************************/

typedef WE_VOID (*FUNCCONNECTIONOK)(WE_VOID*);
typedef WE_VOID (*FUNCREADABLE)(WE_VOID*);   
typedef WE_VOID (*FUNCWRITEABLE)(WE_VOID*);
typedef WE_VOID (*FUNCERRORHANDLER)(WE_VOID*, WE_INT32);

typedef WE_VOID* HSocket;

/*****************************************************************************/

/* reg needed call back */
WE_VOID EME_Socket_RegCbConnectionOk(HSocket pvSocket, WE_VOID* pvCallback);
WE_VOID EME_Socket_RegCbReadable(HSocket pvSocket, WE_VOID* pvCallback);
WE_VOID EME_Socket_RegCbWriteable(HSocket pvSocket, WE_VOID* pvCallback);
WE_VOID EME_Socket_RegCbErrHandler(HSocket pvSocket, WE_VOID* pvCallback);

/* we need IShell obj to create instance about network, so pObj do this */
HSocket EME_Socket_Create(WE_VOID* piShell,     /* the handle of Shell */
                          WE_VOID* piAsyncMgr,
                          WE_VOID* piNetMgr,
                          WE_VOID* pvUser, 
                          WE_VOID* pvMemMgr, 
                          WE_INT32 iSSLType);    /* whether support SSL? yes, TRUE; no, FALSE */
WE_INT32 EME_Socket_Close(HSocket pvSocket);
WE_INT32 EME_Socket_Connection(HSocket pvSocket, 
                               const WE_CHAR *pcHostName, 
                               WE_UINT16 usPort);
WE_INT32 EME_Socket_Write(HSocket pvSocket,
                          WE_CHAR *pcBuffer,
                          WE_UINT32 uiDataSize,
                          WE_BOOL bIsEnd);
WE_INT32 EME_Socket_Read(HSocket pvSocket, 
                         WE_CHAR *pcBuffer,
                         WE_UINT32 uiBufLen,
                         WE_UINT32 *puiReadSize);

WE_VOID EME_Socket_StartTls(HSocket pvSock);
   
/*****************************************************************************/



#endif /* _EME_SOCKET_API_H_ */