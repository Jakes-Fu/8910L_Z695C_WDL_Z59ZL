/******************************************************************************
 * This file is for 64 MB, small page NAND flash.
 *
 * This type of NAND flash has total 4096 blocks. But 70 blocks are reserved by
 * BML as replaced blocks and another 3 blocks as meta blocks. So the total
 * available blocks are 4023. The sum of blocks occupied by all partitions should
 * NOT exceed this number.
 ******************************************************************************/
#include "NAND_PartitionId.h"
#include "NAND_PartitionCfg.h"
#include "sci_ftl_api.h"

#define BASE_PART_NUM				(9)
#ifdef POWER_OFF_PROTECT_SUPPORT
	#define PROTECT_PART_NUM		(1)
#else
	#define PROTECT_PART_NUM		(0)
#endif
#ifdef USB_VCOM_AUTORUN_SUPPORT
	#define ROMDISK_PART_NUM		(1)        
#else
	#define ROMDISK_PART_NUM		(0)        
#endif

#define PART_NUM		(BASE_PART_NUM + PROTECT_PART_NUM + ROMDISK_PART_NUM)

/******************************************************************************
 * Partition Size in BLOCK
 * !!!Please refer to "SCXXXX Customization Configuration User Guide(Driver).doc"
 * for detailed explanation
 ******************************************************************************/
#define NBL_SIZE					8                     /* 1 block,     16KB */
#define OSB_SIZE					193                   /* 192 blocks,  3MB */
#define OSA_SIZE					1153                  /* 1472 blocks,     18MB */
#define DL_NV_SIZE					4                     /* 1 block,     16KB */
#define CALIPARAM_SIZE				3                     /* 3 blocks,    48KB */
#define MMI_RESOURCE_SIZE			1985                  /* 1985 blocks,  31MB */
#ifdef POWER_OFF_PROTECT_SUPPORT
#define PROTECT_NV_SIZE					DL_NV_SIZE	/* 1 blocks,the same as download nv*/
#else
#define PROTECT_NV_SIZE					0
#endif
#ifdef USB_VCOM_AUTORUN_SUPPORT
#define ROMDISK_FS_SIZE					120			/* 5MB   */
#else
#define ROMDISK_FS_SIZE					0			/* 5MB   */
#endif
#define RUNNING_NV_SIZE			192                   /* 192 blocks,  2M */
#define HIDDEN_FS_SIZE				256                   /* 256 blocks,   4MB */
#define FS_SIZE						NAND_REMAIN_AREA     /* 2031 blocks, 31.7MB */
/* Total 1 + 640 + 4 + 3 + 512 + 128 + 2415 + 320 = 4023 blocks */

/******************************************************************************
 * Partition Offset in BLOCK
 ******************************************************************************/
#define NBL_OFFSET					0
#define OSB_OFFSET					(NBL_OFFSET + NBL_SIZE)
#define OSA_OFFSET					(OSB_OFFSET + OSB_SIZE)
#define DL_NV_OFFSET				(OSA_OFFSET + OSA_SIZE)
#define CALIPARAM_OFFSET			(DL_NV_OFFSET + DL_NV_SIZE)
#define MMIRESOURCE_OFFSET		(CALIPARAM_OFFSET + CALIPARAM_SIZE)
#define PROTECT_NV_OFFSET			(MMIRESOURCE_OFFSET + MMI_RESOURCE_SIZE)
#define ROMDISK_FS_OFFSET			(PROTECT_NV_OFFSET+PROTECT_NV_SIZE)
#define RUNNING_NV_OFFSET			(ROMDISK_FS_OFFSET + ROMDISK_FS_SIZE)
#define HIDDEN_FS_OFFSET			(RUNNING_NV_OFFSET + RUNNING_NV_SIZE)
#define FS_OFFSET					(HIDDEN_FS_OFFSET + HIDDEN_FS_SIZE)

/******************************************************************************
 * Partition Attribute
 ******************************************************************************/
#define NBL_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define OSB_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define OSA_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define DL_NV_ATTRI				SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define CALIPARAM_ATTRI			SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define MMIRESOURCE_ATTRI			SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define PROTECT_NV_ATTRI			SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define ROMDISK_FS_ATTRI			SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define RUNNING_NV_ATTRI			SCI_FTL_MODULE_SCT|SCI_FTL_ATTR_RW
#define HIDDEN_FS_ATTRI			SCI_FTL_MODULE_SCT|SCI_FTL_ATTR_RW
#define FS_ATTRI					SCI_FTL_MODULE_SCT|SCI_FTL_ATTR_RW

const NAND_PARTITION PartI_16k =
{
    {PART_NUM},
    {
        {SCI_PARTITION_NBL,           NBL_ATTRI,         NBL_OFFSET,         NBL_SIZE},
        {SCI_PARTITION_OSB,           OSB_ATTRI,         OSB_OFFSET,         OSB_SIZE},
        {SCI_PARTITION_OSA,           OSA_ATTRI,         OSA_OFFSET,         OSA_SIZE},
        {SCI_PARTITION_DOWNLOADED_NV, DL_NV_ATTRI,       DL_NV_OFFSET,       DL_NV_SIZE},
        {SCI_PARTITION_CALIPARAM,     CALIPARAM_ATTRI,   CALIPARAM_OFFSET,   CALIPARAM_SIZE},
        {SCI_PARTITION_MMI_RESOURCE,  MMIRESOURCE_ATTRI, MMIRESOURCE_OFFSET, MMI_RESOURCE_SIZE},
#ifdef POWER_OFF_PROTECT_SUPPORT
		{SCI_PARTITION_PROTECT_NV,    PROTECT_NV_ATTRI,  PROTECT_NV_OFFSET, PROTECT_NV_SIZE},
#endif
#ifdef USB_VCOM_AUTORUN_SUPPORT
		{SCI_PARTITION_ROMDISK_FILESYSTEM, ROMDISK_FS_ATTRI, ROMDISK_FS_OFFSET, ROMDISK_FS_SIZE},
#endif
        {SCI_PARTITION_NV,            RUNNING_NV_ATTRI,  RUNNING_NV_OFFSET,  RUNNING_NV_SIZE},
        {SCI_PARTITION_FILESYSTEM,    FS_ATTRI,          FS_OFFSET,          FS_SIZE},
        {SCI_PARTITION_HIDDEN_FILESYSTEM, HIDDEN_FS_ATTRI, HIDDEN_FS_OFFSET, HIDDEN_FS_SIZE},
    }
};
