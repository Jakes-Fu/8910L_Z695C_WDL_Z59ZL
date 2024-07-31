#ifndef __DATA_COUNTER_INTERNAL__
#define __DATA_COUNTER_INTERNAL__

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmipub.h"
#include "mmiwclk_export.h"

#include "mmicc_nv.h"
#include "mmi_signal_ext.h"
#include "mmicc_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define DATACOUNTER_SENT_TYPE  0
#define DATACOUNTER_REV_TYPE    1
#define DATACOUNTER_OUTGOING_TYPE  0
#define DATACOUNTER_INCOMING_TYPE   1
#define DATACOUNTER_ALL_TYPE   2
#define DATACOUNTER_DATA_SENT_REV_LIMIT  100
#define DATACOUNTER_DATA_INPUT_MAX_LENGTH  5
#define DATACOUNTER_SMS_MMS_INPUT_MAX_LENGTH  5

#define DATACOUNTER_EDIT_HIGHT   16

#define DATACOUNTER_EDIT_X1      7//12
#define DATACOUNTER_EDIT_X2      90//50//63
#define DATACOUNTER_EDIT_X3      240//150//197

#define DATACOUNTER_EDIT_Y0	     MMI_TITLE_HEIGHT+1 + 50
#define DATACOUNTER_EDIT_Y1     DATACOUNTER_EDIT_Y0+22// 29//39//53
#define DATACOUNTER_EDIT_Y2      DATACOUNTER_EDIT_Y1+22//54//63//82
#define DATACOUNTER_EDIT_Y3      DATACOUNTER_EDIT_Y2+20//79//85//112

#define DATACOUNTER_MARGIN   5

#define CALL_HOUR_TO_SECOND					3600	//小时换算为秒
#define CALL_MINUTE_TO_SECOND					60		//分钟换算为秒
#define CALL_DISPLAY_TIME_LEN                                 20

#define DATACOUNTER_TEXT_LEN 128

typedef enum
{
	MMI_SmartDS_SIM_ID_1 = 0,
	MMI_SmartDS_SIM_ID_2 = 1,
	#if defined(MMI_MULTI_SIM_SYS_TRI)
	MMI_SmartDS_SIM_ID_3 = 2,
	#endif
	#if defined(MMI_MULTI_SIM_SYS_QUAD)
	MMI_SmartDS_SIM_ID_4 = 3,
	#endif
}MMI_SmartDS_SIM_ID;

typedef enum
{
    DATACOUNTER_MOVE_PREV_DAY,
    DATACOUNTER_MOVE_NEXT_DAY,
    DATACOUNTER_MOVE_PREV_WEEK,
    DATACOUNTER_MOVE_NEXT_WEEK,
}DATACOUNTER_MOVE_MODE_E;

typedef enum
{
    DATACOUNTER_CALL_OUTGOING_DATATYPE = 0,
    DATACOUNTER_CALL_INCOMING_DATATYPE = 1,
    DATACOUNTER_CALL_ALL_DATATYPE = 2,
}DATACOUNTER_CALL_DATATYPE_E;

typedef enum
{
    DATACOUNTER_SENT_DATATYPE = 0,
    DATACOUNTER_RECV_DATATYPE = 1,
    DATACOUNTER_ALL_DATATYPE = 2,
}DATACOUNTER_DATATYPE_E;

typedef struct
{
    MN_DUAL_SYS_E simID;
    uint8 dataType;
} MMI_DATACOUNTER_DATA_E;

typedef struct
{
   uint8 clearFlag;//1:表示未清除0:表示未存储3:全部清除2:表示只清除所在记录4:表示重置日期
   wchar iccid[25];
   wchar buffer_ALL[20];
   wchar buffer_send_outgoing[20];
   wchar buffer_recv_incoming[20];
   uint32 pre_sendData;
   uint32 pre_recvData;
   uint32 recordClearSendData;
   uint32 recordClearRecvData;
}MMI_DATACOUNTER_RECORD_DATA_NV_E;

typedef struct
{
    SCI_DATE_T date;
    BOOLEAN dataResetFlag;//0:表示设置日期1:重置日期
}MMI_DATACOUNTER_DATE_NV_E;

typedef struct
{
    MMI_DATACOUNTER_DATA_E dataType;
    wchar limit[DATACOUNTER_DATA_INPUT_MAX_LENGTH+1];
    uint8  unit;//0:min/mb;1:h/gb
} MMI_DATACOUNTER_DATA_SET_TYPE;

typedef struct
{
    uint8 dataType;
    BOOLEAN showJustOne;
}DATACOUNTER_SHOW_WIN_JUST_ONE_E;

PUBLIC void MMIAPIDATACOUNTER_OpenResultWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenDataWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenCallWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenSmsWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenMmsWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenPeroidWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenDataAlertWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenSmsAlertWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenMmsAlertWin(void* param);
PUBLIC void MMIAPIDATACOUNTER_OpenCallAlertWin(void* param);
PUBLIC void ReadResetPeriodDate(MMI_DATACOUNTER_DATE_NV_E* date_nv_e);
PUBLIC void GetCallLogRecord(
	MN_DUAL_SYS_E dual_sys, 
       BOOLEAN isPeroidSet,
	uint32 mday,
       uint32 mon, 
       uint32 year, 
       MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv);
PUBLIC void LoadDataNV(MN_DUAL_SYS_E simCard, BOOLEAN isPeroidSet, uint8 nDate,
	uint16 nYear, uint8 nMonth, uint8 monthDay, MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv);
PUBLIC void ClearSmsNV(uint8 clearFlag, MN_DUAL_SYS_E sim_card);
PUBLIC void ClearMmsNV(uint8 clearFlag, MN_DUAL_SYS_E sim_card);
PUBLIC void ClearCallNV(uint8 clearFlag, MN_DUAL_SYS_E sim_card);
PUBLIC void ClearDataNV(uint8 clearFlag, MN_DUAL_SYS_E sim_card);
void readIncomingCallDuration(MN_DUAL_SYS_E dual_sys, MMI_STRING_T* time_str_ptr);
void readOutgoingCallDuration(MN_DUAL_SYS_E dual_sys, MMI_STRING_T* time_str_ptr);
void readAllCallDuration(MN_DUAL_SYS_E dual_sys, MMI_STRING_T* time_str_ptr);
LOCAL void call_TimeCountToStr(MMI_STRING_T* time_str_ptr, uint32 time_count);

PUBLIC void MMIDATACOUNTER_NV_RegNv(void);
PUBLIC void GetSIMIccId(MN_DUAL_SYS_E simCard, wchar* iccid);
PUBLIC void ClearDataCounterPeriodAllRecord(uint8 clearFlag, MN_DUAL_SYS_E sim_card);
PUBLIC BOOLEAN CheckSameSimCard(MN_DUAL_SYS_E simCard, uint32 item_id);
PUBLIC void ResetCallShowJustOne(MN_DUAL_SYS_E dual_sys, uint8 dataType, BOOLEAN showjustOne);
PUBLIC void ResetDataShowJustOne(MN_DUAL_SYS_E dual_sys, uint8 dataType, BOOLEAN showjustOne);
PUBLIC void ResetSmsShowJustOne(MN_DUAL_SYS_E dual_sys, uint8 dataType, BOOLEAN showjustOne);
PUBLIC void ResetMmsShowJustOne(MN_DUAL_SYS_E dual_sys, uint8 dataType, BOOLEAN showjustOne);
PUBLIC void FormatDataFlowrate(MMI_STRING_T *result_str, uint8 data_buff_length, uint32 data_flowrate);
PUBLIC void MMIDataCounter_FormatTimeStrByTime(
                                        uint32 duration_time,
                                        MMI_STRING_T *p_time_str
                                        );

PUBLIC void ClearDataCounterPartRecord(uint8 clearFlag, MN_DUAL_SYS_E sim_card);

PUBLIC void MMIAPIDataCounter_ThresholdTips(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *text1_ptr,
                               MMI_STRING_T *text2_ptr, MMIPUB_HANDLE_FUNC handle_func);
#ifdef MMI_SMS_CHAT_SUPPORT
PUBLIC void MMIDATACounter_GetSmsRecord(MN_DUAL_SYS_E dual_sys, BOOLEAN isPeroidSet,
	uint8 mday,
       uint8 mon, 
       uint16 year, 
       MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv);

PUBLIC void MMIDATACounter_GetMmsRecord(MN_DUAL_SYS_E dual_sys, BOOLEAN isPeroidSet,
	uint8 mday,
       uint8 mon, 
       uint16 year, 
       MMI_DATACOUNTER_RECORD_DATA_NV_E* record_data_nv);
#endif
PUBLIC void MMIAPIDataCounter_SetSMSSimID(MN_DUAL_SYS_E SimId);
PUBLIC MN_DUAL_SYS_E MMIAPIDataCounter_GetSMSSimID(void);
PUBLIC void MMIAPIDataCounter_SetMMSSimID(MN_DUAL_SYS_E SimId);
PUBLIC MN_DUAL_SYS_E MMIAPIDataCounter_GetMMSSimID(void);
PUBLIC void MMIAPIDataCounter_SetDataSimID(MN_DUAL_SYS_E SimId);
PUBLIC MN_DUAL_SYS_E MMIAPIDataCounter_GetDataSimID(void);
PUBLIC void MMIAPIDataCounter_SetCallSimID(MN_DUAL_SYS_E SimId);
PUBLIC MN_DUAL_SYS_E MMIAPIDataCounter_GetCallSimID(void);

LOCAL uint8 select_index = 0;
LOCAL MMI_DATACOUNTER_DATA_E m_dataType = {0};
LOCAL uint8 item_select_state = 0;

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
