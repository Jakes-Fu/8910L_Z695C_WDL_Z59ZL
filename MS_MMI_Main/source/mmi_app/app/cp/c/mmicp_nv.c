/*************************************************************************
 ** File Name:      mmicp_nv.c                                          *
 ** Author:         fu.ruilin                                           *
 ** Date:           2015/08/24                                           *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2015/08/24     fu.ruilin        Create.                              *
*************************************************************************/


#define _MMICP_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmicp_nv.h"
#include "mmi_modu_main.h"
#include "mmicp_export.h"

/*the length of set nv*/
const uint16 cp_nv_len[] =
{
    sizeof(PREFERRED_SIM_E),
};

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: fu.ruilin 
//	Note:
/*****************************************************************************/
PUBLIC void MMICP_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_CP, cp_nv_len, ARR_SIZE(cp_nv_len));
}

/*****************************************************************************/
// 	Description : cp nv size
//	Global resource dependence : none
//  Author: fu.ruilin 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICP_GetNvSize(void)
{
    uint32 allcount = 0;
    uint16 i = 0;
    
    //sum cp_nv_len
    for (i = 0;i<ARR_SIZE(cp_nv_len);i++)
    {
        allcount += cp_nv_len[i];
    }
    return allcount;
}