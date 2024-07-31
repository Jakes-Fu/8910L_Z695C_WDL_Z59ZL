#ifndef _AJMS_PCS_HEADER
#define _AJMS_PCS_HEADER

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @file ajms_pcs.h
 * Permission Control ACI
 */

#include "aj_type.h"

/** @addtogroup permission_control_system_aci 
 *  @{ 
 */

/*
 *
 *  Interfaces listed in the group are used for manage protection domain, 
 *  including these functionalities:
 *
 *  - List all permissions in one protection domain.
 *  - List all permission groups in one protection domain.
 *  - List all relations between the permission groups in one protection domain.
 *  - List all restrictions between the permission groups in one protection domain.
 *  - Change locale string table.
 *  .
 *
 */

/***********************************************************
 * Constant Variable
 ***********************************************************/

/** 
 * @addtogroup Permission_Status_Code Permission Constant Type
 * @{
 */
 
/**
 * \brief The permission group is private.
 *
 * If the permission group is made public, a Java application 
 * can find out all the permission in that group using the 
 * <code>javax.microedition.midlet.MIDlet</code> class <code>checkPermission</code> 
 * method. The private permission group setting cannot be changed by user.
 */
#define AJMS_PCS_ISPRIVATE         (0x1000)

/**
 * \brief The permission group is always allowed.
 * 
 * The Java application is allowed to use the feature without confirming with the user.
 * The allowed permission group setting cannot be changed by user.
 */
#define AJMS_PCS_STATE_ALLOWED     (0x100)

/**
 * \brief The permission group could be changed by user.
 *
 * Indicates that the user must be asked whether the Java application is allowed to use a feature.
 */
#define AJMS_PCS_STATE_USER        (0x200)

/**
 * \brief The permission group does not exist.
 *
 * Indicates that the permission for use of the feature does not exist in the protection 
 * domain to which the Java application belongs. This value could only be occurrd
 * when fatal error occurred.
 */
#define AJMS_PCS_STATE_NOT_EXIST   (0x400)

/**
 * \brief BLANKET permission mode.
 * 
 * Indicates that if the user allows use of a feature, 
 * the permission is valid while the Java application is running, 
 * and the permission applies thereafter.
 */
#define AJMS_PCS_MODE_BLANKET      (0x1)

/**
 * \brief SESSION permission mode.
 *
 * Indicates that if the user allows use of a feature, the permission is valid
 * during the current Java application session.
 */
#define AJMS_PCS_MODE_SESSION      (0x2)

/**
 * \brief ONESHOT permission mode.
 * 
 * Indicates that if the user allows use of a feature, the permission is valid only this time.
 */
#define AJMS_PCS_MODE_ONESHOT      (0x4)

/**
 * \brief NO permission mode.
 * 
 * Indicates that use of the feature is not permitted.
 */
#define AJMS_PCS_MODE_NO           (0x8)

/**
 * \brief BLANKET permission mode with user grant authority.
 * 
 *  Spcecial BLANKET mode that indicates that user has already granted authority.
 *  This mode is only used by #ajms_mss_getGroupMode() and 
 *  #ajms_mss_setGroupMode().
 */
#define AJMS_PCS_MODE_BLANKET_GRANTED (0x11)

/** @} */

/** 
 * @addtogroup Protection_Domain_Identifier Protection Domain Identifier
 * @{
 */

/*
 * Same as the PKCS#11 spec. 
 * Java MIDP security domain: 
 * 0 = unspecified (default value), 
 * 1 = manufacturer, 
 * 2 = operator, 
 * 3 = third party
 */
/** \brief Indicates the Unspecified domain ID for midlet suite */
#define AJMS_PCS_DOMAIN_UNSPECIFIED  (0)
/** \brief Indicates manufacturer domain ID for midlet suite */
#define AJMS_PCS_DOMAIN_MANUFACTURER (1)
/** \brief Indicates operator domain ID for midlet suite */
#define AJMS_PCS_DOMAIN_OPERATOR     (2)
/** \brief Indicates third party domain ID for midlet suite */
#define AJMS_PCS_DOMAIN_THIRD_PARTY  (3)

/** @} */

/** 
 * @addtogroup Restriction_ACI Restriction ACI
 * @{
 */

/**
 * \brief Permission conflict level: No conflict.
 */
#define AJMS_PCS_CONFLICT_NORMAL     (0)

/**
 * \brief Permission conflict level: Alert message required.
 */
#define AJMS_PCS_CONFLICT_ALERT      (1)

/**
 * \brief Permission conflict level: Mutual exclusion processing required.
 */
#define AJMS_PCS_CONFLICT_EXCLUSIVE  (2)

/** @} */

/***********************************************************
 * Structures
 ***********************************************************/
 
/** 
 * @addtogroup Permission_ACI Permission ACI
 * @{
 */
 
/** 
 * \brief The structure type for containing a permission information.
 */
typedef struct _AJMS_Permission
{
    /** \brief The name of permission in UTF-8 encoding. */
    AJ_U8* name;
    /** \brief The permission group ID that this permission belongs to. */
    AJ_S32 group;
} AJMS_Permission;

/** @} */

/** 
 * @addtogroup Permission_Group_ACI Permission Group ACI
 * @{
 */

/** 
 * \brief The structure type for containing a permission group information.
 */
typedef struct _AJMS_DomainGroup
{
    /** \brief The default setting for the permission group. */
    AJ_S32  defaultMode;
    /** \brief The possible settings for the permission group. */
    AJ_S32  interactiveModes;
    /** \brief The permission dialog title that will displayed when VM needs
     *         to query the user of the permission setting. The string is in
     *         UTF-8 encoding.
     */
    AJ_U8*  title;
    /** \brief The permission dialog message that will displayed when VM needs
     *         to query the user of the permission setting. The string is in
     *         UTF-8 encoding.
     */
    AJ_U8*  message;
    /** \brief The permission dialog message that will displayed when AMS needs
     *         to query the user of the permission setting. The string is in
     *         UTF-8 encoding.
     */
    AJ_U8*  ams;
} AJMS_DomainGroup;

/** @} */

/** 
 * @addtogroup Relation_ACI Relation ACI
 * @{
 */

/** 
 * \brief The structure type for containing a relation information for two permission groups.
 */
typedef struct _AJMS_DomainRelation
{
    /** \brief The first permission group ID. */
    AJ_S32 group0;
    /** \brief The first permission group setting. */
    AJ_S32 state0;
    /** \brief The second permission group ID. */
    AJ_S32 group1;
    /** \brief The permission group setting that second permission group should be set. */
    AJ_S32 state1;
} AJMS_DomainRelation;

/** @} */

/** 
 * @addtogroup Restriction_ACI
 * @{
 */

/** 
 * \brief The structure type for containing a restriction information for two permission groups.
 */
typedef struct _AJMS_DomainRestriction
{
    /** \brief The first permission group ID. */
    AJ_S32 group0;
    /** \brief The first permission group setting. */
    AJ_S32 state0;
    /** \brief The second permission group ID. */
    AJ_S32 group1;
    /** \brief The second permission group setting. */
    AJ_S32 state1;
    
    /** \brief The conflict level. It must be #AJMS_PCS_CONFLICT_ALERT or 
     *         #AJMS_PCS_CONFLICT_EXCLUSIVE 
     */
    AJ_S32 conflict;
    /** \brief The permission group setting that for either <i>group0</i> or <i>group1</i>. */
    AJ_S32 state2;
    
    /** \brief The permission dialog title that will displayed when VM needs
     *         to query the user of the permission setting. The string is in
     *         UTF-8 encoding.
     */
    AJ_U8* title;
    /** \brief The permission dialog message that will displayed when VM needs
     *         to query the user of the permission setting. The string is in
     *         UTF-8 encoding.
     */
    AJ_U8* message;
    /** \brief The message that should be displayed on first mutual exclusive 
     *         choice box.
     */
    AJ_U8* choice0;
    /** \brief The message that should be displayed on second mutual exclusive 
     *         choice box.
     */
    AJ_U8* choice1;
} AJMS_DomainRestriction;

/** @} */

/***********************************************************
 * APIs
 ***********************************************************/

/** 
 * @addtogroup Permission_Control_System_ACI Permission Control System ACI
 * @{
 */

/**
 * \brief Initialize protection domain control system. 
 *
 * In most cases, the initialization is done by #ajms_init(). AMS does not need to 
 * initialize with function again. Inside this function, the AJMS
 * tries to load the pre-romized PIK data. The detail about PIK, please
 * refer to \ref page_pik_introduction.
 *
 * @return
 *       - #AJMS_ERR_NONE: Succeed.
 *       - #AJMS_ERR_FILE_NOT_EXIST: Protection domain file is missing.
 *       .
 */
AJ_S32 ajms_pcs_init( void );

/**
 * \brief Finalize protection domain control system.
 *
 * In most cases, the finalization is done by #ajms_fini(). AMS does not need to 
 * finilize with this function again. 
 *
 * @return
 *       #AJMS_ERR_NONE: Succeed.
 *
 */
AJ_S32 ajms_pcs_fini( void );
 
/**
 * \brief Change the locale string table.
 *
 * The string table is defined in PIK config file. For example, if PIK config file
 * contains <code>"StringTable-2: input/cn.res"</code>, AMS could change the string table by 
 * set table id to 2 using this function. 
 *
 * @param tableId
 *        The string table Id defined in PIK config file. Please refer to the 
 *        \ref page_pik_config_file for details.
 *
 * @return
 *      - #AJMS_ERR_NONE: Succeed.
 *      - #AJMS_ERR_ID_NOT_EXIST: No such string table. Please check the
 *        \ref page_pik_config_file for the locale string table setup.
 *      .

 */
AJ_S32 ajms_pcs_setStringTable( AJ_S32 tableId );

/** @} */

/** 
 * @addtogroup Permission_ACI
 * @{
 */

/**
 * \brief Get the number of permissions for certain domain.
 *
 * @param domainId
 *        The domain Id. It should be one of:
 *        - #AJMS_PCS_DOMAIN_UNSPECIFIED 
 *        - #AJMS_PCS_DOMAIN_MANUFACTURER 
 *        - #AJMS_PCS_DOMAIN_OPERATOR
 *        - #AJMS_PCS_DOMAIN_THIRD_PARTY
 *        .
 *
 * @return 
 *        - 0 or positive value - The number of permissions in certain protection domain.
 *        - #AJMS_ERR_ID_NOT_EXIST - No such domain.
 *        .
 */
AJ_S32 ajms_pcs_getPermissionCount( AJ_S32 domainId );

/**
 * \brief Get one certain permission information in one domain.
 *
 * @param domainId
 *        The domain Id. It should be one of:
 *        - #AJMS_PCS_DOMAIN_UNSPECIFIED 
 *        - #AJMS_PCS_DOMAIN_MANUFACTURER 
 *        - #AJMS_PCS_DOMAIN_OPERATOR
 *        - #AJMS_PCS_DOMAIN_THIRD_PARTY
 *        .
 * @param permId
 *        The permission id. The id should be between 0 to the number of total permissions minus 1.
 *
 * @param info
 *        A pointer to the memory area for receiving the permission information.
 *
 * @return 
 *      - #AJMS_ERR_NONE - Succeed.
 *      - #AJMS_ERR_ID_NOT_EXIST - The <i>domainId</i> or <i>permId</i> is invalid .
 *      - #AJMS_ERR_INVALID_PARAMS - The <i>info</i> is null.
 *      .
 */ 
AJ_S32 ajms_pcs_getPermission( AJ_S32 domainId, 
                               AJ_S32 permId,
                               AJMS_Permission* info );

/** @} */

/** 
 * @addtogroup Permission_Group_ACI
 * @{
 */

/**
 * \brief Get number of groups for certain domain.
 *
 * @param domainId
 *        The domain Id. It should be one of:
 *        - #AJMS_PCS_DOMAIN_UNSPECIFIED 
 *        - #AJMS_PCS_DOMAIN_MANUFACTURER 
 *        - #AJMS_PCS_DOMAIN_OPERATOR
 *        - #AJMS_PCS_DOMAIN_THIRD_PARTY
 *        .
 * @return 
 *        - 0 or positive value - The number of groups in certain protection domain.
 *        - #AJMS_ERR_ID_NOT_EXIST - No such domain.
 *        .
 */
AJ_S32 ajms_pcs_getGroupCount( AJ_S32 domainId );

/**
 * \brief Get certain group information in certain domain.
 *
 * @param domainId
 *        The domain Id. It should be one of:
 *        - #AJMS_PCS_DOMAIN_UNSPECIFIED 
 *        - #AJMS_PCS_DOMAIN_MANUFACTURER 
 *        - #AJMS_PCS_DOMAIN_OPERATOR
 *        - #AJMS_PCS_DOMAIN_THIRD_PARTY
 *        .
 * @param groupId
 *        The group id. The id should be between 0 to the number of total groups minus 1.
 *
 * @param info
 *        A pointer to the memory area for receiving the group information.
 *
 * @return 
 *        - #AJMS_ERR_NONE - Succeed.
 *        - #AJMS_ERR_ID_NOT_EXIST - No such domain or group.
 *        - #AJMS_ERR_INVALID_PARAMS: The <i>info</i> is null.
 *        .
 */
AJ_S32 ajms_pcs_getGroup( AJ_S32 domainId, 
                          AJ_S32 groupId, 
                          AJMS_DomainGroup* info );

/** @} */

/** 
 * @addtogroup Relation_ACI
 * @{
 */

/**
 * \brief Get number of relations for certain domain.
 *
 * @param domainId
 *        The domain Id. It should be one of:
 *        - #AJMS_PCS_DOMAIN_UNSPECIFIED 
 *        - #AJMS_PCS_DOMAIN_MANUFACTURER 
 *        - #AJMS_PCS_DOMAIN_OPERATOR
 *        - #AJMS_PCS_DOMAIN_THIRD_PARTY
 *        .
 * @return 
 *        - 0 or positive value - The number of relations in certain protection domain.
 *        - #AJMS_ERR_ID_NOT_EXIST - No such domain.
 */
AJ_S32 ajms_pcs_getRelationCount( AJ_S32 domainId );

/**
 * \brief Get certain relation information in certain domain.
 *
 * @param domainId
 *        The domain Id. It should be one of:
 *        - #AJMS_PCS_DOMAIN_UNSPECIFIED 
 *        - #AJMS_PCS_DOMAIN_MANUFACTURER 
 *        - #AJMS_PCS_DOMAIN_OPERATOR
 *        - #AJMS_PCS_DOMAIN_THIRD_PARTY
 *        .
 * @param relationId
 *        The relation id. The id should be between 0 to the number of total relations minus 1.
 *
 * @param info
 *        A pointer to the memory area for receiving the relation information.
 *
 * @return 
 *        - #AJMS_ERR_NONE - Succeed.
 *        - #AJMS_ERR_ID_NOT_EXIST - No such domain or relation.
 *        - #AJMS_ERR_INVALID_PARAMS: The <i>info</i> is null.
 */
AJ_S32 ajms_pcs_getRelation( AJ_S32 domainId,
                             AJ_S32 relationId,
                             AJMS_DomainRelation* info );

/** @} */

/** 
 * @addtogroup Restriction_ACI
 * @{
 */

/**
 * \brief Get number of restrictions for certain domain.
 *
 * @param domainId
 *        The domain Id. It should be one of:
 *        - #AJMS_PCS_DOMAIN_UNSPECIFIED 
 *        - #AJMS_PCS_DOMAIN_MANUFACTURER 
 *        - #AJMS_PCS_DOMAIN_OPERATOR
 *        - #AJMS_PCS_DOMAIN_THIRD_PARTY
 *        .
 * @return 
 *        - 0 or positive value - The number of restrictions in certain protection domain.
 *        - #AJMS_ERR_ID_NOT_EXIST - No such domain.
 */
AJ_S32 ajms_pcs_getRestrictionCount( AJ_S32 domainId );
                      
/**
 * \brief Get certain restrictions information in certain domain.
 *
 * @param domainId
 *        The domain Id. It should be one of:
 *        - #AJMS_PCS_DOMAIN_UNSPECIFIED 
 *        - #AJMS_PCS_DOMAIN_MANUFACTURER 
 *        - #AJMS_PCS_DOMAIN_OPERATOR
 *        - #AJMS_PCS_DOMAIN_THIRD_PARTY
 *        .
 * @param restricId
 *        The restrictions id. The id should be between 0 to the number of total restrictions minus 1.
 *
 * @param info
 *        A pointer to the memory area for receiving the restriction information.
 *
 * @return 
 *        - #AJMS_ERR_NONE: Succeed.
 *        - #AJMS_ERR_ID_NOT_EXIST: No such domain or restriction.
 *        - #AJMS_ERR_INVALID_PARAMS: The <i>info</i> is null.
 */
AJ_S32 ajms_pcs_getRestriction( AJ_S32 domainId, 
                                AJ_S32 restricId, 
                                AJMS_DomainRestriction* info );
                                
/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif
