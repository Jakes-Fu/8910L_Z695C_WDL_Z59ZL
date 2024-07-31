/**
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_midletinfowin.c#2 $
 */

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include <guiim.h>
//#include <guiim_base.h>
//#include <mmitheme_im.h>
//#include <mmk_app.h>
//#include <mmk_kbd.h>
//#include <mmi_appmsg.h>
//#include <mmi_common.h>
//#include <mmiconnection_export.h>
//#include <mmiset_export.h>
//#include <caf.h>
#include "sci_types.h"
//#include <mmicc_export.h>
//#include <guilistbox.h>
//#include <guilabel.h>
//#include <guiprgbox.h>
//#include <guitext.h>
//#include <guidropdownlist.h>
//#include <mmipub.h>
//#include <jbed_types.h>
//#include <jbed_exported_defines.h>
#include "window_parse.h"
#include "mmijava_id.h"
//#include <mmijava_menutable.h>
//#include <java_mmi_text.h>
//#include <cpl_tpal.h>
//#include <mmijava_common.h>
//#include <jbed_global.h>
#include "guirichtext.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                   *
 **---------------------------------------------------------------------------*/

//Max detail item number.
#define NO_URL_TAG            (L"nourl")
#define TRUSTED_TAG           (L"trusted")

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     ( sizeof (a) / sizeof (a)[0] )
#endif

/*---------------------------------------------------------------------------*/
/*                          EXTERN FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : common window handler, including operations on delete, app details, delete all, runtime error
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleCommonInfoWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                      );

WINDOW_TABLE(JBED_COMM_INFO_WIN_TAB) =
{
    WIN_FUNC((uint32)Jbed_HandleCommonInfoWinMsg),
#ifdef MAINLCD_SIZE_128X160
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
#endif
    WIN_ID(JBED_COMM_INFO_WIN_ID),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    CREATE_RICHTEXT_CTRL(MMIJAVA_COMM_INFO_CRTL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};
/*****************************************************************************/
//  Description : Read one line detail information
//  Global resource dependence :
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL wchar* readline(wchar *data, int *end_pos, int offset, int length);
/*****************************************************************************/
//  Description : Add all midlet information to detail page.
//  Global resource dependence :
//  Author:  Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL void addMidletInfo(unsigned short*  jar_info_data, int length, MMI_CTRL_ID_T ctrl_id);
/*****************************************************************************/
//  Description : Add one datail item that include title and it's content.
//  Global resource dependence :
//  Author:  Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL void AddOneDetailItem(
                            MMI_CTRL_ID_T   ctrl_id,
                            MMI_TEXT_ID_T   label_title,
                            MMI_TEXT_ID_T   label_name,
                            uint16          *index_ptr,
                            wchar           *detail_ptr,
                            uint16          detail_len
                            );
/**************************************************************************************************/
// Description: common window handler, including operations on delete, app details, delete all, runtime error
// Date:2011-08-23
// Author:Alex.Li
// Note:
//*************************************************************************************************/
LOCAL MMI_RESULT_E Jbed_HandleCommonInfoWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                      )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIJAVA_COMM_INFO_CRTL_ID;
    MMIEvent * p_mmi_evt = NULL;
    int * p_cur_event_id = MMK_GetWinAddDataPtr(win_id);
    p_mmi_evt = MMIJAVA_find_MMIEvent(*p_cur_event_id);
    if(NULL == p_mmi_evt)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    {
        MMI_STRING_T title_str;

        // update title on status bar.
        MMI_GetLabelTextByLang(STR_INFO_EXT01, &title_str);
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str.wstr_ptr, title_str.wstr_len))
        {
            GUIWIN_UpdateStb();
        }

        addMidletInfo((unsigned short *)(p_mmi_evt->data), p_mmi_evt->data_bytes>>1, ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    }

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;

    case MSG_KEYLONG_POWER:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIJAVA_util_deallocMMIEvent_byEventID(p_mmi_evt->event_id);
        MMK_FreeWinAddData(win_id);
        MMIJAVA_NotifyJVMExit();
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;

}
/*****************************************************************************/
//  Description : Read one line detail information
//  Global resource dependence :
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL wchar* readline(wchar *data, int *end_pos, int offset, int length)
{
    uint16 size = 0;
    int i = 0;
    int j = 0;
    wchar* ret_data = NULL;
    //check how much space need allocated.
    for (i = offset; i < length; i++)
    {
        if (data[i] != '\n')
        {
            size++;
        }
        else
        {
            *end_pos = i;
            break;
        }
    }
    if (0 == size)
    {
        return NULL;
    }
    ret_data = (wchar *)SCI_ALLOCA(sizeof(wchar) * (size + 1));
    if (NULL == ret_data)
    {
        SCI_TRACE_LOW("Error SCI_ALLOCA failed!!");
        return NULL;
    }
    SCI_MEMSET(ret_data, 0, sizeof(wchar) * (size + 1));
    for (i = offset; (i < length) && (j < (size + 1)); i++,j++)
    {
        if (data[i] != '\n')
        {
            ret_data[j] = data[i];
        }
        else
        {
            ret_data[j] = '\0';
            break;
        }
    }
    return ret_data;
}

/*****************************************************************************/
//  Description : Add all midlet information to detail page.
//  Global resource dependence :
//  Author:  Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL void addMidletInfo(unsigned short*  jar_info_data, int length, MMI_CTRL_ID_T ctrl_id)
{
    wchar*         ret_array = NULL;
    int             end_pos = 0;
    int             start_pos = 0;
    uint16          nIndex = 0;
    int             i;

    MMI_TEXT_ID_T detail_title[] = {
                                      STR_JAVA_NAME,
                                      STR_JAVA_SIZE,
                                      STR_JAVA_VERSION,
                                      STR_JAVA_VENDOR,
                                      STR_JAVA_URL,
                                      STR_JAVA_SIGNED,
                                      STR_JAVA_SERIAL,
                                      STR_JAVA_ISSUED,
                                      STR_JAVA_AUTHENTICATED,
                                      STR_JAVA_DOMAIN,
                                      STR_JAVA_PUSHED
                                 };

    //Read all detail information one by one and add them to detail page.
    //util reach the end of string.
    for (i = 0; (i < ARRAY_SIZE(detail_title)) && (start_pos < length); i++)
    {
        ret_array = readline(jar_info_data, &end_pos, start_pos, length);
        if (NULL == ret_array)
        {
            start_pos = end_pos + 1;
            SCI_TRACE_LOW("No information about item %d", i);
            continue;
        }
        //check wheather has the url information.
        if (4 == i)
        {
            if (0 == MMIAPICOM_Wstrcmp(ret_array, (wchar*)NO_URL_TAG))
            {
                //If no url information just read next item information.
                start_pos = end_pos + 1;
                SCI_FREEIF(ret_array);
                continue;
            }
        }
        //check trust or unstrust domain information
        if (5 == i)
        {
            //If it has trusted domaion tag, skip this tag and read trusted domain information.
            if (0 == MMIAPICOM_Wstrcmp(ret_array, (wchar*)TRUSTED_TAG))
            {
                start_pos = end_pos + 1;
                SCI_FREEIF(ret_array);
                continue;
            }
            else
            {
                //if it is untrusted domain.
                i = i + 4;
            }
        }

        AddOneDetailItem(ctrl_id,
             detail_title[i],
             PNULL,
             &nIndex,
             ret_array,
             start_pos - end_pos + 1);
        SCI_FREEIF(ret_array);
        start_pos = end_pos + 1;
    }
}

/*****************************************************************************/
//  Description : Add one datail item that include title and it's content.
//  Global resource dependence :
//  Author:  Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL void AddOneDetailItem(
                            MMI_CTRL_ID_T   ctrl_id,
                            MMI_TEXT_ID_T   label_title,
                            MMI_TEXT_ID_T   label_name,
                            uint16          *index_ptr,
                            wchar           *detail_ptr,
                            uint16          detail_len
                            )
{
    GUIRICHTEXT_ITEM_T item_data = {0};
    MMI_STRING_T detail_str = {0};
    SCI_TRACE_LOW("AddOneDetailItem enter ");

    //set XXX title
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    MMI_GetLabelTextByLang( label_title, &detail_str );
    item_data.text_data.buf.len = detail_str.wstr_len;
    item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
    GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);

    //set XXX name
    if ( PNULL == label_name && PNULL!=detail_ptr )
    {
        SCI_MEMSET(&item_data,0,sizeof(item_data));
        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.text_data.buf.len = detail_len;
        item_data.text_data.buf.str_ptr = detail_ptr;
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);
    }
    else if (PNULL != label_name)
    {
        SCI_MEMSET(&item_data,0,sizeof(item_data));
        SCI_MEMSET(&detail_str,0,sizeof(detail_str));
        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        MMI_GetLabelTextByLang(label_name, &detail_str );
        item_data.text_data.buf.len = detail_str.wstr_len;
        item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);
    }
    else
    {
        SCI_TRACE_LOW("AddOneDetailItem fail ");
    }
}

/*****************************************************************************/
//  Description : handle "Run" and "Runtime error" events from JVM
//  Global resource dependence : none
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_OpenMidletInfoWin(int event_id)
{
    int * p_mmi_evt_id = NULL;
    p_mmi_evt_id = (int *)SCI_ALLOCA(sizeof(int));
    *p_mmi_evt_id = event_id;
    MMK_CreateWin((uint32*)JBED_COMM_INFO_WIN_TAB, (ADD_DATA)p_mmi_evt_id);

}

/*****************************************************************************/
//  Description :Check whether this menu_id should display on dynamic popup menu, return
//  TRUE if display, otherwise return FALSE
//  Global resource dependence :
//  Author: Alex.Li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_ShowSuiteInfo(int  suite_id)
{
    if(MMIJAVA_HandleStateConflict())
    {
        return FALSE;
    }

#if defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN)
    /* We already check INSTALL or UNINSTALL conflict before, if the jbed thread
     * still exists at this time, it means that the midlet is running on background
     * or vm is starting or closing.
     */
    if (isJbedThreadExist())
    {
        SCI_TRACE_LOW("=== Myriad MMIJAVA_ShowSuiteInfo Jbed thread exist.");

        if (Jbed_tm_getRunningAppNum() > 0) // the midlet is running on background.
        {
            MMIJAVA_PostMIDletRunningBanner();
            return FALSE;
        }
        else // vm is starting or closing.
        {
            MMIJAVA_OpenAlertByTextId(STR_JAVA_VM_BUSY);
            return FALSE;
        }
    }
#endif

    //Display the application detail information
    Jbed_nAms_queryDetail(suite_id);
    MMIJAVA_util_openWaitingWin(NULL);

    return  TRUE;
}
