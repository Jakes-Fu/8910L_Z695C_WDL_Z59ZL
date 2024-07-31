/*****************************************************************************
** File Name:      qsocket.h                                                  *
** Author:                                                                   *
** Date:             11/05/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare QQ socket function and define global               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010       fen.xie       Create
******************************************************************************/
#ifndef _QQ_SOCKET_H_
#define _QQ_SOCKET_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define AEECLSID_NET	0x01001000+46
#define AEE_NET_SUCCESS          (0)   //successful operation
#define AEE_NET_ERROR            (-1)   //unsuccessful operation
#define AEE_NET_WOULDBLOCK       (-2)

#define AEEDNSMAXADDRS           4


#define AEE_htons(a)    ((((a&0xFF00)>>8)|((a&0x00FF)<<8))&0x0000FFFF)


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef unsigned long INetMgr;
typedef unsigned long INAddr;

typedef void (*ConnectCB)(void* pData, int Err);
typedef void (*WriteCB)(void* pData);
typedef void (*ReadCB)(void* pData);

typedef struct _QSocket
{
    int				 socketid;
    ConnectCB             pfnconnect;
    WriteCB                 pfnwrite;
    ReadCB			 pfnRead;

    struct _QSocket*  pNext;
    int			       iState;
    void* 			pUser;
    int             retry;
    unsigned long        ip;
    unsigned short       port;
}ISocket;

typedef struct
{
    ISocket*	       head;
    int				   size;
    int		           gprs_status;
    int                retry_cnt;
}Sockets;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: QSOCKET_Init
//  Global resource dependence : GSockets 
//  Author: 
//  Note: 
/*****************************************************************************/
void QSOCKET_Init(void);
/*****************************************************************************/
//  Description: QSOCKET_Free
//  Global resource dependence :  GSockets
//  Author: 
//  Note: 
/*****************************************************************************/
void QSOCKET_Free(void);
/*****************************************************************************/
//  Description: QSOCKET_Insert
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QSOCKET_Insert(ISocket* socket_ptr);
/*****************************************************************************/
//  Description: QSocket_Remove
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QSocket_Remove(ISocket* socket_ptr);

//  Description: ISOCKET_Writeable
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void ISOCKET_Writeable(ISocket* me_ptr, WriteCB func_ptr, void* user_ptr);

/*****************************************************************************/
//  Description: ISOCKET_Readable
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void ISOCKET_Readable(ISocket* me_ptr, ReadCB func_ptr, void* user_ptr);
/*****************************************************************************/
//  Description: QSOCKET_ConnectCB
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QSOCKET_ConnectCB(void* user_ptr);
/*****************************************************************************/
//  Description: QSOCKET_ConnectTimeOut
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QSOCKET_ConnectTimeOut(void* user_ptr);

/*****************************************************************************/
//  Description : deactive pdp for mms 
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
BOOLEAN QSOCKET_DeActiveGprs(void);//QSOCKET_PDPDeactive(void)
/*****************************************************************************/
//  Description: QSOCKET_ActiveGprsTimeOut
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QSOCKET_ActiveGprsTimeOut(void* user_ptr);
/*****************************************************************************/
//  Description : active pdp for QQ 
//  Global resource dependence : none
//  Author: fen.xie
//  Note:QSOCKET_PDPActive
/*****************************************************************************/
void QSOCKET_ActiveGprs(void* user_ptr);
/*****************************************************************************/
//  Description: QSCOKET_OnTimer
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QSCOKET_OnTimer(Sockets* head_sockets_ptr);
/*****************************************************************************/
//  Description: INETMGR_OpenSocket
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
ISocket* INETMGR_OpenSocket(INetMgr* net_mgr_ptr, int stream);
/*****************************************************************************/
//  Description: ISOCKET_Connect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void ISOCKET_Connect(ISocket* me_ptr, INAddr ip, unsigned short port, ConnectCB func, void* user_ptr);
/*****************************************************************************/
//  Description: ISOCKET_Read
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int	 ISOCKET_Read(ISocket* me_ptr, char* buf_ptr, int size);
/*****************************************************************************/
//  Description: ISOCKET_Write
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int  ISOCKET_Write(ISocket* me_ptr, char* buf_ptr, int size);

/*****************************************************************************/
//  Description: ISOCKET_Release
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void ISOCKET_Release(ISocket* me_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
