/*****************************************************************************
** File Name:      hellosprd.c                                               *
** Author:                                                                   *
** Date:           4/01/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 4/2009         xiaoqing.lu       Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "nps_main.h"
#include "nps_nv.h"
#include "nps_export.h"
#include "mn_type.h"
#include "mmi_nv.h"
#include "mmk_timer.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "nps_id.h"
#include "nps_text.h"
#include "window_parse.h"
#include "guitext.h"
#include "mmidisplay_data.h"
#include "mmi_position.h"
#include "mmiconnection_export.h"
#include "in_message.h"
#include "nps_http.h"
#include "mmi_appmsg.h"
#include "version.h"
#include "guirichtext.h"
#include "mmipub.h"
#ifndef WIN32
#ifdef MS_DEVICE_INFO_SUPPORT
#include "device_info.h"
#endif
#endif

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define USE_SHA1

#define min(a,b)	(((a) < (b)) ? (a) : (b))

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
        
        /* blk0() and blk() perform the initial expand. */
        /* I got the idea of expanding during the round function from SSLeay */
#ifndef WORDS_BIGENDIAN
#define blk0(i) (block->l[i] = (rol(block->l[i], 24) & 0xFF00FF00) | \
        (rol(block->l[i], 8) & 0x00FF00FF))
#else
#define blk0(i) block->l[i]
#endif
#define blk(i) (block->l[i & 15] = rol(block->l[(i + 13) & 15] ^ \
        block->l[(i + 8) & 15] ^ block->l[(i + 2) & 15] ^ block->l[i & 15], 1))
        
        /* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) \
    z += ((w & (x ^ y)) ^ y) + blk0(i) + 0x5A827999 + rol(v, 5); \
        w = rol(w, 30);
#define R1(v,w,x,y,z,i) \
    z += ((w & (x ^ y)) ^ y) + blk(i) + 0x5A827999 + rol(v, 5); \
        w = rol(w, 30);
#define R2(v,w,x,y,z,i) \
        z += (w ^ x ^ y) + blk(i) + 0x6ED9EBA1 + rol(v, 5); w = rol(w, 30);
#define R3(v,w,x,y,z,i) \
    z += (((w | x) & y) | (w & x)) + blk(i) + 0x8F1BBCDC + rol(v, 5); \
        w = rol(w, 30);
#define R4(v,w,x,y,z,i) \
    z += (w ^ x ^ y) + blk(i) + 0xCA62C1D6 + rol(v, 5); \
        w=rol(w, 30);

#define F0(x,y,z)	(z ^ (x & (y ^ z)))
#define F1(x,y,z)	(x ^ y ^ z)
#define F2(x,y,z)	((x & y) | (z & (x | y)))
#define F3(x,y,z)	(x ^ y ^ z)
        
#define STORE64H(x, y) { \
    (y)[0] = (unsigned char)(((x)>>56)&255); \
    (y)[1] = (unsigned char)(((x)>>48)&255); \
    (y)[2] = (unsigned char)(((x)>>40)&255); \
    (y)[3] = (unsigned char)(((x)>>32)&255); \
    (y)[4] = (unsigned char)(((x)>>24)&255); \
    (y)[5] = (unsigned char)(((x)>>16)&255); \
    (y)[6] = (unsigned char)(((x)>>8)&255); \
    (y)[7] = (unsigned char)((x)&255); \
    }

#define STORE32H(x, y) { \
    (y)[0] = (unsigned char)(((x)>>24)&255); \
    (y)[1] = (unsigned char)(((x)>>16)&255); \
    (y)[2] = (unsigned char)(((x)>>8)&255); \
    (y)[3] = (unsigned char)((x)&255); \
    }
 
#define LOAD32H(x, y) { \
    x = ((unsigned long)((y)[0] & 255)<<24) | \
    ((unsigned long)((y)[1] & 255)<<16) | \
    ((unsigned long)((y)[2] & 255)<<8)  | \
    ((unsigned long)((y)[3] & 255)); \
    }
        
#define ROL(x, y) \
    ( (((unsigned long)(x)<<(unsigned long)((y)&31)) | \
    (((unsigned long)(x)&0xFFFFFFFFUL)>>(unsigned long)(32-((y)&31)))) & \
        0xFFFFFFFFUL)

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef struct{
    uint32 state[5];
    uint32 count[2];
    unsigned char buffer[64];
}SHA1_CTX1;

#ifdef USE_SHA1
#define SHA1_HASH_SIZE 20

struct sha1_state {
#ifdef HAVE_NATIVE_INT64
    uint64		length;
#else
    uint32		lengthHi;
    uint32		lengthLo;
#endif /* HAVE_NATIVE_INT64 */
    uint32		state[5], curlen;
    unsigned char	buf[64];
};
#endif /* USE_SHA1 */

typedef union{
    struct sha1_state	sha1; 
}psDigestContext_t;

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
//#define ONE_HOUR_TIMER 60*60*1000
//#define NPS_MAX_COUNT 14*24

#define NPS_EMAIL_ADDR_LEN_MIN   5
#define NPS_EMAIL_ADDR_LEN_MAX   120

#define HTTP_PARAM_SCORE "&score="
#define HTTP_PARAM_FEEDBACK "&feedback="
#define HTTP_PARAM_EMAIL "&email="
#define HTTP_PARAM_COUNTRY "&country="
#define HTTP_PARAM_OPERATOR "&operator="
#define HTTP_PARAM_TIME "&dateEntered="
#define HTTP_PARAM_IMEI "&imei="
#define HTTP_PARAM_SOURCE "&source="
#define HTTP_PARAM_DEVICE  "&device="
#define HTTP_PARAM_DEVICE_WIN32  "&device=-1234"
#define HTTP_PARAM_SW "&sw="
#define HTTP_PARAM_VERSION "&version=1.0"

#define NPS_SOURCE_NOTIFICATION "Notification"
#define NPS_SOURCE_SETTINGS "Settings"

#define MMINPS_AUDIO_SET_BRIMAX  11
#define MMINPS_CURRENT_MAX_TIEM  10

#define  MMINPS_MARGIN_TEXT_LEFT_RIGHT  6
#define  MMINPS_MARGIN_TEXT_TOP_RIGHT   6
/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_nps_app = {0};
LOCAL uint8   s_1h_timer_id = 0;
LOCAL uint16  s_timer_count = 0;
LOCAL BOOLEAN s_is_prompt = FALSE;
LOCAL uint8   s_brightness = 0;// 0;change for india requirement
LOCAL uint8   s_score  =  0;// 0;change for india requirement
LOCAL uint32   s_one_hour_timer = 10*60*1000;// ??
LOCAL uint16   s_nps_max_hour_count = 14*24*6;//????

LOCAL BOOLEAN  s_nps_gray_remain_flag  =  FALSE;//??nps????????,???????,??true
LOCAL BOOLEAN  s_nps_is_email_field_enable = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void NPSTimer_Callback(void);

/*****************************************************************************/
//Description : 
//Global resource dependence : none
//Author:
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleNPSMainWindow(
                                        MMI_WIN_ID_T       win_id, //窗口ID
                                        MMI_MESSAGE_ID_E   msg_id, //消息ID
                                        DPARAM             param   //消息的参数
                                        );

/*****************************************************************************/
//Description : 
//Global resource dependence : none
//Author:
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNPSCompleteWindow(
                                            MMI_WIN_ID_T       win_id, //窗口ID
                                            MMI_MESSAGE_ID_E   msg_id, //消息ID
                                            DPARAM             param   //消息的参数
                                            );

/*****************************************************************************/
//  Description :NPS反馈发送询问窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNPSFeedbackSendWindow(
                                               MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param
                                               );
/*****************************************************************************/
//  Description :NPS Privacy Policy窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNPSCheckPrivacyPolicyWindow(
                                                     MMI_WIN_ID_T win_id,
                                                     MMI_MESSAGE_ID_E msg_id,
                                                     DPARAM param
                                                     );
/*****************************************************************************/
// 	Description : to INITIAL MAIN FORM CONTROL 
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void InitMainFormControl(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : PIN/PUK input window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN NPSMainWinSoftkey(
                                wchar         *str_ptr,   //in
                                uint16        str_len,    //in
                                MMI_TEXT_ID_T *text_id_ptr//in/out:
                                );

/*****************************************************************************/
// 	Description : PIN/PUK input window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void MMINPS_SendFeedBack(void);

/*****************************************************************************/
//  Discription: This function used to process msg comeback form blue tooth
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E AppHandleNPSMsg(
                                   PWND app_ptr,
                                   uint16 msg_id,
                                   DPARAM param
                                   );

/*****************************************************************************/
// Description : Convert str to Escape str(%X)
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: be sure of the range of dst
/*****************************************************************************/
LOCAL char* MMINPS_StrToEscapeStr(char *dst, const char *src);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_IsValidEmailAddress(char *addr);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_IsEmailChar(char c);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_IsAlpha(char c);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_IsDigit(char c);

/*****************************************************************************/
//  Description : sha1 algorithm without key
//  Global resource dependence :  none
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMI_SHA1Hash(const uint8 *src_ptr, uint32 src_length, uint8 *dest_ptr);

/*****************************************************************************/
//  Description : sha1 algorithm with key
//  Global resource dependence :  none
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/
LOCAL void psSha1Init(psDigestContext_t* md);

/*****************************************************************************/
//  Description : sha1 algorithm with key
//  Global resource dependence :  none
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/

LOCAL void psSha1Update(psDigestContext_t * md, const unsigned char *buf, uint32 len);

/*****************************************************************************/
//  Description : sha1 algorithm with key
//  Global resource dependence :  none
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/
LOCAL int32 psSha1Final(psDigestContext_t* md, unsigned char *hash);
/*****************************************************************************/
//  Description : modify MMIPUB_TEXT_CTRL_ID's lineSpace
//  Global resource dependence :  none
//  Author: ak.wang
//  Note: just for fix 1044586
/*****************************************************************************/
LOCAL MMI_RESULT_E MMINPS_ModifyTextCtrlLineSpace( MMI_WIN_ID_T win_id);

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
//NPS主窗口

WINDOW_TABLE( MMINPS_MAIN_WIN_TAB ) = 
{
    WIN_FUNC( (uint32) HandleNPSMainWindow),
    WIN_ID( MMINPS_MAIN_MENU_WIN_ID ),
	WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,NPS_MAIN_FORM_CTRL_ID),	
	WIN_SOFTKEY(STXT_SOFTKEY_MENU_MK, STR_NPS_LIST_SEND_EXT01, TXT_COMMON_COMMON_BACK),
    //1
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_START_TEXT_THIRD_FORM_CTRL_ID,NPS_MAIN_FORM_CTRL_ID),
        CHILD_TEXT_CTRL(FALSE,NPS_EXPLAIN_TEXT_CTRL_ID,NPS_START_TEXT_THIRD_FORM_CTRL_ID),
    //for bug 1053220 : STR_NPS_LIST_0_EXT02 and STR_NPS_LIST_10_EXT02 not display
#if 1
	    CHILD_TEXT_CTRL(FALSE,NPS_SCORE_LABEL_CTRL_ID,NPS_START_TEXT_THIRD_FORM_CTRL_ID),
		//CHILD_PRGBOX_CTRL(TRUE, GUIPRGBOX_STYLE_NPS_SCORE_BAR, NPS_SCORE_PRGBOX_CTRL_ID, NPS_START_TEXT_THIRD_FORM_CTRL_ID),
#endif
        CHILD_EDIT_DIGITAL_CTRL(TRUE, 1000, NPS_SCORE_EDIT_DIGITAL_CTRL_ID, NPS_START_TEXT_THIRD_FORM_CTRL_ID),

    //2
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_SCORE_THIRD_FORM_CTRL_ID,NPS_MAIN_FORM_CTRL_ID),
	    CHILD_TEXT_CTRL(FALSE,NPS_REASON_TEXT_CTRL_ID,NPS_SCORE_THIRD_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE, 60, NPS_REASON_EDIT_TEXT_CTRL_ID, NPS_SCORE_THIRD_FORM_CTRL_ID),
    //3
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_SCORE_RESON_THIRD_FORM_CTRL_ID,NPS_MAIN_FORM_CTRL_ID),
	    CHILD_TEXT_CTRL(FALSE, NPS_MARK_AGREE_CONTACTED_CTRL_ID, NPS_SCORE_RESON_THIRD_FORM_CTRL_ID),
        CHILD_LIST_CTRL(TRUE, GUILIST_CHECKLIST_E, NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, NPS_SCORE_RESON_THIRD_FORM_CTRL_ID),

	//4
	CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, NPS_EMAIL_THIRD_FORM_CTRL_ID,NPS_MAIN_FORM_CTRL_ID),
//	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,NPS_EMAIL_LABEL_CTRL_ID,NPS_EMAIL_THIRD_FORM_CTRL_ID),//特殊语言有错
//        bug498870
        CHILD_TEXT_CTRL(FALSE, NPS_EMAIL_LABEL_CTRL_ID, NPS_EMAIL_THIRD_FORM_CTRL_ID),
	    CHILD_EDIT_TEXT_CTRL(TRUE, 30, NPS_EMAIL_EDIT_TEXT_CTRL_ID, NPS_EMAIL_THIRD_FORM_CTRL_ID),

    //5
	CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, NPS_PRIVACY_THIRD_FORM_CTRL_ID,NPS_MAIN_FORM_CTRL_ID),
	    CHILD_TEXT_CTRL(FALSE,NPS_REMIND_TEXT_CTRL_ID,NPS_PRIVACY_THIRD_FORM_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMINPS_MAIN_SELECT_WIN_TAB) =
{
	WIN_ID(MMINPS_MAIN_SELECT_WIN_ID),
	WIN_HIDE_STATUS,
    END_WIN
};
WINDOW_TABLE(MMINPS_FEEDBACK_SEND_WIN_TAB) =
{
	WIN_FUNC((uint32)HandleNPSFeedbackSendWindow),
	WIN_ID(MMINPS_FEEDBACK_SEND_WIN_ID),
	WIN_HIDE_STATUS,
	WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};
WINDOW_TABLE(MMINPS_FEEDBACK_SENDING_WIN_TAB) =
{
	WIN_ID(MMINPS_FEEDBACK_SENDING_WIN_ID),
	WIN_HIDE_STATUS,
    END_WIN
};
WINDOW_TABLE(MMINPS_INSERT_SIM_ALERT_WIN_TAB) =
{
	WIN_ID(MMINPS_INSERT_SIM_ALERT_WIN_ID),
	WIN_HIDE_STATUS,
    END_WIN
};
WINDOW_TABLE(MMINPS_CHECK_EMAIL_ALERT_WIN_TAB) =
{
	WIN_ID(MMINPS_CHECK_EMAIL_ALERT_WIN_ID),
	WIN_HIDE_STATUS,
    END_WIN
};
WINDOW_TABLE(MMINPS_CHECK_PRIVACY_POLICY_WIN_TAB) =
{
	WIN_FUNC((uint32)HandleNPSCheckPrivacyPolicyWindow),
	WIN_ID(MMINPS_CHECK_PRIVACY_POLICY_WIN_ID),
	WIN_STATUSBAR,
	CREATE_TEXT_CTRL(MMINPS_CHECK_PRIVACY_POLICY_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

//NPS complete窗口

WINDOW_TABLE( MMINPS_COMPLETE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32) HandleNPSCompleteWindow),
    WIN_ID( MMINPS_COMPLETE_WIN_ID ),
	WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_OK_MK, TXT_NULL),
    CREATE_TEXT_CTRL(NPS_COMPLETE_TEXT_ID),
    END_WIN
};
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPINPS_IsTimerArrive(void)
{
    MN_RETURN_RESULT_E    return_value = MN_RETURN_FAILURE;
    BOOLEAN               is_timer_arrive = FALSE;
    
    MMINV_READ(MMINPSNV_IS_TIMER_ARRIVE,&is_timer_arrive,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_timer_arrive = FALSE;
        MMINV_WRITE(MMINPSNV_IS_TIMER_ARRIVE,&is_timer_arrive);
    }
    SCI_TRACE_LOW("[NPS] MMIAPINPS_IsTimerArrive is_timer_arrive = %d", is_timer_arrive);
    return is_timer_arrive;
}

/*****************************************************************************/
// 	Description : prompt NPS select screen
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_PromptNPSJudgeFunction(void)
{
    SCI_TRACE_LOW("MMIAPINPS_PromptNPSJudgeFunction begin");
    if(MMIAPINPS_IsTimerArrive() 
        && (!MMIAPINPS_IsAlreadyPrompt())
        && ( MMIPHONE_IsSimOk(MN_DUAL_SYS_1)
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        || MMIPHONE_IsSimOk(MN_DUAL_SYS_2)
#endif
        )
        && (MMICONNECTION_SETTING_GPRS_SWITCH_ON == MMIAPICONNECTION_GetGPRSSwitchStatus()))
    {
        MMIAPINPS_OpenMainMenuSelectWindow(TRUE);  //[MS]
    }
}

/*****************************************************************************/
// 	Description : change prompt timer for test
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_PromptTimerChange(BOOLEAN is_change_time,
                                                        uint32 one_hour,
                                                        uint16 hour_count)
{
    SCI_TRACE_LOW("[NPS] MMIAPINPS_PromptTimerChange one_hour = %d", one_hour);
    SCI_TRACE_LOW("[NPS] MMIAPINPS_PromptTimerChange hour_count = %d", hour_count);
    if(is_change_time)
   	{  
        if((0 < one_hour) && (one_hour < 0xffffffff) && (0 < hour_count) && (hour_count < 0xffff))
   	   	{
   	   	    s_one_hour_timer = one_hour;
            s_nps_max_hour_count = hour_count;
            if(s_1h_timer_id)
            {
                MMK_StopTimer(s_1h_timer_id);
                s_1h_timer_id = 0;
            }
            s_timer_count = 0;
            MMINV_WRITE(MMINPSNV_TIMER_COUNT,&s_timer_count);
            MMIAPINPS_StartNPSTimer();
            SCI_TRACE_LOW("[NPS] MMIAPINPS_PromptTimerChange setting success");
   	   	}
   	}
}
/*****************************************************************************/
// 	Description : NPS restore factory setting
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_SettingNPSToFactory(void)
{
    ////for bug:476088,476084
    BOOLEAN               is_already_prompt = FALSE;
    BOOLEAN               is_timer_arrive = FALSE;
    //变量初始化
    s_one_hour_timer = 60*60*1000;
    s_nps_max_hour_count = 14*24;
    //定时器初始化
    if(s_1h_timer_id)
    {
        MMK_StopTimer(s_1h_timer_id);
        s_1h_timer_id = 0;
    }
    s_timer_count = 0;
    MMINV_WRITE(MMINPSNV_TIMER_COUNT,&s_timer_count);	
    //prompt变量初始化
    s_is_prompt = FALSE;
    MMINV_WRITE(MMINPSNV_IS_ALREADY_PROMPT,&is_already_prompt);
    //时间到达NV初始化
    MMINV_WRITE(MMINPSNV_IS_TIMER_ARRIVE,&is_timer_arrive);
    //启动定时器
    MMIAPINPS_StartNPSTimer();
    SCI_TRACE_LOW("[NPS] MMIAPINPS_SettingNPSToFactory setting success");

}
/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPINPS_IsAlreadyPrompt(void)
{
    MN_RETURN_RESULT_E    return_value = MN_RETURN_FAILURE;
    BOOLEAN               is_already_prompt = FALSE;
    
    MMINV_READ(MMINPSNV_IS_ALREADY_PROMPT,&is_already_prompt,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_already_prompt = FALSE;
        MMINV_WRITE(MMINPSNV_IS_ALREADY_PROMPT,&is_already_prompt);
    }
    SCI_TRACE_LOW("[NPS] MMIAPINPS_IsAlreadyPrompt is_already_prompt = %d",is_already_prompt);
    return is_already_prompt;
}

/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_StartNPSTimer(void)
{
    MN_RETURN_RESULT_E    return_value = MN_RETURN_FAILURE;
    
    if(!MMIAPINPS_IsTimerArrive())
    {
        MMINV_READ(MMINPSNV_TIMER_COUNT,&s_timer_count,return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            s_timer_count = 0;
            MMINV_WRITE(MMINPSNV_TIMER_COUNT,&s_timer_count);
        }
        
        if(s_1h_timer_id)
        {
            MMK_StopTimer(s_1h_timer_id);
            s_1h_timer_id = 0;
        }
        SCI_TRACE_LOW("[NPS] MMIAPINPS_StartNPSTimer s_one_hour_timer = %d", s_one_hour_timer);
        s_1h_timer_id = MMK_CreateTimerCallback(s_one_hour_timer, NPSTimer_Callback, NULL, TRUE);
    }
}

/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void NPSTimer_Callback(void)
{
    s_timer_count++;
    MMINV_WRITE(MMINPSNV_TIMER_COUNT,&s_timer_count);
    
    if(s_nps_max_hour_count <= s_timer_count)
    {
        BOOLEAN is_timer_arrive = TRUE;
        MMINV_WRITE(MMINPSNV_IS_TIMER_ARRIVE,&is_timer_arrive);
        SCI_TRACE_LOW("[NPS] NPSTimer_Callback is_timer_arrive = %d", is_timer_arrive);
        MMK_StopTimer(s_1h_timer_id);
        s_1h_timer_id = 0;
    }
    SCI_TRACE_LOW("[NPS] NPSTimer_Callback s_timer_count = %d", s_timer_count);
    SCI_TRACE_LOW("[NPS] NPSTimer_Callback s_nps_max_hour_count = %d", s_nps_max_hour_count);
}

/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_InitModule(void)
{
    MMI_RegNPSNv();
}

/*****************************************************************************/
//  Description :NPS反馈询问窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:2015.6.20
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNPSMainSelectWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_STRING_T        title_str = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(MMITHEME_IsMainScreenLandscape())
        {
            bg_rect = both_client_rect.h_rect;
        }
        else
        {
            bg_rect = both_client_rect.v_rect;
        }
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR); 
        MMI_GetLabelTextByLang(STR_NPS_TITLE_FEEDBACK_EXT01, &title_str);
        MMIPUB_SetTextDialogTitle(win_id, &title_str, TRUE);

        //bugid: 1044586 
        if ( MMISET_LANGUAGE_SIMP_CHINESE == MMISET_GetLanguageType() )
        {
            MMINPS_ModifyTextCtrlLineSpace(win_id);
        }

        break;
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
        MMIAPINPS_OpenMainMenuWindow(TRUE);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
	 if (MSG_FULL_PAINT == msg_id)
	 {
		 if(MMITHEME_IsMainScreenLandscape())
		 {
			 bg_rect = both_client_rect.h_rect;
		 }
		 else
		 {
			 bg_rect = both_client_rect.v_rect;
		 }
		 GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
	 }
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}
/*****************************************************************************/
//  Description :NPS反馈发送询问窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNPSFeedbackSendWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMINPS_FEEDBACK_SEND_CTRL_ID);
        break;
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:		
        MMINPS_SendFeedBack();
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
//  Description :NPS反馈正在发送窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNPSFeedbackSendingWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE; 
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
   	GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMIPUB_DIALOG_SOFTKEY_T  dialog_softkey = {0};
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIPUB_DIALOG_SOFTKEY_T dialog_softkey = {0};
            dialog_softkey.index = 2;
            dialog_softkey.content = IMAGE_NULL;
            dialog_softkey.type = DIALOG_SOFTKEY_IMAGE_ID;    
            MMIPUB_SetDialogSoftkey(&win_id,DIALOG_WITH_PROGRESSBAR,&dialog_softkey);
        }            
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        
        break;
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:	
        break;
    case MSG_APP_CANCEL:
    case MSG_APP_OK:
    case MSG_APP_RED:
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}
/*****************************************************************************/
//  Description :NPS插入SIM卡提醒窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNPSInsertSIMAlertWindow(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    MMI_HANDLE_T ctrl_handle = MMI_INVALID_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMIPUB_GetDialogTextCtrlId(win_id);
        GUITEXT_SetAlign(ctrl_handle, ALIGN_LEFT); 
        break;
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:	
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}
/*****************************************************************************/
//  Description :NPS email提醒窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNPSCheckEmailWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE; 
    MMI_HANDLE_T ctrl_handle = MMI_INVALID_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        ctrl_handle = MMIPUB_GetDialogTextCtrlId(win_id);
        GUITEXT_SetAlign(ctrl_handle, ALIGN_LEFT);
        break;
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:	
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}
/*****************************************************************************/
//  Description : 打开反馈询问窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_OpenMainMenuSelectWindow( BOOLEAN is_select_prompt) 
{
    MMI_WIN_PRIORITY_E      win_priority_dialog = WIN_ONE_LEVEL;
    MMI_WIN_ID_T     win_id_query = MMINPS_MAIN_SELECT_WIN_ID;
	#if 0//dc_debug
	StartHttpRequest();
	#endif
    MMIPUB_OpenTextDialogByTextId(PNULL, STR_NPS_ASK_FOR_FEEDBACK_JT, TXT_NULL, IMAGE_NULL, &win_id_query,&win_priority_dialog, 
        MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,HandleNPSMainSelectWindow);
}
/*****************************************************************************/
//  Description : 打开发送选择窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIAPINPS_OpenFeedbackSendWindow(void) 
{
    MMI_CTRL_ID_T    ctrl_id = MMINPS_FEEDBACK_SEND_CTRL_ID;
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    if(MMK_IsOpenWin(MMINPS_FEEDBACK_SEND_WIN_ID))
    {
        MMK_CloseWin(MMINPS_FEEDBACK_SEND_WIN_ID);
    }
    MMK_CreateWin((uint32*)MMINPS_FEEDBACK_SEND_WIN_TAB, PNULL);
    GUIMENU_CreatDynamic(PNULL, MMINPS_FEEDBACK_SEND_WIN_ID, ctrl_id, GUIMENU_STYLE_POPUP);
    MMI_GetLabelTextByLang(STR_NPS_LIST_SEND_EXT01, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = FALSE;
    GUIMENU_InsertNode(0, 0, 0, &node_item, ctrl_id);
}
/*****************************************************************************/
//  Description : 打开发送正在发送窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_OpenFeedbackSendingWindow(void) 
{
    MMI_WIN_PRIORITY_E      win_priority_dialog = WIN_ONE_LEVEL;
    MMI_WIN_ID_T     win_id_query = MMINPS_FEEDBACK_SENDING_WIN_ID;
//	 MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_NPS_SENDING_FEEDBACK_EXT02, PNULL, IMAGE_NULL, &win_id_query,&win_priority_dialog, 
//		            MMIPUB_SOFTKEY_DIALOG_NONE,HandleNPSFeedbackSendingWindow);
    MMIPUB_OpenProgressWaitingIndWinByTextId(STR_NPS_SENDING_FEEDBACK_EXT02, &win_id_query, &win_priority_dialog,
        MMIPUB_SOFTKEY_DIALOG_CLOSE, HandleNPSFeedbackSendingWindow);
}
/*****************************************************************************/
//  Description : 打开插入SIM卡提醒窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIAPINPS_OpenInsertSIMAlertWindow(void) 
{
    MMI_WIN_PRIORITY_E      win_priority_dialog = WIN_ONE_LEVEL;
    MMI_WIN_ID_T     win_id_query = MMINPS_INSERT_SIM_ALERT_WIN_ID;
    MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_SIM_INSERT_SIM_EXT01, PNULL, IMAGE_NULL, &win_id_query,&win_priority_dialog, 
        MMIPUB_SOFTKEY_DIALOG_OK,HandleNPSInsertSIMAlertWindow);
}
/*****************************************************************************/
//  Description : 打开检查邮箱地址提醒窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIAPINPS_OpenCheckEmailAlertWindow(void) 
{
    MMI_WIN_PRIORITY_E      win_priority_dialog = WIN_ONE_LEVEL;
    MMI_WIN_ID_T     win_id_query = MMINPS_CHECK_EMAIL_ALERT_WIN_ID;
    MMIPUB_OpenConfirmationDialogByTextId(PNULL, STR_ID_NPS_INVALID_EMAIL_ADDR_WARNING, PNULL, IMAGE_NULL, &win_id_query,&win_priority_dialog,
        MMIPUB_SOFTKEY_DIALOG_OK,HandleNPSCheckEmailWindow);
}
/*****************************************************************************/
//  Description : 打开反馈窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_OpenMainMenuWindow(BOOLEAN is_prompt) 
{
    MMK_CreateWin((uint32 *)MMINPS_MAIN_WIN_TAB, (ADD_DATA)is_prompt);
}

/*****************************************************************************/
//  Description : 打开反馈窗口                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPINPS_OpenCompleteWindow(void) 
{
    MMK_CreateWin((uint32 *)MMINPS_COMPLETE_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : 子控件灰化                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
//Description : 
LOCAL void MMINPS_SetFormChildGrayDisplay(void)
{
    MMI_CTRL_ID_T   ctrl_id = NPS_MAIN_FORM_CTRL_ID;
    GUI_BG_T       bg_ptr = {GUI_BG_COLOR, 0, IMAGE_NULL, MMI_BLACK_COLOR, 0};
    GUI_BORDER_T   bordor_pyr = {0, MMI_BLACK_COLOR, GUI_BORDER_NONE};
    GUI_COLOR_T      font_color = MMIAPI_GetOpactityColorValue(35, MMI_WHITE_COLOR, MMI_BLACK_COLOR);
    GUI_FONT_ALL_T    font_gray_ptr = {0};
    GUI_FONT_T       font_ptr = MMI_DEFAULT_SMALL_FONT_SIZE;

    font_gray_ptr.font = MMI_DEFAULT_NORMAL_FONT;
    font_gray_ptr.color = font_color;

    GUIFORM_SetChildDisplay(ctrl_id, NPS_SCORE_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);//2
    GUITEXT_SetFont(NPS_REASON_TEXT_CTRL_ID, &font_ptr, &font_color);
    GUIEDIT_SetFontColor(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, font_color);
    GUIEDIT_SetAlign(NPS_SCORE_EDIT_DIGITAL_CTRL_ID,ALIGN_LVMIDDLE);
    GUIEDIT_SetFontColor(NPS_REASON_EDIT_TEXT_CTRL_ID, font_color);
    GUITEXT_SetFont(NPS_MARK_AGREE_CONTACTED_CTRL_ID, &font_ptr, &font_color);
    GUILIST_SetTextFont(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, font_ptr, font_color);
    GUILIST_SetItemGray(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 0, TRUE);
    GUILIST_SetItemInvalid(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 0, TRUE);
    GUILIST_SetListState(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
    GUIFORM_SetChildDisplay(ctrl_id, NPS_PRIVACY_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);//5
    GUITEXT_SetFont(NPS_REMIND_TEXT_CTRL_ID, &font_ptr, &font_color);
    GUIFORM_SetChildDisplay(ctrl_id, NPS_EMAIL_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);//4
    GUITEXT_SetFont(NPS_EMAIL_LABEL_CTRL_ID, &font_ptr, &font_color);
    GUIEDIT_SetFontColor(NPS_EMAIL_EDIT_TEXT_CTRL_ID, font_color);
}
/*****************************************************************************/
//  Description : 子控件正常显示                        
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMINPS_SetFormChildNormalDisplay(void)
{
    MMI_CTRL_ID_T   ctrl_id = NPS_MAIN_FORM_CTRL_ID;
    GUI_FONT_T       font_ptr = MMI_DEFAULT_SMALL_FONT_SIZE;
    GUI_COLOR_T      font_color = MMI_WHITE_COLOR;

    GUIFORM_SetChildDisplay(ctrl_id, NPS_SCORE_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);//2
    GUIEDIT_SetFontColor(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, MMI_WHITE_COLOR);
    GUIEDIT_SetAlign(NPS_SCORE_EDIT_DIGITAL_CTRL_ID,ALIGN_LVMIDDLE);
    GUITEXT_SetFont(NPS_REASON_TEXT_CTRL_ID, &font_ptr, &font_color);
    GUIEDIT_SetFontColor(NPS_REASON_EDIT_TEXT_CTRL_ID, MMI_WHITE_COLOR);
    GUIFORM_SetChildDisplay(ctrl_id, NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);//3	
    GUITEXT_SetFont(NPS_MARK_AGREE_CONTACTED_CTRL_ID, &font_ptr, &font_color);
    GUILIST_SetTextFont(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, font_ptr, font_color);
    GUILIST_SetItemGray(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 0, FALSE);
    GUILIST_SetItemInvalid(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 0, FALSE);
    GUILIST_SetListState(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, GUILIST_STATE_NEED_HIGHTBAR, TRUE);
    GUIFORM_SetChildDisplay(ctrl_id, NPS_PRIVACY_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);//5
    GUITEXT_SetFont(NPS_REMIND_TEXT_CTRL_ID, &font_ptr, &font_color);
    GUIFORM_SetChildDisplay(ctrl_id, NPS_EMAIL_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);//4
    GUITEXT_SetFont(NPS_EMAIL_LABEL_CTRL_ID, &font_ptr, &font_color);
    GUIEDIT_SetFontColor(NPS_EMAIL_EDIT_TEXT_CTRL_ID, font_color);
}
/*****************************************************************************/
//Description : NPS主界面窗口处理函数
//Global resource dependence : none
//Author:
//Note: 
/*****************************************************************************/


LOCAL MMI_RESULT_E  HandleNPSMainWindow(
                                        MMI_WIN_ID_T       win_id, //窗口ID
                                        MMI_MESSAGE_ID_E   msg_id, //消息ID
                                        DPARAM             param   //消息的参数
                                        )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = NPS_MAIN_FORM_CTRL_ID;
    MMI_STRING_T    text = {0};
    GUISTBDATA_TEXT_T text_info_ptr  = {0};
    uint8  i = 0;	
    GUITEXT_TAG_INFO_T tag_info = {0};
    static BOOLEAN score_input_max_len = 1;
    
    switch (msg_id)     
    {
    case MSG_OPEN_WINDOW:
        {
            BOOLEAN is_already_prompt = TRUE;
            BOOLEAN is_already_prompt1 = FALSE;
            MN_RETURN_RESULT_E    return_value = MN_RETURN_FAILURE;
            MMINV_WRITE(MMINPSNV_IS_ALREADY_PROMPT,&is_already_prompt);
            
            MMINV_READ(MMINPSNV_IS_ALREADY_PROMPT,&is_already_prompt1,return_value);
            if (MN_RETURN_SUCCESS != return_value)
            {
                is_already_prompt1 = TRUE;
                MMINV_WRITE(MMINPSNV_IS_ALREADY_PROMPT,&is_already_prompt1);
                SCI_TRACE_LOW("[NPS] HandleNPSMainWindow is_already_prompt = FALSE");
            }
            SCI_TRACE_LOW("[NPS] HandleNPSMainWindow is_already_prompt = %d", is_already_prompt);
            SCI_TRACE_LOW("[NPS] HandleNPSMainWindow is_already_prompt1 = %d", is_already_prompt1);
            
            s_is_prompt = (BOOLEAN)MMK_GetWinAddDataPtr(win_id);
            s_brightness = 8;
            s_score = 8;
            
            s_nps_gray_remain_flag = TRUE;
            
            InitMainFormControl(win_id);
#if 0
            CTRLPROGRESSBAR_SetScoreHighLightFlag(NPS_SCORE_PRGBOX_CTRL_ID, s_nps_gray_remain_flag);
#endif
            GUISTBDATA_GetItemText(0, &text_info_ptr);	
            MMI_GetLabelTextByLang(STR_NPS_HDR_FEEDBACK_EXT02, &text);
            for (i=0; i<text.wstr_len; i++)
            {
                text_info_ptr.wstr_ptr[i] = *text.wstr_ptr++;
            }			
            text_info_ptr.wstr_len = text.wstr_len;
            GUISTBDATA_SetItemText(0, &text_info_ptr);
            GUIWIN_SetStbItemTextValid(0, TRUE);
#if 0
            GUIPRGBOX_SetPosByCtrl(TRUE,s_brightness,s_score,NPS_SCORE_PRGBOX_CTRL_ID);
#endif
            MMK_SetAtvCtrl(win_id, NPS_START_TEXT_THIRD_FORM_CTRL_ID);

            // fixed bug699322, 在open win时就让各个控件为normal状态，"Send"按键高亮(与KEYDOWN_RIGHT行为一致)
            MMINPS_SetFormChildNormalDisplay();
            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, FALSE);
            GUIWIN_UpdateSoftkey(win_id);

        }
        break;
        
    case MSG_FULL_PAINT:
        {
            uint8 selected_num = GUILIST_GetSelectedItemNum(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID);
            
            if(selected_num == 0)
            {
                s_nps_is_email_field_enable = FALSE;
                GUIFORM_SetChildDisplay(ctrl_id, NPS_EMAIL_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            }
            else
            {
                s_nps_is_email_field_enable = TRUE;
                GUIFORM_SetChildDisplay(ctrl_id, NPS_EMAIL_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
            }
        }
        break;
    case MSG_KEYDOWN_LEFT:
#if 0
        if(NPS_SCORE_PRGBOX_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            if (s_brightness > 0)
            {
                s_brightness -= 1;
            }
            else
            {
                s_brightness = 0;
            }
#if 0   //for bug504228
            if(s_brightness <= 0)
            {               
                MMINPS_SetFormChildGrayDisplay();
                
                GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, TRUE);
                GUIWIN_UpdateSoftkey(win_id);
            }
#endif   //end
            GUIPRGBOX_SetPosByCtrl(TRUE,s_brightness,s_score,NPS_SCORE_PRGBOX_CTRL_ID);
            if (s_score <= 0)
            {
                s_score = 0;
            }
            else
            {
                s_score = s_score - 1;
                if(s_score == 0)//forbug487250
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
            }           
        }
#endif
        break;
    case MSG_KEYDOWN_RIGHT:
#if 0
        if(NPS_SCORE_PRGBOX_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            if(FALSE == s_nps_gray_remain_flag)
            {
                s_nps_gray_remain_flag = TRUE;
                CTRLPROGRESSBAR_SetScoreHighLightFlag(NPS_SCORE_PRGBOX_CTRL_ID, s_nps_gray_remain_flag);
            }
            else if (s_brightness < MMINPS_CURRENT_MAX_TIEM)
            {
                s_brightness += 1;
            }
            else
            {
                s_brightness = MMINPS_CURRENT_MAX_TIEM;
            }            
            //for bug504228
            //           if(s_brightness > 0)//??????,???,???????,????
            {
                MMINPS_SetFormChildNormalDisplay();
                GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, FALSE);
                GUIWIN_UpdateSoftkey(win_id);
            }
            //end            
            
            GUIPRGBOX_SetPosByCtrl(TRUE,s_brightness,0,NPS_SCORE_PRGBOX_CTRL_ID);
            if (s_score >= 10)
            {
                s_score = 10;
            }
            else
            {
                if(s_brightness)
                {
                    s_score = s_score + 1;
                }                
                if(s_score == 1)//forbug487250
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
            }
        }
#endif
        break;
        
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        if(((MSG_NOTIFY_MIDSK == msg_id) || (MSG_APP_WEB == msg_id)) && (NPS_REMIND_TEXT_CTRL_ID == MMK_GetActiveCtrlId(win_id)) 
            && (TRUE == GUITEXT_GetCurTag(NPS_REMIND_TEXT_CTRL_ID, &tag_info)))
        {
            if(tag_info.type == GUITEXT_TAG_SET)
            {
                MMK_CreateWin((uint32*)MMINPS_CHECK_PRIVACY_POLICY_WIN_TAB, PNULL);
                break;
            }			
        }
        else
        {
            MMI_STRING_T str_info = {0};            
            uint32 exist_sim_num = 0;
            
            //            if(s_score != 0)   //for bug504228

            SCI_MEMSET(&str_info, 0x00, sizeof(str_info));
            GUIEDIT_GetString(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, &str_info);
            if(str_info.wstr_len != 0)
            {
                char temp_str[360] = {0};
                
                GUI_WstrToUTF8(temp_str, str_info.wstr_len*3 + 1, str_info.wstr_ptr, str_info.wstr_len);
                s_score = atoi(temp_str);
                s_nps_gray_remain_flag = TRUE;
            }
            else
            {
                s_nps_gray_remain_flag = FALSE;
            }

            if(s_nps_gray_remain_flag)
            {
                if (s_score > 10)
                {
                    break;
                }
                SCI_MEMSET(&str_info, 0x00, sizeof(str_info));
                GUIEDIT_GetString(NPS_EMAIL_EDIT_TEXT_CTRL_ID, &str_info);
                if(str_info.wstr_len != 0)
                {
                    char temp_str[360] = {0};
                    
                    GUI_WstrToUTF8(temp_str, str_info.wstr_len*3 + 1, str_info.wstr_ptr, str_info.wstr_len);
                    if(!MMINPS_IsValidEmailAddress(temp_str))
                    {
                        MMIAPINPS_OpenCheckEmailAlertWindow();
                        break;
                    }
                }
                
                exist_sim_num = MMIAPIPHONE_GetSimExistedNum(PNULL,0);
                if(exist_sim_num ==0)
                {
#if 0
#ifndef MMI_IDLE_MINI_SUPPORT
                    MMIPUB_OpenAlertFailWin(TXT_INSERT_SIM);
#else
                    MMIPUB_OpenAlertFailWin(TXT_INSERT_SIM_MINI);
#endif
#endif					
                    MMIAPINPS_OpenInsertSIMAlertWindow();
                    break;
                }
                
                if (MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_OFF)
                {
                    break;
                }
                //MMINPS_GetHttpParamUrl();
                MMIAPINPS_OpenFeedbackSendWindow();
            }  
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_NPS_SCORE_ERROR);
            }
        }
        break;
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
        break;        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T edit_str = {0};
            MMI_STRING_T score_str = {0};
            wchar  score_wchar[10 + 1] = {0};

            GUIEDIT_GetString(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, &edit_str);
            
            score_str.wstr_ptr = score_wchar;
            if((edit_str.wstr_len > 0 ) && ((edit_str.wstr_ptr[0] > '1') || (edit_str.wstr_ptr[0] == '0')) && (2 == score_input_max_len))
            {
                score_str.wstr_len = 1;
                score_str.wstr_ptr[0] = edit_str.wstr_ptr[0];
                GUIEDIT_SetMaxLen(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, 1);
                GUIEDIT_SetString(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, score_str.wstr_ptr, score_str.wstr_len);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                score_input_max_len = 1;
            }
            else if((1 == edit_str.wstr_len) && (edit_str.wstr_ptr[0] == '1') && (1 == score_input_max_len))
            {
                score_str.wstr_len = 1;
                score_str.wstr_ptr[0] = edit_str.wstr_ptr[0];
                GUIEDIT_SetMaxLen(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, 3);
                GUIEDIT_SetMaxLen(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, 2);
                GUIEDIT_SetString(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, score_str.wstr_ptr, score_str.wstr_len);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                score_input_max_len = 2;
            }


            if((edit_str.wstr_len > 1 ) && ((edit_str.wstr_ptr[0] > '1') || (edit_str.wstr_ptr[0] == '0')))
            {
                score_str.wstr_len = 1;
                score_str.wstr_ptr[0] = edit_str.wstr_ptr[0];
                GUIEDIT_SetString(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, score_str.wstr_ptr, score_str.wstr_len);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            else if((edit_str.wstr_len > 1 ) && (edit_str.wstr_ptr[0] == '1') && (edit_str.wstr_ptr[1] != '0'))
            {
                score_str.wstr_len = 1;
                score_str.wstr_ptr[0] = edit_str.wstr_ptr[0];
                GUIEDIT_SetString(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, score_str.wstr_ptr, score_str.wstr_len);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }

        }
        break;
    case MSG_NOTIFY_LIST_MARK_ITEM:
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_UP:
#if 0     //for bug504228     
        if(NPS_REMIND_TEXT_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            if(s_brightness > 0)
            {
                GUIWIN_SetSoftkeyTextId(win_id, PNULL, STXT_SOFTKEY_SELECT_MK, PNULL, TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MENU_MK, 0, TRUE);
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, 2, TRUE);
            }			
        }
#endif  //end
        if(s_brightness <= 0)
        { // GUIWIN_GetSoftkeyTextId(win_id);
            GUIWIN_SetSoftkeyTextId(win_id, PNULL, STR_NPS_LIST_SEND_EXT01, PNULL, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SOFTKEY_MENU_MK, 0, TRUE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, 2, TRUE);
        }
        
        GUIWIN_UpdateStb();
        break;
    case MSG_GET_FOCUS:
        GUISTBDATA_GetItemText(0, &text_info_ptr);  
        MMI_GetLabelTextByLang(STR_NPS_HDR_FEEDBACK_EXT02, &text);
        for (i=0; i<text.wstr_len; i++)
        {
            text_info_ptr.wstr_ptr[i] = *text.wstr_ptr++;
        }           
        text_info_ptr.wstr_len = text.wstr_len;
        GUISTBDATA_SetItemText(0, &text_info_ptr);
        GUIWIN_SetStbItemTextValid(0, TRUE);
        break;
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        s_brightness = 0;
        s_nps_gray_remain_flag = FALSE;
        break;        
        
    default:        
        result = MMI_RESULT_FALSE;
        break;
    }    
    
    return result;
}

/*****************************************************************************/
//Description : 
//Global resource dependence : none
//Author:
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNPSCompleteWindow(
                                            MMI_WIN_ID_T       win_id, //窗口ID
                                            MMI_MESSAGE_ID_E   msg_id, //消息ID
                                            DPARAM             param   //消息的参数
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = NPS_COMPLETE_TEXT_ID;
    
    switch (msg_id)     
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T string = {0};
            MMI_STRING_T    			result_string = {0};
            wchar 						result_wchar[200] = {0};
            
            result_string.wstr_ptr 	= result_wchar;
            
            MMI_GetLabelTextByLang(STR_CHERRY_HELLO_MT_MSG_RM, &string);
            
            MMIAPICOM_CustomGenerateReplaceStr(&string, L"%U", L"UNISOC", 200, &result_string);
            GUITEXT_SetString(ctrl_id, result_string.wstr_ptr, result_string.wstr_len, FALSE);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMK_CloseWin( win_id );      
        break;      
        
    default:        
        result = MMI_RESULT_FALSE;
        break;
    }    
    
    return result;
}

/*****************************************************************************/
//  Description :NPS Privacy Policy窗口处理函数 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNPSCheckPrivacyPolicyWindow(
                                                     MMI_WIN_ID_T win_id,
                                                     MMI_MESSAGE_ID_E msg_id,
                                                     DPARAM param
                                                     )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMINPS_CHECK_PRIVACY_POLICY_CTRL_ID;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
   	GUI_RECT_T   bg_rect = {0};
    GUI_BOTH_RECT_T both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUISTBDATA_TEXT_T statusbar_ptr  = {0};
    MMI_STRING_T    text = {0};
    MMI_STRING_T    text1 = {0};
    uint8   i = 0;
    GUI_BG_T  bg_color = {0};
    GUI_FONT_T  font_ptr = CAF_FONT_20;
    GUI_COLOR_T  font_color = MMI_WHITE_COLOR;
    GUI_RECT_T   text_rect = {0, 24, 239, 280};
    switch (msg_id)     
    {
    case MSG_OPEN_WINDOW:
        {
            if(MMITHEME_IsMainScreenLandscape())
            {
                bg_rect = both_client_rect.h_rect;
            }
            else
            {
                bg_rect = both_client_rect.v_rect;
            }
            bg_color.bg_type = GUI_BG_COLOR;
            bg_color.color = MMI_BLACK_COLOR;
            MMI_GetLabelTextByLang(STR_PRIVACY_POLICY_HDR_EX02, &text1);
            GUITEXT_SetFont(ctrl_id, &font_ptr, &font_color);
            GUITEXT_SetBg(ctrl_id, &bg_color);
            GUITEXT_SetRect(ctrl_id, &text_rect);
            GUITEXT_SetString(ctrl_id, text1.wstr_ptr, text1.wstr_len, FALSE);
            GUISTBDATA_GetItemText(0, &statusbar_ptr);	
            MMI_GetLabelTextByLang(STR_PRIVACY_POLICY_HDR_EX02, &text);
            for (i=0; i<text.wstr_len; i++)
            {
                statusbar_ptr.wstr_ptr[i] = *text.wstr_ptr++;
            }			
            statusbar_ptr.wstr_len = text.wstr_len;
            GUISTBDATA_SetItemText(0, &statusbar_ptr);
            GUIWIN_SetStbItemTextValid(0, TRUE);
            GUIWIN_UpdateStb();
        }
        break;
    case MSG_FULL_PAINT:
        break;	
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
    case MSG_GET_FOCUS:
        GUISTBDATA_GetItemText(0, &statusbar_ptr);	
        MMI_GetLabelTextByLang(STR_PRIVACY_POLICY_HDR_EX02, &text);
        for (i=0; i<text.wstr_len; i++)
        {
            statusbar_ptr.wstr_ptr[i] = *text.wstr_ptr++;
        }			
        statusbar_ptr.wstr_len = text.wstr_len;
        GUISTBDATA_SetItemText(0, &statusbar_ptr);
        GUIWIN_SetStbItemTextValid(0, TRUE);
        GUIWIN_UpdateStb();
        break;
    default:        
        result = MMI_RESULT_FALSE;
        break;
    }    
    return result;
}

/*****************************************************************************/
// 	Description : to INITIAL MAIN FORM CONTROL 
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void InitMainFormControl(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T text = {0};
    
//    MMI_STRING_T text_policy = {0};
    
    MMI_STRING_T text_list10 = {0};
    MMI_STRING_T text_list0 = {0};
    MMI_STRING_T text_give_score = {0};
    wchar     *text_list_ptr = PNULL;
    wchar     *text_list_ptr1 = PNULL;
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0}; 
	GUI_BG_T       bg_ptr = {GUI_BG_COLOR, 0, IMAGE_NULL, MMI_BLACK_COLOR, 0};
	uint16      prg_rect_height =  VOLUME_BRIGHTNESS_BAR_HEIGHT;//2//*GUIFONT_GetHeight(MMI_DEFAULT_TEXT_FONT);
    GUIFORM_CHILD_HEIGHT_T   child_height = {0};
    GUIFORM_CHILD_WIDTH_T    child_width  = {0};
    GUI_BOTH_RECT_T   client_rect =  MMITHEME_GetWinClientBothRect(win_id);
    GUI_RECT_T      parent_form_rect = {0};
    GUI_FONT_T    score_font_ptr = MMI_DEFAULT_NORMAL_FONT_SIZE; //spec small(14),MMI_DEFAULT_SMALL_FONT
    GUI_FONT_T    font_ptr = MMI_DEFAULT_SMALL_FONT_SIZE; //spec small(14),MMI_DEFAULT_SMALL_FONT
    GUI_COLOR_T   font_color = MMI_WHITE_COLOR;
    GUI_BG_T   by_color = {0};
    uint16    form_space_hor = 0;
    uint16    form1_space_hor = 0;
    uint16    child_space = 0;
    GUI_BORDER_T   bordor_pyr = {0, MMI_BLACK_COLOR, GUI_BORDER_NONE};
    GUITEXT_SET_TAG_T   set_tag_ptr = {0};
    GUITEXT_TAG_DATA_T  set_tag_data = {0};
//    GUI_FONT_ALL_T     font_gray_ptr = {MMI_DEFAULT_NORMAL_FONT, MMI_DARK_GRAY_COLOR};
//    GUI_FONT_ALL_T     font_normal_ptr = {MMI_DEFAULT_NORMAL_FONT, MMI_WHITE_COLOR};
    uint16      margin_left = MMINPS_MARGIN_TEXT_LEFT_RIGHT;
    uint16      margin_right = MMINPS_MARGIN_TEXT_LEFT_RIGHT;
    uint16      margin_top  = MMINPS_MARGIN_TEXT_TOP_RIGHT;
    uint16      margin_buttom = MMINPS_MARGIN_TEXT_TOP_RIGHT;
//    wchar    *privacy_policy1 = PNULL;
//    wchar    *privacy_policy2 = PNULL;
//    wchar    *privacy_policy11 = PNULL;
//    wchar    *privacy_policy22 = PNULL;
    
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        parent_form_rect = client_rect.h_rect;      
    }
    else
    {
        parent_form_rect = client_rect.v_rect;
    } 
    GUIFORM_SetType(NPS_MAIN_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
    GUIFORM_SetBg(NPS_MAIN_FORM_CTRL_ID, &bg_ptr);
    GUIFORM_SetMargin(NPS_MAIN_FORM_CTRL_ID, 0);
    GUIFORM_PermitChildBg(NPS_MAIN_FORM_CTRL_ID, FALSE);
    GUIFORM_SetSpace(NPS_MAIN_FORM_CTRL_ID, PNULL, &form_space_hor);
    GUIFORM_SetRect(NPS_MAIN_FORM_CTRL_ID, &parent_form_rect);
    GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, TRUE);   //for bug504228
    GUIFORM_IsSlide(NPS_MAIN_FORM_CTRL_ID, FALSE);
    
    //1
    child_width.add_data = 240;
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIFORM_SetChildWidth(NPS_MAIN_FORM_CTRL_ID, NPS_START_TEXT_THIRD_FORM_CTRL_ID, &child_width);
    GUIFORM_SetChildAlign(NPS_MAIN_FORM_CTRL_ID, NPS_START_TEXT_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
    GUIFORM_SetType(NPS_START_TEXT_THIRD_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
    GUIFORM_SetBg(NPS_START_TEXT_THIRD_FORM_CTRL_ID, &bg_ptr);
    GUIFORM_PermitChildBg(NPS_START_TEXT_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildFont(NPS_START_TEXT_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_SetSpace(NPS_START_TEXT_THIRD_FORM_CTRL_ID, PNULL, &form1_space_hor);
    GUIFORM_IsSlide(NPS_START_TEXT_THIRD_FORM_CTRL_ID, FALSE);
    
    by_color.bg_type = GUI_BG_COLOR;
    by_color.color = MMI_BLACK_COLOR;
    MMI_GetLabelTextByLang(STR_NPS_QUERY_FEEDBACK_EXT02, &text);
    GUITEXT_SetFont(NPS_EXPLAIN_TEXT_CTRL_ID, &font_ptr, &font_color);
//    child_height.add_data = 90;//需要修改
//    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
//    GUIFORM_SetChildHeight(NPS_START_TEXT_THIRD_FORM_CTRL_ID ,NPS_EXPLAIN_TEXT_CTRL_ID, &child_height);
    GUITEXT_SetMarginEx(NPS_EXPLAIN_TEXT_CTRL_ID, margin_left, margin_right, margin_top, margin_buttom);
    GUITEXT_SetBg(NPS_EXPLAIN_TEXT_CTRL_ID, &by_color);
    GUITEXT_IsDisplayPrg(FALSE, NPS_EXPLAIN_TEXT_CTRL_ID);
    GUITEXT_SetString(NPS_EXPLAIN_TEXT_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
    
    MMI_GetLabelTextByLang(STR_NPS_LIST_10_EXT02, &text_list10); 
    MMI_GetLabelTextByLang(STR_NPS_LIST_0_EXT02, &text_list0);
    //for bug 1053220 : STR_NPS_LIST_0_EXT02 and STR_NPS_LIST_10_EXT02 not display
#if 0    
    MMI_GetLabelTextByLang(STR_NPS_GIVE_SCORE_EXT02, &text_give_score);
    text.wstr_len = text_list10.wstr_len + text_list0.wstr_len + text_give_score.wstr_len + 3*sizeof('\n') + 1;
    text_list_ptr = (wchar *)SCI_ALLOC_APPZ(sizeof(wchar) * text.wstr_len);
    text_list_ptr1 = text_list_ptr;
    text.wstr_ptr = text_list_ptr;
    text_list_ptr1 = MMIAPICOM_Wstrncpy(text_list_ptr1, text_list10.wstr_ptr, text_list10.wstr_len);
    text_list_ptr1 = MMIAPICOM_Wstrcat(text_list_ptr1, L"\n");
    text_list_ptr1 = MMIAPICOM_Wstrncat(text_list_ptr1, text_list0.wstr_ptr, text_list0.wstr_len);
    text_list_ptr1 = MMIAPICOM_Wstrcat(text_list_ptr1, L"\n");
    text_list_ptr1 = MMIAPICOM_Wstrcat(text_list_ptr1, L"\n");
    text_list_ptr1 = MMIAPICOM_Wstrncat(text_list_ptr1, text_give_score.wstr_ptr, text_give_score.wstr_len);
#else
    text.wstr_len = text_list10.wstr_len + text_list0.wstr_len + 2*sizeof('\n') + 1;
    text_list_ptr = (wchar *)SCI_ALLOC_APPZ(sizeof(wchar) * text.wstr_len);
    text_list_ptr1 = text_list_ptr;
    text.wstr_ptr = text_list_ptr;
    text_list_ptr1 = MMIAPICOM_Wstrncpy(text_list_ptr1, text_list10.wstr_ptr, text_list10.wstr_len);
    text_list_ptr1 = MMIAPICOM_Wstrcat(text_list_ptr1, L"\n");
    text_list_ptr1 = MMIAPICOM_Wstrncat(text_list_ptr1, text_list0.wstr_ptr, text_list0.wstr_len);
    text_list_ptr1 = MMIAPICOM_Wstrcat(text_list_ptr1, L"\n");
#endif
    GUIFORM_SetChildSpace(NPS_START_TEXT_THIRD_FORM_CTRL_ID, NPS_SCORE_LABEL_CTRL_ID, &child_space, &child_space);
    GUITEXT_SetFont(NPS_SCORE_LABEL_CTRL_ID, &font_ptr, &font_color);
//    child_height.add_data = 85;//需要修改
//    GUIFORM_SetChildHeight(NPS_START_TEXT_THIRD_FORM_CTRL_ID ,NPS_SCORE_LABEL_CTRL_ID, &child_height);
    GUITEXT_SetMarginEx(NPS_SCORE_LABEL_CTRL_ID, margin_left, margin_right, margin_top, margin_buttom);
    GUITEXT_SetBg(NPS_SCORE_LABEL_CTRL_ID, &by_color);
    GUITEXT_IsDisplayPrg(FALSE, NPS_SCORE_LABEL_CTRL_ID);
    GUITEXT_SetString(NPS_SCORE_LABEL_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
    
    child_height.add_data = prg_rect_height;
    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
#if 0
    GUIFORM_SetChildHeight(NPS_START_TEXT_THIRD_FORM_CTRL_ID ,NPS_SCORE_PRGBOX_CTRL_ID, &child_height); 
    GUIPRGBOX_SetParamByCtrl(NPS_SCORE_PRGBOX_CTRL_ID, MMINPS_AUDIO_SET_BRIMAX, MMINPS_AUDIO_SET_BRIMAX);
    GUIPRGBOX_SetPosByCtrl(FALSE,s_brightness,0,NPS_SCORE_PRGBOX_CTRL_ID);
    GUIPRGBOX_SetBgColor(NPS_SCORE_PRGBOX_CTRL_ID, MMI_BLACK_COLOR);
#endif
    SCI_FREE(text_list_ptr);
    text_list_ptr1 = PNULL;
//    GUIEDIT_SetSoftkey(NPS_SCORE_PRGBOX_CTRL_ID, 1, 1,TXT_SEND,TXT_SEND, NPSMainWinSoftkey);
    
    //2
    font_color = MMIAPI_GetOpactityColorValue(35, MMI_WHITE_COLOR, MMI_BLACK_COLOR); //for bug504228
    child_width.add_data = 240;
    GUIFORM_SetChildWidth(NPS_MAIN_FORM_CTRL_ID, NPS_SCORE_THIRD_FORM_CTRL_ID, &child_width);
    GUIFORM_SetChildAlign(NPS_MAIN_FORM_CTRL_ID, NPS_SCORE_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
    GUIFORM_SetChildDisplay(NPS_MAIN_FORM_CTRL_ID, NPS_SCORE_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);//1//GUIFORM_CHILD_DISP_GRAY   //for bug504228
//    GUIFORM_SetChildDisplay(NPS_MAIN_FORM_CTRL_ID, NPS_SCORE_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);//2
    GUIFORM_SetType(NPS_SCORE_THIRD_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
    GUIFORM_SetBg(NPS_SCORE_THIRD_FORM_CTRL_ID, &bg_ptr);
    GUIFORM_PermitChildBg(NPS_SCORE_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildFont(NPS_SCORE_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildBorder(NPS_SCORE_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_SetSpace(NPS_SCORE_THIRD_FORM_CTRL_ID, PNULL, &form1_space_hor);
    GUIFORM_IsSlide(NPS_SCORE_THIRD_FORM_CTRL_ID, FALSE);
    
    MMI_GetLabelTextByLang(STR_NPS_REASON_FOR_SCORE_EXT02, &text);
    GUITEXT_SetFont(NPS_REASON_TEXT_CTRL_ID, &font_ptr, &font_color);
//    child_height.add_data = 50;//需要修改
//    GUIFORM_SetChildHeight(NPS_SCORE_THIRD_FORM_CTRL_ID ,NPS_REASON_TEXT_CTRL_ID, &child_height);
    GUIFORM_SetChildDisplay(NPS_SCORE_THIRD_FORM_CTRL_ID, NPS_REASON_TEXT_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);//GUIFORM_CHILD_DISP_GRAY   //for bug504228
    GUITEXT_SetMarginEx(NPS_REASON_TEXT_CTRL_ID, margin_left, margin_right, margin_top, margin_buttom);
    GUITEXT_SetBg(NPS_REASON_TEXT_CTRL_ID, &by_color);
    GUITEXT_IsDisplayPrg(FALSE, NPS_REASON_TEXT_CTRL_ID);
    GUITEXT_SetString(NPS_REASON_TEXT_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);

    GUIEDIT_SetStyle(NPS_SCORE_EDIT_DIGITAL_CTRL_ID,GUIEDIT_STYLE_MULTI_DYN_DOWN);
    CTRLBASEEDIT_SetLabelId(MMINPS_MAIN_MENU_WIN_ID,NPS_SCORE_EDIT_DIGITAL_CTRL_ID,STR_NPS_GIVE_SCORE_EXT02,FALSE);
    GUIEDIT_SetDefaultStringById(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, STR_NPS_GIVE_SCORE_NEW);

    GUIEDIT_SetAlign(NPS_SCORE_EDIT_DIGITAL_CTRL_ID,ALIGN_HVMIDDLE);
    GUIEDIT_SetMaxLen(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, 2);
    GUIEDIT_SetFontColor(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, font_color);
    GUIEDIT_SetFont(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, score_font_ptr);
    GUIEDIT_PermitBorder(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, FALSE);
    GUIEDIT_SetSoftkeyEx(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, 0, 0, STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_MENU_MK, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
    GUIEDIT_SetSoftkeyEx(NPS_SCORE_EDIT_DIGITAL_CTRL_ID, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);

//    child_height.add_data = 66;//需要修改,GUIEDIT_SetBg()
//    GUIFORM_SetChildHeight(NPS_SCORE_THIRD_FORM_CTRL_ID ,NPS_REASON_EDIT_TEXT_CTRL_ID, &child_height);
    GUIEDIT_SetStyle(NPS_REASON_EDIT_TEXT_CTRL_ID,GUIEDIT_STYLE_SINGLE_DYN_DOWN);
    GUIEDIT_SetFontColor(NPS_REASON_EDIT_TEXT_CTRL_ID, font_color);
    GUIEDIT_SetFont(NPS_REASON_EDIT_TEXT_CTRL_ID, font_ptr);
    GUIEDIT_PermitBorder(NPS_REASON_EDIT_TEXT_CTRL_ID, FALSE);
//    GUIEDIT_SetIm(NPS_REASON_EDIT_TEXT_CTRL_ID, GUIIM_TYPE_STROKE|GUIIM_TYPE_MULTITAP|GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ABC |GUIIM_TYPE_ENGLISH, GUIIM_TYPE_ABC);
//    GUIEDIT_SetImCapital(NPS_REASON_EDIT_TEXT_CTRL_ID, GUIIM_MODE_UPPER);
    GUIEDIT_SetDefaultStringById(NPS_REASON_EDIT_TEXT_CTRL_ID, STR_NPS_HINT_WRITE_EXT01);
    GUIEDIT_SetSoftkeyEx(NPS_REASON_EDIT_TEXT_CTRL_ID, 0, 0, STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_MENU_MK, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
    GUIEDIT_SetSoftkeyEx(NPS_REASON_EDIT_TEXT_CTRL_ID, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
//    GUIEDIT_SetSoftkey(NPS_REASON_EDIT_TEXT_CTRL_ID, 1, 0,TXT_NULL,TXT_NULL, NPSMainWinSoftkey);
	
    //3
    child_width.add_data = 240;
    GUIFORM_SetChildWidth(NPS_MAIN_FORM_CTRL_ID, NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, &child_width);
    GUIFORM_SetChildAlign(NPS_MAIN_FORM_CTRL_ID, NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
//    GUIFORM_SetChildDisplay(NPS_MAIN_FORM_CTRL_ID, NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);//1
//    GUIFORM_SetChildDisplay(NPS_MAIN_FORM_CTRL_ID, NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);//2
    GUIFORM_SetType(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
    GUIFORM_SetBg(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, &bg_ptr);
    GUIFORM_PermitChildBg(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildFont(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, FALSE);	
    GUIFORM_PermitChildBorder(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_SetSpace(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, PNULL, &form1_space_hor);
    GUIFORM_IsSlide(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID, FALSE);
    
    MMI_GetLabelTextByLang(STR_NPS_CONTACT_FOR_DETAILS_EXT02, &text);
    GUITEXT_SetFont(NPS_MARK_AGREE_CONTACTED_CTRL_ID, &font_ptr, &font_color);
//    child_height.add_data = 78;//需要修改
//    GUIFORM_SetChildHeight(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID ,NPS_MARK_AGREE_CONTACTED_CTRL_ID, &child_height);
    GUITEXT_SetMarginEx(NPS_MARK_AGREE_CONTACTED_CTRL_ID, margin_left, margin_right, margin_top, margin_buttom);
    GUITEXT_SetBg(NPS_MARK_AGREE_CONTACTED_CTRL_ID, &by_color);
    GUITEXT_IsDisplayPrg(FALSE, NPS_MARK_AGREE_CONTACTED_CTRL_ID);
    GUITEXT_SetString(NPS_MARK_AGREE_CONTACTED_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);  
    
    item_t.item_data_ptr = &item_data;
    item_t.item_style = GUIITEM_STYLE_MULTI_CHOICE_PICKER_TEXT;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = STR_NPS_AGREE_TO_CONTACT_EXT01;
    item_data.softkey_id[0] = STXT_SOFTKEY_MENU_MK;
    item_data.softkey_id[1] = STXT_SOFTKEY_SELECT_MK;
    item_data.softkey_id[2] = TXT_COMMON_COMMON_BACK;
//    child_height.add_data = 42;//需要修改		 
//    GUIFORM_SetChildHeight(NPS_SCORE_RESON_THIRD_FORM_CTRL_ID ,NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, &child_height);
    GUILIST_SetMaxItem(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 1, FALSE);
    GUILIST_SetCheckIconLeftFlag(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, TRUE);
    GUILIST_SetBgColor(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, MMI_BLACK_COLOR);
    GUILIST_PermitBorder(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, FALSE);
    GUILIST_SetTextFont(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, font_ptr, font_color);
    GUILIST_SetMaxSelectedItem(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 1);
    GUILIST_SetTextListMarkable(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, TRUE);
    GUILIST_SetNeedPrgbarBlock(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, FALSE);
    GUILIST_AppendItem(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, &item_t);
    GUILIST_SetCurItemIndex(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 0);
    GUILIST_SetItemGray(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 0, TRUE);
    GUILIST_SetItemInvalid(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, 0, TRUE);
    GUILIST_SetListState(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
    GUILIST_ChangeCheckListDefSoftkeyID(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID, STXT_SOFTKEY_SELECT_MK, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK);
    GUILIST_SetOwnSofterKey(NPS_FOLLOWUP_CHECK_LIST_CTRL_ID,TRUE);
    //4
    child_width.add_data = 240;
    GUIFORM_SetChildWidth(NPS_MAIN_FORM_CTRL_ID, NPS_EMAIL_THIRD_FORM_CTRL_ID, &child_width);
    GUIFORM_SetChildDisplay(NPS_MAIN_FORM_CTRL_ID, NPS_EMAIL_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    GUIFORM_SetChildAlign(NPS_MAIN_FORM_CTRL_ID, NPS_EMAIL_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
    GUIFORM_SetType(NPS_EMAIL_THIRD_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
    GUIFORM_SetBg(NPS_EMAIL_THIRD_FORM_CTRL_ID, &bg_ptr);
    GUIFORM_PermitChildBg(NPS_EMAIL_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildFont(NPS_EMAIL_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildBorder(NPS_EMAIL_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_SetSpace(NPS_EMAIL_THIRD_FORM_CTRL_ID, PNULL, &form1_space_hor);
    GUIFORM_IsSlide(NPS_EMAIL_THIRD_FORM_CTRL_ID, FALSE);
    
    MMI_GetLabelTextByLang(STR_ID_NPS_EMALL_INPUT_HINT, &text);
    GUITEXT_SetFont(NPS_EMAIL_LABEL_CTRL_ID, &font_ptr, &font_color);
    GUITEXT_SetMarginEx(NPS_EMAIL_LABEL_CTRL_ID, margin_left, margin_right, margin_top, margin_buttom);
    GUITEXT_SetBg(NPS_EMAIL_LABEL_CTRL_ID, &by_color);
    GUITEXT_IsDisplayPrg(FALSE, NPS_EMAIL_LABEL_CTRL_ID);
    GUITEXT_SetString(NPS_EMAIL_LABEL_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
    
    GUIEDIT_SetMargin(NPS_EMAIL_EDIT_TEXT_CTRL_ID, margin_left, margin_left);
    GUIEDIT_SetStyle(NPS_EMAIL_EDIT_TEXT_CTRL_ID,GUIEDIT_STYLE_SINGLE_DYN_DOWN); 
    GUIEDIT_SetFontColor(NPS_EMAIL_EDIT_TEXT_CTRL_ID, MMI_WHITE_COLOR);
    GUIEDIT_SetFont(NPS_EMAIL_EDIT_TEXT_CTRL_ID, font_ptr);
    GUIEDIT_PermitBorder(NPS_EMAIL_EDIT_TEXT_CTRL_ID, FALSE);
    GUIEDIT_SetIm(NPS_EMAIL_EDIT_TEXT_CTRL_ID, GUIIM_TYPE_STROKE|GUIIM_TYPE_MULTITAP|GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ABC |GUIIM_TYPE_ENGLISH, GUIIM_TYPE_ABC);
    GUIEDIT_SetImCapital(NPS_EMAIL_EDIT_TEXT_CTRL_ID, GUIIM_MODE_UPPER);
    GUIEDIT_SetDefaultStringById(NPS_EMAIL_EDIT_TEXT_CTRL_ID, STR_NPS_HINT_EMAIL_EXT01);
    
    GUIEDIT_SetSoftkey(NPS_EMAIL_EDIT_TEXT_CTRL_ID, 1, 0, STR_NPS_LIST_SEND_EXT01, STR_NPS_LIST_SEND_EXT01, PNULL);
    GUIEDIT_SetSoftkeyEx(NPS_EMAIL_EDIT_TEXT_CTRL_ID, 0, 0, STXT_SOFTKEY_MENU_MK, STXT_SOFTKEY_MENU_MK, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
    GUIEDIT_SetSoftkeyEx(NPS_EMAIL_EDIT_TEXT_CTRL_ID, 2, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, PNULL, PNULL, EDIT_BUTTON_TEXT_ID, PNULL);
    
    //5
    child_width.add_data = 240;
    GUIFORM_SetChildWidth(NPS_MAIN_FORM_CTRL_ID, NPS_PRIVACY_THIRD_FORM_CTRL_ID, &child_width);
    GUIFORM_SetChildDisplay(NPS_MAIN_FORM_CTRL_ID, NPS_PRIVACY_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);//1//GUIFORM_CHILD_DISP_GRAY  //for bug504228
//    GUIFORM_SetChildDisplay(NPS_MAIN_FORM_CTRL_ID, NPS_PRIVACY_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);//2
    GUIFORM_SetChildAlign(NPS_MAIN_FORM_CTRL_ID, NPS_PRIVACY_THIRD_FORM_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
    GUIFORM_SetType(NPS_PRIVACY_THIRD_FORM_CTRL_ID,GUIFORM_TYPE_COMMON);
    GUIFORM_SetBg(NPS_PRIVACY_THIRD_FORM_CTRL_ID, &bg_ptr);
    GUIFORM_PermitChildBg(NPS_PRIVACY_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildFont(NPS_PRIVACY_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_PermitChildBorder(NPS_PRIVACY_THIRD_FORM_CTRL_ID, FALSE);
    GUIFORM_SetSpace(NPS_PRIVACY_THIRD_FORM_CTRL_ID, PNULL, &form1_space_hor);
    GUIFORM_IsSlide(NPS_PRIVACY_THIRD_FORM_CTRL_ID, FALSE);
    
    MMI_GetLabelTextByLang(STR_NPS_INFO_ASSOCIATED_JT, &text);
    
#if 0   //MS requirement change
    
    MMI_GetLabelTextByLang(STR_PRIVACY_POLICY_HDR_EX02, &text_policy);
    privacy_policy11 = text.wstr_ptr;
    privacy_policy11 = MMIAPICOM_Wstrstr(text.wstr_ptr, text_policy.wstr_ptr);
    set_tag_data.len = text_policy.wstr_len;//SCI_STRLEN((char*)privacy_policy2);
    set_tag_data.start_pos = abs(privacy_policy11 - text.wstr_ptr);
    set_tag_ptr.tag_num = 1;
    set_tag_ptr.tag_ptr = &set_tag_data;
    
#endif
    
    GUITEXT_SetFont(NPS_REMIND_TEXT_CTRL_ID, &font_ptr, &font_color);
    GUITEXT_SetMarginEx(NPS_REMIND_TEXT_CTRL_ID, margin_left, margin_right, margin_top, margin_buttom);
    GUITEXT_SetBg(NPS_REMIND_TEXT_CTRL_ID, &by_color);
    GUITEXT_SetBorder(&bordor_pyr, NPS_REMIND_TEXT_CTRL_ID);
    GUITEXT_IsDisplayPrg(FALSE, NPS_REMIND_TEXT_CTRL_ID);		 
    GUITEXT_SetString(NPS_REMIND_TEXT_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);   

//    GUITEXT_SetTagType(NPS_REMIND_TEXT_CTRL_ID, GUITEXT_TAG_SET, &set_tag_ptr);	 
//    privacy_policy11 = PNULL;
//    privacy_policy22 = PNULL;

}

/*****************************************************************************/
// 	Description : PIN/PUK input window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN NPSMainWinSoftkey(
                                wchar         *str_ptr,   //in
                                uint16        str_len,    //in
                                MMI_TEXT_ID_T *text_id_ptr//in/out:
                                )
{
    BOOLEAN result = TRUE;
    MMI_STRING_T score_info = {0};
    
#if 0
    GUIEDIT_GetString(NPS_SCORE_PRGBOX_CTRL_ID, &score_info);
#endif
    
    if(score_info.wstr_len != 0)
    {
        GUIWIN_SetSoftkeyBtnGray(MMINPS_MAIN_MENU_WIN_ID, MIDDLE_BUTTON, FALSE);
    }
    else
    {
        GUIWIN_SetSoftkeyBtnGray(MMINPS_MAIN_MENU_WIN_ID, MIDDLE_BUTTON, TRUE);
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : PIN/PUK input window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void MMINPS_SendFeedBack(void)
{
    MMINPS_PdpActive();
}

/*****************************************************************************/
//  Discription: This function is used to Init the NPS app
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPINPS_Init(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    BOOLEAN is_timer_arrive = FALSE;
    BOOLEAN is_already_prompt = FALSE;
    
    g_nps_app.ProcessMsg = AppHandleNPSMsg;
    
    MMINV_READ(MMINPSNV_TIMER_COUNT,&s_timer_count,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_timer_count = 0;
        MMINV_WRITE(MMINPSNV_TIMER_COUNT,&s_timer_count);
    }
    SCI_TRACE_LOW("[NPS] MMIAPINPS_Init s_timer_count = %d", s_timer_count);
    
    MMINV_READ(MMINPSNV_IS_TIMER_ARRIVE,&is_timer_arrive,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_timer_arrive = FALSE;
        MMINV_WRITE(MMINPSNV_IS_TIMER_ARRIVE,&is_timer_arrive);
    }
    SCI_TRACE_LOW("[NPS] MMIAPINPS_Init is_timer_arrive = %d", is_timer_arrive);
    
    MMINV_READ(MMINPSNV_IS_ALREADY_PROMPT,&is_already_prompt,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_already_prompt = FALSE;
        MMINV_WRITE(MMINPSNV_IS_ALREADY_PROMPT,&is_already_prompt);
    }
    SCI_TRACE_LOW("[NPS] MMIAPINPS_Init is_already_prompt = %d", is_already_prompt);
}

/*****************************************************************************/
//  Discription: This function used to process msg comeback form blue tooth
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E AppHandleNPSMsg(
                                   PWND app_ptr,
                                   uint16 msg_id,
                                   DPARAM param
                                   )
{
	MMI_RESULT_E    result = MMI_RESULT_FALSE;
#ifdef ENABLE_HTTPC
	if (HTTPC_SIG_BEG <= msg_id && msg_id <= HTTPC_SIG_END)
    {
        result = HandleNPSHttpMsg(app_ptr, msg_id, param);
    }
#else
	if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        result = HandleNPSHttpMsg(app_ptr, msg_id, param);
    }
#endif
    else if (MSG_NPS_PDP_ACTIVE_CNF <= msg_id && msg_id <= MSG_NPS_HTTP_CLOSE_CNF)
    {
        result = HandleNPSAppMsg(app_ptr, msg_id, param);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : register hello module nv len and max item
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC char* MMINPS_GetHttpParamUrl(void)
{
    int url_len = 0;
    int temp_len = 0;
    char* url_ptr = PNULL;
    char* score_ptr = PNULL;
    char* feedback_ptr = PNULL;
    char* email_ptr = PNULL;
    char* country_ptr = PNULL;
    char* operator_ptr = PNULL;
    char* time_ptr = PNULL;
    char* imei_ptr = PNULL;
    char* source_ptr = PNULL;
    char* device_ptr = PNULL;
    uint8*  product_ptr = PNULL;
    uint8*  product_ptr1 = PNULL;
    char* sw_ptr = PNULL;
    char* version_ptr = PNULL;
    char* temp_ptr1 = PNULL;
    char* temp_ptr2 = PNULL;
    char temp_buf1[1024*3] = {0};
    char temp_buf2[1024*6] = {0};
    MMI_STRING_T str_info = {0};
    MN_DUAL_SYS_E dual_sys = 0;
    MN_PLMN_T hplmn = {0};
    SCI_DATE_T date = {0};
    SCI_TIME_T time = {0};
    MN_IMEI_T imei_arr = {0};
    int i = 0;
    
    //get score param
#if 0
    GUIEDIT_GetString(NPS_SCORE_PRGBOX_CTRL_ID, &str_info);
    temp_len = strlen(HTTP_PARAM_SCORE) + str_info.wstr_len + 1;
    score_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(score_ptr, 0x00, temp_len);
    strcpy(score_ptr, HTTP_PARAM_SCORE);
    MMIAPICOM_WstrToStr(str_info.wstr_ptr, temp_buf1);
    strcat(score_ptr, temp_buf1);
    url_len = strlen(score_ptr);
    SCI_MEMSET(&str_info, 0x00, sizeof(str_info));
#endif
    temp_len = strlen(HTTP_PARAM_SCORE) + 2 + 1;
    score_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(score_ptr, 0x00, temp_len);
    strcpy(score_ptr, HTTP_PARAM_SCORE);
    MMIAPICOM_Int2Str((uint32)(s_score), temp_buf1, 2);
    strcat(score_ptr, temp_buf1); 
    url_len = strlen(score_ptr);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    
    //get feedback param
    GUIEDIT_GetString(NPS_REASON_EDIT_TEXT_CTRL_ID, &str_info);
    if(str_info.wstr_len != 0)
    {
        GUI_WstrToUTF8(temp_buf1, str_info.wstr_len*3 + 1, str_info.wstr_ptr, str_info.wstr_len);
        MMINPS_StrToEscapeStr(temp_buf2, temp_buf1);
        temp_len = strlen(HTTP_PARAM_FEEDBACK) + strlen(temp_buf2) + 1;
        feedback_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
        SCI_MEMSET(feedback_ptr, 0x00, temp_len);
        strcpy(feedback_ptr, HTTP_PARAM_FEEDBACK);
        strcat(feedback_ptr, temp_buf2);
        url_len += strlen(feedback_ptr);
        SCI_MEMSET(&str_info, 0x00, sizeof(str_info));
        SCI_MEMSET(temp_buf1, 0x00, 1024*3);
        SCI_MEMSET(temp_buf2, 0x00, 1024*6);
    }
    
    //get email param
    GUIEDIT_GetString(NPS_EMAIL_EDIT_TEXT_CTRL_ID, &str_info);
    if((str_info.wstr_len != 0) && (TRUE == s_nps_is_email_field_enable))
    {
        GUI_WstrToUTF8(temp_buf1, str_info.wstr_len*3 + 1, str_info.wstr_ptr, str_info.wstr_len);
        MMINPS_StrToEscapeStr(temp_buf2, temp_buf1);
        temp_len = strlen(HTTP_PARAM_EMAIL) + strlen(temp_buf2) + 1;
        email_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
        SCI_MEMSET(email_ptr, 0x00, temp_len);
        strcpy(email_ptr, HTTP_PARAM_EMAIL);
        strcat(email_ptr, temp_buf2);
        url_len += strlen(email_ptr);
        SCI_MEMSET(&str_info, 0x00, sizeof(str_info));
        SCI_MEMSET(temp_buf1, 0x00, 1024*3);
        SCI_MEMSET(temp_buf2, 0x00, 1024*6);
    }
    
    //get country param
    dual_sys = (MN_DUAL_SYS_E)MMIAPISET_GetActiveSim();
    hplmn = MNSIM_GetHplmnEx(dual_sys);
    temp_len = strlen(HTTP_PARAM_COUNTRY) + 3 + 1;
    country_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(country_ptr, 0x00, temp_len);
    strcpy(country_ptr, HTTP_PARAM_COUNTRY);
    MMIAPICOM_Int2Str((uint32)(hplmn.mcc), temp_buf1, 3);
    strcat(country_ptr, temp_buf1);
    url_len += strlen(country_ptr);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    
    //get operator param
    temp_len = strlen(HTTP_PARAM_OPERATOR) + 3 + 1;
    operator_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(operator_ptr, 0x00, temp_len);
    strcpy(operator_ptr, HTTP_PARAM_OPERATOR);
    MMIAPICOM_Int2Str((uint32)(hplmn.mnc), temp_buf1, 3);
    strcat(operator_ptr, temp_buf1);
    url_len += strlen(operator_ptr);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    
    //get dateEntered param
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    temp_len = strlen(HTTP_PARAM_TIME) + 11 + 8 + 1;
    time_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(time_ptr, 0x00, temp_len);
    strcpy(time_ptr, HTTP_PARAM_TIME);
    sprintf(temp_buf1,"%04d-%02d-%02dT", date.year, date.mon, date.mday);
    strcat(time_ptr, temp_buf1);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    sprintf(temp_buf1,"%02d:%02d:%02d", time.hour, time.min, time.sec);
    strcat(time_ptr, temp_buf1);
    url_len += strlen(time_ptr);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    
    //get imei param
    if(!MNNV_GetIMEIEx(dual_sys, imei_arr))
    {
        SCI_MEMSET(imei_arr, 0x00, sizeof(MN_IMEI_T));
    }
    for (i=0; i<8; i++) 
    {
        temp_buf1[2 * i] = (imei_arr[i]) & 0x0F;
        temp_buf1[2 * i + 1] = imei_arr[i] >> 4;
    }
    sprintf(temp_buf2, "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
        temp_buf1[1], temp_buf1[2], temp_buf1[3],temp_buf1[4], temp_buf1[5], temp_buf1[6], temp_buf1[7],temp_buf1[8], 
        temp_buf1[9], temp_buf1[10], temp_buf1[11],temp_buf1[12], temp_buf1[13], temp_buf1[14], temp_buf1[15]);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    MMI_SHA1Hash(temp_buf2, strlen(temp_buf2), temp_buf1);
    //MMIAPICOM_StrToWstr(tmp_buf1, wtemp);
    //SCI_MEMSET(tmp_buf1, 0x00, 20);
    //GUI_WstrToUTF8(tmp_buf1, MMIAPICOM_Wstrlen(wtemp)*3 + 1, wtemp, MMIAPICOM_Wstrlen(wtemp));
    SCI_MEMSET(temp_buf2, 0x00, 1024*6);
    //MMINPS_StrToEscapeStr(temp_buf2, temp_buf1);
    MMIAPICOM_BinaryToHexChar(temp_buf1, MAX(SHA1_HASH_SIZE,strlen(temp_buf1)), temp_buf2, &temp_len);
    temp_len = strlen(HTTP_PARAM_IMEI) + strlen(temp_buf2) + 1;
    imei_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(imei_ptr, 0x00, temp_len);
    strcpy(imei_ptr, HTTP_PARAM_IMEI);
    strcat(imei_ptr, temp_buf2);
    url_len += strlen(imei_ptr);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    SCI_MEMSET(temp_buf2, 0x00, 1024*6);
    
    //get source param
    if(s_is_prompt)
    {
        temp_len = strlen(HTTP_PARAM_SOURCE) + strlen(NPS_SOURCE_NOTIFICATION) + 1;
        source_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
        SCI_MEMSET(source_ptr, 0x00, temp_len);
        strcpy(source_ptr, HTTP_PARAM_SOURCE);
        strcpy(temp_buf1, NPS_SOURCE_NOTIFICATION);
        strcat(source_ptr, temp_buf1);
    }
    else
    {
        temp_len = strlen(HTTP_PARAM_SOURCE) + strlen(NPS_SOURCE_SETTINGS) + 1;
        source_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
        SCI_MEMSET(source_ptr, 0x00, temp_len);
        strcpy(source_ptr, HTTP_PARAM_SOURCE);
        strcpy(temp_buf1, NPS_SOURCE_SETTINGS);
        strcat(source_ptr, temp_buf1);
    }
    url_len += strlen(source_ptr);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    
    //get device param
#if defined(MS_DEVICE_INFO_SUPPORT) && ((!defined(WIN32)))
    SCI_TRACE_LOW("[NPS] get device param");
    product_ptr = PRODUCT_TYPE_Read();
    SCI_TRACE_LOW("[NPS] MMINPS_GetHttpParamUrl product_ptr = %s", product_ptr);
    SCI_TRACE_LOW("[NPS] MMINPS_GetHttpParamUrl product_ptr len = %d", SCI_STRLEN(product_ptr));
    temp_len = strlen(HTTP_PARAM_DEVICE) + TA_TYPE_LEN+1;//TA_TYPE_LEN
    product_ptr1 = (uint8*)SCI_ALLOC_APPZ(TA_TYPE_LEN+1);
    device_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(device_ptr, 0x00, temp_len);
    SCI_MEMSET(product_ptr1, 0x00, TA_TYPE_LEN+1);
    strncpy(product_ptr1, product_ptr, TA_TYPE_LEN);
    SCI_TRACE_LOW("[NPS] MMINPS_GetHttpParamUrl product_ptr1 = %s", product_ptr1);
    SCI_TRACE_LOW("[NPS] MMINPS_GetHttpParamUrl product_ptr1 len = %d", SCI_STRLEN(product_ptr1));
    strcpy(device_ptr, HTTP_PARAM_DEVICE);
    strcat(device_ptr, product_ptr1);
#else
    temp_len = strlen(HTTP_PARAM_DEVICE_WIN32) + 1;
    device_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(device_ptr, 0x00, temp_len);
    strcpy(device_ptr, HTTP_PARAM_DEVICE_WIN32);
#endif
    url_len += strlen(device_ptr);
    SCI_TRACE_LOW("[NPS] get device param device_ptr");
    
    //get sw param
#ifndef _WIN32  //need version new interface 
    temp_ptr1 = VERSION_Read(); //VERSION_GetInfo(PLATFORM_VERSION);   /*lint !e605*/
#else
    temp_ptr1 = (char*)"0.1723.2.11.13";
#endif
//     temp_ptr2 = strchr(temp_ptr1, 'M');
//     MMINPS_StrToEscapeStr(temp_buf1, temp_ptr2);
    temp_len = strlen(HTTP_PARAM_SW) + strlen(temp_ptr1) + 1;
    sw_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(sw_ptr, 0x00, temp_len);
    strcpy(sw_ptr, HTTP_PARAM_SW);
    strcat(sw_ptr, temp_ptr1);
    url_len += strlen(sw_ptr);
    SCI_MEMSET(temp_buf1, 0x00, 1024*3);
    temp_ptr1 = PNULL;
    temp_ptr2 = PNULL;
    
    //get version param
    temp_len = strlen(HTTP_PARAM_VERSION) + 1;
    version_ptr = (char *) SCI_ALLOC_APPZ(temp_len);
    SCI_MEMSET(version_ptr, 0x00, temp_len);
    strcpy(version_ptr, HTTP_PARAM_VERSION);
    url_len += strlen(version_ptr);
    
    url_ptr = (char *) SCI_ALLOC_APPZ(url_len + 1);
    SCI_MEMSET(url_ptr, 0x00, url_len + 1);
    strcpy(url_ptr, score_ptr);
    if(feedback_ptr != PNULL)
    {
        strcat(url_ptr, feedback_ptr);
    }
    if(email_ptr != PNULL)
    {
        strcat(url_ptr, email_ptr);
    }
    strcat(url_ptr, country_ptr);
    strcat(url_ptr, operator_ptr);
    strcat(url_ptr, time_ptr);
    strcat(url_ptr, imei_ptr);
    strcat(url_ptr, source_ptr);
    strcat(url_ptr, device_ptr);
    strcat(url_ptr, sw_ptr);
    strcat(url_ptr, version_ptr);
    
    SCI_FREE(score_ptr);
    SCI_FREE(feedback_ptr);
    SCI_FREE(email_ptr);
    SCI_FREE(country_ptr);
    SCI_FREE(operator_ptr);
    SCI_FREE(time_ptr);
    SCI_FREE(imei_ptr);
    SCI_FREE(source_ptr);
    SCI_FREE(product_ptr1);
    SCI_FREE(device_ptr);
    SCI_FREE(sw_ptr);
    SCI_FREE(version_ptr);
    
    SCI_TRACE_LOW("[NPS] MMINPS_GetHttpParamUrl url_ptr = %s\n", url_ptr);
    return url_ptr;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMINPS_HttpParamUrlFree(char* param_url)
{
    if(PNULL != param_url)
    {
        SCI_Free(param_url);
    }
}

/*****************************************************************************/
// Description : Convert str to Escape str(%X)
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: be sure of the range of dst
/*****************************************************************************/
LOCAL char* MMINPS_StrToEscapeStr(char *dst, const char *src)
{
    char *ret_str = dst;
    
    if (PNULL == dst || PNULL == src)
    {
        return dst;
    }
    
    while (*src)
    {
        if((MMINPS_IsAlpha(*src)) || (MMINPS_IsDigit(*src)) || (*src == '-')
            || (*src == '_') || (*src == '.') || (*src == '~'))
        {
            *dst++ = *src++;
        }
        else
        {
            *dst++ = '%';
            sprintf((char *)dst, "%02X", *((uint8 *)src)++);/*lint !e52*/
            dst +=2;
        }
    }
    
    *dst = 0;
    
    return ret_str;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_IsValidEmailAddress(char *addr)
{
    int addr_len = 0;
    int at_len = 0;
    int dot_len = 0;
    
    char *addr_at = NULL;
    char *addr_dot = NULL;    
    
    if(addr == NULL || *addr == 0)
    {
        return FALSE;
    }
    
    addr_len = strlen(addr);
    
    if((addr_len < NPS_EMAIL_ADDR_LEN_MIN) || (addr_len > NPS_EMAIL_ADDR_LEN_MAX))
    {
        //length not allowed
        return FALSE;
    }
    
    if(*addr == '@')
    {
        //first is @
        return FALSE;
    }
    
    addr_at = strchr(addr, '@');
    if(addr_at == NULL)
    {
        //no @
        return FALSE;
    }
    
    addr_dot = strchr(addr_at, '.');
    if(addr_dot == NULL)
    {
        //no .
        return FALSE;
    } 
    
    at_len = strlen(addr_at);
    dot_len = strlen(addr_dot);
    if((at_len - dot_len) <= 1)
    {
        // *@*.* 
        return FALSE;
    }
    if(dot_len == 1)
    {
        //last is .
        return FALSE;
    }
    
    while(*addr)
    {
        if(!MMINPS_IsEmailChar(*addr))
        {
            //valid char
            return FALSE;            
        }
        addr++;
    }
    
    return TRUE;
    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_IsEmailChar(char c)
{
    if(MMINPS_IsDigit(c) || MMINPS_IsAlpha(c) || c == '&' || c == '.' || c == '!' || c == '#'
        || c == '$' || c == '%' || c == '\'' || c == '*' || c == '+' || c == '-' || c == '/'
        || c == '=' || c == '?' || c == '^'  || c == '_' || c == '`' || c == '{' || c == '|'
        || c == '}' || c == '~' || c == '@')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_IsAlpha(char c)
{
    if((c >= 'a' && c<= 'z') ||(c >= 'A' && c<= 'Z'))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMINPS_IsDigit(char c)
{
    if(c >= '0' && c<= '9')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : sha1 algorithm without key
//  Global resource dependence :  none
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMI_SHA1Hash(const uint8 *src_ptr, uint32 src_length, uint8 *dest_ptr)
{
    psDigestContext_t Sha1Ctx;
    if ((NULL == src_ptr) || (NULL == dest_ptr))
    {
        return FALSE;
    }
    psSha1Init(&Sha1Ctx);
    psSha1Update(&Sha1Ctx, src_ptr, src_length);
    psSha1Final(&Sha1Ctx, dest_ptr);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : sha1 algorithm with key
//  Global resource dependence :  none
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/
LOCAL void psSha1Init(psDigestContext_t* md)
{
    //psAssert(md != NULL);
    md->sha1.state[0] = 0x67452301UL;
    md->sha1.state[1] = 0xefcdab89UL;
    md->sha1.state[2] = 0x98badcfeUL;
    md->sha1.state[3] = 0x10325476UL;
    md->sha1.state[4] = 0xc3d2e1f0UL;
    md->sha1.curlen = 0;
#ifdef HAVE_NATIVE_INT64
    md->sha1.length = 0;
#else
    md->sha1.lengthHi = 0;
    md->sha1.lengthLo = 0;
#endif /* HAVE_NATIVE_INT64 */
}

/*****************************************************************************/
//  Description : sha1 algorithm with key
//  Global resource dependence :  none
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/

LOCAL void psSha1Update(psDigestContext_t * md, const unsigned char *buf, uint32 len)
{
    uint32 n;
    
    //psAssert(md != NULL);
    //psAssert(buf != NULL);
    if(PNULL == md || PNULL == buf)
    {
        return;
    }
    while (len > 0) {
        n = min(len, (64 - md->sha1.curlen));
        memcpy(md->sha1.buf + md->sha1.curlen, buf, (size_t)n);
        md->sha1.curlen		+= n;
        buf					+= n;
        len					-= n;
        
        /* is 64 bytes full? */
        if (md->sha1.curlen == 64) {
            sha1_compress(md);
#ifdef HAVE_NATIVE_INT64
            md->sha1.length += 512;
#else
            n = (md->sha1.lengthLo + 512) & 0xFFFFFFFFL;
            if (n < md->sha1.lengthLo) {
                md->sha1.lengthHi++;
            }
            md->sha1.lengthLo = n;
#endif /* HAVE_NATIVE_INT64 */
            md->sha1.curlen = 0;
        }
    }
} 

/*****************************************************************************/
//  Description : modify MMIPUB_TEXT_CTRL_ID's lineSpace
//  Global resource dependence :  none
//  Author: ak.wang
//  Note: just for fix 1044586
/*****************************************************************************/
LOCAL MMI_RESULT_E MMINPS_ModifyTextCtrlLineSpace( MMI_WIN_ID_T win_id)
{
        MMI_HANDLE_T    ctrl_handle = PNULL;    
        MMIPUB_INFO_T   *win_info_ptr = PNULL;

        win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if (win_info_ptr == PNULL)
        {
            return MMI_RESULT_FALSE;
        }
        ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );
        if (ctrl_handle == PNULL)
        {
            return MMI_RESULT_FALSE;
        }
        //default value is 4.
        GUITEXT_SetLineSpace( ctrl_handle, 3); 

        return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : sha1 algorithm with key
//  Global resource dependence :  none
//  Author: bingjie.chen
//  Note: 
/*****************************************************************************/
LOCAL int32 psSha1Final(psDigestContext_t* md, unsigned char *hash)
{
    int32	i;
#ifndef HAVE_NATIVE_INT64
    uint32	n;
#endif
    //psAssert(md != NULL);
    if (PNULL == md)
    {
        return 0;// PS_ARG_FAIL;
    }
    
    if (md->sha1.curlen >= sizeof(md->sha1.buf) || hash == NULL) {
        //psTraceCrypto("psSha1Final error\n");
        return 0;//PS_ARG_FAIL;
    }
    
    /*
    increase the length of the message
    */
#ifdef HAVE_NATIVE_INT64
    md->sha1.length += md->sha1.curlen << 3;
#else
    n = (md->sha1.lengthLo + (md->sha1.curlen << 3)) & 0xFFFFFFFFL;
    if (n < md->sha1.lengthLo) {
        md->sha1.lengthHi++;
    }
    md->sha1.lengthHi += (md->sha1.curlen >> 29);
    md->sha1.lengthLo = n;
#endif /* HAVE_NATIVE_INT64 */
    
    /*
    append the '1' bit
    */
    md->sha1.buf[md->sha1.curlen++] = (unsigned char)0x80;
    
    /*
    if the length is currently above 56 bytes we append zeros then compress.
    Then we can fall back to padding zeros and length encoding like normal.
    */
    if (md->sha1.curlen > 56) {
        while (md->sha1.curlen < 64) {
            md->sha1.buf[md->sha1.curlen++] = (unsigned char)0;
        }
        sha1_compress(md);
        md->sha1.curlen = 0;
    }
    
    /*
    pad upto 56 bytes of zeroes
    */
    while (md->sha1.curlen < 56) {
        md->sha1.buf[md->sha1.curlen++] = (unsigned char)0;
    }
    
    /*
    store length
    */
#ifdef HAVE_NATIVE_INT64
    STORE64H(md->sha1.length, md->sha1.buf+56);
#else
    STORE32H(md->sha1.lengthHi, md->sha1.buf+56);
    STORE32H(md->sha1.lengthLo, md->sha1.buf+60);
#endif /* HAVE_NATIVE_INT64 */
    sha1_compress(md);
    
    /*
    copy output
    */
    for (i = 0; i < 5; i++) {
        STORE32H(md->sha1.state[i], hash+(4*i));
    }
    memset(md, 0x0, sizeof(psDigestContext_t));
    return SHA1_HASH_SIZE;
}
