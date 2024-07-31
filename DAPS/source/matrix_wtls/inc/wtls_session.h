/*****************************************************************************
** File Name:      wtlssession.h                                                *
** Author:         undiso.cheng                                                *
** Date:           2009.2                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    session functions                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.2     undiso.cheng         Create                                 *
******************************************************************************/

#ifndef _WTLS_SESSION_H
#define _WTLS_SESSION_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "wtls_string.h"
#include "cfl_list.h"
#include "wtls_common.h"

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
#define WTLS_SESSIONTABLE_NUM 3
#define WTLS_MAX_SESSION_LEN 10  /* max session length */
#define WTLS_HOST_LEN        64  /* the length of host name */
#define WTLS_SESSION_ID_LEN  32  /* the length of session id */
#define WTLS_AUTHNAME_LEN    100 /* the length of authority name*/
#define WTLS_CIPHER_SPEC_LEN 2   /* the length of cipher spec */
#define WTLS_MASTER_SECRET_LEN 20/* the length of master secrete*/
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
/* session struct define */
typedef struct 
{
    CFL_OCTSTR_T    *sessionid_ptr;
    uint32          hostip;
    uint16          port;
    uint8           keyexcipher_id;
    uint8           bulkcipher_id;
    uint8           maccipher_id;
	uint8           padding1;
	uint16          padding2;
	CFL_OCTSTR_T    *mastersecret_ptr;
} WTLS_SESSION_T;

typedef struct 
{
    WTLS_SESSION_T session;
    BOOLEAN is_valid;
	uint8 padding1;
	uint16 padding2;
}WTLS_SESSION_TABLE_T;
/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
// 	Description :    get session
//  Param        :  session_table_ptr: session info table
//                  host_ip: host ip address
//                  port : port
//	Global resource dependence : none
//  Return :the qualified session in table
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC WTLS_SESSION_T* WTLSSESSION_Get(CFL_LIST_HANDLE list_ptr, uint32 host_ip, uint16 port);

/****************************************************************************/
// 	Description : create a session
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC WTLS_SESSION_T* WTLSSESSION_Create(void);

/****************************************************************************/
// 	Description : append a session to sessio list
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC void WTLSSESSION_Append(
							   CFL_LIST_HANDLE list_ptr,    //[IN/OUT]session list
							   WTLS_SESSION_T *session_ptr  //[IN]session to append
							  );

/****************************************************************************/
// 	Description :free session
//	Global resource dependence : none
//  Author:undiso.cheng 
//	Note:	
/****************************************************************************/
PUBLIC void WTLSSESSION_Free(
							 WTLS_SESSION_T *session_ptr  //[IN/OUT]session to free
							);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif /* SESSION_H */