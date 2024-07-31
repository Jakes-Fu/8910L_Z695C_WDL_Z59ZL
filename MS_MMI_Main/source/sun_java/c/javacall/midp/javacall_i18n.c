/*
 *
 * Copyright  1990-2006 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */ 

#include "javacall_i18n.h"
#include "javacall_logging.h"
#include "javacall_input.h"
#include "mmijava_main.h"
#include "mmi_modu_main.h"
#include "mmk_type.h"
//#include "java_mmi_text.h"

#define LOC_ENUS "en-US"

static javacall_bool bI18N_useNativeRes;

// do not use text\image\nv\module id out of control of app.mk
//static MMI_TEXT_ID_T indexMap[] = {
//#include "javacall_i18n_stringresource_inc.h"
//};

static int map_index_to_local_index(int index) {
    javacall_int32 indexMapNum = 0;
    MMI_TEXT_ID_T *indexMap = MMIJAVA_I18nGetText(&indexMapNum);    /*lint !e718 !e64*/
    if (index <indexMapNum) 
    {/*lint !e737 !e574*/
        return (javacall_int32)indexMap[index];
    } 
    else 
    {
        return (MMI_TEXT_ID_T)0;
    }
}

void javacall_i18n_init(void)
{
	char locale[6];
	javacall_input_get_locale(locale);
	if (0==strcmp(LOC_ENUS, locale)) {
		bI18N_useNativeRes = JAVACALL_FALSE; // Use default string resource: en-US
	} else {
		bI18N_useNativeRes = JAVACALL_TRUE;
	}
}

/**
 * Check if native platform has resources for string data
 *
 * @return JAVACALL_TRUE if resource data is exist or JAVACALL_FALSE if not
 */
javacall_bool javacall_i18n_check_native_resouce(void) {
    return bI18N_useNativeRes;
}

/**
 * Get string data from native platform resource data
 *
 * @param index     Index of string
 * @param str       Return of string. It should be pointer to constant memory.
 * @param length    Length of str
 *
 * @return JAVACALL_OK or JAVACALL_FAIL
 */
javacall_result javacall_i18n_get_native_string(int index, 
        javacall_utf16** str, int* length) {
    javacall_result ret = JAVACALL_FAIL;
    MMI_STRING_T nativeTxt = {0};

    index = map_index_to_local_index(index);
    if (index != -1) {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)index, &nativeTxt);
        if (nativeTxt.wstr_ptr != 0) {
            *str = (javacall_utf16*)nativeTxt.wstr_ptr;
            *length = (int)nativeTxt.wstr_len;
            ret = JAVACALL_OK;
        }
    }

    return ret;
}

