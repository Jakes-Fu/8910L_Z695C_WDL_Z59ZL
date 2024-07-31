/*! @file mcare_interface.h
@brief 此文件提供了外部调用MCARE的接口，以及提供判断MCARE和QQ等应用是否在运行的接口
@author wilsonzhu wiltan
@version 1.0
@date 06/06/2011
@remarks Copyright: 2011 Tencent, Incorporated. All Rights Reserved.
@details 
@note 06/2011       wilsonzhu wiltan          Create        
*******************************************************************************/
#ifndef _MCARE_INTERFACE_H_
#define _MCARE_INTERFACE_H_

#include "mcfcommon.h"
#include "os_api.h"

#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937) ||(defined SPREAD_PLAT_SC6620)
#include "image_proc.h"
#endif

//////////////////////////McareV30接口封装begin/////////////////////////////////////////

MVOID MCareV31_EntryApp(MINT app_type);
MVOID MCareV31_RunApp(MVOID);

MINT MCareV31_GetAppTypeMcare(MVOID);
MINT MCareV31_GetAppType(MVOID);
MVOID MCareV31_SetAppType(MINT app_type);

MVOID MCareV31_Main_Entry(MVOID);
MVOID MCareV31_QQBrowser_Shortcut(MVOID);
//MVOID MCareV31_QQ_Shortcut(MVOID);
MVOID MCareV31_QQContact_Shortcut(MVOID);
MVOID MCareV31_QQLord_Shortcut(MVOID);
MVOID MCareV31_QQSoso_Shortcut(MVOID);
MVOID MCareV31_QQZone_Shortcut(MVOID);
//get widget soso url   nicklei add 
MVOID  MCareV31_Wigdet_SOSO(char* pData);

/////判断Mcare是否在运行，不区分前台和后台
MBOOL McareV31_IsMcareRunning(MVOID);
MBOOL McareV31_IsMcareRunningFront(MVOID);
MBOOL McareV31_IsMcareRunningBg(MVOID);

/////////mcare退出的提示
MBOOL MCareV31_Exit_Confirm(MVOID);
MBOOL  McareV31_ExitMcare(MVOID);
MBOOL  McareV31_RunToIdle(MVOID);

MVOID MCareV31_StartWapBrowser(MCHAR *url);


MBOOL MCareV31_GetKeyDownSlide(MVOID);
MVOID MCareV31_SetKeyDownSlide(MBOOL isKeyDownSlide);

#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937) ||(defined SPREAD_PLAT_SC6620)
MVOID MCareV31_NotifyGifDecodeOneFrame(IMGPROC_GIF_DISPLAY_INFO_T   * display_info_ptr);
MBOOL MCareV31_IsContinueDecodeGif(MVOID);
MBOOL MCareV31_IsAppDecodeGif(MVOID);
#endif

MCHAR* McareV31_GetMcareTextUrl(MCHAR* pURL);

MINT McareV31_GetBGType(MVOID);


MVOID McareV31_CleanDataInEng(MVOID);


////进入Mcare的操作
MBOOL  McareV31_EntryMcareCallBack(MVOID);
////退出Mcare的操作
MBOOL  McareV31_ExitMcareCallBack(MVOID);
////将要退出Mcare的操作
BOOLEAN  McareV31_WillExitMcareCallBack(void);

////进入Mcare前台的操作
BOOLEAN  McareV31_EntryMcareFrontCallBack(void);
////进入Mcare后台的操作
BOOLEAN  McareV31_ExitMcareBackCallBack(void);

////进入Mcare界面的操作
BOOLEAN  McareV31_EntryMcareWinCallBack(void);
////退出Mcare界面的操作
BOOLEAN  McareV31_ExitMcareWinCallBack(void);

MBOOL McfIF_SupportAppBG(MUINT app_id);


//////////////////////////McareV31接口封装end/////////////////////////////////////////
#endif//_MCARE_INTERFACE_H_


