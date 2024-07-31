/*=====================================================================================
    FILE NAME   : we_mime.c
    MODULE NAME : MIME


    GENERAL DESCRIPTION
    This file define function operating memory.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2006-06-08       Guohong                        create we_mime.c

=====================================================================================*/

/***************************************************************************************
*   Include File Section
****************************************************************************************/
/*Include System head file*/

/*Include Program Global head file*/
//#include "we_mem.h"
/*Include Module head file*/
/*Include Private head file*/
#include "we_mime.h"
#include "we_scl.h"
#include "we_mem.h"
#include "tf3rd_for_HZ.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/
#define WE_MIME_DEFAULT_MIME "application/x-techsoft-unknown-mime"

#define WE_EXT_HASH_TABLE_SIZE                 103
#define WE_EXT_STRING_TABLE_SIZE               73

const WE_UCHAR acMimeExtHashTable[WE_EXT_HASH_TABLE_SIZE] = 
{
		0x02, 0x21, 0x0D, 0x17, 0x00, 0x26, 0x10, 0x36, 0xFF, 0x01, 
		0xFF, 0xFF, 0x16, 0x18, 0x09, 0x3D, 0xFF, 0x2B, 0x27, 0x24, 
		0x2C, 0x34, 0x1C, 0x11, 0x15, 0x3C, 0x12, 0xFF, 0xFF, 0x31, 
		0xFF, 0xFF, 0x08, 0x41, 0x42, 0x2F, 0xFF, 0x3F, 0x32, 0x30, 
		0x1F, 0xFF, 0x05, 0x29, 0x0E, 0x3B, 0xFF, 0x46, 0xFF, 0x2E, 
		0x1A, 0x0C, 0x37, 0xFF, 0xFF, 0x03, 0xFF, 0x40, 0x33, 0x38, 
		0x35, 0xFF, 0xFF, 0x23, 0x07, 0x04, 0x20, 0x0A, 0x39, 0x47, 
		0x1D, 0x2D, 0x3A, 0x22, 0x1B, 0xFF, 0x25, 0x14, 0x13, 0x06, 
		0x19, 0xFF, 0x3E, 0xFF, 0xFF, 0x45, 0xFF, 0xFF, 0x1E, 0xFF, 
		0x2A, 0xFF, 0x0F, 0x48, 0x43, 0x0B, 0xFF, 0xFF, 0xFF, 0xFF,    
		0x28, 0xFF, 0x44             
};//we_mime_ext_hash_table


const WE_CHAR  acMimeExtStr[WE_EXT_STRING_TABLE_SIZE][6]=
{
	"htm", 
	"html",
	"shtml",
	"text",
	"txt", 
	"tex", 
	"asc", 
	"vcs",    
	"vcf",    
	"wml",    
	"wmls",   
	"class",  
	"wmlc",   
	"wmlsc",  
	"cer",    
	"usr",    
	"gif",    
	"jpg", 
	"jpeg",
	"tif", 
	"tiff",
	"png",    
	"wbmp",
	"wbm", 
	"xml", 
	"xml", 
	"xhtml",  
	"css",    
	"html",   
	"drm", 
	"dm",  
	"dcf",    
	"mid",    
	"mp3",   
	"smi",   
	"wav",   
	"svg",   
	"bmp",   
	"mpeg",
	"mpg", 
	"mpe", 
	"amr",    
	"swf",    
	"zip",    
	"jar", 
	"jad", 
	"3g2", 
	"m4a",
	"aac",    
	"mdls",   
	"svgz",   
	"url",    
	"qcp",    
	"qcelp",  
	"spf",    
	"pmd",    
	"3gp",    
	"xmf",     
	"mmf",    
	"midi",   
	"mp4",    
	"rm",     
	"ram",    
	"sdp",    
	"imy",    
	"wpng",
    "wmv",
	"wma",
	"avi",
	"mov",
	"rar",
	"pdf",
	"vmd"

};
const WE_INT32 aiMimeExtStr[WE_EXT_STRING_TABLE_SIZE]=
{
    G_WE_MIME_TYPE_TEXT_HTML,                        
    G_WE_MIME_TYPE_TEXT_HTML,                        
    G_WE_MIME_TYPE_TEXT_HTML,                        
    G_WE_MIME_TYPE_TEXT_PLAIN,                       
    G_WE_MIME_TYPE_TEXT_PLAIN,                       
    G_WE_MIME_TYPE_TEXT_PLAIN,                       
    G_WE_MIME_TYPE_TEXT_PLAIN,                       
    G_WE_MIME_TYPE_TEXT_X_VCALENDAR,                 
    G_WE_MIME_TYPE_TEXT_X_VCARD,                     
    G_WE_MIME_TYPE_TEXT_VND_WAP_WML,                 
    G_WE_MIME_TYPE_TEXT_VND_WAP_WMLSCRIPT,           
    G_WE_MIME_TYPE_APPLICATION_JAVA_VM,              
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_WMLC,         
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_WMLSCRIPTC,   
    G_WE_MIME_TYPE_APPLICATION_X_X509_CA_CERT,       
    G_WE_MIME_TYPE_APPLICATION_X_X509_USER_CERT,     
    G_WE_MIME_TYPE_IMAGE_GIF,                        
    G_WE_MIME_TYPE_IMAGE_JPEG,                       
    G_WE_MIME_TYPE_IMAGE_JPEG,                       
    G_WE_MIME_TYPE_IMAGE_TIFF,                       
    G_WE_MIME_TYPE_IMAGE_TIFF,                       
    G_WE_MIME_TYPE_IMAGE_PNG,                        
    G_WE_MIME_TYPE_IMAGE_VND_WAP_WBMP,               
    G_WE_MIME_TYPE_IMAGE_VND_WAP_WBMP,               
    G_WE_MIME_TYPE_APPLICATION_XML,                  
    G_WE_MIME_TYPE_TEXT_XML,                         
    G_WE_MIME_TYPE_APPLICATION_XHTML_XML,            
    G_WE_MIME_TYPE_TEXT_CSS,                         
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_XHTML_XML,    
    G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE,  
    G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE,  
    G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT,  
    G_WE_MIME_TYPE_AUDIO_MID,                        
    G_WE_MIME_TYPE_MP3,                                      
    G_WE_MIME_TYPE_APPLICATION_SMIL,                 
    G_WE_MIME_TYPE_WAV,                                      
    G_WE_MIME_TYPE_SVG,                                      
    G_WE_MIME_TYPE_BMP,                                      
    G_WE_MIME_TYPE_VIDEO_MPEG,                       
    G_WE_MIME_TYPE_VIDEO_MPEG,                       
    G_WE_MIME_TYPE_VIDEO_MPEG,                       
    G_WE_MIME_TYPE_AUDIO_AMR,                        
    G_WE_MIME_TYPE_APPLICATION_X_SHOCKWAVE_FLASH,    
    G_WE_MIME_TYPE_APPLICATION_ZIP,	                 
    G_WE_MIME_TYPE_APPLICATION_JAVA_ARCHIVE,         
    G_WE_MIME_TYPE_TEXT_VND_SUN_JZME_APP,            
    G_WE_MIME_TYPE_VIDEO_3GPP2,                      
    G_WE_MIME_TYPE_AUDIO_AAC, 	                     
    G_WE_MIME_TYPE_AUDIO_AAC, 	                             
    G_WE_MIME_TYPE_AUDIO_DLS,                        
    G_WE_MIME_TYPE_VIDEO_SVGZ,                       
    G_WE_MIME_TYPE_VIDEO_URL,                        
    G_WE_MIME_TYPE_AUDIO_QCP,                        
    G_WE_MIME_TYPE_AUDIO_VND_QCELP,                  
    G_WE_MIME_TYPE_AUDIO_SPF,                        
    G_WE_MIME_TYPE_VIDEO_PMD,                        
    G_WE_MIME_TYPE_3GP,                                                  
    G_WE_MIME_TYPE_XMF,                              
    G_WE_MIME_TYPE_MMF,                                                          
    G_WE_MIME_TYPE_MIDI,                                                        
    G_WE_MIME_TYPE_MP4,                              
    G_WE_MIME_TYPE_RM,                               
    G_WE_MIME_TYPE_RAM,                              
    G_WE_MIME_TYPE_SDP,                              
    G_WE_MIME_TYPE_IMY,                                                      
    G_WE_MIME_TYPE_IMAGE_X_UP_WPNG,
    G_WE_MIME_TYPE_VIDEO_X_MS_WMV,
	G_WE_MIME_TYPE_AUDIO_X_MS_WMA,
	G_WE_MIME_TYPE_VIDEO_AVI,
	G_WE_MIME_TYPE_VIDEO_MOV,
	G_WE_MIME_TYPE_APPLICATION_RAR,
	G_WE_MIME_TYPE_APPLICATION_PDF,
	G_WE_MIME_TYPE_VMD
};


WE_UCHAR *WeMime_GetExtHashTable()
{
   return (WE_UCHAR *)acMimeExtHashTable;
}

void WeMime_FreeExtHashTable(WE_UCHAR *pcMimeExtHashTable)
{
   return;
}


St_WeSCLStrTableEntry *WeMime_GetExtStringsEx(WE_HANDLE hMem, WE_BOOL bMemMgrr)
{
   WE_INT32 i = 0;
   St_WeSCLStrTableEntry *pstMimeExtStrings = NULL;
   
   pstMimeExtStrings = WE_SCL_MALLOC(sizeof(St_WeSCLStrTableEntry)*WE_EXT_STRING_TABLE_SIZE);
   if(NULL == pstMimeExtStrings)
   {
      return NULL;
   }
   for(i=0; i < WE_EXT_STRING_TABLE_SIZE; i++)
   {
      pstMimeExtStrings[i].pcName = (WE_CHAR *)acMimeExtStr[i];
      pstMimeExtStrings[i].iValue = (WE_INT32 )aiMimeExtStr[i];
   }
   return (St_WeSCLStrTableEntry *)pstMimeExtStrings;
}

void WeMime_FreeExtStringsEx(St_WeSCLStrTableEntry *pstMimeExtStrings,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
   if(NULL == pstMimeExtStrings)
   {
      return;
   }
   WE_FREE((void *)pstMimeExtStrings);
   
   return;
}

#define WE_MIME_TYPE_HASH_TABLE_SIZE                 179
#define WE_MIME_TYPE_STRING_TABLE_SIZE               175
const WE_UCHAR acWeMimeTypeHashTable[WE_MIME_TYPE_HASH_TABLE_SIZE] =
{
	0xFF, 0x36, 0xA8, 0x41, 0x03, 0x5D, 0x3D, 0x7A, 0x64, 0x72, 

0x2D, 0x92, 0x30, 0x98, 0x05, 0x50, 0x21, 0x1E, 0xFF, 0x4C, 

0x3C, 0x88, 0x52, 0x1D, 0x63, 0xA5, 0x66, 0xFF, 0xA0, 0x28, 

0x2C, 0x01, 0x45, 0x73, 0x94, 0x43, 0x23, 0x53, 0x78, 0x9C, 

0x8F, 0x16, 0x67, 0x5F, 0x61, 0xA3, 0xA7, 0x7E, 0x0F, 0x37, 

0x6B, 0x49, 0x86, 0x0E, 0x56, 0x48, 0x38, 0x00, 0x90, 0x87, 

0x0D, 0x1A, 0x22, 0x71, 0x51, 0x60, 0x31, 0x26, 0x11, 0x34, 

0x9F, 0x25, 0x02, 0x5B, 0x8D, 0x77, 0x9B, 0x3B, 0x5A, 0x5C, 

0x62, 0x15, 0x89, 0x2E, 0xA1, 0x99, 0x58, 0x8A, 0x4B, 0x74, 

0x93, 0x6D, 0x1C, 0x65, 0x2A, 0x81, 0x1B, 0x57, 0x97, 0x07, 

0x83, 0x40, 0x0A, 0x75, 0x33, 0x1F, 0x4D, 0x09, 0x54, 0x95, 

0x35, 0x82, 0x55, 0x4E, 0x76, 0x46, 0x13, 0x04, 0x6C, 0x70, 

0x39, 0x68, 0x2B, 0x20, 0x4F, 0x8E, 0x19, 0x59, 0x7F, 0x32, 

0x6A, 0x4A, 0x85, 0x12, 0xA2, 0x2F, 0x0C, 0x9D, 0x91, 0xA4, 

0x08, 0x6E, 0xA9, 0x06, 0xAE, 0x3F, 0x44, 0x9E, 0x17, 0x14, 

0x80, 0xAD, 0x42, 0x96, 0x47, 0x3E, 0x8C, 0xA6, 0xAB, 0x9A, 

0xAC, 0x7B, 0x0B, 0x8B, 0x27, 0x18, 0x29, 0x7D, 0xAA, 0x79, 

0x84, 0x10, 0x24, 0x5E, 0x3A, 0xFF, 0x6F, 0x7C, 0x69	
};//we_mime_type_hash_table

const WE_CHAR acMimeTypeStr[WE_MIME_TYPE_STRING_TABLE_SIZE][48]=
{           
	
	"*/*",                                             
	"text/*",                                          
	"text/html",                                       
	"text/plain",                                      
	"text/x-hdml",                                     
	"text/x-ttml",                                     
	"text/x-vcalendar",                                
	"text/x-vcard",                                    
	"text/vnd.wap.wml",                                
	"text/vnd.wap.wmlscript",                          
	"text/vnd.wap.channel",                            
	"multipart/*",                                     
	"multipart/mixed",                                 
	"multipart/form-data",                             
	"multipart/byteranges",                            
	"multipart/alternative",                           
	"application/*",                                   
	"application/java-vm",                             
	"application/x-www-form-urlencoded",               
	"application/x-hdmlc",                             
	"application/vnd.wap.wmlc",                        
	"application/vnd.wap.wmlscriptc",                  
	"application/vnd.wap.channelc",                    
	"application/vnd.wap.uaprof",                      
	"application/vnd.wap.wtls-ca-certificate",         
	"application/vnd.wap.wtls-user-certificate",       
	"application/x-x509-ca-cert",                      
	"application/x-x509-user-cert",                    
	"image/*",                                         
	"image/gif",                                       
	"image/jpeg",                                      
	"image/jpg",
	"image/tiff",                                      
	"image/png",                                       
	"image/vnd.wap.wbmp",                              
	"application/vnd.wap.multipart.*",                 
	"application/vnd.wap.multipart.mixed",             
	"application/vnd.wap.multipart.form-data",         
	"application/vnd.wap.multipart.byteranges",        
	"application/vnd.wap.multipart.alternative",       
	"application/xml",                                 
	"text/xml",                                        
	"application/vnd.wap.wbxml",                       
	"application/x-x968-cross-cert",                   
	"application/x-x968-ca-cert",                      
	"application/x-x968-user-cert",                    
	"text/vnd.wap.si",                                 
	"application/vnd.wap.sic",                         
	"text/vnd.wap.sl",                                 
	"application/vnd.wap.slc",                         
	"text/vnd.wap.co",                                 
	"application/vnd.wap.coc",                         
	"application/vnd.wap.multipart.related",           
	"application/vnd.wap.sia",                         
	"text/vnd.wap.connectivity-xml",                   
	"application/vnd.wap.connectivity-wbxml",          
	"application/pkcs7-mime",                          
	"application/vnd.wap.hashed-certificate",          
	"application/vnd.wap.signed-certificate",          
	"application/vnd.wap.cert-response",               
	"application/xhtml+xml",                           
	"application/wml+xml",                             
	"text/css",                                        
	"application/vnd.wap.mms-message",                 
	"application/vnd.wap.rollover-certificate",        
	"application/vnd.wap.locc+wbxml",                  
	"application/vnd.wap.loc+xml",                     
	"application/vnd.syncml.dm+wbxml",                 
	"application/vnd.syncml.dm+xml",                   
	"application/vnd.syncml.notification",             
	"application/vnd.wap.xhtml+xml",                   
	"application/vnd.wv.csp.cir",                      
	"application/vnd.oma.dd+xml",                      
	"application/vnd.oma.drm.message",                 
	"application/vnd.oma.drm.content",                 
	"application/vnd.oma.drm.rights+xml",              
	"application/vnd.oma.drm.rights+wbxml",            
	"application/vnd.wv.csp+xml",                      
	"application/vnd.wv.csp+wbxml",                    
	"audio/*",                                         
	"audio/mid",                                       
	"audio/mp3",                                       
	"audio/mpeg",                                      
	"audio/wav",                                       
	"audio/wave",                                      
	"audio/x-wav",                                     
	"application/x-vnd.mms-template-texts",            
	"application/x-wap-prov.browser-settings",         
	"application/x-wap-prov.browser-bookmarks",        
	"application/x-prov.syncset+xml",                  
	"application/x-prov.syncset+wbxml",                
	"application/smil",                                
	"image/svg+xml",                                   
	"video/svg",										                    
	"image/bmp",                                       
	"application/bmp",                                 
	"image/x-bmp",	                                    
	"image/x-bitmap",                                  
	"image/x-xbitmap",                                 
	"image/x-win-bitmap",                              
	"image/x-windows-bmp",                             
	"image/ms-bmp",	                                  
	"image/x-ms-bmp",                                  
	"video/mpeg",                                      
	"application/vnd.siaf",                            
	"application/x-siaf",                              
	"audio/amr",                                       
	"application/x-shockwave-flash",                   
	"application/zip",                                 
	"audio/sp-midi",                                   
	"application/java-archive",                        
	"text/vnd.sun.jzme.app-descriptor",                
	"video/3gpp2",                                     
	"audio/aac",                                       
	"audio/dls",                                       
	"video/svgz",                                      
	"video/url",                                       
	"audio/qcp",                                       
	"audio/vnd.qcelp",                                 
	"audio/spf",                                       
	"video/pmd",                                       
	"video/3gpp",                                      
	"audio/3gpp",                                      
	"video/3gp",                                       
	"audio/3gp",                                       
	"audio/xmf",                                       
	"audio/mobile-xmf",                                
	"audio/x-xmf",                                     
	"audio/mmf",                                       
	"audio/mxmf",                                      
	"audio/xmf0",			                                
	"audio/xmf1",                                      
	"application/x-smaf",                              
	"application/vnd.smaf",                            
	"audio/midi",                                      
	"audio/x-midi",                                    
	"application/x-midi", 
	"video/mp4",                             
	"audio/mp4",                                                                              
	"video/mpeg4",
	"video/vnd.rn-realvideo",                          
	"video/vnd-rn-realvideo",                          
	"video/x-pn-realvideo",                            
	"audio/vnd.rn-realvideo",                          
	"audio/vnd-rn-realvideo",                          
	"audio/x-pn-realvideo",                            
	"audio/x-pn-realaudio",                            
	"video/x-pn-realaudio",                            
	"video/sdp",                                       
	"text/sdp",                                        
	"audio/imy",
	"audio/imelody",                                   
	"text/x-imelody",	                                
	"application/vnd.uplanet.cacheop-wbxml",           
	"application/vnd.uplanet.signal",                  
	"application/vnd.uplanet.alert-wbxml",		          
	"application/vnd.uplanet.list-wbxml",              
	"application/vnd.uplanet.listcmd-wbxml",           
	"application/vnd.uplanet.channel-wbxml",           
	"application/vnd.uplanet.provisioning-status-uri", 
	"x-wap.multipart/vnd.uplanet.header-set",          
	"application/vnd.uplanet.bearer-choice-wbxml",     
	"application/vnd.phonecom.mmc-wbxml",	            
	"application/vnd.nokia.syncset+wbxml",             
	"image/x-up-wpng",                                 
	"application/iota.mmc-wbxml",                      
	"application/iota.mmc-xml",                        
    "video/x-ms-wmv",
	"audio/x-ms-wma",
	"video/avi",
	"video/quicktime",
	"application/rar",
    "application/pdf",                                           
	"chemical/x-vmd",
	"application/vocaltec-media-desc"

};
const WE_INT32 aiMimeTypeStr[WE_MIME_TYPE_STRING_TABLE_SIZE]=
{
    G_WE_MIME_TYPE_ANY_ANY,                                                                                                                                                                                                                                                           
    G_WE_MIME_TYPE_TEXT_ANY,                                                                                                                                                                                                                                                          
    G_WE_MIME_TYPE_TEXT_HTML,                                      
    G_WE_MIME_TYPE_TEXT_PLAIN,                                     
    G_WE_MIME_TYPE_TEXT_X_HDML,                                                                                                                                                                                                                                                       
    G_WE_MIME_TYPE_TEXT_X_TTML,                                                                                                                                                                                                                                                       
    G_WE_MIME_TYPE_TEXT_X_VCALENDAR,                                                                                                                                                                                                                                                  
    G_WE_MIME_TYPE_TEXT_X_VCARD,                                                                                                                                                                                                                                                      
    G_WE_MIME_TYPE_TEXT_VND_WAP_WML,                                                                                                                                                                                                                                                  
    G_WE_MIME_TYPE_TEXT_VND_WAP_WMLSCRIPT,                                                                                                                                                                                                                                            
    G_WE_MIME_TYPE_TEXT_VND_WAP_CHANNEL,                                                                                                                                                                                                                                              
    G_WE_MIME_TYPE_MULTIPART_ANY,                                                                                                                                                                                                                                                     
    G_WE_MIME_TYPE_MULTIPART_MIXED,                                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_MULTIPART_FORM_DATA,                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_MULTIPART_BYTERANGES,                                                                                                                                                                                                                                              
    G_WE_MIME_TYPE_MULTIPART_ALTERNATIVE,                                                                                                                                                                                                                                             
    G_WE_MIME_TYPE_APPLICATION_ANY,                                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_APPLICATION_JAVA_VM,                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_APPLICATION_X_WWW_FORM_URLENCODED,                                                                                                                                                                                                                                 
    G_WE_MIME_TYPE_APPLICATION_X_HDMLC,                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_WMLC,                                                                                                                                                                                                                                          
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_WMLSCRIPTC,                                                                                                                                                                                                                                    
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_CHANNELC,                                                                                                                                                                                                                                      
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_UAPROF,                                                                                                                                                                                                                                        
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_WTLS_CA_CERTIFICATE,                                                                                                                                                                                                                           
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_WTLS_USER_CERTIFICATE,                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_APPLICATION_X_X509_CA_CERT,                                                                                                                                                                                                                                        
    G_WE_MIME_TYPE_APPLICATION_X_X509_USER_CERT,                                                                                                                                                                                                                                      
    G_WE_MIME_TYPE_IMAGE_ANY,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_IMAGE_GIF,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_IMAGE_JPEG,    
	G_WE_MIME_TYPE_IMAGE_JPEG,
    G_WE_MIME_TYPE_IMAGE_TIFF,                                     
    G_WE_MIME_TYPE_IMAGE_PNG,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_IMAGE_VND_WAP_WBMP,                             
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_ANY,                                                                                                                                                                                                                                 
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_MIXED,                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_FORM_DATA,                                                                                                                                                                                                                           
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_BYTERANGES,                                                                                                                                                                                                                          
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_ALTERNATIVE,                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_APPLICATION_XML,                                
    G_WE_MIME_TYPE_TEXT_XML,                                       
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_WBXML,                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_APPLICATION_X_X968_CROSS_CERT,                                                                                                                                                                                                                                     
    G_WE_MIME_TYPE_APPLICATION_X_X968_CA_CERT,                                                                                                                                                                                                                                        
    G_WE_MIME_TYPE_APPLICATION_X_X968_USER_CERT,                                                                                                                                                                                                                                      
    G_WE_MIME_TYPE_TEXT_VND_WAP_SI,                                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_SIC,                                                                                                                                                                                                                                           
    G_WE_MIME_TYPE_TEXT_VND_WAP_SL,                                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_SLC,                                                                                                                                                                                                                                           
    G_WE_MIME_TYPE_TEXT_VND_WAP_CO,                                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_COC,                                                                                                                                                                                                                                           
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_MULTIPART_RELATED,                                                                                                                                                                                                                             
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_SIA,                                                                                                                                                                                                                                           
    G_WE_MIME_TYPE_TEXT_VND_WAP_CONNECTIVITY_XML,                                                                                                                                                                                                                                     
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_CONNECTIVITY_WBXML,                                                                                                                                                                                                                            
    G_WE_MIME_TYPE_APPLICATION_PKCS7_MIME,                                                                                                                                                                                                                                            
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_HASHED_CERTIFICATE,                                                                                                                                                                                                                            
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_SIGNED_CERTIFICATE,                                                                                                                                                                                                                            
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_CERT_RESPONSE,                                                                                                                                                                                                                                 
    G_WE_MIME_TYPE_APPLICATION_XHTML_XML,                                                                                                                                                                                                                                             
    G_WE_MIME_TYPE_APPLICATION_WML_XML,                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_TEXT_CSS,                                                                                                                                                                                                                                                          
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_MMS_MESSAGE,                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_ROLLOVER_CERTIFICATE,                                                                                                                                                                                                                          
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_LOCC_WBXML,                                                                                                                                                                                                                                    
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_LOC_XML,                                                                                                                                                                                                                                       
    G_WE_MIME_TYPE_APPLICATION_VND_SYNCML_DM_WBXML,                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_APPLICATION_VND_SYNCML_DM_XML,                                                                                                                                                                                                                                     
    G_WE_MIME_TYPE_APPLICATION_VND_SYNCML_NOTIFICATION,                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_APPLICATION_VND_WAP_XHTML_XML,                                                                                                                                                                                                                                     
    G_WE_MIME_TYPE_APPLICATION_VND_WV_CSP_CIR,                                                                                                                                                                                                                                        
    G_WE_MIME_TYPE_APPLICATION_VND_OMA_DD_XML,                                                                                                                                                                                                                                        
    G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE,                
    G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT,                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML,                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML,                                                                                                                                                                                                                              
    G_WE_MIME_TYPE_APPLICATION_VND_WV_CSP_XML,                                                                                                                                                                                                                                        
    G_WE_MIME_TYPE_APPLICATION_VND_WV_CSP_WBXML,                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    G_WE_MIME_TYPE_AUDIO_ANY,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_AUDIO_MID,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_MP3,                                            
    G_WE_MIME_TYPE_MP3,                                            
    G_WE_MIME_TYPE_WAV,                                            
    G_WE_MIME_TYPE_WAV,                                            
    G_WE_MIME_TYPE_WAV,                                            
    G_WE_MIME_TYPE_APPLICATION_X_VND_MMS_TEMPLATE_TEXTS,                                                                                                                                                                                                                              
    G_WE_MIME_TYPE_APPLICATION_X_WAP_PROV_BROWSER_SETTINGS,                                                                                                                                                                                                                           
    G_WE_MIME_TYPE_APPLICATION_X_WAP_PROV_BROWSER_BOOKMARKS,                                                                                                                                                                                                                          
    G_WE_MIME_TYPE_APPLICATION_X_WAP_PROV_SYNCSET_XML,                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_APPLICATION_X_WAP_PROV_SYNCSET_WBXML,                                                                                                                                                                                                                              
    G_WE_MIME_TYPE_APPLICATION_SMIL,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
    G_WE_MIME_TYPE_SVG,                                            
    G_WE_MIME_TYPE_SVG,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_BMP,                                            
    G_WE_MIME_TYPE_VIDEO_MPEG,                                     
    G_WE_MIME_TYPE_APPLICATION_VND_SIAF,                                                                                                                                                                                                                                              
    G_WE_MIME_TYPE_APPLICATION_X_SIAF,                                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_AUDIO_AMR,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_APPLICATION_X_SHOCKWAVE_FLASH,                                                                                                                                                                                                                                     
    G_WE_MIME_TYPE_APPLICATION_ZIP,                                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_AUDIO_SP_MIDI,                                                                                                                                                                                                                                                     
    G_WE_MIME_TYPE_APPLICATION_JAVA_ARCHIVE,                       
    G_WE_MIME_TYPE_TEXT_VND_SUN_JZME_APP,                          
    G_WE_MIME_TYPE_VIDEO_3GPP2,                                    
    G_WE_MIME_TYPE_AUDIO_AAC,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_AUDIO_DLS,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_VIDEO_SVGZ,                                                                                                                                                                                                                                                        
    G_WE_MIME_TYPE_VIDEO_URL,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_AUDIO_QCP,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_AUDIO_VND_QCELP,                                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_AUDIO_SPF,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_VIDEO_PMD,                                                                                                                                                                                                                                                         
    G_WE_MIME_TYPE_3GP,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_3GP,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_3GP,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_3GP,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    G_WE_MIME_TYPE_XMF,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_XMF,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_XMF,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
    G_WE_MIME_TYPE_MMF,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_MMF,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_MMF,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_MMF,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_MMF,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_MMF,                                                                                                                                                                                                                                                                                                             
    G_WE_MIME_TYPE_MIDI,                                                                                                                                                                                                                                                              
    G_WE_MIME_TYPE_MIDI,                                                                                                                                                                                                                                                              
    G_WE_MIME_TYPE_MIDI,                                                                                                                                                                                                                                                                                                      
    G_WE_MIME_TYPE_MP4,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_MP4,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_MP4,
    G_WE_MIME_TYPE_RM,                                                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_RM,                                                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_RM,                                                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_RM,                                                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_RM,                                                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_RM,                                                                                                                                                                                                                                                                
    G_WE_MIME_TYPE_RAM,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_RAM,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_SDP,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_SDP,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_IMY,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_IMY,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_IMY,                                                                                                                                                                                                                                                               
    G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_CACHEOP_WBXML,          
    G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_SIGNAL,                 
    G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_ALERT_WBXML,            
    G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_LIST_WBXML,             
    G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_LISTCMD_WBXML,          
    G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_CHANNEL_WBXML,          
    G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_PROVISIONING_STATUS_URI,
    G_WE_MIME_TYPE_X_WAP_MULTIPART_VND_UPLANET_HEADER_SET,         
    G_WE_MIME_TYPE_APPLICATION_VND_UPLANET_BEARER_CHOICE_WBXML,    
    G_WE_MIME_TYPE_APPLICATION_VND_PHONECOM_MMC_WBXML,             
    G_WE_MIME_TYPE_APPLICATION_VND_NOKIA_SYNCSET_WBXML,            
    G_WE_MIME_TYPE_IMAGE_X_UP_WPNG,                                
    G_WE_MIME_TYPE_APPLICATION_IOTA_MMC_WBXML,                     
    G_WE_MIME_TYPE_APPLICATION_IOTA_MMC ,
	G_WE_MIME_TYPE_VIDEO_X_MS_WMV,
	G_WE_MIME_TYPE_AUDIO_X_MS_WMA,
	G_WE_MIME_TYPE_VIDEO_AVI,
	G_WE_MIME_TYPE_VIDEO_MOV,
	G_WE_MIME_TYPE_APPLICATION_RAR,
	G_WE_MIME_TYPE_APPLICATION_PDF,
	G_WE_MIME_TYPE_VMD,
	G_WE_MIME_TYPE_VMD
};


WE_UCHAR *WeMime_GetTypeHashTable(void)
{
   return (WE_UCHAR *)acWeMimeTypeHashTable;
}

void WeMime_FreeTypeHashTable(WE_UCHAR *pcWeMimeTypeHashTable)
{
   return;
}

St_WeSCLStrTableEntry *WeMime_GetTypeStringsEx(WE_HANDLE hMem, WE_BOOL bMemMgr)
{
   WE_INT32    i = 0;
   St_WeSCLStrTableEntry *pstWeMimeTypeStrings = NULL;
   
   pstWeMimeTypeStrings = WE_SCL_MALLOC(sizeof(St_WeSCLStrTableEntry)*WE_MIME_TYPE_STRING_TABLE_SIZE);
   if(NULL == pstWeMimeTypeStrings)
   {
      return NULL;
   }  
   
   for(i=0; i<WE_MIME_TYPE_STRING_TABLE_SIZE; i++)
   {
      pstWeMimeTypeStrings[i].pcName = (WE_CHAR *)acMimeTypeStr[i];
      pstWeMimeTypeStrings[i].iValue = aiMimeTypeStr[i];
   }
   return (St_WeSCLStrTableEntry *)pstWeMimeTypeStrings;
}


void WeMime_FreeTypeStringsEx(St_WeSCLStrTableEntry *pstWeMimeTypeStrings,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
   if(NULL == pstWeMimeTypeStrings)
   {
      return;
   }
   WE_SCL_FREE((void *)pstWeMimeTypeStrings);
   
   return;
}

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
/*Define struct &  union type end*/

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/



/*******************************************************************************
*   Global Variable Declare Section
*******************************************************************************/


/*******************************************************************************
*   File Static Variable Define Section
*******************************************************************************/
St_WeSCLStrtableInfo *WeMime_GetMimeExtensionsEx(WE_HANDLE hMem, WE_BOOL bMemMgr)
{
   St_WeSCLStrtableInfo *pstWeMimeExtensions = NULL;
   St_WeSCLStrtableInfo stWeMimeExtensions = 
   {
      //WeMime_GetExtStrings(hMem),
      //WeMime_GetExtHashTable(hMem),
      NULL,
         NULL,
         WE_EXT_HASH_TABLE_SIZE,
         WE_EXT_STRING_TABLE_SIZE,
         WE_TABLE_SEARCH_LINEAR,
         0
   };//We_Mime_Extensions
	pstWeMimeExtensions = WE_SCL_MALLOC(sizeof(stWeMimeExtensions));
   if(NULL == pstWeMimeExtensions)
   {
      return NULL;
   }
   if(TRUE == bMemMgr)
   {
      stWeMimeExtensions.pstStringTable = WeMime_GetExtStringsEx(hMem,TRUE);
      stWeMimeExtensions.pucHashTable = WeMime_GetExtHashTable();
   }
   else
   {
      stWeMimeExtensions.pstStringTable = WeMime_GetExtStringsEx(NULL,FALSE);
      stWeMimeExtensions.pucHashTable = WeMime_GetExtHashTable();
   }
   
   if(NULL == stWeMimeExtensions.pstStringTable)
   {
      return NULL;
   }
   WE_SCL_MEMCPY((St_WeSCLStrtableInfo *)pstWeMimeExtensions,&stWeMimeExtensions,sizeof(stWeMimeExtensions));
   
   return pstWeMimeExtensions;
}


void WeMime_FreeMimeExtensionsEx(St_WeSCLStrtableInfo *pstWeMimeExtensions,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
   if(NULL == pstWeMimeExtensions)
   {
      return;
   }
   WeMime_FreeExtHashTable(pstWeMimeExtensions->pucHashTable);
   WeMime_FreeExtStringsEx(pstWeMimeExtensions->pstStringTable,NULL,FALSE);
   WE_SCL_FREE((void *)pstWeMimeExtensions);
   return;
}


St_WeSCLStrtableInfo *WeMime_GetMimeTypesEx(WE_HANDLE hMem, WE_BOOL bMemMgr)
{
   St_WeSCLStrtableInfo stWeMimeTypes = 
   {
      NULL,
         NULL,
         WE_MIME_TYPE_HASH_TABLE_SIZE,
         WE_MIME_TYPE_STRING_TABLE_SIZE,
         WE_TABLE_SEARCH_LINEAR,
         0
   };//We_Mime_Types
   St_WeSCLStrtableInfo *pstWeMimeTypes = NULL;
   
   pstWeMimeTypes = WE_SCL_MALLOC(sizeof(St_WeSCLStrtableInfo));
   if(NULL == pstWeMimeTypes)
   {
      return NULL;
   }
   if(TRUE == bMemMgr)
   {
      stWeMimeTypes.pstStringTable = WeMime_GetTypeStringsEx(hMem,TRUE);
      stWeMimeTypes.pucHashTable = WeMime_GetTypeHashTable();
   }
   else
   {
      stWeMimeTypes.pstStringTable = WeMime_GetTypeStringsEx(NULL,FALSE);
      stWeMimeTypes.pucHashTable = WeMime_GetTypeHashTable();
   }
   
   WE_SCL_MEMCPY((St_WeSCLStrtableInfo *)pstWeMimeTypes,&stWeMimeTypes,sizeof(stWeMimeTypes));
   
   return pstWeMimeTypes;
}


void WeMime_FreeMimeTypesEx(St_WeSCLStrtableInfo *pstWeMimeTypes,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
   if(NULL == pstWeMimeTypes)
   {
      return;
   }
   WeMime_FreeTypeHashTable(pstWeMimeTypes->pucHashTable);
   WeMime_FreeTypeStringsEx(pstWeMimeTypes->pstStringTable,NULL,FALSE);
   WE_SCL_FREE((void *)pstWeMimeTypes);
   pstWeMimeTypes = NULL;
}
/*******************************************************************************
*   Function Define Section
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
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/

const WE_CHAR * WeMime_Ext2Mime_Ex (const WE_CHAR *pcExt,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
   const WE_CHAR       *pcStr = WE_MIME_DEFAULT_MIME;
   WE_INT32            iIndx = -1;
   St_WeSCLStrtableInfo *pstWeMimeTypes = NULL;
   St_WeSCLStrtableInfo *pstWeMimeExtensions = NULL;
   
   if(pcExt == NULL)
   {
         pcStr = WE_SCL_STRDUP(pcStr);
         
      return pcStr;
   }
   if(TRUE == bMemMgr)
   {
      pstWeMimeExtensions = WeMime_GetMimeExtensionsEx(hMem,TRUE);
   }
   else
   {
      pstWeMimeExtensions = WeMime_GetMimeExtensionsEx(NULL,FALSE);
   }
   
   if(NULL == pstWeMimeExtensions)
   {
         WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,NULL,FALSE);
         pcStr = WE_STRDUP(pcStr);
      return pcStr;
   }
   
   iIndx = WE_SCL_STR2INTLC(pcExt,WE_SCL_STRLEN(pcExt), pstWeMimeExtensions);
   if(TRUE == bMemMgr)
   {
      WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,hMem,TRUE);
   }
   else
   {
      WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,NULL,FALSE);
   }
   
   if(iIndx >= 0)
   {
         pstWeMimeTypes = WeMime_GetMimeTypesEx(NULL,FALSE);
         pcStr = WE_STRDUP(WE_SCL_INT2STR(iIndx, pstWeMimeTypes));
         WeMime_FreeMimeTypesEx(pstWeMimeTypes,NULL,FALSE);
   }
   else
   {
         pcStr = WE_SCL_STRDUP(pcStr);
   }
   return pcStr;
}

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
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/

const WE_CHAR * WeMime_Mime2Ext_Ex (const WE_CHAR *pcMime,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
   const WE_CHAR       *pcStr = NULL;
   WE_INT32            iIndx = -1;
   St_WeSCLStrtableInfo *pstWeMimeTypes = NULL;
   
   if(pcMime == NULL)
   {
      return NULL;
   }
   if(TRUE == bMemMgr)
   {
      pstWeMimeTypes = WeMime_GetMimeTypesEx(hMem,TRUE);
   }
   else
   {
      pstWeMimeTypes = WeMime_GetMimeTypesEx(NULL,FALSE);
   }
   
   if(NULL == pstWeMimeTypes)
   {
      return NULL;
   }
   iIndx = WE_SCL_STR2INTLC (pcMime, WE_SCL_STRLEN(pcMime), pstWeMimeTypes);
   if(TRUE == bMemMgr)
   {
      WeMime_FreeMimeTypesEx(pstWeMimeTypes,hMem,TRUE);
   }
   else
   {
      WeMime_FreeMimeTypesEx(pstWeMimeTypes,NULL,FALSE);
   }
   
   if(iIndx >= 0)
   {
      const St_WeSCLStrtableInfo *pstWeMimeExtensions = WeMime_GetMimeExtensionsEx(NULL,FALSE);
      pcStr = WE_SCL_STRDUP(WE_SCL_INT2STR (iIndx, pstWeMimeExtensions));
      WeMime_FreeMimeExtensionsEx((St_WeSCLStrtableInfo *)pstWeMimeExtensions,NULL,FALSE);
   }
   return pcStr;
}

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
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_INT32 WeMime_Mime2IntEx (const WE_CHAR *pcMime,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
   WE_INT32 iIndx = -1;
   St_WeSCLStrtableInfo *pstWeMimeTypes = NULL;
   
   if(pcMime == NULL)
   {
      return iIndx;
   }
   
   if(TRUE == bMemMgr)
   {
      pstWeMimeTypes = WeMime_GetMimeTypesEx(hMem,TRUE);
      iIndx = WE_SCL_STR2INTLC (pcMime, WE_SCL_STRLEN(pcMime), pstWeMimeTypes);
      WeMime_FreeMimeTypesEx(pstWeMimeTypes,hMem,TRUE);
   }
   else
   {
      pstWeMimeTypes = WeMime_GetMimeTypesEx(NULL,FALSE);
      iIndx = WE_SCL_STR2INTLC (pcMime, WE_SCL_STRLEN(pcMime), pstWeMimeTypes);
      WeMime_FreeMimeTypesEx(pstWeMimeTypes,NULL,FALSE);
   }
   
   return iIndx;
}

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
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
const WE_CHAR * WeMime_Int2MimeEx (WE_INT32 iKnownVal,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
   const WE_CHAR *pcMimeType = NULL;
   const St_WeSCLStrtableInfo *pstWeMimeTypes = NULL;
   
   pstWeMimeTypes = WeMime_GetMimeTypesEx(NULL,FALSE);
   pcMimeType = WE_SCL_STRDUP(WE_SCL_INT2STR (iKnownVal, pstWeMimeTypes));
   WeMime_FreeMimeTypesEx((St_WeSCLStrtableInfo *)pstWeMimeTypes,NULL,FALSE);
   
   return pcMimeType;
}

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
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    That it is the integer representation of the mime type corresponding
    to the extension that is returned.
=====================================================================================*/
WE_INT32 WeMime_Ext2IntEx (const WE_CHAR *pcExt,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
   WE_INT32 iIndx = -1;
   St_WeSCLStrtableInfo *pstWeMimeExtensions = NULL;
   
   if(pcExt == NULL)
   {
      return iIndx;
   }
   
   if(TRUE == bMemMgr)
   {
      pstWeMimeExtensions = WeMime_GetMimeExtensionsEx(hMem,TRUE);
      iIndx = WE_SCL_STR2INTLC (pcExt, WE_SCL_STRLEN (pcExt), pstWeMimeExtensions);
      WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,hMem,TRUE);
   }
   else
   {
      pstWeMimeExtensions = WeMime_GetMimeExtensionsEx(NULL,FALSE);
      iIndx = WE_SCL_STR2INTLC (pcExt, WE_SCL_STRLEN (pcExt), pstWeMimeExtensions);
      WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,NULL,FALSE);
   }
   return iIndx;
}

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
WE_CHAR ** WeMime_Mime2ExtExEx (const WE_CHAR *pcMime,WE_HANDLE hMem , WE_BOOL bMemMgr) 
{
   WE_CHAR       **pcStr = NULL;
   WE_INT32            iIndx = -1;
   WE_INT32          iNum = 0;
   St_WeSCLStrtableInfo *pstWeMimeTypes = NULL;
   
   if(pcMime == NULL)
   {
      return NULL;
   }
   
   if(TRUE == bMemMgr)
   {
      pstWeMimeTypes = WeMime_GetMimeTypesEx(hMem,TRUE);
      if(NULL == pstWeMimeTypes)
      {
         return NULL;
      }
      iIndx = WE_SCL_STR2INTLC (pcMime, WE_SCL_STRLEN(pcMime), pstWeMimeTypes);
      WeMime_FreeMimeTypesEx(pstWeMimeTypes,hMem,TRUE);
   }
   else
   {
      pstWeMimeTypes = WeMime_GetMimeTypesEx(NULL,FALSE);
      if(NULL == pstWeMimeTypes)
      {
         return NULL;
      }
      iIndx = WE_SCL_STR2INTLC (pcMime, WE_SCL_STRLEN(pcMime), pstWeMimeTypes);
      WeMime_FreeMimeTypesEx(pstWeMimeTypes,NULL,FALSE);
   }
   
   if(iIndx >= 0)
   {
      WE_INT32                         iCount;
      WE_INT32                         iStringTableSize = 0;
      const St_WeSCLStrTableEntry      *pstStringTable = NULL;
      
      const St_WeSCLStrtableInfo *pstWeMimeExtensions = WeMime_GetMimeExtensionsEx(NULL,FALSE);
      if(NULL == pstWeMimeExtensions)
         return NULL;
      iStringTableSize = pstWeMimeExtensions->iStringTableSize;
      pstStringTable = pstWeMimeExtensions->pstStringTable;
      for (iCount = 0; iCount < iStringTableSize; iCount++) 
      {
         if (pstStringTable[iCount].iValue == iIndx)
         {
            iNum++;
         }
      }
      if(iNum == 0)
      {
         return NULL;
      }
      else
      {
         WE_INT i = 0;
         pcStr = (WE_CHAR **)WE_SCL_MALLOC(sizeof(WE_CHAR *) * (iNum+1));
         if(NULL == pcStr)
            return NULL;
         WE_SCL_MEMSET(pcStr,0x00,sizeof(WE_CHAR *) * (iNum+1));
         for (iCount = 0; iCount < iStringTableSize; iCount++) 
         {
            if (pstStringTable[iCount].iValue == iIndx)
            {
               pcStr[i] = pstStringTable[iCount].pcName;
               i++;
            }
         }
         pcStr[i] = NULL;
      }
      WeMime_FreeMimeExtensionsEx((St_WeSCLStrtableInfo *)pstWeMimeExtensions,NULL,FALSE);
   }
   return pcStr;
}

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
WE_CHAR ** WeMime_Ext2MimeExEx (const WE_CHAR *pcExt,WE_HANDLE hMem, WE_BOOL bMemMgr)

{
   WE_CHAR       **pcStr = NULL;
   WE_INT32            iIndx = -1;
   WE_INT32          iNum = 0;
   St_WeSCLStrtableInfo *pstWeMimeTypes = NULL;
   St_WeSCLStrtableInfo *pstWeMimeExtensions = NULL;
   
   if(pcExt == NULL)
   {
      pcStr = (WE_CHAR **)WE_SCL_MALLOC(sizeof(WE_CHAR *) * 2);
      if(NULL == pcStr)
         return NULL;
      WE_SCL_MEMSET(pcStr,0x00,sizeof(WE_CHAR *) * 2);
      pcStr[0] = WE_MIME_DEFAULT_MIME;
      pcStr[1] = NULL;
      
      return pcStr;
   }
   
   if(TRUE == bMemMgr)
   {
      pstWeMimeExtensions = WeMime_GetMimeExtensionsEx(hMem,TRUE);
   }
   else
   {
      pstWeMimeExtensions = WeMime_GetMimeExtensionsEx(NULL,FALSE);
   }
   
   if(NULL == pstWeMimeExtensions)
   {
      WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,NULL,FALSE);
      pcStr = (WE_CHAR **)WE_SCL_MALLOC(sizeof(WE_CHAR *) * 2);
      if(NULL == pcStr)
         return NULL;
      WE_SCL_MEMSET(pcStr,0x00,sizeof(WE_CHAR *) * 2);
      pcStr[0] = WE_MIME_DEFAULT_MIME;
      pcStr[1] = NULL;
      
      return pcStr;
   }
   
   iIndx = WE_SCL_STR2INTLC(pcExt,WE_SCL_STRLEN(pcExt), pstWeMimeExtensions);
   if(TRUE == bMemMgr)
   {
      WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,hMem,TRUE);
   }
   else
   {
      WeMime_FreeMimeExtensionsEx(pstWeMimeExtensions,NULL,FALSE);
   }
   
   if(iIndx >= 0)
   {
      WE_INT32                         i = 0;
      WE_INT32                         iCount = 0;
      WE_INT32                         iStringTableSize = 0;
      const St_WeSCLStrTableEntry      *pstStringTable = NULL;
      
      
      pstWeMimeTypes = WeMime_GetMimeTypesEx(NULL,FALSE);
      if(NULL == pstWeMimeTypes)
         return NULL;
      iStringTableSize = pstWeMimeTypes->iStringTableSize;
      pstStringTable = pstWeMimeTypes->pstStringTable;
      for (iCount = 0; iCount < iStringTableSize; iCount++) 
      {
         if (pstStringTable[iCount].iValue == iIndx)
         {
            iNum++;
         }
      }
      pcStr = (WE_CHAR **)WE_SCL_MALLOC(sizeof(WE_CHAR *) * (iNum+1));
      if(NULL == pcStr)
         return NULL;
      
      WE_SCL_MEMSET(pcStr,0x00,sizeof(WE_CHAR *) * (iNum+1));
      for (iCount = 0; iCount < iStringTableSize; iCount++) 
      {
         if (pstStringTable[iCount].iValue == iIndx)
         {
            pcStr[i] = pstStringTable[iCount].pcName;
            i++;
         }
      }
      pcStr[i] = NULL;
      WeMime_FreeMimeTypesEx(pstWeMimeTypes,NULL,FALSE);
   }
   else
   {
      pcStr = (WE_CHAR **)WE_SCL_MALLOC(sizeof(WE_CHAR *) * 2);
      if(NULL == pcStr)
         return NULL;
      WE_SCL_MEMSET(pcStr,0x00,sizeof(WE_CHAR *) * 2);
      pcStr[0] = WE_MIME_DEFAULT_MIME;
      pcStr[1] = NULL;
   }
   return pcStr;
}

