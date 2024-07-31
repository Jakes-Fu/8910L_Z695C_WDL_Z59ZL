/******************************************************************************
 ** File Name:      mem_cfg.c                                                 *
 ** Author:         Lin.liu                                                   *
 ** DATE:           2004-04-19                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the memory config                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/19/2004     Lin.liu          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 #include "sci_types.h"
 #include "threadx_os.h"
 #include "os_api.h"
 #include "emc_drv.h"
 #include "sc_reg.h"
 #include "chng_freq.h"
 #include "mem_dump.h"
 #include "resparam.h"
  #include "lcd_cfg.h"
 #include  "mmu.h"
 #include "spiflash_spec.h" 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#ifdef GPRS_SUPPORT
#define SINGLE_SIM_BASE_MEMORY_SIZE   (85*1024)
#else
#define SINGLE_SIM_BASE_MEMORY_SIZE   (65*1024)
#endif

#ifdef MMI_MULTI_SIM_SYS_DUAL
#define  BASE_SIM_MEMORY_SIZE   (SINGLE_SIM_BASE_MEMORY_SIZE)
#elif defined(MMI_MULTI_SIM_SYS_TRI)
#define  BASE_SIM_MEMORY_SIZE   (2*SINGLE_SIM_BASE_MEMORY_SIZE)
#elif defined(MMI_MULTI_SIM_SYS_QUAD)
#define  BASE_SIM_MEMORY_SIZE   (3*SINGLE_SIM_BASE_MEMORY_SIZE)
#else
#define  BASE_SIM_MEMORY_SIZE   0
#endif

#define  MAX_PHY_IRAM_MEM_SIZE      (8*1024)

#if defined(VM_SUPPORT) &&  defined(BZP_SUPPORT)
#ifdef VM_TABLE_CONFIG_TINY
#define  MAX_PHY_SYS_MEM_SIZE       (112*1024)   //include MMU Page Table + code swap memory
#define  MAX_PHY_MMU_SWAP_SIZE		(64*1024)
#define  MAX_PHY_MMU_MEM_SIZE       (MAX_PHY_SYS_MEM_SIZE)
#elif defined(VM_TABLE_CONFIG_SMALL)
#define  MAX_PHY_SYS_MEM_SIZE       (448*1024)   //include MMU Page Table + code swap memory
#define  MAX_PHY_MMU_SWAP_SIZE      (8*1024)
#define  MAX_PHY_MMU_MEM_SIZE       (MAX_PHY_SYS_MEM_SIZE)
#else
#define  MAX_PHY_SYS_MEM_SIZE       (512*1024)   //include MMU Page Table + code swap memory
#define  MAX_PHY_RES_MEM_SIZE       (512*1024)
#define  MAX_PHY_MMU_MEM_SIZE       (MAX_PHY_SYS_MEM_SIZE + MAX_PHY_RES_MEM_SIZE)
#endif
#else
#define  MAX_PHY_SYS_MEM_SIZE       (32 * 1024)   //include MMU Page Table + code swap memory
#define  MAX_PHY_RES_MEM_SIZE       (0)
#define  MAX_PHY_MMU_SWAP_SIZE   (4 * 1024)
#define  MAX_PHY_MMU_MEM_SIZE       (MAX_PHY_SYS_MEM_SIZE) // Must aligned 16KB
#endif

CONST uint32 mem_code_start_add = 0x60000000;
CONST uint32 mem_code_size      = 0x00A80000;

CONST uint32 mem_ro_start_add   = mem_code_start_add;
CONST uint32 mem_ro_size        = mem_code_size;



#define  HW_SRAM_START_ADDR          (0x80000000)

#define MEM_MODEM_START_VIRADDR    0x80000000
#define MEM_MODEM_START_PHYADDR   0x80000000
#define MEM_MODEM_SIZE                       0x01000000

//#ifdef SRAM_SIZE_128MBIT
//#ifdef AP_CP_RAM_SIZE_40X88
#define SRAM_CODE_START_ADDR    0x80B00000
#define MEM_AP_SIZE             0x00500000  
//#else
//#define SRAM_CODE_START_ADDR    0x80C00000
//#define MEM_AP_SIZE             0x00400000  		// Set to 4M Bytes
//#endif

//#else
//#define  SRAM_CODE_START_ADDR       (0x80D00000) 
//#define  MEM_AP_SIZE                (0x00500000) 

//#endif

CONST uint32 mem_ap_sram_start_add   = SRAM_CODE_START_ADDR;
CONST uint32 mem_ap_sram_size        = MEM_AP_SIZE;


#ifdef BLUETOOTH_SUPPORT
#define  PHY_BT_CODE_SIZE					0x3B000
#endif

#if 0//def MULTI_BIN_SUPPORT
#define    MEM_RO2_START_ADDR     HW_SRAM_START_ADDR
#ifdef  MULTI_BIN_RAM_32MBIT_LEVEL1    //16x32
#define    MEM_RO2_CODE_SIZE      0x200000
#elif defined(MULTI_BIN_RAM_32MBIT_LEVEL2)  //32x32
#define    MEM_RO2_CODE_SIZE      0x00
#elif defined(MULTI_BIN_RAM_32MBIT_LEVEL3)  //32x32
#define    MEM_RO2_CODE_SIZE      0x00
#elif defined(MULTI_BIN_RAM_32MBIT_LEVEL4)  //32x32
#define    MEM_RO2_CODE_SIZE      0x00
#else
#ifdef IM_SIMP_CHINESE_SUPPORT
#define    MEM_RO2_CODE_SIZE      0x300000
#else
#define    MEM_RO2_CODE_SIZE      0x200000
#endif  
#endif  
CONST uint32 mem_ro2_start_add  = MEM_RO2_START_ADDR;   
CONST uint32 mem_ro2_size       = MEM_RO2_CODE_SIZE;   
CONST uint32 mem_rw_start_add   = MEM_RO2_START_ADDR + MEM_RO2_CODE_SIZE;
#if defined(VM_TABLE_CONFIG_TINY) || defined(VM_TABLE_CONFIG_SMALL)
CONST uint32 mem_rw_size        = SRAM_TOTAL_SIZE - MEM_RO2_CODE_SIZE - MAX_PHY_MMU_MEM_SIZE - MAX_PHY_MMU_SWAP_SIZE;
#else
CONST uint32 mem_rw_size        = SRAM_TOTAL_SIZE - MEM_RO2_CODE_SIZE - MAX_PHY_MMU_MEM_SIZE;
#endif
#else
#if  defined( MULTI_FLASH_SUPPORT)
CONST uint32 mem_ro2_start_add    = 0x70000000;   // ro2 not use in 6800H, just dummy
CONST uint32 mem_ro2_size         = 0x00800000;             // ro2 not use in 6800H, just dummy
#endif
CONST uint32 mem_rw_start_add   = SRAM_CODE_START_ADDR;
#if defined(VM_TABLE_CONFIG_TINY) || defined(VM_TABLE_CONFIG_SMALL) 
CONST uint32 mem_rw_size        = MEM_AP_SIZE- MAX_PHY_MMU_MEM_SIZE - MAX_PHY_MMU_SWAP_SIZE;
#else
CONST uint32 mem_rw_size        = MEM_AP_SIZE - MAX_PHY_MMU_MEM_SIZE;
#endif
#endif

CONST uint32 mem_all_size       = SRAM_TOTAL_SIZE;

CONST uint32 mem_iram_start_add = 0x00800000;//0x40000000;
CONST uint32 mem_iram_size      = 0x0040000;

CONST uint32 mem_irom_start_add = 0;//0x10000000;
CONST uint32 mem_irom_size      = 0x10000;//0x0006000;

CONST uint32 system_stack_size  = (16 * 1024);  //16K
CONST uint32 fiq_stack_size     = (2048);

CONST uint32 g_fiq_stack_size		= fiq_stack_size;
CONST uint32 g_irq_stack_size		= 2048;
CONST uint32 g_undefine_stack_size  = 512;
CONST uint32 g_abort_stack_size		= 2048;
CONST uint32 g_sys_stack_size		= 32; 
CONST uint32 g_timer_stack_size		= 2048;

#if 0
CONST uint32 g_vfp_saved_regs_size  = 136*16;
#pragma arm section zidata = "VFP_SAVED_REG_AREA"
    uint8 g_vfp_reg_list[g_vfp_saved_regs_size];
#pragma arm section zidata	
#endif

#pragma arm section zidata = "SYS_STACK_AREA"
    uint8 g_system_stack[system_stack_size];
#pragma arm section zidata	

#pragma arm section zidata = "FIQ_STACK_AREA"
    uint8 g_fiq_stack[fiq_stack_size];
#pragma arm section zidata	

CONST uint32    g_sram_start	        = HW_SRAM_START_ADDR;
CONST uint32	g_sram_size		        = SRAM_TOTAL_SIZE;

//the system memory allocation define,the allocation map defeine is different when
//chip,module and the flash chip used in the module is different for the production 
//                                                  // Typical Define 
CONST uint32    g_svc_stack_limit_add 	= (uint32)&g_system_stack[system_stack_size];
CONST uint32 	const_RAM_LimitOffset	= (uint32)g_system_stack;    

//the ZW and ZI data should not excced the RAM LIMIT,and RAM after RAM_LIMIT
//is used as stack of exceptions as FIQ,IRQ,ABORT and so on
CONST uint32    g_mem_limit 			= (uint32)g_system_stack;
CONST uint32    g_sys_stack_base        = (uint32)g_system_stack;
CONST uint32 	g_fiq_stack_base 		= (uint32)g_fiq_stack;   
    
//the RO monitor section size
CONST uint32    g_ro_monitor_size   = mem_code_size;


/* Nv itme max number defined by customer */
#ifdef NV_PRTITION_TINY
CONST uint16 NV_MAX_ID = 1500;  
#else
CONST uint16 NV_MAX_ID = 3529;  
#endif

// Each block is 1024byte(defined in efs_config.h), so the total file sytem space is 
//  1024 * g_file_system_data_block_num
CONST uint32 g_file_system_data_block_num = 1;  /*200->1*/
// The max file number that support.
CONST uint32 g_file_system_file_num       = 1; /*32->1*/

CONST uint32 g_nvitem_cache_enabled		  = 1;

//SFS memory config Start Jason.wu
uint32 g_sfs_bufMgr_min_count_of_clusBuf  = 3;	//the counter of memory block in device memory manager.value range is >=1,
uint32 g_sfs_bufMgr_min_buf_size          = 10;	//  minimal  size of buf block.unit is sector.the range is [1,128]
uint32 g_sfs_bufMgr_max_idle_write_size   = 20;	//unit is sector.when handset is In idle, SFS will write a number of block data to physical storage.the value range is >= 1
//SFS memory config End

// Memory Debug Config
#ifdef MEMORY_DEBUG_SUPPORT
#define DEBUG_MEM_SIZE  0xA000		  //40KB
#else
#define DEBUG_MEM_SIZE  0x0	   	      //0KB
#endif

// Be careful, the array below must be 4 bytes alignment.
// Byte heap size
//@Ivan.yin for GPRS 2004-1-13
#ifdef WIN32
	#define BYTE_HEAP_SIZE          (1700 * 1024) //   //bruce, 1700->1200

#else
#if defined(SRAM_SIZE_128MBIT)
        #define     SCI_STATIC_HEAP_SIZE_THRESHOD     0x14000   //80KB
	 #define     SCI_APP_HEAP_SIZE_THRESHOD           0x10000   //64KB
	
	 #define     SCI_SYSTEM_SPACE_ALLOC_THRESHOD   0x4000      //16KB
	 #define     SCI_SYSTEM_MMAP_THRESHOLD         0xC000     //48KB

	#ifdef MULTI_BIN_RAM_32MBIT_LEVEL1  //16x32
	    #define  	MAX_STATIC_SPACE_SIZE             (280*1024 + BASE_SIM_MEMORY_SIZE ) //SFS decrease memory 300K, Alarm decrease memory 30K
	    #define    	MAX_SYSTEM_SPACE_SIZE  	      (800 * 1024 - DEBUG_MEM_SIZE)
	    #define    	MAX_BASE_SPACE_SIZE  	             (50 * 1024) 
	    #define    	MAX_APP_SPACE_SIZE  	             (300* 1024)
	#else     //32x32
	   	#define  	MAX_STATIC_SPACE_SIZE             (400*1024 + BASE_SIM_MEMORY_SIZE ) //SFS decrease memory 300K, Alarm decrease memory 30K
	   #ifdef MULTI_BIN_SUPPORT	

        #ifdef MULTI_BIN_RAM_32MBIT_LEVEL3
            #define    	MAX_SYSTEM_SPACE_SIZE  	          (1400 * 1024 - DEBUG_MEM_SIZE)
       #elif defined(MULTI_BIN_RAM_32MBIT_LEVEL4)
            #define    	MAX_SYSTEM_SPACE_SIZE  	          (1500 * 1024 - DEBUG_MEM_SIZE)	
        #else
	     #define    	MAX_SYSTEM_SPACE_SIZE  	          (1600 * 1024 - DEBUG_MEM_SIZE)
        #endif

       #else
			#ifdef GPS_SUPPORT
				#define 	SAVE_RAM_SPACE		(80*1024)
			#else
				#define 	SAVE_RAM_SPACE		0
			#endif
                #ifdef MULTI_SIM_SYS_TRI
                    #define   MAX_SYSTEM_SPACE_SIZE             (2000 * 1024 - 128 * 1024 - DEBUG_MEM_SIZE - SAVE_RAM_SPACE)
                #else

				#if defined(BZP_SUPPORT)

			        #define   MAX_SYSTEM_SPACE_SIZE             (2380 * 1024 - DEBUG_MEM_SIZE - SAVE_RAM_SPACE)
				#else

				#define   MAX_SYSTEM_SPACE_SIZE             (2800 * 1024 - DEBUG_MEM_SIZE - SAVE_RAM_SPACE)
				#endif

                                                  /*  1700 -> BYTE_HEAP_SIZE = 2554880 */
                                                  /*  2000 -> BYTE_HEAP_SIZE = 2862080*/							

			#endif
	   #endif	
	    #define    	MAX_BASE_SPACE_SIZE  	          (50 * 1024) 
	    #define    	MAX_APP_SPACE_SIZE  	  (480* 1024)    //     (380* 1024)
	#endif	

#elif defined(SRAM_SIZE_256MBIT)
       #define     SCI_STATIC_HEAP_SIZE_THRESHOD     0x14000   //80KB
	#define     SCI_APP_HEAP_SIZE_THRESHOD        0x10000   //64KB
	
	 #define     SCI_SYSTEM_SPACE_ALLOC_THRESHOD   0x4000      //16KB
	 #define     SCI_SYSTEM_MMAP_THRESHOLD         0xC000     //48KB

	#ifdef MULTI_BIN_RAM_32MBIT_LEVEL1  //16x32
	   	#define  	MAX_STATIC_SPACE_SIZE             (280*1024 + BASE_SIM_MEMORY_SIZE ) //SFS decrease memory 300K, Alarm decrease memory 30K
	    #define    	MAX_SYSTEM_SPACE_SIZE  	          (800 * 1024 - DEBUG_MEM_SIZE)
	    #define    	MAX_BASE_SPACE_SIZE  	          (50 * 1024) 
	    #define    	MAX_APP_SPACE_SIZE  	          (300* 1024)
	#else     //32x32
	   	#define  	MAX_STATIC_SPACE_SIZE             (400*1024 + BASE_SIM_MEMORY_SIZE ) //SFS decrease memory 300K, Alarm decrease memory 30K
	   #ifdef MULTI_BIN_SUPPORT	

        #ifdef MULTI_BIN_RAM_32MBIT_LEVEL3
            #define    	MAX_SYSTEM_SPACE_SIZE  	          (1400 * 1024 - DEBUG_MEM_SIZE)
       #elif defined(MULTI_BIN_RAM_32MBIT_LEVEL4)
            #define    	MAX_SYSTEM_SPACE_SIZE  	          (1500 * 1024 - DEBUG_MEM_SIZE)	
        #else
			#define    	MAX_SYSTEM_SPACE_SIZE  	          (1600 * 1024 - DEBUG_MEM_SIZE)
        #endif

       #else
			#ifdef GPS_SUPPORT
				#define 	SAVE_RAM_SPACE		(80*1024)
			#else
				#define 	SAVE_RAM_SPACE		0
			#endif

				#if defined(BZP_SUPPORT)

	    				#define    MAX_SYSTEM_SPACE_SIZE  	          (2380 * 1024 - DEBUG_MEM_SIZE - SAVE_RAM_SPACE)
				#else

				#define   MAX_SYSTEM_SPACE_SIZE             (2850 * 1024 - DEBUG_MEM_SIZE - SAVE_RAM_SPACE)
				#endif

	   #endif	
	    #define    	MAX_BASE_SPACE_SIZE  	          (50 * 1024) 
	    #define    	MAX_APP_SPACE_SIZE  	          (380* 1024)
	#endif	

#else
    #error
#endif
	#define 	BYTE_HEAP_SIZE    			      (MAX_STATIC_SPACE_SIZE  + MAX_SYSTEM_SPACE_SIZE + MAX_BASE_SPACE_SIZE + MAX_APP_SPACE_SIZE)
#endif

	/*Define the memory threshod size in the byte heap, when the allocation memory size less than 
	  the threshod, it will be allocated from the heap low address, or it will be allocated from the
	  heap high address*/
CONST uint32  g_static_heap_size_threshod = SCI_STATIC_HEAP_SIZE_THRESHOD;
CONST uint32  g_app_heap_size_threshod    = SCI_APP_HEAP_SIZE_THRESHOD;
CONST uint32  g_system_space_alloc_threshod    = SCI_SYSTEM_SPACE_ALLOC_THRESHOD;
CONST uint32  g_system_mmap_threshod      = SCI_SYSTEM_MMAP_THRESHOLD;


CONST uint32 max_static_space_size  =  MAX_STATIC_SPACE_SIZE;
CONST uint32 max_system_space_size  =  MAX_SYSTEM_SPACE_SIZE;
CONST uint32 max_app_space_size     =  MAX_APP_SPACE_SIZE;
CONST uint32 byte_head_size     	=  BYTE_HEAP_SIZE;
CONST uint32 max_base_space_size    =  MAX_BASE_SPACE_SIZE;

#pragma arm section zidata = "BYTE_HEAP_AREA"
uint32    BYTE_HEAP_ADDR[ byte_head_size >> 2 ];
#pragma arm section zidata 

#pragma arm section zidata = "LCD_FRAME_BUF_AREA"
//modified, @robert.wang, 09-8-15, cr146714
//__align(32) uint16 s_main_lcd_buffer[LCD_WIDTH*LCD_HEIGHT];
//__align(256) uint16 s_main_lcd_buffer[((LCD_WIDTH+15)/16*16)*((LCD_HEIGHT+7)/8*8)];
//end cr146714
__align(32) uint16 s_sub_lcd_buffer[SUBLCD_WIDTH * SUBLCD_HEIGHT];
#pragma arm section zidata	


#if defined(VM_TABLE_CONFIG_TINY) || defined(VM_TABLE_CONFIG_SMALL)
//#pragma arm section zidata = "PHY_MMU_IRAM_SWAP"
//LOCAL  uint32    s_phy_iram_swap_buffer[ MAX_PHY_IRAM_MEM_SIZE >> 2 ];/*lint -esym(551,s_phy_iram_swap_buffer)*//*lint -esym(728,s_phy_iram_swap_buffer)*/
//#pragma arm section zidata 
#pragma arm section zidata = "PHY_MMU_PSRAM_SWAP"
LOCAL  uint32    s_phy_psram_swap_buffer[ MAX_PHY_MMU_SWAP_SIZE >> 2 ];
#pragma arm section zidata
#endif

//Physic Memory Layerout
#define  MEM_RO_START_ADDR      mem_ro_start_add
#define  MEM_RO_CODE_SIZE       (MAX_HW_FLASH_SIZE + DEBUG_FLASH_SIZE)
#define  MEM_RW_START_ADDR      mem_rw_start_add
#define  MEM_RW_AREA_SIZE       mem_rw_size

#if  defined( MULTI_FLASH_SUPPORT)
#define MEM_RO2_START_ADDR    mem_ro2_start_add
#define MEM_RO2_CODE_SIZE      mem_ro2_size
#endif


#ifdef VM_SUPPORT

#define  MMU_PTETABLE_MEM_SIZE(_VIR_SPACE_SIZE_, _PAGE_SIZE_)   (((_VIR_SPACE_SIZE_)/(_PAGE_SIZE_)) << 2)

#if defined(BZP_SUPPORT)
//Virtual Image Define
#ifdef  FLASH_SIZE_16MBIT
#define  VIR_IMAGE_START_ADDR          0xD0000000
#define  VIR_IMAGE_SIZE                (MAX_HW_FLASH_SIZE + 0x100000)  /* !should be larger than the USER_IMAGE_LENGTH*/
#define  VIR_RESOURCE_STRAT_ADDR       (VIR_IMAGE_START_ADDR + VIR_IMAGE_SIZE)
#define  VIR_RESOURCE_SIZE             (MAX_HW_FLASH_SIZE >> 1)  /* !should be larger than the Resource LENGTH*/
#else
#define  VIR_IMAGE_START_ADDR          0xD0000000
#define  VIR_IMAGE_SIZE                (MAX_HW_FLASH_SIZE)  /* !should be larger than the USER_IMAGE_LENGTH*/
#define  VIR_RESOURCE_STRAT_ADDR       (VIR_IMAGE_START_ADDR + VIR_IMAGE_SIZE)
#define  VIR_RESOURCE_SIZE             0x100000 //MAX_HW_FLASH_SIZE /* !should be larger than the Resource LENGTH*/
#endif

#else
#define  VIR_IMAGE_START_ADDR          0xD0000000
#define  VIR_IMAGE_SIZE                 (4)  /* !should be larger than the USER_IMAGE_LENGTH*/
#define  VIR_RESOURCE_STRAT_ADDR       (VIR_IMAGE_START_ADDR + VIR_IMAGE_SIZE)
#define  VIR_RESOURCE_SIZE             (4)  //MAX_HW_FLASH_SIZE /* !should be larger than the Resource LENGTH*/
#endif


#define  VIR_STRAT_ADDR               VIR_IMAGE_START_ADDR
#define  VIR_END_ADDR                 (VIR_IMAGE_START_ADDR + VIR_IMAGE_SIZE + VIR_RESOURCE_SIZE)

//Phy Memory Define
#define  UNALIGNED_MEM_PGDTABLE_SIZE    ((MAX_PHY_MMU_MEM_SIZE & (PAGE_SIZE_1M - 1))  +  PAGE_SIZE_1M - 1)& PAGE_SIZE_1M_MASK
#define  MMU_PGDTABLE_MEM_SIZE     (8*4096)
#define  PHY_MMU_TABLE_SIZE_1       (MMU_PGDTABLE_MEM_SIZE + MMU_PTETABLE_MEM_SIZE(UNALIGNED_MEM_PGDTABLE_SIZE, PAGE_SIZE_4K) + MMU_PTETABLE_MEM_SIZE(VIR_IMAGE_SIZE, PAGE_SIZE_4K) + MMU_PTETABLE_MEM_SIZE(VIR_RESOURCE_SIZE, PAGE_SIZE_4K))

#if defined(BZP_SUPPORT)
#define  PHY_MMU_TABLE_SIZE         ((PHY_MMU_TABLE_SIZE_1 + PAGE_SIZE_4K - 1) & (~(PAGE_SIZE_4K - 1)))
#else
#define  PHY_MMU_TABLE_SIZE  (MAX_PHY_MMU_MEM_SIZE)
#endif
//Swap memory define

#ifdef  VM_TABLE_CONFIG_TINY
#define  PHY_RES_SWAP_MEM_SIZE             MAX_PHY_MMU_SWAP_SIZE 
#define  PHY_IMAGE_SWAP_MEM_SIZE           (MAX_PHY_SYS_MEM_SIZE - PHY_MMU_TABLE_SIZE)
#define  PHY_TOTAL_SWAP_MEM_SIZE           (PHY_IMAGE_SWAP_MEM_SIZE)
#define  PHY_MMU_MEM_START_ADDR            (SRAM_CODE_START_ADDR + MEM_RW_AREA_SIZE + MAX_PHY_MMU_SWAP_SIZE)   //Start From 0x750000
#define  PHY_MMU_TABLE_START_ADDR          PHY_MMU_MEM_START_ADDR
#define  PHY_IMAGE_SWAP_MEM_START_ADDR     (PHY_MMU_TABLE_START_ADDR + PHY_MMU_TABLE_SIZE)
#define  PHY_RES_SWAP_MEM_START_ADDR       s_phy_psram_swap_buffer
#elif defined(VM_TABLE_CONFIG_SMALL)
#define  PHY_RES_SWAP_MEM_SIZE             MAX_PHY_MMU_SWAP_SIZE 
#define  PHY_IMAGE_SWAP_MEM_SIZE           (MAX_PHY_SYS_MEM_SIZE - PHY_MMU_TABLE_SIZE)
#define  PHY_TOTAL_SWAP_MEM_SIZE           (PHY_IMAGE_SWAP_MEM_SIZE)
#define  PHY_MMU_MEM_START_ADDR            (SRAM_CODE_START_ADDR + MEM_RW_AREA_SIZE + MAX_PHY_MMU_SWAP_SIZE)   //Start From 0x750000
#define  PHY_MMU_TABLE_START_ADDR          PHY_MMU_MEM_START_ADDR
#define  PHY_IMAGE_SWAP_MEM_START_ADDR     (PHY_MMU_TABLE_START_ADDR + PHY_MMU_TABLE_SIZE)
#define  PHY_RES_SWAP_MEM_START_ADDR       s_phy_psram_swap_buffer
#else
#define  PHY_RES_SWAP_MEM_SIZE             MAX_PHY_RES_MEM_SIZE 
#define  PHY_IMAGE_SWAP_MEM_SIZE           (MAX_PHY_SYS_MEM_SIZE - PHY_MMU_TABLE_SIZE)
#define  PHY_TOTAL_SWAP_MEM_SIZE           (PHY_IMAGE_SWAP_MEM_SIZE +  PHY_RES_SWAP_MEM_SIZE)
#define  PHY_MMU_MEM_START_ADDR            (MEM_RW_START_ADDR + MEM_RW_AREA_SIZE)   //Start From 0x750000
#define  PHY_MMU_TABLE_START_ADDR          PHY_MMU_MEM_START_ADDR
#define  PHY_IMAGE_SWAP_MEM_START_ADDR     (PHY_MMU_TABLE_START_ADDR + PHY_MMU_TABLE_SIZE)
#define  PHY_RES_SWAP_MEM_START_ADDR       (PHY_IMAGE_SWAP_MEM_START_ADDR + PHY_IMAGE_SWAP_MEM_SIZE)   
#endif

#ifdef BLUETOOTH_SUPPORT
#define  PHY_BT_CODE_START_ADDR				(SRAM_CODE_START_ADDR + MEM_RW_AREA_SIZE - PHY_BT_CODE_SIZE)
uint8 *bt_code_addr = (uint8 *)PHY_BT_CODE_START_ADDR;
uint32 bt_code_size = PHY_BT_CODE_SIZE;
#endif


CONST uint32 mem_vm_start_addr       = VIR_STRAT_ADDR;
CONST uint32 mem_vm_end_addr         = VIR_END_ADDR;


#else
#pragma  arm section  zidata = "MMU_PGD_TLB_SECTION"
PGD_T    g_mmu_pgd[MMU_PGD_ITEM_NUM]; //= 0x1FF0000 ;
#pragma  arm section  zidata

#define PAGE_TABLE_ADDR    (g_mmu_pgd)
#define MEM_DP_PAGE_SIZE   (0x4000)
#endif
//


//rw,ro region need align to 4kB(1pages)
//For every demand paging region pages need > 3 (page tyes:1k or 4k).
const static VM_REGION vm_region[] = 
{
	/*--vir base addr-----size----- phy base addr--------type------access----control---- */
    /* rt code kernel img1, 4MB */
    {/*{0x00000000, 0x00400000,  0x00000000,  VM_USE_RTCODE, 0,    0}*/
        MEM_RO_START_ADDR,
        MEM_RO_CODE_SIZE,
        MEM_RO_START_ADDR,
        VM_USE_RTCODE,
        (VM_PROT_RW),
        0
	},

#if  defined( MULTI_FLASH_SUPPORT)
    {/*{0x00000000, 0x00400000,  0x00000000,  VM_USE_RTCODE, 0,    0}*/
        MEM_RO2_START_ADDR,
        MEM_RO2_CODE_SIZE,
        MEM_RO2_START_ADDR,
        VM_USE_RTCODE,
        (VM_PROT_RW),
        0
	},
  #endif
  
#ifdef MULTI_BIN_SUPPORT
#if (!defined(MULTI_BIN_RAM_32MBIT_LEVEL2)  &&  !defined(MULTI_BIN_RAM_32MBIT_LEVEL3)  &&  !defined(MULTI_BIN_RAM_32MBIT_LEVEL4))
    /* rt code kernel img2*/
    {/*{0x04000000, 0x04300000,  0x00000000,  VM_USE_RTCODE, 0,    0}*/
        MEM_RO2_START_ADDR,
        MEM_RO2_CODE_SIZE,
        MEM_RO2_START_ADDR,
        VM_USE_RTCODE,
        (VM_PROT_RW),
        0
	},
#endif	
#endif
     /* rw data */
    {
        (MEM_RW_START_ADDR),
        (MEM_RW_AREA_SIZE),
        (MEM_RW_START_ADDR),
        (VM_USE_UNKNOWN),
        (VM_PROT_RW),
        (VM_CB_WB)
    },

#ifdef BLUETOOTH_SUPPORT
    {
        (PHY_BT_CODE_START_ADDR),
        (PHY_BT_CODE_SIZE),
        (PHY_BT_CODE_START_ADDR),
        (VM_USE_UNKNOWN),
        (VM_PROT_RW),
        (VM_CB_UNCACHED)
    },

#endif
		

#ifndef VM_SUPPORT
 /* page table and pfn, 4MB */
    {
        (PAGE_TABLE_ADDR),
        (MEM_DP_PAGE_SIZE),
        (PAGE_TABLE_ADDR),
        (VM_USE_PAGETABLE),
        (0),
        (0)
    }  
#else

#if defined(VM_TABLE_CONFIG_TINY) || defined(VM_TABLE_CONFIG_SMALL)
	{
        (PHY_RES_SWAP_MEM_START_ADDR),
        (MAX_PHY_MMU_SWAP_SIZE), //The physic Memory size Only For demand paging 
        (PHY_RES_SWAP_MEM_START_ADDR),
        (VM_USE_UNKNOWN),
        (VM_PROT_RW),
        (VM_CB_WT)
    },
#endif
 /* page table and pfn */
    {
        (PHY_MMU_MEM_START_ADDR),
        (PHY_MMU_TABLE_SIZE), //The physic Memory size Only For PGD Table and all PTE Table 
        (PHY_MMU_MEM_START_ADDR),
        (VM_USE_PAGETABLE),
        (PHY_TOTAL_SWAP_MEM_SIZE),//total physic memory swap  size
        (0)
    },
#ifdef BZP_SUPPORT
    /*
	   if user image increaed, so pay attention to modify here in condition "VM_SUPPORT == TRUE"
	*/
    {   
        (VIR_IMAGE_START_ADDR),//vir start
        (VIR_IMAGE_SIZE), //vir size
        (PHY_IMAGE_SWAP_MEM_START_ADDR),//demand paging phy start addr
        (VM_USE_DPVIR), //used for demandpaging region 
        (PHY_IMAGE_SWAP_MEM_SIZE),//page_size 1024,4096
        (PAGE_SIZE_4K)
    },
    {
        (VIR_RESOURCE_STRAT_ADDR), //vir start
        (VIR_RESOURCE_SIZE), //vir size
        (PHY_RES_SWAP_MEM_START_ADDR),//demand paging phy start addr
        (VM_USE_DPVIR), //used for demandpaging region 
        (PHY_RES_SWAP_MEM_SIZE),//phy size :at least > 2 pagesize
        (PAGE_SIZE_4K)//page_size 1024,4096
    },
#endif    
#endif      
};

void MEM_GetVmRegion(const VM_REGION **region, uint32 *num)
{
    *region = vm_region;
    *num = sizeof(vm_region) / sizeof(vm_region[0]);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
