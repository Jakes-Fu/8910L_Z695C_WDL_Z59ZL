/*****************************************************************************
** File Name:                                                 *
** Author:                                             *
** Date:                                                         *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe kelock internal interface            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 0
******************************************************************************/

#ifndef _MMI_DEVICELOCK_INTERNAL_H_
#define _MMI_DEVICELOCK_INTERNAL_H_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guifont.h"

#include "mmi_devicelock_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/
#if 0
typedef enum
{
    DEVICELOCK_NOTIFICATION_NEW_MESSAGE,
    DEVICELOCK_NOTIFICATION_MISSED_CALL,
    DEVICELOCK_NOTIFICATION_MUSIC_PLAYER,
    DEVICELOCK_NOTIFICATION_FM,
    DEVICELOCK_NOTIFICATION_SNOOZED_ALARM,
    DEVICELOCK_NOTIFICATION_SNOOZED_CALENAR,
    DEVICELOCK_NOTIFICATION_VOICE_MAIL,
    DEVICELOCK_NOTIFICATION_MAILBOX,
    DEVICELOCK_NOTIFICATION_OPERATOR_SETTINGS,
    DEVICELOCK_NOTIFICATION_WINDOWS,
    DEVICELOCK_NOTIFICATION_MAX	
} DEVICELOCK_NOTIFICATION_TYPE_E;
#endif

typedef struct
{
    BOOLEAN 	has_indicator;
    MMI_IMAGE_ID_T icon;
    uint32 timestamp;//TM_GetTotalSeconds
}DEVICELOCK_NOTIFICATION_NV_STRUCT;

typedef struct
{
    MMISET_NOTIFICATION_TYPE_E type;
    BOOLEAN visible_enable;
    DEVICELOCK_NOTIFICATION_NV_STRUCT nv;
}DEVICELOCK_NOTIFICATION_STRUCT;

#define MMIDEVICELOCK_NOTIFICATION_DISPLAY_MAX        8
typedef struct
{
    uint8 index[MMIDEVICELOCK_NOTIFICATION_DISPLAY_MAX];
    uint8 num;    
}DEVICELOCK_NOTIFICATION_DISPLAY_STRUCT;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define DEVICELOCK_MASK_LAYER_ALPHA 204

#define DEVICELOCK_TIME_FONT SONG_FONT_48
#define DEVICELOCK_AMPM_FONT SONG_FONT_24
#define DEVICELOCK_SOFTKEY_FONT SONG_FONT_20

#define SOS_INPUT_MAX 3 //CC_EMERGENCY_CALL_NUM_MAX_LEN
#define DEVICELOCK_SOS_INPUT_DELAY 5000

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/

#ifdef   __cplusplus
    }
#endif

#endif

