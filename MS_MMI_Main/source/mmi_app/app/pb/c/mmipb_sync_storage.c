/****************************************************************************
** File Name:      mmipb_sync_storage.c                                        *
** Author:           wang.kun                                                      *
** Date:           07/09/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data storage.                                  *             
**
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _MMIPB_SYNC_STORAGE_C_
#define _MMIPB_SYNC_STORAGE_C_

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mn_type.h"
#include "mmipb_datalist.h"
#include "mmipb_storage.h"
#include "mmipb_sync_datalist.h"
#include "mmipb_sync_storage.h"


/**--------------------------------------------------------------------------*
**                                                                                                                              *
**--------------------------------------------------------------------------*/
#define MAX_PB_SYNC_ITEMS (2000)
#define BIT_SETS_BYTES  256  //be sure it is bigger than (MAX_PB_SYNC_ITEMS/8 + 1)

#define GET_BIT(header, index) ( ( (header.bitsets[index / 8]) & (1 << (index % 8))) >> (index % 8) )
#define SET_BIT(header, index) { header.bitsets[index / 8] = header.bitsets[index / 8] | (1 << (index % 8));}
#define CLR_BIT(header, index) { header.bitsets[index / 8] = header.bitsets[index / 8] & (~(1 << (index % 8)));}

typedef struct
{
	MMIPB_PHONE_UPDATE_INFO_T item;
}MMIPB_PHONE_UPDATE_STOREAGE_RECORD_T;

typedef struct
{
	uint32 total;
	uint8 bitsets[256];
}MMIPB_PHONE_UPDATE_STOREAGE_HEADER_T;


#define PB_SYNC_DATA_FILE_NAME (s_pbsync_data_file)

//LOCAL const wchar s_pbsync_data_file[] = { 'D',':','\\','S','y','s','t','e','m','\\','p','b','_','s','y','n','c','.','d', 'a', 't' , 0 };
LOCAL const wchar s_pbsync_data_file[] = { 'C',':','\\','p','b','_','s','y','n','c','.','d', 'a', 't' , 0 };

LOCAL SCI_SEMAPHORE_PTR s_pb_sync_nv_semphore_p = PNULL;

/********************************************************************************
** Description: This function is used to create semphore for pb sync storage                 *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
LOCAL void MMIPB_SYNC_NV_Create_Locker(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_NV_Create_Locker");
	s_pb_sync_nv_semphore_p = SCI_CreateSemaphore("s_pb_sync_nv_semphore_p", 1);
}

/********************************************************************************
** Description: This function is used to get semphore for pb sync storage                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
LOCAL void MMIPB_SYNC_NV_Lock(void)
{
	SCI_GetSemaphore(s_pb_sync_nv_semphore_p, SCI_WAIT_FOREVER);
	
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_NV_Lock");
}

/********************************************************************************
** Description: This function is used to put semphore for pb sync storage                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
LOCAL void MMIPB_SYNC_NV_Unlock(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_NV_Unlock");
	
	SCI_PutSemaphore(s_pb_sync_nv_semphore_p);
}

/********************************************************************************
** Description: This function is used to initalze for pb sync storage                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_NV_Initalize(void)
{
	MMIPB_SYNC_NV_Create_Locker();
	return MMIPB_ERROR_SUCCESS;	
}	

/********************************************************************************
** Description: This function is used to write one phone update item to file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Write_Phone_Update_Item(uint16* index, MMIPB_PHONE_UPDATE_INFO_T* item)
{
	uint16 i;
	uint32 rw_size  = 0;
	uint32 offset  = 0;
	MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;

	MMIPB_PHONE_UPDATE_STOREAGE_HEADER_T header;
	MMIPB_PHONE_UPDATE_STOREAGE_RECORD_T record;

	MMIFILE_ERROR_E error;

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Write_Phone_Update_Item");

	MMIPB_SYNC_NV_Lock();
	
	/* try to open exist file */
     file_handle = MMIAPIFMM_CreateFile(PB_SYNC_DATA_FILE_NAME, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ | SFS_MODE_WRITE, NULL,NULL);
	
	if (SFS_INVALID_HANDLE == file_handle)
	{
		/* open a new file */
		 file_handle = MMIAPIFMM_CreateFile(PB_SYNC_DATA_FILE_NAME, SFS_MODE_OPEN_ALWAYS | SFS_MODE_READ | SFS_MODE_WRITE, NULL,NULL);
		
		if (SFS_INVALID_HANDLE == file_handle)
		{
			MMIPB_SYNC_NV_Unlock();
			
			SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Write_Phone_Update_Item, open failed!");
			return MMIPB_ERROR_ERROR;
		}

		SCI_MEMSET(&header, 0x00, sizeof(MMIPB_PHONE_UPDATE_STOREAGE_HEADER_T));
		
		/* write header */
		MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);
		
		error = MMIAPIFMM_WriteFile(file_handle, &header, sizeof(header), &rw_size, NULL);
		if((SFS_ERROR_NONE != error) || (rw_size != sizeof(header)))
		{
			MMIAPIFMM_CloseFile(file_handle);

			MMIPB_SYNC_NV_Unlock();

			SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Write_Phone_Update_Item, write header failed!");
			return MMIPB_ERROR_ERROR;
		}		
	}

	/* read header */
	MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);

    error = MMIAPIFMM_ReadFile(file_handle, &header, sizeof(header), &rw_size, NULL);
    if((SFS_ERROR_NONE != error) || (rw_size != sizeof(header)))
	{
		MMIAPIFMM_CloseFile(file_handle);
		
		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Write_Phone_Update_Item, read header failed!");
		return MMIPB_ERROR_ERROR;
	}

	/* locate a free item */
	for(i = 0; i < MAX_PB_SYNC_ITEMS; i++)
	{
		if (GET_BIT(header, i) == 0)
		{
			break;
		}
	}

	/*  failed to locate a free item */
	if (i == MAX_PB_SYNC_ITEMS)
	{
		MMIAPIFMM_CloseFile(file_handle);
		
		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Write_Phone_Update_Item, full!");
		return MMIPB_ERROR_ERROR;
	}

	SET_BIT(header, i);
	header.total++;

	SCI_MEMCPY(&record.item, item, sizeof(MMIPB_PHONE_UPDATE_INFO_T));
	
	/*  set index to contact id */
	if (MMIPB_OPC_DELETE == record.item.modify_type)
	{
		record.item.contact_id = i;
	}

	/* write record */
	offset = sizeof(header) + sizeof(MMIPB_PHONE_UPDATE_STOREAGE_RECORD_T) * i;
	MMIAPIFMM_SetFilePointer(file_handle, offset, MMIFILE_SEEK_BEGIN);
	
    error = MMIAPIFMM_WriteFile(file_handle, &record, sizeof(record), &rw_size, NULL);
    if((SFS_ERROR_NONE != error) || (rw_size != sizeof(record)))
	{
		MMIAPIFMM_CloseFile(file_handle);
		
		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Write_Phone_Update_Item, write record failed!");
		return MMIPB_ERROR_ERROR;
	}
	
	/* write header */
	MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);
	
    error = MMIAPIFMM_WriteFile(file_handle, &header, sizeof(header), &rw_size, NULL);
    if((SFS_ERROR_NONE != error) || (rw_size != sizeof(header)))
	{
		MMIAPIFMM_CloseFile(file_handle);
		
		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Write_Phone_Update_Item, write header2 failed!");
		return MMIPB_ERROR_ERROR;
	}
	
	/* succress */
    *index = i;
	
	MMIAPIFMM_CloseFile(file_handle);
	
	MMIPB_SYNC_NV_Unlock();

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Write_Phone_Update_Item succress,index = %d", *index);

	return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to read one phone update item from file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Read_Phone_Update_Item(uint16 index, MMIPB_PHONE_UPDATE_INFO_T* item)
{
	uint32 rw_size  = 0;
	uint32 offset  = 0;
	 MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;

	MMIPB_PHONE_UPDATE_STOREAGE_HEADER_T header;
	MMIPB_PHONE_UPDATE_STOREAGE_RECORD_T record;

	MMIFILE_ERROR_E error;

	/* check parameter */
	if ((index >= MAX_PB_SYNC_ITEMS) || (item == NULL))
	{
		return MMIPB_ERROR_ERROR;
	}

	MMIPB_SYNC_NV_Lock();
	
	/* open file */
     file_handle = MMIAPIFMM_CreateFile(PB_SYNC_DATA_FILE_NAME, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL,NULL);
	
	if (SFS_INVALID_HANDLE == file_handle)
	{
		MMIPB_SYNC_NV_Unlock();
		
		return MMIPB_ERROR_ERROR;
	}

	/* read header */
    error = MMIAPIFMM_ReadFile(file_handle, &header, sizeof(header), &rw_size, NULL);
    if((SFS_ERROR_NONE != error) || (rw_size != sizeof(header)))
	{
		MMIAPIFMM_CloseFile(file_handle);
		MMIPB_SYNC_NV_Unlock();
		return MMIPB_ERROR_ERROR;
	}

	/* check header bit */
	if (GET_BIT(header, index) == 0)
	{
		MMIAPIFMM_CloseFile(file_handle);
		MMIPB_SYNC_NV_Unlock();
		return MMIPB_ERROR_ERROR;
	}

	/* read record */
	offset = sizeof(header) + sizeof(MMIPB_PHONE_UPDATE_STOREAGE_RECORD_T) * index;
	MMIAPIFMM_SetFilePointer(file_handle, offset, MMIFILE_SEEK_BEGIN);

    error = MMIAPIFMM_ReadFile(file_handle, &record, sizeof(record), &rw_size, NULL);
    if((SFS_ERROR_NONE != error) || (rw_size != sizeof(record)))
	{
		MMIAPIFMM_CloseFile(file_handle);
		MMIPB_SYNC_NV_Unlock();
		return MMIPB_ERROR_ERROR;
	}

	/* check index */
	if (record.item.contact_id != index)
	{
		MMIAPIFMM_CloseFile(file_handle);
		MMIPB_SYNC_NV_Unlock();
		return MMIPB_ERROR_ERROR;
	}
	
	/* succress */
	SCI_MEMCPY(item, &record.item, sizeof(MMIPB_PHONE_UPDATE_INFO_T));

	MMIAPIFMM_CloseFile(file_handle);

	MMIPB_SYNC_NV_Unlock();
	
	return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to read all phone update items from file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Read_All_Phone_Update_Item(void)
{
	uint16 i,count;
	uint32 rw_size	= 0;
	uint32 offset  = 0;
	MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;

	MMIPB_PHONE_UPDATE_STOREAGE_HEADER_T header;
	MMIPB_PHONE_UPDATE_STOREAGE_RECORD_T record;
	MMIPB_PHONE_UPDATE_INFO_T item;

	MMIFILE_ERROR_E error;
	
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Read_All_Phone_Update_Item");

	MMIPB_SYNC_NV_Lock();
	
	/* open file */
    file_handle = MMIAPIFMM_CreateFile(PB_SYNC_DATA_FILE_NAME, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, NULL,NULL);
	
	if (SFS_INVALID_HANDLE == file_handle)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Read_All_Phone_Update_Item, open file failed!");
		MMIPB_SYNC_NV_Unlock();
		return MMIPB_ERROR_ERROR;
	}

	/* read header */
	error = MMIAPIFMM_ReadFile(file_handle, &header, sizeof(header), &rw_size, NULL);
	if((SFS_ERROR_NONE != error) || (rw_size != sizeof(header)))
	{
		MMIAPIFMM_CloseFile(file_handle);

		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Read_All_Phone_Update_Item, open header failed!");
		return MMIPB_ERROR_ERROR;
	}

	for(i = 0, count = 0; i < MAX_PB_SYNC_ITEMS && count < header.total; i++)
	{
		/* check header bit */
		if (GET_BIT(header, i) == 0)
		{
			continue;
		}
		
		/* read record */
		offset = sizeof(header) + sizeof(MMIPB_PHONE_UPDATE_STOREAGE_RECORD_T) * i;
		MMIAPIFMM_SetFilePointer(file_handle, offset, MMIFILE_SEEK_BEGIN);

		error = MMIAPIFMM_ReadFile(file_handle, &record, sizeof(record), &rw_size, NULL);
		if((SFS_ERROR_NONE != error) || (rw_size != sizeof(record)))
		{
			SCI_TRACE_LOW("[MMIPB] (%d), read record failed!", i);
			break;
		}
		
		/* check index */
		if (record.item.contact_id != i)
		{
			SCI_TRACE_LOW("[MMIPB] (%d), record data wrong!", i);
			break;
		}
		
		/* success */
		MMIPB_SYNC_Phone_Update_List_Append_With_Info(&record.item);
		count++;
	}

	if (SFS_INVALID_HANDLE != file_handle)
	{
		MMIAPIFMM_CloseFile(file_handle);
	}

	MMIPB_SYNC_NV_Unlock();

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Read_All_Phone_Update_Item, count = %d, header.total = %d", count, header.total);

	return (count == header.total) ? MMIPB_ERROR_SUCCESS : MMIPB_ERROR_ERROR;
}

/********************************************************************************
** Description: This function is used to save one phone update item to file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Del_Phone_Update_Item(uint16 index)
{
	uint16 i;
	uint32 rw_size  = 0;
	uint32 offset  = 0;
	MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;

	MMIPB_PHONE_UPDATE_STOREAGE_HEADER_T header;

	MMIFILE_ERROR_E error;

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Del_Phone_Update_Item,index=%d", index);
	/* check index */
	if (index >= MAX_PB_SYNC_ITEMS)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Del_Phone_Update_Item, index invaild!");
		return MMIPB_ERROR_ERROR;
	}
	
	MMIPB_SYNC_NV_Lock();
	
	/* open file */
     file_handle = MMIAPIFMM_CreateFile(PB_SYNC_DATA_FILE_NAME, SFS_MODE_OPEN_ALWAYS | SFS_MODE_READ | SFS_MODE_WRITE, NULL,NULL);
	
	if (SFS_INVALID_HANDLE == file_handle)
	{
		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Del_Phone_Update_Item, open file failed!");
		return MMIPB_ERROR_ERROR;
	}

	/* read header */
    error = MMIAPIFMM_ReadFile(file_handle, &header, sizeof(header), &rw_size, NULL);
    if((SFS_ERROR_NONE != error) || (rw_size != sizeof(header)))
	{
		MMIAPIFMM_CloseFile(file_handle);

		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Del_Phone_Update_Item, read file failed!");
		return MMIPB_ERROR_ERROR;
	}

	/* check header bit */
	if (GET_BIT(header, index) == 0)
	{
		MMIAPIFMM_CloseFile(file_handle);

		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Del_Phone_Update_Item, index failed!");
		return MMIPB_ERROR_ERROR;
	}

	/* reset header bit */
	CLR_BIT(header, index);
	header.total--;
	
	/* write header */
	MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);
	
    error = MMIAPIFMM_WriteFile(file_handle, &header, sizeof(header), &rw_size, NULL);
    if((SFS_ERROR_NONE != error) || (rw_size != sizeof(header)))
	{
		MMIAPIFMM_CloseFile(file_handle);

		MMIPB_SYNC_NV_Unlock();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Del_Phone_Update_Item, write file failed!");
		return MMIPB_ERROR_ERROR;
	}

	MMIAPIFMM_CloseFile(file_handle);

	MMIPB_SYNC_NV_Unlock();

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Del_Phone_Update_Item OK");
	/* succress */
	return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to delete all phone update item infile                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Del_All_Phone_Update_Item(void)
{
    uint16 path_len = 0;
    MMIFILE_ERROR_E error = SFS_NO_ERROR;
	MMIPB_ERROR_E ret = MMIPB_ERROR_SUCCESS;

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Del_All_Phone_Update_Item");

    path_len = (uint16)MMIAPICOM_Wstrlen(PB_SYNC_DATA_FILE_NAME);

	MMIPB_SYNC_NV_Lock();
	
    /*file not exit*/
    if(TRUE == MMIAPIFMM_IsFileExist(PB_SYNC_DATA_FILE_NAME,path_len))
    {
    	SCI_TRACE_LOW("[MMIPBSYNC], file exist");
		error= MMIAPIFMM_DeleteFile(PB_SYNC_DATA_FILE_NAME, NULL);
		
		if(SFS_ERROR_NONE != error)
		{
			SCI_TRACE_LOW("[MMIPBSYNC] remove file error !");
			ret = MMIPB_ERROR_ERROR;
		}
    }
	else
	{
		SCI_TRACE_LOW("[MMIPBSYNC] file not exist");
	}

	MMIPB_SYNC_NV_Unlock();

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Del_All_Phone_Update_Item,ret = %d", ret);

	return ret;
}

/* Del cause they were defined in mmipb_datalist.c by SPRD */
#if 1
/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Is_HeartBeat_On(void)
{
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
	BOOLEAN value = FALSE;

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Is_HeartBeat_On");
	nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_HEARTBEAT), sizeof(BOOLEAN), (void *)(&value));  
	
	if(ERR_MNDATAMAG_NO_ERR != nv_result)
	{
		value = FALSE;
		MMIPB_SYNC_NV_Set_HeartBeat(value);
	}
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Is_HeartBeat_On, value=%d", value);
	
	return value;
}

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Set_HeartBeat(BOOLEAN value)
{
	BOOLEAN ret = FALSE;
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
	
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Set_HeartBeat,value=%d", value);
	nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_HEARTBEAT), sizeof(BOOLEAN), (void *)(&value));  
	ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Set_HeartBeat, ret=%d", ret);
	
	return ret;
}
#endif

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Is_First_Sync(void)
{
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
	BOOLEAN value = FALSE;

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Is_First_Sync");
	nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_FIRST_SYNC), sizeof(BOOLEAN), (void *)(&value));  
	
	if(ERR_MNDATAMAG_NO_ERR != nv_result)
	{
		value = TRUE;
		MMIPB_SYNC_NV_Set_First_Sync(value);
	}
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Is_First_Sync, value=%d", value);
	
	return value;
}

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Set_First_Sync(BOOLEAN value)
{
	BOOLEAN ret = FALSE;
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
	
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Set_FirstSync,value=%d", value);
	nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_FIRST_SYNC), sizeof(BOOLEAN), (void *)(&value));  
	ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Set_FirstSync, ret=%d", ret);
	
	return ret;
}

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Is_MSA_Sign_In(void)
{
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
	BOOLEAN value = FALSE;

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Is_MSA_Sign_In");
	nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_MSA_SIGN_IN), sizeof(BOOLEAN), (void *)(&value));  
	
	if(ERR_MNDATAMAG_NO_ERR != nv_result)
	{
		value = FALSE;
		MMIPB_SYNC_NV_Set_MSA_Sign_In(value);
	}
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Is_MSA_Sign_In, value=%d", value);
	
	return value;
}

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Set_MSA_Sign_In(BOOLEAN value)
{
	BOOLEAN ret = FALSE;
	ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
	
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Set_MSA_Sign_In,value=%d", value);
	nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(MMINV_PHONEBOOK_MSA_SIGN_IN), sizeof(BOOLEAN), (void *)(&value));  
	ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Set_MSA_Sign_In, ret=%d", ret);
	
	return ret;
}

/*****************************************************************************/
// 	Description : write pb flag to nv
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 向NV中写入flag
/*****************************************************************************/
LOCAL BOOLEAN  WritePbSyncItemToNV(uint32 item_id, uint32  value)
{
    BOOLEAN ret = FALSE;
    ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;   
    
    nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(item_id), sizeof(uint32), (void *)(&value));  
    ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
    return ret;
}

/*****************************************************************************/
// 	Description : read pb flag from nv.
//	Global resource dependence : 
//  Author:Dufei
//  return 
//	Note: 返回NV中存储的制定ID的flag值，比如是否首次进入PB等
/*****************************************************************************/
LOCAL uint32  ReadPbSyncItemFromNV(uint32 item_id, uint32 defalutVal)
{
    ERR_MNDATAMAG_CODE_E  nv_result   = ERR_MNDATAMAG_NO_ERR;
    uint32 value = defalutVal;
    
    nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(item_id), sizeof(uint32), (void *)(&value));  

   if(ERR_MNDATAMAG_NO_ERR != nv_result)
   {
	value = defalutVal;
   }

    return value;
}

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC void MMIPB_SYNC_NV_Reset_SyncTimeInfo(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Reset_SyncTimeInfo");
	WritePbSyncItemToNV(MMINV_PHONEBOOK_HEAVY_SYNCEDTIME, 0);
	WritePbSyncItemToNV(MMINV_PHONEBOOK_LIGHT_SYNCEDTIME, 0);
	WritePbSyncItemToNV(MMINV_PHONEBOOK_HEAVY_LASTCOUNT, 0);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Reset_SyncTimeInfo");
}

/********************************************************************************
** Description: This function is used to read sync time info from NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_NV_Read_SyncTimeInfo(MMIPB_SYNC_TIME_INFO_T *p_SyncTimeInfo)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Read_SyncTimeInfo");
	if(NULL == p_SyncTimeInfo)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Read_SyncTimeInfo, NULL");
		return MMIPB_ERROR_ERROR;
	}

	p_SyncTimeInfo->last_light_sync_time = ReadPbSyncItemFromNV(MMINV_PHONEBOOK_LIGHT_SYNCEDTIME, 0);
	p_SyncTimeInfo->last_heavy_sync_time = ReadPbSyncItemFromNV(MMINV_PHONEBOOK_HEAVY_SYNCEDTIME, 0);
	p_SyncTimeInfo->last_total_count = (uint16)ReadPbSyncItemFromNV(MMINV_PHONEBOOK_HEAVY_LASTCOUNT, 0);
	
	SCI_TRACE_LOW("[MMIPBSYNC] last_light_sync_time = %d", p_SyncTimeInfo->last_light_sync_time);
	SCI_TRACE_LOW("[MMIPBSYNC] last_heavy_sync_time = %d", p_SyncTimeInfo->last_heavy_sync_time);
	SCI_TRACE_LOW("[MMIPBSYNC] last_total_count = %d", p_SyncTimeInfo->last_total_count);

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Read_SyncTimeInfo");
	return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to write sync time info to NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_NV_Write_SyncTimeInfo(MMIPB_SYNC_TIME_INFO_T *p_SyncTimeInfo)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_NV_Write_SyncTimeInfo");
	if(NULL == p_SyncTimeInfo)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Write_SyncTimeInfo, NULL");
		return MMIPB_ERROR_ERROR;
	}

	SCI_TRACE_LOW("[MMIPBSYNC] last_light_sync_time = %d", p_SyncTimeInfo->last_light_sync_time);
	SCI_TRACE_LOW("[MMIPBSYNC] last_heavy_sync_time = %d", p_SyncTimeInfo->last_heavy_sync_time);
	SCI_TRACE_LOW("[MMIPBSYNC] last_total_count = %d", p_SyncTimeInfo->last_total_count);

	if(!WritePbSyncItemToNV(MMINV_PHONEBOOK_LIGHT_SYNCEDTIME, p_SyncTimeInfo->last_light_sync_time))
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Write_SyncTimeInfo, MMINV_PHONEBOOK_LIGHT_SYNCEDTIME");
		return MMIPB_ERROR_ERROR;
	}
	
	if(!WritePbSyncItemToNV(MMINV_PHONEBOOK_HEAVY_SYNCEDTIME, p_SyncTimeInfo->last_heavy_sync_time))
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Write_SyncTimeInfo, MMINV_PHONEBOOK_HEAVY_SYNCEDTIME");
		return MMIPB_ERROR_ERROR;
	}
	
	if(!WritePbSyncItemToNV(MMINV_PHONEBOOK_HEAVY_LASTCOUNT, (uint32)p_SyncTimeInfo->last_total_count))
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Write_SyncTimeInfo, MMINV_PHONEBOOK_HEAVY_LASTCOUNT");
		return MMIPB_ERROR_ERROR;
	}
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_NV_Write_SyncTimeInfo");
	return MMIPB_ERROR_SUCCESS;
}

#endif
#endif
