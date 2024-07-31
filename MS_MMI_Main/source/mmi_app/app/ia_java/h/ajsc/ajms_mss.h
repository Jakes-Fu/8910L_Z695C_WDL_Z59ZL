/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */
/** 
 * @file ajms_mss.h
 * Application Security Setting ACI
 */

#ifndef AJMS_MSS_HEADER
#define AJMS_MSS_HEADER

#ifdef __cplusplus
extern "C"{
#endif

#include "aj_type.h"
#include "ajms_pcs.h"
#include "ajms_common.h"

/** @addtogroup ams_security_setting_aci
 *  @{ 
 */

/*
 *  Interfaces listed in the group are used for application security setting, 
 *  including these functionalities:
 *       - Get midlet suite's protection domain.
 *       - Get midlet suite's security setting.
 *       - Set midlet suite's security setting.
 *       .
 */

/** 
 * @addtogroup ajms_mss_error_code Security Setting Error Code
 * @{
 */

/** \brief No error. */
#define AJMS_MSS_ERR_NONE                (0)
/** \brief CONFLICT happened, the level is ALERT. */
#define AJMS_MSS_ERR_CONFLICT_ALERT     (-1)
/** \brief CONFLICT happened, the level is EXCLUSIVE. */
#define AJMS_MSS_ERR_CONFLICT_EXCLUSIVE (-2)
/** \brief Invalid parameter. */
#define AJMS_MSS_ERR_INVALID_PARAM      (-3)
/** \brief Group is PRIVATE. */
#define AJMS_MSS_ERR_GROUP_IS_PRIVATE   (-4)
/** \brief Group is ALLOWED. */
#define AJMS_MSS_ERR_GROUP_IS_ALLOWED   (-5)
/** \brief IO failed. */
#define AJMS_MSS_ERR_IO                 (-6)
/** \brief Other error. */
#define AJMS_MSS_ERR_INTERNAL           (-7)

/** @} */

/** 
 * @addtogroup ajms_mss_aci Security Setting ACI
 * @{
 */

/**
 * \brief Get protection domain for one installed application.
 *
 * @param suiteId 
 *        The identifier of the application.
 * @return
 *        - One of valid protection domain ID, which includes:
 *          - #AJMS_PCS_DOMAIN_UNSPECIFIED: The application is under unspecified domain.
 *          - #AJMS_PCS_DOMAIN_MANUFACTURER: The application is under manufacturer domain.
 *          - #AJMS_PCS_DOMAIN_OPERATOR: The application is under operator domain.
 *          - #AJMS_PCS_DOMAIN_THIRD_PARTY: The application is under third party domain.
            .
 *        - #AJMS_MSS_ERR_INVALID_PARAM: The <i>suiteId</i> is not valid.
 *        - #AJMS_MSS_ERR_IO: An file IO error occurred.
 *        - #AJMS_MSS_ERR_INTERNAL: Others.
 *        .
 */
AJ_S32 ajms_mss_getDomainId( const AJ_S32 suiteId );

/**
 * \brief Get one group current setting of an application.
 *
 * @param  suiteId
 *         The identifier of the application.
 * @param  groupId
 *         The identifier of the permission group. The permission groups info could be get 
 *         by #ajms_pcs_getGroup.
 * @param  mode
 *         A pointer to the memory area for receiving the current permission group setting. 
 *         It must be one of 
 *         - #AJMS_PCS_MODE_NO
 *         - #AJMS_PCS_MODE_ONESHOT 
 *         - #AJMS_PCS_MODE_SESSION 
 *         - #AJMS_PCS_MODE_BLANKET 
 *         - #AJMS_PCS_MODE_BLANKET_GRANTED
 *         .
 *         when AJMS_MSS_ERR_NONE is returned.
 *
 * @return
 *      - #AJMS_MSS_ERR_NONE             : successfully get group mode info.
 *      - #AJMS_MSS_ERR_INVALID_PARAM    : The <i>suiteId</i> or <i>groupId</i> is invalid.
 *      - #AJMS_MSS_ERR_GROUP_IS_ALLOWED : group setting is ALLOW, which cannot be changed.
 *      - #AJMS_MSS_ERR_IO               : IO Error.
 *      - #AJMS_MSS_ERR_INTERNAL         : Other error.
 *      .
 */
AJ_S32 ajms_mss_getGroupMode( const AJ_S32  suiteId, 
                              const AJ_S32  groupId,
                                    AJ_S32 *mode );

/**
 * \brief Change one of the permission group setting value.
 *
 * @param  suiteId
 *         The identifier of the application.
 * @param  groupId
 *         The identifier of the permission group. All permission groups info could be get 
 *         by #ajms_pcs_getGroup.
 * @param  mode
 *         The new group setting mode that caller want to change. It must
 *         be one of #AJMS_PCS_MODE_NO, #AJMS_PCS_MODE_ONESHOT, #AJMS_PCS_MODE_SESSION
 *         and #AJMS_PCS_MODE_BLANKET.
 * @param  info
 *         A pointer to the memory area for receiving the exclusive information when return
 *         value is #AJMS_MSS_ERR_CONFLICT_ALERT or #AJMS_MSS_ERR_CONFLICT_EXCLUSIVE.
 * @return
 *        - #AJMS_MSS_ERR_NONE             : Successfully changed group mode info.
 *        - #AJMS_MSS_ERR_INVALID_PARAM    : One of the follows,
 *          - The <i>suiteId</i> is invalid.
 *          - The <i>groupId</i> is invalid.
 *          - The <i>mode</i> in not one of:
 *               - #AJMS_PCS_MODE_NO
 *               - #AJMS_PCS_MODE_ONESHOT
 *               - #AJMS_PCS_MODE_SESSION
 *               - #AJMS_PCS_MODE_BLANKET
 *               - #AJMS_PCS_MODE_BLANKET_GRANTED
 *               .
 *          .
 *        - #AJMS_MSS_ERR_CONFLICT_ALERT   : The fuction call triggers conflict which 
 *                    is "ALERT" level.  Caller should show the alert information in 
 *                    <i>info</i> to alert user.<br>
 *                    NOTE: permission group state still be changed when "ALERT" happened.                                   
 *        - #AJMS_MSS_ERR_CONFLICT_EXCLUSIVE : The fuction call triggers conflict which 
 *                    is "EXCLUSIVE" level.<br>
 *                    NOTE: permission group state was not changed when "EXCLUSIVE" happened.
 *        - #AJMS_MSS_ERR_GROUP_IS_PRIVATE : The PRIVATE permission group could not be changed
 *        - #AJMS_MSS_ERR_GROUP_IS_ALLOWED : The PRIVATE permission ALLOWED group could not be changed
 *        - #AJMS_MSS_ERR_IO               : IO Error.
 *        - #AJMS_MSS_ERR_INTERNAL         : Other error.
 *        .
 */
AJ_S32 ajms_mss_setGroupMode( const AJ_S32 suiteId, 
                              const AJ_S32 groupId, 
                              const AJ_S32 mode,
                              AJMS_DomainRestriction *info );

/** @} */ 

#ifdef __cplusplus
}
#endif 

#endif /* AJMS_MSS_HEADER */
