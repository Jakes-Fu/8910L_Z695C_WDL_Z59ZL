/*****************************************************************************
** File Name:      mmiidle_mstyle.c                                          *
** Author:                                                                   *
** Date:           09/09/2015                                                *
** Copyright:      2015 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe microsoft style idle screen                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 9/2015     grant.peng              Creat
******************************************************************************/
#ifndef _MMIIDLE_MSTYLE_H_
#define _MMIIDLE_MSTYLE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmiidle_export.h"
#include "mmimp3_export.h"
#include "mmifm_internal.h"
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


    
#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)|| defined (FM_SUPPORT)
    typedef struct
    {
        BOOLEAN     is_scroll; //是否滚动
        uint16      str_width;   //mp3名字的宽度
        uint16      str_index;  //mp3名字中字符的索引
    } IDLE_MP3_NAME_INFO;
#endif

    /**--------------------------------------------------------------------------*
     **                         TYPE AND STRUCT                                  *
     **--------------------------------------------------------------------------*/
    typedef enum
    {
        IDLE_UNLOCKED,              //解锁。
        IDLE_UNLOCKED_ONCE,       //解锁操作一次。此时界面提示"按*键解锁"。
        IDLE_LOCKED,            //锁定。此时界面中软键显示"解锁"。
        IDLE_LOCKED_FROM_MAINMENU,   //从Mainmenu进入锁屏界面。
        IDLE_STATUS_MAX
    }MMI_IDLE_LOCK_STATUS_E;

    /**--------------------------------------------------------------------------*
     **                         EXTERNAL DEFINITION                              *
     **--------------------------------------------------------------------------*/

    /**--------------------------------------------------------------------------*
     **                         FUNCTION DEFINITION                              *
     **--------------------------------------------------------------------------*/
    /*****************************************************************************/
    //  Description : handle microsoft style idle message
    //  Global resource dependence :
    //  Author: grant.peng
    //  Note:
    /*****************************************************************************/
    PUBLIC MMI_RESULT_E MMIIDLE_MStyleHandleMsg ( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param );

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)|| defined (FM_SUPPORT)
    /*****************************************************************************/
    //  Description : set idle mp3 name
    //  Global resource dependence :
    //  Author: Jassmine.Meng
    //  Note:
    /*****************************************************************************/
    PUBLIC void MAIN_SetIdleBgPlayName(
        BOOLEAN     is_display,
        MMIBGPLAY_TYPE_E bg_type
    );
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : get idle mp3 info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_MP3_NAME_INFO* MMIIDLECOM_GetIdleMp3Info(void);
#endif

/*****************************************************************************/
//  Description : start mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLECOM_StartIdleMp3Timer(void);

/*****************************************************************************/
//  Description : stop mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLECOM_StopIdleMp3Timer(void);

/*****************************************************************************/
// Description : UpdateMp3NameStartIndex
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIIDLECOM_UpdateMp3NameStartIndex(void);

/*****************************************************************************/
// Description : MMIIDLECOM_GetMp3NameTimerId
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC uint8 MMIIDLECOM_GetMp3NameTimerId(void);

/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/

    
/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: grant.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateDynamicCtrl(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_IsDisplayEmergencyNum(BOOLEAN is_display);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_IsCorrectCode(const wchar *code_ptr, uint16 pwd_len);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_EnterControlPanel(void *para1, void *para2);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_SwtichTorchOnOff(void);
#ifdef MMI_UNISOC_IDLE_EMERGENCY_CALL_SUPPORT
/*****************************************************************************/
//  Description : handle emergency call in idle 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIdle_MsgidToAscii(
                                   MMI_MESSAGE_ID_E msg_id
                                   );
/*****************************************************************************/
//  Description : handle emergency call in idle 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIdle_IsEmergencyCallOnly(MMI_MESSAGE_ID_E msg_id);
/*****************************************************************************/
//  Description : handle  idle emergency call message
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIdle_OpenEmergencyWin(MMI_MESSAGE_ID_E msg_id);
#endif                                   
    /**--------------------------------------------------------------------------*
     **                         Compiler Flag                                    *
     **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_MMIIDLE_MSTYLE_H_
