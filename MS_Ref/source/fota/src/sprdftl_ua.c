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
 ** 12/15/2010      rick.zhang       Modify through FTL API access nand dev   *
 ******************************************************************************/		
 
 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "ms_ref_fota_trc.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "sci_types.h"
#include "sprdftl_ua.h"
#include "RB_ImageUpdate.h"
#include "RbErrors.h"
#include "dal_lcd.h"
#include "log.h"


#include "sc_reg.h"
#include "Tb_comm.h"
#include "cmddef.h"
#include "dal_lcd.h"



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



/* Last bit 0b'1, not enter fota if re-poweron */
#define	ERR_UPDATE_SUCCESS		(0xFFFFFFFB)
#define	ERR_UPDATE_UNCOMPATIBLE	(0xFFFFFFF3)

/* Last bit 0b'0, enter fota if re-poweron */
#define	ERR_UPDATE_SYSTEM		(0xFFFFFFE2)
#define	ERR_UPDATE_UNKNOW		(0xFFFFFF12)
#define	VERSION_CHECK_OK		(0xFFFFF0FC)


#define	KERNEL_IMAGE_SIGNATURE     (0xE59FF018)
#define KERNEL_IMAGE_SIGNATURE_LE   (0x18F09FE5)

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


#define DM_PRG_TEXT_LEFT                (60*MAINSCREEN_WIDTH/239)
#define DM_PRG_TEXT_TOP                 (170*MAINSCREEN_HEIGHT/319)
#define DM_PRG_TEXT_RIGHT              (100*MAINSCREEN_WIDTH/239)
#define DM_PRG_TEXT_BOTTOM           (190*MAINSCREEN_HEIGHT/319)

#define DM_PRG_RECT_LEFT                (30*MAINSCREEN_WIDTH/239)
#define DM_PRG_RECT_TOP                 (130*MAINSCREEN_HEIGHT/319)
#define DM_PRG_RECT_RIGHT              (210*MAINSCREEN_WIDTH/239)
#define DM_PRG_RECT_BUTTPON          (150*MAINSCREEN_HEIGHT/319)

#if defined(PLATFORM_SC8800H)
#define  RAM_START_ADDR  0x300000   
#define  RAM_START_SIZE  0xA00000    
#endif

#if defined(PLATFORM_SC8800G)
#define  RAM_START_ADDR  0x300000   
#define  RAM_START_SIZE  0xA00000    
#endif


#define	 LCD_DisplayText	LCD_DisplayText_Fota 

/* Define RB_ImageUpdate operation */

/* Verify that device is updateable and then apply update progress */
#define FOTA_VERIFY_UPDATE_IMAGE        0x0

/* Verify that device is updateable, by calculating version signature in the 
   device and the old (source) version in the delta file.
*/
#define FOTA_VERIFY_SOURCE_ONLY         0x1

/* Verify the update operation will succeed */
#define FOTA_VERIFY_TARGET_ONLY         0x2

/* Perform update without verify */
#define FOTA_UPDATE_ONLY                 0x3

/* Revert update operation, but must include /ReverseDelta=1 when create delta */
#define FOTA_REVERT_UPDATE              0x4



typedef enum
{
    ERR_FOTA_NONE = 0,
    ERR_FOTA_READ_VERIFY_OK,
    ERR_FOTA_SYSTEM,
    ERR_FOTA_LOST,
    ERR_FOTA_SOURCE_NOT_COMP,
    ERR_FOTA_FILE_NOT_COMP,
    ERR_FOTA_VERIFY_CHECK_FAILURE,
    ERR_FOTA_MAX
} FOTA_FOTA_VERIFY_ERR_E;



#define nFtlDev                 0       // Only support 1 nand device now


#define UA_MBUF_SIZE            128*1024
#define MAX_UBB_BLK_NUM         60



uint32 g_SectorsPerBlk_Ua;
uint32 g_Sector_Size;



UAPartI_T 	        g_ua_PartI;         /* Partition information */
SCI_FTL_DEV_INFO	g_FtlDevInfo;       /* Nand device information */


__align(8) uint8 g_ua_mbuf[UA_MBUF_SIZE] = {0};
uint32 g_ua_mbuf_size = UA_MBUF_SIZE;



uint32 g_Array_UBB_Addr[MAX_UBB_BLK_NUM];



static  uint32 new_kernel_image_size = 0;
uint32	g_current_image_map[8]={0},g_file_count = 0;

uint32	update_partitions[]={
    //SCI_PARTITION_OSB,  // Special partition, OS info stored in first 16KB of the partition 
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


SCI_FTL_HANDLE GetFtlHanlde(uint32 nPartID)
{
    SCI_FTL_HANDLE FtlHandle = 0;
    uint32 partition_Idx = 0;
    uint32 ID = 0;
    
    for (partition_Idx = 0; partition_Idx < g_ua_PartI.nNumOfPartEntry; 
                partition_Idx++)
    {
        ID = g_ua_PartI.stPEntry[partition_Idx].nID;
        if (nPartID == ID)
        {
            FtlHandle = g_ua_PartI.stPEntry[partition_Idx].ftlHandle;
            break;
        }
    }

    return FtlHandle;
    
}

uint32  GetUAPartIEntry(uint32 nPartID, UAPartEntry_T **ppEntry)
{

    uint32 partition_Idx = 0;
    uint32 ID = 0;

    if (NULL == ppEntry)
    {
        return FALSE;
    }

    *ppEntry = NULL;
    
    for (partition_Idx = 0; partition_Idx < g_ua_PartI.nNumOfPartEntry; 
                partition_Idx++)
    {
        ID = g_ua_PartI.stPEntry[partition_Idx].nID;
        if (nPartID == ID)
        {
            *ppEntry = &g_ua_PartI.stPEntry[partition_Idx];
            return TRUE;
        }
    }


    return FALSE;
    
}





/*****************************************************************************/
// 	Description : Set Version check result.
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/

BOOLEAN SetUpdateProcessFlag(uint32 update_result)
{



    uint32         ret = TRUE;
    uint32         flag_sct_inx = 0;
    SCI_FTL_HANDLE FtlHandle = 0;
    uint32          read_size = 0;
    SCI_FTL_PARTITION_INFO partInfo;

    uint32          erase_sector_num = 0;


    FtlHandle = GetFtlHanlde(SCI_PARTITION_DU);
    if (0 == FtlHandle)    
    {
        //UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_426_112_2_18_1_31_29_69,(uint8*)"d");
        return FALSE;
    }

    /* Get partition info */
    ret = SCI_FTL_GetPartInfo(FtlHandle, &partInfo);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"SetUpdateProcessFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_435_112_2_18_1_31_29_70,(uint8*)"dd", SCI_PARTITION_DU, ret);
        return FALSE;
    }

    memset(g_ua_mbuf, 0xFF, sizeof(g_ua_mbuf));
    
    /* Get the last block in PART DIFF_DATA store the flag */
    flag_sct_inx = partInfo.sctTotalNum - g_SectorsPerBlk_Ua;
    read_size = partInfo.sctSize * g_SectorsPerBlk_Ua;
    ret = SCI_FTL_Read(FtlHandle, flag_sct_inx, g_SectorsPerBlk_Ua, g_ua_mbuf);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"Failed to read SCI_PARTITION_DU parition (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_447_112_2_18_1_31_29_71,(uint8*)"d", ret);
        return FALSE;
    }

    /* Set update flag */
    *(unsigned long *)&g_ua_mbuf[read_size-4] = update_result;

    /* Erase the block */
    erase_sector_num = g_SectorsPerBlk_Ua;      // read one block

    /* erase sector must align to erase_unit */
    if(partInfo.eraseUnit)
    {
        erase_sector_num = ((erase_sector_num + partInfo.eraseUnit - 1) / partInfo.eraseUnit) * partInfo.eraseUnit;
    }

    
    ret = SCI_FTL_Erase(FtlHandle, flag_sct_inx, erase_sector_num);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"SetUpdateProcessFlag: SCI_FTL_Erase SCI_PARTITION_DU parition err (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_467_112_2_18_1_31_29_72,(uint8*)"d", ret);
        return FALSE;
    }

    /* Wriete block */
    ret = SCI_FTL_Write(FtlHandle, flag_sct_inx, g_SectorsPerBlk_Ua, g_ua_mbuf);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"SetUpdateProcessFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_475_112_2_18_1_31_29_73,(uint8*)"d", ret);
        return FALSE;
    }

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
    uint32         ret = TRUE;
    uint32         flag_sct_inx = 0;
    SCI_FTL_HANDLE FtlHandle = 0;
    uint32         read_size = 0;
    SCI_FTL_PARTITION_INFO partInfo;
    uint32         blkNum = 0;

    uint32			user_flag;
    uint32          write_idx = 0;


    FtlHandle = GetFtlHanlde(SCI_PARTITION_DU);
    if (0 == FtlHandle)   
    {
        //UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_502_112_2_18_1_31_29_74,(uint8*)"d");
        return FALSE;
    }

    /* Get partition info */
    ret = SCI_FTL_GetPartInfo(FtlHandle, &partInfo);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"ClearUpdateFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_511_112_2_18_1_31_29_75,(uint8*)"dd", SCI_PARTITION_DU, ret);
        return FALSE;
    }

    memset(g_ua_mbuf, 0xFF, sizeof(g_ua_mbuf));
    
    /* Get the last block in PART DIFF_DATA store the flag */
    flag_sct_inx = partInfo.sctTotalNum - g_SectorsPerBlk_Ua;
    read_size = partInfo.sctSize * g_SectorsPerBlk_Ua;
    ret = SCI_FTL_Read(FtlHandle, flag_sct_inx, g_SectorsPerBlk_Ua, g_ua_mbuf);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"Failed to read SCI_PARTITION_DU parition (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_523_112_2_18_1_31_29_76,(uint8*)"d", ret);
        return FALSE;
    }
    
    /* save user_flag */
    user_flag = *(unsigned long *)&g_ua_mbuf[read_size-8];


    /* Erase the whole partition */
    ret = SCI_FTL_Erase(FtlHandle, 0, partInfo.sctTotalNum);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"ClearUpdateFlag: SCI_FTL_Erase SCI_PARTITION_DU parition err (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_535_112_2_18_1_31_29_77,(uint8*)"d", ret);
        return FALSE;
    }

    /* write 0xFF into redundance area according to jason's VBM/VSM require */
    memset((void *)g_ua_mbuf, 0xFF, g_ua_mbuf_size);
    write_idx = 0;
    do
    {
        ret = SCI_FTL_Write(FtlHandle, write_idx, g_SectorsPerBlk_Ua, g_ua_mbuf);
        if (SCI_FTL_ERROR_NONE != ret)
        {
            //UA_TRACE_ERR:"ClearUpdateFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_547_112_2_18_1_31_29_78,(uint8*)"d", ret);
            return FALSE;
        }
        
        /* write next block */
        write_idx += g_SectorsPerBlk_Ua;
    }while(write_idx < flag_sct_inx);

    
    /* save update flag */
    *(unsigned long *)&g_ua_mbuf[read_size-8] = user_flag;
    *(unsigned long *)&g_ua_mbuf[read_size-4] = update_result;


    /* Wriete block */
    ret = SCI_FTL_Write(FtlHandle, flag_sct_inx, g_SectorsPerBlk_Ua, g_ua_mbuf);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"ClearUpdateFlag: SCI_FTL_Write SCI_PARTITION_DU parition err (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_565_112_2_18_1_31_29_79,(uint8*)"d", ret);
        return FALSE;
    }

    return TRUE;
}
/*****************************************************************************/
// 	Description : read version check flag
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
uint32 ReadUpdateProcessFlag(void)
{

    uint32         ret = TRUE;
    uint32         flag_sct_inx = 0;
    SCI_FTL_HANDLE FtlHandle = 0;
    uint32          read_size = 0;
    SCI_FTL_PARTITION_INFO partInfo;
    uint32          update_flag = 0;

    FtlHandle = GetFtlHanlde(SCI_PARTITION_DU);
    if (0 == FtlHandle)   
    {
        //UA_TRACE_ERR:"Failed to load update flag entry (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_588_112_2_18_1_31_29_80,(uint8*)"d");
        return FALSE;
    }

    /* Get partition info */
    ret = SCI_FTL_GetPartInfo(FtlHandle, &partInfo);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"ReadUpdateProcessFlag: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_597_112_2_18_1_31_29_81,(uint8*)"dd", SCI_PARTITION_DU, ret);
        return FALSE;
    }


    memset(g_ua_mbuf, 0xFF, sizeof(g_ua_mbuf));
    
    /* Get the last block in PART DIFF_DATA store the flag */
    flag_sct_inx = partInfo.sctTotalNum - g_SectorsPerBlk_Ua;
    read_size = partInfo.sctSize * g_SectorsPerBlk_Ua;
    ret = SCI_FTL_Read(FtlHandle, flag_sct_inx, g_SectorsPerBlk_Ua, g_ua_mbuf);
    if (SCI_FTL_ERROR_NONE != ret)
    {
        //UA_TRACE_ERR:"Failed to read SCI_PARTITION_DU parition (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_610_112_2_18_1_31_29_82,(uint8*)"d", ret);
        return FALSE;
    }


    /* Get update_flag from the last 4 bytes in the block */
    update_flag = *(unsigned long *)&g_ua_mbuf[read_size-4];


    return update_flag;

    
}



/*****************************************************************************/
// 	Description : Get Image information
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
uint32 _get_one_image_map_ionfo(uint32 nPartitionID, 
            uint32 *start_addr, uint32 *end_addr)
{

    uint32              ret = TRUE;

    UAPartEntry_T       *pUaEntry = NULL;

    if ( (NULL == start_addr) || (NULL == end_addr))
    {
        //UA_TRACE_ERR:"Input pointer should not be NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_640_112_2_18_1_31_29_83,(uint8*)"");
        return FALSE;
    }

    /* Open partition handle, no need close it */
    ret = GetUAPartIEntry(nPartitionID, &pUaEntry);
    if ((NULL == pUaEntry) || (!ret))
    {
        //UA_TRACE_ERR:"Failed to open PARTITION(0x%x): Error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_648_112_2_18_1_31_29_84,(uint8*)"dd",nPartitionID, ret);
        
        return FALSE;
    }

    *start_addr = pUaEntry->n1stVbn * g_SectorsPerBlk_Ua * g_Sector_Size;
    *end_addr   = *start_addr + pUaEntry->nNumOfBlks * g_SectorsPerBlk_Ua * g_Sector_Size;
    UA_TRACE(("userImage start_addr=0x%x, end_addr=0x%x  blk_num %d \r\n", *start_addr, *end_addr, pUaEntry->nNumOfBlks));

   
    return TRUE;


}




uint8 GetImageInfo(uint32 *start_addr, uint32 *end_addr)
{

    uint32 ret = 0;
    UAPartEntry_T       *pUaEntry = NULL;
	uint32  start_address;
	uint32  end_address;
 	uint32 end_Sct;   
	uint32 kernel_offset=0;
	uint32 i;
	uint32 flag;

    if ( (NULL == start_addr) || (NULL == end_addr) )
    {
	    //UA_TRACE_ERR:"GetImageInfo() input parameter error\r\n"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_680_112_2_18_1_31_29_85,(uint8*)"");
	    return FALSE;
    }
	
	*start_addr = 0;
	*end_addr = 0;
	
	for(i=0;i<sizeof(update_partitions)/sizeof(update_partitions[0]);i++)
	{

		ret = _get_one_image_map_ionfo(update_partitions[i], &start_address, &end_address);
		if (ret)
		{
		    /* record one partition start address */
            add_image_map_element(start_address);

            /* record the max value of end address */
            if(*end_addr < end_address)
            {
                *end_addr = end_address;
            }
		}
		else
		{
		    //UA_TRACE_ERR:"GetImageInfo: _get_one_image_map_ionfo (partition 0x%x) get error.rn"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_705_112_2_18_1_31_29_86,(uint8*)"d",update_partitions[i]);
		    return FALSE;
		}
	}
	
    /* Open special partition OSB, OS info stored in first 16KB of the partition */
    ret = GetUAPartIEntry(SCI_PARTITION_OSB, &pUaEntry);
    if ((NULL == pUaEntry) || (!ret))
    {
        //UA_TRACE_ERR:"Failed to open PARTITION(0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_714_112_2_18_1_31_29_87,(uint8*)"d",SCI_PARTITION_OSB);
        
        return FALSE;
    }

    /* Get last secotr of the OSB partition */
    end_Sct = pUaEntry->nNumOfBlks * g_SectorsPerBlk_Ua;

    memset((void *)g_ua_mbuf, 0xFF, sizeof(g_ua_mbuf));
    
    /* Search 0xE59FF018 from the OSB partition */
    for (i=0; i < end_Sct; i++)
    {
        /* Read one sector of the OSB partition */
        ret = SCI_FTL_Read(pUaEntry->ftlHandle, i, 1, g_ua_mbuf);
        if (SCI_FTL_ERROR_NONE != ret)
        {
            //UA_TRACE_ERR:"Failed to read SCI_PARTITION_OSB parition (0x%x)\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_731_112_2_18_1_31_29_88,(uint8*)"d", ret);
            return FALSE;
        }

        flag = *(uint32 *)&g_ua_mbuf[0];
        
		if ((KERNEL_IMAGE_SIGNATURE == flag) || (KERNEL_IMAGE_SIGNATURE_LE == flag))
		{
			UA_TRACE(("Kernel offset =  (0x%x)\r\n",kernel_offset));
			break;
		}
		kernel_offset += g_Sector_Size;
        
    }

    /* IF not found, return error */
    if (i == end_Sct)
    {
        UA_TRACE(("Not found Kernel code! \r\n"));
        return FALSE;
    }
    
    /* Record OSB partition address */
	start_address = pUaEntry->n1stVbn * g_SectorsPerBlk_Ua * g_Sector_Size;
	end_address  = start_address + pUaEntry->nNumOfBlks * g_SectorsPerBlk_Ua * g_Sector_Size;
	UA_TRACE(("Kernel Image:  start_addr=0x%x, end_addr=0x%x\r\n",  start_address+kernel_offset, end_address));

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


    uint32              ret = 0;
    uint32              i   = 0;
    RB_UpdateParams *   updateparams;
    UAPartEntry_T       *pUaEntry = NULL;
    uint32              nUBB_num_Blks = 0;
    uint32              nUBB_Start_Blks = 0;
    uint32              UBB_Base_addr = 0;

    /* Initialize backup ua app addr */
    memset((void *)g_Array_UBB_Addr, 0x0, sizeof(g_Array_UBB_Addr));

    /* Set RM_ImageUpdate parameter */
    updateparams->isBackgroundUpdate = 0;
    updateparams->maxReaders         = 0;

    /* Open Update image back up partition */
    ret = GetUAPartIEntry(SCI_PARTITION_UBB, &pUaEntry);
    if ((NULL == pUaEntry) || (!ret))

    {
        //UA_TRACE_ERR:"VerifySourceVersion: Failed to open PARTITION(0x%x): Error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_798_112_2_18_1_31_29_89,(uint8*)"dd",SCI_PARTITION_UBB, ret);
        
        return FALSE;
    }

    nUBB_Start_Blks= pUaEntry->n1stVbn;
    nUBB_num_Blks = pUaEntry->nNumOfBlks;
    if (nUBB_num_Blks > MAX_UBB_BLK_NUM)
    {
        //UA_TRACE_ERR:"VerifySourceVersion: UBB partiton's blk(%d) > MAX_UBB_BLK_NUM(%d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_807_112_2_18_1_31_29_90,(uint8*)"dd",nUBB_num_Blks,MAX_UBB_BLK_NUM );
        return FALSE;
    }

    /* Get UBB addr in sectors to array g_Array_UBB_Addr */
    UBB_Base_addr = nUBB_Start_Blks * g_SectorsPerBlk_Ua * g_Sector_Size;
    for (i = 0; i < nUBB_num_Blks; i++)
    {
        g_Array_UBB_Addr[i] = UBB_Base_addr + (i *g_SectorsPerBlk_Ua * g_Sector_Size );
    }

    UA_TRACE(("UBB ImageUpdate addr=0x%x\r\n",g_Array_UBB_Addr[0]));
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
		               g_Array_UBB_Addr,
		               nUBB_num_Blks,
		               FOTA_VERIFY_SOURCE_ONLY, /* verify version */
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
FOTA_FOTA_VERIFY_ERR_E IsVersionCompatible( uint32 image_start_addr,uint32 image_end_addr)
{
	uint32	i;
	RB_SectionData sectionsArray[8];
	long ret;

	ret = RB_GetSectionsData(NULL,sizeof(update_partitions)/sizeof(update_partitions[0])+1,sectionsArray, (unsigned char *)RAM_START_ADDR,RAM_START_SIZE);
	if(ret < 0)
	{
		//UA_TRACE_ERR:"RB_GetSectionsData failed  \r\n  "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_855_112_2_18_1_31_29_91,(uint8*)"");
		return ERR_FOTA_SYSTEM;
	}

    new_kernel_image_size = sectionsArray[0].new_size;
	for(i=0;i<=sizeof(update_partitions)/sizeof(update_partitions[0]);i++)
	{

		if((sectionsArray[i].new_offset==0) || (sectionsArray[i].old_offset==0))
		{
			//UA_TRACE_ERR:"ERR_FOTA_LOST: lost file.\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_865_112_2_18_1_31_29_92,(uint8*)"");
			return ERR_FOTA_LOST;
		}
		if(sectionsArray[i].old_offset!=g_current_image_map[i])
		{
			//UA_TRACE_ERR:"ERR_FOTA_SOURCE_NOT_COMP: map[%d] : 0x%08x - 0x%08x\r\n "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_870_112_2_18_1_31_30_93,(uint8*)"ddd",i,sectionsArray[i].old_offset,g_current_image_map[i]);
			return ERR_FOTA_SOURCE_NOT_COMP;
		}
		if(sectionsArray[i].new_offset!=sectionsArray[i].old_offset)
		{
			//UA_TRACE_ERR:"ERR_FOTA_FILE_NOT_COMP: map[%d] : 0x%08x - 0x%08x \r\n  "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_875_112_2_18_1_31_30_94,(uint8*)"ddd",i,sectionsArray[i].new_offset,sectionsArray[i].old_offset);

			return ERR_FOTA_FILE_NOT_COMP;
		}
	}

	if(ReadUpdateProcessFlag() == VERSION_CHECK_OK)
	{
		return ERR_FOTA_READ_VERIFY_OK;
	}
	if(VerifySourceVersion(image_start_addr,image_end_addr)==S_RB_SUCCESS)
	{
	    ret = SetUpdateProcessFlag(VERSION_CHECK_OK);
		if (ret)
		{
		    return ERR_FOTA_NONE;
		}
		else
		{
		    return ERR_FOTA_SYSTEM;
		}
	}
	return ERR_FOTA_VERIFY_CHECK_FAILURE;
}


/*****************************************************************************/
// 	Description : Image Update
//	Global resource dependence : 
//  Author: Rui.Zhang
//	Note:
/*****************************************************************************/
uint8 ImageUpdate(uint32 image_start_addr,uint32 image_end_addr, FOTA_FOTA_VERIFY_ERR_E eVerifyRet)
{

    uint32              ret = 0;
    uint32              i   = 0;
    RB_UpdateParams *   updateparams;
    UAPartEntry_T       *pUaEntry = NULL;
    uint32              nUBB_num_Blks = 0;
    uint32              nUBB_Start_Blks = 0;
    uint32              UBB_Base_addr = 0;

    uint32              wOperation = FOTA_VERIFY_UPDATE_IMAGE;

    if( (ERR_FOTA_NONE != eVerifyRet) && (ERR_FOTA_READ_VERIFY_OK != eVerifyRet))
    {
        return FALSE;
    }
    

    /* Initialize UBB partition addr */
    memset((void *)g_Array_UBB_Addr, 0x0, sizeof(g_Array_UBB_Addr));

    /* Set RM_ImageUpdate parameter */
    updateparams->isBackgroundUpdate = 0;
    updateparams->maxReaders         = 0;

    /* Open UBB partition */
    ret = GetUAPartIEntry(SCI_PARTITION_UBB, &pUaEntry);
    if ((NULL == pUaEntry) || (!ret))
    {
        //UA_TRACE_ERR:"ImageUpdate: Failed to open PARTITION(0x%x): Error (0x%x)\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_935_112_2_18_1_31_30_95,(uint8*)"dd",SCI_PARTITION_UBB, ret);
        
        return FALSE;
    }

    nUBB_Start_Blks= pUaEntry->n1stVbn;
    nUBB_num_Blks = pUaEntry->nNumOfBlks;
    if (nUBB_num_Blks > MAX_UBB_BLK_NUM)
    {
        //UA_TRACE_ERR:"ImageUpdate: UBB partiton's blk(%d) > MAX_UBB_BLK_NUM(%d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_944_112_2_18_1_31_30_96,(uint8*)"dd",nUBB_num_Blks,MAX_UBB_BLK_NUM );
        return FALSE;
    }

    /* Get UBB addr in sectors to array g_Array_UBB_Addr */
    UBB_Base_addr = nUBB_Start_Blks * g_SectorsPerBlk_Ua * g_Sector_Size;
    for (i = 0; i < nUBB_num_Blks; i++)
    {
        g_Array_UBB_Addr[i] = UBB_Base_addr + (i *g_SectorsPerBlk_Ua * g_Sector_Size );
    }

    if (ERR_FOTA_READ_VERIFY_OK == eVerifyRet)
    {
        /* Maybe power failure and re-poweron */
        wOperation = FOTA_VERIFY_UPDATE_IMAGE;
    }
    else if (ERR_FOTA_NONE == eVerifyRet)
    {
        /* verify delta through RB_ImageUpdate by operation FOTA_VERIFY_SOURCE_ONLY(0x1) */
        wOperation = FOTA_UPDATE_ONLY;
    }


    /* Update DU data */
	ret = RB_ImageUpdate(
		               NULL,
		               image_start_addr,
		               image_end_addr,
		               (unsigned char *)RAM_START_ADDR,
		               RAM_START_SIZE,
		               NULL,
		               NULL,
		               g_Array_UBB_Addr,
		               nUBB_num_Blks,
		               wOperation,
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


    uint32 new_kernel_length;
    uint8   *new_address;
    uint32 image_start_addr,image_end_addr;
	    
    uint32 ret =0;

	uint32			nandId;
	uint32          partition_Idx = 0;
	uint32          nArray_cnt = 0;

	uint32          nFtlJumpPoint = 4;          /* Required by FTL API, BML do not regard, STL default set 4 */

	FOTA_FOTA_VERIFY_ERR_E  eVerifyRet = ERR_FOTA_NONE;
	uint32          nUpdateFlag = ERR_UPDATE_SUCCESS;

    /* 1st. Init all device */
    UA_TRACE(("enter fota process..............\r\n"));
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

    UA_TRACE(("hight %d width %d\r\n",MAINSCREEN_HEIGHT,MAINSCREEN_WIDTH));

    LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"Prepare Update",sizeof("Prepare Update"));


	NANDCTL_Init (100000000);
    
    NANDCTL_Reset();


    /* 2nd. Init FTL layer */
    nandId = nFtlDev;           // now we support only one device
    
	if(SCI_FTL_ERROR_NONE != (ret = SCI_FTL_MODULE_Init()))
	{
	    //UA_TRACE_ERR:"main: SCI_FTL_MODULE_Init err(0x%x)\r\n"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1051_112_2_18_1_31_30_97,(uint8*)"d", ret);
		return FALSE;
	}

	if(SCI_FTL_ERROR_NONE != (ret = SCI_FTL_Load(nandId)))
	{
	    /* Nand device must be formatted previously */
	    //UA_TRACE_ERR:"main: SCI_FTL_Load err(0x%x)\r\n"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1058_112_2_18_1_31_30_98,(uint8*)"d", ret);
        return FALSE;
	}

	/* Read NAND dev info from FTL layer */
	if(SCI_FTL_ERROR_NONE != (ret = SCI_FTL_GetDevInfo(nandId, &g_FtlDevInfo)))
	{
	    /* Get dev info error */
	    //UA_TRACE_ERR:"main: SCI_FTL_GetDevInfo err(0x%x)\r\n"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1066_112_2_18_1_31_30_99,(uint8*)"d", ret);
		SCI_FTL_UnLoad(nandId);
		return FALSE;
	}


    /* Open all partition handle */
    g_ua_PartI.nNumOfPartEntry = g_FtlDevInfo.partsInfo.partsNum;

    nArray_cnt = sizeof(g_ua_PartI.stPEntry) /  sizeof(g_ua_PartI.stPEntry[0]);
    if(g_ua_PartI.nNumOfPartEntry > nArray_cnt)
    {
	    //UA_TRACE_ERR:"main: Part entry num(%d) more than g_ua_PartI size (%d). rn"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1079_112_2_18_1_31_30_100,(uint8*)"dd", g_ua_PartI.nNumOfPartEntry, nArray_cnt);
		return FALSE;
    }
    
    for (partition_Idx = 0; partition_Idx < g_ua_PartI.nNumOfPartEntry; 
                partition_Idx++)
    {
        g_ua_PartI.stPEntry[partition_Idx].nID 
                    = g_FtlDevInfo.partsInfo.parts[partition_Idx].partId;
        g_ua_PartI.stPEntry[partition_Idx].nAttr 
                    = g_FtlDevInfo.partsInfo.parts[partition_Idx].attrib;
        g_ua_PartI.stPEntry[partition_Idx].n1stVbn
                    = g_FtlDevInfo.partsInfo.parts[partition_Idx].ofst;
        g_ua_PartI.stPEntry[partition_Idx].nNumOfBlks
                    = g_FtlDevInfo.partsInfo.parts[partition_Idx].len;

        ret = SCI_FTL_Open(nandId, g_ua_PartI.stPEntry[partition_Idx].nID , 
                &g_ua_PartI.stPEntry[partition_Idx].ftlHandle, nFtlJumpPoint);
        if ((SCI_FTL_ERROR_NONE != ret) || (0 == g_ua_PartI.stPEntry[partition_Idx].ftlHandle))
        {
            //UA_TRACE_ERR:"main: SCI_FTL_Open(ID 0x%x) err(0x%x). rn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1100_112_2_18_1_31_30_101,(uint8*)"dd",g_ua_PartI.stPEntry[partition_Idx].nID, ret);
            return FALSE;
        }


    }

    g_SectorsPerBlk_Ua = g_FtlDevInfo.pgPerBlk * g_FtlDevInfo.sctsPerPg;
    g_Sector_Size = g_FtlDevInfo.sctSize;

    /* check buffer size */
    if (sizeof(g_ua_mbuf) <  g_Sector_Size * g_SectorsPerBlk_Ua)
    {
            //UA_TRACE_ERR:"main: (sizeof(g_ua_mbuf) <  g_Sector_Size * g_SectorsPerBlk_Ua)\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1113_112_2_18_1_31_30_102,(uint8*)"");
            return FALSE;
    }
    
    
    /* 3rd. Start RB image update */
    Load_FixnvEntry();

    UA_TRACE(("enter image update process\r\n"));	
	    	
    SCI_Sleep(1000);


    do 
    {

        
        ret = GetImageInfo(&image_start_addr, &image_end_addr);
        if(0 == ret)
        {
    		//UA_TRACE_ERR:"Failed to get image start address and end address\r\n"
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1133_112_2_18_1_31_30_103,(uint8*)"");
    		//ClearUpdateFlag(ERR_UPDATE_SYSTEM);
    		SetUpdateProcessFlag(ERR_UPDATE_SYSTEM);
            return FALSE;
        }



        nUpdateFlag = ReadUpdateProcessFlag();


        /* if update flag is update_unknow or update_system, not do version check, update directly */
        if ( (ERR_UPDATE_UNKNOW != nUpdateFlag) && (ERR_UPDATE_SYSTEM != nUpdateFlag) 
                && (VERSION_CHECK_OK != nUpdateFlag))
        {
            eVerifyRet = IsVersionCompatible(image_start_addr,image_end_addr);
            if( (ERR_FOTA_NONE != eVerifyRet) && (ERR_FOTA_READ_VERIFY_OK != eVerifyRet))
            {
        		LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"              ",sizeof("              "));
        		if (ERR_FOTA_LOST == eVerifyRet)
        		{
            		LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Delta file err",sizeof("Delta file err"));
        		}
        		else if ( ERR_FOTA_SOURCE_NOT_COMP == eVerifyRet)
        		{
            		LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Source offset uncompatible",sizeof("Source offset not compatible"));
        		}
        		else if (ERR_FOTA_FILE_NOT_COMP == eVerifyRet)
        		{
            		LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"New/Old offset uncompatible",sizeof("New/Old offset uncompatible"));
        		}
        		else if (ERR_FOTA_VERIFY_CHECK_FAILURE == eVerifyRet)
        		{
            		LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Fota verify err",sizeof("Fota verify err"));
        		}
        		else
        		{
            		LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Fota system err",sizeof("Fota system err"));
        		}

        		/* Erase du partition and not enter fota again */
        		ClearUpdateFlag(ERR_UPDATE_UNCOMPATIBLE);
        		SCI_Sleep(1000);
                return FALSE;

            }
        }
        else
        {
            /* if update progress be interrupted in previously update, do FOTA_VERIFY_UPDATE_IMAGE operation  */
            eVerifyRet = ERR_FOTA_READ_VERIFY_OK;
        }
    				
        LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"Image Updating",sizeof("Image Updating"));
    	
        /* enter update process */
        ret = ImageUpdate(image_start_addr,image_end_addr, eVerifyRet);
        UA_TRACE(("image update complete\r\n"));
        if(S_RB_SUCCESS != ret)
        {
            LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"              ",sizeof("Image Updating"));	     	
            LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Version uncompatible",sizeof("Version uncompatible"));

            /* do update in next poweron */
            SetUpdateProcessFlag(ERR_UPDATE_UNKNOW);
            SCI_Sleep(1000);
            return FALSE;

        } 
    			
    	new_address =(uint8   *) MakeMasterImageHeader(new_kernel_image_size,&new_kernel_length,0);
    	ret = Update_Kernel_Head(new_address,new_kernel_length);
    	if (!ret)
    	{
            LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"              ",sizeof("Image Updating"));	     	
            LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Update OS Header Error",sizeof("Update OS Header Error"));

            /* do update in next poweron */	
            SetUpdateProcessFlag(ERR_UPDATE_UNKNOW);
            SCI_Sleep(1000);
            return FALSE;

    	}
    		
        /* 4th. Update nv through update_nv partition to download nv partition */
    	ret = update_fixnv_image();
    	if (!ret)
    	{
            LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP,"              ",sizeof("Image Updating"));	     	
            LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Update NV Error",sizeof("Update NV Error"));

            /* do update in next poweron */
            SetUpdateProcessFlag(ERR_UPDATE_UNKNOW);
            SCI_Sleep(1000);
            return FALSE;

    	}
    	//erase_running_nv_partition();	
    	//modify update flag 	

    	LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"                     ",sizeof("Version uncompatible  "));	     	
    	LCD_DisplayText(DM_PRG_TEXT_LEFT-20,DM_PRG_TEXT_TOP,"Image Update Success",sizeof("Image Update Success"));

    	/* Clear DU partition */
    	ret = ClearUpdateFlag(ERR_UPDATE_SUCCESS);		
    	SCI_Sleep(2000);


    	if(TRUE != ret)
    	{
    		//UA_TRACE_ERR:"change update flag failed(0x%x)\r\n"
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1243_112_2_18_1_31_30_104,(uint8*)"d",ret);
            return FALSE;
    	}


        /*5th. Close FTL */
        for (partition_Idx = 0; partition_Idx < g_ua_PartI.nNumOfPartEntry; 
                    partition_Idx++)
        {


            ret = SCI_FTL_Close(g_ua_PartI.stPEntry[partition_Idx].ftlHandle);
            if (SCI_FTL_ERROR_NONE != ret) 
            {
                //UA_TRACE_ERR:"main: SCI_FTL_Close(ID 0x%x) err(0x%x). rn"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1258_112_2_18_1_31_30_105,(uint8*)"dd",g_ua_PartI.stPEntry[partition_Idx].nID, ret);
                return FALSE;
            }

            ret = SCI_FTL_UnLoad(nandId);
            if (SCI_FTL_ERROR_NONE != ret) 
            {
                //UA_TRACE_ERR:"main: SCI_FTL_UnLoad(ID 0x%x) err(0x%x). rn"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1266_112_2_18_1_31_30_106,(uint8*)"dd",nandId, ret);
                return FALSE;
            }


        }

    }while (0);

    
    /*6th. Remap and reboot */
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

    uint32 ret = 0;
    uint32 PartEntry_Index = 0;
    uint32 nPartEntry_FirstSct = 0;
    uint32 nPartEntry_LastSct  = 0;

    uint32 nReadSct_Idx = src;
    uint32 nReadSct_num = size;

    uint32 nReadSct_offset = 0;

    uint32 nPartID = 0;
    UAPartEntry_T       *pUaEntry = NULL;

    if (NULL == dst)
    {
        //UA_TRACE_ERR:"Input dst pointer null! \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1318_112_2_18_1_31_30_107,(uint8*)"");
        return 1;
    }

    for (PartEntry_Index = 0; PartEntry_Index < g_ua_PartI.nNumOfPartEntry; PartEntry_Index++)
    {
        pUaEntry = &(g_ua_PartI.stPEntry[PartEntry_Index]);

        nPartEntry_FirstSct = pUaEntry->n1stVbn * g_SectorsPerBlk_Ua;
        nPartEntry_LastSct  = nPartEntry_FirstSct + pUaEntry->nNumOfBlks * g_SectorsPerBlk_Ua;
        
        if ( (nPartEntry_FirstSct <= nReadSct_Idx) &&
             (nPartEntry_LastSct >= (nReadSct_Idx + nReadSct_num))
            )
        {
            break;
        }
        else
        {
            pUaEntry = NULL;
        }
        
    }

    /* Find partition */
    if (NULL != pUaEntry)
    {

        nReadSct_offset = nReadSct_Idx - nPartEntry_FirstSct;

        /* Initialize data buffer */
        memset((void *)dst, 0xFF, (nReadSct_num * g_Sector_Size));
        
        ret = SCI_FTL_Read(pUaEntry->ftlHandle, nReadSct_offset, nReadSct_num, (void *) dst);
        if (SCI_FTL_ERROR_NONE != ret)
        {
            //UA_TRACE_ERR:"Failed to read nand flash by sector (0x%x)\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1354_112_2_18_1_31_30_108,(uint8*)"d", ret);
            return 1;
        }
    }
    else
    {
        //UA_TRACE_ERR:"ReadNandflashBysector: Can not find partition, Sct_Idx(%d) Sct_Num(%d).rn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1361_112_2_18_1_31_30_109,(uint8*)"dd", nReadSct_Idx, nReadSct_num);
        return 1;
    }

    return 0;

}



PUBLIC int WriteNandflashByBlk(void * pWBuf, uint32 nWBlk_Idx, uint32 nWBlk_num)
{
    uint32 ret = 0;
    uint32 PartEntry_Index = 0;

    uint32 nPartEntry_FirstBlk = 0;
    uint32 nPartEntry_LastBlk  = 0;

    uint32 nWSct_Idx = 0;
    uint32 nWSct_num = 0;
    uint32 nWSct_offset = 0;

    uint32 nPartID = 0;
    UAPartEntry_T       *pUaEntry = NULL;
    SCI_FTL_PARTITION_INFO partInfo;

    if (NULL == pWBuf)
    {
        //UA_TRACE_ERR:"Input pWBuf pointer null! \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1389_112_2_18_1_31_31_110,(uint8*)"");
        return 1;
    }


    /* Find partition entry by block offset and block num */
    for (PartEntry_Index = 0; PartEntry_Index < g_ua_PartI.nNumOfPartEntry; PartEntry_Index++)
    {
        pUaEntry = &(g_ua_PartI.stPEntry[PartEntry_Index]);

        nPartEntry_FirstBlk = pUaEntry->n1stVbn ;
        nPartEntry_LastBlk  = nPartEntry_FirstBlk + pUaEntry->nNumOfBlks;
        
        if ( (nPartEntry_FirstBlk <= nWBlk_Idx) &&
             (nPartEntry_LastBlk >= (nWBlk_Idx + nWBlk_num))
            )
        {

            break;
        }
        else
        {
            pUaEntry = NULL;
        }
        
    }

    /* Get partition info */
    if (NULL != pUaEntry)
    {
        ret = SCI_FTL_GetPartInfo(pUaEntry->ftlHandle, &partInfo);
        if (SCI_FTL_ERROR_NONE != ret)
        {
            //UA_TRACE_ERR:"WriteNandflashByBlk: SCI_FTL_GetPartInfo (ID:0x%x) err(0x%x)rn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1423_112_2_18_1_31_31_111,(uint8*)"dd", pUaEntry->nID, ret);
            return 1;
        }

        /* Do erase block firstly */
        nWSct_num = nWBlk_num * g_SectorsPerBlk_Ua;
        nWSct_offset = (nWBlk_Idx - nPartEntry_FirstBlk) * g_SectorsPerBlk_Ua;
        
        /* erase sector must align to erase_unit */
        if(partInfo.eraseUnit)
        {
            if ( (nWSct_offset % partInfo.eraseUnit) || (nWSct_num % partInfo.eraseUnit))
            {
                //UA_TRACE_ERR:"WriteNandflashByBlk: align error. nWSct_offset(%d) nWSct_num(%d) partInfo.eraseUnit(%d)rn"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1437_112_2_18_1_31_31_112,(uint8*)"ddd", nWSct_offset, nWSct_num, partInfo.eraseUnit);
                return 1;
            }
        }
        else
        {
            /* not support erase STL partition in fota */
            //UA_TRACE_ERR:"WriteNandflashByBlk: not support erase STL partition(0x%x) in fota rn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1445_112_2_18_1_31_31_113,(uint8*)"d", pUaEntry->nID);
            return 1;
        }
        
        ret = SCI_FTL_Erase(pUaEntry->ftlHandle, nWSct_offset, nWSct_num);
        if (SCI_FTL_ERROR_NONE != ret)
        {
            //UA_TRACE_ERR:"WriteNandflashByBlk: Failed to erase nand flash (0x%x)\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1452_112_2_18_1_31_31_114,(uint8*)"d", ret);
            return 1;
        }

        
        /* Write sector */
        ret = SCI_FTL_Write(pUaEntry->ftlHandle, nWSct_offset, nWSct_num, (uint8 *)pWBuf);
        if (SCI_FTL_ERROR_NONE != ret)
        {
            //UA_TRACE_ERR:"WriteNandflashByBlk: Failed to write nand flash (0x%x)\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1461_112_2_18_1_31_31_115,(uint8*)"d", ret);
            return 1;
        }
    }
    else
    {
        //UA_TRACE_ERR:"ReadNandflashBysector: Can not find partition, Blk_Idx(%d) Blk_Num(%d).rn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRDFTL_UA_1468_112_2_18_1_31_31_116,(uint8*)"dd", nWBlk_Idx, nWBlk_num);
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

