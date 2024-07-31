#include "mmi_app_kuro_trc.h"
#include "std_header.h"
#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmikur_nv.c                                               *
** Author:         cmzs                                                           *
** Date:           18/10/2007                                                *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 18/10/2007       cmzs       Create                                  *
******************************************************************************/

#define _MMIMP_NV_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmikur_internal.h"
#include "klrfunction.h"
#include "mmikur_nv.h"
#include "nvitem.h"
#include "mn_type.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*the length of kur nv*/
uint16 kur_nv_len[] =
{
    sizeof(MMIKUR_SETTINGS_INFO_T),    // MMINV_MP3_SET_INFO
    KURO_SYSDAT_NVSIZE,
    sizeof(uint16),
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register module nv len and max item
//  Global resource dependence : none
//  Author: cmzs
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_KURO, kur_nv_len, ARR_SIZE(kur_nv_len) + MMINV_MAX_KGC_RECORDS);
//    MMI_RegModuleNv(MMI_MODULE_KURO,kur_nv_len,sizeof(kur_nv_len)/sizeof(uint16));
}

/*****************************************************************************/
//  Description :read KGC infos in NV by MMI
//  Global resource dependence :  
//  Author:Cmzs
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKURO_NV_ReadKgcInfo(                                      //RETURN:
                              uint16                        rid,        //IN:
                              KGC_INFO  *item_ptr   //OUT:
                              )
{
    BOOLEAN                 ret_value = TRUE;
    uint16                  true_id   = 0;
    uint32                  m_item_id = MMINV_KURO_KGC_FIRST_ENTRY + rid;
    ERR_MNDATAMAG_CODE_E    nv_result = ERR_MNDATAMAG_NO_ERR;

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != item_ptr);
    if(PNULL == item_ptr)
	{
		return FALSE;
	}
    
    SCI_MEMSET(item_ptr, 0x00, sizeof (KGC_INFO));
    
    true_id   = MMI_GetTrueNVItemId(m_item_id);
    nv_result = MNNV_ReadItem(
        true_id, 
        sizeof (KGC_INFO),
        (void *)item_ptr
        ); 
    
    if (ERR_MNDATAMAG_NO_ERR == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }
    
    return (ret_value);
}


/*****************************************************************************/
//  Description :write Kuro Kgc Info in NV by MMI
//  Global resource dependence :  
//  Author:cmzs 
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIKURO_NV_WriteKgcInfo(                                         //RETURN:
                               BOOLEAN                  is_need_sync,   //IN:
                               uint16                   rid,            //IN:
                               KGC_INFO     *item_ptr       //IN:
                               )
{
    BOOLEAN                 ret_value = TRUE;
    BOOLEAN                 is_async  = !is_need_sync;
    uint16                  true_id   = 0;
    uint32                  m_item_id = MMINV_KURO_KGC_FIRST_ENTRY + rid;
    NVITEM_ERROR_E          nv_result = NVERR_NONE;
    
    true_id   = MMI_GetTrueNVItemId(m_item_id);
    
    // call EFS API
    nv_result = EFS_NvitemWrite(
                                true_id,
                                sizeof (KGC_INFO),
                                (void *)item_ptr,
                                is_async
                                ); 
    
    if (NVERR_NONE == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }
    
    //SCI_TRACE_LOW:"SMS: MMIAPISMS_NV_WriteSmsItem nv_result = %d, rid = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_NV_132_112_2_18_2_31_43_56,(uint8*)"dd", nv_result, rid);
    
    return (ret_value);
}   

/*****************************************************************************/
//  Description :Get Kgc info's Count From NV
//  Global resource dependence :  
//  Author:cmzs 
//  Note:
/*****************************************************************************/ 
PUBLIC uint16 MMIKURO_GetKgcInfoCount(void)
{
    uint16 nCount = 0;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_KURO_KGC_COUNT,&nCount,return_value);
    
    if(MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_KURO_KGC_COUNT,&nCount);
    }
    return nCount;
}

/*****************************************************************************/
//  Description :Get Kgc info
//  Global resource dependence :  
//  Author:cmzs 
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIKURO_GetKgcInfo(int16 nIndex,KGC_INFO *kgcinfo)
{
    return MMIKURO_NV_ReadKgcInfo(nIndex,kgcinfo);
}

/*****************************************************************************/
//  Description :Get Kgc info by sonid,if not found return -1 
//  Global resource dependence :  
//  Author:cmzs 
//  Note:
/*****************************************************************************/ 
PUBLIC int16 MMIKURO_FindKgcInfo(uint8 *songid,KGC_INFO *kgcinfo)
{
    uint16 nCount = MMIKURO_GetKgcInfoCount();
    KGC_INFO info;
    int16 foundindex = -1;
    int i = 0;
    for(i = 0;i < nCount; i++)
    {
        if(MMIKURO_NV_ReadKgcInfo(i,&info))
        {
            //SCI_TRACE_LOW:"------MMIKURO_NV_ReadKgcInfo:%s,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_NV_177_112_2_18_2_31_43_57,(uint8*)"sd",info.songid,info.playcount);
            if(songid[0] == info.songid[0] 
                &&songid[1] == info.songid[1] 
                &&songid[2] == info.songid[2] 
                &&songid[3] == info.songid[3] 
                &&songid[4] == info.songid[4] 
                &&songid[5] == info.songid[5] 
                &&songid[6] == info.songid[6] 
                &&songid[7] == info.songid[7] 
                &&songid[8] == info.songid[8] 
                &&songid[9] == info.songid[9] 
                &&songid[10] == info.songid[10] 
                &&songid[11] == info.songid[11] 
                &&songid[12] == info.songid[12] 
                &&songid[13] == info.songid[13] 
                &&songid[14] == info.songid[14] 
                &&songid[15] == info.songid[15] 
                &&songid[16] == info.songid[16] 
                &&songid[17] == info.songid[17] 
                &&songid[18] == info.songid[18] 
                &&songid[19] == info.songid[19] 
                &&songid[20] == info.songid[20] 
                &&songid[21] == info.songid[21] 
                &&songid[22] == info.songid[22] 
                &&songid[23] == info.songid[23] 
                &&songid[24] == info.songid[24] 
                &&songid[25] == info.songid[25] 
                &&songid[26] == info.songid[26] 
                &&songid[27] == info.songid[27] 
                &&songid[28] == info.songid[28] 
                &&songid[29] == info.songid[29] 
                &&songid[30] == info.songid[30] 
                &&songid[31] == info.songid[31] 
                )
            {
                SCI_MEMCPY(kgcinfo,&info,sizeof(KGC_INFO));
                foundindex = i;
                break;
            }
        }
    }
    return foundindex;
}

/*****************************************************************************/
//  Description :Add Kgc info
//  Global resource dependence :  
//  Author:cmzs 
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIKURO_AddKgcInfo(KGC_INFO *kgcinfo)
{
    uint16 nCount = MMIKURO_GetKgcInfoCount();
    BOOLEAN bResult = FALSE;
    if(nCount >= MMINV_MAX_KGC_RECORDS)
    {
        nCount = 0;
    }
    
    bResult = MMIKURO_NV_WriteKgcInfo(TRUE,nCount++,kgcinfo);
    if(bResult)
        MMINV_WRITE(MMINV_KURO_KGC_COUNT,&nCount);
    return bResult;
}

/*****************************************************************************/
//  Description :Change Kgc info
//  Global resource dependence :  
//  Author:cmzs 
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIKURO_ChangeKgcInfo(uint16 nIndex,KGC_INFO *kgcinfo)
{
    BOOLEAN bResult = FALSE;
    if(nIndex >= MMINV_MAX_KGC_RECORDS)
    {
        return FALSE;
    }
    
    bResult = MMIKURO_NV_WriteKgcInfo(TRUE,nIndex,kgcinfo);
    return bResult;
}
/*****************************************************************************/
//  Description : clear kgc info fromnv
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_ClearKgcCount(void)
{
    uint16 nCount = 0;
    MMINV_WRITE(MMINV_KURO_KGC_COUNT,&nCount);
}

/*****************************************************************************/
//     Description : get sum size of all kur nv itme
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetSumNvSize(void)
{
    uint16 count = 0;
    uint32 kur_nv_sum_size = 0;
    /* get kuro settings nv size */
    for(count = 0; count < ARR_SIZE(kur_nv_len); count ++)
    {
        kur_nv_sum_size += kur_nv_len[count];
    }

    /* get kuro all items nv size */
    kur_nv_sum_size += MMINV_MAX_KGC_RECORDS*(sizeof(KGC_INFO));

    return kur_nv_sum_size;
}
#endif
