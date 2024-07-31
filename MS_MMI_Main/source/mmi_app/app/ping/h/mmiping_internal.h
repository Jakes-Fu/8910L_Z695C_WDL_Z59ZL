/*****************************************************************************
** File Name:      contactsync_app.h                                         *
** Author:                                                                   *
** Date:           09/2020                                                   *
** Description:    This file deal with contact sync .                        *
***************************************************************************/
#ifdef MMIENG_PING_SUPPORT
#include "mmiconnection_export.h"

#define MMIPING_STRNUM_MAX_LEN                    80
#define MMIPING_WEBADDRESS_MAX_URL_LEN 128
#define MMIPING_MAX_COUNT_LEN 5
#define MMIPING_MAX_PORT_VALUE 10

typedef uint8 RES_PING_STR[1024];
typedef enum
{
    MMIPING_SELECT_NORMAL,              
    MMIPING_SELECT_LONG_MODE,
    
    MMIPING_SELECT_MAX           
}MMIPING_SELECT_MODE_E;

typedef struct
{   
    uint16 count;
    uint8  homepage[MMIPING_WEBADDRESS_MAX_URL_LEN + 1];	
    uint16 homepage_len; 
#ifdef IPVERSION_SUPPORT_V4_V6
    MMICONNECTION_IP_TYPE_E ip_type;
#endif
}MMIPING_SETTING_INFO_T;

LOCAL MMI_RESULT_E HandleShowPacketInfoWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E MMIENG_HandlePingModeSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandlePingSelectModeWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandlePingInputWebAddressWinMsg(
                                                   MMI_WIN_ID_T	    win_id,
                                                   MMI_MESSAGE_ID_E	msg_id,
                                                   DPARAM	        param
                                                   );

LOCAL MMI_RESULT_E HandlePingInputCntMsg(
                                        MMI_WIN_ID_T       win_id,
                                        MMI_MESSAGE_ID_E   msg_id,
                                        DPARAM             param
                                        );
LOCAL MMI_RESULT_E HandlePingSettingIpTypeMsg(
                                              MMI_WIN_ID_T  	win_id,
                                              MMI_MESSAGE_ID_E	msg_id,
                                              DPARAM			param
                                              );                                        
#endif

