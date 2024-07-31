/*****************************************************************************
** File Name:      mmigps_menutable.h                                        *
** Author:         jian.ma                                                   *
** Date:           2007/07/19                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to bt menu                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME         DESCRIPTION                                   *
** 2007/07/19     jian.ma        Create                                      *
******************************************************************************/

#ifndef _MMIGPS_MENUTABLE_H_
#define _MMIGPS_MENUTABLE_H_

#include "mmi_module.h"

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


//Menu ID
typedef enum
{
    GPS_ID_MENU_START = (MMI_MODULE_GPS << 16),              

    GPS_ID_MENU_MAX
}GPS_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    GPS_MENU_LABEL_START = (MMI_MODULE_GPS << 16),
    #include "mmigps_menutable.def"
    GPS_MENUTABLE_MAX
} GPS_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* MENUTABLE_H */

