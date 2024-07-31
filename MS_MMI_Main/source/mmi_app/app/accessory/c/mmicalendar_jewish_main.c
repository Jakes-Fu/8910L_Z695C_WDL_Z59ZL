/*****************************************************************************
** File Name:      mmicalendar_main.c                                               
** Author:           mark.hu    
** Date:            2005/0907
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe calendar window             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2005/09/07    mark.hu              Create                                 *
** 2006/08/09    liqing.peng          Modify
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef MMI_JEWISH_CALENDAR
#include "mmi_app_accessory_trc.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_text.h"
#include "guisoftkey.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "guilabel.h"
#include "mmi_theme.h"
#include "mmicalendar_export.h"
#include "mmiset_export.h"
#include "mmialarm_export.h"
#include "mmi_appmsg.h"
#include "mmi_solarlunar.h"
#include "mmialarm_id.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "guimenu.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmiacc_position.h"
#include "mmi_mainmenu_export.h"
#include "guires.h"
#include "guiedit.h"
#include "guistring.h"
#include "mmi_default.h"
#include "guirichtext.h"
#include "mmiacc_image.h"
#include "guistatusbar.h"
#include "mmicalendar_jewish_def.h"
#include "mmicalendar_jewish_export.h"

#include "mmiacc_nv.h"
#include "mmidatacounter_nv.h"
#include "guitext.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//pop menu
#define  MMIJEWISHCAL_POP_MENU_NODE_ID      0
#define  MMIJEWISHCAL_POP_MENU_TEXT_ID      1
#define  MMIJEWISHCAL_POP_MENU_ITEM_DIMEN   2

#define MMIJEWISHCAL_DAY_DETAIL_BG_COLOR    0xCDCDCD
#define MMIJEWISHCAL_TODAY_TEXT_COLOR       0x0000FF
#define MMIJEWISHCAL_GRAY_TEXT_COLOR        0x888888

#define LOCATION_TABLE_LEN                  (sizeof(location_settings_table)/sizeof(MMI_JEWISHCAL_LOCATION_ITEM_T))

#define MMIJEWISHCAL_FIRST_DAY_OF_WEEK      0
#define LINENUM                             6 
#define COLNUM                              WEEKDAY_NUM
#define VIEW_MARGIN                         3
#define LINE_SPACE                          2
#define MONTH_TITLE_TOP_MARGIN              10
#define MONTH_TITLE_LEFT                    29

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum {
    MMIJEWISHCAL_OPT_NULL,
    MMIJEWISHCAL_SEL_DATE,
    MMIJEWISHCAL_SEL_LOCATION,
    MMIJEWISHCAL_SETTING,
    MMIJEWISHCAL_ABOUT,
    MMIJEWISHCAL_HELP,
    MMIJEWISHCAL_EXIT,
    MMIJEWISHCAL_OPT_MAX
} MMIJEWISHCAL_OPT_E;

typedef enum{
    MMIJEWISHCAL_SELDATE_MODE_DATE,
    MMIJEWISHCAL_SELDATE_MODE_MONTH,
    MMIJEWISHCAL_SELDATE_MODE_MAX
}MMI_JEWISHCAL_SELDATE_MODE_E;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL const MMI_JEWISHCAL_LOCATION_ITEM_T location_settings_table[MMIJEWISHCAL_LOCATION_MAX_NUM]=
{
    {TXT_LOCATION_JERUSALEM, 31.78, -35.22, 2.0, 800, FALSE, 40}, //jerusalem
    {TXT_LOCATION_BNEY_BRAK, 32.08, -34.82, 2.0, 65, FALSE, 21}, //bney_brak
    {TXT_LOCATION_HAIFA, 32.8, -34.991, 2.0, 350, FALSE, 30}, //haifa
    {TXT_LOCATION_MODIIN_ILIT, 31.93, -35.04, 2.0, 300, FALSE, 30}, //modiin_ilit
    {TXT_LOCATION_BET_SHEMESH, 31.74, -34.98, 2.0, 300, FALSE, 30}, //bet_shemesh
    {TXT_LOCATION_BEITAR_ILIT, 31.69, -35.12, 2.0, 800, FALSE, 30}, //beitar_ilit
    {TXT_LOCATION_ELAD, 32.05, -34.95, 2.0, 150, FALSE, 30}, //elad
    {TXT_LOCATION_NETIVOT, 31.42, -34.59, 2.0, 142, FALSE, 30}, //netivot
    {TXT_LOCATION_TSFAT, 32.962, -35.496, 2.0, 850, FALSE, 30}, //tsfat
    {TXT_LOCATION_LONDON, 51.6, 0.1, 0.0, 45, TRUE, 30}, //london
    {TXT_LOCATION_NEW_YORK, 40.71, 74, -5.0, 6, TRUE, 30}, //new_york
    {TXT_LOCATION_PARIS, 48.86, -2.34, 1.0, 50, TRUE, 30}, //paris
    {TXT_LOCATION_ZIRICH, 47.37, -8.54, 1.0, 410, TRUE, 30}, //zirich
    {TXT_LOCATION_ANTVERPEN, 51.22, -4.41, 1.0, 0, TRUE, 30}, //antverpen
};

LOCAL const MMI_TEXT_ID_T s_dls_option_text_id[MMIJEWISHCAL_DLS_SETTING_MAX_NUM] =
{
    TXT_SETTINGS_SCR_DLS_OPTION_0,
    TXT_SETTINGS_SCR_DLS_OPTION_1,
    TXT_SETTINGS_SCR_DLS_OPTION_2,
};

LOCAL const uint8 s_bavli_pages_per_masechet[MMIJEWISHCAL_BAVLI_MASECHET_NUM] = 
{63,156,104,120,21,87,55,39,34,30,31,28,26,121,111,90,65,48,89,81,118,118,175,112,23,48,75,13,119,109,141,60,33,33,27,21,3,8,4,72};

LOCAL const MMI_TEXT_ID_T s_bavli_masechet_name[MMIJEWISHCAL_BAVLI_MASECHET_NUM] =
{
    TXT_MASECHET_BRACHOT,
    TXT_MASECHET_SHABAT,
    TXT_MASECHET_IRUVIN,
    TXT_MASECHET_PSACHIM,
    TXT_MASECHET_SHKALIM,
    TXT_MASECHET_YOMA,
    TXT_MASECHET_SUCA,
    TXT_MASECHET_BEYTZA,
    TXT_MASECHET_ROSH_HASHANA,
    TXT_MASECHET_TAANIT,
    TXT_MASECHET_MEGILA,
    TXT_MASECHET_MOED_KATAN,
    TXT_MASECHET_CHAGIGA,
    TXT_MASECHET_YEVAMOT,
    TXT_MASECHET_KTUBOT,
    TXT_MASECHET_NEDARIM,
    TXT_MASECHET_NAZIR,
    TXT_MASECHET_SOTA,
    TXT_MASECHET_GITIN,
    TXT_MASECHET_KIDUSHIN,
    TXT_MASECHET_BABA_KAMA,
    TXT_MASECHET_BABA_METZIA,
    TXT_MASECHET_BABA_BATRA,
    TXT_MASECHET_SANHEDRIN,
    TXT_MASECHET_MAKOT,
    TXT_MASECHET_SHVUOT,
    TXT_MASECHET_AVODA_ZARA,
    TXT_MASECHET_HORAYOT,
    TXT_MASECHET_ZEVACHIM,
    TXT_MASECHET_MENACHOT,
    TXT_MASECHET_CHULIN,
    TXT_MASECHET_BECHOROT,
    TXT_MASECHET_ARACHIN,
    TXT_MASECHET_TMURA,
    TXT_MASECHET_KRITUT,
    TXT_MASECHET_MEILA,
    TXT_MASECHET_KINIM,
    TXT_MASECHET_TAMID,
    TXT_MASECHET_MIDOT,
    TXT_MASECHET_NIDA
};

LOCAL const uint8 s_yerushalmi_pages_per_masechet[MMIJEWISHCAL_YERUSHALMI_MASECHET_NUM] = 
{68,37,34,44,31,59,26,33,28,20,13,92,65,71,22,22,42,26,26,33,34,22,19,85,72,47,40,47,54,48,44,37,34,44,9,57,37,19,13};

LOCAL const MMI_TEXT_ID_T s_yerushalmi_masechet_name[MMIJEWISHCAL_YERUSHALMI_MASECHET_NUM] =
{
    TXT_MASECHET_BRACHOT,
    TXT_MASECHET_PEAH,
    TXT_MASECHET_DMAY,
    TXT_MASECHET_KILAYIM,
    TXT_MASECHET_SHVIIT,
    TXT_MASECHET_TRUMOT,
    TXT_MASECHET_MAASROT,
    TXT_MASECHET_MAASER_SHENI,
    TXT_MASECHET_CHALA,
    TXT_MASECHET_ORLA,
    TXT_MASECHET_BIKURIM,
    TXT_MASECHET_SHABAT,
    TXT_MASECHET_IRUVIN,
    TXT_MASECHET_PSACHIM,
    TXT_MASECHET_BEYTZA,
    TXT_MASECHET_ROSH_HASHANA,
    TXT_MASECHET_YOMA,
    TXT_MASECHET_SUCA,
    TXT_MASECHET_TAANIT,
    TXT_MASECHET_SHKALIM,
    TXT_MASECHET_MEGILA,
    TXT_MASECHET_CHAGIGA,
    TXT_MASECHET_MOED_KATAN,
    TXT_MASECHET_YEVAMOT,
    TXT_MASECHET_KTUBOT,
    TXT_MASECHET_SOTA,
    TXT_MASECHET_NEDARIM,
    TXT_MASECHET_NAZIR,
    TXT_MASECHET_GITIN,
    TXT_MASECHET_KIDUSHIN,
    TXT_MASECHET_BABA_KAMA,
    TXT_MASECHET_BABA_METZIA,
    TXT_MASECHET_BABA_BATRA,
    TXT_MASECHET_SHVUOT,
    TXT_MASECHET_MAKOT,
    TXT_MASECHET_SANHEDRIN,
    TXT_MASECHET_AVODA_ZARA,
    TXT_MASECHET_HORAYOT,
    TXT_MASECHET_NIDA
};

LOCAL  GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

/**--------------------------------------------------------------------------*
 **                         GLOBAL VARIABLE DEFINITION                                *
 **--------------------------------------------------------------------------*/
MMI_JEWISH_CALENDAR_T                   g_current_date = {0};
MMI_JEWISH_SUNTIME_T                    g_current_suntimes = {0};
LOCAL   BOOLEAN                         s_is_appointed_date = FALSE;
LOCAL   MMISET_DATE_T                   s_appointed_date = {0};
LOCAL   uint16                          s_location_index = 0;
LOCAL   uint8                           s_dls_index       = 0;
LOCAL   uint8                           s_dls_status = JEWISH_DLS_OPT_AUTO;
LOCAL   MMI_JEWISHCAL_SELDATE_MODE_E    s_select_date_mode = MMIJEWISHCAL_SELDATE_MODE_DATE;

 /**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
const uint32 s_jewishcal_main_opt_item[][MMIJEWISHCAL_POP_MENU_ITEM_DIMEN] = 
{ 
    {MMIJEWISHCAL_SEL_DATE, TXT_SK_SELECT_DATE},
    {MMIJEWISHCAL_SEL_LOCATION, TXT_SK_SELECT_LOC},
    {MMIJEWISHCAL_SETTING, TXT_SK_SETTINGS},
    {MMIJEWISHCAL_ABOUT, TXT_SK_ABOUT},
    {MMIJEWISHCAL_HELP, TXT_SK_HELP},
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : jewish calendar main window
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJewishCalendarMainWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM              param
                                                  );

/*****************************************************************************/
//    Description : jewish calendar option window
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJewishCalendarOptWinMsg(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 );

/*****************************************************************************/
//    Description : jewish calendar select date window
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectDateWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//    Description : jewish calendar location setting window
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocationSettingWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               );

/*****************************************************************************/
//    Description : jewish calendar dls(Day-light-saving) setting window
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDLSSettingWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : insert dynamic menu node by label
//	Global resource dependence : 
//  Author:liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void InsertMenuNodeByLabel(
                                 MMI_CTRL_ID_T      ctrl_id, 
                                 MMI_TEXT_ID_T   	text_id,
                                 MMI_IMAGE_ID_T     img_id,
                                 uint16          	node_id,
                                 uint16          	parent_node_id,
                                 uint16          	index
                                 );

/*****************************************************************************/
//    Description : MainJewishCalendar_Init
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL void* MainJewishCalendar_Init(void);

/*****************************************************************************/
//    Description :MainJewishCalendar_SetParam
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL void MainJewishCalendar_SetParam( 
                                       MMI_JEWISH_CALENDAR_T        *calendar_ptr,
                                       MMISET_DATE_T                j_date,
                                       uint16                       hour,
                                       MMISET_DATE_T                g_date
                                       );

/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void AppendListItemForJewishCalendar(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMI_TEXT_ID_T    text_id,
                                           uint16           pos,
                                           BOOLEAN          is_update
                                           );

/*****************************************************************************/
//	Description : SetLocationUserSetting
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void SetLocationUserSetting(uint8 value);

/*****************************************************************************/
//	Description : GetLocationUserSetting
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL uint8 GetLocationUserSetting(void);

/*****************************************************************************/
//  Description : DisplayDLSSettingView
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDLSSettingView(MMI_WIN_ID_T win_id, uint8 index);

/*****************************************************************************/
//	Description : SetDLSUserSetting
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void SetDLSUserSetting(uint8 value);

/*****************************************************************************/
//	Description : GetDLSUserSetting
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL uint8 GetDLSUserSetting(void);

/*****************************************************************************/
//  Description : Calculates whether we are on Day-light-saving time or not.
//                In case settings parameter is set to summer - returns true
//                In case settings parameter is set to winter - returns false
//                In case settings parameter is set to auto - returns false for
//                locations out of israel & the DLS in israel otherwise.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLocationIsDLS(
                               MMISET_DATE_T g_date, 
                               MMI_JEWISHCAL_LOCATION_ITEM_T location
                               );

/*****************************************************************************/
//  Description : DrawJewishCalendarDetail
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DrawJewishCalendarDetail(
                                    MMI_JEWISH_CALENDAR_T *calendar_ptr, 
                                    MMI_WIN_ID_T win_id,
                                    BOOLEAN is_dls_on
                                    );

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
LOCAL void SetCommonCharacter( GUIRICHTEXT_ITEM_T* p_item, BOOLEAN is_img );

/*****************************************************************************/
// Description : DrawCalendarDetailTitle
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
LOCAL void DrawCalendarDetailTitle(
                                   MMI_JEWISH_CALENDAR_T *calendar_ptr, 
                                   MMI_WIN_ID_T win_id,
                                   BOOLEAN is_dls_on
                                   );

/*****************************************************************************/
// Description : GetItemForDayDetails
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
LOCAL void GetItemForDayDetails(
                                MMI_TEXT_ID_T text_id,
                                double j_time,
                                double timezone,
                                wchar *text
                                );

/*****************************************************************************/
//  Description : IsHoliday
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsHoliday(MMI_TEXT_ID_T *holidays);

/*****************************************************************************/
//  Description : IsHolidayEve
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsHolidayEve(MMI_TEXT_ID_T *holidays);

/*****************************************************************************/
//  Description : draw background
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DrawBackground(void);

/*****************************************************************************/
//    Description : DrawMonthCalendar
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthCalendar(MMI_JEWISH_CALENDAR_T * calendar_ptr);

/*****************************************************************************/
//  Description : DrawMonthTitle
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DrawMonthTitle(MMI_JEWISH_CALENDAR_T * calendar_ptr);

/*****************************************************************************/
// 	Description : SetCalendarTextStyle
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL __inline void SetCalendarTextStyle(
                                         GUISTR_STYLE_T *style_ptr,
                                         GUI_ALIGN_E align,
                                         GUI_FONT_T  font,
                                         GUI_COLOR_T font_color
                                         );

/*****************************************************************************/
//    Description : DrawMonthDates
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthDates(MMI_JEWISH_CALENDAR_T * calendar_ptr);

/*****************************************************************************/
//    Description : get date rect
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDateRect(
                             uint32               row,
                             uint32               col
                             );

/*****************************************************************************/
//    Description : get pos by date
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void GetPosByDate(
                        uint8                   day,
                        uint8                   weekday,
                        uint32*                 row_ptr,
                        uint32*                 col_ptr
                        );

/*****************************************************************************/
//    Description : draw one date
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawMonthDate(
                            MMI_JEWISH_CALENDAR_T   * calendar_ptr,
                            uint8                   day,
                            uint8                   month_day,
                            GUI_RECT_T*             rect_ptr,
                            BOOLEAN                 is_highlight
                            );

/*****************************************************************************/
//    Description : draw gray date of a month
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawGrayMonthDate(
                             uint8 day, 
                             GUI_RECT_T* rect_ptr
                             );

/*****************************************************************************/
//    Description : HandleJewishCalKeyUp
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleJewishCalKeyUp(
                                MMI_JEWISH_CALENDAR_T * calendar_ptr
                                );

/*****************************************************************************/
//    Description : HandleJewishCalKeyDown
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleJewishCalKeyDown(
                                  MMI_JEWISH_CALENDAR_T * calendar_ptr
                                  );

/*****************************************************************************/
//    Description : HandleJewishCalKeyLeft
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleJewishCalKeyLeft(
                                  MMI_JEWISH_CALENDAR_T * calendar_ptr
                                  );

/*****************************************************************************/
//    Description : HandleJewishCalKeyRight
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleJewishCalKeyRight(
                                   MMI_JEWISH_CALENDAR_T * calendar_ptr
                                   );

/*****************************************************************************/
//    Description : HandleJewishCalendarAboutWinMsg
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJewishCalendarAboutWinMsg(
                                                   MMI_WIN_ID_T        win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM              param
                                                   );

/*****************************************************************************/
//    Description : HandleJewishCalendarHelpWinMsg
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJewishCalendarHelpWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM              param
                                                  );

/**--------------------------------------------------------------------------*
 **                          CONSTANT DEFINITION                             *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(JEWISH_CALENDAR_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleJewishCalendarMainWinMsg),
    WIN_ID(MMIJEWISHCAL_WIN_ID),
    CREATE_RICHTEXT_CTRL(MMIJEWISHCAL_DAY_DETAIL_TEXT_CTRL_ID),
    WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(JEWISH_CALENDAR_OPTION_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleJewishCalendarOptWinMsg),
    WIN_ID(MMIJEWISHCAL_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(JEWISH_CALENDAR_SEL_DATE_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleSelectDateWinMsg),
    WIN_ID(MMIJEWISHCAL_SEL_DATE_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(JEWISH_CALENDAR_LOCATION_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleLocationSettingWinMsg),
    WIN_ID(MMIJEWISHCAL_LOCATION_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIJEWISHCAL_LOCATION_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

// Israel daylight saving time setting
WINDOW_TABLE(JEWISH_CALENDAR_DLS_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleDLSSettingWinMsg),
    WIN_ID(MMIJEWISHCAL_DLS_WIN_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIJEWISHCAL_DLS_LABEL_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(JEWISH_CALENDAR_ABOUT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleJewishCalendarAboutWinMsg),
    WIN_ID(MMIJEWISHCAL_ABOUT_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(JEWISH_CALENDAR_HELP_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleJewishCalendarHelpWinMsg),
    WIN_ID(MMIJEWISHCAL_HELP_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description :MMIAPICALENDAR_OpenJewishCalMainWin
//    Global resource dependence : 
//    Author: 
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPICALENDAR_OpenJewishCalMainWin(void)
{
    MMK_CreateWin((uint32 *)JEWISH_CALENDAR_WIN_TAB,PNULL);  
}

/*****************************************************************************/
//    Description : calendar main window
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJewishCalendarMainWinMsg(
									MMI_WIN_ID_T        win_id, 
									MMI_MESSAGE_ID_E    msg_id, 
									DPARAM              param
									)
{
    MMI_RESULT_E                    ret_val = MMI_RESULT_TRUE;
    MMI_JEWISH_CALENDAR_T           *calendar_ptr = &g_current_date;
    MMI_JEWISHCAL_LOCATION_ITEM_T   location ={0};
    MMISET_DATE_T                   jewish_date = {0};
    MMISET_DATE_T                   gregorian_date = {0};
    SCI_DATE_T                      date = {0};
    SCI_TIME_T                      time = {0};
    IGUICTRL_T                      * ctrl_handle = PNULL;  

    SCI_TRACE_LOW("[Calendar] HandleJewishCalendarMainWinMsg msg_id = 0x%x, win_id = 0x%x", msg_id, win_id);

    switch( msg_id )
    {		
	case MSG_OPEN_WINDOW:
        
        MainJewishCalendar_Init();

        GUIRICHTEXT_SetFocusMode(MMIJEWISHCAL_DAY_DETAIL_TEXT_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        
        MMK_SetAtvCtrl(win_id, MMIJEWISHCAL_DAY_DETAIL_TEXT_CTRL_ID);
		break;
		   
    case MSG_FULL_PAINT:
        if(!s_is_appointed_date)
        {
            TM_GetSysDate(&date);
        }
        else
        {
            date.year = s_appointed_date.date_year;
            date.mon = s_appointed_date.date_month;
            date.mday = s_appointed_date.date_day;
        }
        
        jewish_date = MMICALENDAR_Gregorian2Jewish(date.year, date.mon, date.mday);
        gregorian_date = MMICALENDAR_Jewish2Gregorian(jewish_date.date_year, jewish_date.date_month, jewish_date.date_day);
        
        MainJewishCalendar_SetParam(calendar_ptr, jewish_date, 0, gregorian_date);
        
        s_location_index = GetLocationUserSetting();
        location = location_settings_table[s_location_index];
        
        s_dls_status = GetLocationIsDLS(gregorian_date, location);
        g_current_suntimes = MMICALENDAR_CalcDaySunTimes(location, gregorian_date, s_dls_status);

        GUIRICHTEXT_DeleteAllItems(MMIJEWISHCAL_DAY_DETAIL_TEXT_CTRL_ID);
        DrawJewishCalendarDetail(calendar_ptr, win_id, s_dls_status);

        if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STXT_CALENDAR))
        {
            GUIWIN_UpdateStb();
        }
        break;
		
	case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
         s_is_appointed_date = FALSE;
        MMK_CloseWin(win_id);
		break;       

    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)JEWISH_CALENDAR_OPTION_WIN_TAB, PNULL);
        break;

    default:
		ret_val = MMI_RESULT_FALSE;
	    break;
    }

    return ret_val;
}

/*****************************************************************************/
//    Description : jewish calendar option window
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJewishCalendarOptWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    uint16                              i                   = 0;
    uint16                              node_id             = 0;
    BOOLEAN                             *is_weekly_ptr      = NULL;
    MMI_RESULT_E                        ret_val             = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id             = MMIJEWISHCAL_OPT_MENU_CTRL_ID;
    SCI_DATE_T                          cur_date            = {0};
    SCI_DATE_T		                    date                = {0};

    switch( msg_id )
    {		
	case MSG_OPEN_WINDOW:

        GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
        
        i = 0;
        
        InsertMenuNodeByLabel(ctrl_id, TXT_SK_SELECT_DATE, 0,MMIJEWISHCAL_SEL_DATE, 0, i++);
        InsertMenuNodeByLabel(ctrl_id, TXT_SK_SELECT_LOC, 0,MMIJEWISHCAL_SEL_LOCATION, 0, i++);
        InsertMenuNodeByLabel(ctrl_id, TXT_SK_SETTINGS,0, MMIJEWISHCAL_SETTING, 0, i++);
        InsertMenuNodeByLabel(ctrl_id, TXT_SK_ABOUT, 0,MMIJEWISHCAL_ABOUT, 0, i++);
        InsertMenuNodeByLabel(ctrl_id, TXT_SK_HELP, 0,MMIJEWISHCAL_HELP, 0, i++);
        
        GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK);

        MMK_SetAtvCtrl(win_id, ctrl_id);
		break;
    case MSG_FULL_PAINT:
        if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(TXT_SK_MENU))
        {
            GUIWIN_UpdateStb();
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch( node_id )
        {
        case MMIJEWISHCAL_SEL_DATE:
            MMK_CreateWin((uint32 *)JEWISH_CALENDAR_SEL_DATE_WIN_TAB, PNULL);
            break;

        case MMIJEWISHCAL_SEL_LOCATION:
            MMK_CreateWin((uint32 *)JEWISH_CALENDAR_LOCATION_WIN_TAB, PNULL);
            MMK_CloseWin(MMIJEWISHCAL_SEL_DATE_WIN_ID);
            break;

        case MMIJEWISHCAL_SETTING:
            MMK_CreateWin((uint32 *)JEWISH_CALENDAR_DLS_WIN_TAB, PNULL);
            MMK_CloseWin(MMIJEWISHCAL_SEL_DATE_WIN_ID);
            break;

		case MMIJEWISHCAL_ABOUT:
            MMK_CreateWin((uint32 *)JEWISH_CALENDAR_ABOUT_WIN_TAB, PNULL);
            MMK_CloseWin(MMIJEWISHCAL_SEL_DATE_WIN_ID);
			break;

		case MMIJEWISHCAL_HELP:
            MMK_CreateWin((uint32 *)JEWISH_CALENDAR_HELP_WIN_TAB, PNULL);
            MMK_CloseWin(MMIJEWISHCAL_SEL_DATE_WIN_ID);
			break;	
        default:
            break;
        }

        MMK_CloseWin(win_id);
        break;


    case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;

    default:
	    ret_val = MMI_RESULT_FALSE;
	    break;
    }

    return ret_val;
}

/*****************************************************************************/
// 	Description : insert dyna menu node by label
//	Global resource dependence : 
//  Author:liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void InsertMenuNodeByLabel(
                                 MMI_CTRL_ID_T      ctrl_id,
                                 MMI_TEXT_ID_T   	text_id,
                                 MMI_IMAGE_ID_T     img_id,
                                 uint16          	node_id,
                                 uint16          	parent_node_id,
                                 uint16          	index
                                 )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;

    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}


/*****************************************************************************/
//    Description : MainJewishCalendar_Init
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL void* MainJewishCalendar_Init(void)
{
    MMI_JEWISH_CALENDAR_T* calendar_ptr = &g_current_date;

    SCI_MEMSET(calendar_ptr, 0x00, sizeof(MMI_CALENDAR_T));

    calendar_ptr->j_year    = 2000; 
    calendar_ptr->j_month   = 1;
    calendar_ptr->j_day     = 1;
    calendar_ptr->j_hour    = 0;
    calendar_ptr->g_year    = 2000;
    calendar_ptr->g_month   = 1;
    calendar_ptr->g_day     = 1;

    return (void*)calendar_ptr;

}

/*****************************************************************************/
//    Description : MainJewishCalendar_SetParam
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL void MainJewishCalendar_SetParam( 
                                       MMI_JEWISH_CALENDAR_T        *calendar_ptr,
                                       MMISET_DATE_T                j_date,
                                       uint16                       hour,
                                       MMISET_DATE_T                g_date
                                       )
{
    calendar_ptr->j_year    = j_date.date_year; 
    calendar_ptr->j_month   = j_date.date_month;
    calendar_ptr->j_day     = j_date.date_day;
    calendar_ptr->j_hour    = hour;
    calendar_ptr->g_year    = g_date.date_year; 
    calendar_ptr->g_month   = g_date.date_month;
    calendar_ptr->g_day     = g_date.date_day;
}

/*****************************************************************************/
//    Description : jewish calendar select date window
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectDateWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E                        ret_val             = MMI_RESULT_TRUE;
    MMI_JEWISH_CALENDAR_T               *calendar_ptr = &g_current_date;
    MMI_JEWISHCAL_LOCATION_ITEM_T       location_info = {0};
    MMI_CTRL_ID_T                       ctrl_id = MMIJEWISHCAL_LOCATION_LIST_CTRL_ID;
    MMI_STRING_T                        year_str = {0};
    wchar                               temp[MMIJEWISHCAL_GEMATRIA_MAX_NUM]={0};
    
    SCI_TRACE_LOW("[Calendar] HandleSelectDateWinMsg msg_id = 0x%x", msg_id);

    switch( msg_id )
    {		
    case MSG_OPEN_WINDOW:
        //get date, month and year names
        year_str.wstr_ptr = temp;
        MMICALENDAR_Number2Gematria(year_str.wstr_ptr, calendar_ptr->j_year);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(year_str.wstr_ptr, MMIAPICOM_Wstrlen(year_str.wstr_ptr)))
        {
            GUIWIN_UpdateStb();
        }
        break;
    
    case MSG_FULL_PAINT:
        DrawMonthCalendar(calendar_ptr);
        break;

    case MSG_APP_UP:
        HandleJewishCalKeyUp(calendar_ptr);
        DrawMonthCalendar(calendar_ptr);
        break;
        
    case MSG_APP_DOWN:
        HandleJewishCalKeyDown(calendar_ptr);
        DrawMonthCalendar(calendar_ptr);
        break;

    case MSG_APP_LEFT:
        HandleJewishCalKeyLeft(calendar_ptr);
        DrawMonthCalendar(calendar_ptr);
        break;

    case MSG_APP_RIGHT:
        HandleJewishCalKeyRight(calendar_ptr);
        DrawMonthCalendar(calendar_ptr);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)JEWISH_CALENDAR_OPTION_WIN_TAB, PNULL);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        // open day detail window
        s_is_appointed_date = TRUE;
        s_appointed_date = MMICALENDAR_Jewish2Gregorian(calendar_ptr->j_year, calendar_ptr->j_month, calendar_ptr->j_day);
        MMK_CloseWin(MMIJEWISHCAL_OPTION_WIN_ID);
        MMK_CloseWin( win_id );
        break;
        
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        ret_val = MMI_RESULT_FALSE;
        break;
    }
    
    return ret_val;
}

/*****************************************************************************/
//    Description : jewish calendar location setting window
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocationSettingWinMsg(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E                        ret_val             = MMI_RESULT_TRUE;
    MMI_JEWISHCAL_LOCATION_ITEM_T       location_info = {0};
    MMI_CTRL_ID_T                       ctrl_id = MMIJEWISHCAL_LOCATION_LIST_CTRL_ID;
    uint8                               i =0;

    SCI_TRACE_LOW("[Calendar] HandleLocationSettingWinMsg msg_id = 0x%x", msg_id);

    switch( msg_id )
    {		
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, MMIJEWISHCAL_LOCATION_MAX_NUM, FALSE); //max item 14
        for(i = 0; i < LOCATION_TABLE_LEN; i++)                           
        {
            AppendListItemForJewishCalendar(ctrl_id,
                location_settings_table[i].text_id,
                i,
                FALSE
                );
        }

        s_location_index = GetLocationUserSetting();
        GUILIST_SetCurItemIndex(ctrl_id, s_location_index);
        GUILIST_SetSelectedItem(ctrl_id, s_location_index, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    
    case MSG_FULL_PAINT:
        if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(TXT_SK_SELECT_LOC))
        {
            GUIWIN_UpdateStb();
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:

        s_location_index = GUILIST_GetCurItemIndex(ctrl_id);
        SetLocationUserSetting(s_location_index);
        MMK_CloseWin( win_id );
        break;
        
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        ret_val = MMI_RESULT_FALSE;
        break;
    }
    
    return ret_val;
}

/*****************************************************************************/
//    Description : jewish calendar Israel daylight saving time(DLS) setting window
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDLSSettingWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E                        ret_val         = MMI_RESULT_TRUE;
    MMI_JEWISHCAL_LOCATION_ITEM_T       location_info   = {0};
    MMI_CTRL_ID_T                       ctrl_id         = MMIJEWISHCAL_DLS_LABEL_CTRL_ID;
    uint16                              cur_select      = 0;

    SCI_TRACE_LOW("[Calendar] HandleDLSSettingWinMsg msg_id = 0x%x", msg_id);

    switch( msg_id )
    {		
    case MSG_OPEN_WINDOW:
        s_dls_index = GetDLSUserSetting();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT:
        DisplayDLSSettingView(win_id, s_dls_index);

        if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(TXT_SETTINGS_SCR_DLS_LABEL))
        {
            GUIWIN_UpdateStb();
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        SetDLSUserSetting(s_dls_index);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_RIGHT:
        if(s_dls_index < (MMIJEWISHCAL_DLS_SETTING_MAX_NUM - 1))
        {
            s_dls_index++;
        }
        else
        {
            s_dls_index = 0;
        }

        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
    case MSG_APP_LEFT:
        if(s_dls_index < 1)
        {
            s_dls_index = MMIJEWISHCAL_DLS_SETTING_MAX_NUM - 1;
        }
        else
        {
            s_dls_index --;
        }

        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        ret_val = MMI_RESULT_FALSE;
        break;
    }
    
    return ret_val;
}

/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void AppendListItemForJewishCalendar(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMI_TEXT_ID_T    text_id,
                                           uint16           pos,
                                           BOOLEAN          is_update
                                           )
{
    GUILIST_ITEM_T          item_t    = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/*****************************************************************************/
//	Description : SetLocationUserSetting
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void SetLocationUserSetting(uint8 value)
{
    SCI_TRACE_LOW("[Calendar] SetLocationUserSetting location = %d", value);
    MMINV_WRITE(MMINV_JEWISH_CALENDAR_LOCATION_INFO, &value);
}

/*****************************************************************************/
//	Description : GetLocationUserSetting
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL uint8 GetLocationUserSetting(void)
{
    MN_RETURN_RESULT_E return_value  = MN_RETURN_FAILURE;
    uint8              location = 0;   

    MMINV_READ(MMINV_JEWISH_CALENDAR_LOCATION_INFO, &location, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        location  =  0;
        MMINV_WRITE(MMINV_JEWISH_CALENDAR_LOCATION_INFO, &location);    
    }

    SCI_TRACE_LOW("[Calendar] GetLocationUserSetting location = %d", location);
    
    return location;
}

/*****************************************************************************/
//	Description : SetDLSUserSetting
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void SetDLSUserSetting(uint8 value)
{
    SCI_TRACE_LOW("[Calendar] SetDLSUserSetting dls = %d", value);
    MMINV_WRITE(MMINV_JEWISH_CALENDAR_DLS_SETTING, &value);
}

/*****************************************************************************/
//	Description : GetLocationUserSetting
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL uint8 GetDLSUserSetting(void)
{
    MN_RETURN_RESULT_E return_value  = MN_RETURN_FAILURE;
    uint8              dls = 0;   
    
    MMINV_READ(MMINV_JEWISH_CALENDAR_DLS_SETTING, &dls, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        dls  =  0;
        MMINV_WRITE(MMINV_JEWISH_CALENDAR_DLS_SETTING, &dls);    
    }
    
    SCI_TRACE_LOW("[Calendar] GetDLSUserSetting dls = %d", dls);
    
    return dls;
}

/*****************************************************************************/
//  Description : DisplayDLSSettingButton
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDLSSettingView(MMI_WIN_ID_T win_id, uint8 index)
{
    uint16          img_width = 0;
    uint16          img_height = 0;
    GUI_RECT_T      full_rect   = MMITHEME_GetFullScreenRect();
    GUI_RECT_T      title_rect  = {0};
    GUI_RECT_T      text_rect   = JEWISHCAL_DLS_TEXT_RECT;
    GUI_RECT_T      rect = {0};
    MMI_STRING_T    title_string = {0};
    MMI_STRING_T    string = {0};
    GUISTR_STYLE_T  text_style={0};
    GUISTR_STATE_T  state =  GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;

    LCD_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);

    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_JEWISH_CALENDAR_ARROWS_ITEM, win_id);
    rect.left = MONTH_TITLE_LEFT;
    rect.top = MMITHEME_GetStatusBarHeight() + MONTH_TITLE_TOP_MARGIN;
    rect.right = rect.left + img_width;
    rect.bottom = rect.top + img_height;
    GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        win_id,
        IMAGE_JEWISH_CALENDAR_ARROWS_ITEM,
        &lcd_dev_info);
    
    MMI_GetLabelTextByLang(s_dls_option_text_id[index], &string);
    GUILABEL_SetRect(MMIJEWISHCAL_DLS_LABEL_CTRL_ID, &text_rect, FALSE);
    GUILABEL_SetFont(MMIJEWISHCAL_DLS_LABEL_CTRL_ID, CALENDAR_DATE_FONT, MMI_WHITE_COLOR);
    GUILABEL_SetText(MMIJEWISHCAL_DLS_LABEL_CTRL_ID, &string, FALSE);
}

/*****************************************************************************/
//  Description : Calculates whether we are on Day-light-saving time or not.
//                In case settings parameter is set to summer - returns true
//                In case settings parameter is set to winter - returns false
//                In case settings parameter is set to auto - returns false for
//                locations out of israel & the DLS in israel otherwise.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLocationIsDLS(
                               MMISET_DATE_T g_date, 
                               MMI_JEWISHCAL_LOCATION_ITEM_T location
                               )
{
    BOOLEAN status = FALSE;
    BOOLEAN is_on = FALSE;
    uint8 dls_status = JEWISH_DLS_OPT_AUTO;

    dls_status = GetDLSUserSetting();
    switch (dls_status)
    {
    case JEWISH_DLS_OPT_SUMMER:
        status = TRUE;
        break;
    case JEWISH_DLS_OPT_WINTER:
        status = FALSE;
        break;
    case JEWISH_DLS_OPT_AUTO:
    default:
        is_on = MMICALENDAR_isIsraelDLSOn(g_date);
        status = ((location.timezone == 2.0) && is_on);
        break;
    }

    SCI_TRACE_LOW("[Calendar] GetLocationIsDLS status = %d", status);
    return status;
}

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void SetCommonCharacter( GUIRICHTEXT_ITEM_T* p_item, BOOLEAN is_img )
{
    if(PNULL == p_item)
    {
        SCI_TRACE_LOW("[Calendar] SetCommonCharacter,param error!");
        return;
    }
    
    if(!is_img)
    {
        p_item->img_type = GUIRICHTEXT_IMAGE_NONE;
        p_item->text_type = GUIRICHTEXT_TEXT_BUF;
        p_item->text_set_font = TRUE;
        p_item->setting_font = MMI_DEFAULT_SMALL_FONT;
    }
    else
    {
        p_item->text_type = GUIRICHTEXT_TEXT_NONE;
        p_item->img_type = GUIRICHTEXT_IMAGE_RES;
        p_item->img_data.res.id = IMAGE_JEWISH_CALENDAR_CITY_ITEM_TOP;
        p_item->img_set_size = TRUE;
        p_item->img_setting_width = 240;
        p_item->img_setting_height = 1;
    }
    
}

/*****************************************************************************/
// Description : DrawCalendarDetailTitle
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void DrawCalendarDetailTitle(
                                   MMI_JEWISH_CALENDAR_T *calendar_ptr, 
                                   MMI_WIN_ID_T win_id,
                                   BOOLEAN is_dls_on
                                   )
{
    MMI_JEWISHCAL_LOCATION_ITEM_T   location = {0};
    GUI_COLOR_T     color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
    GUI_RECT_T      client_rect = MMITHEME_GetClientRect();
    GUI_RECT_T      img_rect = {0};
    GUI_RECT_T      title_rect = {0};
    MMI_STRING_T    title_string = {0};
    MMI_TEXT_ID_T   month_text_id = {0};
    MMI_STRING_T    month_str = {0};
    MMI_IMAGE_ID_T  img_id = IMAGE_JEWISH_CALENDAR_DLS_SUMMER;
    GUISTR_STYLE_T  text_style={0};
    GUISTR_STATE_T  state =  GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS; 
    uint16          img_width = 0;
    uint16          img_height = 0;
    wchar           year_str[GUIRICHTEXT_TEXT_MAX_LEN + 1]={0};
    wchar           temp[GUIRICHTEXT_TEXT_MAX_LEN + 1]={0};
    wchar           format_str[1] = {0x05D1};
    wchar           space[2] = {0x20,0};

    location = location_settings_table[s_location_index];

    title_rect.top = MMITHEME_GetStatusBarHeight() + 1;
    title_rect.bottom = title_rect.top + 50;
    title_rect.left = client_rect.left;
    title_rect.right = client_rect.right;
    
    GUI_FillRect(&lcd_dev_info, title_rect, color);
    SCI_TRACE_LOW("GUI_FillRect");

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = MMI_DEFAULT_BIG_FONT;
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.line_space = 0;
    
    title_rect.bottom -= 25;
    
    MMI_GetLabelTextByLang(location.text_id, &title_string);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&title_rect,
        (const GUI_RECT_T      *)&title_rect,
        (const MMI_STRING_T    *)&title_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    img_id = (is_dls_on == TRUE) ? IMAGE_JEWISH_CALENDAR_DLS_SUMMER : IMAGE_JEWISH_CALENDAR_DLS_WINTER;

    GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, win_id);
    img_rect.top = title_rect.top;
    img_rect.bottom = img_rect.top + img_height;
    img_rect.right = client_rect.right;
    img_rect.left = img_rect.right - img_width;
    GUIRES_DisplayImg(PNULL,
        &img_rect,
        PNULL,
        win_id,
        img_id,
        &lcd_dev_info);

    //get date, month and year names
    title_string.wstr_ptr = temp;
    MMICALENDAR_Number2Gematria(title_string.wstr_ptr, calendar_ptr->j_day);
    MMIAPICOM_Wstrncat(title_string.wstr_ptr, space, 1);

    MMIAPICOM_Wstrncat(title_string.wstr_ptr, format_str, 1);
    month_text_id = MMICALENDAR_GetJewishMonthName(calendar_ptr->j_month, calendar_ptr->j_year);
    MMI_GetLabelTextByLang(month_text_id, &month_str);
    MMIAPICOM_Wstrncat(title_string.wstr_ptr, month_str.wstr_ptr, month_str.wstr_len);
    MMIAPICOM_Wstrncat(title_string.wstr_ptr, space, 1);

    MMICALENDAR_Number2Gematria(year_str, calendar_ptr->j_year);
    MMIAPICOM_Wstrncat(title_string.wstr_ptr, year_str, MMIAPICOM_Wstrlen(year_str));
    title_string.wstr_len = MMIAPICOM_Wstrlen(title_string.wstr_ptr);

    title_rect.top += 25;
    title_rect.bottom += 25;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&title_rect,       //the fixed display area
        (const GUI_RECT_T      *)&title_rect,       
        (const MMI_STRING_T    *)&title_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    LCD_DrawHLine(&lcd_dev_info, 0, title_rect.bottom+1, client_rect.right-1, MMI_DARK_GRAY_COLOR);
}

/*****************************************************************************/
// Description : DrawCalendarDetailTitle
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void GetItemForDayDetails(
                                   MMI_TEXT_ID_T text_id,
                                   double j_time,
                                   double timezone,
                                   wchar *text
                                   )
{
    MMI_TM_T                    tm = {0};
    MMI_JEWISH_CALENDAR_DATE_T time = {0};
    MMI_STRING_T    title_string = {0};
    char   time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]  = {0};
    wchar  time_wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1]  = {0};
    wchar  space[2]={0x20,0};
    wchar  format_str[]=L": ";
    double new_time = 0;
    uint8  hours = 0;
    uint16  minutes = 0;
    uint16  seconds = 0;

    time = ConvertJulianToGregorianDate(j_time, timezone);

    MMI_GetLabelTextByLang(text_id, &title_string);
    MMIAPICOM_Wstrncpy(text, title_string.wstr_ptr,title_string.wstr_len);
    MMIAPICOM_Wstrcat (text, format_str);
    if(time.hour == 0)
    {
        MMIAPICOM_Wstrncat (text, L"-", 1);
    }
    else
    {
        hours = (int)time.hour;
        new_time = time.hour - hours;
        seconds = (uint16)(new_time * 3600);

        minutes = seconds / 60;
        seconds = seconds % 60;
        if(seconds >= 30)
        {
            ++minutes;
        }
        if(minutes == 60)
        {
            ++hours;
            minutes = 0;
        }
        
        sprintf((char*)time_str,"%02d:%02d", hours, minutes);
        MMIAPICOM_StrToWstr(time_str, time_wstr);
        MMIAPICOM_Wstrncat (text, time_wstr, MMIAPICOM_Wstrlen(time_wstr));
    }
}

/*****************************************************************************/
//  Description : IsHoliday
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsHoliday(MMI_TEXT_ID_T *holidays)
{
    uint8 i = 0;
    BOOLEAN result = FALSE; //no holiday detected ...

    //traverse holidays vector, return true if we need to add candle lighting times
    for (i = 0; i < sizeof(holidays); ++i)
    {
        if (TXT_HOLIDAY_PESACH == holidays[i]
            || TXT_HOLIDAY_SHVII_PESACH  == holidays[i]
            || TXT_HOLIDAY_SHVII_PESACH_YIZKOR == holidays[i]
            || TXT_HOLIDAY_SHAVUOT == holidays[i]
            || TXT_HOLIDAY_SHAVUOT_YIZKOR == holidays[i]
            || TXT_HOLIDAY_ROSH_SHANA_B == holidays[i]
            || TXT_HOLIDAY_KIPUR_YIZKOR == holidays[i]
            || TXT_HOLIDAY_SUKOT == holidays[i]
            || TXT_HOLIDAY_SIMCHAT_TORA == holidays[i]
            || TXT_HOLIDAY_SHMINI_ATZERET_YIZKOR == holidays[i])
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : IsHolidayEve
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsHolidayEve(MMI_TEXT_ID_T *holidays)
{
    uint8 i = 0;
    BOOLEAN result = FALSE; //no holiday detected ...
    
    //traverse holidays vector, return true if we need to add candle lighting times
    for (i = 0; i < sizeof(holidays); ++i)
    {
        if (TXT_HOLIDAY_EREV_PESACH == holidays[i]
            || TXT_HOLIDAY_EREV_SHAVUOT == holidays[i]
            || TXT_HOLIDAY_EREV_ROSH_HSANA == holidays[i]
            || TXT_HOLIDAY_EREV_KIPUR == holidays[i]
            || TXT_HOLIDAY_EREV_SUKOT == holidays[i]
            || TXT_HOLIDAY_HOSHANA_RABA == holidays[i])
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : DrawJewishCalendarDetail
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DrawJewishCalendarDetail(
                                    MMI_JEWISH_CALENDAR_T *calendar_ptr, 
                                    MMI_WIN_ID_T win_id,
                                    BOOLEAN is_dls_on
                                    )
{
    MMI_JEWISHCAL_LOCATION_ITEM_T   location = {0};
    MMI_CTRL_ID_T                   ctrl_id = MMIJEWISHCAL_DAY_DETAIL_TEXT_CTRL_ID;
    GUIRICHTEXT_ITEM_T              item_data = {0};
    GUI_RECT_T                      client_rect = MMITHEME_GetClientRect();
    GUI_RECT_T                      richtext_rect = {0};
    MMI_STRING_T                    title_string = {0};
    MMI_STRING_T                    string = {0};
    MMI_TEXT_ID_T                   holiday_text_id[MMIJEWISHCAL_HOLIDAY_MAX_NUM] = {0};
    BOOLEAN                         is_summer = FALSE;
    BOOLEAN                         is_holiday = FALSE;
    BOOLEAN                         is_holiday_eve = FALSE;
    BOOLEAN                         add_holiday_start = FALSE;
    wchar                           buff_wstr[GUIRICHTEXT_TEXT_MAX_LEN + 1]={0};
    wchar                           temp_buff_wstr[GUIRICHTEXT_TEXT_MAX_LEN +1] = {0};
    wchar                           space[2]={0x20,0};
    wchar                           format_str[]=L": ";
    char                            time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]  = {0};
    double                          julian_time = 0;
    int32                           bavli_index = 0;
    int32                           yerushalmi_index = 0;
    uint16                          index = 0;
    uint16                          buff_len = 0;
    uint8                           i = 0;
    uint8                           datestr[MMISET_DATE_STR_LEN] = {0};
    uint8                           cur_page_num = 0;
    uint8                           masechet = 0;
    uint8                           cur_index = 0;
    uint8                           weekday = 0;
    
    SCI_TRACE_LOW("DrawJewishCalendarDetail");

    location = location_settings_table[s_location_index];
    is_summer = (MMICALENDAR_GetZmanit(TRUE) > (1.0/24.0));
    bavli_index = g_current_suntimes.bavli_index;
    yerushalmi_index = g_current_suntimes.yerushalmi_index;
    weekday = MMICALENDAR_GetWeekdayOfHebrewDate(g_current_date.j_day, g_current_date.j_month, g_current_date.j_year);

    richtext_rect.left = 0;
    richtext_rect.right = 240;
    richtext_rect.top = MMITHEME_GetStatusBarHeight() + 50;
    richtext_rect.bottom = client_rect.bottom;
    GUIRICHTEXT_SetItemBorderSpace(ctrl_id, 0);
    GUIRICHTEXT_SetItemSpace(ctrl_id, 0);
    GUIRICHTEXT_SetRect(ctrl_id, richtext_rect);

    DrawCalendarDetailTitle(calendar_ptr, win_id, is_dls_on);

    // show holidays
    MMICALENDAR_GetHolidayForJewishDate(location.is_abroad, &holiday_text_id);
    if(holiday_text_id[0] != PNULL)
    {
        MMI_GetLabelTextByLang(TXT_JEWISH_DAY_DETAILS_SCREEN_HOLIDAYS_TITLE, &title_string);
        MMIAPICOM_Wstrncpy(buff_wstr, title_string.wstr_ptr,title_string.wstr_len);
        MMIAPICOM_Wstrcat (buff_wstr, format_str);
        
        MMI_GetLabelTextByLang(holiday_text_id[0], &title_string);
        MMIAPICOM_Wstrncat (buff_wstr, title_string.wstr_ptr, title_string.wstr_len);
        
        if(holiday_text_id[1] != PNULL)
        {
            MMI_GetLabelTextByLang(holiday_text_id[1], &title_string);
            MMIAPICOM_Wstrcat (buff_wstr, format_str);
            MMIAPICOM_Wstrncat (buff_wstr, title_string.wstr_ptr, title_string.wstr_len);
        }
        buff_len = MMIAPICOM_Wstrlen(buff_wstr);

        SCI_MEMSET(&item_data,0,sizeof(item_data));
        item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = buff_wstr;
        SetCommonCharacter(&item_data, FALSE);
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    }

    //show gregorian date
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));   
    MMI_GetLabelTextByLang(TXT_JEWISH_DAY_DETAILS_SCREEN_LOAZI, &title_string);
    MMIAPICOM_Wstrncpy(buff_wstr, title_string.wstr_ptr,title_string.wstr_len);
    MMIAPICOM_Wstrcat (buff_wstr, format_str);
    MMIAPISET_FormatDateStrByDateStyle(g_current_date.g_year,g_current_date.g_month,g_current_date.g_day,'/',datestr,MMISET_DATE_STR_LEN);
    MMIAPICOM_StrToWstr(datestr, temp_buff_wstr);
    MMIAPICOM_Wstrcat (buff_wstr, temp_buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);

    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // add the bavli masechet for daf yomi
    for(masechet = 0; masechet < MMIJEWISHCAL_BAVLI_MASECHET_NUM; ++masechet)
    {
        cur_page_num = s_bavli_pages_per_masechet[masechet];
        if(bavli_index <= cur_page_num)
        {
            break;
        }

        bavli_index -= cur_page_num;
    }

    if(masechet <= 38 && masechet > 35)
    {
        cur_index = masechet;
        while(cur_index > 35)
        {
            bavli_index += s_bavli_pages_per_masechet[--cur_index];
        }
    }

    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    SCI_MEMSET(temp_buff_wstr, 0x00, sizeof(temp_buff_wstr))
    MMI_GetLabelTextByLang(TXT_JEWISH_DAY_DETAILS_SCREEN_BAVLI, &title_string);
    MMIAPICOM_Wstrncpy(buff_wstr, title_string.wstr_ptr,title_string.wstr_len);
    MMIAPICOM_Wstrcat (buff_wstr, format_str);
    MMI_GetLabelTextByLang(s_bavli_masechet_name[masechet], &string);
    MMIAPICOM_Wstrncat (buff_wstr, string.wstr_ptr, string.wstr_len);
    MMIAPICOM_Wstrcat (buff_wstr, space);
    MMICALENDAR_Number2Gematria(temp_buff_wstr, bavli_index + 1);
    MMIAPICOM_Wstrcat (buff_wstr, temp_buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);

    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // Calculate the masechet & page that belongs to the day
    for(masechet = 0; masechet < MMIJEWISHCAL_YERUSHALMI_MASECHET_NUM; ++masechet)
    {
        cur_page_num = s_yerushalmi_pages_per_masechet[masechet];
        if(yerushalmi_index <= cur_page_num)
        {
            break;
        }
        
        yerushalmi_index -= cur_page_num;
    }

    SCI_MEMSET(temp_buff_wstr, 0x00, sizeof(temp_buff_wstr));
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    MMI_GetLabelTextByLang(TXT_JEWISH_DAY_DETAILS_SCREEN_JERUSHALMI, &title_string);
    MMIAPICOM_Wstrncpy(buff_wstr, title_string.wstr_ptr,title_string.wstr_len);
    MMIAPICOM_Wstrcat (buff_wstr, format_str);
    MMI_GetLabelTextByLang(s_yerushalmi_masechet_name[masechet], &string);
    MMIAPICOM_Wstrncat (buff_wstr, string.wstr_ptr, string.wstr_len);
    MMIAPICOM_Wstrcat (buff_wstr, space);
    MMICALENDAR_Number2Gematria(temp_buff_wstr, yerushalmi_index);
    MMIAPICOM_Wstrcat (buff_wstr, temp_buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //separator
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    SetCommonCharacter(&item_data, TRUE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // 90min
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.alot_90min + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_90MIN, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);

    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // 72min
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.alot_72min + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_72MIN, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // tfilin
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.tfilin + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_TFILIN, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // hanetz
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.rising + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_HANETZ, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // ks_mga
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.alot_90min + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0) + 3.0 * MMICALENDAR_GetZmanit(FALSE);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_KS_MGA, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // ks_gra
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.rising + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0) + 3.0 * MMICALENDAR_GetZmanit(TRUE);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_KS_GRA, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // tfila_mga
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.alot_90min + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0) + 4.0 * MMICALENDAR_GetZmanit(FALSE);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_TFILA_MGA, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // tfila_gra
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.rising + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0) + 4.0 * MMICALENDAR_GetZmanit(TRUE);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_TFILA_GRA, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //separator
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    SetCommonCharacter(&item_data, TRUE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // hatzot
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.transit + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_HATZOT, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // mincha_g
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.transit + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0) 
        + (is_summer ? (0.5 * MMICALENDAR_GetZmanit(TRUE)) : (1.0 / 48.0));
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_MINCHA_G, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // minch_k
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.rising + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0) + (9.5 * MMICALENDAR_GetZmanit(TRUE));
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_MINCH_K, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // mincha_p
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.rising + (g_current_suntimes.dls ? (1.0 / 24.0) : 0.0) + (10.75 * MMICALENDAR_GetZmanit(TRUE));
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_MINCHA_P, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    is_holiday = IsHoliday(&holiday_text_id);
    is_holiday_eve = IsHolidayEve(&holiday_text_id);
    
    //holiday is starting if:
    //1. this is not saturday or holiday AND
    //2. this is either friday or holiday eve
    add_holiday_start = (weekday != 6) && !is_holiday && (weekday == 5 || is_holiday_eve);

    //add the candle lighting times, if needed
    if (add_holiday_start)
    {
        //add candle lighting times
        SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
        if(location.rule_for_candle_light == 40)
        {
            julian_time = g_current_suntimes.setting_height + (is_dls_on ? (1.0 / 24.0) : 0.0) - 40.0 / 60.0 / 24.0;
            
        }
        else
        {
            julian_time = g_current_suntimes.setting + (is_dls_on ? (1.0 / 24.0) : 0.0) - location.rule_for_candle_light / 60.0 / 24.0;
        }

        GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_LIGHT_CANDLE, julian_time, location.timezone, &buff_wstr);
        buff_len = MMIAPICOM_Wstrlen(buff_wstr);

        SCI_MEMSET(&item_data,0,sizeof(item_data));
        item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = buff_wstr;
        SetCommonCharacter(&item_data, FALSE);
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    }

    //separator
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    SetCommonCharacter(&item_data, TRUE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // shkiah
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.setting + (is_dls_on ? (1.0 / 24.0) : 0.0);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_SHKIAH, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // shkiah_height
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.setting_height + (is_dls_on ? (1.0 / 24.0) : 0.0);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_SHKIAH_HEIGHT, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //add stars times
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.kochavim + (is_dls_on ? (1.0 / 24.0) : 0.0);
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_STARS, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    // stars_tam
    SCI_MEMSET(buff_wstr, 0x00, sizeof(buff_wstr));
    julian_time = g_current_suntimes.setting + (is_dls_on ? (1.0 / 24.0) : 0.0) + 72.0 / 60.0 / 24.0;
    GetItemForDayDetails(TXT_JEWISH_DAY_DETAILS_SCREEN_STARS_TAM, julian_time, location.timezone, &buff_wstr);
    buff_len = MMIAPICOM_Wstrlen(buff_wstr);
    
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    item_data.text_data.buf.len = MIN(buff_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = buff_wstr;
    SetCommonCharacter(&item_data, FALSE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    //separator
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    SetCommonCharacter(&item_data, TRUE);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

}

/*****************************************************************************/
//  Description : draw background
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DrawBackground(void)
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(MMIJEWISHCAL_SEL_DATE_WIN_ID);
    
    rect.top=client_rect.top;
    rect.bottom=client_rect.bottom;
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
}

/*****************************************************************************/
// 	Description : SetCalendarTextStyle
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL __inline void SetCalendarTextStyle(
                                         GUISTR_STYLE_T *style_ptr,
                                         GUI_ALIGN_E align,
                                         GUI_FONT_T  font,
                                         GUI_COLOR_T font_color
                                         )
{
    if(PNULL == style_ptr)
    {
        SCI_TRACE_LOW("[Calendar] SetCalendarTextStyle,param error!");
        return;
    }
    
    style_ptr->align = align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 0;
}

/*****************************************************************************/
//  Description : draw background
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DrawMonthTitle(MMI_JEWISH_CALENDAR_T * calendar_ptr)
{
    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T          month_rect   = JEWISHCAL_DLS_TEXT_RECT;
    GUI_RECT_T          img_rect = {0};
    GUI_RECT_T          rect = {0};
    wchar               wsz_monthyear[20] = {0};
    MMI_STRING_T        month_str = {0};
    uint16              month = 0;
    uint16              year = 0;
    wchar               temp[MMIJEWISHCAL_GEMATRIA_MAX_NUM]={0};
    MMI_TEXT_ID_T       text_id = {0};
    uint16              img_width = 0;
    uint16              img_height = 0;
    MMI_WIN_ID_T        win_id = MMIJEWISHCAL_SEL_DATE_WIN_ID;
    GUI_COLOR_T         txt_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
    
    if(PNULL == calendar_ptr)
    {
        SCI_TRACE_LOW("[Calendar] DrawMonthTitle,param error!");
        return;
    }
    
    text_id = MMICALENDAR_GetJewishMonthName(calendar_ptr->j_month, calendar_ptr->j_year);
    MMI_GetLabelTextByLang(text_id, &month_str);

    // display
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_JEWISH_CALENDAR_ARROWS_ITEM, win_id);
    rect.left = MONTH_TITLE_LEFT;
    rect.top = MMITHEME_GetStatusBarHeight() + MONTH_TITLE_TOP_MARGIN;
    rect.right = rect.left + img_width;
    rect.bottom = rect.top + img_height;

    if(s_select_date_mode == MMIJEWISHCAL_SELDATE_MODE_DATE)
    {
        GUIRES_DisplayImg(PNULL,
            &rect,
            PNULL,
            win_id,
            IMAGE_JEWISH_CALENDAR_ARROWS_ITEM,
            &lcd_dev_info);
    }
    else
    {
        GUIRES_DisplayImg(PNULL,
            &rect,
            PNULL,
            win_id,
            IMAGE_JEWISH_CALENDAR_ARROWS_ITEM_FOCUS,
            &lcd_dev_info);
    }

    SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, MMI_WHITE_COLOR);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&month_str,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
//    Description : get pos by date
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void GetPosByDate(
                        uint8                   day,
                        uint8                   weekday,
                        uint32*                 row_ptr,
                        uint32*                 col_ptr
                        )
{
    uint32 offset;
    
    if(PNULL == row_ptr || PNULL == col_ptr)
    {
        SCI_TRACE_LOW("[Calendar] GetPosByDate,param error!");
        return;
    }
    
    offset = ( WEEKDAY_NUM + weekday - MMIJEWISHCAL_FIRST_DAY_OF_WEEK) % WEEKDAY_NUM + day - 1;
    
    *row_ptr = offset / WEEKDAY_NUM;
    *col_ptr = offset % WEEKDAY_NUM;
}


/*****************************************************************************/
//    Description : get date rect
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDateRect(
                             uint32         row,
                             uint32         col
                             )
{
    GUI_RECT_T rect = {0};
    int16 istyle_height=0;
    int16 istyle_week_height=0;

    col = COLNUM - col - 1;

    if(row >= LINENUM || col >= COLNUM)
    {
        return rect;
    }

    rect.left   = CALENDAR_MONTH_ITEM0_LEFT_V + col * CALENDAR_BLOCK_WIDTH_V;
    rect.top    = CALENDAR_MONTH_LINE0_TOP_V + row * CALENDAR_BLOCK_HEIGHT_V;
    
    rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_V - 1;
    rect.bottom = rect.top + CALENDAR_BLOCK_HEIGHT_V  - 1;
    return rect;
}

/*****************************************************************************/
//    Description : draw one date
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawMonthDate(
                            MMI_JEWISH_CALENDAR_T   * calendar_ptr,
                            uint8                   day,
                            uint8                   month_day,
                            GUI_RECT_T*             rect_ptr,
                            BOOLEAN                 is_highlight
                            )
{
    BOOLEAN result = FALSE;
    BOOLEAN is_today = FALSE;
    SCI_DATE_T          today_t = {0};
    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    uint16              img_width = 0;
    uint16              img_height = 0;
    GUI_RECT_T          img_rect = {0};
    GUI_COLOR_T         color = MMI_WHITE_COLOR;
    MMI_STRING_T        day_string = {0};
    wchar               temp[MMIJEWISHCAL_GEMATRIA_MAX_NUM]={0};
    MMISET_DATE_T       g_date = {0};

    if(PNULL == calendar_ptr || PNULL == rect_ptr)
    {
        SCI_TRACE_LOW("[Calendar] DrawMonthDate,param error!");
        return FALSE;
    }
    
    TM_GetSysDate(&today_t);
    
    if ( day <= month_day )
    {
        day_string.wstr_ptr = temp;
        MMICALENDAR_Number2Gematria(day_string.wstr_ptr, day);
        day_string.wstr_len = MIN(MMIAPICOM_Wstrlen(day_string.wstr_ptr), 2);

        g_date = MMICALENDAR_Jewish2Gregorian(calendar_ptr->j_year, calendar_ptr->j_month, day);
        is_today = ((g_date.date_day == today_t.mday) && (g_date.date_year == today_t.year) && (g_date.date_month == today_t.mon));

        if (is_today)
        {
            color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);
        }

        // Draw month date BG
        if ( is_highlight && (s_select_date_mode == MMIJEWISHCAL_SELDATE_MODE_DATE))
        {
            
            GUIRES_DisplayImg(PNULL,
                rect_ptr,
                PNULL,
                MMIJEWISHCAL_SEL_DATE_WIN_ID,
                IMAGE_JEWISH_CALENDAR_CALITEMBG_FOCUSED,
                &lcd_dev_info);
        }
        else
        {
            // Draw month date BG
            GUIRES_DisplayImg(PNULL,
                rect_ptr,
                PNULL,
                MMIJEWISHCAL_SEL_DATE_WIN_ID,
                IMAGE_JEWISH_CALENDAR_CALITEMBG,
                &lcd_dev_info);
        }
        
        // End
        SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, color);
        // display

        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)rect_ptr,       //the fixed display area
            (const GUI_RECT_T      *)rect_ptr,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&day_string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//    Description : draw gray date of a month
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawGrayMonthDate(
                             uint8 day, 
                             GUI_RECT_T* rect_ptr)
{
    
    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUI_COLOR_T  color = MMIAPISCH_GetAlphaedColor((uint32)(255 * 0.35), MMI_WHITE_COLOR, MMI_BLACK_COLOR );
    GUI_RECT_T rect_bg={0};
    MMI_STRING_T        day_string = {0};
    wchar               temp[MMIJEWISHCAL_GEMATRIA_MAX_NUM]={0};
    
    day_string.wstr_ptr = temp;
    MMICALENDAR_Number2Gematria(day_string.wstr_ptr, day);
    day_string.wstr_len = MIN(MMIAPICOM_Wstrlen(day_string.wstr_ptr), 2);

    // Draw month date BG
    GUIRES_DisplayImg(PNULL,
        (const GUI_RECT_T      *)rect_ptr,
        PNULL,
        MMIJEWISHCAL_SEL_DATE_WIN_ID,
        IMAGE_JEWISH_CALENDAR_CALITEMBG,
        &lcd_dev_info);

    SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, color);
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)rect_ptr,       //the fixed display area
        (const GUI_RECT_T      *)rect_ptr,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&day_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
}

/*****************************************************************************/
//    Description :DrawMonthDates
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthDates(
                          MMI_JEWISH_CALENDAR_T * calendar_ptr
                          )
{
    uint32 offset1 = 0;
    uint32 offset2 = 0;
    uint32 pre_month_day = 0;
    uint32 next_month_day = 0;
    uint8 day = 0;
    uint16 colum = 0;
    uint16 row_ = 0;
    GUI_RECT_T rect      = {0};
    uint32     row       = 0;
    uint32     col       = 0;
    uint8      month_day = 0;
    uint8      i =0;
    uint8 num_of_days = 0;
    uint8 first_day_weekday = 0;
    uint32 abs_date = 0;
    uint8 pre_month = 0;
    uint16 pre_month_year = 0;
    uint8 pre_num_of_days = 0;
    uint8 next_month_day_weekday = 0;
    uint8 next_month = 0;
    uint16 next_month_year = 0;
    
    if(PNULL == calendar_ptr)
    {
        SCI_TRACE_LOW("[Calendar] DrawMonthDates,param error!");
        return;
    }
    
    //holds number of days for this month
    num_of_days = MMICALENDAR_GetLastDayOfJewishMonth(calendar_ptr->j_month, calendar_ptr->j_year);
    
    //get first day weekday
    abs_date = MMICALENDAR_AbsoluteFromJewishDate(calendar_ptr->j_year, calendar_ptr->j_month, 1);
    first_day_weekday = MMIJEWISHCAL_GetWeekday(abs_date);
    SCI_TRACE_LOW("DrawMonthDates First day of month is : %d weekday", first_day_weekday);

    //add the weekdays prior to the first day of month (from previous month)
    if (first_day_weekday > 0)
    {
        //calculate previous month
        pre_month = (calendar_ptr->j_month > 1) ? (calendar_ptr->j_month-1) : (MMICALENDAR_IsHebrewLeapYear(calendar_ptr->j_year) ? 13 : 12);
        
        //calculate previous month's year
        pre_month_year = (calendar_ptr->j_month != 7) ? calendar_ptr->j_year : calendar_ptr->j_year-1; //7 is "????"
        
        //holds number of days for this month
        pre_num_of_days = MMICALENDAR_GetLastDayOfJewishMonth(pre_month, pre_month_year);
    }

    //get last day weekday
    abs_date = MMICALENDAR_AbsoluteFromJewishDate(calendar_ptr->j_year, calendar_ptr->j_month, num_of_days);
    next_month_day_weekday = MMIJEWISHCAL_GetWeekday(abs_date);

    //add the weekdays after the last day of month(from next month)
    if(next_month_day_weekday < 6)
    {
        //calc next month
        next_month = (calendar_ptr->j_month < 12) ? (calendar_ptr->j_month+1) : (calendar_ptr->j_month == 12) 
            ? (MMICALENDAR_IsHebrewLeapYear(calendar_ptr->j_year)  ? 13 : 1): 1;

        //calc next month's year
        next_month_year = (calendar_ptr->j_month != 6) ? calendar_ptr->j_year : calendar_ptr->j_year+1;
    }

    for( day = 1; day <= num_of_days; day++)
    {
        GetPosByDate( day, first_day_weekday, &row, &col );
        
        rect = GetDateRect( row, col );
        
        DrawMonthDate(calendar_ptr, day, num_of_days, &rect, (BOOLEAN)(day == calendar_ptr->j_day));
    }
    
    offset1 = ( WEEKDAY_NUM + first_day_weekday - MMIJEWISHCAL_FIRST_DAY_OF_WEEK ) % WEEKDAY_NUM + 1 - 1;
    offset2 = ( WEEKDAY_NUM + first_day_weekday - MMIJEWISHCAL_FIRST_DAY_OF_WEEK ) % WEEKDAY_NUM + num_of_days - 1;

    pre_month_day = offset1;
    next_month_day = WEEKDAY_NUM -((offset2+1)%WEEKDAY_NUM);
    
    for(day = 1; day <= pre_month_day; day++)
    {
        uint8 dis_day = 0;
        row_ = 0;
        colum = day-1;
        rect = GetDateRect( row_, colum );
        
        dis_day = pre_num_of_days - pre_month_day + day;
        DrawGrayMonthDate(dis_day, &rect);
    }

    for( day = 1; day <= next_month_day; day++ )
    {
        row_ = (offset2+1)/WEEKDAY_NUM;

        colum = WEEKDAY_NUM - next_month_day + day-1;
        rect = GetDateRect( row_, colum);
        DrawGrayMonthDate(day, &rect);
    }
    if(row_<5)
    {
        row_+=1;
        colum=0;
        for( day = next_month_day+1; day <= next_month_day+WEEKDAY_NUM; day++ )
        {
            rect = GetDateRect( row_, colum);
            DrawGrayMonthDate(day, &rect);
            colum++;
        }
    }
}

/*****************************************************************************/
//    Description : DrawMonthCalendar
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthCalendar(MMI_JEWISH_CALENDAR_T * calendar_ptr)
{
    GUI_RECT_T      rect      = {0};
    MMI_STRING_T    year_str = {0};
    wchar           temp[MMIJEWISHCAL_GEMATRIA_MAX_NUM]={0};

    if(NULL == calendar_ptr)
    {
        return;
    }

    //get date, month and year names
    year_str.wstr_ptr = temp;
    MMICALENDAR_Number2Gematria(year_str.wstr_ptr, calendar_ptr->j_year);
    if(MMIAPICOM_StatusAreaSetViewHeaderinfo(year_str.wstr_ptr, MMIAPICOM_Wstrlen(year_str.wstr_ptr)))
    {
        GUIWIN_UpdateStb();
    }

    DrawBackground();
    DrawMonthTitle(calendar_ptr);
    DrawMonthDates(calendar_ptr);
}

/*****************************************************************************/
//    Description : HandleJewishCalKeyUp
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleJewishCalKeyUp(
                                MMI_JEWISH_CALENDAR_T * calendar_ptr
                                )
{
    uint16      year   = calendar_ptr->j_year;
    uint8       month  = calendar_ptr->j_month;
    uint8       date   = calendar_ptr->j_day;
    
    if(PNULL == calendar_ptr)
    {
        SCI_TRACE_LOW("[Calendar] HandleJewishCalKeyUp,param error!");
        return;
    }
    
    if(s_select_date_mode == MMIJEWISHCAL_SELDATE_MODE_DATE)
    {
        if (WEEKDAY_NUM < date)
        {
            date -= WEEKDAY_NUM;
            calendar_ptr->j_day = date;
        }
        else
        {
            s_select_date_mode = MMIJEWISHCAL_SELDATE_MODE_MONTH;
        }
    }
    else // MMIJEWISHCAL_SELDATE_MODE_MONTH
    {
        s_select_date_mode = MMIJEWISHCAL_SELDATE_MODE_DATE;
    }
}

/*****************************************************************************/
//    Description : HandleJewishCalKeyDown
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleJewishCalKeyDown(
                                  MMI_JEWISH_CALENDAR_T * calendar_ptr
                                  )
{
    uint16      year   = calendar_ptr->j_year;
    uint8       month  = calendar_ptr->j_month;
    uint8       date   = calendar_ptr->j_day;
    uint8       monthday = 0;
    
    if(PNULL == calendar_ptr)
    {
        SCI_TRACE_LOW("[Calendar] HandleJewishCalKeyDown,param error!");
        return;
    }
    
    //holds number of days for this month
    monthday = MMICALENDAR_GetLastDayOfJewishMonth(month, year);
    
    if(s_select_date_mode == MMIJEWISHCAL_SELDATE_MODE_DATE)
    {
        if (monthday >= (date + WEEKDAY_NUM))
        {
            date += WEEKDAY_NUM;
            calendar_ptr->j_day = date;
        }
        else
        {
            s_select_date_mode = MMIJEWISHCAL_SELDATE_MODE_MONTH;
        }
    }
    else // MMIJEWISHCAL_SELDATE_MODE_MONTH
    {
        s_select_date_mode = MMIJEWISHCAL_SELDATE_MODE_DATE;
    }
}

/*****************************************************************************/
//    Description : HandleJewishCalKeyLeft
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleJewishCalKeyLeft(
                                  MMI_JEWISH_CALENDAR_T * calendar_ptr
                                  )
{
    uint16      year   = calendar_ptr->j_year;
    uint8       month  = calendar_ptr->j_month;
    uint8       date   = calendar_ptr->j_day;
    uint8       monthday = 0;
    
    if(PNULL == calendar_ptr)
    {
        SCI_TRACE_LOW("[Calendar] HandleJewishCalKeyLeft,param error!");
        return;
    }
    
    //holds number of days for this month
    monthday = MMICALENDAR_GetLastDayOfJewishMonth(month, year);

    if(s_select_date_mode == MMIJEWISHCAL_SELDATE_MODE_DATE)
    {
        if (monthday != date)
        {
            date++;
            calendar_ptr->j_day = date;
        }
        else
        {
            s_select_date_mode = MMIJEWISHCAL_SELDATE_MODE_MONTH;
        }
    }
    else // MMIJEWISHCAL_SELDATE_MODE_MONTH
    {
        if (JEWISH_MONTH_ADAR != month)
        {
            month++;
        }
        else if (MMICOM_DATE_MAX_YEAR > year)
        {
            year++;
            month = 1;
        }
        else if (MMICOM_DATE_MAX_YEAR == year)
        {
            year = MMICOM_DATE_MIN_YEAR;
            month = 1;
        }

        calendar_ptr->j_year = year;
        calendar_ptr->j_month = month;
        calendar_ptr->j_day = 1;
    }
}

/*****************************************************************************/
//    Description : HandleJewishCalKeyRight
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleJewishCalKeyRight(
                                   MMI_JEWISH_CALENDAR_T * calendar_ptr
                                   )
{
    uint16      year   = calendar_ptr->j_year;
    uint8       month  = calendar_ptr->j_month;
    uint8       date   = calendar_ptr->j_day;
    
    if(PNULL == calendar_ptr)
    {
        SCI_TRACE_LOW("[Calendar] HandleJewishCalKeyRight,param error!");
        return;
    }

    if(s_select_date_mode == MMIJEWISHCAL_SELDATE_MODE_DATE)
    {
        if (1 != date)
        {
            date--;
            calendar_ptr->j_day = date;
        }
        else
        {
            s_select_date_mode = MMIJEWISHCAL_SELDATE_MODE_MONTH;
        }
    }
    else // MMIJEWISHCAL_SELDATE_MODE_MONTH
    {
        if (1 != month)
        {
            month--;
        }
        else if (MMICOM_DATE_MIN_YEAR < year)
        {
            year--;
            month = JEWISH_MONTH_ADAR;
        }
        else if (MMICOM_DATE_MIN_YEAR == year)
        {
            year = MMICOM_DATE_MAX_YEAR;
            month = JEWISH_MONTH_ADAR;
        }

        calendar_ptr->j_year = year;
        calendar_ptr->j_month = month;
        calendar_ptr->j_day = 1;
    }
}

/*****************************************************************************/
//    Description : HandleJewishCalendarAboutWinMsg
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJewishCalendarAboutWinMsg(
                                                   MMI_WIN_ID_T        win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM              param
                                                   )
{
    MMI_RESULT_E                        ret_val             = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id = MMIJEWISHCAL_ABOUT_TEXT_CTRL_ID;
    MMI_STRING_T                        string = {0};
    GUI_RECT_T                          full_rect   = MMITHEME_GetFullScreenRect();
    GUI_RECT_T                          text_rect = MMITHEME_GetClientRectEx(MMIJEWISHCAL_ABOUT_WIN_ID);
    GUI_FONT_T                          font = MMI_DEFAULT_TEXT_FONT;
    GUI_COLOR_T                         color = MMI_WHITE_COLOR;
    GUISTR_STYLE_T                      str_style = {0};
    GUISTR_STATE_T                      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK/*|GUISTR_STATE_SINGLE_LINE*/;

    switch( msg_id )
    {		
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_FULL_PAINT:
        if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(TXT_SK_ABOUT))
        {
            GUIWIN_UpdateStb();
        }
        LCD_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);

        str_style.align = ALIGN_HVMIDDLE;
        str_style.font  = MMI_DEFAULT_SMALL_FONT;
        str_style.font_color = MMI_WHITE_COLOR;
        str_style.line_space = LINE_SPACE;

        text_rect.left += VIEW_MARGIN;
        text_rect.right -= VIEW_MARGIN;
        
        MMI_GetLabelTextByLang(TXT_ABOUT_SCR_ABOUT_TEXT, &string);

        GUISTR_DrawTextToLCDInRect(
            &lcd_dev_info,
            &full_rect,
            &full_rect,
            &string,
            &str_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        ret_val = MMI_RESULT_FALSE;
        break;
    }
    
    return ret_val;
}

/*****************************************************************************/
//    Description : HandleJewishCalendarHelpWinMsg
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJewishCalendarHelpWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM              param
                                                  )
{
    MMI_RESULT_E                        ret_val             = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id = MMIJEWISHCAL_ABOUT_TEXT_CTRL_ID;
    MMI_STRING_T                        string = {0};
    GUI_RECT_T                          full_rect   = MMITHEME_GetFullScreenRect();
    GUI_RECT_T                          text_rect = MMITHEME_GetClientRectEx(MMIJEWISHCAL_ABOUT_WIN_ID);
    GUI_FONT_T                          font = MMI_DEFAULT_TEXT_FONT;
    GUI_COLOR_T                         color = MMI_WHITE_COLOR;
    GUISTR_STYLE_T                      str_style = {0};
    GUISTR_STATE_T                      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK/*|GUISTR_STATE_SINGLE_LINE*/;
    
    switch( msg_id )
    {		
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_FULL_PAINT:
        if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(TXT_SK_HELP))
        {
            GUIWIN_UpdateStb();
        }
        LCD_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);
        
        str_style.align = ALIGN_HVMIDDLE;
        str_style.font  = MMI_DEFAULT_SMALL_FONT;
        str_style.font_color = MMI_WHITE_COLOR;
        str_style.line_space = LINE_SPACE;
        
        text_rect.left += VIEW_MARGIN;
        text_rect.right -= VIEW_MARGIN;
        
        MMI_GetLabelTextByLang(TXT_HELP_SCR_HELP_TEXT, &string);
        
        GUISTR_DrawTextToLCDInRect(
            &lcd_dev_info,
            &full_rect,
            &full_rect,
            &string,
            &str_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        ret_val = MMI_RESULT_FALSE;
        break;
    }
    
    return ret_val;
}
#endif
