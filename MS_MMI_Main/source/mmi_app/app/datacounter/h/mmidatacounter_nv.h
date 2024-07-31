/*************************************************************************
 ** File Name:      mmidatacounter_nv.h                                          *
 ** Author:         zhanglu                                       *
 ** Date:           2015/8/10                                          *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2015/8/10     zhanglu        Create.                              *
*************************************************************************/

#ifndef _MMIDATACOUNTER_NV_H_
#define _MMIDATACOUNTER_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define DEFAULT_PEROID_VALUE    FALSE

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
        MMI_DATACOUNTER_SIM1_PEROID_NV_FLAG = MMI_MODULE_DATA_COUNTER<< 16,
	 MMI_DATACOUNTER_SIM2_PEROID_NV_FLAG,
        MMI_DATACOUNTER_SIM1_SAVE_SELECT_DATE,
        MMI_DATACOUNTER_SIM2_SAVE_SELECT_DATE,

        MMI_DATACOUNTER_SIM1_DATA_ALL_LIMIT,
        MMI_DATACOUNTER_SIM1_DATA_SENT_LIMIT,
        MMI_DATACOUNTER_SIM1_DATA_RECEIVED_LIMIT,
        MMI_DATACOUNTER_SIM1_DATA_SAVE_SELECT_ITEM,
        MMI_DATACOUNTER_SIM2_DATA_SAVE_SELECT_ITEM,
        
        MMI_DATACOUNTER_SIM1_CALL_SAVE_SELECT_ITEM,
        MMI_DATACOUNTER_SIM2_CALL_SAVE_SELECT_ITEM,
        MMI_DATACOUNTER_SIM1_CALL_ALL_LIMIT,
        MMI_DATACOUNTER_SIM1_CALL_OUTGOING_LIMIT,
        MMI_DATACOUNTER_SIM1_CALL_INCOMING_LIMIT,
        
        MMI_DATACOUNTER_SIM1_SMS_SAVE_SELECT_ITEM,
        MMI_DATACOUNTER_SIM2_SMS_SAVE_SELECT_ITEM,
        MMI_DATACOUNTER_SIM1_SMS_ALL_LIMIT,
        MMI_DATACOUNTER_SIM1_SMS_SENT_LIMIT,
        MMI_DATACOUNTER_SIM1_SMS_RECEIVED_LIMIT,
        
        MMI_DATACOUNTER_SIM1_MMS_SAVE_SELECT_ITEM,
        MMI_DATACOUNTER_SIM2_MMS_SAVE_SELECT_ITEM,
        MMI_DATACOUNTER_SIM1_MMS_ALL_LIMIT,
        MMI_DATACOUNTER_SIM1_MMS_SENT_LIMIT,
        MMI_DATACOUNTER_SIM1_MMS_RECEIVED_LIMIT,

        MMI_DATACOUNTER_SIM2_DATA_ALL_LIMIT,
        MMI_DATACOUNTER_SIM2_DATA_SENT_LIMIT,
        MMI_DATACOUNTER_SIM2_DATA_RECEIVED_LIMIT,
        MMI_DATACOUNTER_SIM2_CALL_ALL_LIMIT,
        MMI_DATACOUNTER_SIM2_CALL_OUTGOING_LIMIT,
        MMI_DATACOUNTER_SIM2_CALL_INCOMING_LIMIT,
        MMI_DATACOUNTER_SIM2_SMS_ALL_LIMIT,
        MMI_DATACOUNTER_SIM2_SMS_SENT_LIMIT,
        MMI_DATACOUNTER_SIM2_SMS_RECEIVED_LIMIT,
        MMI_DATACOUNTER_SIM2_MMS_ALL_LIMIT,
        MMI_DATACOUNTER_SIM2_MMS_SENT_LIMIT,
        MMI_DATACOUNTER_SIM2_MMS_RECEIVED_LIMIT,
        //save get record
        MMI_DATACOUNTER_SIM1_DATA_RECORD,
        MMI_DATACOUNTER_SIM1_CALL_RECORD,
        MMI_DATACOUNTER_SIM1_SMS_RECORD,
        MMI_DATACOUNTER_SIM1_MMS_RECORD,
        MMI_DATACOUNTER_SIM2_DATA_RECORD,
        MMI_DATACOUNTER_SIM2_CALL_RECORD,
        MMI_DATACOUNTER_SIM2_SMS_RECORD,
        MMI_DATACOUNTER_SIM2_MMS_RECORD,
        MMI_DATACOUNTER_SIM1_CALL_SHOW_WIN_NV,
        MMI_DATACOUNTER_SIM2_CALL_SHOW_WIN_NV,
        MMI_DATACOUNTER_SIM1_DATA_SHOW_WIN_NV,
        MMI_DATACOUNTER_SIM2_DATA_SHOW_WIN_NV,
        MMI_DATACOUNTER_SIM1_SMS_SHOW_WIN_NV,
        MMI_DATACOUNTER_SIM2_SMS_SHOW_WIN_NV,
        MMI_DATACOUNTER_SIM1_MMS_SHOW_WIN_NV,
        MMI_DATACOUNTER_SIM2_MMS_SHOW_WIN_NV,
#ifdef MMI_JEWISH_CALENDAR
    // Due to T-upgrade reason, can not modify the order of current nv items, new items can be only added at the end of the whole nv list
    // In uis8910 queen project ,the last module is datacounter, so move below items to module datacounter.   it should be in MMI_MODULE_ACC.
        MMINV_JEWISH_CALENDAR_LOCATION_INFO,
        MMINV_JEWISH_CALENDAR_DLS_SETTING,
#endif
	 SET_MAX_NV_ITEM_NUM

}SET_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIDATACOUNTER_NV_H_

