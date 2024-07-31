/******************************************************************************            
 ** File Name:     RB_ImageUpdate.c                                           *
 ** Description:	this file is a implementation of RedBend request 													 	 				       				*
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
#include "ms_ref_fota_trc.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "sci_types.h"
#include "RB_ImageUpdate.h"
#include "RbErrors.h"
#include "log.h"
#include "XsrTypes.h"
#include "BML.h"
#include "STL.h"
#include "Nand_partinfo.h"
#include "ua.h"
#include "LLD.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
 static  int _g_BlockSize_=0;
#define	UART0_BASE_ADDR  	(0x84000000)
#define	UART_NUM			(1)

extern  unsigned int g_SctsPerBlk;

extern  uint32 NANDCTL_Read_ID(uint8 *pID);
extern void ShowProgress(uint32 current, uint32 total);

__align(8)  unsigned char g_buffer[1*1024*1024] = {0};
#ifdef _ENABLE_TEST_
uint8 buffer[0x20000];
#endif
uint8 g_data_cache1[128*1024];

uint8 *RB_GetBuffer()
{
	return g_buffer;
}
long RB_GetDelta(
	void *pbUserData,				    /* User data passed to all porting routines */
	unsigned char *pbBuffer,			/* pointer to user buffer */
    unsigned long dwStartAddressOffset, /* offset from start of delta file */
    unsigned long dwSize)               /* buffer size in bytes */
{

	int             ret = 0;
    XSRPartEntry    part_entry;
    uint32 sector_offset = 0;
    uint32 sector_count = 0;
    uint32 byte_offset = 0;
    
    UA_TRACE(("enter RB_GetDelta\r\n"));

    memset(g_buffer,0,sizeof(g_buffer));
    
    
    ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_DU, &part_entry);
    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"RB_GetDelta: Failed to load Delta partition entry (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_78_112_2_18_1_31_23_15,(uint8*)"d", ret);
        return 1;
    }
    
    UA_TRACE(("RB_GetDelta: n1stVbn = %d, nNumOfBlks = %d, g_SctsPerBlk = %d\r\n", part_entry.n1stVbn, 
        part_entry.nNumOfBlks, g_SctsPerBlk));
    UA_TRACE(("RB_GetDelta: dwStartAddressOffset = %d, dwSize = %d\r\n", dwStartAddressOffset, 
        dwSize));
   
    sector_offset = dwStartAddressOffset/XSR_SECTOR_SIZE;
    byte_offset = dwStartAddressOffset - (sector_offset*XSR_SECTOR_SIZE);
    sector_count = ((byte_offset+dwSize)/XSR_SECTOR_SIZE)+1;
    
    UA_TRACE(("RB_GetDelta: sector_offset=%ld, byte_offset=%ld, sector_count=%ld\r\n", sector_offset, byte_offset, sector_count));
               
    ret = ReadNandflashBysector(g_buffer, part_entry.n1stVbn*g_SctsPerBlk + sector_offset, sector_count);
    
    memcpy(pbBuffer, g_buffer+byte_offset, dwSize);
                
    if (0 != ret) {
    	//UA_TRACE_ERR:"RB_GetDelta: copy data failed.\r\n"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_98_112_2_18_1_31_23_16,(uint8*)"");
        return 1;
    }   
    
    
    
    
    return 0;      
    
}

long RB_ReadBackupBlock(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned char *pbBuffer,			/* pointer to user buffer in RAM where the data will be copied */
	unsigned long dwBlockAddress,		/* address of data to read into RAM. Must be inside one of the backup buffer blocks */
	unsigned long dwSize)				/* buffer size in bytes */
{

	int             ret;
    XSRPartEntry    part_entry;
    STLInfo 	info;
    uint32 sector_offset = 0;
    uint32 sector_count = 0;
    uint32 byte_offset = 0;
    
	info.nSamBufFactor = 10;
	info.bASyncMode = FALSE32;
	
    UA_TRACE(("enter RB_ReadBackupBlock\r\n"));
    
    memset(g_buffer,0,sizeof(g_buffer));
    
    
        
    UA_TRACE(("RB_ReadBackupBlock: n1stVbn = %d, nNumOfBlks = %d, g_SctsPerBlk = %d\r\n", part_entry.n1stVbn, 
        part_entry.nNumOfBlks, g_SctsPerBlk));
    UA_TRACE(("RB_ReadBackupBlock: dwStartAddressOffset = 0x%x, dwSize = %d\r\n", dwBlockAddress, dwSize));
   
    sector_offset = dwBlockAddress/XSR_SECTOR_SIZE;
    byte_offset = dwBlockAddress - sector_offset*XSR_SECTOR_SIZE;
    sector_count = ((byte_offset+dwSize)/XSR_SECTOR_SIZE)+1;
   
    UA_TRACE(("RB_ReadBackupBlock: sector_offset=%ld, byte_offset=%ld, sector_count=%ld\r\n", sector_offset, byte_offset, sector_count));
               
    ret = ReadNandflashBysector(g_buffer, sector_offset, sector_count);

    if (BML_SUCCESS != ret) {
    	//UA_TRACE_ERR:"RB_ReadBackupBlock:read data failed (0x%x).\r\n"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_145_112_2_18_1_31_23_17,(uint8*)"d",ret);
        return 1;
    }
    
    memcpy(pbBuffer, g_buffer+byte_offset, dwSize);
             
    
    
    return 0;   
}


long RB_WriteBackupBlock(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned long dwBlockStartAddress,	/* address of the block to be updated */
	unsigned char *pbBuffer)  	        /* RAM to copy data from */		
{

	uint8             ret;
    STLInfo 	info;
    uint32 sector_offset = 0;
	uint32 block_offset = 0;
		
	
	info.nSamBufFactor = 10;
	info.bASyncMode = FALSE32;
	
	UA_TRACE(("enter RB_WriteBackupBlock\r\n"));	
    
    sector_offset = dwBlockStartAddress/XSR_SECTOR_SIZE;
    block_offset = sector_offset/g_SctsPerBlk;
    
    UA_TRACE(("RB_WriteBlock:sector_offset=%ld  block_offset=%ld\r\n",sector_offset, block_offset));

    ret = BML_EraseBlk(VOLUME_NUMBER, block_offset, BML_FLAG_SYNC_OP);
    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"RB_WriteBackupBlock: Failed to erase backup block (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_181_112_2_18_1_31_23_18,(uint8*)"d", ret);
        return 1;
    }
    
    ret = BML_MWrite(VOLUME_NUMBER, sector_offset, g_SctsPerBlk, pbBuffer, 
    		0, BML_FLAG_ECC_ON | BML_FLAG_SYNC_OP);

  
    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"Failed to write fota flag parition (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_190_112_2_18_1_31_24_19,(uint8*)"d", ret);
        return 1;
    }
    

    
    
    return 0;
}



long RB_WriteBlock(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned long dwBlockAddress,		/* address of the block to be updated */
	unsigned char *pbBuffer)			/* pointer to data to be written */	
{
    uint8             ret;
    STLInfo 	info;
    uint32 sector_offset = 0;
    uint32 block_offset = 0;	
    uint32 SctOffInBlk=0;	
	
    info.nSamBufFactor = 10;
    info.bASyncMode = FALSE32;
	
    UA_TRACE(("enter RB_WriteBlock\r\n"));
     
    sector_offset = dwBlockAddress/XSR_SECTOR_SIZE;
    block_offset = sector_offset/g_SctsPerBlk;
    SctOffInBlk = sector_offset%g_SctsPerBlk;
    
    UA_TRACE(("dwBlockAddress = 0x%x RB_WriteBlock:sector_offset1=%ld  block_offset=%ld\r\n",dwBlockAddress,sector_offset, block_offset));
    if(SctOffInBlk!=0)
    {
	    ret = BML_MRead(VOLUME_NUMBER, sector_offset-SctOffInBlk, SctOffInBlk, g_data_cache1,NULL, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);
	    if (BML_SUCCESS != ret) {
	        //UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_227_112_2_18_1_31_24_20,(uint8*)"d", ret);
	        return FALSE;
	    }
	    memcpy(&g_data_cache1[XSR_SECTOR_SIZE*SctOffInBlk],pbBuffer,(g_SctsPerBlk-SctOffInBlk)*XSR_SECTOR_SIZE);
	
	    ret = BML_EraseBlk(VOLUME_NUMBER, block_offset, BML_FLAG_SYNC_OP);  
	    if (BML_SUCCESS != ret) {
	        //UA_TRACE_ERR:"RB_WriteBlock: Failed to erase image block(0x%x)\r\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_234_112_2_18_1_31_24_21,(uint8*)"d", ret);
	        return 1;
	    }
	       
	    ret = BML_MWrite(VOLUME_NUMBER, sector_offset-SctOffInBlk, g_SctsPerBlk, g_data_cache1, 
	    		0, BML_FLAG_ECC_ON | BML_FLAG_SYNC_OP);
	    		
	  
	    if (BML_SUCCESS != ret) {
	        //UA_TRACE_ERR:"RB_WriteBlock: Failed to write image (0x%x)\r\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_243_112_2_18_1_31_24_22,(uint8*)"d", ret);
	        return 1;
	    }
	    ret = BML_MRead(VOLUME_NUMBER, (block_offset+1)*g_SctsPerBlk, g_SctsPerBlk, g_data_cache1, 
	    		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);
	    if (BML_SUCCESS != ret) {
	        //UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_249_112_2_18_1_31_24_23,(uint8*)"d", ret);
	        return FALSE;
	    }
	    memcpy(&g_data_cache1[0],&pbBuffer[XSR_SECTOR_SIZE*(g_SctsPerBlk-SctOffInBlk)],SctOffInBlk*XSR_SECTOR_SIZE);
	    
	    ret = BML_EraseBlk(VOLUME_NUMBER, block_offset+1, BML_FLAG_SYNC_OP);  
	    if (BML_SUCCESS != ret) {
	        //UA_TRACE_ERR:"RB_WriteBlock: Failed to erase image block(0x%x)\r\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_256_112_2_18_1_31_24_24,(uint8*)"d", ret);
	        return 1;
	    }
	    ret = BML_MWrite(VOLUME_NUMBER, (block_offset+1)*g_SctsPerBlk, g_SctsPerBlk, g_data_cache1, 
	    		0, BML_FLAG_ECC_ON | BML_FLAG_SYNC_OP);
	    		
	    if (BML_SUCCESS != ret) {
	        //UA_TRACE_ERR:"RB_WriteBlock: Failed to write image (0x%x)\r\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_263_112_2_18_1_31_24_25,(uint8*)"d", ret);
	        return 1;
	    }	    
    }
    else
    {
	    ret = BML_EraseBlk(VOLUME_NUMBER, block_offset, BML_FLAG_SYNC_OP);  
	    if (BML_SUCCESS != ret) {
	        //UA_TRACE_ERR:"RB_WriteBlock: Failed to erase image block(0x%x)\r\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_271_112_2_18_1_31_24_26,(uint8*)"d", ret);
	        return 1;
	    }
	       
	    ret = BML_MWrite(VOLUME_NUMBER, sector_offset, g_SctsPerBlk, pbBuffer, 
	    		0, BML_FLAG_ECC_ON | BML_FLAG_SYNC_OP);
	    		
	  
	    if (BML_SUCCESS != ret) {
	        //UA_TRACE_ERR:"RB_WriteBlock: Failed to write image (0x%x)\r\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_280_112_2_18_1_31_24_27,(uint8*)"d", ret);
	        return 1;
	    }
	    
	    UA_TRACE(("RB_WriteBlock: write image success\r\n"));
    
    }
    

    return 0;
}


long RB_ReadImage(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned char *pbBuffer,			/* pointer to user buffer */
    unsigned long dwStartAddress,		/* memory address to read from */
    unsigned long dwSize)				/* number of bytes to copy */	
{

	int             ret;
    uint32 sector_offset = 0;
    uint32 sector_count = 0;
    uint32 byte_offset = 0;
    
    UA_TRACE(("enter RB_ReadImage\r\n"));
    
    memset(g_buffer,0,sizeof(g_buffer));
    
  
    
    //UA_TRACE("RB_ReadImage: n1stVbn = %d, nNumOfBlks = %d, g_SctsPerBlk = %d\r\n", part_entry.n1stVbn, 
        //part_entry.nNumOfBlks, g_SctsPerBlk);
    UA_TRACE(("RB_ReadImage: dwStartAddress = 0x%x, dwSize = %d\r\n", dwStartAddress, dwSize));
   
    sector_offset = dwStartAddress/XSR_SECTOR_SIZE;
    byte_offset = dwStartAddress - sector_offset*XSR_SECTOR_SIZE;
    if(((byte_offset+dwSize)%XSR_SECTOR_SIZE)!=0)
    	sector_count = ((byte_offset+dwSize)/XSR_SECTOR_SIZE)+1;
    else
    	sector_count = ((byte_offset+dwSize)/XSR_SECTOR_SIZE);
    
    UA_TRACE(("RB_ReadImage: sector_offset=%ld, byte_offset=%ld, sector_count=%ld\r\n", sector_offset, byte_offset, sector_count));
                
    ret = ReadNandflashBysector(g_buffer, sector_offset, sector_count);
    memcpy(pbBuffer, g_buffer+byte_offset, dwSize);
    
             
    if (0 != ret) {
    	//UA_TRACE_ERR:"RB_ReadImage:copy data failed.\r\n"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_329_112_2_18_1_31_24_28,(uint8*)"");
        return 1;
    }      
    
    
    return 0;   
}

extern LLDSpec astNandSpec[];

#define SMALL_PAGE		0
#define LARGE_PAGE		1

BOOLEAN	CheckDev(int *pDev)
{
	unsigned char 	id[4];
	int				i;
	unsigned char	nMID;
	unsigned char	nDID;
	
	memset(id, 0, sizeof(id));
	
	NANDCTL_Read_ID(id);
	nMID = id[0];
	nDID = id[1];
	
	for (i = 0; astNandSpec[i].nMID != 0; i++) {
        if ((nDID == astNandSpec[i].nDID) && (nMID == astNandSpec[i].nMID))
            break;
    }
    if (0 == astNandSpec[i].nMID)
    {
       //UA_TRACE_ERR:"Error: (M,D) =(0x%x,0x%x); use default CheckDev = 13 \r\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,RB_IMAGEUPDATE_361_112_2_18_1_31_24_29,(uint8*)"dd",i,nMID,nDID);
      *pDev = 13;
    	return FALSE;
    }
    
     *pDev = i;
    UA_TRACE(("CheckDev = %d; (M,D) =(0x%x,0x%x)\r\n",i,nMID,nDID));   

    return TRUE;
}


long RB_GetBlockSize(void)/* returns the size of a memory-block */  
{


 	int			 nDev;
 	long  		 BlockSize; 
 	int          PageAttr;

	if( 	_g_BlockSize_ == 0)
	{
		

		CheckDev(&nDev);

		PageAttr = (32 == astNandSpec[nDev].nPgsPerBlk) ? SMALL_PAGE : LARGE_PAGE;
		BlockSize = astNandSpec[nDev].nSctsPerPg * astNandSpec[nDev].nPgsPerBlk * XSR_SECTOR_SIZE;
		_g_BlockSize_ = BlockSize;
	}
       UA_TRACE(("RB_GetBlockSize: block size is  0x%x\r\n",_g_BlockSize_));
    
       return  _g_BlockSize_;   	
}


void RB_Progress(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned long uPercent)			/* progress info in percents */  
{
	UA_TRACE(("enter RB_Progress %d\r\n",uPercent));
	ShowProgress(uPercent, 100);
}

//*********************************************************************************************//
#define SIO_TX_EMPTY(s)    	    ((s) & 0xFF00)
#define WAIT_FIFO_EMPTY                                          \
    {                                                            \
        volatile uint32 i;                                      \
        for(i=0;i<0x2000;i++);                                   \
        {                                                        \
           while( SIO_TX_EMPTY(*(volatile uint32*)(UART0_BASE_ADDR+0x0c)));\
        }                                                        \
    }

#define TRACE_LOG_MSG \
        va_start(args, aFormat);\
        nBuf = vsprintf(format_str, aFormat, args);\
        /* was there an error? */\
        /* Was the expanded string too long? */\
        va_end(args);\
        /* Send message to serial buffer! */ \
        SendMsgOut(format_str, nBuf + 1);

/**---------------------------------------------------------------------------*
 **             Function Definitions                                          *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : write char info 
//	Global resource dependence : 
//  Author:kelly.li
//	Note:
/*****************************************************************************/
LOCAL void WriteChar (char c)
{
	while ((((*(volatile uint32*)(UART0_BASE_ADDR+0x0c)) >> 8 )&0xFF) >= 64 ) {};
	
	*(volatile uint32*)UART0_BASE_ADDR = c;
}
  
/*****************************************************************************/
// 	Description : send trace msg out
//	Global resource dependence : 
//  Author:kelly.li
//	Note:
/*****************************************************************************/      
LOCAL void SendMsgOut(char * buf_ptr, int size)
{
  while (size --)
  {
   WriteChar (*(buf_ptr++));
  }
    WriteChar('\n');
}

//*******************************************************//
unsigned long RB_Trace(
	void *pbUserData,					/* User data passed to all porting routines */
	const char *aFormat,...)			/* format string to printf */	
{
	
	char       format_str[1024];
	
	va_list    args;
	int        nBuf;
    WAIT_FIFO_EMPTY
    memset (format_str,0,1024);
    TRACE_LOG_MSG    /*assert verified*/
    WAIT_FIFO_EMPTY
    
    return 0;
}	
	
long RB_ResetTimerA(void)
{
   return 0;
}
#ifdef	_ENABLE_TEST_
void UA_InterfaceTest(void)
{

    uint8 ret = 0;
    uint32 i = 0;
    
    
    UA_TRACE("enter RB_InterfaceTest\r\n");
    
	memset(buffer,0,sizeof(buffer));
 	ret = RB_GetDelta(
	              NULL,				    
	              buffer,			
     			  0, 
                  0x20000);
            
     if(ret == 0)	
     {	
     	for(i=0; i<16; i++)		
     	UA_TRACE("UA_InterfaceTest: buffer[%d]=0x%x\r\n",i,*(buffer+i));	
     }
             
    if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: get delta failed\r\n");
                   
     ret = RB_WriteBackupBlock(
			NULL,					
			0x11a0000,		
			buffer);	
			
	 if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: write backup block failed\r\n");	
      
         ret = RB_WriteBackupBlock(
			NULL,					
			0x11c0000,		
			buffer);	
			
	 if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: write backup block failed\r\n");	 
      
           ret = RB_WriteBackupBlock(
			NULL,					
			0x11e0000,		
			buffer);	
			
	 if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: write backup block failed\r\n");	 
      
     memset(buffer,0,sizeof(buffer));
     ret = RB_ReadBackupBlock(
			NULL,					
			buffer,			
    		0x11a0000,		
    		0x20000);
    		  
     if(ret == 0)	
     {	
     	for(i=0; i<16; i++)		
     	UA_TRACE("UA_InterfaceTest: buffer[%d]=0x%x\r\n",i,*(buffer+i));	
     }	
     
     if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: read backup block failed\r\n");	 
       
     memset(buffer,0,sizeof(buffer));
     ret = RB_ReadBackupBlock(
			NULL,					
			buffer,			
    		0x11c0000,		
    		0x20000);
    		  
     if(ret == 0)	
     {	
     	for(i=0; i<16; i++)		
     	UA_TRACE("UA_InterfaceTest: buffer[%d]=0x%x\r\n",i,*(buffer+i));	
     }	
     
     if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: read backup block failed\r\n");	
       
       memset(buffer,0,sizeof(buffer));
     ret = RB_ReadBackupBlock(
			NULL,					
			buffer,			
    		0x11e0000,		
    		0x20000);
    		  
     if(ret == 0)	
     {	
     	for(i=0; i<16; i++)		
     	UA_TRACE("UA_InterfaceTest: buffer[%d]=0x%x\r\n",i,*(buffer+i));	
     }	
     
     if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: read backup block failed\r\n");	    

  
   ret = RB_WriteBlock(
			NULL,					
			0x420000,		
			buffer);	
			
	 if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: write backup block failed\r\n");
       
       
    memset(buffer,0,sizeof(buffer));
     ret = RB_ReadBackupBlock(
			NULL,					
			buffer,			
    		0x420000,		
    		0x20000);
    		  
     if(ret == 0)	
     {	
     	for(i=0; i<16; i++)		
     	UA_TRACE("UA_InterfaceTest: buffer[%d]=0x%x\r\n",i,*(buffer+i));	
     }	
     
     if(ret!= 0)
       UA_TRACE("UA_InterfaceTest: read backup block failed\r\n");    
      		                
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 