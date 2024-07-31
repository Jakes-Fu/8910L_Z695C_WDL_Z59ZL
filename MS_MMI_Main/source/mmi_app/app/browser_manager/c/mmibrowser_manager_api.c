/*****************************************************************************
** File Name:      mmibrowser_manager_api.c                                          *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser api             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011.11        fen.xie         modify filename(mmibrowser_api.c to mmibrowser_manager_api.c) *
**                                                                           *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_browser_manager_trc.h"
#include "mmk_app.h"
#include "mmibrowser_export.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "cfl_wchar.h"
#include "mmiconnection_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmi_appmsg.h"
#include "gui_ucs2b_converter.h"
#include "mmicl_export.h"
#include "mmibrowser_manager_func.h"
#include "mmibrowser_manager_wintable.h"
#include "browser_manager_text.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#ifdef BROWSER_SUPPORT_DORADO 
extern const MMIBROWSER_PROCESS_T g_dorado_proc;
#endif    
#ifdef BROWSER_SUPPORT_NETFRONT
extern const MMIBROWSER_PROCESS_T g_netfront_proc;
#endif    
#ifdef OPERA_MINI_SUPPORT
#ifndef WIN32
extern const MMIBROWSER_PROCESS_T g_opera_mini_proc;
#else
extern const MMIBROWSER_PROCESS_T g_opera_mini_proc = {0};
#endif
#endif    

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

MMIBROWSER_MANAGER_INFO_T   g_browser_info = {
{
#ifdef BROWSER_SUPPORT_DORADO 
    &g_dorado_proc,
#endif    
#ifdef OPERA_MINI_SUPPORT
    &g_opera_mini_proc,
#endif    
#ifdef BROWSER_SUPPORT_NETFRONT
    &g_netfront_proc,
#endif   
    },
    MMIBROWSER_TYPE_MAX
};


/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : whether other browser is running
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMIBROWSER_BROWSER_TYPE_E BMIsOtherRunning(MMIBROWSER_BROWSER_TYPE_E cur_browser_type);
        
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init browser module
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_InitModule(void)
{
    uint32  i = 0;
    
    MMIBM_InitModule();
    
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->InitModule != PNULL))
        {
            g_browser_info.process_func[i]->InitModule();
        }
    }
}

/*****************************************************************************/
//  Description : init browser 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_InitApp(void)
{
    uint32  i = 0;
    
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->InitApp != PNULL))
        {
            g_browser_info.process_func[i]->InitApp();
        }
    }
}

/*****************************************************************************/
//  Description : init browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_UninitApp(void)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->UninitApp != PNULL))
        {
            g_browser_info.process_func[i]->UninitApp();
        }
    }
}

/*****************************************************************************/
// 	Description : open confirmation dialog
//	Global resource dependence : none
//  Author: weipeng.wang
//	Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleBrowserConfirmationDialogMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T       *pubwin_info_ptr = PNULL;
	MMIBROWSER_ENTRY_PARAM * entry_param_ptr = PNULL;
  
	
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
	pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
	if(PNULL != pubwin_info_ptr)
	{	
		entry_param_ptr = (MMIBROWSER_ENTRY_PARAM *)pubwin_info_ptr->user_data;
		MMK_CloseWin(win_id);
		if(PNULL !=  entry_param_ptr)
		{
			if (entry_param_ptr->url_ptr)
			{
				SCI_FREE(entry_param_ptr->url_ptr);
				entry_param_ptr->url_ptr = NULL;
			}
			if (entry_param_ptr->url_title_ptr)
			{
				SCI_FREE(entry_param_ptr->url_title_ptr);
				entry_param_ptr->url_title_ptr = NULL;
			}
			if (entry_param_ptr->user_agent_ptr)
			{
				SCI_FREE(entry_param_ptr->user_agent_ptr);
				entry_param_ptr->user_agent_ptr = NULL;
			}
			SCI_FREE(entry_param_ptr);
		}
	}	
		
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
		
	#ifdef FM_SUPPORT
	//关闭FM
	MMIAPIFM_Exit();
	#endif
	#ifdef MMI_AUDIO_PLAYER_SUPPORT
	//关闭MP3
	MMIAPMAINWIN_Destory();
	#endif
	 
	pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
	if(PNULL != pubwin_info_ptr)
	{
		entry_param_ptr = (MMIBROWSER_ENTRY_PARAM *)pubwin_info_ptr->user_data;
		MMK_CloseWin(win_id);

		if(PNULL !=  entry_param_ptr)
		{
			MMIAPIBROWSER_Entry(entry_param_ptr);
			if (entry_param_ptr->url_ptr)
			{
				SCI_FREE(entry_param_ptr->url_ptr);
				entry_param_ptr->url_ptr = NULL;
			}
			if (entry_param_ptr->url_title_ptr)
			{
				SCI_FREE(entry_param_ptr->url_title_ptr);
				entry_param_ptr->url_title_ptr = NULL;
			}
			if (entry_param_ptr->user_agent_ptr)
			{
				SCI_FREE(entry_param_ptr->user_agent_ptr);
				entry_param_ptr->user_agent_ptr = NULL;
			}
			SCI_FREE(entry_param_ptr);
		}
	}	
	break;
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
	 break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : open confirmation dialog
//	Global resource dependence : none
//  Author: weipeng.wang
//	Note:
/*****************************************************************************/
LOCAL void MMIAPIBROWSER_OpenConfirmationDialog(MMI_TEXT_ID_T text_id,MMIBROWSER_ENTRY_PARAM * entry_param_ptr)
{	    
	int len = 0;
	int ret = MMIBROWSER_SUCC;
	MMIBROWSER_ENTRY_PARAM * user_data =  SCI_ALLOCA((sizeof(MMIBROWSER_ENTRY_PARAM)));

	SCI_ASSERT(PNULL!= user_data );
	SCI_MEMSET(user_data,0,(sizeof(MMIBROWSER_ENTRY_PARAM)));
	SCI_MEMCPY(user_data,entry_param_ptr,sizeof(MMIBROWSER_ENTRY_PARAM));

	user_data->url_ptr = PNULL;
	user_data->url_title_ptr = PNULL;
	user_data->user_agent_ptr = PNULL;
	
	do 
	{
	    if (entry_param_ptr->url_ptr && entry_param_ptr->url_ptr[0])
	    {
	        len = strlen(entry_param_ptr->url_ptr);
	        if (user_data->url_ptr)
	        {
	            SCI_FREE(user_data->url_ptr);
	        }
	        user_data->url_ptr = SCI_ALLOCA(len + 1);
	        if (user_data->url_ptr == NULL)
	        {
			ret = MMIBROWSER_FAIL;
		      SCI_TRACE_LOW("[BRW] func[%s] line[%d] malloc fail",__FUNCTION__,__LINE__);
	            break;
	        }
	        SCI_MEMSET(user_data->url_ptr, 0, len + 1);
	        strncpy(user_data->url_ptr, entry_param_ptr->url_ptr, len);
	    }

	    if (entry_param_ptr->url_title_ptr && entry_param_ptr->url_title_ptr[0])
	    {
	        len = strlen(entry_param_ptr->url_title_ptr);
	        if (user_data->url_title_ptr)
	        {
	            SCI_FREE(user_data->url_title_ptr);
	        }
	        user_data->url_title_ptr = SCI_ALLOCA(len + 1);
	        if (user_data->url_title_ptr == NULL)
	        {
	            ret = MMIBROWSER_FAIL;
	            SCI_TRACE_LOW("[BRW] func[%s] line[%d] malloc fail",__FUNCTION__,__LINE__);
	            break;
	        }
	        SCI_MEMSET(user_data->url_title_ptr, 0, len + 1);
	        strncpy(user_data->url_title_ptr, entry_param_ptr->url_title_ptr, len);
	    }

	    if (entry_param_ptr->user_agent_ptr && entry_param_ptr->user_agent_ptr[0])
	    {
	        len = strlen(entry_param_ptr->user_agent_ptr);
	        if (user_data->user_agent_ptr)
	        {
	            SCI_FREE(user_data->user_agent_ptr);
	        }
	        user_data->user_agent_ptr = SCI_ALLOCA(len + 1);
	        if (user_data->user_agent_ptr == NULL)
	        {
	            ret = MMIBROWSER_FAIL;
	            SCI_TRACE_LOW("[BRW] func[%s] line[%d] malloc fail",__FUNCTION__,__LINE__);
	            break;
	        }
	        SCI_MEMSET(user_data->user_agent_ptr, 0, len + 1);
	        strncpy(user_data->user_agent_ptr, entry_param_ptr->user_agent_ptr, len);
	    }
	} while (0);
	
	if (ret != MMIBROWSER_SUCC)
	{
	   	if(PNULL !=  user_data)
		{
			if (user_data->url_ptr)
			{
				SCI_FREE(user_data->url_ptr);
				user_data->url_ptr = NULL;
			}
			if (user_data->url_title_ptr)
			{
				SCI_FREE(user_data->url_title_ptr);
				user_data->url_title_ptr = NULL;
			}
			if (user_data->user_agent_ptr)
			{
				SCI_FREE(user_data->user_agent_ptr);
				user_data->user_agent_ptr = NULL;
			}
			SCI_FREE(user_data);
		}
		MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
	      return;
	}

 	MMIPUB_OpenAlertWinByTextIdEx(
                                                            MMK_GetFirstAppletHandle(), 
                                                            PNULL, 
                                                            text_id,
                                                            TXT_NULL, 
                                                            IMAGE_NULL,
                                                            PNULL,
                                                            PNULL,
                                                            MMIPUB_SOFTKEY_DIALOG_TURNOFF_CLOSE,
                                                            HandleBrowserConfirmationDialogMsg,
                                                            (uint32)user_data);
}


/*****************************************************************************/
// 	Description : MMIAPIBROWSER_OpenConfirmation
//	Global resource dependence : none
//  Author: weipeng.wang
//	Note:
/*****************************************************************************/

LOCAL MMIBROWSER_ERROR_E MMIAPIBROWSER_OpenConfirmation(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr)
{
	#ifdef MMI_AUDIO_PLAYER_SUPPORT
	 if(MMIAPIMP3_IsPlayerPlaying())//FM active suspend or play background
	{
		MMIAPIBROWSER_OpenConfirmationDialog(STR_COM_TURN_BGD_MUSIC,entry_param_ptr);
		return MMIBROWSER_FAIL;
	}
	 //关闭MP3
	MMIAPMAINWIN_Destory();
	#endif
	
	return MMIBROWSER_SUCC;

}

/*****************************************************************************/
//  Description : the entry of browser 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIAPIBROWSER_Entry(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr)
{
    MMIBROWSER_BROWSER_TYPE_E   default_browser_type = MMIBROWSER_TYPE_MAX;
    MMIBROWSER_ERROR_E  entry_result = MMIBROWSER_SUCC;
	
    if (PNULL == entry_param_ptr)
    {
        return MMIBROWSER_ERROR_INPUT;
    }
	
    if(0 == default_browser_type)/*lint !e774*/
    {
        //SCI_TRACE_LOW:"MMIAPIBROWSER_Entry MMIBROWSER_TYPE_MAX = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_API_165_112_2_18_2_7_36_0,(uint8*)"d",default_browser_type);
        return MMIBROWSER_FAIL;
    }

	if( MMIBROWSER_FAIL == MMIAPIBROWSER_OpenConfirmation(entry_param_ptr))
	{
		return MMIBROWSER_FAIL;
	}

     if(MMIAPISET_GetFlyMode())
     {
        MMIPUB_OpenAlertWarningWin(STR_TURN_OFF_FLIGHT_MODE);
        return MMIBROWSER_FAIL;
     }
     if (MMIAPIPHONE_GetSimOkNum(NULL,0)==0)
    {

        MMIPUB_OpenAlertWarningWin(STR_SIM_NOT_SIM_EXT01);
        return MMIBROWSER_FAIL;
    }

    if(MMICC_GetCallNum()>0) 
    {
        SCI_TRACE_LOW("MMICC_GetCallNum %d", MMICC_GetCallNum());
        MMIPUB_OpenAlertWarningWin(TXT_IN_CALLING);
        return MMIBROWSER_FAIL;
    }

#ifdef CMCC_UI_STYLE
    if (MMIBROWSER_ACCESS_URL == entry_param_ptr->type && entry_param_ptr->is_alert)
    {
        return MMIBM_QueryOpenUrlWin(entry_param_ptr);
    }
#endif

    if (MMIBROWSER_ACCESS_FLOWRATE == entry_param_ptr->type)//流量查看统一处理
    {
        MMIAPICL_OpenGprsFlowRateWin();
        return MMIBROWSER_SUCC;
    }

#ifdef JAVA_SUPPORT_MYRIAD
    if (PNULL != MMIAPICOM_Stristr(entry_param_ptr->url_ptr, ".jad")
        || PNULL != MMIAPICOM_Stristr(entry_param_ptr->url_ptr, ".jar"))
    {
        wchar url[MMIFILE_FULL_PATH_MAX_LEN] = {0};
        MMIAPICOM_StrToWstr(entry_param_ptr->url_ptr, url);
        MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID,MSG_JAVA_OTA_INSTALL_START,(DPARAM)url,sizeof(url));
        return MMIBROWSER_SUCC;
    }
#endif

    if (1 == MMIBROWSER_TYPE_MAX                             /*lint !e506 !e774*///only one browser
#ifdef MMI_BROWSER_MINI_SUPPORT
         //Bug 79601 - [ST][6530][64X64_220X176TK_QW]push信息，将网址添加到opera浏览器时提示“not support”
         || (MMIBROWSER_ADD_BOOKMARK == entry_param_ptr->type)
#endif
        )
    {
        if ((g_browser_info.process_func[0] != PNULL)
            && (g_browser_info.process_func[0]->Entry != PNULL))
        {
            g_browser_info.cur_browser_type = 0;
            entry_result = g_browser_info.process_func[0]->Entry(entry_param_ptr);//active it
        }
        else
        {
            entry_result = MMIBROWSER_FAIL;
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
        return entry_result;
    }

#ifndef WIN32
    if ((entry_param_ptr->is_browser_type_customized) && (MMIBROWSER_TYPE_MAX > entry_param_ptr->browser_type))//customize the browser type
    {
#ifndef BROWSER_ALL_RUNNING_SUPPORT    
        if (BMIsOtherRunning(entry_param_ptr->browser_type) < MMIBROWSER_TYPE_MAX)
        {
            MMIPUB_OpenAlertWarningWin(TXT_BM_EXIT_BROWSER_FIRST);
            return MMIBROWSER_FAIL;
        }     
        else
#endif            
        {
            if ((g_browser_info.process_func[entry_param_ptr->browser_type] != PNULL)
                && (g_browser_info.process_func[entry_param_ptr->browser_type]->Entry != PNULL))
            {
                g_browser_info.cur_browser_type = entry_param_ptr->browser_type;
                entry_result = g_browser_info.process_func[entry_param_ptr->browser_type]->Entry(entry_param_ptr);//active it
            }
            else
            {
                entry_result = MMIBROWSER_FAIL;
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
        }
    }
    else
#endif
    {
#if 0//ndef BROWSER_ALL_RUNNING_SUPPORT  
        MMIBROWSER_BROWSER_TYPE_E   running_browser_type = MMIBROWSER_TYPE_MAX;

        running_browser_type= BMIsOtherRunning(MMIBROWSER_TYPE_MAX);
        if (running_browser_type < MMIBROWSER_TYPE_MAX)
        {
            if ((g_browser_info.process_func[running_browser_type] != PNULL)
                && (g_browser_info.process_func[running_browser_type]->Entry != PNULL))
            {
                g_browser_info.cur_browser_type = running_browser_type;
                entry_result = g_browser_info.process_func[running_browser_type]->Entry(entry_param_ptr);//active it
            }
            else
            {
                entry_result = MMIBROWSER_FAIL;
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
        }
        else
#endif
        {
            default_browser_type = MMIBM_GetDefaultBrowser();
#ifdef WIN32
#ifdef BROWSER_SUPPORT_DORADO
            default_browser_type = MMIBROWSER_TYPE_DORADO;
#elif defined (OPERA_MINI_SUPPORT)
			default_browser_type = MMIBROWSER_TYPE_OPERA_MINI;
#else
#error "Either BROWSER_SUPPORT_DORADO or OPERA_MINI_SUPPORT has to be defined here."
#endif
#endif
            
            if (default_browser_type < MMIBROWSER_TYPE_MAX)
            {
                if ((g_browser_info.process_func[default_browser_type] != PNULL)
                    && (g_browser_info.process_func[default_browser_type]->Entry != PNULL))
                {
                    g_browser_info.cur_browser_type = default_browser_type;
                    entry_result = g_browser_info.process_func[default_browser_type]->Entry(entry_param_ptr);//active it
                }
                else
                {
                    entry_result = MMIBROWSER_FAIL;
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                }
            }
            else
            {
                MMIBM_CreateEntryParam(*entry_param_ptr);
                MMIBM_OpenBrowserSelectWin();
            }
        }
    }

    return entry_result;
}

/*****************************************************************************/
//  Description : close browser 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIAPIBROWSER_Close(void)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->Exit != PNULL))
        {
            g_browser_info.process_func[i]->Exit();
        }
        else
        {
            //do nothing
        }
    }
    return MMIBROWSER_SUCC;
}

/*****************************************************************************/
//  Description : Clean cahe
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 清除浏览器和http的cache
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_CleanCache(uint8 * url_ptr, uint16 url_len)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->CleanCache != PNULL))
        {
            g_browser_info.process_func[i]->CleanCache(url_ptr, url_len);
        }
        else
        {
            //do nothing
        }
    }
}

/*****************************************************************************/
//  Description : Get browser Version of one browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC char* MMIAPIBROWSER_GetVersion(MMIBROWSER_BROWSER_TYPE_E browser_type)
{
    if (browser_type < MMIBROWSER_TYPE_MAX)
    {
        if ((g_browser_info.process_func[browser_type] != PNULL)
            && (g_browser_info.process_func[browser_type]->GetBrowserVersion != PNULL))
        {
            return g_browser_info.process_func[browser_type]->GetBrowserVersion();
        }
    }
    
    return PNULL;
}

/*****************************************************************************/
//  Description : Delete completed download task of all browsers
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_DeleteCompletedDownloadTask(uint32 task_index)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->DeleteCompletedDownloadTask != PNULL))
        {
            g_browser_info.process_func[i]->DeleteCompletedDownloadTask(task_index);
        }      
    }
}

/*****************************************************************************/
//  Description : Check availability of Timer(only valid for Netfront)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_CheckTimerDisable(void)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->CheckTimerDisable != PNULL))
        {
            g_browser_info.process_func[i]->CheckTimerDisable();
        }    
    }
}

/*****************************************************************************/
//  Description : set net setting index for all browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_SetNetSettingIndex(MN_DUAL_SYS_E dual_sys, uint8 index)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->SetNetAccountBySim != PNULL))
        {
            g_browser_info.process_func[i]->SetNetAccountBySim(dual_sys, index);
        }    
    }
}

/*****************************************************************************/
//  Description : get net setting index for browser(priority :default > MMIBROWSER_BROWSER_TYPE_E )
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPIBROWSER_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    MMIBROWSER_BROWSER_TYPE_E   default_browser_type = MMIBROWSER_TYPE_MAX;
    uint8	i = 0;
    uint8   index = 0;
        
    default_browser_type = MMIBM_GetDefaultBrowser();
    
    if (default_browser_type < MMIBROWSER_TYPE_MAX)//get the default browser's setting
    {
        if ((g_browser_info.process_func[default_browser_type] != PNULL)
            && (g_browser_info.process_func[default_browser_type]->GetNetAccountBySim != PNULL))
        {
            return g_browser_info.process_func[default_browser_type]->GetNetAccountBySim(dual_sys);//get the valid default browser's setting
        }    
    }
    
    //the priority is in MMIBROWSER_TYPE_MAX, get the first valid browser's setting
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->GetNetAccountBySim != PNULL))
        {
            index = g_browser_info.process_func[i]->GetNetAccountBySim(dual_sys);
            break;
        }    
    }

    return index;
}

/*****************************************************************************/
//  Description : handle plug out sd card
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_SDCardPlugOut(void)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->SDCardPlugOutNotify != PNULL))
        {
            g_browser_info.process_func[i]->SDCardPlugOutNotify();
        }    
    }
}

/*****************************************************************************/
//  Description : handle browser when format the storage 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_HandleStorageFormat(void)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->StorageFormatNotify != PNULL))
        {
            g_browser_info.process_func[i]->StorageFormatNotify();
        }       
    }
}

/*****************************************************************************/
//  Description : for reset to factory setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_ResetFactorySetting(void)
{
    uint32  i = 0;
    
    MMIBM_ResetFactorySetting();
        
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->ResetFactorySetting != PNULL))
        {
            g_browser_info.process_func[i]->ResetFactorySetting();
        }       
    }
}

/*****************************************************************************/
//  Description : handle app message
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_HandleAppMsg(MMI_MESSAGE_ID_E msg_id,void* param)
{
    uint32  i = 0;

    if (MMIBM_HandleAppMsg(msg_id, param))
    {
        return;//has been handled by browser manager
    }

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->HandleAppMsg != PNULL))
        {
            g_browser_info.process_func[i]->HandleAppMsg(msg_id, param);
        }       
    }
}

/*****************************************************************************/
//  Description : register the callback function of mime type
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_RegMimeFunc(MMIBROWSER_MIME_DOWNLOAD_PARAM_T* param_ptr)
{
#ifdef BROWSER_SUPPORT_NETFRONT
    if  ((g_browser_info.process_func[MMIBROWSER_TYPE_NF] != PNULL)
        && (g_browser_info.process_func[MMIBROWSER_TYPE_NF]->RegMimeHandleFunc != PNULL))
    {
         g_browser_info.process_func[MMIBROWSER_TYPE_NF]->RegMimeHandleFunc(param_ptr);
    }       
#endif    
#if defined(BROWSER_SUPPORT_WEBKIT) && (!defined(WIN32) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
    if  ((g_browser_info.process_func[MMIBROWSER_TYPE_WK] != PNULL)
        && (g_browser_info.process_func[MMIBROWSER_TYPE_WK]->RegMimeHandleFunc != PNULL))
    {
         g_browser_info.process_func[MMIBROWSER_TYPE_WK]->RegMimeHandleFunc(param_ptr);
    }       
#endif    

    return MMIBM_RegMimeFunc(param_ptr);
}

/*****************************************************************************/
//  Description : Clean Usert Data Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_CleanUserInfo(void)
{
    uint32  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->CleanUserInfo != PNULL))
        {
            g_browser_info.process_func[i]->CleanUserInfo();
        }       
    }
}

/*****************************************************************************/
//  Description : check if browser is the focused win
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_IsActive(void)
{
    MMIBROWSER_BROWSER_TYPE_E  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->IsActive != PNULL))
        {
            if (g_browser_info.process_func[i]->IsActive())
            {
                //SCI_TRACE_LOW:"[MMIBM]MMIAPIBROWSER_IsActive:active_browser_type=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_API_549_112_2_18_2_7_36_1,(uint8*)"d", i);
                return TRUE;
            }
        }      
    }

    return  FALSE;
}

/*****************************************************************************/
//  Description : check if browser is running
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_IsRunning(void)
{
    MMIBROWSER_BROWSER_TYPE_E  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->IsRunnning != PNULL))
        {
            if (g_browser_info.process_func[i]->IsRunnning())
            {
                //SCI_TRACE_LOW:"[MMIBM]MMIAPIBROWSER_IsRunning:running_browser_type=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_API_573_112_2_18_2_7_36_2,(uint8*)"d", i);
                return TRUE;
            }
        }      
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Other browser Is Running
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMIBROWSER_BROWSER_TYPE_E BMIsOtherRunning(MMIBROWSER_BROWSER_TYPE_E cur_browser_type)
{
    MMIBROWSER_BROWSER_TYPE_E  i = 0;

    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if (cur_browser_type != i)
        {
            if ((g_browser_info.process_func[i] != PNULL)
                && (g_browser_info.process_func[i]->IsRunnning != PNULL))
            {
                if (g_browser_info.process_func[i]->IsRunnning())
                {
                    //SCI_TRACE_LOW:"[MMIBM]BMIsOtherRunning:cur_browser_type=%d,running_browser_type=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_API_599_112_2_18_2_7_36_3,(uint8*)"dd", cur_browser_type, i);
                    break;
                }
            }
        }
    }

    return i;
}

/*****************************************************************************/
//  Description : check if the customized browser is running
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_IsRunningEx(MMIBROWSER_BROWSER_TYPE_E browser_type)
{
    if (browser_type < MMIBROWSER_TYPE_MAX)//customized browser type
    {
        if ((g_browser_info.process_func[browser_type] != PNULL)
            && (g_browser_info.process_func[browser_type]->IsRunnning != PNULL))
        {
            return g_browser_info.process_func[browser_type]->IsRunnning();
        }      
    }    

    return FALSE;
}

/*****************************************************************************/
//  Description : check if the customized browser is the focused win
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_IsActiveEx(MMIBROWSER_BROWSER_TYPE_E browser_type)
{
    if (browser_type < MMIBROWSER_TYPE_MAX)//customized browser type
    {
        if ((g_browser_info.process_func[browser_type] != PNULL)
            && (g_browser_info.process_func[browser_type]->IsActive != PNULL))
        {
            return g_browser_info.process_func[browser_type]->IsActive();
        }      
    }    

    return FALSE;
}

/*****************************************************************************/
//  Description : Delete completed download task of the customized browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_DeleteCompletedDownloadTaskEx(uint32 task_index, MMIBROWSER_BROWSER_TYPE_E browser_type)
{
    if (browser_type < MMIBROWSER_TYPE_MAX)//customized browser type
    {
        if ((g_browser_info.process_func[browser_type] != PNULL)
            && (g_browser_info.process_func[browser_type]->DeleteCompletedDownloadTask != PNULL))
        {
            g_browser_info.process_func[browser_type]->DeleteCompletedDownloadTask(task_index);
        }      
    }    
}

/*****************************************************************************/
//  Description : set net setting index fo customized browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_SetNetSettingIndexEx(MN_DUAL_SYS_E dual_sys, uint8 index, MMIBROWSER_BROWSER_TYPE_E browser_type)
{
    if (browser_type < MMIBROWSER_TYPE_MAX)//customized browser type
    {
        if ((g_browser_info.process_func[browser_type] != PNULL)
            && (g_browser_info.process_func[browser_type]->SetNetAccountBySim != PNULL))
        {
            g_browser_info.process_func[browser_type]->SetNetAccountBySim(dual_sys, index);
        }    
    }
}

/*****************************************************************************/
//  Description : handle download callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: only used by dorado and opera
/*****************************************************************************/
PUBLIC void MMIAPIBROWSER_MimeDownlaodCallBack(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{
    MMIBM_MimeDownlaodCallBack(param_ptr);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function  Declare                                                              */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle url input from at+sytin cmd
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Atest_BROWSERInsertURL(
                wchar * url_ptr,                 //input url 
                uint32 len                          //input url length 
                )
{
    MMIBROWSER_BROWSER_TYPE_E  i = 0;

    //to the first valid func
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->Atest_BROWSERInsertURL != PNULL))
        {
            return g_browser_info.process_func[i]->Atest_BROWSERInsertURL(url_ptr, len);
        }
    }

    return FALSE;
}                

/*****************************************************************************/
//  Description : input bookmark at cmd into bookmark title edit box
//  Global resource dependence : 
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Atest_BROWSERBookmarkTitleInput(
                wchar * wstr_ptr,             //point to the bookmark title information buffer    
                uint16  ilen                      //length of the information buffer
                )
{
    MMIBROWSER_BROWSER_TYPE_E  i = 0;

    //to the first valid func
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->Atest_BROWSERBookmarkTitleInput!= PNULL))
        {
            return g_browser_info.process_func[i]->Atest_BROWSERBookmarkTitleInput(wstr_ptr, ilen);
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : input bookmark at cmd into bookmark URL edit box
//  Global resource dependence : 
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Atest_BROWSERBookmarkURLInput(
                wchar * wstr_ptr,             //point to the bookmakr url buffer
                uint16  ilen                      //length of the information buffer
                )
{
    MMIBROWSER_BROWSER_TYPE_E  i = 0;

    //to the first valid func
    for (i = 0; i < MMIBROWSER_TYPE_MAX; i++)
    {
        if ((g_browser_info.process_func[i] != PNULL)
            && (g_browser_info.process_func[i]->Atest_BROWSERBookmarkURLInput!= PNULL))
        {
            return g_browser_info.process_func[i]->Atest_BROWSERBookmarkURLInput(wstr_ptr, ilen);
        }
    }

    return FALSE;
}               
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    End of Atest  Function  Declare                                                                  */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

