/*****************************************************************************
** File Name:      mmiset_func.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li          Create
******************************************************************************/

#ifndef _MMISET_FUNC_H_
#define _MMISET_FUNC_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef UNISOC_VERSION
#include "mmipb_export.h"
#endif


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMISET_DEFAULT_KEYLOCK_TIME MMISET_KEYLOCK_15_SEC ///MMISET_KEYLOCK_CLOSE //锁屏的默认设置

#define MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_90  0x20 //cr227614 maryxiao
#define MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_180  0x10
#define MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_270  0x8
#define MMISET_SENSOR_OPEN_FLAG_CALL_SILENCE  0x4
#define MMISET_SENSOR_OPEN_FLAG_REPLACE_WALLPAPER  0x2
#define MMISET_SENSOR_OPEN_FLAG_NEXT_SONG  0x1
//#define MMISET_SENSOR_OPEN_FLAG_VIDEO_FULLSCREEN  0x10
#define        MMISET_LANG_NAME_MAX_LEN         20
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
#ifdef MMI_RES_ORIENT_BOTH
	MMISET_SENSOR_TYPE_LANDSCAPE_90,//自动横竖屏显示 cr227614 maryxiao
	MMISET_SENSOR_TYPE_LANDSCAPE_180,
	MMISET_SENSOR_TYPE_LANDSCAPE_270,
#endif
	MMISET_SENSOR_TYPE_CALL_SILENCE,//电话静音
	MMISET_SENSOR_TYPE_REPLACE_WALLPAPER,//更换墙纸
	MMISET_SENSOR_TYPE_NEXT_SONG,//播发下一曲
	//MMISET_SENSOR_TYPE_VIDEO_FULLSCREEN,
	
}MMISET_SENSOR_TYPE_E;

#define MMISET_SENSOR_TYPE_MAX  4//CR 227614 MARYXIAO
#define MMISET_SENSOR_AUTOROTATION_MAX 3


typedef struct
{
	uint8  nv_forced_type[2];
}MMISET_LANG_FORECD_NV_BYTE_T;
typedef struct
{
    uint16   lang_valid[MMISET_MAX_LANGUAGE + 1];
    uint16   lang_valid_num;
}MMISET_LANG_VALID_INFO_T;
typedef struct
{
    wchar   lang_name[MMISET_LANG_NAME_MAX_LEN];
    BOOLEAN lang_status;
}MMISET_LANG_SET_T;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
#ifdef UNISOC_VERSION
/*****************************************************************************/
//  Description : Get prefer sim from PB 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_PREFERRED_SIM_TYPE_T MMIAPISET_GetPreferSimFromPB();

/*****************************************************************************/
//  Description : Set prefer sim
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPreferSimFromPB(MMIPB_PREFERRED_SIM_TYPE_T dual_sys);
#endif

/*****************************************************************************/
//  Description : Set the mode of anykey and reject
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetAnykeyRejectMode(
                                       MMI_CTRL_ID_T ctrl_id
                                       );

/*****************************************************************************/
//  Description : reset factory setting 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : 添加语言选择的radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendLanguageListItem(
                                          MMI_CTRL_ID_T     ctrl_id
                                          );

/*****************************************************************************/
PUBLIC void MMISET_InitLanguageForcedNVFirstPowerOn(void);
PUBLIC void MMISET_GetLanguageFirstPowerOn(
											  MMISET_LANGUAGE_TYPE_E *prefer_language
											  );
PUBLIC void MMISET_AppendLanguageFirstPowerOnListItem(
                                          MMI_CTRL_ID_T     ctrl_id,
                                          MMISET_LANGUAGE_TYPE_E  prefer_language
                                          );
//  Description : 设置语言选择的结果
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectLanguage(
                                     MMI_CTRL_ID_T      ctrl_id
                                     );
PUBLIC void MMISET_SetSelectLanguageFirstPowerOn(
                                     MMI_CTRL_ID_T      ctrl_id
                                     );

/*****************************************************************************/
//  Description : 添加键盘灯时间到radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendKeyboardLightListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               );

/*****************************************************************************/
//  Description : 设置键盘灯选择的结果
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetKeyboardLight(
                                    MMI_CTRL_ID_T       ctrl_id
                                    );


PUBLIC void MMISET_SetAutoKeyLock(
                                    MMI_CTRL_ID_T       ctrl_id
                                    );
PUBLIC void MMISET_RedKeyLock(
                                MMI_CTRL_ID_T   ctrl_id
                                );
/*****************************************************************************/
//  Description : 添加move style到radio list的item
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendMoveStyleListItem(
                                           MMI_CTRL_ID_T        ctrl_id
                                           );

/*****************************************************************************/
//  Description : 设置move style选择的结果
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMoveStyle(
                                MMI_CTRL_ID_T       ctrl_id
                                );

/*****************************************************************************/
//  Description : 添加开启关闭的单选list items 
//  Global resource dependence :  
//  Author: jibin
//  Note: 通过funcGet从nv得到当前开启关闭状态 
/*****************************************************************************/ 
PUBLIC void MMISET_AppendOpenCloseListItem( 
                                           MMI_CTRL_ID_T ctrl_id,
                                           BOOLEAN (*funcGet)(void)
                                           );

/*****************************************************************************/
//  Description : set is auto boardkey
//  Global resource dependence : none
//  Author: jibin
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetAutoBoardKey(MMISET_KEYLOCK_TYPE_E auto_boardkey_time);

/*****************************************************************************/
//  Description : 添加网络选择方式到radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSelectNetworkListItem(
                                               MN_DUAL_SYS_E dual_sys,
                                               MMI_CTRL_ID_T        ctrl_id
                                               );

/*****************************************************************************/
//  Description : 设置网络选择方式
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectNetworkMode(
                                        MN_DUAL_SYS_E dual_sys,
                                        MMI_CTRL_ID_T       ctrl_id
                                        );

/*****************************************************************************/
//  Description : 选择指定的网络
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SelectNetwork(
                                 MN_DUAL_SYS_E dual_sys,
                                 MMI_CTRL_ID_T      ctrl_id
                                 );

/*****************************************************************************/
//  Description : 添加频段选择方式到list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSelectBansListItem(
                                            MMI_CTRL_ID_T   ctrl_id
                                            );

/*****************************************************************************/
//  Description : 设置频段选择方式
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSelectBandType(
                                        MMI_CTRL_ID_T       ctrl_id
                                        );

/*****************************************************************************/
//  Description : 设置开机密码开启或者关闭
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetPowerOnPwdStatus(
                                          BOOLEAN  is_open
                                          );
/*****************************************************************************/
//  Description : 获得开机密码长度，用来判断开机密码是否已经设置
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsSetPowerOnPwd(void);

/*****************************************************************************/
//  Description : 网络设置中，自动搜索和手动搜索从LIST中选择网络的过程中，不允许取消！
//只有在手动搜索 --------------〉网络LIST,这中间才可以取消（包括red key, flip）
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleQuitSelNetworkConnectWindow(
                                                             MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                             DPARAM             param       // 相应消息的参数
                                                             );
/*****************************************************************************/
//  Description : 网络设置中，自动搜索和手动搜索从LIST中选择网络的过程中，不允许取消！
//只有在手动搜索 --------------〉网络LIST,这中间才可以取消（包括red key, flip）
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleSelNetworkConnectWindow(
                                                         MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                         MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                         DPARAM             param       // 相应消息的参数
                                                         );

/*****************************************************************************/
// Description : Get Is Allow Open Query Fly mode
// Global resource dependence :
// return :  
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetIsQueryOpenFlyMode(void);

/*****************************************************************************/
// Description : Stop PS Application
// Global resource dependence :
// return :  
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC void MMISET_StopPSApplication(void);
#if 0
/*****************************************************************************/
//  Description : 添加语言选择的dropdownlist的item
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendShowLanguageItem(
                                          MMI_CTRL_ID_T     ctrl_id
                                          );

/*****************************************************************************/
//  Description : 添加语言输入的dropdownlist的item
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendInputLanguageItem(
											   MMI_CTRL_ID_T ctrl_id, 
											   MMISET_LANGUAGE_TYPE_E language_type
											   );
#endif
/*****************************************************************************/
//  Description : 设置输入语言的结果
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetInputLanguage(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description :是否相关sensor功能开启
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
BOOLEAN MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_E type);

/*****************************************************************************/
//  Description :设置sensor是否开启flag
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISET_SetIsSensorOpenFlag(uint32 open_flag);

/*****************************************************************************/
//  Description :获取sensor是否开启flag
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISET_GetIsSensorOpenFlag(void);

/*****************************************************************************/
//  Description :设置语言可active label的值
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetLanguageCtrlContent(void);

/*****************************************************************************/
//  Description :设置输入法语言可active label的text
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetInputLanguageCtrlContent(void);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : load blacklist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetBlackList(void);

/*****************************************************************************/
//  Description : set blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetBlackList(MMISET_BLACKLIST_T *blacklist);

/*****************************************************************************/
//  Description : init blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISET_InitBlacklistParam(void);

/*****************************************************************************/
//  Description : free blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_FreeBlacklistParam(void);
#endif
#if defined(MMI_WHITELIST_SUPPORT)
/*****************************************************************************/
//  Description : init whitelist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISET_InitWhitelistParam(void);

/*****************************************************************************/
//  Description : load whitelist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetWhiteList(void);

/*****************************************************************************/
//  Description : free whitelist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_FreeWhitelistParam(void);

/*****************************************************************************/
//  Description : set is whitelist audio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsWhitelistAudioCall(BOOLEAN is_white_audio_call);

/*****************************************************************************/
//  Description : set is whitelist message
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsWhitelistMessage(BOOLEAN is_white_mesage);

/*****************************************************************************/
//  Description : set is whitelist vedio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsWhitelistVedioCall(BOOLEAN is_white_vedio_call); 

#endif
/*****************************************************************************/
//  Description : init date display type
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_InitDateDisplayType(void);

/*****************************************************************************/
//  Description : get valid language number
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_GetValidLanguageNum(void);

/*****************************************************************************/

//  Description : 
//  Global resource dependence : 
//  Author:maryxiao
//  Modify:wei.zhou, for wre use
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_ReturnToAngleSupport(uint32 sensor_open_flag);

/*****************************************************************************/
//  Description :获取当前系统语言
//  Global resource dependence : 
//  Author: yuantao.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCurrentLanguageTextAlignRTLDirection(void);
PUBLIC MMISET_LANGUAGE_TYPE_E MMISET_GetLanguageType(void);
/*****************************************************************************/
//  Description :获取当前系统语言
//  Global resource dependence : 
//  Author: yuantao.xu
//  Note:
/*****************************************************************************/
PUBLIC MMISET_LANGUAGE_TYPE_E MMISET_GetCurrentLanguageType(void);
/*****************************************************************************/
//  Description : Select SIM API of SET app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC MMISetSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E    append_type,
                                        ADD_DATA        	        add_data
                                        );
/*****************************************************************************/
//  Description : Select NETWORK API of SET app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_SelectNetworkFunc(MMIAPIPHONE_SELECTSIM_FUNC MMISetSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA        add_data
                                        );

/*****************************************************************************/
//  Description : Get T9 Predictive Status from NV 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPISET_GetT9ConfigValue( BOOLEAN t9_status);

/*****************************************************************************/
//  Description : Check the dictionary language status
//  Global resource dependence : 
//  Author:
//  Note: TRUE: Dictionary language ready; FALSE: Dictionary language not ready
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_CheckDictionaryLanguageStatus(MMISET_LANGUAGE_TYPE_E language_type);
/*****************************************************************************/
//  Description : 获取当前Dictionary语言的默认设置
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISET_LANGUAGE_TYPE_E MMIAPISET_GetCurrentDictionaryInputLanguage(void);
/*****************************************************************************/
//  Description : 设置选择的Dictionary输入语言
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetDictionaryInputLanguage(MMISET_LANGUAGE_TYPE_E language_type);
/*****************************************************************************/
//  Description : 获取Dictionary status
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetDictionaryStatus(void);
/*****************************************************************************/
//  Description : 设置Dictionary status
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDictionaryStatus(BOOLEAN t9_status);
/*****************************************************************************/
//  Description : set bt log disabled
//  Global resource dependence : none
//  Author: wenhui.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_DisableBTLog(void);

/*****************************************************************************/
//  Description : set bt log enabled
//  Global resource dependence : none
//  Author: wenhui.li
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPISET_EnableBTLog(void);
/*****************************************************************************/
//  Description : get bt log enable flag
//  Global resource dependence : none
//  Author: wenhui.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetBTLogEnable();

/*****************************************************************************/
//  Description :  bt log init
//  Global resource dependence : none
//  Author: wenhui.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_BTLogInit(void);

PUBLIC BOOLEAN MMIAPISET_isRegionDE();
#endif
