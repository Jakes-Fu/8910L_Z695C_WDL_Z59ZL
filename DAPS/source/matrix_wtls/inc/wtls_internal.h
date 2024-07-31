/*****************************************************************************
** File Name:     wtls_internal.h                                               *
** Author:        undiso cheng                                               *
** Date:          2009.02                                               *
** Copyright:     2007 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    ssl                          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009-02    undiso cheng           Create	                                 *
******************************************************************************/
#ifndef _WTLS_API_H
#define _WTLS_API_H

//#define WTLS_HALF_HANDSHAKE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "cfl_list.h"
#include "wtls_string.h"
#include "wtls_session.h"
#include "wtls_interface.h"

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
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
typedef enum 
{
    WTLS_OK                 = 1,
    WTLS_ERROR_INVALIDINPUT = 2,
    WTLS_ERROR_MALLOC_FAIL  = 3,
    WTLS_ERROR_FILE_FAIL    = 4,
    WTLS_ERROR_INTERN_FAIL  = 5,
    WTLS_ERROR_MAX          = 255
}WTLS_ERROR_E;
/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
//handle list and id number counter
typedef struct 
{
    CFL_LIST_HANDLE wtls_hsm_list;
    uint32 accumulated_id;
}WTLS_HANDLE_LIST_T;

typedef struct 
{
    WTLS_HANDLE handler;
    uint32 id;
}WTLS_HANDLE_ID_T;
/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
// 	Description : load session list from global variable
//	Global resource dependence : s_sessionlist_ptr
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC void WTLS_LoadSession(
							 CFL_LIST_HANDLE list_ptr //[IN]session list
							);
/****************************************************************************/
// 	Description : save session list to global variable
//	Global resource dependence : s_sessioninfo_ptr
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC void WTLS_SessionSave(
							 WTLS_SESSION_T *session_ptr  //[IN]session info
							);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif

















