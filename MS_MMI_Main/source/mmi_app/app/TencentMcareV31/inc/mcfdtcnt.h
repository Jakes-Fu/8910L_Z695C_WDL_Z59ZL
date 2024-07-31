/******************************************************************************/
//File Name:        McfDtcnt.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Modifed:          nicklei 09/21/2011
//Modifed:          wiltan 10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide simple data account interface for mcare
/******************************************************************************/
#ifndef _MCFDTCNT_H_
#define _MCFDTCNT_H_

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                        Include Files                                       */
/*----------------------------------------------------------------------------*/

#include "mcfcommon.h"



/*----------------------------------------------------------------------------*/
/*                       Macro Declaration    							      */
/*----------------------------------------------------------------------------*/
#define MCARE_ACTIVE_GPRS_TIMES (5)

/*----------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                  */
/*----------------------------------------------------------------------------*/
//未知接入点
const static MWCHAR unknownapn[]={0x672a,0x77e5,0x63a5,0x5165,0x70b9,0x0};

//接入点列表
#define MCF_APN_CMWAP		L"cmwap"
#define MCF_APN_CMNET		L"cmnet"
#define MCF_APN_UNIWAP		L"uniwap"
#define MCF_APN_UNINET		L"uninet"
#define MCF_APN_CTWAP		L"ctwap"
#define MCF_APN_CTNET		L"ctnet"
#define MCF_APN_WIFI		L"wifi"
#define MCF_APN_UNKNOWN     (MWCHAR*)unknownapn

//卡名称
#define MCF_CARDNAME_CARD1		L"Card1"
#define MCF_CARDNAME_CARD2		L"Card2"
#define MCF_CARDNAME_CARD3		L"Card3"
#define MCF_CARDNAME_CARD4		L"Card4"
#define MCF_CARDNAME_WIFI		L"WIFI NET"

/*----------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                   */
/*----------------------------------------------------------------------------*/


MBOOL McfDtcnt_GetApnName(MBYTE simId, MBYTE apnType, MBYTE* apnName);


//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_OpenNetworkBearer
//	Descrption:     创建一个数据账户的链接
//	Param:
//	   	IN          pDataAcct:创建链接需要的数据帐号
//	Return:         
//                  MTRUE : success
//                  MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_OpenNetworkBearer(McfDtcntInfo *pDataAcct);

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_HoldNetworkBearer
//	Descrption:     保持一个数据账户的链接
//	Param:
//	   	IN          pDataAcct:保持链接需要的数据帐号
//	Return:         
//                  MTRUE : success
//                  MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_HoldNetworkBearer(McfDtcntInfo *pDataAcct);

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_CloseNetworkBearer
//	Descrption:     关闭一个数据账户的链接
//	Param:
//	   	IN          pDataAcct:关闭链接需要的数据帐号
//	Return:         
//                  MTRUE : success
//                  MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_CloseNetworkBearer(McfDtcntInfo* pDataAcct);

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_GprsActived
//	Descrption:     判断当前网络是否激活
//	Param:
//	   	NONE       
//	Return:         
//                  MTRUE : ACTIVED
//                  MFALSE : NOT ACTIVED
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_GprsActived(MVOID);

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_GprsActiving
//	Descrption:     判断当前网络是否正在激活
//	Param:
//	   	NONE       
//	Return:         
//                  MTRUE : ACTIVING
//                  MFALSE : NOT ACTIVING
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_GprsActiving(MVOID);

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_GprsActived
//	Descrption:     判断当前网络是否正在去激活
//	Param:
//	   	NONE       
//	Return:         
//                  MTRUE : DEACTIVING
//                  MFALSE : NOT DEACTIVING
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_GprsDeactiving(MVOID);


//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_GprsDeactived
//	Descrption:     判断当前网络是去激活成功，断开了GPRS
//	Param:
//	   	NONE       
//	Return:         
//                  MTRUE : DEACTIVING
//                  MFALSE : NOT DEACTIVING
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_GprsDeactived(MVOID);

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_GprsBlock
//	Descrption:     判断当前网络是激活多次都没有成功，就Block
//	Param:
//	   	NONE       
//	Return:         
//                  MTRUE : DEACTIVING
//                  MFALSE : NOT DEACTIVING
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_GprsBlock(MVOID);

//------------------------------------------------------------------------------
//	FUNCTION:       HandleMCarePsMsg
//	Descrption:     处理激活网络的协议处理消息函数
//	Param:
//	   	IN          msg_ptr:传入的pdp消息结构
//	Return:         
//                  VOID
//  Note:
//------------------------------------------------------------------------------
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G)\
    || (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
MVOID HandleMCarePsMsg(MVOID* pMsg_ptr);
#else
MUINT HandleMCarePsMsg(MVOID* app_ptr, MUSHORT msg_id, MVOID* param);
#endif


#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_ActiveWifi
//	Descrption:     激活WIFI
//	Param:
//	   	NONE       
//	Return:         
//                  MTRUE : SUCCESS
//                  MFALSE : FAIL
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_ActiveWifi(MVOID);

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_ActiveWifi
//	Descrption:     去激活WIFI
//	Param:
//	   	NONE       
//	Return:         
//                  MTRUE : SUCCESS
//                  MFALSE : FAIL
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_DeActiveWifi(MVOID);

//------------------------------------------------------------------------------
//	FUNCTION:       HandleMCareWifiMsg
//	Descrption:     处理WIFI的消息处理函数
//	Param:
//	   	IN          msg_ptr:传入的pdp消息结构
//	Return:         
//                  VOID
//  Note:
//------------------------------------------------------------------------------
MVOID HandleMCareWifiMsg(MVOID* pMsg_ptr);
#endif

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_GetNeedActiveGprs
//	Descrption:     获取是否需要激活GPRS的标志
//	Param:
//	   	NONE
//	Return:         
//                  MTRUE : NEED
//                  MFALSE : NOT NEED
//  Note:
//------------------------------------------------------------------------------
MBOOL McfDtcnt_GetNeedActiveGprs(MVOID);

//------------------------------------------------------------------------------
//	FUNCTION:       McfDtcnt_SetNeedActiveGprs
//	Descrption:     设置是否需要激活GPRS的标志
//	Param:
//	   	IN          need_active : BOOL的参数，标志
//	Return:         
//                  VOID
//  Note:
//------------------------------------------------------------------------------
MVOID McfDtcnt_SetNeedActiveGprs(MBOOL need_active);


MBOOL Mcare_ActiveGprs(MVOID);
MBOOL Mcare_DeactiveGprs(MVOID);

MINT McfDtcnt_EntrySelection(MVOID* pParam);

/**********************************
* McfDtcnt_GetProxyInfo ,get proxy info of current data account
*@pProxy --[out] proxy host server ip/name
*@nLen --[in]buffer len of pProxy
*@pPort --[out] proxy port number
*#return 1-- yes,use proxy
        0 -- no,connect through
        -1 --error
*/
MINT McfDtcnt_GetProxyInfo(MCHAR* pProxy, MINT nLen, MUSHORT* pPort);

MINT McfDtcnt_IsNetworkReady(MVOID);

MBOOL McfDtcnt_GetApnInfo(MApnInfo *pApnInfo,MINT simId);


MVOID McfDtcnt_Init(MVOID);

MVOID McfDtcnt_Deinit(MVOID);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_MCFDTCNT_H_
