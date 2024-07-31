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
#include "jktypes.h"
#include "jgurtoolkit.h"
#include "jgincommandmenu.h"
#include "jgintitle.h"
#include "jginlistitem.h"
#include "jglistitem.h"
#include "jkgraphics.h"

#include "jgtextbox.h"
#include "jgintextbox.h"

#include "jkvideo.h"
#include "jginimplicit.h"
#include "mmijava_internal.h"

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
static JKImageHandle        hImageTitleBG = JK_INVALID_HANDLE;
static JKImageHandle        hImageDlgBG = JK_INVALID_HANDLE;
//static JKImageHandle        hImageMenuBarBG = JK_INVALID_HANDLE;
//static JKImageHandle        hImageIndexNumBG = JK_INVALID_HANDLE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : commandmenuCreated
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL JKUint32 commandmenuCreated(const JGT_ComEvent *comEvent);

/*****************************************************************************/
//  Description : commandmenuPaint
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL JKUint32 commandmenuPaint(const JGT_ComEvent *comEvent);



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
JKUint32 JgurTextboxKeyevent_handler(const JGT_KeyEvent *event)
{
	JKUint32 key = event->keyCode;
    if ((event->type == AM_EVENT_KEY_CLICKED) &&( key >= AM_KEYCODE_NUM0 && key <= AM_KEYCODE_NUM9 || key == AM_KEYCODE_SELECT) )//lint !e685 !e568
    {
		if(JgTextInputIsEditable(event->component))
	    {
		    return JG_KEY_RES_SET_ACTIVE;
	    }
    }
    return 0;
}

/*****************************************************************************/
//  Description : JgurInit
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
JKSint32 JgurInit(int argc, const char *argv[])
{

    JginCommandMenuSetComEvtHandler(JG_CMLST_EVT_CREATED, commandmenuCreated);
    JginCommandMenuSetComEvtHandler(JG_COM_EVT_PAINT,commandmenuPaint);
    JginTextBoxSetPreKeyEvtHandler(JgurTextboxKeyevent_handler);

    return (JKSint32)0;
}

/*****************************************************************************/
//  Description : commandmenuCreated
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL JKUint32 commandmenuCreated(const JGT_ComEvent *comEvent)
{
    JKSint32 top = SPREADTRUM_LCD_SOFTKEY_HEIGHT;
    JKSint32 left = 0;
    JKSint32 right = 0;
    JKSint32 bottom = 5;
    
    JginComSetInsets(comEvent->component, top, left, right, bottom);
    JginComSetBackground(comEvent->component, 0xff00ff00);
    
    return 0;
}

/*****************************************************************************/
//  Description : commandmenuPaint
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL JKUint32 commandmenuPaint(const JGT_ComEvent *comEvent)
{
    JGCommandMenu  commondmenu = 0;
    JKGraphicsContext g = 0;
    JKSint32 x = 0, y = 0, width = 0, height = 0;
    JKT_Rect rect = {0};
    //+CR254300
    MMI_STRING_T textmenu = {0};
    //-cr254300
    //JKGraphicsContext bg;//huangjc
    commondmenu = (JGCommandMenu)comEvent->component;
    g = (JKGraphicsContext)comEvent->param1;    
    
    x = JgComGetX(commondmenu);                        
    y = JgComGetY(commondmenu);                       
    width = JgComGetWidth(commondmenu);
    height = JgComGetHeight(commondmenu);
    
    //SCI_TRACE_LOW:"commandmenuPaint"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JGURTOOLKIT_117_112_2_18_2_22_34_261,(uint8*)"");
    JkGraphicsSetColor(g, 0xfff8f8ff);
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    JkVideoFillRect(g, &rect);
    
    if (hImageDlgBG != JK_INVALID_HANDLE)
    {
        JkVideoDrawImage(g, hImageDlgBG, 0, 0 , JK_VIDEO_TRANS_NONE);
    }
    
    /* 上面的BG可能会覆盖Title, 所以再画一次Title */ 
    if (hImageTitleBG != JK_INVALID_HANDLE)
    {
        JkVideoDrawImage(g, hImageTitleBG, 0, 0, JK_VIDEO_TRANS_NONE);  
    }
    else    /* 以白色填充 */
    {
        JkGraphicsSetColor(g, 0xffffffff);        
        rect.x = 0;
        rect.y = 0;
        rect.width = SPREADTRUM_LCD_SOFTKEY_WIDTH;
        rect.height = SPREADTRUM_LCD_SOFTKEY_HEIGHT;
        JkVideoFillRect(g, &rect);
        JkGraphicsSetColor(g, 0xff);
        JkVideoDrawLine(g, 0, rect.height, rect.width, rect.height);
    }
    
    JkGraphicsSetColor(g, 0xff000000);
    //+CR254300
    MMI_GetLabelTextByLang(STXT_MENU, &textmenu);
    JkVideoDrawString(g, textmenu.wstr_ptr, textmenu.wstr_len, 100, 5);
    //+CR254300
    
    JginCommandMenuPaintContent(commondmenu,g);
    return 0;   
}

#endif
