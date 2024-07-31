/*****************************************************************************
** File Name:      mmikur_bt.c                                               *
** Author:                                                                   *
** Date:           01/02/2008                                                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2008       Cmzs                Create                                  *
******************************************************************************/
#include "std_header.h"
#define _MMIKUR_BT_C_
#ifdef KURO_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmikur_internal.h"
#include "mmiaudio_ctrl.h"
#include "mmibt_a2dp.h"
#include "mmibt_app.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:process blue tooth a2dp avrcp of play/pause mp3
//  Global resource dependence:
//  Author: kelly.li
//  Note:
/*****************************************************************************/
#ifdef  BLUETOOTH_SUPPORT
PUBLIC void MMIAPIKUR_HandleA2dpMsg(
                MMIBT_FUNCTION_TYPE_E action_type
                )
{
    switch(action_type) 
    {
    case MMIBT_OPEN_DEVICE:
    case MMIBT_PAUSE_DEVICE:
        switch(MMIKUR_GetPlayerState())
        {
            case MMIKUR_PLAYER_STATE_NONE:
                //NOT TO PLAY
                break;
                
            case MMIKUR_PLAYER_AUDIO_ACTIVE:
                if (MMIKUR_AUDIO_PAUSED == MMIKUR_GetAudioState())
                {
                    MMIAPIKUR_ResumeKurPlayer(TRUE);
                }
                else if (MMIKUR_AUDIO_PLAYING == MMIKUR_GetAudioState())
                {
                    MMIAPIKUR_PauseKurPlayer(TRUE);
                }
                break;
                
            case MMIKUR_PLAYER_MUSIC_SWITCH:
            case MMIKUR_PLAYER_SWITCH_PAUSED:
                break;
                
            default:
                break;
        }
        break;

    case MMIBT_CLOSE_DEVICE:
        if (MMIKUR_AUDIO_STOPPED != MMIKUR_GetAudioState())
        {
            MMIAPIKUR_StopKurPlayer();
        }
        break;
        
    default:
        break;
    }
}
#endif

#endif