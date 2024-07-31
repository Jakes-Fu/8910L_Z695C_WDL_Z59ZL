/*****************************************************************************
** File Name:      mmiDataCounter_export.h                                               
** Author:         zhanglu   
** Date:           2015/0730
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2015/07/30    zhanglu          Create        	                             *
******************************************************************************/

#ifndef _MMIDATA_COUNTER_EXPORT_H_
#define _MMIDATA_COUNTER_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//#ifdef DATA_COUNTER_SUPPORT
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmk_msg.h"
#include "mmidisplay_data.h"
#include "mmiacc_position.h"
#include "datacounter_internal.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//##endif//#ifdef DATA_COUNTER_SUPPORT

/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenMainWin
//         
//	Global resource dependence : 
//   Author:        zhanglu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIDATACOUNTER_OpenMainWin(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

PUBLIC void MMIAPIDATACOUNTER_InitModule(void);

PUBLIC void MMIAPIDataCounter_dataAlertNotifaction(MN_DUAL_SYS_E dual_sys, DATACOUNTER_DATATYPE_E dataType);
PUBLIC void MMIAPIDataCounter_callAlertNotifaction(MN_DUAL_SYS_E dual_sys, DATACOUNTER_CALL_DATATYPE_E dataType);
PUBLIC void MMIAPIDataCounter_smsAlertNotifaction(MN_DUAL_SYS_E dual_sys, DATACOUNTER_DATATYPE_E dataType);
PUBLIC void MMIAPIDataCounter_mmsAlertNotifaction(MN_DUAL_SYS_E dual_sys, DATACOUNTER_DATATYPE_E dataType);

#ifdef   __cplusplus
    }
#endif

#endif  


