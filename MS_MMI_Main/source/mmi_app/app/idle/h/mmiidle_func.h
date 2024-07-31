/*****************************************************************************
** File Name:      mmiidle_func.h                                            *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Create                                 *       
** 02/2012        murphy.xie           Modify some interface                 *
******************************************************************************/

#ifndef _MMIIDLE_FUNC_H_
#define _MMIIDLE_FUNC_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_font.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guistring.h"
#include "mn_type.h"
#include "mmi_signal_ext.h"
//#include "mmiset.h"
#include "mmiset_export.h"
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
//蓝牙的状态标示
typedef uint8  BLUETOOTH_STATE_TYPE;
#define ON_AND_VISIBLE  0 //开启并且可见
#define ON_AND_HIDDEN   1 //开启并且隐藏
#define ON_AND_HEADSETCONNECT   2 //开启并且蓝牙耳机连接
#define OFF_BLUETOOTH   3 //蓝牙关闭
#ifdef MAINLCD_SIZE_128X160
#define MMI_IDLE_SIM_ICON_INTERVAL 0  //sim icon的区间/128X160的宽度太小显示不下please insert SIM card,现在显示please insert SIM ，card超出屏幕
#else
#define MMI_IDLE_SIM_ICON_INTERVAL 4  //sim icon的区间
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// @Great.Tian, 定义拨号窗口中的PhoneNum长度
#define MMIDIAL_PHONENUM_LEN 40
#define MMIIDLE_TIME_LEN        10
#define MMIIDLE_COUNT_TIME_LEN        20//10
//#define MMI_MAIN_SPN_MAX_LEN  50
#define MMIIDLE_PHONENUM_MAX_LEN 20
#define MMIIDLE_APP_NAME_MAX_LEN 64
#define MMIIDLE_LIST_ICON_PATH_MAX_LEN 256

//#define CTRLLIST_MAX_STRLEN      0xFF //list支持的最大字符个数
#define MMIIDLE_LIST_MAX_STRLEN 0xFF
#define MMICC_PHONE_NUM_MAX_LEN  (MMIPB_BCD_NUMBER_MAX_LEN << 1)

#define MMIIDLE_EMERGENCY_CALL_NUM_MAX_LEN 3

#define MMIIDLE_LCD_OFF_PS_FAST_SLEEP_TIME  3
#define MMIIDLE_LCD_ON_PS_FAST_SLEEP_TIME  12


// the idle window information
// 实际上以下的数据中有些数据是可以直接获取的。
typedef struct
{
    MN_DUAL_SYS_E   dual_sys;
    uint8           time_id;
    uint8           rxlevel[MMI_DUAL_SYS_MAX];       // 小区信号强度(0-5, 6 level)，由phoneapp来设置， APP_MN_SCELL_RSSI_IND
    uint8           batlevel;      // 电池电量(0-5, 6 level)，由phoneapp来设置， CHR_CAP_IND
#ifdef MMI_DUAL_BATTERY_SUPPORT
    uint8           batlevel_aux; 
#endif
    //CALL_STATE_TYPE call_state;    // call状态，由callapp来设置
    //BOOLEAN         call_is_flash; // 只有当call_state == NEW_CALL时才有效
    BOOLEAN         is_gprs[MMI_DUAL_SYS_MAX];      //是否支持GPRS
    BOOLEAN         is_roaming[MMI_DUAL_SYS_MAX];		//是否漫游
    BOOLEAN         is_calling;  //当前是否处于通话中
    BOOLEAN         is_missed_call; // 当前是否有未接来电
    BOOLEAN         is_stopwatch_running;      // 当前是否有秒表运行。
    BOOLEAN         is_countdown_timer;      // 当前是否有倒计时提醒。
    BOOLEAN         is_reminder;      // 当前是否有日历提醒。
    BOOLEAN         is_app_notice;      // 当前是否有应用通知消息。
    BOOLEAN         is_alarm;      // 当前是否设置闹钟。由setapp来设置
    BOOLEAN         is_shake;      // 当前是否设置震动。由setapp来设置
    BOOLEAN         is_silent;     // 当前是否设置静音。由setapp来设置
    BOOLEAN         is_charge; //是否在充电
    BOOLEAN         is_display_mp3; //是否播放mp3
    BOOLEAN         is_display_spn; //当前是否要显示SPN(Service Providor Number)
    BOOLEAN			is_forward[MMI_DUAL_SYS_MAX];		//当前是否设置了呼叫转移

    BOOLEAN         is_display_stk_idle_text; //是否显示STK IDLE Text

    uint8           rxlevel_step[MMI_DUAL_SYS_MAX];       // 记录当前显示的小区信号强度(0-5, 6 level)，由phoneapp来设置， APP_MN_SCELL_RSSI_IND

    uint8           cur_batlevel; //当前电池电量
    // 还有运营商，时间日期，背景图片,屏幕保护等等。
    // 对于屏幕保护的做法：单独开一个窗口，按任意键之后就回到Idle窗口。
    // 但是这样当有Call的时候，屏幕保护窗口就没有被杀掉。
    MMI_IMAGE_ID_T  image_id;      // 由setapp来设置Idle下的背景图片,如果通过ID得不到Data，则设成默认的
    BLUETOOTH_STATE_TYPE   bluetooth_state; //蓝牙的状态
//add for cphs icon begin
    uint8           line_state[MMI_DUAL_SYS_MAX];           //通话线路的状态，有setapp来设置，0为线路1,1为线路2
    BOOLEAN         is_new_vm;          //是否有新的语音信息，不用区分线路1或线路2
//add for cphs icon end
    BOOLEAN         is_usb_mode;      // 当前是否USB连接
}
IDLE_WIN_INFO_T;

// the stk idle text
typedef struct
{
    BOOLEAN is_scroll; //是否滚动
    MMI_STRING_T stk_idle_str;
    uint16 str_width;
    uint16 str_index;
    uint8 idle_display_timer;
    MN_DUAL_SYS_E dual_sys;
} IDLE_STK_TEXT_INFO;



//Push notification message max count on idle screen
#define MMI_IDLE_PUSH_MSG_MAX_COUNT 30

//The app entry parameter
typedef void *IDLE_APP_PARA_T;

typedef enum
{
    IDLE_APP_ENTRY_NULL = 0,
    IDLE_APP_ENTRY_SMS,
    IDLE_APP_ENTRY_SMS_ANSWER,
    IDLE_APP_ENTRY_CALL,
    IDLE_APP_ENTRY_CALLLOG,
    IDLE_APP_ENTRY_CALLMISSED,
    IDLE_APP_ENTRY_CURCALL,
    IDLE_APP_ENTRY_ALARM,
    IDLE_APP_ENTRY_CALENDER,
    IDLE_APP_ENTRY_WNS,

    IDLE_APP_ENTRY_MAX
}IDLE_APP_ENTRY_E;

//App entry function ptr
typedef void (*IDLE_APP_ENTRY_FUNC_T)(IDLE_APP_PARA_T param1, IDLE_APP_PARA_T param2);

//Push notification message type
typedef enum
{
    MMI_IDLE_PUSH_MSG_SMS = 0,  
    MMI_IDLE_PUSH_MSG_MISSED_CALL,
    MMI_IDLE_PUSH_MSG_ONGOING_CALL,
    MMI_IDLE_PUSH_MSG_SNOOZED_ALARM,
    MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER,
    MMI_IDLE_PUSH_MSG_VOICE_MAIL,
    MMI_IDLE_PUSH_MSG_MAIL,
    MMI_IDLE_PUSH_MSG_OPERATER_SETTING,
    MMI_IDLE_PUSH_MSG_WNS,
    MMI_IDLE_PUSH_MSG_STK,
    
    MMI_IDLE_PUSH_MSG_TYPE_MAX
}MMI_IDLE_PUSH_MSG_TYPE_E;

//Push notification message status
typedef enum
{
    MMI_IDLE_PUSH_MSG_STATE_NULL = 0,  
    MMI_IDLE_PUSH_MSG_STATE_VALID,
    MMI_IDLE_PUSH_MSG_STATE_CLEAR,
    MMI_IDLE_PUSH_MSG_STATE_MAX
}MMI_IDLE_PUSH_MSG_STATE_E;


/*! @enum GUIITEM_DATA_TYPE_E
@brief Item数据类型，通过这些类型定义数据信息的UNION
*/
typedef enum
{
    MMI_IDLE_ICON_DATA_NONE,  /*!<  none */
    MMI_IDLE_ICON_DATA_IMAGE_ID,    /*!<  图片资源ID */ 
    MMI_IDLE_ICON_DATA_ANIM_PATH,   /*!<  图片路径 */
    MMI_IDLE_ICON_DATA_MAX,
}MMI_IDLE_ICON_DATA_TYPE_E;

/*! @union GUIITEM_DATA_U
@brief 数据联合体
*/
typedef union
{
    MMI_IMAGE_ID_T          image_id;          /*!< 图片资源ID */
    wchar                               full_path_wstr_ptr[MMIIDLE_LIST_ICON_PATH_MAX_LEN];     
}MMI_IDLE_ICON_DATA_U;


typedef struct mmicc_number_tag
{
    uint8 tel_num[MMICC_PHONE_NUM_MAX_LEN + 2];
    uint8 tel_num_len;
    MN_DUAL_SYS_E dual_sys;
} MMICC_NUMBER_T;


//Push notification message info 
typedef struct idle_push_msg_info_tag 
{ 
    MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type; 
    MMI_IDLE_ICON_DATA_TYPE_E icon_data_type; 
    MMI_IDLE_ICON_DATA_U icon_data;
    char   appname[MMIIDLE_APP_NAME_MAX_LEN]; 
    //uint8  sender_num[MMIIDLE_PHONENUM_MAX_LEN + 1];
    uint8  recv_num[MMIIDLE_PHONENUM_MAX_LEN + 1];
    MMICC_NUMBER_T cc_number; //for missed call
    MMIPB_BCD_NUMBER_T pb_bcd;  // for msg
    wchar title[MMIIDLE_LIST_MAX_STRLEN + 1]; 
    wchar contents[MMIIDLE_LIST_MAX_STRLEN + 1]; 
#if 0
    IDLE_APP_ENTRY_FUNC_T app_entry_func_ptr; 
    IDLE_APP_ENTRY_FUNC_T app_left_focus_action_func_ptr; 
    IDLE_APP_ENTRY_FUNC_T app_right_focus_action_func_ptr; 
#else
    IDLE_APP_ENTRY_E app_entry_e; 
    IDLE_APP_ENTRY_E app_left_focus_action_e; 
    IDLE_APP_ENTRY_E app_right_focus_action_e; 
#endif
} IDLE_PUSH_MSG_INFO_T; 

//Push notification message list
typedef struct idle_push_msg_node_tag
{
    MMI_IDLE_PUSH_MSG_STATE_E msg_state;
    IDLE_PUSH_MSG_INFO_T  idle_push_msg_info;
    uint32 msg_count;
} IDLE_PUSH_MSG_NODE_T;


//Push notification message info
typedef struct idle_push_msg_info_nv_tag
{
    IDLE_PUSH_MSG_NODE_T  idle_push_msg_info[MMI_IDLE_PUSH_MSG_MAX_COUNT];
} IDLE_PUSH_MSG_NV_T;


typedef enum
{
    MMI_IDLE_PHONE_NORMAL = 0,
    MMI_IDLE_PHONE_START_UP,
    
    MMI_IDLE_PHONE_STATUS_MAX
}MMI_IDLE_PHONE_STATUS_E;


//Idle lock data info
typedef struct idle_lock_data_tag
{
    MMI_IDLE_PHONE_STATUS_E e_phone_status;
    IDLE_APP_ENTRY_FUNC_T app_entry_func_ptr;
    IDLE_APP_PARA_T para1;
    IDLE_APP_PARA_T para2;
} IDLE_LOCK_DATA;


typedef enum
{
    IDLE_DISPLAY_NORMAL,
    IDLE_DISPLAY_LIMITED,
    IDLE_DISPLAY_NO_SERVICE,
    IDLE_DISPLAY_SERVICE_MAX
}IDLE_DISPLAY_SERVICE_TYPE_T;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON || defined MMI_COMMON_IDLE_SUPPORT ||defined BAR_PHONE_WIDGET  || defined MMI_MSTYLE_SUPPORT
#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : display schedule
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplaySchedule(const GUI_LCD_DEV_INFO *lcd_dev_ptr,uint8 direction, uint16 max_len, GUI_POINT_T  *dis_point,GUI_FONT_T font_type);
PUBLIC uint16 MMIIDLE_DisplayScheduleEx(
                                        GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                        GUI_RECT_T      *rect_ptr,
                                        GUI_FONT_T       font_type
                                        );
#endif
#ifdef MMI_SMSCB_CELL_INFO_SUPPORT 
/*****************************************************************************/
//  Description : display cell info
//  Global resource dependence : 
//  Author: Yongli Tong
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayCellInfo(const GUI_LCD_DEV_INFO *lcd_dev_ptr,uint8 direction, uint16 max_len, GUI_POINT_T  *dis_point,GUI_FONT_T font_type);
#endif
/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_GetIdleTextStyle(
                            GUISTR_STYLE_T *style_ptr,
                            GUI_FONT_T     font,
                            GUI_COLOR_T    font_color
                            );

/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayIdleText(//string pixel width
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len
                             );

PUBLIC uint16 MMIIDLE_DisplayLineTextInRect(
                              GUI_LCD_DEV_INFO *lcd_dev_ptr,
                              GUI_RECT_T      *rect_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len
                             );
/*****************************************************************************/
//  Description : get display string
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_GetDisPlayString(wchar const * src_str,     //in
                            uint16  src_len,     //in
                            uint16  max_display_len,//in
                            GUI_FONT_T font_type, //font
                            wchar * des_str,     //out
                            uint16* des_len      //out
                            );
#endif //#if defined MMI_SMART_IDLE_SUPPORT || defined MMI_COMMON_IDLE_SUPPORT
/*****************************************************************************/
//  Description : init win Info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_InitWinInfo(void);

/*****************************************************************************/
//  Description : get idle info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC IDLE_WIN_INFO_T* MMIIDLE_GetIdleWinInfo(void);

/*****************************************************************************/
//  Description : display the main and sub idle soft icon 
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_SendMsgToIdleWin(
                               MMI_MESSAGE_ID_E msg_id,
                               DPARAM            param_ptr
                               );

/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayIdleTextEx(
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len,
                             GUISTR_STATE_T  text_state
                             );
#ifdef MMI_IDLE_STKTEXT_SUPPORT
/*****************************************************************************/
//  Description : start stk idle display timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_StartIdleSTKTextTimer(void);

/*****************************************************************************/
//  Description : stop stk idle display timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_StopIdleSTKTextTimer(void);

/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_DrawStkIdleModeText(uint32 block_id);

/*****************************************************************************/
//  Description : get stk text info
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_STK_TEXT_INFO* MMIIDLECOM_GetStkTextInfo(void);

#endif

/**************************************************************************/ 
// Description : 提供给应用的接口，保存IDLE界面显示的通知信息 
// Global resource dependence : 
// Author: grant.peng 
// Note: 
/**************************************************************************/ 
PUBLIC BOOLEAN MMIIDLE_AppendPushMsg(IDLE_PUSH_MSG_INFO_T *push_msg_info_ptr);


/**************************************************************************/ 
// Description : 提供给应用的接口，清除Idle Screen消息的接口
// Global resource dependence : 
// Author: grant.peng 
// Note: 
/**************************************************************************/ 
PUBLIC void MMIIDLE_RemovePushMsg(
                        MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type,
                        uint8 *appname, 
                        MMICC_NUMBER_T *cc_number); 

/**************************************************************************/ 
// Description : 提供给msg的接口，清除Idle Screen消息的接口
// Global resource dependence : 
// Author: grant.peng 
// Note: 
/**************************************************************************/ 
PUBLIC void MMIIDLE_RemovePushMsgEx(
                                MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type,
                                uint8 *appname, 
                                MMIPB_BCD_NUMBER_T *pb_bcd
                                );

/*****************************************************************************/
//  Description : 清除Idle 界面上所有的通知
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_RemoveAllPushMsg(void);

/*****************************************************************************/
//     Description : load push msg info form nv
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_LoadPushMsgInfo(void);

/*****************************************************************************/
//  Description : Get push msg node pointer
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_PUSH_MSG_NODE_T *MMIIDLE_GetPushMsgNode(uint16 node_index);

/*****************************************************************************/
//  Description : 获取通知对应的图标。
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIIDLE_GetPushMsgIcon(MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type);



/*****************************************************************************/
//  Description : 设置通知列表每一项对应的软键
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_SetPushMsgSoftkey(MMI_IDLE_PUSH_MSG_TYPE_E e_push_msg_type, GUILIST_ITEM_DATA_T *item_data);


/*****************************************************************************/
//  Description : 设置通知列表每一项对应的左右Action
//  Global resource dependence : 
//  Author: grant.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_SetPushListItemLRAction(MMI_CTRL_ID_T ctrl_id, int item_count);

PUBLIC BOOLEAN MMIIDLE_HaveNewNotification(MMI_IDLE_PUSH_MSG_TYPE_E e_type);

/*****************************************************************************/
//  Discription: set ps fast sleep timer
//  Global resource dependence: None
//  Author: grant.peng
//  Note : 
/*****************************************************************************/
PUBLIC void MMIIDLE_SetPsFastSleepTimer(uint16 time_out);


#ifdef MMI_MSTYLE_SUPPORT
/*****************************************************************************/
//  Description : lock phone
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_LockPhone(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsPhoneLocked(void);


/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_UnlockPhone(void);


/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsDeviceLockWinOnTop(void);


/*****************************************************************************/
//  Description :Unlock phone by star
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_UlockPhoneByStar(IDLE_LOCK_DATA *idle_lock_data_ptr);

/*****************************************************************************/
//  Description :Unlock phone by security code
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_UlockPhoneBySecurityCode(IDLE_LOCK_DATA *idle_lock_data_ptr);

/*****************************************************************************/
//  Description :Unlock phone by security code
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_UlockPhoneBySecurityMode(IDLE_LOCK_DATA *idle_lock_data_ptr);

/*****************************************************************************/
// Description : emergency call win msg
// Global resource dependence : 
//  Author:bownzhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIdle_IsInputEmergencyCall(MMI_STRING_T *str_info_ptr, uint16 *emergency_call_len_ptr, MN_DUAL_SYS_E dual_sys);


/*****************************************************************************/
// Description : get sim card and plmn service status string to display in idle
//Global resource dependence : 
//  Author: bruce.chi
//Note:  
/*****************************************************************************/
PUBLIC uint32 MMIIDLE_GetSimAndServiceString(MMI_STRING_T *network_str_ptr);


#endif //#ifdef MMI_MSTYLE_SUPPORT

#ifdef BROWSER_SUPPORT
PUBLIC void MMIIDLE_StartBrowser (void);
#endif

/*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
