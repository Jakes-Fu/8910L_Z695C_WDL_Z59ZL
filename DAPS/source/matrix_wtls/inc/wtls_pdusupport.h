/*****************************************************************************
** File Name:     wtlspdusupport.h                                               *
** Author:        undiso cheng                                               *
** Date:          2009.02                                               *
** Copyright:     2007 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    wtls  pdu support function                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009-02    undiso cheng           Create	                                 *
******************************************************************************/

#ifndef _WTLS_PDU_SUPPORT_H
#define _WTLS_PDU_SUPPORT_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "wtls_string.h"
#include "cfl_list.h"
#include "wtls_machine.h"
#include "wtls_pdu.h"
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
#ifdef WIN32
#define WTLS_RAND_MAX 0x7fff
#else
#define WTLS_RAND_MAX 0x7fffffff
#endif
/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
// 	Description : pack random
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC int32 WTLSPDUS_PackRandom(                           //position in data
								 CFL_OCTSTR_T *data_ptr,    //[IN/OUT]data to add packed info
								 WTLS_RANDOM_T *random_ptr  //[IN]random to be packed
								);

/****************************************************************************/
// 	Description : 	pack the list of key exchange cipher suite
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC int32 WTLSPDUS_PackKeyexSuiteList(                                   //position in data
										 WTLS_MACHINE_T *machine_ptr,       //[IN/OUT]security machine
										 CFL_OCTSTR_T *data_ptr,            //[IN/OUT]data to add packed info
										 CFL_LIST_HANDLE keyex_suite_handle //[IN]suite list to be packed
										 );

/****************************************************************************/
// 	Description : 	pack the list of cipher suite
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC int32 WTLSPDUS_PackBulkCipherSuiteList(                                    //position in data
											  CFL_OCTSTR_T *data_ptr,             //[IN/OUT]data to add packed info
											  CFL_LIST_HANDLE ciphersuite_handle  //[IN]cipher suite list to be packed
											 );

/****************************************************************************/
// 	Description :  pack the list of compression method
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC int32 WTLSPDUS_PackCompressionMethodList(                                       //position in data
												CFL_OCTSTR_T *data_ptr,                //[IN/OUT]data to add packed info
												CFL_LIST_HANDLE compmethodlist_handle  //[IN] list
											   );

/****************************************************************************/
// 	Description : 	destroy random
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC BOOLEAN WTLSPDUS_DestroyRandom(
									  WTLS_RANDOM_T *random_ptr  //[OUT]random to free
									 );

/****************************************************************************/
// 	Description : 	destroy the list of key exchange cipher suite
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC BOOLEAN WTLSPDUS_DestroyKeyexsuiteList(
											  CFL_LIST_HANDLE keyex_suite_handle  //[IN/OUT]cipher suite list to free
											 );

/****************************************************************************/
// 	Description : 	destroy the list of compression method
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC BOOLEAN WTLSPDUS_DestroyCompressionMethodList(
													 CFL_LIST_HANDLE methodlist_handle //[IN/OUT]list to destroy
													);

/****************************************************************************/
// 	Description : 	destroy the list of compression method
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC BOOLEAN WTLSPDUS_DestroyBulkSuiteList(
											 CFL_LIST_HANDLE bulk_cipher_list_handle  //[OUT]list
											);

/****************************************************************************/
// 	Description : 	destroy the content of certificate
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC void WTLSPDUS_DestroyCertificate(
										WTLS_MACHINE_T *machine_ptr,  //[IN/OUT]the machine instance
										WTLS_WCERT_T *cert            //[IN/OUT]wtls format cert     
									   );

/****************************************************************************/
// 	Description : 	destroy public key
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC BOOLEAN WTLSPDUS_DestroyRsaPubkey(
										 WTLS_RSAPUBKEY_T *key_ptr  //[IN/OUT]RSA key to free
										);

/****************************************************************************/
// 	Description : 	destroy param spec
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC void WTLSPDUS_DestroyParamSpec(
									  WTLS_PUBKEYTYPE_E key_type,                    //[IN]key type
									  WTLS_KEYPARAMIDENTITY_T *spec_ptr  //[IN/OUT]param spec to destroy
									 );

/****************************************************************************/
// 	Description : 	destroy DH public key
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC void WTLSPDUS_DestroyDHPubKey(
									 WTLS_DHPUBKEY_T *key_ptr //[IN/OUT]DH key
									);

/****************************************************************************/
// 	Description : 	destroy EC public key
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC void WTLSPDUS_DestroyECPubKey(
									 WTLS_ECPUBKEY_T *key_ptr //[IN/OUT]EC key
									);

/****************************************************************************/
// 	Description : 	destroy rsa encrypted secret
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC BOOLEAN WTLSPDUS_DestroyRsaEncryptedSecret(
												  CFL_OCTSTR_T *secret_ptr  //[IN/OUT]rsa secret to free
												 );

/****************************************************************************/
// 	Description : 	unpack wtls cetificate from buf
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC WTLS_WCERT_T* WTLSPDUS_UnPackWCert(
										  WTLS_MACHINE_T *machine_ptr, //security machine
										  CFL_OCTSTR_T *buf_ptr,       //[OUT]buffer contains the cert information
										  uint32 *charpos_ptr           //[IN/OUT]the place where to extract information
										 );

/****************************************************************************/
// 	Description : 	unpack param spec
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC WTLS_KEYPARAMIDENTITY_T* WTLSPDUS_UnpackParamSpec(
														 uint8 keyexsuite,        //[IN]the keyexchange suite negotiated in hello message
														 CFL_OCTSTR_T *buf_ptr,   //[IN]buffer contains the cert information
														 uint32 *charpos_ptr       //[IN/OUT]the place where to extract information
														);

/****************************************************************************/
// 	Description : 	unpack EC publickey
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  
/****************************************************************************/
PUBLIC WTLS_ECPUBKEY_T* WTLSPDUS_UnpackECPubKey(
												CFL_OCTSTR_T *data_ptr, //[IN]data buffer contains ec key 
												uint32 *charpos_ptr       //[IN/OUT]the place where to extract information
											   );

/****************************************************************************/
// 	Description : 	unpack RSA publickey
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC WTLS_RSAPUBKEY_T* WTLSPDUS_UnpackRsaPubKey(
												  CFL_OCTSTR_T *data_ptr,  //[IN]data buffer 
												  uint32 *charpos_ptr        //[IN/OUT]the place where to extract information
												 );

/****************************************************************************/
// 	Description :get data packet length
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC int32 WTLSPDUS_GetPacketLen(                             //packet length
								   WTLS_MACHINE_T *machine_ptr, //[IN]security instance
								   CFL_OCTSTR_T *ostr_ptr       //[IN]data received
								  );

/****************************************************************************/
// 	Description : 	unpack diffie hellman public key
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note: 
/****************************************************************************/
PUBLIC WTLS_DHPUBKEY_T * WTLSPDUS_UnpackDHPubKey(
												 CFL_OCTSTR_T *data_ptr, //[IN]data buffer contains ec key
												 uint32 *charpos_ptr      //[IN/OUT]the place where to extract information
												);

/*****************************************************************************/
// 	Description :Whether handshake is success
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN WTLSPDUS_IsHandShakeSuccess(
										   WTLS_MACHINE_T *machine_ptr  //[IN]security instance
										  );

/****************************************************************************/
// 	Description : 	create client bulkcipher suites. 
//	Global resource dependence : s_wtls_ciphersuite_id
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC CFL_LIST_HANDLE WTLSPDUS_CreateBulkCipherSuites(void);

/****************************************************************************/
// 	Description : 	create client key exchange cipher suites.
//	Global resource dependence : s_keyex_suite
//  Author: undiso.cheng
//	Note: this function maybe a little different if you add more keyex algo.
/****************************************************************************/
PUBLIC CFL_LIST_HANDLE WTLSPDUS_CreateClientKeyexSuite(void);

/****************************************************************************/
// 	Description : create compress methods
//	Global resource dependence : s_compressionmethods
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC CFL_LIST_HANDLE WTLSPDUS_CreateCompressMethods(void);

/****************************************************************************/
// 	Description : 	create random.
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:the random string it has packeted from time and random number 
/****************************************************************************/
PUBLIC WTLS_RANDOM_T* WTLSPDUS_CreateRandom(                             //the random it generated
											WTLS_MACHINE_T *machine_ptr  //[IN/OUT]security instance
										   );

/****************************************************************************/
// 	Description : create session id.
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T* WTLSPDUS_CreateSessionId(                             //the session id string
											  WTLS_MACHINE_T *machine_ptr  //[IN]security instance
											 );

/****************************************************************************/
// 	Description : 	get encrypted secret
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *WTLSPDUS_GetEncryptedSecret(                             //secret string
												 WTLS_MACHINE_T *machine_ptr  //[IN/OUT]security instance
												);
/****************************************************************************/
// 	Description : 	destroy public key
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:  pointer should set to NULL after call this function
/****************************************************************************/
PUBLIC void WTLSPDUS_DestroyPublicKey(
									  WTLS_PUBKEY_T *key_ptr  //[IN/OUT]public key to destroy
								     );
/****************************************************************************/
// 	Description : 	unpack int24
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC int16 WTLSPDUS_UnpackInt16(                        //the unpacked value
								  CFL_OCTSTR_T *data_ptr, //[IN]data to unpack
								  uint32 *charpos_ptr     //[IN/OUT]position in data
								 );

/****************************************************************************/
// 	Description : 	unpack string contains length(uint16)+data
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC  CFL_OCTSTR_T *WTLSPDUS_UnpackOctstr16(
											   CFL_OCTSTR_T *data_ptr, //[OUT]data to unpack
											   uint32 *charpos_ptr       //[IN/OUT]position in data
											  );

/****************************************************************************/
// 	Description : 	unpack serverhello info to pdu
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:	
/****************************************************************************/
PUBLIC WTLS_ERROR_E WTLSPDUS_UnpackServerHello(                             //pdu unpacked
											   WTLS_PDU_T * pdu_ptr,        //[IN/OUT]handshake pdu 
											   CFL_OCTSTR_T *buf_ptr,       //[IN]the buffer contains original info
											   WTLS_MACHINE_T *machine_ptr, //[OUT]security machine
											   uint32 *charpos_ptr           //[IN/OUT]place to get info from buf
											  );

/****************************************************************************/
// 	Description : 	pack dh negotiated key 
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC BOOLEAN WTLSPDUS_PackDhPubkey(
									 CFL_OCTSTR_T *data_ptr,   //[OUT]data will contain packed key
									 uint32 *charpos_ptr,      //[OUT]position in data
									 WTLS_DHPUBKEY_T *key_ptr  //[IN]dh publick key
									);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif
#endif