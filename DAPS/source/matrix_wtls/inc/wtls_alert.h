/*****************************************************************************
** File Name:     wtlsalert.h                                                *
** Author:        undiso cheng                                               *
** Date:          2009.02                                                    *
** Copyright:     2007 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:   WTLS                          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009-02    undiso cheng           Create	                                 *
******************************************************************************/

#ifndef _WTLS_ALERT_H
#define _WTLS_ALERT_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "wtls_string.h"
#include "wtls_machine.h"
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

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
/* the available alert level */
typedef enum 
{
    WTLSALEVEL_NULL     = 0,
	WTLSALEVEL_WARNING  = 1,  //warning level rfc4346
	WTLSALEVEL_FATAL    = 2,  //fatal level rfc4346
	WTLSALEVEL_NONE     = 255 //preserved
} WTLS_ALERTLEVEL_E;

/* the available alert describe */
typedef enum 
{
	WTLS_CONNECTION_CLOSE_NOTIFY    = 0,
    WTLS_SESSION_CLOSE_NOTIFY       = 1,
    WTLS_NO_CONNECTION              = 5,
    WTLS_UNEXPECTED_MESSAGE         = 10,
    WTLS_BAD_RECORD_MAC             = 20,
    WTLS_DECRYPTION_FAILURE         = 21,
    WTLS_RECORD_OVERFLOW            = 22,
    WTLS_DECOMPRESSION_FAILURE      = 30,
    WTLS_HANDSHAKE_FAILURE          = 40,
    WTLS_BAD_CERTIFICATE            = 42,
    WTLS_UNSUPPORTED_CERT           = 43,
    WTLS_CERT_REVOKED               = 44,
    WTLS_CERT_EXPIRED               = 45,
    WTLS_CERT_UNKNOWN               = 46,
    WTLS_ILLEGAL_PARAM              = 47,
    WTLS_UNKOWN_CA                  = 48,
    WTLS_ACCESS_DENIED              = 49,
    WTLS_DECODE_ERROR               = 50,
    WTLS_DECRYPT_ERROR              = 51,
    WTLS_UNKOWN_KEY_ID              = 52,
    WTLS_DISABLED_KEY_ID            = 53,
    WTLS_KEYEXCHANGE_DISABLE        = 54,
    WTLS_SESSION_NOT_READY          = 55,
    WTLS_UNKNOWN_PARAMINDEX         = 56,
    WTLS_DUPLICATE_FINISHMESSAGE    = 57,
    WTLS_EXPORT_RESTRICTION         = 60,
    WTLS_PROTOCOL_VERSION           = 70,
    WTLS_INSUF_SECURITY             = 71,
    WTLS_INTERNAL_ERROR             = 80,
    WTLS_USER_CANCLED               = 90,
    WTLS_NO_RENEGOTION              = 100,
    WTLS_NO_DES                     = 255   //preserved
} WTLS_ALERTDESC_E; 


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef struct
{
	uint8 level;
	uint8 desc;
	uint16 padding;
	CFL_OCTSTR_T *chksum_ptr;
}WTLS_ALERT_T;

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
// 	Description : send alert message to server
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_ERROR_E WTLSALERT_SendAlert(
										WTLS_MACHINE_T *machine_ptr,   //[IN/OUT]security instance
										WTLS_ALERTLEVEL_E alert_level, //[IN]alert level
										WTLS_ALERTDESC_E alert_desc    //[IN]alert description
										);

/****************************************************************************/
// 	Description : handle alert event
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_ERROR_E WTLSALERT_HandleAlertEvent(
											   WTLS_MACHINE_T *machine_ptr,   //[IN/OUT]security instance
											   WTLS_ALERTLEVEL_E alert_level, //[IN]alert level
											   WTLS_ALERTDESC_E alert_desc    //[IN]alert description
											   );

											   /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif