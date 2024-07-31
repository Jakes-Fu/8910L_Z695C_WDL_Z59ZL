
#ifndef _MMI_DEVICELOCK_MENUTABLE_H_
#define _MMI_DEVICELOCK_MENUTABLE_H_

#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"

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
//²Ëµ¥ID
typedef enum
{
    ID_DEVICELOCK_TEST_MENU,
	  ID_DEVICELOCK_TOTAL
}MMI_DEVICELOCK_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
	DEVICELOCK_MENU_LABEL_START = (MMI_MODULE_DEVICELOCK << 16),
    #include "mmi_devicelock_menutable.def"
	DEVICELOCK_MENU_LABEL_TABLE_MAX
} MMI_DEVICELOCK_MENU_LABEL_E;

#undef MENU_DEF

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register menu group
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIDEVICELOCK_RegMenuGroup(void);

/*-------------------------------------------------------------------------*/
/*                         VARIABLES                                       */
/*-------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* _MMI_DEVICELOCK_MENUTABLE_H_ */
