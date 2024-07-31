/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"

#include "opera_api_adapt_platform.h"

static MMIBROWSER_ERROR_E opera_clone_browser_entry_param(MMIBROWSER_ENTRY_PARAM *dst_entry_param, const MMIBROWSER_ENTRY_PARAM *src_entry_param)
{
    int ret = MMIBROWSER_SUCC;
    int len = 0;

    if (!dst_entry_param || !src_entry_param)
    {
        return MMIBROWSER_ERROR_INPUT;
    }

    do 
    {
        dst_entry_param->type = src_entry_param->type;
        dst_entry_param->dual_sys = src_entry_param->dual_sys;
        dst_entry_param->is_connection_customized = src_entry_param->is_connection_customized;
        dst_entry_param->is_use_wifi = src_entry_param->is_use_wifi;
        dst_entry_param->connection_index = src_entry_param->connection_index;

        if (src_entry_param->url_ptr && src_entry_param->url_ptr[0])
        {
            len = strlen(src_entry_param->url_ptr);
            if (dst_entry_param->url_ptr)
            {
                SCI_FREE(dst_entry_param->url_ptr);
            }
            dst_entry_param->url_ptr = SCI_ALLOCA(len + 1);
            if (dst_entry_param->url_ptr == NULL)
            {
                OD_TRACE_LOW("opera_clone_browser_entry_param ALLOC FAIL len =%d\n", len);
                ret = MMIBROWSER_FAIL;
                break;
            }
            SCI_MEMSET(dst_entry_param->url_ptr, 0, len + 1);
            strncpy(dst_entry_param->url_ptr, src_entry_param->url_ptr, len);
        }

        if (src_entry_param->url_title_ptr && src_entry_param->url_title_ptr[0])
        {
            len = strlen(src_entry_param->url_title_ptr);
            if (dst_entry_param->url_title_ptr)
            {
                SCI_FREE(dst_entry_param->url_title_ptr);
            }
            dst_entry_param->url_title_ptr = SCI_ALLOCA(len + 1);
            if (dst_entry_param->url_title_ptr == NULL)
            {
                OD_TRACE_LOW("opera_clone_browser_entry_param ALLOC FAIL len =%d\n", len);
                ret = MMIBROWSER_FAIL;
                break;
            }
            SCI_MEMSET(dst_entry_param->url_title_ptr, 0, len + 1);
            strncpy(dst_entry_param->url_title_ptr, src_entry_param->url_title_ptr, len);
        }

        if (src_entry_param->user_agent_ptr && src_entry_param->user_agent_ptr[0])
        {
            len = strlen(src_entry_param->user_agent_ptr);
            if (dst_entry_param->user_agent_ptr)
            {
                SCI_FREE(dst_entry_param->user_agent_ptr);
            }
            dst_entry_param->user_agent_ptr = SCI_ALLOCA(len + 1);
            if (dst_entry_param->user_agent_ptr == NULL)
            {
                OD_TRACE_LOW("opera_clone_browser_entry_param ALLOC FAIL len =%d\n", len);
                ret = MMIBROWSER_FAIL;
                break;
            }
            SCI_MEMSET(dst_entry_param->user_agent_ptr, 0, len + 1);
            strncpy(dst_entry_param->user_agent_ptr, src_entry_param->user_agent_ptr, len);
        }
    } while (0);

    if (ret != MMIBROWSER_SUCC)
    {
        opera_release_browser_entry_param(dst_entry_param);
    }

    return ret;
}

static OD_STATUS opera_create_browser_entry_param(MMIBROWSER_ENTRY_PARAM **entry_param)
{
    if (entry_param == NULL)
    {
        return OD_ERROR;
    }
    *entry_param = SCI_ALLOCA(sizeof(MMIBROWSER_ENTRY_PARAM));
    if (*entry_param == NULL)
    {
        return OD_ERROR;
    }

    memset(*entry_param, 0, sizeof(**entry_param));
    return OD_SUCCESS;
}

static void opera_release_browser_entry_param(MMIBROWSER_ENTRY_PARAM *entry_param)
{
    if (entry_param->url_ptr)
    {
        SCI_FREE(entry_param->url_ptr);
        entry_param->url_ptr = NULL;
    }
    if (entry_param->url_title_ptr)
    {
        SCI_FREE(entry_param->url_title_ptr);
        entry_param->url_title_ptr = NULL;
    }
    if (entry_param->user_agent_ptr)
    {
        SCI_FREE(entry_param->user_agent_ptr);
        entry_param->user_agent_ptr = NULL;
    }
    SCI_FREE(entry_param);
}
