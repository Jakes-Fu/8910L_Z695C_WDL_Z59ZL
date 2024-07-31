/****************************************************************************
** File Name:     mmibt_headset.c                                     *
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
#define _MMIBT_HEADSET_C_
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
#include "guires.h"
#include "guiedit.h"
#include "guictrl_api.h"
#include "mmibt_headset.h"
#include "mmi_appmsg.h"
#include "mmicc_id.h"

#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_bt_a2dp_show_connected_alert = TRUE;

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:check headset all service is close
//  Global resource dependence: none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetIsClosed(BT_ADDRESS * in_addr, BT_ADDRESS *out_addr);
/*****************************************************************************/
//  Description :  HandleConnectCnfAlertWin
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleConnectCnfAlertWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id,
                         DPARAM             param
                         );
/*****************************************************************************/
//  Description :  HandleConnectCnfAlertWin
//  Global resource dependence :
//  Author:lifei.lu
//  Note:Click the OK will close the alert win
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisconnectCnfAlertWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id,
                         DPARAM             param
                         );

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : close headset
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS CloseHeadset(const BT_ADDRESS *src_addr)
{
    uint32 dev_service = BT_GetPairedDeviceService(src_addr);
    BT_STATUS  return_value = BT_ERROR;

    //SCI_TRACE_LOW:"CloseHeadset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10694_112_2_18_2_8_22_342,(uint8*)"");
    if(BT_SERVICE_A2DP & dev_service && MMIBT_GetCurrentA2dpStatus(src_addr))/*lint !e605 */
    {
        //A2DP need to close
        return_value = MMIBT_CloseA2dpHeadset();
        if(TRUE == return_value)
        {
            return_value = BT_PENDING;
        }
        else
        {
            return_value = BT_ERROR;
        }

    }
    else if((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & dev_service && MMIBT_IsDeviceActivedEarphone(src_addr))
    {
        //HFG need to close
        return_value =  BT_DeactivateHS();
    }
    else
    {
        return_value = BT_SUCCESS;
    }

    return return_value;
}
/*****************************************************************************/
//  Discription:check headset all service is close
//  Global resource dependence: none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetIsClosed(BT_ADDRESS * in_addr, BT_ADDRESS *out_addr)
{
    BOOLEAN ret = TRUE;
    BOOLEAN is_active = FALSE;
    uint32      service_type = 0;
    BT_DEVICE_INFO dev_info = {0};
    BT_STATUS    status = 0;
    uint32      device_num = 0;
    uint32       i = 0;

    if(PNULL != in_addr)
    {
        //only check in_addr is connected headset or not
        //get current earphone status, if is active do not allow to delete device
        service_type = BT_GetPairedDeviceService(in_addr);
        if ((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & service_type)
        {
            is_active = MMIBT_IsDeviceActivedEarphone(in_addr);//MMIBT_GetCurrentEarphoneStatus(in_addr);
        }
    //#ifdef  BT_A2DP_SUPPORT
        if (BT_SERVICE_A2DP & service_type)
        {
            is_active |= MMIBT_GetCurrentA2dpStatus(in_addr);
        }
    //#endif
        if(is_active)
        {
            if(PNULL != out_addr)
            {
              SCI_MEMCPY(out_addr, in_addr, sizeof(BT_ADDRESS));
            }
        }
    }
    else
    {
        //check all paired device to find exist Connected Headset
        device_num = BT_GetPairedDeviceCount(BT_SERVICE_HANDSET);
        for(i = 1; i <= device_num; i++)
        {
             status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDSET, i, &dev_info);
             if(BT_SUCCESS == status)
             {
                service_type = BT_GetPairedDeviceService(&dev_info.addr);
                if (BT_SERVICE_HANDSET & service_type)
                {
                    is_active = MMIBT_IsDeviceActivedEarphone(&dev_info.addr);//MMIBT_GetCurrentEarphoneStatus(&dev_info.addr);
                }
           // #ifdef  BT_A2DP_SUPPORT
                if (BT_SERVICE_A2DP & service_type)
                {
                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
            //#endif
                if(is_active)
                {
                    if(PNULL != out_addr)
                    {
                      SCI_MEMCPY(out_addr, &dev_info.addr, sizeof(BT_ADDRESS));
                    }
                    break;
                }
             }//if(BT_SUCCESS == status)
        }//for(i = 1; i <= device_num; i++)
    }
    ret = !is_active;
    //SCI_TRACE_LOW:"HeadsetIsClosed ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9149_112_2_18_2_8_19_316,(uint8*)"d",ret);
    return ret;
}
/*****************************************************************************/
//  Description : handle headset close result
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void handleHeadsetCloseResult(BT_STATUS bt_status, BT_ADDRESS *headset_addr_ptr)
{
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    BT_ADDRESS   connect_addr = {0};
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();

    //wait_text.wstr_ptr = wait_text_buf;
    if(BT_SUCCESS == bt_status)
    {
        //close finished
        if(MMIBT_OPC_OPEN_HEADSET != operation_info.current_opertion)
        {
            //xingdong_bt
            //no HFG and A2DP need to active, show "Alerady Closed"
            if(HeadsetIsClosed(PNULL, PNULL))
            {
                MMIDEFAULT_SetBackLight(TRUE);
				MMI_GetLabelTextByLang(STR_BT_AUDIO_PHONE, &wait_text);

                //lulifei add to change the ui style
                MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleDisconnectCnfAlertWin);
                //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
                MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
#ifdef MMI_BT_PBAP_SUPPORT
                MMIBT_GetBtStatusForIcon();
#endif
            }
            //SetBackgroupRun(FALSE);
        }
        else
        {
            //current operation not close headset, need continue to do current_opertion
            //xingdong_bt
            //SetBackgroupRun(FALSE);
            DoOperation(operation_info.current_opertion, FALSE);
        }
    }
    else if (BT_PENDING == bt_status)
    { //Closing...
        //xingdong_bt

        if(HeadsetIsClosed(PNULL, &connect_addr) || (!memcmp(headset_addr_ptr, &connect_addr, sizeof(BT_ADDRESS))))
        {
            if(!MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))// && (!GetIsBackgroupRun()))
            {
                if(MMIBT_OPC_OPEN_HEADSET == operation_info.current_opertion)
                {
                    MMI_GetLabelTextByLang(STR_BT_WAIT_CONNECTING_EXT01, &wait_text);
                }
                else
                {
                    //Dective
                    MMI_GetLabelTextByLang(STR_NOTE_WAITING, &wait_text);
                }
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_EARCONN_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
            }
        }
    }
    else
    {
        //xingdong_bt
        //close error
        //if(!GetIsBackgroupRun())
        {
            MMIDEFAULT_SetBackLight(TRUE);
            //lulife change the ui style
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_URN_NO_DEV_FOUND,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
            //MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_BT_URN_NO_DEV_FOUND,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
            MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        }
        //SetBackgroupRun(FALSE);
    }
}
/*****************************************************************************/
//  Description : open headset
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void OpenHeadset(const BT_ADDRESS *src_addr)
{
    uint32 dev_service = 0;
    BT_STATUS return_value = BT_ERROR;
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    MMI_WIN_ID_T connect_alert_id = MMI_BT_CONNECT_CNF_WIN_ID;
    BT_ADDRESS  address = {0};
    //uint16 		dev_name[BT_DEVICE_NAME_SIZE] = {0};

    //open headset
    //SCI_TRACE_LOW:"OpenHeadset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10737_112_2_18_2_8_22_343,(uint8*)"");
    if(!HeadsetIsClosed(PNULL, &address) && (memcmp(src_addr, &address, sizeof(BT_ADDRESS))))
    {
        //exist not closed service, close it first
        return_value = CloseHeadset(&address);
        handleHeadsetCloseResult(return_value, &address);
        return;
    }
    //SCI_TRACE_LOW:"OpenHeadset address = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10745_112_2_18_2_8_22_344,(uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
    //SCI_TRACE_LOW:"OpenHeadset src_addr = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10746_112_2_18_2_8_22_345,(uint8*)"dddddd", src_addr->addr[0],src_addr->addr[1],src_addr->addr[2],src_addr->addr[3],src_addr->addr[4],src_addr->addr[5]);

    dev_service = BT_GetPairedDeviceService(src_addr);
    if((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & dev_service && (!MMIBT_IsDeviceActivedEarphone(src_addr)))
    {
        //HFG need to active
        return_value = BT_ActivateHS(src_addr);
        SCI_TRACE_LOW("OpenHeadset HFG need to active return_value=%d", return_value);
    }
    else if(BT_SERVICE_A2DP & dev_service && (!MMIBT_GetCurrentA2dpStatus(src_addr)))/*lint !e605 */
    {
        //A2DP need to active
        return_value = MMIBT_OpenA2dpHeadset(*src_addr);
        SCI_TRACE_LOW("OpenHeadset A2DP need to active return_value=%d", return_value);
        if(TRUE == return_value)
        {
            return_value = BT_PENDING;
        }
		else
		{
			return_value = BT_ERROR;
		}
    }
    else
    {
        //SCI_TRACE_LOW:"MMIBT LOCAL void OpenHeadset enter, TXT_BT_HEADSET_OPENED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10769_112_2_18_2_8_22_346,(uint8*)"");
        MMIDEFAULT_SetBackLight(TRUE);
        MMK_CloseWin(alert_id);
        MMI_GetLabelTextByLang(STR_BT_AUDIO_ACCESSORY, &wait_text);
        //lulifei add to change the ui style
        MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
            &connect_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleConnectCnfAlertWin);
        s_bt_a2dp_show_connected_alert = FALSE;
        //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
        //&connect_alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleConnectCnfAlertWin);
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_ACTIVE);
#ifdef BT_BQB_SUPPORT
        MMIAPICC_IsCallingAndInfoBt();
		SCI_TRACE_LOW("MMIBT LOCAL void OpenHeadset, MMIAPICC_IsCallingAndInfoBt");
#endif
#ifdef MMI_BT_PBAP_SUPPORT
        MMIBT_GetBtStatusForIcon();
#endif
        return;
    }
    if (BT_PENDING == return_value)
    {
        //xingdong_bt
        //Opening...
        if(!MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))// && (!GetIsBackgroupRun()))
        {
            MMI_GetLabelTextByLang(STR_BT_WAIT_CONNECTING_EXT01, &wait_text);
			MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_EARCONN_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
        }
    }
    //xingdong_bt
    else if(return_value != BT_SUCCESS)// && (!GetIsBackgroupRun()))
    {
        //Open or close error
        MMIDEFAULT_SetBackLight(TRUE);
        //MMIPUB_CloseAlertWin();
        MMK_CloseWin(alert_id);
        //lulifei update the ui style
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_URN_NO_DEV_FOUND,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
        //MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_BT_URN_NO_DEV_FOUND,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    }
}
/*****************************************************************************/
//  Discription: get active earphone result confirm
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_OpenHeadsetCnf(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp)
{
    BT_ADDRESS     address = {0};
    BT_ADDRESS     connected_address = {0};
    BT_DEVICE_INFO connect_dev_info = {0};
    //BT_STATUS     return_value = BT_SUCCESS;
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    MMI_WIN_ID_T connect_alert_id = MMI_BT_CONNECT_CNF_WIN_ID;

    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_OpenHeadsetCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6264_112_2_18_2_8_13_270,(uint8*)"");
        return;
    }
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
    if(MMIBT_IsOnTransferStatus())
    {
        //lulifei update the UI style
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DELETE_TRANSFERRING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
        //MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_BT_DELETE_TRANSFERRING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
        return;
    }
#endif
    //xingdong_bt
    //SetBackgroupRun(FALSE);
    if(is_a2dp)
    {
        //set a2dp flag and status
        MMIBT_A2dpLinkConnected(msg_body_ptr);
        address = MMIBT_GetA2dAddress(msg_body_ptr);
    }
    else
    {
        address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
        //address = remote_dev_info->addr;
        MMIBT_SetHfuStatus();
    }
    //SCI_TRACE_LOW:"MMIBT_OpenHeadsetCnf address = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6281_112_2_18_2_8_13_271,(uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);

    BT_GetPairedDeviceName(&address, connect_dev_info.name);
	if(BT_SUCCESS != msg_body_ptr->status)
	{
		//SCI_TRACE_LOW:"MMIBT_OpenHeadsetCnf msg_body_ptr->status %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6286_112_2_18_2_8_13_272,(uint8*)"d", msg_body_ptr->status);
		MMIDEFAULT_SetBackLight(TRUE);
		ClosePrmAndOpenDoneWin(
			MMIBT_EARCONN_WAIT_WIN_ID,
			STR_BT_URN_NO_DEV_FOUND,
			IMAGE_PUBWIN_FAIL,
			MMI_2SECONDS
			);
		return;
	}
    //只有当窗口打开时才处理该消息
    if(MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))
    {
        if (BT_SUCCESS == msg_body_ptr->status)
        {
            OpenHeadset(&address);
        }
    }
    else
    {
        //remote open
        //CR 228983
        if(!HeadsetIsClosed(PNULL, &connected_address) && (memcmp(&connected_address, &address, sizeof(BT_ADDRESS))))
        {
            //another headset already open, must close this headset
            //SetBackgroupRun(TRUE);
            handleHeadsetCloseResult(CloseHeadset(&(address)), &address);
        }
        else
        {
            if(MMIBT_DeviceIsConnectedHeadset(&address))
            {
                //opened finished
                //this headset is opened
                //xingdong_bt
                //if(!GetIsBackgroupRun() && MMIBT_OpenedHeadsetIsExist(&connect_dev_info))
                if(MMIBT_OpenedHeadsetIsExist(&connect_dev_info))
                {
                    //SCI_TRACE_LOW:"MMIBT MMIBT_OpenHeadsetCnf enter, TXT_BT_HEADSET_OPENED"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6324_112_2_18_2_8_13_273,(uint8*)"");
                    MMIDEFAULT_SetBackLight(TRUE);
                    MMK_CloseWin(alert_id);
                    //wait_text.wstr_ptr = wait_text_buf;
                    MMI_GetLabelTextByLang(STR_BT_AUDIO_ACCESSORY, &wait_text);
                    MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_ACTIVE);
                    //lulifei add to change the ui style
                    MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
                        &connect_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleConnectCnfAlertWin);
                    s_bt_a2dp_show_connected_alert = FALSE;
                    //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
                    //&connect_alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleConnectCnfAlertWin);
#ifdef MMI_BT_PBAP_SUPPORT
                    MMIBT_GetBtStatusForIcon();
#endif
                }
                #ifdef BT_BQB_SUPPORT
                MMIAPICC_IsCallingAndInfoBt();
                #endif
            }
            else
            {
                OpenHeadset(&address);
            }
        }
        //remote open
    }
    if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID) && MMIBT_DeviceIsConnectedHeadset(&address))
    {
        //Update Device list
        MMK_PostMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_FINISHED, PNULL, PNULL);
    }
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT
    MMIAPIENVSET_SetBluetoothMode();
#endif
    if(BT_SUCCESS == msg_body_ptr->status)
    {
		MMIBT_UpdateBtIdleIcon(BT_GetState(),MMIBT_GetIsDeviceVisible());
    }
}

/*****************************************************************************/
//  Discription: get deactive earphone confirm
//  Global resource dependence: None
//  Author: kelly.li
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_CloseHeadsetCnf(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp)
{
    MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    BT_ADDRESS   address = {0};
    BT_STATUS    return_value = BT_SUCCESS;
    BT_DEVICE_INFO *device = PNULL;
    BT_DEVICE_INFO  device_info = {0};
    MMI_STRING_T wait_text = {0};

    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_CloseHeadsetCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6371_112_2_18_2_8_13_274,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMIBT_CloseHeadsetCnf is_a2dp %d msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6374_112_2_18_2_8_13_275,(uint8*)"dd",is_a2dp, msg_body_ptr->status);
    device = (BT_DEVICE_INFO *)msg_body_ptr->body_ptr;
	if(is_a2dp)
    {
	    //MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_DISCONNECT);
        //set a2dp flag and status
        MMIBT_A2dpLinkDisconnect(msg_body_ptr);
        address = MMIBT_GetA2dAddress(msg_body_ptr);
    }
    else
    {
        address = device->addr;
    }
    BT_GetPairedDeviceName(&address, device_info.name);
    //SCI_TRACE_LOW:"MMIBT_CloseHeadsetCnf address = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6387_112_2_18_2_8_13_276,(uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
    //xingdong_bt
    if(HeadsetIsClosed(PNULL, PNULL))
    {
        //closed headset is playing headset or no active headset exist, need stop music
//         MMIBT_RecoveryBtStatus(MMIBT_MODULE_A2DP);
        //xingdong_bt
        MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_DISCONNECT);

        MMIBT_StopDevice(MMIBT_GetA2dpAppType());
        //recovery device status
        MMIBT_Init_ActionQueue();
        MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_STOP);
    }
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        //check need close other service or not
        return_value = CloseHeadset(&address);
        if (MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))
        {
            handleHeadsetCloseResult(return_value, &address);
        }
        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            //Update Device list
            MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_FINISHED, PNULL);
        }
    }
    else
    {
        MMIDEFAULT_SetBackLight(TRUE);
        //lulifei change the ui style
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_URN_NO_DEV_FOUND,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        //MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_BT_URN_NO_DEV_FOUND,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    }

    if (MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))
    {
         MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_DEACTIVE);
        //xingdong_bt
        //SetBackgroupRun(FALSE);
    }
    else if(MMIBT_IsPhoneReady())
    {
        //phone ready and not bt power off action

	    if(MMK_IsOpenWin(MMICC_STATUS_WIN_ID))
        {
     		MMK_SendMsg(MMICC_STATUS_WIN_ID, MSG_CC_CONNECT_UPDATE_BUTTON, PNULL);
	    }
        //wait_text.wstr_ptr = wait_text_buf;
        MMI_GetLabelTextByLang(STR_BT_AUDIO_PHONE, &wait_text);
        if(HeadsetIsClosed(PNULL, PNULL))
        {
            //xingdong_bt
            //close all service
            //if(!GetIsBackgroupRun())
            {
                MMIDEFAULT_SetBackLight(TRUE);
                //lulifei add for the dialog style
                if(MMK_IsOpenWin(MMI_BT_REMOTE_ALERT_WIN_ID))
                {
                    MMK_CloseWin(MMI_BT_REMOTE_ALERT_WIN_ID);
                }
                MMIPUB_OpenAlertWinByTextPtr(PNULL, &wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleDisconnectCnfAlertWin);
                //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL, &wait_text,PNULL,IMAGE_NULL,&alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,PNULL);
                //Update device list
                MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_DEACTIVE);
                if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
                {
                    MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_FINISHED, NULL);
                }
            }
            //SetBackgroupRun(FALSE);
        }
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    }
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT
    MMIAPIENVSET_UnSetBluetoothMode();
#endif
    if(BT_SUCCESS == msg_body_ptr->status)
    {
		MMIBT_UpdateBtIdleIcon(BT_GetState(),MMIBT_GetIsDeviceVisible());
    }
}
/*****************************************************************************/
//  Discription: get active earphone result confirm
//  Global resource dependence: None
//  Author:
//  Note :
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_OpenHeadsetConfirmIND(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp)
{
    BT_ADDRESS     address = {0};
    BT_DEVICE_INFO connect_dev_info = {0};
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    MMI_WIN_ID_T connect_alert_id = MMI_BT_CONNECT_CNF_WIN_ID;

    if(PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_OpenHeadsetConfirmIND msg_body_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11086_112_2_18_2_8_23_354,(uint8*)"");
        return;
    }
    if(is_a2dp)
    {
        //set a2dp flag and status
        //MMIBT_A2dpLinkConnected(msg_body_ptr);
        address = MMIBT_GetA2dAddress(msg_body_ptr);
    }
    else
    {
        address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
        //MMIBT_SetHfuStatus();
        MMIBT_StopOpenWaitTimer();
    }
	 //added tonny.chen
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
    if(MMIBT_IsOnTransferStatus())
    {
        CloseHeadset(&address);
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        //lulifei update the ui style
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_BT_DELETE_TRANSFERRING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        //MMIPUB_OpenConfirmationDialogByTextId(PNULL,STR_BT_DELETE_TRANSFERRING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif
    //end added

    if(is_a2dp)
    {
        //set a2dp flag and status
        MMIBT_A2dpLinkConnected(msg_body_ptr);
        //address = MMIBT_GetA2dAddress(msg_body_ptr);
    }
    else
    {
        //address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
        MMIBT_SetHfuStatus();
    }
    //SCI_TRACE_LOW:"MMIBT_OpenHeadsetConfirmIND address = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11101_112_2_18_2_8_23_355,(uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);

    BT_GetPairedDeviceName(&address, connect_dev_info.name);
	if(BT_SUCCESS != msg_body_ptr->status)
	{
		//SCI_TRACE_LOW:"MMIBT_OpenHeadsetConfirmIND msg_body_ptr->status %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11106_112_2_18_2_8_23_356,(uint8*)"d", msg_body_ptr->status);
		MMIDEFAULT_SetBackLight(TRUE);
		ClosePrmAndOpenDoneWin(
			MMIBT_EARCONN_WAIT_WIN_ID,
			STR_BT_URN_NO_DEV_FOUND,
			IMAGE_PUBWIN_FAIL,
			MMI_2SECONDS
			);
		return;
	}
    if(MMIBT_DeviceIsConnectedHeadset(&address))
    {
        //opened finished
        //this headset is opened
        if(MMIBT_OpenedHeadsetIsExist(&connect_dev_info))
        {
            //SCI_TRACE_LOW:"MMIBT MMIBT_OpenHeadsetConfirmIND enter, TXT_BT_HEADSET_OPENED"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11122_112_2_18_2_8_23_357,(uint8*)"");
            MMIDEFAULT_SetBackLight(TRUE);
			MMK_CloseWin(alert_id);
            MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
			MMI_GetLabelTextByLang(STR_BT_AUDIO_ACCESSORY, &wait_text);
	   //lulifei add to change the ui style
            if(s_bt_a2dp_show_connected_alert)
            {
                MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
                &connect_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleConnectCnfAlertWin);
            }
            s_bt_a2dp_show_connected_alert = TRUE;
            //MMIPUB_OpenConfirmationDialogByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
            //&connect_alert_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleConnectCnfAlertWin);

            MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_ACTIVE);
#ifdef MMI_BT_PBAP_SUPPORT
            MMIBT_GetBtStatusForIcon();
#endif
        }
#ifdef BT_BQB_SUPPORT
        MMIAPICC_IsCallingAndInfoBt();
        SCI_TRACE_LOW("MMIBT LOCAL void OpenHeadset, MMIAPICC_IsCallingAndInfoBt");
#endif
    }
    else
    {
        SCI_TRACE_LOW("MMIBT_OpenHeadsetConfirmIND Device is not connected headset");

        //special BT device VM Audio
        if (((0xB3 == address.addr[0]) && (0x33 == address.addr[1]) && (0x70 == address.addr[2]) && (0x7B == address.addr[3]) && (0xF0 == address.addr[4]) && (0x48==address.addr[5])) 
			|| ((0x9A == address.addr[0]) && (0x91 == address.addr[1]) && (0xD2 == address.addr[2]) && (0xB9 == address.addr[3]) && (0x62 == address.addr[4]) && (0xFC==address.addr[5])))
        {
            OpenHeadset(&address);
        }

        if(MMK_IsOpenWin(alert_id))
        {
            MMK_CloseWin(alert_id);
        }
        if(MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        }
    }

    if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID) && MMIBT_DeviceIsConnectedHeadset(&address))
    {
        //Update Device list
        MMK_PostMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_FINISHED, PNULL, PNULL);
    }
	
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT
    MMIAPIENVSET_SetBluetoothMode();
#endif

    //lulifei modify to update the status bar icon
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        MMIBT_UpdateBtIdleIcon(BT_GetState(),MMIBT_GetIsDeviceVisible());
    }
}

/*****************************************************************************/
//  Discription:check the deviece is connected headset
//  Global resource dependence: none
//  Author:
//  Note: if addr == NULL, it will check all paired device to find exist Connected Headset,
//        the connected headset will return in  out_addr
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsConnectedHeadset(BT_ADDRESS * in_addr)
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_single_service = TRUE;
    uint32      service_type = 0;

    if(PNULL != in_addr)
    {
        //only check in_addr is connected headset or not
        //get current earphone status, if is active do not allow to delete device
        service_type = BT_GetPairedDeviceService(in_addr);
        if ((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & service_type)
        {
            ret = MMIBT_IsDeviceActivedEarphone(in_addr);//MMIBT_IsDeviceActivedEarphone(in_addr);
            is_single_service = FALSE;
        }
    //#ifdef  BT_A2DP_SUPPORT
        if (BT_SERVICE_A2DP & service_type)
        {
            if(is_single_service)
            {
                ret = MMIBT_GetCurrentA2dpStatus(in_addr);
            }
            else
            {
                ret &= MMIBT_GetCurrentA2dpStatus(in_addr);
            }
        }
    //#endif
    }
    //SCI_TRACE_LOW:"MMIBT_DeviceIsConnectedHeadset ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9016_112_2_18_2_8_18_312,(uint8*)"d",ret);
    return ret;
}
/*****************************************************************************/
//  Discription:check exist opened headset or not
//  Global resource dependence: none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenedHeadsetIsExist(BT_DEVICE_INFO *out_dev)
{
    uint32 device_num = BT_GetPairedDeviceCount(BT_SERVICE_HANDSET);
    uint32 i = 0;
    BT_STATUS status = BT_SUCCESS;
    BT_DEVICE_INFO dev_info = {0};
    BOOLEAN   exist = FALSE;
    for(i = 1; i <= device_num; i++)
    {
        status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDSET, i, &dev_info);
        if(BT_SUCCESS == status)
        {
            if(MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr)))
            {
                exist = TRUE;
                if(PNULL != out_dev)
                {
                    SCI_MEMCPY(out_dev, &dev_info, sizeof(BT_DEVICE_INFO));
                }
                break;
            }
        }

    }//f
    //SCI_TRACE_LOW:"MMIBT_OpenedHeadsetIsExist exist %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9077_112_2_18_2_8_18_315,(uint8*)"d",exist);
    return exist;
}
/*****************************************************************************/
//  Discription:  MMIAPIBT_ Open Bt Headset List
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBtHeadsetList(void)
{
    MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_LIST_PAIRED);
    //MMIBT_EnterDeviceListWin(PNULL,MMIBT_OPC_MAIN,BT_SERVICE_HANDFREE);
    MMIBT_EnterInquireDeviceListWin(PNULL,MMIBT_OPC_MAIN,BT_SERVICE_HANDFREE);
}
/*****************************************************************************/
//  Description :  HandleConnectCnfAlertWin
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleConnectCnfAlertWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id,
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
    case MSG_CLOSE_WINDOW:
        MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_CONNECT);
        MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description :  HandleConnectCnfAlertWin
//  Global resource dependence :
//  Author:lifei.lu
//  Note:Close the win when click the OK for bug 480380
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDisconnectCnfAlertWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id,
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
    case MSG_CLOSE_WINDOW:
        MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return result;
}

#ifdef MMIBT_HEADSET_AUTHORIZE
/*****************************************************************************/
//  Discription: Set bluetooth device authorize status
//  Global resource dependence: none
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SetAuthorization(BT_MSG_T* msg_body_ptr)
{
    MMI_STRING_T    display_text = {0};
    MMI_STRING_T    device_name = {0};
    BT_ADDRESS     address = {0};
    BT_DEVICE_INFO connect_dev_info = {0};

    if(PNULL == msg_body_ptr)
    {
        SCI_TRACE_LOW("MMIBT_SetAuthorization msg_body_ptr is null");
        return;
    }

    address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
    SCI_TRACE_LOW("MMIBT_OpenHeadsetConfirmIND address = %x, %x, %x, %x, %x, %x",
            (uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);

    MMI_GetLabelTextByLang(STR_BT_SETT_CONN_TYPE, &display_text);

    BT_GetPairedDeviceName(&address, connect_dev_info.name);
    device_name.wstr_ptr = connect_dev_info.name;
    device_name.wstr_len = MMIAPICOM_Wstrlen(connect_dev_info.name);
    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_HFG_AUDIO_CONNECT);
    MMIBT_OpenQueryWin(MMI_BT_QUERY_WIN_ID, &display_text, &device_name, MMIBT_HFG_AUDIO_CONNECT);
}
#endif
#endif
