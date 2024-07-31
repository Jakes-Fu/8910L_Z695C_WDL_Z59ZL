/****************************************************************************
** File Name:      mmk_timer.h                                             *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct and       *
**                  function of timer                                      *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create
** 
****************************************************************************/
#ifndef  _MMK_EXTERN_APP_H_    
#define _MMK_EXTERN_APP_H_   

/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

    extern MMI_APPLICATION_T    g_cc_app;
    extern MMI_APPLICATION_T    g_mmiphone_app;  
    extern MMI_APPLICATION_T    g_mmiset_app;
    extern MMI_APPLICATION_T    g_main_app;
    extern MMI_APPLICATION_T    g_mmisms_app;
    extern MMI_APPLICATION_T    g_mmipb_app;
    // add by allen  2004.08.18  begin
    extern MMI_APPLICATION_T	g_mmistk_app;
    extern MMI_APPLICATION_T	g_mmieng_app;
    //add by figo.feng 2004.09.24
    //forinline edit
    extern MMI_APPLICATION_T	 g_ctrl_inlineedit;

#ifdef QBTHEME_SUPPORT
	   extern MMI_APPLICATION_T g_mmiqbtheme_app;
#endif
    
#ifdef MMI_ENABLE_DCD
    extern MMI_APPLICATION_T    g_mmidcd_app;
#endif
#ifdef CMCC_UI_STYLE
    extern MMI_APPLICATION_T    g_mmics_app;
#endif

#ifdef BROWSER_SUPPORT_DORADO
    extern MMI_APPLICATION_T	g_mmibrowser_app;
#endif

#ifdef OAUTH_SUPPORT
    extern MMI_APPLICATION_T	g_net_proxy_app;
#endif

#ifdef PIM_SUPPORT 
extern MMI_APPLICATION_T g_mmipim_app;  
#endif

#ifdef DL_SUPPORT
    extern MMI_APPLICATION_T	g_download_app;
#endif

#ifdef BLUETOOTH_SUPPORT
	//add by wenming 2005.12.28
    extern MMI_APPLICATION_T    g_bluetooth_app;
#endif
    //add control panel
    extern MMI_APPLICATION_T    g_control_panel_app;
	extern MMI_APPLICATION_T    g_nps_app;
#ifdef MMI_OCR_ENABLE
	extern MMI_APPLICATION_T    g_ocr_app;
#endif
#ifdef CMMB_SUPPORT
    extern MMI_APPLICATION_T    g_mtv_app;
#endif
#ifdef ATV_SUPPORT
    extern MMI_APPLICATION_T    g_atv_app;
#endif
#ifdef WRE_SUPPORT
    extern MMI_APPLICATION_T    g_wre_asyn_app;
    extern MMI_APPLICATION_T    g_wre_videoplayer_app;
    #ifdef QBTHEME_SUPPORT
    extern MMI_APPLICATION_T    g_wre_qbtheme_app;
    #endif
#endif
#ifdef MMI_GPRS_SUPPORT
    extern MMI_APPLICATION_T	g_pdp_app;
#endif
#ifdef MMS_SUPPORT_DORADO
    extern MMI_APPLICATION_T	g_mmimms_app;
#endif
    extern MMI_APPLICATION_T	g_caf_asyn_app;
#ifdef FDN_SUPPORT
    extern MMI_APPLICATION_T g_mmifdn_app;
#endif
#ifdef BROWSER_SUPPORT_NETFRONT
    extern MMI_APPLICATION_T g_mmiwww_app;
#endif
#ifdef WIFI_SUPPORT
    extern MMI_APPLICATION_T	g_mmiwifi_app;
#endif
#ifdef VT_SUPPORT
    extern MMI_APPLICATION_T g_mmivt_app;
#endif
#ifdef VIDEO_PLAYER_SUPPORT
    extern MMI_APPLICATION_T    g_mmivp_app;  
#endif

#ifdef PUSH_EMAIL_SUPPORT 
    extern MMI_APPLICATION_T g_mmimail_app;
#endif /* PUSH_EMAIL_SUPPORT  */

#ifdef MOBILE_VIDEO_SUPPORT
     extern MMI_APPLICATION_T    g_mv_app;
#endif 

#ifdef WEATHER_SUPPORT
     extern MMI_APPLICATION_T   g_weather_app;
#endif 
#ifdef MMIEMAIL_SUPPORT
     extern MMI_APPLICATION_T    g_mmiemail_app;  
#endif

#ifdef WRE_SUPPORT
     extern MMI_APPLICATION_T    g_wre_app;
#endif 
#ifdef MENU_CRYSTALCUBE_SUPPORT
     extern MMI_APPLICATION_T    g_mmicrystalcube_app;
#endif
#ifdef MENU_CYCLONE_SUPPORT
     extern MMI_APPLICATION_T    g_mmicyclone_app;
#endif

#ifdef DYNAMIC_MODULE_SUPPORT  
extern MMI_APPLICATION_T    g_caf_tapi_app;
#endif

#ifdef OPERA_MOBILE_SUPPORT_V11
     extern MMI_APPLICATION_T    g_opera_mobile_app;
#endif 

#if defined(OPERA_MINI_SUPPORT_VER42) || defined(OPERA_MINI_SUPPORT_VER6)
     extern MMI_APPLICATION_T    g_opera_mini_app;
#endif 

#ifdef BROWSER_SUPPORT
    extern MMI_APPLICATION_T g_mmibrowser_manager_app;
#endif

#ifdef SXH_APPLE_SUPPORT
    extern MMI_APPLICATION_T   g_apple_app;         
#endif

#ifdef SALES_SUPPORT
    extern MMI_APPLICATION_T   g_sales_app;
#endif

#ifdef FOTA_SUPPORT
    extern MMI_APPLICATION_T   g_fota_app;
#endif

#ifdef SFR_SUPPORT
    extern MMI_APPLICATION_T   g_sfr_app;
#endif

#ifdef CMCCDM_SUPPORT
    extern MMI_APPLICATION_T   g_cmccdm_app;
#endif

#ifdef MMI_REFACTOR_SAMPLE
    extern MMI_APPLICATION_T    g_appsample_app;
#endif

#ifdef MMISRVAP_SUPPORT
extern MMI_APPLICATION_T    g_mmisrv_ap;
#endif

#ifdef MMIWNS_SUPPORT
     extern MMI_APPLICATION_T   g_wns_app;
#endif 
#ifdef CAMERAROLL_SUPPORT
extern MMI_APPLICATION_T g_camera_roll;
#endif
#ifdef MSA_SUPPORT
extern MMI_APPLICATION_T    g_msa_app;
#endif

#ifdef MMIPB_SYNC_WITH_O365
    extern MMI_APPLICATION_T	g_contactsync_app;
#endif

#ifdef BROWSER_SUPPORT_WEBKIT
extern MMI_APPLICATION_T g_wkmmi_app;
#endif
#ifdef HERO_ENGINE_SUPPORT
extern MMI_APPLICATION_T gHeroEngineAppModlue;
#endif
#ifdef HERO_QR_DECODE_SUPPORT
extern MMI_APPLICATION_T  gHeroQrDecodeModlue;
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT
extern MMI_APPLICATION_T  gHeroTtsApp;
#endif
//zmt add start
#ifdef ZDT_HTTP_APP_SUPPORT
extern MMI_APPLICATION_T    g_zdthttp_app;
#endif
//zmt add end
/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif
