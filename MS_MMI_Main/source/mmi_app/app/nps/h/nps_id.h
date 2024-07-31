/*****************************************************************************
** File Name:      nps_id.h                                                   *
** Author:         cheney.wu                                                         *
** Date:           2017/08/21                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe fm win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/08/21     kelly.li      Create                                    *
******************************************************************************/

#ifndef MMINPS_ID_H_
#define MMINPS_ID_H_ 

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
    NPS_WIN_ID_START = (MMI_MODULE_NPS << 16),
    MMINPS_MAIN_MENU_WIN_ID,
    MMINPS_MAIN_SELECT_WIN_ID,
    MMINPS_FEEDBACK_SEND_WIN_ID,
    MMINPS_FEEDBACK_SENDING_WIN_ID,
    MMINPS_INSERT_SIM_ALERT_WIN_ID,
    MMINPS_FDN_OPENED_ALERT_WIN_ID,
    MMINPS_CHECK_EMAIL_ALERT_WIN_ID,
    MMINPS_CHECK_PRIVACY_POLICY_WIN_ID,
    MMINPS_WAIT_WIN_ID,
    MMINPS_COMPLETE_WIN_ID,
    NPS_MAX_WIN_ID
}NPS_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    NPS_CTRL_ID_ID_START = NPS_MAX_WIN_ID,//(MMI_MODULE_BT << 16),
    NPS_MAIN_FORM_CTRL_ID,
    NPS_START_TEXT_THIRD_FORM_CTRL_ID,
    NPS_EXPLAIN_TEXT_CTRL_ID,
    NPS_SCORE_LABEL_CTRL_ID,
    NPS_SCORE_THIRD_FORM_CTRL_ID,
    NPS_SCORE_PRGBOX_CTRL_ID,
    NPS_SCORE_RESON_THIRD_FORM_CTRL_ID,
    NPS_REASON_TEXT_CTRL_ID,
    NPS_SCORE_EDIT_DIGITAL_CTRL_ID,
    NPS_REASON_EDIT_TEXT_CTRL_ID,
    NPS_MARK_AGREE_CONTACTED_CTRL_ID,
    NPS_FOLLOWUP_CHECK_LIST_CTRL_ID,
    NPS_EMAIL_THIRD_FORM_CTRL_ID,
    NPS_EMAIL_LABEL_CTRL_ID,
    NPS_EMAIL_EDIT_TEXT_CTRL_ID,
    NPS_PRIVACY_THIRD_FORM_CTRL_ID,
    NPS_REMIND_TEXT_CTRL_ID,
    MMINPS_FEEDBACK_SEND_CTRL_ID,
    MMINPS_CHECK_PRIVACY_POLICY_CTRL_ID,
    NPS_COMPLETE_TEXT_ID,
    NPS_MAX_CTRL_ID
}NPS_CONTROL_ID_E;

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
