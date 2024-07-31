/*
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include "string.h"
#include "javacall_pim.h"
#include "javacall_logging.h"
#include "javacall_file.h"
#include "javacall_dir.h"
#include "mmijava_pim.h"
#include "javacall_memory.h"

extern javacall_result javautil_unicode_utf16_to_utf8(javacall_utf16*, javacall_int32, javacall_uint16*, javacall_int32, javacall_int32*);

extern javacall_result javautil_unicode_utf16_ulength(javacall_utf16*, javacall_int32*);

extern javacall_result javautil_unicode_utf8_to_utf16(char*, javacall_int32, javacall_uint16*, javacall_int32, javacall_int32*);


static javacall_pim_field pim_contact_fields[] = {
    // NAME field
    {
        // id
        JAVACALL_PIM_CONTACT_FIELD_NAME,
        // maxValues
        -1,
        // type
        JAVACALL_PIM_FIELD_TYPE_STRING_ARRAY,
        // label
        {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'N', 'a', 'm', 'e', 0},/*lint !e785 */
        // code
        {'N'},/*lint !e785 */
        // attributes
        0,
        // arrayElements
        {
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_NAME_FAMILY,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'N', 'a', 'm', 'e', '.', '0', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_NAME_GIVEN,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'N', 'a', 'm', 'e', '.', '1', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_NAME_OTHER,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'N', 'a', 'm', 'e', '.', '2', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_NAME_PREFIX,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'N', 'a', 'm', 'e', '.', '3', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_NAME_SUFFIX,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'N', 'a', 'm', 'e', '.', '4', 0}/*lint !e785 */
            },
            {JAVACALL_PIM_INVALID_ID, {0}}
        },/*lint !e785 */
    },
    // ADDR field
    {
        // id
        JAVACALL_PIM_CONTACT_FIELD_ADDR,
        // maxValues
        -1,
        // type
        JAVACALL_PIM_FIELD_TYPE_STRING_ARRAY,
        // label
        {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'A', 'd', 'd', 'r', 0},/*lint !e785 */
        // code
        {'A','D','R'},/*lint !e785 */
        // attributes
        0x200,
        // arrayElements
        {
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_ADDR_POBOX,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'A', 'd', 'd', 'r', '.', '0', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_ADDR_EXTRA,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'A', 'd', 'd', 'r', '.', '1', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_ADDR_STREET,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'A', 'd', 'd', 'r', '.', '2', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_ADDR_LOCALITY,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'A', 'd', 'd', 'r', '.', '3', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_ADDR_REGION,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'A', 'd', 'd', 'r', '.', '4', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_ADDR_POSTALCODE,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'A', 'd', 'd', 'r', '.', '5', 0}/*lint !e785 */
            },
            {
                JAVACALL_PIM_CONTACT_FIELD_ARRAY_ELEMENT_ADDR_COUNTRY,
                {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'A', 'd', 'd', 'r', '.', '6', 0}/*lint !e785 */
            },
            {JAVACALL_PIM_INVALID_ID, {0}}
        },/*lint !e785 */
    },
    // EMAIL field
    {
        // id
        JAVACALL_PIM_CONTACT_FIELD_EMAIL,
        // maxValues
        -1,
        // type
        JAVACALL_PIM_FIELD_TYPE_STRING,
        // label
        {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'E', 'm', 'a', 'i', 'l', 0},/*lint !e785 */
        // code
        {'E','M','A','I','L'},/*lint !e785 */
        // attributes
        0x080,
        // arrayElements
        {
            {JAVACALL_PIM_INVALID_ID, {0}}
        },
    },
    // NOTE field
    {
        // id
        JAVACALL_PIM_CONTACT_FIELD_NOTE,
        // maxValues
        -1,
        // type
        JAVACALL_PIM_FIELD_TYPE_STRING,
        // label
        {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'N', 'o', 't', 'e', 0},/*lint !e785 */
        // code
        {'N','O','T','E'},/*lint !e785 */
        // attributes
        0,
        // arrayElements
        {
            {JAVACALL_PIM_INVALID_ID, {0}}
        },
    },
    // ORG field
    {
        // id
        JAVACALL_PIM_CONTACT_FIELD_ORG,
        // maxValues
        -1,
        // type
        JAVACALL_PIM_FIELD_TYPE_STRING,
        // label
        {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'O', 'r', 'g', 0},/*lint !e785 */
        // code
        {'O','R','G'},/*lint !e785 */
        // attributes
        0,
        // arrayElements
        {
            {JAVACALL_PIM_INVALID_ID, {0}}
        },
    },
    // TEL field
    {
        // id
        JAVACALL_PIM_CONTACT_FIELD_TEL,
        // maxValues
        -1,
        // type
        JAVACALL_PIM_FIELD_TYPE_STRING,
        // label
        {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'T', 'e', 'l', 0},/*lint !e785 */
        // code
        {'T','E','L'},/*lint !e785 */
        // attributes
        0x21C,//need to add VOICE tag!!!//0x23C,
        // arrayElements
        {
            {JAVACALL_PIM_INVALID_ID, {0}}
        },
    },
    // TITLE field
    {
        // id
        JAVACALL_PIM_CONTACT_FIELD_TITLE,
        // maxValues
        -1,
        // type
        JAVACALL_PIM_FIELD_TYPE_STRING,
        // label
        {'P', 'I', 'M', '.', 'C', 'o', 'n', 't', 'a', 'c', 't', 'L', 'i', 's', 't', '.', 'T', 'i', 't', 'l', 'e', 0},/*lint !e785 */
        // code
        {'T','I','T','L','E'},/*lint !e785 */
        // attributes
        0,
        // arrayElements
        {
            {JAVACALL_PIM_INVALID_ID, {0}}
        },
    }
};

static javacall_pim_field_attribute pim_attributes[] = {
    {
        JAVACALL_PIM_CONTACT_FIELD_ATTR_FAX,
        {'F', 'a', 'c', 's', 'i', 'm', 'i', 'l', 'i', 'e', 0}/*lint !e785 */
    },/*lint !e785 */
    {
        JAVACALL_PIM_CONTACT_FIELD_ATTR_HOME,
        {'H', 'o', 'm', 'e', 0}/*lint !e785 */
    },/*lint !e785 */
    {
        JAVACALL_PIM_CONTACT_FIELD_ATTR_MOBILE,
        {'M', 'o', 'b',  'i', 'l', 'e', 0}/*lint !e785 */
    },/*lint !e785 */
    {
        JAVACALL_PIM_CONTACT_FIELD_ATTR_PREFERRED,
        {'P', 'r','e', 'e', 'f', 'e', 'r', 'r', 'e', 'd', 0}/*lint !e785 */
    },/*lint !e785 */
    {
        JAVACALL_PIM_CONTACT_FIELD_ATTR_WORK,
        {'W', 'o', 'r', 'k', 0}/*lint !e785 */
    },/*lint !e785 */
    {
        JAVACALL_PIM_INVALID_ID,/*lint !e570 */
        {0}
    }/*lint !e785 */
};

typedef javacall_result (*java_pim_list_get_next_item)(unsigned char *item, 
    int maxItemLen, javacall_utf16 *categories, int maxCategoriesLen, 
    javacall_handle *itemHandle, int* index);

typedef javacall_result (*java_pim_list_modify_item)(javacall_handle itemHandle, 
    const unsigned char *item, const javacall_utf16 *categories);

typedef javacall_result (*java_pim_list_add_item)(const unsigned char *item,
    const javacall_utf16 *categories, javacall_handle *itemHandle);

typedef javacall_result (*java_pim_list_remove_item)(javacall_handle itemHandle);

typedef javacall_result (*java_pim_list_add_category)(javacall_utf16* categoryName);

typedef javacall_result (*java_pim_list_remove_category)(javacall_utf16* categoryName);

typedef javacall_result (*java_pim_list_rename_category)(
    javacall_utf16* oldCategoryName, javacall_utf16* newCategoryName);

typedef int (*java_pim_list_get_category_size)(void);

typedef javacall_result (*java_pim_list_get_categories)(
    javacall_utf16* categories, int max_categories_len);

static javacall_result java_pim_contact_get_next_item(unsigned char *item, 
    int maxItemLen, javacall_utf16 *categories, int maxCategoriesLen, 
    javacall_handle *itemHandle, int* index);

static javacall_result java_pim_contact_modify_item(javacall_handle itemHandle, 
    const unsigned char *item, const javacall_utf16 *categories);

static javacall_result java_pim_contact_add_item(const unsigned char *item, 
    const javacall_utf16 *categories, javacall_handle *itemHandle);

static javacall_result java_pim_contact_remove_item(javacall_handle itemHandle);

static int java_pim_contact_get_category_size();

typedef struct {
    javacall_pim_type       type;
    javacall_pim_open_mode  mode;
    char                    *name;
    int                     index;
    int                     default_index;
    int                     num_of_fields;
    javacall_pim_field*     fields;
    int                     num_of_attributes;
    javacall_pim_field_attribute    *attributes;
    java_pim_list_get_next_item     get_netx_item;
    java_pim_list_modify_item   modify_item;
    java_pim_list_add_item      add_item;
    java_pim_list_remove_item   remove_item;
    java_pim_list_get_category_size   get_category_size;
    java_pim_list_rename_category  rename_category;
    java_pim_list_get_categories     get_categories;
    java_pim_list_add_category     add_category;
    java_pim_list_remove_category  remove_category;
} pim_list_dscr;


static pim_list_dscr pim_lists[] = {
    {
        JAVACALL_PIM_TYPE_CONTACT,
        JAVACALL_PIM_OPEN_MODE_READ_WRITE,
        "Contacts",
        -1,
        -1,
        sizeof(pim_contact_fields) / sizeof(javacall_pim_field),
        pim_contact_fields,
        sizeof(pim_attributes) / sizeof(javacall_pim_field_attribute),
        pim_attributes,
        java_pim_contact_get_next_item,
        java_pim_contact_modify_item,
        java_pim_contact_add_item,
        java_pim_contact_remove_item,
        java_pim_contact_get_category_size,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

#define JAVA_PIM_LIST_SIZE  (sizeof(pim_lists) / sizeof(pim_list_dscr))

#define JAVA_PIM_NAME_MAX_LEN 512
static char* java_pim_utf16_to_utf8(const javacall_utf16* utf16,
                                 /*OUT*/ int* putf8len) {
    
    static char utf8[JAVA_PIM_NAME_MAX_LEN];
    javacall_int32 len = JAVA_PIM_NAME_MAX_LEN;
    int utf16Len;
    javacall_result res;
    if (JAVACALL_OK != javautil_unicode_utf16_ulength(utf16, &utf16Len)) {/*lint !e718 !e746 !e605*/
        return NULL;
    }
    res = javautil_unicode_utf16_to_utf8(utf16, utf16Len, (javacall_uint16*)utf8, len -1, &len);/*lint !e605 !e718 !e746 */
    if(res != JAVACALL_OK){
        utf8[0] = 0;
        return NULL;
    }
    utf8[len] = 0;

    if(putf8len != NULL){
        *putf8len = len;
    }

    return utf8;
}

static void java_pim_dbg_item(unsigned char* item) {
#ifdef  PIM_DEBUG
    int i;
    char str[1024];
    for(i = 0; i < JAVA_PIM_NAME_MAX_LEN && item[i] != 0; i++){
        javacall_printf("%.2X", item[i]);
        //str[i] = item[i];
    }
    //str[i] = 0;
    javacall_printf("[PIM][UTF8 - CODE][%s]\n",str);

    for(i = 0; i < JAVA_PIM_NAME_MAX_LEN && item[i] != 0; i++){
        //javacall_printf("%c", item[i]);
        str[i] = item[i];
    }
    str[i] = 0;
    javacall_printf("[PIM][UTF8 - STR(%d)][%s]\n", i, str);
#endif
}

static javacall_result java_pim_contact_get_next_item(unsigned char *item, 
    int maxItemLen, javacall_utf16 *categories, int maxCategoriesLen, 
    javacall_handle *itemHandle, int* index) {
    
    javacall_result res;

    res = MMIJAVA_PIM_Get_Next_Item(0, *index, (int32 *)itemHandle, item, (uint32)maxItemLen); 
    *index = (int)*itemHandle;
    java_pim_dbg_item(item);
    return res;
}

static javacall_result java_pim_contact_modify_item(javacall_handle itemHandle, 
    const unsigned char *item, const javacall_utf16 *categories) {
    java_pim_dbg_item(item);         /*lint !e605*/
    return MMIJAVA_PIM_Modify_Item(0, (int32)itemHandle, item);     /*lint !e605*/
}

static javacall_result java_pim_contact_add_item(const unsigned char *item, 
    const javacall_utf16 *categories, javacall_handle *itemHandle) {
    java_pim_dbg_item(item);        /*lint !e605*/
    return MMIJAVA_PIM_Add_Item(0, (int32 *)itemHandle, item);      /*lint !e605*/
}

static javacall_result java_pim_contact_remove_item(javacall_handle itemHandle)  {
    return MMIJAVA_PIM_Remove_Item(0, (int32)itemHandle); 
}

static int java_pim_contact_get_category_size() {
    return 0;
}

/**
 * Returns JAVACALL_PIM_STRING_DELIMITER separated list that contains the names
 * of PIM lists that match the given list type
 * (e.g., "Contact" Or "JohnContact\nSuziContact").
 *
 * @param listType Type of the PIM list the user wishes to obtain.
 * @param pimList Pointer to the buffer in which to store the returned list.
 *                The list must be delimited by JAVACALL_PIM_STRING_DELIMITER).
 *                The default list name should appear in the first place.
 * @param pimListLen Buffer size.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_FAIL when no list exists or when the buffer size is too
 *                       small.
 */
javacall_result javacall_pim_get_lists(javacall_pim_type listType,
    javacall_utf16 /*OUT*/ *pimList, int pimListLen) {

    uint32 i;
    int len;
    javacall_result res = JAVACALL_OK;
    javacall_utf16 *pCurrent = pimList;
    javacall_int32 delimiterLen = strlen(JAVACALL_PIM_STRING_DELIMITER);
    //javacall_int32 bufferLen = pimListLen;

    PIM_LOG_1("[javacall_pim_get_lists()]listType = 0X%X", listType);
    *pCurrent = 0;
    for(i = 0; i < JAVA_PIM_LIST_SIZE ; i++) {
        if(pim_lists[i].type == listType) {
            res = javautil_unicode_utf8_to_utf16(pim_lists[i].name,strlen((char*)pim_lists[i].name),pCurrent,pimListLen,&len);/*lint !e718 !e746 e!526*/
            if(JAVACALL_FAIL == res) {
                pCurrent[pimListLen - 1] = 0;
                PIM_LOG_0("[javacall_pim_get_lists()] FAIL 1");
                return JAVACALL_FAIL;
            }
            pimListLen -= len;
            pCurrent+=len;
            *pCurrent = 0;
            i++;
            break;
        }
    }

    for( ; (i < JAVA_PIM_LIST_SIZE) && (pimListLen > delimiterLen + 1) && (res == JAVACALL_OK); i++) {
        if(pim_lists[i].type != listType) {
            continue;
        }

        res = javautil_unicode_utf8_to_utf16(JAVACALL_PIM_STRING_DELIMITER, delimiterLen, pCurrent, pimListLen, &len);/*lint !e718 !e746 !e516*/
        if(JAVACALL_FAIL == res) {
            pCurrent[pimListLen - 1] = 0;
            PIM_LOG_0("[javacall_pim_get_lists()] FAIL 2");
            return JAVACALL_FAIL;
        }
        pimListLen -= len;
        pCurrent+=len;        
        res = javautil_unicode_utf8_to_utf16(pim_lists[i].name,strlen((char*)pim_lists[i].name),pCurrent,pimListLen,&len);/*lint !e718 !e746*/
        if(JAVACALL_FAIL == res) {
            pCurrent[pimListLen - 1] = 0;
            res = JAVACALL_FAIL;
        }
        else {
            pimListLen -= len;
            pCurrent+=len;
            *pCurrent = 0;
        }
    }
    if(i < JAVA_PIM_LIST_SIZE) {
        res = JAVACALL_FAIL;
    }
    PIM_LOG_1("[javacall_pim_get_lists()] res = %d", res);
    return res;
}

/**
 * Checks if a given PIM list type is supported by the platform.
 *
 * @return JAVACALL_TRUE if the list type is supported,
 *         JAVACALL_FALSE otherwise.
 */
javacall_bool javacall_pim_list_is_supported_type(javacall_pim_type listType) {
    uint32 i;

    PIM_LOG_1("[javacall_pim_list_is_supported_type()] listType = 0X%X", listType);
    for(i = 0; i < JAVA_PIM_LIST_SIZE; i++) {
        if(pim_lists[i].type == listType) {
            return JAVACALL_TRUE;
        }
    }
    return JAVACALL_FALSE;
}

/**
 * Opens the requested PIM list in the given mode.
 *
 * @param listType type of the PIM list to open.
 * @param pimListName the name of the list to open,
 *        if pimList is null the handle of default dummy
 *        list will be returned; this kind of list may
 *        be used only for getting default list structure.
 * @param mode the open mode for the list.
 * @param listHandle pointer to where to store the list handle.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_INVALID_ARGUMENT If an invalid mode is provided as a
 *         parameter or if listType is not a valid PIM list type
 * @retval JAVACALL_FAIL on other error.
 */
javacall_result javacall_pim_list_open(javacall_pim_type listType,
    javacall_utf16 *pimList, javacall_pim_open_mode mode,
    javacall_handle *listHandle) {

    uint32 i;

    PIM_LOG_3("[javacall_pim_list_open()] listType = 0X%X, pimList = %d, mode = %d", listType, pimList, mode);
    for(i = 0; i < JAVA_PIM_LIST_SIZE; i++) {
        char *name;
        if(pimList != NULL) {
            name = java_pim_utf16_to_utf8(pimList, NULL);
            PIM_LOG_1("[javacall_pim_list_open()] name = [%s]", name);
        } else {
            name = NULL;
        }
        if((pim_lists[i].type == listType)&& ((name == NULL) || (strcmp(name, pim_lists[i].name) == 0))) {
            pim_list_dscr *pl = (pim_list_dscr*) javacall_malloc (sizeof( pim_list_dscr));
            if(NULL == pl) {
                PIM_LOG_0("[javacall_pim_list_open()] out of memory!");
                return JAVACALL_FAIL;
            }
            pl->mode = mode;
            pl->index = pim_lists[i].default_index;
            memcpy (pl, &pim_lists[i], sizeof (pim_list_dscr));
            *listHandle = pl;
            return JAVACALL_OK;
        }
    }
    return JAVACALL_FAIL;
}

/**
 * Closes an open PIM list.
 *
 * @param listHandle a handle of the list to close.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_FAIL in case the list is no longer accessible.
 */
javacall_result javacall_pim_list_close(javacall_handle listHandle) {

    pim_list_dscr *pl = (pim_list_dscr*) listHandle;

    PIM_LOG_1("[javacall_pim_list_close()] listHandle = 0X%X", listHandle);

    if (pl == NULL) { 
        return JAVACALL_FAIL;
    }
    javacall_free (pl);
    return JAVACALL_OK;
}

/**
 * Returns the next item in the given PIM list.
 * For Contact item the item will be in vCard 2.1 / 3.0 format.
 * For Event/Todo item the item will be in vCalendar 1.0 format.
 *
 * @param listHandle handle of the list from which to get the item.
 * @param item pointer to a buffer in which to store the item.
 * @param maxItemLen the maximum size of the item.
 * @param categories pointer to a buffer in which to store the item's
 *                   categories separated by JAVACALL_PIM_STRING_DELIMITER.
 * @param maxCategoriesLen size of the categories buffer.
 * @param itemHandle pointer to where to store a unique identifier
 *                   for the returned item.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_INVALID_ARGUMENT  maxItemLen is too small
 * @retval JAVACALL_FAIL in case reached the last item in the list.
 */
javacall_result javacall_pim_list_get_next_item(javacall_handle listHandle,
    unsigned char *item, int maxItemLen, javacall_utf16 *categories,
    int maxCategoriesLen, javacall_handle *itemHandle) {

    javacall_result res;
    pim_list_dscr *pl = (pim_list_dscr*) listHandle;

    if(pl == NULL || pl->get_netx_item == NULL) {
        javacall_printf("[PIM][javacall_pim_list_get_next_item()] Failed\n");
        return JAVACALL_FAIL;
    }

    javacall_printf("[PIM][javacall_pim_list_get_next_item()][Before] index = %d\n", pl->index);
    res = pl->get_netx_item(item, maxItemLen, categories, maxCategoriesLen, itemHandle, &(pl->index));
    javacall_printf("[PIM][javacall_pim_list_get_next_item()] [After] index = %d, res = %d\n", pl->index, res);
    return res;
}

/**
 * Modifies an existing item.
 * For Contact item the item will be in vCard 2.1 / 3.0 format.
 * For Event/Todo item the item will be in vCalendar 1.0 format.
 *
 * @param listHandle handle to the list to which the item belongs.
 * @param itemHandle handle of the item to modify.
 * @param item pointer to the new item data.
 * @param categories pointer to the item's categories separated by comma.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_FAIL in case of an error.
 */
javacall_result javacall_pim_list_modify_item(javacall_handle listHandle,
    javacall_handle itemHandle, const unsigned char *item,
    const javacall_utf16 *categories) {

    pim_list_dscr *pl = (pim_list_dscr*) listHandle;
    javacall_result res;

    javacall_printf("[PIM][javacall_pim_list_modify_item()] listHandle = 0X%X, itemHandle = 0X%X\n", listHandle, itemHandle);

    if(pl == NULL || pl->modify_item == NULL) {
        javacall_printf("[PIM][javacall_pim_list_modify_item()] Failed\n");
        return JAVACALL_FAIL;
    }

    res = pl->modify_item(itemHandle, item, categories);
    javacall_printf("[PIM][javacall_pim_list_modify_item()] res = %d\n", res);
    return res;
}

/**
 * Adds a new item to the given item list.
 * For Contact item the item will be in vCard 2.1 / 3.0 format.
 * For Event/Todo item the item will be in vCalendar 1.0 format.
 *
 * @param listHandle handle to the list to which to add the new item.
 * @param item pointer to the item to add to the list.
 * @param categories pointer to the item's categories separated by comma.
 * @param itemHandle pointer to where to store a unique identifier
 *                   for the new item.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_FAIL in case of an error.
 */
javacall_result javacall_pim_list_add_item(javacall_handle listHandle,
    const unsigned char *item, const javacall_utf16 *categories,
    javacall_handle *itemHandle) {

    pim_list_dscr *pl = (pim_list_dscr*) listHandle;
    javacall_result res;
    javacall_printf("[PIM][javacall_pim_list_add_item()] listHandle = 0X%X", listHandle);

    if(pl == NULL || pl->add_item== NULL) {
        javacall_printf("[PIM][javacall_pim_list_add_item()] Failed\n");
        return JAVACALL_FAIL;
    }
    res = pl->add_item(item, categories, itemHandle);
    javacall_printf("[PIM][javacall_pim_list_add_item()] res = %d, itemHandle = 0X%X\n", res, *itemHandle);
    return res;
}

/**
 * Removes an item from the list.
 *
 * @param listHandle handle to the list from which to delete the item.
 * @param itemHandle handle to the item to remove.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_INVALID_ARGUMENT if either of the handles is not valid
 * @retval JAVACALL_FAIL in case of any error.
 */
javacall_result javacall_pim_list_remove_item(javacall_handle listHandle,
    javacall_handle itemHandle) {

    pim_list_dscr *pl = (pim_list_dscr*) listHandle;
    javacall_result res;

    javacall_printf("[PIM][javacall_pim_list_remove_item()] listHandle = 0X%X\n", listHandle);

    if(pl == NULL || pl->remove_item == NULL) {
        javacall_printf("[PIM][javacall_pim_list_remove_item()] Failed\n");
        return JAVACALL_FAIL;
    }
    res = pl->remove_item(itemHandle);
    javacall_printf("[PIM][javacall_pim_list_remove_item()] res = %d, itemHandle = 0X%X\n", res, itemHandle);
    return res;
}

/**
 * Adds the given category to the PIM list.
 * If the given category already exists in the list, this function does not add
 * another category, considers the call successful and returns.
 *
 * If the string is not a valid category as defined by the platform,
 * JAVACALL_FAIL is returned when trying to add it.
 *
 * @param listHandle handle of the list to add the new category to.
 * @param categoryName the name of the category to be added.
 * @retval JAVACALL_OK  on success
 * @retval JAVACALL_FAIL   if categories are unsupported, an error occurs,
 *                         or the list is no longer accessible or closed.
 */
javacall_result javacall_pim_list_add_category(javacall_handle listHandle,
    javacall_utf16 *categoryName) {

    pim_list_dscr *pl = (pim_list_dscr *) listHandle;

    PIM_LOG_1("[javacall_pim_list_add_category()] listHandle = 0X%X", listHandle);

    if(pl == NULL || pl->add_category== NULL) {
        return JAVACALL_FAIL;
    }

    return pl->add_category(categoryName);
}

/**
 * Removes the indicated category from the PIM list. If the indicated category
 * is not in the PIM list, this call is treated as successfully completed.
 *
 * @param listHandle handle to the list from which to remove the given category
 * @param categoryName the name of the category to be removed.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_FAIL if categories are unsupported, an error occurs,
 *                       or the list is no longer accessible or closed.
 */
javacall_result javacall_pim_list_remove_category(javacall_handle listHandle,
    javacall_utf16 *categoryName) {

    pim_list_dscr *pl = (pim_list_dscr *) listHandle;

    PIM_LOG_1("[javacall_pim_list_remove_category()] listHandle = 0X%X\n", listHandle);
    
    if(pl == NULL || pl->remove_category== NULL) {
        return JAVACALL_FAIL;
    }

    return pl->remove_category(categoryName);
}

/**
 * Renames a category.
 * If the new name is not valid for a category on the platform,
 * JAVACALL_FAIL must be returned when trying to rename a category to it.
 *
 * @param listHandle handle to the list to rename the category in.
 * @param oldCategoryName the old category name.
 * @param newCategoryName the new category name.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_FAIL in case of any error.
 */
javacall_result javacall_pim_list_rename_category(javacall_handle listHandle,
    javacall_utf16 *oldCategoryName, javacall_utf16 *newCategoryName) {

    pim_list_dscr *pl = (pim_list_dscr *) listHandle;

    PIM_LOG_1("[javacall_pim_list_rename_category()] listHandle = 0X%X\n", listHandle);
    
    if(pl == NULL || pl->rename_category== NULL) {
        return JAVACALL_FAIL;
    }

    return pl->rename_category(oldCategoryName, newCategoryName);
}

/**
 * Returns the maximum number of categories that this list can have.
 *
 * @param listHandle handle to the list.
 * @retval -1 - indicates there is no limit to the number of categories that
 *              this list can have
 * @retval 0  - indicates no category support
 * @retval >0 - in case a limit exists.
 */
int javacall_pim_list_max_categories(javacall_handle listHandle) {

    pim_list_dscr *pl = (pim_list_dscr *) listHandle;

    PIM_LOG_1("[javacall_pim_list_max_categories()] listHandle = 0X%X", listHandle);

    if(pl == NULL || pl->get_category_size== NULL) {
        return JAVACALL_FAIL;
    }

    return pl->get_category_size();
}

/**
 * Returns the maximum number of categories a list's item can be assigned to.
 *
 * @param listHandle handle to the list.
 * @retval -1 - indicates there is no limit to the number of categories
 *              this item can be assigned to
 * @retval 0  - indicates no category support
 * @retval >0 - in case a limit exists.
 */
int javacall_pim_list_max_categories_per_item(javacall_handle listHandle) {

    PIM_LOG_1("[javacall_pim_list_max_categories_per_item()] listHandle = 0X%X\n", listHandle);
    return 0;
}

/**
 * Returns a list of categories defined for the given PIM list in
 * ','-separated format (e.g., "Work,HOME,Friends").
 * If there are no categories defined for the PIM list or categories are
 * unsupported for the list, then JAVACALL_FAIL must be returned.
 *
 * @param listHandle handle to the list to get the categories for.
 * @param categoriesName pointer to a buffer in which to store the
 *                       categories list.
 * @param maxCategoriesLen available size of the categoriesName buffer.
  *
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_FAIL in case no categories found or incase of an error.
 */
javacall_result javacall_pim_list_get_categories(javacall_handle listHandle,
    javacall_utf16 *categoriesName, int maxCategoriesLen) {

    pim_list_dscr *pl = (pim_list_dscr *) listHandle;

    PIM_LOG_1("[javacall_pim_list_get_categories()] listHandle = 0X%X", listHandle);

    if(pl == NULL || pl->get_categories == NULL) {
        PIM_LOG_0("[javacall_pim_list_get_categories()] Failed\n");
        return JAVACALL_FAIL;
    }

    return pl->get_categories(categoriesName, maxCategoriesLen);
}

/**
 * Gets all fields that are supported in the given list.
 *
 * In order to identify unused fields, field attributes, field array elements,
 * JAVACALL_PIM_INVALID_ID should be set for each member that is not in use.
 *
 * @param listHandle handle to the list.
 * @param fields pointer to a buffer in which to store the fields.
 * @param maxFields maximum number of fields the field buffer can hold.
 *
 * @retval JAVACALL_OK  on success
 * @retval JAVACALL_FAIL  in case of reaching the last item in the list.
 */
javacall_result javacall_pim_list_get_fields(javacall_handle listHandle,
    javacall_pim_field *fields, int maxFields) {

    int i;
    pim_list_dscr *pl = (pim_list_dscr *) listHandle;
    
    PIM_LOG_2("[javacall_pim_list_get_fields()] listHandle = 0X%X, maxFields = %d\n", listHandle, maxFields);
    if(pl == NULL || pl->fields == NULL) {
        return JAVACALL_FAIL;
    }

    for(i = 0; i<maxFields && i< pl->num_of_fields; i++) {
        fields[i] = pl->fields[i];
    }    

    if(i < maxFields) {
        fields[i].id = JAVACALL_PIM_INVALID_ID;
    }
    PIM_LOG_1("[javacall_pim_list_get_fields()] num of fields = %d\n", i);
    
    return JAVACALL_OK;
}

/**
 * Gets all attributes supported by the given list.
 *
 * @param listHandle handle to the list.
 * @param attributes pointer to a buffer in which to store the attributes.
 * @param maxAttributes the maximum number of attributes the buffer can hold.
 * @retval JAVACALL_OK on success
 * @retval JAVACALL_FAIL in case of reaching the last item in the list.
 */
javacall_result javacall_pim_list_get_attributes(javacall_handle listHandle,
    javacall_pim_field_attribute *attributes, int maxAttributes) {

    int i = 0;
    pim_list_dscr *pl = (pim_list_dscr *)listHandle;

    PIM_LOG_2("[javacall_pim_list_get_attributes()] listHandle = 0X%X, maxAttributes = %d\n", listHandle, maxAttributes);
    if(pl == NULL || pl->attributes == NULL) {
        return JAVACALL_FAIL;
    }

    if(maxAttributes < JAVACALL_PIM_MAX_ATTRIBUTES) {
        return JAVACALL_FAIL;
    }

    if(pl->type != JAVACALL_PIM_TYPE_CONTACT) {
        attributes[i].id = (unsigned int)JAVACALL_PIM_INVALID_ID;
    }
    PIM_LOG_1("[javacall_pim_list_get_attributes()] pl->num_of_attributes = %d\n", pl->num_of_attributes);
    for(i =0; i < maxAttributes && i < pl->num_of_attributes; i++) {
        attributes[i] = pl->attributes[i];
    }

    /*if(i < maxAttributes) {
        attributes[i].id = JAVACALL_PIM_INVALID_ID;    
    }*/
    PIM_LOG_1("[javacall_pim_list_get_attributes()] num of attributes = %d\n", i+1);

    return JAVACALL_OK;
}
