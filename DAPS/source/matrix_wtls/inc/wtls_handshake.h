/*****************************************************************************
** File Name:     wtlshandshake.h                                        *
** Author:        undiso cheng                                               *
** Date:          2009.02                                               *
** Copyright:     2007 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:   wtls  handshake interface                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009-02    undiso cheng           Create	                                 *
******************************************************************************/
#ifndef _WTLS_HANDSHAKE_H
#define _WTLS_HANDSHAKE_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "cfl_list.h"
#include "wtls_string.h"
#include "wtls_machine.h"
#include "wtls_interface.h"
#include "wtls_session.h"
#include "wtls_event.h"
#include "wtls_internal.h"
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
#define WTLS_KEYFRESH_TIME 5
#define WTLS_SEQ_EXPLICITY 2

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
//wtls status judge
typedef enum 
{
    WTLSCOND_TRUE                    = 1,
    WTLSCOND_IS_HELLO_REQUEST        = 2,
    WTLSCOND_IS_WARNING              = 3,
    WTLSCOND_IS_FATAL                = 4,
    WTLSCOND_IS_SERVERHELLO          = 5,
    WTLSCOND_IS_SERVERHELLODONE      = 6,
    WTLSCOND_IS_SERVERHELLO_FINISHED = 7,
    WTLSCOND_IS_FINISHED             = 8,
    WTLSCOND_IS_USER_DATA            = 9,
    WTLSCOND_IS_CHANGECIPHER         = 10,
    WTLSCOND_MAX                     = 255
}WTLS_CONDITION_E;

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :	when get a T_Create_ClientFinish event, send the Cipher 
//                  and Finished pdu, and the state of machine is changed 
//                  from CLIENT_EXCHANGE to CLIENT_FINISHED.
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC void WTLSHAND_SendCCAndFinshedMessage(
											 WTLS_MACHINE_T *machine_ptr, //[IN]security instance
											 WTLS_EVENT_T *event_ptr      //[IN]event
											);
/*****************************************************************************/
// 	Description :when get WTLS_CLIENT_HELLO event, send a ClientHello pud,
//               and the state of machine is changed from NULL_STATE to CLIENT_HELLO
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC void WTLSHAND_SendClientHello(
									 WTLS_MACHINE_T *wtls_ptr, //[IN]security instance
									 WTLS_EVENT_T *event_ptr   //[IN]send client hello request event
									);
/*****************************************************************************/
// 	Description :	when get a pdu include serverhello serverkeyex serverhellodone, 
//                  handle the pdu,
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC void WTLSHAND_HandleServerHello(
									   WTLS_MACHINE_T *machine_ptr, //[IN]security instance
									   WTLS_EVENT_T *event_ptr      //[IN]server hello event
									  );
/*****************************************************************************/
// 	Description :	when get a T_Create_ClientExchange event, send the 
//                  ClientExchange pdu,and the state of machine is changed 
//                  from SERVERHELLODONE to CLIENTEXCHANGE.
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC void WTLSHAND_HandleClientExchange(
										  WTLS_MACHINE_T *machine_ptr, //[IN]security instance
										  WTLS_EVENT_T *event_ptr      //[IN]client exchange event
										 );
/*****************************************************************************/
// 	Description :handle server hello+changecipher_serverhellofinish in resume handshake
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC void WTLSHAND_HandleSeverHelloFinish(
											WTLS_MACHINE_T *machine_ptr, //[IN]security instance
											WTLS_EVENT_T* event_ptr      //[IN]event to process
										   );
/*****************************************************************************/
// 	Description :handle received ciphered application data
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC void WTLSHAND_HandleRecvData(
									WTLS_MACHINE_T *machine_ptr,  //[IN]security instance
									WTLS_EVENT_T *event_ptr       //[IN]event to process
								   );
/*****************************************************************************/
// 	Description :encrypt user data to send
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC void WTLSHAND_EncyptUserDataToSend(
										  WTLS_MACHINE_T *machine_ptr,   //[IN]security instance
										  WTLS_EVENT_T *event_ptr        //[IN]event to process
										 );
/************************************************************************* 
// Function: inform upper layer handshake succ
//  Global resource dependence :none
//  Author  :  undiso.cheng
//  Note:
/ ************************************************************************/
PUBLIC void WTLSHAND_NotifyHandShakeSuccess(
											WTLS_MACHINE_T *machine_ptr, //[IN]WTLS global machine
											WTLS_EVENT_T *event_ptr      //[IN]WTLS event
										   );
/************************************************************************* 
//  Description: when get a Alert pdu, handle the pdu,
//  Global resource dependence :none
//  Author  :  undiso.cheng
//  Note:
/ ************************************************************************/
PUBLIC void WTLSHAND_HandleAlert(
								 WTLS_MACHINE_T *machine_ptr, //[IN]wtls global machine
								 WTLS_EVENT_T *event_ptr      //[IN]wtls event
								);
/************************************************************************* 
//  Description: send warning alert
//  Global resource dependence :none
//  Author  :  undiso cheng
//  Note:
/ ************************************************************************/
PUBLIC void WTLSHAND_HandleSendWarningAlert(
											WTLS_MACHINE_T *machine_ptr,   //[IN]WTLS global machine
											WTLS_EVENT_T *event_ptr        //[IN]WTLS event
										   );
/************************************************************************* 
//  Description: send fatal alert
//	Global resource dependence :none
//  Author  :  undiso cheng
//  Note:
/ ************************************************************************/
PUBLIC void WTLSHAND_HandleSendFatalAlert(
										  WTLS_MACHINE_T *machine_ptr,  //[IN]WTLS global machine
										  WTLS_EVENT_T *event_ptr       //[IN]WTLS event
										 );
/*****************************************************************************/
// 	Description :full handshake  server change cipher and finish
//  Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC  void WTLSHAND_HandleServerCCFinish(
										   WTLS_MACHINE_T *machine_ptr, //[IN]security instance
										   WTLS_EVENT_T *event_ptr      //[IN]event
										  );
/*****************************************************************************/
// 	Description : full handshake last step send client finish
//  Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC void WTLSHAND_SendClientFinish(
									  WTLS_MACHINE_T *machine_ptr, //[IN]security instance
									  WTLS_EVENT_T *event_ptr      //[IN]event 
									 );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif
#endif

