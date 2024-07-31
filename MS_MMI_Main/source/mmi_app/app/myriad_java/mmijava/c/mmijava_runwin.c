
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include <window_parse.h>
#include <guiform.h>
#include <mmicc_export.h>
#include <jbed_exported_defines.h>
#include <mmijava_id.h>
#include <mmialarm_export.h>
//#include <mmijava_nv.h>
#include <java_mmi_text.h>
#include <java_mmi_image.h>
//#include <mmijava_menutable.h>
#include <mmijava_common.h>
//#include <opl_global.h>
#include <jbed_main.h>
//#include <mmi_appmsg.h>
//#include <javastatic.h>
//#include "mmiidle_statusbar.h"
#ifdef MMI_PDA_SUPPORT
//#include <guistatusbar.h>
//#include <guires.h>
//#include "guibutton.h"
#endif
#include "mmipub.h"
#include <mmi_appmsg.h>
#include "guires.h"
#include "mmiidle_statusbar.h"
#ifdef SUPPORT_BACKGROUND_RUN
#include "mmijava_backgroundwin.h"
#endif
#include "mmijava_runwin.h"
#include "mmijava_imwin.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                   *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN   s_b_enable_pasue = TRUE;


#ifdef SUPPORT_BACKGROUND_RUN
extern bool_t gotoBackground;
#endif

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
extern  bool_t bCapsLockPressed ;
extern  bool_t bShiftPressed    ;
extern  bool_t bControlPressed  ;
#endif

/* As required, define the midlet is not able to run on background in this project. */
#define MIDLET_NOT_RUNNING_BACKGROUND
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Jbed_HandleRunningWinMsg
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleRunningWinMsg(
                                                 MMI_WIN_ID_T winId,
                                                 MMI_MESSAGE_ID_E msgId,
                                                 DPARAM param
                                             );

#ifdef VIRTUAL_KEY_BOARD
/*****************************************************************************/
//  Description : the callback of num botton in keyboard1
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL int MapVirtualKeyboardToKeyCode(
                                         uint32 ctr_id
                                     );

/*****************************************************************************/
//  Description : the callback of num botton in keyboard1
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NumberButtonCallBack(
                                           void
                                       );

/*****************************************************************************/
//  Description : the callback of return botton in keyboard2
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ReturnButtonCallBack(
                                           void
                                       );

/*****************************************************************************/
//  FUNCTION:       SetWCFormParam
//  Description :   set the from parameter
//  Global resource dependence :
//  Author:         Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL void SetJavaKeyboardFormparam(
                                        void
                                    );
#endif

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

WINDOW_TABLE( JBED_RUNNING_APP_WIN_TAB ) =
{
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC( (uint32)Jbed_HandleRunningWinMsg),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),

#ifdef  VIRTUAL_KEY_BOARD
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_KEYBOARD1_FORM),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_KEYBOARD1_LINE_ONE_FORM,MMIJAVA_KEYBOARD1_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_LS_OFF,MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY,MMIJAVA_KEYBOARD1_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_UP_OFF,MMIJAVA_KEYBOARD1_BOTTON_UP_KEY,MMIJAVA_KEYBOARD1_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_RS_OFF,MMIJAVA_KEYBOARD1_BOTTON_RIGHT_SOFTKEY,MMIJAVA_KEYBOARD1_LINE_ONE_FORM),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_KEYBOARD1_LINE_TWO_FORM,MMIJAVA_KEYBOARD1_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_NUM_OFF,MMIJAVA_KEYBOARD1_BOTTON_NUM_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_LEFT_OFF,MMIJAVA_KEYBOARD1_BOTTON_LEFT_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_DOWN_OFF,MMIJAVA_KEYBOARD1_BOTTON_DOWN_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_RIGHT_OFF,MMIJAVA_KEYBOARD1_BOTTON_RIGHT_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_OK_OFF,MMIJAVA_KEYBOARD1_BOTTON_OK_KEY,MMIJAVA_KEYBOARD1_LINE_TWO_FORM),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIJAVA_KEYBOARD2_FORM),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_KEYBOARD2_LINE_ONE_FORM,MMIJAVA_KEYBOARD2_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_LS_OFF,MMIJAVA_KEYBOARD2_BOTTON_LFFT_SOFTKEY,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM1_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_1,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM2_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_2,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM3_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_3,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM4_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_4,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM5_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_5,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM6_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_6,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_RS_OFF,MMIJAVA_KEYBOARD2_BOTTON_RIGHT_SOFTKEY,MMIJAVA_KEYBOARD2_LINE_ONE_FORM),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIJAVA_KEYBOARD2_LINE_TWO_FORM,MMIJAVA_KEYBOARD2_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_RETURN_OFF,MMIJAVA_KEYBOARD2_BOTTON_RETURN_KEY,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM7_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_7,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM8_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_8,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM9_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_9,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_NUM0_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_0,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_ASTERISK_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_ASTERISK,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD2_POUND_OFF,MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_POUND,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),
            CHILD_BUTTON_CTRL(TRUE,IMAGE_JAVA_KEYBOARD1_OK_OFF,MMIJAVA_KEYBOARD2_BOTTON_OK_KEY,MMIJAVA_KEYBOARD2_LINE_TWO_FORM),

#endif
    WIN_ID( JBED_RUNNING_APP_WIN_ID ),
    END_WIN
};

#ifdef VIRTUAL_KEY_BOARD
/*****************************************************************************/
//  Description : get virtual keyboard Bitmap
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
void Jbed_Get_VritualKeyBoardBitmap(GUIIMG_BITMAP_T *virtualkey_bitmap_ptr)
{
    static uint16  image_width = 0,image_height = 0;
    static BOOL is_init = FALSE;
    if(!is_init && MMK_IsOpenWin(JBED_RUNNING_APP_WIN_ID))
    {
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD_BG,JBED_RUNNING_APP_WIN_ID);
        is_init = TRUE;
    }
    virtualkey_bitmap_ptr->img_height = image_height;
    virtualkey_bitmap_ptr->img_width = image_width;
    return;
}
#endif

#ifdef VIRTUAL_KEY_BOARD
/*****************************************************************************/
//  Description : the callback of num botton in keyboard1
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL int MapVirtualKeyboardToKeyCode(
                                         uint32 ctr_id
                                     )
{
    switch(ctr_id)
    {
        case MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY:
        case MMIJAVA_KEYBOARD2_BOTTON_LFFT_SOFTKEY:
            return KEY_OK;

        case MMIJAVA_KEYBOARD1_BOTTON_RIGHT_SOFTKEY:
        case MMIJAVA_KEYBOARD2_BOTTON_RIGHT_SOFTKEY:
            return KEY_CANCEL;

        case MMIJAVA_KEYBOARD1_BOTTON_UP_KEY:
            return KEY_UP;

        case MMIJAVA_KEYBOARD1_BOTTON_LEFT_KEY:
            return KEY_LEFT;

        case MMIJAVA_KEYBOARD1_BOTTON_DOWN_KEY:
            return KEY_DOWN;

        case MMIJAVA_KEYBOARD1_BOTTON_RIGHT_KEY:
            return KEY_RIGHT;

        case MMIJAVA_KEYBOARD1_BOTTON_OK_KEY:
        case MMIJAVA_KEYBOARD2_BOTTON_OK_KEY:
            return KEY_WEB;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_1:
            return KEY_1;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_2:
            return KEY_2;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_3:
            return KEY_3;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_4:
            return KEY_4;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_5:
            return KEY_5;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_6:
            return KEY_6;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_7:
            return KEY_7;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_8:
            return KEY_8;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_9:
            return KEY_9;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_0:
            return KEY_0;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_ASTERISK:
            return KEY_STAR;

        case MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_POUND:
            return KEY_HASH;

         default:
            return KEY_NONE;
    }
}

/*****************************************************************************/
//  Description : the callback of num botton in keyboard1
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NumberButtonCallBack(
                                           void
                                       )
{
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD2_FORM,TRUE,TRUE);
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD1_FORM,FALSE,TRUE);
    MMK_SendMsg(JBED_RUNNING_APP_WIN_ID, MSG_FULL_PAINT, PNULL);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : the callback of return botton in keyboard2
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ReturnButtonCallBack(
                                           void
                                       )
{
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD2_FORM,FALSE,TRUE);
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD1_FORM,TRUE,TRUE);
    MMK_SendMsg(JBED_RUNNING_APP_WIN_ID, MSG_FULL_PAINT, PNULL);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  FUNCTION:       SetWCFormParam
//  Description :   set the from parameter
//  Global resource dependence :
//  Author:         Delia Yuan
//  Note:
/*****************************************************************************/
LOCAL void SetJavaKeyboardFormparam(
                                        void
                                    )
{
    GUI_BG_T                    bg_info = {0};
    uint16                      form_space = 1;
    uint16                      form_margin = 4;
    GUI_RECT_T                  rect= MMITHEME_GetFullScreenRect();
    int                         i = 0;
    uint16                      image_width = 0,image_height = 0;
    GUIFORM_CHILD_WIDTH_T       child_width = {0};

    //not slide

    GUIFORM_IsSlide(MMIJAVA_KEYBOARD1_FORM,FALSE);
    GUIFORM_IsSlide(MMIJAVA_KEYBOARD1_FORM,FALSE);

    //set rect
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD_BG,JBED_RUNNING_APP_WIN_ID);

    rect.top = rect.bottom + 1 - image_height;

    GUIFORM_SetRect(MMIJAVA_KEYBOARD1_FORM,&rect);
    GUIFORM_SetRect(MMIJAVA_KEYBOARD2_FORM,&rect);

    //set bg
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_JAVA_KEYBOARD_BG;
    GUIFORM_SetBg(MMIJAVA_KEYBOARD1_FORM, &bg_info);
    GUIFORM_SetBg(MMIJAVA_KEYBOARD2_FORM, &bg_info);

    //set keyboard1 rect info
    //left+right softkey icon
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD1_LS_ON,JBED_RUNNING_APP_WIN_ID);
    child_width.add_data = image_width;
    GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD1_LINE_ONE_FORM,MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY,&child_width);
    GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD1_LINE_ONE_FORM,MMIJAVA_KEYBOARD1_BOTTON_RIGHT_SOFTKEY,&child_width);


    //set keyboard2 rect info
    //left+right softkey icon
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD2_LS_OFF,JBED_RUNNING_APP_WIN_ID);
    child_width.add_data = image_width;
    GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD2_LINE_ONE_FORM,MMIJAVA_KEYBOARD2_BOTTON_RIGHT_SOFTKEY,&child_width);

    //line1  num
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD2_NUM0_OFF,JBED_RUNNING_APP_WIN_ID);
    child_width.add_data = image_width;
    for(i = MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_1; i <= MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_6; i++)
    {
        GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD2_LINE_ONE_FORM,i,&child_width);
    }


    //return icon
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD2_RETURN_OFF,JBED_RUNNING_APP_WIN_ID);
    child_width.add_data = image_width;
    GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD2_LINE_TWO_FORM,MMIJAVA_KEYBOARD2_BOTTON_RETURN_KEY,&child_width);


    //line2 num
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD2_NUM0_OFF,JBED_RUNNING_APP_WIN_ID);
    child_width.add_data = image_width;
    for(i = MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_7; i <= MMIJAVA_KEYBOARD2_BOTTON_NUM_KEY_POUND; i++)
    {
        GUIFORM_SetChildWidth(MMIJAVA_KEYBOARD2_LINE_TWO_FORM,i,&child_width);
    }

    //set from margin + space
    GUIFORM_SetSpace(MMIJAVA_KEYBOARD1_FORM,&form_space,&form_space);

    #if defined (MAINLCD_SIZE_240X400)|| defined(MAINLCD_SIZE_240X320)
    form_space = 3;
    #endif

    GUIFORM_SetSpace(MMIJAVA_KEYBOARD2_FORM,&form_space,&form_space);

    GUIFORM_SetMargin(MMIJAVA_KEYBOARD1_FORM,form_margin);
    GUIFORM_SetMargin(MMIJAVA_KEYBOARD2_FORM,form_margin);

    //set button
    for(i = MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY; i <= MMIJAVA_KEYBOARD2_BOTTON_OK_KEY; i++)
    {
        GUIBUTTON_PermitBorder(i, FALSE);
        GUIBUTTON_SetRunSheen(i,FALSE);
        if(i < MMIJAVA_KEYBOARD2_BOTTON_OK_KEY)
        {
            bg_info.img_id = IMAGE_JAVA_KEYBOARD1_LS_ON + (i - MMIJAVA_KEYBOARD1_BOTTON_LFFT_SOFTKEY) *2;
            GUIBUTTON_SetPressedFg(i, &bg_info);
        }
    }

    //ok botton
    bg_info.img_id = IMAGE_JAVA_KEYBOARD1_OK_ON;
    GUIBUTTON_SetPressedFg(MMIJAVA_KEYBOARD2_BOTTON_OK_KEY, &bg_info);


    //set callback
    GUIBUTTON_SetCallBackFunc(MMIJAVA_KEYBOARD1_BOTTON_NUM_KEY, NumberButtonCallBack);
    GUIBUTTON_SetCallBackFunc(MMIJAVA_KEYBOARD2_BOTTON_RETURN_KEY, ReturnButtonCallBack);

    // hide keyboard2
    GUIFORM_SetVisible(MMIJAVA_KEYBOARD2_FORM,FALSE,FALSE);
}
#endif

LOCAL MMI_RESULT_E Jbed_HandleQueryExitWinMsg(
                                       MMI_WIN_ID_T win_id,
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM param
                                   )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_KEYUP_RED:
          break;

        case MSG_KEYDOWN_RED:
        case MSG_APP_WEB:
        {
            MMI_STRING_T prompt = {0};

            SCI_TRACE_LOW("=== Myriad Jbed_HandleQueryExitWinMsg exit midlet. ==");

            MMI_GetLabelTextByLang(STR_JAVA_CLOSING, &prompt);
            MMIJAVA_util_openWaitingWin(&prompt);
            if (NULL != g_active_launching_app)
            {
                Jbed_nAms_destroy(g_active_launching_app->sid, g_active_launching_app->mid);
            }
            else
            {
                Jbed_requestState(JBED_MAIN_DOWN, NULL, NULL);
                Jbed_wakeupVM();
            }

            Jbed_createJbedDownTimer();
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description : Jbed_HandleRunningWinMsg
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
static MMI_RESULT_E Jbed_HandleRunningWinMsg(
                                                 MMI_WIN_ID_T winId,
                                                 MMI_MESSAGE_ID_E msgId,
                                                 DPARAM param
                                             )
{
    MMI_RESULT_E mmi_ret = MMI_RESULT_TRUE;
    switch (msgId)
    {
        case MSG_OPEN_WINDOW:
        {
            IGUICTRL_T     *ctrl_handle = PNULL;
            ctrl_handle = MMK_GetCtrlPtrByWin(winId, MMITHEME_GetStatusBarCtrlId());
            if(0 != ctrl_handle)
            {
                IGUICTRL_SetState(ctrl_handle, GUICTRL_STATE_DISABLE_ACTIVE\
                    | GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, TRUE);
            }
            GUIWIN_SetStbState(winId, GUISTATUSBAR_STATE_INVALID, TRUE);

        #ifdef VIRTUAL_KEY_BOARD
            SetJavaKeyboardFormparam();
        #endif
            //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        #ifdef KEYPAD_TYPE_QWERTY_KEYPAD
            bCapsLockPressed = FALSE;
            bShiftPressed    = FALSE;
            bControlPressed  = FALSE;
        #endif
            MMIJAVA_updateRunningWinState(STATE_GET_FOCUS, TRUE);
            MMIJAVA_SetJavaIsEnablePause(TRUE);
        }
        break;

        case MSG_TIMER:
        break;

        case MSG_BACKLIGHT_TURN_ON:
            MMIJAVA_SetJavaIsEnablePause(TRUE);
            Jbed_vmWantForeground(TRUE);
        break;

        case MSG_BACKLIGHT_TURN_OFF:
        #ifndef AMS_TCK_RUNNER
            if(s_b_enable_pasue)
            {
                Jbed_vmWantToIdle();
            }
        #endif
            break;


            case MSG_LOSE_FOCUS:
            if(!MMIJAVA_GetTextEditorForegroundStatus())
            {
                //MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                ClearKeyUp();
                MMIJAVA_updateRunningWinState(STATE_LOSE_FOCUS, TRUE);

                if (Jbed_tm_getRunningAppNum() > 0)
                {
                    MAIN_UpdateIdleJavaState(TRUE);
                }

                #ifdef CMCC_41_SUPPORT
                // We should request to JBED_MAIN_ACTIVE NOT JBED_MAIN_IDLE if cets MIDlet
                // wants itself still running on background, otherwise, it will make the midlet pause.
                if (JBED_ams_getCurrentSuiteTrusted() && JBED_ams_checkCurrentSuiteKeepBackground())  /*lint !e718 !e746*/
                {
                    SCI_TRACE_LOW("=== myriad java: Jbed_HandleRunningWinMsg reuest to JBED_MAIN_ACTIVE for cets ===");
                    Jbed_vmWantForeground(FALSE);
                }
                else
                #endif //CMCC_41_SUPPORT
                {
                    //Send jvm to background
                    if(s_b_enable_pasue)
                    {
                    #ifdef JAVA_SUPPORT_TYPE_MVM
                        //new code here if needed
                    #else
                        if(MMIAPICC_IsInState(CC_IN_CALL_STATE) || MMIAPIALM_IsAlarmNotifying())
                        {
                            Jbed_vmWantToIdle();
                        }
                    #endif
                    }
                }
            }
            break;

        case MSG_GET_FOCUS:
        if(MMIJAVA_GetTextEditorForegroundStatus())
        {
            MMIJAVA_SetTextEditorForegroundStatus(FALSE);
            MMIJAVA_updateRunningWinState(STATE_GET_FOCUS, TRUE);
        }
        else
        {
            /*这句为解决重新获取焦点时有些应用没有实时刷屏造成屏幕灰暗加的，
             *追踪是否有花屏的副作用,nix 2013-4-22*/
            MMIJAVA_updateRunningWinState(STATE_GET_FOCUS, TRUE);
            //Send jvm to forground
            MMIJAVA_SetJavaIsEnablePause(TRUE);
        #ifdef JAVA_SUPPORT_TYPE_MVM
            //new code here if needed
        #else
            Jbed_vmWantForeground(TRUE);  //not use Jbed_nAms_requestVmToState,because jbed may be in sleep
        #endif
            //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        #ifdef SUPPORT_CAMERA_CAPTURE
            Jbed_set_previewStatus(START_PREVIEW);
        #endif
        }
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:
        case MSG_TP_PRESS_UP:
        case MSG_TP_PRESS_MOVE:
#ifdef VIRTUAL_KEY_BOARD
            {
                GUI_POINT_T point = {0};
                JbedGfxInfo info = {0};
                BOOLEAN is_virtual_key = FALSE;
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);

                cpl_gfx_getInfo(&info);
                if(point.y <= info.bitmap.img_height)
                {
                    is_virtual_key = FALSE;
                }
                else
                {
                    is_virtual_key = TRUE;
                    param = (DPARAM)MapVirtualKeyboardToKeyCode(MMK_GetActiveCtrlId(JBED_RUNNING_APP_WIN_ID));
                }
                if(param != KEY_NONE)
                {
                    Jbed_HandleEvents(msgId, param,is_virtual_key);
                }
            }
#else
            Jbed_HandleEvents(msgId, param,FALSE);
#endif
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527


        case MSG_CLOSE_WINDOW:
            //MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            MMIJAVA_updateRunningWinState(STATE_LOSE_FOCUS, TRUE);
            Jbed_vmWantForeground(FALSE);

        #ifdef SUPPORT_BACKGROUND_RUN
            if (gotoBackground)
            {
                Jbed_nAms_gotoBackground();
                gotoBackground = FALSE;
            }
            if (Jbed_tm_getRunningAppNum() > 0)
            {
                SCI_TRACE_LOW("+++++Myriadlog+++++MAIN_UpdateIdleJavaState(TRUE) runing app number:%d", Jbed_tm_getRunningAppNum());
                MAIN_UpdateIdleJavaState(TRUE);
            }
        #else
            //use this judgement to find that launching is from javalist or grid menu shortcut
            if (!Jbed_is_javalist_open())
            {
                SCI_TRACE_LOW("===suite list win is closed,close msg is from grid menu,exit java==");
                MMIJAVA_NotifyJVMExit();
            }
        #endif
            break;

        case MSG_KEYUP_RED:
            break;

        case MSG_KEYLONG_POWER:
        case MSG_KEYDOWN_RED:    /*lint !e616*/
            SCI_TRACE_LOW("=== Myriad jbed win: MSG_KEYUP_RED==");

            /* The midlet is installed as background mode.
             * But for the sake of lacking memory and other issues, the Microsoft
             * required that the midlet was not running on background.
             */
#if !defined(SUPPORT_BACKGROUND_RUN) || defined(MIDLET_NOT_RUNNING_BACKGROUND)
            {
                MMI_STRING_T prompt_text = {0};
                MMI_STRING_T exit_text = {0};
                wchar *question_text = L"?";

                SCI_TRACE_LOW("=== Myriad running win: invoke exit query ==");

                MMI_GetLabelTextByLang(STR_JAVA_EXIT, &exit_text);
                prompt_text.wstr_len = exit_text.wstr_len + MMIAPICOM_Wstrlen(question_text) + 1;
                prompt_text.wstr_ptr = (wchar *)SCI_ALLOCA(prompt_text.wstr_len*sizeof(wchar));
                if (prompt_text.wstr_ptr == PNULL)
                {
                    break;
                }
                SCI_MEMSET(prompt_text.wstr_ptr, 0x00, (prompt_text.wstr_len*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(prompt_text.wstr_ptr, exit_text.wstr_ptr, exit_text.wstr_len);
                MMIAPICOM_Wstrncat(prompt_text.wstr_ptr, question_text, MMIAPICOM_Wstrlen(question_text));

                MMIPUB_OpenConfirmationDialogByTextPtr(PNULL, &prompt_text, PNULL,
                    IMAGE_NULL, PNULL, PNULL, MMIPUB_SOFTKEY_DIALOG_OK_CLOSE, Jbed_HandleQueryExitWinMsg);
                SCI_FREEIF(prompt_text.wstr_ptr);
                break;
            }
#else
            SCI_TRACE_LOW("=== Myriad running win: will open MMIJAVA_OpenRedkeyDialog ==");
#ifdef SUPPORT_CAMERA_CAPTURE
            // set message to guarantee that: close preview when press red key.
            Jbed_set_previewStatus(POPUP_REDKEYDIALOG);
#else
            MMIJAVA_OpenRedkeyDialog();
#endif
#endif
            break;

        case MSG_KEYDOWN_POWER:
        case MSG_KEYUP_POWER:
            mmi_ret = MMI_RESULT_TRUE;//can't lock screen when java is running because of lacking memory
            break;

        case MSG_KEYDOWN_UPSIDE:
        case MSG_KEYDOWN_DOWNSIDE:
        case MSG_KEYUP_UPSIDE:
        case MSG_KEYUP_DOWNSIDE:
        case MSG_KEYREPEAT_UPSIDE:
        case MSG_KEYREPEAT_DOWNSIDE:
        case MSG_KEYUP_CAMERA:
        case MSG_KEYDOWN_CAMERA:
        case MSG_KEYLONG_CAMREA:
            mmi_ret = MMI_RESULT_FALSE;
            break;

#ifdef VIDEO_PLAYER_SUPPORT
        case MSG_JAVA_TRIGGER_JBED_REFRESH:
        {/* Should be triggered by video player*/
            extern void update_video_frame();
            SCI_TRACE_LOW("MSG_JAVA_TRIGGER_JBED_REFRESH - INFO: swith start... ");
            update_video_frame();
        }
        break;
#endif
        case MSG_FULL_PAINT:
             /*unused here because JBED task and MMI task may called this at the same
              time , this may cause screen refresh issue , refreshPaintWindowFull()
              called only when running window open */
            if(MMK_IsFocusWin(winId))
            {
                if(STATE_GET_FOCUS == MMIJAVA_updateRunningWinState(STATE_DEFAULT, FALSE))
                {
                    refreshPaintWindowFull();
                }
                MMIJAVA_updateRunningWinState(STATE_GET_FOCUS, TRUE);
            }
            else
            {
                refreshPaintWindowFull();
            }
            break;

#ifdef MICROSOFT_INLINE_EDITOR_SUPPORT
        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
            {
                MMI_NOTIFY_T *notify = (MMI_NOTIFY_T *)param;
                Jbed_ie_notifyTextChanged(notify->src_id);
            }
            break;
#endif


#ifdef SCREEN_ROTATION_SUPPORT
        //case 0xfa60:
        case MSG_LCD_SWITCH:
#ifndef VIRTUAL_KEY_BOARD
            SCI_TRACE_LOW("MSG_LCD_SWITCH - INFO: swith start... ");
            {
                LCD_ANGLE_E cur_lcd_angle = LCD_ANGLE_0;
                int rotationType = 0;
                cur_lcd_angle = MMK_GetScreenAngle();

                if(cur_lcd_angle == LCD_ANGLE_90)
                {
                    rotationType = STATIC_com_jbed_tina_event_EventSystem_EVT_SCREEN_ROTATION_90;
                }
                else if(cur_lcd_angle == LCD_ANGLE_180)
                {
                    rotationType = STATIC_com_jbed_tina_event_EventSystem_EVT_SCREEN_ROTATION_180;
                }
                else if(cur_lcd_angle == LCD_ANGLE_270)
                {
                    rotationType = STATIC_com_jbed_tina_event_EventSystem_EVT_SCREEN_ROTATION_270;
                }
                else
                {
                    rotationType = STATIC_com_jbed_tina_event_EventSystem_EVT_SCREEN_ROTATION_0;
                }
                SCI_TRACE_LOW("MSG_LCD_SWITCH - INFO: rotationType = %d", rotationType);
                Jbed_doOtherEvent(eventsHandle, rotationType);
                Jbed_wakeupVM();
            }
#endif
            break;
#endif//SCREEN_ROTATION_SUPPORT

    #ifdef KEYPAD_TYPE_QWERTY_KEYPAD
        case MSG_KEYDOWN_FN:  break;

        case MSG_KEYUP_FN:
        {
            bCapsLockPressed = bCapsLockPressed ? FALSE : TRUE ;
        }
        break;

        case MSG_KEYDOWN_SHIFT:
        {
            bShiftPressed = TRUE;
            bControlPressed = FALSE;
        }
        break;

        case MSG_KEYDOWN_CTRL:
        {
            bShiftPressed = FALSE;
            bControlPressed = TRUE;
        }
        break;

        case MSG_KEYUP_SHIFT:
        case MSG_KEYUP_CTRL:     //not support crtl + shift at the same time
        {
            bShiftPressed = FALSE;
            bControlPressed = FALSE;
        }
        break;
    #endif
#ifdef MICROSOFT_INLINE_EDITOR_SUPPORT
        case MSG_NOTIFY_OK:
            Jbed_HandleEvents(MSG_KEYDOWN, KEY_OK, FALSE);
            Jbed_HandleEvents(MSG_KEYUP, KEY_OK, FALSE);
            break;

        case MSG_NOTIFY_MIDSK:
            Jbed_HandleEvents(MSG_KEYDOWN, KEY_WEB, FALSE);
            Jbed_HandleEvents(MSG_KEYUP, KEY_WEB, FALSE);
            break;
#endif
        default:
        {
        #if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
            if (msgId > MSG_KEYUP && msgId <= MSG_KEYUP_MO)
            {
                param = (DPARAM )(msgId - MSG_KEYUP);
                Jbed_HandleEvents((MMI_MESSAGE_ID_E)MSG_KEYUP, param,FALSE);
            }
            else if (msgId > MSG_KEYDOWN && msgId <= MSG_KEYDOWN_MO)
            {
                param = (DPARAM )(msgId - MSG_KEYDOWN);
                Jbed_HandleEvents(MSG_KEYDOWN, param,FALSE);
            }
            else if (msgId > MSG_KEYPRESSUP && msgId <= MSG_KEYPRESSUP_MO)
            {
                //Consider long pressed up event is still a key up event.
                param = (DPARAM )(msgId - MSG_KEYPRESSUP);
                Jbed_HandleEvents(MSG_KEYUP, param,FALSE);
            }
        #else
            if (msgId > MSG_KEYUP && msgId < MSG_KEYUP_AT)
            {
                param = (DPARAM )(msgId - MSG_KEYUP);
                Jbed_HandleEvents((MMI_MESSAGE_ID_E)MSG_KEYUP, param,FALSE);
            }
            else if (msgId > MSG_KEYDOWN && msgId < MSG_KEYDOWN_AT)
            {
                param = (DPARAM )(msgId - MSG_KEYDOWN);
                Jbed_HandleEvents(MSG_KEYDOWN, param,FALSE);
            }
            else if (msgId > MSG_KEYPRESSUP && msgId < MSG_KEYPRESSUP_AT)
            {
                //Consider long pressed up event is still a key up event.
                param = (DPARAM )(msgId - MSG_KEYPRESSUP);
                Jbed_HandleEvents(MSG_KEYUP, param,FALSE);
            }
        #endif
        }
        break;
    }
    return mmi_ret;
}


/*****************************************************************************/
//  Description : create a window for running apps. If window has already been opened,
//                nothing to do except return false. Otherwise, open the window.
//  Global resource dependence :
//  Author: Delia Yuan
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_CreateRunningWin(void)
{
    MMI_HANDLE_T winHandle = 0;

    if(MMK_IsOpenWin(JBED_RUNNING_APP_WIN_ID))
    {
        MMK_HideWindow(JBED_RUNNING_APP_WIN_ID, FALSE);
        return;
    }
#ifdef VIRTUAL_KEY_BOARD
    GUILCD_SetLogicAngle( GUI_MAIN_LCD_ID, LCD_ANGLE_0 );
#endif
    winHandle = MMK_CreateWin((uint32*)JBED_RUNNING_APP_WIN_TAB, PNULL);
    //Fix inline editor cannot be displayed fully when start position is from (0, 0)
    MMK_SetWinHasStb(winHandle, FALSE);
    MMK_SetWinDisplayStyleState(winHandle, WS_HAS_STATUSBAR, FALSE);
}

/*****************************************************************************/
//  Description : MMIJAVA_CloseJbedRunningWindow
//  Global resource dependence : close jbed running and waiting window if they exist.
//  Author: Sean Xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_CloseRunningWin(void)
{
    if(MMK_IsOpenWin(JBED_RUNNING_APP_WIN_ID))
    {
        MMK_CloseWin(JBED_RUNNING_APP_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : MMIJAVA_SetIsJavaEnablePause
//  Global resource dependence :
//  Author:murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetJavaIsEnablePause(BOOLEAN is_enable)
{
    s_b_enable_pasue = is_enable;

}

/*****************************************************************************/
//  Description : Update running window state, or get state value
//  Global resource dependence :
//  Author: Fei Ming
//  Note: param [IN] state  int    RunningWinState value to set
//        param [IN] needUpdate  BOOLEAN   TRUE: update state FALSE: get state
//        return RunningWinState type
/*****************************************************************************/
int MMIJAVA_updateRunningWinState(int state, BOOLEAN needUpdate)
{
  static int curState = STATE_LOSE_FOCUS;

  curState = (needUpdate? state : curState);
  return curState;
}

