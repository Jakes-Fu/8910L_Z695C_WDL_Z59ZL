/******************************************************************************
 ** File Name:    seine_reg.h                                                 *
 ** Author:       Tom.Wu		                                              *
 ** DATE:         06/15/2007                                                  *
 ** Copyright:    2006 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 06/15/2007    Daniel.Ding     Create.                                     *
 ** 10/23/2007    Tom.Wu     	  Port to SC6600V                             *
 ******************************************************************************/

#ifndef _SEINE_REG_H_
#define _SEINE_REG_H_
//#include "types.h"
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/**----------------------------------------------------------------------------*
**                     Seine EMC Register defination                         **
**----------------------------------------------------------------------------*/
/*----------external memory cotnrol registers----------*/
#define EMC_BASE_ADDRESS        		0x20400000

/*-----------External Memory Register defination----------*/
/*EMC_BASE_ADDRESS=0x20400000*/
#define	EMC_CHANNEL_MODE0			(EMC_BASE_ADDRESS + 0x0000)
#define	EMC_AHB_TIMEOUT				(EMC_BASE_ADDRESS + 0x0004)
#define	EMC_CHANNEL_MODE1			(EMC_BASE_ADDRESS + 0x0008)
#define	EMC_CHANNEL_PRIORITY			(EMC_BASE_ADDRESS + 0x000C)
#define	EMC_CHANNEL_TIMEOUT0			(EMC_BASE_ADDRESS + 0x0010)
#define EMC_CHANNEL_TIMEOUT1			(EMC_BASE_ADDRESS + 0x0014)
#define EMC_CHANNEL_TIMEOUT2			(EMC_BASE_ADDRESS + 0x0018)
#define EMC_DRAM_TIMING0			(EMC_BASE_ADDRESS + 0x001C)
#define EMC_DRAM_TIMING1			(EMC_BASE_ADDRESS + 0x0020)
#define EMC_DRAM_TIMING2			(EMC_BASE_ADDRESS + 0x0024)
#define EMC_SLEEP_CTRL				(EMC_BASE_ADDRESS + 0x0028)
#define EMC_SLEEP_STATUS			(EMC_BASE_ADDRESS + 0x002C)
#define EMC_SOFT_CMD				(EMC_BASE_ADDRESS + 0x0030)
#define EMC_OUTPUT_TIMING0			(EMC_BASE_ADDRESS + 0x0034)
#define EMC_OUTPUT_TIMING1			(EMC_BASE_ADDRESS + 0x0038)
#define EMC_ADDR_MAPPING			(EMC_BASE_ADDRESS + 0x003C)
#define EMC_INBND_CTRL0				(EMC_BASE_ADDRESS + 0x0040)
#define EMC_INBND_CTRL1				(EMC_BASE_ADDRESS + 0x0044)
#define EMC_INBND_CTRL2				(EMC_BASE_ADDRESS + 0x0048)
#define EMC_MISC_CTRL0				(EMC_BASE_ADDRESS + 0x004c)
#define EMC_MISC_CTRL1				(EMC_BASE_ADDRESS + 0x0050)


/**----------------------------------------------------------------------------*
**                         Seine AHB Register defination                      **
**----------------------------------------------------------------------------*/

#define AHB_REG_BASE                0x20300000

#define AHB_SOFT_RST       (AHB_REG_BASE + 0x0000)
#define AHB_MODULE_EN      (AHB_REG_BASE + 0x0004)
#define AHB_SLEEP_CTL0     (AHB_REG_BASE + 0x0008)
#define AHB_SLEEP_CTL1     (AHB_REG_BASE + 0x000C)
#define AHB_GLB_CTL0       (AHB_REG_BASE + 0x0010)
#define AHB_GLB_CTL1       (AHB_REG_BASE + 0x0014)
#define AHB_INT_CTL0       (AHB_REG_BASE + 0x0018)
#define AHB_INT_CTL1       (AHB_REG_BASE + 0x001C)
#define AHB_MPLL_CTL       (AHB_REG_BASE + 0x0020)
#define AHB_DPLL_CTL       (AHB_REG_BASE + 0x0024)
#define AHB_SYNC_CLK_CTL   (AHB_REG_BASE + 0x0040)
#define AHB_CLK_EN0        (AHB_REG_BASE + 0x0044)

#define AHB_CLK_INV        (AHB_REG_BASE + 0x004C)
#define AHB_CLK_DIV0       (AHB_REG_BASE + 0x0050)
#define AHB_CLK_DIV1       (AHB_REG_BASE + 0x0054)
#define AHB_CLK_DIV2       (AHB_REG_BASE + 0x0058)

#define AHB_PLL_CTL        (AHB_REG_BASE + 0x0070)
#define AHB_CLK_SEL        (AHB_REG_BASE + 0x0074)
#define AHB_POWER_CTL_PROD (AHB_REG_BASE + 0x0080)
#define AHB_DLY_LINE_CTL1  (AHB_REG_BASE + 0x0084)

#define AHB_CHIP_TYPE      (AHB_REG_BASE + 0x00FC)

#define		DDR_CLK_20M	0
#define		DDR_CLK_50M	1
#define		DDR_CLK_80M	2
#define		DDR_CLK_100M	3


#define		EMC_DEV_DDR	0
#define		EMC_DEV_SDR	1

#define		MEMORY_DENSITY_128M	0
#define		MEMORY_DENSITY_256M	1
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

//void SDRAM_Init(uint32 emc_dev,uint32 capacity,uint32 clock)

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/



#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif// End 
