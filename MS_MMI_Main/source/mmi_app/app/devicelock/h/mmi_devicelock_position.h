/*****************************************************************************
** File Name:      mmi_keylock_position.h                                             *
** Author:         Yintang.Ren                                                 *
** Date:           15/08/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe kelock interface            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011       Yintang.Ren	       Create
******************************************************************************/

#ifndef _MMI_DEVICELOCK_POSITION_H_
#define _MMI_DEVICELOCK_POSITION_H_

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
		
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


#define TIME_AREA_TOP_MARGIN 24
#define TIME_AREA_LEFT_MARGIN 6
#define TIME_AREA_HEIGHT 68
#define TIME_AMPM_INTERVAL 4

#define NOTIFICATION_TOP_MARGIN 4
#define NOTIFICATION_INTERVAL_MARGIN 4
#define NOTIFICATION_WIDTH_HEIGHT 26
#define NOTIFICATION_AREA_TOP_MARGIN (TIME_AREA_TOP_MARGIN + TIME_AREA_HEIGHT + NOTIFICATION_TOP_MARGIN)

#define SOFTKEY_ICON_LEFT_OR_RIGHT_MARGIN 0
#define SOFTKEY_ICON_BOTTOM_MARGIN 0

#define SECURITY_SOFTKEY_BAR_HEIGHT 38
#define SECURITY_PWD_BOTTOM_MARGIN (SECURITY_SOFTKEY_BAR_HEIGHT + 89)
#define SECURITY_PWD_HEIGHT 66
#define SECURITY_ANIM_BOTTOM_INTERVAL_MARGIN 6
#define SECURITY_ANIM_BOTTOM_MARGIN (SECURITY_PWD_BOTTOM_MARGIN + SECURITY_PWD_HEIGHT + SECURITY_ANIM_BOTTOM_INTERVAL_MARGIN)
#define SECURITY_ANIM_RIGHT_MARGIN 84
#define SECURITY_ANIM_WIDTH_HEIGHT 72

#define SECURITY_EMERGENCY_NUM_PROMPT_HEITHT 72



/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

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

