/*****************************************************************************
** File Name:      http_event.h                                              *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP event              *
*****************************************************************************/

#ifndef _http_event_H_
#define _http_event_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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
#include "http_def.h"
#include "http_util.h"
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :  
//  Global resource dependence :
//  Author: li.li
//  Note: 
/*****************************************************************************/

/*****************************************************************************/
//  Description : init event queue
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void  HttpEventQueueInit(HTTP_EVENT_QUEUE_T* queue_ptr);

/*****************************************************************************/
//  Description : insert event to event queue
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpEventQueueInsert(HTTP_EVENT_QUEUE_T* queue_ptr, HTTP_EVENT_TYPE_E event);

/*****************************************************************************/
//  Description : fecth event from event queue
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_EVENT_TYPE_E HttpEventQueueFetch(HTTP_EVENT_QUEUE_T* queue_ptr);

/*****************************************************************************/
//  Description : check if the event queue is empty
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpEventQueueIsEmpty(HTTP_EVENT_QUEUE_T* queue_ptr);

/*****************************************************************************/
//  Description : clear the event queue
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpEventQueueClear(HTTP_EVENT_QUEUE_T* queue_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif