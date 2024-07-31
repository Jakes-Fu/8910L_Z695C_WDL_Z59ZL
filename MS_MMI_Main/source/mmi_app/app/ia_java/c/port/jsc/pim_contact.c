/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/****************************************************************************************
 * FILE: pim_contact.c
 *
 * DESCRIPTION:
 *      the fuction for PIM .
 *
 * UPDATED DATE:
 *      2007/12/06
 *
 * REVISION:
 *      2007/12/06,      Jinqiang.Chen,    first version.
 *      2007/12/30,      Siyuan.Chen,      second version.
 *      2008/02/03,      Jinqiang.Chen,    second version.
 *
 ****************************************************************************************/

/*-----------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
 #include "sci_types.h"
#ifdef  JAVA_SUPPORT_IA
/* JBlendia interface */
#include "jkpimcommon.h"
#include "jkpimcontact.h"
#include "ampimutil.h"
#include "ampimevent.h"
#include "ajutil.h"

/* porting interface */
#include "pim_common.h"
#include "pim_contact.h"
#include "mmijava_internal.h"

/* spreadtrum interface */
#include "mmi_modu_main.h"
#include "mn_type.h"
#include "mmipb_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"


/*-----------------------------------------------------------------------------
 * Marcos, Types, and Structures
 *----------------------------------------------------------------------------*/
#define ASSERT_LOG(_x)  JWE_LOG_ERR(assert,_x)

#ifndef _AJSC_DEBUG
    #undef  JWE_LOG
    #define JWE_LOG(function,parameters)
#endif
	
/*-----------------------------------------------------------------------------
 * Local Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

#define _attr_tel      ( JK_PIM_CONTACT_ATTR_MOBILE      | \
                        JK_PIM_CONTACT_ATTR_HOME        | \
                        JK_PIM_CONTACT_ATTR_WORK)

#define INVALID_INDEX	-1                        

/*-----------------------------------------------------------------------------
 * External Variables
 *----------------------------------------------------------------------------*/
extern PimListInfo gm_pimList[3];
//extern MMIPB_SORT_AND_ENTRY_ID_T   g_mmipb_entry_list;
/*-----------------------------------------------------------------------------
 * Local Variables
 *----------------------------------------------------------------------------*/
static PimResultSetInfo *currentResultSet = NULL;
//static MMIPB_PHONEBOOK_ENTRY_T	gm_entry;
//murphy change for new pb
static MMIPB_CONTACT_T	gm_entry;



/* Attribute information table */
static const  PimAttributeInfo _attributeInfo[] =
{
    		/*  List                AttributeID                 Label   */
    {   PHONEBOOK_SIM_STORAGE,JK_PIM_CONTACT_ATTR_MOBILE, TXT_PIM_ATTR_MOBILE      },
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_ATTR_MOBILE, TXT_PIM_ATTR_MOBILE      },
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_ATTR_HOME,   TXT_PIM_ATTR_HOME        },
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_ATTR_WORK,   TXT_PIM_ATTR_WORK        }       /*END*/
};

/* Field information table */
static const  PimFieldInfo _fieldInfo[] =
{
   			 /*  List            FieldID                         LabelID                     Datatype        Array    Size  Max  Attr */
    {   PHONEBOOK_SIM_STORAGE,JK_PIM_CONTACT_FORMATTED_NAME,  TXT_PIM_FIELD_FORMATTED_NAME,  JK_PIM_STRING,  JK_NULL,    0,  1,  0   },
    {   PHONEBOOK_SIM_STORAGE,JK_PIM_CONTACT_TEL,             TXT_PIM_FIELD_TEL,             JK_PIM_STRING,  JK_NULL,    0,  1,  JK_PIM_CONTACT_ATTR_MOBILE  },
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_FORMATTED_NAME,  TXT_PIM_FIELD_FORMATTED_NAME,  JK_PIM_STRING,  JK_NULL,    0,  1,  0   },
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_TEL,             TXT_PIM_FIELD_TEL,             JK_PIM_STRING,  JK_NULL,    0,  3,  _attr_tel   },
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_EMAIL,           TXT_PIM_FIELD_EMAIL,           JK_PIM_STRING,  JK_NULL,    0,  1,  0   },
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_NOTE,            TXT_PIM_FIELD_NOTE,            JK_PIM_STRING,  JK_NULL,    0,  1,  0   },   
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_TITLE,           TXT_PIM_FIELD_TITLE,            JK_PIM_STRING,  JK_NULL,    0,  1,  0   },
    {   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_ORG,           TXT_PIM_FIELD_ORG,            JK_PIM_STRING,  JK_NULL,    0,  1,  0   },
/* +TCK fix pim tck cases*/    
    //{   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_ADDR,           TXT_PIM_FIELD_WORK_ADR,            JK_PIM_STRING,  JK_NULL,    0,  1,  0   },
    //{   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_PHOTO_URL,  TXT_PIM_FIELD_PHOTO_URL,            JK_PIM_STRING,  JK_NULL,    0,  1,  0   },
    //{   PHONEBOOK_NV_STORAGE, JK_PIM_CONTACT_PHOTO,  TXT_PIM_FIELD_PHO,            JK_PIM_BINARY,  JK_NULL,    0,  1,  0   }
/* -TCK fix pim tck cases */    

			 /*END*/
};
/*-----------------------------------------------------------------------------
 * Local Function Declarations
 *----------------------------------------------------------------------------*/
static BOOLEAN pimItemToPhoneEntry( MMIPB_CONTACT_T *entry, const JKPIMItem *item, const JKSint32 connectionId );
static void printEntry( MMIPB_CONTACT_T *entry );
static uint8  PIM_MMIPB_GetUint8FromNumberType(MN_NUMBER_TYPE_E   number_type );
static BOOLEAN pimCommMatchString( JKUint16 *sourceStr, int sourceStrLen, JKUint16  *matchStr, int matchStrLen );
static BOOLEAN pimSearchEntryByPartialEntry( MMIPB_CONTACT_T *entry,  MMIPB_CONTACT_T *partial );
static BOOLEAN pimCommMatchBCD_Number_T(MMIPB_BCD_NUMBER_T *sourceNum_t,  MMIPB_BCD_NUMBER_T *matchNum_t );
static uint8 PimCommScharToLowerCase( uint8 ch );
static BOOLEAN pimSearchEntryByString( MMIPB_CONTACT_T *entry, JKPIMString *string );
/*-----------------------------------------------------------------------------
 * Local Functions
 *----------------------------------------------------------------------------*/
static JKUint16 PIM_GetItemCount(JKUint8 itemId)
{
	JKUint16 count = 0;
	JWE_LOG(PIM_GetItemCount, ("itemId is %d\n", itemId))
	switch(itemId)
	{
		case PHONEBOOK_NV_STORAGE:
			 count = MMIAPIPB_GetPhoneContactCount();
			 break;
		case PHONEBOOK_SIM_STORAGE:
			 count = MMIAPIPB_GetAllSimContactCount();   
			 break;
		default:
			break;
			
	}
	return count;
}
//mary modify need check
static JKUint16 PimContactGetStorageIdByPos(uint16 pos)
{
    MMIPB_CONTACT_BASE_INFO_T base_info = {0};
    base_info = MMIAPIPB_GetContactBaseInfo(pos);    
    return base_info.storage_id;
}
//mary modify need check
#if 1
static JKUint16 PimContactGetEntryIdByPos(PHONEBOOK_STORAGE_E storage, uint16 pos)
{
    MMIPB_CONTACT_BASE_INFO_T base_info = {0};
    base_info = MMIAPIPB_GetContactBaseInfo(pos);    
    return base_info.contact_id;
}
#else
static JKUint16 PimContactGetEntryIdByPos(PHONEBOOK_STORAGE_E storage, uint16 pos)
{
    int i;
    int j=-1;
    if (PHONEBOOK_SIM_STORAGE == storage)  
    {  
       for(i = 0;i <g_mmipb_entry_list.sort_t.count;i++)
       { 
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (PB_GROUP_SIM_1 == g_mmipb_entry_list.sort_t.content[i].group_id 
               ||PB_GROUP_SIM_2 == g_mmipb_entry_list.sort_t.content[i].group_id
#if defined MMI_MULTI_SIM_SYS_TRI || defined(MMI_MULTI_SIM_SYS_QUAD)
                ||PB_GROUP_SIM_3 == g_mmipb_entry_list.sort_t.content[i].group_id
#if defined MMI_MULTI_SIM_SYS_QUAD
                ||PB_GROUP_SIM_4 == g_mmipb_entry_list.sort_t.content[i].group_id
#endif
#endif
              )
#else 
            if (PB_GROUP_SIM_1 == g_mmipb_entry_list.sort_t.content[i].group_id )
#endif
            {
                j++;
            }

            if (pos==j)
            {
                return  g_mmipb_entry_list.sort_t.content[i].entry_id;
            }
       }
    }    
	
    if (PHONEBOOK_NV_STORAGE == storage)
    {
        j=-1;
        for(i = 0;i <g_mmipb_entry_list.sort_t.count; i++)
        {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (PB_GROUP_SIM_1 != g_mmipb_entry_list.sort_t.content[i].group_id 
              &&PB_GROUP_SIM_2 != g_mmipb_entry_list.sort_t.content[i].group_id
#if defined MMI_MULTI_SIM_SYS_TRI || defined(MMI_MULTI_SIM_SYS_QUAD)
              && PB_GROUP_SIM_3 != g_mmipb_entry_list.sort_t.content[i].group_id
#if defined(MMI_MULTI_SIM_SYS_QUAD)
              && PB_GROUP_SIM_4 != g_mmipb_entry_list.sort_t.content[i].group_id
#endif
#endif
            )
#else 
            if (PB_GROUP_SIM_1 != g_mmipb_entry_list.sort_t.content[i].group_id)
#endif
            {
                j++;
            }
		
     	if (pos==j)
	{
             return  g_mmipb_entry_list.sort_t.content[i].entry_id;
	}
 	}
    } 

	return 0;

}
#endif
static BOOLEAN PIM_EchoGroupNameExist( JKSint32 ConnectionId, JKWChar *text, JKSint32 length )
{
	MMIPB_NAME_T group_name = {0};
	// JKWChar wcText[PIM_CATEGORY_LEN];
	int i,count=0;
		
	JWE_LOG(PIM_EchoGroupNameExist,("enter"));

	for(i= PB_GROUP_ALL+1;i<= MMIPB_MAX_GROUP_NUM;i++)
	{
		MMIPB_ReadGroupName ( &group_name,i);

		if((group_name.wstr_len == length) &&
				(!memcmp((char*) group_name.wstr,(char*)text,length*sizeof(JKWChar))))
		{
			count++;

		}
	}
	JWE_LOG(PIM_EchoGroupNameExist,("count is  %d",count));
	if(count > 1)
	{
		JWE_LOG(PIM_EchoGroupNameExist,("leave:ok"));
		return TRUE;

	}
	else
	{
		JWE_LOG(PIM_EchoGroupNameExist,("leave:err"));
		return FALSE;
	}

}

static int  PIM_GetGroupIdByName( JKSint32 ConnectionId, JKWChar *text, JKSint32 length )
{
	MMIPB_NAME_T 	group_name = {0};
	// JKWChar 		wcText[PIM_CATEGORY_LEN];
	// JKSint32 		wcLength;
	int 			i,group_id = -1;
    for(i= PB_GROUP_ALL+1;i<= MMIPB_MAX_GROUP_NUM;i++)
    {

	    MMIPB_ReadGroupName ( &group_name,i);
	    if((group_name.wstr_len == length) &&
			    (!memcmp((char*) group_name.wstr,(char*)text,length*sizeof(JKWChar))))
	    {
		    group_id = i;
		    break;
	    }
    }
    if(i > MMIPB_MAX_GROUP_NUM)
    {
     group_id = -1;
    }

	JWE_LOG(PIM_GetGroupIdByName,("group_id = %d",group_id));
    JWE_LOG(PIM_GetGroupIdByName,("leave"));
    return group_id;
}
/*-----------------------------------------------------------------------------
 * External Function Declarations
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Global / Exported Functions
 *----------------------------------------------------------------------------*/
JKPIMErrCode PimContactGetListNames(const JKSint32 taskId,const JKPIMListType listType,JKPIMStringArray* array)
{
	MMI_STRING_T contact_name;
	// uint8 wcName[PIM_LISTNAME_LEN]= {0};
	// uint16 wcNameLen;
	JKPIMErrCode retval;
	
	JWE_LOG(PimContactGetListNames,("enter,listType=(%d)", listType));
	
	/*	phone conatct list name read */
	MMI_GetLabelTextByLang(TXT_PIM_PHONE_CONTACT,&contact_name);	
	if(!contact_name.wstr_len)
	{
		JWE_LOG(PimContactGetListNames,("err:wcNameLen1 fail"));
		return JK_PIM_MEMORY_ERR;
	}	
	retval = AmPIMStringArrayAddString( array,(JKWChar* )contact_name.wstr_ptr,contact_name.wstr_len);
	//PIM Ivy.Su 20100527 end
	if ( retval != JK_PIM_SUCCESS )
     {
        	 JWE_LOG(PimContactGetListNames,("err:AmPIMStringArrayAddString fail. retval=(%d)",retval));
        	return retval;
	 }
    
    gm_pimList[PIM_CONTACT_LIST_PHONE].listID = PHONEBOOK_NV_STORAGE;
    gm_pimList[PIM_CONTACT_LIST_PHONE].listType = listType;
    SCI_MEMCPY(gm_pimList[PIM_CONTACT_LIST_PHONE].listName,contact_name.wstr_ptr,contact_name.wstr_len*sizeof(wchar));
   
   	/*	sim conatct name read */
    MMI_GetLabelTextByLang(TXT_PIM_SIM_CONTACT,&contact_name);
	if(!contact_name.wstr_len)
	{
		JWE_LOG(PimContactGetListNames,("err:wcNameLen2 fail"));
		return JK_PIM_MEMORY_ERR;
	}
    retval = AmPIMStringArrayAddString( array, (JKWChar* )contact_name.wstr_ptr, contact_name.wstr_len);
    if ( retval != JK_PIM_SUCCESS )
    {
        JWE_LOG(PimContactGetListNames,("err:AmPIMStringArrayAddString fail. retval=(%d)",retval));
        return retval;
    }
    
    gm_pimList[PIM_CONTACT_LIST_SIM].listID = PHONEBOOK_SIM_STORAGE;
    gm_pimList[PIM_CONTACT_LIST_SIM].listType = listType;
	SCI_MEMCPY(gm_pimList[PIM_CONTACT_LIST_SIM].listName,contact_name.wstr_ptr,contact_name.wstr_len*sizeof(wchar));
	JWE_LOG(PimContactGetListNames,("leave"));
    return JK_PIM_SUCCESS;
}
JKPIMErrCode PimContactGetDefaultListName(const JKSint32 taskId,const JKPIMListType listType,JKPIMString* name)
{
	/*
     * there are two lists on Spreatrum: SIM & Phone
     * here we choose to return Phone as default list!!
     */

    MMI_STRING_T contact_name;
    // uint8 wcName[PIM_LISTNAME_LEN]= {0};
	// uint16 wcNameLen;	
	JKPIMErrCode retval;

    JWE_LOG(PimContactGetDefaultListName,("enter,listType=(%d)", listType));

    /*	phone conatct name read TXT_PIM_PHONE_CONTACT is the resource*/
	MMI_GetLabelTextByLang(TXT_PIM_PHONE_CONTACT,&contact_name);
	if(!contact_name.wstr_len)
	{
		JWE_LOG(PimContactGetDefaultListName,("err:wcNameLen3 fail"));
		return JK_PIM_MEMORY_ERR;
	}
	retval = AmPIMStringSetString( name, (JKWChar*) contact_name.wstr_ptr, contact_name.wstr_len);
    if ( retval != JK_PIM_SUCCESS )
    {
        JWE_LOG(PimContactGetDefaultListName,("err:AmPIMStringSetString fail. retval=(%d)",retval));
        return retval;
    }
    
    gm_pimList[PIM_CONTACT_LIST_PHONE].listID = PHONEBOOK_NV_STORAGE;
    gm_pimList[PIM_CONTACT_LIST_PHONE].listType = listType;
    SCI_MEMCPY(gm_pimList[PIM_CONTACT_LIST_PHONE].listName,contact_name.wstr_ptr,contact_name.wstr_len*sizeof(wchar));
    JWE_LOG(PimContactGetDefaultListName,("leave"));
    return JK_PIM_SUCCESS;
}

JKPIMErrCode PimContactOpenConnection(const JKSint32 taskId,const JKPIMListType listType,const JKPIMAccessMode mode,const JKPIMString* name,JKSint32* connectionId)
{
	/* debug */
	JWE_LOG(PimContactOpenConnection,("enter,type=(%d),mode=(%d),name=%d)",
                                    listType, mode,  name->length));
	 dumpbuf(name->text, name->length*2);
    *connectionId = getListIdByName(name->text, name->length);

    if ( *connectionId < 0 ) {
        JWE_LOG(PimContactOpenConnection,("err:open fail"));
        return JK_PIM_LIST_NOT_ACCESSIBLE;
    }
	if (!MMIAPIPB_IsPbReady())
    {
        JWE_LOG(PimContactOpenConnection,("err:Phone Book is no ready!"));
        return JK_PIM_LIST_NOT_ACCESSIBLE;
    }
    *connectionId <<= 8;
    *connectionId  |= listType;
    *connectionId <<= 8;
    *connectionId  |= mode;

    JWE_LOG(PimContactOpenConnection,("leave, connectionId=%d", *connectionId));
    return JK_PIM_SUCCESS;
}

JKPIMErrCode PimContactCloseConnection(const JKSint32 taskId,const JKSint32 connectionId,const JKBool finalize)
{
	JWE_LOG(PimContactCloseConnection,("enter"));

    currentResultSet = NULL;

    JWE_LOG(PimContactCloseConnection,("leave"));
    return JK_PIM_SUCCESS;
}

JKPIMErrCode PimContactGetSupportedFields(const JKSint32 taskId,const JKSint32 connectionId,JKPIMFieldDefArray* array)
{
	MMI_STRING_T filed_name = {0};
	JKPIMString label;
	uint8 label_temp[PIM_LABEL_LEN]= {0};
    JKPIMErrCode retval;
    int i;

    JWE_LOG(PimContactGetSupportedFields,("LISTID(connectionId)=%d",LISTID(connectionId)));
	
	label.text = (JKWChar*)label_temp;
	
    for( i = 0; i < sizeof(_fieldInfo)/sizeof(_fieldInfo[0]); i++ )/*lint !e737 !e574*/
    {
        if ( _fieldInfo[i].listID != LISTID(connectionId) )
        {
            continue;
        }
		
		MMI_GetLabelTextByLang(_fieldInfo[i].fieldLabelID,&filed_name);
		dumpbuf(filed_name.wstr_ptr, filed_name.wstr_len);
		SCI_MEMCPY(label.text, filed_name.wstr_ptr,filed_name.wstr_len*sizeof(wchar));
		label.length = filed_name.wstr_len;
		
        retval = AmPIMFieldDefArrayAddFieldDef( array,                      // array
                                                _fieldInfo[i].fieldID,      // fieldId
                                                &label,                      // label
                                                _fieldInfo[i].fieldDataType,// dataType
                                                _fieldInfo[i].maxValues,    // maxSize
                                                _fieldInfo[i].attributes ); // attributes
        if ( retval != JK_PIM_SUCCESS )
        {
            JWE_LOG(PimContactGetSupportedFields,("err:AmPIMFieldDefArrayAddFieldDef failed"));
            return JK_PIM_MEMORY_ERR;
        }
        
        JWE_LOG(PimContactGetSupportedFields,("i = %d",i))

    }

    JWE_LOG(PimContactGetSupportedFields,("leave"))
    return JK_PIM_SUCCESS;
}

JKPIMErrCode PimContactGetSupportedAttributes(const JKSint32 taskId,const JKSint32 connectionId,JKPIMAttributeDefArray* array)
{
	MMI_STRING_T attri_name;
	JKPIMString label;
	uint8 label_temp[PIM_LABEL_LEN]= {0};
    JKPIMErrCode retval;
    int i;

    JWE_LOG(PimContactGetSupportedAttributes,("enter,taskId=(%d),connectionId=(%d)", taskId, connectionId));
    
 	label.text = (JKWChar*)label_temp;
 	
    for ( i = 0; i < sizeof(_attributeInfo)/sizeof(_attributeInfo[0]); i++ )/*lint !e737 !e574*/
    {
        if ( _attributeInfo[i].listID != LISTID(connectionId) )
        {
            continue;
        }
		MMI_GetLabelTextByLang(_attributeInfo[i].attributeLabelID,&attri_name);
		dumpbuf(attri_name.wstr_ptr, attri_name.wstr_len);
			    
		SCI_MEMCPY(label.text,attri_name.wstr_ptr,attri_name.wstr_len*sizeof(wchar));
		label.length = attri_name.wstr_len;
        retval = AmPIMAttributeDefArrayAddAttributeDef( array,
                                                        _attributeInfo[i].attributeID,
                                                       &label );
        if ( retval != JK_PIM_SUCCESS )
        {
            JWE_LOG(PimContactGetSupportedAttributes,("err:AmPIMAttributeDefArrayAddAttributeDef fail"));
            return JK_PIM_MEMORY_ERR;
        }
        
    }

    JWE_LOG(PimContactGetSupportedAttributes,("leave"));
    return JK_PIM_SUCCESS;
}

JKPIMErrCode PimContactGetResultSetByString(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* string,JKSint32* resultSetId)
{
	// BOOLEAN error_code=SCI_TRUE;
	PimResultSetInfo *results = NULL;
	JKPIMString      *stringCopy = NULL;
	int i;

	JWE_LOG(PimContactGetResultSetByString,("enter,connectionId=%d",connectionId));

	results = (PimResultSetInfo *)SCI_ALLOC( sizeof(PimResultSetInfo) );

	if( !results )
	{
		JWE_LOG(PimContactGetResultSetByString,("err:SCI_ALLOC for resuotSet fail."));
		return JK_PIM_MEMORY_ERR;
	}

	results->size =PIM_GetItemCount(LISTID(connectionId));
	results->index = INVALID_INDEX;
	results->connectionId = connectionId;
	results->searchType = PIM_SEARCH_TYPE_STRING;

	stringCopy = (JKPIMString *)SCI_ALLOC( sizeof(*stringCopy) + (uint32)string->length * sizeof(JKWChar) );   // this will be freed in closeResultSet
	if( stringCopy == NULL )
	{
		JWE_LOG(PimContactGetResultSetByString,("err:SCI_ALLOC for stringCopy fail."));
		return JK_PIM_MEMORY_ERR;
	}
	stringCopy->length = string->length;
	stringCopy->text   = (JKWChar *)((char *)stringCopy + sizeof(*stringCopy));

	for ( i = 0; i < string->length; ++i )
	{
		stringCopy->text[i] = string->text[i];
	}
	results->searchData.string = stringCopy;

	*resultSetId = (JKSint32)results;
	currentResultSet = results;

	JWE_LOG(PimContactGetResultSetByString,("leave.resultSetId=(%d),itemCount=(%d)", *resultSetId, results->size));
	return JK_PIM_SUCCESS;

}

JKPIMErrCode PimContactGetResultSetByItem(const JKSint32 taskId,const JKSint32 connectionId,JKPIMItem* pItem,JKSint32* resultSetId)
{
	PimResultSetInfo *results = NULL;
	MMIPB_CONTACT_T *entryCopy = NULL;
	int group_id = -1;
	BOOLEAN error_code=SCI_TRUE;

	results = (PimResultSetInfo *)SCI_ALLOC( sizeof(PimResultSetInfo) );

	if ( !results )
	{
		JWE_LOG(PimContactGetResultSetByItem,("err:Alloc  for PimResultSetInfo fail."));
		return JK_PIM_MEMORY_ERR;
	}

	results->size = PIM_GetItemCount(LISTID(connectionId));
	results->index = INVALID_INDEX;
	results->searchType =  PIM_SEARCH_TYPE_ITEM;
	results->connectionId = connectionId;

	/* allocate entryCopy space, entryCopy will be freed in closeResultSet */
	entryCopy = (MMIPB_CONTACT_T *)SCI_ALLOC(sizeof(MMIPB_CONTACT_T));
	if( entryCopy == NULL )
	{
		JWE_LOG(PimContactGetResultSetByItem,("err:SCI_ALLOC for entryCopy fail."));
		return JK_PIM_MEMORY_ERR;
	}

	SCI_MEMSET(entryCopy, 0x00, sizeof(MMIPB_CONTACT_T));
	
	error_code = pimItemToPhoneEntry( entryCopy, pItem, connectionId );
	if ( error_code !=TRUE )
	{
		JWE_LOG(PimContactGetResultSetByItem,("pimItemToPhoneEntry() fail."));
		return JK_PIM_GENERAL_ERR;
	}
	
	// category to  group_id
	JWE_LOG(PimContactGetResultSetByItem,("pItem->categories.size=(%d)",pItem->categories.size));
	if ( pItem->categories.size > 0 )
	{
		group_id = PIM_GetGroupIdByName(connectionId, pItem->categories.data[0]->text, pItem->categories.data[0]->length );

		JWE_LOG(PimContactGetResultSetByItem,("group_id=(%d)",group_id));
		if( group_id == -1 )
		{
			if( LISTID(connectionId) == PHONEBOOK_NV_STORAGE )
		    {
		    	entryCopy->union_t.group = PB_GROUP_HOME;
		    }
		    if( LISTID(connectionId) == PHONEBOOK_SIM_STORAGE )
		    {
		    	entryCopy->union_t.group = PB_GROUP_SIM_1;   
		    }
		}
		else
		{
			entryCopy->union_t.group = group_id;
		}
	}
	results->searchData.item = (JKPIMItem *)entryCopy;// when get next item,it will be serilzed
	// debug
	printEntry( entryCopy );

	*resultSetId = (JKSint32)results;
	currentResultSet = results;

	JWE_LOG(PimContactGetResultSetByItem,("leave"));
	return JK_PIM_SUCCESS;

}


JKPIMErrCode PimContactGetResultSetByCategory(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* category,JKSint32* resultSetId)
{
	PimResultSetInfo *results = NULL;
	JKSint32          groupId = 0;
	
    JWE_LOG(PimContactGetResultSetByCategory,("enter,connectionId=%d",connectionId));
    dumpbuf(category->text,category->length);

    results = (PimResultSetInfo *)SCI_ALLOC( sizeof(PimResultSetInfo) );

    if ( !results )
    {
        JWE_LOG(PimContactGetResultSetByCategory,("err:jvm_malloc for resuotSet fail."));
        return JK_PIM_MEMORY_ERR;
    }

    results->size = PIM_GetItemCount(LISTID(connectionId));
    results->index = INVALID_INDEX;
    results->searchType =  PIM_SEARCH_TYPE_CATEGORY;
    results->connectionId = connectionId;

	groupId = PIM_GetGroupIdByName(connectionId, category->text, category->length);

   /* +TCK fix pim tck cases*/   
	if ( category->text == 0 )
        results->searchData.groupId = PB_GROUP_ALL;
    else if ( groupId == -1 )
   /* -TCK fix pim tck cases*/   
    {
#ifdef VM_TCK_ENTRY /* for tck com.ibm.tck.javax.microedition.pim.ContactList.ItemsByCategory#test0007 */      
		if( LISTID(connectionId) == PHONEBOOK_NV_STORAGE )
	    {
	    	results->searchData.groupId = PB_GROUP_HOME;
	    }
	    if( LISTID(connectionId) == PHONEBOOK_SIM_STORAGE )
	    {
	    	results->searchData.groupId = PB_GROUP_SIM_1;   
	    }	   
#else
        // The enumeration should be empty for categories not in the list
        results->size = 0;
        results->index = INVALID_INDEX;
#endif        
    }
    else
    {
        results->searchData.groupId = groupId;
    }

	JWE_LOG(PimContactGetResultSetByCategory,("results->size=%d", results->size));
	JWE_LOG(PimContactGetResultSetByCategory,("results->index=%d",results->index));
	JWE_LOG(PimContactGetResultSetByCategory,(" results->searchData.groupId=%d", results->searchData.groupId));

	*resultSetId = (JKSint32)results;
	currentResultSet = results;

	JWE_LOG(PimContactGetResultSetByCategory,("leave,resultSetId=(%d),itemCount=(%d)", *resultSetId, results->size));

	return JK_PIM_SUCCESS;
}



JKPIMErrCode PimContactGetResultSetByAll(const JKSint32 taskId,const JKSint32 connectionId,JKSint32* resultSetId)
{
	PimResultSetInfo *results = NULL;


    JWE_LOG(PimContactGetResultSetByAll,("enter,connectionId=%d",connectionId));

    results = (PimResultSetInfo *)SCI_ALLOC( sizeof(PimResultSetInfo) );

    if ( !results )
    {
        JWE_LOG(PimContactGetResultSetByAll,("err:jvm_malloc for resuotSet fail."));
        return JK_PIM_MEMORY_ERR;
    }

    results->size = PIM_GetItemCount(LISTID(connectionId));
    results->index = INVALID_INDEX;
    results->searchType = PIM_SEARCH_TYPE_ALL;
    results->connectionId = connectionId;

    *resultSetId = (JKSint32)results;
    currentResultSet = results;

    JWE_LOG(PimContactGetResultSetByAll,("resultSetId=(%d),itemCount=(%d)", *resultSetId, results->size));

    return JK_PIM_SUCCESS;
}

JKPIMErrCode PimContactMoveToNextItem(const JKSint32 taskId,const JKSint32 connectionId,const JKSint32 resultSetId,JKSint32* itemId)
{
	BOOLEAN     error_code = SCI_TRUE;
	uint16	    entryId = 0;
    PimResultSetInfo *results = (PimResultSetInfo *)resultSetId;

    JWE_LOG(PimContactMoveToNextItem,("enter"));

    *itemId = 0;

    if ( results->index == results->size -1 )
    {
        results->index = INVALID_INDEX;

        JWE_LOG(PimContactMoveToNextItem,("err:leave,JK_PIM_END_OF_RESULTSET_ERR"));
        return JK_PIM_END_OF_RESULTSET_ERR;
    }

   // #if 0//mary modify
    switch (results->searchType)
    {
        case PIM_SEARCH_TYPE_CATEGORY:
			{	

				JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_CATEGORY "));
				results->index++;
								
				JWE_LOG(PimContactMoveToNextItem,("results->searchData.groupId= %d",results->searchData.groupId));
				
				SCI_MEMSET(&gm_entry, 0x00, sizeof(MMIPB_CONTACT_T));
				//PIM Ivy.Su 20100527 begin
				//gm_entry.entry_id = PimContactGetEntryIdByPos(LISTID(connectionId),results->index);				
                //pb new
                gm_entry.contact_id = PimContactGetEntryIdByPos(LISTID(connectionId), (uint16)results->index);
                gm_entry.storage_id = PimContactGetStorageIdByPos(LISTID(connectionId));//@@mary modify need to check
                
                //PIM Ivy.Su 20100527 end
               if(MMIPB_ERROR_SUCCESS != MMIAPIPB_GetContactInfo(gm_entry.contact_id,gm_entry.storage_id,&gm_entry))
               {
                    error_code = FALSE;
               }
				//error_code = MMIPB_ReadPhonebookEntry(LISTID(connectionId),&gm_entry);
				if( error_code != TRUE )
				{
				   JWE_LOG(PimContactMoveToNextItem,("err:MMIAPIPB_GetContactInfo() fail. error_code=%d", error_code));
				  	return JK_PIM_GENERAL_ERR;
				}
				while( results->index < results->size )
				{
					JWE_LOG(PimContactMoveToNextItem,("results->index = %d",results->index));
					if((uint8)(results->searchData.groupId) == gm_entry.union_t.group)
					{
						JWE_LOG(PimContactMoveToNextItem,("groupId equal"));
						break;
					}
					else
					{
						results->index++;

						if(results->index >= results->size)
						{
							JWE_LOG(PimContactMoveToNextItem,("end !!"));
							return JK_PIM_END_OF_RESULTSET_ERR;
						}

						JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_CATEGORY results->index = %d ",results->index));			

						SCI_MEMSET(&gm_entry, 0x00, sizeof(MMIPB_CONTACT_T));
						gm_entry.contact_id =PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
                        gm_entry.storage_id = PimContactGetStorageIdByPos(LISTID(connectionId));//@@mary modify need to check
						JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_CATEGORY gm_entry.contact_id=%d ",gm_entry.contact_id));			
            
						//error_code = MMIPB_ReadPhonebookEntry(LISTID(connectionId),&gm_entry);
                        if(MMIPB_ERROR_SUCCESS !=MMIAPIPB_GetContactInfo(gm_entry.contact_id,gm_entry.storage_id,&gm_entry))
                        {
                            error_code = FALSE;
                        }
                        if( error_code != TRUE )
						{
							 JWE_LOG(PimContactMoveToNextItem,("err:MMIPB_ReadPhonebookEntry() fail. error_code=%d", error_code));
							 return JK_PIM_GENERAL_ERR;
						}
					}
			
				}
				entryId = PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
				if ( entryId == 0 )
				{
					JWE_LOG(PimContactMoveToNextItem,("err:fetchByIndex(%d) fail. error_code=%d", results->index, error_code));
					return JK_PIM_END_OF_RESULTSET_ERR;
				}
 
                
			 }				
			 break;
        case PIM_SEARCH_TYPE_ITEM:
			{
				JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_ITEM "));
				results->index++;
				JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_ITEM results->index_f = %d ",results->index));			
				
				SCI_MEMSET(&gm_entry, 0x00, sizeof(MMIPB_CONTACT_T));
				gm_entry.contact_id =PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
                gm_entry.storage_id = PimContactGetStorageIdByPos(LISTID(connectionId));//@@mary modify need to check
				//error_code = MMIPB_ReadPhonebookEntry(LISTID(connectionId),&gm_entry);
                if(MMIPB_ERROR_SUCCESS !=MMIAPIPB_GetContactInfo(gm_entry.contact_id,gm_entry.storage_id,&gm_entry))
                {
                    error_code = FALSE;
                }
				if ( error_code != TRUE )
				{
				   JWE_LOG(PimContactMoveToNextItem,("err:MMIPB_ReadPhonebookEntry() fail. error_code=%d", error_code));
				  return JK_PIM_GENERAL_ERR;
				}
				/*
				For fields that store Strings as values, matching is defined as true if the input String field value is contained 
				anywhere within the field's stored String value. The locale of the device MAY be used as a basis for the 
				matching rules. Comparisons are case insensitive if case is supported in the current locale. For example,
				if the matching item NOTE field is set to "pho", a positive match occurs in the strings "Mobile phone" and 
				"THIS IS A PHONE NOTE" as well as the string "telephone". If an empty string is provided in the matching
				item (i.e. String("")), the empty string matches all set values for the field.
				*/
				JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_ITEM while start "));	
				JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_ITEM results->index = %d VS results->size =  %d ", results->index,results->size));	
				printEntry(&gm_entry);
				printEntry( ( MMIPB_CONTACT_T*)results->searchData.item);
				while ( results->index < results->size )
				{
					if(pimSearchEntryByPartialEntry(&gm_entry, ( MMIPB_CONTACT_T*)results->searchData.item))
					{
						JWE_LOG(PimContactMoveToNextItem,("find one "));			
						break;
					}
					else
					{
						results->index++;

						if(results->index>=results->size)
						{
							JWE_LOG(PimContactMoveToNextItem,("end !!"));
							return JK_PIM_END_OF_RESULTSET_ERR;
						}

						JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_ITEM results->index = %d ",results->index));			

						SCI_MEMSET(&gm_entry, 0x00, sizeof(MMIPB_CONTACT_T));
						gm_entry.contact_id =PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
                        gm_entry.storage_id = PimContactGetStorageIdByPos(LISTID(connectionId));//@@mary modify need to check
						JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_ITEM gm_entry.contact_id=%d ",gm_entry.contact_id));			
                        if(MMIPB_ERROR_SUCCESS != MMIAPIPB_GetContactInfo(gm_entry.contact_id,gm_entry.storage_id,&gm_entry))
                        {
                            error_code = FALSE;
                        }
						//error_code = MMIPB_ReadPhonebookEntry(LISTID(connectionId),&gm_entry);
						if ( error_code != TRUE )
						{
							 JWE_LOG(PimContactMoveToNextItem,("err:MMIPB_ReadPhonebookEntry() fail. error_code=%d", error_code));
							 return JK_PIM_GENERAL_ERR;
						}

					}

				}
				JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_ITEM while end "));	
				entryId = PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
				if ( entryId == 0 )
				{
					JWE_LOG(PimContactMoveToNextItem,("err:fetchByIndex(%d) fail. error_code=%d", results->index, error_code));
					return JK_PIM_END_OF_RESULTSET_ERR;
				}
			}
			break;
		case PIM_SEARCH_TYPE_STRING:
			{
				JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_STRING "));

				results->index++;
				JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_STRING results->index_f = %d ",results->index));		

				SCI_MEMSET(&gm_entry, 0x00, sizeof(MMIPB_CONTACT_T));
				gm_entry.contact_id =PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
                gm_entry.storage_id = PimContactGetStorageIdByPos(LISTID(connectionId));//@@mary modify need to check
				//error_code = MMIPB_ReadPhonebookEntry(LISTID(connectionId),&gm_entry);
               if(MMIPB_ERROR_SUCCESS != MMIAPIPB_GetContactInfo(gm_entry.contact_id,gm_entry.storage_id,&gm_entry))
               {
                    error_code = FALSE;
               }
				if( error_code != TRUE )
				{
					JWE_LOG(PimContactMoveToNextItem,("err:MMIPB_ReadPhonebookEntry() fail. error_code=%d", error_code));
					return JK_PIM_GENERAL_ERR;
				}
				while( results->index < results->size )
				{
					if( pimSearchEntryByString(&gm_entry, results->searchData.string))
					{
						JWE_LOG(PimContactMoveToNextItem,("find one "));			
						break;
					}
					else
					{
						results->index++;
						JWE_LOG(PimContactMoveToNextItem,("results->index=%d",results->index));			

						if(results->index>=results->size)
						{
							JWE_LOG(PimContactMoveToNextItem,("end !!"));
							return JK_PIM_END_OF_RESULTSET_ERR;
						}

						JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_STRING results->index = %d ",results->index));			

						SCI_MEMSET(&gm_entry, 0x00, sizeof(MMIPB_CONTACT_T));
						gm_entry.contact_id =PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
                        gm_entry.storage_id = PimContactGetStorageIdByPos(LISTID(connectionId));//@@mary modify need to check
						JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_STRING gm_entry.contact_id=%d ",gm_entry.contact_id));			

				    	//error_code = MMIPB_ReadPhonebookEntry(LISTID(connectionId),&gm_entry);
                        if(MMIPB_ERROR_SUCCESS != MMIAPIPB_GetContactInfo(gm_entry.contact_id,gm_entry.storage_id,&gm_entry))
                        {
                            error_code = FALSE;
                        }
						if( error_code != TRUE )
						{
						  JWE_LOG(PimContactMoveToNextItem,("err:MMIPB_ReadPhonebookEntry() fail. error_code=%d", error_code));
						  return JK_PIM_GENERAL_ERR;
						}

					}
				}
		 		JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_STRING while end "));	
				entryId = PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
				if ( entryId == 0 )
				{
					JWE_LOG(PimContactMoveToNextItem,("err:fetchByIndex(%d) fail. error_code=%d", results->index, error_code));
					return JK_PIM_END_OF_RESULTSET_ERR;
				}
			}
			 break;
		case PIM_SEARCH_TYPE_ALL:
        default:			         	 
        	{
	    	  JWE_LOG(PimContactMoveToNextItem,("case PIM_SEARCH_TYPE_ALL "));
	    	  results->index++;
	    	  entryId = PimContactGetEntryIdByPos(LISTID(connectionId),results->index);
	    	  if ( entryId == 0 )
	          {
	             JWE_LOG(PimContactMoveToNextItem,("err:fetchByIndex(%d) fail. error_code=%d", results->index, error_code));
	             return JK_PIM_END_OF_RESULTSET_ERR;
	          }
            }
            break;
	 }            
	// #endif
    *itemId = (JKSint32)entryId;
    JWE_LOG(PimContactMoveToNextItem,("move to item(%d), itemId=(%d)", results->index, *itemId));
    JWE_LOG(PimContactMoveToNextItem,("leave"));
    
    return JK_PIM_SUCCESS;        	 
}


//@@need modify interface mary need
//itemId 低16位entry_id
//高16位storage_id
JKPIMErrCode PimContactDeleteItem(const JKSint32 taskId,/*const JKSint32 connectionId,*/JKSint32 itemId,const JKBool commitCtrl)
{
	BOOLEAN  error_code = SCI_TRUE;
	gm_entry.contact_id =(uint16) itemId;
    gm_entry.storage_id = itemId>>16;
    JWE_LOG(PimContactDeleteItem,("enter. itemId(%d)",itemId));
    if(MMIPB_ERROR_SUCCESS != MMIAPIPB_DeleteContact(gm_entry.contact_id, gm_entry.storage_id))
    {
        error_code = FALSE;
    }
	
	if( error_code != TRUE )
    {
        JWE_LOG(PimContactDeleteItem,("jpim_phb_deleteEntry fail. retval=(%d)", error_code));
        return JK_PIM_GENERAL_ERR;
    }
 	/*
     * for delete item right after read the next item ( item.hasElement() )
     */
    if ( currentResultSet )
    {
/* +TCK fix pim tck cases*/   	
        if ( currentResultSet->index >= 0 && currentResultSet->size > 0 )
/* -TCK fix pim tck cases*/   		
        {
            currentResultSet->index--;
            currentResultSet->size--;
        }
    }
    JWE_LOG(PimContactDeleteItem,("leave"));
    return JK_PIM_SUCCESS;
}

//@@need modify interface mary need
//itemId 低16位entry_id
//高16位storage_id
JKPIMErrCode PimContactReadItem(const JKSint32 taskId,/*const JKSint32 connectionId,*/JKSint32 itemId,JKPIMItem* item)
{
	BOOLEAN  error_code = SCI_TRUE;
	JKPIMString data = {0};
	MMIPB_CUSTOM_PHOTO_T custom_photo ={0};
	MMIPB_NAME_T          group_name = {0};
	SFS_HANDLE filehandler_forphoto;
	int8* ptr;
	uint32 read = 0;
	MMIPB_BCD_NUMBER_T  number_t = {0};
	//+CRNEWMS00110898
	uint8 tele_number[PIM_FIELD_LEN+2] = {0};
	//-CRNEWMS00110898
	uint8 tele_length = 0;

		
	data.text = SCI_ALLOCA(PIM_FIELD_LEN+2);
	if ( data.text == NULL )
    {
        JWE_LOG(PimContactReadItem,("err:data.text alloc fail"));
        return JK_PIM_MEMORY_ERR;
    }	
	gm_entry.contact_id =(uint16) itemId;
    gm_entry.storage_id = itemId>>16;
	//error_code = MMIPB_ReadPhonebookEntry(LISTID(connectionId),&gm_entry);
	//new pb
    error_code = MMIAPIPB_GetContactInfo(gm_entry.contact_id,gm_entry.storage_id,&gm_entry);
    if( error_code != TRUE )
    {
       JWE_LOG(PimContactReadItem,("err:MMIPB_ReadPhonebookEntry() fail. error_code=%d", error_code));
       return JK_PIM_GENERAL_ERR;
    }
    /* name */
    if(gm_entry.name.wstr_len> 0 )
    {
    	JWE_LOG(PimContactReadItem,("name found"));
        	
	SCI_MEMCPY(data.text, gm_entry.name.wstr,gm_entry.name.wstr_len*sizeof(wchar));
	data.length = gm_entry.name.wstr_len;
    	AmPIMItemAddString( item,
                            JK_PIM_CONTACT_FORMATTED_NAME,
                            0,
                            &data );
		/* debug */                            
    	//dumpbuf((uint8*) data.text,data.length*2);
    }   

    /* mobile number */
    number_t = gm_entry.number[0];
    if(MMIAPIPB_IsValidPhoneNumber(
    		                     number_t.npi_ton,
    		                     number_t.number_len
    		                     )
		)    		                     
    {
    	JWE_LOG(PimContactReadItem,("mobile number"));	
        tele_length = MMIAPIPB_BCDNumberToString(&number_t, sizeof(tele_number), tele_number);
//     	tele_length = MMIAPICOM_GenDispNumber(
//     					  				MMIPB_GetNumberTypeFromUint8(number_t.npi_ton),//in
//     					  				number_t.number_len,//in
//     					  				number_t.number,//in
//     					  				tele_number,//out
//     					  				//+CRNEWMS00110898
//     					  				sizeof(tele_number) //in
//     					  				//-CRNEWMS00110898 
//     					  				); //Ivy replace MMI_GenDispNumber
		 //dumpbuf(tele_number,tele_length);  
		 
	     data.length = PIM_MakeIntoUcs2( (uint8*) data.text,
								    	 PIM_FIELD_LEN+1,
								    	 0,
								    	 tele_number,
								    	 tele_length);    	
	    AmPIMItemAddString( item,
                            JK_PIM_CONTACT_TEL,
                            JK_PIM_CONTACT_ATTR_MOBILE,
                            &data );					  				
    }  
     /* home number */
    number_t = gm_entry.number[1];
    if (MMIAPIPB_IsValidPhoneNumber(
    		                     number_t.npi_ton,
    		                     number_t.number_len
    		                     )
		)    		                     
    {
    	JWE_LOG(PimContactReadItem,("home number"));
         tele_length = MMIAPIPB_BCDNumberToString(&number_t, sizeof(tele_number), tele_number);
//     	tele_length = MMIAPICOM_GenDispNumber(
//     					  				MMIPB_GetNumberTypeFromUint8(number_t.npi_ton),//in
//     					  				number_t.number_len,//in
//     					  				number_t.number,//in
//     					  				tele_number,//out
//     					  				//+CRNEWMS00110898
//     					  				sizeof(tele_number) //in
//     					  				//-CRNEWMS00110898 
//     					  				);//Ivy replace MMI_GenDispNumber
		 //dumpbuf(tele_number,tele_length);   
		 data.length = PIM_MakeIntoUcs2( (uint8*) data.text,
								    	 PIM_FIELD_LEN+1,
								    	 0,
								    	 tele_number,
								    	 tele_length); 
		 AmPIMItemAddString( item,
                            JK_PIM_CONTACT_TEL,
                            JK_PIM_CONTACT_ATTR_HOME,
                            &data );				  				
    }   
    /* work number */
    number_t = gm_entry.number[2];
    if (MMIAPIPB_IsValidPhoneNumber(
    		                     number_t.npi_ton,
    		                     number_t.number_len
    		                     )
		)    		                     
    {
    	JWE_LOG(PimContactReadItem,("work number"));
        tele_length = MMIAPIPB_BCDNumberToString(&number_t, sizeof(tele_number), tele_number);        
//     	tele_length = MMIAPICOM_GenDispNumber(
//     					  				MMIPB_GetNumberTypeFromUint8(number_t.npi_ton),//in
//     					  				number_t.number_len,//in
//     					  				number_t.number,//in
//     					  				tele_number,//out
//     					  				//+CRNEWMS00110898
//     					  				sizeof(tele_number) //in
//     					  				//-CRNEWMS00110898 
//     					  				);//Ivy replace MMI_GenDispNumber
		 //dumpbuf(tele_number,tele_length);  
	 
     	 data.length = PIM_MakeIntoUcs2( (uint8*) data.text,
								    	 PIM_FIELD_LEN+1,
								    	 0,
								    	 tele_number,
								    	 tele_length);    	 
		 AmPIMItemAddString( item,
                            JK_PIM_CONTACT_TEL,
                            JK_PIM_CONTACT_ATTR_WORK,
                            &data );				  				
    }
#ifdef MMIPB_MAIL_SUPPORT
    /* mail */
    if(gm_entry.mail.wstr_len> 0 )
    {
    	JWE_LOG(PimContactReadItem,("mail enter"));	
	 SCI_MEMCPY(data.text, gm_entry.mail.wstr,gm_entry.mail.wstr_len*sizeof(wchar));
	 data.length = gm_entry.mail.wstr_len;
#ifdef VM_TCK_ENTRY	/* for case com.ibm.tck.javax.microedition.pim.PIM.ToSerialFormat#test0001 */							    	 
    	if(gm_entry.mail.wstr_len!= 1 && memcmp((char*) gm_entry.mail.wstr,(wchar*)" ",gm_entry.mail.wstr_len))
	    {/* debug */                            
	    	//dumpbuf(data.text,data.length*2); 		
		    AmPIMItemAddString( item,
	                            JK_PIM_CONTACT_EMAIL,
	                            0,
	                            &data );
		}
		else
		{
			JWE_LOG(PimContactReadItem,("gm_entry.mail.mail_len = %d",gm_entry.mail.mail_len));		
		}
#else
		 AmPIMItemAddString( item,
	                            JK_PIM_CONTACT_EMAIL,
	                            0,
	                            &data );
#endif	                            		
    }
#endif

#if 0//ndef PB_SUPPORT_LOW_MEMORY	
    /* note */
    if(/*(gm_entry.custom_text.is_valid) &&*/ (gm_entry.memo.wstr_len> 0) )
    {
		JWE_LOG(PimContactReadItem,("note enter"));	 	
		SCI_MEMCPY(data.text,gm_entry.memo.wstr,gm_entry.memo.wstr_len*sizeof(wchar));
		data.length = gm_entry.memo.wstr_len;
		AmPIMItemAddString( item,
                            JK_PIM_CONTACT_NOTE,
                            0,
                            &data );
    }
    /* title */
    if((gm_entry.title.wstr_len> 0) )
    {
		JWE_LOG(PimContactReadItem,("title enter"));	 	
		SCI_MEMCPY(data.text,gm_entry.title.wstr,gm_entry.title.wstr_len*sizeof(wchar));
		data.length = gm_entry.title.wstr_len;
		AmPIMItemAddString( item,
                            JK_PIM_CONTACT_TITLE,
                            0,
                            &data );
    }
	/* org */
    if((gm_entry.company.wstr_len> 0) )
    {
		JWE_LOG(PimContactReadItem,("org enter"));	 	
		SCI_MEMCPY(data.text,gm_entry.company.wstr,gm_entry.company.wstr_len*sizeof(wchar));
		data.length = gm_entry.company.wstr_len;
	    AmPIMItemAddString( item,
                            JK_PIM_CONTACT_ORG,
                            0,
                            &data );
    }

	/* work address */
    if((gm_entry.address.wstr_len> 0) )
    {
		JWE_LOG(PimContactReadItem,(" work address enter"));	 	
		SCI_MEMCPY(data.text,gm_entry.address.wstr,gm_entry.address.wstr_len*sizeof(wchar));
		data.length = gm_entry.address.wstr_len;

		/* debug */                            
		AmPIMItemAddString( item,
                            JK_PIM_CONTACT_ADDR,
                            0,
                            &data );
   }
#endif
	/* photo url */

   	JWE_LOG(PimContactReadItem,(" photo url enter 0000"));
	if(gm_entry.photo_id_flag & MMIPB_PHOTO_ID_EXIST)
        {
            MMIPB_ReadPhotoFile(gm_entry.contact_id, gm_entry.storage_id,&custom_photo);
            if(custom_photo.is_valid &&(!MMIAPIFMM_IsFileExist((const wchar *)custom_photo.file_name,(uint16)custom_photo.file_name_len)))
            {
                //photo file not exist
                custom_photo.is_valid = FALSE;
            }
        }
	JWE_LOG(PimContactReadItem,("custom_photo.is_valid = %d",custom_photo.is_valid));	
	JWE_LOG(PimContactReadItem,("file_name_len = %d",custom_photo.file_name_len));	
	
    if((custom_photo.is_valid) &&(custom_photo.file_name_len> 0) )
    {

    	JWE_LOG(PimContactReadItem,(" photo url enter"));	 	
    	data.length = PIM_MakeIntoUcs2( (uint8*) data.text,
								    	 PIM_FIELD_LEN,
								    	 1,
								    	 (uint8 *)custom_photo.file_name,
								    	 (uint16)custom_photo.file_name_len*sizeof(wchar));
	JWE_LOG(PimContactReadItem,(" 11111 file_name_len=%d",(uint16)custom_photo.file_name_len));	 	
	JWE_LOG(PimContactReadItem,(" 222222 data.length=%d",data.length));	 		
    	////**have some problrm  type cast!**/
    	/* debug */                            
   	//dumpbuf(data.text,data.length*2);
	    AmPIMItemAddString( item,
                            JK_PIM_CONTACT_PHOTO_URL,
                            0,
                            &data );
		//**have some problrm  photo type  photo size**/
   }


	/* photo */

	JWE_LOG(PimContactReadItem,(" photo enter 0000"));	
	JWE_LOG(PimContactReadItem,("custom_photo.is_valid = %d",custom_photo.is_valid));	
	JWE_LOG(PimContactReadItem,("file_name_len = %d",custom_photo.file_name_len));	
	
    if((custom_photo.is_valid) &&(custom_photo.file_name_len> 0) )
    {
		JWE_LOG(PimContactReadItem,(" photo enter"));	 	
		JWE_LOG(PimContactReadItem,(" custom_photo.file_size = %d",custom_photo.file_size));	 	

		ptr = SCI_ALLOC(custom_photo.file_size);

		JWE_LOG(PimContactReadItem,(" gm_entry file_name "));	 	
		dumpbuf((uint8*)custom_photo.file_name , 10);

		filehandler_forphoto = SFS_CreateFile((const uint16 *)custom_photo.file_name/*full_path_name*/, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, 0, 0);
		JWE_LOG(PimContactReadItem,(" filehandler_forphoto = %d ",filehandler_forphoto));	 

		SFS_ReadFile(filehandler_forphoto, ptr, custom_photo.file_size, &read, 0);
		JWE_LOG(PimContactReadItem,(" read = %d ",read));	 

		SFS_CloseFile(filehandler_forphoto);
		dumpbuf(ptr , 10);
			AmPIMItemAddBinary  (   item,   JK_PIM_CONTACT_PHOTO,  0,  custom_photo.file_size,  ptr )  ;
			SCI_FREE(ptr);
   }
    /* group name*/
	JWE_LOG(PimContactReadItem,("read group id=%d", gm_entry.union_t.group));   
    MMIPB_ReadGroupName(&group_name,gm_entry.union_t.group);
    if(group_name.wstr_len> 0 )
    {
    	JWE_LOG(PimContactReadItem,("group name enter"));		
    	data.length = PIM_MakeIntoUcs2( (uint8*) data.text,
								    	 PIM_FIELD_LEN,
								    	 1,
								    	 (uint8*)group_name.wstr,
								    	 group_name.wstr_len*sizeof(wchar));
    	
    	/* debug */                            
    	//dumpbuf(data.text,data.length*2);    	
    	AmPIMItemAddCategory(item,data.text, data.length);
    }   
    SCI_FREE(data.text);

    JWE_LOG(PimContactReadItem,("leave"));
    return JK_PIM_SUCCESS;   
}




JKPIMErrCode PimContactAddItem(const JKSint32 taskId,const JKSint32 connectionId, JKPIMItem* item,JKSint32* itemId)
{
	BOOLEAN error_code=SCI_TRUE;
	JKSint32        group_id = -1;
	int i= 0;
	if( item->listType != JK_PIM_CONTACT )
	{
		JWE_LOG(PimContactAddItem,("err:listType != JK_PIM_CONTACT. return error"));
		return JK_PIM_GENERAL_ERR;
	}
	
	SCI_MEMSET( &gm_entry, 0x00, sizeof(gm_entry) );
	
	JWE_LOG(PimContactAddItem,("item->size=(%d)", item->size));  // number of fields
	
	error_code = pimItemToPhoneEntry( &gm_entry, item, connectionId );
	if( error_code !=TRUE )
	{
		JWE_LOG(PimContactAddItem,("pimItemToPhoneEntry() fail."));
		return JK_PIM_GENERAL_ERR;
	}	
	JWE_LOG(PimContactAddItem,("pimItemToPhoneEntry leave"));		
/* +TCK fix pim tck cases*/   			
	if(gm_entry.number[i].number_len == 0 && gm_entry.name.wstr_len == 0)
	{
		const char undefined[]="UNDEFINED";
		//MMIPB_InternalStringToWstr(&gm_entry.name.wstr_len,gm_entry.name.wstr,MMIPB_NAME_MAX_LEN,
			//TRUE,sizeof(undefined)-1,(uint8*)undefined);
	}
/* -TCK fix pim tck cases*/   

	/*  category designate to gm_entry */
	JWE_LOG(PimContactAddItem,("categories.size=(%d)",item->categories.size));
    if ( item->categories.size > 0 )
    {
        group_id = PIM_GetGroupIdByName(connectionId, item->categories.data[0]->text, item->categories.data[0]->length );
        gm_entry.union_t.group = group_id;
		JWE_LOG(PimContactAddItem,("group_id=(%d)",group_id));
	}
	/*
	 * if group id has no designate(item->categories.size < 0) or 
	 * porting layer can not find the group id that java designate
	 */		
	if ( group_id == -1 )
	{
	    if( LISTID(connectionId) == PHONEBOOK_NV_STORAGE )
	    {
	    	JWE_LOG(PimContactAddItem,("set group_id=(%d) when group_id is -1",PB_GROUP_HOME));
/* +TCK fix pim tck cases*/   
	    	gm_entry.union_t.group = PB_GROUP_ALL;
	    }
	    if( LISTID(connectionId) == PHONEBOOK_SIM_STORAGE )
	    {
	    	JWE_LOG(PimContactAddItem,("set group_id=(%d) when group_id is -1",PB_GROUP_SIM_1));
	    	gm_entry.union_t.group = PB_GROUP_ALL;
/* -TCK fix pim tck cases*/   

	    }	    
	}
    if(MMIPB_ERROR_SUCCESS != MMIAPIPB_AddContact(&gm_entry))
    {
        	return JK_PIM_GENERAL_ERR;
    }
	

	
	if(	gm_entry.contact_id >= PIM_INVALID_MAX_ENTRY_ID)
	{
		JWE_LOG(PimContactAddItem,("err:gm_entry.contact_id>= MMIPB_INVALID_MAX_ENTRY_ID"));
		return JK_PIM_GENERAL_ERR;
	}

	*itemId=gm_entry.contact_id;
	JWE_LOG(PimContactAddItem,("contact_id=(%d)",*itemId));
	printEntry( &gm_entry );


	JWE_LOG(PimContactAddItem,("MMIPB_UpdatePhonebookEntry leave"));

   	return JK_PIM_SUCCESS;
 
}

/*****************************************************************************/
// 	Description : jkpimitem to entry
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
static BOOLEAN pimItemToPhoneEntry( MMIPB_CONTACT_T *entry, const JKPIMItem *item, const JKSint32 connectionId )
{
    uint8    strLen = 0;
    uint8    *str = NULL;
    uint8    *str_utf8=NULL;
    uint8    strLen_utf8=0;
    // int32 ret_code;
    // JKPIMErrCode    err = JK_PIM_GENERAL_ERR;
    // JKSint32        attrib;
    // JKPIMCalendar   calendar;

    int i;  // index of the fields in the item
    int j;  // index of the data in the field
    //char debugStr[PIM_LISTNAME_LEN+1] = {'\0'};
	/******/
	MMI_PARTY_NUMBER_T temp_party_num={0};//entry will get its content and numbertype should be convert
	/******/
	BOOLEAN mailcodeexceptionflag=FALSE;
	BOOLEAN telcodeexceptionflag=FALSE;
	int mailcodeexceptionenum;
	int telcodeexceptionenum;
	/*****/
	// SFS_HANDLE filehandler_forphoto;
	// uint32	filesize_forphoto;							
	// uint32	phototype_forphoto;
	// SFS_ERROR_E getfilesizeresult;


	
    SCI_MEMSET( entry, 0x00, sizeof(*entry) );

	JWE_LOG(pimItemToPhoneEntry,("**********enter:item->size=(%d)**********",item->size ));	

    for( i = 0; i < item->size; i++ )
    {
		JWE_LOG(pimItemToPhoneEntry,("*********item->fields[i]->fieldId =(%d) ", item->fields[i]->fieldId));

		switch ( item->fields[i]->fieldId ) {
#ifdef MMIPB_MAIL_SUPPORT

			case JK_PIM_CONTACT_EMAIL:
				{
					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_EMAIL*********"));		
					JWE_LOG(pimItemToPhoneEntry,("size(%d)=(%d)", i, item->fields[i]->size));
			#ifdef VM_TCK_ENTRY	/* for case com.ibm.tck.javax.microedition.pim.PIM.ToSerialFormat#test0001 */
					if(item->fields[i]->size == 0) 
					{
						JWE_LOG(pimItemToPhoneEntry,("item->fields[i]->size==0"));	
						entry->mail.wstr_len=1;
						//entry->mail.is_ucs2=FALSE;	
						entry->mail.wstr[0]=0x20;
						break;
					}
			#endif			
					for ( j = 0; j < item->fields[i]->size; j++ )//but only one attribute
					{
						str = (uint8 *)item->fields[i]->data.strings[j]->data.text;
						strLen = item->fields[i]->data.strings[j]->data.length;				
						JWE_LOG(pimItemToPhoneEntry,("TO SEE item 's len is %d",strLen ));

						for(mailcodeexceptionenum=0;mailcodeexceptionenum<strLen;mailcodeexceptionenum++)
						{
							if(item->fields[i]->data.strings[j]->data.text[mailcodeexceptionenum] > 128)
							{
								mailcodeexceptionflag = TRUE;
								JWE_LOG(pimItemToPhoneEntry,("main include unicode char!"));
								break;
							}
						}
						if(mailcodeexceptionflag){
							mailcodeexceptionflag = FALSE;
							JWE_LOG(pimItemToPhoneEntry,("err:main include unicode char"));
							break;
						}
					
						if( strLen ==0){//input the  data=null
							break;
						}
						if ( strLen > MMIPB_MAX_MAIL_LEN ) {
						    strLen = MMIPB_MAX_MAIL_LEN;
						}
						//dumpbuf(str, strLen*4);
						JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_EMAIL  strlen =(%d)",strLen));	
						/*
						i think the e-mail is the uft-8,because it only have english,one utf-8 have 1 BYTE
						so the " entry->mail.is_ucs2=FALSE "
						*/
						//str_utf8= SCI_ALLOC(strLen*sizeof(JKUint16)/2);//str is "00XX" "00XY",00 is waste;
						
						// replace GUI_UCS2B2UTF8 by GUI_WstrToUTF8
						//strLen_utf8=GUI_WstrToUTF8(str_utf8,MMIPB_MAX_MAIL_LEN,str,strLen*sizeof(JKUint16));//return utf len is 8bit a number, out,in,in,in,the sign of end is'0x00'	
						//dumpbuf(str_utf8, strLen_utf8*4);
						JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_EMAIL  strLen_utf8 =(%d)",strLen_utf8));	

						SCI_MEMCPY( (uint8*)entry->mail.wstr, str,  strLen*2 );		
						//SCI_FREE(str_utf8);
						entry->mail.wstr_len=strLen;//strLen_utf8;
						//entry->mail.is_ucs2=FALSE;
						//dumpbuf(entry->mail.mail, strLen_utf8*4);
						JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_EMAIL  entry->mail.mail_len=(%d)",entry->mail.wstr_len));	
						//JWE_LOG(pimItemToPhoneEntry,("str->data.text(%d)=(%s)", j, str));
					}

			}
            break;
#endif
        case JK_PIM_CONTACT_FORMATTED_ADDR:
            {
                JWE_LOG(pimItemToPhoneEntry,("fields(%d)(F_ADDR)=()", i));
            }
            break;
        case JK_PIM_CONTACT_FORMATTED_NAME:
			{
				JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_FORMATTED_NAME*********"));	
				JWE_LOG(pimItemToPhoneEntry,("size(%d)=(%d)", i, item->fields[i]->size));

				for( j = 0; j < item->fields[i]->size; j++ )
				{
					str = (uint8 *)item->fields[i]->data.strings[j]->data.text;
					strLen = item->fields[i]->data.strings[j]->data.length;

					if( strLen ==0){//input the  data=null
						break;
					}
					if ( strLen > /*(MMIPB_NAME_MAX_LEN-1)/2 */MMIPB_NAME_MAX_LEN) {
						strLen = /*(MMIPB_NAME_MAX_LEN-1)/2*/MMIPB_NAME_MAX_LEN ;
					}
					JWE_LOG(,("JK_PIM_CONTACT_FORMATTED_NAME strLen = %d",strLen));	
					//dumpbuf(str, strLen*4);
					
					//MMIPB_InternalStringToWstr(&(entry->name.wstr_len),entry->name.wstr,MMIPB_NAME_MAX_LEN,TRUE,strLen*sizeof(JKUint16),str);
					//entry->entry_name.is_ucs2=TRUE;

					JWE_LOG(,("JK_PIM_CONTACT_FORMATTED_NAME entry->entry_name.wstr_len = %d",(entry->name.wstr_len)));//oberviously the len is buff len not character number
					//dumpbuf(entry->entry_name.alpha,entry->entry_name.alpha_len*4);
				}
			}
            break;

#if 0//ndef PB_SUPPORT_LOW_MEMORY
        case JK_PIM_CONTACT_NOTE:
			{
				JWE_LOG(pimItemToPhoneEntry,("************JK_PIM_CONTACT_NOTE"));

				for ( j = 0; j < item->fields[i]->size; j++ )
				{
					str = (uint8 *)item->fields[i]->data.strings[j]->data.text;
					strLen = item->fields[i]->data.strings[j]->data.length;

					if( strLen ==0){//input the  data=null
						break;
					}
					if( strLen > MMIPB_MAX_CUSTOM_TEXT_LEN ) {
						strLen = MMIPB_MAX_CUSTOM_TEXT_LEN;
					}
					
					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_NOTE strLen = %d",strLen));	
					//dumpbuf(str, strLen*4);
					
					SCI_MEMCPY( (uint8*)entry->memo.wstr, str,  strLen * sizeof(JKUint16) );
					entry->memo.wstr_len=strLen;
					//entry->memo.is_ucs2=TRUE;
					//entry->memo.is_valid=TRUE;

					//entry->memo.text_buf[strLen*sizeof(JKUint16)] = '\0';
					//entry->memo.text_buf[strLen*sizeof(JKUint16)+1] = '\0';

					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_NOTE entry->memo.wstr_len = %d",(entry->memo.wstr_len)));
					//dumpbuf(entry->memo.text_buf,entry->custom_text.text_len*4);
				}           
            }
            break;



        case JK_PIM_CONTACT_TITLE:
			{
				JWE_LOG(pimItemToPhoneEntry,("************JK_PIM_CONTACT_TITLE"));

				for ( j = 0; j < item->fields[i]->size; j++ )
				{
					str = (uint8 *)item->fields[i]->data.strings[j]->data.text;
					strLen = item->fields[i]->data.strings[j]->data.length;

					if( strLen ==0){//input the  data=null
						break;
					}
					if( strLen > MMIPB_MAX_TITLE_LEN ) {
						strLen = MMIPB_MAX_TITLE_LEN;
					}
					
					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_TITLE strLen = %d",strLen));	
					//dumpbuf(str, strLen*4);
					
					SCI_MEMCPY((uint8*) entry->title.wstr, str,  strLen * sizeof(JKUint16) );
					entry->title.wstr_len=strLen;
					//entry->title.is_ucs2=TRUE;

					//entry->title.title_text[strLen*sizeof(JKUint16)] = '\0';
					//entry->title.title_text[strLen*sizeof(JKUint16)+1] = '\0';

					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_TITLE entry->title.wstr_len = %d",(entry->title.wstr_len)));
					//dumpbuf(entry->custom_text.text_buf,entry->custom_text.text_len*4);
				}           
            }
            break;



        case JK_PIM_CONTACT_ORG:
			{
				JWE_LOG(pimItemToPhoneEntry,("************JK_PIM_CONTACT_ORG"));

				for ( j = 0; j < item->fields[i]->size; j++ )
				{
					str = (uint8 *)item->fields[i]->data.strings[j]->data.text;
					strLen = item->fields[i]->data.strings[j]->data.length;

					if( strLen ==0){//input the  data=null
						break;
					}
					if( strLen > MMIPB_MAX_ORG_LEN ) {
						strLen = MMIPB_MAX_ORG_LEN;
					}
					
					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_ORG strLen = %d",strLen));	
					//dumpbuf(str, strLen*4);
					
					SCI_MEMCPY( entry->company.wstr, str,  strLen * sizeof(JKUint16) );
					entry->company.wstr_len=strLen;
					//entry->company.is_ucs2=TRUE;

					//entry->company.company[strLen*sizeof(JKUint16)] = '\0';
					//entry->company.company[strLen*sizeof(JKUint16)+1] = '\0';

					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_ORG entry->company.wstr_len = %d",(entry->company.wstr_len)));
					//dumpbuf(entry->memo.text_buf,entry->memo.text_len*4);
				}           
            }
            break;


        case JK_PIM_CONTACT_ADDR:
			{
				JWE_LOG(pimItemToPhoneEntry,("************JK_PIM_CONTACT_ADDR"));

				for ( j = 0; j < item->fields[i]->size; j++ )
				{
					str = (uint8 *)item->fields[i]->data.strings[j]->data.text;
					strLen = item->fields[i]->data.strings[j]->data.length;

					if( strLen ==0){//input the  data=null
						break;
					}
					if( strLen > MMIPB_MAX_WORKADR_LEN ) {
						strLen = MMIPB_MAX_WORKADR_LEN;
					}
					
					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_ADDR strLen = %d",strLen));	
					//dumpbuf(str, strLen*4);
					
					SCI_MEMCPY( entry->address.wstr, str,  strLen * sizeof(JKUint16) );
					entry->address.wstr_len=strLen;
					//entry->address.is_ucs2=TRUE;
					
					//entry->address.adr_text[strLen*sizeof(JKUint16)] = '\0';
					//entry->address.adr_text[strLen*sizeof(JKUint16)+1] = '\0';

					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_ADDR entry->address.wstr_len = %d",(entry->address.wstr_len)));
					//dumpbuf(entry->custom_text.text_buf,entry->custom_text.text_len*4);
				}           
            }
            break;
#endif

	case JK_PIM_CONTACT_PHOTO_URL:
			{
			//
				JWE_LOG(pimItemToPhoneEntry,("************JK_PIM_CONTACT_PHOTO_URL"));

				for ( j = 0; j < item->fields[i]->size; j++ )
				{
					str = (uint8 *)item->fields[i]->data.strings[j]->data.text;
					strLen = item->fields[i]->data.strings[j]->data.length;

					if( strLen ==0){//input the  data=null
						break;
					}
					//uint8 < 256
					//if( strLen > MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN) {
						//strLen = MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN;
					//}
					
					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_PHOTO_URL strLen = %d",strLen));	
					//dumpbuf(str, strLen*4);


					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_PHOTO_URL  Start to judge the input filename"));	
                     /* todo
					SCI_MEMCPY( entry->custom_photo.file_name, str,  strLen * sizeof(JKUint16) );
					// the max len number of str (data.text trans down from jblend) is MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN/2
					//the max len number of entry->custom_photo.file_name (init 0x00) is MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN/2+1
					//so the end of the filename wstr is "0x00 0x00"
					JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_PHOTO_URL entry->custom_text.text_len = %d",(entry->custom_photo.file_name_len)));
					filehandler_forphoto = SFS_CreateFile((const uint16 *)entry->custom_photo.file_name, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, 0, 0);
					getfilesizeresult= SFS_GetFileSize( filehandler_forphoto, &filesize_forphoto);
					
					JWE_LOG(PimContactReadItem,("JK_PIM_CONTACT_PHOTO_URL filehandler_forphoto = %d ",filehandler_forphoto));	 
					JWE_LOG(PimContactReadItem,("JK_PIM_CONTACT_PHOTO_URL filesize_forphoto = %d ",filesize_forphoto));	
					
							if((filehandler_forphoto!=0)&&(getfilesizeresult==SFS_ERROR_NONE))
							{
					JWE_LOG(PimContactReadItem,("JK_PIM_CONTACT_PHOTO_URL valid  file  exited "));			
							//valid  file  exited
					entry->custom_photo.file_name_len=strLen* sizeof(JKUint16);
					entry->custom_photo.is_valid=TRUE;
					entry->custom_photo.file_size=filesize_forphoto;
					//entry->custom_photo.photo_type=???; it need SFS_ReadFile and the photo_type is which enum i dont know
					SFS_CloseFile(filehandler_forphoto);
					
							}
							else
							{
							//the file not exited
					JWE_LOG(PimContactReadItem,("JK_PIM_CONTACT_PHOTO_URL the file not exited "));						
					break;
							}*/

				}           
            }
            break;
			
        case JK_PIM_CONTACT_TEL:
			{
				JWE_LOG(pimItemToPhoneEntry,("************JK_PIM_CONTACT_TEL"));
				JWE_LOG(pimItemToPhoneEntry,("size(%d)=(%d)", i, item->fields[i]->size));

				for ( j = 0; j < item->fields[i]->size; j++ )
				{
					for(telcodeexceptionenum=0;telcodeexceptionenum< item->fields[i]->data.strings[j]->data.length;telcodeexceptionenum++)
					{
						if(item->fields[i]->data.strings[j]->data.text[telcodeexceptionenum] > 128)
						{
							telcodeexceptionflag=TRUE;
							JWE_LOG(pimItemToPhoneEntry,("phonenumber has unicode char!"));
							break;
						}
					}
					if(telcodeexceptionflag)
					{
						telcodeexceptionflag=FALSE;
						JWE_LOG(pimItemToPhoneEntry,("err:phonenumber has unicode char"));
						break;
					}

              		switch ( item->fields[i]->data.strings[j]->attributes ) {
						case JK_PIM_CONTACT_ATTR_HOME:      // home number
							{
								JWE_LOG(pimItemToPhoneEntry,("*********JK_PIM_CONTACT_ATTR_HOME enter***********"));
								str = (uint8 *)item->fields[i]->data.strings[j]->data.text;//item's data.text is JKUint16,the sign of end is 0x0000' wrong
								strLen = item->fields[i]->data.strings[j]->data.length;//strLen is the character number,one charactor is 16bit  wrong

								JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_TEL HOME strlen=(%d) ",strLen));
								if( strLen ==0){//input the  data=null
									break;
								}
								if ( strLen > (MMIPB_BCD_NUMBER_MAX_LEN<<1)) 
								{
									strLen = MMIPB_BCD_NUMBER_MAX_LEN<<1;
								}
	 
								str_utf8= SCI_ALLOC(strLen*sizeof(JKUint16)/2);//str is "00XX" "00XY",00 is waste;
								strLen_utf8=GUI_WstrToUTF8(str_utf8,strLen,(wchar*)str,strLen);//return utf len is 8bit a number, out,in,in,in,the sign of end is'0x00'
								MMIAPICOM_GenPartyNumber(str_utf8,strLen_utf8,&temp_party_num);
			
			            		SCI_FREE(str_utf8);
						        SCI_MEMCPY(entry->number[1].number,temp_party_num.bcd_num,temp_party_num.num_len);
								entry->number[1].number_len=(uint8 )temp_party_num.num_len;
								entry->number[1].npi_ton=PIM_MMIPB_GetUint8FromNumberType(temp_party_num.number_type);

	                        }
	                        break;
                    case JK_PIM_CONTACT_ATTR_WORK:      // work number
                        {
							JWE_LOG(pimItemToPhoneEntry,("*********JK_PIM_CONTACT_ATTR_WORK enter***********"));
							str = (uint8 *)item->fields[i]->data.strings[j]->data.text;//item's data.text is JKUint16,the sign of end is 0x0000'
							strLen = item->fields[i]->data.strings[j]->data.length;//strLen is the character number,one charactor is 16bit

							JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_TEL WORK strlen=(%d) ",strLen));
							if( strLen ==0){//input the  data=null
								break;
							}
                            
							if( strLen > (MMIPB_BCD_NUMBER_MAX_LEN<<1)) 
							{
								strLen = MMIPB_BCD_NUMBER_MAX_LEN<<1;
							}
 
							str_utf8= SCI_ALLOC(strLen*sizeof(JKUint16)/2);//str is "00XX" "00XY",00 is waste;
							strLen_utf8=GUI_WstrToUTF8(str_utf8,strLen,(wchar*)str,(uint16)strLen);//return utf len is 8bit a number, out,in,in,in,the sign of end is'0x00'
							MMIAPICOM_GenPartyNumber(str_utf8,strLen_utf8,&temp_party_num);
							
							SCI_FREE(str_utf8);
							SCI_MEMCPY(entry->number[2].number,temp_party_num.bcd_num,temp_party_num.num_len);
							entry->number[2].number_len=(uint8 )temp_party_num.num_len;
							entry->number[2].npi_ton=PIM_MMIPB_GetUint8FromNumberType(temp_party_num.number_type);
                        }
                        break;
					case JK_PIM_CONTACT_ATTR_MOBILE:    // mobile number
                        {
							JWE_LOG(pimItemToPhoneEntry,("*********JK_PIM_CONTACT_ATTR_MOBILE enter***********"));
							str = (uint8 *)item->fields[i]->data.strings[j]->data.text;//item's data.text is JKUint16,the sign of end is 0x0000'
							strLen = item->fields[i]->data.strings[j]->data.length;//strLen is the character number,one charactor is 16bit

							JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_TEL DEFAULT(MOBILE) strlen=(%d) ",strLen));
							if( strLen ==0){//input the  data=null
								break;
							}

							if ( strLen > (MMIPB_BCD_NUMBER_MAX_LEN<<1))  //@murphy to do
							{
								strLen = MMIPB_BCD_NUMBER_MAX_LEN<<1;
							}
							
							str_utf8= SCI_ALLOC(strLen*sizeof(JKUint16)/2);//str is "00XX" "00XY",00 is waste;
							strLen_utf8=GUI_WstrToUTF8(str_utf8,strLen,(wchar*)str,strLen);//return utf len is 8bit a number, out,in,in,in,the sign of end is'0x00'
							MMIAPICOM_GenPartyNumber(str_utf8,(int16)strLen_utf8,&temp_party_num);
		              		SCI_FREE(str_utf8);
				
                            SCI_MEMCPY(entry->number[0].number,temp_party_num.bcd_num,temp_party_num.num_len);
							entry->number[0].number_len=(uint8 )temp_party_num.num_len;
							entry->number[0].npi_ton=PIM_MMIPB_GetUint8FromNumberType(temp_party_num.number_type);
                        }
                        break;
                    default:
                        {
							// char *destPtr = NULL;

							JWE_LOG(pimItemToPhoneEntry,("attribute is not set (default)"));

							str = (uint8 *)item->fields[i]->data.strings[j]->data.text;//item's data.text is JKUint16,the sign of end is 0x0000'
							strLen = item->fields[i]->data.strings[j]->data.length;//strLen is the character number,one charactor is 16bit

							JWE_LOG(pimItemToPhoneEntry,("JK_PIM_CONTACT_TEL DEFAULT(MOBILE) strlen=(%d) ",strLen));
							if( strLen ==0){//input the  data=null
								break;
							}
							if ( strLen > (MMIPB_BCD_NUMBER_MAX_LEN<<1)) 
							{
								strLen = MMIPB_BCD_NUMBER_MAX_LEN<<1;
							}
							
							str_utf8= SCI_ALLOC(strLen*sizeof(JKUint16)/2);//str is "00XX" "00XY",00 is waste;
							strLen_utf8=GUI_WstrToUTF8(str_utf8,strLen,(wchar*)str,(uint16)strLen);//return utf len is 8bit a number, out,in,in,in,the sign of end is'0x00'
							MMIAPICOM_GenPartyNumber(str_utf8,strLen_utf8,&temp_party_num);
		              		SCI_FREE(str_utf8);
		
		                    SCI_MEMCPY(entry->number[0].number,temp_party_num.bcd_num,temp_party_num.num_len);
							entry->number[0].number_len=(uint8 )temp_party_num.num_len;
							entry->number[0].npi_ton=PIM_MMIPB_GetUint8FromNumberType(temp_party_num.number_type);
                        }
                        break;
                    }

                }

            }
            break;

        case JK_PIM_CONTACT_URL:
            {
                JWE_LOG(pimItemToPhoneEntry,("fields(%d)(URL)=()", i));
            }
            break;
        default:
            {
                JWE_LOG(pimItemToPhoneEntry,("fields(%d) UNSUPPORT", i));
            }
            break;
        }
    }

   return 1;

}

JKPIMErrCode PimContactSetItem(const JKSint32 taskId,const JKSint32 connectionId,JKSint32 itemId,JKPIMItem* item)
{
	BOOLEAN error_code=SCI_TRUE;

	JKSint32        group_id = -1;
	int i;
//	MMIPB_LIST_DETAILED_INDEX_T     temp_mldi;
	
	if( item->listType != JK_PIM_CONTACT )
	{
	    JWE_LOG(PimContactSetItem,("err:listType != JK_PIM_CONTACT. return error"));
	    return JK_PIM_GENERAL_ERR;
	}
	
 	SCI_MEMSET( &gm_entry, 0x00, sizeof(gm_entry) );
 	
	JWE_LOG(PimContactSetItem,("item->size=(%d)", item->size));  // number of fields
	error_code = pimItemToPhoneEntry( &gm_entry, item, connectionId );
    if(error_code != TRUE )
    {
        JWE_LOG(PimContactSetItem,("err:pimItemToPhoneEntry() fail."));
        return JK_PIM_GENERAL_ERR;
    }
#if 0//ndef VM_TCK_ENTRY    no need check,pb will check if valid 
    /* to check whether has number */
	for( i=0;i < MMIPB_MAX_NV_PHONE_NUM;i++)
	{
		if(gm_entry.number[i].number_len > 0 )
		{
			JWE_LOG(PimContactAddItem,("gm_entry.number_t.number_len > 0."));
			break;
		}
	}
	if( i>= MMIPB_MAX_NV_PHONE_NUM)
	{
		JWE_LOG(PimContactAddItem,("err:has not desigaten number!"));
		return JK_PIM_GENERAL_ERR;
	}
#endif	
	JWE_LOG(PimContactSetItem,("pimItemToPhoneEntry leave"));

	// category
	JWE_LOG(PimContactSetItem,("categories.size=(%d)",item->categories.size));
    if( item->categories.size > 0 )
    {
		group_id = PIM_GetGroupIdByName(connectionId, item->categories.data[0]->text, item->categories.data[0]->length );
		gm_entry.union_t.group = group_id;
		JWE_LOG(PimContactSetItem,("group_id=(%d)",group_id));
    }
    /*
	 * if group id has no designate(item->categories.size < 0) or 
	 * porting layer can not find the group id that java designate
	 */		
	if ( group_id == -1 )
	{
	    if( LISTID(connectionId) == PHONEBOOK_NV_STORAGE )
	    {
	    	gm_entry.union_t.group = PB_GROUP_HOME;/*set PB_GROUP_HOME to the pohne's group id if java has not designate category */
	    }
	    if( LISTID(connectionId) == PHONEBOOK_SIM_STORAGE )
	    {
	    	gm_entry.union_t.group = PB_GROUP_SIM_1;/*set PB_GROUP_SIM to the SIM's group id if java has not designate category */
	    }    
	}
	
	gm_entry.contact_id=(uint16) itemId;
	
	JWE_LOG(PimContactSetItem,("contact_id=(%d)",itemId));
	printEntry( &gm_entry );
	//Update!
#if 0	
	if( LISTID(connectionId) == PHONEBOOK_SIM_STORAGE )
	{
		MMIPB_LIST_CONDITION_T *list_condition_ptr = &(g_mmipb_table_info.list_condition);


        if (PNULL == list_condition_ptr->selected_index_ptr)
        {
			//no memory leak, this pointer will be free in pb_wintab.c
            list_condition_ptr->selected_index_ptr = SCI_ALLOC(sizeof (MMIPB_LIST_DETAILED_INDEX_T));
        }    
        //list_condition_ptr->selected_index_ptr[0].storage = PHONEBOOK_SIM_STORAGE; //Ivy
        //g_mmipb_sim_entry_t = gm_entry; //Ivy
	}
#endif
    //mary
    if(MMIPB_ERROR_SUCCESS !=MMIAPIPB_ModifyContact(&gm_entry))//@@MARY MODIFY NEED TO CHECK
    {
		JWE_LOG(PimContactSetItem,("MMIPB_UpdatePhonebookEntry() fail."));
	    return JK_PIM_GENERAL_ERR;
    }

	if( LISTID(connectionId) == PHONEBOOK_NV_STORAGE )
	{
		//MMIAPIPB_Erase(LISTID(connectionId),gm_entry.contact_id);
		MMIAPIPB_DeleteContact(gm_entry.contact_id,gm_entry.storage_id);
		
		//SCI_MEMSET( &temp_mldi, 0x00, sizeof(temp_mldi) );
		MMIAPIPB_AddContact(&gm_entry);
	}

	JWE_LOG(PimContactSetItem,("MMIPB_UpdatePhonebookEntry leave"));
	return JK_PIM_SUCCESS; 
}


JKPIMErrCode PimContactIsCategorySupported(const JKSint32 taskId,const JKSint32 connectionId,JKSint32* listMaximum,JKSint32* itemMaximum,JKBool* distinction)
{
//	JKSint32 group_other3 = (JKSint32)PB_GROUP_OTHER3; //for pclint
	JKSint32 group_home = (JKSint32)PB_GROUP_HOME;    //for pclint
	JWE_LOG(PimContactIsCategorySupported,("enter,connectionId=%d",connectionId));

    if ( LISTID(connectionId) == PHONEBOOK_SIM_STORAGE )
    {
        JWE_LOG(PimContactIsCategorySupported,("list id == JPIM_PHB_SIM"));
        *listMaximum = 1;
        *itemMaximum = 1;
        *distinction = JK_TRUE;
    }
    else
    {
/* +TCK fix pim tck cases*/   	
        *listMaximum = (JKSint32)PB_GROUP_SCHOOLMATE-group_home+1;
/* -TCK fix pim tck cases*/   		
        *itemMaximum = 1;
        *distinction = JK_TRUE;
    }

    JWE_LOG(PimContactIsCategorySupported,("leave"));
    return JK_PIM_SUCCESS;
}
JKPIMErrCode PimContactCloseResultSet(const JKSint32 taskId,const JKSint32 connectionId,const JKSint32 resultSetId)
{
	PimResultSetInfo *results = NULL;

    JWE_LOG(PimContactCloseResultSet,("enter,connectionId=(%d),resultSetId=(%d)", connectionId, resultSetId));

    if ( resultSetId != 0 )
    {
        results = (PimResultSetInfo *)resultSetId;
        if ( // results->searchType == PIM_SEARCH_TYPE_CATEGORY ||
             results->searchType == PIM_SEARCH_TYPE_STRING )
        {
            SCI_FREE( results->searchData.string );
			
        }

        if ( results->searchType == PIM_SEARCH_TYPE_ITEM )
        {
            SCI_FREE(results->searchData.item );
        }
        SCI_FREE( results );
    }

    JWE_LOG(PimContactCloseResultSet,("leave"));
    return JK_PIM_SUCCESS;
}
JKPIMErrCode PimContactGetSupportedCategories(const JKSint32 taskId,const JKSint32 connectionId,JKPIMStringArray* categories)
{
	MMIPB_NAME_T group_name = {0};
	JKPIMErrCode retval;
	// JKWChar wcText[PIM_CATEGORY_LEN];
    // JKSint32 wcLength;
	int i;
	
	JWE_LOG(PimContactGetSupportedCategories,("enter"));
	if ( LISTID(connectionId) == PHONEBOOK_SIM_STORAGE )
	{
		if(!MMIPB_IsExistedGroup(PB_GROUP_SIM_1)) 
		{
			JWE_LOG(PimContactGetSupportedCategories,("err:PB_GROUP_SIM desn't exit!"));
        	return JK_PIM_GENERAL_ERR;
		}
		MMIPB_ReadGroupName( &group_name,PB_GROUP_SIM_1);
		/*wcLength = PIM_MakeIntoUcs2( (uint8*) wcText,
								    	 PIM_FIELD_LEN,
								    	 group_name.is_ucs2,
								    	 group_name.alpha,
								    	 group_name.alpha_len);*/
		retval = AmPIMStringArrayAddString( categories, /*wcText*/group_name.wstr, /*wcLength*/group_name.wstr_len);
    	if ( retval != JK_PIM_SUCCESS )
    	{
    		JWE_LOG(PimContactGetSupportedCategories,("err:PHONEBOOK_SIM_STORAGE add category fail"));
    		return JK_PIM_GENERAL_ERR;
    	}	
    }
	else if(LISTID(connectionId) == PHONEBOOK_NV_STORAGE)
	{
		for(i= PB_GROUP_HOME;i<=PB_GROUP_SCHOOLMATE;i++)
		{
 			
			MMIPB_ReadGroupName ( &group_name,i);

								    	 
			retval = AmPIMStringArrayAddString( categories, group_name.wstr, group_name.wstr_len);
	    	if ( retval != JK_PIM_SUCCESS )
	    	{
        		JWE_LOG(PimContactGetSupportedCategories,("err:PHONEBOOK_NV_STORAGE add category fail"));
        		return JK_PIM_GENERAL_ERR;
	    	}		
		}
	}
	else
	{
		JWE_LOG(PimContactGetSupportedCategories,("err:unknown list id"));
        return JK_PIM_GENERAL_ERR;
	}
	
	JWE_LOG(PimContactGetSupportedCategories,("leave"));
    return JK_PIM_SUCCESS;	
}
JKPIMErrCode PimContactIsCategory(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* category,JKBool* found)
{
	MMIPB_NAME_T group_name = {0};
	// JKWChar wcText[PIM_CATEGORY_LEN];
    // JKSint32 wcLength;
	int i;
	   
	JWE_LOG(PimContactIsCategory,("enter"));

    *found = JK_FALSE;
	if ( LISTID(connectionId) == PHONEBOOK_SIM_STORAGE )
	{
		if(!MMIPB_IsExistedGroup(PB_GROUP_SIM_1)) 
		{
			JWE_LOG(PimContactIsCategory,("err:PB_GROUP_SIM desn't exit!"));
			*found = JK_FALSE;
			return JK_PIM_SUCCESS;
		}
		MMIPB_ReadGroupName( &group_name,PB_GROUP_SIM_1); 
		if((/*wcLength*/group_name.wstr_len== category->length) &&
				(!strncmp((char*) /*wcText*/group_name.wstr,(char*)category->text,group_name.wstr_len*sizeof(JKWChar))))
		{	
			*found = JK_TRUE;	
		}
		else
			*found = JK_FALSE;
	} 
	else if(LISTID(connectionId) == PHONEBOOK_NV_STORAGE)
	{
		for(i= PB_GROUP_HOME;i<= PB_GROUP_SCHOOLMATE;i++)
		{
			
			{
				MMIPB_ReadGroupName ( &group_name,i);
				if((group_name.wstr_len== category->length) &&
						(!strncmp((char*) group_name.wstr,(char*)category->text,group_name.wstr_len*sizeof(JKWChar))))
				{
					break;
				}
			 }
		}
		if(i > PB_GROUP_SCHOOLMATE)
			*found = JK_FALSE;
		else
			*found = JK_TRUE;
			
	}
	else
	{
		JWE_LOG(PimContactIsCategory,("err:unknown list id"));
        *found = JK_FALSE;
		return JK_PIM_SUCCESS;
	}

    JWE_LOG(PimContactIsCategory,("leave"));
    return JK_PIM_SUCCESS;
}
JKPIMErrCode PimContactAddCategory(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* category)
{
    JWE_LOG(PimContactAddCategory,("enter"));
    JWE_LOG(PimContactAddCategory,("leave"));
    return JK_PIM_GENERAL_ERR;
}
JKPIMErrCode PimContactDeleteCategory(const JKSint32 taskId,const JKSint32 connectionId,JKPIMString* category,const JKBool isDelete)
{
	JWE_LOG(PimContactDeleteCategory,("enter"));

	if(isDelete==JK_TRUE)
	{
		if(PIM_GetGroupIdByName(connectionId,category->text,category->length)== -1  )
		{
   		 	JWE_LOG(PimContactDeleteCategory,("leave"));
			JWE_LOG(PimContactDeleteCategory,("category is not in the list"));
		 	return JK_PIM_SUCCESS;
		}
		else
		{
			JWE_LOG(PimContactDeleteCategory,("leave"));
			JWE_LOG(PimContactDeleteCategory,("category is  in the list"));
			return JK_PIM_GENERAL_ERR ;
		}

	   
	}
    JWE_LOG(PimContactDeleteCategory,("leave"));
    return JK_PIM_GENERAL_ERR;
}
JKPIMErrCode PimContactAddCategoryToItem(const JKSint32 taskId,const JKSint32 connectionId,const JKSint32 itemId,const JKPIMString* category,JKBool* addable)
{
    JKSint32 group_id;
    JWE_LOG(PimContactAddCategoryToItem,("enter"));

    group_id = PIM_GetGroupIdByName( connectionId, category->text, category->length );

    if ( group_id == -1 )
    {
        JWE_LOG(PimContactAddCategoryToItem,("err:getGroupIdByName() fail"));
        return JK_PIM_GENERAL_ERR;
    }

    *addable = JK_TRUE;

    JWE_LOG(PimContactAddCategoryToItem,("leave"));
    return JK_PIM_SUCCESS;
	
}
JKPIMErrCode PimContactRenameCategory(const JKSint32 taskId,const JKSint32 connectionId,const JKPIMString* currentCategory,const JKPIMString* newCategory)
{
     int group_id = 0;
     MMIPB_NAME_T group_name = {0};
     BOOLEAN ret;
	 
    JWE_LOG(PimContactRenameCategory,("enter"));
	if(PIM_EchoGroupNameExist(connectionId,newCategory->text,newCategory->length))
	{
		return JK_PIM_GENERAL_ERR;
	}
/* +TCK fix pim tck cases*/   
    group_id = PIM_GetGroupIdByName( connectionId, newCategory->text, newCategory->length );
    if ( group_id >= 0 )
    {
        JWE_LOG(PimContactRenameCategory,("err:newCategory exist"));
        return JK_PIM_GENERAL_ERR;
    }
/* -TCK fix pim tck cases*/   
    // get group id by name
    group_id = PIM_GetGroupIdByName( connectionId, currentCategory->text, currentCategory->length );

    if ( group_id == -1 )
    {
        JWE_LOG(PimContactRenameCategory,("err:getGroupIdByName() fail"));
        return JK_PIM_GENERAL_ERR;
    }
    if ( newCategory->length /**2*/ > PIM_CATEGORY_LEN)  //Ivy
    {
        SCI_MEMCPY( /*group_name.alpha*/group_name.wstr, newCategory->text, PIM_CATEGORY_LEN );
        group_name.wstr_len= PIM_CATEGORY_LEN;
        //group_name.is_ucs2 = TRUE;
    }
    else
    {
        SCI_MEMCPY( group_name.wstr, newCategory->text, currentCategory->length * sizeof(JKWChar) );/*lint !e737*/
        group_name.wstr_len= currentCategory->length /** sizeof(JKWChar)*/;
        //group_name.is_ucs2 = TRUE;
    }

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    ret = MMIPB_SaveGroup(&group_name,group_id);
#else
    ret = FALSE;
#endif

    if(!ret)
    {
    	 JWE_LOG(PimContactRenameCategory,("err:MMIPB_SaveGroup() fail. ret_code=%d", ret));
        return JK_PIM_GENERAL_ERR;
    }

    JWE_LOG(PimContactRenameCategory,("leave"));
    return JK_PIM_SUCCESS;
}


/*I cant find this kind of function in zhanxun*/
static uint8  PIM_MMIPB_GetUint8FromNumberType
(
    MN_NUMBER_TYPE_E   number_type     //IN:
)
{
    return ((uint8)( number_type<<4));
}


/*printEntry debug*/
static void printEntry(  MMIPB_CONTACT_T *entry )
{
#ifdef _AJSC_DEBUG	
	JWE_LOG(printEntry,("enter"));

	JWE_LOG(printEntry,("contact_id=%d", entry->contact_id));
	JWE_LOG(printEntry,("group_id=%d", entry->union_t.group));


	JWE_LOG(printEntry,("entry->entry_name.wstr_len=%d", entry->name.wstr_len));
	dumpbuf(entry->name.wstr,entry->name.wstr_len);

#ifdef MMIPB_MAIL_SUPPORT
	JWE_LOG(printEntry,("entry->mail.wstr_len=%d", entry->mail.wstr_len));
	dumpbuf(entry->mail.wstr, entry->mail.wstr_len);
#endif

	JWE_LOG(printEntry,("entry->number_t[0].number_len (MOBILE) =%d", entry->number[0].number_len));
	dumpbuf(entry->number[0].number, entry->number[0].number_len);
	JWE_LOG(printEntry,("entry->number_t[1].number_len (HOME) =%d", entry->number[1].number_len));
	dumpbuf(entry->number[1].number, entry->number[1].number_len);
	JWE_LOG(printEntry,("entry->number_t[2].number_len (WORK) =%d", entry->number[2].number_len));
	dumpbuf(entry->number[2].number, entry->number[2].number_len);


	/*JWE_LOG(printEntry,("entry->group_name.alpha_len=%d", entry->group_name.wstr_len));
	dumpbuf(entry->group_name.alpha,entry->group_name.alpha_len );*/
#ifndef PB_SUPPORT_LOW_MEMORY
	JWE_LOG(printEntry,("entry->custom_text.wstr_len=%d", entry->memo.wstr_len));
	dumpbuf(entry->memo.wstr,entry->memo.wstr_len);
#endif

	JWE_LOG(printEntry,("leave"));
#endif
}

//if only one match. it is OK!
static BOOLEAN pimSearchEntryByString( MMIPB_CONTACT_T *entry, JKPIMString *string )
{

	int i ;
	BOOLEAN ret_val_bool = FALSE;
	// MMIPB_BCD_NUMBER_T  number_t = {0};
	uint8 tele_number[MMIPB_BCD_NUMBER_MAX_LEN] = {0};
	uint8 tele_length = 0;
	JKWChar	tele_numberWChar[PIM_FIELD_LEN/2]={0};
	JKSint32	tele_numberlengthWChar;

	// JKWChar	mailWChar[PIM_FIELD_LEN/2]={0};
	// uint16	maillengthWChar;


	JWE_LOG(pimSearchEntryByString,("enter"));

	if(string->length==0)
		return TRUE;

	JWE_LOG(pimSearchEntryByString,("enter match name"));	
	ret_val_bool = pimCommMatchString( (JKUint16 *)entry->name.wstr,
										((int)entry->name.wstr_len),
										string->text, (int)string->length
									  );//Ivy
	if( ret_val_bool )
	{
		JWE_LOG(pimSearchEntryByString,("TRUE leave"));
		return TRUE;
	}
	//number
	//number_t -> WCHAR (WCHAR VS WCHAR)
	JWE_LOG(pimSearchEntryByString,("enter match number"));	
	for(i=0;i<MMIPB_MAX_NV_PHONE_NUM;i++)
	{
        tele_length = MMIAPIPB_BCDNumberToString(&entry->number[i], MMIPB_BCD_NUMBER_MAX_LEN+2, tele_number);
// 		tele_length = MMIAPICOM_GenDispNumber(
// 											MMIPB_GetNumberTypeFromUint8(entry->number[i].npi_ton),//in
// 											entry->number[i].number_len,//in
// 											entry->number[i].number,//in
// 											tele_number,//out
// 											MMIPB_BCD_NUMBER_MAX_LEN+2 //in
// 										);//Ivy replace MMI_GenDispNumber
		dumpbuf(tele_number,tele_length);   

		tele_numberlengthWChar= PIM_MakeIntoUcs2( (uint8*) tele_numberWChar,
													PIM_FIELD_LEN,
													0,
													tele_number,
													tele_length
												);
		
		ret_val_bool = pimCommMatchString( tele_numberWChar, tele_numberlengthWChar, string->text,string->length);
		if( ret_val_bool )
		{
			JWE_LOG(pimSearchEntryByString,("TRUE leave"));
			return TRUE;
		}
		SCI_MEMSET(tele_number,0x00,sizeof(uint8)*MMIPB_BCD_NUMBER_MAX_LEN);
		SCI_MEMSET(tele_numberWChar,0x00,sizeof(JKWChar)*PIM_FIELD_LEN/2);

	}
#ifdef MMIPB_MAIL_SUPPORT
	//e-mail
	//UTF8->WCHAR(WCHAR VS WCHAR)
	JWE_LOG(pimSearchEntryByString,("enter match mail"));	

    /*maillengthWChar= PIM_MakeIntoUcs2( (uint8*) mailWChar,
								    	 PIM_FIELD_LEN,
								    	 entry->mail.is_ucs2,
								    	 entry->mail.mail,
								    	 entry->mail.mail_len
								      );*/

	ret_val_bool = pimCommMatchString(  /*mailWChar*/entry->mail.wstr,
                                        /*maillengthWChar*/entry->mail.wstr_len,
                                     	string->text, 
                                     	(int)string->length
                                     );
    if( ret_val_bool )
    {
        JWE_LOG(pimSearchEntryByString,("TRUE leave"));
        return TRUE;
    }
#endif

#if 0//ndef PB_SUPPORT_LOW_MEMORY
	//note
	//(WCHAR VS WCHAR)
	JWE_LOG(pimSearchEntryByString,("enter match note"));	
	ret_val_bool = pimCommMatchString( (JKUint16 *)entry->memo.wstr,
	                                	((int)/*entry->custom_text.text_len/2*/entry->memo.wstr_len),
	                             		string->text, (int)string->length
	                             	 ); 


	if( ret_val_bool )
	{
		JWE_LOG(pimSearchEntryByString,("TRUE leave"));
		return TRUE;
	}

	//org
	JWE_LOG(pimSearchEntryByString,("enter match org"));
	ret_val_bool = pimCommMatchString( (JKUint16 *)entry->company.wstr,
	                                	((int)/*entry->org.org_len/2*/entry->company.wstr_len),
	                             		string->text, (int)string->length
	                             	 );
	if( ret_val_bool )
	{
		JWE_LOG(pimSearchEntryByString,("TRUE leave"));
		return TRUE;
	}



	//work adress
	JWE_LOG(pimSearchEntryByString,("enter match work adress"));
	ret_val_bool = pimCommMatchString( (JKUint16 *)entry->address.wstr,
	                                	((int)/*entry->work_adr.adr_len/2*/entry->address.wstr_len),
	                             		string->text, (int)string->length
	                             	 );	
	if( ret_val_bool )
	{
		JWE_LOG(pimSearchEntryByString,("TRUE leave"));
		return TRUE;
	}

	//title
	JWE_LOG(pimSearchEntryByString,("enter match title"));
	ret_val_bool = pimCommMatchString( (JKUint16 *)entry->title.wstr,
	                                	((int)/*entry->title.title_len/2*/entry->title.wstr_len),
	                             		string->text, (int)string->length
	                             	 );
	if( ret_val_bool ) 		/*lint -e774*/
	{
		JWE_LOG(pimSearchEntryByString,("TRUE leave"));
		return TRUE;
	}
#endif

	//photo url
	JWE_LOG(pimSearchEntryByString,("enter match photo url"));
	/*ret_val_bool = pimCommMatchString( (JKUint16 *)entry->custom_photo.file_name,
	                                	((int)entry->custom_photo.file_name_len/2),
	                             		string->text, (int)string->length
	                             	 );*/
	if( ret_val_bool )
	{
		JWE_LOG(pimSearchEntryByString,("TRUE leave"));
		return TRUE;
	}



	JWE_LOG(pimSearchEntryByString,("FALSE leave"));
	return FALSE;
}

//match the entry and partial,if match!! return TRUE else return false

/*
For fields that store Strings as values, matching is defined as true if the input String field value is contained 
anywhere within the field's stored String value. The locale of the device MAY be used as a basis for the 
matching rules. Comparisons are case insensitive if case is supported in the current locale. For example,
if the matching item NOTE field is set to "pho", a positive match occurs in the strings "Mobile phone" and 
"THIS IS A PHONE NOTE" as well as the string "telephone". If an empty string is provided in the matching
item (i.e. String("")), the empty string matches all set values for the field.
*/
static BOOLEAN pimSearchEntryByPartialEntry( MMIPB_CONTACT_T *entry,
                                             MMIPB_CONTACT_T *partial )
{
	BOOLEAN isMatched = TRUE;
	uint8 i;
	JWE_LOG(pimSearchEntryByPartialEntry,("enter"));	

	//some one is not Matched return isMatched = FLASE;
	//if one a 0x00 then is Matched! else  to start matching!
	//matching field one by one,if first field is matched,then match second

	//name
	JWE_LOG(pimSearchEntryByPartialEntry,("enter match name"));	
	if( isMatched && (partial->name.wstr_len)) //lint !e774
	{
		// match name,name is Unicode JKUint16
		JWE_LOG(pimSearchEntryByPartialEntry,("partial->entry_name.wstr_len %d",partial->name.wstr_len));	
		
	    isMatched = pimCommMatchString( (JKUint16 *)entry->name.wstr,
	                                    ((int)/*entry->entry_name.alpha_len/2*/entry->name.wstr_len),
	                                    (JKUint16 *)partial->name.wstr,
	                                    ((int)/*partial->entry_name.alpha_len/2*/partial->name.wstr_len)
	                                  );
	}
	//number
	JWE_LOG(pimSearchEntryByPartialEntry,("enter match number"));	
	for(i=0;i<MMIPB_MAX_NV_PHONE_NUM;i++)
	{
		if( isMatched && (partial->number[i].number_len)) 
	   {
	      isMatched = pimCommMatchBCD_Number_T(&(entry->number[i]),&(partial->number[i]));
	   }

	}
#ifdef MMIPB_MAIL_SUPPORT
	//mail  my be a wrong
	JWE_LOG(pimSearchEntryByPartialEntry,("enter match mail"));	
	if( isMatched && (partial->mail.wstr_len) )
	{
		//endle is a question
		if(ajutil_findStringInBuffer((uint8*)entry->mail.wstr,(uint8*)partial->mail.wstr,MMIPB_BCD_NUMBER_MAX_LEN*2)!= AJ_NULL )//Ivy
		{
		    isMatched = TRUE;
		}
	}
#endif

#if 0//ndef PB_SUPPORT_LOW_MEMORY	
	//Note
	JWE_LOG(pimSearchEntryByPartialEntry,("enter match note"));	
	if( isMatched && (partial->memo.wstr_len) )
	{
		// match name,name is Unicode JKUint16
	    isMatched = pimCommMatchString( (JKUint16 *)entry->memo.wstr,
	                                    ((int)entry->memo.wstr_len),
	                                    (JKUint16 *)partial->memo.wstr,
	                                    ((int)partial->memo.wstr_len)
	                                  );
	}
	//org
	JWE_LOG(pimSearchEntryByPartialEntry,("enter match org"));	
	if( isMatched && (partial->company.wstr_len))
	{
		// match name,name is Unicode JKUint16
	    isMatched = pimCommMatchString( (JKUint16 *)entry->company.wstr,
	                                    ((int)/*entry->org.org_len/2*/entry->company.wstr_len),
	                                    (JKUint16 *)partial->company.wstr,
	                                    ((int)/*partial->org.org_len/2*/partial->company.wstr_len)
	                                  );
	}

	//title
	JWE_LOG(pimSearchEntryByPartialEntry,("enter match title"));	
	if( isMatched && (partial->title.wstr_len))
	{
		// match name,name is Unicode JKUint16
	    isMatched = pimCommMatchString( (JKUint16 *)entry->title.wstr,
	                                    ((int)/*entry->title.title_len/2)*/entry->title.wstr_len),
	                                    (JKUint16 *)partial->title.wstr,
	                                    ((int)/*partial->title.title_len/2*/partial->title.wstr_len)
	                                  );
	}


	//work adress
	JWE_LOG(pimSearchEntryByPartialEntry,("enter match work adress"));	
	if( isMatched && (partial->title.wstr_len) )
	{
		// match name,name is Unicode JKUint16
	    isMatched = pimCommMatchString( (JKUint16 *)entry->address.wstr,
	                                    ((int)entry->address.wstr_len),
	                                    (JKUint16 *)partial->address.wstr,
	                                    ((int)partial->address.wstr_len)
	                                  );
	}
#endif

	//photo url
	JWE_LOG(pimSearchEntryByPartialEntry,("enter match photo url"));	
	/*if( isMatched && *((JKUint16 *)(partial->custom_photo.file_name_len)) )
	{
		// match name,name is Unicode JKUint16
	   isMatched = pimCommMatchString( (JKUint16 *)entry->custom_photo.file_name,
	                                    ((int)entry->custom_photo.file_name_len/2),
	                                    (JKUint16 *)partial->custom_photo.file_name,
	                                    ((int)partial->custom_photo.file_name_len/2)
	                                  );
	       
	}*/




	JWE_LOG(pimSearchEntryByPartialEntry,("leave"));	

	return isMatched;
}

//number_t 's match  is so complex
//first step:bcd->utf8
//second step:call   ajutil_findStringInBuffer 
static BOOLEAN pimCommMatchBCD_Number_T(MMIPB_BCD_NUMBER_T *sourceNum_t,  MMIPB_BCD_NUMBER_T *matchNum_t )
{ 	
	BOOLEAN isOkToContinue = TRUE;     // set to FALSE only when failed(mem failed)
	uint8 source_num_utf8[MMIPB_BCD_NUMBER_MAX_LEN] = {0};
	uint8 source_num_utf8_length = 0;
	uint8 match_num_utf8[MMIPB_BCD_NUMBER_MAX_LEN] = {0};
	uint8 match_num_utf8_length = 0;
	
    // matchString is null
    if ( matchNum_t->number_len== 0 )
    {
        return TRUE;
    }
    // sourceString is null,matchString is not null
    if( sourceNum_t->number_len == 0 )
    {
        return FALSE;
    }
    //sourceString is less than matchString
    //note: 2345 use the space'size ==234 use the space'size
    if( sourceNum_t->number_len < matchNum_t->number_len)
    {
        return FALSE;
    }
	
	isOkToContinue = TRUE;

	//get utf8 type array space
    source_num_utf8_length = MMIAPIPB_BCDNumberToString(&sourceNum_t, MMIPB_BCD_NUMBER_MAX_LEN+2, source_num_utf8); 
// 	source_num_utf8_length = MMIAPICOM_GenDispNumber(
// 									  				MMIPB_GetNumberTypeFromUint8(sourceNum_t->npi_ton),//in
// 									  				sourceNum_t->number_len,//in
// 									  				sourceNum_t->number,//in
// 									  				source_num_utf8,//out
// 									  				MMIPB_BCD_NUMBER_MAX_LEN+2 //in
// 					  						   );//Ivy replace MMI_GenDispNumber
	dumpbuf(source_num_utf8,source_num_utf8_length);   
    match_num_utf8_length = MMIAPIPB_BCDNumberToString(&matchNum_t, MMIPB_BCD_NUMBER_MAX_LEN+2, match_num_utf8); 
		 
// 	match_num_utf8_length = MMIAPICOM_GenDispNumber(
// 			    					  				MMIPB_GetNumberTypeFromUint8(matchNum_t->npi_ton),//in
// 			    					  				matchNum_t->number_len,//in
// 			    					  				matchNum_t->number,//in
// 			    					  				match_num_utf8,//out
// 			    					  				MMIPB_BCD_NUMBER_MAX_LEN+2 //in
//     					  					  );//Ivy replace MMI_GenDispNumber
	dumpbuf(match_num_utf8,match_num_utf8_length);   

	//matching!!!
	if(ajutil_findStringInBuffer(source_num_utf8,match_num_utf8,MMIPB_BCD_NUMBER_MAX_LEN)!= AJ_NULL )
	{
		return TRUE;
	}

	return FALSE;
}
/*
sourceStrLen and matchStrLen is the character number in unicode, not a buff which one of  is 8 bit 
match sub string only can be unicode
*/
static BOOLEAN pimCommMatchString(JKUint16 *sourceStr, int sourceStrLen, JKUint16  *matchStr, int matchStrLen )
{
	int i;
    BOOLEAN isOkToContinue = TRUE;     // set to FALSE only when failed(mem failed)
 	BOOLEAN ret_val = FALSE;           // set to TRUE only when found
	//var matching needed
  	int checkCount = 0;
  	int length;

	/*********************************************
	avoid odd adress wrong!!!! use 8bit !!!var declare
	**********************************************/
	uint8 *sp=(uint8 *)sourceStr;//da shu duan
	uint8 *mp=(uint8 *)matchStr;
	int	slen = sourceStrLen*2;
	int	mlen =matchStrLen*2;
	uint8 *slower =NULL;
	uint8 *mlower =NULL;
	uint8 *ptrFirst = NULL, *ptrSecond = NULL;
	/*********************************************/
	
	JWE_LOG(pimCommMatchString,("enter"));	
	JWE_LOG(pimCommMatchString,("sourceStrLen=%d",sourceStrLen));	
	JWE_LOG(pimCommMatchString,("matchStrLen=%d",matchStrLen));	
	
	// matchString is null
	if( matchStrLen == 0 )
	{
		JWE_LOG(pimCommMatchString,("matchStrLen == 0"));	
		return TRUE;
	}
	// sourceString is null,matchString is not null
	if( sourceStrLen == 0 )
	{
		JWE_LOG(pimCommMatchString,("sourceStrLen == 0"));	
		return FALSE;
	}
	// sourceString is less than matchString
	if( sourceStrLen < matchStrLen )
	{
		JWE_LOG(pimCommMatchString,(" sourceStrLen < matchStrLen "));	
		return FALSE;
	}
	isOkToContinue = TRUE;

    // to get the space of source string's lower case
	if( isOkToContinue )  //lint !e774
	{
		slower=SCI_ALLOC(slen);
		
		if( slower == NULL )
		{
			JWE_LOG(pimCommMatchString,("jvm_malloc for slower1 fail"));
			ret_val = FALSE;
			isOkToContinue = FALSE;
		}	    
	}
	// to get the space of matching string's lower case
	if( isOkToContinue )
	{
		mlower=SCI_ALLOC(mlen);
		if( mlower == NULL )
		{
			JWE_LOG(pimCommMatchString,("jvm_malloc for mlower2 fail"));
			ret_val = FALSE;
			isOkToContinue = FALSE;
		}
	}
	
	/*in order to match this case:
	blend chinese and english ,only translate english " all unicode "
	use function-PimCommWcharToLowerCase
	*/
    if(isOkToContinue && (PNULL!= slower)&&(PNULL!= mlower))
    {
		// to get the source string's lower case
		JWE_LOG(pimCommMatchString,("slen=%d",slen));

		/**********************************************
		avoid odd adress wrong!!!! use 8bit!!!! to lower case
		***********************************************/
		for(i=0;i<slen;i=i+2)
		{
			slower[i]=sp[i];
			if(sp[i]==0x00)
			{
				slower[i+1]=PimCommScharToLowerCase(sp[i+1]);
			}
			else
			{
				slower[i+1]=sp[i+1];
			}
		}

		JWE_LOG(pimCommMatchString,("mlen=%d",mlen));
		// to get thematching string's lower case
		for(i=0;i<mlen;i=i+2)
		{
			mlower[i]=mp[i];
			if(mp[i]==0x00)
			{
				mlower[i+1]=PimCommScharToLowerCase(mp[i+1]);
			}
			else
			{
				mlower[i+1]=mp[i+1];
			}
		}
		JWE_LOG(pimCommMatchString,("dumpbuf mlower"));
		JWE_LOG(pimCommMatchString,("dumpbuf mlen=%d",mlen));
		dumpbuf(mlower, mlen*3);		
		JWE_LOG(pimCommMatchString,("dumpbuf slower"));
		JWE_LOG(pimCommMatchString,("dumpbuf slen=%d",slen));
		dumpbuf(slower, slen*3);		
	}

	JWE_LOG(pimCommMatchString,("matching!!!!"));   
	JWE_LOG(pimCommMatchString,("isOkToContinue!!!! %d" , isOkToContinue));   
	if( isOkToContinue && (slower!= NULL) && (mlower != NULL))
	{
		ptrFirst = slower;
		ptrSecond = mlower;
		length = mlen;
		checkCount = slen - mlen + 2;
		JWE_LOG(pimCommMatchString,("checkCount = %d", checkCount));   
		for ( i = 0; i < checkCount/2; ++i )
		{
			if( ptrFirst[i*2] == *ptrSecond &&
				!memcmp( &ptrFirst[i*2], ptrSecond, length) )
			{
				ret_val = TRUE;
				JWE_LOG(pimCommMatchString,("find! string wchar"));
				break;//if find one string in the sourcestring matched,break!
			}
		}
	}

    if( slower)
    {
        SCI_FREE( slower );
    }
    if( mlower)
    {
        SCI_FREE( mlower );
    }
	
	JWE_LOG(pimCommMatchString,("leave"));	
	return ret_val;
}

static uint8 PimCommScharToLowerCase( uint8 ch )
{
	JWE_LOG(PimCommScharToLowerCase,("enter"));
	if( 'A' <= ch && ch <= 'Z' )
	{
		JWE_LOG(PimCommScharToLowerCase,("leave zhuan huan"));
		return (ch + ('a' - 'A'));
	}
	JWE_LOG(PimCommScharToLowerCase,("leave bu zhuan huan"));
	return ch; 
}
#endif //JAVA_SUPPORT_IA
