/******************************************************************************            
 ** File Name:      ua.c                                                      *
 ** Description:	 													 	  *
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
#include "ua.h"
#include "RB_ImageUpdate.h"
#include "RbErrors.h"
#include "dal_lcd.h"
#include "log.h"
#include "XsrTypes.h"
#include "BML.h"
#include "bml.h"
#include "STL.h"
#include "Nand_partinfo.h"
#include "LLD.h"
#include "sc_reg.h"
#include "Tb_comm.h"
#include "cmddef.h"
#include "dal_lcd.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define	KERNEL_IMAGE_ID	0
#define	USER_IMAGE_ID		1
#define	FIXNV_IMAGE_ID	2


#define	ERR_UPDATE_SUCCESS		(0xFFFFFFFB)
#define	ERR_UPDATE_UNCOMPATIBLE	(0xFFFFFFF3)
#define	ERR_UPDATE_SYSTEM		(0xFFFFFFE3)
#define	ERR_UPDATE_UNKNOW		(0xFFFFFF13)
#define	VERSION_CHECK_OK		(0xFFFFF0FC)
#define	KERNEL_IMAGE_SIGNATURE     (0xE59FF018)

//chip define
#define IRQ_STACK_LIMIT     0x1000
#define FIQ_STACK_LIMIT     0x1000
#define SVC_STACK_LIMIT     0x1000
#define SVC_HEAP_LIMIT      0x1000
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
uint32 svc_heap_space [SVC_HEAP_LIMIT  >> 2];
uint32 svc_stack_space[SVC_STACK_LIMIT >> 2];
uint32 irq_stack_space[IRQ_STACK_LIMIT >> 2];
uint32 fiq_stack_space[FIQ_STACK_LIMIT >> 2];

const uint32 irq_stack_bottom = (uint32) (irq_stack_space + IRQ_STACK_LIMIT/4);
const uint32 fiq_stack_bottom = (uint32) (fiq_stack_space + FIQ_STACK_LIMIT/4);
const uint32 svc_stack_bottom = (uint32) (svc_stack_space + SVC_STACK_LIMIT/4);
const uint32 svc_heap_bottom  = (uint32) (svc_heap_space);
static  LCD_INFO_T	 g_LcdInfo;

#define DM_WHITE_COLOR		            0xffff

#define MAINSCREEN_WIDTH               ( g_LcdInfo.lcd_width-1)
#define MAINSCREEN_HEIGHT               (g_LcdInfo.lcd_height-1)


#define DM_PRG_TEXT_LEFT                (MAINSCREEN_WIDTH/4)
#define DM_PRG_TEXT_TOP                 (MAINSCREEN_HEIGHT/2+10)
#define DM_PRG_TEXT_RIGHT               (5*MAINSCREEN_WIDTH/12)
#define DM_PRG_TEXT_BOTTOM              (19*MAINSCREEN_HEIGHT/32)

#define DM_PRG_RECT_LEFT                (1*MAINSCREEN_WIDTH/8)
#define DM_PRG_RECT_TOP                 (13*MAINSCREEN_HEIGHT/32)
#define DM_PRG_RECT_RIGHT               (7*MAINSCREEN_WIDTH/8)
#define DM_PRG_RECT_BUTTPON             (15*MAINSCREEN_HEIGHT/32)

unsigned int g_SctsPerBlk;
uint8 g_mbuf[XSR_SECTOR_SIZE] = {0};
static  uint32 new_kernel_image_size = 0;
uint32	g_current_image_map[8]={0},g_file_count = 0;

uint32	update_partitions[]={
#ifdef VM_SUPPORT
    SCI_PARTITION_OSC,
#endif
    SCI_PARTITION_OSA,
    SCI_PARTITION_MMI_RESOURCE,
    SCI_PARTITION_UPDATED_NV,
    SCI_PARTITION_DSPCODE
};

                                                  
extern void remap_and_reboot(unsigned long boot_addr); /* defined in tx_deepsleep.s */
extern void SetResetFlag( void );


#define		BML_IOCTL_UNLOCK_WHOLEAREA		XSR_IOCTL_CODE(XSR_MODULE_BML,	\
													0,						\
													XSR_METHOD_IN_DIRECT,	\
													XSR_WRITE_ACCESS)

#define  RAM_START_ADDR  0x300000   
#define  RAM_START_SIZE  0xA00000    
#define	 LCD_DisplayText	LCD_DisplayText_Fota 

extern  int _g_BlockSize_;
/*****************************************************************************/
// 	Description : record the current flash map in order.
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
void add_image_map_element(uint32 address)
{
	int32 	i;
	uint32	count;

	count=sizeof(g_current_image_map)/sizeof(g_current_image_map[0]);
	if(g_file_count >=count)
		return;
	if(g_file_count == 0)
	{
		g_current_image_map[0] = address;
		g_file_count++;
		return;		
	}
	for(i=g_file_count-1;i>=0;i--)
	{
		if(address < g_current_image_map[i])
		{
			g_current_image_map[i+1] = g_current_image_map[i];
			
			if(i==0)
			{
				g_current_image_map[i] = address;
				g_file_count++;
			}
		}
		else
		{
			g_current_image_map[i+1] = address;
			g_file_count++;
			break;
		}	
	}
	
}
/*****************************************************************************/
// 	Description : Initialize UART.
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
#if 0
LOCAL void UART_Init(void)
{
    uint32 i=0;
    unsigned long ext_clk,baud_rate;
    return;
    ext_clk=*(volatile unsigned long *)0x8b000018;

    if(ext_clk &(0x1<<15))
    baud_rate=0xe2;
    else
    baud_rate=0x71;

    for(i=0;i<1000;i++);
    
    //uart init;
    *(volatile uint32 *)0x8B000018 |= (1<<22);
    *(volatile uint32 *)0x83000010 = 0x00;
    *(volatile uint32 *)0x83000018 = 0x1C;
    *(volatile uint32 *)0x8300001C = 0x00;
    *(volatile uint32 *)0x83000024 = baud_rate; //115200/52Mhz
    *(volatile uint32 *)0x83000028 = 0x00;

    //Enable sys_cnt
    *(volatile uint32 *)0x8b000018|=(BIT_13);   
}

LOCAL void UART_Init(void)
{
    uint32 i=0;
    unsigned long baud_rate;

    baud_rate=0xe2;


    for(i=0;i<1000;i++);
    
    //uart init;
    *(volatile uint32 *)0x8B000008 |= (BIT_21);
    *(volatile uint32 *)0x8B00000c |= ~(BIT_8);
    *(volatile uint32 *)0x8b00005c |=   (BIT_22|BIT_23);
    *(volatile uint32 *)0x8b000018  |=(BIT_1|BIT_2);
    *(volatile uint32 *)0x84000010 = 0x00;
    *(volatile uint32 *)0x84000024 = baud_rate; //115200/52Mhz
    *(volatile uint32 *)0x84000028 = 0x00;

    //Enable sys_cnt
    *(volatile uint32 *)0x8b000018|=(BIT_19|BIT_27);   
}
#endif
/*****************************************************************************/
//  Discription: Draw a line from (left, top) --> (right, bottom)
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DrawLine(
                         uint16     left,
                         uint16     top,
                         uint16     right,
                         uint16     bottom,
                         uint8      color)
{
    uint16  dx = right - left, dy = bottom - top;
    uint16  wc_x = 0, wc_y = 0;
    uint16  inc_x, inc_y, sum;
    uint16  i;

    uint16 main_lcd_width = 240;
    uint16 main_lcd_height = 320;
    // check input
    left    = (left >= main_lcd_width)       ?    main_lcd_width   :   left;
    right   = (right >= main_lcd_width)      ?    main_lcd_width   :   right;
    top     = (top >= main_lcd_height)       ?    main_lcd_height  :   top;
    bottom  = (bottom >= main_lcd_height)    ?    main_lcd_height  : bottom;

    if( dx & 0x8000 )
    {
        //dx < 0;
        dx      = 0x0 - dx; // dx = -dx;
        inc_x   = (uint16)(-1);
    }
    else
    {
        //inc_x = (dx) ? 1 : 0;
        inc_x = !(dx);
        inc_x++;
    }

    if( dy & 0x8000 )
    {
        //dy < 0;
        dy      = 0 - dy;
        inc_y   = (uint16)(-1);
    }
    else
    {
        //inc_y = (dy) ? 1 : 0;
        inc_y = !(dy);
        inc_y++;
    }

    sum = (dx > dy) ?  dx  : dy;

    for( i = 0; i <= sum; i++)
    {
        LCD_Set8Pixels(left, top, color);

        wc_x += dx;
        wc_y += dy;
        if( wc_x >= sum )
        {
            wc_x -= sum;
            left += inc_x;
        }
        if( wc_y >= sum )
        {
            wc_y -=sum;
            top += inc_y;
        }
    }
}			
	
/*****************************************************************************/
//  Discription:Lcd Draw progress
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void LCD_DisplayProgressBar(uint16 left, uint16 top, uint16 right, uint16 bottom, uint16 progress)
{
	uint16 i = 0;
	
	DrawLine(left,top,left,bottom,1);
	DrawLine(left,top,right,top,1);
	DrawLine(right,top,right,bottom,1);
	DrawLine(left,bottom,right,bottom,1);
	
	for(i=0; i<progress; i++)
	{
		DrawLine(left,top,left,bottom,1);
		left++;
	}
}

/*****************************************************************************/
//  Discription:Lcd Draw progress
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
void ShowProgress(uint32 current, uint32 total)
{  

    uint8 display_pro[] = "current/total";
    uint16 display_pro_len = sizeof(display_pro);


    LCD_Clear(0, DM_WHITE_COLOR);    
            
    LCD_DisplayProgressBar(DM_PRG_RECT_LEFT,DM_PRG_RECT_TOP,DM_PRG_RECT_RIGHT,DM_PRG_RECT_BUTTPON, ((DM_PRG_RECT_RIGHT - DM_PRG_RECT_LEFT)*current)/100);

    LCD_InvalidateRect(0,0,0,MAINSCREEN_WIDTH,MAINSCREEN_HEIGHT);
    
    if(current == total)
    {

        LCD_Clear(0, DM_WHITE_COLOR);	
        LCD_InvalidateRect(0,0,0,MAINSCREEN_WIDTH,MAINSCREEN_HEIGHT);
        
        SCI_Sleep(1000);
    }
}

/*****************************************************************************/
// 	Description : Set Version check result.
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
extern uint8 g_data_cache1[];
BOOLEAN SetUpdateProcessFlag(uint32 update_result)
{
    uint8             	ret;
    XSRPartEntry    part_entry;
    uint32 			flag_addr;
    uint8			i;
    uint32			user_flag;
	
    ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_DU, &part_entry);

    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_350_112_2_18_1_31_32_117,(uint8*)"d",ret);
        return FALSE;
    }
    //the last sector in PART DIFF_DATA store the flag  
    flag_addr = (part_entry.n1stVbn) * g_SctsPerBlk+(part_entry.nNumOfBlks-1)*g_SctsPerBlk;
    ret = BML_MRead(VOLUME_NUMBER, flag_addr, g_SctsPerBlk, g_data_cache1, 
    		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);
  
    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_359_112_2_18_1_31_32_118,(uint8*)"d", ret);
        return FALSE;
    }

    *(unsigned long *)&g_data_cache1[XSR_SECTOR_SIZE*g_SctsPerBlk-4] = update_result;
    RB_WriteBlock(NULL,flag_addr*XSR_SECTOR_SIZE,g_data_cache1);

    return TRUE;
}
/*****************************************************************************/
// 	Description : modify update flag
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
BOOLEAN ClearUpdateFlag(uint32 update_result)
{
    uint8             	ret;
    XSRPartEntry    part_entry;
    uint32 			flag_addr;
    uint8			i;
    uint32			user_flag;
	
    ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_DU, &part_entry);

    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_383_112_2_18_1_31_32_119,(uint8*)"d",ret);
        return FALSE;
    }
    //the last sector in PART DIFF_DATA store the flag  
    flag_addr = (part_entry.n1stVbn) * g_SctsPerBlk+(part_entry.nNumOfBlks-1)*g_SctsPerBlk;
    ret = BML_MRead(VOLUME_NUMBER, flag_addr, g_SctsPerBlk, g_data_cache1, 
    		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);
  
    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_392_112_2_18_1_31_32_120,(uint8*)"d", ret);
        return FALSE;
    }
    user_flag = *(unsigned long *)&g_data_cache1[XSR_SECTOR_SIZE*g_SctsPerBlk-8];

    for(i=0;i<part_entry.nNumOfBlks;i++)
    {
    		ret = BML_EraseBlk(VOLUME_NUMBER, part_entry.n1stVbn+i, BML_FLAG_SYNC_OP); 
    		if (BML_SUCCESS != ret) {
        		//UA_TRACE_ERR:"Failed to erase update flag parition (0x%x)\r\n"
        		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_401_112_2_18_1_31_32_121,(uint8*)"d", ret);
        		return FALSE;
    		}
    }
    *(unsigned long *)&g_data_cache1[XSR_SECTOR_SIZE*g_SctsPerBlk-8] = user_flag;
    *(unsigned long *)&g_data_cache1[XSR_SECTOR_SIZE*g_SctsPerBlk-4] = update_result;
    RB_WriteBlock(NULL,flag_addr*XSR_SECTOR_SIZE,g_data_cache1);

    return TRUE;
}
/*****************************************************************************/
// 	Description : read version check flag
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
uint32 ReadUpdateProcessFlag()
{
    uint8             	ret;
    XSRPartEntry    part_entry;
    uint32 			flag_addr;
    uint8			i;
    uint32			user_flag;
	
    ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_DU, &part_entry);

    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_426_112_2_18_1_31_32_122,(uint8*)"d",ret);
        return FALSE;
    }
    //the last sector in PART DIFF_DATA store the flag  
    flag_addr = (part_entry.n1stVbn) * g_SctsPerBlk+(part_entry.nNumOfBlks-1)*g_SctsPerBlk;
    ret = BML_MRead(VOLUME_NUMBER, flag_addr, g_SctsPerBlk, g_data_cache1, 
    		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);
  
    if (BML_SUCCESS != ret) {
        //UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_435_112_2_18_1_31_32_123,(uint8*)"d", ret);
        return FALSE;
    }
    user_flag = *(unsigned long *)&g_data_cache1[XSR_SECTOR_SIZE*g_SctsPerBlk-4];


    return user_flag;
}

/*****************************************************************************/
// 	Description : Get Image information
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
#define	record_one_image_map_info(image_partition_id) \
{																		\
	ret = BML_LoadPIEntry(VOLUME_NUMBER, image_partition_id, &part_entry); 	\
	if (BML_SUCCESS != ret) { 												\
	    //UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_452_112_2_18_1_31_32_124,(uint8*)"d",ret); \
#endif
	    return FALSE;														\
	 }																	\
	 start_address = (part_entry.n1stVbn) * g_SctsPerBlk*XSR_SECTOR_SIZE;		\
	 end_address  = start_address + (part_entry.nNumOfBlks) * g_SctsPerBlk*XSR_SECTOR_SIZE;	\
	 add_image_map_element(start_address);							\
	 if(*end_addr < end_address)											\
		*end_addr = end_address;	 										\
	 //UA_TRACE_ERR:"part[%d]:  (0x%x, 0x%x), blk_num %d \r\n"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_460_112_2_18_1_31_32_125,(uint8*)"dddd",i, start_address, end_address,part_entry.nNumOfBlks); \
#endif
}

uint8 GetImageInfo(uint32 *start_addr, uint32 *end_addr)
{
	uint8 ret = 0;
       XSRPartEntry    part_entry;
	uint32  start_address;
	uint32  end_address;
 	uint32 flag_addr;   
	uint32 kernel_offset=0;
	uint32 i;
	
	*start_addr = 0;
	*end_addr = 0;
	for(i=0;i<sizeof(update_partitions)/sizeof(update_partitions[0]);i++)
	{
		record_one_image_map_info(update_partitions[i]);
	}

	ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_OSB, &part_entry);

	if (BML_SUCCESS != ret) {
	    //UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_483_112_2_18_1_31_32_126,(uint8*)"d",ret);
	    return FALSE;
	}
	
	flag_addr = (part_entry.n1stVbn) * g_SctsPerBlk;
	
	for(i=0;i<part_entry.nNumOfBlks*g_SctsPerBlk;i++)
	{
			ret = BML_MRead(VOLUME_NUMBER, flag_addr+i, 1, g_mbuf,
				0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);     
			if (BML_SUCCESS != ret) {
	   		 	//UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
	   		 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_494_112_2_18_1_31_32_127,(uint8*)"d",ret);
	    			return FALSE;
			}
		if(*(uint32 *)g_mbuf == KERNEL_IMAGE_SIGNATURE)
		{
			UA_TRACE(("Kernel offset =  (0x%x)\r\n",kernel_offset));
			break;
		}
		kernel_offset +=XSR_SECTOR_SIZE;
	}
	
	start_address = (part_entry.n1stVbn) * g_SctsPerBlk*XSR_SECTOR_SIZE;
	end_address  = start_address + (part_entry.nNumOfBlks) * g_SctsPerBlk*XSR_SECTOR_SIZE;
	//UA_TRACE_ERR:"Kernel Image:  (0x%x, 0x%x)\r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_507_112_2_18_1_31_32_128,(uint8*)"dd",  start_address+kernel_offset, end_address);

	add_image_map_element(start_address+kernel_offset);

	if(*end_addr < end_address)
		*end_addr = end_address;
	


	UA_TRACE(("UpdatedImage:  start_addr=0x%x, end_addr=0x%x\r\n", *start_addr, *end_addr));

	return TRUE;
}
/*****************************************************************************/
// 	Description : Image Update
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
uint8 VerifySourceVersion(uint32 image_start_addr,uint32 image_end_addr)
{
    uint32 ret = 0;
    uint8 i=0;
    XSRPartEntry    part_entry;
    unsigned long addr[10];
    RB_UpdateParams * updateparams;
    updateparams->isBackgroundUpdate =0;
    updateparams->maxReaders = 0;
    
    ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_UBB, &part_entry);

    if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to load update backup buffer entry (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_537_112_2_18_1_31_32_129,(uint8*)"d",ret);
        
    }
          
    for(i=0; i<10;i++)
	{
	addr[i] = part_entry.n1stVbn*g_SctsPerBlk*XSR_SECTOR_SIZE + 0x20000*i;
	}	

    UA_TRACE(("ImageUpdate addr=0x%x\r\n",addr[0]));
	LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"                     ",sizeof("Version uncompatible  "));	     	
	LCD_DisplayText(DM_PRG_TEXT_LEFT-18,DM_PRG_TEXT_TOP,"Version Verification",sizeof("Source Version Verify"));

	ret = RB_ImageUpdate(
		               NULL,
		               image_start_addr,
		               image_end_addr,
		               (unsigned char *)RAM_START_ADDR,
		               RAM_START_SIZE,
		               NULL,
		               NULL,
		               addr,
		               0xa,
		               0x1,
		               sizeof(RB_UpdateParams),
		               updateparams
		               );

	
	UA_TRACE(("RB_ImageUpdate return value: 0x%lx \r\n", ret));
           
	return ret;		               
}
/*****************************************************************************/
// 	Description : compare old version and new version.
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
uint8 IsVersionCompatible( uint32 image_start_addr,uint32 image_end_addr)
{
	uint32	i;
	RB_SectionData sectionsArray[8];
	long ret;

	ret = RB_GetSectionsData(NULL,sizeof(update_partitions)/sizeof(update_partitions[0])+1,sectionsArray, (unsigned char *)RAM_START_ADDR,RAM_START_SIZE);
	if(ret < 0)
	{
		//UA_TRACE_ERR:"RB_GetSectionsData failed  \r\n  "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_583_112_2_18_1_31_32_130,(uint8*)"");
		return FALSE;
	}

	for(i=0;i<=sizeof(update_partitions)/sizeof(update_partitions[0]);i++)
	{
		new_kernel_image_size = sectionsArray[0].new_size;
		if((sectionsArray[i].new_offset==0) || (sectionsArray[i].old_offset==0))
		{
			//UA_TRACE_ERR:"Err1: lost file.\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_592_112_2_18_1_31_32_131,(uint8*)"");
			return FALSE;
		}
		if(sectionsArray[i].old_offset!=g_current_image_map[i])
		{
			//UA_TRACE_ERR:"Err2: map[%d] : 0x%08x - 0x%08x\r\n "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_597_112_2_18_1_31_32_132,(uint8*)"ddd",i,sectionsArray[i].old_offset,g_current_image_map[i]);
			return FALSE;
		}
		if(sectionsArray[i].new_offset!=sectionsArray[i].old_offset)
		{
			//UA_TRACE_ERR:"Err3: map[%d] : 0x%08x - 0x%08x \r\n  "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_602_112_2_18_1_31_32_133,(uint8*)"ddd",i,sectionsArray[i].new_offset,sectionsArray[i].old_offset);

			return FALSE;
		}
	}

	if(ReadUpdateProcessFlag() == VERSION_CHECK_OK)
	{
		return TRUE;
	}
	if(VerifySourceVersion(image_start_addr,image_end_addr)==S_RB_SUCCESS)
	{	
		SetUpdateProcessFlag(VERSION_CHECK_OK);
		return TRUE;
	}
	return FALSE;
}


/*****************************************************************************/
// 	Description : Image Update
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
uint8 ImageUpdate(uint32 image_start_addr,uint32 image_end_addr)
{
    uint32 ret = 0;
    uint8 i=0;
    XSRPartEntry    part_entry;
    unsigned long addr[10];
    RB_UpdateParams * updateparams;
    updateparams->isBackgroundUpdate =0;
    updateparams->maxReaders = 0;
    
    ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_UBB, &part_entry);

    if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to load update backup buffer entry (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_638_112_2_18_1_31_32_134,(uint8*)"d",ret);
        
    }
          
    for(i=0; i<10;i++)
	{
	addr[i] = part_entry.n1stVbn*g_SctsPerBlk*XSR_SECTOR_SIZE + 0x20000*i;
	}	

    UA_TRACE(("ImageUpdate addr=0x%x\r\n",addr[0]));

	ret = RB_ImageUpdate(
		               NULL,
		               image_start_addr,
		               image_end_addr,
		               (unsigned char *)RAM_START_ADDR,
		               RAM_START_SIZE,
		               NULL,
		               NULL,
		               addr,
		               0xa,
		               0x0,
		               sizeof(RB_UpdateParams),
		               updateparams
		               );

	
	UA_TRACE(("RB_ImageUpdate return value: 0x%lx \r\n", ret));
           
	return ret;		               
}


/*****************************************************************************/
// 	Description : update agent main function
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
int  main(void)
{
	unsigned int nBytesReturned;
    uint8 ret =0;
    BMLVolSpec 	VolSpec;


	//SCI_TRACE_LOW:"enter fota process..............\r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_682_112_2_18_1_31_32_135,(uint8*)"");
	GPIO_PROD_InitCfgTable();
#ifdef PLATFORM_SC8800G
	ANA_Init();
#endif    
#ifdef PRODUCT_DRIVER_MTV_ENABLE
       GPIO_MTVSoftSwitchON(TRUE);
#endif

	LCD_Init();	
#ifdef PRODUCT_DRIVER_MTV_ENABLE
	DTL_CloseLdo();
#endif

       GPIO_SetLcdBackLight(TRUE);
	LCD_Clear(0, DM_WHITE_COLOR);
	LCD_GetInfo(0,&g_LcdInfo);

	//SCI_TRACE_LOW:"hight %d width %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_700_112_2_18_1_31_32_136,(uint8*)"dd",MAINSCREEN_HEIGHT,MAINSCREEN_WIDTH);

    LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"Prepare Update",sizeof("Prepare Update"));

    NANDCTL_Init(96000000);
    do
    {
	    uint32 new_kernel_length;
	    uint8   *new_address;
	    uint32 image_start_addr,image_end_addr;
			
	    if (BML_SUCCESS != (ret = BML_Init())) 
	    {
			//UA_TRACE_ERR:"BML_Init failed(0x%x).\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_713_112_2_18_1_31_32_137,(uint8*)"d", ret);
	    		break;
	    }
	    
	        
	    if (BML_SUCCESS != (ret = BML_Open(VOLUME_NUMBER))) 
	    {
			//UA_TRACE_ERR:"BML_Open failed(0x%x).\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_720_112_2_18_1_31_32_138,(uint8*)"d", ret);
	    		break;
	    }	   

	    if (BML_SUCCESS !=(ret = BML_IOCtl(VOLUME_NUMBER, BML_IOCTL_UNLOCK_WHOLEAREA,\
			NULL, 0, NULL,	0, &nBytesReturned))) 
	    {
			//UA_TRACE_ERR:"BML_Unlock failed(0x%x).\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_727_112_2_18_1_31_32_139,(uint8*)"d", ret);
			break;
	    }

	    ret = BML_GetVolInfo(VOLUME_NUMBER, &VolSpec);
	    if (BML_SUCCESS != ret) 
	    {
		   	//UA_TRACE_ERR:"BML_GetVolInfo information failed(0x%x).\r\n"
		   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_734_112_2_18_1_31_32_140,(uint8*)"d", ret);
			break;
	    }
           UA_TRACE(("BML_GetVolInfo information VolSpec.nPgsPerBlk=%d, VolSpec.nSctsPerPg=%d.\r\n", VolSpec.nPgsPerBlk, VolSpec.nSctsPerPg));
	    g_SctsPerBlk = VolSpec.nPgsPerBlk * VolSpec.nSctsPerPg;

	    Load_FixnvEntry();

	    UA_TRACE(("enter image update process\r\n"));	
	    	
	    SCI_Sleep(1000);
	    ret = GetImageInfo(&image_start_addr, &image_end_addr);
	    if(0 == ret)
	    {
			//UA_TRACE_ERR:"Failed to get image start address and end address\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_748_112_2_18_1_31_32_141,(uint8*)"");
			ClearUpdateFlag(ERR_UPDATE_SYSTEM);
		     break;
	    }		 
 	    if(!IsVersionCompatible(image_start_addr,image_end_addr))
	    {
			LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"              ",sizeof("Image Updating"));
			LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Version uncompatible",sizeof("Version uncompatible"));
			ClearUpdateFlag(ERR_UPDATE_UNCOMPATIBLE);
			SCI_Sleep(1000);
			break;
	    }
				
           LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"Image Updating",sizeof("Image Updating"));
	
            // enter update process
	     ret = ImageUpdate(image_start_addr,image_end_addr);
	     UA_TRACE(("image update complete\r\n"));
	     if(S_RB_SUCCESS != ret)
	     {
				LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"              ",sizeof("Image Updating"));	     	
			  LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Version uncompatible",sizeof("Version uncompatible"));
				ret = ClearUpdateFlag(ERR_UPDATE_UNKNOW);	
			  SCI_Sleep(1000);
			  break;
	     } 
			
		new_address =(uint8   *) MakeMasterImageHeader(new_kernel_image_size,&new_kernel_length,0);
		Update_Kernel_Head(new_address,new_kernel_length);

		update_fixnv_image();
		//MS00216980 cheney
		#ifndef CMCC_TEST_FLAG
		erase_running_nv_partition();	
		#endif

		//modify update flag 	

		LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"                     ",sizeof("Version uncompatible  "));	     	
		LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Image Update Success",sizeof("Image Update Success"));
		ret = ClearUpdateFlag(ERR_UPDATE_SUCCESS);		
		SCI_Sleep(2000);


		if(TRUE != ret)
		{
			//UA_TRACE_ERR:"change update flag failed(0x%x)\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_794_112_2_18_1_31_33_142,(uint8*)"d",ret);
			break;
		}
		
		// Close BML
		if (BML_SUCCESS != (ret = BML_Close(VOLUME_NUMBER)))
		{
			//UA_TRACE_ERR:"BML_Close failed(0x%x).\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_801_112_2_18_1_31_33_143,(uint8*)"d", ret);
	    	break;
	    }
	    	
    }while(0);	
    SetResetFlag();
	
	//remap to rom code and restart system	
	remap_and_reboot(0);    
	return 0;
	         
}



/****************************************************************************** 
 * copy_img
 *
 * Parameters:
 *		dst  - Destition address.
 *      src  - First sector, measured from the start of OS partition.
 *		size - Size of OS image in SECTORS.
 *
 * Return values:
 *		0 - The OS image is copied to SDRAM successfully.
 *		1 - NAND flash error.
 ******************************************************************************/
PUBLIC int ReadNandflashBysector(unsigned char * dst, unsigned int src, unsigned int size)
{
	int ret;

	if (BML_SUCCESS != (ret = BML_MRead(VOLUME_NUMBER, src, size, dst, 0,
			BML_FLAG_ECC_ON | BML_FLAG_BBM_ON))) {
		//UA_TRACE_ERR:"Failed to read nand flash by sector (0x%x).\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UA_833_112_2_18_1_31_33_144,(uint8*)"d", ret);
		return 1;
	}
	
	return 0;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
