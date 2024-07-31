#ifdef WIN32
#include "std_header.h"
#endif
#include "mmicp_mainwin.h"
#include "mmicp_image.h"
#include "mmicp_text.h"
#include "mmiconnection_export.h"
#include "bt_abs.h"
#include "mmibt_func.h"
#include "mmibt_export.h"
#include "mmiphone_export.h"
#include "mmitheme_softkey.h"
#include "guires.h"
#include "ctrltext_export.h"
#include "mmipub.h"
#include "mmicom_panel.h"
#include "mmicom_statusarea.h"
#include "mmienvset_export.h"
#include "mmi_app_controlpanel_trc.h"
#include "variant_config_nv.h"
#include "mmicp_export.h"
#include "mmiset_func.h"
#include "mmicom_banner.h"
#include "nvitem.h"

LOCAL MMI_RESULT_E   HandleMainWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );
LOCAL MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );
LOCAL void MMICP_DrawBTOwnerDrawCtrl(GUIOWNDRAW_INFO_T *owner_draw_ptr);
LOCAL void MMICP_DrawDataOwnerDrawCtrl(GUIOWNDRAW_INFO_T *owner_draw_ptr);
LOCAL void MMICP_DrawSIMPOwnerDrawCtrl(GUIOWNDRAW_INFO_T *owner_draw_ptr);

LOCAL void AddSIM1Status(MMI_WIN_ID_T win_id);
LOCAL void UpdateSIM1Text();

LOCAL void AddSIM2Status(MMI_WIN_ID_T win_id);
LOCAL void UpdateSIM2Text();

LOCAL void InitCtrls(MMI_WIN_ID_T win_id);

LOCAL void AddVolumeBar();
LOCAL void HandleVolumeBar(BOOLEAN is_increase);

LOCAL void AddBrightnessBar();
LOCAL void UpdateBrightnessBar();
LOCAL void HandleBrightnessBar(BOOLEAN is_increase);
#ifdef BLUETOOTH_SUPPORT
LOCAL void AddBTRect(MMI_WIN_ID_T win_id);
LOCAL void UpdateBTRect(MMI_WIN_ID_T win_id,BOOLEAN highlight);
LOCAL void UpdateBTIcon(MMI_WIN_ID_T win_id);
LOCAL void UpdateBTText();
LOCAL void HighLightBTRect(MMI_WIN_ID_T win_id,BOOLEAN highlight);
#endif
LOCAL void AddDataRect(MMI_WIN_ID_T win_id);
LOCAL void UpdateDataRect(MMI_WIN_ID_T win_id,BOOLEAN highlight);
LOCAL void UpdateDataIcon(MMI_WIN_ID_T win_id);
LOCAL void UpdateDataText();
LOCAL void HighLightDataRect(MMI_WIN_ID_T win_id,BOOLEAN highlight);

LOCAL void UpdateSIMPreferredRect(MMI_WIN_ID_T win_id,BOOLEAN highlight);
LOCAL void HighLightSIMPreferredRect(MMI_WIN_ID_T win_id,BOOLEAN highlight);
LOCAL void UpdateSIMPreferredContent(MMI_WIN_ID_T win_id);
LOCAL void UpdateSIMPreferredTitle(MMI_WIN_ID_T win_id);
LOCAL void UpdateSIMPreferredSim(MMI_WIN_ID_T win_id);

LOCAL void UpdateIMLanguageRect(MMI_WIN_ID_T win_id,BOOLEAN highlight);
LOCAL void HighLightIMLanguageRect(MMI_WIN_ID_T win_id,BOOLEAN highlight);
LOCAL void UpdateIMLanguageContent(MMI_WIN_ID_T win_id);
LOCAL void UpdateIMLanguageTitle(MMI_WIN_ID_T win_id);
LOCAL void UpdateIMLanguage(MMI_WIN_ID_T win_id);

LOCAL void DrawMainWindow(MMI_WIN_ID_T win_id);
LOCAL void UpdateMainWindow(MMI_WIN_ID_T win_id);
LOCAL void GetCpThemeInfo(void);

LOCAL void InitCp();
LOCAL void GetTopRect();
LOCAL void GetBottomRect();

LOCAL BOOLEAN IsDataOn();
LOCAL MMI_STRING_T GetDataStateText();
#ifdef BLUETOOTH_SUPPORT
LOCAL CP_BT_STATE_E GetBTState();
LOCAL void GetBTStateText(MMI_STRING_T* bt_state_text);
#endif
LOCAL CP_SIM_STATE_E GetSIMState(MN_DUAL_SYS_E dual_sys);
LOCAL void GetSIMStateText(MMI_STRING_T* sim_state_text,MN_DUAL_SYS_E dual_sys);
LOCAL uint8 GetVolumnValue();
LOCAL uint8 GetBrightnessValue();
LOCAL void HandleMIDSK(MMI_WIN_ID_T win_id);
LOCAL BOOLEAN SetCurrentFoucs(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id);
LOCAL void SetRectBackground(MMI_WIN_ID_T win_id,GUI_RECT_T rect,GUI_COLOR_T color);
LOCAL void CPSetRingVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr);
LOCAL BOOLEAN CP_GetBTVisibleNDT();
PUBLIC MMI_RESULT_E AppHandleMsg(
                                  PWND app_ptr,
                                  uint16 msg_id,
                                  DPARAM param
                                  );
LOCAL void OpenBTWaitWin();
LOCAL uint8 GetNDTEmergencyUIStyle();
LOCAL BOOLEAN IsEmergencyCallOnly(MN_DUAL_SYS_E dual_sys);
LOCAL BOOLEAN IsCampOnOperator(MN_DUAL_SYS_E dual_sys);
LOCAL BOOLEAN GetEmptyString(MMI_STRING_T* sim_state_text);
LOCAL void MMICP_SetPreferredSim(PREFERRED_SIM_E * preferred_sim);
LOCAL void MMICP_SyncToSetting(PREFERRED_SIM_E preferred_sim);
LOCAL void MMICP_InitPreferredSim();
LOCAL MMI_RESULT_E HandlePreferredSIMSelectWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    );
LOCAL MMI_RESULT_E   HandleIMLanguageMenuWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);
LOCAL void CreateSimSelectionMenu(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id);
LOCAL void  OpenIMLanguageSelectMenu(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id);
extern MMISET_LANG_SET_T s_lang_set[MMISET_MAX_LANGUAGE + 1];
extern BOOLEAN s_lang_T9_status_info[MMISET_MAX_LANGUAGE+1][2];

typedef struct _BT_SIGNAL {
    SIGNAL_VARS
    BT_MSG_T        content;
} BT_SIGNAL;

LOCAL MMICOM_BANNER_INFO_T banner_info = {0};

LOCAL MMI_CTRL_ID_T current_ctrl_id = MMICP_NONE_CTRL_ID;

LOCAL uint16 lcd_width = 0;
LOCAL uint16 lcd_height = 0;
LOCAL MMI_CP_THEME_INFO_T cp_theme_info = {0};
static int16 volumebar_value = 0;
static int16 volumebar_pos = 0;
static int16 brightnessbar_value = 0;
static int16 brightnessbar_pos = 0;

LOCAL GUI_LCD_DEV_INFO  s_cp_bt_layer_handle = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_cp_data_layer_handle = {0,UILAYER_NULL_HANDLE};

LOCAL GUI_RECT_T SIM1_STATUS_RECT = {0};
LOCAL GUI_RECT_T SIM1_STATUS_ICON_RECT = {0};
LOCAL GUI_RECT_T SIM1_STATUS_TEXT_RECT = {0};

LOCAL GUI_RECT_T SIM2_STATUS_RECT = {0};
LOCAL GUI_RECT_T SIM2_STATUS_ICON_RECT = {0};
LOCAL GUI_RECT_T SIM2_STATUS_TEXT_RECT = {0};

LOCAL GUI_RECT_T VB_FORM_RECT = {0};
LOCAL GUI_RECT_T VOLUME_BAR_RECT = {0};
LOCAL GUI_RECT_T BRIGIHTNESS_BAR_RECT = {0};

LOCAL GUI_RECT_T BT_STATUS_RECT = {0};
LOCAL GUI_RECT_T BT_STATUS_ICON_RECT = {0};
LOCAL GUI_RECT_T BT_STATUS_TEXT_RECT = {0};

LOCAL GUI_RECT_T DATA_STATUS_RECT = {0};
LOCAL GUI_RECT_T DATA_STATUS_ICON_RECT = {0};
LOCAL GUI_RECT_T DATA_STATUS_TEXT_RECT = {0};

LOCAL GUI_RECT_T SIM_PREFERRED_RECT = {0};
LOCAL GUI_RECT_T SIM_PREFERRED_TITLE_RECT = {0};
LOCAL GUI_RECT_T SIM_PREFERRED_SIM_RECT = {0};

LOCAL GUI_RECT_T DICTIONARY_RECT = {0};
LOCAL GUI_RECT_T DICTIONARY_TITLE_RECT = {0};
LOCAL GUI_RECT_T DICTIONARY_LANGUAGE_RECT = {0};

LOCAL CP_NDT_EMERGENCY_UI_TYPE NDT_EMERGENCY_CALL_UI = CP_EMERGENCY_GENERAL;

LOCAL const MMI_IMAGE_ID_T THEME_BT_ON_ICON [MMITHEME_ACCENT_COLOR_MAX] =
{
    IMAGE_CONTROL_PANEL_BT_ON_VIOLET,
    IMAGE_CONTROL_PANEL_BT_ON_RED,
    IMAGE_CONTROL_PANEL_BT_ON_YELLOW,
    IMAGE_CONTROL_PANEL_BT_ON_GREEN,
    IMAGE_CONTROL_PANEL_BT_ON_COPPER,
    IMAGE_CONTROL_PANEL_BT_BLUE
};

LOCAL const MMI_IMAGE_ID_T THEME_DATA_ON_ICON[MMITHEME_ACCENT_COLOR_MAX] =
{
    IMAGE_CONTROL_PANEL_DATA_ON_VIOLET,
    IMAGE_CONTROL_PANEL_DATA_ON_RED,
    IMAGE_CONTROL_PANEL_DATA_ON_YELLOW,
    IMAGE_CONTROL_PANEL_DATA_ON_GREEN,
    IMAGE_CONTROL_PANEL_DATA_ON_COPPER,
    IMAGE_CONTROL_PANEL_DATA_ON_BLUE
};

WINDOW_TABLE(MMICP_MAIN_WIN_TAB) = 
{
    WIN_STATUSBAR,
    WIN_FUNC((uint32) HandleMainWinMsg ),
    WIN_ID(MMICP_MAIN_WIN_ID),
    //WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    //WIN_BACKGROUND_ID(IMAGE_CP_BG),
    //CREATE_SCROLL_CTRL(CP_WIN_SCROLL_CTRL_ID),

    //CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,CP_VB_FORM_CTRL_ID),
    CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_VOLUME_BAR, CP_VOL_PROGRESS_CTRL_ID),
    CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_BRIGHTNESS_BAR,CP_BRI_PROGRESS_CTRL_ID),
    CREATE_OWNDRAW_CTRL(CP_BT_CTRL_ID,(uint32)MMICP_DrawBTOwnerDrawCtrl),
    CREATE_OWNDRAW_CTRL(CP_DATA_CTRL_ID,(uint32)MMICP_DrawDataOwnerDrawCtrl),
    //CREATE_OWNDRAW_CTRL(CP_SIM_PREFERRED_CTRL_ID,(uint32)MMICP_DrawSIMPOwnerDrawCtrl),

    WIN_SOFTKEY(STXT_MAIN_SETTING, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE( MMICP_PREFERRED_SIM_WIN_TAB ) = 
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)HandlePreferredSIMSelectWinMsg),    
    WIN_ID(MMICP_PREFERRED_SIM_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(MMICP_IM_LANGUAGE_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleIMLanguageMenuWinMsg),
    WIN_ID(MMICP_IM_LANGUAGE_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

LOCAL WINDOW_TABLE( MMICP_BT_WAIT_WIN_TAB ) = {
    WIN_ID(MMICP_BTSTART_WAIT_WIN_ID),
    WIN_FUNC((uint32)HandlePubwinWaitingMsg),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    CREATE_SPINNER_CTRL(CP_BT_SPINNER_CTRL_ID),
    END_WIN
};

PUBLIC void MMIAPICP_AppInit(void)
{
    g_control_panel_app.ProcessMsg = AppHandleMsg;
}

PUBLIC void MMIAPICP_InitModule(void)
{
    MMICP_RegNv();
    MMICP_RegWinIdNameArr();
}

PUBLIC PREFERRED_SIM_E MMIAPICP_GetPreferredSimState(void)
{
    PREFERRED_SIM_E read_value = ALWAYS_ASK;
    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
    MMICP_InitPreferredSim();
    MMINV_READ(MMINV_CP_PREFERRED_SIM,&read_value,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //error log;
    }
    return read_value;
}

LOCAL void MMICP_SetPreferredSim(PREFERRED_SIM_E* preferred_sim)
{
    MMINV_WRITE(MMINV_CP_PREFERRED_SIM,preferred_sim);
    MMIAPICOM_StatusAreaUpdateSignalState();
    MMICP_SyncToSetting(*preferred_sim);
}

LOCAL void MMICP_InitPreferredSim()
{
    PREFERRED_SIM_E default_value = ALWAYS_ASK;
    MN_RETURN_RESULT_E   return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_CP_PREFERRED_SIM,&default_value,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_CP_PREFERRED_SIM,&default_value);
        MMICP_SyncToSetting(default_value);
    }
}

LOCAL void MMICP_SyncToSetting(PREFERRED_SIM_E preferred_sim)
{
#ifdef MMI_MULTI_SIM_SYS_DUAL
    MMISET_MAJOR_SIM_SEL_TYPE_E e_sim_sel_type;
    switch(preferred_sim)
    {
    case PREF_SIM_1:
        e_sim_sel_type = MMISET_MAJOR_SIM_SEL_SIM1;
        break;
    case PREF_SIM_2:
        e_sim_sel_type = MMISET_MAJOR_SIM_SEL_SIM2;
        break;
    case ALWAYS_ASK:
        e_sim_sel_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
        break;
    default:
        return;
    }
    MMISET_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_CALL,e_sim_sel_type);
    MMISET_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_SMS,e_sim_sel_type);
    MMISET_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_MMS,e_sim_sel_type);
#endif
}

LOCAL void InitCp()
{
    volumebar_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, CALL_RING_VOL);
    brightnessbar_value = MMIAPISET_GetMainLCDContrast();
    GetTopRect();
    GetNDTEmergencyUIStyle();
}

LOCAL void GetTopRect()
{
    SIM1_STATUS_RECT.left = 0;
    SIM1_STATUS_RECT.top = MMI_STATUSBAR_HEIGHT + 1;
    SIM1_STATUS_RECT.right = lcd_width;
    SIM1_STATUS_RECT.bottom = SIM1_STATUS_RECT.top + SIM_STATUS_HEIGH;

    //GUIRES_GetImgWidthHeight(&icon_width,&icon_heigh,IMAGE_CONTROL_PANEL_SIM1,MMICP_MAIN_WIN_ID);
    SIM1_STATUS_ICON_RECT.top = SIM1_STATUS_RECT.top + SIM_STATUS_TOP_MARGIN;
    SIM1_STATUS_ICON_RECT.right = SIM1_STATUS_RECT.right - SIM_STATUS_RIGHT_MARGIN;
    SIM1_STATUS_ICON_RECT.left = SIM1_STATUS_ICON_RECT.right - SIM_ICON_IMAGE_WIDTH;
    SIM1_STATUS_ICON_RECT.bottom = SIM1_STATUS_ICON_RECT.top + SIM_ICON_IMAGE_HEIGH;

    SIM1_STATUS_TEXT_RECT.left = SIM1_STATUS_RECT.left;
    SIM1_STATUS_TEXT_RECT.top = SIM1_STATUS_ICON_RECT.top;
    SIM1_STATUS_TEXT_RECT.right = SIM1_STATUS_ICON_RECT.left - SIM_STATUS_LEFT_MARGIN;
    SIM1_STATUS_TEXT_RECT.bottom = SIM1_STATUS_RECT.bottom;
#ifdef MMI_MULTI_SIM_SYS_DUAL
    SIM2_STATUS_RECT.left = 0;
    SIM2_STATUS_RECT.top = SIM1_STATUS_RECT.bottom + 1;
    SIM2_STATUS_RECT.right = lcd_width;
    SIM2_STATUS_RECT.bottom = SIM2_STATUS_RECT.top + SIM_STATUS_HEIGH;

    SIM2_STATUS_ICON_RECT.top = SIM2_STATUS_RECT.top + SIM_STATUS_TOP_MARGIN;
    SIM2_STATUS_ICON_RECT.right = SIM2_STATUS_RECT.right - SIM_STATUS_RIGHT_MARGIN;
    SIM2_STATUS_ICON_RECT.left = SIM2_STATUS_ICON_RECT.right - SIM_ICON_IMAGE_WIDTH;
    SIM2_STATUS_ICON_RECT.bottom = SIM2_STATUS_ICON_RECT.top + SIM_ICON_IMAGE_HEIGH;

    SIM2_STATUS_TEXT_RECT.left = SIM2_STATUS_RECT.left;
    SIM2_STATUS_TEXT_RECT.top = SIM2_STATUS_ICON_RECT.top;
    SIM2_STATUS_TEXT_RECT.right = SIM2_STATUS_ICON_RECT.left - SIM_STATUS_LEFT_MARGIN;
    SIM2_STATUS_TEXT_RECT.bottom = SIM2_STATUS_RECT.bottom;
#else
    SIM2_STATUS_RECT.left = 0;
    SIM2_STATUS_RECT.top = 0;
    SIM2_STATUS_RECT.right = 0;
    SIM2_STATUS_RECT.bottom = 0;

    SIM2_STATUS_ICON_RECT.top = 0;
    SIM2_STATUS_ICON_RECT.right = 0;
    SIM2_STATUS_ICON_RECT.left = 0;
    SIM2_STATUS_ICON_RECT.bottom = 0;

    SIM2_STATUS_TEXT_RECT.left = 0;
    SIM2_STATUS_TEXT_RECT.top = 0;
    SIM2_STATUS_TEXT_RECT.right = 0;
    SIM2_STATUS_TEXT_RECT.bottom = 0;
#endif
    VOLUME_BAR_RECT.left = 0;
    VOLUME_BAR_RECT.top = (SIM2_STATUS_TEXT_RECT.bottom!=0?SIM2_STATUS_TEXT_RECT.bottom:SIM1_STATUS_TEXT_RECT.bottom) + 1;
    VOLUME_BAR_RECT.right = lcd_width;
    VOLUME_BAR_RECT.bottom = VOLUME_BAR_RECT.top + VOLUME_BAR_HEIGH;

    BRIGIHTNESS_BAR_RECT.left = 0;
    BRIGIHTNESS_BAR_RECT.top = VOLUME_BAR_RECT.bottom + 1;
    BRIGIHTNESS_BAR_RECT.right = lcd_width;
    BRIGIHTNESS_BAR_RECT.bottom = BRIGIHTNESS_BAR_RECT.top + VOLUME_BAR_HEIGH;

    VB_FORM_RECT.left = VOLUME_BAR_RECT.left;
    VB_FORM_RECT.top = VOLUME_BAR_RECT.top;
    VB_FORM_RECT.bottom = BRIGIHTNESS_BAR_RECT.bottom;
    VB_FORM_RECT.right = BRIGIHTNESS_BAR_RECT.right;

    BT_STATUS_RECT.left = 0;//0
    BT_STATUS_RECT.top = BRIGIHTNESS_BAR_RECT.bottom + 1 + 1;//209+1 for 2 limit
    BT_STATUS_RECT.right = (lcd_width-BT_DATA_MARGIN)/2 - 1;// 117-1 for 2 limit
    BT_STATUS_RECT.bottom = BT_STATUS_RECT.top + BT_AND_DATA_HEIGH;//270

    BT_STATUS_ICON_RECT.top = BT_STATUS_RECT.top + BT_DATA_MARGIN;
    BT_STATUS_ICON_RECT.left = BT_DATA_MARGIN;
    BT_STATUS_ICON_RECT.right = BT_STATUS_ICON_RECT.left + BT_AND_DATA_ICON_WIDTH;
    BT_STATUS_ICON_RECT.bottom = BT_STATUS_ICON_RECT.top + BT_AND_DATA_ICON_HEIGH;

    BT_STATUS_TEXT_RECT.left=BT_STATUS_ICON_RECT.left;
    BT_STATUS_TEXT_RECT.top = BT_STATUS_ICON_RECT.bottom + BT_DATA_MARGIN;
    BT_STATUS_TEXT_RECT.right = BT_STATUS_RECT.right;
    BT_STATUS_TEXT_RECT.bottom = BT_STATUS_RECT.bottom;

    DATA_STATUS_RECT.left = BT_STATUS_RECT.right + 6 + 1 + 1;//124,for 2 limit
    DATA_STATUS_RECT.top = BT_STATUS_RECT.top;
    DATA_STATUS_RECT.right = lcd_width;
    DATA_STATUS_RECT.bottom = BT_STATUS_RECT.bottom;

    DATA_STATUS_ICON_RECT.top = DATA_STATUS_RECT.top + BT_DATA_MARGIN;
    DATA_STATUS_ICON_RECT.left = DATA_STATUS_RECT.left + BT_DATA_MARGIN;
    DATA_STATUS_ICON_RECT.right = DATA_STATUS_ICON_RECT.left + BT_AND_DATA_ICON_WIDTH;
    DATA_STATUS_ICON_RECT.bottom  =DATA_STATUS_ICON_RECT.top + BT_AND_DATA_ICON_HEIGH;

    DATA_STATUS_TEXT_RECT.left = DATA_STATUS_ICON_RECT.left;
    DATA_STATUS_TEXT_RECT.top = DATA_STATUS_ICON_RECT.bottom + BT_DATA_MARGIN;
    DATA_STATUS_TEXT_RECT.right = DATA_STATUS_RECT.right;
    DATA_STATUS_TEXT_RECT.bottom = DATA_STATUS_RECT.bottom;
#ifdef MMI_MULTI_SIM_SYS_DUAL
    SIM_PREFERRED_RECT.top = DATA_STATUS_TEXT_RECT.bottom + 1 +1;
    SIM_PREFERRED_RECT.bottom = SIM_PREFERRED_RECT.top + 64;
    SIM_PREFERRED_RECT.left = 0;
    SIM_PREFERRED_RECT.right = lcd_width;

    SIM_PREFERRED_TITLE_RECT.top = SIM_PREFERRED_RECT.top + 6;
    SIM_PREFERRED_TITLE_RECT.bottom = SIM_PREFERRED_TITLE_RECT.top + 28;
    SIM_PREFERRED_TITLE_RECT.left = 6;
    SIM_PREFERRED_TITLE_RECT.right= lcd_width - 6;

    SIM_PREFERRED_SIM_RECT.top = SIM_PREFERRED_TITLE_RECT.bottom;
    SIM_PREFERRED_SIM_RECT.bottom = SIM_PREFERRED_SIM_RECT.top + 24;
    SIM_PREFERRED_SIM_RECT.left = SIM_PREFERRED_TITLE_RECT.left;
    SIM_PREFERRED_SIM_RECT.right = SIM_PREFERRED_TITLE_RECT.right;

    DICTIONARY_RECT.top = 0;
    DICTIONARY_RECT.bottom = 0;
    DICTIONARY_RECT.left = 0;
    DICTIONARY_RECT.right = 0;

    DICTIONARY_TITLE_RECT.top = 0;
    DICTIONARY_TITLE_RECT.bottom = 0;
    DICTIONARY_TITLE_RECT.left = 0;
    DICTIONARY_TITLE_RECT.right = 0;

    DICTIONARY_LANGUAGE_RECT.top = 0;
    DICTIONARY_LANGUAGE_RECT.bottom = 0;
    DICTIONARY_LANGUAGE_RECT.left = 0;
    DICTIONARY_LANGUAGE_RECT.right = 0;
#else
    SIM_PREFERRED_RECT.top = 0;
    SIM_PREFERRED_RECT.bottom = 0;
    SIM_PREFERRED_RECT.left = 0;
    SIM_PREFERRED_RECT.right = 0;

    SIM_PREFERRED_TITLE_RECT.top = 0;
    SIM_PREFERRED_TITLE_RECT.bottom = 0;
    SIM_PREFERRED_TITLE_RECT.left = 0;
    SIM_PREFERRED_TITLE_RECT.right= 0;

    SIM_PREFERRED_SIM_RECT.top = 0;
    SIM_PREFERRED_SIM_RECT.bottom = 0;
    SIM_PREFERRED_SIM_RECT.left = 0;
    SIM_PREFERRED_SIM_RECT.right = 0;

    DICTIONARY_RECT.top = DATA_STATUS_TEXT_RECT.bottom + 1 +1;
    DICTIONARY_RECT.bottom = DICTIONARY_RECT.top + 64;
    DICTIONARY_RECT.left = 0;
    DICTIONARY_RECT.right = lcd_width;

    DICTIONARY_TITLE_RECT.top = DICTIONARY_RECT.top + 6;
    DICTIONARY_TITLE_RECT.bottom = DICTIONARY_TITLE_RECT.top + 28;
    DICTIONARY_TITLE_RECT.left = 6;
    DICTIONARY_TITLE_RECT.right = lcd_width - 6;

    DICTIONARY_LANGUAGE_RECT.top = DICTIONARY_TITLE_RECT.bottom;
    DICTIONARY_LANGUAGE_RECT.bottom = DICTIONARY_LANGUAGE_RECT.top + 24;
    DICTIONARY_LANGUAGE_RECT.left = DICTIONARY_TITLE_RECT.left;
    DICTIONARY_LANGUAGE_RECT.right = DICTIONARY_TITLE_RECT.right;
#endif
}

LOCAL void GetBottomRect()
{
    DICTIONARY_RECT.bottom = lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
    DICTIONARY_RECT.top = DICTIONARY_RECT.bottom - 64;
    DICTIONARY_RECT.left = 0;
    DICTIONARY_RECT.right = lcd_width;

    DICTIONARY_TITLE_RECT.top = DICTIONARY_RECT.top + 6;
    DICTIONARY_TITLE_RECT.bottom = DICTIONARY_TITLE_RECT.top + 28;
    DICTIONARY_TITLE_RECT.left = 6;
    DICTIONARY_TITLE_RECT.right = lcd_width - 6;

    DICTIONARY_LANGUAGE_RECT.top = DICTIONARY_TITLE_RECT.bottom;
    DICTIONARY_LANGUAGE_RECT.bottom = DICTIONARY_LANGUAGE_RECT.top + 24;
    DICTIONARY_LANGUAGE_RECT.left = DICTIONARY_TITLE_RECT.left;
    DICTIONARY_LANGUAGE_RECT.right = DICTIONARY_TITLE_RECT.right;
#ifdef MMI_MULTI_SIM_SYS_DUAL
    SIM_PREFERRED_RECT.bottom = DICTIONARY_RECT.top - 1 - 1;
    SIM_PREFERRED_RECT.top = SIM_PREFERRED_RECT.bottom - 64;
    SIM_PREFERRED_RECT.left = 0;
    SIM_PREFERRED_RECT.right = lcd_width;

    SIM_PREFERRED_TITLE_RECT.top = SIM_PREFERRED_RECT.top + 6;
    SIM_PREFERRED_TITLE_RECT.bottom = SIM_PREFERRED_TITLE_RECT.top + 28;
    SIM_PREFERRED_TITLE_RECT.left = 6;
    SIM_PREFERRED_TITLE_RECT.right= lcd_width - 6;

    SIM_PREFERRED_SIM_RECT.top = SIM_PREFERRED_TITLE_RECT.bottom;
    SIM_PREFERRED_SIM_RECT.bottom = SIM_PREFERRED_SIM_RECT.top + 24;
    SIM_PREFERRED_SIM_RECT.left = SIM_PREFERRED_TITLE_RECT.left;
    SIM_PREFERRED_SIM_RECT.right = SIM_PREFERRED_TITLE_RECT.right;
#else
    SIM_PREFERRED_RECT.bottom = 0;
    SIM_PREFERRED_RECT.top = 0;
    SIM_PREFERRED_RECT.left = 0;
    SIM_PREFERRED_RECT.right = 0;

    SIM_PREFERRED_TITLE_RECT.top = 0;
    SIM_PREFERRED_TITLE_RECT.bottom = 0;
    SIM_PREFERRED_TITLE_RECT.left = 0;
    SIM_PREFERRED_TITLE_RECT.right= 0;

    SIM_PREFERRED_SIM_RECT.top = 0;
    SIM_PREFERRED_SIM_RECT.bottom = 0;
    SIM_PREFERRED_SIM_RECT.left = 0;
    SIM_PREFERRED_SIM_RECT.right = 0;
#endif
    BT_STATUS_RECT.left = 0;//0
    BT_STATUS_RECT.right = (lcd_width-BT_DATA_MARGIN)/2 - 1;// 117-1 for 2 limit
    BT_STATUS_RECT.bottom = (SIM_PREFERRED_RECT.top!=0?SIM_PREFERRED_RECT.top:DICTIONARY_RECT.top) - 1 - 1;//270
    BT_STATUS_RECT.top = BT_STATUS_RECT.bottom - BT_AND_DATA_HEIGH;//209+1 for 2 limit

    BT_STATUS_ICON_RECT.top = BT_STATUS_RECT.top + BT_DATA_MARGIN;
    BT_STATUS_ICON_RECT.left = BT_DATA_MARGIN;
    BT_STATUS_ICON_RECT.right = BT_STATUS_ICON_RECT.left + BT_AND_DATA_ICON_WIDTH;
    BT_STATUS_ICON_RECT.bottom = BT_STATUS_ICON_RECT.top + BT_AND_DATA_ICON_HEIGH;

    BT_STATUS_TEXT_RECT.left=BT_STATUS_ICON_RECT.left;
    BT_STATUS_TEXT_RECT.top = BT_STATUS_ICON_RECT.bottom + BT_DATA_MARGIN;
    BT_STATUS_TEXT_RECT.right = BT_STATUS_RECT.right;
    BT_STATUS_TEXT_RECT.bottom = BT_STATUS_RECT.bottom;

    DATA_STATUS_RECT.left = BT_STATUS_RECT.right + 6 + 1 + 1;//124,for 2 limit
    DATA_STATUS_RECT.top = BT_STATUS_RECT.top;
    DATA_STATUS_RECT.right = lcd_width;
    DATA_STATUS_RECT.bottom = BT_STATUS_RECT.bottom;

    DATA_STATUS_ICON_RECT.top = DATA_STATUS_RECT.top + BT_DATA_MARGIN;
    DATA_STATUS_ICON_RECT.left = DATA_STATUS_RECT.left + BT_DATA_MARGIN;
    DATA_STATUS_ICON_RECT.right = DATA_STATUS_ICON_RECT.left + BT_AND_DATA_ICON_WIDTH;
    DATA_STATUS_ICON_RECT.bottom  =DATA_STATUS_ICON_RECT.top + BT_AND_DATA_ICON_HEIGH;

    DATA_STATUS_TEXT_RECT.left = DATA_STATUS_ICON_RECT.left;
    DATA_STATUS_TEXT_RECT.top = DATA_STATUS_ICON_RECT.bottom + BT_DATA_MARGIN;
    DATA_STATUS_TEXT_RECT.right = DATA_STATUS_RECT.right;
    DATA_STATUS_TEXT_RECT.bottom = DATA_STATUS_RECT.bottom;

    BRIGIHTNESS_BAR_RECT.left = 0;
    BRIGIHTNESS_BAR_RECT.right = lcd_width;
    BRIGIHTNESS_BAR_RECT.bottom = BT_STATUS_RECT.top - 1 - 1;
    BRIGIHTNESS_BAR_RECT.top = BRIGIHTNESS_BAR_RECT.bottom - VOLUME_BAR_HEIGH;
#ifdef MMI_MULTI_SIM_SYS_DUAL
    VOLUME_BAR_RECT.left = 0;
    VOLUME_BAR_RECT.right = 0;
    VOLUME_BAR_RECT.bottom = 0;
    VOLUME_BAR_RECT.top = 0;

    VB_FORM_RECT.left = BRIGIHTNESS_BAR_RECT.left;
    VB_FORM_RECT.top = BRIGIHTNESS_BAR_RECT.top;
    VB_FORM_RECT.bottom = BRIGIHTNESS_BAR_RECT.bottom;
    VB_FORM_RECT.right = BRIGIHTNESS_BAR_RECT.right;
#else
    VOLUME_BAR_RECT.left = 0;
    VOLUME_BAR_RECT.right = lcd_width;
    VOLUME_BAR_RECT.bottom = BRIGIHTNESS_BAR_RECT.top - 1;
    VOLUME_BAR_RECT.top = VOLUME_BAR_RECT.bottom - VOLUME_BAR_HEIGH;

    VB_FORM_RECT.left = VOLUME_BAR_RECT.left;
    VB_FORM_RECT.top = VOLUME_BAR_RECT.top;
    VB_FORM_RECT.bottom = BRIGIHTNESS_BAR_RECT.bottom;
    VB_FORM_RECT.right = BRIGIHTNESS_BAR_RECT.right;
#endif
    SIM2_STATUS_RECT.left = 0;
    SIM2_STATUS_RECT.right = 0;
    SIM2_STATUS_RECT.bottom = 0;
    SIM2_STATUS_RECT.top = 0;

    SIM2_STATUS_ICON_RECT.top = 0;
    SIM2_STATUS_ICON_RECT.right = 0;
    SIM2_STATUS_ICON_RECT.left = 0;
    SIM2_STATUS_ICON_RECT.bottom = 0;

    SIM2_STATUS_TEXT_RECT.left = 0;
    SIM2_STATUS_TEXT_RECT.top = 0;
    SIM2_STATUS_TEXT_RECT.right = 0;
    SIM2_STATUS_TEXT_RECT.bottom = 0;

    SIM1_STATUS_RECT.left = 0;
    SIM1_STATUS_RECT.right = 0;
    SIM1_STATUS_RECT.bottom = 0;
    SIM1_STATUS_RECT.top = 0;

    SIM1_STATUS_ICON_RECT.top = 0;
    SIM1_STATUS_ICON_RECT.right = 0;
    SIM1_STATUS_ICON_RECT.left = 0;
    SIM1_STATUS_ICON_RECT.bottom = 0;

    SIM1_STATUS_TEXT_RECT.left = 0;
    SIM1_STATUS_TEXT_RECT.top = 0;
    SIM1_STATUS_TEXT_RECT.right = 0;
    SIM1_STATUS_TEXT_RECT.bottom = 0;

}

LOCAL uint8 GetNDTEmergencyUIStyle()
{
    uint8 emergency_ui = 0;
    NVITEM_ERROR_E result = NVERR_NONE;
    result = EFS_NvitemRead(APP_NV_EMERGENCY_CALL_UI,sizeof(emergency_ui),&emergency_ui);
    if(result == NVERR_NONE)
    {
        if(emergency_ui == 1)
        {
            NDT_EMERGENCY_CALL_UI = CP_EMERGENCY_CHINA;
        }
        else if(emergency_ui == 2)
        {
            NDT_EMERGENCY_CALL_UI = CP_EMERGENCY_AUSTRALIA;
        }
        else if(emergency_ui == 3)
        {
            NDT_EMERGENCY_CALL_UI = CP_EMERGENCY_NEWZEALAND;
        }
    }
    else
    {
        SCI_TRACE_LOW("cpapp.c: GetNDTEmergencyUIStyle nv read result=%d",result);
    }
    SCI_TRACE_LOW("cpapp.c: GetNDTEmergencyUIStyle emergency_ui=%d",emergency_ui);
    return emergency_ui;
}

PUBLIC MMI_RESULT_E AppHandleMsg(
                                  PWND app_ptr,
                                  uint16 msg_id,
                                  DPARAM param
                                  )
{
    BT_MSG_T *msg_body = &((BT_SIGNAL*)param)->content;
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICP_APPHANDLEMSG_0,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    #ifdef BLUETOOTH_SUPPORT
    case MMI_BT_ME_ON_CNF:
        if(CP_GetBTVisibleNDT())
        {
            DoOperation(MMIBT_OPC_VISIBLE,FALSE);
        }
        else
        {
             SCI_TRACE_LOW("mmicp_mainwin.c-AppHandleMsg");
             DoOperation(MMIBT_OPC_HIDE,FALSE);
        }
        if(MMK_IsOpenWin(MMICP_BTSTART_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMICP_BTSTART_WAIT_WIN_ID);
        }
        break;
    case MMI_BT_ME_OFF_CNF:
        if(MMK_IsOpenWin(MMICP_BTSTART_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMICP_BTSTART_WAIT_WIN_ID);
        }
        break;
    case MMI_BT_VISIBILE_CNF:
        break;
    case MMI_BT_HIDDEN_CNF:
        break;
    case MMI_BT_INQ_DEV_NAME:
        break;
    case MMI_BT_INQ_COMP_CNF:
        break;
    case MMI_BT_DEV_PAIR_COMPLETE:
        break;
    default:
        break;
    #endif
    }
    return result;
}

PUBLIC BOOLEAN MMIAPICP_IsMainWinOpen()
{
    return MMK_IsOpenWin(MMICP_MAIN_WIN_ID);
}

PUBLIC void MMIAPICP_OpenMainWin(void)
{
    SCI_TRACE_LOW("cpapp.c: MMIAPICP_OpenMainWin");
    if(!MMIAPIKL_IsPhoneLocked() && !MMIAPISET_IsSecruityWinOpen())
    {
        if(MMK_IsExistWin(MMK_GetAppletHandleByWin(MMK_ConvertIdToHandle(MMICP_MAIN_WIN_ID)),MMICP_MAIN_WIN_ID))
        {
            SCI_TRACE_LOW("cpapp.c: MMIAPICP_OpenMainWin close first");
            MMK_CloseWin(MMICP_MAIN_WIN_ID);
        }
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        InitCp();
        SCI_TRACE_LOW("cpapp.c: MMIAPICP_OpenMainWin create now");
        MMK_CreateWin((uint32 *)MMICP_MAIN_WIN_TAB, NULL);
    }
}

LOCAL MMI_RESULT_E HandleMainWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    LCD_ANGLE_E    angle_ptr = {0};
    SCI_TRACE_LOW("cpapp.c: HandleMainWinMsg msg_id=%d",msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_GetWinAngle(win_id,&angle_ptr);
        if(angle_ptr != LCD_ANGLE_0)
        {
            MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
            MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
            InitCp();
            MMK_UpdateScreen();
            DrawMainWindow(win_id);
            MMK_UpdateScreen();
        }
        else
        {
            DrawMainWindow(win_id);
        }
        break;
    case MSG_GET_FOCUS:
        DrawMainWindow(win_id);
        break;
    case MSG_LOSE_FOCUS:
#ifdef STATUS_TEXT_SUPPORT
        GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, TRUE);
#endif
        break;
    case MSG_NOTIFY_GET_ACTIVE:
        break;
    case MSG_NOTIFY_LOSE_ACTIVE:
        break;
    case MSG_CLOSE_WINDOW:
        if(MMK_IsOpenWin(MMICP_BTSTART_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMICP_BTSTART_WAIT_WIN_ID);
        }
        current_ctrl_id = MMICP_NONE_CTRL_ID;
        break;
    case MSG_FULL_PAINT:
        if(MMIAPISET_IsMainWinOpen())
        {
            GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_NULL,LEFT_BUTTON,TRUE);
        }
        else
        {
            GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_MAIN_SETTING,LEFT_BUTTON,TRUE);
        }
        //UpdateMainWindow(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        HandleMIDSK(win_id);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if(!MMIAPISET_IsMainWinOpen())
        {
            MMIAPISET_EnterPhoneSettingWin();
        }
        //MMIUILAYER_Test();
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_UP:
        result = SetCurrentFoucs(win_id,MSG_APP_UP);
        break;
    case MSG_APP_DOWN:
        result = SetCurrentFoucs(win_id,MSG_APP_DOWN);
        break;
    case MSG_APP_LEFT:
        result = SetCurrentFoucs(win_id,MSG_APP_LEFT);
        break;
    case MSG_APP_RIGHT:
        result = SetCurrentFoucs(win_id,MSG_APP_RIGHT);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

LOCAL MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if 1
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMI_STRING_T            spinner_text = {0};

    //set control info
    ctrl_info.ctrl_id = CP_BT_SPINNER_CTRL_ID;
    ctrl_info.is_ctrl_id = TRUE;
    //data_info.anim_id = ANIM_PUBWIN_WAIT;
    data_info.img_id = IMAGE_CIRCULAR_LOADER;

    //set display info 
    display_info.align_style    = GUIANIM_ALIGN_NONE;
    display_info.is_play_once   = FALSE;
    display_info.is_bg_buf      = TRUE;
    display_info.bg.bg_type     = GUI_BG_COLOR;
    display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;

    GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    CTRLANIM_SetSpinnerTextById(CP_BT_SPINNER_CTRL_ID,STR_NOTE_WAITING);
}
#endif
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,CP_MAIN_BG_COLOR,0);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_KEYDOWN_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;  
}

LOCAL void MMICP_DrawBTOwnerDrawCtrl(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
#ifdef BLUETOOTH_SUPPORT
    UpdateBTRect(MMICP_MAIN_WIN_ID, current_ctrl_id == CP_BT_CTRL_ID);
#endif
}

LOCAL void MMICP_DrawDataOwnerDrawCtrl(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    UpdateDataRect(MMICP_MAIN_WIN_ID, current_ctrl_id == CP_DATA_CTRL_ID);
}

LOCAL void MMICP_DrawSIMPOwnerDrawCtrl(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    UpdateSIMPreferredRect(MMICP_MAIN_WIN_ID, current_ctrl_id == CP_SIM_PREFERRED_CTRL_ID);
}

LOCAL void AddSIM1Status(MMI_WIN_ID_T win_id)
{
    const GUI_LCD_DEV_INFO* lcd_dev_info = MMITHEME_GetDefaultLcdDev(); 
#ifdef MMI_MULTI_SIM_SYS_DUAL
    GUIRES_DisplayImg(PNULL,
                         &SIM1_STATUS_ICON_RECT,
                         PNULL,
                         win_id,
                         IMAGE_CONTROL_PANEL_SIM1,
                         lcd_dev_info);
#else
    GUIRES_DisplayImg(PNULL,
                         &SIM1_STATUS_ICON_RECT,
                         PNULL,
                         win_id,
                         IMAGE_CONTROL_PANEL_SIM,
                         lcd_dev_info);
#endif
    UpdateSIM1Text();
}

LOCAL void UpdateSIM1Text()
{
    MMI_STRING_T sim_text = {0};
    GUIFONT_REGION_STYLE_T region_style = {0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_GRADUAL_HOR|GUISTR_STATE_GRADUAL_VER;
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    sim_text.wstr_ptr = opn_spn_name;
    GetSIMStateText(&sim_text,MN_DUAL_SYS_1);
    text_style.region_ptr = &region_style;
    text_style.align = ALIGN_RIGHT; 
    text_style.font = MMI_DEFAULT_SMALL_FONT; 
    text_style.font_color = SIM_TEXT_COLOR;
    text_style.begin_alpha = text_style.end_alpha = BT_DATA_TEXT_COLOR_OPACITY;
    //CTRLTEXT_SetString(CP_SIM1_TEXT_CTRL_ID,sim_text.wstr_ptr,sim_text.wstr_len,TRUE);
    //GUISTR_DrawTextToLCDSingleLineExt(MMITHEME_GetDefaultLcdDev(),&SIM1_STATUS_TEXT_RECT,&sim_text,&text_style,text_state);
    GUISTR_DrawSingleLineGradualInRect(MMITHEME_GetDefaultLcdDev(),&SIM1_STATUS_TEXT_RECT,&sim_text,&text_style,text_state);
}

LOCAL void AddSIM2Status(MMI_WIN_ID_T win_id)
{
    const GUI_LCD_DEV_INFO* lcd_dev_info = MMITHEME_GetDefaultLcdDev(); 

    GUIRES_DisplayImg(PNULL,
                         &SIM2_STATUS_ICON_RECT,
                         PNULL,
                         win_id,
                         IMAGE_CONTROL_PANEL_SIM2,
                         lcd_dev_info);
    UpdateSIM2Text();
}

LOCAL void UpdateSIM2Text()
{
#ifdef MMI_MULTI_SIM_SYS_DUAL
    MMI_STRING_T sim_text = {0};
    GUIFONT_REGION_STYLE_T region_style = {0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_GRADUAL_HOR|GUISTR_STATE_GRADUAL_VER;
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    sim_text.wstr_ptr = opn_spn_name;
    GetSIMStateText(&sim_text,MN_DUAL_SYS_2);
    text_style.region_ptr = &region_style;
    text_style.align = ALIGN_RIGHT; 
    text_style.font = MMI_DEFAULT_SMALL_FONT; 
    text_style.font_color = SIM_TEXT_COLOR;
    text_style.begin_alpha = text_style.end_alpha = BT_DATA_TEXT_COLOR_OPACITY;
    //CTRLTEXT_SetString(CP_SIM2_TEXT_CTRL_ID,sim_text.wstr_ptr,sim_text.wstr_len,TRUE);
    //GUISTR_DrawTextToLCDSingleLineExt(MMITHEME_GetDefaultLcdDev(),&SIM2_STATUS_TEXT_RECT,&sim_text,&text_style,text_state);
    GUISTR_DrawSingleLineGradualInRect(MMITHEME_GetDefaultLcdDev(),&SIM2_STATUS_TEXT_RECT,&sim_text,&text_style,text_state);
#endif
}

LOCAL void InitCtrls(MMI_WIN_ID_T win_id)
{
    if(volumebar_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
    {
        volumebar_pos = 0;
    }
#ifdef ZDT_PRODUCT_VIBRATE    
    else if(volumebar_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_MEETING_MODE)
    {
        volumebar_pos = 1;
    }
#endif    
    else
    {
        volumebar_pos = volumebar_value + 1;
    }
    GUIPRGBOX_SetRect(CP_VOL_PROGRESS_CTRL_ID,&VOLUME_BAR_RECT);   
    GUIPRGBOX_SetParamByCtrl(CP_VOL_PROGRESS_CTRL_ID, MMISRVAUD_VOLUME_LEVEL_MAX + 2, MMISRVAUD_VOLUME_LEVEL_MAX + 2);
    GUIPRGBOX_SetPosByCtrl(FALSE,volumebar_pos,volumebar_pos,CP_VOL_PROGRESS_CTRL_ID);

    if(VOLUME_BAR_RECT.top!=0 && VOLUME_BAR_RECT.bottom!=0)
    {
        CTRLBASE_SetVisible(CP_VOL_PROGRESS_CTRL_ID,TRUE);
    }
    else
    {
        CTRLBASE_SetVisible(CP_VOL_PROGRESS_CTRL_ID,FALSE);
    }
    GUIPRGBOX_SetRect(CP_BRI_PROGRESS_CTRL_ID,&BRIGIHTNESS_BAR_RECT);
    GUIPRGBOX_SetParamByCtrl(CP_BRI_PROGRESS_CTRL_ID, MMICP_BRIGHTNESS_LEVEL_MAX, MMICP_BRIGHTNESS_LEVEL_MAX);
    if(brightnessbar_value >= 1)
    {
        brightnessbar_pos = brightnessbar_value -1;
    }
    GUIPRGBOX_SetPosByCtrl(FALSE,brightnessbar_pos,brightnessbar_pos,CP_BRI_PROGRESS_CTRL_ID);

    CTRLOWNERDRAW_SetRect(CP_BT_CTRL_ID,&BT_STATUS_RECT);
    CTRLOWNERDRAW_SetRect(CP_DATA_CTRL_ID,&DATA_STATUS_RECT);

    if(current_ctrl_id != MMICP_NONE_CTRL_ID)
    {
        MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), current_ctrl_id);
    }
    else
    {
        MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), CP_VOL_PROGRESS_CTRL_ID);
        current_ctrl_id = CP_VOL_PROGRESS_CTRL_ID;
    }
}

LOCAL void AddVolumeBar()
{
    CTRLPROGRESSBAR_SetRect(CP_VOL_PROGRESS_CTRL_ID,&VOLUME_BAR_RECT);
    CTRLPROGRESSBAR_SetParam(CP_VOL_PROGRESS_CTRL_ID,MMISRVAUD_VOLUME_LEVEL_MAX,1);
    CTRLPROGRESSBAR_SetPos(TRUE,MMISET_VOL_ZERO,1,CP_VOL_PROGRESS_CTRL_ID);
}

LOCAL void HandleVolumeBar(BOOLEAN is_increase)
{
    MMICOM_PANEL_CALLBACK_T para = {0};
    BOOLEAN should_handle = FALSE;
    para.last_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, CALL_RING_VOL);
    para.cur_value = para.last_value;
    if(is_increase)
    {
        if (volumebar_pos < MMISRVAUD_VOLUME_LEVEL_MAX + 1)
        {
            should_handle = TRUE;
            volumebar_pos++;
            if(MMIAPIENVSET_GetActiveModeId() == MMIENVSET_SILENT_MODE)
            {
                volumebar_value = 0;
            }
            else
            {
                volumebar_value++;
            }
            para.cur_value = volumebar_value;//para.last_value + 1;
        }
    }
    else
    {
        if (volumebar_pos > 0)
        {
            should_handle = TRUE;
            volumebar_pos--;
            if(volumebar_value > 0)
            {
                volumebar_value--;
            }
            para.cur_value = volumebar_value;//para.last_value - 1;
        }
    }
    if(should_handle)
    {
        GUIPRGBOX_SetPosByCtrl(FALSE,volumebar_pos,volumebar_pos,CP_VOL_PROGRESS_CTRL_ID);
        CTRLPROGRESSBAR_Update(CP_VOL_PROGRESS_CTRL_ID);

        CPSetRingVolumeCB(&para);
    }
}

LOCAL void AddBrightnessBar()
{
    CTRLPROGRESSBAR_SetRect(CP_BRI_PROGRESS_CTRL_ID,&BRIGIHTNESS_BAR_RECT);
    CTRLPROGRESSBAR_SetParam(CP_BRI_PROGRESS_CTRL_ID,100,7);
    CTRLPROGRESSBAR_SetPos(TRUE,0,1,CP_BRI_PROGRESS_CTRL_ID);
}

LOCAL void UpdateBrightnessBar()
{
    brightnessbar_value = MMIAPISET_GetMainLCDContrast();
    if(brightnessbar_value >= 1)
    {
        brightnessbar_pos = brightnessbar_value -1;
    }
    GUIPRGBOX_SetPosByCtrl(FALSE,brightnessbar_pos,brightnessbar_pos,CP_BRI_PROGRESS_CTRL_ID);
    CTRLPROGRESSBAR_Update(CP_BRI_PROGRESS_CTRL_ID);
}

LOCAL void HandleBrightnessBar(BOOLEAN is_increase)
{
    if(is_increase)
    {
        if (brightnessbar_pos < MMISET_CONTRAST_SEVEN -1)
        {
            brightnessbar_value++;
            brightnessbar_pos++;
            GUIPRGBOX_SetPosByCtrl(TRUE,brightnessbar_pos,brightnessbar_pos,CP_BRI_PROGRESS_CTRL_ID);
        }
    }
    else
    {
        if (brightnessbar_pos > 0)
        {
            brightnessbar_value--;
            brightnessbar_pos--;
            GUIPRGBOX_SetPosByCtrl(TRUE,brightnessbar_pos,brightnessbar_pos,CP_BRI_PROGRESS_CTRL_ID);
        }
    }
    CTRLPROGRESSBAR_Update(CP_BRI_PROGRESS_CTRL_ID);
    if(brightnessbar_value >= 1 && brightnessbar_value <= MMISET_CONTRAST_SEVEN)
    {
        MMIAPISET_SetCurrentContrast(brightnessbar_value);
        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,brightnessbar_value);
        MMIAPISET_SetMainLCDContrast();
    }

}


LOCAL void SetRectBackground(MMI_WIN_ID_T win_id,GUI_RECT_T rect,GUI_COLOR_T color){
    GUI_BG_T bg = {0};
    GUI_BG_DISPLAY_T bg_display = {0};
    bg.bg_type = GUI_BG_COLOR;
    bg.color = color;
    bg_display.display_rect = rect;
    bg_display.rect = rect;
    bg_display.win_handle = MMK_ConvertIdToHandle(win_id);
    GUI_DisplayBg(&bg,&bg_display,MMITHEME_GetDefaultLcdDev());
}

#ifdef BLUETOOTH_SUPPORT

LOCAL void AddBTRect(MMI_WIN_ID_T win_id)
{
    SetRectBackground(win_id,BT_STATUS_RECT,BT_DATA_RECT_DEFAULT_COLOR);
    UpdateBTRect(win_id,current_ctrl_id == CP_BT_CTRL_ID);
}

LOCAL void UpdateBTRect(MMI_WIN_ID_T win_id,BOOLEAN highlight)
{
    HighLightBTRect(win_id,highlight);
    UpdateBTIcon(win_id);
    UpdateBTText();
}

LOCAL void UpdateBTText()
{
    MMI_STRING_T text_str = {0};
    GUIFONT_REGION_STYLE_T region_style = {0};
    GUISTR_STYLE_T text_style = {0}; 
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T bt_text = {0};
    wchar headset_name_ptr[BT_DEVICE_NAME_SIZE] = {0};
    BOOLEAN bt_on = (GetBTState() != CP_BT_STATE_OFF);
    bt_text.wstr_ptr = headset_name_ptr;
    GetBTStateText(&bt_text);
    text_str.wstr_ptr = bt_text.wstr_ptr;
    //text_str.wstr_len = MMIAPICOM_Wstrlen(bt_text.wstr_ptr);
    text_str.wstr_len = bt_text.wstr_len;
    text_style.region_ptr = &region_style;
    text_style.align = ALIGN_LEFT;
    text_style.font = MMI_DEFAULT_SMALL_FONT;
    if(bt_on)
    {
        text_style.font_color = cp_theme_info.text_focus_color;
    }
    else
    {
        text_style.font_color = BT_TEXT_OFF_COLOR;
        text_style.begin_alpha = text_style.end_alpha = BT_DATA_TEXT_COLOR_OPACITY;
        text_state |= GUISTR_STATE_EFFECT|GUISTR_STATE_GRADUAL_HOR|GUISTR_STATE_GRADUAL_VER;
    }
    /*GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO *)&s_cp_bt_layer_handle,
                                  &BT_STATUS_TEXT_RECT,&BT_STATUS_TEXT_RECT,
                                  (const MMI_STRING_T *)& text_str, 
                                  &text_style, text_state,GUISTR_TEXT_DIR_AUTO);*/
    //GUISTR_DrawTextToLCDSingleLineExt(MMITHEME_GetDefaultLcdDev(),&BT_STATUS_TEXT_RECT,&text_str,&text_style,text_state);
    GUISTR_DrawSingleLineGradualInRect(MMITHEME_GetDefaultLcdDev(),&BT_STATUS_TEXT_RECT,&text_str,&text_style,text_state);
}

LOCAL void UpdateBTIcon(MMI_WIN_ID_T win_id)
{
    BOOLEAN bt_on = (GetBTState() != CP_BT_STATE_OFF);
    GUIRES_DisplayImg(PNULL,
                         &BT_STATUS_ICON_RECT,
                         PNULL,
                         win_id,
                         bt_on ? cp_theme_info.bt_on_image : IMAGE_CONTROL_PANEL_BT_OFF,
                         (const GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
}

LOCAL void HighLightBTRect(MMI_WIN_ID_T win_id,BOOLEAN highlight)
{
    GUI_BORDER_T border_ptr = {0};
    BOOLEAN bt_on = (GetBTState() != CP_BT_STATE_OFF);
    border_ptr.color = cp_theme_info.rect_focus_color;
    border_ptr.width = FOCUS_FRAME_SIZE;
    border_ptr.type = GUI_BORDER_SOLID;
    if(highlight)
    {
        current_ctrl_id = CP_BT_CTRL_ID;
        MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), CP_BT_CTRL_ID);
        if(bt_on)
        {
            SetRectBackground(win_id,BT_STATUS_RECT,BT_DATA_RECT_DEFAULT_COLOR);
            GUI_DisplayBorder(BT_STATUS_RECT,BT_STATUS_RECT,&border_ptr,MMITHEME_GetDefaultLcdDev());
        }
        else
        {
            SetRectBackground(win_id,BT_STATUS_RECT,cp_theme_info.rect_focus_color);
            //UILAYER_FillColor(&s_cp_bt_layer_handle,BT_DATA_FOCUS_COLOR);
        }
    }
    else
    {
        SetRectBackground(win_id,BT_STATUS_RECT,BT_DATA_RECT_DEFAULT_COLOR);
        //UILAYER_FillColor(&s_cp_bt_layer_handle,BT_DATA_DEFAULT_COLOR);
    }
}
#endif

LOCAL void AddDataRect(MMI_WIN_ID_T win_id)
{
    SetRectBackground(win_id,DATA_STATUS_RECT,BT_DATA_RECT_DEFAULT_COLOR);
    UpdateDataRect(win_id,current_ctrl_id == CP_DATA_CTRL_ID);
}


LOCAL void UpdateDataRect(MMI_WIN_ID_T win_id,BOOLEAN highlight)
{
    HighLightDataRect(win_id,highlight);
    UpdateDataIcon(win_id);
    UpdateDataText();
}

LOCAL void UpdateDataIcon(MMI_WIN_ID_T win_id)
{
    GUIRES_DisplayImg(PNULL,
                         &DATA_STATUS_ICON_RECT,
                         PNULL,
                         win_id,
                         IsDataOn() ? cp_theme_info.data_on_image : IMAGE_CONTROL_PANEL_DATA_OFF,
                         MMITHEME_GetDefaultLcdDev());
}

LOCAL void UpdateDataText()
{
    MMI_STRING_T text_str = {0};
    GUIFONT_REGION_STYLE_T region_style = {0};
    GUISTR_STYLE_T text_style = {0}; 
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T data_text = GetDataStateText();

    text_str.wstr_ptr = data_text.wstr_ptr;
    //text_str.wstr_len = MMIAPICOM_Wstrlen(data_text.wstr_ptr);
    text_str.wstr_len = data_text.wstr_len;
    text_style.region_ptr = &region_style;
    text_style.align = ALIGN_LEFT;
    text_style.font = MMI_DEFAULT_SMALL_FONT;
    if(IsDataOn())
    {
        text_style.font_color = cp_theme_info.text_focus_color;
    }
    else
    {
        text_style.font_color = DATA_TEXT_OFF_COLOR;
        text_style.begin_alpha = text_style.end_alpha = BT_DATA_TEXT_COLOR_OPACITY;
        text_state |= GUISTR_STATE_EFFECT|GUISTR_STATE_GRADUAL_HOR|GUISTR_STATE_GRADUAL_VER;
    }
    /*GUISTR_DrawTextToLCDInRect(&s_cp_data_layer_handle,
                                  &DATA_STATUS_TEXT_RECT,&DATA_STATUS_TEXT_RECT,
                                  (const MMI_STRING_T *)& text_str, 
                                  &text_style, text_state,GUISTR_TEXT_DIR_AUTO);*/
    //GUISTR_DrawTextToLCDSingleLineExt(MMITHEME_GetDefaultLcdDev(),&DATA_STATUS_TEXT_RECT,&text_str,&text_style,text_state);
    GUISTR_DrawSingleLineGradualInRect(MMITHEME_GetDefaultLcdDev(),&DATA_STATUS_TEXT_RECT,&text_str,&text_style,text_state);
}

LOCAL void HighLightDataRect(MMI_WIN_ID_T win_id,BOOLEAN highlight)
{
    GUI_BORDER_T border_ptr = {0};
    BOOLEAN data_on = IsDataOn();
    border_ptr.color = cp_theme_info.rect_focus_color;
    border_ptr.width = FOCUS_FRAME_SIZE;
    border_ptr.type = GUI_BORDER_SOLID;

    if(highlight)
    {
        current_ctrl_id = CP_DATA_CTRL_ID;
        MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), CP_DATA_CTRL_ID);
        if(data_on)
        {
            SetRectBackground(win_id,DATA_STATUS_RECT,BT_DATA_RECT_DEFAULT_COLOR);
            GUI_DisplayBorder(DATA_STATUS_RECT,DATA_STATUS_RECT,&border_ptr,MMITHEME_GetDefaultLcdDev());
        }
        else
        {
            SetRectBackground(win_id,DATA_STATUS_RECT,cp_theme_info.rect_focus_color);
            //UILAYER_FillColor(&s_cp_data_layer_handle,BT_DATA_FOCUS_COLOR);
        }
    }
    else
    {
        SetRectBackground(win_id,DATA_STATUS_RECT,BT_DATA_RECT_DEFAULT_COLOR);
        //UILAYER_FillColor(&s_cp_data_layer_handle,BT_DATA_DEFAULT_COLOR);
    }
}

LOCAL void AddSIMPreferredRect(MMI_WIN_ID_T win_id)
{
    SetRectBackground(win_id,SIM_PREFERRED_RECT,BT_DATA_RECT_DEFAULT_COLOR);
    UpdateSIMPreferredRect(win_id,current_ctrl_id == CP_SIM_PREFERRED_CTRL_ID);
}

LOCAL void UpdateSIMPreferredRect(MMI_WIN_ID_T win_id,BOOLEAN highlight)
{
    HighLightSIMPreferredRect(win_id,highlight);
    UpdateSIMPreferredContent(win_id);
}

LOCAL void HighLightSIMPreferredRect(MMI_WIN_ID_T win_id,BOOLEAN highlight)
{
    if(highlight)
    {
        current_ctrl_id = CP_SIM_PREFERRED_CTRL_ID;
        MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), CP_SIM_PREFERRED_CTRL_ID);
        SetRectBackground(win_id,SIM_PREFERRED_RECT,cp_theme_info.rect_focus_color);
    }
    else
    {
        SetRectBackground(win_id,SIM_PREFERRED_RECT,BT_DATA_RECT_DEFAULT_COLOR);
    }
}

LOCAL void UpdateSIMPreferredContent(MMI_WIN_ID_T win_id)
{
    UpdateSIMPreferredTitle(win_id);
    UpdateSIMPreferredSim(win_id);

}

LOCAL void UpdateSIMPreferredTitle(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T text_str = {0};
    GUIFONT_REGION_STYLE_T region_style = {0};
    GUISTR_STYLE_T text_style = {0}; 
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T data_text = {0};
    MMI_GetLabelTextByLang(STR_SETT_LIST_DUAL_SIM_EXT02,&data_text);

    text_str.wstr_ptr = data_text.wstr_ptr;
    text_str.wstr_len = data_text.wstr_len;
    text_style.region_ptr = &region_style;
    text_style.align = ALIGN_LEFT;
    text_style.font = MMI_DEFAULT_BIG_FONT;
    text_style.font_color = DATA_TEXT_OFF_COLOR;
    text_style.begin_alpha = text_style.end_alpha = BT_DATA_TEXT_COLOR_OPACITY;
    text_state |= GUISTR_STATE_EFFECT|GUISTR_STATE_GRADUAL_HOR|GUISTR_STATE_GRADUAL_VER;

    GUISTR_DrawSingleLineGradualInRect(MMITHEME_GetDefaultLcdDev(),&SIM_PREFERRED_TITLE_RECT,&text_str,&text_style,text_state);

}

LOCAL void UpdateSIMPreferredSim(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T text_str = {0};
    GUIFONT_REGION_STYLE_T region_style = {0};
    GUISTR_STYLE_T text_style = {0}; 
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T data_text = {0};
    PREFERRED_SIM_E prefer_sim = MMIAPICP_GetPreferredSimState();
    switch(prefer_sim)
    {
    case PREF_SIM_1:
        MMI_GetLabelTextByLang(STR_ILIST_SIM_ONE,&data_text);
        break;
    case PREF_SIM_2:
        MMI_GetLabelTextByLang(STR_ILIST_SIM_TWO,&data_text);
        break;
    case ALWAYS_ASK:
        MMI_GetLabelTextByLang(STR_LIST_SIM_MGR_ASK,&data_text);
        break;
    default:
        break;
    }

    text_str.wstr_ptr = data_text.wstr_ptr;
    text_str.wstr_len = data_text.wstr_len;
    text_style.region_ptr = &region_style;
    text_style.align = ALIGN_LEFT;
    text_style.font = MMI_DEFAULT_NORMAL_FONT;
    text_style.font_color = DATA_TEXT_OFF_COLOR;

    GUISTR_DrawSingleLineGradualInRect(MMITHEME_GetDefaultLcdDev(),&SIM_PREFERRED_SIM_RECT,&text_str,&text_style,text_state);
}

LOCAL MMI_RESULT_E HandlePreferredSIMSelectWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_CTRL_ID_T menu_ctrl_id = CP_SIM_SELECT_MENU_CTRL_ID;
    uint8 node_id = 0;
    PREFERRED_SIM_E prefer_sim = ALWAYS_ASK;
    MMI_STRING_T stb_str = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateSimSelectionMenu(win_id,menu_ctrl_id);
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
    case MSG_FULL_PAINT:
         break;  
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:  
        node_id = GUIMENU_GetCurNodeId(menu_ctrl_id);
        switch (node_id)
        {
        case CP_SIM_SEL_MENU_SIM1:
            prefer_sim= PREF_SIM_1;
            break;
        case CP_SIM_SEL_MENU_SIM2:
            prefer_sim= PREF_SIM_2;
            break;
        case CP_SIM_SEL_MENU_ALWAYS_ASK:
            prefer_sim= ALWAYS_ASK;
            break;
        default:
            break;
        }
        MMICP_SetPreferredSim(&prefer_sim);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        break;
    }
    return MMI_RESULT_TRUE;
}

LOCAL void CreateSimSelectionMenu(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
    uint8 index = 0;
    MMI_STRING_T name_str = {0};
    MMI_STRING_T stb_str = {0};
    GUIMENU_DYNA_ITEM_T menu_ptr = {0};
    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);

    MMI_GetLabelTextByLang(STR_ILIST_SIM_ONE,&name_str);
    menu_ptr.item_text_ptr = &name_str;
    GUIMENU_InsertNode(index++,CP_SIM_SEL_MENU_SIM1,CP_SIM_SEL_MENU_NODE_ROOT,&menu_ptr,ctrl_id);

    MMI_GetLabelTextByLang(STR_ILIST_SIM_TWO,&name_str);
    menu_ptr.item_text_ptr = &name_str;
    GUIMENU_InsertNode(index++,CP_SIM_SEL_MENU_SIM2,CP_SIM_SEL_MENU_NODE_ROOT,&menu_ptr,ctrl_id);

    MMI_GetLabelTextByLang(STR_LIST_SIM_MGR_ASK,&name_str);
    menu_ptr.item_text_ptr = &name_str;
    GUIMENU_InsertNode(index++,CP_SIM_SEL_MENU_ALWAYS_ASK,CP_SIM_SEL_MENU_NODE_ROOT,&menu_ptr,ctrl_id);
    GUIMENU_SetPopMenuCentered(ctrl_id,TRUE);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id,TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK);
    MMI_GetLabelTextByLang(STR_SETT_LIST_DUAL_SIM_EXT02, &stb_str);
    CTRLMENU_SetTitle(&stb_str,ctrl_id);
}

LOCAL void DrawMainWindow(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO* lcd_dev_info = MMITHEME_GetDefaultLcdDev(); 
    GUI_RECT_T rect = {0};
    GUI_BG_T bg = {0};

    bg.bg_type = GUI_BG_COLOR;
    bg.color = CP_MAIN_BG_COLOR;
    rect.right = lcd_width;
    rect.bottom = lcd_height;
#ifdef UI_MULTILAYER_SUPPORT	
    if (UILAYER_IsLayerActive(MMITHEME_GetDefaultLcdDev()))
    {
        UILAYER_Clear(MMITHEME_GetDefaultLcdDev());
    }
#endif
    LCD_FillArgbRect(lcd_dev_info,rect,CP_MAIN_BG_COLOR,255);
    //SetRectBackground(rect,CP_MAIN_BG_COLOR);
    GetCpThemeInfo();
    //GUIWIN_SeSoftkeytButtonTextId(win_id,STXT_MAIN_SETTING,LEFT_BUTTON,TRUE);
    //GUIWIN_SeSoftkeytButtonTextId(win_id,TXT_COMMON_COMMON_BACK,RIGHT_BUTTON,TRUE);
#ifdef STATUS_TEXT_SUPPORT
    GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
#endif
    GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,CP_MAIN_BG_COLOR,0);
    GUIWIN_SetStbBg(win_id,bg);
    InitCtrls(win_id);

    AddSIM1Status(win_id);
#ifdef MMI_MULTI_SIM_SYS_DUAL
    AddSIM2Status(win_id);
#endif
#ifdef MMI_MULTI_SIM_SYS_DUAL
    UpdateSIMPreferredRect(win_id,current_ctrl_id == CP_SIM_PREFERRED_CTRL_ID);
#endif
    UpdateBrightnessBar();
    UpdateIMLanguageRect(win_id,current_ctrl_id == CP_IM_LANGUAGE_CTRL_ID);
    //AddVolumeBar();
    //AddBrightnessBar();
    //AddBTRect(win_id);
    //AddDataRect(win_id);
}

LOCAL void GetCpThemeInfo()
{
    MMITHEME_ACCENT_COLOR_E index = MMITHEME_GetAccentId();
    cp_theme_info.bt_on_image = THEME_BT_ON_ICON[index];
    cp_theme_info.data_on_image = THEME_DATA_ON_ICON[index];
    cp_theme_info.rect_focus_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);//THEME_FOCUS_FRAME_COLOR[index];
    cp_theme_info.text_focus_color = MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_DEFAULT);//THEME_FOCUS_FRAME_COLOR[index];
}

LOCAL void UpdateMainWindow(MMI_WIN_ID_T win_id)
{
#ifdef STATUS_TEXT_SUPPORT
    GUIWIN_SetStbItemVisible(MMICOM_WIN_TEXT_INFO, FALSE);
#endif
    UpdateSIM1Text();
#ifdef MMI_MULTI_SIM_SYS_DUAL
    GUIRES_DisplayImg(PNULL,
                         &SIM1_STATUS_ICON_RECT,
                         PNULL,
                         win_id,
                         IMAGE_CONTROL_PANEL_SIM1,
                         MMITHEME_GetDefaultLcdDev());
    UpdateSIM2Text();
    GUIRES_DisplayImg(PNULL,
                         &SIM2_STATUS_ICON_RECT,
                         PNULL,
                         win_id,
                         IMAGE_CONTROL_PANEL_SIM2,
                         MMITHEME_GetDefaultLcdDev());
#else
    GUIRES_DisplayImg(PNULL,
                         &SIM1_STATUS_ICON_RECT,
                         PNULL,
                         win_id,
                         IMAGE_CONTROL_PANEL_SIM,
                         MMITHEME_GetDefaultLcdDev());
#endif
    if(VOLUME_BAR_RECT.top!=0 && VOLUME_BAR_RECT.bottom!=0)
    {
        CTRLBASE_SetVisible(CP_VOL_PROGRESS_CTRL_ID,TRUE);
        CTRLPROGRESSBAR_Update(CP_VOL_PROGRESS_CTRL_ID);
    }
    else
    {
        CTRLBASE_SetVisible(CP_VOL_PROGRESS_CTRL_ID,FALSE);
    }
    CTRLPROGRESSBAR_Update(CP_BRI_PROGRESS_CTRL_ID);
    #ifdef BLUETOOTH_SUPPORT    
    UpdateBTRect(win_id,current_ctrl_id == CP_BT_CTRL_ID);
    #endif
    UpdateDataRect(win_id,current_ctrl_id == CP_DATA_CTRL_ID);
#ifdef MMI_MULTI_SIM_SYS_DUAL
    UpdateSIMPreferredRect(win_id,current_ctrl_id == CP_SIM_PREFERRED_CTRL_ID);
#endif
    UpdateIMLanguageRect(win_id,current_ctrl_id == CP_IM_LANGUAGE_CTRL_ID);
    GUIWIN_UpdateStb();
    CTRLSOFTKEY_Update(win_id,MMK_GetWinSoftkeyCtrlId(win_id));
}

LOCAL BOOLEAN IsDataOn()
{
    return MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_ON?TRUE:FALSE;
}

LOCAL MMI_STRING_T GetDataStateText()
{
    MMI_STRING_T data_state_text = {0};
    MMI_GetLabelTextByLang(IsDataOn() ? STR_DATA_ON_EXT01 : STR_DATA_OFF_EXT01,&data_state_text);
    return data_state_text;
}

#ifdef BLUETOOTH_SUPPORT
LOCAL CP_BT_STATE_E GetBTState()
{
    BOOLEAN bt_on = MMIAPIBT_IsBtOn();
    CP_BT_STATE_E bt_state = CP_BT_STATE_OFF;
    if(bt_on)
    {
        bt_state = CP_BT_STATE_ON;
        if(MMIBT_GetIsDeviceVisible())
        {
            bt_state = CP_BT_STATE_VISIBLE;
        }
        if(MMIAPIBT_GetActiveBTHeadset() || MMIAPIBT_IsSendingOrReceiving())
        {
            bt_state = CP_BT_STATE_CONNECTED;
        }
    }
    else
    {
        bt_state = CP_BT_STATE_OFF;
    }
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICP_GETBTSTATE_0,(uint8*)"d", bt_state);
    return bt_state;
}

LOCAL void GetBTStateText(MMI_STRING_T* bt_state_text)
{
    CP_BT_STATE_E bt_state = GetBTState();
    switch(bt_state)
    {
        case CP_BT_STATE_OFF:
            MMI_GetLabelTextByLang(STR_BT_BLUETOOTH_EXT01,bt_state_text);
            break;
        case CP_BT_STATE_ON:
            MMI_GetLabelTextByLang(STR_BT_BLUETOOTH_EXT01,bt_state_text);
            break;
        case CP_BT_STATE_VISIBLE:
            MMI_GetLabelTextByLang(STR_BT_VISIBLE_EXT01,bt_state_text);
            break;
        case CP_BT_STATE_CONNECTED:
            {
                if(MMIAPIBT_GetActiveBTHeadset())
                {
                    if(MMIAPIBT_GetActiveHeadsetName(bt_state_text->wstr_ptr,&bt_state_text->wstr_len))
                    {
                    }
                    else
                    {
                        MMI_GetLabelTextByLang(STR_BT_BLUETOOTH_EXT01,bt_state_text);
                    }
                }
                else
                {
                    BT_DEVICE_INFO current_bt_device = MMIBT_GetCurBtInfo();
                    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
                    SCI_MEMCPY(bt_state_text->wstr_ptr, (const)current_bt_device.name, sizeof(uint16)*BT_DEVICE_NAME_SIZE);
                    bt_state_text->wstr_len = MMIAPICOM_Wstrlen(current_bt_device.name);
                }
            }
            break;
        default:
            MMI_GetLabelTextByLang(STR_BT_BLUETOOTH_EXT01,bt_state_text);
            break;
        }
}
#endif
LOCAL CP_SIM_STATE_E GetSIMState(MN_DUAL_SYS_E dual_sys)
{
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);
    SCI_TRACE_LOW("cpapp.c: GetSIMState sim_status= %d",sim_status);
    switch(sim_status)
    {
        case SIM_STATUS_OK:
            return CP_SIM_STATE_VALID;
        case SIM_STATUS_NO_SIM:
            return CP_SIM_STATE_NOT_FOUND;
        case SIM_STATUS_REJECTED:
        case SIM_STATUS_REGISTRATION_FAILED:
#if defined(MMI_SIM_LOCK_SUPPORT)
        case SIM_LOCKED:
#endif
        case SIM_STATUS_PIN_BLOCKED:
        case SIM_STATUS_PUK_BLOCKED:
        case SIM_STATUS_PUK2_BLOCKED:
        case SIM_NOT_INITED:
        case SIM_STATUS_NOT_SUPPORT:
        default:
            return CP_SIM_STATE_INVALID;
        }
}

LOCAL void GetSIMStateText(MMI_STRING_T* sim_state_text,MN_DUAL_SYS_E dual_sys)
{
#if 1
    MMIAPIPHONE_GetSimAndServiceString(dual_sys, sim_state_text);
    if(sim_state_text->wstr_len == 0)
    {
        GetEmptyString(sim_state_text);
    }
#else
    CP_SIM_STATE_E sim_state = GetSIMState(dual_sys);
    switch(sim_state)
    {
        case CP_SIM_STATE_VALID:
            {
                if(IsCampOnOperator(dual_sys))
                {
                    MMIAPIPHONE_GetSimAndServiceString(dual_sys, sim_state_text);
                }
                else if(IsEmergencyCallOnly(dual_sys))
                {
                    if(NDT_EMERGENCY_CALL_UI == CP_EMERGENCY_AUSTRALIA
                        ||NDT_EMERGENCY_CALL_UI == CP_EMERGENCY_CHINA)
                    {
                        MMI_GetLabelTextByLang(STXT_EMERGENCY_ONLY,sim_state_text);
                    }
                    else
                    {
                        GetEmptyString(sim_state_text);
                    }
                }
                else//means no network
                {
                    if(NDT_EMERGENCY_CALL_UI == CP_EMERGENCY_CHINA)
                    {
                        MMI_GetLabelTextByLang(STR_SIM_NO_SERVICE_EXT01,sim_state_text);
                    }
                    else
                    {
                        GetEmptyString(sim_state_text);
                    }
                }
            }
            break;
        case CP_SIM_STATE_NOT_FOUND:
            MMI_GetLabelTextByLang(STR_NSM_IND_NO_SIM,sim_state_text);
            break;
        case CP_SIM_STATE_INVALID:
            if(IsCampOnOperator(dual_sys))
            {
                if(NDT_EMERGENCY_CALL_UI == CP_EMERGENCY_AUSTRALIA
                    ||NDT_EMERGENCY_CALL_UI == CP_EMERGENCY_CHINA)
                {
                    MMI_GetLabelTextByLang(STXT_EMERGENCY_ONLY,sim_state_text);
                }
                else
                {
                    GetEmptyString(sim_state_text);
                }
            }
            else if(IsEmergencyCallOnly(dual_sys))
            {
                if(NDT_EMERGENCY_CALL_UI == CP_EMERGENCY_AUSTRALIA
                    ||NDT_EMERGENCY_CALL_UI == CP_EMERGENCY_CHINA)
                {
                    MMI_GetLabelTextByLang(STXT_EMERGENCY_ONLY,sim_state_text);
                }
                else
                {
                    GetEmptyString(sim_state_text);
                }
            }
            else//means no network
            {
                if(NDT_EMERGENCY_CALL_UI == CP_EMERGENCY_CHINA)
                {
                    MMI_GetLabelTextByLang(STR_SIM_NO_SERVICE_EXT01,sim_state_text);
                }
                else
                {
                    GetEmptyString(sim_state_text);
                }
            }
            break;
        default:
            break;
    }
#endif
}

LOCAL BOOLEAN GetEmptyString(MMI_STRING_T* sim_state_text)
{
    if(PNULL != sim_state_text)
    {
        sim_state_text->wstr_ptr = L"      ";
        sim_state_text->wstr_len = 6;
        return TRUE;
    }
    return FALSE;
}

LOCAL BOOLEAN IsCampOnOperator(MN_DUAL_SYS_E dual_sys)
{
    MN_PLMN_T cur_plmn = {0};
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
    MMIAPIPHONE_GetCurPLMN(&cur_plmn,dual_sys);
    SCI_TRACE_LOW("cpapp.c: IsCampOnOperator plmn_status=%d | cur_plmn.mnc_digit_num=%d ",plmn_status,cur_plmn.mnc_digit_num);
    if(cur_plmn.mnc_digit_num > 0
        &&(plmn_status==PLMN_NORMAL_GSM_ONLY
        ||plmn_status==PLMN_NORMAL_GSM_GPRS_BOTH))
    {
        SCI_TRACE_LOW("cpapp.c: IsCampOnOperator return TRUE");
        return TRUE;
    }
    SCI_TRACE_LOW("cpapp.c: IsCampOnOperator return FALSE");
    return FALSE;
}

//Condition for emergency call only.
LOCAL BOOLEAN IsEmergencyCallOnly(MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
    SCI_TRACE_LOW("cpapp.c: IsCampOnOperator plmn_status= %d",plmn_status);
    if(plmn_status!=PLMN_NORMAL_GSM_ONLY
           && plmn_status!=PLMN_NORMAL_GSM_GPRS_BOTH)
    {
        SCI_TRACE_LOW("cpapp.c: IsEmergencyCallOnly return TRUE");
        return TRUE;
    }
    SCI_TRACE_LOW("cpapp.c: IsEmergencyCallOnly return FALSE");
    return FALSE;
}

LOCAL uint8 GetVolumnValue()
{
    return MMIAPISET_GetMultimVolume();
}

LOCAL uint8 GetBrightnessValue()
{
    return MMIAPISET_GetMainLCDContrast();
}

LOCAL MMI_RESULT_E HandleBTAlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param) {
    MMI_RESULT_E         result = MMI_RESULT_TRUE;
    switch(msg_id) 
    {
        case MSG_APP_WEB:
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_APP_OK:
        case MSG_CTL_OK:
            break;
        default:
            result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }
    return result;
}

LOCAL void HandleMIDSK(MMI_WIN_ID_T win_id)
{
    switch(current_ctrl_id)
    {
        case MMICP_NONE_CTRL_ID:
            break;
        case CP_VOL_PROGRESS_CTRL_ID:
            break;
        case CP_BRI_PROGRESS_CTRL_ID:
            break;
        case CP_BT_CTRL_ID:
            {
                if(MMIAPISET_GetFlyMode())
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_FLIGHT_MODE,PNULL,PNULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleBTAlertWinMsg);
                }
                else
                {
                    BOOLEAN bt_on = MMIAPIBT_IsBtOn();
                    //MMIBT_OpenBluetooth(bt_on?FALSE:TRUE,TRUE);
                    MMIAPIBT_OpenBlueTooth(bt_on ? FALSE : TRUE);
                    OpenBTWaitWin();
                }
            }
            break;
        case CP_DATA_CTRL_ID:
            MMIAPICONNECTION_SetGPRSSwitchStatus(IsDataOn() ? MMICONNECTION_SETTING_GPRS_SWITCH_OFF : MMICONNECTION_SETTING_GPRS_SWITCH_ON);
            UpdateDataRect(win_id,TRUE);
            break;
        case CP_SIM_PREFERRED_CTRL_ID:
            MMK_CreateWin((uint32*)MMICP_PREFERRED_SIM_WIN_TAB, PNULL); 
            break;
        case CP_IM_LANGUAGE_CTRL_ID:
            MMK_CreateWin((uint32*)MMICP_IM_LANGUAGE_OPT_WIN_TAB, PNULL); 
            break;
        default:
            break;
        }
}

LOCAL void OpenBTWaitWin()
{
    /*MMI_STRING_T wait_text = {0};
    MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMICP_BTSTART_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);*/
    MMK_CreateWin((uint32 *)MMICP_BT_WAIT_WIN_TAB, NULL);
}

LOCAL void UpdateIMLanguageRect(MMI_WIN_ID_T win_id,BOOLEAN highlight)
{
    HighLightIMLanguageRect(win_id,highlight);
    UpdateIMLanguageContent(win_id);
}

LOCAL void HighLightIMLanguageRect(MMI_WIN_ID_T win_id,BOOLEAN highlight)
{
    if(highlight)
    {
        current_ctrl_id = CP_IM_LANGUAGE_CTRL_ID;
        MMK_SetAtvCtrl(MMK_ConvertIdToHandle(win_id), CP_IM_LANGUAGE_CTRL_ID);
        SetRectBackground(win_id,DICTIONARY_RECT,cp_theme_info.rect_focus_color);
    }
    else
    {
        SetRectBackground(win_id,DICTIONARY_RECT,BT_DATA_RECT_DEFAULT_COLOR);
    }
}

LOCAL void UpdateIMLanguageContent(MMI_WIN_ID_T win_id)
{
    UpdateIMLanguageTitle(win_id);
    UpdateIMLanguage(win_id);

}

LOCAL void UpdateIMLanguageTitle(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T text_str = {0}; 
	GUIFONT_REGION_STYLE_T region_style = {0};
    GUISTR_STYLE_T text_style = {0}; 
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T data_text = {0};
    MMI_GetLabelTextByLang(STXT_T9_SETTINGS,&data_text);

    text_str.wstr_ptr = data_text.wstr_ptr;
    text_str.wstr_len = data_text.wstr_len;
    text_style.region_ptr = &region_style;
    text_style.align = ALIGN_LEFT;
    text_style.font = MMI_DEFAULT_BIG_FONT;
    text_style.font_color = DATA_TEXT_OFF_COLOR;
    text_style.begin_alpha = text_style.end_alpha = BT_DATA_TEXT_COLOR_OPACITY;
    text_state |= GUISTR_STATE_EFFECT|GUISTR_STATE_GRADUAL_HOR|GUISTR_STATE_GRADUAL_VER;

    GUISTR_DrawSingleLineGradualInRect(MMITHEME_GetDefaultLcdDev(),&DICTIONARY_TITLE_RECT,&text_str,&text_style,text_state);

}

LOCAL void UpdateIMLanguage(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T text_str = {0};
    GUIFONT_REGION_STYLE_T region_style = {0};
    GUISTR_STYLE_T text_style = {0}; 
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T dict_string = {0};
    uint8 lang_name_len = 0;
    MMISET_LANGUAGE_TYPE_E cur_im_language = MMIAPISET_GetCurrentDictionaryInputLanguage();
    //GUIIM_LANGUAGE_T sel_im_index = MMIAPIIM_GetLanguage();
    //uint16 cur_im_language = MMIAPIIM_GetSetLangByGUILangIdx(sel_im_index);
    if(MMIAPISET_GetDictionaryStatus() == FALSE
#ifdef MMI_SIM_LANGUAGE_SUPPORT
        ||(cur_im_language==0)
#endif
        )
    {
        MMI_GetLabelTextByLang(STR_POC_ILIST_OFF,&dict_string);
    }
    else
    {
        lang_name_len = MMIAPICOM_Wstrlen(s_lang_set[cur_im_language].lang_name);
        dict_string.wstr_ptr =s_lang_set[cur_im_language].lang_name;
        dict_string.wstr_len= lang_name_len;
    }
    text_str.wstr_ptr = dict_string.wstr_ptr;
    text_str.wstr_len = dict_string.wstr_len;
    text_style.region_ptr = &region_style;
    text_style.align = ALIGN_LEFT;
    text_style.font = MMI_DEFAULT_NORMAL_FONT;
    text_style.font_color = DATA_TEXT_OFF_COLOR;

    GUISTR_DrawSingleLineGradualInRect(MMITHEME_GetDefaultLcdDev(),&DICTIONARY_LANGUAGE_RECT,&text_str,&text_style,text_state);
}


LOCAL void  OpenIMLanguageSelectMenu(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
    MMI_STRING_T stb_str = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    GUIMENU_POP_SEL_INFO_T pop_info = {0};
    MMI_STRING_T dict_string = {0};
    MMI_STRING_T dict_off = {0};
    uint8 lang_name_len = 0;
    //GUIIM_LANGUAGE_T sel_im_index  =  0;
    MMISET_LANGUAGE_TYPE_E cur_im_language = MMISET_LANGUAGE_ENGLISH;
    uint8 i = 0;

    //sel_im_index = MMIAPIIM_GetLanguage();
    cur_im_language = MMIAPISET_GetCurrentDictionaryInputLanguage();
    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP_RADIO);

    MMI_GetLabelTextByLang(STXT_T9_SETTINGS_OFF, &dict_off);
    node_item.item_text_ptr = &dict_off; 
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(0,MMICP_LANGUAGE_DICTIONARY_OFF,MMICP_MENU_DICTIONARY_ITEM_ID,&node_item,ctrl_id);
    pop_info.is_static = FALSE;
    pop_info.ctrl_id = ctrl_id;
    pop_info.node_id = MMICP_LANGUAGE_DICTIONARY_OFF;
    GUIMENU_SetPopItemStatus(
                (MMIAPISET_GetDictionaryStatus()==FALSE)
#ifdef MMI_SIM_LANGUAGE_SUPPORT
                ||(cur_im_language==0)
#endif
                ,&pop_info);

#ifdef  MMI_SIM_LANGUAGE_SUPPORT
    for(i = 1; i < MMISET_MAX_LANGUAGE; i++)
#else
    for(i = 0; i < MMISET_MAX_LANGUAGE; i++)
#endif
    {
        if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i)&& MMIAPISET_CheckDictionaryLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))
        {
            lang_name_len = MMIAPICOM_Wstrlen(s_lang_set[i].lang_name);
            dict_string.wstr_ptr = s_lang_set[i].lang_name ;

            dict_string.wstr_len = lang_name_len;
            node_item.item_text_ptr = &dict_string;
            node_item.is_grayed = FALSE;

#ifdef  MMI_SIM_LANGUAGE_SUPPORT
            GUIMENU_InsertNode(i,i + MMICP_LANGUAGE_DICTIONARY_OFF,MMICP_MENU_DICTIONARY_ITEM_ID,&node_item,ctrl_id);
#else
            GUIMENU_InsertNode(i + 1,i + 1 + MMICP_LANGUAGE_DICTIONARY_OFF,MMICP_MENU_DICTIONARY_ITEM_ID,&node_item,ctrl_id);
#endif
            pop_info.is_static = FALSE;
            pop_info.ctrl_id = ctrl_id;
#ifdef  MMI_SIM_LANGUAGE_SUPPORT
            pop_info.node_id = i + MMICP_LANGUAGE_DICTIONARY_OFF;
#else
            pop_info.node_id = i + MMICP_LANGUAGE_DICTIONARY_OFF + 1 ;
#endif

            if(MMIAPISET_GetDictionaryStatus() == TRUE)
            {
                if(cur_im_language == i)
                {
                    GUIMENU_SetPopItemStatus(TRUE,&pop_info);
                }
                else
                {
                    GUIMENU_SetPopItemStatus(FALSE,&pop_info);
                }
            }
        }
    }

    GUIMENU_SetPopMenuCentered(ctrl_id,TRUE);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id,TXT_NULL,STXT_SOFTKEY_SELECT_MK,TXT_COMMON_COMMON_BACK);
    //MMI_GetLabelTextByLang(STR_SETT_LIST_DUAL_SIM_EXT02, &stb_str);
    //CTRLMENU_SetTitle(&stb_str,ctrl_id);
}

 LOCAL void  CP_HandleDisplayBannerTimeout( uint8 timer_id, uint32 param)
{
    MMIAPICOM_OpenBannerWin(MMICP_IM_LANGUAGE_BANNER_WIN_ID, banner_info);
}


LOCAL MMI_RESULT_E   HandleIMLanguageMenuWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E  err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = CP_IM_LANGUAGE_MENU_CTRL_ID;
    uint32 node_id = 0;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            OpenIMLanguageSelectMenu(win_id,ctrl_id);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch (node_id)
            {
                case MMICP_LANGUAGE_ENGLISH:
                case MMICP_LANGUAGE_SIMP_CHINESE:
                case MMICP_LANGUAGE_TRAD_CHINESE:
                case MMICP_LANGUAGE_ARABIC:
                case MMICP_LANGUAGE_FRENCH:
                case MMICP_LANGUAGE_HINDI:
                case MMICP_LANGUAGE_HUNGARIAN:
                case MMICP_LANGUAGE_INDONESIAN:
                case MMICP_LANGUAGE_MALAY:
                case MMICP_LANGUAGE_PORTUGUESE:
                case MMICP_LANGUAGE_RUSSIAN:
                case MMICP_LANGUAGE_SPANISH:
                case MMICP_LANGUAGE_TAGALOG:
                case MMICP_LANGUAGE_THAI:
                case MMICP_LANGUAGE_VIETNAMESE:
                case MMICP_LANGUAGE_URDU:
                case MMICP_LANGUAGE_ITALIAN:
                case MMICP_LANGUAGE_PERSIAN:
                case MMICP_LANGUAGE_TURKISH:
                case MMICP_LANGUAGE_GERMAN:
                case MMICP_LANGUAGE_GREEK:
                case MMICP_LANGUAGE_HEBREW:
                case MMICP_LANGUAGE_BENGALI:
                case MMICP_LANGUAGE_CZECH:
                case MMICP_LANGUAGE_SLOVENIAN:
                case MMICP_LANGUAGE_ROMANIAN:
                case MMICP_LANGUAGE_TELUGU:
                case MMICP_LANGUAGE_MARATHI:
                case MMICP_LANGUAGE_TAMIL:
                case MMICP_LANGUAGE_GUJARATI:
                case MMICP_LANGUAGE_KANNADA:
                case MMICP_LANGUAGE_MALAYALAM:
                case MMICP_LANGUAGE_ORIYA:
                case MMICP_LANGUAGE_PUNJABI:
                case MMICP_LANGUAGE_AFRIKAANS:
                case MMICP_LANGUAGE_ALBANIAN:
                case MMICP_LANGUAGE_ARMENIAN:
                case MMICP_LANGUAGE_AZERBAIJANI:
                case MMICP_LANGUAGE_BASQUE:
                case MMICP_LANGUAGE_BULGARIAN:
                case MMICP_LANGUAGE_CATALAN:
                case MMICP_LANGUAGE_CROATIAN:
                case MMICP_LANGUAGE_DANISH:
                case MMICP_LANGUAGE_DUTCH:
                case MMICP_LANGUAGE_ESTONIAN:
                case MMICP_LANGUAGE_FILIPINO:
                case MMICP_LANGUAGE_FINNISH:
                case MMICP_LANGUAGE_GALICIAN:
                case MMICP_LANGUAGE_GEORGIAN:
                case MMICP_LANGUAGE_HAUSA:
                case MMICP_LANGUAGE_ICELANDIC:
                case MMICP_LANGUAGE_IGBO:
                case MMICP_LANGUAGE_IRISH:
                case MMICP_LANGUAGE_KAZAKH:
                case MMICP_LANGUAGE_LATVIAN:
                case MMICP_LANGUAGE_LITHUANIAN:
                case MMICP_LANGUAGE_MACEDONIAN:
                case MMICP_LANGUAGE_MOLDOVAN:
                case MMICP_LANGUAGE_NORWEGIAN:
                case MMICP_LANGUAGE_POLISH:
                case MMICP_LANGUAGE_SERBIAN:
                case MMICP_LANGUAGE_SESOTHO:
                case MMICP_LANGUAGE_SLOVAK:
                case MMICP_LANGUAGE_SWEDISH:
                case MMICP_LANGUAGE_UKRAINIAN:
                case MMICP_LANGUAGE_YORUBA:
                case MMICP_LANGUAGE_XHOSA:
                case MMICP_LANGUAGE_ZULU:
                case MMICP_LANGUAGE_ASSAMESE:
                case MMICP_LANGUAGE_SWAHILI:
                case MMICP_LANGUAGE_MYANMAR:
                case MMICP_LANGUAGE_AMHARIC:
                case MMICP_LANGUAGE_KHMER:
                case MMICP_LANGUAGE_LAO:
                case MMICP_LANGUAGE_UYGHUR:
                case MMICP_LANGUAGE_TIBETAN:
                case MMICP_LANGUAGE_SINHALESE:
                case MMICP_LANGUAGE_BOSNIAN:
                case MMICP_LANGUAGE_TRAD_CHINESE_HK:
                case MMICP_LANGUAGE_AMERICAN:
                case MMICP_LANGUAGE_PORTUGUESE_BR:
                case MMICP_LANGUAGE_TURKMEN_LATIN:
                case MMICP_LANGUAGE_SPANISH_AM:
                case MMICP_LANGUAGE_BENGALI_BD:
                case MMICP_LANGUAGE_PASHTO:
                case MMICP_LANGUAGE_UZBEK:
                {
                    uint32 cur_dict_index = node_id - MMICP_LANGUAGE_DICTIONARY_OFF;
                    GUI_POINT_T start_point = {0, 0};
                    /*if(node_id == MMICP_LANGUAGE_AMERICAN)
                    {
                        cur_dict_index = MMICP_LANGUAGE_ENGLISH - MMICP_LANGUAGE_DICTIONARY_OFF;
                    }*/
                    MMIAPISET_SetDictionaryStatus(TRUE);
                    MMISET_SetDictionaryInputLanguage((MMISET_LANGUAGE_TYPE_E)cur_dict_index);
                    banner_info.banner_type = MMI_BANNER_TEXT;
                    banner_info.text1_id = STXT_T9_ENABLED_CONF;
                    banner_info.start_point = start_point;
                    MMK_CloseWin(win_id);
                    //MMIAPICOM_OpenBannerWin(MMICP_IM_LANGUAGE_BANNER_WIN_ID, banner_info);
                    MMK_CreateTimerCallback(CP_BANNER_DELAY_TIME, CP_HandleDisplayBannerTimeout,(uint32)0,FALSE);
                }
                    break;
                case MMICP_LANGUAGE_DICTIONARY_OFF:
                { 
                    GUI_POINT_T start_point = {0, 0};
                    MMIAPISET_SetDictionaryStatus(FALSE);
                    banner_info.banner_type = MMI_BANNER_TEXT;
                    banner_info.text1_id = STXT_T9_DISABLED_CONF;
                    banner_info.start_point = start_point;
                    MMK_CloseWin(win_id);
                    MMK_CreateTimerCallback(CP_BANNER_DELAY_TIME, CP_HandleDisplayBannerTimeout,(uint32)0,FALSE);
                    //MMIAPICOM_OpenBannerWin(MMICP_IM_LANGUAGE_BANNER_WIN_ID, banner_info);
                }
                    break;
                default:
                    break;
            }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default :
            err_code = MMI_RESULT_FALSE;
            break;
    }
    return err_code;
}


LOCAL BOOLEAN SetCurrentFoucs(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN focus_changed = TRUE;
    MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
    switch(msg_id)
    {
        case MSG_APP_UP:
            focus_changed = TRUE;
            if(current_ctrl_id == MMICP_NONE_CTRL_ID)
            {
                UpdateIMLanguageRect(win_id,TRUE);
            }
#ifdef MMI_MULTI_SIM_SYS_DUAL
            else if(current_ctrl_id == CP_IM_LANGUAGE_CTRL_ID)
            {
                UpdateSIMPreferredRect(win_id,TRUE);
                UpdateIMLanguageRect(win_id,FALSE);
            }
            else if(current_ctrl_id == CP_SIM_PREFERRED_CTRL_ID)
            {
                UpdateSIMPreferredRect(win_id,FALSE);
                UpdateDataRect(win_id,TRUE);
            }
#else
            else if(current_ctrl_id == CP_IM_LANGUAGE_CTRL_ID)
            {
                UpdateIMLanguageRect(win_id,FALSE);
                UpdateDataRect(win_id,TRUE);
            }
#endif

#ifdef BLUETOOTH_SUPPORT
            else if(current_ctrl_id == CP_DATA_CTRL_ID)
            {
                UpdateDataRect(win_id,FALSE);
                UpdateBTRect(win_id,TRUE);
            }
            else if(current_ctrl_id == CP_BT_CTRL_ID)
            {
                UpdateBTRect(win_id,FALSE);
                current_ctrl_id = CP_BRI_PROGRESS_CTRL_ID;
                MMK_SetAtvCtrl(win_id, CP_BRI_PROGRESS_CTRL_ID);
            }
#endif            
            else if(current_ctrl_id == CP_BRI_PROGRESS_CTRL_ID)
            {
#ifdef MMI_MULTI_SIM_SYS_DUAL
                if(VOLUME_BAR_RECT.bottom==0 || VOLUME_BAR_RECT.top==0)
                {
                GUI_RECT_T rect = {0};
                rect.top = MMI_STATUSBAR_HEIGHT;
                rect.bottom = lcd_height;
                rect.left = 0;
                rect.right = lcd_width;
                GetTopRect();
                LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),rect,CP_MAIN_BG_COLOR,255);
                if(VOLUME_BAR_RECT.bottom!=0 && VOLUME_BAR_RECT.top!=0)
                {
                    CTRLBASE_SetVisible(CP_VOL_PROGRESS_CTRL_ID,TRUE);
                    GUIPRGBOX_SetRect(CP_VOL_PROGRESS_CTRL_ID,&VOLUME_BAR_RECT);
                }
                GUIPRGBOX_SetRect(CP_BRI_PROGRESS_CTRL_ID,&BRIGIHTNESS_BAR_RECT);
                UpdateMainWindow(win_id);
                }
#endif
                current_ctrl_id = CP_VOL_PROGRESS_CTRL_ID;
                MMK_SetAtvCtrl(win_id, CP_VOL_PROGRESS_CTRL_ID);
            }
            else if(current_ctrl_id == CP_VOL_PROGRESS_CTRL_ID)
            {
#ifndef MMI_MULTI_SIM_SYS_DUAL
                GUI_RECT_T rect = {0};
                rect.top = MMI_STATUSBAR_HEIGHT;
                rect.bottom = lcd_height;
                rect.left = 0;
                rect.right = lcd_width;
                GetTopRect();
                LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),rect,CP_MAIN_BG_COLOR,255);
                GUIPRGBOX_SetRect(CP_VOL_PROGRESS_CTRL_ID,&VOLUME_BAR_RECT);
                GUIPRGBOX_SetRect(CP_BRI_PROGRESS_CTRL_ID,&BRIGIHTNESS_BAR_RECT);
                UpdateMainWindow(win_id);
#endif
            }
            else
            {
                focus_changed = FALSE;
            }
            break;
        case MSG_APP_DOWN:
            focus_changed = TRUE;
            if(current_ctrl_id == MMICP_NONE_CTRL_ID)
            {
                current_ctrl_id = CP_VOL_PROGRESS_CTRL_ID;
                MMK_SetAtvCtrl(win_id, CP_VOL_PROGRESS_CTRL_ID);
            }
            else if(current_ctrl_id == CP_VOL_PROGRESS_CTRL_ID)
            {
                current_ctrl_id = CP_BRI_PROGRESS_CTRL_ID;
                MMK_SetAtvCtrl(win_id, CP_BRI_PROGRESS_CTRL_ID);
            }
            #ifdef BLUETOOTH_SUPPORT
            else if(current_ctrl_id == CP_BRI_PROGRESS_CTRL_ID)
            {
                UpdateBTRect(win_id,TRUE);
            }
            else if(current_ctrl_id == CP_BT_CTRL_ID)
            {
                UpdateBTRect(win_id,FALSE);
                UpdateDataRect(win_id,TRUE);
            }
            #endif
#ifdef MMI_MULTI_SIM_SYS_DUAL
            else if(current_ctrl_id == CP_DATA_CTRL_ID)
            {
                GUI_RECT_T rect = {0};
                rect.top = MMI_STATUSBAR_HEIGHT;
                rect.bottom = lcd_height;
                rect.left = 0;
                rect.right = lcd_width;
                GetBottomRect();
                current_ctrl_id = CP_SIM_PREFERRED_CTRL_ID;
                LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),rect,CP_MAIN_BG_COLOR,255);
                if(VOLUME_BAR_RECT.bottom==0 || VOLUME_BAR_RECT.top==0)
                {
                    GUIPRGBOX_SetRect(CP_VOL_PROGRESS_CTRL_ID,&VOLUME_BAR_RECT);
                    CTRLBASE_SetVisible(CP_VOL_PROGRESS_CTRL_ID,FALSE);
                }
                GUIPRGBOX_SetRect(CP_BRI_PROGRESS_CTRL_ID,&BRIGIHTNESS_BAR_RECT);
                UpdateMainWindow(win_id);
            }
            else if(current_ctrl_id == CP_SIM_PREFERRED_CTRL_ID)
            {
                UpdateSIMPreferredRect(win_id,FALSE);
                UpdateIMLanguageRect(win_id,TRUE);
            }
#else
            else if(current_ctrl_id == CP_DATA_CTRL_ID)
            {
                GUI_RECT_T rect = {0};
                rect.top = MMI_STATUSBAR_HEIGHT;
                rect.bottom = lcd_height;
                rect.left = 0;
                rect.right = lcd_width;
                GetBottomRect();
                current_ctrl_id = CP_IM_LANGUAGE_CTRL_ID;
                LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),rect,CP_MAIN_BG_COLOR,255);
                GUIPRGBOX_SetRect(CP_VOL_PROGRESS_CTRL_ID,&VOLUME_BAR_RECT);
                GUIPRGBOX_SetRect(CP_BRI_PROGRESS_CTRL_ID,&BRIGIHTNESS_BAR_RECT);
                UpdateMainWindow(win_id);
            }
#endif
            else
            {
                focus_changed = FALSE;
            }
            break;
        case MSG_APP_LEFT:
            if(current_ctrl_id == MMICP_NONE_CTRL_ID)
            {
                focus_changed = TRUE;
                #ifdef BLUETOOTH_SUPPORT
                UpdateBTRect(win_id,TRUE);
                #endif
            }
            else if(current_ctrl_id == CP_DATA_CTRL_ID)
            {
                focus_changed = TRUE;
                UpdateDataRect(win_id,FALSE);
                #ifdef BLUETOOTH_SUPPORT
                UpdateBTRect(win_id,TRUE);
                #endif
            }
            else if(ctrl_id == CP_VOL_PROGRESS_CTRL_ID && current_ctrl_id == CP_VOL_PROGRESS_CTRL_ID)
            {
                HandleVolumeBar(FALSE);
            }
            else if(ctrl_id == CP_BRI_PROGRESS_CTRL_ID && current_ctrl_id == CP_BRI_PROGRESS_CTRL_ID)
            {
                HandleBrightnessBar(FALSE);
            }
            break;
        case MSG_APP_RIGHT:
            if(current_ctrl_id == MMICP_NONE_CTRL_ID)
            {
                focus_changed = TRUE;
                UpdateDataRect(win_id,TRUE);
            }
#ifdef BLUETOOTH_SUPPORT            
            else if(current_ctrl_id == CP_BT_CTRL_ID)
            {
                focus_changed = TRUE;
                UpdateBTRect(win_id,FALSE);
                UpdateDataRect(win_id,TRUE);
            }
#endif
            else if(ctrl_id == CP_VOL_PROGRESS_CTRL_ID && current_ctrl_id == CP_VOL_PROGRESS_CTRL_ID)
            {
                HandleVolumeBar(TRUE);
            }
            else if(ctrl_id == CP_BRI_PROGRESS_CTRL_ID && current_ctrl_id == CP_BRI_PROGRESS_CTRL_ID)
            {
                HandleBrightnessBar(TRUE);
            }
            break;
        default:
            break;
        }
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICP_GETCURRENTFOUCS_0,(uint8*)"d", current_ctrl_id);
    return focus_changed;
}

LOCAL void CPSetRingVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
    if(PNULL == para_ptr)
    {
        return;
    }

    if(para_ptr->last_value > 0 && para_ptr->cur_value == 0)
    {
        MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());
#ifdef ZDT_PRODUCT_VIBRATE        
        MMIAPIENVSET_ActiveMode(MMIENVSET_MEETING_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_MEETING_MODE);
#else        
        MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_SILENT_MODE);
#endif        
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, 0, MN_DUAL_SYS_MAX);
        MMISet_SetNotificationAlert(FALSE);//dong.chunguang add for disable Alert when Meeting and Silent
#ifdef MMI_MULTI_SIM_SYS_DUAL
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
#endif
    }
    else if(para_ptr->cur_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_SILENT_MODE)
    {
        MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());
#ifdef ZDT_PRODUCT_VIBRATE        
        MMIAPIENVSET_ActiveMode(MMIENVSET_MEETING_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_MEETING_MODE);
#else        
        MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_SILENT_MODE);
#endif 
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, 0, MN_DUAL_SYS_MAX);
        MMISet_SetNotificationAlert(FALSE);//dong.chunguang add for disable Alert when Meeting and Silent
#ifdef MMI_MULTI_SIM_SYS_DUAL
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MN_DUAL_SYS_2);
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA,MMIAPISET_GetActiveSim());
#endif
    }
#ifdef ZDT_PRODUCT_VIBRATE    
    else if(para_ptr->cur_value==0 && MMIAPIENVSET_GetActiveModeId()==MMIENVSET_MEETING_MODE)
    {
        MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());
        MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_SILENT_MODE);
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, 0, MN_DUAL_SYS_MAX);
#ifdef MMI_MULTI_SIM_SYS_DUAL
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_SILENT,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_SILENT,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_SILENT,MN_DUAL_SYS_1);
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_SILENT,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_SILENT,MN_DUAL_SYS_2);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_SILENT,MN_DUAL_SYS_2);
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_SILENT,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_SILENT,MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_SILENT,MMIAPISET_GetActiveSim());
#endif
    }
#endif    
    else
    {
        /*if(para_ptr->last_value == 0 && para_ptr->cur_value > 0 && MMIAPIENVSET_GetActiveModeId() == MMIENVSET_MEETING_MODE)
        {
            MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());
            MMIAPIENVSET_ActiveMode(MMIENVSET_STANDARD_MODE);
        }*/
        MMIAPIENVSET_ActiveMode(MMIENVSET_STANDARD_MODE);
        MMIENVSET_SetCurModeId(MMIENVSET_STANDARD_MODE);
        //dong.chunguang add for enable Alert when go to standard mode
        if(MMISet_GetNotificationAlert()==FALSE&&para_ptr->last_value == 0)
            MMISet_SetNotificationAlert(TRUE);
        //end
        //MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
#ifdef MMI_MULTI_SIM_SYS_DUAL
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
        MMIENVSET_SetCurModeOptValue(MSG_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
        MMIENVSET_SetCurModeOptValue(ALARM_RING_VOL, para_ptr->cur_value, MN_DUAL_SYS_MAX);
        if(MMISET_CALL_VIBRA == MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1,CALL_RING_TYPE)
            ||MMISET_CALL_VIBRA == MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_2,CALL_RING_TYPE)
            ||MMISET_CALL_VIBRA_AND_RING == MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1,CALL_RING_TYPE)
            ||MMISET_CALL_VIBRA_AND_RING == MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_2,CALL_RING_TYPE))
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA_AND_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA_AND_RING,MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MN_DUAL_SYS_2);
        }
        else
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_1);
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_RING,MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_2);
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MN_DUAL_SYS_2);
        }
#else
        MMIENVSET_SetCurModeOptValue(CALL_RING_VOL, para_ptr->cur_value, MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(MSG_RING_VOL, para_ptr->cur_value, MMIAPISET_GetActiveSim());
        MMIENVSET_SetCurModeOptValue(ALARM_RING_VOL, para_ptr->cur_value, MMIAPISET_GetActiveSim());
        if(MMISET_CALL_VIBRA == MMIAPIENVSET_GetCurModeOptValue(MMIAPISET_GetActiveSim(),CALL_RING_TYPE)
            ||MMISET_CALL_VIBRA_AND_RING == MMIAPIENVSET_GetCurModeOptValue(MMIAPISET_GetActiveSim(),CALL_RING_TYPE))
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_VIBRA_AND_RING,MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_VIBRA_AND_RING,MMIAPISET_GetActiveSim());
        }
        else
        {
            MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE,MMISET_CALL_RING,MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(MSG_RING_TYPE,MMISET_MSG_RING,MMIAPISET_GetActiveSim());
            MMIENVSET_SetCurModeOptValue(ALARM_RING_TYPE,MMISET_MSG_RING,MMIAPISET_GetActiveSim());
        }
#endif
    }
    return;
}

LOCAL BOOLEAN CP_GetBTVisibleNDT()
{
    NVITEM_ERROR_E nv_reslut = NVERR_NONE;
    uint16 nv_len = 0;
    NDT_BT_CONFIG_T bt_config;
    MMIBT_HIDE_DEVICE_TYPE_E cur_selection = MMIBT_HIDE_DEVICE_HIDDEN;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    bt_config.is_visible = 0;
    bt_config.storage_src = 0;

    MMINV_READ(MMINV_BT_HIDE_DEVICE,&cur_selection,return_value);
    if(MN_RETURN_SUCCESS == return_value)
    {
        if(cur_selection == MMIBT_HIDE_DEVICE_HIDDEN)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        nv_len = NVITEM_GetLength(APP_NV_BT_CONFIG);
        nv_reslut = EFS_NvitemRead(APP_NV_BT_CONFIG, nv_len ,(uint8*)(&bt_config));
        if (NVERR_NONE == nv_reslut)
        {
            if(bt_config.is_visible)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            SCI_TRACE_LOW("MMICP read APP_NV_BT_CONFIG fail");
            return FALSE;
        }
    }
}
