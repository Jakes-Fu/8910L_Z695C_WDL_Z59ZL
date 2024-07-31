/*
* Copyright 2004-2015 Myriad Group AG. All Rights Reserved.
* $Id$
*/

/**
* @file
* Jbed Reference Port: Stub version implementation.
*/

#include <jbed_exported_defines.h>

/* As we couldn't add the feature flag of PDAP_PIM_SUPPORT in app.mk to include or exclude
 * cpl_pim.c to build. So we add PDAP_PIM_SUPPORT in source code; If PDAP_PIM_SUPPORT undefined,
 * we just let empty file taking part into build.
 */
#ifdef PDAP_PIM_SUPPORT
#include <opl_global.h>
#include <mmipb_nv.h>
//#include <mmipb_internal.h>
//#include <mmipb_mnapi.h>
#include <mmifmm_export.h>

#include <jbed_global.h>
#include <jbed_jbni.h>
//#include <jbed_legacy.h>
//#include <jbed_jbni_legacy.h>
#include <cpl_pim.h>
#include <cpl_file.h>
#include <cpl_os.h>
#include <mmipb_app.h>
#include <mmipb_export.h>
#define _TYPES_H_
#include <mmivcard_export.h>
#undef _TYPES_H_

#define CPL_PIM_DEBUG
#ifdef MMIJavaLog
#undef MMIJavaLog
#endif

#if defined(CPL_PIM_DEBUG) && defined(TRACE_ON)
#define MMIJavaLog(X) SCI_TRACE_LOW X
#else
#define MMIJavaLog(X)
#endif

/**
* Constant defines maximum values for all fields.
* Every field in the given pim implementation can contain only one value.
*/
#define JBED_MAX_VALUES 1

/* Initialisation count */
static int initCount = 0;

const wchar g_mmivcard_default_dir_wstr[] = { 'v', 'C', 'a', 'r', 'd', 0 };
#define VCARD_DEFAULT_DIR          (g_mmivcard_default_dir_wstr)//"\x00\x76\x00\x43\x00\x61\x00\x72\x00\x64\x00\x00"
#define VCARD_DEFAULT_DIR_LEN      5


enum
{
    PHONE,
    SIM,
    SIM2,
    SIM3,
    SIM4
};

typedef struct _CategoryDef
{
    int32_t type;
    const char * name;
    BOOLEAN  is_exist;
}CATEGORYDEF_T;

static CATEGORYDEF_T s_categorydefs[] =
{
    {PHONE, "Phone",TRUE},
    {SIM, "SIM",FALSE},
    {SIM2,"SIM2",FALSE},
    {SIM3,"SIM3",FALSE},
    {SIM4,"SIM4",FALSE}
};

typedef struct UID_Mapping_S
{
    int32_t storage;
    int32_t group_id;
    uint16 entry_id;
    int32_t uId;
    struct UID_Mapping_S *next;
}UID_MAPPING_T;

typedef struct PimList_s
{
    CATEGORYDEF_T *category;  // phone book, SIM, SIM2
    int32_t   type;
    UID_MAPPING_T *uidMapHeader;
    struct PimList_s *next;
    struct PimList_s *prev;
} PIMLIST_T,*PIMLIST_PRT_T;

static int32_t const s_supportedfields[] =
{
    CONTACT_UID,
    CONTACT_NAME,
    CONTACT_FORMATTED_NAME,
    CONTACT_TEL,
#ifdef MMIPB_MAIL_SUPPORT
    CONTACT_EMAIL,
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    CONTACT_URL,
    CONTACT_BIRTHDAY,
#endif
    CONTACT_NOTE,
};

/**
* Fields are supported by SIM address book.
*/
static int32_t const s_supportedfieldssim[] =
{
    CONTACT_FORMATTED_NAME,
    CONTACT_NAME,
    CONTACT_TEL,
};

static int32_t const s_telattributes_phone[] =
{
    CONTACT_ATTR_MOBILE,
    CONTACT_ATTR_HOME,
    CONTACT_ATTR_WORK,
#ifndef MMIPB_SYNC_WITH_O365
    CONTACT_ATTR_FAX,
    CONTACT_ATTR_PREF,
#endif
};

static int32_t const s_telattributes_sim[] =
{
    CONTACT_ATTR_MOBILE,
};

typedef struct
{
    int32_t        pimCode;
}FieldMap_s;

static const FieldMap_s s_suppnameelemmap[] =
{
    CONTACT_NAME_FAMILY,
    CONTACT_NAME_GIVEN,
    CONTACT_NAME_OTHER,
    CONTACT_NAME_PREFIX,
    CONTACT_NAME_SUFFIX,
#ifdef JBED_VFX_PIM
    CONTACT_NAME_READING
#endif
};

static const FieldMap_s s_suppaddrelemmap[] =
{
    CONTACT_ADDR_STREET
};

static CPL_pim_asyncResult_t s_pimresult;

#define DEFAULT_CONTACT_LIST_NAME "Contact List"  //16
#define DEFAULT_EVENT_LIST_NAME   "Event List" //14
#define DEFAULT_TODO_LIST_NAME    "Todo List"  //13

#define VCARD_PREFIX  "BEGIN:VCARD\r\n"
#define VCARD_VERSION "VERSION:2.1\r\n"
#define VCARD_UID     "UID:%d\r\n"
#define VCARD_CHARSET_GB2312_ "CHARSET=GB2312:"
#define VCARD_CHARSET_UTF8_ "CHARSET=UTF-8:"
#define VCARD_CELL_VOICE_ "CELL;VOICE:"
#define VCARD_WORK_VOICE_ "WORK;VOICE:"
#define VCARD_HOME_VOICE_ "HOME;VOICE:"
#define VCARD_PREF_INTERNET_ "PREF;INTERNET:"
#define VCARD_WORK_CHARSET_GB2312_ "WORK;CHARSET=GB2312:"
#define VCARD_WORK_CHARSET_UTF8_ "WORK;CHARSET=UTF-8:"
#define DEFAULT_TEL "TEL;CELL;VOICE:**#\r\n"
#define VCARD_UID_STRLEN 16
#define DEFAULT_NAME "No Name"
#define JBED_VCARD_MAX_LEN (5*1024)

static PIMLIST_T *s_listroot = NULL;

#define MMINV_READ_PHONEBOOK(_item_id, _addr, _ret_value)



void Jbed_writeInt(char *buf, int32_t value)
{
    buf[0] = (value >> 24) & 0xFF;
    buf[1] = (value >> 16) & 0xFF;
    buf[2] = (value >> 8) & 0xFF;
    buf[3] = (value) & 0xFF;
}

/* See cpl_pim.h. */
int32_t CPL_pim_startUp(void)
{
    return CPL_PIM_SUCCESS;
}

/* See cpl_pim.h. */
int32_t CPL_pim_shutDown()
{
    int flag=1;
    PIMLIST_T *templist=NULL;
    while (NULL!=s_listroot)
    {
        templist=s_listroot;

        s_listroot=s_listroot->next;
        s_listroot->prev=NULL;
        if (CPL_PIM_SUCCESS!=CPL_pim_closePIMList((int32_t)templist))
        {
            flag=0;
        }
    }
    if (flag)
        return CPL_PIM_SUCCESS;
    return CPL_PIM_ERROR;
}

static BYTEARRAY getPimListNames(CATEGORYDEF_T pimList[], int32_t count)
{
    BYTEARRAY namesArray;
    int32_t arrLength = 0;
    int32_t byteOffset = 0;
    int32_t i = 0;
    int32_t supportGroupCount = 0;
    char * array_ptr = NULL;

    MMIJavaLog(("getPimListNames(%p, %d)", pimList, count));
    if (pimList == NULL || count == 0)
    {
        return NULL;
    }

    arrLength += (int32_t)sizeof(int32_t);

    for(i = 0; i < count; i++)
    {
        if(PHONE == i)
        {
            supportGroupCount++;
        }
        else if(i >= (MN_DUAL_SYS_1 + 1) && i <= (MN_DUAL_SYS_MAX))
        {
            if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1 + i - 1))
            {
                pimList[i].is_exist = TRUE;
                supportGroupCount++;
            }
        }
        else
        {
            break;
        }
        if(pimList[i].is_exist)
            arrLength += ((int32_t)CRTL_strlen(pimList[i].name) + (int32_t)sizeof(int32_t));
    }
    MMIJavaLog(("getPimListNames arrLength = %d", arrLength));
    namesArray = Jbni_newByteArray(arrLength);   //auto GC
    array_ptr  = SCI_ALLOCA(arrLength + 1);
    if(array_ptr ==NULL)
    {
        return NULL;
    }
    CRTL_memset(array_ptr,0,arrLength + 1);

    if (supportGroupCount)
    {
        //Jbed_writeInt((char *)namesArray->data, supportGroupCount);
        Jbed_writeInt(array_ptr, supportGroupCount);
        byteOffset += (int32_t)sizeof(int32_t);
    }
    MMIJavaLog(("getPimListNames byteOffset1 = %d", byteOffset));

    for(i = 0; i < count; i++)
    {
        if(pimList[i].is_exist)
        {
            int elemLength = CRTL_strlen(pimList[i].name);

            Jbed_writeInt((char *)&array_ptr[byteOffset], elemLength);
            byteOffset += (int32_t)sizeof(int32_t);
            MMIJavaLog(("getPimListNames byteOffset2 = %d", byteOffset));

            CRTL_memcpy((char *)&array_ptr[byteOffset], pimList[i].name, elemLength);
            byteOffset += elemLength;
            MMIJavaLog(("getPimListNames byteOffset3 = %d", byteOffset));
        }
    }
    CRTL_memcpy((char*)namesArray->data,array_ptr,byteOffset);
    SCI_FREE(array_ptr);

    return namesArray;
}

/* See cpl_pim.h. */
int32_t CPL_pim_listPIMLists(int32_t pimListType)
{
    BYTEARRAY result = NULL;

    MMIJavaLog(("CPL_pim_listPIMLists(%d)", pimListType));
    switch(pimListType)
    {
        case CONTACT_LIST:
             result = getPimListNames(s_categorydefs, TABLE_SIZE(s_categorydefs));
             break;
        case EVENT_LIST:
        case TODO_LIST:
             break;
        default:
           MMIJavaLog(("CPL_pim_listPIMLists return error."));
           return CPL_PIM_ERROR;
    }
    if (result != NULL)
    {
        MMIJavaLog(("CPL_pim_listPIMLists return success."));
        return (int32_t)result;
    }

    MMIJavaLog(("CPL_pim_listPIMLists return error."));
    return CPL_PIM_ERROR;
}

static char *addUIDtoVcardBuffer(char *vcard, int32_t uid ,int flag,char *spetel)
{
    char uidStr[VCARD_UID_STRLEN] = {0};
    int oldLen = 0;
    int newLen = 0;
    int uidStrLen = 0 ;
    char *newVcard = NULL;
    int fixCharLen = 0;
    int i = 0 ;


    CRTL_sprintf(uidStr, VCARD_UID, uid);
    uidStrLen = CRTL_strlen(uidStr);
    oldLen = CRTL_strlen(vcard);
    newLen = oldLen + uidStrLen;
    fixCharLen = CRTL_strlen(VCARD_PREFIX)+CRTL_strlen(VCARD_VERSION);

    newVcard = SCI_ALLOCA(newLen);
    if(NULL == newVcard)
    {
        return NULL;
    }
    CRTL_memset(newVcard, 0, newLen);

    for (i=0;i<fixCharLen;i++)
    {
        newVcard[i] = vcard[i];
    }

    for (i = 0;i<uidStrLen;i++)
    {
        newVcard[i + fixCharLen] = uidStr[i];
    }

    for (i = fixCharLen ; i< oldLen; i++)
    {
        newVcard[i + uidStrLen] = vcard[i];
    }
    newVcard[newLen - 1] = '\0';


#if defined MMI_VCARD_SUPPORT
    MMIAPIVCARD_BuildFree();
#endif
    return newVcard;
}

static char *getVcardBufferByUidMap(UID_MAPPING_T *uidMap)
{
    char *ret_ptr = NULL;
    char *old_vcard_ptr = NULL;
    char tel[MMIPB_BCD_NUMBER_MAX_LEN]={0};
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
    MMIPB_CONTACT_T contact_ptr;
    MMIPB_CONTACT_T contact_ptr_tmp;
    int flag = 0;
    int len = 0 ;
    SCI_MEMSET(&contact_ptr, 0X00, sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(&contact_ptr_tmp, 0X00, sizeof(MMIPB_CONTACT_T));
#if defined MMI_VCARD_SUPPORT
    MMIAPIVCARD_BuildInit();
#endif

    if(MMIPB_ERROR_SUCCESS != MMIAPIPB_GetContactInfo(uidMap->entry_id,uidMap->storage, &contact_ptr))
    {
        return NULL;
    }
    
    // phonebook just support 20 length. need cut off length.
    if (contact_ptr.number[0].number_len > MMIPB_BCD_NUMBER_MAX_LEN)
    {
        contact_ptr.number[0].number_len = MMIPB_BCD_NUMBER_MAX_LEN;
        contact_ptr.number[0].number[MMIPB_BCD_NUMBER_MAX_LEN-1] = '\0';
    }
    
    //**************************************************************************************************
    //special phone number and the letter '&' neends to be removed according to
    //the "CPL_pim_storeItem#phone number filter part"
    if (contact_ptr.number[0].number[0]=='&')
    {
        if(MMIPB_ERROR_SUCCESS != MMIAPIPB_GetContactInfo(uidMap->entry_id,uidMap->storage, &contact_ptr_tmp))
        {
            return NULL;
        }
        else
        {
            flag =1;
            CRTL_memcpy(tel,contact_ptr_tmp.number[0].number+1,contact_ptr_tmp.number[0].number_len-1);
            tel[contact_ptr_tmp.number[0].number_len-1]='\0';
        }
    }

    old_vcard_ptr = (char *)SCI_ALLOCA(VCARD_MAX_FILE_LEN);
    if(PNULL == old_vcard_ptr)
    {
        MMIJavaLog(("getVcardBufferByUidMap, error: malloc failed!"));
        return NULL;
    }
    SCI_MEMSET(old_vcard_ptr, 0, VCARD_MAX_FILE_LEN);

    do
    {
#if defined MMI_VCARD_SUPPORT
        //****************************************************************************************************
        MMIAPIVCARD_ContactToVcard(&contact_ptr , VCARD_MAX_FILE_LEN , old_vcard_ptr, (uint32*)&len );
        MMIJavaLog(("--len:%d",len));
        //CRTL_strlen("BEGIN:VCARD\r\nVERSION:2.1\r\nEND:VCARD\r\n") == 37
        //if vcard buffer is empty buffer
        if(len<38)
        {
            break;
        }
#else
        //TO DO: convert contact data to Vcard format and stored into oldVcard buffer
        break;
#endif

        MMIJavaLog(("oldVcard:%s", old_vcard_ptr));
        ret_ptr = addUIDtoVcardBuffer(old_vcard_ptr, uidMap->uId, flag, tel);
    } while(0);

    SCI_FREE(old_vcard_ptr);
    return ret_ptr;
}


static uint16 getNextContactEntryID(uint16 curEntryId, int32_t storage, int32_t group_id)
{
        uint16 nextEntryId = 0;
        uint16 i;
        MMIPB_CONTACT_T *contact_ptr = PNULL;
        MMIPB_STORAGE_INFO_T storageInfo;
        int32_t storage_type = PHONEBOOK_NV_STORAGE;
        int32_t sim_id = MN_DUAL_SYS_1;

        if (group_id == PB_GROUP_SIM_1)
        {
            sim_id = MN_DUAL_SYS_1;
            storage_type = PHONEBOOK_SIM_STORAGE;
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        //SIM?¡§2
        if (group_id == PB_GROUP_SIM_2)
        {
            sim_id = MN_DUAL_SYS_2;
            storage_type = PHONEBOOK_SIM_STORAGE;
        }
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        //SIM?¡§3
        if (group_id == PB_GROUP_SIM_3)
        {
            sim_id = MN_DUAL_SYS_3;
            storage_type = PHONEBOOK_SIM_STORAGE;
        }
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        //SIM?¡§4
        if (group_id == PB_GROUP_SIM_4)
        {
            sim_id = MN_DUAL_SYS_4;
            storage_type = PHONEBOOK_SIM_STORAGE;
        }
#endif
#endif

    storageInfo = MMIPB_GetContactStorage(sim_id, storage_type);

    contact_ptr = (MMIPB_CONTACT_T*)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        MMIJavaLog(("getNextContactEntryID, error: malloc failed!"));
        return nextEntryId;
    }

    MMIJavaLog(("getNextContactEntryID, sim_id:%d, storage:%d", sim_id, storage));
    MMIJavaLog(("getNextContactEntryID, storageInfo.is_valid:%d", storageInfo.is_valid));
    MMIJavaLog(("getNextContactEntryID, storageInfo.max_record_num:%d, sizeof(MMIPB_CONTACT_T):%d",
        storageInfo.max_record_num, sizeof(MMIPB_CONTACT_T)));
    for (i = curEntryId + 1; i <= storageInfo.max_record_num; i++)
    {
        SCI_MEMSET(contact_ptr, 0, sizeof(MMIPB_CONTACT_T));
        if(MMIPB_ERROR_SUCCESS == MMIAPIPB_GetContactInfo(i,storage, contact_ptr))
        {
            nextEntryId = i;
            break;
        }
    }

    SCI_FREE(contact_ptr);
    MMIJavaLog(("getNextContactEntryID, nextEntryId:%d", nextEntryId));
    return nextEntryId;
}

static UID_MAPPING_T *getUidMapByEntryId(uint16 entryId, int32_t storage, int32_t group_id, UID_MAPPING_T *header)
{
    UID_MAPPING_T *uidMap = NULL;

    uidMap = header;
    while(uidMap)
    {
        if(uidMap->entry_id == entryId && uidMap->storage == storage && uidMap->group_id == group_id)
        {
            break;
        }
        uidMap = uidMap->next;
    }

    return uidMap;
}

static long addUidMap(UID_MAPPING_T *uidMap, UID_MAPPING_T **header)
{
    UID_MAPPING_T *tmpUidMap = *header;
    int32_t maxUid = 0;
    uint32_t uid = 0;

    //get new uid
    while(tmpUidMap)
    {
        if(tmpUidMap == uidMap)
        {
            return tmpUidMap->uId;
        }

        maxUid = (tmpUidMap->uId > maxUid) ? tmpUidMap->uId : maxUid;

        tmpUidMap = tmpUidMap->next;
    }

    maxUid++;
#ifdef MMIPB_UID_SUPPORT
    if(MMIPB_STORAGE_PHONE == uidMap->storage)
    {
        if(MMIAPIPB_GetUidByContactId(uidMap->entry_id, uidMap->storage, &uid) == TRUE)
        {
            uidMap->uId = uid;
        }
        else
        {
            uidMap->uId = maxUid;
        }

    }
    else
    {
        uidMap->uId = maxUid;
    }
#else//MMIPB_UID_SUPPORT
    uidMap->uId = maxUid;
#endif//MMIPB_UID_SUPPORT
    uidMap->next = *header;
    *header = uidMap;

    return (long)maxUid;
}

static void deleteUidMap(UID_MAPPING_T *uidMap, UID_MAPPING_T **header)
{
    UID_MAPPING_T *prevUidMap = *header;
    UID_MAPPING_T *tmpUidMap = *header;

    while(tmpUidMap)
    {
        if(tmpUidMap == uidMap)
        {
            if(tmpUidMap == prevUidMap)
            {
                *header = uidMap->next;
            }
            else
            {
                prevUidMap->next = uidMap->next;
            }
            break;
        }
        prevUidMap = tmpUidMap;
        tmpUidMap = tmpUidMap->next;
    }
}

static char *convertUcs2ToUtf8(STRING_INSTANCE string)
{
    int   len;
    char *retStr = NULL;
    if (string != NULL)
    {
        len = Jbni_getStringUtf8(string, NULL, 0);

        if (len > 0)
        {
            retStr = SCI_ALLOCA(len);
            if (retStr == NULL)
            {
                return NULL;
            }
            CRTL_memset(retStr, 0, len);
            Jbni_getStringUtf8(string, retStr, len);
        }
        else
        {
            /* Return empty string */
            retStr = SCI_ALLOCA(1 * sizeof(char));
            if (retStr == NULL)
            {
                return NULL;
            }
            retStr[0] = '\0';
        }
    }

    return retStr;
}

static uint32_t convertUcs2ToLong(STRING_INSTANCE string)
{
    uint32_t value = 0L;
    char *buffer = convertUcs2ToUtf8(string);
    if(NULL != buffer)
    {
        value = (uint32_t)CRTL_atoi(buffer);
        SCI_FREE(buffer);
        buffer=NULL;
    }

    return value;
}

static UID_MAPPING_T *getUidMapByUid(long uid, UID_MAPPING_T *header)
{
    UID_MAPPING_T *uidMap = NULL;

    uidMap = header;
    while(uidMap)
    {
        if(uidMap->uId == (int32_t)uid)
        {
            break;
        }
        uidMap = uidMap->next;
    }

    return uidMap;
}

int32_t getNextContactItem(
               struct INSTANCE_com_jbed_pim_PIMItemImpl_s * curItem,
               PIMLIST_T *pimListHandle)
{
    UID_MAPPING_T *uidMapHeader = pimListHandle->uidMapHeader;
    UID_MAPPING_T *nextUidMap;
    int32_t storage;
    int32_t group_id;
    uint16 curEntryId, nextEntryId;
    long curUid = -1L;
    char *vcardBuffer = NULL;
    int vCardBufLen = 0;
    BYTEARRAY itemArray = NULL;
    BOOLEAN ifExist = FALSE;
    char * itemArray_data =NULL;
    int str_len =0;

    MMIJavaLog(("getNextContactItem curItem:%p",curItem));

    if (curItem != NULL)
    {
        MMIJavaLog(("getNextContactItem curItem->itemUID:%p",curItem->itemUID));
        if(curItem->itemUID == NULL)
        {
            return CPL_PIM_ERROR;
        }

        curUid = convertUcs2ToLong(curItem->itemUID);
    }

    MMIJavaLog(("getNextContactItem curUid:%d",curUid));
    if(-1 == curUid)
    {
        switch(pimListHandle->category->type)
        {
        case PHONE:
            storage = MMIPB_STORAGE_PHONE;
            //storage = PHONEBOOK_NV_STORAGE;
            group_id = PB_GROUP_ALL;
            break;

        case SIM:
            storage = MMIPB_STORAGE_SIM1;
            //storage = PHONEBOOK_SIM_STORAGE;
            group_id = PB_GROUP_SIM_1;
            break;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case SIM2:
            storage = MMIPB_STORAGE_SIM2;
            //storage = PHONEBOOK_SIM_STORAGE;
            group_id = PB_GROUP_SIM_2;
            break;
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case SIM3:
            storage = MMIPB_STORAGE_SIM3;
            //storage = PHONEBOOK_SIM_STORAGE;
            group_id = PB_GROUP_SIM_3;
            break;
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case SIM4:
            storage = MMIPB_STORAGE_SIM4;
            //storage = PHONEBOOK_SIM_STORAGE;
            group_id = PB_GROUP_SIM_4;
            break;
#endif
#endif
        default:
            return CPL_PIM_ERROR;
        }
        curEntryId = 0;
    }
    else
    {
        UID_MAPPING_T *curUidMap = getUidMapByUid(curUid, uidMapHeader);

        MMIJavaLog(("getNextContactItem curUidMap:%p",curUidMap));
        if(NULL == curUidMap)
        {
            return CPL_PIM_ERROR;
        }
        storage = curUidMap->storage;
        group_id = curUidMap->group_id;
        curEntryId = curUidMap->entry_id;
    }

    do
    {
        MMIJavaLog(("getNextContactItem curEntryId:%d, storage:%d, group_id:%d",curEntryId, storage, group_id));
        nextEntryId = getNextContactEntryID(curEntryId, storage, group_id);
        MMIJavaLog(("getNextContactItem nextEntryId:%d",nextEntryId));
        if(0 == nextEntryId)
        {
            return CPL_PIM_ERROR;
        }
    }while(0);

    nextUidMap = getUidMapByEntryId(nextEntryId, storage, group_id, uidMapHeader);
    MMIJavaLog(("getNextContactItem nextUidMap:%p",nextUidMap));
    if(NULL == nextUidMap)
    {
        //new uid map
        nextUidMap = SCI_ALLOCA(sizeof(UID_MAPPING_T));
        if(NULL == nextUidMap)
        {
            return CPL_PIM_ERROR;
        }
        CRTL_memset(nextUidMap, 0, sizeof(UID_MAPPING_T));
        nextUidMap->storage = storage;
        nextUidMap->group_id = group_id;
        nextUidMap->entry_id = nextEntryId;
        addUidMap(nextUidMap, &(pimListHandle->uidMapHeader));
    }
    else
    {
        ifExist = TRUE;
    }

    //get vcard buffer
    vcardBuffer = getVcardBufferByUidMap(nextUidMap);
    MMIJavaLog(("vcardbuffer:%s",vcardBuffer));
    //    vcardBuffer = fakevcard2;
    if(NULL == vcardBuffer)
    {
        if(TRUE != ifExist)
        {
            //new uid map
            deleteUidMap(nextUidMap, &(pimListHandle->uidMapHeader));
            SCI_FREE(nextUidMap);
            nextUidMap=NULL;
        }
        MMIJavaLog(("getNextContactItem vcardBuffer is NULL"));
        return CPL_PIM_ERROR;
    }


    vCardBufLen = CRTL_strlen(vcardBuffer);

    itemArray = (BYTEARRAY)Jbni_newByteArray(vCardBufLen + (int32_t)sizeof(vCardBufLen));
    if (NULL == itemArray )
    {
        if(TRUE != ifExist)
        {
            //new uid map
            deleteUidMap(nextUidMap, &(pimListHandle->uidMapHeader));
            SCI_FREE(nextUidMap);
            nextUidMap=NULL;
        }
        SCI_FREE(vcardBuffer);
        Jbni_raiseOutOfMemoryError();
        return CPL_PIM_ERROR;
    }
    itemArray_data = SCI_ALLOCA(vCardBufLen + (int32_t)sizeof(vCardBufLen) +1);
    if(itemArray_data ==NULL)
    {
        if(TRUE != ifExist)
        {
            //new uid map
            deleteUidMap(nextUidMap, &(pimListHandle->uidMapHeader));
            SCI_FREE(nextUidMap);
            nextUidMap=NULL;
        }
        SCI_FREE(vcardBuffer);
        return CPL_PIM_ERROR;
    }
    CRTL_memset(itemArray_data,0,vCardBufLen + (int32_t)sizeof(vCardBufLen) +1);
    Jbed_writeInt((char*)itemArray_data,vCardBufLen);
    CRTL_memcpy((void*)(itemArray_data + (int)sizeof(vCardBufLen)),vcardBuffer,(int)vCardBufLen);
    str_len = (int)sizeof(vCardBufLen) + vCardBufLen ;    //CRTL_strlen(itemArray_data);
    CRTL_memcpy((void*)itemArray->data,itemArray_data,str_len);
    SCI_FREE(vcardBuffer);
    SCI_FREE(itemArray_data);
    vcardBuffer=NULL;

    MMIJavaLog(("getNextContactItem SUCCESS"));
    return (int32_t)itemArray;
}

/* See cpl_pim.h. */
int32_t CPL_pim_getNextItem(
              struct INSTANCE_com_jbed_pim_PIMItemImpl_s * curItem,
              int32_t listHandle)
{
    PIMLIST_T *pimListHandle = (PIMLIST_T *)listHandle;
    int32_t ret ;

    if(NULL == pimListHandle)
    {
        return CPL_PIM_ERROR;
    }

    switch(pimListHandle->type)
    {
    case CONTACT_LIST:
        ret = getNextContactItem(curItem, pimListHandle);
        break;

    default:
        ret = CPL_PIM_ERROR;
        break;
    }

    return ret;
}

/* See cpl_pim.h. */
int32_t CPL_pim_getCategories(int32_t pimListHandle)
{
    MMIJavaLog(("CPL_pim_getCategories(%d)", pimListHandle));
    return (int32_t)Jbni_newByteArray(0);
}

/* See cpl_pim.h. */
int32_t CPL_pim_getItemCategories(struct INSTANCE_com_jbed_pim_PIMItemImpl_s * pimItem)
{

    MMIJavaLog(("CPL_pim_getItemCategories(%p)", pimItem));
    return (int32_t)Jbni_newByteArray(0);
}

char * addtoVcardBuffer(BYTEARRAY vData,int * newBufferlength, char * spetel)
{
    char * newBuffer;
    char tel[3][MMIPB_BCD_NUMBER_MAX_LEN]={0};
    char tel_def[MMIPB_BCD_NUMBER_MAX_LEN]={0};

    int hastel=0;
    int i=0,j=0,z=0,length=vData->length;
    int counter=0;
    int numofdiftel=0;
    int work=0,cell=0,home=0;
    int flagwork=0,flagcell=0,flaghome=0;
    int telattr=0;
    int telworklen=0,telcelllen=0,telhomelen=0;
    int buildflag=1;
    int str_len = 0;
    BOOLEAN is_found_name = FALSE;

    //for pc lint
    char * vData_data = NULL;

    vData_data = SCI_ALLOCA(vData->length + 1);
    if(vData_data ==NULL)
    {
        return NULL;
    }
    CRTL_memset(vData_data,0,vData->length + 1);
    CRTL_memcpy(vData_data,vData->data,vData->length);
    //over

    for (i=0;i<vData->length;i++)
    {
        if (vData_data[i]!='\n')
            continue;
        else
        {
            if(i+2 <vData->length   &&
               vData_data[i+1]=='F'&&
               vData_data[i+2]=='N')
            {
                length=length+(int)CRTL_strlen(VCARD_CHARSET_UTF8_)-1;
                is_found_name = TRUE;
                continue;
            }
            if (i+3 < vData->length  &&
                vData_data[i+1]=='T'&&
                vData_data[i+2]=='E'&&
                vData_data[i+3]=='L')
            {
                numofdiftel++;
                hastel=1;
                i+=4;
                if (i < vData->length && vData_data[i]==':')
                {
                    telattr=0;
                    length=length + (int)CRTL_strlen(VCARD_HOME_VOICE_);
                    i++;
                    z=0;
                    while (i < vData->length && vData_data[i]!='\r')
                    {
                      tel_def[z++]=vData_data[i++];
                    }
                    tel_def[z]='\0';

                }
                else if (i<vData->length && vData_data[i]==';')
                {
                    telattr=1;
                    i++;

                    while (1)/*lint !e716*/
                    {
                        if (i + 3 <vData->length&&
                            vData_data[i]=='W'  &&
                            vData_data[i+1]=='O'&&
                            vData_data[i+2]=='R'&&
                            vData_data[i+3]=='K')
                        {
                            work=1;flagwork=1;
                        }
                        else if (i + 3 <vData->length&&
                                 vData_data[i]=='C'  &&
                                 vData_data[i+1]=='E'&&
                                 vData_data[i+2]=='L'&&
                                 vData_data[i+3]=='L')
                        {
                            cell = 1;
                            flagcell = 2;
                        }
                        else if (i + 3 <vData->length &&
                                 vData_data[i]=='H'  &&
                                 vData_data[i+1]=='O'&&
                                 vData_data[i+2]=='M'&&
                                 vData_data[i+3]=='E')
                        {
                            home = 1;
                            flaghome = 3;
                        }
                        i+=4;
                        if (vData_data[i]==':')
                        {
                            break;
                        }
                        else
                        {
                            i++;
                        }
                    }

                    i++;
                    //get the telnumber
                    z=0;
                    while (vData_data[i]!='\r')
                    {
                        if (flagwork)
                            tel[flagwork-1][z]=vData_data[i];
                        if (flagcell)
                            tel[flagcell-1][z]=vData_data[i];
                        if (flaghome)
                            tel[flaghome-1][z]=vData_data[i];
                        z++;
                        i++;
                    }
                    if (flagwork)
                    {
                        tel[flagwork-1][z]='\0';
                        telworklen=CRTL_strlen(tel[flagwork-1]);
                    }
                    if (flagcell)
                    {     tel[flagcell-1][z]='\0';

                        telcelllen=CRTL_strlen(tel[flagcell-1]);
                        CRTL_memcpy(spetel,tel[flagcell-1],telcelllen);
                        spetel[telcelllen]='\0';
                        MMIJavaLog(("spetel:%s",spetel));
                    }
                    if (flaghome)
                    {
                        tel[flaghome-1][z]='\0';
                        telhomelen=CRTL_strlen(tel[flaghome-1]);
                    }
                    flagwork=flagcell=flaghome=0;
                    continue;
                }

            }

            if (vData_data[i+1]=='E'&&
                vData_data[i+2]=='M'&&
                vData_data[i+3]=='A'&&
                vData_data[i+4]=='I'&&
                vData_data[i+5]=='L'&&
                vData_data[i+6]==':')
            {
                length=length + (int)CRTL_strlen(VCARD_PREF_INTERNET_);
                continue;
            }
            if (vData_data[i+1]=='T'&&
                vData_data[i+2]=='I'&&
                vData_data[i+3]=='T'&&
                vData_data[i+4]=='L'&&
                vData_data[i+5]=='E'&&
                vData_data[i+6]==':')
            {
                length=length + (int)CRTL_strlen(VCARD_CHARSET_UTF8_);
                continue;
            }
            if (vData_data[i+1]=='O'&&
              vData_data[i+2]=='R'&&
              vData_data[i+3]=='G'&&
              vData_data[i+4]==':')
            {
                length=length + (int)CRTL_strlen(VCARD_CHARSET_UTF8_);
                continue;
            }
            if (vData_data[i+1]=='A'&&
              vData_data[i+2]=='D'&&
              vData_data[i+3]=='R'&&
              vData_data[i+4]==':')
            {
                length=length + (int)CRTL_strlen(VCARD_WORK_CHARSET_UTF8_);
                continue;
            }
            if(vData_data[i+1]=='E'&&
               vData_data[i+2]=='N'&&
               vData_data[i+3]=='D'&&
               vData_data[i+4]==':')
            {
                if(!is_found_name)
                {
                    length=length + (int)CRTL_strlen("N;")+3;
                    length=length + (int)CRTL_strlen(VCARD_CHARSET_UTF8_)-1;
                    length=length + (int)CRTL_strlen(DEFAULT_NAME);
                    continue;
                }
            }
        }
    }
    if (hastel==0)
        length += (int)CRTL_strlen(DEFAULT_TEL);
    if (telattr==1)
    {
        length=length-
            (4*numofdiftel+(home+cell+work)*5+2*numofdiftel)+
            (4+5+6+telworklen+2)*work+
            (4+5+6+telcelllen+2)*cell+
            (4+5+6+telhomelen+2)*home;

    }
    newBuffer=SCI_ALLOCA(length);
    CRTL_memset(newBuffer,0,length);

    for (i=0;i<vData->length;i++)
    {
        if (buildflag)
            newBuffer[j++]=vData_data[i];
        else buildflag=1;

        if (vData_data[i]!='\n')
        {

            continue;
        }
        else
        {
            counter++;
            if (counter==2&&hastel==0)
            {
                str_len = CRTL_strlen(DEFAULT_TEL);
                CRTL_memcpy(newBuffer+j,DEFAULT_TEL,str_len);
                j+= str_len;

                i--;
                buildflag=0;
                continue;
            }

            if(vData_data[i+1]=='F'&&
                 vData_data[i+2]=='N')
            {
                newBuffer[j++]='N';
                newBuffer[j++]=';';
                i+=3;
                str_len = (int)CRTL_strlen(VCARD_CHARSET_UTF8_);
                CRTL_memcpy(newBuffer+j,VCARD_CHARSET_UTF8_,str_len);
                j+= str_len;
                continue;
            }
            if (vData_data[i+1]=='T'&&
                vData_data[i+2]=='E'&&
                vData_data[i+3]=='L')
            {

                if (vData_data[i+4]==':')
                {
                    newBuffer[j++]='T';
                    newBuffer[j++]='E';
                    newBuffer[j++]='L';
                    newBuffer[j++]=';';
                    i+=4;
                    str_len = (int)CRTL_strlen(VCARD_HOME_VOICE_);
                    CRTL_memcpy(newBuffer+j,VCARD_HOME_VOICE_,str_len);
                    j+= str_len;
                    str_len = (int)CRTL_strlen(tel_def);
                    CRTL_memcpy(newBuffer+j,tel_def,str_len);
                    j+= str_len;

                    i+= (int)CRTL_strlen(tel_def);
                    continue;
                }
                else if (vData_data[i+4]==';')
                {

                    if (work)
                    {
                        newBuffer[j++]='T';
                        newBuffer[j++]='E';
                        newBuffer[j++]='L';
                        newBuffer[j++]=';';
                        str_len = (int)CRTL_strlen(VCARD_WORK_VOICE_);
                        CRTL_memcpy(newBuffer+j,VCARD_WORK_VOICE_,str_len);
                        j+= str_len;
                        str_len = CRTL_strlen(tel[0]);
                        CRTL_memcpy(newBuffer+j,tel[0],str_len);
                        j+= str_len;
                        newBuffer[j++]='\r';
                        newBuffer[j++]='\n';
                    }
                    if (cell)
                    {
                        newBuffer[j++]='T';
                        newBuffer[j++]='E';
                        newBuffer[j++]='L';
                        newBuffer[j++]=';';
                        str_len = (int)CRTL_strlen(VCARD_CELL_VOICE_);
                        CRTL_memcpy(newBuffer+j,VCARD_CELL_VOICE_,str_len);
                        j+= str_len;
                        str_len = (int)CRTL_strlen(tel[1]);
                        CRTL_memcpy(newBuffer+j,tel[1],str_len);
                        j+= str_len;
                        newBuffer[j++]='\r';
                        newBuffer[j++]='\n';

                    }

                    if (home)
                    {
                        newBuffer[j++]='T';
                        newBuffer[j++]='E';
                        newBuffer[j++]='L';
                        newBuffer[j++]=';';
                        str_len = (int)CRTL_strlen(VCARD_HOME_VOICE_);
                        CRTL_memcpy(newBuffer+j,VCARD_HOME_VOICE_,str_len);
                        j+= str_len;
                        str_len = (int)CRTL_strlen(tel[2]);
                        CRTL_memcpy(newBuffer+j,tel[2],str_len);
                        j+= str_len;
                        newBuffer[j++]='\r';
                        newBuffer[j++]='\n';
                    }
                    while (numofdiftel!=0)
                    {
                        i++;
                        while (vData_data[i]!='\r')
                          i++;

                        numofdiftel--;
                        buildflag=0;
                    }

                }
            }
            if (vData_data[i+1]=='E'&&
                vData_data[i+2]=='M'&&
                vData_data[i+3]=='A'&&
                vData_data[i+4]=='I'&&
                vData_data[i+5]=='L'&&
                vData_data[i+6]==':')
            {
                newBuffer[j++]='E';
                newBuffer[j++]='M';
                newBuffer[j++]='A';
                newBuffer[j++]='I';
                newBuffer[j++]='L';
                newBuffer[j++]=';';
                i+=6;
                str_len = (int)CRTL_strlen(VCARD_PREF_INTERNET_);
                CRTL_memcpy(newBuffer+j,VCARD_PREF_INTERNET_,str_len);
                j+= str_len;
                continue;
            }
            if (vData_data[i+1]=='T'&&
                vData_data[i+2]=='I'&&
                vData_data[i+3]=='T'&&
                vData_data[i+4]=='L'&&
                vData_data[i+5]=='E'&&
                vData_data[i+6]==':')
            {
                newBuffer[j++]='T';
                newBuffer[j++]='I';
                newBuffer[j++]='T';
                newBuffer[j++]='L';
                newBuffer[j++]='E';
                newBuffer[j++]=';';
                i+=6;
                str_len = (int)CRTL_strlen(VCARD_CHARSET_UTF8_);
                CRTL_memcpy(newBuffer+j,VCARD_CHARSET_UTF8_,str_len);
                j+= str_len;
                continue;
            }
            if (vData_data[i+1]=='O'&&
                vData_data[i+2]=='R'&&
                vData_data[i+3]=='G'&&
                vData_data[i+4]==':')
            {
                newBuffer[j++]='O';
                newBuffer[j++]='R';
                newBuffer[j++]='G';
                newBuffer[j++]=';';
                i+=4;
                str_len = (int)CRTL_strlen(VCARD_CHARSET_UTF8_);
                CRTL_memcpy(newBuffer+j,VCARD_CHARSET_UTF8_,str_len);
                j+= str_len;
                continue;
            }
            if (vData_data[i+1]=='A'&&
                vData_data[i+2]=='D'&&
                vData_data[i+3]=='R'&&
                vData_data[i+4]==':')
            {
                newBuffer[j++]='A';
                newBuffer[j++]='D';
                newBuffer[j++]='R';
                newBuffer[j++]=';';
                i+=4;
                str_len = (int)CRTL_strlen(VCARD_WORK_CHARSET_UTF8_);
                CRTL_memcpy(newBuffer+j,VCARD_WORK_CHARSET_UTF8_,str_len);
                j+= str_len;
                continue;
            }
            if(vData_data[i+1]=='E'&&
               vData_data[i+2]=='N'&&
               vData_data[i+3]=='D'&&
               vData_data[i+4]==':')
            {
                if(!is_found_name)
                {
                    newBuffer[j++]='N';
                    newBuffer[j++]=';';
                    str_len = (int)CRTL_strlen(VCARD_CHARSET_UTF8_);
                    CRTL_memcpy(newBuffer+j,VCARD_CHARSET_UTF8_,str_len);
                    j+= str_len;
                    str_len = (int)CRTL_strlen(DEFAULT_NAME);
                    CRTL_memcpy(newBuffer+j,DEFAULT_NAME,str_len);
                    j+= str_len;
                    newBuffer[j++]='\r';
                    newBuffer[j++]='\n';
                    continue;
                }
            }
        }
    }
    SCI_FREE(vData_data);
    newBuffer[j]='\0';
    *newBufferlength=CRTL_strlen(newBuffer);
    return newBuffer;
}
#ifndef PB_SUPPORT_LOW_MEMORY
void addUrlToContact(char* vcard,uint16 vcard_length, MMIPB_CONTACT_T* contact)
{
    uint16 i=0;
    uint16 j=0;
    uint16 for_length = 0;

    if (vcard == NULL || vcard_length < 13)
    {
        return;
    }

    for_length = vcard_length - 13; // "END:VCARD" length is 9 "URL:" length is 4

    for (i=0; i<for_length; i++)
    {
        if (vcard[i]=='U' && vcard[i+1]=='R' && vcard[i+2]=='L' && vcard[i+3]==':')
        {
            while(vcard[i+4+j] != '\r')
            {
                contact->url.wstr[j] = vcard[i+4+j];
                j++;
            }
            contact->url.wstr_len = j;
        }
    }

}

void addBrithToContact(char* vcard,uint32 vcard_length, MMIPB_CONTACT_T* contact)
{
    uint16 i=0;
    uint16 j=0;
    uint16 for_length = 0;

    if (vcard == NULL || vcard_length < 14)
    {
        return;
    }

    for_length = vcard_length - 14; // "END:VCARD" length is 9 "BDAY:" length is 5

    for (i=0; i<for_length; i++)
    {
        if (vcard[i]=='B' && vcard[i+1]=='D' && vcard[i+2]=='A' && vcard[i+3]=='Y' && vcard[i+4]==':')
        {
            while(vcard[i+5+j] != '\r' && j<8)
            {
                contact->birth.wstr[j] = vcard[i+5+j];
                j++;
            }
            contact->birth.wstr_len = j;
        }
    }
}
#endif//PB_SUPPORT_LOW_MEMORY

#ifdef AMS_TCK_RUNNER
void addDefulteDetailToContact(char* vcard,uint16 vcard_length, MMIPB_CONTACT_T* contact)
{
    uint16 i=0;
    uint16 j=0;
    uint16 for_length = 0;

    if (vcard == NULL || vcard_length < 13)
    {
        return;
    }

    for_length = vcard_length - 13; // "END:VCARD" length is 9 "TEL:" length is 4

    for (i=0; i<for_length; i++)
    {
        if (vcard[i]=='T' && vcard[i+1]=='E' && vcard[i+2]=='L' && (vcard[i+3]==':' || vcard[i+3]==';'))
        {
            // phonebook just support 20 length. need cut off length.
            if (contact->number[0].number_len > MMIPB_BCD_NUMBER_MAX_LEN)
            {
                contact->number[0].number_len = MMIPB_BCD_NUMBER_MAX_LEN;
                contact->number[0].number[MMIPB_BCD_NUMBER_MAX_LEN] = '\0';
            }
            return;
        }
    }
    contact->number[0].number[0] = 1;
    contact->number[0].number[1] = 2;
    contact->number[0].number[2] = 3;
    contact->number[0].number[3] = 0;
    contact->number[0].number_len = 4;
    contact->number[0].npi_ton = 1;
}
#endif

int32_t CPL_pim_storeItem(struct INSTANCE_com_jbed_pim_PIMItemImpl_s * pimItem)
{
    PIMLIST_T *listHandle = NULL;
    UID_MAPPING_T *uidMap;
    BYTEARRAY vData = NULL;
    int32_t uid;
    char* vcard_buffer =  NULL;
    MMIPB_CONTACT_T *vcard_contact = NULL;
    int storage_id = 0;
    int group_id = 0;

    if(NULL == pimItem ||
        NULL == pimItem->pimList ||
        NULL == pimItem->pimList->handle)
    {
        return CPL_PIM_ERROR;
    }

    listHandle = (PIMLIST_T *)pimItem->pimList->handle;

    MMIJavaLog(("CPL_pim_storeItem pimItem->itemUID:%p",pimItem->itemUID));

    if (pimItem->itemUID != NULL)
    {
        uid = convertUcs2ToLong(pimItem->itemUID);
    }
    else
    {
        uid = -1;
    }

    MMIJavaLog(("CPL_pim_storeItem uid:%d",uid));
    vData = pimItem->vData;

    vcard_buffer = SCI_ALLOCA(vData->length -1);
    if (NULL == vcard_buffer)
    {
        return CPL_PIM_ERROR;
    }
    CRTL_memset(vcard_buffer, 0 , vData->length - 1);
    CRTL_memcpy(vcard_buffer , vData->data , vData->length-2);

    vcard_contact = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if (NULL == vcard_contact)
    {
        SCI_FREE(vcard_buffer);
        return CPL_PIM_ERROR;
    }
    CRTL_memset(vcard_contact, 0 ,sizeof(MMIPB_CONTACT_T));

#ifdef MMI_VCARD_SUPPORT
    if (MMIPB_ERROR_SUCCESS != MMIAPIPB_VcardToContactForPim((uint8*)vcard_buffer, vData->length + 1, vcard_contact))
    {
        SCI_FREE(vcard_buffer);
        SCI_FREE(vcard_contact);
        return CPL_PIM_ERROR;
    }
#endif

#ifndef PB_SUPPORT_LOW_MEMORY
    addUrlToContact(vcard_buffer,vData->length-1,vcard_contact);
    addBrithToContact(vcard_buffer,vData->length-1,vcard_contact);
#endif//PB_SUPPORT_LOW_MEMORY

#ifdef AMS_TCK_RUNNER
    // when data from user doesn't include tel property, add default tel property, or add record would fail on Spreadtrum platform.
    addDefulteDetailToContact(vcard_buffer,vData->length-1,vcard_contact);
#endif

    SCI_FREE(vcard_buffer);

    MMIJavaLog(("CPL_pim_storeItem uidMap:%p\n", uidMap));
    switch(listHandle->category->type)
    {
    case PHONE:
        storage_id = MMIPB_STORAGE_PHONE;
        group_id = PB_GROUP_ALL;
        break;

    case SIM:
        storage_id = MMIPB_STORAGE_SIM1;
        group_id = PB_GROUP_SIM_1;
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case SIM2:
        storage_id = MMIPB_STORAGE_SIM2;
        group_id = PB_GROUP_SIM_2;
        break;
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    case SIM3:
        storage_id = MMIPB_STORAGE_SIM3;
        group_id = PB_GROUP_SIM_3;
        break;
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    case SIM4:
        storage_id = MMIPB_STORAGE_SIM3;
        group_id = PB_GROUP_SIM_4;
        break;
#endif
#endif
    default:
        return CPL_PIM_ERROR;
    }

    uidMap = getUidMapByUid((long)uid, listHandle->uidMapHeader);
    //new uid map
    if(uidMap == NULL)
    {
        uidMap = SCI_ALLOCA(sizeof(UID_MAPPING_T));

        if(NULL == uidMap)
        {
            SCI_FREE(vcard_contact);
            return CPL_PIM_ERROR;
        }

        CRTL_memset(uidMap, 0, sizeof(UID_MAPPING_T));

        vcard_contact->contact_id = 0;
        vcard_contact->storage_id = storage_id;
        vcard_contact->union_t.group = group_id;

        if(MMIPB_ERROR_SUCCESS != MMIAPIPB_AddContact(vcard_contact))
        {
            SCI_FREE(vcard_contact);
            return CPL_PIM_ERROR;
        }
        // fill the uidMap param for deleting or updating.
        uidMap->entry_id = vcard_contact->contact_id;
        uidMap->storage = vcard_contact->storage_id;
        uidMap->group_id = vcard_contact->union_t.group;

        SCI_FREE(vcard_contact);
        addUidMap(uidMap, &(listHandle->uidMapHeader));
    }
    else
    {
        vcard_contact->contact_id = uidMap->entry_id;
        vcard_contact->storage_id = uidMap->storage;
        vcard_contact->union_t.group = uidMap->group_id;

        if(MMIPB_ERROR_SUCCESS != MMIPB_ModifyContact(vcard_contact))
        {
            SCI_FREE(vcard_contact);
            return CPL_PIM_ERROR;
        }
    }

    s_pimresult.byteArrLength = 12;
    s_pimresult.byteArrAddr = (uint8_t*)jbedPimMallocMemory(s_pimresult.byteArrLength);

    if(s_pimresult.byteArrAddr != NULL)
    {
        // Write -1 (0xFFFFFFFFFFFFFFFF) to the first 8 bytes, cause REVISION is unsupported.
        CRTL_memset(s_pimresult.byteArrAddr, 0xFF, 8);
        CRTL_sprintf((char *)(s_pimresult.byteArrAddr + 8), "%d", uidMap->uId);
    }
    else
    {
        s_pimresult.byteArrLength = 0;
    }

    return (int32_t)(&s_pimresult);
}

static int32_t openContactList(STRING_INSTANCE pimListName)
{
    PIMLIST_PRT_T pimListHandle;
    char *tmpStr = NULL;
    int32_t count = TABLE_SIZE(s_categorydefs);
    int32_t i;

    if (NULL == pimListName)
    {
        tmpStr = (char *)s_categorydefs[0].name;
    }
    else
    {
        if (Jbni_getStringLength(pimListName) <= 0)
        {
            MMIJavaLog(("openContactList() invalid length"));
            return CPL_PIM_ERROR;
        }
        tmpStr = convertUcs2ToUtf8(pimListName);

        if (tmpStr == NULL)
        {
            MMIJavaLog(("openContactList() convertUcs2ToUtf8 failed"));
            return CPL_PIM_ERROR;
        }
    }

    MMIJavaLog(("openContactList() tmpStr = %s", tmpStr));

    for(i = 0; i < count; i++)
    {
        if(tmpStr == s_categorydefs[i].name ||
            0 == CRTL_strcmp(tmpStr, s_categorydefs[i].name))
        {
            break;
        }
    }
    if(count == i)
    {
        return CPL_PIM_ERROR; //name doesn't match
    }

    if(tmpStr != s_categorydefs[0].name)
    {
        SCI_FREE(tmpStr);
    }

    MMIJavaLog(("openContactList() i = %d", i));

    pimListHandle = jbedPimMallocMemory(sizeof(struct PimList_s));
    if(NULL == pimListHandle)
    {
        MMIJavaLog(("openContactList() malloc list failed"));
        return CPL_PIM_ERROR;
    }
    CRTL_memset(pimListHandle, 0, sizeof(PIMLIST_T));
    pimListHandle->category = &s_categorydefs[i];
    pimListHandle->type = CONTACT_LIST;
    pimListHandle->next = s_listroot;
    if(NULL != s_listroot)
    {
        s_listroot->prev = pimListHandle;
    }
    s_listroot = pimListHandle;

    s_pimresult.result = (int32_t)pimListHandle;
    s_pimresult.byteArrLength = CRTL_strlen(s_categorydefs[i].name);
    s_pimresult.byteArrAddr = (uint8_t*)jbedPimMallocMemory(s_pimresult.byteArrLength);

    if (s_pimresult.byteArrAddr == NULL)
    {
        MMIJavaLog(("openContactList() malloc failed"));
        return CPL_PIM_ERROR;
    }

    CRTL_memcpy(s_pimresult.byteArrAddr, s_categorydefs[i].name, s_pimresult.byteArrLength);

    MMIJavaLog(("openContactList() success, listHandle = %d", (int32_t)pimListHandle));
    return (int32_t) &s_pimresult;
}


/* See cpl_pim.h. */
int32_t CPL_pim_openPIMList(int32_t pimListType,
              STRING_INSTANCE pimListName)
{
    int32_t result = CPL_PIM_ERROR;

    MMIJavaLog(("CPL_pim_openPIMList list type:%d", pimListType));
    if (pimListName != NULL)
    {
        if (Jbni_getStringLength(pimListName) <= 0)
        {
            MMIJavaLog(("CPL_pim_openPIMList invalid length"));
            return CPL_PIM_ERROR;
        }
    }

    switch(pimListType)
    {
    case CONTACT_LIST:
        result = openContactList(pimListName);
        break;

    case EVENT_LIST:
        result = CPL_PIM_ERROR;
        break;

    case TODO_LIST:
        result = CPL_PIM_ERROR;
        break;
    default:
        result =  CPL_PIM_ERROR;
    }

    MMIJavaLog(("CPL_pim_openPIMList return %d", result));
    return result;
}

/* See cpl_pim.h. */
int32_t CPL_pim_closePIMList(int32_t listHandle)
{
    PIMLIST_T *templist=NULL;
    PIMLIST_T *pimListHandle = (PIMLIST_T *)listHandle;
    UID_MAPPING_T *uidMapHeader = pimListHandle->uidMapHeader;
    UID_MAPPING_T *UidMap, *tempUidMap;

    for (UidMap=uidMapHeader;NULL!=UidMap;)
    {
        tempUidMap=UidMap;
        UidMap=UidMap->next;
        SCI_FREE(tempUidMap);
        tempUidMap=NULL;
    }

    templist=s_listroot;
    while (NULL!=templist)
    {
        if (templist==pimListHandle)
        {

            if (templist==s_listroot)
                s_listroot=templist->next;
            if (NULL!=templist->prev)
                templist->prev->next=pimListHandle->next;
            else if(NULL!=templist->next)
                templist->next->prev=NULL;
            if (NULL!=templist->next)
                templist->next->prev=pimListHandle->prev;
            else if (NULL!=templist->prev)
                templist->prev->next=NULL;
            if (NULL==templist->next&&NULL==templist->prev)
            {
                templist=NULL;
                break;
            }
        }
        templist=templist->next;
    }

    jbedPimFreeMemory(pimListHandle);
    pimListHandle=NULL;

    return CPL_PIM_SUCCESS;
}

/* See cpl_pim.h. */
int32_t CPL_pim_addCategory(int32_t pimListHandle,
              STRING_INSTANCE category)
{

    /* TODO: Add host-specific implementation code here. */
    return CPL_PIM_ERROR;
}

/* See cpl_pim.h. */
int32_t CPL_pim_deleteCategory(int32_t pimListHandle,
                 STRING_INSTANCE category)
{

    /* TODO: Add host-specific implementation code here. */
    return CPL_PIM_ERROR;
}

/* See cpl_pim.h. */
int32_t CPL_pim_renameCategory(int32_t pimListHandle,
                 STRING_INSTANCE oldCategory, STRING_INSTANCE newCategory)
{

    MMIJavaLog(("CPL_pim_renameCategory pimListHandle=%d, oldCategory=0x%08X, newCategory=0x%08X\n", pimListHandle, oldCategory, newCategory));
    return CPL_PIM_SUCCESS;
}

/* See cpl_pim.h. */
int32_t CPL_pim_maxCategories(int32_t pimListHandle)
{

    PIMLIST_PRT_T list = (PIMLIST_PRT_T)pimListHandle;
    int32_t result = CPL_PIM_ERROR;

    MMIJavaLog(("CPL_pim_maxCategories(%d)", pimListHandle));
    switch(list->type)
    {
    case CONTACT_LIST:
        result = 0;
        break;

    case EVENT_LIST:
        break;

    case TODO_LIST:
        break;
    default:
        break;
    }

    return result;
}

static INTARRAY createIntJbedArray(const int32_t *initArr, int32_t count)
{
    int32_t  i;
    INTARRAY result = Jbni_newIntArray(count);

    if (result == NULL)
    {
        Jbni_raiseOutOfMemoryError();
        return NULL;
    }

    for (i = 0; i < count; i++)
    {
        result->data[i] = initArr[i];
    }

    return result;
}

/* See cpl_pim.h. */
int32_t CPL_pim_maxValues(int32_t pimListHandle, int32_t fieldID)
{
    PIMLIST_PRT_T list = (PIMLIST_PRT_T)pimListHandle;
    int32_t result = CPL_PIM_ERROR;

    MMIJavaLog(("CPL_pim_maxValues(%d, %d)", pimListHandle, fieldID));
    switch(list->type)
    {
    case CONTACT_LIST:
        switch(fieldID)
        {
        case CONTACT_TEL:
            if (list->category->type == PHONE)
            {
              result = TABLE_SIZE(s_telattributes_phone);
            }
            else
            {
              result = TABLE_SIZE(s_telattributes_sim);
            }
            break;
        default:
            result = JBED_MAX_VALUES;
            break;
        }
        break;

    case EVENT_LIST:
        break;

    case TODO_LIST:
        break;
    default:
      break;
    }

    return result;
}

/* See cpl_pim.h. */
int32_t CPL_pim_getSuppFields(int32_t pimListHandle)
{

    PIMLIST_PRT_T list = (PIMLIST_PRT_T)pimListHandle;
    int32_t result = 0;

    MMIJavaLog(("CPL_pim_getSuppFields(%d)", pimListHandle));
    switch(list->type)
    {
    case CONTACT_LIST:
        if (list->category->type == PHONE)
        {
            result = (int32_t)createIntJbedArray(s_supportedfields, TABLE_SIZE(s_supportedfields));
        }
        else
        {
            result = (int32_t)createIntJbedArray(s_supportedfieldssim, TABLE_SIZE(s_supportedfieldssim));
        }
        break;

    case EVENT_LIST:
        result = (int32_t)createIntJbedArray(NULL, 0);
        break;

    case TODO_LIST:
        result = (int32_t)createIntJbedArray(NULL, 0);
        break;
    default:
        break;
    }

    return result;
}

/* See cpl_pim.h. */
int32_t CPL_pim_getSuppRRuleFields(int32_t pimListHandle,
                   int32_t frequency)
{

    INTARRAY array = Jbni_newIntArray(1);

    if (array == NULL)
    {
        Jbni_raiseOutOfMemoryError();
        return 0;
    }
    UNUSED(frequency);/*lint !e522*/
    array->data[0] = 0;
    return (int32_t)array;
}

/* See cpl_pim.h. */
int32_t CPL_pim_getSuppAttrs(int32_t pimListHandle, int32_t fieldID)
{

    PIMLIST_PRT_T list = (PIMLIST_PRT_T)pimListHandle;
    int32_t result = 0;

    MMIJavaLog(("CPL_pim_getSuppAttrs(%d, %d)", pimListHandle, fieldID));
    switch(list->type)
    {
    case CONTACT_LIST:
        switch (fieldID)
        {
        case CONTACT_TEL:
            if (list->category->type == PHONE)
            {
              result = (int32_t)createIntJbedArray(s_telattributes_phone, TABLE_SIZE(s_telattributes_phone));
            }
            else
            {
              result = (int32_t)createIntJbedArray(s_telattributes_sim, TABLE_SIZE(s_telattributes_sim));
            }

            break;

        default:
            result = (int32_t)createIntJbedArray(NULL, 0);
            break;
        }
        break;

    case EVENT_LIST:
        result = (int32_t)createIntJbedArray(NULL, 0);
        break;

    case TODO_LIST:
        result = (int32_t)createIntJbedArray(NULL, 0);
        break;
    default:
        break;

    }
    return result;
}

/* See cpl_pim.h. */
int32_t CPL_pim_getSuppArrElems(int32_t pimListHandle,
                int32_t fieldID)
{

    PIMLIST_PRT_T list = (PIMLIST_PRT_T)pimListHandle;
    int32_t result = 0;

    MMIJavaLog(("CPL_pim_getSuppArrElems(%d, %d)", pimListHandle, fieldID));
    switch(list->type)
    {
    case CONTACT_LIST:
        switch (fieldID)
        {
        case CONTACT_NAME:
            {
                int32_t suppElems[TABLE_SIZE(s_suppnameelemmap)];
                int32_t i;

                for (i = 0; i < (int32_t)TABLE_SIZE(s_suppnameelemmap); i++)
                {
                    suppElems[i] = s_suppnameelemmap[i].pimCode;
                }

                result = (int32_t)createIntJbedArray(suppElems, TABLE_SIZE(s_suppnameelemmap));
            }
            break;

        case CONTACT_ADDR:
            {
                int32_t suppElems[TABLE_SIZE(s_suppaddrelemmap)];
                int32_t i;

                MMIJavaLog(("CPL_pim_getSuppArrElems addr size:%d", TABLE_SIZE(s_suppaddrelemmap)));
                for (i = 0; i < (int32_t)TABLE_SIZE(s_suppaddrelemmap); i++)
                {
                    suppElems[i] = s_suppaddrelemmap[i].pimCode;
                    MMIJavaLog(("CPL_pim_getSuppArrElems [%d] element:%d", i, suppElems[i]));
                }

                result = (int32_t)createIntJbedArray(suppElems, TABLE_SIZE(s_suppaddrelemmap));
            }
            break;
        default:
            result = (int32_t)createIntJbedArray(NULL, 0);
            break;
        }
        break;

    case EVENT_LIST:
        result = (int32_t)createIntJbedArray(NULL, 0);
        break;

    case TODO_LIST:
        result = (int32_t)createIntJbedArray(NULL, 0);
        break;
    default:
        break;
    }

    return result;
}

/* See cpl_pim.h. */
int32_t CPL_pim_itemMaxCategories(
                  struct INSTANCE_com_jbed_pim_PIMItemImpl_s * pimItem)
{
    return 0;
}

/* See cpl_pim.h. */
int32_t CPL_pim_removePIMItem(int32_t listHandle,
                STRING_INSTANCE uid)
{
    uint32_t Uid = 0;
    PIMLIST_T *pimListHandle = (PIMLIST_T *)listHandle;
    UID_MAPPING_T *uidMapHeader = pimListHandle->uidMapHeader;
    UID_MAPPING_T *UidMap;
    int32_t sim_id;
    MMIJavaLog(("CPL_pim_removePIMItem"));
    if (uid == NULL || Jbni_getStringLength(uid) <= 0)
    {

        return CPL_PIM_ERROR;
    }
    Uid = convertUcs2ToLong(uid);
    UidMap = getUidMapByUid(Uid, uidMapHeader);

    sim_id = MN_DUAL_SYS_1;

    switch(UidMap->group_id)
    {
    case PB_GROUP_SIM_1:
        sim_id = MN_DUAL_SYS_1;
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case PB_GROUP_SIM_2:
        sim_id = MN_DUAL_SYS_2;
        break;

#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    case PB_GROUP_SIM_3:
        sim_id = MN_DUAL_SYS_3;
        break;
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    case PB_GROUP_SIM_4:
        sim_id = MN_DUAL_SYS_4;
        break;
#endif
#endif
    default:
        sim_id = MN_DUAL_SYS_1;
        break;
    }

    if (MMIAPIPB_DeleteContact(UidMap->entry_id,UidMap->storage))
    {
        MMIJavaLog(("CPL_pim_removePIMItem error"));
        return CPL_PIM_ERROR;
    }

    deleteUidMap(UidMap, &uidMapHeader);
    MMIJavaLog(("CPL_pim_removePIMItem success"));
    return CPL_PIM_SUCCESS;
}

#endif //PDAP_PIM_SUPPORT