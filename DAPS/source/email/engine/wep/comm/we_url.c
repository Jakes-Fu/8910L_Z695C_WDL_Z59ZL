/*=====================================================================================
    FILE NAME   : we_url.c
    MODULE NAME : URL


    GENERAL DESCRIPTION
    This file define function operating memory.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2006-06-08       Guohong                        create we_url.c

=====================================================================================*/

/***************************************************************************************
*   Include File Section
****************************************************************************************/
/*Include System head file*/

/*Include Program Global head file*/
#include "we_cfg.h"
#include "we_def.h"
/*Include Module head file*/
#include "we_Mem.h"
#include "we_scl.h"
/*Include Private head file*/
#include "we_url.h"

#include "brw_def.h"
#include "cmmn_mem.h"
#include "tf3rd_for_HZ.h"

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/
#define WE_LAST_SCHEME 14
#define URL_CHARTOLOW(chr) ((chr>=0x41 && chr<=0x5A) ? (chr-32): chr)

/*Define Constant Macro end*/

/*Define Macro Function start*/
#define HASH_CHAR(n, c, d)  {n ^= (c << d); d = (d + 8) & 0x1f; }

#define IS_DOT(x) ((x >= pcBuf + 2) && (*(x - 1) == '.') && \
    (*(x - 2) == '\0'))

#define IS_DOTDOT(x) ((x >= pcBuf + 3) && (*(x - 1) == '.') && \
    (*(x - 2) == '.') && (*(x - 3) == '\0'))
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
typedef struct tagSt_WeUrlSchemes
{
    const E_WEURL_SCHEME  eScheme;
    const WE_CHAR      *pcStr;
    const WE_UINT8     ucLen;
}St_WeUrlSchemes,*P_St_WeUrlSchemes;

/*Define struct &  union type end*/

const E_WEURL_SCHEME aeUrlSchemeInfo[15]=
{
    E_WE_SCHEME_EMPTY,
    E_WE_SCHEME_HTTP,
    E_WE_SCHEME_HTTPS,
    E_WE_SCHEME_FILE,
    E_WE_SCHEME_WTAI,
    E_WE_SCHEME_ABOUT,
    E_WE_SCHEME_FUNCTION,
    E_WE_SCHEME_MAILTO,
    E_WE_SCHEME_MMSTO,
    E_WE_SCHEME_SMSTO,
    E_WE_SCHEME_TEL,
    E_WE_SCHEME_RTSP,
    E_WE_SCHEME_FS,
	E_WE_SCHEME_DATA,
    E_WE_SCHEME_UNKNOWN, 
};
const WE_CHAR acUrlSchemeInfo[15][9]=
{
    {""},
    { "http"},
    {"https"},
    {"file"},
    { "wtai"},
    { "about"},
    {"function"},
    {"mailto"},
    {"mmsto"},
    {"smsto"},
    {"tel"},
    {"rtsp"},
    {"fs"},
	{"data"},
    {"unknown"}
};
const WE_UINT8 aucUrlSchemeLen[15]=
{
    0,
    4,
    5,
    4,
    4,
    5,
    8,
    6,
    5,
    5,
    3,
    4,
    2,
	4,
    7
};


/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
WE_INT32 WeUrl_GetNum(const char *pcS,
                      WE_INT32 iLen);

WE_BOOL WeUrl_CompareStrings(const WE_CHAR *pcBs1,
                             WE_INT32 iLen1,
                             const WE_CHAR *pcBs2,
                             WE_INT32 iLen2,
                             WE_BOOL iNoCase);

E_WEURL_SCHEME WeUrl_Scheme2Type(const WE_CHAR *pcBs,
                              WE_INT32 iLen);

void WeUrl_Clear(St_WeUrl *pstUrl);

WE_BOOL WeUrl_IsNumber(const WE_CHAR *pcBs,
                       WE_INT32 iLen);

WE_BOOL WeUrl_ValueLimit(const WE_CHAR *pcBs,
                         WE_INT32 iLen,
                         WE_INT32 iLimit);

WE_BOOL WeUrl_CheckTopLabel(const WE_CHAR *pcBs,
                            WE_INT32 iLen);

WE_BOOL WeUrl_CheckDomainLabel(const WE_CHAR *pcBs,
                               WE_INT32 iLen);

WE_BOOL WeUrl_CheckHostName(const WE_CHAR *pcBs,
                            WE_INT32 iLen);

WE_BOOL WeUrl_CheckIpv4Address(const WE_CHAR *pcBs,
                               WE_INT32 iLen);

WE_BOOL WeUrl_CheckIpv6Address(const WE_CHAR *pcS,
                               WE_INT32 iLen);

WE_BOOL WeUrl_CheckScheme(St_WeUrl *pstUrl);

WE_BOOL WeUrl_CheckUserInfo(St_WeUrl *pstUrl);

WE_BOOL WeUrl_CheckHost(St_WeUrl *pstUrl);

WE_BOOL WeUrl_CheckPort(St_WeUrl *pstUrl);

WE_BOOL WeUrl_CheckAuthority(St_WeUrl *pstUrl);

WE_BOOL WeUrl_CheckFragment(St_WeUrl *pstUrl);


void WeUrl_HashPart(St_WeUrl *pstUrl,
                    WE_INT32 iWhichPart,
                    WE_UINT32 *puiN,
                    WE_INT32 *piD);


WE_BOOL WeUrl_EqualInternal(St_WeUrl *pstUrl1,
                            St_WeUrl *pstUrl2,
                            WE_INT32 iWhichComponents);


void WeUrl_UnescapeStringInPlace(WE_CHAR *pcDst,
                                 const WE_CHAR *pcSrc);

WE_BOOL WeUrl_DomainIsSuffix(const WE_CHAR *pcStr1,
                             WE_INT32 iLen1,
                             const WE_CHAR *pcStr2,
                             WE_INT32 iLen2);

WE_BOOL WeUrl_PathIsPrefix(const WE_CHAR *pcStr1,
                           WE_INT32 iLen1,
                           const WE_CHAR *pcStr2,
                           WE_INT32 iLen2);

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
The Interface Functions
=====================================================================================*/
/*=====================================================================================
FUNCTION: 
    WeUrl_GetSchemeStrInfo
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
const WE_CHAR * WeUrl_GetSchemeStrInfo(WE_INT32 iIndx)
{   
    
  return acUrlSchemeInfo[iIndx];
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetSchemeLenInfo
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
WE_UINT8 WeUrl_GetSchemeLenInfo(WE_INT32 iIndx)
{
  
    return aucUrlSchemeLen[iIndx];
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetSchemeStrInfo
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
const E_WEURL_SCHEME WeUrl_GetSchemeInfo(WE_INT32 iIndx)
{   
   
    return aeUrlSchemeInfo[iIndx];
}

/*=====================================================================================
FUNCTION: 
    WeUrl_Str2Scheme
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
E_WEURL_SCHEME WeUrl_Str2Scheme (const WE_CHAR *pcScheme)
{
    E_WEURL_SCHEME eSchemeType;
    eSchemeType = WeUrl_Scheme2Type(pcScheme, WE_SCL_STRLEN(pcScheme));
    return eSchemeType;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_Scheme2Str
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
const WE_CHAR * WeUrl_Scheme2Str (E_WEURL_SCHEME eScheme)
{
    WE_INT32 iSchemeIndx;

    if((((WE_INT32)eScheme) >= 0) && (eScheme < WE_LAST_SCHEME))
        iSchemeIndx = (WE_INT32)eScheme;
    else
        iSchemeIndx = WE_LAST_SCHEME;
    return WeUrl_GetSchemeStrInfo(iSchemeIndx);//s_WeUrl_stSchemes [iSchemeIndx].pcStr;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_Parse
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Take a string representation of a URL and parse it into its
 * components, and store these as fields in the given URL struct.
 * All components are stored in their original (possibly escaped) form.
 * Returns TRUE if the URL could be broken down into its
 * components, and FALSE otherwise.
 * The input parameter "bs" is a null-terminated character string.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_Parse (const WE_CHAR *pcBs,
                     St_WeUrl *pstUrl)
{
    WE_CHAR *pcStart = (WE_CHAR *)pcBs;
    WE_CHAR *pcTmp = NULL;
    WE_CHAR cTmpChar = 0;
    WE_INT32 iIndx = 0;
    WE_INT32 iLen = 0;

    WE_BOOL bJudge = FALSE;
    WE_BOOL bPathPart = FALSE;
    WE_BOOL bQueryPart = FALSE;
    WE_BOOL bFragmentPart = FALSE;
    WE_BOOL bNoAuth = FALSE;
    WE_BOOL bHostPort = FALSE;
    WE_BOOL bBeginPath = FALSE;
    WE_BOOL bBeginPort = FALSE;
    WE_BOOL bEnd = FALSE;
    WE_BOOL bAuth = FALSE;
    /* add variable bScheme for the instance of no "http://" */
 
    if((NULL == pcBs) || (NULL == pstUrl) || (NULL == pcStart))
        return FALSE;

    WeUrl_Clear(pstUrl);
    iLen = WE_SCL_STRLEN(pcBs);

    for(pcTmp = pcStart; *pcTmp;pcTmp++)
    {
        cTmpChar = *pcTmp;
        if((':' == cTmpChar) || ('/' == cTmpChar) || ('?' == cTmpChar) || ('#' == cTmpChar))
            break;
    }
    if(*pcTmp == 0)
    {
        bPathPart = TRUE;
        bJudge = TRUE;
    }
    if(bJudge== FALSE)
    {
        switch (cTmpChar)
        {
        case ':':
            pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART] = pcStart;
            pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART] = pcTmp - pcStart;
	     if(pcTmp[1] == ':')
	     {
	         pstUrl->eSchemeType = E_WE_SCHEME_UNKNOWN;
	     }
	     else
	     {
                pstUrl->eSchemeType = WeUrl_Scheme2Type(pcStart, pcTmp- pcStart);
	     }           
            pcStart = pcTmp + 1;
            break;
        case '?':
        case '#':
            pstUrl->apcPartsStr[G_WE_URL_PATH_PART] = pcStart;
            pstUrl->aiPartsLen[G_WE_URL_PATH_PART] = pcTmp - pcStart;
            pcStart = pcTmp;
            if(cTmpChar == '?')
            {
                bQueryPart = TRUE;
                bJudge = TRUE;
            }
            else
            {
                bFragmentPart = TRUE;
                bJudge = TRUE;
            }
	     break;	
		
        default:
            break;
        }
        
        if(bJudge == FALSE)
        {
            if((pcStart[0] != '/') || (pcStart[1] != '/'))
            {
                if(WeUrl_CompareStrings (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART],"mailto",6,TRUE) ||
                    WeUrl_CompareStrings (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART],"mmsto",5,TRUE)||
                    WeUrl_CompareStrings (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART],"smsto",5,TRUE) ||
                    WeUrl_CompareStrings (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART],"tel",3,TRUE))
                {
                    bAuth = TRUE;
                }
                else
                {
                    bNoAuth = TRUE;
                    bJudge = TRUE;
                }
            }
            if(bJudge == FALSE)
            {
                if(bAuth == FALSE)
                {
                    pcStart += 2;
                }
                for(pcTmp = pcStart; *pcTmp; pcTmp++)
                {
                    cTmpChar = *pcTmp;
                    if(('/' == cTmpChar) || ('?' == cTmpChar) || ('#' == cTmpChar))
                        break;
                }
                pstUrl->apcPartsStr[G_WE_URL_AUTHORITY_PART] = pcStart;
                iLen = pcTmp - pcStart;
                pstUrl->aiPartsLen[G_WE_URL_AUTHORITY_PART] = iLen;
                if('[' == pcStart[0])
                {
                    if(WeUrl_CompareStrings (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART],"tel",3,TRUE))
                        return FALSE;
                    else
                    {
                        bHostPort = TRUE;
                        bJudge = TRUE;
                    }
                }
                if(bJudge == FALSE)
                {
                    for(iIndx = 0;iIndx < iLen; iIndx++)
                    {
                        if(pcStart[iIndx] == '@')
                        {   
                            if(WeUrl_CompareStrings (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART],"tel",3,TRUE))
                                return FALSE;
                            pstUrl->apcPartsStr[G_WE_URL_USERINFO_PART] = pcStart;
                            pstUrl->aiPartsLen[G_WE_URL_USERINFO_PART] = iIndx;
                            pcStart += iIndx + 1;
                            iLen -= (iIndx + 1);
                            bHostPort = TRUE;
                            bJudge = TRUE;
                        }
                        else if(pcStart[iIndx] == ':')
                        {   
                            if(WeUrl_CompareStrings (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART],"tel",3,TRUE))
                                return FALSE;
                            pstUrl->apcPartsStr[G_WE_URL_HOST_PART] = pcStart;
                            pstUrl->aiPartsLen[G_WE_URL_HOST_PART] = iIndx;
                            pstUrl->apcPartsStr[G_WE_URL_PORT_PART] = &pcStart[iIndx + 1];
                            pstUrl->aiPartsLen[G_WE_URL_PORT_PART] = iLen - (iIndx + 1);
                            bBeginPath = TRUE;
                            bJudge = TRUE;
                        }
                    }
                    
                    if(bJudge == FALSE)
                    {
                        pstUrl->apcPartsStr[G_WE_URL_HOST_PART] = pcStart;
                        pstUrl->aiPartsLen[G_WE_URL_HOST_PART] = iLen;
                        if(WeUrl_CompareStrings (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART],"tel",3,TRUE))
                            return TRUE;
                        else
                        {
                            bBeginPath = TRUE;
                            bJudge = TRUE;
                        }
                    }
                }
            }
        }
    }


  
    if((bHostPort == TRUE) || (bJudge == FALSE))
    {
        pstUrl->apcPartsStr[G_WE_URL_HOST_PART] = pcStart;
        pstUrl->aiPartsLen[G_WE_URL_HOST_PART] = iLen;
        if(pcStart[0] == '[')
        {
            for(iIndx = 1;iIndx < iLen;iIndx++)
            {
                if(pcStart[iIndx] == ']')
                {
                    pstUrl->aiPartsLen[G_WE_URL_HOST_PART] = iIndx + 1;
                    iIndx++;
                    bBeginPort = TRUE;
                    break;
                }
            }
            if(bBeginPort == FALSE)
                return FALSE;
            else
               bJudge = FALSE;
        }
        else
        {
            for(iIndx = 0;iIndx < iLen;iIndx++)
            {
                if(pcStart[iIndx] == ':')
                {
                    pstUrl->aiPartsLen[G_WE_URL_HOST_PART] = iIndx;
                    bBeginPort = TRUE;
                    bJudge = TRUE;
                    break;
                }
            }
                bJudge = FALSE;
        }
    }

    if((bBeginPort == TRUE) || (bJudge == FALSE))
    {
        if((iIndx < iLen) && (pcStart[iIndx] == ':'))
        {
            pstUrl->apcPartsStr[G_WE_URL_PORT_PART] = &pcStart[iIndx + 1];
            pstUrl->aiPartsLen[G_WE_URL_PORT_PART] = iLen -( iIndx + 1);
        }
        bJudge = FALSE;
    }

    if((bBeginPath == TRUE) || (bJudge == FALSE))
    {
        if(*pcTmp == 0)
        {
            bEnd = TRUE;
      return TRUE;
            //bJudge = TRUE;
        }
        else
        {
            pcStart = pcTmp;
            bJudge = FALSE;
        }
    }

    if((bNoAuth == TRUE) || (bJudge == FALSE))
    {
        for(pcTmp = pcStart; *pcTmp;pcTmp++)
        {
            cTmpChar = *pcTmp;
            if(('?' == cTmpChar) || ('#' == cTmpChar))
                break;
        }
        bJudge = FALSE;
    }

    if((bPathPart == TRUE) || (bJudge == FALSE))
    {
        pstUrl->apcPartsStr[G_WE_URL_PATH_PART] = pcStart;
        pstUrl->aiPartsLen[G_WE_URL_PATH_PART] = pcTmp - pcStart;
        if(*pcTmp == 0)
        {
            bEnd = TRUE;
            return TRUE;
        }
        if(bEnd == FALSE)
        { 
            bJudge = FALSE;
            pcStart = pcTmp;
        }
    }

    if((bQueryPart == TRUE) || (bJudge == FALSE))
    {
        if(pcStart[0] != '?')
        {
            bFragmentPart = TRUE;
            bJudge = TRUE;
        }
        if(bFragmentPart == FALSE)
        {
            pcStart += 1;
            for(pcTmp = pcStart;*pcTmp;pcTmp++)
            {
                if(*pcTmp == '#')
                    break;
            }
            pstUrl->apcPartsStr[G_WE_URL_QUERY_PART] = pcStart;
            pstUrl->aiPartsLen[G_WE_URL_QUERY_PART] = pcTmp - pcStart;
            if(*pcTmp == 0)
            {
                bEnd = TRUE;
        return TRUE;
                //bJudge = TRUE;
            }
            if(bEnd == FALSE)
            {
                bJudge = FALSE;
                pcStart = pcTmp;
            }
        }
    }

    if((bFragmentPart == TRUE) || (bJudge == FALSE))
    {
        if(pcStart[0] != '#')
            return FALSE;
        pcStart += 1;
        for(pcTmp = pcStart; *pcTmp;pcTmp++);
        pstUrl->apcPartsStr[G_WE_URL_FRAGMENT_PART] = pcStart;
        pstUrl->aiPartsLen[G_WE_URL_FRAGMENT_PART] = pcTmp -pcStart;
    }
    return TRUE;
}


/*=====================================================================================
FUNCTION: 
    WeUrl_Hash
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
WE_BOOL WeUrl_Hash (const WE_CHAR *pcBs,
                    WE_UINT32 *puiHashValue)
{
    WE_INT32 iIndx = 0;
    WE_INT32 iLen = 0;
    WE_INT32 iPort = 0;
    WE_CHAR *pcTmpStr = NULL;
    WE_CHAR cTmpChar;
    St_WeUrl stUrl;
    WE_UINT32 uiN = 0;
    WE_INT32 iD = 0;

    if((NULL == pcBs) || !WeUrl_Parse(pcBs, &stUrl) || (NULL == puiHashValue))
        return FALSE;
    
    /* Hash the scheme */
    if(NULL != stUrl.apcPartsStr[G_WE_URL_SCHEME_PART])
    {
        pcTmpStr = stUrl.apcPartsStr[G_WE_URL_SCHEME_PART];
        for(iIndx = 0;iIndx < stUrl.aiPartsLen[G_WE_URL_SCHEME_PART];iIndx++)
        {    
        cTmpChar = URL_CHARTOLOW(pcTmpStr[iIndx]);
        HASH_CHAR (uiN,cTmpChar,iD);
        }
        HASH_CHAR(uiN, ':', iD);
    }
    if(NULL != stUrl.apcPartsStr[G_WE_URL_AUTHORITY_PART])
    {
        HASH_CHAR(uiN,'/',iD);
        HASH_CHAR(uiN,'/',iD);
        if(NULL != stUrl.apcPartsStr[G_WE_URL_USERINFO_PART])
        {
            pcTmpStr = stUrl.apcPartsStr[G_WE_URL_USERINFO_PART];
            for(iIndx = 0;iIndx < stUrl.aiPartsLen[G_WE_URL_USERINFO_PART]; iIndx++)
            {
                cTmpChar = URL_CHARTOLOW(pcTmpStr[iIndx]);
                HASH_CHAR(uiN, cTmpChar, iD);
            }
            HASH_CHAR(uiN, '@', iD);
        }
        if(NULL != stUrl.apcPartsStr[G_WE_URL_HOST_PART])
        {
            pcTmpStr = stUrl.apcPartsStr[G_WE_URL_HOST_PART];
            iLen = stUrl.aiPartsLen[G_WE_URL_HOST_PART];
            if((iLen > 0) && (pcTmpStr[iLen - 1] == '.'))
                iLen --;
            for(iIndx = 0; iIndx < iLen; iIndx++)
            {
                cTmpChar = URL_CHARTOLOW(pcTmpStr[iIndx]);
                HASH_CHAR(uiN, cTmpChar, iD);
            }
        }
        /* Get port number: skip default port numbers and empty port numbers, and 
        drop leading zeros in the port number. */
        pcTmpStr = stUrl.apcPartsStr[G_WE_URL_PORT_PART];
        iLen = stUrl.aiPartsLen[G_WE_URL_PORT_PART];
        if((NULL != pcTmpStr) && (iLen > 0))
        {
            iPort = WeUrl_GetNum(pcTmpStr, iLen);
            if(!((stUrl.eSchemeType == E_WE_SCHEME_HTTP) && (iPort == 80) ||
                   ((stUrl.eSchemeType == E_WE_SCHEME_HTTPS) && (iPort == 443))))
            {
                HASH_CHAR(uiN, ':', iD);
                if(iPort == 0)
                {
                    HASH_CHAR(uiN, '0', iD);
                }
                else
                {
                    while(*pcTmpStr == '0')
                    {
                        pcTmpStr++;
                        iLen --;
                    }
                    for(iIndx = 0; iIndx < iLen; iIndx ++)
                    {
                        HASH_CHAR(uiN, pcTmpStr[iIndx], iD);
                    }
                }
            }
        }
    }
    WeUrl_HashPart(&stUrl, G_WE_URL_PATH_PART, &uiN, &iD);

    if(NULL != stUrl.apcPartsStr[G_WE_URL_QUERY_PART])
    {
        HASH_CHAR(uiN, '?', iD);
        WeUrl_HashPart(&stUrl, G_WE_URL_QUERY_PART, &uiN, &iD);
    }

    if(NULL != stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART])
    {
        HASH_CHAR(uiN, '#', iD);
        WeUrl_HashPart(&stUrl, G_WE_URL_FRAGMENT_PART, &uiN, &iD);
    }

    *puiHashValue = uiN;
    return TRUE;
}


/*=====================================================================================
FUNCTION: 
    WeUrl_Url2Str
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
WE_CHAR * WeUrl_Url2StrEx (St_WeUrl *pstUrl,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_CHAR *pcStr = NULL;
    if(NULL == pstUrl)
        return NULL;
    else
    {
		if(TRUE == bMemMgr)
		{
			pcStr = WeUrl_AssembleEx(pstUrl,hMem,TRUE);
		}
		else
		{
			pcStr = WeUrl_AssembleEx(pstUrl,NULL,FALSE);
		}
        return pcStr;
    }
}

/*=====================================================================================
FUNCTION: 
    WeUrl_Resolve
CREATE DATE: 
    2007-1-29
AUTHOR: 
    Caiyi
DESCRIPTION:
	Return a copy of 'pcUrl' where each backslash has been replaced by a slash of the given url.
ARGUMENTS PASSED:
    pcUrl: The given string
RETURN VALUE:
    Returns NULL in case of error.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    It is the callers responsibility to deallocate the returned string.
=====================================================================================*/
WE_CHAR* WeUrl_ReplaceBKSlashEx(const WE_CHAR *pcUrl,WE_HANDLE hMem ,WE_BOOL bMemMgr)
{
    const WE_CHAR  *pcPtr = NULL;
	WE_CHAR  *pcNewUrl = NULL;
	WE_CHAR  *pcQ = NULL;
    
    if(NULL == pcUrl)
    {
        return NULL;
    }

	if(TRUE == bMemMgr)
	{
		
		pcNewUrl = BrwMem_Alloc(hMem,TF3RD_STRLEN(pcUrl) + 1);
	}
	else
	{
		pcNewUrl = WE_MALLOC(WE_SCL_STRLEN(pcUrl) + 1);
	}

	if(NULL == pcNewUrl)
    {
        return NULL;
    }

    for (pcPtr = pcUrl, pcQ = pcNewUrl; *pcPtr; pcPtr++) 
    {
        if ((*pcPtr == '?') || (*pcPtr == '#')) 
        {
            for(; *pcPtr; pcPtr++)
		{
		    *pcQ++ = *pcPtr;
		}
            break;
        }
        if (*pcPtr == '\\')
        {
            *pcQ++ = '/';
        }
        else
		*pcQ++ = *pcPtr;
    }
    *pcQ = '\0';

    return pcNewUrl;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_Resolve
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
WE_BOOL WeUrl_ResolveEx(const WE_CHAR *pcBase,
						const WE_CHAR *pcRel,
						WE_CHAR **ppcAbs,
						WE_HANDLE hMem, 
						WE_BOOL bMemMgr)

{
    St_WeUrl stBaseUrl;
    St_WeUrl stRelativeUrl;
    WE_BOOL bIsOk = TRUE;
	WE_CHAR *pcRelTmp = NULL;

    if((NULL == pcBase) || (NULL == pcRel) || (NULL == ppcAbs))
        return FALSE;
    
    if(TRUE == bMemMgr)
	{
		pcRelTmp = WeUrl_ReplaceBKSlashEx(pcRel,hMem,TRUE);
		WeUrl_Clear (&stRelativeUrl);
		if(!WeUrl_Parse(pcBase, &stBaseUrl) ||
			!WeUrl_Parse(pcRelTmp, &stRelativeUrl) || !WeUrl_ResolveInternalEx(&stBaseUrl, &stRelativeUrl, ppcAbs,hMem,TRUE))
		{
			if(*ppcAbs )
			{
				BrwMem_Free(hMem,*ppcAbs);
			}			
			*ppcAbs = NULL;
			bIsOk = FALSE;
		}
		BrwMem_Free(hMem,pcRelTmp);
	}
	else
	{
		pcRelTmp = WeUrl_ReplaceBKSlashEx(pcRel,NULL,FALSE);
		WeUrl_Clear (&stRelativeUrl);
		if(!WeUrl_Parse(pcBase, &stBaseUrl) ||
			!WeUrl_Parse(pcRelTmp, &stRelativeUrl) || !WeUrl_ResolveInternalEx(&stBaseUrl, &stRelativeUrl, ppcAbs,NULL,FALSE))
		{
			if(*ppcAbs )
			{
				WE_FREE(*ppcAbs);
			}			
			*ppcAbs = NULL;
			bIsOk = FALSE;
		}
		WE_FREE(pcRelTmp);
	}
	return bIsOk;
}


/*=====================================================================================
FUNCTION: 
    WeUrl_Equal
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
WE_BOOL WeUrl_Equal(const WE_CHAR *pcBs1,
                    const WE_CHAR *pcBs2,
                    WE_INT32 iWhichComponents)
{
    St_WeUrl stUrl1;
    St_WeUrl stUrl2;
    WE_BOOL bIsInternalEqual = FALSE;

    if(NULL == pcBs1)
        return (NULL == pcBs2);
    if(NULL == pcBs2)
        return FALSE;
    if(WeUrl_Parse(pcBs1, &stUrl1) && WeUrl_Parse(pcBs2,&stUrl2) && 
        WeUrl_EqualInternal(&stUrl1,&stUrl2,iWhichComponents))
    {
        bIsInternalEqual = TRUE;
    }
    return bIsInternalEqual;
}


/*=====================================================================================
FUNCTION: 
    WeUrl_IsValid
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
WE_BOOL WeUrl_IsValidEx (const WE_CHAR *pcUrl, WE_HANDLE hMem, WE_BOOL bMemMgr)
{
    St_WeUrl stUrl;
    WE_BOOL bIsValid = FALSE;

    if(TRUE == bMemMgr)
	{
		if(WeUrl_Parse(pcUrl, &stUrl) && 
			WeUrl_CheckScheme(&stUrl) &&
			WeUrl_CheckAuthority(&stUrl) &&
			WeUrl_CheckPathEx(&stUrl,hMem,TRUE) && 
			WeUrl_CheckQueryEx(&stUrl,hMem,TRUE) && 
			WeUrl_CheckFragment(&stUrl))
		{
			bIsValid = TRUE;
		}
	}
	else
	{
		if(WeUrl_Parse(pcUrl, &stUrl) && 
			WeUrl_CheckScheme(&stUrl) &&
			WeUrl_CheckAuthority(&stUrl) &&
			WeUrl_CheckPathEx(&stUrl,NULL,FALSE) && 
			WeUrl_CheckQueryEx(&stUrl,NULL,FALSE) && 
			WeUrl_CheckFragment(&stUrl))
		{
			bIsValid = TRUE;
		}
	}

    return bIsValid;
}


/*=====================================================================================
FUNCTION: 
    WeUrl_GetScheme
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
WE_CHAR * WeUrl_GetSchemeEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
    WE_CHAR *pcScheme = NULL;
	if(TRUE == bMemMgr)
	{
		pcScheme = WeUrl_GetPartEx(pcUrl, G_WE_URL_SCHEME_PART,hMem,TRUE);
	}
	else
	{
		pcScheme = WeUrl_GetPartEx(pcUrl, G_WE_URL_SCHEME_PART,NULL,FALSE);
	}
    return pcScheme;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetSchemeType
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
E_WEURL_SCHEME WeUrl_GetSchemeType (const WE_CHAR *pcUrl)
{
    St_WeUrl stUrl;

    if(NULL == pcUrl)
        return E_WE_SCHEME_EMPTY;
    if(!WeUrl_Parse(pcUrl, &stUrl) ||
        !WeUrl_CheckScheme(&stUrl))
    {
        return E_WE_SCHEME_UNKNOWN;
    }
    else
    {
        return stUrl.eSchemeType;
    }
}


/*=====================================================================================
FUNCTION: 
    WeUrl_GetHost
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
WE_CHAR * WeUrl_GetHostEx (const WE_CHAR *pcUrl,WE_HANDLE hMem ,  WE_BOOL bMemMgr)
{
    WE_CHAR *pcHostStr = NULL;
    if(TRUE == bMemMgr)
	{
		pcHostStr = WeUrl_GetPartEx(pcUrl, G_WE_URL_HOST_PART,hMem,TRUE);
	}
	else
	{
		pcHostStr = WeUrl_GetPartEx(pcUrl, G_WE_URL_HOST_PART,NULL,FALSE);
	}
    return pcHostStr;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetPort
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
WE_CHAR * WeUrl_GetPortEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
    WE_CHAR *pcPortStr = NULL;
    if(TRUE == bMemMgr)
	{
		pcPortStr = WeUrl_GetPartEx(pcUrl, G_WE_URL_PORT_PART,hMem,TRUE);
	}
	else
	{
		pcPortStr = WeUrl_GetPartEx(pcUrl, G_WE_URL_PORT_PART,NULL,FALSE);
    }
    return pcPortStr;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetHostPort
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
WE_CHAR * WeUrl_GetHostPortEx (const WE_CHAR *pcUrl,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
    St_WeUrl stUrl;
    WE_CHAR *pcPart = NULL;

    if((NULL != pcUrl) && WeUrl_Parse(pcUrl, &stUrl) &&
        (NULL != stUrl.apcPartsStr[G_WE_URL_HOST_PART]))
    {
        WE_INT32 iLen = stUrl.aiPartsLen[G_WE_URL_HOST_PART];
        if(NULL != stUrl.apcPartsStr[G_WE_URL_PORT_PART])
            iLen += stUrl.aiPartsLen[G_WE_URL_PORT_PART] + 1;
        if(TRUE == bMemMgr)
		{
			pcPart = BrwMem_Alloc(hMem,iLen + 1);
		}
        else
		{
			pcPart = WE_MALLOC(iLen + 1);
        }
        if(NULL == pcPart)
            return NULL;

        WE_STRNCPY(pcPart, stUrl.apcPartsStr[G_WE_URL_HOST_PART], iLen);
        pcPart[iLen] = '\0';

        if(NULL != stUrl.apcPartsStr[G_WE_URL_PORT_PART])
        {
            WE_CHAR *pcTmp = pcPart + stUrl.aiPartsLen[G_WE_URL_HOST_PART];
            *pcTmp ++ = ':';
            WE_STRNCPY(pcTmp, stUrl.apcPartsStr[G_WE_URL_PORT_PART], stUrl.aiPartsLen[G_WE_URL_PORT_PART]);
            pcTmp[stUrl.aiPartsLen[G_WE_URL_PORT_PART]] = '\0';
        }
    }
    return pcPart;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetPath
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
WE_CHAR * WeUrl_GetPathEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
    St_WeUrl stUrl;
    WE_CHAR *pcPath = NULL;

    if(NULL == pcUrl)
        return NULL;
    if(!WeUrl_Parse(pcUrl, &stUrl))
        return NULL;
    if(TRUE == bMemMgr)
	{
		pcPath = WeUrl_GetPathInternalEx(&stUrl,hMem,TRUE);
	}
	else
	{
		pcPath = WeUrl_GetPathInternalEx(&stUrl,NULL,FALSE);
	}
    return pcPath;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetParameters
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
WE_CHAR * WeUrl_GetParametersEx (const WE_CHAR *pcUrl,WE_HANDLE hMem ,WE_BOOL bMemMgr )
{
    St_WeUrl stUrl;
    WE_CHAR *pcParam = NULL;

    if((NULL != pcUrl) && 
        WeUrl_Parse(pcUrl, &stUrl))
    {
        if(TRUE == bMemMgr)
		{
			
			pcParam = WeUrl_GetParametersInternalEx(&stUrl,hMem,TRUE);
			
		}
		else
		{
			pcParam = WeUrl_GetParametersInternalEx(&stUrl,NULL,FALSE);
		}
    }
    return pcParam;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetQuery
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
WE_CHAR * WeUrl_GetQueryEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
    WE_CHAR *pcQuery = NULL;
	if(TRUE == bMemMgr)
	{
		pcQuery = WeUrl_GetPartEx(pcUrl, G_WE_URL_QUERY_PART,hMem,TRUE);
	}
	else
	{
		pcQuery = WeUrl_GetPartEx(pcUrl, G_WE_URL_QUERY_PART,NULL,FALSE);
	}
    return pcQuery;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetFragment
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
WE_CHAR * WeUrl_GetFragmentEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
    WE_CHAR *pcFragment = NULL;
	if(TRUE == bMemMgr)
	{
		pcFragment = WeUrl_GetPartEx(pcUrl, G_WE_URL_FRAGMENT_PART,hMem,TRUE);
	}
	else
	{
		pcFragment = WeUrl_GetPartEx(pcUrl, G_WE_URL_FRAGMENT_PART,NULL,FALSE);
	}
    return pcFragment;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_FindPath
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
const WE_CHAR * WeUrl_FindPath (const WE_CHAR *pcUrl)
{
    St_WeUrl stUrl;
    const WE_CHAR *pcPath = NULL;

    if((NULL != pcUrl) && 
        WeUrl_Parse(pcUrl,&stUrl))
    {
        pcPath = stUrl.apcPartsStr[G_WE_URL_PATH_PART];
    }
    return pcPath;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_DropFragment
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
WE_CHAR * WeUrl_DropFragmentEx (const WE_CHAR *pcUrl,
                              WE_CHAR **ppcFragment,
                              WE_HANDLE hMem,
							  WE_BOOL bMemMgr)
{
    St_WeUrl stUrl;
    WE_CHAR *pcNewUrl = NULL;

    if((NULL != pcUrl) && WeUrl_Parse(pcUrl, &stUrl))
    {
        if(NULL != ppcFragment)
        {
            if(NULL != stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART])
			{
				if(TRUE == bMemMgr)
				{
					*ppcFragment = Brw_StrDup(hMem,stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART]);
				}
				else
				{
					*ppcFragment = WE_SCL_STRDUP(stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART]);
                }
			}
            else
                *ppcFragment = NULL;
        }
        stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART] = NULL;
        stUrl.aiPartsLen[G_WE_URL_FRAGMENT_PART] = 0;
		if(TRUE == bMemMgr)
		{
			pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
		}
		else
		{
			pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
		}
    }
    return pcNewUrl;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_DropQuery
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
WE_CHAR * WeUrl_DropQueryEx (const WE_CHAR *pcUrl,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
    St_WeUrl stUrl;
    WE_CHAR *pcNewUrl = NULL;

    if((NULL != pcUrl) && WeUrl_Parse(pcUrl, &stUrl))
    {
        stUrl.apcPartsStr[G_WE_URL_QUERY_PART] = NULL;
        stUrl.aiPartsLen[G_WE_URL_QUERY_PART] = 0;
		if(TRUE == bMemMgr)
		{
			pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
		}
		else
		{
			pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
		}
    }
    return pcNewUrl;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_AddFragment
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
WE_CHAR * WeUrl_AddFragmentEx (const WE_CHAR *pcUrl,
                             const WE_CHAR *pcFragment,
                             WE_HANDLE hMem,
							 WE_BOOL bMemMgr)
{
    St_WeUrl stUrl;
    WE_CHAR *pcNewUrl = NULL;
    WE_INT32 iLen = 0;
    if((NULL != pcUrl) && WeUrl_Parse(pcUrl, &stUrl))
    {
        stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART] = NULL;
		stUrl.aiPartsLen[G_WE_URL_FRAGMENT_PART] = 0;	
        iLen = (NULL != pcFragment) ? WE_SCL_STRLEN(pcFragment) : 0;
        stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART] = (WE_CHAR *)pcFragment;
		stUrl.aiPartsLen[G_WE_URL_FRAGMENT_PART] = iLen;
		if(TRUE == bMemMgr)
		{
			pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
		}
		else
		{
			pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
		}
    }
    return pcNewUrl;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_EscapeString
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
WE_CHAR * WeUrl_EscapeStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    const WE_CHAR *pcP = NULL;
    WE_CHAR *pcQ = NULL;
    WE_CHAR *pcS = NULL;
    WE_INT32 iL = 0;
    WE_INT32 iR = 0;

    if(NULL == pcStr)
        return NULL;
    for(pcP = pcStr; *pcP != '\0'; pcP++)
    {
        if(ct_istspecial(*pcP) || (*pcP & 0x80))
            iR++;
        else
            iL++;

    }
    if(TRUE == bMemMgr)
	{
		pcS = BrwMem_Alloc(hMem,iL + 3*iR + 1);
	}
	else
	{
		pcS = WE_MALLOC(iL + 3*iR + 1);
	}

    if(NULL == pcS)
        return NULL;

    for(pcP = pcStr, pcQ = pcS; *pcP != '\0';pcP++)
    {
        if(ct_istspecial(*pcP) || (*pcP & 0x80))
        {
            *pcQ++ = '%';
            WE_SCL_BYTE2HEX(*pcP, pcQ);
            pcQ += 2; 
        }
        else
            *pcQ++ = *pcP;
    }
    *pcQ = '\0';
    return pcS;
}

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
WE_CHAR * WeUrl_EscapeUrlStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
    const WE_CHAR *pcP = NULL;
    WE_CHAR *pcQ = NULL;
    WE_CHAR *pcS = NULL;
    WE_INT32 iL = 0;
    WE_INT32 iR = 0;
	
    if(NULL == pcStr)
        return NULL;
    for(pcP = pcStr; *pcP != '\0'; pcP++)
    {
        if(ct_isunwise(*pcP) || ct_iswhitespace(*pcP))
            iR++;
        else
            iL++;
    }
    if(TRUE == bMemMgr)
    pcS = BrwMem_Alloc(hMem,iL + 3*iR + 1);
    else
    pcS = WE_MALLOC(iL + 3*iR + 1);
	
    if(NULL == pcS)
        return NULL;
	
    for(pcP = pcStr, pcQ = pcS; *pcP != '\0';pcP++)
    {
        if(ct_isunwise(*pcP) || ct_iswhitespace(*pcP))
        {
            *pcQ++ = '%';
            WE_SCL_BYTE2HEX(*pcP, pcQ);
            pcQ += 2; 
        }
        else
            *pcQ++ = *pcP;
    }
    *pcQ = '\0';
    return pcS;
}


/*=====================================================================================
FUNCTION: 
    WeUrl_EscapeNonAscii
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
WE_CHAR * WeUrl_EscapeNonAsciiEx (const WE_CHAR *psStr,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
    const WE_CHAR *pcP = NULL;
    WE_CHAR *pcQ = NULL;
    WE_CHAR *pcNewStr = NULL;
    WE_INT32 iL = 0;
    WE_INT32 iR = 0;

    if(NULL == psStr)
        return NULL;

    for(pcP = psStr; *pcP; pcP++)
    {
        if(*pcP & 0x80)
            iR++;
        else
            iL++;
    }
    if(TRUE == bMemMgr)
    pcNewStr = BrwMem_Alloc (hMem,iL + 3*iR + 1);
	else
    pcNewStr = WE_MALLOC (iL + 3*iR + 1);

    if(NULL == pcNewStr)
        return NULL;

    for(pcP = psStr, pcQ = pcNewStr;*pcP;pcP++)
    {
        if(*pcP & 0x80)
        {
            *pcQ++ = '%';
            WE_SCL_BYTE2HEX(*pcP,pcQ);
            pcQ += 2;
        }
        else
            *pcQ++ = *pcP;
    }
    *pcQ = '\0';
    return pcNewStr;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_WmlsEscapeString
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
WE_CHAR * WeUrl_WmlsEscapeStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    const WE_CHAR *pcP = NULL;
    WE_CHAR *pcQ = NULL;
    WE_CHAR *pcS = NULL;
    WE_INT32 iN = 0;
    WE_UCHAR ucB;

    if(pcStr == NULL)
        return NULL;
    /* We should not allow characters > 0xff. Now, in UTF-8, the characters
    in the range 0x80-0xff are represented by two bytes each:
    from <0xc2,0x80> to <0xc3,0xbf>. Hence, the condition becomes
    to test that no byte has a value > 0xc3. */
    for(pcP = pcStr; *pcP; pcP++)
    {
        if(((WE_UCHAR)*pcP) > 0xc3)
            return NULL;
        if(ct_isspecial(*pcP))
        {
            iN += 3;
        }
        else if(*pcP & 0x80)
        {
            iN += 3;
            pcP++;
        }
        else
        {
           iN++;
        }
    }
    if(TRUE == bMemMgr)
    pcS = BrwMem_Alloc(hMem,iN + 1);
	else
    pcS = WE_MALLOC(iN + 1);

    if(NULL == pcS)
        return NULL;

    for(pcP = pcStr,pcQ = pcS; *pcP; pcP++)
    {
        if(ct_isspecial(*pcP))
        {
            *pcQ++ = '%';
            WE_SCL_BYTE2HEX(*pcP, pcQ);
            pcQ += 2;
        }
        else if(*pcP & 0x80)
        {
            ucB = (WE_UCHAR) ((*pcP++ << 6) & 0xff);
            ucB |= (WE_UCHAR)(*pcP &0x3f);
            *pcQ++ = '%';
            WE_SCL_BYTE2HEX(ucB,pcQ);
            pcQ += 2;
        }
        else
            *pcQ++ = *pcP;
    }
    *pcQ = '\0';
    return pcS;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_UnescapeString
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
WE_CHAR *WeUrl_UnescapeStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_CHAR *pcTmp = NULL;
    WE_CHAR *pcDest = NULL;
    const WE_CHAR *pcSrc = NULL;
    WE_UCHAR ucB;

    if(NULL != pcStr)
    {
        if(TRUE == bMemMgr)
        pcTmp = BrwMem_Alloc(hMem,TF3RD_STRLEN(pcStr) + 1);
        else
        pcTmp = WE_MALLOC(WE_SCL_STRLEN(pcStr) + 1);

        if(NULL == pcTmp)
            return NULL;
        pcSrc = pcStr;
        pcDest = pcTmp;
        while(*pcSrc)
        {
            if(!ct_isascii(*pcSrc))
            {
                if(TRUE == bMemMgr)
                {
                    BrwMem_Free(hMem,pcTmp);
                }
                else
                {
                    WE_FREE(pcTmp);
                }
                return NULL;
            }
            if((*pcSrc == '%') && WE_SCL_HEX2BYTE(pcSrc + 1, &ucB))
            {
                if(ucB & 0x80)
                {
                    *pcDest++ = (WE_CHAR) (0xc0 | (ucB >> 6));
                    *pcDest++ = (WE_CHAR) (0x80 | (ucB & 0x3f));
                }
                else
                {
                    *pcDest++ = (WE_CHAR)ucB;
                }
                pcSrc += 3;
            }
            else
                *pcDest++ = *pcSrc++;
        }
        *pcDest = '\0';
    }

    return pcTmp;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_WmlsUnescapeString
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
WE_CHAR * WeUrl_WmlsUnescapeStringEx (const WE_CHAR *pcStr,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_CHAR *pcTmp = NULL;
    WE_CHAR *pcDest = NULL;
    const WE_CHAR *pcSrc = NULL;
    WE_UCHAR ucB;

    if(NULL != pcStr)
    {
        if(TRUE == bMemMgr)
        pcTmp = BrwMem_Alloc(hMem,TF3RD_STRLEN(pcStr) + 1);
        else
        pcTmp = WE_MALLOC(WE_SCL_STRLEN(pcStr) + 1);

        if(NULL == pcTmp)
            return NULL;
        pcSrc = pcStr;
        pcDest = pcTmp;
        while(*pcSrc)
        {
            if(!ct_isascii(*pcSrc))
            {
				if(TRUE == bMemMgr)
				{
					BrwMem_Free(hMem,pcTmp);					
				}
				else
				{
					WE_FREE(pcTmp);
				}

                return NULL;
            }
            if((*pcSrc == '%') && WE_SCL_HEX2BYTE(pcSrc + 1, &ucB))
            {
                if(ucB & 0x80)
                {
                    *pcDest++ = (WE_CHAR) (0xc0 | (ucB >> 6));
                    *pcDest++ = (WE_CHAR) (0x80 | (ucB & 0x3f));
                }
                else
                {
                    *pcDest++ = (WE_CHAR)ucB;
                }
                pcSrc += 3;
            }
            else
                *pcDest++ = *pcSrc++;
        }
        *pcDest = '\0';
    }
    return pcTmp;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_EscapeBlanks
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
WE_CHAR * WeUrl_EscapeBlanksEx (const WE_CHAR *pcStr,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
    const WE_CHAR *pcP = NULL;
    WE_CHAR *pcQ = NULL;
    WE_CHAR *pcS = NULL;
    WE_INT32 iL = 0;
    WE_INT32 iR = 0;

    if(pcStr == NULL)
        return NULL;

    for(pcP = pcStr; *pcP; pcP++)
    {
        if(ct_isblank(*pcP))
            iR++;
        else
            iL++;
    }
    if(TRUE == bMemMgr)
    pcS = BrwMem_Alloc(hMem,iL + 3*iR + 1);
    else
    pcS = WE_MALLOC(iL + 3*iR + 1);

    if(NULL == pcS)
        return NULL;

    for(pcP = pcStr,pcQ = pcS; *pcP; pcP++)
    {
        if(ct_isblank(*pcP))
        {
            *pcQ++ = '%';
            WE_SCL_BYTE2HEX(*pcP, pcQ);
            pcQ += 2;
        }
        else
            *pcQ++ = *pcP;
    }
    *pcQ = '\0';
    return pcS;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckAccess
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
WE_BOOL WeUrl_CheckAccess (WE_CHAR *pcAbsUrl,
                                              WE_CHAR *pcDomain,
                                              WE_CHAR *pcPath)
{
    St_WeUrl stUrl;
    WE_BOOL bIsAccess = FALSE;

    if((NULL == pcAbsUrl) || (NULL == pcDomain))
        return FALSE;
    if(NULL == pcPath)
        pcPath = "/";
    if(!WeUrl_Parse(pcAbsUrl, &stUrl))
        return FALSE;
        
    if((NULL != stUrl.apcPartsStr[G_WE_URL_HOST_PART]) &&
       (stUrl.aiPartsLen[G_WE_URL_PATH_PART] > 0) &&
       WeUrl_DomainIsSuffix(pcDomain, WE_SCL_STRLEN(pcDomain),stUrl.apcPartsStr[G_WE_URL_HOST_PART],
                                         stUrl.aiPartsLen[G_WE_URL_HOST_PART]) &&
        WeUrl_PathIsPrefix(pcPath, WE_SCL_STRLEN(pcPath), stUrl.apcPartsStr[G_WE_URL_PATH_PART],
                                     stUrl.aiPartsLen[G_WE_URL_PATH_PART]))
    {
        bIsAccess = TRUE;
    }

    return bIsAccess;
}


/*=====================================================================================
FUNCTION: 
    WeUrl_MakeComplete
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
WE_CHAR * WeUrl_MakeCompleteEx (WE_CHAR *pcBs,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    St_WeUrl stUrl;
    St_WeUrl stNewUrl;
    WE_CHAR *pcP = NULL;
    WE_CHAR *pcNs = NULL;

    if((NULL == pcBs) || !WeUrl_Parse(pcBs, &stUrl))
    {
        return NULL;
    }
    stNewUrl = stUrl;
    /* The original URL has a scheme different from http(s). */
    if((NULL != stUrl.apcPartsStr[G_WE_URL_SCHEME_PART]) && 
        (E_WE_SCHEME_HTTP != stUrl.eSchemeType) && 
        (E_WE_SCHEME_HTTPS != stUrl.eSchemeType))
    {
        /* It might be that what we think is the scheme is really the host followed by a 
        port number. Consider for example this case:  www.abc.com:8080/some/path
        We use the following heuristic: if the scheme part contains one or more '.'-characters,
        and if the part immediately following the scheme is an integer, then we assume
        that what we really have is a host and port.*/
        WE_CHAR *pcS = WE_SCL_STRNCHR(stUrl.apcPartsStr[G_WE_URL_PATH_PART],'/',
                                                                 stUrl.aiPartsLen[G_WE_URL_PATH_PART]);
        WE_INT32 iPlen;
   if(NULL != pcS)
            iPlen = pcS - stUrl.apcPartsStr[G_WE_URL_PATH_PART];
        else
            iPlen = stUrl.aiPartsLen[G_WE_URL_PATH_PART];
        if((WE_SCL_STRNCHR(stUrl.apcPartsStr[G_WE_URL_SCHEME_PART], '.',
                                          stUrl.aiPartsLen[G_WE_URL_SCHEME_PART]) != NULL) &&
            (NULL != stUrl.apcPartsStr[G_WE_URL_PATH_PART]) && (iPlen > 0) &&
            WeUrl_IsNumber(stUrl.apcPartsStr[G_WE_URL_PATH_PART],iPlen))
        {
            stNewUrl.apcPartsStr[G_WE_URL_SCHEME_PART] = "http";
            stNewUrl.aiPartsLen[G_WE_URL_SCHEME_PART] = 4;
            stNewUrl.eSchemeType = E_WE_SCHEME_HTTP;

            stNewUrl.apcPartsStr[G_WE_URL_AUTHORITY_PART] = stNewUrl.apcPartsStr[G_WE_URL_HOST_PART] = pcBs;
       stNewUrl.aiPartsLen[G_WE_URL_AUTHORITY_PART] = stNewUrl.aiPartsLen[G_WE_URL_HOST_PART] = 
                               stUrl.aiPartsLen[G_WE_URL_SCHEME_PART];
            stNewUrl.apcPartsStr[G_WE_URL_PORT_PART] = stUrl.apcPartsStr[G_WE_URL_PATH_PART];
            stNewUrl.aiPartsLen[G_WE_URL_PORT_PART] = iPlen;

            stNewUrl.apcPartsStr[G_WE_URL_PATH_PART] = pcS;
            stNewUrl.aiPartsLen[G_WE_URL_PATH_PART] = stUrl.aiPartsLen[G_WE_URL_PATH_PART] - iPlen;
        }
        else
        {
            if(TRUE == bMemMgr)
            return Brw_StrDup(hMem,pcBs);
            else
            return WE_SCL_STRDUP(pcBs);
            /* We have to assume that it is an absolute URL with an unknown scheme */
        }
    }
    /* The original URL does not have a scheme; we attach 'http:' */
    if(NULL == stUrl.apcPartsStr[G_WE_URL_SCHEME_PART])
    {
        stNewUrl.apcPartsStr[G_WE_URL_SCHEME_PART] = "http";
        stNewUrl.aiPartsLen[G_WE_URL_SCHEME_PART] = 4;
        stNewUrl.eSchemeType = E_WE_SCHEME_HTTP;
    }
    /* The original URL does not have an authority part (starting with '//'); we promote 
    the initial segment of the path, up to (but not including) the first '/'-character, or the 
    whole path in case it has no such delimiter. If the path is an absolute path (starting 
    with '/'), we cannot guess what the host should have been. */
    if(NULL == stNewUrl.apcPartsStr[G_WE_URL_AUTHORITY_PART])
    {
        if(stUrl.aiPartsLen[G_WE_URL_PATH_PART] == 0)
            return NULL;
        pcP = WE_SCL_STRNCHR(stUrl.apcPartsStr[G_WE_URL_PATH_PART], '/',stUrl.aiPartsLen[G_WE_URL_PATH_PART]);
        if(NULL == pcP)
        {
            /* There is no '/' character: make the whole string be the host part. */
            stNewUrl.apcPartsStr[G_WE_URL_AUTHORITY_PART] = stNewUrl.apcPartsStr[G_WE_URL_HOST_PART] = 
                                  stUrl.apcPartsStr[G_WE_URL_PATH_PART];
            stNewUrl.aiPartsLen[G_WE_URL_AUTHORITY_PART] = stNewUrl.aiPartsLen[G_WE_URL_HOST_PART] = 
                                  stUrl.aiPartsLen[G_WE_URL_PATH_PART];
            stNewUrl.aiPartsLen[G_WE_URL_PATH_PART] = 0;
        }
        else if(pcP != stUrl.apcPartsStr[G_WE_URL_PATH_PART])
        {
            /* String does not start with '/': let prefix be host part. */
            stNewUrl.apcPartsStr[G_WE_URL_AUTHORITY_PART] = stNewUrl.apcPartsStr[G_WE_URL_HOST_PART] = 
                                   stUrl.apcPartsStr[G_WE_URL_PATH_PART];
            stNewUrl.aiPartsLen[G_WE_URL_AUTHORITY_PART] = stNewUrl.aiPartsLen[G_WE_URL_HOST_PART] = 
                                    (pcP - stUrl.apcPartsStr[G_WE_URL_PATH_PART]);
            stNewUrl.apcPartsStr[G_WE_URL_PATH_PART] = pcP;
            stNewUrl.aiPartsLen[G_WE_URL_PATH_PART] = stUrl.aiPartsLen[G_WE_URL_PATH_PART] - 
                                    stNewUrl.aiPartsLen[G_WE_URL_HOST_PART];
        }
        else
            return NULL;
            /* The URL is simply an absolute path, we cannot deduce the host! */
    }
    if(0 == stNewUrl.aiPartsLen[G_WE_URL_PATH_PART])
    {
         /* The URL (original or modified as above), which is now a http(s) URL, has no path part;
         we attach '/', the root path. */
         stNewUrl.apcPartsStr[G_WE_URL_PATH_PART] = "/";
         stNewUrl.aiPartsLen[G_WE_URL_PATH_PART] = 1;
    }
    /* Allocate a new buffer and copy all the parts to it. */
	if(TRUE == bMemMgr)
    pcNs = WeUrl_AssembleEx(&stNewUrl,hMem,TRUE);
    else
    pcNs = WeUrl_AssembleEx(&stNewUrl,NULL,FALSE);

    return pcNs;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_MinRelativePath
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
WE_CHAR * WeUrl_MinRelativePathEx (WE_CHAR *pcBs1,
                                 WE_CHAR *pcBs2,
                                 WE_BOOL bIncludFragment,
                                 WE_HANDLE hMem,
								 WE_BOOL bMemMgr)
{
    St_WeUrl stUrl1;
    St_WeUrl stUrl2;
    const WE_CHAR *pcS1;
    const WE_CHAR *pcS2;
    WE_CHAR *pcBuf = NULL;
    WE_CHAR *pcBufTmp = NULL;
    WE_CHAR *pcRes = NULL;
    WE_INT32 iLen = 0;
    WE_INT32 iLen1 = 0;
    WE_INT32 iLen2 = 0;
    WE_INT32 iL1 = 0;
    WE_INT32 iL2 = 0;
    WE_INT32 iNum = 0;
    WE_INT32 iIndx = 0;
    WE_INT32 iL1Indx = 0;
    WE_INT32 iL2Indx = 0;
    WE_INT32 iK1 = 0;
    WE_INT32 iK2 = 0;

    if(!WeUrl_Parse(pcBs1,&stUrl1) ||
        !WeUrl_Parse(pcBs2,&stUrl2))
    {
        return NULL;
    }
    if(!WeUrl_EqualInternal(&stUrl1,&stUrl2,
                                  G_WE_URL_SCHEME_COMP | G_WE_URL_HOST_COMP |
                                  G_WE_URL_PORT_COMP))
    {
        if(TRUE == bMemMgr)
        return WeUrl_Url2StrEx(&stUrl2,hMem,TRUE);
        else        
        return WeUrl_Url2StrEx(&stUrl2,NULL,FALSE);
    }
    pcS1 = stUrl1.apcPartsStr[G_WE_URL_PATH_PART];
    pcS2 = stUrl2.apcPartsStr[G_WE_URL_PATH_PART];
    if((NULL == pcS1) || (NULL == pcS2))
        return NULL;
    iLen1 = stUrl1.aiPartsLen[G_WE_URL_PATH_PART];
    iLen2 = stUrl2.aiPartsLen[G_WE_URL_PATH_PART];
    if((0 == iLen1) || (0 == iLen2))
        return NULL;
    if((pcS1[0] != '/') || (pcS2[0] != '/'))
        return NULL;
    /* Drop everything after the last '/' in path 1. */
    for(iL1Indx = iLen1 - 1; iL1Indx > 0;iL1Indx --)
    {
        if(pcS1[iL1Indx] == '/')
            break;
    }
    iLen1 = iL1Indx + 1;
    /* Now, path 1 ends with a '/'. Search past the initial, equal, path segments in the two
    paths. A path segment starts right after a '/' and ends on the next '/'. */
    for(iK1 = 1, iK2 = 1;(iK1 < iLen1) && (iK2 < iLen2);)
    {
        /* Here, iK1 is the index following the last seen '/', and iL1Indx is moved  up to the next '/', 
        or the end of the string. */
        for(iL1Indx = iK1; (iL1Indx < iLen1) && (pcS1[iL1Indx] != '/');iL1Indx++);
        for(iL2Indx = iK2; (iL2Indx < iLen2) && (pcS2[iL2Indx] != '/');iL2Indx++);
        iL1 = iL1Indx - iK1;
        iL2 = iL2Indx - iK2;
        if(!((iL1 == iL2) && WeUrl_CompareStrings(&pcS1[iK1],iL1,&pcS2[iK2],iL2,FALSE)))
        {
            break;
        }
        iK1 = iL1Indx + 1;
        iK2 = iL2Indx + 1;
    }
    /* Count how many more '/' there are in path 1. */
    iNum = 0;
    for(iL1Indx = iK1;iL1Indx < iLen1; iL1Indx++)
    {
        if(pcS1[iL1Indx] == '/')
            iNum++;
    } 
    /* The path we create will consist of what ever part was left in path 2 (i.e., that did not match 
    a corresponding part in path 1), plus n "../" segments. Note, that we might end up with a string
    of length 0 here. Special case: if the paths differ already in the first segment, then just use the 
    absolute path from url2. */
	if(iK1 == 1 || iK2 >= iLen2)  
	{                              
        iLen = iLen2;                                               
        if(TRUE == bMemMgr)                                     
		pcBufTmp = pcBuf = BrwMem_Alloc(hMem,iLen + 1);   
        else                                                       
		pcBufTmp = pcBuf = WE_MALLOC(iLen + 1);            
                                                     
		if(NULL == pcBuf)                                         
			return NULL;                                          
		WE_MEMCPY (pcBufTmp, pcS2, iLen2);                    
		pcBufTmp += iLen2;                                  
	}                                                     
	else                                                         
	{                                                            
		iLen = iNum * 3 + iLen2 - iK2;                               
		if(TRUE == bMemMgr)                                       
		pcBufTmp = pcBuf = BrwMem_Alloc(hMem,iLen + 1);   
		else                                                       
		pcBufTmp = pcBuf = WE_MALLOC(iLen + 1);            
                                                     
		if(NULL == pcBuf)                                         
			return NULL;                                          
		for(iIndx = 0;iIndx < iNum;iIndx++)                           
		{                                                             
			WE_MEMCPY(pcBufTmp,"../",3);                              
			pcBufTmp += 3;                                            
		}                                                             
		if(iLen2 > iK2)                                               
		{                                                             
			WE_MEMCPY(pcBufTmp, &pcS2[iK2],iLen2 - iK2);              
			pcBufTmp += iLen2 - iK2;                                  
		}                                                             
	}                                                             
	*pcBufTmp = '\0';                                             
	
    
    stUrl2.apcPartsStr[G_WE_URL_PATH_PART] = pcBuf;
    stUrl2.aiPartsLen[G_WE_URL_PATH_PART] = iLen;
    stUrl2.apcPartsStr[G_WE_URL_AUTHORITY_PART] = NULL;
    stUrl2.apcPartsStr[G_WE_URL_SCHEME_PART] = NULL;
    stUrl2.apcPartsStr[G_WE_URL_HOST_PART] = NULL;
    stUrl2.apcPartsStr[G_WE_URL_PORT_PART] = NULL;

    stUrl2.aiPartsLen[G_WE_URL_AUTHORITY_PART] = 0;
    stUrl2.aiPartsLen[G_WE_URL_SCHEME_PART] = 0;
    stUrl2.aiPartsLen[G_WE_URL_HOST_PART] = 0;
    stUrl2.aiPartsLen[G_WE_URL_PORT_PART] = 0;

    stUrl2.eSchemeType = E_WE_SCHEME_EMPTY;

    if(!bIncludFragment)
    {
        stUrl2.apcPartsStr[G_WE_URL_FRAGMENT_PART] = NULL;
        stUrl2.aiPartsLen[G_WE_URL_FRAGMENT_PART] = 0;
    }
    if(TRUE == bMemMgr)
    {
        pcRes = WeUrl_AssembleEx(&stUrl2,hMem,TRUE);
        BrwMem_Free(hMem,pcBuf);
    }
    else
    {
        pcRes = WeUrl_AssembleEx(&stUrl2,NULL,FALSE);
        WE_FREE(pcBuf);
    }

    return pcRes;
}


/*=====================================================================================
FUNCTION: 
    WeUrl_IsComplete
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
WE_BOOL WeUrl_IsComplete (const WE_CHAR *pcUrl)
{
    St_WeUrl stUrl;
    WE_BOOL bIsComplete = FALSE;

    if(WeUrl_Parse(pcUrl, &stUrl) && 
        (NULL != stUrl.apcPartsStr[G_WE_URL_SCHEME_PART]) &&
        (NULL != stUrl.apcPartsStr[G_WE_URL_HOST_PART]) &&
        (stUrl.aiPartsLen[G_WE_URL_PATH_PART] > 0))
    {
        bIsComplete = TRUE;
    }

    return bIsComplete;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_AppendToQuery
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
WE_CHAR * WeUrl_AppendToQueryEx (WE_CHAR *pcOldUrl,
                               WE_CHAR *pcNewQuery,
                               WE_HANDLE hMem,
							   WE_BOOL bMemMgr)
{
    St_WeUrl stUrl;
    WE_INT32 iLen = 0;
    WE_CHAR *pcNewUrl = NULL;
    WE_CHAR *pcTmp = NULL;

    if(NULL == pcOldUrl)
        return NULL;
    if(pcNewQuery != NULL)
    {
        iLen = WE_SCL_STRLEN(pcNewQuery);
    }    
    if((NULL == pcNewQuery)||(iLen == 0))
    {
		if(TRUE == bMemMgr)
        return Brw_StrDup(hMem,pcOldUrl);
		else
        return WE_SCL_STRDUP(pcOldUrl);
    }
    if(!WeUrl_Parse(pcOldUrl, &stUrl))
        return NULL;

    if(NULL == stUrl.apcPartsStr[G_WE_URL_QUERY_PART])
    {
        stUrl.apcPartsStr[G_WE_URL_QUERY_PART] = pcNewQuery;
        stUrl.aiPartsLen[G_WE_URL_QUERY_PART] = iLen;
    }
    else
    {
        if(TRUE == bMemMgr)
        pcTmp = BrwMem_Alloc(hMem,stUrl.aiPartsLen[G_WE_URL_QUERY_PART] + iLen + 2);
		else
        pcTmp = WE_MALLOC(stUrl.aiPartsLen[G_WE_URL_QUERY_PART] + iLen + 2);

        if(NULL == pcTmp)
            return NULL;

        WE_MEMCPY(pcTmp, stUrl.apcPartsStr[G_WE_URL_QUERY_PART], stUrl.aiPartsLen[G_WE_URL_QUERY_PART]);
        pcTmp[stUrl.aiPartsLen[G_WE_URL_QUERY_PART]] = '&';
        WE_MEMCPY(pcTmp + stUrl.aiPartsLen[G_WE_URL_QUERY_PART] + 1,pcNewQuery,iLen);
        stUrl.apcPartsStr[G_WE_URL_QUERY_PART] = pcTmp;
        stUrl.aiPartsLen[G_WE_URL_QUERY_PART] += iLen + 1;
    }
	if(TRUE == bMemMgr)
    pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
    else
    pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);

    if(NULL != pcTmp)
	{
		if(TRUE == bMemMgr)
		{
			BrwMem_Free(hMem,pcTmp);			
		}
		else
		{
			WE_FREE(pcTmp);
		}
    }
    return pcNewUrl;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_IsPrefix
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
WE_BOOL WeUrl_IsPrefix (WE_CHAR *pcPath1,
                                       WE_CHAR *pcPath2)
{
    St_WeUrl stUrl1;
    St_WeUrl stUrl2;

    if(!WeUrl_Parse(pcPath1, &stUrl1) || 
        !WeUrl_Parse(pcPath2, &stUrl2))
    {
        return FALSE;
    }
    if(!WeUrl_EqualInternal(&stUrl1,&stUrl2,G_WE_URL_SCHEME_COMP))
        return FALSE;

    if((NULL == stUrl1.apcPartsStr[G_WE_URL_AUTHORITY_PART]) && 
        (NULL == stUrl2.apcPartsStr[G_WE_URL_AUTHORITY_PART]) &&
        (stUrl1.aiPartsLen[G_WE_URL_PATH_PART] > 0) && 
        (stUrl2.aiPartsLen[G_WE_URL_PATH_PART] > 0) &&
        (stUrl1.apcPartsStr[G_WE_URL_PATH_PART][0] != '/') &&
        (stUrl2.apcPartsStr[G_WE_URL_PATH_PART][0] != '/'))
    {
        return WeUrl_EqualInternal(&stUrl1,&stUrl2,G_WE_URL_PATH_COMP);
        /* Both URLs are of the "opaque" type. */
    }
    /* For non-opaque URLs, the authority parts must be equal. */
    if(!WeUrl_EqualInternal(&stUrl1,&stUrl2, G_WE_URL_AUTHORITY_COMP))
        return FALSE;
    /* An empty path is the prefix on any other path. */
    if(stUrl1.aiPartsLen[G_WE_URL_PATH_PART] == 0)
        return TRUE;
    /* Check that path1 is a segment-wise prefix of path2. */
    return WeUrl_PathIsPrefix(stUrl1.apcPartsStr[G_WE_URL_PATH_PART],stUrl1.aiPartsLen[G_WE_URL_PATH_PART],
                                            stUrl2.apcPartsStr[G_WE_URL_PATH_PART],stUrl2.aiPartsLen[G_WE_URL_PATH_PART]);
}
/*=====================================================================================
The Internal Functions
=====================================================================================*/
/*=====================================================================================
FUNCTION: 
    WeUrl_GetNum
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:

 * Return the value of the unsigned decimal integer stored
 * in the first "len" bytes of the string "s".
 * Used to read port numbers.
 *
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_INT32 WeUrl_GetNum(const char *pcS,
                      WE_INT32 iLen)
{
    WE_INT32 iNum = 0;
    WE_INT32 iIndx = 0;

    if(NULL == pcS)
        return iNum;	

    for(iIndx = 0; iIndx < iLen;iIndx++)
        iNum = iNum*10 + (pcS[iIndx] - '0');
    return iNum;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CompareStrings
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    String comparison routine that returns TRUE if the strings are equal.
 * If 'noCase' is TRUE, the case of letters is insignificant.
 * Hex escape sequences of the form '%XY' are handled correctly,
 * that is, such a sequence is compared as if the character had first
 * been unescaped, unless it is a character in the "reserved" set.
 * The routine accepts NULL pointers as input, in which case the
 * result is TRUE if both string pointers are NULL.

ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CompareStrings(const WE_CHAR *pcBs1,
                             WE_INT32 iLen1,
                             const WE_CHAR *pcBs2,
                             WE_INT32 iLen2,
                             WE_BOOL bNoCase)
{
    WE_CHAR cB1 = '\0';
    WE_CHAR cB2 = '\0';
    WE_CHAR cTmp = '\0';
    WE_BOOL bReservedEscape1 = FALSE;
    WE_BOOL bReservedEscape2 = FALSE;
    
    if(NULL == pcBs1)
        return (NULL == pcBs2);
    else if(NULL == pcBs2)
        return FALSE;
    
    for(;(iLen1 > 0) && (iLen2 > 0);iLen1--,iLen2--)
    {
               cB1 = *pcBs1++;
        if((cB1 == '%') && (iLen1 >= 2) && 
            WE_SCL_HEX2BYTE(pcBs1,(WE_UCHAR *)&cTmp))
        {
            pcBs1 += 2;
            iLen1 -= 2;
            cB1 = cTmp;
            if(ct_isreserved(cTmp) || (cTmp == '#'))
            {
                bReservedEscape1 = TRUE;
            }
        }
        cB2 = *pcBs2++;
        if((cB2 == '%') && (iLen2 >= 2) &&
            WE_SCL_HEX2BYTE(pcBs2,(WE_UCHAR *)&cTmp))
        {
            pcBs2 += 2;
            iLen2 -= 2;
            cB2 = cTmp;
            if(ct_isreserved(cTmp) || (cTmp == '#'))
            {
                bReservedEscape2 = TRUE;
            }
        }
        if(bNoCase)
        {
            cB1 = URL_CHARTOLOW(cB1);
            cB2 = URL_CHARTOLOW(cB2);
        }
        if((cB1 != cB2) || (bReservedEscape1 ^ bReservedEscape2))
            return FALSE;
    }
    return (iLen1 == iLen2);
}




/*=====================================================================================
FUNCTION: 
    WeUrl_Scheme2Type
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    
ARGUMENTS PASSED:
    
RETURN VALUE:
    Return the scheme type named by the given string.
 * Returns scheme_unknown if it is not one of the predefined types.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
E_WEURL_SCHEME WeUrl_Scheme2Type(const WE_CHAR *pcBs,
                                 WE_INT32 iLen)
{
    WE_INT32 iIndx;
    if(NULL == pcBs)
        return E_WE_SCHEME_EMPTY;
    for(iIndx = 0;iIndx < WE_LAST_SCHEME;iIndx++)
    {   
        if((!WeSCL_StrnCmpNc(pcBs,WeUrl_GetSchemeStrInfo(iIndx),iLen))
			&& (iLen == WeUrl_GetSchemeLenInfo(iIndx)))
        {
            return WeUrl_GetSchemeInfo(iIndx);//s_WeUrl_stSchemes[iIndx].eScheme;
        }
    }
    return E_WE_SCHEME_UNKNOWN;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_Clear
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Sets all fields in the URL struct to NULL and 0, respectively.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
void WeUrl_Clear(St_WeUrl *pstUrl)
{
    WE_INT32 iIndx;
    if(NULL == pstUrl)
        return;
    pstUrl->eSchemeType = E_WE_SCHEME_EMPTY;
    for(iIndx = 0;iIndx < WE_URL_NUM_PARTS;iIndx++)
    {
        pstUrl->apcPartsStr[iIndx] = NULL;
        pstUrl->aiPartsLen[iIndx] = 0;
    }
}

/*=====================================================================================
FUNCTION: 
    WeUrl_IsNumber
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check that the string of length 'len' is a non-empty decimal number.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_IsNumber(const WE_CHAR *pcBs,
                                         WE_INT32 iLen)
{
    WE_INT32 iIndx;
    if(iLen <= 0)
        return FALSE;
    for(iIndx = 0;iIndx < iLen;iIndx++,pcBs++)
    {
        if(!ct_isdigit(*pcBs))
            return FALSE;
    }
    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_ValueLimit
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
WE_BOOL WeUrl_ValueLimit(const WE_CHAR *pcBs,
                                          WE_INT32 iLen,
                                          WE_INT32 iLimit)
{
    WE_INT32 iIndx;
    WE_INT32 iWeight = 1;
    WE_INT32 iSum = 0;

    if(iLen <= 0)
        return FALSE;
    for(iIndx = iLen,pcBs += iLen - 1;iIndx > 0 && iSum <= iLimit;iIndx--,pcBs--,iWeight *= 10)
    {
        if(ct_isdigit(*pcBs))
            iSum += ((*pcBs) - '0') * iWeight;
        else
            return FALSE;
    }
    return (iSum <= iLimit);
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckTopLabel
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    * Return the scheme type named by the given string.
 * Returns scheme_unknown if its not one of the predefined types.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckTopLabel(const WE_CHAR *pcBs,
                                                 WE_INT32 iLen)
{
    WE_INT32 iIndx1 = 0;
    WE_INT32 iIndx2 = 0;

    if((NULL == pcBs) || (iLen <= 0))
        return FALSE;	

    if(!ct_isalpha(pcBs[0]))
        return FALSE;
    for(iIndx1 = 1;iIndx1 < iLen -1;iIndx1++)
    {
		if(' ' != pcBs[iIndx1])
		{
			if(!ct_isalphanum(pcBs[iIndx1]) && (pcBs[iIndx1] != '-'))
				return FALSE;
		}
		else
		{
			for(iIndx2 = iIndx1 + 1;iIndx2 < iLen - 1;iIndx2++) 
			{
				if(' ' != pcBs[iIndx2])
					return FALSE;
			}	
			return TRUE;
		}
        
    }
    return ct_isalphanum(pcBs[iLen - 1]);
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckDomainLabel
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
WE_BOOL WeUrl_CheckDomainLabel(const WE_CHAR *pcBs,
                                                       WE_INT32 iLen)
{
    WE_INT32 iIndx;
    if((NULL == pcBs) || (iLen <= 0))
        return FALSE;
    if(!ct_isalphanum(pcBs[0]))
        return FALSE;
    for(iIndx = 1;iIndx < iLen - 1; iIndx++)
    {
        if(!ct_isalphanum(pcBs[iIndx]) && (pcBs[iIndx] != '-') && (pcBs[iIndx] != '_'))
            return FALSE;
    }
    return ct_isalphanum(pcBs[iLen - 1]);
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckHostName
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
WE_BOOL WeUrl_CheckHostName(const WE_CHAR *pcBs,
                            WE_INT32 iLen)
{
    WE_INT32 iIndx1;
    WE_INT32 iIndx2;
    if((NULL == pcBs) || (iLen <= 0))
        return FALSE;

    for(iIndx1 = 0;iIndx1 < iLen;)
    {
        for(iIndx2 = iIndx1; (iIndx2 < iLen) && (pcBs[iIndx2] != '.');iIndx2++);
        if(iIndx2 >= iLen)
            return WeUrl_CheckTopLabel(&pcBs[iIndx1], (WE_UINT16)(iIndx2 - iIndx1));
        if(!WeUrl_CheckDomainLabel(&pcBs[iIndx1], (WE_INT16)(iIndx2 - iIndx1)))
            return FALSE;
        iIndx1 = iIndx2 + 1;
    }
    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckIpv4Address
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     The grammar for IPv4 addresses is:
 *   IPv4address = 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckIpv4Address(const WE_CHAR *pcBs,
                                                       WE_INT32 iLen)
{
    WE_INT32 iIndx1;
    WE_INT32 iIndx2;
    WE_INT32 iNum = 0;
    if((NULL == pcBs) || (iLen <= 0))
        return FALSE;

    for(iIndx1 = 0;iIndx1 < iLen;)
    {
        for(iIndx2 = iIndx1;(iIndx2 < iLen) && (pcBs[iIndx2] != '.');iIndx2++);
        if(iIndx2 + 1 == iLen)
            return FALSE;
        if((iIndx2 == iIndx1) || (iIndx2 > iIndx1 + 3) || !WeUrl_ValueLimit(&pcBs[iIndx1], (WE_INT16)(iIndx2 - iIndx1), 255))
        {
            return FALSE;
        }
        iNum ++;
        iIndx1 = iIndx2 + 1;
    }
    return (iNum == 4);
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckIpv6Address
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * The grammar for IPv6 addresses is:
 *   IPv6address = hexpart [ ":" IPv4address ]
 *   IPv4address = 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
 *
 *   hexpart     = hexseq | hexseq "::" [ hexseq ] | "::" [ hexseq ]
 *   hexseq      = hex4 *( ":" hex4)
 *   hex4        = 1*4HEXDIG
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckIpv6Address(const WE_CHAR *pcS,
                                                      WE_INT32 iLen)
{
    WE_INT32 iIndx1;
    WE_INT32 iIndx2;
    WE_INT32 iIndx3;
    WE_INT32 iLen1 = 0;
    WE_INT32 iLen2 = 0;
    WE_BOOL bIsDouble = FALSE;
    WE_BOOL bHasSeenDouble = FALSE;

    if(iLen <= 0)
        return FALSE;
    for(iIndx1 = 0; iIndx1 < iLen;)
    {
        for(iIndx2 = iIndx1;(iIndx2 < iLen) && (pcS[iIndx2] != ':');iIndx2++);
        if(iIndx2 >= iLen)
        {
            iLen2 = iIndx2 - iIndx1;
            break;
        }
        if((iIndx2 < iLen - 1) && (pcS[iIndx2 + 1] == ':'))
        {
            if(bHasSeenDouble)
                return FALSE;
            bIsDouble = TRUE;
            bHasSeenDouble = TRUE;
        }
        else
        {
            bIsDouble = FALSE;
        }
        iLen1 = iIndx2 - iIndx1;
        if(iLen1 == 0)
        {
            /* Empty first seqment only allowed if it is terminated by "::" */
            if((iIndx1 > 0) || !bIsDouble)
            {
                return FALSE;
            }
        }
        else if (iLen1 > 4)
        {
            return FALSE;
        }
        else
        {
            for(iIndx3 = iIndx1;iIndx3 < iIndx1 + iLen1;iIndx3++)
            {
                if(!ct_ishex(pcS[iIndx3]))
                    return FALSE;
            }
        }
        iIndx1 = iIndx2 + (bIsDouble ? 2 : 1);
    }
    /* Last component can be either a IPv4 address, or a hex4 */
    if(iLen2 == 0)
    {
        return bIsDouble;
        /* The last component may be empty iff it was preceded by "::" */
    }
    else if (NULL != WE_SCL_STRNCHR(pcS + iIndx1, '.', iLen2))
    {
        return WeUrl_CheckIpv4Address(pcS + iIndx1, iLen2);
    }
    else if (iLen2 > 4)
    {
        return FALSE;
    }
    else
    {
        for(iIndx3 = iIndx1;iIndx3 < iIndx1 + iLen2;iIndx3++)
        {
            if(!ct_ishex(pcS[iIndx3]))
                return FALSE;
        }
    }

    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckScheme
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check the syntax of the scheme part.
 * As a side effect, store the scheme type in the we_url_t structure.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckScheme(St_WeUrl *pstUrl)
{
    const WE_CHAR *pcTmp = NULL;
    WE_INT32 iLen = 0;
    WE_INT32 iIndx = 0;

    if(NULL == pstUrl)
	return FALSE;

    pcTmp = pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART];
    iLen = pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART];

    if((NULL == pcTmp) || (iLen == 0))
    {
        pstUrl->eSchemeType = E_WE_SCHEME_EMPTY;
        return TRUE;
    }
	if(E_WE_SCHEME_UNKNOWN == pstUrl->eSchemeType)
		return FALSE;

    /* Must start with a letter. */
    if(!ct_isalpha(*pcTmp))
        return FALSE;
    /* The rest of the scheme characters should be drawn from the set {a-z,A-Z,0-9,+,-,.} */
    for(iIndx = 1;iIndx < iLen;iIndx++)
    {
        WE_CHAR cB = pcTmp[iIndx];
        if(!ct_isalphanum(cB) && (cB != '+') && (cB != '-') && (cB != '.'))
            return FALSE;
    }
    pstUrl->eSchemeType = WeUrl_Scheme2Type(pcTmp,iLen);
    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckUserInfo
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check the syntax of the userinfo part of a URL.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckUserInfo(St_WeUrl *pstUrl)
{
    const WE_CHAR *pcTmp = pstUrl->apcPartsStr[G_WE_URL_USERINFO_PART];
    WE_INT32 iLen = pstUrl->aiPartsLen[G_WE_URL_USERINFO_PART];
    WE_INT32 iIndx;

	if((NULL == pcTmp) || (0 == iLen))
		return TRUE;

    /* The userinfo part is *( pchar | ";" ) */
    for(iIndx = 0;iIndx < iLen;iIndx++)
    {
        WE_CHAR cB = pcTmp[iIndx];
        if(!ct_ispchar(cB) && (cB != ';'))
            return FALSE;
    }
    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckHost
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Check the syntax of the host part of a URL.
 * The host part is defined as:
 *   host  = hostname | IPv4address | IPv6reference
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckHost(St_WeUrl *pstUrl)
{
    const WE_CHAR *pcTmp = pstUrl->apcPartsStr[G_WE_URL_HOST_PART];
    WE_INT32 iLen = pstUrl->aiPartsLen[G_WE_URL_HOST_PART];
    WE_INT32 iIndx;

    if((NULL == pcTmp) || (iLen == 0))
        return TRUE;

	
    if((NULL != (pstUrl->apcPartsStr[G_WE_URL_PATH_PART])) && 
		(' ' == pcTmp[iLen - 1]))
        return FALSE;

    if(pcTmp[0] == '[')
        return WeUrl_CheckIpv6Address(pcTmp + 1, iLen - 2);

    if(pcTmp[0] == '.')
        return FALSE;

    /* Find right-most '.' */
    for(iIndx = iLen - 1; (iIndx > 0) && (pcTmp[iIndx] != '.');iIndx--);

    if(iIndx == 0)
        return WeUrl_CheckTopLabel(pcTmp, iLen);
        /* Only one component, must be a top-level domain. */
    else if (iIndx == iLen - 1)
        return WeUrl_CheckHostName(pcTmp, iLen - 1);
        /* A final '.' can be ignored in a hostname. */
    else if(ct_isdigit(pcTmp[iIndx + 1]))
        return WeUrl_CheckIpv4Address(pcTmp, iLen);
        /* If the final component starts with a digit, it must be an IP-address */
    else 
        return WeUrl_CheckHostName(pcTmp, iLen);
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckPort
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Check the syntax of the port number field of a URL.
 * The definition of the port number field is:  port = *digit
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckPort(St_WeUrl *pstUrl)
{
    const WE_CHAR *pcTmp = pstUrl->apcPartsStr[G_WE_URL_PORT_PART];
    WE_INT32 iLen = pstUrl->aiPartsLen[G_WE_URL_PORT_PART];
    WE_INT32 iIndx;

    if(NULL == pcTmp)
        return TRUE;

    if(iLen == 0)
        return FALSE;

    for(iIndx = 0;iIndx < iLen;iIndx++)
    {
        if(!ct_isdigit(pcTmp[iIndx]))
            return FALSE;
    }
    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckAuthority
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check the syntax of the "authority" part of a URL.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckAuthority(St_WeUrl *pstUrl)
{
    WE_BOOL bResult = FALSE;

    if((NULL == (pstUrl->apcPartsStr[G_WE_URL_AUTHORITY_PART])) || 
      (pstUrl->aiPartsLen[G_WE_URL_AUTHORITY_PART] == 0))
    {
        bResult = TRUE;
        return bResult;
    }

    if(WeUrl_CheckUserInfo(pstUrl) && WeUrl_CheckHost(pstUrl) && WeUrl_CheckPort(pstUrl))
        bResult = TRUE;
    return bResult;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckQuery
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check the syntax of the query part of a URL.
 * The definition of the query part is:  query = *uric
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckQueryEx(St_WeUrl *pstUrl,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    const WE_CHAR *pcTmp = pstUrl->apcPartsStr[G_WE_URL_QUERY_PART];
    WE_INT32 iLen = pstUrl->aiPartsLen[G_WE_URL_QUERY_PART];
    WE_INT32 iIndx;
	WE_CHAR *pcQueryTmp = NULL;

	if((NULL == pcTmp) || (0 == iLen))
		return TRUE;

 /*Allow the unwise characters in query part,Most other browsers allow it.*/    
    if(TRUE == bMemMgr)
	pcQueryTmp = WeUrl_EscapeAllStringEx(pcTmp,TF3RD_STRLEN(pcTmp),hMem,TRUE);
	else
	pcQueryTmp = WeUrl_EscapeAllStringEx(pcTmp,WE_SCL_STRLEN(pcTmp),NULL,FALSE);

    if(NULL == pcQueryTmp)
	return TRUE;
    for(iIndx = 0;iIndx < iLen;iIndx++)
    {
        WE_CHAR cB = pcQueryTmp[iIndx];
        /* Allow \ even thou it should not be allowed according to the specification. 
        Most other browsers allow it. */
        if(!ct_isuric(cB) && cB != '\\')
        {
            if((cB == '%') && WE_SCL_HEX2BYTE(pcQueryTmp + iIndx + 1,(WE_UCHAR *)&cB))
            {
                iIndx += 2;
            }
            else
            {
				if(TRUE == bMemMgr)
				{
					BrwMem_Free(hMem, pcQueryTmp);					
				}
				else
				{
					WE_FREE(pcQueryTmp);					
				}
            
                return FALSE;
            }
        }
    }

	if(TRUE == bMemMgr)
	{
		BrwMem_Free(hMem, pcQueryTmp);		
	}
    else
	{
		WE_FREE(pcQueryTmp);		
	}

    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckFragment
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check the syntax of the fragment part of a URL.
 * The definition of the fragment part is:  fragment = *uric
 * NOTE: our checking of fragments differ from RFC2396 in that we
 * allow '#' and '"' in fragment parts.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckFragment(St_WeUrl *pstUrl)
{
    const WE_CHAR *pcTmp = pstUrl->apcPartsStr[G_WE_URL_FRAGMENT_PART];
    WE_INT32 iLen = pstUrl->aiPartsLen[G_WE_URL_FRAGMENT_PART];
    WE_INT32 iIndx;

	if((NULL == pcTmp) || (0 == iLen))
		return TRUE;

    for(iIndx = 0;iIndx < iLen;iIndx++)
    {
        WE_CHAR cB = pcTmp[iIndx];
        if(!ct_isuric(cB) && (cB != '#') && (cB != '"'))
        {
            if((cB == '%') && WE_SCL_HEX2BYTE(pcTmp + iIndx + 1, (WE_UCHAR *) &cB))
                iIndx += 2;
            else
                return FALSE;
        }
    }
    return TRUE;  
}

/*=====================================================================================
FUNCTION: 
    WeUrl_CheckPath
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Check the syntax of the path component of a URL.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_CheckPathEx(St_WeUrl *pstUrl,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
    const WE_CHAR *pcTmp = pstUrl->apcPartsStr[G_WE_URL_PATH_PART];
    WE_INT32 iLen = pstUrl->aiPartsLen[G_WE_URL_PATH_PART];
    WE_INT32 iIndx = 0;
	WE_CHAR *pcPathTmp = NULL;
  

	if((NULL == pcTmp) || (0 == iLen))
		return TRUE;

    /*Allow the unwise characters in query part,Most other browsers allow it.*/    	
	if(TRUE == bMemMgr)
		pcPathTmp = WeUrl_EscapeUrlStringEx(pcTmp,hMem,TRUE);
	else
		pcPathTmp = WeUrl_EscapeUrlStringEx(pcTmp,NULL,FALSE);

	if(NULL == pcPathTmp)
		return TRUE;
	
	for(iIndx = 0;iIndx < iLen;iIndx++)
	{  
        WE_CHAR cB = pcPathTmp[iIndx];
        if(!ct_ispchar(cB) &&('/' != cB ) && (';' != cB) && ('<' != cB) && ('>' != cB) )
		{
            if( ('%' == cB) && WE_SCL_HEX2BYTE(pcPathTmp + iIndx + 1, (WE_UCHAR *) &cB))
                iIndx += 2;
			else
			{
				if(TRUE == bMemMgr)
				{
					BrwMem_Free(hMem, pcPathTmp);					
				}
				else
				{
					WE_FREE(pcPathTmp);					
				}
				return FALSE;
	           }
	       }	
	   }
	if(TRUE == bMemMgr)
	{
		BrwMem_Free(hMem, pcPathTmp);		
	}
	else
	{
		WE_FREE(pcPathTmp);		
	}
	
    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_Assemble
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * The inverse of the parsing routine: create a URL string
 * from its different parts.
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned string.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_CHAR *WeUrl_AssembleEx(St_WeUrl *pstUrl,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_INT32 iLen = 
              (pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART] != NULL ? pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART] +1 : 0) +
              (pstUrl->apcPartsStr[G_WE_URL_AUTHORITY_PART] != NULL ? 2 : 0) +
              (pstUrl->apcPartsStr[G_WE_URL_USERINFO_PART] != NULL ? pstUrl->aiPartsLen[G_WE_URL_USERINFO_PART] +1 : 0) +
              (pstUrl->apcPartsStr[G_WE_URL_HOST_PART] != NULL ? pstUrl->aiPartsLen[G_WE_URL_HOST_PART] + 1 : 0) +
              (pstUrl->apcPartsStr[G_WE_URL_PORT_PART] != NULL ? pstUrl->aiPartsLen[G_WE_URL_PORT_PART] + 1 : 0) +
              (pstUrl->apcPartsStr[G_WE_URL_PATH_PART] != NULL ? pstUrl->aiPartsLen[G_WE_URL_PATH_PART] + 1 : 0) +
              (pstUrl->apcPartsStr[G_WE_URL_QUERY_PART] != NULL ? pstUrl->aiPartsLen[G_WE_URL_QUERY_PART] + 1 : 0) +
              (pstUrl->apcPartsStr[G_WE_URL_FRAGMENT_PART] != NULL ? pstUrl->aiPartsLen[G_WE_URL_FRAGMENT_PART] + 1 : 0);
    WE_CHAR *pcBuf = NULL;
    WE_CHAR *pcTmp = NULL;
    if(TRUE == bMemMgr)
		pcBuf = BrwMem_Alloc(hMem,iLen + 1);
	else
		pcBuf = WE_MALLOC(iLen + 1);

    if(NULL == pcBuf)
        return NULL;

    pcTmp = pcBuf;
    if(pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART] != NULL)
    {
        WE_MEMCPY(pcTmp,pstUrl->apcPartsStr[G_WE_URL_SCHEME_PART],pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART]);
        pcTmp += pstUrl->aiPartsLen[G_WE_URL_SCHEME_PART];
        *pcTmp++ = ':';
    }
    if(pstUrl->apcPartsStr[G_WE_URL_AUTHORITY_PART] != NULL)
    {
        *pcTmp++ = '/';
        *pcTmp++ = '/';
    }
    if(pstUrl->apcPartsStr[G_WE_URL_USERINFO_PART] != NULL)
    {
        WE_MEMCPY(pcTmp,pstUrl->apcPartsStr[G_WE_URL_USERINFO_PART],pstUrl->aiPartsLen[G_WE_URL_USERINFO_PART]);
        pcTmp += pstUrl->aiPartsLen[G_WE_URL_USERINFO_PART];
        *pcTmp++ = '@';
    }
    if(pstUrl->apcPartsStr[G_WE_URL_HOST_PART] != NULL)
    {
        WE_MEMCPY(pcTmp,pstUrl->apcPartsStr[G_WE_URL_HOST_PART],pstUrl->aiPartsLen[G_WE_URL_HOST_PART]);
        pcTmp += pstUrl->aiPartsLen[G_WE_URL_HOST_PART];
    }
    if(pstUrl->apcPartsStr[G_WE_URL_PORT_PART] != NULL)
    {
        *pcTmp++ = ':';
        WE_MEMCPY(pcTmp,pstUrl->apcPartsStr[G_WE_URL_PORT_PART],pstUrl->aiPartsLen[G_WE_URL_PORT_PART]);
        pcTmp += pstUrl->aiPartsLen[G_WE_URL_PORT_PART];
    }
    if(pstUrl->apcPartsStr[G_WE_URL_PATH_PART] != NULL)
    {
        WE_MEMCPY(pcTmp,pstUrl->apcPartsStr[G_WE_URL_PATH_PART],pstUrl->aiPartsLen[G_WE_URL_PATH_PART]);
        pcTmp += pstUrl->aiPartsLen[G_WE_URL_PATH_PART];
    }
    if(pstUrl->apcPartsStr[G_WE_URL_QUERY_PART] != NULL)
    {
        *pcTmp++ = '?';
        WE_MEMCPY(pcTmp,pstUrl->apcPartsStr[G_WE_URL_QUERY_PART],pstUrl->aiPartsLen[G_WE_URL_QUERY_PART]);
        pcTmp += pstUrl->aiPartsLen[G_WE_URL_QUERY_PART];
    } 
    if(pstUrl->apcPartsStr[G_WE_URL_FRAGMENT_PART] != NULL)
    {
        *pcTmp++ = '#';
        WE_MEMCPY(pcTmp,pstUrl->apcPartsStr[G_WE_URL_FRAGMENT_PART],pstUrl->aiPartsLen[G_WE_URL_FRAGMENT_PART]);
        pcTmp += pstUrl->aiPartsLen[G_WE_URL_FRAGMENT_PART];
    }
    *pcTmp = '\0';
    return pcBuf;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_HashPart
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Hash part of a URL, while replacing escape sequences with
 * the characters they represent, unless they are "reserved".
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
void WeUrl_HashPart(St_WeUrl *pstUrl,
                                WE_INT32 iWhichPart,
                                WE_UINT32 *puiN,
                                WE_INT32 *piD)
{
    const WE_CHAR *pcTmp;
    WE_INT32 iLen;
    WE_INT32 iIndx;
    WE_CHAR cChar;
    WE_CHAR cCharTmp;
    if((NULL == puiN) || (NULL == pstUrl) || (NULL == piD))
        return;

    pcTmp = pstUrl->apcPartsStr[iWhichPart];
    iLen = pstUrl->aiPartsLen[iWhichPart];
    if((pcTmp != NULL) && (iLen > 0))
    {
        for(iIndx = 0; iIndx < iLen;iIndx++)
        {
            cChar = pcTmp[iIndx];
            if((cChar == '%') && (iIndx + 2 < iLen) && 
              WE_SCL_HEX2BYTE(&pcTmp[iIndx + 1], (WE_UCHAR *)&cCharTmp))
            {
                if(ct_isreserved(cCharTmp) || (cCharTmp == '#'))
                {
                    HASH_CHAR(*puiN, cChar, *piD);
                    HASH_CHAR(*puiN, pcTmp[iIndx + 1], *piD);
                    HASH_CHAR(*puiN, pcTmp[iIndx + 2], *piD);
                }
                else
                {
                    HASH_CHAR(*puiN, cCharTmp, *piD);
                }
                iIndx += 2;
            }
            else
            {
                HASH_CHAR(*puiN, cChar, *piD);
            }
        }
    }
}

/*=====================================================================================
FUNCTION: 
    WeUrl_RemoveDots
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
    Given two path names, an absolute path (starting with '/') in 'base', and a relative path in 'rel',
    combine them into a new absolute path removing all unnecessary './' and '../' segments.
ARGUMENTS PASSED:
    
RETURN VALUE:
    Returns a newly allocated string with the new path.
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_CHAR *WeUrl_RemoveDotsEx(const WE_CHAR *pcBase,
                          WE_INT32 iBaseLen,
                          const WE_CHAR *pcRel,
                          WE_INT32 iRelLen,
                          WE_HANDLE hMem,
						  WE_BOOL bMemMgr)
{
    WE_INT32 iLen = 0;
    WE_CHAR *pcBuf = NULL;
    WE_CHAR *pcTmp1 = NULL;
    WE_CHAR *pcTmp2 = NULL;
    WE_INT32 iIndx1;
    WE_INT32 iIndx2;
    WE_INT32 iN;
    WE_INT32 iNumSegs;
    WE_CHAR **ppcSegment = NULL;
    WE_BOOL bRemovedFinalSlash = FALSE;

    if(NULL == pcBase)
        return NULL;	

  /* Find the right-most '/' character in the base path. */
    for(iIndx1 = iBaseLen - 1;iIndx1 >= 0;iIndx1--)
    {
        if(pcBase[iIndx1] == '/')
            break;
    }
    if(iIndx1 >= 0)
        iLen = iIndx1 + 1;
    if(TRUE == bMemMgr)
		pcBuf = BrwMem_Alloc(hMem,iLen + iRelLen + 1);
	else
		pcBuf = WE_MALLOC(iLen + iRelLen + 1);

     if(NULL == pcBuf)
         return NULL;
    /* Append the relative path to the base path. */
    WE_MEMCPY(pcBuf,pcBase,iLen);
    WE_MEMCPY(pcBuf + iLen,pcRel,iRelLen);
    iN = iLen + iRelLen;
    pcBuf[iN] = '\0';

    /* Count the number of '/'-characters */
    iIndx2 = 0;
    for(iIndx1 = 0;iIndx1 < iN - 1;iIndx1++)
    {
        if(pcBuf[iIndx1] == '/')
            iIndx2++;
    }
    iNumSegs = iIndx2 + 1;
	if(TRUE == bMemMgr)
		ppcSegment = BrwMem_Alloc(hMem,iNumSegs * sizeof(WE_CHAR *) + 1);
	else
		ppcSegment = WE_MALLOC(iNumSegs * sizeof(WE_CHAR *) + 1);

    if(NULL == ppcSegment)
    {
		if(TRUE == bMemMgr)
		{
			BrwMem_Free(hMem,pcBuf);			
		}
		else
		{
			WE_FREE(pcBuf);        			
		}
        return NULL;
    }
    /* Find all '/'-characters, and replace with null bytes. */
    iIndx2 = 0;
    for(iIndx1 = 0;iIndx1 < iN - 1;iIndx1++)
    {
        if(pcBuf[iIndx1] == '/')
        {
            pcBuf[iIndx1] = '\0';
            ppcSegment[iIndx2++] = pcBuf + iIndx1;
        }
    }
    if(pcBuf[iN - 1] == '/')
    {
        pcBuf[iN - 1] = '\0';
        ppcSegment[iIndx2++] = pcBuf + iN -1;
        bRemovedFinalSlash = TRUE;
    }
    else
    {
        ppcSegment[iIndx2++] = pcBuf + iN;
    }
    /* Remove all occurrences of './' */
    iIndx2 = 0;
    for(iIndx1 = 0;iIndx1 < iNumSegs - 1;iIndx1++)
    {
        pcTmp1 = ppcSegment[iIndx1];
        if(!IS_DOT(pcTmp1))
        {
            ppcSegment[iIndx2++] = ppcSegment[iIndx1];
        }
    }
    pcTmp1 = ppcSegment[iNumSegs - 1];
    if(!IS_DOT(pcTmp1))
    {
        ppcSegment[iIndx2++] = ppcSegment[iIndx1];
    }
    else
    {
        bRemovedFinalSlash = TRUE;
    }
    iNumSegs = iIndx2;

    /* Remove all occurrences of '<segment>/../'. */
    for(iIndx1 = 0,iIndx2 = 0;iIndx1 < iNumSegs - 1;iIndx1++)
    {
        if((iIndx2 > 1) && !IS_DOTDOT(ppcSegment[iIndx2 - 1]) && IS_DOTDOT(ppcSegment[iIndx1]))
        {
            iIndx2--;
        }
        else
        {
            ppcSegment[iIndx2++] = ppcSegment[iIndx1];
        }
    }
    if((iIndx2 > 1) && !IS_DOTDOT(ppcSegment[iIndx2 - 1]) && IS_DOTDOT(ppcSegment[iNumSegs - 1]))
    {
        iIndx2--;
        bRemovedFinalSlash = TRUE;
    }
    else
    {
    	if(iNumSegs > 0)
    	{
    		ppcSegment[iIndx2++] = ppcSegment[iNumSegs - 1];
    	}       
    }
    iNumSegs = iIndx2;

    /* Assemble final string */
    pcTmp1 = pcBuf;
    for(iIndx1 = 1;iIndx1 < iNumSegs;iIndx1++)
    {
        *pcTmp1 = '/';
        if(ppcSegment[iIndx1] == NULL)
            break;
        pcTmp2 = ppcSegment[iIndx1] - 1;
        while((*pcTmp2 != '\0') && (*pcTmp2 != '/'))
            pcTmp2--;
        while((*++pcTmp1 = *++pcTmp2) != '\0');
    }
    if(bRemovedFinalSlash)
        *pcTmp1++ = '/';
    *pcTmp1 = '\0';
	if(TRUE == bMemMgr)
	{
		BrwMem_Free(hMem,ppcSegment);		
	}
	else
	{
		WE_FREE(ppcSegment);		
	}

    return pcBuf;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_ResolveInternal
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Given a base URL and a relative URL, resolve the relative reference
 * and store as an absolute URL in the string "*abs".
 * Returns TRUE on success, FALSE otherwise, in which case nothing
 * is stored in "abs".
 * NOTE: It is the callers responsibility to deallocate the returned string.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_ResolveInternalEx(St_WeUrl *pstBase,
                              St_WeUrl *pstRel,
                              WE_CHAR **ppAbs,
                              WE_HANDLE hMem,
							  WE_BOOL bMemMgr)
{
    St_WeUrl stNewUrl;

    if((NULL == pstBase) || (NULL == pstRel) || (NULL == ppAbs))
        return FALSE;
    if(NULL == (pstBase->apcPartsStr[G_WE_URL_SCHEME_PART]))
        return FALSE;/* Not a correct absolute URL. */

    if(pstBase->aiPartsLen[G_WE_URL_PATH_PART] == 0)
    {
        pstBase->apcPartsStr[G_WE_URL_PATH_PART] = "/";
        pstBase->aiPartsLen[G_WE_URL_PATH_PART] = 1;
    }

    /* If rel has a scheme, then it is an absolute reference. Just copy it. */
    if(NULL != (pstRel->apcPartsStr[G_WE_URL_SCHEME_PART]))
    {
		if(TRUE == bMemMgr)
		{
			if(NULL == (*ppAbs = WeUrl_AssembleEx(pstRel,hMem,TRUE)))
				return FALSE;
			
		}
		else
		{
			if(NULL == (*ppAbs = WeUrl_AssembleEx(pstRel,NULL,FALSE)))
				return FALSE;						
		}
    }
    /* If rel has an authority part, just use the scheme from the base part. */
    else if(NULL != (pstRel->apcPartsStr[G_WE_URL_AUTHORITY_PART]))
    {
        stNewUrl = *pstRel;
        stNewUrl.apcPartsStr[G_WE_URL_SCHEME_PART] = pstBase->apcPartsStr[G_WE_URL_SCHEME_PART];
        stNewUrl.aiPartsLen[G_WE_URL_SCHEME_PART] = pstBase->aiPartsLen[G_WE_URL_SCHEME_PART];
        stNewUrl.eSchemeType = pstBase->eSchemeType;
		if(TRUE == bMemMgr)
			*ppAbs = WeUrl_AssembleEx(&stNewUrl,hMem,TRUE);
		else
			*ppAbs = WeUrl_AssembleEx(&stNewUrl,NULL,FALSE);
    }
    else if((pstRel->aiPartsLen[G_WE_URL_PATH_PART]) == 0)
    {
        /* 'pstRel' is empty or just a fragment */
        stNewUrl = *pstBase;
        stNewUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART] = pstRel->apcPartsStr[G_WE_URL_FRAGMENT_PART];
        stNewUrl.aiPartsLen[G_WE_URL_FRAGMENT_PART] = pstRel->aiPartsLen[G_WE_URL_FRAGMENT_PART];
        stNewUrl.apcPartsStr[G_WE_URL_QUERY_PART] = pstRel->apcPartsStr[G_WE_URL_QUERY_PART];
        stNewUrl.aiPartsLen[G_WE_URL_QUERY_PART] = pstRel->aiPartsLen[G_WE_URL_QUERY_PART];
		if(TRUE == bMemMgr)
			*ppAbs = WeUrl_AssembleEx(&stNewUrl,hMem,TRUE);
		else
			*ppAbs = WeUrl_AssembleEx(&stNewUrl,NULL,FALSE);
    }
    else
    {
        /* 'rel' does not have a scheme nor an authority part, but is more than just a fragment */
        WE_CHAR *pcTmp;
		WE_UINT32 index = 0;
        WE_INT32 ioffset = 0;
        if(pstRel->aiPartsLen[G_WE_URL_PATH_PART] == 0)
        {
			if(TRUE == bMemMgr)
				pcTmp = WeUrl_RemoveDotsEx(pstBase->apcPartsStr[G_WE_URL_PATH_PART],pstBase->aiPartsLen[G_WE_URL_PATH_PART],"",0,hMem,TRUE);
			else
				pcTmp = WeUrl_RemoveDotsEx(pstBase->apcPartsStr[G_WE_URL_PATH_PART],pstBase->aiPartsLen[G_WE_URL_PATH_PART],"",0,NULL,FALSE);
		}
        else if (*(pstRel->apcPartsStr[G_WE_URL_PATH_PART]) != '/')
        {
			if(TRUE == bMemMgr)
				pcTmp = WeUrl_RemoveDotsEx(pstBase->apcPartsStr[G_WE_URL_PATH_PART],
										 pstBase->aiPartsLen[G_WE_URL_PATH_PART],
										 pstRel->apcPartsStr[G_WE_URL_PATH_PART],
										 pstRel->aiPartsLen[G_WE_URL_PATH_PART],
										 hMem,TRUE);
			else
				pcTmp = WeUrl_RemoveDotsEx(pstBase->apcPartsStr[G_WE_URL_PATH_PART],
										 pstBase->aiPartsLen[G_WE_URL_PATH_PART],
										 pstRel->apcPartsStr[G_WE_URL_PATH_PART],
                                         pstRel->aiPartsLen[G_WE_URL_PATH_PART],NULL,FALSE);
                                         

			if((NULL != pcTmp) && (WE_SCL_STRLEN(pcTmp) >= 3))
			{
				for(index = 0;index < (WE_UINT32)(WE_SCL_STRLEN(pcTmp) - 2);index++)
				{
					if((pcTmp[index] == '.')&&(pcTmp[index + 1] == '.')&&(pcTmp[index + 2] == '/'))
						ioffset++;	
				}
			}
			
        }
        else
        {
			if(TRUE == bMemMgr)
				pcTmp = Brw_StrnDup(hMem,pstRel->apcPartsStr[G_WE_URL_PATH_PART],
									pstRel->aiPartsLen[G_WE_URL_PATH_PART]);
			else
				pcTmp = WE_SCL_STRNDUP(pstRel->apcPartsStr[G_WE_URL_PATH_PART],
										pstRel->aiPartsLen[G_WE_URL_PATH_PART]);
        }

        stNewUrl = *pstBase;
        stNewUrl.apcPartsStr[G_WE_URL_PATH_PART] = pcTmp + ioffset*3;
        stNewUrl.aiPartsLen[G_WE_URL_PATH_PART] = WE_SCL_STRLEN(pcTmp) - ioffset*3;
        stNewUrl.apcPartsStr[G_WE_URL_QUERY_PART] = pstRel->apcPartsStr[G_WE_URL_QUERY_PART];
        stNewUrl.aiPartsLen[G_WE_URL_QUERY_PART] = pstRel->aiPartsLen[G_WE_URL_QUERY_PART];
        stNewUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART] = pstRel->apcPartsStr[G_WE_URL_FRAGMENT_PART];
        stNewUrl.aiPartsLen[G_WE_URL_FRAGMENT_PART] = pstRel->aiPartsLen[G_WE_URL_FRAGMENT_PART];
		if(TRUE == bMemMgr)
		{
			*ppAbs = WeUrl_AssembleEx(&stNewUrl,hMem,TRUE);
			BrwMem_Free(hMem,pcTmp);
		}
		else
		{
			*ppAbs = WeUrl_AssembleEx(&stNewUrl,NULL,FALSE);
			WE_FREE(pcTmp);			
		}
    }

    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_EqualInternal
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Return TRUE if the two URLs are equal, FALSE otherwise.
 * "which_components" is a bitmap indicating which parts of the URLs
 * should be included in the comparison.
 * Returns FALSE in case of error.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_EqualInternal(St_WeUrl *pstUrl1,
                                              St_WeUrl *pstUrl2,
                                              WE_INT32 iWhichComponents)
{
    const WE_CHAR *pcTmp1 = NULL;
    const WE_CHAR *pcTmp2 = NULL;
    const WE_CHAR *pcPath1 = NULL;
    const WE_CHAR *pcPath2 = NULL;
    WE_INT32 iTmp1Len = 0;
    WE_INT32 iTmp2Len = 0;
    WE_INT32 iPath1Len = 0;
    WE_INT32 iPath2Len = 0;
    WE_INT32 iPort1 = 0;
    WE_INT32 iPort2 = 0;
  
    if(NULL == pstUrl1)
        return (NULL == pstUrl2);
    if(NULL == pstUrl2)
        return FALSE;

    if(iWhichComponents & G_WE_URL_PORT_COMP)
    {
        if((pstUrl1->apcPartsStr[G_WE_URL_PORT_PART] == NULL) &&
           (pstUrl1->eSchemeType == E_WE_SCHEME_HTTP))
        {
            pcTmp1 = "80";
            iTmp1Len = 2;
        }
        else if ((pstUrl1->apcPartsStr[G_WE_URL_PORT_PART] == NULL) &&
                     (pstUrl1->eSchemeType == E_WE_SCHEME_HTTPS))
        {
            pcTmp1 = "443";
            iTmp1Len = 3;
        }
        else
        {
            pcTmp1 = pstUrl1->apcPartsStr[G_WE_URL_PORT_PART];
            iTmp1Len = pstUrl1->aiPartsLen[G_WE_URL_PORT_PART];
        }
        iPort1 = WeUrl_GetNum(pcTmp1, iTmp1Len);

        if((pstUrl2->apcPartsStr[G_WE_URL_PORT_PART] == NULL) &&
           (pstUrl2->eSchemeType == E_WE_SCHEME_HTTP))
        {
            pcTmp2 = "80";
            iTmp2Len = 2;
        }
        else if((pstUrl2->apcPartsStr[G_WE_URL_PORT_PART] == NULL) &&
                   (pstUrl2->eSchemeType == E_WE_SCHEME_HTTPS))
        {
            pcTmp2 = "443";
            iTmp2Len = 3;
        }
        else 
        {
            pcTmp2 = pstUrl2->apcPartsStr[G_WE_URL_PORT_PART];
            iTmp2Len = pstUrl2->aiPartsLen[G_WE_URL_PORT_PART];
        }
        iPort2 = WeUrl_GetNum(pcTmp2,iTmp2Len);
    }

    if(pstUrl1->aiPartsLen[G_WE_URL_PATH_PART] == 0)
    {
        pcPath1 = "/";
        iPath1Len = 1;
    }
    else
    {
        pcPath1 = pstUrl1->apcPartsStr[G_WE_URL_PATH_PART];
        iPath1Len = pstUrl1->aiPartsLen[G_WE_URL_PATH_PART];
    }

    if(pstUrl2->aiPartsLen[G_WE_URL_PATH_PART] == 0)
    {
        pcPath2 = "/";
        iPath2Len = 1;
    }
    else
    {
        pcPath2= pstUrl2->apcPartsStr[G_WE_URL_PATH_PART];
        iPath2Len = pstUrl2->aiPartsLen[G_WE_URL_PATH_PART];
    }

    if( (iWhichComponents & G_WE_URL_SCHEME_COMP) &&
         !(WeUrl_CompareStrings(pstUrl1->apcPartsStr[G_WE_URL_SCHEME_PART],
                                pstUrl1->aiPartsLen[G_WE_URL_SCHEME_PART],
                                pstUrl2->apcPartsStr[G_WE_URL_SCHEME_PART],
                                pstUrl2->aiPartsLen[G_WE_URL_SCHEME_PART],
                                TRUE)))
    {
        return FALSE;
    }

    if((iWhichComponents & G_WE_URL_USERINFO_COMP) &&
        !(WeUrl_CompareStrings(pstUrl1->apcPartsStr[G_WE_URL_USERINFO_PART],
                               pstUrl1->aiPartsLen[G_WE_URL_USERINFO_PART],
                               pstUrl2->apcPartsStr[G_WE_URL_USERINFO_PART],
                               pstUrl2->aiPartsLen[G_WE_URL_USERINFO_PART],
                               TRUE)))
    {
        return FALSE;
    }

    if((iWhichComponents & G_WE_URL_HOST_COMP) && 
        !(WeUrl_CompareStrings(pstUrl1->apcPartsStr[G_WE_URL_HOST_PART],
                               pstUrl1->aiPartsLen[G_WE_URL_HOST_PART],
                               pstUrl2->apcPartsStr[G_WE_URL_HOST_PART],
                               pstUrl2->aiPartsLen[G_WE_URL_HOST_PART],
                               TRUE)))
    {
        return FALSE;
    }

    if((iWhichComponents & G_WE_URL_PORT_COMP) && (iPort1 != iPort2))
    {
        return FALSE;
    }

    if((iWhichComponents & G_WE_URL_PATH_COMP) && 
       !(WeUrl_CompareStrings(pcPath1,iPath1Len,pcPath2,iPath2Len,FALSE)))
    {
        return FALSE;
    }

    if((iWhichComponents & G_WE_URL_QUERY_COMP) && 
        !(WeUrl_CompareStrings(pstUrl1->apcPartsStr[G_WE_URL_QUERY_PART],
                               pstUrl1->aiPartsLen[G_WE_URL_QUERY_PART],
                               pstUrl2->apcPartsStr[G_WE_URL_QUERY_PART],
                               pstUrl2->aiPartsLen[G_WE_URL_QUERY_PART],
                               FALSE)))
    {
        return FALSE;
    }

    if((iWhichComponents & G_WE_URL_FRAG_COMP) && 
       !(WeUrl_CompareStrings(pstUrl1->apcPartsStr[G_WE_URL_FRAGMENT_PART],
                              pstUrl1->aiPartsLen[G_WE_URL_FRAGMENT_PART],
                              pstUrl2->apcPartsStr[G_WE_URL_FRAGMENT_PART],
                              pstUrl2->aiPartsLen[G_WE_URL_FRAGMENT_PART],
                              FALSE)))
    {
        return FALSE;
    }

    return TRUE;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetPart
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Retrieval of the different parts of a URL.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_CHAR *WeUrl_GetPartEx(const WE_CHAR *pcUrl,
                       WE_INT32 iWhichPart,
                       WE_HANDLE hMem,
					   WE_BOOL bMemMgr)
                       
{
    St_WeUrl stUrl;
    WE_CHAR *pcPart = NULL;

    if((NULL != pcUrl) && WeUrl_Parse(pcUrl, &stUrl) && (NULL != stUrl.apcPartsStr[iWhichPart]))
    {
        if(TRUE == bMemMgr)
			pcPart = Brw_StrnDup(hMem,stUrl.apcPartsStr[iWhichPart],stUrl.aiPartsLen[iWhichPart]);
		else
			pcPart = WE_SCL_STRNDUP(stUrl.apcPartsStr[iWhichPart],stUrl.aiPartsLen[iWhichPart]);
    }

    return pcPart;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetPathInternal
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Extract the scheme of a URL.
 * Returns NULL in case of error, including that the URL is not valid,
 * or if the URL does not have a scheme component.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_CHAR *WeUrl_GetPathInternalEx(St_WeUrl *pstUrl,WE_HANDLE hMem, WE_BOOL bMemMgr)
{
    const WE_CHAR *pcTmp1;
    WE_CHAR *pcBs = NULL;
    WE_CHAR *pcTmp2 = NULL;
    WE_INT32 iIndx = 0;
    WE_INT32 iLen = 0;
    WE_BOOL bInparam = FALSE;

    if((NULL == pstUrl) || (pstUrl->aiPartsLen[G_WE_URL_PATH_PART] == 0))
    {
        return NULL;
    }
    pcTmp1 = pstUrl->apcPartsStr[G_WE_URL_PATH_PART];
   
    for(iIndx = 0;iIndx < pstUrl->aiPartsLen[G_WE_URL_PATH_PART];iIndx++)
    {
        if(bInparam && (pcTmp1[iIndx] == '/'))
        {
            bInparam = FALSE;
        }
        else if(!bInparam && (pcTmp1[iIndx] == ';'))
        {
            bInparam = TRUE;
        }

        if(!bInparam)
        {
            iLen++;
        }
    }
    if(TRUE == bMemMgr)
		pcBs = BrwMem_Alloc(hMem,iLen + 1);
	else
		pcBs = WE_MALLOC(iLen + 1);

    if(NULL == pcBs)
        return NULL;
    pcTmp2 = pcBs;
    pcTmp1 = pstUrl->apcPartsStr[G_WE_URL_PATH_PART];
    bInparam = FALSE;

    for(iIndx = 0;iIndx < pstUrl->aiPartsLen[G_WE_URL_PATH_PART];iIndx++)
    {
        if(bInparam && (pcTmp1[iIndx] == '/'))
        {
            bInparam = FALSE;
        }
        else if(!bInparam && (pcTmp1[iIndx] == ';'))
        {
            bInparam = TRUE;
        }

        if(!bInparam)
        {
            *pcTmp2++ = pcTmp1[iIndx];
        }
    }
    *pcTmp2 = '\0';

    return pcBs;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_GetParametersInternal
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Return the parameter component of a URL.
 * Returns NULL in case of error, or if the URL does not have any parameters.
 * NOTE: it is the responsibility of the caller to deallocate the string.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_CHAR *WeUrl_GetParametersInternalEx(St_WeUrl *pstUrl,WE_HANDLE hMem , WE_BOOL bMemMgr)
{
    WE_INT32 iIndx;
    WE_INT32 iLen;
    WE_INT32 iTmpLen;
    const WE_CHAR *pcTmp;
    WE_CHAR *pcBs;

    if((NULL == pstUrl) || (pstUrl->aiPartsLen[G_WE_URL_PATH_PART] == 0))
    {
        return NULL;
    }
    iLen = pstUrl->aiPartsLen[G_WE_URL_PATH_PART];
    pcTmp = pstUrl->apcPartsStr[G_WE_URL_PATH_PART];

    /* Find the right-most '/', if any. */
    for(iIndx = iLen - 1;(iIndx >= 0) && (pcTmp[iIndx] != '/');iIndx--);
    iIndx++;
    
    /* Find the first ';' after the last '/', if any. */
    for(;(iIndx < iLen) && (pcTmp[iIndx] != ';');iIndx++);

    if(iIndx == iLen)
    {
        /* The last path segment has no parameter part */
        return NULL;
    }
    pcTmp += iIndx + 1;
    iTmpLen = iLen - (iIndx + 1);
    if(TRUE == bMemMgr)
		pcBs = BrwMem_Alloc (hMem,iTmpLen + 1);
	else
		pcBs = WE_MALLOC (iTmpLen + 1);

    if(NULL == pcBs)
        return NULL;

    WE_MEMCPY(pcBs,pcTmp,iTmpLen);
    pcBs[iTmpLen] = '\0';

    return pcBs;
}

/*=====================================================================================
FUNCTION: 
    WeUrl_UnescapeStringInPlace
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Copy the string "src" to the string "dst", while replacing
 * all escape sequences with the characters they represent.
 * Works correctly even if called with the same argument for src and dst.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
void WeUrl_UnescapeStringInPlace(WE_CHAR *pcDst,
                                                    const WE_CHAR *pcSrc)
{
    WE_CHAR cTmp;

    if((NULL == pcSrc) || (NULL == pcDst))
        return;

    while (*pcSrc)
    {
        if((*pcSrc == '%') && WE_SCL_HEX2BYTE(pcSrc + 1,(WE_UCHAR *)&cTmp))
        {
            *pcDst++ = cTmp;
            pcSrc += 3;
        }
        else
            *pcDst++ = *pcSrc++;
    }
    *pcDst = '\0';
}

/*=====================================================================================
FUNCTION: 
    WeUrl_DomainIsSuffix
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Check that the string 's1' is a componentwise suffix of string 's2'.
 * That is, if s1 = "def.com" and s2 = "abc.def.com", then return TRUE,
 * but if s1 = "c.def.com" return FALSE.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_DomainIsSuffix(const WE_CHAR *pcStr1,
                                                 WE_INT32 iStr1Len,
                                                 const WE_CHAR *pcStr2,
                                                 WE_INT32 iStr2Len)
{
    WE_INT32 iIndx1;
    WE_INT32 iIndx2;
    WE_INT32 iTmp1;
    WE_INT32 iTmp2;
    WE_INT32 iLen1;
    WE_INT32 iLen2;

    if((NULL == pcStr1) || (NULL == pcStr2))
        return FALSE;

    if(iStr1Len == 0)
        return TRUE;
    if(iStr2Len == 0)
        return FALSE;

    for(iTmp1 = iStr1Len,iTmp2 = iStr2Len;(iTmp1 > 0) && (iTmp2 > 0);)
    {
        /* Search backwards for next '.' */
        for(iIndx1 = iTmp1 - 1;(iIndx1 >= 0) && (pcStr1[iIndx1] != '.');iIndx1--);
        for(iIndx2 = iTmp2 - 1;(iIndx2 >= 0) && (pcStr2[iIndx2] != '.');iIndx2--);
        iLen1 = iTmp1 - (iIndx1 + 1);
        iLen2 = iTmp2 - (iIndx2 + 1);
        if(!WeUrl_CompareStrings(&pcStr1[iIndx1 + 1], iLen1,&pcStr2[iIndx2 + 1],iLen2,TRUE))
            return FALSE;
        iTmp1 = iIndx1;
        iTmp2 = iIndx2;
    }
    return (iTmp1 <= 0);
}

/*=====================================================================================
FUNCTION: 
    WeUrl_PathIsPrefix
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Check that the string 'a' is a componentwise prefix of string 'b'.
 * That is, if a = "/abc/def" and b = "/abc/def/jkl", then return TRUE,
 * but if a = "/abc/def/j" return FALSE.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_BOOL WeUrl_PathIsPrefix(const WE_CHAR *pcStr1,
                                            WE_INT32 iStr1Len,
                                            const WE_CHAR *pcStr2,
                                            WE_INT32 iStr2Len)
{
    WE_INT32 iIndx1 = 0;
    WE_INT32 iIndx2 = 0;
    WE_INT32 iTmp1 = 0;
    WE_INT32 iTmp2 = 0;
    WE_INT32 iLen1 = 0;
    WE_INT32 iLen2 = 0;

    if((NULL == pcStr1) || (NULL == pcStr2))
        return FALSE;

    /* Handle special cases first: */
    if(iStr1Len == 0)
        return TRUE;
    if(iStr2Len == 0)
        return FALSE;

    if((pcStr1[0] != '/') || (pcStr2[0] != '/'))
        return FALSE;

    /* General case: */
    for(iTmp1 = 0,iTmp2 = 0;(iTmp1 < iStr1Len) && (iTmp2 < iStr2Len);)
    {
        /* Search forward for next '/' */
        for(iIndx1 = iTmp1 + 1;(iIndx1 < iStr1Len) && (pcStr1[iIndx1] != '/');iIndx1++);
        for(iIndx2 = iTmp2 + 1;(iIndx2 < iStr2Len) && (pcStr2[iIndx2] != '/');iIndx2++);
        iLen1 = iIndx1 - (iTmp1 + 1);
        iLen2 = iIndx2 - (iTmp2 + 1);
        if((iLen1 > 0) && !(WeUrl_CompareStrings(&pcStr1[iTmp1 + 1],iLen1,&pcStr2[iTmp2 + 1],iLen2,FALSE)))
            return FALSE;
        iTmp1 = iIndx1;
        iTmp2 = iIndx2;
    }
    return (iTmp1 >= iStr1Len);
}

/*=====================================================================================
FUNCTION: 
    WeUrl_LongestCommonPrefix
CREATE DATE: 
    2006-5-23
AUTHOR: 
    Guohong
DESCRIPTION:
     * Return the longest componentwise common prefix of two URL paths.
 * Returns NULL in case of error.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
ARGUMENTS PASSED:
    
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    
USED STATIC VARIABLES:
    
CALL BY:
    
IMPORTANT NOTES:
    
=====================================================================================*/
WE_CHAR *WeUrl_LongestCommonPrefixEx(WE_CHAR *pcStr1,
                                   WE_CHAR *pcStr2,
                                   WE_HANDLE hMem,
								   WE_BOOL bMemMgr)
{
    WE_INT32 iStr1Len = 0;
    WE_INT32 iStr2Len = 0;
    WE_INT32 iLen1 = 0;
    WE_INT32 iLen2 = 0;
    WE_INT32 iIndx1 = 0;
    WE_INT32 iIndx2 = 0;
    WE_INT32 iTmp1 = 0;
    WE_INT32 iTmp2 = 0;
    WE_CHAR *pcTmp = NULL;

    if((NULL == pcStr1) || (NULL == pcStr2))
        return NULL;
    iStr1Len = WE_SCL_STRLEN(pcStr1);
    iStr2Len = WE_SCL_STRLEN(pcStr2);
    if((iStr1Len == 0) || (iStr2Len == 0))
        return NULL;
    if((pcStr1[0] != '/') || (pcStr2[0] != '/'))
        return NULL;

    for(iTmp1 = 0,iTmp2 = 0;(iTmp1 < iLen1) && (iTmp2 < iLen2);)
    {
        for(iIndx1 = iTmp1 + 1;(iIndx1 < iStr1Len) && (pcStr1[iIndx1] != '/'); iIndx1++);
        for(iIndx2 = iTmp2 + 1;(iIndx2 < iStr2Len) && (pcStr2[iIndx2] != '/'); iIndx2++);
        iLen1 = iIndx1 - (iTmp1 + 1);
        iLen2 = iIndx2 - (iTmp2 + 1);
        if(!WeUrl_CompareStrings(&pcStr1[iTmp1 + 1],iLen1,&pcStr2[iTmp2 + 1],iLen2,FALSE))
            break;
        iTmp1 = iIndx1;
        iTmp2 = iIndx2;
    }
    if(iTmp1 == 0)
        iTmp1 = 1;
    if(TRUE == bMemMgr)
		pcTmp = BrwMem_Alloc(hMem,iTmp1 + 1);
	else
		pcTmp = WE_MALLOC(iTmp1 + 1);

    if(NULL == pcTmp)
        return NULL;
    WE_MEMCPY(pcTmp,pcStr1,iTmp1);
    pcTmp[iTmp1] = '\0';

    return pcTmp;
}

/*=====================================================================================
FUNCTION: 
    WeUl_EscapeAllString
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
WE_CHAR *WeUrl_EscapeAllStringEx(const WE_CHAR *pcStr,WE_INT32 iStrLen,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    const WE_CHAR *pcIndx = NULL;
    WE_CHAR *pcTmp = NULL;
    WE_CHAR *pcResult = NULL;
    WE_INT32 iLen = 0;
    WE_INT32 iNbr = 0;
    WE_INT32 iIndx = 0;

    if((NULL == pcStr) || (0 == iStrLen))
        return NULL;

    for(pcIndx = pcStr;iIndx < iStrLen;iIndx++,pcIndx++)
    {
        if(!ct_isalphanum(*pcIndx))
            iNbr++;
        else
            iLen++;
    }
    if(TRUE == bMemMgr)
		pcResult = BrwMem_Alloc(hMem,iLen + 3*iNbr + 1);
	else
		pcResult = WE_MALLOC(iLen + 3*iNbr + 1);

    if(NULL == pcResult)
        return NULL;

    iIndx = 0;
    for(pcIndx = pcStr,pcTmp = pcResult;iIndx < iStrLen;pcIndx++,iIndx++)
    {
        if(ct_isspecial(*pcIndx) || (*pcIndx & 0x80))
        {
            *pcTmp++ = '%';
            WE_SCL_BYTE2HEX(*pcIndx,pcTmp);
            pcTmp += 2;
        }
        else
            *pcTmp++ = *pcIndx;
    }
    *pcTmp = '\0';
    return pcResult;
}



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
WE_CHAR *WeUrl_GetProtocolEx (const WE_CHAR *pcUrl,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
    WE_CHAR *pcProtocol = NULL;
	WE_CHAR *pcTmp = NULL;
    E_WEURL_SCHEME urlScheme;
    WE_INT32 iSchemeIndx = 0;
    WE_INT32 iLen = 0;
	St_WeUrl stUrl;

	if(NULL == pcUrl)
        return NULL;
    
    urlScheme = WeUrl_GetSchemeType(pcUrl);
    if(urlScheme == E_WE_SCHEME_EMPTY)
    {
        return NULL;
    }
	
    iSchemeIndx = (WE_INT32)urlScheme;
    if(iSchemeIndx > WE_LAST_SCHEME)
    {
        if(WeUrl_Parse(pcUrl,&stUrl))
		{
			iLen = stUrl.aiPartsLen[G_WE_URL_SCHEME_PART] + 1;
			pcTmp = stUrl.apcPartsStr[G_WE_URL_SCHEME_PART];
		}
		else
		{
			return NULL;
		}
    }
	else
	{
		iLen = WeUrl_GetSchemeLenInfo(iSchemeIndx) + 1;    
		pcTmp = (WE_CHAR*)WeUrl_GetSchemeStrInfo(iSchemeIndx);
	}	
		
	if(TRUE == bMemMgr)
	{
		pcProtocol = BrwMem_Alloc(hMem,iLen + 1);
		if(NULL == pcProtocol)
			return NULL;
		 TF3RD_MEMMOVE(pcProtocol,pcTmp,iLen);
		 TF3RD_MEMMOVE(pcProtocol + iLen - 1,":",TF3RD_STRLEN(":"));
	}
	else
	{
		pcProtocol = WE_MALLOC(iLen + 1);
		if(NULL == pcProtocol)
			return NULL;
		WE_MEMMOVE(pcProtocol,pcTmp,iLen);
		WE_MEMMOVE(pcProtocol + iLen - 1,":",1);
	}
	pcProtocol[iLen] = '\0';

    return pcProtocol;
}

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
							 WE_BOOL bMemMgr)
{
       St_WeUrl stUrl;
	WE_CHAR * pcNewUrl = NULL;

	if((NULL == pcUrl) || (!WeUrl_Parse(pcUrl,&stUrl)))
		return NULL;

	stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART] = NULL;
	stUrl.aiPartsLen[G_WE_URL_FRAGMENT_PART] = 0;

   /* ========= for js 10 - 24 ============*/
   if('#' == *pcFragment)
   {
      pcFragment++;
   }
   /* ==================================== */

	if(NULL != pcFragment)
	{
		stUrl.apcPartsStr[G_WE_URL_FRAGMENT_PART] = (WE_CHAR*)pcFragment;
	    stUrl.aiPartsLen[G_WE_URL_FRAGMENT_PART] = WE_SCL_STRLEN(pcFragment);
	}
	

	if(TRUE == bMemMgr)
		pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
	else
		pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);

	return pcNewUrl;

}


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
						 WE_BOOL bMemMgr)
{
       St_WeUrl stUrl;
	WE_CHAR * pcNewUrl = NULL;
	WE_CHAR * pcTmp = NULL;


	if((NULL == pcUrl) || (!WeUrl_Parse(pcUrl,&stUrl)))
		return NULL;

    stUrl.apcPartsStr[G_WE_URL_PATH_PART] = NULL;
	stUrl.aiPartsLen[G_WE_URL_PATH_PART] = 0;

	if(NULL != pcPath)
	{
		if(pcPath[0] != '/')
		{
			if(TRUE == bMemMgr)
				pcTmp = BrwMem_Alloc(hMem,TF3RD_STRLEN(pcPath) + 1);
			else
				pcTmp = WE_MALLOC(WE_SCL_STRLEN(pcPath) + 1);

			if(NULL == pcTmp)
				return NULL;
			WE_MEMMOVE(pcTmp,"/",1);
			WE_MEMMOVE(pcTmp + 1,pcPath,WE_SCL_STRLEN(pcPath));	
		     stUrl.apcPartsStr[G_WE_URL_PATH_PART] = (WE_CHAR*)pcTmp;
		     stUrl.aiPartsLen[G_WE_URL_PATH_PART] = WE_SCL_STRLEN(pcPath) + 1;
		}
		else
		{
		    stUrl.apcPartsStr[G_WE_URL_PATH_PART] = (WE_CHAR*)pcPath;
		    stUrl.aiPartsLen[G_WE_URL_PATH_PART] = WE_SCL_STRLEN(pcPath);
		}
		
	}	
	
	if(TRUE == bMemMgr)
	{
		pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
		BrwMem_Free(hMem, pcTmp);
	}
	else
	{
		pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
		WE_FREE(pcTmp);
	}

	return pcNewUrl;

}


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
						  WE_BOOL bMemMgr)
{
	St_WeUrl stUrl;
	WE_CHAR * pcNewUrl = NULL;
	
	if((NULL == pcUrl) || (!WeUrl_Parse(pcUrl,&stUrl)))
		return NULL;
	
	stUrl.apcPartsStr[G_WE_URL_QUERY_PART] = NULL;
	stUrl.aiPartsLen[G_WE_URL_QUERY_PART] = 0;

	if(NULL != pcQuery)
	{
		stUrl.apcPartsStr[G_WE_URL_QUERY_PART] = (WE_CHAR*)pcQuery;
		stUrl.aiPartsLen[G_WE_URL_QUERY_PART] = WE_SCL_STRLEN(pcQuery);
	}	
	
    if(TRUE == bMemMgr)
		pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
	else
		pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
	
	return pcNewUrl;

}


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
						  WE_BOOL bMemMgr)
{
	St_WeUrl stUrl;
	WE_CHAR * pcNewUrl = NULL;
	
	if((NULL == pcUrl) || (!WeUrl_Parse(pcUrl,&stUrl)))
		return NULL;
	
	stUrl.apcPartsStr[G_WE_URL_PORT_PART] = NULL;
	stUrl.aiPartsLen[G_WE_URL_PORT_PART] = 0;

	if(NULL != pcPort)
	{		
		stUrl.apcPartsStr[G_WE_URL_PORT_PART] = (WE_CHAR*)pcPort;
		stUrl.aiPartsLen[G_WE_URL_PORT_PART] = WE_SCL_STRLEN(pcPort);
	}	
	
	if(TRUE == bMemMgr)
		pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
	else
		pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
	
	return pcNewUrl;
	
}

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
						 WE_BOOL bMemMgr)
{
	St_WeUrl stUrl;
	WE_CHAR * pcNewUrl = NULL;
	
	if((NULL == pcUrl) || (!WeUrl_Parse(pcUrl,&stUrl)))
		return NULL;
	
	stUrl.apcPartsStr[G_WE_URL_HOST_PART] = NULL;
	stUrl.aiPartsLen[G_WE_URL_HOST_PART] = 0;
	
	if(NULL != pcHost)
	{		
		stUrl.apcPartsStr[G_WE_URL_HOST_PART] = (WE_CHAR*)pcHost;
		stUrl.aiPartsLen[G_WE_URL_HOST_PART] = WE_SCL_STRLEN(pcHost);
	}	
	
    if(TRUE == bMemMgr)
		pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
	else
		pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
	
	return pcNewUrl;
	
}


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
WE_CHAR * WeUrl_SetHostPortEx(	const WE_CHAR *pcUrl,
								const WE_CHAR *pcHostPort,
								WE_HANDLE hMem,
								WE_BOOL bMemMgr)
{
	St_WeUrl stUrl;
	WE_CHAR * pcNewUrl = NULL;
	WE_INT32 iIndx = 0;
	WE_INT32 iLen = 0;
	
	if((NULL == pcUrl) || (!WeUrl_Parse(pcUrl,&stUrl)))
		return NULL;

    iLen = WE_SCL_STRLEN(pcHostPort);
	stUrl.apcPartsStr[G_WE_URL_HOST_PART] = NULL;
	stUrl.aiPartsLen[G_WE_URL_HOST_PART] = 0;
	stUrl.apcPartsStr[G_WE_URL_PORT_PART] = NULL;
	stUrl.aiPartsLen[G_WE_URL_PORT_PART] = 0;

	if(NULL != pcHostPort)
	{
		for(iIndx = 0;iIndx < iLen;iIndx++)
		{
			if(pcHostPort[iIndx] == ':')
				break;
		}
		if(iIndx == iLen)
		{
			stUrl.apcPartsStr[G_WE_URL_HOST_PART] = (WE_CHAR*)pcHostPort;
			stUrl.aiPartsLen[G_WE_URL_HOST_PART] = iIndx;
			stUrl.apcPartsStr[G_WE_URL_PORT_PART] = NULL;
			stUrl.aiPartsLen[G_WE_URL_PORT_PART] = 0;
		}
		else
		{
			stUrl.apcPartsStr[G_WE_URL_HOST_PART] = (WE_CHAR*)pcHostPort;
			stUrl.aiPartsLen[G_WE_URL_HOST_PART] = iIndx;
			stUrl.apcPartsStr[G_WE_URL_PORT_PART] = (WE_CHAR*)pcHostPort + iIndx + 1;
			stUrl.aiPartsLen[G_WE_URL_PORT_PART] = iLen - (iIndx + 1);
		}
		
	}
	if(TRUE == bMemMgr)
		pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
	else
		pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
	
	return pcNewUrl;
	
}

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
							 WE_BOOL bMemMgr)
{
	St_WeUrl stUrl;
	WE_CHAR * pcNewUrl = NULL;

	if((NULL == pcUrl) || (!WeUrl_Parse(pcUrl,&stUrl)))
		return NULL;
	
	stUrl.apcPartsStr[G_WE_URL_SCHEME_PART] = NULL;
	stUrl.aiPartsLen[G_WE_URL_SCHEME_PART] = 0;
	
	if(NULL != pcProtocol)
	{		
		stUrl.apcPartsStr[G_WE_URL_SCHEME_PART] = (WE_CHAR*)pcProtocol;
		stUrl.aiPartsLen[G_WE_URL_SCHEME_PART] = WE_SCL_STRLEN(pcProtocol) - 1;
	}	
	
    if(TRUE == bMemMgr)
		pcNewUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
	else
		pcNewUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);
	
	return pcNewUrl;
	
}





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
					  WE_BOOL bMemMgr)
{
	St_WeUrl stUrl;
	WE_INT bResult = 0;

	if(NULL == pcUrl)
	{
        bResult = -2;
	    return bResult;
	}

	if(WeUrl_Parse(pcUrl, &stUrl))
	{
	    if(stUrl.aiPartsLen[G_WE_URL_PATH_PART] == 0)
		{
			stUrl.apcPartsStr[G_WE_URL_PATH_PART] = "/";
            stUrl.aiPartsLen[G_WE_URL_PATH_PART] = 1;	
			if(TRUE == bMemMgr)
				*ppcUrl = WeUrl_AssembleEx(&stUrl,hMem,TRUE);
			else
				*ppcUrl = WeUrl_AssembleEx(&stUrl,NULL,FALSE);

			if(NULL == *ppcUrl)
		        bResult = -3;
			else
				bResult = 0;		    
		}
		else
			bResult = 1;
			
	}
	else
		bResult = 3;

	return bResult;
}



/*=====================================================================================
FUNCTION: 
WeUrl_GetSerch
CREATE DATE: 
2007-10-23
AUTHOR: 
DaiGuangsen
DESCRIPTION:

  ARGUMENTS PASSED:
  
    RETURN VALUE:
    
      USED GLOBAL VARIABLES:
      
        USED STATIC VARIABLES:
        
          CALL BY:
          
            IMPORTANT NOTES:
            
=====================================================================================*/
WE_CHAR * WeUrl_GetSerchEx (const WE_CHAR *pcUrl,WE_HANDLE hMem,WE_BOOL bMemMgr)
{
   WE_CHAR *pcSerch = NULL;
   WE_CHAR *pcQuery = NULL;
   
   if(TRUE == bMemMgr)
   {
      pcQuery = (WE_CHAR *)WeUrl_GetQueryEx(pcUrl,hMem,TRUE);
      if(NULL != pcQuery)
      {
         pcSerch = (WE_CHAR *)Brw_StrCat(hMem,"?",pcQuery);
         BrwMem_Free(hMem,pcQuery);
      }
   }
   else
   {
      pcQuery = WeUrl_GetQueryEx(pcUrl,NULL,FALSE);
      if(NULL != pcQuery)
      {
         pcSerch = WE_STRCAT("?",pcQuery);
         WE_FREE(pcQuery);
      }
   }
   return pcSerch;
}

















