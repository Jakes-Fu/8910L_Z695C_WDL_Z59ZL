/*=====================================================================================
FILE NAME   : eme_emn.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Decode the EMN body for push email. 
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-04-20        Hu bin                      create eme_emn.c
    2007-04-25        Wang fengtu                 modify eme_emn.c
=====================================================================================*/
#include "eme_emn_api.h"
#include "cmmn_stream.h"
#include "we_pm.h"
#include "eme_imap_utils.h"

/**************************** micro define ***********************************/
typedef WE_VOID*  IEmnHandle;
typedef WE_VOID*  HEmnSem;

#define TEST_USE  1

/* temp length of temp stamp */
#define EME_EMN_TIMESTAMPLEN                       30

/* Table size */
#define EME_EMN_PUBLIC_LITERAL_TAB_SIZE            2
#define EME_EMN_ELEMENT_TAB_SIZE                   2
#define EME_EMN_ATTR_TAB_SIZE                      2
#define EME_EMN_ATTRSTART_TAB_SIZE                 7
#define EME_EMN_ATTRVALVE_TAB_SIZE                 7
#define EME_EMN_CONTENT_TAB_SIZE                   2
#define EME_EMN_CHARSET_TAB_SIZE                   4
#define EME_EMN_Attrib_Num                         59
#define EME_EMN_Tag_Num                            26

/* XML related */
#define EME_EMN_DOCTYPE                            0x16
#define EME_EMN_ELEMENT                            0x05
#define EME_EMN_ATTRIBUTE_TIME                     0
#define EME_EMN_ATTRIBUTE_MAILBOX                  1

/* CONTENT TYPE */
#define EME_EMN_CONTENTTEXT_EMN                     1
#define EME_EMN_CONTENTBINARY_EMN                   2
#define EME_EMN_CONTENT_DEFAULT_EMN                 M_CONTENTBINARY_EMN
#define EME_EMN_NOCONTENTTYPE_EMN                   0xFF

/* CHARSET */
#define EME_EMN_CHARSETUTF8_EMN                     WE_CSC_UTF_8
#define EME_EMN_CHARSETUTF16_EMN                    WE_CSC_UTF_16
#define EME_EMN_CHARSET_DEFAULT_EMN                 WE_CSC_UTF_8
#define EME_EMN_NOTSUPPORTED_EMN                    (0xFF+1)

/* url to mailbox */
#define UTF16HIGHSTART  0xD800UL
#define UTF16LOSTART    0xDC00UL
#define UTF16MASK       0x03FFUL
#define UTF16BASE       0x10000UL
#define UTF16SHIFT      10
const static WE_CHAR acHex[17] = "0123456789ABCDEF";
const static WE_CHAR acBase64Chars[65] = 
   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";


/**************************** const table define ***********************************/
/* XML related table */
const static WE_CHAR acPublicLiteralStr[EME_EMN_PUBLIC_LITERAL_TAB_SIZE][25] =
{
    "-//OMA//DTD EMN 1.0//EN",
    "-//OMA//DTD EMN 1.0//CH"
};

const static WE_INT32 aiPublicLiteralStr[EME_EMN_PUBLIC_LITERAL_TAB_SIZE] =
{
    EME_EMN_DOCTYPE,
    EME_EMN_DOCTYPE+1
};

const static WE_UCHAR aucEmnPublicHashTable[3] = 
{
    255, 1, 0
};

const static WE_CHAR acEmnEleStr[EME_EMN_ELEMENT_TAB_SIZE][10] =
{
    "emn",
    "emn_dummy"         
};

const static WE_INT32 aiEmnEleStr[EME_EMN_ELEMENT_TAB_SIZE] =
{
    EME_EMN_ELEMENT,
    EME_EMN_ELEMENT+1    
};

const static WE_UCHAR aucEmnEleHashTable[3] = 
{
    255, 1, 0
};

const static WE_CHAR acEmnAttrStr[EME_EMN_ATTR_TAB_SIZE][10] =
{
    "timestamp" ,    
    "mailbox",    
};

const static WE_INT32 aiEmnAttrStr[EME_EMN_ATTR_TAB_SIZE] =
{
    EME_EMN_ATTRIBUTE_TIME,
    EME_EMN_ATTRIBUTE_MAILBOX        
};

const static WE_UCHAR aucEmnAttrHashTable[3] = 
{
    0, 1, 255
};

const static WE_CHAR acEmnAttrStartStr[EME_EMN_ATTRSTART_TAB_SIZE][13] = 
{
    "mailat:"      ,    
    "pop://"       ,
    "imap://"      ,   
    "http://"      ,    
    "http://www."  ,
    "https://"     ,   
    "https://www." 
};

const static WE_INT32 aiEmnAttrStartStr[EME_EMN_ATTRSTART_TAB_SIZE] =
{
    0x07, 
    0x08, 
    0x09, 
    0x0a,
    0x0b,
    0x0c,
    0x0d
};

const static WE_CHAR acAttrValStr[EME_EMN_ATTRVALVE_TAB_SIZE][6] = 
{
    ".com/"    ,
    ".edu/"    ,
    ".net/"    ,
    ".org/"    
};

const static WE_INT32 aiAttrValStr[EME_EMN_ATTRVALVE_TAB_SIZE] = 
{
    0x85,
    0x86,
    0x87,
    0x88
};

const static WE_UINT8 aucCoAttrTransform[14] = 
{
    G_WE_PM_NOT_USED,
    G_WE_PM_NOT_USED,
    G_WE_PM_NOT_USED,
    G_WE_PM_NOT_USED,
    G_WE_PM_NOT_USED,
    EME_EMN_ATTRIBUTE_TIME,
    EME_EMN_ATTRIBUTE_MAILBOX,                         
    G_WE_PM_DEC_DATA | EME_EMN_ATTRIBUTE_MAILBOX,                        
    G_WE_PM_DEC_DATA | EME_EMN_ATTRIBUTE_MAILBOX,  
    G_WE_PM_DEC_DATA | EME_EMN_ATTRIBUTE_MAILBOX, 
    G_WE_PM_DEC_DATA | EME_EMN_ATTRIBUTE_MAILBOX,  
    G_WE_PM_DEC_DATA | EME_EMN_ATTRIBUTE_MAILBOX,      
    G_WE_PM_DEC_DATA | EME_EMN_ATTRIBUTE_MAILBOX,  
    G_WE_PM_DEC_DATA | EME_EMN_ATTRIBUTE_MAILBOX                            
};

const static WE_UCHAR aucEmnElementTable[7]= 
{                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,        
    G_WE_PM_CANCELSPACE|G_WE_PM_TEXT, 
    G_WE_PM_CANCELSPACE|G_WE_PM_TEXT                                     
};

const static WE_INT32 aiEmnAttrtypeStr[4] =
{
    G_WE_PM_TYPE_CDATA,
    G_WE_PM_TYPE_CDATA,
    0,
    0
};

/* content table */
const static WE_CHAR acContentStr[EME_EMN_CONTENT_TAB_SIZE][30] =
{
    "text/vnd.wap.emn+xml",
    "application/vnd.wap.emn+wbxml"
};

const static WE_INT32 aiContentStr[EME_EMN_CONTENT_TAB_SIZE] =
{
    EME_EMN_CONTENTTEXT_EMN,
    EME_EMN_CONTENTBINARY_EMN
};

/* charset table */
const static WE_CHAR acCharsetStr[EME_EMN_CHARSET_TAB_SIZE][7] =
{
    "utf-8",
    "utf-16",
    "UTF-8",
    "UTF-16"
};

const static WE_INT32 aiCharsetStr[EME_EMN_CHARSET_TAB_SIZE] =
{
    EME_EMN_CHARSETUTF8_EMN,
    EME_EMN_CHARSETUTF16_EMN,
    EME_EMN_CHARSETUTF8_EMN,
    EME_EMN_CHARSETUTF16_EMN
};

/**************************** STRUCTURE DEFINE ***********************************/
typedef struct tagSt_SmilTable
{
   WE_CHAR                *pacSmilAttribList[EME_EMN_Attrib_Num];
   WE_CHAR                *pacSmilTagList[EME_EMN_Tag_Num];

   St_WeSCLStrtableInfo   stSmilElem;
   St_WeSCLStrtableInfo   stSmilAttrib;
   St_WeSCLStrtableInfo   stSmilAttribVals;

   WE_UCHAR               *pucSmilElemType;
   WE_UINT16              *pusSmilAttribType;

}St_SmilTable, *P_St_SmilTable;

typedef struct  tagSt_EmnlParseInfo
{
   IEmnHandle           phEmnHandle;
   WE_INT32             iParseErr;
} St_EmnParseInfo;

typedef struct tagSt_EMNPushGlobal
{
    St_WeSCLStrtableInfo         *pstEmnElements;        /*  emn elements string table.   */
    St_WeSCLStrtableInfo         *pstEmnAttributes;      /*  emn attributes string table. */
    St_WeSCLStrtableInfo         *pstEmnAttrVals;        /*  emn attributes value string table. */
    St_WeSCLStrtableInfo         *pstEmnAttrStart;       /*  emn attributes start value string table. */
    
    WE_UCHAR                     *pucEmnElementTable;    /*  emn element table.           */
    WE_UINT16                    *pusEmnAttributeType;   /*  emn attribute value type.    */
    uint8                        *pucEmnAttrTransform;   /*  emn attribute transform.     */
    St_WeSCLStrtableInfo         *pstAttrValue;          /*  Attribute value    */
    St_WeSCLStrtableInfo         *pstPublicLiteral;      /*  Public literal     */
}St_EMNPushGlobal;

typedef struct tagSt_Emn_MUA 
{
   St_EmnParseInfo   stParseInfo;
   St_EMNPushGlobal  *pstGlobe;
   WE_CHAR           *pcData;
   St_PMStreamParam  *pstStream;
   WE_BOOL           bTextMode;
   WE_CHAR           acRes[3];
} St_Emn_MUA;

typedef struct
{
   WE_VOID        *pvCSC;
   WE_VOID        *pvMemMgr;
   WE_VOID        *pvIShell;
   St_Emn_MUA     stEmn;
   St_EmnRet      stEmnRet;
}St_EMNHandle;

/**************************** Functions declare ***********************************/
/* decode XML body for getting mailbox and timestamp. they can be used for later decode */
static WE_INT32 EME_EMN_InitBodyTable(WE_VOID* pvMemMgr, WE_VOID * pEmnParser);
static WE_INT32 EME_EMN_ParserBody(IEMNHandle hEMNHandle, WE_CHAR * pcBody,WE_INT32 iContentType,WE_INT32 iCharsetType,WE_UINT32 uiDataLen);
static WE_UINT32 EME_EMN_NextPrimeNubmer(uint32 uiCurrent);
static WE_UCHAR *EME_EMN_CreateHashTable(St_WeSCLStrTableEntry *pstStringTable,uint32 uiStringTableSize,uint32 uiMaxImpactTime,uint32 *puiHashTableSize);
static WePMParserHandle  EME_EMN_CreateEmnParser(IEMNHandle hEMNHandle, WE_UINT32 uiContentType,WE_INT32 iCharset,WE_UINT32 uiDataLen,WE_BOOL bTextMode);
static WE_INT32 EME_EMN_DeleteEmnParser(WE_VOID* pvMemMgr, WePMParserHandle phParser);
static WE_INT32 EME_EMN_ParseCallBack(WePMParserHandle  hParserData, WePMDecoderHandle hDecoderData, const WE_CHAR *pcszDocType, WE_INT32 iLen, WE_INT32 iDocType, WE_INT32 iCharset);
static WE_INT32 EME_EMN_StartElement(WE_VOID *pvUserData,WE_INT32 iElementType,St_WePMAttr *pstAttribute,WE_INT32  iAttributeSize);
static WE_INT32 EME_EMN_EndElement(WE_VOID* pvUserData);
static WE_INT32 EME_EMN_CharacterData(WE_VOID* pvUserData, const WE_CHAR* pcText, WE_INT32  iTextLen, WE_INT32 iMode);
static WE_VOID  EME_EMN_URLtoMailbox(char *dst, char *src);


/* sem functions, for decode detail of mailbox and timestamp attr */
static WE_INT32 EME_EMN_DecodeEmnSem(IEMNHandle hEMNHandle, St_EmnSemIn* pstEmnSemIn);
static WE_INT32 EME_EMN_DecodeISERVER(IEMNHandle hEMNHandle, WE_CHAR* pcURI, St_EmnRet* pstEmnSemRet);
static WE_INT32 EME_EMN_DecodeMailbox(IEMNHandle hEMNHandle, WE_CHAR* pcStrMailbox, St_EmnRet* pstEmnSemRet);
static WE_INT32 EME_EMN_DecodeURIMailAt(IEMNHandle hEMNHandle, WE_CHAR* pcURI, St_EmnRet* pstEmnSemRet);
static WE_INT32 EME_EMN_DecodeURIPOP(IEMNHandle hEMNHandle, WE_CHAR* pcURI, St_EmnRet* pstEmnSemRet);
static WE_INT32 EME_EMN_DecodeURIIMAP(IEMNHandle hEMNHandle, WE_CHAR* pcURI, St_EmnRet* pstEmnSemRet);
static WE_INT32 EME_EMN_DecodeTimeStamp(IEMNHandle hEMNHandle, WE_CHAR* pcTimeStamp, WE_BOOL bTextMode, St_EmnRet* pstEmnSemRet);
static St_EmnServer* EME_EMN_GetIServer(IEMNHandle hEMNHandle);
static WE_INT32 EME_EMN_ResetRet(WE_VOID*  pvMemMgr, St_EmnRet* pstEmnSemRet);
static WE_INT32 EME_EMN_ResetIServer(WE_VOID* pvMemMgr, St_EmnServer* pstIServer);


WE_INT32 EME_EMN_InitHandle(IEMNHandle* hEMNHandle, WE_VOID* pvIShell, WE_VOID* pvMemMgr, WE_VOID* pvCSC)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_EMNHandle* pstEMN = NULL;
   if (NULL == hEMNHandle)
   {
      return EME_FAILED;
   }
   do 
   {
      pstEMN = *hEMNHandle = EME_UTILS_MALLOC(pvMemMgr, sizeof(St_EMNHandle));
      EME_UTILS_BREAKNULL(*hEMNHandle, iRet, EME_NOMEMORY, "init emn handle failed");
      EME_UTILS_MEMSET(pstEMN, 0, sizeof(St_EMNHandle));
      pstEMN->pvMemMgr = pvMemMgr;
      pstEMN->pvCSC = pvCSC;
      pstEMN->pvIShell = pvIShell;
   } while(0);
   
   return iRet;
}


WE_INT32 EME_EMN_DecodeEMN(IEMNHandle hEMNHandle, 
                           WE_VOID* pvBody,
                           WE_INT32 iContentType,
                           WE_INT32 iCharsetType,
                           WE_UINT32 uiDataLen,
                           St_EmnRet** ppRet)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_EMNHandle* pstEMNHandle = (St_EMNHandle*)hEMNHandle;
   if (NULL == hEMNHandle || NULL == pvBody || NULL == ppRet)
   {
      return EME_FAILED;
   }
   
   do 
   {
      iRet = EME_EMN_InitBodyTable(pstEMNHandle->pvMemMgr, &pstEMNHandle->stEmn);
      EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeEMN EME_EMN_InitBodyTable failed");

      iRet = EME_EMN_ParserBody(hEMNHandle, pvBody, iContentType, iCharsetType,uiDataLen);
      EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeEMN EME_EMN_ParserBody failed");
      
      *ppRet = &pstEMNHandle->stEmnRet;
   } while(0);

   return iRet;
}



WE_INT32 EME_EMN_DeleteHandle(IEMNHandle* hEMNHandle)
{
   WE_INT32 iRet = EME_SUCCESS;
   St_EMNHandle* pstEMNHandle = NULL;
   if (NULL == hEMNHandle)
   {
      return EME_FAILED;
   }

   pstEMNHandle = (St_EMNHandle*)(*hEMNHandle);
   /*add by jade for modify clocwork 20071108*/
   if(pstEMNHandle == NULL)
   {
      return EME_FAILED;
   }

   do 
   {
      EME_EMN_ResetRet(pstEMNHandle->pvMemMgr, &pstEMNHandle->stEmnRet);   

      EME_UTILS_FREEIF(pstEMNHandle->pvMemMgr, pstEMNHandle);
   } while(0);

   return iRet;
}


static WE_INT32 EME_EMN_InitBodyTable(WE_VOID *pvMemMgr, WE_VOID *pEmnParser)
{
   St_Emn_MUA * pMe = NULL;
   St_WeSCLStrtableInfo * pstInfo = NULL;
   St_EMNPushGlobal * pstTmp = NULL;
   WE_INT32  iResult  = EME_SUCCESS;
   WE_INT32 i = 0;

   if (NULL == pEmnParser)
      return EME_FAILED;

   do 
   {
      pMe = (St_Emn_MUA *)pEmnParser;

      /////////////////Globe Malloc////////////////
      if(NULL == pMe->pstGlobe)  
         pMe->pstGlobe = (St_EMNPushGlobal*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_EMNPushGlobal));
      EME_UTILS_BREAKNULL(pMe->pstGlobe, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");

      MEMSET(pMe->pstGlobe,0,sizeof(St_EMNPushGlobal));
      pstTmp = pMe->pstGlobe;
      pMe->pstGlobe->pucEmnElementTable  = (WE_UCHAR *)aucEmnElementTable;
      pMe->pstGlobe->pusEmnAttributeType = (uint16 *)aiEmnAttrtypeStr;
      pMe->pstGlobe->pucEmnAttrTransform = (uint8 *)aucCoAttrTransform;

      /////////////////Element Malloc/////////////////
      pstTmp->pstEmnElements = (St_WeSCLStrtableInfo*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrtableInfo));
      EME_UTILS_BREAKNULL(pstTmp->pstEmnElements, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstTmp->pstEmnElements , 0, sizeof(St_WeSCLStrtableInfo));
      
      pstInfo = pstTmp->pstEmnElements;
      pstInfo->pstStringTable = \
         (St_WeSCLStrTableEntry*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrTableEntry) * EME_EMN_ELEMENT_TAB_SIZE);
      EME_UTILS_BREAKNULL(pstInfo->pstStringTable, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstInfo->pstStringTable, 0, sizeof(St_WeSCLStrTableEntry) * EME_EMN_ELEMENT_TAB_SIZE);
      
      pstInfo->iStringTableSize = EME_EMN_ELEMENT_TAB_SIZE;
      for (i = 0; i < EME_EMN_ELEMENT_TAB_SIZE; i++)
      {
         pstInfo->pstStringTable[i].pcName = (WE_CHAR*)acEmnEleStr[i];
         pstInfo->pstStringTable[i].iValue = aiEmnEleStr[i];
      }
      pstInfo->pucHashTable = EME_EMN_CreateHashTable(pstInfo->pstStringTable,pstInfo->iStringTableSize,4,(WE_UINT32*)&pstInfo->iHashTableSize);
      pstInfo->sSearchMethod = WE_TABLE_SEARCH_BINARY;

      /////////////////Attributes string Malloc/////////////////
      pstTmp->pstEmnAttributes = (St_WeSCLStrtableInfo*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrtableInfo));
      EME_UTILS_BREAKNULL(pstTmp->pstEmnAttributes, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstTmp->pstEmnAttributes , 0, sizeof(St_WeSCLStrtableInfo));
      
      pstInfo = pstTmp->pstEmnAttributes;
      pstInfo->pstStringTable = \
         (St_WeSCLStrTableEntry*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrTableEntry) * EME_EMN_ATTRSTART_TAB_SIZE);
      EME_UTILS_BREAKNULL(pstInfo->pstStringTable, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstInfo->pstStringTable, 0, sizeof(St_WeSCLStrTableEntry) * EME_EMN_ATTRSTART_TAB_SIZE);
      
      pstInfo->iStringTableSize = EME_EMN_ATTRSTART_TAB_SIZE;
      for (i = 0; i < EME_EMN_ATTRSTART_TAB_SIZE; i++)
      {
         pstInfo->pstStringTable[i].pcName = (WE_CHAR*)acEmnAttrStr[i];
         pstInfo->pstStringTable[i].iValue = aiEmnAttrStr[i];
      }      
      pstInfo->pucHashTable = EME_EMN_CreateHashTable(pstInfo->pstStringTable,pstInfo->iStringTableSize,4,(WE_UINT32*)&pstInfo->iHashTableSize);
      pstInfo->sSearchMethod = WE_TABLE_SEARCH_LINEAR;

      /////////////////Attributes start Malloc/////////////////
      pstTmp->pstEmnAttrStart = (St_WeSCLStrtableInfo*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrtableInfo));
      EME_UTILS_BREAKNULL(pstTmp->pstEmnAttrStart, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstTmp->pstEmnAttrStart , 0, sizeof(St_WeSCLStrtableInfo));

      pstInfo = pstTmp->pstEmnAttrStart;
      pstInfo->pstStringTable = \
         (St_WeSCLStrTableEntry*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrTableEntry) * EME_EMN_ATTRSTART_TAB_SIZE);
      EME_UTILS_BREAKNULL(pstInfo->pstStringTable, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstInfo->pstStringTable, 0, sizeof(St_WeSCLStrTableEntry) * EME_EMN_ATTRSTART_TAB_SIZE);
      
      pstInfo->iStringTableSize = EME_EMN_ATTRSTART_TAB_SIZE;
      for (i = 0; i < EME_EMN_ATTRSTART_TAB_SIZE; i++)
      {
         pstInfo->pstStringTable[i].pcName = (WE_CHAR*)acEmnAttrStartStr[i];
         pstInfo->pstStringTable[i].iValue = aiEmnAttrStartStr[i];
      }
      pstInfo->pucHashTable = EME_EMN_CreateHashTable(pstInfo->pstStringTable,pstInfo->iStringTableSize,4,(WE_UINT32*)&pstInfo->iHashTableSize);
      pstInfo->sSearchMethod = WE_TABLE_SEARCH_BINARY;

      /////////////////Attribute value Malloc/////////////////
      pstTmp->pstAttrValue = (St_WeSCLStrtableInfo*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrtableInfo));
      EME_UTILS_BREAKNULL(pstTmp->pstAttrValue, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstTmp->pstAttrValue , 0, sizeof(St_WeSCLStrtableInfo));
      
      pstInfo = pstTmp->pstAttrValue;
      pstInfo->pstStringTable = \
         (St_WeSCLStrTableEntry*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrTableEntry) * EME_EMN_ATTRVALVE_TAB_SIZE);
      EME_UTILS_BREAKNULL(pstInfo->pstStringTable, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstInfo->pstStringTable, 0, sizeof(St_WeSCLStrTableEntry) * EME_EMN_ATTRVALVE_TAB_SIZE);

      pstInfo->iStringTableSize = EME_EMN_ATTRVALVE_TAB_SIZE;
      for (i = 0; i < EME_EMN_ATTRVALVE_TAB_SIZE; i++)
      {
         pstInfo->pstStringTable[i].pcName = (WE_CHAR*)acAttrValStr[i];
         pstInfo->pstStringTable[i].iValue = aiAttrValStr[i];
      }
      pstInfo->pucHashTable = NULL;
      pstInfo->sSearchMethod = WE_TABLE_SEARCH_BINARY;

      /////////////////pstPublicLiteral Malloc/////////////////
      pstTmp->pstPublicLiteral = (St_WeSCLStrtableInfo*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrtableInfo));
      EME_UTILS_BREAKNULL(pstTmp->pstPublicLiteral, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstTmp->pstPublicLiteral , 0, sizeof(St_WeSCLStrtableInfo));

      pstInfo = pstTmp->pstPublicLiteral;
      pstInfo->pstStringTable = \
         (St_WeSCLStrTableEntry*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_WeSCLStrTableEntry) * EME_EMN_PUBLIC_LITERAL_TAB_SIZE);
      EME_UTILS_BREAKNULL(pstInfo->pstStringTable, iResult, EME_NOMEMORY, "EME_EMN_InitBodyTable malloc failed");
      MEMSET(pstInfo->pstStringTable, 0, sizeof(St_WeSCLStrTableEntry) * EME_EMN_PUBLIC_LITERAL_TAB_SIZE);

      pstInfo->iStringTableSize = EME_EMN_PUBLIC_LITERAL_TAB_SIZE;
      for (i = 0; i < EME_EMN_PUBLIC_LITERAL_TAB_SIZE; i++)
      {
         pstInfo->pstStringTable[i].pcName = (WE_CHAR*)acPublicLiteralStr[i];
         pstInfo->pstStringTable[i].iValue = aiPublicLiteralStr[i];
      }
      pstInfo->pucHashTable = EME_EMN_CreateHashTable(pstInfo->pstStringTable,pstInfo->iStringTableSize,4,(WE_UINT32*)&pstInfo->iHashTableSize);
      pstInfo->sSearchMethod = WE_TABLE_SEARCH_LINEAR;
      /////////////////Malloc End/////////////////
   } while(0);

   if (EME_SUCCESS != iResult)
   {
      if (NULL != pMe->pstGlobe)
      {
         if (NULL != pMe->pstGlobe->pstEmnElements)
         {
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnElements->pstStringTable);
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnElements);
         }

         if (NULL != pMe->pstGlobe->pstEmnAttributes)
         {
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttributes->pstStringTable);
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttributes);
         }

         if (NULL != pMe->pstGlobe->pstEmnAttrStart)
         {
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttrStart->pstStringTable);
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttrStart);
         }

         if (NULL != pMe->pstGlobe->pstAttrValue)
         {
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstAttrValue->pstStringTable);
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstAttrValue);
         }

         if (NULL != pMe->pstGlobe->pstPublicLiteral)
         {
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstPublicLiteral->pstStringTable);
            EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstPublicLiteral);
         }
      }
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe);
   }

   return iResult;
}

static WE_INT32 EME_EMN_ParserBody(IEMNHandle hEMNHandle, WE_CHAR * pcBody,WE_INT32 iContentType,WE_INT32 iCharsetType,WE_UINT32 uiDataLen)
{
   WE_INT32    iResult = EME_FAILED;
   St_Emn_MUA * pMe = NULL;
   St_EMNHandle *pstEMNHandle = NULL;
   WePMParserHandle     phParser = NULL;
   WE_VOID* pvMemMgr = NULL;
   WE_BOOL bTextMode = FALSE;
   if (NULL == hEMNHandle)
      return EME_FAILED;

   pstEMNHandle = (St_EMNHandle *)hEMNHandle;
   pMe = (St_Emn_MUA *)(&pstEMNHandle->stEmn);
   pvMemMgr = pstEMNHandle->pvMemMgr;

   do 
   {  
      pMe->pcData = EME_UTILS_MALLOC(pvMemMgr, uiDataLen+1);
      EME_UTILS_BREAKNULL(pMe->pcData, iResult, ENOMEMORY, "EME_EMN_ParserBody malloc failed");
      MEMSET(pMe->pcData,0,uiDataLen+1);
      MEMCPY(pMe->pcData,pcBody,uiDataLen);
      
      if(EME_EMN_CONTENTTEXT_EMN == iContentType) 
         bTextMode = TRUE;
      else
         bTextMode = FALSE;
      pMe->bTextMode = bTextMode;
      phParser = EME_EMN_CreateEmnParser(pstEMNHandle,0,iCharsetType,uiDataLen,bTextMode);
      EME_UTILS_BREAKNULL(phParser, iResult, EME_FAILED, "EME_EMN_ParserBody EME_EMN_CreateEmnParser failed");

      iResult = WePM_Parser(phParser);
      if(E_PM_PARSING_COMPLETED != (E_PMErr)iResult)
      {
         iResult = EME_FAILED;
         break;
      }
      else
      {
         iResult = EME_SUCCESS;
      }
   } while(0);

   if(NULL != phParser)
      EME_EMN_DeleteEmnParser(pvMemMgr, phParser);
   return iResult;
}


static WE_UINT32 EME_EMN_NextPrimeNubmer(uint32 uiCurrent)
{
   WE_UINT32   i = 0;

   uiCurrent++;

   if ( uiCurrent <= 2 ) 
   {
      return 2;
   }

   while (1)
   {

      for ( i = 2; i < uiCurrent; i++ ) 
      {
         if ( (uiCurrent % i) == 0 ) 
         {
            break;
         }
      }
      if ( i == uiCurrent ) 
      {
         return uiCurrent;
      }
      uiCurrent++;
   }
}

static WE_UCHAR *CommonEmn_MallocHash(WE_INT32 iStringTableSize, WE_INT32 *piHashSize)
{
   WE_INT32          iHashSize     = 0;
   WE_UCHAR      *pucHashMemory = NULL;

   iHashSize = EME_EMN_NextPrimeNubmer(iStringTableSize);

   if(iHashSize < iStringTableSize)
   {
      return NULL;
   }

   /*malloc memory for hash table*/
   pucHashMemory = (WE_UCHAR*)MALLOC(iHashSize * sizeof(WE_UCHAR));

   if (pucHashMemory == NULL)
   {
      return NULL;
   }

   MEMSET(pucHashMemory,0xFF, iHashSize);

   *piHashSize  = iHashSize;

   return pucHashMemory;
}

/*=====================================================================================
FUNCTION: 
CreatHashTable

DESCRIPTION:
Create a hash tabe by String table.
ARGUMENTS PASSED:
pstStringTable: Point to the string table.
iHashTableSize: Length of hash table want to be created
iStringTableSize Length of string table
RETURN VALUE:
Returns NULL if Create hash table failed, otherwise returns the pointer to hash table.
USED GLOBAL VARIABLES:
None
USED STATIC VARIABLES:
None
CALL BY:
All program need create a hash table.
IMPORTANT NOTES:
The CreatHashTable function calls MALLOC to allocate storage space for create table
Because CreatHashTable calls MALLOC to allocate storage space for create table,
it is good practice always to release this memory by calling the 
FREE routine on the pointer returned by the call to WeSCL_Strndup.      


=====================================================================================*/
static WE_UCHAR *EME_EMN_CreateHashTable(St_WeSCLStrTableEntry *pstStringTable,WE_UINT32 uiStringTableSize,WE_UINT32 uiMaxImpactTime,WE_UINT32 *puiHashTableSize)
{
   WE_UCHAR                  *pucHashTable = NULL;
   WE_UINT32                     uiStrHas = 0;
   WE_UINT32                     uiDx = 0;
   WE_UINT32                     uiStep = 0;
   //    uint32                     uiTidx = 0;
   WE_UINT32                     uiHashSize = 0;
   St_WeSCLStrTableEntry     *pstStrTab = NULL;

   WE_UINT32                   i = 0;
   WE_UINT32                   j = 0;

   if( NULL == pstStringTable || puiHashTableSize == NULL)
   {
      return NULL;
   }

   pucHashTable = CommonEmn_MallocHash(uiStringTableSize, (WE_INT32 *)&uiHashSize);

   if(pucHashTable == NULL)
   {
      return NULL;
   }

   pstStrTab = pstStringTable;

   while(i < uiHashSize && i < uiStringTableSize)
   {
      /*Get the string hash number*/
      uiStrHas = WE_SCL_STRHASH(pstStrTab->pcName, EME_UTILS_STRLEN(pstStrTab->pcName));

      /*Get the string position in hash table*/
      uiDx = uiStrHas % uiHashSize;

      uiStep = 1 + (uiStrHas % (uiHashSize - 2));

      for(; ;)
      {
         if(pucHashTable[uiDx] == 0xFF)
         {
            j = 0;

            pucHashTable[uiDx] = (WE_UCHAR)i;
            pstStrTab++;
            i++;            //Get next string value
            break;
         }
         else
         {
            j++;
            if(j == uiMaxImpactTime)
            {
               j = 0;
               i = 0;
               FREE(pucHashTable);
               pucHashTable = NULL;

               /*
               if Impact times is large than expect times, than should create a 
               new hash table, and free old hash table memory
               */
               pucHashTable = CommonEmn_MallocHash(uiHashSize, (WE_INT32 *)&uiHashSize);

               /*reset pointer*/
               pstStrTab = pstStringTable;

               break;
            }
            else
            {
               uiDx += uiStep;
               if(uiDx > uiHashSize)
               {
                  uiDx -= uiHashSize;

               }

            }

         }

      } /* End for(; ;) */

   }

   *puiHashTableSize = uiHashSize;

   return pucHashTable;
}



static WE_INT32 EME_EMN_DeleteEmnParser(WE_VOID* pvMemMgr, WePMParserHandle phParser)
{
   WE_INT32         uiType      = 0;
   WeStreamHandle   hStream     = 0; 
   St_Emn_MUA* pMe = NULL;
   St_EMNHandle *pstEMNHandle = NULL;

   WePM_Delete(phParser, &uiType, &hStream, (void**)&pstEMNHandle);
   if (NULL == pstEMNHandle)
       return EME_FAILED;
   
   pMe = (St_Emn_MUA*)(&pstEMNHandle->stEmn);

   if (NULL != pMe->pstStream)
   {
      /*  Close the stream */
      BrwStream_Close(pMe->pstStream->hMgr,
         pMe->pstStream->hStream);
      BrwStream_Release(pMe->pstStream->hMgr);
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstStream);
   }

   /*add by jade for modify clocwork 20071108*/
   if(pMe->pstGlobe == NULL)
   {
      EME_UTILS_FREEIF(pvMemMgr, pMe->pcData);
      return SUCCESS;
   }

   if(NULL != pMe->pstGlobe->pstEmnElements)
   {
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnElements->pstStringTable);
#ifdef TEST_USE
      FREEIF(pMe->pstGlobe->pstEmnElements->pucHashTable);
#endif
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnElements);
   }
   if(NULL != pMe->pstGlobe->pstEmnAttributes)
   {
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttributes->pstStringTable);
#ifdef TEST_USE
      FREEIF(pMe->pstGlobe->pstEmnAttributes->pucHashTable);
#endif
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttributes);
   }
   if(NULL != pMe->pstGlobe->pstEmnAttrVals)
   {
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttrVals->pstStringTable);
#ifdef TEST_USE
      FREEIF(pMe->pstGlobe->pstEmnAttrVals->pucHashTable);
#endif
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttrVals);
   }
   if(NULL != pMe->pstGlobe->pstEmnAttrStart)
   {
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttrStart->pstStringTable);
#ifdef TEST_USE
      FREEIF(pMe->pstGlobe->pstEmnAttrStart->pucHashTable);
#endif
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstEmnAttrStart);
   }
   if(NULL != pMe->pstGlobe->pstAttrValue)
   {
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstAttrValue->pstStringTable);
#ifdef TEST_USE
      FREEIF(pMe->pstGlobe->pstAttrValue->pucHashTable);
#endif
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstAttrValue);
   }
   if(NULL != pMe->pstGlobe->pstPublicLiteral)
   {
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstPublicLiteral->pstStringTable);
#ifdef TEST_USE
      FREEIF(pMe->pstGlobe->pstPublicLiteral->pucHashTable);
#endif
      EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe->pstPublicLiteral);
   }
   EME_UTILS_FREEIF(pvMemMgr, pMe->pstGlobe);
   EME_UTILS_FREEIF(pvMemMgr, pMe->pcData);
   return EME_SUCCESS;
}



static WePMParserHandle  EME_EMN_CreateEmnParser(IEMNHandle hEMNHandle, WE_UINT32 uiContentType,WE_INT32 iCharset,WE_UINT32 uiDataLen,WE_BOOL bTextMode)
{
   WE_INT32    iResult = EME_SUCCESS;
   St_Emn_MUA  *pMe = NULL;
   WePMParserHandle  phParser = NULL;
   St_PMStreamParam  *pstStream = NULL;
   St_BrwStreamInput stBrwStreamInput;
   St_EMNHandle *pstEMNHandle = NULL;
   WE_VOID* pvMemMgr;

   if(NULL == hEMNHandle)
      return NULL;

   pstEMNHandle = (St_EMNHandle *)hEMNHandle;
   pMe = (St_Emn_MUA *)(&pstEMNHandle->stEmn);
   pvMemMgr = pstEMNHandle->pvMemMgr;

   if(NULL == pMe->pcData)
      return NULL;

   do 
   {
      pstStream = (St_PMStreamParam*)EME_UTILS_MALLOC(pvMemMgr, sizeof(St_PMStreamParam));
      EME_UTILS_BREAKNULL(pstStream, iResult, EME_NOMEMORY, "EME_EMN_CreateEmnParser malloc failed");
      MEMSET(pstStream, 0, sizeof(St_PMStreamParam));
      MEMSET(&stBrwStreamInput, 0, sizeof(St_BrwStreamInput));
      
      pstStream->hMgr = BrwStream_Init(pstEMNHandle->pvIShell, NULL);
      EME_UTILS_BREAKNULL(pstStream->hMgr, iResult, EME_NOMEMORY, "EME_EMN_CreateEmnParser BrwStream_Init failed");

      stBrwStreamInput.iType = BRW_STREAM_TYPE_BUFFER_READ;
      stBrwStreamInput.stParam.stBuf.pcStr = pMe->pcData;
      stBrwStreamInput.stParam.stBuf.pcCur = pMe->pcData;
      stBrwStreamInput.stParam.stBuf.iLen  = uiDataLen + 1;   
      pstStream->pcInput   = pMe->pcData;
      pstStream->iType     = G_WE_PM_STREAM_BUF;
      pstStream->iLen      = uiDataLen + 1;

      pstStream->hStream   = BrwStream_Open(pstStream->hMgr, &stBrwStreamInput, 0, NULL);
      if (0 >= pstStream->hStream)
      {
         iResult = EME_FAILED;
         break;
      }
      pMe->pstStream = pstStream;
      phParser = WePM_CreateParser((WE_VOID *)pstEMNHandle,
                                   (Fn_WePMInit)EME_EMN_ParseCallBack,
                                   NULL,
                                   pstStream,
                                   iCharset,
                                   WE_CSC_UTF_8,
                                   bTextMode,
                                   uiDataLen + 1,
                                   FALSE,
                                   NULL,
                                   pstEMNHandle->pvCSC);   
      if(NULL == phParser)
         return NULL;
   } while(0);
   
   if (EME_SUCCESS != iResult)
   {
      if (NULL != pstStream)
      {
         if (NULL != pstStream->hStream)
         {
            BrwStream_Close(pstStream->hMgr, pstStream->hStream);
         }
         BrwStream_Release(pstStream->hMgr);
         EME_UTILS_FREEIF(pvMemMgr, pstStream);
      }
   }

   return phParser;   
}



static WE_INT32  EME_EMN_ParseCallBack(WePMParserHandle  hParserData,
                                       WePMDecoderHandle hDecoderData,
                                       const WE_CHAR       *pcszDocType,
                                       WE_INT32             iLen,
                                       WE_INT32             iDocType,
                                       WE_INT32             iCharset)
{
   WE_INT32 nErr = EME_SUCCESS;
   St_Emn_MUA * pMe = NULL;
   Fn_WePMStartEle      cbStartEle     = NULL;
   WE_INT32    iTmpDocType = -1;
   St_EMNHandle*  pstEMNHandle = (St_EMNHandle*)WePM_GetUserData(hParserData);
   pMe = (St_Emn_MUA *)(&pstEMNHandle->stEmn);
   cbStartEle = EME_EMN_StartElement;

   if (NULL != pcszDocType)
   {
      /* Get the int doc type from the pcDoctype. */
      iTmpDocType = WE_SCL_STR2INT(pcszDocType, iLen, pMe->pstGlobe->pstPublicLiteral);
   }/* end of if (NULL != pcDocType) */       

   WePM_SetParserCapacity(hParserData,
                          pMe->pstGlobe->pstEmnElements, 
                          pMe->pstGlobe->pstEmnAttributes, 
                          pMe->pstGlobe->pstEmnAttrVals, 
                          NULL, 
                          pMe->pstGlobe->pucEmnElementTable,
                          pMe->pstGlobe->pusEmnAttributeType);
   
   WePM_SetParserDocFun(hParserData, 
                        cbStartEle, 
                        EME_EMN_EndElement, 
                        EME_EMN_CharacterData); 

   if(NULL != hDecoderData)
      WePM_SetDecoderCapacity(hDecoderData,
                              pMe->pstGlobe->pucEmnAttrTransform,
                              14,
                              pMe->pstGlobe->pstEmnAttrStart,
                              pMe->pstGlobe->pstAttrValue);
   return nErr;
}


static WE_INT32 EME_EMN_StartElement(WE_VOID *pvUserData, WE_INT32 iElementType, St_WePMAttr *pstAttribute, WE_INT32 iAttributeSize)
{
   WE_INT32 i = 0;
   WE_INT32 iRet = EME_SUCCESS;
   St_EmnSemIn stEmnSemIn = {0};
   St_Emn_MUA * pMe = NULL;
   St_EMNHandle *pstEMNHandle = (St_EMNHandle *)pvUserData;
   WE_VOID* pvMemMgr = NULL;

   if(EME_EMN_ELEMENT != iElementType || NULL == pstEMNHandle)
      return EME_FAILED;

   pMe = (St_Emn_MUA *)(&pstEMNHandle->stEmn);
   pvMemMgr = pstEMNHandle->pvMemMgr;

   do 
   {
      for(i = 0;i < iAttributeSize;i ++)
      {
         switch(pstAttribute[i].sType)
         {
         case EME_EMN_ATTRIBUTE_TIME:
            stEmnSemIn.TimeStamp = pstAttribute[i].stValue.pcStr;
            break;
         case EME_EMN_ATTRIBUTE_MAILBOX:
            stEmnSemIn.MailBox = pstAttribute[i].stValue.pcStr;
            break;
         default:
            break;
         }
      }
      
      stEmnSemIn.bTextMode = pMe->bTextMode;
      iRet = EME_EMN_DecodeEmnSem(pstEMNHandle, &stEmnSemIn);
      EME_UTILS_BREAKIF(iRet, "decode emnsen failed");
   } while(0);

   WePM_FreeAttr(pstAttribute, iAttributeSize);

   return EME_SUCCESS;
}



static WE_INT32 EME_EMN_EndElement(WE_VOID *pvUserData)
{
   return EME_SUCCESS;
}

static WE_INT32 EME_EMN_CharacterData(WE_VOID* pvUserData, const WE_CHAR* pcText, WE_INT32  iTextLen, WE_INT32 iMode)
{
   return EME_SUCCESS;
}

static WE_INT32 EME_EMN_DecodeEmnSem(IEMNHandle hEMNHandle, St_EmnSemIn* pstEmnSemIn)
{
   WE_INT32    iRet = EME_SUCCESS;
   WE_CHAR*    pcStrPos = NULL;
   St_EmnRet* pstEmnSemRet = NULL;
   St_EMNHandle*  pstEMNHandle = (St_EMNHandle*)hEMNHandle;
   if (NULL == hEMNHandle ||
       NULL == pstEmnSemIn ||
       NULL == pstEmnSemIn->MailBox)
      return EME_FAILED;

   pstEmnSemRet = &pstEMNHandle->stEmnRet;
   EME_EMN_ResetRet(pstEMNHandle->pvMemMgr, &pstEMNHandle->stEmnRet);

   /*+++++++++++++++++++++++++++++++ decode URI +++++++++++++++++++++++++++++++++*/
   do 
   {
      pstEmnSemRet->iURIType = EME_EMN_URITYPE_ANY;
      /* codec "mailat" URI if the URI is mailat */
      if (pstEmnSemIn->MailBox == (pcStrPos = EME_UTILS_StrStrNC(pstEmnSemIn->MailBox, "mailat:")))
      {
         pstEmnSemRet->iURIType = EME_EMN_URITYPE_MAILAT;
         iRet = EME_EMN_DecodeURIMailAt(pstEMNHandle, pcStrPos+EME_UTILS_STRLEN("mailat:"), pstEmnSemRet);
         EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeEmnSem EME_EMN_DecodePOP failed");
         break;
      }

      /* codec POP URI if the URI is POP */
      if (pstEmnSemIn->MailBox == (pcStrPos = EME_UTILS_StrStrNC(pstEmnSemIn->MailBox, "pop://")))
      {
         pstEmnSemRet->iURIType = EME_EMN_URITYPE_POP;
         pstEmnSemRet->stIServer.usPort = 110; /* default port is 110 */
         iRet = EME_EMN_DecodeURIPOP(pstEMNHandle, pcStrPos+EME_UTILS_STRLEN("pop://"), pstEmnSemRet);
         EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeEmnSem EME_EMN_DecodePOP failed");
         break;
      }

      /* codec IMAP URI if the URI is IMAP */
      if (pstEmnSemIn->MailBox == (pcStrPos = EME_UTILS_StrStrNC(pstEmnSemIn->MailBox, "imap://")))
      {
         pstEmnSemRet->iURIType = EME_EMN_URITYPE_IMAP;
         pstEmnSemRet->stIServer.usPort = 143; /* default port is 143 */
         iRet = EME_EMN_DecodeURIIMAP(pstEMNHandle, pcStrPos+EME_UTILS_STRLEN("IMAP://"), pstEmnSemRet);
         EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeEmnSem EME_EMN_DecodePOP failed");
         break;
      }

      iRet = EME_FAILED;
   } while(0);


   /*+++++++++++++++++++++++++++++++ decode time stamp ++++++++++++++++++++++++++*/
   do 
   {
      EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeEmnSem failed");

      if (NULL != pstEmnSemIn->TimeStamp)
      {
         iRet = EME_EMN_DecodeTimeStamp(pstEMNHandle, pstEmnSemIn->TimeStamp, pstEmnSemIn->bTextMode, pstEmnSemRet);
         EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeEmnSem EME_EMN_DecodePOP failed");
      }
   } while(0);


   return iRet;
}


static WE_INT32 EME_EMN_DecodeURIMailAt(IEMNHandle hEMNHandle, WE_CHAR* pcURI, St_EmnRet* pstEmnSemRet)
{
   return EME_EMN_DecodeISERVER(hEMNHandle, pcURI, pstEmnSemRet);
}


static WE_INT32 EME_EMN_DecodeURIPOP(IEMNHandle hEMNHandle, WE_CHAR* pcURI, St_EmnRet* pstEmnSemRet)
{
   return EME_EMN_DecodeISERVER(hEMNHandle, pcURI, pstEmnSemRet);
}


static WE_INT32 EME_EMN_DecodeISERVER(IEMNHandle hEMNHandle, WE_CHAR* pcURI, St_EmnRet* pstEmnSemRet)
{
   WE_INT32       iRet = EME_SUCCESS;
   WE_CHAR*       pcURITemp = NULL;
   WE_CHAR*       pcStrPos = NULL;
   WE_CHAR*       pcStrPosAt = NULL; /* pos of char '@' */
   St_EMNHandle*  pstEMNHandle = (St_EMNHandle*)hEMNHandle;
   St_EmnServer*    pstIServer = NULL;

   if (NULL == pstEMNHandle ||
       NULL == pcURI ||
       NULL == pstEmnSemRet)
      return EME_FAILED;
   
   do 
   {
      pstIServer = EME_EMN_GetIServer(pstEmnSemRet);
      EME_UTILS_BREAKNULL(pstIServer, iRet, EME_FAILED, "EME_EMN_DecodeISERVER EME_EMN_GetDetailPOP failed");

      pstIServer->iAuthType = EME_EMN_AUTHTYPE_ANY;
      pcURITemp = EME_UTILS_STRDUP(pstEMNHandle->pvMemMgr, pcURI);
      EME_UTILS_BREAKNULL(pcURITemp, iRet, EME_FAILED, "EME_EMN_DecodeISERVER failed");
      
      /*+++++++++++++++++++++ iserver is like this:  ++++++++++++++++++++++++++++++*/
      /*               "rg;AUTH=+APOP@mail.eudora.com:8110"                        */
      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      pcStrPosAt = EME_UTILS_STRCHR(pcURITemp, '@');
      EME_UTILS_BREAKNULL(pcStrPosAt, iRet, EME_FAILED, "EME_EMN_DecodeISERVER find '@' failed");

      *pcStrPosAt = 0;

      pcStrPos = EME_UTILS_StrStrNC(pcURITemp, ";auth=");

      /* if auth type is not NULL, try to get user name and auth */
      if (NULL != pcStrPos)
      {
         /* if user name exists with auth type , get user name */
         if (pcURITemp != pcStrPos)
         {
            *pcStrPos = 0; /* strip the ';' to get user name */
            pstIServer->pcUser = EME_UTILS_STRDUP(pstEMNHandle->pvMemMgr, pcURITemp);
            EME_UTILS_BREAKNULL(pstIServer->pcUser, iRet, EME_FAILED, "EME_EMN_DecodeISERVER strdup failed");
         }
         pcStrPos = pcStrPos + EME_UTILS_STRLEN(";auth=");
         
         /* get auth type */
         if (pcStrPos == EME_UTILS_StrStrNC(pcStrPos, "+APOP"))
         {
            pstIServer->iAuthType = EME_EMN_AUTHTYPE_POP_APOP;
         }
      }
      else if ( pcURITemp != pcStrPosAt) /* user name exists and "auth=" not exists. */
      {
         pstIServer->pcUser = EME_UTILS_STRDUP(pstEMNHandle->pvMemMgr, pcURITemp);
      }

      /* get host name and port */
      pcStrPos = EME_UTILS_STRCHR(pcStrPosAt, ':');

      /* port exists */
      if (NULL != pcStrPos)
      {
         *pcStrPos = 0;
         pstIServer->usPort = (WE_UINT16)EME_UTILS_ATOI(pcStrPos+1);
      }

      pstIServer->pcHost = EME_UTILS_STRDUP(pstEMNHandle->pvMemMgr, pcStrPosAt+1);
      EME_UTILS_BREAKNULL(pstIServer->pcHost, iRet, EME_FAILED, "EME_EMN_DecodeISERVER strdup failed");
      
      if (NULL != EME_UTILS_STRLEN(pstIServer->pcHost) &&
         ('/' == *(pstIServer->pcHost + EME_UTILS_STRLEN(pstIServer->pcHost) - 1) ||
          '\\' == *(pstIServer->pcHost + EME_UTILS_STRLEN(pstIServer->pcHost) - 1)))
      {
         *(pstIServer->pcHost + EME_UTILS_STRLEN(pstIServer->pcHost) - 1) = 0;
      }
   } while(0);

   if (EME_SUCCESS != iRet)
   {
      EME_EMN_ResetIServer(pstEMNHandle->pvMemMgr, pstIServer);
   }
   EME_UTILS_FREEIF(pstEMNHandle->pvMemMgr, pcURITemp);
   return iRet;
}


static WE_INT32 EME_EMN_DecodeMailbox(IEMNHandle hEMNHandle, WE_CHAR* pcStrMailbox, St_EmnRet* pstEmnSemRet)
{
   WE_INT32       iRet = EME_SUCCESS;
   St_EMNHandle*  pstEMNHandle = (St_EMNHandle*)hEMNHandle;
   WE_CHAR* pcMailboxUtf7 = NULL;

   if (NULL == hEMNHandle || 
       NULL == pcStrMailbox ||
       NULL == pstEmnSemRet)
      return EME_FAILED;

   do 
   {
      pcMailboxUtf7 = EME_UTILS_MALLOC(pstEMNHandle->pvMemMgr, EME_UTILS_STRLEN(pcStrMailbox)*2+2);
      EME_UTILS_BREAKNULL(pcMailboxUtf7, iRet, EME_NOMEMORY, "EME_EMN_DecodeMailbox malloc failed");
      EME_UTILS_MEMSET(pcMailboxUtf7, 0, EME_UTILS_STRLEN(pcStrMailbox)*2+2);
      EME_EMN_URLtoMailbox(pcMailboxUtf7, pcStrMailbox);

      EME_UTILS_FREEIF(pstEMNHandle->pvMemMgr, pstEmnSemRet->pcMailboxName);
      pstEmnSemRet->pcMailboxName = EME_UTILS_MALLOC(pstEMNHandle->pvMemMgr, EME_UTILS_STRLEN(pcMailboxUtf7)*3+3);
      EME_UTILS_BREAKNULL(pstEmnSemRet->pcMailboxName, iRet, EME_NOMEMORY, "EME_EMN_DecodeMailbox malloc failed");
      EME_UTILS_MEMSET(pstEmnSemRet->pcMailboxName, 0, EME_UTILS_STRLEN(pcMailboxUtf7)*3+3);
      iRet = EME_IMAP_Utf7ToUtf8(pcMailboxUtf7, pstEmnSemRet->pcMailboxName, pstEMNHandle->pvMemMgr);
      EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeMailbox EME_IMAP_Utf7ToUtf8 failed");
   } while(0);
   
   EME_UTILS_FREEIF(pstEMNHandle->pvMemMgr, pcMailboxUtf7);

   if (EME_SUCCESS != iRet)
   {
      EME_UTILS_FREEIF(pstEMNHandle->pvMemMgr, pstEmnSemRet->pcMailboxName);
   }

   return iRet;
}


static WE_INT32 EME_EMN_DecodeURIIMAP(IEMNHandle hEMNHandle, WE_CHAR* pcURI, St_EmnRet* pstEmnSemRet)
{
   WE_INT32       iRet = EME_SUCCESS;
   WE_CHAR*       pcURITemp = NULL;
   WE_CHAR*       pcStrPos = NULL;
   WE_CHAR*       pcStrMailbox =NULL;
   St_EMNHandle*  pstEMNHandle = (St_EMNHandle*)hEMNHandle;

   if (NULL == pstEMNHandle ||
      NULL == pcURI ||
      NULL == pstEmnSemRet)
      return EME_FAILED;

   do 
   {
      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      /* imap url is like this(rfc2192):                             */
      /* imap://<iserver>/                                           */
      /* imap://<iserver>/<enc_list_mailbox>;TYPE=<list_type>        */
      /* imap://<iserver>/<enc_mailbox>[uidvalidity][?<enc_search>]  */
      /* imap://<iserver>/<enc_mailbox>[uidvalidity]<iuid>[isection] */
      /* we only decode <iserver> and <enc_mailbox> for EMN use      */
      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

      /* get <iserver> */
      pcURITemp = EME_UTILS_STRDUP(pstEMNHandle->pvMemMgr, pcURI);
      EME_UTILS_BREAKNULL(pcURITemp, iRet, EME_FAILED, "EME_EMN_DecodeURIIMAP strdup failed");
      pcStrPos  = pcURITemp;
      while ('/' != *pcStrPos  && 
             '\\' != *pcStrPos &&
             0  != *pcStrPos)
            pcStrPos++;

      /* if the last char is not null, next string is string of mailbox */
      if (NULL != *pcStrPos)
      {
         pcStrMailbox = pcStrPos + 1;
         *pcStrPos = 0;
      }
      
      /* decode <iserver> */
      iRet = EME_EMN_DecodeISERVER(pstEMNHandle, pcURITemp, pstEmnSemRet);
      EME_UTILS_BREAKIF(iRet, "EME_EMN_DecodeURIIMAP EME_EMN_DecodeISERVER failed");

      /* if the pcStrMailbox is not null, it is string of mailbox */
      if (NULL == pcStrMailbox || NULL == *pcStrMailbox)
         break;
      
      /* let pcStrPos point to the end of <enc_mailbox> */
      pcStrPos = pcStrMailbox;
      while (';' != *pcStrPos &&
             '?' != *pcStrPos &&
             0 != *pcStrPos)
            pcStrPos++;
      *pcStrPos = 0;

      /* not support '*' right now */
      if (NULL != EME_UTILS_STRCHR(pcStrMailbox, '*') ||
         pcStrMailbox == pcStrPos)
      {
         iRet = EME_SUCCESS;
         break;
      }

      pcStrPos--;
      if ('\\' == *pcStrPos ||
         '/' == *pcStrPos)
      {
         *pcStrPos = 0;
      }
   
      EME_EMN_DecodeMailbox(pstEMNHandle, pcStrMailbox, pstEmnSemRet);
   } while(0);

   EME_UTILS_FREEIF(pstEMNHandle->pvMemMgr, pcURITemp);
   return iRet;
}


static St_EmnServer* EME_EMN_GetIServer(IEMNHandle hEMNHandle)
{
   St_EmnRet*  pstEmnSemRet = (St_EmnRet*)hEMNHandle;

   if (NULL == pstEmnSemRet)
      return NULL;

   return &pstEmnSemRet->stIServer;
}


static WE_INT32 EME_EMN_ResetRet(WE_VOID*  pvMemMgr, St_EmnRet* pstEmnSemRet)
{
   if (NULL == pstEmnSemRet)
      return EME_FAILED;
   
   EME_EMN_ResetIServer(pvMemMgr, &pstEmnSemRet->stIServer);

   EME_UTILS_FREEIF(pvMemMgr, pstEmnSemRet->pcMailboxName);
   EME_UTILS_MEMSET(pstEmnSemRet, 0, sizeof(St_EmnRet));

   return EME_SUCCESS;
}


static WE_INT32 EME_EMN_ResetIServer(WE_VOID* pvMemMgr, St_EmnServer* pstIServer)
{
   EME_UTILS_FREEIF(pvMemMgr, pstIServer->pcHost);
   EME_UTILS_FREEIF(pvMemMgr, pstIServer->pcUser);
   pstIServer->iAuthType = 0;
   pstIServer->usPort = 0;
   return EME_SUCCESS;
}


static WE_INT32 EME_EMN_DecodeTimeStamp(IEMNHandle hEMNHandle, WE_CHAR* pcTimeStamp, WE_BOOL bTextMode, St_EmnRet* pstEmnSemRet)
{
   WE_INT32       iRet = EME_SUCCESS;
   WE_CHAR*       pcTimeStampTemp = NULL;
   WE_CHAR*       pcSrc = NULL;
   WE_CHAR*       pcDest = NULL;
   WE_CHAR*       pcStrPosLeft = NULL;
   WE_CHAR*       pcStrPosRight = NULL;
   St_EMNHandle*  pstEMNHandle = (St_EMNHandle*)hEMNHandle;
   WE_UINT8       ucChar = 0;
   JULIANTYPE     stJulianType;

   if (NULL == pstEMNHandle ||
      NULL == pcTimeStamp ||
      NULL == pstEmnSemRet)
      return EME_FAILED;
   
   EME_UTILS_MEMSET(&stJulianType, 0, sizeof(JULIANTYPE));

   do 
   {
      /*+++++++++++++++time stamp is like this "2002-04-08T06:40:00Z"  ++++++++*/
      /*               the following comments will use this example            */
      /*                        2002-04-08T06:40:00Z                           */
      /*+++++++++++++++OMA-Push-EMN-v1_0-20040614-C.pdf    5.2  +++++++++++++++*/
      if (FALSE == bTextMode)
      {
         /* if data in is not string, change it to string first */
         pcTimeStampTemp = EME_UTILS_MALLOC(pstEMNHandle->pvMemMgr, EME_EMN_TIMESTAMPLEN);
         EME_UTILS_BREAKNULL(pcTimeStampTemp, iRet, EME_FAILED, "EME_EMN_DecodeTimeStamp malloc failed");
         EME_UTILS_MEMSET(pcTimeStampTemp, 0, EME_EMN_TIMESTAMPLEN);
         pcSrc  = pcTimeStamp;
         pcDest = pcTimeStampTemp;

         /* the first 4bits of src is '$',discard it */
         pcSrc++;

         ucChar  = (WE_UINT8)(*pcSrc); 
         *pcDest = (WE_UINT8)((ucChar & 0xF0)>>4) + (WE_UINT8)'0'; /*  2  */
         pcDest++;
         *pcDest = (WE_UINT8)((ucChar & 0x0F)) + (WE_UINT8)'0';    /*  0  */
         pcDest++;
         pcSrc++;
                     
         ucChar  = (WE_UINT8)(*pcSrc); 
         *pcDest = (WE_UINT8)((ucChar & 0xF0)>>4) + (WE_UINT8)'0'; /*  0  */
         pcDest++;
         *pcDest = (WE_UINT8)((ucChar & 0x0F)) + (WE_UINT8)'0';    /*  2  */
         pcDest++;
         pcSrc++;

         *pcDest = '-';                      /*  -  */
         pcDest++;

         ucChar  = (WE_UINT8)(*pcSrc); 
         *pcDest = (WE_UINT8)((ucChar & 0xF0)>>4) + (WE_UINT8)'0'; /*  0  */
         pcDest++;
         *pcDest = (WE_UINT8)((ucChar & 0x0F)) + (WE_UINT8)'0';    /*  4  */
         pcDest++;
         pcSrc++;

         *pcDest = '-';                      /*  -  */
         pcDest++;

         ucChar  = (WE_UINT8)(*pcSrc); 
         *pcDest = (WE_UINT8)((ucChar & 0xF0)>>4) + (WE_UINT8)'0'; /*  0  */
         pcDest++;
         *pcDest = (WE_UINT8)((ucChar & 0x0F)) + (WE_UINT8)'0';    /*  8  */
         pcDest++;
         pcSrc++;

         *pcDest = 'T';                      /*  T  */
         pcDest++;

         ucChar  = (WE_UINT8)(*pcSrc); 
         *pcDest = (WE_UINT8)((ucChar & 0xF0)>>4) + (WE_UINT8)'0'; /*  0  */
         pcDest++;
         *pcDest = (WE_UINT8)((ucChar & 0x0F)) + (WE_UINT8)'0';    /*  6  */
         pcDest++;
         pcSrc++;

         *pcDest = ':';                      /*  :  */
         pcDest++;

         ucChar  = (WE_UINT8)(*pcSrc); 
         *pcDest = (WE_UINT8)((ucChar & 0xF0)>>4) + (WE_UINT8)'0'; /*  0  */
         pcDest++;
         *pcDest = (WE_UINT8)((ucChar & 0x0F)) + (WE_UINT8)'0';    /*  4  */
         pcDest++;
         pcSrc++;

         *pcDest = ':';                      /*  :  */
         pcDest++;

         ucChar  = (WE_UINT8)(*pcSrc); 
         *pcDest = (WE_UINT8)((ucChar & 0xF0)>>4) + (WE_UINT8)'0'; /*  0  */
         pcDest++;
         *pcDest = (WE_UINT8)((ucChar & 0x0F)) + (WE_UINT8)'0';    /*  0  */
         pcDest++;
         pcSrc++;
         
         *pcDest = 'Z';                      /*  Z  */
         pcDest++;
      }
      else
      {
         pcTimeStampTemp = EME_UTILS_STRDUP(pstEMNHandle->pvMemMgr, pcTimeStamp);
         EME_UTILS_BREAKNULL(pcTimeStampTemp, iRet, EME_FAILED, "EME_EMN_DecodeTimeStamp strdup failed");
      }
      
      
      /* get year */
      pcStrPosLeft = pcTimeStampTemp;
      pcStrPosRight = EME_UTILS_STRCHR(pcStrPosLeft, '-');
      EME_UTILS_BREAKNULL(pcStrPosRight, iRet, EME_FAILED, "EME_EMN_DecodeTimeStamp EME_UTILS_STRCHR failed");
      *pcStrPosRight = 0; /* let '-' be 0 to get year */
      stJulianType.wYear = (WE_UINT16)EME_UTILS_ATOI(pcStrPosLeft);

      /* get month */
      pcStrPosLeft = pcStrPosRight + 1;
      pcStrPosRight = EME_UTILS_STRCHR(pcStrPosLeft, '-');
      EME_UTILS_BREAKNULL(pcStrPosRight, iRet, EME_FAILED, "EME_EMN_DecodeTimeStamp EME_UTILS_STRCHR failed");
      *pcStrPosRight = 0; /* let '-' be 0 to get month */
      stJulianType.wMonth = (WE_UINT16)EME_UTILS_ATOI(pcStrPosLeft);

      /* get day */
      pcStrPosLeft = pcStrPosRight + 1;
      pcStrPosRight = EME_UTILS_STRCHR(pcStrPosLeft, 'T');
      EME_UTILS_BREAKNULL(pcStrPosRight, iRet, EME_FAILED, "EME_EMN_DecodeTimeStamp EME_UTILS_STRCHR failed");
      *pcStrPosRight = 0; /* let 'T' be 0 to get day */
      stJulianType.wDay = (WE_UINT16)EME_UTILS_ATOI(pcStrPosLeft);
      
      /* get hour */
      pcStrPosLeft = pcStrPosRight + 1;
      pcStrPosRight = EME_UTILS_STRCHR(pcStrPosLeft, ':');
      EME_UTILS_BREAKNULL(pcStrPosRight, iRet, EME_FAILED, "EME_EMN_DecodeTimeStamp EME_UTILS_STRCHR failed");
      *pcStrPosRight = 0; /* let ':' be 0 to get hour */
      stJulianType.wHour = (WE_UINT16)EME_UTILS_ATOI(pcStrPosLeft);
      
      /* get minite */
      pcStrPosLeft = pcStrPosRight + 1;
      pcStrPosRight = EME_UTILS_STRCHR(pcStrPosLeft, ':');
      EME_UTILS_BREAKNULL(pcStrPosRight, iRet, EME_FAILED, "EME_EMN_DecodeTimeStamp EME_UTILS_STRCHR failed");
      *pcStrPosRight = 0; /* let ':' be 0 to get minite */
      stJulianType.wMinute = (WE_UINT16)EME_UTILS_ATOI(pcStrPosLeft);
      
      /* get seconds */
      pcStrPosLeft = pcStrPosRight + 1;
      pcStrPosRight = EME_UTILS_STRCHR(pcStrPosLeft, 'Z');
      EME_UTILS_BREAKNULL(pcStrPosRight, iRet, EME_FAILED, "EME_EMN_DecodeTimeStamp EME_UTILS_STRCHR failed");
      *pcStrPosRight = 0; /* let 'Z' be 0 to get seconds */
      stJulianType.wSecond = (WE_UINT16)EME_UTILS_ATOI(pcStrPosLeft);
     
      pstEmnSemRet->uiUTCTime = EME_UTILS_JULIANTOSECONDS(&stJulianType);

      /* judge valide should consider URI, be careful */

   } while(0);

   EME_UTILS_FREEIF(pstEMNHandle->pvMemMgr, pcTimeStampTemp);
   return EME_SUCCESS;
}

/* Convert hex coded UTF-8 URL path to modified UTF-7 IMAP mailbox
 *  dst should be about twice the length of src to deal with non-hex
 *  coded URLs
 */
static WE_VOID EME_EMN_URLtoMailbox(WE_CHAR *pcDest, WE_CHAR *pcSrc)
{
   WE_UINT32 uiUtf8Pos = 0;
   WE_UINT32 uiUtf8Total = 0;
   WE_UINT32 uiIndex = 0;
   WE_UINT32 uiChar = 0;
   WE_UINT32 uiBitsToGo = 0;
   WE_UINT32 uiUcs4 = 0;
   WE_UINT32 uiBitBuf = 0;
   WE_BOOL   bUtf7Mode = FALSE;
   WE_BOOL   bUtf16Flag = FALSE;
   WE_UCHAR  ucHexTable[256] = {0};

   /* initialize hex lookup table */
   for (uiIndex = 0; uiIndex < sizeof (acHex); ++uiIndex) 
   {
       ucHexTable[acHex[uiIndex]] = (WE_UCHAR)uiIndex;
       if (acHex[uiIndex] > (9 + '0'))
          ucHexTable[acHex[uiIndex]+32] = (WE_UCHAR)uiIndex;
   }

   while ((uiChar = *pcSrc) != '\0') 
   {
       ++pcSrc;
       /* undo hex-encoding */
       if (uiChar == '%' && 
           pcSrc[0] != '\0' && 
           pcSrc[1] != '\0') 
       {
           uiChar = (ucHexTable[pcSrc[0]] << 4) | ucHexTable[pcSrc[1]];
           pcSrc += 2;
       }

       /* normal character? */
       if (uiChar >= ' ' && uiChar <= '~') 
       {
           /* switch out of UTF-7 mode */
           if (TRUE == bUtf7Mode) 
           {
               if (uiBitsToGo) 
               {
                 *pcDest++ = acBase64Chars[(uiBitBuf << (6 - uiBitsToGo)) & 0x3F];
               }
               *pcDest++ = '-';
               bUtf7Mode = FALSE;
           }
           *pcDest++ = (WE_UCHAR)uiChar;
           /* encode '&' as '&-' */
           if (uiChar == '&') 
           {
               *pcDest++ = '-';
           }
           continue;
       }
       /* switch to UTF-7 mode */
       if (TRUE != bUtf7Mode) 
       {
           *pcDest++ = '&';
           bUtf7Mode = TRUE;
       }
       /* Encode US-ASCII characters as themselves */
       if (uiChar < 0x80) 
       {
           uiUcs4 = uiChar;
           uiUtf8Total = 1;
       } 
       else if (uiUtf8Total) 
       {
           /* save UTF8 bits into UCS4 */
           uiUcs4 = (uiUcs4 << 6) | (uiChar & 0x3FUL);
           if (++uiUtf8Pos < uiUtf8Total) 
           {
               continue;
           }
       } 
       else 
       {
           uiUtf8Pos = 1;
           if (uiChar < 0xE0) 
           {
               uiUtf8Total = 2;
               uiUcs4 = uiChar & 0x1F;
           } 
           else if (uiChar < 0xF0) 
           {
               uiUtf8Total = 3;
               uiUcs4 = uiChar & 0x0F;
           } 
           else 
           {
               /* NOTE: can't convert UTF8 sequences longer than 4 */
               uiUtf8Total = 4;
               uiUcs4 = uiChar & 0x03;
           }
           continue;
       }
       /* loop to split ucs4 into two utf16 chars if necessary */
       uiUtf8Total = 0;
       do {
           if (uiUcs4 >= UTF16BASE) 
           {
               uiUcs4 -= UTF16BASE;
               uiBitBuf = (uiBitBuf << 16) | ((uiUcs4 >> UTF16SHIFT) + UTF16HIGHSTART);
               uiUcs4 = (uiUcs4 & UTF16MASK) + UTF16LOSTART;
               bUtf16Flag = TRUE;
           } else 
           {
               uiBitBuf = (uiBitBuf << 16) | uiUcs4;
               bUtf16Flag = FALSE;
           }
           uiBitsToGo += 16;
           /* spew out base64 */
           while (uiBitsToGo >= 6) 
           {
               uiBitsToGo -= 6;
               *pcDest++ = acBase64Chars[(uiBitsToGo ? (uiBitBuf >> uiBitsToGo) : uiBitBuf) & 0x3F];
           }
       } while (TRUE == bUtf16Flag);
   }
   /* if in UTF-7 mode, finish in ASCII */
   if (TRUE == bUtf7Mode) 
   {
       if (uiBitsToGo) 
       {
           *pcDest++ = acBase64Chars[(uiBitBuf << (6 - uiBitsToGo)) & 0x3F];
       }
       *pcDest++ = '-';
   }
   /* tie off string */
   *pcDest = '\0';

}