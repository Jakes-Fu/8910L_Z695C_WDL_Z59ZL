/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "os_api.h"
#include "guiedit.h"
#include "mmk_app.h"
#include "mmijava_internal.h"
#include "ajms_cfg_eime.h"
//#include "mmiim_nv.h"
#include "jblend_platform.h "
#include "window_parse.h"
#include "guiform.h"


#ifdef FS_PROFILE	
extern int  g_vm_exit ;
#endif

#define  MMIJAVA_TEXT_INPUT_MAX_LEN     1024
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN                   s_b_enableSuspendJAVA = TRUE;
LOCAL JAVA_EDITBOX_INIT_DATA_T*  s_edit_init_data_by_java_ptr = PNULL;
LOCAL AJ_S32                    s_constraints = AJMS_EIME_CONSTRAINTS_ANY;
LOCAL AJ_BOOL                   s_is_password_mode = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : ime window message handler
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJavaEimeWinMsg(
                                        MMI_WIN_ID_T         win_id,    
                                        MMI_MESSAGE_ID_E     msg_id, 
                                        DPARAM           param
                                        );

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE( MMIJAVA_EIME_WIN_TAB1 ) = 
{
	WIN_FUNC( (uint32)HandleJavaEimeWinMsg), 
    WIN_TITLE(TXT_NULL),
	WIN_ID( MMIJAVA_EIME_WIN_ID ),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
	END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :the callback function for starting the native input method
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIJAVA_jwe_eimeStartupImpl(AJMS_EimeData *data)
{      
    // MmiSignalS       *sig_ptr        = PNULL;
    uint16    input_mode_set = 0;
   if (MMK_IsOpenWin(MMIJAVA_EIME_WIN_ID))
   {
       return;
   }
   
   MMIJAVA_SetJavaIsEnablePause(FALSE);
   if(s_edit_init_data_by_java_ptr != PNULL)
   {
       return;
   }
   

   s_edit_init_data_by_java_ptr = SCI_ALLOCA(sizeof(JAVA_EDITBOX_INIT_DATA_T)); 
   if(PNULL == s_edit_init_data_by_java_ptr)
   {
       return;
   }
   
   SCI_MEMSET(s_edit_init_data_by_java_ptr,0,sizeof(JAVA_EDITBOX_INIT_DATA_T));
   
   if (PNULL == data->initText || 0 >= data->initTextLen)
   {
       s_edit_init_data_by_java_ptr->wstr_len = 0;  
   }
   else
   {
       s_edit_init_data_by_java_ptr->wstr_len = (uint16)data->initTextLen;
       MMIAPICOM_Wstrncpy(s_edit_init_data_by_java_ptr->wstr,data->initText,
            MIN(s_edit_init_data_by_java_ptr->wstr_len,MMIJAVA_TEXT_INPUT_MAX_LEN));    
   }

   if (PNULL == data->titleText || 0 >= data->titleTextLen)
   {
       s_edit_init_data_by_java_ptr->title_len = 0;
   }
   else
   {
       s_edit_init_data_by_java_ptr->title_len = data->titleTextLen;
       MMIAPICOM_Wstrncpy(s_edit_init_data_by_java_ptr->title,data->titleText,
            MIN(s_edit_init_data_by_java_ptr->title_len,MMIJAVA_TEXT_INPUT_MAX_LEN));
   }   
    
    s_edit_init_data_by_java_ptr->max_default_len = MIN(MMIJAVA_TEXT_INPUT_MAX_LEN, data->maxSize);
    s_edit_init_data_by_java_ptr->cur_inputmethod = input_mode_set;
    JWE_LOG(MMIJAVA_jwe_eimeStartupImpl, ("create eime window"));
    s_is_password_mode = data->isPasswordMode;
    s_constraints = data->constraints;
    MMIAPIJAVA_SendSignal(MSG_OPEN_TEXTEDIT_WIN, PNULL);
    
}

/*****************************************************************************/
// 	Description : open text edit win
//	Global resource dependence : none
//  Author: applezhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenTextEditWindow(AJMS_EimeData *data)
{    
    //SCI_TRACE_LOW:"MMIAPIJAVA_OpenTextEditWindow cur_inputmethod = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JWE_EIME_128_112_2_18_2_23_5_350,(uint8*)"d",s_edit_init_data_by_java_ptr->cur_inputmethod);
	    MMK_CreateWin((uint32*)MMIJAVA_EIME_WIN_TAB1, PNULL);
}


/*****************************************************************************/
// 	Description : the callback function for finishing the native input method. 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_jwe_eimeTerminateImpl(void)
{
    MMIAPIJAVA_SendSignal(MSG_JAVA_CLOSE_APP_WIN,MMIJAVA_EIME_WIN_ID); 
    JWE_LOG(MMIJAVA_jwe_eimeTerminateImpl, ("jwe_eimeTerminateImpl -- Enter!"));    
}


/*****************************************************************************/
// 	Description : ime window message handler
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleJavaEimeWinMsg(
                                        MMI_WIN_ID_T         win_id,    
                                        MMI_MESSAGE_ID_E     msg_id, 
                                        DPARAM           param
                                        )
{
    MMI_RESULT_E                result   = MMI_RESULT_TRUE;
    MMI_STRING_T                str_info = {0};   
    //GUI_BG_T    				edit_bg  = {0};
    // AJMS_EimeData*              inputdata = (AJMS_EimeData *)MMK_GetWinAddDataPtr(win_id);
    GUIIM_TYPE_T                allow_im = GUIIM_TYPE_NONE;
    GUIIM_TYPE_T                init_im = GUIIM_TYPE_NONE;
    GUIIM_TAG_T                 tag = GUIIM_TAG_NONE;
    JWE_LOG(HandleJavaEimeWinMsg, ("msg_id = 0x%08x", msg_id));    
   
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_CONTROL_CREATE_T        create = {0};
            GUIEDIT_INIT_DATA_T	        edit_init_data = {0};
			
#ifdef MMI_PDA_SUPPORT
            GUIFORM_DYNA_CHILD_T        form_child_ctrl= {0};//child form
            GUISOFTKEY_INIT_DATA_T      softkey_init = {0};
#endif
			CAF_BOTH_RECT_T 			both_edit_rect = MMITHEME_GetWinClientBothRect(win_id);
            MMIJAVA_SetJavaIsEnablePause(TRUE);

            if (0 != s_edit_init_data_by_java_ptr->title_len)
            {
                GUIWIN_SetTitleText( win_id, s_edit_init_data_by_java_ptr->title, s_edit_init_data_by_java_ptr->title_len, FALSE);
            }    
            
            //set constraint mode
            switch(s_constraints) 
            {
            case AJMS_EIME_CONSTRAINTS_NUMERIC:
                allow_im = GUIIM_TYPE_DIGITAL;
                init_im = GUIIM_TYPE_DIGITAL;
                tag = GUIIM_TAG_PURE_NUM;
                break;
                
            case AJMS_EIME_CONSTRAINTS_PHONENUMBER:
                allow_im = GUIIM_TYPE_DIGITAL;
                init_im = GUIIM_TYPE_DIGITAL;
                tag = GUIIM_TAG_PHONE_NUM;
                break;
                
            case AJMS_EIME_CONSTRAINTS_EMAILADDR:
                allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;
                init_im = GUIIM_TYPE_ABC;
                tag = GUIIM_TAG_EMAIL;
                break;                
                
            case AJMS_EIME_CONSTRAINTS_URL:
                allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;
                init_im = GUIIM_TYPE_ABC;
                tag = GUIIM_TAG_URL;
                break;
                
            case AJMS_EIME_CONSTRAINTS_DECIMAL:
                allow_im = GUIIM_TYPE_DIGITAL;
                init_im = GUIIM_TYPE_DIGITAL;
                tag = GUIIM_TAG_PURE_NUM;
                break;
                
            case AJMS_EIME_CONSTRAINTS_ANY:
                if(s_is_password_mode)//other implement way in future:GUIEDIT_SetStyle
                {
                    allow_im =  GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;
                    init_im = GUIIM_TYPE_ABC;
                    // s_is_password_mode = FALSE;
                }
                else
                {
                    allow_im = GUIIM_TYPE_SET_ALL;
                    init_im = GUIIM_TYPE_ABC;
                }
                break;
                
            default:
                allow_im = GUIIM_TYPE_SET_ALL;
                init_im = GUIIM_TYPE_ABC;
                break;
            }
            
            edit_init_data.str_max_len =s_edit_init_data_by_java_ptr->max_default_len ;
            edit_init_data.type = s_is_password_mode ? GUIEDIT_TYPE_PASSWORD : GUIEDIT_TYPE_TEXT ;

            #ifndef MMI_PDA_SUPPORT
			edit_init_data.both_rect = both_edit_rect;
            create.ctrl_id = MMIJAVA_EIME_WIN_EDITBOX_CTRL_ID; //Control id
            create.guid = SPRD_GUI_EDITBOX_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &edit_init_data;  
            
            MMK_CreateControl(&create); //Create Control dynamicly   
            #else
            edit_init_data.is_rect_adaptive = TRUE;

            GUIFORM_CreatDynaCtrl(win_id,MMIJAVA_EIME_WIN_FORM_CTRL_ID,GUIFORM_LAYOUT_ORDER);

            form_child_ctrl.child_handle      = MMIJAVA_EIME_WIN_EDITBOX_CTRL_ID;
            form_child_ctrl.guid              = SPRD_GUI_EDITBOX_ID;
            form_child_ctrl.init_data_ptr     = &edit_init_data;
            form_child_ctrl.is_bg = FALSE;
            form_child_ctrl.is_get_active = TRUE;//an active ctrl!!!
            GUIFORM_CreatDynaChildCtrl(win_id, MMIJAVA_EIME_WIN_FORM_CTRL_ID,  &form_child_ctrl);

            form_child_ctrl.is_softkey = TRUE;
            form_child_ctrl.guid = SPRD_GUI_SOFTKEY_ID;
            form_child_ctrl.child_handle=MMITHEME_GetSoftkeyCtrlId();
            softkey_init.leftsoft_id  = TXT_COMMON_OK;
            softkey_init.midsoft_id   = TXT_NULL;
            softkey_init.rightsoft_id = STXT_CANCEL;
            form_child_ctrl.init_data_ptr=&softkey_init;
                
            GUIFORM_CreatDynaChildCtrl(win_id,MMIJAVA_EIME_WIN_FORM_CTRL_ID,&form_child_ctrl);

            #endif
            if (GUIEDIT_TYPE_PASSWORD == edit_init_data.type)//密码输入界面增加输入法切换显示
            {
                BOOLEAN is_disp_im_icon = TRUE;
                BOOLEAN is_disp_num_info = TRUE;
                GUIEDIT_SetStyle(create.ctrl_id, GUIEDIT_STYLE_MULTI);//多行
                GUIEDIT_SetAlign(create.ctrl_id, ALIGN_LEFT);//靠左上起			
                GUIEDIT_SetDispImIcon(create.ctrl_id,&is_disp_im_icon,&is_disp_num_info);
            }
            
            GUIEDIT_SetImTag(MMIJAVA_EIME_WIN_EDITBOX_CTRL_ID, tag);
            GUIEDIT_SetIm(MMIJAVA_EIME_WIN_EDITBOX_CTRL_ID, allow_im, init_im);
            if (0 != s_edit_init_data_by_java_ptr->wstr_len)
            {
                GUIEDIT_SetString(MMIJAVA_EIME_WIN_EDITBOX_CTRL_ID,s_edit_init_data_by_java_ptr->wstr, s_edit_init_data_by_java_ptr->wstr_len);
            }
            
            MMK_SetAtvCtrl(win_id, MMIJAVA_EIME_WIN_EDITBOX_CTRL_ID);
        }
		break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
        
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                 MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527		
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
		case MSG_CTL_MIDSK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
		case MSG_CTL_OK:      
       
			JWE_LOG(HandleJavaEimeWinMsg, ("HandleJavaEimeWinMsg -- MSG_CTL_OK!"));
			GUIEDIT_GetString(MMIJAVA_EIME_WIN_EDITBOX_CTRL_ID, &str_info);    

			ajms_cfg_finalizeEime(str_info.wstr_ptr, str_info.wstr_len, AJMS_EIME_USER_CONFIRM);            
			JWE_LOG(HandleJavaEimeWinMsg, ("close eime window"));
			MMK_CloseWin( win_id ); 
        
		break;

		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
		case MSG_APP_RED:
	        JWE_LOG(HandleJavaEimeWinMsg, ("HandleJavaEimeWinMsg -- MSG_CTL_CANCEL!"));        
	        ajms_cfg_finalizeEime(s_edit_init_data_by_java_ptr->wstr,s_edit_init_data_by_java_ptr->wstr_len, AJMS_EIME_USER_CANCEL );  
	        JWE_LOG(HandleJavaEimeWinMsg, ("close eime window"));
	        MMK_CloseWin( win_id );      
		break;

		case MSG_CLOSE_WINDOW:
	        JWE_LOG(HandleJavaEimeWinMsg, ("HandleJavaEimeWinMsg -- MSG_CLOSE_WINDOW!")); 
#ifdef FS_PROFILE	
			g_vm_exit = 0;
			kjava_profiler_start();
			if(g_vm_exit <2)
			{
				kjava_profiler_startCount(0);
			}	
#endif
            if(PNULL != s_edit_init_data_by_java_ptr)
            {
                SCI_FREE(s_edit_init_data_by_java_ptr);
            }
	    break;

	    default:
        	JWE_LOG(HandleJavaEimeWinMsg, ("HandleJavaEimeWinMsg -- do nothing!"));
		break;
    }
    
    return (result);    
}

#endif


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
