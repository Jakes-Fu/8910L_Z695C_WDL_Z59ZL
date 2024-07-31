/*
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_menutable.c#2 $
 */

#include <jbed_exported_defines.h>
#include <window_parse.h>
#include <guimenu.h>
#include <mmi_text.h>
#include <mmi_image.h>
#include <mmieng_menutable.h>
#include <mmi_menutable.h>
#include <mmi_modu_main.h>
#include <mmijava_id.h>
#include <java_mmi_text.h>
#include <mmijava_menutable.h>
#include <mmi_module.h>
#include <mmijava_common.h>

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

#ifdef SUPPORT_DOWNLOAD
const GUIMENU_ITEM_T menu_download_box[]=
{
    {JAVA_DOWNLOAD_ID_GAME, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_DOWNLOAD_GAME, 0, 0, 0, MENU_NULL} ,
    {JAVA_DOWNLOAD_ID_EBOOK, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_DOWNLOAD_EBOOK, 0, 0, 0, MENU_NULL} ,
    {JAVA_DOWNLOAD_ID_OTHER, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_DOWNLOAD_OTHER, 0, 0, 0, MENU_NULL} ,
    {JAVA_DOWNLOAD_ID_SETTING, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_DOWNLOAD_SETTINGS, 0, 0, 0, MENU_NULL}
};
#endif

const GUIMENU_ITEM_T java_main_popup_menu[] =
{
    {JAVA_SET_ID_PERMISSION_SETTING, TIP_NULL, {IMAGE_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK},
        STR_PERMISSION_EXT01, 0, 0, 0, MENU_NULL},
    {JAVA_OPTION_MENU_ID_INFO, TIP_NULL, {IMAGE_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK},
        STR_INFO_EXT01, 0, 0, 0, MENU_NULL},
#if defined (NATIVE_AMS) && defined (AMS_TCK_RUNNER)
    {JAVA_OPTION_MENU_ID_TCK_RUNNER, TIP_NULL, {IMAGE_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK},
        TXT_APP_MANAGER, 0, 0, 0, MENU_NULL},
    {JAVA_OPTION_MENU_ID_TCK_NET_SETTING, TIP_NULL, {IMAGE_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK},
        TXT_COMMON_LINKSETTING, 0, 0, 0, MENU_NULL},
#endif
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, \
    _TITLE_BACKGROUND, _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT,

const GUIMENU_GROUP_T java_menu_table[] =
{
    {NULL},
    #include <mmijava_menutable.def>
};

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**
 * Register mmijava menu group.
 */
PUBLIC void MMIJAVA_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_JAVA, java_menu_table);
}
