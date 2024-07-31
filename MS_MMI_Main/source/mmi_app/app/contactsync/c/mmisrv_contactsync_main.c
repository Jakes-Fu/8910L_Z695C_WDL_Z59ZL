/****************************************************************************
** File Name:      mmisrv_contactsync_main.c                                             *
** Author:                                              	                *
** Date:           07/01/2015                                           	*
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file  define task of contactsync.                            *
**     这个文件contactsync task中的消息接受，处理和管理        *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2015       Yanyan.An         Create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "priority_app.h"
#include "mmisrv_contactsync_export.h"
#include "mmisrv_contactsync_nv.h"
#include "tasks_id.h"
#include "in_message.h"
#include "mmi_appmsg.h"
#include "window_parse.h"

#ifdef MMIPB_SYNC_WITH_O365
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/  

MMI_APPLICATION_T g_contactsync_app = {0};  

static BLOCK_ID g_pbsync_task_id = SCI_INVALID_BLOCK_ID;

//uint32 g_current_module_id = 0;

LOCAL uint16				s_timer_count = 0;

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : PBSYNC_InitApp
//	Global resource dependence : none
//  Author: Yanyan.an
//	Note:
/*****************************************************************************/
LOCAL void PBSYNC_InitApp(void);
/*****************************************************************************/
//  Discription: HandlePBSyncMsg
//  Global resource dependence: none 
//  Author: Yanyan.an 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePBSyncMsg(
								   PWND app_ptr,
								   uint16 msg_id,
								   DPARAM param
								  );

/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUCNTION IMPLEMENT                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register PbSync module
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_InitModule(void)
{
	MMI_RegPBSYNCNv();
}
/*****************************************************************************/
// 	Description :MMIAPIPBSYNC_Init
//	Global resource dependence : none
//  Author: Yanyan.an
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPBSYNC_Init(void)
{
	PBSYNC_InitApp();
}

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : PBSYNC_InitApp
//	Global resource dependence : none
//  Author: Yanyan.an
//	Note:
/*****************************************************************************/
LOCAL void PBSYNC_InitApp(void)
{
	SCI_TRACE_LOW("[PBSYNC] PBSYNC_InitApp---");
	g_contactsync_app.ProcessMsg = HandlePBSyncMsg;
	PBSYNC_DataProcDriveTblInit();
	MMIPB_SYNC_HTTP_Request_InitList();	

}

/*****************************************************************************/
//  Discription: HandlePBSyncMsg
//  Global resource dependence: none 
//  Author: Yanyan.an
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePBSyncMsg(
								  PWND app_ptr,
								  uint16 msg_id,
								  DPARAM param
								  )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        result = HandlePBSyncHttpMsg(app_ptr, msg_id, param);
    }
    else if (MSG_PBSYNC_START <= msg_id && msg_id <= MSG_PBSYNC_END)
    {
        result = HandlePBSyncAppMsg(app_ptr, msg_id, param);
    }

    return result;
}
#endif //MMIPB_SYNC_WITH_O365