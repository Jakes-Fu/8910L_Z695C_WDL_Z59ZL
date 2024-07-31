/*
 * Copyright 2004 Aplix Corporation. All rights reserved.
 */
/***************************************************************************
 * FILE: kjava_sys_ext.h
 *
 * DESCRIPTION:
 *      The module is one of KJava interface modules. It is for native file
 *      system handling.
 *
 * UPDATED DATE:
 *
 * REVISION:
 *
 ***************************************************************************/


#ifndef KJAVA_SYS_EXT_HEADER
#define KJAVA_SYS_EXT_HEADER

/**
 * @file kjava_sys_ext.h
 * OTA KSI
 */


#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup ams_installation_deletion_ksi
 *  @{
 */

/*
 *  Interfaces listed in the group are used for:
 *     - Verify one X509 chain certificate based on external or native root certificate
 *       authentication .
 *     - Verify the RSA-SHA1 based64 encoding digital signature using one
 *       external X509 certificate.
 *     .
 */

/** 
 * @addtogroup X509_Chain_Cert_Verify Authenticate X509 Chain Certificate
 * @{
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/** \brief The identifier for manufacturer protection domain. */ 
#define KJAVA_DOMAIN_MANUFACTURER  (1)
/** \brief The identifier for operator protection domain. */ 
#define KJAVA_DOMAIN_OPERATOR      (2)
/** \brief The identifier for third party protection domain. */ 
#define KJAVA_DOMAIN_THIRD_PARTY   (3) 
/** \brief The identifier for unspecified protection domain. */ 
#define KJAVA_DOMAIN_UNSPECIFIED   (0)

/** \brief Certificate structure */
typedef struct _KJavaCertChain
{
    /** \brief X509 certificate in DER format. */
	unsigned char *cert;
	/** \brief Size of <i>cert</i>. */
	int            certLen;
} KJavaCertChain;

/** \brief Root certificate structure */
typedef struct _KJavaCaList
{
    /** \brief Mapping protection domain for the Root Certificate. 
     *
     *         Indicate the certificate attribute field
     *         CKA_JAVA_MIDP_SECURITY_DOMAIN defined in PKCS#11. The possible
     *         values are:
     *         - #KJAVA_DOMAIN_MANUFACTURER
     *         - #KJAVA_DOMAIN_OPERATOR
     *         - #KJAVA_DOMAIN_THIRD_PARTY
     *         - #KJAVA_DOMAIN_UNSPECIFIED
     *         .
     */
	int             type;
	/** \brief X509 certificate in DER format. */
	unsigned char*  cert;
	/** \brief Size of <i>cert</i>. */
	int             certLen;
} KJavaCaList;
/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/** \brief The function is for verify X509 chain certificate 
 *
 *         The function is used to do the verify the X509 chain certificate
 *         using external or internal root certificate list. To support external
 *         root certificate, user needs to specific the root certificate list
 *         and their mapping protection domain by PIK (pre-install kit) in the
 *         AJSC compile time. The external root certificate list will be passed
 *         to this function by the <i>pCa</i> and <i>iNumOfCa</i>.
 *
 *         If none root certificate is assigned by PIK, the <i>pCa</i> will be
 *         NULL. In such case, platform should use their own native root 
 *         certificate database to verify the data and assign the protection 
 *         domain in <i>iDomain</i> if succeed.
 *
 *         This function is only called by KVE thread/task during midlet suite 
 *         installation and is only called when the AJSC trusted midlet option 
 *         is enabled.
 *
 * @param pCa         The external root certificate list predefined by PIK tool.
 * @param iNumOfCa    Number of root certificate inside <i>pCa</i>.
 * @param pChain      One X509 certificate chain that needed to be verified.
 * @param iNumOfCertInChain Number of certificate inside <i>pChain</i> X509 certificate
 *                    chain.
 * @param iDomain     A pointer for a buffer that used to retrieve the protection
 *                    domain via the function. When return value is 0, this variable should
 *                    be assigned one of the follows:
 *                   - #KJAVA_DOMAIN_MANUFACTURER
 *                   - #KJAVA_DOMAIN_OPERATOR
 *                   - #KJAVA_DOMAIN_THIRD_PARTY
 *                   - #KJAVA_DOMAIN_UNSPECIFIED
 *                   .
 *
 * @return 0 for succeed and 1 for failed.           
 */
int kjava_ext_verifyChainCert( KJavaCaList     *pCa,
							   int              iNumOfCa,
							   KJavaCertChain  *pChain,
							   int              iNumOfCertInChain,
							   int             *iDomain );

/** @} */

/** 
 * @addtogroup RSA_SHA1_Signature_Verify Verify RSA SHA1 Digital Signature
 * @{
 */

/** \brief The function is called for RSA-SHA1 signature verification.
 *
 * @param pCert    The X509 certificate in DER format.
 * @param iCertLen The size of <i>pCert</i>.
 * @param pJar     The Jar binary data that needs to be verified.
 * @param iJarLen  The size of <i>pJar</i>.
 * @param pSign    The BASE64 encoding signature for <i>pJar</i>.
 * @param iSignLen The size of <i>pSign</i>
 *
 * @return 0 for succeed and 1 for failed.   
 */
int kjava_ext_verifySignature( 
                         unsigned char  *pCert,
						 int             iCertLen,
						 unsigned char  *pJar,
						 int             iJarLen,
						 unsigned char  *pSign,
						 int             iSignLen );

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_EXT_HEADER */
