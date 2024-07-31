#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_module.h"
#include "mmi_modu_main.h"
#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmicp_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmicp_id.def"
};

#undef WIN_ID_DEF

/*****************************************************************************/
//    Description : Register contorl panel IDs
//    Global resource dependence : none
//    Author: fu.ruilin
//    Note:
/*****************************************************************************/
PUBLIC void MMICP_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_CP, mmicp_id_name_arr);
}
