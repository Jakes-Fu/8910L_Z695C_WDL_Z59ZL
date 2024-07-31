/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

/*Merlin Chang*/

#ifndef JBLEND_LOGFILTER_HEADER_
#define JBLEND_LOGFILTER_HEADER_

/**
 * @.h
 *
 */

#ifdef __cplusplus
extern "C"{
#endif



#include "aj_type.h"

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/
#define   MODULE_FILTER_LIST    ("D:\\@Java\\moduleFilter.lst")
/*#define   MODULE_FILTER_LIST    ("moduleFilter.lst")*/

#define NAME_MAX_LEN    (59)

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

AJ_BOOL jblendia_logFilter(const int level, const char *moduleName, const char *functionName);




#ifdef __cplusplus
}
#endif

#endif /* JBLEND_LOGFILTER_HEADER_ */

