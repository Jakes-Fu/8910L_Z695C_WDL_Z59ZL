#ifndef _SEINE_SDRAM_DRV_H_
#define _SEINE_SDRAM_DRV_H_

#include "kd.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VALUE_UNIT_CLK      0
#define VALUE_UNIT_TIM      1
#define VALUE_UNIT_DEF      0xFF

/* data width */
#define	DW_16BIT			0
#define DW_32BIT			1

/* row size */

#define	RS_04Kb				7
#define	RS_08Kb				8
#define	RS_16Kb				9


#define		MEMORY_DENSITY_128M	0
#define		MEMORY_DENSITY_256M	1

#define		DEV_CLK_20M		(20*1000*1000)
#define		DEV_CLK_50M		(50*1000*1000)
#define		DEV_CLK_80M		(80*1000*1000)
#define		DEV_CLK_100M	(100*1000*1000)

#define	SOFT_CMD_PRECHG			(1<<16)
#define SOFT_CMD_AUTOREF		(1<<17)
#define	SOFT_CMD_LOADREG		(1<<18)
#define	SOFT_CMD_SELFREG		(1<<19)
#define	SOFT_CMD_RESUME			(1<<20)
#define	SOFT_CMD_SETCKE			(1<<21)

void SDRAM_Init(void);

#ifdef   __cplusplus
    }
#endif
#endif