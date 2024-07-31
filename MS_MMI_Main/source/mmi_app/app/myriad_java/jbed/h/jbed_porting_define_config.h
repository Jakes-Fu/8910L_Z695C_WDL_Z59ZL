/*
This file is used to : define or undef some function-def,and Scope of these defines
is porting files of jbed.
*/


#ifndef __JBED_PORTING_DEFINE_CONFIG_H__
#define __JBED_PORTING_DEFINE_CONFIG_H__


/*-------------- The following defines CANNOT Be Modified By Customer --------*/

/* *
 * Master switch of TRACE info in jbed files,usually defing this,
 * */
//#ifndef PLATFORM_SC7702
#define TRACE_ON
//#endif

/*
*This define is used to switch customer preinstall certs file check feature.
*if customer preinstalled midlets and need relative certs file preinstalled, define this macro
*otherwise undefine this macro as default
*/
//#define JBED_PREINSTALL_CERTS

/*
* This define is used to switch whether we use memory pool for jbed lib C source or not.
*/
//#define USE_JBEDLIB_MEMORY_POOL

/* *
 * one way to create jbed thread. if support JBED_CHUNKED_HEAP function,not use this way.
 * because it may meet stack overflow problem.
 * usually, undef this
 * */
//#define DYNAMIC_CREATE_JBED_THREAD


#endif /*__JBED_PORTING_DEFINE_CONFIG_H__*/