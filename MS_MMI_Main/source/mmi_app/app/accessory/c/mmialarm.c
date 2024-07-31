/****************************************************************************
** File Name:      mmialarm.c                              
** Author:                                                                 
** Date:            2004/08/24 
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to describe functions of alarm
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 08/24/2004     taul.zhu          Create
** 06/02/2006     bruce.chi         modify
** 08/10/2006     liqing.peng       modify -- code review
****************************************************************************/

/************************************************************************/
//���ӡ��Զ����ػ����ճ̶����¼���ʾ���¼��Ľṹ: MMIACC_SMART_EVENT_T
//                                              (mmiacc_event.h)
//
//�����¼���¼�洢��NV�ÿһ���¼�����һ��Id, Id��0��ʼ�ƣ�
//�����¼���ID: EVENT_ALARM_0, EVENT_ALARM_1, EVENT_ALARM_2
//�Զ����ػ��¼���ID: EVENT_AUTO_POWERON, EVENT_AUTO_POWEROFF
//�ճ��¼���ID: EVENT_SCH_0 -- EVENT_SCH_MAX(99)
//
//�¼��Ļ�������˵������:
//
//is_valid: ֻ����NV�洢����ʾĳ��NVλ���Ƿ��Ѿ���ʹ��
//          (ÿ���¼���¼��������һ��NVλ��)
//
//is_on: �¼��Ƿ񱻼��Ҳ���������Ƿ������ճ̵�ʱ�Ƿ���Ҫ���ѣ�
//
//type: �¼�����(���ӡ��Զ��������Զ��ػ����ճ�),�������ӡ��Զ����ػ��¼�
//      �ֻ��˵���û���������ã������ճ��¼��ֻ��˵�������������
//
//fre_mode: ģʽ(һ�Ρ�ÿ�졢ÿ�ܡ�ÿ�µ�),�������ӡ��Զ����ػ��¼�ģʽ��һ�Ρ�
//          ÿ�졢ÿ�ܡ��Զ��壻�����ճ��¼�ģʽ��һ�Ρ�ÿ�졢ÿ�ܡ�ÿ�¡�ÿ��
//
//fre: ��fre_mode���ʹ�ã���ģʽ��ÿ�ܻ����Զ���ʱ��Ч����һ���ֽ�8bit��ʾ
//      һ�������ļ����¼�Ӧ�÷�����һ��bit��ʾһ���е�һ��
//
//year,mon,day,hour,minute: �ô��һ���¼���һ�ν�Ҫ����������ʱ��,����ͨ��
//      ����Ĳ���(is_on, type, fre_mode, fre)����ó��ģ���������һ��ģʽ
//      ��ÿ�������ʱ�����Զ����ݵ�ʱ�����ں�ʱ�������һ�η��������ں�ʱ�䣬
//      �����ǵ������Щʱ��Ҳ�����ǵڶ��죻��������ӵ�������ʱ����Ϊģʽ��
//      ÿ�죬�����ᱻ�رգ����Ǽ�����һ�η��������ں�ʱ��
//
//
//
//�����¼��ĺ�����Ҫ�������漸����
//MMIALM_Init���ֻ�����ʱ��ʼ��ȫ�ֱ������������ڵ���������������֮ǰ����
//
//MMIALM_GeEvent(����Ӧ����MMIALM_GetEvent)��ͨ��Id���һ���¼��ṹ
//MMIALM_ModifyEvent���޸�һ���¼�
//MMIALM_AddOneEvent������һ���¼�
//MMIALM_DelOneEvent��ɾ��һ���¼�
//MMIALM_DelEvents��ɾ��һ���¼�
//
//MMIALM_UpdateEvents�����ֻ���ʱ���������ú���Ҫ��������������������е�
//                    �¼���¼
//
//MMIALM_HasActiveEvent���鿴��ǰ�Ƿ��м�����¼�(ʱ�䵽������ѵ�)������
//          Idle�����£��ж��Ƿ���ʾ����ͼ��ʱʹ��
//
//MMIALM_GetArrivedEvent, ��MMI�յ�������Ϣʱ�����������������ú͵�ʱ����
//      ʱ����ƥ����¼���Ȼ������¼�(ɾ�����߼�����һ�η�����ʱ�䣬�޸��¼�) 
/************************************************************************/

/*****************************************************************************************************/
//���ӵ�ʱMMI�յ�MISC_MSG_RTC_ALARM��Ϣ,����MMIALM_StartAlarm����, ��������
//��������֮һ����:
//1. MMIAPIALM_StartAlarm -> OpenAlarmExpiredWin (�������)
//2. MMIAPIALM_StartAlarm -> PushDelayedEvent -------->(�����ֻ����ڽ���������Ҫ����,��Ҫ�ȴ�������������Ӧ) 
//                     -> MMIAPIALM_CheckAndStartDelayedAlarm -> OpenAlarmExpiredWin
//3. MMIAPIALM_StartAlarm -> PushDelayedEvent -------->(�����ֻ����ڽ���������Ҫ����,��Ҫ�ȴ�������������Ӧ) 
//                     -> MMIALM_PopupExpiredMsg
/*****************************************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "mmi_app_accessory_trc.h"
#include "tb_dal.h"
#include "window_parse.h"
//#include "win.def"
#include "mmk_app.h"
#include "guitext.h"
#include "guilcd.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmicc_export.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#ifdef DPHONE_SUPPORT
//#include "mmicalc_export.h"  //masked this line for pclint
#else
#include "mmicalc_export.h"  
#endif
#include "mmiwclk_export.h"
#include "mmiphone_export.h"
#include "mmialarm_export.h"
//#include "mmiacc_winmain.h"

//#include "mmigame_export.h"
#include "guimsgbox.h"
#include "mmidisplay_data.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiidle_subwintab.h"
#include "mmi_textfun.h"
#include "mmk_timer.h"
#include "mmk_msg.h"
#include "mmiidle_export.h"
#include "mmiudisk_export.h"
#include "mmi_appmsg.h"
#include "mmikl_export.h"
#include "mmischedule_export.h"
#include "mmi_solarlunar.h"
#include "mmialarm_id.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmialarm_menutable.h"
#include "mmialarm_nv.h"
#include "mmialarm_anim.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "guilabel.h"
#include "guibutton.h"
#include "mmipub.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef MAINLCD_SIZE_128X64
#include "mmialarm_position.h"
#endif
#include "mmiudisk_internal.h"
//#include "mmist_export.h"
#include "mmicountedtime_export.h"//@zhaohui, counted time
#include "mmiudisk_export.h"
#include "mmi_mainmenu_export.h"
#include "mmisd_export.h"
#include "mmifmm.h"
#include "mmicc_id.h"
#include "mmifmm_export.h"
#include "mmisms_export.h"
//#include "mmimpeg4_dc.h"
//#include "mmiset_wintab.h"
#include "mmimms_export.h"
#include "mmipicview_export.h"
#include "mmistk_export.h"
#include "guires.h"
#include "guiedit.h"
#include "guistring.h"
#include "mmialarm_service.h"
//#include "mmiset_display.h"
#include "mmiset_id.h"
#ifdef PIM_SUPPORT
#include "vcalendar_parse.h"
#endif /* PIM_SUPPORT */

#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#include "guiform.h"
#include "guisetlist.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
//#include "mmiset_set_ring.h"

#ifdef FM_SUPPORT
#include "mmifm_export.h"
#include "mmifm_internal.h"
#endif
#include "gpio_prod_api.h"
#ifdef MMI_AZAN_SUPPORT
#include "mmiazan_export.h"
#endif
#include "mmi_text.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmidc_export.h"
#if defined (MMI_PDA_SUPPORT) || defined (MMI_ALARM_DEL_ADD_SUPPORT)
#include "mmialarm_menutable.h"
#endif

#if defined(IM_ENGINE_SOGOU)
#include "wdp_update_wintab.h"
#include "mmiim_nv.h"
#endif
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmiwidget_id.h"
#include "mmiwidget_anim.h"
#include "mmiwidget_image.h"
#include "mmiwidget_text.h"
#include "mmiwidget_export.h"
#endif
#include "guictrl_api.h"
#include "mmiacc_image.h"
#ifdef SCREENSAVER_SUPPORT
#include "mmiss_export.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#include "mmialarm_internal.h"
#include "guitab.h"
#include "mmiacc_id.h"
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmiatv_export.h"
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmicom_banner.h"
//#include "mmicl_export.h"
#include "mmiacc_event.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define ALERT_RING_DURATION                 54000   // about one minute
#define BACKGROUND_ALERT_RING_DURATION      10000   // ��Լ������

// ������ʱ��,��ʱ10����
#define ALM_DELAY_TIME_SEC          600

#define ALM_EVENT_TIME_STR_LEN      5

#define ALM_POWEROFF_DELAY_SEC        30//�ػ�����ʱ������,�����ӱ���ʱ30��

//for event ring info save use
#define MMIALM_RING_INFO_FILE_NAME_LEN   20
#define MMIALM_RING_INFO_FILE_SUFFIX  "_Ring.ini"

#define WEEKS_STR_BUFF_LEN  100

#define ALM_MODE_LIST      7

#ifdef DPHONE_SUPPORT
#define MMI_LISTBOX_LEFT      0

#define ALM_EDIT_FOCUS_LINE_COLOR   MMI_BLACK_COLOR
#define ALM_EDIT_CLEAN_LINE_COLOR   MMI_WHITE_COLOR
#endif

#if defined(MMI_ALARM_MINI_SUPPORT) && defined(MMI_GUI_STYLE_MINISCREEN)
#ifdef  MAINLCD_SIZE_128X160
  //DE �����µ��������ã��������ó�3����ʽ
  #if defined(MAINLCD_SIZE_128X160) && defined(MULTI_LAN_SUPPORT_DE)
    #define ALARM_MODE_LEFT    5
    #define ALARM_MODE_RIGHT  108
    #define ALARM_ICON_LEFT   105
  #else
    #define ALARM_MODE_LEFT    46
    #define ALARM_MODE_RIGHT  120
    #define ALARM_ICON_LEFT   105
  #endif
#elif defined MAINLCD_SIZE_176X220
//DE�µ�����
#define ALARM_MODE_LEFT   (MMI_MAINSCREEN_WIDTH*1/3+MMI_LIST_ICON_LEFT_SPACE*2)
#define ALARM_ICON_LEFT   (MMI_MAINSCREEN_WIDTH-MMI_LIST_ITEM_SELECT_ICON_WIDTH-5)
#define ALARM_MODE_RIGHT  (ALARM_ICON_LEFT-2)

#else
#define ALARM_MODE_LEFT   (MMI_MAINSCREEN_WIDTH*1/3+MMI_LIST_ICON_LEFT_SPACE*2)
#define ALARM_ICON_LEFT   (MMI_MAINSCREEN_WIDTH-MMI_LIST_ITEM_SELECT_ICON_WIDTH-3)
#define ALARM_MODE_RIGHT  (ALARM_ICON_LEFT-2)
#endif
#endif

//[modify alarm edit UI]
#define ALM_EDIT_LIST_ITEM_LABEL_INDEX    0
#define ALM_EDIT_LIST_ITEM_CONTENT_INDEX  1

#define ALM_REPEAT_LIST    7
#define ALM_SNOOZE_NUM     5

#define ALM_DELAY_TIME_2_MIN_SEC     120
#define ALM_DELAY_TIME_5_MIN_SEC     300
#define ALM_DELAY_TIME_10_MIN_SEC    600
#define ALM_DELAY_TIME_15_MIN_SEC    900
#define ALM_DELAY_TIME_20_MIN_SEC    1200
//[end]

//[modify alarm edit UI]
#define ALARM_EDIT_ITEM_HEIGHT 66
//[end]

//[multi country language alarm ring tone]
extern uint8 g_clock_ring_max_num;
extern uint8 g_clock_ring_default_id;
//[end]

#ifdef  MAINLCD_SIZE_128X160
#define MMI_ALARM_BG_COLOR MMI_WHITE_COLOR
#define MMI_ALARM_ITEM_FONT MMI_DEFAULT_NORMAL_FONT
#else
#define MMI_ALARM_BG_COLOR MMI_BACKGROUND_COLOR
#define MMI_ALARM_ITEM_FONT MMI_DEFAULT_BIG_FONT
#endif

/**--------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **--------------------------------------------------------------------------*/
#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL THEMELIST_ITEM_STYLE_T          s_my_style = {0};
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
 
typedef struct _RADIO_BUTTON_TAG_
{
    BOOLEAN is_sel;
    BOOLEAN is_focus;
    GUI_POINT_T point;
    GUI_RECT_T rect;
    MMI_TEXT_ID_T text_id;
    MMI_IMAGE_ID_T image_id[3]; // ѡ��/δѡ��/����
    ALARM_CONTROL_ID_E ctrl_id;
}RADIO_BUTTON_T;

LOCAL BOOLEAN s_weeks_button_is_selected[ALM_WEEK_DAY_NUM]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
LOCAL MMISRV_HANDLE_T s_alarm_play_handle = 0;
#ifdef MMI_ETWS_SUPPORT
LOCAL uint8               s_line_num     = 0;
LOCAL MMI_STRING_T s_alarm_str1 = {0};
LOCAL MMI_STRING_T s_alarm_str2 = {0};
LOCAL MMI_IMAGE_ID_T    s_icon       = IMAGE_NULL;
LOCAL MMI_WIN_PRIORITY_E s_win_priority = WIN_ONE_LEVEL;
LOCAL BOOLEAN s_is_need_ring_and_vib = TRUE;
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
#define CLK_ROTATE_PIC_NUM 2

#ifdef MAINLCD_SIZE_320X480
#define CLK_CENTER_X  171
#define CLK_TIME_X   100
#define CLK_TIME_Y   30
#define CLK_DATE_X   117
#define CLK_DATE_Y   77 
#elif defined MAINLCD_SIZE_240X400
#define CLK_CENTER_X  137
#define CLK_TIME_X   60
#define CLK_TIME_Y   30
#define CLK_DATE_X   77
#define CLK_DATE_Y   77 
#else
#define CLK_CENTER_X  136
#define CLK_TIME_X   60
#define CLK_TIME_Y   0//30
#define CLK_DATE_X   77
#define CLK_DATE_Y   0//77
#endif

typedef struct {
    int32 angel;
    GUI_POINT_T  rotate_point;
    GUI_POINT_T  center_point;
}CLK_ROTATE_INFO_T;

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
LOCAL CLK_ROTATE_INFO_T  s_rotate_info_arr[CLK_ROTATE_PIC_NUM]=
{
    {
        359,
        {0,300},
        {154, 271}
    },
    {
        1,
        {250,300},
        {154, 271}
    }
};
#endif

LOCAL int s_pic_index = 0;
LOCAL uint8 s_clk_timer_id = 0;

#endif
#ifdef DPHONE_SUPPORT
LOCAL uint8                 s_edit_active_ctrl = 0;

// �༭����active�ؼ�
LOCAL const uint32          s_edit_active_ctrl_attr[] =
{
  //  MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,
    MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
    MMIALM_EDIT_RING_TEXT_CTRL_ID,
    MMIALM_EDIT_MODE_SETLIST_CTRL_ID
};

LOCAL MMI_TEXT_ID_T s_mode_week_list_arr[ALM_MODE_WEEK_NUM] =
{
    TXT_SUNDAY, 
    TXT_MONDAY, 
    TXT_TUESDAY,        
    TXT_WEDNESDAY,
    TXT_THURSDAY,
    TXT_FRIDAY, 
    TXT_SATURDAY
};
//ע��:��������£�focus���������귶Χһ��Ҫ���������label���������棬�������ص���
LOCAL GUI_RECT_T focus_rec[3] =
{
    {ALM_EDIT_X2 - 1, ALM_EDIT_Y1 - 1, ALM_EDIT_X3 + 1, ALM_EDIT_Y1 + ALM_EDIT_HIGHT}, //lint !e778 ,  1st line for alarm time, ALM_EDIT_Y1 + 1 ��Ϊ����ʾ������һ���򣬷�����ϵ������ᱻ�ڸǡ�
    {ALM_EDIT_X2 - 1, ALM_EDIT_Y2 - 1, ALM_EDIT_X3 + 1, ALM_EDIT_Y2 + ALM_EDIT_HIGHT},// second line for ring type,ALM_EDIT_Y2 - 1 ��Ϊ�˰���������������ʾ�����������ڸ�.
    {ALM_EDIT_X2 - 1, ALM_EDIT_Y3 - 1, ALM_EDIT_X3 + 1, ALM_EDIT_Y3 + ALM_EDIT_HIGHT} // 3rd line for alarm mode: once,everyday or every week;
};  //lint !e778
#endif
// Don't increase Day for ONCE ALARM
//1.System update;
//2.Alarm occure;
LOCAL BOOLEAN g_ModifyEventDate = TRUE;

BOOLEAN s_alarm_win_redisplay = FALSE;
BOOLEAN s_alarm_vibrate_delayed_flag = FALSE; // for etws alert, if alert comes in etws alert first 10.5s , alarm should not vibrate or ring.

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/            

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Set alarm flag to modify event date time
//  Global resource dependence : 
//  Author:        
//  Note:   
/*****************************************************************************/
LOCAL void Alarm_SetModifyEventDateFlag(BOOLEAN flag);

/*****************************************************************************/
//  Description :  Get alarm flag to modify event date time
//  Global resource dependence : 
//  Author:        
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN Alarm_GetModifyEventDateFlag(void);

/*****************************************************************************/
//  Description : Open Alarm window 
//  Global resource dependence : 
//  Author:        lin.lin
//  Note:   
/*****************************************************************************/
LOCAL void OpenAlarmAliveWin(
                             MMI_WIN_PRIORITY_E win_priority
                             );

/*****************************************************************************/
//  Description : ��wday����ҵ���һ��ƥ��ģ�����offset
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:   
/*****************************************************************************/
LOCAL uint8 RoundWeek(
                      uint8 wday,
                      uint8 fre
                      );

/*****************************************************************************/
//  Description : ��wday��һ������ҵ���һ��ƥ��ģ�����offset
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:   
/*****************************************************************************/
LOCAL uint8 RoundWeek2(
                       uint8 wday,
                       uint8 fre
                       );

/*****************************************************************************/
//  Description : when alarm interrupt is coming, relative alarm will 
//                be activated.
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEventAliveWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : the process message function of the ALARM
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmMainWinMsg(
                                         MMI_WIN_ID_T       win_id,     
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );             

/*****************************************************************************/
//  Description : pop up a msg 
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleAlarmExpiredWinMsg(
//                                              MMI_WIN_ID_T       win_id, 
//                                              MMI_MESSAGE_ID_E   msg_id,     
//                                              DPARAM             param
//                                              );                     

/*****************************************************************************/
//  Description : increase current date by one
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL  void IncreaseDateByOne(
                              SCI_DATE_T  *pdate_ptr
                              );

/*****************************************************************************/
//  Description : �Ƚ�event���Ƿ�͵�ǰ����ʱ��ƥ��
//  Global resource dependence : 
//  Author: bin.ji
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsMatchEvent(
                           SCI_DATE_T                   first_date, 
                           SCI_TIME_T                   first_time, 
                           const MMIACC_SMART_EVENT_FAST_TABLE_T   *event_ptr
                           );


/*****************************************************************************/
//  Description : compare date and time to another group, find smaller(earlier)
//  Global resource dependence : 
//  Author: taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL int16 CompareDateTime(
                            SCI_ALARM_T first, 
                            SCI_ALARM_T second
                            );

/*****************************************************************************/
//  Description : push ID of current dealyed event to stack to save 
//                when a call is active
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL void PushDelayedEvent(
                            void
                            );

/*****************************************************************************/
//  Description : ClearDelayedEvent
//  Global resource dependence : 
//  Author:        liqing.peng
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void ClearDelayedEvent(void);

#ifdef DPHONE_SUPPORT
LOCAL MMI_RESULT_E  HandleAlarmPowerOnOffEditWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM           param
                                       );
#endif

/*****************************************************************************/
//  Description : HandleAutoPowerOnOffWinMsg
//  Global resource dependence : 
//  Author:        lin.lin
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOnOffWinMsg( 
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : auto power on prompt window
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOnExpired(
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : auto power off prompt window
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOffExpired(
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : open alarm expired win according to the alarm type
//  Global resource dependence : 
//  Author:      lin.lin
//  Note:            
/*****************************************************************************/
LOCAL BOOLEAN OpenAlarmExpiredWin(
                                  MMIACC_SMART_EVENT_E    type
                                  );

/*****************************************************************************/
//  Description : select ring of alarm
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmRingWinMsg(
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : select fixed ring tone
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmFixedRingWinMsg(
                                MMI_WIN_ID_T        win_id, 
                                MMI_MESSAGE_ID_E    msg_id, 
                                DPARAM              param
                                );

/*****************************************************************************/
//  Description : Stop ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   
/*****************************************************************************/
LOCAL void StopRingOrVib(BOOLEAN is_close_ring_timer);

/*****************************************************************************/
//  Description : Start ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   
/*****************************************************************************/
LOCAL void StartRingOrVib(void);

/*****************************************************************************/
//  Description : Start ring for ring or vibrate/ring
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   leon.wang add
/*****************************************************************************/
LOCAL void StartRingForRingOrVib(uint8 opt_type);

/*****************************************************************************/
//  Description : SetNextAlarmTimeByCurrent
//  Global resource dependence : 
//  RETURN :  
//  Author:  liqing.peng
//  Note:  ����event_ptr�Ƿ���������(���ӡ��Զ����ػ����ճ�)��
//         ģʽ(һ�Ρ�ÿ�졢ÿ�ܵ�)��ʱ�����ڣ�
//         ����event_ptr��һ�η�����ʱ��
/*****************************************************************************/
LOCAL BOOLEAN SetNextAlarmTimeByCurrent(
                                        MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr         //in & out
                                        );

/*****************************************************************************/
//  Description : find latest active smart event, and set to alarm according to 
//                current SYS time and date
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindLatestEventToSet(void);

/*****************************************************************************/
//  Description : handle alarm edit windows messages
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );
                                   
/*****************************************************************************/
// Description : IncreaseTimeBySecond
// increase current time by seconds inputed.
// Global resource dependence : 
// Author:        taul.zhu
// Note:   
/*****************************************************************************/
LOCAL  void IncreaseTimeBySecond(
                                 SCI_DATE_T *pdate,
                                 SCI_TIME_T *ptime,
                                 uint16      sec
                                 );

/*****************************************************************************/
//  Description : get the events witch will happen at the same time
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetSameTimeSchEvent(
                                 const MMIACC_SMART_EVENT_FAST_TABLE_T *latest_event,
                                 uint16 index,
                                 uint16 start_index,
                                 MMIACC_SCH_LIST_T *p_sch_list
                                 );

/*****************************************************************************/
//  Description : handle past event win msg
//  Global resource dependence : 
//  Author:       xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePastEventWinMsg(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );

/*****************************************************************************/
//  Description : Open bypast sch event win(�򿪹����¼�����)
//  Global resource dependence : 
//  Author: xiaoqing.lu 
//  Note:   
/*****************************************************************************/
LOCAL void OpenPastSchEventWin(
                               MMI_WIN_PRIORITY_E win_priority,
                               uint16 event_id
                               );

/*****************************************************************************/
//  Description : get the events witch will happen at the same time
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneSchEventInList(
                                     uint16 index, // IN
                                     MMIACC_SCH_LIST_T *p_sch_list       // IN OUT:
                                     );

/*****************************************************************************/
//  Description : DisplayTheRingName
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void DisplayTheRingName(void);

/*****************************************************************************/
//  Description : close delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN CloseDelayAlarm(
                              uint16 event_id, 
                              MMIACC_SMART_EVENT_FAST_TABLE_T event,
                              BOOLEAN    is_on
                              );
/*****************************************************************************/
//  Description : set delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SetDelayAlarmTime(
                                uint16 event_id, 
                                MMIACC_SMART_EVENT_T *event_ptr,
                                BOOLEAN    is_auto
                                );

/*****************************************************************************/
//  Description : find if the alarm is set
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAlarmSet(void);

/*****************************************************************************/
//  Description : Start timer about alarm
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void StartAlarmTimer(
                           uint8 *timer_id,
                           uint32 time_out,
                           BOOLEAN is_period
                           );

/*****************************************************************************/
//  Description : to append two line check item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoLineListItem(   
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id_1,
                            MMI_TEXT_ID_T    text_id_2,
                            uint16           event_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            );

#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
/*****************************************************************************/
//  Description : append three line check item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendThreeLineListItem(   
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id_1,
                            MMI_TEXT_ID_T    text_id_2,
                            uint16           event_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            );
#endif

/*****************************************************************************/
//  Description : ������ʱ����
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SetDelayAlarm(
                            uint16 event_id, 
                            MMIACC_SMART_EVENT_T *event_ptr,
                            BOOLEAN    is_on
                            );

/*****************************************************************************/
//  Description : get the delayed event of event_id
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMIACC_SMART_EVENT_FAST_TABLE_T *GetDelayEvent(
                                                     uint16 event_id
                                                     );

/*****************************************************************************/
//  Description : ��鲢���������¼�(��������,�Զ��������¼�����ʱ����)
//  Global resource dependence : 
//  Author: zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void CheckandStartExpiredEvent( void );

/*****************************************************************************/
// Description : ����alarmĿ¼
// Global resource dependence :
// Author:zhaohui
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitAlarmFolder(void);

/*****************************************************************************/
//  Description : ����event_idӳ��ɶ�Ӧ�ü�¼��ring��Ϣ
//  Global resource
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetRingInfoFileName(
                                  uint16 event_id,  
                                  wchar *u16filename_ptr,//OUT
                                  uint16 u16filename_max_len
                                  );

/*****************************************************************************/
//  Description : ������id��Ϣ�ļ��ж�ȡ����id��·����Ϣ  
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN ReadRingInfoFileName(
                                   uint16 event_id,
                                   MMIACC_SMART_EVENT_RING_T *ring_ptr//IN
                                   );

/*****************************************************************************/
//  Description : ��ring_ptr���浽����event_idӳ��ɵ�����id��Ϣ�ļ��� 
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN SaveRingInfoToFile(
                                 uint16 event_id,
                                 const MMIACC_SMART_EVENT_RING_T *ring_ptr//IN
                                 );
/*****************************************************************************/
// 	Description : save time
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimeEditWin(void);                                 

/*****************************************************************************/
//  Description : ɾ������id��Ϣ������id�ļ�  
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteRingInfoFile(
                                 uint16 event_id                        
                                 );

/*****************************************************************************/
//  Description : modify one event 
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN ModifyEvent(
                          uint16                    event_id, //in
                          MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr,  //in,out
                          BOOLEAN is_set_alarm
                          );

/*****************************************************************************/
//  Description : memset 0 to &s_event_fast_table[event_id],but hold s_event_fast_table[event_id].service_id
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void CleanEventFastTable(uint16 event_id);

/*****************************************************************************/
//  Description : add item for setlist
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  AddItemSetlist(void);

/*****************************************************************************/
//  Description : process the MIDSK message
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
// LOCAL void EditWinMIDSKProcess(
//                                 MMI_WIN_ID_T win_id //in
//                                 );
                                
/*****************************************************************************/
//  Description : set title for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetTitleEditWin(
                                MMI_WIN_ID_T win_id //in
                                );

/*****************************************************************************/
//  Description : set name ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetNameParamEditWin(void);

/*****************************************************************************/
//  Description : set time ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetTimeParamEditWin(void);

/*****************************************************************************/
//  Description : set ring ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetRingParamEditWin(void);

/*****************************************************************************/
//  Description : set mode ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetModeParamEditWin(void);

/*****************************************************************************/
//  Description : set  weeks ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetWeeksParamEditWin(MMI_WIN_ID_T      win_id);

/*****************************************************************************/
//  Description : set  weeks display
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetWeeksDisplayEditWin(void);

/*****************************************************************************/
//  Description : save name
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveNameEditWin(void);



/*****************************************************************************/
//  Description : save mode
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  SaveModeEditWin(void);

// LOCAL void AppendWeeksListItem(
//                              MMI_CTRL_ID_T    ctrl_id //in
//                              );

/*****************************************************************************/
//  Description : save the select weeks
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
// LOCAL uint16 SaveSelectWeeks(
//                                                MMI_CTRL_ID_T    ctrl_id //in
//                                                );

/*****************************************************************************/
//  Description : display Alarm window
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayAlarmWin(
                          MMI_WIN_ID_T win_id, 
                          MMIPUB_INFO_T    *win_info_ptr
                          );

LOCAL uint8  GetFreByEditWin(void);

/*****************************************************************************/
//  Description : past auto power on prompt window
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePastAutoPowerOn(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );
/*****************************************************************************/
//  Description : handle alarm edit windows penok messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  HandleAlarmEditPENOKMsg(
                                        MMI_HANDLE_T        win_id,
                                          uint32 ctrl_id
                                          );

/*****************************************************************************/
//  Description : handle alarm edit windows FormSwitchActive messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  HandleAlarmEditFormSwitchActiveMsg(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id
                                          );

/*****************************************************************************/
//  Description : Set EditWin Button bg
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetEditWinButtonBg(MMI_HANDLE_T    win_id,uint32 ctrl_id);


/*****************************************************************************/
//  Description : save fre
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveFreEditWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void setAlarmEditSelectWeeksSoftkey(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id,
                                            BOOLEAN is_update);

/*****************************************************************************/
//  Description : ����ID�ж��Ƿ����ճ��¼�
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
//LOCAL BOOLEAN IsSCHEvent(uint16 event_id);

/*****************************************************************************/
//  Description : Ƶ�ʱ༭������Ϣ������
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFMEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : ����Ƶ�ʱ༭���ڿؼ�����
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetFMEditCtrlParam(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : �ж������Ƿ���Ҫ�ں�̨����
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsNeedBackgroundPrompt(void);

#if  defined DPHONE_SUPPORT || defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : form time str by time
//  Global resource dependence : 
//  Author:        zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void MMIALM_FormTimeStrbyTime(
                      uint8 hour,//in
                      uint8 min,//in
                      char* time_str,//out
                      uint16 time_str_max_len//in
                      );
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );

/*****************************************************************************/
//  Description : to handle set time
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTimeMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );


/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditFmFreMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );

#endif

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Discription: RecordAlarmRingCallBack
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:¼������Ϊ������callback
//
/*****************************************************************************/
LOCAL MMI_RESULT_E RecordAlarmRingCallBack(void);
#endif

#if defined(SCH_ASSISTANT_WIDGET)
LOCAL MMI_RESULT_E HandleSchPOPAlertWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
/*****************************************************************************/
//  Description : when alarm interrupt is coming, relative alarm will 
//                be activated.
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJumpClockEventAliveWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );
#endif

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
/*****************************************************************************/
//Description :   ALARM OPT
//Global resource dependence :  none
//Author: panvs
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmMainOptWin(
                                           MMI_WIN_ID_T    win_id,   // ����ID
                                           MMI_MESSAGE_ID_E   msg_id,   // ��ϢID
                                           DPARAM             param     // ��Ϣ����
                                           );

/*****************************************************************************/
//Description :   GetAlarmRealId
//Global resource dependence :  none
//Author: panvs
//Note: 
/*****************************************************************************/
LOCAL uint16  GetAlarmRealId(uint16 id);

LOCAL uint16  FindAlarmUnuseId(void);

#endif
//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// LOCAL void SetListItemStyle(MMI_CTRL_ID_T ctrl_id, uint16 index);
// #endif

/*****************************************************************************/
//  Description : ���ڹػ���������ʱ��ʼ��lcd����
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
//LOCAL void initLcdBavkground(void);

//[add new alarm list UI]
LOCAL MMI_RESULT_E HandleCustomAlarmMainWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );

LOCAL void AppendNewAlarmItem(MMI_CTRL_ID_T ctrl_id);

LOCAL void SetAlarmListData(
                        MMI_WIN_ID_T      win_id,
                        MMI_CTRL_ID_T     list_ctrl_id,
                        MMI_CTRL_ID_T     btn_ctrl_id
                        );

LOCAL MMI_RESULT_E  HandleCustomAlarmEditWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );

LOCAL void SetCustomEditNameWin(MMI_WIN_ID_T win_id);

LOCAL void SetEditRepeatWin();

LOCAL void SetCustomEditTimeWin(void);

LOCAL void  HandleAlarmEditMskMsg(
                                MMI_HANDLE_T    win_id,
                                uint32 ctrl_id
                                );

LOCAL MMI_RESULT_E  HandleAlarmRepeatListWinMsg(
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  DPARAM            param
                                                  );

LOCAL void AppendAlarmRepeatListBoxItem(
                               MMI_CTRL_ID_T    ctrl_id
                               );

PUBLIC void AppendListItemByTextId(
                                    MMI_TEXT_ID_T        text_id,
                                    MMI_CTRL_ID_T        ctrl_id,
                                    GUIITEM_STYLE_E      item_style
                                    );

LOCAL void UpdateAlarmFreMode(uint16 *index_ptr,uint8 sel_count);

LOCAL void GetRepeatModeString(
                            MMI_STRING_T *p_repeat_str,
                            MMIACC_MODE_TYPE_E fre_mode,
                            uint8 fre_mask
                            );

LOCAL MMI_RESULT_E HandleAlarmEditNameWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                );

LOCAL void SetEditRingParamWin(void);

LOCAL void SetSnoozeTimeParamEditWin(void);

LOCAL MMI_RESULT_E  HandleAlarmRingSelWinMsg(
                                               MMI_WIN_ID_T     win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );

LOCAL MMI_RESULT_E  HandleAlarmSnoozeTimeListWinMsg(
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  DPARAM            param
                                                  );

LOCAL void GetSnoozeTimeStr(MMI_STRING_T *p_snooze_time_str);

LOCAL uint16 GetSnoozeTimeByType(ALM_SNOOZE_TYPE_E snooze_type);

LOCAL MMI_RESULT_E  HandleAlarmEditOptWin(
                                           MMI_WIN_ID_T    win_id,   // ����ID
                                           MMI_MESSAGE_ID_E   msg_id,   // ��ϢID
                                           DPARAM             param     // ��Ϣ����
                                           );

LOCAL void SaveEvent(MMI_WIN_ID_T win_id);

LOCAL MMI_RESULT_E  HandleCustomAlarmMainOptWin(
                                           MMI_WIN_ID_T    win_id,   // ����ID
                                           MMI_MESSAGE_ID_E   msg_id,   // ��ϢID
                                           DPARAM             param     // ��Ϣ����
                                           );

LOCAL void SetMainWinSoftkeyByActiveCtrl(
                                            MMI_HANDLE_T    win_id,
                                            uint32          ctrl_id,
                                            BOOLEAN         is_need_update
                                            );

LOCAL MMI_RESULT_E HandleCustomAutoPowerOnOffWinMsg(
                                              MMI_WIN_ID_T        win_id,
                                              MMI_MESSAGE_ID_E    msg_id,
                                              DPARAM              param
                                              );

LOCAL void SetAutoPowerListItem(
                            MMI_CTRL_ID_T    ctrl_id
                            );

LOCAL MMI_RESULT_E  HandlePowerOnOffEditWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );

LOCAL BOOLEAN isEveryWeekNeedDestroyStr(
                                        MMIACC_MODE_TYPE_E fre_mode,
                                        uint8 fre);

LOCAL void DeleteOneAlarm(
                            EVENT_CURRENT_DATA_T *cur_event_ptr,
                            MMI_CTRL_ID_T list_ctrl_id,
                            uint16 cur_item_index
                            );

LOCAL void MMIALM_RemoveIdlePushMsg(
                                uint16 event_id,
                                MMIACC_SMART_EVENT_E alarm_type
                                );

LOCAL void SetAlarmDialogSoftkey(MMI_WIN_ID_T win_id);

LOCAL BOOLEAN IsAlarmNotifyInCall(MMIACC_SMART_EVENT_E type);
//[end]
LOCAL BOOLEAN GetEventAndDelayedIds(uint16 *eventid_ptr, // in/out
    uint16 *delayid_ptr); // out
LOCAL void HandleAlarmSoftkeyDissmissMsg(MMI_WIN_ID_T win_id,
        uint16 event_id,
        MMIACC_SMART_EVENT_E alarm_type,
        ALM_SNOOZE_TYPE_E snooze_t);

/*---------------------------------------------------------------------------*/
/*                          GLOBE DATA DECLARE                               */
/*---------------------------------------------------------------------------*/
/*MMI_TEXT_ID_T s_mode_week_list_arr[ALM_MODE_WEEK_NUM] =
{
    TXT_SUNDAY, 
    TXT_MONDAY, 
    TXT_TUESDAY,        
    TXT_WEDNESDAY,
    TXT_THURSDAY,
    TXT_FRIDAY, 
    TXT_SATURDAY
};*/

#define CLOCK_REFRESH_SEC_TIME    100
#define CLOCK_REFRESH_MIN_TIME    6000
#if !defined MMI_GUI_STYLE_TYPICAL
ALARM_CLOCK_TYPE_E s_alm_clk_type = ALM_CLOCK_TYPE_1;
LOCAL uint8	s_refresh_timer_id = 0;
#endif

const uint8 s_week_mask[ALM_MODE_WEEK_NUM] = {MASK_B0, MASK_B1, MASK_B2, MASK_B3, MASK_B4, MASK_B5, MASK_B6};
//[edit repeat list]
const uint8 s_week_mask_frm_mon[ALM_MODE_WEEK_NUM] = {MASK_B1, MASK_B2, MASK_B3, MASK_B4, MASK_B5, MASK_B6, MASK_B0};
//[end]

 const MMI_TEXT_ID_T s_alm_week_text_id[ALM_WEEK_DAY_NUM] =
{
    TXT_SHORT_SUNDAY,
    TXT_SHORT_MONDAY,
    TXT_SHORT_TUESDAY,
    TXT_SHORT_WEDNESDAY,
    TXT_SHORT_THURSDAY,
    TXT_SHORT_FRIDAY,
    TXT_SHORT_SATURDAY
};
/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                               */
/*---------------------------------------------------------------------------*/
LOCAL MMIACC_SCH_LIST_T    s_bypast_list = {0}; // ��¼�����¼�
// edit����button�ؼ���λ��
LOCAL const uint8 s_RTC_month_info[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LOCAL const uint8 s_RTC_month_info_leap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

LOCAL EVENT_CURRENT_DATA_T  s_current_event_info = {0, 0, {0}};/*lint !e651*/
LOCAL MMIACC_SMART_EVENT_T  s_arrived_event = {0};  // �����¼�
LOCAL MMIACC_SMART_EVENT_T  s_poweroff_event = {0}; // �ػ��¼�

LOCAL MMIACC_SMART_EVENT_FAST_TABLE_T   s_event_fast_table[MMIACC_EVENT_MAX_NUM] = {0};//���,���������,��¼������/�ճ�/���ػ����¼��Ļ�����Ϣ

LOCAL uint8                 s_event_timer_id = 0;
LOCAL uint8                 s_past_event_timer_id = 0;
LOCAL BOOLEAN               s_is_event_delayed = FALSE;
LOCAL uint16                s_open_event_id = 0;//�ϴ�open_win��ʱ���alarm_id��0-2
LOCAL uint16                s_arrived_event_id = 0;  //ֻ��¼��ǰ���ӵ�alarm_id,0-2
//LOCAL BOOLEAN               is_week_day_switch = FALSE;
LOCAL GUI_POINT_T           s_last_point = {0};
LOCAL uint8                 s_30seconds_timer_id = 0; 
LOCAL uint8                 s_3seconds_timer_id = 0; 
LOCAL uint8                 s_1seconds_timer_id = 0; 
LOCAL uint8                 s_30seconds_count = 30;  // ����
LOCAL uint8                 s_vibrator_timer_id = 0;  
LOCAL BOOLEAN               is_vibrator_on = FALSE;  
LOCAL BOOLEAN               s_alarm_is_interrupted = FALSE;  
LOCAL uint16                s_RefCount = 0; //alarm�ڴ����ʱʹ�õ����ü���,�Ա����η��������

LOCAL uint16                s_past_event_id=0;

LOCAL BOOLEAN               is_ring_on = FALSE;
LOCAL ALM_RING_TYPE_E       s_cur_alarm_ring_type=ALM_FIX_RING;
LOCAL BOOLEAN               s_alarm_notifying = FALSE;

// ���ڸ������н�text_id��ʹ�ù�����ʹ��s_alarm_mode_text������
// ÿ��ģʽʱ������button��ť��ֵ,
// LOCAL RADIO_BUTTON_T s_radio_but[ALM_MODE_NUM] = 
// {
//     {
//         FALSE,
//         FALSE,
//         {0, 0},
//         {0, 0,  0,0},
//         TXT_ALARM_MODE_ONCE,
//         {IMAGE_RADIO_SELECTED_ICON, IMAGE_RADIO_UNSELECTED_ICON, IMAGE_CHECK_SELECTED_ICON},
//         MMIALM_EDIT_MODE_LIST1_CTRL_ID
//     },
//     {
//         FALSE,
//         FALSE,
//         {0, 0},
//         {0, 0,  0, 0},
//         TXT_ALARM_MODE_EVERYDAY,
//         {IMAGE_RADIO_SELECTED_ICON, IMAGE_RADIO_UNSELECTED_ICON, IMAGE_CHECK_SELECTED_ICON},
//         MMIALM_EDIT_MODE_LIST2_CTRL_ID
//     },
//     {
//         FALSE,
//         FALSE,
//         {0, 0},
//         {0, 0,  0, 0},
//         TXT_ALARM_MODE_WEEK,
//         {IMAGE_RADIO_SELECTED_ICON, IMAGE_RADIO_UNSELECTED_ICON, IMAGE_CHECK_SELECTED_ICON},
//         MMIALM_EDIT_MODE_LIST3_CTRL_ID
//     }
// };
LOCAL MMI_TEXT_ID_T s_alarm_mode_text[ALM_MODE_NUM]=
{
    TXT_ALARM_MODE_ONCE,
    TXT_ALARM_MODE_EVERYDAY,
    TXT_ALARM_MODE_WEEK
};

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// main alarm window
WINDOW_TABLE( ALARM_MAINWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleAlarmMainWinMsg ),    
    WIN_ID( MMIALM_MAINWIN_ID ),
#if defined(MMI_ALARM_MINI_SUPPORT) || (!defined(WORLD_CLOCK_SUPPORT) && !defined(STOPWATCH_SUPPORT) && !defined(COUNTEDTIME_SUPPORT))
    WIN_TITLE(TXT_ALARM_CLOCK),
#endif
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, TXT_COMMON_COMMON_BACK),
#else
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_SOFTKEY_SELECT_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
#endif    
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_LISTALL_CTRL_ID),
    END_WIN
}; 

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
//���������˵�(�½����༭��ɾ��) 
WINDOW_TABLE( MMIALM_OPTWIN_TAB ) =
{
    WIN_FUNC((uint32)HandleAlarmMainOptWin ),
    WIN_ID( MMIALM_OPTWIN_ID ),
    WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_POPMENU_CTRL(MENU_ALARM_OPT,MMIALM_OPT_MENU_CTRL_ID),
    END_WIN   
};
#endif

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( ALARM_RINGWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmRingWinMsg ),    
    WIN_ID( MMIALM_RINGLIST_WIN_ID ),
    //WIN_SOFTKEY(STXT_SOFTKEY_OK_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    //WIN_TITLE(TXT_RING),
    CREATE_POPMENU_CTRL(MENU_ALARM_SETTING_RING, MMIALM_RINGLIST_CTRL_ID),
    END_WIN
};
#else
// set alarm ring
WINDOW_TABLE( ALARM_RINGWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmRingWinMsg ),    
    WIN_ID( MMIALM_RINGLIST_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    WIN_TITLE(TXT_RING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIALM_RINGLIST_CTRL_ID),
    END_WIN
}; 
#endif
// set alarm fixed ring
WINDOW_TABLE( ALARM_FIXEDRINGWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmFixedRingWinMsg ),    
    WIN_ID( MMIALM_FIXEDRINGLIST_WIN_ID ),
    WIN_TITLE(TXT_RING_FIXED),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_STATUSBAR,
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),

    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIALM_FIXEDRINGLIST_CTRL_ID),
     END_WIN
};

//�Զ����ػ�����
WINDOW_TABLE( AUTOPOWER_MAINWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAutoPowerOnOffWinMsg ),  
    WIN_ID( MMIAUTOPOWER_WIN_ID ),
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
#endif
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_AUTOPOWER_LIST_CTRL_ID),
#ifdef DPHONE_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_EDIT, TXT_COMMON_COMMON_BACK),
#else
#ifndef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(STXT_SELECT, TXT_EDIT, TXT_COMMON_COMMON_BACK),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
#endif
#endif
    END_WIN
}; 
WINDOW_TABLE( ALARM_EDITWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleAlarmEditWinMsg), 
    WIN_ID(MMIALM_EDIT_WIN_ID),
#ifdef DPHONE_SUPPORT
        //  WIN_STATUSBAR, //masked this for cr221626
        //ע��:�������Ҫtitle,Ҳ������WIN_TITLE(TXT_NULL),����paint title�ؼ�ʱ,lcd�ĵ�һ�л���1�����ص�title����;
        //�������ܻ��ڸ�����Ҫ����0�е�����;
       //  WIN_TITLE(TXT_NULL), /*note:�����mask����,��һ�ν������ӱ༭����ʱ,��һ����������ϵ������ᱻ�ڸ�.*/
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),

        // 1st line--time
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,  MMIALM_EDIT_TITLE_LABEL_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID),

        // second line--ring type
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIALM_EDIT_RING_LABEL_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIALM_EDIT_RING_TEXT_CTRL_ID),
                    
        // 3rdline--alarm mode
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,  MMIALM_EDIT_MODE_LABEL_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MMIALM_MODELIST_CTRL_ID),

#else   
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    //WIN_SOFTKEY(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_EDITWIN_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    CHILD_SOFTKEY_CTRL(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_NAME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_NAME_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_TIME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_TITLE_EDIT_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#endif

//#ifdef MMI_PDA_SUPPORT
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIALM_EDIT_RING_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, MMIALM_EDIT_RING_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_RING_TYPE_CTRL_ID,MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID),
#ifndef MMI_RECORD_SUPPORT
        CHILD_BUTTON_CTRL(FALSE, IMAGE_DROPDOWNLIST_ARROW_PR, MMIALM_EDIT_RING_BUTTON_CTRL_ID, MMIALM_EDIT_RING_FORM_CTRL_ID),
#else
        CHILD_BUTTON_CTRL(TRUE, IMG_SCH_RECORD_ICON, MMIALM_EDIT_RING_BUTTON_CTRL_ID, MMIALM_EDIT_RING_FORM_CTRL_ID),       
#endif
//#else
//    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_RING_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
//        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
//        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_RING_TYPE_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
//#endif

//#ifndef MMI_PDA_SUPPOR
//        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIALM_EDIT_RING_TEXT_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
//#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIALM_EDIT_MODE_FORM_CTRL_ID, MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIALM_EDIT_MODE_FORM2_CTRL_ID, MMIALM_EDIT_MODE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIALM_EDIT_MODE_LABEL_CTRL_ID, MMIALM_EDIT_MODE_FORM2_CTRL_ID),
            CHILD_SETLIST_CTRL(TRUE, MMIALM_EDIT_MODE_SETLIST_CTRL_ID, MMIALM_EDIT_MODE_FORM2_CTRL_ID),
        CHILD_BUTTON_CTRL(FALSE, IMAGE_DROPDOWNLIST_ARROW_PR, MMIALM_EDIT_MODE_BUTTON_CTRL_ID, MMIALM_EDIT_MODE_FORM_CTRL_ID),
#else
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_MODE_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_LABEL_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_MODE_SETLIST_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
#endif
#ifdef CMCC_UI_STYLE
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_RING_DUR_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_DUR_LABLE_CTRL_ID,MMIALM_EDIT_RING_DUR_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID,MMIALM_EDIT_RING_DUR_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_VIBRATE_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_VIBRATE_LABLE_CTRL_ID,MMIALM_EDIT_VIBRATE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID,MMIALM_EDIT_VIBRATE_FORM_CTRL_ID),
#endif

    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_EDITWIN_CHILD_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON0_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON1_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON2_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON3_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON4_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON5_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON6_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),  
 #endif
                
    END_WIN
}; 

#ifdef DPHONE_SUPPORT
WINDOW_TABLE( DPHONE_ALARM_POWERONOFF_EDIT_WIN) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmPowerOnOffEditWinMsg),
    WIN_ID( DPHONE_ALARM_POWERONOFF_EDIT_WIN_ID),
    WIN_TITLE(TXT_TIME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_EDIT_TIME_CTRL(MMIALM_SETTIME_CTRL_ID), 
    END_WIN
}; 
#else
WINDOW_TABLE(POWERONOFF_EDIT_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleAlarmEditWinMsg),  
#if  !defined MMI_GUI_STYLE_TYPICAL
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR 
    WIN_STATUSBAR,
#endif
#endif
    WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    //WIN_SOFTKEY(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
#endif


    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_EDITWIN_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(STXT_SOFTKEY_SAVE_MK, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_NAME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_NAME_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_TIME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_TITLE_EDIT_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_RING_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMIALM_EDIT_RING_TYPE_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIALM_EDIT_RING_TEXT_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_MODE_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_LABEL_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_MODE_SETLIST_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),

    //CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_EDITWIN_CHILD_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON0_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON1_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON2_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON3_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON4_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON5_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON6_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),                       
    WIN_ID(POWERONOFF_EDIT_WIN_ID),
                
    END_WIN
}; 
#endif

WINDOW_TABLE( ALARM_FM_EDITWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleFMEditWinMsg), 
    WIN_ID(MMIALM_FM_EDIT_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_ALARM_FM_RING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
#ifdef MMI_PDA_SUPPORT
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_SELECT_FM_LIST_CTRL_ID),
#else
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_LIST_FORM_CTRL_ID,MMI_FM_EDITWIN_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID,MMI_FM_EDITWIN_LIST_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID,MMI_FM_EDITWIN_FORM_CTRL_ID),
			// CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMI_FM_EDITWIN_FRE_LABEL_CTRL_ID,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_FM_MAX_LEN,MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID),          
#endif
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMIALM_NAMEWIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditNameMsg),    
    WIN_ID(MMIALM_NAMEWIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_TEXT_CTRL(MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),   
    END_WIN
};
WINDOW_TABLE( MMIALM_TIMEWIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditTimeMsg ),    
    WIN_ID( MMIALM_TIMEWIN_ID ),
    WIN_TITLE( TXT_TIME ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIALM_TIME_FORM_CTRL_ID),
    CHILD_EDIT_TOUCH_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMIALM_TIME_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
        END_WIN
};

WINDOW_TABLE(MMIALM_EDITFMFRE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditFmFreMsg),    
    WIN_ID(MMIALM_EDITFMFRE_WIN_ID),
#ifndef MMI_GUI_STYLE_MINISCREEN    
    WIN_TITLE(TXT_ALARM_CUSTOM_FM),
#else
     WIN_TITLE(TXT_ALARM_CUSTOM_FM_TEXT),
#endif
    CREATE_EDIT_TEXT_CTRL(MMIALM_FM_MAX_LEN,MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),   
    END_WIN
};
#endif

#if defined(SCH_ASSISTANT_WIDGET)
WINDOW_TABLE(MMISCH_POP_ALERT_WIN_TAB)=
{
    WIN_FUNC((uint32)HandleSchPOPAlertWinMsg),
    WIN_ID(MMISCH_POP_ALERT_ID),
    WIN_TITLE(TXT_WIDGET_SCHEDULE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMISCH_POP_ALERT_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISCH_POP_ALERT_LIST_CTRL_ID, MMISCH_POP_ALERT_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMISCH_POP_ALERT_ANIM_CTRL_ID, MMISCH_POP_ALERT_FORM_CTRL_ID),
    END_WIN
};
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
WINDOW_TABLE( MMIALM_JUMP_CLOCK_TAB ) = 
{
    WIN_ID( MMIALM_ALIVEWIN_ID ),
    WIN_FUNC((uint32)HandleJumpClockEventAliveWinMsg),  
    WIN_STATUSBAR,
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(STR_POC_ILIST_OFF, TXT_NULL, TXT_ALARM_DELAY),
     END_WIN
};
#endif

//[add new alarm list UI]
WINDOW_TABLE( ALARM_LIST_MAINWIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleCustomAlarmMainWinMsg ),
    WIN_ID( MMIALM_MAINWIN_ID ),
#if defined(MMI_ALARM_MINI_SUPPORT) || (!defined(WORLD_CLOCK_SUPPORT) && !defined(STOPWATCH_SUPPORT) && !defined(COUNTEDTIME_SUPPORT))
    WIN_TITLE(TXT_ALARM_CLOCK),
#endif

    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_ADD_ALARM_EXT01, TXT_COMMON_COMMON_BACK),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIALM_MAINWIN_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE, IMAGE_NULL, MMIALM_NEW_ALARM_CTRL_ID, MMIALM_MAINWIN_FORM_CTRL_ID),
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIALM_LISTALL_CTRL_ID, MMIALM_MAINWIN_FORM_CTRL_ID),
        CHILD_TEXT_CTRL(FALSE, MMIALM_TEXT_NO_ALARMS_CTRL_ID, MMIALM_MAINWIN_FORM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( ALARM_CUSTOM_EDITWIN_TAB ) =
{
    WIN_FUNC((uint32) HandleCustomAlarmEditWinMsg),
    WIN_ID(MMIALM_EDIT_WIN_ID),

#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_ALARM_CLOCK),	
    
    WIN_SOFTKEY(STXT_OPTION, STXT_SOFTKEY_SAVE_MK, TXT_COMMON_COMMON_BACK),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_EDITWIN_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_TIME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
        CHILD_EDIT_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),

    /*CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_MODE_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_LABEL_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_MODE_SETLIST_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),*/
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E,MMIALM_EDIT_REPEAT_LIST_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_NAME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        //CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_NAME_LABEL_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    /*CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIALM_EDIT_RING_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, MMIALM_EDIT_RING_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIALM_EDIT_RING_TYPE_CTRL_ID,MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE, IMG_SCH_RECORD_ICON, MMIALM_EDIT_RING_BUTTON_CTRL_ID, 
        MMIALM_EDIT_RING_FORM_CTRL_ID),*/
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E,MMIALM_EDIT_RING_LIST_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E,MMIALM_EDIT_SNOOZE_TIME_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_EDITWIN_CHILD_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON0_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON1_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON2_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON3_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON4_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON5_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMI_EDITWIN_BUTTON6_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),

    END_WIN
};

WINDOW_TABLE( ALARM_REPEATLISTWIN_TAB ) =
{
    WIN_FUNC((uint32) HandleAlarmRepeatListWinMsg ),
    WIN_ID( MMIALM_REPEATLISTWIN_ID ),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_MARK_MK, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E,MMIALM_REPEAT_LIST_CTRL_ID),
    END_WIN
};

// set alarm fixed ring
WINDOW_TABLE( ALARM_RINGTONE_SEL_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmRingSelWinMsg ),
    WIN_ID( MMIALM_FIXEDRINGLIST_WIN_ID ),
    //WIN_TITLE(TXT_RING_FIXED),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_FIXEDRINGLIST_CTRL_ID),
    END_WIN
};

// set alarm snooze time
WINDOW_TABLE( ALARM_SNOOZETIME_SEL_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmSnoozeTimeListWinMsg ),
    WIN_ID( MMIALM_SNOOZETIMELIST_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIALM_SNOOZETIMELIST_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( ALARM_EDIT_MENU_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAlarmEditOptWin),
    WIN_ID(MMIALM_EDIT_OPT_WIN_ID),

    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_ALARM_EDIT_OPT,MMIALM_EDIT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( MMIALM_MAIN_OPTWIN_TAB ) =
{
    WIN_FUNC((uint32)HandleCustomAlarmMainOptWin ),
    WIN_ID( MMIALM_OPTWIN_ID ),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_POPMENU_CTRL(MENU_ALARM_MAIN_OPT,MMIALM_OPT_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( AUTOPOWER_CUSTOM_MAINWIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleCustomAutoPowerOnOffWinMsg ),
    WIN_ID( MMIAUTOPOWER_WIN_ID ),
#ifndef MMI_GUI_STYLE_TYPICAL
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
#endif
    //WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_AUTOPOWER_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(POWERONOFF_CUSTOM_EDIT_WIN_TAB) =
{
    WIN_FUNC((uint32) HandlePowerOnOffEditWinMsg),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    //WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),

    /*CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_EDITWIN_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_TIME_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),
            CHILD_EDIT_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMIALM_EDIT_TIME_FORM_CTRL_ID),


        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIALM_EDIT_MODE_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_LABEL_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),
            CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_MODE_SETLIST_CTRL_ID,MMIALM_EDIT_MODE_FORM_CTRL_ID),*/

    WIN_ID(POWERONOFF_EDIT_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_POWERONOFF_EDIT_CTRL_ID),
    END_WIN
};
//[end]

/**---------------------------------------------------------------------------*
 **                       FUNCTION BODY 
 **---------------------------------------------------------------------------*/
//pengliqing, 2006-07-28, about alarm, schedule, auto power on/off module Kernel Functions, start
/*****************************************************************************/
//  Description : get one event
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC const MMIACC_SMART_EVENT_FAST_TABLE_T *MMIALM_GeEvent (//�ⲿ�ļ�����s_event_fast_table�Ľӿ�
                                                              uint16 event_id
                                                              )
{

    //SCI_PASSERT(MMIACC_EVENT_MAX_NUM > event_id, ("MMIALM_GeEvent: the event_id is %d",event_id));/*assert verified*/
    if(MMIACC_EVENT_MAX_NUM <= event_id)
    {
        //SCI_TRACE_LOW:"MMIALM_GeEvent: the event_id is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1449_112_2_18_2_0_52_12,(uint8*)"d",event_id);
        return PNULL;
    }
    return &s_event_fast_table[event_id];
}

#if defined(CMCC_UI_STYLE) || defined(CALENDAR_REMINDER_TYPE_SUPPORT)
/*****************************************************************************/
// 	Description : ��ȡʵ������ʱ��
//	Global resource dependence : 
//  Author:  
//	Note:   
/*****************************************************************************/
PUBLIC MMI_TM_T MMIALM_GetSchTime(
                                            uint16 year,
                                            uint8 mon,
                                            uint8 day,
                                            uint8 hour,
                                            uint8 minute,
                                            MMISCH_REMIND_TYPE_E remind_type,
                                            BOOLEAN is_get_remind_time
                                            )
{
    uint32          temp_seconds = 0;
    uint32          res_seconds = 0;
    MMI_TM_T    tm = {0};
    
    tm.tm_year = year;
    tm.tm_mon = mon;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    
    switch(remind_type)
    {
        /*case MMISCH_REMIND_FIVE_MIN: //��ǰ5����
            temp_seconds = 5*60;
            break;*/
            
        case MMISCH_REMIND_FIFTEEN_MIN: //��ǰ15����
            temp_seconds = 15*60;
            break;
            
        case MMISCH_REMIND_ONE_HOUR: //��ǰ1Сʱ
            temp_seconds = 60*60;
            break;
            
        case MMISCH_REMIND_ONE_DAY: //��ǰһ��
            temp_seconds = 24*60*60;
            break;
            
        /*case MMISCH_REMIND_ONE_WEEK: //��ǰһ��
            temp_seconds = 7*24*60*60;
            break;*/
            
        default: //׼ʱ
            break;
    }

    if(temp_seconds > 0)
    {
        res_seconds = MMIAPICOM_Tm2Second(0, minute, hour, day, mon, year);

        if(is_get_remind_time)
        {
            //���ʵ������ʱ��
            if(res_seconds > temp_seconds)
            {
                res_seconds -= temp_seconds;
            }
        }
        else
        {
            res_seconds += temp_seconds;
        }
        
        tm = MMIAPICOM_Second2Tm(res_seconds);
    }

    return tm;
}
#endif

/*****************************************************************************/
//  Description : Open Alarm window 
//  Global resource dependence : 
//  Author:        lin.lin
//  Note:   
/*****************************************************************************/
LOCAL void Alarm_SetModifyEventDateFlag(BOOLEAN flag)
{
	g_ModifyEventDate = flag;
}

/*****************************************************************************/
//  Description : Open Alarm window 
//  Global resource dependence : 
//  Author:        lin.lin
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN Alarm_GetModifyEventDateFlag(void)
{	
	return g_ModifyEventDate;
}

/*****************************************************************************/
//  Description : �������ӷ���
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
LOCAL void SetAlarmService(
                           uint16 event_id,
                           BOOLEAN is_set_alarm
                           )
{
//     SCI_ALARM_T alm_time = {0};
    MMIALMSVC_EVENT_REG_INFO_T event_info = {0};
    
#if defined(CMCC_UI_STYLE) || defined(CALENDAR_REMINDER_TYPE_SUPPORT)
    MMI_TM_T    time = MMIALM_GetSchTime(
                                    s_event_fast_table[event_id].year, 
                                    s_event_fast_table[event_id].mon, 
                                    s_event_fast_table[event_id].day, 
                                    s_event_fast_table[event_id].hour, 
                                    s_event_fast_table[event_id].minute, 
                                    s_event_fast_table[event_id].remind_type, 
                                    TRUE);

    event_info.callback = MMIAPIALM_StartAlarm;
    event_info.event_type = ALMSVC_EVENT_SYSTEM;

    event_info.event_time.hour = time.tm_hour;
    event_info.event_time.min = time.tm_min;
    event_info.event_time.year = time.tm_year;
    event_info.event_time.mon = time.tm_mon;
    event_info.event_time.mday = time.tm_mday;
    event_info.event_time.sec = 0;
#else
    event_info.callback = MMIAPIALM_StartAlarm;
    event_info.event_type = ALMSVC_EVENT_SYSTEM;

    event_info.event_time.hour = s_event_fast_table[event_id].hour;
    event_info.event_time.min = s_event_fast_table[event_id].minute;
    event_info.event_time.year = s_event_fast_table[event_id].year;
    event_info.event_time.mon = s_event_fast_table[event_id].mon;
    event_info.event_time.mday = s_event_fast_table[event_id].day;
    event_info.event_time.sec = 0;
#endif
    if (MMIALMSVC_IsValidSvc(s_event_fast_table[event_id].service_id))
    {
        MMIALMSVC_UpdateServcice(&event_info, s_event_fast_table[event_id].service_id, s_event_fast_table[event_id].is_on, is_set_alarm); // ���·���
    }
    else
    {
        s_event_fast_table[event_id].service_id = MMIALMSVC_RegService(&event_info, s_event_fast_table[event_id].is_on, is_set_alarm); // ע�����
        //s_event_fast_table[event_id].service_id = s_event_fast_table[event_id].service_id;
    }
}


/*****************************************************************************/
//  Description : modify one event 
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN ModifyEvent(
                          uint16                    event_id, //in
                          MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr,  //in,out
                          BOOLEAN is_set_alarm
                          )
{
    BOOLEAN   result = FALSE;
    MMIALMSVC_ID_T      service_id = s_event_fast_table[event_id].service_id;
    //SCI_ASSERT(NULL != event_ptr);/*assert verified*/
    if(NULL == event_ptr)
    {
        return result;
    }
    
    if(MMIACC_EVENT_MAX_NUM <= event_id)
    {
        //SCI_TRACE_LOW:"ModifyEvent: the event_id is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1509_112_2_18_2_0_52_13,(uint8*)"d",event_id);
        return result;
    }

    if(s_event_fast_table[event_id].is_valid)
    {
        //����event_ptr�Ƿ���������(���ӡ��Զ����ػ����ճ�)��ģʽ(һ�Ρ�ÿ�졢ÿ�ܵ�)��ʱ�����ڣ�
        //����event_ptr��һ�η�����ʱ��
        SetNextAlarmTimeByCurrent(event_ptr);

        SCI_MEMCPY(&s_event_fast_table[event_id], (uint8 *)event_ptr, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

        // ���÷���
        MMIALMSVC_FreeAlmRegService(service_id, is_set_alarm);//CR136024
        SetAlarmService(event_id, is_set_alarm);

        FindLatestEventToSet();
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : modify one event 
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_ModifyEvent(
                                  uint16                    event_id, //in
                                  MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr  //in,out
                                  )
{
    return ModifyEvent(event_id, event_ptr, TRUE);
}

/*****************************************************************************/
//  Description : modify one event content name
//  Global resource dependence : 
//  Author:     xiaohua.liu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_ModifyEventName(
                                  uint16                    event_id, //in
                                  MMIACC_SMART_EVENT_CONTENT_T    *event_content_ptr  //in,out
                                  )
{
	if(PNULL == event_content_ptr)
	{
		return FALSE;
	}
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, event_content_ptr);  
	return TRUE;
}
/*****************************************************************************/
//  Description : add one event
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_AddOneEvent(
                           MMIACC_SMART_EVENT_T *event_ptr,
                           uint16               *index_ptr//out
                            )
{
    BOOLEAN result = FALSE;
    MMIALMSVC_ID_T      service_id = {0};
    uint16  i = 0;

    if (PNULL == event_ptr)
    {
        //MMI_ASSERT_WARNING("NULL != event_ptr");/*assert verified*/
        //SCI_TRACE_LOW:"MMIALM_AddOneEvent NULL == event_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1560_112_2_18_2_0_53_14,(uint8*)"");
        return FALSE;/*lint !e527 */
    }
    //SCI_TRACE_LOW:"MMIALM_AddOneEvent enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1563_112_2_18_2_0_53_15,(uint8*)"");

    for(i=EVENT_SCH_0; i<=EVENT_SCH_MAX; i++)
    {
        if(!s_event_fast_table[i].is_valid)
        {
            break;
        }
    }

    if(i <= EVENT_SCH_MAX)
    {
        result = TRUE;

        event_ptr->event_fast_info.is_valid = TRUE;

        //����event_ptr�Ƿ���������(���ӡ��Զ����ػ����ճ�)��ģʽ(һ�Ρ�ÿ�졢ÿ�ܵ�)��ʱ�����ڣ�
        //����event_ptr��һ�η�����ʱ��
        SetNextAlarmTimeByCurrent(&event_ptr->event_fast_info);

        //������
        SCI_MEMCPY((uint8 *)(&s_event_fast_table[i]), (uint8 *)&event_ptr->event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
        //�������ݱ�
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_ptr->event_content_info);  
        //����ring info
        if (ALM_DOWNLOAD_RING == event_ptr->event_content_info.ring_type
#ifdef FM_SUPPORT
            ||ALM_FM_RING == event_ptr->event_content_info.ring_type
#endif
            ||ALM_RECORD_RING == event_ptr->event_content_info.ring_type
            )
        {
            result = SaveRingInfoToFile(i,&event_ptr->event_ring_info);  
        }    
        service_id = s_event_fast_table[i].service_id;
        MMIALMSVC_FreeAlmRegService(service_id, TRUE);//CR136024
        // �������ӷ���
        SetAlarmService(i, TRUE);

        FindLatestEventToSet();
        if(NULL != index_ptr)
        {
            *index_ptr = i;
        }

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.9
        MMIVCAL_UpdatePimStatus(i-EVENT_SCH_0+1,VCAL_RECENT_ADD);
#endif /* HS_PIM_VCAL */
    }

    return result;

}

/*****************************************************************************/
//  Description : delete one event
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelOneEvent(
                           uint16 event_id
                           )
{
    BOOLEAN result = FALSE;
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
        
    if(EVENT_SCH_0 <= event_id && EVENT_SCH_MAX >= event_id && s_event_fast_table[event_id].is_valid)
    {
        // Added by sunhongzhe to remove snoozed events.
        uint16 delayed_id = 0;
        BOOLEAN has_snoozed = GetEventAndDelayedIds(&event_id, &delayed_id);
        MMIACC_SMART_EVENT_E alarm_type = s_event_fast_table[event_id].type;
        // End
        MMIALMSVC_ID_T      service_id = s_event_fast_table[event_id].service_id;
        MMIALMSVC_ID_T      delay_service_id = 0;
        //�������ϵ��¼�
        SCI_MEMSET(&s_event_fast_table[event_id], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        if (has_snoozed)
        {
            delay_service_id = s_event_fast_table[delayed_id].service_id;
            SCI_MEMSET(&s_event_fast_table[delayed_id], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        }
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

        //������ݱ��ϵ��¼�
        SCI_MEMSET(&event_content, 0, sizeof(MMIACC_SMART_EVENT_CONTENT_T));
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &event_content);
        if (has_snoozed)
        {
            MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delayed_id, &event_content);
            MMIALMSVC_UnRegService(delay_service_id, TRUE);
        }

        //ɾ�������ļ�
        DeleteRingInfoFile(event_id);
        // ɾ��һ������֮��Ҫ��������
        MMIALMSVC_UnRegService(service_id, TRUE);

        FindLatestEventToSet();

        result = TRUE;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.9
        MMIVCAL_UpdatePimStatus(event_id-EVENT_SCH_0+1,VCAL_RECENT_DELETE);
#endif /* HS_PIM_VCAL */
#ifdef CALENDAR_SUPPORT
        MMIAPISCH_DeleteUnreadEvent(event_id);
#endif

        MMIALM_RemoveIdlePushMsg(event_id, alarm_type);
    }
    else
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : delete event cnf
//  Global resource dependence : 
//  Author:        
//  Note:   
/*****************************************************************************/
PUBLIC void MMIALM_DeleteCnf(void)
{ 
    MMK_PostMsg(MMISCH_DELETE_ANIM_WIN_ID , MSG_SCH_LIST_DEL_ALL, PNULL, 0);
}

/*****************************************************************************/
//  Description : delete first valid events
//  Global resource dependence : 
//  Author:        
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelFirstValidEvent(void)
{
    BOOLEAN result = FALSE;
    uint32  i = 0;
    MmiSignalS *signal = PNULL; 

    if(MMIALM_GetAlmInterruptFlag())
    {
        //delete be interrupted, need stop delete
        return result;
    }
    for(i= EVENT_SCH_0; i<= EVENT_SCH_MAX; i++)
    {
        //find first valid event
        if(s_event_fast_table[i].is_valid)
        {
            //find it, delete it
            result = MMIALM_DelOneEvent(i);
            if(result)
            {
                //send delete cnf
                //creat signal
                MMI_CREATE_SIGNAL(signal,(uint16)MSG_SET_DEL_ALARMSET_CNF,sizeof(MmiSignalS),P_APP);
                //SCI_ASSERT(PNULL != signal);/*assert verified*/
                //send signal
                MMI_SEND_SIGNAL(signal, P_APP);
            }
            break;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : find if exist valid event
//  Global resource dependence : 
//  Author:        
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_IsExistValidEvent(void)
{
    BOOLEAN result = FALSE;
    uint32  i = 0;  

    for(i= EVENT_SCH_0; i<= EVENT_SCH_MAX; i++)
    {
        //find first valid event
        if(s_event_fast_table[i].is_valid)
        {
            //find it, delete it
            result = FALSE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : delete events
//  Global resource dependence : 
//  Author:        liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_DelEvents(
                                uint16 start_event,
                                uint16 end_event
                                )
{
    BOOLEAN result = FALSE;
    uint32  i = 0;
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
    
    //SCI_PASSERT(start_event <= end_event, ("MMIALM_DelEvents: start_event > end_event")); /*assert verified*/
    if(start_event > end_event)
    {
        //SCI_TRACE_LOW:"MMIALM_DelEvents start_event > end_event!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1742_112_2_18_2_0_53_16,(uint8*)"");
        return FALSE;
    }
    
    //������ݱ��ϵ��¼�
    SCI_MEMSET(&event_content, 0, sizeof(MMIACC_SMART_EVENT_CONTENT_T));

    if((EVENT_SCH_0 <= start_event && EVENT_SCH_MAX >= start_event) &&
        (EVENT_SCH_0 <= end_event && EVENT_SCH_MAX >= end_event) )
    {
        for(i=start_event; i<=end_event; i++)
        {
            if(s_event_fast_table[i].is_valid)
            {
                MMIALMSVC_ID_T      service_id = s_event_fast_table[i].service_id;
                SCI_MEMSET(&s_event_fast_table[i], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

                //������ݱ��ϵ��¼�
                MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content);

                //ɾ�������ļ���
                DeleteRingInfoFile(i);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.9
                MMIVCAL_UpdatePimStatus(i-EVENT_SCH_0+1,VCAL_RECENT_DELETE);
#endif /* HS_PIM_VCAL */
                // ѭ��ɾ��������Ҫÿ�ζ���������
                MMIALMSVC_UnRegService(service_id, FALSE);
            }
        }
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

        // ȫ��ɾ���˷���֮��Ҫ�������ӷ���
        MMIALMSVC_SetAlmService();
        
        FindLatestEventToSet();
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}

// Added by sunhongzhe for snoozed calendar events.
LOCAL BOOLEAN GetEventAndDelayedIds(uint16 *eventid_ptr, // in/out
    uint16 *delayid_ptr) // out
{
    BOOLEAN ret = TRUE;
    uint16 event_id = 0;
    uint16 delay_event_id = 0;

    if (eventid_ptr == PNULL) return FALSE;

    event_id = *eventid_ptr;
    if (event_id <= EVENT_ALARM_MAX)
    {
        delay_event_id = event_id+EVENT_ALARM_DELAY_0 - EVENT_ALARM_0;
    }
    else if (event_id>=EVENT_ALARM_DELAY_0 && event_id <= EVENT_ALARM_DELAY_MAX)
    {
       delay_event_id = event_id;
       event_id = event_id - EVENT_ALARM_DELAY_0 + EVENT_ALARM_0; 
    }
    else if (event_id >= EVENT_SCH_0 && event_id <= EVENT_SCH_MAX)
    {
        delay_event_id = EVENT_SCH_DELAY_0 + event_id - EVENT_SCH_0;
    }
    else if (event_id >= EVENT_SCH_DELAY_0 && event_id <= EVENT_SCH_DELAY_MAX)
    {
       delay_event_id = event_id;
       event_id = event_id - EVENT_SCH_DELAY_0 + EVENT_SCH_0; 
    }
    else
    {
        ret = FALSE;
    }

    if (ret)
    {
        *eventid_ptr = event_id;
        if (delayid_ptr != PNULL)
        {
            *delayid_ptr = delay_event_id;
        }
    }

    return ret;
}

LOCAL void CloseDelayedEventsIfPastTime(uint32 i)
{
    SCI_DATE_T              cur_date = {0};
    SCI_TIME_T              cur_time = {0};
    SCI_DATE_T              second_date = {0};
    SCI_TIME_T              second_time = {0};
    if ((EVENT_ALARM_DELAY_0 <= i && i<=EVENT_ALARM_DELAY_MAX)
        || (EVENT_SCH_DELAY_0<= i && i<= EVENT_SCH_DELAY_MAX))
    {
        TM_GetSysDate(&cur_date);
        TM_GetSysTime(&cur_time);
        second_time.hour = s_event_fast_table[i].hour;
        second_time.min = s_event_fast_table[i].minute;
        second_time.sec = 0;
        if ( (ALM_MODE_ONCE==s_event_fast_table[i].fre_mode)
            &&(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time) != CMP_FIRST_SMALL))
        {
            s_event_fast_table[i].is_on = FALSE;
        }
    }
}

LOCAL BOOLEAN OpenEventsIfNecessary(uint32 i)
{
    SCI_DATE_T              cur_date = {0};
    SCI_TIME_T              cur_time = {0};
    SCI_DATE_T              second_date = {0};
    SCI_TIME_T              second_time = {0};
    BOOLEAN needCheck = FALSE;
    uint32 delay_index = 0;
    BOOLEAN result = FALSE;

    if (i <= EVENT_ALARM_MAX)
    {
        needCheck = TRUE;
        delay_index = i+EVENT_ALARM_DELAY_0;
    }
    else if (i >= EVENT_SCH_0 && i <= EVENT_SCH_MAX)
    {
        needCheck = TRUE;
        delay_index = EVENT_SCH_DELAY_0 + (i - EVENT_SCH_0);
    }

    if (needCheck)
    {
        TM_GetSysDate(&cur_date);
        TM_GetSysTime(&cur_time);
        second_time.hour = s_event_fast_table[i].hour;
        second_time.min = s_event_fast_table[i].minute;
        second_time.sec = 0;
        second_date.year = s_event_fast_table[i].year;
        second_date.mon  = s_event_fast_table[i].mon;
        second_date.mday = s_event_fast_table[i].day;
        if ((MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
            &&(ALM_MODE_ONCE==s_event_fast_table[i].fre_mode)
            && s_event_fast_table[delay_index].is_on)
        {
            MMIALMSVC_ID_T      service_id = 0;
            s_event_fast_table[i].is_on = TRUE;
            s_event_fast_table[delay_index].is_on=FALSE;
            result = TRUE;
            service_id = s_event_fast_table[i].service_id;
            MMIALMSVC_FreeAlmRegService(service_id, FALSE);//CR136024
            SetAlarmService(i, FALSE);
        }
    }

    return result;
}
// End

/*****************************************************************************/
//  Description : update events when date and time setting changes
//  Global resource dependence : 
//  Author:      liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIALM_UpdateEvents(void)
{
    uint32      i = 0;
    BOOLEAN     result = FALSE;
    SCI_DATE_T              cur_date = {0};
    SCI_TIME_T              cur_time = {0};
    SCI_DATE_T              second_date = {0};
    SCI_TIME_T              second_time = {0};
    MMIALMSVC_ID_T      service_id = {0};

    //SCI_TRACE_LOW:"MMIALM_UpdateEvents enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1801_112_2_18_2_0_53_17,(uint8*)"");
#ifdef MMI_AZAN_SUPPORT
    MMIAPIAZAN_StopRepeatWinTimer();
#endif

    for(i=0; i<MMIACC_EVENT_MAX_NUM; i++)
    {
        if(s_event_fast_table[i].is_valid && s_event_fast_table[i].is_on)
        {
            if(SetNextAlarmTimeByCurrent(&s_event_fast_table[i]))
            {
                // Modified by sunhongzhe for snoozed calendar events.
                CloseDelayedEventsIfPastTime(i);
                // End
                /*
                if (EVENT_ALARM_DELAY_0 <= i && i<=EVENT_ALARM_DELAY_MAX)
                {
                    TM_GetSysDate(&cur_date);
                    TM_GetSysTime(&cur_time);
                    second_time.hour = s_event_fast_table[i].hour;
                    second_time.min = s_event_fast_table[i].minute;
                    second_time.sec = 0;
                    if ( (ALM_MODE_ONCE==s_event_fast_table[i].fre_mode)
                        &&(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time) != CMP_FIRST_SMALL))
                    {
                        s_event_fast_table[i].is_on = FALSE;
                    }
                }*/
                result = TRUE;
                //SCI_TRACE_LOW:"MMIALM_UpdateEvents enter! i=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_1826_112_2_18_2_0_53_18,(uint8*)"d",i);
            }
            service_id = s_event_fast_table[i].service_id;
            MMIALMSVC_FreeAlmRegService(service_id, FALSE);//CR136024
            // �������ӷ���
            SetAlarmService(i, FALSE);
        }
    else if(s_event_fast_table[i].is_valid && (!s_event_fast_table[i].is_on))
    {
        // Modified by sunhongzhe for snoozed calendar events.
        if (OpenEventsIfNecessary(i))
        {
            result = TRUE;
        }
        /*
        if (i<=EVENT_ALARM_MAX)
        {
            TM_GetSysDate(&cur_date);
            TM_GetSysTime(&cur_time);
            second_time.hour = s_event_fast_table[i].hour;
            second_time.min = s_event_fast_table[i].minute;
            second_time.sec = 0;
            second_date.year = s_event_fast_table[i].year;
            second_date.mon  = s_event_fast_table[i].mon;
            second_date.mday = s_event_fast_table[i].day;
            if ((MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
                &&(ALM_MODE_ONCE==s_event_fast_table[i].fre_mode)
                && s_event_fast_table[i+EVENT_ALARM_DELAY_0].is_on)
            {
                s_event_fast_table[i].is_on = TRUE;
                s_event_fast_table[i+EVENT_ALARM_DELAY_0].is_on=FALSE;
                result = TRUE;
                service_id = s_event_fast_table[i].service_id;
                MMIALMSVC_FreeAlmRegService(service_id, FALSE);//CR136024
                SetAlarmService(i, FALSE);
            }
        }*/
    }
    }

    if (result)//����޸�
    {
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
        // ��������
        MMIALMSVC_SetAlmService();
    }

    //
    FindLatestEventToSet();

}

/*****************************************************************************/
//  Description : init alarm
//  Global resource dependence : 
//  Author:      liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_Init(void)//�ú����ڿ���ʱ����
{
    uint16                  fast_nv_return = 0;
    uint16                  content_nv_return = 0;
    //uint16                  nv_return = 0;
    uint32                  i = 0;
    //BOOLEAN                 is_set = FALSE;
    BOOLEAN                 is_events_updated = FALSE;

    MMIACC_SMART_EVENT_CONTENT_T   event_content = {0};

    // init alarm flag
    /*MMINV_READ(MMINV_ACC_ALARM_FLAG, &is_set, nv_return);
    if (MN_RETURN_SUCCESS != nv_return)
    {
        is_set = FALSE;
        MMINV_WRITE(MMINV_ACC_ALARM_FLAG, &is_set);
    }*/

    SCI_MEMSET(s_event_fast_table,0x00,(sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T)*MMIACC_EVENT_MAX_NUM));

    MMINV_READ(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table, fast_nv_return);

    for (i=0; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        s_event_fast_table[i].service_id = 0; // ��ʼ����ʱ��id����0.���Ǹ���̬ID��ÿ�ο������᲻ͬ
        MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content, content_nv_return);

        if ((MN_RETURN_SUCCESS != fast_nv_return)||(MN_RETURN_SUCCESS != content_nv_return))
        {
            event_content.ring = /*ALM_DEFAULT_ALARM_RINGTONG*/g_clock_ring_default_id;   // default is music No.6

            //[modify alarm default ring time]
            s_event_fast_table[i].hour = /*8*/9;
            //[end]
            s_event_fast_table[i].minute = 0;
            if ( i<= EVENT_ALARM_DELAY_MAX)
            {
                MMI_STRING_T txt_default_alarm_name = {0};

                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_ALARM;
                //for cr122785,����������ÿ���Ϊ����
                s_event_fast_table[i].fre_mode = ALM_MODE_ONCE;
                //[change repeat list fre init value to 0x0]
                //s_event_fast_table[i].fre = 0x3e; // ��ʾ��һ������ѡ��
                s_event_fast_table[i].fre = 0x0;
                //[end]

                //[alarm snooze time]
                s_event_fast_table[i].snooze_type = ALM_SNOOZE_TIME_10_MIN;
                //[end]

                event_content.ring_type = ALM_FIX_RING;
#ifdef CMCC_UI_STYLE
                event_content.is_vibrate = FALSE;
#endif
                event_content.ring = /*1ALM_DEFAULT_ALARM_RINGTONG*/g_clock_ring_default_id;
                //[alarm default name]
                event_content.w_length = 0;
                /*MMI_GetLabelTextByLang(STR_DEFAULT_ALARM_NAME,&txt_default_alarm_name);
                MMI_WSTRNCPY(
                        event_content.w_content, MMIACC_EVENT_TEXT_MAX_LEN,
                        txt_default_alarm_name.wstr_ptr, txt_default_alarm_name.wstr_len,
                        txt_default_alarm_name.wstr_len);
                event_content.w_length = txt_default_alarm_name.wstr_len;*/
                //[end]
            }
            //@Lin.Lin, add for auto power on/off, start
            else if(EVENT_AUTO_POWERON == i)
            {
                s_event_fast_table[i].hour = 9;
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_AUTOPOWERON;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x3e; // ��ʾ��һ������ѡ��
            }
            else if(EVENT_AUTO_POWEROFF == i)
            {
                s_event_fast_table[i].hour = 0;
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_AUTOPOWEROFF;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x3e; // ��ʾ��һ������ѡ��
            }
            //@Lin.Lin, add for auto power on/off, end
            if ((i >= EVENT_SCH_0)&&(i <= EVENT_SCH_MAX))//cr137019
            {
                s_event_fast_table[i].type = SMART_EVENT_SCH;
            }
#ifdef MMI_AZAN_SUPPORT
            if ((i >= EVENT_AZAN_ALARM_1)&&(i <= EVENT_AZAN_ALARM_6))
            {
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_AZAN;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x7f;
            }
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
            if (i == EVENT_FM_TIMER_RECORD)
            {
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].is_on = FALSE;
                s_event_fast_table[i].type = SMART_FM_TIMER_RECORD;
                s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                s_event_fast_table[i].fre = 0x3e;
            }
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
            if (i == EVENT_SOGOU_UPDATE)
            {
                VOCABULARY_UPDATE_FREQUENCY_E freq = VOCABULARY_UPDATE_FREQUENCY_E_ONE;
                SCI_TIME_T time;
                if (MN_RETURN_SUCCESS != MMI_ReadNVItem(IMNV_SOGOU_UPDATE_FREQUENCY, &freq))
                {
                    freq = VOCABULARY_UPDATE_FREQUENCY_E_NEVER;
                }
                if (MN_RETURN_SUCCESS != MMI_ReadNVItem(IMNV_SOGOU_UPDATE_TIME, &time))
                {
                    SCI_MEMSET(&time, 0, sizeof(SCI_TIME_T));
                }
                s_event_fast_table[i].is_valid = TRUE;
                s_event_fast_table[i].is_on = TRUE;
                s_event_fast_table[i].type = SMART_EVENT_SOGOU_UPDATE;
                switch (freq)
                {
                case VOCABULARY_UPDATE_FREQUENCY_E_ONE:
                    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;
                    break;
                case VOCABULARY_UPDATE_FREQUENCY_E_SEVEN:
                    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYWEEK;
                    break;
                case VOCABULARY_UPDATE_FREQUENCY_E_FIFTEEN:
                    s_event_fast_table[i].fre_mode = ALM_MODE_EVERY15DAYS;
                    break;
                case VOCABULARY_UPDATE_FREQUENCY_E_THIRTY:
                    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYMONTH;
                    break;
                case VOCABULARY_UPDATE_FREQUENCY_E_NEVER:
                default:
                    s_event_fast_table[i].fre_mode = ALM_MODE_NO;
                    s_event_fast_table[i].is_on = FALSE;
                    break;
                }
                s_event_fast_table[i].fre = 0x7F;
                s_event_fast_table[i].hour = time.hour;
                s_event_fast_table[i].minute = time.min;
            }
#endif
            MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content);
            is_events_updated = TRUE;
        }
        else
        {
            if( s_event_fast_table[i].is_valid )
            {
//                 SCI_ALARM_T alm_time = {0};
                MMIALMSVC_EVENT_REG_INFO_T event_info = {0};
#if defined(CMCC_UI_STYLE) || defined(CALENDAR_REMINDER_TYPE_SUPPORT)
				MMI_TM_T time = {0};
#endif
                event_info.callback = MMIAPIALM_StartAlarm;
                event_info.event_type = ALMSVC_EVENT_SYSTEM;
#if defined(CMCC_UI_STYLE) || defined(CALENDAR_REMINDER_TYPE_SUPPORT)
				time = MMIALM_GetSchTime(
								s_event_fast_table[i].year, 
								s_event_fast_table[i].mon, 
								s_event_fast_table[i].day, 
								s_event_fast_table[i].hour, 
								s_event_fast_table[i].minute, 
								s_event_fast_table[i].remind_type, 
								TRUE);				
				event_info.event_time.hour = time.tm_hour;
				event_info.event_time.min = time.tm_min;
				event_info.event_time.year = time.tm_year;
				event_info.event_time.mon = time.tm_mon;
				event_info.event_time.mday = time.tm_mday;
#else
                event_info.event_time.hour = s_event_fast_table[i].hour;
                event_info.event_time.min = s_event_fast_table[i].minute;
                event_info.event_time.year = s_event_fast_table[i].year;
                event_info.event_time.mon = s_event_fast_table[i].mon;
                event_info.event_time.mday = s_event_fast_table[i].day;
#endif
                if( ALM_FIX_RING == event_content.ring_type && MMISET_MAX_RING_ID < event_content.ring )
                {
                    event_content.ring = /*ALM_DEFAULT_ALARM_RINGTONG*/g_clock_ring_default_id;
                    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content);
                    is_events_updated = TRUE;
                }
                // ע����񣬲���¼�����ID

                if (s_event_fast_table[i].is_on)
                {
                    s_event_fast_table[i].service_id = MMIALMSVC_RegService(&event_info, s_event_fast_table[i].is_on, FALSE);
                    is_events_updated = TRUE;
                }
            }
        }
    }

    if (is_events_updated)
    {
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
    }
    MMIAPICOM_StatusAreaSetAlarmState(IsAlarmSet());

#ifndef WIN32
    if(RESTART_BY_ALARM != POWER_GetRestartCondition())
    {
        // ��������
        MMIALMSVC_SetAlmService();
        FindLatestEventToSet();
        //        MMIALM_UpdateEvents();
    }
#endif
#ifdef SCHEDULE_SUPPORT
#ifdef IKEYBACKUP_SUPPORT
    MMIAPISCH_ikeybackup_register();
#endif
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : memset 0 to &s_event_fast_table[event_id],but hold s_event_fast_table[event_id].service_id
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void CleanEventFastTable(uint16 event_id)
{
    MMIALMSVC_ID_T      temp_service_id = 0;
    
    temp_service_id = s_event_fast_table[event_id].service_id;  
    SCI_MEMSET(&s_event_fast_table[event_id], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
    s_event_fast_table[event_id].service_id = temp_service_id;
}

/*****************************************************************************/
//  Description : restore factory setting of alarm
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_RestoreFactorySetting(void)//����Զ����ػ�,����/�ճ̱���
{
    //BOOLEAN     is_set = FALSE;
    uint32      i = 0;
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
    MMIALMSVC_ID_T      temp_service_id = 0;   
//    SCI_TRACE_LOW("MMIAPIALM_ClearUserData");

    //clear all NV, do in setting

    //reset alarm flag
    //MMINV_WRITE(MMINV_ACC_ALARM_FLAG, &is_set);

    //reset alarm records
    for (i = EVENT_SCH_0;i<=EVENT_SCH_MAX;i++)
    {
        temp_service_id = s_event_fast_table[i].service_id;
        SCI_MEMSET(&s_event_fast_table[i], 0, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        s_event_fast_table[i].type = SMART_EVENT_SCH;
        // ѭ��ɾ��������Ҫÿ�ζ���������
        MMIALMSVC_UnRegService(temp_service_id, FALSE);
    }
//    SCI_MEMSET(&s_event_fast_table[i], 0, (sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T) * MMIACC_EVENT_MAX_NUM));

    SCI_MEMSET(&event_content, 0, sizeof(MMIACC_SMART_EVENT_CONTENT_T));

    for(i = 0; i < (MMIACC_ALARM_NUM + MMIACC_DELAY_ALARM_NUM); i++)
    {
        CleanEventFastTable(i); 
        s_event_fast_table[i].hour = 9;
        s_event_fast_table[i].is_valid = TRUE;
        s_event_fast_table[i].type = SMART_EVENT_ALARM;
        //for cr122785,����������ÿ���Ϊ����
        s_event_fast_table[i].fre_mode = ALM_MODE_ONCE; 
        s_event_fast_table[i].fre = 0x0; // ��ʾ��һ������ѡ��
        s_event_fast_table[i].snooze_type = ALM_SNOOZE_TIME_10_MIN;
        event_content.ring_type = ALM_FIX_RING;
#ifdef CMCC_UI_STYLE
        event_content.is_vibrate = FALSE;
#endif
        event_content.ring = 1;
        event_content.w_length = 0;
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+i, &event_content);
        temp_service_id = s_event_fast_table[i].service_id;
        MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
        SetAlarmService(i, FALSE);
    }

    //reset auto power on/off
    i = EVENT_AUTO_POWERON;
    CleanEventFastTable(i);
    s_event_fast_table[i].is_valid = TRUE;
    s_event_fast_table[i].hour = 9;
    s_event_fast_table[i].type = SMART_EVENT_AUTOPOWERON;
    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;                
    s_event_fast_table[i].fre = 0x3e;    
    temp_service_id = s_event_fast_table[i].service_id;
    MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
    SetAlarmService(i, FALSE);
        
    i = EVENT_AUTO_POWEROFF;
    CleanEventFastTable(i);         
    s_event_fast_table[i].is_valid = TRUE;
    s_event_fast_table[i].hour = 0;
    s_event_fast_table[i].type = SMART_EVENT_AUTOPOWEROFF;
    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;      
    s_event_fast_table[i].fre = 0x3e;    
    temp_service_id = s_event_fast_table[i].service_id;
    MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
    SetAlarmService(i, FALSE);

#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    i = EVENT_FM_TIMER_RECORD;
    CleanEventFastTable(i);         
    s_event_fast_table[i].is_valid = TRUE;
    s_event_fast_table[i].is_on = FALSE;
    s_event_fast_table[i].hour = 8;
    s_event_fast_table[i].type = SMART_FM_TIMER_RECORD;
    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;      
    s_event_fast_table[i].fre = 0x3e;
    temp_service_id = s_event_fast_table[i].service_id;
    MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
    SetAlarmService(i, FALSE);
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    i = EVENT_SOGOU_UPDATE;
    CleanEventFastTable(i);
    s_event_fast_table[i].is_valid = TRUE;
    s_event_fast_table[i].is_on = FALSE;
    s_event_fast_table[i].hour = 0;
    s_event_fast_table[i].minute = 0;
    s_event_fast_table[i].type = SMART_EVENT_SOGOU_UPDATE;
    s_event_fast_table[i].fre_mode = ALM_MODE_EVERYDAY;      
    s_event_fast_table[i].fre = 0x7F;
    temp_service_id = s_event_fast_table[i].service_id;
    MMIALMSVC_FreeAlmRegService(temp_service_id, FALSE);//CR136024
    SetAlarmService(i, FALSE);
#endif
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table); 

    MMIALMSVC_SetAlmService();

    FindLatestEventToSet();
}

/*****************************************************************************/
//  Description : check whether there's active alarm
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN  MMIALM_HasActiveEvent ( void)
{
    return IsAlarmSet();
}

/*****************************************************************************/
//  Description : get valid event match with current date and time
//  Global resource dependence : 
//  RETURN :  if event found, return TRUE
//  Author:   taul.zhu
//  Note: ��MMI�յ�������Ϣʱ����ú͵�ʱ����ʱ����ƥ����¼���Ȼ������¼�(ɾ������
//      ������һ�η�����ʱ�䣬�޸��¼�)
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_GetArrivedEvent(
                              MMIACC_SMART_EVENT_T *event_ptr,
                              uint16               *event_id
                              )
{
    uint16                      i              = 0; 
    BOOLEAN                     result         = FALSE;
    SCI_DATE_T                  cur_date       = {0};
    SCI_TIME_T                  cur_time       = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T           event_mod      = {0};
    const MMIACC_SMART_EVENT_FAST_TABLE_T     *cur_event_ptr = NULL;

    if (PNULL == event_ptr || PNULL == event_id)
    {
        //MMI_ASSERT_WARNING("PNULL != event_ptr, NULL != event_id");/*assert verified*/
        //SCI_TRACE_LOW:"MMIALM_GetArrivedEvent PNULL == event_ptr || PNULL == event_id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2186_112_2_18_2_0_54_19,(uint8*)"");
        return FALSE;/*lint !e527*/
    }

    // if alarm after ACTIVE MODE is support !
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);

    // find first coming EVENT, just find ONE
    for (i = 0; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        cur_event_ptr = MMIALM_GeEvent(i);

        if (cur_event_ptr->is_valid &&
            cur_event_ptr->is_on &&
            IsMatchEvent(cur_date, cur_time, cur_event_ptr) )
        {
            MMIAPIALM_FormInteEventInfo(i,event_ptr);
            MMIAPIALM_GetRingInfo(i, event_ptr);
            MMI_MEMCPY(
                (void*)&event_mod, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T),
                (void*)cur_event_ptr, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T),
                sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

            //��ʱ�����ڹػ���������,����ʱ30s
            if (MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID))
            {
                SCI_ALARM_T             latest_alarm    = {0};

                latest_alarm.year = cur_event_ptr->year;
                latest_alarm.mon = cur_event_ptr->mon;
                latest_alarm.mday = cur_event_ptr->day;
                latest_alarm.hour = cur_event_ptr->hour;
                latest_alarm.min = cur_event_ptr->minute;
                latest_alarm.sec  = ALM_POWEROFF_DELAY_SEC;//���������

                if (TM_SetAlarm(latest_alarm)!= ERR_TM_NONE)
                {
                }
                //SCI_TRACE_LOW:"mmialarm.c MMIALM_GetArrivedEvent when poweroff"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2225_112_2_18_2_0_54_20,(uint8*)"");
                break;
            }

            if (SMART_EVENT_ALARM == event_mod.type ||
                SMART_EVENT_AUTOPOWERON == event_mod.type ||
                SMART_EVENT_AUTOPOWEROFF == event_mod.type
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
                ||SMART_FM_TIMER_RECORD == event_mod.type
#endif
                )
            {
#if 0 //modified by shipeng.lv at sprd for cr149822
//����һ�ε�����ҲӦ�ùر�
                //cr149955 will be resolved after cr149822 resolved.
                //shipeng.lv add at sprd. no code is modified for cr149955.
                //check cr149955 after modified code is merged.
                if (ALM_MODE_ONCE == event_mod.fre_mode)
                    &&(SMART_EVENT_AUTOPOWERON == event_mod.type ||
                    SMART_EVENT_AUTOPOWEROFF == event_mod.type)
                )
#else
                if (ALM_MODE_ONCE == event_mod.fre_mode)
#endif
                {
                    event_mod.is_on = TRUE;
                }

                MMIALM_ModifyEvent(i, &event_mod);

                if (SMART_EVENT_AUTOPOWERON == event_mod.type)
                {
                    PHONE_STARTUP_CONDITION_E   e_startup_cond = STARTUP_NONE;

                    e_startup_cond = MMIAPIPHONE_GetStartUpCondition();

                    if(e_startup_cond == STARTUP_NORMAL)
                    {
                        continue;//������������Զ�����,��������
                    }
                }
            }
            else
            {
                if(ALM_MODE_ONCE == event_mod.fre_mode)
                {
                    /*event_mod.is_valid = FALSE;
                    event_mod.is_on = FALSE;
                    MMIALM_DelOneEvent(i);*/
                    SCI_MEMCPY((uint8 *)(&s_event_fast_table[i]), (uint8 *)&event_mod, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
                    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
                }
                else
                {
                    MMIALM_ModifyEvent(i, &event_mod);
                }
            }

            *event_id = i;
            result = TRUE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : find if the alarm is set
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAlarmSet(void)
{
    BOOLEAN                     is_alarm        = FALSE;
    uint32                      i               = 0;    
    const MMIACC_SMART_EVENT_FAST_TABLE_T     *cur_event_ptr  = NULL;
    const MMIACC_SMART_EVENT_FAST_TABLE_T     *cur_event_delay_ptr  = NULL;
    
    for (i = 0; i< MMIACC_ALARM_NUM; i++)
    {
        cur_event_ptr = MMIALM_GeEvent(i);
        cur_event_delay_ptr = GetDelayEvent(i);
    
        if ((SMART_EVENT_ALARM == cur_event_ptr->type && cur_event_ptr->is_on && cur_event_ptr->is_valid)
        ||(SMART_EVENT_ALARM == cur_event_delay_ptr->type && cur_event_delay_ptr->is_on && cur_event_delay_ptr->is_valid)
            )
        {
            is_alarm = TRUE;
            break;
        }
    }

    return (is_alarm);
}

/*****************************************************************************/
//  Description : find latest active smart event, and set to alarm according to 
//                current SYS time and date
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FindLatestEventToSet(void)
{
    BOOLEAN                 result          = TRUE;
    uint32                  i               = 0;    
    //uint32                  event_id        = 0;    
    //BOOLEAN                   is_set          = FALSE;
    SCI_DATE_T              cur_date        = {0};
    SCI_TIME_T              cur_time        = {0};
//     SCI_ALARM_T             latest_alarm    = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T       event           = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T       latest_event    = {0}; 
    MMIACC_SMART_EVENT_FAST_TABLE_T       cur_point       = {0};
    BOOLEAN                 is_alarm        = FALSE;

//    SCI_TRACE_LOW("mmialarm.c:FindLatestEventToSet() enter");

    SCI_MEMSET( &event, 0, sizeof( MMIACC_SMART_EVENT_FAST_TABLE_T));   
    SCI_MEMSET( &latest_event, 0, sizeof( MMIACC_SMART_EVENT_FAST_TABLE_T));   
    
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);
    cur_point.year = cur_date.year;    //  initial value to first compare
    cur_point.mon = cur_date.mon;
    cur_point.day = cur_date.mday;
    cur_point.hour = cur_time.hour;
    cur_point.minute = cur_time.min;
    latest_event.year = 0xFFFF;

    for(i = 0; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        event = s_event_fast_table[i];

        if (event.is_valid && event.is_on)
        {
            //�õ���ǰʱ��֮�����������ʱ��
            if (MMIAPIALM_CompareTwoEvent(&event, &cur_point) == CMP_SECOND_SMALL &&
                MMIAPIALM_CompareTwoEvent(&latest_event, &event) == CMP_SECOND_SMALL)
            {
                //event_id = i;
                latest_event = event;
            }
        }
    }  

    if(0xffff == latest_event.year)    // not find valid event 
    {
        // TM_DisableAlarm(); 
        //is_set = FALSE;
        result = FALSE;
        // �������ӷ���
        //MMIALMSVC_SetAlmService();
    }
    else
    {
//         SCI_ALARM_T alm_time = {0};
//         MMIALMSVC_EVENT_REG_INFO_T event_info = {0};

//         latest_alarm.year = latest_event.year;
//         latest_alarm.mon = latest_event.mon;
//         latest_alarm.mday = latest_event.day;
//         latest_alarm.hour = latest_event.hour;
//         latest_alarm.min = latest_event.minute;
//         latest_alarm.sec  = 0;
        
//         event_info.callback = MMIAPIALM_StartAlarm;
//         event_info.event_type = ALMSVC_EVENT_SYSTEM;
//         
//         event_info.event_time.hour = latest_event.hour;
//         event_info.event_time.min = latest_event.minute;
//         event_info.event_time.year = latest_event.year;
//         event_info.event_time.mon = latest_event.mon;
//         event_info.event_time.mday = latest_event.day;
//         event_info.event_time.sec = 0;

//         if (MMIALMSVC_IsValidSvc(latest_event.service_id))
//         {
//             MMIALMSVC_UpdateServcice(&event_info, latest_event.service_id, TRUE); // ���·���
//         }
//         else
//         {
//             latest_event.service_id = MMIALMSVC_RegService(&event_info, latest_event.is_on); // ע�����
//             s_event_fast_table[event_id].service_id = latest_event.service_id;
//         }
        
//         if (TM_SetAlarm(latest_alarm)!= ERR_TM_NONE)
//         {
//         }
//        is_set = TRUE;
    }

    //MMINV_WRITE(MMINV_ACC_ALARM_FLAG, &is_set);
    is_alarm = IsAlarmSet();
    MMIAPICOM_StatusAreaSetAlarmState(is_alarm);

    return result;
}

/*****************************************************************************/
//  Description : SetNextAlarmTimeByCurrent
//  Global resource dependence : 
//  Author:  liqing.peng
//  Note:  ����event_ptr�Ƿ���������(���ӡ��Զ����ػ����ճ�)��
//         ģʽ(һ�Ρ�ÿ�졢ÿ�ܵ�)��ʱ�����ڣ�
//         ����event_ptr��һ�η�����ʱ��
/*****************************************************************************/
LOCAL BOOLEAN SetNextAlarmTimeByCurrent(
                                        MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr         //in & out
                                        )
{
    SCI_ALARM_T             first = {0};
    SCI_ALARM_T             second = {0};
    BOOLEAN                 result = FALSE;
    SCI_DATE_T              second_date = {0};
    SCI_TIME_T              second_time = {0};
    SCI_DATE_T              cur_date = {0};
    SCI_TIME_T              cur_time = {0};
    uint32                  offset_day = 0;
    uint32                  i = 0;
    uint16                  year = 0;
    uint8                   month = 0;
    uint8                   day = 0;
    SCI_DATE_T              event_start_date = {0};
    SCI_DATE_T              event_end_date = {0};

    //if (!(event_ptr->is_valid && event_ptr->is_on))
    // Modified by sunhongzhe for Calendar MS Custom UI.
    if (!(event_ptr->is_valid && event_ptr->is_on && event_ptr->remind_type != MMISCH_REMIND_NONE))
    // End
    {
        return FALSE;
    }
#ifdef CALENDAR_SUPPORT
    if(SMART_EVENT_SCH == event_ptr->type)
    {
        if(!MMICALENDAR_IsDateValid(event_ptr->year,
            event_ptr->mon,
            event_ptr->day))
        {
            return FALSE;
        }
    }
#endif
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);

    second_time.hour = event_ptr->hour;
    second_time.min = event_ptr->minute;
    second_time.sec = 0;

    switch(event_ptr->type)
    {
    case SMART_EVENT_ALARM:
    case SMART_EVENT_AUTOPOWERON:
    case SMART_EVENT_AUTOPOWEROFF:
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    case SMART_FM_TIMER_RECORD:
#endif
        switch(event_ptr->fre_mode)
        {
        case ALM_MODE_ONCE:
        case ALM_MODE_EVERYDAY:
            if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time)  == CMP_FIRST_SMALL)
            {
                event_ptr->year   = cur_date.year;
                event_ptr->mon    = cur_date.mon;
                event_ptr->day    = cur_date.mday;
                result = TRUE;
            }
            else
            {
                if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                {
                    //                    SCI_TRACE_LOW("SetNextAlarmTimeByCurrent() 2099!");
                    result = FALSE;
                }
                else
                {
					if ((event_ptr->fre_mode == ALM_MODE_ONCE)
						&& (!Alarm_GetModifyEventDateFlag()))
					{
						SCI_TraceLow("\nDon't modify event date for once mode!!!!!! [%s, %d]\n",__FUNCTION__,__LINE__);
					}
                   	else
                    {
                        IncreaseDateByOne(&cur_date);
                        event_ptr->year   = cur_date.year;
                        event_ptr->mon    = cur_date.mon;
                        event_ptr->day    = cur_date.mday;
                    }
                    result = TRUE;
               }
            }
            break;

        case ALM_MODE_EVERYWEEK:
            //SCI_PASSERT(0 != (0x7f & event_ptr->fre), ("SetNextAlarmTimeByCurrent: event_ptr->fre is %d",event_ptr->fre));/*assert verified*/

            if(0 == (0x7f & event_ptr->fre))
            {
                //SCI_TRACE_LOW:"SetNextAlarmTimeByCurrent: event_ptr->fre is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2512_112_2_18_2_0_55_21,(uint8*)"d",event_ptr->fre);
                break;
            }
            if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time)  == CMP_FIRST_SMALL)
            {
                offset_day = RoundWeek(cur_date.wday, event_ptr->fre);
            }
            else
            {
                offset_day = RoundWeek2(cur_date.wday, event_ptr->fre);
            }

            for (i=0; i <offset_day; i++)
            {
                if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                {
                    break;
                }
                IncreaseDateByOne(&cur_date);
            }
            if (i == offset_day)
            {
                event_ptr->year   = cur_date.year;
                event_ptr->mon    = cur_date.mon;
                event_ptr->day    = cur_date.mday;
                result = TRUE;
            }
            else
            {
                //                SCI_TRACE_LOW("SetNextAlarmTimeByCurrent() 2099!");
                result = FALSE;
            }
            break;

        default:
            break;
        }
        break;

    case SMART_EVENT_SCH:
#ifdef MMI_AZAN_SUPPORT
    case SMART_EVENT_AZAN:
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    case SMART_EVENT_SOGOU_UPDATE:
#endif
        second_date.year = event_ptr->year;
        second_date.mon  = event_ptr->mon;
        second_date.mday = event_ptr->day;
        event_start_date.year = event_ptr->start_year;
        event_start_date.mon = event_ptr->start_mon;
        event_start_date.mday = event_ptr->start_day;

        event_end_date.year = event_ptr->end_year;
        event_end_date.mon = event_ptr->end_mon;
        event_end_date.mday = event_ptr->end_day;
        if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, event_start_date, second_time) == CMP_FIRST_SMALL)//cur_date < event_start_date�����
        {
            event_ptr->year   = event_start_date.year;
            event_ptr->mon    = event_start_date.mon;
            event_ptr->day    = event_start_date.mday;
            result = TRUE;
        }
        else if ( (SMART_EVENT_SCH==event_ptr->type) && (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, event_end_date, second_time) != CMP_FIRST_SMALL))
        {
            result = TRUE; // �Ѿ�������
        }
        else//event_start_date < cur_date < event_date�����,��event_date < cur_date �����
        {
            //event date is expired
            switch(event_ptr->fre_mode)
            {
            case ALM_MODE_ONCE:
                if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time)  == CMP_FIRST_SMALL)
                {
                    result = TRUE;
                }
                else
                {
                    if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                    {
                        result = FALSE;
                    }
                    else
                    {
                        result = TRUE;
                    }
                }
                break;

            case ALM_MODE_EVERYDAY:
                if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time)  == CMP_FIRST_SMALL)
                {
                    event_ptr->year   = cur_date.year;
                    event_ptr->mon    = cur_date.mon;
                    event_ptr->day    = cur_date.mday;
                    result = TRUE;
                }
                else
                {
                    if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                    {
                        result = FALSE;
                    }
                    else
                    {
                        IncreaseDateByOne(&cur_date);
                        event_ptr->year   = cur_date.year;
                        event_ptr->mon    = cur_date.mon;
                        event_ptr->day    = cur_date.mday;
                        result = TRUE;
                    }
                }
                break;

            case ALM_MODE_EVERYWEEK:
                if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, cur_date, second_time)  == CMP_FIRST_SMALL)
                {
                    offset_day = RoundWeek(cur_date.wday, event_ptr->fre);
                }
                else
                {
                    offset_day = RoundWeek2(cur_date.wday, event_ptr->fre);
                }

                for (i=0; i <offset_day; i++)
                {
                    if ((QUERY_YEAR_TOP == cur_date.year) && (ALM_DECEMBER == cur_date.mon) && (BIG_MONTH_DAY == cur_date.mday))
                    {
                        break;
                    }
                    IncreaseDateByOne(&cur_date);
                }
                if (i == offset_day)
                {
                    event_ptr->year   = cur_date.year;
                    event_ptr->mon    = cur_date.mon;
                    event_ptr->day    = cur_date.mday;
                    result = TRUE;
                }
                else
                {
                    //                    SCI_TRACE_LOW("SetNextAlarmTimeByCurrent() 2099!");
                    result = FALSE;
                }
                break;

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
            case ALM_MODE_EVERY15DAYS:
                second_date.year = cur_date.year;
                second_date.mon  = cur_date.mon;
                second_date.mday = event_ptr->day;
#ifdef CALENDAR_SUPPORT
                if((event_ptr->day <= (MMICALENDAR_GetMonthday(cur_date.year, cur_date.mon)-15) &&
                   (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)))
#else
                if(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
#endif
                {
                    event_ptr->year   = cur_date.year;
                    event_ptr->mon    = cur_date.mon;
                }
                else
                {
                    year = cur_date.year;
                    month = cur_date.mon;
                    day = event_ptr->day;
#ifdef CALENDAR_SUPPORT
                    MMICALENDAR_MoveDaysForward(year, month, day, 15, &year, &month, &day);
#endif
                    event_ptr->year = year;
                    event_ptr->mon = month;
                    event_ptr->day = day;
                }
                result = TRUE;
                break;
#endif
            case ALM_MODE_EVERYMONTH:
                //check current month has the day,  and time in this month hasn't expire
                second_date.year = cur_date.year;
                second_date.mon  = cur_date.mon;
                second_date.mday = event_ptr->day;
#ifdef CALENDAR_SUPPORT
                if((event_ptr->day <= MMICALENDAR_GetMonthday(cur_date.year, cur_date.mon) &&
                    (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time)  == CMP_FIRST_SMALL)))
#else
                if(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time)  == CMP_FIRST_SMALL)
#endif
                {
                    //start from this month
                    event_ptr->year   = cur_date.year;
                    event_ptr->mon    = cur_date.mon;
                }
                else
                {
                    //start from next month
                    year = cur_date.year;
                    month = cur_date.mon;
                    day = event_ptr->day;
#ifdef CALENDAR_SUPPORT
                    MMICALENDAR_MoveDaysForward(year, month, day,
                        MMICALENDAR_GetMonthday(year, month),
                        &year, &month, &day);
#endif
                    if(day == event_ptr->day)
                    {
                        //next month has the day
                        event_ptr->year = year;
                        event_ptr->mon = month;
                    }
                    else
                    {
                        //next month hasn't the day, move next month again
                        event_ptr->year = year;
                        event_ptr->mon = month+1;
                    }
                }

                result = TRUE;
                break;

            case ALM_MODE_EVERYYEAR:
                //check current year has the day, and time in this year hasn't expire
                second_date.year = cur_date.year;
                second_date.mon  = event_ptr->mon;
                second_date.mday = event_ptr->day;

                if((2 != event_ptr->mon || 29 > event_ptr->day || MMIAPICOM_IsLeapYear(cur_date.year)) &&
                    (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time)  == CMP_FIRST_SMALL))
                {
                    //start from this year
                    event_ptr->year = cur_date.year;
                }
                else
                {
                    //start from next year
                    if(2 == event_ptr->mon && 29 == event_ptr->day)
                    {
                        //leap year
                        event_ptr->year = (cur_date.year+4)/4*4;
                        if(!MMIAPICOM_IsLeapYear(event_ptr->year))
//                             {
//                                 event_ptr->year = event_ptr->year;
//                             }
//                             else
                        {
                            event_ptr->year = event_ptr->year+4;
                        }
                    }
                    else
                    {
                        //normal year
                        event_ptr->year = cur_date.year+1;
                    }
                }

                result = TRUE;
                break;

            default:
                break;
        }
    }

        // �ж��Ƿ����
        first.year  = event_ptr->year;
        first.mon  = event_ptr->mon;
        first.mday = event_ptr->day;
        first.hour = 0;
        first.min = 0;
        first.sec = 0;          // ignore second when compare to event

        second.year = event_ptr->end_year;
        second.mon = event_ptr->end_mon;
        second.mday = event_ptr->end_day;
        second.hour = 0;
        second.min  = 0;
        second.sec = 0;        // ignore second

        if(CompareDateTime(first, second) == CMP_SECOND_SMALL)
        {
            event_ptr->year = cur_date.year;
            event_ptr->mon = cur_date.mon;
            event_ptr->day = cur_date.mday;
        }
        break;

    default:
        break;
    }

    if(result)
    {
        //SCI_TRACE_LOW:"SetNextAlarmTimeByCurrent(), success, year = %d, month = %d, day = %d, hour = %d, minute = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2795_112_2_18_2_0_55_22,(uint8*)"ddddd", event_ptr->year, event_ptr->mon, event_ptr->day, event_ptr->hour, event_ptr->minute);
    }
    else
    {
        //SCI_TRACE_LOW:"SetNextAlarmTimeByCurrent(), fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_2799_112_2_18_2_0_55_23,(uint8*)"");
    }

    return result;
}
//pengliqing, 2006-07-28, about alarm, schedule, auto power on/off module Kernel Functions, end

/*****************************************************************************/
//  Description : SetNextAlarmTimeByCurrent
//  Global resource dependence : 
//  Author:  liqing.peng
//  Note:  ����event_ptr�Ƿ���������(���ӡ��Զ����ػ����ճ�)��
//         ģʽ(һ�Ρ�ÿ�졢ÿ�ܵ�)��ʱ�����ڣ�
//         ����event_ptr��һ�η�����ʱ��
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_SetNextAlarmTimeByCurrent(
                                        MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr         //in & out
                                        )
{
    return SetNextAlarmTimeByCurrent(event_ptr);
}

/*****************************************************************************/
//  Description : open alarm main window
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenMainWin(void)
{
    MMK_CloseMiddleWin(MMIALM_START_WIN_ID, MMIALM_END_WIN_ID);
    //[add new alarm list UI]
    //MMK_CreateWin((uint32 *)ALARM_MAINWIN_TAB, PNULL);
    MMK_CreateWin((uint32 *)ALARM_LIST_MAINWIN_TAB, PNULL);
    //[end]
}

/*****************************************************************************/
//  Description : open auto power on/off main window
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUTOPOWER_OpenMainWin(void)
{
    //[power on off function]
    MMK_CreateWin((uint32 *)AUTOPOWER_MAINWIN_TAB,PNULL);
//     MMK_CreateWin((uint32 *)AUTOPOWER_CUSTOM_MAINWIN_TAB,PNULL);
    //[end]
}

/*****************************************************************************/
//  Description : ��wday����ҵ���һ��ƥ��ģ�����offset
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:   
/*****************************************************************************/
LOCAL uint8 RoundWeek(
                      uint8 wday,
                      uint8 fre
                      )
{
    uint8   offset = 0;
    uint32  i = 0;

    for (i = wday; i < ALM_MODE_WEEK_NUM; i++)
    {
        if (fre & s_week_mask[i])
        {
            break;
        }
        offset++;
    }
    if (i < ALM_MODE_WEEK_NUM)
    {
        return offset;
    }
    for (i = 0; i < wday; i++)
    {
        if (fre & s_week_mask[i])
        {
            break;
        }
        offset++;
    }
    return offset;
}

/*****************************************************************************/
//  Description : ��wday��һ������ҵ���һ��ƥ��ģ�����offset
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:   
/*****************************************************************************/
LOCAL uint8 RoundWeek2(
                      uint8 wday,
                      uint8 fre
                      )
{
    if (6 != wday)
    {
        return (RoundWeek((uint8)(wday+1), fre)+1);
    }
    else
    {
        return (RoundWeek(0, fre)+1);
    }
}

/*****************************************************************************/
//  Description : CompareDateTime
//         compare date and time to another group, find smaller(earlier)
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL int16 CompareDateTime(
                            SCI_ALARM_T first, 
                            SCI_ALARM_T second
                            )
{
    if(first.year > second.year)
    {
        return CMP_SECOND_SMALL;
    }
    else if(first.year < second.year)
    {
        return CMP_FIRST_SMALL;
    }
    else
    {
        if(first.mon > second.mon)
        {
            return CMP_SECOND_SMALL;
        }
        else if(first.mon < second.mon)
        {
            return CMP_FIRST_SMALL;
        }
        else
        {
            if(first.mday > second.mday)
            {
                return CMP_SECOND_SMALL;
            }
            else if(first.mday < second.mday)
            {
                return CMP_FIRST_SMALL;
            }
            else
            {
                if(first.hour > second.hour)
                {
                    return CMP_SECOND_SMALL;
                }
                else if(first.hour < second.hour)
                {
                    return CMP_FIRST_SMALL;
                }
                else
                {
                    if(first.min >second.min)
                    {
                        return CMP_SECOND_SMALL;
                    }
                    else if(first.min <second.min)
                    {
                        return CMP_FIRST_SMALL;
                    }
                    else
                    {
                        return CMP_EQUAL;
                    }
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : compare two event, find smaller with date and time
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note: �Ƚϵ�����event��Ҫ����������date��time
/*****************************************************************************/
PUBLIC int16 MMIAPIALM_CompareTwoEvent(
                const MMIACC_SMART_EVENT_FAST_TABLE_T *latest_event_ptr,
                const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr 
                ) 
{
   SCI_ALARM_T first, second;

    first.year  = latest_event_ptr->year;
    first.mon  = latest_event_ptr->mon;
    first.mday = latest_event_ptr->day;
    first.hour = latest_event_ptr->hour;
    first.min = latest_event_ptr->minute;
    first.sec = 0;          // ignore second when compare to event

    second.year = event_ptr->year;
    second.mon = event_ptr->mon;
    second.mday = event_ptr->day;
    second.hour = event_ptr->hour;
    second.min  = event_ptr->minute;
    second.sec = 0;        // ignore second 
    
    return CompareDateTime(first, second);
}

// Added by sunhongzhe to compare events by real notification time.
LOCAL BOOLEAN MMIAPIALM_IsSameRealNotifyTime(
                const MMIACC_SMART_EVENT_FAST_TABLE_T *latest_event_ptr,
                const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr 
                ) 
{
    BOOLEAN     result = FALSE;
    MMI_TM_T    time  = {0};
    MMI_TM_T    time2  = {0};

    if (PNULL == latest_event_ptr || PNULL == event_ptr)
    {
        return FALSE;
    }

    time.tm_year = latest_event_ptr->year;
    time.tm_mon = latest_event_ptr->mon;
    time.tm_mday = latest_event_ptr->day;
    time.tm_hour = latest_event_ptr->hour;
    time.tm_min = latest_event_ptr->minute;

    time2.tm_year = event_ptr->year;
    time2.tm_mon = event_ptr->mon;
    time2.tm_mday = event_ptr->day;
    time2.tm_hour = event_ptr->hour;
    time2.tm_min = event_ptr->minute;

#if defined(CMCC_UI_STYLE) || defined(CALENDAR_REMINDER_TYPE_SUPPORT)
    time = MMIALM_GetSchTime(
                                    latest_event_ptr->year, 
                                    latest_event_ptr->mon, 
                                    latest_event_ptr->day, 
                                    latest_event_ptr->hour, 
                                    latest_event_ptr->minute, 
                                    latest_event_ptr->remind_type, 
                                    TRUE);
    time2 = MMIALM_GetSchTime(
                                    event_ptr->year, 
                                    event_ptr->mon, 
                                    event_ptr->day, 
                                    event_ptr->hour, 
                                    event_ptr->minute, 
                                    event_ptr->remind_type, 
                                    TRUE);
#endif

    if(time.tm_year == time2.tm_year &&
           time.tm_mon == time2.tm_mon &&
           time.tm_mday == time2.tm_mday &&
           time.tm_hour == time2.tm_hour &&
           time.tm_min == time2.tm_min)
    {
        result = TRUE;
    }

    return result;
}
// End.


/*****************************************************************************/
//  Description : �Ƚ�event���Ƿ�͵�ǰ����ʱ��ƥ��
//  Global resource dependence :  
//  Author: bin.ji
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsMatchEvent(
                           SCI_DATE_T                   cur_date,
                           SCI_TIME_T                   cur_time,
                           const MMIACC_SMART_EVENT_FAST_TABLE_T   *event_ptr
                           )
{
    BOOLEAN     result = FALSE;
    MMI_TM_T    time  = {0};
    if (PNULL == event_ptr)
    {
        //MMI_ASSERT_WARNING("NULL != event_ptr");/*assert verified*/
        //SCI_TRACE_LOW:"IsMatchEvent:NULL == event_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_3002_112_2_18_2_0_56_24,(uint8*)"");
        return FALSE;/*lint !e527*/
    }
    time.tm_year = event_ptr->year;
    time.tm_mon = event_ptr->mon;
    time.tm_mday = event_ptr->day;
    time.tm_hour = event_ptr->hour;
    time.tm_min = event_ptr->minute;
#if defined(CMCC_UI_STYLE) || defined(CALENDAR_REMINDER_TYPE_SUPPORT)
    time = MMIALM_GetSchTime(
                                    event_ptr->year, 
                                    event_ptr->mon, 
                                    event_ptr->day, 
                                    event_ptr->hour, 
                                    event_ptr->minute, 
                                    event_ptr->remind_type, 
                                    TRUE);
#endif

    if(time.tm_year == cur_date.year &&
       time.tm_mon == cur_date.mon &&
       time.tm_mday == cur_date.mday &&
       time.tm_hour == cur_time.hour &&
       time.tm_min == cur_time.min)
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : �Ƚ���������ʱ��
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
PUBLIC int16 MMIAPIALM_CompareDateAndTime(
                                SCI_DATE_T first_date, 
                                SCI_TIME_T first_time, 
                                SCI_DATE_T second_date, 
                                SCI_TIME_T second_time 
                                )
{
    SCI_ALARM_T     first = {0};
    SCI_ALARM_T     second = {0};

    first.year  = first_date.year;
    first.mon  = first_date.mon;
    first.mday = first_date.mday;
    first.hour = first_time.hour;
    first.min = first_time.min;
    first.sec = 0;          // ignore second when compare to event

    second.year = second_date.year;
    second.mon = second_date.mon;
    second.mday = second_date.mday;
    second.hour = second_time.hour;
    second.min  = second_time.min;
    second.sec = 0;        // ignore second 
    
    return CompareDateTime(first, second);
}

/*****************************************************************************/
//  Description : get the events witch will happen at the same time
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetSameTimeSchEvent(
                                 const MMIACC_SMART_EVENT_FAST_TABLE_T *latest_event, // IN: �Ƚϵ�event
                                 uint16 event_id,                    // IN: event��id
                                 uint16 start_id,                    // IN: start id
                                 MMIACC_SCH_LIST_T *p_sch_list       // OUT:
                                 )
{
    uint16                  i                   = 0; 
    uint16                  num                 = 0;
    MMIACC_SMART_EVENT_FAST_TABLE_T    event                = {0};

    //SCI_PASSERT(event_id < MMIACC_EVENT_MAX_NUM && start_id < MMIACC_EVENT_MAX_NUM, ("GetSameTimeSchEvent: event_id is %d, start_id is %d",event_id,start_id));  /*assert verified*/    
    if(event_id >= MMIACC_EVENT_MAX_NUM || start_id >= MMIACC_EVENT_MAX_NUM)
    {
        return num;
    }
    
    //SCI_ASSERT(NULL != latest_event);/*assert verified*/
    //SCI_ASSERT(NULL != p_sch_list); /*assert verified*/
    if(NULL == latest_event||NULL == p_sch_list)
    {
        return num;
    }

    SCI_MEMSET((uint8 *)p_sch_list, 0, sizeof(MMIACC_SCH_LIST_T));

    for(i = start_id; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        SCI_MEMCPY((uint8 *)&event, (uint8 *)&s_event_fast_table[i], sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

        if (event.is_valid && event.is_on )//&& i != event_id) /////ZDT_PRODUCT_JMJ   ///ZDT_PRODUCT_Z685S
        {
            //if (MMIAPIALM_CompareTwoEvent(latest_event, &event) == CMP_EQUAL)
            if (MMIAPIALM_IsSameRealNotifyTime(latest_event, &event))
            {
                num++;
                p_sch_list->entry_id[p_sch_list->total] = i;
                p_sch_list->total++;
            }
        }
    }

    return num;
}

/*****************************************************************************/
//  Description : get the events witch will happen at the same time
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneSchEventInList(
                                     uint16 index, // IN
                                     MMIACC_SCH_LIST_T *p_sch_list       // IN OUT:
                                     )
{
    uint16                  i                   = 0; 

    //SCI_ASSERT(NULL != p_sch_list); /*assert verified*/
    if (PNULL == p_sch_list)
    {
        return  FALSE;
    }
    
    if(index>=p_sch_list->total)
    {
        return FALSE;
    }

    if(index == p_sch_list->total - 1)
    {
        p_sch_list->entry_id[index] = 0;
        p_sch_list->total--;
    }
    else
    {
        for(i = index; i < p_sch_list->total - 1; i++)
        {
            p_sch_list->entry_id[i] = p_sch_list->entry_id[i + 1];
        }
        p_sch_list->total--;
        p_sch_list->entry_id[p_sch_list->total] = 0;
    }
    
    return TRUE;
}

/*****************************************************************************/
// Description : IncreaseTimeBySecond
// increase current time by seconds inputed.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:   
/*****************************************************************************/
LOCAL  void IncreaseTimeBySecond(
                                 SCI_DATE_T *pdate,  // inout
                                 SCI_TIME_T *ptime,  // inout
                                 uint16      sec            // in
                                 )
{
    uint16 i = 0;

    i = ptime->sec + sec;
    if (i < 60)
    {
        ptime->sec = i;
    }
    else
    {
        // ptime->sec = i - 60;
        ptime->sec = i%60;
        ptime->min += i/60;

        ptime->hour += ptime->min/60;
        ptime->min = ptime->min%60;

        for (i = 0; i < ptime->hour/24; i++)
        {
            IncreaseDateByOne(pdate);
        }
        ptime->hour = ptime->hour%24;
    }
}

/*****************************************************************************/
//  Description : increase current date by one
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL  void IncreaseDateByOne(
                              SCI_DATE_T  *pdate_ptr
                              )
{
    if (pdate_ptr->mon<1 ||pdate_ptr->mon>12)
    {
        //SCI_TRACE_LOW:"IncreaseDateByOne error! mon=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_3181_112_2_18_2_0_56_25,(uint8*)"d",pdate_ptr->mon);
        return;
    }
    if(MMIAPICOM_IsLeapYear((pdate_ptr->year)))
    {
        if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon = 1;
            pdate_ptr->year++; 
        }
        else if(pdate_ptr->mday == s_RTC_month_info_leap[pdate_ptr->mon-1])
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon++;
        }
        else
        {
            pdate_ptr->mday++;
        }
    }
    else        // not leap year
    {
        if((pdate_ptr->mon == 12)&&(pdate_ptr->mday == 31))
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon = 1;
            pdate_ptr->year++;
        }
        else if(pdate_ptr->mday == s_RTC_month_info[pdate_ptr->mon-1])
        {
            pdate_ptr->mday = 1;
            pdate_ptr->mon++;
        }
        else
        {
            pdate_ptr->mday++;
        }
    }

    if (pdate_ptr->year >MMICOM_DATE_MAX_YEAR)
    {
        pdate_ptr->year = MMICOM_DATE_MIN_YEAR;       // when 2099, return 1980
    }
}

#if !defined MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : start timer
//	Global resource dependence : 
//  Author:       maryxiao
//	Note: 
/*****************************************************************************/
// LOCAL uint8 CreateClockTimer(MMI_WIN_ID_T  win_id, ALARM_CLOCK_TYPE_E alarm_type)
// {
//     uint16 time_out= 0;
// 
//     switch(alarm_type)
//     {
//     case ALM_CLOCK_TYPE_1:
//         time_out = CLOCK_REFRESH_SEC_TIME;
//         break;
//     case ALM_CLOCK_TYPE_2:
//          time_out = CLOCK_REFRESH_MIN_TIME;
//         break;
//     default:
//         time_out = CLOCK_REFRESH_SEC_TIME;
//         break;
//     }
//     return MMK_CreateWinTimer(win_id, time_out, TRUE);
// }

/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: maryxiao
//  Note:
/*****************************************************************************/
// LOCAL void StopClockTimer(void)
// {
//     if (0 != s_refresh_timer_id )
//     {
//         MMK_StopTimer(s_refresh_timer_id );
//         s_refresh_timer_id  = 0;
//     }
//     else
//     {
//         //SCI_TRACE_LOW:"[StopClockTimer]: StopTimer: the timer has stopped!"
//         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_3264_112_2_18_2_0_56_26,(uint8*)"");
//     }
// }
#endif
/*****************************************************************************/
//  Description : the process message function of the ALARM
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmMainWinMsg( 
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    uint16                      i               = 0;        
    MMI_RESULT_E                recode          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               current_ctrl_id = MMIALM_LISTALL_CTRL_ID;
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = NULL;
//#if !defined MMI_GUI_STYLE_TYPICAL
//    SCI_TIME_T      sys_time = {0};
//    uint16          width = 0;
//    uint16          height = 0;
//    GUI_RECT_T      rect_info =MMITHEME_GetClientRect();
//#endif
                
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:      //create check list box
//#if !defined MMI_GUI_STYLE_TYPICAL
       // if(!MMITHEME_IsMainScreenLandscape())//only display when not landscape
      //  {
      //      s_refresh_timer_id = CreateClockTimer(win_id, s_alm_clk_type);
      //  }       
//#endif        
        GUILIST_SetMaxItem( current_ctrl_id, ALM_ALARM_NUM, FALSE );//max item 3

        for (i = 0; i< ALM_ALARM_NUM; i++)
        {
            uint16 event_id = (uint16)( EVENT_ALARM_0 + i );
            const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent(event_id);

#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
            AppendThreeLineListItem( current_ctrl_id,
                TXT_NULL,
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                FALSE );
#else
            AppendTwoLineListItem( current_ctrl_id,
                TXT_NULL,
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                FALSE );
                //cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
//          SetListItemStyle(current_ctrl_id, i);
// #endif
#endif
        }
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;
        
#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_TIMER:
        if((*(uint8*)param) == s_refresh_timer_id)
        {
            MMK_UpdateScreen();
        }
        break;
#endif        

    case MSG_LOSE_FOCUS:
// #if !defined MMI_GUI_STYLE_TYPICAL
//  //       StopClockTimer();
// #endif
        break;
        
    case MSG_GET_FOCUS:  
// #if !defined MMI_GUI_STYLE_TYPICAL
//         //s_refresh_timer_id = CreateClockTimer(win_id, s_alm_clk_type);
// #endif        
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:	
	/*	if(!MMITHEME_IsMainScreenLandscape())//only display when not landscape
        {
            GUI_POINT_T point;
             GUI_RECT_T      rect_clock =MMITHEME_GetClientRect();
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            GUIRES_GetImgWidthHeight(&width, &height, s_alm_clock_arr[s_alm_clk_type].bg_img_id, win_id);
            rect_clock.top = rect_clock.top + MMI_STATUSBAR_HEIGHT;//+ MMITHEME_GetWinTitleHeight();//+ MMI_STATUSBAR_HEIGHT; 
            rect_clock.bottom = rect_clock.top+height;
            if((point.y>=rect_clock.top) &&(point.y<=rect_clock.bottom))
            {
                s_alm_clk_type++;
                if(s_alm_clk_type == ALM_CLOCK_TYPE_MAX)
                {
                    s_alm_clk_type = ALM_CLOCK_TYPE_1;
                }
                StopClockTimer();
                s_refresh_timer_id = CreateClockTimer(win_id, s_alm_clk_type);
                GUIRES_GetImgWidthHeight(&width, &height, s_alm_clock_arr[s_alm_clk_type].bg_img_id, win_id);
                rect_info.top = rect_info.top + height+ MMI_STATUSBAR_HEIGHT;//+ MMITHEME_GetWinTitleHeight();//+ MMI_STATUSBAR_HEIGHT; 
                GUILIST_SetRect(current_ctrl_id, &rect_info);
                MMK_UpdateScreen();
            }
        }*/		
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
 
     case MSG_LCD_SWITCH: 
     #if 0
        if(!MMITHEME_IsMainScreenLandscape())//only display when not landscape
        {
            //��ͼ����Ӧ����widget��ͼ
            GUIRES_GetImgWidthHeight(&width, &height, IMAGE_WIDGET_ACLOCK, win_id);
            rect_info.top = rect_info.top + height + MMI_STATUSBAR_HEIGHT;//+ MMITHEME_GetWinTitleHeight();//+ MMI_STATUSBAR_HEIGHT; 
            GUILIST_SetRect(current_ctrl_id, &rect_info);
        }
      #endif
         break;
#ifdef TOUCH_PANEL_SUPPORT
     case MSG_CTL_LIST_CHECK_CONTENT:
         {
             // save select
             s_current_event_info.id = GUILIST_GetCurItemIndex(current_ctrl_id);

             MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
             event_delay_ptr = GetDelayEvent(s_current_event_info.id);
             
             s_current_event_info.type = SMART_EVENT_ALARM;
             s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
             
             if (s_current_event_info.event.event_fast_info.is_on ||event_delay_ptr->is_on)
             {
                 s_current_event_info.event.event_fast_info.is_on = FALSE;
             }
             else
             {
                 s_current_event_info.event.event_fast_info.is_on = TRUE;
             }
             // �ر���ʱ����
             SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
             // �޸���������
             MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);      
             MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); 
         }
         break;
#endif

     case MSG_FULL_PAINT:
        {
            BOOLEAN is_update = TRUE;
            
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
            uint16 cur_index = GUILIST_GetCurItemIndex(current_ctrl_id);
            is_update = FALSE;
            GUILIST_RemoveAllItems(current_ctrl_id);
#endif
         for ( i = 0; i < ALM_ALARM_NUM; i++ )
         {
             uint16 event_id = (uint16)( EVENT_ALARM_0 + i );
             const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );

#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
             AppendThreeLineListItem( current_ctrl_id,
                 TXT_NULL,
                 s_alarm_mode_text[event_ptr->fre_mode],
                 event_id,
                 i,
                 is_update);
#else
             AppendTwoLineListItem( current_ctrl_id,
                TXT_NULL,
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                is_update );
				//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 			SetListItemStyle(current_ctrl_id, i);
// #endif
#endif
            }
            
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
         GUILIST_SetCurItemIndex(current_ctrl_id, cur_index);
#endif
        }
         break;
         
     case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_APP_WEB:
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
        //�������ر�
        {
            uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(current_ctrl_id));
            
            if(ALM_ALARM_NUM > id)
            {
                s_current_event_info.id = id;
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                event_delay_ptr = GetDelayEvent(s_current_event_info.id);

                s_current_event_info.type = SMART_EVENT_ALARM;
                s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;

                if (s_current_event_info.event.event_fast_info.is_on ||event_delay_ptr->is_on)
                {
                    s_current_event_info.event.event_fast_info.is_on = FALSE;
#ifdef CALENDAR_SUPPORT
                     MMIAPISCH_DeleteUnreadEvent(s_current_event_info.id);
#endif

                }
                else
                {
                    s_current_event_info.event.event_fast_info.is_on = TRUE;
                }
                // �ر���ʱ����
                SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
                // �޸���������
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);		
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);	
            }
        }
#else
         s_current_event_info.id = GUILIST_GetCurItemIndex(current_ctrl_id);
         //         SCI_MEMCPY((uint8 *)&s_current_event_info.event, 
         //                    (uint8 *)MMIALM_GeEvent((uint16)(EVENT_ALARM_0+s_current_event_info.id)), 
         //                    sizeof(MMIACC_SMART_EVENT_T));
         MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event); 
         if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
         {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
         }
         s_current_event_info.type = SMART_EVENT_ALARM;
         s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
         
         MMK_CreateWin((uint32*)ALARM_EDITWIN_TAB, PNULL);
#ifdef CALENDAR_SUPPORT
         MMIAPISCH_DeleteUnreadEvent(s_current_event_info.id);
#endif
#endif
         break;

    case MSG_APP_OK:
    case MSG_CTL_OK:        // goes to setting menu of alarm
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
        // save select
        MMK_CreateWin((uint32 *)MMIALM_OPTWIN_TAB, PNULL);
#else
        s_current_event_info.id = GUILIST_GetCurItemIndex(current_ctrl_id);
//         SCI_MEMCPY((uint8 *)&s_current_event_info.event, 
//                    (uint8 *)MMIALM_GeEvent((uint16)(EVENT_ALARM_0+s_current_event_info.id)), 
//                    sizeof(MMIACC_SMART_EVENT_T));
        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
        event_delay_ptr = GetDelayEvent(s_current_event_info.id);

        s_current_event_info.type = SMART_EVENT_ALARM;
        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;

        if (s_current_event_info.event.event_fast_info.is_on ||event_delay_ptr->is_on)
        {
        s_current_event_info.event.event_fast_info.is_on = FALSE;
        }
        else
        {
        s_current_event_info.event.event_fast_info.is_on = TRUE;
        }
        // �ر���ʱ����
        SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
        // �޸���������
        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);       
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); 
#endif
        break;

    case MSG_CLOSE_WINDOW:
// #if !defined MMI_GUI_STYLE_TYPICAL
//         //StopClockTimer();
// #endif
        break;

//���clock�н�ʣalarm,����Ӧtab�л�
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYREPEAT_LEFT:
        if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
        {
            GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID,  GUITAB_SWITCH_PRE);
        }
        break;

    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
        {
            GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID, GUITAB_SWITCH_NEXT);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(!MMK_IsChildWin(win_id))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMK_CloseParentWin(MMI_TIMER_PARENT_WIN_ID);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : get the delayed event of event_id
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
LOCAL MMIACC_SMART_EVENT_FAST_TABLE_T *GetDelayEvent(uint16 event_id)
{
    /*switch(event_id)
    {
    case EVENT_ALARM_0:
    case EVENT_ALARM_DELAY_0:
        event_id = EVENT_ALARM_DELAY_0;
        break;

    case EVENT_ALARM_1:
    case EVENT_ALARM_DELAY_1:
        event_id = EVENT_ALARM_DELAY_1;
        break;

    case EVENT_ALARM_2:
    case EVENT_ALARM_DELAY_2:
        event_id = EVENT_ALARM_DELAY_2;
        break;
        
    case EVENT_ALARM_3:
    case EVENT_ALARM_DELAY_3:
        event_id = EVENT_ALARM_DELAY_3;
        break;
        
    case EVENT_ALARM_4:
    case EVENT_ALARM_DELAY_4:
        event_id = EVENT_ALARM_DELAY_4;
        break;
        
    case EVENT_ALARM_5:
    case EVENT_ALARM_DELAY_5:
        event_id = EVENT_ALARM_DELAY_5;
        break;
            
    case EVENT_ALARM_6:
    case EVENT_ALARM_DELAY_6:
        event_id = EVENT_ALARM_DELAY_6;
        break;
            
    case EVENT_ALARM_7:
    case EVENT_ALARM_DELAY_7:
        event_id = EVENT_ALARM_DELAY_7;
        break;
            
    case EVENT_ALARM_8:
    case EVENT_ALARM_DELAY_8:
        event_id = EVENT_ALARM_DELAY_8;
        break;
            
    case EVENT_ALARM_9:
    case EVENT_ALARM_DELAY_9:
        event_id = EVENT_ALARM_DELAY_9;
        break;

    default:
        break;
    }*/
    if(event_id<=EVENT_ALARM_MAX)
    {
        event_id=event_id+EVENT_ALARM_DELAY_0;
    }

    return &s_event_fast_table[event_id];
}

// Added by sunhongzhe for snoozed calendar events.
LOCAL BOOLEAN CalendarHasSnoozedEvents()
{
    BOOLEAN ret = FALSE;
    SCI_DATE_T      date                = {0};
    SCI_TIME_T      time                = {0};
    SCI_ALARM_T cur_time            = {0};
    int i = 0;

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    cur_time.hour = time.hour;
    cur_time.min = time.min;
    cur_time.year = date.year;
    cur_time.mon = date.mon;
    cur_time.mday = date.mday;
    for (i = EVENT_SCH_DELAY_0; i <= EVENT_SCH_DELAY_MAX; i++)
    {
        if (MMIALMSVC_IsValidTimedSvc(s_event_fast_table[i].service_id, cur_time))
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

PUBLIC void MMIALM_HideStatusBarSnoozeIcon()
{
    if (!CalendarHasSnoozedEvents())
    {
        MMIAPICOM_StatusAreaSetReminderState(FALSE);
    }
}

LOCAL void HideSnoozeNotifications(uint16 event_id, MMIACC_SMART_EVENT_E event_type)
{
    if (GetEventAndDelayedIds(&event_id, PNULL)) {}
    MMIALM_RemoveIdlePushMsg(event_id, event_type);
    if (SMART_EVENT_SCH == event_type)
    {
        MMIALM_HideStatusBarSnoozeIcon();
    }
}

LOCAL void SetDelayAlarmTimeAndUpdateUI(
                                MMI_WIN_ID_T win_id,
                                uint16 event_id,
                                MMIACC_SMART_EVENT_T *event_ptr,
                                BOOLEAN    is_auto)
{
    BOOLEAN snoozed = SetDelayAlarmTime(event_id, event_ptr, is_auto);
    MMIACC_SMART_EVENT_E event_type = event_ptr->event_fast_info.type;
    if (snoozed)
    {
        HandleAlarmSoftkeyDissmissMsg(win_id, event_id,
                event_type, event_ptr->event_fast_info.snooze_type);
        if (SMART_EVENT_SCH == event_type)
        {
            MMIAPICOM_StatusAreaSetReminderState(TRUE);
        }
    }
    else
    {
        HideSnoozeNotifications(event_id, event_type);
    }
}

LOCAL void SetAlarmNotifyMark(BOOLEAN mark)
{
    s_alarm_notifying = mark;
}

PUBLIC BOOLEAN MMIAPIALM_IsAlarmNotifying()
{
    return s_alarm_notifying;
}
// End

/*****************************************************************************/
//     Description : enter app by idle notification
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMIALM_AppEntry(void *param1, void *param2)
{
    MMIAPIALM_OpenMainWin();
}

/*****************************************************************************/
//     Description : enter app by idle notification
//    Global resource dependence : none
//  Author:grant.peng
//    Note:
/*****************************************************************************/
PUBLIC void MMICALEND_AppEntry(void *param1, void *param2)
{
    if (param1 != PNULL)
    {
        uint16 id = (uint16)atoi((const char *)param1);
        if (id >= EVENT_SCH_0 && id <= EVENT_SCH_MAX)
        {
            MMIAPISCH_OpenSchEventFromApp(id);
            return;
        }
    }
#ifdef CALENDAR_SUPPORT
    MMIAPICALEND_OpenMainWin();
#endif
}

/*****************************************************************************/
//  Description : close alarm 
//                be activated.
//  Global resource dependence : 
//  Author:   grant.peng
//  Note:   
/*****************************************************************************/
LOCAL void CloseAlarmAliveWin(MMI_WIN_ID_T win_id)
{
    uint16 event_id=0;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    //if (s_arrived_event_id < EVENT_SCH_0)
    // Modified by sunhongzhe for snoozed calendar events.
    if (s_arrived_event_id <= EVENT_SCH_DELAY_MAX)
    {
        event_id = s_arrived_event_id;
        if(!GetEventAndDelayedIds(&event_id, PNULL))
        {
        }
        // End
        /*
        if(s_arrived_event_id<=EVENT_ALARM_MAX)
        {
            event_id = s_arrived_event_id;
        }
        else if(s_arrived_event_id>=EVENT_ALARM_DELAY_0&&s_arrived_event_id<=EVENT_ALARM_DELAY_MAX)
        {
           event_id = s_arrived_event_id-EVENT_ALARM_DELAY_0; 
        }*/

#ifdef CALENDAR_SUPPORT
        MMIAPISCH_DeleteUnreadEvent(event_id);
#endif
        StopRingOrVib(TRUE);
        CloseDelayAlarm(s_arrived_event_id, s_arrived_event.event_fast_info, FALSE);

        if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE)&& !MMIAPIMMS_IsSendingOrRecving())
            &&(0==s_bypast_list.total)
            )
        {
            //��������²���MMK_CloseWin(win_id);����մ򿪵��Զ��ػ������ֹص��ˣ�
            //�ȵ���MMK_CloseWin�������CheckandStartExpiredEvent�ǿ��Եģ�ֻ�� ������CR225729 ��
        }
        else
        {
            if (PNULL != win_info_ptr)//��Ϊ�ճ�widgetҲҪ����������������Լ��뱣������ֹwidget�Ӵ��ڱ��ر�
            {
                MMK_CloseWin(win_id);
            }
        }

        CheckandStartExpiredEvent();
    }
}

/*****************************************************************************/
//  Description : close alarm 
//                be activated.
//  Global resource dependence : 
//  Author:   grant.peng
//  Note:   
/*****************************************************************************/
LOCAL void SnoozedAlarm(MMI_WIN_ID_T win_id)
{
    uint16 event_id=0;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMI_HANDLE_T    win_handle=0;
    BOOLEAN snoozedOk = FALSE;

    win_handle=MMK_ConvertIdToHandle(win_id);

#ifdef CALENDAR_SUPPORT
    MMIAPISCH_DeleteUnreadEvent(s_arrived_event_id);
#endif            
    StopRingOrVib(TRUE);

    if (MMIAPIKL_IsPhoneLocked())
    {
        if(MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)||MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
        }
    }
    // ������ʱ������
    SetDelayAlarmTimeAndUpdateUI(win_id,
            s_arrived_event_id, &s_arrived_event, FALSE);

    if(MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
    {
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
    }

    CheckandStartExpiredEvent();

    if (PNULL != win_info_ptr)//��Ϊ�ճ�widgetҲҪ����������������Լ��뱣������ֹwidget�Ӵ��ڱ��ر�
    {
        MMK_CloseWin(win_id);
    }
}

/*****************************************************************************/
//  Description : close alarm 
//                be activated.
//  Global resource dependence : 
//  Author:   grant.peng
//  Note:   
/*****************************************************************************/
LOCAL void SnoozedPastAlarm(MMI_WIN_ID_T win_id)
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMI_HANDLE_T    win_handle=0;
    
    win_handle=MMK_ConvertIdToHandle(win_id);

#ifdef CALENDAR_SUPPORT
    MMIAPISCH_AddUnreadEvent(s_past_event_id);
#endif            
    StopRingOrVib(TRUE);

    if (MMIAPIKL_IsPhoneLocked())
    {
        if(MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)||MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
        }
    }

    if(MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
    {
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
    }

    if (PNULL != win_info_ptr)//��Ϊ�ճ�widgetҲҪ����������������Լ��뱣������ֹwidget�Ӵ��ڱ��ر�
    {
        MMK_CloseWin(win_id);
    }
    
    CheckandStartExpiredEvent();

}

/*****************************************************************************/
//  Description : when alarm is coming, handle softkey msg 
//                be activated.
//  Global resource dependence : 
//  Author:   grant.peng
//  Note:   
/*****************************************************************************/
LOCAL void HandleAlarmSoftkeyOpenMsg(void)
{
#if 0//def MMI_MSTYLE_SUPPORT
    if(MMIAPIKL_IsPhoneLocked())
    {
        IDLE_LOCK_DATA idle_lock_data = {0};
        idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
        idle_lock_data.app_entry_func_ptr =  MMIALM_AppEntry;
        idle_lock_data.para1 = PNULL;
        idle_lock_data.para2 = PNULL;
        MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
    }
    else
    {
        MMIALM_AppEntry(PNULL, PNULL);
    }
#else //#ifdef MMI_MSTYLE_SUPPORT
    MMIALM_AppEntry(PNULL, PNULL);
#endif //#ifdef MMI_MSTYLE_SUPPORT

}




/*****************************************************************************/
//  Description : when alarm is coming, handle softkey msg 
//                be activated.
//  Global resource dependence : 
//  Author:   grant.peng
//  Note:   
/*****************************************************************************/
LOCAL void HandleAlarmSoftkeyDissmissMsg(MMI_WIN_ID_T win_id,
        uint16 event_id,
        MMIACC_SMART_EVENT_E alarm_type,
        ALM_SNOOZE_TYPE_E snooze_t)
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);


    if(PNULL != win_info_ptr)
    {
        SCI_DATE_T cur_date = {0};
        SCI_TIME_T cur_time = {0};
        MMI_STRING_T tmp_name_str = {0};
        MMI_STRING_T tmp_content_str = {0};
        IDLE_PUSH_MSG_INFO_T push_msg_info = {0};
        char                time_unicode_str[MMIALM_TIME_STR_12HOURS_LEN*2 + 2]  = {0};  //10:34
        char                time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]  = {0};  //10:34
        wchar             wstr_space[]        = {L" "};
        wchar             wstr_old[]        = {L"%U"};
        uint8 record_key[MMIIDLE_APP_NAME_MAX_LEN] = {0};


        TM_GetSysDate(&cur_date);
        TM_GetSysTime(&cur_time);

        // ������ʱ���ʱ��
        IncreaseTimeBySecond(&cur_date, &cur_time, GetSnoozeTimeByType(snooze_t));
        MMIAPISET_FormatTimeStrByTime(cur_time.hour,cur_time.min,(uint8*)time_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
        MMIAPICOM_StrToWstr(time_str, time_unicode_str);
       if (SMART_EVENT_ALARM == alarm_type)
        {
            push_msg_info.e_push_msg_type = MMI_IDLE_PUSH_MSG_SNOOZED_ALARM;
        }
        else if(SMART_EVENT_SCH == alarm_type)
        {
            push_msg_info.e_push_msg_type = MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER;
        }

        if (GetEventAndDelayedIds(&event_id, PNULL)) {}
        MMIAPICOM_Int2Str(event_id, record_key, MMIIDLE_APP_NAME_MAX_LEN);
        strncpy(push_msg_info.appname, record_key, MMIIDLE_APP_NAME_MAX_LEN - 1);

        if(0 != win_info_ptr->string[0].wstr_len)
        {
            MMIAPICOM_Wstrcpy(push_msg_info.title, win_info_ptr->string[0].wstr_ptr);
        }
        else
        {
            if (SMART_EVENT_ALARM == alarm_type)
            {
                MMI_GetLabelTextByLang(STR_DEFAULT_ALARM_NAME, &tmp_name_str);
                MMIAPICOM_Wstrncpy(push_msg_info.title, tmp_name_str.wstr_ptr, tmp_name_str.wstr_len);
            }
            else if(SMART_EVENT_SCH == alarm_type)
            {
                MMI_GetLabelTextByLang(STR_CALE_EVENT_DEFAULT_EXT02, &tmp_name_str);
                MMIAPICOM_Wstrncpy(push_msg_info.title, tmp_name_str.wstr_ptr, tmp_name_str.wstr_len);
            }
        }
        
        MMI_GetLabelTextByLang(STR_CALE_ILIST_SNOOZED_EXT02, &tmp_content_str);

        MMIAPICOM_Wstrncpy(push_msg_info.contents, tmp_content_str.wstr_ptr, tmp_content_str.wstr_len); 

        MMIAPICOM_WstrReplace(push_msg_info.contents, sizeof(push_msg_info.contents), wstr_old, time_unicode_str);
#if 0
        push_msg_info.app_entry_func_ptr = MMIALM_AppEntry;
        push_msg_info.app_left_focus_action_func_ptr = PNULL;
        push_msg_info.app_right_focus_action_func_ptr = PNULL;
#else
        push_msg_info.app_entry_e = IDLE_APP_ENTRY_ALARM;
        push_msg_info.app_left_focus_action_e = IDLE_APP_ENTRY_NULL;
        push_msg_info.app_right_focus_action_e = IDLE_APP_ENTRY_NULL;
#endif
        push_msg_info.icon_data_type = MMI_IDLE_ICON_DATA_IMAGE_ID;
        if (SMART_EVENT_ALARM == alarm_type)
        {
            push_msg_info.icon_data.image_id = IMAGE_IDLE_SCREEN_ALARM;
        }
        else if(SMART_EVENT_SCH == alarm_type)
        {
            push_msg_info.icon_data.image_id = IMAGE_IDLE_SCREEN_CALENDAR;
#if 0
            push_msg_info.app_entry_func_ptr = MMICALEND_AppEntry;
#else
            push_msg_info.app_entry_e = IDLE_APP_ENTRY_CALENDER;
#endif
        }
        
        MMIIDLE_AppendPushMsg(&push_msg_info);
    }

}


/*****************************************************************************/
//  Description : when alarm is coming, handle softkey msg 
//                be activated.
//  Global resource dependence : 
//  Author:   grant.peng
//  Note:   
/*****************************************************************************/
LOCAL void HandleAlarmSoftkeyCloseMsg(MMI_WIN_ID_T win_id)
{

}



/*****************************************************************************/
//  Description : when alarm is coming, handle softkey msg 
//                be activated.
//  Global resource dependence : 
//  Author:   grant.peng
//  Note:   
/*****************************************************************************/
LOCAL void HandleCalendarSoftkeyOpenMsg(uint16 event_id, void *param2)
{
#if 0//def MMI_MSTYLE_SUPPORT
    if(MMIAPIKL_IsPhoneLocked())
    {
        IDLE_LOCK_DATA idle_lock_data = {0};
        idle_lock_data.e_phone_status = MMI_IDLE_PHONE_NORMAL;
        idle_lock_data.app_entry_func_ptr =  MMICALEND_AppEntry;
        idle_lock_data.para1 = PNULL;
        idle_lock_data.para2 = PNULL;
        MMIIDLE_UlockPhoneBySecurityMode(&idle_lock_data);
    }
    else
    {
        MMICALEND_AppEntry(PNULL, PNULL);
    }
    
#else //#ifdef MMI_MSTYLE_SUPPORT
    uint8 record_key[MMIIDLE_APP_NAME_MAX_LEN] = {0};

    if (GetEventAndDelayedIds(&event_id, PNULL)) {}
    MMIAPICOM_Int2Str(event_id, record_key, MMIIDLE_APP_NAME_MAX_LEN);
    MMICALEND_AppEntry(record_key, param2);
#endif //#ifdef MMI_MSTYLE_SUPPORT
}


/*****************************************************************************/
//  Description : when alarm is coming, handle softkey msg 
//                be activated.
//  Global resource dependence : 
//  Author:   grant.peng
//  Note:   
/*****************************************************************************/
LOCAL void HandleCalendarSoftkeyCloseMsg(void)
{

}


/*****************************************************************************/
//  Description : when alarm interrupt is coming, relative alarm will 
//                be activated.
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEventAliveWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_STRING_T    temp_str = {0};
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMI_HANDLE_T    win_handle=0;
    BOOLEAN      snoozed = FALSE;
    //[show alarm expired win when start up charging]
    static uint8    s_focus_timer_id = 0;
    //[end]
    //static BOOLEAN is_operate_backLight=FALSE;
    if (PNULL != win_info_ptr)
    {
        win_info_ptr->win_id = win_id;
    }

    win_handle=MMK_ConvertIdToHandle(win_id);

#ifdef MMI_ETWS_SUPPORT
        if( MMIALM_ALIVEWIN_ID == win_id )
        {
            SCI_TRACE_LOW("HandleEventAliveWinMsg, MMIALM_ALIVEWIN_ID");
        }
        else if(MMIALM_ALIVEWIN_DURING_ETWS_ID == win_id )
        {
            SCI_TRACE_LOW("HandleEventAliveWinMsg, MMIALM_ALIVEWIN_DURING_ETWS_ID");
        }
#endif

    //SCI_TRACE_LOW:" HandleEventAliveWinMsg, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_3609_112_2_18_2_0_57_27,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW: {
        PHONE_STARTUP_CONDITION_E e_startup_cond =
                    MMIAPIPHONE_GetStartUpCondition();
        BOOLEAN abnormal_poweron = e_startup_cond == STARTUP_ALARM
                || e_startup_cond == STARTUP_CHARGE;
        BOOLEAN is_alarm = SMART_EVENT_ALARM == s_arrived_event.event_fast_info.type;
        BOOLEAN is_sch = SMART_EVENT_SCH == s_arrived_event.event_fast_info.type;

        s_open_event_id = s_arrived_event_id;//CR192240
        temp_str.wstr_len = s_arrived_event.event_content_info.w_length;
        temp_str.wstr_ptr = s_arrived_event.event_content_info.w_content;

        if (is_alarm)
        {
#ifdef MMI_ALARM_MINI_SUPPORT
//            MMIPUB_SetWinSoftkey(MMIALM_ALIVEWIN_ID, STR_POC_ILIST_OFF, STXT_SOFTKEY_SLEEP_MK, FALSE);
#else
            //MMIPUB_SetWinSoftkey(MMIALM_ALIVEWIN_ID, TXT_CLOSE, TXT_ALARM_DELAY, FALSE);
#endif
            MMI_GetLabelTextByLang(TXT_ALARM_CLOCK, &temp_str);
            //MMIPUB_SetWinTitleTextId(win_id, TXT_ALARM_CLOCK, FALSE);
        }
        else if (is_sch)
        {
            MMIPUB_SetWinTitleTextId(win_id,TXT_IDLE_CALENDAR,FALSE);
        }


#ifdef MMI_ETWS_SUPPORT
        if( TRUE == s_is_need_ring_and_vib)
        {
#endif        
        //[modify power off alarm function]
        if ((is_alarm || is_sch) && abnormal_poweron)
        {
            SetAlarmDialogSoftkey(MMIALM_ALIVEWIN_ID);
        }
        //[end]

        if((MMK_GetWinPriority(win_handle)==WIN_LOWEST_LEVEL)
            ||(MMIAPICC_IsInState(CC_IN_CALL_STATE)))//��̨�������
        {
            StartAlarmTimer(&s_event_timer_id, BACKGROUND_ALERT_RING_DURATION, FALSE);
        }
        else
        {
#ifdef CMCC_UI_STYLE
            if(s_arrived_event.event_content_info.ring_dur > 0)
            {
                StartAlarmTimer(&s_event_timer_id, s_arrived_event.event_content_info.ring_dur*60*1000, FALSE);//ALERT_RING_DURATION
            }
            else
            {
                StartAlarmTimer(&s_event_timer_id, ALERT_RING_DURATION, FALSE);
            }
#else
            StartAlarmTimer(&s_event_timer_id, ALERT_RING_DURATION, FALSE);
#endif
        }
		if (!is_alarm)
		{
        	StartRingOrVib();//U��ʹ���е����Ҳ�ڸú����д���
		}

#ifdef MMI_ETWS_SUPPORT
           }
        else
        {
            s_is_need_ring_and_vib = TRUE;
        }
#endif


        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        //�����ǵ�����Ļ�Ĳ���,�����MMIPUB_HandleWaitWinMsg֮����
        if ( STARTUP_ALARM != e_startup_cond)
        {
            if(/*MMIDEFAULT_IsAllowBackLightTurnOff()&&*/ MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
            {
                //MMIDEFAULT_TurnOnBackLight();
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                //is_operate_backLight=TRUE;
            }
        }
        else
        {
            //MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            //is_operate_backLight=TRUE;
        }

#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_ALARM,&temp_str);
#endif

        //[set power off alarm expired win's opacity]
        if(STARTUP_ALARM == e_startup_cond)
        {
            //SCI_TRACE_LOW("HandleEventAliveWinMsg startup condition = %d",MMIAPIPHONE_GetStartUpCondition());
            MMIPUB_SetDialogBgLayerStyle(win_id,DIALOG_FULL_BG_SCREEN);
        }
        //[491289]
        //[show alarm expired win when start up charging]
        if (MMK_GetWinPriority(win_handle) != WIN_LOWEST_LEVEL)
        {
            StartAlarmTimer(&s_focus_timer_id, 100, FALSE);
        }
		if (is_alarm)
		{
			/*Before turn on backlight, need update idle window date time.*/
			if ((MMIAPIIDLE_IdleWinIsOpen())
					&& (MMIDEFAULT_GetBackLightStateEx() == MMI_BACKLIGHT_OFF)
					&& abnormal_poweron == FALSE)
			{
				MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_FULL_PAINT, NULL);
			}
			MMIDEFAULT_TurnOnBackLight();
			StartRingOrVib();
		}
        //[end]
        break;
    }
    case MSG_FULL_PAINT:
        {

#ifdef MMI_ETWS_SUPPORT
       if( MMIALM_ALIVEWIN_DURING_ETWS_ID == win_id)
        {
            break;
        }
#endif       
            
            if(PNULL != win_info_ptr)
            {
                DisplayAlarmWin(win_id,win_info_ptr);
            }
        }
        break;

    case MSG_GET_FOCUS:
        if(MMK_GetWinPriority(win_handle)==WIN_LOWEST_LEVEL)//��̨��������²������κ��¡�
        {//�޸�����ο�CR122782��comments
            break;
        }
        if (s_event_timer_id ==0)
        {
            if (MMIAPIKL_IsPhoneLocked())
            {
                MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
            }
        }
        else
        {
            if ( STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition())
            {
                if(/*MMIDEFAULT_IsAllowBackLightTurnOff()&&*/ MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
                {
                    MMIDEFAULT_TurnOnBackLight();
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                    //is_operate_backLight=TRUE;
                }
            }
            else
            {
                MMIDEFAULT_TurnOnBackLight();
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                //is_operate_backLight=TRUE;
            }
//�޸�����ο�CR122782��comments
            if(MMIAPIKL_IsPhoneLocked())
            {
                MMK_PostMsg(win_id,MSG_ALM_START_RING,PNULL,0);//�������Ҷ�������������
            }
            else
            {
                StartRingOrVib();
            }
        }
        break;
    case MSG_ALM_START_RING:
        StartRingOrVib();
        break;

    case MSG_LOSE_FOCUS:
//�޸�����ο�CR122782��comments
        StopRingOrVib(FALSE);
        //SetDelayAlarmTime(s_arrived_event_id, s_arrived_event, TRUE);
        //if(is_operate_backLight)
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            //is_operate_backLight=FALSE;
        }
        //[add interface for judge if alarm win is on foreground]
        SetAlarmNotifyMark(FALSE);
        //[end]
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_PROMPTWIN_OK: {
    //case MSG_KEYDOWN_FLIP://�ϸ�Ӧ�úͰ�ֹͣ��ͬ������,@zhaohui,cr102341
        //[modify power off alarm function]
        PHONE_STARTUP_CONDITION_E e_startup_cond =
                MMIAPIPHONE_GetStartUpCondition();
        MMIACC_SMART_EVENT_E event_type = s_arrived_event.event_fast_info.type;
        BOOLEAN isalarm = SMART_EVENT_ALARM == event_type;
        BOOLEAN isschedule = SMART_EVENT_SCH == event_type;
        uint16 eventid = s_arrived_event_id;
        BOOLEAN abnormal_poweron = e_startup_cond == STARTUP_ALARM
                || e_startup_cond == STARTUP_CHARGE;

#ifdef MMI_ETWS_SUPPORT
       if( MMIALM_ALIVEWIN_DURING_ETWS_ID == win_id)
        {
            break;
        }
#endif       
    
        // Remove idle snoozed event message.
        if (isalarm || isschedule)
        {
            HideSnoozeNotifications(eventid, event_type);
        }

        if ((isalarm || isschedule) && abnormal_poweron)
        {
            //SCI_TRACE_LOW("power_off_alarm press left softkey,do nothing");
            break;
        }
        //[end]

        CloseAlarmAliveWin(win_id);
        if (isalarm)
        {
            HandleAlarmSoftkeyOpenMsg();
        }
        else if (isschedule)
        {
            HandleCalendarSoftkeyOpenMsg(eventid, PNULL);
        }

        break;
   }
   case MSG_APP_WEB:
        if (SMART_EVENT_ALARM == s_arrived_event.event_fast_info.type 
            ||SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
        {
            SnoozedAlarm(win_id);
        }

        break;

    case MSG_TIMER:
    {
        uint16 eventid = s_arrived_event_id;
        MMIACC_SMART_EVENT_E event_type = s_arrived_event.event_fast_info.type;
        BOOLEAN isschedule = SMART_EVENT_SCH == event_type;
        MMISET_ALL_RING_TYPE_E  ring_type = MMISET_RING_TYPE_ALARM;
        if (isschedule)
        {
            ring_type = MMISET_RING_TYPE_SCH;
        }

        if (s_vibrator_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            if (is_vibrator_on)
            {
                MMIAPISET_SetVibrator(FALSE, ring_type);
                is_vibrator_on = FALSE;
            }
            else
            {
                MMIAPISET_SetVibrator(TRUE, ring_type);
                is_vibrator_on = TRUE;
            }
            StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
        }
        else if (s_event_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            // ֹͣ����
            if(is_ring_on)
            {
                StopRingOrVib(TRUE);
            }
            else
            {
                MMK_StopTimer(s_event_timer_id);
                s_event_timer_id = 0;
            }

            if(s_vibrator_timer_id!=0)
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            is_vibrator_on = FALSE;
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_AddUnreadEvent(eventid);
#endif
            if (MMIAPIKL_IsPhoneLocked())
            {
                if(MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)||MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
                }
            }
            // ������ʱ������
            SetDelayAlarmTimeAndUpdateUI(win_id,
                    eventid, &s_arrived_event, TRUE);

            //CheckandStartExpiredEvent();
            //if(is_operate_backLight)
            if(MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
            {
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                //is_operate_backLight=FALSE;
            }
            //shipeng.lv end

            CheckandStartExpiredEvent();
            if (PNULL != win_info_ptr)//��Ϊ�ճ�widgetҲҪ����������������Լ��뱣������ֹwidget�Ӵ��ڱ��ر�
            {//�޸�����ο�CR122782��comments
                MMK_CloseWin(win_id);
            }
        }
        //[show alarm expired win when start up charging]
        else if (s_focus_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            MMI_HANDLE_T    win_handle = MMK_ConvertIdToHandle(win_id);
            MMI_WIN_PRIORITY_E win_priority = MMK_GetWinPriority(win_handle);
            MMI_WIN_ID_T    focus_win_id = MMK_GetFocusWinId();

            if(focus_win_id != win_id)
            {
#ifdef MMI_ETWS_SUPPORT
                if( FALSE == MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
#endif                 
                {
                MMK_SetWinPriority(win_handle, WIN_THREE_LEVEL);
                MMK_WinGrabFocus(win_id);
                }
                MMK_SetWinPriority(win_handle, win_priority);
                MMK_PostMsg(win_id, MSG_FULL_PAINT,PNULL,0);
            }
        }
        //[end]
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    }
#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP://�ϸ�Ӧ�úͰ�ֹͣ��ͬ������,@zhaohui,cr102341
        StopRingOrVib(TRUE);
        CloseDelayAlarm(s_arrived_event_id, s_arrived_event.event_fast_info, FALSE);
        if (PNULL != win_info_ptr)//��Ϊ�ճ�widgetҲҪ����������������Լ��뱣������ֹwidget�Ӵ��ڱ��ر�
        {
            MMK_CloseWin(win_id);
        }
        CheckandStartExpiredEvent();
        recode = MMI_RESULT_FALSE;
        break;
#endif

    case MSG_CLOSE_WINDOW:

#ifdef MMI_ETWS_SUPPORT
       if( MMIALM_ALIVEWIN_DURING_ETWS_ID == win_id)
        {
            MMIAPIALM_OpenAlarmDialogWin();
            break;
        }
#endif       
        
        if (s_event_timer_id != 0)
        {//CR192240
            MMIACC_SMART_EVENT_T open_event = {0};

            MMIAPIALM_FormInteEventInfo(s_open_event_id,&open_event);
            SetDelayAlarmTimeAndUpdateUI(win_id,
                    s_open_event_id, &open_event, TRUE);

            s_open_event_id = 0;
        }
        StopRingOrVib(TRUE);
        //if(is_operate_backLight)
        if(MMK_GetWinPriority(win_handle)!=WIN_LOWEST_LEVEL)
        {
            MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            //is_operate_backLight=FALSE;
        }
#ifndef SUBLCD_SIZE_NONE
        //ˢ��С����ʾ���ر���ʾ���ӵ��¼���Ϣ
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_ALARM,PNULL);
#endif
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        MMIAPICT_CheckandStartDelayedCoutimeEvent();//@zhaohui,counted time,�����ӳٵĵ���ʱ����ʾ
        //[add interface for judge if alarm win is on foreground]
        SetAlarmNotifyMark(FALSE);
        //[end]
        MMK_DelayUpdateScreen();
        break;

    case MSG_KEYDOWN_RED:
    case MSG_KEYUP_POWER:
        {
            //[don't return to mainmenu win
            // when press end key in power off alarm win]
            PHONE_STARTUP_CONDITION_E e_startup_cond =
                        MMIAPIPHONE_GetStartUpCondition();
            BOOLEAN abnormal_poweron = e_startup_cond == STARTUP_ALARM ||
                    e_startup_cond == STARTUP_CHARGE;
            //[end]

            StopRingOrVib(TRUE);
            SetDelayAlarmTimeAndUpdateUI(win_id,
                    s_arrived_event_id, &s_arrived_event, FALSE);

            if (MMIAPIKL_IsPhoneLocked()||
                (s_bypast_list.total > 0)||
                (s_poweroff_event.event_fast_info.is_valid)||
                MMIAPICT_CheckDelayedCoutimeEvent()||
                MMK_IsOpenWin(MMISET_FLY_MODE_OPEN_PHONE_WIN_ID) ||
                abnormal_poweron
                )//����������ʱ�¼�����ֻ�رյ�ǰ����
            {
                recode = MMI_RESULT_TRUE;
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }

            CheckandStartExpiredEvent();

            if (MMI_RESULT_TRUE == recode||MSG_KEYUP_POWER==msg_id)
            {
                if (PNULL != win_info_ptr)//��Ϊ�ճ�widgetҲҪ����������������Լ��뱣������ֹwidget�Ӵ��ڱ��ر�
                {
                    MMK_CloseWin(win_id);
                }
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_PROMPTWIN_CANCEL:
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYUP_UPSIDE:
    case MSG_KEYUP_DOWNSIDE:
    case MSG_KEYLONG_UPSIDE:
    case MSG_KEYLONG_DOWNSIDE:
    case MSG_KEYPRESSUP_DOWNSIDE:
    case MSG_KEYPRESSUP_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE: {
        MMIACC_SMART_EVENT_E type = s_arrived_event.event_fast_info.type;
        StopRingOrVib(TRUE);

        if (SMART_EVENT_ALARM == type
            ||SMART_EVENT_SCH == type)
        {
            uint16 eventid = s_arrived_event_id;

            // Remove idle snoozed event message.
            HideSnoozeNotifications(eventid, type);

            CloseDelayAlarm(eventid, s_arrived_event.event_fast_info, FALSE);
        }
        else
        {
            SetDelayAlarmTime(s_arrived_event_id, &s_arrived_event, FALSE);
        }

#ifdef CALENDAR_SUPPORT
        MMIAPISCH_DeleteUnreadEvent(s_arrived_event_id);
#endif
        // send a msg to vertual win 
        //if ( STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition())
        //{
        //  MMK_SendMsg(VIRTUAL_WIN_ID, MSG_ALM_ALERTING_END, PNULL);
        //}
        //else
        //{
            //CheckandStartExpiredEvent();
        //}
        //MMK_CloseWin(win_id);
        if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
                &&(0==s_bypast_list.total)
            )
        {
            CheckandStartExpiredEvent();
            //��������²���MMK_CloseWin(win_id);����մ򿪵��Զ��ػ������ֹص��ˣ�
            //�ȵ���MMK_CloseWin�������CheckandStartExpiredEvent�ǿ��Եģ�ֻ�� ������CR225729 ��
        }
        else
        {
            CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO
            if (PNULL != win_info_ptr)//��Ϊ�ճ�widgetҲҪ����������������Լ��뱣������ֹwidget�Ӵ��ڱ��ر�
            {
                MMK_CloseWin(win_id);
            }
        }
        break;
    }
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_KEYUP_FLIP:
        MMIDEFAULT_HandleFlipKey(TRUE);
        break;
#endif

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Stop ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   
/*****************************************************************************/
LOCAL void StopRingOrVib(BOOLEAN is_close_ring_timer)
{
    MMISET_ALL_RING_TYPE_E  ring_type = MMISET_RING_TYPE_ALARM;
    if (SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
    {
        ring_type = MMISET_RING_TYPE_SCH;
    }

    if (0 != s_vibrator_timer_id )
    {
        MMK_StopTimer(s_vibrator_timer_id);
        s_vibrator_timer_id = 0;
    }

    MMIAPISET_SetVibratorEx(FALSE,ring_type,FALSE,0);
    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_NONE);

    if(is_close_ring_timer)
    {
        if (0 != s_event_timer_id )
        {
            MMK_StopTimer(s_event_timer_id);
            s_event_timer_id = 0;
        }
    }

    if(!is_ring_on)
    {
        return ;
    }

    if (ALM_FIX_RING == s_cur_alarm_ring_type)
    {
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
    }
    else if(ALM_DOWNLOAD_RING == s_cur_alarm_ring_type
        ||ALM_RECORD_RING == s_cur_alarm_ring_type)
    {
        MMIAPISET_StopAppRing();
    }
#ifdef FM_SUPPORT
    else
    {
        MMISRVAUD_Stop(MMIAPIALARM_GetPlayHandle());
        MMIAPIFM_Stop();
        MMIAPIALARM_FreePlaySrv();
    }
#endif

    is_ring_on=FALSE;
}

/*****************************************************************************/
//  Description : Start timer about alarm
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void StartAlarmTimer(
                           uint8 *timer_id,
                           uint32 time_out,
                           BOOLEAN is_period
                           )
{
    //SCI_ASSERT(timer_id != NULL);/*assert verified*/
    if (NULL == timer_id)
    {
        return;
    }
    
    if (0 != *timer_id)
    {
        MMK_StopTimer(*timer_id);
        *timer_id = 0;
    }
    *timer_id = MMK_CreateTimer(time_out, is_period);
}

PUBLIC void MMIALM_SetAlarmRedisplayFlag(BOOLEAN flag)
{
    s_alarm_win_redisplay = flag;
}

PUBLIC BOOLEAN MMIALM_GetAlarmRedisplayFlag()
{
    return s_alarm_win_redisplay;
}

PUBLIC void MMIALM_SetAlarmVibrateDelayedFlag(BOOLEAN flag)
{
    s_alarm_vibrate_delayed_flag = flag;
}

PUBLIC BOOLEAN MMIALM_GetAlarmVibrateDelayedFlag()
{
    return s_alarm_vibrate_delayed_flag;
}

PUBLIC void MMIALM_Redisplay()
{
    OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
}

PUBLIC void MMIALM_StartRingOrVib()
{
    StartRingOrVib();
}

/*****************************************************************************/
//  Description : Start ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   
/*****************************************************************************/
#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN WRE_IsFocusRunWin(void);
#endif
LOCAL void StartRingOrVib(void)
{
//    uint8           ring_vol = 0;
    uint8           opt_type = MMIAPIENVSET_GetActiveModeOptValue(0,ALARM_RING_TYPE); /*lint !e64*/
    uint8           native_opt_type = opt_type;
#ifdef CMCC_UI_STYLE
    BOOLEAN         is_need_vibrate = FALSE;
#endif
    BOOLEAN place_holder = FALSE;
    MMISET_ALL_RING_TYPE_E  ring_type = MMISET_RING_TYPE_ALARM;
    if (SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
    {
        ring_type = MMISET_RING_TYPE_SCH;
    }

    //SCI_TRACE_LOW:"mmialarm.c: ring type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4024_112_2_18_2_0_58_28,(uint8*)"d", opt_type);

#ifdef CMCC_UI_STYLE
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef VT_SUPPORT
        || MMIAPIVT_IsVtCalling()
#endif
        )
    {
        return;
    }
#endif

#ifdef MET_MEX_SUPPORT
    if(1==MMIMex_Phs_HookResult(MEX_MSG_ID_WILL_PLAY_ALARMTONE, NULL, 0, NULL, MEX_HOOK_MSG_FROM_ALARM))
    {
        mex_log(1, "hook almTone");
        //mex application registered to hook this message
        return;
    }
#endif /* MET_MEX_SUPPORT */

    is_ring_on=TRUE;

#ifdef LEBAO_MUSIC_SUPPORT
	{
		extern int lebao_playing_enter_stop_state(int mode);	/* lebao\music\view\lebao_app.c */
		lebao_playing_enter_stop_state(0);
	}
#endif
    //[alarm can response when answering call]
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIAPISET_PlayRing(MN_DUAL_SYS_MAX,FALSE,0,1,MMISET_RING_TYPE_MSG_IN_CALL,PNULL);
        return;
    }else
    //[end]
    if(place_holder//MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef JAVA_SUPPORT
        || MMIAPIJAVA_IsFocusJAVARunWin()
#endif
//NEWMS00220222
#if defined(BROWSER_SUPPORT) && defined(LOW_MEMORY_SUPPORT_32X16)
        || MMIAPIBROWSER_IsRunning()
#endif
#ifdef WRE_SUPPORT
        || WRE_IsFocusRunWin()
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
#ifdef VT_SUPPORT
        || MMIAPIVT_IsVtCalling()
#endif
        || MMIAPIATV_IsRecording()
        ) //cr63879
    {
        if((MMISET_MSG_RING == opt_type)||(MMISET_MSG_VIBRA_AND_RING == opt_type))
        {
            opt_type = MMISET_MSG_VIBRA;
        }
    }

    else if ((MMIAPIFMM_IsFileOption()) ||
             (MMIAPIPICVIEW_IsDeleteFiles()))
    {
        if(MMISET_MSG_RING == opt_type
            || MMISET_MSG_VIBRA_AND_RING == opt_type)
        {
            opt_type = MMISET_MSG_VIBRA;
        }
    }

    switch (opt_type)
    {
    case MMISET_MSG_RING: //ring
   // case MMISET_MSG_SILENT: // Added by sunhongzhe to fix Bug 533182, Alarm tone doesn't play, when DUT is in silent mode.
#ifdef MMI_DUALMODE_ENABLE
#ifdef CMCC_UI_STYLE
        if (MMIAPISET_IsPermitPlayRingForVibrate(ring_type) && s_arrived_event.event_content_info.is_vibrate)
        {
            is_need_vibrate = TRUE;
        }
#endif
#endif
        StartRingForRingOrVib(MMISET_MSG_RING);
#ifdef CMCC_UI_STYLE
        if(is_need_vibrate)
        {
            MMIAPISET_SetVibrator(TRUE, ring_type);
            if (0 != s_vibrator_timer_id )
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            StartAlarmTimer(&s_vibrator_timer_id, MMI_3SECONDS, FALSE);
            is_vibrator_on = TRUE;
        }
 #endif
        break;

    case MMISET_MSG_VIBRA: //vibration
        if (MMIAPISET_IsPermitPlayRingForVibrate(ring_type))
        {

#ifdef VIRTUAL_VIBRATE_FUNC
            // Added by sunhongzhe  to fix Bug 533182, set alarm ringtone volume as 7.
            if (native_opt_type == opt_type && ring_type == MMISET_RING_TYPE_ALARM)
            {
                //StartRingForRingOrVib(MMISET_MSG_VIBRA_AND_RING);
                //MMIAPISET_SetVibratorWithAlarmVolume(TRUE,ring_type,TRUE,MMI_1SECONDS);
                //bug1089693 by zgt.2019.6.6
                MMIAPISET_SetVibrator(TRUE, ring_type);
                if (0 != s_vibrator_timer_id )
                {
                    MMK_StopTimer(s_vibrator_timer_id);
                    s_vibrator_timer_id = 0;
                }
                StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
                is_vibrator_on = TRUE;
            }
            else
            // End.
#endif
            {
                MMIAPISET_SetVibrator(TRUE, ring_type);
                if (0 != s_vibrator_timer_id )
                {
                    MMK_StopTimer(s_vibrator_timer_id);
                    s_vibrator_timer_id = 0;
                }
                StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
                is_vibrator_on = TRUE;
            }
        }

        break;

    case MMISET_MSG_SILENT: //silent    Removed by sunhongzhe to fix Bug 533182.
        if(MMIAPISET_IsPermitPlayRingForVibrate(ring_type))
        {
            //do nothing ~
       }
       break;
    
    case MMISET_MSG_VIBRA_AND_RING: // ring&vibration
        if (MMIAPISET_IsPermitPlayRingForVibrate(ring_type))
        {
#ifdef VIRTUAL_VIBRATE_FUNC
            MMIAPISET_SetVibStartPara();
#endif
            StartRingForRingOrVib(MMISET_MSG_VIBRA_AND_RING);
            //[modify alarm/calendar play vibrate]
#ifdef VIRTUAL_VIBRATE_FUNC
            MMIAPISET_SetVibratorWithAlarmVolume(TRUE,ring_type,TRUE,MMI_1SECONDS);
#else
       MMIAPISET_SetVibratorEx(TRUE,ring_type,TRUE,MMI_1SECONDS);
#endif
            //[end]
            if (0 != s_vibrator_timer_id )
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            //[modify alarm/calendar play vibrate]
            //StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
            //is_vibrator_on = TRUE;
            //[end]
        }
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : Start ring for ring or vibrate
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note:   leon.wang add
/*****************************************************************************/
LOCAL void StartRingForRingOrVib(uint8 opt_type)
{
    uint8   ring_vol = 0;
    MMISET_ALL_RING_TYPE_E ring_class = MMISET_RING_TYPE_ALARM;

    if (SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
    {
        ring_class = MMISET_RING_TYPE_SCH;
    }
    
    if (ALM_FIX_RING == s_arrived_event.event_content_info.ring_type)
    {  
        MMIAPISET_PlayRingById(TRUE,s_arrived_event.event_content_info.ring, 0xffff, ring_class, PNULL);
        s_cur_alarm_ring_type=ALM_FIX_RING;
    }
    else if(ALM_DOWNLOAD_RING == s_arrived_event.event_content_info.ring_type
            ||ALM_RECORD_RING == s_arrived_event.event_content_info.ring_type)
    {               
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPISET_PlayRingById(TRUE, 1, 0xffff, ring_class, PNULL);
            s_cur_alarm_ring_type=ALM_FIX_RING;
        }
        else
        {
            if(MMIAPIFMM_IsFileExist(s_arrived_event.event_ring_info.dring_name_arr,
                                   s_arrived_event.event_ring_info.dring_name_len))
            {
                ring_vol = MMIAPIENVSET_GetActiveModeRingVol(ring_class);
                if(0 < ring_vol)
                {
                    //reset audio mode
                    MMIAPISET_SetAudioMode(TRUE);
                    
                    if(!MMIAPIFMM_PlayMusicFile(ring_class, FALSE,
                                            s_arrived_event.event_ring_info.dring_name_arr,
                                            s_arrived_event.event_ring_info.dring_name_len,
                                            0xffff,
                                            0,
                                            ring_vol,
                                            NULL))
                    {
                        MMIAPISET_PlayRingById(TRUE, 1, 0xffff, ring_class, PNULL);
                        s_cur_alarm_ring_type=ALM_FIX_RING;
                    }
                    else
                    {
                        s_cur_alarm_ring_type=s_arrived_event.event_content_info.ring_type;
                    }
                    //reset audio mode
                    MMIAPISET_SetAudioMode(FALSE);
                }
            }
            else
            {
                //s_arrived_event.event_content_info.ring_type = ALM_FIX_RING;
                //s_arrived_event.event_content_info.ring = 1;
                MMIAPISET_PlayRingById(TRUE, 1, 0xffff, ring_class, PNULL);
                s_cur_alarm_ring_type=ALM_FIX_RING;
            }
        }
    }
#ifdef FM_SUPPORT
    else
    {               
        ring_vol = MMIAPIENVSET_GetActiveModeRingVol(ring_class);       
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIAPISET_PlayRingById(TRUE, 1, 0xffff, ring_class, PNULL);
            s_cur_alarm_ring_type=ALM_FIX_RING;
        }
        else
        {
            MMI_STRING_T  str_ptr={0};
            str_ptr.wstr_ptr=s_arrived_event.event_ring_info.dring_name_arr;
            str_ptr.wstr_len=s_arrived_event.event_ring_info.dring_name_len;
            if( MMIAPIALM_IsValidFM(s_arrived_event.event_ring_info.dring_name_arr,s_arrived_event.event_ring_info.dring_name_len)
                    && (MMIFM_CheckHeadsetStatusWithoutPrompt()) && (!MMIAPISET_GetFlyMode()))
            {
                //SCI_TRACE_LOW:"mmialarm.c MMIAPIFM_Play"
                if(MMISET_MSG_RING==opt_type)
                {
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4141_112_2_18_2_0_58_29,(uint8*)"");
                }
                else
                {
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4246_112_2_18_2_0_58_30,(uint8*)"");
                }
                MMIAPIALARM_ReqPlaySrv();
                MMIAPIFM_Play(MMIAPIALARM_GetPlayHandle(), MMIAPIFM_ConvertStringToFreq(&str_ptr));
                s_cur_alarm_ring_type=ALM_FM_RING;
            }
            else
            {
                //s_arrived_event.event_content_info.ring_type = ALM_FIX_RING;
                //s_arrived_event.event_content_info.ring = 1;
                MMIAPISET_PlayRingById(TRUE, 1, 0xffff, ring_class, PNULL);
                s_cur_alarm_ring_type=ALM_FIX_RING;
            }
        }
    }
#endif
}
/*****************************************************************************/
//  Description : pop up a msg 
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleAlarmExpiredWinMsg(
//                                              MMI_WIN_ID_T       win_id,     
//                                              MMI_MESSAGE_ID_E   msg_id,
//                                              DPARAM             param
//                                              )
// {
//     MMI_RESULT_E    recode = MMI_RESULT_TRUE;
// 
//     //SCI_TRACE_LOW:"mmialarm.c HandleAlarmExpiredWinMsg() msg_id = %x"
//     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4287_112_2_18_2_0_58_31,(uint8*)"d", msg_id);
//     switch(msg_id)
//     {
//     case MSG_GET_FOCUS:
//         MMIDEFAULT_TurnOnBackLight();
//         break;
// 
//     default:
//         break;
//     }
// 
//     recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
//     
//     return recode;
// }


/*****************************************************************************/
//  Description : start delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SetDelayAlarmTime(
                                uint16 event_id,
                                MMIACC_SMART_EVENT_T *event_ptr,
                                BOOLEAN    is_auto
                                )
{
    BOOLEAN    result   = TRUE;
    SCI_DATE_T cur_date = {0};
    SCI_TIME_T cur_time = {0};
    uint16 delay_event_id = 0;
    MMIALMSVC_ID_T temp_service_id = 0;
    MMIACC_SMART_EVENT_T event = *event_ptr;

    /*switch(event_id)
    {
    case EVENT_ALARM_0:
    case EVENT_ALARM_DELAY_0:
        delay_event_id = EVENT_ALARM_DELAY_0;
        event_id = EVENT_ALARM_0;
        break;

    case EVENT_ALARM_1:
    case EVENT_ALARM_DELAY_1:
        delay_event_id = EVENT_ALARM_DELAY_1;
        event_id = EVENT_ALARM_1;
        break;

    case EVENT_ALARM_2:
    case EVENT_ALARM_DELAY_2:
        delay_event_id = EVENT_ALARM_DELAY_2;
        event_id = EVENT_ALARM_2;
        break;

    case EVENT_ALARM_3:
    case EVENT_ALARM_DELAY_3:
        delay_event_id = EVENT_ALARM_DELAY_3;
        event_id = EVENT_ALARM_3;
        break;

    case EVENT_ALARM_4:
    case EVENT_ALARM_DELAY_4:
        delay_event_id = EVENT_ALARM_DELAY_4;
        event_id = EVENT_ALARM_4;
        break;

    case EVENT_ALARM_5:
    case EVENT_ALARM_DELAY_5:
        delay_event_id = EVENT_ALARM_DELAY_5;
        event_id = EVENT_ALARM_5;
        break;

    case EVENT_ALARM_6:
    case EVENT_ALARM_DELAY_6:
        delay_event_id = EVENT_ALARM_DELAY_6;
        event_id = EVENT_ALARM_6;
        break;

    case EVENT_ALARM_7:
    case EVENT_ALARM_DELAY_7:
        delay_event_id = EVENT_ALARM_DELAY_7;
        event_id = EVENT_ALARM_7;
        break;

    case EVENT_ALARM_8:
    case EVENT_ALARM_DELAY_8:
        delay_event_id = EVENT_ALARM_DELAY_8;
        event_id = EVENT_ALARM_8;
        break;

    case EVENT_ALARM_9:
    case EVENT_ALARM_DELAY_9:
        delay_event_id = EVENT_ALARM_DELAY_9;
        event_id = EVENT_ALARM_9;
        break;

    default:
        // ������һ���ճ�
        FindLatestEventToSet();
        return FALSE;
        //break;
    }*/
    // Modified by sunhongzhe for snoozed calendar events.
    if (GetEventAndDelayedIds(&event_id, &delay_event_id))
    {
    }
    // End
    /*
    if(event_id<=EVENT_ALARM_MAX)
    {
        delay_event_id = event_id+EVENT_ALARM_DELAY_0;
        event_id = event_id;
    }
    else if(event_id>=EVENT_ALARM_DELAY_0&&event_id<=EVENT_ALARM_DELAY_MAX)
    {
       delay_event_id = event_id;
       event_id = event_id-EVENT_ALARM_DELAY_0; 
    }*/
    else
    {
        temp_service_id = s_event_fast_table[event_id].service_id;
        FindLatestEventToSet();
        MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
        return FALSE;
    }


    // �ж��Ƿ��ֶ���ʱ
    if (is_auto)// �Զ���ʱ
    {
        event.event_fast_info.delay_num++;
    }
    else // �ֶ���ʱ ������ʱ����Ϊ0
    {
        event.event_fast_info.delay_num = 0;
    }

    if (MMIALM_DELAY_MAX_NUM < event.event_fast_info.delay_num)
    {
        event.event_fast_info.is_valid = FALSE;
        event.event_fast_info.is_on = FALSE;
        if (ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode)
        {
            s_event_fast_table[event_id].is_on = FALSE;//�ر���ʱ���ӵ�ͬʱ,������ʱ��ԭ����Ҳ�ر�
        }
        result = FALSE;
        temp_service_id = s_event_fast_table[delay_event_id].service_id;
        SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event.event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id������Ӧ���Լ���event_id        
    }
    else
    {
        //[alarm snooze time]
        uint16 snooze_time = GetSnoozeTimeByType(event.event_fast_info.snooze_type);
        //[end]

        event.event_fast_info.is_on = TRUE;

        //TM_GetSysDate(&cur_date);
        //TM_GetSysTime(&cur_time);
        cur_date.year = event.event_fast_info.year;
        cur_date.mon = event.event_fast_info.mon;
        cur_date.mday = event.event_fast_info.day;
        cur_time.hour = event.event_fast_info.hour;
        cur_time.min = event.event_fast_info.minute;
        cur_time.sec = 0;

        // ������ʱ���ʱ��
        IncreaseTimeBySecond(&cur_date, &cur_time, /*ALM_DELAY_TIME_SEC*/snooze_time);
        //s_event_fast_table[event_id].is_on = TRUE;
        //SCI_MEMCPY(&s_event_fast_table[event_id], (uint8 *)&event.event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

        temp_service_id = s_event_fast_table[delay_event_id].service_id;
        SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event.event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id������Ӧ���Լ���event_id

        s_event_fast_table[delay_event_id].year = cur_date.year;
        s_event_fast_table[delay_event_id].mon = cur_date.mon;
        s_event_fast_table[delay_event_id].day = cur_date.mday;
        s_event_fast_table[delay_event_id].hour = cur_time.hour;
        s_event_fast_table[delay_event_id].minute = cur_time.min; 

        if (ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode)
        {
            s_event_fast_table[event_id].is_on = FALSE;
        }
    };
    // ����
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_event_id, &event.event_content_info);  

    /*if (ALM_DOWNLOAD_RING == event.event_content_info.ring_type
        ||ALM_FM_RING == event.event_content_info.ring_type)
    {
        SaveRingInfoToFile(delay_event_id,&event.event_ring_info);  
    } */   

    // ���÷���
    MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
    SetAlarmService(delay_event_id, TRUE);
    // �������������
    FindLatestEventToSet();

    return result;
}

/*****************************************************************************/
//  Description : ��wday��һ������ҵ���һ��ƥ��ģ�����offset
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  Note:   
/*****************************************************************************/
PUBLIC uint8 MMIAPIALM_RoundWeek(
                              uint8 wday,
                              uint8 fre,
                              uint32 type
                              )
{
    uint8 offset_day = 0;

    switch(type) 
    {
    case CMP_FIRST_SMALL:
        offset_day = RoundWeek(wday, fre);
        break;

    default:
        offset_day = RoundWeek2(wday, fre);
        break;
    }
    
    return offset_day;
}

/*****************************************************************************/
//  Description : increase current date by one
//  Global resource dependence : 
//  Author:        xiaoqinglu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_IncreaseDateByOne(
                                     SCI_DATE_T  *pdate_ptr
                                     )
{
    //SCI_ASSERT(NULL != pdate_ptr);/*assert verified*/
    if(PNULL == pdate_ptr)
    {
        return;
    }
    
    IncreaseDateByOne(pdate_ptr);
}

/*****************************************************************************/
//  Description : close delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN CloseDelayAlarm(
                              uint16 event_id, 
                              MMIACC_SMART_EVENT_FAST_TABLE_T event,
                              BOOLEAN    is_on
                              )
{
    BOOLEAN    result   = TRUE;
    uint16 delay_event_id = 0;
    MMIALMSVC_ID_T temp_service_id = 0;

   /* switch(event_id)
    {
    case EVENT_ALARM_0:
    case EVENT_ALARM_DELAY_0:
        delay_event_id = EVENT_ALARM_DELAY_0;
        event_id = EVENT_ALARM_0;
        break;

    case EVENT_ALARM_1:
    case EVENT_ALARM_DELAY_1:
        delay_event_id = EVENT_ALARM_DELAY_1;
        event_id = EVENT_ALARM_1;
        break;
        
    case EVENT_ALARM_2:
    case EVENT_ALARM_DELAY_2:
        delay_event_id = EVENT_ALARM_DELAY_2;
        event_id = EVENT_ALARM_2;
        break;
        
    case EVENT_ALARM_3:
    case EVENT_ALARM_DELAY_3:
        delay_event_id = EVENT_ALARM_DELAY_3;
        event_id = EVENT_ALARM_3;
        break;
        
    case EVENT_ALARM_4:
    case EVENT_ALARM_DELAY_4:
        delay_event_id = EVENT_ALARM_DELAY_4;
        event_id = EVENT_ALARM_4;
        break;
        
    case EVENT_ALARM_5:
    case EVENT_ALARM_DELAY_5:
        delay_event_id = EVENT_ALARM_DELAY_5;
        event_id = EVENT_ALARM_5;
        break;
        
    case EVENT_ALARM_6:
    case EVENT_ALARM_DELAY_6:
        delay_event_id = EVENT_ALARM_DELAY_6;
        event_id = EVENT_ALARM_6;
        break;
        
    case EVENT_ALARM_7:
    case EVENT_ALARM_DELAY_7:
        delay_event_id = EVENT_ALARM_DELAY_7;
        event_id = EVENT_ALARM_7;
        break;
        
    case EVENT_ALARM_8:
    case EVENT_ALARM_DELAY_8:
        delay_event_id = EVENT_ALARM_DELAY_8;
        event_id = EVENT_ALARM_8;
        break;
        
    case EVENT_ALARM_9:
    case EVENT_ALARM_DELAY_9:
        delay_event_id = EVENT_ALARM_DELAY_9;
        event_id = EVENT_ALARM_9;
        break;

    default:
        // ������һ���ճ�
        FindLatestEventToSet();
        return FALSE;
        //break;
    }*/
    // Added by sunhongzhe for snoozed calendar events.
    if (GetEventAndDelayedIds(&event_id, &delay_event_id))
    {
    }
    // End
    /*
    if(event_id<=EVENT_ALARM_MAX)
    {
        delay_event_id = event_id+EVENT_ALARM_DELAY_0;
        event_id = event_id;
    }
    else if(event_id>=EVENT_ALARM_DELAY_0&&event_id<=EVENT_ALARM_DELAY_MAX)
    {
       delay_event_id = event_id;
       event_id = event_id-EVENT_ALARM_DELAY_0; 
    }*/
    else
    {
        FindLatestEventToSet();
        temp_service_id = s_event_fast_table[event_id].service_id;
        MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
        return FALSE;
    }

    if (event.is_valid) // ��������ʱ��Źرգ�����ǹر�״̬����û�б�Ҫ��������Ĳ�����
    {
        event.is_on = is_on;
        event.is_valid = FALSE;
    
        temp_service_id = s_event_fast_table[delay_event_id].service_id;
        SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
        s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id������Ӧ���Լ���event_id
        
        if (ALM_MODE_ONCE == s_event_fast_table[event_id].fre_mode)
        {
            s_event_fast_table[event_id].is_on = FALSE;//�ر���ʱ���ӵ�ͬʱ,������ʱ��ԭ����Ҳ�ر�
        }
#if 0 //modify by shipeng.lv at sprd 2009.08.28 for cr149822
//�ر���ʱ����û��Ҫ��ԭ�������ӹر�
//����һ���Ե����ӣ���MMIALM_StartAlarm���Ѿ��ر�
//ͨ�����·�ʽ��֤:
//һ�ε����ӱ�����ر�ʱ�б��йرգ�
//һ�ε����ӱ���ʱ��ʱ���б�����ʾ����״̬��
//��ε����ӱ�������ʱ��ر�ʱ�б�����ʾ����
        if ((!is_on)&&(ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode))
        {
            s_event_fast_table[event_id].is_on = FALSE;//�ر���ʱ���ӵ�ͬʱ,������ʱ��ԭ����Ҳ�ر�
        }
#endif
        // ����
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);
        MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
        // ���÷���
        SetAlarmService(delay_event_id, TRUE);
        // �������������
        FindLatestEventToSet();
    }
    return result;
}

/*****************************************************************************/
//  Description : start alarm when interrupt of alarm is coming
//  Global resource dependence : 
//  Author:        taul.zhu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_StartAlarm(
                                 MMIALMSVC_ID_T svc_id,
                                 MMIALMSVC_EVENT_TYPE_E svc_type
                                 )
{
    BOOLEAN    result = FALSE;
    uint32     i = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T  *event_ptr = NULL;
    MMIACC_SMART_EVENT_FAST_TABLE_T        event_mod = {0};
    MMIACC_SCH_LIST_T           bypast_list = {0};
    BOOLEAN                     is_set_bypast = FALSE;

//  SCI_TRACE_LOW("MMIAPIALM_StartAlarm()");

    s_arrived_event_id = 0;
    s_arrived_event.event_fast_info.is_on = FALSE;

    ClearDelayedEvent();
	Alarm_SetModifyEventDateFlag(FALSE);
    result = MMIALM_GetArrivedEvent(&s_arrived_event, &s_arrived_event_id); //find matched event
	Alarm_SetModifyEventDateFlag(TRUE);

    if (result) //  got a arrived event
    {
        GetSameTimeSchEvent(&s_arrived_event.event_fast_info, s_arrived_event_id, EVENT_SCH_0, &s_bypast_list);
        {//������ʱ�¼��е��Զ����ػ��¼�

            for (i=0;i<s_bypast_list.total;i++)
            {
                // �ж���ʱ�¼����Ƿ���EVENT_AUTO_POWERON�¼�,
                if (EVENT_AUTO_POWERON == s_bypast_list.entry_id[i])//������������Զ�����,��������
                {
                    PHONE_STARTUP_CONDITION_E   e_startup_cond = STARTUP_NONE;

                    e_startup_cond = MMIAPIPHONE_GetStartUpCondition();

                    if(e_startup_cond == STARTUP_NORMAL)
                    {
                        DeleteOneSchEventInList(i, &s_bypast_list);
                        i--;
                    }
                }
                // �ж���ʱ�¼����Ƿ���EVENT_AUTO_POWEROFF�¼�,
                else if (EVENT_AUTO_POWEROFF == s_bypast_list.entry_id[i])
                {
                    MMIAPIALM_FormInteEventInfo(EVENT_AUTO_POWEROFF,&s_poweroff_event);
                    DeleteOneSchEventInList(i, &s_bypast_list);
                }
            }

            // For now, we have no auto power on/off alarms, and we only show one
            // arrived calendar event, others are just omitted.
            SCI_MEMSET(&s_bypast_list, 0, sizeof(MMIACC_SCH_LIST_T));
        }

        //��������������¼���ͬһʱ�䷢����ֻȡһ����ʾ
        SCI_MEMSET(((uint8*)&bypast_list), 0, sizeof(MMIACC_SCH_LIST_T));
        GetSameTimeSchEvent(&s_arrived_event.event_fast_info, s_arrived_event_id, s_arrived_event_id, &bypast_list); // ����ȡ��ͬʱ�����������¼�
        for (i = 0; i < bypast_list.total; i++) // �޸�ÿһ���¼�
        {
            event_ptr = MMIALM_GeEvent(bypast_list.entry_id[i]); // �õ��¼�������

            MMI_MEMCPY(
                    (void*)&event_mod, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T),
                    (void*)event_ptr, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T),
                    sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T)); // copy
            // һ�ε�����,�ճ�,���ػ�,ֱ�ӹر�
            if (ALM_MODE_ONCE == event_mod.fre_mode)
            {
                if (SMART_EVENT_ALARM == event_mod.type ||
                    SMART_EVENT_AUTOPOWERON == event_mod.type ||
                    SMART_EVENT_AUTOPOWEROFF == event_mod.type
 #ifdef MMI_FM_TIMER_RECORD_SUPPORT
                    || SMART_FM_TIMER_RECORD == event_mod.type
 #endif
                    )
                {
                    event_mod.is_on = FALSE;
                }
            }

            if (ModifyEvent(bypast_list.entry_id[i], &event_mod, FALSE))// ������¼�����һ�η���ʱ��,����ǵ���,��ֱ�ӹر�
            {
                is_set_bypast = TRUE;
            }
        }
        if (is_set_bypast) // ����ѭ���޸ĵ�ʱ��û���������ӣ�����ͳһ���ã������ٶȻ����
        {
            MMIALMSVC_SetAlmService();
        }

        //SCI_TRACE_LOW:"MMIAPIALM_StartAlarm, find event result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4751_112_2_18_2_0_59_32,(uint8*)"d", result);


        if ( MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
            //[alarm can response when answering call]
            //|| MMIAPICC_IsInState(CC_IN_CALL_STATE)
            || (MMIAPICC_IsInState(CC_IN_CALL_STATE)
                    && !IsAlarmNotifyInCall(s_arrived_event.event_fast_info.type))
            //[end]
 #ifdef MMI_FM_TIMER_RECORD_SUPPORT
            || ((MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef MMI_RECORD_SUPPORT
                 ||MMIAPIRECORD_IsRecording()
#endif
                 ||MMIAPIFM_IsFmRecordActive()
                 ||MMIAPIDC_IsOpened())
           && (SMART_FM_TIMER_RECORD == s_arrived_event.event_fast_info.type))
 #endif
            //|| MMIAPIUDISK_IsFormatWaitWinOpened()
            //|| MMIAPISD_IsFormatWaitWinOpened()
            ///|| MMIAPISMS_IsSendingSMS()
            /* added by plum peng for v1 cr to v2 */
           // || MMIAPISTK_IsInSTK()
//#ifdef VT_SUPPORT
//            || MMIAPIVT_IsVtCalling()
//#endif
            )
        {
            // delay processing alarm
            //SCI_TRACE_LOW:"MMIAPIALM_StartAlarm, delay alarm win pop"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4773_112_2_18_2_0_59_33,(uint8*)"");
            PushDelayedEvent();
            s_arrived_event.event_fast_info.is_on = TRUE;
            SetDelayAlarmTime(s_arrived_event_id, &s_arrived_event, FALSE);//CR192240
        }
        else if (
                (MMIAPICC_IsInState(CC_IN_CALL_STATE) 
                && (s_arrived_event.event_fast_info.type == SMART_EVENT_AUTOPOWEROFF))
                || (MMIAPIMMS_IsSendingOrRecving() 
                && (s_arrived_event.event_fast_info.type == SMART_EVENT_AUTOPOWEROFF))
        )
        {
            /*��ʱ�¼���¼��s_arrived_event��,�����ڴ�ʱ���Զ��ػ�����ʱ�ڼ�,s_arrived_event���ܻᱻ����������,
            �ճ̵��¼����,����ؽ�����ʱ���Զ��ػ��¼�������s_poweroff_event��*/
            MMIAPIALM_FormInteEventInfo(EVENT_AUTO_POWEROFF,&s_poweroff_event);
        }
#ifdef MMI_ETWS_SUPPORT
        else if(TRUE == MMIAPISMSCB_IsETWSAlert())
        {
                        ClearDelayedEvent();
                        SetDelayAlarmTime(s_arrived_event_id, &s_arrived_event, FALSE);
        }
#endif        
        else
        {
            //SCI_TRACE_LOW:"MMIAPIALM_StartAlarm, open alarm win at once"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4790_112_2_18_2_0_59_34,(uint8*)"");
            MMIALM_SetAlmInterruptFlag(TRUE);
#ifdef MMI_ETWS_SUPPORT            
            if( MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
            {
                if(MMIAPISMSCB_IsETWSAlerting())
                    MMIALM_SetAlarmVibrateDelayedFlag(TRUE);
                
                MMIALM_SetAlarmRedisplayFlag(TRUE);
                result = TRUE;
            }else
#endif            
            {
                result = OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIALM_StartAlarm, the alarm is not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4797_112_2_18_2_0_59_35,(uint8*)"");
        MMIALMSVC_SetAlmService();
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
        {
            MMIDEFAULT_TurnOffBackLight();
            POWER_PowerOff();
        }
    }

    return result;
}


/*****************************************************************************/
//  Description : open alarm expired win according to the alarm type
//  Global resource dependence : 
//  Author:      lin.lin
//  Note:            
/*****************************************************************************/
LOCAL BOOLEAN OpenAlarmExpiredWin(
                                  MMIACC_SMART_EVENT_E    type
                                  )
{
    BOOLEAN                     recode = TRUE;
    PHONE_STARTUP_CONDITION_E   e_startup_cond = STARTUP_NONE;

    e_startup_cond = MMIAPIPHONE_GetStartUpCondition();

    //SCI_TRACE_LOW:"mmialarm.c: OpenAlarmExpiredWin(), type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4823_112_2_18_2_1_0_36,(uint8*)"d", type);
    ClearDelayedEvent();

    switch(type)
    {
    case SMART_EVENT_AUTOPOWERON:
        if((e_startup_cond == STARTUP_CHARGE) ||
           (e_startup_cond == STARTUP_ALARM))
        {
            if (MMK_IsOpenWin(PHONE_STARTUP_CHARGE_WIN_ID))
            {
                MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
            }
//            MMIDEFAULT_TurnOnBackLight();

            if (MMK_IsOpenWin(MMIALM_ALIVEWIN_ID))
            {
                MMK_CloseWin(MMIALM_ALIVEWIN_ID);
            }

            OpenAlarmAliveWin(WIN_ONE_LEVEL);
            // �Զ�����ʱ����Զ��ػ�������
            SCI_MEMSET(&s_poweroff_event, 0, sizeof(MMIACC_SMART_EVENT_T));
        }
        else // ����״̬�²���Ӧ�����¼�,�ж��Ƿ�ͬʱ�����йػ��¼����ճ��¼���
        {
            CheckandStartExpiredEvent();
        }
        break;

    case SMART_EVENT_AUTOPOWEROFF:
        if ((e_startup_cond != STARTUP_CHARGE) &&
            (e_startup_cond != STARTUP_ALARM))
        {
//            MMIDEFAULT_TurnOnBackLight();

            if (MMK_IsOpenWin(MMIALM_ALIVEWIN_ID))
            {
                MMK_CloseWin(MMIALM_ALIVEWIN_ID);
            }

            OpenAlarmAliveWin(WIN_ONE_LEVEL);
        }
        else
        {
            MMIALMSVC_SetAlmService();
            if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
            {
                MMIDEFAULT_TurnOffBackLight();
                POWER_PowerOff();
            }
            recode = FALSE;
        }
        break;

    default:
#ifdef CMCC_UI_STYLE
//��ֱ�����У������յ����ӻ����ճ������쳣�¼�������Ҫ������ʾ�����ҵ��˳�ֱ��ʱ����Ҫ�������쳣�¼������������������Ϊ�ӳ��¼���
#ifdef VIDEO_PLAYER_SUPPORT
        if (MMIAPIVP_IsLiving())  //add for NEWMS00173193
        {
            SCI_TRACE_LOW("MMIALM_StartAlarm, delay alarm win pop");
            PushDelayedEvent();
            s_arrived_event.event_fast_info.is_on = TRUE;
        }
#endif
#endif
        if (MMK_IsOpenWin(MMIALM_ALIVEWIN_ID))
        {
            MMK_CloseWin(MMIALM_ALIVEWIN_ID);
        }
        if(IsNeedBackgroundPrompt())
        {
            OpenAlarmAliveWin(WIN_LOWEST_LEVEL);
        }
        //[enhance alarm expired win's priority when start
        // up charging]
        /*else if(MMK_IsFocusWin(PHONE_STARTUP_CHARGE_WIN_ID))
        {
            SCI_TRACE_LOW("OpenAlarmExpiredWin smart_event_alarm,in charging");
            OpenAlarmAliveWin(WIN_THREE_LEVEL);
        }*/
        //[end]
        else
        {
            OpenAlarmAliveWin(WIN_ONE_LEVEL);
        }
        break;
    }

    return recode;
}

/***********************************************************************************
 Name:          MMIALM_IsAlarmExpired
 Description:   if an alarm/event is waiting for occuring
 Input:     
 Return:         True if  an alarm is waiting for prompt
***********************************************************************************/
PUBLIC BOOLEAN MMIALM_IsAlarmExpired(void)
{
    return s_is_event_delayed;
}

/*****************************************************************************/
//  Description : open alarm expired win according to the alarm type
//  Global resource dependence : 
//  Author:      lin.lin
//  Note:            
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenAlarmExpiredWin(void)
{
    OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
}

/*****************************************************************************/
//  Description : check to see if there is a delayed alarm and activate it
//  Global resource dependence : 
//  Author:        taul.zhu
//  RETRUN:  TRUE:  delayed alarm is activated
//  Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_CheckAndStartDelayedAlarm(void)
{

    //SCI_TRACE_LOW:"MMIAPIALM_CheckAndStartDelayedAlarm(), is_delayed = %d, is_on = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_4924_112_2_18_2_1_0_37,(uint8*)"dd", s_is_event_delayed, s_arrived_event.event_fast_info.is_on);

#ifdef WIN32
    s_arrived_event.event_fast_info.is_on = TRUE;
#endif

    if (s_is_event_delayed && s_arrived_event.event_fast_info.is_on)        // delayed event is valid
    {
        MMK_PostMsg((MMI_WIN_ID_T)VIRTUAL_WIN_ID, MSG_SET_OPENALARM_EXPIRED_WIN, PNULL, 0);
        //OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
        s_arrived_event.event_fast_info.is_on = FALSE;
        
        return TRUE;
    }
    else if (s_poweroff_event.event_fast_info.is_valid)
    {
        MMI_MEMCPY(
            (void*)&s_arrived_event, sizeof(MMIACC_SMART_EVENT_T),
            (void*)&s_poweroff_event, sizeof(MMIACC_SMART_EVENT_T),
            sizeof(MMIACC_SMART_EVENT_T));
        s_arrived_event_id = EVENT_AUTO_POWEROFF;
        MMK_PostMsg((MMI_WIN_ID_T)VIRTUAL_WIN_ID, MSG_SET_OPENALARM_EXPIRED_WIN, PNULL, 0);
        //OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
        SCI_MEMSET(&s_poweroff_event, 0, sizeof(MMIACC_SMART_EVENT_T));
        return TRUE;        
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : PushDelayedEvent
//         push ID of current dealyed event to stack to save when a call is active
//         currently, this stack only contains one flag 
//  Global resource dependence : 
//  Author:        taul.zhu
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void PushDelayedEvent(
                            void
                            )
{
    s_is_event_delayed = TRUE;
}

/*****************************************************************************/
//  Description : ClearDelayedEvent
//  Global resource dependence : 
//  Author:        liqing.peng
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void ClearDelayedEvent(void)
{
    s_is_event_delayed = FALSE;
}

/*****************************************************************************/
//  Description : get current ring type of  two alarm: 0 and 1
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIALM_GetRingId(
                        uint16   alarm_id
                        )
{
    uint16                  nv_return = 0;
    MMIACC_SMART_EVENT_CONTENT_T event = {0};

    MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+alarm_id, &event, nv_return);

    if( MMIAPIUDISK_UdiskIsRun() && (MMISET_RING_FIXED_DEFAUT_NUM < event.ring)) //Set default ring
    {
        event.ring = /*ALM_DEFAULT_ALARM_RINGTONG*/g_clock_ring_default_id;
    }
    else if(event.ring_type == ALM_DOWNLOAD_RING
#ifdef FM_SUPPORT
        ||event.ring_type == ALM_FM_RING
#endif
        ||event.ring_type == ALM_RECORD_RING)
    {
        event.ring = MMISET_MAX_RING_ID;
    }

    return event.ring;
}

#ifdef DPHONE_SUPPORT
LOCAL MMI_RESULT_E  HandleAlarmPowerOnOffEditWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   current_ctrl_id = MMIALM_SETTIME_CTRL_ID;
    MMISET_TIME_T   time = {0};
    GUIEDIT_TIME_STYLE_E        time_style = GUIEDIT_TIME_STYLE_NULL;
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:  
        time_type = MMIAPISET_GetTimeDisplayType();
        if (MMISET_TIME_12HOURS == time_type)
        {
            time_style = GUIEDIT_TIME_STYLE_12;
        }
        else
        {
            time_style = GUIEDIT_TIME_STYLE_24;
        }
        
         //set time display style
        GUIEDIT_SetTimeStyle(current_ctrl_id,PNULL,&time_style,PNULL,FALSE);
        GUIEDIT_SetFont(current_ctrl_id, MMI_SET_TIME_FONT);
        if (s_current_event_info.event.event_fast_info.is_valid )
        {
            GUIEDIT_SetTime(current_ctrl_id,
            s_current_event_info.event.event_fast_info.hour,
            s_current_event_info.event.event_fast_info.minute,
            0);
        }
        else
        {
            SCI_TIME_T      sys_time = {0};
            TM_GetSysTime(&sys_time);
            GUIEDIT_SetTime(current_ctrl_id, sys_time.hour, sys_time.min, 0); 
        }
        MMK_SetAtvCtrl(win_id,  current_ctrl_id);

        //SCI_ASSERT(s_current_event_info.id <= EVENT_AUTO_POWEROFF);/*assert verified*/
        break;

        case MSG_FULL_PAINT:
        break;
        case MSG_CTL_OK:        // completed
        case MSG_APP_OK:       
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIEDIT_GetTime(current_ctrl_id, &time.time_hour, &time.time_minute, PNULL);
            if ( MMIAPICOM_CheckTimeIsValid(time))
            {
                s_current_event_info.event.event_fast_info.hour = time.time_hour; 
                s_current_event_info.event.event_fast_info.minute = time.time_minute;
                s_current_event_info.event.event_fast_info.is_on = TRUE;
                // �ر���ʱ����
                SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
                // ��ʾ����
                if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MEMORY_FULL);
                }
            }
            else // time invalid
            {
                MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
            }
            MMK_CloseWin(win_id);
            break;
        case MSG_CTL_CANCEL:
        case MSG_KEYDOWN_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#endif

/*****************************************************************************/
//  Description : HandleAutoPowerOnOffWinMsg
//  Global resource dependence : 
//  Author:        lin.lin
//   RETRUN: 
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOnOffWinMsg( 
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    uint16                      i                = 0;
    MMI_CTRL_ID_T               ctrl_id          = MMIALM_AUTOPOWER_LIST_CTRL_ID;
    MMI_RESULT_E                recode           = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T               text_label[]     = {TXT_AUTOPOWER_ON, TXT_AUTOPOWER_OFF};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem( ctrl_id, ALM_AUTOPOWER_NUM, FALSE );//max item 2
        //GUILIST_SetMaxSelectedItem( ctrl_id, ALM_AUTOPOWER_NUM );
        //GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_EDIT_ICON, 1, FALSE);
#endif
        
        for ( i = 0; i < ALM_AUTOPOWER_NUM; i++ )
        {
            uint16 event_id = (uint16)( EVENT_AUTO_POWERON + i );
            const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );
            
            AppendTwoLineListItem( ctrl_id,
                text_label[i],
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                FALSE );
				//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 			SetListItemStyle(ctrl_id, i);
// #endif
        }
        
        MMK_SetAtvCtrl( win_id, ctrl_id );
        GUIWIN_SetTitleTextId( win_id, TXT_AUTO_POWERONOFF, FALSE );
        break;
    
    case MSG_FULL_PAINT:
        for ( i = 0; i < ALM_AUTOPOWER_NUM; i++ )
        {
            uint16 event_id = (uint16)( EVENT_AUTO_POWERON + i );
            const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );
            
            AppendTwoLineListItem( ctrl_id,
                text_label[i],
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                TRUE );
				//cr132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 			SetListItemStyle(ctrl_id, i);
// #endif
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifndef MMI_GUI_STYLE_TYPICAL  
     case MSG_CTL_LIST_CHECK_CONTENT:
#endif
        {
            uint16 cur_idx = GUILIST_GetCurItemIndex(MMIALM_AUTOPOWER_LIST_CTRL_ID);
			BOOLEAN is_need_full_paint = TRUE;
            switch(cur_idx)
            {
            case TYPE_AUTO_POWERON:
            case TYPE_AUTO_POWEROFF:
                if(TYPE_AUTO_POWERON == cur_idx)
                {
                    s_current_event_info.id = EVENT_AUTO_POWERON;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWERON;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWERON;
                }
                else
                {
                    s_current_event_info.id = EVENT_AUTO_POWEROFF;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWEROFF;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWEROFF;                    
                }
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event);
                if (s_current_event_info.event.event_fast_info.is_on)
                {
                s_current_event_info.event.event_fast_info.is_on = FALSE;
                }
                else
                {	//CR177677:�ж���һ���Ƿ���������������ʱ���Ƿ�һ�£���һ�£�������ʾ��
					SCI_TRACE_LOW("[MMIALARM.C][HandleAutoPowerOnOffWinMsg] s_current_event_info.id = %d",s_current_event_info.id);
					if (EVENT_AUTO_POWERON == s_current_event_info.id)
					{
						MMIACC_SMART_EVENT_FAST_TABLE_T   *poweroff_event = {0};
						MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
						poweroff_event = MMIALM_GeEvent(EVENT_AUTO_POWEROFF);
						if (poweroff_event->is_on)
						{//������
							if((poweroff_event->minute == s_current_event_info.event.event_fast_info.minute)
								&&(poweroff_event->hour == s_current_event_info.event.event_fast_info.hour)
                                 && (0 != (GetFreByEditWin() & poweroff_event->fre)))
							{
								MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
								MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
							    is_need_full_paint = FALSE;
							}
						}

					}
					else if(EVENT_AUTO_POWEROFF == s_current_event_info.id)
					{
						MMIACC_SMART_EVENT_FAST_TABLE_T   *poweron_event = {0};
						MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
						poweron_event = MMIALM_GeEvent(EVENT_AUTO_POWERON);
						if (poweron_event->is_on)
						{//������
							if((poweron_event->minute == s_current_event_info.event.event_fast_info.minute)
								&&(poweron_event->hour == s_current_event_info.event.event_fast_info.hour))
							{
								MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
								MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
							    is_need_full_paint = FALSE;
							}
						}
					}

					//FALSE == s_current_event_info.event.event_fast_info.is_on && ��=
					if (is_need_full_paint)
					{
						s_current_event_info.event.event_fast_info.is_on = TRUE;
					}

                }
				if (is_need_full_paint)
				{
					// �ر���ʱ����
					SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
					// �޸���������
					MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);   
				}     
                break;

            default:
                break;
            }
        }
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        {
            uint16 cur_idx = GUILIST_GetCurItemIndex(MMIALM_AUTOPOWER_LIST_CTRL_ID);
            switch(cur_idx)
            {
            case TYPE_AUTO_POWERON:
            case TYPE_AUTO_POWEROFF:
                if(TYPE_AUTO_POWERON == cur_idx)
                {
                    s_current_event_info.id = EVENT_AUTO_POWERON;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWERON;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWERON;
                }
                else
                {
                    s_current_event_info.id = EVENT_AUTO_POWEROFF;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWEROFF;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWEROFF;                    
                }
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);  
                MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event);
#ifdef DPHONE_SUPPORT
                MMK_CreateWin((uint32*)DPHONE_ALARM_POWERONOFF_EDIT_WIN, PNULL);
#else
                MMK_CreateWin((uint32*)POWERONOFF_EDIT_WIN_TAB, PNULL);    
#endif
                break;

            default:
                break;
            }
        }
        break;            

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;    

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;

}


/*****************************************************************************/
//  Description : auto power on prompt window
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOnExpired(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
     MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    win_info_ptr->win_id = win_id;
    
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:  
          // �������ڶ�ʱ��
          StartAlarmTimer(&s_3seconds_timer_id, MMI_3SECONDS, FALSE);
          //GUIWIN_SetSoftkeyTextId(MMIALM_ALIVEWIN_ID,  STXT_SOFTKEY_OK_MK, TXT_NULL, (MMI_TEXT_ID_T)STXT_EXIT, FALSE);  
          recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
          MMIDEFAULT_TurnOnBackLight(); 
          break;

        case MSG_APP_OK:
        case MSG_CTL_OK :
            /*SCI_TRACE_LOW("mmialarm.c: HandleAutoPowerOnExpired() MSG_CTL_OK "); 
            MMK_CloseWin(win_id);
            if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
            {
               MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
            }
            MMIAPIPHONE_PowerReset();*/
#ifdef FM_SUPPORT
           MMIAPIFM_StopRecord();
#endif
            break;
                            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            //POWER_PowerOff()֮ǰ�رձ���
            /*MMIDEFAULT_TurnOffBackLight();

             POWER_PowerOff();*/
            break;
        
        case MSG_KEYLONG_RED:
            MMK_CloseWin(win_id);
#ifdef FM_SUPPORT
           MMIAPIFM_StopRecord();
#endif
            MMIAPIPHONE_PowerReset();
            break;

       case MSG_TIMER:                  
            if (s_3seconds_timer_id == *((uint8 *) param))
            {
                GUIWIN_UpdateTitle(win_id);
                MMK_CloseWin(win_id);

                if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
                {
                   MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
                }
#ifdef FM_SUPPORT
           MMIAPIFM_StopRecord();
#endif
                MMIAPIPHONE_PowerReset();
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
           break;
           
       case MSG_GET_FOCUS:
//            MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_FULL_PAINT, PNULL);
            break;

        case MSG_FULL_PAINT:
            //recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            //initLcdBavkground();
            DisplayAlarmWin(win_id,win_info_ptr);
            break;

        default:
            if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_WEB ) ||
                 (MSG_KEYUP_FLIP == msg_id ))//|| MSG_CTL_OK == msg_id))
            {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.            
                if (MSG_KEYUP_FLIP == msg_id)
                {
                    MMIDEFAULT_HandleFlipKey(TRUE);  
                }
#endif                
            }else
            {
                recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            }
            break;
    }    

    return recode;
}

/*****************************************************************************/
//  Description : auto power off prompt window
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoPowerOffExpired( 
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
     MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
     static MMIACC_SMART_EVENT_T *poweroff=PNULL;

     MMI_STRING_T        tip_str   = {0};
    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    win_info_ptr->win_id = win_id;

    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:
        if (0 == s_30seconds_count)
        {
            s_30seconds_count = 30;
        }
        StartAlarmTimer(&s_1seconds_timer_id, MMI_1SECONDS, TRUE);
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);        
		poweroff=SCI_ALLOC_APP(sizeof(MMIACC_SMART_EVENT_T));
		if(PNULL!=poweroff)
		{
            MMI_MEMCPY(
            (void*)poweroff, sizeof(MMIACC_SMART_EVENT_T),
            (void*)&s_arrived_event, sizeof(MMIACC_SMART_EVENT_T),
            sizeof(MMIACC_SMART_EVENT_T));
        }
        //MMIAPIPB_InterruptPB();
        //MMIAPIPICVIEW_StopDeleteFiles();
        MMIPUB_SetWinSoftkey(MMIALM_ALIVEWIN_ID, TXT_COMMON_OK, STXT_CANCEL, FALSE);
        break;

    case MSG_GET_FOCUS:
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
       // StartAlarmTimer(&s_1seconds_timer_id, MMI_1SECONDS, TRUE);
        break;
        
    case MSG_LOSE_FOCUS:
       /* if(0 != s_1seconds_timer_id)
        {
            MMK_StopTimer(s_1seconds_timer_id);
            s_1seconds_timer_id = 0;
        }*/
        break;

    case MSG_TIMER:
        if (s_30seconds_timer_id == *((uint8 *) param))
        {
            if(MMK_IsFocusWin(win_id))
            {
#ifdef FM_SUPPORT
           	   MMIAPIFM_StopRecord();
#endif      
                SCI_TRACE_LOW("PowerOff by HandleAutoPowerOffExpired, Line 7464");
                GUIWIN_UpdateTitle(win_id);
                MMK_ReturnIdleWin();
                MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
                MMIAPIPHONE_PowerOffEx();
            }
            else
            {
                if(0 != s_1seconds_timer_id)
                {
                    MMK_StopTimer(s_1seconds_timer_id);
                    s_1seconds_timer_id = 0;
                }
                MMI_GetLabelTextByLang(TXT_POWER_OFF, &tip_str);
                MMIPUB_UpdateWaitWinText(TRUE, 1, &tip_str,PNULL,PNULL, MMIALM_ALIVEWIN_ID);
                MMK_WinGrabFocus(win_id);
            }
        }
        else if (s_1seconds_timer_id == *((uint8 *) param))
        {
            uint8               second_num   = 0;
            uint8               num_str[32]  = {0};
            uint8               char_num_len = 0;
            //uint8             ucs2_num[32] = {0};
            uint8               line_num     = 0;
            char                time_str[MMIALM_TIME_STR_12HOURS_LEN+1]  = {0};  //10:34
            wchar               *tmp_str2    = PNULL;
            SCI_TIME_T          past_time    = {0};
            MMI_STRING_T        tmp_str_t1   = {0};
            MMI_STRING_T        tmp_str_t2   = {0};
            MMI_STRING_T        alarm_str[5] = {0};
          
            s_30seconds_count--;
            second_num = s_30seconds_count;
            
            past_time.hour = s_arrived_event.event_fast_info.hour;
            past_time.min = s_arrived_event.event_fast_info.minute;
            
            //��Ӧ�Ķ�Ϊ��ǰʱ�䣬��Ӧ��ȥ��ȡNV��ʱ��
			MMIAPISET_FormatTimeStrByTime(past_time.hour,past_time.min,(uint8*)time_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
            alarm_str[0].wstr_len = strlen(time_str);
            alarm_str[0].wstr_ptr = SCI_ALLOC_APP((alarm_str[0].wstr_len+1)*sizeof(wchar));
            SCI_MEMSET(alarm_str[0].wstr_ptr,0,((alarm_str[0].wstr_len + 1) * sizeof(wchar)));
            MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

            MMI_GetLabelTextByLang(TXT_ALARM_WILL_TO, &tmp_str_t1); 
            MMI_GetLabelTextByLang(TXT_ALARM_SHUT_DOWN, &tmp_str_t2);
            
            {
                MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
                MMIAPISET_GetLanguageType(&lang_type);

                if( MMISET_LANGUAGE_SIMP_CHINESE == lang_type
                          || MMISET_LANGUAGE_TRAD_CHINESE == lang_type )
                {

                    sprintf((char*)num_str, "%02d", (int)second_num);
                }
                else
                {
                    sprintf((char*)num_str, " %02d", (int)second_num);//��������ӿո�
                }
            }

            char_num_len = strlen((char*)num_str);
            
                
            alarm_str[2].wstr_len = tmp_str_t1.wstr_len + tmp_str_t2.wstr_len + char_num_len;
            tmp_str2 = SCI_ALLOC_APP( ( alarm_str[2].wstr_len + 1 ) * sizeof(wchar) );
            if(PNULL != tmp_str2)
            {
                SCI_MEMSET(tmp_str2, 0, (( alarm_str[2].wstr_len + 1 ) * sizeof(wchar)) );

                MMI_WSTRNCPY( tmp_str2, tmp_str_t1.wstr_len, tmp_str_t1.wstr_ptr, tmp_str_t1.wstr_len, tmp_str_t1.wstr_len );
                MMI_STRNTOWSTR( tmp_str2+tmp_str_t1.wstr_len, char_num_len, (char*)num_str, char_num_len, char_num_len );/*lint !e64*/
                MMI_WSTRNCPY( tmp_str2 + tmp_str_t1.wstr_len + char_num_len, tmp_str_t2.wstr_len,
                    tmp_str_t2.wstr_ptr, tmp_str_t2.wstr_len, tmp_str_t2.wstr_len );

                alarm_str[2].wstr_ptr = tmp_str2;
                line_num = 3;

                MMIPUB_UpdateWaitWinText(TRUE, line_num, &alarm_str[0],PNULL,&alarm_str[2], MMIALM_ALIVEWIN_ID);
            }
            GUIWIN_UpdateTitle(win_id);
            if (0 == s_30seconds_count)
            {
                if(MMK_IsFocusWin(win_id))
                {
#ifdef FM_SUPPORT
                    MMIAPIFM_StopRecord();
#endif
                    SCI_TRACE_LOW("PowerOff by HandleAutoPowerOffExpired, Line 7555");
                    MMIAPIMMS_ExitMMS();
                    MMK_ReturnIdleWin();
                    MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
                    MMIAPIPHONE_PowerOffEx();
                }
                else
                {
                    if(0 != s_1seconds_timer_id)
                    {
                        MMK_StopTimer(s_1seconds_timer_id);
                        s_1seconds_timer_id = 0;
                    }
                    MMI_GetLabelTextByLang(TXT_POWER_OFF, &tip_str);
                    MMIPUB_UpdateWaitWinText(TRUE, 1, &tip_str,PNULL,PNULL, MMIALM_ALIVEWIN_ID);
                    MMK_WinGrabFocus(win_id);
                }
            }
            
            if(PNULL != tmp_str2)
            {
                SCI_FREE(tmp_str2);
                tmp_str2 = PNULL;
            }
 
            if (PNULL != alarm_str[0].wstr_ptr)
            {
                SCI_FREE(alarm_str[0].wstr_ptr);
                alarm_str[0].wstr_ptr= PNULL;
            }
        }
        else
        {
             recode = MMI_RESULT_FALSE;
        }
        break;


    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef FM_SUPPORT
           MMIAPIFM_StopRecord();
#endif
        s_30seconds_count=0;
        //SCI_TRACE_LOW:"mmialarm.c: HandleAutoPowerOffExpired() MSG_CTL_OK "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_5540_112_2_18_2_1_1_38,(uint8*)"");
        MMK_ReturnIdleWin();
        MMIAPIPHONE_PowerOff();
        break;

	case MSG_KEYDOWN_RED:        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_KEYUP_POWER:
        s_30seconds_count=0;
        if(s_bypast_list.total > 0)
        {
            OpenPastSchEventWin(WIN_ONE_LEVEL, s_bypast_list.entry_id[0]);
            DeleteOneSchEventInList(0, &s_bypast_list);
        }
        else
        {
            MMIAPICT_CheckandStartDelayedCoutimeEvent();//zhaohui,counted time
        }
        MMK_CloseWin(win_id);
        if(MSG_KEYUP_POWER==msg_id)
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
                  
    case MSG_FULL_PAINT:
       // recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
       DisplayAlarmWin(win_id,win_info_ptr);
        break;            

    case MSG_CLOSE_WINDOW:
        if(0 != s_1seconds_timer_id)
        {
            MMK_StopTimer(s_1seconds_timer_id);
            s_1seconds_timer_id = 0;
        }
        if(0 != s_30seconds_count)//��;�������¼����
        {
            if(PNULL!=poweroff)
            {
                MMI_MEMCPY(
                (void*)&s_poweroff_event, sizeof(MMIACC_SMART_EVENT_T),
                (void*)poweroff, sizeof(MMIACC_SMART_EVENT_T),
                sizeof(MMIACC_SMART_EVENT_T));
            }
            else
            {
                s_30seconds_count=0;
            }
        }
        if(PNULL!=poweroff)
        {
            SCI_FREE(poweroff);
			poweroff=PNULL;
		}
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);  
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

    default:
        if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_WEB ) ||
             (MSG_KEYUP_FLIP == msg_id ))//|| MSG_CTL_OK == msg_id))
        {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
            if (MSG_KEYUP_FLIP == msg_id)
            {
                MMIDEFAULT_HandleFlipKey(TRUE);  
            }
#endif            
        }else
        {
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        }
        break;
    }    

    return recode;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : select alarm ring
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmRingWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIALM_RINGLIST_CTRL_ID;
    uint32                  item_id = 0;
    MULTIM_SELECT_RETURN_T  *win_param = NULL;
    MMIFMM_SELECT_INFO_T    select_info = {{MMIFMM_MUSIC_ALL,0},0,0,PNULL,0,-1};
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    GUIMENU_BUTTON_INFO_T    button_info={0};
    GUIMENU_TITLE_INFO_T      title_info={0};
    uint32 group_id=MENU_ALARM_SETTING_RING;
    MMI_STRING_T title={0};
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		//if alarm ring is download ring, check whether file exists
        if((ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type || ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type) 
            &&!MMIAPIFMM_IsFileExist(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#ifdef FM_SUPPORT
        else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type &&
           !MMIAPIALM_IsValidFM(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#endif
        button_info.is_static=TRUE;
        button_info.group_id=group_id;
        button_info.button_style=GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id,&button_info);

        title_info.group_id=group_id;
        title_info.is_static=TRUE;
        MMI_GetLabelTextByLang(TXT_RING,&title);
        title_info.title_ptr=&title;
        GUIMENU_SetMenuTitleEx(ctrl_id,&title_info);
            
        if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type)
        {
            pop_item_info.menu_id=ID_ALARM_RING_DOWNLOAD;
        }
#ifdef FM_SUPPORT
        else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type)
        {

            pop_item_info.menu_id=ID_ALARM_RING_FM;
        }
#endif
        else if(ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type)
        {

            pop_item_info.menu_id=ID_ALARM_RING_RECORD;
        }
        else
        {
             pop_item_info.menu_id=ID_ALARM_RING_FIX;
        }
        //set pop-up menu status
        pop_item_info.is_static = TRUE;
        pop_item_info.ctrl_id   = ctrl_id;
        pop_item_info.group_id  = group_id;
        GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &item_id);
        switch(item_id)
        {
        case ID_ALARM_RING_FIX:
            MMK_CreatePubListWin((uint32 *)ALARM_FIXEDRINGWIN_TAB, PNULL);  
            break;

        case ID_ALARM_RING_DOWNLOAD:
            select_info.ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);

#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif

            if (0 == select_info.ring_vol)
            {
                select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
            }
            if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type)
            {
                select_info.ticked_file_wstr = s_current_event_info.event.event_ring_info.dring_name_arr;
                select_info.ticked_file_len= s_current_event_info.event.event_ring_info.dring_name_len;
                select_info.win_id = win_id;  
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len= 0;
                select_info.win_id = win_id;
            }
           if (!MMIAPIFMM_OpenSelectMusicWin(&select_info))
            {
                pop_item_info.menu_id=ID_ALARM_RING_FIX;
                pop_item_info.is_static = TRUE;
                pop_item_info.ctrl_id   = ctrl_id;
                pop_item_info.group_id  = group_id;
                GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
                limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
            break;
        case ID_ALARM_RING_FM:
            {
                if (MMIAPIUDISK_UdiskIsRun()) //U��ʹ����
                {
                    pop_item_info.menu_id=ID_ALARM_RING_FIX;
                    pop_item_info.is_static = TRUE;
                    pop_item_info.ctrl_id   = ctrl_id;
                    pop_item_info.group_id  = group_id;
                    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
                else
                {
                    MMK_CreateWin((uint32*)ALARM_FM_EDITWIN_TAB, PNULL); 
                }
            }
            break;
#ifdef MMI_RECORD_SUPPORT
         case ID_ALARM_RING_RECORD:
            {
                RecordAlarmRingCallBack();
            }
            break;
#endif
        default:
            break;
        }
        break;

    case MSG_MULTIM_SELECTED_RETURN:
        win_param = (MULTIM_SELECT_RETURN_T *)param;

        s_current_event_info.event.event_content_info.ring_type = ALM_DOWNLOAD_RING;
        s_current_event_info.event.event_ring_info.dring_name_len = win_param->file_name_len;

        SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
        MMI_WSTRNCPY(s_current_event_info.event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                   win_param->file_name_ptr, win_param->file_name_len, win_param->file_name_len);

        s_current_event_info.event.event_content_info.ring = MMISET_MAX_RING_ID;
//        CloseDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
//        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event);
        //lizhen:�˴�����Ҫ����"�����"
//        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(win_id);//@zhaohui,108296
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;     
}
#else
/*****************************************************************************/
//  Description : select alarm ring
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmRingWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIALM_RINGLIST_CTRL_ID;
    uint16                  item_id = 0;
    MULTIM_SELECT_RETURN_T  *win_param = NULL;
    MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, ALM_RING_TYPE_MAX, FALSE);
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
        GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
#endif
        //����
        MMIAPISET_AppendListItemByTextId(TXT_RING_FIXED,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        MMIAPISET_AppendListItemByTextId(TXT_COMMON_MORE_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#if 0//def FM_SUPPORT//delete by James li
        MMIAPISET_AppendListItemByTextId(TXT_ALARM_FM_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
//@leon.wang modify for QQVGA(128x160)
#ifdef MMI_RECORD_SUPPORT
        MMIAPISET_AppendListItemByTextId(TXT_ALARM_RECORD_RING_TYPE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
        //if alarm ring is download ring, check whether file exists
        if((ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type || ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type)
            &&!MMIAPIFMM_IsFileExist(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#ifdef FM_SUPPORT
        else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type &&
           !MMIAPIALM_IsValidFM(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#endif

        item_id = (uint16)s_current_event_info.event.event_content_info.ring_type;
        if(ALM_RING_TYPE_MAX<=item_id)
        {
            item_id = 0;
        }
        GUILIST_SetCurItemIndex(ctrl_id, item_id);
        GUILIST_SetSelectedItem(ctrl_id, item_id, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        item_id = GUILIST_GetCurItemIndex(ctrl_id);
        switch( item_id)
        {
        case ALM_FIX_RING:
            MMK_CreateWin((uint32 *)ALARM_FIXEDRINGWIN_TAB, PNULL); 
            break;

        case ALM_DOWNLOAD_RING:
            select_info.ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);

#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif

            if (0 == select_info.ring_vol)
            {
                select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
            }
            if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type)
            {
                select_info.ticked_file_wstr = s_current_event_info.event.event_ring_info.dring_name_arr;
                select_info.ticked_file_len= s_current_event_info.event.event_ring_info.dring_name_len;
                select_info.win_id = win_id;  
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len= 0;
                select_info.win_id = win_id;
            }
           if (!MMIAPIFMM_OpenSelectMusicWin(&select_info))
            {
                GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
                GUILIST_SetSelectedItem(ctrl_id, 1, FALSE);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
                limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
            break;
#ifdef FM_SUPPORT
            case ALM_FM_RING:
            {
				if (MMIAPIUDISK_UdiskIsRun()) //U��ʹ����
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
				else
				{
                    MMK_CreateWin((uint32*)ALARM_FM_EDITWIN_TAB, (ADD_DATA)&(s_current_event_info.event));
				}
            }
            break; 
#endif

#ifdef MMI_RECORD_SUPPORT
        case ALM_RECORD_RING:
            {
                RecordAlarmRingCallBack(); 
            }
            break;
#endif
        default:
            break;
        }
        break;

    case MSG_MULTIM_SELECTED_RETURN:
        win_param = (MULTIM_SELECT_RETURN_T *)param;

        s_current_event_info.event.event_content_info.ring_type = ALM_DOWNLOAD_RING;
        s_current_event_info.event.event_ring_info.dring_name_len = win_param->file_name_len;

        SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
        MMI_WSTRNCPY(s_current_event_info.event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                   win_param->file_name_ptr, win_param->file_name_len, win_param->file_name_len);

        s_current_event_info.event.event_content_info.ring = MMISET_MAX_RING_ID;
//        CloseDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
//        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event);
        //lizhen:�˴�����Ҫ����"�����"
//        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(win_id);//@zhaohui,108296
        break;

    case MSG_GET_FOCUS:
        item_id = (uint16)s_current_event_info.event.event_content_info.ring_type;
        if(ALM_RING_TYPE_MAX<=item_id)
        {
            item_id = 0;
        }
        GUILIST_SetCurItemIndex(ctrl_id, item_id);
        GUILIST_SetSelectedItem(ctrl_id, item_id, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;     
}

#endif
/*****************************************************************************/
//  Description : select fixed ring
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmFixedRingWinMsg(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIALM_FIXEDRINGLIST_CTRL_ID;
//    uint16          i = 0;
    uint16          cur_select = 0;
    uint16          ring_id = 0;
    uint8           ring_vol=0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISRVAUD_ReqVirtualHandle("FIX ALARM SELECT RING", MMISRVAUD_PRI_NORMAL);
#ifdef DPHONE_SUPPORT
        //if alarm ring is download ring, check whether file exists
        if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type &&
           !MMIAPIFMM_IsFileExist(s_current_event_info.event.event_ring_info.dring_name_arr,
                                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#ifdef FM_SUPPORT
        else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type &&
        !MMIAPIALM_IsValidFM(s_current_event_info.event.event_ring_info.dring_name_arr,
                        s_current_event_info.event.event_ring_info.dring_name_len))
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            s_current_event_info.event.event_ring_info.dring_name_len = 0;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        }
#endif
#endif
        GUILIST_SetMaxItem(ctrl_id,MMISET_RING_FIXED_DEFAUT_NUM, FALSE );//max item 8
        MMIAPISET_AppendFixedItems(
                                    TXT_RING,
                                    MMISET_RING_FIXED_DEFAUT_NUM,
                                    TXT_COMMON_OK,
                                    TXT_NULL,
                                    TXT_COMMON_COMMON_BACK,
                                    ctrl_id,
                                    GUIITEM_STYLE_ONE_LINE_RADIO
                                    );
        //for(i=0; i<MMISET_RING_FIXED_DEFAUT_NUM; i++)
        //{
            //GUILIST_ChangeIconId(ctrl_id,i,IMAGE_SELECT_TICK_OFF_PIC,IMAGE_NULL);
        //}
        
        if(ALM_FIX_RING == s_current_event_info.event.event_content_info.ring_type)
        {
            ring_id = s_current_event_info.event.event_content_info.ring;
            cur_select = ring_id-1;
            //GUILIST_ChangeIconId(ctrl_id,cur_select,IMAGE_SELECT_TICK_ON_PIC,IMAGE_NULL);
        }
        else
        {
            ring_id = 1;
            cur_select = 0;
        }

        GUILIST_SetCurItemIndex(ctrl_id, cur_select);
        GUILIST_SetSelectedItem(ctrl_id, cur_select, TRUE);
        ////GUILIST_UpdateListBox(ctrl_id);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
        if (0 == ring_vol)
        {
            ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMIALM_FIXEDRINGLIST_CTRL_ID:
                MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
                cur_select = GUILIST_GetCurItemIndex(ctrl_id);

                ring_id = cur_select+1;
                ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
                if (0 == ring_vol)
                {
                    ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
                }
                MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break; 
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break; 
            default:
                break;
            }

        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP: 
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        cur_select = GUILIST_GetCurItemIndex(ctrl_id);

        ring_id = cur_select+1;
        ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
        if (0 == ring_vol)
        {
            ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
        break;
#endif

    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:  
        if ( MMK_IsSingleClick(msg_id, param))
        {
            break;
        }
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);

        cur_select = GUILIST_GetCurItemIndex(ctrl_id);
        ring_id = cur_select +1;
        s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
        s_current_event_info.event.event_content_info.ring = ring_id;
        //MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event);

//@zhaohui,cr108296,lizhen:�˴�����Ҫ����"�����"
//      MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

#ifdef DPHONE_SUPPORT
#else
        MMK_CloseWin( MMIALM_RINGLIST_WIN_ID ); 
#endif        
        MMK_CloseWin( win_id );
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMISRVAUD_FreeVirtualHandle("FIX ALARM SELECT RING"); 
        break;        

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode; 
}

//#else


/*****************************************************************************/
//  Description : Open Alarm window 
//  Global resource dependence : 
//  Author:        lin.lin
//  Note:   
/*****************************************************************************/
LOCAL void OpenAlarmAliveWin(
                             MMI_WIN_PRIORITY_E win_priority
                             )
{
    uint8               line_num     = 0;
    uint8               second_num   = 30;
    uint8               char_num_len = 0;
    uint8               num_str[32]  = {0};
    //uint8             ucs2_num[32] = {0};
    //uint8               *tmp_str  = PNULL;
    wchar               *tmp_str2    = PNULL;
    uint8               date_str[MMISET_DATE_STR_LEN] = {0}; //2005.06.30
    char                time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]  = {0};  //10:34
    char                date_unicode_str[11*2+2] = {0}; //2005.06.30
    char                time_unicode_str[MMIALM_TIME_STR_12HOURS_LEN*2 + 2]  = {0};  //10:34
//    uint16              event_id     = 0;
    SCI_DATE_T          sys_date     = {0};
    SCI_TIME_T          sys_time     = {0};
    MMI_STRING_T        alarm_str[5] = {0};
    MMI_STRING_T        tmp_str_t1   = {0};
    MMI_STRING_T        tmp_str_t2   = {0};
    MMI_IMAGE_ID_T      icon         = IMAGE_NULL;
    MMIPUB_HANDLE_FUNC  func         = NULL;
    wchar                   wstr_buf[30]        = {0};
    wchar                   wstr_split[]        = {L", "};
    MMI_TEXT_ID_T           txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10,};

   /* MMI_TEXT_ID_T           txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                                TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                                TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};*///CR196314
    //get current system date
    TM_GetSysDate(&sys_date);

    //get current system time
    TM_GetSysTime(&sys_time);
#ifdef SCREENSAVER_SUPPORT
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif
    if(MMK_IsOpenWin(PHONE_STARTUP_ALARM_WIN_ID))
    {
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
    }

    sys_date.mday=s_arrived_event.event_fast_info.day;
    sys_date.mon=s_arrived_event.event_fast_info.mon;
    sys_date.year=s_arrived_event.event_fast_info.year;
    sys_time.hour=s_arrived_event.event_fast_info.hour;
    sys_time.min=s_arrived_event.event_fast_info.minute;

    MMIAPISET_FormatDateStrByDateStyle(sys_date.year, sys_date.mon, sys_date.mday,'.',date_str,MMISET_DATE_STR_LEN);

    MMIAPISET_FormatTimeStrByTime(sys_time.hour,sys_time.min,(uint8*)time_str,MMIALM_TIME_STR_12HOURS_LEN + 1);

    if (SMART_EVENT_ALARM ==s_arrived_event.event_fast_info.type) // (0  ==s_arrived_event.length)    // assigne default text                
    {
        alarm_str[0].wstr_len = strlen(time_str);
        //alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = (wchar*) time_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        if (0 == s_arrived_event.event_content_info.w_length)
        {
            if (s_arrived_event_id <= EVENT_ALARM_MAX)//&&s_arrived_event_id>=EVENT_ALARM_0 /*lint !e685 !e568*/
            {
                MMI_GetLabelTextByLang(txt_alarm_id[s_arrived_event_id],&alarm_str[1]);
            }
            else if(s_arrived_event_id>=EVENT_ALARM_DELAY_0&&s_arrived_event_id <= EVENT_ALARM_DELAY_MAX)
            {
                MMI_GetLabelTextByLang(txt_alarm_id[s_arrived_event_id-EVENT_ALARM_DELAY_0],&alarm_str[1]);//CR196314
            }

        }
        else
        {
            alarm_str[1].wstr_len = s_arrived_event.event_content_info.w_length;                 
            alarm_str[1].wstr_ptr = s_arrived_event.event_content_info.w_content;  
        }
        line_num = 2;
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
        MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_ALARM);
#endif
    }
    else if ( SMART_EVENT_AUTOPOWERON == s_arrived_event.event_fast_info.type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
        //alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = (wchar*)time_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        MMI_GetLabelTextByLang(TXT_AUTOPOWER_ON, &alarm_str[1]); 
        line_num = 2;
    }
    else if ( SMART_EVENT_AUTOPOWEROFF == s_arrived_event.event_fast_info.type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
        //alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = (wchar*)time_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        MMI_GetLabelTextByLang(TXT_ALARM_WILL_TO, &tmp_str_t1); 
        MMI_GetLabelTextByLang(TXT_ALARM_SHUT_DOWN, &tmp_str_t2);

        {
            MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
            MMIAPISET_GetLanguageType(&lang_type);
            if (0 != s_30seconds_count)
            {
                second_num = s_30seconds_count;
            }

            if( MMISET_LANGUAGE_SIMP_CHINESE == lang_type
                      || MMISET_LANGUAGE_TRAD_CHINESE == lang_type )
            {

                sprintf((char*)num_str, "%02d", (int)second_num);
            }
            else
            {
                sprintf((char*)num_str, " %02d", (int)second_num);//��������ӿո�
            }
        }

        char_num_len = strlen((char*)num_str);

        alarm_str[2].wstr_len = tmp_str_t1.wstr_len + tmp_str_t2.wstr_len + char_num_len;
        tmp_str2 = SCI_ALLOC_APP( ( alarm_str[2].wstr_len + 1 ) * sizeof(wchar) );
        if(PNULL != tmp_str2)
        {
            SCI_MEMSET( tmp_str2, 0, (( alarm_str[2].wstr_len + 1 ) * sizeof(wchar)) );

            MMI_WSTRNCPY( tmp_str2, tmp_str_t1.wstr_len, tmp_str_t1.wstr_ptr, tmp_str_t1.wstr_len, tmp_str_t1.wstr_len );
            MMI_STRNTOWSTR( tmp_str2+tmp_str_t1.wstr_len, char_num_len, (char*)num_str, char_num_len, char_num_len );/*lint !e64*/
            MMI_WSTRNCPY(tmp_str2 + tmp_str_t1.wstr_len + char_num_len, tmp_str_t2.wstr_len, 
                tmp_str_t2.wstr_ptr, tmp_str_t2.wstr_len, tmp_str_t2.wstr_len );
            alarm_str[2].wstr_ptr = tmp_str2;
        }

        line_num = 3;
    } 
#ifdef MMI_AZAN_SUPPORT
    else if (SMART_EVENT_AZAN == s_arrived_event.event_fast_info.type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
        alarm_str[0].wstr_ptr = (wchar*)time_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr, 0, ((alarm_str[0].wstr_len + 1) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/
        MMI_GetLabelTextByLang(STR_AZAN_ALARM, &alarm_str[1]); 
        line_num = 2;
    }
#endif /*MMI_AZAN_SUPPORT*/ 
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    else if(SMART_FM_TIMER_RECORD == s_arrived_event.event_fast_info.type)
    {
        //do nothing;
    }
#endif
    else if (SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
    {
        //�ճ�
#if 0 //UNISOC_MMI_Delete
        alarm_str[0].wstr_len = strlen((char*)date_str);
        //alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = (wchar*)date_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,date_str,strlen(date_str),strlen(date_str));/*lint !e64*/

        alarm_str[1].wstr_len = strlen(time_str);
        //tmp_str2 = SCI_ALLOC_APP(( alarm_str[1].wstr_len + 1 ) * sizeof(wchar) );
        //MMI_STRNTOWSTR(tmp_str2,alarm_str[1].wstr_len,time_str,strlen(time_str),strlen(time_str));
        alarm_str[1].wstr_ptr = (wchar*)time_unicode_str;
        SCI_MEMSET(alarm_str[1].wstr_ptr,0,(( alarm_str[1].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[1].wstr_ptr,alarm_str[1].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/
        alarm_str[2].wstr_len = s_arrived_event.event_content_info.w_length;                 
        alarm_str[2].wstr_ptr = s_arrived_event.event_content_info.w_content;   
        line_num = 3;
#else //UNISOC_MMI_Delete

        size_t                        time_len = strlen(time_str);
        size_t                        date_len = strlen(date_str);

        alarm_str[0].wstr_ptr = wstr_buf;

		MMIAPICOM_StrToWstr(time_str, time_unicode_str);
        MMIAPICOM_StrToWstr(date_str, date_unicode_str);


		MMIAPICOM_Wstrcpy(alarm_str[0].wstr_ptr, (const wchar*)time_unicode_str);
        MMIAPICOM_Wstrncat(alarm_str[0].wstr_ptr, wstr_split,MMIAPICOM_Wstrlen(wstr_split));
        MMIAPICOM_Wstrncat(alarm_str[0].wstr_ptr, date_unicode_str, date_len);


        alarm_str[0].wstr_len = MMIAPICOM_Wstrlen(alarm_str[0].wstr_ptr);


        if(0 == s_arrived_event.event_content_info.w_length)
        {
            MMI_GetLabelTextByLang(STR_CALE_EVENT_DEFAULT_EXT02,&alarm_str[1]);
        }
        else
        {
            alarm_str[1].wstr_len = s_arrived_event.event_content_info.w_length;
            alarm_str[1].wstr_ptr = s_arrived_event.event_content_info.w_content;
        }

        line_num = 2;
#endif //UNISOC_MMI_Delete

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
        MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_SCHEDULE);
#endif
    }

    if (SMART_EVENT_ALARM ==s_arrived_event.event_fast_info.type        
#ifdef MMI_AZAN_SUPPORT
        ||(SMART_EVENT_AZAN == s_arrived_event.event_fast_info.type)
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
        ||(SMART_FM_TIMER_RECORD == s_arrived_event.event_fast_info.type)
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
        || (SMART_EVENT_SOGOU_UPDATE == s_arrived_event.event_fast_info.type)
#endif
    )
    {
        icon = IMAGE_IDLE_SCREEN_ALARM_ICON_LARGE;
    }
else if(SMART_EVENT_AUTOPOWERON == s_arrived_event.event_fast_info.type ||
        SMART_EVENT_AUTOPOWEROFF == s_arrived_event.event_fast_info.type)
{
icon = IMAGE_IDLE_SCREEN_ALARM;
}
    else if (SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
    {
        icon = IMAGE_CLOCK_CALENDAR_ALERT;
    }
    else
    {
        //SCI_PASSERT(0, ("OpenAlarmAliveWin: Invalid s_arrived_event.type is %d",s_arrived_event.event_fast_info.type)); /*assert verified*/ 
        //SCI_TRACE_LOW:"OpenAlarmAliveWin error: Invalid s_arrived_event.type  is %d:"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6419_112_2_18_2_1_3_39,(uint8*)"d",s_arrived_event.event_fast_info.type);
        if (PNULL != tmp_str2)
        {
            SCI_FREE(tmp_str2);
            tmp_str2 = PNULL;
        }
        return;
    }
	/*After open alarm dialog window, turn on backlight with alarm event.*/
	if (s_arrived_event.event_fast_info.type != SMART_EVENT_ALARM)
	{
    	MMIDEFAULT_TurnOnBackLight();//�޸�����ο�CR122782��comments
	}
    if (SMART_EVENT_AUTOPOWERON == s_arrived_event.event_fast_info.type)
    {
        //initLcdBavkground();
        func = (MMIPUB_HANDLE_FUNC )HandleAutoPowerOnExpired;
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_NONE,func);   
    }
    else if (SMART_EVENT_AUTOPOWEROFF == s_arrived_event.event_fast_info.type)
    {
        func = (MMIPUB_HANDLE_FUNC )HandleAutoPowerOffExpired;
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],PNULL,&alarm_str[2],
            MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_OKCANCEL,func);  
        /*if( SMART_EVENT_AUTOPOWERON == s_arrived_event.event_fast_info.type)
        {
            GUIWIN_CreateTitleDyna(MMIALM_ALIVEWIN_ID, TXT_AUTOPOWER_ON);
        }
        else if( SMART_EVENT_AUTOPOWEROFF == s_arrived_event.event_fast_info.type)
        {
            GUIWIN_CreateTitleDyna(MMIALM_ALIVEWIN_ID, TXT_AUTOPOWER_OFF);
        }*/
    }
    else if (SMART_EVENT_ALARM == s_arrived_event.event_fast_info.type)
    {
#if defined MMI_ALARM_JUMP_CLOCK_SUPPORT && defined MMI_PDA_SUPPORT
        if(IsNeedBackgroundPrompt()
            #if defined JAVA_SUPPORT
            || MMIAPIJAVA_IsJavaRuning()
            #endif
            #if defined BROWSER_SUPPORT
            || MMIAPIBROWSER_IsRunning()
            #endif
            #if defined WRE_SUPPORT
            || MMIWRE_Actived()
            #endif
            )/*lint !e774*/
        {
            func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
            MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2], 
              MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_CUSTOMER, func); 
        }
        else
        {
            MMK_CreateWin((uint32*)MMIALM_JUMP_CLOCK_TAB, PNULL);
        }
#else
#ifdef MMI_GUI_STYLE_MINISCREEN
        if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
        {
            MMIAPIPHONE_SetStartUpCondition(STARTUP_ALARM);
        }
#endif
        //[add interface for judge if alarm win is
        // on foreground]
        SetAlarmNotifyMark(win_priority != WIN_LOWEST_LEVEL);
        //[end]
        func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
        //MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
        //  MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_CUSTOMER, func);

#ifdef MMI_ETWS_SUPPORT
        if(TRUE == MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
        {
            // Copy alarm str1, alarm str2, icon, line num, priority
            if( PNULL != s_alarm_str1.wstr_ptr )
            {
                SCI_FREE(s_alarm_str1.wstr_ptr);
            }
            s_alarm_str1.wstr_ptr = SCI_ALLOCAZ(alarm_str[1].wstr_len * sizeof(wchar));
            SCI_MEMSET(s_alarm_str1.wstr_ptr , 0, alarm_str[1].wstr_len* sizeof(wchar));
            
            MMIAPICOM_Wstrncpy(s_alarm_str1.wstr_ptr, alarm_str[1].wstr_ptr, alarm_str[1].wstr_len );
            s_alarm_str1.wstr_len = alarm_str[1].wstr_len;
        
            if( PNULL != s_alarm_str2.wstr_ptr )
            {
                SCI_FREE(s_alarm_str2.wstr_ptr);
            }
            s_alarm_str2.wstr_ptr = SCI_ALLOCAZ(alarm_str[0].wstr_len * sizeof(wchar));
            SCI_MEMSET(s_alarm_str2.wstr_ptr , 0, alarm_str[0].wstr_len* sizeof(wchar));
            
            MMIAPICOM_Wstrncpy(s_alarm_str2.wstr_ptr, alarm_str[0].wstr_ptr, alarm_str[0].wstr_len );
            s_alarm_str2.wstr_len = alarm_str[0].wstr_len;

            s_line_num = line_num;
            s_icon = icon;
            s_win_priority = win_priority;

            s_is_need_ring_and_vib = TRUE;
            
            MMIPUB_OpenWaitWin( line_num,PNULL,PNULL,PNULL,
             MMIALM_ALIVEWIN_DURING_ETWS_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_DIALOG_DISMISS, func);
        }
        else
#endif
    {
        MMIPUB_OpenAlarmDialogWin(line_num,&alarm_str[1],&alarm_str[0],PNULL,
             MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_DIALOG_ALARM, func);
    }
#endif
    }
#ifdef MMI_AZAN_SUPPORT
    else if (SMART_EVENT_AZAN == s_arrived_event.event_fast_info.type)
    {
        MMIAPIAZAN_OpenAlarmExpiredWin(FALSE);
    }
#endif /*MMI_AZAN_SUPPORT*/
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    else if (SMART_FM_TIMER_RECORD == s_arrived_event.event_fast_info.type)
    {
        MMIAPIFM_StartFMTimerRecord();
    }
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    else if (SMART_EVENT_SOGOU_UPDATE == s_arrived_event.event_fast_info.type)
    {
        //SCI_TRACE_LOW:"SOGOU Update event coming"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6491_112_2_18_2_1_3_40,(uint8*)"");
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition())
        {
            MMK_SendMsg(VIRTUAL_WIN_ID, MSG_ALM_ALERTING_END, PNULL);
        }
#ifdef WIFI_SUPPORT
        else if (MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
#else
        else if (MMIAPIPHONE_GetSimOkNum(PNULL, 0) == 0 || MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
#endif
        {
            // ���߱���������������������������ʾ
        }
        else
        {
            MMIAPIIM_SogouAutoUpdateWordRemind();
        }
    }
#endif
    else if (SMART_EVENT_SCH == s_arrived_event.event_fast_info.type)
    {
#if defined(SCH_ASSISTANT_WIDGET) && defined(MMI_GRID_IDLE_SUPPORT)
        if(MMIAPIWIDGET_IsScheduleOnTop())
        {
            MMK_SendMsg(WIDGET_SCHEDULE_WIN_ID, MSG_SCH_EVENT_ARRIVE, &s_arrived_event);
        }
        else
        {
            //if(WIN_LOWEST_LEVEL == win_priority)//�����ȼ���û����ʾ
            //{
                func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
                MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
                MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_ONE, func);
            //}
            //else
            //{
            //    MMK_CreatePubFormWin((uint32 *)MMISCH_POP_ALERT_WIN_TAB, PNULL);
            //}
        }
#else
        //[add interface for judge if alarm win is
        // on foreground]
        SetAlarmNotifyMark(win_priority != WIN_LOWEST_LEVEL);
        //[end]
        func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
        //MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
        //    MMIALM_ALIVEWIN_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_ONE, func);
#ifdef MMI_ETWS_SUPPORT
        if(TRUE == MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
        {
            // Copy alarm str1, alarm str2, icon, line num, priority
            if( PNULL != s_alarm_str1.wstr_ptr )
            {
                SCI_FREE(s_alarm_str1.wstr_ptr);
            }
            s_alarm_str1.wstr_ptr = SCI_ALLOCAZ(alarm_str[1].wstr_len * sizeof(wchar));
            SCI_MEMSET(s_alarm_str1.wstr_ptr , 0, alarm_str[1].wstr_len* sizeof(wchar));
            
            MMIAPICOM_Wstrncpy(s_alarm_str1.wstr_ptr, alarm_str[1].wstr_ptr, alarm_str[1].wstr_len );
            s_alarm_str1.wstr_len = alarm_str[1].wstr_len;
        
            if( PNULL != s_alarm_str2.wstr_ptr )
            {
                SCI_FREE(s_alarm_str2.wstr_ptr);
            }
            s_alarm_str2.wstr_ptr = SCI_ALLOCAZ(alarm_str[0].wstr_len * sizeof(wchar));
            SCI_MEMSET(s_alarm_str2.wstr_ptr , 0, alarm_str[0].wstr_len* sizeof(wchar));
            
            MMIAPICOM_Wstrncpy(s_alarm_str2.wstr_ptr, alarm_str[0].wstr_ptr, alarm_str[0].wstr_len );
            s_alarm_str2.wstr_len = alarm_str[0].wstr_len;

            s_line_num = line_num;
            s_icon = icon;
            s_win_priority = win_priority;

            s_is_need_ring_and_vib = TRUE;

            MMIPUB_OpenWaitWin( line_num,PNULL,PNULL,PNULL,
             MMIALM_ALIVEWIN_DURING_ETWS_ID,icon,PNULL,win_priority,MMIPUB_SOFTKEY_DIALOG_DISMISS, func);
        }
        else
#endif
        {
        MMIPUB_OpenAlarmDialogWin(line_num, &alarm_str[1], &alarm_str[0], PNULL,
            MMIALM_ALIVEWIN_ID, icon, PNULL, win_priority, MMIPUB_SOFTKEY_DIALOG_ALARM, func);
        }

#endif

    }

    if (PNULL != tmp_str2)
    {
        SCI_FREE(tmp_str2);
        tmp_str2 = PNULL;
    }
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
// Description : open alarm dialog
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_OpenAlarmDialogWin(void)
{
    MMIPUB_HANDLE_FUNC  func         = NULL;

        func = (MMIPUB_HANDLE_FUNC )HandleEventAliveWinMsg;
        s_is_need_ring_and_vib = FALSE;

        MMIPUB_OpenAlarmDialogWin(s_line_num, &s_alarm_str1, &s_alarm_str2,PNULL,
             MMIALM_ALIVEWIN_ID, s_icon,PNULL,s_win_priority,MMIPUB_SOFTKEY_DIALOG_ALARM, func);

}
#endif

/*****************************************************************************/
//  Description : handle past event win msg
//  Global resource dependence : 
//  Author:       xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePastEventWinMsg(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E                            recode = MMI_RESULT_TRUE;    
    MMIPUB_INFO_T*                          win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    static uint8                            s_timer_id = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T*  past_event = MMIALM_GeEvent(s_past_event_id);

    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    win_info_ptr->win_id = win_id;

    //SCI_TRACE_LOW:"HandlePastEventWinMsg, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6565_112_2_18_2_1_3_41,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW: 

        s_arrived_event_id = s_past_event_id;
        MMIAPIALM_FormInteEventInfo(s_arrived_event_id,&s_arrived_event);
        MMIAPIALM_GetRingInfo(s_arrived_event_id, &s_arrived_event);

        if (SMART_EVENT_ALARM == past_event->type)
        {
            MMIPUB_SetWinTitleTextId(win_id,STR_DEFAULT_ALARM_NAME,FALSE);
        }
        else if(SMART_EVENT_SCH == past_event->type)
        {
            MMIPUB_SetWinTitleTextId(win_id,STR_CALE_EVENT_DEFAULT_EXT02,FALSE);
        }

#if 0 //UNISOC_MMI_Delete
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_EXIT, FALSE);
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)STXT_EXIT, FALSE);
#endif
#endif //UNISOC_MMI_Delete

        StartAlarmTimer(&s_past_event_timer_id, ALERT_RING_DURATION, FALSE);
        StartAlarmTimer(&s_timer_id, 100, FALSE);
        StartRingOrVib();//CR162626
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);

        break;

    case MSG_FULL_PAINT:
        {        
            if(PNULL != win_info_ptr)
            {
                DisplayAlarmWin(win_id,win_info_ptr);
            }
        }
        break;

    case MSG_GET_FOCUS: 
        recode = MMIPUB_HandleWaitWinMsg(win_id, MSG_FULL_PAINT, PNULL);
        if (0 == s_past_event_timer_id )
        {
            if (MMIAPIKL_IsPhoneLocked())
            {
                MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
            }
        }
        MMIDEFAULT_TurnOnBackLight();//CR162626
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        break;

    case MSG_TIMER:
        if (s_past_event_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            MMK_StopTimer(s_past_event_timer_id);
            s_past_event_timer_id = 0;

            //MMK_CloseWin(win_id);
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_AddUnreadEvent(s_past_event_id);
#endif
            if (MMIAPIKL_IsPhoneLocked())
            {
                if(MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
                }
            }
            MMK_CloseWin(win_id);
            CheckandStartExpiredEvent();
        }
        else if(s_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            MMK_StopTimer(s_timer_id);
            s_timer_id = 0;
            if(!MMK_IsFocusWin(win_id))
            {
                MMK_WinGrabFocus(win_id);
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            }
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_PROMPTWIN_OK:
        if (SMART_EVENT_ALARM == past_event->type)
        {
            MMK_CloseWin(win_id);
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_DeleteUnreadEvent(s_past_event_id);
#endif
            CheckandStartExpiredEvent();

            HandleAlarmSoftkeyOpenMsg();
        }
        else if(SMART_EVENT_SCH == past_event->type)
        {
            MMK_CloseWin(win_id);
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_DeleteUnreadEvent(s_past_event_id);
#endif
            CheckandStartExpiredEvent();
            HandleCalendarSoftkeyOpenMsg(s_past_event_id, PNULL);
        }
        break;

   case MSG_APP_WEB:
        if (SMART_EVENT_ALARM == past_event->type
            ||SMART_EVENT_SCH == past_event->type)
        {
            HandleAlarmSoftkeyDissmissMsg(win_id, s_past_event_id, past_event->type, past_event->snooze_type);
            SnoozedPastAlarm(win_id);
        }

        break;

    case MSG_CLOSE_WINDOW:
        if (0 != s_past_event_timer_id )
        {
            MMK_StopTimer(s_past_event_timer_id);
            s_past_event_timer_id = 0;
        }
        StopRingOrVib(FALSE);//CR162626
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        break;

    case MSG_KEYDOWN_RED:
    case MSG_KEYLONG_RED:
    //case MSG_KEYDOWN_FLIP:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);// �ȹرգ��ٴ�
#ifdef CALENDAR_SUPPORT
        MMIAPISCH_DeleteUnreadEvent(s_past_event_id);
#endif

        CheckandStartExpiredEvent();
        break;

    default:
        if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_WEB ) ||
            (MSG_KEYUP_FLIP == msg_id || MSG_CTL_OK == msg_id))
        {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
            if (MSG_KEYUP_FLIP == msg_id)
            {
                MMIDEFAULT_HandleFlipKey(TRUE);  
            }
#endif            
        }
        else
        {
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
        }
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Open bypast sch event win(�򿪹����¼�����)
//  Global resource dependence : 
//  Author: xiaoqing.lu 
//  Note:   
/*****************************************************************************/
LOCAL void OpenPastSchEventWin(
                               MMI_WIN_PRIORITY_E win_priority,
                               uint16 event_id
                               )
{
    SCI_DATE_T          past_date = {0};
    SCI_TIME_T          past_time = {0};
    MMI_STRING_T        alarm_str[5] = {0};
    MMI_IMAGE_ID_T      icon = IMAGE_NULL;
    uint8                date_str[MMISET_DATE_STR_LEN] = {0}; //2005.06.30
    char                time_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};  //10:34
    wchar                date_unicode_str[11+1] = {0}; //2005.06.30
    wchar                time_unicode_str[MMIALM_TIME_STR_12HOURS_LEN+1]  = {0};  //"%02d:%02d PM"   
    MMIPUB_HANDLE_FUNC  func = NULL;
    uint8               line_num = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T   *past_event = {0};
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
    uint16                          nv_return = 0;

    s_past_event_id=event_id;

    past_event = MMIALM_GeEvent(event_id);

    /*past_date.year = s_arrived_event.event_fast_info.year;
    past_date.mon = s_arrived_event.event_fast_info.mon;
    past_date.mday = s_arrived_event.event_fast_info.day;
    past_time.hour = s_arrived_event.event_fast_info.hour;
    past_time.min = s_arrived_event.event_fast_info.minute;
    past_time.sec = 0;*/

    past_date.year = past_event->year;
    past_date.mon = past_event->mon;
    past_date.mday = past_event->day;
    past_time.hour = past_event->hour;
    past_time.min = past_event->minute;
    past_time.sec = 0;

    //��Ӧ�Ķ�Ϊ��ǰʱ�䣬��Ӧ��ȥ��ȡNV��ʱ��
    MMIAPISET_FormatDateStrByDateStyle(past_date.year, past_date.mon, past_date.mday,'.',date_str,MMISET_DATE_STR_LEN);

    MMIAPISET_FormatTimeStrByTime(past_time.hour,past_time.min,(uint8*)time_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
    if (SMART_EVENT_ALARM ==past_event->type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
//        alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = time_unicode_str;        
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

       // alarm_str[0].str_ptr = (uint8*)time_str;
       // alarm_str[0].length = strlen(time_str);
       // alarm_str[0].is_ucs2 = FALSE;
        MMI_GetLabelTextByLang(STR_DEFAULT_ALARM_NAME, &alarm_str[1]);                
        line_num = 2;
    }
    else if ( SMART_EVENT_AUTOPOWERON == past_event->type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
//        alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = time_unicode_str;   
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        MMI_GetLabelTextByLang(TXT_AUTOPOWER_ON, &alarm_str[1]); 
        line_num = 2;
    }
    else if ( SMART_EVENT_AUTOPOWEROFF == past_event->type)
    {
        alarm_str[0].wstr_len = strlen(time_str);
//        alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = time_unicode_str;  
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

        MMI_GetLabelTextByLang(TXT_AUTOPOWER_OFF, &alarm_str[1]); 
        line_num = 2;
    } 
    else if (SMART_EVENT_SCH == past_event->type)
    {
        wchar                       wstr_buf[30]        = {0};
        wchar                       wstr_split[]        = {L", "};
        size_t                        time_len = strlen(time_str);

        size_t                        date_len = strlen(date_str);

        alarm_str[0].wstr_ptr = wstr_buf;

        MMIAPICOM_StrToWstr(time_str, time_unicode_str);
        MMIAPICOM_StrToWstr(date_str, date_unicode_str);

        MMIAPICOM_Wstrcpy(alarm_str[0].wstr_ptr, (const wchar*)time_unicode_str);
        MMIAPICOM_Wstrncat(alarm_str[0].wstr_ptr, wstr_split,MMIAPICOM_Wstrlen(wstr_split));
        MMIAPICOM_Wstrncat(alarm_str[0].wstr_ptr, date_unicode_str, date_len);

        alarm_str[0].wstr_len = MMIAPICOM_Wstrlen(alarm_str[0].wstr_ptr);


        MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &event_content,nv_return);
        if(0 == event_content.w_length)
        {
            MMI_GetLabelTextByLang(STR_CALE_EVENT_DEFAULT_EXT02,&alarm_str[1]);
        }
        else
        {
            alarm_str[1].wstr_len = event_content.w_length;       
            alarm_str[1].wstr_ptr = event_content.w_content;    /*lint !e605 */
        }

        line_num = 2;
    }
    else
    {
        alarm_str[0].wstr_len = strlen((char*)date_str);
//        alarm_str[0].wstr_ptr = SCI_ALLOC_APP(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[0].wstr_ptr = date_unicode_str;
        SCI_MEMSET(alarm_str[0].wstr_ptr,0,(( alarm_str[0].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[0].wstr_ptr,alarm_str[0].wstr_len,date_str,strlen(date_str),strlen(date_str));/*lint !e64*/

        alarm_str[1].wstr_len = strlen(time_str);
//        alarm_str[1].wstr_ptr = SCI_ALLOC_APP(( alarm_str[1].wstr_len + 1 ) * sizeof(wchar) );
        alarm_str[1].wstr_ptr = time_unicode_str;
        SCI_MEMSET(alarm_str[1].wstr_ptr,0,(( alarm_str[1].wstr_len + 1 ) * sizeof(wchar)));
        MMI_STRNTOWSTR(alarm_str[1].wstr_ptr,alarm_str[1].wstr_len,time_str,strlen(time_str),strlen(time_str));/*lint !e64*/

//         alarm_str[2].wstr_len = past_event->w_length;       
//         alarm_str[2].wstr_ptr = past_event->w_content;   /*lint !e605 */

        MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &event_content,nv_return);
        alarm_str[2].wstr_len = event_content.w_length;       
        alarm_str[2].wstr_ptr = event_content.w_content;    /*lint !e605 */

        line_num = 3;
    }

    if (SMART_EVENT_ALARM ==past_event->type ||
        SMART_EVENT_AUTOPOWERON == past_event->type ||
        SMART_EVENT_AUTOPOWEROFF == past_event->type)
    {
        icon = IMAGE_IDLE_SCREEN_ALARM_ICON_LARGE;
    }
    else if (SMART_EVENT_SCH == past_event->type
#ifdef MMI_AZAN_SUPPORT
        ||(SMART_EVENT_AZAN == past_event->type)
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
      ||(SMART_FM_TIMER_RECORD == past_event->type)
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
      || (SMART_EVENT_SOGOU_UPDATE == past_event->type)
#endif
    )
    {
        icon = IMAGE_CLOCK_CALENDAR_ALERT;
    }
    else
    {
        // SCI_PASSERT(0, ("OpenPastSchEventWin: Invalid past_event->type is %d",past_event->type));/*assert verified*/        
        //SCI_TRACE_LOW:"OpenPastSchEventWin error: Invalid past_event->type is %d:"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6792_112_2_18_2_1_4_42,(uint8*)"d",past_event->type);
        return;
    }

    if (SMART_EVENT_AUTOPOWERON == past_event->type)
    {
        func = (MMIPUB_HANDLE_FUNC)HandlePastAutoPowerOn;
        if(MMK_IsOpenWin(MMIALM_BYPAST_SCH_WIN_ID))
        {
            MMK_CloseWin(MMIALM_BYPAST_SCH_WIN_ID);
        }
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_BYPAST_SCH_WIN_ID,icon,PNULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,func);    
    }
    else if (SMART_EVENT_AUTOPOWEROFF == past_event->type)
    {
        func = (MMIPUB_HANDLE_FUNC)HandlePastEventWinMsg;
        if(MMK_IsOpenWin(MMIALM_BYPAST_SCH_WIN_ID))
        {
            MMK_CloseWin(MMIALM_BYPAST_SCH_WIN_ID);
        }
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_BYPAST_SCH_WIN_ID,icon,PNULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,func);    
    }
#ifdef MMI_AZAN_SUPPORT
    else if (SMART_EVENT_AZAN == past_event->type)
    {
        MMIAPIAZAN_OpenAlarmExpiredWin(FALSE);
    }
#endif /*MMI_AZAN_SUPPORT*/
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    else if (SMART_FM_TIMER_RECORD == past_event->type)
    {
            MMIAPIFM_StartFMTimerRecord();
    }
#endif
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
    else if (SMART_EVENT_SOGOU_UPDATE == past_event->type)
    {
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition())
        {
            MMK_SendMsg(VIRTUAL_WIN_ID, MSG_ALM_ALERTING_END, PNULL);
        }
#ifdef WIFI_SUPPORT
        else if ( MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
#else
        else if (MMIAPIPHONE_GetSimOkNum(PNULL, 0) == 0 
                    || MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
#endif
        {
            // ���߱���������������������������ʾ
        }
        else
        {
            MMIAPIIM_SogouAutoUpdateWordRemind();
        }
    }
#endif
    else if(SMART_EVENT_ALARM == past_event->type || SMART_EVENT_SCH == past_event->type)
    {
        func = (MMIPUB_HANDLE_FUNC)HandlePastEventWinMsg;
        MMIPUB_OpenAlarmDialogWin(line_num, &alarm_str[1], &alarm_str[0], PNULL, 
            MMIALM_ALIVEWIN_ID, icon, PNULL, win_priority, MMIPUB_SOFTKEY_DIALOG_ALARM, func);
    }
    else
    {
        func = (MMIPUB_HANDLE_FUNC)HandlePastEventWinMsg;
        if(MMK_IsOpenWin(MMIALM_BYPAST_SCH_WIN_ID))
        {
            MMK_CloseWin(MMIALM_BYPAST_SCH_WIN_ID);
        }
        MMIPUB_OpenWaitWin(line_num,&alarm_str[0],&alarm_str[1],&alarm_str[2],
            MMIALM_BYPAST_SCH_WIN_ID,icon,PNULL,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE, func);    
    }

}

/*****************************************************************************/
//  Description : intial Alarm module  
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_InitModule(void)
{
    MMIALM_RegMenuGroup();
    MMIALM_RegAlarmNv();
    MMIALARM_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : return alarm expired win id  
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIALM_GetAlarmPromptWinId(void)
{
    return MMIALM_ALIVEWIN_ID;
}

/*****************************************************************************/
//  Description : get first alarm ring, for setting use
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALM_GetFirstAlarmRing(
                              BOOLEAN       *is_fixed,
                              uint16        *ring_id,
                              uint16        *file_name_len,
                              wchar         *file_name_ptr
                              )
{
    uint16                          nv_return = 0;
    MMIACC_SMART_EVENT_CONTENT_T    event = {0};
    MMIACC_SMART_EVENT_RING_T       event_ring = {0};

    //SCI_ASSERT(NULL != is_fixed); /*assert verified*/
    //SCI_ASSERT(NULL != ring_id); /*assert verified*/
    //SCI_ASSERT(NULL != file_name_len); /*assert verified*/
    //SCI_ASSERT(NULL != file_name_ptr); /*assert verified*/
    if(NULL == is_fixed || NULL == ring_id ||NULL == file_name_len ||NULL == file_name_ptr)
    {
        return;
    }
    MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+EVENT_ALARM_0, &event, nv_return);

    if(ALM_FIX_RING == event.ring_type)
    {
        *is_fixed = TRUE;
        *ring_id = event.ring;
    }
    else
    {
        //��Ҫfs��ز���
        *is_fixed = FALSE;

        if (!ReadRingInfoFileName(EVENT_ALARM_0,&event_ring))//��������
        {
            *is_fixed = TRUE;
            *ring_id = event.ring;
            //SCI_TRACE_LOW:"mmialarm.c MMIAPIALM_GetFirstAlarmRing ReadRingInfoFileName fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_6923_112_2_18_2_1_4_43,(uint8*)"");
            
            return;
        }
        *file_name_len = event_ring.dring_name_len;
        MMI_WSTRNCPY( file_name_ptr, MMIFILE_FULL_PATH_MAX_LEN, 
                   event_ring.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN, MMIFILE_FULL_PATH_MAX_LEN);
    }
}

/*****************************************************************************/
//  Description : DisplayTheRingName
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void DisplayTheRingName(void)
{
    //wchar              ring_str[MMIFILE_FILE_NAME_MAX_LEN+1]  = {0};
    wchar              file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    wchar               *ring_name_ptr                         = NULL;
    uint16              file_name_len                          = 0;
    uint16              fixed_ringid                           = 0;
    uint16              ring_name_len                          = 0;
    MMI_STRING_T        ring                                   = {0};
    MMI_STRING_T        text            = {0};
#ifdef FM_SUPPORT    
    wchar fm[3]={'F','M',0};
#endif
#ifdef DRM_SUPPORT
    BOOLEAN     drm_permission = TRUE;
    if (MMIAPIFMM_IsDrmFileBySuffix(s_current_event_info.event.event_ring_info.dring_name_arr,
        s_current_event_info.event.event_ring_info.dring_name_len))
    {
        //����Ƿ�����ˣ���������ˣ����ó�Ĭ�ϵ�ͼƬ
        if (MMIAPIDRM_IsRightsExpired(SFS_INVALID_HANDLE, 
            s_current_event_info.event.event_ring_info.dring_name_arr, DRM_PERMISSION_PLAY))
        {
            drm_permission = FALSE; 
            SCI_TRACE_LOW("DisplayTheRingName: drm_perssion set as false\n");
        }
    }
#endif
//     MMI_TEXT_ID_T       ring_text                              = TXT_NULL;

    fixed_ringid = s_current_event_info.event.event_content_info.ring;

    ring_name_ptr = s_current_event_info.event.event_ring_info.dring_name_arr;

    ring_name_len = s_current_event_info.event.event_ring_info.dring_name_len;

    ring.wstr_ptr = file_name;        
    
//@zhaohui.cr112262,Ӧֻ��ALM_DOWNLOAD_RING��ALM_FIX_RING�������  
    if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type
        ||ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type)
    {
        //SCI_ASSERT(NULL != ring_name_ptr); /*assert verified*/
        if (MMIAPIUDISK_UdiskIsRun())//@zhaohui,cr111342
        {
             ring.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
             MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
             GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
             
        }
        else if(!MMIAPIFMM_IsFileExist(ring_name_ptr, ring_name_len)
#ifdef DRM_SUPPORT
            || !drm_permission
#endif
            )
        {
            //�����ļ�������ʱ������������
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

            ring.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
            MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
            GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
        }
        else
        {
            MMIAPIFMM_SplitFullPath(ring_name_ptr, ring_name_len,
                                  NULL, NULL, NULL, NULL,
                                  file_name, &file_name_len);

            ring.wstr_len = file_name_len;
            ring.wstr_ptr = file_name;
        }
        
    }
#ifdef FM_SUPPORT
    else if(ALM_FM_RING == s_current_event_info.event.event_content_info.ring_type)
    {
        //SCI_ASSERT(NULL != ring_name_ptr); /*assert verified*/
        if (MMIAPIUDISK_UdiskIsRun())//@zhaohui,cr111342
        {
             ring.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
             MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
             GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
        }
        else if(!MMIAPIALM_IsValidFM(ring_name_ptr, ring_name_len))
        {
            //�����ļ�������ʱ������������
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

            ring.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
            MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
            GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
        }
        else
        {
            //ring.wstr_len = ring_name_len;
            //ring.wstr_ptr = ring_name_ptr;
            MMI_STRING_T ring_str={0};
#ifndef FM_SUPPORT_NONE
            wchar*channel_name=PNULL;
#endif
            ring_str.wstr_len=ring_name_len;
            ring_str.wstr_ptr = ring_name_ptr;
#ifndef FM_SUPPORT_NONE
            channel_name = MMIAPIFM_GetChannelName(MMIAPIFM_ConvertStringToFreq(&ring_str));
#endif
#ifndef FM_SUPPORT_NONE
            if(PNULL!=channel_name)
            {
                ring.wstr_len = MMIAPICOM_Wstrlen(channel_name);
                ring.wstr_ptr = channel_name; 
            }
            else
#endif
            {
                MMIAPICOM_Wstrncpy(ring.wstr_ptr, fm, 2);
                MMIAPICOM_Wstrcat(ring.wstr_ptr, ring_name_ptr);
                ring.wstr_len=MMIAPICOM_Wstrlen(ring.wstr_ptr);
            }
        }
        
    }
#endif  //FM_SUPPORT
    else
    {
        ring.wstr_len = MMIAPISET_GetRingName(fixed_ringid, file_name, MMIFILE_FILE_NAME_MAX_LEN);
    }
//#ifdef MMI_PDA_SUPPORT
    GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &ring, FALSE);
//#else
//    GUILABEL_SetText(MMIALM_EDIT_RING_TEXT_CTRL_ID, &ring, FALSE);
//#endif
}

/*****************************************************************************/
//  Description : add item for setlist
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  AddItemSetlist(void)
{
    MMI_CTRL_ID_T           current_ctrl_id = MMIALM_EDIT_MODE_SETLIST_CTRL_ID;
//     uint32 text_id[3]=
//     {
//         TXT_ALARM_MODE_ONCE,
//         TXT_ALARM_MODE_EVERYDAY,
//         TXT_ALARM_MODE_WEEK
//     };
    uint32 loop=0;

    
    for(loop=0;loop<3;loop++)
    {
        GUISETLIST_AddItemById(current_ctrl_id, s_alarm_mode_text[loop]);
    }
    
    return;
}

/*****************************************************************************/
//  Description : process the MIDSK message
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
// LOCAL void EditWinMIDSKProcess(
//                                 MMI_WIN_ID_T win_id //in
//                                 )
// {
// //  uint32 setlist_index=0;
//     MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
//     
//     switch(ctrl_id)
//     {
//     case MMIALM_EDIT_RING_TYPE_CTRL_ID:
//         MMK_CreateWin((uint32 *)ALARM_RINGWIN_TAB, PNULL);
//         break;
//     case MMI_EDITWIN_BUTTON0_CTRL_ID:
//     case MMI_EDITWIN_BUTTON1_CTRL_ID:
//     case MMI_EDITWIN_BUTTON2_CTRL_ID:
//     case MMI_EDITWIN_BUTTON3_CTRL_ID:
//     case MMI_EDITWIN_BUTTON4_CTRL_ID:
//     case MMI_EDITWIN_BUTTON5_CTRL_ID:
//     case MMI_EDITWIN_BUTTON6_CTRL_ID:
//             if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
//             {
//                 s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=FALSE;
//             }
//             else
//             {
//                 s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=TRUE;
//             }
// #ifndef MMI_PDA_SUPPORT  
//             setAlarmEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
// #endif
//             SetEditWinButtonBg(win_id,ctrl_id);
//             break;
//     default:
//         break;
//     }
// }

/*****************************************************************************/
//  Description : set title for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetTitleEditWin(
                                MMI_WIN_ID_T win_id //in
                                )
{
    switch(s_current_event_info.id) 
    {
    case EVENT_AUTO_POWERON:
        GUIWIN_SetTitleTextId(win_id,TXT_AUTOPOWER_ON,FALSE);
        break;
    case EVENT_AUTO_POWEROFF:
        GUIWIN_SetTitleTextId(win_id,TXT_AUTOPOWER_OFF,FALSE);
        break;            
    default:
        GUIWIN_SetTitleTextId(win_id,TXT_ALARM_CLOCK,FALSE);
        break;
    }
     
}

/*****************************************************************************/
//  Description : set name ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetNameParamEditWin(void)
{
    MMI_STRING_T  name_str        = {0};
    MMI_STRING_T  name_str1      = {0};
    MMI_STRING_T  text                = {0};
    MMI_TEXT_ID_T txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10};
    MMI_TEXT_ID_T txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                        TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                        TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};
    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        GUIFORM_SetStyle(MMIALM_EDIT_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        MMI_GetLabelTextByLang(STXT_MAIN_MESSAGE, &text);
        GUILABEL_SetText(MMIALM_EDIT_NAME_LABEL_CTRL_ID, &text, FALSE);

        MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str1);
        if (!MMIAPICOM_CompareTwoWstrExt(name_str1.wstr_ptr,
                                            name_str1.wstr_len,
                                            s_current_event_info.event.event_content_info.w_content,
                                            s_current_event_info.event.event_content_info.w_length,
                                            TRUE)
            ||(0 == s_current_event_info.event.event_content_info.w_length) )
        {
            if (s_current_event_info.id <= EVENT_ALARM_MAX) //&&s_current_event_info.id>=EVENT_ALARM_0 /*lint !e685 !e568*/
            {
                MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str);
            }
            else if(s_current_event_info.id>=EVENT_ALARM_DELAY_0&&s_current_event_info.id <= EVENT_ALARM_DELAY_MAX)
            {
                MMI_GetLabelTextByLang(txt_alarm_id_1[s_current_event_info.id-EVENT_ALARM_DELAY_0],&name_str);
            }
        }
        else
        {
            name_str.wstr_ptr = s_current_event_info.event.event_content_info.w_content;
            name_str.wstr_len = s_current_event_info.event.event_content_info.w_length;
        }
#ifdef MMI_PDA_SUPPORT
        GUILABEL_SetText(MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID, &name_str,FALSE);
#else
        GUIEDIT_SetString(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, name_str.wstr_ptr, name_str.wstr_len);
#endif
    }   


}

/*****************************************************************************/
//  Description : set time ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetTimeParamEditWin(void)
{
    
#ifndef MMI_PDA_SUPPORT
    GUIEDIT_TIME_STYLE_E time_style =GUIEDIT_TIME_STYLE_24;
//  MMISET_TIME_T   time = {0};
    MMISET_TIME_DISPLAY_TYPE_E time_type=MMISET_TIME_24HOURS;
#endif
    MMI_STRING_T        title_text            = {0};

    GUIFORM_SetStyle(MMIALM_EDIT_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_TIME, &title_text);
    GUILABEL_SetText(MMIALM_EDIT_TITLE_LABEL_CTRL_ID, &title_text, FALSE);

#ifdef MMI_PDA_SUPPORT
    {
        uint8               time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]    = {0};

        MMI_STRING_T        text            = {0};

        if (s_current_event_info.event.event_fast_info.is_valid )
        {
            MMIALM_FormTimeStrbyTime(s_current_event_info.event.event_fast_info.hour,
                s_current_event_info.event.event_fast_info.minute,
                (char*)time_str,
                MMIALM_TIME_STR_12HOURS_LEN+1
                );
        }
        else
        {
            SCI_TIME_T      sys_time = {0};
            TM_GetSysTime(&sys_time);
            MMIALM_FormTimeStrbyTime(sys_time.hour,
                sys_time.min,
                (char*)time_str,
                MMIALM_TIME_STR_12HOURS_LEN+1
                );
        }

        text.wstr_len = 10;
        text.wstr_ptr = SCI_ALLOC_APP((text.wstr_len +1)* sizeof(wchar));
        SCI_MEMSET(text.wstr_ptr,0,((text.wstr_len +1)* sizeof(wchar)));
        MMI_STRNTOWSTR(text.wstr_ptr,10,(uint8*)time_str,10,10);

        GUILABEL_SetText(MMIALM_EDIT_TITLE_EDIT_LABEL_CTRL_ID, &text, FALSE);
        if (PNULL != text.wstr_ptr)
        {
            SCI_FREE(text.wstr_ptr);
            text.wstr_ptr = PNULL;
        }

    }
#else
    time_type = MMIAPISET_GetTimeDisplayType();
    if (MMISET_TIME_12HOURS == time_type)
    {
        time_style = GUIEDIT_TIME_STYLE_12;
    }
    else
    {
        time_style = GUIEDIT_TIME_STYLE_24;
    }

    //set time display style
    GUIEDIT_SetTimeStyle(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,PNULL,&time_style,PNULL,FALSE);
    //GUIEDIT_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_SET_TIME_FONT);

    if (s_current_event_info.event.event_fast_info.is_valid )
    {
        GUIEDIT_SetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
                            s_current_event_info.event.event_fast_info.hour, 
                            s_current_event_info.event.event_fast_info.minute,
                            0);
    }
    else
    {
        SCI_TIME_T      sys_time = {0};
        TM_GetSysTime(&sys_time);
        GUIEDIT_SetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
                            sys_time.hour, 
                            sys_time.min,
                            0);
    }
#endif
}

/*****************************************************************************/
//  Description : set ring ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetRingParamEditWin(void)
{
    MMI_STRING_T        text            = {0};
    
    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        GUIFORM_SetStyle(MMIALM_EDIT_RING_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        MMI_GetLabelTextByLang(TXT_RING, &text);
        GUILABEL_SetText(MMIALM_EDIT_RING_LABEL_CTRL_ID, &text, FALSE); // text
/*#ifndef MMI_PDA_SUPPORT
        if(ALM_DOWNLOAD_RING==s_current_event_info.event.event_content_info.ring_type)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_MORE_RING, &text);
        }
        else if(ALM_FM_RING==s_current_event_info.event.event_content_info.ring_type)
        {
            MMI_GetLabelTextByLang(TXT_ALARM_FM_RING, &text);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_RING_FIXED, &text);
        }
        GUILABEL_SetText(MMIALM_EDIT_RING_TYPE_CTRL_ID, &text, FALSE);
#endif*/
        DisplayTheRingName();
    }
}

#ifdef  CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : set VIBRATE ctrl param for edit win
//	Global resource dependence : 
//  Author: panvs
//	Note:   
/*****************************************************************************/
LOCAL void SetVibrateParamEditWin(void)
{
    MMI_STRING_T text = {0};
    uint16 index = 0;

    GUIFORM_SetStyle(MMIALM_EDIT_VIBRATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_RING_VIBRA, &text);
    GUILABEL_SetText(MMIALM_EDIT_VIBRATE_LABLE_CTRL_ID, &text, FALSE);
    
    GUISETLIST_AddItemById(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID, STR_POC_ILIST_OFF);
    GUISETLIST_AddItemById(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID, TXT_OPEN);

    if(s_current_event_info.event.event_content_info.is_vibrate)
    {
        index = 1;
    }
    else
    {
        index = 0;
    }
    
    GUISETLIST_SetCurIndex(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID, index );    
    GUISETLIST_SetFontAlign(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID,ALIGN_LVMIDDLE);

}
#endif

/*****************************************************************************/
//  Description : set mode ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetModeParamEditWin(void)
{
    MMI_STRING_T        text            = {0};
//  GUIFONT_REGION_STYLE_T  text_region = {0};
//#ifdef MMI_PDA_SUPPORT
//    uint16 len=0;
//    wchar buff[100]={0};
//#endif
    GUIFORM_SetStyle(MMIALM_EDIT_MODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef MMI_GUI_STYLE_MINISCREEN
    GUIFORM_SetStyle(MMIALM_EDIT_MODE_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    
    MMI_GetLabelTextByLang(TXT_ALARM_EDIT_MODE, &text);
    GUILABEL_SetText(MMIALM_EDIT_MODE_LABEL_CTRL_ID, &text, FALSE);
    //text_region.length = text.wstr_len;
    //text_region.start = 0;
    //text_region.state = GUISTR_STATE_UNDERLINE;
    //GUILABEL_SetTextRegion(MMIALM_EDIT_MODE_LABEL_CTRL_ID, &text_region, 1);
//#ifdef MMI_PDA_SUPPORT
 //   len=GetFreModeString(s_current_event_info.event.event_fast_info.fre_mode,buff,100);
//    text.wstr_ptr=buff;
//    text.wstr_len=len;
//    GUILABEL_SetText(MMIALM_EDIT_MODE_SETLIST_CTRL_ID, &text, FALSE);
//#else
    GUISETLIST_SetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,
                            s_current_event_info.event.event_fast_info.fre_mode);
    
    GUISETLIST_SetTitleTextId(MMIALM_EDIT_MODE_SETLIST_CTRL_ID, TXT_ALARM_EDIT_MODE);
//#endif
}

/*****************************************************************************/
//  Description : set  weeks ctrl param for edit win
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetWeeksParamEditWin(MMI_WIN_ID_T      win_id)
{
    uint8 day_num=0;
    uint8   week_day =0;
//  MMI_STRING_T string={0};
    uint32 ctrl_id[]={  MMI_EDITWIN_BUTTON0_CTRL_ID,
                        MMI_EDITWIN_BUTTON1_CTRL_ID,
                        MMI_EDITWIN_BUTTON2_CTRL_ID,
                        MMI_EDITWIN_BUTTON3_CTRL_ID,
                        MMI_EDITWIN_BUTTON4_CTRL_ID,
                        MMI_EDITWIN_BUTTON5_CTRL_ID,
                        MMI_EDITWIN_BUTTON6_CTRL_ID,
                    };
    GUIFORM_CHILD_WIDTH_T  width={0}; 
    GUI_BG_T  bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};
    uint16 hor_space = 0;
    uint16 image_width = 0;
    uint16 image_height = 0;
    GUI_BORDER_T border = {0};
	uint16 week_day_image_adjust = 0;
#ifdef MMI_GUI_STYLE_MINISCREEN
	uint16 space_sub = MMITHEME_SLIDEWIN_SCROLLBAR_WIDTH-1;
#else
	uint16 space_sub = 1;
#endif
   
    bg.bg_type=GUI_BG_IMG;
    enabled_font.color=MMI_BLACK_COLOR;//MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    enabled_font.font=SONG_FONT_14;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_BLACK_COLOR;//MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    disabled_font.font=SONG_FONT_14;
#else
    disabled_font.font=SONG_FONT_16;
#endif

	GUIFORM_SetStyle(MMI_EDITWIN_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_CLOCK_ALARM_WEEK_ENABLED, win_id);

    width.type=GUIFORM_CHILD_WIDTH_FIXED;
    width.add_data = (MMI_MAINSCREEN_WIDTH- space_sub * 2) / ALM_WEEK_DAY_NUM;
    GUIFORM_SetSpace(MMI_EDITWIN_CHILD_FORM_CTRL_ID, &hor_space, PNULL);    
    GUIFORM_SetMargin(MMI_EDITWIN_CHILD_FORM_CTRL_ID,0);  
    GUIFORM_PermitChildFont(MMI_EDITWIN_CHILD_FORM_CTRL_ID, FALSE);    
    border.type = GUI_BORDER_NONE;
    IGUICTRL_SetBorder(MMK_GetCtrlPtr(MMI_EDITWIN_CHILD_FORM_CTRL_ID), &border);
    
    for(day_num = 0; day_num < ALM_WEEK_DAY_NUM; day_num++)   
    {
		if ((width.add_data * ALM_WEEK_DAY_NUM + week_day_image_adjust) < (MMI_MAINSCREEN_WIDTH- space_sub * 2))
		{
			width.add_data = width.add_data + 1;
			week_day_image_adjust++;
		}
        week_day = MMIAPISET_GetWeekDayByOffset(day_num );
// #ifdef MAINLCD_DEV_SIZE_128X160 //CR133106
//         GUIBUTTON_SetTextAlign(ctrl_id[day_num],ALIGN_TOP);
// #else
        GUIBUTTON_SetTextAlign(ctrl_id[day_num],ALIGN_HVMIDDLE);
// #endif
        GUIBUTTON_SetTextId(ctrl_id[day_num],s_alm_week_text_id[ week_day ]);
        if(0!=(s_current_event_info.event.event_fast_info.fre & s_week_mask[week_day]) ) 
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_ENABLED;
            s_weeks_button_is_selected[day_num]=TRUE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;
            s_weeks_button_is_selected[day_num]=FALSE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &disabled_font);
        }
        GUIBUTTON_SetBg(ctrl_id[day_num],&bg);
        GUIBUTTON_PermitBorder(ctrl_id[day_num],FALSE);
        GUIBUTTON_SetOwnerDraw(ctrl_id[day_num],ButtonOwnerDrawFun);
        bg.img_id=IMAGE_CLOCK_ALARM_WEEK_PRESSED;
        GUIBUTTON_SetRunSheen(ctrl_id[day_num],FALSE);
        GUIBUTTON_SetPressedBg(ctrl_id[day_num],&bg);        
        GUIFORM_SetChildWidth(MMI_EDITWIN_CHILD_FORM_CTRL_ID,ctrl_id[day_num],&width);
		width.add_data = (MMI_MAINSCREEN_WIDTH - space_sub * 2) / ALM_WEEK_DAY_NUM;
    }
}

/*****************************************************************************/
//  Description : set  weeks display
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetWeeksDisplayEditWin(void)
{
    uint32 setlist_index=0;

    GUIFORM_SetCircularHandleLeftRight(MMI_EDITWIN_CHILD_FORM_CTRL_ID,TRUE);

    setlist_index=GUISETLIST_GetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID);
    if(ALM_MODE_EVERYWEEK!=setlist_index)
    {
        
        GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID, 
                            //MMIALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID, 
                            MMI_EDITWIN_CHILD_FORM_CTRL_ID,
                            GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        
        GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID, 
                            //MMIALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID,
                            MMI_EDITWIN_CHILD_FORM_CTRL_ID,
                            GUIFORM_CHILD_DISP_NORMAL);
        
    }
}

/*****************************************************************************/
//  Description : save name
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveNameEditWin(void)
{
    MMI_STRING_T        str        = {0};
    MMIACC_SMART_EVENT_CONTENT_T*event_content_ptr=PNULL;
    MMI_STRING_T  name_str      = {0};
    MMI_TEXT_ID_T txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                        TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                        TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10};
    MMI_TEXT_ID_T txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                        TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                        TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};
    event_content_ptr = &s_current_event_info.event.event_content_info;
    GUIEDIT_GetString(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, &str);
    if (MMIAPISET_IsIncludeReturnChar(str.wstr_ptr,str.wstr_len))
    {
        MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_TITLE);
        return FALSE;
    }

    if(0==str.wstr_len)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMM_NO_TITLE);
        return FALSE;
    }
    if(s_current_event_info.id <= EVENT_ALARM_MAX) //&&s_current_event_info.id>=EVENT_ALARM_0 /*lint !e685 !e568*/
    {
        MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str);
    }
    else if(s_current_event_info.id>=EVENT_ALARM_DELAY_0&&s_current_event_info.id <= EVENT_ALARM_DELAY_MAX)
    {
        MMI_GetLabelTextByLang(txt_alarm_id_1[s_current_event_info.id-EVENT_ALARM_DELAY_0],&name_str);
    }
    if(0==MMIAPICOM_CompareTwoWstrExt(name_str.wstr_ptr,
                                            name_str.wstr_len,
                                            str.wstr_ptr,
                                            str.wstr_len,
                                            TRUE))
    {
        /*if(0==MMIAPICOM_CompareTwoWstrExt(event_content_ptr->w_content,
                                            event_content_ptr->w_length,
                                            str.wstr_ptr,
                                            str.wstr_len,
                                            TRUE))
        {
            return TRUE ;
        }*/
        event_content_ptr->w_length=0;
        SCI_MEMSET(event_content_ptr->w_content,0,(sizeof(wchar))*(MMIACC_EVENT_TEXT_MAX_LEN+1));
        return TRUE ;
    }

    event_content_ptr->w_length = str.wstr_len;
    MMI_WSTRNCPY(
    event_content_ptr->w_content, MMIACC_EVENT_TEXT_MAX_LEN,
    str.wstr_ptr, str.wstr_len,
    str.wstr_len);

    return TRUE ;
}


/*****************************************************************************/
//  Description : save time
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimeEditWin(void)
{
    MMISET_TIME_T   time = {0};
    
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
     MMIALARM_GetTime(&time.time_hour, &time.time_minute);
#else	
	GUIEDIT_GetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
#endif
    if ( MMIAPICOM_CheckTimeIsValid(time))
    {
        s_current_event_info.event.event_fast_info.hour = time.time_hour; 
        s_current_event_info.event.event_fast_info.minute = time.time_minute;
    }
    else   // time invalid
    {
        MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
        return FALSE;
    }

    return TRUE ;
}

/*****************************************************************************/
//  Description : save mode
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  SaveModeEditWin(void)
{
//  int32 loop=0;
//  uint16      mode = 0;
    uint32 setlist_index=GUISETLIST_GetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID);

    switch(setlist_index)
    {
        case 0:
            s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
            break;
        case 1:
            s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
            break;
        case 2:
            s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
            /*if(0==s_current_event_info.event.event_fast_info.fre )//������һ��Ҳ��ѡ���������ġ�
            {
                for(loop=1;loop<ALM_WEEK_DAY_NUM-1;loop++)//ûѡ�Ļ�Ĭ��ѡ��һ~  ����
                {
                    mode |= s_week_mask[loop];
                }

                s_current_event_info.event.event_fast_info.fre = mode & 0x00ff;
            }*/
            break;
        default:
            s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
            break;
            
    }
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : set RingDur ctrl param for edit win
//	Global resource dependence : 
//  Author: panvs
//	Note:   
/*****************************************************************************/
LOCAL void SetRingDurParamEditWin(void)
{
    MMI_STRING_T text = {0};
    uint16 index = 0;
    uint32 i = 0;
    GUISETLIST_ITEM_INFO_T item_info = {0};
    wchar wstr[50] = {0};
    char   str[5][3]  = {{'1','\0','\0'},{'5','\0','\0'},{'1','0','\0'},{'1','5','\0'},{'3','0','\0'}};
    uint16 str_len  = 0;
        
    GUIFORM_SetStyle(MMIALM_EDIT_RING_DUR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_RING_DUR_TIME, &text);
    GUILABEL_SetText(MMIALM_EDIT_RING_DUR_LABLE_CTRL_ID, &text, FALSE);

    //set string
    SCI_MEMSET(&text, 0, sizeof(MMI_STRING_T));
    MMI_GetLabelTextByLang(TXT_TIME_MINUTE, &text);
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
    item_info.str_info.wstr_ptr = wstr;
    for(i=0; i<5; i++)
    {
        SCI_MEMSET(wstr, 0, sizeof(wstr));
        
        str_len = strlen(str[i]);
        MMI_STRNTOWSTR(wstr, 50, (uint8*)str[i], str_len, str_len);
        
        MMI_WSTRNCPY(&wstr[str_len], 50-str_len, text.wstr_ptr, text.wstr_len, text.wstr_len);
        item_info.str_info.wstr_len = text.wstr_len+str_len;
        
        GUISETLIST_AddItem(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID,&item_info);
    }
    
    switch(s_current_event_info.event.event_content_info.ring_dur)
    {
        case 1:
            index = 0;
            break;
        case 5:
            index = 1;
            break;
        case 10:
            index = 2;
            break;
        case 15:
            index = 3;
            break;
        case 30:
            index = 4;
            break;
        default:
            index = 0;
            break;
    }
    
    GUISETLIST_SetCurIndex(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID, index);    
    GUISETLIST_SetTitleTextId(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID, TXT_RING_DUR_TIME);
    GUISETLIST_SetFontAlign(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
}

/*****************************************************************************/
// 	Description : save RingDur
//	Global resource dependence : 
//  Author: panvs
//	Note:   
/*****************************************************************************/
LOCAL void  SaveRingDurEditWin(void)
{
    uint32 index=GUISETLIST_GetCurIndex(MMIALM_EDIT_RING_DUR_SETLIST_CTRL_ID);

    switch(index)
    {
        case 0:
            s_current_event_info.event.event_content_info.ring_dur = 1;
            break;
        case 1:
            s_current_event_info.event.event_content_info.ring_dur = 5;
            break;
        case 2:
            s_current_event_info.event.event_content_info.ring_dur = 10;
            break;
        case 3:
            s_current_event_info.event.event_content_info.ring_dur = 15;
            break;
        case 4:
            s_current_event_info.event.event_content_info.ring_dur = 30;
            break;
        default:
            s_current_event_info.event.event_content_info.ring_dur = 1;
            break;
    }
}
#endif

/*****************************************************************************/
//  Description : Append the item of weeks list
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
// LOCAL void AppendWeeksListItem(
//                                                MMI_CTRL_ID_T    ctrl_id //in
//                                                )
// {
//         uint8 day_num=0;
//     uint8   week_day =0;
//     GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
//        GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
// 
//     //add items to listbox
//     for(day_num = 0; day_num < ALM_WEEK_DAY_NUM; day_num++)   
//     {
//         week_day = MMIAPISET_GetWeekDayByOffset(day_num );
//         //MMIAPISET_AppendListItemByTextId(s_alm_week_text_id[ week_day ], STXT_SOFTKEY_OK_MK, ctrl_id, GUIITEM_STYLE_ONE_LINE_CHECK );
//     
//         item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
//         item_t.item_data_ptr = &item_data;
// 
//         item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
//         item_data.item_content[0].item_data.text_id = s_alm_week_text_id[ week_day ];
// 
//         //item_data.softkey_id[0] = STXT_SOFTKEY_OK_MK;
//         //item_data.softkey_id[1] = STXT_SELECT;
//         //item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
//     
//             GUILIST_AppendItem( ctrl_id, &item_t );  
// 
//         if ((s_current_event_info.event.event_fast_info.fre & s_week_mask[week_day]) != 0) 
//         {
//             GUILIST_SetSelectedItem(ctrl_id, day_num, TRUE);
//         }
//     }  
// }

/*****************************************************************************/
//  Description : save the select weeks
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
// LOCAL uint16 SaveSelectWeeks(
//                                                MMI_CTRL_ID_T    ctrl_id //in
//                                                )
// {
//     uint16      cur_selection[ALM_WEEK_DAY_NUM]   =  {0};
//     uint16      cur_num       =   0;
//     uint8   week_day =0;
//     int32 loop=0;
// //  uint32      i = 0; 
//     uint16      mode = 0;
//     
//     cur_num=GUILIST_GetSelectedItemIndex(ctrl_id, cur_selection, ALM_WEEK_DAY_NUM);
// 
//     if(0==cur_num)//������һ��Ҳ��ѡ�����򱣴�����ʱ������
//     {
//         return cur_num;
//     }
// 
//     for(loop=0;loop<cur_num;loop++)
//     {
//         week_day = MMIAPISET_GetWeekDayByOffset(cur_selection[loop]);
//         mode |= s_week_mask[week_day];
//     }
// 
//     if (mode != s_current_event_info.event.event_fast_info.fre) 
//     {
//         s_current_event_info.event.event_fast_info.fre = mode & 0x00ff;
//     }
//     
//     return cur_num;
// }
#if  defined DPHONE_SUPPORT || defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : form time str by time
//  Global resource dependence : 
//  Author:        zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void MMIALM_FormTimeStrbyTime(
                      uint8 hour,//in
                      uint8 min,//in
                      char* time_str,//out
                      uint16 time_str_max_len//in
                      )
{
    MMISET_TIME_DISPLAY_TYPE_E time_type = MMISET_TIME_12HOURS;

    //SCI_ASSERT(PNULL != time_str);  /*assert verified*/
    //SCI_ASSERT(time_str_max_len >= MMIALM_TIME_STR_12HOURS_LEN + 1);  /*assert verified*/
    if(PNULL == time_str || time_str_max_len < (MMIALM_TIME_STR_12HOURS_LEN + 1))
    {
        return;
    }
    
    time_type = MMIAPISET_GetTimeDisplayType();
    if (MMISET_TIME_12HOURS == time_type)
    {
        if ((12 > hour))//AM
        {
            if ((1 > hour))//00:00-1:00
            {
                hour = (uint8)(hour + 12);
            }
            sprintf( (char*)time_str,"%02d:%02dAM",hour,min);
        }
        else//PM
        {
            if ((12 <= hour) && (13 > hour))//12:00-13:00
            {
            }
            else
            {
                hour = (uint8)(hour - 12);
            }
            sprintf( (char*)time_str,"%02d:%02dPM",hour,min);
        }
    }
    else
    {
        sprintf( (char*)time_str, "%02d:%02d", hour, min );
    }
}
#endif

#ifdef DPHONE_SUPPORT
LOCAL void DisplayAlarmTime(void)
{
    uint8               time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]    = {0};

    MMI_STRING_T        text            = {0};

        //ʱ��      
        MMI_GetLabelTextByLang(TXT_TIME, &text);
        GUILABEL_SetText(MMIALM_EDIT_TITLE_LABEL_CTRL_ID, &text, FALSE);

        if (s_current_event_info.event.event_fast_info.is_valid )
        {
            MMIALM_FormTimeStrbyTime(s_current_event_info.event.event_fast_info.hour,
                s_current_event_info.event.event_fast_info.minute,
                (char*)time_str,
                MMIALM_TIME_STR_12HOURS_LEN+1
                );
        }
        else
        {
            SCI_TIME_T      sys_time = {0};
            TM_GetSysTime(&sys_time);
            MMIALM_FormTimeStrbyTime(sys_time.hour,
                sys_time.min,
                (char*)time_str,
                MMIALM_TIME_STR_12HOURS_LEN+1
                );
        }

        text.wstr_len = 10;
        text.wstr_ptr = SCI_ALLOC_APP((text.wstr_len +1)* sizeof(wchar));
        SCI_MEMSET(text.wstr_ptr,0,((text.wstr_len +1)* sizeof(wchar)));
        MMI_STRNTOWSTR(text.wstr_ptr,10,(uint8*)time_str,10,10);

        GUILABEL_SetText(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &text, FALSE);
        if (PNULL != text.wstr_ptr)
        {
            SCI_FREE(text.wstr_ptr);
            text.wstr_ptr = PNULL;
        }
   //     GUILABEL_SetBackgroundColor(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_WINDOW_BACKGROUND_COLOR);
        // end
}

LOCAL void DisplayAlarmRingType(void)
{
    MMI_STRING_T        text            = {0};

    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        // begin
        MMI_GetLabelTextByLang(TXT_RING, &text);
        GUILABEL_SetText(MMIALM_EDIT_RING_LABEL_CTRL_ID, &text, FALSE); // text

        // begin
        DisplayTheRingName();
    }
    else
    {
    
    }
}

LOCAL void DisplayAlarmMode(void)
{
    MMI_STRING_T        text            = {0};

    MMI_GetLabelTextByLang(TXT_ALARM_EDIT_MODE, &text);
    
    GUILABEL_SetText(MMIALM_EDIT_MODE_LABEL_CTRL_ID, &text, FALSE);
    

    if (s_current_event_info.event.event_fast_info.fre_mode == ALM_MODE_ONCE)
        {
                MMI_GetLabelTextByLang(TXT_ALARM_MODE_ONCE, &text);
        }
    else if (s_current_event_info.event.event_fast_info.fre_mode == ALM_MODE_EVERYDAY)
        {
                MMI_GetLabelTextByLang(TXT_ALARM_MODE_EVERYDAY, &text);
        }
    else  if (s_current_event_info.event.event_fast_info.fre_mode == ALM_MODE_EVERYWEEK)
        {
                MMI_GetLabelTextByLang(TXT_ALARM_MODE_WEEK, &text);
        }  

     GUILABEL_SetText(MMIALM_MODELIST_CTRL_ID, &text, FALSE);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:zhangwp
//  Note:
/*****************************************************************************/
LOCAL void AppendAlarmModeListBoxItem(
                                               MMI_CTRL_ID_T    ctrl_id,
                                               MMIACC_SMART_EVENT_FAST_TABLE_T    *event_ptr  //in
                                               )
{
//    int32               i               =   0;

    //add items to listbox
    MMIAPISET_AppendListItemByTextId(TXT_ALARM_MODE_ONCE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_ALARM_MODE_EVERYDAY, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_ALARM_MODE_WEEK, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
   
    switch(event_ptr->fre_mode)
    {
    case ALM_MODE_ONCE:
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_ONCE, TRUE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYDAY, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYWEEK, FALSE);
        break;

    case ALM_MODE_EVERYDAY:
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_ONCE, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYDAY, TRUE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYWEEK, FALSE);
        break;

    case ALM_MODE_EVERYWEEK:
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_ONCE, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYDAY, FALSE);
        GUILIST_SetSelectedItem(ctrl_id, ALM_MODE_EVERYWEEK, TRUE);
        break;
        
    default:
        break;
    }  
    GUILIST_SetCurItemIndex(ctrl_id,event_ptr->fre_mode); //add by sandywan
}


/*****************************************************************************/
//  Description :set startup mode(sunday to saturday ) of alarm 
//  Global resource dependence : 
//  Author: zhangwp
//  Note: 
/*****************************************************************************/
LOCAL void AppendAlarmWeekDayListBoxItem(
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    uint16      max_item = ALM_MODE_LIST;
    uint16      i = 0;

    //set list max item and selected max item
    GUILIST_SetMaxItem(ctrl_id,max_item,FALSE);
    GUILIST_SetMaxSelectedItem(ctrl_id,max_item);

    //append item
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[0],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[1],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[2],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[3],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[4],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[5],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    MMIAPISET_AppendListItemByTextId(s_mode_week_list_arr[6],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);

    for (i=0; i<max_item;i++)
    {
        if (0x00000001 == ((s_current_event_info.event.event_fast_info.fre>>i) & 0x00000001))
        {
            //set selected item
            GUILIST_SetSelectedItem(ctrl_id,i,TRUE);
        }
    }
}


/*****************************************************************************/
//  Description : set time of alarm and set status to ON and save to EFS
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmTimeWinMsg(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   current_ctrl_id = MMIALM_SETTIME_CTRL_ID;
    MMISET_TIME_T   time = {0};
    GUIEDIT_TIME_STYLE_E        time_style = GUIEDIT_TIME_STYLE_NULL;
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:  
        time_type = MMIAPISET_GetTimeDisplayType();
        if (MMISET_TIME_12HOURS == time_type)
        {
            time_style = GUIEDIT_TIME_STYLE_12;
        }
        else
        {
            time_style = GUIEDIT_TIME_STYLE_24;
        }
        
        //set time display style
        GUIEDIT_SetTimeStyle(current_ctrl_id,PNULL,&time_style,PNULL,FALSE);
        GUIEDIT_SetFont(current_ctrl_id, MMI_SET_TIME_FONT);
        if (s_current_event_info.event.event_fast_info.is_valid )
        {
            GUIEDIT_SetTime(
            current_ctrl_id,
            s_current_event_info.event.event_fast_info.hour,
            s_current_event_info.event.event_fast_info.minute,
            0);
        }
        else
        {
            SCI_TIME_T      sys_time = {0};
            TM_GetSysTime(&sys_time);
            GUIEDIT_SetTime(
            current_ctrl_id,
            sys_time.hour,
            sys_time.min,
            0); 
        }
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);

        //SCI_ASSERT(s_current_event_info.id <= EVENT_AUTO_POWEROFF); /*assert verified*/
        GUIWIN_SetTitleTextId(win_id, TXT_TIME, FALSE); // for cr40327
        break;

        case MSG_FULL_PAINT:
        break;

        case MSG_CTL_OK:        // completed
        case MSG_APP_OK:       
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        GUIEDIT_GetTime(current_ctrl_id, &time.time_hour, &time.time_minute, PNULL);
        if ( MMIAPICOM_CheckTimeIsValid(time))
        {
            s_current_event_info.event.event_fast_info.hour = time.time_hour; 
            s_current_event_info.event.event_fast_info.minute = time.time_minute;
            MMK_CloseWin(win_id);
    
            s_current_event_info.event.event_fast_info.is_on = TRUE;
            // �ر���ʱ����
            SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
            // ��ʾ����
            //@zhaohui,cr108847
            if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
            {
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_MEMORY_FULL);
            }   
            break;
        
        }
        else   // time invalid
        {
            MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
        }
        break;

   
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

// set alarm ON after set time
WINDOW_TABLE( ALARM_TIMEWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmTimeWinMsg ),
    WIN_ID( MMIALM_TIMEWIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),

   /* CREATE_EDIT_TIME_CTRL(
                    0, MMI_CLIENT_RECT_TOP, 
                    MMI_MAINSCREEN_RIGHT_MAX_PIXEL,
                    MMI_CLIENT_RECT_BOTTOM,
                    MMIALM_SETTIME_CTRL_ID
                    ), */
    CREATE_EDIT_TIME_CTRL(MMIALM_SETTIME_CTRL_ID), 
     END_WIN
}; 
/*****************************************************************************/
//  Description :set startup mode(sunday to saturday ) of alarm and save to EFS
//  Global resource dependence : 
//  Author:        ZHANGWP
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmWeekDayModeWinMsg(
                                                  MMI_WIN_ID_T      win_id, 
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  DPARAM            param
                                                  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    uint16              sel_items[ALM_MODE_LIST] = {0};
    uint16              sel_count = 0;
    MMI_CTRL_ID_T       current_ctrl_id = MMIALM_MODSEL_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:      //create check list box
            AppendAlarmWeekDayListBoxItem(current_ctrl_id);
            if (0 == s_current_event_info.event.event_fast_info.fre)
            {
                s_current_event_info.event.event_fast_info.fre = 0x3e;
            }

            // read info of alarm , set check flag to indicate if the alarm is setting
            GUILIST_SetMaxSelectedItem(current_ctrl_id, ALM_MODE_LIST);
            MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;

    case MSG_CTL_CANCEL:  //MSG_CTL_OK:     
            sel_count = GUILIST_GetSelectedItemIndex(current_ctrl_id,sel_items, ALM_MODE_LIST);
            //  save current settings of ferquency to EFS
            if (0 == sel_count)         // no day selected
            {
                MMIPUB_OpenAlertWarningWin(TXT_MUST_SELECT_ONE);
            }
            else
            {
                UpdateAlarmMode(sel_items, s_current_event_info.event.event_fast_info.fre, sel_count);
                s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

                //      MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 
                MMK_CloseWin(win_id);
            }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

WINDOW_TABLE( ALARM_WEEKDAYMODEWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmWeekDayModeWinMsg ),    
    WIN_ID( MMIALM_WEEKDAYMODEWIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E,MMIALM_MODSEL_CTRL_ID),
     END_WIN
}; 

/*****************************************************************************/
//  Description :set startup mode(frequency)
//  Global resource dependence : 
//  Author:        zhangwp
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmModeWinMsg(
                                          MMI_WIN_ID_T win_id,  
                                          MMI_MESSAGE_ID_E msg_id,      
                                          DPARAM param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    uint16                  item_id = 0;


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIALM_MODELIST_CTRL_ID, 3, FALSE);//max item 3
        AppendAlarmModeListBoxItem(MMIALM_MODELIST_CTRL_ID,&s_current_event_info.event.event_fast_info);
        MMK_SetAtvCtrl(win_id,MMIALM_MODELIST_CTRL_ID);
        break;

    case MSG_NOTIFY_MIDSK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:        
        item_id = GUILIST_GetCurItemIndex(MMIALM_MODELIST_CTRL_ID);
        s_current_event_info.event.event_fast_info.fre_mode = (MMIACC_MODE_TYPE_E)item_id;
        switch( item_id)
        {
            case ALM_MODE_ONCE:
                s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
                s_current_event_info.event.event_fast_info.fre = MASK_B7; //meanse only once
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

           //     MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                MMK_CloseWin(win_id);
                break;

            case ALM_MODE_EVERYDAY:
                s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
                s_current_event_info.event.event_fast_info.fre = 0x7f; 
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

            //    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                MMK_CloseWin(win_id);
                break;

            case ALM_MODE_EVERYWEEK:
                MMK_CreateWin((uint32*)ALARM_WEEKDAYMODEWIN_TAB, PNULL);
                MMK_CloseWin(win_id);
                break;

            default:
                break;
        }
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;            
}

WINDOW_TABLE( ALARM_MODEWIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleAlarmModeWinMsg ),    
    WIN_ID( MMIALM_MODEWIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIALM_MODELIST_CTRL_ID ),
    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
}; 



/*****************************************************************************/
//  Description : EditWinDrawFocusRect 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void EditWinDrawFocusRect(
                                GUI_RECT_T rect,
                                GUI_COLOR_T color
                                )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);

    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);
}


/*****************************************************************************/
//  Description : EditWinCleanFocus
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void EditWinCleanFocus(
                             uint8          index,
                             MMI_WIN_ID_T   win_id
                             )
{
 //   uint8            i            = 0;
//    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

//    MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);
    
    switch(index) 
    {
    case 0:  //ring time
        {
            if(EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
            {
          //      IGUICTRL_T *ctrl_ptr = NULL;

             //   GUILABEL_GetRect(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,&rect);
         //       ctrl_ptr = MMK_GetCtrlPtr(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
           /*        GUIRES_DisplayImg(PNULL,
                &rect,
                &rect,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);*/
                //                GUILABEL_SetTextScroll(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, FALSE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_WHITE_COLOR/*����ɫ���*/);
            GUILABEL_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
#else
            //  LCD_FillRect(&lcd_dev_info, focus_rec[0], MMI_WHITE_COLOR);
            //     GUILABEL_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
            EditWinDrawFocusRect(focus_rec[0], ALM_EDIT_CLEAN_LINE_COLOR); 
#endif

              //  IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT,NULL);
            }
        }
        break;
    
    case 1: //ring type
        {
         //   IGUICTRL_T *ctrl_ptr = NULL;

        //    ctrl_ptr = MMK_GetCtrlPtr(MMIALM_EDIT_RING_TEXT_CTRL_ID);
          /*       GUIRES_DisplayImg(PNULL,
            &rect,
            &rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);*/
            //            GUILABEL_SetTextScroll(MMIALM_EDIT_RING_TEXT_CTRL_ID, FALSE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_EDIT_RING_TEXT_CTRL_ID, MMI_WHITE_COLOR/*����ɫ���*/);
            GUILABEL_SetFont(MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);

#else
            //  LCD_FillRect(&lcd_dev_info, focus_rec[1], MMI_WHITE_COLOR);
            //GUILABEL_SetFont(MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);

            EditWinDrawFocusRect(focus_rec[1], ALM_EDIT_CLEAN_LINE_COLOR); 
#endif

         //   IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT, NULL);
        }
        break;

    case 2: //alarm mode
        {
            if(EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
            {
            //    IGUICTRL_T *ctrl_ptr = NULL;

                //       ctrl_ptr = MMK_GetCtrlPtr(MMIALM_MODELIST_CTRL_ID);

                /*    GUIRES_DisplayImg(PNULL,
                &rect,
                &rect,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);*/
                //                GUILABEL_SetTextScroll(MMIALM_MODELIST_CTRL_ID, FALSE);
#if 0
                GUILABEL_SetBackgroundColor(MMIALM_MODELIST_CTRL_ID, MMI_WHITE_COLOR/*����ɫ���*/);
                GUILABEL_SetFont(MMIALM_MODELIST_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
#else
                //      LCD_FillRect(&lcd_dev_info, focus_rec[2], MMI_WHITE_COLOR);
                //      GUILABEL_SetFont(MMIALM_MODELIST_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_BLACK_COLOR);
                EditWinDrawFocusRect(focus_rec[2], ALM_EDIT_CLEAN_LINE_COLOR); 
#endif
                //   IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT, NULL);
            }
        }
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : EditWinDrawFocus 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void EditWinDrawFocus(
                            uint8           index,
                            MMI_WIN_ID_T    win_id
                            )
{
    //ʱ�䣬����and alarm mode �������Ҫ�а�ɫ�߿�
    MMI_TEXT_ID_T   middlesoft_id   = TXT_NULL;
//    GUI_RECT_T rect = {ALM_EDIT_X2-1, ALM_EDIT_Y1+1, ALM_EDIT_X3, ALM_EDIT_Y1 + ALM_EDIT_HIGHT};

//    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    #if 0 // if  make a frame for every item, need add this part;
    EditWinDrawFocusRect(rect, ALM_EDIT_FOCUS_LINE_COLOR); /*this is for ring time.*/

    if(EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
    rect.top = ALM_EDIT_Y0 - 1;
    rect.bottom = ALM_EDIT_Y0 + ALM_EDIT_HIGHT;
    EditWinDrawFocusRect(rect, ALM_EDIT_FOCUS_LINE_COLOR);//this is for alarm name, don't care.
  
    rect.top = ALM_EDIT_Y2;
    rect.bottom = ALM_EDIT_Y2 + ALM_EDIT_HIGHT ;
    EditWinDrawFocusRect(rect, ALM_EDIT_FOCUS_LINE_COLOR);//this is for ring type

    rect.top = ALM_EDIT_Y3;
    rect.bottom = ALM_EDIT_Y3 + ALM_EDIT_HIGHT ;
    EditWinDrawFocusRect(rect, ALM_EDIT_FOCUS_LINE_COLOR);//this is for alarm mode.
    }
#endif

    switch(index) 
    {
        case 0: //alarm time
        {
            //   IGUICTRL_T *ctrl_ptr = NULL;

            //    ctrl_ptr = MMK_GetCtrlPtr(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
            //            GUILABEL_SetTextScroll(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, TRUE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_BLACK_COLOR/*����ɫ���*/);
            GUILABEL_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);

#else
            //    LCD_FillRect(&lcd_dev_info, focus_rec[0], MMI_BLACK_COLOR);
            //     GUILABEL_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);

            EditWinDrawFocusRect(focus_rec[0], ALM_EDIT_FOCUS_LINE_COLOR); 
#endif
            //     IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT,NULL);
            middlesoft_id = TXT_EDIT; 
        }
        break;

        case 1: // alarm ring
        {
            if(EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
            {
            //    IGUICTRL_T *ctrl_ptr = NULL;

            //  ctrl_ptr = MMK_GetCtrlPtr(MMIALM_EDIT_RING_TEXT_CTRL_ID);
            //                GUILABEL_SetTextScroll(MMIALM_EDIT_RING_TEXT_CTRL_ID, TRUE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_EDIT_RING_TEXT_CTRL_ID, MMI_BLACK_COLOR/*����ɫ���*/);
            GUILABEL_SetFont(MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);
#else
            //  LCD_FillRect(&lcd_dev_info, focus_rec[1], MMI_BLACK_COLOR);
            //  GUILABEL_SetFont(MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);

            EditWinDrawFocusRect(focus_rec[1], ALM_EDIT_FOCUS_LINE_COLOR); 
#endif

            //    IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT,NULL);
            middlesoft_id = STXT_SOFTKEY_SELECT_MK;        
            }
        }
        break;

        case 2: //alarm mode
        {
         //  IGUICTRL_T *ctrl_ptr = NULL;
                    
        //   ctrl_ptr = MMK_GetCtrlPtr(MMIALM_MODELIST_CTRL_ID);
            //                GUILABEL_SetTextScroll(MMIALM_MODELIST_CTRL_ID, TRUE);
#if 0
            GUILABEL_SetBackgroundColor(MMIALM_MODELIST_CTRL_ID, MMI_BLACK_COLOR/*����ɫ���*/);
            GUILABEL_SetFont(MMIALM_MODELIST_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);
#else
       //     LCD_FillRect(&lcd_dev_info, focus_rec[2], MMI_BLACK_COLOR);
        //    GUILABEL_SetFont(MMIALM_MODELIST_CTRL_ID,MMI_DEFAULT_TEXT_FONT, MMI_WHITE_COLOR);
            EditWinDrawFocusRect(focus_rec[2], ALM_EDIT_FOCUS_LINE_COLOR); 
#endif

          //  IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT,NULL);

        middlesoft_id = STXT_SOFTKEY_SELECT_MK;        
        }
        break;

        default:
        break;
    }
//���������
GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, middlesoft_id, TXT_COMMON_COMMON_BACK, TRUE);
}


/*****************************************************************************/
//  Description : EditWinKeyWebDown 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void EditWinKeyWebDown(
                             MMI_MESSAGE_ID_E   msg_id,
                             MMI_WIN_ID_T       win_id
                             )
{
//    uint8 index = 0;

    switch(s_edit_active_ctrl)
    {   
    case 0: // time
        // s_edit_active_ctrl = 0;
        if (MSG_KEYDOWN_WEB == msg_id)
        {
            MMK_CreateWin((uint32*)ALARM_TIMEWIN_TAB, PNULL);
        }
        break;
        
    case 1: // ring type
        //s_edit_active_ctrl = 1;
        if (MSG_KEYDOWN_WEB == msg_id)
        {
#ifdef DPHONE_SUPPORT
            MMK_CreateWin((uint32 *)ALARM_FIXEDRINGWIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32 *)ALARM_RINGWIN_TAB, PNULL);//@zhaohui,cr108296
#endif
        }
        break;

    case 2: // alarm mode
        //s_edit_active_ctrl = 2;
        if (MSG_KEYDOWN_WEB == msg_id)
        {
        MMK_CreateWin((uint32 *)ALARM_MODEWIN_TAB, PNULL);
        }
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : HandleAlarmEditCtrlId ����༭���ڵĿؼ�
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL void HandleAlarmEditCtrlId(
                                 MMI_MESSAGE_ID_E   msg_id,
                                 MMI_WIN_ID_T       win_id
                                 )
{
    switch(s_edit_active_ctrl)
    {
    case 0: //time
        //s_edit_active_ctrl = 0;
        if (MSG_KEYUP_WEB != msg_id)
        {
            MMK_CreateWin((uint32*)ALARM_TIMEWIN_TAB, PNULL);
        }
        break;
    
    case 1: // ring type
        //s_edit_active_ctrl = 1;
        if (MSG_KEYUP_WEB != msg_id)
        {
#ifdef DPHONE_SUPPORT
            MMK_CreateWin((uint32 *)ALARM_FIXEDRINGWIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32 *)ALARM_RINGWIN_TAB, PNULL);//@zhaohui,cr108296
#endif
        }
        break;
        
    case 2: // alarm mode
        //s_edit_active_ctrl = 2;
        if (MSG_KEYUP_WEB != msg_id)
        {
        MMK_CreateWin((uint32 *)ALARM_MODEWIN_TAB, PNULL);
        }
        break;

    default:
        break;
    }

}

LOCAL void EditWinKeyDown(MMI_WIN_ID_T      win_id)
{
        EditWinCleanFocus(s_edit_active_ctrl,win_id);

        if(s_edit_active_ctrl == 2)
        {
            s_edit_active_ctrl = 0;
        }
        else
        {
            s_edit_active_ctrl++;
        }

        EditWinDrawFocus(s_edit_active_ctrl,win_id);   
        MMK_SetAtvCtrlEx(win_id, s_edit_active_ctrl_attr[s_edit_active_ctrl], FALSE);
        //   MMK_SendMsg( s_edit_active_ctrl_attr[s_edit_active_ctrl], MSG_CTL_PAINT, 0 );
}

LOCAL void EditWinKeyUp(MMI_WIN_ID_T      win_id)
{
        EditWinCleanFocus(s_edit_active_ctrl,win_id);

        if(s_edit_active_ctrl == 0)
        {
            s_edit_active_ctrl = 2;
        }
        else
        {
            s_edit_active_ctrl--;
        }

        EditWinDrawFocus(s_edit_active_ctrl,win_id);    
        MMK_SetAtvCtrlEx(win_id, s_edit_active_ctrl_attr[s_edit_active_ctrl], FALSE);
        //MMK_SendMsg( s_edit_active_ctrl_attr[s_edit_active_ctrl], MSG_CTL_PAINT, 0 );
      
}

/*****************************************************************************/
//  Description : handle alarm edit windows messages
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    GUI_RECT_T          bk_rect         = MMI_CLIENT_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_RESULT_E        recode          = MMI_RESULT_TRUE;

     MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
            {
            GUI_RECT_T rect = {0};
            
// 1st line--alarm time
            rect.left = ALM_EDIT_X1;
            rect.top = ALM_EDIT_Y1;
            rect.right = ALM_EDIT_X2 - 2;
            rect.bottom = ALM_EDIT_Y1 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_TITLE_LABEL_CTRL_ID, &rect, FALSE);
            rect.left = ALM_EDIT_X2;
            rect.top = ALM_EDIT_Y1;
            rect.right = ALM_EDIT_X3;
            rect.bottom = ALM_EDIT_Y1 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &rect, FALSE);
// second line--ring type          
            rect.left = ALM_EDIT_X1;
            rect.top = ALM_EDIT_Y2;
            rect.right = ALM_EDIT_X2 - 2;
            rect.bottom = ALM_EDIT_Y2 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_RING_LABEL_CTRL_ID, &rect, FALSE);
            rect.left = ALM_EDIT_X2;
            rect.top = ALM_EDIT_Y2;
            rect.right = ALM_EDIT_X3;
            rect.bottom = ALM_EDIT_Y2 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_RING_TEXT_CTRL_ID, &rect, FALSE);

// 3rd line--alarm mode
            rect.left = ALM_EDIT_X1;
            rect.top = ALM_EDIT_Y3;
            rect.right = ALM_EDIT_X2 - 2;
            rect.bottom = ALM_EDIT_Y3 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_EDIT_MODE_LABEL_CTRL_ID, &rect, FALSE);
            rect.left = ALM_EDIT_X2;
            rect.top = ALM_EDIT_Y3;
            rect.right = ALM_EDIT_X3;
            rect.bottom = ALM_EDIT_Y3 + ALM_EDIT_HIGHT - 1;
            GUILABEL_SetRect(MMIALM_MODELIST_CTRL_ID, &rect, FALSE);

            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK, FALSE);

            MMK_SetAtvCtrl(win_id, s_edit_active_ctrl_attr[s_edit_active_ctrl]);
            }
        break;

    case MSG_FULL_PAINT:
      //  IMG_EnableTransparentColor(TRUE);
        GUIRES_DisplayImg(PNULL,
            &bk_rect,
            &bk_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        //SCI_ASSERT((s_current_event_info.id <= EVENT_ALARM_DELAY_2)||(s_current_event_info.id == EVENT_AUTO_POWERON)||(s_current_event_info.id == EVENT_AUTO_POWEROFF)); /*assert verified*/
     
        DisplayAlarmTime();
        DisplayAlarmRingType();
    //    IMG_EnableTransparentColor(FALSE); 
        DisplayAlarmMode();
        EditWinDrawFocus(s_edit_active_ctrl,win_id);
        break;

    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
        EditWinKeyWebDown(msg_id,win_id);
        HandleAlarmEditCtrlId(msg_id,win_id);
        break;

    case MSG_KEYDOWN_UP:
        EditWinKeyUp(win_id);
        break;
        
    case MSG_KEYDOWN_DOWN:
           EditWinKeyDown(win_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        s_current_event_info.event.event_fast_info.is_on = TRUE;
#ifndef WIN32
        //SCI_TRACE_LOW:"%s  %d  Event %d is set to on!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_8646_112_2_18_2_1_8_44,(uint8*)"sdd",__func__,__LINE__,s_current_event_info.id);
#endif
        // �ر���ʱ����
        SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
        // ��ʾ����
        //@zhaohui,cr108847
        if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
        {
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_MEMORY_FULL);
        }   
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        s_edit_active_ctrl = 0;
        MMK_CloseWin(MMIAUTOPOWER_OPTWIN_ID);
        break;
        
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
              }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

#else
/*****************************************************************************/
//  Description : handle alarm edit windows messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id =MMIALM_EDIT_MODE_SETLIST_CTRL_ID;
//#ifdef MMI_PDA_SUPPORT
    uint16                  img_width  = 0;
    uint16                  img_height = 0;
    GUIFORM_CHILD_WIDTH_T   button_width = {0};
    GUIFORM_CHILD_HEIGHT_T  button_height = {0};
#ifdef MMI_RECORD_SUPPORT
    GUI_BG_T bg={0};
#endif
    BOOLEAN is_need_save = TRUE;
//#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_PDA_SUPPORT
        GUIFORM_SetCircularHandleUpDown(MMI_EDITWIN_FORM_CTRL_ID,TRUE);
#endif
        GUIFORM_SetType(MMI_EDITWIN_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        GUISETLIST_SetFontAlign(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
        //GUIEDIT_SetAlign(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);
        GUIFORM_SetStyle(MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIAPICTRL_SetState( MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, GUICTRL_STATE_DISABLE_TP,TRUE ); 
        //NEWMS00222350
        GUIEDIT_SetHandleRedKey(FALSE, MMIALM_EDIT_NAME_EDITBOX_CTRL_ID);

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DROPDOWNLIST_ARROW_PR, win_id);
        button_width.add_data = img_width;
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        button_height.add_data = img_height;
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        GUIFORM_SetChildWidth(MMIALM_EDIT_MODE_FORM_CTRL_ID, MMIALM_EDIT_MODE_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMIALM_EDIT_MODE_FORM_CTRL_ID, MMIALM_EDIT_MODE_BUTTON_CTRL_ID, &button_height);
#ifndef MMI_RECORD_SUPPORT
        GUIFORM_SetChildWidth(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_height);
#else
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_SCH_RECORD_ICON, win_id);
        button_width.add_data = img_width;
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        button_height.add_data = img_height;
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        GUIFORM_SetChildWidth(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_height);
      //  GUIBUTTON_SetCallBackFunc(MMIALM_EDIT_RING_BUTTON_CTRL_ID, RecordAlarmRingCallBack);
        GUIBUTTON_SetRunSheen(MMIALM_EDIT_RING_BUTTON_CTRL_ID,FALSE);
        bg.bg_type=GUI_BG_IMG;
        bg.img_id=IMG_SCH_RECORD_ICON;
        GUIBUTTON_SetPressedBg(MMIALM_EDIT_RING_BUTTON_CTRL_ID,&bg);
#endif


        SetTitleEditWin(win_id) ;
        AddItemSetlist();
#ifndef MMI_PDA_SUPPORT
        //GUIFORM_SetType(MMI_EDITWIN_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        SetNameParamEditWin();
        SetTimeParamEditWin();
        GUIEDIT_SetAlign(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
        SetModeParamEditWin();
        SetWeeksParamEditWin(win_id); 
        SetWeeksDisplayEditWin();
#ifdef CMCC_UI_STYLE
        SetRingDurParamEditWin();
        SetVibrateParamEditWin();
#endif

#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK, FALSE);
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK, FALSE);
#endif
#endif
        if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF!= s_current_event_info.id)
        {
            #ifndef MMI_PDA_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_NAME_EDITBOX_CTRL_ID);
            #else
            MMK_SetAtvCtrl(win_id, MMI_EDITWIN_FORM_CTRL_ID);
            #endif
        }
        else
        {
            #ifndef MMI_PDA_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
            #else
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TIME_FORM_CTRL_ID);
            #endif
            GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
            GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
        }
#if defined(MMI_ALARM_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)
#ifdef TOUCH_PANEL_SUPPORT
        GUIEDIT_SetIm(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,
                      ((GUIIM_TYPE_T)GUIIM_TYPE_ENGLISH|(GUIIM_TYPE_T)GUIIM_TYPE_ABC|(GUIIM_TYPE_T)GUIIM_TYPE_DIGITAL|(GUIIM_TYPE_T)GUIIM_TYPE_SMART),
                      GUIIM_TYPE_ABC);
       // GUIEDIT_SetImInput(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, (GUIIM_INPUT_TYPE_T)GUIIM_INPUT_TYPE_QWERT, (GUIIM_INPUT_TYPE_T)GUIIM_INPUT_TYPE_ALL);
        //ȥ��ԭ��:�ؼ�ȥ�����������
#endif
#endif
        break;

    case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
         if(ALM_MODE_EVERYWEEK==GUISETLIST_GetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID))
         {
            wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
            MMI_STRING_T str={0};
            str.wstr_len=MMIAPIALARM_GetFreModeString(ALM_MODE_EVERYWEEK,s_current_event_info.event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
            str.wstr_ptr=buff;
            GUISETLIST_SetDispText(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,&str,FALSE);
         }
        SetNameParamEditWin();
        SetTimeParamEditWin();
#endif
        SetRingParamEditWin();
        break;
    case MSG_GET_FOCUS:
        break;

    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
        //EditWinMIDSKProcess(win_id);
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        HandleAlarmEditPENOKMsg(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        HandleAlarmEditPENOKMsg(win_id,ctrl_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
         ctrl_id = MMK_GetActiveCtrlId(win_id);
         HandleAlarmEditFormSwitchActiveMsg(win_id,ctrl_id);
         break;

#ifdef MMI_RECORD_SUPPORT
	case MMI_MSG_RECORDTORING:
		if(PNULL != param)
		{       
			MMI_STRING_T        ring_name = {0};
			
            ring_name.wstr_ptr = (wchar*)param;
            ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
            SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr,0,(MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
            s_current_event_info.event.event_ring_info.dring_name_len = ring_name.wstr_len;
            MMIAPICOM_Wstrncpy(s_current_event_info.event.event_ring_info.dring_name_arr, ring_name.wstr_ptr, ring_name.wstr_len);
            SCI_MEMSET(ring_name.wstr_ptr,0,ring_name.wstr_len * sizeof(wchar));
            
            MMIAPIFMM_SplitFullPath(s_current_event_info.event.event_ring_info.dring_name_arr, s_current_event_info.event.event_ring_info.dring_name_len, PNULL, PNULL, PNULL, PNULL, ring_name.wstr_ptr, &ring_name.wstr_len);

            s_current_event_info.event.event_content_info.ring_type = ALM_RECORD_RING;
		}        
        DisplayTheRingName();
        MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
	    break;
#endif
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
		//CR177677:�ж���һ���Ƿ���������������ʱ���Ƿ�һ�£���һ�£�������ʾ��
        if (EVENT_AUTO_POWERON == s_current_event_info.id)
		{
			MMIACC_SMART_EVENT_FAST_TABLE_T   *poweroff_event = {0};
			MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
			poweroff_event = MMIALM_GeEvent(EVENT_AUTO_POWEROFF);
			if (poweroff_event->is_on)
			{//������
				MMISET_TIME_T   time = {0};
#ifdef  MMI_PDA_SUPPORT
                time.time_hour = s_current_event_info.event.event_fast_info.hour;
				time.time_minute = s_current_event_info.event.event_fast_info.minute;
#else
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
				MMIALARM_GetTime(&time.time_hour, &time.time_minute);
#else	
				GUIEDIT_GetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
#endif
#endif
				if((poweroff_event->minute == time.time_minute)
					&&(poweroff_event->hour == time.time_hour)
                    && (0 != (GetFreByEditWin() & poweroff_event->fre)))
				{
					MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
					is_need_save = FALSE;
				}
			}
		}
		else if(EVENT_AUTO_POWEROFF == s_current_event_info.id)
		{
			MMIACC_SMART_EVENT_FAST_TABLE_T   *poweron_event = {0};
			MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
			poweron_event = MMIALM_GeEvent(EVENT_AUTO_POWERON);
			if (poweron_event->is_on)
			{//������
				MMISET_TIME_T   time = {0};
#ifdef  MMI_PDA_SUPPORT
                time.time_hour = s_current_event_info.event.event_fast_info.hour;
				time.time_minute = s_current_event_info.event.event_fast_info.minute;
#else				
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
				MMIALARM_GetTime(&time.time_hour, &time.time_minute);
#else	
				GUIEDIT_GetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
#endif
#endif
				if((poweron_event->minute == time.time_minute)
					&&(poweron_event->hour == time.time_hour)
                    && (0 != (GetFreByEditWin() & poweron_event->fre)))
				{
					MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
				    is_need_save = FALSE;
				}
			}
		}
		if(is_need_save)
		{
#ifndef  MMI_PDA_SUPPORT
        //save name
        if (EVENT_AUTO_POWERON != s_current_event_info.id 
            && EVENT_AUTO_POWEROFF!= s_current_event_info.id)
        {
            if(!SaveNameEditWin())
            {
                break;
            }
        }
        //save time
        if(!SaveTimeEditWin())
        {
            break;
        }
#endif
        //save mode
        SaveModeEditWin();

        if(s_current_event_info.event.event_fast_info.fre_mode == ALM_MODE_EVERYWEEK)
        {
            if(!SaveFreEditWin())
            {
                break;
            }
        }

#ifdef CMCC_UI_STYLE
        SaveRingDurEditWin();
        //save vibrate
        if(0 == GUISETLIST_GetCurIndex(MMIALM_EDIT_VIBRATE_SWICTH_CTRL_ID))
        {
            s_current_event_info.event.event_content_info.is_vibrate = FALSE;
        }
        else
        {
            s_current_event_info.event.event_content_info.is_vibrate = TRUE;
        }
#endif

        s_current_event_info.event.event_fast_info.is_on = TRUE;
        // �ر���ʱ����
        SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
        // ��ʾ����
        //@zhaohui,cr108847
        if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
        {
#ifdef FM_SUPPORT
            if(s_current_event_info.event.event_content_info.ring_type!=ALM_FM_RING)
            {
#if 0 //#ifndef MMI_PDA_SUPPORT
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif
            }
            else
#endif
            {
 #ifdef MMI_FM_NEED_HEADSET
                MMIPUB_OpenAlertWarningWin(TXT_FM_TIMER_SAVE_PROMPT);
 #endif
            }
        }
        else
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
            MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        }   
        MMK_CloseWin(win_id);
		}
            break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
    break;

        case MSG_CLOSE_WINDOW:
        MMK_CloseWin(MMIAUTOPOWER_OPTWIN_ID);
        break;
        
    case MSG_CTL_SETLIST_SWITCH:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        if(MMIALM_EDIT_MODE_SETLIST_CTRL_ID==ctrl_id)
        {
            SetWeeksDisplayEditWin();
        }
    
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
#endif

/*****************************************************************************/
//  Description : save current frequency setting
//  Global resource dependence :
//  Author:        zhangwp
//  Note:
/*****************************************************************************/
LOCAL void UpdateAlarmMode(
                           uint16   *mode_ptr,
                           uint16   old,
                           uint16   total
                           )
{
    uint32      i = 0;
    uint16      mode = 0;

    //SCI_ASSERT(PNULL != mode_ptr); /*assert verified*/
    //SCI_ASSERT(total<=ALM_MODE_LIST); /*assert verified*/
    if(PNULL == mode_ptr || total > ALM_MODE_LIST)
    {
        return;
    }

    mode = 0;
    for (i=0; i<total; i++)
    {
        mode |= /*s_week_mask*/s_week_mask_frm_mon[mode_ptr[i]];
    }

    if (mode != old)
    {
        s_current_event_info.event.event_fast_info.fre = mode & 0x00ff;
    }
}

/*****************************************************************************/
//  Description : if alarm alive win is opened
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsAliveWinOpened(void)
{
    if (MMK_IsOpenWin(MMIALM_ALIVEWIN_ID))
    {
    return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : append two line check item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoLineListItem(   
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id_1,
                            MMI_TEXT_ID_T    text_id_2,
                            uint16           event_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    MMI_TEXT_ID_T           txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10,
                                                TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                                TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                                TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};
    MMIACC_SMART_EVENT_T    smart_event = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint8 index=0;
    uint8 list_sk_index = 0;

#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
#endif
//���ø�����list item�е�λ�ã�����䶯;
//��Ӧ����:icon/txt_alarm_id/alarm_mode/event_time
#if  !defined MMI_GUI_STYLE_TYPICAL
#ifdef MMI_ALARM_MINI_SUPPORT
#if defined(MAINLCD_SIZE_128X160) && defined(MULTI_LAN_SUPPORT_DE)
    uint8 indexArray[4]= {3, 0, 2, 1};
#else
    uint8 indexArray[4]= {1, 0, 3, 2};
#endif
#else
    uint8 indexArray[4]= {1, 2, 3, 0};
#endif
#else
    uint8 indexArray[4]= {0, 1, 3, 2};
#endif
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = GetDelayEvent( event_id );
    //SCI_ASSERT(PNULL!=event_delay_ptr);  /*assert verified*/    
    if(PNULL == event_delay_ptr)
    {
        return;
    }

    MMIAPIALM_FormInteEventInfo(event_id,&smart_event);

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    if(!smart_event.event_content_info.is_add && event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
    {
        return;//ͨ���������ж��Ƿ����û��½���
    }
#endif
    
#ifdef DPHONE_SUPPORT
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;     //add this line for cr225118
#else
#if  !defined MMI_GUI_STYLE_TYPICAL
	//cr132925 
// #if defined(MMI_ALARM_MINI_SUPPORT) && defined(MMI_GUI_STYLE_MINISCREEN)
// 	item_t.item_style = GUIITEM_STYLE_CUSTOMIZE;
// #else
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL;
// #endif
#else
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT;
#endif
#endif 
#if  !defined MMI_GUI_STYLE_TYPICAL
    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
#endif
    item_t.item_data_ptr = &item_data;

// #if  !defined MMI_GUI_STYLE_TYPICAL
//     index=1;
// #else
//     index=0;
// #endif
    index = indexArray[0];
    item_data.item_content[index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[index].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;

    if (smart_event.event_fast_info.is_on)
    {
            item_data.item_content[index].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
            //item_t.item_state = GUIITEM_STATE_SELECTED;
    }
    else if (MMIACC_ALARM_NUM > event_id)
    {
        if (event_delay_ptr->is_on)
        {
                item_data.item_content[index].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
                //item_t.item_state = GUIITEM_STATE_SELECTED;
        }
    }

//    index++;
// #if  !defined MMI_GUI_STYLE_TYPICAL
//     index=2;
// #else
//     index=1;
// #endif  
    index = indexArray[1];
    if (event_id <= EVENT_ALARM_MAX)
    {
        MMI_STRING_T name_str = {0};
        MMI_GetLabelTextByLang(txt_alarm_id[event_id],&name_str);
        if ((0 == smart_event.event_content_info.w_length)
            || !MMIAPICOM_CompareTwoWstrExt(
                name_str.wstr_ptr,
                name_str.wstr_len,
                smart_event.event_content_info.w_content,
                smart_event.event_content_info.w_length,
                TRUE)
            )
        {
            item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[index].item_data.text_id = txt_alarm_id[event_id];
        }
        else
        {
                item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[index].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;
                
                item_data.item_content[index].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;
//              MMI_WSTRNCPY(item_data.item_content[1].item_data.text_buffer.wstr,
//                     item_data.item_content[1].item_data.text_buffer.wstr_len,
//                     smart_event.event_content_info.w_content,
//                     smart_event.event_content_info.w_length,
//                     smart_event.event_content_info.w_length);
        }
        
    }
    else 
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[index].item_data.text_id = text_id_1;
    }

    index = indexArray[2];
#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
    if(TXT_ALARM_MODE_WEEK!=text_id_2)
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[index].item_data.text_id = text_id_2; 
    }
    else
    {
        if(smart_event.event_fast_info.fre_mode == ALM_MODE_EVERYWEEK && smart_event.event_fast_info.fre == 62 ) //62 = 01111110//add by James li
        {
            item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[index].item_data.text_id = TXT_ALARM_MODE_WEEK;

        }
        else
        {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPIALARM_GetFreModeString(smart_event.event_fast_info.fre_mode,smart_event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
        item_data.item_content[index].item_data.text_buffer.wstr_ptr = buff;
        }
    }
#else
    item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[index].item_data.text_id = text_id_2;
#endif
// #if  !defined MMI_GUI_STYLE_TYPICAL
//     index=0;
// #else
//     index=2;
// #endif
    index = indexArray[3];
    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    MMIAPIALARM_GetEventTimeString( &smart_event.event_fast_info, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
    
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = temp_wstr;
    item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    if(event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
    {
        item_data.softkey_id[0] = STXT_OPTION;
        list_sk_index = 1;
    }
    else
    {
        list_sk_index = 0;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
        item_data.softkey_id[1] = TXT_EDIT;
#endif
    }
        
#else
    list_sk_index = 0;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
    item_data.softkey_id[1] = TXT_EDIT;
#endif
#endif
    item_data.softkey_id[list_sk_index] = TXT_COMMON_ENABLE_ON;

    if (smart_event.event_fast_info.is_on )
    {
        item_data.softkey_id[list_sk_index] = TXT_CLOSE;
    }
    else if (MMIACC_ALARM_NUM > event_id)
    {
        if (event_delay_ptr->is_on)
        {
            item_data.softkey_id[list_sk_index] = TXT_CLOSE;
        }
    }
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
#if  !defined MMI_GUI_STYLE_TYPICAL && !defined MMI_ALARM_MINI_SUPPORT
/*****************************************************************************/
//  Description : append three line check item
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendThreeLineListItem(   
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id_1,
                            MMI_TEXT_ID_T    text_id_2,
                            uint16           event_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    MMI_TEXT_ID_T           txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10,
                                                TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                                TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                                TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};
    MMIACC_SMART_EVENT_T    smart_event = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint8 index=0;
    uint8 list_sk_index = 0;
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = GetDelayEvent( event_id );
    //SCI_ASSERT(PNULL!=event_delay_ptr);  /*assert verified*/    
    if(PNULL == event_delay_ptr)
    {
        return;
    }

    MMIAPIALM_FormInteEventInfo(event_id,&smart_event);

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    if(!smart_event.event_content_info.is_add && event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
    {
        return;//ͨ���������ж��Ƿ����û��½���
    }
#endif
    
    item_t.item_style    = GUIITEM_STYLE_THREE_LINE_TEXT_ICON;

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    if (event_id <= EVENT_ALARM_MAX)
    {
        MMI_STRING_T name_str = {0};
        MMI_GetLabelTextByLang(txt_alarm_id[event_id],&name_str);
        if (!MMIAPICOM_CompareTwoWstrExt(
            name_str.wstr_ptr,
            name_str.wstr_len,
            smart_event.event_content_info.w_content,
            smart_event.event_content_info.w_length,
            TRUE)
            ||(0 == smart_event.event_content_info.w_length))
        {
            item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[index].item_data.text_id = txt_alarm_id[event_id];
        }
        else
        {
                item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[index].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;
                
                item_data.item_content[index].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;
//              MMI_WSTRNCPY(item_data.item_content[1].item_data.text_buffer.wstr,
//                     item_data.item_content[1].item_data.text_buffer.wstr_len,
//                     smart_event.event_content_info.w_content,
//                     smart_event.event_content_info.w_length,
//                     smart_event.event_content_info.w_length);
        }
        
    }
    else 
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[index].item_data.text_id = text_id_1;
    }

    index++;

    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    MMIAPIALARM_GetEventTimeString( &smart_event.event_fast_info, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
    
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = temp_wstr;
    item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);

    index++;

    if(TXT_ALARM_MODE_WEEK!=text_id_2)
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[index].item_data.text_id = text_id_2; 
    }
    else
    {
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[index].item_data.text_buffer.wstr_len = MMIAPIALARM_GetFreModeString(smart_event.event_fast_info.fre_mode,smart_event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
        item_data.item_content[index].item_data.text_buffer.wstr_ptr = buff;
    }

    index++;

    item_data.item_content[index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[index].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;

    if (smart_event.event_fast_info.is_on)
    {
            item_data.item_content[index].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
            //item_t.item_state = GUIITEM_STATE_SELECTED;
    }
    else if (MMIACC_ALARM_NUM > event_id)
    {
        if (event_delay_ptr->is_on)
        {
                item_data.item_content[index].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
                //item_t.item_state = GUIITEM_STATE_SELECTED;
        }
    }
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    list_sk_index = 1;
#else
    list_sk_index = 0;
#endif

    item_data.softkey_id[list_sk_index] = TXT_COMMON_ENABLE_ON;
    if (smart_event.event_fast_info.is_on )
    {
        item_data.softkey_id[list_sk_index] = TXT_CLOSE;
    }
    else if (MMIACC_ALARM_NUM > event_id)
    {
        if (event_delay_ptr->is_on)
        {
            item_data.softkey_id[list_sk_index] = TXT_CLOSE;
        }
    }

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    item_data.softkey_id[0] = STXT_OPTION;
#else
    item_data.softkey_id[1] = TXT_EDIT;
#endif
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;

    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
#endif

/*****************************************************************************/
//  Description : get event timer string
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALARM_GetEventTimeString(
                              const MMIACC_SMART_EVENT_FAST_TABLE_T* event_ptr,
                              BOOLEAN                     is_valid,
                              wchar*                      str_ptr,
                              uint16                      str_len
                              )
{
    uint8 * tmp_str = PNULL;
    uint16 tmp_length = 0;
//     MMISET_TIME_DISPLAY_TYPE_E time_type = MMISET_TIME_12HOURS;
// 
//     time_type = MMIAPISET_GetTimeDisplayType();

    //SCI_PASSERT(PNULL != str_ptr && ALM_EVENT_TIME_STR_LEN <= str_len, ("MMIAPIALARM_GetEventTimeString: str_len is %d",str_len));/*assert verified*/
    if(PNULL == str_ptr || ALM_EVENT_TIME_STR_LEN > str_len)
    {
        //SCI_TRACE_LOW:"MMIAPIALARM_GetEventTimeString:PNULL == str_ptr || ALM_EVENT_TIME_STR_LEN > str_len"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9249_112_2_18_2_1_9_45,(uint8*)"");
        return;
    }
    
    tmp_length = str_len;
    tmp_str = SCI_ALLOC_APP(tmp_length);
    if(PNULL == tmp_str)
    {
        return;
    }
    SCI_MEMSET(tmp_str,0,tmp_length);
    if ( is_valid )
    {
        MMIAPISET_FormatTimeStrByTime(event_ptr->hour,event_ptr->minute, tmp_str,MMIALM_TIME_STR_12HOURS_LEN+1);
    }
    else
    {
        SCI_TIME_T   sys_time = {0};

        TM_GetSysTime( &sys_time );
        MMIAPISET_FormatTimeStrByTime(sys_time.hour,sys_time.min, tmp_str,MMIALM_TIME_STR_12HOURS_LEN+1);

    }
    MMI_STRNTOWSTR(str_ptr,str_len, (char*)tmp_str,tmp_length,tmp_length);/*lint !e64*/

    SCI_FREE(tmp_str);
    tmp_str = PNULL;

}

/*****************************************************************************/
//  Description : close delay alarm time by event_id
//  Global resource dependence : 
//  Author:        xiaoqing.lu
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SetDelayAlarm(
                            uint16 event_id,
                            MMIACC_SMART_EVENT_T *event_ptr,
                            BOOLEAN    is_on
                            )
{
    BOOLEAN    result   = TRUE;
    uint16 delay_event_id = FALSE;
    MMIALMSVC_ID_T temp_service_id = 0;
    MMIACC_SMART_EVENT_T event = *event_ptr;

    /*switch(event_id)
    {
    case EVENT_ALARM_0:
    case EVENT_ALARM_DELAY_0:
        delay_event_id = EVENT_ALARM_DELAY_0;
        event_id = EVENT_ALARM_0;
        break;

    case EVENT_ALARM_1:
    case EVENT_ALARM_DELAY_1:
        delay_event_id = EVENT_ALARM_DELAY_1;
        event_id = EVENT_ALARM_1;
        break;
        
    case EVENT_ALARM_2:
    case EVENT_ALARM_DELAY_2:
        delay_event_id = EVENT_ALARM_DELAY_2;
        event_id = EVENT_ALARM_2;
        break;
        
    case EVENT_ALARM_3:
    case EVENT_ALARM_DELAY_3:
        delay_event_id = EVENT_ALARM_DELAY_3;
        event_id = EVENT_ALARM_3;
        break;
        
    case EVENT_ALARM_4:
    case EVENT_ALARM_DELAY_4:
        delay_event_id = EVENT_ALARM_DELAY_4;
        event_id = EVENT_ALARM_4;
        break;
        
    case EVENT_ALARM_5:
    case EVENT_ALARM_DELAY_5:
        delay_event_id = EVENT_ALARM_DELAY_5;
        event_id = EVENT_ALARM_5;
        break;
        
    case EVENT_ALARM_6:
    case EVENT_ALARM_DELAY_6:
        delay_event_id = EVENT_ALARM_DELAY_6;
        event_id = EVENT_ALARM_6;
        break;
        
    case EVENT_ALARM_7:
    case EVENT_ALARM_DELAY_7:
        delay_event_id = EVENT_ALARM_DELAY_7;
        event_id = EVENT_ALARM_7;
        break;
        
    case EVENT_ALARM_8:
    case EVENT_ALARM_DELAY_8:
        delay_event_id = EVENT_ALARM_DELAY_8;
        event_id = EVENT_ALARM_8;
        break;
        
    case EVENT_ALARM_9:
    case EVENT_ALARM_DELAY_9:
        delay_event_id = EVENT_ALARM_DELAY_9;
        event_id = EVENT_ALARM_9;
        break;

    default:
        return FALSE;
        //break;
    }*/
    // Modified by sunhongzhe for snoozed calendar events.
    if (GetEventAndDelayedIds(&event_id, &delay_event_id))
    {
    }
    // End
    /*
    if(event_id<=EVENT_ALARM_MAX)
    {
        delay_event_id = event_id+EVENT_ALARM_DELAY_0;
        event_id = event_id;
    }
    else if(event_id>=EVENT_ALARM_DELAY_0&&event_id<=EVENT_ALARM_DELAY_MAX)
    {
       delay_event_id = event_id;
       event_id = event_id-EVENT_ALARM_DELAY_0; 
    }*/
    else
    {
        return FALSE;
    }

    event.event_fast_info.is_on = is_on;
    event.event_fast_info.is_valid = FALSE;

    temp_service_id = s_event_fast_table[delay_event_id].service_id;
    SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event.event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
    s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id������Ӧ���Լ���event_id

    if ((!is_on)&&(ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode))
    {
        s_event_fast_table[event_id].is_on = FALSE;//�ر���ʱ���ӵ�ͬʱ,������ʱ��ԭ����Ҳ�ر�
    }
    // ����
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_event_id, &event.event_content_info);  

    /*if (ALM_DOWNLOAD_RING == event.event_content_info.ring_type
        ||ALM_FM_RING == event.event_content_info.ring_type)
    {
        SaveRingInfoToFile(delay_event_id,&event.event_ring_info);
    } */

    return result;
}

/*****************************************************************************/
//  Description : Set alarm ring by file full path name.
//  Global resource dependence : 
//  Author: xingdong.li
//  RETRUN:  TRUE: alarm is valid
//  Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALARM_SetAlarmRing(
                                     uint16 alarm_id,
                                     const wchar *full_path_ptr,
                                     uint16 full_path_len        //len in WORD
                                     )
{
    MMIACC_SMART_EVENT_RING_T       event_ring = {0};
    uint16                          nv_return = 0;
    MMIACC_SMART_EVENT_CONTENT_T    event_content = {0};
    uint16                          delay_alarm_id = alarm_id + MMIACC_ALARM_NUM;
    BOOLEAN                         result = TRUE;  
    
    //SCI_PASSERT(alarm_id <= EVENT_ALARM_MAX && PNULL != full_path_ptr && 0 != full_path_len,("MMIAPIALARM_SetAlarmRing: wrong param"));/*assert verified*/
    if(alarm_id > EVENT_ALARM_MAX || PNULL == full_path_ptr || 0 == full_path_len)
    {
        //SCI_TRACE_LOW:"MMIAPIALARM_SetAlarmRing:alarm_id > EVENT_ALARM_MAX || PNULL == full_path_ptr || 0 == full_path_len!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9419_112_2_18_2_1_9_46,(uint8*)"");
        return FALSE;
    }

    ReadRingInfoFileName(alarm_id,&event_ring);    
    SCI_MEMSET(event_ring.dring_name_arr, 0, sizeof(event_ring.dring_name_arr));
    event_ring.dring_name_len = full_path_len;
    MMI_WSTRNCPY(
        event_ring.dring_name_arr, sizeof(event_ring.dring_name_arr),
        full_path_ptr, full_path_len,
        full_path_len
        );
    result = SaveRingInfoToFile(alarm_id,&event_ring);

    MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+alarm_id, &event_content, nv_return);
    event_content.ring_type = ALM_DOWNLOAD_RING;    
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+alarm_id, &event_content);

    if (result
        && s_event_fast_table[delay_alarm_id].is_on 
        && s_event_fast_table[delay_alarm_id].is_valid)
    {
        ReadRingInfoFileName(delay_alarm_id,&event_ring);    
        SCI_MEMSET(event_ring.dring_name_arr, 0, sizeof(event_ring.dring_name_arr));
        event_ring.dring_name_len = full_path_len;
        MMI_WSTRNCPY(
            event_ring.dring_name_arr, sizeof(event_ring.dring_name_arr),
            full_path_ptr, full_path_len,
            full_path_len
            );
        result = SaveRingInfoToFile(delay_alarm_id,&event_ring);

        MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_alarm_id, &event_content, nv_return);
        event_content.ring_type = ALM_DOWNLOAD_RING;    
        MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_alarm_id, &event_content);    
    }

    if(MMK_IsOpenWin(MMIALM_EDIT_WIN_ID)
//#ifdef MMI_RECORD_SUPPORT        
//        &&MMIAPIRECORD_IsMainWinOpened()  //�ݷţ�������
//#endif 
        )
    {
        if(MMK_IsOpenWin(MMIALM_RINGLIST_WIN_ID))
		{
			MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
		}
        if(s_current_event_info.id == alarm_id)
        {
            MMI_STRING_T        ring_name = {0};
			
            ring_name.wstr_ptr = (wchar*)full_path_ptr;
            ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
            SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr,0,(MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
            s_current_event_info.event.event_ring_info.dring_name_len = ring_name.wstr_len;
            MMIAPICOM_Wstrncpy(s_current_event_info.event.event_ring_info.dring_name_arr, ring_name.wstr_ptr, ring_name.wstr_len);
            SCI_MEMSET(ring_name.wstr_ptr,0,ring_name.wstr_len * sizeof(wchar));
            
            MMIAPIFMM_SplitFullPath(s_current_event_info.event.event_ring_info.dring_name_arr, s_current_event_info.event.event_ring_info.dring_name_len, PNULL, PNULL, PNULL, PNULL, ring_name.wstr_ptr, &ring_name.wstr_len);
//#ifdef MMI_RECORD_SUPPORT 
            //s_current_event_info.event.event_content_info.ring_type = ALM_RECORD_RING;
//#endif
        }       
    }

    return result;
}

/*****************************************************************************/
//  Description : ��鲢���������¼�(��������,�Զ��������¼�����ʱ����)
//  Global resource dependence : 
//  Author: zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void CheckandStartExpiredEvent( void )
{
//cr137019
    if(s_bypast_list.total > 0)
    {
        uint16 event_id = s_bypast_list.entry_id[0];

        DeleteOneSchEventInList(0, &s_bypast_list);

        OpenPastSchEventWin(WIN_ONE_LEVEL, event_id);
    }
    else if((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE) && !MMIAPIMMS_IsSendingOrRecving())
    {
        MMI_MEMCPY(
            (void*)&s_arrived_event, sizeof(MMIACC_SMART_EVENT_T),
            (void*)&s_poweroff_event, sizeof(MMIACC_SMART_EVENT_T),
            sizeof(MMIACC_SMART_EVENT_T));
        s_arrived_event_id = EVENT_AUTO_POWEROFF;
        OpenAlarmExpiredWin(s_arrived_event.event_fast_info.type);
        SCI_MEMSET(&s_poweroff_event, 0, sizeof(MMIACC_SMART_EVENT_T));
    }
    else if ( STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition() || STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition())
    {
        MMK_SendMsg(VIRTUAL_WIN_ID, MSG_ALM_ALERTING_END, PNULL);
    }
}

/*save event ring info to fs*/
/*****************************************************************************/
// Description : ����alarmĿ¼
// Global resource dependence :
// Author:zhaohui
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitAlarmFolder(void)
{
    wchar            full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16           full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    wchar            *file_dev_ptr = PNULL;
    uint16           file_dev_len = 0;
    BOOLEAN          result = FALSE;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SDCARD;
    
    dev = MMIAPIFMM_GetFirstValidDevice();
    file_dev_ptr = MMIAPIFMM_GetDevicePath(dev);
    file_dev_len = MMIAPIFMM_GetDevicePathLen(dev);
    if((PNULL == file_dev_ptr) || (0 == file_dev_len))
    {
        return FALSE;
    }
    
    if(MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
        MMIMULTIM_DIR_ALARM, MMIMULTIM_DIR_ALARM_LEN, 
        NULL, 0, 
        full_path_name, &full_path_len))
    {
        if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            result = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
        else
        {
            result = TRUE;
        }
    }
    //SCI_TRACE_LOW:"InitAlarmFolder result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9527_112_2_18_2_1_10_47,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : ����event_idӳ��ɶ�Ӧ�ü�¼��ring��Ϣ
//  Global resource
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetRingInfoFileName(
                                     uint16 event_id,  
                                     wchar *u16filename_ptr,//OUT
                                     uint16 u16filename_max_len
                                     )
{
    BOOLEAN         ret = FALSE;
    uint8           u8filename[MMIALM_RING_INFO_FILE_NAME_LEN +1] = {0};
    wchar           wstr_filename[MMIALM_RING_INFO_FILE_NAME_LEN +1] = {0};
    uint32          u32index = event_id;//(group_id << 16) + entry_id;
//  FILE_DEV_E_T    file_dev = FS_UDISK;
    uint16          full_path_len = u16filename_max_len;
    uint16          wstr_len  = 0;
    wchar           *device_name_ptr = PNULL;
    uint16           device_name_len = 0;   
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
   
    if(PNULL != u16filename_ptr)
    {
        //ring id informatio file name
        sprintf( (char*)u8filename,"%ld%s",u32index,MMIALM_RING_INFO_FILE_SUFFIX);
        
        dev = MMIAPIFMM_GetFirstValidDevice();
        device_name_ptr = MMIAPIFMM_GetDevicePath(dev);
        device_name_len = MMIAPIFMM_GetDevicePathLen(dev);
        
        if((PNULL == device_name_ptr) || (0 == device_name_len))
        {
            return FALSE;
        }
    
        wstr_len = MIN(strlen( (char*)u8filename), MMIALM_RING_INFO_FILE_NAME_LEN); /*lint !e666*/
        MMI_STRNTOWSTR(wstr_filename, MMIALM_RING_INFO_FILE_NAME_LEN, (char*)u8filename, MMIALM_RING_INFO_FILE_NAME_LEN, wstr_len);/*lint !e64*/
        if (!MMIAPIFMM_CombineFullPath(
                                device_name_ptr,
                                device_name_len,
                                MMIMULTIM_DIR_ALARM,
                                MMIMULTIM_DIR_ALARM_LEN,
                                wstr_filename,
                                wstr_len,
                                (wchar *)u16filename_ptr,  
                                &full_path_len
                              )
        )
        {
            //SCI_TRACE_LOW:"mmialarm.c GetRingInfoFileName MMIAPIFMM_CombineFullPath fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9580_112_2_18_2_1_10_48,(uint8*)"");
            return ret;
        }

        if(full_path_len <= MMIFILE_FILE_NAME_MAX_LEN)
        {
            u16filename_ptr[full_path_len] = 0;
        }

        ret = TRUE;
    }
    //SCI_TRACE_LOW:"GetRingInfoFileName event_id %d, ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9591_112_2_18_2_1_10_49,(uint8*)"dd",event_id, ret);
    return ret;
    
}

/*****************************************************************************/
//  Description : ��ring_ptr���浽����event_idӳ��ɵ�����id��Ϣ�ļ��� 
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN SaveRingInfoToFile(
                                           uint16 event_id,
                                           const MMIACC_SMART_EVENT_RING_T *ring_ptr//IN
                                           )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint32 write_size = 0;

    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    if(PNULL != ring_ptr)
    {
        if(GetRingInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN + 1)))
        {
            if(InitAlarmFolder())
            {
                //���ļ�
                file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
                if(SFS_INVALID_HANDLE != file_handle)
                {
                    file_ret = MMIAPIFMM_WriteFile(
                        file_handle, 
                        ring_ptr, 
                        sizeof(MMIACC_SMART_EVENT_RING_T), 
                        &write_size, 
                        NULL
                        );
                    //SCI_TRACE_LOW:"MMIALM_SaveRingIDInfoToFile file_ret %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9630_112_2_18_2_1_10_50,(uint8*)"d",file_ret);
                    if(SFS_ERROR_NONE == file_ret)
                    {
                        ret = TRUE;
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }

            }
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : ������id��Ϣ�ļ��ж�ȡ����id��·����Ϣ  
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN ReadRingInfoFileName(
                                        uint16 event_id,
                                        MMIACC_SMART_EVENT_RING_T *ring_ptr//IN
                                        )
{
    BOOLEAN ret = FALSE;
    wchar u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 read_size = 0;

    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    if(PNULL != ring_ptr)
    {
        if(GetRingInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN + 1)))
        {
            file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
            
            if(SFS_INVALID_HANDLE != file_handle)
            {
                file_ret =  MMIAPIFMM_ReadFile(
                    file_handle, 
                    ring_ptr, 
                    sizeof(MMIACC_SMART_EVENT_RING_T), 
                    &read_size, 
                    NULL
                    );
                //SCI_TRACE_LOW:"ReadRingInfoFileName file_ret %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9676_112_2_18_2_1_10_51,(uint8*)"d",file_ret);
                if(SFS_ERROR_NONE == file_ret && (read_size > 0) /*&& (ring_ptr->file_name_len > 0) && (ring_ptr->file_name_len <= MMIFILE_FILE_NAME_MAX_LEN)*/)
                {
                    ret = TRUE;
                }
                MMIAPIFMM_CloseFile(file_handle);
            }
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : ɾ������id��Ϣ������id�ļ�  
//  Global resource
//  Author: zhaohui
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteRingInfoFile(
                                    uint16 event_id                     
                )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;

    if(GetRingInfoFileName(event_id, u16filename,(MMIFILE_FILE_NAME_MAX_LEN +1)))
    {
        file_ret = MMIAPIFMM_DeleteFile(u16filename, PNULL);
        //SCI_TRACE_LOW:"DeleteRingInfoFile file_ret %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9704_112_2_18_2_1_10_52,(uint8*)"d",file_ret);
        if(SFS_NO_ERROR == file_ret)
        {
            ret = TRUE;
        }
    }

    return ret; 
}

/*****************************************************************************/
//  Description : get the integrity event info by event_id
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIALM_FormInteEventInfo(
                            uint16 event_id,//in
                            MMIACC_SMART_EVENT_T* integrity_event_ptr//out
                            )//�õ��������¼���Ϣ
{
    MMIACC_SMART_EVENT_CONTENT_T*   event_content_ptr = PNULL;
    MMIACC_SMART_EVENT_RING_T*      event_ring_ptr = PNULL;
    uint16                  nv_return = 0;

    //SCI_ASSERT(PNULL != integrity_event_ptr); /*assert verified*/
    if(PNULL == integrity_event_ptr)
    {
        return;
    }
    event_content_ptr = SCI_ALLOC_APPZ(sizeof(MMIACC_SMART_EVENT_CONTENT_T));
    //SCI_ASSERT(PNULL != event_content_ptr);/*assert verified*/
    SCI_MEMSET( event_content_ptr, 0, sizeof(MMIACC_SMART_EVENT_CONTENT_T) );

    event_ring_ptr = SCI_ALLOC_APPZ(sizeof(MMIACC_SMART_EVENT_RING_T));
    //SCI_ASSERT(PNULL != event_ring_ptr);/*assert verified*/
    SCI_MEMSET( event_ring_ptr, 0, sizeof(MMIACC_SMART_EVENT_RING_T) );

    SCI_MEMCPY((uint8 *)&integrity_event_ptr->event_fast_info, 
               (uint8 *)MMIALM_GeEvent((uint16)(EVENT_ALARM_0+event_id)), 
               sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));

    MMINV_READ(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, event_content_ptr, nv_return);
    SCI_MEMCPY((uint8 *)&integrity_event_ptr->event_content_info, 
               (uint8 *)event_content_ptr, 
               sizeof(MMIACC_SMART_EVENT_CONTENT_T));

//     if (ReadRingInfoFileName(event_id,event_ring_ptr))
//    {
//        SCI_MEMCPY((uint8 *)&integrity_event_ptr->event_ring_info, 
//                   (uint8 *)event_ring_ptr, 
//                   sizeof(MMIACC_SMART_EVENT_RING_T));
//    }
//    else
//    {//���洢��������,���޷���ȡ,��
//        if (ALM_DOWNLOAD_RING == integrity_event_ptr->event_content_info.ring_type)
//        {
//            integrity_event_ptr->event_content_info.ring_type = ALM_FIX_RING;
//            integrity_event_ptr->event_content_info.ring = 1;
//            MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &integrity_event_ptr->event_content_info);
//        }
//     }

    SCI_FREE(event_content_ptr);
    event_content_ptr = PNULL;

    SCI_FREE(event_ring_ptr);
    event_ring_ptr = PNULL;
}

/*****************************************************************************/
//  Description : Save the integrity event info 
//  Global resource dependence : 
//  Author:        zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_SaveInteEventInfo(
                            uint16 event_id,//in
                            MMIACC_SMART_EVENT_T* integrity_event_ptr//in
                            )
{
    BOOLEAN   result = FALSE;

    //SCI_ASSERT(PNULL != integrity_event_ptr); /*assert verified*/
    if(PNULL == integrity_event_ptr)
    {
        return FALSE;
    }
    result = MMIALM_ModifyEvent(event_id, &integrity_event_ptr->event_fast_info);
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &integrity_event_ptr->event_content_info);

    if (ALM_DOWNLOAD_RING == integrity_event_ptr->event_content_info.ring_type
        ||ALM_RECORD_RING == integrity_event_ptr->event_content_info.ring_type
#ifdef FM_SUPPORT
        ||ALM_FM_RING == integrity_event_ptr->event_content_info.ring_type
#endif
        )
    {
        if (!MMIAPIUDISK_UdiskIsRun())
        {
            result = SaveRingInfoToFile(event_id,&integrity_event_ptr->event_ring_info);
        }
    }
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.9
    MMIVCAL_UpdatePimStatus(event_id-EVENT_SCH_0+1,VCAL_RECENT_REPLACE);
#endif /* HS_PIM_VCAL */

    return result;
}



/*****************************************************************************/
//  Description : get total times of the arrived alarm and the arrived calender 
//  Global resource dependence : 
//  Author:      jianhui.luo
//  Note:            
/*****************************************************************************/
PUBLIC uint32 MMIAPIALM_GetTotalTimesOfArrivedEvent(void)
{
    /*if(MMIAPIALM_IsAliveWinOpened())
    {  
        return 1;
    }
    else
    {
         return 0;
    } */
#ifdef CALENDAR_SUPPORT
    return MMIAPISCH_GetTotalTimesOfUnreadEvent();
#endif
}

/*****************************************************************************/
//  Description :interrupt alarm delete 
//  Global resource dependence : 
//  Author:      
//  Note:            
/*****************************************************************************/
PUBLIC void MMIALM_SetAlmInterruptFlag(BOOLEAN  flag)
{
    s_alarm_is_interrupted = flag;
    if(flag)
    {
        //interrup delete, close waitting win
        MMK_CloseWin(MMISCH_DELETE_ANIM_WIN_ID);
    }
    //SCI_TRACE_LOW:"[MMIALM] MMIALM_Interrupt s_alarm_is_interrupted %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9834_112_2_18_2_1_10_53,(uint8*)"d", s_alarm_is_interrupted);
}

/*****************************************************************************/
//  Description :interrupt alarm delete 
//  Global resource dependence : 
//  Author:      
//  Note:            
/*****************************************************************************/
PUBLIC BOOLEAN MMIALM_GetAlmInterruptFlag(void)
{
    //SCI_TRACE_LOW:"[MMIALM] MMIALM_GetAlmInterruptFlag s_alarm_is_interrupted %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9843_112_2_18_2_1_10_54,(uint8*)"d", s_alarm_is_interrupted);
    return s_alarm_is_interrupted;
}

/*****************************************************************************/
// Description : get ring info
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_GetRingInfo(uint16 event_id, MMIACC_SMART_EVENT_T *event_info_ptr)
{
    BOOLEAN result=TRUE;
    if(PNULL == event_info_ptr)
    {
        return result;
    }
    if(ALM_DOWNLOAD_RING == event_info_ptr->event_content_info.ring_type
        ||ALM_RECORD_RING == event_info_ptr->event_content_info.ring_type
#ifdef FM_SUPPORT
        ||ALM_FM_RING == event_info_ptr->event_content_info.ring_type
#endif
        )
    {
        if(event_id >= EVENT_ALARM_DELAY_0 && event_id <= EVENT_ALARM_DELAY_MAX)
        {
            event_id = event_id - MMIACC_ALARM_NUM;
        }
        // Added for snoozed calendar.
        else if (event_id >= EVENT_SCH_DELAY_0 && event_id <= EVENT_SCH_DELAY_MAX)
        {
            event_id -= MMIACC_SCH_NUM;
        }
        // End

        if (!ReadRingInfoFileName(event_id, &event_info_ptr->event_ring_info))
        {//���洢��������,���޷���ȡ,��
            //event_info_ptr->event_content_info.ring_type = ALM_FIX_RING;
            //event_info_ptr->event_content_info.ring = 1;
            //MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+event_id, &event_info_ptr->event_content_info);
            result=FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : display Alarm window
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayAlarmWin(
                          MMI_WIN_ID_T win_id, 
                          MMIPUB_INFO_T    *win_info_ptr
                          )
{
    MMIPUB_HandleWaitWinMsg(win_id, MSG_FULL_PAINT, PNULL);
//     GUI_POINT_T         dis_point = {0};
//     uint16          image_width = 0;
//     uint16          image_height = 0;
//     GUI_RECT_T      disp_rect = {0};
//     GUI_RECT_T      image_rect = {0};
// 
// 
//     //display wait win background image
//     dis_point.x = win_info_ptr->pubwin_theme.bg_display_rect.left;
//     dis_point.y = win_info_ptr->pubwin_theme.bg_display_rect.top;
//     GUIRES_DisplayImg(&dis_point,
//         PNULL,
//         PNULL,
//         win_info_ptr->win_id,
//         win_info_ptr->pubwin_theme.bg_id,
//         &win_info_ptr->pubwin_theme.bg_lcd_dev);
// 
//     IMG_EnableTransparentColor(TRUE);
//     if (0 != win_info_ptr->pubwin_theme.anim_bg_image_id)
//     {
//         //display wait win background image
//         dis_point.x = win_info_ptr->pubwin_theme.anim_bg_display_rect.left;
//         dis_point.y = win_info_ptr->pubwin_theme.anim_bg_display_rect.top;
//         GUIRES_DisplayImg(&dis_point,
//             PNULL,
//             PNULL,
//             win_info_ptr->win_id,
//             win_info_ptr->pubwin_theme.anim_bg_image_id,
//             &win_info_ptr->pubwin_theme.bg_lcd_dev);
//     }
// 
//     if (0 != win_info_ptr->prompt_image_id)
//     {
//         //display alert image
//         dis_point.x = win_info_ptr->pubwin_theme.prompt_display_rect.left;
//         dis_point.y = win_info_ptr->pubwin_theme.prompt_display_rect.top;
//         GUIRES_DisplayImg(&dis_point,
//             PNULL,
//             PNULL,
//             win_info_ptr->win_id,
//             win_info_ptr->prompt_image_id,
//             &win_info_ptr->pubwin_theme.bg_lcd_dev);
//     }
// 
//     IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : past auto power on prompt window
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePastAutoPowerOn(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:  
          // �������ڶ�ʱ��
          StartAlarmTimer(&s_3seconds_timer_id, MMI_3SECONDS, FALSE);
          //GUIWIN_SetSoftkeyTextId(MMIALM_BYPAST_SCH_WIN_ID,  STXT_SOFTKEY_OK_MK, TXT_NULL, (MMI_TEXT_ID_T)STXT_EXIT, FALSE);  
          recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
          MMIDEFAULT_TurnOnBackLight(); 
          break;

        case MSG_APP_OK:
        case MSG_CTL_OK :
            break;
        case MSG_KEYLONG_RED:
            //SCI_TRACE_LOW:"mmialarm.c: HandlePastAutoPowerOn() MSG_CTL_OK "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_9961_112_2_18_2_1_11_55,(uint8*)"");
            MMK_CloseWin(win_id);
            if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
            {
               MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
            }
            MMIAPIPHONE_PowerReset();
            break;
                            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            /*MMIDEFAULT_TurnOffBackLight();

             POWER_PowerOff();*/
            break;
        
        /*case MSG_KEYLONG_RED:
            MMK_CloseWin(win_id);

            MMIAPIPHONE_PowerReset();
            break;*/

       case MSG_TIMER:                  
            if (s_3seconds_timer_id == *((uint8 *) param))
            {
                GUIWIN_UpdateTitle(win_id);
                MMK_CloseWin(win_id);

                if(MMIAPIPHONE_GetStartUpCondition() == STARTUP_CHARGE)
                {
                   MMK_CloseWin(PHONE_STARTUP_CHARGE_WIN_ID);
                }
                
                MMK_StopTimer(s_3seconds_timer_id);
                s_3seconds_timer_id = 0;
                
                MMIAPIPHONE_PowerReset();
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
           break;

        case MSG_FULL_PAINT:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            break;
            
        case MSG_CLOSE_WINDOW:
            if (0 != s_3seconds_timer_id)
            {
                MMK_StopTimer(s_3seconds_timer_id);
                s_3seconds_timer_id = 0;
            }
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            break;

        default:
            if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_WEB ) ||
                 (MSG_KEYUP_FLIP == msg_id ))//|| MSG_CTL_OK == msg_id))
            {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.            
                if (MSG_KEYUP_FLIP == msg_id)
                {
                    MMIDEFAULT_HandleFlipKey(TRUE);  
                }
#endif                
            }else
            {
                recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, PNULL);
            }
            break;
    }    

    return recode;
}

/*****************************************************************************/
//  Description : handle alarm edit windows penok messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  HandleAlarmEditPENOKMsg(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id
                                          )
{
    switch(ctrl_id)
    {
        case MMIALM_EDIT_RING_TYPE_CTRL_ID:
            MMK_CreateWin((uint32 *)ALARM_RINGWIN_TAB, PNULL);
            break;
#ifdef MMI_PDA_SUPPORT
        case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
            MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
            break;
        case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
            MMK_SendMsg(win_id, MSG_APP_CANCEL,  PNULL);
            break;
        case MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubEditWin((uint32 *)MMIALM_NAMEWIN_TAB, PNULL);
            break;
        case MMIALM_EDIT_TITLE_EDIT_LABEL_CTRL_ID:
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
            {
                SCI_TIME_T      time_info = {0};
                if (s_current_event_info.event.event_fast_info.is_valid )
                {
                    time_info.hour=s_current_event_info.event.event_fast_info.hour;
                    time_info.min = s_current_event_info.event.event_fast_info.minute;
                }
                else
                {
        	        TM_GetSysTime(&time_info);
                    
                }
                MMIAPIALARM_OpenAlmClockWin(time_info, SaveTimeEditWin);
            }
          // MMK_CreateWin((uint32*)MMIALM_JUMP_CLOCK_TAB, PNULL);
           
#else
			MMK_CreatePubFormWin((uint32 *)MMIALM_TIMEWIN_TAB, PNULL);
#endif
            break;
#endif
        case MMI_EDITWIN_BUTTON0_CTRL_ID:
        case MMI_EDITWIN_BUTTON1_CTRL_ID:
        case MMI_EDITWIN_BUTTON2_CTRL_ID:
        case MMI_EDITWIN_BUTTON3_CTRL_ID:
        case MMI_EDITWIN_BUTTON4_CTRL_ID:
        case MMI_EDITWIN_BUTTON5_CTRL_ID:
        case MMI_EDITWIN_BUTTON6_CTRL_ID:
            if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
            {
                s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=FALSE;
            }
            else
            {
                s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=TRUE;
            }
#ifndef MMI_PDA_SUPPORT
            setAlarmEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
            SetEditWinButtonBg(win_id,ctrl_id);
#ifdef MMI_PDA_SUPPORT
         SaveFreEditWin();
         if(ALM_MODE_EVERYWEEK==GUISETLIST_GetCurIndex(MMIALM_EDIT_MODE_SETLIST_CTRL_ID))
         {
            wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
            MMI_STRING_T str={0};
            str.wstr_len=MMIAPIALARM_GetFreModeString(ALM_MODE_EVERYWEEK,s_current_event_info.event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
            str.wstr_ptr=buff;
            GUISETLIST_SetDispText(MMIALM_EDIT_MODE_SETLIST_CTRL_ID,&str,TRUE);
         }
#endif
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : handle alarm edit windows FormSwitchActive messages
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void  HandleAlarmEditFormSwitchActiveMsg(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id
                                          )
{
    switch(ctrl_id)
    {
        case MMIALM_EDIT_RING_TYPE_CTRL_ID:
        case MMIALM_EDIT_MODE_SETLIST_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK, TRUE);
            GUIWIN_SeSoftkeytButtonIconId(win_id,  IMAGE_COMMON_SOFTKEY_SELECTED_ICON, 1, TRUE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK, TRUE);
#endif
#endif
            break;
        case MMI_EDITWIN_BUTTON0_CTRL_ID:
        case MMI_EDITWIN_BUTTON1_CTRL_ID:
        case MMI_EDITWIN_BUTTON2_CTRL_ID:
        case MMI_EDITWIN_BUTTON3_CTRL_ID:
        case MMI_EDITWIN_BUTTON4_CTRL_ID:
        case MMI_EDITWIN_BUTTON5_CTRL_ID:
        case MMI_EDITWIN_BUTTON6_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
            setAlarmEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
            SetEditWinButtonBg(win_id,ctrl_id);
            break;
        //case MMIALM_EDIT_NAME_EDITBOX_CTRL_ID:
        //[alarm edit option menu]
        case MMIALM_EDIT_REPEAT_LIST_CTRL_ID:
        case MMIALM_EDIT_RING_LIST_CTRL_ID:
        case MMIALM_EDIT_SNOOZE_TIME_CTRL_ID:
            GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_SELECT_MK,MIDDLE_BUTTON,TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_OPTION,LEFT_BUTTON,TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,TRUE);
        //[end]
            break;
//         //[alarm edit option menu]
        case MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID:
            if(win_id != POWERONOFF_EDIT_WIN_ID)
            {
                GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_SAVE_MK,MIDDLE_BUTTON,TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_OPTION,LEFT_BUTTON,TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,TRUE);
            }
            else
            {
                GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_NULL,MIDDLE_BUTTON,TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_OK,LEFT_BUTTON,TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,TRUE);

                SetEditWinButtonBg(win_id,ctrl_id);
            }
            break;
//         //[end]
        case MMIALM_EDIT_NAME_EDITBOX_CTRL_ID:
            break;
        default:
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK, TRUE);
            GUIWIN_SeSoftkeytButtonIconId(win_id, TXT_NULL, 1, TRUE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK, TRUE);
#endif
#endif
            SetEditWinButtonBg(win_id,ctrl_id);
            break;
    }
}

/*****************************************************************************/
//  Description : Set EditWin Button bg
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetEditWinButtonBg(MMI_HANDLE_T    win_id,uint32 ctrl_id)
{
    uint32 i=0;
    GUI_BG_T  bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};

    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    enabled_font.font=SONG_FONT_14;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    disabled_font.font=SONG_FONT_14;
#else
    disabled_font.font=SONG_FONT_16;
#endif
    bg.bg_type=GUI_BG_IMG;
    for(i=0;i<ALM_WEEK_DAY_NUM;i++)
    {
        if(s_weeks_button_is_selected[i])
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_ENABLED;            
            GUIBUTTON_SetFont(MMI_EDITWIN_BUTTON0_CTRL_ID+i, &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;            
            GUIBUTTON_SetFont(MMI_EDITWIN_BUTTON0_CTRL_ID+i, &disabled_font);
        }
#ifndef MMI_PDA_SUPPORT
        if(ctrl_id==MMI_EDITWIN_BUTTON0_CTRL_ID+i)
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_PRESSED;
        }
#endif
        GUIBUTTON_SetBg(MMI_EDITWIN_BUTTON0_CTRL_ID+i, &bg);
        GUIBUTTON_Update(MMI_EDITWIN_BUTTON0_CTRL_ID+i);
    }
    
}

/*****************************************************************************/
//  Description : GetFreByEditWin
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL uint8  GetFreByEditWin(void)
{
    uint8   week_day =0;
    int32 loop=0;
    uint8      mode = 0;
    
    for(loop=0;loop<ALM_WEEK_DAY_NUM;loop++)
    {
        week_day = MMIAPISET_GetWeekDayByOffset(loop);
        if(s_weeks_button_is_selected[loop])
        {
            mode |= s_week_mask[week_day];
        }
    }
    
    return mode;
}

/*****************************************************************************/
//  Description : save fre
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveFreEditWin(void)
{
//  uint16      cur_num       =   0;
    uint8   week_day =0;
    int32 loop=0;
//  uint32      i = 0; 
    uint16      mode = 0;
    
    for(loop=0;loop<ALM_WEEK_DAY_NUM;loop++)
    {
        week_day = MMIAPISET_GetWeekDayByOffset(loop);
        if(s_weeks_button_is_selected[loop])
        {
            mode |= s_week_mask[week_day];
        }
    }

    if (mode != s_current_event_info.event.event_fast_info.fre) 
    {
        s_current_event_info.event.event_fast_info.fre = mode & 0x00ff;
    }

    if(0==s_current_event_info.event.event_fast_info.fre)
    {
        MMIPUB_OpenAlertWarningWin(TXT_ALARM_SELECT_ONE_DAY);
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    uint32 ctrl_id=MMI_EDITWIN_BUTTON0_CTRL_ID;
    GUI_RECT_T rect={0};//max_rect��owner_draw_ptr->display_rect�ཻ����Ҳ��ͼƬ�����ʾ����
    GUI_RECT_T max_rect={0};//ͼƬ������ʾ���������
    GUI_RECT_T img_rect={0};//ͼƬ�ü���
    MMI_HANDLE_T win_handle = 0;
    MMI_WIN_ID_T    win_id=0;
    uint16 image_w=0;
    uint16 iamge_h=0;

    ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
    win_handle=MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
    win_id=MMK_GetWinId(win_handle);
    GUIRES_GetImgWidthHeight(&image_w, &iamge_h, IMAGE_CLOCK_ALARM_WEEK_MARK, win_id);

    max_rect.left=owner_draw_ptr->display_x;
    max_rect.top=owner_draw_ptr->display_y;
    max_rect.right=max_rect.left+image_w;
    max_rect.bottom=max_rect.top+iamge_h;
    
    if(!GUI_IntersectRect(&rect,max_rect,owner_draw_ptr->display_rect))
    {//û���ཻ���򣬷���
        return ;
    }

    img_rect.left=rect.left-max_rect.left;
    img_rect.top=rect.top-max_rect.top;
    img_rect.right=img_rect.left+(rect.right-rect.left);
    img_rect.bottom=img_rect.top+(rect.bottom-rect.top);

    if(GUI_IsInvalidRect(img_rect)||GUI_IsInvalidRect(rect))//��Ч���β�Ҫ����
    {
        return ;
    }
    
    if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
    {
        GUIRES_DisplayImg(PNULL, &rect, &img_rect, win_handle, IMAGE_CLOCK_ALARM_WEEK_MARK, &owner_draw_ptr->lcd_dev);
    }
}
//for widget
PUBLIC void  MMIAPIALARM_OpenEditAlarmWinbyEventid(uint16 event_id)
{
         s_current_event_info.id = event_id;
         MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event); 
         if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
         {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
         }
         s_current_event_info.type = SMART_EVENT_ALARM;
         s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;      
		 if (MMK_IsOpenWin(MMIALM_EDIT_WIN_ID))
		 {
			 MMK_CloseWin(MMIALM_EDIT_WIN_ID);
		 }
      MMK_CreateWin((uint32*)ALARM_EDITWIN_TAB, PNULL);
#ifdef CALENDAR_SUPPORT
      MMIAPISCH_DeleteUnreadEvent(s_current_event_info.id);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL void setAlarmEditSelectWeeksSoftkey(
                                            MMI_HANDLE_T    win_id,
                                            uint32 ctrl_id,
                                            BOOLEAN is_update)
{
    if(ctrl_id>=MMI_EDITWIN_BUTTON0_CTRL_ID && ctrl_id<=MMI_EDITWIN_BUTTON6_CTRL_ID)
    {
        if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK, is_update);
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_CANCEL_ICON, 1, is_update);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_CANCEL, TXT_COMMON_COMMON_BACK, is_update);
#endif
        }
        else
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK, is_update);
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_SELECTED_ICON, 1, is_update);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK, is_update);
#endif
        }
    }
}

/*****************************************************************************/
//  Description : ����ID�ж��Ƿ����ճ��¼�
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
// LOCAL BOOLEAN IsSCHEvent(uint16 event_id)
// {
//     if (event_id <EVENT_SCH_0||event_id >EVENT_SCH_MAX)
//     {
//         return FALSE;
//     }
//      return TRUE;
// }

/*****************************************************************************/
//  Description : �ж��Ƿ���һ����ЧƵ��ֵ
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsValidFM(    const wchar *fre,     //in
                            uint16       fre_len       //in, ˫�ֽ�Ϊ��λ
                            )
{
    BOOLEAN result=TRUE;
    int i=0;
    int dot_pos=0;
    BOOLEAN is_exist_dot=FALSE;
    int16 ret=0;
    MMI_STRING_T str={0};
    str.wstr_ptr=fre;   /*lint !e605*/
    str.wstr_len=fre_len;
#ifdef FM_SUPPORT    
    if(fre_len>(MMIFM_GetFmTuneStep() == 0?6 : MMIALM_FM_MAX_LEN) ||fre_len==0 || PNULL == fre)
    {
        return FALSE;
    }
#endif
    for(i=0;i<fre_len;i++)
    {
        if((fre[i]<'0'||fre[i]>'9')&&fre[i]!='.')
        {
            return FALSE;
        }
        if(fre[i]=='.')
        {
            dot_pos=i;
            is_exist_dot=TRUE;
        }
    }
#ifdef FM_SUPPORT
    if(MMIFM_GetFmTuneStep() == 1){
        if(is_exist_dot&&(dot_pos != (fre_len -2)))//ֻ��1λ��С�����ж�
        {//CR141079
            return FALSE;
        }
        ret = MMIAPIFM_ConvertStringToFreq(&str);
        if(!(MMIALM_FREQ_RANGE_LOW <= ret && ret <= MMIALM_FREQ_RANGE_HIGH))
        {
            return FALSE;
        }
    }else
#endif
    {
        if(is_exist_dot && ((fre_len -dot_pos -1 >2)||((fre_len -dot_pos -1 == 2) && !(fre[fre_len-1]=='0' ||fre[fre_len-1]=='5')) ))
        {
            return FALSE;
        }
	 ret = MMIAPIFM_ConvertStringToFreq(&str);
	 if(!(8750 <= ret && ret <= 10800))
        {
            return FALSE;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Ƶ�ʱ༭������Ϣ������
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFMEditWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E        recode          = MMI_RESULT_TRUE;
#ifdef FM_SUPPORT
#ifdef MMI_PDA_SUPPORT
    uint16 s_cur_list_index=0;
#else
    MMI_STRING_T str={0};
    MMI_CTRL_ID_T ctrl_id =MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID;
#endif
	MMIACC_SMART_EVENT_T *detail_win_param = (MMIACC_SMART_EVENT_T *) MMK_GetWinAddDataPtr(win_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetFMEditCtrlParam(win_id);
#ifdef MMI_PDA_SUPPORT
        MMK_SetAtvCtrl(win_id, MMIALM_SELECT_FM_LIST_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id, MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID);
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_PDA_SUPPORT
        s_cur_list_index = GUILIST_GetCurItemIndex(MMIALM_SELECT_FM_LIST_CTRL_ID);
        if(0==s_cur_list_index)
        {
            MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
            *w_id = MMIALM_FM_EDIT_WIN_ID;
            MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
        }
        else if(1==s_cur_list_index)
        {
           MMK_CreatePubEditWin((uint32 *)MMIALM_EDITFMFRE_WIN_TAB, PNULL); 
        }
#else
        ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        if(ctrl_id==MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID)
        {
            MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
            *w_id = MMIALM_FM_EDIT_WIN_ID;
            MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
        }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
    case MSG_APP_MENU:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        if(ctrl_id==MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID)
        {
            MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
            *w_id = MMIALM_FM_EDIT_WIN_ID;
            MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
        }
        else if(ctrl_id==MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID)
        {
            GUIEDIT_GetString(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, &str);
            if(MMIAPIALM_IsValidFM(str.wstr_ptr, str.wstr_len))
            {
                int32 iloop = 0, dot_index = 0;
                BOOLEAN exist_dot = FALSE;

                //����ڵ�Ŷ���ź���û�����ݣ��������ݷǷ����ں������0
                //��ʵӦ���ں���MMIALM_IsValidFM�д���ģ������������޸���ԭ����
                //�Ĺ��ܣ��˴���һ���ط�ע�⣬�ڵ�ź�ֱ�����0��
                //���ڽ��༭�������һ���ַ�0��������Ӧ�ò�����ʲôӰ���
                for(iloop = 0; iloop < str.wstr_len && iloop < MMIALM_FM_MAX_LEN; iloop ++)
                {
                    if((uint16)str.wstr_ptr[iloop] == '.')
                    {
                        exist_dot = TRUE;
                        dot_index = iloop;
                        //break;        //��ֹ���ڵ�źͺ������ݣ���Ȼ��������
                    }
                }

                if(exist_dot && (dot_index + 1 == iloop))   //���Ϊ���һλ�����0
                {
                    str.wstr_ptr[dot_index + 1] = '0';
                    str.wstr_len += 1;
                }
                else if(exist_dot == FALSE)     //û�е�ţ���Ҫ��ӵ�ź�0
                {
                    str.wstr_ptr[str.wstr_len] = '.';
                    str.wstr_ptr[str.wstr_len + 1] = '0';
                    str.wstr_len += 2;
                }
            
                detail_win_param->event_content_info.ring_type = ALM_FM_RING;
                detail_win_param->event_ring_info.dring_name_len = str.wstr_len;

                SCI_MEMSET(detail_win_param->event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
                MMI_WSTRNCPY(detail_win_param->event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                str.wstr_ptr, str.wstr_len, str.wstr_len);
                detail_win_param->event_content_info.ring = MMISET_MAX_RING_ID;
                
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
				MMK_CloseWin(MMISCH_RINGLIST_WIN_ID);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ALARM_FM_INVALID_PROMPT);
            }
        }
        break;
#endif
    case MSG_ALM_SELECTED_FM_RETURN:
        {
            char freq_str[MMIALM_FREQ_MAX_LEN+1]={0};
            MMI_STRING_T str_t = {0};
            wchar wfreq_str[MMIALM_FREQ_MAX_LEN+1]={0};
            MMIFM_CHANNEL_T *win_param = (MMIFM_CHANNEL_T *)param;
			
			// channel's frequency valid
			MMIAPIFM_ConvertFreqToString(win_param->freqency, (char*) freq_str);
			
            str_t.wstr_len = MIN(strlen(freq_str), MMIALM_FREQ_MAX_LEN);    /*lint !e666*/
            str_t.wstr_ptr = wfreq_str;
            MMI_STRNTOWSTR(str_t.wstr_ptr, MMIALM_FREQ_MAX_LEN, freq_str, MMIALM_FREQ_MAX_LEN, str_t.wstr_len);/*lint !e64*/
            if(MMIAPIALM_IsValidFM(str_t.wstr_ptr, str_t.wstr_len))
            {
                detail_win_param->event_content_info.ring_type = ALM_FM_RING;
                detail_win_param->event_ring_info.dring_name_len = str_t.wstr_len;

                SCI_MEMSET(detail_win_param->event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
                MMI_WSTRNCPY(detail_win_param->event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                str_t.wstr_ptr, str_t.wstr_len, str_t.wstr_len);
                detail_win_param->event_content_info.ring = MMISET_MAX_RING_ID;
                
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
				MMK_CloseWin(MMISCH_RINGLIST_WIN_ID);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ALARM_FM_INVALID_PROMPT);
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
#endif  //FM_SUPPORT
    return recode;
}
#ifdef FM_SUPPORT//CR132759
/*****************************************************************************/
//  Description : ����Ƶ�ʱ༭���ڿؼ�����
//  Global resource dependence : 
//  Author: liyanzhu
//  Note:   cr132759 liyan.zhu modify local->public
/*****************************************************************************/
PUBLIC void MMIAPIALM_SetFMEditCtrlParam(MMI_WIN_ID_T win_id)
{
	SetFMEditCtrlParam(win_id);
}
/*****************************************************************************/
//  Description : ����ӿڣ���ALARM_FM_EDITWIN_TAB
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:   cr132759 liyan.zhu modify local->public
/*****************************************************************************/
PUBLIC void MMIAPIALM_OpenFMEditWin(void* param_ptr)
{
	MMK_CreateWin((uint32*)ALARM_FM_EDITWIN_TAB, (ADD_DATA)param_ptr);
}
#endif
/*****************************************************************************/
//  Description : ����Ƶ�ʱ༭���ڿؼ�����
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   cr132759 liyan.zhu modify local->public
/*****************************************************************************/
LOCAL void SetFMEditCtrlParam(MMI_WIN_ID_T win_id)
{
#ifdef MMI_PDA_SUPPORT
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data_info = {0};/*lint !e64*/

    GUILIST_SetMaxItem(MMIALM_SELECT_FM_LIST_CTRL_ID, 2, FALSE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data_info;
    item_data_info.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data_info.item_content[0].item_data.text_id = TXT_COMM_CHANNEL_LIST;
    GUILIST_AppendItem( MMIALM_SELECT_FM_LIST_CTRL_ID, &item_t );

#ifndef MMI_GUI_STYLE_MINISCREEN
    item_data_info.item_content[0].item_data.text_id = TXT_ALARM_CUSTOM_FM;
#else
    item_data_info.item_content[0].item_data.text_id = TXT_ALARM_CUSTOM_FM_TEXT;
#endif
    GUILIST_AppendItem( MMIALM_SELECT_FM_LIST_CTRL_ID, &item_t );
    
#else
    wchar  dot_str[] = {'.'};
    GUIFORM_SetType(MMI_FM_EDITWIN_FORM_CTRL_ID, GUIFORM_TYPE_TP);
    GUIFORM_SetStyle(MMI_FM_EDITWIN_LIST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	GUILABEL_SetTextById(MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID,TXT_COMM_CHANNEL_LIST,FALSE);
#if 0
	// ��Mocor��ʽ
    // GUIFORM_SetStyle(MMI_FM_EDITWIN_FRE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifndef MMI_GUI_STYLE_MINISCREEN    
    // GUILABEL_SetTextById(MMI_FM_EDITWIN_FRE_LABEL_CTRL_ID,TXT_ALARM_CUSTOM_FM,FALSE);
#else
    // GUILABEL_SetTextById(MMI_FM_EDITWIN_FRE_LABEL_CTRL_ID,TXT_ALARM_CUSTOM_FM_TEXT,FALSE);
#endif
#else
	// Mocor 20b��ʽ
	GUIEDIT_SetStyle(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, GUIEDIT_STYLE_MULTI_DYN_DOWN);
	GUIEDIT_SetFontColor(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, MMI_BLACK_COLOR);
#ifndef MMI_GUI_STYLE_MINISCREEN    
	CTRLBASEEDIT_SetLabelId(win_id, MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, TXT_ALARM_CUSTOM_FM, FALSE);
#else
	CTRLBASEEDIT_SetLabelId(win_id, MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, TXT_ALARM_CUSTOM_FM_TEXT, FALSE);
#endif
#endif // if 0
	GUIEDIT_SetIm(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);
    GUIEDIT_ConfigImCustomKey(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
    GUIEDIT_SetImTag(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, GUIIM_TAG_DOT_NUM);

#ifdef  MMI_GUI_STYLE_MINISCREEN      
    GUIEDIT_SetDefaultStringById(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, TXT_ALARM_CUSTOM_FM_FREQ);
#endif
#endif
}

/*****************************************************************************/
//  Description : �ж������Ƿ���Ҫ�ں�̨����
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsNeedBackgroundPrompt(void)
{
    BOOLEAN result =FALSE;

    if (MMIAPIATV_IsRecording()
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
#if defined(CMCC_UI_STYLE)
        || (MMIAPICC_IsInState(CC_IN_CALL_STATE))
#endif
        )
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : ��ȡ��������
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
PUBLIC  void MMIAPIALM_GetAlarmName(uint16 index,MMI_STRING_T* str_ptr)
{
    MMIACC_SMART_EVENT_T    smart_event = {0};
    uint16 event_id = (uint16)( EVENT_ALARM_0 + index );
    MMI_TEXT_ID_T           txt_alarm_id[] = {  TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,
                                                TXT_ALARM_CLOCK10};
    MMI_STRING_T name_str = {0};
    
    MMIAPIALM_FormInteEventInfo(event_id,&smart_event);

    if (event_id <= EVENT_ALARM_MAX)
    {
        MMI_GetLabelTextByLang(txt_alarm_id[event_id],&name_str);
        if (0 == smart_event.event_content_info.w_length)
        {
            str_ptr->wstr_ptr = name_str.wstr_ptr;
            str_ptr->wstr_len = name_str.wstr_len;
        }
        else
        {
            if(PNULL != str_ptr->wstr_ptr)
            {
                MMI_WSTRNCPY(
                        str_ptr->wstr_ptr, str_ptr->wstr_len,
                        smart_event.event_content_info.w_content, 
                        smart_event.event_content_info.w_length,
                        smart_event.event_content_info.w_length
                        );
                
                str_ptr->wstr_len= smart_event.event_content_info.w_length;
            }
        }       
    }
    else
    {
    }
}

/*****************************************************************************/
//  Description : ���ڹػ���������ʱ��ʼ��lcd����
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
/*LOCAL void initLcdBavkground(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T               rect = { 0};
    uint16 w=0;
    uint16 h=0;
    if ( (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition()) || (STARTUP_CHARGE == MMIAPIPHONE_GetStartUpCondition()))
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&w,&h);
        rect.right =w-1;
        rect.bottom =h-1;    
        LCD_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR); 
    }
}*/
#ifdef MMI_MOCOR_SAMPLE
/*****************************************************************************/
//  Description : sample code �������ã��벻Ҫɾ��
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIALM_AddTestListCtrlChild(MMI_CTRL_ID_T ctrl_id)
{
    uint16 i = 0; 

    GUILIST_SetMaxItem( ctrl_id, ALM_ALARM_NUM, FALSE );

    for (i = 0; i< ALM_ALARM_NUM; i++)
    {
        uint16 event_id = (uint16)( EVENT_ALARM_0 + i );
        const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );
        
        AppendTwoLineListItem( ctrl_id,
            TXT_NULL,
            s_alarm_mode_text[event_ptr->fre_mode],
            event_id,
            i,
            FALSE );
// #ifdef MMI_GUI_STYLE_MINISCREEN
// 	 	SetListItemStyle(ctrl_id, i);
// #endif
    }
}
#endif
#ifdef MMI_AZAN_SUPPORT
/*****************************************************************************/
// Description : get event table
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T * MMIAPIALM_GetEventFastTab(void)
{
    return s_event_fast_table;
}
/*****************************************************************************/
// Description : find latest event
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_FindLatestEventToSet(void)
{
    return FindLatestEventToSet();
}
#endif

/*****************************************************************************/
// Description : MMIAPIALM_CheckandStartExpiredEvent
// Global resource dependence : 
// Author:  Jesse.Yu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIALM_CheckandStartExpiredEvent(void)
{
     CheckandStartExpiredEvent();
}

/*****************************************************************************/
// Description : MMIAPIALM_IsAlarmActive
// Global resource dependence : 
// Author:  renyi.hu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALM_IsAlarmActive(void)
{
    BOOLEAN result=FALSE;
    if(MMK_IsFocusWin(MMIALM_ALIVEWIN_ID)
        &&(s_event_timer_id!=0)) 
    {
        result=TRUE;
    }
    return result;
}
//#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   Modifed by leon.wang on 2012.5.28 for Reducing code size
/*****************************************************************************/
PUBLIC uint16 MMIAPIALARM_GetFreModeString(MMIACC_MODE_TYPE_E  fre_mode,uint8 fre,wchar* buff,uint16 buff_max_len)
{
    MMI_STRING_T name_str = {0};
    uint16       len = 0;

#ifndef MMI_ALARM_MINI_SUPPORT
    if(0!=fre && ALM_MODE_EVERYWEEK==fre_mode)
    {
        uint8   week_day = 0;
        wchar   space[2] = {0x20,0};
        uint32  day_num  = 0;

        for(day_num = 0; day_num < ALM_WEEK_DAY_NUM; day_num++)   
        {
            week_day = MMIAPISET_GetWeekDayByOffset(day_num);
            if(0!=(fre & s_week_mask[week_day]) ) 
            {
                MMI_GetLabelTextByLang(s_alm_week_text_id[week_day], &name_str);
                if((MMIAPICOM_Wstrlen(buff)+name_str.wstr_len+1)<buff_max_len)
                {
                    MMIAPICOM_Wstrncat(buff, name_str.wstr_ptr, name_str.wstr_len); 
                    MMIAPICOM_Wstrncat(buff, space, 1); 
                }
            }
        }
    }
    else
#endif
    {
        if(ALM_MODE_NUM>fre_mode)
        {    
            MMI_GetLabelTextByLang(s_alarm_mode_text[fre_mode], &name_str);
            MMIAPICOM_Wstrncpy(buff, name_str.wstr_ptr, MIN(name_str.wstr_len, buff_max_len));
        }
    }

    len=MMIAPICOM_Wstrlen(buff);
    return len;
}
//#endif

/*******************************************************************/
//  Interface:      MMIALM_TimeUpdateCallback
//  Description :   MMIALM_TimeUpdateCallback
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIALM_TimeUpdateCallback(uint32 old_time, uint32 new_time)
{
	Alarm_SetModifyEventDateFlag(FALSE);
    //update alarm event
    MMIALM_UpdateEvents();
	Alarm_SetModifyEventDateFlag(TRUE);
    //set alarm icon in idle
    if (MMIALM_HasActiveEvent())
    {
        MMIAPICOM_StatusAreaSetAlarmState(TRUE);
    }
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id = MMIALM_EDIT_NAME_EDITBOX_CTRL_ID;

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
         {
            MMI_STRING_T  name_str        = {0};
            MMI_STRING_T  name_str1      = {0};
            MMI_STRING_T  text                = {0};
            wchar           forbid_char_value[]= {0x0d, 0x0a};
            MMI_TEXT_ID_T txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                    TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                    TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10};
            MMI_TEXT_ID_T txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                            TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                            TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};

			if (s_current_event_info.id>=EVENT_ALARM_0&&s_current_event_info.id <= EVENT_ALARM_MAX)/*lint !e685 !e568*/
			{
				MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str1);//coverity 12728
			}
            if (!MMIAPICOM_CompareTwoWstrExt(name_str1.wstr_ptr,
                                                name_str1.wstr_len,
                                                s_current_event_info.event.event_content_info.w_content,
                                                s_current_event_info.event.event_content_info.w_length,
                                                TRUE)
                ||(0 == s_current_event_info.event.event_content_info.w_length) )
            {
                if (s_current_event_info.id>=EVENT_ALARM_0&&s_current_event_info.id <= EVENT_ALARM_MAX)/*lint !e685 !e568*/
                {
                    MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str);
                }
                else if(s_current_event_info.id>=EVENT_ALARM_DELAY_0&&s_current_event_info.id <= EVENT_ALARM_DELAY_MAX)
                {
                    MMI_GetLabelTextByLang(txt_alarm_id_1[s_current_event_info.id-EVENT_ALARM_DELAY_0],&name_str);
                }
            }
            else
            {
                name_str.wstr_ptr = s_current_event_info.event.event_content_info.w_content;
                name_str.wstr_len = s_current_event_info.event.event_content_info.w_length;
            }

            GUIEDIT_SetString(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, name_str.wstr_ptr, name_str.wstr_len);
            MMI_GetLabelTextByLang(STXT_MAIN_MESSAGE,&text);
            GUIWIN_SetTitleText(win_id, text.wstr_ptr, text.wstr_len, FALSE);
            GUIEDIT_ConfigDisableChar(ctrl_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            MMK_SetAtvCtrl(win_id,ctrl_id);

       }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveNameEditWin())
        {
            MMK_CloseWin(win_id);
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : HandleEditTimeMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTimeMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    GUIEDIT_TIME_STYLE_E time_style =GUIEDIT_TIME_STYLE_24;
    MMISET_TIME_DISPLAY_TYPE_E time_type=MMISET_TIME_24HOURS;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIFORM_IsSlide(MMIALM_TIME_FORM_CTRL_ID,FALSE);
            time_type = MMIAPISET_GetTimeDisplayType();
            if (MMISET_TIME_12HOURS == time_type)
            {
                time_style = GUIEDIT_TIME_STYLE_12;
            }
            else
            {
                time_style = GUIEDIT_TIME_STYLE_24;
            }

            //set time display style
            GUIEDIT_SetTimeStyle(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,PNULL,&time_style,PNULL,FALSE);
            //GUIEDIT_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_SET_TIME_FONT);

            if (s_current_event_info.event.event_fast_info.is_valid )
            {
                GUIEDIT_SetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
                                    s_current_event_info.event.event_fast_info.hour, 
                                    s_current_event_info.event.event_fast_info.minute,
                                    0);
            }
            else
            {
                SCI_TIME_T      sys_time = {0};
                TM_GetSysTime(&sys_time);
                GUIEDIT_SetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,
                                    sys_time.hour, 
                                    sys_time.min,
                                    0);
            }
            MMK_SetAtvCtrl(win_id,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveTimeEditWin())
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);      
        break;
       
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditFmFreMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
#ifdef FM_SUPPORT //pclint
    MMI_CTRL_ID_T            ctrl_id = MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID;
    wchar  dot_str[] = {'.'};
    MMI_STRING_T str={0};

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
        GUIEDIT_ConfigImCustomKey(ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_DOT_NUM);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            GUIEDIT_GetString(ctrl_id, &str);
            if(MMIAPIALM_IsValidFM(str.wstr_ptr, str.wstr_len))
            {
                s_current_event_info.event.event_content_info.ring_type = ALM_FM_RING;
                s_current_event_info.event.event_ring_info.dring_name_len = str.wstr_len;

                SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
                MMI_WSTRNCPY(s_current_event_info.event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                str.wstr_ptr, str.wstr_len, str.wstr_len);
                s_current_event_info.event.event_content_info.ring = MMISET_MAX_RING_ID;
                
                MMK_CloseWin(MMIALM_FM_EDIT_WIN_ID);
                MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ALARM_FM_INVALID_PROMPT);
            }
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
#endif
    return result;
}
#endif

#if defined(SCH_ASSISTANT_WIDGET) && defined(MMI_GRID_IDLE_SUPPORT)
LOCAL void LoadCurSchList(MMI_CTRL_ID_T  ctrl_id)
{
    GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T     item_data= {0};
    uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_1[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_2[GUILIST_STRING_MAX_NUM] = {0};
    
    item_info.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL;
    item_info.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = s_arrived_event.event_content_info.image_id;
    
    item_data.item_content[1].item_data_type = item_data.item_content[2].item_data_type 
        = item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    MMIAPISET_FormatDateStrByDateStyle(s_arrived_event.event_fast_info.start_year,
        s_arrived_event.event_fast_info.start_mon,s_arrived_event.event_fast_info.start_day,
        '/',tmp_item,GUILIST_STRING_MAX_NUM);

    item_data.item_content[1].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)tmp_item);

    item_data.item_content[1].item_data.text_buffer.wstr_ptr = tmp_wstr_1;

    MMI_STRNTOWSTR( tmp_wstr_1,
        item_data.item_content[1].item_data.text_buffer.wstr_len,
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    
	//time
    SCI_MEMSET(tmp_item,0,strlen((char*)tmp_item));/*lint !e666*/
	MMIAPISET_FormatTimeStrByTime(
							 s_arrived_event.event_fast_info.hour,
							 s_arrived_event.event_fast_info.minute,
							 tmp_item,
							 GUILIST_STRING_MAX_NUM
							 );

    item_data.item_content[2].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)tmp_item);

    item_data.item_content[2].item_data.text_buffer.wstr_ptr = tmp_wstr_2;

    MMI_STRNTOWSTR( tmp_wstr_2,
        item_data.item_content[2].item_data.text_buffer.wstr_len,
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    if (0!= s_arrived_event.event_content_info.w_length)
    {
        item_data.item_content[3].item_data.text_buffer.wstr_len = s_arrived_event.event_content_info.w_length;

        item_data.item_content[3].item_data.text_buffer.wstr_ptr = s_arrived_event.event_content_info.w_content;
    }
    else
    {
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[3].item_data.text_id = TXT_BLANK;
    }
    GUILIST_AppendItem(ctrl_id, &item_info);
}
#endif

/*****************************************************************************/
//  Discription: MMIAPIALARM_GetALARMTimerID
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:���alarm TIMER��id�����ճ�widgetʹ��
//
/*****************************************************************************/
PUBLIC uint8 MMIAPIALARM_GetALARMTimerID(void)
{
    return s_event_timer_id;
}

#if defined(SCH_ASSISTANT_WIDGET) && defined(MMI_GRID_IDLE_SUPPORT)
LOCAL MMI_RESULT_E HandleSchPOPAlertWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T form_ctrl = MMISCH_POP_ALERT_FORM_CTRL_ID;
    MMI_CTRL_ID_T list_ctrl = MMISCH_POP_ALERT_LIST_CTRL_ID;
    MMI_CTRL_ID_T anim_ctrl = MMISCH_POP_ALERT_ANIM_CTRL_ID;
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
    uint16        anim_height = 0;
    uint16        anim_width = 0;
    MMI_HANDLE_T  win_handle = 0;
    uint16        list_height = 0;
    uint16        list_width= 0;
    
    win_handle = MMK_ConvertIdToHandle(win_id);

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GUIRES_GetImgWidthHeight(&anim_width, &anim_height, IMAGE_WIDGET_SCHEDULE_ASSISTANT, win_id);
    GUIRES_GetImgWidthHeight(&list_width,&list_height,IMAGE_WIDGET_SCHEDULE_TEXT_BG,win_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIANIM_CTRL_INFO_T     anim_ctrl_info = {0};         
            GUIANIM_DISPLAY_INFO_T  anim_display_info = {0}; 
        	GUIANIM_DATA_INFO_T     anim_data_info = {0};
        	GUIFORM_CHILD_WIDTH_T   anim_ctrl_width  = {0};
        	GUIFORM_CHILD_HEIGHT_T  list_ctrl_height = {0};
        	GUI_RECT_T              list_rect = {0};

        	anim_data_info.anim_id = ANIM_WIDGET_SCHEDULE_ASSISTANT;
            anim_ctrl_info.is_ctrl_id = TRUE;
            anim_ctrl_info.ctrl_id = anim_ctrl;
            anim_ctrl_info.ctrl_ptr = PNULL;
            anim_display_info.is_syn_decode = TRUE;
            anim_display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            anim_ctrl_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            anim_ctrl_width.add_data = anim_width;

            list_ctrl_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            list_ctrl_height.add_data= list_height;
            
            GUIFORM_PermitChildBg(form_ctrl,FALSE);
    		GUILIST_SetUserBg(list_ctrl,TRUE);
    		GUILIST_SetBgImage(list_ctrl, IMAGE_WIDGET_SCHEDULE_TEXT_BG, FALSE);
    		GUILIST_SetListState(list_ctrl, GUILIST_STATE_SPLIT_LINE, FALSE);
    		GUILIST_SetNeedHiLightBar(list_ctrl,FALSE);
    		GUILIST_SetMaxItem(list_ctrl, 1, FALSE);    
            GUIANIM_SetParam(&anim_ctrl_info,&anim_data_info,PNULL,&anim_display_info);
            GUIFORM_PermitChildBorder(form_ctrl, FALSE);
            GUIFORM_SetChildWidth(form_ctrl, anim_ctrl, &anim_ctrl_width);
            GUIAPICTRL_GetRect(list_ctrl, &list_rect);
            GUIFORM_SetChildHeight(form_ctrl, list_ctrl, &list_ctrl_height);
            LoadCurSchList(list_ctrl);
            recode = HandleEventAliveWinMsg(win_id, msg_id, param); 
        }
        break;   
    case MSG_FULL_PAINT:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_KEYUP_POWER:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        MMIAPIALARM_DealWithCancelOprate();
        break;      

    default:
        recode = HandleEventAliveWinMsg(win_id, msg_id, param); 
        break;
    }
    return recode;
}
#endif

#ifdef ATEST_SUPPORT
/*****************************************************************************/
//  Description : get the Next valid Alarm Time
//  Global resource dependence : 
//  Author:  lin.chen
//  Note:   
/*****************************************************************************/
PUBLIC void ATESTALM_GetLatestAlarmTime(SCI_ALARM_T  *s_alm)
{
	MMIACC_SMART_EVENT_FAST_TABLE_T       event           = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T       latest_event    = {0}; 
    MMIACC_SMART_EVENT_FAST_TABLE_T       cur_point       = {0};    
	SCI_TIME_T  cur_time	=	{0}; 
	SCI_DATE_T  cur_date	=	{0}; 
	uint32		i			=	0;
	uint32		event_id	=	0;
    
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);
    cur_point.year = cur_date.year; 
    cur_point.mon = cur_date.mon;
    cur_point.day = cur_date.mday;
    cur_point.hour = cur_time.hour;
    cur_point.minute = cur_time.min;
    latest_event.year = 0xFFFF;
	
	for(i = 0; i< MMIACC_EVENT_MAX_NUM; i++)
    {
        event = s_event_fast_table[i];
        if (event.is_valid && event.is_on)
        {
            if (MMIAPIALM_CompareTwoEvent(&event, &cur_point) == CMP_SECOND_SMALL &&
                MMIAPIALM_CompareTwoEvent(&latest_event, &event) == CMP_SECOND_SMALL)
            {
                event_id = i;
                latest_event = event;
            }
        }
    }
	s_alm->year = latest_event.year;
	s_alm->mon = latest_event.mon;
	s_alm->mday = latest_event.day;
	s_alm->hour = latest_event.hour;
	s_alm->min = latest_event.minute;
}

#endif	//ATEST_SUPPORT
/*****************************************************************************/
//  Description : Request alarm play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALARM_ReqPlaySrv(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;

    if(s_alarm_play_handle != 0)
    {
        res = TRUE;
    }
    else
    {
        req.pri = MMISRVAUD_PRI_NORMAL;
        audio_srv.info.fm.type = MMISRVAUD_TYPE_FM;//�����뻻��alarm��type
        audio_srv.volume = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
		if (0 == audio_srv.volume)//CR200011
        {
            audio_srv.volume = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        audio_srv.all_support_route = MMISRVAUD_ROUTE_EARFREE|MMISRVAUD_ROUTE_SPEAKER;
        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(handle > 0)
        {
            s_alarm_play_handle = handle;
            res = TRUE;
        }
    }
    SCI_TRACE_LOW("[fm]MMIAPIALARM_ReqPlaySrv:alarm_handle %d",s_alarm_play_handle);
    return res;
}
/*****************************************************************************/
//  Description : Get alarm play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIAPIALARM_GetPlayHandle(void)
{
    return s_alarm_play_handle;
}
/*****************************************************************************/
//  Description : Free alarm play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIALARM_FreePlaySrv(void)
{
    if(s_alarm_play_handle != 0)
    {
        MMISRVMGR_Free(s_alarm_play_handle);
        s_alarm_play_handle = 0;
    }
}
/*****************************************************************************/
//  Discription: MMIAPIALARM_DealWithCancelOprate
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:����ر��ճ����ֵ�ʱ�䣬ͬHandleEventAliveWinMsg�е�MSG_APP_CANCELҪ��������
//
/*****************************************************************************/
PUBLIC void MMIAPIALARM_DealWithCancelOprate(void)
{
    StopRingOrVib(TRUE);
    SetDelayAlarmTime(s_arrived_event_id, &s_arrived_event, FALSE);
#ifdef CALENDAR_SUPPORT
    MMIAPISCH_DeleteUnreadEvent(s_arrived_event_id);
#endif
    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
    if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
            &&(0==s_bypast_list.total)
        )
    {
        CheckandStartExpiredEvent();
        //��������²���MMK_CloseWin(win_id);����մ򿪵��Զ��ػ������ֹص��ˣ�
        //�ȵ���MMK_CloseWin�������CheckandStartExpiredEvent�ǿ��Եģ�ֻ�� ������CR225729 ��
    }
    else
    {
        CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO        
    }
}
/*****************************************************************************/
//  Discription: MMIAPIALARM_HandleEventAliveMSG
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:HandleEventAliveWinMsg�ķ�װ�����ⲿʹ��
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E   MMIAPIALARM_HandleEventAliveMSG(
                                                     MMI_WIN_ID_T win_id, 
                                                     MMI_MESSAGE_ID_E msg_id, 
                                                     DPARAM param 
                                                     )
{
    return HandleEventAliveWinMsg(win_id,msg_id,param);
}

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :¼�� �� �� �� ��  
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AlarmRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr)
{
    //MMIFILE_FILE_INFO_T file_info = {0};
    static MMI_STRING_T    str_record_file_path = {0};
    uint32          size_of_param= 0;

    if (PNULL == data_info_ptr) 
    {
        SCI_TRACE_LOW("[MMIALM]OpenRecord data_info_ptr is pnull");
        return; 
    } 

    if (MMIRECORD_WORK_SUCCESS == error_ret)  
    { 
        if (data_info_ptr->file_name_len > 0)
        {
            str_record_file_path.wstr_ptr=(wchar*)SCI_ALLOCA((data_info_ptr->file_name_len+1)*sizeof(wchar));
            if(str_record_file_path.wstr_ptr==PNULL)
            {
                return ;
            }
            SCI_MEMSET(str_record_file_path.wstr_ptr,0,(data_info_ptr->file_name_len+1)*sizeof(wchar));
            MMIAPICOM_Wstrncpy(str_record_file_path.wstr_ptr, data_info_ptr->file_name, data_info_ptr->file_name_len);
            str_record_file_path.wstr_len = data_info_ptr->file_name_len;
            size_of_param = (str_record_file_path.wstr_len+1)*sizeof(wchar);
            MMK_PostMsg(MMIALM_EDIT_WIN_ID,  MMI_MSG_RECORDTORING, (DPARAM)str_record_file_path.wstr_ptr,size_of_param);
            SCI_FREE(str_record_file_path.wstr_ptr);
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIALM]OpenRecord result is not succ");
    }

    return ;
}

/*****************************************************************************/
//  Discription: RecordAlarmRingCallBack
//  Global resource dependence: none 
//  Author: juan.wu
//  Note:¼������Ϊ������callback
//
/*****************************************************************************/
LOCAL MMI_RESULT_E RecordAlarmRingCallBack(void)
{
    MMIRECORD_WORK_PARAM_T parameter={0};

    parameter.max_file_size = 0;
    parameter.max_file_time = 0;
    parameter.record_support_type = (uint32)MMIRECORD_WORK_FORMAT_WAV|(uint32)MMIRECORD_WORK_FORMAT_AMR|(uint32)MMIRECORD_WORK_FORMAT_MP3;
    parameter.work_callback = AlarmRecordWorkCallBack;
    if(MMIAPIRECORD_OpenMainWin(&parameter) != MMIRECORD_WORK_SUCCESS)
    {
        SCI_TRACE_LOW("[MMIALM]OpenRecord alarm record error!");
    }
    return MMI_RESULT_TRUE;
}
#endif

#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
/*****************************************************************************/
//  Description : when alarm interrupt is coming, relative alarm will 
//                be activated.
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJumpClockEventAliveWinMsg(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_STRING_T            temp_str = {0};
    uint16 event_id=0;
    //uint16      width = 0; 
    //uint16      height = 0; 
#if defined MAINLCD_SIZE_240X400 || defined MAINLCD_SIZE_320X480
    GUI_POINT_T point;
#endif
    GUI_RECT_T   win_rect ={0};
    SCI_TIME_T  sys_time ={0};
    SCI_DATE_T  sys_date = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		if (SMART_EVENT_ALARM == s_arrived_event.event_fast_info.type)
		{
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
			GUIWIN_SetSoftkeyTextId(win_id, STR_POC_ILIST_OFF, TXT_NULL,TXT_ALARM_DELAY, FALSE);
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
			GUIWIN_SetSoftkeyTextId(win_id, STR_POC_ILIST_OFF, TXT_NULL,TXT_ALARM_DELAY, FALSE);
#endif
            // GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_CLOSE,TXT_NULL, FALSE);
		}
        	
		//ˢ��С����ʾ����ʾ���ӵ��¼���Ϣ
        if (SMART_EVENT_ALARM ==s_arrived_event.event_fast_info.type) // assigne default text                
        {                
            MMI_GetLabelTextByLang(TXT_ALARM_CLOCK, &temp_str);                
        }
        else                
        {                
            temp_str.wstr_len = s_arrived_event.event_content_info.w_length;                 
            temp_str.wstr_ptr = s_arrived_event.event_content_info.w_content;                 
        } 
        
        
        StartAlarmTimer(&s_event_timer_id, ALERT_RING_DURATION, FALSE);
        
        StartRingOrVib();//U��ʹ���е����Ҳ�ڸú����д���
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);//��ֹ����
        s_clk_timer_id = MMK_CreateWinTimer(win_id, 100, TRUE);

        //�����ǵ�����Ļ�Ĳ���,�����MMIPUB_HandleWaitWinMsg֮����
        if ( STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition())
        {
            MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        else
        {
            MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_ALARM,&temp_str);    
        break;	
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
                break;           
            default:
                break;
            }
        }
        break;       
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_FULL_PAINT:
        {
            
            //GUI_POINT_T point={0};
            MMK_GetWinRect(win_id, &win_rect);

            TM_GetSysTime(&sys_time);

            TM_GetSysDate(&sys_date);
   
            GUIRES_DisplayImg(PNULL,
		        &win_rect,
		        &win_rect,
		        win_id,
		        IMG_JUMP_WIN_BG,
		        &lcd_dev_info);
            sys_date.mday=s_arrived_event.event_fast_info.day;
	        sys_date.mon=s_arrived_event.event_fast_info.mon;
	        sys_date.year=s_arrived_event.event_fast_info.year;
	        sys_time.hour=s_arrived_event.event_fast_info.hour;
	        sys_time.min=s_arrived_event.event_fast_info.minute;            
            MMIAPIALARM_DisplayAlmClock(win_id, sys_time, win_rect, s_rotate_info_arr[s_pic_index].angel);
#if defined MAINLCD_SIZE_240X400 || defined MAINLCD_SIZE_320X480
            TM_GetSysTime(&sys_time);

            TM_GetSysDate(&sys_date);
            point.x = CLK_TIME_X;
            point.y = CLK_TIME_Y;
            MMIAPIALARM_DrawTime(sys_time, point);
            point.x = CLK_DATE_X;
            point.y = CLK_DATE_Y;
            MMIAPIALARM_DrawDate(sys_date, point); 
#endif
        }
        break;
        
    case MSG_GET_FOCUS:
        if(s_clk_timer_id == 0)
        {
        
            s_clk_timer_id = MMK_CreateWinTimer(win_id, 100, TRUE);
        }       
        if (s_event_timer_id ==0)
        {
            if (MMIAPIKL_IsPhoneLocked())
	        {
	            MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
	        }
		}
		else
		{
			if ( STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition())
	        {
                    MMIDEFAULT_TurnOnBackLight();
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            }
            else
            {
                MMIDEFAULT_TurnOnBackLight();
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            }
            if(MMIAPIKL_IsPhoneLocked())
            {
                MMK_PostMsg(win_id,MSG_ALM_START_RING,PNULL,0);//�������Ҷ�������������
            }
            else
            {
                StartRingOrVib();
            }
        }
        break;
    case MSG_ALM_START_RING:
        StartRingOrVib();
        break;
     
    case MSG_LOSE_FOCUS:
        if(s_clk_timer_id != 0)
        {
        
            MMK_StopTimer(s_clk_timer_id);
            s_clk_timer_id = 0;
        }
        StopRingOrVib(FALSE);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_PROMPTWIN_OK:    
        if (s_arrived_event_id < EVENT_SCH_0)
        {
            if(s_arrived_event_id>=EVENT_ALARM_0&&s_arrived_event_id<=EVENT_ALARM_MAX)/*lint !e685 !e568*/
            {
                event_id = s_arrived_event_id;
            }
            else if(s_arrived_event_id>=EVENT_ALARM_DELAY_0&&s_arrived_event_id<=EVENT_ALARM_DELAY_MAX)
            {
               event_id = s_arrived_event_id-EVENT_ALARM_DELAY_0; 
            }
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_DeleteUnreadEvent(event_id);
#endif            
            StopRingOrVib(TRUE);
            CloseDelayAlarm(s_arrived_event_id, s_arrived_event.event_fast_info, FALSE);
            if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
                &&(0==s_bypast_list.total)
                )
            {
                CheckandStartExpiredEvent();
                //��������²���MMK_CloseWin(win_id);����մ򿪵��Զ��ػ������ֹص��ˣ�
                //�ȵ���MMK_CloseWin�������CheckandStartExpiredEvent�ǿ��Եģ�ֻ�� ������CR225729 ��
            }
            else
            {
                CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO
                MMK_CloseWin(win_id);
            }

            
        }
        break;
        
    case MSG_TIMER:
        if (s_vibrator_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            if (is_vibrator_on)
            {
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_ALARM);
                is_vibrator_on = FALSE;
            }
            else
            {
                MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
                is_vibrator_on = TRUE;
            }
            StartAlarmTimer(&s_vibrator_timer_id, MMI_1SECONDS, FALSE);
        }
        else if (s_event_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            // ֹͣ����
            if(is_ring_on)
            {
                StopRingOrVib(TRUE);
            }
            else
            {
                MMK_StopTimer(s_event_timer_id);
                s_event_timer_id = 0;
            }
            
            if(s_vibrator_timer_id!=0)
            {
                MMK_StopTimer(s_vibrator_timer_id);
                s_vibrator_timer_id = 0;
            }
            is_vibrator_on = FALSE;
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_AddUnreadEvent(s_arrived_event_id);
#endif
            if (MMIAPIKL_IsPhoneLocked())
            {
                if(MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)||MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNDEAL_EVENT,PNULL,0);
                }
            }
            // ������ʱ������
            SetDelayAlarmTime(s_arrived_event_id, &s_arrived_event, TRUE);
            
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            
            MMK_CloseWin(win_id);
            CheckandStartExpiredEvent();
        }
        else if (s_clk_timer_id == *((uint8 *) param))
        {
            s_pic_index =(s_pic_index+1)%CLK_ROTATE_PIC_NUM;
            MMK_UpdateScreen();   
        }            
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP://�ϸ�Ӧ�úͰ�ֹͣ��ͬ������,@zhaohui,cr102341
        StopRingOrVib(TRUE);
        CloseDelayAlarm(s_arrived_event_id, s_arrived_event.event_fast_info, FALSE);
        MMK_CloseWin(win_id);
        CheckandStartExpiredEvent();
        recode = MMI_RESULT_FALSE;
        break; 
#endif
        
    case MSG_CLOSE_WINDOW:
        if(s_event_timer_id != 0)
        {
            SetDelayAlarmTime(s_arrived_event_id, &s_arrived_event, TRUE);
        }
        if(s_clk_timer_id != 0)
        {
        
            MMK_StopTimer(s_clk_timer_id);
            s_clk_timer_id = 0;
        }      
        StopRingOrVib(TRUE);
        
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        //ˢ��С����ʾ���ر���ʾ���ӵ��¼���Ϣ
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_ALARM,PNULL);
        MMIAPICT_CheckandStartDelayedCoutimeEvent();//@zhaohui,counted time,�����ӳٵĵ���ʱ����ʾ
        break;
        
    case MSG_KEYDOWN_RED:
    case MSG_KEYUP_POWER:
        StopRingOrVib(TRUE);
        SetDelayAlarmTime(s_arrived_event_id, &s_arrived_event, FALSE);
        
        if (MMIAPIKL_IsPhoneLocked()||
            (s_bypast_list.total > 0)||
            (s_poweroff_event.event_fast_info.is_valid)||
            MMIAPICT_CheckDelayedCoutimeEvent()||
            MMK_IsOpenWin(MMISET_FLY_MODE_OPEN_PHONE_WIN_ID)
            )//����������ʱ�¼�����ֻ�رյ�ǰ����
        {
            recode = MMI_RESULT_TRUE;   
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }

        if (MMI_RESULT_TRUE == recode||MSG_KEYUP_POWER==msg_id)
        {
            MMK_CloseWin(win_id);
        }

        CheckandStartExpiredEvent();
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_PROMPTWIN_CANCEL:
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYUP_UPSIDE:
    case MSG_KEYUP_DOWNSIDE:
    case MSG_KEYLONG_UPSIDE:
    case MSG_KEYLONG_DOWNSIDE:
    case MSG_KEYPRESSUP_DOWNSIDE:
    case MSG_KEYPRESSUP_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        StopRingOrVib(TRUE);
        SetDelayAlarmTime(s_arrived_event_id, &s_arrived_event, FALSE);

        
#ifdef CALENDAR_SUPPORT       
        MMIAPISCH_DeleteUnreadEvent(s_arrived_event_id);
#endif       
        if(((s_poweroff_event.event_fast_info.is_valid) && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
                &&(0==s_bypast_list.total)
            )
        {
            CheckandStartExpiredEvent();
            //��������²���MMK_CloseWin(win_id);����մ򿪵��Զ��ػ������ֹص��ˣ�
            //�ȵ���MMK_CloseWin�������CheckandStartExpiredEvent�ǿ��Եģ�ֻ�� ������CR225729 ��
        }
        else
        {
            CheckandStartExpiredEvent();//FOR CR225729 MARYXIAO
            MMK_CloseWin(win_id);
        }
        break;
        
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_KEYUP_FLIP:
        MMIDEFAULT_HandleFlipKey(TRUE);  
        break;
#endif
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#ifdef MMI_ALARM_DEL_ADD_SUPPORT
/*****************************************************************************/
//Description :   ALARM OPT
//Global resource dependence :  none
//Author: panvs
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmMainOptWin(
                                           MMI_WIN_ID_T    win_id,   // ����ID
                                           MMI_MESSAGE_ID_E   msg_id,   // ��ϢID
                                           DPARAM             param     // ��Ϣ����
                                           )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIALM_OPT_MENU_CTRL_ID;    
    uint16 total_alm_num = GUILIST_GetTotalItemNum(MMIALM_LISTALL_CTRL_ID);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(0 == total_alm_num)
        {
            GUIMENU_SetItemGrayed(ctrl_id, MENU_ALARM_OPT, ID_ALM_OPT_EDIT, TRUE);
            GUIMENU_SetItemGrayed(ctrl_id, MENU_ALARM_OPT, ID_ALM_OPT_DEL, TRUE);
        }
        else if(ALM_ALARM_NUM <= total_alm_num)
        {
            GUIMENU_SetItemGrayed(ctrl_id, MENU_ALARM_OPT, ID_ALM_OPT_ADD, TRUE);
        }        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint32      group_id    =   0 ;
            uint32      menu_id     =   0;
            GUIMENU_GetId(ctrl_id,&group_id,&menu_id);

            switch(menu_id)
            {
            case ID_ALM_OPT_ADD:
                {
                    uint16 id = FindAlarmUnuseId();
                    
                    if(ALM_ALARM_NUM > id)
                    {
                        s_current_event_info.id = id;
                        s_current_event_info.type = SMART_EVENT_ALARM;
                        
                        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);	
                        if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
                        {
                            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
                            s_current_event_info.event.event_content_info.ring = 1;
                        }

                        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_content_info.is_add = TRUE;
                        MMK_CreateWin((uint32*)ALARM_EDITWIN_TAB, PNULL);
                    }
                }
                break;

            case  ID_ALM_OPT_EDIT:
                {
                    uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                    
                    if(ALM_ALARM_NUM > id)
                    {
                        s_current_event_info.id = id;

                        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);	
                        if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
                        {
                            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
                            s_current_event_info.event.event_content_info.ring = 1;
                        }
                        s_current_event_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;

                        MMK_CreateWin((uint32*)ALARM_EDITWIN_TAB, PNULL);
                    }
                }
                break;

            case ID_ALM_OPT_DEL: 
                {
                    uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                    uint16 total_num = 0;
                    if(ALM_ALARM_NUM > id)
                    {
                        s_current_event_info.id = id;

                        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                        s_current_event_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.is_on = FALSE;
                        s_current_event_info.event.event_content_info.w_length = 0;
                        s_current_event_info.event.event_content_info.is_add = FALSE;
                        SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
                        MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);	
                        GUILIST_RemoveItem(MMIALM_LISTALL_CTRL_ID, GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                        total_num = GUILIST_GetTotalItemNum(MMIALM_LISTALL_CTRL_ID);
                        if(0 == total_num)
                        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
							GUIWIN_SetSoftkeyTextId(MMIALM_MAINWIN_ID, 
								STXT_OPTION, 
								TXT_NULL, 
								TXT_COMMON_COMMON_BACK,
                                                    FALSE);
							GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
                            GUIWIN_SetSoftkeyTextId(MMIALM_MAINWIN_ID, 
                                                    STXT_OPTION, 
                                                    IMAGE_NULL, 
                                                    TXT_COMMON_COMMON_BACK,
                                                    FALSE);
#endif
                        }
                        MMK_PostMsg(MMIALM_MAINWIN_ID, MSG_FULL_PAINT, PNULL, 0);	
                    }
                }
                break;

            default:
                break;
            } 
            MMK_CloseWin( win_id );    
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
} 

/*****************************************************************************/
//Description :   GetAlarmRealId
//Global resource dependence :  none
//Author: panvs
//Note: 
/*****************************************************************************/
LOCAL uint16  GetAlarmRealId(uint16 id)
{
    uint16 i = 0;
    uint16 num = 0;
    
    for ( i = 0; i < ALM_ALARM_NUM; i++ )
    {
        MMIACC_SMART_EVENT_T smart_event = {0};

        MMIAPIALM_FormInteEventInfo(i, &smart_event);
        if(smart_event.event_content_info.is_add)
        {
            if(id == num)
            {
                return i;
            }
            num++;
        }
    }

    return ALM_ALARM_NUM;
}

LOCAL uint16  FindAlarmUnuseId(void)
{
    uint16 i = 0;
    
    for ( i = 0; i < ALM_ALARM_NUM; i++ )
    {
        MMIACC_SMART_EVENT_T smart_event = {0};

        MMIAPIALM_FormInteEventInfo(i, &smart_event);
        if(!smart_event.event_content_info.is_add)
        {
            return i;
        }
    }

    return ALM_ALARM_NUM;
}
#endif//CMCC_UI_STYLE
//CR132925
// #ifdef MMI_GUI_STYLE_MINISCREEN
// LOCAL void SetListItemStyle(MMI_CTRL_ID_T ctrl_id, uint16 index)
// {
// #if defined(MAINLCD_SIZE_128X160) && defined(MULTI_LAN_SUPPORT_DE)&&(!defined MMI_THEME_LIST_MINI_SUPPORT)
//     THEMELIST_ITEM_STYLE_T* pitem_style = THEMELIST_GetStyle(GUIITEM_STYLE_THREE_LINE_TEXT_ICON);
// #else
//     THEMELIST_ITEM_STYLE_T* pitem_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL);
// #endif
// 	
// 	if (pitem_style)
// 	{	
//         SCI_MEMCPY(&s_my_style, pitem_style, sizeof(THEMELIST_ITEM_STYLE_T));
// #if defined(MAINLCD_SIZE_128X160) && defined(MULTI_LAN_SUPPORT_DE)
//         s_my_style.content[0].rect.left = s_my_style.content[0].rect_focus.left = ALARM_MODE_LEFT;
//         s_my_style.content[1].rect.left = s_my_style.content[1].rect_focus.left = ALARM_MODE_LEFT;
//         s_my_style.content[2].rect.left = s_my_style.content[2].rect_focus.left = ALARM_MODE_LEFT;
//         s_my_style.content[0].rect.right = s_my_style.content[0].rect_focus.right = ALARM_MODE_RIGHT;
//         s_my_style.content[1].rect.right = s_my_style.content[1].rect_focus.right = ALARM_MODE_RIGHT;
//         s_my_style.content[2].rect.right = s_my_style.content[2].rect_focus.right = ALARM_MODE_RIGHT;
//         s_my_style.content[3].rect.left = s_my_style.content[3].rect_focus.left = ALARM_ICON_LEFT;
//         s_my_style.height = 50;
//         s_my_style.content[0].rect.top = s_my_style.content[0].rect_focus.top = 0;
//         s_my_style.content[0].rect.bottom = s_my_style.content[0].rect_focus.bottom = s_my_style.content[0].rect.top + 16;
//         s_my_style.content[1].rect.top = s_my_style.content[1].rect_focus.top = s_my_style.content[0].rect.bottom + 1;
//         s_my_style.content[1].rect.bottom = s_my_style.content[1].rect_focus.bottom = s_my_style.content[1].rect.top + 16;
//         s_my_style.content[2].rect.top = s_my_style.content[2].rect_focus.top = s_my_style.content[1].rect.bottom + 1;
//         s_my_style.content[2].rect.bottom = s_my_style.content[2].rect_focus.bottom = s_my_style.content[2].rect.top + 16;
//         s_my_style.content[3].rect.top = s_my_style.content[3].rect_focus.top = s_my_style.content[1].rect.top;
//         s_my_style.content[3].rect.bottom = s_my_style.content[3].rect_focus.bottom = s_my_style.content[1].rect.bottom;
// 		s_my_style.content[0].scale_left = s_my_style.content[0].scale_right =
// 			s_my_style.content[1].scale_left = s_my_style.content[1].scale_right = 
// 			s_my_style.content[2].scale_left = s_my_style.content[2].scale_right =
// 			s_my_style.content[3].scale_left = s_my_style.content[3].scale_right = 0;
// 
// #else
// 		s_my_style.content[3].rect.left = s_my_style.content[3].rect_focus.left = ALARM_MODE_LEFT;
// 		s_my_style.content[3].rect.right = s_my_style.content[3].rect_focus.right = ALARM_MODE_RIGHT;
// 		s_my_style.content[1].rect.left = s_my_style.content[1].rect_focus.left = ALARM_ICON_LEFT;
// 		s_my_style.content[0].scale_left = s_my_style.content[0].scale_right =
// 			s_my_style.content[1].scale_left = s_my_style.content[1].scale_right = 
// 			s_my_style.content[2].scale_left = s_my_style.content[2].scale_right =
// 			s_my_style.content[3].scale_left = s_my_style.content[3].scale_right = 0;
// 
// #endif
// 		GUILIST_SetItemStyleEx(ctrl_id, index, &s_my_style);
// 	}
// }
// #endif 

/*Edit by script, ignore 7 case. Thu Apr 26 19:00:48 2012*/ //IGNORE9527


/*Edit by script, ignore 13 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527

//[add new alarm list UI]
LOCAL  MMI_RESULT_E CustomAlarmMainWinHookFunc(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    MMI_CTRL_ID_T               list_ctrl_id    = MMIALM_LISTALL_CTRL_ID;
    if (msg_id == MSG_APP_OK
            && MMK_GetActiveCtrlId(win_id) == list_ctrl_id
            && GUILIST_GetTotalItemNum(list_ctrl_id) > 0)
    {
        MMK_CreateWin((uint32 *)MMIALM_MAIN_OPTWIN_TAB, PNULL);
         result = MMI_RESULT_TRUE;
    }

    return result;
}

LOCAL MMI_RESULT_E HandleCustomAlarmMainWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    uint16                      i               = 0;
    MMI_RESULT_E                recode          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               current_ctrl_id = MMIALM_MAINWIN_FORM_CTRL_ID;
    MMI_CTRL_ID_T               btn_ctrl_id     = MMIALM_NEW_ALARM_CTRL_ID;
    MMI_CTRL_ID_T               list_ctrl_id    = MMIALM_LISTALL_CTRL_ID;
    const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = NULL;
    GUI_BG_T        bg_info = {0};
    GUIFORM_CHILD_HEIGHT_T    button_height  = {0};
    GUIFORM_CHILD_HEIGHT_T    list_height    = {0};
    //uint32 duration_time = 0;
//#if !defined MMI_GUI_STYLE_TYPICAL
//    SCI_TIME_T      sys_time = {0};
//    uint16          width = 0;
//    uint16          height = 0;
//    GUI_RECT_T      rect_info =MMITHEME_GetClientRect();
//#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:      //create check list box

        GUIFORM_PermitChildFont(current_ctrl_id,FALSE);
        GUIFORM_SetCircularHandleUpDown(current_ctrl_id,TRUE);
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        button_height.add_data = CHILD_FORM_BUTTON_HIGHT;
        GUIFORM_SetChildHeight(current_ctrl_id, btn_ctrl_id, &button_height);
        list_height.type = GUIFORM_CHILD_HEIGHT_FIXED;//GUIFORM_CHILD_HEIGHT_LEFT;//change by James li
        list_height.add_data = 240;//add by James li
        GUIFORM_SetChildHeight(current_ctrl_id, list_ctrl_id, &list_height);
        GUIFORM_SetChildUpDownPostion(current_ctrl_id,TRUE);
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_HEADER_ALARM_CLOCK);
        AppendNewAlarmItem(btn_ctrl_id);
        //SetAlarmListData(win_id, list_ctrl_id, btn_ctrl_id);
        MMK_SetWinHookFunc( win_id, CustomAlarmMainWinHookFunc);
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;

#if !defined MMI_GUI_STYLE_TYPICAL
    case MSG_TIMER:
        if((*(uint8*)param) == s_refresh_timer_id)
        {
            MMK_UpdateScreen();
        }
        break;
#endif

    case MSG_LOSE_FOCUS:
// #if !defined MMI_GUI_STYLE_TYPICAL
//  //       StopClockTimer();
// #endif
        break;

    case MSG_GET_FOCUS:
// #if !defined MMI_GUI_STYLE_TYPICAL
//         //s_refresh_timer_id = CreateClockTimer(win_id, s_alm_clk_type);
// #endif
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_HEADER_ALARM_CLOCK);
        break;

     case MSG_LCD_SWITCH:
     #if 0
        if(!MMITHEME_IsMainScreenLandscape())//only display when not landscape
        {
            //��ͼ����Ӧ����widget��ͼ
            GUIRES_GetImgWidthHeight(&width, &height, IMAGE_WIDGET_ACLOCK, win_id);
            rect_info.top = rect_info.top + height + MMI_STATUSBAR_HEIGHT;//+ MMITHEME_GetWinTitleHeight();//+ MMI_STATUSBAR_HEIGHT;
            GUILIST_SetRect(current_ctrl_id, &rect_info);
        }
      #endif
         break;

    case MSG_FULL_PAINT:
        SetAlarmListData(win_id, list_ctrl_id, btn_ctrl_id);
        break;

    case MSG_NOTIFY_CONTENT_OVERFLOW:
       {
            GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUI_RECT_T rect = {0};
            GUI_RECT_T rect_list = {0};
		
            GUIAPICTRL_GetDisplayRect(MMK_GetCtrlId(list_ctrl_id), &rect_list);
            GUIAPICTRL_GetDisplayRect(MMK_GetCtrlId(btn_ctrl_id), &rect);
            rect.top = rect.bottom;
            rect.bottom = rect_list.top;
            //string bitmap has overlaid this area, redraw this rect 
            GUI_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);
		
            //string bitmap has overlaid this button, redraw it 
            CTRLBUTTON_Update(btn_ctrl_id);
        }		
        break;
		
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(MMK_GetActiveCtrlId(win_id) == btn_ctrl_id)
        {
            uint16 id = FindAlarmUnuseId();

            if(ALM_ALARM_NUM > id)
            {
                s_current_event_info.id = id;
                s_current_event_info.type = SMART_EVENT_ALARM;

                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
                {
                    s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
                    s_current_event_info.event.event_content_info.ring = 1;
                }

                s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
                s_current_event_info.event.event_content_info.is_add = TRUE;
                MMK_CreateWin((uint32*)ALARM_CUSTOM_EDITWIN_TAB, PNULL);
            }
            else
            {
                GUI_POINT_T start_point = {0, 0};
                MMICOM_BANNER_INFO_T banner_info = {0};

                banner_info.banner_type = MMI_BANNER_TEXT;
                banner_info.text1_id = STR_ALARM_NOTE_MAX_NUM_EXT02;
                banner_info.start_point = start_point;
                if (FALSE == MMIAPICOM_OpenBannerWin(win_id,banner_info))
                {
                    MMK_SetAtvCtrl(win_id, list_ctrl_id);
                }
            }
        }
        else if (MMK_GetActiveCtrlId(win_id) == list_ctrl_id
                && GUILIST_GetTotalItemNum(list_ctrl_id) > 0)
        {
            uint16 curItem = GUILIST_GetCurItemIndex(list_ctrl_id);
            uint16 id = GetAlarmRealId(curItem);

            if(ALM_ALARM_NUM > id)
            {
                s_current_event_info.id = id;
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                s_current_event_info.type = SMART_EVENT_ALARM;
                s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
                // Go to delete.
                if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (list_ctrl_id, curItem), GUIITEM_STATE_LEFT_FOCUS))
                {
                    DeleteOneAlarm(&s_current_event_info,list_ctrl_id,curItem);
                    MMIALM_RemoveIdlePushMsg(s_current_event_info.id, SMART_EVENT_ALARM);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                    break;
                }

                event_delay_ptr = GetDelayEvent(s_current_event_info.id);


                if (s_current_event_info.event.event_fast_info.is_on || event_delay_ptr->is_on)
                {
                    s_current_event_info.event.event_fast_info.is_on = FALSE;
#ifdef CALENDAR_SUPPORT
                    MMIAPISCH_DeleteUnreadEvent(s_current_event_info.id);
#endif
                    MMIALM_RemoveIdlePushMsg(s_current_event_info.id, SMART_EVENT_ALARM);
                }
                else
                {
                    s_current_event_info.event.event_fast_info.is_on = TRUE;
                }
                // �ر���ʱ����
                SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
                // �޸���������
                MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:        // goes to setting menu of alarm
        if(MMK_GetActiveCtrlId(win_id) == list_ctrl_id
                && GUILIST_GetTotalItemNum(list_ctrl_id) > 0)
        {
            MMK_CreateWin((uint32 *)MMIALM_MAIN_OPTWIN_TAB, PNULL);
        }
        break;

    case MSG_CLOSE_WINDOW:
// #if !defined MMI_GUI_STYLE_TYPICAL
//         //StopClockTimer();
// #endif
        break;

//���clock�н�ʣalarm,����Ӧtab�л�
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYREPEAT_LEFT:
        if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
        {
            GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID,  GUITAB_SWITCH_PRE);
        }
        break;

    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
        {
            GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID, GUITAB_SWITCH_NEXT);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(MMK_GetActiveCtrlId(win_id) == list_ctrl_id)
        {
            uint16 curItem = GUILIST_GetCurItemIndex(list_ctrl_id);
            if (GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (list_ctrl_id, curItem), GUIITEM_STATE_LEFT_FOCUS))
            {
                MMK_SendMsg(win_id, MSG_APP_RIGHT, PNULL);
                break;
            }
        }
        if(!MMK_IsChildWin(win_id))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMK_CloseParentWin(MMI_TIMER_PARENT_WIN_ID);
        }
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        SetMainWinSoftkeyByActiveCtrl(win_id,MMK_GetActiveCtrlId(win_id),TRUE);
        break;

    case MSG_CTRL_TOGGLE_CHANGED:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

LOCAL void AppendNewAlarmItem_Fg(MMI_CTRL_ID_T ctrl_id)
{
    GUI_BG_T        img_info          = {0};
    GUI_FONT_ALL_T  str_font          = {0};

    img_info.bg_type          = GUI_BG_IMG;
    img_info.img_id           = IMAGE_COMMON_PLUS_SMALL;
    str_font.font             = MMI_DEFAULT_BIG_FONT;
    str_font.color            = MMI_DEFAULT_TEXT_COLOR;

    GUIBUTTON_SetStyle(ctrl_id,GUIBUTTON_STYLE_LIRTM);
    //CTRLBUTTON_SetButtonIcon(ctrl_id,);
    GUIBUTTON_SetFg(ctrl_id,&img_info);
    GUIBUTTON_SetTextId(ctrl_id,STR_ALARM_NEW_ALARM_EXT01);
    GUIBUTTON_SetFont(ctrl_id,&str_font);
}

LOCAL void AppendNewAlarmItem(MMI_CTRL_ID_T ctrl_id)
{
    GUI_FONT_ALL_T  str_font          = {0};

    str_font.font             = MMI_DEFAULT_BIG_FONT;
    str_font.color            = MMI_DEFAULT_TEXT_COLOR;

    CTRLBUTTON_SetLocation(ctrl_id, 0, 0);
    CTRLBUTTON_SetIsRectButton(ctrl_id, TRUE);
    GUIBUTTON_SetTextId(ctrl_id,STR_ALARM_NEW_ALARM_EXT01);
    GUIBUTTON_SetFont(ctrl_id,&str_font);
    CTRLBUTTON_SetButtonIcon(ctrl_id, IMAGE_COMMON_PLUS_SMALL);
}

LOCAL void testTimeString(MMI_STRING_T *p_time_str)
{
    SCI_DATE_T sys_data = {0};
    MMI_TM_T tm = {0};
    uint32     cl_sec = 0;
    uint32     sys_sec = 0;
    uint32     duration_time = 0;

    TM_GetSysDate(&sys_data);
    tm.tm_mday = 1;
    tm.tm_mon = 8;
    tm.tm_year = 2015;

    cl_sec = MMIAPICOM_Tm2Second(0,0,0,tm.tm_mday,tm.tm_mon,tm.tm_year);
    sys_sec = MMIAPICOM_GetCurTime();
    /*duration_time = MMIAPICL_GetSimCallDurTimeByPeriod(MN_DUAL_SYS_1,MMICL_CALL_RECEIVED,
            cl_sec,sys_sec,p_time_str);*/
}

LOCAL uint8 GetAlarmTotalNum()
{
    uint8 i = 0, num = 0;

    for ( i = 0; i < ALM_ALARM_NUM; i++ )
    {
        MMIACC_SMART_EVENT_T smart_event = {0};
        uint8 event_id = EVENT_ALARM_0 + i;

        MMIAPIALM_FormInteEventInfo(event_id, &smart_event);
        if(smart_event.event_content_info.is_add)
        {
            num++;
        }
    }
    return num;
}

LOCAL void ShowAlarmListOrEmpty(BOOLEAN is_list_has_item)
{
    GUIFORM_SetChildDisplay(MMIALM_MAINWIN_FORM_CTRL_ID,
                            MMIALM_LISTALL_CTRL_ID,
                            is_list_has_item ? GUIFORM_CHILD_DISP_NORMAL : GUIFORM_CHILD_DISP_HIDE);
    GUIFORM_SetChildDisplay(MMIALM_MAINWIN_FORM_CTRL_ID,
                            MMIALM_TEXT_NO_ALARMS_CTRL_ID,
                            is_list_has_item ? GUIFORM_CHILD_DISP_HIDE : GUIFORM_CHILD_DISP_NORMAL);
}

LOCAL void AppendAlarmListItem(MMI_CTRL_ID_T list_ctrl_id)
{
    int i = 0;
    uint8 sel_item_index = 0;
    uint16 item_count = 0;
    GUILIST_ITEM_T          item_t       = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    MMIACC_SMART_EVENT_T    smart_event = {0};
    MMI_STRING_T repeat_content_str = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_STRING_T name_str = {0};
    MMI_STRING_T connect_str = {0};
    wchar connect[] = {L" | "};
    MMI_STRING_T time_str = {0};
    MMI_STRING_T txt_temp = {0};
    MMI_STRING_T txt_temp1 = {0};

    sel_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    GUILIST_SetMaxItem(list_ctrl_id, ALM_ALARM_NUM, FALSE );
    GUILIST_PermitBorder(list_ctrl_id, FALSE);

    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    item_t.item_style = GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT_MS;
    item_t.left_item_style = GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT_WITH_LEFT_FOCUS_MS;

    item_data.softkey_id[LEFT_BUTTON] = STXT_OPTION;
    item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
    item_data.lfocus_softkey_id[LEFT_BUTTON] = STXT_OPTION;
    item_data.lfocus_softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
    item_data.lfocus_softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_DELETE_MK;

    for (i = 0; i< ALM_ALARM_NUM; i++)
    {
        uint16 event_id = (uint16)( EVENT_ALARM_0 + i );
        const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = GetDelayEvent( event_id );
        uint8 index = 0;
        BOOLEAN is_on = FALSE;
        uint8 alpha = 255 * 90 / 100;

        if(PNULL == event_delay_ptr)
        {
            continue;
        }

        MMIAPIALM_FormInteEventInfo(event_id,&smart_event);

        if(!smart_event.event_content_info.is_add && event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
        {
            continue;//ͨ���������ж��Ƿ����û��½���
        }
        is_on = smart_event.event_fast_info.is_on | event_delay_ptr->is_on;

        if (event_id <= EVENT_ALARM_MAX)
        {
            name_str.wstr_ptr = smart_event.event_content_info.w_content;
            name_str.wstr_len = smart_event.event_content_info.w_length;

            MMIAPIALARM_GetEventTimeString( &smart_event.event_fast_info, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
            time_str.wstr_ptr = temp_wstr;
            time_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);

            connect_str.wstr_ptr = connect;
            connect_str.wstr_len = MMIAPICOM_Wstrlen(connect);

            MMIAPICOM_CatTwoString(&txt_temp,&time_str,&connect_str);
            MMIAPICOM_CatTwoString(&txt_temp1,&txt_temp,&name_str);
            MMIAPICOM_DestroyString(txt_temp);

            item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[index].item_data.text_buffer.wstr_ptr = txt_temp1.wstr_ptr;
            item_data.item_content[index].item_data.text_buffer.wstr_len = txt_temp1.wstr_len;
            item_data.item_content[index].is_opacity = FALSE; //is_on ? FALSE : TRUE;
            if(!is_on)
            {
                GUILIST_SetItemContentOpacityFontColor(&item_data,index,alpha,alpha);
            }
        }

        index++;

        SCI_MEMSET(&repeat_content_str,0,sizeof(MMI_STRING_T));
        GetRepeatModeString(&repeat_content_str,smart_event.event_fast_info.fre_mode,smart_event.event_fast_info.fre);
        item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[index].item_data.text_buffer.wstr_ptr =
                repeat_content_str.wstr_ptr;
        item_data.item_content[index].item_data.text_buffer.wstr_len =
                repeat_content_str.wstr_len;
        item_data.item_content[index].is_custom_font_color = TRUE;
        item_data.item_content[index].custom_font_color_focus = MMI_DARK_GRAY_COLOR;
		item_data.item_content[index].custom_font_color = MMI_DARK_GRAY_COLOR;


        /*if (!smart_event.event_fast_info.is_on )
        {
            item_data.softkey_id[MIDDLE_BUTTON] = TXT_COMMON_ON;
        }
        else if (MMIACC_ALARM_NUM > event_id)
        {
            if (event_delay_ptr->is_on)
            {
                item_data.softkey_id[MIDDLE_BUTTON] = TXT_COMMON_OFF;
            }
        }*/
        item_data.item_content[index].is_opacity = TRUE;
        GUILIST_SetItemContentOpacityFontColor(&item_data,index,alpha,alpha);
        item_data.softkey_id[MIDDLE_BUTTON] = is_on ? STXT_SOFTKEY_TURN_OFF_MK : STXT_SOFTKEY_TURN_ON_MK;

        GUILIST_AppendItem( list_ctrl_id, &item_t );
        GUILIST_SetToggleState(list_ctrl_id, item_count, is_on);
        item_count++;

        if(isEveryWeekNeedDestroyStr(smart_event.event_fast_info.fre_mode,
                smart_event.event_fast_info.fre))
        {
            MMIAPICOM_DestroyString(repeat_content_str);
        }
        MMIAPICOM_DestroyString(txt_temp1);
    }

    GUILIST_SetCurItemIndex(list_ctrl_id, sel_item_index);
    //GUILIST_SetShowLRTriangleIcon(list_ctrl_id, TRUE, FALSE);
    GUILIST_SetQuickActionIcon(list_ctrl_id, IMAGE_SHORTCUT_DELETE,
            IMAGE_NULL);
}

LOCAL void SetAlarmEditWinTitle()
{
    MMI_STRING_T txt_hdr_new_alarm = {0};
    wchar *cur_evt_title_ptr = s_current_event_info.event.event_content_info.w_content;
    uint16 cur_evt_title_len = s_current_event_info.event.event_content_info.w_length;

    MMI_GetLabelTextByLang(STR_ALARM_HDR_NEW_EXT01,&txt_hdr_new_alarm);
    if(!MMIAPICOM_CompareTwoWstr(txt_hdr_new_alarm.wstr_ptr,txt_hdr_new_alarm.wstr_len,
            cur_evt_title_ptr,cur_evt_title_len))
    {
        MMIAPICOM_StatusAreaSetViewHeaderinfo(txt_hdr_new_alarm.wstr_ptr,txt_hdr_new_alarm.wstr_len);
    }
    else
    {
        MMIAPICOM_StatusAreaSetViewHeaderinfo(cur_evt_title_ptr,cur_evt_title_len);
    }
}

LOCAL void SetAlarmListData(
                        MMI_WIN_ID_T      win_id,
                        MMI_CTRL_ID_T     list_ctrl_id,
                        MMI_CTRL_ID_T     btn_ctrl_id
                        )
{
    GUI_RECT_T list_rect = {0};
    uint8 alpha = 255 * 35 / 100;

    MMI_CTRL_ID_T empty_text_id = MMIALM_TEXT_NO_ALARMS_CTRL_ID;
    MMI_STRING_T  title_no_alarms     = {0};
    GUI_FONT_T empty_font = MMI_DEFAULT_BIG_FONT;
#ifdef SCHEDULE_SUPPORT
    GUI_COLOR_T empty_color = MMIAPISCH_GetAlphaedColor(alpha, MMI_DEFAULT_TEXT_COLOR, MMI_BACKGROUND_COLOR);
#else
    GUI_COLOR_T empty_color = MMI_DEFAULT_TEXT_COLOR;
#endif
    BOOLEAN is_list_has_item = GetAlarmTotalNum() > 0;

    GUITEXT_SetFont(empty_text_id, &empty_font, &empty_color);
    GUITEXT_SetAlign(empty_text_id, /*ALIGN_LVMIDDLE*/ALIGN_HMIDDLE);
    MMI_GetLabelTextByLang(STR_ALARM_EMPTY_EXT01, &title_no_alarms);
    GUITEXT_SetString(empty_text_id, title_no_alarms.wstr_ptr, title_no_alarms.wstr_len, FALSE);

    GUILIST_SetHandleLeftRightEx(list_ctrl_id, TRUE, FALSE);
    AppendAlarmListItem(list_ctrl_id);

    ShowAlarmListOrEmpty(is_list_has_item);
    SetMainWinSoftkeyByActiveCtrl(win_id,MMK_GetActiveCtrlId(win_id),FALSE);
    MMK_SetAtvCtrl( win_id, is_list_has_item ? list_ctrl_id : btn_ctrl_id);
}

LOCAL MMI_RESULT_E  HandleCustomAlarmEditWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id =MMIALM_EDIT_MODE_SETLIST_CTRL_ID;
    MMI_CTRL_ID_T           form_ctrl_id = MMI_EDITWIN_FORM_CTRL_ID;
    GUI_BG_T        bg_info = {0};

    uint16                  img_width  = 0;
    uint16                  img_height = 0;
    GUIFORM_CHILD_WIDTH_T   button_width = {0};
    GUIFORM_CHILD_HEIGHT_T  button_height = {0};
#ifdef MMI_RECORD_SUPPORT
    GUI_BG_T bg={0};
#endif
    BOOLEAN is_need_save = TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetCircularHandleUpDown(form_ctrl_id,TRUE);
        GUIFORM_SetType(form_ctrl_id,GUIFORM_TYPE_TP);

        GUIFORM_SetStyle(MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIAPICTRL_SetState( MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, GUICTRL_STATE_DISABLE_TP,TRUE );
        //NEWMS00222350
        GUIEDIT_SetHandleRedKey(FALSE, MMIALM_EDIT_NAME_EDITBOX_CTRL_ID);
        GUIFORM_PermitChildFont(form_ctrl_id,FALSE);

#ifdef RECORD_SUPPORT
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_SCH_RECORD_ICON, win_id);
        button_width.add_data = img_width;
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        button_height.add_data = img_height;
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        GUIFORM_SetChildWidth(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_height);
        GUIBUTTON_SetCallBackFunc(MMIALM_EDIT_RING_BUTTON_CTRL_ID, RecordAlarmRingCallBack);
        GUIBUTTON_SetRunSheen(MMIALM_EDIT_RING_BUTTON_CTRL_ID,FALSE);
        bg.bg_type=GUI_BG_IMG;
        bg.img_id=IMG_SCH_RECORD_ICON;
        GUIBUTTON_SetPressedBg(MMIALM_EDIT_RING_BUTTON_CTRL_ID,&bg);
#endif

        //MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_ALARM_HDR_NEW_EXT01);
        SetAlarmEditWinTitle();

        SetCustomEditNameWin(win_id);
        SetCustomEditTimeWin();
        GUIEDIT_SetAlign(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);

        SetWeeksParamEditWin(win_id);
        SetWeeksDisplayEditWin();

        if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF!= s_current_event_info.id)
        {
            #ifndef MMI_PDA_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
            #else
            MMK_SetAtvCtrl(win_id, form_ctrl_id);
            #endif
        }
        else
        {
            #ifndef MMI_PDA_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
            #else
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TIME_FORM_CTRL_ID);
            #endif
            GUIFORM_SetChildDisplay(form_ctrl_id,MMIALM_EDIT_NAME_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
            GUIFORM_SetChildDisplay(form_ctrl_id,MMIALM_EDIT_RING_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
        }
        break;

    case MSG_FULL_PAINT:
        SetEditRepeatWin();
        SetEditRingParamWin();
        SetSnoozeTimeParamEditWin();
        break;
    case MSG_GET_FOCUS:
        //MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_ALARM_HDR_NEW_EXT01);
        SetAlarmEditWinTitle();
        break;

    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
        //EditWinMIDSKProcess(win_id);
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        HandleAlarmEditMskMsg(win_id, ctrl_id);
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
         ctrl_id = MMK_GetActiveCtrlId(win_id);
         HandleAlarmEditFormSwitchActiveMsg(win_id,ctrl_id);
         break;

#ifdef MMI_RECORD_SUPPORT
    case MMI_MSG_RECORDTORING:
        if(PNULL != param)
        {
            MMI_STRING_T        ring_name = {0};

            ring_name.wstr_ptr = (wchar*)param;
            ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
            SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr,0,(MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
            s_current_event_info.event.event_ring_info.dring_name_len = ring_name.wstr_len;
            MMIAPICOM_Wstrncpy(s_current_event_info.event.event_ring_info.dring_name_arr, ring_name.wstr_ptr, ring_name.wstr_len);
            SCI_MEMSET(ring_name.wstr_ptr,0,ring_name.wstr_len * sizeof(wchar));

            MMIAPIFMM_SplitFullPath(s_current_event_info.event.event_ring_info.dring_name_arr, s_current_event_info.event.event_ring_info.dring_name_len, PNULL, PNULL, PNULL, PNULL, ring_name.wstr_ptr, &ring_name.wstr_len);

            s_current_event_info.event.event_content_info.ring_type = ALM_RECORD_RING;
        }
        DisplayTheRingName();
        MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
        break;
#endif

    case MSG_CTL_OK:
    case MSG_APP_OK:
        //CR177677:�ж���һ���Ƿ���������������ʱ���Ƿ�һ�£���һ�£�������ʾ��
        MMK_CreateWin((uint32 *)ALARM_EDIT_MENU_WIN_TAB, PNULL);
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //MMK_CloseWin(MMIAUTOPOWER_OPTWIN_ID);
        break;

    case MSG_CTL_SETLIST_SWITCH:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        if(MMIALM_EDIT_MODE_SETLIST_CTRL_ID==ctrl_id)
        {
            SetWeeksDisplayEditWin();
        }
        break;

    case MSG_NOTIFY_CONTENT_OVERFLOW:
       GUIWIN_UpdateStb();
       break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

PUBLIC GUI_BOTH_RECT_T MMIAPITEST_GetTextEditRect(MMI_WIN_ID_T win_id)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    both_rect.v_rect.left = 6;
    both_rect.v_rect.top = 50;
    both_rect.v_rect.bottom = 300;
    both_rect.v_rect.right = both_rect.v_rect.right - 6;

    return both_rect;
}

LOCAL void SetEditCtrlSoftkey(
                            MMI_CTRL_ID_T ctrl_id,
                            uint16        left_btn_str_len,
                            uint16        mid_btn_str_len,
                            uint16        right_btn_str_len,
                            MMI_TEXT_ID_T mid_btn_txt_id)
{
    GUIEDIT_SetSoftkeyEx(ctrl_id,LEFT_BUTTON,left_btn_str_len,STXT_OPTION,STXT_OPTION,
            IMAGE_NULL,IMAGE_NULL,EDIT_BUTTON_TEXT_ID,PNULL);
    GUIEDIT_SetSoftkeyEx(ctrl_id,MIDDLE_BUTTON,mid_btn_str_len,mid_btn_txt_id,mid_btn_txt_id,
            IMAGE_NULL,IMAGE_NULL,EDIT_BUTTON_TEXT_ID,PNULL);
    GUIEDIT_SetSoftkeyEx(ctrl_id,RIGHT_BUTTON,right_btn_str_len,TXT_COMMON_COMMON_BACK,STR_LIST_DELETE,
            IMAGE_NULL,IMAGE_NULL,
            EDIT_BUTTON_TEXT_ID,PNULL);
}

LOCAL void SetCustomEditNameWin_old(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T  name_str        = {0};
    MMI_STRING_T  name_str1      = {0};
    MMI_STRING_T  text                = {0};
    MMI_TEXT_ID_T txt_alarm_id[] = {TXT_ALARM_CLOCK1,TXT_ALARM_CLOCK2,TXT_ALARM_CLOCK3,
                                                TXT_ALARM_CLOCK4,TXT_ALARM_CLOCK5,TXT_ALARM_CLOCK6,
                                                TXT_ALARM_CLOCK7,TXT_ALARM_CLOCK8,TXT_ALARM_CLOCK9,TXT_ALARM_CLOCK10};
    MMI_TEXT_ID_T txt_alarm_id_1[] = {TXT_ALARM_DELAY_1,TXT_ALARM_DELAY_2,TXT_ALARM_DELAY_3,
                                        TXT_ALARM_DELAY_4,TXT_ALARM_DELAY_5,TXT_ALARM_DELAY_6,
                                        TXT_ALARM_DELAY_7,TXT_ALARM_DELAY_8,TXT_ALARM_DELAY_9,TXT_ALARM_DELAY_10};

    GUI_BOTH_RECT_T edit_both_rect = {0};
    GUI_BOTH_RECT_T display_rect = {0};
    MMI_CTRL_ID_T   edit_name_ctrl_id = MMIALM_EDIT_NAME_EDITBOX_CTRL_ID;

    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        //GUIFORM_SetStyle(MMIALM_EDIT_NAME_FORM_CTRL_ID,GUIEDIT_STYLE_MULTI_DYN_UP);
        GUIEDIT_SetStyle(edit_name_ctrl_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);

        MMI_GetLabelTextByLang(STR_LABEL_ALARM_NAME_EXT01, &text);
        GUILABEL_SetText(MMIALM_EDIT_NAME_LABEL_CTRL_ID, &text, FALSE);

        MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str1);
        if (!MMIAPICOM_CompareTwoWstrExt(name_str1.wstr_ptr,
                                            name_str1.wstr_len,
                                            s_current_event_info.event.event_content_info.w_content,
                                            s_current_event_info.event.event_content_info.w_length,
                                            TRUE)
            ||(0 == s_current_event_info.event.event_content_info.w_length) )
        {
            if (s_current_event_info.id <= EVENT_ALARM_MAX) //&&s_current_event_info.id>=EVENT_ALARM_0 /*lint !e685 !e568*/
            {
                MMI_GetLabelTextByLang(txt_alarm_id[s_current_event_info.id],&name_str);
            }
            else if(s_current_event_info.id>=EVENT_ALARM_DELAY_0&&s_current_event_info.id <= EVENT_ALARM_DELAY_MAX)
            {
                MMI_GetLabelTextByLang(txt_alarm_id_1[s_current_event_info.id-EVENT_ALARM_DELAY_0],&name_str);
            }
        }
        else
        {
            name_str.wstr_ptr = s_current_event_info.event.event_content_info.w_content;
            name_str.wstr_len = s_current_event_info.event.event_content_info.w_length;
        }
#ifdef MMI_PDA_SUPPORT
        GUILABEL_SetText(MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID, &name_str,FALSE);
#else
        GUIEDIT_SetString(edit_name_ctrl_id, name_str.wstr_ptr, name_str.wstr_len);
#endif
        edit_both_rect = MMIAPITEST_GetTextEditRect(win_id);
        edit_both_rect.v_rect.bottom = 250;
        edit_both_rect.v_rect.top = edit_both_rect.v_rect.bottom - 28;

        GUIAPICTRL_SetBothRect(edit_name_ctrl_id, &edit_both_rect);
    }
}

LOCAL void SetCustomEditNameWin(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T  name_str        = {0};

    GUI_BOTH_RECT_T edit_both_rect = {0};
    GUI_BOTH_RECT_T display_rect = {0};
    MMI_CTRL_ID_T   edit_name_ctrl_id = MMIALM_EDIT_NAME_EDITBOX_CTRL_ID;
    MMI_TEXT_ID_T txt_default_alarm_id = STR_DEFAULT_ALARM_NAME;

    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        GUIEDIT_SetStyle(edit_name_ctrl_id,GUIEDIT_STYLE_MULTI_DYN_DOWN);

        if(s_current_event_info.event.event_content_info.w_length == 0)
        {
            MMI_GetLabelTextByLang(txt_default_alarm_id,&name_str);
        }
        else
        {
            name_str.wstr_ptr = s_current_event_info.event.event_content_info.w_content;
            name_str.wstr_len = s_current_event_info.event.event_content_info.w_length;
        }
#ifdef MMI_PDA_SUPPORT
        GUILABEL_SetText(MMIALM_EDIT_NAME_EDIT_LABEL_CTRL_ID, &name_str,FALSE);
#else
        GUIEDIT_SetString(edit_name_ctrl_id, name_str.wstr_ptr, name_str.wstr_len);
        GUIEDIT_SetDefaultStringById(edit_name_ctrl_id,STR_INPUT_ALARM_NAME_EXT01);
#endif
        edit_both_rect = MMIAPITEST_GetTextEditRect(win_id);
        edit_both_rect.v_rect.bottom = 250;
        edit_both_rect.v_rect.top = edit_both_rect.v_rect.bottom - 28;

        //GUIAPICTRL_SetBothRect(edit_name_ctrl_id, &edit_both_rect);

        CTRLBASEEDIT_SetLabelId(win_id,edit_name_ctrl_id,STR_LABEL_ALARM_NAME_EXT01,FALSE);
        SetEditCtrlSoftkey(edit_name_ctrl_id,0,0,1,STXT_SOFTKEY_SAVE_MK);
    }
}

LOCAL BOOLEAN  SaveCustomNameEditWin(void)
{
    MMI_STRING_T        str        = {0};
    MMIACC_SMART_EVENT_CONTENT_T*event_content_ptr=PNULL;
    MMI_TEXT_ID_T txt_default_alarm_id = STR_DEFAULT_ALARM_NAME;

    event_content_ptr = &s_current_event_info.event.event_content_info;
    GUIEDIT_GetString(MMIALM_EDIT_NAME_EDITBOX_CTRL_ID, &str);
    if (MMIAPISET_IsIncludeReturnChar(str.wstr_ptr,str.wstr_len))
    {
        MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_TITLE);
        return FALSE;
    }

    if(0 == str.wstr_len)
    {
        MMI_GetLabelTextByLang(txt_default_alarm_id,&str);
    }

    event_content_ptr->w_length = str.wstr_len;
    MMI_WSTRNCPY(
            event_content_ptr->w_content, MMIACC_EVENT_TEXT_MAX_LEN,
            str.wstr_ptr, str.wstr_len,
            str.wstr_len);

    return TRUE ;
}

LOCAL BOOLEAN isEveryWeekNeedDestroyStr(
                                        MMIACC_MODE_TYPE_E fre_mode,
                                        uint8 fre)
{
    BOOLEAN is_weekday_or_weekend = FALSE;

    is_weekday_or_weekend = (fre == MASK_WEEKDAY || fre == MASK_WEEKEND);
    if(fre_mode == ALM_MODE_EVERYWEEK && !is_weekday_or_weekend)
    {
        return TRUE;
    }
    return FALSE;
}

LOCAL void SetEditRepeatWin()
{
    MMI_CTRL_ID_T list_ctrl_id = MMIALM_EDIT_REPEAT_LIST_CTRL_ID;

    GUILIST_ITEM_T          item_t       = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};

    MMI_STRING_T  repeat_content_str        = {0};

    GUI_COLOR_T font_color = MMI_DEFAULT_TEXT_COLOR;
    uint8 alpha = 255 * 60 / 100;

    GUILIST_SetMaxItem(list_ctrl_id, 1, FALSE );

    item_t.item_data_ptr = &item_data;


    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS_FOR_ALARM;//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS//change by James li
    item_data.item_content[ALM_EDIT_LIST_ITEM_LABEL_INDEX].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[ALM_EDIT_LIST_ITEM_LABEL_INDEX].item_data.text_id = STR_LABEL_REPEAT_EXT01;
    GUILIST_SetItemState(&item_t,GUIITEM_STATE_COSTMER_FONT_COLOR,TRUE);
    GUILIST_SetItemContentCustomFontColor(list_ctrl_id,ALM_EDIT_LIST_ITEM_LABEL_INDEX,
            &font_color,alpha,&font_color,alpha);

    GetRepeatModeString(&repeat_content_str,s_current_event_info.event.event_fast_info.fre_mode,
            s_current_event_info.event.event_fast_info.fre);
    item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data.text_buffer.wstr_len = repeat_content_str.wstr_len;
    item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data.text_buffer.wstr_ptr = repeat_content_str.wstr_ptr;
	//item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].is_custom_font_color = TRUE;
    //item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].custom_font_color_focus = MMI_GRAY_WHITE_COLOR;

    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    GUILIST_AppendItem( list_ctrl_id, &item_t );

    if(isEveryWeekNeedDestroyStr(s_current_event_info.event.event_fast_info.fre_mode,
            s_current_event_info.event.event_fast_info.fre))
    {
        MMIAPICOM_DestroyString(repeat_content_str);
    }
}

LOCAL void SetCustomEditTimeWin()
{
    GUIEDIT_TIME_STYLE_E time_style =GUIEDIT_TIME_STYLE_24;
    MMISET_TIME_DISPLAY_TYPE_E time_type=MMISET_TIME_24HOURS;
    MMI_STRING_T        title_text            = {0};
    MMI_CTRL_ID_T   edit_time_ctrl_id = MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID;
    GUIFORM_CHILD_HEIGHT_T timeeditor_height = {ALARM_EDIT_ITEM_HEIGHT, GUIFORM_CHILD_HEIGHT_FIXED};

    GUIFORM_SetStyle(MMIALM_EDIT_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(STR_LABEL_SET_TIME_EXT01, &title_text);
    GUILABEL_SetText(MMIALM_EDIT_TITLE_LABEL_CTRL_ID, &title_text, FALSE);
    GUILABEL_SetFont(MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMI_DEFAULT_BIG_FONT,MMI_DARK_GRAY_COLOR);
    GUILABEL_SetMargin(MMIALM_EDIT_TITLE_LABEL_CTRL_ID, 6);

    GUIFORM_SetChildHeight(MMI_EDITWIN_FORM_CTRL_ID,edit_time_ctrl_id,&timeeditor_height);
    CTRLTIMEEDIT_SetIsFullTimeEditorFlag(edit_time_ctrl_id,FALSE);

    time_type = MMIAPISET_GetTimeDisplayType();
    if (MMISET_TIME_12HOURS == time_type)
    {
        time_style = GUIEDIT_TIME_STYLE_12;
    }
    else
    {
        time_style = GUIEDIT_TIME_STYLE_24;
    }

    //set time display style
    GUIEDIT_SetTimeStyle(edit_time_ctrl_id,PNULL,&time_style,PNULL,FALSE);
    //GUIEDIT_SetFont(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, MMI_SET_TIME_FONT);

    if (s_current_event_info.event.event_fast_info.is_valid )
    {
        GUIEDIT_SetTime(edit_time_ctrl_id,
                            s_current_event_info.event.event_fast_info.hour,
                            s_current_event_info.event.event_fast_info.minute,
                            0);
    }
    else
    {
        SCI_TIME_T      sys_time = {0};
        TM_GetSysTime(&sys_time);
        GUIEDIT_SetTime(edit_time_ctrl_id,
                            sys_time.hour,
                            sys_time.min,
                            0);
    }
}

LOCAL void  HandleAlarmEditMskMsg(
                                MMI_HANDLE_T    win_id,
                                uint32 ctrl_id
                                )
{
    MMI_STRING_T name_id_str = {0};

    switch(ctrl_id)
    {
        case MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID:
        case MMIALM_EDIT_NAME_EDITBOX_CTRL_ID:
            SaveEvent(win_id);
            break;
        /*case MMIALM_EDIT_NAME_EDITBOX_CTRL_ID:
            MMI_GetLabelTextByLang(STR_LABEL_ALARM_NAME_EXT01,&name_id_str);
            MMIPUB_OpenDialogInputFieldWinByTextPtr(&name_id_str,PNULL,PNULL,
                    MMIPUB_SOFTKEY_DIALOG_CLOSE_OK_RETURN,HandleAlarmEditNameWinMsg);
            break;*/
        case MMIALM_EDIT_REPEAT_LIST_CTRL_ID:
            MMK_CreateWin((uint32 *)ALARM_REPEATLISTWIN_TAB, PNULL);
            break;
        case MMIALM_EDIT_RING_LIST_CTRL_ID:
            MMK_CreateWin((uint32 *)ALARM_RINGTONE_SEL_WIN_TAB, PNULL);
            break;
        case MMIALM_EDIT_SNOOZE_TIME_CTRL_ID:
            MMK_CreateWin((uint32 *)ALARM_SNOOZETIME_SEL_WIN_TAB,PNULL);
            break;
        case MMI_EDITWIN_BUTTON0_CTRL_ID:
        case MMI_EDITWIN_BUTTON1_CTRL_ID:
        case MMI_EDITWIN_BUTTON2_CTRL_ID:
        case MMI_EDITWIN_BUTTON3_CTRL_ID:
        case MMI_EDITWIN_BUTTON4_CTRL_ID:
        case MMI_EDITWIN_BUTTON5_CTRL_ID:
        case MMI_EDITWIN_BUTTON6_CTRL_ID:
            if(s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID])
            {
                s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=FALSE;
            }
            else
            {
                s_weeks_button_is_selected[ctrl_id-MMI_EDITWIN_BUTTON0_CTRL_ID]=TRUE;
            }
#ifndef MMI_PDA_SUPPORT
            setAlarmEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
            SetEditWinButtonBg(win_id,ctrl_id);
            break;
        default:
            break;
    }
}

LOCAL  MMI_RESULT_E AlarmRepeatListWinHookFunc(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    if (msg_id == MSG_APP_OK || msg_id == MSG_CTL_OK)
    {
         result = MMI_RESULT_TRUE;
    }

    return result;
}

LOCAL MMI_RESULT_E  HandleAlarmRepeatListWinMsg(
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  DPARAM            param
                                                  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    uint16              sel_items[ALM_REPEAT_LIST] = {0};
    uint16              sel_count = 0;
    MMI_CTRL_ID_T       current_ctrl_id = MMIALM_REPEAT_LIST_CTRL_ID;
    GUILIST_MARK_ITEM_T *list_param_ptr = PNULL;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:      //create check list box
            MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CALE_HDR_REPEAT_EXT02);
            AppendAlarmRepeatListBoxItem(current_ctrl_id);
            MMK_SetWinHookFunc( win_id, AlarmRepeatListWinHookFunc);
            // read info of alarm , set check flag to indicate if the alarm is setting
            //GUILIST_SetMaxSelectedItem(current_ctrl_id, ALM_MODE_LIST);
            MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;

    case MSG_CTL_LIST_MARK_ITEM:
        /*list_param_ptr = (GUILIST_MARK_ITEM_T*)param;
        if(list_param_ptr == PNULL)
        {
            break;
        }
        if(list_param_ptr->had_maked)
        {
            GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_UNMARK_MK,MIDDLE_BUTTON,TRUE);
        }
        else
        {
            GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_MARK_MK,MIDDLE_BUTTON,TRUE);
        }*/
        break;

    case MSG_NOTIFY_LIST_SELECT_QUERY:
        {
            GUILIST_QRERY_SELECT_ITEM_T * mark_item_ptr = (GUILIST_QRERY_SELECT_ITEM_T *)param;

            mark_item_ptr->is_check_select = TRUE;
        }
        break;

    case MSG_CTL_CANCEL:  //MSG_CTL_OK:
        sel_count = GUILIST_GetSelectedItemIndex(current_ctrl_id,sel_items, ALM_REPEAT_LIST);
        //  save current settings of ferquency to EFS
            //MMIPUB_OpenAlertWarningWin(TXT_MUST_SELECT_ONE);

        UpdateAlarmFreMode(sel_items,sel_count);
        //MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
        MMK_CloseWin(win_id);
        break;

    case MSG_GET_FOCUS:
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CALE_HDR_REPEAT_EXT02);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

LOCAL void AppendAlarmRepeatListBoxItem(
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    uint16      max_item = ALM_REPEAT_LIST;
    uint16      i = 0;
    MMI_TEXT_ID_T repeat_week_list_arr[ALM_MODE_WEEK_NUM] =
    {
        STXT_ALARM_CLOCK_REP_MON,
        STXT_ALARM_CLOCK_REP_TUE,
        STXT_ALARM_CLOCK_REP_WED,
        STXT_ALARM_CLOCK_REP_THU,
        STXT_ALARM_CLOCK_REP_FRI,
        STXT_ALARM_CLOCK_REP_SAT,
        STXT_ALARM_CLOCK_REP_SUN
    };
    GUILIST_ITEM_T      item_t      =   {0};
    GUILIST_ITEM_DATA_T item_data   =   {0};

    //set list max item and selected max item
    GUILIST_SetMaxItem(ctrl_id,max_item,FALSE);
    GUILIST_SetMaxSelectedItem(ctrl_id,max_item);
    GUILIST_SetCheckIconLeftFlag(ctrl_id,TRUE);
    GUILIST_SetListState(ctrl_id,GUILIST_STATE_OWN_SOFTKEY,TRUE);
    GUILIST_ChangeCheckListSoftkeyIndex(ctrl_id,MIDDLE_BUTTON);
    GUILIST_ChangeCheckListDefSoftkeyID(ctrl_id,STXT_SOFTKEY_CANCEL_MK	,STXT_SOFTKEY_MARK_MK,TXT_NULL);//STXT_SOFTKEY_UNMARK_MK//change by James li

    //append item
    //MMIAPISET_AppendListItemByTextId(s_repeat_week_list_arr[6],TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
    item_t.item_style    = GUIITEM_STYLE_MULTI_CHOICE_PICKER_TEXT;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
    for (i = 0; i < max_item; i++)
    {
        //AppendListItemByTextId(repeat_week_list_arr[i],ctrl_id,GUIITEM_STYLE_MULTI_CHOICE_PICKER_TEXT);
        BOOLEAN is_selected_item = FALSE;

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = repeat_week_list_arr[i];

        GUILIST_AppendItem( ctrl_id, &item_t );

        is_selected_item = (s_current_event_info.event.event_fast_info.fre &
                s_week_mask_frm_mon[i]) != 0;
        if (is_selected_item)
        {
            //set selected item
            GUILIST_SetSelectedItem(ctrl_id,i,TRUE);
        }
    }

    /*for (i = 0; i < max_item; i++)
    {
        if (s_current_event_info.event.event_fast_info.fre &
                s_week_mask_frm_mon[i])
        {
            //set selected item
            GUILIST_SetSelectedItem(ctrl_id,i,TRUE);
        }
    }*/
}

PUBLIC void AppendListItemByTextId(
                                    MMI_TEXT_ID_T        text_id,
                                    MMI_CTRL_ID_T        ctrl_id,
                                    GUIITEM_STYLE_E      item_style
                                    )
{
    GUILIST_ITEM_T      item_t      =   {0};
    GUILIST_ITEM_DATA_T item_data   =   {0};

    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;

    GUILIST_AppendItem( ctrl_id, &item_t );
}
//[end]

//[edit repeat list]
LOCAL void UpdateAlarmFreMode(uint16 *index_ptr,uint8 sel_count)
{
    UpdateAlarmMode(index_ptr,s_current_event_info.event.event_fast_info.fre,sel_count);

    if(sel_count == 0)
    {
        s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
    }
    else if(sel_count == ALM_MODE_WEEK_NUM)
    {
        s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
    }
    /*else if(s_current_event_info.event.event_fast_info.fre == MASK_WEEKDAY)
    {
        s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_WEEKDAY;
    }
    else if(s_current_event_info.event.event_fast_info.fre == MASK_WEEKEND)
    {
        s_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_WEEKEND;
    }*/
    else
    {
        s_current_event_info.event.event_fast_info.fre_mode =
                /*ALM_MODE_SPECIALDAY*/ALM_MODE_EVERYWEEK;
    }

    return;
}

LOCAL void GetRepeatModeString(
                            MMI_STRING_T *p_repeat_str,
                            MMIACC_MODE_TYPE_E fre_mode,
                            uint8 fre_mask
                            )
{
    MMI_STRING_T txt_specday_temp = {0};
    MMI_STRING_T txt_specday_temp1 = {0};
    MMI_STRING_T txt_comma_space = {0};
    MMI_STRING_T res_strid = {0};
    wchar comma_and_space[] = {',',' ',0};
    MMI_TEXT_ID_T edit_repeat_arr[ALM_MODE_WEEK_NUM] =
    {
        STR_ALARM_REPEAT_MO_EXT01,
        STR_ALARM_REPEAT_TU_EXT01,
        STR_ALARM_REPEAT_WE_EXT01,
        STR_ALARM_REPEAT_TH_EXT01,
        STR_ALARM_REPEAT_FR_EXT01,
        STR_ALARM_REPEAT_SA_EXT01,
        STR_ALARM_REPEAT_SU_EXT01
    };

    txt_comma_space.wstr_ptr = comma_and_space;
    txt_comma_space.wstr_len = MMIAPICOM_Wstrlen(comma_and_space);

    switch(fre_mode)
    {
        case ALM_MODE_ONCE:
            MMI_GetLabelTextByLang(STR_ILIST_NOT_REPEATED,p_repeat_str);
            break;
        case ALM_MODE_EVERYDAY:
            MMI_GetLabelTextByLang(STR_ALARM_LIST_RP_EVD_EXT02,p_repeat_str);
            break;
        case ALM_MODE_EVERYWEEK:
            if(fre_mask == MASK_WEEKDAY)
            {
                MMI_GetLabelTextByLang(STR_ALARM_LIST_RP_WKD_EXT02,p_repeat_str);
            }
            else if(fre_mask == MASK_WEEKEND)
            {
                MMI_GetLabelTextByLang(STR_LIST_REPEAT_WEEKENDS_EXT01,p_repeat_str);
            }
            else
            {
                int i = 0;
                BOOLEAN is_first_time = TRUE;
                for (i = 0; i < ALM_MODE_WEEK_NUM; i++)
                {
                    if (fre_mask & s_week_mask_frm_mon[i])
                    {
                        MMI_GetLabelTextByLang(edit_repeat_arr[i],&res_strid);
                        if(is_first_time)
                        {
                            MMI_STRING_T empty_str = {0};
                            MMIAPICOM_CatTwoString(&txt_specday_temp1,&empty_str,&res_strid);
                            is_first_time = FALSE;
                        }
                        else
                        {
                            MMIAPICOM_CatTwoString(&txt_specday_temp1,&txt_comma_space,&res_strid);
                        }
                        MMIAPICOM_CatTwoString(&txt_specday_temp,p_repeat_str,&txt_specday_temp1);
                        MMIAPICOM_DestroyString(*p_repeat_str);
                        MMIAPICOM_DestroyString(txt_specday_temp1);
                        p_repeat_str->wstr_ptr = SCI_ALLOCA((txt_specday_temp.wstr_len + 1) * sizeof(wchar));
                        p_repeat_str->wstr_len = txt_specday_temp.wstr_len;
                        SCI_MEMSET(p_repeat_str->wstr_ptr,0,(p_repeat_str->wstr_len + 1) * sizeof(wchar));
                        MMIAPICOM_Wstrcpy(p_repeat_str->wstr_ptr,txt_specday_temp.wstr_ptr);
                        MMIAPICOM_DestroyString(txt_specday_temp);
                    }
                }
            }
            break;
        /*case ALM_MODE_WEEKDAY:
            MMI_GetLabelTextByLang(STR_ALARM_LIST_RP_WKD_EXT02,p_repeat_str);
            break;
        case ALM_MODE_WEEKEND:
            MMI_GetLabelTextByLang(STR_LIST_REPEAT_WEEKENDS_EXT01,p_repeat_str);
            break;
        case ALM_MODE_SPECIALDAY:
            {
                int i = 0;
                BOOLEAN is_first_time = TRUE;
                for (i = 0; i < ALM_MODE_WEEK_NUM; i++)
                {
                    if (fre_mask & s_week_mask_frm_mon[i])
                    {
                        MMI_GetLabelTextByLang(edit_repeat_arr[i],&res_strid);
                        if(is_first_time)
                        {
                            MMI_STRING_T empty_str = {0};
                            MMIAPICOM_CatTwoString(&txt_specday_temp1,&empty_str,&res_strid);
                            is_first_time = FALSE;
                        }
                        else
                        {
                            MMIAPICOM_CatTwoString(&txt_specday_temp1,&txt_comma_space,&res_strid);
                        }
                        MMIAPICOM_CatTwoString(&txt_specday_temp,p_repeat_str,&txt_specday_temp1);
                        MMIAPICOM_DestroyString(*p_repeat_str);
                        MMIAPICOM_DestroyString(txt_specday_temp1);
                        p_repeat_str->wstr_ptr = SCI_ALLOCA((txt_specday_temp.wstr_len + 1) * sizeof(wchar));
                        p_repeat_str->wstr_len = txt_specday_temp.wstr_len;
                        SCI_MEMSET(p_repeat_str->wstr_ptr,0,(p_repeat_str->wstr_len + 1) * sizeof(wchar));
                        MMIAPICOM_Wstrcpy(p_repeat_str->wstr_ptr,txt_specday_temp.wstr_ptr);
                        MMIAPICOM_DestroyString(txt_specday_temp);
                    }
                }
            }
            break;*/
        default:
            break;
    }
}

LOCAL MMI_RESULT_E HandleAlarmEditNameWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin(win_id,
            MMIPUB_INPUTFIELD_CTRL_ID);
    MMI_CTRL_ID_T edit_name_ctrl_id = MMIALM_EDIT_NAME_EDITBOX_CTRL_ID;
    MMI_STRING_T edit_str = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_GetString(edit_name_ctrl_id,&edit_str);
        GUIEDIT_SetString(ctrl_handle,edit_str.wstr_ptr,edit_str.wstr_len);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_KEYUP_WEB:
        GUIEDIT_GetString(ctrl_handle,&edit_str);
        GUIEDIT_SetString(edit_name_ctrl_id,edit_str.wstr_ptr,edit_str.wstr_len);
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleDialogInputFieldWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

LOCAL void SetEditRingParamWin(void)
{
    MMI_STRING_T        ring            = {0};
    wchar              file_name[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16              fixed_ringid                          = 0;

    MMI_CTRL_ID_T list_ctrl_id = MMIALM_EDIT_RING_LIST_CTRL_ID;

    GUILIST_ITEM_T          item_t       = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};

    MMI_STRING_T  ring_content_str        = {0};
    uint16        file_name_len = 0;
    wchar         *ring_name_ptr          = PNULL;
    uint16        ring_name_len = 0;

    GUI_COLOR_T font_color = MMI_DEFAULT_TEXT_COLOR;
    uint8 alpha = 255 * 60 / 100;

    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        #ifdef DRM_SUPPORT
        BOOLEAN     drm_permission = TRUE;
        if (MMIAPIFMM_IsDrmFileBySuffix(s_current_event_info.event.event_ring_info.dring_name_arr,
                s_current_event_info.event.event_ring_info.dring_name_len))
        {
            //����Ƿ�����ˣ���������ˣ����ó�Ĭ�ϵ�ͼƬ
            if (MMIAPIDRM_IsRightsExpired(SFS_INVALID_HANDLE,
                    s_current_event_info.event.event_ring_info.dring_name_arr, DRM_PERMISSION_PLAY))
            {
                drm_permission = FALSE;
                SCI_TRACE_LOW("SetEditRingParamWin: drm_perssion set as false\n");
            }
        }
        #endif

        ring_content_str.wstr_ptr = file_name;
        if(ALM_DOWNLOAD_RING == s_current_event_info.event.event_content_info.ring_type
                ||ALM_RECORD_RING == s_current_event_info.event.event_content_info.ring_type)
        {
            //SCI_ASSERT(NULL != ring_name_ptr); /*assert verified*/
            ring_name_ptr = s_current_event_info.event.event_ring_info.dring_name_arr;
            ring_name_len = s_current_event_info.event.event_ring_info.dring_name_len;

            if (MMIAPIUDISK_UdiskIsRun())//@zhaohui,cr111342
            {
                 ring_content_str.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
            }
            else if(!MMIAPIFMM_IsFileExist(ring_name_ptr, ring_name_len)
                    #ifdef DRM_SUPPORT
                    || !drm_permission
                    #endif
                    )
            {
                //�����ļ�������ʱ������������
                s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
                s_current_event_info.event.event_content_info.ring = 1;
                //MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);

                ring_content_str.wstr_len = MMIAPISET_GetRingName(1, file_name, MMIFILE_FILE_NAME_MAX_LEN);
            }
            else
            {
                MMIAPIFMM_SplitFullPath(ring_name_ptr, ring_name_len,
                                      NULL, NULL, NULL, NULL,
                                      file_name, &file_name_len);
                ring_content_str.wstr_len = file_name_len;
            }
        }
        else if(ALM_FIX_RING == s_current_event_info.event.event_content_info.ring_type)
        {
            fixed_ringid = s_current_event_info.event.event_content_info.ring;
            ring_content_str.wstr_len = MMIAPISET_GetRingName(fixed_ringid, file_name,
                    MMIFILE_FILE_NAME_MAX_LEN);
        }

        GUILIST_SetMaxItem(list_ctrl_id, 1, FALSE );

        item_t.item_data_ptr = &item_data;

        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS;
        item_data.item_content[ALM_EDIT_LIST_ITEM_LABEL_INDEX].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[ALM_EDIT_LIST_ITEM_LABEL_INDEX].item_data.text_id = STR_LABEL_ALARM_TONE_EXT01;
        GUILIST_SetItemState(&item_t,GUIITEM_STATE_COSTMER_FONT_COLOR,TRUE);
        GUILIST_SetItemContentCustomFontColor(list_ctrl_id,ALM_EDIT_LIST_ITEM_LABEL_INDEX,
                &font_color,alpha,&font_color,alpha);

        item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data.text_buffer.wstr_len = ring_content_str.wstr_len;
        item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data.text_buffer.wstr_ptr = ring_content_str.wstr_ptr;

        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
        item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
        GUILIST_AppendItem( list_ctrl_id, &item_t );
    }
}

LOCAL MMI_RESULT_E  HandleAlarmRingSelWinMsg(
                                               MMI_WIN_ID_T     win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIALM_FIXEDRINGLIST_CTRL_ID;
//    uint16          i = 0;
    uint16          cur_select = 0;
    uint16          ring_id = 0;
    uint8           ring_vol=0;
    BOOLEAN is_fixed_ring = TRUE;
    MULTIM_SELECT_RETURN_T  *win_param = NULL;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISRVAUD_ReqVirtualHandle("FIX ALARM SELECT RING", MMISRVAUD_PRI_NORMAL);
        GUILIST_SetMaxItem(ctrl_id,/*MMIALM_RING_FIXED_DEFAULT_NUM*/g_clock_ring_max_num + 1,
                FALSE );//max item 9
        MMIAPISET_AppendFixedItems(
                                    TXT_RING,
                                    /*MMIALM_RING_FIXED_DEFAULT_NUM*/g_clock_ring_max_num,
                                    IMAGE_NULL,
                                    STXT_SOFTKEY_SELECT_MK,
                                    TXT_COMMON_COMMON_BACK,
                                    ctrl_id,
                                    GUIITEM_STYLE_ONE_LINE_TEXT_MS
                                    );

        if(ALM_FIX_RING == s_current_event_info.event.event_content_info.ring_type)
        {
            ring_id = s_current_event_info.event.event_content_info.ring;
            cur_select = ring_id - 1;
            //GUILIST_ChangeIconId(ctrl_id,cur_select,IMAGE_SELECT_TICK_ON_PIC,IMAGE_NULL);
        }
        else
        {
            ring_id = 1;
            cur_select = 0;
            is_fixed_ring = FALSE;
        }

        GUILIST_SetCurItemIndex(ctrl_id, cur_select);
        GUILIST_SetSelectedItem(ctrl_id, cur_select, TRUE);
        ////GUILIST_UpdateListBox(ctrl_id);

        MMK_SetAtvCtrl(win_id, ctrl_id);
        if (!is_fixed_ring)
            break;

        ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
        if (0 == ring_vol)
        {
            ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
        break;

    case MSG_FULL_PAINT:
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_HEADER_RINGING_TYPE);
        break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;

            switch (src_id)
            {
            case MMIALM_FIXEDRINGLIST_CTRL_ID:
                MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
                cur_select = GUILIST_GetCurItemIndex(ctrl_id);

                ring_id = cur_select+1;
                ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
                if (0 == ring_vol)
                {
                    ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
                }
                MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }

        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#else
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP:
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        cur_select = GUILIST_GetCurItemIndex(ctrl_id);

        ring_id = cur_select + 1;
        ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
        if (0 == ring_vol)
        {
            ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
        break;
#endif

    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    //case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if ( MMK_IsSingleClick(msg_id, param))
        {
            break;
        }
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);

        cur_select = GUILIST_GetCurItemIndex(ctrl_id);
        if (cur_select == 0)
        {
           // MMIAPMULTILIST_EnterSongListWin(win_id); 
           // break;
        }
        ring_id = cur_select + 1;
        s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
        s_current_event_info.event.event_content_info.ring = ring_id;
        //MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event);

//@zhaohui,cr108296,lizhen:�˴�����Ҫ����"�����"
//      MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

#ifdef DPHONE_SUPPORT
#else
        MMK_CloseWin( MMIALM_RINGLIST_WIN_ID );
#endif
        MMK_CloseWin( win_id );
        break;

    case MSG_MULTIM_SELECTED_RETURN:
        win_param = (MULTIM_SELECT_RETURN_T *)param;

        s_current_event_info.event.event_content_info.ring_type = ALM_DOWNLOAD_RING;
        s_current_event_info.event.event_ring_info.dring_name_len = win_param->file_name_len;

        SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
        MMI_WSTRNCPY(s_current_event_info.event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
                   win_param->file_name_ptr, win_param->file_name_len, win_param->file_name_len);

        s_current_event_info.event.event_content_info.ring = MMISET_MAX_RING_ID;
//        CloseDelayAlarm(s_current_event_info.id, s_current_event_info.event, FALSE);
//        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event);
        //lizhen:�˴�����Ҫ����"�����"
//        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(win_id);//@zhaohui,108296
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMISRVAUD_FreeVirtualHandle("FIX ALARM SELECT RING");
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
LOCAL void SetSnoozeTimeParamEditWin(void)
{
    MMI_CTRL_ID_T list_ctrl_id = MMIALM_EDIT_SNOOZE_TIME_CTRL_ID;

    GUILIST_ITEM_T          item_t       = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};

    MMI_STRING_T  snooze_time_str        = {0};

    GUI_COLOR_T font_color = MMI_DEFAULT_TEXT_COLOR;
    uint8 alpha = 255 * 60 / 100;

    if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF != s_current_event_info.id)
    {
        GUILIST_SetMaxItem(list_ctrl_id, 1, FALSE );

        item_t.item_data_ptr = &item_data;


        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS;
        item_data.item_content[ALM_EDIT_LIST_ITEM_LABEL_INDEX].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[ALM_EDIT_LIST_ITEM_LABEL_INDEX].item_data.text_id = STR_LABEL_SNOOZE_TIME_EXT01;
        GUILIST_SetItemState(&item_t,GUIITEM_STATE_COSTMER_FONT_COLOR,TRUE);
        GUILIST_SetItemContentCustomFontColor(list_ctrl_id,ALM_EDIT_LIST_ITEM_LABEL_INDEX,
                &font_color,alpha,&font_color,alpha);

        GetSnoozeTimeStr(&snooze_time_str);
        item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data.text_buffer.wstr_len = snooze_time_str.wstr_len;
        item_data.item_content[ALM_EDIT_LIST_ITEM_CONTENT_INDEX].item_data.text_buffer.wstr_ptr = snooze_time_str.wstr_ptr;

        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
        item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
        GUILIST_AppendItem( list_ctrl_id, &item_t );

    }
}

LOCAL void GetSnoozeTimeStr(MMI_STRING_T *p_snooze_time_str)
{
    uint8 snooze_type =
            s_current_event_info.event.event_fast_info.snooze_type;

    switch(snooze_type)
    {
        case ALM_SNOOZE_TIME_2_MIN:
            MMI_GetLabelTextByLang(STR_ILIST_2_MINUTES_EXT01,p_snooze_time_str);
            break;
        case ALM_SNOOZE_TIME_5_MIN:
            MMI_GetLabelTextByLang(STR_ILIST_5_MINUTES_EXT01,p_snooze_time_str);
            break;
        case ALM_SNOOZE_TIME_10_MIN:
            MMI_GetLabelTextByLang(STR_ILIST_10_MINUTES_EXT01,p_snooze_time_str);
            break;
        case ALM_SNOOZE_TIME_15_MIN:
            MMI_GetLabelTextByLang(STR_ILIST_15_MINUTES_EXT01,p_snooze_time_str);
            break;
        case ALM_SNOOZE_TIME_20_MIN:
            MMI_GetLabelTextByLang(STR_ILIST_20_MINUTES_EXT01,p_snooze_time_str);
            break;
        default:
            break;
    }
}

LOCAL void AppendAlarmSnoozeTimeListBoxItem(
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    uint8       cur_select = s_current_event_info.event.event_fast_info.snooze_type;
    uint8       max_item_num = ALM_SNOOZE_NUM;
    uint8       i = 0;
    MMI_TEXT_ID_T   snooze_time_arr[ALM_SNOOZE_NUM] =
    {
        STR_ILIST_2_MINUTES_EXT01,
        STR_ILIST_5_MINUTES_EXT01,
        STR_ILIST_10_MINUTES_EXT01,
        STR_ILIST_15_MINUTES_EXT01,
        STR_ILIST_20_MINUTES_EXT01
    };

    //set list max item and selected max item
    GUILIST_SetMaxItem(ctrl_id,max_item_num,FALSE);

    //append item
    for (i = 0; i < max_item_num; i++)
    {
        AppendListItemByTextId(snooze_time_arr[i],ctrl_id,GUIITEM_STYLE_RADIO_ONE_LINE_TEXT);
    }

    GUILIST_SetCurItemIndex(ctrl_id, cur_select);
    GUILIST_SetSelectedItem(ctrl_id, cur_select, TRUE);
}

LOCAL MMI_RESULT_E  HandleAlarmSnoozeTimeListWinMsg(
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  DPARAM            param
                                                  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       current_ctrl_id = MMIALM_SNOOZETIMELIST_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:      //create check list box
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_HDR_SNOOZE_TIME_EXT01);
        AppendAlarmSnoozeTimeListBoxItem(current_ctrl_id);

        // read info of alarm , set check flag to indicate if the alarm is setting
        //GUILIST_SetMaxSelectedItem(current_ctrl_id, ALM_MODE_LIST);
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;

    case MSG_CTL_LIST_MARK_ITEM:
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK: 
    case MSG_CTL_MIDSK:
        s_current_event_info.event.event_fast_info.snooze_type =
        GUILIST_GetCurItemIndex(current_ctrl_id);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_GET_FOCUS:
        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_HDR_SNOOZE_TIME_EXT01);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

LOCAL uint16 GetSnoozeTimeByType(ALM_SNOOZE_TYPE_E snooze_type)
{
    uint16 snooze_time = ALM_DELAY_TIME_15_MIN_SEC;

    switch(snooze_type)
    {
    case ALM_SNOOZE_TIME_2_MIN:
        snooze_time = ALM_DELAY_TIME_2_MIN_SEC;
        break;
    case ALM_SNOOZE_TIME_5_MIN:
        snooze_time = ALM_DELAY_TIME_5_MIN_SEC;
        break;
    case ALM_SNOOZE_TIME_10_MIN:
        snooze_time = ALM_DELAY_TIME_10_MIN_SEC;
        break;
    case ALM_SNOOZE_TIME_15_MIN:
        snooze_time = ALM_DELAY_TIME_15_MIN_SEC;
        break;
    case ALM_SNOOZE_TIME_20_MIN:
        snooze_time = ALM_DELAY_TIME_20_MIN_SEC;
        break;
    default:
        break;
    }

    return snooze_time;
}

LOCAL void SaveEvent(MMI_WIN_ID_T win_id)
{
    BOOLEAN is_need_save = TRUE;

    //CR177677:�ж���һ���Ƿ���������������ʱ���Ƿ�һ�£���һ�£�������ʾ��
    if (EVENT_AUTO_POWERON == s_current_event_info.id)
    {
        MMIACC_SMART_EVENT_FAST_TABLE_T   *poweroff_event = {0};
        MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
        poweroff_event = MMIALM_GeEvent(EVENT_AUTO_POWEROFF);
        if (poweroff_event->is_on)
        {//������
            MMISET_TIME_T   time = {0};
#ifdef  MMI_PDA_SUPPORT
            time.time_hour = s_current_event_info.event.event_fast_info.hour;
            time.time_minute = s_current_event_info.event.event_fast_info.minute;
#else
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
            MMIALARM_GetTime(&time.time_hour, &time.time_minute);
#else
            GUIEDIT_GetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
#endif
#endif
            if((poweroff_event->minute == time.time_minute)
                    &&(poweroff_event->hour == time.time_hour))
            {
                MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                is_need_save = FALSE;
            }
        }
    }
    else if(EVENT_AUTO_POWEROFF == s_current_event_info.id)
    {
        MMIACC_SMART_EVENT_FAST_TABLE_T   *poweron_event = {0};
        MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
        poweron_event = MMIALM_GeEvent(EVENT_AUTO_POWERON);
        if (poweron_event->is_on)
        {//������
            MMISET_TIME_T   time = {0};
#ifdef  MMI_PDA_SUPPORT
            time.time_hour = s_current_event_info.event.event_fast_info.hour;
            time.time_minute = s_current_event_info.event.event_fast_info.minute;
#else
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
            MMIALARM_GetTime(&time.time_hour, &time.time_minute);
#else
            GUIEDIT_GetTime(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
#endif
#endif
            if((poweron_event->minute == time.time_minute)
                    &&(poweron_event->hour == time.time_hour))
            {
                MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                is_need_save = FALSE;
            }
        }
    }
    if(is_need_save)
    {
        //save name
        if (EVENT_AUTO_POWERON != s_current_event_info.id
            && EVENT_AUTO_POWEROFF!= s_current_event_info.id)
        {
            if(!SaveCustomNameEditWin())
            {
                return;
            }
        }
        //save time
        if(!SaveTimeEditWin())
        {
            return;
        }

        s_current_event_info.event.event_fast_info.is_on = TRUE;
        // �ر���ʱ����
        SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
        // ��ʾ����
        //@zhaohui,cr108847
        if (MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event))
        {
            #ifdef FM_SUPPORT
            if(s_current_event_info.event.event_content_info.ring_type!=ALM_FM_RING)
            {
                #if 0 //#ifndef MMI_PDA_SUPPORT
                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                #endif
            }
            else
            #endif
            {
                #ifdef MMI_FM_NEED_HEADSET
                MMIPUB_OpenAlertWarningWin(TXT_FM_TIMER_SAVE_PROMPT);
                #endif
            }
            MMIALM_RemoveIdlePushMsg(s_current_event_info.id,SMART_EVENT_ALARM);
        }
        else
        {
            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
            s_current_event_info.event.event_content_info.ring = 1;
            MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
            MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        }
        MMK_CloseWin(win_id);
    }
}

LOCAL MMI_RESULT_E  HandleAlarmEditOptWin(
                                           MMI_WIN_ID_T    win_id,   // ����ID
                                           MMI_MESSAGE_ID_E   msg_id,   // ��ϢID
                                           DPARAM             param     // ��Ϣ����
                                           )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIALM_EDIT_MENU_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    //case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint32      group_id    =   0;
            uint32      menu_id     =   0;
            GUIMENU_GetId(ctrl_id,&group_id,&menu_id);

            switch(menu_id)
            {
            case ID_ALM_OPT_SAVE:
                SaveEvent(MMIALM_EDIT_WIN_ID);
                break;

            default:
                break;
            }
            MMK_CloseWin( win_id );
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

LOCAL MMI_RESULT_E HandleAlarmDeleteAllQueryWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        {
            //uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
            uint8 i = 0;

            for (i = 0; i< ALM_ALARM_NUM; i++)
            {
                MMIACC_SMART_EVENT_T smart_event = {0};

                MMIAPIALM_FormInteEventInfo(i, &smart_event);
                if(smart_event.event_content_info.is_add)
                {
                    s_current_event_info.id = i;

                    MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                    DeleteOneAlarm(&s_current_event_info,MMIALM_LISTALL_CTRL_ID,GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                    //MMK_PostMsg(MMIALM_MAINWIN_ID, MSG_FULL_PAINT, PNULL, 0);
                }
            }
            MMIIDLE_RemovePushMsgByType(MMI_IDLE_PUSH_MSG_SNOOZED_ALARM);
            MMK_CloseWin(win_id);
        }
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

LOCAL MMI_RESULT_E  HandleCustomAlarmMainOptWin(
                                           MMI_WIN_ID_T    win_id,   // ����ID
                                           MMI_MESSAGE_ID_E   msg_id,   // ��ϢID
                                           DPARAM             param     // ��Ϣ����
                                           )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIALM_OPT_MENU_CTRL_ID;
    uint16 total_alm_num = GUILIST_GetTotalItemNum(MMIALM_LISTALL_CTRL_ID);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    //case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint32      group_id    =   0;
            uint32      menu_id     =   0;
            GUIMENU_GetId(ctrl_id,&group_id,&menu_id);

            switch(menu_id)
            {
            case ID_ALM_OPT_EDIT:
                {
                    uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));

                    if(ALM_ALARM_NUM > id)
                    {
                        s_current_event_info.id = id;

                        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                        if(!MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event))
                        {
                            s_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
                            s_current_event_info.event.event_content_info.ring = 1;
                        }
                        s_current_event_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;

                        MMK_CreateWin((uint32*)ALARM_CUSTOM_EDITWIN_TAB, PNULL);
                    }
                }
                break;

            case ID_ALM_OPT_DEL: 
                {
                    uint16 id = GetAlarmRealId(GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                    uint16 total_num = 0;
                    if(ALM_ALARM_NUM > id)
                    {
                        s_current_event_info.id = id;
                        
                        MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                        s_current_event_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
                        s_current_event_info.event.event_fast_info.is_on = FALSE;
                        s_current_event_info.event.event_content_info.w_length = 0;
                        s_current_event_info.event.event_content_info.is_add = FALSE;
                        SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
                        MMIAPIALM_SaveInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                        MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);	
                        GUILIST_RemoveItem(MMIALM_LISTALL_CTRL_ID, GUILIST_GetCurItemIndex(MMIALM_LISTALL_CTRL_ID));
                        total_num = GUILIST_GetTotalItemNum(MMIALM_LISTALL_CTRL_ID);
                        if(0 == total_num)
                        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
                            GUIWIN_SetSoftkeyTextId(MMIALM_MAINWIN_ID, 
                                STXT_OPTION, 
                                TXT_NULL, 
                                TXT_COMMON_COMMON_BACK,
                                FALSE);
                            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
                            GUIWIN_SetSoftkeyTextId(MMIALM_MAINWIN_ID, 
                                STXT_OPTION, 
                                TXT_NULL, 
                                TXT_NULL,
                                FALSE);
                            GUIWIN_SeSoftkeytButtonTextId(MMIALM_MAINWIN_ID,TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,FALSE);
#endif
                        }
                        MMK_PostMsg(MMIALM_MAINWIN_ID, MSG_FULL_PAINT, PNULL, 0);
			 MMIALM_RemoveIdlePushMsg(s_current_event_info.id, SMART_EVENT_ALARM);
		     }
                }			 
                break;
                
            case ID_ALM_OPT_DEL_ALL:
                {
                    MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_QUERY_DELETE_ALL_ALARMS_EXT01,PNULL,
                            IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleAlarmDeleteAllQueryWinMsg);
                }
                break;

            default:
                break;
            }
            MMK_CloseWin( win_id );
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

LOCAL void SetMainWinSoftkeyByActiveCtrl(
                                            MMI_HANDLE_T    win_id,
                                            uint32          ctrl_id,
                                            BOOLEAN         is_need_update
                                            )
{
    switch(ctrl_id)
    {
        case MMIALM_NEW_ALARM_CTRL_ID:
            GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_NULL,LEFT_BUTTON,TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_SOFTKEY_ADD_ALARM_EXT01,MIDDLE_BUTTON,TRUE);
            break;
        case MMIALM_LISTALL_CTRL_ID:
            //GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_MENU,LEFT_BUTTON,TRUE);
            break;
        case MMIALM_TEXT_NO_ALARMS_CTRL_ID:
            GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_NULL,LEFT_BUTTON,TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_NULL,MIDDLE_BUTTON,TRUE);
            break;

        default:
            break;
    }
}

LOCAL MMI_RESULT_E HandleCustomAutoPowerOnOffWinMsg(
                                              MMI_WIN_ID_T        win_id,
                                              MMI_MESSAGE_ID_E    msg_id,
                                              DPARAM              param
                                              )
{
    uint16                      i                = 0;
    MMI_CTRL_ID_T               ctrl_id          = MMIALM_AUTOPOWER_LIST_CTRL_ID;
    MMI_RESULT_E                recode           = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T               text_label[]     = {TXT_AUTOPOWER_ON, TXT_AUTOPOWER_OFF};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetAutoPowerListItem(ctrl_id);
        MMK_SetAtvCtrl( win_id, ctrl_id );
        break;

    case MSG_FULL_PAINT:
        /*for ( i = 0; i < ALM_AUTOPOWER_NUM; i++ )
        {
            uint16 event_id = (uint16)( EVENT_AUTO_POWERON + i );
            const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = MMIALM_GeEvent( event_id );

            SetAutoPowerListItem( ctrl_id,
                text_label[i],
                s_alarm_mode_text[event_ptr->fre_mode],
                event_id,
                i,
                TRUE );
        }*/
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifndef MMI_GUI_STYLE_TYPICAL
     case MSG_CTL_LIST_CHECK_CONTENT:
#endif
        {
            uint16 cur_idx = GUILIST_GetCurItemIndex(MMIALM_AUTOPOWER_LIST_CTRL_ID);
            BOOLEAN is_need_full_paint = TRUE;
            switch(cur_idx)
            {
            case TYPE_AUTO_POWERON:
            case TYPE_AUTO_POWEROFF:
                if(TYPE_AUTO_POWERON == cur_idx)
                {
                    s_current_event_info.id = EVENT_AUTO_POWERON;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWERON;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWERON;
                }
                else
                {
                    s_current_event_info.id = EVENT_AUTO_POWEROFF;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWEROFF;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWEROFF;
                }
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event);
                if (s_current_event_info.event.event_fast_info.is_on)
                {
                    s_current_event_info.event.event_fast_info.is_on = FALSE;
                }
                else
                {
                    //CR177677:�ж���һ���Ƿ���������������ʱ���Ƿ�һ�£���һ�£�������ʾ��
                    SCI_TRACE_LOW("[MMIALARM.C][HandleAutoPowerOnOffWinMsg] s_current_event_info.id = %d",s_current_event_info.id);
                    if (EVENT_AUTO_POWERON == s_current_event_info.id)
                    {
                        MMIACC_SMART_EVENT_FAST_TABLE_T   *poweroff_event = {0};
                        MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
                        poweroff_event = MMIALM_GeEvent(EVENT_AUTO_POWEROFF);
                        if (poweroff_event->is_on)
                        {//������
                            if((poweroff_event->minute == s_current_event_info.event.event_fast_info.minute)
                                    &&(poweroff_event->hour == s_current_event_info.event.event_fast_info.hour))
                            {
                                MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
                                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                                is_need_full_paint = FALSE;
                            }
                        }

                    }
                    else if(EVENT_AUTO_POWEROFF == s_current_event_info.id)
                    {
                        MMIACC_SMART_EVENT_FAST_TABLE_T   *poweron_event = {0};
                        MMI_WIN_ID_T alert_win_id = MMIALM_CONFLIT_WIN_ID;
                        poweron_event = MMIALM_GeEvent(EVENT_AUTO_POWERON);
                        if (poweron_event->is_on)
                        {
                            //������
                            if((poweron_event->minute == s_current_event_info.event.event_fast_info.minute)
                                    &&(poweron_event->hour == s_current_event_info.event.event_fast_info.hour))
                            {
                                MMK_CloseWin(MMIALM_CONFLIT_WIN_ID);
                                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ALM_CONFLIT,PNULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                                is_need_full_paint = FALSE;
                            }
                        }
                    }

                    //FALSE == s_current_event_info.event.event_fast_info.is_on && ��=
                    if (is_need_full_paint)
                    {
                        s_current_event_info.event.event_fast_info.is_on = TRUE;
                    }

                }
                if (is_need_full_paint)
                {
                    // �ر���ʱ����
                    SetDelayAlarm(s_current_event_info.id, &s_current_event_info.event, FALSE);
                    // �޸���������
                    MMIALM_ModifyEvent(s_current_event_info.id, &s_current_event_info.event.event_fast_info);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                }
                break;

            default:
                break;
            }
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            uint16 cur_idx = GUILIST_GetCurItemIndex(MMIALM_AUTOPOWER_LIST_CTRL_ID);
            switch(cur_idx)
            {
            case TYPE_AUTO_POWERON:
            case TYPE_AUTO_POWEROFF:
                if(TYPE_AUTO_POWERON == cur_idx)
                {
                    s_current_event_info.id = EVENT_AUTO_POWERON;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWERON;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWERON;
                }
                else
                {
                    s_current_event_info.id = EVENT_AUTO_POWEROFF;
                    s_current_event_info.type = SMART_EVENT_AUTOPOWEROFF;
                    s_current_event_info.event.event_fast_info.type = SMART_EVENT_AUTOPOWEROFF;
                }
                MMIAPIALM_FormInteEventInfo(s_current_event_info.id,&s_current_event_info.event);
                MMIAPIALM_GetRingInfo(s_current_event_info.id,&s_current_event_info.event);
                MMK_CreateWin((uint32*)POWERONOFF_CUSTOM_EDIT_WIN_TAB, PNULL);
                break;

            default:
                break;
            }
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;

}

LOCAL void AppendTwoLineTextAnd2TextListItem(
                                            MMI_CTRL_ID_T    ctrl_id,
                                            MMI_TEXT_ID_T    txt_id_line_1,
                                            MMI_STRING_T     *p_txt_line_2_1,
                                            MMI_STRING_T     *p_txt_line_2_2)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    uint8 index = 0;


    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS;
    item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[index].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[index].item_data.text_id  = txt_id_line_1;

    index++;
    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[index].item_data.text_buffer.wstr_ptr =
            p_txt_line_2_1->wstr_ptr;
    item_data.item_content[index].item_data.text_buffer.wstr_len =
            p_txt_line_2_1->wstr_len;

    if(p_txt_line_2_2 != PNULL)
    {
        index++;
        item_data.item_content[index].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[index].item_data.text_buffer.wstr_len =
            p_txt_line_2_2->wstr_len;
        item_data.item_content[index].item_data.text_buffer.wstr_ptr =
            p_txt_line_2_2->wstr_ptr;
    }

    GUILIST_AppendItem( ctrl_id, &item_t );
}

LOCAL void SetAutoPowerListItem(
                            MMI_CTRL_ID_T    ctrl_id
                            )
{
    MMI_TEXT_ID_T           txt_alarm_id[] =
            {STR_SETT_LIST_POWER_ON_EXT01,STR_SETT_LIST_POWER_OFF_EXT01};
    MMIACC_SMART_EVENT_T    smart_event = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_STRING_T    txt_time_str = {0};
    MMI_STRING_T    txt_repeat_str = {0};
    int i = 0;


    GUILIST_SetMaxItem( ctrl_id, ALM_AUTOPOWER_NUM, FALSE );//max item 2

//���ø�����list item�е�λ�ã�����䶯;
//��Ӧ����: txt_alarm_id/event_time/alarm_mode
    for(i = 0; i < ALM_AUTOPOWER_NUM; i++)
    {
        uint16 event_id = (uint16)( EVENT_AUTO_POWERON + i );

        MMIAPIALM_FormInteEventInfo(event_id,&smart_event);
        #if 0 //def MMI_ALARM_DEL_ADD_SUPPORT
        if(!smart_event.event_content_info.is_add && event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
        {
            continue;//ͨ���������ж��Ƿ����û��½���
        }
        #endif

        SCI_MEMSET(&txt_time_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&temp_wstr,0,sizeof(temp_wstr));
        txt_time_str.wstr_ptr = temp_wstr;
        SCI_MEMSET(&txt_repeat_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&buff,0,sizeof(buff));
        txt_repeat_str.wstr_ptr = buff;

        MMIAPIALARM_GetEventTimeString( &smart_event.event_fast_info, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
        txt_time_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
        txt_repeat_str.wstr_len = MMIAPIALARM_GetFreModeString(smart_event.event_fast_info.fre_mode,smart_event.event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);

        AppendTwoLineTextAnd2TextListItem(ctrl_id,txt_alarm_id[i],&txt_time_str,&txt_repeat_str);
    }
}

LOCAL void SetPowerOnOffEditListItem(MMI_CTRL_ID_T    ctrl_id)
{
    MMI_TEXT_ID_T           txt_alarm_id[] =
            {STR_SETT_LABEL_TIME_EXT01,STR_SETT_LIST_REPEAT_EXT01};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    MMI_STRING_T    txt_time_repeat_str = {0};
    int i = 0;


    GUILIST_SetMaxItem( ctrl_id, ALM_POWERONOFF_EDIT_ITEM_NUM, FALSE );//max item 2

//���ø�����list item�е�λ�ã�����䶯;
//��Ӧ����: event_time/alarm_mode
    for(i = 0; i < ALM_POWERONOFF_EDIT_ITEM_NUM; i++)
    {
        SCI_MEMSET(&txt_time_repeat_str,0,sizeof(MMI_STRING_T));
        SCI_MEMSET(&temp_wstr,0,sizeof(temp_wstr));
        txt_time_repeat_str.wstr_ptr = temp_wstr;

        if(i == ALM_POWERONOFF_EDIT_ITEM_TIME)
        {
            MMIAPIALARM_GetEventTimeString( &s_current_event_info.event.event_fast_info, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
            txt_time_repeat_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
        }
        else
        {
            txt_time_repeat_str.wstr_len = MMIAPIALARM_GetFreModeString(s_current_event_info.event.event_fast_info.fre_mode,
                    s_current_event_info.event.event_fast_info.fre,temp_wstr,GUILIST_STRING_MAX_NUM+1);
        }

        AppendTwoLineTextAnd2TextListItem(ctrl_id,txt_alarm_id[i],&txt_time_repeat_str,PNULL);
    }
}

LOCAL MMI_RESULT_E  HandlePowerOnOffEditWinMsg(
                                          MMI_WIN_ID_T      win_id,
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id =MMIALM_EDIT_MODE_SETLIST_CTRL_ID;
    GUI_BG_T        bg_info = {0};

    uint16                  img_width  = 0;
    uint16                  img_height = 0;
    GUIFORM_CHILD_WIDTH_T   button_width = {0};
    GUIFORM_CHILD_HEIGHT_T  button_height = {0};
#ifdef MMI_RECORD_SUPPORT
    GUI_BG_T bg={0};
#endif
    BOOLEAN is_need_save = TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetCircularHandleUpDown(MMI_EDITWIN_FORM_CTRL_ID,TRUE);
        GUIFORM_SetType(MMI_EDITWIN_FORM_CTRL_ID,GUIFORM_TYPE_TP);

        GUIFORM_SetStyle(MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIAPICTRL_SetState( MMIALM_EDIT_RING_TITLE_FORM_CTRL_ID, GUICTRL_STATE_DISABLE_TP,TRUE );
        //NEWMS00222350
        GUIEDIT_SetHandleRedKey(FALSE, MMIALM_EDIT_NAME_EDITBOX_CTRL_ID);

        bg_info.bg_type = GUI_BG_COLOR;
        bg_info.color = MMI_BACKGROUND_COLOR;
        GUIFORM_SetBg(MMI_EDITWIN_FORM_CTRL_ID,&bg_info);
        GUIFORM_PermitChildFont(MMI_EDITWIN_FORM_CTRL_ID,FALSE);


#ifdef RECORD_SUPPORT
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_SCH_RECORD_ICON, win_id);
        button_width.add_data = img_width;
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        button_height.add_data = img_height;
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        GUIFORM_SetChildWidth(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMIALM_EDIT_RING_FORM_CTRL_ID, MMIALM_EDIT_RING_BUTTON_CTRL_ID, &button_height);
       // GUIBUTTON_SetCallBackFunc(MMIALM_EDIT_RING_BUTTON_CTRL_ID, RecordAlarmRingCallBack);
        GUIBUTTON_SetRunSheen(MMIALM_EDIT_RING_BUTTON_CTRL_ID,FALSE);
        bg.bg_type=GUI_BG_IMG;
        bg.img_id=IMG_SCH_RECORD_ICON;
        GUIBUTTON_SetPressedBg(MMIALM_EDIT_RING_BUTTON_CTRL_ID,&bg);
#endif


        SetCustomEditNameWin(win_id);
        SetCustomEditTimeWin();
        GUIEDIT_SetAlign(MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);

        //SetWeeksParamEditWin(win_id);
        //SetWeeksDisplayEditWin();

        if (EVENT_AUTO_POWERON != s_current_event_info.id && EVENT_AUTO_POWEROFF!= s_current_event_info.id)
        {
        #ifndef MMI_PDA_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID);
        #else
            MMK_SetAtvCtrl(win_id, MMI_EDITWIN_FORM_CTRL_ID);
        #endif
        }
        else
        {
        #ifndef MMI_PDA_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIALM_POWERONOFF_EDIT_CTRL_ID);
        #else
            MMK_SetAtvCtrl(win_id, MMIALM_EDIT_TIME_FORM_CTRL_ID);
        #endif
            GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID,MMIALM_EDIT_NAME_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
            GUIFORM_SetChildDisplay(MMI_EDITWIN_FORM_CTRL_ID,MMIALM_EDIT_RING_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
        }
        SetPowerOnOffEditListItem(MMIALM_POWERONOFF_EDIT_CTRL_ID);
        break;

    case MSG_FULL_PAINT:
        SetEditRepeatWin();
        SetEditRingParamWin();
        SetSnoozeTimeParamEditWin();
        break;
    case MSG_GET_FOCUS:
        break;

    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
        //EditWinMIDSKProcess(win_id);
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        HandleAlarmEditMskMsg(win_id, ctrl_id);
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
         ctrl_id = MMK_GetActiveCtrlId(win_id);
         HandleAlarmEditFormSwitchActiveMsg(win_id,ctrl_id);
         break;

#ifdef MMI_RECORD_SUPPORT
    case MMI_MSG_RECORDTORING:
        if(PNULL != param)
        {
            MMI_STRING_T        ring_name = {0};

            ring_name.wstr_ptr = (wchar*)param;
            ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
            SCI_MEMSET(s_current_event_info.event.event_ring_info.dring_name_arr,0,(MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
            s_current_event_info.event.event_ring_info.dring_name_len = ring_name.wstr_len;
            MMIAPICOM_Wstrncpy(s_current_event_info.event.event_ring_info.dring_name_arr, ring_name.wstr_ptr, ring_name.wstr_len);
            SCI_MEMSET(ring_name.wstr_ptr,0,ring_name.wstr_len * sizeof(wchar));

            MMIAPIFMM_SplitFullPath(s_current_event_info.event.event_ring_info.dring_name_arr, s_current_event_info.event.event_ring_info.dring_name_len, PNULL, PNULL, PNULL, PNULL, ring_name.wstr_ptr, &ring_name.wstr_len);

            s_current_event_info.event.event_content_info.ring_type = ALM_RECORD_RING;
        }
        DisplayTheRingName();
        MMK_CloseWin(MMIALM_RINGLIST_WIN_ID);
        break;
#endif

    case MSG_CTL_OK:
    case MSG_APP_OK:
        //CR177677:�ж���һ���Ƿ���������������ʱ���Ƿ�һ�£���һ�£�������ʾ��
        MMK_CreateWin((uint32 *)ALARM_EDIT_MENU_WIN_TAB, PNULL);
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(MMIAUTOPOWER_OPTWIN_ID);
        break;

    case MSG_CTL_SETLIST_SWITCH:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        if(MMIALM_EDIT_MODE_SETLIST_CTRL_ID==ctrl_id)
        {
            SetWeeksDisplayEditWin();
        }

        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
//[end]

LOCAL void SetNewAddedAlarmDelayTime(
                                uint16 event_id, 
                                MMIACC_SMART_EVENT_T *event_ptr
                                )
{
    SCI_DATE_T cur_date = {0};
    SCI_TIME_T cur_time = {0};
    uint16 delay_event_id = 0;
    MMIALMSVC_ID_T temp_service_id = 0;

    if (GetEventAndDelayedIds(&event_id, &delay_event_id))
    {
    }
    else
    {
        return;
    }

    event_ptr->event_fast_info.is_on = TRUE;

    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);

    // Delay 1m from now, then notify.
    IncreaseTimeBySecond(&cur_date, &cur_time, 60);

    temp_service_id = s_event_fast_table[delay_event_id].service_id;
    SCI_MEMCPY(&s_event_fast_table[delay_event_id], (uint8 *)&event_ptr->event_fast_info, sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
    s_event_fast_table[delay_event_id].service_id = temp_service_id;//service_id������Ӧ���Լ���event_id

    s_event_fast_table[delay_event_id].year = cur_date.year;
    s_event_fast_table[delay_event_id].mon = cur_date.mon;
    s_event_fast_table[delay_event_id].day = cur_date.mday;
    s_event_fast_table[delay_event_id].hour = cur_time.hour;
    s_event_fast_table[delay_event_id].minute = cur_time.min; 
    // Temporarily reset remind_type.
    s_event_fast_table[delay_event_id].remind_type = MMISCH_REMIND_ONTIME;

    if (ALM_MODE_ONCE == s_event_fast_table[delay_event_id].fre_mode)
    {
        s_event_fast_table[event_id].is_on = FALSE;
    }

    // ����
    MMINV_WRITE(MMINV_ACC_SMART_EVENT_FAST_TABLE, s_event_fast_table);

    MMINV_WRITE(MMINV_ACC_SMART_EVENT_ALM_1_CONTENT+delay_event_id, &event_ptr->event_content_info);  

    // ���÷���
    MMIALMSVC_FreeAlmRegService(temp_service_id, TRUE);//CR136024
    SetAlarmService(delay_event_id, TRUE);
    // �������������
    FindLatestEventToSet();
}

/*****************************************************************************/
//  Discription: Snooze a new schedule if its notification time has passed for reminder_type.
//  Global resource dependence: none 
//  Author: sunhongzhe
//  Note: Only for a new or updated schedule.
//
/*****************************************************************************/
PUBLIC void MMIALM_SnoozeNewAddedSch(uint16 index, MMIACC_SMART_EVENT_T *event_ptr)
{
    uint16 event_id = 0;
    SCI_DATE_T              cur_date = {0};
    SCI_TIME_T              cur_time = {0};
    SCI_DATE_T              real_notify_date = {0};
    SCI_TIME_T              real_notify_time = {0};
    SCI_DATE_T              second_date = {0};
    SCI_TIME_T              second_time = {0};
    MMI_TM_T    time  = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T *fast_event_ptr = PNULL;
    BOOLEAN needDelayNow = FALSE;

    if (PNULL == event_ptr) return;

    fast_event_ptr = &event_ptr->event_fast_info;
    if (!(fast_event_ptr->is_valid && fast_event_ptr->is_on && fast_event_ptr->remind_type != MMISCH_REMIND_NONE))
    {
        return;
    }

    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);

    second_date.year  = fast_event_ptr->year;
    second_date.mon  = fast_event_ptr->mon;
    second_date.mday = fast_event_ptr->day;
    second_time.hour = fast_event_ptr->hour;
    second_time.min = fast_event_ptr->minute;
    second_time.sec = 0;

    time = MMIALM_GetSchTime(
                                    fast_event_ptr->year, 
                                    fast_event_ptr->mon, 
                                    fast_event_ptr->day, 
                                    fast_event_ptr->hour, 
                                    fast_event_ptr->minute, 
                                    fast_event_ptr->remind_type, 
                                    TRUE);
    real_notify_date.year = time.tm_year;
    real_notify_date.mon = time.tm_mon;
    real_notify_date.mday = time.tm_mday;
    real_notify_time.hour = time.tm_hour;
    real_notify_time.min = time.tm_min;

    if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL
        && (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, real_notify_date, real_notify_time) == CMP_SECOND_SMALL
               || MMIAPIALM_CompareDateAndTime(cur_date, cur_time, real_notify_date, real_notify_time) == CMP_EQUAL))
    {
        needDelayNow = TRUE;
    }

    if (!needDelayNow)
    {
        return;
    }

    SetNewAddedAlarmDelayTime(index, event_ptr);
}

LOCAL void SetDefaultAlarmValue(EVENT_CURRENT_DATA_T *cur_event_ptr)
{
    MMIACC_SMART_EVENT_T smart_event = cur_event_ptr->event;
    MMI_STRING_T txt_default_alarm_name = {0};

    cur_event_ptr->type = SMART_EVENT_ALARM;
    smart_event.event_fast_info.type = SMART_EVENT_ALARM;
    smart_event.event_fast_info.is_on = FALSE;
    smart_event.event_content_info.is_add = FALSE;
    //set time
    smart_event.event_fast_info.hour = 9;
    smart_event.event_fast_info.minute = 0;
    //set repeat
    smart_event.event_fast_info.fre_mode = ALM_MODE_ONCE;
    smart_event.event_fast_info.fre = 0x0;
    //set alarm name
    SCI_MEMSET(smart_event.event_content_info.w_content,0,sizeof(wchar)*(MMIACC_EVENT_TEXT_MAX_LEN+1));
    smart_event.event_content_info.w_length = 0;
    //set ringtone
    smart_event.event_content_info.ring_type = ALM_FIX_RING;
    smart_event.event_content_info.ring = /*ALM_DEFAULT_ALARM_RINGTONG*/g_clock_ring_default_id;
    //set snooze
    smart_event.event_fast_info.snooze_type = ALM_SNOOZE_TIME_10_MIN;

    //save alarm info
    SetDelayAlarm(cur_event_ptr->id, &smart_event, FALSE);
    MMIAPIALM_SaveInteEventInfo(cur_event_ptr->id,&smart_event);
}

LOCAL void DeleteOneAlarm(
                            EVENT_CURRENT_DATA_T *cur_event_ptr,
                            MMI_CTRL_ID_T list_ctrl_id,
                            uint16 cur_item_index
                            )
{
    SetDefaultAlarmValue(cur_event_ptr);
    GUILIST_RemoveItem(list_ctrl_id,cur_item_index);
}

LOCAL void MMIALM_RemoveIdlePushMsg(
                                uint16 event_id,
                                MMIACC_SMART_EVENT_E alarm_type
                                )
{
    uint8 record_key[MMIIDLE_APP_NAME_MAX_LEN + 1] = {0};

    MMIAPICOM_Int2Str(event_id, record_key, MMIIDLE_APP_NAME_MAX_LEN);
    MMIIDLE_RemovePushMsg(alarm_type == SMART_EVENT_ALARM ? MMI_IDLE_PUSH_MSG_SNOOZED_ALARM : MMI_IDLE_PUSH_MSG_SNOOZED_CALENDER,
        record_key, PNULL);
}

LOCAL void SetAlarmDialogSoftkey(MMI_WIN_ID_T win_id)
{
    MMIPUB_DIALOG_SOFTKEY_T dialog_softkey = {0};

    dialog_softkey.index = LEFT_BUTTON;
    dialog_softkey.content = TXT_NULL;
    dialog_softkey.type = DIALOG_SOFTKEY_TEXT_ID;
    MMIPUB_SetDialogSoftkey(&win_id,DIALOG_WITH_ALARM,&dialog_softkey);
}

LOCAL BOOLEAN IsAlarmNotifyInCall(MMIACC_SMART_EVENT_E type)
{
    BOOLEAN isNotify = FALSE;

    if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE)
            && type == SMART_EVENT_ALARM)
    {
        isNotify = TRUE;
    }
    return isNotify;
}
PUBLIC BOOLEAN MMI_IsRingAlarm(void)
{
	if (MMK_IsOpenWin(MMIALM_FIXEDRINGLIST_WIN_ID))
	{
		return TRUE;
	}     
	else
	{
		return FALSE;
	}
}
