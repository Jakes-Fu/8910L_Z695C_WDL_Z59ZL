/*=====================================================================================

    HEADER NAME : we_hdr.h
    MODULE NAME : HDR

    PRE-INCLUDE FILES DESCRIPTION

    GENERAL DESCRIPTION
        This file be included by all WE Programmer.In this file some widget  type and  macro is defined.
        These Info is  variational and is changed by switching Project.

    TECHFAITH Wireless Confidential Proprietary
    (c) Copyright 2006 by TECHFAITH Software. All Rights Reserved.
=======================================================================================
    Revision History
   
    Modification                 Tracking
    Date           Author         Number        Description of changes
    ----------   ------------    ---------   --------------------------------------------
    2006-5-23   Guohong              create we_hdr.h
    2006-7-5    Guohong                       change the function WeHdr_AddHeaderFromTextLine 
                                                              as the interface     
    2007-7-17   TomTang              Update
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
#ifndef WE_HDR_H
#define WE_HDR_H

/*******************************************************************************
*   Include File Section
*******************************************************************************/
/*Include System head file*/

/*Include Program Global head file*/
#include "we_def.h"
#include "we_cfg.h"
#include "we_mem.h"
//#include "we_cmmn.h"
#include "we_scl.h"

/*Include Module head file*/
#include "we_mime.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/
/* 
 * Supported header fields.
 * The numeric representation is the one used by WSP.
 * No suffix msans that the code belongs to encoding version 1.1-1.2.
 */
#define G_WE_HDR_ACCEPT                         0x00
#define G_WE_HDR_ACCEPT_CHARSET                 0x01
#define G_WE_HDR_ACCEPT_ENCODING                0x02
#define G_WE_HDR_ACCEPT_LANGUAGE                0x03
#define G_WE_HDR_ACCEPT_RANGES                  0x04
#define G_WE_HDR_AGE                            0x05
#define G_WE_HDR_ALLOW                          0x06
#define G_WE_HDR_AUTHORIZATION                  0x07
#define G_WE_HDR_CACHE_CONTROL                  0x08
#define G_WE_HDR_CONNECTION                     0x09
#define G_WE_HDR_CONTENT_BASE                   0x0a
#define G_WE_HDR_CONTENT_ENCODING               0x0b
#define G_WE_HDR_CONTENT_LANGUAGE               0x0c
#define G_WE_HDR_CONTENT_LENGTH                 0x0d
#define G_WE_HDR_CONTENT_LOCATION               0x0e
#define G_WE_HDR_CONTENT_MD5                    0x0f
#define G_WE_HDR_CONTENT_RANGE                  0x10
#define G_WE_HDR_CONTENT_TYPE                   0x11
#define G_WE_HDR_DATE                           0x12
#define G_WE_HDR_ETAG                           0x13
#define G_WE_HDR_EXPIRES                        0x14
#define G_WE_HDR_FROM                           0x15
#define G_WE_HDR_HOST                           0x16
#define G_WE_HDR_IF_MODIFIED_SINCE              0x17
#define G_WE_HDR_IF_MATCH                       0x18
#define G_WE_HDR_IF_NONE_MATCH                  0x19
#define G_WE_HDR_IF_RANGE                       0x1a
#define G_WE_HDR_IF_UNMODIFIED_SINCE            0x1b
#define G_WE_HDR_LOCATION                       0x1c
#define G_WE_HDR_LAST_MODIFIED                  0x1d
#define G_WE_HDR_MAX_FORWARDS                   0x1e
#define G_WE_HDR_PRAGMA                         0x1f
#define G_WE_HDR_PROXY_AUTHENTICATE             0x20
#define G_WE_HDR_PROXY_AUTHORIZATION            0x21
#define G_WE_HDR_PUBLIC                         0x22
#define G_WE_HDR_RANGE                          0x23
#define G_WE_HDR_REFERER                        0x24
#define G_WE_HDR_RETRY_AFTER                    0x25
#define G_WE_HDR_SERVER                         0x26
#define G_WE_HDR_TRANSFER_ENCODING              0x27
#define G_WE_HDR_UPGRADE                        0x28
#define G_WE_HDR_USER_AGENT                     0x29
#define G_WE_HDR_VARY                           0x2a
#define G_WE_HDR_VIA                            0x2b
#define G_WE_HDR_WARNING                        0x2c
#define G_WE_HDR_WWW_AUTHENTICATE               0x2d
#define G_WE_HDR_CONTENT_DISPOSITION            0x2e
#define G_WE_HDR_X_WAP_APPLICATION_ID           0x2f
#define G_WE_HDR_X_WAP_CONTENT_URI              0x30
#define G_WE_HDR_X_WAP_INITIATOR_URI            0x31
#define G_WE_HDR_ACCEPT_APPLICATION             0x32
#define G_WE_HDR_BEARER_INDICATION              0x33
#define G_WE_HDR_PUSH_FLAG                      0x34
#define G_WE_HDR_PROFILE                        0x35
#define G_WE_HDR_PROFILE_DIFF                   0x36
#define G_WE_HDR_PROFILE_WARNING                0x37
#define G_WE_HDR_EXPECT                         0x38
#define G_WE_HDR_TE                             0x39
#define G_WE_HDR_TRAILER                        0x3a
#define G_WE_HDR_ACCEPT_CHARSET_1_3             0x3b
#define G_WE_HDR_ACCEPT_ENCODING_1_3            0x3c
#define G_WE_HDR_CACHE_CONTROL_1_3              0x3d
#define G_WE_HDR_CONTENT_RANGE_1_3              0x3e
#define G_WE_HDR_X_WAP_TOD                      0x3f
#define G_WE_HDR_CONTENT_ID                     0x40
#define G_WE_HDR_SET_COOKIE                     0x41
#define G_WE_HDR_COOKIE                         0x42
#define G_WE_HDR_ENCODING_VERSION               0x43
#define G_WE_HDR_PROFILE_WARNING_1_4            0x44
#define G_WE_HDR_CONTENT_DISPOSITION_1_4 0x45
#define G_WE_HDR_X_WAP_SECURITY                 0x46
#define G_WE_HDR_CACHE_CONTROL_1_4              0x47
#define G_WE_HDR_EXPECT_1_5                     0x48
#define G_WE_HDR_X_WAP_LOC_INVOCATION           0x49
#define G_WE_HDR_X_WAP_LOC_DELIVERY             0x4a

/* The following headers do not have any WSP values. */
#define G_WE_HDR_X_WAP_CPITAG                   0x6e
#define G_WE_HDR_X_WAP_PUSH_PROVURL             0x6f
#define G_WE_HDR_X_WAP_PUSH_ACCEPT              0x70
#define G_WE_HDR_X_WAP_PUSH_ACCEPT_CHARSET      0x71
#define G_WE_HDR_X_WAP_PUSH_ACCEPT_ENCODING     0x72
#define G_WE_HDR_X_WAP_PUSH_ACCEPT_LANGUAGE     0x73
#define G_WE_HDR_X_WAP_PUSH_ACCEPT_APP_ID       0x74
#define G_WE_HDR_X_WAP_PUSH_MSG_SIZE            0x75 
#define G_WE_HDR_X_WAP_PUSH_ACCEPT_MAX_PUSH_REQ 0x76 
#define G_WE_HDR_X_WAP_TERMINAL_ID              0x77
#define G_WE_HDR_X_WAP_PUSH_INFO                0x78
#define G_WE_HDR_X_WAP_PUSH_STATUS              0x79
#define G_WE_HDR_X_WAP_PUSH_OTA_VERSION         0x7a
#define G_WE_HDR_X_WAP_BEARER_INDICATION        0x7b
#define G_WE_HDR_X_WAP_AUTHENTICATE             0x7c
#define G_WE_HDR_X_WAP_AUTHORIZATION            0x7d
#define G_WE_HDR_X_WAP_PROFILE                  0x7e
#define G_WE_HDR_X_WAP_PROFILE_DIFF             0x7f
#define G_WE_HDR_X_WAP_PROFILE_WARNING          0x80
#define G_WE_HDR_X_UP_TPD_E2E_HEADERS           0x81
#define G_WE_HDR_X_FORWARDED_FOR                0x82
#define G_WE_HDR_CONTENT_TRANSFER_ENCODING      0x83
#define G_WE_HDR_REFRESH                        0x84
#define G_WE_HDR_X_DRM                          0x85
#define G_WE_HDR_SET_COOKIE2                    0x86                     
#define G_WE_HDR_COOKIE2                        0x87
#define G_WE_HDR_X_VODAFONE_3GPDP_CONTEXT       0x88
#define G_WE_HDR_X_OMA_DRM_SEPARATE_DELIVERY    0x89
#define G_WE_HDR_RIGHTS_ISSUER                  0x8a
#define G_WE_HDR_CONTENT_NAME                   0x8b
#define G_WE_HDR_CONTENT_DESCRIPTION            0x8c
#define G_WE_HDR_CONTENT_VENDOR                 0x8d
#define G_WE_HDR_ICON_URI                       0x8e
#define G_WE_HDR_ENCRYPTION_METHOD              0x8f
#define G_WE_HDR_X_VODAFONE_CONTENT             0x90
#define G_WE_HDR_KEEP_ALIVE                     0x91
#define G_WE_HDR_PROXY_CONNECTION            0x92
#define G_WE_HDR_X_SP_RESULT                    0x93


#define G_WE_HDR_UNKNOWN                        0x94




/* Languages */
#define G_WE_LANGUAGE_AA                 0x01
#define G_WE_LANGUAGE_AB                 0x02
#define G_WE_LANGUAGE_AF                 0x03
#define G_WE_LANGUAGE_AM                 0x04
#define G_WE_LANGUAGE_AR                 0x05
#define G_WE_LANGUAGE_AS                 0x06
#define G_WE_LANGUAGE_AY                 0x07
#define G_WE_LANGUAGE_AZ                 0x08
#define G_WE_LANGUAGE_BA                 0x09
#define G_WE_LANGUAGE_BE                 0x0A
#define G_WE_LANGUAGE_BG                 0x0B
#define G_WE_LANGUAGE_BH                 0x0C
#define G_WE_LANGUAGE_BI                 0x0D
#define G_WE_LANGUAGE_BN                 0x0E
#define G_WE_LANGUAGE_BO                 0x0F
#define G_WE_LANGUAGE_BR                 0x10
#define G_WE_LANGUAGE_CA                 0x11
#define G_WE_LANGUAGE_CO                 0x12
#define G_WE_LANGUAGE_CS                 0x13
#define G_WE_LANGUAGE_CY                 0x14
#define G_WE_LANGUAGE_DA                 0x15
#define G_WE_LANGUAGE_DE                 0x16
#define G_WE_LANGUAGE_DZ                 0x17
#define G_WE_LANGUAGE_EL                 0x18
#define G_WE_LANGUAGE_EN                 0x19
#define G_WE_LANGUAGE_EO                 0x1A
#define G_WE_LANGUAGE_ES                 0x1B
#define G_WE_LANGUAGE_ET                 0x1C
#define G_WE_LANGUAGE_EU                 0x1D
#define G_WE_LANGUAGE_FA                 0x1E
#define G_WE_LANGUAGE_FI                 0x1F
#define G_WE_LANGUAGE_FJ                 0x20
#define G_WE_LANGUAGE_FO                 0x82
#define G_WE_LANGUAGE_FR                 0x22
#define G_WE_LANGUAGE_FY                 0x83
#define G_WE_LANGUAGE_GA                 0x24
#define G_WE_LANGUAGE_GD                 0x25
#define G_WE_LANGUAGE_GL                 0x26
#define G_WE_LANGUAGE_GN                 0x27
#define G_WE_LANGUAGE_GU                 0x28
#define G_WE_LANGUAGE_HA                 0x29
#define G_WE_LANGUAGE_HE                 0x2A
#define G_WE_LANGUAGE_HI                 0x2B
#define G_WE_LANGUAGE_HR                 0x2C
#define G_WE_LANGUAGE_HU                 0x2D
#define G_WE_LANGUAGE_HY                 0x2E
#define G_WE_LANGUAGE_IA                 0x84
#define G_WE_LANGUAGE_ID                 0x30
#define G_WE_LANGUAGE_IE                 0x86
#define G_WE_LANGUAGE_IK                 0x87
#define G_WE_LANGUAGE_IS                 0x33
#define G_WE_LANGUAGE_IT                 0x34
#define G_WE_LANGUAGE_IU                 0x89
#define G_WE_LANGUAGE_JA                 0x36
#define G_WE_LANGUAGE_JW                 0x37
#define G_WE_LANGUAGE_KA                 0x38
#define G_WE_LANGUAGE_KK                 0x39
#define G_WE_LANGUAGE_KL                 0x8A
#define G_WE_LANGUAGE_KM                 0x3B
#define G_WE_LANGUAGE_KN                 0x3C
#define G_WE_LANGUAGE_KO                 0x3D
#define G_WE_LANGUAGE_KS                 0x3E
#define G_WE_LANGUAGE_KU                 0x3F
#define G_WE_LANGUAGE_KY                 0x40
#define G_WE_LANGUAGE_LA                 0x8B
#define G_WE_LANGUAGE_LN                 0x42
#define G_WE_LANGUAGE_LO                 0x43
#define G_WE_LANGUAGE_LT                 0x44
#define G_WE_LANGUAGE_LV                 0x45
#define G_WE_LANGUAGE_MG                 0x46
#define G_WE_LANGUAGE_MI                 0x47
#define G_WE_LANGUAGE_MK                 0x48
#define G_WE_LANGUAGE_ML                 0x49
#define G_WE_LANGUAGE_MN                 0x4A
#define G_WE_LANGUAGE_MO                 0x4B
#define G_WE_LANGUAGE_MR                 0x4C
#define G_WE_LANGUAGE_MS                 0x4D
#define G_WE_LANGUAGE_MT                 0x4E
#define G_WE_LANGUAGE_MY                 0x4F
#define G_WE_LANGUAGE_NA                 0x81
#define G_WE_LANGUAGE_NE                 0x51
#define G_WE_LANGUAGE_NL                 0x52
#define G_WE_LANGUAGE_NO                 0x53
#define G_WE_LANGUAGE_OC                 0x54
#define G_WE_LANGUAGE_OM                 0x55
#define G_WE_LANGUAGE_OR                 0x56
#define G_WE_LANGUAGE_PA                 0x57
#define G_WE_LANGUAGE_PO                 0x58
#define G_WE_LANGUAGE_PS                 0x59
#define G_WE_LANGUAGE_PT                 0x5A
#define G_WE_LANGUAGE_QU                 0x5B
#define G_WE_LANGUAGE_RM                 0x8C
#define G_WE_LANGUAGE_RN                 0x5D
#define G_WE_LANGUAGE_RO                 0x5E
#define G_WE_LANGUAGE_RU                 0x5F
#define G_WE_LANGUAGE_RW                 0x60
#define G_WE_LANGUAGE_SA                 0x61
#define G_WE_LANGUAGE_SD                 0x62
#define G_WE_LANGUAGE_SG                 0x63
#define G_WE_LANGUAGE_SH                 0x64
#define G_WE_LANGUAGE_SI                 0x65
#define G_WE_LANGUAGE_SK                 0x66
#define G_WE_LANGUAGE_SL                 0x67
#define G_WE_LANGUAGE_SM                 0x68
#define G_WE_LANGUAGE_SN                 0x69
#define G_WE_LANGUAGE_SO                 0x6A
#define G_WE_LANGUAGE_SQ                 0x6B
#define G_WE_LANGUAGE_SR                 0x6C
#define G_WE_LANGUAGE_SS                 0x6D
#define G_WE_LANGUAGE_ST                 0x6E
#define G_WE_LANGUAGE_SU                 0x6F
#define G_WE_LANGUAGE_SV                 0x70
#define G_WE_LANGUAGE_SW                 0x71
#define G_WE_LANGUAGE_TA                 0x72
#define G_WE_LANGUAGE_TE                 0x73
#define G_WE_LANGUAGE_TG                 0x74
#define G_WE_LANGUAGE_TH                 0x75
#define G_WE_LANGUAGE_TI                 0x76
#define G_WE_LANGUAGE_TK                 0x77
#define G_WE_LANGUAGE_TL                 0x78
#define G_WE_LANGUAGE_TN                 0x79
#define G_WE_LANGUAGE_TO                 0x7A
#define G_WE_LANGUAGE_TR                 0x7B
#define G_WE_LANGUAGE_TS                 0x7C
#define G_WE_LANGUAGE_TT                 0x7D
#define G_WE_LANGUAGE_TW                 0x7E
#define G_WE_LANGUAGE_UG                 0x7F
#define G_WE_LANGUAGE_UK                 0x50
#define G_WE_LANGUAGE_UR                 0x21
#define G_WE_LANGUAGE_UZ                 0x23
#define G_WE_LANGUAGE_VI                 0x2F
#define G_WE_LANGUAGE_VO                 0x85
#define G_WE_LANGUAGE_WO                 0x31
#define G_WE_LANGUAGE_XH                 0x32
#define G_WE_LANGUAGE_YI                 0x88
#define G_WE_LANGUAGE_YO                 0x35
#define G_WE_LANGUAGE_ZA                 0x3A
#define G_WE_LANGUAGE_ZH                 0x41
#define G_WE_LANGUAGE_ZU                 0x5C
#define G_WE_LANGUAGE_UNKNOWN            0xff


/* Application wid, well known values */
#define G_WE_X_WAP_APPLICATION                     0x00
#define G_WE_X_WAP_APPLICATION_PUSH_SIA            0x01
#define G_WE_X_WAP_APPLICATION_WML_UA              0x02
#define G_WE_X_WAP_APPLICATION_WTA_UA              0x03
#define G_WE_X_WAP_APPLICATION_MMS_UA              0x04
#define G_WE_X_WAP_APPLICATION_PUSH_SYNCML         0x05
#define G_WE_X_WAP_APPLICATION_LOC_UA              0x06

/* Application wid, registered values */
#define G_WE_X_WAP_MICROSOFT_LOCALCONTENT_UA        0x8000
#define G_WE_X_WAP_MICROSOFT_IMCLIENT_UA            0x8001
#define G_WE_X_WAP_DOCOMO_IMODE_MAIL_UA             0x8002
#define G_WE_X_WAP_DOCOMO_IMODE_MR_UA               0x8003
#define G_WE_X_WAP_DOCOMO_IMODE_MF_UA               0x8004


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
typedef struct tagSt_WeHdr St_WeHdr;

St_WeSCLStrtableInfo *WeHdr_GetHeaderNamesEx(WE_HANDLE hMem,WE_BOOL bMemMgr);


void WeHdr_FreeHeaderNamesEx(St_WeSCLStrtableInfo * pSt_WeHdrNames,WE_HANDLE hMem,WE_BOOL bMemMgr);

//extern  const St_WeSCLStrtableInfo stWeLanguages;
St_WeSCLStrtableInfo * WeHdr_GetLanguagesEx(WE_HANDLE hMem,WE_BOOL bMemMgr);

void WeHdr_FreeLanguagesEx(St_WeSCLStrtableInfo *pstWeLanguages,WE_HANDLE hMem,WE_BOOL bMemMgr);

St_WeHdr * WeHdr_CreateEx(WE_HANDLE hMem,WE_BOOL bMemMgr);

void WeHdr_DeleteEx(St_WeHdr *pstHdr,WE_HANDLE hMem,WE_BOOL bMemMgr);

St_WeHdr * WeHdr_CopyEx(St_WeHdr *pstHdr,WE_HANDLE hMem,WE_BOOL bMemMgr);

void WeHdr_DeleteFieldEx(St_WeHdr *pstHdr,WE_INT32 iFieldName,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_BOOL WeHdr_AddStringEx(St_WeHdr *pstHdr,WE_INT32 iFieldName,WE_CHAR *pcValue,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_BOOL WeHdr_AddParamEx(St_WeHdr      *pstHdr,WE_INT32  iFieldName,const WE_CHAR *pcParam,const WE_CHAR *pcValue,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_BOOL WeHdr_AddHeaderFromTextLineEx(St_WeHdr *pstHdr,const WE_CHAR *pcStr,WE_INT32 iLen,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_BOOL WeHdr_AddHeadersFromTextEx(St_WeHdr *pstHdr,const WE_CHAR *pstText,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_BOOL WeHdr_MakeStringEx(St_WeHdr *pstHdr,WE_CHAR **ppcStr,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_BOOL WeHdr_GetUint32Ex(WE_HANDLE hCSC,St_WeHdr *pstHdr,WE_INT32 iFieldName,WE_UINT32 *puiVal,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_CHAR * WeHdr_GetParameterEx(St_WeHdr        *pstHdr,WE_INT32        iFieldName,const WE_CHAR *pcParameter,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_CHAR * WeHdr_BuildParaListEx(WE_INT32        iNumParams,const WE_CHAR *apcParamName[],const WE_CHAR *apcParamValue[],WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_INT32 WeHdr_GetParamsEx(const WE_CHAR *pcParamList,WE_INT32 iMaxParams,WE_CHAR *apcParamName[],WE_CHAR *apcParamValue[],WE_HANDLE hMem,WE_BOOL bMemMgr);

const WE_CHAR * WeHdr_GetString (St_WeHdr *pstHdr,WE_INT32 iFieldName);



#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_GetHeaderNames
#undef WeHdr_GetHeaderNames
#endif
#define WeHdr_GetHeaderNames(hMem) WeHdr_GetHeaderNamesEx( hMem,TRUE)
#else 
#ifdef WeHdr_GetHeaderNames
#undef WeHdr_GetHeaderNames
#endif
#define WeHdr_GetHeaderNames() WeHdr_GetHeaderNamesEx(NULL,FALSE)
#endif

#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_FreeHeaderNames
#undef WeHdr_FreeHeaderNames
#endif
#define WeHdr_FreeHeaderNames(pSt_WeHdrNames, hMem) WeHdr_FreeHeaderNamesEx( pSt_WeHdrNames, hMem,TRUE)
#else 
#ifdef WeHdr_FreeHeaderNames
#undef WeHdr_FreeHeaderNames
#endif
#define WeHdr_FreeHeaderNames( pSt_WeHdrNames)\
		WeHdr_FreeHeaderNamesEx(pSt_WeHdrNames,NULL,FALSE)
#endif

#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_GetLanguages
#undef WeHdr_GetLanguages
#endif
#define WeHdr_GetLanguages(hMem)\
		   WeHdr_GetLanguagesEx(hMem,TRUE)
#else 
#ifdef WeHdr_GetLanguages
#undef WeHdr_GetLanguages
#endif
#define WeHdr_GetLanguages()\
		WeHdr_GetLanguagesEx(NULL,FALSE)
#endif


#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_FreeLanguages
#undef WeHdr_FreeLanguages
#endif
#define  WeHdr_FreeLanguages(pstWeLanguages,hMem) WeHdr_FreeLanguagesEx(pstWeLanguages, hMem,TRUE)
#else 
#ifdef WeHdr_FreeLanguages
#undef WeHdr_FreeLanguages
#endif
#define WeHdr_FreeLanguages(pstWeLanguages)\
		WeHdr_FreeLanguagesEx(pstWeLanguages,NULL,FALSE)
#endif



#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_Create
#undef WeHdr_Create
#endif
#define WeHdr_Create(hMem)  WeHdr_CreateEx( hMem,TRUE)
#else 
#ifdef WeHdr_Create
#undef WeHdr_Create
#endif
#define WeHdr_Create()\
		WeHdr_CreateEx(NULL,FALSE)
#endif




#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_Delete
#undef WeHdr_Delete
#endif
#define WeHdr_Delete(pstHdr, hMem) WeHdr_DeleteEx( pstHdr, hMem,TRUE)
#else 
#ifdef WeHdr_Delete
#undef WeHdr_Delete
#endif
#define WeHdr_Delete(pstHdr)\
		WeHdr_DeleteEx(pstHdr,NULL,FALSE)
#endif

#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_Copy
#undef WeHdr_Copy
#endif
#define WeHdr_Copy(pstHdr, hMem) WeHdr_CopyEx(pstHdr, hMem,TRUE)
#else 
#ifdef WeHdr_Copy
#undef WeHdr_Copy
#endif
#define WeHdr_Copy(pstHdr)\
		WeHdr_CopyEx(pstHdr,NULL,FALSE)
#endif

#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_DeleteField
#undef WeHdr_DeleteField
#endif
#define WeHdr_DeleteField(pstHdr,iFieldName,hMem)\
		WeHdr_DeleteFieldEx( pstHdr, iFieldName, hMem,TRUE)
#else 
#ifdef WeHdr_DeleteField
#undef WeHdr_DeleteField
#endif
#define WeHdr_DeleteField(pstHdr, iFieldName)\
		WeHdr_DeleteFieldEx(pstHdr, iFieldName,NULL,FALSE)
#endif


#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_AddString
#undef WeHdr_AddString
#endif
#define WeHdr_AddString(pstHdr, iFieldName,pcValue, hMem)\
		WeHdr_AddStringEx(pstHdr, iFieldName, pcValue, hMem,TRUE)
#else 
#ifdef WeHdr_AddString
#undef WeHdr_AddString
#endif
#define WeHdr_AddString(pstHdr, iFieldName,pcValue)\
		WeHdr_AddStringEx(pstHdr, iFieldName, pcValue,NULL,FALSE)
#endif

#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_AddParam
#undef WeHdr_AddParam
#endif
#define WeHdr_AddParam(  pstHdr, iFieldName,pcParam,pcValue, hMem)\
		WeHdr_AddParamEx(pstHdr, iFieldName,pcParam,pcValue, hMem,TRUE)
#else 
#ifdef WeHdr_AddParam
#undef WeHdr_AddParam
#endif
#define WeHdr_AddParam(pstHdr, iFieldName,pcParam,pcValue)\
		WeHdr_AddParamEx(  pstHdr, iFieldName,pcParam,pcValue,NULL,FALSE)
#endif


#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_AddHeaderFromTextLine
#undef WeHdr_AddHeaderFromTextLine
#endif
#define WeHdr_AddHeaderFromTextLine(pstHdr,pcStr, iLen, hMem)\
		WeHdr_AddHeaderFromTextLineEx(pstHdr,pcStr, iLen, hMem,TRUE)
#else 
#ifdef WeHdr_AddHeaderFromTextLine
#undef WeHdr_AddHeaderFromTextLine
#endif
#define WeHdr_AddHeaderFromTextLine(pstHdr,pcStr, iLen)\
		WeHdr_AddHeaderFromTextLineEx(pstHdr,pcStr, iLen,NULL,FALSE)
#endif


#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_AddHeadersFromText
#undef WeHdr_AddHeadersFromText
#endif
#define WeHdr_AddHeadersFromText(pstHdr,pstText, hMem)\
		WeHdr_AddHeadersFromTextEx(pstHdr,pstText, hMem,TRUE)
#else 
#ifdef WeHdr_AddHeadersFromText
#undef WeHdr_AddHeadersFromText
#endif
#define WeHdr_AddHeadersFromText(pstHdr,pstText)\
		WeHdr_AddHeadersFromTextEx(pstHdr,pstText,NULL,FALSE)
#endif


#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_MakeString
#undef WeHdr_MakeString
#endif
#define WeHdr_MakeString(pstHdr,ppcStr, hMem)\
	   WeHdr_MakeStringEx(pstHdr,ppcStr, hMem,TRUE)
#else 
#ifdef WeHdr_MakeString
#undef WeHdr_MakeString
#endif
#define WeHdr_MakeString(pstHdr, ppcStr)\
		WeHdr_MakeStringEx(pstHdr,ppcStr,NULL,FALSE)
#endif



#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_GetUint32
#undef WeHdr_GetUint32
#endif
#define WeHdr_GetUint32( hCSC,pstHdr, iFieldName,puiVal, hMem)\
	   WeHdr_GetUint32Ex( hCSC,pstHdr, iFieldName, puiVal, hMem,TRUE)
#else 
#ifdef WeHdr_GetUint32
#undef WeHdr_GetUint32
#endif
#define WeHdr_GetUint32( hCSC,pstHdr, iFieldName, puiVal)\
		WeHdr_GetUint32Ex( hCSC,pstHdr, iFieldName, puiVal,NULL,FALSE)
#endif


#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_GetParameter
#undef WeHdr_GetParameter
#endif
#define WeHdr_GetParameter(pstHdr, iFieldName,pcParameter, hMem)\
    WeHdr_GetParameterEx(pstHdr, iFieldName,pcParameter, hMem,TRUE)
#else 
#ifdef WeHdr_GetParameter
#undef WeHdr_GetParameter
#endif
#define WeHdr_GetParameter(pstHdr, iFieldName,pcParameter)\
		WeHdr_GetParameter(pstHdr, iFieldName,pcParameter,NULL,FALSE)
#endif


#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_BuildParaList
#undef WeHdr_BuildParaList
#endif
#define WeHdr_BuildParaList( iNumParams,apcParamName,apcParamValue, hMem)\
	   WeHdr_BuildParaListEx( iNumParams,apcParamName,apcParamValue, hMem,TRUE)
#else 
#ifdef WeHdr_BuildParaList
#undef WeHdr_BuildParaList
#endif
#define WeHdr_BuildParaList( iNumParams,apcParamName,apcParamValue)\
		WeHdr_BuildParaListEx( iNumParams,apcParamName,apcParamValue,NULL,FALSE)
#endif


#ifdef _USE_MEM_HANDLE
#ifdef WeHdr_GetParams
#undef WeHdr_GetParams
#endif
#define WeHdr_GetParams(pcParamList, iMaxParams, apcParamName, apcParamValue, hMem)\
	   WeHdr_GetParamsEx(pcParamList, iMaxParams, apcParamName,apcParamValue, hMem,TRUE)
#else 
#ifdef WeHdr_GetParams
#undef WeHdr_GetParams
#endif
#define WeHdr_GetParams(pcParamList, iMaxParams,apcParamName, apcParamValue)\
		WeHdr_GetParamsEx(pcParamList, iMaxParams,apcParamName,apcParamValue,NULL,FALSE)
#endif



/*#ifdef _USE_MEM_HANDLE
St_WeSCLStrtableInfo *WeHdr_GetHeaderNames(WE_HANDLE hMem);


void WeHdr_FreeHeaderNames(St_WeSCLStrtableInfo * pSt_WeHdrNames,
                           WE_HANDLE hMem);

//extern  const St_WeSCLStrtableInfo stWeLanguages;
St_WeSCLStrtableInfo * WeHdr_GetLanguages(WE_HANDLE hMem);

void WeHdr_FreeLanguages(St_WeSCLStrtableInfo *pstWeLanguages,
                         WE_HANDLE hMem);

St_WeHdr * WeHdr_Create (WE_HANDLE hMem);

void WeHdr_Delete (St_WeHdr *pstHdr,
                   WE_HANDLE hMem);

St_WeHdr * WeHdr_Copy (St_WeHdr *pstHdr,
                       WE_HANDLE hMem);

void WeHdr_DeleteField (St_WeHdr *pstHdr,
                        WE_INT32 iFieldName,
                        WE_HANDLE hMem);
WE_BOOL WeHdr_AddString(St_WeHdr *pstHdr,
                        WE_INT32 iFieldName,
                        WE_CHAR *pcValue,
                        WE_HANDLE hMem);
WE_BOOL WeHdr_AddParam (St_WeHdr      *pstHdr,
                        WE_INT32    iFieldName,
                        const WE_CHAR *pcParam,
                        const WE_CHAR *pcValue,
                        WE_HANDLE hMem);
WE_BOOL WeHdr_AddHeaderFromTextLine(St_WeHdr *pstHdr,
                                    const WE_CHAR *pcStr,
                                    WE_INT32 iLen,
                                    WE_HANDLE hMem);
WE_BOOL WeHdr_AddHeadersFromText (St_WeHdr *pstHdr,
                                  const WE_CHAR *pstText,
                                  WE_HANDLE hMem);
WE_BOOL WeHdr_MakeString (St_WeHdr *pstHdr,
                          WE_CHAR **ppcStr,
                          WE_HANDLE hMem);
WE_BOOL WeHdr_GetUint32 (WE_HANDLE hCSC,
                         St_WeHdr *pstHdr,
                         WE_INT32 iFieldName,
                         WE_UINT32 *puiVal,
                         WE_HANDLE hMem);

WE_CHAR * WeHdr_GetParameter (St_WeHdr        *pstHdr,
                              WE_INT32        iFieldName,
                              const WE_CHAR *pcParameter,
                              WE_HANDLE hMem);
WE_CHAR * WeHdr_BuildParaList (WE_INT32        iNumParams,
                               const WE_CHAR *apcParamName[],
                               const WE_CHAR *apcParamValue[],
                               WE_HANDLE hMem);

WE_INT32 WeHdr_GetParams (const WE_CHAR *pcParamList,
                          WE_INT32 iMaxParams,
                          WE_CHAR *apcParamName[],
                          WE_CHAR *apcParamValue[],
                          WE_HANDLE hMem);
#else




//extern  const St_WeSCLStrtableInfo St_WeHdrNames;
St_WeSCLStrtableInfo *WeHdr_GetHeaderNames(void);
void WeHdr_FreeHeaderNames(St_WeSCLStrtableInfo * pSt_WeHdrNames);

//extern  const St_WeSCLStrtableInfo stWeLanguages;
St_WeSCLStrtableInfo * WeHdr_GetLanguages(void);
void WeHdr_FreeLanguages(St_WeSCLStrtableInfo *pstWeLanguages);*/
/*Define struct &  union type end*/


/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
    WeHdr_Create
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Create the header structure
ARGUMENTS PASSED:
    void
RETURN VALUE:
    The pointer to the header structure that has been created just now.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
//St_WeHdr * WeHdr_Create (void);

/*=====================================================================================
FUNCTION: 
    WeHdr_Delete
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Delete the resource of the header structure
ARGUMENTS PASSED:
    The pointer to the structure that to be deleted.
RETURN VALUE:
    void
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
//void WeHdr_Delete (St_WeHdr *pstHdr);

/*=====================================================================================
FUNCTION: 
    WeHdr_Copy
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Copy the header structure that given
ARGUMENTS PASSED:
    The pointer to the structure to be coplied.
RETURN VALUE:
    Return the pointer to the header structure that coplied.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
//St_WeHdr * WeHdr_Copy (St_WeHdr *pstHdr);

/*=====================================================================================
FUNCTION: 
    WeHdr_DeleteField
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Delete all fields of the specified kind.
ARGUMENTS PASSED:
    pstHdr: The header structure
    iFieldName: The name of the header field that to be deleted.
RETURN VALUE:
    void
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*void WeHdr_DeleteField (St_WeHdr *pstHdr,
                        WE_INT32 iFieldName);
*/
/*=====================================================================================
FUNCTION: 
    WeHdr_AddString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Add a string to the header table.
    The string is copied into the table, and will be deallocated when the header table is removed.
ARGUMENTS PASSED:
    
RETURN VALUE:
    Returns TRUE on success and FALSE on error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*WE_BOOL WeHdr_AddString(St_WeHdr *pstHdr,
                        WE_INT32 iFieldName,
                        WE_CHAR *pcValue);*/
/*=====================================================================================
FUNCTION: 
    WeHdr_AddParam
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Add a parameter value to a header that is already in the table. The string is copied, 
    and will be deallocated when the header table is deleted.
ARGUMENTS PASSED:
    pstHdr: The header structure
    iFieldName: The field name in the structure in which the pararmeter to be add.
RETURN VALUE:
    Returns TRUE on success, or FALSE on error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*WE_BOOL WeHdr_AddParam (St_WeHdr      *pstHdr,
                        WE_INT32    iFieldName,
                        const WE_CHAR *pcParam,
                        const WE_CHAR *pcValue);
*/
/*=====================================================================================
FUNCTION: 
    WeHdr_AddHeaderFromTextLine
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Parses one line of text as an HTTP header, and adds the relevant  content to the 
    header table. The parameter 'pcStr' is a null-terminated character string holding
    one line of text, and 'iLen' is the number of characters in 'pcStr'.
ARGUMENTS PASSED:
    
RETURN VALUE:
    Returns TRUE on success and FALSE on error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*WE_BOOL WeHdr_AddHeaderFromTextLine(St_WeHdr *pstHdr,
                                    const WE_CHAR *pcStr,
                                    WE_INT32 iLen);*/
/*=====================================================================================
FUNCTION: 
    WeHdr_AddHeadersFromText
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Given a text string that holds a number of lines, each terminated by <CR><LF> 
    (except perhaps the last one), this function reads these lines and adds the header 
    value to the table.
ARGUMENTS PASSED:
    pstHdr: The header structure
    pstText: The content of the header to be added.
RETURN VALUE:
    Returns TRUE on success and FALSE on error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*WE_BOOL WeHdr_AddHeadersFromText (St_WeHdr *pstHdr,
                                  const WE_CHAR *pstText);*/

/*=====================================================================================
FUNCTION: 
    WeHdr_MakeString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Make a string out of the contents of a header structure.
ARGUMENTS PASSED:
    pstHdr: The header structure
    ppcStr: The address that is used to store the header string made just now.
RETURN VALUE:
    Returns TRUE on success and FALSE on error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*WE_BOOL WeHdr_MakeString (St_WeHdr *pstHdr,
                          WE_CHAR **ppcStr);
*/
/*=====================================================================================
FUNCTION: 
    WeHdr_GetUint32
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Get a WE_UINT32 value from the first element with the given header name.
ARGUMENTS PASSED:
    hCSC:   Pointer to the CSC Interface 
    pstHdr: The header structure
    iFieldName: The header field name
    puiVal: The address that is used to store the value got from the header structure
RETURN VALUE:
    Returns TRUE if a header element was found, and FALSE otherwise.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*WE_BOOL WeHdr_GetUint32 (WE_HANDLE hCSC,
                         St_WeHdr *pstHdr,
                         WE_INT32 iFieldName,
                         WE_UINT32 *puiVal);*/

/*=====================================================================================
FUNCTION: 
    WeHdr_GetParameter
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    It get the parameter value of the field name in the header structure.
ARGUMENTS PASSED:
    pstHdr: The header structure
    iFieldName: The field name in the structure
    pcParameter: Pointer to the parameter name whose value want to be got.
RETURN VALUE:
    Success: Returns the value of the parameter
    Else: Returns NULL
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*WE_CHAR * WeHdr_GetParameter (St_WeHdr        *pstHdr,
                              WE_INT32        iFieldName,
                              const WE_CHAR *pcParameter);*/

/*=====================================================================================
FUNCTION: 
    WeHdr_BuildParaList
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Converts the strings param_name and param_value parameters to be 
    uniquely identified according to the HTTP/1.1 header encoding. 
    This implies that the name parameters are token strings, as defined 
    by HTTP/1.1, i.e. no presence of CTL's (US-ASCII 0x00 - 0x1F, 0x7F) 
    or reserved separators below: 
    
    '('  ')'  '<'  '>'  '@'  
    ','  ';'  ':'  '\'  '"'  
    '/'  '['  ']'  '?'  '='
    '{'  '}'  SPACE (0x20)  HTAB (0x09)
    
    IF A FORBIDDEN CHARACTER IS ENCOUNTERED INSIDE A NAME ATTRIBUTE 
    THE WHOLE PARAMETER PAIR IS DISCARDED!
  
    This restriction does not apply to the strings in the value array. 
    Values are transformed to quoted strings ( val(ue -> "val(ue" ) 
    with quoted pairs ( val"ue -> "val\"ue" ) if required.

ARGUMENTS PASSED:
    iNumParams: The number of the parameters
    apcParamName: Store the parameter name given
    apcParamValue: Store the parameter value given
RETURN VALUE:
    A string with the parameter pairs concatenated in consecutive order, 
    e.g :param_name[0] = name1, param_value[0] = NULL
           param_name[1] = name2, param_value[1] = val(ue2
           returns  name1;name2="val(ue2"
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    The caller is responsible for for deallocating the returned memory buffer.
=====================================================================================*/
/*WE_CHAR * WeHdr_BuildParaList (WE_INT32        iNumParams,
                               const WE_CHAR *apcParamName[],
                               const WE_CHAR *apcParamValue[]);*/

/*=====================================================================================
FUNCTION: 
    WeHdr_GetParams
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Unpacks the parameter list into a name array and a corresponding value array.
    If a parameter name's corresponding value pair is not encountered its value 
    string pointer will point at NULL. This is the reverse function of
    WeHdr_BuildParameterString(). See its description header for more details.

ARGUMENTS PASSED:
    pcParamList: The parameter list
    iMaxParams: The number of the parameter to be get
    apcParamName: To store the parameter name got from the parameter list
    apcParamValue: To store the parameter value 
RETURN VALUE:
    The number of parameters that were found in the string, if this number 
    exceeds the iMaxParams only the given number of parameters was 
    retrieved. The operation have to be redone with array sizes the returned 
    int tells in order to retrieve all parameters. 
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    1. The caller is responsible for for deallocating the memory buffers attached 
        to the apcParamName and apcParamValue arrays after the function returns. 
    2. The arrays should not contain any pointers when used in this function. All 
        array pointers will be reset by the function, if an array slot points out a 
        memory buffer it might result in a leak!
=====================================================================================*/
/*WE_INT32 WeHdr_GetParams (const WE_CHAR *pcParamList,
                          WE_INT32 iMaxParams,
                          WE_CHAR *apcParamName[],
                          WE_CHAR *apcParamValue[]);
#endif*/
#endif/*endif WE_HDR_H*/
/*--------------------------END-----------------------------*/


