/*=====================================================================================

    HEADER NAME : we_url.h
    MODULE NAME : HDR

    PRE-INCLUDE FILES DESCRIPTION

    GENERAL DESCRIPTION
        This file be included by all WE Programmer.In this file some widget  type and  macro is defined.
        These Info is  variational and is changed by switching Project.

    TECHFAITH Wireless Confidential Proprietary
    (c) Copyright 2002 by TECHFAITH Wireless. All Rights Reserved.
=======================================================================================
    Revision History
   
    Modification                 Tracking
    Date           Author         Number        Description of changes
    ----------   ------------    ---------   --------------------------------------------
    2006-5-23   Guohong              create we_url.h

    Self-documenting Code
    Describe/explain low-level design, especially things in header files, of this module and/or group 
    of funtions and/or specific funtion that are hard to understand by reading code and thus requires 
    detail description.
    Free format !

=====================================================================================*/
/*--------------------------START-----------------------------*/



/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef WE_URL_H
#define WE_URL_H

/*******************************************************************************
*   Include File Section
*******************************************************************************/
/*Include System head file*/
/*Include Program Global head file*/
#include "we_cfg.h"
#include "we_def.h"
/*Include Module head file*/
#include "we_scl.h"
/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/

/* Bit flags to select one or more of the components of a URL.*/
#define G_WE_URL_NO_COMP                 0
#define G_WE_URL_SCHEME_COMP             0x1
#define G_WE_URL_USERINFO_COMP           0x2
#define G_WE_URL_HOST_COMP               0x4
#define G_WE_URL_PORT_COMP               0x8
#define G_WE_URL_AUTHORITY_COMP          G_WE_URL_USERINFO_COMP | G_WE_URL_HOST_COMP |\
                                         G_WE_URL_PORT_COMP
#define G_WE_URL_PATH_COMP               0x10
#define G_WE_URL_QUERY_COMP              0x40
#define G_WE_URL_FRAG_COMP               0x80
#define G_WE_URL_ALL_COMP                0xff

/* The different parts of a URL, used to index into the arrays in the St_WeUrl type.*/
#define G_WE_URL_SCHEME_PART             0
#define G_WE_URL_AUTHORITY_PART          1
#define G_WE_URL_PATH_PART               2
#define G_WE_URL_QUERY_PART              3
#define G_WE_URL_FRAGMENT_PART           4
#define G_WE_URL_USERINFO_PART           5
#define G_WE_URL_HOST_PART               6
#define G_WE_URL_PORT_PART               7

#define WE_URL_NUM_PARTS                 8


/*Define Constant Macro end*/


/*Define Macro Function start*/
/*Define Macro Function end*/

/*******************************************************************************
*   Type Define Section
*******************************************************************************/

/*Define base type start*/
/*Define base type end*/ 

/*Define Enum type start*/
/*Define Enum type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
enum  E_WE_SCHEME
{
    E_WE_SCHEME_EMPTY = 0,
    E_WE_SCHEME_HTTP = 1,
    E_WE_SCHEME_HTTPS = 2,
    E_WE_SCHEME_FILE = 3,
    E_WE_SCHEME_WTAI = 4,
    E_WE_SCHEME_ABOUT = 5,
    E_WE_SCHEME_FUNCTION = 6,
    E_WE_SCHEME_MAILTO = 7,
    E_WE_SCHEME_MMSTO = 8,
    E_WE_SCHEME_SMSTO = 9,
    E_WE_SCHEME_TEL = 10,
    E_WE_SCHEME_RTSP = 11,
    E_WE_SCHEME_FS = 12,
	E_WE_SCHEME_DATA = 13,
    E_WE_SCHEME_UNKNOWN = 99
};

typedef enum E_WE_SCHEME E_WEURL_SCHEME;

typedef struct tagSt_WeUrl
{
    E_WEURL_SCHEME   eSchemeType;                       
    WE_CHAR       *apcPartsStr[WE_URL_NUM_PARTS];  
    WE_INT32      aiPartsLen[WE_URL_NUM_PARTS];
}St_WeUrl, * P_St_WeUrl;

/*Define struct &  union type end*/

WE_CHAR* WeUrl_ReplaceBKSlashEx(const WE_CHAR *pcUrl,WE_HANDLE hMem ,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_ReplaceBKSlash
#undef WeUrl_ReplaceBKSlash
#endif
#define WeUrl_ReplaceBKSlash(pcUrl,hMem) WeUrl_ReplaceBKSlashEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_ReplaceBKSlash
#undef WeUrl_ReplaceBKSlash
#endif
#define WeUrl_ReplaceBKSlash(pcUrl) WeUrl_ReplaceBKSlashEx(pcUrl,NULL,FALSE)
#endif


WE_BOOL WeUrl_CheckQueryEx(St_WeUrl *pstUrl,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_CheckQuery
#undef WeUrl_CheckQuery
#endif
#define WeUrl_CheckQuery(pstUrl, hMem) WeUrl_CheckQueryEx(pstUrl, hMem,TRUE)
#else 
#ifdef WeUrl_CheckQuery
#undef WeUrl_CheckQuery
#endif
#define WeUrl_CheckQuery(pstUrl) WeUrl_CheckQueryEx(pstUrl,NULL,FALSE)
#endif

WE_BOOL WeUrl_CheckPathEx(St_WeUrl *pstUrl,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_CheckPath
#undef WeUrl_CheckPath
#endif
#define WeUrl_CheckPath(pstUrl,hMem) WeUrl_CheckPathEx(pstUrl,hMem,TRUE)
#else 
#ifdef WeUrl_CheckPath
#undef WeUrl_CheckPath
#endif
#define WeUrl_CheckPath(pstUrl) WeUrl_CheckPathEx(pstUrl,NULL,FALSE)
#endif

WE_CHAR *WeUrl_AssembleEx(St_WeUrl *pstUrl,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_Assemble
#undef WeUrl_Assemble
#endif
#define WeUrl_Assemble(pstUrl,hMem) WeUrl_AssembleEx(pstUrl, hMem,TRUE)
#else 
#ifdef WeUrl_Assemble
#undef WeUrl_Assemble
#endif
#define WeUrl_Assemble(pstUrl) WeUrl_AssembleEx(pstUrl,NULL,FALSE)
#endif

WE_CHAR *WeUrl_RemoveDotsEx(const WE_CHAR *pcBase,
                          WE_INT32 iBaseLen,
                          const WE_CHAR *pcRel,
                          WE_INT32 iRelLen,
                          WE_HANDLE hMem,
						  WE_BOOL bMemMgr);


#ifdef _USE_MEM_HANDLE
#ifdef	WeUrl_RemoveDots
#undef	WeUrl_RemoveDots
#endif
#define WeUrl_RemoveDots(pcBase, iBaseLen,pcRel, iRelLen, hMem) \
		WeUrl_RemoveDotsEx(pcBase, iBaseLen,pcRel, iRelLen, hMem,TRUE)
#else 
#ifdef	WeUrl_RemoveDots
#undef	WeUrl_RemoveDots
#endif
#define WeUrl_RemoveDots(pcBase, iBaseLen,pcRel, iRelLen) \
		WeUrl_RemoveDotsEx(pcBase, iBaseLen,pcRel, iRelLen,NULL,FALSE)
#endif


WE_BOOL WeUrl_ResolveInternalEx(St_WeUrl *pstBase,
								St_WeUrl *pstRel,
								WE_CHAR **ppAbs,
								WE_HANDLE hMem,
								WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef	WeUrl_ResolveInternal
#undef	WeUrl_ResolveInternal
#endif
#define WeUrl_ResolveInternal(pstBase, pstRel,ppAbs, hMem) \
		WeUrl_ResolveInternalEx(pstBase,pstRel,ppAbs, hMem,TRUE)
#else 
#ifdef	WeUrl_ResolveInternal
#undef	WeUrl_ResolveInternal
#endif
#define WeUrl_ResolveInternal(pstBase,pstRel,ppAbs) \
		WeUrl_ResolveInternalEx(pstBase,pstRel,ppAbs,NULL,FALSE)
#endif


WE_CHAR *WeUrl_GetPartEx(const WE_CHAR *pcUrl,
						 WE_INT32 iWhichPart,
						 WE_HANDLE hMem,
						 WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef	WeUrl_GetPart
#undef	WeUrl_GetPart
#endif
#define WeUrl_GetPart(pcUrl, iWhichPart, hMem) \
		WeUrl_GetPartEx(pcUrl, iWhichPart, hMem,TRUE)
#else 
#ifdef	WeUrl_GetPart
#undef	WeUrl_GetPart
#endif
#define WeUrl_GetPart(pcUrl, iWhichPart) \
		WeUrl_GetPartEx(pcUrl, iWhichPart,NULL,FALSE)
#endif


WE_CHAR *WeUrl_GetPathInternalEx(St_WeUrl *pstUrl,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetPathInternal
#undef WeUrl_GetPathInternal
#endif
#define WeUrl_GetPathInternal(pstUrl, hMem) WeUrl_GetPathInternalEx(pstUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetPathInternal
#undef WeUrl_GetPathInternal
#endif
#define WeUrl_GetPathInternal(pstUrl) WeUrl_GetPathInternalEx(pstUrl,NULL,FALSE)
#endif


WE_CHAR *WeUrl_GetParametersInternalEx(St_WeUrl *pstUrl,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetParametersInternal
#undef WeUrl_GetParametersInternal
#endif
#define WeUrl_GetParametersInternal(pstUrl, hMem) WeUrl_GetParametersInternalEx( pstUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetParametersInternal
#undef WeUrl_GetParametersInternal
#endif
#define WeUrl_GetParametersInternal(pstUrl) WeUrl_GetParametersInternalEx(pstUrl,NULL,FALSE)
#endif


WE_CHAR *WeUrl_LongestCommonPrefixEx(WE_CHAR *pcStr1,
									 WE_CHAR *pcStr2,
									 WE_HANDLE hMem,
									 WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_LongestCommonPrefix
#undef WeUrl_LongestCommonPrefix
#endif
#define WeUrl_LongestCommonPrefix(pcStr1,pcStr2, hMem) \
		WeUrl_LongestCommonPrefixEx(pcStr1,pcStr2, hMem,TRUE)
#else 
#ifdef WeUrl_LongestCommonPrefix
#undef WeUrl_LongestCommonPrefix
#endif
#define WeUrl_LongestCommonPrefix(pcStr1,pcStr2) \
		WeUrl_LongestCommonPrefixEx(pcStr1,pcStr2,NULL,FALSE)
#endif


WE_CHAR *WeUrl_EscapeAllStringEx(const WE_CHAR *pcStr,WE_INT32 iStrLen,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_EscapeAllString
#undef WeUrl_EscapeAllString
#endif
#define WeUrl_EscapeAllString(pcStr, iStrLen, hMem) \
		WeUrl_EscapeAllStringEx(pcStr, iStrLen, hMem,TRUE)
#else 
#ifdef WeUrl_EscapeAllString
#undef WeUrl_EscapeAllString
#endif
#define WeUrl_EscapeAllString(pcStr, iStrLen) \
		WeUrl_EscapeAllStringEx(pcStr, iStrLen,NULL,FALSE)
#endif


/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
    WeUrl_Str2Scheme
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Get the unique integer that indicates the scheme type given.
ARGUMENTS PASSED:
    pcSecheme: The scheme type string
RETURN VALUE:
    Return the scheme type named by the given string.
    Returns scheme_unknown if its not one of the predefined types.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
  
=====================================================================================*/
E_WEURL_SCHEME WeUrl_Str2Scheme (const WE_CHAR *pcScheme);

/*=====================================================================================
FUNCTION: 
    WeUrl_Scheme2Str
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Get the scheme string according to the parameter eScheme
ARGUMENTS PASSED:
    eScheme: The integer that indicates the scheme type
RETURN VALUE:
    The scheme type string
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
const WE_CHAR * WeUrl_Scheme2Str (E_WEURL_SCHEME eScheme);

/*=====================================================================================
FUNCTION: 
    WeUrl_Parse
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    It parses the URL string by devideing different parts such as the scheme part, the host 
    part, the path part and so on.
ARGUMENTS PASSED:
    pcBs: The URL string that to be parsed
    pstUrlp: The address that is used to store the different parts of the URL after parsed.
RETURN VALUE:
    Success: Returns TRUE
    Failed: Returns FALSE
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_Parse (const WE_CHAR *pcBs,
                     St_WeUrl *pstUrlp);

/*=====================================================================================
FUNCTION: 
    WeUrl_Hash
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Calculate the hash value of the URL string given
ARGUMENTS PASSED:
    pcBs: Pointer to the string that want to be calculated its hash value
    puiHashValue: The address that is used to store the hash value of the string given
RETURN VALUE:
    Success: Returns TRUE
    Failed: Returns FALSE
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_Hash (const WE_CHAR *pcBs,
                    WE_UINT32 *puiHashValue);

/*=====================================================================================
FUNCTION: 
    WeUrl_Url2Str
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Given a URL struct, construct and return a string representation of the URL.
ARGUMENTS PASSED:
    pstUrl: The structure which stores the parts of a URL.
RETURN VALUE:
    It returns the whole URL string if success, else it returns NULL.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_Url2StrEx (St_WeUrl *pstUrl,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_Url2Str
#undef WeUrl_Url2Str
#endif
#define WeUrl_Url2Str( pstUrl, hMem) WeUrl_Url2StrEx(pstUrl, hMem,TRUE)
#else 
#ifdef WeUrl_Url2Str
#undef WeUrl_Url2Str
#endif
#define WeUrl_Url2Str(pstUrl) WeUrl_Url2StrEx(pstUrl,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_Resolve
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    It calculates the absolute URL by the base URL and the relative URL address.
ARGUMENTS PASSED:
    pcBase: The base URL address string
    pcRel: The relative URL
    ppcAbs: The address that is used to store the absolute URL
RETURN VALUE:
    Sucess: It returns TRUE
    Failed: It returns FALSE
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_BOOL WeUrl_ResolveEx(const WE_CHAR *pcBase,
					  const WE_CHAR *pcRel,
					  WE_CHAR **ppcAbs,
					  WE_HANDLE hMem, 
					  WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_Resolve
#undef WeUrl_Resolve
#endif
#define WeUrl_Resolve(pcBase,pcRel,ppcAbs, hMem) \
        WeUrl_ResolveEx(pcBase,pcRel,ppcAbs, hMem,TRUE)
#else 
#ifdef WeUrl_Resolve
#undef WeUrl_Resolve
#endif
#define WeUrl_Resolve(pcBase,pcRel,ppcAbs) \
        WeUrl_ResolveEx(pcBase,pcRel,ppcAbs,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_Equal
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Judge whether the two url is equal that the parameter iWhichComponents indicates.
ARGUMENTS PASSED:
    pcBs1: The first URL string
    pcBs2: The second URL string
    iWhichComponents: The bitmap indicating which parts of the URLs should be included 
                                  in the comparison.
RETURN VALUE:
    Return TRUE if the two URLs are equal, FALSE otherwise.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_Equal(const WE_CHAR *pcBs1,
                    const WE_CHAR *pcBs2,
                    WE_INT32 iWhichComponents);

/*=====================================================================================
FUNCTION: 
    WeUrl_IsValid
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Judge whether the URL is valid.
ARGUMENTS PASSED:
    pcURL: The URL string
RETURN VALUE:
    Return TRUE if the given string URL has a valid format, FALSE otherwise.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_IsValidEx (const WE_CHAR *pcUrl, WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_IsValid
#undef WeUrl_IsValid
#endif
#define WeUrl_IsValid(pcUrl, hMem) WeUrl_IsValidEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_IsValid
#undef WeUrl_IsValid
#endif
#define WeUrl_IsValid(pcUrl) WeUrl_IsValidEx(pcUrl,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_GetScheme
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the scheme of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Returns NULL in case of error, including that the URL is not valid, or if the URL 
    does not have a scheme component.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_GetSchemeEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetScheme
#undef WeUrl_GetScheme
#endif
#define WeUrl_GetScheme(pcUrl, hMem) WeUrl_GetSchemeEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetScheme
#undef WeUrl_GetScheme
#endif
#define WeUrl_GetScheme(pcUrl) WeUrl_GetSchemeEx(pcUrl,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_GetSchemeType
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the scheme type of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Returns scheme_empty in case of error, including that the URL is not valid, or if the 
    URL does not have a scheme component.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
E_WEURL_SCHEME WeUrl_GetSchemeType (const WE_CHAR *pcUrl);

/*=====================================================================================
FUNCTION: 
    WeUrl_GetHost
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the host part of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Returns NULL in case of error, including that the URL is not valid, or if the ULR 
    does not have a host part.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_GetHostEx (const WE_CHAR *pcUrl,WE_HANDLE hMem ,  WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetHost
#undef WeUrl_GetHost
#endif
#define WeUrl_GetHost(pcUrl, hMem) WeUrl_GetHostEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetHost
#undef WeUrl_GetHost
#endif
#define WeUrl_GetHost(pcUrl) WeUrl_GetHostEx(pcUrl,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_GetPort
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     Extract the port number of a URL.
ARGUMENTS PASSED:
    
RETURN VALUE:
    Returns NULL in case of error, including that the URL is not valid, or if the URL 
    does not have a port number.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_GetPortEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetPort
#undef WeUrl_GetPort
#endif
#define WeUrl_GetPort(pcUrl, hMem) WeUrl_GetPortEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetPort
#undef WeUrl_GetPort
#endif
#define WeUrl_GetPort(pcUrl) WeUrl_GetPortEx(pcUrl,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_GetHostPort
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the host part and port number of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Returns NULL in case of error, including that the URL is not valid, or if the URL does
    not have a host part.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_GetHostPortEx (const WE_CHAR *pcUrl,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetHostPort
#undef WeUrl_GetHostPort
#endif
#define WeUrl_GetHostPort(pcUrl, hMem) WeUrl_GetHostPortEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetHostPort
#undef WeUrl_GetHostPort
#endif
#define WeUrl_GetHostPort(pcUrl) WeUrl_GetHostPortEx(pcUrl,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_GetPath
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the path component of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Returns NULL in case of error, including that the URL is not valid, or if the URL 
    does not have a path component.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_GetPathEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetPath
#undef WeUrl_GetPath
#endif
#define WeUrl_GetPath(pcUrl, hMem) WeUrl_GetPathEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetPath
#undef WeUrl_GetPath
#endif
#define WeUrl_GetPath(pcUrl) WeUrl_GetPathEx(pcUrl,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_GetParameters
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the parameter component of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Returns NULL in case of error, including that the URL is not valid, or if the URL
    does not have any parameters.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_GetParametersEx (const WE_CHAR *pcUrl,WE_HANDLE hMem ,WE_BOOL bMemMgr );

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetParameters
#undef WeUrl_GetParameters
#endif
#define WeUrl_GetParameters(pcUrl, hMem) WeUrl_GetParametersEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetParameters
#undef WeUrl_GetParameters
#endif
#define WeUrl_GetParameters(pcUrl) WeUrl_GetParametersEx(pcUrl,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_GetQuery
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the query part of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Returns NULL in case of error, including that the URL is not valid, or if the URL does 
    not have a query part.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_GetQueryEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetQuery
#undef WeUrl_GetQuery
#endif
#define WeUrl_GetQuery(pcUrl,hMem) WeUrl_GetQueryEx(pcUrl,hMem,TRUE)
#else 
#ifdef WeUrl_GetQuery
#undef WeUrl_GetQuery
#endif
#define WeUrl_GetQuery(pcUrl) WeUrl_GetQueryEx(pcUrl,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_GetFragment
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the fragment part of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Returns NULL in case of error, including that the URL is not valid, or if the URL 
    does not have a fragment part.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_GetFragmentEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_GetFragment
#undef WeUrl_GetFragment
#endif
#define WeUrl_GetFragment(pcUrl, hMem) WeUrl_GetFragmentEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_GetFragment
#undef WeUrl_GetFragment
#endif
#define WeUrl_GetFragment(pcUrl) WeUrl_GetFragmentEx(pcUrl,NULL,FALSE)
#endif


/*=====================================================================================
FUNCTION: 
    WeUrl_FindPath
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Extract the path part of a URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Return a pointer to where the path part of the URL begins.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It returns a pointer into the original string, i.e., does not allocate any new string.
=====================================================================================*/
const WE_CHAR * WeUrl_FindPath (const WE_CHAR *pcUrl);

/*=====================================================================================
FUNCTION: 
    WeUrl_DropFragment
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     It drop the fragment part of the url given and return the rest parts of the URL.
ARGUMENTS PASSED:
    pcUrl: The URL string
    ppcFragment: The address that is used to store the fragment part of the url
RETURN VALUE:
    Return a copy of 'pcUrl' with the fragment part (if any) removed. Also sets
    '*ppcFragment' to point to a copy of the fragment part of 'pcUrl', or NULL 
    if there is no fragment part.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string, 
    as well as the fragment string.
=====================================================================================*/
WE_CHAR * WeUrl_DropFragmentEx (const WE_CHAR *pcUrl,
                              WE_CHAR **ppcFragment,
                              WE_HANDLE hMem,
							  WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_DropFragment
#undef WeUrl_DropFragment
#endif
#define WeUrl_DropFragment(pcUrl,ppcFragment, hMem) \
		WeUrl_DropFragmentEx(pcUrl,ppcFragment, hMem,TRUE)
#else 
#ifdef WeUrl_DropFragment
#undef WeUrl_DropFragment
#endif
#define WeUrl_DropFragment(pcUrl,ppcFragment) \
		WeUrl_DropFragmentEx(pcUrl,ppcFragment,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_DropQuery
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    It drops the query part of the url given and then returns the rest parts of the url.
ARGUMENTS PASSED:
    pcUrl: The URL string
RETURN VALUE:
    Return a copy of 'pcUrl' with the query part (if any) removed. 
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_DropQueryEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_DropQuery
#undef WeUrl_DropQuery
#endif
#define WeUrl_DropQuery(pcUrl, hMem) \
		WeUrl_DropQueryEx(pcUrl, hMem,TRUE)
#else 
#ifdef WeUrl_DropQuery
#undef WeUrl_DropQuery
#endif
#define WeUrl_DropQuery(pcUrl) \
		WeUrl_DropQueryEx(pcUrl,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_AddFragment
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Add the fragment part to the URL string.
ARGUMENTS PASSED:
    
RETURN VALUE:
    Return a copy of 'pcUrl' with 'pcFragment' added as the fragment part. If 'pcUrl' already
    has a fragment part, it is replaced with 'pcFragment'.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_AddFragmentEx (const WE_CHAR *pcUrl,
							   const WE_CHAR *pcFragment,
							   WE_HANDLE hMem,
							   WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_AddFragment
#undef WeUrl_AddFragment
#endif
#define WeUrl_AddFragment(pcUrl,pcFragment, hMem) \
		WeUrl_AddFragmentEx(pcUrl,pcFragment, hMem,TRUE)
#else 
#ifdef WeUrl_AddFragment
#undef WeUrl_AddFragment
#endif
#define WeUrl_AddFragment(pcUrl,pcFragment) \
		WeUrl_AddFragmentEx(pcUrl,pcFragment,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_EscapeString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    It escapes the "special characters" in the URL string.
ARGUMENTS PASSED:
    
RETURN VALUE:
    Return a copy of 'pcS' where each character belonging to the set of so called 
    "special characters" or being in the range 0x80-0xff, has been replaced by a 
    hexadecimal esacape sequence of the form "%xy".Returns NULL in case of error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_EscapeStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeUrl_EscapeString
#undef WeUrl_EscapeString
#endif
#define WeUrl_EscapeString(pcStr, hMem) \
		WeUrl_EscapeStringEx(pcStr, hMem,TRUE)
#else 
#ifdef WeUrl_EscapeString
#undef WeUrl_EscapeString
#endif
#define WeUrl_EscapeString(pcStr) \
		WeUrl_EscapeStringEx(pcStr,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_EscapeNonAscii
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    It escapes the no-ASCII characters in the given string.
ARGUMENTS PASSED:
    pcStr: The given string
RETURN VALUE:
    Return a copy of 'pcStr' where each non-ASCII character has been replaced 
    by a hexadecimal esacape sequence of the form "%xy".
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_EscapeNonAsciiEx (const WE_CHAR *psStr,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef	WeUrl_EscapeNonAscii
#undef	WeUrl_EscapeNonAscii
#endif
#define	WeUrl_EscapeNonAscii(pcStr, hMem) \
		WeUrl_EscapeNonAsciiEx(pcStr, hMem,TRUE)
#else 
#ifdef	WeUrl_EscapeNonAscii
#undef	WeUrl_EscapeNonAscii
#endif
#define WeUrl_EscapeNonAscii(pcStr) \
		WeUrl_EscapeNonAsciiEx(pcStr,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
WeUrl_EscapeUrlString
CREATE DATE: 
2006-12-19
AUTHOR: 
Caiyi
DESCRIPTION:
It escapes the Unwise characters and whitespace in the given Url string.
ARGUMENTS PASSED:
pcStr: The given string
RETURN VALUE:
Return a copy of 'pcStr' where each Unwise characters and whitespace has been replaced 
by a hexadecimal esacape sequence of the form "%xy".
USED GLOBAL VARIABLES:

  USED STATIC VARIABLES:
  
	CALL BY:
    
	  IMPORTANT NOTES:
	  It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_EscapeUrlStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_EscapeUrlString
#undef	WeUrl_EscapeUrlString
#endif
#define WeUrl_EscapeUrlString(pcStr, hMem) \
		WeUrl_EscapeUrlStringEx(pcStr, hMem,TRUE)
#else 
#ifdef	WeUrl_EscapeUrlString
#undef	WeUrl_EscapeUrlString
#endif
#define WeUrl_EscapeUrlString(pcStr) \
		WeUrl_EscapeUrlStringEx(pcStr,NULL,FALSE)
#endif


/*=====================================================================================
FUNCTION: 
    WeUrl_WmlsEscapeString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:

ARGUMENTS PASSED:
    pcStr: The given string 
RETURN VALUE:
    Return a copy of 'pcStr' where each character belonging to the set of so called 
    "special characters" or being in the range 0x80-0xff, has been replaced by
    a hexadecimal esacape sequence of the form "%xy".
    Returns NULL if 'pcStr' contains characters whose Unicode representation is 
    greater than 0xff.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_WmlsEscapeStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_WmlsEscapeString
#undef	WeUrl_WmlsEscapeString
#endif
#define WeUrl_WmlsEscapeString(pcStr, hMem) \
		WeUrl_EscapeUrlStringEx(pcStr, hMem,TRUE)
#else 
#ifdef	WeUrl_WmlsEscapeString
#undef	WeUrl_WmlsEscapeString
#endif
#define WeUrl_WmlsEscapeString(pcStr) \
		WeUrl_WmlsEscapeStringEx(pcStr,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_UnescapeString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Return a copy of 'pcStr' where each hexadecimal escape sequence of the form "%xy"
    has been replaced with the character it represents.
ARGUMENTS PASSED:
    pcStr: The given string
RETURN VALUE:
    Returns NULL in case of error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR *WeUrl_UnescapeStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_UnescapeString
#undef	WeUrl_UnescapeString
#endif
#define WeUrl_UnescapeString(pcStr, hMem) \
		WeUrl_UnescapeStringEx(pcStr, hMem,TRUE)
#else 
#ifdef	WeUrl_UnescapeString
#undef	WeUrl_UnescapeString
#endif
#define WeUrl_UnescapeString(pcStr) \
		WeUrl_UnescapeStringEx(pcStr,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_WmlsUnescapeString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Return a copy of 'pcStr' where each hexadecimal escape sequence of the form 
    "%xy" has been replaced with the character it represents.
ARGUMENTS PASSED:
    pcStr: The given string
RETURN VALUE:
    Returns NULL if 'pcStr' contains characters that are not part of the US-ASCII character set.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_WmlsUnescapeStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_WmlsUnescapeString
#undef	WeUrl_WmlsUnescapeString
#endif
#define WeUrl_WmlsUnescapeString(pcStr, hMem) \
		WeUrl_WmlsUnescapeStringEx(pcStr, hMem,TRUE)
#else 
#ifdef	WeUrl_WmlsUnescapeString
#undef	WeUrl_WmlsUnescapeString
#endif
#define WeUrl_WmlsUnescapeString(pcStr) \
		WeUrl_WmlsUnescapeStringEx(pcStr,NULL,FALSE)
#endif


/*=====================================================================================
FUNCTION: 
    WeUrl_EscapeBlanks
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Return a copy of 'pcStr' where each blank character has been replaced by a hexadecimal
    esacape sequence of the form "%xy".
ARGUMENTS PASSED:
    pcStr: The given string
RETURN VALUE:
    Returns NULL in case of error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_EscapeBlanksEx (const WE_CHAR *pcStr,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_EscapeBlanks
#undef	WeUrl_EscapeBlanks
#endif
#define WeUrl_EscapeBlanks(pcStr, hMem) \
		WeUrl_EscapeBlanksEx(pcStr, hMem,TRUE)
#else 
#ifdef	WeUrl_EscapeBlanks
#undef	WeUrl_EscapeBlanks
#endif
#define WeUrl_EscapeBlanks(pcStr) \
		WeUrl_EscapeBlanksEx(pcStr,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckAccess
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check if the given absolute URL has access rights, given 'pcDomain' and 'pcPath' 
    as access control attributes.
 
   The check is performed by verifying that the domain attribute
   is a component-wise suffix of the domain part of the absolute URL,
   and that the path attribute is a component-wise prefix of
   the path part of the absolute URL. Comparisons of the paths
   are case-sensitive, but comparisons of the domain components are not.
   Handles escape sequences ("%xy") correctly.
ARGUMENTS PASSED:
    pcAbsUrl: The absolute url string
    pcDomain: The domain string
    pcPath: The path string
RETURN VALUE:
    Assumes that all three strings are zero-terminated char strings, and that 'pcAbsUrl' 
    is a valid absolute URL. Returns FALSE if either 'pcAbsUrl' or 'pcDomain' is NULL.
    If 'pcPath' is NULL, the routine works as if it were the path "/".
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckAccess (WE_CHAR *pcAbsUrl,
                           WE_CHAR *pcDomain,
                           WE_CHAR *pcPath);

/*=====================================================================================
FUNCTION: 
    WeUrl_MakeComplete
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Take an incomplete URL, like "www.abc.com", and turn it into a correct absolute
    URL using heuristic methods. This is not a well-defined process, rather it makes 
    a guess as to what the user msans.
ARGUMENTS PASSED:
    pcBs: The URL string given
RETURN VALUE:
    In the example above, the result would be "http://www.abc.com/". In case of 
    failure, NULL is returned.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_MakeCompleteEx (WE_CHAR *pcBs,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_MakeComplete
#undef	WeUrl_MakeComplete
#endif
#define WeUrl_MakeComplete(pcBs, hMem) \
		WeUrl_MakeCompleteEx(pcBs, hMem,TRUE)
#else 
#ifdef	WeUrl_MakeComplete
#undef	WeUrl_MakeComplete
#endif
#define WeUrl_MakeComplete(pcBs) \
		WeUrl_MakeCompleteEx(pcBs,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_MinRelativePath
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Given two URLs, compute the minimum relative path, i.e., the shortest relative path that
    together with 'pcBs1' can be resolved to yield 'pcBs2'. If 'bIncludeFragment' is TRUE any
    fragment part in 'bs2' is included in the result, otherwise it is dropped.
ARGUMENTS PASSED:
    pcBs1: The first url string
    pcBs2: The second url string
    bIncludeFragment: It indicates that whether the relative path includes the fragment part 
                                of the second url string.
RETURN VALUE:
    Returns NULL in case of error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_MinRelativePathEx (WE_CHAR *pcBs1,
                                 WE_CHAR *pcBs2,
                                 WE_BOOL bIncludFragment,
                                 WE_HANDLE hMem,
								 WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_MinRelativePath
#undef	WeUrl_MinRelativePath
#endif
#define WeUrl_MinRelativePath( pcBs1,pcBs2, bIncludFragment, hMem) \
		WeUrl_MinRelativePathEx( pcBs1, pcBs2,bIncludFragment, hMem,TRUE)
#else 
#ifdef	WeUrl_MinRelativePath
#undef	WeUrl_MinRelativePath
#endif
#define WeUrl_MinRelativePath(pcBs1,pcBs2, bIncludFragment) \
		WeUrl_MinRelativePathEx(pcBs1,pcBs2, bIncludFragment,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_IsComplete
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check that a URL can be parsed and has at least a scheme, host, 
    and path component.
ARGUMENTS PASSED:
    pcUrl: Pointer to the URL string to be checked
RETURN VALUE:
    It returns TRUE if the url has all the scheme part, the host part and the path part
    otherwise returns FALSE.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_IsComplete (const WE_CHAR *pcUrl);

/*=====================================================================================
FUNCTION: 
    WeUrl_IsComplete
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Return a URL that is a copy of 'pcOldUrl', but with 'pcNewQuery' appended to the query part.
    If 'pcOldUrl' already has a query part, that old part will be separated from the new by a 
    '&'-character.
ARGUMENTS PASSED:
    pcOldUrl: The URL string that the query string adds to
    pcNewQuery: Pointer to the query string
RETURN VALUE:
    Returns NULL in case of error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the responsibility of the caller to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_AppendToQueryEx (WE_CHAR *pcOldUrl,
								 WE_CHAR *pcNewQuery,
								 WE_HANDLE hMem,
								 WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_AppendToQuery
#undef	WeUrl_AppendToQuery
#endif
#define WeUrl_AppendToQuery( pcOldUrl,pcNewQuery, hMem) \
		WeUrl_AppendToQueryEx( pcOldUrl,pcNewQuery, hMem,TRUE)
#else 
#ifdef	WeUrl_AppendToQuery
#undef	WeUrl_AppendToQuery
#endif
#define WeUrl_AppendToQuery(pcOldUrl,pcNewQuery) \
		WeUrl_AppendToQueryEx(pcOldUrl,pcNewQuery,NULL,FALSE)
#endif
/*=====================================================================================
FUNCTION: 
    WeUrl_GetProtocol
CREATE DATE: 
    2006-11-17
AUTHOR: 
    Caiyi
DESCRIPTION:
Extract the protocol part of a URL.
ARGUMENTS PASSED:
pcUrl: The URL string
RETURN VALUE:
Returns NULL in case of error, including that the URL is not valid, or if the URL 
does not have a scheme part.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
IMPORTANT NOTES:
It is the responsibility of the caller to deallocate the returned string.   
=====================================================================================*/
WE_CHAR *WeUrl_GetProtocolEx (const WE_CHAR *pcUrl,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_GetProtocol
#undef	WeUrl_GetProtocol
#endif
#define WeUrl_GetProtocol( pcUrl, hMem) \
		WeUrl_GetProtocolEx( pcUrl, hMem,TRUE)
#else 
#ifdef	WeUrl_GetProtocol
#undef	WeUrl_GetProtocol
#endif
#define WeUrl_GetProtocol(pcUrl) \
		WeUrl_GetProtocolEx(pcUrl,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_SetFragment
CREATE DATE: 
    2006-11-17
AUTHOR: 
    Caiyi
DESCRIPTION:
Set the new fragment replace the primary fragment, and return the new Url.
ARGUMENTS PASSED:
pcUrl: The URL string
pcFrgament: The Fragment string
RETURN VALUE:
Returns NULL in case of error, including that the URL is not valid.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
IMPORTANT NOTES:   
it is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_SetFragmentEx (const WE_CHAR *pcUrl,
							 const WE_CHAR *pcFragment,
							 WE_HANDLE hMem,
							 WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_SetFragment
#undef	WeUrl_SetFragment
#endif
#define WeUrl_SetFragment( pcUrl,pcFragment, hMem) \
		WeUrl_SetFragmentEx(pcUrl,pcFragment, hMem,TRUE)
#else 
#ifdef	WeUrl_SetFragment
#undef	WeUrl_SetFragment
#endif
#define WeUrl_SetFragment(pcUrl,pcFragment) \
		WeUrl_SetFragmentEx(pcUrl,pcFragment,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_SetPath
CREATE DATE: 
    2006-11-17
AUTHOR: 
    Caiyi
DESCRIPTION:
Set the new Path replace the primary Path, and return the new Url.
ARGUMENTS PASSED:
pcUrl: The URL string
pcFrgament: The Path string
RETURN VALUE:
Returns NULL in case of error, including that the URL is not valid.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
IMPORTANT NOTES:   
it is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_SetPathEx (const WE_CHAR *pcUrl,
						   const WE_CHAR *pcPath,
						   WE_HANDLE hMem,
						   WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_SetPath
#undef	WeUrl_SetPath
#endif
#define WeUrl_SetPath( pcUrl,pcPath, hMem) \
		WeUrl_SetPathEx(pcUrl,pcPath, hMem,TRUE)
#else 
#ifdef	WeUrl_SetPath
#undef	WeUrl_SetPath
#endif
#define WeUrl_SetPath(pcUrl,pcPath) \
		WeUrl_SetPathEx(pcUrl,pcPath,NULL,FALSE)
#endif


/*=====================================================================================
FUNCTION: 
    WeUrl_SetQuery
CREATE DATE: 
    2006-11-17
AUTHOR: 
    Caiyi
DESCRIPTION:
Set the new Query replace the primary Query, and return the new Url.
ARGUMENTS PASSED:
pcUrl: The URL string
pcFrgament: The Query string
RETURN VALUE:
Returns NULL in case of error, including that the URL is not valid.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
IMPORTANT NOTES:   
it is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_SetQueryEx (const WE_CHAR *pcUrl,
						  const WE_CHAR *pcQuery,
						  WE_HANDLE hMem,
						  WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_SetQuery
#undef	WeUrl_SetQuery
#endif
#define WeUrl_SetQuery(pcUrl,pcQuery, hMem) \
		WeUrl_SetQueryEx(pcUrl,pcQuery, hMem,TRUE)
#else 
#ifdef	WeUrl_SetQuery
#undef	WeUrl_SetQuery
#endif
#define WeUrl_SetQuery(pcUrl,pcQuery) \
		WeUrl_SetQueryEx(pcUrl,pcQuery,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_SetPort
CREATE DATE: 
    2006-11-17
AUTHOR: 
    Caiyi
DESCRIPTION:
Set the new Port replace the primary Port, and return the new Url.
ARGUMENTS PASSED:
pcUrl: The URL string
pcFrgament: The Port string
RETURN VALUE:
Returns NULL in case of error, including that the URL is not valid.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
IMPORTANT NOTES:   
it is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_SetPortEx (const WE_CHAR *pcUrl,
						 const WE_CHAR *pcPort,
						 WE_HANDLE hMem,
						 WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_SetPort
#undef	WeUrl_SetPort
#endif
#define WeUrl_SetPort(pcUrl,pcPort, hMem) \
		WeUrl_SetPortEx(pcUrl,pcPort, hMem,TRUE)
#else 
#ifdef	WeUrl_SetPort
#undef	WeUrl_SetPort
#endif
#define WeUrl_SetPort(pcUrl,pcPort) \
		WeUrl_SetPortEx(pcUrl,pcPort,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_SetHost
CREATE DATE: 
    2006-11-17
AUTHOR: 
    Caiyi
DESCRIPTION:
Set the new Host replace the primary Host, and return the new Url.
ARGUMENTS PASSED:
pcUrl: The URL string
pcFrgament: The Host string
RETURN VALUE:
Returns NULL in case of error, including that the URL is not valid.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
IMPORTANT NOTES:   
it is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_SetHostEx (const WE_CHAR *pcUrl,
						 const WE_CHAR *pcHost,
						 WE_HANDLE hMem,
						 WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_SetHost
#undef	WeUrl_SetHost
#endif
#define WeUrl_SetHost(pcUrl,pcHost, hMem) \
		WeUrl_SetHostEx(pcUrl,pcHost, hMem,TRUE)
#else 
#ifdef	WeUrl_SetHost
#undef	WeUrl_SetHostEx
#endif
#define WeUrl_SetHost(pcUrl,pcHost) \
		WeUrl_SetHostEx(pcUrl,pcHost,NULL,FALSE)
#endif


/*=====================================================================================
FUNCTION: 
    WeUrl_SetHostPort
CREATE DATE: 
    2006-11-17
AUTHOR: 
    Caiyi
DESCRIPTION:
Set the new HostPort replace the primary HostPort, and return the new Url.
ARGUMENTS PASSED:
pcUrl: The URL string
pcFrgament: The HostPort string
RETURN VALUE:
Returns NULL in case of error, including that the URL is not valid.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
IMPORTANT NOTES:   
it is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_SetHostPortEx(const WE_CHAR *pcUrl,
							const WE_CHAR *pcHostPort,
							WE_HANDLE hMem,
							WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_SetHostPort
#undef	WeUrl_SetHostPort
#endif
#define WeUrl_SetHostPort( pcUrl,pcHostPort, hMem) \
		WeUrl_SetHostPortEx(pcUrl,pcHostPort, hMem,TRUE)
#else 
#ifdef	WeUrl_SetHostPort
#undef	WeUrl_SetHostPort
#endif
#define WeUrl_SetHostPort(pcUrl,pcHostPort) \
		WeUrl_SetHostPortEx(pcUrl,pcHostPort,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_SetProtocol
CREATE DATE: 
    2006-11-17
AUTHOR: 
    Caiyi
DESCRIPTION:
Set the new Protocol replace the primary Protocol, and return the new Url.
ARGUMENTS PASSED:
pcUrl: The URL string
pcFrgament: The Protocol string
RETURN VALUE:
Returns NULL in case of error, including that the URL is not valid.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
IMPORTANT NOTES:   
it is the caller's responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR * WeUrl_SetProtocolEx (const WE_CHAR *pcUrl,
							 const WE_CHAR *pcProtocol,
							 WE_HANDLE hMem,
							 WE_BOOL bMemMgr);

#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_SetProtocol
#undef	WeUrl_SetProtocol
#endif
#define WeUrl_SetProtocol(pcUrl,pcProtocol, hMem) \
		WeUrl_SetProtocolEx(pcUrl,pcProtocol, hMem,TRUE)
#else 
#ifdef	WeUrl_SetProtocol
#undef	WeUrl_SetProtocol
#endif
#define WeUrl_SetProtocol(pcUrl,pcProtocol) \
		WeUrl_SetProtocolEx(pcUrl,pcProtocol,NULL,FALSE)
#endif



/*=====================================================================================
FUNCTION: 
WeUrl_Addslash
CREATE DATE: 
2007-2-1
AUTHOR: 
Caiyi
DESCRIPTION:
* Given a URL add a slash when it has no path part
* and store as an new URL in the string "*ppcUrl" 
* otherwise nothing is stored in "*ppcUrl".
ARGUMENTS PASSED:
pcUrl: the given URL
ppcUrl: store an new URL
RETURN VALUE:
* 0:add a slash
* 1:not add a slash
* -2:the given parameter is null.
* -3:parse failed
* 3:malloc failed
  
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
	  
CALL BY:
		
IMPORTANT NOTES:
* NOTE: It is the callers responsibility to deallocate the ppcUrl.		  
=====================================================================================*/
WE_INT WeUrl_AddslashEx(const WE_CHAR *pcUrl,
                      WE_CHAR **ppcUrl,
                      WE_HANDLE hMem,
					  WE_BOOL bMemMgr);


#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_Addslash
#undef	WeUrl_Addslash
#endif
#define WeUrl_Addslash( pcUrl, ppcUrl, hMem) \
		WeUrl_AddslashEx(pcUrl, ppcUrl, hMem,TRUE)
#else 
#ifdef	WeUrl_Addslash
#undef	WeUrl_Addslash
#endif
#define WeUrl_Addslash(pcUrl, ppcUrl) \
		WeUrl_AddslashEx(pcUrl,ppcUrl,NULL,FALSE)
#endif


WE_CHAR * WeUrl_GetSerchEx (const WE_CHAR *pcUrl,
                            WE_HANDLE hMem,
                            WE_BOOL bMemMgr);



#ifdef	_USE_MEM_HANDLE
#ifdef	WeUrl_GetSerch
#undef	WeUrl_GetSerch
#endif
#define WeUrl_GetSerch(pcUrl, hMem) \
WeUrl_GetSerchEx(pcUrl,hMem,TRUE)
#else 
#ifdef	WeUrl_GetSerch
#undef	WeUrl_GetSerch
#endif
#define WeUrl_GetSerch(pcUrl) \
WeUrl_GetSerchEx(pcUrl,NULL,FALSE)
#endif

/*=====================================================================================
FUNCTION: 
    WeUrl_IsPrefix
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check if the URL "pcPath1" is a prefix of "pcPath2".
    The prefix match is done according to the rules in section 7.4 in "WAP Cache
    Operation Specification".
ARGUMENTS PASSED:
    pcPath1: The first url string
    pcPath2: The second url string
RETURN VALUE:
    It returns TRUE if the first url is a prefix of the second url string,
    otherwise it returns FALSE.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/

WE_BOOL WeUrl_IsPrefix (WE_CHAR *pcPath1,
                        WE_CHAR *pcPath2);

#endif/*endif WE_URL_H*/
/*--------------------------END-----------------------------*/
