/******************************************************************************
 ** File Name:      master_img_gen.h                                          *
 ** Author:         Hongliang Xin                                             *
 ** DATE:           09/16/2009                                                *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    Implemention of master_img_gen.h                          *    
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/16/2009     Hongliang Xin    Create.                                   *
 ******************************************************************************/

#include <stdlib.h>
#include "master_img_gen.h"

#ifndef __BIG_ENDIAN
#define BYTESWITCH(s) s = ((((s) >> 8) & 0xff) | (((s) << 8) & 0xff00))
#else
#define BYTESWITCH(s) 
#endif


extern uint8 *RB_GetBuffer();

static const uint8 MAGIC[] = { 'S', 'T', 'B', 'P' };

void * MakeMasterImageHeader(uint32 ps_size,uint32 *pimg_hdr_size, int32 page_type )
{

	int32  sects_per_block;
	uint32 buf_size = 0;
	uint32 boot_size= 0;
	BOOT_PARAM * pBoot;
	OS_INFO    * pOS;

	BOOT_PARAM * pBootBak;
	OS_INFO    * pOSBak;


	void * pbuf = NULL;

	sects_per_block = (SMALL_PAGE == page_type) ? 32 : 256;
	boot_size = sizeof(BOOT_PARAM) + sizeof( OS_INFO );

	buf_size = sects_per_block * SECTOR_SIZE;

	pbuf = RB_GetBuffer();
	memset(pbuf,0xFF,buf_size);

	//init boot param	
	pBoot = (BOOT_PARAM *)pbuf;
	memset(pBoot,0,sizeof(BOOT_PARAM));
	memcpy( pBoot->Magic, MAGIC, sizeof(pBoot->Magic) );
	pBoot->Size = (uint16)sizeof( BOOT_PARAM );
	BYTESWITCH(pBoot->Size);
	
	pBoot->TotalSize = (uint16)boot_size;
    BYTESWITCH(pBoot->TotalSize);
	
	//time(&pBoot->timeStamp);
	pBoot->TraceOn   = ( uint8 )1;
	pBoot->Reserved  = ( uint8 )0;
	pBoot->CurrentOS = ( uint8 )0;
	pBoot->NumOfOS   = ( uint8 )1;

	pBoot->SizeOfOSInfo =  (uint16)sizeof( OS_INFO );
    BYTESWITCH(pBoot->SizeOfOSInfo);
	
	pBoot->OSOffset  = (uint16) sizeof( BOOT_PARAM );
    BYTESWITCH(pBoot->OSOffset);

	//init OS info
	pOS = (OS_INFO*)(((uint8 *)pbuf) + sizeof(BOOT_PARAM));
	memset(pOS,0,sizeof(OS_INFO));

	pOS->Offset = (uint16)sects_per_block;
    BYTESWITCH(pOS->Offset);

	
	pOS->Size = (uint16)(( ps_size + SECTOR_SIZE - 1 ) / SECTOR_SIZE );
	BYTESWITCH(pOS->Size);

	pBootBak = (BOOT_PARAM*)(((uint8 *)pbuf) + buf_size/2);
	pOSBak = (OS_INFO*)(((uint8 *)pBootBak) + sizeof(BOOT_PARAM));
	memcpy(pBootBak,pBoot,sizeof(BOOT_PARAM));
	memcpy(pOSBak,pOS,sizeof(OS_INFO));

	*pimg_hdr_size = buf_size;

	return pbuf;
	
}



