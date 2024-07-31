/****************************************************************************
** File Name:      mmieng_export.h							                *
** Author:         allen								                    *
** Date:           2012.02.03												*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    									               			*
*****************************************************************************
**                         Important Edit History                        	*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             	*
** 08/2004      allen               Create
** 02/2012      xiyuan.ma           Edit
****************************************************************************/
#ifndef  _MMIENG_EXPORT_H_    
#define  _MMIENG_EXPORT_H_   

#include "mmiparse_export.h"

//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmieng_main.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_main.h"
#include "mmimbbms_main.h"
#include "mbbms_service.h"
#endif
#include "adpcmrecord_adp.h"
#include "usbservice_api.h"
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/
#define MMIENG_MAX_URL_LEN		512		

//@songbin.zeng,2006-08-08
typedef enum
{
	MMIENG_SENSOR_MODE_DEFAULT = 0,
	MMIENG_SENSOR_MODE_RGB, 
	MMIENG_SENSOR_MODE_YUV,
	MMIENG_SENSOR_MODE_MAX
} MMIENG_SENSOR_MODE_E;

//@songbin.zeng,2006-12-05
typedef enum
{
	MMIENG_DV_FRAME_FRE_MIN, 
	MMIENG_DV_FRAME_FRE_7HZ    = 7,
	MMIENG_DV_FRAME_FRE_10HZ   = 10,
	MMIENG_DV_FRAME_FRE_15HZ   = 15, 
	MMIENG_DV_FRAME_FRE_20HZ   = 20, 
	MMIENG_DV_FRAME_FRE_25HZ   = 25, 
	MMIENG_DV_FRAME_FRE_30HZ   = 30, 
	MMIENG_DV_FRAME_FRE_MAX 
} MMIENG_DV_FRAME_FRE_E;

//@songbin.zeng,2006-12-05
typedef enum
{
    MMIENG_IDLE_DIAL_NUM_MIN,
    MMIENG_IDLE_DIAL_NUM_UI_TEST1,
    MMIENG_IDLE_DIAL_NUM_UI_TEST2,
    MMIENG_IDLE_DIAL_NUM_UI_TEST3,
    MMIENG_IDLE_DIAL_NUM_PRODUCT,
    MMIENG_IDLE_DIAL_NUM_PHONE_INFO,
    MMIENG_IDLE_DIAL_NUM_SET_MAX_POWER,
    MMIENG_IDLE_DIAL_NUM_PRODUCT_SN,
    MMIENG_IDLE_DIAL_NUM_SHOW_NET,
    MMIENG_IDLE_DIAL_NUM_MENU,
    MMIENG_IDLE_DIAL_NUM_SW_VERSION,//[UI spec][Call UI]Dialer code definitions
    MMIENG_IDLE_DIAL_NUM_LCD_INFO,
    MMIENG_IDLE_DIAL_NUM_BATRY_INFO,
    MMIENG_IDLE_DIAL_NUM_LEGAL_STATEMENT,    
    MMIENG_IDLE_DIAL_NUM_INDIA_SAR,    
    MMIENG_IDLE_DIAL_NUM_BT_ADDRESS,
    MMIENG_IDLE_DIAL_NUM_LIFE_TIMER,
    MMIENG_IDLE_DIAL_NUM_SHOW_VERSION1,
    MMIENG_IDLE_DIAL_NUM_SHOW_VERSION2,
    MMIENG_IDLE_DIAL_NUM_CHIP_TEST,
    MMIENG_IDLE_DIAL_NUM_CMMB_TEST,
    MMIENG_IDLE_DIAL_NUM_MONKEY_TEST_AUTOSET,
    #ifdef TF_LOAD_SUPPORT
    MMIENG_IDLE_DIAL_NUM_TFLOAD_ENTER,
    #endif        
    MMIENG_IDLE_DIAL_NUM_NV_COUNTING,
    //gmw---begin
    MMIENG_IDLE_DIAL_CONFIG_NV_TEST,
	//gmw---end
#ifdef MEDIA_DEMO_SUPPORT
    MMIENG_IDLE_DIAL_NUM_AUDIO_DEMO,
    MMIENG_IDLE_DIAL_NUM_VIDEO_DEMO,
    MMIENG_IDLE_DIAL_NUM_MEDIA_DEMO,//for video demos
#endif
#ifdef MET_MEX_SUPPORT    
	MMIENG_IDLE_DIAL_NUM_MET_MEX,
#endif

#ifdef AUTODEMO_SUPPORT
    MMI_AUTODEMO_WINDOW,
#endif

#ifdef MCARE_V31_SUPPORT
	MMIENG_IDLE_DIAL_MCARE_TEST,
#endif
#ifdef NORMAL_TO_DEBUG_MODE
	MMIENG_IDLE_DIAL_NUM_AUTO_LOG,
	MMIENG_IDLE_DIAL_NUM_LOG_OFF,
#endif
	MMIENG_IDLE_DIAL_GPRS_SHOW,
	MMIENG_IDLE_PHONE_LOCK_SHOW,
	MMIENG_IDLE_DIAL_BACKLIGHT_TIMEOUT,//nhl add backlight value
	MMIENG_BT_LOG_ENABLE,
	MMIENG_BT_LOG_DISABLE,
	MMIENG_VIDEO_MODEONE_TEST_ENABLE,
	MMIENG_VIDEO_MODEONE_TEST_DISABLE,
	MMIENG_FLY_BT_FM_TEST_ENABLE,
		MMIENG_FIRST_TIME,
	MMIENG_CLEAR_TIME,
	  MMIENG_SAVE_LOG,
	MMIENG_IDLE_AGINGTEST_UI,
#if defined(ZDT_SHOW_SIM_NETWORK_RSSI_IN_FACTORY_MODE)   
	 MMIENG_SIM_NETWORK_RSSI,
#endif	 
    	MMIENG_IDLE_DIAL_NUM_MAX
} MMIENG_IDLE_DIAL_NUM_TYPE_E;

typedef enum
{
    MMIENG_CHIP_TEST_SETTING_ON = 0,
    MMIENG_CHIP_TEST_SETTING_OFF, 
    MMIENG_CHIP_TEST_SETTING_MAX
} MMIENG_CHIP_TEST_SETTING_E;

typedef enum
{
    MMIENG_UPDATE_TEST_NEVER,
    MMIENG_UPDATE_TEST_DAY,
    MMIENG_UPDATE_TEST_WEEK,
    MMIENG_UPDATE_TEST_MONTH,
    MMIENG_UPDATE_TEST_MAX,
} MMIENG_UPDATE_TEST_MODE_E;

typedef enum
{
    MMIENG_IQ_MODE_A,
    MMIENG_IQ_MODE_B,
    MMIENG_IQ_MODE_C,
/*[START] Jerry Liang for IQ MODE 2008/07/31*/
    MMIENG_IQ_MODE_D,
/*[END] Jerry Liang for IQ MODE 2008/07/31*/    
    MMIENG_IQ_MODE_CUSTOMIZED,
    MMIENG_IQ_MODE_MAX
}MMIENG_IQ_MODE_E;

typedef struct
{
    BOOLEAN        b_status;//是否开启IQ模式
    BOOLEAN        b_is_start;//是否已经启动IQ数据
    BOOLEAN        b_power_on_start;//是否开机时启动
    uint16         cur_mode;//当前选择的模式
    RECORD_PARAM_T record_param[MMIENG_IQ_MODE_MAX];
}MMIENG_IQ_MODE_T;

typedef struct
{
    uint8					user_agent[MMIENG_MAX_URL_LEN + 1];	
    uint8					uaprofile[MMIENG_MAX_URL_LEN + 1];//默认设置的索引
}MMIENG_UA_T;//ua的信息

typedef enum
{
    MMIENG_SCREEN_LOG_AVALID_MEM,
    MMIENG_SCREEN_LOG_LCD,
    MMIENG_SCREEN_LOG_APP,
    MMIENG_SCREEN_LOG_LAYER_NUM,
	MMIENG_SCREEN_LOG_MAX
} MMIENG_SCREEN_LOG_E;

#if defined ENG_NETWORK_SET //zhouqin
typedef enum
{
    MMIENG_LTE_CS_VOICE_ONLY,
    MMIENG_LTE_CS_VOICE_PREFER,
    MMIENG_LTE_PS_VOICE_PREFER,
    MMIENG_LTE_PS_VOICE_ONLY,    
    MMIENG_LTE_SERVIC_MAX
} MMIENG_LTE_SERVICE_E;
typedef enum
{
    MMIENG_LTE_PS_MODE_2,
    MMIENG_LTE_CS_PS_MODE_1,
    MMIENG_LTE_CS_PS_MODE_2,
    MMIENG_LTE_PS_MODE_1,
    MMIENG_LTE_OPERATION_MODE_MAX
} MMIENG_LTE_MODE_E;

typedef struct
{
    MMIENG_LTE_SERVICE_E LTE_Service;	
    MMIENG_LTE_MODE_E Operation_Mode;	
	
}MMIENG_NETWORK_T;
#endif

#if defined ENG_BAND_FREQ_SUPPORT //zhouqin
#define LTE_BAND_MAX  20 //the max support LTE band num
#define LTE_BAND_NUM 17    //the LTE band already support
#define GSM_BAND_MAX 4
#define ENG_FREQ_MAX_NUM          9 //Freq max number
typedef struct
{
    uint16 LTE_band[LTE_BAND_MAX];	
    BOOLEAN GSM_band[GSM_BAND_MAX];	
    uint32 Freq_list[ENG_FREQ_MAX_NUM];
    uint32 GSM_Freq[ENG_FREQ_MAX_NUM];	

}MMIENG_BAND_FREQ_T;
#endif

#define MMIENG_SCREEN_DEBUG_NONE             0
#define MMIENG_SCREEN_DEBUG_STRING          (MMIENG_SCREEN_DEBUG_NONE+1)
#define MMIENG_SCREEN_DEBUG_RECT            (MMIENG_SCREEN_DEBUG_NONE+2)
#define MMIENG_SCREEN_DEBUG_MAX             (MMIENG_SCREEN_DEBUG_NONE+3)
typedef uint8 MMIENG_SCREEN_DEBUG_T;


#ifdef ENG_SUPPORT
/********************************************************************************
 NAME:			MMIAPIENG_SaveLastTime
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENG_SaveLastTime(void);

/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENG_AppInit(void);

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_RegNv(void);

/********************************************************************************
 NAME:			MMIAPIENG_RedkeyIsEnableInIdle
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RedkeyIsEnableInIdle(void);

/********************************************************************************
 NAME:			MMIAPIENG_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartEngineerMenu(void);
PUBLIC BOOLEAN MMIAPIENG_StartIMEIEdit(void);
//[UI spec][Call UI]Dialer code definitions
PUBLIC MMI_HANDLE_T MMI_CreateTextSwVersionWin(void);
PUBLIC MMI_HANDLE_T MMI_CreateLcdInfoWin(void);

PUBLIC MMI_HANDLE_T MMI_CreateTextLegalStatementWin(void);
PUBLIC MMI_HANDLE_T MMI_CreateTextIndiaSARWin(void);
PUBLIC MMI_HANDLE_T MMI_CreateTextBTAddressWin(void);
PUBLIC MMI_HANDLE_T MMI_CreateTextLifeTimerWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC MMIENG_SENSOR_MODE_E MMIAPIENG_GetSensorMode(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetSensorMode(MMIENG_SENSOR_MODE_E sensor_mode);

/********************************************************************************
 NAME:			MMIAPIENG_VibrateWinGetFocus
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_VibrateWinGetFocus(void);

/********************************************************************************
 NAME:			MMIAPIENG_IsFocusInLoopBack
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsFocusInLoopBack(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowOutVersionWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowOutVersionWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateProductWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateProductWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreatePhoneInfoWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreatePhoneInfoWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetMaxPowerOff
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetMaxPowerOff(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateProductSNWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
BOOLEAN MMIAPIENG_CreateProductSNWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowNetWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowNetWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowNetWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIENG_HandlePlmnCnf( DPARAM param);

/********************************************************************************
 NAME:			MMIAPIENG_Netinfo_Statistics_ShowWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_Netinfo_Statistics_ShowWin(void);



/********************************************************************************
 NAME:			MMIAPIENG_BandSelectCnf
 DESCRIPTION:	
 PARAM IN:		None 
 PARAM OUT:		None
 AUTHOR:		songbin.zeng
 DATE:			2006.11.16
********************************************************************************/
PUBLIC void MMIAPIENG_BandSelectCnf(DPARAM  param);

/*****************************************************************************/
// 	Description : to append one item to listbox style by self
//	Global resource dependence : 
//  Author:songbin.zeng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_AppendListItemStyleBySelf(									
                                                MMI_CTRL_ID_T        ctrl_id,
                                                MMI_TEXT_ID_T        text_id,
                                                uint16                      item_pos,
                                                MMI_IMAGE_ID_T      left_icon_id,
                                                MMI_IMAGE_ID_T      right_icon_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        mid_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                BOOLEAN                 is_item_sel
                                                );

/********************************************************************************
 NAME:			MMIAPIENG_CreateRFWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.11.22
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateRFWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetDVFrameFre(MMIENG_DV_FRAME_FRE_E dv_frame_frequency);

/********************************************************************************
 NAME:			MMIAPIENG_GetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC MMIENG_DV_FRAME_FRE_E MMIAPIENG_GetDVFrameFre(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateWinDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateWinDVFrameFre(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateChipTestWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2007.7.31
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateChipTestWin(void);

#ifdef CMMB_SUPPORT
/********************************************************************************
 NAME:			MMIAPIENG_CreateCMMBTestWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.07.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateCMMBTestWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateCMMBTestWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.07.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateCMMBChannelStatusWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_InitMTV
 DESCRIPTION:	MTV初始化
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_InitMTV(void);

/********************************************************************************
 NAME:			MMIAPIENG_ReleaseMTV
 DESCRIPTION:	MTV释放
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_ReleaseMTV(void);

/********************************************************************************
 NAME:			MMIAPIENG_ReleaseMTV
 DESCRIPTION:	MTV停止流程
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2010-12-30
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StopMTVFlow(void);

/********************************************************************************
 NAME:			MMIAPIENG_PlayMTVService
 DESCRIPTION:	直播
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_PlayMTVService(uint32 net_id, uint32 service_id);

/********************************************************************************
 NAME:			MMIAPIENG_MTVRFTest
 DESCRIPTION:	RF测试(用于无信号时的工厂测试)
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_MTVRFTest(uint32 net_frequency);

/********************************************************************************
 NAME:			MMIAPIENG_GetMTVFlowInfo
 DESCRIPTION:	获取状态信息
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MMIMBBMS_FLOW_INFO_T* MMIAPIENG_GetMTVFlowInfo(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetPlayResult
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MBBMS_SERVICE_RESULT_E MMIAPIENG_GetPlayResult(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetMTVChannelDisplayMode
 DESCRIPTION:	获取Channel信息显示模式
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MMIENG_MTV_CHANNEL_DISP_MODE_E MMIAPIENG_GetMTVChannelDisplayMode(void);
#endif//CMMB_SUPPORT

/********************************************************************************
 NAME:			MMIAPIENG_GetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2007.08.08
********************************************************************************/
PUBLIC MMIENG_CHIP_TEST_SETTING_E MMIAPIENG_GetChipTestMode(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2006.08.08
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetChipTestMode(MMIENG_CHIP_TEST_SETTING_E test_mode);

/*****************************************************************************/
// 	Description : start chip test
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIENGAPI_StartChipTest(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallForwardQuery(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetAutoAnswerCallSet(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetAutoAnswerCallSet
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC void MMIAPIENG_SetAutoAnswerCallSet(
                                           BOOLEAN   is_on
                                           );

/********************************************************************************
 NAME:			MMIAPIENG_SetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC void MMIAPIENG_SetCallForwardQuery(
                                           BOOLEAN   is_on
                                           );

/********************************************************************************
 NAME:			MMIAPIENG_GetIsCardLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsCardLogOn(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIsBrowserLogOn 
//	Global resource dependence : none
//  Author: sally.he
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsBrowserLogOn(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIsScreenLogOn 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_SCREEN_DEBUG_T MMIAPIENG_GetIsScreenLogOn(void);

/*****************************************************************************/
// 	Description : get user wre url change 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsBrowserChangeURL(void);

/*****************************************************************************/
// 	Description : set user wre url change 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIsBrowserChangeURL(BOOLEAN is_URL_change);

/*****************************************************************************/
// 	Description : MMIAPIENG_SetIsScreenLogOn 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIsScreenLogOn(MMIENG_SCREEN_DEBUG_T type);

/*****************************************************************************/
// 	Description : MMIAPIENG_SetScreenTag 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetScreenTag( const char* tag, MMIENG_SCREEN_LOG_E type, uint32 ticks );

/*****************************************************************************/
// 	Description : MMIAPIENG_AppendScreenTicks 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
//void MMIAPIENG_AppendScreenTicks( MMIENG_SCREEN_LOG_E type );

/*****************************************************************************/
// 	Description : MMIAPIENG_PrintScreenLog 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_PrintScreenLog( void );

/*****************************************************************************/
// 	Description : display debug rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_DisplayDebugRect(
                                       GUI_RECT_T update_rect     //in:
                                       );

/*****************************************************************************/
// 	Description : clear debug rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_ClearDebugRect( void );

/*****************************************************************************/
// 	Description : is debug layer
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsDebugLayer( 
                                      GUI_LCD_DEV_INFO const *dev_info_ptr
                                      );

/********************************************************************************
 NAME:			MMIAPIENG_StartupAdjustCardLogOn
 DESCRIPTION:	开机调整CardLog状态
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			2007.10.11
********************************************************************************/
PUBLIC void MMIAPIENG_StartupAdjustCardLogOn(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_SetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIQMode(MMIENG_IQ_MODE_T *iq_mode);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_IQ_MODE_T MMIAPIENG_GetIQMode(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQModeStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQModeStatus(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQDataStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQDataStatus(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQPowerOnStartStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQPowerOnStartStatus(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenIQDataWin
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQDataWin(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_StartIQDataAcquire
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartIQData(void);

/*****************************************************************************/
//  Description : 停止录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIENG_StopENGRecord(void);

/*****************************************************************************/
//  Description : MMIENG_DeletRecordFile
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIENG_DeletRecordFile(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_StopQData
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_StopQData(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_AlertIQMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_AlertIQMode(void);

/*****************************************************************************/
// 	Description : Open IQ Data Query Win
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_OpenHandleIQDataQueryWin(BOOLEAN is_succeed);
/*****************************************************************************/
// 	Description : is arm log on when power on
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsUSBLOGON(void);

/*****************************************************************************/
// 	Description : is eng band select cmd
//	Global resource dependence : 
//  Author: jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsBandSelectWinOpen(void);

/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_GetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC  BOOLEAN MMIAPIENG_GetCMCCTestFlag(void);

/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_SetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetCMCCTestFlag(BOOLEAN *pTestFlag);

#else  //ENG_SUPPORT
/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENG_AppInit(void);

/********************************************************************************
 NAME:			MMIAPIENG_RedkeyIsEnableInIdle
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RedkeyIsEnableInIdle(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC MMIENG_SENSOR_MODE_E MMIAPIENG_GetSensorMode(void);

/********************************************************************************
 NAME:			MMIAPIENG_VibrateWinGetFocus
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_VibrateWinGetFocus(void);

/********************************************************************************
 NAME:			MMIAPIENG_IsFocusInLoopBack
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
//PUBLIC BOOLEAN MMIAPIENG_IsFocusInLoopBack(void);

/********************************************************************************
 NAME:			MMIAPIENG_BandSelectCnf
 DESCRIPTION:	
 PARAM IN:		None 
 PARAM OUT:		None
 AUTHOR:		songbin.zeng
 DATE:			2006.11.16
********************************************************************************/
PUBLIC void MMIAPIENG_BandSelectCnf(DPARAM  param);

/********************************************************************************
 NAME:			MMIAPIENG_GetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC MMIENG_DV_FRAME_FRE_E MMIAPIENG_GetDVFrameFre(void);

/********************************************************************************
 NAME:			MMIAPIENG_DoIdleDialNumOpt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC void MMIAPIENG_DoIdleDialNumOpt(
                                       MMIENG_IDLE_DIAL_NUM_TYPE_E opt_type
                                       );

/********************************************************************************
 NAME:			MMIAPIENG_ParseIdleDialNumString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_ParseIdleDialNumString(
                                                uint8* num_str_ptr, 
                                                uint8 num_str_len,
                                                MMIENG_IDLE_DIAL_NUM_TYPE_E *opt_type_ptr,//OUT
                                                parsCtrlCodeE *ctrl_code_ptr//OUT
                                                );

/********************************************************************************
 NAME:			MMIAPIENG_GetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2007.08.08
********************************************************************************/
PUBLIC MMIENG_CHIP_TEST_SETTING_E MMIAPIENG_GetChipTestMode(void);

/*****************************************************************************/
// 	Description : start chip test
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIENGAPI_StartChipTest(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallForwardQuery(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetAutoAnswerCallSet(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetIsCardLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsCardLogOn(void);

/********************************************************************************
 NAME:			MMIAPIENG_StartupAdjustCardLogOn
 DESCRIPTION:	开机调整CardLog状态
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			2007.10.11
********************************************************************************/
PUBLIC void MMIAPIENG_StartupAdjustCardLogOn(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_IQ_MODE_T MMIAPIENG_GetIQMode(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQModeStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQModeStatus(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenIQDataWin
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQDataWin(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_AlertIQMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_AlertIQMode(void);

/*****************************************************************************/
// 	Description : Open IQ Data Query Win
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_OpenHandleIQDataQueryWin(BOOLEAN is_succeed);

/*****************************************************************************/
// 	Description : is arm log on when power on
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsUSBLOGON(void);

/*****************************************************************************/
// 	Description : is eng band select cmd
//	Global resource dependence : 
//  Author: jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsBandSelectWinOpen(void);

/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_GetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC  BOOLEAN MMIAPIENG_GetCMCCTestFlag(void);
#endif//ENG_SUPPORT

/********************************************************************************
 NAME:			GetWapMmsSasTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		hui.zhao
 DATE:			2009.07.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetWapMmsSasTestMode(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowOutVersionWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowOutVersionWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_StartUITest
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartUITest(void);
#ifdef NORMAL_TO_DEBUG_MODE
PUBLIC BOOLEAN MMIAPIENG_SwitchAutoUsbLogOn(void);
PUBLIC BOOLEAN MMIAPIENG_SwitchAutoUsbLogOff(void);
#endif
/********************************************************************************
 NAME:			MMIAPIENG_DoIdleDialNumOpt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC void MMIAPIENG_DoIdleDialNumOpt(
                                       MMIENG_IDLE_DIAL_NUM_TYPE_E opt_type
                                       );


/********************************************************************************
 NAME:			MMIAPIENG_ParseIdleDialNumString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_ParseIdleDialNumString(
                                                uint8* num_str_ptr, 
                                                uint8 num_str_len,
                                                MMIENG_IDLE_DIAL_NUM_TYPE_E *opt_type_ptr,//OUT
                                                parsCtrlCodeE *ctrl_code_ptr//OUT
                                                );

/********************************************************************************
 NAME:			MMIAPIENG_ParseIdleSetLanguageTypeNumString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			2009.2.25
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_ParseIdleSetLanguageTypeNumString(
                                                           uint8* num_str_ptr, 
                                                           uint8 num_str_len,
                                                           MMISET_LANGUAGE_TYPE_E *language_type_ptr
                                                           );

/********************************************************************************
 NAME:			MMIAPIENG_DoIdleSetLanguageTypeNumOpt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaoghui
 DATE:			2009.2.25
********************************************************************************/
PUBLIC void MMIAPIENG_DoIdleSetLanguageTypeNumOpt(
                                                  MMISET_LANGUAGE_TYPE_E language_type
                                                  );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_InitModule(void);

/********************************************************************************
 NAME:			MMIAPIENG_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC void MMIAPIENG_SetPktOutPut(void);

/*****************************************************************************/
//  Description : MMIAPIENG_GetUa,when eng ua is open, return the eng ua info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetUa(MMIENG_UA_T *ua_ptr);

/*****************************************************************************/
//  Description : MMIAPIENG_SetUa
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetUa(MMIENG_UA_T *ua_ptr);



/********************************************************************************
 NAME:			MMIAPIENG_GetCallBarring
 DESCRIPTION:	MS00186848
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallBarring(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetCallBarring
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC void MMIAPIENG_SetCallBarring(BOOLEAN is_on);

/*****************************************************************************/
// 	Description : restore factory setting of ENG
//	Global resource dependence : 
//  Author: fen.xie
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIENG_RestoreFactorySetting(void);


#ifdef TF_LOAD_SUPPORT
/********************************************************************************
 NAME:			MMIAPIENG_CreateTFloadWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: tao.feng
 DATE: 2010.12.16
********************************************************************************/
BOOLEAN MMIAPIENG_CreateTFloadWin(void);
#endif

#ifdef ENG_SUPPORT
/*****************************************************************************/
// 	Description : open cardlog win
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenCardLogWin(void);

/*****************************************************************************/
// 	Description : open IQ Mode win
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQModeOnOffWin(void);

#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : is smd counter enable
//	Global resource dependence : 
//  Author: lisa.lin
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsSMDCounterEnable(void);

/*****************************************************************************/
// 	Description : is interface testing eable
//	Global resource dependence : 
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsInterfaceTestingEnable(void);
#endif //MBBMS_SUPPORT

#ifdef CMMB_SUPPORT
/*****************************************************************************/
// 	Description : is smd counter enable
//	Global resource dependence : 
//  Author: lisa.lin
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsAudioServiceEnable(void);
#endif //CMMB_SUPPORT

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenBrowserLogWin
//	Global resource dependence : 
//  Author: li.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenBrowserLogWin(void);

/*****************************************************************************/
//  Description : browser change url by wre
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenBrowserChangeURLWin(void);

/*****************************************************************************/
//  Description : open screenlog win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenScreenLogWin(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenUserAgentWin
//	Global resource dependence : 
//  Author: li.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenUserAgentWin(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenUaProfileWin
//	Global resource dependence : 
//  Author: li.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenUaProfileWin(void);

#endif//ENG_SUPPORT

#ifdef MMI_AUTOTEST_SUPPORT
/*****************************************************************************/
//  Description : open auto test set win
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenAutoTestSetWin(uint8 set_id);
#endif

#ifdef WIFI_SUPPORT
/*****************************************************************************/
//  Description : open WIFI performance  win
//  Global resource dependence : 
//  Author: George.Liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWlanPerformanceModeWin(void);

/*****************************************************************************/
//  Description : open WIFI iperf  win
//  Global resource dependence : 
//  Author: YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWifiIperfWin(void);


/*****************************************************************************/
//  Description : open WIFI radio frequency win
//  Global resource dependence : 
//  Author: George.Liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWlanRadioFrequencyWin(void);
#endif


/********************************************************************************
 NAME:			MMIAPIENG_RunUITestNextStep
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RunUITestNextStep(void);

/********************************************************************************
 NAME:			MMIAPIENG_SaveTestResult
 DESCRIPTION:	api of MMIAPIENG_SaveTestResult
 AUTHOR:		ying.xu
 DATE:			2010.8.10
********************************************************************************/
PUBLIC uint32 MMIAPIENG_SaveTestResult(uint32 win_id, uint32 result);

/*****************************************************************************/
//  Description :get power key flag
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetPowerKeyFlag(void);

/*****************************************************************************/
//  Description :set power key flag
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetPowerKeyFlag(BOOLEAN is_enable);

/*****************************************************************************/
// 	Description :  MMIAPIENGTestMode Write
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIAPIENGTestMode_Write(BOOLEAN value);

/*****************************************************************************/
// 	Description : MMIAPIENGTestMode  IsSet
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIAPIENGTestMode_IsSet(void);

/*****************************************************************************/
// 	Description :  MMIAPIENGUSBTestSet_Write
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/

PUBLIC BOOLEAN MMIAPIENGUSBTestSet_Write(USB_SERVICE_E usb_mode);

/*****************************************************************************/
// 	Description : MMIAPIENGUSBTestSet_GetMode
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/

PUBLIC USB_SERVICE_E MMIAPIENGUSBTestSet_GetMode(void);

/*****************************************************************************/
// 	Description : MMIAPIENGESDTestMode  IsSet
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIAPIENGESDTestMode_IsSet(void);

#ifdef WIFI_SUPPORT
/********************************************************************************
NAME:          MMIAPIENG_IsWifiUITest
DESCRIPTION:   is wifi ui test
AUTHOR:        jian.ma
DATE:    
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsWifiUITest(void);
#endif

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowOutVersionWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowVersionWin(void);

#ifdef ENG_SECONDEDAC_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsSECONDEDACON(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetSECONDEDACON(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetSECONDEDACOFF(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SECONDEDAC_InitforPowerOn(void);
#endif

#ifdef MMI_UNISOC_MANUALLY_CIT_SUPPORT
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_OpenUnisocTestWin(void);

PUBLIC BOOLEAN MMIENG_OpenAgingTestWin(void);
#if defined(ZDT_SHOW_SIM_NETWORK_RSSI_IN_FACTORY_MODE)
PUBLIC BOOLEAN MMIENG_OpenUnisocSimnetworkTestWin(void);
#endif	
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartUnisocTest(void);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RunUnisocTestNextStep(void);
PUBLIC BOOLEAN MMIAPIENG_RunUnisocTestRESET(void);
#endif/*MMI_UNISOC_MANUALLY_CIT_SUPPORT*/

/*****************************************************************************/
//  Discription: MMIENG_GetMode_FLY_BT_FM
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetMode_FLY_Enter_BT_FM(void);
PUBLIC void MMIENG_FLY_Enter_BT_FM_ENABLE(void);
#ifdef FM_SUPPORT
/*****************************************************************************/
//  Discription: MMIAPIENG_AutoSearchCallback
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIENG_AutoSearchCallback(void);
#endif
#ifdef __cplusplus
    }
#endif
#endif
