/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */
/**
 *  @file ajms_ota.h
 *  OTA ACI
 */

#ifndef AJMS_OTA_HEADER
#define AJMS_OTA_HEADER

#ifdef __cplusplus
extern "C"{
#endif
#include "ajms_common.h"
/** @addtogroup ams_installation_deletion_aci
 *  @{ 
 */

/*
 *  Interfaces listed in the group are used for application installation, 
 *  including these functionalities:
 *       - Verify a midlet suite's Jad/Jar data.
 *       - Install a midlet suite by Jad/Jar data.
 *       - Install a midlet suite by Jad/Jar native file path.
 *       - Delete an installed midlet suite.
 *       .
 */
 
/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/
/** 
 * @addtogroup Application_Checking_ACI Midlet Suite Verification ACI
 * @{
 */
 
/** \brief Indicate the data type passed to #ajms_ota_check is Jad file. */
#define AJMS_CHECK_TYPE_JAD (1)
/** \brief Indicate the data type passed to #ajms_ota_check is Jar file. */
#define AJMS_CHECK_TYPE_JAR (2)

/** \brief There are no application with the same suite name and vendor name in AJMS. */
#define AJMS_UPDATE_MIDLET_SUITE_NOT_EXIST   (1)
/** \brief There is a application with the same suite name and vendor name in AJMS. 
 *         But since unsigned application could not update signed application. The
 *         upgrade is not allowed to perform.
 */
#define AJMS_UPDATE_SECURITY_VIOLATTION      (2)
/** \brief There is a application with the same suite name and vendor name in AJMS.
 *         The Jad/Jar passed to #ajms_ota_check has newer version than one in AJMS.
 */
#define AJMS_UPDATE_NEWER_VERSION            (3)
/** \brief There is a application with the same suite name and vendor name in AJMS.
 *         The Jad/Jar passed to #ajms_ota_check is with the same version as the one in AJMS.
 */
#define AJMS_UPDATE_SAME_VERSION             (4)
/** \brief There is a application with the same suite name and vendor name in AJMS.
 *         The Jad/Jar passed to #ajms_ota_check has older version than one in AJMS.
 */
#define AJMS_UPDTAE_OLDER_VERSION            (5)

/** @} */

/** 
 * @addtogroup Application_Installation_ACI Midlet Suite Installation ACI
 * @{
 */

/** \brief Installation flag which indicates to install as default setting. */
#define AJMS_INSTALL_DEFAULT                 (0x00)
/** \brief Installation flag which indicates to keep original rms files during upgrading. */
#define AJMS_INSTALL_KEEP_RMS                (0x01)
/** \brief Installation flag which indicates to do Jar precheck during installation. */
#define AJMS_INSTALL_PRECHECK                (0x02)

/** \brief Installation flag which indicates to make a copy of Jar file.
 *  It should only be used in #ajms_ota_installEx. 
 */
#define AJMS_INSTALL_COPY_JAR                (0x10)

/** \brief Installation flag which indicates to make a copy of Jad file.
 *  It should only be used in #ajms_ota_installEx. 
 */
#define AJMS_INSTALL_COPY_JAD                (0x20)

/** \brief Installation flag which indicates to force installed application to be 
  *        under manufacturer domain without doing any integrity checking.
  */
#define AJMS_INSTALL_AS_MANUFACTURER         (0x100)

/** \brief Installation flag which indicates to force install application to be 
  *        under operator domain without doing any integrity checking.
  */
#define AJMS_INSTALL_AS_OPERATOR             (0x200)

/** \brief Installation flag which indicates to force install application to be  
  *        under third party domain without doing any integrity checking.
  */
#define AJMS_INSTALL_AS_THIRD_PARTY          (0x400)

/** \brief Installation flag which indicates to force install application to be 
  *        under unspecific domain without doing any integrity checking.
  */
#define AJMS_INSTALL_AS_UNSPECIFIED          (0x800)

/** @} */

/** 
 * @addtogroup Ota_Code OTA Error Code
 * @{
 */

/** \brief OTA status code: Success */
#define AJMS_ERR_OTA_OK                         (900)
/** \brief OTA status code: Insufficient space in file system */
#define AJMS_ERR_OTA_NO_ENOUGH_SPACE            (901)
/** \brief OTA status code: User Cancelled */
#define AJMS_ERR_OTA_USER_CANCEL                (902)
/** \brief OTA status code: Loss of Service */
#define AJMS_ERR_OTA_LOSS_OF_SERVICE            (903)
/** \brief OTA status code: JAR size Mismatch */
#define AJMS_ERR_OTA_JAR_SIZE_MISMATCH          (904)
/** \brief OTA status code: Attribute Mismatch */
#define AJMS_ERR_OTA_ATTRIBUTE_MISMATCH         (905)
/** \brief OTA status code: Invalid Descriptor */
#define AJMS_ERR_OTA_INVALID_JAD                (906)
/** \brief OTA status code: Invalid JAR */
#define AJMS_ERR_OTA_INVALID_JAR                (907)
/** \brief OTA status code: Incompatible Configuration or Profile */
#define AJMS_ERR_OTA_CLDC_MIDP_VERSION          (908)
/** \brief OTA status code: Application authetication failure */
#define AJMS_ERR_OTA_AUTHENTICATION_FAILURE     (909)
/** \brief OTA status code: Application authorization failure */
#define AJMS_ERR_OTA_AUTHORIZATION_FAILURE      (910)
/** \brief OTA status code: Push registration failure */
#define AJMS_ERR_OTA_PUSH_REGISTRATION_FALIURE  (911)
/** \brief OTA status code: Deletion Notification */
#define AJMS_ERR_OTA_DELETION_NOTIFICATION      (912)
/** \brief OTA status code: Non-defined OTA error code */
#define AJMS_ERR_OTA_NOT_DEFINE                 (999)

/** @} */

/** 
 * @addtogroup Application_Checking_ACI
 * @{
 */

/**
 * \brief the data structure for the result of Jad/Jar checking. 
 */
typedef struct _AJMS_CheckResult
{
    /** \brief The name string (a NUL-terminated UTF-8 string) of the midlet suite. */
    AJ_U8     name[AJMS_MAX_ENTRY_NAME_LENGTH + 1];
    
    /** \brief The vendor string (a NUL-terminated UTF-8 string) of the midlet suite. */
    AJ_U8     vendor[AJMS_MAX_ENTRY_NAME_LENGTH + 1];
    
    /** \brief The URL string (a NUL-terminated UTF-8 string) for downloading JAR. 
     *         This field will not be used in Jar-Checking. */
    AJ_U8     jarUrl[AJMS_MAX_ENTRY_URL_LENGTH + 1];
    
    /** \brief The URL string (a NUL-terminated UTF-8 string) for install notification. */
    AJ_U8     installNotifyUrl[AJMS_MAX_ENTRY_URL_LENGTH + 1];
    
    /** \brief OTA status code. 
     *
     *         The status code must be one of:
     *       - #AJMS_ERR_OTA_OK
     *       - #AJMS_ERR_OTA_NO_ENOUGH_SPACE
     *       - #AJMS_ERR_OTA_USER_CANCEL
     *       - #AJMS_ERR_OTA_LOSS_OF_SERVICE
     *       - #AJMS_ERR_OTA_INVALID_JAD
     *       - #AJMS_ERR_OTA_INVALID_JAR
     *       - #AJMS_ERR_OTA_CLDC_MIDP_VERSION
     *       - #AJMS_ERR_OTA_NOT_DEFINE
     *       .
     */
    AJ_S32    otaCode;
    
    /** \brief The value indicate if MIDlet-Push-1 exist in Jad */
    AJ_BOOL   hasPush;
    
    /** \brief The name of MIDlet-1 property. If the MIDlet-1 property
     *  is longer than AJMS_MAX_ENTRY_NAME_LENGTH, this function only takes
     *  first #AJMS_MAX_ENTRY_NAME_LENGTH bytes and plus
     *  a null-character at the end.
     */
    AJ_U8     midlet[AJMS_MAX_ENTRY_NAME_LENGTH + 1];
    
    /** \brief The version value in major field in the checking data. */
    AJ_S32  majorVersion;
    /** \brief The version value in minor field in the checking data. */
    AJ_S32  minorVersion;
    /** \brief The version value in micro field in the checking data. */
    AJ_S32  microVersion;
    
    /** \brief The size in bytes of JAR content. */
    AJ_U32    jarSize;
    
    /** \brief The minimum file storage space (in bytes) needed for midlet suite. */
    AJ_U32    minimumInstallSize;
    
    /** \brief The candidate application ID for the next installation
     *
     *         For type 3 application, install as "Device protected application",
     *         AMS might need to know the candidate application ID for the
     *         next installation. For example, the AMS might want to 
     *         change the JAD/JAR filename as unique one based on application
     *         ID.
     */
    AJ_S32    candidateId;
    
    /** 
     * \brief Avalue indicating whether midlet suite update information.
     *
     * - #AJMS_UPDATE_MIDLET_SUITE_NOT_EXIST: AJMS has no midlet suite with same vendor name and midlet suite name.<br>
     * - #AJMS_UPDATE_SECURITY_VIOLATTION: The midlet suite in AJMS is trusted, it could not be updated by
     *                                  an untrusted midlet suite.
     * - #AJMS_UPDATE_NEWER_VERSION: the new midlet suite has newer version and it allows to update.
     * - #AJMS_UPDATE_SAME_VERSION: the new midlet suite has same version and update is not needed.
     * - #AJMS_UPDTAE_OLDER_VERSION: the new midlet suite has older version and update should not be performed.
     * .
     */
    AJ_S32   update;

    /** 
     * \brief The midlet suite ID with the same vendor name and midlet suite name.
     *
     * When <i>update</i> is #AJMS_UPDATE_NOT_EXIST, this field will be filled by some
     * invalid midlet suite id that for #ajms_ota_install() usage.
     */
    AJ_S32 oldId;
    
    /** \brief The version value in major field for the existing application that already 
     *         in AJMS. */
    AJ_S32  oldMajorVersion;
    /** \brief The version value in minor field for the existing application that already 
     *         in AJMS. */
    AJ_S32  oldMinorVersion;
    /** \brief The version value in micro field for the existing application that already 
     *         in AJMS. */
    AJ_S32  oldMicroVersion;
    
    AJ_S32  domainId;
    AJ_S32  chainId;
} AJMS_CheckResult;

/** @} */

/** 
 * @addtogroup Application_Installation_ACI
 * @{
 */

/**
 * \brief the data structure for the result of installation. 
 */
typedef struct _AJMS_InstallResult
{
    /** \brief The midlet suite ID. */
    AJ_S32    suiteId;
    
    /** \brief The protection domain Id.
     *         
     *         The protection domain ID must be one of:
     *       - #AJMS_PCS_DOMAIN_UNSPECIFIED 
     *       - #AJMS_PCS_DOMAIN_MANUFACTURER 
     *       - #AJMS_PCS_DOMAIN_OPERATOR
     *       - #AJMS_PCS_DOMAIN_THIRD_PARTY
     *       .
     */
    AJ_S32    domainId;
    
    /** \briefThe NUL-terminated UTF-8 string for install notification. */
    AJ_U8     installNotifyUrl[AJMS_MAX_ENTRY_URL_LENGTH + 1];
    
    /** \brief OTA status code. 
     *
     *         The status code must be one of:
     *       - #AJMS_ERR_OTA_OK
     *       - #AJMS_ERR_OTA_NO_ENOUGH_SPACE
     *       - #AJMS_ERR_OTA_USER_CANCEL
     *       - #AJMS_ERR_OTA_LOSS_OF_SERVICE
     *       - #AJMS_ERR_OTA_JAR_SIZE_MISMATCH
     *       - #AJMS_ERR_OTA_ATTRIBUTE_MISMATCH
     *       - #AJMS_ERR_OTA_INVALID_JAD
     *       - #AJMS_ERR_OTA_INVALID_JAR
     *       - #AJMS_ERR_OTA_CLDC_MIDP_VERSION
     *       - #AJMS_ERR_OTA_AUTHENTICATION_FAILURE
     *       - #AJMS_ERR_OTA_AUTHORIZATION_FAILURE
     *       - #AJMS_ERR_OTA_PUSH_REGISTRATION_FALIURE
     *       - #AJMS_ERR_OTA_DELETION_NOTIFICATION
     *       - #AJMS_ERR_OTA_NOT_DEFINE
     */
    AJ_S32    otaCode;
    
} AJMS_InstallResult;

/** @} */

/** 
 * @addtogroup Application_Checking_ACI
 * @{
 */

/**
 * \brief The function type of the result notificatin callback for #ajms_ota_check.
 * @param total
 *        The total checking steps. When the value of this parameter is -1,
 *        it indicates that an error occurred during JAD/JAR checking.
 *        The error code is passed in <code>step</code> parameter.
 * @param step
 *        - 0 or positive value: The current step.
 *        - #AJMS_ERR_USER_CANCEL: User cancel the checking procedure by return 0 in callback function.
 *        - #AJMS_ERR_INTERNAL: Other errors.
 *        - #AJMS_ERR_INVALID_FORMAT: Jad or Jar file format is illegal.
 *        - #AJMS_ERR_MEMORY_OVERFLOW: Insufficient memory space.
 *        - #AJMS_ERR_NOT_SUPPORT: No "MIDlet-1" in MANIFEST. This code only occurred in
 *                                 #AJMS_CHECK_TYPE_JAR checking.
 *        .
 *
 * @param result
 *        A pointer to the structure indicating the JAD/JAR checking result.
 *        This parameter is the pointer
 *        passed to #ajms_ota_check() and the buffer this parameter
 *        points to will be filled with the data. If <i>total > 0 </i>
 *        and <i>total != step</i>, all data put inside the structure
 *        pointed is only for tracing usage without guarantee the
 *        correctness. If <i>total == -1</i>, only the <i>otaCode</i>
 *        inside this structure is guarantee with correct value.
 *
 * @return
 *        Return 1 to continue verification.<br>
 *        Return 0 to cancel verification.
 *
 * A callback function of this type should be passed to #ajms_ota_check.
 * The callback function is to be implemented by the device manufacturer.
 *
 * Once an error is reported by this function, the checking process is aborted
 * and this function is no longer called.
 *
 * When this function returns 0, AJMS cancels the
 * checking procedure immediately and release all resources prepared for
 * verification. Finally, AJMS invokes the callback function with
 * <i>total</i> == -1 and <i>steps</i> == #AJMS_ERR_OTA_USER_CANCEL
 * for error notification.
 *
 * At the final step, the structure indicated by <i>result</i> 
 * contains the information of the checking data. 
 *
 * If the <i>hasPush</i> == AJ_TRUE, AMS should ask user to ask if
 * user allow this application to be installed with the push registry request.
 *
 * If the <i>update</i> is not #AJMS_UPDATE_MIDLET_SUITE_NOT_EXIST, AMS should
 * ask user if they want to perform such application upgrade.
 *
 * @see #ajms_ota_check()
 */
typedef AJ_S32 (*AJMS_CheckCB)
               ( AJ_S32 total, AJ_S32 step, AJMS_CheckResult *result );

/** @} */

/** 
 * @addtogroup Application_Installation_ACI
 * @{
 */

/**
 * \brief The function type of the progress/result notificatin callback for #ajms_ota_install.
 * @param total
 *        The total installation steps. When the value of this parameter is -1,
 *        it indicates that an error occurred during installation.
 *        The error code is passed in <i>step</i> parameter.
 * @param step
 *        The current step or an negative error code.
 *      - #AJMS_ERR_USER_CANCEL - User cancel the checking procedure by return 0 in callback function.
 *      - #AJMS_ERR_INTERNAL - Other errors.
 *      - #AJMS_ERR_INVALID_FORMAT - Jad or Jar file format is illegal.
 *      - #AJMS_ERR_MEMORY_OVERFLOW - Insufficient memory space.
 *      - #AJMS_ERR_NOT_SUPPORT: No "MIDlet-1" in MANIFEST. This code only occurred in
 *                               #AJMS_CHECK_TYPE_JAR checking.
 *      - #AJMS_ERR_NOT_ALLOWED - Not allow to install because of the security issue.
 *      - #AJMS_ERR_IO_ERROR - An IO error occurred.
 *      .
 * @param result
 *        A pointer to the structure indicating the installation result.
 *        This parameter is the pointer
 *        passed to #ajms_ota_install() and the buffer this parameter
 *        points to will be filled with the data. If <i>total > 0 </i>
 *        and <i>total != step</i>, all data put inside the structure
 *        pointed is only for tracing usage without guarantee the
 *        correctness. If <i>total == -1</i>, only the <i>otaCode</i>
 *        inside this structure is guarantee with correct value.
 *
 * @return
 *        Return 1 to continue installation.<br>
 *        Return 0 to cancel installation.
 *
 * A callback function of this type should be passed to #ajms_ota_install.
 * The callback function is to be implemented by the device manufacturer.
 *
 * The callback function is called several times in the KVE
 * thread context to indicate the progress of installation.
 *
 * Once an error is reported by this function, the installation process is aborted
 * and this function is no longer called.
 *
 * When this function returns 0, AJMS cancels the
 * installation procedure immediately and release all resources prepared for
 * installation. Finally, AJMS invokes the callback function with
 * <i>total</i> == -1 and <i>steps</i> == #AJMS_ERR_OTA_USER_CANCEL
 * for error notification.
 *
 * See @ref page_application_browsing for details of the MIDlet suite identifier.
 *
 * @see #ajms_ota_install
 */
typedef AJ_S32 (*AJMS_InstallCB)
               ( AJ_S32 total, AJ_S32 step, AJMS_InstallResult *result);

/** @} */

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Application_Checking_ACI
 * @{
 */

/**
 * \brief Requests Jad/Jar checking.
 *
 *        The #ajms_ota_check must be performed before any kind of
 *        installation. For Jad+Jar installation, AMS only needs to
 *        do the Jad checking before installation. For Jar-only 
 *        installation, AMS needs to do the Jar-checking before 
 *        installation.
 *
 * @param dataSize
 *        The size of the Jad/Jar data indicated by <i>dataAddr</i>.
 * @param dataAddr
 *        A pointer to the buffer holding the Jad/Jar.
 * @param dataRead
 *        The function address of an Jad/Jar reader callback function.<br>
 * @param dataType
 *        A flag to specific the <i>data</i> is Jad or Jar. The only possible value for this
 *        parameter is either #AJMS_CHECK_TYPE_JAD or #AJMS_CHECK_TYPE_JAR.
 * @param jadUrl
 *        A pointer to the string holding the Jad URL string. For Jar checking,
 *        this field should be AJ_NULL.
 * @param result
 *        A pointer to the structure for receiving verification result.
 * @param callback
 *        The address of the progress/result notification callback function.
 * @return
 *        - #AJMS_ERR_NONE: The checking request was sent normally. 
 *        - #AJMS_ERR_INVALID_PARAMS: Some parameter is invalid.<br>
 *        .
 *
 *
 * A value must be set in either dataAddr or dataRead.
 * If both are designated, the value designated in jadAddr is used.<br>
 *
 * @see AJMS_CheckCB
 */
AJ_S32 ajms_ota_check(
                             const AJ_S32                      dataSize,
                             const AJ_U8                      *dataAddr,
                             AJMS_Reader                       dataRead,
                             const AJ_S32                      dataType,
                             const AJ_U8                      *jadUrl,
                             AJMS_CheckResult                 *result,
                             AJMS_CheckCB                      callback);

/** @} */

/** 
 * @addtogroup Application_Installation_ACI
 * @{
 */

/**
 * \brief Installs a Java application by Jad's URL, Jad data and Jar data.
 * @param categoryId
 *        The category to bind the Java application to be installed.
 * @param jadSize
 *        The size of the Jad data indicated by <i>adfAddr</i>. Set 0 if install by Jar file only.
 * @param jadAddr
 *        A pointer to the buffer holding the Jad. Set AJ_NULL if install by Jar file only.
 * @param jadRead
 *        The function address of an Jad reader callback function. Set AJ_NULL if install by Jar file only.
 * @param jadUrl
 *        A pointer to the string holding the Jad URL string. Set AJ_NULL if by with Jar file only.
 * @param jarSize
 *        The size of the JAR data indicated by <i>jarAddr</i>.
 * @param jarAddr
 *        A pointer to the buffer holding the Jar file.
 * @param jarRead
 *        The function address of a JAR reader callback function.
 * @param flag
 *        Bit mask to set the following flags:
 *     - #AJMS_INSTALL_DEFAULT  - Default setting.
 *     - #AJMS_INSTALL_KEEP_RMS - Force to keep the old application RMS data after upgrade. The
 *                                flag is only meaningful when update is allowed. In other words,
 *                                user MUST use #ajms_ota_check to see get the upgrade
 *                                information first.
 *     - #AJMS_INSTALL_PRECHECK - Do Jar data precheck during installation. It will
 *                                speed up the midlet launch time but spend more time
 *                                during installation.
 *     - #AJMS_INSTALL_AS_MANUFACTURER - Force application to be under manufacturer domain.
 *     - #AJMS_INSTALL_AS_OPERATOR     - Force application to be under operator domain.
 *     - #AJMS_INSTALL_AS_THIRD_PARTY  - Force application to be under trusted third party domain.
 *     - #AJMS_INSTALL_AS_UNSPECIFIED  - Force application to be under untrusted domain.<br>
 *                                 Without doing real midlet suite integrity checking,
 *                                 raise one of these four flags directly assign the 
 *                                 domain to the installed application. Use these flags
 *                                 only when the domain is 
 *                                 determined before installation. For example,
 *                                 when application integrity checking is done by Wap 
 *                                 browser during Jar download.
 *     .
 * @param oldId
 *        Pass the <i>OldId</i> field in #AJMS_CheckResult returned by #ajms_ota_check.
 * @param callback
 *        The address of the progress/result notification callback function.
 * @param result 
 *        A pointer to the structure for receiving installation result.
 * @return
 *        - #AJMS_ERR_NONE: The install request has been sent normally. 
 *        - #AJMS_ERR_INVALID_PARAMS: Some parameter is invalid.
 *
 * Use this function to install a Java application into AJMS repository.
 * This function does not perform Jad/Jar check. It is device manufacturer's
 * responsibility to verify jad/manifest by invoking #ajms_ota_check before this function.
 *
 * The installation is performed asynchronously in the KVE thread.
 * The callback function <i>callback</i> is called several times in the KVE
 * thread context to indicate the verification progress or result.
 * See #AJMS_InstallCB for details.
 *
 *  A value must be set in either jarAddr or jarRead.
 *  If both are designated, the value designated in jarAddr is used. Similiar
 *  as jadAddr and jadRead except the jadAddr and jadRead could both be AJ_NULL.
 *
 * The pointer or address indicated by the following parameters
 * must remain valid until <i>callback</i> completes notification or this
 * function returns #AJMS_ERR_INVALID_PARAMS.
 * - <i>jadAddr</i> or <i>jadReader</i>
 * - <i>jarAddr</i> or <i>jarReader</i>
 * - <i>result</i>
 * - <i>callback</i>
 * .
 *
 * The number of MIDlets in one MIDlet suite could not more than 255. If the
 * number of MIDlets is more than 255, the MIDlet with index greater than 255
 * will be ignored.
 *
 * @see AJMS_InstallCB
 */
AJ_S32 ajms_ota_install( const AJ_S32                      categoryId,
                         const AJ_S32                      jadSize,
                         const AJ_U8                      *jadAddr,
                         AJMS_Reader                       jadRead,
                         const AJ_U8                      *jadUrl,
                         const AJ_S32                      jarSize,
                         const AJ_U8                      *jarAddr,
                         AJMS_Reader                       jarRead,
                         const AJ_S32                      flag,
                         const AJ_S32                      oldId,
                         AJMS_InstallResult               *result,
                         AJMS_InstallCB                    callback);
//Ajsc #ifdef SUPPORT_OTA_INSTALLATION_WITH_JAD_JAR_LOCATION
AJ_S32 ajms_ota_install_to( const AJ_S32                      categoryId,
	                        int                               installtype,
                            const AJ_U8                      *jadinstallPath,
                            const AJ_U8                      *jarinstallPath,
                            const AJ_S32                      jadSize,
                            const AJ_U8                      *jadAddr,
                            AJMS_Reader                       jadRead,
                            const AJ_U8                      *jadUrl,
                            const AJ_S32                      jarSize,
                            const AJ_U8                      *jarAddr,
                            AJMS_Reader                       jarRead,
                            const AJ_S32                      flag,
                            const AJ_S32                      oldId,
                            AJMS_InstallResult               *result,
                            AJMS_InstallCB                    cb);

/**
 *  \brief  Installs a Java midlet suite by native file path.
 *   @param categoryId 
 *             The category Id. Only has meaning when category is supported.
 *   @param jad
 *             Absolutely file path and name of Jad file. The path must be
 *             decalred in kjava_sys_fs.c and returns by kjava_fs_getPath.
 *             For example, when this path is "./storage/midlet/AplixBench.jad",
 *             the "./storage/midlet/" must be declared in kjava_sys_fs.c and
 *             can be retrieved by AJMS via #kjava_fs_getPath. Otherwise
 *             #AJMS_ERR_NOT_ALLOWED will be returned.
 *
 *   @param jadUrl
 *             The url that where to download the Jad. Only assign this 
 *             parameter when AMS needs to know the upgrade Url from 
 *             AJMS.
 *
 *   @param jar
 *             Absolutely file path and name of Jar file. Please check the
 *             <i>jad</i> parameter for file path rule.
 *
 *   @param flag
 *             Bit mask to set the following flags:
 *     - #AJMS_INSTALL_DEFAULT  - Default setting.
 *     - #AJMS_INSTALL_KEEP_RMS - Force to keep the old application RMS data after upgrade. The
 *                                flag is only meaningful when update is allowed. In other words,
 *                                user MUST use #ajms_ota_check to see get the upgrade
 *                                information first.
 *     - #AJMS_INSTALL_PRECHECK - Do Jar data precheck during installation. It will
 *                                speed up the midlet launch time but spend more time
 *                                during installation. This only has meaning when
 *                                precheck is supported.
 *     - #AJMS_INSTALL_COPY_JAR - Make a copy of Jad into default java folder.
 *     - #AJMS_INSTALL_COPY_JAD - Make a copy of Jar into default java folder.<br>
 *         Do not raise #AJMS_INSTALL_COPY_JAR or #AJMS_INSTALL_COPY_JAD flag if you 
 *         want VM read the Jad and Jar directly from the location that assigned by 
 *         <i>jad</i> and <i>jar</i>.             
 *     - #AJMS_INSTALL_AS_MANUFACTURER - Force application to be under manufacturer domain.
 *     - #AJMS_INSTALL_AS_OPERATOR     - Force application to be under operator domain.
 *     - #AJMS_INSTALL_AS_THIRD_PARTY  - Force application to be under trusted third party domain.
 *     - #AJMS_INSTALL_AS_UNSPECIFIED  - Force application to be under untrusted domain.<br>
 *                                 Without doing real midlet suite integrity checking,
 *                                 raise one of these four flags directly assign the 
 *                                 domain to the installed application. Use these flags
 *                                 only when the domain is 
 *                                 determined before installation. For example,
 *                                 when application integrity checking is done by Wap 
 *                                 browser during Jar download.
 *
 *    @param oldId
 *        Pass the <i>OldId</i> field in #AJMS_CheckResult returned by #ajms_ota_check.
 *
 *    @param result 
 *        A pointer to the structure for receiving installation result.
 *
 *    @param callback
 *        The address of the progress/result notification callback function.
 *
 *    @return
 *        - #AJMS_ERR_NONE: The install request has been sent normally. 
 *        - #AJMS_ERR_INVALID_PARAMS: Some parameter is invalid.
 *        - #AJMS_ERR_NOT_ALLOWED: Not allow to access the directory of <i>jad</i>
 *                                 or <i>jar</i>.
 *
 * Use this function to install a Java application into AJMS repository.
 * This function does not perform Jad/Jar checking. It is device manufacturer's
 * responsibility to verify jad/manifest by invoking #ajms_ota_check before this function.
 *
 * The installation is performed asynchronously in the KVE thread.
 * The callback function <i>callback</i> is called several times in the KVE
 * thread context to indicate the verification progress or result.
 * See #AJMS_InstallCB for details.
 *
 * The pointer of <i>callback</i> and <i>result</i> 
 * must remain valid until <i>callback</i> completes notification or this
 * function returns #AJMS_ERR_INVALID_PARAMS.
 *
 * The number of MIDlets in one MIDlet suite could not be more than 255. If the
 * number of MIDlets is more than 255, the MIDlet with index greater than 255
 * will be ignored.
 *
 * @see AJMS_InstallCB
 */
AJ_S32 ajms_ota_installEx( const AJ_S32        categoryId,
                           const AJ_U8*        jad,
                           const AJ_U8*        jadUrl,
                           const AJ_U8*        jar,
                           const AJ_S32        flag,
                           const AJ_S32        oldId,
                           AJMS_InstallResult *result,
                           AJMS_InstallCB      callback );

/** @} */

/** 
 * @addtogroup Application_Deletion_ACI Midlet Suite Deletion ACI
 * @{
 */
/**
 * \brief Deletes an installed application in the AJMS repository.
 * @param suiteId
 *        The identifier of the midlet suite to be deleted.
 *        See \ref ams_application_browsing_aci for details of obtaining midlet suite identifier.
 * @return
 *     - #AJMS_ERR_NONE: The application has been deleted normally.
 *     - #AJMS_ERR_ID_NOT_EXIST: The <i>suiteId</i> is invalid.
 */
AJ_S32 ajms_ota_delete( const AJ_S32 suiteId );

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AJMS_OTA_HEADER */
