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
#include "master_img_gen.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**
#	search nvitem mode: 
#					0: search specified nv id in nv partition
#					1: search the first nv id in nv partition
#					2: search the nv after the specified nv id in nv partition
**/
#define		SEARCH_BY_NVID		0
#define		SEARCH_FIRST_NVID		1
#define		SEARCH_NEXT_NVID		2

/**
#			define the max buffer used to update nv parameters.
**/
#define		MAX_FIXNV_LENGTH		(64*1024)

#define		INVALID_NVID		(0xFFFF)


#define RAND_TO_UINT32( _addr ) \
    if( (_addr) & 0x3 ){ _addr += 0x4 - ((_addr) & 0x3); }


extern  unsigned int g_SctsPerBlk;
extern  uint8 g_mbuf[]; 
extern uint8 g_data_cache1[];
/*
#
# list the reserve nv item during update...
#
*/
static uint16	backup_nvitem_count=0;
static uint16	backup_nvitems[]={
	0x0401,//NV_BT_ADDRESS		
	377,	   //NV_IMEI1
	5,
	2,
	1302,
	INVALID_NVID,
};

/*
#
# define buffer used to create new fixnv.
#
*/
static	uint32 backup_buffer_offset;
uint8	backup_nv_buffer[MAX_FIXNV_LENGTH];


static  XSRPartEntry  g_download_nv_part_entry;
static  XSRPartEntry  g_updated_nv_part_entry;


/*****************************************************************************/
// 	Description : initialize the two nv partition entry.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
uint32 Load_FixnvEntry()
{
    int           ret;
		
    ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_DOWNLOADED_NV, &g_download_nv_part_entry);
    if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to load FIXNV partition entry (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_80_112_2_18_1_31_22_0,(uint8*)"d",ret);
        return FALSE;
    }
    UA_TRACE(("FIXNV_Partition: start blkno  %d  block count %d\n",g_download_nv_part_entry.n1stVbn,g_download_nv_part_entry.nNumOfBlks));

    ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_UPDATED_NV, &g_updated_nv_part_entry);
    if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to load Updated NV partition entry (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_87_112_2_18_1_31_22_1,(uint8*)"d",ret);
        return FALSE;
    }
    UA_TRACE(("UPDATEDNV_Partition: start blkno  %d  block count %d\n",g_updated_nv_part_entry.n1stVbn,g_updated_nv_part_entry.nNumOfBlks));
	return BML_SUCCESS;
}

/*****************************************************************************/
// 	Description : read nvitem by nvid and the mode.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
static uint32 read_nvitem_by_id(XSRPartEntry  *read_part_entry_ptr,uint16 nvid,uint8 *buffer,uint32 max_length,uint8 mode)
{
    int           ret;
    uint32	  start_sector_no;
    uint32 	  end_sector_no;
    uint16	  cur_nv_id;
    uint16	  cur_nv_length;
    uint32	  cur_sector_no;
    uint32	  next_sector_no;
    uint32	  OffInSec;
    uint32 	  OffInBlk;
    uint32 	  find = 0;
    uint32   search_flag;
    uint32 copy_len=0;

    search_flag = 0;
    
    start_sector_no = (read_part_entry_ptr->n1stVbn) * g_SctsPerBlk;
    end_sector_no = start_sector_no + read_part_entry_ptr->nNumOfBlks*g_SctsPerBlk;

    ret = BML_MRead(VOLUME_NUMBER, start_sector_no, 1, g_mbuf, 
    	0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);

    if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_122_112_2_18_1_31_22_2,(uint8*)"d", ret);
       	return FALSE;
    }
	
    cur_sector_no = start_sector_no;
    OffInSec =  sizeof(uint32);
    OffInBlk =  sizeof(uint32);

    do
    {
    	cur_nv_id = *(uint16 *)&g_mbuf[OffInSec];
   	cur_nv_length = *(uint16 *)&g_mbuf[OffInSec+sizeof(uint16)];
   	
	if((cur_nv_length == 0xffff)||(cur_nv_id == 0xffff))
   	{
   		return 0;
   	}
	else 	if(mode == SEARCH_FIRST_NVID)
	{
		//UA_TRACE("find first....\n");
		find = 1;
		break;
	}
	else   if(nvid == cur_nv_id)
 	{
		if(mode == SEARCH_BY_NVID)
 		{
 			//UA_TRACE("find...........\n");
			find = 1;
 			break;
		}
		else if(mode == SEARCH_NEXT_NVID)
		{
			search_flag = 1;
		}
 	}
	else if(search_flag == 1)
	{
		//UA_TRACE("find next...........\n");
		find = 1;
		break;
	}
	
 	
 	OffInBlk += sizeof(uint32);	
       OffInBlk = OffInBlk + cur_nv_length + (cur_nv_length & 0x1); // work boundary
            	
        RAND_TO_UINT32( OffInBlk )
        next_sector_no = OffInBlk/XSR_SECTOR_SIZE + start_sector_no;
        OffInSec = OffInBlk%XSR_SECTOR_SIZE;
	//UA_TRACE("next_sector_no: %d  OffInBlk  %d \n",next_sector_no,OffInBlk);
    	if(next_sector_no!=cur_sector_no)
    	{
	    	ret = BML_MRead(VOLUME_NUMBER, next_sector_no, 1, g_mbuf, 
	    		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);
	
	        if (BML_SUCCESS != ret) {
		       //UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
		       SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_179_112_2_18_1_31_22_3,(uint8*)"d", ret);
		       	return FALSE;
		}
		cur_sector_no=next_sector_no;
	}
    }while(cur_sector_no < end_sector_no);
    
     if(buffer==NULL)
    {
    		
    		return cur_nv_length + sizeof(uint32);
    }
	//UA_TRACE("offset: 0x%04x nvid 0x%04x len 0x%04x \n",OffInBlk,cur_nv_id,cur_nv_length); 
    if(max_length >= sizeof(uint32))
    {
	    *(uint16 *)&buffer[0] = cur_nv_id;
	    *(uint16 *)&buffer[2] = cur_nv_length;

    }
     copy_len = 0;
    if(max_length >= sizeof(uint32)+cur_nv_length)
    {
	    if(find==1)
	    {
	    	uint32 size;
	    	size = cur_nv_length+4;
		// UA_TRACE("Load ID= 0x%04x len=0x%04x  OffInSec = %d \r\n",cur_nv_id,size,OffInSec);
		do{
		    	if(size <= (XSR_SECTOR_SIZE- OffInSec))
	    		{
	    			memcpy(buffer+copy_len,&g_mbuf[OffInSec],size);
	    			size = 0;
	    			copy_len += size;
	    		}
	    		else
	    		{
		    		memcpy(buffer+copy_len,&g_mbuf[OffInSec],XSR_SECTOR_SIZE- OffInSec);
	    			size = size - (XSR_SECTOR_SIZE- OffInSec);
				copy_len += (XSR_SECTOR_SIZE- OffInSec);
		    		cur_sector_no++;
		    		OffInSec = 0;
			    	ret = BML_MRead(VOLUME_NUMBER, cur_sector_no ,1, g_mbuf, 
			    		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);
				
			        if (BML_SUCCESS != ret) {
				       //UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
				       SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_224_112_2_18_1_31_22_4,(uint8*)"d", ret);
				       	return FALSE;
				}    		
		    	}    		
	    	}while(size > 0);
    	}
    }
    return (cur_nv_length+sizeof(uint32));
}
/*****************************************************************************/
// 	Description : get the nvitem specified by the nvitem ID.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
static uint16 get_fixnv_item(XSRPartEntry  *read_part_entry_ptr, uint16 identifier ,uint8 *buffer ,uint32  max_buffer_length)
{
	if(identifier == INVALID_NVID)
	{
		return 0;
	}
	return read_nvitem_by_id(read_part_entry_ptr,identifier,buffer,max_buffer_length,SEARCH_BY_NVID);
}
/*****************************************************************************/
// 	Description : get the first nvitem in a nv partition.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
static uint16 get_first_nvitem(XSRPartEntry  *read_part_entry_ptr,uint8 *buffer ,uint32  max_buffer_length)
{
	return read_nvitem_by_id(read_part_entry_ptr,INVALID_NVID,buffer,max_buffer_length,SEARCH_FIRST_NVID);
}

/*****************************************************************************/
// 	Description : get the nvitem after the specifid nv ID.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
static uint16 get_next_nvitem(XSRPartEntry  *read_part_entry_ptr,uint16 identifier,uint8 *buffer ,uint32  max_buffer_length)
{
	if(identifier == INVALID_NVID)
	{
		return 0;
	}
	return read_nvitem_by_id(read_part_entry_ptr,identifier,buffer,max_buffer_length,SEARCH_NEXT_NVID);
}

/*****************************************************************************/
// 	Description : save the update flag of update nv partition.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
static void save_update_flag(uint8 *buffer)
{
	int           ret;
	uint32	  start_sector_no;

	start_sector_no = (g_updated_nv_part_entry.n1stVbn) * g_SctsPerBlk;

	ret = BML_MRead(VOLUME_NUMBER, start_sector_no, 1, backup_nv_buffer, 
		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);
	if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_281_112_2_18_1_31_22_5,(uint8*)"d", ret);
		return ;
	} 
	backup_buffer_offset = 4;
}

 void	 backup_fixnv_items()
{
	uint16	nv_length;
	uint16	i;

	backup_buffer_offset = 0;
	memset(backup_nv_buffer,0x00,sizeof(backup_nv_buffer));
	
	save_update_flag(backup_nv_buffer);
	;
	for(i=0;i<sizeof(backup_nvitems)/sizeof(backup_nvitems[0]);i++)
	{
		if(backup_nvitems[i]== INVALID_NVID)
		{
			break;
		}
	}
	backup_nvitem_count=i;
	if(backup_nvitem_count == 0)
	{
		return ;	
	}
	UA_TRACE(("backup_nv start:...\n"));

	for(i=0;i<backup_nvitem_count;i++)
	{

		nv_length = get_fixnv_item( &g_download_nv_part_entry,backup_nvitems[i],&backup_nv_buffer[backup_buffer_offset],sizeof(backup_nv_buffer)-backup_buffer_offset);

		backup_buffer_offset = backup_buffer_offset + nv_length + (nv_length & 0x1);
		RAND_TO_UINT32( backup_buffer_offset );
		UA_TRACE(("NV id: 0x%04x   Offset: 0x%04x\n",backup_nvitems[i],backup_buffer_offset));
		if(backup_buffer_offset >= sizeof(backup_nv_buffer))
		{
			//UA_TRACE_ERR:"Analysis fixnv error0 !!!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_321_112_2_18_1_31_23_6,(uint8*)"");
			while(1);
		}
	}
	UA_TRACE(("backup_fixnv end\n"));
}
/*****************************************************************************/
// 	Description : decide whether the nv ID in reserved group.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
static uint32	 IsReservedItem(uint16 nvitem_id)
{
	uint16	i;

	if(backup_nvitem_count == 0)
	{
		return 0;
	}
	for(i=0;i<backup_nvitem_count;i++)
	{
		if(nvitem_id == backup_nvitems[i])
		{
			return 1;
		}
	}
	return 0;
}
/*****************************************************************************/
// 	Description : save the updated nvitem to download partition.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
uint32 save_updated_fixnv()
{
	int           ret;
	uint32	  start_sector_no;


	UA_TRACE(("save_updated_fixnv ......:0x%04x \r\n",backup_buffer_offset));

	start_sector_no = (g_download_nv_part_entry.n1stVbn) * g_SctsPerBlk;


	ret = BML_MRead(VOLUME_NUMBER, start_sector_no, g_SctsPerBlk, g_data_cache1,
			0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);   

	if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to BML_MRead\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_367_112_2_18_1_31_23_7,(uint8*)"",ret);
		return FALSE;
	}	

	memcpy(g_data_cache1,backup_nv_buffer,backup_buffer_offset);
	return RB_WriteBlock(NULL,start_sector_no*XSR_SECTOR_SIZE,g_data_cache1);	 

}
/*****************************************************************************/
// 	Description : compose the new nv parameters and save updated nv parameter.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
void  update_fixnv_image()
{
	uint16	nv_length;
	uint16	nvitem_id;
	uint32	nvitem_size;
	uint16	NvitemInfo[2];
	
	backup_fixnv_items();
	nvitem_size = get_first_nvitem(&g_updated_nv_part_entry,(uint8 *)NvitemInfo,sizeof(NvitemInfo));

	UA_TRACE(("Update fixnv image start:....\n",nvitem_id));
	if(nvitem_size >= sizeof(uint32))
	{
		do{
				
			nvitem_id = NvitemInfo[0];
			nv_length = NvitemInfo[1];
			if(nvitem_id == INVALID_NVID)
			{
				break;
			}
			UA_TRACE(("(NVID:0x%04x length %d\n",nvitem_id,nv_length));
			if(!IsReservedItem(nvitem_id))
			{
				nv_length = get_fixnv_item(&g_updated_nv_part_entry,nvitem_id,&backup_nv_buffer[backup_buffer_offset],sizeof(backup_nv_buffer)-backup_buffer_offset);
				
				backup_buffer_offset = backup_buffer_offset + nv_length + (nv_length & 0x1);
				RAND_TO_UINT32( backup_buffer_offset );

				if(backup_buffer_offset > sizeof(backup_nv_buffer))
				{
					//UA_TRACE_ERR:"Analysis fixnv error1	!!!!"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_410_112_2_18_1_31_23_8,(uint8*)"");
					while(1);
				}
			}
			else
			{
				UA_TRACE(("Skip : 0x%04x\n",nvitem_id));
			}

			nvitem_size = get_next_nvitem(&g_updated_nv_part_entry,nvitem_id,(uint8 *)NvitemInfo ,sizeof(NvitemInfo));
			
		}while(nvitem_size > sizeof(uint32));
		
	}
	save_updated_fixnv();
}

/*****************************************************************************/
// 	Description : compare old version and new version.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
void	erase_running_nv_partition()
{
	int             ret;
	XSRPartEntry    part_entry;
	int		i;

	ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_NV, &part_entry);
	if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to load DM DATA partition entry (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_439_112_2_18_1_31_23_9,(uint8*)"d",ret);
		return ;
	}
	for(i=0;i<part_entry.nNumOfBlks;i++)
	{
		ret = BML_EraseBlk(VOLUME_NUMBER, part_entry.n1stVbn+i, BML_FLAG_SYNC_OP); 
		if (BML_SUCCESS != ret) {
			//UA_TRACE_ERR:"Failed to erase update flag parition (0x%x)\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_446_112_2_18_1_31_23_10,(uint8*)"d", ret);
			return ;
		}
	}	
}

/*****************************************************************************/
// 	Description : calculate the kernel head length.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
uint32 Get_Kernel_Head_Length()
{
	int           ret;
	static  XSRPartEntry  part_entry;	
	uint32 flag_addr;   
	uint i;	


	ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_OSB, &part_entry);
	if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to load DM DATA partition entry (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_466_112_2_18_1_31_23_11,(uint8*)"d",ret);
		return FALSE;
	}

	flag_addr = (part_entry.n1stVbn) * g_SctsPerBlk;

	i=0;
	do{
		ret = BML_MRead(VOLUME_NUMBER, flag_addr+i, 1, g_mbuf,
		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);   	
		if (BML_SUCCESS != ret) {
			//UA_TRACE_ERR:"Failed to read UP parition (0x%x)\r\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_477_112_2_18_1_31_23_12,(uint8*)"d", ret);
			return FALSE;
		}

		i++;
	}while((*(uint32 *)g_mbuf)==0xFFFFFFFF);

	return ( i-1);

}
/*****************************************************************************/
// 	Description : compare old version and new version.
//	Global resource dependence : 
//	Author: jiayong.yang
//	Note:
/*****************************************************************************/
uint32 Update_Kernel_Head(uint8 *buffer,uint32 length)
{
	int           ret;
	static  XSRPartEntry  part_entry;	
	uint32 flag_addr;   
	uint32 head_offset=Get_Kernel_Head_Length();


	ret = BML_LoadPIEntry(VOLUME_NUMBER, SCI_PARTITION_OSB, &part_entry);
	if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to load DM DATA partition entry (0x%x)\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_501_112_2_18_1_31_23_13,(uint8*)"d",ret);
		return FALSE;
	}

	flag_addr = (part_entry.n1stVbn) * g_SctsPerBlk;

	ret = BML_MRead(VOLUME_NUMBER, flag_addr, g_SctsPerBlk, g_data_cache1,
		0, BML_FLAG_ECC_ON | BML_FLAG_BBM_ON);  

	if (BML_SUCCESS != ret) {
		//UA_TRACE_ERR:"Failed to BML_MRead\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,NV_UPDATE_511_112_2_18_1_31_23_14,(uint8*)"",ret);
		return FALSE;
	}

	memcpy(g_data_cache1+head_offset*XSR_SECTOR_SIZE,buffer,length);
	return RB_WriteBlock(NULL,flag_addr*XSR_SECTOR_SIZE,g_data_cache1);

}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
