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
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_default.h"
#include "mmi_theme.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "opera_api_adapt_platform.h"

/************************************************************************/
/* Message Handler                                                      */
/************************************************************************/
#if defined(OD_BUILD_OPERA_MINI)

static OD_MMI_TASK MMI_RESULT_E OperaMini_MMIMessageHandler(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	OD_Context* context = OperaMini_GetODContext();

	if (!context || !IsValidContext(context))
	{
		OD_TRACE_LOW("Opera mobile not running now in OperaMini_MMIMessageHandler, context = 0x%x", context);
		return MMI_RESULT_FALSE;
	}

	return OD_MMIMessageHandler(context, app_ptr, msg_id, param);
}

MMI_APPLICATION_T g_opera_mini_app = { OperaMini_MMIMessageHandler, CT_APPLICATION, NULL };

/************************************************************************/
/* Window Table                                                         */
/************************************************************************/
	
static WINDOW_TABLE(OPERAMINI_MAIN_WINDOW_TABLE) = 
{
    WIN_FUNC((uint32)OD_SPRD_MainWinProc),
    WIN_ID(OPERAMINI_MAIN_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif /* MMI_ONLY_IDLE_DISP_STATUSBAR */
    END_WIN
};


/***********************************************************************/
/* App Entry                                                            */
/************************************************************************/

OD_MMI_TASK MMIBROWSER_ERROR_E OperaMini_Entry(const MMIBROWSER_ENTRY_PARAM* entry_param_ptr)
{
	OD_Context* context = NULL;
    MMIBROWSER_ENTRY_PARAM *entry_param = NULL;

    OD_CHECK_ARGS(entry_param_ptr, MMIBROWSER_ERROR_INPUT);

#ifdef OD_STACK_TEST
    OD_StackTest_init();
    OD_StackTest_initTaskInfo(OD_STACKTEST_BREAM_TASK);
#endif

	OD_TRACE_LOW("OperaMini_Entry, entry_param_ptr->type=%d", entry_param_ptr->type);

	if (entry_param_ptr->type == MMIBROWSER_ADD_BOOKMARK)
	{
		MMIPUB_OpenAlertWinByTextId(PNULL, TXT_FACILITY_NOT_SUPPORTED, TXT_NULL,
			IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
		return MMIBROWSER_FAIL;
	}

	context = OperaMini_AllocODContext(sizeof(OD_Context));
	if (!context)
	{
		OD_TRACE_LOW("Error. Opera Mini Context allocation fail.");
		return MMIBROWSER_FAIL;
	}

	OD_TRACE_LOW("OperaMini Context 0x%x", context);

	if (!IsValidContext(context))
	{
		OD_STATUS ret;

		OD_InitDebugger();

		od_memset(&context->data, 0, sizeof(context->data));

		context->data.memory.size = OD_MEMORY_POOL_SIZE_MINI;
		context->data.app.win_id = OPERAMINI_MAIN_WIN_ID;
		context->data.app.module_id = MMI_MODULE_OPERA_MINI;

		ret = OperaMini_InitODContext();
		if (ret != OD_SUCCESS)
		{
			OD_TRACE_LOW("Error. Opera Mini Context Init Fail!");
			return MMIBROWSER_FAIL;
		}
	}

    if (opera_create_browser_entry_param(&entry_param) != OD_SUCCESS)
    {
        return MMIBROWSER_FAIL;
    }

	if (opera_clone_browser_entry_param(entry_param, entry_param_ptr) != MMIBROWSER_SUCC)
	{
		return MMIBROWSER_FAIL;
	}

    if (context->data.app.entry_arg)
    {
        opera_release_browser_entry_param(context->data.app.entry_arg);
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    {
        //Get Major settings 
        uint16  sim_ok = 0;
        uint32  sim_num = 0;
        MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

        sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);
        if (sim_num>1)
        {
            major_sim_type = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_WAP);

            if ((major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                && MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1))/*lint !e656*/ 
                )
            {
                entry_param->dual_sys = MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1);
            }
        }
    }
#endif

	context->data.app.entry_arg = entry_param;
	return OD_AppEntry(context, (uint32*)OPERAMINI_MAIN_WINDOW_TABLE, context->data.app.entry_arg);
}

#endif /* OD_BUILD_OPERA_MINI */

#if defined(OPERA_MINI_SUPPORT) && defined(OPERA_MINI_DUMMY)
static MMI_RESULT_E OperaMini_MMIMessageHandler(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	return MMI_RESULT_FALSE;
}
MMI_APPLICATION_T g_opera_mini_app = { OperaMini_MMIMessageHandler, CT_APPLICATION, NULL };
#endif /* defined(OPERA_MINI_SUPPORT) && defined(OPERA_MINI_DUMMY) */
