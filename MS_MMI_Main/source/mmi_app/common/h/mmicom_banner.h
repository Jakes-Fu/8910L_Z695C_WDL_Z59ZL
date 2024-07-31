/****************************************************************************
** File Name:      mmicom_banner.h                                           *
** Author:                                                                  *
** Date:           2015/04/07                                                *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 4/2015        Yang.Yang            Create                                  *
**                                                                          *
****************************************************************************/
#ifndef MMICOM_BANNER_H
#define MMICOM_BANNER_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
typedef enum
{
	MMI_BANNER_TEXT,
	MMI_BANNER_ICONTEXT,
	MMI_BANNER_STR_TEXT,
	MMI_BANNER_STR_ICONTEXT,
	MMI_BANNER_TYPE_MAX
}MMI_BANNER_TYPE_E;

typedef struct  
{
	MMI_BANNER_TYPE_E banner_type;
	MMI_TEXT_ID_T     text1_id;
	MMI_TEXT_ID_T     text2_id;
	MMI_IMAGE_ID_T    image_id;
	GUI_POINT_T       start_point;
	MMI_STRING_T      text1_str;
	MMI_STRING_T      text2_str;
}MMICOM_BANNER_INFO_T;

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: Ä¬ÈÏ¾ÓÖÐ£» 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_OpenBannerWin(MMI_WIN_ID_T win_id,
											MMICOM_BANNER_INFO_T banner_info);

/*****************************************************************************/
//  Description : close banner
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPICOM_CloseBannerWin(uint32 keystatus, uint32 keycode);

/*****************************************************************************/
//  Description : MMIAPICOM_IsBannerWinOpen
//  Global resource dependence :
//  Author:dong.chunguang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsBannerWinOpen(void);

/*****************************************************************************/
//  Description : close banner
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPICOM_CloseBanner(void);
PUBLIC MMI_WIN_ID_T MMIAPICOM_GetBannerParentWin(void);
#endif  // MMICOM_BANNER_H
