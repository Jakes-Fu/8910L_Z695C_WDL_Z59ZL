/**
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_localinstallwin.c#1 $
 */

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include <window_parse.h>
#include "mmi_id.h"
//#include <mmipub.h>
#include "mmifmm_export.h"
#include "mmiudisk_export.h"
//#include <java_mmi_text.h>
#include "mmijava_export.h"
#include "mmijava_localinstallwin.h"

/*****************************************************************************/
//  Description : handle java select win
//  Global resource dependence :
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectWinMsg(
                                      MMI_WIN_ID_T          win_id,
                                      MMI_MESSAGE_ID_E      msg_id,
                                      DPARAM                param
                                      )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    FILEARRAY_DATA_T    file_data = {0};

    //SCI_TRACE_LOW("HandleSelectWinMsg msg_id = %x", msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            break;

        case MSG_FULL_PAINT:
            GUIWIN_SetTitleTextId(win_id, TXT_COM_LOCALINSTALL, FALSE);
            break;
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
			if (MSG_CTL_PENOK == msg_id && PNULL != param)
			{
				uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
				if(src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID || src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID || src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
				{
					result = MMI_RESULT_FALSE; 
					break;
				}
			}
#endif

            MMIAPIFMM_GetOpenFileWinCurFile(&file_data);
            //SCI_TRACE_LOW("HandleSelectWinMsg len = %d",file_data.name_len);
            if (FILEARRAY_TYPE_FILE == file_data.type)
            {
                //start install
                MMIAPIJAVA_Install(file_data.filename, MMIAPICOM_Wstrlen(file_data.filename));
                result = MMI_RESULT_TRUE;
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    //if (MMI_RESULT_FALSE == result)
    //{
    //    result = MMIFMM_HandleOpenFileByLayerWin(win_id, msg_id, param);
    //}

    return result;
}

/*****************************************************************************/
//  Description : Entry point for select install package.
//  Global resource dependence :
//  Author: Martin.Zhao
//  Note:
/*****************************************************************************/
PUBLIC void  MMIJAVA_StartLocalInstall(void)
{
    MMIFMM_FILTER_T         filter     = {0};
    MMIFMM_FUNC_TYPE_E      func_type  = 0;
    uint32                  suffix_filter = MMIFMM_JAVA_JAR|MMIFMM_JAVA_JAD;

    if (!MMIAPIUDISK_IsDeviceReady())
    {
        SCI_TRACE_LOW("Error: Device is not ready!");
        return;
    }

    MMIAPIFMM_GetFilerInfo(&filter,suffix_filter);
    func_type = FUNC_FIND_FILE_AND_ALLFOLER;
    MMIAPIFMM_OpenFileWinByLayer(PNULL, 0,&filter, func_type, FALSE,HandleSelectWinMsg, NULL, NULL,FALSE);
}

