/***************************************************************************
** File Name:      mmisms_api.c                                            *
** Author:                                                                 *
** Date:           04/2006                                                   *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to define the API function.           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2006          Tracy Zhang       Modularize
****************************************************************************/
/**------------------------------------------------------------------------*
**                         Include Files                                   *
**-------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmiphone_export.h"
#include "mmi_textfun.h"
#include "mmipub.h"
#include "mmisms_export.h"
#include "mmisms_internal.h"
#include "mmisms_app.h"
#include "mmisms_id.h"
#include "mmisms_nv.h"
#include "mmisms_menutable.h"
//#include "mmisms_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmisms_delete.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmisms_send.h"
#include "mmi_appmsg.h"
#include "mmi_atc.h"
#include "mmiidle_export.h"
#include "mmisms_set.h"
#include "mmisms_read.h"
#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif
#include "mmiidle_statusbar.h"
#include "mmicom_statusarea.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmisms_timermsg.h"
#include "mmialarm_service.h"
#endif
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
#include "mmimail_export.h"
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmisms_edit.h"
#include "mmisms_save.h"

#include "mmisms_chatreadwin.h"
#endif

#include "mmiota_export.h"
#include "mmiidle_statusbar.h"
#include "mmimms_export.h"
#include "mmisms_image.h"
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#include "mmisms_messageboxwin.h"
#include "mmisms_pushmsgboxwin.h"
#include "mmisms_commonui.h"
#include "mmimms_id.h"
#include "mmisms_appcontrol.h"
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
#include "mmisms_voicemail.h"
#endif
#include "mmismsapp_interface.h"
#include "variant_config_nv.h"
/**------------------------------------------------------------------------*
**                         EXTERN DECLARE                                  *
**-------------------------------------------------------------------------*/
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
extern MMISMS_VMN_OPER_TYPE_E   g_vmn_oper_type;//查询语音信箱号是否为了通话转移
#endif

extern MMISMS_GLOBAL_T g_mmisms_global;            // SMS的全局变量
uint8 g_sms_back_play_enter_app = 0;
#ifndef MMI_SYNCTOOL_SUPPORT
#define SMS_SYNC_POS_ME				'0'
#define SMS_SYNC_POS_SIM_1	        '1'
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/***************************************************************************/
//     Description : init the SMS for ps
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/***************************************************************************/
void MMIAPISMS_InitForPS(void)
{
    MMISMS_InitForPS();
}

/***************************************************************************/
//     Description : init the SMS global
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/***************************************************************************/
void MMIAPISMS_AppInit(void)
{
    MMISMS_AppInit();
}


#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : get delete all flag
//                  TRUE:        do delete all SMS operation  
//                  FALSE:    without do delete all SMS operation
//    Global resource dependence : 
//  Author:jian.ma
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_GetDelAllFlag(void)
{
    return (MMISMS_GetDelAllFlag());
}
#endif /* Reduce Code size */


/*****************************************************************************/
//     Description : to enter the window for writing SMS 
//    Global resource dependence : 
//  Author:louis.wei
//    Note:多个联系人短信编辑
/*****************************************************************************/
PUBLIC void MMIAPISMS_SendGroupSms(
                               MN_DUAL_SYS_E   dual_sys,           //IN:
                               MMI_STRING_T    *sms_init_ptr,        //IN:
                               MMISMS_DEST_ADDR_INFO_T *dest_info
                               )
{
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();
    
    MMIAPISMS_ExitFromSMS();

    if (MMISMS_IsOrderOk()&& MMISMS_IsMMSOrderReady())
    {
        if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
        {           
            MMISMS_SEND_GROUP_INFO_T mmisms_sendgroup_info = {0};
            uint16 length = 0;
            mmisms_sendgroup_info.dest_info = SCI_ALLOCAZ(sizeof(MMISMS_DEST_ADDR_INFO_T));
            if(PNULL == mmisms_sendgroup_info.dest_info)
            {
                return;
            }    
               
            SCI_MEMCPY((void*)mmisms_sendgroup_info.dest_info, (void*)dest_info, sizeof(MMISMS_DEST_ADDR_INFO_T));
            if(sms_init_ptr != PNULL)
            {
                length = sms_init_ptr->wstr_len;
                mmisms_sendgroup_info.sms_str.wstr_ptr = SCI_ALLOCAZ( (length + 1) * sizeof(wchar) );
                if(PNULL == mmisms_sendgroup_info.sms_str.wstr_ptr)
                {
                    SCI_FREE(mmisms_sendgroup_info.dest_info);
                    return;
                } 
                SCI_MEMCPY((void*)mmisms_sendgroup_info.sms_str.wstr_ptr , (void*)sms_init_ptr->wstr_ptr,  (length + 1) * sizeof(wchar));
                mmisms_sendgroup_info.sms_str.wstr_len = length;
            }
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, MMISMS_OpenSendGroupWinExt,(DPARAM)&mmisms_sendgroup_info,sizeof(MMISMS_SEND_GROUP_INFO_T));
        }
        else
        {
            MMISMS_SendGroupSms(dual_sys,sms_init_ptr,dest_info);
        }
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        MMISMS_OpenInitWaitingWindow(MMISMS_WRITE_NEW_SMS);
    }
}


/*****************************************************************************/
//     Description : to enter the window for writing SMS 
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 单个联系人或无联系人短信编辑
/*****************************************************************************/
PUBLIC void MMIAPISMS_WriteNewMessage(
                               MN_DUAL_SYS_E    dual_sys,           //IN:
                               MMI_STRING_T        *sms_init_ptr,        //IN:
                               uint8            *dest_addr_ptr,        //IN:
                               uint8            dest_addr_len        //IN:
                               )
{
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();
    
    MMIAPISMS_ExitFromSMS();

    if (MMISMS_IsOrderOk()&& MMISMS_IsMMSOrderReady())
    {
        MMISMS_SetCurType(MMISMS_BOX_NONE); //Bug 264174 add box type 

        if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
        {           
            MMISMS_WRITE_MSG_INFO_T mmisms_write_info = {0};
            uint16 length = 0;
            mmisms_write_info.dest_addr_ptr = SCI_ALLOCAZ( dest_addr_len + 1 );
            if(PNULL == mmisms_write_info.dest_addr_ptr)
            {
                return;
            }    
               
            SCI_MEMCPY((void*)mmisms_write_info.dest_addr_ptr, (void*)dest_addr_ptr, dest_addr_len);
            mmisms_write_info.dest_addr_len = dest_addr_len;
			if(sms_init_ptr != PNULL)
			{
                length = sms_init_ptr->wstr_len;
                mmisms_write_info.sms_str.wstr_ptr = SCI_ALLOCAZ( (length + 1) * sizeof(wchar) );
                if(PNULL == mmisms_write_info.sms_str.wstr_ptr)
                {
                    SCI_FREE(mmisms_write_info.dest_addr_ptr);
                    return;
                } 
                SCI_MEMCPY((void*)mmisms_write_info.sms_str.wstr_ptr , (void*)sms_init_ptr->wstr_ptr,  (length + 1) * sizeof(wchar));
                mmisms_write_info.sms_str.wstr_len = length;
            }
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, MMISMS_OpenWriteWindowExt,(DPARAM)&mmisms_write_info,sizeof(MMISMS_WRITE_MSG_INFO_T));
        }
        else
        {
#ifdef MMI_SMS_MINI_SUPPORT
            MMISMS_ClearDestNum(); 
#endif
            MMISMS_WriteNewMessage(
                dual_sys,
                sms_init_ptr,
                FALSE,
                dest_addr_ptr,
                dest_addr_len
                );
        }
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        MMISMS_OpenInitWaitingWindow(MMISMS_WRITE_NEW_SMS);
    }
}

/*****************************************************************************/
//  Description :  set dest number for write new msg
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: interface for other module/pb
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetAddressToMessageEx(
                                               uint8            *dest_addr_ptr,        //IN:
                                               uint8            dest_addr_len        //IN:
                                               )
{
    
    if (PNULL == dest_addr_ptr || 0 == dest_addr_len)
    {
        return FALSE;
    }
    if (MMISMS_DEST_ADDR_MAX_NUM <= g_mmisms_global.edit_content.dest_info.dest_list.number)
    {
        return FALSE;
    }
    MMISMS_SetAddressToMessage(dest_addr_ptr, dest_addr_len, &g_mmisms_global.edit_content.dest_info.dest_list);
    return TRUE;
}

/*****************************************************************************/
//     Description :set address to message
//    Global resource dependence : 
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetAddressToMessage(
                                       const uint8        *num_ptr,    //IN:
                                       uint8            num_len,        //IN:
                                       uint32   *dest_list_ptr
                                       )
{
    MMISMS_SetAddressToMessage(num_ptr,num_len,(MMISMS_NUMBER_LIST_T *)dest_list_ptr);
}

/*****************************************************************************/
//     Description : to check the SMS state
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_CheckSMSState(BOOLEAN is_draw)
{
    if (MMISMS_IsOrderOk())
    {
        MMIAPICOM_StatusAreaSetMsgState(is_draw);
    }
}

/*****************************************************************************/
//     Description : to check the space
//    Global resource dependence : 
//  Author:louis.wei
//  Modify by: jian.ma
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSFull(void)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        if (MMISMS_CountFreeSpace(i) >0) 
        {
            return FALSE;
        }
    }
    return TRUE;
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : to delete all SMS
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_DelAllSMS(MMISMS_BOX_TYPE_E box_type)
{
    MMIAPISMS_DeleteAllMMSDone(box_type);
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : to check the order is ok
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_IsOrderOk ( void )
{
    return MMISMS_IsOrderOk();
}

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_IsReady( void )
{
    return MMISMS_IsReady();
}

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSReady(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_IsSMSReady(dual_sys);
}


/*****************************************************************************/
//     Description : to delete SMS info form order list by AT
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPISMS_DelSMSByAT(                                            //RETURN:
                             BOOLEAN                 is_mt_sms,            //IN:
                             uint8                    pos,                //IN:
                             MN_SMS_RECORD_ID_T        record_id            //IN:
                             )
{
    BOOLEAN result = FALSE;
    
#ifdef MMI_SYNCTOOL_SUPPORT
    if(SMS_SYNC_POS_ME == pos)
    {
        result = MMISMS_DelSMSByAT(
            MN_DUAL_SYS_1,
            is_mt_sms,
            FALSE,
            record_id
            );
    }
    else if(pos >= SMS_SYNC_POS_SIM_1 && pos < (SMS_SYNC_POS_SIM_1+MMI_DUAL_SYS_MAX))
    {
        result = MMISMS_DelSMSByAT(
            (MN_DUAL_SYS_E)(pos - SMS_SYNC_POS_SIM_1),
            is_mt_sms,
            TRUE,
            record_id
            );
    }
#else
    if(0x30 == pos)
    {
        result = MMISMS_DelSMSByAT(
            MN_DUAL_SYS_1,
            is_mt_sms,
            FALSE,
            record_id
            );
    }
    else if(pos >= 0x31 && pos < (0x31 +MMI_DUAL_SYS_MAX))
    {
        result = MMISMS_DelSMSByAT(
            (MN_DUAL_SYS_E)(pos - 0x31),
            is_mt_sms,
            TRUE,
            record_id
            );
    }
#endif   

    return result;
}

//window related
/*****************************************************************************/
//     Description : to open the list of box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenSmsBox(
                          MMISMS_BOX_TYPE_E        box_type
                          )
{
    MMISMS_OpenSmsBox(box_type);
}

/*****************************************************************************/
//     Description : to show the new message prompt
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMIAPISMS_ShowNewMsgPrompt(void)
{
    MMISMS_ShowNewMsgPrompt(MN_DUAL_SYS_1, FALSE);  //if is_play_ring is FALSE,we won't care the param dual_sys
}

/*****************************************************************************/
//     Description : exit from sms module
//    Global resource dependence : 
//  Author: gang.tong
//    Note: 
/*****************************************************************************/
void MMIAPISMS_ExitFromSMS(void)
{
    MMK_CloseMiddleWin(MMISMS_START_WIN_ID, MMISMS_END_WIN_ID);

    if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        MMK_CloseWin(MMIMSG_MAIN_MENU_WIN_ID);
    }

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //Added by yuanl  2010.8.19
    MMIAPIMAIL_Exit();
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
}

/*****************************************************************************/
//     Description : to open the SMS main menu window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenMainMenuWin(void)
{
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();

    if(MMISMS_IsOrderOk() && MMISMS_IsMMSOrderReady()) // 增加判断mms是否排序完毕
    {
        MMIAPISMS_ExitFromSMS();
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_SMS, MMISMS_OpenMainMenuWindowExt);
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        // 需要开启等待窗口    
        MMISMS_OpenInitWaitingWindow(MMISMS_OPEN_MAIN_MENU);
    }
}

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
/*****************************************************************************/
//     Description : open frequent word list for cc module
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenFreqWordListForCc(
                                     MN_DUAL_SYS_E dual_sys,
                                     uint8    number_len,    //IN:
                                     uint8    *number_str    //IN:
                                     )
{
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_OpenFreqWordListForCc len = %d, addr = %p"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_376_112_2_18_2_52_27_0,(uint8*)"dd",number_len, number_str);

    MMIAPISMS_ExitFromSMS();

    MMISMS_OpenFreqWordListForCc(dual_sys,number_len, number_str);
}
#endif

/*****************************************************************************/
//  Description : init SMS Module
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitModule(void)
{
    MMISMS_RegMenuGroup();
    MMISMS_RegWinIdNameArr();
    MMISMS_RegNv();
}

/*****************************************************************************/
//  Description : close all SMS window
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
void MMIAPISMS_CloseAllSMSWindow(void) 
{
    if (MMK_IsOpenWin(MMIMSG_MAIN_MENU_WIN_ID))
    {
        MMK_CloseMiddleWin(MMISMS_START_WIN_ID, MMISMS_END_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : open deleting SMS window
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
/*****************************************************************************/
void MMIAPISMS_OpenDelWindow(void) 
{
    if (MMISMS_GetDelAllFlag())
    {
        MMI_STRING_T prompt_str = {0};

        MMI_GetLabelTextByLang(STR_NOTE_WAITING, &prompt_str);
        MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISMS_DELETE_ANIM_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,MMISMS_HandleWaitWinMsg);
    }
}
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
//     Description : to open the window to call the voice mail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
void MMIAPISMS_OpenCallVMWin(MN_DUAL_SYS_E dual_sys)
{
    g_vmn_oper_type = MMISMS_VMN_OPER_TYPE_IDLE_DIAL;

    MMISMS_OpenVMNumberWin(dual_sys, MMISMS_LINE1_VMN_RECORD_ID);
}
#endif
/*****************************************************************************/
#if 0 /* Reduce Code size */
/*****************************************************************************/
//Description : Get SMS Sending SIM(Not In Sending return MMI_DUAL_SYS_MAX)
//    Global resource dependence : 
//  Author: wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetSendingSIM(void)
{
    if (MMIAPISMS_IsSendingSMS())
    {
        return MMISMS_GetCurSendDualSys();
    }

    return MMI_DUAL_SYS_MAX;
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : restore factory setting
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMIAPISMS_RestoreFactorySetting(void)
{
    MN_RETURN_RESULT_E ret_value = MN_RETURN_SUCCESS;
    MMINV_SMS_STATUS_REPORT_T sms_status_report_flag = FALSE;

    MMISMS_SetValidPeriod(MMISMS_VALID_PERIOD_DEFAULT);//@zhaohui,cr95392
        
    ret_value = MMISMS_GetNDTNvSetting(MESSAGES_NV_SMS_DELIVERY_REPORT,&sms_status_report_flag);
    if(ret_value != MN_RETURN_SUCCESS )
    {
        sms_status_report_flag  = MMISMS_STATUSREPORT_FLAG_DEFAULT;
    }

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT    
    MMISMS_InitAlphabetNDT(); 
#endif

    MMISMS_SetStatusReportFlag(sms_status_report_flag);
#if defined(MMI_SMSCB_SUPPORT)
    MMIAPISMSCB_RestoreFactorySetting();
#endif
    MMISMS_SetPreferredConnection(MMISMS_PREFERRED_PATH_GSM_PREFERRED);
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT 
    MMISMS_SetAutoSave(MMISMS_AUTOSAVE_FLAG_DEFAULT);
#endif
    MMISMS_SetSavePos(MMISMS_SAVEPOS_PHONE);
    MMISMS_SetOrderType(MMISMS_ORDER_BY_TIME);
    MMIAPISMS_SetDefaultPresetSms(FALSE);
    MMISMS_SetPushSmsBrowserInfo(MMISMS_BROWSERINFO_FLAG_DEFAULT);
    MMISMS_SetSMSIsExtract(MMISMS_ISEXTRACT_FLAG_DEFAULT);
    MMISMS_SetSecurityPwd();
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    MMISMS_SetAutoSignatureFlag(MMISMS_AUTO_SIGNATURE_FLAG_DEFAULT);
#endif
}

/*****************************************************************************/
//     Description : 根据record_id将一条信息从order list 中删除
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteMsgInorder(MMISMS_MSG_TYPE_E msg_type, int32 record_id)
{
    MMISMS_ORDER_ID_T      order_id = PNULL;
    BOOLEAN                ret_value = FALSE;

    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_DeleteMsgInorder : msg_type = %d,record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_483_112_2_18_2_52_27_1,(uint8*)"dd", msg_type, record_id );

    MMISMS_GetOrderIdByStorage(&order_id, msg_type, MN_DUAL_SYS_MAX, MN_SMS_NO_STORED, record_id);
    if ( PNULL == order_id )
    {
        //can't find the mms in order list by record id
        ret_value = FALSE;        
    }
    else
    {
#if 0   //user MMISMS_DATACHANGE_TYPE_DELETE trigger             
        if (MMISMS_TYPE_MMS == order_id->flag.msg_type)
        {
            if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)) 
            {
                MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容 
            }
        }
#endif
        ret_value = MMISMS_DelSmsInfoInOrder(order_id);
        if (MMISMS_IsMMSOrderReady())
        {
            //MMISMS_HandleOrderChangeCallBack();
            MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_DELETE);              
        }
    }

    return ret_value;
}


/*****************************************************************************/
//     Description : 插入一条信息到order list
//    Global resource dependence : 
//  Author: liming.deng
//    Note:  为彩信、wap push、wap ota、mms ota模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_InsertOneMsgToOrder(MMISMS_ORDER_INFO_T *order_ptr)
{
    MMISMS_STATE_T state_info = {0};
    BOOLEAN ret_value  = FALSE;
    MMISMS_DATACHANGE_TYPE_E type = MMISMS_DATACHANGE_TYPE_MAX;
    if (PNULL == order_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_InsertOneMsgToOrder order_ptr==PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_524_112_2_18_2_52_27_2,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_InsertOneMsgToOrder order->mo_mt_type = %d, mms_record_id = %d, dual_sys = %d, msg_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_528_112_2_18_2_52_27_3,(uint8*)"dddd",order_ptr->mo_mt_type, order_ptr->record_id, order_ptr->dual_sys, order_ptr->msg_type);

    state_info.flag.mo_mt_type = order_ptr->mo_mt_type;
    state_info.flag.msg_type = order_ptr->msg_type;
    state_info.flag.storage = MN_SMS_NO_STORED;
    state_info.time = order_ptr->time;
    state_info.record_id = order_ptr->record_id;
    state_info.flag.dual_sys = order_ptr->dual_sys;
    state_info.flag.is_concatenate_sms = FALSE;
    state_info.flag.is_locked = order_ptr->is_lock;
    state_info.flag.folder_type = order_ptr->folder_type;
    state_info.display_content.content_len = MIN(order_ptr->display_content.content_len, MMIMESSAGE_DIAPLAY_LEN );
    MMI_WSTRNCPY( state_info.display_content.content,
                            MMIMESSAGE_DIAPLAY_LEN,
                            order_ptr->display_content.content,
                            state_info.display_content.content_len,
                            state_info.display_content.content_len
                            );
    //SCI_TRACE_LOW:"state_info.display_content.content_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_546_112_2_18_2_52_27_4,(uint8*)"d",state_info.display_content.content_len);
    SCI_MEMCPY(&(state_info.number.sender),&(order_ptr->sender),(sizeof(uint8)*MMISMS_NUMBER_MAX_LEN));

    if (MMISMS_IsOrderOk())
    {
        ret_value = MMISMS_InsertSmsToOrder(&state_info); 
        
        //for msg delete all 11/02/18 信息全部删除使用
#ifdef MMI_SMS_DELALL_SUPPORT        
#ifdef  MMISMS_CHATMODE_ONLY_SUPPORT      
        if (MMK_IsOpenWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID))
        {
            MMK_SendMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容    
        }
#else
        if (MMK_IsOpenWin(MMISMS_MSGDEL_ALL_WIN_ID))
        {
            MMK_SendMsg(MMISMS_MSGDEL_ALL_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容    
        }
#endif        
#endif
        if (MMISMS_IsMMSOrderReady())
        {   
            if(MMISMS_FOLDER_NORMAL == order_ptr->folder_type)
            {
                type = MMISMS_DATACHANGE_TYPE_ADD;
            }
            else
            {
                type = MMISMS_DATACHANGE_TYPE_SECURITY_ADD;
            }
            
            MMISMS_TrigSmsDataChangeEvent(type);
            MMIAPICOM_StatusAreaSetMsgState(TRUE);
        }
    }
    else
    {
        ret_value = MMISMS_InsertSmsToUnhandleOrder(&state_info);        
    }
#ifdef MMI_SMS_CHAT_SUPPORT
    if ((MMISMS_TYPE_MMS == order_ptr->msg_type) &&
         MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) &&
         MMISMS_FOLDER_SECURITY != order_ptr->folder_type  &&
         MMISMS_CompareMMSNum(order_ptr->sender,g_mmisms_global.sender)
                 && (order_ptr->mo_mt_type != MMISMS_MO_DRAFT))
    {
        MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MSG_MMS_UPDATE_CHAT, PNULL,0); 
    }
#endif

    return ret_value;
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : 更新聊天界面的彩信发送状态
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_UpdateResendMMSLabel(int32 record_id)
{
#if 0 
    MMI_STRING_T time_str = {0};
    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar time[20] = {0};
    uint16 i = 0;    

    for(i=0;i<MMISMS_CHAT_MAX_ITEM;i++)
    {
        if((g_mmisms_global.chat_item[i].cur_order_id != PNULL) && (g_mmisms_global.chat_item[i].is_valid) && (record_id == g_mmisms_global.chat_item[i].cur_order_id->record_id))
        {
            order_id =  g_mmisms_global.chat_item[i].cur_order_id;
            break;
        }
    }
    if(PNULL == order_id || i == MMISMS_CHAT_MAX_ITEM)
    {
        SCI_TRACE_LOW("mmisms resend order pnull");
        return;        
    }

    time_str.wstr_ptr = time;
    time_str.wstr_len = MMISMS_GetDateTimeStringByOrder(order_id, TRUE,FALSE,TRUE, time);
    
    if (MMISMS_MO_SEND_FAIL == order_id->flag.mo_mt_type 
        && order_id->flag.msg_type == MMISMS_TYPE_MMS
        && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
        && MMISMS_CompareMMSNum(order_id->number.sender,g_mmisms_global.sender)
        )
    {
        wchar *str = PNULL;
        MMI_STRING_T wstr = {0};
        MMI_STRING_T fail_str = {0};
        
        if (MMISMS_TYPE_MMS == order_id->flag.msg_type
                && (MMIAPIMMS_IsProcessMMSById(order_id->record_id) || MMIAPIMMS_IsMMSInDownloadQueue(order_id->record_id)))
        {
            MMI_GetLabelTextByLang(TXT_SENDING, &fail_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_COMM_SEND_FAILED, &fail_str);
        }

        str = SCI_ALLOC_APPZ((MMISMS_CHAT_SENDFAIL_STR_MAX_LEN + 1) * sizeof(wchar));
        if (PNULL == str)
        {
            return;
        }
        wstr.wstr_ptr = str;
        MMI_WSTRNCPY(str, MMISMS_CHAT_SENDFAIL_STR_MAX_LEN , time_str.wstr_ptr, time_str.wstr_len, MIN(time_str.wstr_len, MMISMS_CHAT_SENDFAIL_STR_MAX_LEN));
        MMIAPICOM_Wstrcat(str, L" ");
        MMIAPICOM_Wstrncat(str, fail_str.wstr_ptr, fail_str.wstr_len);
        wstr.wstr_len = MMIAPICOM_Wstrlen(str);

        GUILABEL_SetText(MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + i, &wstr, FALSE);
        SCI_FREE(str);

    }    
#endif    
}


/*****************************************************************************/
//  Description : 清除发送文字接口
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearChatEditString(void)
{
    GUIEDIT_ClearAllStr(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
}
/*****************************************************************************/
//  Description : 更新聊天界面封装接口
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_UpdateChatShow(MMISMS_ORDER_INFO_T *order_info_ptr)
{
    if (PNULL == order_info_ptr)
    {
        return FALSE;
    }
    if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) &&
         MMISMS_CompareMMSNum(order_info_ptr->sender,g_mmisms_global.sender)
         &&!MMK_IsOpenWin(MMISMS_READSMS_WAIT_WIN_ID))
    {
        MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MSG_MMS_UPDATE_CHAT, PNULL,0);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 更新聊天界面封装接口
//  Global resource dependence : 
//  Author: liming.deng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_UpdateRecChatShow()
{
    if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
    {
        MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MSG_MMS_DATA_RECEIVED, PNULL,0);
    }
}

/*****************************************************************************/
//  Description :是否锁屏是否未读短信
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsKeyLockNeedCountSms(BOOLEAN is_keylock_need_count_sms)
{
    MMISMS_SetIsKeyLockNeedCountSms(is_keylock_need_count_sms);
}

/*****************************************************************************/
//  Description :是否锁屏是否未读短信
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsKeyLockNeedCountSms(void)
{
    return MMISMS_GetIsKeyLockNeedCountSms();
}
#endif

/*****************************************************************************/
//     Description : 更新信息状态
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_UpdateMsgStatusInOrder(MMISMS_ORDER_INFO_T *order_info_ptr)
{
    MMISMS_STATE_T state_info = {0};
    MMISMS_ORDER_ID_T old_order_id = PNULL;
    MMISMS_STATE_T old_state_info = {0};
    BOOLEAN ret_value = FALSE;


    if (PNULL == order_info_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_InsertOneMsgToOrder order_ptr==PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_613_112_2_18_2_52_27_5,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_UpdateMsgStatusInOrder enter: record_id = %d, dual_sys = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_617_112_2_18_2_52_27_6,(uint8*)"dd", order_info_ptr->record_id, order_info_ptr->dual_sys);

    state_info.time = order_info_ptr->time;
    state_info.record_id = order_info_ptr->record_id;    
    state_info.flag.mo_mt_type = order_info_ptr->mo_mt_type;
    state_info.flag.storage = MN_SMS_NO_STORED;
    state_info.flag.dual_sys = order_info_ptr->dual_sys;
    SCI_MEMCPY(&(state_info.number.sender),&(order_info_ptr->sender),(sizeof(uint8)*MMISMS_NUMBER_MAX_LEN));
    state_info.flag.is_locked = order_info_ptr->is_lock;
    state_info.display_content.content_len = MIN(order_info_ptr->display_content.content_len, MMIMESSAGE_DIAPLAY_LEN );
    MMI_WSTRNCPY( state_info.display_content.content,
                MMIMESSAGE_DIAPLAY_LEN,
                order_info_ptr->display_content.content,
                state_info.display_content.content_len,
                state_info.display_content.content_len
                );

    state_info.flag.msg_type = order_info_ptr->msg_type;
    MMISMS_GetOrderIdByStorage(&old_order_id, (MMISMS_MSG_TYPE_E)state_info.flag.msg_type, MN_DUAL_SYS_MAX, MN_SMS_NO_STORED, state_info.record_id);
    if (PNULL == old_order_id ) //can't find the mms in order list by record id
    {
        ret_value = FALSE;
    }
    else
    {
        
    
        MMISMS_GetSmsInfoFromOrder(old_order_id, &old_state_info);
        state_info.flag.folder_type = order_info_ptr->folder_type;
        state_info.flag.is_search = old_state_info.flag.is_search;
        if (!(MMISMS_TYPE_WAP_OTA == old_state_info.flag.msg_type && MMISMS_MT_HAVE_READ == old_state_info.flag.mo_mt_type))
        {//如果是已读ota，就不需要更新了
            //MMISMS_UpdateMMSMarkedFlag(old_order_id);

            MMISMS_UpdateSmsInfoInOrder(old_order_id,&state_info); 
            //for msg delete all 11/02/18 信息全部删除使用
#ifdef MMI_SMS_DELALL_SUPPORT            
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            if (MMK_IsOpenWin(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID))
            {
                MMK_SendMsg(MMISMS_MSGTYPEDEL_ALL_CHILD_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容    
            }
#else
            if (MMK_IsOpenWin(MMISMS_MSGDEL_ALL_WIN_ID))
            {
                MMK_SendMsg(MMISMS_MSGDEL_ALL_WIN_ID, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容    
            }
#endif            
#endif
            MMISMS_SetNeedSort(TRUE);
            MMIAPICOM_StatusAreaSetMsgState(TRUE);
        }        
        ret_value = TRUE;        
    }
#ifdef MMI_SMS_CHAT_SUPPORT
    if (MMISMS_TYPE_MMS == order_info_ptr->msg_type)
    {
        MMISMS_ORDER_ID_T new_order_id = PNULL;
        MMISMS_GetOrderIdByStorage(&new_order_id, (MMISMS_MSG_TYPE_E)state_info.flag.msg_type, MN_DUAL_SYS_MAX, MN_SMS_NO_STORED, state_info.record_id);   
        if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) &&
         MMISMS_CompareMMSNum(order_info_ptr->sender,g_mmisms_global.sender)
         && MMK_IsOpenWin(MMISMS_READSMS_WAIT_WIN_ID)
         && old_order_id != new_order_id)
        {         
            MMISMS_UpdateChatItemOrder(old_order_id,new_order_id);    
        }
        MMIAPISMS_UpdateChatShow(order_info_ptr);
    }
#endif

    return ret_value;
}

/*****************************************************************************/
// Description : 更新彩信图标
// Global resource dependence : 
// Author: minghu.mao
// Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC void MMIAPISMS_UpdateMsgStatus(void)
{
    //MMISMS_HandleOrderChangeCallBack();
    MMISMS_TrigSmsDataChangeEvent(MMISMS_DATACHANGE_TYPE_UPDATE);
    MMIAPICOM_StatusAreaSetMsgState(TRUE);
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : 当MMS全部删除完毕后，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteAllMMSDone (MMISMS_BOX_TYPE_E    box_type)
{
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_DeleteAllMMSDone enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_725_112_2_18_2_52_27_8,(uint8*)"");

    //删除完毕
    if (0 == MMISMS_GetBoxTotalNumberFromOrder(box_type))
    {
       MMISMS_OpenMsgBox(TXT_DELETED, IMAGE_PUBWIN_SUCCESS); 
       MMIPUB_CloseWaitWin(MMISMS_DELETE_ANIM_WIN_ID);
    }
    else
    {
        MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_DELETE_NEXT);  
    }
    return TRUE;
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : 当MMS排序完成时，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_MMSOrderDone (void)
{
    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_MMSOrderDone enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_747_112_2_18_2_52_28_9,(uint8*)"");
    MMISMS_SetMMSOrderReady(TRUE);

    return TRUE;
}
/*****************************************************************************/
//     Description : MMS是否已经排序完成
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_IsMMSOrderReady(void)
{
    return MMISMS_IsMMSOrderReady();
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//     Description : 当MMS初始化完成时，调用该函数，通知SMS模块。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC void MMIAPISMS_MMSInitDone(void)
{
    //SCI_TRACE_LOW:"MMISMS:MMIAPISMS_MMSInitDone"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_768_112_2_18_2_52_28_10,(uint8*)"");
    if (MMISMS_IsOrderOk())//短信排序完，主动触发，否则由短信触发
    {
        BOOLEAN is_open_waiting = MMK_IsOpenWin(MMISMS_WAITING_WIN_ID);//在MMISMS_HandleSortTimer会关闭MMISMS_WAITING_WIN_ID，所以需要记录

        MMISMS_HandleSortTimer(0, NULL);

        if (!is_open_waiting)//如果开启等待窗口，不需要发消息
        {
            MMISMS_HandleOrderChangeCallBack();
        }
    }
}
#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : 当打开新彩信窗口时，按下ok键调用此函数进入收件箱。
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为彩信模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_OpenMtBox(void)
{
    MMIAPISMS_ExitFromSMS();
    MMISMS_OpenSmsBox( MMISMS_BOX_MT );
    return TRUE;
}

/*****************************************************************************/
//     Description : idle answer sms
//    Global resource dependence : 
//  Author: rong.gu
//    Note: idle msg reply
/*****************************************************************************/
PUBLIC void MMIAPISMS_AnswerIdleMsg(void *param1, void *param2)
{
    uint8 sender_number[MMISMS_NUMBER_MAX_LEN+1] = {0};
    uint8 sender_number_len = 0;

	if( SCI_FALSE == MMIAPISMS_OpenConfirmation((void *)param1,MMISMS_ENTER_BY_IDLE_Reply))
	{
		return ;
	}

    if(PNULL != param1)
    {
        MMIPB_BCD_NUMBER_T *bcd_number_ptr = ((MMIPB_BCD_NUMBER_T *)param1);
        sender_number_len  = MMIAPICOM_GenNetDispNumber( 
                    MMIPB_GetNumberTypeFromUint8(bcd_number_ptr->npi_ton),
                    (uint8)MIN(bcd_number_ptr->number_len, (MMISMS_PBNUM_MAX_LEN >> 1)),
                    (uint8*) (bcd_number_ptr->number),
                    sender_number,
                    (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                    );

    }
    MMIAPISMS_AnswerMessage(MN_DUAL_SYS_MAX,PNULL, sender_number, sender_number_len);
}

/*****************************************************************************/
//     Description : to enter the window for writing SMS 
//    Global resource dependence : 
//  Author:liming.deng
//    Note: interface for other module
/*****************************************************************************/
void MMIAPISMS_AnswerMessage(
                               MN_DUAL_SYS_E    dual_sys,           //IN:
                               MMI_STRING_T        *sms_init_ptr,        //IN:
                               uint8            *dest_addr_ptr,        //IN:
                               uint8            dest_addr_len        //IN:
                               )
{    
    BOOLEAN is_not_inited = FALSE;

    is_not_inited = MMISMS_IsSimNotInited();

    if (MMISMS_IsOrderOk()&& MMISMS_IsMMSOrderReady())
    {
        if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
        {           
            MMISMS_WRITE_MSG_INFO_T mmisms_write_info = {0};
            uint16 length = 0;
            mmisms_write_info.dest_addr_ptr = SCI_ALLOCAZ( dest_addr_len + 1 );
            if(PNULL == mmisms_write_info.dest_addr_ptr)
            {
                return;
            }    
               
            SCI_MEMCPY((void*)mmisms_write_info.dest_addr_ptr, (void*)dest_addr_ptr, dest_addr_len);
            mmisms_write_info.dest_addr_len = dest_addr_len;
			if(sms_init_ptr != PNULL)
			{
                length = sms_init_ptr->wstr_len;
                mmisms_write_info.sms_str.wstr_ptr = SCI_ALLOCAZ( (length + 1) * sizeof(wchar) );
                if(PNULL == mmisms_write_info.sms_str.wstr_ptr)
                {
                    SCI_FREE(mmisms_write_info.dest_addr_ptr);
                    return;
                } 
                SCI_MEMCPY((void*)mmisms_write_info.sms_str.wstr_ptr , (void*)sms_init_ptr->wstr_ptr,  (length + 1) * sizeof(wchar));
                mmisms_write_info.sms_str.wstr_len = length;
            }
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, MMISMS_OpenWriteWindowExt,(DPARAM)&mmisms_write_info,sizeof(MMISMS_WRITE_MSG_INFO_T));
        }
        else
        {
            MMISMS_WriteNewMessage(
                dual_sys,
                sms_init_ptr,
                FALSE,
                dest_addr_ptr,
                dest_addr_len
                );
        }
    }
    else if(is_not_inited)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_INIT);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
    }
}

/*****************************************************************************/
//     Description : Notify MN to save mms push.
//    Global resource dependence : 
//  Author:liming.deng
//    Note: interface for udisk module
/*****************************************************************************/
void MMIAPISMS_SetSavePush( BOOLEAN is_save )
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        MNSMS_SetSaveMMSPushEx(i, is_save);
    }

}

/*****************************************************************************/
//     Description : Begin read mms push for mms module.
//    Global resource dependence : 
//  Author:liming.deng
//    Note: interface for udisk module
/*****************************************************************************/
BOOLEAN MMIAPISMS_ReadMmsPush( void )
{
    BOOLEAN ret_val = FALSE;

    ret_val = MMISMS_ReadMMSPushFromUnhandledList();

    return ret_val;
}

/*****************************************************************************/
//     Description : Open message main menu
//    Global resource dependence : 
//  Author:Lin.Lin
//    Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISMS_EnterSmsChatWin(void *param1, void *param2)
{
#ifdef MMI_SMS_CHAT_SUPPORT
    MMISMS_EnterSMSChatReadWinByNumber((MMIPB_BCD_NUMBER_T*)param1);
#endif
}


		/*****************************************************************************/
// 	Description : open confirmation dialog
//	Global resource dependence : none
//  Author: weipeng.wang
//	Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleSMSConfirmationDialogMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
		switch(g_sms_back_play_enter_app)
		{
			case MMISMS_ENTER_BY_IDLE: 
			case MMISMS_ENTER_BY_IDLE_Reply:
				{					
					MMIPUB_INFO_T * pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
					if(PNULL != pubwin_info_ptr)
					{
						MMIPB_BCD_NUMBER_T *  entry_param_ptr = (MMIPB_BCD_NUMBER_T *)pubwin_info_ptr->user_data;
						MMK_CloseWin(win_id);
						if(PNULL != entry_param_ptr )
						{
							SCI_FREE(entry_param_ptr);
						}
					}
				}	
       	 		break;

			case MMISMS_ENTER_BY_WRITEMESSAGE:
				{
					MMIPUB_INFO_T * pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
					if(PNULL != pubwin_info_ptr)
					{
						MMISMS_ENTER_BY_MUSIC *  entry_param_ptr = (MMISMS_ENTER_BY_MUSIC *)pubwin_info_ptr->user_data;
						MMK_CloseWin(win_id);
						if(PNULL != entry_param_ptr )
						{
							if(PNULL != entry_param_ptr->dest_addr_ptr)
							{
								SCI_FREE(entry_param_ptr->dest_addr_ptr);
							}
							if(PNULL != entry_param_ptr->sms_init_ptr)
							{
								if(PNULL != entry_param_ptr->sms_init_ptr->wstr_ptr)
								{
									SCI_FREE(entry_param_ptr->sms_init_ptr->wstr_ptr);
								}
								SCI_FREE(entry_param_ptr->sms_init_ptr);
							}
							
							SCI_FREE(entry_param_ptr);
						}
						
					}
				}
				break;
			default :
				MMK_CloseWin(win_id);	
			break;
		}

        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:

	#ifdef MMI_AUDIO_PLAYER_SUPPORT
	//关闭MP3
	MMIAPMAINWIN_Destory();
	#endif
	 
	
	SCI_TRACE_LOW("[MMISMS] func[%s] line[%d] app %d",__FUNCTION__,__LINE__,g_sms_back_play_enter_app);
	switch(g_sms_back_play_enter_app)
	{
		case     MMISMS_ENTER_BY_MAINMENU :
				MMK_CloseWin(win_id);				
				MMIMAINMENU_StartMessage();
				break;
			
		case     MMISMS_ENTER_BY_PB :				
				MMIAPIPB_ENTERMESSAGE_BACK_PLAY();
				MMK_CloseWin(win_id);
				break; 
			
		case     MMISMS_ENTER_BY_CL :
				MMK_CloseWin(win_id);				
				MMIAPICL_HandleAnswerSms();
				break;
			
		case     MMISMS_ENTER_BY_IDLE: 
				{					
					MMIPUB_INFO_T * pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
					if(PNULL != pubwin_info_ptr)
					{
						MMIPB_BCD_NUMBER_T *  entry_param_ptr = (MMIPB_BCD_NUMBER_T *)pubwin_info_ptr->user_data;
						MMK_CloseWin(win_id);
						if(PNULL !=entry_param_ptr )
						{
						#ifdef MMI_SMS_CHAT_SUPPORT
							MMISMS_EnterSMSChatReadWinByNumber(entry_param_ptr);
                                                #endif
							SCI_FREE(entry_param_ptr);
						}
					}
				}
				break;

		case MMISMS_ENTER_BY_IDLE_Reply:
				{	
					MMIPUB_INFO_T * pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
					if(PNULL != pubwin_info_ptr)
					{
						MMIPB_BCD_NUMBER_T *  entry_param_ptr = (MMIPB_BCD_NUMBER_T *)pubwin_info_ptr->user_data;					
						MMK_CloseWin(win_id);
						if(PNULL !=entry_param_ptr )
						{
							MMIAPISMS_AnswerIdleMsg((void *)entry_param_ptr,PNULL);
							SCI_FREE(entry_param_ptr);
						}
					}
				}	
       	 		break;

		case MMISMS_ENTER_BY_DIAL_IDLE:
				{
					MMIPUB_INFO_T * pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

					if(PNULL != pubwin_info_ptr)
					{
						uint32   entry_param = (uint32 )pubwin_info_ptr->user_data;	
						MMIAPIDIAL_IDLE_ENTERMESSAGE(entry_param);
						MMK_CloseWin(win_id);	
					}			
				}
			break;
		case MMISMS_ENTER_BY_DIAL_MENU:
				{
					MMIPUB_INFO_T * pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

					if(PNULL != pubwin_info_ptr)
					{
						uint32   entry_param = (uint32 )pubwin_info_ptr->user_data;								
						MMIAPIDIAL_MENU_ENTERMESSAGE(entry_param);
						MMK_CloseWin(win_id);			
					}					
				}
			break;	
		case MMISMS_ENTER_BY_WRITEMESSAGE:
				{
					MMIPUB_INFO_T * pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
					if(PNULL != pubwin_info_ptr)
					{
						MMISMS_ENTER_BY_MUSIC *  entry_param_ptr = (MMISMS_ENTER_BY_MUSIC *)pubwin_info_ptr->user_data;
						if(PNULL !=entry_param_ptr )
						{
							MMISMS_WriteNewMessage(entry_param_ptr->dual_sys,entry_param_ptr->sms_init_ptr,entry_param_ptr->need_load_saved,entry_param_ptr->dest_addr_ptr,entry_param_ptr->dest_addr_len);
							
							if(PNULL != entry_param_ptr->dest_addr_ptr)
							{
								SCI_FREE(entry_param_ptr->dest_addr_ptr);
							}

							if(PNULL != entry_param_ptr->sms_init_ptr)
							{
								if(PNULL != entry_param_ptr->sms_init_ptr->wstr_ptr)
								{
									SCI_FREE(entry_param_ptr->sms_init_ptr->wstr_ptr);
								}
								SCI_FREE(entry_param_ptr->sms_init_ptr);
							}

							SCI_FREE(entry_param_ptr);
						}
						MMK_CloseWin(win_id);
					}
				}
				break;
		
		
		}
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
LOCAL void MMIAPISMS_OpenConfirmationDialog(MMI_TEXT_ID_T text_id,void * enter_ptr,uint8 app)
{
	void * user_data = PNULL;
	uint8 flag_malloc = TRUE;
	
	SCI_TRACE_LOW("MMISMS func[%s] line[%d] app[%d]  ",__FUNCTION__,__LINE__,app);
	switch(app)
	{
		case MMISMS_ENTER_BY_IDLE:
				(MMIPB_BCD_NUMBER_T *) user_data  =  SCI_ALLOCA((sizeof(MMIPB_BCD_NUMBER_T)));
				if(PNULL== user_data )
				{
					flag_malloc = FALSE;
					break;
				}
				SCI_MEMSET(user_data, 0, sizeof(MMIPB_BCD_NUMBER_T));
				SCI_MEMCPY(user_data,enter_ptr,sizeof(MMIPB_BCD_NUMBER_T));
			break;

		case MMISMS_ENTER_BY_IDLE_Reply:
				(MMIPB_BCD_NUMBER_T *) user_data  =  SCI_ALLOCA((sizeof(MMIPB_BCD_NUMBER_T)));
				if(PNULL== user_data )
				{
					flag_malloc = FALSE;
					break;
				}
				SCI_MEMSET(user_data, 0, sizeof(MMIPB_BCD_NUMBER_T));
				SCI_MEMCPY(user_data,enter_ptr,sizeof(MMIPB_BCD_NUMBER_T));

			break;

		case MMISMS_ENTER_BY_DIAL_MENU:
		case MMISMS_ENTER_BY_DIAL_IDLE:
				user_data  =  enter_ptr;
				
			break;
			
		case MMISMS_ENTER_BY_WRITEMESSAGE:
			{
				MMISMS_ENTER_BY_MUSIC  *user_data_music = PNULL;
				user_data_music = (MMISMS_ENTER_BY_MUSIC *)enter_ptr;
				(MMISMS_ENTER_BY_MUSIC *) user_data = SCI_ALLOCA((sizeof(MMISMS_ENTER_BY_MUSIC)));
				if(PNULL== user_data )
				{
					flag_malloc = FALSE;
					break;
				}
				SCI_MEMSET(user_data, 0, sizeof(MMISMS_ENTER_BY_MUSIC));
				SCI_MEMCPY((MMISMS_ENTER_BY_MUSIC *)user_data,user_data_music,sizeof(MMISMS_ENTER_BY_MUSIC));
				if(PNULL != user_data_music->dest_addr_ptr )
				{				
					((MMISMS_ENTER_BY_MUSIC *)user_data)->dest_addr_ptr = SCI_ALLOCA((user_data_music->dest_addr_len)+1);
					if(PNULL == (((MMISMS_ENTER_BY_MUSIC *)user_data)->dest_addr_ptr))
					{
						flag_malloc = FALSE;
						break;
					}
					SCI_MEMSET(((MMISMS_ENTER_BY_MUSIC *)user_data)->dest_addr_ptr, 0,(user_data_music->dest_addr_len)+1);
					SCI_MEMCPY(((MMISMS_ENTER_BY_MUSIC *)user_data)->dest_addr_ptr,user_data_music->dest_addr_ptr,user_data_music->dest_addr_len);					
				}
				if(PNULL != user_data_music->sms_init_ptr)
				{
					uint8 length = user_data_music->sms_init_ptr->wstr_len;
					((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr = SCI_ALLOCA(sizeof(MMI_STRING_T));
					if(PNULL== (((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr) )
					{
						flag_malloc = FALSE;				
						break;
					}
					SCI_MEMSET(((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr,0,sizeof(MMI_STRING_T));
					if(PNULL != user_data_music->sms_init_ptr->wstr_ptr)
					{
						((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr->wstr_ptr = SCI_ALLOCA((length+1)*(sizeof(wchar)));
						if(PNULL == (((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr->wstr_ptr ))
						{
							flag_malloc = FALSE;		
							break;
						}
						((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr->wstr_len = length;
						SCI_TRACE_LOW("[MMISMS] func[%s] line[%d] length[%d]  ",__FUNCTION__,__LINE__,length);
						SCI_MEMSET((((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr->wstr_ptr),0,(length+1)*(sizeof(wchar)));
						MMI_WSTRNCPY(((MMISMS_ENTER_BY_MUSIC *)user_data)->sms_init_ptr->wstr_ptr, length, user_data_music->sms_init_ptr->wstr_ptr, length, length);
					}
				}
			
			}
			break;
	
	}
	if(!flag_malloc)
	{
		
		if((MMISMS_ENTER_BY_WRITEMESSAGE == app) &&(PNULL != user_data))
		{
			if(PNULL !=(((MMISMS_ENTER_BY_MUSIC *)user_data)->dest_addr_ptr))
			{		
				SCI_FREE(((MMISMS_ENTER_BY_MUSIC *)user_data)->dest_addr_ptr);	
			}
			if(PNULL !=(((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr) )
			{		
				SCI_FREE(((MMISMS_ENTER_BY_MUSIC *) user_data)->sms_init_ptr);	
			}						
			SCI_FREE(user_data);				
		}

		MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);	
		return ;
	}
	g_sms_back_play_enter_app = app;
 	MMIPUB_OpenAlertWinByTextIdEx(
                                                            MMK_GetFirstAppletHandle(), 
                                                            PNULL, 
                                                            text_id,
                                                            TXT_NULL, 
                                                            IMAGE_NULL,
                                                            PNULL,
                                                            PNULL,
                                                            MMIPUB_SOFTKEY_DIALOG_TURNOFF_CLOSE,
                                                            HandleSMSConfirmationDialogMsg,
                                                           (uint32)user_data);
}


PUBLIC  BOOLEAN MMIAPISMS_OpenConfirmation(void * enter_ptr,uint8 app)
{

	#ifdef MMI_AUDIO_PLAYER_SUPPORT
	 if(MMIAPIMP3_IsPlayerPlaying())//FM active suspend or play background
	{
		MMIAPISMS_OpenConfirmationDialog(STR_COM_TURN_BGD_MUSIC,(void *) enter_ptr, app);
		
		return SCI_FALSE;
	}
	else
	{
		MMIAPMAINWIN_Destory();
	}
	#endif
	
	return SCI_TRUE;

}


/*****************************************************************************/
//     Description : Open message main menu
//    Global resource dependence : 
//  Author:Lin.Lin
//    Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISMS_OpenMessageMainMenuWin(void) 
{

	if( SCI_FALSE == MMIAPISMS_OpenConfirmation(PNULL,MMISMS_ENTER_BY_MAINMENU))
	{
		return ;
	}

    MMK_CloseWin(MMIMSG_MAIN_MENU_WIN_ID);
    MMIAPISMS_CloseAllSMSWindow();
    MMIAPISMS_OpenMainMenuWin();
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : Get the sms sort timer Id.
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISMS_GetSmsSortTimerId(void)
{
    uint8 ret_val = 0;

    ret_val = MMISMS_GetSmsSortTimerId();
    
    return ret_val;
}

/*****************************************************************************/
//  Description :Insert the mms/wap/ota into sms list. 
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleSortTimer(uint8 timer_id, uint32 param)
{
    MMISMS_HandleSortTimer(timer_id, param);
}

/*****************************************************************************/
//  Description: init sms order list
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitSmsOrder(void)
{
    MMISMS_InitSmsOrder();
}

/*****************************************************************************/
//     Description : to get the count of sms which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadSmsCount(MN_DUAL_SYS_E dual_sys)
{
    uint16 ret_val = 0;
    
    ret_val = MMISMS_GetTobeReadSmsCount(dual_sys);

    return ret_val;
}

/*****************************************************************************/
//     Description : to append the info to list_box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetListCurrentItemContent(
                                                MMI_CTRL_ID_T ctrl_id,
                                                uint8 *str_len_ptr,
                                                wchar *str_ptr
                                                )
{
    MMISMS_GetListCurrentItemContent(
        ctrl_id,
        str_len_ptr,
        str_ptr
        );
}

#endif /* Reduce Code size */
/*****************************************************************************/
//  Description :clear dest number 
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearDestNum(void)
{
    MMISMS_ClearDestNum();
}

#if defined(MMI_VCARD_SUPPORT) || defined(MMI_VCALENDAR_SUPPORT)
/*****************************************************************************/
// Description : to show the new Vcard prompt
//               (参考函数:MMISMS_ShowNewMsgPrompt)
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_ShowNewVcardOrVcalendarPrompt(BOOLEAN is_play_ring,MMIVC_SMS_SEND_TYPE_E type)
{
    MMISMS_ShowNewVcardOrVcalendarPrompt(is_play_ring,type);
}
#endif

/*****************************************************************************/
//     Description : API for is in sms moudle window
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsInSMSWin(void)
{
    return MMISMS_IsInSMSWin();
}

/*****************************************************************************/
//  Description : MMIAPISMS_RefreshSMS
//  Global resource dependence : 
//  Author: wancna.you
//RETURN: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_RefreshSMS(MN_DUAL_SYS_E dual_sys)
{
    if (MMISMS_RefreshSMS(dual_sys))
    {
        MMISMS_InitSmsOrder();

        MMISMS_InitSmsList();

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//     Description : set default preset SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDefaultPresetSms(BOOLEAN is_clear)
{
    MMISMS_SetDefaultPresetSms(is_clear);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetMaxSimSmsNum(MN_DUAL_SYS_E dual_sys, uint8 sim_sms_num)
{
    MMISMS_SetMaxSimSmsNum(dual_sys, sim_sms_num);
}

/*****************************************************************************/
//     Description : to get the count of msg which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_E dual_sys, MMISMS_MSG_TYPE_E msg_type)
{
    return MMISMS_GetTobeReadMsgCount(dual_sys, msg_type);
}

/*****************************************************************************/
//     Description : to get the count of undownload msg
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_GetUnDownloadMsgCount(dual_sys);
}

/*****************************************************************************/
//     Description : to get the count of total message which is to be read(包括未读sms，mms，ota，push...)
//    Global resource dependence : s_mmisms_order
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetTobeReadMessageCount(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_GetTobeReadMessageCount(dual_sys);
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//    Description : to create URL option WIN for MMS mdu
//    Global resource dependence : 
//    Author:Kun.Yu
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateWinURLOpt( ADD_DATA add_data )
{
    MMISMS_Create_Win_URL_Opt( add_data );
}

/*****************************************************************************/
//     Description : get move all flag
//                  TRUE:        do move all SMS operation  
//                  FALSE:    without do move all SMS operation
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetMoveAllFlag(void)
{
    return (MMISMS_GetMoveAllFlag());
}

/*****************************************************************************/
//     Description : to get the resend count
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSMSBusy( void )
{
    if(MMIAPISMS_GetMoveAllFlag() ||
       MMIAPISMS_IsOrderOk())
    {
        return TRUE;
    }
    return FALSE;
    //return MMK_IsOpenWin(MMISMS_MOVE_ANIM_WIN_ID);
}

#endif /* Reduce Code size */

/*****************************************************************************/
//  Description : Get the push sms flag.
//  Global resource dependence : 
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetPushSmsBrowserInfo(void)
{
    return MMISMS_GetPushSmsBrowserInfo();
}

/*****************************************************************************/
//     Description : handle left and right msg in show SMS window
//    Global resource dependence : 
//  Author: minghu.mao
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleLeftRightMsgInShowSmsWin(
                                                     MMI_WIN_ID_T        win_id,        //IN:
                                                     MMI_MESSAGE_ID_E    msg_id        //IN:
                                                     )
{
    HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
}

#ifdef MMI_SMS_RETRY_SUPPORT
/*****************************************************************************/
//     Description : to set the resend count
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetResendCount( 
                               BOOLEAN     is_set_resend_count
                               )
{
    MMISMS_SetDeliverResendFlag(is_set_resend_count);
}

/*****************************************************************************/
//     Description : to get the resend count
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetResendCount( void )
{
    return MMISMS_GetDeliverResendFlag();
}
#endif


/*****************************************************************************/
//     Description : Set Need Sort
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetNeedSort( BOOLEAN is_sort )
{
   MMISMS_SetNeedSort(is_sort);
   return;
}

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
//     Description : async search sms
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_StartLocalSearch(MMI_STRING_T *key_word_str,BOOLEAN is_mms_only)
{
    if (MMISMS_IsOrderOk()  && MMISMS_IsMMSOrderReady())
    {
        MMISMS_HandleSearchSms(key_word_str,is_mms_only);
    }
}

/*****************************************************************************/
// 	Description : 本地搜索退出，短信模块释放模块内部资源
//	Global resource dependence : 
//    Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_FreeLocalSearchResource(void)
{    
    
}

/*****************************************************************************/
//     Description : stop async search sms
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_StopSearchSms(void)
{
    MMISMS_StopSearchSms();
}
#endif

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description : get win font size
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMIAPISMS_GetWinFont(void)
{
    return (MMISMS_GetWinFont());
}
#endif

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT  //delete all mail from message list
/*****************************************************************************/
//     Description : delete all mail from message list
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 为mail模块提供的接口函数。
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISMS_DeleteAllMailMsg(void)
{

    //SCI_TRACE_LOW:"MMISMS: MMIAPISMS_DeleteAllMailMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_1193_112_2_18_2_52_29_11,(uint8*)"");

    MMISMS_DeleteAllMail(MMISMS_TYPE_MAIL);

    return TRUE;

}
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description : 设置当前的字体大小
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetWinFont(GUI_FONT_T  font)
{
    MMISMS_FONT_E font_size = MMISMS_FONT_NORMAL;
    switch(font) 
    {
    case MMI_DEFAULT_BIG_FONT:
        font_size = MMISMS_FONT_BIG;
    	break;
    case MMI_DEFAULT_SMALL_FONT:
        font_size = MMISMS_FONT_SMALL;
        break;
    default:
        font_size = MMISMS_FONT_NORMAL;
        break;
    }
    MMISMS_SetFontSize(font_size);
}

#endif

/*****************************************************************************/
//  Description : response sms control
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_ResponseSmsCtl(MN_DUAL_SYS_E dual_sys,
                                                                                        MMI_STRING_T *alert_str_ptr,
                                                                                        MMI_STRING_T *display_str_ptr,
                                                                                        MN_CALLED_NUMBER_T *dest_me,
                                                                                        MN_CALLED_NUMBER_T *dest_sc,
                                                                                        BOOLEAN is_need_modified)
{
    return MMISMS_ResponseSmsCtl(dual_sys, alert_str_ptr, display_str_ptr, dest_me, dest_sc, is_need_modified);
}

/*****************************************************************************/
//     Description : MMIAPISMS_SetIsSMSRefresh
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsSMSRefresh(MN_DUAL_SYS_E dual_sys, BOOLEAN is_sms_refresh)
{
    MMISMS_SetIsSMSRefresh(dual_sys, is_sms_refresh);
}

/*****************************************************************************/
//     Description : MMIAPISMS_OrderInit
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OrderInit(void)
{
    MMISMS_InitSortStatusAccordingToSetting();
    MMISMS_FreeSmsOrder();

    MMISMS_SetMMSOrderReady(FALSE);

    MMIAPICOM_StatusAreaSetMsgState(TRUE);
}

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_CheckTimerMSG(void)
{
#ifdef MMI_TIMERMSG_SUPPORT
    return MMISMS_TIMERMSG_CheckTimerMSG(FALSE);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : read new sms
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_ReadNewSms(
                                 MMISMS_BOX_TYPE_E        box_type
                                        )
{
    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, (MMISET_PRIVACY_CALLBACK_PFUNC)MMISMS_ReadNewSmsEx, &box_type, sizeof(box_type));
}
#if 0
// no use
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void SMS_ValidatePrivacyReadNewSms(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_NONE;

    if (is_permitted)
    {
        if (PNULL != param_ptr)
        {
            box_type = *(MMISMS_BOX_TYPE_E*)param_ptr;
        }
        MMISMS_ReadNewSms(box_type);
    }
}
#endif

/*****************************************************************************/
//  Description : 得到第一条未读短信号码
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPISMS_GetOriginNumByPos(                        //RETURN: the length of num_ptr
                                      MN_DUAL_SYS_E     *dual_sys_ptr,  //out
                                      uint8                *num_ptr,        //OUT:
                                      uint8                 max_num_len    //IN:                                          
                                      )
{
    uint8 len = 0;
    uint8 i = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    
	//  g_mmisms_global.operation.cur_order_index = 0;
    cur_order_id = MMISMS_GetFirstUnreadMsgId();
 
    if (PNULL == cur_order_id)
    {
        *dual_sys_ptr = MN_DUAL_SYS_MAX;
        return 0;
    }

    if (PNULL == num_ptr || PNULL == dual_sys_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_NewSms_GetOriginNumByPos PNULL == num_ptr || PNULL == dual_sys_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_1344_112_2_18_2_52_29_12,(uint8*)"");
        return len;
    }
    
    if (cur_order_id->flag.msg_type == MMISMS_TYPE_SMS)
    {
        len = MMIAPICOM_GenDispNumber(
            MMIPB_GetNumberTypeFromUint8(cur_order_id->number.bcd_number.npi_ton),
            MIN(MN_MAX_ADDR_BCD_LEN, cur_order_id->number.bcd_number.number_len),
            cur_order_id->number.bcd_number.number,
            num_ptr,
            max_num_len
            );        
    }
    else
    {
        len = strlen((char *)cur_order_id->number.sender);
        //changed by minghu cr128630
        for (i = 0; i < len; i++)
        {
            if (MMIAPICOM_IsTelephoneNum(*(cur_order_id->number.sender+i)))
            {
                *(num_ptr + i) = *(cur_order_id->number.sender + i);
            }
            else
            {
                len = i;
                break;
            }
        }
        *(num_ptr + len) = 0;
    }
    *dual_sys_ptr = (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys;
	
    return (len);
}

/*****************************************************************************/
//  Description : 得到第一条未读短信的日期和时间
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetOriginTimeAndDate(
                                        wchar    *date_ptr,        //OUT:
                                        wchar    *time_ptr        //OUT:
                                        )
{

    MMISMS_ORDER_ID_T   cur_order_id = PNULL;

    if (PNULL == date_ptr || PNULL == time_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_NewSms_GetOriginTimeAndDate PNULL == date_ptr || PNULL == time_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_API_1401_112_2_18_2_52_29_13,(uint8*)"");
        return;
    }
    
    cur_order_id = MMISMS_GetFirstUnreadMsgId();
    
    if (PNULL == cur_order_id)
    {
        return;
    }

    MMISMS_GetDateTimeStringByOrder(cur_order_id,TRUE,TRUE,FALSE,date_ptr);
    MMISMS_GetDateTimeStringByOrder(cur_order_id,FALSE,FALSE,TRUE,time_ptr);
    
}

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//     Description : to Set the status report flag
//    Global resource dependence : g_mmisms_global
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetStatusReportFlag(BOOLEAN status_report_flag)
{
    MMISMS_SetStatusReportFlag(status_report_flag);
}

/*****************************************************************************/
//     Description : to set the save position
//    Global resource dependence : g_mmisms_global
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetSavePos(MMISMS_SMS_SAVEPOS_TYPE_E pos)
{
    MMISMS_SetSavePos(pos);
}

#endif


/*****************************************************************************/
//  Description : send the SMS for other modules
//  Global resource dependence :  
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SendSmsForOther(MMISMS_SEND_DATA_T *send_data_ptr)
{
    return MMISMS_SendSmsForOther(send_data_ptr);
}

#if defined( MMI_INDIAN_SOS_SUPPORT)||defined(SOS_KEYLONGWEB)
/*****************************************************************************/
//  Description : send one or more sms messages
//  Global resource dependence :  
//  Author:Yongli Tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MultiSendSms(MMISMS_MULTI_SEND_DATA_T *send_data_ptr)
{
    return MMISMS_MultiSendSms(send_data_ptr);
}
#endif

/*****************************************************************************/
//  Description : Select SIM API of SMS app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC	MMISMSSelectSIMWinCallback,
                                        ADD_DATA    add_data,
                                        BOOLEAN  psReady
                                        )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T    selectSimWinID = MMISMS_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    if (psReady)
    {
        caller_data.append_type = (uint32)MMISET_APPEND_SYS_OK;
    }
    else
    {
        caller_data.append_type = (uint32)MMISET_APPEND_SIM_OK;
    }
    caller_data.callback_func =MMISMSSelectSIMWinCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;

    caller_data.publist_id = MMISMS_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = add_data;
    caller_data.extra_data = PNULL;

    MMIAPIPHONE_OpenSelectSimWin(&caller_data);

#endif
    return MMI_RESULT_TRUE;
}


/*****************************************************************************/
//  Description : is sending sms
//  Global resource dependence : s_mmisms_sendlist
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsSendingSMS( void )
{
    if (MMISMS_GetIsSendingSms() && !MMISMS_IsSendListEmpty())
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : to handle the create select word win
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateSelectWordWin(MMI_HANDLE_T inserted_ctrl)
{
    MMISMS_CreateSelectWordWin(inserted_ctrl);
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT                
/*****************************************************************************/
// 	Description : get contact info
//	Global resource dependence : 
//  Author: rong.gu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetContactInfo(
                                  uint32 handle,
                                  MMI_STRING_T    *contact_info_ptr,    //OUT:
                                  uint16        max_len                //IN:
                                  )
{
    MMISMS_GetContactInfo(handle, contact_info_ptr,max_len);
}

#if 0
/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_HandleSendOperation(
                               MN_DUAL_SYS_E    dual_sys,
                               MMI_WIN_ID_T    win_id,    //IN:
                               BOOLEAN        is_re_send 
                               )
{
    MMISMS_HandleSendOperation(dual_sys,win_id,is_re_send);
}           
#endif

/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SendSms(MN_DUAL_SYS_E    dual_sys,BOOLEAN is_re_send)
{
    MMISMSAPP_SendSmsWithGlobalContent(dual_sys,is_re_send);
}

/*****************************************************************************/
//     Description : handle send operation
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SaveAsDraft(MN_DUAL_SYS_E    dual_sys,BOOLEAN is_replace,BOOLEAN is_timer_msg)
{
    SMS_SAVE_OPERATE_DATA_T op_data={0};

    op_data.dual_sys = dual_sys;
    op_data.is_needLoadSaved = FALSE;
    op_data.is_use_global_data = TRUE;
    op_data.is_timer_msg = is_timer_msg;

#ifdef MMI_TIMERMSG_SUPPORT
    if (is_timer_msg)
    {
       if(!MMISMS_TIMERMSG_CheckTimerSend(TRUE))
       {
            return;
       }
    }
#endif    
    
    if (is_replace)
    {
        MMISMSAPP_ReplaceSms(op_data,PNULL,PNULL,PNULL);
    }
    else
    {
        MMISMSAPP_SaveSms(op_data,PNULL,PNULL,PNULL);
    }
    
}

/*****************************************************************************/
//     Description : to delete request to MN
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_DeleteReqToMN(
                            MMISMS_BOX_TYPE_E        type,        //IN:
                            MMISMS_STATE_T*       order_id,    //IN:
                            uint32    oper_status    //IN:
                            )
{
    return MMISMS_DeleteReqToMN(type,(MMISMS_ORDER_ID_T)order_id,(MMISMS_OPER_STATUS_E)oper_status);
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE

/*****************************************************************************/
//     Description : check me have enough space for save sms
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MEHaveEnoughSpace(void)
{
    return MMISMS_MEHaveEnoughSpace();
}

#endif

/*****************************************************************************/
//     Description : to save the content which user input
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetMessageContent( 
                                        uint16    length,            //IN:
                                        wchar     *content_ptr,    //IN:
                                        BOOLEAN   is_need_packet,
                                        MMISMS_SMSEDIT_CONTENT_T *message_info
                                        )
{
    MMISMS_SetMessageContent(length,content_ptr,is_need_packet,message_info);
}
#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
// 	Description : get timer msg state
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetIsTimerMode(void)
{
    return MMISMS_GetIsTimerMode();
}

/*****************************************************************************/
// 	Description : set timer msg on
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsTimerMode(BOOLEAN is_timer_mode)
{
    MMISMS_SetIsTimerMode(is_timer_mode);
}

/*****************************************************************************/
// 	Description : create a window according to timer msg index
//	Global resource dependence : 
//  Author: hongbing.ju
//	Note: if index == MMISMS_TIMERMSG_MAX_ITEM create a timer msg interface with current time
//        otherwise   create a timer msg interface with timer msg info from NV
/*****************************************************************************/
PUBLIC void MMIAPISMS_CreateTimerMsgWin(void)
{
    MMISMS_EnterTimerMsgMainWin();
}

/*****************************************************************************/
// Description : Check Timer Msg
// Global resource dependence : none
// Author: 
// Note:must Implement after sms sending/ussd/ss/cc/pdp/fly mode disable and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_TIMERMSG_CheckTimerMSG(BOOLEAN is_direct)
{
    return MMISMS_TIMERMSG_CheckTimerMSG(is_direct);
}

/*****************************************************************************
// 	Description : set box type info for current timer message
// 	Global resource dependence : sort table index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC void MMIAPISMS_TIMERMSG_SetCurrBoxType(MMISMS_BOX_TYPE_E box_type)
{
    MMISMS_TIMERMSG_SetCurrBoxType(box_type);
}
#endif

#if 0
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetCurSendDualSys(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_SetCurSendDualSys(dual_sys);
}


/*****************************************************************************/
//     Description : to clear the id of destinations in g_mmisms_deliver
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ClearDestId(void)
{
    MMISMS_ClearDestId();
}
#endif
/*****************************************************************************/
//  Description : init update info
//  Global resource dependence : g_mmisms_global
//  Author:rong.gu
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPISMS_InitUpdateInfo(void)
{
    MMISMS_InitUpdateInfo();
}

#if 0
/*****************************************************************************/
//  Description : to set the resend flag
//  Global resource dependence : g_mmisms_deliver
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetDeliverResendFlag(BOOLEAN     is_resend)
{
    MMISMS_SetDeliverResendFlag(is_resend);
}
#endif

#if defined MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description :发送Vcard短信
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISMS_WintabSendVcard(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_WintabSendVcard(dual_sys);
}
#endif
#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description :发送Vcalendar短信
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISMS_WintabSendVcalendar(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_WintabSendVcard(dual_sys);
}
#endif

/*****************************************************************************/
//     Description : to get the length according the alpha of message 
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetLenByAlpha(
                                 MN_SMS_ALPHABET_TYPE_E    alpha,            //IN:
                                 uint8                     *split_len_ptr,    //OUT: the len to split message
                                 uint8                     *limit_len_ptr,    //OUT: the limit len
                                 BOOLEAN                   is_head_has_port
                                 )
{
    MMISMS_GetLenByAlpha(alpha,split_len_ptr,limit_len_ptr,is_head_has_port);
}

/*****************************************************************************/
//     Description : to set the g_mmisms_global.operation.cur_order_index
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void    MMIAPISMS_SetCurOrderArrIndex(int index)
{
    MMISMS_SetCurOrderArrIndex(index);
}

/*****************************************************************************/
//     Description : Compute Current Order Id
//    Global resource dependence : g_mmisms_global
//  Author:liming.deng
//    Note:  
/*****************************************************************************/
PUBLIC MMISMS_STATE_T* MMIAPISMS_GetCurOperationOrderId(void)
{
    return MMISMS_GetCurOperationOrderId();
}

#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************
// 	Description : get current smart event instance if exists
// 	Global resource dependence : current global timer msg index
// 	Author: 
// 	Note:
*****************************************************************************/
PUBLIC MMIACC_SMART_EVENT_FAST_TABLE_T* MMIAPISMS_TIMERMSG_GetCurEvent(void)
{
    return MMISMS_TIMERMSG_GetCurEvent();
}
#endif
#if 0
/*****************************************************************************/
//     Description : set dest dual sys
//    Global resource dependence : g_mmisms_deliver
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
void MMIAPISMS_SetDestDualSys( MN_DUAL_SYS_E dual_sys )
{
    MMISMS_SetDestDualSys(dual_sys);
}
#endif
/*****************************************************************************/
//     Description : open the SMS sending window
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenSendingWindow(MN_DUAL_SYS_E  dual_sys,MMI_STRING_T    *prompt_ptr)
{
    MMISMS_OpenSendingWindow(dual_sys,prompt_ptr);
}

/*****************************************************************************/
//     Description : get the sms is in the draft box
//    Global resource dependence : 
//  Author: liming.deng
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_MsgIsInDraftBox(void)
{
    return MMISMS_MsgIsInDraftBox();
}

/*****************************************************************************/
//     Description : to get the destation number according the pointed position
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_GetPointedDestNum(                            //RETURN: 
                                        uint16     index,            //IN: start from 0
                                        uint8        *num_len_ptr,    //OUT:
                                        uint8        *num_ptr        //OUT:
                                        )
{
    return MMISMS_GetPointedDestNum(index,num_len_ptr,num_ptr);
}
#if 0
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetCurSaveDualSys(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_SetCurSaveDualSys(dual_sys);
}
#endif

/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetCurReadMsgDualSys(void)
{
    return MMISMS_GetCurReadMsgDualSys();
}



#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_IsChatEditMMS(void)
{
    BOOLEAN ret = FALSE;
    if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID) && (MMISMS_INSERT_INIT != MMISMS_GetChatEditInsertOpType()))
    {
        ret = TRUE;
    }
    return ret;
}
#endif

#ifdef MMI_SMS_REPLY_PATH_SUPPORT
/*****************************************************************************/
//     Description : Set Answer SMS Flag
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetIsAnwserSMS(BOOLEAN is_answer)
{
    g_mmisms_global.is_answer_sms = is_answer;
}
#endif
#endif

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : set current operating voice mail dual sys
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenPushMsgBox(void)
{
    MMIAPISMS_ExitFromSMS();
    MMISMS_EnterPushMessageWin();
}
#endif

#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : SMS备份函数
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_IKeyBackup(MMIIKB_SIG_BACKUP_ITEM_DATA_REQ_T *backup_data_ptr)
{
    MMISMS_IKeyBackup(backup_data_ptr);
}
/*****************************************************************************/
//  Description : SMS还原函数
//  Global resource dependence : none
//  Author: minghu.mao
//  Note: 
/***************************************************************************/
PUBLIC void MMIAPISMS_IKeyRestore(MMIIKB_SIG_RESTORE_ITEM_DATA_REQ_T *restore_data_ptr)
{
    MMISMS_IKeyRestore(restore_data_ptr);
}
#endif
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : get read msg content
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_GetReadMsgContent(
                                    MMISMS_READMSG_CONTENT_T *read_content_ptr      //OUT
                                    )
{
    MMISMS_GetReadMsgContent(read_content_ptr);
}
#endif

/*****************************************************************************/
//     Description : to get the amount of the destination number
//    Global resource dependence : g_mmisms_deliver
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC uint8   MMIAPISMS_GetDestCount( void )
{
    return MMISMS_GetDestCount();
}

/*****************************************************************************/
//  Description : get the edit content from global API
//  Global resource dependence : g_mmisms_global
//  Author:minghu.mao
//  Note:该结构在sms编辑时候用
/*****************************************************************************/
PUBLIC MMISMS_SMSEDIT_CONTENT_T * MMIAPISMS_GetEditContentFromGlobal( void )
{
    return MMISMS_GetEditContentFromGlobal();
}

/*****************************************************************************/
//  Description : Select SIM API of SMS app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISMS_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC	MMISMSSelectSIMWinCallback,
                                        ADD_DATA    add_data
                                        )
{
    return MMISMS_SelectSIMFunc(MMISMSSelectSIMWinCallback,add_data,TRUE);
}

/*****************************************************************************/
//     Description : get dest dual sys
//    Global resource dependence : 
//  Author:bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E    MMIAPISMS_GetDestDualSys( void )
{
    return MMISMS_GetDestDualSys();
}

/*****************************************************************************/
//     Description : get current operating voice mail dual sys
//    Global resource dependence : 
//  Author: bruce.chi
//    Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISMS_GetCurSendDualSys(void)
{
    return MMISMS_GetCurSendDualSys();
}

/* ----------------------------------------------------------------------------
* Function Name: MMISMS_GetPBNameByNumber
* Purpose:通过电话得到电话所属姓名
* Input:  uint8 *number_ptr:电话号码指针
MMI_STRING_T *name_pt:姓名结构
* Output: BOOLEAN.
* Returns:
* Author:
* ----------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPISMS_GetNameByNumberFromPB(uint8 *number_ptr, MMI_STRING_T *name_ptr, uint16 max_name_len)
{
    return MMISMS_GetNameByNumberFromPB(number_ptr,name_ptr,max_name_len);
}

/*****************************************************************************/
//     Description : to Enter the list of box
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_EnterSmsBox(
                       MMISMS_BOX_TYPE_E        box_type
                       )
{
    MMISMS_EnterSmsBox(box_type);
}

/*****************************************************************************/
//     Description : to get the count of msg(sms mms wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetFirstUnreadMsgId(void)
{
    return MMISMS_GetFirstUnreadMsgId();
}

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//     Description : to get the count of msg(wap push) which is to be read
//    Global resource dependence : s_mmisms_order
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetFirstUnreadPushMsgId(void)
{
    return MMISMS_GetFirstUnreadPushMsg();
}

/*****************************************************************************/
//     Description : open push box
//    Global resource dependence :
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenPushBox(void)
{
    MMISMS_EnterPushMessageWin();
}
#endif


/*****************************************************************************/
//     Description : get the count of msg(sms\wap push\mms\..)for widget
//    Global resource dependence : 
//  Author:Cheng.Luo
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMS_GetAllUnreadMsgCount()
{
    return MMISMS_GetAllUnreadMsgCount();
}

/*****************************************************************************/
//     Description : to get the save position
//    Global resource dependence : g_mmisms_global
//  Author: liqing.peng
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_SMS_SAVEPOS_TYPE_E MMIAPISMS_GetSavePos(void)
{
    return MMISMS_GetSavePos();
}

/*****************************************************************************/
//     Description : set the SMS num of the read message
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
PUBLIC MMISMS_BOX_TYPE_E MMIAPISMS_GetCurBoxType(void)
{
    return MMISMS_GetCurBoxType();
}

/*****************************************************************************/
//  Description :sms edit text property
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_SetEditTextProperty(MMI_CTRL_ID_T ctrl_id)
{
    MMISMS_SetEditTextProperty(ctrl_id);
}

/*****************************************************************************/
//  Description :SmsBCDToPbBCD
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_BCD_NUMBER_T MMIAPISMS_SmsBCDToPbBCD(MMISMS_BCD_NUMBER_T sms_num)
{
    return MMISMS_SmsBCDToPbBCD(sms_num);
}

/*****************************************************************************/
//  Description :Get not download Sms Image-Id 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetUndownLoadImageId(void)
{
    return IMAGE_SMS_MMS_NOTIFY_NEW_ICON;
}

/*****************************************************************************/
//  Description :Get Not-Read-Sms Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadSmsImageId(void)
{
    return IMAGE_SMS_NOT_READ_SMS_ICON;
}

/*****************************************************************************/
//  Description :Get Not-Read-Mms Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadMmsImageId(void)
{
    return IMAGE_SMS_NOT_READ_MMS_ICON;
}

/*****************************************************************************/
//  Description :Get Not-Read-WAPPUSH Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadWAPPUSHImageId(void)
{
    return IMAGE_SMS_WAPPUSH_ICON;
}

/*****************************************************************************/
//  Description :Get Not-Read-WAPOTA Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetNotReadWAPOTAImageId(void)
{
    return IMAGE_SMS_WAPOTA_ICON;
}

/*****************************************************************************/
//  Description :Get FULL_MESSAGE Image-Id 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISMS_GetFullMessageImageId(void)
{
    return IMAGE_SMS_TITLE_ICON_FULL_MESSAGE;
}

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
/*****************************************************************************/
//     Description : to Set the concatenated sms
//    Global resource dependence : none
//  Author:wancan.you
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_SetLocalSettings(MN_DUAL_SYS_E dual_sys)
{
    return MMISMS_SetLocalSettings(dual_sys);
}
#endif

/*****************************************************************************/
//     Description :delete all SMS item in NV by MMI
//    Global resource dependence :  
//  Author:jian.ma
//    Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMIAPISMS_NV_DelAllSmsItem(                                            //RETURN:
                                MN_SMS_RECORD_ID_T    sms_in_me_valid_num    //IN:
                                )
{
    return MMISMS_NV_DelAllSmsItem(sms_in_me_valid_num);
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description :delete all SMSCB item in NV by MMI
//    Global resource dependence :  
//  Author:
//    Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMIAPISMS_NV_DelAllSmsCBItem(                                            //RETURN:
                                uint16    smscb_in_me_valid_num    //IN:
                                )
{
    return MMISMS_NV_DelAllSmsCBItem(smscb_in_me_valid_num);
}
#endif

/*****************************************************************************/
//     Description :get normal close
//    Global resource dependence :  
//  Author:rong.gu
//    Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMIAPISMS_GetNormalClose(void)
{
    return g_mmisms_global.is_sms_normal_close;
}

/*****************************************************************************/
//     Description :delete all SMS item in NV by MMI
//    Global resource dependence :  
//  Author:rong.gu
//    Note:
/*****************************************************************************/    
PUBLIC void MMIAPISMS_SetNormalClose(BOOLEAN is_sms_normal_close)
{
    g_mmisms_global.is_sms_normal_close = is_sms_normal_close;
}

/*****************************************************************************/
//     Description : insert string to sms edit
//    Global resource dependence : none
//  Author:
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPISMS_InsertMessage(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             )
{
    BOOLEAN result = FALSE;

#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if(MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
        result = GUIEDIT_InsertString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, wstr_ptr, wstr_len);                                            		
    }
    MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID, MSG_FULL_PAINT, PNULL);
#else
    if(MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID))
    {
        result = GUIEDIT_InsertString(MMISMS_EDIT_SMS_TEXT_CTRL_ID, wstr_ptr, wstr_len);                                            		
    }
    MMK_SendMsg(MMISMS_EDIT_SMS_WIN_ID, MSG_FULL_PAINT, PNULL);
#endif

    //SCI_TRACE_LOW:"[MMISMS ATC] MMIAPISMS_InsertMessage result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_25180_112_2_18_2_55_12_841,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//     Description :  check and show new msg prompt when exsit
//    Global resource dependence : 
//  Author:taiping.lai
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_CheckAndShowNewMsg(void)
{
    if (MMK_IsOpenWin(MMISMS_NEWMSG_WIN_ID) 
        && !MMK_IsFocusWin(MMISMS_NEWMSG_WIN_ID))
    {
        MMK_SetWinPriority(MMK_ConvertIdToHandle(MMISMS_NEWMSG_WIN_ID), WIN_ONE_LEVEL);
        MMK_WinGrabFocus(MMISMS_NEWMSG_WIN_ID);
    }
#ifdef MMS_SUPPORT
    else if (MMK_IsOpenWin(MMIMMS_NEWMSG_WIN_ID) 
        && !MMK_IsFocusWin(MMIMMS_NEWMSG_WIN_ID))
    {
        MMK_SetWinPriority(MMK_ConvertIdToHandle(MMIMMS_NEWMSG_WIN_ID), WIN_ONE_LEVEL);
        MMK_WinGrabFocus(MMIMMS_NEWMSG_WIN_ID);
    }
#endif
}

/*****************************************************************************/
//  Description : Enter Send Option Menu
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_EnterSendOptionMenu(uint32 send_type)
{
    MMIAPISMS_ExitFromSMS();
            
    if(MMK_IsOpenWin(MMISMS_SENDNUMOPTION_WIN_ID))
    {
        MMK_CloseWin(MMISMS_SENDNUMOPTION_WIN_ID);
    }
    MMISMS_SetDestDualSys(MN_DUAL_SYS_MAX);
    MMISMS_EnterSendNumOptWin(send_type);
}

/*****************************************************************************/
//  Description : Get the push sms SL Operation
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_SL_OPERATION_E MMIAPISMS_GetPushSmsSLOperation(void)
{
    return MMISMS_GetPushSmsSLOperation();
}

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
//     Description : Reset Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMS_ResetVMInfo(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_ResetVMInfo(dual_sys);
}
#endif

#ifdef MMS_SUPPORT
/*****************************************************************************/
//     Description : Reset Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMS_ReadMMSNotify(void)
{
    MMISMS_ORDER_ID_T   cur_order_id = PNULL;
    uint16 undownloadnum = 0;
    BOOLEAN ret = FALSE;
    undownloadnum = MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX);
    if(1 == undownloadnum)
    {
        cur_order_id = MMISMS_GetUndownLoadHeadOrder();     
        if(cur_order_id != PNULL)
        {           
            MMIAPIMMS_ReadMMS(cur_order_id->record_id);
            ret = TRUE;
        }      
        else
        {
            SCI_TRACE_LOW("MMISMS no undown mms");
            ret = FALSE;
        }
    }   
    else
    {
#ifdef  MMISMS_CHATMODE_ONLY_SUPPORT
       MMISMS_EnterSmsBox(MMISMS_BOX_CHAT);
#else
       MMISMS_EnterSmsBox(MMISMS_BOX_MT);
#endif
       ret = TRUE;
    }
    return ret;
}

/*****************************************************************************/
//    Description : 
//    Global resource dependence :
//    Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_ID_T MMIAPISMS_GetUndownLoadHeadOrder(void)
{
    return MMISMS_GetUndownLoadHeadOrder(); 
}
#endif


/*****************************************************************************/
//  Description : open SMS/mms setting window      
//  Global resource dependence : 
//  Author: yuguang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenSettingWin(void)
{
	MMISMS_OpenMessageSetting();
}
