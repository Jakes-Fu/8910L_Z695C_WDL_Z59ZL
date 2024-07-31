#include "std_header.h"
#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmikur_musicstyle.c                                           *
** Author:                                                                   *
** Date:           10/2007                                                   *
** Description:    This file is used to describe kur Music Style             *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2007        cmzs Lics          create                                  *
******************************************************************************/

#define _MMIKUR_MUSICSTYLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "window_parse.h"
#include "mmikur_id.h"
#include "mmidisplay_data.h"
#include "mmikuro_text.h"
#include "mmipub.h"
#include "guilcd.h"
#include "mmikur.h"
#include "mmikur_internal.h"
#include "guilistbox.h"
#include "mmifmm_export.h"
#include "mmiidle_subwintab.h"
#include "guilabel.h"
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmikur_lyric_internal.h"
#include "guiref_scale.h"
#include "mmiaudio_ctrl.h"
#include "mmifm_export.h"
#include "mmimpeg4.h"
#include "mmi_default.h"
//#include "mmimultim.h"

#include "mmk_timer.h"

#include "mmikur_musicstyle.h"
#include "kdbparser.h"
  /**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

extern WINDOW_TABLE( MMIKUR_MAIN_PLAY_WIN_TAB );

int    kdb_classes_type_current_count = 0;
int     c_KURMS_Class_currentselect = 0;
int     c_KURMS_Type_currentselect = 0;
int     c_KURMS_PlayList_currentselect = 0;

BOOL    g_bCheckSysDatOk = FALSE;

LOCAL BOOL KDBParser(void);

/*****************************************************************************/
//  Description : handle kur play list window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurMusicStyleList(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     );
/*****************************************************************************/
//  Description : handle kur Music Style Type window message
//  Global resource dependence : 
//  Author:cmzs Lics
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurMusicStyleTypeList(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     );

/*****************************************************************************/
//  Description : handle kur play list window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurMusicStylePlayList(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     );
/*****************************************************************************/
//  Description : append kur play list
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AppendKurMusicStylePlayListItem(
                                 MMI_TEXT_ID_T      left_softkey_id,
                                 MMI_CTRL_ID_T      ctrl_id,
                                 BOOLEAN            is_update
                                 );

/*****************************************************************************/
//  Description : append kur Music Style list
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AppendKurMusicStyleClassListItem(
                                 MMI_TEXT_ID_T      left_softkey_id,
                                 MMI_CTRL_ID_T      ctrl_id,
                                 BOOLEAN            is_update
                                 );

LOCAL void AppendKurMusicStyleTypeListItem(
                                 MMI_TEXT_ID_T      left_softkey_id,
                                 MMI_CTRL_ID_T      ctrl_id,
                                 BOOLEAN            is_update
                                 );

/*****************************************************************************/
//  Description : append one kur name to listbox
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
//LOCAL void AppendKurMusicStyleMusicNameToListbox(
//                               uint16               kur_index,      //in:
//                               MMI_STRING_T         *ucs2_name_ptr, //in: 仅为了减少多次内存分配
//                               GUILIST_ITEM_DATA_T   *item_ptr,      //in:
//                               MMI_CTRL_ID_T        ctrl_id         //in:
//                                );

/*****************************************************************************/
//  Description : append one kur name to listbox
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
//LOCAL void AddKurMusicStyleClassNameToListbox(
//                               uint16               kur_index,      //in:
//                               MMI_STRING_T         *ucs2_name_ptr, //in: 仅为了减少多次内存分配
//                               GUILIST_ITEM_DATA_T   *item_ptr,      //in:
//                               MMI_CTRL_ID_T        ctrl_id         //in:
//                               );

/*****************************************************************************/
//  Description : Play a song reference by kdb
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL BOOL PlayKdbSong(int nclassindex,int ntypeindex,int nsongindex);

/*****************************************************************************/
//  Description : add songs to g_all_kur_info from kdb
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AddKdbSong(int nclassindex,int ntypeindex);

// the window for KUR Music Style window
WINDOW_TABLE( MMIKUR_MUSICSTYLE_CLASS_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleKurMusicStyleList ),    
    WIN_ID( MMIKUR_MUSICSTYLE_CLASS_WIN_ID ),
    WIN_TITLE( TXT_KUR_PLAY_LIST ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIKUR_PLAY_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

WINDOW_TABLE( MMIKUR_MUSICSTYLE_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleKurMusicStyleTypeList ),    
    WIN_ID( MMIKUR_MUSICSTYLE_TYPE_WIN_ID ),
    WIN_TITLE( TXT_KUR_PLAY_LIST ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIKUR_PLAY_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

WINDOW_TABLE( MMIKUR_MUSICTYPE_PLAY_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleKurMusicStylePlayList ),    
    WIN_ID( MMIKUR_MUSICSTYLE_PLAY_LIST_WIN_ID),
    WIN_TITLE( TXT_KUR_PLAY_LIST ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIKUR_PLAY_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};

/*****************************************************************************/
//  Description : open kur player
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OpenKurMusicStyle(void)
{
    MMI_WIN_ID_T    alert_win_id = MMIKUR_ALERT_WIN_ID;
    //init kur Music Style
    //for the file name has exited already, add the file name with "(i)"
    KDBParser();

    //judge if has kur
    if (0 < MMIKDB_get_classescount())
    {
        if(MMK_CreateWin((uint32 *)MMIKUR_MUSICSTYLE_CLASS_WIN_TAB, PNULL))
        {
        }
    }
    else
    {
        //提示没有kur
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_NO_KDB,TXT_NULL,
            IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}

BOOL KDBParser()
{
    if(!MMIKDB_kdbinit())
        return FALSE;
    return TRUE;
}

/*****************************************************************************/
//  Description : handle kur play list window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurMusicStyleList(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     )
{
//    uint16          max_num = g_kur_all_info.total_num;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIKUR_PLAY_LIST_CTRL_ID;
    uint16          cur_selection = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
//        GUILIST_SetMaxItem(ctrl_id,100,FALSE);//max_num);
        AppendKurMusicStyleClassListItem(TXT_COMMON_OK,ctrl_id,FALSE);
    //  GUILISTBOX_SetNeedDisplayInfo(ctrl_id, TRUE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

       case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
            cur_selection = GUILIST_GetCurItemIndex(MMIKUR_PLAY_LIST_CTRL_ID);
            //if(cur_selection != -1)
            {
                c_KURMS_Class_currentselect = cur_selection;
                if(MMIKDB_get_class_typecount(c_KURMS_Class_currentselect) > 0)
                {
                    MMK_CloseWin(win_id);
                    MMK_CreateWin((uint32 *)MMIKUR_MUSICSTYLE_TYPE_WIN_TAB, PNULL);
                }
                else
                {
                    MMK_CloseWin(win_id);
                    c_KURMS_Type_currentselect = 0;
                    MMK_CreateWin((uint32 *)MMIKUR_MUSICTYPE_PLAY_LIST_WIN_TAB, PNULL);
                }
            }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        MMIKDB_kdbuninit();
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }


    return (result);
}

/*****************************************************************************/
//  Description : handle kur Music Style Type window message
//  Global resource dependence : 
//  Author:cmzs Lics
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurMusicStyleTypeList(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     )
{
//    uint16          max_num = g_kur_all_info.total_num;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIKUR_PLAY_LIST_CTRL_ID;
    uint16          cur_selection = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
//        GUILIST_SetMaxItem(ctrl_id,100,FALSE);//max_num);
        AppendKurMusicStyleTypeListItem(TXT_COMMON_OK,ctrl_id,FALSE);//TXT_COMMON_PLAY
    //  GUILISTBOX_SetNeedDisplayInfo(ctrl_id, TRUE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

        case MSG_CTL_MIDSK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
            cur_selection = GUILIST_GetCurItemIndex(MMIKUR_PLAY_LIST_CTRL_ID);
            c_KURMS_Type_currentselect = cur_selection;
            //if(cur_selection != -1)
            {
                MMK_CloseWin(win_id);
                MMK_CreateWin((uint32 *)MMIKUR_MUSICTYPE_PLAY_LIST_WIN_TAB, PNULL);
            }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        MMK_CreateWin((uint32 *)MMIKUR_MUSICSTYLE_CLASS_WIN_TAB, PNULL);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }


    return (result);
}
/*****************************************************************************/
//  Description : append kur play list
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AppendKurMusicStyleClassListItem(
                                 MMI_TEXT_ID_T      left_softkey_id,
                                 MMI_CTRL_ID_T      ctrl_id,
                                 BOOLEAN            is_update
                                 )
{
    uint16              i               = 0;
    MMI_STRING_T        ucs2_uint8_name = {0};   
    GUILIST_ITEM_T       item_t = {0};
    GUILIST_ITEM_DATA_T  log_list        = {0};
    MMI_HANDLE_T    list_handle = 0;

    char** pClassList = (char **)MMIKDB_get_classlist();
    list_handle = MMK_ConvertIdToHandle(ctrl_id);

    if(GUILIST_GetTotalItemNum(ctrl_id) && !is_update)
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }
    SCI_MEMSET(&ucs2_uint8_name,0,sizeof(MMI_STRING_T));
    ucs2_uint8_name.wstr_ptr = SCI_ALLOC_APP(MMIKUR_NAME_MAX_LEN*sizeof(wchar));
    SCI_MEMSET(&log_list, 0, sizeof(GUILIST_ITEM_DATA_T));

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &log_list;

    //set softkey
    log_list.softkey_id[0] = (MMI_TEXT_ID_T)left_softkey_id;
    log_list.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    log_list.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
    
    log_list.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    log_list.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
   

    if(!is_update)
        GUILIST_SetMaxItem( ctrl_id, MMIKDB_get_classescount(), FALSE);
    //add items to listbox
    for (i=0; i< MMIKDB_get_classescount(); i++)
    {
        if(pClassList[i] == PNULL)
            continue;

        //set left icon
        
        log_list.item_content[0].item_data.image_id = (MMI_IMAGE_ID_T)IMAGE_CONTROL_LIST_CHECK_SELECTED_ICON;

        SCI_MEMSET((char*)ucs2_uint8_name.wstr_ptr,0,(MMIKUR_NAME_MAX_LEN*sizeof(wchar)));

        //To UC2
        if(strlen(pClassList[i]) >= MMIKUR_NAME_MAX_LEN)
        {
            pClassList[i][MMIKUR_NAME_MAX_LEN] = 0;
        }
        
        ucs2_uint8_name.wstr_len = GUI_GBToWstr(ucs2_uint8_name.wstr_ptr, (uint8*)pClassList[i], SCI_STRLEN((char*)pClassList[i]));
        
        //append to listbox
        log_list.item_content[1].item_data.text_buffer.wstr_len = ucs2_uint8_name.wstr_len;
        log_list.item_content[1].item_data.text_buffer.wstr_ptr = ucs2_uint8_name.wstr_ptr;
//         MMI_WSTRNCPY(
//             log_list.item_content[1].item_data.text_buffer.wstr, GUILIST_STRING_MAX_NUM + 1, 
//             ucs2_uint8_name.wstr_ptr, ucs2_uint8_name.wstr_len, 
//             ucs2_uint8_name.wstr_len
//             );
       
        if( is_update )
        {
            GUILIST_ReplaceItem( list_handle, &item_t, i );
        }
        else
        {
            GUILIST_AppendItem( list_handle, &item_t );
        }
    }
    if(c_KURMS_Class_currentselect < MMIKDB_get_classescount() && c_KURMS_Class_currentselect >= 0)
    {
        GUILIST_SetCurItemIndex(list_handle,c_KURMS_Class_currentselect);
    }
    //free memory
    SCI_FREE(ucs2_uint8_name.wstr_ptr);
    
   // GUILISTBOX_SetNeedDisplayInfo(ctrl_id,FALSE);

}

/*****************************************************************************/
//  Description : append kur play list
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AppendKurMusicStyleTypeListItem(
                                 MMI_TEXT_ID_T      left_softkey_id,
                                 MMI_CTRL_ID_T      ctrl_id,
                                 BOOLEAN            is_update
                                 )
{
    uint16              nCount = 0;
    uint16              i               = 0;
    MMI_STRING_T        ucs2_uint8_name = {0};
    GUILIST_ITEM_T       item_t = {0};
    GUILIST_ITEM_DATA_T  log_list        = {0};
    MMI_HANDLE_T        list_handle = 0;

    char **pKMS_Type;
    uint16 cur_selection = c_KURMS_Class_currentselect;
    kdb_classes_type_current_count = MMIKDB_get_class_typecount(cur_selection);
    pKMS_Type = (char **)MMIKDB_get_typelist(cur_selection);
    list_handle = MMK_ConvertIdToHandle(ctrl_id);
    
    if(GUILIST_GetTotalItemNum(ctrl_id) && !is_update)
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }
    
    //保存Type
    SCI_MEMSET(&ucs2_uint8_name,0,sizeof(MMI_STRING_T));
    ucs2_uint8_name.wstr_ptr = SCI_ALLOC_APP(MMIKUR_NAME_MAX_LEN*sizeof(wchar));
    SCI_MEMSET(&log_list, 0, sizeof(GUILIST_ITEM_DATA_T));

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &log_list;

    //set softkey
    log_list.softkey_id[0] = (MMI_TEXT_ID_T)left_softkey_id;
    log_list.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    log_list.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;

    log_list.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    log_list.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    if(!is_update)
        GUILIST_SetMaxItem( ctrl_id, kdb_classes_type_current_count, FALSE);

    //add items to listbox
    for (i=0; i< kdb_classes_type_current_count; i++)
    {
        if(pKMS_Type[i] == PNULL)
            break;
        if(strlen(pKMS_Type[i]) == 0)
            continue;

        //set left icon
        log_list.item_content[0].item_data.image_id = IMAGE_CONTROL_LIST_CHECK_SELECTED_ICON;

        SCI_MEMSET((char*)ucs2_uint8_name.wstr_ptr,0,(MMIKUR_NAME_MAX_LEN*sizeof(wchar)));

        //To UC2
        ucs2_uint8_name.wstr_len = GUI_GBToWstr(ucs2_uint8_name.wstr_ptr, (uint8*)pKMS_Type[i], SCI_STRLEN(pKMS_Type[i]));

        //append to listbox
        log_list.item_content[1].item_data.text_buffer.wstr_len = ucs2_uint8_name.wstr_len;
        log_list.item_content[1].item_data.text_buffer.wstr_ptr = ucs2_uint8_name.wstr_ptr;
//         MMI_WSTRNCPY(
//             log_list.item_content[1].item_data.text_buffer.wstr, GUILIST_STRING_MAX_NUM + 1, 
//             ucs2_uint8_name.wstr_ptr, ucs2_uint8_name.wstr_len, 
//             ucs2_uint8_name.wstr_len
//             );

        if( is_update )
        {
            GUILIST_ReplaceItem( list_handle, &item_t, i );
        }
        else
        {
            GUILIST_AppendItem( list_handle, &item_t );
        }

        nCount ++;
    }
    
    //free memory
    if(c_KURMS_Type_currentselect < kdb_classes_type_current_count && c_KURMS_Type_currentselect >= 0)
    {
        GUILIST_SetCurItemIndex(list_handle,c_KURMS_Type_currentselect);
    }

    SCI_FREE(ucs2_uint8_name.wstr_ptr);
    
   // GUILISTBOX_SetNeedDisplayInfo(ctrl_id,FALSE);

}
/*****************************************************************************/
//  Description : append one kur name to listbox
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
//LOCAL void AddKurMusicStyleClassNameToListbox(
//                               uint16               kur_index,      //in:
//                               MMI_STRING_T         *ucs2_name_ptr, //in: 仅为了减少多次内存分配
//                               GUILIST_ITEM_DATA_T   *item_ptr,      //in:
//                               MMI_CTRL_ID_T        ctrl_id         //in:
//                                )
// {
//    SCI_ASSERT(PNULL != ucs2_name_ptr);
//    SCI_ASSERT(PNULL != ucs2_name_ptr->str_ptr);
//
//    SCI_ASSERT(PNULL != item_ptr);
//    
//    //append to listbox
//    item_ptr->str_len = MIN(ucs2_name_ptr->length,LISTBOX_STRING_MAX_NUM);
//    MMI_MEMCPY(item_ptr->str,LISTBOX_STRING_MAX_NUM,
//        ucs2_name_ptr->str_ptr,ucs2_name_ptr->length,
//        item_ptr->str_len);
//    item_ptr->is_ucs2 = ucs2_name_ptr->is_ucs2;
//
//    GUILISTBOX_AppendItem(ctrl_id, item_ptr);
// }

/*****************************************************************************/
//  Description : handle kur play list window message
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKurMusicStylePlayList(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E   msg_id, 
                                     DPARAM             param
                                     )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIKUR_PLAY_LIST_CTRL_ID;
    uint16              cur_selection = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendKurMusicStylePlayListItem(TXT_COMMON_PLAY,ctrl_id,FALSE);
    //  GUILISTBOX_SetNeedDisplayInfo(ctrl_id, TRUE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

        case MSG_CTL_MIDSK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
            cur_selection = GUILIST_GetCurItemIndex(MMIKUR_PLAY_LIST_CTRL_ID);
            c_KURMS_PlayList_currentselect = cur_selection;
            if(PlayKdbSong(c_KURMS_Class_currentselect,c_KURMS_Type_currentselect,c_KURMS_PlayList_currentselect))
            {
                MMK_CloseWin(win_id);
                if(MMK_IsOpenWin(MMIKUR_MUSICSTYLE_CLASS_WIN_ID))
                    MMK_CloseWin(MMIKUR_MUSICSTYLE_CLASS_WIN_ID);
                if(MMK_IsOpenWin(MMIKUR_MUSICSTYLE_TYPE_WIN_ID))
                    MMK_CloseWin(MMIKUR_MUSICSTYLE_TYPE_WIN_ID);
                if(MMK_IsOpenWin(MMIKUR_PLAY_MENU_WIN_ID))
                    MMK_CloseWin(MMIKUR_PLAY_MENU_WIN_ID);
            }
            else
            {
                if(!KDBParser())
                {
                    MMK_CloseWin(win_id);
                    if(MMK_IsOpenWin(MMIKUR_MUSICSTYLE_CLASS_WIN_ID))
                        MMK_CloseWin(MMIKUR_MUSICSTYLE_CLASS_WIN_ID);
                    if(MMK_IsOpenWin(MMIKUR_MUSICSTYLE_TYPE_WIN_ID))
                        MMK_CloseWin(MMIKUR_MUSICSTYLE_TYPE_WIN_ID);
                    if(MMK_IsOpenWin(MMIKUR_PLAY_MENU_WIN_ID))
                        MMK_CloseWin(MMIKUR_PLAY_MENU_WIN_ID);
                }
            }
                
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id );
        if(MMIKDB_get_class_typecount(c_KURMS_Class_currentselect) > 0)
            MMK_CreateWin((uint32 *)MMIKUR_MUSICSTYLE_TYPE_WIN_TAB, PNULL);
        else
            MMK_CreateWin((uint32 *)MMIKUR_MUSICSTYLE_CLASS_WIN_TAB, PNULL);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : append kur play list
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AppendKurMusicStylePlayListItem(
                                 MMI_TEXT_ID_T      left_softkey_id,
                                 MMI_CTRL_ID_T      ctrl_id,
                                 BOOLEAN            is_update
                                 )
{
    uint16              i               = 0;
    MMI_STRING_T        ucs2_uint8_name = {0};
    GUILIST_ITEM_T       item_t = {0};
    GUILIST_ITEM_DATA_T  log_list        = {0};
    MMI_HANDLE_T        list_handle = 0;

    char pSongPath[KDB_MAX_PATH_LEN];
    char** pSongList;

    int nSongCount = 0;

    list_handle = MMK_ConvertIdToHandle(ctrl_id);
    
    if(GUILIST_GetTotalItemNum(ctrl_id) && !is_update)
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }

    pSongList = (char **)MMIKDB_get_songslist(c_KURMS_Class_currentselect,c_KURMS_Type_currentselect, &nSongCount);

    SCI_MEMSET(&ucs2_uint8_name,0,sizeof(MMI_STRING_T));
    ucs2_uint8_name.wstr_ptr = SCI_ALLOC_APP(MMIKUR_NAME_MAX_LEN*sizeof(wchar));
    SCI_MEMSET(&log_list, 0, sizeof(GUILIST_ITEM_DATA_T));
    
    //get current call ring index

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &log_list;
    //set softkey
    log_list.softkey_id[0] = (MMI_TEXT_ID_T)left_softkey_id;
    log_list.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    log_list.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;

    log_list.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    log_list.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    if(!is_update)
        GUILIST_SetMaxItem(ctrl_id,nSongCount,FALSE);
    //add items to listbox
    for (i=0; i< nSongCount; i++)
    {
        if(pSongList[i] == PNULL)
            continue;
        if(MMIKDB_get_songpath(c_KURMS_Class_currentselect,c_KURMS_Type_currentselect,i,pSongPath,KDB_MAX_PATH_LEN))
        {
            if(pSongPath[0] == 'E' || pSongPath[0] == 'e')
            {           
                log_list.item_content[0].item_data.image_id = (MMI_IMAGE_ID_T)IMAGE_COMMON_MUSIC_SD_ICON;
            }
            else
            {
                log_list.item_content[0].item_data.image_id = (MMI_IMAGE_ID_T)IMAGE_COMMON_MUSIC_UDISK_ICON;
            }

            SCI_MEMSET(ucs2_uint8_name.wstr_ptr, 0, (MMIKUR_NAME_MAX_LEN*sizeof(wchar)));
            ucs2_uint8_name.wstr_len = GUI_GBToWstr(ucs2_uint8_name.wstr_ptr, (uint8*)pSongList[i], SCI_STRLEN((char*)pSongList[i]));
            //append to listbox
            log_list.item_content[1].item_data.text_buffer.wstr_len = ucs2_uint8_name.wstr_len;

            log_list.item_content[1].item_data.text_buffer.wstr_ptr = ucs2_uint8_name.wstr_ptr;

//             MMI_WSTRNCPY(
//                 log_list.item_content[1].item_data.text_buffer.wstr, GUILIST_STRING_MAX_NUM + 1, 
//                 ucs2_uint8_name.wstr_ptr, ucs2_uint8_name.wstr_len, 
//                 ucs2_uint8_name.wstr_len
//                 );

            if( is_update )
            {
                GUILIST_ReplaceItem( list_handle, &item_t, i );
            }
            else
            {
                GUILIST_AppendItem( list_handle, &item_t );
            }
        }
    }

    if(c_KURMS_PlayList_currentselect < nSongCount && c_KURMS_PlayList_currentselect >= 0)
    {
        GUILIST_SetCurItemIndex(list_handle,c_KURMS_PlayList_currentselect);
    }

    //free memory
    SCI_FREE(ucs2_uint8_name.wstr_ptr);
    
  //  GUILISTBOX_SetNeedDisplayInfo(ctrl_id,FALSE);

}

/*****************************************************************************/
//  Description : append one kur name to listbox
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
//LOCAL void AppendKurMusicStyleMusicNameToListbox(
//                               uint16               kur_index,      //in:
//                               MMI_STRING_T         *ucs2_name_ptr, //in: 仅为了减少多次内存分配
//                               GUILIST_ITEM_DATA_T   *item_ptr,      //in:
//                               MMI_CTRL_ID_T        ctrl_id         //in:
//                               )
// {
//    SCI_ASSERT(PNULL != ucs2_name_ptr);
//    SCI_ASSERT(PNULL != ucs2_name_ptr->str_ptr);
//
//    SCI_ASSERT(PNULL != item_ptr);
//    
//    //append to listbox
//    item_ptr->str_len = MIN(ucs2_name_ptr->length,LISTBOX_STRING_MAX_NUM);
//    MMI_MEMCPY(item_ptr->str,LISTBOX_STRING_MAX_NUM,
//        ucs2_name_ptr->str_ptr,ucs2_name_ptr->length,
//        item_ptr->str_len);
//    item_ptr->is_ucs2 = ucs2_name_ptr->is_ucs2;
//    GUILISTBOX_AppendItem(ctrl_id, item_ptr);
// }


PUBLIC void MMKKUR_MUSICSTYLE_CreatePlayListWindow()
{
    MMK_CreateWin((uint32 *)MMIKUR_MUSICTYPE_PLAY_LIST_WIN_TAB, PNULL);
}


/*****************************************************************************/
//  Description : Play a song reference by kdb
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL BOOL PlayKdbSong(int nclassindex,int ntypeindex,int nsongindex)
{
    AddKdbSong(nclassindex,ntypeindex);
    MMIKUR_LoadKuroByIndex((uint32)nsongindex);
    if(!MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
    {
#ifdef UI_P3D_SUPPORT
        MMI_Disable3DEffect(MMI_3D_EFFECT_KUR_PLAYER);
#endif 
        MMK_CreateWin((uint32 *)MMIKUR_MAIN_PLAY_WIN_TAB, PNULL);
    }
    return MMIKUR_PlayKur();
//  MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_NO_MP3,TXT_NULL,
//      IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
//  return TRUE;
}

/*****************************************************************************/
//  Description : add songs to g_all_kur_info from kdb
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
LOCAL void AddKdbSong(int nclassindex,int ntypeindex)
{
    int     nIndex = 0;
    char    ** ppSongList = {0};
    char    *pSongName = {0};
    int     nSongCount = 0;
    MMIKUR_LIST_INFO_T  kur_list = {0};
    FILEARRAY           file_array = PNULL;
    FILEARRAY_DATA_T    kur_info = {0};

    MMIAPIKUR_StopKurPlayer();
    MMIKUR_ResetList();
    
    ppSongList = (char **)MMIKDB_get_songslist(nclassindex,ntypeindex,&nSongCount);
    if(0 < nSongCount)
    {
        file_array = MMIAPIFILEARRAY_Create();
        if(PNULL != file_array)
        {
            for (nIndex = 0; nIndex < nSongCount; nIndex ++)
            {
                pSongName = (char *)ppSongList[nIndex];

				//arvinzhang: delete assert
                //SCI_ASSERT(SCI_STRLEN(pSongName) < MMIFILE_FULL_PATH_MAX_LEN);
                
                kur_info.name_len = GUI_GBToWstr(kur_info.filename, (uint8*)pSongName,
												MIN(SCI_STRLEN(pSongName), MMIFILE_FULL_PATH_MAX_LEN));

				if(MMIAPIFILEARRAY_Add(file_array, &kur_info))
                {
                    kur_list.total_num++;
                }
                else
                {
                    break;
                }
            }
        }
        kur_list.is_from_kdb        = TRUE;
        kur_list.kdb_class_index    = nclassindex;
        kur_list.kdb_type_index     = ntypeindex;
        kur_list.list_array         = file_array;
        
        MMIKUR_SetKurList(&kur_list);
        MMIKUR_InitRandomList();
    }
}
#endif



/*Edit by script, ignore 3 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
