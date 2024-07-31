/*
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/inc/mmijava_menutable.h#1 $
 */

#ifndef _JAVA_MENUTABLE_H_
#define _JAVA_MENUTABLE_H_

#include <mmijava_export.h>
#include <mmi_module.h>
#include <guimenu.h>

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
typedef enum
{
    JAVA_MENU_ID_START = (MMI_MODULE_JAVA << 16),
    JAVA_OPTION_MENU_ID_RUN,
#ifdef SUPPORT_BACKGROUND_RUN
    JAVA_OPTION_MENU_ID_FOREGROUND,
#endif
    JAVA_OPTION_MENU_ID_OPEN,
    JAVA_OPTION_MENU_ID_DELETE,
    JAVA_OPTION_MENU_ID_INFO,
#ifdef SUPPORT_UPDATE_FUNCTION
    JAVA_OPTION_MENU_ID_UPDATE,
#endif
    JAVA_OPTION_MENU_ID_SETTING,
#if defined (NATIVE_AMS) && defined(AMS_TCK_RUNNER)
    JAVA_OPTION_MENU_ID_TCK_RUNNER,
    JAVA_OPTION_MENU_ID_TCK_NET_SETTING,
#endif
    JAVA_OPTION_MENU_ID_LOCALINSTALL,
#ifdef SUPPORT_DOWNLOAD       
    JAVA_OPTION_MENU_ID_DOWNLOAD,
#endif
#ifdef SUPPORT_TASK_MANAGER       
    JAVA_OPTION_MENU_ID_TASKMANAGER,
#endif
#ifdef SUPPORT_DELETE_ALL_FUNCTION
    JAVA_OPTION_MENU_ID_DELETEALL,
#endif
    JAVA_OPTION_MENU_ID_ABOUT,
    JAVA_SET_ID_NETWORK_SETTING,
    JAVA_SET_ID_PERMISSION_SETTING,
#ifdef SUPPORT_DOWNLOAD       
    JAVA_DOWNLOAD_ID_GAME,
    JAVA_DOWNLOAD_ID_EBOOK,
    JAVA_DOWNLOAD_ID_OTHER,
    JAVA_DOWNLOAD_ID_SETTING,
#endif
    JAVA_MENU_ID_MAX
}JAVA_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, \
                 _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT) \
                 _TABLEID,

typedef enum
{
    JAVA_MENU_LABEL_START = (MMI_MODULE_JAVA << 16),
    #include "mmijava_menutable.def"
    JAVA_MENUTABLE_MAX
} JAVA_MENU_LABEL_E;

#undef MENU_DEF

/**---------------------------------------------------------------------------*/
/**                         Compiler Flag                                     */
/**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //#ifndef _JAVA_MENUTABLE_H_
