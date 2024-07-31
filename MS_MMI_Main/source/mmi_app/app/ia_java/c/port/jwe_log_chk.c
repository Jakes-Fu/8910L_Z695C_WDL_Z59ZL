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
#include "jwe_log.h"
#include "ajms_common.h"
#include <string.h>
#include "sci_log.h"



/*-----------------------------------------------------------------------------
* Marcos, Types, and Structures
*----------------------------------------------------------------------------*/
#define ASSERT_LOG(_x)  JWE_LOG_ERR(assert,_x)

#ifndef _AJSC_DEBUG
    #undef  JWE_LOG
    #define JWE_LOG(function,parameters)
#endif

#define MSG_ONLY_FOR_CACHE_TCK                  0
#define USE_EXCLUSIVE_LIST                      1


/*-----------------------------------------------------------------------------
* Local variables
*----------------------------------------------------------------------------*/
#if USE_EXCLUSIVE_LIST
static const char exclusiveFunctions[][60] = 
{
    "MMIJAVA_IsInitialize",
    /* END */
    "" 
};

static const char exclusiveModules[][60] = 
{
	"kjava_sys.c",
	"JkSoftkey.c",
	"kjava_sys_core.c",
	"kjava_sys_gfx.c",
	"kjava_sys_gfx_font_intra.c",
	"kjava_dev_fs.c",
	"kjava_sys_fs.c",
	"jas_gfx_font.c",
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

int jwe_log_check(const int level, const char* moduleName, const char* functionName)
{
    
#if USE_EXCLUSIVE_LIST
    int32 i = 0;
    int32 moduleCount = sizeof(exclusiveModules) / 60;
    int32 functionCount = sizeof(exclusiveFunctions) / 60;
#endif
#ifndef WIN32           
    if(!SCI_GetArmLogFlag())
    {        
        return 0;
    }
#endif    

    if (level < 2)
    {
        return 0;
    }
    //return 0; //No debug - comment out to enable JAVA debug
    
#if MSG_ONLY_FOR_CACHE_TCK
    
    if(strcmp(functionName,"STD") == 0){
        return 1;   
    }
    else {
        return 0;
    }
#endif
    
    
#if USE_EXCLUSIVE_LIST
    
    for (i = 0 ; i < moduleCount - 1 ; i++)
    {
	/*    
        if (stricmp(moduleName, exclusiveModules[i]) == 0)//忽略大小写
        {
            return 0;
        }
        */
		
        if (strstr(moduleName, exclusiveModules[i]) != 0)
        {
            return 0;
        }
        
    }
    
    for (i = 0 ; i < functionCount - 1 ; i++)/*lint !e681*/
    {
	/*
        if (stricmp(functionName, exclusiveFunctions[i]) == 0)//忽略大小写
        {
            return 0;
        }
        */
		
        if (strstr(functionName, exclusiveFunctions[i]) != 0)
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
