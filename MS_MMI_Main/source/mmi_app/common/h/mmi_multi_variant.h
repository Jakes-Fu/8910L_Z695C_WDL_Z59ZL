/*************************************************************************
 ** File Name:      mmi_multi_variant.h                                  *
 ** Author:         baokun.yin                                           *
 ** Date:           06/08/2015                                           *
 ** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the interface for multi-variant   *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 06/08/2015     baokun.yin        Create.                             *
*************************************************************************/
#ifndef _MMI_MULTI_VARIANT_H
#define _MMI_MULTI_VARIANT_H

#include "mn_type.h"
#include "mmi_module.h"

#include "nv_item_id.h"
#include "mmi_resource.h"

//#define DEBUG_TEST

#define ECC_MAX_COUNT 10
#define VOICEMAIL_MAX_COUNT 1
#define BOOKMARK_MAX_COUNT 10

#define NUMBER_STRING_LEN 32
#define VOICEMAIL_STRING_LEN 32
#define HOMEPAGE_STRING_LEN 128

//number 
typedef struct
{
	uint8 number[NUMBER_STRING_LEN];
	
}MMI_NUMBER_STRING_T;

//voice mail
typedef struct
{
	uint8 viocemail[VOICEMAIL_STRING_LEN];	
}MMI_VOICEMAIL_ITEM_T;

//homepage
typedef struct
{
	uint8  url[128];
}MMI_MULTI_VAR_URL_T;

/*****************************************************************************/
//  Description : read ECC number from multi-variant 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadECC(MMI_NUMBER_STRING_T *ecc_ptr);

/*****************************************************************************/
//  Description : read voice mail number from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadVoiceMail(MMI_VOICEMAIL_ITEM_T *voice_ptr);

/*****************************************************************************/
//  Description : read home page from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadHomePage(MMI_MULTI_VAR_URL_T *homepage_ptr);

/*****************************************************************************/
//  Description : read current home page from multi-variant
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_MULTIVAR_ReadCurHomePage(MMI_MULTI_VAR_URL_T *homepage_ptr);

/*****************************************************************************/
//  Description : read bookmark from DNT
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
#ifdef BROWSER_SUPPORT
PUBLIC BOOLEAN MMI_DNT_ReadBookMark(MMIRES_BOOKMARK_INFO_T *bookmark_ptr);
#endif
/*****************************************************************************/
//  Description : read emergency call ui configure from DNT
/*****************************************************************************/
PUBLIC uint8 MMI_DNT_ReadEmergencyCallUIConfigure(void);

#ifdef DEBUG_TEST
PUBLIC void MMI_TEST_MULTI_VARINT(void);
#endif

#endif
