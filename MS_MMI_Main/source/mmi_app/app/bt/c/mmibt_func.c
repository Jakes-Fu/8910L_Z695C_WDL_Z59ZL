/****************************************************************************
** File Name:     mmibt_func.c                                        *
** Author:         Wenming.feng                                            *
** Date:           12/14/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** Dec/14/2005    Wenming.feng     Create                                  *
**                                                                         *
****************************************************************************/
#define _MMIBT_FUNC_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmibt_mainwin.h"
#include "mmibt_app.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的踞构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "mmiset_export.h"
#include "guitext.h"

#include "mmivcard_export.h"
#include "mmieng_export.h"
#include "mmicl_export.h"
#include "mmiphone_export.h"
#include "mmibt_a2dp.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmibt_nv.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmipicview_export.h"
#include "mmialarm_export.h"
#include "mmi_mainmenu_export.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "mmidc_export.h"
#include "guiedit.h"
#include "mmiidle_export.h"
#include "mmimtv_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmieng_id.h"
#include "guictrl_api.h"
#include "mmieng_text.h"
#include "mmienvset_export.h"
#include "mime_type.h"
#include "mmisms_export.h"

#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif
#include "mmikl_export.h"
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmiidle_statusbar.h"

#include "mmimultim_image.h"
#include "mmisrvaud_api.h"
#include "mmi_nv.h"
#include "mmibt_headset.h"
#include "mmisrvfmm_export.h"
#if defined MMI_VCARD_SUPPORT
#include "mmiebook_file.h"
#endif
#include "mmicom_banner.h"
#include "mmipb_id.h"
#include "variant_config_nv.h"

#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMIBT_SCROLL_BAR_DEAFULT_LEN    2

#define  MMIBT_VIBRA_DEFAULT_TIME           1500

#define MMIBT_BT_DIR_NAME               "Bt"
#define MMIBT_REPEAT_NAME_SUFFIX_LEN     16

#define MMIBT_FILE_NAME_NO_SUFFIX_LEN   100

#define MMIBT_FTP_ALL_ROOT          "*"
#define MMIBT_DIR_BT_LEN                2


#define  MMIBT_PIC_TYPE_MAX_NUM           10            //The number of picture file type
#define  MMIBT_RING_TYPE_MAX_NUM          14            //The number of ring file type
#define  MMIBT_MOVIE_TYPE_MAX_NUM         4             //The number of movie file type
#define  MMIBT_TEXT_TYPE_MAX_NUM          2             //The number of text file type
#define  MMIBT_JAVA_TYPE_MAX_NUM          4             //The number of java file type
#define  MMIBT_VCARD_TYPE_MAX_NUM         2             //The number of vcard file type

#define MMIBT_SET_VISIBILITY_HIDDEN     0
#define MMIBT_SET_VISIBILITY_VISIBLE     1

#define MMIBT_FTP_REQ_DEVICE_MAX_LEN        20
#define MMIBT_FTP_REQ_FILE_MAX_LEN          40

//"Ebook"
//#define MMIBT_SIZE_INFO_RECT            {MMIBT_LABEL_LEFT,MMIBT_LABEL_SIZE_Y,MMI_MAINSCREEN_RIGHT_MAX_PIXEL, (MMIBT_LABEL_SIZE_Y + MMIBT_NEW_FILE_LINE_HEIGHT)}
//define all menu item number



#define MMIBT_ANIM_PLAY                 1
#define MMIBT_MAX_QUERY_NUM             20//所允许打开的最大询问的窗口数目

#ifdef MMI_BT_PBAP_SUPPORT
#define MMIBT_DISCONNECT_RETRY_COUNT    5

LOCAL const char s_pb_name[] = {'t','e','l','e','c','o','m','\/','p','b','.','v','c','f',0}; /*lint !e606*/
LOCAL const char s_sim_name[] = {'S','I','M','1','\/','t','e','l','e','c','o','m','\/','p','b','.','v','c','f',0}; /*lint !e606*/
#define MMIBT_PBAP_VCARD_FORMAT_21         BT_VCARD_FORMAT_21         
LOCAL     BT_PBAP_DATA_IND_T  pbap = {0};   //for test    
#endif

//#define MMIBT_FTP_BUSY      	        3
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


extern HAUDIODEV hPCMDev; /*lint !e762*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static  BOOLEAN     s_fts_del_wallpaper_file = FALSE;


//BT Interaction begin
static  MMIBT_TRANSFER_FILE_INFO_T    s_transfer_file_info = {0};            //info of transfer file: file name, file size, service type
//lulifei add for 510562
static  MMIBT_TRANSFER_FILE_INFO_T    s_transfer_vcf_file_info = {0};
LOCAL BOOLEAN s_is_vcf_file = FALSE;

static MMIBT_TRANSFER_STATUS_TYPE_E s_bt_transfer_status = MMIBT_TRANSFER_END;
BT_DEVICE_INFO     g_current_bt = {0};

#ifdef BT_DUN_SUPPORT
static  BOOLEAN                      s_is_vdun_on = FALSE;
#endif

LOCAL BOOLEAN s_is_permit_open = FALSE;
LOCAL BOOLEAN MMIbt_resetflag = FALSE;
#ifdef MMI_BT_PBAP_SUPPORT
LOCAL uint8 s_hfu_reco_timer = 0;
#define BT_HFU_RECO_TIMER_OUT        (6*1000)        //HFU重连间隔时长
LOCAL uint8 s_hfu_reco_count = 0;
#define BT_HFU_RECO_COUNT            20
LOCAL BOOLEAN s_is_auto_reco = FALSE;
#define BT_HFU_AUTO_CONNECT_TIMER_OUT        (10*1000)        //HFU重连间隔时长
LOCAL uint8 s_bt_hfu_auto_connect_timer = 0;
#if (defined MMI_SMS_SYN_SUPPORT) && (defined BT_SPP_SUPPORT)
LOCAL BOOLEAN s_is_sms_sync = FALSE;
#endif

LOCAL BOOLEAN s_is_cancel_pbap_pull = FALSE; 
LOCAL BT_PBAP_TYPE_E s_pbap_connect_type = BT_PBAP_TYPE_NONE; 
LOCAL uint8 s_bt_connect_timeout_timer = 0;
LOCAL BOOLEAN s_is_pbap_pull_phone = TRUE;  
#endif
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:  clean all hide device
//  Global
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
// LOCAL void CleanAllHideDevice(void);

/*****************************************************************************/
//  Description : is premit open this file or not
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/


LOCAL BOOLEAN Is_PermitOpenFile(void);

#ifdef BT_DUN_SUPPORT
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDUNWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id,
                                                    MMI_MESSAGE_ID_E msg_id,
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Discription: Handle try pair again or not win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleConnectDUNOrNotMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM param
                                                   );
#endif

/*****************************************************************************/
//  Discription:  close send file windows
//  Global resource dependence: None
//  Author:
//  Note : if current opertation is opp_send, close inquery list and device list
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN  CloseSendWin(void);



/*****************************************************************************/
//  Discription:display memory or udisk not exist or sd card not exist
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMemoryInformation(MMIFILE_DEVICE_E filedev);


/*****************************************************************************/
//  Discription: Handle send connect waiting win msg
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendConnectWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Discription: Get the show string of lock status
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleListMenuWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );
#endif
/*****************************************************************************/
//  Discription: Handle blue tooth ui test result win msg
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL   MMI_RESULT_E HandleBtUiResultWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );


/*****************************************************************************/
//  Discription: Handle blue tooth incoming file win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIncomingFileWinMsg(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );

/*****************************************************************************/
//  Discription: Handle blue tooth new file win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleNewFileWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

/*****************************************************************************/
//  Discription:start vibrator
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
//LOCAL void StartVibrator(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Discription: set bt operation to backgroup runing
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
//LOCAL void  SetBackgroupRun(BOOLEAN is_enable);

///*****************************************************************************/
////  Discription:  get bt backgroup runing or not
////  Global resource dependence: None
////  Author: kelly.li
////  Note :
////  Return:
///*****************************************************************************/
// LOCAL BOOLEAN  GetIsBackgroupRun(void);
/*****************************************************************************/
//  Discription:stop vibrator timer
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
//LOCAL void StopVibratorTimer(void);

/*****************************************************************************/
//  Discription: Handle ftp device serive list win msg
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleTextBoxWinMsg(
                                                      MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                      );

/*****************************************************************************/
//  Discription: set if delete wallpaper file by fts
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL void  SetIsDelWallpaperFile(BOOLEAN   is_del_wallpaper);

/*****************************************************************************/
//  Discription: get if delete wallpaper file by fts
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN  GetIsDelWallpaperFile(void);

/*****************************************************************************/
//  Discription: Set selected blue tooth visibility
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL void  SetBtVisibility(MMIBT_VISIBBILE_TYPE  type);
// @baokun fix cr96895 begin
/*****************************************************************************/
//  Discription: Handle no space for incoming file alert win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoSpaceForFileAlertMsg(
                                               MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param
                                               );
// @baokun fix cr96895 end


/*****************************************************************************/
//  Discription: Handle delete paired device or not win msg
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL void DeletePairedDevice(BOOLEAN is_all,MMIBT_OPERTATION_TYPE_E  current_opt);
/*****************************************************************************/
//  Discription:
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN MMIBT_IsEnoughSpaceToRecieve(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : open direct and get user selected dir
//  Global resource dependence :
//  Author:
//  Note: after user selected a dir, it will be set to shaired directory
/*****************************************************************************/
PUBLIC void MMIBT_SetFileLocationCallBack( BOOLEAN     is_success,
                                    FILEARRAY   file_array
                                    );


/*****************************************************************************/
//  Discription: None
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleSaveNewFileFailedWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );


#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_GetPairedDevAddr
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_GetPairedDevAddr(BT_ADDRESS *dev_addr);

/*****************************************************************************/
//  Description : MMIBT_StartRecoTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_StartRecoTimer(void);

/*****************************************************************************/
//  Description : MMIBT_StopRecoTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_StopRecoTimer(void);

/*****************************************************************************/
//  Description : MMIBT_HFUReconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_RecoTimeOut(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : MMIBT_HFUReconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_HFUReconnect();

/*****************************************************************************/
//  Description : handle headset close result
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void handleHandsetGatewayCloseResult(BT_STATUS bt_status, BT_ADDRESS *headset_addr_ptr);

/*****************************************************************************/
//  Description : ConnectHandsetGatewayService
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ConnectHandsetGatewayService(void);

/*****************************************************************************/
//  Description : StopAutoConnectHandsetGatewayTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StopAutoConnectHandsetGatewayTimer(void);

/*****************************************************************************/
//  Description : StartAutoConnectHandsetGatewayTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StartAutoConnectHandsetGatewayTimer(void);

/*****************************************************************************/
//  Description : close headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BT_STATUS CloseHandsetGateway(const BT_ADDRESS *src_addr);
#endif

LOCAL MMI_RESULT_E HandleBTNoDeviceWarningWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param) ;

LOCAL MMI_RESULT_E HandleConnectCancelingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param) ;

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none
//  Author: lifei.lu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinCancelingMsg(
                                          MMI_WIN_ID_T         win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );
/*****************************************************************************/
// 	Description : handle vcard win
//	Global resource dependence : none
//  Author: lifei.lu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewVcardWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : abort the vcard import
//	Global resource dependence : none
//  Author: lifei.lu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleImportVcardWaitingBtWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
/**************************************************************************************************/
// Description:import vcard call back function
// Date:
// Author:lifei.lu
// Note:
//*************************************************************************************************/
LOCAL void  Importvcard_Callback (MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);

#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_PBPbapParserCallback(MMIPB_PBAP_PARSER_RESULT_T* result);

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIBT_PbapDisconnect(void);

/*******************************************************************************/
//  Description : generate whole display string
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_GenerateDisplayInfo(wchar *info_ptr,uint32 sync_contact_count);

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinDirectConnectWaitingMsg(
                                                       MMI_WIN_ID_T         win_id, 
                                                       MMI_MESSAGE_ID_E    msg_id, 
                                                       DPARAM              param
                                                       );
#endif

/*---------------------------------------------------------------------------*3
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Set the status that: blue tooth is hidden
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
//LOCAL void SetHiddenBtDevice(BOOLEAN    hidden_bt_device)
//{
//    s_bt_hidden_for_app = hidden_bt_device;
// }

/*****************************************************************************/
//  Discription: get the status that: blue tooth is hidden because of other application
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
//LOCAL BOOLEAN  GetIsBtHiddenForApp(void)
//{
//    return (s_bt_hidden_for_app);
//}
/*****************************************************************************/
/*****************************************************************************/
//  Discription: close all pb window except main window
//  Global resource dependence: none
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CloseAllWindow(void)
{
    BT_CancelSearch();
    if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_PIN_INPUT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SET_SAVE_LOCATION_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SET_SAVE_LOCATION_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_RECEIVING_FILE_WIN_ID))
    {
        MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_INCOMING_FILE_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_DEVICE_DETAILS_WIN_ID))
    {
        MMK_CloseWin(MMIBT_DEVICE_DETAILS_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_INQUIRY_OPT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INQUIRY_OPT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_TEXT_BOX_WIN_ID))
    {
        MMK_CloseWin(MMIBT_TEXT_BOX_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SET_VISIBILITY_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SET_VISIBILITY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SHARED_DIRECTORY_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SHARED_DIRECTORY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SHARED_AUTHORITY_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SHARED_AUTHORITY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SHARED_AUTHORITY_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SHARED_AUTHORITY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SHARED_FILE_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SHARED_FILE_WIN_ID);
    }
}
/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
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

/*****************************************************************************/
//  Discription: Set selected blue tooth visibility
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL void  SetBtVisibility(MMIBT_VISIBBILE_TYPE  type)
{
    BT_STATUS               ret_val = BT_SUCCESS;
    MMI_STRING_T            wait_text = {0};
    MMI_WIN_ID_T               visible_win = MMIBT_VISIBLE_WAIT_WIN_ID;
    MMI_WIN_PRIORITY_E         win_priority_ptr = WIN_ONE_LEVEL;

    MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
    //if(0 == cur_select)
    if(MMI_BT_HIDE == type)
    {
        ret_val = BT_SetVisibility(BT_PAGE_ENABLE);//set blue tooth hidden
    }
    else
    {
        //Set Visibility
        ret_val = BT_SetVisibility((BT_INQUIRY_ENABLE | BT_PAGE_ENABLE));
    }
    if ( BT_PENDING == ret_val)
    {
        //lulifei modify for change the style
        if(MMK_IsOpenWin(MMIBT_MAIN_MENU_WIN_ID))
        {
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_VISIBLE_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
        }
        //MMIPUB_OpenProgressWaitingIndWinByTextId(TXT_COMMON_WAITING,&visible_win,&win_priority_ptr,MMIPUB_SOFTKEY_NONE,PNULL);
    }
    else
    {
    	MMIDEFAULT_SetBackLight(TRUE);
#ifndef MMI_PDA_SUPPORT
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
#endif
    }
}
/*****************************************************************************/
//  Discription: send next file with blue tooth
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SendNextFile(
                            	const wchar     *filepath_name,      //in
                            	uint16           filepath_len,       //in
                            	uint32           file_size,           //in
					BOOLEAN          is_temp_file        //in
                           )
{
	if (PNULL == filepath_name)
	{
		//SCI_TRACE_LOW:"MMIBT_SendNextFile PNULL == filepath_name!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_3882_112_2_18_2_8_8_241,(uint8*)"");
		return ;
	}
	//SCI_TRACE_LOW:"[&&&]MMIBT_SendNextFile start,file_size = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_3885_112_2_18_2_8_8_242,(uint8*)"d",file_size);
	//get file  name info
	SCI_MEMSET(s_transfer_file_info.transfer_file_name, 0x00, sizeof(s_transfer_file_info.transfer_file_name));
	MMI_WSTRNCPY(s_transfer_file_info.transfer_file_name, MMIFILE_FULL_PATH_MAX_LEN, filepath_name, filepath_len, MIN(filepath_len, MMIFILE_FULL_PATH_MAX_LEN));
    s_transfer_file_info.is_temp_file = is_temp_file;
	//store send file size info
	s_transfer_file_info.total_file_size = file_size;
    MMIBT_SetOpertor(MMIBT_OPC_OPP_SEND, MMIBT_OPC_OPP_SEND);
	DoOperation(MMIBT_OPC_OPP_SEND,TRUE);
}

/*****************************************************************************/
//  Discription: send file with blue tooth
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SendFile(
                            const wchar     *filepath_name,      //in
                            uint16           filepath_len,       //in
                            uint32           file_size,           //in
							BOOLEAN          is_temp_file        //in
                           )
{
    MMI_STRING_T         wait_text = {0};

    if (PNULL == filepath_name)
    {
        //SCI_TRACE_LOW:"MMIBT_SendFile PNULL == filepath_name!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_3917_112_2_18_2_8_8_243,(uint8*)"");
        return ;
    }
    if(MMIAPIBT_IsOpenFileWin())
	{
		MMIPUB_OpenAlertWarningWin(TXT_BT_TRANSFER);
		return;
	}
    if(MMK_IsOpenWin(MMI_BT_QUERY_WIN_ID))
    {
        MMK_SendMsg(MSG_APP_CANCEL, MMI_BT_QUERY_WIN_ID, PNULL);
        MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
    }
    //get file  name info
    SCI_MEMSET(s_transfer_file_info.transfer_file_name, 0x00, sizeof(s_transfer_file_info.transfer_file_name));
    MMI_WSTRNCPY(s_transfer_file_info.transfer_file_name, MMIFILE_FULL_PATH_MAX_LEN, filepath_name, filepath_len, MIN(filepath_len, MMIFILE_FULL_PATH_MAX_LEN));
    s_transfer_file_info.is_temp_file = is_temp_file;

    //store send file size info
    s_transfer_file_info.total_file_size = file_size;

    SCI_TRACE_BUF("[MMIBT] [MMIBT_SendFile] transfer_file_name = ", s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)*sizeof(wchar));
    //check whether the blue tooth device is turn on
    if(BT_GetState())//blue tooth is power on
    {
        //open paired blue tooth device
        MMIBT_EnterInquireDeviceListWin(&s_transfer_file_info,
                            MMIBT_OPC_OPP_SEND,
                            BT_SERVICE_FTP);
    }
    else//blue tooth is power off
    {
        //power on BT first
        MMI_GetLabelTextByLang(STR_BT_TURN_ON, &wait_text);
        MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_POWER_ONOFF);
    }
}

/*****************************************************************************/
//  Discription: set if delete wallpaper file by fts
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL void  SetIsDelWallpaperFile(BOOLEAN   is_del_wallpaper)
{
    s_fts_del_wallpaper_file = is_del_wallpaper;
    //SCI_TRACE_LOW:"SetIsDelWallpaperFile: Set del file is wallpaper = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4000_112_2_18_2_8_8_244,(uint8*)"d",is_del_wallpaper);
}

/*****************************************************************************/
//  Discription: get if delete wallpaper file by fts
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN  GetIsDelWallpaperFile(void)
{
    //SCI_TRACE_LOW:"GetIsDelWallpaperFile: Get file is wallpaper = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4010_112_2_18_2_8_8_245,(uint8*)"d",s_fts_del_wallpaper_file);
    return(s_fts_del_wallpaper_file);
}
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsDelReq(BT_MSG_T    *msg_body_ptr)
{
    BT_FILE_INFO    file_info  = {0};
    MMI_STRING_T    del_query_text = {0};
	MMI_STRING_T    name_text = {0};
    uint16          file_name_len = 0;
    uint16          file_path_len = 0;
    BOOLEAN       be_authorize = FALSE;

    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_FtsDelReq PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4030_112_2_18_2_8_8_246,(uint8*)"");
        return ;
    }

	file_info  = *((BT_FILE_INFO*) msg_body_ptr->body_ptr);
    /*if(MMK_IsFocusWin(MMIBT_SENDING_FILE_WAIT_WIN_ID)
		|| MMK_IsFocusWin(MMIBT_RECEIVING_FILE_WIN_ID)
		|| MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
		|| MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
		)*/
     SCI_TRACE_LOW("MMIBT_FtsDelReq %d", MMIBT_GetTransferStatus());
     if(((MMIBT_GetTransferStatus() != MMIBT_TRANSFER_END)
	 	&&(MMIBT_GetTransferStatus() != MMIBT_TRANSFER_FREE))
	 	|| MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
		|| MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
		)
    {
        //In sending or Recieving file, cancel delete atcion
        BT_DeleteFile(FALSE);
    }
    else
    {
        CloseSendWin();
        //del_query_text.wstr_ptr = del_buf;
        //turn on the back light
        MMIDEFAULT_SetBackLight(TRUE);
        //get prompt string: Delete + name ?
        //ConnectUSC2String(TXT_DELETE, file_info.name, TXT_NULL, (MMIBT_FULL_PATH_LEN + MMIBT_FTP_REQ_DEVICE_MAX_LEN), &del_query_text);
        //set if is delete wallpaper file
        file_name_len = MMIAPICOM_Wstrlen((wchar*)file_info.name);
        if(MMIAPIFMM_IsWallPaperFile((wchar*)file_info.name, file_name_len))
        {
            SetIsDelWallpaperFile(TRUE);
        }
        MMI_GetLabelTextByLang(STXT_SOFTKEY_DELETE_MK, &del_query_text);
        name_text.wstr_ptr = file_info.name;
        name_text.wstr_len = file_name_len;
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_FTS_DLE);
        SCI_MEMSET(s_transfer_file_info.transfer_file_name, 0x00, sizeof(s_transfer_file_info.transfer_file_name));

        MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name,file_info.name,file_name_len);

        BT_GetDeviceAuthorisation(&(file_info.remote_dev.addr),&be_authorize);
        if(be_authorize)
        {
            DoOperation(MMIBT_OPC_FTS_DLE, TRUE);
        }
        else
        {
            MMIBT_OpenQueryWin(MMI_BT_FTS_QUERY_WIN_ID, &del_query_text, &name_text, MMIBT_OPC_FTS_DLE);
        }
    }
}
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsConnectReq(BT_MSG_T *msg_body_ptr)
{
    DoOperation(MMIBT_FTS_CONNECT_REQ,TRUE);
}
#endif//MMIBT_FTS_SUPPORT

// @baokun fix cr96895 begin
/*****************************************************************************/
//  Discription: Handle no space for incoming file alert win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoSpaceForFileAlertMsg(
                                               MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	static uint8  timer_id = 0;

    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
	    timer_id = MMK_CreateTimer(MMIBT_VIBRA_DEFAULT_TIME, FALSE);
		result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);;
        MMIPUB_SetWinSoftkey(win_id, STXT_SOFTKEY_OK_MK, TXT_NULL, FALSE);
		break;
    case MSG_APP_RED:
        MMIBT_CancelFileTransfer(TRUE);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMIBT_OpenFMMDirectory();
		MMK_CloseWin(win_id);
        break;
     case MSG_LOSE_FOCUS:
		 break;
    case MSG_TIMER:
        if (timer_id == *(uint8*)param)
        {
            MMIBT_OpenFMMDirectory();
			MMK_CloseWin(win_id);
        }
		else
		{
			result = MMI_RESULT_FALSE;
		}
        //result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	case MSG_CLOSE_WINDOW:
		MMK_StopTimer(timer_id);
		timer_id = 0;
		result = MMI_RESULT_FALSE;
		break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : set ftp shared directory
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIBT_SETTING_RET MMIBT_OpenFMMDirectory(void)
{
    MMIFMM_FILTER_T filter = {0};
    uint32          path_len = 0;
    FILEARRAY       ret_array = NULL;
    MMIBT_SETTING_RET result = MMIBT_SUCCESS;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/

    if(MMIAPIUDISK_UdiskIsRun())
    {
        result = MMIBT_UDISK_RUNING;
    }

    if((MMIAPIFMM_GetFirstValidDevice() >= MMI_DEVICE_NUM))
    {
        result = MMIBT_UDISK_SDCARD_NOT_EXIST;
    }

    //selected folder
    MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
    path_len = MMIAPICOM_Wstrlen((wchar *)setting.dir.shaired_path_name);
    SCI_MEMCPY(filter.filter_str, "*", strlen("*"));/*lint !e666*/
    //get directory from FMM
    ret_array = MMIAPIFILEARRAY_Create();
    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(setting.dir.shaired_path_name, path_len,
               &filter, FUNC_FIND_FOLDER,FALSE, NULL, ret_array, MMIBT_SetFileLocationCallBack,FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
    //SCI_TRACE_LOW:"MMIBT_OpenFMMDirectory: result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11309_112_2_18_2_8_23_361,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Discription: Handle send connect waiting win msg
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendConnectWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    BT_STATUS   return_value = BT_PENDING;
    GUI_BOTH_RECT_T both_rect = {0};

   MMI_STRING_T  send_cancel_string = {0};
   MMI_WIN_ID_T               seng_cancel_win = MMIENG_BT_CANCEL_WAIT_WIN_ID;
   MMI_WIN_PRIORITY_E         win_priority_ptr = WIN_ONE_LEVEL;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	    both_rect = MMITHEME_GetFullScreenBothRect();
        MMK_SetBothRect(win_id, &both_rect);
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
		recode = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
		MMIPUB_SetWinSoftkey(win_id, PNULL, STXT_SOFTKEY_CANCEL_MK, FALSE);
		recode = MMI_RESULT_FALSE;
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
	break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        //User refuse to accept the file
        return_value = MMIBT_CancelFileTransfer(TRUE);
        BtResumeBGPlay(MMIBT_OPP_SUSPEND);
         if(BT_PENDING == return_value)
         {
                //need wait
               //OpenCancelWaitingWin();
               MMI_GetLabelTextByLang(TXT_COMMON_CANCELING, &send_cancel_string);
               MMIPUB_OpenProgressWaitingIndWinByTextPtr(&send_cancel_string,&seng_cancel_win,&win_priority_ptr,MMIPUB_SOFTKEY_DIALOG_CLOSE,HandleConnectCancelingWinMsg);
         }
         else
         {
             if(BT_SUCCESS ==return_value)
             {
                MMIPUB_OpenAlertSuccessWin(STXT_SOFTKEY_CANCEL_MK);
             }
             else
             {
                //SCI_TRACE_LOW:"[&&&]HandleSendConnectWindow"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4464_112_2_18_2_8_9_250,(uint8*)"");
                MMIPUB_OpenAlertFailWin(STR_BT_SEND_FAIL);
             }
         }
	if (MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID)){
             MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
	}
#ifdef MMI_PDA_SUPPORT
         if(msg_id==MSG_APP_RED)
         {
             //need back to idle
            recode = MMI_RESULT_FALSE;
         }
         else
         {
            MMK_CloseWin( win_id );
         }
#else
         MMK_CloseWin( win_id );
#endif
        break;
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
		recode = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    default:
        recode = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;

}
/*****************************************************************************/
//  Discription: Handle receive file waiting win msg
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN  AssignReceiveFileCfg(MMIBT_FILE_SERVER_TYPE_E       service_type, BOOLEAN is_overwrite)
{
    BT_STATUS           return_result = BT_SUCCESS;
    //wchar               file_full_path_ptr[MMIBT_FULL_PATH_LEN +1] = {0};
	MMI_STRING_T        query_text = {0};

    //SCI_TRACE_LOW:"AssignReceiveFileCfg service_type %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4542_112_2_18_2_8_9_251,(uint8*)"d",service_type);
    BtPauseBGPlay(MMIBT_FTP_SUSPEND);
    if (MMIBT_FILE_SERVER_OPP_RECIEVE == service_type)
    {
        //OPP Recieved
		if (!is_overwrite && MMIAPIFMM_IsFileExist(s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)))
		{

			MMI_GetLabelTextByLang(STR_FILE_EXIST_QUERY, &query_text);
			MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &query_text, PNULL, MMIBT_OPC_RECIEVE);
			return FALSE;
		}
		else
		{
			//call api to receive file
			return_result = BT_OppReceiveFile((uint16*)s_transfer_file_info.transfer_file_name);
         SCI_TRACE_LOW("AssignReceiveFileCfg, BT_OppReceiveFile, return_result = %d", return_result);
		}
    }
    else if (MMIBT_FILE_SERVER_FTP_RECIEVE == service_type)
    {
        return_result = BT_FtpReceiveFile((uint16*)s_transfer_file_info.transfer_file_name);
    }
    if (BT_PENDING == return_result)
    {

         MMIBT_SetTransferStatus(MMIBT_RECEIVEING_FILE_FOREGROUND);
		 MMIBT_UpdateMainMenu(TRUE);
        //set hidden for blue tooth for receive file
        //MMIBT_HiddenBtForAPP(MMIBT_MODULE_BLUETOOTH,BT_PAGE_ENABLE);
    }
    else
    {
        //receive file error
        BtResumeBGPlay(MMIBT_FTP_SUSPEND);
        MMIPUB_OpenAlertFailWin(TXT_COMMON_RECEIVE_FAIL);
    }
	return TRUE;
}

/*****************************************************************************/
//  Discription: open the window of new file win
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_OpenNewReceivedFileWin(
                                          BT_MSG_T *msg_body_ptr
                                          )
{
    uint32        time_period = 0;
    MMI_WIN_ID_T alert_id = MMI_BT_NEW_FILE_WIN_ID;
    MMI_WIN_ID_T alert_sync_id  = MMIBT_ALERT_VCF_WATING_WIN_ID;
    uint32       softkey_type = 0;
    //lulifei add for the vcard&vcalendar file
    uint16      file_len = 0;
    wchar       suffix[MMIFMM_FILE_FULL_PATH_MAX_LEN+1] = {0};
    const wchar suffix_vcf[] = {'.', 'v', 'c','f', 0};
    const wchar suffix_vcs[] = {'.', 'v', 'c', 's', 0};
    uint16      suffix_len = MMIFMM_FILE_FULL_PATH_MAX_LEN;
    uint16      vcf_len = 0;
    uint16      vcs_len = 0;
    MMI_STRING_T name_vcard = {0};
    uint32       file_type = MMIBT_VCARD_OTHERS;

    //turn on the back light
    MMIDEFAULT_SetBackLight(TRUE);
    MMK_SendMsg(MMIBT_RECEIVING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
    MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
//     MMIBT_RecoveryBtStatus(MMIBT_MODULE_BLUETOOTH);
    //if(!MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        if(MMIAPICOM_Wstrlen(s_transfer_vcf_file_info.transfer_file_name) > MMIFILE_FILE_NAME_MAX_LEN)
        {
            //file name too large can not open fmm
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_FILE_NAME_TOO_LARGE,TXT_NULL,IMAGE_PUBWIN_WARNING, &alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

        }
        else
        {
            //s_is_permit_open = Is_PermitOpenFile();
            s_is_permit_open = FALSE; //NOT ALLOW TO SEE THE FILE
            if(!s_is_permit_open)
            {
                softkey_type = MMIPUB_SOFTKEY_ONE;
                time_period = MMI_3SECONDS;
            }
            else
            {
                softkey_type = MMIPUB_SOFTKEY_OKEXIT;
            }
	   //lulifei modify for the vcard and vcalender
	   file_len  = (uint16) MMIAPICOM_Wstrlen (s_transfer_vcf_file_info.transfer_file_name);
	   //split file suffix
            MMIAPIFMM_SplitFullPathExt (s_transfer_vcf_file_info.transfer_file_name,
                                    file_len,
                                    PNULL, PNULL,
                                    PNULL, PNULL,
                                    suffix, &suffix_len);
	   //get vcf and vcs suffix len
	   SCI_TRACE_BUF("MMIBT_Func MMIBT_OpenNewReceivedFileWin suffix",&suffix,suffix_len);
            vcf_len  = (uint16) MMIAPICOM_Wstrlen (suffix_vcf);
            vcs_len = (uint16) MMIAPICOM_Wstrlen (suffix_vcs);
	   if(0 == MMIAPICOM_CompareTwoWstrExt (suffix, suffix_len, suffix_vcf, vcf_len, FALSE)){
	       MMI_GetLabelTextByLang(STR_QUERY_IMPORT_VCARD,&name_vcard);
	       file_type =MMIBT_VCARD_VCARD;
                MMIPUB_OpenConfirmationDialogWinByTextPtrEx(MMK_GetFirstAppletHandle(),
					PNULL,
					&name_vcard,
					PNULL,
					PNULL,
					&alert_sync_id,
					PNULL,
					MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
					HandleNewVcardWinMsg,
					file_type);

	   }else if(0 == MMIAPICOM_CompareTwoWstrExt (suffix, suffix_len, suffix_vcs, vcs_len, FALSE)){
	       MMI_GetLabelTextByLang(STR_QUERY_SAVE_VCALENDAR,&name_vcard);
	       file_type =MMIBT_VCARD_VCALENDAR;
                MMIPUB_OpenConfirmationDialogWinByTextPtrEx(MMK_GetFirstAppletHandle(),
					PNULL,
					&name_vcard,
					PNULL,
					PNULL,
					&alert_sync_id,
					PNULL,
					MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,
					HandleNewVcardWinMsg,
					file_type);
	   }else{
	            MMIPUB_OpenAlertWinByTextIdEx(
	                MMK_GetFirstAppletHandle(),
	                &time_period,
	                STR_BT_RECEIVE_SUCCESS,TXT_NULL,
	                IMAGE_PUBWIN_SUCCESS,
	                &alert_id,PNULL,
	                MMIPUB_SOFTKEY_CUSTOMER,
	                HandleNewFileWinMsg,
	                softkey_type);
	       }
        }
    }
}


/*****************************************************************************/
//  Discription: delete one or all pair device
//  Global resource dependence: none
//  Author:maryxiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBTAPI_DeletePairedDevice(BOOLEAN is_all)
{
}
/*****************************************************************************/
//  Discription: delete one or all pair device
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void DeletePairedDevice(BOOLEAN is_all,MMIBT_OPERTATION_TYPE_E  current_opt)
{
	uint16         select_index = 0;
	int            device_number = 0;
	int            i = 0;
    BOOLEAN        is_deleted = FALSE;
	BT_STATUS      delete_result = BT_ERROR;
	BT_DEVICE_INFO dev_info = {0};
	MMI_STRING_T   wait_text = {0};
	uint8          paired_device_index[MMIBT_MAX_DEV_ITEMS] = {0};
#ifdef MMI_BT_PBAP_SUPPORT
	BT_ADDRESS     dev_addr = {0};
#endif

	/*lulifei remove to change the ui
	get paired device index
	if (MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
	{
		select_index =	GUILIST_GetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID);
	}*/
	if (MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
	{
		select_index =	GUILIST_GetCurItemIndex(MMIBT_INQUIRED_DEV_LIST_CTRL_ID);
	}

	if(!is_all)
	{
		//delete one
		//first check this device permit to be delete
		MMIBT_StopOperationWin(current_opt);
        /*if(select_index == 0)
		{
			return;
		}*/
#ifdef MMI_PDA_SUPPORT
         if(select_index == 1)
		{
			return;
		}
        select_index = select_index-1;
#endif
        SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));
        //get the paired device and append to the list
        MMIBT_GetPairedDevList(MMIBT_GetCurrentSevice(), paired_device_index);
        BT_GetPairedDeviceInfo(MMIBT_GetCurrentSevice(), paired_device_index[select_index], &dev_info);
#ifdef MMI_BT_PBAP_SUPPORT
        /* Clean connected bt address data */
        SCI_MEMSET(&dev_addr,0,sizeof(BT_ADDRESS)); 
        MMINV_WRITE(MMINV_BT_ADDRESS_INFO,&dev_addr);
#endif
		if(MMIBT_DeviceIsConnectedHeadset(&dev_info.addr))
		{
			//can not  delete  connected headset
			MMIPUB_OpenAlertWarningWin(STR_BT_DELETE_CONNECTED);
			return;
		}
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
		else if(MMIBT_DeviceIsConnectedFTP(&dev_info.addr))
        {
            //can not delete connected ftp device
			MMIPUB_OpenAlertWarningWin(STR_BT_DELETE_TRANSFERRING);
			return;
        }
#endif
        else if(MMIBT_DeviceIsOnTransferStatus(&dev_info.addr,&s_transfer_file_info.device_info.addr))
        {
            //can not delete transfer file device
			MMIPUB_OpenAlertWarningWin(STR_BT_DELETE_TRANSFERRING);
			return;
        }
		else
		{
            //can be delete
            delete_result = BT_RemovePairedDevice( &dev_info.addr);
            is_deleted = TRUE;
		}

	}
	else
	{
        //delete all
        device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);
		for(i =device_number; i>= 1; i--)
		{
			 delete_result = BT_GetPairedDeviceInfo(BT_SERVICE_ALL, i, &dev_info);
			if(!MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr))
#ifdef BT_DIALER_SUPPORT
                &&(!BT_HfuGetConnStatus(&dev_info.addr))
#endif
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
				&&(!MMIBT_DeviceIsConnectedFTP(&(dev_info.addr)))
#endif
                &&(!MMIBT_DeviceIsOnTransferStatus(&(dev_info.addr),&s_transfer_file_info.device_info.addr))
               )
			{
				delete_result = BT_RemovePairedDevice(&(dev_info.addr));
				is_deleted = TRUE;
                break;
			}
		}
		if(!is_deleted)
		{
            //delete all fininshed
			//update device list
			if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
			{
			   MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_FINISHED, NULL);
			}
			if(device_number)
			{
				//exist not delete device
		        MMIPUB_OpenAlertSuccessWin(TXT_BT_DELETE_ALL_EXIST);
			}
			//SCI_TRACE_LOW:"[MMIBT] DeletePairedDevice, close MMIBT_DELETE_DEVICE_WAIT_WIN_ID"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4974_112_2_18_2_8_10_254,(uint8*)"");
            MMIBT_StopOperationWin(current_opt);
			return;
		}

	}

    //SCI_TRACE_LOW:"[MMIBT] DeletePairedDevice is_all %d delete_result %d is_deleted %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4981_112_2_18_2_8_10_255,(uint8*)"ddd", is_all, delete_result, is_deleted);
	if (BT_PENDING == delete_result && (!MMK_IsOpenWin(MMIBT_DELETE_DEVICE_WAIT_WIN_ID)))
	{
		MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
		MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_DELETE_DEVICE_WAIT_WIN_ID,IMAGE_NULL,
			ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
        MMIBT_StartOperationWin(MMIBT_DELETE_DEVICE_WAIT_WIN_ID,current_opt,CreatePubWinCallBack);
	}
	else if(BT_PENDING != delete_result)
	{
		MMIPUB_OpenAlertFailWin(TXT_ERROR);
        MMIBT_StopOperationWin(current_opt);
	}
}
/*****************************************************************************/
//  Discription: Handle blue tooth new file win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleNewFileWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
	wchar               *file_name_ptr = PNULL;
    wchar               *path_name_ptr = PNULL;
    MMIPUB_INFO_T       *pubwin_info_ptr = PNULL;
	//static uint16       transfer_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};

	uint16  path_len = 0;//MMIAPICOM_Wstrlen((uint16 *)s_transfer_file_info.transfer_path_name);
	uint16  file_len = 0;
    uint32  softkey_type = 0;
	//go to FMM dirctory
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if(PNULL != pubwin_info_ptr)
        {
            softkey_type = pubwin_info_ptr->user_data;
        }
        if(MMIPUB_SOFTKEY_OKEXIT == softkey_type)
        {

            MMIPUB_SetWinSoftkey(win_id, STXT_SOFTKEY_OPEN_MK, STXT_SOFTKEY_STOP_MK, FALSE);
        }
        else
        {
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_SOFTKEY_STOP_MK, FALSE);
        }
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    case MSG_CTL_CANCEL:
    //case MSG_APP_RED:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        //accept this operation
        if(s_is_permit_open)
        {	file_name_ptr = SCI_ALLOCA((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
            if(file_name_ptr != PNULL)
            {
                path_name_ptr = SCI_ALLOCA((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
                if(path_name_ptr != PNULL)
                {
                    SCI_MEMSET(file_name_ptr, 0x00, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));
                    SCI_MEMSET(path_name_ptr, 0x00, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));
                    MMIAPICOM_GetLongFileNameByFullPath(s_transfer_file_info.transfer_file_name, file_name_ptr);
                    MMIAPICOM_GetLongFilePathNameByFullPath(s_transfer_file_info.transfer_file_name, path_name_ptr);
                     path_len = MMIAPICOM_Wstrlen((uint16 *)path_name_ptr);
                     file_len = MMIAPICOM_Wstrlen((uint16 *)file_name_ptr);

                    if(MMIAPIFMM_IsFolderExist(path_name_ptr, path_len))
                    {
                        MMIAPIFMM_OpenExplorerExt((uint16 *)path_name_ptr, path_len,
                            file_name_ptr, file_len, TRUE,MMIFMM_FILE_ALL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_FOLDER_NOT_EXIST);
                    }
                    SCI_Free(path_name_ptr);
                }
                SCI_Free(file_name_ptr);
            }
        }
         MMK_CloseWin(win_id); //lulifei modify for bug 477637
        break;
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		s_is_permit_open = FALSE;
		break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
//  Discription: get FTP server request
//  Global resource dependence: none
//  Author: kelly.li
//  Note://对方要通过FTP取走文件的提示框
//
/*****************************************************************************/
PUBLIC void MMIBT_GetFtpReq(BT_MSG_T* msg_body_ptr)
{
    BT_FILE_INFO    file_info = {0};
    MMI_STRING_T    temp_str = {0}, query_text = {0};
    BOOLEAN       be_authorize = FALSE;

    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_GetFtpReq PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5405_112_2_18_2_8_11_257,(uint8*)"");
        return ;
    }

	file_info = *(( BT_FILE_INFO *) msg_body_ptr->body_ptr);
    if(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
    {
         s_transfer_file_info.server_type = MMIBT_FILE_SERVER_FTP_SEND;
         MMIBT_CancelFileTransfer(TRUE);
         //MMIBT_CancelTransfer(MMIBT_FILE_SERVER_FTP_SEND);
         return;
    }
	MMK_CloseWin(MMI_BT_ALERT_WIN_ID);
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)STR_BT_SENDING, &temp_str);
	query_text.wstr_ptr = file_info.name;
    query_text.wstr_len = MMIAPICOM_Wstrlen(file_info.name);
    //store ftp server req file info
    SCI_MEMSET(&s_transfer_file_info, 0x00, sizeof(s_transfer_file_info));
    s_transfer_file_info.server_type = MMIBT_FILE_SERVER_FTP_SEND;
    SCI_MEMCPY(&s_transfer_file_info.device_info,&file_info.remote_dev,sizeof(BT_DEVICE_INFO));
    MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name, file_info.name, MIN(MMIAPICOM_Wstrlen(file_info.name), MMIBT_FULL_PATH_LEN));/*lint !e666*/
    s_transfer_file_info.total_file_size = file_info.size;
    //turn on the back light
	CloseSendWin();
    MMIDEFAULT_SetBackLight(TRUE);
    BT_GetDeviceAuthorisation(&(file_info.remote_dev.addr),&be_authorize);
    if(be_authorize)
    {
        DoOperation(MMIBT_OPC_FTP_SEND, TRUE);
    }
    else
    {
        MMIBT_OpenQueryWin(MMI_BT_FTS_QUERY_WIN_ID,&temp_str, &query_text, MMIBT_OPC_FTP_SEND);
    }
}

/*****************************************************************************/
//  Discription: Open the blue tooth incoming file win
//  Global resource dependence: none
//  Author:kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_OpenIncomingFileWin(
                                       BT_MSG_T                     *msg_body_ptr,
                                       MMIBT_FILE_SERVER_TYPE_E     service_type
                                       )

{
    BT_FILE_INFO         file_info  = {0};
    uint16               name_len = 0;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    MMIFILE_DEVICE_E fmm_dev =  0;
    const wchar* dev_ptr = PNULL;
    uint16       dev_len = 0;
    uint16 full_path_max_len = MMIFILE_FULL_PATH_MAX_LEN;

    SCI_TRACE_LOW("MMIBT_OpenIncomingFileWin");
    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_OpenIncomingFileWin PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5565_112_2_18_2_8_11_262,(uint8*)"");
        return;
    }
    //接收文件有可能是两三个文件连续接收的，需要把之前一个新接收文件的提示窗口关掉

    file_info  = *((BT_FILE_INFO*) msg_body_ptr->body_ptr);
    name_len = MMIAPICOM_Wstrlen((wchar*)file_info.name);
    //Set receive file server type
    if(name_len > MMIBT_FULL_PATH_LEN)
    {
        //file name too long
        //SCI_TRACE_LOW:"MMIBT_OpenIncomingFileWin:name_len is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5576_112_2_18_2_8_11_263,(uint8*)"d",name_len);
        MMIBT_CancelFileTransfer(TRUE);
        return;
    }
    MMK_CloseWin(MMI_BT_NEW_FILE_WIN_ID);
    MMK_CloseWin(MMI_BT_ALERT_WIN_ID);
    //if(MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT != MMIBT_GetOpertor().current_opertion)
    {
        if(MMK_IsOpenWin(MMI_BT_QUERY_WIN_ID))
        {
            MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_APP_CANCEL, PNULL);
            MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
        }
    }
    CloseSendWin();
    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_RECIEVE);
    s_transfer_file_info.is_temp_file = FALSE;
    SCI_MEMSET(&s_transfer_file_info, 0x00, sizeof(s_transfer_file_info));
    s_transfer_file_info.server_type = service_type;
    SCI_MEMSET(s_transfer_file_info.transfer_file_name,0x00, sizeof(s_transfer_file_info.transfer_file_name));

    //get file name and suffix info
    SCI_MEMCPY(&s_transfer_file_info.device_info, &file_info.remote_dev, sizeof(BT_DEVICE_INFO));
    SCI_MEMSET(s_transfer_file_info.device_info.name,0x00, sizeof(s_transfer_file_info.device_info.name));
    BT_GetPairedDeviceName(&(file_info.remote_dev.addr) ,s_transfer_file_info.device_info.name);
    s_transfer_file_info.total_file_size = file_info.size;
    if(MMIBT_FILE_SERVER_OPP_RECIEVE == s_transfer_file_info.server_type)
    {
        //opp receive file
        MMIBT_GetFileUserSetting(MMINV_BT_FILE_LOCATION, &setting);
	/*lulifei modify the receive file path
        MMIAPICOM_CombineDefaultFullPathByFilename(setting.filedev,
            file_info.name,
            MMIAPICOM_Wstrlen(file_info.name),
            s_transfer_file_info.transfer_file_name,
            MMIFILE_FULL_PATH_MAX_LEN
            );
	MMIAPICOM_CombinePath(MMIMULTIM_DIR_RECEIVED,
		MMIAPICOM_Wstrlen(MMIMULTIM_DIR_RECEIVED),
		file_info.name,
		MMIAPICOM_Wstrlen(file_info.name),
		FALSE,
		s_transfer_file_info.transfer_file_name,
                  MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)
		);*/
         fmm_dev = MMIAPIFMM_GetDefaultDisk();

        dev_ptr = MMIAPIFMM_GetDevicePath(fmm_dev);
        dev_len = MMIAPIFMM_GetDevicePathLen(fmm_dev);

        MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                                MMIMULTIM_DIR_RECEIVED, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_RECEIVED),
                                file_info.name, MMIAPICOM_Wstrlen(file_info.name),
                                s_transfer_file_info.transfer_file_name,
                                &full_path_max_len);

	SCI_TRACE_BUF("[MMIBT] [MMIBT_OpenIncomingFileWin] transfer_file_name = ", s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)*sizeof(wchar));
    }
    else
    {
        //ftp receive file
        MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name, file_info.name, name_len);
        setting.filedev = MMIAPIFMM_GetDeviceTypeByPath(
                            s_transfer_file_info.transfer_file_name,
                            MMIFILE_DEVICE_LEN
                            );
    }

    //all disk not exist
    if(MMIAPIUDISK_UdiskIsRun())
    {
        //usb runing
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        MMIBT_CancelFileTransfer(TRUE);

    }
    else if(!MMIAPIUDISK_IsDeviceReady())
    {
        //all disk not exist
        MMIBT_CancelFileTransfer(TRUE);
    }
    else if(MMIBT_IsEnoughSpaceToRecieve(setting.filedev))
    {
        //memory enough
        //overwrite file, space enough
        s_transfer_file_info.filedev = setting.filedev;
        SCI_MEMSET(&g_current_bt,0x00, sizeof(BT_DEVICE_INFO));
        SCI_MEMCPY(&g_current_bt, &file_info.remote_dev, sizeof(BT_DEVICE_INFO));
        if(!s_is_vcf_file)
        {
            CheckAuthorizeAndOpenIncomingWin();
        }
    }
    else
    {
        MMI_TEXT_ID_T text_id = TXT_CMSBRW_UDISK_FULL_CHANGE_STORE;
        MMI_WIN_ID_T  alert_id = MMI_BT_ALERT_WIN_ID;

        if(MMI_DEVICE_UDISK != setting.filedev)
        {
            text_id = TXT_CMSBRW_SD_FULL_CHANGE_STORE;
        }
        if(MMIBT_FILE_SERVER_OPP_RECIEVE == s_transfer_file_info.server_type)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleNoSpaceForFileAlertMsg);
        }
        else
        {
            //ftp need not auto change device
            DisplayMemoryInformation(setting.filedev);
            MMIBT_CancelFileTransfer(TRUE);
        }
    }
}

/**--------------------------------------------------------------------------*
**                          BLUETOOTH FUCNTIONS NOT IN WIN TAB
**                          BlueTooth Interface Functions implement
**                          *I**N**T**E**R**F**A**C**E*
**--------------------------------------------------------------------------**/
/*****************************************************************************/
//  Discription: receive bt on cnf
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetBtOnOffCnf( BT_MSG_T     *msg_body_ptr, BOOLEAN is_on)
{
    MMIBT_OPERATION_INFO_T  bt_opc = MMIBT_GetOpertor();
    uint32          node_id = 0;       //node id,from 1
    MMI_STRING_T    node_text = {0};
    MMI_TEXT_ID_T   text_id = TXT_NULL;
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_IMAGE_ID_T              imgae_id = IMAGE_NULL;
    BOOLEAN                     isgray = FALSE;
#endif
    MMI_TEXT_ID_T          left_text_id = TXT_NULL;

    //SCI_TRACE_LOW:"MMIBT_GetBtOnOffCnf status %d current_opertion %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5692_112_2_18_2_8_11_264,(uint8*)"dd",msg_body_ptr->status, bt_opc.current_opertion);
    if(MMIbt_resetflag == TRUE)
    {
    	MMIBT_ClearAllDevice(TRUE);  
    }

	
#ifdef ENG_SUPPORT
	if(MMIAPISET_GetFlyMode() && is_on && !MMIENG_GetMode_FLY_Enter_BT_FM() )// fly mode on
#else
 	if(MMIAPISET_GetFlyMode() && is_on)// fly mode on
#endif
	{
		//in fly mode need close bt
		MMIBT_OpenBluetooth(FALSE, FALSE);
	}
	else
	{
		//update blue tooth idle icon
		MMIBT_UpdateBtIdleIcon(BT_GetState(),MMIBT_GetIsDeviceVisible());
	}
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        BT_UpdateNV();
        if(is_on)
        {
            //start cnf
            text_id = TXT_SET_BT_OFF;
            left_text_id = TXT_COMMON_CLOSE;
            node_id = ID_SET_BT_OFF;
#ifndef MMI_GUI_STYLE_TYPICAL
            imgae_id = IMAGE_CHECK_SELECTED_ICON;
            isgray   =FALSE;
#endif
            //set bt on confirm
            SetFtpRootFolderAfterPowerOn();
            #ifdef BT_BQB_SUPPORT
            //+++ mdy by qing.yu@for cr238585
            MMIAPIBT_SetNetWorkRoamStatus(MMIAPIPHONE_GetIsNetworkRoaming());
            MMIAPIBT_SetOperatorName();
            MMIAPIBT_SetMyPhoneNumber();
			//--- mdy by qing.yu@for cr238585
			#endif //BT_BQB_SUPPORT
            if(MMIBT_OPC_AUTO_TEST == bt_opc.root_opertion)
            {
                //auto test
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SEARCH);
                MMIBT_OpenSearchDeviceWaitWin(BT_SERVICE_ALL);
            }
            else if(MMIBT_OPC_OPP_SEND == bt_opc.root_opertion)
            {
                DoOperation(MMIBT_OPC_LIST_PAIRED,FALSE);
            }
#ifdef MMI_BT_PBAP_SUPPORT
            else if(MMIBT_OPC_CONNECT_PBAP == bt_opc.root_opertion)
            {
                SCI_TRACE_LOW("[MMIBT] MMIBT_GetBtOnOffCnf do MMIBT_OPC_LIST_PAIRED_PBAP");
                DoOperation(MMIBT_OPC_LIST_PAIRED_PBAP,FALSE);
            }
#endif
            else if(MMIBT_OPC_POWER_ONOFF != bt_opc.current_opertion)
            {
                //not power on from setting, so do current option directly
                DoOperation(bt_opc.current_opertion, FALSE);
            }
            else
            {
                if(MMK_IsOpenWin(MMIBT_BTSTART_WAIT_WIN_ID))
                {
                    //finished opertion
                    MMIDEFAULT_SetBackLight(TRUE);
                }
            }
        }
        else
        {
            text_id = TXT_BT_SET_ON;
            left_text_id = STXT_SOFTKEY_OPEN_MK;
            node_id = ID_SET_BT_ON;
#ifndef MMI_GUI_STYLE_TYPICAL
            imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
            isgray   = TRUE;
#endif
            //close bt cnf
            if(MMK_IsOpenWin(MMIBT_BTSTART_WAIT_WIN_ID))
            {
                MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
                if(MMIBT_OPC_AUTO_TEST != bt_opc.root_opertion)
                {
                    //close BT confirm
                    MMIDEFAULT_SetBackLight(TRUE);
                }
				 MMK_CloseWin( MMIBT_BTSTART_WAIT_WIN_ID);
            }
        }

        if(MMK_IsOpenWin(MMIBT_MAIN_MENU_WIN_ID))
        {
#ifndef MMI_GUI_STYLE_TYPICAL
            //lulifei modify for UI spec
            //MMIBT_AppendOneLineTextIconListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BLUETOOTH,imgae_id,BT_INDEX_OPEN,left_text_id,TRUE);
            AppendPdaBtItem1ToggleLine(MMIBT_MAIN_LIST_CTRL_ID,STR_BT_LIST_BLUETOOTH_EXT01,BT_INDEX_OPEN,left_text_id,TRUE);
            MMIBT_UpdateMainMenu(TRUE);
#else
            //update main menu
            MMI_GetLabelTextByLang(text_id, &node_text);
            GUIMENU_ReplaceNode(0, node_id, 0, &node_text, MMIBT_MAIN_MENU_CTRL_ID);
#endif
        }
        //close waiting win
        MMK_CloseWin(MMIBT_BTSTART_WAIT_WIN_ID);

    }
    else
    {
        //open and close fail
        if(MMIBT_OPC_AUTO_TEST == bt_opc.root_opertion)
        {
            MMIDEFAULT_SetBackLight(TRUE);
            //auto test
            if(is_on)
            {
                MMIBT_CreateUitestWin();
            }
            else
            {
                //MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
            }
        }
        else if(MMIBT_OPC_INIT != bt_opc.root_opertion)
        {
            MMIDEFAULT_SetBackLight(TRUE);
#ifndef MMI_PDA_SUPPORT
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
#endif
            //MMK_CloseWin(MMIBT_BTSTART_WAIT_WIN_ID);
        }
		MMK_CloseWin( MMIBT_BTSTART_WAIT_WIN_ID);
    }

#if 0
    {
        uint8         bt_profile_list = 0;
        
        EFS_NvitemRead(APP_NV_BT_PROFILE_LIST, sizeof(uint8), &bt_profile_list);
        if(1 == bt_profile_list)
        {
#ifndef WIN32
            BT_CloseRunningService(BT_SERVICE_OPP);
            BT_CloseRunningService(BT_SERVICE_FTP);
#endif
        }
    }
#endif
    //Invoke Event
    
    MMIAPISET_FuncFinishedInd();
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetBtOnOffCnf Exit");

}


/*****************************************************************************/
//  Discription: receive bt visible cnf
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetVisibleCnf(BT_MSG_T *msg_body_ptr, BOOLEAN is_visible)
{
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_IMAGE_ID_T              imgae_id = IMAGE_NULL;
#endif
    MMI_TEXT_ID_T          left_text_id = TXT_NULL;
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_GetVisibleCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5854_112_2_18_2_8_11_265,(uint8*)"");
        return;
    }
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        BT_UpdateNV();
        MMIBT_UpdateBtIdleIcon( BT_GetState(), is_visible);

        if (MMK_IsOpenWin( MMIBT_VISIBLE_WAIT_WIN_ID ))
        {
            MMIDEFAULT_SetBackLight(TRUE);
            MMK_CloseWin(MMIBT_VISIBLE_WAIT_WIN_ID);
        }
    }
    else
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        if(MMK_IsOpenWin( MMIBT_SET_VISIBILITY_WIN_ID ))

        {
            if(MMIBT_GetIsDeviceVisible())
            {
                GUILIST_SetCurItemIndex(MMIBT_SET_VISIBILITY_CTRL_ID,0);
                GUILIST_SetSelectedItem(MMIBT_SET_VISIBILITY_CTRL_ID, 0, TRUE);
            }
            else
            {
                GUILIST_SetCurItemIndex(MMIBT_SET_VISIBILITY_CTRL_ID,1);
                GUILIST_SetSelectedItem(MMIBT_SET_VISIBILITY_CTRL_ID, 1, TRUE);
            }
        }
#endif
        if (MMK_IsOpenWin( MMIBT_VISIBLE_WAIT_WIN_ID ))
        {
            MMIDEFAULT_SetBackLight(TRUE);
            ClosePrmAndOpenDoneWin(
                                MMIBT_VISIBLE_WAIT_WIN_ID,
                                TXT_ERROR,
                                IMAGE_PUBWIN_FAIL,
                                MMI_2SECONDS
                                );
        }
    }
#ifndef MMI_GUI_STYLE_TYPICAL
    if(MMIBT_GetIsDeviceVisible())
    {
        imgae_id = IMAGE_CHECK_SELECTED_ICON;
        left_text_id = TXT_COMMON_CLOSE;
    }
    else
    {
        imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
         left_text_id = TXT_OPEN;
    }
    //MMIBT_AppendOneLineTextIconListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_MY_BT_VISIBILITY,imgae_id,BT_INDEX_VISIBLE,left_text_id,TRUE);
    MMK_SendMsg(MMIBT_MAIN_MENU_WIN_ID, MSG_FULL_PAINT, PNULL);
#endif
}
/*****************************************************************************/
//  Discription: get inquired device complete confirm
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void  MMIBT_GetInqCompCnf(void)
{
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
    AnimatePlayCtrlByState(MMIBT_ANIM_STOP);
    MMIDEFAULT_AllowOpenKeylocWin(TRUE);
    if (MMK_IsOpenWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID))
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_GetInqCompCnf no device found");
        //no device fond
        //turn on the back light
        MMIDEFAULT_SetBackLight(TRUE);
        
        //MMIPUB_OpenAlertWarningWin(TXT_BT_NO_DEVICE);
        MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_BT_URN_NO_DEV_FOUND,PNULL,
            IMAGE_NULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleBTNoDeviceWarningWinMsg);
        MMK_SendMsg(MMIBT_SEARCH_DEVICE_PUBWIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
        MMK_CloseWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID);
        if(MMIBT_GetOpertor().current_opertion == MMIBT_OPC_SEARCH)
        {
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_INIT);
        }
    }
    else if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
    {
        //inform search list
        MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_FINISHED, PNULL);
        MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
    }
    //进行蓝牙UI TesT测试，搜索结束消息
    if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
    {
        MMIBT_CreateUitestWin();
        MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
    }
}

/*****************************************************************************/
//  Discription: send file to other device failed ind
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SendFileFailedInd(void)
{
    //send file fail
    //SCI_MEMSET(&g_current_bt, 0x00, sizeof(g_current_bt));
	MMK_CloseWin(MMI_BT_ALERT_WIN_ID);
    BtResumeBGPlay(MMIBT_FTP_SUSPEND);
    
    //if(MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID) || MMK_IsOpenWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID))
    {
        MMIDEFAULT_SetBackLight(TRUE);
        //if(!MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            //SCI_TRACE_LOW:"[&&&]MMIBT_SendFileFailedInd"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6948_112_2_18_2_8_14_284,(uint8*)"");
            MMIPUB_OpenAlertFailWin(STR_BT_SEND_FAIL);
        }
        MMK_CloseWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID);
        MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
    //     MMIBT_RecoveryBtStatus(MMIBT_MODULE_BLUETOOTH);
    MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
    MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
}

/*****************************************************************************/
//  Discription: send file to other device failed ind
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_TransferCancelled(BOOLEAN is_send)
{
    MMIDEFAULT_SetBackLight(TRUE);
    BtResumeBGPlay(MMIBT_FTP_SUSPEND);
    MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);
    if(is_send)
    {
        //send file canceled
        MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
    else
    {
        //recevied file canceled
        MMIBT_UpdateCurrentFileList();
        MMK_SendMsg(MMIBT_INCOMING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
        MMK_SendMsg(MMIBT_RECEIVING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
    }
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
    MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
    MMK_CloseWin(MMI_BT_QUERY_WIN_ID);

}

/*****************************************************************************/
//  Discription: get receive file fail indication
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_ReceiveFileFail(void)
{
    //reset blue tooth status
    BtResumeBGPlay(MMIBT_FTP_SUSPEND);

//when if 0,fix bug721845  when receive fail, no need to update pic lis
#if 1
    MMIBT_UpdateCurrentFileList();
#else
    MMIAPIPICVIEW_ReloadFile(); 
#endif

    UpdateCurrentPictureViewList();
    MMK_CloseWin(MMI_BT_ALERT_WIN_ID);
    //SCI_TRACE_LOW:"MMIBT_ReceiveFileFail s_transfer_file_info.server_type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7067_112_2_18_2_8_14_286,(uint8*)"d",s_transfer_file_info.server_type);

    MMIDEFAULT_SetBackLight(TRUE);
    MMIPUB_OpenAlertSuccessWin(STXT_SOFTKEY_CANCEL_MK);
    if(MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID) || MMK_IsOpenWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID);
        MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
    //receive file error
    else if ( MMK_IsOpenWin(MMIBT_RECEIVING_FILE_WIN_ID))
    {
        MMIBT_UpdateCurrentFileList();
        MMK_SendMsg(MMIBT_RECEIVING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
    }
    if (MMK_IsOpenWin(MMIBT_INCOMING_FILE_WIN_ID))
    {
        MMIBT_UpdateCurrentFileList();
        MMK_SendMsg(MMIBT_INCOMING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
    }
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
    MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
    MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
    MMK_CloseWin(MMI_BT_FTS_QUERY_WIN_ID);
}

/*****************************************************************************/
//  Discription: cancel blue tooth ftp function when is in call(MT & MO)
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_CancelFileTransfer(BOOLEAN is_close_query)
{
    BT_STATUS bt_status = 0;
    
    if (MMIBT_FILE_SERVER_OPP_SEND == s_transfer_file_info.server_type)
    {
        bt_status = BT_CancelOppSendFile();
    }
    else if (MMIBT_FILE_SERVER_OPP_RECIEVE == s_transfer_file_info.server_type)
    {
        bt_status = BT_CancelOppReceiveFile();
    }
    else if (MMIBT_FILE_SERVER_FTP_RECIEVE== s_transfer_file_info.server_type)
    {
        bt_status = BT_CancelFtpTransfer();
    }
    else if (MMIBT_FILE_SERVER_FTP_SEND == s_transfer_file_info.server_type)
    {
        bt_status = BT_CancelFtpTransfer();
    }
    if(BT_PENDING != bt_status)
    {
        s_transfer_file_info.server_type = MMIBT_FILE_SERVER_MAX;
        if(BT_SUCCESS == bt_status)
        {
            //cancel success
            MMIBT_UpdateCurrentFileList();
            //MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);
        }
        else
        {
            //cancel error
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7151_112_2_18_2_8_14_287,(uint8*)"");
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
        MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
        MMK_SendMsg(MMIBT_RECEIVING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
        MMK_SendMsg(MMIBT_INCOMING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
        MMK_CloseWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID);
    }
    if(is_close_query)
    {
        MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
        MMK_CloseWin(MMI_BT_FTS_QUERY_WIN_ID);
    }
    
    return bt_status;
}

/*****************************************************************************/
//  Discription: receive bt hidden cnf
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void  MMIBT_CancelSearchCnf(BT_MSG_T *msg_body_ptr)
{
    BT_STATUS return_value= BT_SUCCESS;
    MMI_TEXT_ID_T ui_text = TXT_NULL;
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();

    MMIDEFAULT_SetBackLight(TRUE);
    if(PNULL != msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_CancelSearchCnf  status is = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7185_112_2_18_2_8_14_288,(uint8*)"d",msg_body_ptr->status);

        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            //inform search list
            MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_CANCELED, PNULL);
        }
        else if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
        {
            //finished error
            if(!MMK_IsOpenWin(MMIBT_UITEST_RESULT_WIN_ID)
                && (MMK_IsOpenWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID)
                ||MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID)
                ||MMK_IsOpenWin(MMIENG_BT_CANCEL_WAIT_WIN_ID)
                )
                )
            {
                if(BT_SUCCESS == msg_body_ptr->status)
                {
                    ui_text = TXT_UITEST_SUCCESS_BT;
                }
                else
                {
                    ui_text = TXT_UITEST_FAIL_BT;
                }
                MMIBT_CreateUitestWin();
            }
        }
        if(BT_SUCCESS == msg_body_ptr->status)
        {
            if (MMK_IsOpenWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID))
            {
                //start seaching device
                return_value = BT_SearchDevice(MMIBT_GetCurrentSevice());
            }
            else
            {
                //real cancel
                MMIDEFAULT_AllowOpenKeylocWin(TRUE);
                BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
            }
        }
        else
        {
            //cancel search error !
            if(MMIBT_OPC_AUTO_TEST != operation_info.root_opertion)
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_SendMsg(MMIBT_SEARCH_DEVICE_PUBWIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
            MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
            MMK_CloseWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID);
            BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
        }
    }
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
}
/*****************************************************************************/
//  Discription: phone is ready or not
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsPhoneReady(void)
{
    BOOLEAN               is_ready = FALSE;
    if (!MMK_IsOpenWin(PHONE_POWER_OFF_WIN_ID)
        && (!MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID))
        )
    {
        is_ready = TRUE;
    }
    //SCI_TRACE_LOW:"MMIBT_IsPhoneReady is_ready %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7816_112_2_18_2_8_16_292,(uint8*)"d",is_ready);
    return is_ready;
}

/*****************************************************************************/
//  Discription: open or close BT
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenBluetooth(BOOLEAN set_on, BOOLEAN is_need_cnf)
{
    BOOLEAN    result = FALSE;
    BT_STATUS  ret_val = BT_SUCCESS;
    BT_MSG_T   msg_body= {0};
    MMI_STRING_T wait_text = {0};
    MMIBT_OPERATION_INFO_T opt = MMIBT_GetOpertor();

    if(set_on)
    {
        //启动蓝牙
#ifdef ENG_SUPPORT
	if(MMIAPISET_GetFlyMode() && !MMIENG_GetMode_FLY_Enter_BT_FM() )// fly mode on
#else
 	if(MMIAPISET_GetFlyMode())// fly mode on
#endif
        {
            //not permit to open BT
            if(MMIBT_OPC_AUTO_TEST == opt.root_opertion)
            {
                //auto test finished bt test and run next step
                MMIBT_CreateUitestWin();
                //GUILABEL_SetTextById(MMIBT_UI_RESULT_LABEL_CTRL_ID,TXT_CLOSE_FLY_MODE_FIRST,FALSE);
            }
            else
            {
                //MMIPUB_OpenAlertWarningWin(STR_BT_FLIGHT_MODE);
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CLOSE_FLY_MODE_FIRST,PNULL,PNULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleBTNoDeviceWarningWinMsg);
                //lulifei add to update the ui.
                MMIBT_UpdateMainMenu(TRUE);
            }
            return result;
        }
        else if(!BT_GetState())
        {
            ret_val = BT_Start();
        }
    }
    else
    {
        //首先检测有无文件在传输(OPP/FTP)，如果正在传输文件，则提示用户先自行取消
        if(MMIAPIBT_IsOpenFileWin())
        {
            MMIPUB_OpenAlertWarningWin(TXT_BT_CLOSE_WARNING);
            return result;
        }
        else
        {
            if(BT_GetState())
            {
                ret_val = BT_Stop();
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIBT]MMIBT_OpenBluetooth set_on %d ret_val %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7999_112_2_18_2_8_16_295,(uint8*)"dd",set_on, ret_val);
    //打开等待窗口;
    if(BT_PENDING == ret_val && is_need_cnf)
    {
        
        MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_BTSTART_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
    }
    else
    {
        //need not wait callback
        if(is_need_cnf)
        {
            msg_body.status = ret_val;
            MMIBT_GetBtOnOffCnf(&msg_body, set_on);
        }
    }

    if(BT_PENDING == ret_val)
    {
        result = TRUE;
    }
    return result;
}

PUBLIC BOOLEAN MMIBT_OpenBluetooth_Ext(BOOLEAN set_on, BOOLEAN is_need_cnf)
{
    BOOLEAN    result = FALSE;
    BT_STATUS  ret_val = BT_SUCCESS;
    BT_MSG_T   msg_body= {0};
    MMI_STRING_T wait_text = {0};
    MMIBT_OPERATION_INFO_T opt = MMIBT_GetOpertor();

    if(set_on)
    {
        //启动蓝牙
#ifdef ENG_SUPPORT
	if(MMIAPISET_GetFlyMode() && !MMIENG_GetMode_FLY_Enter_BT_FM() )// fly mode on
#else
 	if(MMIAPISET_GetFlyMode())// fly mode on
#endif
        {
            //not permit to open BT
            if(MMIBT_OPC_AUTO_TEST == opt.root_opertion)
            {
                //auto test finished bt test and run next step
                MMIBT_CreateUitestWin();
                //GUILABEL_SetTextById(MMIBT_UI_RESULT_LABEL_CTRL_ID,TXT_CLOSE_FLY_MODE_FIRST,FALSE);
            }
            else
            {
                //MMIPUB_OpenAlertWarningWin(STR_BT_FLIGHT_MODE);
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CLOSE_FLY_MODE_FIRST,PNULL,PNULL,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleBTNoDeviceWarningWinMsg);
                //lulifei add to update the ui.
                MMIBT_UpdateMainMenu(TRUE);
            }
            return result;
        }
        else//if(!BT_GetState())
        {
            ret_val = BT_Start();
        }
    }
    else
    {
#if 0
        //首先检测有无文件在传输(OPP/FTP)，如果正在传输文件，则提示用户先自行取消
        if(MMIAPIBT_IsOpenFileWin())
        {
            MMIPUB_OpenAlertWarningWin(TXT_BT_CLOSE_WARNING);
            return result;
        }
        else
#endif
        {
            if(BT_GetState())
            {
                ret_val = BT_Stop_Ex();
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIBT]MMIBT_OpenBluetooth set_on %d ret_val %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7999_112_2_18_2_8_16_295,(uint8*)"dd",set_on, ret_val);
    //打开等待窗口;
    if(BT_PENDING == ret_val && is_need_cnf)
    {
/*
        MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_BTSTART_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
*/
    }
    else
    {
        //need not wait callback
        if(is_need_cnf)
        {
            msg_body.status = ret_val;
            MMIBT_GetBtOnOffCnf(&msg_body, set_on);
        }
    }

    if(BT_PENDING == ret_val)
    {
        result = TRUE;
    }
    return result;
}


/*****************************************************************************/
//  Discription: Handle query win msg
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIBT_HandleQueryWinMsg(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static BOOLEAN  s_is_cancelled = FALSE;
    static MMIBT_OPERTATION_TYPE_E s_quer_opc_arr[MMIBT_MAX_QUERY_NUM] = {0};
    static uint8  s_current_query_num = 0;
    MMIBT_OPERTATION_TYPE_E  current_opc = MMIBT_OPC_INIT;
    wchar               file_full_path_ptr[MMIBT_FULL_PATH_LEN +1] = {0};
    wchar               vp_file_full_path_ptr[MMIBT_FULL_PATH_LEN +1] = {0};//add by CR233033
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    //SCI_TRACE_LOW:"[MMIBT] HandleQueryWinMsg win_id 0x%x, msg_id 0x%x s_current_query_num %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8123_112_2_18_2_8_16_299,(uint8*)"ddd", win_id, msg_id, s_current_query_num);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_is_cancelled = FALSE;
        if(s_current_query_num < MMIBT_MAX_QUERY_NUM)
        {
            s_quer_opc_arr[s_current_query_num]= MMIBT_GetInterruptOper();
            s_current_query_num ++;
        }
        //StartVibrator(win_id);
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //accept this operation
        s_is_cancelled = TRUE;
        if(s_current_query_num > 0 && (s_current_query_num <= MMIBT_MAX_QUERY_NUM))
        {
            current_opc = s_quer_opc_arr[s_current_query_num -1];
        }
        else
        {
            current_opc = MMIBT_GetInterruptOper();
        }
        //SCI_TRACE_LOW:"OpenQueryWin MSG_APP_OK current_opc %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8162_112_2_18_2_8_16_300,(uint8*)"d",current_opc);
        if(MMIBT_OPC_RECIEVE == current_opc)
        {
            
#ifdef VIDEO_PLAYER_SUPPORT
            
            MMIAPIVP_GetPlayingFilename(vp_file_full_path_ptr);//add by CR233033
            
#endif
            
            //SCI_TRACE_LOW:"MMIBT file name %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8175_112_2_18_2_8_16_301,(uint8*)"s", file_full_path_ptr);
            //if(SFS_ERROR_NONE != MMIAPIFMM_DeleteFile(file_full_path_ptr, PNULL))//MARYXIAO CR230219
            if(IsFileUsed(s_transfer_file_info.transfer_file_name)||0 == MMIAPICOM_Wstrcmp(s_transfer_file_info.transfer_file_name,vp_file_full_path_ptr))//modified by CR233033
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_USING);
                CancelOperation(current_opc);
                result = MMI_RESULT_FALSE;
            }
            else if(AssignReceiveFileCfg(s_transfer_file_info.server_type, TRUE))
            {
                //enter receiving file win
                MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                    s_transfer_file_info.server_type,
                    s_transfer_file_info.transfered_size,
                    s_transfer_file_info.total_file_size);
            }
        }
        else if(MMIBT_OPC_CLOSE_HEADSET == current_opc)
        {
            BT_DEVICE_INFO dev_info = {0};
            BOOLEAN is_exist_connect_headset = MMIBT_OpenedHeadsetIsExist(&dev_info);
            SCI_TRACE_LOW("[MMIBT] DoOperation MMIBT_OPC_CLOSE_HEADSET: is_exist_connect_headset=%d", is_exist_connect_headset);
            if(is_exist_connect_headset){
                MMIBT_SetCurBtInfo(&dev_info);
            }
            DoOperation(current_opc, IsNeedBTOn(current_opc));
        }
        else
        {
            if(MMIBT_OPC_FTS_DLE == current_opc)
            {
                if(IsFileUsed(s_transfer_file_info.transfer_file_name))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_USING);
                    CancelOperation(current_opc);
                    result = MMI_RESULT_FALSE;
                }
            }
            if(MMI_RESULT_TRUE == result)
            {
                DoOperation(current_opc, IsNeedBTOn(current_opc));
            }
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        s_is_cancelled = TRUE;
        //cancel this operation
        if(s_current_query_num > 0 && (s_current_query_num <= MMIBT_MAX_QUERY_NUM))
        {
            current_opc = s_quer_opc_arr[s_current_query_num -1];
        }
        else
        {
            current_opc = MMIBT_GetInterruptOper();
        }
        if(MMIBT_OPC_OPP_SEND == operation_info.root_opertion)
        {
            if(s_transfer_file_info.is_temp_file)
            {
                MMIAPIFMM_DeleteFile(s_transfer_file_info.transfer_file_name, PNULL);
            }
        }
        if(MMIBT_OPC_PAIR == MMIBT_GetInterruptOper())
        {
            //luliei add for UI spec
            MMIPUB_OpenConfirmationDialogByTextId(
                PNULL,
                STR_BT_CONNECTION_CANCELLED_EXT01,
                PNULL,
                PNULL,
                PNULL,
                PNULL,
                MMIPUB_SOFTKEY_DIALOG_OK,
                HandleBTNoDeviceWarningWinMsg
                );
        }
        CancelOperation(current_opc);
        result = MMI_RESULT_FALSE;
        MMK_CloseWin(win_id);
        break;
    case MSG_BT_AUTO_CLOSE_WIN:
        s_is_cancelled = TRUE;
        break;
    case MSG_CLOSE_WINDOW:
        if(!s_is_cancelled)
        {
            if(s_current_query_num > 0 && (s_current_query_num <= MMIBT_MAX_QUERY_NUM))
            {
                current_opc = s_quer_opc_arr[s_current_query_num -1];
            }
            else
            {
                current_opc = MMIBT_GetInterruptOper();
            }
            CancelOperation(current_opc);
            if(MMIBT_OPC_OPP_SEND == operation_info.root_opertion)
            {
                if(s_transfer_file_info.is_temp_file)
                {
                    MMIAPIFMM_DeleteFile(s_transfer_file_info.transfer_file_name, PNULL);
                }
            }
        }
        if(s_current_query_num > 0)
        {
            s_current_query_num --;
        }
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}


LOCAL MMI_RESULT_E HandleConfirmationDialogMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T       *pubwin_info_ptr = PNULL;
    static BOOLEAN  is_recieve = FALSE;
    SCI_TRACE_LOW("[MMIBT] HandleConfirmationDialogMsg: msg_id=0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        SCI_TRACE_LOW("[MMIBT] HandleConfirmationDialogMsg: pubwin_info_ptr=0x%x", pubwin_info_ptr);
        if(PNULL != pubwin_info_ptr)
        {
            is_recieve = pubwin_info_ptr->user_data;
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        //关闭FM
        //MMIAPIFM_Exit();
        //关闭MP3
        MMIAPMAINWIN_Destory();
        MMK_CloseWin(win_id);
        SCI_TRACE_LOW("[MMIBT] HandleConfirmationDialogMsg: is_recieve=%d", is_recieve);
        if(is_recieve)
        {
           //create incoming window
           MMIBT_EnterFileIncomingWin(&s_transfer_file_info);
        }
        break;
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : open confirmation dialog
//	Global resource dependence : none
//  Author: weipeng.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIBT_OpenConfirmationDialog(MMI_TEXT_ID_T text_id, BOOLEAN is_recievie)
{
    SCI_TRACE_LOW("[MMIBT] line[%d] MMIAPIDC_OpenConfirmationDialog");
    MMIPUB_OpenAlertWinByTextIdEx(
                                                            MMK_GetFirstAppletHandle(), 
                                                            PNULL, 
                                                            text_id,
                                                            TXT_NULL, 
                                                            IMAGE_NULL,
                                                            PNULL,
                                                            PNULL,
                                                            MMIPUB_SOFTKEY_DIALOG_TURNOFF_CLOSE,
                                                            HandleConfirmationDialogMsg,
                                                            is_recievie);
}


/*****************************************************************************/
//  Discription: do operation by  MMIBT_OPERTATION_TYPE_E
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN DoOperation(MMIBT_OPERTATION_TYPE_E  current_opt, BOOLEAN is_poweron_first)
{
   BT_STATUS return_value = 0;
   MMI_STRING_T disp_text1 = {0}; /*lint !e64*/
   MMI_STRING_T disp_text2 = {0}; /*lint !e64*/
   //wchar        *text1_buf_ptr = PNULL;
   wchar        *text2_buf_ptr = PNULL;
   //lulifei add for the progress dialog display
   wchar text_temp[64] = {0};
   wchar new_string[50] = {0};
   MMI_STRING_T final_display_string = {0};
   MMI_WIN_ID_T               visible_win = MMIBT_CONNECT_DEVICE_WAIT_WIN_ID;
   MMI_WIN_PRIORITY_E         win_priority_ptr = WIN_ONE_LEVEL;

#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
   BT_ADDRESS addr = {0};
#endif
   BT_DEVICE_INFO  pin_request_info = {0};
#ifdef MMI_BT_PBAP_SUPPORT
   BT_ADDRESS   dev_addr = {0};
#endif

   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8285_112_2_18_2_8_17_302,(uint8*)"dd",current_opt, is_poweron_first);

   if(is_poweron_first && (!BT_GetState()))
   {
      //power on bt first
       MMIBT_OpenBluetooth(TRUE, TRUE);
       return TRUE;
   }
   switch(current_opt)
   {
   case MMIBT_OPC_POWER_ONOFF:
       //open bt
       MMIBT_OpenBluetooth(TRUE, TRUE);
       break;
   case MMIBT_OPC_SEARCH:
        //search
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
       //HL added it for not support search while blue headset connected
       if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr))
       {
           MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ACTIVE_HEADSET_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
           break;
       }
       //add for not support search while ftp connected
       if(MMIBT_IsConnectedFTP())
       {
           MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DELETE_FTP_CONNECTED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
           break;
       }
       //added tonny.chen for HL bt
       if(MMIBT_IsOnTransferStatus())
       {
           MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DELETE_TRANSFERRING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
           break;
       }
       //end added
#endif
        
// #ifdef MMI_BT_PBAP_SUPPORT
//         if(MMIBT_IsConnectDevice(&dev_addr))
//         {
//             MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
//             break;
//         }
// #endif

       //pause BG play
       BtPauseBGPlay(MMIBT_SEARCH_SUSPEND);
       //open waiting win
       MMIBT_OpenSearchDeviceWaitWin(MMIBT_GetCurrentSevice());
       break;
   case MMIBT_OPC_VISIBILITY:
       MMIBT_EnterSetVisibilityWin();
       break;
#ifdef MMIBT_FTS_SUPPORT
   case MMIBT_OPC_SHARED_FILE_SETTING:
        //shared file setting
        MMIBT_EnterSharedWin();
       break;
#endif
   case MMIBT_OPC_OPP_SEND:
       //send file
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
       //HL added it for not support search while blue headset connected
       if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr))
       {
           MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ACTIVE_HEADSET_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
           break;
       }
       //added by tonnychen for HL bt
       if(MMIBT_IsOnTransferStatus())
       {
           MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DELETE_TRANSFERRING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
           break;
       }
       //end added
#endif
       //added by wangwp to close music when transfer files
#ifdef MMI_AUDIO_PLAYER_SUPPORT
       SCI_TRACE_LOW("[MMIBT] DoOperation:MMIBT_OPC_OPP_SEND %d, %d", MMIMP3_GetAudioState(), MMIAPIMP3_GetPlayerState());   
       if(MMIAPIMP3_IsPlayerPlaying())
       {
            MMIBT_OpenConfirmationDialog(STR_COM_TURN_BGD_MUSIC, FALSE);
            break;
       }
       else
       {
            MMIAPMAINWIN_Destory();
       }
#endif
#ifdef MMI_BT_PBAP_SUPPORT
//         if(MMIBT_IsConnectDevice(&dev_addr))
//         {
//             MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
//             break;
//         }
#endif
        s_transfer_file_info.server_type = MMIBT_FILE_SERVER_OPP_SEND;
        SCI_MEMCPY(&s_transfer_file_info.device_info,&g_current_bt, sizeof(BT_DEVICE_INFO));
        //call bt api to send file to paired device，目前还都是OPP Client方式的发送
        BtPauseBGPlay(MMIBT_FTP_SUSPEND);
        
        SCI_TRACE_BUF("[MMIBT] [DoOperation: MMIBT_OPC_OPP_SEND] transfer_file_name = ", s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)*sizeof(wchar));
        return_value = BT_OppSendFile(&g_current_bt.addr,s_transfer_file_info.transfer_file_name);
        if (BT_PENDING == return_value)
        {
            //sending file suspend MP3
            MMI_GetLabelTextByLang(STR_BT_CONNECTING_TO_EXT01, &disp_text1);
            text2_buf_ptr = (wchar *)SCI_ALLOC_APP(((BT_DEVICE_NAME_SIZE + 1)*sizeof(wchar)));
            if(text2_buf_ptr != PNULL)
            {
                SCI_MEMSET(text2_buf_ptr, 0x00, ((BT_DEVICE_NAME_SIZE + 1)*sizeof(wchar)));
                disp_text2.wstr_ptr = text2_buf_ptr;
                Get_Transfer_DeviceName(&disp_text2, BT_DEVICE_NAME_SIZE);
            }
            MMIAPICOM_Wstrncpy(text_temp, disp_text1.wstr_ptr,disp_text1.wstr_len);
            MMIAPICOM_Wstrncpy(new_string, disp_text2.wstr_ptr,disp_text2.wstr_len);
            MMIAPICOM_WstrReplace(text_temp, sizeof(text_temp), L"%U", new_string);
            final_display_string.wstr_ptr = text_temp;
            final_display_string.wstr_len = MMIAPICOM_Wstrlen(final_display_string.wstr_ptr);
            //set hidden of blue tooth for send file
            //MMIPUB_OpenWaitWin(2,&disp_text1, &disp_text2,PNULL,MMIBT_CONNECT_DEVICE_WAIT_WIN_ID,IMAGE_NULL,
            //ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER, HandleSendConnectWindow);//HandleSendConnectWindow);
            MMIPUB_OpenProgressWaitingIndWinByTextPtr(&final_display_string,&visible_win,&win_priority_ptr,MMIPUB_SOFTKEY_DIALOG_CLOSE,HandleSendConnectWindow);
            if(text2_buf_ptr != PNULL)
            {
                SCI_Free(text2_buf_ptr);
                text2_buf_ptr = PNULL;
            }
        }
        else if(BT_BUSY == return_value)
        {
            BtResumeBGPlay(MMIBT_FTP_SUSPEND);
            SCI_TRACE_LOW("DoOperation_OPC_OPP_SEND BT_BUSY");
            MMIPUB_OpenAlertFailWin(STR_MSA_SYS_BUSY);
            
        }
        else
        {
            BtResumeBGPlay(MMIBT_FTP_SUSPEND);
            //SCI_TRACE_LOW:"[&&&]DoOperation start,OPC_OPP_SEND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8372_112_2_18_2_8_17_304,(uint8*)"");
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
       break;
   case MMIBT_OPC_RECIEVE:
   	//added by wangwp to close music when transfer files
#ifdef MMI_AUDIO_PLAYER_SUPPORT  
       SCI_TRACE_LOW("[MMIBT] DoOperation:MMIBT_OPC_RECIEVE %d, %d", MMIMP3_GetAudioState(), MMIAPIMP3_GetPlayerState());
       if(MMIAPIMP3_IsPlayerPlaying())
       {
            MMIBT_OpenConfirmationDialog(STR_COM_TURN_BGD_MUSIC, TRUE);
            break;
       }
       else
       {
            MMIAPMAINWIN_Destory();
       }
#endif
       //create incoming window
       MMIBT_EnterFileIncomingWin(&s_transfer_file_info);
       break;

   case MMIBT_OPC_PAIR:
       //pair
       pin_request_info = MMIBT_GetPinRequestInfo();
       return_value = BT_PairDevice( &pin_request_info.addr);
       if (BT_PENDING != return_value)
       {
           MMIPUB_OpenAlertFailWin(TXT_ERROR);
       }
       else
       {
           MMIBT_SetSelfPair(TRUE);
           MMI_GetLabelTextByLang(STR_NOTE_WAITING, &disp_text1);
           MMIPUB_OpenWaitWin(1,&disp_text1,PNULL,PNULL,MMIBT_PAIR_REQUEST_WAIT_WIN_ID,IMAGE_NULL,
               ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
           MMIBT_StartOperationWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID,MMIBT_OPC_PAIR,CreatePubWinCallBack);
       }
        break;

   case MMIBT_OPC_FTP_SEND:
        //ftp send file, get file from device to other dir
        BtPauseBGPlay(MMIBT_FTP_SUSPEND);
        return_value = BT_FtpSendFile((uint16*)s_transfer_file_info.transfer_file_name);
        if(BT_PENDING == return_value)
        {
        	MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_FOREGROUND);
            //show sending progress
            MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                           MMIBT_FILE_SERVER_FTP_SEND,
                           s_transfer_file_info.transfered_size,
                           s_transfer_file_info.total_file_size);
        }
        else if(BT_SUCCESS == return_value)
        {
            //send success :show finished
            BtResumeBGPlay(MMIBT_FTP_SUSPEND);
            //send file success
            MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
            ClosePrmAndOpenDoneWin(
                MMIBT_SENDING_FILE_WAIT_WIN_ID,
                STR_BT_SEND_SUCCESS,
                IMAGE_PUBWIN_SUCCESS,
                MMI_2SECONDS
                );
			MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
        }
        else
        {
            //ftp send fail
            MMIBT_SendFileFailedInd();
			MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
        }
       break;
   case MMIBT_OPC_LIST_PAIRED:
       //list paired device
       MMIBT_EnterInquireDeviceListWin(PNULL,MMIBT_OPC_OPP_SEND,BT_SERVICE_FTP);
       break;
#ifdef MMI_BT_PBAP_SUPPORT
   case MMIBT_OPC_LIST_PAIRED_PBAP:
       //list paired device
       //MMIBT_EnterDeviceListWin(PNULL,MMIBT_OPC_OPP_SEND,BT_SERVICE_FTP);
       SCI_TRACE_LOW("[MMIBT] DoOperation MMIBT_OPC_LIST_PAIRED_PBAP send MMIBT_OPC_LIST_PAIRED_PBAP");
       MMIBT_EnterInquireDeviceListWin(PNULL,MMIBT_OPC_CONNECT_PBAP,BT_SERVICE_PBAP_SERVER);
       break;
#endif
   case MMIBT_OPC_VISIBLE:
       //set visibile
	   //CleanAllHideDevice();
       SetBtVisibility(MMI_BT_VISIBILE);
       break;
   case MMIBT_OPC_HIDE:
       //set visibile
	   //CleanAllHideDevice();
       SetBtVisibility(MMI_BT_HIDE);
       break;
   case MMIBT_OPC_SAVE_LOCATION:
       //set file save location
        MMIBT_EnterStorageSelectWin();
        break;
   case MMIBT_OPC_INPUT_PIN:
       //Input paired pin
	   BtPauseBGPlay(MMIBT_INPUTPIN_SUSPEND);
       pin_request_info = MMIBT_GetPinRequestInfo();
       MMIBT_CreatePinInputWin(&pin_request_info);
       break;
   case MMIBT_OPC_NUMERIC_PASSKEY_CHECK:
       //numeric passkey check
	   MMIBT_NumbericPasskeyCheck();
       break;
   case MMIBT_OPC_DEVICE_DETAILS:
       //device details
       MMIBT_EnterTextBoxWin(&(g_current_bt.addr),MMIBT_OPC_DEVICE_DETAILS);
       break;
   case MMIBT_OPC_OPEN_HEADSET:
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
		if(MMIBT_IsOnTransferStatus())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DELETE_TRANSFERRING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#endif
	   //Open Headset
       OpenHeadset(&(g_current_bt.addr));
        MMIBT_ReplaceDeviceListItem(&(g_current_bt),TRUE);

       break;
   case MMIBT_OPC_CLOSE_HEADSET:
       //Close Headset
       SCI_TRACE_LOW("[MMIBT] DoOperation MMIBT_OPC_CLOSE_HEADSET");
       return_value = CloseHeadset(&(g_current_bt.addr));
       handleHeadsetCloseResult(return_value, &(g_current_bt.addr));
       MMIBT_ReplaceDeviceListItem(&(g_current_bt),FALSE);
       break;
   case MMIBT_FTS_CONNECT_REQ:
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
        //HL added it for not support search while blue headset connected
        if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ACTIVE_HEADSET_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            BT_FtsConnect(FALSE);
            break;
        }
		//added by tonny.chen for HL bt
        if(MMIBT_IsOnTransferStatus())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DELETE_TRANSFERRING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#ifdef BT_HL_PATCH_SUPPORT
        MMIBT_SetTransferStatus(MMIBT_TRANSFER_FTS_CONNECTED);
		MMIBT_UpdateMainMenu(TRUE);
        //end added
#endif
#endif
        BT_FtsConnect(TRUE);
        break;
   case MMIBT_OPS_CONNECT_REQ:
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
        //HL added it for not support search while blue headset connected
        if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ACTIVE_HEADSET_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            BT_OpsConnect(FALSE);
            break;
        }
#endif
        BT_OpsConnect(TRUE);
       break;
   case MMIBT_OPC_FTS_DLE:
       //FTP Del
        //Exit DC,DV
        MMIAPIDC_Exit();
        //MMIAPIPB_CloseVCardWin();
        //Mp4

#ifdef VIDEO_PLAYER_SUPPORT
        //MMIAPIVP_ExitVideoPlayer(); //cr246138  @andy.he 20100616
#endif

        return_value = BT_DeleteFile(TRUE);
        //if delete file is wallpaper should reset wallpaper info
        if (BT_SUCCESS == return_value && GetIsDelWallpaperFile())
        {
            MMIAPIFMM_ResetWallpaperSetting();
        }

        if (BT_SUCCESS == return_value )
        {
            //Del success
            MMIBT_UpdateCurrentFileList();
            UpdateCurrentPictureViewList();
            MMIDEFAULT_SetBackLight(TRUE);
            //if any file(folder) has been deleted, then close new file window(querywin) to avoid
            //open the file which has been deleted by remote control device(i.e. pc) (bug79303)
            if(MMK_IsOpenWin(MMI_BT_NEW_FILE_WIN_ID))
            {
                MMK_CloseWin(MMI_BT_NEW_FILE_WIN_ID);
            }
        }
        else
        {
            //Del fail
            MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
       break;
   case MMIBT_OPC_DEVICE_DELETE:
#ifdef MMI_BT_PBAP_SUPPORT
       MMIBT_PbapDisconnect();
#endif
       DeletePairedDevice(FALSE,MMIBT_OPC_DEVICE_DELETE);
       break;
   case MMIBT_OPC_DEVICE_DELETE_ALL:
#ifdef MMI_BT_PBAP_SUPPORT
       MMIBT_PbapDisconnect();
#endif
       DeletePairedDevice(TRUE,MMIBT_OPC_DEVICE_DELETE_ALL);
       break;
   case MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT:
	{
		BT_SetDeviceAuthorisation(&g_current_bt.addr,TRUE);
		CheckAuthorizeAndOpenIncomingWin();
	}
       break;
#ifdef MMIBT_HEADSET_AUTHORIZE
   case MMIBT_HFG_AUDIO_CONNECT:
        BT_AcceptAuthorisation(&g_current_bt.addr, TRUE);
    break;
#endif
#ifdef MMI_BT_PBAP_SUPPORT
   case MMIBT_OPC_CONNECT_PBAP:
       {
           uint32        paired_service = 0;

           SCI_TRACE_LOW("[MMIBT] DoOperation MMIBT_OPC_CONNECT_PBAP do OpenHandsetGateway");
           OpenHandsetGateway(&(g_current_bt.addr));
           
           paired_service = BT_GetPairedDeviceService(&(g_current_bt.addr));
           if(paired_service & BT_SERVICE_PBAP_SERVER)
           {
               if(MMIAPIPB_GetActivateBTWinId())
               {
                   if(MMK_IsOpenWin(MMI_BT_QUERY_WIN_ID))
                   {
                       MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
                   }
                   
                   if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
                   {
                       MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
                   }
                   
                   SCI_TRACE_LOW("[MMIBT] DoOperation MMIBT_OPC_CONNECT_PBAP send MSG_BT_CONTACT_SYNC_CNF");
                   MMK_SendMsg(MMIAPIPB_GetActivateBTWinId(), MSG_BT_CONTACT_SYNC_CNF, PNULL);
               }
           }
       }

       break;
   case MMIBT_OPC_OPEN_HANDSET_GATEWAY:
       //Open Headset
       SCI_TRACE_LOW("[MMIBT] DoOperation MMIBT_OPC_OPEN_HANDSET_GATEWAY");
       OpenHandsetGateway(&(g_current_bt.addr));
       break;
   case MMIBT_OPC_CLOSE_HANDSET_GATEWAY:
       //Close Headset
       SCI_TRACE_LOW("[MMIBT] DoOperation MMIBT_OPC_CLOSE_HANDSET_GATEWAY");
       return_value = CloseHandsetGateway(&(g_current_bt.addr));
       handleHandsetGatewayCloseResult(return_value, &(g_current_bt.addr));
       break;
#endif
   default:
       break;
   }
   return TRUE;
}

/*****************************************************************************/
//  Discription: cancel operation by  MMIBT_OPERTATION_TYPE_E
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS CancelOperation(MMIBT_OPERTATION_TYPE_E  current_opt)
{
   BT_STATUS bt_status = BT_SUCCESS;
   BT_DEVICE_INFO  pin_request_info = {0};

   switch(current_opt)
   {
   case MMIBT_OPC_PAIR:
       BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
       MMIBT_SetPinCancel(TRUE);
       pin_request_info = MMIBT_GetPinRequestInfo();

       //We should not cancel pair immediately after begin pair.
	   //The low layer is already in message exchange.
       SCI_Sleep(3000);

       if(0 != MMIAPICOM_Wstrnlen((wchar*)pin_request_info.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
       {
           bt_status = BT_CancelPair(&pin_request_info.addr);
       }
       break;
   case MMIBT_OPC_SEARCH:
       bt_status = BT_CancelSearch();
       BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
       break;
   case MMIBT_OPC_INPUT_PIN:
       MMIBT_SetPinCancel(TRUE);
       pin_request_info = MMIBT_GetPinRequestInfo();
       if(0 != MMIAPICOM_Wstrnlen((wchar*)pin_request_info.addr.addr, BT_ADDRESS_SIZE/sizeof(wchar)))
       {
           bt_status = BT_CancelPair(&pin_request_info.addr);
       }
       BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
       break;
   case MMIBT_OPC_NUMERIC_PASSKEY_CHECK:
       MMIBT_CancelNumricPair();
       break;
   case MMIBT_OPC_RECIEVE:
   case MMIBT_OPC_OPP_SEND://Send
   case MMIBT_OPC_FTP_SEND:
        bt_status = MMIBT_CancelFileTransfer(TRUE);
        if(BT_PENDING != bt_status)
        {
            s_transfer_file_info.server_type = MMIBT_FILE_SERVER_MAX;
            if(BT_SUCCESS == bt_status)
            {
                //cancel success
                //MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);
            }
            else
            {
                //cancel error
                //SCI_TRACE_LOW:"[&&&]DoOperation start,CancelOperation MMIBT_OPC_OPP_SEND"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8551_112_2_18_2_8_17_305,(uint8*)"");
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
        }
        else
        {
            OpenCancelWaitingWin();
        }
        break;
   case MMIBT_FTS_CONNECT_REQ:
        BT_FtsConnect(FALSE);
        break;
   case MMIBT_OPS_CONNECT_REQ:
        BT_OpsConnect(FALSE);
        break;
   case MMIBT_OPC_FTS_DLE:
       //cancel ftp del
        bt_status = BT_DeleteFile(FALSE);
       break;
    case MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT:
	{
		BT_SetDeviceAuthorisation(&g_current_bt.addr,FALSE);
		CheckAuthorizeAndOpenIncomingWin();
	}
        break;
#ifdef MMIBT_HEADSET_AUTHORIZE
   case MMIBT_HFG_AUDIO_CONNECT:
        BT_AcceptAuthorisation(&g_current_bt.addr,FALSE);
        break;
#endif
   default:
       break;
   }
   return bt_status;
}



/*****************************************************************************/
//  Description : setting the state of connecting animate ctrl
//  Global resource dependence :
//  Author: wanqiangzh
//  Note:   state 联网动画当前需要设定的状态:1,动画;2,静止
///*****************************************************************************/
PUBLIC void AnimatePlayCtrlByState(int32 state)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id = MMIBT_ANIMATE_SEARCHING_CTRL_ID;
    data_info.anim_id =  AINM_BT_SEARCHING;//IMAGE_CMSBRW_CONNECTION_ANIM;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update = FALSE;
    if(MMIBT_ANIM_INIT == state)
    {
        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        }
    }
    else if(MMIBT_ANIM_PLAY == state)
    {
        //play
        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            GUIANIM_ResumeAnim(MMIBT_ANIMATE_SEARCHING_CTRL_ID);
        }
    }
    else if(MMIBT_ANIM_STOP == state)
    {
        //stop
        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
            GUIANIM_PauseAnim(MMIBT_ANIMATE_SEARCHING_CTRL_ID);
        }
    }
}

#ifdef BT_DUN_SUPPORT
// @baokun DUN
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author: baokun.yin
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenDUNWin(BT_MSG_T* msg_body_ptr)
{
    MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;

    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_OpenDUNWin fail PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9731_112_2_18_2_8_20_322,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMIBT_OpenDUNWin msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9734_112_2_18_2_8_20_323,(uint8*)"d",msg_body_ptr->status);
        //turn on the back light
    MMIDEFAULT_SetBackLight(TRUE);
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        uint32 i = MN_DUAL_SYS_1;
        BOOLEAN is_sim_ok = FALSE;

        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
           if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(i))
           {
                is_sim_ok = TRUE;
                // 此处只是用来判断是否有sim status OK,一旦有sim OK,直接break
                break;
           }
        }

        if(SIO_UserPortBusy())
        {
         //SCI_TRACE_LOW:"UART selected SIO_UserPortBusy"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9754_112_2_18_2_8_20_324,(uint8*)"");
          MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMM_UCOM_BUSY,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        //PC数据通信包含了对SMS和PB的操作,No sim 不允许操作
        else if((MMIAPISMS_IsOrderOk()) && (MMIAPIPB_IsPbReady()) && (is_sim_ok))
        {
             MMI_STRING_T query_text = {0};
             uint32       alert_time_period = 0;
             MMI_WIN_ID_T alert_win_id = MMIBT_DUN_CONNECT_QUERY_WIN_ID;
             //SCI_TRACE_LOW:"UART selected"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9763_112_2_18_2_8_20_325,(uint8*)"");
             //MMK_CreateWin((uint32 *)MMIBT_UCOM_CONNECT_REQ_TAB, PNULL);
             //Open alert window
              MMI_GetLabelTextByLang(TXT_BT_ACCEPT_DUN_OR_NOT, &query_text);
              MMIPUB_OpenAlertWinByTextPtr(&alert_time_period,&query_text,PNULL,IMAGE_PUBWIN_QUERY,&alert_win_id,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleConnectDUNOrNotMsg);
        }
        else if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
        {
            //SCI_TRACE_LOW:"MMIAPIPHONE_GetSimStatus not ready"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9771_112_2_18_2_8_20_326,(uint8*)"");
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_SIM_NOT_SIM_EXT01,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIBT_OpenDUNWin error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9776_112_2_18_2_8_20_327,(uint8*)"");
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_NOTE_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }

}

 /*****************************************************************************/
//  Discription:
//
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_DUNOpenedInd(BT_MSG_T* msg_body_ptr)
{
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_DUNOpenedInd PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9794_112_2_18_2_8_20_328,(uint8*)"");
        return;
    }
      //SCI_TRACE_LOW:"MMIBT_DUNOpenedInd msg_body_ptr->status %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9797_112_2_18_2_8_20_329,(uint8*)"d",msg_body_ptr->status);
        //turn on the back light
    MMIDEFAULT_SetBackLight(TRUE);
      if (BT_SUCCESS == msg_body_ptr->status)
     {
          s_is_vdun_on = TRUE;
          if (MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
            {
        ClosePrmAndOpenDoneWin(
                    MMIBT_DUN_WAITING_PUBWIN_ID,
                    TXT_BT_DUN_CONNECTED,
                    IMAGE_PUBWIN_SUCCESS,
                    MMI_2SECONDS
                    );

            }
      }
      else if(MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
     {
          ClosePrmAndOpenDoneWin(
                      MMIBT_DUN_WAITING_PUBWIN_ID,
                      TXT_ERROR,
                      IMAGE_PUBWIN_FAIL,
                      MMI_2SECONDS
                      );
      }

}

  /*****************************************************************************/
//  Discription:
//
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_DUNClosedInd(BT_MSG_T* msg_body_ptr)
{
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_DUNClosedInd PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9837_112_2_18_2_8_20_330,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMIBT_DUNClosedInd msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9840_112_2_18_2_8_20_331,(uint8*)"d",msg_body_ptr->status);
    //turn on the back light
    MMIDEFAULT_SetBackLight(TRUE);
    MMK_CloseWin(MMIBT_DUN_CONNECT_QUERY_WIN_ID);
    if (BT_SUCCESS == msg_body_ptr->status)
    {

      s_is_vdun_on = FALSE;
     // if (MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
        {
            ClosePrmAndOpenDoneWin(
                    MMIBT_DUN_WAITING_PUBWIN_ID,
                    TXT_BT_DUN_DISCONNECT,
                    IMAGE_PUBWIN_SUCCESS,
                    MMI_3SECONDS
                    );
        }
    }
    else //if(MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
    {
        ClosePrmAndOpenDoneWin(
                    MMIBT_DUN_WAITING_PUBWIN_ID,
                    TXT_ERROR,
                    IMAGE_PUBWIN_FAIL,
                    MMI_3SECONDS
                    );
    }

}
/*****************************************************************************/
//  Discription: Handle try pair again or not win msg
//  Global resource dependence: none
//  Author: Wenming.feng
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleConnectDUNOrNotMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T        wait_text = {0};

    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"HandleConnectDUNOrNotMsg Cancel"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9888_112_2_18_2_8_20_332,(uint8*)"");
        BT_RejectDunReq();
        MMI_GetLabelTextByLang(TXT_BT_DUN_CANCEL, &wait_text);
        //s_is_vdun_on = FALSE;
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_DUN_WAITING_PUBWIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleDUNWaitingWinMsg);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //SCI_TRACE_LOW:"HandleConnectDUNOrNotMsg Accept"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9900_112_2_18_2_8_20_333,(uint8*)"");
        BT_AcceptDunReq();
        //s_is_vdun_on = TRUE;
        MMI_GetLabelTextByLang(TXT_DUN_CONNECT, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_DUN_WAITING_PUBWIN_ID,IMAGE_NULL,
           ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleDUNWaitingWinMsg);
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);;
        break;
    }
    return(result);
}
/*****************************************************************************/
//  Description :get vuart run status
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_DUNIsRun(void)
{
    return s_is_vdun_on;
}
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDUNWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id,
                                                    MMI_MESSAGE_ID_E msg_id,
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

    case MSG_APP_RED:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //User refuse to accept the file
        if(s_is_vdun_on)
          {
               BT_RejectDunReq();
          }
          MMK_CloseWin( win_id );
        break;
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;
}
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CloseDUNWin(void)
{
      if(MMK_IsOpenWin(MMIBT_DUN_CONNECT_QUERY_WIN_ID))
      {
          MMK_CloseWin( MMIBT_DUN_CONNECT_QUERY_WIN_ID );
      }
      if(MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
      {
          MMK_CloseWin( MMIBT_DUN_WAITING_PUBWIN_ID );
      }
}
#endif

/*****************************************************************************/
//  Discription: Open CancelW aiting Window
//  Parameter:
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void OpenCancelWaitingWin(void)
{
    //MMI_STRING_T wait_text = {0};
    MMI_WIN_PRIORITY_E      win_priority = WIN_ONE_LEVEL;
    MMI_WIN_ID_T            win_id = MMIENG_BT_CANCEL_WAIT_WIN_ID;

    //MMI_GetLabelTextByLang(TXT_BT_CANCELLING, &wait_text);
    //MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIENG_BT_CANCEL_WAIT_WIN_ID,IMAGE_NULL,
        //ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
    //MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_CONNECTION_CANCELLED_EXT01,PNULL,PNULL,&win_id,&win_priority,MMIPUB_SOFTKEY_DIALOG_OK,HandlePubwinCancelingMsg);
    if(!MMK_IsOpenWin(MMIENG_BT_CANCEL_WAIT_WIN_ID)){
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CANCELED,PNULL,PNULL,&win_id,&win_priority,MMIPUB_SOFTKEY_DIALOG_OK,HandlePubwinCancelingMsg);
    }
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none
//  Author: lifei.lu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinCancelingMsg(
                                          MMI_WIN_ID_T         win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_KEYDOWN_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;
}


/*****************************************************************************/
//  Discription:check file exist or not, if file not exist, open incoming window;
//              if file already exist,query user overwrite or not if user selected
//              "Yes", open incoming window, if user select "No", cancel recevie file
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void CheckAuthorizeAndOpenIncomingWin(void)
{
    BOOLEAN       be_authorize = FALSE;

    BT_GetDeviceAuthorisation(&g_current_bt.addr,&be_authorize);

         SCI_TRACE_LOW("CheckAuthorizeAndOpenIncomingWin, be_authorize = %d", be_authorize);
	if(MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT == MMIBT_GetOpertor().current_opertion)
	{
		return;
	}
  	else if(MMIBT_FTS_CONNECT_REQ == MMIBT_GetOpertor().current_opertion
           ||MMIBT_OPS_CONNECT_REQ == MMIBT_GetOpertor().current_opertion
           ||MMIBT_OPC_RECIEVE == MMIBT_GetOpertor().current_opertion
        )
	{
#if 0
		if(be_authorize)
		{
            if(AssignReceiveFileCfg(s_transfer_file_info.server_type, FALSE))
            {
                //enter receiving file win
                MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                               s_transfer_file_info.server_type,
                               s_transfer_file_info.transfered_size,
                               s_transfer_file_info.total_file_size);
            }
		}
		else
#endif
		{
			DoOperation(MMIBT_OPC_RECIEVE, FALSE);
		}
	}
}

/*****************************************************************************/
//  Discription:display memory or udisk not exist or sd card not exist
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMemoryInformation(MMIFILE_DEVICE_E filedev)
{
    MMI_TEXT_ID_T text_id1 = TXT_NULL;

    if(!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(filedev),
                                MMIAPIFMM_GetDevicePathLen(filedev)))
    {
        //sd card not exist
        text_id1 = MMIAPIFMM_GetAlertTextIDByDev(filedev);
    }
    else
    {
        text_id1 = TXT_CMSBRW_FULL_NO_STORE;
    }

    MMIPUB_OpenAlertWinByTextId(PNULL,text_id1,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleSaveNewFileFailedWinMsg);
}
/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_Reset(void)
{
    MMIBT_USER_SETTING_U  setting = {0};/*lint !e64*/
    //MMIFILE_DEVICE_E filedev = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    //SCI_TRACE_LOW:"MMIBT_Reset enter"
    MMIbt_resetflag = TRUE;
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10155_112_2_18_2_8_21_336,(uint8*)"");
    MMIAPISET_SetWaitFlag();
    MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
    //reset bt local name
    BT_SetLocalName(BT_DEFAULT_LOCAL_NAME);
    //hide bt
    BT_SetVisibility(BT_PAGE_ENABLE);
    //set defaul shairded dir

    strcpy((char*)setting.authority.file_mode, MMIBT_FTP_RW_MODE);
    MMINV_WRITE(MMINV_BT_SHAIRED_AUTHORITY,&setting);
    SCI_MEMSET(&setting, 0x00, sizeof(MMIBT_USER_SETTING_U ));
    dev = MMIAPIFMM_GetDefaultDisk();

    MMIAPICOM_Wstrncpy(
        setting.dir.shaired_path_name,
        MMIAPIFMM_GetDevicePath(dev),
        MMIAPIFMM_GetDevicePathLen(dev)
        );
    setting.dir.shaired_path_name[1] = MMIFILE_COLON;

    MMINV_WRITE(MMINV_BT_SHAIRED_DIRECTORY,&setting);
    BT_SetFtpRootFolder((uint16*)setting.dir.shaired_path_name, (char*)MMIBT_FTP_RW_MODE);
    //set opp file location
    SCI_MEMSET(&setting, 0x00, sizeof(MMIBT_USER_SETTING_U));
    setting.filedev = dev;
    MMINV_WRITE(MMINV_BT_FILE_LOCATION,&setting);
    MMIBT_SetFileSaveLocation();

    //delete all
    MMIBT_ClearAllDevice(FALSE);
    if(!MMIBT_OpenBluetooth(FALSE, FALSE))
	{
              SCI_TRACE_LOW("MMIBT_Reset MMIBT_OpenBluetooth FALSE");
		//need not to wait confirm
		MMIAPISET_FuncFinishedInd();
	}
    SCI_TRACE_LOW("MMIBT_Reset Exit");
}

/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_ClearAllDevice(BOOLEAN is_delect_connect)
{
    int            device_number = 0;
    int            i = 0;
    BT_DEVICE_INFO dev_info = {0};

    //delete all
    device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);
    SCI_TRACE_LOW("MMIBT_ClearAllDevice device_number=%d", device_number);
	for(i =device_number; i>= 1; i--)
	{
		BT_GetPairedDeviceInfo(BT_SERVICE_ALL, i, &dev_info);
		if(is_delect_connect
            ||(!MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr))
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
               &&(!MMIBT_DeviceIsConnectedFTP(&(dev_info.addr)))
#endif
               &&(!MMIBT_DeviceIsOnTransferStatus(&(dev_info.addr),&s_transfer_file_info.device_info.addr))
              )
           )
		{
			BT_RemovePairedDevice(&(dev_info.addr));
		}
	}
}





/*****************************************************************************/
//  Description : is premit open this file or not
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Is_PermitOpenFile(void)
{
    BOOLEAN      result = FALSE;
    MMI_WIN_ID_T   previous_win_id = 0;

    previous_win_id = MMK_GetFocusWinId();
    //previous_win_id = MMK_GetPrevWinId(previous_win_id);
    if(MMIKL_CLOCK_DISP_WIN_ID == previous_win_id)
    {
        previous_win_id = MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID);
    }
     //if current in idle, mainmenu or BT mainmenu, can permit open the recieved file
     if(MAIN_IDLE_WIN_ID == previous_win_id || MAIN_MAINMENU_WIN_ID == previous_win_id || MMIBT_MAIN_MENU_WIN_ID == previous_win_id)
     {
         result = TRUE;
     }
     else
     {
         result = FALSE;
     }
     //SCI_TRACE_LOW:"Is_PermitOpenFile result %d previous_win_id 0x%x "
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10608_112_2_18_2_8_22_341,(uint8*)"dd", result, previous_win_id);
     return result;
}

/*****************************************************************************/
//  Discription:  close send file windows
//  Global resource dependence: None
//  Author:
//  Note : if current opertation is opp_send, close inquery list and device list
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN  CloseSendWin(void)
{
   MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();

   if(MMIBT_OPC_OPP_SEND == operation_info.root_opertion)
   {
	  MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
	  MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
	  MMK_SendMsg(MMIBT_PIN_INPUT_WIN_ID, MSG_APP_CANCEL, PNULL);
   }
   return TRUE;
}

/*****************************************************************************/
//  Discription:  MMIBT_CloseStorageMenu
//  Global
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_CloseStorageMenu(void)
{
	MMK_CloseWin(MMIBT_SHARED_DIRECTORY_WIN_ID);
	MMK_CloseWin(MMIBT_SET_SAVE_LOCATION_WIN_ID);
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN MMIBT_IsEnoughSpaceToRecieve(MMIFILE_DEVICE_E file_dev)
{
    BOOLEAN result = FALSE;

     MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name,
         MMIAPIFMM_GetDevicePath(file_dev),
         MMIAPIFMM_GetDevicePathLen(file_dev));

    if (MMIAPIFMM_IsDevEnoughSpace(s_transfer_file_info.total_file_size, file_dev)
            ||( MMIAPIFMM_IsFileExist(s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name))
                && MMIBT_DiskEnoughSpaceForOverWrite(s_transfer_file_info.transfer_file_name, s_transfer_file_info.total_file_size,file_dev))
            )
    {
        result = TRUE;
    }

    return result;

}

/*****************************************************************************/
//  Description : open direct and get user selected dir
//  Global resource dependence :
//  Author:
//  Note: after user selected a dir, it will be set to shaired directory
/*****************************************************************************/
PUBLIC void MMIBT_SetFileLocationCallBack( BOOLEAN     is_success,
                                    FILEARRAY   file_array
                                    )
{
    uint32  file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    wchar   full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};

    //SCI_TRACE_LOW:"MMIBT_SetFileLocationCallBack, Enter,is_success = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11325_112_2_18_2_8_23_362,(uint8*)"d",is_success);
    if(is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        //SCI_TRACE_LOW:"MMIBT_SetFileLocationCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11329_112_2_18_2_8_23_363,(uint8*)"d", file_num);
        if(file_num > 0)
        {
            //read user selected dir
            if(MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data))
			{
				//set shaired directory
				MMIDEFAULT_SetBackLight(TRUE);
				MMIAPICOM_GetLongFileNameByFullPath(s_transfer_file_info.transfer_file_name, full_path_name);//arvin zhang fixed for Bug 4288(full_path_name存储文件全名，i.e. sample.jpg)
                SCI_MEMSET(s_transfer_file_info.transfer_file_name,0x00, sizeof(s_transfer_file_info.transfer_file_name));
                MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name,file_array_data.filename,MIN(file_array_data.name_len, MMIFILE_FULL_PATH_MAX_LEN ));
                MMIAPICOM_Wstrcat(s_transfer_file_info.transfer_file_name, L"\\");
                MMIAPICOM_Wstrcat(s_transfer_file_info.transfer_file_name, full_path_name);
                s_transfer_file_info.filedev = MMIAPIFMM_GetDeviceTypeByPath(s_transfer_file_info.transfer_file_name, MMIFILE_DEVICE_LEN);

                if (MMIBT_IsEnoughSpaceToRecieve(s_transfer_file_info.filedev))
                {
    				//overwirte space enough
                    CheckAuthorizeAndOpenIncomingWin();
    			}
                else
                {
                    //other memory space is not enough too
                    DisplayMemoryInformation(s_transfer_file_info.filedev);
                    MMIBT_CancelFileTransfer(TRUE);
                }
            }
            else
            {
                //folder not exist or other error
                MMIPUB_OpenAlertWarningWin(TXT_FOLDER_NOT_EXIST);
            }
        }
    }
    else
    {
        MMIBT_CancelFileTransfer(TRUE);
    }
    //释放列表
    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}
/*****************************************************************************/
//  Discription: None
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSaveNewFileFailedWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_SOFTKEY_STOP_MK, FALSE);
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    case MSG_CTL_CANCEL:
    //case MSG_APP_RED:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetCurBtInfo(BT_DEVICE_INFO *device_info)
{
    SCI_MEMSET(&g_current_bt,0x00, sizeof(BT_DEVICE_INFO));
    if(PNULL != device_info)
    {
    	SCI_MEMCPY(&g_current_bt, device_info, sizeof(BT_DEVICE_INFO));
    }
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC BT_DEVICE_INFO  MMIBT_GetCurBtInfo(void)
{
    return g_current_bt;
}
/*****************************************************************************/
//  Discription: get bluetooth transfer file status.(foreground/background)
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC MMIBT_TRANSFER_STATUS_TYPE_E  MMIBT_GetTransferStatus(void)
{
    return s_bt_transfer_status;
}
/*****************************************************************************/
//  Discription: set blue tooth send or receive file status
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetTransferStatus(MMIBT_TRANSFER_STATUS_TYPE_E transfer_status)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_SetTransferStatus transfer_status %d", transfer_status);
    s_bt_transfer_status = transfer_status;
    if((MMIBT_TRANSFER_END == transfer_status) || (MMIBT_TRANSFER_FREE == transfer_status))
    {
        SCI_MEMSET(&s_transfer_file_info.device_info,0x00, sizeof(s_transfer_file_info.device_info));
        s_transfer_file_info.transfered_size = 0;
    }
}
/*****************************************************************************/
//  Discription: set blue tooth send or receive file Transfered Size
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetTransferedSize(uint32 size)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_SetTransferedSize transfered_size %d", size);
    s_transfer_file_info.transfered_size = size;
}
/*****************************************************************************/
//  Discription: if blue tooth on transfer status
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN  MMIBT_IsOnTransferStatus(void)
{
    BOOLEAN 					 result = FALSE;
	MMIBT_TRANSFER_STATUS_TYPE_E transfer_stats = MMIBT_TRANSFER_END;

	transfer_stats = MMIBT_GetTransferStatus();
    if((MMIBT_TRANSFER_END != transfer_stats) && (MMIBT_TRANSFER_FREE != transfer_stats))
        result = TRUE;

    SCI_TRACE_LOW("[MMIBT] MMIBT_IsOnTransferStatus result %d", result);
    return result;
}
/*****************************************************************************/
//  Description : Open BT transfer file win(send/receive)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_OpenTransferWin(void)
{
    if(MMIBT_SENDING_FILE_BACKGROUND == MMIBT_GetTransferStatus())
    {
        MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_FOREGROUND);
    }
    else if(MMIBT_REVEIVEING_FILE_BACKGROUND == MMIBT_GetTransferStatus())
    {
        MMIBT_SetTransferStatus(MMIBT_RECEIVEING_FILE_FOREGROUND);
    }
    MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                               s_transfer_file_info.server_type,
                               s_transfer_file_info.transfered_size,
                               s_transfer_file_info.total_file_size);
}

/*****************************************************************************/
//  Description : display receive file ratio info
//  Global resource dependence :
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void DisplayFtpFileSizeInfo(
                                  uint32    file_size,
                                  MMI_CTRL_ID_T ctrl_id,
								  MMI_WIN_ID_T win_id
                                  )
{
	MMI_STRING_T        size_str = {0};
    wchar               wsize_info[(MMIBT_FILE_LENGTH_INFO << 1) + 1] = {0};
	uint16              wstr_len = 0;

	//get tranfered size string
	MMIAPIFMM_GetFileSizeString(file_size, wsize_info, MMIBT_FILE_LENGTH_INFO, FALSE);
	wstr_len = MMIAPICOM_Wstrlen(wsize_info);
	if(wstr_len + 1 < (MMIBT_FILE_LENGTH_INFO << 1))
	{
		wsize_info[wstr_len] = '/';
	}
    wstr_len ++;
	//get total file size string
	if(wstr_len  < (MMIBT_FILE_LENGTH_INFO << 1))
	{
		MMIAPIFMM_GetFileSizeString(s_transfer_file_info.total_file_size, &wsize_info[wstr_len], (MMIBT_FILE_LENGTH_INFO << 1) - wstr_len, FALSE);
	}
    size_str.wstr_ptr  = wsize_info;
	size_str.wstr_len = MMIAPICOM_Wstrlen(wsize_info);
    GUILABEL_SetText(ctrl_id,&size_str, TRUE);
}


/*****************************************************************************/
//  Description : get transfer file name
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Get_Transfer_FileName(MMI_STRING_T *file_name)
{
    if(PNULL ==file_name || PNULL == file_name->wstr_ptr)
    {
        //SCI_TRACE_LOW:"file_name string invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10357_112_2_18_2_8_21_339,(uint8*)"");
        return FALSE;
    }
    MMIAPICOM_GetLongFileNameByFullPath(s_transfer_file_info.transfer_file_name, file_name->wstr_ptr);
    file_name->wstr_len = MMIAPICOM_Wstrlen( file_name->wstr_ptr);
    return TRUE;
}


/*****************************************************************************/
//  Description : get transfer file total size
//  Global resource dependence : none
//  Author:life.lu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetTransferedTotalSize(void)
{
    return s_transfer_file_info.total_file_size;
}


/*****************************************************************************/
//  Description : get transfer device name
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Get_Transfer_DeviceName(MMI_STRING_T *device_name, uint16 max_str_len)
{
    if(PNULL ==device_name || PNULL == device_name->wstr_ptr)
    {
        //SCI_TRACE_LOW:"Get_Transfer_DeviceName string invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10341_112_2_18_2_8_21_338,(uint8*)"");
        return FALSE;
    }
    device_name->wstr_len = MIN(MMIAPICOM_Wstrlen(s_transfer_file_info.device_info.name), max_str_len);/*lint !e666*/
    MMIAPICOM_Wstrncpy(device_name->wstr_ptr, s_transfer_file_info.device_info.name, device_name->wstr_len);
    return TRUE;
}
/*****************************************************************************/
//  Discription: Update current picture view list
//  Global resource dependence: none
//  Author: haiwu.chen
//  Note: update picture view list when is needed
/*****************************************************************************/
PUBLIC void UpdateCurrentPictureViewList(void)
{
#ifdef PIC_VIEWER_SUPPORT
    MMI_STRING_T string = {0};
    wchar *path_name_ptr = PNULL;
    uint16 suffix_len = MMIFMM_FILENAME_LEN;
    uint32 type =0;
    wchar list_suffix_name[MMIFMM_FILENAME_LEN+1] = {0};
    BOOLEAN isPicOrVideo = FALSE;
    //SCI_TRACE_LOW:"UpdateCurrentPictureViewList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10130_112_2_18_2_8_21_335,(uint8*)"");
	
    path_name_ptr = SCI_ALLOCA((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
	
    if(path_name_ptr != PNULL)
    {
        SCI_MEMSET(path_name_ptr, 0x00, (MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
        MMIAPICOM_GetLongFilePathNameByFullPath(s_transfer_file_info.transfer_file_name, path_name_ptr);
        //检查图片保存的路径是否是默认的图片浏览打开的目录
        string.wstr_ptr = path_name_ptr;
        string.wstr_len = MMIAPICOM_Wstrlen(path_name_ptr);
        //zhuyongjing change when bt receive pic/video, gallery open, gallery reload
		/*
        if (MMIAPIPICVIEW_IsDefaultPath(string))
        {
            MMIAPIPICVIEW_ReloadFile();
		}
		*/
		if (MMIAPIFMM_SplitFileName (s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name), PNULL, PNULL, list_suffix_name, &suffix_len))
		{
			type = MMIAPIFMM_GetFileType (list_suffix_name, suffix_len);
			if((type==MMIFMM_MOVIE_MP4) ||(type==MMIFMM_MOVIE_3GP) || (type==MMIFMM_MOVIE_RMVB)
				|| (type==MMIFMM_MOVIE_AVI) || (type==MMIFMM_MOVIE_FLV)||(type==MMIFMM_PICTURE_GIF)
				||(type==MMIFMM_PICTURE_BMP)||(type==MMIFMM_PICTURE_WBMP)||(type==MMIFMM_PICTURE_PNG)
				||(type==MMIFMM_PICTURE_JPEG)||(type==MMIFMM_PICTURE_JPG))
			{
				isPicOrVideo =  TRUE;
			}
		}
		if(MMIAPIPICVIEW_IsPicFolderWinOpen() && isPicOrVideo)
		{
			MMIAPIPICVIEW_ReloadFile(); 
			MMIPICVIEW_InsertPicArray(s_transfer_file_info.transfer_file_name);
			
		}
        //change end
        SCI_FREE(path_name_ptr);
    }
#endif
}
/*****************************************************************************/
//  Discription: connect device to send file result ind
//  Global resource dependence: none
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDeviceResCnf( BT_MSG_T* msg_body_ptr )
{
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_ConnectDeviceResCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4130_112_2_18_2_8_8_247,(uint8*)"");
        return ;
    }

    MMIDEFAULT_SetBackLight(TRUE);
    if (BT_SUCCESS == msg_body_ptr->status)
    {
		if(MMK_IsOpenWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID))
		{
            MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                               MMIBT_FILE_SERVER_OPP_SEND,
                               s_transfer_file_info.transfered_size,
                               s_transfer_file_info.total_file_size);
			//MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_FOREGROUND);
			MMIBT_UpdateMainMenu(TRUE);
			MMK_CloseWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID);
		}
		else
		{
			BtResumeBGPlay(MMIBT_FTP_SUSPEND);
			MMIBT_CancelFileTransfer(TRUE);
		}
    }
    else
    {
        //connect device fail, resume mp3
        BtResumeBGPlay(MMIBT_FTP_SUSPEND);
        //reset blue tooth status
        //MMIBT_RecoveryBtStatus(MMIBT_MODULE_BLUETOOTH);

        if ( MMK_IsOpenWin( MMIBT_CONNECT_DEVICE_WAIT_WIN_ID ))
        {
            //SCI_TRACE_LOW:"[&&&]MMIBT_ConnectDeviceResCnf"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4161_112_2_18_2_8_8_248,(uint8*)"");
            MMIPUB_OpenAlertFailWin(STR_BT_SEND_FAIL);
            MMK_CloseWin( MMIBT_CONNECT_DEVICE_WAIT_WIN_ID );
        }
    }
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
    }
}

/*****************************************************************************/
//  Discription: get transfered file size
//  Global resource dependence: none
//  Author: yang.yang
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetTransferedSize(void)
{
    return s_transfer_file_info.transfered_size;
}

#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_GetDeviceTypeByService
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_GetDeviceType(BT_DEVICE_INFO * dev_info, uint16 * dev_type)
{
    uint32       dev_service = 0;
    BT_STATUS    ret_val = BT_SUCCESS;
    
    dev_service = BT_GetPairedDeviceService(&dev_info->addr);
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType dev_service = 0x%x", dev_service);
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType dev_service = 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", dev_info->addr.addr[0],dev_info->addr.addr[1],dev_info->addr.addr[2],dev_info->addr.addr[3],dev_info->addr.addr[4],dev_info->addr.addr[5]);
    
    ret_val = BT_GetDeviceType(dev_info, dev_type);
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType dev_type = 0x%x", *dev_type);
    
    if(BT_DEVICE_CLASS_COMPUTER == *dev_type)
    {
        return ret_val;
    }
    
    if ((BT_SERVICE_HANDFREE_GATEWAY |BT_SERVICE_HEADSET_GATEWAY) & dev_service)
    {
        *dev_type |= BT_DEVICE_CLASS_PHONE;
        SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType with HFG dev_type = 0x%x", *dev_type);
    }
    
    if ((BT_SERVICE_MAP_SERVER |BT_SERVICE_PBAP_SERVER) & dev_service)
    {
        *dev_type |= BT_DEVICE_CLASS_PHONE;
        SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType with HFG dev_type = 0x%x", *dev_type);
    }
    
    if ((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & dev_service)
    {
        *dev_type |= BT_DEVICE_CLASS_AUDIO;
        SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType with HFC dev_type = 0x%x", *dev_type);
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType final dev_type = 0x%x", *dev_type);
    
    return ret_val;
}

/*****************************************************************************/
//  Description : receive msg: AG device connected
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDeviceCnf(BT_MSG_T* msg_body_ptr)
{
    BT_ADDRESS     address = {0};
    MMI_WIN_ID_T   alert_id = MMI_BT_DEV_CONNECT_WIN_ID;
    
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf PNULL == msg_body_ptr!");
        return;
    }
    
    address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
    SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf address = %x, %x, %x, %x, %x, %x",
        address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
    SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf msg_body_ptr->status %d", msg_body_ptr->status);
    
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        MMINV_WRITE(MMINV_BT_ADDRESS_INFO,&address);
        SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf Device connect success!");
        //MMIPUB_OpenAlertSuccessWin(TXT_BT_DEVICE_CONNECTED);
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_STATUS_CONNECTED_EXT01,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        /*if(MMK_IsOpenWin(MMIBT_BTPHONE_WIN_ID))
        {
            AppendOneLineTextIconListItem(MMIBT_BTPHONE_LIST_CTRL_ID,TXT_BT_CONNECTION,IMAGE_CHECK_SELECTED_ICON,BT_PHONE_INDEX_CONNECTION,TXT_BT_DEVICE_DISCONNECT,TRUE);
        }*/
        MMIBT_GetBtStatusForIcon();
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }

#ifndef WIN32
#if defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
        BT_SppConnect(&address, SPP_PORT_SPEED_115200);
#endif
#endif

        /* Connect to other HFG profile */
//        ConnectHandsetGatewayService();
        MMIAPIPB_SetAllowSync(TRUE);
    }
    else if(BT_ERROR == msg_body_ptr->status)
    {
        SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf Device connect fail!");
        //MMIPUB_OpenAlertFailWin(STR_BT_TITLE_CONN_FAILED_EXT01);
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_TITLE_CONN_FAILED_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    
    MMK_CloseWin(MMIBT_CONNECT_WAIT_WIN_ID);
}

/*****************************************************************************/
//  Description : receive msg: AG device disconnected
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_DisconnectDeviceCnf(BT_MSG_T* msg_body_ptr)
{
    BT_ADDRESS     address = {0};
    MMI_WIN_ID_T   alert_id = MMI_BT_DEV_DISCONNECT_WIN_ID;
    BT_ADDRESS            dev_addr = {0};
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_DisconnectDeviceCnf");

    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf PNULL == msg_body_ptr!");
        return;
    }
    
    address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
    SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf address = %x, %x, %x, %x, %x, %x", 
        address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
    SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf msg_body_ptr->status %d", msg_body_ptr->status);
    
    if((BT_SUCCESS == msg_body_ptr->status) || (BT_TIMEOUT == msg_body_ptr->status))
    {
        SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf Device disconnect success!");
        //MMIPUB_OpenAlertSuccessWin(STR_BT_CONNECTION_CANCELLED_EXT01);

        if(!MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_CONNECTION_CANCELLED_EXT01,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        /*if(MMK_IsOpenWin(MMIBT_BTPHONE_WIN_ID))
        {
            AppendOneLineTextIconListItem(MMIBT_BTPHONE_LIST_CTRL_ID,TXT_BT_CONNECTION,IMAGE_CHECK_UNSELECTED_ICON,BT_PHONE_INDEX_CONNECTION,TXT_COMMON_CONNECT_COMN,TRUE);
        }*/

        /* Clean connected bt address data */
        SCI_MEMSET(&dev_addr,0,sizeof(BT_ADDRESS)); 
        MMINV_WRITE(MMINV_BT_ADDRESS_INFO,&dev_addr);

        MMIBT_GetBtStatusForIcon();
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }

#if (defined MMI_SMS_SYN_SUPPORT) && (defined BT_SPP_SUPPORT)
#ifndef WIN32
        BT_SppDisconnect();
#endif 
        MMK_CloseWin(MMIBT_SYNC_SMS_WAIT_WIN_ID);
        s_is_sms_sync = FALSE;
#endif

//         if(BT_TIMEOUT == msg_body_ptr->status)
//         {
//             MMIBT_HFUReconnect();
//         }
    }
    else if(BT_ERROR == msg_body_ptr->status)
    {
        SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf Device disconnect error!");
        //MMIPUB_OpenAlertFailWin(STR_BT_DEVICE_NO_REPOND_EXT01);
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DEVICE_NO_REPOND_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    
    MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    MMK_CloseWin(MMIBT_DISCONNECT_WAIT_WIN_ID);
#ifdef MMI_BT_PBAP_SUPPORT
//    MMICL_ClosePBAPWaitWin();
    MMIBT_StopPbapSyncTimer();

    MMIBT_PbapDisconnect();
    //MMIBT_PbapClientDeregister();

    if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SYNC_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
    }
#endif
}

/*****************************************************************************/
//  Description : MMIBT_DisconnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_DisconnectDevice(void)
{
#ifdef WIN32

#else
    MMI_STRING_T wait_text = {0};
    BT_STATUS    ret_val = BT_SUCCESS;
    MMI_WIN_ID_T alert_id = MMI_BT_DEV_DISCONNECT_WIN_ID;
    
#ifdef BT_HFU_SUPPORT
    ret_val = BT_HfuDisconnect();
#endif
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_DisconnectDevice ret_val = %d", ret_val);
#ifdef BT_HFU_SUPPORT
    if(BT_PENDING == ret_val)
    {
        
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_DISCONNECT_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
    }
    else if(BT_SUCCESS == ret_val)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_CONNECTION_CANCELLED_EXT01,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMIBT_GetBtStatusForIcon();
    }
    else if(BT_ERROR == ret_val)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DEVICE_NO_REPOND_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
#else
    MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_CONNECTION_CANCELLED_EXT01,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    MMIBT_GetBtStatusForIcon();
#endif

#if defined(BT_MAP_SUPPORT)
    if(MMIAPIBT_MapIsConnected())
    {
        //MMIAPISMS_MapDisconnect();
    }
#endif
    
#endif
}

/*****************************************************************************/
//  Description : MMIBT_ConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDevice(BT_ADDRESS *dev_addr)
{
    MMI_STRING_T wait_text = {0};
    BT_STATUS    ret_val = BT_SUCCESS;
    BT_STATUS    status = BT_SUCCESS;
    MMI_WIN_ID_T alert_id = MMI_BT_DEV_CONNECT_WIN_ID;
    uint32       device_num = BT_GetPairedDeviceCount(BT_SERVICE_PBAP_SERVER);
    uint32 i;
    
    BT_DEVICE_INFO dev_info = {0};
    BOOLEAN   exist = FALSE;
    for(i = 0; i <= device_num; i++)
    {
        status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDSET, i, &dev_info);
        if(BT_SUCCESS == status)
        {
            if(MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr)))
            {
                exist = TRUE;
                if(PNULL != dev_addr)
                {
                    SCI_MEMCPY(dev_addr, &(dev_info.addr), sizeof(BT_ADDRESS));
                }
                break;
            }
        }
        
    }//f

//     if(BT_PENDING == ret_val)
//     {
//      
//        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
//        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
//             ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
//     }
// 	else if(BT_SUCCESS == ret_val)
// 	{
// 		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_CONNECTED,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
// 		MMIBT_GetBtStatusForIcon();
// 	}
// 	else if(BT_ERROR == ret_val)
// 	{
// 		MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_TITLE_CONN_FAILED_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
// 	}
}

/*****************************************************************************/
//  Description : MMIBT_IsConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectDevice(BT_ADDRESS *dev_addr)
{
#ifdef WIN32
    return TRUE;
#else
    BOOLEAN is_connect = FALSE;

    if(MMIBT_HasConnected(dev_addr))
    {
#ifdef BT_HFU_SUPPORT
		is_connect = BT_HfuGetConnStatus(dev_addr);
#else
        is_connect = TRUE;
#endif
	}

    SCI_TRACE_LOW("[MMIBT] MMIBT_IsConnectDevice is_connect = %d", is_connect);
	return is_connect;
#endif
}

/*****************************************************************************/
//  Description : MMIBT_HasConnected
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HasConnected(BT_ADDRESS *dev_addr)
{
    uint8    default_addr[BT_ADDRESS_SIZE] = {0};
    BOOLEAN  has_connect = TRUE;
    
    MMIBT_GetPairedDevAddr(dev_addr);
    
    if(0 == memcmp(dev_addr, default_addr, sizeof(BT_ADDRESS)))
    {
        has_connect = FALSE;
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_HasConnected has_connect = %d", has_connect);
    return has_connect;
}

/*****************************************************************************/
//  Description : MMIBT_GetPairedDevAddr
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_GetPairedDevAddr(BT_ADDRESS *dev_addr)
{
    MN_RETURN_RESULT_E    return_value             = MN_RETURN_FAILURE;
    
    SCI_MEMSET(dev_addr, 0 , sizeof(BT_ADDRESS));
    MMINV_READ(MMINV_BT_ADDRESS_INFO,dev_addr,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(dev_addr,0,sizeof(BT_ADDRESS)); 
        MMINV_WRITE(MMINV_BT_ADDRESS_INFO,dev_addr);
    }
}

/*****************************************************************************/
//  Description : MMIBT_Init_NVItems
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_Init_NVItems(void)
{
    MN_RETURN_RESULT_E    return_value = MN_RETURN_FAILURE;
    BT_ADDRESS            dev_addr = {0};
    
    MMINV_READ(MMINV_BT_ADDRESS_INFO,&dev_addr,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&dev_addr,0,sizeof(BT_ADDRESS)); 
        MMINV_WRITE(MMINV_BT_ADDRESS_INFO,&dev_addr);
    }
}
#endif

/*Edit by script, ignore 14 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
//lulifei add for UI spec
LOCAL MMI_RESULT_E HandleBTNoDeviceWarningWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param) {
    MMI_RESULT_E         result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_WEB:
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

//lulifei add for UI spec
LOCAL MMI_RESULT_E HandleConnectCancelingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param) {
    MMI_RESULT_E         result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}


/*****************************************************************************/
// 	Description : handle vcard win
//	Global resource dependence : none
//  Author: lifei.lu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNewVcardWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E	res = MMI_RESULT_TRUE;
    MMIPUB_INFO_T       *pubwin_info_ptr = PNULL;
   uint32 file_type = MMIBT_VCARD_OTHERS;
    uint16 file_name_len = 0;
    MMI_WIN_ID_T alert_win = MMIBT_ASYN_WATING_WIN_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        res = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
         {
             pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
             if(PNULL != pubwin_info_ptr)
             {
                 file_type = pubwin_info_ptr->user_data;
             }
             file_name_len = (uint16) MMIAPICOM_Wstrlen (s_transfer_vcf_file_info.transfer_file_name);
             if(file_type ==MMIBT_VCARD_VCARD )
             {
#if defined MMI_VCARD_SUPPORT
                 wchar *file_name_ptr = PNULL;

                 MMK_CloseWin( win_id );
                 if(!MMIPB_IsPbReady())
                 {
                     MMIPB_DisplaybannerInfo(0, STR_CONTACTS_NOT_READY_EXT01, PNULL);
                     return MMI_RESULT_FALSE;
                 }

                 file_name_ptr=(wchar*)SCI_ALLOC_APP(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1));
                 if(PNULL==file_name_ptr)
                 {
                     break;
                 }
                 SCI_MEMSET(file_name_ptr,0,sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1));
                 MMIAPICOM_Wstrncpy(file_name_ptr, s_transfer_vcf_file_info.transfer_file_name, file_name_len);
                 SCI_TRACE_BUF("MMIBT_Func HandleNewVcardWinMsg file_name_ptr ",(char*)file_name_ptr,file_name_len);
                 if(MMIPB_ERROR_PENDING == MMIAPIPB_ImportVcardAsyn(MMIAPIPB_CreateHandle(), file_name_ptr, Importvcard_Callback))
                 {
                     //MMIPUB_OpenAlertWinByTextId(PNULL,STR_NOTE_WAITING,PNULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleImportVcardWaitingBtWinMsg);
                     MMIPUB_OpenProgressWaitingIndWinByTextId(STR_NOTE_WAITING,&alert_win,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleImportVcardWaitingBtWinMsg);
                 }
#ifdef MMIPB_SYNC_WITH_O365
                 else if(MMIPB_ERROR_IS_SYNCING == MMIAPIPB_ImportVcardAsyn(MMIAPIPB_CreateHandle(), file_name_ptr, Importvcard_Callback))
                 {
                     MMIPUB_OpenAlertWinByTextId(PNULL,STR_OPERATION_NOT_ALLOWED_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                 }
#endif
                 else
                 {
                     MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                 }
                 SCI_FREE(file_name_ptr);
#endif
		}else if (file_type ==MMIBT_VCARD_VCALENDAR){
#ifdef MMI_VCALENDAR_SUPPORT
            FILEARRAY file_array = MMIAPISCH_CreateFileArray();
            FILEARRAY_DATA_T   *data_ptr = PNULL;
            int i = 0;

	   MMK_CloseWin( win_id );
            data_ptr = (FILEARRAY_DATA_T*) SCI_ALLOC_APP (sizeof (FILEARRAY_DATA_T));
            if (PNULL == data_ptr)
            {
                if (PNULL != file_array)
                {
                    MMIAPIFILEARRAY_Destroy (&file_array);
                    file_array = PNULL;
                }

                break;
            }
            if (PNULL != file_array)
            {
                SCI_MEMSET (data_ptr, 0, sizeof (FILEARRAY_DATA_T));
                data_ptr->name_len = file_name_len;
                MMIAPICOM_Wstrncpy (data_ptr->filename, s_transfer_vcf_file_info.transfer_file_name, data_ptr->name_len);
                data_ptr->size =s_transfer_vcf_file_info.total_file_size;
                data_ptr->type = FILEARRAY_TYPE_FILE;
                MMIAPIFILEARRAY_Add (file_array, data_ptr);
	       SCI_TRACE_BUF("MMIBT_Func HandleNewVcardWinMsg file_array ",&file_array,data_ptr->name_len);
                MMIAPISCH_LoadVcalendar (file_array);
            }
            SCI_FREE (data_ptr);
            data_ptr = PNULL;
#endif
        }
	    break;
    	}
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }
    case MSG_CLOSE_WINDOW:
            if(s_is_vcf_file && (0 != memcmp(s_transfer_vcf_file_info.transfer_file_name, s_transfer_file_info.transfer_file_name, sizeof(s_transfer_vcf_file_info.transfer_file_name)))){
            CheckAuthorizeAndOpenIncomingWin();
        	}
	s_is_vcf_file = FALSE;
        res = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    default:
        {
            res = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
        }
    }
    return res;
}

/*****************************************************************************/
//  Description :  abort the vcard import
//  Global resource dependence :
// Author: lifei.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleImportVcardWaitingBtWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            recode = MMIPUB_HandleProgressWinMsg (win_id, msg_id, param);
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
            // 不处理
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            MMIAPIPB_AbortPB();
            recode = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
            break;

        default:
            recode = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
            break;
    }

    return recode;
}

/**************************************************************************************************/
// Description:import vcard call back function
// Date:
// Author:lifei.lu
// Note:
//*************************************************************************************************/
LOCAL void  Importvcard_Callback (MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMICOM_BANNER_INFO_T banner_info = {0};
    MMIAPIPB_CloseHandle (handle);
    //MMIPUB_CloseAlertWin();
    if(MMK_IsOpenWin(MMIBT_ASYN_WATING_WIN_ID)){
        MMIPUB_CloseWaitWin(MMIBT_ASYN_WATING_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_ALERT_VCF_WATING_WIN_ID)){
        MMIPUB_CloseWaitWin(MMIBT_ALERT_VCF_WATING_WIN_ID);
    }
    banner_info.text1_id = STR_VCARD_IMPORTED;
    if (MMIPB_ERROR_NO_SPACE == error)
    {
        //MMIPUB_OpenAlertWinByTextId (PNULL, TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        banner_info.text1_id = TXT_NO_SPACE;
    }
    else if (MMIPB_ERROR_SUCCESS != error
             && MMIPB_ERROR_NO_ENTRY != error)
    {
        banner_info.text1_id = STR_VCARD_IMPORT_FAIL;
    }
    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.start_point.x = 0;
    banner_info.start_point.y = 0;
    MMIAPICOM_OpenBannerWin(MMI_BT_NEW_FILE_WIN_ID, banner_info);

    MMK_CloseWin (MMI_BT_NEW_FILE_WIN_ID);
}

/*****************************************************************************/
//  Discription: set the vcf&vcs file
//  Global resource dependence: none
//  Author:lifei.lu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SetVcfAndVcsFile( )
{
    //lulifei add for the vcard&vcalendar file
    uint16      file_len = 0;
    wchar       suffix[MMIFMM_FILE_FULL_PATH_MAX_LEN+1] = {0};
    const wchar suffix_vcf[] = {'.', 'v', 'c','f', 0};
    const wchar suffix_vcs[] = {'.', 'v', 'c', 's', 0};
    uint16      suffix_len = MMIFMM_FILE_FULL_PATH_MAX_LEN;
    uint16      vcf_len = 0;
    uint16      vcs_len = 0;
    
    //lulifei modify for the vcard and vcalender
    file_len  = (uint16) MMIAPICOM_Wstrlen (s_transfer_file_info.transfer_file_name);
    //split file suffix
    MMIAPIFMM_SplitFullPathExt (s_transfer_file_info.transfer_file_name,
        file_len,
        PNULL, PNULL,
        PNULL, PNULL,
        suffix, &suffix_len);
    vcf_len  = (uint16) MMIAPICOM_Wstrlen (suffix_vcf);
    vcs_len = (uint16) MMIAPICOM_Wstrlen (suffix_vcs);
    if ((0 == MMIAPICOM_CompareTwoWstrExt (suffix, suffix_len, suffix_vcf, vcf_len, FALSE)) ||(0 == MMIAPICOM_CompareTwoWstrExt (suffix, suffix_len, suffix_vcs, vcs_len, FALSE)) )
    {
        s_is_vcf_file = TRUE;
        SCI_MEMSET(s_transfer_vcf_file_info.transfer_file_name, 0x00, sizeof(s_transfer_vcf_file_info.transfer_file_name));
        MMI_WSTRNCPY(s_transfer_vcf_file_info.transfer_file_name, MMIFILE_FULL_PATH_MAX_LEN, s_transfer_file_info.transfer_file_name, MMIFILE_FULL_PATH_MAX_LEN, MIN(file_len, MMIFILE_FULL_PATH_MAX_LEN));
        s_transfer_vcf_file_info.is_temp_file = s_transfer_file_info.is_temp_file;
        //store send file size info
        s_transfer_vcf_file_info.total_file_size = s_transfer_file_info.total_file_size;
    }
    else
    {
        SCI_MEMSET(s_transfer_vcf_file_info.transfer_file_name, 0x00, sizeof(s_transfer_vcf_file_info.transfer_file_name));
        s_transfer_vcf_file_info.total_file_size = 0;
    }
    
}


#endif

#ifdef MMI_BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_StartRecoTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_StartRecoTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_StartRecoTimer enter");
    
    if(s_hfu_reco_timer != 0) 
    {
        MMIBT_StopRecoTimer();
    }
    
    s_hfu_reco_timer = MMK_CreateTimerCallback(BT_HFU_RECO_TIMER_OUT, MMIBT_RecoTimeOut, 0, FALSE);
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_StartRecoTimer exit, timer_id = %d", s_hfu_reco_timer);
}

/*****************************************************************************/
//  Description : MMIBT_StopRecoTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_StopRecoTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopRecoTimer enter");
    
    if(s_hfu_reco_timer != 0)    
    {   
        MMK_StopTimer(s_hfu_reco_timer);   
        s_hfu_reco_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopRecoTimer exit, timer_id = %d", s_hfu_reco_timer);
}

/*****************************************************************************/
//  Description : MMIBT_HFUReconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_RecoTimeOut(uint8 timer_id, uint32 param)
{
    MMIBT_StopRecoTimer();
    s_hfu_reco_count++;
    SCI_TRACE_LOW("[MMIBT] MMIBT_RecoTimeOut, s_hfu_reco_count = %d", s_hfu_reco_count);
    
    if(s_hfu_reco_count < BT_HFU_RECO_COUNT)
    {
        MMIAPIBT_ConnectDevice();
        MMIBT_StartRecoTimer();
    }
    else
    {
        s_is_auto_reco = FALSE;
    }
}

/*****************************************************************************/
//  Description : MMIBT_HFUReconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_HFUReconnect()
{
    s_hfu_reco_count = 0;
    s_is_auto_reco = TRUE;
    MMIAPIBT_ConnectDevice();
    MMIBT_StartRecoTimer();
}

/*****************************************************************************/
//  Discription: get the device is active earphone by address of addr_ptr
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsDeviceActivedHandsetGateway(
                                                   const BT_ADDRESS    *addr_ptr
                                                  )
{
    BOOLEAN         connected = FALSE;
    
    if (PNULL == addr_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_IsDeviceActivedEarphone PNULL == addr_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_1633_112_2_18_2_8_2_239,(uint8*)"");
        return FALSE;
    }

#ifndef WIN32
#ifdef BT_HFU_SUPPORT
    connected = BT_HfuGetConnStatus(addr_ptr);/*lint !e605 */
#endif
#endif
    
    SCI_TRACE_LOW("MMIBT_IsDeviceActivedHandsetGateway: return value is %d", connected);
    
    return  connected;
}

/*****************************************************************************/
//  Description : MMIBT_IsConnectToAddr
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectToAddr(BT_ADDRESS *dev_addr)
{
#ifdef WIN32
    return FALSE;
#else
    BOOLEAN     is_connect = FALSE;
    BT_ADDRESS  default_addr = {0};
    
    if(MMIBT_HasConnected(&default_addr))
    {
        if(0 == memcmp(dev_addr, &default_addr, sizeof(BT_ADDRESS)))
        {
#ifdef BT_HFU_SUPPORT
            is_connect = BT_HfuGetConnStatus(dev_addr);
#endif
        }
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_IsConnectToAddr is_connect = %d", is_connect);
    return is_connect;
#endif
}

/*****************************************************************************/
//  Discription:check the deviece is connected headset
//  Global resource dependence: none 
//  Author: 
//  Note: if addr == NULL, it will check all paired device to find exist Connected Headset,
//        the connected headset will return in  out_addr
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsConnectedHandsetGateway(BT_ADDRESS * in_addr)
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_single_service = TRUE;
    uint32  service_type = 0;

    if(PNULL != in_addr)
    {
        //only check in_addr is connected headset or not
        //get current earphone status, if is active do not allow to delete device
        service_type = BT_GetPairedDeviceService(in_addr);
        if ((BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY) & service_type)
        {
            ret = MMIBT_IsDeviceActivedHandsetGateway(in_addr);
            is_single_service = FALSE;
        }

// Not necessary to check PBAP and MAP connection. 
#if 0
#ifdef BT_PBAP_SUPPORT
        if(!MMIBT_IsConnectToAddr(in_addr))
        {
            return FALSE;
        }
        // Not necessary to check PBAP connection. 
        if (BT_SERVICE_PBAP_SERVER & service_type)
        {
            if(is_single_service)
            {
                ret =  MMIAPIBT_PbapIsConnected();
            }
            else
            {
                ret &= MMIAPIBT_PbapIsConnected();
            }
            SCI_TRACE_LOW("[MMIBT] MMIBT_DeviceIsConnectedHandsetGateway: is connect to PBAP = %d", ret);
        }
#endif
        // Not necessary to check MAP connection. 
#ifdef BT_MAP_SUPPORT
        if (BT_SERVICE_MAP_SERVER & service_type)
        {
            if(is_single_service)
            {
                ret =  MMIAPIBT_MapIsConnected();
            }
            else
            {
                ret &= MMIAPIBT_MapIsConnected();
            }
            SCI_TRACE_LOW("[MMIBT] MMIBT_DeviceIsConnectedHandsetGateway: is connect to MAP = %d", ret);
        }
#endif
#endif
    }
    //SCI_TRACE_LOW:"MMIBT_DeviceIsConnectedHeadset ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9016_112_2_18_2_8_18_312,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//  Discription:check exist opened headset or not
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenedHandsetGatwayIsExist(BT_DEVICE_INFO *out_dev)
{
    uint32 device_num = BT_GetPairedDeviceCount(BT_SERVICE_HANDFREE_GATEWAY);    
    uint32 i = 0;
    BT_STATUS status = BT_SUCCESS;
    BT_DEVICE_INFO dev_info = {0};
    BOOLEAN   exist = FALSE;

    for(i = 1; i <= device_num; i++)
    {
        status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDFREE_GATEWAY, i, &dev_info);
        if(BT_SUCCESS == status)
        {
            if(MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr)))
            {
                exist = TRUE;
                if(PNULL != out_dev)
                {
                    SCI_MEMCPY(out_dev, &dev_info, sizeof(BT_DEVICE_INFO));
                }
                break;
            }
        }
        
    }//f
    //SCI_TRACE_LOW:"MMIBT_OpenedHeadsetIsExist exist %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9077_112_2_18_2_8_18_315,(uint8*)"d",exist);
    return exist;
}

/*****************************************************************************/
//  Discription:check headset all service is close
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandsetGatewayIsClosed(BT_ADDRESS * in_addr, BT_ADDRESS *out_addr)
{
    BOOLEAN ret = TRUE;
    BOOLEAN is_active = FALSE;
    uint32      service_type = 0;
    BT_DEVICE_INFO dev_info = {0};
    BT_STATUS    status = 0;
    uint32      device_num = 0;
    uint32       i = 0;
    
    if(PNULL != in_addr)
    {
        //only check in_addr is connected headset or not
        //get current earphone status, if is active do not allow to delete device
        service_type = BT_GetPairedDeviceService(in_addr);

        if ((BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY) & service_type)
        {
            is_active = MMIBT_IsDeviceActivedHandsetGateway(in_addr);//MMIBT_GetCurrentEarphoneStatus(in_addr);
        }
        if (BT_SERVICE_A2DP_SRC & service_type)
        {
//            is_active |= MMIBT_GetCurrentA2dpStatus(in_addr);
        }
        if (BT_SERVICE_A2DP_SRC & service_type)
        {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
        }
        if (BT_SERVICE_PBAP_SERVER & service_type)
        {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
        }
        if (BT_SERVICE_MAP_SERVER & service_type)
        {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
        }
        if (BT_SERVICE_AVRCP_TG & service_type)
        {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
        }
        if(is_active)
        {
            if(PNULL != out_addr)
            {
                SCI_MEMCPY(out_addr, in_addr, sizeof(BT_ADDRESS));
            }
        }
    }
    else
    {
        //check all paired device to find exist Connected Headset
        device_num = BT_GetPairedDeviceCount(BT_SERVICE_HANDFREE_GATEWAY);   
        for(i = 1; i <= device_num; i++)
        {
            status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDFREE_GATEWAY, i, &dev_info);
            if(BT_SUCCESS == status)
            {
                service_type = BT_GetPairedDeviceService(&dev_info.addr);
                if (BT_SERVICE_HANDFREE_GATEWAY & service_type)
                {
                    is_active = MMIBT_IsDeviceActivedEarphone(&dev_info.addr);//MMIBT_GetCurrentEarphoneStatus(&dev_info.addr);
                }
                if (BT_SERVICE_A2DP_SRC & service_type)
                {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
                if (BT_SERVICE_PBAP_SERVER & service_type)
                {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
                if (BT_SERVICE_MAP_SERVER & service_type)
                {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
                if (BT_SERVICE_AVRCP_TG & service_type)
                {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
                if(is_active)
                {
                    if(PNULL != out_addr)
                    {
                        SCI_MEMCPY(out_addr, &dev_info.addr, sizeof(BT_ADDRESS));
                    }                   
                    break;
                }
            }//if(BT_SUCCESS == status)
        }//for(i = 1; i <= device_num; i++)
    }
    ret = !is_active;
    //SCI_TRACE_LOW:"HeadsetIsClosed ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9149_112_2_18_2_8_19_316,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//  Description : handle headset close result
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void handleHandsetGatewayCloseResult(BT_STATUS bt_status, BT_ADDRESS *headset_addr_ptr)
{
    //wchar wait_text_buf[MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH] = {0};
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    BT_ADDRESS   connect_addr = {0};
    MMIBT_OPERATION_INFO_T bt_opertor = {0};
    
    bt_opertor = MMIBT_GetOpertor();
    
    //wait_text.wstr_ptr = wait_text_buf;    
    if((BT_SUCCESS == bt_status) || (BT_NO_CONNECTION == bt_status))
    {
        //close finished
        if(MMIBT_OPC_OPEN_HANDSET_GATEWAY != bt_opertor.current_opertion)
        {
            //xingdong_bt
            //no HFG and A2DP need to active, show "Already Closed"
            if(HandsetGatewayIsClosed(PNULL, PNULL))
            {
                MMIDEFAULT_SetBackLight(TRUE);
//                MMI_GetLabelTextByLang(TXT_BT_HEADSET_CLOSED, &wait_text);
                
                //ConnectUSC2String(TXT_NULL, g_current_bt.name, TXT_BT_HEADSET_ALREADY_CLOSED, MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH, &wait_text);
                MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                MMIBT_GetBtStatusForIcon();
                MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
            }
            //SetBackgroupRun(FALSE);
        }
        else
        { 
            //current operation not close headset, need continue to do current_opertion
            //xingdong_bt
            //SetBackgroupRun(FALSE);
            DoOperation(bt_opertor.current_opertion, FALSE);
        }
    }
    else if (BT_PENDING == bt_status)
    { //Closing...
        //xingdong_bt
        
        if(HandsetGatewayIsClosed(PNULL, &connect_addr) || (!memcmp(headset_addr_ptr, &connect_addr, sizeof(BT_ADDRESS))))
        {
            if(!MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))// && (!GetIsBackgroupRun()))
            {
                if(MMIBT_OPC_OPEN_HANDSET_GATEWAY == bt_opertor.current_opertion)
                {
                    MMI_GetLabelTextByLang(TXT_BT_ACTIVATING, &wait_text);
                }
                else
                {
                    //Dective
                    MMI_GetLabelTextByLang(TXT_BT_DISCONNECTING, &wait_text);
                }
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_EARCONN_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);           
            }
        }
    }
    else
    {
        //xingdong_bt
        //close error
        //if(!GetIsBackgroupRun())
        {
            MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_CANNOT_FIND_DEVICE,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        }
        //SetBackgroupRun(FALSE);
    } 
}

/*****************************************************************************/
//  Description : open headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void OpenHandsetGateway(const BT_ADDRESS *src_addr)
{
    uint32 dev_service = 0;
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    MMI_WIN_ID_T connect_alert_id = MMI_BT_CONNECT_CNF_WIN_ID;
    BT_ADDRESS   dev_addr = {0};

    //open headset
    //SCI_TRACE_LOW:"OpenHeadset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10737_112_2_18_2_8_22_343,(uint8*)"");

//#ifdef MMI_BT_PBAP_SUPPORT
//     if(MMIBT_IsConnectDevice(&dev_addr) && (memcmp(src_addr, &dev_addr, sizeof(BT_ADDRESS))))
//     {
//         MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
//         return;
//     }
//#endif

    if(!HeadsetIsClosed(PNULL, &dev_addr))
    {
        //exist not closed service, close it first
        BT_STATUS return_value = CloseHeadset(&dev_addr);
        handleHandsetGatewayCloseResult(return_value, &dev_addr);
        return;
    }

#ifndef WIN32
#ifdef BT_HFU_SUPPORT
     if(BT_HfuGetConnStatus((BT_ADDRESS *)src_addr))
#endif
     {
         ConnectHandsetGatewayService();
         return;
     }
#endif

#ifdef BT_HFU_SUPPORT
    dev_service = BT_GetPairedDeviceService(src_addr);


    MMIDEFAULT_SetBackLight(TRUE);
    MMK_CloseWin(alert_id);
    MMI_GetLabelTextByLang(TXT_BT_HEADSET_OPENED, &wait_text);
    MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
        &connect_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleConnectCnfAlertWin);
    MMIBT_GetBtStatusForIcon();

    MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    
    MMIBT_ConnectDevice((BT_ADDRESS *)src_addr);
#endif
}

/*****************************************************************************/
//  Description : close headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BT_STATUS CloseHandsetGateway(const BT_ADDRESS *src_addr)
{
    uint32 dev_service = BT_GetPairedDeviceService(src_addr);
    BT_STATUS  return_value = BT_ERROR;
    int8       retry_count = 0;
    
    //SCI_TRACE_LOW:"CloseHeadset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10694_112_2_18_2_8_22_342,(uint8*)"");

#ifndef WIN32
#if defined(BT_PBAP_SUPPORT)
    if((BT_SERVICE_PBAP_SERVER & dev_service) && (TRUE == BT_PbapIsConnected()))
    {
        //HFG need to close
        do 
        {
            return_value =  BT_PbapDisconnect();
            if((BT_SUCCESS == return_value) || (BT_NO_CONNECTION == return_value))
            {
                break;
            }
            SCI_Sleep(100);
            retry_count++;
            if((retry_count > MMIBT_DISCONNECT_RETRY_COUNT) || (FALSE == BT_PbapIsConnected()))
            {
                break;
            }
            SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting PBAP return_value = 0x%d", return_value);
        } while(BT_PENDING != return_value);
    }
#endif
    
#if defined(BT_MAP_SUPPORT)
    if((BT_SERVICE_MAP_SERVER & dev_service) && (TRUE == BT_MapIsConnected()))
    {
        retry_count = 0;
        //HFG need to close
        do 
        {
            return_value =  BT_MapDisconnect();
            if((BT_SUCCESS == return_value) || (BT_NO_CONNECTION == return_value))
            {
                break;
            }
            SCI_Sleep(100);
            retry_count++;
            if((retry_count > MMIBT_DISCONNECT_RETRY_COUNT) || (FALSE == BT_MapIsConnected()))
            {
                break;
            }
            SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting MAP return_value = 0x%d", return_value);
        } while(BT_PENDING != return_value);
    }
#endif

#if defined(BT_AV_SNK_SUPPORT)
    if((BT_SERVICE_AVRCP_TG & dev_service) && (TRUE == MMIAPIBT_IsAvrcpCtConnected()))
    {
        retry_count = 0;
        //HFG need to close
        do 
        {
            return_value =  AVRCP_CT_Disconnect();
            if((BT_SUCCESS == return_value) || (BT_NO_CONNECTION == return_value))
            {
                break;
            }
            SCI_Sleep(100);
            retry_count++;
            if((retry_count > MMIBT_DISCONNECT_RETRY_COUNT) || (FALSE == MMIAPIBT_IsAvrcpCtConnected()))
            {
                break;
            }
            SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting AVRCP CT return_value = 0x%d", return_value);
        } while(BT_PENDING != return_value);
    }
#endif

#endif
    
#ifdef BT_HFU_SUPPORT
    if((BT_SERVICE_HANDFREE_GATEWAY |BT_SERVICE_HEADSET_GATEWAY) & dev_service && MMIBT_IsDeviceActivedHandsetGateway(src_addr))
    {
        //HFG need to close
        SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting HFu return_value = 0x%d", return_value);
#ifndef WIN32
        return_value =  BT_HfuDisconnect();
#endif
    }
    else
    {
        return_value = BT_SUCCESS;
    }
#endif

#ifndef WIN32
#if defined(BT_AV_SNK_SUPPORT)
    if((BT_SERVICE_A2DP_SRC & dev_service) && (TRUE == MMIAPIBT_IsA2dpSinkConnected()))
    {
        retry_count = 0;
        //HFG need to close
        do 
        {
            return_value =  A2DP_Sink_Disconnect();
            if((BT_SUCCESS == return_value) || (BT_NO_CONNECTION == return_value))
            {
                break;
            }
            SCI_Sleep(100);
            retry_count++;
            if((retry_count > MMIBT_DISCONNECT_RETRY_COUNT) || (FALSE == MMIAPIBT_IsA2dpSinkConnected()))
            {
                break;
            }
            SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting A2DP SINK return_value = 0x%d", return_value);
        } while(BT_PENDING != return_value);
    }
#endif
#endif
    
    return return_value;    
}

/*****************************************************************************/
//  Description : ConnectHandsetGatewayService
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ConnectHandsetGatewayService(void)
{
//     uint16  msg_id = MMI_BT_HFG_SERVICE_CONNECT;
//     DPARAM  param = SCI_ALLOC_APP(10);
// 
//     SCI_TRACE_LOW("[MMIBT] ConnectHandsetGatewayService send signal MMI_BT_HFG_SERVICE_CONNECT");
//     MMIBT_SendSignal(msg_id, param);
    MMIBT_ConnectHandsetGatewayService(1);
}

/*****************************************************************************/
//  Description : MMIBT_ConnectHandsetGatewayProfile
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ConnectHandsetGatewayService(int openTimeoutFlag)
{
#ifndef WIN32
    uint32       dev_service = 0;
    BT_ADDRESS   dev_addr = {0};

    MMIAPIBT_GetHfuConnectedAddress(&dev_addr);
    SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService entry");
#ifdef BT_HFU_SUPPORT
    if(BT_HfuGetConnStatus(&dev_addr))
#endif
    {
        dev_service = BT_GetPairedDeviceService(&dev_addr);

        SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService dev_service = 0x%x", dev_service);

        if((BT_SERVICE_PBAP_SERVER & dev_service) && (FALSE == MMIAPIBT_PbapIsConnected()))
        {
            SCI_Sleep(100);
//            SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService connecting to PBAP");
            MMIAPIBT_DirectConnectPbapService(openTimeoutFlag);
        }
#ifdef BT_MAP_SUPPORT
        else if((BT_SERVICE_MAP_SERVER & dev_service) && (FALSE == MMIAPIBT_MapIsConnected()))
        {
            SCI_Sleep(100);
            SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService connecting to MAP");
            MMIAPIBT_DirectConnectMapService();
        }
#endif
#ifdef BT_AV_SNK_SUPPORT
        else if((BT_SERVICE_A2DP_SRC & dev_service) && (FALSE == MMIAPIBT_IsA2dpSinkConnected()))
        {
            SCI_Sleep(100);
            SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService connecting to A2DP Sink");
            MMIAPIBT_A2dpSinkConnect(dev_addr);
        }
#endif
    }
#endif
}

/*****************************************************************************/
//  Description : Auto connect to handset gateway if authorization
//  Global resource dependence : none
//  Author: Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL void AutoConnectHandsetGateway(uint8 timer_id, uint32 param)
{
    BOOLEAN     be_authorize = FALSE;
    BT_ADDRESS  dev_addr = {0};
    
    if(MMIBT_HasConnected(&dev_addr))
    {
        BT_GetDeviceAuthorisation(&dev_addr, &be_authorize);
        
        if(be_authorize)
        {
            MMIAPIBT_ConnectDevice();
        }
    }
}

/*****************************************************************************/
//  Description : StopAutoConnectHandsetGatewayTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StopAutoConnectHandsetGatewayTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] StopAutoConnectHandsetGatewayTimer enter");
    
    if(s_bt_hfu_auto_connect_timer != 0)    
    {   
        MMK_StopTimer(s_bt_hfu_auto_connect_timer);   
        s_bt_hfu_auto_connect_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIBT] StopAutoConnectHandsetGatewayTimer exit, timer_id = %d", s_bt_hfu_auto_connect_timer);
}

/*****************************************************************************/
//  Description : StartAutoConnectHandsetGatewayTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StartAutoConnectHandsetGatewayTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] StartAutoConnectHandsetGatewayTimer enter");
    
    if(s_bt_hfu_auto_connect_timer != 0) 
    {
        StopAutoConnectHandsetGatewayTimer();
    }
    
    s_bt_hfu_auto_connect_timer = MMK_CreateTimerCallback(BT_HFU_AUTO_CONNECT_TIMER_OUT, AutoConnectHandsetGateway, 0, FALSE);
    
    SCI_TRACE_LOW("[MMIBT] StartAutoConnectHandsetGatewayTimer exit, timer_id = %d", s_bt_hfu_auto_connect_timer);
}

/*****************************************************************************/
//  Description: Is open BT Remote Alert WIN  
//  Global resource dependence: none 
//  Author: Cheney Wu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsCloseingHeadsetGateway(void)
{

    MMIBT_OPERATION_INFO_T  bt_opc = MMIBT_GetOpertor();
    if(MMIBT_OPC_CLOSE_HANDSET_GATEWAY == bt_opc.current_opertion)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : receive msg: MMIBT_GetHfuConnectedAddress
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetHfuConnectedAddress(BT_ADDRESS *addr)
{
    uint16	device_number =0;
    BT_DEVICE_INFO dev = {0};
    uint16      index =0;
    BOOLEAN connected = FALSE;
    uint32 dev_service = 0;
    
    device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);

    SCI_TRACE_LOW("[MMIBT] PBAP Debug:, device_number: %d", device_number);
    
    for (index = 1; index <= device_number; index++)
    {
        BT_GetPairedDeviceInfo(BT_SERVICE_PBAP_SERVER, index, &dev);
        SCI_TRACE_LOW("[MMIBT] PBAP Debug:, paired address: %02X-%02X-%02X-%02X-%02X-%02X", dev.addr.addr[5], dev.addr.addr[4], dev.addr.addr[3], dev.addr.addr[2], dev.addr.addr[1], dev.addr.addr[0]);
        
        dev_service = BT_GetPairedDeviceService(&dev.addr);

// #ifndef WIN32
//         connected = BT_HfuGetConnStatus(&dev.addr);
//         SCI_TRACE_LOW("BT_AV, is connected: %d", connected);
// #endif
        if(BT_SERVICE_PBAP_SERVER & dev_service)
        {
            SCI_TRACE_LOW("MMIBT_GetHfuConnectedAddress, connected address: %02X-%02X-%02X-%02X-%02X-%02X", dev.addr.addr[5], dev.addr.addr[4], dev.addr.addr[3], dev.addr.addr[2], dev.addr.addr[1], dev.addr.addr[0]);
            memcpy(addr, &dev.addr, sizeof(BT_ADDRESS));
            MMINV_WRITE(MMINV_BT_ADDRESS_INFO, addr);

            return TRUE;
        }
    }
    
    for (index = 0; index < BT_ADDRESS_SIZE; index++)
    {
        dev.addr.addr[index] = 0;
        memcpy(addr, &dev.addr, sizeof(BT_ADDRESS));
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : Create a transport connection to the specified device 
//                and issue an OBEX Connect Request
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapConnect(const BT_ADDRESS *addr)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;

    ret_val = BT_PbapConnect(addr);
    SCI_TRACE_LOW("[MMIBT_PbapConnect] ret_val = 0x%x", ret_val);
    
    return ret_val;
#else
	return BT_SUCCESS;
#endif
}

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIBT_PbapDisconnect(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapDisconnect();

	SCI_TRACE_LOW("[MMIBT_PbapDisconnect] ret_val = 0x%x", ret_val);
	if(BT_SUCCESS == ret_val)
	{
		MMIBT_PbapClientDeregister();
	}
	else if((BT_PENDING != ret_val) && (BT_BUSY != ret_val))
	{
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
	}
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : to initiate a transport disconnection. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note: BT_PbapDisconnect will cause the transport to be brought down automatically, 
//  but if the transport must be brought down immediately, perhaps due an error condition, 
//  this routine may be used. 
/*****************************************************************************/
PUBLIC void MMIBT_PbapClientTpDisconnect(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapClientTpDisconnect();

    SCI_TRACE_LOW("[MMIBT_PbapClientTpDisconnect] ret_val = 0x%x", ret_val);
    if(BT_SUCCESS == ret_val)
    {
        MMIBT_PbapDisconnect();
    }
    else if(BT_PENDING != ret_val)
    {
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : Get vcard listing 
//                from the remote Phonebook Access Server. 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapPullVcardListing(void)
{
#if 0
#ifndef WIN32
    BT_PBAP_VCARD_FORMAT_T  format = MMIBT_PBAP_VCARD_FORMAT_21;//BT_VCARD_FORMAT_30;//
    uint16                  max_list_count = MMIPB_NV_MAX_RECORD_NUM;
    uint16                  list_offset = 0;

    return BT_PbapPullVcardListing(
                                   s_pb_name,
                                   BT_PBAP_VCARD_SEARCH_ATTRIB_NAME,
                                   PNULL,
                                   BT_PBAP_VCARD_SORT_ORDER_INDEXED,
                                   0xFFFF,
                                   0
                                   );

#endif
#endif
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Description : Initiates the OBEX "Get" operation to retrieve a phonebook object 
//                from the remote Phonebook Access Server. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapPullPhonebook(void)
{
#ifndef WIN32
    BT_PBAP_VCARD_FILTER_T  pbap_filter = {0};
    BT_PBAP_VCARD_FORMAT_T  format = MMIBT_PBAP_VCARD_FORMAT_21;//BT_VCARD_FORMAT_30;//
    uint16                  max_list_count = MMIPB_NV_MAX_RECORD_NUM;
    uint16                  list_offset = 0;
    
//#ifdef MMI_CONTACT_PHOTO_SUPPORT
    //SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x8d\x00\x00\x00\x00\x00\x00\x00", BT_PBAP_FILTER_SIZE*sizeof(uint8));
//#else
    SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x85\x00\x00\x00\x00\x00\x00\x00", BT_PBAP_FILTER_SIZE*sizeof(uint8));
//#endif
    return BT_PbapPullPhonebook(
                        s_pb_name,
                        &pbap_filter,
                        format,
                        max_list_count,
                        list_offset
                        );
#else
	return BT_PENDING;
#endif
}

/*****************************************************************************/
//  Description : Initiates the OBEX "Get" operation to retrieve a phonebook object 
//                from the remote Phonebook Access Server. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapPullSIMPhonebook(void)
{
#ifndef WIN32
    BT_PBAP_VCARD_FILTER_T  pbap_filter = {0};
    BT_PBAP_VCARD_FORMAT_T  format = MMIBT_PBAP_VCARD_FORMAT_21;//BT_VCARD_FORMAT_30;//
    uint16                  max_list_count = MMIPB_NV_MAX_RECORD_NUM;
    uint16                  list_offset = 0;
//#ifdef MMI_CONTACT_PHOTO_SUPPORT
    //SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x8d\x00\x00\x00\x00\x00\x00\x00", BT_PBAP_FILTER_SIZE*sizeof(uint8));
//#else
    SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x85\x00\x00\x00\x00\x00\x00\x00", BT_PBAP_FILTER_SIZE*sizeof(uint8));
//#endif
    return BT_PbapPullPhonebook(
                        s_sim_name,
                        &pbap_filter,
                        format,
                        max_list_count,
                        list_offset
                        );
#else
	return BT_PENDING;
#endif
}

/*****************************************************************************/
//  Description : Registers the Phonebook Access Client
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapClientRegister(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;

    ret_val = BT_PbapRegisterClient();
    SCI_TRACE_LOW("[MMIBT_PbapClientRegister] ret_val = 0x%x", ret_val);
    
    return ret_val;
#else
	return BT_SUCCESS;
#endif
}

/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIBT_PbapClientDeregister(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapDeregisterClient();
    
    SCI_TRACE_LOW("[MMIBT_PbapClientDeregister] ret_val = 0x%x", ret_val);
    if(BT_BUSY == ret_val)
    {
    }
    else if((BT_SUCCESS != ret_val) &&(BT_PENDING != ret_val))
    {
//        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
    
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : Aborts the current client operation. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note: The completion event will signal the status of the operation in progress, 
//  either COMPLETE or ABORTED. 
/*****************************************************************************/
PUBLIC void MMIBT_PbapAbortClient(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapAbortClient();

    SCI_TRACE_LOW("[MMIBT_PbapAbortClient] ret_val = 0x%x", ret_val);
    if(BT_SUCCESS == ret_val)
    {
        MMIBT_PbapDisconnect();
    }
    else
    {
        MMIPUB_OpenAlertFailWin(STR_CONTACT_SYNC_INTERRUPT);
    }
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapDataInd(BT_MSG_T* msg_body_ptr)
{
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapDataInd] PNULL == msg_body_ptr!");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT_PbapDataInd] s_pbap_connect_type = %d", s_pbap_connect_type);
    SCI_TRACE_LOW("[MMIBT_PbapDataInd] s_is_cancel_pbap_pull = %d", s_is_cancel_pbap_pull);
    if(s_is_cancel_pbap_pull)
    {
        return;
    }
    
    switch(s_pbap_connect_type)
    {
    case BT_PBAP_TYPE_PB:
    case BT_PBAP_TYPE_SIM:
        MMIBT_AddVcardToContact(msg_body_ptr);
        break;
//     case BT_PBAP_TYPE_CL:
//         MMIBT_AddVcardToCalllog(msg_body_ptr);
//         break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapParamsRx(BT_MSG_T* msg_body_ptr)
{
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapParamsRx] PNULL == msg_body_ptr!");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT_PbapParamsRx] The Application Parameters header information has been fully parsed!");
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapAbort(BT_MSG_T* msg_body_ptr)
{
    BT_PBAP_OP_T        oper_type = BT_PBAPOP_NONE;
    BT_PBAP_RESP_CODE_T resp_code = BT_PBRC_STATUS_SUCCESS;
    
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapAbort] PNULL == msg_body_ptr!");
        return;
    }
    
    oper_type = (BT_PBAP_OP_T)(((BT_PBAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->oper_type);
    resp_code = (BT_PBAP_RESP_CODE_T)(((BT_PBAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->resp_code);
    
    SCI_TRACE_LOW("[MMIBT_PbapAbort] The operation is 0x%x, fail reason is 0x%x.", oper_type, resp_code);
    if(BT_PBAPOP_PULL_PHONEBOOK == oper_type)
    {
        if(BT_PBRC_USER_ABORT ==resp_code)
        {
// #ifndef BT_DIALER_DIRECT_CONNECT
            MMIBT_PbapDisconnect();
// #endif
        }
        else if(BT_PBRC_LINK_DISCONNECT !=resp_code) 
        {
            if(BT_PBAP_TYPE_SIM == MMIBT_GetPbapRequestType())
            {
                 MMIAPIPB_RefreshList();
            }
            else
            {
                MMIPUB_OpenAlertFailWin(STR_CONTACT_SYNC_INTERRUPT);
            }
// #ifndef BT_DIALER_DIRECT_CONNECT
            MMIBT_PbapDisconnect();
// #endif
        }
        
        s_is_cancel_pbap_pull = FALSE;

        SCI_TRACE_LOW("[MMIBT_PbapAbort] MMIBT_GetPbapRequestType() = %d", MMIBT_GetPbapRequestType());
        MMIPB_FreeAllPbapData();

        MMIBT_SetPbapRequestType(BT_PBAP_TYPE_NONE);
        if(MMK_IsOpenWin(MMIBT_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_WAIT_WIN_ID);
            MMIPUB_OpenAlertSuccessWin(STXT_CANCEL);
        }
        //需要确认complete情况
        
        if(MMK_IsOpenWin(MMIBT_SYNC_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapComplete(BT_MSG_T* msg_body_ptr)
{
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapComplete] PNULL == msg_body_ptr!");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT_PbapComplete] The operation is 0x%x.", *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)));
    
    if(BT_PBAP_TYPE_CL == MMIBT_GetPbapRequestType())
    {
#if 0
        if(BT_PBAPOP_CONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS to Calllog");
            MMK_SendMsg(MMIAPICL_GetActivateBTWinId(), MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
        }
        else if(BT_PBAPOP_PULL_PHONEBOOK == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("[MMIBT_PbapComplete] MMICL_NotifyPBStartParser");
#if defined(TRACE_MAP_BUFFER)
            DEBUG_MakeList("CALLLOG");
#endif
            
            MMICL_NotifyPBStartParser((MMICL_PBAP_PARSER_CALLBACK_FUNC)MMIBT_PBPbapParserCallback);
        }
        else if(BT_PBAPOP_DISCONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("[MMIBT_PbapComplete] MMICL_PbapClientDeregister");
            MMICL_PbapClientDeregister();
        }
#endif
    }
    else
    {
        if(BT_PBAPOP_CONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS to BT UI");
            
            if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
            {
                SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS 2");
                MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
                MMIAPIPB_InitPBAPDataList();
                if(MMK_IsOpenWin(MMIPB_ENTRY_LIST_FOR_EX_WIN_ID))
                {
                    SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS 3");
                    MMK_SendMsg(MMIPB_ENTRY_LIST_FOR_EX_WIN_ID, MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
                }
                if(MMK_IsOpenWin(MMIPB_ENTRY_LIST_WITH_SEARCH_WIN_ID))
                {
                    SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS 4");
                    MMK_SendMsg(MMIPB_ENTRY_LIST_WITH_SEARCH_WIN_ID, MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
                }
            }
            MMIAPIPB_SetAllowSync(TRUE);
            
            SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS 5");
            if(0 != MMIAPIPB_GetActivateBTWinId())
            {
                SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS 6");
                MMK_SendMsg(MMIAPIPB_GetActivateBTWinId(), MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
                //                 MMIAPIPB_RefreshList();
            }  
            
            MMIBT_StopPbapSyncTimer();
            
            SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS 7");
            if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
            {
                SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS 8");
                MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
            }
            
            /* Connect to other service */
            //ConnectHandsetGatewayService();
        }
        else if(BT_PBAPOP_PULL_PHONEBOOK == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            MMIAPIPB_NotifyPBStartParser(MMIBT_PBPbapParserCallback);
        }
        else if(BT_PBAPOP_DISCONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
#ifdef BT_DIALER_ORIG_SUPPORT
            MMIBT_PbapClientDeregister();
#else
            MMIAPIPB_PbapClientDeregister();
#endif
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapTpDisconnect(BT_MSG_T* msg_body_ptr)
{
    BT_PBAP_OP_T        oper_type = BT_PBAPOP_NONE;
#if MTOBEX_STACK_VERSION <= 380    /*lint !e553*/
    BT_PBAP_RESP_CODE_T resp_code = BT_PBRC_STATUS_SUCCESS;
#endif
    SCI_TRACE_LOW("MMIBT_PbapTpDisconnect");
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] PNULL == msg_body_ptr!");
        return;
    }
    
    oper_type = (BT_PBAP_OP_T)(((BT_PBAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->oper_type);
    SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] The operation is 0x%x", oper_type);
#if MTOBEX_STACK_VERSION <= 380    /*lint !e553*/
    resp_code = (BT_PBAP_RESP_CODE_T)(((BT_PBAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->resp_code);
    SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] The response is 0x%x", resp_code);
#endif

    SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] BT_PbapDeregisterServer oper_type:%d",oper_type);

#ifdef BT_PBAP_SERVER_SUPPORT
     BT_PbapDeregisterServer();
#endif

    if(BT_PBAPOP_NONE == oper_type)
    {
        MMIBT_StopPbapSyncTimer();
        MMIAPIPB_StopPBAPTimer();
//        MMIAPICL_StopPbapTimer();
        if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
        {
        MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
        }
        if(MMK_IsOpenWin(MMIBT_SYNC_WAIT_WIN_ID))
        {
        MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
        }
//         MMICL_ClosePBAPWaitWin();
        SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] MMIBT_GetPbapRequestType:%d",MMIBT_GetPbapRequestType());
//         if(BT_PBAP_TYPE_NONE == MMIBT_GetPbapRequestType())
//         {
//             MMIPUB_OpenAlertWarningWin(TXT_BT_PBAP_CONNECT_FAIL);
//         }
        
        s_is_cancel_pbap_pull = FALSE;
        MMIBT_SetPbapRequestType(BT_PBAP_TYPE_NONE);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_PBPbapParserCallback(MMIPB_PBAP_PARSER_RESULT_T *result)
{
    switch(result->ret)
    {
    case MMIPB_PBAP_PARSER_FINISHED:
        switch(s_pbap_connect_type)
        {
        case BT_PBAP_TYPE_PB:
            SCI_TRACE_LOW("[MMIBT_PBPbapParserCallback] BT_PBAP_TYPE_PB");
            // Get SIM data again
            MMIBT_PbapPullSIMPhonebook();
            
            break;
            
        case BT_PBAP_TYPE_SIM:
            SCI_TRACE_LOW("[MMIBT_PBPbapParserCallback] BT_PBAP_TYPE_SIM");
            MMIPUB_OpenAlertSuccessWin(STR_CONTACT_SYNC_COMPLETE);
            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
            // #ifndef BT_DIALER_DIRECT_CONNECT
            //             MMIBT_PbapDisconnect();
            // #endif
            MMIPB_FreeAllPbapData();
            MMIAPIPB_RefreshList();
            break;
            
#if 0
        case BT_PBAP_TYPE_CL:
            // 	            MMICL_ClosePBAPWaitWin();
            //            MMIPUB_OpenAlertSuccessWin(TXT_BT_SYNC_FINISHED);
            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
            // #ifndef BT_DIALER_DIRECT_CONNECT
            //             MMIBT_PbapDisconnect();
            // #endif
            break;
#endif
            
        default:
            // 	            MMICL_ClosePBAPWaitWin();
            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
            MMIAPIPB_RefreshList();
            // #ifndef BT_DIALER_DIRECT_CONNECT
            //             MMIBT_PbapDisconnect();
            // #endif
            break;
        }
        
        if(MMK_IsOpenWin(MMIBT_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_WAIT_WIN_ID);
        }
        
        if(BT_PBAP_TYPE_PB == s_pbap_connect_type)
        {
            MMIBT_SetPbapRequestType(BT_PBAP_TYPE_SIM);
        }
        s_is_cancel_pbap_pull = FALSE;
        break;
        
    case MMIPB_PBAP_PARSER_NO_SPACE:
        {
            MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
            MMI_STRING_T    display_str = {0};
            wchar           temp_wchar[60] = {0};
            
            SCI_TRACE_LOW("[MMIBT] MMIBT_PBPbapParserCallback MMIPB_PBAP_PARSER_NO_SPACE");
            SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact sync_count:%d", result->sync_count);
            
            /************************************************************************/
            /* 拼接提示字符: 已同步XXX个联系人，空间已满                            */
            /************************************************************************/ 
            display_str.wstr_ptr = temp_wchar;
            
            MMIBT_GenerateDisplayInfo(display_str.wstr_ptr,result->sync_count);
            display_str.wstr_len = MMIAPICOM_Wstrlen(display_str.wstr_ptr);
            
            /*MMIPUB_OpenAlertTextWinByTextPtr(PNULL,
            &display_str,
            PNULL,
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL);*/
            
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&display_str,PNULL,IMAGE_PUBWIN_WARNING,
                &alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
            
            //                MMICL_ClosePBAPWaitWin();
            //MMIBT_PbapAbortClient();
            MMIPB_FreeAllPbapData();
            MMIAPIPB_RefreshList();
            
            s_is_cancel_pbap_pull = TRUE;
        }
        break;
        
    case MMIPB_PBAP_PARSER_CANCELED:
        MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
        //            MMICL_ClosePBAPWaitWin();
        //MMIBT_PbapAbortClient();
        MMIPB_FreeAllPbapData();
        MMIAPIPB_RefreshList();
        
        s_is_cancel_pbap_pull = TRUE;
        SCI_TRACE_LOW("[MMIBT] HandlePubwinWaitingMsg sync has been canceled");
        break;
    }
}

/*******************************************************************************/
//  Description : generate whole display string
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_GenerateDisplayInfo(wchar *info_ptr,uint32 sync_contact_count)
{
    MMI_STRING_T        text_str1                       = {0};
    MMI_STRING_T        text_str2                       = {0};
    char                count_str[5] = {0};
    uint32              count_len = 0;
    wchar               count_wstr[5] = {0};
	uint32              content_len                     = 0; 

    //left string    
    MMI_GetLabelTextByLang(STR_CONTACT_SYNC_COMPLETE, &text_str1); 
    if(text_str1.wstr_ptr != PNULL)
    {
        SCI_MEMCPY(info_ptr, text_str1.wstr_ptr, text_str1.wstr_len * sizeof(wchar));
    }
    
#if 0
    //middle string
    MMIAPICOM_Wstrcat(info_ptr, L" ");
    sprintf((char *)count_str, "%d", sync_contact_count);
    count_len = strlen((char *)count_str);
    MMIAPICOM_StrToWstr((uint8 *)count_str, count_wstr);
    MMIAPICOM_Wstrncat(info_ptr, count_wstr, MMIAPICOM_Wstrlen(count_wstr));
    MMIAPICOM_Wstrcat(info_ptr, L" ");
    
    //right string
    MMI_GetLabelTextByLang(TXT_BT_SYNC_TIP, &text_str2); 
    MMIAPICOM_Wstrncat(info_ptr, text_str2.wstr_ptr, text_str2.wstr_len * sizeof(wchar));
#endif
    content_len = MMIAPICOM_Wstrlen(info_ptr);    
    
    SCI_TRACE_LOW("MMIBT_GenerateDisplayInfo info len %d",content_len);
}

#ifdef WIN32
/*******************************************************************************/
//  Description :MMIBT_SimuNotifyCLStartParse
//  Parameter: void
//  Global resource dependence :                                
//  Author: Cheney.Wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_SimuNotifyCLStartParse(void)
{
#if 0
    MMICL_NotifyPBStartParser((MMICL_PBAP_PARSER_CALLBACK_FUNC)MMIBT_PBPbapParserCallback);
#endif
}

/*******************************************************************************/
//  Description :MMIBT_SimuNotifyPBStartParse
//  Parameter: void
//  Global resource dependence :                                
//  Author: Cheney.Wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_SimuNotifyPBStartParse(void)
{
     MMIAPIPB_NotifyPBStartParser(MMIBT_PBPbapParserCallback);
}
#endif

/*******************************************************************************/
//  Description : MMIBT_SetPbapRequestType
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_SetPbapRequestType(BT_PBAP_TYPE_E pbapType)
{
    s_pbap_connect_type = pbapType;
}

/*******************************************************************************/
//  Description : MMIBT_GetPbapRequestType
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney wu
//  Return : 
/*****************************************************************************/
PUBLIC BT_PBAP_TYPE_E MMIBT_GetPbapRequestType(void)
{
    return s_pbap_connect_type;
}

/*****************************************************************************/
//  Description : MMIBT_GetPbapCancelValue
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetPbapCancelValue(void)
{
    return s_is_cancel_pbap_pull;
}

/*****************************************************************************/
//  Description : MMIBT_SetPbapCancelValue
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetPbapCancelValue(BOOLEAN is_cancel)
{
    s_is_cancel_pbap_pull = is_cancel;
}

/*******************************************************************************/
//  Description :添加一条电话本记录
//  Parameter: msg_body_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			   vcard_size[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_AddVcardToContact(BT_MSG_T  *msg_body_ptr)
{
    if(PNULL == msg_body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact msg_body_ptr is null");
        return;
    }
    
    if (PNULL == msg_body_ptr->body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact msg_body_ptr->body_ptr is null");
        return;
    }
    
    if (BT_SUCCESS != msg_body_ptr->status)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact msg_body_ptr->status isnot success");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact oper_type:%d, len:%d, is_first_block:%d", 
        (BT_PBAP_OP_T)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->oper_type,
        (uint16)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len, 
        (BOOLEAN)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->is_first_block);
    
    if (BT_PBAPOP_PULL_PHONEBOOK != (BT_PBAP_OP_T)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->oper_type)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact oper_type Error");
        return;
    }
    
    /************************************************************************/
    /* 将待解析的数据构建成一个静态链表 	                                    */
    /************************************************************************/    
    
    MMIAPIPB_InsertPBAPDataToList((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff,
        (uint16)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len);
    
    if((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff != PNULL)
    {
#ifdef WIN32
        SCI_FREE((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff);
#else		
        OS_Pfree((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff); /*lint !e718 */
#endif
        (( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff = PNULL;
    }
	//SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact ret:%d, bt_status:d", ret, bt_status);
}

/*****************************************************************************/
//  Discription: sync confirm implementation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_DirectConnectPbapService(int openTimeoutFlag)
{
    BT_ADDRESS                  dev_addr = {0};
    BOOLEAN                     is_dev_connect = FALSE;
    BT_STATUS                   register_ret = BT_ERROR;
    BT_STATUS                   connect_ret = BT_ERROR;
    MMI_STRING_T                wait_text = {0};

    is_dev_connect = MMIBT_IsConnectDevice(&dev_addr);
   
    if(!is_dev_connect)
    {
//        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_TURN_ON,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
//        return;
    }

    /************************************************************************/
    /*Registers the Phonebook Access Client                                  */
    /************************************************************************/
    register_ret = MMIBT_PbapClientRegister();
    SCI_TRACE_LOW("[MMIBT] MMIAPIBT_DirectConnectPbapService register_ret:%d", register_ret);

    if((BT_SUCCESS != register_ret) && (BT_PENDING != register_ret))
    {
        //MMIBT_OpenAlertTipWin(TXT_BT_REGISTER_FAIL); 
        MMIBT_OpenAlertTipWin(STR_CONTACT_SYNC_INTERRUPT);
        return;
    }
    
    /************************************************************************/
    /* Create a transport connection to the specified device and issue an  
    OBEX Connect Request */
    /************************************************************************/
    if(!MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
    {
        MMI_GetLabelTextByLang(STR_CONTACT_SYNC_WAIT, &wait_text); 
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PBAP_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
             ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinDirectConnectWaitingMsg); 
        MMIPUB_SetWinSoftkey(MMIBT_PBAP_CONNECT_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
    }

    //lzk begin
    if (openTimeoutFlag == 1)
        MMIBT_CreatePbapSyncTimer();
    //lzk end

    connect_ret = MMIBT_PbapConnect(&dev_addr);
    
    SCI_TRACE_LOW("[MMIBT] MSG_BT_CONTACT_SYNC_CNF connect_ret:%d", connect_ret);

    if(BT_PENDING != connect_ret)
    {
        MMIBT_OpenAlertTipWin(STR_CONTACT_SYNC_INTERRUPT);                    
    }
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinDirectConnectWaitingMsg(
                                                       MMI_WIN_ID_T         win_id, 
                                                       MMI_MESSAGE_ID_E    msg_id, 
                                                       DPARAM              param
                                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIBT_StopPbapSyncTimer();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

#endif





