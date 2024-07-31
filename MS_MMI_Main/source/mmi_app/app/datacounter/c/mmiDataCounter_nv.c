/*************************************************************************
** File Name:      mmiDataCounter_nv.c                                         *
** Author:         zhanglu                                         *
** Date:           2015/08/18                                           *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*                           *
*************************************************************************/


#define _MMISET_NV_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmidatacounter_nv.h"
#include "dal_time.h"
#include "datacounter_internal.h"

/*the length of set nv*/
const uint16 peroid_nv_len[] =
{
	sizeof(BOOLEAN),  //MMI_DATACOUNTER_SIM1_PEROID_NV_FLAG
	sizeof(BOOLEAN),  //MMI_DATACOUNTER_SIM2_PEROID_NV_FLAG
	sizeof(MMI_DATACOUNTER_DATE_NV_E),//MMI_DATACOUNTER_SIM1_SAVE_SELECT_DATE
	sizeof(MMI_DATACOUNTER_DATE_NV_E),//MMI_DATACOUNTER_SIM2_SAVE_SELECT_DATE

       sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_DATA_ALL_LIMIT,
	sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_DATA_SENT_LIMIT,
       sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_DATA_RECEIVED_LIMIT,
       sizeof(uint8),//MMI_DATACOUNTER_SIM1_DATA_SAVE_SELECT_ITEM,
       sizeof(uint8),//MMI_DATACOUNTER_SIM2_DATA_SAVE_SELECT_ITEM,
       
        sizeof(uint8),//MMI_DATACOUNTER_SIM1_CALL_SAVE_SELECT_ITEM,
        sizeof(uint8),//MMI_DATACOUNTER_SIM2_CALL_SAVE_SELECT_ITEM,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_DATA_ALL_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_CALL_OUTGOING_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_CALL_INCOMING_LIMIT,
        
        sizeof(uint8),//MMI_DATACOUNTER_SIM1_SMS_SAVE_SELECT_ITEM,
        sizeof(uint8),//MMI_DATACOUNTER_SIM2_SMS_SAVE_SELECT_ITEM,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_SMS_ALL_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_SMS_SENT_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_SMS_RECEIVED_LIMIT,
        
        sizeof(uint8),//MMI_DATACOUNTER_SIM1_MMS_SAVE_SELECT_ITEM,
        sizeof(uint8),//MMI_DATACOUNTER_SIM2_MMS_SAVE_SELECT_ITEM,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_MMS_ALL_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_MMS_SENT_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM1_MMS_RECEIVED_LIMIT,

        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_DATA_ALL_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_DATA_SENT_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_DATA_RECEIVED_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_CALL_ALL_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_CALL_OUTGOING_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_CALL_INCOMING_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_SMS_ALL_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_SMS_SENT_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_SMS_RECEIVED_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_MMS_ALL_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_MMS_SENT_LIMIT,
        sizeof(MMI_DATACOUNTER_DATA_SET_TYPE),//MMI_DATACOUNTER_SIM2_MMS_RECEIVED_LIMIT,
        sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E),//MMI_DATACOUNTER_SIM1_DATA_RECORD,
        sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E),//MMI_DATACOUNTER_SIM1_CALL_RECORD,
        sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E),//MMI_DATACOUNTER_SIM1_SMS_RECORD,
        sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E),//MMI_DATACOUNTER_SIM1_MMS_RECORD,
        sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E),//MMI_DATACOUNTER_SIM2_DATA_RECORD,
        sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E),//MMI_DATACOUNTER_SIM2_CALL_RECORD,
        sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E),//MMI_DATACOUNTER_SIM2_SMS_RECORD,
        sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E),//MMI_DATACOUNTER_SIM2_MMS_RECORD,
        sizeof(DATACOUNTER_SHOW_WIN_JUST_ONE_E),//MMI_DATACOUNTER_SIM1_CALL_SHOW_WIN_NV,
        sizeof(DATACOUNTER_SHOW_WIN_JUST_ONE_E),//MMI_DATACOUNTER_SIM2_CALL_SHOW_WIN_NV,
        sizeof(DATACOUNTER_SHOW_WIN_JUST_ONE_E),//MMI_DATACOUNTER_SIM1_DATA_SHOW_WIN_NV,
        sizeof(DATACOUNTER_SHOW_WIN_JUST_ONE_E),//MMI_DATACOUNTER_SIM2_DATA_SHOW_WIN_NV,
        sizeof(DATACOUNTER_SHOW_WIN_JUST_ONE_E),//MMI_DATACOUNTER_SIM1_SMS_SHOW_WIN_NV,
        sizeof(DATACOUNTER_SHOW_WIN_JUST_ONE_E),//MMI_DATACOUNTER_SIM2_SMS_SHOW_WIN_NV,
        sizeof(DATACOUNTER_SHOW_WIN_JUST_ONE_E),//MMI_DATACOUNTER_SIM1_MMS_SHOW_WIN_NV,
        sizeof(DATACOUNTER_SHOW_WIN_JUST_ONE_E),//MMI_DATACOUNTER_SIM2_MMS_SHOW_WIN_NV,
#ifdef MMI_JEWISH_CALENDAR
    // Due to T-upgrade reason, can not modify the order of current nv items, new items can be only added at the end of the whole nv list
    // In uis8910 queen project ,the last module is datacounter, so move below items to module datacounter.   it should be in MMI_MODULE_ACC.
        sizeof(uint8),				  // MMINV_JEWISH_CALENDAR_LOCATION_INFO
        sizeof(uint8),				  // MMINV_JEWISH_CALENDAR_DLS_SETTING
#endif
};

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIDATACOUNTER_NV_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_DATA_COUNTER, peroid_nv_len, sizeof(peroid_nv_len) / sizeof(uint16));
}



