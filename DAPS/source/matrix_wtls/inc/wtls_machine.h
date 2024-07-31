
/*****************************************************************************
** File Name:     wtlsalgo.h                                               *
** Author:        undiso cheng                                               *
** Date:          2009.02                                               *
** Copyright:     2007 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    wtls algorithm interface                          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009-02    undiso cheng           Create	                                 *
******************************************************************************/
#ifndef _WTLS_MACHINE_H
#define _WTLS_MACHINE_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "wtls_string.h"
#include "cfl_list.h"
#include "wtls_session.h"
#include "wtls_interface.h"
#include "symmetric.h"
#include "../../matrix_ssl/src/crypto/keyformat/x509.h"
#include "pubkey.h"

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

#define WTLS_HASH_LENGTH 20
#define WTLS_HANDSHAKE_FULL 0 /*full handshake*/
#define WTLS_HANDSHAKE_RESUME 1/*resume handshake*/
#define WTLS_HANDSHAKE_SUCC  1/*handshake success*/
#define WTLS_HANDSHAKE_FAIL 0/*handshake fail*/
#define WTLS_FINISH_MESSAGE_LENGTH 12

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
typedef enum
{
    WTLS_EVENT_CREATE_REQ,
    WTLS_EVENT_EXCHANGE_RES,
    WTLS_EVENT_COMMIT_REQ,
    WTLS_EVENT_SENDDATA_REQ,
    WTLS_EVENT_RECVDATA_IND,
    WTLS_EVENT_ERROR_IND,
    WTLS_EVENT_WARNING_IND,
    WTLS_EVENT_NULL,
    WTLS_EVENT_MAX
}WTLS_EVENTTYPE_E;

typedef enum
{
    WTLS_STATE_NULL     = 0,
    WTLS_STATE_CREATING = 1,
    WTLS_STATE_EXCHANGE = 2,
    WTLS_STATE_COMMIT1  = 3,
    WTLS_STATE_COMMIT2  = 4,
    WTLS_STATE_CREATED  = 5,
    WTLS_STATE_OPENING  = 6,
    WTLS_STATE_OPEN     = 7
}WTLS_STATE_E;

typedef enum
{
    WTLS_KEYEXSUITE_NULL            = 0,
    WTLS_KEYEXSUITE_SHAREDSECRETE   = 1,
    WTLS_KEYEXSUITE_DH_ANON         = 2,
    WTLS_KEYEXSUITE_DH_ANON_512     = 3,
    WTLS_KEYEXSUITE_DH_ANON_768     = 4,
    WTLS_KEYEXSUITE_RSA_ANON        = 5,
    WTLS_KEYEXSUITE_RSA_ANON_512    = 6,
    WTLS_KEYEXSUITE_RSA_ANON_768    = 7,
    WTLS_KEYEXSUITE_RSA             = 8,
    WTLS_KEYEXSUITE_RSA_512         = 9,
    WTLS_KEYEXSUITE_RSA_768         = 10,
    WTLS_KEYEXSUITE_ECDH_ANON       = 11,
    WTLS_KEYEXSUITE_ECDH_ANON_113   = 12,
    WTLS_KEYEXSUITE_ECDH_ANON_131   = 13,
    WTLS_KEYEXSUITE_ECDH_ECDSA      = 14
}WTLS_KEYEXSUITE_E;
/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
//structure typedef , these structures are original in matrix
typedef psX509Cert_t WTLS_RSACERT_T; 
typedef psRsaKey_t  WTLS_RSAKEY_T;
typedef psCipherContext_t WTLS_CIPHERCONTEXT_T;

//the data taken by data receive event
typedef struct 
{  
    CFL_LIST_HANDLE  pdu_list;
}WTLS_DATARECV_EVENT_T;
//data taken by data send event
typedef struct    
{  
    CFL_OCTSTR_T *user_data_ptr;
} WTLS_DATASEND_EVENT_T;

//data taken by error event
typedef struct     
{  
    int32  alert_level; 
    int32  alert_desc;
} WTLS_ERROR_EVENT_T;

typedef struct 
{
    int32  alert_level; 
    int32  alert_desc;
}WTLS_WARNING_EVENT_T;

typedef struct 
{
    CFL_OCTSTR_T *encrypted_data_ptr;
}WTLS_COMMITREQ_EVENT_T;

typedef struct 
{
    int alert_level;
    int alert_desc;
}WTLS_EXCEPTIONREQ_EVENT_T;

typedef struct 
{
    int alert_level;
    int alert_desc;
}WTLS_TERMINALREQ_EVENT_T;

//event structure
typedef struct _tag_WTLS_EVENT_T
{
    WTLS_EVENTTYPE_E type;//event type
	uint8 padding1;
	uint16 padding2;
    struct   _tag_WTLS_EVENT_T  *next_ptr;//next event pointer
	union
	{
        WTLS_DATARECV_EVENT_T   data_received;
        WTLS_ERROR_EVENT_T      terminate_request; 
        WTLS_WARNING_EVENT_T    exception_request; 
        WTLS_DATASEND_EVENT_T   data_tosend; 
        WTLS_COMMITREQ_EVENT_T  commit_data;
	}u_event;//event union 
    
}WTLS_EVENT_T;

//parameters get from cipher suite
typedef struct 
{
    uint8  bulk_cipher_algo ;
    uint8  mac_algo ; 
    uint8  client_keyex_algo ;
    uint8  is_exportable ;
    uint8  pubkey_type;
    uint8  key_material_length ;  
    uint8  expkey_material_length ; 
    uint8  iv_size ;  
    uint8  mac_size ;  
	uint8  padding1;          /*add pad value here! */
	uint16 padding2;          /*add pad value here! */
}WTLS_CIPHERPRAM_T;
//event queue structure
typedef struct 
{
    WTLS_EVENT_T *eventqhead_ptr;  
    WTLS_EVENT_T *eventqtail_ptr;
}WTLS_EVENTQUEUE_T;
//state machine's status tag
typedef struct 
{
    BOOLEAN   isworking;  
    BOOLEAN   ishandshakesuccess;
    BOOLEAN   isresumehandshake;
    BOOLEAN   isserverkeyex;
    BOOLEAN   isclientenc;
    BOOLEAN   isserverenc; 
    BOOLEAN   iscertconfirmed;
	uint8     padding;           /*add pad value here! */
}WTLS_MACHINETAG_T;
//secret's detail info,client and server will has its own one
typedef struct 
{
    CFL_OCTSTR_T *writemacsecret_ptr;
    CFL_OCTSTR_T *writeenckey_ptr;
    CFL_OCTSTR_T *writeiv_ptr; 
}WTLS_SECRET_T;

typedef struct {
	CFL_OCTSTR_T *point_ptr;
} WTLS_ECPOINT_T;

typedef WTLS_ECPOINT_T WTLS_ECPUBKEY_T;
/* rsa key 's component*/
typedef struct  
{
	CFL_OCTSTR_T *rsa_exponent_ptr;
	CFL_OCTSTR_T *rsa_modulus_ptr;
} WTLS_RSAPUBKEY_T;


typedef struct {
	/* dh */
	WTLS_ECPUBKEY_T *ecdh_pubkey_ptr;
	/* ecdsa */
	WTLS_ECPUBKEY_T *ecdsa_pubkey_ptr;
	/* rsa */
//	WTLS_RSAPUBKEY_T 
    WTLS_RSAPUBKEY_T *rsa_pubkey_ptr;

}WTLS_PUBKEY_T;
//machine structure
typedef struct 
{ 
    void *app_handle ;                      //up layer's handle
    WTLS_EVENTQUEUE_T *event_queue_ptr;     //event queue
    WTLS_STATE_E state;                     //machine's state
    uint8 client_version;
    uint8 server_version;
	uint8 padding1;      
    uint16 client_seqnum ;                  //client data sequence number
    uint16 server_seqnum ;                  //server data's sequence number
    uint16 refresh_time;                    //refresh time
    uint16 padding2;
    uint32 seqnumber_mode;                  //explicit or implicit
    uint32 keyfresh_time;                   // key fresh frequency
    CFL_OCTSTR_T *client_random_ptr ;       //client random
    CFL_OCTSTR_T *server_random_ptr ;       //server random
    WTLS_SECRET_T *client_secrete_ptr;      //client secret
    WTLS_SECRET_T *server_secrete_ptr;      //server secret
    CFL_OCTSTR_T *client_pubkey_ptr;
    WTLS_PUBKEY_T *server_pubkey_ptr;       //write key
    CFL_OCTSTR_T *premaster_secret_ptr ;    //premaster secrete
    WTLS_MACHINETAG_T tag;                  //machine structure's assist tag
    WTLS_CIPHERPRAM_T param;                //machine's ciphersuit detail info
    CFL_LIST_HANDLE sessionlist_handle;     //one connection's detail info
    WTLS_SESSION_T  *sessioninfo_ptr ;      //this connection's session info
    CFL_OCTSTR_T    *last_packet_checksum_ptr;
    CFL_OCTSTR_T    *todecrypt_buffer_ptr;  //received from up layer to decrypt
    CFL_OCTSTR_T    *decrypted_buffer_ptr;  //decrypted data to return to up layer
    uint8            *toencrypt_buffer_ptr;  //received from up layer to encryt
    CFL_OCTSTR_T    *encrypted_buffer_ptr;  //encrypted data to send to server
    CFL_OCTSTR_T    *handshakedata_ptr;     //handshake data
    void *postmessage_cb ;                  //callback
    void *internalsend_cb;
    
} WTLS_MACHINE_T;
/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :		create a security instance
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC WTLS_MACHINE_T* WTLSMACHINE_Create(void);
/*****************************************************************************/
// 	Description :destroy a security instance. assume it is safe to do so.
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC WTLS_RESULT_E WTLSMACHINE_Destroy(
										 void *machine_ptr //[IN]security instance
										);
/*****************************************************************************/
// 	Description :set working flag
//	Global resource dependence :none
//  Param :machine_ptr: the security instance
//          flag: working flag
//  Return : none
//  Author: undiso.cheng
//	Note:
/*****************************************************************************/
PUBLIC void WTLSMACHINE_SetWorkingFlag(WTLS_MACHINE_T *machine_ptr, BOOLEAN flag);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif









