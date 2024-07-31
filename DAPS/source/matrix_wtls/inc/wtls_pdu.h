/*****************************************************************************
** File Name:     wtlspdu.h                                               *
** Author:        undiso cheng                                               *
** Date:          2009.02                                               *
** Copyright:     2007 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    ssl  pdu interface                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009-02    undiso cheng           Create	                                 *
******************************************************************************/

#ifndef _WTLS_PDU_H
#define _WTLS_PDU_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "wtls_string.h"
#include "wtls_machine.h"
#include "wtls_alert.h"


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
#define WTLS_HELLORANDOM_LEN                12 // length of clienthello random length
#define WTLS_RECORD_TYPE_MASK_BIT0          0x80
#define WTLS_RECORD_TYPE_MASK_BIT1          0x40
#define WTLS_RECORD_TYPE_MASK_BIT2          0x20
#define WTLS_RECORD_TYPE_MASK_BIT3          0x10
#define WTLS_RECORD_TYPE_MASK_HIGHBIT       0x0f
#define WTLS_SHA1_HASH_LEN                  20
#define WTLS_MD5_HASH_LEN                   16
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
typedef enum
{
	WTLS_CERTFORMAT_WTLS = 1,
	WTLS_CERTFORMAT_X509,
	WTLS_CERTFORMAT_X968
} WTLS_CERTFORMAT_E;
typedef enum
{
    WTLS_BULKCIPHER_NULL                = 0,
    WTLS_BULKCIPHER_RC5_CBC_40          = 1,
    WTLS_BULKCIPHER_RC5_CBC_56          = 2,
    WTLS_BULKCIPHER_RC5_CBC             = 3,
    WTLS_BULKCIPHER_DES_CBC_40          = 4,
    WTLS_BULKCIPHER_DES_CBC             = 5,
    WTLS_BULKCIPHER_TRIPLE_DES_CBC_EDE  = 6,
    WTLS_BULKCIPHER_IDEA_CBC_40         = 7,
    WTLS_BULKCIPHER_IDEA_CBC_56         = 8,
    WTLS_BULKCIPHER_IDEA_CBC            = 9
}WTLS_BULKCIPHER_E;

typedef enum 
{
    WTLS_KEYMAC_SHA_0 = 0,
    WTLS_KEYMAC_SHA_40,
    WTLS_KEYMAC_SHA_80,
    WTLS_KEYMAC_SHA_NOLIMIT,
    WTLS_KEYMAC_SHA_XOR_40,
    WTLS_KEYMAC_MD5_40,
    WTLS_KEYMAC_MD5_80,
    WTLS_KEYMAC_MD5_NOLIMIT
}WTLS_KEYMAC_E;

//pdu's type
typedef enum 
{
	WTLS_CHANGE_CIPHER_PDU = 1,
	WTLS_ALERT_PDU,
	WTLS_HANDSHAKE_PDU,        
	WTLS_APPLICATION_PDU       
} WTLS_PDUTYPE_E;

typedef enum  
{
	WTLS_SIGALGO_ANONYMOUSE,
	WTLS_SIGALGO_ECDSA_SHA,
	WTLS_SIGALGO_RSA_SHA
} WTLS_SIGALGO_E;


/*compression method*/
typedef enum 
{
	NULL_COMP = 0
} WTLS_COMPRESSMETHOD_E;

typedef enum  
{
	WTLS_PUBKEYTYPE_RSA_KEY = 2,
	WTLS_PUBKEYTYPE_DH_KEY  = 3,
	WTLS_PUBKEYTYPE_EC_KEY  = 4
} WTLS_PUBKEYTYPE_E;

typedef enum  
{
	WTLS_CERTIDENTIFIERTYPE_NULL      = 0,
	WTLS_CERTIDENTIFIERTYPE_TEXT,
	WTLS_CERTIDENTIFIERTYPE_BIN,
	WTLS_CERTIDENTIFIERTYPE_HASH_SHA  = 254,
	WTLS_CERTIDENTIFIERTYPE_X509_NAME = 255
} WTLS_CERTIDENTIFIERTYPE_E;

typedef enum  
{
	WTLS_ECFIELD_PRIME_P,
	WTLS_ECFIELD_CHARACTER_TWO
}WTLS_ECFIELD_E;

typedef enum  
{
	WTLS_ECBASIS_ONB = 1,
	WTLS_ECBASIS_TRINOMIAL,
	WTLS_ECBASIS_PENTANOMIAL,
	WTLS_ECBASIS_POLYNOMIAL
} WTLS_ECBASISTYPE_E;
typedef enum  
{
	WTLS_HANDSHAKETYPE_HELLOREQ         = 0,
	WTLS_HANDSHAKETYPE_CLIENTHELLO      = 1,
	WTLS_HANDSHAKETYPE_SERVERHELLO      = 2,
	WTLS_HANDSHAKETYPE_CERTIFICATE      = 11,
	WTLS_HANDSHAKETYPE_SERVERKEYEX      = 12,
	WTLS_HANDSHAKETYPE_CERTREQ          = 13,
	WTLS_HANDSHAKETYPE_SERVERHELLODONE  = 14,
	WTLS_HANDSHAKETYPE_CERTVERIFY       = 15,
	WTLS_HANDSHAKETYPE_CLIENTKEYEX      = 16,
	WTLS_HANDSHAKETYPE_FINISHED         = 20
} WTLS_HANDSHAKETYPE_E;
/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
//random data structure
typedef struct 
{
    uint32 gmt_unix_time;
    CFL_OCTSTR_T *random_bytes_ptr;
} WTLS_RANDOM_T;  

typedef struct {
	uint8 bulk_cipher_algo;
	uint8 mac_algo;
	uint16 padding;          /*add pad value here! */
} WTLS_CIPHERSUITE_ID_T;

typedef struct{
	/* case anonymous */
	/* nothing */
	/* case ecdsa_sha and rsa_sha */
	CFL_LIST_HANDLE sha_hash;
} WTLS_SIGNATURE_T;

typedef struct  {
	CFL_OCTSTR_T *a_ptr;
	CFL_OCTSTR_T *b_ptr;
	CFL_OCTSTR_T *seed_ptr;
} WTLS_ECCURV_T;

typedef struct {
	uint8 dh_e;
	uint8 padding1;           /*add pad value here! */
	uint16 padding2;          /*add pad value here! */
	CFL_OCTSTR_T *dh_p_ptr;
	CFL_OCTSTR_T *dh_g_ptr;
} WTLS_DHPARAM_T;

typedef struct {
	uint8 field;
	uint8 basis;
	/* case ec_characteristic_two */
	uint16 m;
	/* case ec_basis_onb : nothing*/
	/* case ec_trinomial */
	uint16 k;
	/* case ec_pentanomial */
	uint16 k1;
	uint16 k2;
	uint16 k3;
	/* case ec_prime_p */
	CFL_OCTSTR_T *prime_p_ptr;
	/* case ec_basis_polynomial */
	CFL_OCTSTR_T *irreducible_ptr; 
	WTLS_ECCURV_T *curve_ptr;
	WTLS_ECPOINT_T *base_ptr;
	CFL_OCTSTR_T *order_ptr;
	CFL_OCTSTR_T *cofactor_ptr;
} WTLS_ECPARAM_T;


typedef struct{
	uint8 length;
	uint8 padding1;           /*add pad value here! */
	uint16 padding2;          /*add pad value here! */
	/* rsa: empty */
	/* diffie-hellman */
	WTLS_DHPARAM_T *dhparams_ptr;
	/* eliptic curve */
	WTLS_ECPARAM_T *ecparams_ptr;
} WTLS_KEYPARAMSET_T;

typedef struct{
	uint8 param_index;
	uint8 padding1;           /*add pad value here! */
	uint16 padding2;          /*add pad value here! */
	WTLS_KEYPARAMSET_T *param_set_ptr; //when parameter_index == 255
} WTLS_KEYPARAMIDENTITY_T;

typedef struct{
	uint8 id_type;
	uint8 padding;           /*add pad value here! */
	/* text */
	uint16 charset;
	CFL_OCTSTR_T *name_ptr;
	/* binary */
	CFL_OCTSTR_T *identifier_ptr;
	/* key_hash_sha */
	CFL_OCTSTR_T *key_hash_ptr;
	/* x509 */
	CFL_OCTSTR_T *distinguished_name_ptr;
} WTLS_CERTIDENTIFIER_T;

typedef struct{
	uint8 key_exchange_suite;
	uint8 padding1;           /*add pad value here! */
	uint16 padding2;          /*add pad value here! */
	WTLS_KEYPARAMIDENTITY_T *param_specif_ptr;
	WTLS_CERTIDENTIFIER_T *identifier_ptr;
} WTLS_KEYEXDATA_T;

typedef struct {
	CFL_OCTSTR_T *dh_y_ptr;
} WTLS_DHPUBKEY_T;

typedef struct  
{
	CFL_OCTSTR_T *encrypted_secret_ptr;
} WTLS_RSAENCRYPTEDSEC_T;

typedef struct {
	uint8 certificate_version;
	uint8 signature_algo;
	uint8 pubkey_type;
	uint8 padding;           /*add pad value here! */
	int32 valid_not_before;
	int32 valid_not_after;	
	WTLS_CERTIDENTIFIER_T *issuer_ptr;
	WTLS_CERTIDENTIFIER_T *subject_ptr;
	WTLS_KEYPARAMIDENTITY_T *param_spec_ptr;
	WTLS_PUBKEY_T *pubkey_ptr;
} WTLS_TOBESIGNEDCERT_T;	

typedef struct 
{
	WTLS_TOBESIGNEDCERT_T *to_be_signed_cert_ptr;
	WTLS_SIGNATURE_T *signature_ptr;
} WTLS_WCERT_T;

/* ClientHello */
typedef struct 
{
	uint8 client_version;
	uint8 snmode;
	uint8 krefresh;
	uint8 padding;           /*add pad value here! */
	WTLS_RANDOM_T *random_ptr;
	CFL_OCTSTR_T *sessionid_ptr;
	CFL_LIST_HANDLE client_keyex_suite;
	CFL_LIST_HANDLE trusted_key_ids;
	CFL_LIST_HANDLE ciphersuites;
	CFL_LIST_HANDLE comp_methods;
} WTLS_CLIENTHELLO_T;

/* ServerHello */
typedef struct 
{
	uint8 server_version;
	uint8 snmode;
	uint8 krefresh_time;
 	uint8 choosed_keyex_suite;   
	uint8 comp_method;
	uint8 padding1;           /*add pad value here! */
	uint16 padding2;          /*add pad value here! */
	WTLS_CIPHERSUITE_ID_T ciphersuite;
	WTLS_RANDOM_T *random_ptr;
	CFL_OCTSTR_T *sessionid_ptr;
} WTLS_SERVERHELLO_T;

/* certificate */
typedef struct 
{
    uint8 certificateformat;
	uint8 padding1;           /*add pad value here! */
	uint16 padding2;          /*add pad value here! */
    WTLS_WCERT_T *wtls_cert_ptr;
    CFL_OCTSTR_T *x509_certificate_ptr;
    CFL_OCTSTR_T *x968_certificate_ptr;
} WTLS_CERTIFICATE_T;

typedef struct
{
	WTLS_KEYPARAMIDENTITY_T *param_spec_ptr;
	/* case rsa_anon */
	//WTLS_RSAPUBKEY_T 
    WTLS_RSAPUBKEY_T *rsa_params_ptr;
	/* case dh_anon */
	WTLS_DHPUBKEY_T *dh_params_ptr;
	/* case ecdh_anon */
	WTLS_ECPUBKEY_T *ecdh_params_ptr;
} WTLS_SERVERKEYEX_T;

typedef struct 
{
	/* case rsa and rsa_anon*/
	WTLS_RSAENCRYPTEDSEC_T *rsa_params_ptr;
	/* case dh_anon */
	WTLS_DHPUBKEY_T *dh_anon_params_ptr;
	/* case ecdh_anon and ecdh_ecdsa*/
	WTLS_ECPUBKEY_T *ecdh_params_ptr;
} WTLS_CLIENTKEYEX_T;

typedef struct
{
	CFL_OCTSTR_T *verifydata_ptr;
} WTLS_FINISHED_T;

typedef struct  
{
	CFL_LIST_HANDLE trusted_authorities; // List_T of KeyExchangeIds
}WTLS_CERTREQ_T;

typedef struct  
{
	WTLS_SIGNATURE_T *signature_ptr;
} WTLS_CERTVERIFY_T;

typedef struct{
	/* nothing here */
	int32 temp;
}WTLS_HELLOREQUEST_T;


/* handshake */
typedef struct 
{
	uint8 msg_type;
	uint8 padding;           /*add pad value here! */
	uint16 length;
	/* case hello_request */
	/* case client_hello */
	WTLS_CLIENTHELLO_T *client_hello_ptr;
	/* case server_hello */
	WTLS_SERVERHELLO_T *server_hello_ptr;
	/* case certificate */
	WTLS_CERTIFICATE_T *certificate_ptr;
	/* case server_key_exchange */
	WTLS_SERVERKEYEX_T *server_key_exchange_ptr;
	/* case certificate_request */
	WTLS_CERTREQ_T *certificate_request_ptr;
	/* case certificate_verify */
	WTLS_CERTVERIFY_T *cert_verify_ptr;
	/* case client_key_exchange */
	WTLS_CLIENTKEYEX_T *client_key_exchange_ptr;
	/* case finished */
	WTLS_FINISHED_T *finished_ptr;
} WTLS_HANDSHAKE_T;

typedef struct
{
	uint8 change;
	uint8 padding1;           /*add pad value here! */
	uint16 padding2;          /*add pad value here! */
} WTLS_CHANGECIPHER_T;

/* application pdu */
typedef struct 
{
    CFL_OCTSTR_T *data_ptr;
} WTLS_APPDATA_T;

//pdu structure
typedef struct  
{
	uint8 reserved_bit;
	uint8 cipher;
    uint8 type;   //pdu type
	uint8 padding;
    uint16 seqnum; //sequence number
    uint16 len;  //pdu lenght
    union
	{
		WTLS_CHANGECIPHER_T  changecipher;
		WTLS_ALERT_T         alert;
		WTLS_HANDSHAKE_T     handshake;
		WTLS_APPDATA_T       application;
	} u_subtype;//detail pdu 
} WTLS_PDU_T;

typedef struct 
{
	uint8 type;
	uint8 reserved_bit;
	uint8 cipher;
	uint8 padding;
	uint16 seqnum;
	uint16 len;
	CFL_OCTSTR_T *data_ptr;
} WTLS_PAYLOAD_T;
/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
// 	Description : create a empty pdu
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	WTLS_PDUTYPE_E
//          1 -- ChangeCipher_PDU
//          2 -- Alert_PDU
//          3 -- Handshake_PDU
//          4 -- application_pdu
/****************************************************************************/
PUBLIC WTLS_PDU_T* WTLSPDU_CreatePdu(
									 WTLS_PDUTYPE_E type  //[IN]pdu type 
									);

/****************************************************************************/
// 	Description : 	unpack pdu
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_PDU_T* WTLSPDU_UnpackPDU(                               //pdu unpacked
									 WTLS_PAYLOAD_T *payload_ptr,   //[IN]unpack payload to get pdu
									 WTLS_MACHINE_T *machine_ptr    //[IN/OUT]security machine
									);

/****************************************************************************/
// 	Description : 	pack pdu
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_PAYLOAD_T* WTLSPDU_PackPdu(
									   WTLS_PDU_T *pdu_ptr,         //[IN]pdu pointer
									   WTLS_MACHINE_T *machine_ptr  //[IN/OUT]security machine
									  );

/****************************************************************************/
// 	Description : 	destroy pdu
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	user should set pdu to NULL after call this function
/****************************************************************************/
PUBLIC void WTLSPDU_DestroyPdu(
							   WTLS_PDU_T *pdu_ptr,          //[IN]pdu pointer
							   WTLS_MACHINE_T *machine_ptr   //[IN]security machine
							  );

/****************************************************************************/
// 	Description : 	pack a payload
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC CFL_OCTSTR_T* WTLSPDU_PayloadPack(
										 WTLS_MACHINE_T *machine_ptr, //[IN]security machine
										 WTLS_PAYLOAD_T *payload_ptr  //[IN]payload to pack
										);

/****************************************************************************/
// 	Description : 	destroy payload
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: user will set payload_ptr to NULL after use this function
/****************************************************************************/
PUBLIC void WTLSPDU_PayloadDestroy(                            //the payload pointer
								   WTLS_PAYLOAD_T *payload_ptr //[IN/OUT]the payload need to destroy
								  );

/****************************************************************************/
// 	Description : 	add a pdu to pdu list
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN WTLSPDU_AddPdu(
							  WTLS_MACHINE_T *machine_ptr, //[IN/OUT]security machine
							  WTLS_PDU_T *pdu_ptr          //[IN]pdu to be added to list
							 );

/****************************************************************************/
// 	Description : This function will unpack data to a list of pdu and create 
//                an data received event to let machine process
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: user will set payload_ptr to NULL after use this function
/****************************************************************************/
PUBLIC WTLS_EVENT_T* WTLSPDU_DatagramUnpack(                             //the event pointer
											WTLS_MACHINE_T *machine_ptr, //[IN/OUT]the security instance
											CFL_OCTSTR_T *data_ptr       //[IN]data contain payload info
										   );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif

