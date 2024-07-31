/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */
/** 
 * @file ajms_ab.h
 * Application Browse ACI
 */

#ifndef AJMS_AB_HEADER
#define AJMS_AB_HEADER

#ifdef __cplusplus
extern "C"{
#endif

#include "aj_type.h"
#include "ajms_common.h"

/** @addtogroup ams_application_browsing_aci 
 *  @{ 
 */

/*  
 *  Interfaces listed in the group are used for application browsing, 
 *  including these functionalities:
 *      - list/manage categories.
 *      - list/manage midlet suites in a specified category.
 *      - list midlets in a specified midlet suite.
 *      - list midlet suites' Jad and manifest content.
 *      - Parse one Jad and manifest content to get certain property.
 *      - Retrieve midlet suite identifier by name and vendor.
 *      - Retrieve midlet and midlet suite icon data.
 *      .
 *
 */
/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Suite_Raw_ACI Suite's Raw Data ACI
 * @{
 */
/** \brief The flag indicates the descriptor is JAD content. */
#define AJMS_DESC_TYPE_JAD        (1)
/** \brief The flag indicates the descriptor is MANIFEST.MF content. */
#define AJMS_DESC_TYPE_MANIFEST   (2)
/** @} */

/** 
 * @addtogroup Category_Browsing_ACI Category Management ACI
 * @{
 */
 
/** 
 * \brief The identifier of the default unfiled category 
 */
#define AJMS_DEFAULT_CATEGORY_ID_UNFILED          (0)
/** 
 * \brief The identifier of the default pre-installed category 
 */
#define AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED     (1)


/**
 * \brief The structure type for containing a category information.
 */
typedef struct _AJMS_Category
{
    /** 
     * \brief The identifier of the category. 
     */
    AJ_S32  id;
    
    /** 
     * \brief The category name, which is a UTF8 encoded and null-terminated string. 
     */
    AJ_U8   name[AJMS_MAX_ENTRY_NAME_LENGTH+1];
    
    /** 
     * \brief A flag indicating whether or not the category is default (immutable).
     */
    AJ_BOOL isDefault;
}
AJMS_Category;

/** @} */

/** 
 * @addtogroup Suite_Browsing_ACI Suite Browsing ACI
 * @{
 */

/** 
 * \brief Sort type: By the name of MIDlet suite.
 */
#define AJMS_SORT_BY_NAME                (1)
/** 
 * \brief Sort type: By the vendor name of MIDlet suite.
 */
#define AJMS_SORT_BY_VENDOR_NAME         (2)
/** 
 * \brief Sort type: By the name of the category that MIDlet suite belongs to.
 */
#define AJMS_SORT_BY_CATEGORY            (3)
/** 
 * \brief Sort type: Application type.
 */
#define AJMS_SORT_BY_TYPE                (4)
/** 
 * \brief Sort type: By the size of MIDlet-suite.
 */
#define AJMS_SORT_BY_SIZE                (6)
/** 
 * \brief Sort type: By the order of installation time of MIDlet-suite. 
 */
#define AJMS_SORT_BY_INSTALL_TIME        (7)

/**
 * \brief The icon ID for midlet suite, used by #ajms_ab_getIcon.
 */
#define AJMS_SUITE_ICON_ID               (0xFF)

/** \brief Application Type 1 */
#define AJMS_APPLICATION_TYPE_1          (0x01)

/** \brief Application Type 2 */
#define AJMS_APPLICATION_TYPE_2          (0x02)

/** \brief Application Type 3 */
#define AJMS_APPLICATION_TYPE_3          (0x03)

/**
 * \brief The structure type for containing the information of a MIDlet suite.
 */ 
typedef struct _AJMS_Suite
{
    /** 
     * \brief The identitifier of the MIDlet suite.
     */
    AJ_S32  id;
    
    /** 
     * \brief The name of the MIDlet suite, which is a UTF8 encoded and null-terminated string.
     */
    AJ_U8   aliasName[AJMS_MAX_ENTRY_NAME_LENGTH + 1];

    /** 
     * \brief The name of the MIDlet suite, which is a UTF8 encoded and null-terminated string.
     */
    AJ_U8   suiteName[AJMS_MAX_ENTRY_NAME_LENGTH + 1];
    
    /** 
     * \brief The vendor name of the MIDlet suite, which is a UTF8 encoded and null-terminated string.
     */
    AJ_U8   vendor[AJMS_MAX_ENTRY_NAME_LENGTH + 1];

    /** \brief The version value in major field. */
    AJ_S32  majorVersion;
    /** \brief The version value in minor field. */
    AJ_S32  minorVersion;
    /** \brief The version value in micro field. */
    AJ_S32  microVersion;
    
    /** 
     * \brief The identifier of protected domain ID.
     * The identitifier of the security domain to which the MIDlet suite belongs.
     * Must be one of #AJMS_PCS_DOMAIN_UNSPECIFIED, #AJMS_PCS_DOMAIN_MANUFACTURER,
     * #AJMS_PCS_DOMAIN_OPERATOR or #AJMS_PCS_DOMAIN_THIRD_PARTY that defined in
     * ajms_pcs.h.
     */
    AJ_S32  domainId;

    /** 
     * \brief The identifier of the category to which the MIDlet suite belongs.
     */
    AJ_U8  categoryId;
    
    /** 
     *  \brief A flag indicating the type of the installed application.
     *         
     *         - #AJMS_APPLICATION_TYPE_1
     *         - #AJMS_APPLICATION_TYPE_2
     *         - #AJMS_APPLICATION_TYPE_3
     *         .
     */
    AJ_S32 type;
    
    /** 
     * \brief The total size of the MIDlet suite. 
     */
    AJ_U32  size;

    /** 
     * \brief The number of midlets in the MIDlet suite.
     */
    AJ_U8  numOfMidlet;

    /**
     * \brief The relative order of the installation time
     */
    AJ_S16  order;
}
AJMS_Suite;
/** @} */

/** 
 * @addtogroup Midlet_Browsing_ACI Midlet Browsing ACI
 * @{
 */

/** 
 * \brief The data structure type for containing the attributes of a MIDlet.
 */
typedef struct _AJMS_Midlet
{
    /** 
     * \brief The Name field of the 'MIDlet-?' attribute.
     */
    AJ_U8 name[AJMS_MAX_ENTRY_NAME_LENGTH + 1];
    
    /** 
     * \brief The Icon field of the 'MIDlet-?' attribute.
     */
    AJ_U8 icon[AJMS_MAX_ENTRY_NAME_LENGTH + 1];
    
    /** 
     * \brief The Class field of the 'MIDlet-?' attribute.
     */
    AJ_U8 mainClass[AJMS_MAX_ENTRY_CLASS_LENGTH + 1]; 
}
AJMS_Midlet;

/** @} */

/*-----------------------------------------------------------------------------
 * Global Function Prototype
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Category_Browsing_ACI
 * @{
 */

/**
 * \brief Gets the number of categories stored in the AJMS Java application repository.
 *
 * @return The number of categories in AJMS.
 *
 * Call this function to get the number of categories stored in AJMS. 
 * 
 * If supports category, there are always 2 default categories:
 *  - unfiled       ( #AJMS_DEFAULT_CATEGORY_ID_UNFILED )
 *  - pre-installed ( #AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED )
 *  .
 * The return value number would always be 2 or more. 
 *
 * If not support category, there is always one and only one dummy category named "none".
 * So the return valye number is always be 1.
 */
AJ_S32 ajms_ab_getCategoryListCount( void );

/**
 * \brief Gets the categories stored in AJMS.
 * @param categoryArray
 *        A pointer to the #AJMS_Category array to receive categories stored 
 *        in the AJMS Java application repository.
 *        The number of elements in the array should be same as the return value
 *        of #ajms_ab_getCategoryListCount.
 * @return The number of categories that put in the <i>categoryArray</i>.
 *
 * If not support category, there is always one and only one dummy category named "none".
 */
AJ_S32 ajms_ab_getCategoryList( AJMS_Category *categoryArray );

/**
 * \brief Gets the identifier of a category by a given category name.
 * @param name
 *        A pointer to the category name string. 
 *        It must be UTF8 encoded and NUL-terminated.
 * @return 
 *         - 0 or positive value: Category ID.
 *         - #AJMS_ERR_ID_NOT_EXIST
 *         .
 */
AJ_S32 ajms_ab_getCategoryId( const AJ_U8* name );

/**
 * \brief Adds a category to the AJMS Java application repository.
 * @param name
 *        A pointer to the category name string. It must be UTF8 encoded
 *        and NUL-terminated.
 * @return 
 *     - 0 or positive value: Category ID.
 *     - #AJMS_ERR_ENTRY_ALREADY_EXIST: Category already exists.
 *     - #AJMS_ERR_FILE_FULL: Total categories is 20 already.
 *     - #AJMS_ERR_NOT_SUPPORT: Not support category.
 *     .
 */
AJ_S32 ajms_ab_addCategory( const AJ_U8* name );

/**
 * \brief Renames an existing category in the AJMS Java application repository.
 * @param categoryId
 *        The identifier of the category to be renamed.
 * @param name
 *        A pointer to the string representing the new name for the category. 
 *        It should be UTF8 encoded and NUL-terminated.
 * @return 
 *      - #AJMS_ERR_NONE
 *      - #AJMS_ERR_ENTRY_ALREADY_EXIST: The <i>name</i> already exists.
 *      - #AJMS_ERR_ID_NOT_EXIST: The <i>categoryId</i> does not exist.
 *      - #AJMS_ERR_NOT_SUPPORT: Not support category.
 *      - #AJMS_ERR_INVALID_PARAMS: The <i>categoryId</i> is not changeable.
 *      .
 */
AJ_S32 ajms_ab_renameCategory( const AJ_S32 categoryId, 
                               const AJ_U8* name );

/**
 *  \brief Deletes an existing category in the AJMS Java application repository.
 * @param categoryId
 *        The identifier of the category to be deleted.
 * @return 
 *      - #AJMS_ERR_NONE
 *      - #AJMS_ERR_INVALID_PARAMS: The <i>categoryId</i> is not valid.
 *      - #AJMS_ERR_ID_NOT_EXIST: The <i>categoryId</i> does not exist.
 *      - #AJMS_ERR_NOT_SUPPORT: Not support category.
 *      .
 *
 * When a category is deleted, all the applications that belong to the deleted category 
 * are bound to the #AJMS_DEFAULT_CATEGORY_ID_UNFILED category.
 */
AJ_S32 ajms_ab_deleteCategory( const AJ_S32 categoryId );

/** @} */

/** 
 * @addtogroup Suite_Browsing_ACI
 * @{
 */

/**
 * \brief Gets the number of applications stored in the AJMS Java application repository.
 * @param categoryId
 *        The category identifier. 
 *        -1 can be designated in this parameter to get the number of all 
 *        applications rather than applications in a specific category.
 *        If not support category, this parameter will be ignored.
 * @return 
 *        - 0 or positive: Number of suites in AJMS.
 *        - #AJMS_ERR_ID_NOT_EXIST
 *        .
 */
AJ_S32 ajms_ab_getSutieListCount( const AJ_S32 categoryId );

/**
 * \brief Get the information of the applications stored in the AJMS Java application repository.
 * @param categoryId
 *        The identifier of the category of which application information is to be obtained.
 *        -1 can be designated in this parameter to get the information of all 
 *        applications rather than applications in a specific category.
 *        If not support category, this parameter will be ignored.
 * @param suiteList
 *        A pointer to the #AJMS_Suite array to receive application information.
 *        The number of elements in the array should be the return value
 *        of #ajms_ab_getSutieListCount.
 * @param sortBy
 *        The order of fields for sorting. 
 *        The one of the following sort types can be designated.
 *        - Java application name (#AJMS_SORT_BY_NAME)
 *        - Java application type (#AJMS_SORT_BY_TYPE)
 *        - Vendor (#AJMS_SORT_BY_VENDOR_NAME)
 *        - Category (#AJMS_SORT_BY_CATEGORY)
 *        - Size (#AJMS_SORT_BY_SIZE)
 *        - Install time (#AJMS_SORT_BY_INSTALL_TIME)
 *        .
 *        If sorting is not required, designate 0 in this parameter.
 * @return 
 *      - 0 or positive value: Number of suites read into <i>suiteList</i>.
 *      - #AJMS_ERR_ID_NOT_EXIST
 *      .
 */
AJ_S32 ajms_ab_getSuiteList( const AJ_S32           categoryId, 
                                   AJMS_Suite*      suiteList, 
                             const AJ_S32           sortBy );

/**
 * \brief Change the suite alias name.
 *
 * @param suiteId
 *        The identifier of the midlet suite to be renamed.
 * @param aliasName
 *        A pointer to the string representing the new name for the suite. 
 *        It should be UTF8 encoded and NUL-terminated.
 *
 * @return 
 *     - #AJMS_ERR_NONE
 *     - #AJMS_ERR_ID_NOT_EXIST
 *     .
 */
AJ_S32 ajms_ab_setSuiteName( const AJ_S32  suiteId, const AJ_U8* aliasName );

/** @} */

/** 
 * @addtogroup Midlet_Browsing_ACI
 * @{
 */

/**
 * \brief Gets the number of midlets of an installed midlet suite.
 * @param suiteId
 *        The midlet suite identifier. 
 * @return 
 *      - positive - Number of midlets.
 *      - #AJMS_ERR_IO_ERROR
 *      - #AJMS_ERR_ID_NOT_EXIST
 *      .
 */
AJ_S32 ajms_ab_getMidletListCount( const AJ_S32 suiteId );

/**
 * \brief Gets all MIDlet infomation of an installed application in the AJMS Java application repository.
 * @param suiteId
 *        The identifier of the application of which information is to be obtained. 
 * @param midletArray
 *        A pointer to the #AJMS_Midlet structure array for receiving all
 *        the midlets' information.
 *
 * @return 
 *      - #AJMS_ERR_NONE
 *      - #AJMS_ERR_IO_ERROR
 *      - #AJMS_ERR_ID_NOT_EXIST
 *      .
 *
 * It is caller's responsibility to allocate enough size in <i>midletArray</i> to store
 * all the midlet information. Caller should use #ajms_ab_getMidletListCount
 * to get number of midlet in the midlet suite.
 */
AJ_S32 ajms_ab_getMidletList( const AJ_S32        suiteId, 
                                    AJMS_Midlet*  midletArray );

/** @} */

/** 
 * @addtogroup Suite_Browsing_ACI
 * @{
 */

/**
 * \brief Gets the identifier of the specified application with given suite name and vendor name.
 * @param suiteName
 *        A pointer to the UTF-8 string indicating the application suite name.
 * @param vendorName
 *        A pointer to the UTF-8 string indicating the application vendor name.
 * @return 
 *        - A positive value: The identifier of the found application.
 *        - #AJMS_ERR_INVALID_PARAMS
 *        - #AJMS_ERR_ID_NOT_EXIST
 *        .
 *
 * The function is used to get the unique identifier of the specified
 * application. According to the MIDP2.0 spec, the vendor name plus suite name
 * should be unique for each different midlet suite.
 */
AJ_S32 ajms_ab_getSuiteIdByNameVendor( const AJ_U8* suiteName, 
                                       const AJ_U8* vendorName );

/** @} */

/** 
 * @addtogroup Category_Browsing_ACI
 * @{
 */

/** 
 * \brief Binds an application with a category.
 * @param suiteId
 *        The identifier of the application.
 * @param categoryId
 *        The identifier of the category with which the application is to be bound.
 * @return 
 *       - #AJMS_ERR_NONE
 *       - #AJMS_ERR_ID_NOT_EXIST
 *       - #AJMS_ERR_NOT_SUPPORT: Not support category.
 *       - #AJMS_ERR_NOT_ALLOWED
 *       .
 *
 * Note that the applications that belong to the 
 * #AJMS_DEFAULT_CATEGORY_ID_PREINSTALLED category cannot be 
 * bound with another category.
 * In those cases this function returns #AJMS_ERR_NOT_ALLOWED. 
 */
AJ_S32 ajms_ab_bindSuiteToCategory( const AJ_S32 suiteId, 
                                    const AJ_S32 categoryId );

/** @} */

/** 
 * @addtogroup Suite_Raw_ACI
 * @{
 */

/** 
 * \brief Gets icon data by application ID.
 * @param suiteId
 *        The identifier of the application.
 * @param iconId
 *        The identifier of the icon. The icon id for MIDlet-1 to MIDlet-N is 0 to N-1. The icon id
 *        for midlet suite is #AJMS_SUITE_ICON_ID.
 * @param buffer
 *        A pointer to the buffer for receiving the icon content. 
 *        This parameter can be AJ_NULL. 
 *        In this case the data is not passed to the caller but the size of the icon data is returned.
 * @param buflen
 *        The length in bytes of the buffer indicated by <i>buffer</i>.
 * @return 
 *        - A positive value: The size of the icon data.
 *        - 0: The icon does not exist.
 *        - #AJMS_ERR_ID_NOT_EXIST: The <i>suiteId</i> is invalid.
 *        - #AJMS_ERR_IO_ERROR: An I/O occurred when opening or reading icon data.
 *        .
 */
AJ_S32 ajms_ab_getIcon( const  AJ_S32 suiteId,
                        const  AJ_S32 iconId,
                               AJ_U8* buffer,
                        const  AJ_S32 buflen );

/** 
 * \brief Gets complete JAD content by application ID.
 * @param suiteId
 *        The identifier of the application.
 * @param buffer
 *        A pointer to the buffer for receiving the Jad content. 
 *        This parameter can be AJ_NULL. 
 *        In this case the data is not passed to the caller but the size of the Jad data is returned.
 * @param buflen
 *        The length in bytes of the buffer indicated by <i>buffer</i>.
 * @return 
 *        - A positive value: The size of the JAD data.
 *        - 0: Jad file does not exist.
 *        - #AJMS_ERR_ID_NOT_EXIST: The <i>suiteId</i> is invalid.
 *        - #AJMS_ERR_IO_ERROR: An I/O occurred when opening or reading JAD.
 *        .
 *
 * Use this function to get the installed application's JAD content. 
 */
AJ_S32 ajms_ab_getJadFileContent( const AJ_S32 suiteId, 
                                        AJ_U8* buffer, 
                                  const AJ_S32 buflen );

/** 
 * \brief Gets complete MANIFEST.MF content in Jar file by application ID.
 * @param suiteId
 *        The identifier of the application.
 * @param buffer
 *        A pointer to the buffer for receiving the Jad content. 
 *        This parameter can be AJ_NULL. 
 *        In this case the data is not passed to the caller but the size of the MANIFEST.MF
 *        data is returned.
 * @param buflen
 *        The length in bytes of the buffer indicated by <i>buffer</i>.
 * @return 
 *        - A positive value: The size of the MANIFEST.MF data.
 *        - #AJMS_ERR_ID_NOT_EXIST: The <i>suiteId</i> is invalid.
 *        - #AJMS_ERR_IO_ERROR: An I/O occurred when opening or reading MANIFEST.MF.
 *        .
 *
 * Use this function to get the installed application's MANIFEST.MF content. 
 */
AJ_S32 ajms_ab_getManifestFileContent( const AJ_S32 suiteId, 
                                       AJ_U8* buffer, 
                                       const AJ_S32 buflen );

/** 
 * \brief Parses a complete JAD/MANIFEST.MF data to get a descripter key value.
 * @param desc
 *        A pointer to the buffer holding JAD/MANIFEST.MF content.
 * @param descLen
 *        The length in bytes of the data indicated by <i>desc</i>.
 * @param descType
 *        - #AJMS_DESC_TYPE_JAD Indicates <i>desc</i> is a JAD content.
 *        - #AJMS_DESC_TYPE_MANIFEST Indicates <i>desc</i> is a MANIFEST.MF content.
 *        .
 * @param key
 *        A pointer to the descriptor key in a NUL-terminated UTF-8 string.
 * @param propertyBuf
 *        A pointer to the buffer for receiving the key value as a NUL-terminated UTF-8 string.
 *        This parameter can be AJ_NULL. 
 *        In this case the value is not passed but the necessary buffer size is returned.
 * @param propertyBuflen
 *        The length in bytes of the buffer indicated by <i>propertyBuf</i>.
 * @return 
 *        - A positive value: <br>
 *          - When <i>propertyBuf</i> is not AJ_NULL,
 *            the number of characters in the descripter value string stored in <i>propertyBuf</i> is returned.
 *          - When <i>propertyBuf</i> is AJ_NULL,
 *            the necessary buffer size in bytes for the descripter value is returned.
 *          .
 *        - #AJMS_ERR_APPKEY_NOT_FOUND <br>
 *           if the designated key is not found.
 *        - #AJMS_ERR_OUT_OF_RANGE <br>
 *           if the size of the <i>propertyBuf</i> is not enough.
 *        .
 *
 * Use this function to get an JAD/MANIFEST.MF descriptor key value of an JAD/MANIFEST.MF content.
 * It is caller's responsibility to make sure the JAD/MANIFEST.MF is fullfilled the
 * spec defined in MIDP2.0 and J2SE. Passing invalid JAD/MANIFEST.MF inside this function
 * will cause unpredictable result. 
 */
AJ_S32 ajms_ab_getAppDescProperty( const AJ_U8*  desc,
                                   const AJ_S32  descLen,
                                   const AJ_S32  descType,
                                   const AJ_U8*  key,
                                         AJ_U8*  propertyBuf,
                                   const AJ_S32  propertyBuflen );


/**
 *  \brief Get Jad url for an installed midlet suite.
 *  @param suiteId
 *        The identifier of the application.
 *  @param errorCode
 *         A pointer to the memory area for receiving the error code that
 *         indicates the result of the function processing.<br>
 *         One of the following values is put in this memory area.<br>
 *         - #AJMS_ERR_NONE: Succeeded.
 *         - #AJMS_ERR_ID_NOT_EXIST: An illegal ID was designated.
 *         .
 *
 * @return 
 *        - AJ_NULL: Error occurred.
 *        - UTF-8 format string that terminated with null character.
 *        .
 *
 * Jad Url is given during midlet suite installation time. The Url is used
 * by AMS to get the upgrade information for a midlet suite. The return 
 * UTF-8 string is only guarantee available until next ACI call.
 */
const AJ_U8* ajms_ab_getJadUrl( const AJ_S32 suiteId, AJ_S32 *errorCode);

/**
 *  \brief Get Jad file path for an installed midlet suite.
 *  @param suiteId
 *        The identifier of the application.
 *  @param errorCode
 *         A pointer to the memory area for receiving the error code that
 *         indicates the result of the function processing.<br>
 *         One of the following values is put in this memory area.<br>
 *         - #AJMS_ERR_NONE: Succeeded.
 *         - #AJMS_ERR_ID_NOT_EXIST: An illegal ID was designated.
 *         - #AJMS_ERR_FILE_NOT_EXIST: Jad is in romzied virtual directory or
 *                                     the authenticated root is not available.
 *         .
 *
 * @return 
 *        - AJ_NULL: Error occurred.
 *        - UTF-8 format string that terminated with null character.
 *        .
 */
const AJ_U8* ajms_ab_getJadPath( const AJ_S32 suiteId, AJ_S32 *errorCode );

/**
 *  \brief Get Jad file path for an installed midlet suite.
 *  @param suiteId
 *        The identifier of the application.
 *  @param errorCode
 *         A pointer to the memory area for receiving the error code that
 *         indicates the result of the function processing.<br>
 *         One of the following values is put in this memory area.<br>
 *         - #AJMS_ERR_NONE: Succeeded.
 *         - #AJMS_ERR_ID_NOT_EXIST: An illegal ID was designated.
 *         - #AJMS_ERR_FILE_NOT_EXIST: Jad is in romzied virtual directory or
 *                                     the authenticated root is not available.
 *         .
 *
 * @return 
 *        - AJ_NULL: Error occurred.
 *        - UTF-8 format string that terminated with null character.
 *        .
 */
const AJ_U8* ajms_ab_getJarPath( const AJ_S32 suiteId, AJ_S32 *errorCode );

/** @} */ 

#ifdef __cplusplus
}
#endif 

#endif /* AJMS_AB_HEADER */
