/*****************************************************************************
** File Name:      ctrltimeedit_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLTIMEEDIT_EXPORT_H_
#define _CTRLTIMEEDIT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasefixed_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @enum GUIEDIT_TIME_STYLE_E
@brief ʱ������
*/
typedef enum
{
    GUIEDIT_TIME_STYLE_NULL,    /*!<����λ*/
    GUIEDIT_TIME_STYLE_12,      /*!<12Сʱ��*/
    GUIEDIT_TIME_STYLE_24,      /*!<24Сʱ��*/
    GUIEDIT_TIME_STYLE_TIMER,   /*!<��ʱ��*/
    GUIEDIT_TIME_STYLE_MAX      /*!<����λ*/
} GUIEDIT_TIME_STYLE_E;

/*! @enum GUIEDIT_TIME_DISPLAY_STYLE_E
@brief ʱ��༭����ʾ����
*/
typedef enum
{
    GUIEDIT_TIME_DISPLAY_STYLE_NULL,/*!<����λ*/
    GUIEDIT_TIME_DISPLAY_STYLE_HM,  /*!<��ʾʱ:��*/
    GUIEDIT_TIME_DISPLAY_STYLE_HMS, /*!<��ʾʱ:��:��*/
    GUIEDIT_TIME_DISPLAY_STYLE_MAX  /*!<����λ*/
} GUIEDIT_TIME_DISPLAY_STYLE_E;

typedef enum
{
	GUIEDIT_TIME_DEFAUL_DISPLAYTYPE,
    GUIEDIT_TIME_NORMAL_DISPLAYTYPE,
    GUIEDIT_TIME_DIALOG_DISPLAYTYPE,    
    GUIEDIT_TIME_COMBINE_DATE_DISPLAYTYPE,    
    GUIEDIT_TIME_MAX_DISPLAYTYPE
}GUIEDIT_TIME_DISPLAY_TYPE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get time
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_GetTime (
    MMI_CTRL_ID_T   ctrl_id,    //in
    uint8           *hour_ptr,  //in/out:may PNULL
    uint8           *minute_ptr,//in/out:may PNULL
    uint8           *second_ptr //in/out:may PNULL
);

/*****************************************************************************/
//  Description : set time
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_SetTime (
    MMI_CTRL_ID_T   ctrl_id,
    uint8           hour,
    uint8           minute,
    uint8           second
);

/*****************************************************************************/
//  Description : set time style and display style
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_SetTimeStyle (
    MMI_CTRL_ID_T                  ctrl_id,
    GUIEDIT_SEPARATOR_E            *separator_ptr,     //may PNULL
    GUIEDIT_TIME_STYLE_E           *time_style_ptr,    //may PNULL
    GUIEDIT_TIME_DISPLAY_STYLE_E   *display_style_ptr, //may PNULL
    BOOLEAN                        is_update
);

/*****************************************************************************/
//  Description : set time editor title 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_SetTitle(
                                 MMI_CTRL_ID_T  ctrl_id,   
                                 MMI_STRING_T   *title_str_ptr
                                 );

/*****************************************************************************/
//  Description : free time editor title memory
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_FreeTitle(
                                 MMI_CTRL_ID_T  ctrl_id,   
                                 MMI_STRING_T   *title_str_ptr
                                 );

/*****************************************************************************/
//  Description : Set display type 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_SetTimeEditorDisplayType(
                                MMI_CTRL_ID_T                   ctrl_id,
                                GUIEDIT_TIME_DISPLAY_TYPE_E     display_type
                                );

/*****************************************************************************/
//  Description : Get display type 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_TIME_DISPLAY_TYPE_E CTRLTIMEEDIT_GeTimeEditorDisplayType(
                                MMI_CTRL_ID_T                   ctrl_id
                                );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
