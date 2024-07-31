/*****************************************************************************
** File Name:      xxx_mmi_font.h                                            *
** Author:                                                                   *
** Date:           05/2006                                                   *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMI_FONT_H__
#define _MMI_FONT_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Font Resource 
///////////////////////////////////////////////////////////////////////////////

// Font Type
typedef enum _GUI_FONT_T
{
#ifndef _MMI_FONT_H__
#define _MMI_FONT_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Font Resource 
///////////////////////////////////////////////////////////////////////////////

// Font Type
typedef enum _GUI_FONT_T
{
	GUI_MAX_FONT_NUM
} GUI_FONT_T;

typedef struct _GUI_FONT_INFO_T
{
	GUI_FONT_T   type;                //font type
	uint32       ngb_db_size;
	uint8      * gb_database_ptr;     //font GB DB
	uint32       nascii_db_size;
	uint8      * pascii_ptr;          //font ASCII DB
} GUI_FONT_INFO_T, * P_GUI_FONT_INFO_T;

typedef struct _GUI_FONT_DB_T
{
	uint32       db_size;
	uint8      * pdb;                 //font DB
} GUI_FONT_DB_T, * P_GUI_FONT_DB_T;

typedef struct _GUI_FONT_INFO_EX_T
{
	GUI_FONT_T   type;                //font type
	uint8      * pdbtt;               //DBs,point to GUI_FONT_DB_T
} GUI_FONT_INFO_EX_T, * P_GUI_FONT_INFO_EX_T;


#ifdef __cplusplus
}
#endif

#endif // _MMI_FONT_DEF_H__