/******************************************************************************
 ** File Name:     spiflash_cfg.c                                              *
 ** Description:														 	 *
 ** This file contains spiflash config info                                  *
 ** Author:         Fei.Zhang                                              	 *
 ** DATE:           01/18/2011                                               *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                              *
 ** 01/18/2011      Fei.Zhang       Create									 *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "spiflash.h"
//#include "emc_drv.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

#define _NULL 0xFF


static struct SPIFLASH_ExtCfg cfg_GD25LQ256 = {SPIFLASH_VOLTAGE_1V8, 0x02000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ128 = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ64B = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ32B = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25LQ16 = {SPIFLASH_VOLTAGE_1V8, 0x00200000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25Q64B = {SPIFLASH_VOLTAGE_3V, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_GD25Q32B = {SPIFLASH_VOLTAGE_3V, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_N25W128 = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};

static struct SPIFLASH_ExtCfg cfg_N25W064 = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_N25W032 = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};

static struct SPIFLASH_ExtCfg cfg_MX25U3235E = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_MX25U6435E = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_MX25U12835E = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q128FW = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q64FW = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q32FV = {SPIFLASH_VOLTAGE_3V, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q64FVSTIM = {SPIFLASH_VOLTAGE_3V, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q128FV = {SPIFLASH_VOLTAGE_3V, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_W25Q256FV = {SPIFLASH_VOLTAGE_3V, 0x02000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_F25D64QA = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25M128A = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25M64C = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25M64A = {SPIFLASH_VOLTAGE_1V8, 0x00800000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25Q32A = {SPIFLASH_VOLTAGE_3V, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_FM25M32A = {SPIFLASH_VOLTAGE_1V8, 0x00400000,0,0,0,0,0};
static struct SPIFLASH_ExtCfg cfg_BG25Q128AL = {SPIFLASH_VOLTAGE_1V8, 0x01000000,0,0,0,0,0};

#ifdef FLASH_SIZE_128MBITX64MBIT_OLD

//*********** WARNING: PLEASE CONSULT SPREADTRUM SUPPORT ENGINEER BEFORE MODIFYING THIS FILE !***********

/**************FLASH_A PARTITON******************/
#define  PHY_FLASH_SIZE            MAX_HW_FLASH_SIZE
#define  PHY_FLASH_SIZE_B          MAX_HW_FLASH_SIZE_B

#define  FLASH_SECTOR_SIZE         0x00010000
#define  FLASH_SECTOR_SIZE_B       0x00010000

#define  MAX_FLASH_SIZE   		  (PHY_FLASH_SIZE)
#define  MAX_FLASH_SIZE_B         (PHY_FLASH_SIZE_B)

#define  MMI_RES_SECTOR_NUM          88
#define  MMI_RESOURCE_SIZE           (MMI_RES_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define  MMI_RES_BASE_ADDR       	 (PHY_FLASH_START_ADDR + MAX_FLASH_SIZE - MMI_RESOURCE_SIZE)

#define  CUSTOM_ADDR_SECTOR_NUM		32
#define  CUSTOM_ADDR_SIZE			(CUSTOM_ADDR_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define  CUSTOM_ADDR				(MMI_RES_BASE_ADDR-CUSTOM_ADDR_SIZE)

#define  PS_STONE_SECTOR_NUM		134
#define  STONE_SIZE					(PS_STONE_SECTOR_NUM*FLASH_SECTOR_SIZE)
//该地址与fdl2、scatter有关联，调整分区时请注意
#define  PS_BASE_ADDR				(0x60020000)

//BOOTLOADER
#define  PHY_FLASH_START_ADDR 		0x60000000
#define  BOOTLOADER_SIZE			(2*FLASH_SECTOR_SIZE)

/**************FLASH_B PARTITON******************/

#define  UMEM_SECTOR_NUM            25    //1600KB
#ifdef UMEM_SUPPORT
#define  UDISK_SIZE                 (UMEM_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  UDISK_BASE_ADDR        	(CP_BASE_ADDR + MAX_FLASH_SIZE_B - UDISK_SIZE)
#else
#define  UDISK_SIZE                 (0)
#define  UDISK_BASE_ADDR            (-1)
#endif

#define  RUNNIN_NV_SECTOR_NUM       9
#define  RUNNIN_NV_SECTOR_COUNT    	(UMEM_SECTOR_NUM + RUNNIN_NV_SECTOR_NUM+FIXNV_SECTOR_NUM)
#define  RUNNIN_NV_SIZE          	(RUNNIN_NV_SECTOR_COUNT*FLASH_SECTOR_SIZE_B)
#define  RUNNIN_NV_BASE_ADDR    	(CP_BASE_ADDR + MAX_FLASH_SIZE_B - RUNNIN_NV_SIZE)

#define  PRODUCT_NV_BASE_ADDR    	(FIXED_NV_BASE_ADDR + FIXED_NV_SIZE)

#define  FIXNV_SECTOR_NUM			2
#define  FIXED_NV_SIZE          	0x1FC00    //127KB
#define  FIXED_NV_BASE_ADDR      	(RUNNIN_NV_BASE_ADDR - FIXNV_SECTOR_NUM*FLASH_SECTOR_SIZE)

#define  OPERATOR_DATA_SECTOR_NUM   1  // 64KB
#define  OPERATOR_DATA_SIZE       	(OPERATOR_DATA_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  OPERATOR_DATA_BASE_ADDR  	(FIXED_NV_BASE_ADDR - OPERATOR_DATA_SIZE)

#define  APN_SECTOR_NUM				5
#define  APN_SIZE					(APN_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  APN_BASE_ADDR 				(OPERATOR_DATA_BASE_ADDR-APN_SIZE)

#define  CONFIG_NV_SECTOR_NUM   	1
#define  CONFIG_NV_SIZE      		(CONFIG_NV_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  CONFIG_NV_BASE_ADDR  		(APN_BASE_ADDR-CONFIG_NV_SIZE)

#define  SIM_LOCK_SECTOR_NUM 	  	1
#define	 SIM_LOCK_SIZE           	(SIM_LOCK_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  SIM_LOCK_BASE_ADDR      	(CONFIG_NV_BASE_ADDR - SIM_LOCK_SIZE)

#define  XFILE_SECTOR_NUM	        16
#define  XFILE_SIZE					(XFILE_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  XFILE_BASE_ADDR		    (SIM_LOCK_BASE_ADDR-XFILE_SIZE)

#define  CRYSTAL_AGING_SECTOR_NUM     2
#define  CRYSTAL_AGING_SIZE         (CRYSTAL_AGING_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  CRYSTAL_AGING_ADDR        	(XFILE_BASE_ADDR-CRYSTAL_AGING_SIZE)

#define  IMS_TOGGEL_CFG_SECTOR_NUM    1
#define  IMS_TOGGEL_CFG_SIZE         (IMS_TOGGEL_CFG_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  IMS_TOGGEL_CFG_ADDR       	(CRYSTAL_AGING_ADDR-IMS_TOGGEL_CFG_SIZE)

#define  CP_SECTOR_SECTOR_NUM     	64
#define  CP_SIZE                	(CP_SECTOR_SECTOR_NUM*FLASH_SECTOR_SIZE_B)
#define  CP_BASE_ADDR          		0x70000000

/***********Flash LayeOut******************

  FLASH A PARTITON(16MB)
  ---------------------| 0x60000000
  |bootloader          |
  |--------------------| 0x60020000
  |stone               |
  |--------------------| 0x60EF0000
  |Crystal aging       |
  |--------------------| 0x60F10000
  |simlock             |
  |--------------------| 0x60F20000
  |fixd NV             |
  |--------------------| 0x60F3FC00
  |production info     |
  |--------------------| 0x60F40000
  |fixd NV backup      |
  |--------------------| 0x60F60000
  |running nv          |
  |--------------------| 0x60FF0000
  |UMEM				   |
  |--------------------| 0x61000000

  FLASH B PARTITON(8MB)
  |--------------------| 0x70000000
  |CP                  |
  |--------------------| 0x703F0000
  |mmiresource         |
  |--------------------| 0x707D0000
  |IMS Toggle		   |
  |--------------------| 0x707E0000
  | ConfigNV		   |
  |--------------------| 0x707F0000
  | Operator Data 	   |
  |--------------------| 0x70800000

******************************************/

#pragma arm section rodata = "SPIFLASH_CFG_TABLE_DETAIL"
PUBLIC  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config =
{
    1,            				/*single-bank*/
    RUNNIN_NV_SECTOR_COUNT,       /*runningNV/EFS sector number*/
    0,            				/* abandon */
    FLASH_SECTOR_SIZE,  		/*sector size*/
    PHY_FLASH_START_ADDR,   	            /*start address*/
    RUNNIN_NV_BASE_ADDR,        /*runningNV/EFS start address*/
    MAX_FLASH_SIZE,   		    /*flash size*/
    FIXED_NV_BASE_ADDR, //FIXED_NV_BASE_ADDR,         /*fixed nv start address*/
    PRODUCT_NV_BASE_ADDR,       /*product info start address*/
    MMI_RES_BASE_ADDR,          /*lint !e570 *//*resource start address*/
#ifdef CHIP_VER_UIS8910FF
    MMI_RESOURCE_SIZE,
#endif
    UDISK_BASE_ADDR,            /*lint !e570*//*U Disk start address	*/
    UDISK_SIZE,                 /*lint !e570*//*U Disk Size*/
    -1,                         /*lint !e570*/ /*blbin*/
   SIM_LOCK_BASE_ADDR,    //simlock Start Addr
#ifdef NOR_BOOTLOADER_SUPPORT
#ifdef SECURE_BOOT_SUPPORT
    PS_BASE_ADDR + KERNEL_HEADER_SIZE,//For Secure boot, kernel image will start after KERNEL_HEADER_SIZE
#else
    PS_BASE_ADDR,     //PS Start Addr
#endif
#endif        //XFILE_DUMP_SIZE
    OPERATOR_DATA_BASE_ADDR,  // operator_data_addr
    OPERATOR_DATA_SIZE,            // operator_data_size
#ifdef CHIP_VER_UIS8910FF

    CP_BASE_ADDR, //cp_addr
    CP_SIZE, //cp_size
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    APN_BASE_ADDR,
    APN_SIZE,
#endif
    CONFIG_NV_BASE_ADDR,  // config_nv_addr
    CONFIG_NV_SIZE,            // config_nv_size
#endif
    CRYSTAL_AGING_ADDR,
    CRYSTAL_AGING_SIZE,
    IMS_TOGGEL_CFG_ADDR,
    IMS_TOGGEL_CFG_SIZE,
};//lint !e570

PUBLIC  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config_B =
{
    1,            				/*single-bank*/
    RUNNIN_NV_SECTOR_COUNT,       /*runningNV/EFS sector number*/
    0,            				/* abandon */
    FLASH_SECTOR_SIZE,  		/*sector size*/
    CP_BASE_ADDR,   			/*start address*/
    RUNNIN_NV_BASE_ADDR,        /*runningNV/EFS start address*/
    MAX_FLASH_SIZE_B,   		    /*flash size*/
    FIXED_NV_BASE_ADDR,         /*fixed nv start address*/
    PRODUCT_NV_BASE_ADDR,       /*product info start address*/
    MMI_RES_BASE_ADDR,          /*lint !e570 *//*resource start address*/
#ifdef CHIP_VER_UIS8910FF
   MMI_RESOURCE_SIZE,
#endif
    UDISK_BASE_ADDR,            /*lint !e570*//*U Disk start address	*/
    UDISK_SIZE,                 /*lint !e570*//*U Disk Size*/
    -1,                         /*lint !e570*/ /*blbin*/
	SIM_LOCK_BASE_ADDR,    //simlock Start Addr
     -1,     //PS Start Addr
    OPERATOR_DATA_BASE_ADDR,
    OPERATOR_DATA_SIZE,
#ifdef CHIP_VER_UIS8910FF
    CP_BASE_ADDR, //cp_addr
    CP_SIZE, //cp_size
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    APN_BASE_ADDR,
    APN_SIZE,
#endif
    CONFIG_NV_BASE_ADDR,  // config_nv_addr
    CONFIG_NV_SIZE,            // config_nv_size
#endif
    CRYSTAL_AGING_ADDR,
    CRYSTAL_AGING_SIZE,
    IMS_TOGGEL_CFG_ADDR,
    IMS_TOGGEL_CFG_SIZE,
};//lint !e570


#elif FLASH_SIZE_128MBITX64MBIT_NEW

//*********** WARNING: PLEASE CONSULT SPREADTRUM SUPPORT ENGINEER BEFORE MODIFYING THIS FILE !***********

/**************FLASH_A PARTITON******************/
#define  PHY_FLASH_SIZE            MAX_HW_FLASH_SIZE
#define  PHY_FLASH_SIZE_B          MAX_HW_FLASH_SIZE_B

#define  FLASH_SECTOR_SIZE         0x00010000
#define  FLASH_SECTOR_SIZE_B       0x00010000

#define  MAX_FLASH_SIZE   		  (PHY_FLASH_SIZE)
#define  MAX_FLASH_SIZE_B         (PHY_FLASH_SIZE_B)

#define  UMEM_SECTOR_NUM            1    //64KB
#ifdef UMEM_SUPPORT
#define  UDISK_SIZE                 (UMEM_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define  UDISK_BASE_ADDR        	(PHY_FLASH_START_ADDR + MAX_FLASH_SIZE - UDISK_SIZE)
#else
#define  UDISK_SIZE                 (0)
#define  UDISK_BASE_ADDR            (-1)
#endif

#define  RUNNIN_NV_SECTOR_NUM       11
#define  RUNNIN_NV_SECTOR_COUNT    	(UMEM_SECTOR_NUM + RUNNIN_NV_SECTOR_NUM)
#define  RUNNIN_NV_SIZE          	(RUNNIN_NV_SECTOR_COUNT*FLASH_SECTOR_SIZE)
#define  RUNNIN_NV_BASE_ADDR    	(PHY_FLASH_START_ADDR + MAX_FLASH_SIZE - RUNNIN_NV_SIZE)

#define  PRODUCT_NV_BASE_ADDR    	(FIXED_NV_BASE_ADDR + FIXED_NV_SIZE)

#define  FIXNV_SECTOR_NUM			2
#define  FIXED_NV_SIZE          	0x1FC00    //127KB
#define  FIXED_NV_BASE_ADDR      	(RUNNIN_NV_BASE_ADDR - FIXNV_SECTOR_NUM*FLASH_SECTOR_SIZE)

#define  SIM_LOCK_SECTOR_NUM 	  	1
#define	 SIM_LOCK_SIZE           	(SIM_LOCK_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define  SIM_LOCK_BASE_ADDR      	(FIXED_NV_BASE_ADDR - SIM_LOCK_SIZE)

#define  CRYSTAL_AGING_SECTOR_NUM     2
#define  CRYSTAL_AGING_SIZE         (CRYSTAL_AGING_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define  CRYSTAL_AGING_ADDR        	(SIM_LOCK_BASE_ADDR-CRYSTAL_AGING_SIZE)

#ifndef FOTA_SUPPORT_NONE
#define  FOTA_BOOTLOADER_SECTOR_NUM     3
#define  FOTA_BOOTLOADER_SIZE         (FOTA_BOOTLOADER_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define  FOTA_BOOTLOADER_ADDR        	(CRYSTAL_AGING_ADDR-FOTA_BOOTLOADER_SIZE)
#ifdef FOTA_SUPPORT_ABUP
#ifdef ABUP_FOTA_SUPPORT_TCARD
#define  ABUP_FOTA_APP_SECTOR_NUM            2
#else
#define  ABUP_FOTA_APP_SECTOR_NUM            80 //5M
#endif
#define  ABUP_FOTA_APP_SIZE                  (ABUP_FOTA_APP_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define  ABUP_FOTA_APP_ADDR        	        (FOTA_BOOTLOADER_ADDR-ABUP_FOTA_APP_SIZE)
#endif
#else
#define  FOTA_BOOTLOADER_SECTOR_NUM     0xff
#define  FOTA_BOOTLOADER_SIZE         0xff
#define  FOTA_BOOTLOADER_ADDR        0xff
#endif

#ifndef FOTA_SUPPORT_NONE
#ifdef FOTA_SUPPORT_ABUP
#ifdef ABUP_FOTA_SUPPORT_TCARD
#define  PS_STONE_SECTOR_NUM		232
#else
#define  PS_STONE_SECTOR_NUM		154
#endif
#else
#define  PS_STONE_SECTOR_NUM		234
#endif
#else
#define  PS_STONE_SECTOR_NUM		237
#endif

#define  STONE_SIZE					(PS_STONE_SECTOR_NUM*FLASH_SECTOR_SIZE)
//该地址与fdl2、scatter有关联，调整分区时请注意
#define  PS_BASE_ADDR				(0x60020000)

//BOOTLOADER
#define  PHY_FLASH_START_ADDR 		0x60000000
#define  BOOTLOADER_SIZE			(2*FLASH_SECTOR_SIZE)

/**************FLASH_B PARTITON******************/

#define OPERATOR_DATA_SECTOR_NUM   	1  // 64KB
#define OPERATOR_DATA_SIZE       	(OPERATOR_DATA_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define OPERATOR_DATA_BASE_ADDR  	(CP_BASE_ADDR+PHY_FLASH_SIZE_B-OPERATOR_DATA_SIZE)

#define CONFIG_NV_SECTOR_NUM   		1
#define CONFIG_NV_SIZE      		(CONFIG_NV_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define CONFIG_NV_BASE_ADDR  		(OPERATOR_DATA_BASE_ADDR-CONFIG_NV_SIZE)

#define IMS_TOGGEL_CFG_SECTOR_NUM    1
#define IMS_TOGGEL_CFG_SIZE         (IMS_TOGGEL_CFG_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define IMS_TOGGEL_CFG_ADDR       	(CONFIG_NV_BASE_ADDR-IMS_TOGGEL_CFG_SIZE)

#define MMI_RES_SECTOR_NUM          75
#define MMI_RESOURCE_SIZE           (MMI_RES_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define MMI_RES_BASE_ADDR       	(IMS_TOGGEL_CFG_ADDR - MMI_RESOURCE_SIZE)

#define CP_SECTOR_SECTOR_NUM     	50
#define CP_SIZE                		(CP_SECTOR_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define CP_BASE_ADDR          		0x70000000

/***********Flash LayeOut******************

  FLASH A PARTITON(16MB)
  ---------------------| 0x60000000
  |bootloader          |
  |--------------------| 0x60020000
  |stone               |
  |--------------------| 0x60EF0000
  |Crystal aging       |
  |--------------------| 0x60F10000
  |simlock             |
  |--------------------| 0x60F20000
  |fixd NV             |
  |--------------------| 0x60F3FC00
  |production info     |
  |--------------------| 0x60F40000
  |fixd NV backup      |
  |--------------------| 0x60F60000
  |running nv          |
  |--------------------| 0x60FF0000
  |UMEM				   |
  |--------------------| 0x61000000

  FLASH B PARTITON(8MB)
  |--------------------| 0x70000000
  |CP                  |
  |--------------------| 0x70320000
  |mmiresource         |
  |--------------------| 0x707D0000
  |IMS Toggle		   |
  |--------------------| 0x707E0000
  | ConfigNV		   |
  |--------------------| 0x707F0000
  | Operator Data 	   |
  |--------------------| 0x70800000

******************************************/

#pragma arm section rodata = "SPIFLASH_CFG_TABLE_DETAIL"
PUBLIC  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config =
{
    1,            				/*single-bank*/
    RUNNIN_NV_SECTOR_COUNT,       /*runningNV/EFS sector number*/
    0,            				/* abandon */
    FLASH_SECTOR_SIZE,  		/*sector size*/
    PHY_FLASH_START_ADDR,   	            /*start address*/
    RUNNIN_NV_BASE_ADDR,        /*runningNV/EFS start address*/
    MAX_FLASH_SIZE,   		    /*flash size*/
    FIXED_NV_BASE_ADDR, //FIXED_NV_BASE_ADDR,         /*fixed nv start address*/
    PRODUCT_NV_BASE_ADDR,       /*product info start address*/
    MMI_RES_BASE_ADDR,          /*lint !e570 *//*resource start address*/
#ifdef CHIP_VER_UIS8910FF
    MMI_RESOURCE_SIZE,
#endif
    UDISK_BASE_ADDR,            /*lint !e570*//*U Disk start address	*/
    UDISK_SIZE,                 /*lint !e570*//*U Disk Size*/
    -1,                         /*lint !e570*/ /*blbin*/
   SIM_LOCK_BASE_ADDR,    //simlock Start Addr
#ifdef NOR_BOOTLOADER_SUPPORT
#ifdef SECURE_BOOT_SUPPORT
    PS_BASE_ADDR + KERNEL_HEADER_SIZE,//For Secure boot, kernel image will start after KERNEL_HEADER_SIZE
#else
    PS_BASE_ADDR,     //PS Start Addr
#endif
#endif        //XFILE_DUMP_SIZE
    OPERATOR_DATA_BASE_ADDR,  // operator_data_addr
    OPERATOR_DATA_SIZE,            // operator_data_size
#ifdef CHIP_VER_UIS8910FF

    CP_BASE_ADDR, //cp_addr
    CP_SIZE, //cp_size
    CONFIG_NV_BASE_ADDR,  // config_nv_addr
    CONFIG_NV_SIZE,            // config_nv_size
#endif
    CRYSTAL_AGING_ADDR,
    CRYSTAL_AGING_SIZE,
    IMS_TOGGEL_CFG_ADDR,
    IMS_TOGGEL_CFG_SIZE,
	FOTA_BOOTLOADER_ADDR,
#ifdef FOTA_SUPPORT_ABUP
    ABUP_FOTA_APP_ADDR,
    ABUP_FOTA_APP_SIZE
#endif

};//lint !e570

PUBLIC  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config_B =
{
    1,            				/*single-bank*/
    RUNNIN_NV_SECTOR_COUNT,       /*runningNV/EFS sector number*/
    0,            				/* abandon */
    FLASH_SECTOR_SIZE,  		/*sector size*/
    CP_BASE_ADDR,   	            /*start address*/
    RUNNIN_NV_BASE_ADDR,        /*runningNV/EFS start address*/
    MAX_FLASH_SIZE_B,   		    /*flash size*/
    FIXED_NV_BASE_ADDR,         /*fixed nv start address*/
    PRODUCT_NV_BASE_ADDR,       /*product info start address*/
    MMI_RES_BASE_ADDR,          /*lint !e570 *//*resource start address*/
#ifdef CHIP_VER_UIS8910FF
   MMI_RESOURCE_SIZE,
#endif
    UDISK_BASE_ADDR,            /*lint !e570*//*U Disk start address	*/
    UDISK_SIZE,                 /*lint !e570*//*U Disk Size*/
    -1,                         /*lint !e570*/ /*blbin*/
	SIM_LOCK_BASE_ADDR,    //simlock Start Addr
     -1,     //PS Start Addr
    OPERATOR_DATA_BASE_ADDR,
    OPERATOR_DATA_SIZE,
#ifdef CHIP_VER_UIS8910FF
    CP_BASE_ADDR, //cp_addr
    CP_SIZE, //cp_size
    CONFIG_NV_BASE_ADDR,  // config_nv_addr
    CONFIG_NV_SIZE,            // config_nv_size
#endif
    CRYSTAL_AGING_ADDR,
    CRYSTAL_AGING_SIZE,
    IMS_TOGGEL_CFG_ADDR,
    IMS_TOGGEL_CFG_SIZE,
	FOTA_BOOTLOADER_ADDR,
#ifdef FOTA_SUPPORT_ABUP
    ABUP_FOTA_APP_ADDR,
    ABUP_FOTA_APP_SIZE
#endif

};//lint !e570

#elif FLASH_SIZE_128MBIT

#define  MAX_FLASH_SIZE				 MAX_HW_FLASH_SIZE

#define  KERNEL_HEADER_SIZE			 1024

#define  RUNNIN_NV_SECTOR_COUNT_B	 36

#define  FLASH_SECTOR_SIZE	        0x00010000

#define  RUNNIN_NV_SECTOR_NUM       11

#define  UMEM_SECTOR_NUM            1    //64KB

#define RUNNIN_NV_SECTOR_COUNT    	(UMEM_SECTOR_NUM + RUNNIN_NV_SECTOR_NUM)

#define FIXNV_SECTOR_NUM			2
#define RUNNIN_NV_SIZE          	(RUNNIN_NV_SECTOR_COUNT*FLASH_SECTOR_SIZE)
#define FIXED_NV_SIZE          	 	0x1FC00    //127KB
#define RUNNIN_NV_BASE_ADDR    		(PHY_FLASH_START_ADDR + MAX_FLASH_SIZE - RUNNIN_NV_SIZE)
#define FIXED_NV_BASE_ADDR      	(RUNNIN_NV_BASE_ADDR - FIXNV_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define PRODUCT_NV_BASE_ADDR    	(FIXED_NV_BASE_ADDR + FIXED_NV_SIZE)

#ifdef  UMEM_SUPPORT
#define UDISK_SIZE                 (UMEM_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define UDISK_BASE_ADDR        	   (PHY_FLASH_START_ADDR + MAX_FLASH_SIZE - UDISK_SIZE)
#else
#define UDISK_SIZE                 (0)
#define UDISK_BASE_ADDR            (-1)
#endif

#define OPERATOR_DATA_SECTOR_NUM   	1  // 64KB
#define OPERATOR_DATA_SIZE       	(OPERATOR_DATA_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define OPERATOR_DATA_BASE_ADDR  	(FIXED_NV_BASE_ADDR-OPERATOR_DATA_SIZE)

#define CONFIG_NV_SECTOR_NUM   		1
#define CONFIG_NV_SIZE      		(CONFIG_NV_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define CONFIG_NV_BASE_ADDR  		(OPERATOR_DATA_BASE_ADDR-CONFIG_NV_SIZE)

#define SIM_LOCK_SECTOR_NUM 	  	(1)
#define SIM_LOCK_SIZE           	(SIM_LOCK_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define SIM_LOCK_BASE_ADDR      	(CONFIG_NV_BASE_ADDR - SIM_LOCK_SIZE)

#define CRYSTAL_AGING_SECTOR_NUM     2
#define CRYSTAL_AGING_SIZE         	(CRYSTAL_AGING_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define CRYSTAL_AGING_ADDR        	(SIM_LOCK_BASE_ADDR-CRYSTAL_AGING_SIZE)

#define IMS_TOGGEL_CFG_SECTOR_NUM    1
#define IMS_TOGGEL_CFG_SIZE         (IMS_TOGGEL_CFG_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define IMS_TOGGEL_CFG_ADDR       	(CRYSTAL_AGING_ADDR-IMS_TOGGEL_CFG_SIZE)

#define CP_SECTOR_SECTOR_NUM     	50
#define CP_SIZE                		(CP_SECTOR_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define CP_BASE_ADDR          		(IMS_TOGGEL_CFG_ADDR-CP_SIZE)

#define MMI_RES_SECTOR_NUM   70//   73//    70 //60
#define MMI_RESOURCE_SIZE           (MMI_RES_SECTOR_NUM*FLASH_SECTOR_SIZE)
#define MMI_RES_BASE_ADDR       	(CP_BASE_ADDR - MMI_RESOURCE_SIZE)

#define  PS_STONE_SECTOR_NUM	112/// 109//	112 //122
#define  STONE_SIZE					(PS_STONE_SECTOR_NUM*FLASH_SECTOR_SIZE)
//该地址与fdl2、scatter有关联，调整分区时请注意
#define  PS_BASE_ADDR				(0x60020000)

//BOOTLOADER
#define  PHY_FLASH_START_ADDR 		0x60000000
#define  BOOTLOADER_SIZE			(2*FLASH_SECTOR_SIZE)

/***********Flash LayeOut******************

  ONLY FLASH A PARTITION (16MB)
  |--------------------| 0x60000000
  |bootloader          |
  |--------------------| 0x60020000
  |stone               |
  |--------------------| 0x607E0000
  |mmiresource         |
  |--------------------| 0x60BA0000
  |CP                  |
  |--------------------| 0x60EC0000
  |IMS Toggle		   |
  |--------------------| 0x60ED0000
  |Crystal aging       |
  |--------------------| 0x60EF0000
  |simlock             |
  |--------------------| 0x60F00000
  |ConfigNV		       |
  |--------------------| 0x60F10000
  |Operator Data	   |
  |--------------------| 0x60F20000
  |fixd NV             |
  |--------------------| 0x60F3FC00
  |production info     |
  |--------------------| 0x60F40000
  |fixd NV backup      |
  |--------------------| 0x60F60000
  |running nv          |
  |--------------------| 0x60FF0000
  |UMEM				   |
  |--------------------| 0x61000000

******************************************/

#pragma arm section rodata = "SPIFLASH_CFG_TABLE_DETAIL"
PUBLIC  const NOR_FLASH_CONFIG_T  s_platform_patitiion_config =
{
    1,            				/*single-bank*/
    RUNNIN_NV_SECTOR_COUNT,       /*runningNV/EFS sector number*/
    0,            				/* abandon */
    FLASH_SECTOR_SIZE,  		/*sector size*/
    PHY_FLASH_START_ADDR,   	            /*start address*/
    RUNNIN_NV_BASE_ADDR,        /*runningNV/EFS start address*/
    MAX_FLASH_SIZE,   		    /*flash size*/
    FIXED_NV_BASE_ADDR, //FIXED_NV_BASE_ADDR,         /*fixed nv start address*/
    PRODUCT_NV_BASE_ADDR,       /*product info start address*/
    MMI_RES_BASE_ADDR,          /*lint !e570 *//*resource start address*/

    MMI_RESOURCE_SIZE,

    UDISK_BASE_ADDR,            /*lint !e570*//*U Disk start address	*/
    UDISK_SIZE,                 /*lint !e570*//*U Disk Size*/
    -1,                         /*lint !e570*/ /*blbin*/
   SIM_LOCK_BASE_ADDR,    //simlock Start Addr
#ifdef NOR_BOOTLOADER_SUPPORT
#ifdef SECURE_BOOT_SUPPORT
    PS_BASE_ADDR + KERNEL_HEADER_SIZE,//For Secure boot, kernel image will start after KERNEL_HEADER_SIZE
#else
    PS_BASE_ADDR,     //PS Start Addr
#endif
#endif
    OPERATOR_DATA_BASE_ADDR,  // operator_data_addr
    OPERATOR_DATA_SIZE,            // operator_data_size


    CP_BASE_ADDR, //cp_addr
    CP_SIZE, //cp_size
    CONFIG_NV_BASE_ADDR,  // config_nv_addr
    CONFIG_NV_SIZE,            // config_nv_size

    CRYSTAL_AGING_ADDR,
    CRYSTAL_AGING_SIZE,
    IMS_TOGGEL_CFG_ADDR,
    IMS_TOGGEL_CFG_SIZE,
};//lint !e570


#define FLASH_LOGIC_PATITION   &s_platform_patitiion_config


#pragma arm section rodata = "SPIFLASH_CFG_TABLE"

LOCAL const SPIFLASH_SPEC_T SpiFlashSpec[] = {
    /***************************************************************************/
    /* nMID, nDID1, nDID2, nQEPos, nWIPPos, EnterQPI, ExitQPI, cmd_4pp, cmd_4read, suspend, resume, reset_support, QPI_support, spiflash_cfg, pExtcfg*/
    /***************************************************************************/
    { 0xC8, 0x60, 0x19, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ256},//GD25LQ256
    //{ 0xC8, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ128},//GD25LQ128
    { 0xC8, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ128},//GD25LQ128
    {0X00, 0X00, 0X00, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//GD25LQ64B
    { 0xC8, 0x60, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ32B},//GD25LQ32B
    { 0xC8, 0x60, 0x15, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ16},//GD25LQ16

    { 0xC8, 0x40, 0x17, 0x09, 0x00, _NULL, _NULL, 0x32, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25Q64B},//GD25Q64B
    { 0xC8, 0x40, 0x16, 0x09, 0x00, _NULL, _NULL, 0x32, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25Q32B},//GD25Q32B

  //  { 0x2C, 0xCB, 0x18, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W128},//N25W128
  //  { 0x2C, 0xCB, 0x17, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W064},//N25W064
   // { 0x2C, 0xCB, 0x16, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W032},//N25W032
	{ 0X00, 0X00, 0X00, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//N25W128
    { 0X00, 0X00, 0X00, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//N25W064
    { 0X00, 0X00, 0X00, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//N25W032

    { 0xC2, 0x25, 0x38, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U12835E},//MX25U12835E
    { 0xC2, 0x25, 0x37, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U6435E},//MX25U6435E
    { 0xC2, 0x25, 0x36, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U3235E},//MX25U3235E

    { 0xE0, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_BG25Q128AL},//BG25Q128AL

    { 0xEF, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q128FW},//W25Q128FW
    { 0xEF, 0x60, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q64FW},//W25Q64FW
    { 0xEF, 0x40, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q32FV},//W25Q32FV
    { 0xEF, 0x40, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q64FVSTIM},//W25Q64FVSTIM
    { 0xEF, 0x40, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q128FV},//W25Q128FV
    { 0xEF, 0x40, 0x19, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q256FV},//W25Q256FV

    { 0xF8, 0x32, 0x16, 0x09, 0x00, _NULL, _NULL, 0x38, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25Q32A},//FM25Q32, SC2112C-V
    { 0xF8, 0x42, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M128A},//FM25M128A
    { 0xF8, 0x43, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M64C},//FM25M64C
    { 0xF8, 0x42, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M64A},//FM25M64A
    { 0xF8, 0x42, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M32A},//FM25M32

    { 0x8C, 0x25, 0x37, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_F25D64QA},//F25D64QA

    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, FALSE, FALSE, NULL, NULL}
};

#pragma arm section

#endif

#if defined (FLASH_SIZE_128MBITX64MBIT_OLD) || defined (FLASH_SIZE_128MBITX64MBIT_NEW)

#define FLASH_LOGIC_PATITION   &s_platform_patitiion_config
#define FLASH_LOGIC_PATITION_B   &s_platform_patitiion_config_B


#pragma arm section rodata = "SPIFLASH_CFG_TABLE"

LOCAL const SPIFLASH_SPEC_T SpiFlashSpec[] = {
    /***************************************************************************/
    /* nMID, nDID1, nDID2, nQEPos, nWIPPos, EnterQPI, ExitQPI, cmd_4pp, cmd_4read, suspend, resume, reset_support, QPI_support, spiflash_cfg, pExtcfg*/
    /***************************************************************************/
    { 0xC8, 0x60, 0x19, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ256},//GD25LQ256
    //{ 0xC8, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ128},//GD25LQ128
    { 0xC8, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ128},//GD25LQ128
    { 0xC8, 0x60, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_GD25LQ64B},//GD25LQ64B
    { 0xC8, 0x60, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ32B},//GD25LQ32B
    { 0xC8, 0x60, 0x15, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25LQ16},//GD25LQ16

    { 0xC8, 0x40, 0x17, 0x09, 0x00, _NULL, _NULL, 0x32, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25Q64B},//GD25Q64B
    { 0xC8, 0x40, 0x16, 0x09, 0x00, _NULL, _NULL, 0x32, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_GD25Q32B},//GD25Q32B

  //  { 0x2C, 0xCB, 0x18, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W128},//N25W128
  //  { 0x2C, 0xCB, 0x17, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W064},//N25W064
   // { 0x2C, 0xCB, 0x16, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_N25W032},//N25W032
    { 0x2C, 0xCB, 0x18, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_N25W128},//N25W128
    { 0x2C, 0xCB, 0x17, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_N25W064},//N25W064
    { 0x2C, 0xCB, 0x16, 0xFF, 0x00, _NULL, _NULL, 0x12, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION_B, &cfg_N25W032},//N25W032

    { 0xC2, 0x25, 0x38, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U12835E},//MX25U12835E
    { 0xC2, 0x25, 0x37, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U6435E},//MX25U6435E
    { 0xC2, 0x25, 0x36, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_MX25U3235E},//MX25U3235E

    { 0xE0, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_BG25Q128AL},//BG25Q128AL

    { 0xEF, 0x60, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q128FW},//W25Q128FW
    { 0xEF, 0x60, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q64FW},//W25Q64FW
    { 0xEF, 0x40, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q32FV},//W25Q32FV
    { 0xEF, 0x40, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q64FVSTIM},//W25Q64FVSTIM
    { 0xEF, 0x40, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q128FV},//W25Q128FV
    { 0xEF, 0x40, 0x19, 0x09, 0x00, 0x38, 0xFF, 0x32, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_W25Q256FV},//W25Q256FV

    { 0xF8, 0x32, 0x16, 0x09, 0x00, _NULL, _NULL, 0x38, 0xEB, 0x75, 0x7A, FALSE, FALSE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25Q32A},//FM25Q32, SC2112C-V
    { 0xF8, 0x42, 0x18, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M128A},//FM25M128A
    { 0xF8, 0x43, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M64C},//FM25M64C
    { 0xF8, 0x42, 0x17, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M64A},//FM25M64A
    { 0xF8, 0x42, 0x16, 0x09, 0x00, 0x38, 0xFF, 0x33, 0xEB, 0x75, 0x7A, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_FM25M32A},//FM25M32

    { 0x8C, 0x25, 0x37, 0x06, 0x00, 0x35, 0xF5, 0x38, 0xEB, 0xB0, 0x30, TRUE, TRUE, (NOR_FLASH_CONFIG_PTR)FLASH_LOGIC_PATITION, &cfg_F25D64QA},//F25D64QA

    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, FALSE, FALSE, NULL, NULL}
};
#pragma arm section
#endif

#pragma arm section code = "SPIFLASH_CFG_CODE"
SPIFLASH_SPEC_T *SPIFLASH_SPEC_Get(uint16 nMID, uint16 nDID1, uint16 nDID2)
{
   uint32 i;

   for(i=0; SpiFlashSpec[i].nMID!=0x0; i++)
   {
       if((nDID1 == SpiFlashSpec[i].nDID1) && (nMID == SpiFlashSpec[i].nMID) && (nDID2 == SpiFlashSpec[i].nDID2))
       {
           //Find Nand Type
           return (SPIFLASH_SPEC_T *)&SpiFlashSpec[i];
       }
   }
   //Not find
   return NULL;
}
#pragma arm section code


#pragma arm section rodata = "DFILE_DOWNLOAD_CFG"
const DFILE_CONFIG_T  s_download_file_config =
{
   DFILE_MAGIC_FIRST,
   DFILE_MAGIC_SECOND,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_MAGIC_END
};
#pragma arm section rodata


#pragma arm section rodata = "DFILE_DOWNLOAD_CFG_B"
const DFILE_CONFIG_T  s_download_file_config_B =
{
   DFILE_MAGIC_FIRST,
   DFILE_MAGIC_SECOND,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_INVAILD_ADDR,
   DFILE_MAGIC_END
};

#pragma arm section rodata


#pragma arm section rodata = "SPIFLASH_TYPE_CFG"
const SPIFLASH_TYPE_CONFIG_T  s_spiflash_type_config =
{
   SPIFLASH_MAGIC_FIRST,
   SPIFLASH_MAGIC_SECOND,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_INVAILD_DATA,
   SPIFLASH_MAGIC_END
};

#pragma arm section rodata

/*****************************************************************************/
//  Description :
//      Get the DFILE relative information of ps/res bin files.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the pointer for the relative information of ps/bin files
//  Author :
//  Note   : Only for the combination of the bin files
//
/*****************************************************************************/
PUBLIC DFILE_CONFIG_T*  SPIFLASH_GetDFileConfig(void)
{
   if( (DFILE_MAGIC_FIRST == s_download_file_config.magic_first) &&
 	   (DFILE_MAGIC_SECOND == s_download_file_config.magic_second) &&
 	   (DFILE_MAGIC_END == s_download_file_config.magic_end))
   	{
        return (DFILE_CONFIG_T* )&s_download_file_config;
    }
    else
    {
    	return NULL;
    }
}



/*****************************************************************************/
//  Description :
//      Get the DFILE relative information of cp/nv/UMEM/operator bin files.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the pointer for the relative information of cp/nv/UMEM/operator  files
//  Author :
//  Note   : Only for the combination of the bin files
//
/*****************************************************************************/
PUBLIC DFILE_CONFIG_T*  SPIFLASH_GetDFileConfigForFlashB(void)
{
   if( (DFILE_MAGIC_FIRST == s_download_file_config_B.magic_first) &&
 	   (DFILE_MAGIC_SECOND == s_download_file_config_B.magic_second) &&
 	   (DFILE_MAGIC_END == s_download_file_config_B.magic_end))
   	{
        return (DFILE_CONFIG_T* )&s_download_file_config_B;
    }
    else
    {
    	return NULL;
    }
}


PUBLIC SPIFLASH_TYPE_CONFIG_T*  SPIFLASH_GetFlashType(void)
{
#ifdef SECURE_BOOT_SUPPORT
	SPIFLASH_TYPE_CONFIG_T* ptr = NULL;
	ptr = ((SPIFLASH_TYPE_CONFIG_T*)FLASH_CONFIG_OFF);//In Bootloader, Flash Type Configuration is stored at FLASH_CONFIG_OFF offset

	if( (SPIFLASH_MAGIC_FIRST == ptr->magic_first) &&
		(SPIFLASH_MAGIC_SECOND == ptr->magic_second) &&
	 	(SPIFLASH_MAGIC_END == ptr->magic_end))
	{
		return ptr;
	}
	else
	{
		return NULL;
	}
#else
   if( (SPIFLASH_MAGIC_FIRST == s_spiflash_type_config.magic_first) &&
 	   (SPIFLASH_MAGIC_SECOND == s_spiflash_type_config.magic_second) &&
 	   (SPIFLASH_MAGIC_END == s_spiflash_type_config.magic_end))
   	{
        return (SPIFLASH_TYPE_CONFIG_T* )&s_spiflash_type_config;
    }
    else
    {
    	return NULL;
    }
#endif
}



uint32 SPIFLASH_GetOperatorDataAddress(void)
{
   return s_platform_patitiion_config.operator_data_addr;
}


uint32 SPIFLASH_GetOperatorDataSize(void)
{
   return s_platform_patitiion_config.operator_data_size;
}

uint32 SPIFLASH_GetCrystalAgingAddress(void)
{
   return s_platform_patitiion_config.crystal_aging_addr;
}


uint32 SPIFLASH_GetCrystalAgingSize(void)
{
   return s_platform_patitiion_config.crystal_aging_size;
}

uint32 SPIFLASH_GetImsToggelCfgAddress(void)
{
   return s_platform_patitiion_config.ims_toggel_cfg_addr;
}


uint32 SPIFLASH_GetImsToggelCfgSize(void)
{
   return s_platform_patitiion_config.ims_toggel_cfg_size;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// end flash_cfg.c
