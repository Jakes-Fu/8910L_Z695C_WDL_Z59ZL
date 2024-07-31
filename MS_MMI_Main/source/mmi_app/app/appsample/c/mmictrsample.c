/*****************************************************************************
 ** File Name:   mmictrsample.c
 ** Author:      baokun
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe gui control example
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      baokun.yin       written
 ****************************************************************************/


#include "mmiappsample_internal.h"
#include "mmiappsample_text.h"
#include "mmiappsample_id.h"
#include "mmiappsample_image.h"

#include "srvsample_export.h"
#include "mmi_position.h"
#include "mmi_pubmenuwin.h"
#include "window_parse.h"
#include "mmk_msg.h"

#include "guilistbox.h"
#include "ctrlform_export.h"
#include "ctrllabel_export.h"
/******************************************************************************
//  Description : static form windown message
//   Global resource dependence :
//  Author:  baokun.yin
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleStaticFormUserWinMsg(MMI_WIN_ID_T win_id,
                                                     MMI_MESSAGE_ID_E msg_id,
                                                     DPARAM param
                                                    );



WINDOW_TABLE(MMIAPPSAMPLE_STATIC_FORM_USER_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleAppSampleStaticFormUserWinMsg),
    WIN_ID( MMIAPPSAMPLE_STATIC_FORM_USER_WIN_ID),
    WIN_TITLE(TXT_APPSAMPLE_FORMUSER),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_FUNCWINLIST_LIST_CTRL_ID),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMIAPPSAMPLE_ROOT_FORM_CTRL_ID),//root form
	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIAPPSAMPLE_FORM_SUB1_CTRL_ID,MMIAPPSAMPLE_ROOT_FORM_CTRL_ID),//sub1
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIAPPSAMPLE_FORM_SUB11_CTRL_ID,MMIAPPSAMPLE_FORM_SUB1_CTRL_ID),//sub1-label
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIAPPSAMPLE_FORM_SUB2_CTRL_ID,MMIAPPSAMPLE_ROOT_FORM_CTRL_ID),//sub1
	CHILD_BUTTON_CTRL(TRUE,IMAGE_DROPDOWNLIST_ARROW_PR, MMIAPPSAMPLE_FORM_SUB21_CTRL_ID, MMIAPPSAMPLE_FORM_SUB2_CTRL_ID),//sub1-button1
	CHILD_BUTTON_CTRL(TRUE,IMAGE_DROPDOWNLIST_ARROW_PR, MMIAPPSAMPLE_FORM_SUB22_CTRL_ID, MMIAPPSAMPLE_FORM_SUB2_CTRL_ID),//sub1-button1
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};


/******************************************************************************
//  Description : static form windown message
//   Global resource dependence :
//  Author:  baokun.yin
//  Note:
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleStaticFormUserWinMsg(MMI_WIN_ID_T win_id,
                                                     MMI_MESSAGE_ID_E msg_id,
                                                     DPARAM param
                                                    )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T   ctrl_id = 0;
	MMI_HANDLE_T    rootform_handle = 0;
	MMI_HANDLE_T    sub1_handle = 0;
	MMI_HANDLE_T    sub2_handle = 0;
	MMI_HANDLE_T    sub21_handle = 0;
	MMI_HANDLE_T    sub22_handle = 0;
	MMI_HANDLE_T    label_handle = 0;
	GUI_RECT_T      rect = MMITHEME_GetClientRect();
	GUIFORM_CHILD_WIDTH_T width_form = {80, GUIFORM_CHILD_WIDTH_FIXED};
	
	//{ 160, MMI_TITLE_THEME_HEIGHT, 239, MMI_TITLE_THEME_HEIGHT+MMI_TITLE_THEME_HEIGHT};
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
		rootform_handle = MMK_ConvertIdToHandle(MMIAPPSAMPLE_ROOT_FORM_CTRL_ID);
		sub1_handle = MMK_ConvertIdToHandle(MMIAPPSAMPLE_FORM_SUB1_CTRL_ID);
		label_handle = MMK_ConvertIdToHandle(MMIAPPSAMPLE_FORM_SUB11_CTRL_ID);
		sub2_handle = MMK_ConvertIdToHandle(MMIAPPSAMPLE_FORM_SUB2_CTRL_ID);
		sub21_handle = MMK_ConvertIdToHandle(MMIAPPSAMPLE_FORM_SUB21_CTRL_ID);
		sub22_handle = MMK_ConvertIdToHandle(MMIAPPSAMPLE_FORM_SUB22_CTRL_ID);
        CTRLLABEL_SetTextById(MMIAPPSAMPLE_FORM_SUB11_CTRL_ID, TXT_APPSAMPLE_FORM_LEFT_LABLE, FALSE);	
		CTRLFORM_SetAlign(MMIAPPSAMPLE_FORM_SUB2_CTRL_ID, GUIFORM_CHILD_ALIGN_RIGHT);
		rect.bottom = rect.top + MMI_TITLE_THEME_HEIGHT;
		rect.left = 160;
		CTRLFORM_SetChildWidth(MMIAPPSAMPLE_ROOT_FORM_CTRL_ID, MMIAPPSAMPLE_FORM_SUB2_CTRL_ID, &width_form);
		//CTRLFORM_SetRect(MMIAPPSAMPLE_FORM_SUB2_CTRL_ID, &rect);
		break;
		

    case MSG_KEYDOWN_WEB:
    case MSG_CTL_MIDSK:
        //EditWinMIDSKProcess(win_id);
        ctrl_id = MMK_GetActiveCtrlId(win_id);
        if(MMIAPPSAMPLE_FORM_SUB21_CTRL_ID == ctrl_id)
		{
			//button 1
            CTRLLABEL_SetTextById(MMIAPPSAMPLE_FORM_SUB11_CTRL_ID, TXT_APPSAMPLE_FORM_BUTTON1_PRESSED, TRUE);
		}
		else if(MMIAPPSAMPLE_FORM_SUB22_CTRL_ID == ctrl_id)
		{
			//button 2
			CTRLLABEL_SetTextById(MMIAPPSAMPLE_FORM_SUB11_CTRL_ID, TXT_APPSAMPLE_FORM_BUTTON2_PRESSED, TRUE);
		}
		break;
	case MSG_CTL_PENOK:

        break;
	case MSG_APP_OK:
	case MSG_CTL_PENLONGOK:

        break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin( win_id );
		break;
		
	case MSG_CLOSE_WINDOW:
		break;
		
	default:
		recode = MMI_RESULT_FALSE;
		break;
    }
	
    return recode;
}



/********************************************************************
//  Description :open static form sample window
//  Global resource dependence :
//  Author: baokun
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_OpenStaticForm(void)
{
   MMK_CreateWin((uint32*)MMIAPPSAMPLE_STATIC_FORM_USER_WIN_TAB, PNULL);
}

/********************************************************************
//  Description :open dynamic form sample window
//  Global resource dependence :
//  Author: baokun
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_OpenDynamicForm(void)
{

}