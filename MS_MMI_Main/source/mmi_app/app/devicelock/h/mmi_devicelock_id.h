/*****************************************************************************
** File Name:      ***_id.h                                               *
** Author:                                                      *
** Date:                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ACC win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**                                  *
******************************************************************************/

#ifndef _MMI_DEVICELOCK_ID_H_
#define _MMI_DEVICELOCK_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          win_id

// window ID
typedef enum
{
    MMI_DEVICELOCK_WIN_ID_START = (MMI_MODULE_DEVICELOCK << 16),        
    #include "mmi_devicelock_id.def"    
    MMI_DEVICELOCK_MAX_WIN_ID
}MMI_DEVICELOCK_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMI_DEVICELOCK_CTRL_ID_START = MMI_DEVICELOCK_MAX_WIN_ID,
    
    MMI_DEVICELOCK_ANIM_BG_ID,
    MMI_DEVICELOCK_CLOCK_ANIM_ID,
    MMI_DEVICELOCK_SECURITY_BG_ID,
    MMI_DEVICELOCK_SECURITY_ANIM_ID,
    MMI_DEVICELOCK_SECURITY_PWD_ID,
    MMI_DEVICELOCK_MAX_CTRL_ID
}MMI_DEVICELOCK_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIDEVICELOCK_RegWinIdNameArr(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_DEVICELOCK_ID_H_
