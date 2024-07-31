/*****************************************************************************
** File Name:      mmijava_pim.c                                           *
** Author:                                                     *
** Date:           6/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2009                              Create                                *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_sun_java_trc.h"
#include "sci_types.h"
#include "mmipb_adapt.h"
#include "mmipb_export.h"
#include "mmipb_app.h"
#include "mmivcard.h"
#include "mmivcalendar.h"
#include "javacall_defs.h"
#include "javacall_logging.h"
#include "mmijava_pim.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define   MMIJAVA_PIM_OPERATE  0x10
#define   MMIJAVA_PIM_FIND_START_FLAG  (-1)

#define   MMIJAVA_PIM_PHONEBOOK_STORAGE  (PHONEBOOK_NV_STORAGE)
#define   MMIJAVA_PIM_MN_DUAL_SYS  (MN_DUAL_SYS_1)

//pb search handler
LOCAL MMIPB_HANDLE_T s_pb_find_handle = {0};
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*******************************************************************************/
// Description : uuid = storage_id << 16 + storage_id
// Parameter: type [IN] storage_id
//            uuid [IN]storage_id；
// Return : uuid
/*****************************************************************************/
LOCAL int32 vcard_createuuid(uint16 storage_id,
						    uint16 contact_id
						   );

/*******************************************************************************/
// Description :get entry's group and entry id by uuid
// Parameter: storage_id_ptr [OUT]: pointer to group id
//            storage_id_ptr [OUT]: pointer to entry id
//            uuid         [IN] : item's uuid
// Return : NONE
/*****************************************************************************/
LOCAL void vcard_parseuuid(uint16 *storage_id_ptr,
						   uint16 *contact_id_ptr,
						   int32 uuid);

/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*******************************************************************************/
// Description :获得下一个vCard/vCalendar条目
// Parameter: type      [IN] 0代表vCard, 1代表Event，2代表Todo；
//            lastItemId[IN] 上次读到条目的uuid, -1表示从头开始读取；
//            uuid_ptr      [OUT] vCard/vCalendar条目的uuid；
//            item_ptr      [IN/OUT]用于保存UTF-8编码的vCard/vCalendar条目数据，编码
//                           符合vCard2.1/vCalendar 1.0规范的规定。item的存储空间由Java分配；
//            maxItemLen 【IN】 Java分配的item存储空间的大小；
//
// Return : 0代表成功; -1代表出错，或者已经读到vCard/vCalendar的末尾，本次读取没有获
//          得vCard/vCalendar条目数据。
/*****************************************************************************/
PUBLIC int32 MMIJAVA_PIM_Get_Next_Item(MMIJAVA_PIM_TYPE_E type,
									   int32             lastItemId,
									   int32             *uuid_ptr, 
									   uint8              *item_ptr,
									   uint32             maxItemLen
									   )
{
	int32 ret = -1;
    uint16 storage_id = 0;
	uint16 contact_id = 0;
    MMIPB_CONTACT_T  *entry_ptr = PNULL;
	
	PIM_DEBUG(LOG_STR_ENTER("type = %d , lastItemId = 0x%x"), type, lastItemId);
	
	switch(type) 
	{
	case MMIJAVA_PIM_VCARD:    
		entry_ptr = (MMIPB_CONTACT_T  *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
		SCI_MEMSET(entry_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
		
		if(MMIJAVA_PIM_FIND_START_FLAG != lastItemId )
		{
		    if( 0 == s_pb_find_handle)
		    {
                PIM_DEBUG(LOG_STR("find next fail, s_pb_find_handle = 0"));
                goto GET_ITEM_ERROR_LABEL;
		    }
		    //FIND NEXT
		   ret = MMIAPIPB_FindNextContact(s_pb_find_handle , &contact_id, &storage_id);
		        
		   if(MMIPB_ERROR_SUCCESS != ret)
		   {
               PIM_DEBUG(LOG_STR("MMIAPIPB_FindNextContact error = %d"), ret);
               goto GET_ITEM_ERROR_LABEL;
		   }		    
		}
		else
		{
		    if( 0 != s_pb_find_handle)
		    {
                PIM_DEBUG(LOG_STR("find firstly, s_pb_find_handle != 0"));
		        MMIAPIPB_FindClose(s_pb_find_handle);
		    }
		    //find first
            ret = MMIAPIPB_FindFirstContact(&contact_id, &storage_id, &s_pb_find_handle);
            if(MMIPB_ERROR_SUCCESS != ret)
            {
                PIM_DEBUG(LOG_STR("MMIAPIPB_FindFirstContact error = %d"), ret);
                goto GET_ITEM_ERROR_LABEL;
            }            
		}
		
		//get pb entry	
		ret = MMIAPIPB_GetContactInfo(contact_id,  storage_id, entry_ptr);
		if(MMIPB_ERROR_SUCCESS != ret)
		{
            PIM_DEBUG(LOG_STR("MMIAPIPB_GetContactInfo error = %d"), ret);
            goto GET_ITEM_ERROR_LABEL;
		}
        #ifdef MMI_VCARD_SUPPORT 
		ret = MMIAPIPB_ContactToVcard(entry_ptr,maxItemLen, item_ptr, &maxItemLen);
		if(MMIPB_ERROR_SUCCESS != ret)
		{
            PIM_DEBUG(LOG_STR("MMIAPIPB_ContactToVcard error = %d"), ret);
            goto GET_ITEM_ERROR_LABEL;
		}	
        #endif
		//create uuid by group id and entry id
        *uuid_ptr = vcard_createuuid(entry_ptr->storage_id, entry_ptr->contact_id);		
        ret = MMIJAVA_PIM_SUCCESS;
		SCI_FREE(entry_ptr);	
		break;
GET_ITEM_ERROR_LABEL:
        ret = MMIJAVA_PIM_FAILE;
        MMIAPIPB_FindClose( s_pb_find_handle);
		SCI_FREE(entry_ptr);		
		break;
	case MMIJAVA_PIM_EVENT:
		ret=MMIVCAL_GetNextVcalItem(MMIVCAL_OBJECT_TYPE_VEVENT,lastItemId,uuid_ptr,item_ptr,maxItemLen);
        break;
	case MMIJAVA_PIM_TODO:
		ret=MMIVCAL_GetNextVcalItem(MMIVCAL_OBJECT_TYPE_VTODO,lastItemId,uuid_ptr,item_ptr,maxItemLen);
        break;
	default:
		break;
	}
	return ret;
}

/*******************************************************************************/
// Description :修改一个vCard/vCalendar条目
// Parameter: type [IN] 0代表vCard, 1代表Event，2代表Todo
//            uuid [IN]vCard/vCalendar条目的uuid；
//            item_ptr[IN/OUT]用于保存UTF-8编码的vCard/vCalendar条目数据，编码
//                           符合vCard2.1/vCalendar 1.0规范的规定。item的存储空间由Java分配；
//
// Return : 0代表成功; -1代表出错。
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Modify_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 uuid, 
									  uint8 *item_ptr
									  )
{
	int32 ret = -1;
    uint16 storage_id = 0;
	uint16 contact_id = 0;
    MMIPB_CONTACT_T  *entry_ptr = PNULL;
	
	//SCI_TRACE_LOW:"[JAVA PIM] MMIJAVA_PIM_Modify_Item type %d uuid 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_PIM_175_112_2_18_3_27_18_18,(uint8*)"dd", type, uuid);

	switch(type) 
	{
#ifdef MMI_VCARD_SUPPORT       
	case MMIJAVA_PIM_VCARD:
		entry_ptr = (MMIPB_CONTACT_T  *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
		SCI_MEMSET(entry_ptr, 0x00, sizeof(MMIPB_CONTACT_T));

		//convert vcard to pb entry		
		ret = MMIAPIPB_VcardToContact(item_ptr, strlen((char *)item_ptr), entry_ptr);
		if(MMIPB_ERROR_SUCCESS != ret)
		{
            PIM_DEBUG(LOG_STR("MMIAPIPB_VcardToContact error = %d"), ret);
            goto MODIFY_ITEM_ERROR_LABEL;
		}	
		
		//parse group id and entry id from uuid
		vcard_parseuuid(&storage_id, &contact_id, uuid);
        entry_ptr->storage_id = storage_id;
        entry_ptr->contact_id = contact_id;
		ret = MMIAPIPB_ModifyContact(entry_ptr)	;	
        if(MMIPB_ERROR_SUCCESS != ret)
        {
            PIM_DEBUG(LOG_STR("MMIAPIPB_ModifyContact error = %d"), ret);
            goto MODIFY_ITEM_ERROR_LABEL;
        }   
        
        ret = MMIJAVA_PIM_SUCCESS;
		SCI_FREE(entry_ptr);
		break;
#endif        
MODIFY_ITEM_ERROR_LABEL:
        ret = MMIJAVA_PIM_FAILE;
		SCI_FREE(entry_ptr);
		break;
	case MMIJAVA_PIM_EVENT:
		ret =MMIVCAL_ModifyVcalItem(item_ptr,SCI_STRLEN((char*)item_ptr),uuid);
		break;
	case MMIJAVA_PIM_TODO:
		ret =MMIVCAL_ModifyVcalItem(item_ptr,SCI_STRLEN((char*)item_ptr),uuid);
		break;
	default:
		break;
	}

	return ret;
}

/*******************************************************************************/
// Description :添加一个vCard/vCalendar条目
// Parameter: type [IN] 0代表vCard, 1代表Event，2代表Todo
//            uuid_ptr [OUT]vCard/vCalendar条目的uuid；
//            item_ptr[IN/OUT]用于保存UTF-8编码的vCard/vCalendar条目数据，编码
//                           符合vCard2.1/vCalendar 1.0规范的规定。item的存储空间由Java分配；
//
// Return : 0代表成功; -1代表出错。
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Add_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 *uuid_ptr, 
									  uint8 *item_ptr
									  )
{
	int32 ret = -1;
    MMIPB_CONTACT_T  *entry_ptr = PNULL;
	uint16 storage_id = 0;
	uint16 contact_id = 0;
	//SCI_TRACE_LOW:"[JAVA PIM] MMIJAVA_PIM_Add_Item type %d "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_PIM_238_112_2_18_3_27_19_19,(uint8*)"d", type);
	switch(type) 
	{
#ifdef MMI_VCARD_SUPPORT       
	case MMIJAVA_PIM_VCARD:
		entry_ptr = (MMIPB_CONTACT_T  *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
		SCI_MEMSET(entry_ptr, 0x00, sizeof(MMIPB_CONTACT_T));

		//convert vcard to pb entry		
		ret = MMIAPIPB_VcardToContact(item_ptr, strlen((char *)item_ptr), entry_ptr);
		if(MMIPB_ERROR_SUCCESS != ret)
		{
            PIM_DEBUG(LOG_STR("MMIAPIPB_VcardToContact error = %d"), ret);
            goto ADD_ITEM_ERROR_LABEL;
		}	
		
        entry_ptr->storage_id = MMIAPIPB_GetStorageId( MMIJAVA_PIM_MN_DUAL_SYS,  MMIJAVA_PIM_PHONEBOOK_STORAGE);        
        entry_ptr->contact_id = MMIAPIPB_GetFreeContactId(entry_ptr->storage_id);
        
		ret = MMIAPIPB_AddContact(entry_ptr);
		if(MMIPB_ERROR_SUCCESS != ret)
		{
            PIM_DEBUG(LOG_STR("MMIAPIPB_AddContact error = %d"), ret);
            goto ADD_ITEM_ERROR_LABEL;
		}			
    	 *uuid_ptr = vcard_createuuid(entry_ptr->storage_id, entry_ptr->contact_id);    	 
         ret = MMIJAVA_PIM_SUCCESS;
         SCI_FREE(entry_ptr);
         break;
#endif         
ADD_ITEM_ERROR_LABEL:
        ret = MMIJAVA_PIM_FAILE;
		SCI_FREE(entry_ptr);
		break;
	case MMIJAVA_PIM_EVENT:
		ret =MMIVCAL_AddVcalItem(item_ptr,SCI_STRLEN((char*)item_ptr),uuid_ptr);
		break;
	case MMIJAVA_PIM_TODO:
		ret =MMIVCAL_AddVcalItem(item_ptr,SCI_STRLEN((char*)item_ptr),uuid_ptr);
		break;
	default:
		break;
	}
	return ret;
}

/*******************************************************************************/
// Description :删除一个vCard/vCalendar条目
// Parameter: type [IN] 0代表vCard, 1代表Event，2代表Todo
//            uuid [IN]vCard/vCalendar条目的uuid；
// Return : 0代表成功; -1代表出错。
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Remove_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 uuid
									  )
{
	int32 ret = -1;
    uint16 storage_id = 0;
	uint16 contact_id = 0;
	//SCI_TRACE_LOW:"[JAVA PIM] MMIJAVA_PIM_Remove_Item type %d uuid 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_PIM_293_112_2_18_3_27_19_20,(uint8*)"dd", type, uuid);
	
	switch(type) 
	{
	case MMIJAVA_PIM_VCARD:
		vcard_parseuuid(&storage_id, &contact_id, uuid);
		ret = MMIAPIPB_DeleteContact(contact_id ,  storage_id);
		if(MMIPB_ERROR_SUCCESS != ret)
		{
            PIM_DEBUG(LOG_STR("MMIAPIPB_DeleteContact error = %d"), ret);
            ret = MMIJAVA_PIM_FAILE;
            break;
		}			
        ret = MMIJAVA_PIM_SUCCESS;
		break;
	case MMIJAVA_PIM_EVENT:
		ret =MMIVCAL_RemoveVcalItem(uuid);
		break;
	case MMIJAVA_PIM_TODO:
		ret =MMIVCAL_RemoveVcalItem(uuid);
		break;
	default:
		break;
	}
	return ret;
}

/*******************************************************************************/
// Description :get entry's group and entry id by uuid
// Parameter: storage_id_ptr [OUT]: pointer to group id
//            storage_id_ptr [OUT]: pointer to entry id
//            uuid         [IN] : item's uuid
// Return : NONE
/*****************************************************************************/
LOCAL void vcard_parseuuid(uint16 *storage_id_ptr,
						   uint16 *contact_id_ptr,
						   int32 uuid)
{
	 if(PNULL == storage_id_ptr || PNULL == storage_id_ptr)
	 {
		 //SCI_TRACE_LOW:"[JAVA PIM] vcard_parseuuid parameter error !"
		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_PIM_331_112_2_18_3_27_19_21,(uint8*)"");
		 return;
	 }
     if(-1 == uuid)
	 {
		 //start from head
		 *storage_id_ptr = 0;
		 *contact_id_ptr = 0;
	 }
	 else
	 {
		 *storage_id_ptr =(uint16)(uuid >> 0x10);
		 *contact_id_ptr = (uint16)(uuid & 0xFFFF);
	 }
     //SCI_TRACE_LOW:"[JAVA PIM] vcard_parseuuid gropu id 0x%x storage_id 0x%x uuid 0x%x"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_PIM_345_112_2_18_3_27_19_22,(uint8*)"ddd", *storage_id_ptr,*storage_id_ptr, uuid);
}

/*******************************************************************************/
// Description : uuid = storage_id << 16 + storage_id
// Parameter: type [IN] storage_id
//            uuid [IN]storage_id；
// Return : uuid
/*****************************************************************************/
LOCAL int32 vcard_createuuid(uint16 storage_id,
						    uint16 contact_id
						   )
{
	 int32  uuid = 0;
	 uuid = storage_id << 0x10;
     uuid += contact_id;
     //SCI_TRACE_LOW:"[JAVA PIM] vcard_createuuid gropu id 0x%x storage_id 0x%x uuid 0x%x"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIJAVA_PIM_359_112_2_18_3_27_19_23,(uint8*)"ddd", storage_id,storage_id, uuid);
	 return uuid;
}

