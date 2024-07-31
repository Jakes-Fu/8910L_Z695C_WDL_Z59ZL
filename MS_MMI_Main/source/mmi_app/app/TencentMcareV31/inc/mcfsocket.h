/******************************************************************************/
//File Name:        McfSocket.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide simple socket interface for mcare
//Modifed:          wilsonzhu 01/10/2011
/******************************************************************************/
#ifndef _MCFSOCKET_H_
#define _MCFSOCKET_H_ 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/ 
#include "mcfcommon.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_Create
//Descrption:����socket
//Param:
//  IN:pUser:sokcet�ĸ�������
//  notifyHandler:�ص�����
//  OUT:MSocHandle
//Return: > = 0�Ϸ�
//Note:
//----------------------------------------------------------------------------
MSocHandle McfSoc_Create (MVOID* pUser, pfnMcfSocNotify notifyHandler);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_Close
//Descrption:�ر�socket
//Param:
//  IN:socHd:socket�ľ��
//  OUT:MINT
//Return: MINT
//Note:
//----------------------------------------------------------------------------
MINT McfSoc_Close(MSocHandle socHd);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_Connect
//Descrption:����socket
//Param:
//  IN:socHd:socket�ľ��
//  addr:IP��ַ
//  port:�˿�
//  OUT:MINT
//Return: MINT
//Note:
//----------------------------------------------------------------------------
MINT McfSoc_Connect(MSocHandle socHd, const MINT addr, MUSHORT port);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_ConnectByName
//Descrption:ʹ��name��������
//Param:
//  IN:socHd:socket�ľ��
//  pName:��ַ
//  port:�˿�
//  OUT:MINT
//Return: MINT
//Note:
//----------------------------------------------------------------------------
MINT McfSoc_ConnectByName(MSocHandle socHd, const MCHAR* pName, MUSHORT port);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_Send
//Descrption:socket������Ϣ
//Param:
//  IN:socHd:socket�ľ��
//  pBuf:����
//  len:����
//  OUT:MINT
//Return: MINT����������������·��س���
//Note:
//----------------------------------------------------------------------------
MINT McfSoc_Send(MSocHandle socHd, MBYTE* pBuf, MINT len);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_Recv
//Descrption:socket������Ϣ
//Param:
//  IN:socHd:socket�ľ��
//  pBuf:����
//  len:����
//  OUT:MINT
//Return: MINT����������������·��س���
//Note:
//----------------------------------------------------------------------------
MINT McfSoc_Recv(MSocHandle socHd, MBYTE* pBuf, MINT len);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_GetHostByName
//Descrption:ʹ�õ�ַ����IP
//Param:
//  IN:pUser:����
//  pName:��ַ
//  pAddr:���ص�IP
//  pAddrLen:���صĳ���
//  hostNotifyHandler:�ص�����
//  pReqId:������
//  OUT:MINT
//Return: MINT
//Note:
//----------------------------------------------------------------------------
MINT McfSoc_GetHostByName(MVOID* pUser, const MCHAR* pName,MCHAR* pAddr, MINT* pAddrLen, 
                                        pfnMcfDnsNotify hostNotifyHandler, MINT* pReqId);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_CancelGetHostByName
//Descrption:ȡ��ʹ�õ�ַ����IP
//Param:
//  IN:requestId:������
//  MINT:MINT
//Return: MINT
//Note:
//----------------------------------------------------------------------------
MINT McfSoc_CancelGetHostByName(MINT requestId);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_ntohl
//Descrption:
//Param:
//  IN:x:
//Return: MUINT
//Note:
//----------------------------------------------------------------------------
MUINT McfSoc_ntohl(MUINT x);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_ntohs
//Descrption:
//Param:
//  IN:x:
//Return: MUSHORT
//Note:
//----------------------------------------------------------------------------
MUSHORT McfSoc_ntohs(MUSHORT x);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_ntohs
//Descrption:
//Param:
//  IN:x:
//Return: MUINT
//Note:
//----------------------------------------------------------------------------
MUINT  McfSoc_htonl(MUINT x);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_ntohs
//Descrption:
//Param:
//  IN:x:
//Return: MUSHORT
//Note:
//----------------------------------------------------------------------------
MUSHORT McfSoc_htons(MUSHORT x);

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
//---------------------------------------------------------------------------
//FUNCTION: MSOCKET_SetNetId
//Descrption:
//Param:
//  IN:netId:
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID MSOCKET_SetNetId(MUINT netId);
#endif

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_Init
//Descrption:Scoket�ĳ�ʼ��
//Param:
//  IN:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfSoc_Init(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_Deinit
//Descrption:Scoket�ķ���ʼ��
//Param:
//  IN:MVOID
//Return: MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfSoc_Deinit(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfSoc_CloseAll
//Descrption:�ر����е�socket
//Param:
//  IN:MVOID
//  OUT:MINT
//Return:
//Note:
//----------------------------------------------------------------------------
MINT McfSoc_CloseAll(MVOID);


//---------------------------------------------------------------------------
//FUNCTION: McfSoc_IpCheck
//Descrption:���IP
//Param:
//  IN:MVOID
//  OUT:MBOOL
//Return:
//Note:
//----------------------------------------------------------------------------
//MBOOL McfSoc_IpCheck(MCHAR* asci_addr, MCHAR* ip_addr, MBOOL* ip_validity);
MBOOL McfSoc_IpCheck(MCHAR * asci_addr, MBYTE* ip_addr);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif//_MCFSOCKET_H_

