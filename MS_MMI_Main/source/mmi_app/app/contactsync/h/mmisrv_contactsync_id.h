/*****************************************************************************
** File Name:      mmibt_id.h                                                   *
** Author:         kelly.li                                                         *
** Date:           2006/08/21                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe fm win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/08/21     kelly.li      Create                                    *
******************************************************************************/

#ifdef MMIPB_SYNC_WITH_O365

#ifndef PBSYNC_ID_H_
#define PBSYNC_ID_H_ 

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
	PBSYNC_WIN_ID_START = (MMI_MODULE_CONTACTSYNC << 16),
	PBSYNC_MAIN_MENU_WIN_ID,
	PBSYNC_WAIT_WIN_ID,
	PBSYNC_COMPLETE_WIN_ID,
	PBSYNC_MAX_WIN_ID
}PBSYNC_WINDOW_ID_E;

#undef WIN_ID_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

#endif //MMIPB_SYNC_WITH_O365