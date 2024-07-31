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


#include "sprdftl_ua.h"


#include "Nand_partitionID.h"
#include "nand_spec.h"
#include "sci_ftl_api.h"


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


extern uint32 g_SectorsPerBlk_Ua;
extern uint32 g_Sector_Size;

extern  uint32 NANDCTL_Read_ID(uint8 *pID);
extern void ShowProgress(uint32 current, uint32 total);

__align(8)  unsigned char g_buffer[1*1024*1024] = {0};


#ifdef _ENABLE_TEST_
uint8 buffer[0x20000];
#endif




uint8 *RB_GetBuffer(void)
{
	return g_buffer;
}

long RB_GetDelta(
	void *pbUserData,				    /* User data passed to all porting routines */
	unsigned char *pbBuffer,			/* pointer to user buffer */
    unsigned long dwStartAddressOffset, /* offset from start of delta file */
    unsigned long dwSize)               /* buffer size in bytes */
{


	uint32 ret = 0;
    uint32 sector_offset = 0;
    uint32 sector_count = 0;
    uint32 byte_offset = 0;
    uint32 nScts_Du_Partition = 0;

    UAPartEntry_T       *pUaEntry = NULL;

    UA_TRACE(("enter RB_GetDelta\r\n"));

    if (NULL == pbBuffer)
    {
        //UA_TRACE_ERR:"RB_GetDelta: Input parameter pbBuffer == NULL\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_89_112_2_18_1_31_27_52,(uint8*)"");
        return E_RB_FAILURE;
    }

    if (0 == dwSize)
    {
        return 0;
    }

    memset(g_buffer,0,sizeof(g_buffer));
    
    /* Open DU partition */
    ret = GetUAPartIEntry(SCI_PARTITION_DU, &pUaEntry);
    if ((!ret) || (NULL == pUaEntry))
    {
        //UA_TRACE_ERR:"RB_GetDelta: Failed to load Delta partition entry (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_104_112_2_18_1_31_27_53,(uint8*)"d", ret);
        return E_RB_FAILURE;
    }

    UA_TRACE(("RB_GetDelta: n1stVbn = %d, nNumOfBlks = %d, g_SectorsPerBlk_Ua = %d\r\n", 
      pUaEntry->n1stVbn, pUaEntry->nNumOfBlks, g_SectorsPerBlk_Ua));
    UA_TRACE(("RB_GetDelta: dwStartAddressOffset = %d, dwSize = %d\r\n", dwStartAddressOffset, 
                dwSize));

    /* Get read sector offset and secotr count  */
    sector_offset = dwStartAddressOffset / g_Sector_Size;
    byte_offset = dwStartAddressOffset - (sector_offset * g_Sector_Size);
    sector_count = (byte_offset + dwSize + g_Sector_Size -1) / g_Sector_Size ;

    UA_TRACE(("RB_GetDelta: sector_offset=%ld, byte_offset=%ld, sector_count=%ld\r\n", sector_offset, byte_offset, sector_count));

    nScts_Du_Partition = pUaEntry->nNumOfBlks * g_SectorsPerBlk_Ua;
    if (sector_offset > nScts_Du_Partition)
    {
        //UA_TRACE_ERR:"RB_GetDelta:Input dwStartAddressOffset error\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_123_112_2_18_1_31_27_54,(uint8*)"");
        return E_RB_FAILURE;
    }

    if ((sector_offset + sector_count) > nScts_Du_Partition)
    {
        //UA_TRACE_ERR:"RB_GetDelta:Input dwSize error\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_129_112_2_18_1_31_27_55,(uint8*)"");
        return E_RB_FAILURE;
    }

    if (sector_count * g_Sector_Size> sizeof(g_buffer))
    {
        //UA_TRACE_ERR:"RB_GetDelta: g_buffer size error\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_135_112_2_18_1_31_27_56,(uint8*)"");
        return 1;
    }

    /* Read delta data from DU partition */
    ret = SCI_FTL_Read(pUaEntry->ftlHandle, sector_offset, sector_count, g_buffer);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"RB_GetDelta:Failed to read SCI_PARTITION_DU parition (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_143_112_2_18_1_31_27_57,(uint8*)"d", ret);
        return E_RB_FAILURE;
    }

    /* Copy delta data to user buffer */
    memcpy((void *)pbBuffer, g_buffer+byte_offset, dwSize);
                
    /* 0 : success */
    return S_RB_SUCCESS;   
    

}

long RB_ReadBackupBlock(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned char *pbBuffer,			/* pointer to user buffer in RAM where the data will be copied */
	unsigned long dwBlockAddress,		/* address of data to read into RAM. Must be inside one of the backup buffer blocks */
	unsigned long dwSize)				/* buffer size in bytes */
{

	int    ret = 0;
    uint32 sector_offset = 0;
    uint32 sector_count = 0;
    uint32 byte_offset = 0;
    uint32 nScts_Du_Partition = 0;


    UA_TRACE(("enter RB_ReadBackupBlock \r\n"));

    if (NULL == pbBuffer)
    {
        //UA_TRACE_ERR:"RB_ReadBackupBlock: Input parameter pbBuffer == NULL\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_174_112_2_18_1_31_27_58,(uint8*)"");
        return E_RB_FAILURE;
    }

    if (0 == dwSize)
    {
        return S_RB_SUCCESS;
    }

    memset(g_buffer,0,sizeof(g_buffer));

    /* Get read sector offset and secotr count  */
    sector_offset = dwBlockAddress / g_Sector_Size;
    byte_offset = dwBlockAddress - (sector_offset * g_Sector_Size);
    sector_count = (byte_offset + dwSize + g_Sector_Size -1) / g_Sector_Size ;

    UA_TRACE(("RB_ReadBackupBlock: sector_offset=%ld, byte_offset=%ld, sector_count=%ld\r\n", sector_offset, byte_offset, sector_count));

    if (sector_count * g_Sector_Size > sizeof(g_buffer))
    {
        //UA_TRACE_ERR:"RB_ReadBackupBlock: g_buffer size error\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_194_112_2_18_1_31_27_59,(uint8*)"");
        return E_RB_FAILURE;
    }
    
    ret = ReadNandflashBysector(g_buffer, sector_offset, sector_count);

    if (0 != ret) 
    {
    	//UA_TRACE_ERR:"RB_ReadBackupBlock:read data failed (0x%x).\r\n"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_202_112_2_18_1_31_27_60,(uint8*)"d",ret);
        return E_RB_FAILURE;
    }
    
    memcpy((void *)pbBuffer, (void *)(g_buffer+byte_offset), dwSize);
             
    
    
    return S_RB_SUCCESS;   


}


long RB_WriteBackupBlock(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned long dwBlockStartAddress,	/* address of the block to be updated */
	unsigned char *pbBuffer)  	        /* RAM to copy data from */		
{

    int ret = 0;
    uint32 nWBlk_Idx = 0;
    uint32 nWBlk_offset = 0;

    
	UA_TRACE(("enter RB_WriteBackupBlock\r\n"));

    if (NULL == pbBuffer)
    {
        //UA_TRACE_ERR:"RB_WriteBackupBlock: Input pbBuffer NULL \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_231_112_2_18_1_31_27_61,(uint8*)"");
        return E_RB_FAILURE;
    }
	
    nWBlk_offset = dwBlockStartAddress % (g_SectorsPerBlk_Ua * g_Sector_Size);
    if (nWBlk_offset)
    {
        //UA_TRACE_ERR:"RB_WriteBackupBlock: Input address not align to BLOCK (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_238_112_2_18_1_31_27_62,(uint8*)"d", dwBlockStartAddress);
        return E_RB_FAILURE;
    }

    nWBlk_Idx = dwBlockStartAddress / (g_SectorsPerBlk_Ua * g_Sector_Size);
    ret = WriteNandflashByBlk((void *) pbBuffer, nWBlk_Idx, 1);
    if (0 != ret) 
    {
        //UA_TRACE_ERR:"RB_WriteBackupBlock: Failed to write backup block (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_246_112_2_18_1_31_27_63,(uint8*)"d", ret);
        return E_RB_FAILURE;
    }

    return S_RB_SUCCESS;



}



long RB_WriteBlock(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned long dwBlockAddress,		/* address of the block to be updated */
	unsigned char *pbBuffer)			/* pointer to data to be written */	
{


    int ret = 0;
    uint32 nWBlk_Idx = 0;
    uint32 nWBlk_offset = 0;

    
	UA_TRACE(("enter RB_WriteBlock\r\n"));

    if (NULL == pbBuffer)
    {
        //UA_TRACE_ERR:"RB_WriteBlock: Input pbBuffer NULL \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_274_112_2_18_1_31_27_64,(uint8*)"");
        return E_RB_FAILURE;
    }
	
    nWBlk_offset = dwBlockAddress % (g_SectorsPerBlk_Ua * g_Sector_Size);
    if (nWBlk_offset)
    {
        //UA_TRACE_ERR:"RB_WriteBlock: Input address not align to BLOCK (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_281_112_2_18_1_31_27_65,(uint8*)"d", dwBlockAddress);
        return E_RB_FAILURE;
    }

    nWBlk_Idx = dwBlockAddress / (g_SectorsPerBlk_Ua * g_Sector_Size);
    ret = WriteNandflashByBlk((void *) pbBuffer, nWBlk_Idx, 1);
    if (0 != ret) 
    {
        //UA_TRACE_ERR:"RB_WriteBlock: Failed to write backup block (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_289_112_2_18_1_31_27_66,(uint8*)"d", ret);
        return E_RB_FAILURE;
    }

    return S_RB_SUCCESS;


    }



long RB_ReadImage(
	void *pbUserData,					/* User data passed to all porting routines */
	unsigned char *pbBuffer,			/* pointer to user buffer */
    unsigned long dwStartAddress,		/* memory address to read from */
    unsigned long dwSize)				/* number of bytes to copy */	
{

	uint32 ret = 0;
    uint32 sector_offset = 0;
    uint32 sector_count = 0;
    uint32 byte_offset = 0;




    UA_TRACE(("enter RB_ReadImage.\r\n"));

    if (NULL == pbBuffer)
    {
        //UA_TRACE_ERR:"RB_ReadImage: Input parameter pbBuffer == NULL\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_319_112_2_18_1_31_27_67,(uint8*)"");
        return E_RB_FAILURE;
    }

    if (0 == dwSize)
    {
        return S_RB_SUCCESS;
    }

    memset(g_buffer,0,sizeof(g_buffer));
    

    UA_TRACE(("RB_ReadImage: dwStartAddress = 0x%x, dwSize = %d\r\n", dwStartAddress, dwSize));


    /* Get read sector offset and secotr count  */
    sector_offset = dwStartAddress / g_Sector_Size;
    byte_offset = dwStartAddress - (sector_offset * g_Sector_Size);
    sector_count = (byte_offset + dwSize + g_Sector_Size -1) / g_Sector_Size ;

    UA_TRACE(("RB_ReadImage: sector_offset=%ld, byte_offset=%ld, sector_count=%ld\r\n", sector_offset, byte_offset, sector_count));
                
    ret = ReadNandflashBysector(g_buffer, sector_offset, sector_count);
             
    if (0 != ret) {
    	//UA_TRACE_ERR:"RB_ReadImage:copy data failed.\r\n"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_RB_IMAGEUPDATE_344_112_2_18_1_31_27_68,(uint8*)"");
        return E_RB_FAILURE;
    }      
    
    
    memcpy((void *)pbBuffer, (void *)(g_buffer+byte_offset), dwSize);

    return S_RB_SUCCESS;  

}


long RB_GetBlockSize(void)/* returns the size of a memory-block */  
{


    _g_BlockSize_ = g_SectorsPerBlk_Ua * g_Sector_Size;
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
