/****************************************************************************
** File Name:     mmibt_deviceservice.c                                     *
** Author:         yuantao.xu                                            *
** Date:           8/22/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth device paired*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/22/2012    yuantao.xu           Create                                  *
**                                                                         *
****************************************************************************/
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#define _MMIBT_DEVICESERVICE_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmibt_func.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的踞构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "guitext.h"
#include "mmibt_a2dp.h"
#include "mmibt_app.h"
#include "mmibt_nv.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "guiedit.h"
#include "mmibt_deviceservice.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define  MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH   240                       //baokun change from 100 to 120
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct mmibt_device_service_param_tag
{    
    BT_ADDRESS device_addr;
    MMIBT_OPERTATION_TYPE_E operation_type;
}MMIBT_DEVICE_SERVICE_PARAM_T;

#define  MMI_BLACK_COLOR		  0x0000

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle ftp device serive list win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleTextBoxWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param 
                                          );  
/*****************************************************************************/
// Description : 取得蓝牙设备的详情信息
// Global resource dependence : 
// Author: 
// Note:根据g_bt_current_addr，得到该设备的名称，设备号，服务，转化为UCS2格式输出，
//      在该函数中会为字符串分配内存，需要在调用该函数后，释放
/*****************************************************************************/
LOCAL void GetTextBoxString(MMI_STRING_T *info_str, BT_ADDRESS *bt_addr, MMIBT_OPERTATION_TYPE_E type);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenTextBoxWin(MMIBT_DEVICE_SERVICE_PARAM_T *param_ptr);
/*****************************************************************************/
//the window for show ftp device service list 
WINDOW_TABLE(MMIBT_TEXT_BOX_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleTextBoxWinMsg ),    
    WIN_ID(MMIBT_TEXT_BOX_WIN_ID),
    //WIN_TITLE(TXT_NULL),
    CREATE_TEXT_CTRL(MMIBT_TEXT_BOX_DEVICE_DETAILS_CTRL_ID),
    WIN_SOFTKEY(NULL, NULL, STXT_RETURN),
    END_WIN
};

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle ftp device serive list win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleTextBoxWinMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param 
                                                  )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIBT_TEXT_BOX_DEVICE_DETAILS_CTRL_ID;
    MMI_STRING_T            content_str = {0};
    MMIBT_DEVICE_SERVICE_PARAM_T *win_param_ptr = (MMIBT_DEVICE_SERVICE_PARAM_T *)MMK_GetWinUserData(win_id);

    GUI_FONT_T font = SONG_FONT_18;
   //GUI_BG_T  	text_backgrand = {0};

    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        if(MMIBT_OPC_HELP == win_param_ptr->operation_type)
        {
            MMI_GetLabelTextByLang(TXT_HELP, &content_str);
        }
        else if(MMIBT_OPC_DEVICE_DETAILS == win_param_ptr->operation_type)
        {
            MMI_GetLabelTextByLang(STXT_SOFTKEY_DETAILS_MK, &content_str);
        }

        //GUIWIN_SetTitleText(win_id, content_str.wstr_ptr, content_str.wstr_len, FALSE);

        //set text string
        //text_backgrand.bg_type = GUI_BG_COLOR; //éè??±3?°ààDíê???é?
        //text_backgrand.color = MMI_BLACK_COLOR;
        GetTextBoxString(&content_str, &win_param_ptr->device_addr, win_param_ptr->operation_type);
        //GUITEXT_SetBg(ctrl_id, &text_backgrand);
        GUITEXT_SetFont(ctrl_id, &font, PNULL);
        GUITEXT_SetString(ctrl_id,content_str.wstr_ptr,content_str.wstr_len,FALSE);
        if(PNULL != content_str.wstr_ptr)
        {
            SCI_FREEIF(content_str.wstr_ptr);
            content_str.wstr_ptr = PNULL;
        }

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        {
	uint16 		deviceName[BT_DEVICE_NAME_SIZE] = {0};
	wchar test1[64] = {0};
	wchar test_exa[4] = {0x002e,0x002e,0x002e, 0x0000};

	BT_GetPairedDeviceName(&win_param_ptr->device_addr,deviceName);
       SCI_TRACE_LOW("MMIBT: HandleTextBoxWinMsg-%d", MMIAPICOM_Wstrlen(deviceName));
	if(MMIAPICOM_Wstrlen(deviceName) >35){
             MMIAPICOM_Wstrncpy(test1,deviceName,35);
             MMIAPICOM_Wstrcat(test1,test_exa);
	    MMIAPICOM_Wstrncpy(deviceName,test1, MIN(BT_DEVICE_NAME_SIZE, MMIAPICOM_Wstrlen(test1)));
        }
        if(MMIAPICOM_StatusAreaSetViewHeaderinfo(deviceName, MMIAPICOM_Wstrlen(deviceName)))
        {
           GUIWIN_UpdateStb();
        }
        MMIBT_ClrClientRect(win_id);
    	}
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        SCI_FREEIF(win_param_ptr);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
/*****************************************************************************/
//  Discription: Open storage select win 
//  Global resource dependence: None
//  Author: yuantao.xu
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void  MMIBT_EnterTextBoxWin(BT_ADDRESS *device_addr, MMIBT_OPERTATION_TYPE_E operation_type)
{
    MMIBT_DEVICE_SERVICE_PARAM_T param = {0};
    
    if(PNULL != device_addr)
    {
        SCI_MEMCPY(&param.device_addr, device_addr, sizeof(BT_ADDRESS));
    }
    param.operation_type = operation_type;
    
    OpenTextBoxWin(&param);
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenTextBoxWin(MMIBT_DEVICE_SERVICE_PARAM_T *param_ptr)
{
    MMIBT_DEVICE_SERVICE_PARAM_T *win_param_ptr = PNULL;
    MMI_HANDLE_T                win_handle = 0;
    BOOLEAN                     res = MMI_RESULT_FALSE;  
    
    if(PNULL == param_ptr)
        return res;
    
    if(MMK_IsOpenWin(MMIBT_TEXT_BOX_WIN_ID))
    {
        MMK_CloseWin(MMIBT_TEXT_BOX_WIN_ID);
    }

    win_param_ptr = (MMIBT_DEVICE_SERVICE_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_DEVICE_SERVICE_PARAM_T));
    if(PNULL != win_param_ptr)
    {
        SCI_MEMSET(&win_param_ptr->device_addr,0,sizeof(BT_ADDRESS));
        SCI_MEMCPY(&win_param_ptr->device_addr, &param_ptr->device_addr, sizeof(BT_ADDRESS));

        win_param_ptr->operation_type = param_ptr->operation_type;
        
        win_handle = MMK_CreateWin((uint32 *)MMIBT_TEXT_BOX_WIN_TAB, (ADD_DATA)win_param_ptr);
        if (!win_handle || (MMI_INVALID_ID == win_handle))
        {
            SCI_FREEIF(win_param_ptr);
        }
        else
        {
            MMK_SetWinUserData(MMIBT_TEXT_BOX_WIN_ID,(void *)win_param_ptr);
            res = MMI_RESULT_TRUE;
        }
    }
    else
    {
        SCI_TRACE_LOW("OpenTextBoxWin alloc fail");
    }

    return res;
}
/*****************************************************************************/
// Description : 取得蓝牙设备的详情信息
// Global resource dependence : 
// Author: 
// Note:根据g_bt_current_addr，得到该设备的名称，设备号，服务，转化为UCS2格式输出，
//      在该函数中会为字符串分配内存，需要在调用该函数后，释放
/*****************************************************************************/
LOCAL void GetTextBoxString(MMI_STRING_T *info_str, BT_ADDRESS *bt_addr, MMIBT_OPERTATION_TYPE_E type)
{
    uint32 service_type = 0;
    int32   i = 0;
    uint16  u16lf[] = {0x0D,0x0A,'\0'};
    uint16  len = 0;
    uint16  cpy_len = 0;
    uint16  lf_len = 0;
    wchar   *str_ptr = NULL;
    wchar   *lf_ptr = NULL;
    uint8  addr_str[BT_ADDRESS_SIZE*4] = {0};
    MMI_STRING_T text_str = {0};
	//Help
	MMI_STRING_T text_str_1 ={0};
	MMI_STRING_T text_str_2 ={0};
#ifdef MMIBT_FTS_SUPPORT    
	MMI_STRING_T text_str_3 ={0};
#endif
	MMI_STRING_T text_str_4 ={0};
	
    //SCI_PASSERT(info_str != NULL,("GetTextBoxString info_str %d", info_str));/*assert verified*/
    if(info_str == PNULL)
    {
        return;
    }
    
    if(MMIBT_OPC_DEVICE_DETAILS == type)
    {
        //SCI_PASSERT(bt_addr != NULL,("GetDetailsInfo bt_addr %d", bt_addr));/*assert verified*/
        if(bt_addr == NULL)
        {
            return;
        }
        
        //detail
        str_ptr = (wchar *)SCI_ALLOC_APPZ((MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH +1)*sizeof(wchar));
        //SCI_PASSERT(str_ptr != NULL ,("GetDetailsInfo str_ptr %d ",str_ptr));/*assert verified*/
		SCI_MEMSET(str_ptr, 0x00, (MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH +1)*sizeof(wchar));
        info_str->wstr_ptr = str_ptr; 
        //device number:xxx
        MMI_GetLabelTextByLang(STR_BT_DEVICE_NUMBER, &text_str);
        lf_ptr = u16lf;
        lf_len = 2;
        len = MIN(text_str.wstr_len, MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH);
        info_str->wstr_len = len;
        MMIAPICOM_Wstrncpy(info_str->wstr_ptr, text_str.wstr_ptr, info_str->wstr_len);
        if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
        {
            MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
            info_str->wstr_len += lf_len;
        }
        for(i = BT_ADDRESS_SIZE -1 ; i >= 0; i--)
        {
           sprintf((char*)addr_str+strlen((char*)addr_str), "%02x", bt_addr->addr[i]);
           if(i > 0)
           {
             sprintf(addr_str+strlen((char*)addr_str), "%s", ":"); /*lint !e64 */
           }
        }
        cpy_len = MIN((uint32)(MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH - (uint32)info_str->wstr_len), strlen((char*)addr_str));/*lint !e666*/
        
        MMI_STRNTOWSTR(&(info_str->wstr_ptr[info_str->wstr_len]), MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH, addr_str, BT_ADDRESS_SIZE*4, cpy_len);
        info_str->wstr_len += cpy_len;
        info_str->wstr_ptr[info_str->wstr_len] = 0;
        if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
        {
            MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
            info_str->wstr_len += lf_len;
        }
        //Serive Type: xxx
        MMI_GetLabelTextByLang(STR_BT_DEVICE_TYPE, &text_str);
        if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
        {
            MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
            info_str->wstr_len += text_str.wstr_len;
        }
        if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
        {
            MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
            info_str->wstr_len += lf_len;
        }
        service_type =  BT_GetPairedDeviceService(bt_addr); 
        if ((BT_SERVICE_NONE | BT_SERVICE_OPP| BT_SERVICE_FTP)& service_type)
        {
			//not handfree
            MMI_GetLabelTextByLang(STR_BT_DEVICE_NO_HANDFREE, &text_str);
        }
        else if ((BT_SERVICE_HANDFREE | BT_SERVICE_HANDSET | BT_SERVICE_A2DP)& service_type)
        {
             //Handfree
            MMI_GetLabelTextByLang(STR_BT_DEVICE_HANDFREE, &text_str);
        }
		else
		{
			//not handfree
            MMI_GetLabelTextByLang(STR_BT_DEVICE_NO_HANDFREE, &text_str);
		}

        if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
        {
            MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
            info_str->wstr_len += text_str.wstr_len;
        }
        if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
        {
            MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
            info_str->wstr_len += lf_len;
        }
        //Support Service :
        MMI_GetLabelTextByLang(STR_BT_DEVICE_SERVICE, &text_str);
        if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
        {
            MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
            info_str->wstr_len += text_str.wstr_len;
        }
        if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
        {
            MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
            info_str->wstr_len += lf_len;
        }
        if ((BT_SERVICE_FTP | BT_SERVICE_OPP) & service_type)
        {
            //Send File
            MMI_GetLabelTextByLang(STR_BT_SEND_FILE, &text_str);
            if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
                info_str->wstr_len += text_str.wstr_len;
            }
            if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
                info_str->wstr_len += lf_len;
            }
            //Send VCard
            MMI_GetLabelTextByLang(STR_BT_SEND_VCARD, &text_str);
            if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
                info_str->wstr_len += text_str.wstr_len;
            }
            if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
                info_str->wstr_len += lf_len;
            }
        }
        if((BT_SERVICE_HANDFREE | BT_SERVICE_HANDSET)& service_type)
        {
             //Handfree Mode
            MMI_GetLabelTextByLang(STR_BT_SERVICE_HANDFREE, &text_str);
            if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
                info_str->wstr_len += text_str.wstr_len;
            }
            //SCI_MEMCPY(str_ptr + len, text_str.str_ptr, text_str.length);
            if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
                info_str->wstr_len += lf_len;
            }
            //Handset Mode
            MMI_GetLabelTextByLang(STR_BT_SERVICE_HANDSET, &text_str);
            if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
                info_str->wstr_len += text_str.wstr_len;
            }
            //SCI_MEMCPY(str_ptr + len, text_str.str_ptr, text_str.length);
            if(info_str->wstr_len + lf_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
                info_str->wstr_len += lf_len;
            }
        }
        if(BT_SERVICE_A2DP & service_type)
        {
            //Play Music
            MMI_GetLabelTextByLang(STR_BT_SERVICE_MUSIC, &text_str);
            if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
                info_str->wstr_len += text_str.wstr_len;
            }
        }
#if 0
        if(BT_SERVICE_PBAP_SERVER & service_type)
        {
            //PBAP phonebook sync
            MMI_GetLabelTextByLang(STR_CONTACT_SYNC_VIA_BT, &text_str);
            if(info_str->wstr_len + text_str.wstr_len <= MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH)
            {
                MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
                info_str->wstr_len += text_str.wstr_len;
            }
        }
#endif
    }
    else if(MMIBT_OPC_HELP == type)
    {
        MMI_GetLabelTextByLang(TXT_BT_HELP_OPEN, &text_str);
        MMI_GetLabelTextByLang(TXT_BT_HELP_VISIBLE, &text_str_1);
        MMI_GetLabelTextByLang(TXT_BT_HELP_DEVICE, &text_str_2);
#ifdef MMIBT_FTS_SUPPORT        
        MMI_GetLabelTextByLang(TXT_BT_HELP_SHARE, &text_str_3);
#endif
        MMI_GetLabelTextByLang(TXT_BT_HELP_NAME, &text_str_4);
        lf_ptr = u16lf;
        lf_len = 2;
#ifdef MMIBT_FTS_SUPPORT        
		len = text_str.wstr_len + text_str_1.wstr_len + text_str_2.wstr_len + text_str_3.wstr_len + text_str_4.wstr_len + 12;
#else
        len = text_str.wstr_len + text_str_1.wstr_len + text_str_2.wstr_len + text_str_4.wstr_len + 12;
#endif
        str_ptr = (wchar *)SCI_ALLOC_APPZ(len*sizeof(wchar));
        SCI_MEMSET(str_ptr, 0x00, len*sizeof(wchar));
        info_str->wstr_ptr = str_ptr;
        info_str->wstr_len = 0;
        //SCI_PASSERT(str_ptr != NULL ,("GetDetailsInfo str_ptr %d ",str_ptr));/*assert verified*/
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
        info_str->wstr_len += text_str.wstr_len;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
        info_str->wstr_len += lf_len;
        //info_str->wstr_ptr[info_str->wstr_len] = 0;
        
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str_1.wstr_ptr, text_str_1.wstr_len);
        info_str->wstr_len += text_str_1.wstr_len;
        
        //SCI_MEMCPY(str_ptr + len, text_str.str_ptr, text_str.length);
        
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
        info_str->wstr_len += lf_len;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str_2.wstr_ptr, text_str_2.wstr_len);
        info_str->wstr_len += text_str_2.wstr_len;
#ifdef MMIBT_FTS_SUPPORT        
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
        info_str->wstr_len += lf_len;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str_3.wstr_ptr, text_str_3.wstr_len);
        info_str->wstr_len += text_str_3.wstr_len;
#endif        
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, lf_ptr, lf_len);
        info_str->wstr_len += lf_len;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str_4.wstr_ptr, text_str_4.wstr_len);
        info_str->wstr_len += text_str_4.wstr_len;		
    }
}
#endif
