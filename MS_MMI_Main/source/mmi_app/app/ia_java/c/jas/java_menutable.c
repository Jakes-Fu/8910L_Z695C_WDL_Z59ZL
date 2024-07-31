/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/*****************************************************************************
** File Name:      java_menutable.c                              *
** Description:    This file is used to describe digit camera                *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifndef _JAVA_MENUTABLE_C_
#define _JAVA_MENUTABLE_C_


#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "window_parse.h"
#include "guimenu.h"
//#include "mmibrw_text.h"
#include "mmi_modu_main.h"
#include "java_id.h"
#include "java_mmi_text.h"
#include "java_menutable.h"
#include "mmi_module.h"
#ifdef MMI_PDA_SUPPORT
#include "java_mmi_image.h"
#endif
    
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TIP_NULL 0
#define MENU_NULL NULL //lint !e767
    
#ifdef APP_HEXIN
#define HEXIN_MENU_ITEM 1
#else
#define HEXIN_MENU_ITEM 0 
#endif
#ifdef APP_N2SC
#define N2SC_MENU_ITEM 1 
#else
#define N2SC_MENU_ITEM 0 
#endif
    
#define GWM_MENU_ITEM 1 
    
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/    

const GUIMENU_ITEM_T	menu_application_set[] =
{
#ifdef MMI_JAVA_NW_V2
    {ID_JAVA_AMS_OPTION_NETWORK_PROFILE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_LINKSETTING, 0, 0, 0, MENU_NULL},
#endif
    {ID_JAVA_AMS_OPTION_PERMIT_SET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_JAVA_AMS_OPTION_PERMITION_SET, 0, 0, 0, MENU_NULL}
};


const GUIMENU_ITEM_T menu_download_box[]=
{
    {ID_JAVA_DOWNLOAD_GAME, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_DOWNLOAD_GAME, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_DOWNLOAD_EBOOK, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_DOWNLOAD_EBOOK, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_DOWNLOAD_OTHER, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_DOWNLOAD_OTHER, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_DOWNLOAD_SET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_DOWNLOAD_SETTINGS, 0, 0, 0, MENU_NULL} 
};


const GUIMENU_ITEM_T menu_java_ams_option[]=
{
#ifndef MMI_PDA_SUPPORT
    {ID_JAVA_AMS_OPTION_PLAY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_JAVA_AMS_OPTION_PLAY, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_DELETE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_DELETE, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_INFO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_DETAIL, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_APPLICATION_SET, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_JAVA_AMS_OPTION_APPLICATION_SET, 0, 0, 0, MENU_JAVA_AMS_OPTION_APPLICATION_SET},
    {ID_JAVA_UPDATE,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_UPDATE, 0, 0, 0, MENU_NULL},
#endif
    
#ifdef MMI_PDA_SUPPORT
    {ID_JAVA_APP_INSTALL,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COM_LOCALINSTALL, IMG_JAVA_MENU_LOCATION_INSTALL_ICON, IMG_JAVA_MENU_LOCATION_INSTALL_ICON, IMG_JAVA_MENU_LOCATION_INSTALL_ICON, MENU_NULL},
#ifdef JAVA_OTA_SUPPORT
    {ID_JAVA_DOWNLOAD,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_DOWNLOAD_COMN, IMG_JAVA_MENU_DOWNLOAD_ICON, IMG_JAVA_MENU_DOWNLOAD_ICON, IMG_JAVA_MENU_DOWNLOAD_ICON, MENU_DOWNLOAD_BOX},
#endif
#ifdef JAVA_SUPPORT_SYSTEM_RESET_FUNCTION
    {ID_JAVA_RESET,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_JAVA_APP_RESET_SYSTEM, IMG_JAVA_MENU_RESET_SYSTEM_ICON, IMG_JAVA_MENU_RESET_SYSTEM_ICON, IMG_JAVA_MENU_RESET_SYSTEM_ICON, MENU_NULL},
#endif
#else////not define pda
    
    {ID_JAVA_APP_INSTALL,TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COM_LOCALINSTALL, 0, 0, 0, MENU_NULL},
#ifdef JAVA_OTA_SUPPORT
    {ID_JAVA_DOWNLOAD,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_DOWNLOAD_COMN, 0, 0, 0, MENU_DOWNLOAD_BOX},
#endif
#ifdef JAVA_SUPPORT_SYSTEM_RESET_FUNCTION
    {ID_JAVA_RESET,TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_JAVA_APP_RESET_SYSTEM, 0, 0, 0, MENU_NULL},
#endif
#endif
};

const GUIMENU_ITEM_T menu_java_ams_option_permition_set[]=
{
    {ID_JAVA_AMS_OPTION_SET_CALL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_COMM_CALL, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_SET_LINK, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_JAVA_AMS_OPTION_SET_LINK, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_SET_MESSAGE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_MSG_SETTINGS, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_SET_AUTOPLAY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_JAVA_AMS_OPTION_SET_AUTOPLAY, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_SET_LOCATION, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_JAVA_AMS_OPTION_SET_LOCATION, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_SET_MULTIM, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_JAVA_AMS_OPTION_SET_MULTIM, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_SET_USERDATAREAD, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_JAVA_AMS_OPTION_SET_USERDATAREAD, 0, 0, 0, MENU_NULL} ,
    {ID_JAVA_AMS_OPTION_SET_USERDATAWRITE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_JAVA_AMS_OPTION_SET_USERDATAWRITE, 0, 0, 0, MENU_NULL} 
};

     
    
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/    
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
    _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 
    
const GUIMENU_GROUP_T java_menu_table[] = 
{
    {NULL},
    #include "java_menutable.def"
};

    
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIJAVA_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_JAVA, java_menu_table);
} 


#endif

#endif /* _JAVA_MENUTABLE_C_ */

