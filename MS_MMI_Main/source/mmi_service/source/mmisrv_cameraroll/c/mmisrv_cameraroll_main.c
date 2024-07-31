
/*****************************************************************************
** File Name:      mmisrv_cameraroll_main.c                                         *
** Author:                                                                  *
** Date:                                                                     *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
                                *
******************************************************************************/

#include "os_api.h"
#include "priority_app.h"
#include "mmisrv_cameraroll_export.h"
#include "tasks_id.h"
#include "in_message.h"
#include "mmimsa_export.h"
#include "mmi_appmsg.h"

#define   P_CAMERA_ROLL_STACK_SIZE              4096    //6K
#define   P_CAMERA_ROLL_QUEUE_NUM               8

static BLOCK_ID g_cameraroll_task_id = SCI_INVALID_BLOCK_ID;

uint32 g_current_module_id = 0;
CAMERA_ROLL_TYPE_E s_cameraroll_init_from = CAMERA_ROLL_TYPE_NONE;
/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_download_thumbnail (void);

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_upload_photo (void);

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_Cancel_Task (void);

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void CAMERAROLL_Task (uint32 argc, void * argv);

/*****************************************************************************/
//  Description : create camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
#ifdef WIN32
#define tx_pri_PRI_CAMERA_ROLL_TASK   128
#endif
void CAMERAROLL_CreateTask (void)
{
    MMISRV_CAMERAROLL_MSG_T sig_data={0};	
	
	SCI_TRACE_LOW("[cameraroll]Enter CAMERAROLL_CreateTask !!!!s_cameraroll_init_from = %d " , s_cameraroll_init_from);
		
	MMISRV_CAMERAROLL_Load_Upload_File_array();
	MMIAPIMSA_get_token(FALSE ,MSA_APP_TYPE_CAMERA_ROLL , MMISRV_CAMERAROLL_Get_Token);
	MMISRV_CAMERAROLL_PdpActive();

	if(SCI_INVALID_BLOCK_ID == g_cameraroll_task_id)
	{
		g_cameraroll_task_id = SCI_CreateAppThread (
			"T_P_APP_CAMERAROLL_TASK",
			"Q_P_APP_CAMERAROLL_TASK",
			CAMERAROLL_Task,
			0,
			0,
			1024*20,
			100,
			PRI_CAMERA_ROLL_TASK,
			SCI_PREEMPT,
			SCI_AUTO_START);

    		SCI_TRACE_LOW("CAMERAROLL_CreateTask, g_cameraroll_task_id = %d" , g_cameraroll_task_id);
	}
}


/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void CAMERAROLL_Task (
    uint32 argc,
    void * argv
)
{
    CAMERA_ROLL_DOWNLOAD_THUMBNAIL_SIG_T    *psig = NULL;

    while (1) /*lint !e716*/
    {
        MMI_RECEIVE_SIGNAL ( (xSignalHeader*) psig, g_cameraroll_task_id);/*lint !e63*/
		//signal = (CAMERA_ROLL_DOWNLOAD_THUMBNAIL_SIG_T *)SCI_GetSignal(g_cameraroll_task_id);
       if(PNULL == psig){
            SCI_TRACE_LOW ("[MMISRV CARMERA ROLL]zyj CAMERAROLL_Task psig=null");/*lint !e613*/
            break;
        }        //SCI_TRACE_LOW ("[MMISRV CARMERA ROLL]CAMERAROLL_Task psig->SignalCode = 0x%x", psig->SignalCode);/*lint !e613*/
		
        switch (psig->sig.SignalCode) /*lint !e613*/
        {
        case CAMERA_ROLL_DOWNLOAD_THUMBNAIL:
			Handle_download_thumbnail();
			break;
		case CAMERA_ROLL_UPLOAD_PHOTO:
			Handle_upload_photo();
			break;
		case CAMERA_ROLL_CANCEL:
			Handle_Cancel_Task();
			break;
		case HTTP_SIG_INIT_CNF:
		case HTTP_SIG_NEED_AUTH_IND:
		case HTTP_SIG_REQUEST_ID_IND:
		case HTTP_SIG_GET_CNF:
		case HTTP_SIG_HEAD_CNF:
		case HTTP_SIG_DATA_IND:
		case HTTP_SIG_HEADER_IND:
		case HTTP_SIG_ERROR_IND:
		case HTTP_SIG_CLOSE_CNF:
		case HTTP_SIG_CANCEL_CNF:
		case HTTP_SIG_NET_PROG_IND:
		case HTTP_SIG_REDIRECT_IND:
		case HTTP_SIG_POST_CNF:
			MMISRV_CAMERAROLL_HandleHTTPMsg(PNULL , psig->sig.SignalCode , (DPARAM)psig);
	              SCI_TRACE_LOW ("[cameraroll] CAMERAROLL_Task: HTTP SigCode !!");
			break;
        default:
            SCI_TRACE_LOW ("[cameraroll] CAMERAROLL_Task: SigCode unknown");
            break;
        }
        SCI_TRACE_LOW ("[cameraroll] CAMERAROLL_Task: handle signal done , free Memory!");
        MMI_FREE_SIGNAL (psig);
    }
}

/**************************************************************************************/
// Description : delete camera roll thread
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
BOOLEAN MMISRV_CAMERAROLL_CreateTask(uint32 module_id , CAMERA_ROLL_TYPE_E type)
{
	g_current_module_id =  module_id;
	s_cameraroll_init_from = type;

	CAMERAROLL_CreateTask();
	return TRUE;
}

/**************************************************************************************/
// Description : delete camera roll thread
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMISRV_DeleteCameraRollTaskThread(void)
{
   if(SCI_INVALID_BLOCK_ID != g_cameraroll_task_id)
   {
	SCI_TRACE_LOW("[cameraroll]: Enter MMISRV_DeleteCameraRollTaskThread !!!!");  
	//SCI_DeleteThread(g_cameraroll_task_id);
	//g_cameraroll_task_id = SCI_INVALID_BLOCK_ID;//0 zyj
	MMISRV_CAMERAROLL_Destroy();
   }

   return SCI_SUCCESS;
}


/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_download_thumbnail (void)
{
    MMISRV_CAMERAROLL_Get_delta_token_info_req();
}

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_upload_photo (void)
{
	//Get upload linking list
	//Start upload
	MMISRV_CAMERAROLL_start_upload_file();
}

/*****************************************************************************/
//  Description : the function of the camera roll task
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void Handle_Cancel_Task (void)
{

}

/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
BOOLEAN MMISRV_CAMERAROLL_Download_Thumbnail(void)
{
	CAMERA_ROLL_DOWNLOAD_THUMBNAIL_SIG_T*sig_ptr = PNULL;

    SCI_ASSERT(SCI_NULL != g_cameraroll_task_id);/*assert verified*/
    
    sig_ptr = (CAMERA_ROLL_DOWNLOAD_THUMBNAIL_SIG_T *)SCI_ALLOCA(sizeof(CAMERA_ROLL_DOWNLOAD_THUMBNAIL_SIG_T));
    SCI_ASSERT(SCI_NULL != sig_ptr);/*assert verified*/
    SCI_MEMSET(sig_ptr, 0, sizeof(CAMERA_ROLL_DOWNLOAD_THUMBNAIL_SIG_T));
    
    //init the signal
    sig_ptr->sig.SignalCode   = CAMERA_ROLL_DOWNLOAD_THUMBNAIL;
    sig_ptr->sig.SignalSize   = sizeof(CAMERA_ROLL_DOWNLOAD_THUMBNAIL_SIG_T);
    sig_ptr->sig.Sender       = SCI_IdentifyThread();
        
    if(SCI_SUCCESS == SCI_SendSignal((xSignalHeader) sig_ptr, g_cameraroll_task_id))
    {
            return SCI_SUCCESS;                                                                                                                              
    }                                                                                                                                                    
    else                                                                                                                                                 
    {                                                                                                                                                    
            return SCI_ERROR;                                                                                                                                
    }                                                                                                                                                    
}


/**************************************************************************************/
// Description : start download thumbnail 
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
BOOLEAN MMISRV_CAMERAROLL_Upload_photo(void)
{
	CAMERA_ROLL_UPLOAD_PHOTO_SIG_T*sig_ptr = PNULL;

	MMI_CREATE_SIGNAL (sig_ptr,		
	               CAMERA_ROLL_UPLOAD_PHOTO,
	               sizeof (CAMERA_ROLL_UPLOAD_PHOTO_SIG_T),
	               P_APP
	              );

	MMI_SEND_SIGNAL (sig_ptr, g_cameraroll_task_id);
	return TRUE;
}

