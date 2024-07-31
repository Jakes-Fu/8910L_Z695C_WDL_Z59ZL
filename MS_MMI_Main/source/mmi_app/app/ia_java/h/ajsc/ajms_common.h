/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */
/** 
 * @file ajms_common.h
 * AJMS Common Header
 */

#ifndef AJMS_COMMON_HEADER
#define AJMS_COMMON_HEADER

#ifdef __cplusplus
extern "C"{
#endif

#include "aj_type.h"

/** @addtogroup jblendia_initialization_aci
 *  @{ 
 */

/*
 *    Interfaces listed in the group are used for general interface, 
 *    including these functionalities:
 *
 *    - Initialize/Finalize AJMS.
 *    .
 */
 
/** 
 * @addtogroup Common_Constant Common Constant Types 
 * @{
 */

/** \brief Maximum length for all "name" string. */
#define AJMS_MAX_ENTRY_NAME_LENGTH       (64)
/** \brief Maximum length for all internal file path string. */
#define AJMS_MAX_ENTRY_URI_LENGTH        (64)
/** \brief Maximum length for all java class name string. */
#define AJMS_MAX_ENTRY_CLASS_LENGTH      (256)
/** \brief Maximum length for all url string. */
#define AJMS_MAX_ENTRY_URL_LENGTH        (256)

/** @} */

/** 
 * @addtogroup Common_Error_Code Common Error Code
 * @{
 */
/** \brief No error. */
#define AJMS_ERR_NONE                  (0)
/** \brief File not found. */
#define AJMS_ERR_FILE_NOT_EXIST       (-1)
/** \brief File already exists. */
#define AJMS_ERR_FILE_ALREADY_EXIST   (-2)
/** \brief Invalid parameter. */
#define AJMS_ERR_INVALID_PARAMS       (-3)
/** \brief An IO error occurred. */
#define AJMS_ERR_IO_ERROR             (-4)
/** \brief ID not found. */
#define AJMS_ERR_ID_NOT_EXIST         (-5)
/** \brief Invalid AJMS file found. */
#define AJMS_ERR_INVALID_AJMS_FILE    (-6)
/** \brief File full. */
#define AJMS_ERR_FILE_FULL            (-7)
/** \brief The entry already exists.*/
#define AJMS_ERR_ENTRY_ALREADY_EXIST  (-8)
/** \brief Memory overflow. */
#define AJMS_ERR_MEMORY_OVERFLOW      (-9)
/** \brief Application descriptor not found. */
#define AJMS_ERR_APPKEY_NOT_FOUND     (-10)
/** \brief Invalid format. */
#define AJMS_ERR_INVALID_FORMAT       (-11)
/** \brief Too long character string. */
#define AJMS_ERR_VALUE_TOO_LONG       (-12)
/** \brief Not allowed. */
#define AJMS_ERR_NOT_ALLOWED          (-13)
/** \brief The value is out of range. */
#define AJMS_ERR_OUT_OF_RANGE         (-14)
/** \brief Not supported. */
#define AJMS_ERR_NOT_SUPPORT          (-15)
/** \brief Wrong AJMS version */      
#define AJMS_ERR_WRONG_VERSION        (-16)
/** \brief User canceled. */
#define AJMS_ERR_USER_CANCEL          (-20)
/** \brief Invalid JAR size. */
#define AJMS_ERR_INVALID_JAR_SIZE     (-21)
/** \brief An internal error occurred. */
#define AJMS_ERR_INTERNAL             (-100)

/** @} */

/** 
 * @addtogroup Basic_ACI Basic ACI
 * @{
 */

/**
 * \brief The <i>TRACE</i> level designates the detailed tracing 
 * information, such like enter a function, exit a function, ...
 */
#define AJMS_LOGGING_LEVEL_TRACE          (-1)

/** 
 * \brief The <i>DEBUG</i> level designates fine-grained informational 
 * messages that are most useful to debug the Java execution environment.
 */
#define AJMS_LOGGING_LEVEL_DEBUG          (0)

/** 
 * \brief The <i>INFO</i> level designates informational messages that 
 * highlight the progress of Java execution environment at 
 * coarse-grained level.
 *
 * Note that most logging messsages output from Java execution environment
 * are this type.
 */
#define AJMS_LOGGING_LEVEL_INFO           (1)

/**
 * \brief The <i>WARN</i> level designates potentially harmful situations.
 */
#define AJMS_LOGGING_LEVEL_WARN           (2)

/**
 * \brief The <i>ERROR</i> level designates error events that might 
 * still allow the Java execution environment to continue working.
 */
#define AJMS_LOGGING_LEVEL_ERR            (3)

/** @} */ 


/** \ingroup trace_debug_aci */
/** 
 * @addtogroup trace_debug_aci_main Debug and Log Filter Function
 * @{
 */

/**
 * \brief The function type for the callback asking whether or not a specific 
 *        type of debugging message can be output.
 * @param level
 *        The logging level. Here are the possible values:
 *        - #AJMS_LOGGING_LEVEL_TRACE
 *        - #AJMS_LOGGING_LEVEL_DEBUG
 *        - #AJMS_LOGGING_LEVEL_INFO
 *        - #AJMS_LOGGING_LEVEL_WARN
 *        - #AJMS_LOGGING_LEVEL_ERR
 *        .
 * @param moduleName
 *        The module name of the output logging message
 * @param functionName
 *        The function name of the output logging message
 * @return 
 *        Return 1 to allow output this type of logging message.<br>
 *        Return 0 to deny.
 * 
 * A callback function of this type can be passed to ajms_init(). 
 * The callback function is to be implemented by the device manufacturer. 
 *
 * The callback function is called by AJMS to ask whether or not a logging
 * message from Java execution environment can be output to the native
 * system by using #kjava_sys_log_printOut.
 *
 */
typedef AJ_BOOL (* AJMS_DEBUG_FilterCallback) (
                                    const AJ_S32 level,
                                    AJ_S8* moduleName, 
                                    AJ_S8* functionName);

/** @} */

/** 
 * @addtogroup Common_Reader_Type Common Reader Type
 * @{
 */

/**
 * \brief The type of the stream function to read byte data
 *  @param buffer
 *         A pointer to the buffer to store read data
 *  @param offset
 *         The position to start reading. It is designated as the offset (in bytes) from
 *         the head.
 *  @param size
 *         The size (in bytes) to be read.
 *  @return
 *         If the read operation succeeds, return the number of bytes (0 or more)
 *         actually read.
 *         If it fails, return a negative value.
 *
 *  This is the type of the callback function to read JAD and JAR.
 *
 *  If the size of read data is less than the requested size, the function retries
 *  reading the remaining data.
 *  If the size of read data returned by this function is greater than the requested
 *  size, AJMS treats this result as an error.
 *  If the read data exceeds the buffer size, the behavior of AJMS is not guaranteed.
 */
typedef AJ_S32 (*AJMS_Reader)(void *buffer, AJ_S32 offset,  AJ_S32 size);

/** @} */

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/
 
/**
 * @addtogroup Basic_ACI
 * @{
 */
/**
 * \brief Initializes AJMS.
 *
 *    The AJMS initialization should be done when 
 *    the handset has been turned on. In 
 *    other words, they should be called one and only
 *    once during handset lifecycle.
 
 * @param filter
 *        Native system can implement the callback function for 
 *        the customized logging message filtering. If the parameter
 *        is given as AJ_NULL, all logging messages from Java execution
 *        environment will be output.
 * @return
 *       - #AJMS_ERR_NONE - Success.
 *       - #AJMS_ERR_IO_ERROR - Failed to read AJMS file in file system.
 *       - #AJMS_ERR_FILE_NOT_EXIST - The romzied AJMS file disappeared. Check
 *                                    PIK configuration.
 *       - #AJMS_ERR_WRONG_VERSION - The AJMS version is not consistence. To 
 *                                   check the AJMS version, please refer to
 *                                   #ajms_cfg_getProperty. When this 
 *                                   error code happen, AMS should remove all
 *                                   the file inside default Java folder and
 *                                   invoke #ajms_init again to create new
 *                                   AJMS file codebase.
 *       - #AJMS_ERR_INVALID_FORMAT - The AJMS file layout format is invalid.
 *                                   The storage system might have problem.
 *       - #AJMS_ERR_INTERNAL - Others.
 */
AJ_S32 ajms_init(AJMS_DEBUG_FilterCallback filter);

/**
 * \brief Finalizes AJMS
 *
 *    The AJMS finalization should be done when 
 *    the handset has been turned off. In 
 *    other words, they should be called one and only
 *    once during handset lifecycle.
 *
 * @return
 *       - #AJMS_ERR_NONE - Succeeded.
 *       - #AJMS_ERR_IO_ERROR - Failed to read AJMS file in file system.
 *       - #AJMS_ERR_FILE_NOT_EXIST - The romzied AJMS file disappeared.
 *       - #AJMS_ERR_INTERNAL - Others.
 */
AJ_S32 ajms_fini(void);
/**
 * \brief get jblenda lib version
 *
 * @return
 */
AJ_U8*  ajsc_getJBlendVersion(void);
/** @} */

#ifdef __cplusplus
}
#endif 

#endif
