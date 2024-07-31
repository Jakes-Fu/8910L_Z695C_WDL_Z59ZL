/******************************************************************************            
 ** File Name:      ua.h                                           			  *
 ** Description:														 	  *
 ** 				                                                          *
 ** Author:         Rui.Zhang                                              	  *
 ** DATE:           21/07/2009                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************									
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME             DESCRIPTION                              *
 ** 11/23/2006      Rui.Zhang        Create									  *
 ******************************************************************************/		
 
 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "RB_ImageUpdate.h"
#include "RbErrors.h"


#include "sci_ftl_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


#define VOLUME_NUMBER		0
#ifdef VECTOR_TABLE_ADDR_HIGH
/* address define according to kernel region base in scf file*/
#define SDRAM_BASE_ADDR     0x400000
#else
#define SDRAM_BASE_ADDR     0x0
#endif


typedef struct 
{
    uint32       nID;           /* partition entry ID                        */
    uint32       nAttr;         /* Attribute                                 */
    uint32       n1stVbn;       /* 1st virtual block number                  */
    uint32       nNumOfBlks;    /* # of blocks                               */
    SCI_FTL_HANDLE ftlHandle;   /* Operation handle of the partition         */
} UAPartEntry_T;

typedef struct 
{
    uint32        nNumOfPartEntry;	/* # of partition entry          */
    UAPartEntry_T  stPEntry[SCI_FTL_MAX_PARTS_PER_NAND];
} UAPartI_T;



PUBLIC int ReadNandflashBysector(unsigned char * dst, unsigned int src, unsigned int size);

uint32 Load_FixnvEntry(void);
uint32  update_fixnv_image(void);
uint32	erase_running_nv_partition(void);
uint32 Update_Kernel_Head(uint8 *buffer,uint32 length);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
