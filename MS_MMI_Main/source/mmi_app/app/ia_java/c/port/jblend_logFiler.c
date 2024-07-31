/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/*-----------------------------------------------------------------------------
* Include Files
*----------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
//#include "os_api.h"
#include "mmk_type.h"
#include "ajms_common.h"


/*-----------------------------------------------------------------------------
* Marcos, Types, and Structures
*----------------------------------------------------------------------------*/

#define USE_EXCLUSIVE_LIST      1



/*-----------------------------------------------------------------------------
* Local variables
*----------------------------------------------------------------------------*/

#if USE_EXCLUSIVE_LIST

static const char exclusiveFunctions[][60] = 
{
    /* END */
    "PS_TimerCb",
        ""
};

static const  char exclusiveModules[][60] = 
{
   // "kjava_sys_zlib.c",
        //  "jk_core_log.c",
        /* Life-cycle management module */
        "kjava_ve.c",
        
        /* END */
        "" 
};
#endif /* USE_EXCLUSIVE_LIST */


/*-----------------------------------------------------------------------------
* External Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Local Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global functions
*----------------------------------------------------------------------------*/

AJ_BOOL jblendia_logFilter(const int level, const char *moduleName, const char *functionName)
{
    
#if USE_EXCLUSIVE_LIST
    int i;
    int moduleCount = sizeof(exclusiveModules) / 60;
    int functionCount = sizeof(exclusiveFunctions) / 60;
#endif
    
    /* Only print standard log instead of some trace */
    if ( level == AJMS_LOGGING_LEVEL_ERR )
    {
        return 1;
    }
    
    
#if USE_EXCLUSIVE_LIST
    
    for (i = 0 ; i < moduleCount - 1 ; i++)
    {
        if (strcmp(moduleName, exclusiveModules[i]) == 0)
        {
            return 0;
        }
    }
    
    for (i = 0 ; i < functionCount - 1 ; i++)
    {
        if (strcmp(functionName, exclusiveFunctions[i]) == 0)
        {
            return 0;
        }
    }
    
#endif
    
    return 1;
}
#endif

/*-----------------------------------------------------------------------------
* Local functions
*----------------------------------------------------------------------------*/
