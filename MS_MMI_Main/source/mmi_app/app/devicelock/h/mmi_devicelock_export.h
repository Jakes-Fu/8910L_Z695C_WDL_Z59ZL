/***************************************************************************
** File Name:      ***_export.h                                             *
** Author:                                                                 *
** Date:                                                          *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
**                                   *
**																		   *
****************************************************************************/
#ifndef _MMI_DEVICELOCK_EXPORT_H_
#define _MMI_DEVICELOCK_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_devicelock_id.h"
#include "mmiset_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

#define ACLOCK12_TIME      12

#define GET_12STYLE_HOUR(hour, is_am)    \
do  \
{   \
    if (0 == (hour))    \
    {   \
        (hour) = ACLOCK12_TIME;    \
    }   \
    else if((hour) >= ACLOCK12_TIME)   \
    {   \
        if ((hour) > ACLOCK12_TIME)    \
        {   \
            (hour) = (hour) - ACLOCK12_TIME;   \
        }   \
        (is_am) = FALSE;    \
    }	\
}while(0);

#define MMI_DEVICELOCK_PWD_MIN_LEN         4
#define MMI_DEVICELOCK_PWD_MAX_LEN         8
#define MMI_DEVICELOCK_INIT_PWD			"1234"


/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

typedef struct
{
    BOOLEAN     is_enable;                          //密码是否开启
    uint8       pwd[MMI_DEVICELOCK_PWD_MAX_LEN];
    uint8       pwd_len;                            //密码长度
}MMI_DEVICELOCK_PWD_T;


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDEVICELOCK_InitModule(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDEVICELOCK_Init(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_EnterInputSecurityWin(void);

/*****************************************************************************/
// 	Description : lock phone
//	Global resource dependence :
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_LockPhone(void);

/*****************************************************************************/
// 	Description : lock phone
//	Global resource dependence :
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_IsPhoneLocked(void);

/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_UnlockPhone(void);

/*****************************************************************************/
//  Description : unlock phone
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_IsDeviceLockWinOnTop(void);

/*****************************************************************************/
// 	Description : lock phone
//	Global resource dependence :
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDEVICELOCK_SendHourOrMinuteInd(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_EnableDisplayNotification
(
 MMISET_NOTIFICATION_TYPE_E notifi_type,
 BOOLEAN enable
 );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDEVICELOCK_SetDisplayNotification
(
 MMISET_NOTIFICATION_TYPE_E notifi_type,
 BOOLEAN display,
 BOOLEAN update_timestamp,
 MMI_IMAGE_ID_T icon_id
 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEVICELOCK_SetPwd(const wchar *pwd_ptr, uint16 pwd_len);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEVICELOCK_SetPwdEnable(BOOLEAN is_enable);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEVICELOCK_ReadPwd(MMI_DEVICELOCK_PWD_T* devicelock_code);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_MMI_DEVICELOCK_EXPORT_H_
