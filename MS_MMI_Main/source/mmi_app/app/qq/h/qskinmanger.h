/*****************************************************************************
** File Name:      qskinmanger.h                                                  *
** Author:                                                                   *
** Date:             11/05/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare QQ skin manager              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010       fen.xie       Create
******************************************************************************/
#ifndef SKINMGR_H
#define SKINMGR_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "datatype.h"
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

#define QIntToRGBVAL(_x) 			MAKE_RGB((_x>>16)&0x000000ff,(_x>>8)&0x000000ff,(_x)&0x000000ff)  
#define QIntToColor(_x)      		MAKE_RGB((_x>>16)&0x000000ff,(_x>>8)&0x000000ff,(_x)&0x000000ff)


#define QGetRGBVAL_B(_x)       	_x&0x001f//((_x>>24)&0xff)
#define QGetRGBVAL_G(_x)	     	(_x>>5)&0x003f//((_x>>16)&0xff)
#define QGetRGBVAL_R(_x)	     	(_x>>11)&0x001f //((_x>>8)&0xff)


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/

typedef enum{
	QTHEME_INDOOR = 0,  //  “ƒ⁄
	QTHEME_OUTDOOR,		//  “Õ‚
	QTHEME_NIGHT,		// “πº‰	
	QTHEME_TOTAL_NUM
}QTheme_Index;

typedef struct
{
	
	// image
	QU16	Image_Left_Arrow;
	QU16	Image_Right_Arrow;
	
	// RGBVAL
	RGBVAL 	GdiRGBVAL_Buddy_Sel_Fill;			
	RGBVAL 	GdiRGBVAL_Buddy_Sel_Frame;	
	RGBVAL 	GdiRGBVAL_Notify_BG;
	RGBVAL 	GdiRGBVAL_Menu_BG;
	RGBVAL 	GdiRGBVAL_Menu_Sel_Fill;
	RGBVAL 	GdiRGBVAL_Menu_Frame;
	RGBVAL	GuiRGBVAL_Menu_Text;
	
	// new
	RGBVAL	GdiRGBVAL_Tab_Bg_Fill;
	RGBVAL	GdiRGBVAL_Tab_Frame;
	RGBVAL	GdiRGBVAL_Tab_Sel_Fill;
	RGBVAL	GdiRGBVAL_BMenu_Fill;
	RGBVAL	GdiRGBVAL_BMenu_ISFrame;
	RGBVAL	GdiRGBVAL_BMenu_OSFrame;
	RGBVAL	GdiRGBVAL_Login_Bg_Fill;
	RGBVAL	GdiRGBVAL_Scroll_Frame;
	RGBVAL	GdiRGBVAL_Scroll_Fill;
	
	
}QTheme;

typedef struct{
	QTheme			stQThemes[QTHEME_TOTAL_NUM];
	QU8				cThemeSel;
}QSkinManger;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
RGBVAL GetBuddySelFill(QSkinManger* sk);
RGBVAL GetBuddySelFrame(QSkinManger* sk);
RGBVAL GetNotifyBG(QSkinManger* sk);
RGBVAL GetMenuBG(QSkinManger* sk);
RGBVAL GetMenuSelFill(QSkinManger* sk);
RGBVAL GetMenuFrame(QSkinManger* sk);
RGBVAL GetMenuText(QSkinManger* sk);

// new
RGBVAL GetTabBgFill(QSkinManger* sk);
RGBVAL GetTabFrame(QSkinManger* sk);
RGBVAL GetTabSelFill(QSkinManger* sk);
RGBVAL GetBMenuFill(QSkinManger* sk);
RGBVAL GetBMenuISFrame(QSkinManger* sk);
RGBVAL GetBMenuOSFrame(QSkinManger* sk);
RGBVAL GetLoginBgFill(QSkinManger* sk);
RGBVAL GetScrollFrame(QSkinManger* sk);
RGBVAL GetScrollFill(QSkinManger* sk);


RGBVAL QTAB_BG_FILL(QSkinManger* sk);      
RGBVAL QTAB_TRANS_BEGIN(QSkinManger* sk); 
RGBVAL QTAB_TRANS_END(QSkinManger* sk);    
RGBVAL QTAB_BO_TOP(QSkinManger* sk);		
RGBVAL QTAB_BO_BOTTOM(QSkinManger* sk);	 
RGBVAL QTAB_TRI(QSkinManger* sk);		     
RGBVAL QTAB_FONT(QSkinManger* sk);	 
RGBVAL QSTATUSBAR_BO(QSkinManger* sk);	   
RGBVAL QSTATUSBAR_TRANS_BEGIN(QSkinManger* sk); 
RGBVAL QSTATUSBAR_TRANS_END(QSkinManger* sk);   
RGBVAL QSTATUSBAR_FONT(QSkinManger* sk);		
RGBVAL QBG(QSkinManger* sk);					
RGBVAL QGOP1(QSkinManger* sk);				
RGBVAL QGOP2(QSkinManger* sk);			
RGBVAL QCHOOSE_TRANS_BEGIN(QSkinManger* sk);			
RGBVAL QCHOOSE_TRANS_END(QSkinManger* sk);			
RGBVAL QCHOOSE_BO(QSkinManger* sk); 			
RGBVAL QMENU_BG(QSkinManger* sk);				
RGBVAL QMENU_BO(QSkinManger* sk);				
RGBVAL QMENU_FONT(QSkinManger* sk);
RGBVAL QMENU_FONT_SEL(QSkinManger* sk);
RGBVAL QMENU_FALSEFONT(QSkinManger* sk);

RGBVAL QCHOOSEBOX_BO1(QSkinManger* sk);			
RGBVAL QCHOOSEBOX_BO2(QSkinManger* sk);	
RGBVAL QCHOOSEBOX_BO3(QSkinManger* sk);	
RGBVAL QCHOOSEBOX_BO4(QSkinManger* sk);	
RGBVAL QCHOOSEBOX_BO5(QSkinManger* sk);	
RGBVAL QCHOOSEBOX_BO6(QSkinManger* sk);	
RGBVAL QCHOOSEBOX_BO7(QSkinManger* sk);
RGBVAL QCHOOSEBOX_BO8(QSkinManger* sk);
RGBVAL QCHOOSEBOX_SETFONT1(QSkinManger* sk);
RGBVAL QCHOOSEBOX_SETFONT2(QSkinManger* sk);
RGBVAL QOPTIONBOX_RECT(QSkinManger* sk);
RGBVAL QOPTIONBOX_BG(QSkinManger* sk);
RGBVAL QOPTIONBOX_SELBG(QSkinManger* sk);
RGBVAL QSELOKBOX_B01(QSkinManger* sk);
RGBVAL QSELOKBOX_B02(QSkinManger* sk);
RGBVAL QSELDOWNBUTTON_B01(QSkinManger* sk);
RGBVAL QSELDOWNBUTTON_B02(QSkinManger* sk);
RGBVAL QSELDOWNBUTTON_TRIAGNLE(QSkinManger* sk);

RGBVAL QSELECTLIST_SELRECT(QSkinManger* sk);
RGBVAL QSELECTLIST_TEXT   (QSkinManger* sk);
RGBVAL QSELECTLIST_FILL   (QSkinManger* sk);

RGBVAL QEMO_DIVIDLINE (QSkinManger* sk);
RGBVAL QEMO_SELECT_FRAME (QSkinManger* sk);
RGBVAL QEMO_SELECT_FILL (QSkinManger* sk);
RGBVAL QTIP_BG(QSkinManger* sk);
RGBVAL 	QTIP_TEXT(QSkinManger* sk);	


RGBVAL QLOGIN_WAIT_SLIDER_BG  (QSkinManger* sk);
RGBVAL QLOGIN_WAIT_SLIDER_TOP  (QSkinManger* sk);
RGBVAL QLOGIN_WAIT_SLIDER_BOTTOM  (QSkinManger* sk);
RGBVAL QLOGIN_WAIT_SLIDER_FRAME  (QSkinManger* sk);

RGBVAL QCHATWIN_BG(QSkinManger* sk);
RGBVAL 	QCHATWIN_TEXT(QSkinManger* sk);
RGBVAL 	QCHATWIN_FRIENDNAME(QSkinManger* sk);
RGBVAL 	QCHATWIN_MYNAME(QSkinManger* sk);
QU32 	QCHATWIN_OPERATE(QSkinManger* sk);
RGBVAL QCHATWIN_TEXT_BG(QSkinManger* sk);
QU32 	QCHATWIN_TEXT_SEL(QSkinManger* sk);
RGBVAL QCHATWIN_SELECT(QSkinManger* sk);
RGBVAL QCHATWIN_TOOLSPANEL(QSkinManger* sk);
RGBVAL QCHATWIN_TOOLBAR_BGTOP (QSkinManger* sk);
RGBVAL QCHATWIN_TOOLBAR_BGBOTTOM (QSkinManger* sk);
RGBVAL QCHATWIN_TOOLBAR_SELFRAME  (QSkinManger* sk);
RGBVAL QCHATWIN_TOOLBAR_SELFILL(QSkinManger* sk);
RGBVAL 	QCHATWIN_TOOLBAR_HELPTEXT(QSkinManger* sk);
RGBVAL QCHATWIN_SCROLLBAR(QSkinManger* sk);

RGBVAL QLOADING_BODER(QSkinManger* sk);
RGBVAL QLOADING_TRANSB(QSkinManger* sk);
RGBVAL QLOADING_TRANSE(QSkinManger* sk);
RGBVAL  QLOADINGEX_FRAME(QSkinManger* sk);

RGBVAL 	QBAR_TEXT(QSkinManger* sk);
RGBVAL 	QLIST_VIP(QSkinManger* sk);
RGBVAL 	QLIST_VIP_SEL(QSkinManger* sk);
RGBVAL 	QTEXT(QSkinManger* sk);
RGBVAL 	QTEXT_SEL(QSkinManger* sk);

RGBVAL QSCROLLBAR_FRAME(QSkinManger* sk);
RGBVAL QSCROLLBAR_FILL(QSkinManger* sk);
RGBVAL QSCROLLBAR_DIVIDLINE(QSkinManger* sk);
RGBVAL QSCROLLBAR_BG(QSkinManger* sk);

RGBVAL QTAB_MYFACE_FRAME(QSkinManger* sk);

RGBVAL QSELECTBAR_FRAME(QSkinManger* sk);

RGBVAL QTIPS_BG(QSkinManger* sk);
RGBVAL QTIPS_FONT(QSkinManger* sk);

RGBVAL BUTTON_FRAME (QSkinManger* sk);
RGBVAL BUTTON_TRANS_BEGIN (QSkinManger* sk);
RGBVAL BUTTON_TRANS_END (QSkinManger* sk);
RGBVAL QBUTTON_FONT(QSkinManger* sk);
RGBVAL 	GetTitleText(QSkinManger* sk);




/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


