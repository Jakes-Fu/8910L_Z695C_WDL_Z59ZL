/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

/**
 *  @file ajms_cfg.h
 */

/*
 *  
 *  Interfaces listed in the group are used for AJMS configuration, 
 *  including these functionalities:
 *       - Set HTTP implementation by profile identifier.
 *       - Set HTTP user agent.
 *       - Set HTTP proxy.
 *       - Get AJMS configuration
 *       .
 */

#ifndef AJMS_CFG_HEADER
#define AJMS_CFG_HEADER

#ifdef __cplusplus
extern "C"{
#endif

#include "aj_type.h"

/** \ingroup network_aci */
/** 
 * @addtogroup HTTP_Config_ACI HTTP Configuration ACI
 * @{
 */

/** \brief The type of the DISABLE Http profile. */
#define AJSC_HTTP_PROFILE_DISABLED  0
/** \brief The type of the DEFAULT Http profile. */
#define AJSC_HTTP_PROFILE_DEFAULT   1
/** \brief The type of the EXTLIB Http profile. */
#define AJSC_HTTP_PROFILE_EXTLIB    2
/** \brief The type of the SERIAL Http profile. */
#define AJSC_HTTP_PROFILE_SERIAL    3
/** \brief The type of the proxy descriptor is BASIC. */
#define AJMS_PROXY_DESCRIPTOR_TYPE_BASIC      0
/** \brief type of the proxy descriptor is ADVANCED. */
#define AJMS_PROXY_DESCRIPTOR_TYPE_ADVANCED   1
/** @} */

/** 
 * @addtogroup AJMS_Config_ACI AJMS Configuration ACI
 * @{
 */
/**
 *  \brief An ID for getting the configuration supported by AJMS <br>
 *  Pass this ID as a parameter when calling #ajms_cfg_getProperty.
 */
#define AJMS_SUPPORTED_CONFIGURATION        (0)

/**
 *  \brief An ID for getting the profile supported by AJMS <br>
 *  Pass this ID as a parameter when calling #ajms_cfg_getProperty.
 */
#define AJMS_SUPPORTED_PROFILE              (1)

/**
 * \brief An ID for query if the trusted midlet is supported or not.
 *  Pass this ID as a parameter when calling #ajms_cfg_getProperty.
 */
#define AJMS_SUPPORT_TRUSTED_MIDLET         (10)
/**
 * \brief An ID for query if the category is supported or not.
 *  Pass this ID as a parameter when calling #ajms_cfg_getProperty.
 */
#define AJMS_SUPPORT_CATEGORY               (11)

/**
 * \brief An ID for query the AJMS file layout version. The file
 *  layout version only be changed when the file layout has been
 *  changed for new feature has been added.
 *  Pass this ID as a parameter when calling #ajms_cfg_getProperty.
 */
#define AJMS_LAYOUT_VERSION                 (13)
/** @} */

/** 
 * @addtogroup HTTP_Config_ACI HTTP Configuration ACI
 * @{
 */

/**
 * \brief Queries the proxy infromation for a specified host.
 *
 * Based on the remote address and port indicated by <i>remoteAddr</i> and
 * <i>remotePort</i>, store the information of an appropriate proxy server
 * in the buffers indicated by <i>proxyAddr</i> and <i>proxyPort</i>.
 *
 * @param remoteAddr
 *        A pointer to the string holding the remote host address.
 * @param remotePort
 *        The destination port of the remote host.
 * @param proxyAddr
 *        A pointer to the buffer for putting the proxy server address.
 * @param proxyAddrLength
 *        The size of the buffer indicated by <i>proxyAddr</i>.
 * @param proxyPort
 *        A pointer to the buffer for putting the proxy server port.
 * @return
 *        If the native system finds a proxy server setting for the host,
 *        return #AJ_SUCCESS, otherwise return #AJ_FAILURE.
 */
typedef AJ_RESULT (* AJMS_DynamicProxyHandler) (
                        AJ_U8       *remoteAddr,
                        const AJ_S32 remotePort,
                        AJ_U8       *proxyAddr,
                        const AJ_S32 proxyAddrLength,
                        AJ_S32      *proxyPort);

/**
 * \brief The structure type for specifying the proxy information
 * for HTTP connection controlled by Java application.
 */
typedef struct _AJMS_ProxyDescriptorTag
{
    /**
     * \brief The type of proxy descriptor.
     *
     * There are 2 types of proxy descriptors:
     * - #AJMS_PROXY_DESCRIPTOR_TYPE_BASIC
     * - #AJMS_PROXY_DESCRIPTOR_TYPE_ADVANCED
     * .
     *
     * If the proxy descriptor is #AJMS_PROXY_DESCRIPTOR_TYPE_BASIC,
     * the HTTP connection controlled by Java
     * application will use fixed proxy server address and port.
     * In this case only <i>serverAddress</i>, <i>serverPort</i> fields
     * should be filled.
     *
     * If the proxy descriptor is #AJMS_PROXY_DESCRIPTOR_TYPE_ADVANCED,
     * the HTTP connection will use the
     * <i>dynamicProxyHandler</i> callback function to query the
     * preferred proxy server based on the given remote server info of current
     * HTTP connection.
     * In this case only <i>dynamicProxyHandler</i> should be filled.
     */
    AJ_S32                      type;

    /**
     * \brief A pointer to the proxy server address, a null-terminated string.
     */
    AJ_U8*                 serverAddress;

    /**
     * \brief The proxy server port.
     */
    AJ_S32                      serverPort;

    /**
     * \brief The callback function to query proxy server dynamically.
     */
    AJMS_DynamicProxyHandler    dynamicProxyHandler;
}
AJMS_ProxyDescriptor;


/**
 * \brief Set the HTTP implementation adapter by HTTP profile identifier.
 *
 * Use this function to change the HTTP profile. The function should be called
 * before starting a Java application. When the Java application finished, the 
 * HTTP profile is reset.
 *
 * @param impl_id The HTTP profile identifier, could be one of the follows:
 *                  - #AJSC_HTTP_PROFILE_DISABLED
 *                  - #AJSC_HTTP_PROFILE_DEFAULT
 *                  - #AJSC_HTTP_PROFILE_EXTLIB
 *                  - #AJSC_HTTP_PROFILE_SERIAL
 *                  .
 *
 *  @return 
 *        - #AJMS_ERR_NONE: The HTTP implementation has been set normally.
 *        - #AJMS_ERR_ID_NOT_EXIST: The <i>impl_id</i> is invalid.
 *        .
 */
AJ_INT ajms_cfg_setHttpImpl(AJ_INT impl_id);

/**
 * \brief Sets the user-agent for HTTP connections used by the Java application.
 *
 * Use this function to set the user agent for HTTP connections for use by
 * Java application. The function should be called before
 * starting a Java application. When the Java application
 * finishes, the user agent information in the Java execution environment is reset.
 * Namely, the user agent should be set every time before starting a Java
 * application. 
 *
 * @param userAgent
 *        A pointer to the buffer holding the User-Agent header value
 * @return
 *      - #AJMS_ERR_NONE: The user agent was set normally.
 *      - #AJMS_ERR_INTERNAL: Failed to set user agent.
 *      .
 *
 *
 * @see ajms_arc_setHttpProxy 
 */
AJ_S32 ajms_cfg_setHttpUserAgent(AJ_U8* userAgent);

/**
 * \brief Sets the proxy for HTTP connections used by the Java application.
 * @param descriptor_p
 *        A pointer to the ProxyDescriptor structure holding the
 *        proxy information.
 * @return
 *     - #AJMS_ERR_NONE: The user agent was set normally. 
 *     - #AJMS_ERR_INTERNAL: Failed to set user agent. 
 *     .
 *
 * @see ajms_cfg_setHttpUserAgent
 */
AJ_S32 ajms_cfg_setHttpProxy(AJMS_ProxyDescriptor *descriptor_p);

/** @} */

/** \ingroup ams_system_property_query_aci */
/** 
 * @addtogroup AJMS_Config_ACI AJMS Configuration ACI
 * @{
 */

/**
 *  \brief Gets a string indicating a VM property.
 *  @param id
 *         An ID indicating the property to be acquired <br>
 *         The following IDs can be designated.<br>
 *         - #AJMS_SUPPORTED_CONFIGURATION<br>
 *           In the case of the AJMS described in this manual,
 *           "CLDC-1.0" or "CLDC-1.1" is returned.<br>
 *           More than one configuration cannot be supported at the same time. <br>
 *         - #AJMS_SUPPORTED_PROFILE<br>
 *           If more than one profile is supported, the return string lists
 *           each profile separated by a blank space. <br>
 *           In the case of the AJMS described in this manual, "MIDP-2.0"
 *           or "MIDP-1.0" is returned.
 *         - #AJMS_SUPPORT_TRUSTED_MIDLET<br>
 *           In the case of AJMS supports trusted midlet or not, "support" or "non-support"
 *           is returned.
 *         - #AJMS_SUPPORT_CATEGORY<br>
 *           In the case of AJMS supports category or not, "support" or "non-support"
 *           is returned.
 *         - #AJMS_LAYOUT_VERSION<br>
 *           Query the AJMS file layout version in ASCII string with NULL terminated char.
 *         .
 *  @param errorCode
 *         A pointer to the memory area for receiving the error code that
 *         indicates the result of the function processing.<br>
 *         One of the following values is put in this memory area.<br>
 *         - #AJMS_ERR_NONE: Succeeded <br>
 *         - #AJMS_ERR_ID_NOT_EXIST: An illegal ID was designated.
 *         - #AJMS_ERR_FILE_NOT_EXIST: Only happen when query AJMS file layout 
 *           version. The error code indicates the default index file could not
 *           be found in the AJMS image. Please refer to \ref page_pik_introduction 
 *           for the detail of the romzied AJMS file.
 *         - #AJMS_ERR_INVALID_FORMAT: Only happen when query AJMS file layout 
 *           version. The error code indicates the default index file format
 *           is invalid. Please refer to \ref page_pik_introduction 
 *           for the detail of the romzied AJMS file.
 *         - #AJMS_ERR_INTERNAL: Internal error;
 *         .
 *  @return
 *         A pointer to the memory area holding the property string for the
 *         designated ID with null terminated charatcer.<br>
 */
const AJ_U8 *ajms_cfg_getProperty(AJ_S32 id, AJ_S32 *errorCode);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_EIME_HEADER */
