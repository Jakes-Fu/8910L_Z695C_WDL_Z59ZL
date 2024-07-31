/*************************************************************************
 ** File Name:      hello_nv.c                                           *
 ** Author:         xiaoqing.lu                                          *
 ** Date:           04/09/2009                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 04/09/2009     xiaoqing.lu      Create.                              *
*************************************************************************/
#include "std_header.h"

#ifdef MMIPB_SYNC_WITH_O365

#include "sci_types.h"
#include "mmisrv_contactsync_nv.h"
#include "mmisrv_contactsync_main.h"
#include "mmi_modu_main.h"


/*the length of NPS nv*/
const uint16 pbsync_nv_len[] =
{
    	sizeof(uint16),  // MMINPSNV_TIMER_COUNT
	sizeof(BOOLEAN), // MMINPSNV_IS_TIMER_ARRIVE
	sizeof(BOOLEAN), // MMINPSNV_IS_ALREADY_PROMPT
};

/*****************************************************************************/
// 	Description : register hello module nv len and max item
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMI_RegPBSYNCNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_CONTACTSYNC, pbsync_nv_len, sizeof(pbsync_nv_len)/sizeof(uint16));
}
#endif //MMIPB_SYNC_WITH_O365

