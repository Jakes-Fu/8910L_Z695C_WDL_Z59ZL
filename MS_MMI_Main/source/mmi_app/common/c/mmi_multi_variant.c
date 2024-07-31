/*************************************************************************
 ** File Name:      mmi_multi_variant.c                                  *
 ** Author:         baokun.yin                                           *
 ** Date:           06/08/2015                                           *
 ** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the interface for multi-variant   *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 06/08/2015     baokun.yin        Create.                             *
*************************************************************************/
#define _MMI_MULTI_VARIANT_C

#include "mn_type.h"
#include "nvitem.h"
#include "mmi_nv.h"
#include "variant_config_nv.h"

#include "mmi_modu_main.h"
#include "mmi_module.h"
#include "mmi_multi_variant.h"
#include "mmi_data.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#define MV_MAGIC_LEN 4
#define NDT_MAX_URL_LEN 128 

#define VOICEMAIL_MAX_RES_COUNT 10 
//Multi-variant header
typedef struct
{
	uint8 magic[MV_MAGIC_LEN];
	uint32 version;
	uint32 item_count;
    uint8  reserved[4];	
}MMI_MV_HEADER_T;

//multi-variant item 
typedef struct
{
	uint32 mcc;
	uint32 mnc;
    MMI_NUMBER_STRING_T ecc[ECC_MAX_COUNT];
	MMI_VOICEMAIL_ITEM_T voicemail[VOICEMAIL_MAX_RES_COUNT];
	MMI_MULTI_VAR_URL_T homepage;
}MMI_MV_ITEM_T;

#ifdef BROWSER_SUPPORT

//bookmark item 
typedef struct 
{
	uint8 name[MMIBROWSER_BOOKMARK_MAX_TITLE_LENGTH];
}BOOKMARK_NAME;

typedef struct 
{
	uint8 url[NDT_MAX_URL_LEN];
}BOOKMARK_URL;

typedef struct
{
    BOOKMARK_NAME name_array[BOOKMARK_MAX_COUNT];
	BOOKMARK_URL  url_array[BOOKMARK_MAX_COUNT];	
}MMI_MV_BOOKMARK_ITEM_T;

BOOKMARK_URL g_url_table[BOOKMARK_MAX_COUNT] = {0};

#endif

#ifdef MULIT_VARIANT_SUPPORT
/*****************************************************************************/
//  Description : read Multi-Variant data form resource
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_MV_ITEM_T* ReadMultiVariantData(BOOLEAN is_orignal_operator)
{
	MN_PLMN_T cur_plmn = {0};
    MMIRES_DATA_INFO_T file_info = {0}; /*lint !e64*/
	BOOLEAN is_res_data_exist = FALSE;
	MMI_MV_HEADER_T *pHeader = PNULL;
    MMI_MV_ITEM_T   *pItem = PNULL;
	uint32 item_count = 0;
	uint32 i = 0;

	is_res_data_exist = MMI_GetDataInfo(R_MULIT_VARIANT, &file_info);
	if(is_res_data_exist)
	{
		if(is_orignal_operator)
		{
			MMIAPIPHONE_GetSimHPLMN(&cur_plmn,MMIAPISET_GetActiveSim());
		}
		else
		{
			MMIAPIPHONE_GetCurPLMN(&cur_plmn,MMIAPISET_GetActiveSim());
		}

		if(cur_plmn.mcc ==0 && cur_plmn.mnc ==0)
		{
			SCI_TRACE_LOW("ReadMultiVariantData invalid Operator code :mcc %d mnc %d mnc_digit_num %d", cur_plmn.mcc, cur_plmn.mnc,cur_plmn.mnc_digit_num);
			return PNULL;
		}
		if(file_info.data_len > sizeof(MMI_MV_HEADER_T))
		{
			pHeader = (MMI_MV_HEADER_T *)file_info.data_ptr;
			if(pHeader != PNULL)
			{
			   pItem = (MMI_MV_ITEM_T *)(file_info.data_ptr + sizeof(MMI_MV_HEADER_T));
               item_count = pHeader->item_count;
			   for(i=0; i < item_count && pItem != PNULL; i++)
			   {
                   if(pItem->mcc == cur_plmn.mcc && pItem->mnc == cur_plmn.mnc)
				   {
					   //SCI_TRACE_LOW("baokun ReadMultiVariantData get pItem 0x%x", pItem);
					   return pItem;
				   }

                   pItem++;
			   }
			}
		}
		
	}	

	return PNULL;
}

/*****************************************************************************/
//  Description : read ECC number from multi-variant 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadECC(MMI_NUMBER_STRING_T *ecc_ptr)
{
	BOOLEAN result = FALSE;
#if 0
    
    if(ecc_ptr != PNULL)
	{
	   MMI_MV_ITEM_T *pItem = ReadMultiVariantData(TRUE);
	   if(pItem != PNULL)
	   {
		    SCI_MEMCPY(ecc_ptr, pItem->ecc, sizeof(pItem->ecc));
			result = TRUE;
	   }
	}
    if(!result && ecc_ptr != PNULL)
	{
		uint16   nv_len = 0;
		//read from NDT
		SCI_MEMSET(ecc_ptr, 0x00, sizeof(MMI_NUMBER_STRING_T)*ECC_MAX_COUNT);	
		if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
		{
			//no sim
            nv_len = NVITEM_GetLength(SETTINGS_NV_DEFAULT_NOSIM_ECC);
			if(nv_len != sizeof(MMI_NUMBER_STRING_T)*ECC_MAX_COUNT)
			{
			   SCI_TRACE_LOW("MMI_MULTIVAR_ReadNOSIMECC read fail !! nv_len %d mmi_len %d", nv_len, sizeof(MMI_NUMBER_STRING_T)*ECC_MAX_COUNT);
			   return FALSE;
			}
			if(NVERR_NONE == EFS_NvitemRead(SETTINGS_NV_DEFAULT_NOSIM_ECC,nv_len , (uint8*)ecc_ptr))
			{
				result = TRUE;
			}
		}
		else
		{
			//exist sim
            nv_len = NVITEM_GetLength(SETTINGS_NV_DEFAULT_SIM_ECC);
			if(nv_len != sizeof(MMI_NUMBER_STRING_T)*ECC_MAX_COUNT)
			{
				SCI_TRACE_LOW("MMI_MULTIVAR_ReadSIMECC read fail !! nv_len %d mmi_len %d", nv_len, sizeof(MMI_NUMBER_STRING_T)*ECC_MAX_COUNT);
				return FALSE;
			}
			if(nv_len > 0 && NVERR_NONE == EFS_NvitemRead(SETTINGS_NV_DEFAULT_SIM_ECC,nv_len, (uint8*)ecc_ptr))
			{
				result = TRUE;
			}
		}
	}
#endif
	return result;
}

/*****************************************************************************/
//  Description : read voice mail number from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadVoiceMail(MMI_VOICEMAIL_ITEM_T *voice_ptr)
{
	BOOLEAN result = FALSE;
    
    if(voice_ptr != PNULL)
	{
	   MMI_MV_ITEM_T *pItem = ReadMultiVariantData(TRUE);
	   if(pItem != PNULL)
	   {
		   SCI_MEMCPY(voice_ptr, pItem->voicemail , sizeof(MMI_VOICEMAIL_ITEM_T)*VOICEMAIL_MAX_COUNT);
		   result = TRUE;
	   }
	}
    if(!result && voice_ptr != PNULL)
	{
		uint16   nv_len = 0;
		//read from NDT
		SCI_MEMSET(voice_ptr, 0x00, sizeof(MMI_VOICEMAIL_ITEM_T)*VOICEMAIL_MAX_COUNT);		
		nv_len = NVITEM_GetLength(SETTINGS_NV_DEFAULT_VOICEMAIL);
		if(nv_len != sizeof(MMI_VOICEMAIL_ITEM_T)*VOICEMAIL_MAX_COUNT)
		{
			SCI_TRACE_LOW("ReadVoiceMail length error ! nv_len %d mmi_len %d", nv_len, sizeof(MMI_VOICEMAIL_ITEM_T)*VOICEMAIL_MAX_COUNT);
			return result;
		}
		if(nv_len >0 &&NVERR_NONE == EFS_NvitemRead(SETTINGS_NV_DEFAULT_VOICEMAIL,nv_len, (uint8*)voice_ptr))
		{
			result = TRUE;
		}
	}
	return result;
	
}

#ifdef BROWSER_SUPPORT


/*****************************************************************************/
//  Description : read home page from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadHomePage(MMI_MULTI_VAR_URL_T *homepage_ptr)
{
	BOOLEAN result = FALSE;

	if(homepage_ptr != PNULL)
	{
		MMI_MV_ITEM_T *pItem = ReadMultiVariantData(TRUE);
		if(pItem != PNULL && (strlen(pItem->homepage.url) > 0))
		{
			SCI_MEMCPY(homepage_ptr, &(pItem->homepage) , sizeof(pItem->homepage));
		    result = TRUE;
		}
	}
    if(!result)
	{
		//read from NDT
		uint16   nv_len = 0;
		nv_len = NVITEM_GetLength(SETTINGS_NV_DEFAULT_HOMEPAGE);
		if(nv_len != sizeof(MMI_MULTI_VAR_URL_T))
		{
            SCI_TRACE_LOW("ReadHomePage length error ! nv_len %d mmi_len %d", nv_len, sizeof(MMI_MULTI_VAR_URL_T));
			return result;
		}
		if(homepage_ptr != PNULL)
		{
		    if(NVERR_NONE == EFS_NvitemRead(SETTINGS_NV_DEFAULT_HOMEPAGE,nv_len , (uint8*)homepage_ptr))
		    {
		     	    result = TRUE;
		    }
		}
	}    
	
	return result;
}

/*****************************************************************************/
//  Description : read current home page from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadCurHomePage(MMI_MULTI_VAR_URL_T *homepage_ptr)
{
	BOOLEAN result = FALSE;
	MMI_MV_ITEM_T *pItem = NULL;
	
	if(homepage_ptr == PNULL)
	{
		return result;
	}
	
	pItem = ReadMultiVariantData(FALSE);
	if(pItem != PNULL && strlen(pItem->homepage.url) > 0)
	{
		SCI_MEMCPY(homepage_ptr, &(pItem->homepage) , sizeof(pItem->homepage));
		result = TRUE;
	}

	if(!result)
	{
		//read from NDT
		uint16   nv_len = 0;
		nv_len = NVITEM_GetLength(SETTINGS_NV_DEFAULT_HOMEPAGE);
		if(nv_len != sizeof(MMI_MULTI_VAR_URL_T))
		{
            SCI_TRACE_LOW("ReadCurHomePage length error ! nv_len %d mmi_len %d", nv_len, sizeof(MMI_MULTI_VAR_URL_T));
			return result;
		}
		if(NVERR_NONE == EFS_NvitemRead(SETTINGS_NV_DEFAULT_HOMEPAGE,nv_len , (uint8*)homepage_ptr))
		{
			result = TRUE;
		}	
	}    
	
	return result;
}

/*****************************************************************************/
//  Description : read bookmark from DNT
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN MMI_DNT_ReadBookMark(MMIRES_BOOKMARK_INFO_T *bookmark_ptr)
{
	MMI_MV_BOOKMARK_ITEM_T *pBK = PNULL;
	BOOLEAN result = FALSE;

	BOOLEAN bSimpChinse =MMIAPISET_CheckLanguageStatus(MMISET_LANGUAGE_SIMP_CHINESE);

	uint16   nv_len = 0;

	nv_len = NVITEM_GetLength(SETTINGS_NV_DEFAULT_BOOKMARK);	 
	
    if(bookmark_ptr != PNULL && nv_len > 0)
	{
		uint32 len = sizeof(MMI_MV_BOOKMARK_ITEM_T);
         pBK = SCI_ALLOC_APPZ(len);
         
		 if(pBK != PNULL)
		 {
			 SCI_TRACE_LOW("MMI_DNT_ReadBookMark read nv start");
			 if(nv_len != sizeof(MMI_MV_BOOKMARK_ITEM_T))
			 {
                  SCI_TRACE_LOW("MMI_DNT_ReadBookMark nv size error ! nv_len %d mmi_len %d", nv_len, sizeof(MMI_MV_BOOKMARK_ITEM_T));
			 }
			if(NVERR_NONE == EFS_NvitemRead(SETTINGS_NV_DEFAULT_BOOKMARK, nv_len, (uint8*)pBK))
			{
				
				uint32 i;
				uint16 booknum=0;				
				//copy to bookmark_ptr
				
				bookmark_ptr->res_item_ptr = SCI_ALLOCA(sizeof(MMIRES_BOOKMARK_ITEM_T) * BOOKMARK_MAX_COUNT);
				if(bookmark_ptr->res_item_ptr != PNULL)
				{
					SCI_MEMSET(bookmark_ptr->res_item_ptr, 0x00, (sizeof(MMIRES_BOOKMARK_ITEM_T) * BOOKMARK_MAX_COUNT));
					
					for(i = 0; i < BOOKMARK_MAX_COUNT; i++)
					{
						if(strlen(&(pBK->name_array[i])) > 0 || strlen(&(pBK->url_array[i])) > 0)
						{
							//name
							
							if(strlen(&(pBK->name_array[i]))> 0)
							{

								bookmark_ptr->res_item_ptr[booknum].name_len = MIN(MMIBROWSER_BOOKMARK_MAX_TITLE_LENGTH, strlen(&(pBK->name_array[i])));							
								bookmark_ptr->res_item_ptr[booknum].name = SCI_ALLOC_APPZ((bookmark_ptr->res_item_ptr[booknum].name_len + 1) * sizeof(wchar));		

#ifdef GBK_SUPPORT
								if(bSimpChinse)
								{
#ifdef GBK_SUPPORT
									GUI_GBKToWstr((wchar *)bookmark_ptr->res_item_ptr[booknum].name,  &(pBK->name_array[i]), (unsigned int)bookmark_ptr->res_item_ptr[booknum].name_len);  
#else
									GUI_GBToWstr((wchar *)bookmark_ptr->res_item_ptr[booknum].name,  &(pBK->name_array[i]), (unsigned int)bookmark_ptr->res_item_ptr[booknum].name_len);  
#endif
								}
								else
#endif
								{
									MMI_STRNTOWSTR(bookmark_ptr->res_item_ptr[booknum].name,
										bookmark_ptr->res_item_ptr[booknum].name_len,
										&(pBK->name_array[i]),
										bookmark_ptr->res_item_ptr[booknum].name_len,
									bookmark_ptr->res_item_ptr[booknum].name_len);
								}


							}

							//url
							/**/
							bookmark_ptr->res_item_ptr[booknum].url_len = MIN(NDT_MAX_URL_LEN, strlen(&(pBK->url_array[i])));
							if(bookmark_ptr->res_item_ptr[booknum].url_len > 0)
							{																
								bookmark_ptr->res_item_ptr[booknum].url = &g_url_table[booknum];
								SCI_MEMSET(bookmark_ptr->res_item_ptr[booknum].url, 0x00, NDT_MAX_URL_LEN);
								SCI_MEMCPY(bookmark_ptr->res_item_ptr[booknum].url, &(pBK->url_array[i]), bookmark_ptr->res_item_ptr[booknum].url_len);
							}

							booknum++;
						}
					} 
					
					bookmark_ptr->res_num =booknum; 
					if(bookmark_ptr->res_num > 0)
					{
						result = TRUE;
					}
					else
					{
                        SCI_FREE(bookmark_ptr->res_item_ptr);
					}
				}               
			}
			SCI_FREE(pBK);
		 }
	}
	return result;
}

#endif

/*****************************************************************************/
//  Description : read bookmark from DNT
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMI_DNT_ReadEmergencyCallUIConfigure(void)
{

    uint8 emergency_config = 0;
    EFS_NvitemRead(APP_NV_EMERGENCY_CALL_UI,sizeof(emergency_config),&emergency_config);
    
    if(emergency_config>3)
    {
        SCI_TRACE_LOW("MMI_DNT_ReadEmergencyCallUIConfigure(): Error. emergency_config[%d]", emergency_config);
        return 0;
    }
    return emergency_config;
}

#else
/*****************************************************************************/
//  Description : read ECC number from multi-variant 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadECC(MMI_NUMBER_STRING_T *ecc_ptr)
{
	return FALSE;
}

/*****************************************************************************/
//  Description : read voice mail number from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadVoiceMail(MMI_VOICEMAIL_ITEM_T *voice_ptr)
{
	return FALSE;	
}

#ifdef BROWSER_SUPPORT

/*****************************************************************************/
//  Description : read home page from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadHomePage(MMI_MULTI_VAR_URL_T *homepage_ptr)
{
	return FALSE;
}

/*****************************************************************************/
//  Description : read current home page from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadCurHomePage(MMI_MULTI_VAR_URL_T *homepage_ptr)
{
	return FALSE;
}

/*****************************************************************************/
//  Description : read bookmark from DNT
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_DNT_ReadBookMark(MMIRES_BOOKMARK_INFO_T *bookmark_ptr)
{
	return FALSE;
}
#endif
/*****************************************************************************/
//  Description : read bookmark from DNT
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMI_DNT_ReadEmergencyCallUIConfigure(void)
{
    return 0;
}

#endif
#ifdef DEBUG_TEST
PUBLIC void MMI_TEST_MULTI_VARINT(void)
{
	SCI_TRACE_LOW("[baokun] Mulit-variant Test begin =================== ");
    //read ecc
    {
        MMI_NUMBER_STRING_T ecc[ECC_MAX_COUNT] = {0};

		if(MMI_MULTIVAR_ReadECC(&ecc))
		{
			int32 i;
			for(i=0; i < ECC_MAX_COUNT; i++)
			{
				SCI_TRACE_LOW("Mulit-variant read ECC: number[%d] = 0x%x 0x%x 0x%x 0x%x", i, ecc[i].number[0],ecc[i].number[1],
					ecc[i].number[2], ecc[i].number[3]);
			}
			

		}
		else
		{
			SCI_TRACE_LOW("Mulit-variant read ECC: failed !");
		}
	}
    //read voicemail
    {
        MMI_VOICEMAIL_ITEM_T vmail[VOICEMAIL_MAX_COUNT] = {0};
		
		if(MMI_MULTIVAR_ReadVoiceMail(&vmail))
		{
			int32 i;
			for(i=0; i < VOICEMAIL_MAX_COUNT; i++)
			{
				SCI_TRACE_LOW("Mulit-variant read voicemail: number[%d] =0x%x 0x%x 0x%x 0x%x", i, vmail[i].viocemail[0], vmail[i].viocemail[1], vmail[i].viocemail[2], vmail[i].viocemail[3]);
			}
			
		}
		else
		{
			SCI_TRACE_LOW("Mulit-variant read vmail: failed !");
		}
	}
	//read homepage
    {
        MMI_MULTI_VAR_URL_T homepage = {0};
		
		if(MMI_MULTIVAR_ReadHomePage(&homepage))
		{
			SCI_TRACE_LOW("Mulit-variant read homepage: homepage = %s", &(homepage.url));
			
		}
		else
		{
			SCI_TRACE_LOW("Mulit-variant read homepage: failed !");
		}
	}
	//readbookmark
    {
		MMIRES_BOOKMARK_INFO_T bookmark ={0};
		if(MMIRES_LoadBookmarkFromRes(&bookmark))
		{
			int32 i;
			for(i < 0; i<bookmark.res_num ; i++)
			{
				
				SCI_TRACE_LOW("Mulit-variant read bookmark[%d] name 0x%x 0x%x 0x%x url %s", i, bookmark.res_item_ptr[i].name[0], bookmark.res_item_ptr[i].name[1],bookmark.res_item_ptr[i].name[2],
					bookmark.res_item_ptr[i].url);
			}
		}
		else
		{
			SCI_TRACE_LOW("Mulit-variant read bookmark: failed !");
		}
	}
	//read ndt bookmark
	{
        MMIRES_BOOKMARK_INFO_T bookmark ={0};
        if(MMI_DNT_ReadBookMark(&bookmark))
		{
			int32 i;
			for(i = 0; i< bookmark.res_num ; i++)
			{
				
				SCI_TRACE_LOW("Mulit-variant read NDTbookmark[%d] name 0x%x 0x%x 0x%x url %s", bookmark.res_item_ptr[i].name[0], bookmark.res_item_ptr[i].name[1],bookmark.res_item_ptr[i].name[2],
					bookmark.res_item_ptr[i].url);
			}
		}
		else
		{
			SCI_TRACE_LOW("Mulit-variant read ndt bookmark: failed !");
		}
	}
	SCI_TRACE_LOW("[baokun] Mulit-variant Test End =================== ");
}
#endif

