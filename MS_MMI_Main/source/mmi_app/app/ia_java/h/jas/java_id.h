/*****************************************************************************
** File Name:      java_id.h                                               *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting win and control id   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18     kelly.li        Create                                    *
******************************************************************************/
#ifndef _JAVA_ID_H_
#define _JAVA_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                     *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "jblend_config.h"

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
typedef enum
{
    jAVA_WIN_ID_START = (MMI_MODULE_JAVA << 16),

#include "mmijava_id.def"

    JAVA_MAX_WIN_ID
}JAVA_WINDOW_ID_E;        
   
#undef WIN_ID_DEF
     
typedef enum
{
	JAVA_CTRL_ID_START = JAVA_MAX_WIN_ID,
	MMIJAVA_AMS_WIN_LISTBOX_CTRL_ID,
	MMIJAVA_AMS_OPTION_WIN_MENU_CTRL_ID,
	MMIJAVA_AMS_OPTION_INFO_CTRL_ID,
	MMIJAVA_AMS_OPTION_PERMIT_SET_WIN_CTRL_ID,
	#ifdef MMI_PDA_SUPPORT
	MMIJAVA_EIME_WIN_FORM_CTRL_ID, 
	MMIJAVA_EIME_WIN_SOFTKEY_CTRL_ID,
    #endif
	MMIJAVA_EIME_WIN_EDITBOX_CTRL_ID, 
	MMIJAVA_AMS_MIDLETLIST_WIN_LISTBOX_CTRL_ID, 
	MMIJAVA_DETAIL_NAME_TITLE_CTRL_ID,
	MMIJAVA_DETAIL_NAME_CTRL_ID,
	MMIJAVA_DETAIL_VERSION_TITLE_CTRL_ID,
	MMIJAVA_DETAIL_VERSION_CTRL_ID,
	MMIJAVA_DETAIL_VENDOR_TITLE_CTRL_ID,
	MMIJAVA_DETAIL_VENDOR_CTRL_ID,
	MMIJAVA_DETAIL_SIZE_TITLE_CTRL_ID,
	MMIJAVA_DETAIL_SIZE_CTRL_ID,
	MMIJAVA_DETAIL_DOMAIN_TITLE_CTRL_ID,
	MMIJAVA_DETAIL_DOMAIN_CTRL_ID,
	MMIJAVA_JAD_INFO_CTRL_ID,
	MMIJAVA_PERMIT_OPT_LIST_CTRL_ID,
	MMIJAVA_APP_INSTALL_SELECT_WIN_CTRL_ID,
	MMIJAVA_INSTALL_INFO_WIN_CTRL_ID,
    MMIJAVA_DETAIL_CTRL_ID,
	MMIJAVA_PAUSE_DIALG_LISTBOX_CTRL_ID,
	MMIJAVA_RESUME_DIALG_LISTBOX_CTRL_ID,
    MMIJAVA_SELECT_NET_FORM_ID,
    MMIJAVA_SELECT_NET_FORM1_ID,
    MMIJAVA_SELECT_NET_FORM2_ID,
    MMIJAVA_SELECT_NET_FORM3_ID,
    MMIJAVA_SELECT_NET_FORM4_ID,
    MMIJAVA_SELECT_NET_FORM5_ID,
    MMIJAVA_SELECT_NET_LABEL_NET_ID,
    MMIJAVA_SELECT_NET_DROPDOWNLIST_NET_ID,
    MMIJAVA_SELECT_NET_LABEL_SIM1_ID,
    MMIJAVA_SELECT_NET_LABEL2_SIM1_ID,
    //MMIJAVA_SELECT_NET_DROPDOWNLIST_SIM1_ID,
    MMIJAVA_SELECT_NET_LABEL_SIM2_ID,
    MMIJAVA_SELECT_NET_LABEL2_SIM2_ID,
    //MMIJAVA_SELECT_NET_DROPDOWNLIST_SIM2_ID,
    MMIJAVA_SELECT_NET_LABEL_SIM3_ID,
    MMIJAVA_SELECT_NET_LABEL2_SIM3_ID,
    //MMIJAVA_SELECT_NET_DROPDOWNLIST_SIM3_ID,
    MMIJAVA_SELECT_NET_LABEL_SIM4_ID,
    MMIJAVA_SELECT_NET_LABEL2_SIM4_ID,
    //MMIJAVA_SELECT_NET_DROPDOWNLIST_SIM4_ID,
    MMIJAVA_DOWNLOAD_SET_FORM_ID,
    MMIJAVA_DOWNLOAD_SET_GAME_LABEL_ID,
    MMIJAVA_DOWNLOAD_SET_GAME_URL_INPUT_ID,
    MMIJAVA_DOWNLOAD_SET_GAME_FORM_ID,
        MMIJAVA_DOWNLOAD_SET_GAME_ENTER_BOTTON_ID,
        MMIJAVA_DOWNLOAD_SET_GAME_ENTER_SPACE_ID,  
    MMIJAVA_DOWNLOAD_SET_EBOOK_LABEL_ID,
    MMIJAVA_DOWNLOAD_SET_EBOOK_URL_INPUT_ID,
    MMIJAVA_DOWNLOAD_SET_EBOOK_FORM_ID,
        MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_BOTTON_ID,
        MMIJAVA_DOWNLOAD_SET_EBOOK_ENTER_SPACE_ID, 
        
    MMIJAVA_DOWNLOAD_SET_OTHER_LABEL_ID,
    MMIJAVA_DOWNLOAD_SET_OTHER_URL_INPUT_ID,
    MMIJAVA_DOWNLOAD_SET_OTHER_FORM_ID,
        MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_BOTTON_ID,
        MMIJAVA_DOWNLOAD_SET_OTHER_ENTER_SPACE_ID, 
    MMIJAVA_DOWNLOAD_SET_GAME_FORM1_ID,
    MMIJAVA_DOWNLOAD_SET_EBOOK_FORM1_ID,
    MMIJAVA_DOWNLOAD_SET_OTHER_FORM1_ID,
    MMIJAVA_LONGPRESS_OPT_CTRL_ID,
    #ifdef  VIRTUAL_KEY_BOARD
    MMIJAVA_KEYBOARD1_FORM,
    MMIJAVA_KEYBOARD2_FORM,
        MMIJAVA_KEYBOARD1_LINE_ONE_FORM,
        MMIJAVA_KEYBOARD1_LINE_TWO_FORM,
        MMIJAVA_KEYBOARD2_LINE_ONE_FORM,
        MMIJAVA_KEYBOARD2_LINE_TWO_FORM,
        
            MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY,
            MMIJAVA_KEYBOARD1_BOTTON_UP_KEY,
            MMIJAVA_KEYBOARD1_BOTTON_RIGHT_SOFTKEY,
            MMIJAVA_KEYBOARD1_BOTTON_NUM_KEY,
            MMIJAVA_KEYBOARD1_BOTTON_LEFT_KEY,
            MMIJAVA_KEYBOARD1_BOTTON_DOWN_KEY,
            MMIJAVA_KEYBOARD1_BOTTON_RIGHT_KEY,
            MMIJAVA_KEYBOARD1_BOTTON_OK_KEY,
            
            MMIJAVA_KEYBOARD2_BOTTON_LFFT_SOFTKEY,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_1,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_2,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_3,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_4,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_5,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_6,
            MMIJAVA_KEYBOARD2_BOTTON_RIGHT_SOFTKEY,
            MMIJAVA_KEYBOARD2_BOTTON_RETURN_KEY,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_7,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_8,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_9,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_0,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_ASTERISK,
            MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_POUND,
            MMIJAVA_KEYBOARD2_BOTTON_OK_KEY,
    #endif
        
	JAVA_MAX_CTRL_ID
}JAVA_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_RegWinIdNameArr(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

