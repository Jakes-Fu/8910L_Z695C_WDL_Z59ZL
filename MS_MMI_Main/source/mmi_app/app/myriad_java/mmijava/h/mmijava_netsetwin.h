/*****************************************************************************
** File Name:      mmijava_netsetwin.h                                                
** Author:         murphy.xie   
** Date:           2012/08/15
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe DATA of alarm
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2012/08/15        murphy.xie     Create                                 *
******************************************************************************/

#ifndef  _MMIJAVA_NETSETWIN_H_
#define  _MMIJAVA_NETSETWIN_H_
#include "mmk_type.h"
#include "sci_types.h"

typedef enum
{
    JAVA_NETWORK_TYPE_SIM1 = 0,
#if defined(MMI_MULTI_SIM_SYS_DUAL)||defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    JAVA_NETWORK_TYPE_SIM2,
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    JAVA_NETWORK_TYPE_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    JAVA_NETWORK_TYPE_SIM4,
#endif
#if defined(WLAN_SUPPORT) || defined(WIFI_SUPPORT)
    JAVA_NETWORK_TYPE_WIFI,
#endif
    JAVA_NETWORK_TYPE_MAX
}JAVA_NETWORK_TYPE_E;   //network type


typedef struct  java_app_setting_tag
{
    JAVA_NETWORK_TYPE_E  nw_type;   //network type
    uint8                sim_apn[MMI_DUAL_SYS_MAX];//APN of network Source,such as SIM1,SIM[MMI_DUAL_SYS_MAX-1]
}JAVA_APP_SETTING_T;


/**************************************************************************************************
  * Description: MMIJAVA_OpenNetSettingWin
  * Input Paras:
  * Output Paras: 
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_OpenNetSettingWin(uint32 suiteid);

/**************************************************************************************************
  * Description: get setting info of suite about network
  * Input Paras:
      > suite_id  : unique id of midlet suite
  * Output Paras:
      > return : TRUE ,app_setting_info is valid;or else, invalid
      > app_setting_info : network info of suite network
  * note :
  *************************************************************************************************/
PUBLIC BOOLEAN MMIJAVA_setting_getAppNetInfo(IN int32 suite_id,OUT JAVA_APP_SETTING_T * app_setting_info);

PUBLIC BOOLEAN MMIJAVA_setting_setAppNetInfo(IN int32 suite_id,IN JAVA_APP_SETTING_T app_setting_info);


/**************************************************************************************************
  * Description: delete setting info of suite about network
  * Input Paras:
      > suite_id  : unique id of midlet suite
  * Output Paras:
      > return : TRUE ,del ok is valid;or else, fail
  * note : call the func when uninstall a midlet suite
  *************************************************************************************************/
PUBLIC BOOLEAN MMIJAVA_setting_delAppNetInfo(IN int32 suite_id);

#endif //_MMIJAVA_NETSETWIN_H_




