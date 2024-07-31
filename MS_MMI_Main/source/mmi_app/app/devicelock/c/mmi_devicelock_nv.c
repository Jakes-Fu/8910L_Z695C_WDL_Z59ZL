/*************************************************************************
 ** File Name:                                           *
 ** Author:                             *
 ** Date:                                     *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 **                 *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmi_devicelock_nv.h"
#include "mmi_modu_main.h"
#include "mmi_devicelock_internal.h"
#include "mmi_devicelock_export.h"

const uint16 devicelock_nv_len[] =
{
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_MESSAGES*/
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_CALLS*/
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_ALARMS*/
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_CALENDAR*/
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_OUTLOOK*/
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_MAILS*/
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_OPERATOR*/
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_RADIO*/
    sizeof(DEVICELOCK_NOTIFICATION_NV_STRUCT), /*MMINV_DEVICELOCK_NOTIFICATION_MUSIC*/
    sizeof(MMI_DEVICELOCK_PWD_T), /*MMINV_DEVICELOCK_PWD*/
};
/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//   Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIDEVICELOCK_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_DEVICELOCK, devicelock_nv_len, sizeof(devicelock_nv_len) / sizeof(uint16));
}
