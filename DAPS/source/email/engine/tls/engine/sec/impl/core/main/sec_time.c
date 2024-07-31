/*==================================================================================================
    HEADER NAME : sec_time.c
    MODULE NAME : SEC
        
    GENERAL DESCRIPTION:
        In this file, define the function for getting time.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                               Tracking
    Date                    Author           Number                 Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07      Kevin Yang                                      create sec_time.c
==================================================================================================*/

/***************************************************************************************************
*   Multi-Include-Prevent Section
***************************************************************************************************/

#include "we_def.h"
#include "we_scl.h"
#include "sec_ast.h"

#define TEN_YEAR_SECONDS 315964800L
/*==================================================================================================
FUNCTION: 
    Sec_TimeGetCurrent
CREATE DATE:
    2006-07-08
AUTHOR:
    Kevin Yang
DESCRIPTION:
    This function return the seconds elapsed since 1970-01-01 00.00.00 in local time. 
ARGUMENTS PASSED:
    None.
RETURN VALUE:
    the time value.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    OMIT.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_UINT32 Sec_TimeGetCurrent(void)
{
    WE_UINT32 uiTmpTime = 0;
#ifdef WE_CFG_BREW_PLATFORM
    uiTmpTime = GETTIMESECONDS();
    uiTmpTime += (WE_UINT32)((10 * 365 + 2 + 5) * 3600 * 24);  
#else    
    /*modify linux by Bird 070313*/
#ifdef WIN32_DEBUG   
    time( (time_t *)&uiTmpTime );
#else
    uiTmpTime = WE_SCL_GETSECONDS();
#endif
#endif
    return uiTmpTime;
}

