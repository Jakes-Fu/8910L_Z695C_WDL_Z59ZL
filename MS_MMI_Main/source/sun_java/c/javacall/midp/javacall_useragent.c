/*
 *
 * Copyright  1990-2008 Sun Microsystems, Inc. All Rights Reserved.
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

#include "javacall_defs.h"
#include "mmk_type.h"
#include "os_api.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "javacall_logging.h"

/**
 * Returns the extra user agent
 * Take the code in comments area as reference. Note that the memory to store the string
 * is NOT allocated in Java.
 * 
 * @param result pointer to the returned string in utf16
 * @param resultLen out string length in utf16 characters
 * @return JAVACALL_OK on success, otherwise JAVACALL_FAIL
 */  
javacall_result javacall_get_user_agent(unsigned short** result, int* resultLen) {      /*lint -e{765}*/
    int ua_len = 0;
    static wchar*ua_ptr = PNULL;
    MMIENG_UA_T ua = {0};
    
    //char useragent[MMIENG_MAX_URL_LEN + 1] = {0};
    
    if (!MMIAPIENG_GetUa(&ua))
    {
        javacall_printf("javacall_get_user_agent useragent fail");
        return JAVACALL_FAIL;    
    }
    
    javacall_printf("javacall_get_user_agent enter");
    
    ua_len = (int)strlen((char *)ua.user_agent);

    if (0 == ua_len)
    {
        ua_len = (int)strlen((char *)MMIAPICOM_GetUserAgent());
        SCI_MEMCPY(ua.user_agent, MMIAPICOM_GetUserAgent(), ua_len);
    }

    if (PNULL == ua_ptr)
    {
        ua_ptr = SCI_ALLOCA((ua_len + 1)*sizeof(wchar));/*lint !e737 */
        
        if (PNULL == ua_ptr)
        {
            //*result = PNULL;
            *resultLen = 0;
        }
        else
        {
            SCI_MEMSET(ua_ptr, 0x00, ((ua_len + 1)*sizeof(wchar)));/*lint !e737 */
            
            MMI_STRNTOWSTR(
                ua_ptr,
                ua_len,
                (uint8*)ua.user_agent,
                ua_len,
                ua_len
                );
        }
    }
    
    if (PNULL == ua_ptr)
    {
        return JAVACALL_FAIL;
    }
    else
    {
        *result = (unsigned short *)ua_ptr;
        *resultLen = ua_len;
        
        return JAVACALL_OK;
    }
}