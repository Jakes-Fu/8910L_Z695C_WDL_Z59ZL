/*****************************************************************************
** File Name:     wtlsstatesupport.h                                               *
** Author:        undiso cheng                                               *
** Date:          2009.02                                               *
** Copyright:     2007 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    wtls state support function                         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009-02    undiso cheng           Create	                                 *
******************************************************************************/

#ifndef _WTLS_STATE_SUPPORT_H
#define _WTLS_STATE_SUPPORT_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "cfl_list.h"
#include "wtls_string.h"
#include "wtls_machine.h"
#include "wtls_session.h"
#include "wtls_handshake.h"

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
#define WTLS_KEYSIZE_MAX 2048
#define WTLS_KEYSIZE_512 64
#define WTLS_KEYSIZE_768 96
#define WTLS_KEYSIZE_113 113
#define WTLS_KEYSIZE_131 131
#define WTLS_NOT_EXPORTABLE 0
#define WTLS_EXPORTABLE 1
#define WTLS_BLOCK 0
#define WTLS_STREAM 1
#define WTLS_HEADER_LENGTH 5
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    char *title_ptr;
    uint16 key_size_limit;
	uint16 padding;          /*add pad value here! */
} WTLS_KEYEX_TABLE_T;

typedef struct 
{
	char *title_ptr;
	BOOLEAN is_exportable;
	uint8 block_or_stream;
	uint8 key_material_length;
	uint8 expkey_material_length;
	uint8 effective_key_bits;
	uint8 iv_size;
	uint8 block_size;
	uint8 padding;           /*add pad value here! */
} WTLS_BULK_TABLE_T;

typedef struct
{
    char *title_ptr;
    uint8 key_size;
    uint8 mac_size; 
	uint16 padding;          /*add pad value here! */
} WTLS_HASH_TABLE_T;
/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :encrypt data
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note:
/*****************************************************************************/
PUBLIC WTLS_ERROR_E WTLSALGOPROC_DataEncrypt(                             //return operate success or fail
											 WTLS_MACHINE_T *machine_ptr, //[IN/OUT]the security instance
											 uint8 *buffer_ptr,            //[IN]buffer to encrypt
											 uint32 buffer_len            //[IN]buffer length
											);

/****************************************************************************/
// 	Description :  Calculate prf
//	Global resource dependence :
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *WTLSALGOPROC_CalculatePrf(                          //PRF
											   CFL_OCTSTR_T *secret_ptr, //[IN]point to secret
											   CFL_OCTSTR_T *label_ptr,  //[IN]label
											   CFL_OCTSTR_T *seed_ptr,   //[IN]seed
											   uint32 bytelen,           //[IN]length of the prf
											   uint8 mac_algo            //[IN]mac algo
											  );

/*****************************************************************************/
// 	Description :calculate server key block
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note:
/*****************************************************************************/
PUBLIC WTLS_ERROR_E WTLSALGOPROC_CalculateServerKeyBlock(                            //return operate success or fail
														 WTLS_MACHINE_T *machine_ptr //[IN/OUT]the security instance
														);

/*****************************************************************************/
// 	Description :calculate client key block
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note:
/*****************************************************************************/
PUBLIC WTLS_ERROR_E WTLSALGOPROC_CalculateClientKeyBlock(                             //operate success or fail
														 WTLS_MACHINE_T *machine_ptr  //[IN/OUT]the security instance
														);

/****************************************************************************/
// 	Description :	compute hash value, output length is mac_size.
//	Global resource dependence :s_hash_table
//  Author: undiso.cheng
//	Note: this algorithm is ok
/****************************************************************************/
PUBLIC CFL_OCTSTR_T* WTLSALGOPROC_HASH(                         //return hash
									   CFL_OCTSTR_T *input_ptr, //[IN]buffer to hash
									   uint8 mac_algo           //[IN]mac algo 
									  );

/****************************************************************************/
// 	Description : decrypt pdu list
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC BOOLEAN WTLSALGOPROC_DecryptPduList(
										   WTLS_MACHINE_T *machine_ptr,  //[IN/OUT]security instance
										   CFL_LIST_HANDLE pdu_list      //[IN]pdu
										  );

/****************************************************************************/
// 	Description : convert our buffer into a completed GenericBlockCipher,
//                encrypt(data + mac + padding) 
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T* WTLSALGOPROC_Encrypt(                              //encrypted string
										  WTLS_MACHINE_T *machine_ptr,  //[IN/OUT]security instance
										  CFL_OCTSTR_T *buf_ptr,        //[IN]buffer to decrypt
										  uint8 type                    //[IN]pdu type
										 );

/****************************************************************************/
// 	Description :Verify handshake server finish message
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T* WTLSALGOPROC_FinishedVerify(                             //verify result
												 WTLS_MACHINE_T *machine_ptr, //[IN]security instance
												 CFL_OCTSTR_T* input_ptr      //[IN]buffer to input
												);

/****************************************************************************/
// 	Description : decrypt
//	Global resource dependence : none
//  Author: undiso.cheng
//	Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T* WTLSALGOPROC_Decrypt(                              //decrypted string
										  WTLS_MACHINE_T *machine_ptr,  //[IN/OUT]security instance
										  CFL_OCTSTR_T *buffer_ptr,     //[IN]buffer to decrypt
										  uint8 recordtype              //[IN]pdu type
										  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif



