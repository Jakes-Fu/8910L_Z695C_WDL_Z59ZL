/*=====================================================================================
    FILE NAME   : we_hdr.c
    MODULE NAME : HDR


    GENERAL DESCRIPTION
    This file define function operating memory.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2006-06-08       Guohong                        create we_hdr.c
    2007-7-16        TomTang                        Update

=====================================================================================*/
//pclint
/*lint -e722 */

/***************************************************************************************
*   Include File Section
****************************************************************************************/
/*Include System head file*/

/*Include Program Global head file*/
#include "we_mem.h"
/*Include Module head file*/
/*Include Private head file*/



#include "we_def.h"
#include "we_hdr.h"
#include "we_mime.h"
#include "we_url.h"
#include "we_scl.h"
#include "we_csc.h"

#include "cmmn_mem.h"
#include "tf3rd_for_HZ.h"


/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/

#define WE_HDR_INITIAL_SIZE            15
#define WE_HDR_SIZE_INCREMENT          10

#define WE_HDR_STR_DOUBLE_QUOTE        "\""
#define WE_HDR_STR_EQUAL               "="
#define WE_HDR_STR_SEMI_COMMA          ";"

#define WE_HDR_CH_DOUBLE_QUOTE         '"'
#define WE_HDR_CH_EQUAL                '='
#define WE_HDR_CH_SEMI_COMMA           ';'
#define WE_HDR_CH_BACKSLASH            '\\'
#define WE_HDR_CH_BLANK                ' ' 



/*The header field name's string table and its hash table*/
#define WE_HDR_NAME_HASH_TABLE_SIZE               157
#define WE_HDR_NAME_STRING_TABLE_SIZE            105

const WE_UCHAR stNamehashTable[WE_HDR_NAME_HASH_TABLE_SIZE] = 
    {
	    0xFF, 0xFF, 0xFF, 0x59, 0x12, 0x2F, 0xFF, 0xFF, 0x54, 0xFF,
		0x43, 0x44, 0x49, 0x22, 0xFF, 0xFF, 0xFF, 0x00, 0x0D, 0x34,
		0x23, 0x1F, 0xFF, 0xFF, 0x57, 0xFF, 0x39, 0x30, 0xFF, 0x1C,
		0xFF, 0x53, 0x18, 0x1A, 0xFF, 0x36, 0xFF, 0xFF, 0xFF, 0xFF,
		0x24, 0x31, 0x07, 0x56, 0xFF, 0x20, 0x05, 0x2E, 0xFF, 0xFF,
		0x0B, 0xFF, 0x65, 0x02, 0x09, 0x29, 0xFF, 0x06, 0x45, 0x5E,
		0x3F, 0x01, 0x33, 0x0A, 0x27, 0x13, 0x47, 0x1B, 0xFF, 0x08,
		0xFF, 0x63, 0x16, 0x2A, 0x48, 0x42, 0xFF, 0x28, 0x4D, 0x64,
		0xFF, 0x4E, 0xFF, 0x50, 0x68, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0x46, 0x19, 0xFF, 0x5D, 0x3E, 0x61, 0x60, 0xFF,
		0x1D, 0xFF, 0x14, 0x17, 0x3A, 0x52, 0x15, 0xFF, 0x0C, 0x62,
		0x58, 0xFF, 0x32, 0x3C, 0x5B, 0x10, 0x03, 0xFF, 0x11, 0xFF,
		0x2B, 0xFF, 0x37, 0xFF, 0xFF, 0x2C, 0x51, 0x3B, 0x41, 0xFF,
		0x25, 0xFF, 0x55, 0x5A, 0xFF, 0x0E, 0xFF, 0x1E, 0x2D, 0x0F,
		0x38, 0x3D, 0xFF, 0x5F, 0x26, 0x40, 0x4A, 0xFF, 0x67, 0x66,
		0x4B, 0x04, 0x35, 0x4C, 0x5C, 0x4F, 0x21           
		
     };
const WE_CHAR acHdrNameStr[WE_HDR_NAME_STRING_TABLE_SIZE][29]=
{
	{"accept"},
    {"accept-charset"},
    {"accept-encoding"},
    {"accept-language"},
    {"accept-ranges"},
    {"age"},
    {"allow"},
    {"authorization"},
    {"cache-control"},
    {"connection"},
    {"content-base"},
    {"content-encoding"},
    {"content-language"},
    {"content-length"},
    {"content-location"},
    {"content-md5"},
    {"content-range"},
    {"content-type"},
    {"date"},
    {"etag"},
    {"expires"},
    {"from"},
    {"host"},
    {"if-modified-since"},
    {"if-match"},
    {"if-none-match"},
    {"if-range"},
    {"if-unmodified-since"},
    {"location"},
    {"last-modified"},
    {"max-forwards"},
    {"pragma"},
    {"proxy-authenticate"},
    {"proxy-authorization"},
    {"public"},
    {"range"},
    {"referer"},
    {"retry-after"},
    {"server"},
    {"transfer-encoding"},
    {"upgrade"},
    {"user-agent"},
    {"vary"},
    {"via"},
    {"warning"},
    {"www-authenticate"},
    {"content-disposition"},
    {"x-wap-application-id"},
    {"x-wap-content-uri"},
    {"x-wap-initiator-uri"},
    {"accept-application"},
    {"bearer-indication"},
    {"push-flag"},
    {"profile"},
    {"profile-diff"},
    {"profile-warning"},
    {"expect"},
    {"te"},
    {"trailer"},
    {"x-wap-tod"},
    {"content-id"},
    {"set-cookie"},
    {"cookie"},
    {"encoding-version"},
    {"x-wap-security"},
    {"x-wap-loc-invocation"},
    {"x-wap-loc-delivery"},
    {"x-wap-cpitag"},
    {"x-wap-push-provurl"},
    {"x-wap-push-accept"},
    {"x-wap-push-accept-charset"},
    {"x-wap-push-accept-encoding"},
    {"x-wap-push-accept-language"},
    {"x-wap-push-accept-appid"},
    {"x-wap-push-maxmsgsize"},
    {"x-wap-push-accept-maxpushreq"},
    {"x-wap-terminal-id"},
    {"x-wap-push-info"},
    {"x-wap-push-status"},
    {"x-wap-push-ota-version"},
    {"x-wap-bearer-indication"},        
    {"x-wap-authenticate"},
    {"x-wap-authorization"},
    {"x-wap-profile"},
    {"x-wap-profile-diff"},
    {"x-wap-profile-warning"},
    {"x-up-tpd-e2e-headers"},
    {"x-forwarded-for"},
    {"content-transfer-encoding"},
    {"refresh"},
    {"x-drm"},
    {"set-cookie2"},
    {"cookie2"},
    {"x-vodafone-3gpdpcontext"},
    {"x-oma-drm-separate-delivery"},
    {"rights-issuer"},
    {"content-name"},
    {"content-description"},
    {"content-vendor"},
    {"icon-uri"},
    {"encryption-method"},
    {"x-vodafone-content"},
    {"keep-alive"},
	{"proxy-connection"},
	{"x-sp-result"}

};
const WE_INT32 aiHdrNameStr[WE_HDR_NAME_STRING_TABLE_SIZE]=
{
    G_WE_HDR_ACCEPT,
    G_WE_HDR_ACCEPT_CHARSET,
    G_WE_HDR_ACCEPT_ENCODING,
    G_WE_HDR_ACCEPT_LANGUAGE,
    G_WE_HDR_ACCEPT_RANGES,
    G_WE_HDR_AGE,
    G_WE_HDR_ALLOW,
    G_WE_HDR_AUTHORIZATION,
    G_WE_HDR_CACHE_CONTROL,
    G_WE_HDR_CONNECTION,
    G_WE_HDR_CONTENT_BASE,
    G_WE_HDR_CONTENT_ENCODING,
    G_WE_HDR_CONTENT_LANGUAGE,
    G_WE_HDR_CONTENT_LENGTH,
    G_WE_HDR_CONTENT_LOCATION,
    G_WE_HDR_CONTENT_MD5,
    G_WE_HDR_CONTENT_RANGE,
    G_WE_HDR_CONTENT_TYPE,
    G_WE_HDR_DATE,
    G_WE_HDR_ETAG,
    G_WE_HDR_EXPIRES,
    G_WE_HDR_FROM,
    G_WE_HDR_HOST,
    G_WE_HDR_IF_MODIFIED_SINCE,
    G_WE_HDR_IF_MATCH,
    G_WE_HDR_IF_NONE_MATCH,
    G_WE_HDR_IF_RANGE,
    G_WE_HDR_IF_UNMODIFIED_SINCE,
    G_WE_HDR_LOCATION,
    G_WE_HDR_LAST_MODIFIED,
    G_WE_HDR_MAX_FORWARDS,
    G_WE_HDR_PRAGMA,
    G_WE_HDR_PROXY_AUTHENTICATE,
    G_WE_HDR_PROXY_AUTHORIZATION,
    G_WE_HDR_PUBLIC,
    G_WE_HDR_RANGE,
    G_WE_HDR_REFERER,
    G_WE_HDR_RETRY_AFTER,
    G_WE_HDR_SERVER,
    G_WE_HDR_TRANSFER_ENCODING,
    G_WE_HDR_UPGRADE,
    G_WE_HDR_USER_AGENT,
    G_WE_HDR_VARY,
    G_WE_HDR_VIA,
    G_WE_HDR_WARNING,
    G_WE_HDR_WWW_AUTHENTICATE,
    G_WE_HDR_CONTENT_DISPOSITION,
    G_WE_HDR_X_WAP_APPLICATION_ID,
    G_WE_HDR_X_WAP_CONTENT_URI,
    G_WE_HDR_X_WAP_INITIATOR_URI,
    G_WE_HDR_ACCEPT_APPLICATION,
    G_WE_HDR_BEARER_INDICATION,
    G_WE_HDR_PUSH_FLAG,
    G_WE_HDR_PROFILE,
    G_WE_HDR_PROFILE_DIFF,
    G_WE_HDR_PROFILE_WARNING,
    G_WE_HDR_EXPECT,
    G_WE_HDR_TE,
    G_WE_HDR_TRAILER,
    G_WE_HDR_X_WAP_TOD,
    G_WE_HDR_CONTENT_ID,
    G_WE_HDR_SET_COOKIE,
    G_WE_HDR_COOKIE,
    G_WE_HDR_ENCODING_VERSION,
    G_WE_HDR_X_WAP_SECURITY,
    G_WE_HDR_X_WAP_LOC_INVOCATION,
    G_WE_HDR_X_WAP_LOC_DELIVERY,
    G_WE_HDR_X_WAP_CPITAG ,
    G_WE_HDR_X_WAP_PUSH_PROVURL,
    G_WE_HDR_X_WAP_PUSH_ACCEPT,
    G_WE_HDR_X_WAP_PUSH_ACCEPT_CHARSET,
    G_WE_HDR_X_WAP_PUSH_ACCEPT_ENCODING,
    G_WE_HDR_X_WAP_PUSH_ACCEPT_LANGUAGE,
    G_WE_HDR_X_WAP_PUSH_ACCEPT_APP_ID,
    G_WE_HDR_X_WAP_PUSH_MSG_SIZE,
    G_WE_HDR_X_WAP_PUSH_ACCEPT_MAX_PUSH_REQ  ,
    G_WE_HDR_X_WAP_TERMINAL_ID,
    G_WE_HDR_X_WAP_PUSH_INFO ,
    G_WE_HDR_X_WAP_PUSH_STATUS    ,
    G_WE_HDR_X_WAP_PUSH_OTA_VERSION,
    G_WE_HDR_X_WAP_BEARER_INDICATION,        
    G_WE_HDR_X_WAP_AUTHENTICATE,
    G_WE_HDR_X_WAP_AUTHORIZATION,
    G_WE_HDR_X_WAP_PROFILE,
    G_WE_HDR_X_WAP_PROFILE_DIFF,
    G_WE_HDR_X_WAP_PROFILE_WARNING,
    G_WE_HDR_X_UP_TPD_E2E_HEADERS,
    G_WE_HDR_X_FORWARDED_FOR,
    G_WE_HDR_CONTENT_TRANSFER_ENCODING,
    G_WE_HDR_REFRESH,
    G_WE_HDR_X_DRM,
    G_WE_HDR_SET_COOKIE2,
    G_WE_HDR_COOKIE2,
    G_WE_HDR_X_VODAFONE_3GPDP_CONTEXT,
    G_WE_HDR_X_OMA_DRM_SEPARATE_DELIVERY,
    G_WE_HDR_RIGHTS_ISSUER,
    G_WE_HDR_CONTENT_NAME,
    G_WE_HDR_CONTENT_DESCRIPTION,
    G_WE_HDR_CONTENT_VENDOR,
    G_WE_HDR_ICON_URI,
    G_WE_HDR_ENCRYPTION_METHOD,
    G_WE_HDR_X_VODAFONE_CONTENT, 
    G_WE_HDR_KEEP_ALIVE,
	G_WE_HDR_PROXY_CONNECTION,
	G_WE_HDR_X_SP_RESULT
};

/*The language's string table and its hash table*/
#define WE_LANGUAGES_HASH_TABLE_SIZE                 211
#define WE_LANGUAGES_STRING_TABLE_SIZE               139
const WE_UCHAR aucLangHashTable[WE_LANGUAGES_HASH_TABLE_SIZE] = 
  {
        0x61, 0x12, 0xFF, 0xFF, 0x80, 0x21, 0x56,
        0xFF, 0xFF, 0xFF, 0xFF, 0x76, 0x16, 0xFF,
        0xFF, 0xFF, 0xFF, 0x71, 0xFF, 0xFF, 0x68,
        0x5C, 0x40, 0x01, 0xFF, 0x8A, 0xFF, 0x14,
        0xFF, 0xFF, 0xFF, 0x0D, 0x6E, 0x6B, 0x0C,
        0xFF, 0xFF, 0x3E, 0x65, 0x83, 0x74, 0xFF,
        0xFF, 0x22, 0x52, 0xFF, 0x7D, 0x86, 0x35,
        0x20, 0x32, 0xFF, 0x1F, 0xFF, 0x10, 0x05,
        0xFF, 0x85, 0xFF, 0x2F, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0x4E, 0x25, 0x28, 0x4D, 0xFF,
        0x23, 0x4A, 0x58, 0x1B, 0xFF, 0x3F, 0x19,
        0xFF, 0xFF, 0x67, 0xFF, 0x08, 0x63, 0x2A,
        0x3A, 0xFF, 0x5A, 0x6A, 0x51, 0x6F, 0x7C,
        0x34, 0xFF, 0x07, 0x73, 0x43, 0x88, 0x31,
        0x42, 0x72, 0x5F, 0xFF, 0xFF, 0x2E, 0x89,
        0x02, 0x5B, 0x26, 0x00, 0x15, 0xFF, 0xFF,
        0x38, 0x0F, 0xFF, 0x48, 0xFF, 0x6D, 0x2C,
        0x0B, 0x17, 0xFF, 0xFF, 0x55, 0x13, 0x3C,
        0xFF, 0x82, 0x77, 0xFF, 0x11, 0x5D, 0x50,
        0x29, 0xFF, 0xFF, 0x44, 0x1E, 0xFF, 0xFF,
        0x04, 0xFF, 0xFF, 0x03, 0xFF, 0x27, 0x69,
        0x45, 0xFF, 0xFF, 0xFF, 0x7A, 0x78, 0x4C,
        0xFF, 0xFF, 0x49, 0x0E, 0x1A, 0x46, 0x33,
        0x18, 0x2B, 0x09, 0x66, 0xFF, 0x84, 0x47,
        0xFF, 0x39, 0x7E, 0xFF, 0xFF, 0xFF, 0x87,
        0x7B, 0x24, 0x70, 0x06, 0x4F, 0x7F, 0x75,
        0x37, 0x41, 0x1D, 0x30, 0x81, 0x54, 0x79,
        0x36, 0xFF, 0xFF, 0xFF, 0x53, 0x6C, 0x60,
        0x62, 0xFF, 0x59, 0x1C, 0x2D, 0x57, 0x4B,
        0xFF, 0x0A, 0x5E, 0xFF, 0x3D, 0x64, 0xFF,
        0x3B
  };//we_languages_hash_table    
const WE_CHAR acLangStr[WE_LANGUAGES_STRING_TABLE_SIZE][4]=
{
    {"aa"},
    {"ab"},
    {"af"},
    {"am"},
    {"ar"},
    {"as"},
    {"ay"},
    {"az"},
    {"ba"},
    {"be"},
    {"bg"},
    {"bh"},
    {"bi"},
    {"bn"},
    {"bo"},
    {"br"},
    {"ca"},
    {"co"},
    {"cs"},
    {"cy"},
    {"da"},
    {"de"},
    {"dz"},
    {"el"},
    {"en"},
    {"eo"},
    {"es"},
    {"et"},
    {"eu"},
    {"fa"},
    {"fi"},
    {"fj"},
    {"fo"},
    {"fr"},
    {"fy"},
    {"ga"},
    {"gd"},
    {"gl"},
    {"gn"},
    {"gu"},
    {"ha"},
    {"he"},
    {"hi"},
    {"hr"},
    {"hu"},
    {"hy"},
    {"ia"},
    {"id"},
    {"ie"},
    {"ik"},
    {"is"},
    {"it"},
    {"iu"},
    {"ja"},
    {"jw"},
    {"ka"},
    {"kk"},
    {"kl"},
    {"km"},
    {"kn"},
    {"ko"},
    {"ks"},
    {"ku"},
    {"ky"},
    {"la"},
    {"ln"},
    {"lo"},
    {"lt"},
    {"lv"},
    {"mg"},
    {"mi"},
    {"mk"},
    {"ml"},
    {"mn"},
    {"mo"},
    {"mr"},
    {"ms"},
    {"mt"},
    {"my"},
    {"na"},
    {"ne"},
    {"nl"},
    {"no"},
    {"oc"},
    {"om"},
    {"or"},
    {"pa"},
    {"po"},
    {"ps"},
    {"pt"},
    {"qu"},
    {"rm"},
    {"rn"},
    {"ro"},
    {"ru"},
    {"rw"},
    {"sa"},
    {"sd"},
    {"sg"},
    {"sh"},
    {"si"},
    {"sk"},
    {"sl"},
    {"sm"},
    {"sn"},
    {"so"},
    {"sq"},
    {"sr"},
    {"ss"},
    {"st"},
    {"su"},
    {"sv"},
    {"sw"},
    {"ta"},
    {"te"},
    {"tg"},
    {"th"},
    {"ti"},
    {"tk"},
    {"tl"},
    {"tn"},
    {"to"},
    {"tr"},
    {"ts"},
    {"tt"},
    {"tw"},
    {"ug"},
    {"uk"},
    {"ur"},
    {"uz"},
    {"vi"},
    {"vo"},
    {"wo"},
    {"xh"},
    {"yi"},
    {"yo"},
    {"za"},
    {"zh"},
    {"zu"}

};
const WE_INT32 aiLangStr[WE_LANGUAGES_STRING_TABLE_SIZE]=
{
    G_WE_LANGUAGE_AA,
    G_WE_LANGUAGE_AB,
    G_WE_LANGUAGE_AF,
    G_WE_LANGUAGE_AM,
    G_WE_LANGUAGE_AR,
    G_WE_LANGUAGE_AS,
    G_WE_LANGUAGE_AY,
    G_WE_LANGUAGE_AZ,
    G_WE_LANGUAGE_BA,
    G_WE_LANGUAGE_BE,
    G_WE_LANGUAGE_BG,
    G_WE_LANGUAGE_BH,
    G_WE_LANGUAGE_BI,
    G_WE_LANGUAGE_BN,
    G_WE_LANGUAGE_BO,
    G_WE_LANGUAGE_BR,
    G_WE_LANGUAGE_CA,
    G_WE_LANGUAGE_CO,
    G_WE_LANGUAGE_CS,
    G_WE_LANGUAGE_CY,
    G_WE_LANGUAGE_DA,
    G_WE_LANGUAGE_DE,
    G_WE_LANGUAGE_DZ,
    G_WE_LANGUAGE_EL,
    G_WE_LANGUAGE_EN,
    G_WE_LANGUAGE_EO,
    G_WE_LANGUAGE_ES,
    G_WE_LANGUAGE_ET,
    G_WE_LANGUAGE_EU,
    G_WE_LANGUAGE_FA,
    G_WE_LANGUAGE_FI,
    G_WE_LANGUAGE_FJ,
    G_WE_LANGUAGE_FO,
    G_WE_LANGUAGE_FR,
    G_WE_LANGUAGE_FY,
    G_WE_LANGUAGE_GA,
    G_WE_LANGUAGE_GD,
    G_WE_LANGUAGE_GL,
    G_WE_LANGUAGE_GN,
    G_WE_LANGUAGE_GU,
    G_WE_LANGUAGE_HA,
    G_WE_LANGUAGE_HE,
    G_WE_LANGUAGE_HI,
    G_WE_LANGUAGE_HR,
    G_WE_LANGUAGE_HU,
    G_WE_LANGUAGE_HY,
    G_WE_LANGUAGE_IA,
    G_WE_LANGUAGE_ID,
    G_WE_LANGUAGE_IE,
    G_WE_LANGUAGE_IK,
    G_WE_LANGUAGE_IS,
    G_WE_LANGUAGE_IT,
    G_WE_LANGUAGE_IU,
    G_WE_LANGUAGE_JA,
    G_WE_LANGUAGE_JW,
    G_WE_LANGUAGE_KA,
    G_WE_LANGUAGE_KK,
    G_WE_LANGUAGE_KL,
    G_WE_LANGUAGE_KM,
    G_WE_LANGUAGE_KN,
    G_WE_LANGUAGE_KO,
    G_WE_LANGUAGE_KS,
    G_WE_LANGUAGE_KU,
    G_WE_LANGUAGE_KY,
    G_WE_LANGUAGE_LA,
    G_WE_LANGUAGE_LN,
    G_WE_LANGUAGE_LO,
    G_WE_LANGUAGE_LT,
    G_WE_LANGUAGE_LV,
    G_WE_LANGUAGE_MG,
    G_WE_LANGUAGE_MI,
    G_WE_LANGUAGE_MK,
    G_WE_LANGUAGE_ML,
    G_WE_LANGUAGE_MN,
    G_WE_LANGUAGE_MO,
    G_WE_LANGUAGE_MR,
    G_WE_LANGUAGE_MS,
    G_WE_LANGUAGE_MT,
    G_WE_LANGUAGE_MY,
    G_WE_LANGUAGE_NA,
    G_WE_LANGUAGE_NE,
    G_WE_LANGUAGE_NL,
    G_WE_LANGUAGE_NO,
    G_WE_LANGUAGE_OC,
    G_WE_LANGUAGE_OM,
    G_WE_LANGUAGE_OR,
    G_WE_LANGUAGE_PA,
    G_WE_LANGUAGE_PO,
    G_WE_LANGUAGE_PS,
    G_WE_LANGUAGE_PT,
    G_WE_LANGUAGE_QU,
    G_WE_LANGUAGE_RM,
    G_WE_LANGUAGE_RN,
    G_WE_LANGUAGE_RO,
    G_WE_LANGUAGE_RU,
    G_WE_LANGUAGE_RW,
    G_WE_LANGUAGE_SA,
    G_WE_LANGUAGE_SD,
    G_WE_LANGUAGE_SG,
    G_WE_LANGUAGE_SH,
    G_WE_LANGUAGE_SI,
    G_WE_LANGUAGE_SK,
    G_WE_LANGUAGE_SL,
    G_WE_LANGUAGE_SM,
    G_WE_LANGUAGE_SN,
    G_WE_LANGUAGE_SO,
    G_WE_LANGUAGE_SQ,
    G_WE_LANGUAGE_SR,
    G_WE_LANGUAGE_SS,
    G_WE_LANGUAGE_ST,
    G_WE_LANGUAGE_SU,
    G_WE_LANGUAGE_SV,
    G_WE_LANGUAGE_SW,
    G_WE_LANGUAGE_TA,
    G_WE_LANGUAGE_TE,
    G_WE_LANGUAGE_TG,
    G_WE_LANGUAGE_TH,
    G_WE_LANGUAGE_TI,
    G_WE_LANGUAGE_TK,
    G_WE_LANGUAGE_TL,
    G_WE_LANGUAGE_TN,
    G_WE_LANGUAGE_TO,
    G_WE_LANGUAGE_TR,
    G_WE_LANGUAGE_TS,
    G_WE_LANGUAGE_TT,
    G_WE_LANGUAGE_TW,
    G_WE_LANGUAGE_UG,
    G_WE_LANGUAGE_UK,
    G_WE_LANGUAGE_UR,
    G_WE_LANGUAGE_UZ,
    G_WE_LANGUAGE_VI,
    G_WE_LANGUAGE_VO,
    G_WE_LANGUAGE_WO,
    G_WE_LANGUAGE_XH,
    G_WE_LANGUAGE_YI,
    G_WE_LANGUAGE_YO,
    G_WE_LANGUAGE_ZA,
    G_WE_LANGUAGE_ZH,
    G_WE_LANGUAGE_ZU
};

   
WE_UCHAR *WeHdr_GetNameHashTable()
{
  return (WE_UCHAR *)stNamehashTable;
}

void WeHdr_FreeNameHashTable()
{
    return;
}

St_WeSCLStrTableEntry *WeHdr_GetNameStrings(WE_HANDLE hMem,WE_BOOL bMemMgr)    
{
    WE_INT32    i = 0;
    St_WeSCLStrTableEntry *pstHdrNameStrings = NULL;
	
    if(TRUE == bMemMgr)
    {
        pstHdrNameStrings = BrwMem_Alloc(hMem,sizeof(St_WeSCLStrTableEntry)*WE_HDR_NAME_STRING_TABLE_SIZE);
    }
   else
   {
       pstHdrNameStrings = WE_MALLOC(sizeof(St_WeSCLStrTableEntry)*WE_HDR_NAME_STRING_TABLE_SIZE);
   }
    
    if(NULL == pstHdrNameStrings)
    {
        return NULL; 
    }
	
    for(i=0; i<WE_HDR_NAME_STRING_TABLE_SIZE; i++)
    {
        pstHdrNameStrings[i].pcName = (WE_CHAR *)acHdrNameStr[i];
        pstHdrNameStrings[i].iValue = aiHdrNameStr[i];
    }
    return pstHdrNameStrings;
}



void WeHdr_FreeNameStrings(St_WeSCLStrTableEntry *pstHdrNameStrings,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    if (TRUE == bMemMgr)
    {
        BrwMem_Free(hMem,pstHdrNameStrings);
    }
    else
    {
        WE_FREE((void *)pstHdrNameStrings);
    }
    return;
}


St_WeSCLStrtableInfo *WeHdr_GetHeaderNamesEx(WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    St_WeSCLStrtableInfo St_WeHdrNames = 
    {
        NULL,
        NULL,
        WE_HDR_NAME_HASH_TABLE_SIZE,
        WE_HDR_NAME_STRING_TABLE_SIZE,
        WE_TABLE_SEARCH_BINARY,
        0
    };//we_hdr_names
    St_WeSCLStrtableInfo *pSt_WeHdrNames = NULL;
     
  if (TRUE == bMemMgr)
  {
      pSt_WeHdrNames = BrwMem_Alloc(hMem,sizeof(St_WeHdrNames));

  }
  else
  {
     pSt_WeHdrNames = WE_MALLOC(sizeof(St_WeHdrNames));
  }

  if(NULL == pSt_WeHdrNames)
  {
	  return NULL;
  }
  if (TRUE == bMemMgr)
  {
     St_WeHdrNames.pstStringTable = WeHdr_GetNameStrings(hMem,TRUE);
  }
  else
  {
     St_WeHdrNames.pstStringTable = WeHdr_GetNameStrings(NULL,FALSE);
  }    
  St_WeHdrNames.pucHashTable =WeHdr_GetNameHashTable();
  WE_MEMCPY((St_WeSCLStrtableInfo *)pSt_WeHdrNames,&St_WeHdrNames,sizeof(St_WeHdrNames));
  return pSt_WeHdrNames;
}
 
void WeHdr_FreeHeaderNamesEx(St_WeSCLStrtableInfo * pSt_WeHdrNames,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    if(NULL != pSt_WeHdrNames)
    {
        if (TRUE == bMemMgr)
        {
            WeHdr_FreeNameStrings(pSt_WeHdrNames->pstStringTable,hMem,TRUE);
            if(NULL != pSt_WeHdrNames)
            {
                BrwMem_Free(hMem,pSt_WeHdrNames);
            }
        }
        else
        {
            WeHdr_FreeNameStrings(pSt_WeHdrNames->pstStringTable,NULL,FALSE);
            if(NULL != pSt_WeHdrNames)
            {
                WE_FREE((void *)pSt_WeHdrNames);
            }
        }
    }
    return;
}


WE_UCHAR *WeHdr_GetLangHashTable(void)
{
    return (WE_UCHAR *)aucLangHashTable;
}


void WeHdr_FreeLangHashTable(void)
{
  return;
}

St_WeSCLStrTableEntry *WeHdr_GetLangStrings(WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_INT32    i = 0;
    St_WeSCLStrTableEntry *pstLangStrings = NULL;
	
       
    if (TRUE == bMemMgr)
    {
        pstLangStrings = BrwMem_Alloc(hMem,sizeof(St_WeSCLStrTableEntry)*WE_LANGUAGES_STRING_TABLE_SIZE);
        
    }
    else
    {
        pstLangStrings = WE_MALLOC(sizeof(St_WeSCLStrTableEntry)*WE_LANGUAGES_STRING_TABLE_SIZE);
    }

    if(NULL == pstLangStrings)
    {
        return NULL;
    }
    for(i=0; i<WE_LANGUAGES_STRING_TABLE_SIZE; i++)
    {
        pstLangStrings[i].pcName = (WE_CHAR *)acLangStr[i];
        pstLangStrings[i].iValue = aiLangStr[i];
    }
    return pstLangStrings;
}

void WeHdr_FreeLangStrings(St_WeSCLStrTableEntry *pstLangStrings,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    if (TRUE == bMemMgr)
    {
        BrwMem_Free(hMem,pstLangStrings);
    }
    else
    {
        WE_FREE((void *)pstLangStrings);
    }  
}


St_WeSCLStrtableInfo * WeHdr_GetLanguagesEx(WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    St_WeSCLStrtableInfo stWeLanguages = 
    {
        NULL,
        NULL,
        WE_LANGUAGES_HASH_TABLE_SIZE,
        WE_LANGUAGES_STRING_TABLE_SIZE,
        WE_TABLE_SEARCH_LINEAR,
        0
    };//we_languages
    St_WeSCLStrtableInfo * pstWeLanguages = NULL;


    if (TRUE == bMemMgr)
    {
        pstWeLanguages = BrwMem_Alloc(hMem,sizeof(St_WeSCLStrtableInfo));
        stWeLanguages.pstStringTable = WeHdr_GetLangStrings(hMem,TRUE);
        stWeLanguages.pucHashTable = WeHdr_GetLangHashTable();
    }
    else
    {
        pstWeLanguages = WE_MALLOC(sizeof(St_WeSCLStrtableInfo));
        stWeLanguages.pstStringTable = WeHdr_GetLangStrings(NULL,FALSE);
        stWeLanguages.pucHashTable = WeHdr_GetLangHashTable();
    }
    if(NULL == pstWeLanguages)
    {
      return NULL;
    }
    WE_MEMCPY((St_WeSCLStrtableInfo *)pstWeLanguages,&stWeLanguages,sizeof(St_WeSCLStrtableInfo));
    return pstWeLanguages;
}

void WeHdr_FreeLanguagesEx(St_WeSCLStrtableInfo *pstWeLanguages,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    if(NULL != pstWeLanguages)
    {
        if (TRUE == bMemMgr)
        {
            WeHdr_FreeLangHashTable();
            WeHdr_FreeLangStrings(pstWeLanguages->pstStringTable,hMem,TRUE);
            if(NULL != pstWeLanguages)
            {
                BrwMem_Free(hMem,pstWeLanguages);
            }
        }
        else
        {
            WeHdr_FreeLangHashTable();
             WeHdr_FreeLangStrings(pstWeLanguages->pstStringTable,NULL,FALSE);
             if(NULL != pstWeLanguages)
             {
                 WE_FREE((void *)pstWeLanguages);
             }    
        }
    }
}

/*Define Constant Macro end*/

/*Define Macro Function start*/
#define WE_ATOL   (WE_LONG)WE_SCL_ATOI

#define WE_HDR_IS_QUOTED_STRING(s)  ((WE_SCL_STRLEN(s) > 2) && \
        (s[0] == WE_HDR_CH_DOUBLE_QUOTE) && \
        (s[WE_SCL_STRLEN(s)-1] == WE_HDR_CH_DOUBLE_QUOTE))
        
#define WE_HDR_MEM_FREE_AND_RESET( p,hMem,bMemMgr) \
      if (p && (TRUE == bMemMgr)) { BrwMem_Free(hMem,p); p = NULL; }\
      else{ WE_FREE(p); p = NULL; }
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
typedef struct tagSt_WeHdrElement
{
  WE_INT32 iNameIndx;         /* Well-known header index */
  WE_CHAR *pcName;           /* Unknown header as string, or NULL if well-known */
  WE_CHAR *pcValue;           /* Value, always != NULL */
} St_WehdrElement, *P_St_WeHdrElement;

typedef struct tagSt_WeHdr
{
    St_WehdrElement *pstHdrElement;
    WE_INT32 iMaxSize;
    WE_INT32 iSize;
    WE_INT32 iLastAdded;
}St_WeHdr,*P_St_WeHdr;

/*Define struct &  union type end*/

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
WE_INT32 WeHdr_FindByInt(St_WeHdr *pstHdr,
                         WE_INT32 iFieldName);

WE_INT32 WeHdr_FindByString(St_WeHdr *pstHdr,
                            const WE_CHAR *pcName,
                            WE_INT32 iLen); 

void WeHdr_SplitString(WE_CHAR *pcValue,
                       WE_INT32 iValueLen,
                       WE_CHAR cSplitChar,
                       WE_CHAR *apcParts[],
                       WE_INT32 aiPartsLen[],
                       WE_INT32 *piNumParts);
                       
void WeHdr_MakeHeaderName(WE_CHAR *pcDst,
                          const WE_CHAR *pcSrc);
                          
WE_INT32 WeHdr_FindTextLine(const WE_CHAR *pcText,
                            const WE_CHAR **ppcAfterline);

WE_INT32 WeHdr_FirstField(WE_CHAR *pcS);

WE_BOOL WeHdr_ContainsReservedChars(const WE_CHAR *pcStr);

WE_BOOL WeHdr_Expand(St_WeHdr *pstHdr,WE_HANDLE hMem,WE_BOOL bMemMgr);


WE_BOOL WeHdr_AppendValue(St_WeHdr *pstHdr,WE_INT32 iIndx,const WE_CHAR *pcValue,WE_INT32 iLen,WE_BOOL bCommaSeparated,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_INT32 WeHdr_CalculateTextLength (St_WeHdr *pstHdr,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_BOOL WeHdr_WriteString (St_WeHdr *pstHdr,WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_CHAR * WeHdr_EscapeDoubleQuote(const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_CHAR * WeHdr_TrimParamStr(WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr);

WE_CHAR * WeHdr_TrimUnquoteParamStr(WE_CHAR *pcStr,WE_HANDLE hMemMgr,WE_BOOL bMemMgr);

/*******************************************************************************
*   Global Variable Declare Section
*******************************************************************************/


/*******************************************************************************
*   File Static Variable Define Section
*******************************************************************************/


/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
    WeHdr_Create
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Create a new, empty header table
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
St_WeHdr * WeHdr_CreateEx
(
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    St_WeHdr *pstHdr = NULL;
 
    if(TRUE == bMemMgr)
    {
        if(!hMem)
        {
            return NULL;
        }        
    }
    if (TRUE == bMemMgr)
    {  
        pstHdr = BrwMem_Alloc (hMem,sizeof(St_WeHdr));
        
    }
    else
    { 
        pstHdr = WE_MALLOC (sizeof(St_WeHdr));
    }
    if(pstHdr != NULL)
    {
        pstHdr->iSize = 0;
        pstHdr->iMaxSize = WE_HDR_INITIAL_SIZE;
        pstHdr->iLastAdded = -1;
        if (TRUE == bMemMgr)
        { 
            pstHdr->pstHdrElement = BrwMem_Alloc(hMem,sizeof(St_WehdrElement) * WE_HDR_INITIAL_SIZE);
        }
        else
        {
            pstHdr->pstHdrElement = WE_MALLOC(sizeof(St_WehdrElement) * WE_HDR_INITIAL_SIZE);            
        }
        if(pstHdr->pstHdrElement == NULL)
        {
            if (TRUE == bMemMgr)
            {
                BrwMem_Free (hMem,pstHdr);
            }
            else
            {
                WE_FREE (pstHdr);                
            }       
            pstHdr = NULL;
        }
    }
    return pstHdr;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_Delete
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Deallocate a header table and all its content 
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
void WeHdr_DeleteEx(St_WeHdr *pstHdr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_INT32 iIndx = 0;
    if(pstHdr != NULL)
    {
        for(iIndx = 0;iIndx < pstHdr->iSize;iIndx++)
        {
            if (TRUE == bMemMgr)
            {
                if(pstHdr->pstHdrElement[iIndx].pcName != NULL)
                {
                    BrwMem_Free(hMem,pstHdr->pstHdrElement[iIndx].pcName);
                    pstHdr->pstHdrElement[iIndx].pcName = NULL;
                }
                if(pstHdr->pstHdrElement[iIndx].pcValue != NULL)
                {
                    BrwMem_Free(hMem,pstHdr->pstHdrElement[iIndx].pcValue);
                    pstHdr->pstHdrElement[iIndx].pcValue = NULL;
                }
            }
            else
            {
                WE_FREE(pstHdr->pstHdrElement[iIndx].pcName);
                WE_FREE(pstHdr->pstHdrElement[iIndx].pcValue);
            }
            
        }
        if (TRUE == bMemMgr)
        {
            BrwMem_Free(hMem,pstHdr->pstHdrElement);
            BrwMem_Free(hMem,pstHdr);
        }
        else
        {       
            WE_FREE(pstHdr->pstHdrElement);
            WE_FREE(pstHdr);
        }
    }
}
/*=====================================================================================
FUNCTION: 
    WeHdr_Copy
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Copy a header table 
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
St_WeHdr * WeHdr_CopyEx(St_WeHdr *pstHdr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    St_WeHdr *pstNewHdr = NULL;
    WE_INT32 iIndx = 0;
    WE_CHAR *pcTmp1 = NULL;
    WE_CHAR *pcTmp2 = NULL;

	if(!pstHdr)
	{	
		return NULL;
	}
    if (TRUE == bMemMgr)
    {
        pstNewHdr = BrwMem_Alloc(hMem,sizeof(St_WeHdr));            
    }
    else
    {
        pstNewHdr = WE_MALLOC(sizeof(St_WeHdr));
    }
    if(pstNewHdr == NULL)
        return NULL;
    if (TRUE == bMemMgr)
    {

        pstNewHdr->pstHdrElement = BrwMem_Alloc(hMem,sizeof(St_WehdrElement) * (pstHdr->iMaxSize));        
    }
    else
    {
        pstNewHdr->pstHdrElement = WE_MALLOC(sizeof(St_WehdrElement) * (pstHdr->iMaxSize));
    }    
    if(pstNewHdr->pstHdrElement == NULL)
    {
        if (TRUE == bMemMgr)
        {
            BrwMem_Free(hMem,pstNewHdr);
        }
        else
        {
            WE_FREE(pstNewHdr);
        }    
        return NULL;
    }
    pstNewHdr->iSize = pstHdr->iSize;
    pstNewHdr->iMaxSize = pstHdr->iMaxSize;
    pstNewHdr->iLastAdded = pstHdr->iLastAdded;
    for(iIndx = 0;pstHdr->iSize;iIndx++)
    {
        pstNewHdr->pstHdrElement[iIndx].iNameIndx = pstHdr->pstHdrElement[iIndx].iNameIndx;
        if (TRUE == bMemMgr)
        {
            if( ( (pstHdr->pstHdrElement[iIndx].pcName != NULL) &&
                ((pcTmp1 = Brw_StrDup(hMem,pstHdr->pstHdrElement[iIndx].pcName)) == NULL) ) 
                || ((pcTmp2 = Brw_StrDup(hMem,pstHdr->pstHdrElement[iIndx].pcValue)) == NULL))
                {
                    pstNewHdr->iSize = iIndx;
                    WeHdr_DeleteEx(pstNewHdr,hMem,TRUE);
                    return NULL;
                }
                else
                {
                    pstNewHdr->pstHdrElement[iIndx].pcName = pcTmp1;
                    pstNewHdr->pstHdrElement[iIndx].pcValue = pcTmp2;
                }
        }
        else
        {
             if( (  (pstHdr->pstHdrElement[iIndx].pcName != NULL) &&
                 ((pcTmp1 = WE_SCL_STRDUP(pstHdr->pstHdrElement[iIndx].pcName)) == NULL))
                 || ((pcTmp2 = WE_SCL_STRDUP(pstHdr->pstHdrElement[iIndx].pcValue)) == NULL))
              {
                  pstNewHdr->iSize = iIndx;
                  WeHdr_DeleteEx(pstNewHdr,NULL,FALSE);
                  return NULL;
              }
              else
              {
                  pstNewHdr->pstHdrElement[iIndx].pcName = pcTmp1;
                  pstNewHdr->pstHdrElement[iIndx].pcValue = pcTmp2;
              }
        }       
/*#ifdef _USE_MEM_HANDLE 
        if( (  (pstHdr->pstHdrElement[iIndx].pcName != NULL) &&
            ((pcTmp1 = Brw_StrDup(hMem,pstHdr->pstHdrElement[iIndx].pcName,"")) == NULL)
            )  || ((pcTmp2 = Brw_StrDup(hMem,pstHdr->pstHdrElement[iIndx].pcValue,"")) == NULL)
            )
#else
        if( (  (pstHdr->pstHdrElement[iIndx].pcName != NULL) &&
                 ((pcTmp1 = WE_SCL_STRDUP(pstHdr->pstHdrElement[iIndx].pcName)) == NULL)
             )  || ((pcTmp2 = WE_SCL_STRDUP(pstHdr->pstHdrElement[iIndx].pcValue)) == NULL)
          )
#endif
        {
            pstNewHdr->iSize = iIndx;
#ifdef _USE_MEM_HANDLE
            WeHdr_Delete(pstNewHdr,hMem);
#else            
            WeHdr_Delete(pstNewHdr);
#endif
            return NULL;
        }
        else
        {
            pstNewHdr->pstHdrElement[iIndx].pcName = pcTmp1;
            pstNewHdr->pstHdrElement[iIndx].pcValue = pcTmp2;
        }*/
        
    }
    return pstNewHdr;
}

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
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
void WeHdr_DeleteFieldEx
(
    St_WeHdr *pstHdr,
    WE_INT32 iFieldName,
    WE_HANDLE hMem,
    WE_BOOL    bMemMgr
 )
{
    WE_INT32 iIndx1 = 0;
    WE_INT32 iIndx2 = 0;

	if(!pstHdr)
	{	
		return;
	}

    for(iIndx1 = 0,iIndx2 = 0;iIndx1 < pstHdr->iSize;iIndx1++)
    {
        if(pstHdr->pstHdrElement[iIndx1].iNameIndx == iFieldName)
        {
            if (TRUE == bMemMgr)
            {
                BrwMem_Free(hMem,pstHdr->pstHdrElement[iIndx1].pcName);
                BrwMem_Free(hMem,pstHdr->pstHdrElement[iIndx1].pcValue);
            }
            else
            {      
                WE_FREE(pstHdr->pstHdrElement[iIndx1].pcName);
                WE_FREE(pstHdr->pstHdrElement[iIndx1].pcValue);
            }
        }
        else if (iIndx1 != iIndx2)
        {
            pstHdr->pstHdrElement[iIndx2++] = pstHdr->pstHdrElement[iIndx1];
        }
        else
        {
            iIndx2++;
        }
    }
    pstHdr->iSize = iIndx2;
}

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
    
RETURN VALUE:
    Returns TRUE on success, or FALSE on error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeHdr_AddParamEx
(
    St_WeHdr *pstHdr,
    WE_INT32 iFieldName,
    const WE_CHAR *pcParam,
    const WE_CHAR *pcValue,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    WE_INT32 iIndx = 0;
    WE_CHAR *pcStr = NULL;
    WE_CHAR *pcTmp = NULL;

    if(pstHdr == NULL)
        return FALSE;
    iIndx = WeHdr_FindByInt(pstHdr,iFieldName);
    if(iIndx < 0)
        return FALSE;
    pcStr = WE_SCL_STRSTR(pstHdr->pstHdrElement[iIndx].pcValue,pcParam);
    if(pcStr != NULL)
        return FALSE;
    if (TRUE == bMemMgr)
    {
        pcTmp = BrwMem_Alloc(hMem,TF3RD_STRLEN(pstHdr->pstHdrElement[iIndx].pcValue) + 
            TF3RD_STRLEN(pcParam) + TF3RD_STRLEN(pcValue) + 4);
    }
    else
    {
        pcTmp = WE_MALLOC(WE_SCL_STRLEN(pstHdr->pstHdrElement[iIndx].pcValue) + 
            WE_SCL_STRLEN(pcParam) + WE_SCL_STRLEN(pcValue) + 4);
    }    
    if(pcTmp == NULL)
        return FALSE;
		WE_SCL_SPRINTF(pcTmp, "%s; %s=%s",pstHdr->pstHdrElement[iIndx].pcValue, pcParam, pcValue);
	if (TRUE == bMemMgr)
    {
        BrwMem_Free(hMem,pstHdr->pstHdrElement[iIndx].pcValue);
    }
    else
    {
        WE_FREE(pstHdr->pstHdrElement[iIndx].pcValue);
    }
    pstHdr->pstHdrElement[iIndx].pcValue = pcTmp;
    return TRUE;
}

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
    
RETURN VALUE:
    Returns TRUE on success and FALSE on error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeHdr_AddHeadersFromTextEx
(
    St_WeHdr *pstHdr,
    const WE_CHAR *pcText,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
 )
{
    const WE_CHAR *pcS = NULL;
    const WE_CHAR *pcP = NULL;
    WE_INT32 iLen;

	if(!pstHdr)
	{	
		return FALSE;
	}

    if(pcText == NULL)
        return TRUE;
    pcP = pcText;
    for(;;)
    {
        pcS = pcP;
        iLen = WeHdr_FindTextLine(pcS,&pcP);
        if(iLen == 0)
            break;
        if (TRUE == bMemMgr)
        {
            if(!WeHdr_AddHeaderFromTextLineEx(pstHdr, pcS, iLen,hMem,TRUE))
            {
                return FALSE;
            }
        }
        else
        {
            if(!WeHdr_AddHeaderFromTextLineEx(pstHdr, pcS, iLen,NULL,FALSE))
            {   
                return FALSE;
            }
        }
        
    }
    return TRUE;
}

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
    
RETURN VALUE:
     Returns TRUE on success and FALSE on error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeHdr_MakeStringEx 
(
    St_WeHdr *pstHdr,
    WE_CHAR **ppcStr,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
 )
{
    WE_INT32 iLen = 0;

	if(!pstHdr)
	{
		return FALSE;
	}
	if (TRUE == bMemMgr)
    {
        iLen = WeHdr_CalculateTextLength (pstHdr,hMem,TRUE);
    }
    else
    {
        iLen = WeHdr_CalculateTextLength (pstHdr,NULL,FALSE);
    }
    if(iLen == 0)
    {
        *ppcStr = NULL;
    }
    else
    {
        if (TRUE == bMemMgr)
        {
            *ppcStr = (WE_CHAR *)BrwMem_Alloc(hMem,sizeof(WE_CHAR)*iLen + 1);
        }
        else
        {
            *ppcStr = (WE_CHAR *)WE_MALLOC(sizeof(WE_CHAR)*iLen + 1);
        }
        if(*ppcStr == NULL)
            return FALSE;
        if (TRUE == bMemMgr)
        {
            if(!WeHdr_WriteString(pstHdr, *ppcStr,hMem,TRUE))
            return FALSE;
        }
        else
        {
            if(!WeHdr_WriteString(pstHdr, *ppcStr,NULL,FALSE))
            return FALSE;
        }
    }
    return TRUE;
}

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
    
RETURN VALUE:
    Returns TRUE if a header element was found, and FALSE otherwise.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeHdr_GetUint32Ex
(
    WE_HANDLE hCSC,
    St_WeHdr *pstHdr,
    WE_INT32 iFieldName,
    WE_UINT32 *puiVal,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    WE_INT32 iIndx = 0;
    WE_INT32 iLen = 0;
    WE_UINT32 uiTmp = 0;
    WE_CHAR *pcValue = NULL;
    St_WeSCLStrtableInfo *pstWeMimeTypes = NULL;

	if(!pstHdr)
	{
		return FALSE;
	}
  /* Locate the header, and try to convert the string into an integer */
    for(iIndx = 0;iIndx < pstHdr->iSize;iIndx++)
    {
        if(pstHdr->pstHdrElement[iIndx].iNameIndx == iFieldName)
        {
            pcValue = pstHdr->pstHdrElement[iIndx].pcValue;
            switch (iFieldName)
            {
                case G_WE_HDR_DATE:
                case G_WE_HDR_EXPIRES:
                case G_WE_HDR_IF_MODIFIED_SINCE:
                case G_WE_HDR_IF_UNMODIFIED_SINCE:
                case G_WE_HDR_LAST_MODIFIED:
                    if(!WE_SCL_STR2TIME (pcValue, &uiTmp))
                        return FALSE;
                    break;

                case G_WE_HDR_RETRY_AFTER:
                    if(ct_isdigit(pcValue[0]))
                    {
                        uiTmp = (WE_UINT32)WE_ATOL ((const unsigned char *)pcValue);
                    }
                    else
                    {
                        if(!WE_SCL_STR2TIME(pcValue, &uiTmp))
                            return FALSE;
                    }
                    break;

                case G_WE_HDR_AGE:
                case G_WE_HDR_CONTENT_LENGTH:
                    uiTmp = (WE_UINT32)WE_ATOL ((const unsigned char *)pcValue);
                    break;

                case G_WE_HDR_ACCEPT:
                case G_WE_HDR_CONTENT_TYPE:
                case G_WE_HDR_X_WAP_PUSH_ACCEPT:
                    iLen = WeHdr_FirstField (pcValue);
                    if (TRUE == bMemMgr)
                    {
                        
                        pstWeMimeTypes = WeMime_GetMimeTypesEx(hMem,TRUE);
                       
                    }
                    else
                    {
                       
                        pstWeMimeTypes = WeMime_GetMimeTypesEx(NULL,FALSE);
                        
                    }
                    uiTmp = (WE_UINT32)WE_SCL_STR2INTLC(pcValue, iLen, pstWeMimeTypes);
                    if (TRUE == bMemMgr)
                    {
                        WeMime_FreeMimeTypesEx(pstWeMimeTypes,hMem,TRUE);
                    }
                    else
                    {
                        WeMime_FreeMimeTypesEx(pstWeMimeTypes,NULL,FALSE);
                    }
                    break;

                case G_WE_HDR_ACCEPT_CHARSET:
                case G_WE_HDR_X_WAP_PUSH_ACCEPT_CHARSET:
                    iLen = WeHdr_FirstField (pcValue);
					
                    if(hCSC != NULL)
                    {
                        uiTmp = WE_CSC_STR2INT(hCSC,pcValue, iLen);
                    }
                    break;

                case G_WE_HDR_ACCEPT_LANGUAGE:
                case G_WE_HDR_CONTENT_LANGUAGE:
                case G_WE_HDR_X_WAP_PUSH_ACCEPT_LANGUAGE:
                case G_WE_HDR_ACCEPT_APPLICATION:
                    break;

                default:
                    return FALSE;/* This is not a header whose value we can convert to an integer */
            }
      *puiVal = uiTmp;
      return TRUE;
        }
    }
    return FALSE;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_GetString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Get a string value from the first element with the given header name.
ARGUMENTS PASSED:
    
RETURN VALUE:
    Returns NULL if the requested header element is not in the table.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    The caller MUST NOT deallocate the returned string!
=====================================================================================*/
const WE_CHAR * WeHdr_GetString (St_WeHdr *pstHdr,
                                 WE_INT32 iFieldName)
{
    WE_INT32 iIndx = 0;
	if(!pstHdr)
	{
		return NULL;
	}
    iIndx = WeHdr_FindByInt(pstHdr, iFieldName);
    if(iIndx < 0)
        return NULL;
    else
        return pstHdr->pstHdrElement[iIndx].pcValue;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_GetParameter
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_CHAR * WeHdr_GetParameterEx 
(
    St_WeHdr  *pstHdr,
    WE_INT32  iFieldName,
    const WE_CHAR *pcParameter,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    WE_INT32 iIndx = 0;
    WE_INT32 iLen = 0;
    WE_CHAR *pcTmp = NULL;
    WE_CHAR *pcValue = NULL;
    WE_CHAR *apcParts[8];
    WE_INT32  aiPartsLen[8];
    WE_INT32  iNumParts = 0;

	if(!pstHdr)
	{
		return NULL;
	}

    for(iIndx = 0;iIndx < pstHdr->iSize;iIndx++)
    {
        if(pstHdr->pstHdrElement[iIndx].iNameIndx == iFieldName)
            break;
    }
    if(iIndx < pstHdr->iSize)
    {
        iNumParts = 1;
        WeHdr_SplitString (pstHdr->pstHdrElement[iIndx].pcValue, 
                           WE_SCL_STRLEN(pstHdr->pstHdrElement[iIndx].pcValue),',',
                           apcParts, aiPartsLen, &iNumParts);
        if(iNumParts >= 1)
        {
            iNumParts = 8;
            WeHdr_SplitString (apcParts[0], aiPartsLen[0], ';', apcParts, aiPartsLen, &iNumParts);
            for(iIndx = 1;iIndx < iNumParts;iIndx++)
            {
                pcTmp = WE_SCL_STRNCHR (apcParts[iIndx], '=', aiPartsLen[iIndx]);
                if(pcTmp != NULL)
                {
                    iLen = WE_SCL_SKIPTAILINGBLANKS (apcParts[iIndx], pcTmp - apcParts[iIndx]);
                    if(WE_SCL_STRNCMPNC(apcParts[iIndx], pcParameter, iLen) == 0)
                    {
                        pcTmp = WE_SCL_SKIPBLANKS(pcTmp + 1);
                        iLen = aiPartsLen[iIndx] - (pcTmp - apcParts[iIndx]);
                        if(*pcTmp == '"' && iLen > 1 && pcTmp[iLen-1] == '"')
                        {
                            pcTmp++;
                            iLen -= 2;
                        }
                        if(iLen > 0)
                        {
                            if (TRUE == bMemMgr)
                            {
                               pcValue = Brw_StrnDup(hMem,pcTmp, iLen);  
                            }
                            else
                            {
                                pcValue = WE_SCL_STRNDUP(pcTmp, iLen);
                            }
                        }
                                   /* In case of a corrupt header and the string is just a bunch 
                                   of white spaces, len will be < 0. This causes a crash in 
                                   WESCL_STRNDUP. In this case value will be NULL */
                        break;
                    }
                }
            }
        }
    }
    return pcValue;
}

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
WE_CHAR * WeHdr_BuildParaListEx 
(
    WE_INT32 iNumParams,
    const WE_CHAR *apcParamName[],
    const WE_CHAR *apcParamValue[],
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    WE_CHAR *pcTmp = NULL;
    WE_CHAR *pcConvName = NULL;
    WE_CHAR *pcConvValue = NULL;
    WE_CHAR *pcResultStr = NULL;
    WE_INT32 iIndx = 0;

    for(iIndx = 0;iIndx < iNumParams;iIndx++)
    {
        if(apcParamName[iIndx] == NULL)
            continue; /* If the apcParamName is empty, the whole pair is discarded */
        if(WE_HDR_IS_QUOTED_STRING(apcParamName[iIndx]))
            continue; /* Name string is quoted, does not conform to the HTTP/1.1 
                                 convention, discarding param */
        else
        {
            if(WeHdr_ContainsReservedChars(apcParamName[iIndx]))
                continue; /* parameter name does not conform to the HTTP/1.1 convention, 
                                     discarding param */
            else
            {
                if (TRUE == bMemMgr)
                {
                    pcConvName = Brw_StrDup(hMem,apcParamName[iIndx]);
                }
                else
                {
                    pcConvName = WE_SCL_STRDUP(apcParamName[iIndx]);
                }
            }
                /* the name is a valid token string, keep it as it is */
        }
        /* If the parameter value is missing the parameter is regarded as a boolean */
        if(apcParamValue[iIndx] != NULL)
        {
            if(WE_HDR_IS_QUOTED_STRING(apcParamValue[iIndx]))
            {
              /* String is qouted, check it for unescaped qoutes */
                if (TRUE == bMemMgr)
                {
                    pcConvValue = WeHdr_EscapeDoubleQuote(apcParamValue[iIndx],hMem,TRUE);
                }
                else
                {
                    pcConvValue = WeHdr_EscapeDoubleQuote(apcParamValue[iIndx],NULL,FALSE);
                }
            }
            else
            {
                if(WeHdr_ContainsReservedChars(apcParamValue[iIndx]))
                {
                    if (TRUE == bMemMgr)
                    {
                        pcTmp = Brw_Str3Cat(hMem,
                                            (const WE_CHAR *)WE_HDR_STR_DOUBLE_QUOTE,
                                            apcParamValue[iIndx],
                                            (const WE_CHAR *)WE_HDR_STR_DOUBLE_QUOTE);  
                        pcConvValue = WeHdr_EscapeDoubleQuote(pcTmp,hMem,TRUE);
                    }
                    else
                    {
                        pcTmp = WE_SCL_STR3CAT((const WE_CHAR *)WE_HDR_STR_DOUBLE_QUOTE,
                                              apcParamValue[iIndx],
                                              (const WE_CHAR *)WE_HDR_STR_DOUBLE_QUOTE);
                        pcConvValue = WeHdr_EscapeDoubleQuote(pcTmp,NULL,FALSE);
                    }
                    
                    if(pcTmp != NULL)
                    {
                        if (TRUE == bMemMgr)
                        {
                            
                        }
                        else
                        {
                            WE_FREE((void *)pcTmp);
                        }
                        pcTmp = NULL;
                    }
                }
                else
                {
                    if (TRUE == bMemMgr)
                    {
                        pcConvValue = Brw_StrDup(hMem,apcParamValue[iIndx]);
                    }
                    else
                    {
                        pcConvValue = WE_SCL_STRDUP(apcParamValue[iIndx]);
                    }
                    /* the name is a valid token string, keep it as it is */
                }
            }
        }
        if(pcResultStr)
        {
            if (TRUE == bMemMgr)
            {     
                pcTmp = Brw_Str3Cat(hMem,
                    (WE_CHAR *)pcResultStr, 
                    (WE_CHAR *) WE_HDR_STR_SEMI_COMMA,
                    (WE_CHAR *) pcConvName);
            }
            else
            {
                pcTmp = WE_SCL_STR3CAT((WE_CHAR *)pcResultStr, 
                                      (WE_CHAR *) WE_HDR_STR_SEMI_COMMA,
                                      (WE_CHAR *) pcConvName);
            }
            /* parameter content formatted, build result string */
        }
        else
        {
            pcTmp = pcConvName;
            pcConvName = NULL;
        }
        if(pcConvValue)
        {
            if (TRUE == bMemMgr)
            {
                if(pcResultStr)
                    BrwMem_Free(hMem,pcResultStr);
                pcResultStr = Brw_Str3Cat(hMem,
                                            (WE_CHAR *)pcTmp,
                                            (WE_CHAR *)WE_HDR_STR_EQUAL,
                                            (WE_CHAR *)pcConvValue);
            }
            else
            {
                if(pcResultStr)
                    WE_FREE(pcResultStr);
                pcResultStr = WE_SCL_STR3CAT((WE_CHAR *)pcTmp,
                                        (WE_CHAR *)WE_HDR_STR_EQUAL,
                                        (WE_CHAR *)pcConvValue);               
            }
        }
        else
        {
            if(pcResultStr)
            {
                if (TRUE == bMemMgr)
                {
                    BrwMem_Free(hMem,pcResultStr);
                }
                else
                {
                    WE_FREE(pcResultStr);
                }
            }
            pcResultStr = pcTmp;
            pcTmp = NULL;
        }
        if (TRUE == bMemMgr)
        {
            WE_HDR_MEM_FREE_AND_RESET(pcTmp,hMem,TRUE);
            WE_HDR_MEM_FREE_AND_RESET(pcConvName,hMem,TRUE);
            WE_HDR_MEM_FREE_AND_RESET(pcConvValue,hMem,TRUE);
        }
        else
        {
            WE_HDR_MEM_FREE_AND_RESET(pcTmp,NULL,FALSE);
            WE_HDR_MEM_FREE_AND_RESET(pcConvName,NULL,FALSE);
            WE_HDR_MEM_FREE_AND_RESET(pcConvValue,NULL,FALSE);
        }
    }
    return pcResultStr;
}

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

WE_INT32 WeHdr_GetParamsEx
( 
    const WE_CHAR *pcParamList,
    WE_INT32 iMaxParams,
    WE_CHAR *apcParamName[],
    WE_CHAR *apcParamValue[],
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    WE_INT32 iIndx = 0;
    WE_INT32 iParamCount = 0;
    WE_CHAR *pcTmp = NULL;
    WE_CHAR *pcPL = NULL;
    WE_BOOL bInsideQuotedStr = FALSE;

    pcPL = (WE_CHAR *)pcParamList;
    if(pcPL == NULL)
        return iParamCount;
    iIndx = 0;
    /* avoid wild pointers */
    while(iIndx < iMaxParams)
    {
        apcParamName[iIndx] = NULL;
        apcParamValue[iIndx] = NULL;
        iIndx++;
    }
    /* Skip any leading blanks */
    while(*pcPL == WE_HDR_CH_BLANK)
    {
        pcPL++;
    }

    if((*pcPL != WE_HDR_CH_SEMI_COMMA) && (*pcPL != '\0'))
    {
        if(iParamCount < iMaxParams)
            apcParamName[iParamCount] = pcPL;
        iParamCount++;
    }

    /* traverse the string */
    while(*pcPL != '\0')
    {
     /* check if it is a parameter delimiter ';' */
        if((!bInsideQuotedStr) && (*pcPL == WE_HDR_CH_SEMI_COMMA) && (*(pcPL+1) != '\0'))
        {
            if(iParamCount <= iMaxParams)
            {
                if(iParamCount < iMaxParams)
                    apcParamName[iParamCount] = pcPL + 1;
                if(iParamCount)
                {
                    pcTmp = apcParamName[iParamCount - 1];
                    if(!pcTmp)
                    {
                        pcTmp = apcParamName[iParamCount - 1];
                        if (TRUE == bMemMgr)
                        {
                            apcParamName[iParamCount - 1] = BrwMem_Alloc(hMem,(pcPL - pcTmp) + 1);
                        }
                        else
                        {
                            apcParamName[iParamCount - 1] = WE_MALLOC((pcPL - pcTmp) + 1);
                        }
                        if(!apcParamName[iParamCount - 1])
                            return -1;
							WE_STRNCPY (apcParamName[iParamCount - 1], (WE_CHAR *)pcTmp, (pcPL - pcTmp));
                        apcParamName[iParamCount - 1][pcPL - pcTmp] = '\0';
                    }
                    else
                    {
                        if (TRUE == bMemMgr)
                        {
                            apcParamValue[iParamCount - 1] = BrwMem_Alloc(hMem,(pcPL - pcTmp) + 1);
                        }
                        else
                        {
                            apcParamValue[iParamCount - 1] = WE_MALLOC((pcPL - pcTmp) + 1);
                        }
                        if(!apcParamValue[iParamCount - 1])
                            return -1;
                        WE_STRNCPY (apcParamValue[iParamCount - 1], (WE_CHAR *)pcTmp,(pcPL - pcTmp));
                        apcParamValue[iParamCount - 1][pcPL - pcTmp] = '\0';
                    }
                }
            }
            iParamCount++;
        }
        /* check if it is a value delimiter '=' */
        else if ((!bInsideQuotedStr) && (*pcPL == WE_HDR_CH_EQUAL) && (*(pcPL + 1) != '\0'))
        {
            if((iParamCount) && (iParamCount <= iMaxParams))
            {
                apcParamValue[iParamCount - 1] = pcPL + 1;
                pcTmp = apcParamName[iParamCount - 1];
                if (TRUE == bMemMgr)
                {
                    apcParamName[iParamCount - 1] = BrwMem_Alloc(hMem,(pcPL - pcTmp) + 1);
                }
                else
                {
                    apcParamName[iParamCount - 1] = WE_MALLOC((pcPL - pcTmp) + 1);
                }
                if(!apcParamName[iParamCount - 1])
                    return -1;
                WE_STRNCPY (apcParamName[iParamCount - 1], (WE_CHAR *)pcTmp, (pcPL - pcTmp));
                apcParamName[iParamCount - 1][pcPL - pcTmp] = '\0';
            }
            else if (!iParamCount)
                return -1;/* Malformatted parameter string */
        }
        else if (*pcPL != WE_HDR_CH_DOUBLE_QUOTE)
        {
            if (pcPL == pcParamList)
                bInsideQuotedStr = TRUE;
            else if (*(pcPL - 1) != WE_HDR_CH_BACKSLASH)
                bInsideQuotedStr = !bInsideQuotedStr;
        }
        pcPL++;
    }
    /* Copy the last parameter/value */
    if(iParamCount <= iMaxParams)
    {
        if(iParamCount)
        {
            pcTmp = apcParamValue[iParamCount - 1];
            if(!pcTmp)
            {
                pcTmp = apcParamName[iParamCount - 1];
                if (TRUE == bMemMgr)
                {
                    apcParamName[iParamCount - 1] = BrwMem_Alloc(hMem,(pcPL - pcTmp) + 1);
                }
                else
                {
                    apcParamName[iParamCount - 1] = WE_MALLOC((pcPL - pcTmp) + 1);
                }
                if(!apcParamName[iParamCount - 1])
                    return -1;
                WE_STRNCPY (apcParamName[iParamCount - 1], (WE_CHAR *)pcTmp, (pcPL - pcTmp));
                apcParamName[iParamCount - 1][pcPL - pcTmp] = '\0';
            }
            else
            {
                if (TRUE == bMemMgr)
                {
                    apcParamValue[iParamCount - 1] = BrwMem_Alloc(hMem,(pcPL - pcTmp) + 1);
                }
                else
                {
                    apcParamValue[iParamCount - 1] = WE_MALLOC((pcPL - pcTmp) + 1);
                }
                if(!apcParamValue[iParamCount - 1])
                    return -1;
                WE_STRNCPY (apcParamValue[iParamCount - 1], (WE_CHAR *)pcTmp, (pcPL - pcTmp));
                apcParamValue[iParamCount - 1][pcPL - pcTmp] = '\0';
            }
        }
    }

    /* Trim and unquote the value strings */
    for(iIndx = 0;iIndx < iMaxParams;iIndx++)
    {
        if(apcParamName[iIndx])
        {
            pcTmp = apcParamName[iIndx];
            if (TRUE == bMemMgr)
            {      
                apcParamName[iIndx]= WeHdr_TrimParamStr(pcTmp,hMem,TRUE);
                BrwMem_Free(hMem,pcTmp);
            }
            else
            {
                apcParamName[iIndx]= WeHdr_TrimParamStr(pcTmp,NULL,FALSE);
                WE_FREE(pcTmp);
            }            
            if(!apcParamName[iIndx])
                return -1;
        }
        if(apcParamValue[iIndx])
        {
            pcTmp = apcParamValue[iIndx];
            if (TRUE == bMemMgr)
            {
                apcParamValue[iIndx] = WeHdr_TrimUnquoteParamStr(pcTmp,hMem,TRUE);
                BrwMem_Free(hMem,pcTmp);
            }
            else
            {
                apcParamValue[iIndx] = WeHdr_TrimUnquoteParamStr(pcTmp,NULL,FALSE);
                WE_FREE(pcTmp);
            }
            if(!apcParamValue[iIndx])
                return -1;
        }
    }
    return iParamCount;
}


/*Local Functions*/

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
WE_BOOL WeHdr_AddStringEx
(
    St_WeHdr *pstHdr,
    WE_INT32 iFieldName,
     WE_CHAR *pcValue,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    WE_INT32 iIndx = 0;

    if((pstHdr == NULL) || (pcValue == NULL) || (iFieldName < 0) || (iFieldName >= G_WE_HDR_UNKNOWN))
        return FALSE;
    iIndx = WeHdr_FindByInt(pstHdr, iFieldName);
    if(iIndx >= 0)
    {
        if (TRUE == bMemMgr)
        {
            if(!WeHdr_AppendValue(pstHdr, iIndx,pcValue, WE_SCL_STRLEN(pcValue), TRUE,hMem,TRUE))
            
                return FALSE;
        }
        else
        {
            if(!WeHdr_AppendValue(pstHdr, iIndx,pcValue, WE_SCL_STRLEN(pcValue), TRUE,NULL,FALSE))
            return FALSE;
        }
    }
    else
    {
        if(pstHdr->iSize == pstHdr->iMaxSize)
        {
            if (TRUE == bMemMgr)
            {
                if(!WeHdr_Expand(pstHdr,hMem,TRUE))
                
                    return FALSE;
            }
            else
            {
                if(!WeHdr_Expand(pstHdr,NULL,FALSE))
                return FALSE;
            }
        }
        iIndx = pstHdr->iSize;
        pstHdr->pstHdrElement[iIndx].iNameIndx = iFieldName;
        pstHdr->pstHdrElement[iIndx].pcName = NULL;
        if (TRUE == bMemMgr)
        {
            pstHdr->pstHdrElement[iIndx].pcValue = Brw_StrDup(hMem,pcValue);
            if(NULL == pstHdr->pstHdrElement[iIndx].pcValue)
            {
                return FALSE;
            }
        }
        else
        {
            pstHdr->pstHdrElement[iIndx].pcValue = WE_SCL_STRDUP(pcValue);
        }
        pstHdr->iSize++;
    }
    pstHdr->iLastAdded = iIndx;
    return TRUE;
 }

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
WE_BOOL WeHdr_AddHeaderFromTextLineEx
(
    St_WeHdr *pstHdr,
    const WE_CHAR *pcStr,
    WE_INT32 iLen,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
 )
                                    
{
    WE_INT32 iIndx = 0;
    WE_INT32 iNameLen = 0;
    WE_INT32 iValueLen = 0;
    WE_INT32 iHdrNbr = 0;
    const WE_CHAR *pcName = NULL;
    const WE_CHAR *pcValue = NULL;
    WE_CHAR *pcTmp = NULL;
    St_WeSCLStrtableInfo *pstHdrNames = NULL; 

	if(!pstHdr)
	{
		return FALSE;
	}
	

    /* First check if this is a continuation line, belonging to the previous header. */
    if((pcStr[0] == ' ') || (pcStr[0] == '\t'))
    {
        if(pstHdr->iLastAdded < 0)
            return FALSE;
        if (TRUE == bMemMgr)
        {
            return WeHdr_AppendValue(pstHdr, pstHdr->iLastAdded, pcStr, iLen, FALSE,hMem,TRUE);
        }
        else
        {
            return WeHdr_AppendValue(pstHdr, pstHdr->iLastAdded, pcStr, iLen, FALSE,NULL,FALSE);            
        }
    }
    pcTmp = WE_SCL_STRNCHR(pcStr,':',iLen);
    if(pcTmp == NULL)
        return FALSE;
    pcValue = WE_SCL_SKIPBLANKS(pcTmp + 1);
    while((pcTmp > pcStr) && (*pcTmp == ' '))
        pcTmp--;
    if(pcTmp == pcStr)
        return FALSE;
    iNameLen = pcTmp - pcStr;
    pcName = pcStr;
    if (TRUE == bMemMgr)
    {
        pstHdrNames= WeHdr_GetHeaderNamesEx(hMem,TRUE);
    }
    else
    {
        pstHdrNames= WeHdr_GetHeaderNamesEx(NULL,FALSE);
    }
    if(NULL == pstHdrNames)
	return FALSE;
    iHdrNbr = WE_SCL_STR2INTLC (pcName, iNameLen, pstHdrNames);
    if (TRUE == bMemMgr)
    {
        WeHdr_FreeHeaderNamesEx(pstHdrNames,hMem,TRUE);
    }
    else
    {
        WeHdr_FreeHeaderNamesEx(pstHdrNames,NULL,FALSE);
    }

    iValueLen = WE_SCL_SKIPTAILINGBLANKS(pcValue, iLen - (pcValue - pcStr));
    iIndx = WeHdr_FindByInt(pstHdr, iHdrNbr);
    if(iIndx < 0)
        iIndx = WeHdr_FindByString(pstHdr, pcName, iNameLen);
    if(iIndx >= 0)
    {
        if (TRUE == bMemMgr)
        {    
            return WeHdr_AppendValue(pstHdr, iIndx, pcValue, iValueLen, TRUE,hMem,TRUE);
        }
        else
        { 
            return WeHdr_AppendValue(pstHdr, iIndx, pcValue, iValueLen, TRUE,NULL,FALSE);
        }
    }
    if(pstHdr->iSize == pstHdr->iMaxSize)
    {
        if (TRUE == bMemMgr)
        {
            if(!WeHdr_Expand(pstHdr,hMem,TRUE))
            {
                return FALSE;
            }
        }
        else
        {
            if(!WeHdr_Expand(pstHdr,NULL,FALSE))
            {
                return FALSE;
            }
        }
            
    }
    /* If it is a header name that we do not know about, store the text version of the name. */
    if((iHdrNbr < 0) || (iHdrNbr >= G_WE_HDR_UNKNOWN))
    {
        pstHdr->pstHdrElement[pstHdr->iSize].iNameIndx = G_WE_HDR_UNKNOWN;
        if (TRUE == bMemMgr)
        {
            if((pcTmp = Brw_StrnDup(hMem,pcName,iNameLen)) == NULL)
            {     
                return FALSE;
            }
        }
        else
        {
            if((pcTmp = WE_SCL_STRNDUP(pcName,iNameLen)) == NULL)
            {     
                return FALSE;
            }
        }
        pstHdr->pstHdrElement[pstHdr->iSize].pcName = pcTmp;
    }
    else
    {
        pstHdr->pstHdrElement[pstHdr->iSize].iNameIndx = iHdrNbr;
        pstHdr->pstHdrElement[pstHdr->iSize].pcName = NULL;
    }
    if (TRUE == bMemMgr)
    {
        if((pcTmp = Brw_StrnDup (hMem,pcValue,iValueLen)) == NULL)
        {
            BrwMem_Free(hMem,pstHdr->pstHdrElement[pstHdr->iSize].pcName);      
            return FALSE;
        }
    }
    else
    {
        if((pcTmp = WE_SCL_STRNDUP (pcValue,iValueLen)) == NULL)
        {
            WE_FREE(pstHdr->pstHdrElement[pstHdr->iSize].pcName);
            return FALSE;
        }
    }
    pstHdr->pstHdrElement[pstHdr->iSize].pcValue = pcTmp;
    pstHdr->iLastAdded = pstHdr->iSize;
    pstHdr->iSize++;
    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_FindByInt
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_INT32 WeHdr_FindByInt(St_WeHdr *pstHdr,
                         WE_INT32 iFieldName)
{
    WE_INT32 iIndx = 0;
	if(!pstHdr)
	{
		return -1;
	}
    for(iIndx = 0; iIndx < pstHdr->iSize; iIndx++)
    {
        if(pstHdr->pstHdrElement[iIndx].iNameIndx == iFieldName)
            return iIndx;
    }
    return -1;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_FindByString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_INT32 WeHdr_FindByString(St_WeHdr *pstHdr,
                            const WE_CHAR *pcName,
                            WE_INT32 iLen)
{
    WE_INT32 iIndx = 0;
	if(!pstHdr)
	{
		return -1;
	}

    for(iIndx = 0; iIndx < pstHdr->iSize; iIndx++)
    {
        if((pstHdr->pstHdrElement[iIndx].pcName != NULL) && 
            (WE_SCL_STRNCMPNC(pstHdr->pstHdrElement[iIndx].pcName,pcName,iLen) == 0))
        {
            return iIndx;
        }
    }
    return -1;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_Expand
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeHdr_Expand(St_WeHdr *pstHdr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_INT32 iIndx = 0;
	St_WehdrElement *pstElement = NULL;
	WE_INT32 iNewMaxSize = 0;
	
	if(!pstHdr)
	{
		return FALSE;
	}
    iNewMaxSize = pstHdr->iMaxSize + WE_HDR_SIZE_INCREMENT;	
    if (TRUE == bMemMgr)
    {
        pstElement = BrwMem_Alloc(hMem,iNewMaxSize * (sizeof(St_WehdrElement)));
    }
    else
    {
        pstElement = WE_MALLOC(iNewMaxSize * (sizeof(St_WehdrElement)));
    }
    if(pstElement == NULL)
        return FALSE;

    for(iIndx = 0; iIndx < pstHdr->iSize; iIndx++)
    {
        pstElement[iIndx] = pstHdr->pstHdrElement[iIndx];
    }
    if (TRUE == bMemMgr)
    {
        BrwMem_Free(hMem,pstHdr->pstHdrElement);
    }
    else
    {
        WE_FREE(pstHdr->pstHdrElement);
    }
    pstHdr->pstHdrElement = pstElement;
    pstHdr->iMaxSize = iNewMaxSize;
    
    return TRUE;
}


/*=====================================================================================
FUNCTION: 
    WeHdr_SplitString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
void WeHdr_SplitString(WE_CHAR *pcValue,
                       WE_INT32 iValueLen,
                       WE_CHAR cSplitChar,
                       WE_CHAR *apcParts[],
                       WE_INT32 aiPartsLen[],
                       WE_INT32 *piNumParts)
{
    WE_CHAR *pcS = NULL;
    WE_CHAR *pcT = NULL;
    WE_CHAR *pcEnd = NULL;
    WE_CHAR cQc = '\0';
    WE_INT32 iSLen = 0;
    WE_INT32 iN = 0;
    WE_BOOL bEnd = FALSE;

    if(NULL == pcValue)
        return;
    pcEnd = pcValue + iValueLen;
    pcT = pcValue;
    while(pcT < pcEnd)
    {
        pcS = WE_SCL_SKIPBLANKS(pcT);

	 if((NULL == pcT) || (NULL == pcS))
            return;
        for(pcT = pcS;(pcT < pcEnd) && (*pcT != cSplitChar); pcT++)
        {
            if((pcT[0] == '"') ||(pcT[0] == '\''))
            {
                cQc = pcT[0];
                pcT = WE_SCL_STRCHR(pcT + 1, cQc);
                if((pcT == NULL) || (pcT >= pcEnd))
                {
                    bEnd = TRUE;
                    break;
                }
            }
        }
        if(TRUE == bEnd)
            break;
        else
        {
            iSLen = WE_SCL_SKIPTAILINGBLANKS(pcS, pcT - pcS);
            apcParts[iN] = pcS;
            aiPartsLen[iN++] = iSLen;
            if(iN >= *piNumParts)
                break;
            pcT++;
        }
    }
    *piNumParts = iN;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_AppendValue
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeHdr_AppendValue
(
    St_WeHdr *pstHdr,
    WE_INT32 iIndx,
    const WE_CHAR *pcValue,
    WE_INT32 iLen,
    WE_BOOL bCommaSeparated,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    WE_CHAR *pcStr = NULL;
    WE_CHAR *pcTmp1 = NULL;
    WE_CHAR *pcTmp2 =NULL;
    WE_INT32 iOldLen = 0;
    WE_INT32 iNewLen = 0;

	if(!pstHdr)
	{
		return FALSE;
	}

    pcStr = WE_SCL_SKIPBLANKS(pcValue);
    iLen = WE_SCL_SKIPTAILINGBLANKS(pcStr, iLen - (pcStr - pcValue));
    iOldLen = WE_SCL_STRLEN(pstHdr->pstHdrElement[iIndx].pcValue);
    iNewLen = iOldLen + iLen + 1;
    if(bCommaSeparated)
        iNewLen++;
    if (TRUE == bMemMgr)
    {
        pcTmp1 = BrwMem_Alloc(hMem,iNewLen + 1);
    }
    else
    {
        pcTmp1 = WE_MALLOC(iNewLen + 1);
    }
    pcTmp2 = pcTmp1;
    if(pcTmp1 == NULL)
        return FALSE;
    WE_STRCPY(pcTmp2, pstHdr->pstHdrElement[iIndx].pcValue);
    pcTmp2 += iOldLen;
    if(bCommaSeparated)
    {
        WE_STRCPY(pcTmp2, ", ");
   pcTmp2 += 2;
    }
    else
    {
        WE_STRCPY(pcTmp2, " ");
        pcTmp2 += 1;
    }
    if(iLen < 0)
    {
        return FALSE;
    }
    WE_STRNCPY(pcTmp2, pcStr, iLen);
    pcTmp2[iLen] = '\0';
    if (TRUE == bMemMgr)
    {
        BrwMem_Free(hMem,pstHdr->pstHdrElement[iIndx].pcValue);
    }
    else
    {
        WE_FREE(pstHdr->pstHdrElement[iIndx].pcValue);
    }
    pstHdr->pstHdrElement[iIndx].pcValue = pcTmp1;
    pstHdr->iLastAdded = iIndx;

    return TRUE;
}


/*=====================================================================================
FUNCTION: 
    WeHdr_MakeHeaderName
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
void WeHdr_MakeHeaderName(WE_CHAR *pcDest,
                          const WE_CHAR *pcSrc)
{
    const WE_CHAR *pcTmp = NULL;
    WE_BOOL bIsFirst = TRUE;
    
    for(pcTmp = pcSrc; *pcTmp != '\0'; pcTmp++)
    {
        if(bIsFirst)
        {
            if(ct_isalpha(*pcTmp) && ct_islower(*pcTmp))
                *pcDest++ =(WE_CHAR) (*pcTmp + 'A' - 'a');
            else
                *pcDest++ =*pcTmp;
            bIsFirst = FALSE;
        }
        else
        {
            *pcDest++ = *pcTmp;
            if(!ct_isalphanum(*pcTmp))
                bIsFirst = TRUE;
        }
    }
    *pcDest++ = ':';
    *pcDest++ = ' ';
    *pcDest = '\0';
}


/*=====================================================================================
FUNCTION: 
    WeHdr_FindTextLine
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_INT32 WeHdr_FindTextLine(const WE_CHAR *pcText,
                            const WE_CHAR **ppcAfterLine)
{
    const WE_CHAR *pcStr = NULL;

    *ppcAfterLine = NULL;
    if(pcText == NULL)
        return 0;

    for(pcStr = pcText; *pcStr; pcStr++)
    {
        if(pcStr[0] == '\r')
        {
            if(pcStr[1] == '\n')
            {
                *ppcAfterLine = pcStr + 2;
                break;
            }
            else if(pcStr[1] == '\0')
            {
                *ppcAfterLine = pcStr + 1;
                break;
            }
        }
        else if (pcStr[0] == '\n')
        {
            *ppcAfterLine = pcStr + 1;
             break;
        }
    }
    return (pcStr - pcText);
}


/*=====================================================================================
FUNCTION: 
    WeHdr_FirstField
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_INT32 WeHdr_FirstField(WE_CHAR *pcStr)
{
    WE_CHAR *pcIndx = NULL;
    for(pcIndx = pcStr; *pcIndx && (*pcIndx != ',') && (*pcIndx != ';');pcIndx++);
        return pcIndx-pcStr;
}


/*=====================================================================================
FUNCTION: 
    WeHdr_FirstField
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Calculate the total length of the headerstream that needs to be built, and return the result.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_INT32 WeHdr_CalculateTextLength(St_WeHdr *pstHdr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_INT32 iLen = 0;
    WE_INT32 iIndx = 0;
    const WE_CHAR *pcName = NULL;
    St_WeSCLStrtableInfo *pstHdrNames = NULL;

    if (TRUE == bMemMgr)
    {
        pstHdrNames = WeHdr_GetHeaderNamesEx(hMem,TRUE);
    }
    else
    {
        pstHdrNames = WeHdr_GetHeaderNamesEx(NULL,FALSE);
    }
    
    if(NULL == pstHdrNames)
	return iLen;
    for(iIndx = 0;iIndx < pstHdr->iSize;iIndx++)
    {
        if(pstHdr->pstHdrElement[iIndx].pcName != NULL)
        {
            pcName = pstHdr->pstHdrElement[iIndx].pcName;
        }
        else
        {
      
            pcName = WE_SCL_INT2STR(pstHdr->pstHdrElement[iIndx].iNameIndx,
                                    (const St_WeSCLStrtableInfo *)pstHdrNames);
      
        }
        if(pcName != NULL)
        {
            iLen += WE_SCL_STRLEN(pcName) + WE_SCL_STRLEN(": ");
        }
        iLen += WE_SCL_STRLEN(pstHdr->pstHdrElement[iIndx].pcValue) + WE_SCL_STRLEN("\r\n");
        pcName = NULL;
    }
    iLen += WE_SCL_STRLEN("\r\n");
    if (TRUE == bMemMgr)
    { 
        WeHdr_FreeHeaderNamesEx((St_WeSCLStrtableInfo *)pstHdrNames,hMem,TRUE);
    }
    else
    {
        WeHdr_FreeHeaderNamesEx((St_WeSCLStrtableInfo *)pstHdrNames,NULL,FALSE);
    }
    return iLen; //janet
}

/*=====================================================================================
FUNCTION: 
    WeHdr_WriteString
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    The main function for creating the textual header 
ARGUMENTS PASSED:
    
RETURN VALUE:
    Returns TRUE on sucess, else FALSE.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeHdr_WriteString(St_WeHdr *pstHdr,WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_INT32 iIndx = 0;
    WE_INT32 iLen = 0;
    const WE_CHAR *pcName = NULL;
    St_WeSCLStrtableInfo *pstHdrNames = NULL;
    //WE_CHAR *pcTmp = pcStr;

    if (TRUE == bMemMgr)
    { 
        pstHdrNames = WeHdr_GetHeaderNamesEx(hMem,TRUE);
    }
    else
    {   
        pstHdrNames = WeHdr_GetHeaderNamesEx(NULL,FALSE);
    }
    if(NULL == pstHdrNames)
	return FALSE;
    for(iIndx = 0; iIndx < pstHdr->iSize; iIndx++)
    {
        if(pstHdr->pstHdrElement[iIndx].pcName != NULL)
        {
            pcName = pstHdr->pstHdrElement[iIndx].pcName;
        }
        else
    {
            pcName = WE_SCL_INT2STR(pstHdr->pstHdrElement[iIndx].iNameIndx,
                                   (const St_WeSCLStrtableInfo *)pstHdrNames);
    }
        if(NULL == pcName)
        {
            if (TRUE == bMemMgr)
            {
                WeHdr_FreeHeaderNamesEx((St_WeSCLStrtableInfo *)pstHdrNames,hMem,TRUE);
            }
            else
            {
                WeHdr_FreeHeaderNamesEx((St_WeSCLStrtableInfo *)pstHdrNames,NULL,FALSE);
            }
            return FALSE;
        }
        WeHdr_MakeHeaderName(pcStr,pcName);

        pcStr += WE_SCL_STRLEN(pcName) + WE_SCL_STRLEN(": ");

        iLen = WE_SCL_STRLEN(pstHdr->pstHdrElement[iIndx].pcValue);
        WE_STRCPY(pcStr, pstHdr->pstHdrElement[iIndx].pcValue);
        pcStr += iLen;

        WE_STRCPY(pcStr, "\r\n");
        pcStr += 2;
    }
    if (TRUE == bMemMgr)
    {
        WeHdr_FreeHeaderNamesEx((St_WeSCLStrtableInfo *)pstHdrNames,hMem,TRUE);
    }
    else
    {
        WeHdr_FreeHeaderNamesEx((St_WeSCLStrtableInfo *)pstHdrNames,NULL,FALSE);
    }
    return TRUE;
}


/*=====================================================================================
FUNCTION: 
    WeHdr_EscapeDoubelQuote
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Utility function that scans the string for the double quote character.  All occurrences 
    are replaced with an escaped representation of the characters.
ARGUMENTS PASSED:
    
RETURN VALUE:
    A string with escaped double quotes
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    The caller is responsible for deallocating the returned memory buffer.
=====================================================================================*/
WE_CHAR * WeHdr_EscapeDoubleQuote
(
    const WE_CHAR *pcStr,
    WE_HANDLE hMem,
    WE_BOOL bMemMgr
)
{
    WE_CHAR *pcTmp = NULL;
    WE_CHAR *pcResultStr = NULL;
    WE_INT32 iNumOfOccurrences = 0;
    WE_INT32 iNewStrLen = 0;
    WE_INT32 iIndx = 0;

    if(!pcStr)
        return NULL;
    /* Scan for any occurrencies of the double quote character */
    pcTmp = WE_SCL_STRCHR(pcStr, (WE_INT32) WE_HDR_CH_DOUBLE_QUOTE);
    if(!pcTmp)
    {   /* double quote not found, we're done */
        if (TRUE == bMemMgr)
        {
            return Brw_StrDup(hMem,pcStr);
        }
        else
        {
            return WE_SCL_STRDUP(pcStr);
        }
    }

    /* calculate the size for the new string */
    while(*pcTmp != '\0')
    {
        /* Don't count the double quotes in the beginning or end, 
        or already escaped double quotes. */ 
        if((*pcTmp == WE_HDR_CH_DOUBLE_QUOTE) && (pcTmp != pcStr) &&
            (pcTmp != (pcStr + WE_SCL_STRLEN(pcStr) - 1)) && 
            (*(pcTmp - 1) != WE_HDR_CH_BACKSLASH))
        {
            iNumOfOccurrences ++;
        }
        pcTmp ++;
    }
      /* Check if the first and last characters are double quotes, 
      if so, and no other were found, we're done */
    if((*pcStr == WE_HDR_CH_DOUBLE_QUOTE) &&
        (*(pcStr + WE_SCL_STRLEN(pcStr) - 1) == WE_HDR_CH_DOUBLE_QUOTE))
    {
        if(!iNumOfOccurrences)
        {
            if (TRUE == bMemMgr)
            {
                return Brw_StrDup(hMem,pcStr);
            }
            else
            {
                return WE_SCL_STRDUP(pcStr);
            }
        }
    }
    iNewStrLen = WE_SCL_STRLEN(pcStr) + (iNumOfOccurrences * sizeof(WE_CHAR)) + 1;
    if (TRUE == bMemMgr)
    {
        pcResultStr = BrwMem_Alloc(hMem,iNewStrLen);        
    }
    else
    {
        pcResultStr = WE_MALLOC(iNewStrLen);
    }
    if(!pcResultStr)
        return NULL;
    pcTmp = (WE_CHAR *)pcStr;
    /* copy the string, insert escape characters where applicable */
    iIndx = 0;
    while(*pcTmp != '\0')
    {
        if((*pcTmp == WE_HDR_CH_DOUBLE_QUOTE) && (pcTmp != pcStr) && 
            (pcTmp != (pcStr + WE_SCL_STRLEN(pcStr) - 1)) && (*(pcTmp - 1) != WE_HDR_CH_BACKSLASH))
        {
            pcResultStr[iIndx] = WE_HDR_CH_BACKSLASH;
            iIndx++;
        }
        pcResultStr[iIndx] = *pcTmp;
        iIndx++;
        pcTmp++;
    }
    pcResultStr[iIndx] = '\0';

    return pcResultStr;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_ContainsReservedChars
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Utility function that scans str for reserved characters.
ARGUMENTS PASSED:
    
RETURN VALUE:
    TRUE if a reserved character was found, otherwise FALSE
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeHdr_ContainsReservedChars(const WE_CHAR *pcStr)
{
    WE_CHAR *pcTmp = (WE_CHAR *)pcStr;

    if(!pcTmp)
        return FALSE;
    while(*pcTmp != '\0')
    {
        if(ct_isctrl(*pcTmp) || ct_istspecial(*pcTmp))
            return TRUE;
        pcTmp++;
    }
    return FALSE;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_TrimParamStr
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Takes a string and trims consecutive blanks from its head and tail
ARGUMENTS PASSED:
    
RETURN VALUE:
    A trimmed string.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    The caller is responsible for for deallocating the returned memory buffer.
=====================================================================================*/
WE_CHAR * WeHdr_TrimParamStr(WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_CHAR *pcHead = (WE_CHAR *) pcStr;
    WE_CHAR *pcTail = NULL;
    WE_CHAR *pcTrimmedStr = NULL;
    WE_INT32 iHeaderBlanks = 0;
    WE_INT32 iTailBlanks = 0;
    WE_INT32 iTrimmedStrLen = 0;

    if(!pcStr)
        return NULL;
    pcTail = (WE_CHAR *)(pcStr + WE_SCL_STRLEN(pcStr) - 1);

    while((*pcHead == WE_HDR_CH_BLANK) && (pcHead != pcTail))
    {
        iHeaderBlanks ++;
        pcHead ++;
    }

    while((*pcTail == WE_HDR_CH_BLANK) && (pcHead != pcTail))
    {
        iTailBlanks ++;
        pcTail --;
    }

    iTrimmedStrLen = (pcTail + 1 - pcHead);
    if (TRUE == bMemMgr)
    {
        pcTrimmedStr = BrwMem_Alloc(hMem,iTrimmedStrLen + 1);
    }
    else
    {
        pcTrimmedStr = WE_MALLOC(iTrimmedStrLen + 1);
    }
    if(!pcTrimmedStr)
        return NULL;
    WE_STRNCPY(pcTrimmedStr, pcHead, iTrimmedStrLen);
    pcTrimmedStr[iTrimmedStrLen] = '\0';

    return pcTrimmedStr;
}

/*=====================================================================================
FUNCTION: 
    WeHdr_TrimUnquoteParamStr
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Takes a string and trims consecutive blanks from its head and tail. Moreover it removes 
    double quotes from its head and tail, and removes quoted pairs if any found.
ARGUMENTS PASSED:
    
RETURN VALUE:
    A trimmed string, non-double quoted.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    The caller is responsible for for deallocating the returned memory buffer.
=====================================================================================*/
WE_CHAR * WeHdr_TrimUnquoteParamStr(WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_CHAR *pcHead = (WE_CHAR *)pcStr;
    WE_CHAR *pcTail = NULL;
    WE_CHAR *pcTrimmedStr = NULL;
    WE_CHAR *pcTmp = NULL;
    WE_INT32 iHeadBlanks = 0;
    WE_INT32 iTailBlanks = 0;
    WE_INT32 iNumEscapes = 0;
    WE_INT32 iIndx = 0;
    WE_INT32 iTrimmedStrLen = 0;

    if(!pcStr)
        return NULL;
    pcTail = (WE_CHAR *)(pcStr + WE_SCL_STRLEN(pcStr) - 1);

    while((*pcHead == WE_HDR_CH_BLANK) && (pcHead != pcTail))
    {
        iHeadBlanks++;
        pcHead++;
    }
    while((*pcTail == WE_HDR_CH_BLANK) && (pcHead != pcTail))
    {
        iTailBlanks++;
        pcTail--;
    }
    if((*pcHead == WE_HDR_CH_DOUBLE_QUOTE) && (*pcTail == WE_HDR_CH_DOUBLE_QUOTE))
    {
        pcHead++;
        pcTail--;
        if(pcTail <= pcHead)
            return NULL;
       /* check if the string contains any escaped double quotes that should be reset */
       pcTmp = pcHead;
  while(pcTmp != (pcTail + 1))
  {
      if((*pcTmp == WE_HDR_CH_DOUBLE_QUOTE) && (pcTmp != pcHead) && 
               (*(pcTmp - 1) == WE_HDR_CH_BACKSLASH))
      {
          iNumEscapes++;
      }
           pcTmp++;
  }
    }
    iTrimmedStrLen = (pcTail + 1 -pcHead) - iNumEscapes;
    if (TRUE == bMemMgr)
    {  
        pcTrimmedStr = BrwMem_Alloc(hMem,iTrimmedStrLen + 1);
    }
    else
    {
        pcTrimmedStr = WE_MALLOC(iTrimmedStrLen + 1);
    }
    if(!pcTrimmedStr)
        return NULL;
    pcTmp = pcHead;
    while(pcTmp != (pcHead + 1))
    {
        if((*pcTmp == WE_HDR_CH_BACKSLASH) && (*(pcTmp + 1) == WE_HDR_CH_DOUBLE_QUOTE))
            pcTmp++;
        pcTrimmedStr[iIndx] = *pcTmp;
        iIndx ++;
        pcTmp ++;
    }
    pcTrimmedStr[iTrimmedStrLen] = '\0';
    return pcTrimmedStr;
}


