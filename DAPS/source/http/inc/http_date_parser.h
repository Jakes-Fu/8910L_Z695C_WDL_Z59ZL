/*****************************************************************************
** File Name:      http_date_parser.h                                        *
** Author:          li.li                                                    *
** Date:           02/03/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP data parse         *
*****************************************************************************/
#ifndef HTTP_DATEPARSER_H
#define HTTP_DATEPARSER_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "http_def.h"
#include "cfl_time.h"

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

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct _TIME_ZONE 
{
  uint8*  name_ptr;
  int32   offset; /* +/- in minutes */
}TIME_ZONE_T;

typedef enum  
{
  DATE_MDAY = 0,
  DATE_YEAR,
  DATE_TIME
}DATE_E;

/*****************************************************************************/
//  Description : parse the date string
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC time32 HttpDateParse(uint8* date_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif//HTTP_DATEPARSER_H
