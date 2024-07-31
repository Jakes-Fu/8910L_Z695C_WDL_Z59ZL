/*************************************************************************
 ** File Name:      ***_nv.h                                        *
 ** Author:                                                  *
 ** Date:                                                   *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 **                               *
*************************************************************************/

#ifndef _MMI_DEVICELOCK_NV_H_
#define _MMI_DEVICELOCK_NV_H_

#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_DEVICELOCK_NOTIFICATION_MESSAGES = MMI_MODULE_DEVICELOCK << 16, /*MMISET_NOTIFICATION_MESSAGES*/
    MMINV_DEVICELOCK_NOTIFICATION_CALLS,/*MMISET_NOTIFICATION_CALLS*/
    MMINV_DEVICELOCK_NOTIFICATION_ALARMS,/*MMISET_NOTIFICATION_ALARMS*/
    MMINV_DEVICELOCK_NOTIFICATION_CALENDAR,/*MMISET_NOTIFICATION_CALENDAR*/
    MMINV_DEVICELOCK_NOTIFICATION_OUTLOOK,/*MMISET_NOTIFICATION_OUTLOOK*/
    MMINV_DEVICELOCK_NOTIFICATION_MAILS,/*MMISET_NOTIFICATION_MAILS*/
    MMINV_DEVICELOCK_NOTIFICATION_OPERATOR,/*MMISET_NOTIFICATION_OPERATOR*/
    MMINV_DEVICELOCK_NOTIFICATION_RADIO,/*MMISET_NOTIFICATION_RADIO*/
    MMINV_DEVICELOCK_NOTIFICATION_MUSIC,/*MMISET_NOTIFICATION_MUSIC*/
    MMINV_DEVICELOCK_PWD,	
    MMINV_DEVICELOCK_MAX_ITEM_NUM

}DEVICELOCK_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
			 MMI_WriteNVItem(_item_id, _addr)

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDEVICELOCK_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_DEVICELOCK_NV_H_

