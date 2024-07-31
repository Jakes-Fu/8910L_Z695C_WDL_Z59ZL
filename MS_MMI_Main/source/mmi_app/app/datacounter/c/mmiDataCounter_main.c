/****************************************************************************
** File Name:      mmiDataCounter_main.c                            
** Author:                                                                  
** Date:            2015/08/24
** Description:    This file is used to process msg to main window of CALCULATION
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
****************************************************************************/
#define _MMIDATACOUNTER_MAIN_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "guicommon.h"
#include "mmi_text.h"
#include "tb_dal.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "guimenu.h"

#include "guimsgbox.h"
#include "guilabel.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "guibutton.h"
#include "guistatusbar.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_msg.h"
#include "guiwin.h"

#include "mmi_resource.h"
#include "guistring.h"
#include "mmipdp_export.h"

#include "datacounter_text.h"
#include "datacounter_internal.h"
#include "datacounter_menutable.h"
#include "datacounter_id.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmidatacounter_nv.h"

LOCAL MMI_RESULT_E HandleDataCounterWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param);


WINDOW_TABLE( DATA_COUNTER_WIN_TAB ) = 
{
	WIN_ID(MMIACC_DATA_COUNTER_WIN_ID),
	WIN_FUNC((uint32) HandleDataCounterWinMsg),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_DATACOUNTER_SIM_SETTINGS_CTRL_ID),
	WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, STXT_RETURN),
	END_WIN
};

PUBLIC void GetSIMIccId(MN_DUAL_SYS_E simCard, wchar* iccid)
{
        MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
	  MN_IMSI_T imsi[MMI_DUAL_SYS_MAX] = {0};
	  MMINV_READ(MMINV_PHONE_IMSI,imsi,return_value);
	  if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(imsi,0,sizeof(imsi));
            MMINV_WRITE(MMINV_PHONE_IMSI,imsi);
        }
       MMIAPICOM_StrToWstr(imsi[simCard].imsi_val, iccid);
	  
}

PUBLIC BOOLEAN CheckSameSimCard(MN_DUAL_SYS_E simCard, uint32 item_id)
{
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    wchar sim_iccid[25] = {0};

    if(simCard >= MN_DUAL_SYS_MAX)
    {
        simCard = MN_DUAL_SYS_1;
    }

    GetSIMIccId(simCard, sim_iccid);

    MMINV_READ(item_id, &record_data_nv, return_value);

    if (return_value != MN_RETURN_SUCCESS)
    {
        SCI_MEMSET(&record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        record_data_nv.clearFlag = 0;//未存储
    }

    //当卡槽所在sim IMSI不一致时，说明卡发生变化，此时清除上次保存记录
    if (0 == memcmp(sim_iccid, record_data_nv.iccid, MMIAPICOM_Wstrlen(sim_iccid)))//同一张卡
    {
         return TRUE;
    }

    return FALSE;
      
}

LOCAL void CheckSameCardAndClearAllRecord(MN_DUAL_SYS_E simCard)
{
    uint32 item_id = 0;
    MMI_DATACOUNTER_RECORD_DATA_NV_E record_data_nv = {0};
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    if ( MN_DUAL_SYS_1 == simCard )
    {
        item_id = MMI_DATACOUNTER_SIM1_DATA_RECORD;
    }
    else if( MN_DUAL_SYS_2 == simCard )
    {
        item_id = MMI_DATACOUNTER_SIM2_DATA_RECORD;
    }

    MMINV_READ(item_id, &record_data_nv, return_value);
    SCI_TRACE_LOW("mmiDataCounter_main.c ---- CheckSameCardAndClearAllRecord read NV=%d  SIM= %d",return_value, simCard);

    if ( return_value != MN_RETURN_SUCCESS )
    {
        SCI_MEMSET(&record_data_nv, 0x00, sizeof(MMI_DATACOUNTER_RECORD_DATA_NV_E));
        record_data_nv.clearFlag = 0;//未存储
    }
    
    if ( FALSE == CheckSameSimCard(simCard, item_id) )
    {
        if ( record_data_nv.clearFlag != 0 )
        {
            ClearDataCounterPeriodAllRecord(0, simCard);
        }
    }
 
}
#define NETWORKNAME_BUF_MAX_LEN   50
LOCAL void MMIDataCounter_DualSimItems(void)
{
    uint8 sim_index = 0; 
    MMI_CTRL_ID_T ctrl_id = MMI_DATACOUNTER_SIM_SETTINGS_CTRL_ID;
    GUILIST_ITEM_T item_t = {0};
    MMI_TEXT_ID_T text_id = 0;
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T networkName_sim = {0};
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    item_t.item_data_ptr = &item_data;

    GUILIST_SetMaxItem(ctrl_id, MMI_DATACOUNTER_SIM_SETTINGS_ITEM_NUM,FALSE);
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS;

    if (MMIAPIPHONE_GetSimAvailableNum(NULL, 0) == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else
    { 
	   for(sim_index = 0; sim_index< MMI_DUAL_SYS_MAX; ++sim_index)
	   { 
	          if (SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(sim_index))
		   {
		          CheckSameCardAndClearAllRecord(sim_index);

                       #ifdef MMI_MULTI_SIM_SYS_SINGLE
                            text_id = STR_LIST_SIM_EXT01;
                       #else
                            switch( sim_index )
                            {
                                case 0:
                                    text_id = STR_ILIST_SIM_ONE;
                                    break;
                                case 1:
                                    text_id = STR_ILIST_SIM_TWO;
                                    break;
                            }
                       #endif
                       
			    SCI_MEMSET(opn_spn_name, 0x00, sizeof(wchar)*MMIPHONE_MAX_OPER_NAME_LEN);
	    		    networkName_sim.wstr_ptr = opn_spn_name;
			    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
			    item_data.item_content[0].item_data.text_id = text_id;
			    MMIAPIPHONE_GetSimAndServiceString(sim_index, &networkName_sim);
			    item_data.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
			    item_data.item_content[1].item_data.text_buffer = networkName_sim;
			    //item_data.item_content[1].is_opacity = TRUE;
                //GUILIST_SetItemContentOpacityFontColor(&item_data,1,255*0.6,255*0.6);
			    GUILIST_InsertItem(ctrl_id, &item_t, sim_index);
			  
		   }

		}
   	}
}

LOCAL MMI_RESULT_E HandleDataCounterWinMsg(
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E simId = MMI_SmartDS_SIM_ID_1;
    MMI_STRING_T    statusBar_str = {0};
    MMI_MENU_ID_T menu_id = 0xffffff;	
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIDataCounter_DualSimItems();
            MMK_SetAtvCtrl(win_id, MMI_DATACOUNTER_SIM_SETTINGS_CTRL_ID);
            break;

        case MSG_FULL_PAINT:
            MMI_GetLabelTextByLang(STR_HDR_COUNTERS, &statusBar_str);

            if(MMIAPICOM_StatusAreaSetViewHeaderinfo(statusBar_str.wstr_ptr, statusBar_str.wstr_len)) 
            {
                GUIWIN_UpdateStb();
            }
            break;	

        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        {
            if ( MMIAPIPHONE_GetSimAvailableNum(NULL, 0) == 0 )
            {
                return recode;
            }
            
            menu_id = GUILIST_GetCurItemIndex(MMI_DATACOUNTER_SIM_SETTINGS_CTRL_ID);
            switch(menu_id)
            {
                case MMI_DATACOUNTER_SIM_SETTINGS_SIM1:
                    #ifdef MMI_MULTI_SIM_SYS_SINGLE
                        simId = MMI_SmartDS_SIM_ID_1;
                        if ( SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(simId) )
                        {
                            MMIAPIDATACOUNTER_OpenResultWin(&simId);
                        }
                    #else
                        simId = MMI_SmartDS_SIM_ID_1;
                        if ( SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(simId) )
                        {
                            simId = MMI_SmartDS_SIM_ID_2;
                        }

                        if ( SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(simId) )
                        {
                            MMIAPIDATACOUNTER_OpenResultWin(&simId);
                        }
                    #endif
                    break;
                case MMI_DATACOUNTER_SIM_SETTINGS_SIM2:
                    simId = MMI_SmartDS_SIM_ID_2;
                    if ( SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(simId) )
                    {
                        MMIAPIDATACOUNTER_OpenResultWin(&simId);
                    }
                    
                    break;

                default:
                    ;
                    break;
            }

        }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;

    }

    return recode;
}

/*****************************************************************************/
// 	Description : MMIAPIDATACOUNTER_OpenMainWin
//         
//	Global resource dependence : 
//   Author:        zhanglu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIDATACOUNTER_OpenMainWin(void)
{
   MMK_CloseMiddleWin(MMI_DATACOUNTER_START_WIN_ID, MMI_DATACOUNTER_MAX_WID_ID);
   MMK_CreateWin((uint32*)DATA_COUNTER_WIN_TAB,PNULL);
    
}

//#else
/*****************************************************************************/
// 	Description : MMIAPICALC_OpenMainWin
//
//	Global resource dependence :
//   Author:        zhaohui
//	Note:
/*****************************************************************************/
/*
PUBLIC void MMIAPIDATACOUNTER_OpenMainWin(void)
{

}*/
//#endif//DATA_COUNTER_SUPPORT

PUBLIC void MMIAPIDATACOUNTER_InitModule(void)
{
   MMI_DataCounter_RegMenuGroup();
   MMIDATACOUNTER_NV_RegNv();
}

/*Edit by script, ignore 3 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
