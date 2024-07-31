#include "ms_ref_fota_trc.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "sci_types.h"
#include "RB_ImageUpdate.h"
#include "RbErrors.h"
#include "log.h"
#include "sprdftl_ua.h"
#include "master_img_gen.h"
#include "nv_item_id.h"


#include "Nand_partitionID.h"
#include "nand_spec.h"
#include "sci_ftl_api.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


#define PUBLIC
#define LOCAL           static

/**
#	search nvitem mode: 
#					0: search specified nv id in nv partition
#					1: search the first nv id in nv partition
#					2: search the nv after the specified nv id in nv partition
**/
#define		SEARCH_BY_NVID		0
#define		SEARCH_FIRST_NVID		1
#define		SEARCH_NEXT_NVID		2

/**
#			define the max buffer used to update nv parameters.
**/
#define		MAX_FIXNV_LENGTH		(64*1024)

#define		INVALID_NVID		(0xFFFF)


#define RAND_TO_UINT32( _addr ) \
    if( (_addr) & 0x3 ){ _addr += 0x4 - ((_addr) & 0x3); }


extern uint32 g_SectorsPerBlk_Ua;
extern uint32 g_Sector_Size;
static const uint32 BOOTPARAM_MAGIC_BIG = 0x53544250; /* 'STBP' */
static const uint32 BOOTPARAM_MAGIC_LITTLE = 0x50425453; /* 'PBTS' */ 

extern uint8 g_ua_mbuf[];
extern uint32 g_ua_mbuf_size;



/*
#
# list the reserve nv item during update...
#
*/

static uint16	backup_nvitem_count=0;
static uint16	backup_nvitems[]={
	NV_BT_CONFIG,                       // 0x191,   NV_BT_ADDRESS		
	NV_IMEI1,                           // 0x179,   NV_IMEI1
	NV_IMEI,                            // 0x5,    NV_IMEI
	NV_L1_CALIBRATION_PARAMETER,        // 0x2,    SM_Calibration
	NV_L1_CALIBRATION_PARAMETER_TD,     //  0x516, TD_Calibration
	INVALID_NVID,
};

/*
#
# define buffer used to create new fixnv.
#
*/




typedef struct 
{
    uint16 nvitem_id;
    uint16 nv_length;
    uint32 nv_offset;
} FOTA_NVITEM_SCAN_T;

#define     INVALID_NVLENGTH  (0xFFFF)

#define NV_BIN_TIMESTAMP_SIZE       4

#define BACKUP_NV_COUNT   sizeof(backup_nvitems) / sizeof(backup_nvitems[0])

FOTA_NVITEM_SCAN_T g_download_nv_map[BACKUP_NV_COUNT] = {0};
FOTA_NVITEM_SCAN_T g_update_nv_map[BACKUP_NV_COUNT] = {0};

UAPartEntry_T *g_download_nv_part_entry = NULL;
UAPartEntry_T *g_updated_nv_part_entry = NULL;

#define     NV_BUFFER_LEN       (2*128*1024)

__align(8) uint8 g_DownloadNV_Buffer[NV_BUFFER_LEN] = {0};
__align(8) uint8 g_UpdateNV_Buffer[NV_BUFFER_LEN] = {0};


extern uint32  GetUAPartIEntry(uint32 nPartID, UAPartEntry_T **ppEntry);








LOCAL uint32 _IsBackupItems(uint16 nvitem_id)
{
    uint32 nv_map_item_count = 0;


    while(nv_map_item_count < BACKUP_NV_COUNT)
    {
        if (nvitem_id == backup_nvitems[nv_map_item_count])
        {
            /* find backup nvitem */
            return TRUE;
        }

        nv_map_item_count++;
    }

    /* Not find */
    return FALSE;
}



LOCAL uint32 _Scan_NV_Buffer_For_Backup(uint8 *pNVBuf, 
                                uint32 NVBuf_size, FOTA_NVITEM_SCAN_T *nv_map )
{
    uint32 ret = TRUE;
    uint32 nBuf_offset = 0;
    uint32 nBuf_record_offset = 0;
    uint32 nv_map_item_count = 0;
    uint16 nvitem_id = 0;
    uint16 nv_length = 0;

    if ( (NULL == pNVBuf) || (NULL == nv_map))
    {
        //UA_TRACE_ERR:"_Scan_NV_Buffer_For_Backup: Input para error.\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_150_112_2_18_1_31_25_30,(uint8*)"");
        return FALSE;
    }

    /* Read offset increase time stamp size */
    nBuf_offset += NV_BIN_TIMESTAMP_SIZE;

    /* scan nv buffer until go to nv bin end */
    do
    {
        nBuf_record_offset = nBuf_offset;
        
        /* read nv_item id and nv_item length */
        nvitem_id = *(uint16 *)&pNVBuf[nBuf_offset];
        nBuf_offset += sizeof(nvitem_id);
        nv_length = *(uint16 *)&pNVBuf[nBuf_offset];
        nBuf_offset += sizeof(nv_length);

        if ( (INVALID_NVID == nvitem_id) || (INVALID_NVLENGTH == nv_length))
        {
            /* Search end */
            break;
        }

        if (_IsBackupItems(nvitem_id))
        {
            if(nv_map_item_count < BACKUP_NV_COUNT)
            {
                nv_map[nv_map_item_count].nvitem_id = nvitem_id;
                nv_map[nv_map_item_count].nv_length = nv_length;
                nv_map[nv_map_item_count].nv_offset = nBuf_record_offset;

                /* record map count increase */
                nv_map_item_count++;
            }

        }

        if (nv_map_item_count >= BACKUP_NV_COUNT)
        {
            /* no need search */
            break;
        }

        /* Go to next nv item, nv_offset increased by nv_item length */
        nBuf_offset += nv_length;

        /* offset must align to 4 bytes */
        RAND_TO_UINT32(nBuf_offset);

    } while (nBuf_offset < NVBuf_size);

    return TRUE;
    

}



/*****************************************************************************/
// 	Description : initialize the two nv partition entry.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
uint32 Load_FixnvEntry()
{

    int           ret;
    
    /* Open Fixed NV handle */
    ret = GetUAPartIEntry(SCI_PARTITION_DOWNLOADED_NV, &g_download_nv_part_entry);
    if ((!ret) || (NULL == g_download_nv_part_entry))
    {
        //UA_TRACE_ERR:"Load_FixnvEntry: Open FIXNV partition error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_222_112_2_18_1_31_26_31,(uint8*)"d",ret);
        return FALSE;
    } 


    UA_TRACE(("FIXNV_Partition: start blkno  %d  block count %d\n",
            g_download_nv_part_entry->n1stVbn,
            g_download_nv_part_entry->nNumOfBlks));

    /* Open Update NV partition handle */

    ret = GetUAPartIEntry(SCI_PARTITION_UPDATED_NV, &g_updated_nv_part_entry);
    if ((!ret) || (NULL == g_updated_nv_part_entry))
    {
        //UA_TRACE_ERR:"Load_FixnvEntry: Open Updated partition error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_236_112_2_18_1_31_26_32,(uint8*)"d",ret);
        return FALSE;
    } 
    
    UA_TRACE(("UPDATEDNV_Partition: start blkno  %d  block count %d\n",
        g_updated_nv_part_entry->n1stVbn,
        g_updated_nv_part_entry->nNumOfBlks));

    return TRUE;
    

}

/*****************************************************************************/
// 	Description : compose the new nv parameters and save updated nv parameter.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
uint32  update_fixnv_image(void)
{


    uint32 ret = TRUE;
    uint32 nReadSct_Idx = 0;
    uint32 nReadSct_Num = 0;
    uint32 nWSct_Idx = 0;
    uint32 nWSct_Num = 0;

    uint32 nDownload_Idx = 0;
    uint32 nUpdate_Idx = 0;
    uint16 nvitem_id = 0;
    uint16 nv_length = 0;
    uint32 nv_offset = 0;

    void *dst, *src;
    uint32 size;

    SCI_FTL_PARTITION_INFO partInfo;


    /* 1st Read fixed NV and update NV to buffer */
    if((NULL == g_download_nv_part_entry) || (NULL == g_updated_nv_part_entry))
    {
        ret = Load_FixnvEntry();
        if (!ret)
        {
            //UA_TRACE_ERR:"update_fixnv_image: Can not open fixed or update nv partition.\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_281_112_2_18_1_31_26_33,(uint8*)"");
            return FALSE;
        }
    }

    nReadSct_Idx = 0;
    nReadSct_Num = g_download_nv_part_entry->nNumOfBlks * g_SectorsPerBlk_Ua;

    /* check buffer size */
    if(nReadSct_Num*g_Sector_Size> sizeof(g_DownloadNV_Buffer))
    {
        //UA_TRACE_ERR:"update_fixnv_image: Fixed NV size more than g_DownloadNV_Buffer\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_292_112_2_18_1_31_26_34,(uint8*)"");
        return FALSE;
    }

    memset((void *) g_DownloadNV_Buffer, 0xFF, sizeof(g_DownloadNV_Buffer));
    /* Read fixed nv to buffer */
    ret = SCI_FTL_Read(g_download_nv_part_entry->ftlHandle, nReadSct_Idx, 
                    nReadSct_Num, g_DownloadNV_Buffer);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"update_fixnv_image: Read fixed nv partition error(0x%x) \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_302_112_2_18_1_31_26_35,(uint8*)"d", ret);
        return FALSE;
    }
    
    nReadSct_Idx = 0;
    nReadSct_Num = g_updated_nv_part_entry->nNumOfBlks * g_SectorsPerBlk_Ua;

    /* check buffer size */
    if(nReadSct_Num*g_Sector_Size > sizeof(g_UpdateNV_Buffer))
    {
        //UA_TRACE_ERR:"update_fixnv_image: Update NV size more than g_UpdateNV_Buffer\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_312_112_2_18_1_31_26_36,(uint8*)"");
        return FALSE;
    }

    memset((void *) g_UpdateNV_Buffer, 0xFF, sizeof(g_UpdateNV_Buffer));
    /* Read update nv to buffer */
    ret = SCI_FTL_Read(g_updated_nv_part_entry->ftlHandle, nReadSct_Idx, nReadSct_Num, 
                     g_UpdateNV_Buffer);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"update_fixnv_image: Read update nv partition error(0x%x) \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_322_112_2_18_1_31_26_37,(uint8*)"d", ret);
        return FALSE;
    }

    
    /* 2nd Search backup items' offset and size in the fix_nv buffer */

    ret = _Scan_NV_Buffer_For_Backup(g_DownloadNV_Buffer,
                    sizeof(g_DownloadNV_Buffer), g_download_nv_map);
    if (!ret)
    {
        //UA_TRACE_ERR:"update_fixnv_image: Scan fixed nv buffer error\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_333_112_2_18_1_31_26_38,(uint8*)"");
        return FALSE;
    }

    /* 3rd Search backup items' offset and size in the update_nv buffer */
    ret = _Scan_NV_Buffer_For_Backup(g_UpdateNV_Buffer, 
                    sizeof(g_UpdateNV_Buffer),g_update_nv_map);
    if (!ret)
    {
        //UA_TRACE_ERR:"update_fixnv_image: Scan update nv buffer error\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_342_112_2_18_1_31_26_39,(uint8*)"");
        return FALSE;
    }
    
    /* 4th Copy backup items from fixed_nv buffer to update_nv buffer */
    nDownload_Idx = 0;
    while(nDownload_Idx < BACKUP_NV_COUNT)
    {
        nvitem_id = g_download_nv_map[nDownload_Idx].nvitem_id;
        nv_length = g_download_nv_map[nDownload_Idx].nv_length;
        nv_offset = g_download_nv_map[nDownload_Idx].nv_offset;

        for(nUpdate_Idx = 0; nUpdate_Idx<BACKUP_NV_COUNT; nUpdate_Idx++)
        {
            if (nvitem_id == g_update_nv_map[nUpdate_Idx].nvitem_id)
            {
                break;
            }
        }

        /* if not find nvitem info in update nv map */
        if (nUpdate_Idx >= BACKUP_NV_COUNT)
        {
            //UA_TRACE_ERR:"update_fixnv_image: Copy backup items error.\r\n "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_365_112_2_18_1_31_26_40,(uint8*)"");
            return FALSE;
        }

        /* Check download nv and update nv length */
        if (nv_length != g_update_nv_map[nUpdate_Idx].nv_length)
        {
            //UA_TRACE_ERR:"update_fixnv_image: Download NV and Update NV incompatible.\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_372_112_2_18_1_31_26_41,(uint8*)"");
            return FALSE;
        }

        /* Copy backup item from download to update nv buffer */
        dst = (void *)&g_UpdateNV_Buffer[g_update_nv_map[nUpdate_Idx].nv_offset];
        src = (void *)&g_DownloadNV_Buffer[g_download_nv_map[nDownload_Idx].nv_offset];
        size = g_update_nv_map[nUpdate_Idx].nv_length;
        memcpy(dst, src, size);

        /* get next backup nv item */
        nDownload_Idx++;
        
    }

    /* 5th. Save update_nv buffer to fixed NV partition */

    /* erase whole fixed nv partition */
    ret = SCI_FTL_GetPartInfo(g_download_nv_part_entry->ftlHandle, &partInfo);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"update_fixnv_image: Get download nv partition info error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_393_112_2_18_1_31_26_42,(uint8*)"d", ret);
        return FALSE;
    }
    
    ret = SCI_FTL_Erase(g_download_nv_part_entry->ftlHandle, 0, 
                partInfo.sctTotalNum);
    if(SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"update_fixnv_image: Erase download nv error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_401_112_2_18_1_31_26_43,(uint8*)"d", ret);
        return FALSE;
    }
    
    nWSct_Idx = 0;
    nWSct_Num = g_download_nv_part_entry->nNumOfBlks * g_SectorsPerBlk_Ua;

    ret = SCI_FTL_Write(g_download_nv_part_entry->ftlHandle, nWSct_Idx, 
                nWSct_Num, g_UpdateNV_Buffer);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"update_fixnv_image: Write download nv error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_412_112_2_18_1_31_26_44,(uint8*)"d", ret);
        return FALSE;
    }

    return TRUE;

}

/*****************************************************************************/
// 	Description : compare old version and new version.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
uint32	erase_running_nv_partition(void)
{


    uint32 ret = SCI_FTL_ERROR_NONE;
    UAPartEntry_T       *pUaEntry = NULL;
    SCI_FTL_PARTITION_INFO uaPartInfo;
    uint32 i;


    /* Open running nv partition */
    ret = GetUAPartIEntry(SCI_PARTITION_NV, &pUaEntry);
    
    if ((!ret) || (NULL == pUaEntry))
    {
        //UA_TRACE_ERR:"erase_running_nv_partition: Failed to open running nv partition, ret(0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_439_112_2_18_1_31_26_45,(uint8*)"d",ret);
        return FALSE;
    }

    /* Erase all running nv partition */
    ret = SCI_FTL_GetPartInfo(pUaEntry->ftlHandle, &uaPartInfo);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"erase_running_nv_partition: Get running nv parition info err (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_447_112_2_18_1_31_26_46,(uint8*)"d", ret);
		return FALSE;
    }
    ret = SCI_FTL_Erase(pUaEntry->ftlHandle, 0, uaPartInfo.sctTotalNum);
    if (SCI_FTL_ERROR_NONE != ret)
    {
		//UA_TRACE_ERR:"erase_running_nv_partition: Failed to erase running nv parition (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_453_112_2_18_1_31_26_47,(uint8*)"d", ret);
		return FALSE;
    }


    /* we need write 0xFF to nand even if not any data, according to jason's VBM/VSM required */

    memset(g_ua_mbuf, 0xFF, g_ua_mbuf_size);
    for (i=0; i<uaPartInfo.sctTotalNum;i++)
    {
        ret = SCI_FTL_Write(pUaEntry->ftlHandle, i, 1, g_ua_mbuf);
    if (SCI_FTL_ERROR_NONE != ret)
    {
		//UA_TRACE_ERR:"erase_running_nv_partition: Failed to write 0xFF into running nv parition (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_466_112_2_18_1_31_26_48,(uint8*)"d", ret);
		return FALSE;
    }
    }



    return TRUE;

}


/*****************************************************************************/
// 	Description : compare old version and new version.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
uint32 Update_Kernel_Head(uint8 *buffer,uint32 length)
{

    uint32  ret = SCI_FTL_ERROR_NONE;
    uint32 nReadBlk_Idx = 0;
    uint32 nReadSct_Idx = 0;

    uint32 *pTmpData = NULL;
    uint32 i = 0;
    uint32 bFound = FALSE;
    uint32 search_size_limit = 0;
    uint32 nBlkWriteAddr = 0;
    
    UAPartEntry_T       *pUaEntry = NULL;
    SCI_FTL_PARTITION_INFO uaPartInfo;


    /* Open running nv partition */
    ret = GetUAPartIEntry(SCI_PARTITION_OSB, &pUaEntry);
    
    if ((!ret) || (NULL == pUaEntry))
    {
        //UA_TRACE_ERR:"Update_Kernel_Head: Open SCI_PARTITION_OSB error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_504_112_2_18_1_31_26_49,(uint8*)"d",ret);
        return FALSE;
    }

    /* Read data from OSB and search "STBP" signature */
    do
    {
        memset((void *)g_ua_mbuf, 0xFF, g_ua_mbuf_size);
    
        nReadSct_Idx = nReadBlk_Idx * g_SectorsPerBlk_Ua;
        /* read one block each time */
        ret = SCI_FTL_Read(pUaEntry->ftlHandle, nReadSct_Idx, g_SectorsPerBlk_Ua, g_ua_mbuf);
        if(SCI_FTL_ERROR_NONE != ret)
        {
            //UA_TRACE_ERR:"Update_Kernel_Head: Read block(%d) error.\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_518_112_2_18_1_31_26_50,(uint8*)"d", nReadBlk_Idx);
            return FALSE;
        }

        /* Search signature in the data */
        pTmpData = (uint32 *)&g_ua_mbuf[0];
        i = 0;
        search_size_limit = g_ua_mbuf_size / sizeof(uint32) ;
        do
        {
            if ((BOOTPARAM_MAGIC_BIG == *(pTmpData+i)) || 
                 (BOOTPARAM_MAGIC_LITTLE == *(pTmpData+i))
                )
            {
                bFound = TRUE;
            }
            else
            {
                i++;
            }
        
        }while ((!bFound) && (i<search_size_limit)) ;

        /* read next block */
        nReadBlk_Idx++;
    } while((!bFound) && (nReadBlk_Idx<pUaEntry->nNumOfBlks));


    /* Write kernel header into block */
    if(bFound)
    {
    	nReadBlk_Idx--;  //  if found, nReadBlk_Idx should decreased one because (nReadBlk_Idx++ ) error
        
    	memcpy((void *)(g_ua_mbuf + i*sizeof(uint32)), buffer, length);
    	nBlkWriteAddr = (nReadBlk_Idx + pUaEntry->n1stVbn) * g_SectorsPerBlk_Ua * g_Sector_Size;
    	ret = RB_WriteBlock(NULL,nBlkWriteAddr,g_ua_mbuf);
    	if (S_RB_SUCCESS == ret)
    	{
    	    return TRUE;
    	}
    	else
    	{
    	    return FALSE;
    	}
    }
    else
    {
        //UA_TRACE_ERR:"Update_Kernel_Head: Can not find \"STBP\" signature.\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_NV_UPDATE_565_112_2_18_1_31_26_51,(uint8*)"");
        return FALSE;
    }




}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

