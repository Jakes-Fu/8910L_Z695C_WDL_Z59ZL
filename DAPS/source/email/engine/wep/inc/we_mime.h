/*=====================================================================================

    HEADER NAME : we_mime.h
    MODULE NAME : MIME

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
    2006-5-23   Guohong				       create we_mime.h

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
#ifndef WE_MIME_H
#define WE_MIME_H

/*******************************************************************************
*   Include File Section
*******************************************************************************/
/*Include System head file*/
/*Include Program Global head file*/

#ifdef __cplusplus
extern "C" 
{
#endif

#ifndef WE_DEF_H
#include "we_def.h"
#endif
#include "we_scl.h"


/*Include Module head file*/

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/

/*
 * Media types that have a pre-defined integer representation.
 * OMNA "Well-Known Values".
 */
#define G_WE_MIME_TYPE_ANY_ANY                                       0x00
#define G_WE_MIME_TYPE_TEXT_ANY                                      0x01
#define G_WE_MIME_TYPE_TEXT_HTML                                     0x02
#define G_WE_MIME_TYPE_TEXT_PLAIN                                    0x03
#define G_WE_MIME_TYPE_TEXT_X_HDML                                   0x04
#define G_WE_MIME_TYPE_TEXT_X_TTML                                   0x05
#define G_WE_MIME_TYPE_TEXT_X_VCALENDAR                              0x06
#define G_WE_MIME_TYPE_TEXT_X_VCARD                                  0x07
#define G_WE_MIME_TYPE_TEXT_VND_WAP_WML                              0x08
#define G_WE_MIME_TYPE_TEXT_VND_WAP_WMLSCRIPT                        0x09
#define G_WE_MIME_TYPE_TEXT_VND_WAP_CHANNEL                          0x0a
#define G_WE_MIME_TYPE_MULTIPART_ANY                                 0x0b
#define G_WE_MIME_TYPE_MULTIPART_MIXED                               0x0c
#define G_WE_MIME_TYPE_MULTIPART_FORM_DATA                           0x0d
#define G_WE_MIME_TYPE_MULTIPART_BYTERANGES                          0x0e
#define G_WE_MIME_TYPE_MULTIPART_ALTERNATIVE                         0x0f
#define G_WE_MIME_TYPE_APPLICATION_ANY                               0x10
#define G_WE_MIME_TYPE_APPLICATION_JAVA_VM                           0x11
#define G_WE_MIME_TYPE_APPLICATION_X_WWW_FORM_URLENCODED             0x12
#define G_WE_MIME_TYPE_APPLICATION_X_HDMLC                           0x13
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_WMLC                      0x14
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_WMLSCRIPTC                0x15
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_CHANNELC                  0x16
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_UAPROF                    0x17
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_WTLS_CA_CERTIFICATE       0x18
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_WTLS_USER_CERTIFICATE     0x19
#define G_WE_MIME_TYPE_APPLICATION_X_X509_CA_CERT                    0x1a
#define G_WE_MIME_TYPE_APPLICATION_X_X509_USER_CERT                  0x1b
#define G_WE_MIME_TYPE_IMAGE_ANY                                     0x1c
#define G_WE_MIME_TYPE_IMAGE_GIF                                     0x1d
#define G_WE_MIME_TYPE_IMAGE_JPEG                                    0x1e
#define G_WE_MIME_TYPE_IMAGE_TIFF                                    0x1f
#define G_WE_MIME_TYPE_IMAGE_PNG                                     0x20
#define G_WE_MIME_TYPE_IMAGE_VND_WAP_WBMP                            0x21
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_ANY             0x22
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_MIXED           0x23
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_FORM_DATA       0x24
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_BYTERANGES      0x25
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_ALTERNATIVE     0x26
#define G_WE_MIME_TYPE_APPLICATION_XML                               0x27
#define G_WE_MIME_TYPE_TEXT_XML                                      0x28
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_WBXML                     0x29
#define G_WE_MIME_TYPE_APPLICATION_X_X968_CROSS_CERT                 0x2a
#define G_WE_MIME_TYPE_APPLICATION_X_X968_CA_CERT                    0x2b
#define G_WE_MIME_TYPE_APPLICATION_X_X968_USER_CERT                  0x2c
#define G_WE_MIME_TYPE_TEXT_VND_WAP_SI                               0x2d
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_SIC                       0x2e
#define G_WE_MIME_TYPE_TEXT_VND_WAP_SL                               0x2f
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_SLC                       0x30
#define G_WE_MIME_TYPE_TEXT_VND_WAP_CO                               0x31
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_COC                       0x32
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_RELATED         0x33
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_SIA                       0x34
#define G_WE_MIME_TYPE_TEXT_VND_WAP_CONNECTIVITY_XML                 0x35
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_CONNECTIVITY_WBXML        0x36
#define G_WE_MIME_TYPE_APPLICATION_PKCS7_MIME                        0x37
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_HASHED_CERTIFICATE        0x38
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_SIGNED_CERTIFICATE        0x39
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_CERT_RESPONSE             0x3a
#define G_WE_MIME_TYPE_APPLICATION_XHTML_XML                         0x3b
#define G_WE_MIME_TYPE_APPLICATION_WML_XML                           0x3c
#define G_WE_MIME_TYPE_TEXT_CSS                                      0x3d
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_MMS_MESSAGE               0x3e
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_ROLLOVER_CERTIFICATE      0x3f
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_LOCC_WBXML                0x40
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_LOC_XML                   0x41
#define G_WE_MIME_TYPE_APPLICATION_VND_SYNCML_DM_WBXML               0x42
#define G_WE_MIME_TYPE_APPLICATION_VND_SYNCML_DM_XML                 0x43
#define G_WE_MIME_TYPE_APPLICATION_VND_SYNCML_NOTIFICATION           0x44
#define G_WE_MIME_TYPE_APPLICATION_VND_WAP_XHTML_XML                 0x45
#define G_WE_MIME_TYPE_APPLICATION_VND_WV_CSP_CIR                    0x46
#define G_WE_MIME_TYPE_APPLICATION_VND_OMA_DD_XML                    0x47
#define G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE               0x48
#define G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT               0x49
#define G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML            0x4a
#define G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML          0x4b
#define G_WE_MIME_TYPE_APPLICATION_VND_WV_CSP_XML                    0x4c
#define G_WE_MIME_TYPE_APPLICATION_VND_WV_CSP_WBXML                  0x4d
                                                                                       
/*
 * OMNA "Registered Values"
 */
                                                                                       
#define G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_CACHEOP_WBXML            0x0201
#define G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_SIGNAL                   0x0202
#define G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_ALERT_WBXML              0x0203
#define G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_LIST_WBXML               0x0204
#define G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_LISTCMD_WBXML            0x0205
#define G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_CHANNEL_WBXML            0x0206
#define G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_PROVISIONING_STATUS_URI  0x0207
#define G_WE_MIME_TYPE_X_WAP_MULTIPART_VND_UPLANET_HEADER_SET           0x0208
#define G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_BEARER_CHOICE_WBXML      0x0209
#define G_WE_MIME_TYPE_APPLICATION_VND_PHONECOM_MMC_WBXML               0x020a
#define G_WE_MIME_TYPE_APPLICATION_VND_NOKIA_SYNCSET_WBXML              0x020b
#define G_WE_MIME_TYPE_IMAGE_X_UP_WPNG                                  0x020c
#define G_WE_MIME_TYPE_APPLICATION_IOTA_MMC_WBXML                       0x0300
#define G_WE_MIME_TYPE_APPLICATION_IOTA_MMC                             0x0301

/*
 * Other media types that have a pre-defined integer representation.
 * These are pre-defined internally by WE and used to enable the 
 * parser to handle these media types.
 */

#define G_WE_MIME_TYPE_AUDIO_ANY                                        0xa0
#define G_WE_MIME_TYPE_AUDIO_MID                                        0xa1 
#define G_WE_MIME_TYPE_MP3                                              0xa2 
#define G_WE_MIME_TYPE_WAV                                              0xa3
#define G_WE_MIME_TYPE_APPLICATION_X_VND_MMS_TEMPLATE_TEXTS             0xa4
#define G_WE_MIME_TYPE_APPLICATION_X_WAP_PROV_BROWSER_SETTINGS          0xa5
#define G_WE_MIME_TYPE_APPLICATION_X_WAP_PROV_BROWSER_BOOKMARKS         0xa6
#define G_WE_MIME_TYPE_APPLICATION_X_WAP_PROV_SYNCSET_XML               0xa7
#define G_WE_MIME_TYPE_APPLICATION_X_WAP_PROV_SYNCSET_WBXML             0xa8
#define G_WE_MIME_TYPE_APPLICATION_SMIL                                 0xa9
#define G_WE_MIME_TYPE_SVG                                              0xaa
#define G_WE_MIME_TYPE_BMP                                              0xab                                                  
#define G_WE_MIME_TYPE_VIDEO_MPEG                                       0xac
#define G_WE_MIME_TYPE_APPLICATION_VND_SIAF                             0xad
#define G_WE_MIME_TYPE_APPLICATION_X_SIAF                               0xae
#define G_WE_MIME_TYPE_AUDIO_AMR                                        0xaf
#define G_WE_MIME_TYPE_APPLICATION_X_SHOCKWAVE_FLASH                    0xb0
#define G_WE_MIME_TYPE_APPLICATION_ZIP                                  0xb1
#define G_WE_MIME_TYPE_AUDIO_SP_MIDI                                    0xb2
#define G_WE_MIME_TYPE_APPLICATION_JAVA_ARCHIVE                         0xb3
#define G_WE_MIME_TYPE_TEXT_VND_SUN_JZME_APP                            0xb4
#define G_WE_MIME_TYPE_VIDEO_3GPP2                                      0xb5
#define G_WE_MIME_TYPE_AUDIO_AAC                                        0xb6
#define G_WE_MIME_TYPE_AUDIO_DLS                                        0xb7
#define G_WE_MIME_TYPE_VIDEO_SVGZ                                       0xb8
#define G_WE_MIME_TYPE_VIDEO_URL                                        0xb9
#define G_WE_MIME_TYPE_AUDIO_QCP                                        0xc0
#define G_WE_MIME_TYPE_AUDIO_VND_QCELP                                  0xc1
#define G_WE_MIME_TYPE_AUDIO_SPF                                        0xc2
#define G_WE_MIME_TYPE_VIDEO_PMD                                        0xc3
#define G_WE_MIME_TYPE_3GP                                              0xc4    
#define G_WE_MIME_TYPE_XMF                                              0xc5
#define G_WE_MIME_TYPE_MMF 	                 	                 	    0xc6                                                                        
#define G_WE_MIME_TYPE_MIDI                                             0xc7                                       
#define G_WE_MIME_TYPE_MP4                                              0xc8
#define G_WE_MIME_TYPE_RM                                               0xc9
#define G_WE_MIME_TYPE_RAM                                              0xca
#define G_WE_MIME_TYPE_SDP                                              0xcb
#define G_WE_MIME_TYPE_IMY                                              0xcc
#define G_WE_MIME_TYPE_VIDEO_X_MS_WMV									0xcd
#define G_WE_MIME_TYPE_AUDIO_X_MS_WMA									0xce
#define G_WE_MIME_TYPE_VIDEO_AVI										0xcf
#define G_WE_MIME_TYPE_VIDEO_MOV										0xd0
#define G_WE_MIME_TYPE_APPLICATION_RAR									0xd1
#define G_WE_MIME_TYPE_APPLICATION_PDF									0xd2
#define G_WE_MIME_TYPE_VMD												0xd3
                                                                                       
#define    G_WE_MIME_TYPE_UNKNOWN                                                0xff                                                                        
           
                              
            
/*Define Constant Macro end*/

/*Define Macro Function start*/
/*Define Macro Function end*/

/*******************************************************************************
*   Type Define Section
*******************************************************************************/

/*Define base type start*/
//extern const St_WeSCLStrtableInfo We_Mime_Types;


WE_CHAR ** WeMime_Ext2MimeExEx(const WE_CHAR *pcExt,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_Ext2MimeEx
#undef WeMime_Ext2MimeEx
#endif
#define WeMime_Ext2MimeEx(hMem) WeMime_Ext2MimeExEx(hMem,TRUE)
#else 
#ifdef WeMime_Ext2MimeEx
#undef WeMime_Ext2MimeEx
#endif
#define WeMime_Ext2MimeEx( ) WeMime_Ext2MimeExEx(NULL,FALSE)
#endif

St_WeSCLStrTableEntry *WeMime_GetExtStringsEx(WE_HANDLE hMem, WE_BOOL bMemMgrr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_GetExtStrings
#undef WeMime_GetExtStrings
#endif
#define WeMime_GetExtStrings(hMem) WeMime_GetExtStringsEx(hMem,TRUE)
#else 
#ifdef WeMime_GetExtStrings
#undef WeMime_GetExtStrings
#endif
#define WeMime_GetExtStrings(void) WeMime_GetExtStringsEx(NULL,FALSE)
#endif


void WeMime_FreeExtStringsEx(St_WeSCLStrTableEntry *pstMimeExtStrings,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_FreeExtStrings
#undef WeMime_FreeExtStrings
#endif
#define WeMime_FreeExtStrings(pstMimeExtStrings, hMem) WeMime_FreeExtStringsEx(pstMimeExtStrings, hMem,TRUE)
#else 
#ifdef WeMime_FreeExtStrings
#undef WeMime_FreeExtStrings
#endif
#define WeMime_FreeExtStrings(pstMimeExtStrings) WeMime_FreeExtStringsEx(pstMimeExtStrings,NULL,FALSE)
#endif

St_WeSCLStrTableEntry *WeMime_GetTypeStringsEx(WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_GetTypeStrings
#undef WeMime_GetTypeStrings
#endif
#define WeMime_GetTypeStrings( hMem) WeMime_GetTypeStringsEx( hMem,TRUE)
#else 
#ifdef WeMime_GetTypeStrings
#undef WeMime_GetTypeStrings
#endif
#define WeMime_GetTypeStrings() WeMime_GetTypeStringsEx(NULL,FALSE)
#endif

void WeMime_FreeTypeStringsEx(St_WeSCLStrTableEntry *pstWeMimeTypeStrings,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_FreeTypeStrings
#undef WeMime_FreeTypeStrings
#endif
#define WeMime_FreeTypeStrings( pstWeMimeTypeStrings, hMem) WeMime_FreeTypeStringsEx(pstWeMimeTypeStrings, hMem,TRUE)
#else 
#ifdef WeMime_FreeTypeStrings
#undef WeMime_FreeTypeStrings
#endif
#define WeMime_FreeTypeStrings(pstWeMimeTypeStrings) WeMime_FreeTypeStringsEx(pstWeMimeTypeStrings,NULL,FALSE)
#endif

St_WeSCLStrtableInfo *WeMime_GetMimeExtensionsEx(WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_GetMimeExtensions
#undef WeMime_GetMimeExtensions
#endif
#define WeMime_GetMimeExtensions( hMem) WeMime_GetMimeExtensionsEx( hMem,TRUE)
#else 
#ifdef WeMime_GetMimeExtensions
#undef WeMime_GetMimeExtensions
#endif
#define WeMime_GetMimeExtensions( ) WeMime_GetMimeExtensionsEx(NULL,FALSE)
#endif


void WeMime_FreeMimeExtensionsEx(St_WeSCLStrtableInfo *pstWeMimeExtensions,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_FreeMimeExtensions
#undef WeMime_FreeMimeExtensions
#endif
#define WeMime_FreeMimeExtensions(pstWeMimeExtensions, hMem) WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions, hMem,TRUE)
#else 
#ifdef WeMime_FreeMimeExtensions
#undef WeMime_FreeMimeExtensions
#endif
#define WeMime_FreeMimeExtensions(pstWeMimeExtensions) WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,NULL,FALSE)
#endif


St_WeSCLStrtableInfo *WeMime_GetMimeTypesEx(WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_GetMimeTypes
#undef WeMime_GetMimeTypes
#endif
#define WeMime_GetMimeTypes( hMem) WeMime_GetMimeTypesEx( hMem,TRUE)
#else 
#ifdef WeMime_GetMimeTypes
#undef WeMime_GetMimeTypes
#endif
#define WeMime_GetMimeTypes() WeMime_GetMimeTypesEx(NULL,FALSE)
#endif

void WeMime_FreeMimeTypesEx(St_WeSCLStrtableInfo *pstWeMimeTypes,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_FreeMimeTypes
#undef WeMime_FreeMimeTypes
#endif
#define WeMime_FreeMimeTypes(pstWeMimeTypes, hMem) WeMime_FreeMimeTypesEx(pstWeMimeTypes, hMem,TRUE)
#else 
#ifdef WeMime_FreeMimeTypes
#undef WeMime_FreeMimeTypes
#endif
#define WeMime_FreeMimeTypes(pstWeMimeTypes) WeMime_FreeMimeTypesEx(pstWeMimeTypes,NULL,FALSE)
#endif


const WE_CHAR * WeMime_Ext2Mime_Ex (const WE_CHAR *pcExt,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_Ext2Mime
#undef WeMime_Ext2Mime
#endif
#define WeMime_Ext2Mime(pcExt, hMem) WeMime_Ext2Mime_Ex(pcExt, hMem,TRUE)
#else 
#ifdef WeMime_Ext2Mime
#undef WeMime_Ext2Mime
#endif
#define WeMime_Ext2Mime(pcExt) WeMime_Ext2Mime_Ex(pcExt,NULL,FALSE)
#endif

const WE_CHAR * WeMime_Mime2Ext_Ex (const WE_CHAR *pcMime,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_Mime2Ext
#undef WeMime_Mime2Ext
#endif
#define WeMime_Mime2Ext(pcMime, hMem) WeMime_Mime2Ext_Ex(pcMime, hMem,TRUE)
#else 
#ifdef WeMime_Mime2Ext
#undef WeMime_Mime2Ext
#endif
#define WeMime_Mime2Ext(pcMime) WeMime_Mime2Ext_Ex(pcMime,NULL,FALSE)
#endif


WE_INT32 WeMime_Mime2IntEx (const WE_CHAR *pcMime,WE_HANDLE hMem,WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_Mime2Int
#undef WeMime_Mime2Int
#endif
#define WeMime_Mime2Int(pcMime, hMem) WeMime_Mime2IntEx(pcMime, hMem,TRUE)
#else 
#ifdef WeMime_Mime2Int
#undef WeMime_Mime2Int
#endif
#define WeMime_Mime2Int(pcMime) WeMime_Mime2IntEx(pcMime,NULL,FALSE)
#endif

const WE_CHAR * WeMime_Int2MimeEx (WE_INT32 iKnownVal,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_Int2Mime
#undef WeMime_Int2Mime
#endif
#define WeMime_Int2Mime( iKnownVal, hMem) WeMime_Int2MimeEx( iKnownVal, hMem,TRUE)
#else 
#ifdef WeMime_Int2Mime
#undef WeMime_Int2Mime
#endif
#define WeMime_Int2Mime( iKnownVal) WeMime_Int2MimeEx( iKnownVal,NULL,FALSE)
#endif

WE_INT32 WeMime_Ext2IntEx (const WE_CHAR *pcExt,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_Ext2Int
#undef WeMime_Ext2Int
#endif
#define WeMime_Ext2Int(pcExt, hMem) WeMime_Ext2IntEx(pcExt, hMem,TRUE)
#else 
#ifdef WeMime_Ext2Int
#undef WeMime_Ext2Int
#endif
#define WeMime_Ext2Int(pcExt) WeMime_Ext2IntEx(pcExt,NULL,FALSE)
#endif


WE_CHAR ** WeMime_Mime2ExtExEx (const WE_CHAR *pcMime,WE_HANDLE hMem , WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_Mime2ExtEx
#undef WeMime_Mime2ExtEx
#endif
#define WeMime_Mime2ExtEx(pcMime, hMem) WeMime_Mime2ExtExEx(pcMime, hMem,TRUE)
#else 
#ifdef WeMime_Mime2ExtEx
#undef WeMime_Mime2ExtEx
#endif
#define WeMime_Mime2ExtEx(pcMime) WeMime_Mime2ExtExEx(pcMime,NULL,FALSE)
#endif

WE_CHAR ** WeMime_Ext2MimeExEx (const WE_CHAR *pcExt,WE_HANDLE hMem, WE_BOOL bMemMgr);

#ifdef _USE_MEM_HANDLE
#ifdef WeMime_Ext2MimeEx
#undef WeMime_Ext2MimeEx
#endif
#define WeMime_Ext2MimeEx(pcExt, hMem) WeMime_Ext2MimeExEx(pcExt, hMem,TRUE)
#else 
#ifdef WeMime_Ext2MimeEx
#undef WeMime_Ext2MimeEx
#endif
#define WeMime_Ext2MimeEx(pcExt) WeMime_Ext2MimeExEx(pcExt,NULL,FALSE)
#endif

/*Define base type end*/

/*Define Enum type start*/
/*Define Enum type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
/*Define struct &  union type end*/

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
    WeMime_Ext2Mime
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Returns the corresponding mime type for a file extension 
ARGUMENTS PASSED:
    pcExt: The extension file name string
RETURN VALUE:
    The mime type string that matches the extension file name.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*const WE_CHAR * WeMime_Ext2Mime (const WE_CHAR *pcExt);*/

/*=====================================================================================
FUNCTION: 
    WeMime_Mime2Ext
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Returns the corresponding file extension for a mime type
ARGUMENTS PASSED:
    pcMime: The MIME type string
RETURN VALUE:
    The extension file name string that matches this MIME type.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*const WE_CHAR * WeMime_Mime2Ext (const WE_CHAR *pcMime);*/

/*=====================================================================================
FUNCTION: 
    WeMime_Mime2Int
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Returns the integer representation for the mime type
ARGUMENTS PASSED:
    pcMime: The MIME type string
RETURN VALUE:
    Return the uinque integer that indicates this MIME type
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*WE_INT32 WeMime_Mime2Int (const WE_CHAR *pcMime);*/

/*=====================================================================================
FUNCTION: 
    WeMime_Int2Mime
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Returns the mime type given an integer representation.
ARGUMENTS PASSED:
    iKnownVal: The integer that indicates the MIME type well-known
RETURN VALUE:
    It returns the MIME type string that iKonwnVal indicates.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
/*const WE_CHAR * WeMime_Int2Mime (WE_INT32 iKnownVal);*/

/*=====================================================================================
FUNCTION: 
    WeMime_Ext2Int
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Returns the integer representation for the file extension
ARGUMENTS PASSED:
    pcExt: The extension file name string
RETURN VALUE:
    It returns an uinque integer that indicates the MIME type which matches the extension file name.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    That it is the integer representation of the mime type corresponding
    to the extension that is returned.
=====================================================================================*/
/*WE_INT32 WeMime_Ext2Int (const WE_CHAR *pcExt);*/


/*=====================================================================================
FUNCTION: 
WeMime_Mime2ExtEx
CREATE DATE: 
2006-12-27
AUTHOR: 
Guohong
DESCRIPTION:
Returns the corresponding file extension array for a mime type
ARGUMENTS PASSED:
pcMime: The MIME type string
RETURN VALUE:
The extension file name string array that matches this MIME type.
The string array would end with a NULL string.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
    
IMPORTANT NOTES:
It is the callers responsibility to deallocate the returned string array.	  
=====================================================================================*/
/*WE_CHAR ** WeMime_Mime2ExtEx (const WE_CHAR *pcMime);*/

/*=====================================================================================
FUNCTION: 
WeMime_Ext2MimeEx
CREATE DATE: 
2006-12-27
AUTHOR: 
Caiyi
DESCRIPTION:
Returns the corresponding mime type array for a file extension 
ARGUMENTS PASSED:
pcExt: The extension file name string
RETURN VALUE:
The mime type string array that matches the extension file name.
The string array would end with a NULL string.
USED GLOBAL VARIABLES:

USED STATIC VARIABLES:
  
CALL BY:
    
IMPORTANT NOTES:
It is the callers responsibility to deallocate the returned string array.
	  
=====================================================================================*/
/*WE_CHAR ** WeMime_Ext2MimeEx (const WE_CHAR *pcExt);*/


//#endif /* _USE_MEM_HANDLE */

#ifdef __cplusplus
}
#endif

#endif/*endif WE_MIME_H*/
/*--------------------------END-----------------------------*/

