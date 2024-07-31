/*****************************************************************************
** File Name:      mmipicview_position.h                                     *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#ifndef _MMIPICVIEW_POSITION_H_
#define _MMIPICVIEW_POSITION_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

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
#define MMIPICVIEW_TITLE_GAP    3

//time drop down list
#define MMIPICVIEW_SLIDE_TIME_NUM           6

#if defined MAINLCD_SIZE_320X480
#define MMIPICPREVIEW_TITLE_HEIGHT             50
#define MMIPICPREVIEW_BOTTOM_MENU_HEIGHT       50
#define MMIPICPREVIEW_TITLE_MARGIN             ((GUILCD_IsLandscape(0))?18:14) 
#define MMIPICPREVIEW_LIST_ICON_SIZE           70
#define MMIPICPREVIEW_LIST_ICON_MARGIN         6
#define MMIPICPREVIEW_TITLE_FONT               SONG_FONT_28
#elif defined MAINLCD_SIZE_128X160
#define MMIPICPREVIEW_TITLE_HEIGHT             0
#define MMIPICPREVIEW_BOTTOM_MENU_HEIGHT       40
#define MMIPICPREVIEW_TITLE_MARGIN             ((GUILCD_IsLandscape(0))?16:8) 
#define MMIPICPREVIEW_LIST_ICON_SIZE           38
#define MMIPICPREVIEW_LIST_ICON_MARGIN         2
#define MMIPICPREVIEW_TIME_BG_V_TOP            20
#define MMIPICPREVIEW_TIME_BG__RIGHT           128
#define MMIPICPREVIEW_TIME_HEIGHT              16
#define MMIPICPREVIEW_TITLE_FONT               MMI_DEFAULT_BIG_FONT
#define MMIPICVIEW_DELIMETER_MARGIN          2
#define MMIPICVIEW_TOTAL_NUM_TOP               4
#define MMIPICVIEW_TOTAL_NUM_HEIGHT            12
#define MMIPICVIEW_TOTAL_NUM_V_LEFT              100
#define MMIPICVIEW_TOTAL_NUM_V_RIGHT             127
#define MMIPICVIEW_TOTAL_NUM_H_LEFT               130
#define MMIPICVIEW_TOTAL_NUM_H_RIGHT             159
#elif defined MAINLCD_SIZE_176X220
#define MMIPICPREVIEW_TITLE_HEIGHT             0
#define MMIPICPREVIEW_BOTTOM_MENU_HEIGHT       40
#define MMIPICPREVIEW_TITLE_MARGIN             ((GUILCD_IsLandscape(0))?16:8) 
#define MMIPICPREVIEW_LIST_ICON_SIZE           54
#define MMIPICPREVIEW_LIST_ICON_MARGIN         2
#define MMIPICPREVIEW_TIME_BG_V_TOP            24
#define MMIPICPREVIEW_TIME_BG__RIGHT           176
#define MMIPICPREVIEW_TIME_HEIGHT              18
#define MMIPICPREVIEW_TITLE_FONT               MMI_DEFAULT_BIG_FONT
#define MMIPICVIEW_DELIMETER_MARGIN          2
#define MMIPICVIEW_TOTAL_NUM_TOP               6
#define MMIPICVIEW_TOTAL_NUM_HEIGHT            14
#define MMIPICVIEW_TOTAL_NUM_V_LEFT              140
#define MMIPICVIEW_TOTAL_NUM_V_RIGHT             175
#define MMIPICVIEW_TOTAL_NUM_H_LEFT              184
#define MMIPICVIEW_TOTAL_NUM_H_RIGHT             219
#else
#define MMIPICPREVIEW_TITLE_HEIGHT             0//40 zhuyongjing change for gallary ui 20150707
#define MMIPICPREVIEW_BOTTOM_MENU_HEIGHT       40
#define MMIPICPREVIEW_TITLE_MARGIN            0// ((GUILCD_IsLandscape(0))?16:8) zhuyongjing change for gallary ui 20150707
#define MMIPICPREVIEW_LIST_ICON_SIZE           76//54 zhuyongjing change 20150815
#define MMIPICPREVIEW_LIST_ICON_MARGIN         6 ////4 zhuyongjing change 20150815
#define MMIPICPREVIEW_TITLE_FONT               MMI_DEFAULT_BIG_FONT
#define MMIPICPREVIEW_UPLOADING_LABEL_HEIGHT               22//zhuyongjing add for uploading label height
#define MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HEIGHT             15//zhuyongjing add for uploading progress bar height
#define MMIPICPREVIEW_UPLOADING_LABEL_HORIZION_MARGIN    6
#define MMIPICPREVIEW_UPLOADING_PRG_BOTTOM_MARGIN    6
#define MMIPICPREVIEW_UPLOADING_LABEL_BOTTOM_MARGIN    7
#define MMIPICPREVIEW_UPLOADING_PROGRESSBAR_HORIZION_MARGIN    9
#endif//MAINLCD_SIZE_320X480

#ifdef MAINLCD_SIZE_240X320
 //vertical
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_LEFT                          100
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_RIGHT                        220
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_TOP                            441
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_BOTTOM                     479

//horizontal
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_LEFT                          440
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_RIGHT                        479
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_TOP                            124
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_BOTTOM                     244

#elif defined MAINLCD_SIZE_240X400
 //vertical
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_LEFT                          60
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_RIGHT                        180
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_TOP                            361
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_BOTTOM                     399

//horizontal
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_LEFT                          361
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_RIGHT                        399
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_TOP                            81
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_BOTTOM                     201
#elif defined MAINLCD_SIZE_320X480
 //vertical
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_LEFT                          100
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_RIGHT                        220
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_TOP                            441
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_BOTTOM                     479

//horizontal
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_LEFT                          440
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_RIGHT                        479
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_TOP                            124
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_BOTTOM                     244

#else
 //vertical
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_LEFT                          100
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_RIGHT                        220
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_TOP                            441
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_BOTTOM                     479

//horizontal
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_LEFT                          440
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_RIGHT                        479
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_TOP                            124
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_BOTTOM                     244
#endif
//#endif //PIC_PREVIEW_U200_SUPPORT
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif //#ifdef PIC_VIEWER_SUPPORT