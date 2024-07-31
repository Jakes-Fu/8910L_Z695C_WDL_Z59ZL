/*****************************************************************************
*****************************************************************************/
#ifndef MMIWK_MENUTAB_H_
#define MMIWK_MENUTAB_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_text.h"
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
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    ID_MMIWK_MENU_START = (MMI_MODULE_WK<< 16),

    ID_POPUP_MENU_WK_MAIN_SIGNIN,
    ID_POPUP_MENU_WK_MAIN_SIGNUP,

    ID_MMIWK_MENU_TOTAL
}ID_MMIWK_MENU_E;
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
	MMI_MMIWK_BEGIN_MENU_START = (MMI_MODULE_WK<< 16),
	#include "mmiwk_menutable.def"
	MMIWK_MENUTABLE_MAX
} MMI_MMIWK_MENUTAB_E;
#undef MENU_DEF


#ifdef   __cplusplus
    }
#endif

#endif
