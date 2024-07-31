/*****************************************************************************
** File Name:      httpc_serialize.h				                                 *
**                 cheng.du   	                                             *        
******************************************************************************/

#ifndef _HTTPC_SERIALIZE_H_
#define _HTTPC_SERIALIZE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "httpc_api.h"

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
#ifndef HTTPC_MEM_DEBUG
    #define HTTPC_ALLOC(s) SCI_ALLOCA(s)
    #define HTTPC_FREE(p) SCI_FREE(p)
#else
    #define HTTPC_ALLOC(s) CFLMEM_Malloc(s, __FILE__,__LINE__)
    #define HTTPC_FREE(p) CFLMEM_Free(p)     

#endif

#define HTTPC_MEMSET     memset
#define HTTPC_MEMCPY     memcpy
#define HTTPC_MEMNCPY    CFLCHAR_MEMCPY
#define HTTPC_STRCPY     strcpy
#define HTTPC_STRLEN     CFL_StrLen
#define HTTPC_STRNCPY    CFLCHAR_StrNCpy
#define HTTPC_MEMCMP     memcmp
#define HTTPC_MEMMOVE    memmove
#define HTTPC_STRSTR     strstr
#define HTTPC_STRCAT     strcat
#define HTTPC_STRNCAT    strncat
#define HTTPC_STRCMP     strcmp
#define HTTPC_STRNCMP    strncmp
#define HTTPC_STRCHR     strchr
#define HTTPC_STRRCHR    strrchr
#define HTTPC_SSCANF     sscanf
#define HTTPC_SPRINTF    sprintf
#define HTTPC_SNPRINTF   CFL_Snprintf
#define HTTPC_STR_IS_NOT_EMPTY(x)   (x!=PNULL && x[0]!=0)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : serialize HTTPC_INIT_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpcSerializeHttpInitReqSigT(
                    const HTTPC_INIT_REQ_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );
/*****************************************************************************/
//  Description : serialize HTTPC_GET_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpcSerializeHttpGetReqSigT(
                    const HTTPC_GET_REQ_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );
/*****************************************************************************/
//  Description : serialize HTTPC_POST_REQ_SIG_T
//  Global resource dependence : 
//  Author: qin.he
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpcSerializeHttpPostReqSigT(
                    const HTTPC_POST_REQ_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );
/*****************************************************************************/
//  Description : serialize HTTPC_DELETE_REQ_SIG_T
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpcSerializeHttpDeleteReqSigT(
                    const HTTPC_DELETE_REQ_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );     
/*****************************************************************************/
//  Description : serialize HTTPC_AUTH_RSP_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpcSerializeHttpAuthRspSigT(
                    const HTTPC_AUTH_RSP_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );
/*****************************************************************************/
//  Description : serialize HTTPC_CLOSE_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpcSerializeHttpCloseReqSigT(
                    const HTTPC_CLOSE_REQ_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );
#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : serialize HTTPC_SSL_CERT_UNTRUST_RSP_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpcSerializeHttpSslCerRspSigT(
                    const HTTPC_SSL_CERT_UNTRUST_RSP_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                                    **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                                  *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_HTTPC_SERIALIZE_H_
