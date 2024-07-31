/****************************************************************************
** File Name:      mmicc_main.c                                  *
** Author:     samboo.shen                                                            *
** Date:           2/6/2012                                             *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2/2012        samboo.shen         Create
****************************************************************************/
#define _MMICC_MAIN_C_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cc_trc.h"
#ifdef __cplusplus
extern   "C"
{
#endif
    
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_applet_table.h"
#include "mmi_appmsg.h"
#include "mmk_app.h"

#include "mmi_default.h"
#include "mmiphone_export.h"

#include "mmicc_internal.h"
#include "mmicc_audio.h"
#include "mmicl_internal.h"
#ifdef MOTION_SENSOR_TYPE_OPTICAL
#include "mmk_opsensor.h"
#endif
#include "mmiidle_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//global variable declare
MMI_APPLICATION_T			g_cc_app;      // g_cc_app��ʵ�嶨��

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register CC module applet info
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void);

/*****************************************************************************/
//  Description : the process message function of the CC
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  CCApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    );

/*****************************************************************************/
//  Description : reg cc MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_Initialize(MMICC_INSTANCE_T *self, DPARAM param);

/*****************************************************************************/
//  Description : reg cc MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_Finalize(MMICC_INSTANCE_T *self, DPARAM param);

/*****************************************************************************/
//  Description : HandleHeadsetButtonUp
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL void HandleHeadsetButtonUp(void);

/*****************************************************************************/
//  Description : HandleHeadsetButtonLong
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL void HandleHeadsetButtonLong(void);

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                               *
**--------------------------------------------------------------------------*/

LOCAL MMICC_APPLET_T *s_cc_applet_instance = PNULL; /* Temp*/

//applet������
LOCAL const CAF_STATIC_APPLET_T s_mmicc_applet_array[] = 
{
    {   
        GUID_NAME_DEF(SPRD_CC_APPLET_ID), 
        sizeof(MMICC_APPLET_T), 
        CCApplet_HandleEvent , 
#ifndef MMI_RES_LOW_COST
    	IMAGE_SECMENU_ICON_CALL,
#else
    	IMAGE_NULL, 
#endif    	
        TXT_TELEPHONE
    },
};

//applet����Ϣ, ��mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmicc_applet_info = 
{
    (CAF_STATIC_APPLET_T*)s_mmicc_applet_array,
    ARR_SIZE(s_mmicc_applet_array)
};

/*****************************************************************************/
// 	Description : register www module applet info
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_CC, &g_mmicc_applet_info );
}


extern  BOOLEAN poweron_time_isto_record;
#ifdef SOS_KEYLONGWEB

extern BOOLEAN  sos_is_connected ;
extern uint8    sos_times;
extern BOOLEAN  sos_temp;
#endif

/*****************************************************************************/
//  Description : the process message function of the CC
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  CCApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMICC_APPLET_T* cc_applet_ptr = (MMICC_APPLET_T*)iapplet_ptr;
    
    if(MSG_TIMER != msg_id)
    {
        //SCI_TRACE_LOW:"ccapp Applet_HandleEvent msg_id = 0x%04x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_123_112_2_18_2_10_33_290,(uint8*)"d",msg_id);
    }
    
    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_cc_applet_instance = cc_applet_ptr;
        CC_Initialize((&(cc_applet_ptr->cc_instance)), param);
        MMK_SetAppletState(cc_applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_DISABLE_RETURN_IDLE, TRUE);
        MMICC_ConstructAudioHandle();
#ifdef MOTION_SENSOR_TYPE_OPTICAL
        SCI_TRACE_LOW("optical sensor IN CC state start");
        MMK_OpenOPsensor(VIRTUAL_WIN_ID);
#endif
        break;
        
    case MSG_APPLET_STOP: 
        CC_Finalize((&(cc_applet_ptr->cc_instance)), param);
        s_cc_applet_instance = PNULL;
        MMICC_DestroyAudioHandle();
        if(!MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)
#ifdef VIDEO_PLAYER_SUPPORT
            && !MMIAPIVP_IsVideoDisplaying()
#endif
            )
        {
            MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        }
#ifdef MOTION_SENSOR_TYPE_OPTICAL
        SCI_TRACE_LOW("optical sensor IN CC state stop");
        MMK_CloseOPsensor(VIRTUAL_WIN_ID);
#endif

#ifdef MMI_ETWS_SUPPORT        
        if( TRUE == MMIAPISMSCB_GetIsNeedRedisplay() )
        {
            MMIAPISMSCB_ShowNewETWSPrompt();
        }
#endif

        break;

    case MSG_APPLET_SUSPEND:
        MMK_SetAppletState(cc_applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
//#ifdef MMI_CALL_MINIMIZE_SUPPORT         
        MMICC_GetAppletInstance()->cc_instance.is_minimize = TRUE;
//#endif
        break;

//#ifdef MMI_CALL_MINIMIZE_SUPPORT 
    case MSG_APPLET_RESUME:
        MMICC_GetAppletInstance()->cc_instance.is_minimize = FALSE;
        break;
//#endif        

    case MSG_APPLET_SWITCH:
        MMK_SetAppletState(cc_applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_BG_HIDE_WIN, FALSE);        
        break;

#ifdef MMI_CALL_MINIMIZE_SUPPORT
    case MSG_CC_RELEASE_ALL_CALL:
        MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);
        break;
#endif        

    case MSG_TIMER:
        if (s_cc_applet_instance->cc_instance.call_time_count_timer == *(uint8*)param)
        {
            MMICC_CaliberationSetCallTimeCount();//add for msg_timer can not work exactly
            //MMICC_SetCallTimeCount(MMICC_GetCallTimeCount() + 1);
            if(MMK_IsFocusWin(MMICC_STATUS_WIN_ID))
            {
                MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_TIMER, param);
            }
            else if(MMK_IsFocusWin(MMICC_ANIMATION_WIN_ID))
            {
                MMK_SendMsg(MMICC_ANIMATION_WIN_ID, MSG_TIMER, param);
            }
            if(MMIIDLE_HaveNewNotification(MMI_IDLE_PUSH_MSG_ONGOING_CALL))
            {
                MMICC_UpdateOngoingCallIdleMsg();
            }
        }
        /*else if (s_cc_applet_instance->cc_instance.dtmf_sending_timer == *(uint8*)param)
        {
            MMIAPICC_StopDtmf(MN_DUAL_SYS_MAX);
        }*/
        break;
    case MSG_KEYUP_RED:
        result = MMI_RESULT_TRUE;
        break;
    case MSG_APP_RED:
#if defined(KEYPAD_TYPE_FOUR_KEY_UNIVERSE) && defined(MMI_CALL_MINIMIZE_SUPPORT)//@tao.xue  renwei
        if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) && MMK_IsFocusWin(MMICC_STATUS_WIN_ID))
        {
            MMIAPICC_HideCCApplet();
            MMK_ReturnIdleWin();
        }
        else
        {
            MMK_DispMsgToWin(msg_id, param);
        }
#else
        //���������˻�IdleWin
        MMIAPICC_ReleaseCallByRedkey();		
#endif       
        break;

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYUP_FLIP:
        {
            BOOLEAN is_flip_answer = MMIAPISET_GetIsFlipAnswer(MMIAPICC_GetCallDualSys());//@fen.xie MS00175075
            MMIDEFAULT_HandleFlipKey(TRUE);
            if(is_flip_answer)
            {
                MMIAPICC_StopMtCallPlayingRing();
                if(!MMICC_IsCurrentCallIDValid() || MMICC_IsExistOperatedCall() )
                {
                    //can not do other handle in this state
                    break;
                }
                if(MMICC_GetCurrentCallStatus()== CC_INCOMING_CALL_STATE)
                {
                    if(!MMICC_AnswerCall())			
                    {
                        //????
                    }
                    //MMK_CloseWin( MMICC_ANIMATION_WIN_ID );
                }
            }		     
        }
        break;
#endif

    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
        //Answer the Phone
        if (MMK_IsOpenWin(MMICC_ANIMATION_WIN_ID))
        {
            MMK_SendMsg(MMICC_ANIMATION_WIN_ID, msg_id, NULL);
        }
        break;

    case MSG_CC_STOP_APPLET:
        if (!MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
            && !MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
            && !MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))//for STK hold redial
        {
            MMICC_StopCCApplet();
			MMIAPIIDLE_CloseDialWin();//605
			if(MMIENG_GetFirstTimeIsrecord() == 0)
			{
				if(poweron_time_isto_record)
				{
					MMIENG_RecordFirstTime();
					poweron_time_isto_record = FALSE ;
				}
			}
	#ifdef SOS_KEYLONGWEB
				 
				 //Trace_Log_Print("sos_is_connected=%d,sos_times =%d,sos_temp=%d",sos_is_connected,sos_times,sos_temp);
		//	if(sos_times > 1 && sos_temp)
		//	{
		//		sos_temp = FALSE;
		//	}
				SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	0000");
		
				if(sos_times > 8 && sos_temp)
				{
					SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	1111");
					sos_temp = FALSE;
				}
				if((!sos_is_connected) && sos_temp)
				{
		
					//Trace_Log_Print("2222sos_is_connected=%d,sos_times =%d,",sos_is_connected,sos_times);
					SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	2222");
					if(sos_times == 0)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	333");
						SOS_EmergencyCallWin();
					}
					else if(sos_times == 1)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	444");
						SOS_EmergencyCallWin2();
					}
					else if(sos_times == 2)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	55555");
						SOS_EmergencyCallWin3();
					}
					else if(sos_times == 3)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	66666");
						SOS_EmergencyCallWin();
					}
					else if(sos_times == 4)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	7777");
						SOS_EmergencyCallWin2();
					}
					else if(sos_times == 5)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	88888");
						SOS_EmergencyCallWin3();
					}
					else if(sos_times == 6)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	999999");
						SOS_EmergencyCallWin();
					}
					else if(sos_times == 7)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	zzzzzz");
						SOS_EmergencyCallWin2();
					}
					else if(sos_times == 8)
					{
						SCI_TRACE_MID(" zdthero000	  APP_MN_CALL_DISCONNECTED_IND	xxxxxx");
						SOS_EmergencyCallWin3();
					}
					
				}
				//else if((!sos_is_connected) && (2 < sos_times && sos_times < 6) && sos_temp)
			//{
				//	SOS_EmergencyCallWin2();
				//}
				//else if((!sos_is_connected) && (5 < sos_times &&	sos_times < 9) && sos_temp)
				//{
				//	SOS_EmergencyCallWin3();
				//} 
#endif
		
        }
        break;

    case (MMISRVAUD_EARPHONE_EVENT_CLK + KEY_HEADSET_BUTTIN):  // �м�����
        HandleHeadsetButtonUp();                            
        break;  

    case (MMISRVAUD_EARPHONE_EVENT_LONG + KEY_HEADSET_BUTTIN): // �м�����
        HandleHeadsetButtonLong();
        break;
 
    case (MMISRVAUD_EARPHONE_EVENT_DBLCLK + KEY_HEADSET_BUTTIN): // �м�˫��
         //do nothing
         break;
        

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : init the cc application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMIAPICC_Init(void)
{
#ifdef SIM_PLUG_IN_SUPPORT 
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
#endif
    //define call back function and component type of CC
    g_cc_app.ProcessMsg = MMICC_HandlePsMsg;
    //    g_cc_app.component_type = CT_APPLICATION;
    
    //��ʼ��ȫ�ֱ���
    MMICC_Init_Global();
    //s_is_open_auto_dial_win = FALSE;
    
#ifdef SIM_PLUG_IN_SUPPORT     
    sim_plug_notify_info.module_id = MMI_MODULE_CC;
    sim_plug_notify_info.notify_func = MMICC_HandleSIMPlugNotifyCCFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif

#if defined(IKEYBACKUP_SUPPORT)
    //cl���δ�忨���������mmicl_init
    MMICL_BackupRegister();
#endif
}

/*****************************************************************************/
//  Description : reg cc, cl menu group and nv group
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_InitModule(void)
{
    MMICC_RegMenuGroup();
    MMICC_RegNv();
    MMICC_RegWinIdNameArr();
    RegAppletInfo();
}

/*****************************************************************************/
//  Description : CC_Initialize
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_Initialize(MMICC_INSTANCE_T *self, DPARAM param)
{
    //SCI_TRACE_LOW:"ccapp CC_Initialize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_227_112_2_18_2_10_33_291,(uint8*)"");
#ifndef MMI_GUI_STYLE_TYPICAL
    SCI_MEMSET(&self->cc_win_button_order, 0x00, sizeof(self->cc_win_button_order));
    self->cc_display_type = CC_DISPLAY_NONE;
#ifndef MMI_CALLCONTROL_MINI_SUPPORT    
    self->mmicc_disconnect_timer_for_display = 0;
#endif
    self->mmicc_move_unlock_icon_timer = 0;
#else
    self->current_win_button_order_ptr = PNULL;
#endif

#if !defined(TOUCH_PANEL_SUPPORT)
    self->cc_win_focus_button_type = 0;
#endif

    SCI_MEMSET(&self->autodial_info, 0x00, sizeof(self->autodial_info));
    SCI_MEMSET(&self->call_repeat_info, 0x00, sizeof(self->call_repeat_info));
    self->mmicc_disconnect_timer = 0;
    self->dtmf_timer_id = 0;
    self->call_end_hint_timer = 0;
    self->is_play_call_end_hint = FALSE;
    SCI_MEMSET(&self->sim_status, 0x00, sizeof(self->sim_status));    
    self->waiting_call_ring_timer = 0;
    self->call_time_count_timer = 0;
    self->dtmf_sending_timer = 0;
    self->is_need_light_on_cc_win = FALSE;
    self->is_dtmf_switch = TRUE;
    self->call_end_index = 0;
#ifdef MMI_CALL_MINIMIZE_SUPPORT    
    self->is_minimize = FALSE;
#endif
#ifdef PLAY_BUSY_TONE_FOR_UNISOC
    self->busy_tone_timer = 0;
    self->is_play_busy_tone = FALSE;
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : cc_Finalize
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_Finalize(MMICC_INSTANCE_T *self, DPARAM param)
{
    //SCI_TRACE_LOW:"ccapp CC_Finalize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_263_112_2_18_2_10_33_292,(uint8*)"");
    if(MMI_NONE_TIMER_ID !=  self->call_time_count_timer )
    {
        MMK_StopTimer( self->call_time_count_timer );
        self->call_time_count_timer = MMI_NONE_TIMER_ID;
    }
    if(MMI_NONE_TIMER_ID !=  self->call_end_hint_timer)
    {
        MMK_StopTimer( self->call_end_hint_timer );
        self->call_end_hint_timer = MMI_NONE_TIMER_ID;
    }
#ifdef PLAY_BUSY_TONE_FOR_UNISOC
    if(MMI_NONE_TIMER_ID !=  self->busy_tone_timer)
    {
        MMK_StopTimer( self->busy_tone_timer );
        self->busy_tone_timer = MMI_NONE_TIMER_ID;
    }
#endif
    if(MMI_NONE_TIMER_ID !=  self->mmicc_disconnect_timer)
    {
        MMK_StopTimer( self->mmicc_disconnect_timer );
        self->mmicc_disconnect_timer = MMI_NONE_TIMER_ID;
    }
    if(MMI_NONE_TIMER_ID !=  self->dtmf_sending_timer)
    {
        MMK_StopTimer( self->dtmf_sending_timer );
        self->dtmf_sending_timer = MMI_NONE_TIMER_ID;
    }
     if(MMI_NONE_TIMER_ID !=  self->dtmf_timer_id)
    {
        MMK_StopTimer( self->dtmf_timer_id );
        self->dtmf_timer_id = MMI_NONE_TIMER_ID;
    }
#ifndef MMI_GUI_STYLE_TYPICAL
    if(MMI_NONE_TIMER_ID !=  self->mmicc_move_unlock_icon_timer)
    {
        MMK_StopTimer( self->mmicc_move_unlock_icon_timer );
        self->mmicc_move_unlock_icon_timer = MMI_NONE_TIMER_ID;
    }
    if(MMI_NONE_TIMER_ID !=  self->mmicc_anim_unlock_icon_timer)
    {
        MMK_StopTimer( self->mmicc_anim_unlock_icon_timer );
        self->mmicc_anim_unlock_icon_timer = MMI_NONE_TIMER_ID;
    }
#ifndef MMI_CALLCONTROL_MINI_SUPPORT    
    if(MMI_NONE_TIMER_ID !=  self->mmicc_disconnect_timer_for_display)
    {
        MMK_StopTimer( self->mmicc_disconnect_timer_for_display );
        self->mmicc_disconnect_timer_for_display = MMI_NONE_TIMER_ID;
    }
#endif    

#endif
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Get CC Applet Instance
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:���Է���PNULL�����ú�����Ҫ���ж�
/*****************************************************************************/
PUBLIC MMICC_APPLET_T *MMICC_GetAppletInstance(void)
{
    return s_cc_applet_instance;
}

/*****************************************************************************/
//  Description : Start cc applet 
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StartCCApplet(void)
{
    MMI_APPLET_START_T start ={0};
    uint32 user_data = 0;
    //SCI_TRACE_LOW:"ccapp StartCCApplet"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_289_112_2_18_2_10_33_293,(uint8*)"");
    start.guid = SPRD_CC_APPLET_ID;
    start.state = MMI_APPLET_STATE_TASK_MANAGER;
    start.param_ptr = &user_data;
    start.size_of_param = sizeof(uint32);
    MMK_StartApplet( &start );
}

/*****************************************************************************/
//  Description : Stop cc applet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StopCCApplet(void)
{

    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    
    //SCI_TRACE_LOW:"ccapp Stop cc applet"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_305_112_2_18_2_10_33_294,(uint8*)"");


    if (PNULL != cc_applet_ptr)
    {
        MMK_CloseApplet(cc_applet_ptr->caf_applet.app_handle);
    }
}

/*****************************************************************************/
//  Description :close codec handle & audio handle when powering off for normally play poweroff audio or video
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in power off procede
/*****************************************************************************/
PUBLIC void MMIAPICC_QuitForPowerOff ( void )
{
    MMICC_EnableVoiceCodec(FALSE);
    MMICC_DestroyVoiceCodec();
    MMICC_DestroyAudioHandle();
    MMICC_StopCCApplet();
}


/*****************************************************************************/
//  Description : MMIAPICC_SendMsgToApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_SendMsgToApplet(MMI_MESSAGE_ID_E msg_id, DPARAM param_ptr)
{

    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    MMI_HANDLE_T cc_handle = PNULL;
    
    //SCI_TRACE_LOW:"MMIAPICC_SendMsgToApplet msg:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_324_112_2_18_2_10_33_295,(uint8*)"d", msg_id);

    if (PNULL != cc_applet_ptr)
    {
        cc_handle = cc_applet_ptr->caf_applet.app_handle;
        MMK_SendMsg(cc_handle, msg_id, PNULL);
    }
}
/*****************************************************************************/
//  Description : HandleHeadsetButtonUp
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL void HandleHeadsetButtonUp(void)
{    
    if(CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
    {
        MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
    }
    else if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()) // just only 1 incoming call
    {
        MMICC_AnswerCall();//MMIAPICC_ConnectCallReq();
    }
    else if(CC_WAITING_STATE == MMICC_GetCurrentCallStatus()) //waiting
    {
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            //do nothing
        }
        else
        {
            MMICC_HoldAndAnswerCW();
        }
    }
    else if(CC_CALL_STATE == MMICC_GetCurrentCallStatus())
    {
#if 0    
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
        }
        else
#endif            
        {
            MMICC_ReleaseActiveCall();//MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
        }
    }
    else if(CC_HOLD_STATE == MMICC_GetCurrentCallStatus())
    {
    	MMICC_ReleaseHoldCall();
    }
        
}

/*****************************************************************************/
//  Description : HandleHeadsetButtonLong
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL void HandleHeadsetButtonLong(void)
{
    if(CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
    {
        MMICC_ReleaseCurrentCallReq(MN_CAUSE_NORMAL_CLEARING);
    }
    else if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()) // just only 1 incoming call
    {
        MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY);
    }
    else if(CC_WAITING_STATE == MMICC_GetCurrentCallStatus()) //waiting
    {
#if 0    
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY);
        }
        else
#endif            
        {
            MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY);
        }
    }
    else if(((CC_CALL_STATE == MMICC_GetCurrentCallStatus()) || (CC_HOLD_STATE == MMICC_GetCurrentCallStatus())))
    {
        if(MMICC_IsExistActiveAndHoldCalls())
        {
            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);
        }
        else
        {
            MMICC_HoldCall();
        }
    }
}
//#ifdef MMI_CALL_MINIMIZE_SUPPORT            
/*****************************************************************************/
//  Description : MMIAPICC_ActiveCCApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: ��cc  applet��ý��㣬����ֵTRUE������cc applet���ڣ�FALSE����ʾapplet������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ActiveCCApplet(void)
{
    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    MMI_HANDLE_T cc_handle = PNULL;
    BOOLEAN result = FALSE;

    if (PNULL != cc_applet_ptr)
    {
        cc_handle = cc_applet_ptr->caf_applet.app_handle;
        if (PNULL != cc_handle)
        {
            if(!MMICC_GetAppletInstance()->cc_instance.is_minimize)
            {
                SCI_TRACE_LOW("ccapp active applet fail as not min");
                result = FALSE;
            }
            else if(MMK_GrabWinFocusByApplet(cc_handle))
            {
                //MMICC_GetAppletInstance()->cc_instance.is_minimize = FALSE;
                result = TRUE;
            }
        }
    }   
    //SCI_TRACE_LOW:"ccapp ActiveCC result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_358_112_2_18_2_10_33_296,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : MMIAPICC_HideCCApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: ��cc  applet���أ�����ֵTRUE������cc applet���ڣ�FALSE����ʾapplet������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_HideCCApplet(void)
{
    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    MMI_HANDLE_T cc_handle = PNULL;
    BOOLEAN result = FALSE;

    if (PNULL != cc_applet_ptr)
    {
        cc_handle = cc_applet_ptr->caf_applet.app_handle;
        if (PNULL != cc_handle)
        {
            if(MMK_HideWinByApplet(cc_handle))
            {
                //MMICC_GetAppletInstance()->cc_instance.is_minimize = TRUE;
                result = TRUE;
            }
        }
    }   
    //SCI_TRACE_LOW:"ccapp HideCC result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_MAIN_384_112_2_18_2_10_33_297,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : get cc applet handle
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsMinimize(void)
{
    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    BOOLEAN is_minimize = FALSE;

    if(PNULL != cc_applet_ptr)
    {
        is_minimize = cc_applet_ptr->cc_instance.is_minimize;
    }

    return is_minimize;
}

/*****************************************************************************/
//  Description : get cc applet handle
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPICC_GetCCAppletHandle(void)
{
    MMICC_APPLET_T* cc_applet_ptr = MMICC_GetAppletInstance();
    if(PNULL == cc_applet_ptr)
    {
        return 0;
    }
    else
    {
        return cc_applet_ptr->caf_applet.app_handle;
    }
}
//#endif//MMI_CALL_MINIMIZE_SUPPORT
    
#ifdef   __cplusplus
    }
#endif
    
