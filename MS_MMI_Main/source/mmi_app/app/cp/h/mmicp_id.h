#ifndef MMICP_ID_H_
#define MMICP_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    CP_WIN_ID_START = (MMI_MODULE_CP << 16),

#include "mmicp_id.def"

    CP_MAX_WIN_ID
}CP_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    CP_CTRL_ID_ID_START = CP_MAX_WIN_ID,//(MMI_MODULE_CP << 16),
    CP_SIM1_TEXT_CTRL_ID,
    CP_SIM2_TEXT_CTRL_ID,
    CP_VOL_PROGRESS_CTRL_ID,
    CP_BRI_PROGRESS_CTRL_ID,
    CP_BT_CTRL_ID,
    CP_DATA_CTRL_ID,
	CP_SIM_PREFERRED_CTRL_ID,
    CP_IM_LANGUAGE_CTRL_ID,
    CP_WIN_SCROLL_CTRL_ID,
	CP_SIM_SELECT_MENU_CTRL_ID,
    CP_IM_LANGUAGE_MENU_CTRL_ID,
    CP_BT_SPINNER_CTRL_ID,
    CP_MAX_CTRL_ID
}CP_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description : Register control panel IDs
//    Global resource dependence : none
//    Author: fu.ruilin
//    Note:
/*****************************************************************************/
PUBLIC void MMICP_RegWinIdNameArr(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
