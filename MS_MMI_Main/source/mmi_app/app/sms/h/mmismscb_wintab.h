/******************************************************************************
**
**
**
**  小区广播模块
**
**
**
**
**
**
*******************************************************************************/
#if defined(MMI_SMSCB_SUPPORT)

#ifndef  _MMISMS_CB_WINTAB_H_    
#define  _MMISMS_CB_WINTAB_H_   
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "dal_time.h"
#include "mmk_type.h"
#include "mmisms_export.h"
#include "mmismscb_control.h"
#include "mn_type.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

#ifdef MMI_ETWS_SUPPORT
#define MMI_VIBRATE_INTERVAL 500
#endif

/******************************************************************************************************************/
//
//
//                        小区广播模块对外接口函数声明
//
//
/******************************************************************************************************************/

/*****************************************************************************/
//     Description : MMISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_OpenCBList(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         IsCBSwitchSelected
//  功能描述:        获取CB ON / OFF数值
//  全局变量使用:    无
//  返回值:          
//  作者:            Yongli.tong
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN IsCBSwitchSelected(
                                MN_DUAL_SYS_E dual_sys
                                );

/*****************************************************************************/
//     Description : MMISMSCB_SaveCBEmergenceChannel
//    Global resource dependence : 
//  Author: Tong Yongli
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_SaveCBEmergenceChannel(MN_DUAL_SYS_E dual_sys);


#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         NV_LoadSmsCBChannelList
//  功能描述:        读取NV中小区广播信道列表结构
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播信道列表结构
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_NV_LoadSmsCBChannelList(void);
#else
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         NV_LoadSmsCBChannelList
//  功能描述:        读取NV中小区广播信道列表结构
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播信道列表结构
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_T MMISMS_NV_LoadSmsCBChannelList(MN_DUAL_SYS_E dual_sys);
#endif

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_NV_LoadSmsCBMessageList
//  功能描述:        读取NV中小区广播信息数目
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播信息
//  作者:            
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint8 MMISMS_NV_LoadSmsCBMessageListNum(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSNV_SaveSmsCBMessageList
//  功能描述:        保存小区广播信息数目
//  全局变量使用:    无
//  返回值:          无
//  作者:          
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSNV_SaveSmsCBMessageListNum( uint8 message_num);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_NV_LoadSmsCBMessageList
//  功能描述:        读取NV中小区广播信息
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播信息
//  作者:            
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_MESSAGE_ARRAY_T* MMISMS_NV_LoadSmsCBMessageList(uint8 index);

 //----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSNV_SaveSmsCBMessageList
//  功能描述:        保存小区广播信息
//  全局变量使用:    无
//  返回值:          无
//  作者:          
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSNV_SaveSmsCBMessageList(MMISMS_CB_MESSAGE_ARRAY_T*  message_array, uint8 index);
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         GetCBChannelNameByMsgId
//  功能描述:        由小区信道ID获得相应的信道名称
//  全局变量使用:    无
//  返回值:          信道名称
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_NAME_T MMISMS_GetCBChannelNameByMsgId(                                        //RETURN: 
                                                       MMISMS_CB_CHANNEL_T    *channel_list,    //IN: 信道列表结构
                                                       uint16                msg_id            //IN: 小区信道ID
                                                       );

/*****************************************************************************/
//  Description : open SMSCB CLASS0 window      
//  Global resource dependence : 
//  Author: jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_OpenCLASS0Win(void);

#ifdef MMI_SMSCB_CELL_INFO_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_GetCellInfoFlag
//  功能描述:        Get the cell info flag
//  全局变量使用:    s_cell_info_flag
//  返回值:          无
//  作者:            Yongli Tong
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMSCB_GetCellInfoFlag(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//     Description : to show the new SMSCB prompt
//    Global resource dependence : 
//  Author:Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewSMSCBPrompt(void);

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : to show the new ETWS prompt
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowNewETWSPrompt(MMI_STRING_T* etws_prompt_str, MMI_STRING_T* etws_title_string,BOOLEAN is_enable);
#endif

/*****************************************************************************/
//     Description : 在一些特定的网络状态下需要打开/关闭小区广播开关
//                         Situation 1: MCC = 502(Malaysia) and switch from 3G to 2G should be closed CB Switch.
//    Global resource dependence : 
//    Author: Yongli Tong
//    Note: 
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMISMSCB_SwitchCBStatusOnSomeNetworkCircumstance(MN_DUAL_SYS_E dual_sys, BOOLEAN cb_flag);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_DirectDisplayNewCBMsg
//  功能描述:        立即显示收到的新小区广播内容
//  全局变量使用:    无
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSCB_DirectDisplayNewCBMsg(
                                 APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr    //广播信息信号指针  (输入参数)

                                 );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_DirectDisplayNewMarcoCBMsg
//  功能描述:        立即显示收到的新小区广播内容
//  全局变量使用:    无
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMSCB_DirectDisplayNewMarcoCBMsg(
                                 APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr    //广播信息信号指针  (输入参数)
                                 );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         ConvertCBMsgContextToMsgItem
//  功能描述:        将小区广播内容按照其编码方式解压为小区广播信息项结构
//  全局变量使用:    无
//  返回值:          小区广播信息
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint16 ConvertCBMsgContextToMsgItem(                                           //RETURN: 
                                    MN_CB_MSG_ARR_T           msg_arr,         //IN: 小区广播消息内容数据指针
                                    uint16                    msg_len,         //IN: 小区广播消息内容数据长度
                                    MN_SMS_ALPHABET_TYPE_E    alphabet_type,   //IN: 小区广播消息内容编码方式
                                    BOOLEAN                   is_macro_msg,    //IN: 是否为长小区广播
                                    MMISMS_CB_MESSAGE_ITEM_T  *msg_item_ptr    //IN: 小区广播信息项指针
                                    );

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_NV_LoadSmsCBLanguageList
//  功能描述:        读取NV中保存的小区广播语言选择列表结构
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播语言选择列表结构
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMIAPISMSCB_NV_LoadSmsCBLanguageList(MN_DUAL_SYS_E dual_sys, MMISMS_CB_LANGUAGE_T* lang_card_list);
#else
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_NV_LoadSmsCBLanguageList
//  功能描述:        读取NV中保存的小区广播语言选择列表结构
//  全局变量使用:    无
//  返回值:          NV中保存的小区广播语言选择列表结构
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_LANGUAGE_T MMIAPISMSCB_NV_LoadSmsCBLanguageList(MN_DUAL_SYS_E dual_sys);
#endif

#if 0
/*****************************************************************************/
// 	Description : enter cb msg list win
//	Global resource dependence : 
//  Author: jixin.xu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterMsgListWin(void);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
#endif

