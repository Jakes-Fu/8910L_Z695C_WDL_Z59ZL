/****************************************************************************
** File Name:      mmigps_wintab.c                                         *
** Author:         jian.ma                                                 *
** Date:           07/19/2007                                              *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/19/2007    jian.ma          Create                                   *
**                                                                         *
****************************************************************************/
//#ifndef _MMIGPS_WINTAB_C_
#define _MMIGPS_WINTAB_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_gps_trc.h"
#include "std_header.h"
#include "sci_types.h"
#ifdef GPS_SUPPORT
#include "window_parse.h"
#include "mmigps_id.h"
#include "mmigps_export.h"
#include "mmipub.h"
#include "mmigps_text.h"
#include "mmigps_osapi.h"
#include "mmi_modu_main.h"
#include "mmigps_internal.h"
#include "mmi_default.h"
#include "mmitv_out.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                  */
/*---------------------------------------------------------------------------*/
//const char g_pDestFolder[] = "D:\\GPS_MAP";
//const char g_pSourceFolder[]= "E:\\GPS_MAP";
MMI_WIN_ID_T g_gpsWnd = 0;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


//extern void PlayPCMData( char * pBuffer,long len );

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:   handle gps win message
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGpsWinMsg( 
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      );
/*****************************************************************************/
//	Description : calculate the coordinates of the press point in map
//	Global resource dependence : none
//	Author: Ming.Song
//	Note:
/*****************************************************************************/
LOCAL GPS_POINT_T PressAt(
                             DPARAM param
                             );


/*****************************************************************************/
//	Description : open gps main win
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
//PUBLIC MMI_RESULT_E MMIGPS_CopyResource(uint16* src_path_ptr,uint16* dest_path_ptr);

/*****************************************************************************/
//	Description : close GPS exception 
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
//PUBLIC void MMIGPS_CloseGPSException( void );

// the window for new gps
WINDOW_TABLE( GPS_NEW_WIN_TAB ) = 
{
	//CLEAR_LCD,
//	WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32) HandleGpsWinMsg ),    
	WIN_ID( GPS_MAIN_WIN_ID),
	END_WIN
}; 

/*****************************************************************************/
//  Discription:   handle gps win message
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGpsWinMsg( 
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	//static BOOLEAN s_is_key_tone_enable = 0;
    //static BOOLEAN s_is_tp_tone_enable = 0;
	BLOCK_ID	gpsTask = MMIGPS_GetGPSTaskID();
	
	//SCI_TRACE_LOW:"HandleGpsWinMsg,%x,gpsTask:%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_103_112_2_18_2_21_54_38,(uint8*)"dd",msg_id,gpsTask);
	switch(msg_id)
	{
	case MSG_GPS_READ_EXCEPTION:
		{
			uint32			uiTime = 2000;
			MMI_WIN_PRIORITY_E	level = WIN_THREE_LEVEL;
			//MMIPUB_HANDLE_FUNC	temp = NULL;
				  	
			if( gpsTask )
			{
				SCI_DeleteThread( gpsTask );
				SCI_TerminateThread(gpsTask );
				MMIGPS_SetGpsState(0);
	 			MMK_CloseWin( g_gpsWnd );
			}
			gpsTask = 0;
			MMIPUB_OpenAlertWinByTextId( &uiTime,TXT_GPS_NO_RES,GPS_TXT_NULL,
										IMAGE_PUBWIN_WARNING,
										  	NULL,
										  	&level,
										  	MMIPUB_SOFTKEY_NONE,
										  	PNULL );
		}
		break;
	case MSG_OPEN_WINDOW:
		MMIGPS_SetPermitPlay(TRUE);
		
		g_gpsWnd = win_id;
		
		MMIGPS_SetGpsState(1);
		MMIAPIMP3_StopAudioPlayer(); 
		
		if( gpsTask == 0 )
		{
			gpsTask = SCI_CreateAppThread( "GPSTask","GPSTask",GPSTask,0,NULL,0x8000,6,PRI_GPS_TASK,SCI_PREEMPT,SCI_AUTO_START );
			MMIGPS_SetGPSTaskID(gpsTask);
		}
		else
		{
			SCI_ResumeThread( gpsTask );
		}	
		break;

    case MSG_FULL_PAINT:
		MMIGPS_DelMessage( 0x0D,0,0 );
		break;
	case MSG_KEYUP_OK:
		MMIGPS_DelMessage( 1,4224,0 );
		break;
	case MSG_KEYDOWN_OK:
		//SCI_TraceLow:"MSG_KEYDOWN_OK"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_154_112_2_18_2_21_55_39,(uint8*)"");
		MMIGPS_DelMessage( 2,4224,0 );
		break;
	case MSG_KEYUP_CANCEL:
		MMIGPS_DelMessage( 1,4098,0 );
		break;
	case MSG_KEYDOWN_CANCEL:
		//MMK_CloseWin(win_id);
		MMIGPS_DelMessage( 2,4098,0 );
		break;
	case MSG_KEYUP_LEFT:
		MMIGPS_DelMessage( 1,4104,0 );
		//PressTowards(-1,0);
		break;
	case MSG_KEYDOWN_LEFT:
	case MSG_KEYDOWN_4:
		//SCI_TraceLow:"MSG_KEYDOWN_LEFT"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_170_112_2_18_2_21_55_40,(uint8*)"");
		MMIGPS_DelMessage( 2,4104,0 );
		//PressTowards(-1,0);
		break;
	case MSG_KEYUP_RIGHT:
		MMIGPS_DelMessage( 1,4100,0 );
		break;
	case MSG_KEYDOWN_RIGHT:
	case MSG_KEYDOWN_6:
		//PressTowards(1,0);
		//SCI_TraceLow:"MSG_KEYDOWN_RIGHT"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_180_112_2_18_2_21_55_41,(uint8*)"");
		MMIGPS_DelMessage( 2,4100,0 );
		break;
	case MSG_KEYUP_UP:
		MMIGPS_DelMessage( 1,4112,0 );
		break;
	case MSG_KEYDOWN_UP:
	case MSG_KEYDOWN_2:
		//SCI_TraceLow:"MSG_KEYDOWN_UP"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_188_112_2_18_2_21_55_42,(uint8*)"");
		//PressTowards(0,-1);
		MMIGPS_DelMessage( 2,4112,0 );
		break;
	case MSG_KEYUP_DOWN:
		MMIGPS_DelMessage( 1,4128,0 );
		break;
	case MSG_KEYDOWN_DOWN:
	case MSG_KEYDOWN_8:
		//SCI_TraceLow:"MSG_KEYDOWN_DOWN"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_197_112_2_18_2_21_55_43,(uint8*)"");
		//PressTowards(0,1);
		MMIGPS_DelMessage( 2,4128,0 );
		break;

    case MSG_KEYDOWN_HASH:
    	//NavigationMain();
		//gps_Go_Back();
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
	 	{
	 		GPS_POINT_T press_point = {0};
	 		unsigned long ul = 0;
	 	 	press_point = PressAt( param );
	 	 	ul = press_point.y;
	 	 	ul = (ul << 16)|press_point.x;
	 	 	//SCI_TraceLow:"MSG_TP_PRESS_DOWN,point x=%d,y=%d"
	 	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_214_112_2_18_2_21_55_44,(uint8*)"dd",press_point.x,press_point.y );
	 	 	//SCI_TraceLow:"ul=%d"
	 	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_215_112_2_18_2_21_55_45,(uint8*)"d",ul );
			MMIGPS_DelMessage( 3,0,ul);
		//HandlePress(param);
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
    	{
	 		GPS_POINT_T press_point = {0};
	 		unsigned long ul = 0;
	 	 	press_point = PressAt( param );
	 	 	ul = press_point.y;
	 	 	ul = (ul << 16)|press_point.x;
			//SCI_TraceLow:"MSG_TP_PRESS_UP,point x=%d,y=%d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_227_112_2_18_2_21_55_46,(uint8*)"dd",press_point.x,press_point.y );
	 	 	//SCI_TraceLow:"ul=%d"
	 	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_228_112_2_18_2_21_55_47,(uint8*)"d",ul );
			MMIGPS_DelMessage( 4,0,ul);
		}
     	break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
    	{
	 		GPS_POINT_T press_point = {0};
	 		unsigned long ul = 0;
	 	 	press_point = PressAt( param );
	 	 	
	 	 	ul = press_point.y;
	 	 	ul = (ul << 16)|press_point.x;
	 	 		 	 	
	 	 	//SCI_TraceLow:"MSG_TP_PRESS_MOVE"
	 	 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_241_112_2_18_2_21_55_48,(uint8*)"");
			MMIGPS_DelMessage( 5,0,ul);
		}
     	break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_LOSE_FOCUS:
	 MMK_CloseWin(win_id);
     	//MMIGPS_SetGpsState(0);
     	//MMIGPS_StopPlay();
     	
     	break;
     case MSG_GET_FOCUS:
     	//MMIDEFAULT_AllowTurnOffBackLight(FALSE);
     	//MMIGPS_SetGpsState(1);
     	break;
     case MSG_APP_RED:
	{
		SCI_SLEEP(200);
		recode = MMI_RESULT_FALSE;
	}
     	break;
    case MSG_CLOSE_WINDOW:
     	MMIGPS_StopPlay();
     	MMIGPS_SetPermitPlay(FALSE);
     	g_gpsWnd = 0;
     	
        if( MMIGPS_GetGpsState( ) )
     	{
     		MMIGPS_DelMessage( 0xFF,0,0 );
     	}
     	// 等待GPS退出
     	while( MMIGPS_GetGpsState( ) )
     	{
     		SCI_SLEEP( 50 );
     	}
     	
     	RituGps_OsFree(); 
	//SCI_TRACE_LOW:"close gps win over"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_277_112_2_18_2_21_55_49,(uint8*)"");
    	break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}

//**********************************************************
// 拷贝资源目录
// 
//***********************************************************
MMI_RESULT_E GPS_CopyResource(uint16* src_path_ptr,uint16* dest_path_ptr )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32			uiTime = 3000;

	if(PNULL==dest_path_ptr ||PNULL==src_path_ptr)
	{
	    //SCI_TRACE_LOW:"GPS_CopyResource  dest_path_ptr OR src_path_ptr is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_297_112_2_18_2_21_55_50,(uint8*)"");
	    return PNULL;
	}
	//SCI_TRACE_LOW:"GPS_CopyResource begin"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_WINTAB_300_112_2_18_2_21_55_51,(uint8*)"");

	SFS_CreateDirectory(dest_path_ptr);
	if(SFS_NO_ERROR != MMIGPS_CopyDir(src_path_ptr,dest_path_ptr))
	{	
		MMIPUB_OpenAlertWinByTextId( &uiTime,TXT_GPS_COPYFALSE,GPS_TXT_NULL,
									IMAGE_PUBWIN_WARNING,
									  	NULL,
									  	PNULL,
									  	MMIPUB_SOFTKEY_NONE,
									  	PNULL );
		result = MMI_RESULT_FALSE;
	}
	MMIPUB_CloseWaitWin(MMIGPS_WAITING_WIN_ID);	
	return result;
	 

}
/*****************************************************************************/
//  Description : handle gps waiting window
//  Global resource dependence : 
//  Author: Juan.zhang
//  Note:
/*****************************************************************************/
MMI_RESULT_E MMIGPS_HandleWaitWinMsg(
                                     MMI_WIN_ID_T      win_id, 
                                     MMI_MESSAGE_ID_E  msg_id,
                                     DPARAM            param
                                     )
{
    MMI_RESULT_E    	result = MMI_RESULT_TRUE;
	uint16 			pDest[16]={0};
	const uint8 		pstrDest[] 	= "D:\\GPS_MAP";
	uint16 			pSource[16]={0};
	const uint8		pstrSource[]= "E:\\GPS_MAP";

	GUI_GB2UCS( pDest,pstrDest,SCI_STRLEN((void*)pstrDest ) );
	GUI_GB2UCS( pSource,pstrSource,SCI_STRLEN((void*) pstrSource ) );
    switch (msg_id)
    {
	case MSG_FULL_PAINT:
		MMIPUB_HandleWaitWinMsg(MMIGPS_WAITING_WIN_ID,MSG_FULL_PAINT,0);
		MMK_PostMsg(MMIGPS_WAITING_WIN_ID, MSG_GPS_COPY_RES_IND, 0,0);
		break;
	case MSG_GPS_COPY_RES_IND:
		if(MMI_RESULT_TRUE == GPS_CopyResource(pSource,pDest))
		{
			MMK_CreateWin((uint32 *)GPS_NEW_WIN_TAB, PNULL);
		}
		break;

    default:
    		MMIPUB_HandleWaitWinMsg(MMIGPS_WAITING_WIN_ID,msg_id,0);
        break;
    }
    return (result);
}

/*****************************************************************************/
//	Description : calculate the coordinates of the press point in map
//	Global resource dependence : none
//	Author: Ming.Song
//	Note:
/*****************************************************************************/
LOCAL GPS_POINT_T PressAt(
                             DPARAM param
                             )
{

	GPS_POINT_T press_point = {0};//触笔点在地图中的坐标
	press_point.x = MMK_GET_TP_X(param);
	press_point.y = MMK_GET_TP_Y(param);
	
	return press_point;
}

/*****************************************************************************/
//	Description : open gps main win
//	Global resource dependence : none
//	Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIGPS_OpenMainWin(void)
{
	uint32				uiTime = 3000;
	MMI_WIN_PRIORITY_E	level = WIN_THREE_LEVEL;
	uint16 				pDest[16]={0};
	const uint8 			pstrDest[] 	= "D:\\GPS_MAP";
	uint16 				pSource[16]={0};
	const uint8			pstrSource[]= "E:\\GPS_MAP";
	uint32 				iLen = 0;
	MMI_STRING_T        	prompt_str = {0};
	
	if(!MMIAPITVOUT_GetStartingStatus())
	{
		iLen = GUI_GB2UCS( pDest,pstrDest,SCI_STRLEN((void*)pstrDest ) );
		if(!GPS_IsFolderExist(pDest,iLen/2))
		{
			iLen 	= GUI_GB2UCS( pSource,pstrSource,SCI_STRLEN((void*) pstrSource ) );
			if(GPS_IsFolderExist(pSource,iLen/2))
			{
				SCI_MEMSET(&prompt_str, 0, sizeof(prompt_str));
				MMI_GetLabelTextByLang(TXT_GPS_COPYYING, &prompt_str);
				

				MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMIGPS_WAITING_WIN_ID,IMAGE_NULL,
		                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,MMIGPS_HandleWaitWinMsg);
			}
			else
			{
				MMIPUB_OpenAlertWinByTextId( &uiTime,TXT_GPS_NO_RES,GPS_TXT_NULL,
											IMAGE_PUBWIN_WARNING,
											  	NULL,
											  	PNULL,
											  	MMIPUB_SOFTKEY_NONE,
											  	PNULL );
			}
		}
		else
		{
    			MMK_CreateWin((uint32 *)GPS_NEW_WIN_TAB, PNULL);
    		}
    }
    else
    {
    	MMIPUB_OpenAlertWinByTextId( &uiTime,TXT_GPS_STOP_TVOUT,GPS_TXT_NULL,
									IMAGE_PUBWIN_WARNING,
								  	NULL,
								  	&level,
								  	MMIPUB_SOFTKEY_NONE,
								  	PNULL );
    }
}

/*****************************************************************************/
//	Description : close GPS exception 
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_CloseGPSException( void )
{
	MMK_PostMsg(g_gpsWnd,MSG_GPS_READ_EXCEPTION,NULL,0  );
}

/*****************************************************************************/
//	Description : close GPS win
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_CloseWind( void )
{
	if( g_gpsWnd )
	{
		MMIGPS_SetGpsState(0);
	 	MMK_CloseWin( g_gpsWnd );
	}
}
#endif


/*Edit by script, ignore 3 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527
