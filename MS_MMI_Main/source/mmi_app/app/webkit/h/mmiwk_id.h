#ifndef _MMIWK_ID_H_
#define _MMIWK_ID_H_
#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C"
{
#endif

#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    MMIWK_WIN_ID_START = (MMI_MODULE_WK << 16),

    #include "mmiwk_id.def"

    MMIWK_MAX_WIN_ID
}MMIWK_WIN_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIWK_CTRL_ID_START = MMIWK_MAX_WIN_ID,
    MMIWK_MAIN_FORM_CTRL_ID,
    MMIWK_OPTION_FORM_CTRL_ID,
    MMIWK_OPTION_PORT_CTRL_ID,
    MMIWK_SEND_EDIT_CTRL_ID,
    MMIWK_TEXT_EDITBOX_CTRL_ID,
    MMIWK_ANIMATE_SEARCHING_CTRL_ID,
    MMIWK_POPUP_MENU_CTRL_ID,
    MMIWK_MAX_CTRL_ID
}MMIWK_CONTROL_ID_E;


PUBLIC void MMIWK_RegWinIdNameArr(void);
PUBLIC void MMIWK_RegMenuGroup(void);


#ifdef   __cplusplus
}
#endif
#endif
#endif //_MMIWK_ID_H_
