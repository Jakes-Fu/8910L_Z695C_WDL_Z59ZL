#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmiwk_id.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmiwk_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmiwk_id.def"    
};

#undef WIN_ID_DEF

PUBLIC void MMIWK_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_WK, (const uint8 **)mmiwk_id_name_arr);
}
#endif