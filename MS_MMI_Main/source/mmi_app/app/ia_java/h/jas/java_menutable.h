/***********************************************************************
**Author:    huangjc
**Date:      2007-4-18
**CopyRight:AMOI 2007
**Description:This file define the function of java nv
***********************************************************************/
/***********************************************************************
**                                                Edit History
**      Date                    Name               Description
***2007-4-18              huangjc                create
***********************************************************************/
#ifndef _JAVA_MENUTABLE_H_
#define _JAVA_MENUTABLE_H_

#include "mmijava_export.h"
#include "mmi_module.h"
#include "guimenu.h"
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*
#if defined(_MMI_JAVA_)
#define MMIJAVA_MENUITEM_NUMBER_IN_PARENT_MENU (1)
#else
#define MMIJAVA_MENUITEM_NUMBER_IN_PARENT_MENU (0)
#endif
*/
typedef enum
{
     JAVA_ID_MENU_START = (MMI_MODULE_JAVA << 16),
     ID_JAVA,	 
     ID_JAVA_AMS,
     ID_JAVA_DOWNLOAD,
     ID_JAVA_LINKSET,
     ID_JAVA_APP_INSTALL,
     ID_JAVA_INSTALL,
     ID_JAVA_INFO,
     ID_JAVA_DELETE,
     ID_JAVA_AMS_OPTION_PLAY,
     ID_JAVA_AMS_OPTION_DELETE,
     ID_JAVA_AMS_OPTION_INFO,
     ID_JAVA_AMS_OPTION_APPLICATION_SET,
     ID_JAVA_AMS_OPTION_PERMIT_SET,//Permission
#ifdef MMI_JAVA_NW_V2
     ID_JAVA_AMS_OPTION_NETWORK_PROFILE, //Ivy add for network profile
#endif     
     ID_JAVA_AMS_OPTION_SET_CALL,
     ID_JAVA_AMS_OPTION_SET_LINK,
     ID_JAVA_AMS_OPTION_SET_MESSAGE,
     ID_JAVA_AMS_OPTION_SET_AUTOPLAY,
     ID_JAVA_AMS_OPTION_SET_LOCATION,
     ID_JAVA_AMS_OPTION_SET_MULTIM,
     ID_JAVA_AMS_OPTION_SET_USERDATAREAD,
     ID_JAVA_AMS_OPTION_SET_USERDATAWRITE,
     ID_JAVA_DOWNLOAD_GAME,
     ID_JAVA_DOWNLOAD_EBOOK,
     ID_JAVA_DOWNLOAD_OTHER,
     ID_JAVA_DOWNLOAD_SET,
     ID_JAVA_UPDATE,
#ifdef JAVA_SUPPORT_SYSTEM_RESET_FUNCTION
	 ID_JAVA_RESET,
#endif
	 JAVA_ID_MENU_MAX
}JAVA_ID_MENU_E;
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    JAVA_MENU_LABEL_START = (MMI_MODULE_JAVA << 16),
    #include "java_menutable.def"
    JAVA_MENUTABLE_MAX
} JAVA_MENU_LABEL_E;

#undef MENU_DEF
PUBLIC void MMIJAVA_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* MENUTABLE_H */

