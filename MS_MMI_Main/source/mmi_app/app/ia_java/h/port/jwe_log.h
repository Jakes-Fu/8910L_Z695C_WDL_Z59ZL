
/**
 * @file jwe_log.h
 */


#ifndef JWE_LOG_HEADER
#define JWE_LOG_HEADER

#define _AJSC_DEBUG//huangjc add at 2007.7.5 for java log

/** @addtogroup kjava_sys
 *  @{
 */
/**
 *  @defgroup jwe_log debug output interface
 *
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** \brief  Log level TRACE */
#define JWE_LOG_LEVEL_TRACE                       -1
/** \brief  Log level DEBUG */
#define JWE_LOG_LEVEL_DEBUG                       0
/** \brief  Log level WARN */
#define JWE_LOG_LEVEL_WARN                        1
/** \brief  Log level INFO */
#define JWE_LOG_LEVEL_INFO                        2
/** \brief  Log level ERR */
#define JWE_LOG_LEVEL_ERR                         3

#ifndef MODULE_NAME
/** \brief  module name */
#define MODULE_NAME     __FILE__
#endif

/** \brief General logging macro */
#define JWE_LOG_LEVEL(level,function,parameters)                          \
            if ( jwe_log_check(level, MODULE_NAME, #function ) == 1 )     \
            {                                                                   \
                jwe_log_prefix(MODULE_NAME, #function, __LINE__);         \
                jwe_log_message parameters ;                              \
                jwe_log_postfix();                                        \
            }

/** \brief Log for debugging */
#define JWE_LOG_DEBUG(function,parameters)                                \
            JWE_LOG_LEVEL(JWE_LOG_LEVEL_DEBUG,function,parameters)
#ifdef _AJSC_DEBUG
/** \brief Log for info */
#define JWE_LOG(function,parameters)                                      \
            JWE_LOG_LEVEL(JWE_LOG_LEVEL_INFO,function,parameters)
#else
#define JWE_LOG(function,parameters)   
#endif

/** \brief Log for trace */
#define JWE_LOG_TRACE(function,parameters) \
            JWE_LOG_LEVEL(JWE_LOG_LEVEL_TRACE,function,parameters)

/** \brief Log for warning */
#define JWE_LOG_WARN(function,parameters) \
            JWE_LOG_LEVEL(JWE_LOG_LEVEL_WARN,function,parameters)

/** \brief Log for error */
#define JWE_LOG_ERR(function,parameters) \
            JWE_LOG_LEVEL(JWE_LOG_LEVEL_ERR,function,parameters)

/** \brief Dump the text data in a buffer */
#define JWE_LOG_DUMP_TEXT(buffer,size)                                    \
            jwe_log_dumpText(buffer, size);


/** \brief Check if the message can be outputed */
int jwe_log_check(const int level, const char* moduleName, const char* functionName);

/** \brief Create the prefix part of logging message. */
void jwe_log_prefix(const char* moduleName, const char* functionName, int line);

/** \brief Construct the formatted part of logging message. */
void jwe_log_message(const char* formatString, ...);

/** \brief Construct the postfix part of logging message. */
void jwe_log_postfix(void);

/** \brief Dump the text data in a buffer */
void jwe_log_dumpText(char* buffer, int size);


/** \brief Variables for System.out */
#define ENABLE_KVM_STANDARD_OUTPUT              1

#ifdef __cplusplus
}
#endif

/** }@ */

#endif /* JWE_LOG_HEADER */
