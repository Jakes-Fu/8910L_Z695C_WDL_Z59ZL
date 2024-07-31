/*****************************************************************************
** File Name:      mmiphone_internal.h                                          *
** Author:                                                                   *
** Date:           01/2012                                                   *
** Copyright:      20121 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe phone powner on/off         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012        dave.ruan          Create                                    *
******************************************************************************/

#ifndef _MMIPHONE_INTERNAL_H_
#define _MMIPHONE_INTERNAL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmiphone_export.h"
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

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : power off MS
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_PowerOff(void);

/*****************************************************************************/
// 	Description : 设置sim card状态
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_SetSimStatus(MN_DUAL_SYS_E dual_sys, SIM_STATUS_E sim_status);

/*****************************************************************************/
// 	Description : Get Pin/PUK Remain string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_GetPinPukRemainStr(MN_DUAL_SYS_E dual_sys,
                                                                                    MMI_STRING_T *pin_puk_str_ptr,
                                                                                    MNSIM_PIN_INDICATION_E pin_ind,
                                                                                    BOOLEAN chv_is_blocked);

PUBLIC void MMIPHONE_GetPinPukRemainStrCustom(MN_DUAL_SYS_E dual_sys,
                                                                MMI_STRING_T *pin_puk_str_ptr,
                                                                MMI_OPERATE_PIN_T *operate_pin_ptr,
                                                                BOOLEAN chv_is_blocked);
PUBLIC void MMISET_DisplaybannerInfoPinPukErrorTryTimes(
        MMI_WIN_ID_T win_id,
        MMI_TEXT_ID_T text_id,
        MN_DUAL_SYS_E dual_sys,
        MNSIM_PIN_INDICATION_E     pin_num,
        BOOLEAN chv_is_blocked);
/*****************************************************************************/
// 	Description : open query power off 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_OpenQueryPowerOff(MMIPHONE_QUERY_POWER_OFF_TYPE_E query_type);

/*****************************************************************************/
// 	Description : create vodafone window
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_CreateStartupVodafoneSelectWindow(void);

/*****************************************************************************/
//  Description : 获取NDT关于vodafone需求的配置
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetVodafoneSimConfig(void);

/*****************************************************************************/
// 	Description : set PIN sys sim remain
//	Global resource dependence : 
//  Author: 
//	Note:因为新需求中添加了banner提示，如果在banner结束之后通过
//       MMIAPIPHONE_GetCurResponsePinSimID获取到的将是新sim了
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetPinBannerSysSimRemain(void);

/*****************************************************************************/
// 	Description : init vodafone nv
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_InitVodafoneSimSelectNV(void);

/*****************************************************************************/
// 	Description : GET vodafone select result
//	Global resource dependence : 
//  Author:
//	Note:返回值为0：无效；为1表示Vodafone UI Contract；为2表示Vodafone UI YAYG
//  返回值为1，vodafone_text中存储text为：Vodafone UK Contract
//  返回值为2，vodafone_text中存储text为：Vodafone UK YAYG
/*****************************************************************************/
PUBLIC uint16  MMIAPIPHONE_GetVodafoneSimSelectResult(uint32 sim_num, wchar*  vodafone_text);

/*****************************************************************************/
// 	Description : set fasctory vodafone nv
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetFactoryVodafoneSimSelectNV(void);

/*****************************************************************************/
//  Description : Get PUK remain SIM
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetPukRemainDualSys(void);

#if defined(UNISOC_MMI_SIM_LOCK_SUPPORT)&&!defined(MMI_SIM_LOCK_SUPPORT)
/*****************************************************************************/
// 	Description : is phone need restart?
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetPhoneNeedRestart(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set is phone need restart
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetPhoneNeedRestart(MN_DUAL_SYS_E dual_sys, BOOLEAN is_need_restart);
#endif

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : is SIM unlocked?
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetIsSIMUnlocked(void);

/*****************************************************************************/
// 	Description : is phone need restart?
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetPhoneNeedRestart(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set is phone need restart
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetPhoneNeedRestart(MN_DUAL_SYS_E dual_sys, BOOLEAN is_need_restart);

/*****************************************************************************/
//  Description : mmi phone get curr countdown time to restore in NV
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPHONE_GetCurCountdownTime( void );

/*****************************************************************************/
// 	Description : create input sim lock window
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:  
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_CreateInputSimLockWindow(APP_MN_SIM_NOT_READY_IND_T sim_not_ready_ind);

/*****************************************************************************/
// 	Description : get handle sim lock status ptr for multi sim card
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_HANDLE_SIMLOCK_STATUS_T *MMIPHONE_GetHandleSIMLockStatus(void);

/*****************************************************************************/
// 	Description : MMIPHONE SIM Locked Open Shut Off Win
//	Global resource dependence :                                
//      Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SIMLockedOpenShutOffWin (void);

#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
/*****************************************************************************/
// 	Description : MMIPHONE SIM Locked, disable SIM
//	Global resource dependence :                                
//  Author: wubin.zou
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SIMLockedDisableSIM(uint8 *err_info);

/*****************************************************************************/
// 	Description : MMIPHONE SIM Locked, disable BT
//	Global resource dependence :                                
//  Author: wubin.zou
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SIMLockedDisableBT(uint8 *err_info);

/*****************************************************************************/
// 	Description : MMIPHONE SIM Locked, unlock
//	Global resource dependence :                                
//  Author: wubin.zou
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SIMLockedDisableNone(uint8 *err_info);

/*****************************************************************************/
// 	Description : MMIPHONE SIM Locked, verify pass
//	Global resource dependence :                                
//  Author: wubin.zou
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SIMLockedOK(uint8 *err_info);
#endif
#endif/*MMI_SIM_LOCK_SUPPORT*/

/*****************************************************************************/
// 	Description : get opn display string
//	Global resource dependence : 
//  Author: kun.yu
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_GetAciString(PHONE_SERVICE_STATUS_T *service_status,//IN:
                                         MMI_STRING_T *aci_str_ptr, //OUT
                                         uint16 max_aci_len //IN
                                         );

/*****************************************************************************/
// 	Description : cat opn spn display string
//	Global resource dependence : 
//   Author: kun.yu
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_CatAciOpnSpnString(MMI_STRING_T *aci_str_ptr,//IN 
                                                MMI_STRING_T *opn_str_ptr,//IN:
                                                MMI_STRING_T *spn_str_ptr,//IN:
                                                MMI_STRING_T *network_str_ptr//OUT
                                                );

/*****************************************************************************/
// 	Description : cat ppn display string
//	Global resource dependence : 
//   Author: sam.hua
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_CatAciPpnString(MMI_STRING_T *aci_str_ptr,//IN 
                                                MMI_STRING_T *ppn_str_ptr,//IN:
                                                MMI_STRING_T *network_str_ptr//OUT
                                                );

/*****************************************************************************/
// 	Description : get the status of battery(if it is charging)
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetChargeStatus(void);

/*****************************************************************************/
// 	Description : init nitz info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_InitNITZ(void);

/*****************************************************************************/
// 	Description : Store Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:only update and store network name, do not Parse name string 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_StoreNetworkNameInfo(const APP_MN_NETWORK_INFO_IND_T *network_info_ind);
/*****************************************************************************/
// 	Description : get nitz update flag
//	Global resource dependence : none
//  Author: zhongping.shi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_IsIgnoreNITZNetwork(void);
/*****************************************************************************/
// 	Description : get NV Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_NV_NETWORK_NAME_T* MMIPHONE_GetNVNetworkNameInfo(void);

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETWORK_INFO_IND
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:store, update network name,and update mm nitz time，full name和short name
//          ,与opn状态相同,在处理opn时存NV
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleNetworkInfoInd(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : get sim spn Info
//	Global resource dependence : none
//    Author: bean.yu
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_SIM_SPN_INFO_T* MMIPHONE_GetSimSPNInfo(void);

/*****************************************************************************/
// 	Description : sync sim spn
//	Global resource dependence : none
//    Author: bean.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SyncSimSPN(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_InitNetworkNameInfo(void);

#ifdef MMI_NETWORK_NAME_SUPPORT
/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_NETWORK_NAME_T* MMIPHONE_GetNetworkNameInfo(void);

/*****************************************************************************/
// 	Description : get Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_PNN_T* MMIPHONE_GetPNNInfo(void);

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETNAME_READY_IND
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready index, get the SPN SPNI ONS .etc netname that can read derectly
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleNetNameReadyInd(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_CPHS_READY_IND
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready flag, get the ONS .etc netname that can read derectly
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleCPHSReadyInd(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_RECORD_NUM_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:store SIM ready index
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMRecordNumCnf(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_RECORD_READ_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:SIM record read confirm
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMRecordReadCnf(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_BINARY_READ_CNF
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:read SPNI confirm
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_HandleSIMBinaryReadCnf(
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description :Get Curr SIM PLMN Network Name Num
//  Global resource dependence : none
//  Author:dave.ruan
//  Note:this is an asynchronism function
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetPNNum(
                           MN_DUAL_SYS_E    dual_sys
                           );

/*****************************************************************************/
//  Description : Get Network Name String
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:network name string include ACI SPN OPN three part
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetNetworkNameString( 
                           MN_DUAL_SYS_E   dual_sys,
                           MMI_STRING_T  *  network_name_ptr
                           );
                           
PUBLIC BOOLEAN MMIPHONE_GetPlmnNameBySearchInfo(MN_DUAL_SYS_E dual_sys,
                                                MN_PHONE_PLMN_DETAIL_T *plmn_details,
                                                MMI_STRING_T *network_str_ptr);
/*****************************************************************************/
//  Description : set the PNN/PNNI  in to MMI
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_SetPNNWithLac(
                                      MN_DUAL_SYS_E   dual_sys,       //IN:
                                      MN_PLMN_T cur_plmn,                 //IN
                                      uint16  lac,         //IN
                                      BOOLEAN plmn_is_roaming
                                      );

/*****************************************************************************/
//  Description : Get String from SPN/PNN by PLMN and Lac
//  1. get Pnn first, if none, get SPN
//  2. get SPN first, if none, get PNN
//  Note:network name string include ACI SPN OPN three part
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_GetSPNPNNString( 
                           MN_DUAL_SYS_E   dual_sys,
                           MN_PLMN_T           cur_plmn,
                           uint16   lac,
                           MMI_STRING_T  *  network_name_ptr
                           );
#endif

#ifdef   __cplusplus
    }
#endif

#endif

