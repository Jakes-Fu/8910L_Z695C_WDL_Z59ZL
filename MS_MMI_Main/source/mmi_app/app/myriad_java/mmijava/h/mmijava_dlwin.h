/*****************************************************************************
** File Name:      mmijava_dlwin.h                                                
** Author:         murphy.xie   
** Date:           2012/08/15
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe java download function
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2012/08/15        murphy.xie     Create                                 *
******************************************************************************/


#ifndef  _MMIJAVA_DOWNLOADWIN_H_
#define  _MMIJAVA_DOWNLOADWIN_H_

#ifdef SUPPORT_DOWNLOAD
#include "mmk_type.h"


#define     MMIJAVA_MAX_DOWNLOAD_URL_LEN           512
#define     MMIJAVA_DOWNLOAD_DEFAULT_URL_LEN       20


typedef struct
{
    uint8    url[MMIJAVA_MAX_DOWNLOAD_URL_LEN +1];  //download url.
    uint16   url_len;                               //url length.
}MMIJAVA_DOWNLOAD_ITEM_DETAIL_T;

typedef struct
{
    MMIJAVA_DOWNLOAD_ITEM_DETAIL_T game_info;       //game download url
    MMIJAVA_DOWNLOAD_ITEM_DETAIL_T ebook_info;      //ebook download url
    MMIJAVA_DOWNLOAD_ITEM_DETAIL_T other_info;      //other download url
}MMIJAVA_DOWNLOAD_DETAIL_T;



/**************************************************************************************************/
// Description:Download entry for called in other module.
// Author:Martin.Zhao
// Date:2010.11.22
// Note:
//*************************************************************************************************/
PUBLIC void MMIJAVA_EnterDownload(int32 url_type);
/**************************************************************************************************
  * Description: MMIJAVA_EnterDownLoadSettingWin
  * Input Paras:
  * Output Paras: 
  * note : call the func when deleting all
  *************************************************************************************************/
PUBLIC void  MMIJAVA_EnterDownLoadSettingWin(uint32 suiteid);

#endif


#endif //_MMIJAVA_DOWNLOADWIN_H_




