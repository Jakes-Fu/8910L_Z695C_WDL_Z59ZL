/******************************************************************************
 * This file is for 128 MB, large page NAND flash.
 *
 * This type of NAND flash has total 1024 blocks. But 20 blocks are reserved by
 * BML as replaced blocks and another 3 blocks as meta blocks. So the total
 * available blocks are 1001. The sum of blocks occupied by all partitions should
 * NOT exceed this number.
 ******************************************************************************/
#include "NAND_PartitionId.h"
#include "NAND_PartitionCfg.h"
#include "sci_ftl_api.h"

/* commong .nNumOfPartEntry */
#define BASE_PART_NUM          (10)

/*vm ralated partition */
#ifdef VM_SUPPORT
#define VM_PART_NUM            (1)
#else
#define VM_PART_NUM            (0)
#endif

/*dev mangge related partition */
#ifdef DEV_MANAGE_SUPPORT
#define DM_PART_NUM        (5)
#else
#define DM_PART_NUM        (0)
#endif

#ifdef POWER_OFF_PROTECT_SUPPORT
	#define PROTECT_PART_NUM		(1)
#else
	#define PROTECT_PART_NUM		(0)
#endif
/*dev mangge related partition */
#ifdef USB_VCOM_AUTORUN_SUPPORT
#define ROMDISK_PART_NUM   (1)
#else
#define ROMDISK_PART_NUM   (0)
#endif


#define PART_NUM    (BASE_PART_NUM + VM_PART_NUM + DM_PART_NUM + PROTECT_PART_NUM + ROMDISK_PART_NUM)                      /* XSRPartI.nNumOfPartEntry */

/******************************************************************************
 * Partition Size in BLOCK
 * !!!Please refer to "SCXXXX Customization Configuration User Guide(Driver).doc"
 * for detailed explanation
 ******************************************************************************/
#define NBL_SIZE					8           /* 1 block,     128KB */
#ifdef MODEM_PLATFORM
#define OSB_SIZE                512          /* 64 blocks,   8MB  */
#else
#define OSB_SIZE                256          /* 32 blocks,   4MB  */
#endif

#ifdef VM_SUPPORT
#define OSC_SIZE					128          /* 16 blocks,   2MB  */
#else
#define OSC_SIZE					0           /* 0 blocks,    0MB  */
#endif

#define OSA_SIZE					1280         /* 160 blocks,  20MB */
#define DL_NV_SIZE					8           /* 1 block,     128KB */
#define CALIPARAM_SIZE				8           /* 1 blocks,    128KB */

#ifdef DEV_MANAGE_SUPPORT
#define UA_SIZE						16           /* 2 block,     256KB*/
#define UBB_SIZE					80          /* 10 block,    1280KB*/
#define DU_SIZE						160          /* 20 block,    1280KB*/
#define UAEB_SIZE					160          /* 20 block,    1280KB*/
#define UF_SIZE						8           /* 1 block      128KB*/
#else
#define UA_SIZE						0           /* 2 block,     256KB*/
#define UBB_SIZE					0           /* 10 block,    1280KB*/
#define DU_SIZE						0           /* 10 block,    1280KB*/
#define UAEB_SIZE					0           /* 10 block,    1280KB*/
#define UF_SIZE						0           /* 1 block      128KB*/
#endif
#define MMI_RESOURCE_SIZE			640          /* 1 blocks,    128KB */
#define RUNNING_NV_SIZE			384          /* 48 blocks,   6M */
#define HIDDEN_FS_SIZE				2560         /* 40 blocks,   5MB */
#define DSP_CODE_SIZE				248          /* 32 blocks,   4MB */

#ifdef POWER_OFF_PROTECT_SUPPORT
#define PROTECT_NV_SIZE				DL_NV_SIZE	/* 1 blocks,the same as download nv*/
#else
#define PROTECT_NV_SIZE				0
#endif
#ifdef USB_VCOM_AUTORUN_SUPPORT
#define ROMDISK_FS_SIZE			960         /* 120 blocks,  5MB */
#else
#define ROMDISK_FS_SIZE			0           /* 0 blocks,    5MB */
#endif

#define FS_SIZE						NAND_REMAIN_AREA
/******************************************************************************
 * Partition Offset in BLOCK
 ******************************************************************************/
#define NBL_OFFSET					(0)
#define OSB_OFFSET					(NBL_OFFSET + NBL_SIZE)
#define OSC_OFFSET					(OSB_OFFSET + OSB_SIZE)
#define OSA_OFFSET					(OSC_OFFSET + OSC_SIZE)
#define DL_NV_OFFSET				(OSA_OFFSET + OSA_SIZE)
#define CALIPARAM_OFFSET			(DL_NV_OFFSET + DL_NV_SIZE)
#define MMIRESOURCE_OFFSET		(CALIPARAM_OFFSET + CALIPARAM_SIZE)
#define RUNNING_NV_OFFSET			(MMIRESOURCE_OFFSET + MMI_RESOURCE_SIZE)//MUST BE EVEN!!!!
#define UA_OFFSET					(RUNNING_NV_OFFSET + RUNNING_NV_SIZE)
#define UBB_OFFSET					(UA_OFFSET + UA_SIZE)
#define DU_OFFSET					(UBB_OFFSET + UBB_SIZE)
#define UAEB_OFFSET				(DU_OFFSET + DU_SIZE)
#define UF_OFFSET					(UAEB_OFFSET + UAEB_SIZE)
#define HIDDEN_FS_OFFSET			(UF_OFFSET + UF_SIZE)
#define DSP_CODE_OFFSET			(HIDDEN_FS_OFFSET + HIDDEN_FS_SIZE)
#define PROTECT_NV_OFFSET			(DSP_CODE_OFFSET+DSP_CODE_SIZE)
#define ROMDISK_FS_OFFSET			(PROTECT_NV_OFFSET+PROTECT_NV_SIZE)
#define FS_OFFSET					(ROMDISK_FS_OFFSET + ROMDISK_FS_SIZE)

/******************************************************************************
 * Partition Attribute
 ******************************************************************************/
#define NBL_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define OSB_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define OSC_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define OSA_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define DL_NV_ATTRI				SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define CALIPARAM_ATTRI			SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define MMIRESOURCE_ATTRI			SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define RUNNING_NV_ATTRI			SCI_FTL_MODULE_SCT|SCI_FTL_ATTR_RW
#define UA_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define UBB_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define DU_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RW
#define UAEB_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define UF_ATTRI					SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RW
#define HIDDEN_FS_ATTRI			SCI_FTL_MODULE_SCT|SCI_FTL_ATTR_RW
#define DSP_CODE_ATTRI				SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define PROTECT_NV_ATTRI			SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define ROMDISK_FS_ATTRI			SCI_FTL_MODULE_BLK|SCI_FTL_ATTR_RO
#define FS_ATTRI					SCI_FTL_MODULE_SCT|SCI_FTL_ATTR_RW

const NAND_PARTITION PartI_16k =
{
    {PART_NUM},
    {
        {SCI_PARTITION_NBL,           NBL_ATTRI,         NBL_OFFSET,         NBL_SIZE          },
        {SCI_PARTITION_OSB,           OSB_ATTRI,         OSB_OFFSET,         OSB_SIZE          },
#ifdef VM_SUPPORT
        {SCI_PARTITION_OSC,           OSC_ATTRI,         OSC_OFFSET,         OSC_SIZE          },
#endif
        {SCI_PARTITION_OSA,           OSA_ATTRI,         OSA_OFFSET,         OSA_SIZE          },
        {SCI_PARTITION_DOWNLOADED_NV, DL_NV_ATTRI,       DL_NV_OFFSET,       DL_NV_SIZE        },
        {SCI_PARTITION_CALIPARAM,     CALIPARAM_ATTRI,   CALIPARAM_OFFSET,   CALIPARAM_SIZE    },
        {SCI_PARTITION_MMI_RESOURCE,  MMIRESOURCE_ATTRI, MMIRESOURCE_OFFSET, MMI_RESOURCE_SIZE },
        {SCI_PARTITION_NV,            RUNNING_NV_ATTRI,  RUNNING_NV_OFFSET,  RUNNING_NV_SIZE   },
        {SCI_PARTITION_HIDDEN_FILESYSTEM, HIDDEN_FS_ATTRI, HIDDEN_FS_OFFSET, HIDDEN_FS_SIZE    },
        {SCI_PARTITION_DSPCODE,       DSP_CODE_ATTRI,    DSP_CODE_OFFSET,    DSP_CODE_SIZE     },
#ifdef POWER_OFF_PROTECT_SUPPORT
		{SCI_PARTITION_PROTECT_NV,    PROTECT_NV_ATTRI,  PROTECT_NV_OFFSET,  PROTECT_NV_SIZE},
#endif
#ifdef USB_VCOM_AUTORUN_SUPPORT
        {SCI_PARTITION_ROMDISK_FILESYSTEM, ROMDISK_FS_ATTRI, ROMDISK_FS_OFFSET, ROMDISK_FS_SIZE},
#endif

#ifdef DEV_MANAGE_SUPPORT
        {SCI_PARTITION_UA,            UA_ATTRI,          UA_OFFSET,          UA_SIZE           },
        {SCI_PARTITION_UBB,           UBB_ATTRI,         UBB_OFFSET,         UBB_SIZE          },
        {SCI_PARTITION_DU,            DU_ATTRI,          DU_OFFSET,          DU_SIZE           },
        {SCI_PARTITION_UAEB,          UAEB_ATTRI,        UAEB_OFFSET,        UAEB_SIZE         },
        {SCI_PARTITION_UPDATED_NV,    UF_ATTRI,          UF_OFFSET,          UF_SIZE           },
#endif
        {SCI_PARTITION_FILESYSTEM,    FS_ATTRI,          FS_OFFSET,          FS_SIZE           },
    }
};


