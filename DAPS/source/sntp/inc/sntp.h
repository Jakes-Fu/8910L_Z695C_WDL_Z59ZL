/****************************************************************************
** File Name:      sntp.h                                                  *
** Author:         dewu jiang                                              *
** Date:           02/18/2011                                              *
** Copyright:                                                              *
** Description:    sntp                                                    *
****************************************************************************/
#ifdef SNTP_SUPPORT

#ifndef _SNTP_H_
#define _SNTP_H_

#include "sntp_api.h"
#include "sci_types.h"
#include "os_api.h"
#include "cfl_timer.h"
#include "socket_api.h"
#include "app_tcp_if.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                                                    *
 **--------------------------------------------------------------------------*/

 /*---------------------------------------------------------------------------*/
/*                          Enum                                                                                       */
/*---------------------------------------------------------------------------*/
typedef enum 
{
    SNTP_STATUS_INIT = 0x01,
    SNTP_STATUS_SUCCESS,
    SNTP_STATUS_FAILED,
}SNTP_STATUS_E; 

/*---------------------------------------------------------------------------*/
/*                          VARIABLE                                                                                 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          STRUCT                                                                                   */
/*---------------------------------------------------------------------------*/
typedef struct _SNTP_APP_NODE_INFO_STR
{
    uint32 fd_sock;
    uint32 tzone;
    uint32 net_time;
    uint32 ip_addr;
    uint32 app_sender;
    CFLTMR_HANDLE_T timer_id;
    uint16 send_count;
    SNTP_STATUS_E status_id;
}SNTP_APP_NODE_INFO_STR_T;

/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                                                  */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description: Get SNTP  task's id
// Global resource dependence : 
// Author: 
// Return:
// Note: 
/*****************************************************************************/
PUBLIC BLOCK_ID SNTP_GetHandleTaskId( void );

PUBLIC int32 get_offset(int32 gm_time, int16 zone_n);

#endif

#endif  //SNTP_SUPPORT