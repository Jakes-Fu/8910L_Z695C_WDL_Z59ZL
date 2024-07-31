/*****************************************************************************
** File Name:      mmifmm_comwin.c                                           *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#define _MMIFMM_COMWIN_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmifmm_comwin.h"
#include "mmk_msg.h"
#include "mmk_app.h"
#include "mmi_text.h"
#include "mmifmm_id.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_appmsg.h"
#include "mmi_filetask.h"
#include "mmi_image.h"
#include "mmi_modu_main.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : HandleFmmQueryWin
//  Global resource dependence :
//      Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleOverWriteQueryWin (MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIFMM_QUERYWIN_DATA_T *win_data_p = (MMIFMM_QUERYWIN_DATA_T*) MMK_GetWinUserData (win_id);

    switch (msg_id)
    {
    case MSG_APP_OK:
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:

        if (PNULL != win_data_p)
        {
            if (MMK_IsOpenWin (win_data_p->win_id))
            {
                MMK_SendMsg (win_data_p->win_id, MSG_PROMPTWIN_OK, PNULL);
            }
        }

        MMK_CloseWin (win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

        //enter fmm explorer
        if (PNULL != win_data_p)
        {
            if (MMK_IsOpenWin (win_data_p->win_id))
            {
                MMK_SendMsg (win_data_p->win_id, MSG_PROMPTWIN_CANCEL, PNULL);
            }
        }

        MMK_CloseWin (win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMIFMM_QueryCloseCtrlFunc (win_id);
        break;
    default:
        result = MMIPUB_HandleQueryWinMsg (win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_QueryAddCtrlFunc (
    MMI_WIN_ID_T win_id,
    MMIFMM_QUERYWIN_DATA_T *query_data
)
{
    if (PNULL != query_data)
    {
        MMIFMM_QUERYWIN_DATA_T *query_data_ptr = (MMIFMM_QUERYWIN_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_QUERYWIN_DATA_T));

        query_data_ptr->win_id = query_data->win_id;
        query_data_ptr->win_id2 = query_data->win_id2;

        MMK_SetWinUserData (win_id, (void *) query_data_ptr);

        return MMI_RESULT_TRUE;
    }

    return MMI_RESULT_FALSE;

}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIFMM_QueryCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_QUERYWIN_DATA_T *win_data_p = (MMIFMM_QUERYWIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        SCI_FREE (win_data_p);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : open wait win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_OpenComWaitWin (MMIPUB_HANDLE_FUNC    func)
{
    MMI_STRING_T wait_text = {0};
    MMI_GetLabelTextByLang (STR_NOTE_WAITING, &wait_text);
    MMIPUB_OpenWaitWin (1,
                        &wait_text,
                        PNULL,
                        PNULL,
                        MMIFMM_FILE_WAITING_WIN_ID,
                        IMAGE_NULL,
                        ANIM_PUBWIN_WAIT,
                        WIN_ONE_LEVEL,
                        MMIPUB_SOFTKEY_ONE,
                        (MMIPUB_HANDLE_FUNC) func);
}



#ifdef ZDT_VIDEO_KING_SUPPORT  
#define MMIFMM_MEDIA_MAX_FILE_NAME_LEN          256
PUBLIC void MMIFMM_OpenFileList(uint16 m_index)
{
    const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    wchar*              full_path = PNULL;
    uint16              full_path_len = MMIFMM_MEDIA_MAX_FILE_NAME_LEN;
    MMIFILE_DEVICE_E    file_dev = MMIAPIFMM_GetFirstValidSD();
    uint32              file_suffix = MMIFMM_FILE_TYPE(MMIFMM_MOVIE_ALL) |MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_3GP) |MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_MP4) |MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_AVI) ;

    if((MMI_DEVICE_NUM <= file_dev) || (file_dev <= MMI_DEVICE_UDISK))
    {
        MMIPUB_OpenAlertWinByTextId (PNULL, STR_NOTE_NO_CARD, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return;
    }

    full_path = SCI_ALLOC_APP((MMIFMM_MEDIA_MAX_FILE_NAME_LEN )*sizeof(wchar));
    if (PNULL == full_path)
    {
        return;
    }
    SCI_MEMSET(full_path, 0, ((MMIFMM_MEDIA_MAX_FILE_NAME_LEN )* sizeof(wchar)));

	file_dev_ptr = MMIAPIFMM_GetDevicePath((file_dev));
	file_dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));
    if(m_index == 0)
    {
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_01, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_01),PNULL, 0, full_path, &full_path_len);
    }
    else if(m_index == 1)
    {
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_02, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_02),PNULL, 0, full_path, &full_path_len);
    }
    else if(m_index == 2)
    {
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_03, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_03),PNULL, 0, full_path, &full_path_len);
    }
    else if(m_index == 3)
    {
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_04, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_04),PNULL, 0, full_path, &full_path_len);
    }
    else if(m_index == 4)
    {
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_05, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_05),PNULL, 0, full_path, &full_path_len);
    }
    else if(m_index == 5)
    {
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_06, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_06),PNULL, 0, full_path, &full_path_len);
    }
    else if(m_index == 6)
   {
     MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE),PNULL, 0, full_path, &full_path_len);
   }
   else if(m_index == 7)
   {
	MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_08, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_08),PNULL, 0, full_path, &full_path_len);
   }
   else if(m_index == 8)
   {
	MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_09, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_09),PNULL, 0, full_path, &full_path_len);
   }
   else if(m_index == 9)
   {
	MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_VIDEO_KING_10, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VIDEO_KING_10),PNULL, 0, full_path, &full_path_len);
    }
    else
    {
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE),PNULL, 0, full_path, &full_path_len);
    }
#ifdef DRM_SUPPORT
    file_suffix |= MMIFMM_DRM_ALL;
#endif
    if(MMIAPIFMM_IsFolderExist(full_path, full_path_len))
    {
        MMIAPIFMM_OpenExplorerMedia(full_path, full_path_len,PNULL,0,TRUE, file_suffix);    //MMIFMM_FILE_ALL
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
    }
    SCI_FREE(full_path);
}
#endif
