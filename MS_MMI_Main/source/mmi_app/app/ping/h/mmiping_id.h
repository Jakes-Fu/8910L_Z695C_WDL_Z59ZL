#ifdef MMIENG_PING_SUPPORT
#ifndef MMIPING_ID_H_
#define MMIPING_ID_H_

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
typedef enum {
        MMIPING_WIN_ID_START = (MMI_MODULE_PING << 16),
        MMIPING_PING_MODE_WIN_ID,
        MMIPING_PING_COUNT_WIN_ID,
        MMIPING_PING_IPTYPE_WIN_ID,
        MMIPING_PING_WEBADDRESS_WIN_ID,
        MMIPING_WAIT_WIN_ID,
        MMIPING_SHOW_PACKET_WIN_ID,
        MMIPING_PING_SELECT_MODE_WIN_ID,
        MMIPING_MAX_WIN_ID
} PING_WINDOW_ID_E;

#undef WIN_ID_DEF

//pbsync account menu id
typedef enum {
    MMIPING_MENU_ID_START = MMIPING_MAX_WIN_ID,
    MMIPING_MENU_ID_WEB_ADDRESS,
    MMIPING_MENU_ID_IP_TYPE,
    MMIPING_MENU_ID_COUNT,
    MMIPING_MENU_ID_MAX
} MMIPING_MENU_ID_E;

// control ID
typedef enum {
    MMIPING_CTRL_ID_ID_START = MMIPING_MENU_ID_MAX,
    MMIPING_CTRL_ID_SETTING_INFO,
    MMIPING_CTRL_ID_COUNT,
    MMIPING_CTRL_ID_IPTYPE,
    MMIPING_CTRL_ID_WEBADDRESS,
    MMIPING_CTRL_ID_SHOW_PACKET_INFO,
    MMIPING_CTRL_ID_SELECT_MODE_INFO,
    MMIPING_MAX_CTRL_ID

} MMIPING_CONTROL_ID_E;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
#endif
