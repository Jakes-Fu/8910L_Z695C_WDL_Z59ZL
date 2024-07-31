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


PUBLIC int ReadNandflashBysector(unsigned char * dst, unsigned int src, unsigned int size);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 