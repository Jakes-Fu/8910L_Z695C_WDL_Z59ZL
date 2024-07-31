#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmikur_id.h                                               *
** Author:         cmzs                                                      *
** Date:           2007/10/22                                                *
** Description:    This file is used to describe kur win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/10/22     cmzs              Create                                   *
******************************************************************************/

#ifndef _MMIKUR_ID_H_
#define _MMIKUR_ID_H_ 

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
    MMIKUR_WIN_ID_START = (MMI_MODULE_KURO << 16),
    #include "mmikur_id.def"
    MMIKUR_MAX_WIN_ID
}MMIKUR_WINDOW_ID_E;

// control ID
typedef enum
{
    MMIKUR_CTRL_ID_ID_START = MMIKUR_MAX_WIN_ID,//(MMI_MODULE_KURO << 16),
    MMIKUR_EQ_SET_LIST_CTRL_ID,         //eq set list control id
    MMIKUR_PLAY_MODE_LIST_CTRL_ID,      //play mode set list control id
    MMIKUR_PLAY_LIST_CTRL_ID,           //KUR play list control id
    MMIKUR_PLAY_LIST_OPT_MENU_CTRL_ID,  //KUR play list option menu control id

    MMIKUR_DETAIL_NAME_TITLE_CTRL_ID,   //KUR detail name title label control id
    MMIKUR_DETAIL_NAME_CTRL_ID,         //KUR detail name label control id
    MMIKUR_DETAIL_MEM_TITLE_CTRL_ID,    //KUR detail memory title label control id
    MMIKUR_DETAIL_MEM_CTRL_ID,          //KUR detail memory label control id
    MMIKUR_DETAIL_POS_TITLE_CTRL_ID,    //KUR detail position title label control id
    MMIKUR_DETAIL_POS_CTRL_ID,          //KUR detail position label control id         
    MMIKUR_DETAIL_DATE_TITLE_CTRL_ID,   //KUR detail date title label control id
    MMIKUR_DETAIL_DATE_CTRL_ID,         //KUR detail date label control id
    MMIKUR_DETAIL_TIME_TITLE_CTRL_ID,   //KUR detail time title label control id
    MMIKUR_DETAIL_TIME_CTRL_ID,         //KUR detail time label control id 
    MMIKUR_DETAIL_SONG_TITLE_CTRL_ID,   //KUR detail song title label control id
    MMIKUR_DETAIL_SONG_CTRL_ID,         //KUR detail song label control id
    MMIKUR_DETAIL_SINGER_TITLE_CTRL_ID, //KUR detail singer title label control id
    MMIKUR_DETAIL_SINGER_CTRL_ID,       //KUR detail singer label control id

    MENU_KUR_PLAY_MENU_CTRL_ID,
    MMIKUR_HELP_TB_CTRL_ID, //help window
    MMIKUR_PLAYLIST_CTRL_ID,
    MMIKUR_MAX_CTRL_ID
}MMIKUR_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIMP3_ID_H_
#endif
