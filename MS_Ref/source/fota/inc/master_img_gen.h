/******************************************************************************
 ** File Name:      master_img_gen.h                                          *
 ** Author:         Hongliang Xin                                             *
 ** DATE:           09/16/2009                                                *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains general interface to make       *    
 **					kernal image header	                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/16/2009     Hongliang Xin    Create.                                   *
 ******************************************************************************/

#ifndef _MASTER_IMG_GEN_H__
#define _MASTER_IMG_GEN_H__

#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


#define SMALL_PAGE		0
#define LARGE_PAGE		1

#define SECTOR_SIZE			512

typedef struct _OS_INFO {
	int8    Name[24];
	int8    Description[48];
	int8	Version[24];
	uint16	Offset;  // in sectors
	uint16	Size;    // in sectors
} OS_INFO, * POS_INFO;

typedef struct _BOOT_PARAM {
	uint8	Magic[4];
	uint16  Size;
	uint16  TotalSize;
	long    timeStamp;
	uint8	TraceOn;
	uint8	Reserved;
	uint8	CurrentOS;
	uint8	NumOfOS;
	uint16  SizeOfOSInfo;
	uint16  OSOffset;
} BOOT_PARAM, * PBOOT_PARAM;
void * MakeMasterImageHeader(uint32 ps_size,uint32 *pimg_hdr_size, int32 page_type );


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // _MASTER_IMG_GEN_H__