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
#ifndef _WTLS_ALGORITHM_H
#define _WTLS_ALGORITHM_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "wtls_machine.h"
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
// 	Description : md5 hash
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN WTLSALGO_MD5Hash(
								const uint8 *src_ptr, //[IN]the data to be hashed
								uint32 src_length,    //[IN]data length
								uint8 *dest_ptr       //[OUT]the hash result
								);

/*****************************************************************************/
// 	Description : HMAC md5 
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note:definition see rfc2104
/*****************************************************************************/
PUBLIC BOOLEAN WTLSALGO_HMACMD5Hash(
									uint8 *secretkey_ptr,//[IN]secret key 
                                    uint32 key_length,  //[IN]length of the key in bytes 
                                    uint8 *src_ptr,     //[IN]data 
                                    uint32 src_length,  //[IN]length of data in bytes 
                                    uint8 *dest_ptr,    //[OUT]output buffer, at least "dest_length" bytes 
                                    uint32 dest_length  //[IN]must not greater than 16 
								   );
/*****************************************************************************/
// 	Description : HMAC_SHA1 
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note: definition see rfc2104
/*****************************************************************************/
PUBLIC BOOLEAN WTLSALGO_HMACSHA1Hash(
									 uint8 *secretkey_ptr,  //[IN]secret key 
                                     uint32 key_length,    //[IN]length of the key in bytes 
                                     const uint8 *src_ptr, //[IN]data 
                                     uint32 src_length,    //[IN]length of data in bytes 
                                     uint8 *dest_ptr,      //[OUT]output buffer, at least "dest_length" bytes 
                                     uint32 dest_length    //[IN]must not greater than 20  
									 );
/*****************************************************************************/
// 	Description : sha1 hash
//	Global resource dependence :none
//  Author: undiso.cheng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN WTLSALGO_SHA1Hash(
								 const uint8 *src_ptr, //[IN]the data to be hashed
								 uint32 src_length,    //[IN]data length
								 uint8 *dest_ptr       //[OUT]the hash result
								 );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif



