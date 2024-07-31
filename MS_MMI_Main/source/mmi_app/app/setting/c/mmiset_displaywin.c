/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_DISPLAYWIN_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "guiform.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
//#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_display.h"
#include "guires.h"
#include "mmiset_nv.h"
#include "mmiset_wintab.h"
#include "mmiudisk_export.h"
#include "guiownerdraw.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "guisetlist.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_livewallpaper_text.h"
#endif
#include "mmk_tp.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiset_func.h"
#include "mmi_resource.h"
#include "mmiset_export.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_export.h"
#endif
#ifdef UI_P3D_SUPPORT   
#include "mmi_winse.h"
#endif
#include "mmiset_internal.h"
#include "mmiset_lock_effect_internal.h"

//#ifdef PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM
#include "mmiset_mainmenu_export.h"
//#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#include "mmiset_lock_effect_export.h"
#endif
//2015-7-9 tangting modify setting-display ui 
/**--------------------------------------------------------------------------*
**
**--------------------------------------------------------------------------*/
LOCAL const MMI_TEXT_ID_T s_display_type_text_id[2] = 
{
	STR_SETTINGS_BACKLIGHT_HEADER,
    TXT_NULL,
};

//#define HW_LCD_TEST
/**--------------------------------------------------------------------------*
**                function declaration                                       *
**--------------------------------------------------------------------------*/
#define     MMISET_KEYBOARD_LIGHT_TIME_20       20000
#define     MMISET_KEYBOARD_LIGHT_TIME_10       10000
#define     MMISET_KEYBOARD_LIGHT_TIME_5        5000
#define     MMISET_DISPLAY_SET_BRIMAX  7
#if defined MMISET_ANIM_EFFECT_SUPPORT
BOOLEAN     s_fmm_poweron_call = FALSE;
BOOLEAN     s_fmm_poweroff_call = FALSE;
MMISET_FONT_SIZE_TYPE_E s_font_size_type = 0;
/*****************************************************************************/
//  Description : Set Anim effect menu name 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetAnimLabelParam(void);
#endif
#if defined MMISET_WALLPAPER_SUPPORT
/*****************************************************************************/
//  Description : Set Wallpaper menu name 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetWallPaperParam(void);
#endif
#if defined MMISET_THEME_SUPPORT
/*****************************************************************************/
//  Description : set theme content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetThemeParam(void);
#endif
#if defined MMISET_IDLE_STYLE_SUPPORT
/*****************************************************************************/
//  Description : set idle style content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleStyleParam(void);
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
//  Description : set idle style content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleDisplaySettingParam(void);
#endif
#endif
/*****************************************************************************/
//  Description : set contrast lable name
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetContrastParam(void);
#if defined MMISET_BACKLIGHT_TIME_SUPPORT
/*****************************************************************************/
//  Description : set backlight param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetBacklightParam(void);
#endif
//2015-7-9 tangting modify settings-display ui
LOCAL void SetBacklightTimeoutParam(void);
//end
#if defined MMISET_KEYBACKLIGHT_SUPPORT
/*****************************************************************************/
//  Description : set Keyboard light param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetKeyboardLightParam(void);
#endif

LOCAL void SetKeyAUTOKEYPADParam(void);

LOCAL void MMISET_SetENDKEYLOCKPadList(void);

#if defined MMISET_FONT_SUPPORT /* lint_lai */
/*****************************************************************************/
//  Description : set font content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetFontParam(void);
#endif
#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : set mainmenu effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenu_EffectParam(void);
#endif
#endif
#if !defined MMI_PDA_SUPPORT && defined(EFFECT_3D_LIST_ROLL_SUPPORT)
/*****************************************************************************/
//  Description : set list effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetListEffectParam(void);
#endif
#ifdef EFFECT_LCD_ROTATE_SUPPORT
/*****************************************************************************/
//  Description : set g-sensor effect param
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void SetGsensor_EffectParam(void);
#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : set power on param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPowerOnEffectParam(void);


/*****************************************************************************/
//  Description : set power off param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPowerOffEffectParam(void);
#endif
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
/*****************************************************************************/
//  Description : set go effect param
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetRenderEffectParam(void);
#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : save all anim set feature
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllAnimSet(void);



/*****************************************************************************/
//  Description : Handle the window of power on animation setting.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerOnAnimWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : Handle the window of power off animation setting.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerOffAnimWindow(
                                                MMI_WIN_ID_T       win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                );
 
/*****************************************************************************/
//  Description : Append the power on off setting items.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendPowerOnOffAnimItem(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    BOOLEAN         is_power_on_off
                                    );
#endif
                                    
#ifdef SET_3D_STYLE_SUPPORT 
/*****************************************************************************/
//  Description : set 3d interface param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void Set3D_EffectParam(void);
#endif
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : to handle edit idle words win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditIdleWordsWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );   
                                              
#endif
/*****************************************************************************/
//  Description : save all setting
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllFeatureState(void);
#if defined DPHONE_SUPPORT
/*****************************************************************************/
//  Description : SetContraseFormParam
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetContrastFormParam(void);
/*****************************************************************************/
//  Description : to handle set backlight
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBacklightWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description : to handle set constrast
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetConstrastWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );      
                                             

#endif
#if defined MMISET_KEY_BOARD_LIGHT_SUPPORT 
/*****************************************************************************/
//  Description : to handle select keyborad light
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetKeyboardLightWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 );
#endif
#if defined MMISET_FONT_SUPPORT
/*****************************************************************************/
//  Description : get font info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFontInfo( 
                          wchar * font_name_ptr
                          );

/*****************************************************************************/
//  Description : set font info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFontInfo(
                          const wchar* font_name_ptr
                          );

/*****************************************************************************/
//  Description : set font
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFontWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );
#endif 
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : set main menu style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMainmenuStyle(
                            MMI_CTRL_ID_T       ctrl_id
                            );
#endif                            
/*****************************************************************************/
//  Description : set idle style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetIdleStyle(
                           MMI_CTRL_ID_T  ctrl_id
                           );                                                                                                                                                                                                                                                 
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : to handle display setting animation window message
//  Global resource dependence : 
//  Author:applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDisplayAnimationWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );         
#endif

#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
/*****************************************************************************/
//  Description : to handle display setting go animation window message
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetLightShadowAnimationWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT                                
/*****************************************************************************/
//  Description : to handle select idle display mode
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetIdleDisplayWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );   
#endif  
#endif                                                 
/*****************************************************************************/
//  Description : to handle display setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDisplayWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
//2015-7-9 tangting modify settings-display ui
/*****************************************************************************/
//  Description : to handle blacklight window message
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayBlacklightWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
/*****************************************************************************/
//  Description : to handle blacklight time out window message
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayBlacklightTimeOutWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
/*****************************************************************************/
//  Description : to handle font size window message
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayFontSizeWindow(
		                             MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayListWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayFontSizeChoiceWindow(
		                             MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
//end


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBacklightCrWindow(
                                               MMI_WIN_ID_T       win_id,
                                               MMI_MESSAGE_ID_E   msg_id,
                                               DPARAM             param
                                               );

#if defined MMISET_MAINMENU_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenuEffectParam(void);
#endif

#if defined MMISET_UIDLE_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void SetUIdleEffectParam(void);
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenuSelectEffectParam(void);

#endif
/*****************************************************************************/  
#if defined MMISET_FONT_SUPPORT
// the window for select font
WINDOW_TABLE( MMISET_SET_FONT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetFontWindow ),    
        WIN_ID( MMISET_SET_FONT_WIN_ID ),
        WIN_TITLE( TXT_SET_FONT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_FONT_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};
#endif

#if defined MMISET_KEY_BOARD_LIGHT_SUPPORT 
// the window for select keyboard light time
WINDOW_TABLE( MMISET_SET_KEYBOARD_LIGHT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetKeyboardLightWindow ),    
        WIN_ID( MMISET_SET_KEYBOARD_LIGHT_WIN_ID ),
        WIN_TITLE( TXT_SET_KEY_BOARD_LIGHT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_KEYBOARD_LIGHT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
//the window for set idle win display 
WINDOW_TABLE( MMISET_SET_IDLE_DISPLAY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetIdleDisplayWindow ),    
        WIN_ID( MMISET_SET_IDLE_DISPLAY_WIN_ID ),
        WIN_TITLE( TXT_SET_IDLE_DISPLAY ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_IDLE_DISPLAY_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
#ifdef 	GUI_FULLSCREEN_POPMENU_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
#endif
        END_WIN
};
#endif
#endif
//@xingdong.li
#ifdef MMISET_ANIM_EFFECT_SUPPORT//yongsheng.wang add
// the window for select power on animation
WINDOW_TABLE( MMISET_SET_POWER_ON_ANIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetPowerOnAnimWindow ),    
        WIN_ID( MMISET_SET_POWER_ON_ANIM_WIN_ID ),
        WIN_TITLE( TXT_COMMON_POWER_ON_ANIMATION ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_POWER_ON_ANIM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

//@xingdong.li
// the window for select power off animation
WINDOW_TABLE( MMISET_SET_POWER_OFF_ANIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetPowerOffAnimWindow ),    
        WIN_ID( MMISET_SET_POWER_OFF_ANIM_WIN_ID ),
        WIN_TITLE( TXT_COMMON_POWER_OFF_ANIMATION ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_POWER_OFF_ANIM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};
#endif
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
WINDOW_TABLE( MMISET_SET_RENDER_EFFECT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_SET_LIGHT_SHADOW_RENDER_ANIMATION ),
        WIN_FUNC( (uint32)HandleSetLightShadowAnimationWindow ),    
        WIN_ID( MMISET_SET_DISPLAY_RENDER_ANIMATION_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_DISPLAY_ANIM_RENDER_CHECKLIST_CTRL_ID),
        END_WIN
};

#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
//九宫格模式下的动画效果
WINDOW_TABLE( MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_TAB ) = 
{
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayAnimationWindow ),    
        WIN_ID( MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_ID ),
        WIN_TITLE(TXT_SET_ANIMATION),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
#ifdef SET_3D_STYLE_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_3D_EFFECT_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_3D_EFFECT_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_3D_EFFECT_FORM_CTRL_ID),//界面特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_3D_EFFECT_FORM_CTRL_ID),
#endif
#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID),//主菜单特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID),
#endif
#endif
#if !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_LIST_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID),//列表特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID),
#endif
#endif
#ifdef EFFECT_LCD_ROTATE_SUPPORT // 说明支持横竖屏切换
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_G_SENSOR_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID),//g-sensor切换
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID),
#endif

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_POWER_ON_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID),//开关机

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID),//开关机
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_RENDER_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_RENDER_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID),//开关机
#endif
        END_WIN
};                                         
#endif
#if 0//def QBTHEME_SUPPORT
//九宫格模式下的动画效果
WINDOW_TABLE( MMISET_SET_QB_DISPLAY_ICON_ANIMATION_WIN_TAB ) = 
{
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayAnimationWindow ),    
        WIN_ID( MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_ID ),
        WIN_TITLE(TXT_SET_ANIMATION),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),

#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID),//主菜单特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID),
#endif
#endif
#if !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_LIST_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID),//列表特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID),
#endif
#endif    
          

#ifdef EFFECT_LCD_ROTATE_SUPPORT // 说明支持横竖屏切换
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_G_SENSOR_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID),//g-sensor切换
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID),
#endif

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_POWER_ON_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID),//开关机

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID),//开关机
        
        END_WIN
};      
#endif
//#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
//the window of edit idle display words
WINDOW_TABLE( MMISET_EDIT_IDLE_WOEDS_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditIdleWordsWindow ),    
        WIN_ID( MMISET_EDIT_IDLE_WOEDS_WIN_ID ),
        WIN_TITLE( TXT_SET_CUSTOM_WORD ),

#if !defined MMI_GUI_STYLE_TYPICAL
		CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_IDLE_WORDS_FORM_CTRL_ID),
		CHILD_EDIT_TEXT_CTRL(TRUE, MMIENVSET_ENVNAME_MAX_LEN, MMISET_IDLE_WORDS_EDITCTRL_ID, MMISET_IDLE_WORDS_FORM_CTRL_ID),
#else
        CREATE_EDIT_TEXT_CTRL(0,MMISET_IDLE_WORDS_EDITCTRL_ID),
#endif
#ifdef MMI_PDA_SUPPORT
		CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMISET_IDLE_WORDS_FORM_CTRL_ID),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN
};
#endif
//#endif
#if defined DPHONE_SUPPORT
// the window for set constrast
WINDOW_TABLE( MMISET_SET_CONSTRAST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetConstrastWindow ),    
    WIN_ID( MMISET_SET_CONSTRAST_WIN_ID ),
    WIN_TITLE( TXT_SET_CONTRAST ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //create down button ctrl
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMISET_CONTRAST_FROM_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_SET_CONTRAST_DOWN_RELEASED,MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID,MMISET_CONTRAST_FROM_ID),
        CHILD_ANIM_CTRL(FALSE,FALSE,MMISET_CONTRAST_DOWN_BAR_ID,MMISET_CONTRAST_FROM_ID),
    //create up button ctrl
        CHILD_BUTTON_CTRL(TRUE,IMAGE_SET_CONTRAST_UP_RELEASED,MMISET_CONTRAST_UP_BUTTON_CTRL_ID,MMISET_CONTRAST_FROM_ID),
    END_WIN
};

// the window for set backlight
WINDOW_TABLE( MMISET_SET_BACKLIGHT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetBacklightWindow ),    
        WIN_ID( MMISET_SET_BACKLIGHT_WIN_ID ),
        WIN_TITLE( TXT_SET_BACK_LIGHT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_BACKLIGHT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),                     
        END_WIN
};


// the window for display setting 
WINDOW_TABLE( MMISET_SET_DISPLAY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayWindow ),    
    WIN_ID( MMISET_SET_DISPLAY_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SET_DISPLAY_OPT, MMISET_SET_DISPLAY_CTRL_ID),
    END_WIN
};

#else
WINDOW_TABLE( MMISET_SET_DISPLAY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayWindow ),    
        WIN_ID( MMISET_SET_DISPLAY_WIN_ID ),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_FORM_CTRL_ID),
#if 0//defined MMISET_ANIM_EFFECT_SUPPORT //delete by James li       
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_SET_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_DISPLAY_ANIM_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_SET_FORM_CTRL_ID),//动画
#endif
#if defined MMISET_WALLPAPER_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_WALLPAPER_LABEL_CTRL_ID,MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID),//墙纸
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID),
#endif
#ifndef  ZDT_PRODUCT_Z685S
#if defined MMISET_THEME_SUPPORT//delete by James li 
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_THEME_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_THEME_LABEL_CTRL_ID,MMISET_DISPLAY_THEME_FORM_CTRL_ID),//主题
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_THEME_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_THEME_FORM_CTRL_ID),
#endif    
#if defined MMISET_MAINMENU_EFFECT_SUPPORT//delete by James li 
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_MAINMENU_LABEL_CTRL_ID,MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID),//主菜单风格
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_MAINMENU_CONTENT_CTRL_ID,MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID), 
#endif 
#if defined MMISET_UIDLE_EFFECT_SUPPORT//delete by James li 
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_UIDLE_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_UIDLE_LABEL_CTRL_ID,MMISET_DISPLAY_UIDLE_FORM_CTRL_ID),//uidle
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_UIDLE_CONTENT_CTRL_ID,MMISET_DISPLAY_UIDLE_FORM_CTRL_ID), 
#endif 
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_MAINMENU_SELECT_STYLE_LABEL_CTRL_ID,MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID),//主菜单高亮风格
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_MAINMENU_SELECT_STYLE_CONTENT_CTRL_ID,MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID), 
#endif

#if defined MMISET_IDLE_STYLE_SUPPORT
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_IDLE_STYLE_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_IDLE_STYLE_LABEL_CTRL_ID,MMISET_DISPLAY_IDLE_STYLE_FORM_CTRL_ID),//待机风格
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID,MMISET_DISPLAY_IDLE_STYLE_FORM_CTRL_ID),
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_DISPLAY_IDLE_DISPLAY_LABEL_CTRL_ID,MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID),//待机设置
#endif
#endif   
#if defined MMISET_KEYLOCK_SUPPORT
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_CTRL_ID,MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID),//锁屏
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID),  
#endif
#if defined MMISET_SCREENSAVER_SUPPORT//delete by James li  
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_SCREEN_SAVERS_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_SCREEN_SAVERS_LABEL_CTRL_ID,MMISET_DISPLAY_SCREEN_SAVERS_FORM_CTRL_ID),//屏保
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_SCREEN_SAVERS_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_SCREEN_SAVERS_FORM_CTRL_ID),
#endif            
#endif
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_CONTRAST_CTRL_ID,MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID),//屏幕背光亮度
            CHILD_OWNDRAW_CTRL(TRUE,MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID,MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID,(uint32)MMISET_ContrastBarCtrlCallbackFun),

#if defined MMISET_BACKLIGHT_TIME_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_BACKLIGH_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_BACKLIGHT_CTRL_ID,MMISET_DISPLAY_BACKLIGH_FORM_CTRL_ID),//背光时间
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID,MMISET_DISPLAY_BACKLIGH_FORM_CTRL_ID),
#endif  
#if defined MMISET_KEYBACKLIGHT_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_KEYPAD_BACKLIGH_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_KEYPAD_BACKLIGHT_CTRL_ID,MMISET_DISPLAY_KEYPAD_BACKLIGH_FORM_CTRL_ID),//按键背光时间
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID,MMISET_DISPLAY_KEYPAD_BACKLIGH_FORM_CTRL_ID),
#endif

#ifdef ZDT_PRODUCT_Z685S //MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_AUTO_KEYPADLOCK_LABEL_CTRL_ID,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID),//AUTO KEYPAD LOCK
            CHILD_SETLIST_CTRL(TRUE,MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID),//AUTO KEYPAD LOCK
#endif

#ifdef ZDT_PRODUCT_Z685S
	 CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ENDKEY_LOCK_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ENDKEY_LOCK_CTRL_ID,MMISET_DISPLAY_ENDKEY_LOCK_FORM_CTRL_ID),//
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ENDKEY_LOCK_SETLIST_CTRL_ID,MMISET_DISPLAY_ENDKEY_LOCK_FORM_CTRL_ID),

#endif

#if 0
defined MMISET_FONT_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_FONT_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_FONT_SETTING_CTRL_ID,MMISET_DISPLAY_FONT_FORM_CTRL_ID),//字体设置
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_FONT_FORM_CTRL_ID),
#endif
#if 0
def MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_AUTO_KEYPADLOCK_LABEL_CTRL_ID,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID),//AUTO KEYPAD LOCK
            CHILD_SETLIST_CTRL(TRUE,MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID),//AUTO KEYPAD LOCK
#endif
        END_WIN
};
#endif

//blacklight
WINDOW_TABLE(MMISET_SET_DISPLAY_BLACKLIGHT_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleSetDisplayBlacklightWindow ),    
    WIN_ID( MMISET_SET_DISPLAY_BLACKLIGHT_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID),
    CHILD_PRGBOX_CTRL(TRUE, GUIPRGBOX_STYLE_BRIGHTNESS_BAR, MMISET_BLACKLIGHT_PRGBOX_CTRL_ID, MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID),    
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_DISPLAY_BACKLIGHT_TIMEOUT_LABLE_CTRL_ID, MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID),
    END_WIN
};

//font size
WINDOW_TABLE(MMISET_SET_DISPLAY_FONTSIZE_WIN_TAB)=
{
    WIN_FUNC( (uint32)HandleSetDisplayFontSizeWindow ),
    WIN_ID( MMISET_SET_DISPLAY_FONTSIZE_WIN_ID ),
    //modify by nhl for back button
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMISET_SET_DISPLAY_FONTSIZE_CTRL_ID),
    //CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_SET_DISPLAY_FONTSIZE_FORM_CTRL_ID),
    //CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SET_DISPLAY_FONTSIZE_CONTACTS_CTRL_ID, MMISET_SET_DISPLAY_FONTSIZE_FORM_CTRL_ID),
    //CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SET_DISPLAY_FONTSIZE_MENU_CTRL_ID, MMISET_SET_DISPLAY_FONTSIZE_FORM_CTRL_ID),
    //CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISET_SET_DISPLAY_FONTSIZE_MESSAGING_CTRL_ID, MMISET_SET_DISPLAY_FONTSIZE_FORM_CTRL_ID),
    END_WIN
};

//blacklight timeout
WINDOW_TABLE(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleSetDisplayBlacklightTimeOutWindow ),    
    WIN_ID( MMISET_DISPLAY_BACKLIGHT_TIMEOUT_WIN_ID ),
    //nhl mmodify back button
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMISET_DISPLAY_FONTSIZE_CHOICE_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleSetDisplayFontSizeChoiceWindow ),    
    WIN_ID( MMISET_DISPLAY_FONTSIZE_CHOICE_WIN_ID ),
    //modify by nhl for back button
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_DISPLAY_FONTSIZE_CHOICE_CTRL_ID),
    END_WIN
};
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void CreatTimeOutListWin(MMI_TEXT_ID_T text_id)
{
    uint32  i = 0;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    //GUILIST_SetMaxItem(MMISET_DISPLAY_CTRL_ID, 3, FALSE);
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_t.item_data_ptr = &item_data;
        //item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        //item_data.item_content[0].item_data.image_id = IMAGE_NULL;

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id;
                
        item_data.softkey_id[0] = TXT_NULL;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        
        GUILIST_AppendItem( MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, &item_t );      
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayBlacklightTimeOutWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    uint32 loop = 0;
    MMI_STRING_T backlight_name = {0};
    MMI_STRING_T string_to = {0};
    MMISET_BACKLIGHT_SET_T backlight = {0};
    uint16 index = 0;
    uint16 cur_select = 0;
    uint32 text_id[]=
    {
        STXT_BACKLIGHT_TIME_T10,
        STXT_BACKLIGHT_TIME_T20,
        STXT_BACKLIGHT_TIME_T40,
        STXT_BACKLIGHT_TIME_T60,
        STXT_BACKLIGHT_TIME_T120
#ifdef HW_LCD_TEST
        ,TXT_SET_TIME_OPEN_FOREVER
#endif
    };
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
		
// 	    MMI_GetLabelTextByLang(STR_BACKLIGHT_TIMEOUT, &string_to );		
// 	    if(MMIAPICOM_StatusAreaSetViewHeaderinfo(string_to.wstr_ptr, string_to.wstr_len))
// 	    {
// 	        GUIWIN_UpdateStb();
// 	    }
	    GUILIST_SetMaxItem(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, 6, FALSE);

	    for(loop=0;loop<ARR_SIZE(text_id);loop++)
	    {
	        CreatTimeOutListWin(text_id[loop]);
	    }
	    MMIAPISET_GetBackLightInfo(&backlight);
	    switch (backlight.backlight_type)
	    {
	    case MMISET_BACKLIGHT_OPEN:
	        switch (backlight.backlight_time)
	        {
	        case MMISET_TIME_10:
	            index = 0;
	            break;

	        case MMISET_TIME_20:
	            index = 1;
	            break;

	        case MMISET_TIME_40:
	            index = 2;
	            break;

	        case MMISET_TIME_60:
	            index = 3;
	            break;

	        case MMISET_TIME_120:
	            index = 4;
	            break;

	        default:
	            //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight time is %d error!"
	            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1486_112_2_18_2_51_1_173,(uint8*)"d",backlight.backlight_time);
	            break;
	        }
	        break;

	    case MMISET_BACKLIGHT_OPEN_FOREVER:
	        index = 0;
	        break;

	    case MMISET_BACKLIGHT_CLOSE:
	        //SCI_TRACE_LOW("MMISET_AppendBacklightListItem:backlight type is close!");
	        break;

	    default:
	        //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight type is %d error!"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1500_112_2_18_2_51_1_174,(uint8*)"d",backlight.backlight_type);
	        break;
	    }
        GUILIST_SetCurItemIndex(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, index);
        GUILIST_SetSelectedItem(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, index, TRUE);
	 GUILIST_SetListState(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID);
	break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    //case MSG_CTL_OK:
    case MSG_APP_WEB:
	    #if defined (MMISET_BACKLIGHT_TIME_SUPPORT)
	    MMISET_SetBackLight(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID);
	    MMIDEFAULT_TurnOnLCDBackLight();
	    #endif
	    MMK_CloseWin(win_id);
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
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayBlacklightWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{   
    uint8 data_buf[31] = {0};
    wchar wstr[31] = {0};
    int32 buff_length = 30;
    char* str = PNULL;
    MMI_STRING_T text = {0}; 
    MMI_STRING_T indensity_text = {0}; 
    MMI_STRING_T string_to = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_BG_T blacklight_bg = {0};
    uint32 menu_id= (uint32)MMK_GetWinAddDataPtr(win_id);
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    const GUI_RECT_T client_rect = MMITHEME_GetClientRect();
    GUI_RECT_T          prg_rect = client_rect;   
    uint16      prg_rect_interval_from_win_border = (client_rect.right - client_rect.left + 1)/20;
    uint16      prg_rect_height =  VOLUME_BRIGHTNESS_BAR_HEIGHT;//2//*GUIFONT_GetHeight(MMI_DEFAULT_TEXT_FONT);
    uint16      lab_rect_height =  GUIFONT_GetHeight(MMI_DEFAULT_TEXT_FONT);
    GUI_RECT_T lab_rect = client_rect;
    static int16 volume = 0;
    static int16 brightness = 0;
    wchar point = (wchar)(0x25CF);

    GUIFORM_CHILD_WIDTH_T   child_width = {0};
    GUIFORM_CHILD_HEIGHT_T  child_height = {0};
    MMI_CTRL_ID_T ctrl_id = MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID;
    MMI_HANDLE_T form_handle = MMK_GetCtrlHandleByWin(win_id, MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID);
	uint16	  v_space = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
	 GUIFORM_SetCircularHandleUpDown(MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID,TRUE);
	 GUIFORM_SetType(MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        GUIFORM_SetMargin(MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID, 0);
		
	 child_height.add_data = prg_rect_height;
	 child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
	 GUIFORM_SetChildHeight(MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID ,MMISET_BLACKLIGHT_PRGBOX_CTRL_ID, &child_height);
		
        prg_rect.bottom = prg_rect.top + prg_rect_height - 1;
        GUIPRGBOX_SetRect(MMISET_BLACKLIGHT_PRGBOX_CTRL_ID,&prg_rect);   
        
	 brightness = MMIAPISET_GetCurrentContrast();
        
        sprintf((char*)data_buf, "%ld", brightness - 1);
        
        str = (char*)data_buf;
        text.wstr_len = strlen(str);
        text.wstr_ptr = wstr;
        MMI_STRNTOWSTR( wstr, buff_length, (uint8*)str, text.wstr_len, text.wstr_len );

	 GUIPRGBOX_SetParamByCtrl(MMISET_BLACKLIGHT_PRGBOX_CTRL_ID, MMISET_DISPLAY_SET_BRIMAX, MMISET_DISPLAY_SET_BRIMAX);
        GUIPRGBOX_SetPosByCtrl(FALSE,brightness - 1,0,MMISET_BLACKLIGHT_PRGBOX_CTRL_ID);

        blacklight_bg.bg_type = GUI_BG_COLOR;
        blacklight_bg.color = MMI_BLACK_COLOR;
        blacklight_bg.shape = GUI_SHAPE_RECT;
        GUIFORM_SetBg(MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID,&blacklight_bg);
		CTRLFORM_SetDividingLine(MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID,0,MMI_BLACK_COLOR);
        GUIFORM_SetSpace(MMISET_DISPLAY_BLACKLIGHT_FORM_CTRL_ID, PNULL, &v_space);
	 MMK_SetAtvCtrl(win_id, MMISET_BLACKLIGHT_PRGBOX_CTRL_ID);
	 //nhl modify button back
	 //GUIWIN_SetSoftkeyTextId(MMISET_SET_DISPLAY_BLACKLIGHT_WIN_ID,  (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        break;
        
    case MSG_FULL_PAINT:
//         MMI_GetLabelTextByLang(STR_SETTINGS_BACKLIGHT_HEADER, &string_to );		
//         if(MMIAPICOM_StatusAreaSetViewHeaderinfo(string_to.wstr_ptr, string_to.wstr_len))
//         {
//             GUIWIN_UpdateStb();
//         }
	 SetBacklightTimeoutParam();
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    //case MSG_CTL_OK:
    case MSG_APP_WEB:
	 ctrl_id = MMK_GetActiveCtrlId(win_id);
	 if(ctrl_id == MMISET_DISPLAY_BACKLIGHT_TIMEOUT_LABLE_CTRL_ID)
	 {
		MMK_CreateWin((uint32*)MMISET_DISPLAY_BACKLIGHT_TIMEOUT_WIN_TAB,PNULL);
	 }

        break;
        
    case MSG_KEYDOWN_LEFT:
		ctrl_id = MMK_GetActiveCtrlId(win_id);
              if(ctrl_id == MMISET_BLACKLIGHT_PRGBOX_CTRL_ID)
		{
			if (brightness > 1)
			{
	            brightness -= 1;
		        MMIAPISET_Intensity(brightness);
	            GUIPRGBOX_SetPosByCtrl(TRUE,brightness-1,0,MMISET_BLACKLIGHT_PRGBOX_CTRL_ID);
        	}
		}
        break;
        
    case MSG_KEYDOWN_RIGHT:
		ctrl_id = MMK_GetActiveCtrlId(win_id);
              if(ctrl_id == MMISET_BLACKLIGHT_PRGBOX_CTRL_ID)
		{
			if (brightness < MMISET_DISPLAY_SET_BRIMAX)
			{
	            brightness += 1;
		        MMIAPISET_Intensity(brightness);
	            GUIPRGBOX_SetPosByCtrl(TRUE,brightness-1,0,MMISET_BLACKLIGHT_PRGBOX_CTRL_ID);
        	}
		}
        break;
    case MSG_CLOSE_WINDOW:
    	MMIAPISET_SetMainLCDContrast();
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Description : open phone setting window
//  Global resource dependence :
//  Note:
/*****************************************************************************/

WINDOW_TABLE( MMISET_BACKLIGHT_CR_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleSetBacklightCrWindow ),
    WIN_ID( MMISET_BACKLIGHT_CR_WIN_ID ),
//     WIN_TITLE(TXT_SET_SYSTEM),
    WIN_SOFTKEY(TXT_COMMON_CHOOSE, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_BACKLIGHT_CR_CTRL_ID),
    END_WIN
};
/*****************************************************************************/
// 	Description : 测试设定背光灯时间
//	Global resource dependence : 
//  Author: nhl
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetTestBackLight(
                                MMI_CTRL_ID_T   ctrl_id
                                )
{
    uint16                      cur_selection   =   0;
    MMISET_BACKLIGHT_SET_T      backlight_info  =   {0};

    SCI_MEMSET(&backlight_info, 0, sizeof(MMISET_BACKLIGHT_SET_T));

    //get the selected item
#ifdef DPHONE_SUPPORT 
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
#else
    //cur_selection = GUISETLIST_GetCurIndex(ctrl_id);
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
#endif 
    //SCI_ASSERT( (MMISET_BACKLIGHT_MAX_ITEM > cur_selection));
   if(MMISET_BACKLIGHT_MAX_ITEM <= cur_selection)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMISET_SetBackLight cur_selection %d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2003_112_2_18_2_50_55_147,(uint8*)"d", cur_selection);
	return;
   }
    
    switch (cur_selection)
    {

    
    case 0:
        backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
        backlight_info.backlight_time = MMISET_TIME_20;
        break;
	case 1:
			backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;
			break;

   
    default:
        break;
    }
    MMIAPISET_SetBackLightInfo(backlight_info);
}


/*****************************************************************************/
//  Description : open phone setting window
//  Global resource dependence :
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterBacklightSettingWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_BACKLIGHT_CR_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :HandleSetBacklightCrWindow
//  Global resource dependence :
//  Author:niu.huiling
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBacklightCrWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
   
      
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    
    MMI_CTRL_ID_T               ctrl_id = MMISET_SET_BACKLIGHT_CR_CTRL_ID;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMISET_BACKLIGHT_SET_T backlight = {0};
    uint16   cur_item = 0;
	uint16 index = 0;
    int i=0;
    MMI_TEXT_ID_T               str_id[] = {TXT_SET_TIME_20S,
        TXT_SET_TIME_OPEN_FOREVER
                                           };
    switch(msg_id)
    {
     case MSG_OPEN_WINDOW:
		MMK_SetAtvCtrl(win_id,ctrl_id);
	    GUILIST_SetMaxItem(ctrl_id,2, FALSE );
	    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
	    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
        item_t.item_data_ptr = &item_data;
		for(i = 0; i < 2; i++) {
	        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            
            item_data.item_content[0].item_data.text_id = str_id[i];  
            if(!GUILIST_InsertItem(ctrl_id, &item_t, i)) {
               break;
            }
		}
		GUILIST_AppendItem( ctrl_id, &item_t ); 
		
		MMIAPISET_GetBackLightInfo(&backlight);
	    switch (backlight.backlight_type)
	    {
	    case MMISET_BACKLIGHT_OPEN:
	        switch (backlight.backlight_time)
	        {
	        

	        case MMISET_TIME_20:
	            index = 0;
	            
	            break;

	        
	        default:
	            //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight time is %d error!"
	            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1486_112_2_18_2_51_1_173,(uint8*)"d",backlight.backlight_time);
	            break;
	        }
	        break;

	    case MMISET_BACKLIGHT_OPEN_FOREVER:
	        index = 1;
	        break;

	    default:
	        //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight type is %d error!"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1500_112_2_18_2_51_1_174,(uint8*)"d",backlight.backlight_type);
	        break;
	    }
		    GUILIST_SetCurItemIndex(ctrl_id, index);
        GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);

		break;
		
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
          #if defined (MMISET_BACKLIGHT_TIME_SUPPORT)
	    MMISET_SetTestBackLight(MMISET_SET_BACKLIGHT_CR_CTRL_ID);
	    #endif
	    MMK_CloseWin(win_id);
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

//add end 
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetFontSizeTxt(MMISET_FONT_SIZE_TYPE_E fontsize_type)
{
    MMI_TEXT_ID_T text_font_size = 0;
    MMISET_FONT_SIZE_T font_size = {0};
    MMIAPISET_GetFontSizeInfo(fontsize_type,&font_size);
    switch (font_size.font_size_index)
    {
    	case 0:
	    text_font_size = STR_ILIST_NORMAL_FONT_SETT;	
	    break;
	case 1:
	    text_font_size = STR_ILIST_SMSSE_BIG_FONT;		    
	    break;		
    }
    return text_font_size;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void SetDisplayFontSizeParam(void)
{
    MMI_STRING_T        text            = {0};
    MMI_STRING_T backlight_name = {0};
    MMISET_FONT_SIZE_TYPE_E font_type = 0;
    MMI_TEXT_ID_T text_normal_large = 0;
    uint16 index = 0;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    uint32 text_id[]=
    {
		STR_ILIST_CONTACTS_EXT01,
			STR_LIST_MENU_MENU,
			STR_LIST_MENU_MESSAGES
    };

    GUILIST_SetMaxItem(MMISET_SET_DISPLAY_FONTSIZE_CTRL_ID, 3, FALSE);
    //GUILIST_SetMaxItem(MMISET_SET_DISPLAY_FONTSIZE_MENU_CTRL_ID, 1, FALSE);
    //GUILIST_SetMaxItem(MMISET_SET_DISPLAY_FONTSIZE_MESSAGING_CTRL_ID, 1, FALSE);

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id[0];
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = GetFontSizeTxt(0);   
    GUILIST_AppendItem( MMISET_SET_DISPLAY_FONTSIZE_CTRL_ID, &item_t ); 
    //GUILIST_SetListState(MMISET_SET_DISPLAY_FONTSIZE_CONTACTS_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id[1];
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = GetFontSizeTxt(1);   
    GUILIST_AppendItem( MMISET_SET_DISPLAY_FONTSIZE_CTRL_ID, &item_t );
    //GUILIST_SetListState(MMISET_SET_DISPLAY_FONTSIZE_MENU_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id[2];
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = GetFontSizeTxt(2);   
    GUILIST_AppendItem( MMISET_SET_DISPLAY_FONTSIZE_CTRL_ID, &item_t );
    GUILIST_SetListState(MMISET_SET_DISPLAY_FONTSIZE_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void CreatFontSizeListWin(MMI_TEXT_ID_T text_id)
{
    uint32  i = 0;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    //GUILIST_SetMaxItem(MMISET_DISPLAY_CTRL_ID, 3, FALSE);
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
            
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem( MMISET_DISPLAY_FONTSIZE_CHOICE_CTRL_ID, &item_t );      
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayFontSizeChoiceWindow(
		                             MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    uint32 loop = 0;
    MMI_STRING_T string_to = {0};
    uint16 index = 0;
    uint16 cur_select = 0;
    //MMISET_FONT_SIZE_T font_size = {0};
    uint8 font_size = 0;
    uint32 text_id[]=
    {
        STR_ILIST_NORMAL_FONT_SETT,
            STR_ILIST_SMSSE_BIG_FONT
    };
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
//         MMI_GetLabelTextByLang(TXT_FONT_SIZE, &string_to );		
//         if(MMIAPICOM_StatusAreaSetViewHeaderinfo(string_to.wstr_ptr, string_to.wstr_len))
//         {
//             GUIWIN_UpdateStb();
//         }
        GUILIST_SetMaxItem(MMISET_DISPLAY_FONTSIZE_CHOICE_CTRL_ID, 2, FALSE);
        
        for(loop=0;loop<ARR_SIZE(text_id);loop++)
        {
            CreatFontSizeListWin(text_id[loop]);
        }
        
        font_size = MMIAPISET_GetCurrentFontSize();
        GUILIST_SetCurItemIndex(MMISET_DISPLAY_FONTSIZE_CHOICE_CTRL_ID, font_size);
        GUILIST_SetSelectedItem(MMISET_DISPLAY_FONTSIZE_CHOICE_CTRL_ID, font_size, TRUE);
        GUILIST_SetListState(MMISET_DISPLAY_FONTSIZE_CHOICE_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_FONTSIZE_CHOICE_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        //to do
        index = GUILIST_GetCurItemIndex(MMISET_DISPLAY_FONTSIZE_CHOICE_CTRL_ID);
        MMIAPISET_SetFontSize(index);
        MMK_CloseWin(win_id);
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
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayFontSizeWindow(
		                             MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    MMI_STRING_T string_to = {0};
    GUI_BG_T blacklight_bg = {0};
    int32 id = 0;
    MMI_CTRL_ID_T        current_ctrl_id = MMISET_SET_DISPLAY_FONTSIZE_CTRL_ID;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_SET_DISPLAY_FONTSIZE_CTRL_ID);
		//modify by nhl for back button
	 //GUIWIN_SetSoftkeyTextId(MMISET_SET_DISPLAY_FONTSIZE_WIN_ID,  (MMI_TEXT_ID_T)TXT_COMMON_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
	 break;

    case MSG_FULL_PAINT:
//         MMI_GetLabelTextByLang(STR_SMSSE_SETT_FONT_SIZE, &string_to );		
//         if(MMIAPICOM_StatusAreaSetViewHeaderinfo(string_to.wstr_ptr, string_to.wstr_len))
//         {
//             GUIWIN_UpdateStb();
//         }	
	  SetDisplayFontSizeParam();
		break;

    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
		id = GUILIST_GetCurItemIndex(current_ctrl_id);
		switch (id)
	        {
		     case 0:
			  s_font_size_type = 0;
	                MMK_CreateWin((uint32*)MMISET_DISPLAY_FONTSIZE_CHOICE_WIN_TAB, PNULL);
		         break;
		     case 1:
			  s_font_size_type = 1;
			  MMK_CreateWin((uint32*)MMISET_DISPLAY_FONTSIZE_CHOICE_WIN_TAB, PNULL);
			 break;
		     case 2:
			  s_font_size_type = 2;
			  MMK_CreateWin((uint32*)MMISET_DISPLAY_FONTSIZE_CHOICE_WIN_TAB, PNULL);
			 break;
		     default:
	                break;
		}
		break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
              break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
//2015-7-9 tangting modify end
//pclint 
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
// 	Description : set idle win display mode
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDisplaytIdleMode(void)
{
    //write information about idle win display set into NV
    MMINV_WRITE(MMINV_SET_IDLEDISPLAY_INFO,MMIAPISET_GetIdleDisplayMode());
}
#endif 
#endif
/*****************************************************************************/
//  Description : to handle display setting window message
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
#if defined DPHONE_SUPPORT 
LOCAL MMI_RESULT_E  HandleSetDisplayWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_MENU_GROUP_ID_T     group_id    =   0;
    MMI_MENU_ID_T           menu_id     =   0;
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_SET_DISPLAY_CTRL_ID); 
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_SET_DISPLAY_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
        case ID_SET_IDLE_DISPLAY:
            MMK_CreatePubListWin((uint32*)MMISET_SET_IDLE_DISPLAY_WIN_TAB,PNULL);
            break;
#endif
#endif
        case ID_SET_BACK_LIGHT:
            MMK_CreateWin((uint32*)MMISET_SET_BACKLIGHT_WIN_TAB,PNULL);
            break;
            
        case ID_SET_CONTRAST:
            MMK_CreateWin((uint32*)MMISET_SET_CONSTRAST_WIN_TAB,PNULL);
            break;       
        case ID_SET_KEY_BOARD_LIGHT:
            MMK_CreateWin((uint32*)MMISET_SET_KEYBOARD_LIGHT_WIN_TAB,PNULL);
            break; 
        default:
            //SCI_PASSERT(SCI_FALSE, ("HandleSetDisplayWindow! MSG_CTL_OK menu_id = %d", menu_id));
            break;
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
#else
LOCAL MMI_RESULT_E  HandleSetDisplayWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_DISPLAY_ANIM_LABEL_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_PDA_SUPPORT
		GUIFORM_SetCircularHandleUpDown(MMISET_DISPLAY_FORM_CTRL_ID,TRUE);
#endif
#if !defined(MMI_GUI_STYLE_TYPICAL) 
        GUIFORM_SetType(MMISET_DISPLAY_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT 
        GUIFORM_SetMargin(MMISET_DISPLAY_FORM_CTRL_ID, 0);
#if defined MMISET_ANIM_EFFECT_SUPPORT
        SetAnimLabelParam();
#endif
#endif
#if defined MMISET_WALLPAPER_SUPPORT
        SetWallPaperParam();
#endif
#if defined MMISET_THEME_SUPPORT
        SetThemeParam();
#endif   
#if defined MMISET_MAINMENU_EFFECT_SUPPORT
        if(1 < MMISET_PDA_MAINMENU_MAX)/*lint !e506 !e774*///可能存在恒为FALSE或恒为TRUE的情况
        {
            SetMainmenuEffectParam();
        }
        else
        {
            GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, 
                                    MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID, 
                                    GUIFORM_CHILD_DISP_HIDE);
        }
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        SetMainmenuSelectEffectParam();
#endif
#if defined MMISET_UIDLE_EFFECT_SUPPORT
        SetUIdleEffectParam();
#endif
#if defined MMISET_IDLE_STYLE_SUPPORT
        SetIdleStyleParam();
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
        SetIdleDisplaySettingParam();
#endif
#endif
#if defined MMISET_FONT_SUPPORT
        SetFontParam();
#endif    
#if defined MMISET_SCREENSAVER_SUPPORT 
        SetScreenSaverParam();
#endif
        SetContrastParam();
#if defined MMISET_BACKLIGHT_TIME_SUPPORT
        SetBacklightParam();
#endif
#if defined MMISET_KEYBACKLIGHT_SUPPORT
        SetKeyboardLightParam();
#endif

#if defined ZDT_PRODUCT_Z685S
        //SetKeyAUTOKEYPADParam();
	MMISET_SetAutoLockKeyPadList();

#endif

#if defined ZDT_PRODUCT_Z685S
        //SetKeyAUTOKEYPADParam();
	MMISET_SetENDKEYLOCKPadList();

#endif

#if defined MMISET_KEYLOCK_SUPPORT
        if( 1 < MMISET_KEY_LOCK_EFFECT_MAX )/*lint !e506 !e774*///可能存在恒为FALSE或恒为TRUE的情况
        {
            MMISET_SetKeyLockParam();
        }
        else
        {
            GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, 
                                    MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID, 
                                    GUIFORM_CHILD_DISP_HIDE);
        }
#endif
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
        MMISET_SetAutoLockKeyPadList();
#endif

#ifndef MMI_DISPLAYSET_MINI_SUPPORT 
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_FORM_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID);
#endif    
        GUIWIN_SetTitleTextId(win_id,TXT_SETTINGS_DISPLAY,FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);

        break;
        
    case MSG_GET_FOCUS:
#if defined MMISET_WALLPAPER_SUPPORT
        SetWallPaperParam();
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        SetMainmenuSelectEffectParam();
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
#endif
        {
            ctrl_id= MMK_GetActiveCtrlId(win_id);
        }
        
        switch (ctrl_id)
        {
#ifndef MMI_DISPLAYSET_MINI_SUPPORT  
#if defined MMISET_ANIM_EFFECT_SUPPORT
        case MMISET_DISPLAY_ANIM_LABEL_CTRL_ID:
#if 0//def QBTHEME_SUPPORT
            {
                MMISET_IDLE_STYLE_E cur_idle_style = MMIAPISET_GetIdleStyle();

                if (MMISET_IDLE_QBTHEME == cur_idle_style)
                {
                    MMK_CreateWin((uint32 *)MMISET_SET_QB_DISPLAY_ICON_ANIMATION_WIN_TAB, PNULL);
                    break;
                }
            }
#endif
            MMK_CreateWin((uint32 *)MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_TAB, PNULL);

#endif
            break;  
#endif
#if defined MMISET_WALLPAPER_SUPPORT
        case MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID:
           MMIAPISET_SetWallpaper();
            break;
#endif
//#ifdef MMI_MULTI_MAINMENU_SUPPORT
#if defined MMISET_MAINMENU_EFFECT_SUPPORT
        case MMISET_DISPLAY_MAINMENU_CONTENT_CTRL_ID:
            MMIAPISET_CreateMainmenuStyleWin();
            break;
#endif
//#endif
#if defined( PDA_UI_SUPPORT_U_IDLE)
        case MMISET_DISPLAY_UIDLE_CONTENT_CTRL_ID:
            MMIAPISET_CreateUIdleStyleWin();
            break;
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        case MMISET_MAINMENU_SELECT_STYLE_CONTENT_CTRL_ID:
            MMIAPISET_CreateMainmenuSelectStyleWin();
            break;
#endif 
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
        case MMISET_DISPLAY_IDLE_DISPLAY_LABEL_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_IDLE_DISPLAY_WIN_TAB,PNULL);
            break;
#endif
#endif
#if defined MMISET_KEYLOCK_SUPPORT
        case MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_EDIT_CTRL_ID:
            MMIAPISET_CreateLockStyleWin();//NEWMS00204601
            //MMISET_CreateKeyLockEffectWin();
            break;
#endif
#if defined MMISET_THEME_SUPPORT
        case MMISET_DISPLAY_THEME_LABEL_EDIT_CTRL_ID:
            MMISET_CreateThemePubWin();
            break;
#endif
#ifdef TOUCH_PANEL_SUPPORT
        case MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID:
            if (MSG_CTL_PENOK == msg_id)
            {
                GUI_POINT_T     tp_point = {0};
                MMI_TP_STATUS_E tp_status = MMI_TP_NONE;

                MMK_GetMultiTPMsg(&tp_status,&tp_point);

                MMISET_HandleContrastTPDown(&tp_point, 
                                            win_id,
                                            MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID,
                                            FALSE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif
#if defined MMISET_SCREENSAVER_SUPPORT 
        case MMISET_DISPLAY_SCREEN_SAVERS_LABEL_EDIT_CTRL_ID:
            MMISETAPI_CreateScreensaverWin();
            break;
#endif
#if defined MMISET_FONT_SUPPORT        
        case MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_FONT_WIN_TAB, PNULL);
            break;
  #endif
            
        default:
            //SCI_TRACE_LOW:"HandleSetDisplayWindow ctrl_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_862_112_2_18_2_51_0_170,(uint8*)"d",ctrl_id);
            break;
        }

        break;

    case MSG_NOTIFY_SETLIST_SWITCH:
        {
            MMI_CTRL_ID_T  act_ctrl_id = MMK_GetActiveCtrlId(win_id);

            switch(act_ctrl_id)
            {
#ifdef MMISET_KEYBACKLIGHT_SUPPORT
            case MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID:
                MMISET_SetKeyboardLight(act_ctrl_id);
                break;
#endif

#ifdef MMISET_BACKLIGHT_TIME_SUPPORT    
            case MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID:
                MMISET_SetBackLight(act_ctrl_id);
                break;
#endif

#ifdef ZDT_PRODUCT_Z685S ///MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
            case MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID:
                MMISET_SaveAutoKeyPadList();
				//MMIDEFAULT_StartLcdBackLightTimer();//CR140422
                break;
#endif
#ifdef ZDT_PRODUCT_Z685S //MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
			case MMISET_DISPLAY_ENDKEY_LOCK_SETLIST_CTRL_ID:
				{

						MMISET_RedKeyLock(act_ctrl_id);
				}
				break;
#endif

            default:
                break;
            }

        }
        break;
        
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_UP:
    case MSG_KEYLONG_UP:
    case MSG_KEYLONG_DOWN:
        if(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_FINISH, TRUE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, TRUE);
        }
        break;
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
        if(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            MMISET_TuneUpConstrast(msg_id);
            //display contrast picutres
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        SaveAllFeatureState();
        break;
            
    default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#endif
//2015-7-9 tangting modify setting-display ui
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:ting.tang
//  Note: 
/*****************************************************************************/

LOCAL void CreatDislayListWin(void)
{
    uint32  i = 0;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    GUILIST_SetMaxItem(MMISET_DISPLAY_CTRL_ID, 3, FALSE);
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_t.item_data_ptr = &item_data;
    for (i = 0; i < 2; i++)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = s_display_type_text_id[i];
                
        item_t.user_data = i;

	 //item_data.softkey_id[0] = TXT_COMMON_OK;
        //item_data.softkey_id[1] = TXT_COMMON_CHOOSE;
        //item_data.softkey_id[2] = STXT_RETURN;
        
        GUILIST_AppendItem( MMISET_DISPLAY_CTRL_ID, &item_t );      
    } 
    //item_t.item_style  = GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT;
    //item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    //item_data.item_content[0].item_data.text_id = s_display_type_text_id[2];
    //item_data.softkey_id[0] = TXT_COMMON_OK;
    //item_data.softkey_id[1] = TXT_COMMON_CHOOSE;
    //item_data.softkey_id[2] = STXT_RETURN;
    //item_t.user_data = 2;
    //GUILIST_AppendItem( MMISET_DISPLAY_CTRL_ID, &item_t );      
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:ting.tang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayListWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )

{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               current_ctrl_id = MMISET_DISPLAY_CTRL_ID;
    SET_DISPLAY_LIST_ID_E	 id = 0;
    MMI_STRING_T string_to={0};
    switch(msg_id)
   {
    case MSG_OPEN_WINDOW:
	 //CreatDislayListWin();
	  MMK_CreateWin((uint32*)MMISET_SET_DISPLAY_BLACKLIGHT_WIN_TAB, PNULL);
	 /*GUILIST_SetListState(MMISET_DISPLAY_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_CTRL_ID);
        break;
    case MSG_FULL_PAINT:
	 MMI_GetLabelTextByLang(TXT_SETTINGS_DISPLAY, &string_to );		
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(string_to.wstr_ptr, string_to.wstr_len))
        {
            GUIWIN_UpdateStb();
        }
	 break;
    //case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        id = GUILIST_GetCurItemIndex(current_ctrl_id);
        switch (id)
        {
	     case DISPLAY_BLACKLIGHT_ID:
                MMK_CreateWin((uint32*)MMISET_SET_DISPLAY_BLACKLIGHT_WIN_TAB, PNULL);
	         break;
	     //case DISPLAY_FONT_SIZE_ID:
	     //	  MMK_CreateWin((uint32*)MMISET_DISPLAY_FONTSIZE_CHOICE_WIN_TAB, PNULL);
	    // break;
	     //case DISPLAY_LOCKSCREEN_ID:
		 //break;
	     default:
                break;
	}*/
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
#if defined MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : Set Anim effect menu name 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetAnimLabelParam(void)
{
    MMI_STRING_T anim_displayname = {0};
    
    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_ANIMATION, &anim_displayname);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_LABEL_CTRL_ID, &anim_displayname, FALSE);
}
#endif
#if defined MMISET_WALLPAPER_SUPPORT
/*****************************************************************************/
//  Description : Set Wallpaper menu name 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetWallPaperParam(void)
{
    MMI_STRING_T title = {0};
    MMI_STRING_T wallpaper_label_name = {0};
    MMISET_WALLPAPER_SET_T wallpaper_info = {0};
    uint32 text_id[MMISET_WALLPAPER_LIST_MAX]=
    {
        TXT_SET_WALLPAPER_1,
        TXT_SET_WALLPAPER_2, 
#ifndef MMI_SETTING_CUTDOWN_WALLPAPER 
        TXT_SET_WALLPAPER_3,
        TXT_SET_WALLPAPER_4,
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT            
#ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
        TXT_SET_WALLPAPER_LINEAR,         //线性光幕动态墙纸
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_POP
        TXT_SET_WALLPAPER_POP,         //POP动态墙纸
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_BEE
        TXT_SET_WALLPAPER_BEE,         //BEE动态墙纸
#endif

#ifdef LIVE_WALLPAPER_SUPPORT_GALAXY
        TXT_SET_WALLPAPER_GALAXY,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
        TXT_SET_WALLPAPER_SAKURA,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
        TXT_SET_WALLPAPER_SEAWORLD,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LIGHTGRID
        TXT_SET_WALLPAPER_LIGHTGRID,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RIPPLE
        TXT_SET_WALLPAPER_RIPPLE,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION
        TXT_SET_WALLPAPER_COLLISION,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LITTLECAT
        TXT_SET_WALLPAPER_LITTLECAT,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SWEETTREE
        TXT_SET_WALLPAPER_SWEETTREE,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON
        TXT_SET_WALLPAPER_COLLISION_BALLOON,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_MENU
        TXT_SET_WALLPAPER_COLLISION_MENU,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_WINDMILL
        TXT_SET_WALLPAPER_WINDMILL,                           //吹风车
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RAINBOWCITY
        TXT_SET_WALLPAPER_RAINBOWCITY,                   //彩虹城市
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_PAPERTOWN
        TXT_SET_WALLPAPER_PAPERTOWN,                     //卡通小镇
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_AQUARIUM
        TXT_SET_WALLPAPER_AQUARIUM,                     //水族馆
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_GIRLSWING
	TXT_SET_WALLPAPER_GIRLSWING,                //童话世界
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SPRING
        TXT_SET_WALLPAPER_SPRING,         //
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_NEST
        TXT_SET_WALLPAPER_NEST,         //
#endif

#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT            
        TXT_SET_WALLPAPER_VIDEO,
#endif 
        TXT_SET_WALLPAPER_MORE
    };

    GUIFORM_SetStyle(MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_COMMON_SET_WALLPAPER, &title);
    GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_CTRL_ID, &title, FALSE);
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    if (MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type)
    {
      //  if (MMISET_WALLPAPER_DEFAULT_ID <=wallpaper_info.wallpaper_id && wallpaper_info.wallpaper_id <= MMISET_WALLPAPER_FIXED_PIC_MAX_NUM)/*lint !e685 !e568*/ /*Bug 1090849*/
	if (wallpaper_info.wallpaper_id <= MMISET_WALLPAPER_FIXED_PIC_MAX_NUM)/*lint !e685 !e568*/ 
	{
            MMI_TEXT_ID_T txt_id = text_id[wallpaper_info.wallpaper_id];
            
#ifdef QBTHEME_SUPPORT
            if (MMISET_WALLPAPER_DEFAULT_ID != wallpaper_info.wallpaper_id
                && STYLE_IS_SUPPORT_QBTHEME)
            {
                txt_id = TXT_QB_THEME_WALLPAPER;
            }
#endif
            MMI_GetLabelTextByLang(txt_id, &wallpaper_label_name);
        }
    }
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT        	
    else if (MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type)
    {
        #ifdef SLIDESHOW_WALLPAPER_SUPPORT
        if(MMI_LIVE_WALLPAPER_TYPE_SLIDESHOW == wallpaper_info.wallpaper_id)
        {
            MMI_GetLabelTextByLang(TXT_SET_WALLPAPER_SLIDESHOW, &wallpaper_label_name);
        }
        else
		#endif
        {
            MMI_GetLabelTextByLang(text_id[MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + wallpaper_info.wallpaper_id], &wallpaper_label_name);
        }
        
    }
#endif  
#ifdef MMI_VIDEOWALLPAPER_SUPPORT    
    else if (MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type)
    {
        MMI_GetLabelTextByLang(TXT_SET_WALLPAPER_VIDEO, &wallpaper_label_name);
    }
#endif 
    else if(MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type)
    {
        
//        MMIAPIFMM_GetWallPaperInfo(wallpaper_file_ptr);

//        wallpaper_label_name.wstr_ptr = wallpaper_file_ptr->full_path_name;
//        wallpaper_label_name.wstr_len = wallpaper_file_ptr->full_path_len;
		MMI_GetLabelTextByLang(TXT_MORE_PICTURE, &wallpaper_label_name);
    }
    
    GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);
}
#endif

/*****************************************************************************/
//  Description : 供外部使用的设置墙纸label值的接口
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPISET_SetWallPaperName(void)
{
#if defined MMISET_WALLPAPER_SUPPORT
    SetWallPaperParam();
#endif
}  
#if defined MMISET_THEME_SUPPORT
/*****************************************************************************/
//  Description : set theme content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetThemeParam(void)
{
    MMI_STRING_T theme_title_name = {0};
    MMI_STRING_T itemStr={0};
    uint16 index =0;
    uint32 text_id[]=
    {
        TXT_SET_THEME_1,
        TXT_SET_THEME_2
    };

    GUIFORM_SetStyle(MMISET_DISPLAY_THEME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(STR_SETT_LIST_THEME_EXT01, &theme_title_name);
    GUILABEL_SetText(MMISET_DISPLAY_THEME_LABEL_CTRL_ID, &theme_title_name, FALSE);
    
    index = MMIAPISET_GetCurrentStaticThemeID();
    
    MMI_GetLabelTextByLang(text_id[index],&itemStr);
    GUILABEL_SetText(MMISET_DISPLAY_THEME_LABEL_EDIT_CTRL_ID, &itemStr, FALSE);
}
#endif
#if defined MMISET_IDLE_STYLE_SUPPORT
/*****************************************************************************/
//  Description : set idle style content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleStyleParam(void)
{
    MMI_STRING_T idle_label_name = {0};
    MMISET_IDLE_STYLE_E     idle_style  =   MMISET_IDLE_STYLE_COMMON;
    uint16 index = 0;
    uint32 loop = 0;
    uint32 text_id[MMISET_IDLE_STYLE_MAX]=
    {
#ifndef MMI_PDA_SUPPORT    
        TXT_SET_IDLE_COMMON,
        TXT_SET_IDLE_SMART,
#ifndef BAR_PHONE_WIDGET 
#ifdef MMIWIDGET_SUPPORT    
        TXT_SET_IDLE_WIDGET,
#endif
#ifdef MMI_GRID_IDLE_SUPPORT
        TXT_SET_IDLE_WIDGET,
#endif
#endif
#else    
    #ifdef MMI_GRID_IDLE_SUPPORT
        TXT_SET_IDLE_WIDGET,
    #endif

        TXT_SET_MAINMENU_SLIDE,
#endif

    };

    GUIFORM_SetStyle(MMISET_DISPLAY_IDLE_STYLE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
#if defined (MMI_GUI_STYLE_MINISCREEN)  
    MMI_GetLabelTextByLang(TXT_IDLE_STYLE_SWITH, &idle_label_name);    
#else
    MMI_GetLabelTextByLang(TXT_IDLE_SWITH, &idle_label_name);
#endif
    GUILABEL_SetText(MMISET_DISPLAY_IDLE_STYLE_LABEL_CTRL_ID, &idle_label_name, FALSE);
    
    for(loop=0;loop<MMISET_IDLE_STYLE_MAX;loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID, text_id[loop]);
    }
    idle_style = MMIAPISET_GetIdleStyle();
    switch (idle_style)
    {
#ifndef MMI_PDA_SUPPORT    
    case MMISET_IDLE_STYLE_COMMON:
        index = 0;
        break;
        
    case MMISET_IDLE_STYLE_SMART:
        index = 1;
        break; 
        
    #ifdef MMIWIDGET_SUPPORT
    case MMISET_IDLE_STYLE_WIDGET:
        index = 2;
        break;
    #endif
        
    #ifdef MMI_GRID_IDLE_SUPPORT
    case MMISET_IDLE_STYLE_GRID:
        #ifdef BAR_PHONE_WIDGET 
        {
            MMISET_IDLE_STYLE_E pre_idle_style = MMIAPISET_GetPreIdleStyle();
            if(pre_idle_style == MMISET_IDLE_STYLE_COMMON)
            {
                index = 0;
            }
            else
            {
                index = 1;
            }
            
        }
        #else
        index = 2;
        #endif        
        break;
    #endif 
#else
    #ifdef MMI_GRID_IDLE_SUPPORT
    case MMISET_IDLE_STYLE_GRID:
        index = 0;
        break;
    #endif
#if defined MMI_ISTYLE_SUPPORT 
    case MMISET_IDLE_ISTYLE:
        index = 1;
        break;
#endif		
#endif
        
    default:
        //SCI_TRACE_LOW:"HandleIdleMsg:idle_style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1366_112_2_18_2_51_1_172,(uint8*)"d",idle_style);
        break;
    }
    GUISETLIST_SetCtrlState(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID, index );
#if defined (MMI_DISPLAYSET_MINI_SUPPORT)   
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID,TXT_IDLE_STYLE_SWITH);   
#else
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID,TXT_IDLE_SWITH);    
#endif
}
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
//  Description : set idle style content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleDisplaySettingParam(void)
{
    MMI_STRING_T idle_display_setting_name = {0};

    //istyle下隐藏
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if ( MMITHEME_IsIstyle() )
    {
        GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        return;
    }
//add by James li begin
#else
    GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    return;
//add by James li end
#endif
    
    GUIFORM_SetStyle(MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_IDLE_DISPLAY, &idle_display_setting_name);
    GUILABEL_SetText(MMISET_DISPLAY_IDLE_DISPLAY_LABEL_CTRL_ID, &idle_display_setting_name, FALSE);
}
#endif
#endif
/*****************************************************************************/
//  Description : set contrast lable name
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetContrastParam(void)
{
    MMI_STRING_T contrast_name = {0};
    uint16    bar_width = 0;
    uint16    bar_height = 0;
    GUIFORM_SetStyle(MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_CONTRAST, &contrast_name);
    GUILABEL_SetText(MMISET_DISPLAY_CONTRAST_CTRL_ID, &contrast_name, FALSE); 
    GUIRES_GetImgWidthHeight(&bar_width,
        &bar_height,
        IMAGE_SET_CONTRAST_BAR_FG,
        MMISET_SET_DISPLAY_WIN_ID);
    GUIOWNDRAW_SetHeight(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID, (bar_height + 10));
}

#if defined MMISET_BACKLIGHT_TIME_SUPPORT
/*****************************************************************************/
//  Description : set backlight param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetBacklightParam(void)
{
    MMI_STRING_T backlight_name = {0};
    MMISET_BACKLIGHT_SET_T backlight = {0};
    uint16 index = 0;
    GUI_BG_T  bg_id = {0};
    uint32 loop = 0;
    uint32 text_id[]=
    {
        TXT_SET_TIME_OPEN_FOREVER,
        TXT_SET_TIME_30S,
        TXT_SET_TIME_20S,
        TXT_SET_TIME_15S,
        TXT_COMM_10SECONDS,
        TXT_SET_TIME_5S
    };
    GUIFORM_SetStyle(MMISET_DISPLAY_BACKLIGH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

//#ifdef MMI_PDA_SUPPORT || #ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
//#endif

    bg_id.img_id = IMAGE_COMMON_BG;
    MMI_GetLabelTextByLang(STR_SETTINGS_BACKLIGHT_HEADER, &backlight_name);
    GUILABEL_SetText(MMISET_DISPLAY_BACKLIGHT_CTRL_ID, &backlight_name, FALSE);
    
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID, text_id[loop]);
    }

    MMIAPISET_GetBackLightInfo(&backlight);
    switch (backlight.backlight_type)
    {
    case MMISET_BACKLIGHT_OPEN:
        switch (backlight.backlight_time)
        {
        case MMISET_TIME_30:
            index = 1;
            break;

        case MMISET_TIME_20:
            index = 2;
            break;

        case MMISET_TIME_15:
            index = 3;
            break;

        case MMISET_TIME_10:
            index = 4;
            break;

        case MMISET_TIME_5:
            index = 5;
            break;

        default:
            //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight time is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1486_112_2_18_2_51_1_173,(uint8*)"d",backlight.backlight_time);
            break;
        }
        break;

    case MMISET_BACKLIGHT_OPEN_FOREVER:
        index = 0;
        break;

    case MMISET_BACKLIGHT_CLOSE:
        //SCI_TRACE_LOW("MMISET_AppendBacklightListItem:backlight type is close!");
        break;

    default:
        //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight type is %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1500_112_2_18_2_51_1_174,(uint8*)"d",backlight.backlight_type);
        break;
    }
    GUISETLIST_SetCtrlState(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);  
    GUISETLIST_SetBackGround(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID, &bg_id);
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID, index );
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID,STR_SETTINGS_BACKLIGHT_HEADER); 
}
#endif

//2015-7-9 tangting modify settings-display ui
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:ting.tang
//  Note: 
/*****************************************************************************/
LOCAL void SetBacklightTimeoutParam(void)
{
    MMI_STRING_T        text            = {0};
    MMI_STRING_T backlight_name = {0};
    MMI_TEXT_ID_T text_time_out = 0;
    MMISET_BACKLIGHT_SET_T backlight = {0}; 
    uint16 index = 0;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/

    GUILIST_SetMaxItem(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_LABLE_CTRL_ID, 1, FALSE);
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_BACKLIGHT_TIMEOUT;
                
    //item_data.softkey_id[0] = TXT_NULL;
    //item_data.softkey_id[1] = TXT_NULL;
    //item_data.softkey_id[2] = STXT_RETURN;
        
    MMIAPISET_GetBackLightInfo(&backlight);
    switch (backlight.backlight_type)
    {
    case MMISET_BACKLIGHT_OPEN:
        switch (backlight.backlight_time)
        {
        case MMISET_TIME_10:
            index = 0;
	     //MMI_GetLabelTextByLang(TXT_SET_TIME_30S, &text_time_out);
	     text_time_out = STXT_BACKLIGHT_TIME_T10;
            break;

        case MMISET_TIME_20:
            index = 1;
	     //MMI_GetLabelTextByLang(TXT_SET_TIME_20S, &text_time_out);
	     text_time_out = STXT_BACKLIGHT_TIME_T20;
            break;

        case MMISET_TIME_40:
            index = 2;
	     //MMI_GetLabelTextByLang(TXT_SET_TIME_15S, &text_time_out);
	     text_time_out = STXT_BACKLIGHT_TIME_T40;
            break;

        case MMISET_TIME_60:
            index = 3;
	     //MMI_GetLabelTextByLang(TXT_COMM_10SECONDS, &text_time_out);
	     text_time_out = STXT_BACKLIGHT_TIME_T60;
            break;

        case MMISET_TIME_120:
            index = 4;
	     //MMI_GetLabelTextByLang(TXT_SET_TIME_5S, &text_time_out);
	     text_time_out = STXT_BACKLIGHT_TIME_T120;
            break;

        default:
            //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight time is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1486_112_2_18_2_51_1_173,(uint8*)"d",backlight.backlight_time);
            break;
        }
        //GUILABEL_SetText(MMIALM_DISPLAY_BACKLIGHT_TIMEOUT_TYPE_CTRL_ID, &text_time_out, FALSE); // text	 
        break;

    case MMISET_BACKLIGHT_OPEN_FOREVER:
	     text_time_out = TXT_SET_TIME_OPEN_FOREVER;
        index = 0;
        break;

    case MMISET_BACKLIGHT_CLOSE:
        //SCI_TRACE_LOW("MMISET_AppendBacklightListItem:backlight type is close!");
        break;

    default:
        //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight type is %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1500_112_2_18_2_51_1_174,(uint8*)"d",backlight.backlight_type);
        break;
    }
     item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
     item_data.item_content[1].item_data.text_id = text_time_out;
	 GUILIST_AppendItem( MMISET_DISPLAY_BACKLIGHT_TIMEOUT_LABLE_CTRL_ID, &item_t );
 	 GUILIST_SetListState(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_LABLE_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE);
}

/*****************************************************************************/
//  Description : set font content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
#if defined MMISET_FONT_SUPPORT
LOCAL void SetFontParam(void)
{
    MMI_STRING_T title_name = {0};
    MMI_STRING_T font_name ={0};
    wchar font[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};

    GUIFORM_SetStyle(MMISET_DISPLAY_FONT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_FONT, &title_name);
    GUILABEL_SetText(MMISET_DISPLAY_FONT_SETTING_CTRL_ID, &title_name, FALSE);
    GetFontInfo(font);
    if( 0 == *font)
    {
        MMI_GetLabelTextByLang(TXT_SET_DEFAULT_FONT, &font_name);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SET_FONT_MORE, &font_name);
    }
    GUILABEL_SetText(MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID, &font_name, FALSE);
}
#endif


/*****************************************************************************/
//  Description : set ScreenSaver content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
#if defined MMISET_SCREENSAVER_SUPPORT 
PUBLIC void SetScreenSaverParam(void)
{
    MMI_STRING_T screensaver_title= {0};
    MMI_STRING_T screensaver_name= {0};   
    MMISET_SCREENSAVER_INFO_T  ssaver_info = {0};
    uint32 text_id[MMISET_SCREENSAVER_MAX_NUM]=
    {
        TXT_CLOSE,
        TXT_SET_SCREENSAVER_SEA,
        TXT_SET_SCREENSAVER_STAR,
        TXT_SET_SCREENSAVER_SNAKE,
        TXT_SET_SCREENSAVER_SEED,
        TXT_SET_SCREENSAVER_PICTURE,
#ifdef VIDEO_PLAYER_SUPPORT 
        TXT_SET_SCREENSAVER_ANIMATION
#endif
    };

    GUIFORM_SetStyle(MMISET_DISPLAY_SCREEN_SAVERS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_SET_SCREENSAVER, &screensaver_title);
    GUILABEL_SetText(MMISET_DISPLAY_SCREEN_SAVERS_LABEL_CTRL_ID, &screensaver_title, FALSE);
    MMIAPISET_GetScreensaverInfo(&ssaver_info);
    if(ssaver_info.ssaver_type <=MMISET_SCREENSAVER_SEED)
    {
        MMI_GetLabelTextByLang(text_id[ssaver_info.ssaver_type], &screensaver_name);
    }
    else
    {

        if(ssaver_info.ssaver_type == MMISET_SCREENSAVER_MORE_PICTURE)
		{
            if (0 == ssaver_info.more_pic_saver_info.name_wstr_len)
            {
                MMI_GetLabelTextByLang(TXT_SET_SCREENSAVER_PICTURE, &screensaver_name);
            }
            else
            {
                screensaver_name.wstr_ptr = ssaver_info.more_pic_saver_info.name_wstr;
                screensaver_name.wstr_len = ssaver_info.more_pic_saver_info.name_wstr_len;
            }
		}
#ifdef VIDEO_PLAYER_SUPPORT
        else if(ssaver_info.ssaver_type == MMISET_SCREENSAVER_MORE_ANIMATION)
		{
            if (0 == ssaver_info.more_ani_saver_info.name_wstr_len)
            {
                MMI_GetLabelTextByLang(TXT_SET_SCREENSAVER_ANIMATION, &screensaver_name);
            }
            else
            {
                screensaver_name.wstr_ptr = ssaver_info.more_ani_saver_info.name_wstr;
                screensaver_name.wstr_len = ssaver_info.more_ani_saver_info.name_wstr_len;
            }
        }
#endif
    }
    GUILABEL_SetText(MMISET_DISPLAY_SCREEN_SAVERS_LABEL_EDIT_CTRL_ID, &screensaver_name, FALSE);       
}
#endif

#if defined MMISET_KEYBACKLIGHT_SUPPORT
/*****************************************************************************/
//  Description : set Keyboard light param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetKeyboardLightParam(void)
{
    MMI_STRING_T keyboardlight_name = {0};
    MMISET_KEYBOARD_LIGHT_SET_T   keyboard_info = {0};
    uint16 index = 0;
    uint32 loop = 0;
    uint32 text_id[]=
    {
        TXT_SET_TIME_20S,
        TXT_SET_TIME_10S,
        TXT_SET_TIME_5S,
        TXT_NIGHT_MODE,
        STR_POC_ILIST_OFF
    };
    GUIFORM_SetStyle(MMISET_DISPLAY_KEYPAD_BACKLIGH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif

    MMI_GetLabelTextByLang(TXT_SET_KEY_BOARD_LIGHT, &keyboardlight_name);
    GUILABEL_SetText(MMISET_DISPLAY_KEYPAD_BACKLIGHT_CTRL_ID, &keyboardlight_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID, text_id[loop]);
    }
    
    MMIAPISET_GetKeyBoardLightTime(&keyboard_info);
    //get current keyboard light
    if (MMISET_KEYBOARD_LIGHT_OPEN == keyboard_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_20 == keyboard_info.keyboard_light_time)
    {
        index = 0;
    }
    else if (MMISET_KEYBOARD_LIGHT_OPEN == keyboard_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_10 == keyboard_info.keyboard_light_time)
    {
        index = 1;
    }
    else if (MMISET_KEYBOARD_LIGHT_OPEN == keyboard_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_5 == keyboard_info.keyboard_light_time)
    {
        index = 2;
    }
    else if (MMISET_KEYBOARD_LIGHT_AUTO == keyboard_info.keyboard_type)
    {
        index = 3;      
    }
    else
    {
        index = 4;
    }
    GUISETLIST_SetCtrlState(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID, index );
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID,TXT_SET_KEY_BOARD_LIGHT); 
}
#endif

#if defined ZDT_PRODUCT_Z685S
/*****************************************************************************/
//  Description : set Keyboard light param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetKeyAUTOKEYPADParam(void)
{
	uint32 loop = 0;
	uint16 cur_selection = 0;
	MMI_STRING_T label_name  = {0};

	uint32 text_id[]=
	{
				TXT_CLOSE,
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
#else
				TXT_SET_TIME_5S,
#endif
				TXT_SET_TIME_15S,
				TXT_SET_TIME_30S,
				TXT_COMM_1_MINUTE,
				TXT_COMM_5_MINUTES
	};
	
	GUIFORM_SetStyle(MMISET_DISPLAY_AUTOKEYPAD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	GUISETLIST_SetFontAlign(MMISET_DISPLAY_AUTOKEYPAD_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);

#ifdef TOUCH_PANEL_SUPPORT
	MMI_GetLabelTextByLang(TXT_SET_AUTO_LOCK_SCREEN, &label_name);
#else
	MMI_GetLabelTextByLang(TXT_SET_AUTO_BOADRKEY, &label_name);
#endif
	GUILABEL_SetText(MMISET_DISPLAY_AUTOKEYPAD_CTRL_ID, &label_name, FALSE);
	for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
		GUISETLIST_AddItemById(MMISET_DISPLAY_AUTOKEYPAD_SETLIST_CTRL_ID, text_id[loop]);
	}

	switch (MMIAPISET_GetAutoBoardKey())
	{
	case MMISET_KEYLOCK_CLOSE:
		cur_selection = (uint16)MMISET_KEYLOCK_SELECT_CLOSE;
		break;
	
#if !defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
	case MMISET_KEYLOCK_5_SEC:
		cur_selection = (uint16)MMISET_KEYLOCK_SELECT_5_SEC;
		break;
#endif            
	case MMISET_KEYLOCK_15_SEC:
		cur_selection = (uint16)MMISET_KEYLOCK_SELECT_15_SEC;
		break;
	
	case MMISET_KEYLOCK_30_SEC:
		cur_selection = (uint16)MMISET_KEYLOCK_SELECT_30_SEC;
		break;
	
	case MMISET_KEYLOCK_1_MIN:
		cur_selection = (uint16)MMISET_KEYLOCK_SELECT_1_MIN;
		break;
		
	case MMISET_KEYLOCK_5_MIN:
		cur_selection = (uint16)MMISET_KEYLOCK_SELECT_5_MIN;
		break;
	default:
		SCI_TRACE_LOW("AppendAutoKeylockTimeListItem:keylock time is error!");
		break;
	}
#ifdef MMI_PDA_SUPPORT
	GUISETLIST_SetTitleTextId(MMISET_DISPLAY_AUTOKEYPAD_SETLIST_CTRL_ID,TXT_SET_AUTO_LOCK_SCREEN);
#else
	GUISETLIST_SetTitleTextId(MMISET_DISPLAY_AUTOKEYPAD_SETLIST_CTRL_ID,TXT_SET_AUTO_BOADRKEY);
#endif
	GUISETLIST_SetCurIndex(MMISET_DISPLAY_AUTOKEYPAD_SETLIST_CTRL_ID, cur_selection);
	GUISETLIST_SetCtrlState(MMISET_DISPLAY_AUTOKEYPAD_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//forcr226574

}

#endif


#if defined ZDT_PRODUCT_Z685S
/*****************************************************************************/
//  Description : set Keyboard light param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void MMISET_SetENDKEYLOCKPadList(void)
{
	uint32 loop = 0;
	uint16 cur_selection = 0;
	MMI_STRING_T label_name  = {0};

	uint32 text_id[]=
	{
				TXT_CLOSE,
				TXT_OPEN
	};
	
	GUIFORM_SetStyle(MMISET_DISPLAY_ENDKEY_LOCK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	GUISETLIST_SetFontAlign(MMISET_DISPLAY_ENDKEY_LOCK_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);

#ifdef TOUCH_PANEL_SUPPORT
	MMI_GetLabelTextByLang(TXT_SET_RED_KEYLOCK, &label_name);
#else
	MMI_GetLabelTextByLang(TXT_SET_RED_KEYLOCK, &label_name);
#endif
	GUILABEL_SetText(MMISET_DISPLAY_ENDKEY_LOCK_CTRL_ID, &label_name, FALSE);
	for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
		GUISETLIST_AddItemById(MMISET_DISPLAY_ENDKEY_LOCK_SETLIST_CTRL_ID, text_id[loop]);
	}

	if (MMIAPISET_GetRedKeyLock())
	{
		cur_selection = 1;
	}
	else
	{
		cur_selection =0;
	}
#ifdef MMI_PDA_SUPPORT
	GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ENDKEY_LOCK_SETLIST_CTRL_ID,TXT_SET_RED_KEYLOCK);
#else
	GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ENDKEY_LOCK_SETLIST_CTRL_ID,TXT_SET_RED_KEYLOCK);
#endif
	GUISETLIST_SetCurIndex(MMISET_DISPLAY_ENDKEY_LOCK_SETLIST_CTRL_ID, cur_selection);
	GUISETLIST_SetCtrlState(MMISET_DISPLAY_ENDKEY_LOCK_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);//forcr226574

}

#endif


#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : to handle display setting animation window message
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDisplayAnimationWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMISET_DISPLAY_ANIM_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        GUIFORM_SetMargin(MMISET_DISPLAY_ANIM_FORM_CTRL_ID, 0);
//#ifndef MMI_PDA_SUPPORT
//        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID); 
//#else
//        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_ANIM_FORM_CTRL_ID); 
//#endif
#ifdef SET_3D_STYLE_SUPPORT 
        Set3D_EffectParam();
#endif
#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
		SetMainmenu_EffectParam();
#endif
#endif
#if !defined MMI_PDA_SUPPORT && defined(EFFECT_3D_LIST_ROLL_SUPPORT)
        SetListEffectParam();
#endif
#ifdef EFFECT_LCD_ROTATE_SUPPORT
        SetGsensor_EffectParam();
#endif
        SetPowerOnEffectParam();
        SetPowerOffEffectParam();
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER) 
        SetRenderEffectParam();
#endif
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_ANIM_FORM_CTRL_ID); 
		GUIFORM_SetCircularHandleUpDown(MMISET_DISPLAY_ANIM_FORM_CTRL_ID, TRUE);
		GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
        break;

    case MSG_GET_FOCUS:
        SaveAllAnimSet();
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        if (MSG_CTL_MIDSK == msg_id)/*lint !e774 */
        {
            ctrl_id= MMK_GetActiveCtrlId(win_id);
        }
#ifdef TOUCH_PANEL_SUPPORT
        else
        {
            ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        }
#endif
        switch (ctrl_id)
        {
        case MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_POWER_ON_ANIM_WIN_TAB,PNULL);           
            break;
            
        case MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_POWER_OFF_ANIM_WIN_TAB,PNULL);
            break;

#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
        case MMISET_DISPLAY_ANIM_RENDER_LABEL_EDIT_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_RENDER_EFFECT_WIN_TAB,PNULL);
            break;
#endif
        default:
            //SCI_TRACE_LOW:"HandleSetDisplayAnimationWindow ctrl_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1744_112_2_18_2_51_2_175,(uint8*)"d",ctrl_id);
            break;
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMK_SendMsg(MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_ID, MSG_APP_WEB, PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        SaveAllAnimSet();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        SaveAllAnimSet();
        recode = MMI_RESULT_FALSE;
        break;
            
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
/*****************************************************************************/
//  Description : to handle display setting go animation window message
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetLightShadowAnimationWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{
    MMI_RESULT_E                      recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   check_list_id = MMISET_DISPLAY_ANIM_RENDER_CHECKLIST_CTRL_ID;
    uint32 i = 0;
    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMISET_EFFECT_RENDER_SETTING* render_setting_ptr = PNULL;
            //init list string
            GUILIST_SetMaxItem(check_list_id, MMISET_EFFECT_RENDER_SETTING_MAX, FALSE );//max item 4
            GUILIST_SetMaxSelectedItem(check_list_id, MMISET_EFFECT_RENDER_SETTING_MAX);

            /*第一个ITEM*/
            MMIAPISET_AppendListItemByTextId(TXT_SET_GO_MENU_ANIMATION, STXT_SELECT, check_list_id, GUIITEM_STYLE_ONE_LINE_CHECK);
            render_setting_ptr = MMISET_GetLightShadowRenderSetting();

            for(i = 0;i < MMISET_EFFECT_RENDER_SETTING_MAX;i++) /*lint !e685 !e586 !e681 !e568*/
            {
                GUILIST_SetSelectedItem(check_list_id, (uint16)i,render_setting_ptr->render_array[i]);
            }
            MMK_SetAtvCtrl(win_id, check_list_id);
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_CLOSE_WINDOW:
        {
            uint16 select_item[MMISET_EFFECT_RENDER_SETTING_MAX + 1] = {0};
            MMISET_EFFECT_RENDER_SETTING render_setting = {0};
            if(0 < GUILIST_GetSelectedItemIndex(check_list_id, select_item, MMISET_EFFECT_RENDER_SETTING_MAX))
            {
                for(i = 0;i < MMISET_EFFECT_RENDER_SETTING_MAX;i++) /*lint !e685 !e586 !e681 !e568*/
                {
                    if(i == select_item[i])
                    {
                        render_setting.render_array[i] = TRUE;
                    }
                    else
                    {
                        render_setting.render_array[i] = FALSE;
                    }
                }
            }
            MMISET_SetLightShadowRenderSetting(&render_setting);
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}
#endif     
#ifdef SET_3D_STYLE_SUPPORT 
/*****************************************************************************/
//  Description : set 3d interface param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void Set3D_EffectParam(void)
{
    MMI_STRING_T title = {0};
    uint16 index = 0;
    uint32 loop = 0;

    uint32 text_id[]=
    {
        TXT_CLOSE,
        TXT_SET_3D_STYLE_SCREEN_DEFAULT,
        TXT_AUTO, 
#ifdef EFFECT_WIN_SLIDE_SUPPORT        
        TXT_SET_3D_STYLE_SCREEN_SLIDE_MOVE,
        TXT_SET_3D_STYLE_SCREEN_SLIDE, 
#endif        
#ifdef EFFECT_ZOOM_SUPPORT        
        TXT_SET_3D_STYLE_SCREEN_ZOOM,
#endif        
#if 0//def EFFECT_WIN_SLIDE_SUPPORT
        TXT_COMM_FADE_INOUT,
#endif        
#ifdef EFFECT_3D_ROTATE_SUPPORT		        
        TXT_SET_3D_STYLE_SCREEN_ROLL
#endif        
    };

#if 0//def QBTHEME_SUPPORT
    MMISET_IDLE_STYLE_E cur_idle_style = MMIAPISET_GetIdleStyle();

    if (MMISET_IDLE_QBTHEME == cur_idle_style)
    {
        return;
    }
#endif

    
    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_3D_EFFECT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_3D_STYLE_SCREEN, &title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_3D_EFFECT_LABEL_CTRL_ID, &title, FALSE);
#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID, text_id[loop]);
    }
#ifdef UI_P3D_SUPPORT   
    // 后续特效请在框架中添加
    for(loop=loop ;loop < MMISET_3D_STYLE_SCREEN_MAX;loop++)
    {
        uint32 winse_text_id = 0;
        
        winse_text_id = MMIWINSE_GetNameIdBySetingStyle(loop);
        if(0 != winse_text_id)
        {
            GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID, winse_text_id);
        }
    }
    
#endif
    index = MMISET_Get3DStyleScreen();    
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID, index);
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID,TXT_SET_3D_STYLE_SCREEN);   
}
#endif
#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : set mainmenu effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenu_EffectParam(void)
{
    MMI_STRING_T title = {0};
    uint16 index = 0;
    uint32 loop = 0;
    uint32 text_id[MMISET_3D_STYLE_MAINMENU_MAX]=
    {
        TXT_COMMON_COMMON_NORMAL, 
        TXT_MMISET_3D_STYLE_MAINMENU_MAGICMOVE,
        TXT_MMISET_3D_STYLE_MAINMENU_ROTATE3D,
        TXT_MMISET_3D_STYLE_MAINMENU_SLIDEMOVE, 
        TXT_MMISET_3D_STYLE_MAINMENU_ROTATEMOVE,
        TXT_MMISET_3D_STYLE_MAINMENU_LUCKYBOARD, 
        TXT_MMISET_3D_STYLE_MAINMENU_WRAPBAND, 
        TXT_MMISET_3D_STYLE_MAINMENU_DISTORTMOVE    
    };
	//GO不支持3D动画
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
	if(MMIMAINMENU_GO_SLIDE_E == MMIAPISET_GetMainmenuStyle())
	{
		GUIFORM_SetChildDisplay(MMISET_DISPLAY_ANIM_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
	}
	else
#endif
	{
            GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifndef MMI_GUI_STYLE_TYPICAL
	     GUISETLIST_SetFontAlign(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
	     MMI_GetLabelTextByLang(TXT_SET_3D_STYLE_MAINMENU, &title);
	     GUILABEL_SetText(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_LABEL_CTRL_ID, &title, FALSE);
		    
	     for(loop=0;loop<MMISET_3D_STYLE_MAINMENU_MAX;loop++)
		    {
		        GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID, text_id[loop]);
		    }
		    
	     index = MMISET_Get3DStyleMainmenu();  
            GUISETLIST_SetCurIndex(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID, index);
	     GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID,TXT_SET_3D_STYLE_MAINMENU);   
	}
}
#endif
#endif
#if !defined MMI_PDA_SUPPORT && defined(EFFECT_3D_LIST_ROLL_SUPPORT)
/*****************************************************************************/
//  Description : set list effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetListEffectParam(void)
{
    MMI_STRING_T title = {0};
    BOOLEAN is_list_effect_on = 0;
    uint32 loop = 0;
    uint32 text_id[]=
    {
        STR_POC_ILIST_OFF,
        TXT_COMMON_ENABLE_ON
    };

    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_MMISET_3D_STYLE_LIST_ROLL,&title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_LIST_LABEL_CTRL_ID, &title, FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID, text_id[loop]);
    }
    is_list_effect_on = MMIAPISET_GetList3DStyle();
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID, is_list_effect_on);
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID,TXT_MMISET_3D_STYLE_LIST_ROLL);  
}
#endif
#ifdef EFFECT_LCD_ROTATE_SUPPORT
/*****************************************************************************/
//  Description : set g-sensor effect param
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void SetGsensor_EffectParam(void)
{
#ifdef MMI_RES_ORIENT_BOTH // 说明支持横竖屏切换
    MMI_STRING_T title = {0};
    uint16 index = 0;
    uint32 loop = 0;
    uint32 text_id[MMISET_G_SENSOR_SWITCH_MAX]=
    {
        STR_POC_ILIST_OFF, 
        TXT_SET_3D_STYLE_SCREEN_DEFAULT,
        TXT_COMM_FADE_INOUT 
    };
    
    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_MMISET_3D_STYLE_GSENSOR_ROLL, &title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_G_SENSOR_LABEL_CTRL_ID, &title, FALSE);

#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<MMISET_G_SENSOR_SWITCH_MAX;loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID, text_id[loop]);
    }
    
    index = MMIAPISET_GetGsensorSwitchStyle();  
    
    if (index >= MMISET_G_SENSOR_SWITCH_MAX)
    {
        index = MMISET_G_SENSOR_SWITCH_DEFAULT;
    }

    GUISETLIST_SetCurIndex(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID, index);
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID,TXT_MMISET_3D_STYLE_GSENSOR_ROLL);   
#endif
}
#endif


/*****************************************************************************/
//  Description : set list effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPowerOnEffectParam(void)
{
    MMI_STRING_T title = {0};

    MMI_STRING_T power_on_name ={0};
    MMISET_POWER_ON_OFF_ANIM_INFO_T     power_on_off_info  =   {0};
    MMISET_POWER_ON_OFF_ANIM_TYPE_E     index =MMISET_POWER_ON_OFF_ANIM_DEFAULT;
    
    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_COMMON_POWER_ON_ANIMATION, &title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_ON_LABEL_CTRL_ID, &title, FALSE);
    
    MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_info,TRUE);
    index = power_on_off_info.anim_type;
    if(MMISET_POWER_ON_OFF_ANIM_DEFAULT == index)
    {
        MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &power_on_name);
    }
    else if(MMISET_POWER_ON_OFF_PIC_MORE == index
        || MMISET_POWER_ON_OFF_ANIM_MORE == index)
    {
        //MMI_GetLabelTextByLang(TXT_MORE_PICTURE, &power_on_name);
        power_on_name.wstr_ptr = power_on_off_info.more_anim_info.name_wstr;
        power_on_name.wstr_len = power_on_off_info.more_anim_info.name_wstr_len;
    }
//    else
//    {
//        MMI_GetLabelTextByLang(TXT_SET_POWER_ON_OFF_ANIM_MORE, &power_on_name);
//    }
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID, &power_on_name, FALSE); //MODIFY BY MARY
}


/*****************************************************************************/
//  Description : set power off param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPowerOffEffectParam(void)
{
    MMI_STRING_T title = {0};
    MMI_STRING_T power_off_name ={0};
    MMISET_POWER_ON_OFF_ANIM_INFO_T     power_on_off_info  =   {0};
    MMISET_POWER_ON_OFF_ANIM_TYPE_E     index =MMISET_POWER_ON_OFF_ANIM_DEFAULT;

    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_COMMON_POWER_OFF_ANIMATION, &title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_CTRL_ID, &title, FALSE);

    MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_info,FALSE);
    index = power_on_off_info.anim_type;
    if(MMISET_POWER_ON_OFF_ANIM_DEFAULT == index)
    {
        MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &power_off_name);
    }
    else if(MMISET_POWER_ON_OFF_PIC_MORE == index
        || MMISET_POWER_ON_OFF_ANIM_MORE == index)
    {
        //MMI_GetLabelTextByLang(TXT_MORE_PICTURE, &power_off_name);
        power_off_name.wstr_ptr = power_on_off_info.more_anim_info.name_wstr;
        power_off_name.wstr_len = power_on_off_info.more_anim_info.name_wstr_len;
    }
//    else
//    {
//        MMI_GetLabelTextByLang(TXT_SET_POWER_ON_OFF_ANIM_MORE, &power_off_name);
//    }
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID, &power_off_name, FALSE);// MODIFY BY MARY

}   
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER) 
/*****************************************************************************/
//  Description : set go effect param
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetRenderEffectParam(void)
{
    MMI_STRING_T title = {0};

	//在iphone下不显示设置光亮特效等
    if(MMISET_EFFECT_RENDER_SETTING_MAX > 0
	   && (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
	   )/*lint !e506 !e774*/
    {
        GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        MMI_GetLabelTextByLang(TXT_SET_LIGHT_SHADOW_RENDER_ANIMATION, &title);
        
        GUILABEL_SetText(MMISET_DISPLAY_ANIM_RENDER_LABEL_CTRL_ID, &title, FALSE);
        MMI_GetLabelTextByLang(TXT_SET_GO_MENU_ANIMATION, &title);
        GUILABEL_SetText(MMISET_DISPLAY_ANIM_RENDER_LABEL_EDIT_CTRL_ID, &title, FALSE);// 
    }
    else
    {
        //GUIFORM_SetVisible(MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID,FALSE,FALSE);
        GUIFORM_SetChildDisplay(MMISET_DISPLAY_ANIM_FORM_CTRL_ID, MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    }
    
} 
#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : save all anim set feature
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllAnimSet(void)
{
    // @xiaoqing.lu(注) 这是一段效率很低的代码，读取4次NV，保存4次NV 
#ifdef SET_3D_STYLE_SUPPORT
    MMISET_Set3DStyleScreen(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID);
#endif
#ifndef FLASH_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
    MMISET_Set3DStyleMainmenu(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID);
#endif
#endif
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
    MMISET_Set3DStyleList3DStyle(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID);
#endif
#ifdef MMI_RES_ORIENT_BOTH // 说明支持横竖屏切换
    MMISET_SetGsensorSwitchStyle(GUISETLIST_GetCurIndex(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID));
#endif
}
#endif

#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : Handle the window of power on animation setting.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerOnAnimWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    MMI_RESULT_E                    recode              =   MMI_RESULT_TRUE;
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_anim_info  =   {0};
    MULTIM_SELECT_RETURN_T          *win_param          =   NULL;
    uint16                          cur_item            = 0;
    MMI_STRING_T                    power_on_name ={0};
    static   MMISET_POWER_ON_OFF_ANIM_TYPE_E   power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
#ifdef MMI_PIC_CROP_SUPPORT
    static   BOOLEAN             is_power_on_crop_in    = TRUE;//crop只在开始进入，之后不在进入
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(
            MMISET_SET_POWER_ON_ANIM_CTRL_ID, 
            MMISET_POWER_ON_OFF_ANIM_MAX_NUM, FALSE 
            );//max item 2
        AppendPowerOnOffAnimItem(MMISET_SET_POWER_ON_ANIM_CTRL_ID, TRUE);        
        MMK_SetAtvCtrl(win_id,MMISET_SET_POWER_ON_ANIM_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        cur_item = GUILIST_GetCurItemIndex(MMISET_SET_POWER_ON_ANIM_CTRL_ID);
        MMIAPISET_GetPowerOnOffAnimInfo(&power_on_anim_info,TRUE);
        GUILIST_SetSelectedItem(MMISET_SET_POWER_ON_ANIM_CTRL_ID, power_on_anim_info.anim_type, TRUE);    
        GUILIST_SetCurItemIndex(MMISET_SET_POWER_ON_ANIM_CTRL_ID,cur_item);
        break;    
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        switch (GUILIST_GetCurItemIndex(MMISET_SET_POWER_ON_ANIM_CTRL_ID))
        {
            
        case MMISET_POWER_ON_OFF_ANIM_DEFAULT:
            {
                power_on_anim_info.is_open     = TRUE;
                power_on_anim_info.anim_type   = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
                power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
                MMIAPISET_SetPowerOnOffAnimInfo(&power_on_anim_info, TRUE);
                
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &power_on_name);
                GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID, &power_on_name, FALSE); //MODIFY BY MARY
                MMK_CloseWin( win_id );
            }
            break;
            
            
        case MMISET_POWER_ON_OFF_ANIM_MORE:
            {
                if (MMIAPIUDISK_UdiskIsRun())
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
                else
                {
                    MMIFMM_SELECT_INFO_T    select_info = {{MMIFMM_MOVIE_ALL,0},0,0,PNULL,0,-1};
                    MMIAPISET_GetPowerOnOffAnimInfo(&power_on_anim_info, TRUE);
                    power_off_type = MMISET_POWER_ON_OFF_ANIM_MORE;
#ifdef DRM_SUPPORT
            		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif                   
                    if(MMISET_POWER_ON_OFF_ANIM_MORE == power_on_anim_info.anim_type)
                    {
                        select_info.ticked_file_wstr = power_on_anim_info.more_anim_info.name_wstr;
                        select_info.ticked_file_len = power_on_anim_info.more_anim_info.name_wstr_len;
                        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMovieWin(&select_info);
                    }
                    else
                    {
                        select_info.ticked_file_wstr = PNULL;
                        select_info.ticked_file_len = 0;
                        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMovieWin(&select_info);
                    }
#ifdef DRM_SUPPORT
                    {
                        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                        limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;
            			limit_value.is_rights_valid = 1;
                        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
                    }
#endif
                }
            }
            break;
 
        case MMISET_POWER_ON_OFF_PIC_MORE:
        {
        
            MMIFMM_SELECT_INFO_T    select_info = {0};

            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;
            select_info.ring_vol = -1;
            MMIAPISET_GetPowerOnOffAnimInfo(&power_on_anim_info, TRUE);
            power_off_type = MMISET_POWER_ON_OFF_PIC_MORE;
#ifdef DRM_SUPPORT
    		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif                   
			s_fmm_poweron_call = TRUE;
            if(MMISET_POWER_ON_OFF_PIC_MORE == power_on_anim_info.anim_type)
            {
                select_info.win_id = win_id;
                select_info.ticked_file_wstr = power_on_anim_info.more_anim_info.name_wstr;
                select_info.ticked_file_len = power_on_anim_info.more_anim_info.name_wstr_len;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len = 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
    			limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif

        }
            break;
 
        default:
            //SCI_PASSERT(0,("HandleSetPowerOnAnimWindow msg_id = %d", msg_id));
            break;
        }
        break;
        
        
        case MSG_MULTIM_SELECTED_RETURN:
            {          
                MMI_TEXT_ID_T   txt_id = 0;
                 
                win_param  = (MULTIM_SELECT_RETURN_T *)param;
                power_on_anim_info.more_anim_info.name_wstr_len = (uint16)MIN(
                    win_param->file_name_len,
                    MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN
                    );
                
                MMI_WSTRNCPY(
                    power_on_anim_info.more_anim_info.name_wstr, 
                    MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN,
                    win_param->file_name_ptr, 
                    win_param->file_name_len,
                    power_on_anim_info.more_anim_info.name_wstr_len
                    );
                
                power_on_anim_info.is_open      = TRUE;
                power_on_anim_info.anim_type    = power_off_type;
#ifdef MMI_PIC_CROP_SUPPORT
				if (MMISET_POWER_ON_OFF_PIC_MORE == power_off_type&&is_power_on_crop_in )
				{
                    is_power_on_crop_in = FALSE;
					MMIAPIFMM_SetPictureToPowerAndSaverEx(win_param->file_name_ptr, win_param->file_name_len);
				}
                else
				{
#endif
                MMIAPISET_SetPowerOnOffAnimInfo(&power_on_anim_info, TRUE);
                AppendPowerOnOffAnimItem(MMISET_SET_POWER_ON_ANIM_CTRL_ID, TRUE);

                /* Modify by michael on 2/23/2012 for NEWMS00171595 : 去掉"成功"提示框*/
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                /************************Modify over*************************/

                if(MMISET_POWER_ON_OFF_ANIM_MORE == power_on_anim_info.anim_type
                    || MMISET_POWER_ON_OFF_PIC_MORE == power_on_anim_info.anim_type)
                {
                    //txt_id = TXT_SET_POWER_ON_OFF_ANIM_MORE;
                    power_on_name.wstr_ptr = power_on_anim_info.more_anim_info.name_wstr;
                    power_on_name.wstr_len = power_on_anim_info.more_anim_info.name_wstr_len;
                }
                else if(MMISET_POWER_ON_OFF_PIC_MORE == power_on_anim_info.anim_type)
                {
                    txt_id = TXT_MORE_PICTURE;
                }
                MMI_GetLabelTextByLang(txt_id, &power_on_name);
                GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID, &power_on_name, FALSE); //MODIFY BY MARY
#if !defined MMI_GUI_STYLE_TYPICAL
                MMK_CloseWin( win_id );
#endif
#ifdef MMI_PIC_CROP_SUPPORT
				}
#endif
            }
            break;
            
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
            
        case MSG_CLOSE_WINDOW:
			s_fmm_poweron_call = FALSE;
#ifdef MMI_PIC_CROP_SUPPORT
			is_power_on_crop_in = TRUE;
#endif
            power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : Handle the window of power off animation setting.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerOffAnimWindow(
                                                MMI_WIN_ID_T       win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                )
{
    MMI_RESULT_E                        recode                =     MMI_RESULT_TRUE;
    MMISET_POWER_ON_OFF_ANIM_INFO_T     power_off_anim_info   =     {0};
    MULTIM_SELECT_RETURN_T              *win_param            =     NULL;
    uint16                              cur_item              = 0;
    MMI_STRING_T power_off_name ={0};
    static   MMISET_POWER_ON_OFF_ANIM_TYPE_E   power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
#ifdef MMI_PIC_CROP_SUPPORT
    static   BOOLEAN is_power_off_crop_in    = TRUE;//crop_support return
#endif
	switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(
            MMISET_SET_POWER_OFF_ANIM_CTRL_ID, 
            MMISET_POWER_ON_OFF_ANIM_MAX_NUM, FALSE 
            );//max item 2
        AppendPowerOnOffAnimItem(MMISET_SET_POWER_OFF_ANIM_CTRL_ID, FALSE);
        MMK_SetAtvCtrl(win_id,MMISET_SET_POWER_OFF_ANIM_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        cur_item = GUILIST_GetCurItemIndex(MMISET_SET_POWER_OFF_ANIM_CTRL_ID);
        MMIAPISET_GetPowerOnOffAnimInfo(&power_off_anim_info,FALSE);
        GUILIST_SetSelectedItem(MMISET_SET_POWER_OFF_ANIM_CTRL_ID, power_off_anim_info.anim_type, TRUE);    
        GUILIST_SetCurItemIndex(MMISET_SET_POWER_OFF_ANIM_CTRL_ID,cur_item);
        break;  
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        switch (GUILIST_GetCurItemIndex(MMISET_SET_POWER_OFF_ANIM_CTRL_ID))
        {
            
        case MMISET_POWER_ON_OFF_ANIM_DEFAULT:
            {
                power_off_anim_info.is_open     = TRUE;
                power_off_anim_info.anim_type   = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
                power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
                MMIAPISET_SetPowerOnOffAnimInfo(&power_off_anim_info, FALSE);
                
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

            MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &power_off_name);
            GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID, &power_off_name, FALSE);// MODIFY BY MARY
                MMK_CloseWin( win_id );
            }
            break;
            
            
        case MMISET_POWER_ON_OFF_ANIM_MORE:
            {
                if (MMIAPIUDISK_UdiskIsRun())
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
                else
                {
                    MMIFMM_SELECT_INFO_T    select_info = {0};
                    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_MOVIE_ALL;
                    select_info.ring_vol = -1;
                    MMIAPISET_GetPowerOnOffAnimInfo(&power_off_anim_info, FALSE);
                    power_off_type = MMISET_POWER_ON_OFF_ANIM_MORE;
#ifdef DRM_SUPPORT
            		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif                   
                    if(MMISET_POWER_ON_OFF_ANIM_MORE == power_off_anim_info.anim_type)
                    {
                        select_info.win_id = win_id;
                        select_info.ticked_file_wstr = power_off_anim_info.more_anim_info.name_wstr;
                        select_info.ticked_file_len = power_off_anim_info.more_anim_info.name_wstr_len;
                        MMIAPIFMM_OpenSelectMovieWin(&select_info);
                    }
                    else
                    {
                        select_info.ticked_file_wstr = PNULL;
                        select_info.ticked_file_len = 0;
                        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMovieWin(&select_info);
                    }
#ifdef DRM_SUPPORT
                    {
                        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                        limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
            			limit_value.is_rights_valid = 1;
                        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
                    }
#endif
                }
            }
            break;
     
        case MMISET_POWER_ON_OFF_PIC_MORE:
        {
        
            MMIFMM_SELECT_INFO_T    select_info = {0};
            select_info.ring_vol = -1;
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;

            MMIAPISET_GetPowerOnOffAnimInfo(&power_off_anim_info, FALSE);
            power_off_type = MMISET_POWER_ON_OFF_PIC_MORE;
#ifdef DRM_SUPPORT
    		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif            
			s_fmm_poweroff_call = TRUE;
            if(MMISET_POWER_ON_OFF_PIC_MORE == power_off_anim_info.anim_type)
            {
                select_info.win_id = win_id;
                select_info.ticked_file_wstr = power_off_anim_info.more_anim_info.name_wstr;
                select_info.ticked_file_len = power_off_anim_info.more_anim_info.name_wstr_len;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len = 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT |MMIFMM_DRM_CONS_LIMIT_INTERVAL |MMIFMM_DRM_CONS_LIMIT_DATETIME;;
    			limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif

        }
            break;
 
        default:
            //SCI_PASSERT(0,("HandleSetPowerOffAnimWindow msg_id = %d", msg_id));
            break;
        }
        break;
        
        
        case MSG_MULTIM_SELECTED_RETURN:
            {          
                MMI_TEXT_ID_T   txt_id = 0;
                
                win_param  = (MULTIM_SELECT_RETURN_T *)param;                
                power_off_anim_info.more_anim_info.name_wstr_len = (uint16)MIN(
                    win_param->file_name_len,
                    MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN
                    );
                
                MMI_WSTRNCPY(
                    power_off_anim_info.more_anim_info.name_wstr, 
                    MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN,
                    win_param->file_name_ptr, 
                    win_param->file_name_len,
                    power_off_anim_info.more_anim_info.name_wstr_len
                    );
                
                power_off_anim_info.is_open      = TRUE;
                power_off_anim_info.anim_type    = power_off_type;
#ifdef MMI_PIC_CROP_SUPPORT
				if (MMISET_POWER_ON_OFF_PIC_MORE == power_off_type &&is_power_off_crop_in)
				{
                    is_power_off_crop_in = FALSE;
					MMIAPIFMM_SetPictureToPowerAndSaverEx(win_param->file_name_ptr, win_param->file_name_len);
				}
				else
				{  
#endif  
                MMIAPISET_SetPowerOnOffAnimInfo(&power_off_anim_info, FALSE);
                AppendPowerOnOffAnimItem(MMISET_SET_POWER_OFF_ANIM_CTRL_ID, FALSE);
                
                /* Modify by michael on 2/23/2012 for NEWMS00171595 : 去掉"成功"提示框*/
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                /************************Modify over*************************/
                
                if(MMISET_POWER_ON_OFF_ANIM_MORE == power_off_anim_info.anim_type
                    || MMISET_POWER_ON_OFF_PIC_MORE == power_off_anim_info.anim_type)
                {
                    //txt_id = TXT_SET_POWER_ON_OFF_ANIM_MORE;
                    power_off_name.wstr_ptr = power_off_anim_info.more_anim_info.name_wstr;
                    power_off_name.wstr_len = power_off_anim_info.more_anim_info.name_wstr_len;
                }
                else if(MMISET_POWER_ON_OFF_PIC_MORE == power_off_anim_info.anim_type)
                {
                    txt_id = TXT_MORE_PICTURE;
                }
                MMI_GetLabelTextByLang(txt_id, &power_off_name);
                
                GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID, &power_off_name, FALSE);// MODIFY BY MARY
#if !defined MMI_GUI_STYLE_TYPICAL
                MMK_CloseWin( win_id );
#endif
#ifdef MMI_PIC_CROP_SUPPORT
				}
#endif
            }
            break;
            
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
            
        case MSG_CLOSE_WINDOW:
            s_fmm_poweroff_call = FALSE;
#ifdef MMI_PIC_CROP_SUPPORT
            is_power_off_crop_in = TRUE;
#endif
			
            power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : Append the power on off setting items.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendPowerOnOffAnimItem(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    BOOLEAN         is_power_on_off
                                    )
{
    
    uint16                              cur_selection           =   0;
    MMISET_POWER_ON_OFF_ANIM_INFO_T     power_on_off_anim_info  =   {0};
    
    MMIAPISET_AppendListItemByTextId( TXT_EXPORT_LOC_DEFAULT, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#ifdef VIDEO_PLAYER_SUPPORT
    MMIAPISET_AppendListItemByTextId( TXT_SET_POWER_ON_OFF_ANIM_MORE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
    MMIAPISET_AppendListItemByTextId( TXT_MORE_PICTURE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_GetPowerOnOffAnimInfo(
        &power_on_off_anim_info,
        is_power_on_off
        );
    cur_selection = (uint16)power_on_off_anim_info.anim_type;
    
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

#endif
/*****************************************************************************/
//  Description : get power on off animation set info
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetPowerOnOffAnimInfo(
                                            MMISET_POWER_ON_OFF_ANIM_INFO_T *power_on_off_anim_info_ptr,    //in/out:
                                            BOOLEAN is_power_on_off     //TRUE:power on, FALSE:power off
                                            )
{
    MN_RETURN_RESULT_E   return_value   =   MN_RETURN_FAILURE;
    BOOLEAN              is_need_reset  =   FALSE;
    BOOLEAN              is_file_exist  =   TRUE;
    
    //SCI_ASSERT(PNULL != power_on_off_anim_info_ptr);
   if(PNULL == power_on_off_anim_info_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetPowerOnOffAnimInfo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_2609_112_2_18_2_51_4_176,(uint8*)"");
        return;
    }     
    if(is_power_on_off)
    {
        MMINV_READ(
            MMINV_POWER_ON_ANIM_SETTING_INFO, 
            power_on_off_anim_info_ptr, 
            return_value
            );
    }
    else
    {
        MMINV_READ(
            MMINV_POWER_OFF_ANIM_SETTING_INFO, 
            power_on_off_anim_info_ptr, 
            return_value
            );
    }
    
    if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info_ptr->anim_type)
        || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info_ptr->anim_type))
    {
        is_file_exist = MMIAPIFMM_IsFileExist(power_on_off_anim_info_ptr->more_anim_info.name_wstr, power_on_off_anim_info_ptr->more_anim_info.name_wstr_len);
    }
    if ((MN_RETURN_SUCCESS != return_value) || (!is_file_exist))
    {
        is_need_reset = TRUE;
    }
#ifdef DRM_SUPPORT
        if (!is_need_reset
            && MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE, power_on_off_anim_info_ptr->more_anim_info.name_wstr)
            && !MMIAPIDRM_IsRightsValid(SFS_INVALID_HANDLE, power_on_off_anim_info_ptr->more_anim_info.name_wstr, TRUE, DRM_PERMISSION_PLAY))
        {
            is_need_reset = TRUE;
        }
#endif	
    if(is_need_reset)
    {    
        power_on_off_anim_info_ptr->is_open     = TRUE;
        power_on_off_anim_info_ptr->anim_type   = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
        SCI_MEMSET(
            &power_on_off_anim_info_ptr->more_anim_info, 
            0, 
            sizeof(MMISET_MORE_POWER_ON_OFF_ANIM_T)
            );
        if(!MMIAPIUDISK_UdiskIsRun())
        {        
            if(is_power_on_off)
            {
                MMINV_WRITE(
                    MMINV_POWER_ON_ANIM_SETTING_INFO, 
                    power_on_off_anim_info_ptr
                    );
            }
            else
            {
                MMINV_WRITE(
                    MMINV_POWER_OFF_ANIM_SETTING_INFO, 
                    power_on_off_anim_info_ptr
                    );
            }
        }
    }
}
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
//  Description : to handle select idle display mode
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetIdleDisplayWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;    
    uint16          cur_index   =   0;
	uint16			top_index	=	0;
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:  
        MMK_SetAtvCtrl(win_id,MMISET_SET_IDLE_DISPLAY_CTRL_ID);
        break;
    case MSG_FULL_PAINT:
		top_index = GUILIST_GetTopItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID);
		cur_index = GUILIST_GetCurItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID);
        MMISET_AppendIdleDisplayListItem(MMISET_SET_IDLE_DISPLAY_CTRL_ID); 
		GUILIST_SetTopItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID, top_index);
        GUILIST_SetCurItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID, cur_index);
        break;
    case MSG_CTL_OK:
#if !defined MMI_GUI_STYLE_TYPICAL
#ifndef MMI_GUI_STYLE_MINISCREEN //小屏项目不需要执行该语句add by yongsheng.wang
        MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
        break;
#endif
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:

        //get current selected items
        // selected_num = GUILIST_GetSelectedItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID,selected_item,MMISET_IDLE_DISPLAY_MAX_ITEM);
        cur_index = GUILIST_GetCurItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID); 
		
        switch(cur_index) 
        {
//#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
        case MMISET_IDLE_CUSTOM_WORD:
            if (MMIAPISET_IsDisplayIdleWords())
            {
                MMISET_SetIdleDisplayMode(cur_index);   
                MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
            }
            else
            {
                MMK_CreateWin((uint32*)MMISET_EDIT_IDLE_WOEDS_WIN_TAB,PNULL);
            } 
            break;
#endif
//#endif
        default:
            MMISET_SetIdleDisplayMode(cur_index);   
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);            
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            //MMIAPISET_SetDisplaytIdleMode();
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_CLOSE_WINDOW:
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
            MMIAPISET_SetDisplaytIdleMode();        /*lint !e718 !e18 */
#endif
#endif
            break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.            
        case MSG_APP_FLIP:
            MMIDEFAULT_HandleFlipKey(FALSE);
            MMIAPISET_SetDisplaytIdleMode();
            recode = MMI_RESULT_FALSE;
            break;
#endif            
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }   
    
    
    return recode;
}

#endif
#endif
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : to handle edit idle words win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditIdleWordsWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E            recode          =   MMI_RESULT_TRUE;
#ifdef DPHONE_SUPPORT 
#else       
    MMI_CTRL_ID_T           list_ctrl_id    =   MMISET_SET_IDLE_DISPLAY_CTRL_ID;
#endif 
    MMI_STRING_T            edit_string     =   {0};
    MMISET_IDLE_DISPLAY_T   *display_info   =   PNULL;
    wchar                               forbid_char_value[]= {0x0d, 0x0a};
    display_info = MMIAPISET_GetIdleDisplayMode();
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        break;
        
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetTextMaxLen(MMISET_IDLE_WORDS_EDITCTRL_ID,MMISET_IDLE_WORDS_UCS_MAX_LEN,MMISET_IDLE_WORDS_UCS_MAX_LEN);
        GUIEDIT_SetString(MMISET_IDLE_WORDS_EDITCTRL_ID, display_info->idle_words,(uint16)display_info->wstr_len);         

     //set softkey //个性词语可以为空
        //GUIEDIT_SetSoftkey(MMISET_IDLE_WORDS_EDITCTRL_ID,0,1,TXT_NULL,STXT_SOFTKEY_OK_MK,PNULL);
         GUIEDIT_ConfigDisableChar(MMISET_IDLE_WORDS_EDITCTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
        
        MMK_SetAtvCtrl(win_id, MMISET_IDLE_WORDS_EDITCTRL_ID);
        break;
#ifdef TOUCH_PANEL_SUPPORT
    case  MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T     softkey_ctrl_id = 0;
            softkey_ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == softkey_ctrl_id)
            {
				MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
            }
            else if(MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == softkey_ctrl_id)
            {
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
            }
        }
        break;
#endif

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        // to get the content which the user input  
        SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
        GUIEDIT_GetString( MMISET_IDLE_WORDS_EDITCTRL_ID, &edit_string);

    //个性词语可以为空
    // if (0 == edit_string.wstr_len)
       // {
       //       break;
       // }
        if (MMIAPISET_IsIncludeReturnChar( edit_string.wstr_ptr,edit_string.wstr_len ))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_INVALID_CUSTOM_WORD);
            break;
        }
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/04
#else               
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD))       
        // to save the information
        if (MMISET_SetIdleWordsContent(&edit_string))
        {
            if (MMK_IsOpenWin(MMISET_SET_IDLE_DISPLAY_WIN_ID))
            {
            #if defined MMISET_IDLE_DISPLAY_SUPPORT
                MMISET_SetIdleDisplayMode(MMISET_IDLE_CUSTOM_WORD);  
            #endif
                GUILIST_SetSelectedItem(list_ctrl_id,MMISET_IDLE_CUSTOM_WORD,FALSE);
            }
            MMK_CloseWin( win_id ); 
        }
#endif  
#endif 
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        // close the edit window
        MMK_CloseWin( win_id );        
        break;        
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
/*****************************************************************************/
//  Description : HandleContrastTPDown
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: for user touch bar support
/*****************************************************************************/
PUBLIC void MMISET_HandleContrastTPDown(
                                GUI_POINT_T     *point_ptr,
                                MMI_WIN_ID_T    win_id,
                                MMI_CTRL_ID_T   ctrl_id,
                                BOOLEAN         is_called_by_ps//是否由省电模式呼叫
                                )
{
    int16           cur_contrast = 0;
    int16           step_len = 0;
    int16           display_x = 0;
    int16           display_y = 0;
    uint16          bar_width = 0;
    uint16          bar_height = 0;
    GUI_RECT_T      display_rect = {0};
    GUI_RECT_T      ctrl_display_rect = {0};
    GUI_RECT_T      img_rect = {0};
#ifdef MMI_POWER_SAVING_MODE_SUPPORT    
    MMISET_POWER_SETTING_ITEM_T power_sav_setting = MMIAPISET_GetPowerSavingSetting();
#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
    bar_width = MMISET_DISPLAY_CONTRAST_BAR_LONG;
    bar_height = MMISET_DISPLAY_CONTRAST_BAR_HIGHT;
#else
    GUIRES_GetImgWidthHeight(&bar_width,
        &bar_height,
        IMAGE_SET_CONTRAST_BAR_FG,
        win_id);
#endif

    GUIOWNDRAW_GetRect(ctrl_id,&display_rect);
    display_x = display_rect.left;
    display_y = display_rect.top;
    GUIANIM_AdjustDisplayPosition(&display_x,
        &display_y,
        bar_width,
        bar_height,
        display_rect,
        GUIANIM_ALIGN_HVMIDDLE);

    //set display rect
    display_rect.left   = display_x;
    display_rect.top    = display_y;
    display_rect.right  = (int16)(display_rect.left + bar_width - 1);
    display_rect.bottom = (int16)(display_rect.top + bar_height - 1);

    GUIOWNDRAW_GetDisplayRect(ctrl_id,&ctrl_display_rect);

    if (GUI_IntersectRect(&display_rect,display_rect,ctrl_display_rect))
    {
        //set image rect
        img_rect.left   = (int16)(display_rect.left - display_x);
        img_rect.top    = (int16)(display_rect.top - display_y);
        img_rect.right  = (int16)(display_rect.right - display_x);
        img_rect.bottom = (int16)(display_rect.bottom - display_y);

        //display bg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            &img_rect,
            win_id,
#ifdef MMI_GUI_STYLE_MINISCREEN
            IMAGE_SET_CONTRAST_BAR_BG_NEW,
#else
            IMAGE_SET_CONTRAST_BAR_BG,
#endif
            MMITHEME_GetDefaultLcdDev());

        //get current contrast
        if (point_ptr->x <= display_rect.left)
        {
            cur_contrast = 0;
        }
        else if (point_ptr->x >= display_rect.right)
        {
            cur_contrast = MMISET_CONTRAST_SEVEN;
        }
        else
        {
            step_len     = bar_width/MMISET_CONTRAST_SEVEN;
	    if(step_len != 0)
	    {
                cur_contrast = (point_ptr->x - display_x + step_len)/step_len;
	    }
        }
        //set image rect
        img_rect.right = img_rect.left + (cur_contrast*bar_width/MMISET_CONTRAST_SEVEN) - 1;

        //display fg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            &img_rect,
            win_id,
#ifdef MMI_GUI_STYLE_MINISCREEN
            IMAGE_SET_CONTRAST_BAR_BG_NEW,
#else
            IMAGE_SET_CONTRAST_BAR_FG,
#endif
            MMITHEME_GetDefaultLcdDev());

#ifdef MMI_POWER_SAVING_MODE_SUPPORT    
        if(is_called_by_ps)//NEWMS00160657是由省电模式呼叫，且当前未开启省电模式
        {
            power_sav_setting.lcd_contrast = cur_contrast;
            MMIAPISET_SetPowerSavingSetting(power_sav_setting);
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,cur_contrast);
            
            if(power_sav_setting.is_active)
            {
                MMIAPISET_SetCurrentContrast(cur_contrast);
            }
        }
        else
#endif
        {
            MMIAPISET_SetCurrentContrast(cur_contrast);
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,cur_contrast);
        }
    }
} 




/*****************************************************************************/
//  Description : save all setting
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllFeatureState(void)
{
    //设置背光亮度
    MMIAPISET_SetMainLCDContrast();
    //背光时间
//#if defined MMISET_BACKLIGHT_TIME_SUPPORT
//    MMISET_SetBackLight(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID);
//#endif
    MMIDEFAULT_TurnOnLCDBackLight();        
    //按键背光时间
//#if defined MMISET_KEYBACKLIGHT_SUPPORT
//    MMISET_SetKeyboardLight(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID); 
//#endif
#if defined MMISET_IDLE_STYLE_SUPPORT
    //待机界面风格
    if ( SetIdleStyle(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID) )/*lint !e774*/
    {
        MMK_ReturnIdleWin();
    }
#endif
}  

#if defined DPHONE_SUPPORT
/*****************************************************************************/
//  Description : to handle set backlight
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBacklightWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_BACKLIGHT_CTRL_ID,MMISET_BACKLIGHT_MAX_ITEM, FALSE );//max item 6
        MMISET_AppendBacklightListItem(MMISET_SET_BACKLIGHT_CTRL_ID);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_BACKLIGHT_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMISET_SetBackLight(MMISET_SET_BACKLIGHT_CTRL_ID);
        MMIDEFAULT_TurnOnLCDBackLight();        
        //success!
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
        MMK_CloseWin( win_id );
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
//  Description : to handle set constrast
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetConstrastWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{

    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id         =   MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID;
    static uint8    cur_constrast   =   0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISET_DisplayInitContrast(win_id);
        SetContrastFormParam();
        break;
    case MSG_FULL_PAINT:
        //MMIMAIN_DisplayDefaultWallpaper();
        break;
        
        
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
        MMISET_TuneUpConstrast(msg_id);
        //display contrast picutres
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            //HandleContrastTPDown(point,win_id);
        }
        break;

     case MSG_CTL_PENOK:
        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        
        if (MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID == ctrl_id)
        {
            MMK_PostMsg(win_id, MSG_APP_LEFT, PNULL, 0);
        }
        else if (MMISET_CONTRAST_UP_BUTTON_CTRL_ID == ctrl_id)
        {
            MMK_PostMsg(win_id, MSG_APP_RIGHT, PNULL, 0);
        }       
        break;      
#endif        

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
        MMIAPISET_SetMainLCDContrast();
        
        //success!
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_LOSE_FOCUS:
        cur_constrast = MMIAPISET_GetCurrentContrast();
        //SCI_TRACE_LOW:"HandleSetConstrastWindow cur_constrast = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_3150_112_2_18_2_51_5_177,(uint8*)"d",cur_constrast);
        //MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        break;
        
    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"HandleSetConstrastWindow cur_constrast = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_3155_112_2_18_2_51_5_178,(uint8*)"d",cur_constrast);
        MMIAPISET_SetCurrentContrast(cur_constrast);      
        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,cur_constrast);       
        
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_APP_RED:
        //SCI_TRACE_LOW("HandleSetConstrastWindow:received MSG_APP_RED and return idle win!");
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        
        recode = MMI_RESULT_FALSE;
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        //SCI_TRACE_LOW("HandleSetConstrastWindow:received MSG_APP_FLIP and return idle win!");
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        recode = MMI_RESULT_FALSE;
        break;
#endif
        
    case MSG_CLOSE_WINDOW:
        cur_constrast = 0;
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : SetContraseFormParam
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetContrastFormParam(void)
{
    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
   
    data_info.img_id = IMAGE_SET_CONTRAST_BAR_BG;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
}
#endif
/*****************************************************************************/
//  Description : ContrastBarCtrlCallbackFun
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_ContrastBarCtrlCallbackFun (
                                       GUIOWNDRAW_INFO_T    *draw_ptr
                                       )
{
    uint8           cur_contrast = 0;
    int16           display_x = 0;
    int16           display_y = 0;
    uint16          bar_width = 0;
    uint16          bar_height = 0;
#if (defined  MMI_GUI_STYLE_MINISCREEN ) && (!defined TOUCH_PANEL_SUPPORT)
    uint16          triangle_width = 0;
    uint16          triangle_height = 0;
    GUI_RECT_T      triangle_display_rect = {0};
#endif    
    GUI_RECT_T      display_rect = {0};
#ifndef MMI_GUI_STYLE_MINISCREEN //pclint
    GUI_RECT_T      img_rect = {0};
#endif
    MMI_WIN_ID_T    win_id = 0;
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    MMISET_POWER_SETTING_ITEM_T power_sav_setting = MMIAPISET_GetPowerSavingSetting();
#endif

    if(MMK_IsOpenWin(MMISET_SET_DISPLAY_WIN_ID))
    {
        win_id = MMISET_SET_DISPLAY_WIN_ID;
        //get current contrast
        cur_contrast = MMIAPISET_GetCurrentContrast();
    }
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    else if(MMK_IsOpenWin(MMISET_POWER_SAVE_SETTING_WIN_ID))
    {
        win_id = MMISET_POWER_SAVE_SETTING_WIN_ID;
        //get powersaving contrast
        cur_contrast = power_sav_setting.lcd_contrast;
    }
#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
#ifndef TOUCH_PANEL_SUPPORT  //Touch情况下不需要三角提示
    GUIRES_GetImgWidthHeight(&triangle_width,
                                                    &triangle_height,
                                                    IMAGE_SET_CONTRAST_TRIANGLE_LEFT,
                                                    win_id);
#endif
    bar_width = MMISET_DISPLAY_CONTRAST_BAR_LONG;
    bar_height = MMISET_DISPLAY_CONTRAST_BAR_HIGHT;
    
    display_x = draw_ptr->ctrl_rect.left;
    display_y = draw_ptr->ctrl_rect.top;
    
    GUIANIM_AdjustDisplayPosition(&display_x,
                                                        &display_y,
                                                        bar_width,
                                                        bar_height,
                                                        draw_ptr->ctrl_rect,
                                                        GUIANIM_ALIGN_HVMIDDLE);

    //set display rect
    display_rect.left   = display_x;
    display_rect.top    = display_y;
    display_rect.right  = (int16)(display_rect.left + bar_width - 1);
    display_rect.bottom = (int16)(display_rect.top + bar_height - 1);

    if (GUI_IntersectRect(&display_rect,display_rect,draw_ptr->display_rect))
    {
#ifndef TOUCH_PANEL_SUPPORT  //Touch情况下不需要三角提示
        if(MMK_IsActiveCtrl(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID)
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
           || MMK_IsActiveCtrl(MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID)
#endif
          )
        {
            //display left triangle 
            triangle_display_rect.left = display_rect.left - MMISET_DISPLAY_CONTRAST_TRIANGLE_LEFT_OFFSET;
            triangle_display_rect.top = display_rect.top + MMISET_DISPLAY_CONTRAST_TRIANGLE_TOP_OFFSET;
            triangle_display_rect.right =  triangle_display_rect.left + triangle_width;
            triangle_display_rect.bottom = triangle_display_rect.top + triangle_height;

            GUIRES_DisplayImg(PNULL,
                                            &triangle_display_rect,
                                            PNULL,
                                            win_id,
                                            IMAGE_SET_CONTRAST_TRIANGLE_LEFT,
                                            MMITHEME_GetDefaultLcdDev());

            //display right triangle 
            triangle_display_rect.left = display_rect.right + MMISET_DISPLAY_CONTRAST_TRIANGLE_RIGHT_OFFSET;
            triangle_display_rect.top = display_rect.top + MMISET_DISPLAY_CONTRAST_TRIANGLE_TOP_OFFSET;
            triangle_display_rect.right =  triangle_display_rect.left + triangle_width;
            triangle_display_rect.bottom = triangle_display_rect.top + triangle_height;
        
            GUIRES_DisplayImg(PNULL,
                                            &triangle_display_rect,
                                            PNULL,
                                            win_id,
                                            IMAGE_SET_CONTRAST_TRIANGLE_RIGHT,
                                            MMITHEME_GetDefaultLcdDev());
        }
#endif
        
        //display bg
        GUIRES_DisplayImg(PNULL,
                                        &display_rect,
                                        PNULL,
                                        win_id,
                                        IMAGE_SET_CONTRAST_BAR_BG_NEW,
                                        MMITHEME_GetDefaultLcdDev());

        //set image rect
        display_rect.right = (int16)(display_rect.left + (cur_contrast*bar_width/MMISET_CONTRAST_SEVEN) - 1) - MMISET_DISPLAY_CONTRAST_HIGHLIGHT_RIGHT_OFFSET;        
        display_rect.top = display_rect.top + MMISET_DISPLAY_CONTRAST_HIGHLIGHT_TOP_OFFSET;
        display_rect.left = display_rect.left + MMISET_DISPLAY_CONTRAST_HIGHLIGHT_LEFT_OFFSET;
        //display fg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            PNULL,
            win_id,
            IMAGE_SET_CONTRAST_BAR_FG_NEW,
            MMITHEME_GetDefaultLcdDev());
    }   
#else
    GUIRES_GetImgWidthHeight(&bar_width,
        &bar_height,
        IMAGE_SET_CONTRAST_BAR_FG,
        win_id);

    display_x = draw_ptr->ctrl_rect.left;
    display_y = draw_ptr->ctrl_rect.top;
    GUIANIM_AdjustDisplayPosition(&display_x,
        &display_y,
        bar_width,
        bar_height,
        draw_ptr->ctrl_rect,
        GUIANIM_ALIGN_HVMIDDLE);

    //set display rect
    display_rect.left   = display_x;
    display_rect.top    = display_y;
    display_rect.right  = (int16)(display_rect.left + bar_width - 1);
    display_rect.bottom = (int16)(display_rect.top + bar_height - 1);

    if (GUI_IntersectRect(&display_rect,display_rect,draw_ptr->display_rect))
    {
        //set image rect
        img_rect.left   = (int16)(display_rect.left - display_x);
        img_rect.top    = (int16)(display_rect.top - display_y);
        img_rect.right  = (int16)(display_rect.right - display_x);
        img_rect.bottom = (int16)(display_rect.bottom - display_y);

        //display bg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            &img_rect,
            win_id,
            IMAGE_SET_CONTRAST_BAR_BG,
            MMITHEME_GetDefaultLcdDev());


        //set image rect
        img_rect.right = (int16)(img_rect.left + (cur_contrast*bar_width/MMISET_CONTRAST_SEVEN) - 1);

        //display fg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            &img_rect,
            win_id,
            IMAGE_SET_CONTRAST_BAR_FG,
            MMITHEME_GetDefaultLcdDev());
    }
#endif    
}


#if defined MMISET_KEY_BOARD_LIGHT_SUPPORT 
/*****************************************************************************/
//  Description : to handle select keyborad light
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetKeyboardLightWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_KEYBOARD_LIGHT_CTRL_ID,MMISET_KEYBOARD_LIGHT_MAX_ITEM, FALSE );//max item 5
        MMISET_AppendKeyboardLightListItem(MMISET_SET_KEYBOARD_LIGHT_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMISET_SET_KEYBOARD_LIGHT_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMISET_SetKeyboardLight(MMISET_SET_KEYBOARD_LIGHT_CTRL_ID);
        
        //success!
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
        MMK_CloseWin( win_id );
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
#endif

#if defined MMISET_FONT_SUPPORT
/*****************************************************************************/
//  Description : load font
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetFont( 
                                 const uint16* new_name,  //字体路径
                                 BOOLEAN       is_unload, //是否卸载
                                 BOOLEAN       is_prompt  //是否需要提示
                                 )
{
    BOOLEAN result = FALSE;
    wchar font_name[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    if ( is_unload )
    {
        result = MMIRES_UnloadFileFont();
        
        //用户主动卸载时才需要清除nv, u盘时不需要.
        if ( is_prompt )
        {
            SetFontInfo( font_name );
        }
    }
    else
    {
        //get default font
        if ( PNULL == new_name )
        {
            GetFontInfo( font_name );
            if ( 0 != *font_name )
            {
                if ( MMIRES_LoadFileFont( font_name ) )
                {
                    result = TRUE;
                }
                else
                {
                    font_name[0] = 0;
                    SetFontInfo( font_name );
                }
            }
        }
        else
        {
            if ( MMIRES_LoadFileFont( new_name ) )
            {
                SetFontInfo( new_name );
                
                result = TRUE;
            }
        }
    }
    
    //是否需要提示用户
    if ( is_prompt )
    {
        if ( result )
        {
            //MMIPUB_OpenAlertSuccessWin(TXT_SET_FONT_SUCCEED);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_SET_FONT_FAILED);
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get font info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFontInfo( 
                          wchar * font_name_ptr
                          )
{
    BOOLEAN result = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_SUCCESS;
    
    if ( PNULL != font_name_ptr )
    {
        MMINV_READ(MMINV_SET_FONT, font_name_ptr, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {           
            *font_name_ptr = 0;
            MMINV_WRITE( MMINV_SET_FONT, font_name_ptr );
        }
        else
        {
            result = TRUE;
        }
    }
    
    return result;
}
/*****************************************************************************/
//  Description : set font info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFontInfo(
                          const wchar* font_name_ptr
                          )
{
    BOOLEAN result = FALSE;
    
    if ( PNULL != font_name_ptr )
    {
        MMINV_WRITE( MMINV_SET_FONT, font_name_ptr );/*lint !e605*/
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set font
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFontWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_STRING_T    font_str = {0};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_FONT_CTRL_ID;
    wchar font_name[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 cur_index = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE );
        MMIAPISET_AppendListItemByTextIdExt(TXT_SET_DEFAULT_FONT,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMIAPISET_AppendListItemByTextIdExt(TXT_SET_FONT_MORE,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMK_SetAtvCtrl(win_id,ctrl_id);   
        break;
        
    case MSG_FULL_PAINT:
        GetFontInfo( font_name );
        if ( 0 == *font_name )
        {
            cur_index = 0;
        }
        else
        {
            cur_index = 1;
        }
        GUILIST_SetSelectedItem( ctrl_id, cur_index, TRUE );      
        GUILIST_SetCurItemIndex(ctrl_id,cur_index);
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);     
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (0 == GUILIST_GetCurItemIndex(ctrl_id)) 
        {
            MMIAPISET_SetFont( PNULL, TRUE, TRUE );

            MMI_GetLabelTextByLang(TXT_SET_DEFAULT_FONT, &font_str);
            GUILABEL_SetText(MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID, &font_str, FALSE);

            MMK_CloseWin(win_id);
        }
        else
        {
            MMIFMM_SELECT_INFO_T select_info = {{MMIFMM_FONT,0},0,0,PNULL,0,-1};
            
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_FONT;
            select_info.ticked_file_wstr = PNULL;
            select_info.ticked_file_len = 0;
            select_info.win_id = win_id;
            MMIAPIFMM_OpenSelectFontWin(&select_info);

        }
        break;
        
    case MSG_MULTIM_SELECTED_RETURN:
        { 
            MULTIM_SELECT_RETURN_T *selected_info = (MULTIM_SELECT_RETURN_T *)param;
            
            MMIAPISET_SetFont( selected_info->file_name_ptr, FALSE, TRUE );

            MMI_GetLabelTextByLang(TXT_SET_FONT_MORE, &font_str);
            GUILABEL_SetText(MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID, &font_str, FALSE);

            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
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
//  Description : open display setting window
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterDisplaySettingWin(void)
{
    MMK_CreateWin((uint32 *)MMISET_SET_DISPLAY_WIN_TAB, PNULL);
}


/*****************************************************************************/
// Description : open set idle display win
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-21
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSetIdleDisWin(void)
{
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
    MMK_CreatePubListWin((uint32*)MMISET_SET_IDLE_DISPLAY_WIN_TAB,PNULL);
#endif
#endif
}

#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
PUBLIC void MMIAPISET_CreateCustomWordEditWin(void)
{
    MMK_CreateWin((uint32*)MMISET_EDIT_IDLE_WOEDS_WIN_TAB,PNULL);
}
#endif   
#endif

#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : set main menu style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMainmenuStyle(
                            MMI_CTRL_ID_T       ctrl_id
                            )
{
    uint16              cur_selection   =   0;
    MMIMAINMENU_TYPE_E     mainmenu_style  =   MMIMAINMENU_MATRIX_E;
    BOOLEAN             result = FALSE;
    //get the selected item
    //GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    cur_selection = GUISETLIST_GetCurIndex(ctrl_id);

#ifndef MMI_DISPLAYSET_MINI_SUPPORT   
    mainmenu_style = MMIMAINMENU_MATRIX_TITLE_E;
#else
    switch (cur_selection)
    {
    case GUIMENU_STYLE_INDEX_ICON:
        mainmenu_style = MMIMAINMENU_MATRIX_E;
        break;
        
    case GUIMENU_STYLE_INDEX_TITLE_ICON:
        mainmenu_style = MMIMAINMENU_MATRIX_TITLE_E;
        break;

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    case GUIMENU_STYLE_INDEX_GO:
        mainmenu_style = MMIMAINMENU_GO_SLIDE_E;
        break;
#endif
#ifdef MENU_SWEETS_SUPPORT
    case GUIMENU_STYLE_INDEX_SWEETS:
        mainmenu_style = MMIMAINMENU_SWEETS_E;
        break;
#endif    
    default:
        //SCI_TRACE_LOW:"SetMainmenuStyle:cur_selection %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_3679_112_2_18_2_51_6_179,(uint8*)"d",cur_selection);
        break;
    }
#endif
    
    if (mainmenu_style !=MMIAPISET_GetMainmenuStyle())
    {
        //set main menu style
        MMIAPISET_SetMainmenuStyle(mainmenu_style);
        result = TRUE;
    }
    return result;
} 
#endif


/*****************************************************************************/
//  Description : set idle style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetIdleStyle(
                           MMI_CTRL_ID_T  ctrl_id
                           )
{
    BOOLEAN                 result      =   FALSE;
    uint16                  cur_index   =   0;
    MMISET_IDLE_STYLE_E     idle_style  =   MMISET_IDLE_STYLE_COMMON;
    
    //get the selected item
    //GUILIST_GetSelectedItemIndex(ctrl_id,&cur_index,1);
    cur_index = GUISETLIST_GetCurIndex(ctrl_id);
    switch (cur_index)
    {
#ifndef MMI_PDA_SUPPORT    
    case 0:
        idle_style = MMISET_IDLE_STYLE_COMMON;
        break;
        
    case 1:
        idle_style = MMISET_IDLE_STYLE_SMART;
        break;
        
#ifdef MMIWIDGET_SUPPORT
    case 2:
        idle_style = MMISET_IDLE_STYLE_WIDGET;
        break;
#endif

#ifdef MMI_GRID_IDLE_SUPPORT
    case 2:
        idle_style = MMISET_IDLE_STYLE_GRID;
        break;
#endif

#else //MMI_PDA_SUPPORT

#ifdef MMI_GRID_IDLE_SUPPORT
    case 0:
        idle_style = MMISET_IDLE_STYLE_GRID;
        break;
#endif
#if defined MMI_ISTYLE_SUPPORT 
    case 1:
        idle_style = MMISET_IDLE_ISTYLE;
        break;
#endif		
#endif //MMI_PDA_SUPPORT

    default:
        //SCI_TRACE_LOW:"SetIdleStyle:cur_index %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_3747_112_2_18_2_51_6_180,(uint8*)"d",cur_index);
        break;
    }
#ifdef BAR_PHONE_WIDGET
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        if ( idle_style != MMIAPISET_GetPreIdleStyle() )
        {
            //set idle style
            MMINV_WRITE(MMINV_SET_IDLE_FORMER_STYLE,&idle_style);
            result = FALSE;            
        }  
    }
    else
#endif
    if ( idle_style != MMIAPISET_GetIdleStyle() )
    {
        //set idle style
        MMISET_SetIdleStyle(idle_style);

        result = TRUE;
    }

    return result;
}  
#if defined MMISET_MAINMENU_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenuEffectParam(void)
{
    MMI_TEXT_ID_T    mainmenu_effect_txt = {0};
    MMI_CTRL_ID_T   title_ctrl_id = MMISET_DISPLAY_MAINMENU_LABEL_CTRL_ID;
    MMI_CTRL_ID_T   content_ctrl_id = MMISET_DISPLAY_MAINMENU_CONTENT_CTRL_ID;

	//只在andriod下显示设置主菜单的选项
	if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()||MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle())
	{
		GUIFORM_SetStyle(MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(title_ctrl_id, TXT_SET_MAINMENU_STYLE, FALSE);
		mainmenu_effect_txt = MMISET_GetMenuStyleTxt();
		GUILABEL_SetTextById(content_ctrl_id, mainmenu_effect_txt, FALSE);
	}
	else
	{
        GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
	}
}
#endif
#if defined MMISET_UIDLE_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void SetUIdleEffectParam(void)
{
    MMI_TEXT_ID_T   uidle_effect_txt = {0};
    MMI_CTRL_ID_T   title_ctrl_id   = MMISET_DISPLAY_UIDLE_LABEL_CTRL_ID;
    MMI_CTRL_ID_T   content_ctrl_id = MMISET_DISPLAY_UIDLE_CONTENT_CTRL_ID;
#if defined( PDA_UI_SUPPORT_U_IDLE)
	//只在andriod下显示设置主菜单的选项
	if (   MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()
	    || MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
	{
		GUIFORM_SetStyle(MMISET_DISPLAY_UIDLE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(title_ctrl_id, TXT_SET_UIDLE_STYLE, FALSE);
		
		uidle_effect_txt = MMISET_GetUIdleStyleTxt();
		GUILABEL_SetTextById(content_ctrl_id, uidle_effect_txt, FALSE);
	}
	else
#endif     
	{
        GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_DISPLAY_UIDLE_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
	}
}
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenuSelectEffectParam(void)
{
    MMI_TEXT_ID_T    select_effect_txt = {0};
    MMI_CTRL_ID_T   title_ctrl_id = MMISET_MAINMENU_SELECT_STYLE_LABEL_CTRL_ID;
    MMI_CTRL_ID_T   content_ctrl_id = MMISET_MAINMENU_SELECT_STYLE_CONTENT_CTRL_ID;

	//只在andriod下显示设置选项
	//if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
	{
		GUIFORM_SetStyle(MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(title_ctrl_id, TXT_SET_MAINMENU_SELECT_STYLE, FALSE);
		select_effect_txt = MMISET_GetMenuSelectStyleTxt();
		GUILABEL_SetTextById(content_ctrl_id, select_effect_txt, FALSE);   
	}
	//else
	{
    //    GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
	}
}

#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : MMIAPISET_IsPoweronCall
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: copy MMIAPISET_IsWallpaperCall
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPoweronCall(void)
{
    return s_fmm_poweron_call;
}
/*****************************************************************************/
//  Description : MMIAPISET_IsPoweroffCall
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: copy MMIAPISET_IsWallpaperCall
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPoweroffCall(void)
{
    return s_fmm_poweroff_call;
}
#endif


PUBLIC void MMISet_OpenSetPowerOnAnimWin(void)
{
    MMK_CreatePubListWin((uint32*)MMISET_SET_POWER_ON_ANIM_WIN_TAB,PNULL);
}

PUBLIC void MMISet_OpenSetPowerOffAnimWin(void)
{
    MMK_CreatePubListWin((uint32*)MMISET_SET_POWER_OFF_ANIM_WIN_TAB,PNULL);
}


/*Edit by script, ignore 1 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527


/*Edit by script, ignore 12 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
