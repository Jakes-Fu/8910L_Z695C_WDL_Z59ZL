/*****************************************************************************
** File Name:      mmibrowser_dorado_api.h                                          *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser api             *
*****************************************************************************/
#ifndef MMIBROWSER_DORADO_API_H
#define MMIBROWSER_DORADO_API_H

#ifdef BROWSER_SUPPORT_DORADO

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mn_type.h"
#include "mmibrowser_export.h"
#include "brw_control_api.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//一整块大内存申请到的概率较低，故采用分块申请的方式
#ifdef BZP_SUPPORT
#define MMIDORADO_MEMORY_GRANULARITY           (400*1024)  //0.5M;  MMIDORADO_MEMORY_GRANULARITY must = n*(256*1024)
#else
#define MMIDORADO_MEMORY_GRANULARITY           (500*1024)  //0.5M;  MMIDORADO_MEMORY_GRANULARITY must = n*(256*1024)
#endif
#define MMIDORADO_MEMORY_UNIT                 MMIDORADO_MEMORY_GRANULARITY 
#define MMIDORADO_MEMORY_SIZE                  (3 * MMIDORADO_MEMORY_UNIT)
#define MMIDORADO_MEMORY_SIZE_OPERA_IN_USE                  (2 * 500*1024)
#ifdef CMCC_UI_STYLE
#ifdef MMIDORADO_MEMORY_SIZE
#undef MMIDORADO_MEMORY_SIZE
#define MMIDORADO_MEMORY_SIZE                  (2 * MMIDORADO_MEMORY_UNIT)
#endif
#endif
#define MMIBROWSER_MAX_TITLE_LEN                  64   //the web page title that displays in address label ctrl
#define MMIBROWSER_INVALID_GATEWAY              "0.0.0.0"
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init browser
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_InitApp(void);

/*****************************************************************************/
//  Description : init browser
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_UninitApp(void);

/*****************************************************************************/
//  Description : check if browser is inited
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_IsRunning(void);

/*****************************************************************************/
//  Description : check if browser is the top win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_IsActive(void);

/*****************************************************************************/
//  Description : the entry of browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIDORADO_Entry(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr);

/*****************************************************************************/
//  Description : Clean cahe
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 清除浏览器和http的cache
/*****************************************************************************/
PUBLIC void  MMIDORADO_CleanCache(uint8 * url_ptr, uint16 url_len);

/*****************************************************************************/
//  Description : close browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIDORADO_Close(void);

/*****************************************************************************/
//  Description : init browser module
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_InitModule(void);

/*****************************************************************************/
//  Description : set net setting to default
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : handle control message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_HandleBrwControlMsg(MMI_MESSAGE_ID_E msg_id,void* param);

/*****************************************************************************/
//  Description : Clean Usert Data Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_CleanUserInfo(void);

/*****************************************************************************/
//  Description : Get browser Version 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDORADO_GetVersion(void);

/*****************************************************************************/
//  Description : get net setting index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIDORADO_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set net setting index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_SetNetSettingIndex(MN_DUAL_SYS_E dual_sys, uint8 index);

/*****************************************************************************/
//  Description : handle plug out sd card
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_SDCardPlugOut(void);

/*****************************************************************************/
//  Description : handle browser when format the storage 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_HandleStorageFormat(void);


/*****************************************************************************/
// 	Description : set watch dog pointer for dorado's js
//	Global resource dependence : 
//  Author:fen.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDORADO_SetWatchDogPtr(void *SwdgPtr);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function  Declare                                                              */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle url input from at+sytin cmd
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_AtestInsertURL(
                wchar * url_ptr,                 //input url 
                uint32 len                          //input url length 
                );

/*****************************************************************************/
//  Description : input bookmark at cmd into bookmark title edit box
//  Global resource dependence : 
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_AtestBookmarkTitleInput(
                wchar * wstr_ptr,             //point to the bookmark title information buffer    
                uint16  ilen                      //length of the information buffer
                );

/*****************************************************************************/
//  Description : input bookmark at cmd into bookmark URL edit box
//  Global resource dependence : 
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_AtestBookmarkURLInput(
                wchar * wstr_ptr,             //point to the bookmakr url buffer
                uint16  ilen                      //length of the information buffer
                );

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    End of Atest  Function  Declare                                                                  */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Dorado Set Default Bookmark
//  Global resource dependence : 
//  Author: dave.raun
//  Note:
/*****************************************************************************/
PUBLIC void MMIDORADO_SetDefaultBookmark(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

#endif//BROWSER_SUPPORT_DORADO
