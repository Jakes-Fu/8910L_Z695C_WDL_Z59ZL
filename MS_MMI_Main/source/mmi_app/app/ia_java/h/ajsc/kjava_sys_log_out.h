/**
 * @file kjava_sys_log_out.h
 */


#ifndef KJAVA_SYS_LOG_OUT_HEADER
#define KJAVA_SYS_LOG_OUT_HEADER

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup trace_debug_ksi
 *  @{
 */

/**
 *  @addtogroup Trace_Output_KSI Trace and Debug Message
 *  @{
 */

/** 
 * \brief Outputs a logging message. 
 * @param message
 *        A pointer to a null-terminated message string.
 */
void kjava_sys_log_printOut(char *message);

/** @} */

#ifdef __cplusplus
}
#endif


#endif /* KJAVA_SYS_LOG_OUT_HEADER */
