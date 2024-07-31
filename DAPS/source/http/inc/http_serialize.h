/*****************************************************************************
** File Name:      http_serialize.h				                             *
** Author:                                                                   *
** Date:           02/19/2009                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    Serialize signals defined in http_api.h                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/19/2009     bruce.chi         Create									 *	
******************************************************************************/

#ifndef _HTTP_SERIALIZE_H_
#define _HTTP_SERIALIZE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "http_api.h"

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
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : serialize HTTP_INIT_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpInitReqSigT(
                    const HTTP_INIT_REQ_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_GET_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpGetReqSigT(
                    const HTTP_GET_REQ_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_POST_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpPostReqSigT(
                    const HTTP_POST_REQ_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_HEAD_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpHeadReqSigT(
                    const HTTP_HEAD_REQ_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_CANCEL_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCancelReqSigT(
                    const HTTP_CANCEL_REQ_SIG_T  *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_CLOSE_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCloseReqSigT(
                    const HTTP_CLOSE_REQ_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_AUTH_IND_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpAuthIndSigT(
                    const HTTP_AUTH_IND_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_AUTH_RSP_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpAuthRspSigT(
                    const HTTP_AUTH_RSP_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_INIT_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpInitCnfSigT(
                    const HTTP_INIT_CNF_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_GET_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpGetCnfSigT(
                    const HTTP_GET_CNF_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_POST_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpPostCnfSigT(
                    const HTTP_POST_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_PATCH_CNF_SIG_T
//  Global resource dependence : 
//  Author: mingzhe.jin
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpPatchCnfSigT(
                    const HTTP_PATCH_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_HEAD_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpHeadCnfSigT(
                    const HTTP_HEAD_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_HEADER_IND_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpHeaderIndSigT(
                    const HTTP_HEADER_IND_SIG_T  *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_HEADER_RSP_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpHeaderRspSigT(
                    const HTTP_HEADER_RSP_SIG_T  *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_DATA_IND_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpDataIndSigT(
                    const HTTP_DATA_IND_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_ERROR_IND_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpErrorIndSigT(
                    const HTTP_ERROR_IND_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_CLOSE_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCloseCnfSigT(
                    const HTTP_CLOSE_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_NET_PROG_IND_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpNetProgIndSigT(
                    const HTTP_NET_PROG_IND_SIG_T  *sig_ptr,               //[IN] input signal pointer
                    void                           **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                         *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_REDIRECT_IND_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpRedirectIndSigT(
                    const HTTP_REDIRECT_IND_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                            **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                          *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_REQUEST_ID_IND_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpRequestIdIndSigT(
                    const HTTP_REQUEST_ID_IND_SIG_T *sig_ptr,               //[IN] input signal pointer
                    void                            **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                          *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_CANCEL_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCancelCnfSigT(
                    const HTTP_CANCEL_CNF_SIG_T  *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_COOKIE_CLEAR_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCookieClearReqSigT(
                    const HTTP_COOKIE_CLEAR_REQ_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                                **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                              *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_COOKIE_CLEAR_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCookieClearCnfSigT(
                    const HTTP_COOKIE_CLEAR_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                                **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                              *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_CACHE_DELETE_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCacheDeleteReqSigT(
                    const HTTP_CACHE_DELETE_REQ_SIG_T  *sig_ptr,               //[IN] input signal pointer
                    void                               **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                             *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_CACHE_DELETE_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCacheDeleteCnfSigT(
                    const HTTP_CACHE_DELETE_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                                **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                              *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );
/*****************************************************************************/
//  Description : serialize HTTP_COOKIE_SET_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCookieSetReqSigT(
                                                 const HTTP_COOKIE_SET_REQ_SIG_T  *sig_ptr,               //[IN] input signal pointer
                                                 void                             **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                                                 uint32                           *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                                           );

/*****************************************************************************/
//  Description : serialize HTTP_COOKIE_SET_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpCookieSetCnfSigT(
                                                 const HTTP_COOKIE_SET_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                                                 void                              **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                                                 uint32                            *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                                             );
/*****************************************************************************/
//  Description : serialize HTTP_OPTIONS_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpOptionsReqSigT(
                    const HTTP_OPTIONS_REQ_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_PATCH_REQ_SIG_T
//  Global resource dependence : 
//  Author: mingzhe.jin
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpPatchReqSigT(
                    const HTTP_PATCH_REQ_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );                    
                       
/*****************************************************************************/
//  Description : serialize HTTP_DELETE_REQ_SIG_T
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpDeleteReqSigT(
                    const HTTP_DELETE_REQ_SIG_T    *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );                    
                                    
/*****************************************************************************/
//  Description : serialize HTTP_OPTIONS_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpOptionsCnfSigT(
                    const HTTP_OPTIONS_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_DELETE_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpDeleteCnfSigT(
                    const HTTP_DELETE_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                        **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                      *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : serialize HTTP_SSL_CERT_UNTRUST_IND_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpSslCerIndSigT(
                    const HTTP_SSL_CERT_UNTRUST_IND_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                                    **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                                  *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_SSL_CERT_UNTRUST_RSP_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpSslCerRspSigT(
                    const HTTP_SSL_CERT_UNTRUST_RSP_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                                    **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                                  *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_SSL_GET_CERT_REQ_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpSslCerReqSigT(
                    const HTTP_SSL_GET_CERT_REQ_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                                **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                              *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );

/*****************************************************************************/
//  Description : serialize HTTP_SSL_GET_CERT_CNF_SIG_T
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN HttpSerializeHttpSslGetCertCnfSigT(
                    const HTTP_SSL_GET_CERT_CNF_SIG_T   *sig_ptr,               //[IN] input signal pointer
                    void                                **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                              *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );



#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_HTTP_UTIL_H_
