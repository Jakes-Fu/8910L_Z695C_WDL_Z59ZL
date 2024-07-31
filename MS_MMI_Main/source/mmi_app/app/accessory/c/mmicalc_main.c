/****************************************************************************
** File Name:      mmicalc_main.c                            
** Author:                                                                  
** Date:            2004/08/24
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to process msg to main window of CALCULATION
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 08/24/2004     taul.zhu          Create
** 12/10/2007     zhaohui		modify
****************************************************************************/
#define _MMICALC_MAIN_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_accessory_trc.h"
#include "window_parse.h"
#ifdef CALCULATOR_SUPPORT
//#include "sci_types.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guilcd.h"
#include "guifont.h"
#include "guicommon.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmicalc_export.h"
#include "mmiwclk_export.h"
#include "tb_dal.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiacc_winmain.h"
//#include "math.h"
//#include "float.h"
#include <stdio.h>
//#include <setjmp.h>
#include "mmipub.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmiacc_menutable.h"
#include "guires.h"
#include "guibutton.h"
#include "mmk_timer.h"
#include "guistring.h"
#include "guiform.h"
#include "guimenu.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/            
#define CALC_ERROR_INFO_TIME 3000
//@zhaohui,calc optimize
#define CALC_EVALUATE(DEST_PTR,SRC_PTR) MMI_MEMCPY((char*)(DEST_PTR), CALC_MAX_LENGTH+2, (char*)(SRC_PTR), CALC_MAX_LENGTH+2,strlen((char *)(SRC_PTR))+1)
//计算器显示行数，最多2行。行数调整时，同时需要调整字体大小MMICALC_TEXT_FONT与行的高度CALC_LINE_HIGHT
#ifdef MMI_CALCULATOR_MINI_SUPPORT
#define CALC_LINE_NUM               2
#else
#define CALC_LINE_NUM               2//1//change by James li
#endif
//调整字体大小的步进
#define FONT_STEP    4
//每行每列的BUTTON数

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MMI_CALCULATOR_MINI_SUPPORT)
#	define BUTTONS_EACH_LINE    4
#	define BUTTONS_EACH_ROW    6
#	define BUTTONS_EACH_LINE_H    6
#	define BUTTONS_EACH_ROW_H    4
#else
#	define BUTTONS_EACH_LINE    3
//michael wang 0423
#	if (defined MAINLCD_SIZE_128X160) || (defined MAINLCD_SIZE_176X220)
#		define BUTTONS_EACH_ROW    3
#	elif defined(MAINLCD_SIZE_240X320)
#		ifndef TOUCH_PANEL_SUPPORT
#			define BUTTONS_EACH_ROW    3
#		else
#			define BUTTONS_EACH_ROW    7
#		endif
#	else
		#define BUTTONS_EACH_ROW	3
#	endif
#	if defined(MAINLCD_SIZE_128X160)
#		define BUTTONS_EACH_LINE_H  3  //6
#	else
#		define BUTTONS_EACH_LINE_H  6
#	endif
#	define BUTTONS_EACH_ROW_H    4
#endif

/**-------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
**--------------------------------------------------------------------------*/   

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : the process message function of the CALCULATE
//	Global resource dependence : 
//   Author:     ying.xu
//	Note:   
/*****************************************************************************/
LOCAL void LcdSwitch(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : convert qwertykey to typicalkey
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL MMI_MESSAGE_ID_E ConvertKey(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : the process message function of the CALCULATE
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalcWinMsg( 
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									);

/*****************************************************************************/
// 	Description : init button text
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void InitButtonText(void);

/*****************************************************************************/
// 	Description : init button width and height
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL void InitButtonWidthandHeight(uint16 form_width, uint16 form_height);

/*****************************************************************************/
// 	Description : init button font
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL void InitButtonFont(void);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : update button status
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL void UpdateButtonStatus(MMI_MESSAGE_ID_E msg_id, BOOLEAN is_pressed);
#endif
/*****************************************************************************/
// 	Description : DigitLineEditor
//         simple digit line editor, input 0-9, .
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN DigitLineEditor(
                              uint8 *buf,
                              uint16 max_len,
                              BOOLEAN is_add,
                              uint8 ch
                              );

/***********************************************************************
**
** FUNCTION-NAME: ProcessKeyPressed
**
** DESCRIPTION:
		Only the numeric keys and the #-Key  are allowed.
**
** RETURN VALUES:
***********************************************************************/
LOCAL BOOLEAN  ProcessKeyPressed(
                                 MMI_MESSAGE_ID_E msg_id,
                                 uint8 *buf,
                                 uint16 max_len
                                 );


/* **********************************************************************
**
** LOCAL-FUNCTION-NAME:               ShowCalcError
**
** DESCRIPTION:
** .bb
** The function is called the if the calculator library returns with
** an error.
** .be
**
** PARAMETERS:   None
**
** RETURN VALUES: None
**
***********************************************************************/
LOCAL void ShowCalcError(
                         uint16 math_error,
                         MMI_WIN_ID_T win_id
                         );
#if  0
/* **********************************************************************
**
** LOCAL-FUNCTION-NAME:               ShowCalcResult
**
** DESCRIPTION:
** .bb
** The function is show the calc result,if math_error is set,then show the error msg
** PARAMETERS:   	 double t1_dResult: calc result
** RETURN VALUES: None
**
***********************************************************************/
LOCAL uint16 CalcResult(
                            double t1_dResult,
                            uint16 math_error,
                            MMI_WIN_ID_T win_id
                            );
#endif
/* **********************************************************************
**
** LOCAL-FUNCTION-NAME:               ExecuteFormula
**
** DESCRIPTION:
** The function execute the formula and get the result
** PARAMETERS:   	 
** RETURN VALUES: None
**
***********************************************************************/

LOCAL uint16 ExecuteFormula(MMI_WIN_ID_T win_id);

/***********************************************************************
**
** LOCAL-FUNCTION-NAME:               FormatFormula
**
** DESCRIPTION:
** The key_pressed-function for the CALCULATOR
**
** PARAMETERS:   The message of key
**
**
************************************************************************/
LOCAL BOOLEAN  FormatFormula(
                             MMI_WIN_ID_T win_id,
                             MMI_MESSAGE_ID_E msg_id 
                             );

/*****************************************************************************/
// 	Description : when pen ok, handle the message
//	Global resource dependence : 
//  Author:   xiaoqing.lu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePenOk(
                               MMI_WIN_ID_T win_id,
                               DPARAM param
                               );

/*****************************************************************************/
// 	Description : display the back ground
//	Global resource dependence : 
//  Author:   xiaoqing.lu
//	Note:   
/*****************************************************************************/
LOCAL void DisplayCalcBackground(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : input sign
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHashKey(void);

/***********************************************************************
		FUNCTION-DESCRIPTION
**
** LOCAL-FUNCTION-NAME:               InitCalc
**
** DESCRIPTION:
**	 This function initialises the calculator. Memory allocation
** 	is done here, too. The state is set to PMI_CALCULATOR.
**
** PARAMETERS:   None
**
** RETURN VALUES:       None
**
************************************************************************/
LOCAL void InitCalc( MMI_WIN_ID_T win_id );
LOCAL void InitCalcExceptMemory(MMI_WIN_ID_T win_id);
LOCAL void InitCalcMemory(MMI_WIN_ID_T win_id);
                   
/*****************************************************************************/
// 	Description : init the window based on form control
//	Global resource dependence : 
//  Author:   ying.xu
//	Note:   
/*****************************************************************************/
LOCAL void InitFormWin(void);

/*****************************************************************************/
// 	Description : if right softkey should show "return" or "returnaword"
//	Global resource dependence : 
//  Author:   hui.zhao
//	Note:   
/*****************************************************************************/
BOOLEAN isSoftkeyShowReturn(void);           

/*****************************************************************************/
//  Description : dispaly the contents
//  Global resource dependence :
//  Author:chunyang.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayCalcFormulaEx(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : Display the 'M' symbol at the left_top of screen 
//	Global resource dependence : 
//  Author:chunyang.wang
//	Note:   
/*****************************************************************************/
LOCAL void CalcRefreshContent(uint32 dis_type);

/*****************************************************************************/
// 	Description :the message handler of pop window 
//	Global resource dependence : 
//  Author:chunyang.wang
//	Note:   
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleCalcPopWindMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									);


/*****************************************************************************/
// 	Description :execute memory operations,such as clear,display,add,subtract.
//	Global resource dependence : 
//  Author: chunyang.wang
//	Note:   
/*****************************************************************************/
LOCAL void CalcExecuteMemoryOperations(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctr_id);

/*****************************************************************************/
// 	Description :calculate the two operators ,type: 1,2,3,4 =-*/
//	Global resource dependence : 
//  Author: ying.xu
//	Note:   
/*****************************************************************************/
LOCAL int32 CalculateTwo(uint8* result, const uint8* operand1, const uint8* operand2, uint8 type);

/*****************************************************************************/
// 	Description :softkey control 
//	Global resource dependence : 
//  Author: ying.xu
//	Note:   
/*****************************************************************************/
LOCAL void UpdateSoftkey(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id );


/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                           */
/*---------------------------------------------------------------------------*/
typedef enum  
{	
    CALC_MODE_DIGIT1,//have five state
    CALC_MODE_OPER,
    CALC_MODE_DIGIT2,
    CALC_MODE_RESULT,
    CALC_MODE_RESULT_ERROR,   
    CALC_MODE_MAX	
}CALC_MODE_T;

/*the operate window of calc
----------------------------------
|				       CALC_DIGIT1,    |
| CALC_OPER1,		 CALC_DIGIT2,   |
| CALC_OPER2,            CALC_RESULT,   |
-----------------------------------
*/

typedef enum
{
   CALC_DIGIT1,
   CALC_OPER1,
   CALC_DIGIT2,
   CALC_OPER2,
   CALC_RESULT,
   CALC_MAX
}CALC_OP_BUF_T;//@zhaohui add

//MEMORY STATUS ,added by ying.xu,20100804
typedef enum
{
   MEMORY_CLEAR,	
   MEMORY_SET,		//update memory opreation
   MEMORY_RECALL,
   MEMORY_STORE,   
   MEMORY_MAX
}MEMORY_STATUS_E;

typedef enum
{
    CALC_NO_ERROR,
    CALC_DEVIDED_BY_ZERO,
    CALC_OVERFLOW,
    CALC_IMAGINARY      // XU SHU
}CALC_RESULT_T;

static int32 s_CALC_LINE_START_X   =   0;
static int32 s_CALC_LINE_START_Y    = 0;       
static int32 s_CALC_LINE_END_X	= 0;    			
static int32 s_CALC_LINE_END_Y        = 0;  
static int32 s_CALC_LINE_HEIGHT    = 0;
static int32 s_CALC_LINE_NUM    = CALC_LINE_NUM;
static GUI_FONT_T s_CALC_FONT_SIZE = 0;
static uint16 s_hspace = 0 ;
static uint16 s_vspace = 0;
static uint16 s_margin= 0;

LOCAL uint8 s_op_m_buf[CALC_MAX_LENGTH+2];
LOCAL BOOLEAN s_need_show_result = TRUE;

LOCAL uint8 s_op_buf[5][CALC_MAX_LENGTH+2];//@zhaohui
LOCAL CALC_MODE_T s_mode = CALC_MODE_DIGIT1;
LOCAL MEMORY_STATUS_E memory_status = MEMORY_CLEAR ;
 
LOCAL uint32 key_press_status = 0;
LOCAL const uint8 s_key_map[CALC_KEY_MAP_LENGTH] =
{
    '1',         //MSG_KEYDOWN_1,          
    '2',        //MSG_KEYDOWN_2 ,         
    '3',        //MSG_KEYDOWN_3 ,         
    '4',        //MSG_KEYDOWN_4 ,         
    '5',        //MSG_KEYDOWN_5 ,         
    '6',        //MSG_KEYDOWN_6 ,         
    '7',        //MSG_KEYDOWN_7 ,         
    '8',        //MSG_KEYDOWN_8 ,         
    '9',        //MSG_KEYDOWN_9 ,         
    '.',        //MSG_KEYDOWN_STAR,  
    '0',        //MSG_KEYDOWN_0,          
    '#'        //MSG_KEYDOWN_HASH
};

LOCAL CALC_RESULT_T s_math_error_info = CALC_NO_ERROR;
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
WINDOW_TABLE( MMIACC_CALC_POPWIN_TAB ) = 
{
    //CLEAR_LCD,
//    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_ID( MMIACC_CALC_POPWIN_ID ),
    WIN_FUNC((uint32)HandleCalcPopWindMsg ),  
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#else
    WIN_HIDE_STATUS,
#endif
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    WIN_TITLE(TXT_NULL),
#endif
     END_WIN
};

WINDOW_TABLE( CALC_WIN_TAB ) = 
{
    //CLEAR_LCD,
//    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleCalcWinMsg ),    
    WIN_ID( MMIACC_CALC_WIN_ID ),
#ifdef MMI_CALCULATOR_MINI_SUPPORT
//    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    //WIN_HIDE_STATUS,
    WIN_TITLE(TXT_TOOLS_CALCULATOR),
#else
    WIN_STATUSBAR,
#endif
    END_WIN
}; 

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MMI_CALCULATOR_MINI_SUPPORT)
WINDOW_TABLE( CALC_CTRL_TAB ) = 
{
    WIN_SOFTKEY(STXT_OPTION, STXT_EQUAL, STXT_RETURN ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMI_CALC_FORMCTRL_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR1_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MCLEAR_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),	
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MRECALL_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_7_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_4_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_1_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_HASH_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR2_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MPLUS_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),  
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MMINUS_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_8_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_5_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_2_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_0_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR3_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_CLEAR_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_DIV_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_9_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_6_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_3_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_STAR_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),

       CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR4_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_BACKSPACE_CTRL_ID,MMI_CALC_CHILDFORMCTRR4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MUL_CTRL_ID,MMI_CALC_CHILDFORMCTRR4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_SUB_CTRL_ID,MMI_CALC_CHILDFORMCTRR4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_PLUS_CTRL_ID,MMI_CALC_CHILDFORMCTRR4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_EQUAL_BACKGROUND,MMIACC_CALC_EQUAL_CTRL_ID,MMI_CALC_CHILDFORMCTRR4_ID),
    
    END_WIN
}; 

WINDOW_TABLE( CALC_CTRL_TAB_H ) = 
{
    WIN_SOFTKEY(STXT_OPTION, STXT_EQUAL, STXT_RETURN ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_CALC_FORMCTRL_ID),

       CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL1_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_CLEAR_CTRL_ID,MMI_CALC_CHILDFORMCTRL1_ID),	
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MCLEAR_CTRL_ID,MMI_CALC_CHILDFORMCTRL1_ID),  
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MPLUS_CTRL_ID,MMI_CALC_CHILDFORMCTRL1_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MMINUS_CTRL_ID,MMI_CALC_CHILDFORMCTRL1_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MRECALL_CTRL_ID,MMI_CALC_CHILDFORMCTRL1_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_BACKSPACE_CTRL_ID,MMI_CALC_CHILDFORMCTRL1_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL2_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_7_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_8_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_9_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_HASH_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_MUL_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_DIV_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL3_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_4_CTRL_ID,MMI_CALC_CHILDFORMCTRL3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_5_CTRL_ID,MMI_CALC_CHILDFORMCTRL3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_6_CTRL_ID,MMI_CALC_CHILDFORMCTRL3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_0_CTRL_ID,MMI_CALC_CHILDFORMCTRL3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_SUB_CTRL_ID,MMI_CALC_CHILDFORMCTRL3_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND,MMIACC_CALC_PLUS_CTRL_ID,MMI_CALC_CHILDFORMCTRL3_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL4_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_1_CTRL_ID,MMI_CALC_CHILDFORMCTRL4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_2_CTRL_ID,MMI_CALC_CHILDFORMCTRL4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_3_CTRL_ID,MMI_CALC_CHILDFORMCTRL4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND,MMIACC_CALC_STAR_CTRL_ID,MMI_CALC_CHILDFORMCTRL4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_EQUAL_BACKGROUND,MMIACC_CALC_EQUAL_CTRL_ID,MMI_CALC_CHILDFORMCTRL4_ID),

    END_WIN
};
#else
WINDOW_TABLE( CALC_CTRL_TAB ) = 
{
    WIN_SOFTKEY(STXT_OPTION, STXT_EQUAL, STXT_RETURN ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_CALC_FORMCTRL_ID),

       CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL1_ID,MMI_CALC_FORMCTRL_ID),            
            CHILD_BUTTON_CTRL(FALSE,0,MMIACC_CALC_EMPTY_CTRL_ID_1,MMI_CALC_CHILDFORMCTRL1_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND_BAR,MMIACC_CALC_PLUS_CTRL_ID,MMI_CALC_CHILDFORMCTRL1_ID),	
            CHILD_BUTTON_CTRL(FALSE,0,MMIACC_CALC_EMPTY_CTRL_ID_2,MMI_CALC_CHILDFORMCTRL1_ID),    

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL2_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND_BAR,MMIACC_CALC_MUL_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_EQUAL_BACKGROUND_BAR,MMIACC_CALC_EQUAL_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND_BAR,MMIACC_CALC_DIV_CTRL_ID,MMI_CALC_CHILDFORMCTRL2_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL3_ID,MMI_CALC_FORMCTRL_ID),        
            CHILD_BUTTON_CTRL(FALSE,0,MMIACC_CALC_EMPTY_CTRL_ID_3,MMI_CALC_CHILDFORMCTRL3_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND_BAR,MMIACC_CALC_SUB_CTRL_ID,MMI_CALC_CHILDFORMCTRL3_ID),
            CHILD_BUTTON_CTRL(FALSE,0,MMIACC_CALC_EMPTY_CTRL_ID_4,MMI_CALC_CHILDFORMCTRL3_ID),    

#if defined(TOUCH_PANEL_SUPPORT) //#ifndef MMI_CALCULATOR_MINI_SUPPORT
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL4_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_7_CTRL_ID,MMI_CALC_CHILDFORMCTRL4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_8_CTRL_ID,MMI_CALC_CHILDFORMCTRL4_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_9_CTRL_ID,MMI_CALC_CHILDFORMCTRL4_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL5_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_4_CTRL_ID,MMI_CALC_CHILDFORMCTRL5_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_5_CTRL_ID,MMI_CALC_CHILDFORMCTRL5_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_6_CTRL_ID,MMI_CALC_CHILDFORMCTRL5_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL6_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_1_CTRL_ID,MMI_CALC_CHILDFORMCTRL6_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_2_CTRL_ID,MMI_CALC_CHILDFORMCTRL6_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_3_CTRL_ID,MMI_CALC_CHILDFORMCTRL6_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMI_CALC_CHILDFORMCTRL7_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_HASH_CTRL_ID,MMI_CALC_CHILDFORMCTRL7_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_0_CTRL_ID,MMI_CALC_CHILDFORMCTRL7_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_STAR_CTRL_ID,MMI_CALC_CHILDFORMCTRL7_ID),
#endif

    END_WIN
}; 

WINDOW_TABLE( CALC_CTRL_TAB_H ) = 
{
    WIN_SOFTKEY(STXT_OPTION, STXT_EQUAL, STXT_RETURN ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMI_CALC_FORMCTRL_ID),
#if !defined (MAINLCD_SIZE_128X160)
       CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR1_ID,MMI_CALC_FORMCTRL_ID),       
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_7_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),            
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_4_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_1_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_HASH_CTRL_ID,MMI_CALC_CHILDFORMCTRR1_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR2_ID,MMI_CALC_FORMCTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_8_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_5_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_2_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_0_CTRL_ID,MMI_CALC_CHILDFORMCTRR2_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR3_ID,MMI_CALC_FORMCTRL_ID),        
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_9_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_6_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_3_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_NUMBER_BACKGROUND_BAR,MMIACC_CALC_STAR_CTRL_ID,MMI_CALC_CHILDFORMCTRR3_ID),
#endif
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR4_ID,MMI_CALC_FORMCTRL_ID),          
            CHILD_BUTTON_CTRL(FALSE,0,MMIACC_CALC_EMPTY_CTRL_ID_1,MMI_CALC_CHILDFORMCTRR4_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND_BAR,MMIACC_CALC_MUL_CTRL_ID,MMI_CALC_CHILDFORMCTRR4_ID),
            CHILD_BUTTON_CTRL(FALSE,0,MMIACC_CALC_EMPTY_CTRL_ID_2,MMI_CALC_CHILDFORMCTRR4_ID),    

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR5_ID,MMI_CALC_FORMCTRL_ID),        
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND_BAR,MMIACC_CALC_PLUS_CTRL_ID,MMI_CALC_CHILDFORMCTRR5_ID),       
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_EQUAL_BACKGROUND_BAR,MMIACC_CALC_EQUAL_CTRL_ID,MMI_CALC_CHILDFORMCTRR5_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND_BAR,MMIACC_CALC_SUB_CTRL_ID,MMI_CALC_CHILDFORMCTRR5_ID),

        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_ORDER,MMI_CALC_CHILDFORMCTRR6_ID,MMI_CALC_FORMCTRL_ID),          
            CHILD_BUTTON_CTRL(FALSE,0,MMIACC_CALC_EMPTY_CTRL_ID_3,MMI_CALC_CHILDFORMCTRR6_ID),    
            CHILD_BUTTON_CTRL(FALSE,IMAGE_CALC_OPERATOR_BACKGROUND_BAR,MMIACC_CALC_DIV_CTRL_ID,MMI_CALC_CHILDFORMCTRR6_ID),
            CHILD_BUTTON_CTRL(FALSE,0,MMIACC_CALC_EMPTY_CTRL_ID_4,MMI_CALC_CHILDFORMCTRR6_ID),    
    
    END_WIN
}; 
#endif

/**---------------------------------------------------------------------------*
 **                       function bodies  -- global
 **---------------------------------------------------------------------------*/


/***********************************************************************
 **                       function bodies  -- local
 ***********************************************************************/
 
/*****************************************************************************/
// 	Description : the process message function of the CALCULATE
//	Global resource dependence : 
//   Author:     ying.xu
//	Note:   
/*****************************************************************************/
LOCAL void LcdSwitch(MMI_WIN_ID_T win_id)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect( win_id);
    
    if(!MMITHEME_IsMainScreenLandscape())
    {
        s_CALC_LINE_START_X = both_rect.v_rect.left + CALC_LINE_RIGHT_BORDER -1;	
        s_CALC_LINE_END_X = both_rect.v_rect.right - CALC_LINE_RIGHT_BORDER;
        s_CALC_LINE_START_Y = both_rect.v_rect.top + 1;
#ifdef MMI_CALCULATOR_MINI_SUPPORT
		s_CALC_LINE_END_Y = s_CALC_LINE_START_Y + CALC_LINE_HIGHT;
#else
        s_CALC_LINE_END_Y = s_CALC_LINE_START_Y + CALC_LINE_HIGHT * CALC_LINE_NUM ;
#endif
        s_CALC_LINE_HEIGHT = CALC_LINE_HIGHT;
        s_CALC_FONT_SIZE = MMICALC_TEXT_FONT;
        s_hspace = BUTTON_HORIZONAL_SPACE ;
        s_vspace = BUTTON_VERTICAL_SPACE;
        s_margin = FORM_MARGIN;
    }
    else
    {
        s_CALC_LINE_START_X = both_rect.h_rect.left + CALC_LINE_RIGHT_BORDER_H-1;	
        s_CALC_LINE_END_X =  both_rect.h_rect.right - CALC_LINE_RIGHT_BORDER_H;
#if defined(MAINLCD_SIZE_128X160)
#ifdef MULTI_LAN_SUPPORT_DE
        s_CALC_LINE_START_Y = both_rect.h_rect.top + 1;
#else
        s_CALC_LINE_START_Y = both_rect.h_rect.top - 3;
#endif
#else
        s_CALC_LINE_START_Y = both_rect.h_rect.top + 1;
#endif
#ifdef MMI_CALCULATOR_MINI_SUPPORT
        s_CALC_LINE_END_Y = s_CALC_LINE_START_Y + CALC_LINE_HIGHT_H;
#else
        s_CALC_LINE_END_Y = s_CALC_LINE_START_Y + CALC_LINE_HIGHT_H * CALC_LINE_NUM ;
#endif
        s_CALC_LINE_HEIGHT = CALC_LINE_HIGHT_H;
        s_CALC_FONT_SIZE = MMICALC_TEXT_FONT_H;
        s_hspace = BUTTON_HORIZONAL_SPACE_H ;
        s_vspace = BUTTON_VERTICAL_SPACE_H;
        s_margin = FORM_MARGIN_H;
    }     
}

/*****************************************************************************/
// 	Description : when pen ok, handle the message
//	Global resource dependence : 
//  Author:   xiaoqing.lu
//	Note:   
/*****************************************************************************/
LOCAL void DisplayCalcBackground(MMI_WIN_ID_T win_id)
{    
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect( win_id);

    if(MMITHEME_IsMainScreenLandscape())
    {
        GUIRES_DisplayImg(PNULL, &both_rect.h_rect,  &both_rect.h_rect,  win_id, IMAGE_COMMON_BG, &lcd_dev_info);
    }
    else
    {
        GUIRES_DisplayImg(PNULL, &both_rect.v_rect,  &both_rect.v_rect,  win_id, IMAGE_COMMON_BG, &lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : input sign
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHashKey(void)//@zhaohui modify
{
    int16       i       = 0;
    uint16       str_len = 0;
    MMI_RESULT_E result  = MMI_RESULT_TRUE;

    if (CALC_NO_ERROR != s_math_error_info) return result;

    if ((MEMORY_SET == memory_status) || (MEMORY_RECALL == memory_status) )
    {
        switch (s_mode)
        {
            case CALC_MODE_DIGIT1:
                //case CALC_MODE_RESULT_ERROR:
                SCI_MEMSET(s_op_buf[CALC_DIGIT1],0,sizeof(s_op_buf[CALC_DIGIT1]));   
                s_op_buf[CALC_DIGIT1][0] = '0' ;
                break;

            case CALC_MODE_RESULT:
                SCI_MEMSET(s_op_buf[CALC_DIGIT2],0,sizeof(s_op_buf[CALC_DIGIT2]));   
                s_op_buf[CALC_DIGIT2][0] = '0' ;
                break;
                
            default:
                break;
        }
        memory_status = MEMORY_STORE ;	
    }

    // 添加正负符号
    if ((CALC_MODE_DIGIT1 == s_mode)||(CALC_MODE_RESULT_ERROR == s_mode))
    {
#if 0        
        if (MMIAPICOM_Stricmp(s_op_buf[CALC_DIGIT1], (uint8 *)"0") == 0)
        {
            result = MMI_RESULT_FALSE;      
            //MMK_WinInactiveCtrl(MMIACC_CALC_WIN_ID, TRUE);   
            return result;
        }
#endif

        str_len = SCI_STRLEN((char*)s_op_buf[CALC_DIGIT1]);
        //if (str_len<1||str_len>CALC_MAX_LENGTH+1)              
        if (s_op_buf[CALC_DIGIT1][0] == '-')
        {
             if (str_len > CALC_MAX_LENGTH + 1 )        //13bit number + 1bit symbol
             {
                 result = MMI_RESULT_FALSE;
                 //MMK_WinInactiveCtrl(MMIACC_CALC_WIN_ID, FALSE);   
                 return result;
             } 
            for(i = 0; i < str_len; i++)
            {
                s_op_buf[CALC_DIGIT1][i] = s_op_buf[CALC_DIGIT1][i+1];
            }            
            s_op_buf[CALC_DIGIT1][str_len-1] = '\0';
        }
        else//@zhaohui,无，则加上
        {
             if (str_len > CALC_MAX_LENGTH )        
             {
                 result = MMI_RESULT_FALSE;               
                 MMK_WinInactiveCtrl(MMIACC_CALC_WIN_ID, FALSE);   
                 return result;
             } 
            for(i = str_len - 1; i >= 0; i--)
            {
                s_op_buf[CALC_DIGIT1][i+1] = s_op_buf[CALC_DIGIT1][i];
            }

            s_op_buf[CALC_DIGIT1][0] = '-';
            s_op_buf[CALC_DIGIT1][str_len + 1] = '\0';
        }
    }
    else if (CALC_MODE_DIGIT2== s_mode)
    {
        if (MMIAPICOM_Stricmp(s_op_buf[CALC_DIGIT2], (uint8 *)"0") == 0)
        {
            result = MMI_RESULT_FALSE;     
            //MMK_WinInactiveCtrl(MMIACC_CALC_WIN_ID, FALSE);   
            return result;
        }
        str_len = SCI_STRLEN((char*)s_op_buf[CALC_DIGIT2]);
        if (s_op_buf[CALC_DIGIT2][0] == '-')
        {
            if (str_len > CALC_MAX_LENGTH + 1)        
             {
                 result = MMI_RESULT_FALSE;
                 MMK_WinInactiveCtrl(MMIACC_CALC_WIN_ID, FALSE);   
                 return result;
             } 
            for(i = 0; i < str_len; i++)
            {
                s_op_buf[CALC_DIGIT2][i] = s_op_buf[CALC_DIGIT2][i+1];
            }           
            s_op_buf[CALC_DIGIT2][str_len - 1] = '\0';
        }
        else
        {
            if (str_len > CALC_MAX_LENGTH  )        
             {
                 result = MMI_RESULT_FALSE;           
                 //MMK_WinInactiveCtrl(MMIACC_CALC_WIN_ID, FALSE);   
                 return result;
             } 
            for(i = str_len - 1; i >= 0; i--)
            {
                s_op_buf[CALC_DIGIT2][i+1] = s_op_buf[CALC_DIGIT2][i];
            }
            s_op_buf[CALC_DIGIT2][0] = '-';
            s_op_buf[CALC_DIGIT2][str_len + 1] = '\0';
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;     
    }
    
    MMK_WinInactiveCtrl(MMIACC_CALC_WIN_ID, FALSE);   
    //DisplayCalcFormulaEx(MMIACC_CALC_WIN_ID);
    return result;
}

/*****************************************************************************/
// 	Description : 文本风格
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL __inline void SetCalcTextStyle(
                                     GUISTR_STYLE_T *style_ptr,
                                     GUI_ALIGN_E align,
                                     GUI_FONT_T  font,
                                     GUI_COLOR_T font_color
                                     )
{
    //SCI_ASSERT(PNULL != style_ptr);/*assert verified*/
    if(PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"SetCalcTextStyle:PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICALC_MAIN_784_112_2_18_2_1_23_70,(uint8*)"");
        return;
    }
    style_ptr->align = align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 4;
    style_ptr->bg_color = MMI_WHITE_COLOR;
}

/*****************************************************************************/
//  Description : dispaly content as the calculator state.
//  Global resource dependence :
//  Author:chunyang.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayCalcFormulaEx(MMI_WIN_ID_T win_id)
{ 
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T			     rect = { 0};
    rect.left = CALC_LINE_OP_STARTX ;
    rect.top =    CALC_LINE_OP_STARTY(1);/*lint !e778*/
    rect.right = CALC_LINE_NUM_ENDX ;
    rect.bottom = CALC_LINE_NUM_ENDY(CALC_LINE_NUM);     

   GUIRES_DisplayImg(PNULL, &rect,  &rect,  win_id, IMAGE_COMMON_BG, &lcd_dev_info);

    switch(s_mode)
    {
        case CALC_MODE_DIGIT1:  
            CalcRefreshContent(ACC_CALC_DISPLAY_M_SYMBOL|ACC_CALC_DISPLAY_FIRST_OPERAND);
            break;

        case CALC_MODE_OPER:
            CalcRefreshContent(ACC_CALC_DISPLAY_M_SYMBOL|ACC_CALC_DISPLAY_FIRST_OPERAND|ACC_CALC_DISPLAY_OPERATOR);
            break;

        case CALC_MODE_DIGIT2:
            CalcRefreshContent(ACC_CALC_DISPLAY_M_SYMBOL|ACC_CALC_DISPLAY_FIRST_OPERAND |ACC_CALC_DISPLAY_OPERATOR|ACC_CALC_DISPLAY_SECOND_OPERAND);
            break;      

        case CALC_MODE_RESULT_ERROR:
            CalcRefreshContent(ACC_CALC_DISPLAY_M_SYMBOL|ACC_CALC_DISPLAY_FIRST_OPERAND);
            break;

        case CALC_MODE_RESULT:  
            CalcRefreshContent(ACC_CALC_DISPLAY_M_SYMBOL|ACC_CALC_DISPLAY_EQUAL_SYMBOL|ACC_CALC_DISPLAY_RESULT);     
            break;

        default:
        break;
    } 
}

/***********************************************************************
**
** FUNCTION-NAME: ProcessKeyPressed
**
** DESCRIPTION:
		Only the numeric keys and the #-Key  are allowed.
**
** RETURN VALUES:
***********************************************************************/
LOCAL BOOLEAN  ProcessKeyPressed  (
                                   MMI_MESSAGE_ID_E msg_id,
                                   uint8 *buf,
                                   uint16 max_len
                                   )
{
    BOOLEAN recode = TRUE;     //default, the key is processed
    //SCI_ASSERT(PNULL != buf);/*assert verified*/
    //SCI_ASSERT(max_len >0);/*assert verified*/    
    if(PNULL == buf || max_len == 0)
    {
        //SCI_TRACE_LOW:"ProcessKeyPressed:PNULL == buf || max_len <= 0 !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICALC_MAIN_854_112_2_18_2_1_24_71,(uint8*)"");
        return FALSE;
    }
    
    switch (msg_id)
    {
    case MSG_KEYDOWN_CANCEL://cr132551
    case MSG_KEYDOWN_DEL:

    //case MSG_KEYDOWN_HASH://@PHOEBE 0706 
        recode = DigitLineEditor(buf, max_len, FALSE,' ');		// delete a char 
        break;		
        		
    case MSG_KEYDOWN_0:		// forbit 00xxx  or -0xxx
        if ((MMIAPICOM_Stricmp(buf, (uint8 *)"0") != 0) && (MMIAPICOM_Stricmp(buf, (uint8 *)"-0") !=0))
        {
            recode = DigitLineEditor(buf, max_len, TRUE, s_key_map[msg_id - CALC_FIRST_MAP]);
        }
        break;
            
    case MSG_KEYDOWN_STAR:	//@PHOEBE 0706 from hash	// add char of ' .' , forbit -.  xx.xx.
        if ((MMIAPICOM_Stricmp(buf, (uint8 *)"-") !=0) && (MMIAPICOM_SearchChar(buf, '.') ==NULL ))
        {
            recode = DigitLineEditor(buf, max_len, TRUE, s_key_map[msg_id - CALC_FIRST_MAP]);
        }
     	break;		
        		
    case MSG_KEYDOWN_1:		
    case MSG_KEYDOWN_2:		
    case MSG_KEYDOWN_3:		
    case MSG_KEYDOWN_4:		
    case MSG_KEYDOWN_5:		
    case MSG_KEYDOWN_6:		
    case MSG_KEYDOWN_7:		
    case MSG_KEYDOWN_8:		
    case MSG_KEYDOWN_9:		
        if( MMIAPICOM_Stricmp(buf, (uint8 *)"0") == 0)   // 0xxx not allowed
        {//remove first '0'		
            buf[0] = 0;
        }
        else if ( MMIAPICOM_Stricmp(buf, (uint8 *)"-0") == 0)  // -0x  allowed
        {		
            buf[1] = 0;		// change to  -xxx
        }
        if ( (msg_id - CALC_FIRST_MAP) <CALC_KEY_MAP_LENGTH)
        {
        DigitLineEditor(buf, max_len, TRUE, s_key_map[msg_id - CALC_FIRST_MAP]);		
        }
        break;		
    default:		
        recode = FALSE;
        break;		
    }// end of switch 

    return recode;
}

/* **********************************************************************
* FUNCTION-DESCRIPTION
**
** LOCAL-FUNCTION-NAME:               ShowCalcError
**
** DESCRIPTION:
** .bb
** The function is called the if the calculator library returns with
** an error.
** .be
**
** PARAMETERS:   None
**
** RETURN VALUES: None
**
*/
LOCAL void ShowCalcError(uint16 math_error,MMI_WIN_ID_T win_id)
{
    MMI_TEXT_ID_T  label = TXT_NULL;

    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T        text = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_NOT_HALF_CHAR;  
#else
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_NOT_HALF_CHAR;  
#endif
    GUI_RECT_T			     rect = {0};
    
    rect.left = CALC_LINE_OP_STARTX;  
    rect.top = CALC_LINE_OP_STARTY(1);/*lint !e778*/
    rect.right = CALC_LINE_NUM_ENDX;
    rect.bottom = CALC_LINE_NUM_ENDY(CALC_LINE_NUM);  
    
    switch (math_error)
    {
        case CALC_OVERFLOW:
            label = TXT_ACC_OVERFLOW;            
            break;
        case CALC_DEVIDED_BY_ZERO:
            label = TXT_ACC_DIVIDE_BY_ZERO;           
            break;
        case CALC_IMAGINARY:
            label = TXT_ACC_IMAGINARY;
            break;
        default:
            label = TXT_COMM_ERROR;
            break;
    }
    
    //   InitCalc(win_id);

    GUIRES_DisplayImg(PNULL, &rect,  &rect,  win_id, IMAGE_COMMON_BG, &lcd_dev_info);

    MMI_GetLabelTextByLang(label, &text);

    // style
    SetCalcTextStyle(&text_style, ALIGN_HVMIDDLE, s_CALC_FONT_SIZE, CALC_TEXT_COLOR);
//#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
    //  adjust font size 
    while(GUI_GetStringWidth(text_style.font, text.wstr_ptr,text.wstr_len) > (rect.right - rect.left))
    {
        if(GUI_GetStringHeight(text_style.font, text.wstr_ptr,text.wstr_len) < (rect.bottom- rect.top)/2-1)
        {
            //字体高度小于区域一半的高度时，分行显示
            state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_NO_HALF_LINE;  
            while(2 < GUI_CalculateStringLinesByPixelNum(rect.right - rect.left, text.wstr_ptr, text.wstr_len, text_style.font, text_style.word_space, TRUE))//1
            {                        
				if (text_style.font>1)
				{
                text_style.font--;   
				}
				else
				break;
            }
            break;
        }                
		if (text_style.font>FONT_STEP)
		{
        text_style.font-= FONT_STEP;   
		}
		else
		 break;
    }  
//#endif
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&text,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    MMK_WinInactiveCtrl(win_id, FALSE);    
} /* end ShowCalcError */

#if  0
/* **********************************************************************
**
** LOCAL-FUNCTION-NAME:               ShowCalcResult
**
** DESCRIPTION:
** .bb
** The function is show the calc result,if math_error is set,then show the error msg
** PARAMETERS:   	 double t1_dResult: calc result
** RETURN VALUES: None
**
***********************************************************************/
LOCAL uint16 CalcResult(//@zhaohui modify
                double t1_dResult, 
                uint16 math_error,
                MMI_WIN_ID_T  win_id)
{
    int16 ilength = -1;
    uint8 buf[CALC_MAX_LENGTH*2] = {0};
    uint8 calc_digit1_length = 0;
    uint8 calc_digit2_length = 0;
    uint8 calc_result_length = 0;
    
    if((CALC_NO_ERROR == math_error &&(t1_dResult<1.0e-100&&t1_dResult>-1.0e-100) &&(0 != t1_dResult))
        ||(0 == math_error &&(t1_dResult>9.9e100||t1_dResult<-(9.9e100)))
        )
    {
        math_error = CALC_OVERFLOW;
    }

    if(CALC_NO_ERROR == math_error) /* No error */
    {
        calc_digit1_length = SCI_STRLEN((char*)s_op_buf[CALC_DIGIT1]);
        calc_digit2_length = SCI_STRLEN((char*)s_op_buf[CALC_DIGIT2]);
        calc_result_length = MAX(calc_digit1_length,calc_digit2_length);
	
        if(CALC_MAX_LENGTH-4 >= calc_result_length)//根据最大位数来显示结果
        {
            sprintf ( (char*)buf, "%-.12g", t1_dResult);
        }
        else
        {
            sprintf ( (char*)buf, "%-.15g", t1_dResult);
        }
        ilength = SCI_STRLEN((char*)buf);
        //超过最大可显示的最大长度，改为指数形式显示
        if(ilength>CALC_MAX_LENGTH)
        {
            if (s_mode == CALC_MODE_RESULT_ERROR)
            {
                sprintf((char*)s_op_buf[CALC_DIGIT1], "%-e", t1_dResult);//显示在第一行的结果
            }
            else
            {
                sprintf((char*)s_op_buf[CALC_RESULT], "%-e", t1_dResult);//显示在第三行的结果
            }
        }
        else
        {
            if (s_mode == CALC_MODE_RESULT_ERROR)
            {
                MMI_MEMCPY((char *)s_op_buf[CALC_DIGIT1], sizeof(s_op_buf[CALC_DIGIT1]),
                    (char *)buf,CALC_MAX_LENGTH*2,
                    strlen((char *)buf)+1); /*lint !e666*/ //要将结束符copy进去
            }
            else
            {
                MMI_MEMCPY((char *)s_op_buf[CALC_RESULT], sizeof(s_op_buf[CALC_RESULT]),
                    (char *)buf,CALC_MAX_LENGTH*2, strlen((char *)buf)+1);/*lint !e666*/
            }
        }      
    }
    else// Error 
    {
        s_math_error_info = math_error; /*lint !e64*/
    }   

    return math_error;
}
#endif
/* **********************************************************************
**
** LOCAL-FUNCTION-NAME:               ExecuteFormula
**
** DESCRIPTION:
** The function execute the formula and get the result
** PARAMETERS:   	 
** RETURN VALUES: None
**
***********************************************************************/
LOCAL uint16 ExecuteFormula(MMI_WIN_ID_T win_id)
{
    uint16 math_error = 0;

    math_error = CalculateTwo(s_op_buf[CALC_RESULT], s_op_buf[CALC_DIGIT1], 
    	s_op_buf[CALC_DIGIT2] , s_op_buf[CALC_OPER1][0]);

    return math_error;
}

/***********************************************************************
**
** LOCAL-FUNCTION-NAME:               FormatFormula
**
** DESCRIPTION:
** The key_pressed-function for the CALCULATOR
**
** PARAMETERS:   The message of key
**
**
************************************************************************/
LOCAL BOOLEAN  FormatFormula(//@zhaohui modify
							 MMI_WIN_ID_T win_id,
							 MMI_MESSAGE_ID_E msg_id 
							 )
{
    BOOLEAN recode =TRUE;
    CALC_RESULT_T calc_result = CALC_NO_ERROR;

    /*用于"回退"时，判断当前第一行是"操作数"还是"结果"，相应的处理会不一样.
    若是操作数则逐位清除，若是"结果"则要整个清除*/
#ifndef MMI_CALCULATOR_MINI_SUPPORT
    static CALC_MODE_T last_mode = CALC_MODE_DIGIT1;
#endif
    //M+/M-/MR操作后，用于直接清除操作数1或者2
    if ((MEMORY_SET == memory_status) || (MEMORY_RECALL == memory_status) )
    {             
        switch (msg_id)
        {
            case MSG_KEYDOWN_STAR:	
            case MSG_KEYDOWN_0:
            case MSG_KEYDOWN_1:		
            case MSG_KEYDOWN_2:		
            case MSG_KEYDOWN_3:		
            case MSG_KEYDOWN_4:		
            case MSG_KEYDOWN_5:		
            case MSG_KEYDOWN_6:		
            case MSG_KEYDOWN_7:		
            case MSG_KEYDOWN_8:		
            case MSG_KEYDOWN_9:	
                switch (s_mode)
                {
                    case CALC_MODE_DIGIT1:
                        //case CALC_MODE_RESULT_ERROR:
                        SCI_MEMSET(s_op_buf[CALC_DIGIT1],0,sizeof(s_op_buf[CALC_DIGIT1]));   
                        s_op_buf[CALC_DIGIT1][0] = '0';
                        break;

                    case CALC_MODE_DIGIT2:
                        SCI_MEMSET(s_op_buf[CALC_DIGIT2],0,sizeof(s_op_buf[CALC_DIGIT2]));   
                        s_op_buf[CALC_DIGIT2][0] = '0';
                        break;
                        
                    default:
                        break;
                }		
                memory_status = MEMORY_STORE ;	
                break;
                
            default:
                break;
        }		
    }	
	
    //deal with key input,s_mode has five states
    if (s_mode == CALC_MODE_DIGIT1)			// first operatant
    {
        switch (msg_id)
        {
            case MSG_KEYDOWN_UP:        // ADD
            case MSG_KEYDOWN_LEFT:      // MULTI
            case MSG_KEYDOWN_RIGHT:     //  DIVIDE
                if ((MMIAPICOM_Stricmp(s_op_buf[CALC_DIGIT1], "-") !=0)) /*lint !e64*/
                {
                    s_op_buf[CALC_OPER1][1] = 0;
                    if (msg_id == MSG_KEYDOWN_UP /*|| msg_id == MSG_KEYDOWN_STAR*/)//@PHOEBE 0706
                    {
                        s_op_buf[CALC_OPER1][0] = '+';
                    }
                    else if (msg_id == MSG_KEYDOWN_LEFT)
                    {
                        s_op_buf[CALC_OPER1][0] = 215 /* '*' */;
                    }
                    else
                    {
                        s_op_buf[CALC_OPER1][0] = 247 /* '/' */;
                    }
                    s_mode = CALC_MODE_OPER;
#ifndef MMI_CALCULATOR_MINI_SUPPORT
                    last_mode = CALC_MODE_DIGIT1;
#endif
                    MMIAPICOM_CheckDecimalPoint(s_op_buf[CALC_DIGIT1]);//check if digit1 is "xx."
                }
                break;

            case MSG_KEYDOWN_DOWN:      // SUB
                {
                    //input operator
                    s_op_buf[CALC_OPER1][0] = '-';
                    s_op_buf[CALC_OPER1][1] = 0;
                    s_mode = CALC_MODE_OPER;
#ifndef MMI_CALCULATOR_MINI_SUPPORT
                    last_mode = CALC_MODE_DIGIT1;
#endif
                    MMIAPICOM_CheckDecimalPoint(s_op_buf[CALC_DIGIT1]);//check if digit1 is "xx."
                }
                break;
            case MSG_KEYLONG_DEL:
            case MSG_KEYLONG_CANCEL:
                SCI_MEMSET(s_op_buf[CALC_DIGIT1], 0, (CALC_MAX_LENGTH+2)); 			
                s_op_buf[CALC_DIGIT1][0] = '0';	
                break;

            case MSG_KEYDOWN_HASH:
                recode = HandleHashKey();//好象刷了两次，再仔细看看
                break;
            case MSG_KEYDOWN_DEL:
            case MSG_KEYDOWN_CANCEL://cr132551
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT1],"0") ==0) /*lint !e64*/
                {
                    recode = FALSE;    // let window closed 
                }
                else 
                {
                    ProcessKeyPressed(msg_id, s_op_buf[CALC_DIGIT1], CALC_MAX_LENGTH);
				//	UpdateSoftkey(win_id, msg_id);//cr132551
                    if (SCI_STRLEN((char*)s_op_buf[CALC_DIGIT1]) ==0 || MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT1],"-") ==0) /*lint !e64*/
                    {
                        s_op_buf[CALC_DIGIT1][0] = '0';
                        s_op_buf[CALC_DIGIT1][1] = 0;
                    }
                }
                break;

            default:
                recode = ProcessKeyPressed(msg_id, s_op_buf[CALC_DIGIT1], CALC_MAX_LENGTH);
                break;
        }
    }
    else if (s_mode == CALC_MODE_OPER)		// get operator +,- x /
    {
        switch (msg_id)
        {
            case MSG_KEYDOWN_UP://改变操作符
                s_op_buf[CALC_OPER1][0] = '+';
                break;

            case MSG_KEYDOWN_LEFT:
                s_op_buf[CALC_OPER1][0] = 215 /* '*' */;
                break;

            case MSG_KEYDOWN_RIGHT:
                s_op_buf[CALC_OPER1][0] = 247 /* '/' */;
                break;

            case MSG_KEYDOWN_DOWN:
                s_op_buf[CALC_OPER1][0] = '-';
                break;
            case MSG_KEYDOWN_DEL:
            case MSG_KEYLONG_DEL:
            case MSG_KEYLONG_CANCEL:
            case MSG_KEYDOWN_CANCEL://cr132551
                s_op_buf[CALC_OPER1][0] = 0;   //  delete operator
                s_op_buf[CALC_OPER1][1] = 0;       
                s_mode = CALC_MODE_DIGIT1;    // chage state 	
                break;


            case MSG_KEYDOWN_0:
            case MSG_KEYDOWN_1:
            case MSG_KEYDOWN_2:
            case MSG_KEYDOWN_3:
            case MSG_KEYDOWN_4:
            case MSG_KEYDOWN_5:
            case MSG_KEYDOWN_6:
            case MSG_KEYDOWN_7:
            case MSG_KEYDOWN_8:
            case MSG_KEYDOWN_9:
                s_mode = CALC_MODE_DIGIT2;
                s_op_buf[CALC_DIGIT2][0] = 0; //clear digit2
                ProcessKeyPressed(msg_id, s_op_buf[CALC_DIGIT2], CALC_MAX_LENGTH);
                break;

            case MSG_KEYDOWN_STAR:	
                s_mode = CALC_MODE_DIGIT2;
                s_op_buf[CALC_DIGIT2][0] = '0'; //set digit2

                s_op_buf[CALC_DIGIT2][1] = 0;   //set digit2
                ProcessKeyPressed(msg_id, s_op_buf[CALC_DIGIT2], CALC_MAX_LENGTH);
                break;

            case MSG_CTL_MIDSK:        
            case MSG_KEYDOWN_WEB:        	
                s_mode = CALC_MODE_RESULT;
                CALC_EVALUATE(s_op_buf[CALC_DIGIT2],s_op_buf[CALC_DIGIT1]);/*lint !e666*/ //digit2 = digit1
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT2], "-") == 0) /*lint !e64*/
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                }      
                else 
                {
                    recode = FALSE;
                    calc_result = (CALC_RESULT_T)ExecuteFormula(win_id);
                    switch (calc_result)
                    {
                        case CALC_NO_ERROR:
                            s_mode = CALC_MODE_RESULT;
                            break;

                        case CALC_OVERFLOW:
                        case CALC_DEVIDED_BY_ZERO:
                            s_mode = CALC_MODE_DIGIT1;
                            break;

                        default:
                            s_mode = CALC_MODE_DIGIT2;
                            break;
                    }
                }
                break;

            default:
                recode = FALSE;
                break;
        }
    }
    else if (s_mode == CALC_MODE_DIGIT2)		// get second operatant
    {
        switch (msg_id)
        {
            case MSG_KEYDOWN_UP://"与点=  的消息处理相同"
                MMIAPICOM_CheckDecimalPoint(s_op_buf[CALC_DIGIT2]);//check if digit2 is "xx."
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT2], "-") == 0) /*lint !e64*///由于上面MSG_KEYDOWN_DOWN的操作产生了该情况的可能性，不知有何用
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                }      
                else 
                {
                    recode = FALSE;
                    //calc_result = (CALC_RESULT_T)ExecuteFormula(win_id);
                    calc_result =  CalculateTwo(s_op_buf[CALC_DIGIT1], s_op_buf[CALC_DIGIT1], 
                    s_op_buf[CALC_DIGIT2] , s_op_buf[CALC_OPER1][0]);	 	
                    switch (calc_result)
                    {
                        case CALC_NO_ERROR:
                            s_mode = CALC_MODE_OPER;
                            s_op_buf[CALC_OPER1][1] = 0;
                            s_op_buf[CALC_OPER1][0] = '+';	
                            break;

                        case CALC_OVERFLOW:
                        case CALC_DEVIDED_BY_ZERO:
                            s_mode = CALC_MODE_DIGIT1;
                            break;

                        default:
                            s_mode = CALC_MODE_DIGIT2;
                            break;
                    }
                }
                break;

            case MSG_KEYDOWN_LEFT:                
                MMIAPICOM_CheckDecimalPoint(s_op_buf[CALC_DIGIT2]);//check if digit2 is "xx."
                if (MMIAPICOM_Stricmp(s_op_buf[CALC_DIGIT2], (uint8 *)"-") == 0) /*lint !e64*///由于上面MSG_KEYDOWN_DOWN的操作产生了该情况的可能性，不知有何用
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                }      
                else 
                {
                    recode = FALSE;
                    //calc_result = (CALC_RESULT_T)ExecuteFormula(win_id);
                    calc_result =  CalculateTwo(s_op_buf[CALC_DIGIT1], s_op_buf[CALC_DIGIT1], 
                    s_op_buf[CALC_DIGIT2] , s_op_buf[CALC_OPER1][0]);
                    switch (calc_result)
                    {
                        case CALC_NO_ERROR:
                            s_mode = CALC_MODE_OPER;
                            s_op_buf[CALC_OPER1][1] = 0;
                            s_op_buf[CALC_OPER1][0] = 215 /* '*' */;
                            break;

                        case CALC_OVERFLOW:
                        case CALC_DEVIDED_BY_ZERO:
                            s_mode = CALC_MODE_DIGIT1;
                            break;

                        default:
                            s_mode = CALC_MODE_DIGIT2;
                            break;
                    }
                }
                break;

            case MSG_KEYDOWN_RIGHT:                	
                MMIAPICOM_CheckDecimalPoint(s_op_buf[CALC_DIGIT2]);//check if digit2 is "xx."
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT2], "-") == 0) /*lint !e64*///由于上面MSG_KEYDOWN_DOWN的操作产生了该情况的可能性，不知有何用
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                }      
                else 
                {
                    recode = FALSE;
                    //calc_result = (CALC_RESULT_T)ExecuteFormula(win_id);
                    calc_result =  CalculateTwo(s_op_buf[CALC_DIGIT1], s_op_buf[CALC_DIGIT1], 
                    s_op_buf[CALC_DIGIT2] , s_op_buf[CALC_OPER1][0]);

                    switch (calc_result)
                    {
                        case CALC_NO_ERROR:
                            s_mode = CALC_MODE_OPER;
                            s_op_buf[CALC_OPER1][1] = 0;
                            s_op_buf[CALC_OPER1][0] = 247 /* '/' */;
                            break;

                        case CALC_OVERFLOW:
                        case CALC_DEVIDED_BY_ZERO:
                            s_mode = CALC_MODE_DIGIT1;
                            break;

                        default:
                            s_mode = CALC_MODE_DIGIT2;
                            break;
                    }
                }
                break;	

            case MSG_KEYDOWN_DOWN:          // SUB                	
                MMIAPICOM_CheckDecimalPoint(s_op_buf[CALC_DIGIT2]);//check if digit2 is "xx."
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT2], "-") == 0) /*lint !e64*///由于上面MSG_KEYDOWN_DOWN的操作产生了该情况的可能性，不知有何用
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                }      
                else 
                {
                    recode = FALSE;
                    //calc_result = (CALC_RESULT_T)ExecuteFormula(win_id);
                    calc_result =  CalculateTwo(s_op_buf[CALC_DIGIT1], s_op_buf[CALC_DIGIT1], 
                    s_op_buf[CALC_DIGIT2] , s_op_buf[CALC_OPER1][0]);
                    switch (calc_result)
                    {
                        case CALC_NO_ERROR:
                            s_mode = CALC_MODE_OPER;
                            s_op_buf[CALC_OPER1][1] = 0;
                            s_op_buf[CALC_OPER1][0] = '-';
                            break;

                        case CALC_OVERFLOW:
                        case CALC_DEVIDED_BY_ZERO:
                            s_mode = CALC_MODE_DIGIT1;
                            break;

                        default:
                            s_mode = CALC_MODE_DIGIT2;
                            break;
                    }
                }
                break;

            case MSG_KEYLONG_CANCEL:
                SCI_MEMSET(s_op_buf[CALC_DIGIT2], 0, (CALC_MAX_LENGTH+2)); 	
                s_mode = CALC_MODE_OPER;    // chage state 
                break;

            case MSG_KEYDOWN_HASH:
                recode = HandleHashKey();
                break;

            case MSG_KEYDOWN_CANCEL://cr132551
            case MSG_KEYDOWN_DEL:
                ProcessKeyPressed(msg_id, s_op_buf[CALC_DIGIT2], CALC_MAX_LENGTH);
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT2],"-") ==0) /*lint !e64*/
                {
                    s_op_buf[CALC_DIGIT2][0] = 0;
                }
                if (0 == s_op_buf[CALC_DIGIT2][0])
                {
                    s_mode = CALC_MODE_OPER;    // chage state 
                }
                break;
                
            case MSG_CTL_MIDSK:
            case MSG_KEYDOWN_WEB:        
                s_mode = CALC_MODE_RESULT;
                MMIAPICOM_CheckDecimalPoint(s_op_buf[CALC_DIGIT2]);//check if digit2 is "xx."
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT2], "-") == 0) /*lint !e64*///由于上面MSG_KEYDOWN_DOWN的操作产生了该情况的可能性，不知有何用
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                }      
                else 
                {
                    recode = FALSE;
                    calc_result = (CALC_RESULT_T)ExecuteFormula(win_id);
                    switch (calc_result)
                    {
                        case CALC_NO_ERROR:
                            s_mode = CALC_MODE_RESULT;
                            break;

                        case CALC_OVERFLOW:
                        case CALC_DEVIDED_BY_ZERO:
                            s_mode = CALC_MODE_DIGIT1;
                            break;

                        default:
                            s_mode = CALC_MODE_DIGIT2;
                            break;
                    }
                }
                break;

            default:
                recode = ProcessKeyPressed(msg_id, s_op_buf[CALC_DIGIT2], CALC_MAX_LENGTH);
                break;
        }
    }
    else if (s_mode == CALC_MODE_RESULT)
    {
        switch (msg_id)
        {
            case MSG_KEYDOWN_UP:
                s_op_buf[CALC_OPER1][0] = '+';
                s_mode = CALC_MODE_OPER;
#ifndef MMI_CALCULATOR_MINI_SUPPORT
                last_mode = CALC_MODE_RESULT_ERROR;
#endif
                CALC_EVALUATE(s_op_buf[CALC_DIGIT1],s_op_buf[CALC_RESULT]);/*lint !e666*/ //digit1 = result
                //清空s_op_buf[CALC_RESULT]
                SCI_MEMSET(s_op_buf[CALC_OPER2], 0, (CALC_MAX_LENGTH+1)); 
                break;

            case MSG_KEYDOWN_LEFT:
                s_op_buf[CALC_OPER1][0] = 215 /* '*' */;
                s_mode = CALC_MODE_OPER;
#ifndef MMI_CALCULATOR_MINI_SUPPORT
                last_mode = CALC_MODE_RESULT_ERROR;
#endif
                CALC_EVALUATE(s_op_buf[CALC_DIGIT1],s_op_buf[CALC_RESULT]);/*lint !e666*/ //digit1 = result
                //清空s_op_buf[CALC_RESULT]
                SCI_MEMSET(s_op_buf[CALC_OPER2], 0, (CALC_MAX_LENGTH+1)); 
                break;

            case MSG_KEYDOWN_RIGHT:
                s_op_buf[CALC_OPER1][0] = 247 /* '/' */;
                s_mode = CALC_MODE_OPER;
#ifndef MMI_CALCULATOR_MINI_SUPPORT
                last_mode = CALC_MODE_RESULT_ERROR;
#endif
                CALC_EVALUATE(s_op_buf[CALC_DIGIT1],s_op_buf[CALC_RESULT]);/*lint !e666*/ //digit1 = result
                //清空s_op_buf[CALC_RESULT]
                SCI_MEMSET(s_op_buf[CALC_OPER2], 0, (CALC_MAX_LENGTH+1)); 
                break;

            case MSG_KEYDOWN_DOWN:
                s_op_buf[CALC_OPER1][0] = '-';
                s_mode = CALC_MODE_OPER;
#ifndef MMI_CALCULATOR_MINI_SUPPORT
                last_mode = CALC_MODE_RESULT_ERROR;
#endif
                CALC_EVALUATE(s_op_buf[CALC_DIGIT1],s_op_buf[CALC_RESULT]);/*lint !e666*/ //digit1 = result
                //清空s_op_buf[CALC_RESULT]
                SCI_MEMSET(s_op_buf[CALC_OPER2], 0, (CALC_MAX_LENGTH+1));
                break;

            case MSG_KEYLONG_CANCEL:
            case MSG_KEYDOWN_CANCEL://cr132551
            case MSG_KEYDOWN_DEL:
            case MSG_KEYLONG_DEL:
                SCI_MEMSET(s_op_buf[CALC_DIGIT1], 0, CALC_MAX_LENGTH+2);
                s_op_buf[CALC_DIGIT1][0] = '0';
                s_mode = CALC_MODE_DIGIT1;
			//	UpdateSoftkey(win_id, msg_id);//cr132551
                break;   

            case MSG_KEYDOWN_STAR://@PHOEBE 0706 from hash
            case MSG_KEYDOWN_0:
            case MSG_KEYDOWN_1:
            case MSG_KEYDOWN_2:
            case MSG_KEYDOWN_3:
            case MSG_KEYDOWN_4:
            case MSG_KEYDOWN_5:
            case MSG_KEYDOWN_6:
            case MSG_KEYDOWN_7:
            case MSG_KEYDOWN_8:
            case MSG_KEYDOWN_9:
                SCI_MEMSET(s_op_buf[CALC_DIGIT1], 0, CALC_MAX_LENGTH+2);
                s_op_buf[CALC_DIGIT1][0] = '0';
                s_mode = CALC_MODE_DIGIT1;
                ProcessKeyPressed(msg_id, s_op_buf[CALC_DIGIT1], CALC_MAX_LENGTH);
                break;
            
            case MSG_CTL_MIDSK:     //连等与，将结果作为操作数1，重复前一次的操作     
            case MSG_KEYDOWN_WEB:        	
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT2], "-") == 0) /*lint !e64*///由于上面MSG_KEYDOWN_DOWN的操作产生了该情况的可能性，不知有何用
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                }      
                else 
                {
                    recode = FALSE;

                    calc_result =  CalculateTwo(s_op_buf[CALC_DIGIT1], s_op_buf[CALC_RESULT], 
                    	s_op_buf[CALC_DIGIT2] , s_op_buf[CALC_OPER1][0]);
                    switch (calc_result)
                    {
                        case CALC_NO_ERROR:
                            s_mode = CALC_MODE_RESULT_ERROR;//注意，此时结果在第一行显示
                            break;

                        case CALC_OVERFLOW:
                        case CALC_DEVIDED_BY_ZERO:
                            s_mode = CALC_MODE_DIGIT1;
                            break;

                        default:
                            s_mode = CALC_MODE_DIGIT2;
                            break;
                    }
                }
                break;

            default:
                recode = FALSE;
                break;
        }
    }
    else
    {//CALC_MODE_RESULT_ERROR
        switch (msg_id)
        {
            case MSG_KEYDOWN_UP:
                s_op_buf[CALC_OPER1][0] = '+';
                s_mode = CALC_MODE_OPER;			
                SCI_MEMSET(s_op_buf[CALC_DIGIT2], 0, (CALC_MAX_LENGTH+1));
                break;

            case MSG_KEYDOWN_LEFT:
                s_op_buf[CALC_OPER1][0] = 215 /* '*' */;
                s_mode = CALC_MODE_OPER;			
                SCI_MEMSET(s_op_buf[CALC_DIGIT2], 0, (CALC_MAX_LENGTH+1));
                break;

            case MSG_KEYDOWN_RIGHT:
                s_op_buf[CALC_OPER1][0] = 247 /* '/' */;
                s_mode = CALC_MODE_OPER;
                SCI_MEMSET(s_op_buf[CALC_DIGIT2], 0, (CALC_MAX_LENGTH+1));
                break;

            case MSG_KEYDOWN_DOWN:
                s_op_buf[CALC_OPER1][0] = '-';
                s_mode = CALC_MODE_OPER;
                SCI_MEMSET(s_op_buf[CALC_DIGIT2], 0, (CALC_MAX_LENGTH+1));
                break;

            case MSG_KEYLONG_CANCEL:
            case MSG_KEYDOWN_CANCEL: //cr132551
            case MSG_KEYDOWN_DEL:
            case MSG_KEYLONG_DEL:
			{
				//Don't clear s_op_m_buf and memory_status
				uint8 s_op_m_buf_bak[CALC_MAX_LENGTH+2] = {0};
				MEMORY_STATUS_E memory_status_bak = memory_status;
				memcpy(s_op_m_buf_bak, s_op_m_buf, CALC_MAX_LENGTH);

				InitCalc(win_id);

				memory_status = memory_status_bak; 
				memcpy(s_op_m_buf, s_op_m_buf_bak, CALC_MAX_LENGTH);
				break;
			}			
            case MSG_KEYDOWN_STAR:
            case MSG_KEYDOWN_0:
            case MSG_KEYDOWN_1:
            case MSG_KEYDOWN_2:
            case MSG_KEYDOWN_3:
            case MSG_KEYDOWN_4:
            case MSG_KEYDOWN_5:
            case MSG_KEYDOWN_6:
            case MSG_KEYDOWN_7:
            case MSG_KEYDOWN_8:
            case MSG_KEYDOWN_9:			
                SCI_MEMSET(s_op_buf[CALC_DIGIT1], 0, CALC_MAX_LENGTH+2);
                s_op_buf[CALC_DIGIT1][0] = '0';
                s_mode = CALC_MODE_DIGIT1;
                ProcessKeyPressed(msg_id, s_op_buf[CALC_DIGIT1], CALC_MAX_LENGTH);
                break;

            case MSG_CTL_MIDSK:
            case MSG_KEYDOWN_WEB:   //连等与，将结果作为操作数1，重复前一次的操作            	
                if (MMIAPICOM_Stricmp((uint8*)s_op_buf[CALC_DIGIT2], "-") == 0) /*lint !e64*///由于上面MSG_KEYDOWN_DOWN的操作产生了该情况的可能性，不知有何用
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                }      
                else 
                {
                    recode = FALSE;
                    //calc_result = (CALC_RESULT_T)ExecuteFormula(win_id);
                    calc_result =  CalculateTwo(s_op_buf[CALC_DIGIT1], s_op_buf[CALC_DIGIT1], 
                    s_op_buf[CALC_DIGIT2] , s_op_buf[CALC_OPER1][0]);

                    switch (calc_result)
                    {
                        case CALC_NO_ERROR:
                            s_mode = CALC_MODE_RESULT_ERROR;//注意，此时结果在第一行显示
                            break;

                        case CALC_OVERFLOW:
                        case CALC_DEVIDED_BY_ZERO:
                            s_mode = CALC_MODE_DIGIT1;
                            break;

                        default:
                            s_mode = CALC_MODE_DIGIT2;
                            break;
                    }
                }
                break;		

            default:
                recode = FALSE;
                break;
        }
    }

    s_math_error_info = calc_result ;
    if (CALC_NO_ERROR == s_math_error_info)
    {			
        DisplayCalcFormulaEx(win_id);
    }
    else
    {
        ShowCalcError(s_math_error_info,win_id);
    }
		
    if( CALC_NO_ERROR == s_math_error_info)
    {
        if ((MSG_KEYDOWN < msg_id) && (MSG_KEYDOWN_DEL >= msg_id)||(MSG_KEYLONG_CANCEL == msg_id)||(MSG_KEYLONG_DEL == msg_id))
        {
            if (isSoftkeyShowReturn())
            {
#ifdef GUI_MID_SOFTKEY_USE_IMG              
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL,STXT_RETURN,TRUE);
                GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_EQUAL_ICON,1,TRUE);
#else
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, STXT_EQUAL,STXT_RETURN,TRUE);
#endif
            }       
            else
            {
#ifdef GUI_MID_SOFTKEY_USE_IMG
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL,STXT_RETURNAWORD,TRUE);
                GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_EQUAL_ICON,1,TRUE);
#else
                GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, STXT_EQUAL,STXT_RETURNAWORD,TRUE);
#endif
            }
        }
    }
    else
    {
#ifdef GUI_MID_SOFTKEY_USE_IMG
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL,STXT_RETURNAWORD,TRUE);
        GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_EQUAL_ICON,1,TRUE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, STXT_EQUAL,STXT_RETURNAWORD,TRUE);
#endif
    }
		 
    return recode;
}/* void FormatFormula(uint8 i1_nbr_params) */

/*****************************************************************************/
// 	Description : if right softkey should show "return" or "returnaword"
//	Global resource dependence : 
//  Author:   hui.zhao
//	Note:   
/*****************************************************************************/
BOOLEAN isSoftkeyShowReturn(void)
{
    BOOLEAN recode = FALSE;

    if( (CALC_MODE_DIGIT1 ==  s_mode ) 
    && ( (s_op_buf[CALC_DIGIT1][0] == 0) || (strcmp((char*)s_op_buf[CALC_DIGIT1],"0") ==0 )))
    {
        recode = TRUE;
    }

    return recode;
}

/***********************************************************************
		FUNCTION-DESCRIPTION
**
** LOCAL-FUNCTION-NAME:               InitCalc
**
** DESCRIPTION:
**	 This function initialises the calculator. Memory allocation
** 	is done here, too. The state is set to PMI_CALCULATOR.
**
** PARAMETERS:   None
**
** RETURN VALUES:       None
**
************************************************************************/
LOCAL void InitCalc(MMI_WIN_ID_T win_id)
{
    InitCalcExceptMemory(win_id);
    InitCalcMemory(win_id);	
} /* end InitCalc*/

LOCAL void InitCalcExceptMemory(MMI_WIN_ID_T win_id)
{
    SCI_MEMSET(s_op_buf[CALC_DIGIT1], 0, (CALC_MAX_LENGTH+2)); 
    SCI_MEMSET(s_op_buf[CALC_OPER1], 0, (CALC_MAX_LENGTH+2)); 
    SCI_MEMSET(s_op_buf[CALC_DIGIT2], 0, (CALC_MAX_LENGTH+2)); 
    SCI_MEMSET(s_op_buf[CALC_OPER2], 0, (CALC_MAX_LENGTH+2)); 
    SCI_MEMSET(s_op_buf[CALC_RESULT], 0, (CALC_MAX_LENGTH+2)); 	
    s_mode = CALC_MODE_DIGIT1;
    s_op_buf[CALC_DIGIT1][0] = '0';
    s_op_buf[CALC_DIGIT2][0] = '0';   
    s_math_error_info = CALC_NO_ERROR;	
} /* end InitCalc*/

LOCAL void InitCalcMemory(MMI_WIN_ID_T win_id)
{
    SCI_MEMSET(s_op_m_buf, 0, (CALC_MAX_LENGTH+2)); 
    memory_status = MEMORY_CLEAR ;
}

/*****************************************************************************/
// 	Description :execute memory operations with the result as the current mode is 
//				 CALC_MODE_DIGIT2 or CALC_MODE_RESULT
//	Global resource dependence : 
//  Author: chunyang.wang
//	Note:   
/*****************************************************************************/
LOCAL uint16 CalcSetMemoryWithResult(MMI_CTRL_ID_T ctr_id)
{
    uint16 math_error            = CALC_NO_ERROR;
    uint8	temp_result[CALC_MAX_LENGTH+2]	=	{0};	  

    math_error = CalculateTwo(temp_result, s_op_buf[CALC_DIGIT1], 
    s_op_buf[CALC_DIGIT2] , s_op_buf[CALC_OPER1][0]);

    if(CALC_NO_ERROR == math_error)
    {
        if (MMIACC_CALC_MPLUS_CTRL_ID == ctr_id)
        {
            math_error = CalculateTwo(s_op_m_buf, s_op_m_buf,  temp_result , '+');
        }
        else if (MMIACC_CALC_MMINUS_CTRL_ID == ctr_id)
        {
            math_error = CalculateTwo(s_op_m_buf, s_op_m_buf ,  temp_result , '-');
        }
    }

    return math_error;	
}

/*****************************************************************************/
// 	Description :execute memory operations,such as clear,display,add,subtract.
//	Global resource dependence : 
//  Author: chunyang.wang
//	Note:   
/*****************************************************************************/
LOCAL void CalcExecuteMemoryOperations(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctr_id)
{    
    CALC_RESULT_T error = CALC_NO_ERROR ;

    switch(ctr_id)
    {
        case MMIACC_CALC_MCLEAR_CTRL_ID:
            //clear memory data
            SCI_MEMSET(s_op_m_buf,0,sizeof(s_op_m_buf));         
            memory_status = MEMORY_CLEAR ;	     
            break;

        case MMIACC_CALC_MRECALL_CTRL_ID:            
            if(0 == strlen((char *)s_op_m_buf))  return ;
            //s_mode = CALC_MEMORY_DISPLAY;//以实现后续的操作    
            memory_status = MEMORY_RECALL ;

            if ((s_mode == CALC_MODE_DIGIT1)   || (s_mode == CALC_MODE_RESULT) || (s_mode == CALC_MODE_RESULT_ERROR))
            {
                s_mode = CALC_MODE_DIGIT1;
                SCI_MEMSET(s_op_buf[CALC_DIGIT1],0,sizeof(s_op_m_buf));   
                SCI_MEMCPY(s_op_buf[CALC_DIGIT1],s_op_m_buf,sizeof(s_op_buf[CALC_DIGIT1]));                         
            }        
            else if((s_mode == CALC_MODE_DIGIT2) || (s_mode == CALC_MODE_OPER))
            {
                s_mode = CALC_MODE_DIGIT2;
                SCI_MEMSET(s_op_buf[CALC_DIGIT2],0,sizeof(s_op_buf[CALC_DIGIT2]));             
                SCI_MEMCPY(s_op_buf[CALC_DIGIT2],s_op_m_buf,sizeof(s_op_buf[CALC_DIGIT2]));
            }      		
            break;

        case MMIACC_CALC_MPLUS_CTRL_ID:
        case MMIACC_CALC_MMINUS_CTRL_ID:
        {  
            if (strlen((char*)s_op_m_buf)==0)//init with '0'
            {
                s_op_m_buf[0] = '0';
            }

            switch(s_mode)
            {            
                case CALC_MODE_DIGIT1:
                case CALC_MODE_RESULT_ERROR:			
                //case CALC_MODE_OPER://operate with the first operand
                {            
                    if (MMIACC_CALC_MPLUS_CTRL_ID == ctr_id)
                    {
                        error = CalculateTwo(s_op_m_buf, s_op_m_buf,  s_op_buf[CALC_DIGIT1] , '+');
                    }
                    else if (MMIACC_CALC_MMINUS_CTRL_ID == ctr_id)
                    {
                        error = CalculateTwo(s_op_m_buf, s_op_m_buf ,  s_op_buf[CALC_DIGIT1] , '-');
                    }
                    break;
                }

                case CALC_MODE_DIGIT2://calculate and operate with the result       
                {           
                    MMIAPICOM_CheckDecimalPoint(s_op_buf[CALC_DIGIT2]);//check if digit2 is "xx."
                    if (MMIAPICOM_Stricmp(s_op_buf[CALC_DIGIT2], (uint8*)"-") == 0)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_DEVIDEND_INVALID);
                    }      
                    else 
                    {
                        error = (CALC_RESULT_T)CalcSetMemoryWithResult(ctr_id);	                
                    }
                    break;
                }
                
                case CALC_MODE_RESULT:
                    if (MMIACC_CALC_MPLUS_CTRL_ID == ctr_id)
                    {
                        error =  CalculateTwo(s_op_m_buf, s_op_m_buf,  s_op_buf[CALC_RESULT] , '+');
                    }
                    else if (MMIACC_CALC_MMINUS_CTRL_ID == ctr_id)
                    {
                        error = CalculateTwo(s_op_m_buf, s_op_m_buf ,  s_op_buf[CALC_RESULT] , '-');
                    }			
                    break;

                default:
                    break;
            }        

            break;
        }
        
        default:
        break;
    }
    
    if(	CALC_NO_ERROR == error)
    {
        memory_status = MEMORY_SET;
        DisplayCalcFormulaEx(win_id);      	
    }	 
    else
    {
        s_math_error_info = error ;
        ShowCalcError(s_math_error_info, win_id);
    }
    UpdateSoftkey(win_id, 0);	
}


/*****************************************************************************/
// 	Description : when pen ok, handle the message
//	Global resource dependence : 
//  Author:   xiaoqing.lu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePenOk(
							   MMI_WIN_ID_T win_id,
							   DPARAM param
							   )
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_MESSAGE_ID_E msg_id = 0;
    MMI_CTRL_ID_T ctrl_id = 0;  
    BOOLEAN isHandle             = FALSE;

    ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
	
    switch(ctrl_id) 
    {
        case MMIACC_CALC_MCLEAR_CTRL_ID:
        case MMIACC_CALC_MRECALL_CTRL_ID:
        case MMIACC_CALC_MPLUS_CTRL_ID:
        case MMIACC_CALC_MMINUS_CTRL_ID:
            isHandle = TRUE;
            CalcExecuteMemoryOperations(win_id, ctrl_id);
            break;
            
        case MMIACC_CALC_PLUS_CTRL_ID:
            msg_id = MSG_KEYDOWN_UP;
            break;

        case MMIACC_CALC_SUB_CTRL_ID:
            msg_id = MSG_KEYDOWN_DOWN;
            break;

        case MMIACC_CALC_MUL_CTRL_ID:
            msg_id = MSG_KEYDOWN_LEFT;
            break;

        case MMIACC_CALC_DIV_CTRL_ID:
            msg_id = MSG_KEYDOWN_RIGHT;
            break;

        case MMIACC_CALC_BACKSPACE_CTRL_ID:
            msg_id = MSG_KEYDOWN_CANCEL;//cr132551
            break;

        case MMIACC_CALC_EQUAL_CTRL_ID:
            msg_id = MSG_CTL_MIDSK;
            break;
            
        case MMIACC_CALC_0_CTRL_ID:
            msg_id=MSG_KEYDOWN_0;
            break;
            
        case MMIACC_CALC_1_CTRL_ID:
            msg_id=MSG_KEYDOWN_1;
            break;
            
        case MMIACC_CALC_2_CTRL_ID:
            msg_id=MSG_KEYDOWN_2;
            break;
            
        case MMIACC_CALC_3_CTRL_ID:
            msg_id=MSG_KEYDOWN_3;
            break;
            
        case MMIACC_CALC_4_CTRL_ID:
            msg_id=MSG_KEYDOWN_4;
            break;
            
        case MMIACC_CALC_5_CTRL_ID:
            msg_id=MSG_KEYDOWN_5;
            break;
            
        case MMIACC_CALC_6_CTRL_ID:
            msg_id=MSG_KEYDOWN_6;
            break;
            
        case MMIACC_CALC_7_CTRL_ID:
            msg_id=MSG_KEYDOWN_7;
            break;
            
        case MMIACC_CALC_8_CTRL_ID:
            msg_id=MSG_KEYDOWN_8;
            break;
            
        case MMIACC_CALC_9_CTRL_ID:
            msg_id=MSG_KEYDOWN_9;
            break;
            
        case MMIACC_CALC_STAR_CTRL_ID:
            msg_id = MSG_KEYDOWN_STAR;
            break;
        
        case MMIACC_CALC_HASH_CTRL_ID:
            msg_id = MSG_KEYDOWN_HASH;
            break;
            
        default:
            break;
    }
	
    MMK_WinInactiveCtrl(win_id,FALSE);
    if (isHandle == FALSE && !FormatFormula(win_id, msg_id))
    {
        recode = MMI_RESULT_FALSE;
    }      
  
    return recode;
}

/*****************************************************************************/
// 	Description :the message handler of pop window 
//	Global resource dependence : 
//  Author:chunyang.wang
//	Note:   
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleCalcPopWindMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    uint16 item_id      = 0;   
    MMI_CTRL_ID_T   ctrl_id = MMI_CALC_POPMENU_CTRL_ID;

    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};  
    MMI_NOTIFY_T        notify = {0};
#ifdef MMI_GUI_STYLE_MINISCREEN
    MMI_STRING_T                    str_menutitle  ={0};
#endif
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_CreatDynamic(PNULL,win_id, 
            ctrl_id, GUIMENU_STYLE_POPUP);

            MMI_GetLabelTextByLang(TXT_CALC_MPLUS, &kstring);
            node_item.item_text_ptr = &kstring;
#ifdef MMI_PDA_SUPPORT       
            node_item.select_icon_id = IMAGE_CALC_MENU_ICON_MPLUS;
#endif
            //GUIMENU_InsertNode(0,ID_ACC_CALC_MENU_MPLUS,0,&node_item, ctrl_id  );//delete by James li        
            MMI_GetLabelTextByLang(TXT_CALC_MSUB, &kstring);
            node_item.item_text_ptr = &kstring;
#ifdef MMI_PDA_SUPPORT       
            node_item.select_icon_id = IMAGE_CALC_MENU_ICON_MMINUS;
#endif
            //GUIMENU_InsertNode(1,ID_ACC_CALC_MENU_MMINUS,0,&node_item, ctrl_id  );//delete by James li    
            MMI_GetLabelTextByLang(TXT_CALC_MR, &kstring);
            node_item.item_text_ptr = &kstring;
#ifdef MMI_PDA_SUPPORT       
            node_item.select_icon_id = IMAGE_CALC_MENU_ICON_MR;
#endif
            //GUIMENU_InsertNode(2,ID_ACC_CALC_MENU_MRECALL,0,&node_item, ctrl_id  );//delete by James li    
            MMI_GetLabelTextByLang(TXT_CALC_MCLEAR, &kstring);
            node_item.item_text_ptr = &kstring;
#ifdef MMI_PDA_SUPPORT       
            node_item.select_icon_id = IMAGE_CALC_MENU_ICON_MC;
#endif
            //GUIMENU_InsertNode(3,ID_ACC_CALC_MENU_MCLEAR,0,&node_item, ctrl_id  );//delete by James li            
            MMI_GetLabelTextByLang(TXT_COMMON_CLEAR, &kstring);
#ifndef MMI_PDA_SUPPORT       
            node_item.item_text_ptr = &kstring;	
            //GUIMENU_InsertNode(4,ID_ACC_CALC_MENU_CLEAR,0,&node_item,  ctrl_id  );  
            GUIMENU_InsertNode(0,ID_ACC_CALC_MENU_CLEAR,0,&node_item,  ctrl_id  ); //change by James li
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
			MMI_GetLabelTextByLang(TXT_TOOLS_CALCULATOR,&str_menutitle);
			GUIMENU_SetMenuTitle(&str_menutitle,ctrl_id);
#endif
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            break;

        case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:   
            item_id = GUIMENU_GetCurNodeId(ctrl_id);         
            switch(item_id)
            {
                case ACC_CALC_MENU_MPLUS:  
                    notify.src_id = MMIACC_CALC_MPLUS_CTRL_ID;    
                    break;
                case ACC_CALC_MENU_MSUB:
                    notify.src_id  = MMIACC_CALC_MMINUS_CTRL_ID;
                    break;
                case ACC_CALC_MENU_MR:
                    notify.src_id  = MMIACC_CALC_MRECALL_CTRL_ID;  
                    break;
                case ACC_CALC_MENU_MCLEAR:
                    notify.src_id  = MMIACC_CALC_MCLEAR_CTRL_ID; 
                    break;
                case ACC_CALC_MENU_CLEAR:
                    notify.src_id  = MMIACC_CALC_CLEAR_CTRL_ID; 
                    break;
                default:
                    notify.src_id = MMIACC_CALC_CLEAR_CTRL_ID;
                    return recode;
            }       
            //MMK_PostMsg( MMIACC_CALC_WIN_ID,  MSG_CTL_PENOK, (DPARAM)&notify,  sizeof(MMI_NOTIFY_T));
            HandleCalcWinMsg(MMIACC_CALC_WIN_ID, MSG_CTL_PENOK, (DPARAM)&notify);
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }    
    return recode;
}

/*****************************************************************************/
// 	Description : Display the content at each area
//	Global resource dependence : 
//  Author:chunyang.wang
//	Note:   
/*****************************************************************************/
LOCAL void CalcRefreshContent(uint32 dis_type)
{
    GUI_RECT_T			     rect = {0};
    uint8				    ch[2] = {0,0};    
    uint8				    chs[5] = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char *         custom_string = PNULL;    
    wchar *         custom_string_w = PNULL;
    uint16          custom_length = 0;
    uint16        custom_length_w = 0;
    uint16              font_color = CALC_TEXT_COLOR;
    GUI_ALIGN_E        align_type = ALIGN_RVMIDDLE;
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK;  
#else
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;  
#endif
    MMI_STRING_T        str_data = {0};
    uint16 M_height = CALC_LINE_M_ENDY - CALC_LINE_M_STARTY + 1;

    if(2 == s_CALC_LINE_NUM)
    {
        //双行显示
        if(dis_type & ACC_CALC_DISPLAY_M_SYMBOL)//display the 'M' symbol
        {
            if(strlen((char*)s_op_m_buf) != 0 )
            {            
                if ( (memory_status == MEMORY_SET) ||(memory_status == MEMORY_RECALL) 
                || (memory_status == MEMORY_STORE))
                {
                    align_type   = ALIGN_LVMIDDLE;


#ifdef MMI_CALCULATOR_MINI_SUPPORT
					rect.left    = CALC_LINE_M_STARTX;
                    rect.top     = s_CALC_LINE_START_Y;  /*lint !e778*/
                    rect.right   = CALC_LINE_M_ENDX;
                    rect.bottom  = s_CALC_LINE_END_Y;
				    chs[0] = 'M';
                    chs[1] = ':';
                    custom_length   = SCI_STRLEN((char*)chs) + SCI_STRLEN((char*)s_op_m_buf);
                    custom_length_w = (custom_length +1 );                    
                    custom_string   = (char *)SCI_ALLOCA(custom_length_w*sizeof(char));
                    if(PNULL != custom_string)
                    {
                        SCI_MEMSET(custom_string,0,(custom_length_w*sizeof(char)));
                        strncat(custom_string, (char *)chs, SCI_STRLEN((char*)chs));
                        strncat(custom_string, (char*)s_op_m_buf, SCI_STRLEN((char*)s_op_m_buf));                        
                    }                    
                    custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
                    if(PNULL != custom_string_w && PNULL != custom_string)
                    {
                        SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                        MMI_STRNTOWSTR(custom_string_w,custom_length_w, (uint8*)custom_string ,custom_length,custom_length);
                    }
#else
                    rect.left    = CALC_LINE_OP_STARTX;
                    rect.top     = CALC_LINE_OP_STARTY(1);  /*lint !e778*/
                    rect.right   = CALC_LINE_OP_ENDX;
                    rect.bottom  = CALC_LINE_OP_ENDY(1);

                    ch[0] = 'M';
                    custom_length   = SCI_STRLEN((char*)ch);
                    custom_length_w = (custom_length +1 );
                    custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
                    if(PNULL != custom_string_w)
                    {
                        SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                        MMI_STRNTOWSTR(custom_string_w,custom_length_w, ch,custom_length,custom_length);
                    }
#endif
                }

                // style
#ifdef MMI_CALCULATOR_MINI_SUPPORT
				SetCalcTextStyle(&text_style, align_type, SONG_FONT_12, font_color);//fix bug642342 SONG_FONT_14->SONG_FONT_12
#else
                SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
#endif
                // text
                str_data.wstr_len = custom_length_w;
                str_data.wstr_ptr = custom_string_w;
                //  adjust font size
#ifndef MMI_CALCULATOR_MINI_SUPPORT
                while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
                {
 					if (text_style.font>FONT_STEP)
 					{
                    text_style.font-= FONT_STEP;      	
 					}
					else
						break;
                } 
#endif 
                // display
                GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    (const GUI_RECT_T      *)&rect,       //the fixed display area
                    (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                    (const MMI_STRING_T    *)&str_data,
                    &text_style,
                    state,
                    GUISTR_TEXT_DIR_AUTO
                    );

				if (custom_string != NULL)/*lint !e774*/
				{
					SCI_FREE(custom_string);
					custom_string = PNULL;
				}	
                SCI_FREE(custom_string_w);
                custom_string_w = PNULL;
            }
        }
        
        if(dis_type & ACC_CALC_DISPLAY_FIRST_OPERAND)//draw the first operand 
        {
            align_type = ALIGN_RVMIDDLE;
            
            rect.left    = CALC_LINE_NUM_STARTX;
            rect.top     = CALC_LINE_NUM_STARTY(1); /*lint !e778*/
            rect.right   = CALC_LINE_NUM_ENDX;
            rect.bottom  = CALC_LINE_NUM_ENDY(1);

            custom_length   = SCI_STRLEN((char*)s_op_buf[CALC_DIGIT1]);
            custom_length_w = (custom_length +1 );
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            if(PNULL != custom_string_w)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w, s_op_buf[CALC_DIGIT1],custom_length,custom_length);
            }
            
            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
            //  adjust font size
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
                if (text_style.font>FONT_STEP)
				{
                text_style.font-= FONT_STEP;    	
				}
				else
					break;
            } 
#endif
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );

            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }
        
        if(dis_type &  ACC_CALC_DISPLAY_OPERATOR)
        {
            if(s_op_buf[CALC_OPER1][0] == 215 /* '*' */)
            {
                ch[0] = 215 /* '*' */;
            }
            else
            {
                ch[0] =  s_op_buf[CALC_OPER1][0];
            }

            align_type = ALIGN_LVMIDDLE; 
            
            rect.left    = CALC_LINE_OP_STARTX;
            rect.top     = CALC_LINE_OP_STARTY(2);
            rect.right   = CALC_LINE_OP_ENDX;
            rect.bottom  = CALC_LINE_OP_ENDY(2);
                   
            custom_length   = SCI_STRLEN((char*)ch);
            custom_length_w = (custom_length +1 );
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            if(PNULL != custom_string_w)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w,(uint8*)ch,custom_length,custom_length);
            }

            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
            //  adjust font size
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
                if (text_style.font>FONT_STEP)
				{
                text_style.font-= FONT_STEP;      
				}
				else
					break;
            } 
#endif
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );

            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }

        if(dis_type &  ACC_CALC_DISPLAY_SECOND_OPERAND)
        {
            align_type = ALIGN_RVMIDDLE;

            rect.left    = CALC_LINE_NUM_STARTX;	
            rect.top     = CALC_LINE_NUM_STARTY(2);	
            rect.right   = CALC_LINE_NUM_ENDX;	
            rect.bottom  = CALC_LINE_NUM_ENDY(2);	

            custom_length   = SCI_STRLEN((char*)s_op_buf[CALC_DIGIT2]);
            custom_length_w = (custom_length +1 );
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            if(PNULL != custom_string_w)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w,(uint8*)s_op_buf[CALC_DIGIT2],custom_length,custom_length);
            }

            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
            //  adjust font size
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
                if (text_style.font>FONT_STEP)
				{
                text_style.font-= FONT_STEP;      
				}
				else
					break;
            } 
#endif
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }
        
        if(dis_type &  ACC_CALC_DISPLAY_EQUAL_SYMBOL)//display the "=" symbol
        {
            align_type   = ALIGN_LVMIDDLE;
            
            rect.left    = CALC_LINE_OP_STARTX;
            rect.top     = CALC_LINE_OP_STARTY(2);
            rect.right   = CALC_LINE_OP_ENDX;
            rect.bottom  = CALC_LINE_OP_ENDY(2);

            ch[0] = '=';
            
            custom_length   = SCI_STRLEN((char*)ch);
            custom_length_w = (custom_length +1 );
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            if(PNULL != custom_string_w)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w, ch,custom_length,custom_length);
            }

            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
            //  adjust font size
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
                if (text_style.font>FONT_STEP)
				{
                text_style.font-= FONT_STEP;  
				}
				else
					break;
            } 
#endif
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }      
        
        if(dis_type &  ACC_CALC_DISPLAY_RESULT)
        {
            //line 2,the result
            align_type = ALIGN_RVMIDDLE;

            rect.left    = CALC_LINE_NUM_STARTX;	
            rect.top     = CALC_LINE_NUM_STARTY(2);
            rect.right   = CALC_LINE_NUM_ENDX;
            rect.bottom  = CALC_LINE_NUM_ENDY(2);

            custom_length   = SCI_STRLEN((char*)s_op_buf[CALC_RESULT]);
            custom_length_w = (custom_length +1 );
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            if(PNULL != custom_string_w)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w,(uint8*)s_op_buf[CALC_RESULT],custom_length,custom_length);
            }

            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
            //  adjust font size
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
                if (text_style.font>FONT_STEP)
				{
                text_style.font-= FONT_STEP;  
				}
				else
					break;
            } 
#endif
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }
        
        if(dis_type &  ACC_CALC_DISPLAY_MEMORY_DATA)//display the memory data     
        {
            align_type = ALIGN_RVMIDDLE;

            rect.left    = CALC_LINE_NUM_STARTX;	
            rect.top     = CALC_LINE_NUM_STARTY(2);
            rect.right   = CALC_LINE_NUM_ENDX;
            rect.bottom  = CALC_LINE_NUM_ENDY(2);

            custom_length   = SCI_STRLEN((char*)s_op_m_buf);
            custom_length_w = (custom_length + 1);
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            //SCI_ASSERT(PNULL != custom_string_w);
            if(PNULL != custom_string_w)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w,(uint8*)s_op_m_buf,custom_length,custom_length);
            }

            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
#if defined (MAINLCD_SIZE_128X160)&&defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
            //  adjust font size
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
                if (text_style.font>FONT_STEP)
				{
                text_style.font-= FONT_STEP;      
				}
				else
					break;
            } 
#endif
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }
    }
    else if(1 == s_CALC_LINE_NUM)
    {
        //单行显示
        if(dis_type & ACC_CALC_DISPLAY_M_SYMBOL)//display the 'M' symbol
        {
            if(strlen((char*)s_op_m_buf) != 0 )
            {            
                if ( (memory_status == MEMORY_SET) ||(memory_status == MEMORY_RECALL) 
                || (memory_status == MEMORY_STORE))
                {
                    align_type   = ALIGN_LVMIDDLE;

                    rect.left    = CALC_LINE_M_STARTX;
                    rect.top     = CALC_LINE_M_STARTY;  /*lint !e778*/
                    rect.right   = CALC_LINE_M_ENDX;
                    rect.bottom  = CALC_LINE_M_ENDY;

                    chs[0] = 'M';
                    chs[1] = ':';
                    custom_length   = SCI_STRLEN((char*)chs) + SCI_STRLEN((char*)s_op_m_buf);
                    custom_length_w = (custom_length +1 );                    
                    custom_string   = (char *)SCI_ALLOCA(custom_length_w*sizeof(char));
                    if(PNULL != custom_string)
                    {
                        SCI_MEMSET(custom_string,0,(custom_length_w*sizeof(char)));
                        strncat(custom_string, (char *)chs, SCI_STRLEN((char*)chs));
                        strncat(custom_string, (char*)s_op_m_buf, SCI_STRLEN((char*)s_op_m_buf));                        
                    }                    
                    custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
                    if(PNULL != custom_string_w && PNULL != custom_string)
                    {
                        SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                        MMI_STRNTOWSTR(custom_string_w,custom_length_w, (uint8*)custom_string ,custom_length,custom_length);
                    }
                }

                // style
                SetCalcTextStyle(&text_style, align_type, M_height-1, font_color);
                // text
                str_data.wstr_len = custom_length_w;
                str_data.wstr_ptr = custom_string_w;
                //  adjust font size
                while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
                {
                    if (text_style.font>FONT_STEP)
					{
                    text_style.font-= FONT_STEP;      	
					}
					else
						break;
                } 
                // display
                GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    (const GUI_RECT_T      *)&rect,       //the fixed display area
                    (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                    (const MMI_STRING_T    *)&str_data,
                    &text_style,
                    state,
                    GUISTR_TEXT_DIR_AUTO
                    );
                
                SCI_FREE(custom_string);
                custom_string = PNULL;
                SCI_FREE(custom_string_w);
                custom_string_w = PNULL;
            }
        }

        custom_length = 0;
        
        if(dis_type & ACC_CALC_DISPLAY_FIRST_OPERAND)//draw the first operand 
        {
            custom_length   += SCI_STRLEN((char*)s_op_buf[CALC_DIGIT1]);
        }
        
        if(dis_type &  ACC_CALC_DISPLAY_OPERATOR)
        {
            if(s_op_buf[CALC_OPER1][0] == 215 /* '*' */)
            {
                ch[0] = 215 /* '*' */;
            }
            else
            {
                ch[0] =  s_op_buf[CALC_OPER1][0];
            }
                   
            custom_length   += SCI_STRLEN((char*)ch);
        }    

        if(dis_type &  ACC_CALC_DISPLAY_SECOND_OPERAND)
        {
            custom_length   += SCI_STRLEN((char*)s_op_buf[CALC_DIGIT2]);
        }

        if(0 < custom_length)
        {
            align_type = ALIGN_RVMIDDLE;

            rect.left    = s_CALC_LINE_START_X;
            rect.top     = CALC_LINE_NUM_STARTY(1);  /*lint !e778*/
            rect.right   = CALC_LINE_NUM_ENDX;
            rect.bottom  = CALC_LINE_NUM_ENDY(1);
            
            custom_length_w = (custom_length +1 );            
            custom_string   = (char *)SCI_ALLOCA(custom_length_w*sizeof(char));
            if(PNULL != custom_string)
            {
                SCI_MEMSET(custom_string,0,(custom_length_w*sizeof(char)));
                if(dis_type & ACC_CALC_DISPLAY_FIRST_OPERAND)//draw the first operand 
                {
                    strncat(custom_string, (char *)s_op_buf[CALC_DIGIT1], SCI_STRLEN((char*)s_op_buf[CALC_DIGIT1]));
                }
                if(dis_type &  ACC_CALC_DISPLAY_OPERATOR)
                {
                    if(s_op_buf[CALC_OPER1][0] == 215 /* '*' */)
                    {
                        ch[0] = 215 /* '*' */;
                    }
                    else
                    {
                        ch[0] =  s_op_buf[CALC_OPER1][0];
                    }
                    strncat(custom_string, (char*)ch, SCI_STRLEN((char*)ch));                    
                }
                if(dis_type &  ACC_CALC_DISPLAY_SECOND_OPERAND)
                {
                    strncat(custom_string, (char *)s_op_buf[CALC_DIGIT2], SCI_STRLEN((char*)s_op_buf[CALC_DIGIT2]));
                }
            }            
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            if(PNULL != custom_string_w && PNULL != custom_string)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w, (uint8*)custom_string ,custom_length,custom_length);
            }
            
            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
            //  adjust font size
			// 只需要比较输入数字的宽度与客户区域的宽度，超过客户区域宽度时，逐渐调整字体大小,且不需要对richtext显示区域的top进行调整，否则将出现字符向下跳动的现象 @ Bug140600
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
#if 0
                if(GUI_GetStringHeight(text_style.font, str_data.wstr_ptr,str_data.wstr_len) < (rect.bottom- rect.top - M_height)/2-1)
                {
                    //字体高度小于区域一半的高度时，分行显示
                    state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_NO_HALF_LINE;  
                    while(2 < GUI_CalculateStringLinesByPixelNum(rect.right - rect.left, str_data.wstr_ptr, str_data.wstr_len, text_style.font, text_style.word_space, TRUE))
                    {                        
						if (text_style.font>1)
						{
                        text_style.font--;   
						}
						else
							break;
                    }
                    rect.top += M_height;
                    break;
                }  
#endif
                if (text_style.font>FONT_STEP)
                {
                text_style.font-= FONT_STEP;   
                }
				else
					break;
            } 
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );

            SCI_FREE(custom_string);
            custom_string = PNULL;
            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }

        state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;  

        if(dis_type &  ACC_CALC_DISPLAY_EQUAL_SYMBOL)//display the "=" symbol
        {
            align_type   = ALIGN_LVMIDDLE;
            
            rect.left    = CALC_LINE_OP_STARTX;
            rect.top     = CALC_LINE_OP_STARTY(1);/*lint !e778*/
            rect.right   = CALC_LINE_OP_ENDX;
            rect.bottom  = CALC_LINE_OP_ENDY(1);

            ch[0] = '=';
            custom_length   = SCI_STRLEN((char*)ch);
            custom_length_w = (custom_length +1 );
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            if(PNULL != custom_string_w)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w, ch,custom_length,custom_length);
            }

            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
            //  adjust font size
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
                if (text_style.font>FONT_STEP)
				{
                text_style.font-= FONT_STEP;  
				}
				else
					break;
            } 
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }
        
        if(dis_type &  ACC_CALC_DISPLAY_RESULT)
        {
            //line 2,the result
            align_type = ALIGN_RVMIDDLE;

            rect.left    = CALC_LINE_NUM_STARTX;	
            rect.top     = CALC_LINE_NUM_STARTY(1);/*lint !e778*/
            rect.right   = CALC_LINE_NUM_ENDX;
            rect.bottom  = CALC_LINE_NUM_ENDY(1);

            custom_length   = SCI_STRLEN((char*)s_op_buf[CALC_RESULT]);
            custom_length_w = (custom_length +1 );
            custom_string_w   = (wchar *)SCI_ALLOCA(custom_length_w*sizeof(wchar));
            if(PNULL != custom_string_w)
            {
                SCI_MEMSET(custom_string_w,0,(custom_length_w*sizeof(wchar)));
                MMI_STRNTOWSTR(custom_string_w,custom_length_w,(uint8*)s_op_buf[CALC_RESULT],custom_length,custom_length);
            }

            // style
            SetCalcTextStyle(&text_style, align_type, s_CALC_FONT_SIZE, font_color);
            // text
            str_data.wstr_len = custom_length_w;
            str_data.wstr_ptr = custom_string_w;
            //  adjust font size
            while(GUI_GetStringWidth(text_style.font, str_data.wstr_ptr,str_data.wstr_len) > (rect.right - rect.left))
            {
                if (text_style.font>FONT_STEP)
				{
                text_style.font-= FONT_STEP;  
				}
				else
					break;
            } 
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&str_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            SCI_FREE(custom_string_w);
            custom_string_w = PNULL;
        }    
    }
}

/*****************************************************************************/
// 	Description : convert qwertykey to typicalkey
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL MMI_MESSAGE_ID_E ConvertKey(MMI_MESSAGE_ID_E msg_id)
{
    if (MMK_IsKeyMsg( msg_id))
    {
        uint8 key_code = msg_id & 0xff;

        key_code = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(key_code);

        msg_id = (msg_id & 0xff00) | key_code;
    }

    return msg_id;
}

/*****************************************************************************/
// 	Description : the process message function of the CALCULATE
//	Global resource dependence : 
//  Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalcWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
	//static     uint8        focusflag  = 0;//cr132551

    msg_id = ConvertKey(msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            GUIWIN_CreateTitleDyna(win_id, TXT_TOOLS_CALCULATOR);            
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif
#ifdef MMI_CALCULATOR_MINI_SUPPORT
        GUIWIN_CreateTitleDyna(win_id, TXT_TOOLS_CALCULATOR);
#endif
        GUIWIN_CreateTitleDyna(win_id, TXT_TOOLS_CALCULATOR);//add by James li

        LcdSwitch(win_id);
        InitFormWin();
        InitCalc(win_id);    

        //set form not support slide
        GUIFORM_PermitChildBg(MMI_CALC_FORMCTRL_ID,FALSE);    
        GUIFORM_IsSlide(MMI_CALC_FORMCTRL_ID,FALSE);

        GUIBUTTON_SetHandleLong(MMIACC_CALC_BACKSPACE_CTRL_ID,TRUE);       
        GUIWIN_SetSoftkeyBtnState(win_id,2,FALSE,TRUE);       
        key_press_status = 0;
        break;

    case MSG_LCD_SWITCH :
        LcdSwitch(win_id);       			   
        MMK_CreateCtrlByWinTabEx( MMIACC_CALC_WIN_ID,  
        (MMITHEME_IsMainScreenLandscape() ? (uint32 *)CALC_CTRL_TAB_H : (uint32 *)CALC_CTRL_TAB ));	

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            GUIWIN_CreateTitleDyna(win_id, TXT_TOOLS_CALCULATOR);            
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif

        InitFormWin();

        //set form not support slide
        GUIFORM_IsSlide(MMI_CALC_FORMCTRL_ID,FALSE);

        GUIBUTTON_SetHandleLong(MMIACC_CALC_BACKSPACE_CTRL_ID,TRUE);
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif        
        key_press_status = 0;
        break;

    case MSG_FULL_PAINT:
        DisplayCalcBackground(win_id);
        if (CALC_NO_ERROR == s_math_error_info)
        {			
            DisplayCalcFormulaEx(win_id);
        }
        else
        {
            ShowCalcError(s_math_error_info,win_id);
        }
        UpdateSoftkey(win_id, msg_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:         
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMIACC_CALC_POPWIN_TAB, PNULL);
        break;  
    case MSG_KEYLONG_CANCEL:
    case MSG_CTL_CANCEL:
// #ifdef TOUCH_PANEL_SUPPORT//cr132551
//     case MSG_KEYDOWN_CANCEL:
// #else
    case MSG_KEYDOWN_CANCEL:
// #endif
    case MSG_KEYDOWN_DEL:
    case MSG_KEYLONG_DEL:
#ifdef MMI_PDA_SUPPORT
        MMK_CloseWin(win_id);
#else
// #ifdef TOUCH_PANEL_SUPPORT//cr132551
// 		if (MSG_KEYDOWN_CANCEL == msg_id)
// 		{
//            msg_id = MSG_KEYUP_CANCEL;
// 		}
// #endif
// #ifndef TOUCH_PANEL_SUPPORT
// 		if (focusflag == 0)
// 		{
// #endif
        if (CALC_NO_ERROR != s_math_error_info)
        {
            InitCalc(win_id);   
            DisplayCalcFormulaEx(win_id);           
            UpdateSoftkey(win_id, msg_id);
        }
        else if (!FormatFormula(win_id, msg_id))
        {
            MMK_CloseWin(win_id);
        }
// #ifndef TOUCH_PANEL_SUPPORT//cr132551
// 		}
// 		else
//          focusflag = 0;
// #endif
 #endif
        break;

//#if defined (TOUCH_PANEL_SUPPORT) || defined (MMI_CALCULATOR_MINI_SUPPORT)//IGNORE9527
    case MSG_CTL_PENOK:
        if(((MMI_NOTIFY_T*)param)->src_id == MMIACC_CALC_CLEAR_CTRL_ID)
        {
            if (CALC_NO_ERROR == s_math_error_info)
            {
                InitCalcExceptMemory(win_id);
            }
            else
            {
                InitCalc(win_id);
            }
            DisplayCalcFormulaEx(win_id);
            UpdateSoftkey(win_id, msg_id);
        }
        else  if (CALC_NO_ERROR == s_math_error_info)
        {
            HandlePenOk(win_id, param);
        }
        else  if (CALC_NO_ERROR != s_math_error_info)
        {
            InitCalc(win_id);   
            DisplayCalcFormulaEx(win_id);           
            UpdateSoftkey(win_id, msg_id);
            HandlePenOk(win_id, param);
        }
        else
        {
            if(((MMI_NOTIFY_T*)param)->src_id == MMIACC_CALC_BACKSPACE_CTRL_ID)
            {
                InitCalc(win_id);
                DisplayCalcFormulaEx(win_id);
                UpdateSoftkey(win_id, msg_id);
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;
//#endif //TOUCH_PANEL_SUPPORT //IGNORE9527


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENLONGOK://@zhaohui,cr108731
        if (((MMI_NOTIFY_T*)param)->src_id == MMIACC_CALC_BACKSPACE_CTRL_ID)
        {
            if (CALC_NO_ERROR == s_math_error_info)
            {                
                InitCalcExceptMemory(win_id);   
            }
            else
            {
                InitCalc(win_id);   
            }
            DisplayCalcFormulaEx(win_id);           
            UpdateSoftkey(win_id, msg_id);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_1:		
    case MSG_KEYDOWN_2:		
    case MSG_KEYDOWN_3:		
    case MSG_KEYDOWN_4:		
    case MSG_KEYDOWN_5:		
    case MSG_KEYDOWN_6:		
    case MSG_KEYDOWN_7:		
    case MSG_KEYDOWN_8:		
    case MSG_KEYDOWN_9:		
    case MSG_KEYDOWN_STAR:	
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_HASH:     
    case MSG_KEYDOWN_UP:         
    case MSG_KEYDOWN_LEFT:       
    case MSG_KEYDOWN_RIGHT:      
    case MSG_KEYDOWN_DOWN:   
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_MIDDLE:
        {
#ifdef TOUCH_PANEL_SUPPORT
        UpdateButtonStatus(msg_id, TRUE);
#endif
        if(CALC_NO_ERROR == s_math_error_info)
        {
            FormatFormula(win_id, msg_id);
        }        
        else if (CALC_NO_ERROR != s_math_error_info)
        {
            InitCalc(win_id);   
            DisplayCalcFormulaEx(win_id);           
            UpdateSoftkey(win_id, msg_id);
            FormatFormula(win_id, msg_id);
        }
        key_press_status = msg_id;
        }
        break;	

    case MSG_KEYUP_1:
    case MSG_KEYPRESSUP_1:
    case MSG_KEYUP_2:
    case MSG_KEYPRESSUP_2:
    case MSG_KEYUP_3:	        
    case MSG_KEYPRESSUP_3:
    case MSG_KEYUP_4:        
    case MSG_KEYPRESSUP_4:
    case MSG_KEYUP_5:
    case MSG_KEYPRESSUP_5:
    case MSG_KEYUP_6:
    case MSG_KEYPRESSUP_6: 
    case MSG_KEYUP_7:        
    case MSG_KEYPRESSUP_7:
    case MSG_KEYUP_8:	        
    case MSG_KEYPRESSUP_8:
    case MSG_KEYUP_9:	        
    case MSG_KEYPRESSUP_9:
    case MSG_KEYUP_STAR:        
    case MSG_KEYPRESSUP_STAR:
    case MSG_KEYUP_0:
    case MSG_KEYPRESSUP_0:
    case MSG_KEYUP_HASH:    
    case MSG_KEYPRESSUP_HASH:
    case MSG_KEYUP_UP:
    case MSG_KEYPRESSUP_UP:
    case MSG_KEYUP_LEFT:      
    case MSG_KEYPRESSUP_LEFT:
    case MSG_KEYUP_RIGHT:      
    case MSG_KEYPRESSUP_RIGHT:
    case MSG_KEYUP_DOWN:
    case MSG_KEYPRESSUP_DOWN:
    case MSG_KEYUP_WEB:    
    case MSG_KEYPRESSUP_WEB: 
    case MSG_KEYUP_MIDDLE: 
    case MSG_KEYPRESSUP_MIDDLE: 
#ifdef TOUCH_PANEL_SUPPORT
        UpdateButtonStatus(msg_id, FALSE);     
        //make sure the pressed key is released
#endif
        if((key_press_status & 0x00ff) == (msg_id & 0x00ff))
        {
            key_press_status = 0;
        }
        break;

    case MSG_KEYDOWN_WEB:
        {
#ifdef TOUCH_PANEL_SUPPORT
            UpdateButtonStatus(msg_id, TRUE);
#endif
            if(CALC_NO_ERROR == s_math_error_info)
            {
                FormatFormula(win_id, msg_id);
            }
#ifdef TOUCH_PANEL_SUPPORT
            UpdateButtonStatus(msg_id, FALSE);
#endif
        }
        break;
// 	case MSG_LOSE_FOCUS://cr132551
//         focusflag = 1;
//         break;

    case MSG_GET_FOCUS:
        key_press_status = 0;
        break;
        
    default:
        recode = MMI_RESULT_FALSE; 
        break;
    }
    return recode;
}



/*****************************************************************************/
// 	Description : DigitLineEditor
//         simple digit line editor, input 0-9, .
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN DigitLineEditor(uint8 *buf, uint16 max_len, BOOLEAN is_add, uint8 ch)
{
    uint16 i = 0;
    BOOLEAN recode = TRUE;      //default, all action is valid
    
    //SCI_ASSERT(PNULL != buf);/*assert verified*/
    if(PNULL == buf)
    {
        //SCI_TRACE_LOW:"DigitLineEditor:PNULL == buf!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICALC_MAIN_2991_112_2_18_2_1_28_72,(uint8*)"");
        return FALSE;
    }
    
    if(MMIAPICOM_SearchChar(buf, 'e'))
    {
        SCI_MEMSET(buf, 0 ,max_len);        
    }
    
    i = SCI_STRLEN((char*) buf);
    if (buf[0] == '-')//操作数最大为13位，但负号不算在内
    {
        max_len ++;
    }
    
#ifdef CMCC_UI_STYLE
    if(is_add)
    {
        if(i <= max_len-1)// buffer not full, add a  char  into buffer
        {
            buf[i] = ch;
            buf[i+1] = 0;    
        }
        else
        {
            recode = FALSE;
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_BEYOND_LEN,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }
    else  // delete a char
    {
        if(i > 0)
        {
            buf[i-1 ] = 0;  
        }
        else
        {
            recode = FALSE;
        }
    }
#else
    if (is_add && i <= max_len-1)    // buffer not full, add a  char  into buffer
    {
        buf[i] = ch;
        buf[i+1] = 0;                       // end of buffer
    }
    else if (!is_add && i>0)             // delete a char
    {
        buf[i-1 ] = 0;  
    }
    else 
    	recode = FALSE;          // ACTION IS NOT VALID 
#endif

    return recode;
}

/*****************************************************************************/
// 	Description : init button text
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void InitButtonText(void)
{   
    MMI_CTRL_ID_T ctrl_id= 0;
    const uint32 buffer_size = 8;
    //char     button_str[8] = {0};
    uint8     button_str[8] = {0};
    wchar    button_wstr[8] = {0};
    uint32   button_wstr_length = 0;
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUI_BG_T    fg_info = {0};
#endif
    //set align mode
    for(ctrl_id = MMIACC_CALC_1_CTRL_ID; ctrl_id <= MMIACC_CALC_MRECALL_CTRL_ID; ctrl_id++ )
    {
        GUIBUTTON_SetTextAlign(ctrl_id ,  ALIGN_HVMIDDLE);
    }
    
    //set string C
#ifndef MMI_GUI_STYLE_MINISCREEN
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "AC");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_CLEAR_CTRL_ID, button_wstr,button_wstr_length)	;
#endif
    //set string +
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "+");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_PLUS_CTRL_ID, button_wstr,button_wstr_length)	;
    //set string DEL
#ifndef MMI_GUI_STYLE_MINISCREEN
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = IMAGE_CALC_DELETE;
    GUIBUTTON_SetFg(MMIACC_CALC_BACKSPACE_CTRL_ID, &fg_info);
    GUIBUTTON_SetPressedFg(MMIACC_CALC_BACKSPACE_CTRL_ID, &fg_info);
#endif
/*
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "DEL");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_BACKSPACE_CTRL_ID, button_wstr,button_wstr_length)	;
*/
    //set string x
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    //sprintf(button_str, "*");
    button_str[0] = 215 /* '*' */;
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_MUL_CTRL_ID, button_wstr,button_wstr_length)	;
    //set string =
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "=");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_EQUAL_CTRL_ID, button_wstr,button_wstr_length)	;
    //set string /
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    //sprintf(button_str, "/");
    button_str[0] = 247 /* '/' */;
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_DIV_CTRL_ID, button_wstr,button_wstr_length)	;

    //set string M+
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "M+");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_MPLUS_CTRL_ID, button_wstr,button_wstr_length)	;
    //set string M-
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "M-");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_MMINUS_CTRL_ID, button_wstr,button_wstr_length)	;
    //set string -
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "-");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_SUB_CTRL_ID, button_wstr,button_wstr_length)	;
    //set string MR
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "MR");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_MRECALL_CTRL_ID, button_wstr,button_wstr_length)	;
    //set string MC
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "MC");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_MCLEAR_CTRL_ID, button_wstr,button_wstr_length)	;		
    
    //set string 1~9
    for(ctrl_id = MMIACC_CALC_1_CTRL_ID; ctrl_id <= MMIACC_CALC_9_CTRL_ID; ctrl_id++ )
    {
        SCI_MEMSET(button_str, 0 , buffer_size);
        SCI_MEMSET(button_wstr, 0 , buffer_size * 2);
        sprintf(button_str, "%d", (ctrl_id - MMIACC_CALC_1_CTRL_ID + 1));
        button_wstr_length = SCI_STRLEN(button_str);
        MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1 , button_wstr_length );
        GUIBUTTON_SetText(ctrl_id, button_wstr,button_wstr_length)	;
    }
    //set string *
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, ".");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_STAR_CTRL_ID, button_wstr,button_wstr_length)	;
    
    //set string 0
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "%d", 0);
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size -1,  (uint8*)button_str  , buffer_size - 1, button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_0_CTRL_ID, button_wstr,button_wstr_length)	;
    
    //set string +/-
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    sprintf(button_str, "+/-");
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size -1,  (uint8*)button_str  , buffer_size -1 , button_wstr_length );
    GUIBUTTON_SetText(MMIACC_CALC_HASH_CTRL_ID, button_wstr,button_wstr_length)	;
}

#if defined(TOUCH_PANEL_SUPPORT) && !defined(MMI_CALCULATOR_MINI_SUPPORT)
/*****************************************************************************/
// 	Description : init button width and height
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL void InitButtonWidthandHeight(uint16 form_width, uint16 form_height)
{
    GUIFORM_CHILD_HEIGHT_T height = {0};    
    GUIFORM_CHILD_WIDTH_T width = {0};
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        width.type = GUIFORM_CHILD_WIDTH_FIXED;
        //EUQAL占两个BUTTON宽度
        width.add_data = (form_width - (BUTTONS_EACH_LINE_H-1)*s_hspace - (s_margin<<1))*2/BUTTONS_EACH_LINE_H + s_hspace;
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_EQUAL_CTRL_ID, &width);

        width.type = GUIFORM_CHILD_WIDTH_FIXED;
        width.add_data = (form_width - (BUTTONS_EACH_LINE_H-1)*s_hspace- (s_margin<<1))/BUTTONS_EACH_LINE_H;
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_CLEAR_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_MCLEAR_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_MPLUS_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_MMINUS_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_MRECALL_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_BACKSPACE_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_7_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_8_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_9_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_HASH_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_MUL_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_DIV_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_4_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_5_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_6_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_0_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_SUB_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_PLUS_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_1_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_2_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_3_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_STAR_CTRL_ID, &width);

        height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        height.add_data = (form_height - (BUTTONS_EACH_ROW_H-1)*s_vspace - (s_margin<<1))/BUTTONS_EACH_ROW_H;        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_CLEAR_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_MCLEAR_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_MPLUS_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_MMINUS_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_MRECALL_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL1_ID, MMIACC_CALC_BACKSPACE_CTRL_ID, &height);        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_7_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_8_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_9_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_HASH_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_MUL_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID, MMIACC_CALC_DIV_CTRL_ID, &height);        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_4_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_5_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_6_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_0_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_SUB_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL3_ID, MMIACC_CALC_PLUS_CTRL_ID, &height);        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_1_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_2_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_3_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_STAR_CTRL_ID, &height);        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL4_ID, MMIACC_CALC_EQUAL_CTRL_ID, &height);
    }
    else
    {
        width.type = GUIFORM_CHILD_WIDTH_FIXED;
        width.add_data = (form_width - (BUTTONS_EACH_LINE-1)*s_hspace - (s_margin<<1))/BUTTONS_EACH_LINE;
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_MCLEAR_CTRL_ID, &width); 
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_MRECALL_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_7_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_4_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_1_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_HASH_CTRL_ID, &width);          
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_MPLUS_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_MMINUS_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_8_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_5_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_2_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_0_CTRL_ID, &width);          
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_CLEAR_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_DIV_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_9_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_6_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_3_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_STAR_CTRL_ID, &width);          
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR4_ID,MMIACC_CALC_BACKSPACE_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR4_ID,MMIACC_CALC_MUL_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR4_ID,MMIACC_CALC_SUB_CTRL_ID, &width);  
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR4_ID,MMIACC_CALC_PLUS_CTRL_ID, &width);         
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR4_ID, MMIACC_CALC_EQUAL_CTRL_ID, &width);

        height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        //EUQAL占两个BUTTON高度
        height.add_data = (form_height - (BUTTONS_EACH_ROW-1)*s_vspace - (s_margin<<1))*2/BUTTONS_EACH_ROW + s_vspace;
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR4_ID, MMIACC_CALC_EQUAL_CTRL_ID, &height);
        
        height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        height.add_data = (form_height - (BUTTONS_EACH_ROW-1)*s_vspace - (s_margin<<1))/BUTTONS_EACH_ROW;
        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_MCLEAR_CTRL_ID, &height); 
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_MRECALL_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_7_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_4_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_1_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID,MMIACC_CALC_HASH_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_MPLUS_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_MMINUS_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_8_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_5_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_2_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID,MMIACC_CALC_0_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_CLEAR_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_DIV_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_9_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_6_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_3_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID,MMIACC_CALC_STAR_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR4_ID,MMIACC_CALC_BACKSPACE_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR4_ID,MMIACC_CALC_MUL_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR4_ID,MMIACC_CALC_SUB_CTRL_ID, &height);  
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR4_ID,MMIACC_CALC_PLUS_CTRL_ID, &height);  
    }
}
#else
/*****************************************************************************/
// 	Description : init button width and height
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL void InitButtonWidthandHeight(uint16 form_width, uint16 form_height)
{
    GUIFORM_CHILD_HEIGHT_T height = {0};    
    GUIFORM_CHILD_WIDTH_T width = {0};
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        width.type = GUIFORM_CHILD_WIDTH_FIXED;
        width.add_data = (form_width - (BUTTONS_EACH_LINE_H-1)*s_hspace- (s_margin<<1))/BUTTONS_EACH_LINE_H;
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID, MMIACC_CALC_7_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID, MMIACC_CALC_4_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID, MMIACC_CALC_1_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR1_ID, MMIACC_CALC_HASH_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID, MMIACC_CALC_8_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID, MMIACC_CALC_5_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID, MMIACC_CALC_2_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR2_ID, MMIACC_CALC_0_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID, MMIACC_CALC_9_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID, MMIACC_CALC_6_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID, MMIACC_CALC_3_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR3_ID, MMIACC_CALC_STAR_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR4_ID, MMIACC_CALC_MUL_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR5_ID, MMIACC_CALC_PLUS_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR5_ID, MMIACC_CALC_EQUAL_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR5_ID, MMIACC_CALC_SUB_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRR6_ID, MMIACC_CALC_DIV_CTRL_ID, &width);

        height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        height.add_data = (form_height - (BUTTONS_EACH_ROW_H-1)*s_vspace - (s_margin<<1))/BUTTONS_EACH_ROW_H;        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID, MMIACC_CALC_7_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID, MMIACC_CALC_4_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID, MMIACC_CALC_1_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR1_ID, MMIACC_CALC_HASH_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID, MMIACC_CALC_8_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID, MMIACC_CALC_5_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID, MMIACC_CALC_2_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR2_ID, MMIACC_CALC_0_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID, MMIACC_CALC_9_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID, MMIACC_CALC_6_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID, MMIACC_CALC_3_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR3_ID, MMIACC_CALC_STAR_CTRL_ID, &height);
        //+、-、X、/、=高度三等分
        height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        height.add_data = (form_height - 2*s_vspace - (s_margin<<1))/3;                 
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR4_ID, MMIACC_CALC_EMPTY_CTRL_ID_1, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR4_ID, MMIACC_CALC_MUL_CTRL_ID, &height);        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR4_ID, MMIACC_CALC_EMPTY_CTRL_ID_2, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR5_ID, MMIACC_CALC_PLUS_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR5_ID, MMIACC_CALC_EQUAL_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR5_ID, MMIACC_CALC_SUB_CTRL_ID, &height);        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR6_ID, MMIACC_CALC_EMPTY_CTRL_ID_3, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR6_ID, MMIACC_CALC_DIV_CTRL_ID, &height);        
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRR6_ID, MMIACC_CALC_EMPTY_CTRL_ID_4, &height);
    }
    else
    {
        width.type = GUIFORM_CHILD_WIDTH_FIXED;
        width.add_data = (form_width - (BUTTONS_EACH_LINE-1)*s_hspace - (s_margin<<1))/BUTTONS_EACH_LINE;
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID,MMIACC_CALC_EMPTY_CTRL_ID_1, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID,MMIACC_CALC_PLUS_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL1_ID,MMIACC_CALC_EMPTY_CTRL_ID_2, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID,MMIACC_CALC_MUL_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID,MMIACC_CALC_EQUAL_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL2_ID,MMIACC_CALC_DIV_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID,MMIACC_CALC_EMPTY_CTRL_ID_3, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID,MMIACC_CALC_SUB_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL3_ID,MMIACC_CALC_EMPTY_CTRL_ID_4, &width);
#ifndef MMI_CALCULATOR_MINI_SUPPORT
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL4_ID,MMIACC_CALC_7_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL4_ID,MMIACC_CALC_8_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL4_ID,MMIACC_CALC_9_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL5_ID,MMIACC_CALC_4_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL5_ID,MMIACC_CALC_5_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL5_ID,MMIACC_CALC_6_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL6_ID,MMIACC_CALC_1_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL6_ID,MMIACC_CALC_2_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL6_ID,MMIACC_CALC_3_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL7_ID,MMIACC_CALC_HASH_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL7_ID,MMIACC_CALC_0_CTRL_ID, &width);
        GUIFORM_SetChildWidth(MMI_CALC_CHILDFORMCTRL7_ID,MMIACC_CALC_STAR_CTRL_ID, &width);
#endif

        height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        height.add_data = (form_height - (BUTTONS_EACH_ROW-1)*s_vspace - (s_margin<<1))/BUTTONS_EACH_ROW;
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL1_ID,MMIACC_CALC_PLUS_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID,MMIACC_CALC_MUL_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID,MMIACC_CALC_EQUAL_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL2_ID,MMIACC_CALC_DIV_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL3_ID,MMIACC_CALC_SUB_CTRL_ID, &height);
        //michael wang 0423
#ifndef MMI_CALCULATOR_MINI_SUPPORT
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL4_ID,MMIACC_CALC_7_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL4_ID,MMIACC_CALC_8_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL4_ID,MMIACC_CALC_9_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL5_ID,MMIACC_CALC_4_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL5_ID,MMIACC_CALC_5_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL5_ID,MMIACC_CALC_6_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL6_ID,MMIACC_CALC_1_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL6_ID,MMIACC_CALC_2_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL6_ID,MMIACC_CALC_3_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL7_ID,MMIACC_CALC_HASH_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL7_ID,MMIACC_CALC_0_CTRL_ID, &height);
        GUIFORM_SetChildHeight(MMI_CALC_CHILDFORMCTRL7_ID,MMIACC_CALC_STAR_CTRL_ID, &height);
#endif
    }
}
#endif

/*****************************************************************************/
// 	Description : init button font
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL void InitButtonFont(void)
{
    GUI_FONT_ALL_T button_font = {0};
    GUI_FONT_ALL_T button_font_other = {0};
#if (defined MAINLCD_SIZE_128X160) || (defined MAINLCD_SIZE_176X220)
    button_font.font = SONG_FONT_20  ;
#else
    button_font.font = MMI_DEFAULT_NORMAL_FONT  ;
#endif
    button_font.color =  MMI_WHITE_COLOR;
    button_font_other.font = MMI_DEFAULT_NORMAL_FONT  ;
    button_font_other.color =  MMI_BLACK_COLOR;

    GUIBUTTON_SetFont(MMIACC_CALC_EQUAL_CTRL_ID , &button_font );
#ifdef TOUCH_PANEL_SUPPORT    
    GUIBUTTON_SetFont(MMIACC_CALC_PLUS_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_SUB_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_MUL_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_DIV_CTRL_ID , &button_font_other );
#else
    GUIBUTTON_SetFont(MMIACC_CALC_PLUS_CTRL_ID , &button_font );
    GUIBUTTON_SetFont(MMIACC_CALC_SUB_CTRL_ID , &button_font );
    GUIBUTTON_SetFont(MMIACC_CALC_MUL_CTRL_ID , &button_font );
    GUIBUTTON_SetFont(MMIACC_CALC_DIV_CTRL_ID , &button_font );
#endif    
    GUIBUTTON_SetFont(MMIACC_CALC_1_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_2_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_3_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_4_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_5_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_6_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_7_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_8_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_9_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_STAR_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_0_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_HASH_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_CLEAR_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_MPLUS_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_MMINUS_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_MCLEAR_CTRL_ID , &button_font_other );
    GUIBUTTON_SetFont(MMIACC_CALC_MRECALL_CTRL_ID , &button_font_other );   
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : update button status
//	Global resource dependence : 
//  Author: paul.huang
//	Note:   
/*****************************************************************************/
LOCAL void UpdateButtonStatus(MMI_MESSAGE_ID_E msg_id, BOOLEAN is_pressed)
{
    MMI_CTRL_ID_T ctrl_id = 0;
    MMI_MULTI_KEY_TP_MSG_PARAM_T  para                = {0};
    
    switch(msg_id) 
    {       
        case MSG_KEYDOWN_UP:
        case MSG_KEYUP_UP:
        case MSG_KEYPRESSUP_UP:    
            ctrl_id = MMIACC_CALC_PLUS_CTRL_ID;
            break;

        case MSG_KEYDOWN_DOWN:
        case MSG_KEYUP_DOWN:
        case MSG_KEYPRESSUP_DOWN:                
            ctrl_id = MMIACC_CALC_SUB_CTRL_ID;
            break;

        case MSG_KEYDOWN_LEFT:
        case MSG_KEYUP_LEFT:
        case MSG_KEYPRESSUP_LEFT:                
            ctrl_id = MMIACC_CALC_MUL_CTRL_ID;
            break;

        case MSG_KEYDOWN_RIGHT:            
        case MSG_KEYUP_RIGHT:            
        case MSG_KEYPRESSUP_RIGHT:                
            ctrl_id = MMIACC_CALC_DIV_CTRL_ID;
            break;

        case MSG_KEYDOWN_CANCEL://cr132551
        case MSG_KEYPRESSUP_CANCEL: 
        case MSG_KEYDOWN_DEL:
        case MSG_KEYUP_DEL:
            ctrl_id = MMIACC_CALC_BACKSPACE_CTRL_ID;
            break;

        case MSG_KEYDOWN_WEB:     
        case MSG_KEYDOWN_MIDDLE:
        case MSG_KEYUP_WEB:     
        case MSG_KEYUP_MIDDLE:
        case MSG_KEYPRESSUP_WEB:     
        case MSG_KEYPRESSUP_MIDDLE:     
            ctrl_id = MMIACC_CALC_EQUAL_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_0:            
        case MSG_KEYUP_0:
        case MSG_KEYPRESSUP_0:     
            ctrl_id = MMIACC_CALC_0_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_1:
        case MSG_KEYUP_1:
        case MSG_KEYPRESSUP_1:     
            ctrl_id = MMIACC_CALC_1_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_2:
        case MSG_KEYUP_2:
        case MSG_KEYPRESSUP_2:     
            ctrl_id = MMIACC_CALC_2_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_3:
        case MSG_KEYUP_3:
        case MSG_KEYPRESSUP_3:     
            ctrl_id = MMIACC_CALC_3_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_4:
        case MSG_KEYUP_4:
        case MSG_KEYPRESSUP_4:     
            ctrl_id = MMIACC_CALC_4_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_5:
        case MSG_KEYUP_5: 
        case MSG_KEYPRESSUP_5:     
            ctrl_id = MMIACC_CALC_5_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_6:
        case MSG_KEYUP_6:
        case MSG_KEYPRESSUP_6:     
            ctrl_id = MMIACC_CALC_6_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_7:
        case MSG_KEYUP_7: 
        case MSG_KEYPRESSUP_7:     
            ctrl_id = MMIACC_CALC_7_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_8:
        case MSG_KEYUP_8:
        case MSG_KEYPRESSUP_8:    
            ctrl_id = MMIACC_CALC_8_CTRL_ID;
            break;
            
        case MSG_KEYDOWN_9:
        case MSG_KEYUP_9:
        case MSG_KEYPRESSUP_9:     
            ctrl_id = MMIACC_CALC_9_CTRL_ID;
            break;

        case MSG_KEYDOWN_STAR:
        case MSG_KEYUP_STAR:
        case MSG_KEYPRESSUP_STAR:     
            ctrl_id = MMIACC_CALC_STAR_CTRL_ID;
            break;           
        
        case MSG_KEYDOWN_HASH:        
        case MSG_KEYUP_HASH:
        case MSG_KEYPRESSUP_HASH:     
            ctrl_id = MMIACC_CALC_HASH_CTRL_ID;
            break;
            
        default:
            break;
    }
//#if defined TOUCH_PANEL_SUPPORT
    if(is_pressed)
    {
        MMK_SendMsg(ctrl_id, MSG_TP_PRESS_DOWN, (DPARAM)&para);
    }
    else
    {
        MMK_SendMsg(ctrl_id, MSG_TP_PRESS_UP, (DPARAM)&para);
    }
//#endif
}
#endif
/*****************************************************************************/
// 	Description : MMIAPICALC_OpenMainWin
//         
//	Global resource dependence : 

//   Author:        ying.xu
//	Note:   
/*****************************************************************************/
LOCAL void InitFormWin(void)
{      
    GUI_RECT_T rect = MMITHEME_GetClientRect();
    GUI_RECT_T form_rect = {0};    
//#ifdef MMI_PDA_SUPPORT
    GUI_BG_T                    bg_info = {0}; 
//#endif
    uint16 form_width = 0;
    uint16 form_height = 0;
    MMI_CTRL_ID_T   button = 0;
    GUI_BG_T   press_bg = {0};

#if (defined MAINLCD_SIZE_128X160) || (defined MAINLCD_SIZE_176X220)
    form_rect.left = rect.left + MMICACL_OPERATOR_OFFSET_SPACE;
#ifdef MMI_CALCULATOR_MINI_SUPPORT
	form_rect.top = 	CALC_LINE_NUM_STARTY(3) + MMICACL_OPERATOR_OFFSET_TOP;
#else
    form_rect.top = s_CALC_LINE_END_Y + MMICACL_OPERATOR_OFFSET_TOP;
#endif
    form_rect.right = rect.right - MMICACL_OPERATOR_OFFSET_SPACE;
    form_rect.bottom = rect.bottom - MMICACL_OPERATOR_OFFSET_BOTTOM;
#else
    form_rect.left = rect.left;
#if (defined MAINLCD_SIZE_240X320) && (!defined TOUCH_PANEL_SUPPORT)
	form_rect.top = s_CALC_LINE_START_Y + s_CALC_LINE_HEIGHT * 3 + MMICACL_OPERATOR_OFFSET_TOP;
#else
    form_rect.top = s_CALC_LINE_END_Y + 2;
#endif
    form_rect.right = rect.right;
    form_rect.bottom = rect.bottom;
#endif

    form_width = form_rect.right - form_rect.left + 1;
    form_height = form_rect.bottom - form_rect.top + 1;

//#ifdef MMI_PDA_SUPPORT
	// 在运算按键的CHILDFORM (MMI_CALC_FORMCTRL_ID) 填上灰色背景色块 (IMAGE_CALC_NUMBER_BACKGROUND_BAR)
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_CALC_NUMBER_BACKGROUND_BAR;
    GUIFORM_SetBg(MMI_CALC_FORMCTRL_ID, &bg_info);
//#endif
    //add by James li begin
    form_rect.top -= 40;
    form_height += 40;
    //add by James li end
    //form child不显示背景
    GUIFORM_PermitChildBg(MMI_CALC_FORMCTRL_ID,FALSE);
    GUIFORM_PermitChildFont(MMI_CALC_FORMCTRL_ID,FALSE);
    GUIFORM_SetRect(MMI_CALC_FORMCTRL_ID, &form_rect);
    GUIFORM_SetSpace(MMI_CALC_FORMCTRL_ID, &s_hspace, &s_vspace);  
    GUIFORM_SetMargin(MMI_CALC_FORMCTRL_ID, s_margin);

    InitButtonText();    
    InitButtonWidthandHeight(form_width, form_height);
    InitButtonFont();

    for(button = MMIACC_CALC_1_CTRL_ID; button <= MMIACC_CALC_MRECALL_CTRL_ID; button++)
    {
        GUIBUTTON_SetRunSheen(button, FALSE);
        press_bg.bg_type = GUI_BG_IMG;
#if defined(TOUCH_PANEL_SUPPORT) && !defined(MMI_CALCULATOR_MINI_SUPPORT)
        press_bg.img_id  = IMAGE_CALC_PRESSED_BACKGROUND;
#else        
        press_bg.img_id  = IMAGE_CALC_PRESSED_BACKGROUND_BAR;
#endif
        GUIBUTTON_SetPressedBg(button,  &press_bg);
    }
}

/*****************************************************************************/
// 	Description :calculate the two operators ,type: 1,2,3,4 =-*/
//	Global resource dependence : 
//  Author: ying.xu
//	Note:   
/*****************************************************************************/
LOCAL int32  CalculateTwo(uint8* result, const uint8* operand1, const uint8* operand2, uint8 type)
{
    char * strStop = PNULL;
    double    dOp1 = 0;
    double	dOp2 = 0;
    double 	dResult = 0;
    uint8  buf[CALC_MAX_LENGTH*2 + 1] = {0};        //including symbo bit   
    CALC_RESULT_T error = CALC_NO_ERROR ;

    dOp1 = strtod((char*)operand1, (char**)&strStop);
    dOp2 = strtod((char*)operand2, (char**)&strStop);
    if(dOp1<1.0e-100&&dOp1>-1.0e-100)
    {
        dOp1 = 0;
    }
    if(dOp2<1.0e-100 && dOp2>-1.0e-100)
    {
        dOp2 = 0;
    }
    switch (type)
    {
    case '+':  
        dResult = dOp1 + dOp2;
        break;
        
    case '-':
        dResult = dOp1 - dOp2;
        break;
        
    case 215 /* '*' */:
        dResult = dOp1 * dOp2;
        if(dResult<1.0e-100 && dResult>-1.0e-100)//add for CR177013
        {
            dResult = 0;   
        }
        break;
        
    case 247 /* '/' */:
        if ( (dOp2 == 0) || (dOp2<1.0e-101&&dOp2>-1.0e-101) )
        {
            error = CALC_DEVIDED_BY_ZERO ;
        }
        else
        {
            dResult = dOp1 / dOp2;
            if(dResult<1.0e-100&&dResult>-1.0e-100)//add for CR177013
            {
                dResult = 0;   
            }
        }					
        break;
        
    default:
        //SCI_TRACE_LOW:"wrong operator type"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICALC_MAIN_3688_112_2_18_2_1_30_73,(uint8*)"");
        break;
    }	
    
    if (dResult > 9.9e100||dResult < -(9.9e100))/*错误处理*/
    {
        error = CALC_OVERFLOW ;						
    }
    if( CALC_NO_ERROR == error)
    {
         SCI_MEMSET(result, 0 , CALC_MAX_LENGTH + 2 );
        if( dResult <= -1e13 || dResult >= 1e13 )
        {    
            sprintf((char*)result, "%e", dResult);      
        }
        else if( dResult <= 1e-11 && dResult >= -1e-11 )     //去0和小数点的位置
        {
            if(dResult<1.0e-100 && dResult>-1.0e-100)   //0
            {
                result[0] = '0';
            }
            else
            {
                sprintf((char*)result, "%e", dResult);  
            }
                
        }
        else
        {     
            SCI_MEMSET(buf, 0, CALC_MAX_LENGTH*2 + 1);       
            // 尽可能长的打印，去除小数点的位置
            sprintf ( (char*)buf, "%-.*f", (CALC_MAX_LENGTH - 2), dResult);    
            if( buf[0] == '-')
            {
                SCI_MEMCPY((char*)result, (char*)buf, CALC_MAX_LENGTH + 1) ; 
            }
            else
            {
                SCI_MEMCPY((char*)result, (char*)buf, CALC_MAX_LENGTH) ;     
            }   
            //去除result尾部的0和‘.’
            {
                int32 i=0;
                int32 j=0;
                // 寻找‘.’
                for(i = CALC_MAX_LENGTH; i >= 0 ; i--)
                {
                    if(result[i] == '.')
                    {   
                        break;
                    }          
                }
                
                //找到‘.’,处理尾部的‘0’
                if(i != -1)
                {
                    for(j = CALC_MAX_LENGTH; j > i ; j--)
                    {
                        if(result[j] == '0'|| result[j] == 0)
                        {                    
                            result[j] = 0;                           
                        }       
                        else
                        {
                            break;
                        }
                    }
                }
                
                //判断是否处理‘.’
                if(i == j)
                {
                    result[i] = 0;
                }
                
            }
        }    
    }
    return error ;
}

/*****************************************************************************/
// 	Description :softkey control 
//	Global resource dependence : 
//  Author: ying.xu
//	Note:   
/*****************************************************************************/
LOCAL void UpdateSoftkey(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id )
{
	if(s_math_error_info == CALC_NO_ERROR)
    {
        if (isSoftkeyShowReturn())
        {
#ifdef GUI_MID_SOFTKEY_USE_IMG
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL,STXT_RETURN,TRUE);
            GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_EQUAL_ICON,1,TRUE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, STXT_EQUAL,STXT_RETURN,TRUE);
#endif
        }       
        else
        {
#ifdef GUI_MID_SOFTKEY_USE_IMG
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL,STXT_RETURNAWORD,TRUE);
            GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_EQUAL_ICON,1,TRUE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, STXT_EQUAL,STXT_RETURNAWORD,TRUE);
#endif
        }
    }
    else
	{
#ifdef GUI_MID_SOFTKEY_USE_IMG
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL,STXT_RETURNAWORD,TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_EQUAL_ICON,1,TRUE);
#else
		GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, STXT_EQUAL,STXT_RETURNAWORD,TRUE);
#endif	
	}	 
}
/*****************************************************************************/
// 	Description : MMIAPICALC_OpenMainWin
//         
//	Global resource dependence : 
//   Author:        zhaohui,modified by ying.xu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPICALC_OpenMainWin(void)
{
#if defined MAINLCD_SIZE_128X160 ||defined MAINLCD_SIZE_176X220 || defined MAINLCD_SIZE_240X320 \
    || defined MAINLCD_SIZE_240X400 || defined MAINLCD_SIZE_320X480
    MMK_CreateWin((uint32 *)CALC_WIN_TAB,PNULL);
    MMK_CreateCtrlByWinTabEx( MMIACC_CALC_WIN_ID,
    MMITHEME_IsMainScreenLandscape() ? (uint32 *)CALC_CTRL_TAB_H : (uint32 *)CALC_CTRL_TAB );
#else
    MMK_CreateWin((uint32 *)CALC_WIN_TAB,PNULL);
#endif
}
#else
/*****************************************************************************/
// 	Description : MMIAPICALC_OpenMainWin
//
//	Global resource dependence :
//   Author:        zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICALC_OpenMainWin(void)
{

}
#endif//CALCULATOR_SUPPORT


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
