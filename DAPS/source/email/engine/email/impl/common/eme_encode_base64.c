/*=====================================================================================
FILE NAME   : eme_encode_base64.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Supply functions for base64 encode and decode.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     wangxiushan                  create eme_encode_base64.c
          
=====================================================================================*/

/*==================================================================================================
*        Include files
==================================================================================================*/

#include "eme_encode_base64.h"
/*#include "em_charset.h"*/



/* ===========define============= */
static const WE_CHAR BASE64_ENCODE_TABLE[64] = {
     65,  66,  67,  68,  69,  70,  71,  72,  /* 00 - 07*/
     73,  74,  75,  76,  77,  78,  79,  80,  /* 08 - 15*/
     81,  82,  83,  84,  85,  86,  87,  88,  /* 16 - 23*/
     89,  90,  97,  98,  99, 100, 101, 102,  /* 24 - 31*/
    103, 104, 105, 106, 107, 108, 109, 110,  /* 32 - 39*/
    111, 112, 113, 114, 115, 116, 117, 118,  /* 40 - 47*/
    119, 120, 121, 122,  48,  49,  50,  51,  /* 48 - 55*/
     52,  53,  54,  55,  56,  57,  43,  47 };/* 56 - 63*/

static const WE_UINT32 BASE64_DECODE_TABLE[256] = {
    255, 255, 255, 255, 255, 255, 255, 255, /*--- 00 -  07*/
    255, 255, 255, 255, 255, 255, 255, 255, /*--- 08 -  15*/
    255, 255, 255, 255, 255, 255, 255, 255, /*--- 16 -  23*/
    255, 255, 255, 255, 255, 255, 255, 255, /*--- 24 -  31*/
    255, 255, 255, 255, 255, 255, 255, 255, /*--- 32 -  39*/
    255, 255, 255,  62, 255, 255, 255,  63, /*--- 40 -  47*/
     52,  53,  54,  55,  56,  57,  58,  59, /*--- 48 -  55*/
     60,  61, 255, 255, 255, 255, 255, 255, /*--- 56 -  63*/
    255,   0,   1,   2,   3,   4,   5,   6, /*--- 64 -  71*/
      7,   8,   9,  10,  11,  12,  13,  14, /*--- 72 -  79*/
     15,  16,  17,  18,  19,  20,  21,  22, /*--- 80 -  87*/
     23,  24,  25, 255, 255, 255, 255, 255, /*--- 88 -  95*/
    255,  26,  27,  28,  29,  30,  31,  32, /*--- 96 - 103*/
     33,  34,  35,  36,  37,  38,  39,  40, /*--- 104 - 111*/
     41,  42,  43,  44,  45,  46,  47,  48, /*--- 112 - 119*/
     49,  50,  51, 255, 255, 255, 255, 255, /*--- 120 - 127*/
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255 };


/* ===========function=========== */
/*static uint16 EME_Gb2Unicode(uint16 gbkCode);*/

/*issue  with the  em_base64EncodeSize():wxs*/
/*get encoded string size from original string*/
WE_UINT32 EME_Base64EncodeSize(WE_UINT32 iSize)
{
    WE_UINT32 nSize = 0;
    WE_UINT32 nCR = 0;
    const WE_UINT32 m_LineWidth = 76;
    nSize = (iSize + 2) / 3 * 4 ;
    nCR = nSize / m_LineWidth; /*calculate number of '\n'*/
    nSize += nCR * 2;
    return nSize;
}



/*get decoded size from original string*/
WE_UINT32 EME_Base64DecodeSize(WE_UINT32 iSize)
{
    return (iSize + 3) / 4 * 3;
}


WE_INT32 EME_FromBase64(WE_CHAR *pDest, WE_CHAR *pSrc, WE_UINT32 uiOrigSize)
{
    WE_UINT32 lByteBuffer = 0;
    WE_UINT32 lByteBufferSpace = 0;
    WE_UINT32 C = 0; /*---temp var*/
    WE_INT32 reallen = 0;
    WE_INT16 no = 0;
    WE_CHAR *InPtr = NULL;
    WE_CHAR *InLimitPtr = NULL;
    WE_CHAR *OutPtr = NULL;
    WE_UINT32 nSize = 0;

    if (NULL == pSrc|| NULL == pDest || 0 == (nSize = uiOrigSize)) 
    {
       EME_UTILS_LOG_ERROR(("EME_FromBase64(),input parameters is invalid!!!!"));
       return 0;
    }
	
    lByteBuffer = 0; 
    lByteBufferSpace = 4;
			    
    InPtr = pSrc;
    InLimitPtr= InPtr + nSize;
    OutPtr = pDest;
			    
    while (InPtr != InLimitPtr)
    {
        no=*InPtr;
        C = BASE64_DECODE_TABLE[no]; /*--- Read from InputBuffer.*/
        InPtr++;
        if (C == 0xFF) continue; /*---read 255 invalidate var*/
        lByteBuffer = lByteBuffer << 6 ;
        lByteBuffer = lByteBuffer | C ;
        lByteBufferSpace--;
        if (lByteBufferSpace != 0) continue; /*---read 4 WE_CHAR one time*/
        /*---write to 3 buffers in reverse order*/
        OutPtr[2] = (WE_CHAR)lByteBuffer;
        lByteBuffer = lByteBuffer >> 8;
        OutPtr[1] = (WE_CHAR)lByteBuffer;
        lByteBuffer = lByteBuffer >> 8;
        OutPtr[0] = (WE_CHAR)lByteBuffer;
        /*---prepare next 3 byte*/
        OutPtr+= 3; 
        lByteBuffer = 0; 
        lByteBufferSpace = 4;
    }
    reallen = (WE_INT32)(OutPtr - pDest);
    /*---deal with last and return length*/
    switch (lByteBufferSpace)
    {
        case 1:
            lByteBuffer = lByteBuffer >> 2;
            OutPtr[1] = (WE_CHAR)lByteBuffer;
            lByteBuffer = lByteBuffer >> 8;
            OutPtr[0] = (WE_CHAR)lByteBuffer;
            pDest[ reallen + 2]=0;
            return reallen + 2;
        case 2:
            lByteBuffer = lByteBuffer >> 4;
            OutPtr[0] = (WE_CHAR)lByteBuffer;
            pDest[ reallen + 1]=0;
            return reallen + 1;
	    default:
            pDest[reallen]=0;
            return reallen;
    }
}


WE_INT32 EME_ToBase64(WE_CHAR *pDest, WE_CHAR *pSrc, WE_UINT32 lenSrc, WE_BOOL bChng)
{
    WE_UINT32 iB = 0;
    WE_UINT32 iInMax3 = 0;
    WE_UINT32 Len = 0;
    WE_CHAR *pInPtr = NULL;
    WE_CHAR *pInLimitPtr = NULL;
    WE_CHAR *OutPtr = NULL;
    WE_UINT32 nSize = 0;

    if (NULL == pDest || NULL == pSrc ||
       0 == (nSize = (lenSrc == 0 ? (WE_UINT32)EME_UTILS_STRLEN(pSrc) : lenSrc)))
    {
       EME_UTILS_LOG_ERROR(("EME_ToBase64(),input parameters is invalid!!!!"));
       return 0;
    }
	    
    pInPtr = pSrc;
    iInMax3 = nSize / 3 * 3;
    OutPtr = pDest;
    pInLimitPtr = pInPtr + iInMax3;
				
	while (pInPtr != pInLimitPtr)
	{
        Len = 0;
        while ((Len < 76) && (pInPtr != pInLimitPtr))
        {
            iB = (WE_UINT8) *pInPtr++;
            iB = iB << 8;
	            
            iB = iB | (WE_UINT8) *pInPtr++;
            iB = iB << 8;
			            
            iB = iB | (WE_UINT8) *pInPtr++;
			            
            /*---4 byte write to buffer in reverse order */
            OutPtr[3] = BASE64_ENCODE_TABLE[iB & 0x3F];
            iB = iB >> 6;
            OutPtr[2] = BASE64_ENCODE_TABLE[iB & 0x3F];
            iB = iB >> 6;
            OutPtr[1] = BASE64_ENCODE_TABLE[iB & 0x3F];
            iB = iB >> 6;
            OutPtr[0] = BASE64_ENCODE_TABLE[iB];
            OutPtr+=4;
            Len+=4;
        }
        if (Len >= 76 && !bChng)
        {
            *OutPtr++ = '\r'; /*---set enter flag*/
            *OutPtr++ = '\n';
        }
	}
	/*---set last*/
	switch (nSize - iInMax3)
	{
        case 1:
            iB = (WE_UINT8) *pInPtr;
            iB = iB << 4;
            OutPtr[1] = BASE64_ENCODE_TABLE[iB & 0x3F];
            iB = iB >> 6;
            OutPtr[0] = BASE64_ENCODE_TABLE[iB];
            OutPtr[2] = '='; /*---fill in last */
            OutPtr[3] = '=';
            OutPtr+=4;
            break;
        case 2:
            iB = (WE_UINT8) *pInPtr++;
            iB = iB << 8;
            iB = iB | (WE_UINT8) *pInPtr;
            iB = iB << 2;
            OutPtr[2] = BASE64_ENCODE_TABLE[iB & 0x3F];
            iB = iB >> 6;
            OutPtr[1] = BASE64_ENCODE_TABLE[iB & 0x3F];
            iB = iB >> 6;
            OutPtr[0] = BASE64_ENCODE_TABLE[iB];
            OutPtr[3] = '=';/*---  Fill remaining byte.*/
            OutPtr+=4;
            break;
        default:
            break;
	}
    return (WE_INT32) (OutPtr - pDest);
}


WE_INT32 EME_Ucs2ToUtf7(const WE_UINT16* strIn, WE_CHAR* strOut, WE_BOOL bModify)
{
   WE_INT32 iResult = EME_SUCCESS; /*must be init success*/
    const WE_UINT16 smallLen = 128; /* be care the value of smallLen must be n*2 */
    /* please refer to rfc1642 following WE_CHAR should be re-encoded */
    const WE_CHAR spec[23] = {33, 34, 35, 36, 37, 38, 42, 59, 60, 61, 
                         62, 64, 91, 92, 93, 94, 95, 96, 123, 124, 125, 126, 0};
    WE_UINT8 b1 = 0;
    WE_UINT8 b2 = 0;
    /* if the stack tmp buffer is not enough, use the heap buffer */
    /* do this, the function will be faster. */
    WE_CHAR tmpB64[130] = {0}; /* the last 2 byte is marked end of string */
    WE_CHAR tmpStr[130] = {0}; /* the last 2 byte is marked end of string */

    WE_CHAR *strB64 = NULL;
    WE_CHAR *str    = NULL;
    WE_CHAR startMark = '+';

    void    *pvMemMgr = NULL;
    
    if (NULL == strIn || NULL == strOut  )
    {
      EME_UTILS_LOG_ERROR(("EME_Ucs2ToUtf7(),input parameters is invalid!!!!"));
       return EME_FAILED;
    }


    if (bModify)
    {
       startMark = '&';
    }

    while (*strIn)
    {
        /* get low byte and high byte */
        b1 = *strIn & 0x00ff;
        b2 = *strIn++ >> 8;
        if ( b2 == 0 && b1 < 128 
             && (bModify || (EME_UTILS_STRCHR(spec, b1) == NULL) )  ) /* it is ascii WE_CHAR */
        {
            if ( b1 == startMark )
            {
                *strOut++ = startMark;
                *strOut++ = '-';
            }
            else
            {
               *strOut++ = b1;                
            }
        }
        else
        {
            WE_INT32 len = 0;
            WE_CHAR* pFind = NULL;
            EME_UTILS_MEMSET(tmpStr, 0, smallLen + 2);
            EME_UTILS_MEMSET(tmpB64, 0, smallLen + 2);
            str = tmpStr; /* default: use the stack */
            str[len++] = b2;
            str[len++] = b1;

            while (*strIn)
            {
                b1 = *strIn & 0x00ff;
                b2 = *strIn++ >> 8;
                if ( b2 == 0 && b1 < 128 && (EME_UTILS_STRCHR(spec, b1) == NULL) )
                {
                    strIn--;
                    break;
                }

                if (len < smallLen)
                {
                    str[len++] = b2;
                    str[len++] = b1;
                }
                else /* len >= smallLen */
                {
                    if (len % smallLen == 0)
                    {
                        /* when first use heap copy the stack buffer */
                        if (len / smallLen == 1)
                        {
                            str = EME_UTILS_MALLOC(pvMemMgr, 2 * smallLen + 1);
                            EME_UTILS_BREAKNULL(str,
                                     iResult,
                                     EME_NOMEMORY,
                                     " malloc failed!!!");

                            EME_UTILS_MEMSET(str,0x00,2 * smallLen + 1);
                            EME_UTILS_MEMMOVE(str, tmpStr, smallLen);
                        }
                        else 
                        {
                           str = EME_UTILS_REALLOC(pvMemMgr,str, (len / smallLen + 1) * smallLen + 1);  
                           EME_UTILS_BREAKNULL(str,
                                    iResult,
                                    EME_NOMEMORY,
                                    "EME_UTILS_REALLOC malloc failed!!!");
                        }
                    }                    
                    str[len++] = b2;
                    str[len++] = b1;
                }               
            }

            if (EME_SUCCESS != iResult)
            {
               EME_UTILS_LOG_ERROR(("EME_Ucs2ToUtf7() failed!!!!"));
               break;
            }
            if ( EME_Base64EncodeSize(len) + 1 > smallLen)
            {
                strB64 = EME_UTILS_MALLOC(pvMemMgr,EME_Base64EncodeSize(len) + 1);
                EME_UTILS_BREAKNULL(strB64,
                         iResult,
                         EME_NOMEMORY,
                         " malloc failed!!!");
                EME_UTILS_MEMSET(strB64,0x00,EME_Base64EncodeSize(len) + 1);
            }

            else
            {
                strB64 = tmpB64;
            }

            EME_ToBase64(strB64, str, len, 1);
            *strOut++ = startMark;
            if ( (pFind = EME_UTILS_STRCHR(strB64, '=')) != NULL)
            {
                EME_UTILS_MEMMOVE(strOut, strB64, pFind - strB64);
                strOut += pFind - strB64;
            }
            else
            {
                EME_UTILS_STRCPY(strOut, strB64);
                strOut += EME_UTILS_STRLEN(strB64);
            }
            *strOut++ = '-';
            
            if (str != tmpStr)
            {
               EME_UTILS_FREEIF(pvMemMgr,str);
            }
            if (strB64 != tmpB64)
            {
               EME_UTILS_FREEIF(pvMemMgr,strB64);
            }
        }
    }
    
    /*error handling*/
    if (EME_SUCCESS != iResult)
    {
       if (str != tmpStr)
       {
          EME_UTILS_FREEIF(pvMemMgr,str);
       }
       if (strB64 != tmpB64)
       {
          EME_UTILS_FREEIF(pvMemMgr,strB64);
       }
    }

    return iResult;
}


WE_INT32 EME_Utf7ToUcs2(const WE_CHAR* strIn, WE_UINT16* strOut, WE_BOOL bModify)
{
   WE_INT32 iResult = EME_SUCCESS; /*must be init success*/
   const WE_UINT16 smallLen = 128;
   /* if the stack tmp buffer is not enough, use the heap buffer */
   /* do this, the function will be faster. */
   WE_CHAR tmpB64[128] = {0};
   WE_CHAR* strB64 = NULL;
   WE_CHAR tmpStr[128] = {0};
   WE_CHAR* str = NULL;
   WE_CHAR startMark = '+';

   void* pvMemMgr = NULL;
   
   if (NULL == strIn || NULL == strOut)
   {
      EME_UTILS_LOG_ERROR(("EME_Utf7ToUcs2(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }


   if (bModify)
   {
      startMark = '&';
   }
   
   while (*strIn)
   {
      if (*strIn != startMark)
      {
         *strOut++ = *strIn++; /* copy directly */
      }
      else
      {
         WE_UINT32 len = 0;
         len = 0;
         EME_UTILS_MEMSET(tmpStr, 0, smallLen);
         EME_UTILS_MEMSET(tmpB64, 0, smallLen);
         str = tmpStr; /* default: use the stack */
         strIn++; /* go behind '+' */
         while (*strIn)
         {
            if (*strIn == '-')
            {
               strIn++;
               break;
            }
            else if ( *strIn == '=' || (EME_UTILS_STRCHR(BASE64_ENCODE_TABLE, *strIn) == NULL) )
            {
               break;
            }
            
            if (len < smallLen)
            {
               tmpStr[len++] = *strIn++;
            }
            else if (len == smallLen)
               {
                  /* when first use heap copy the stack buffer */
                     str = EME_UTILS_MALLOC(pvMemMgr,2 * smallLen);
                     EME_UTILS_BREAKNULL(str,
                        iResult,
                        EME_NOMEMORY,
                        " malloc failed!!!");
                     
                     EME_UTILS_MEMSET(str,0x00, 2 * smallLen);
                     EME_UTILS_MEMMOVE(str, tmpStr, smallLen);
                str[len++] = *strIn++;
                  }
                  else  
                  {
                if (len % smallLen == 0)
                {
                     str = EME_UTILS_REALLOC(pvMemMgr,str, (len / smallLen + 1) * smallLen);  
                     EME_UTILS_BREAKNULL(str,
                        iResult,
                        EME_NOMEMORY,
                        "EME_UTILS_REALLOC  malloc failed!!!");
               }                    
               str[len++] = *strIn++;
            }       
         }
         
         if (EME_SUCCESS != iResult)
         {
            EME_UTILS_LOG_ERROR(("EME_Utf7ToUcs2() failed!!!!"));
            break;
         }
         
         if (EME_UTILS_STRLEN(str) == 0)
         {
            *strOut++ = startMark;
         }
         else
         {                
            WE_UINT16 cL = 0;
            WE_INT32 sizeB64 = 0;
            WE_CHAR* pTravel = NULL;
            WE_UINT16 cH = 0;
            
            if ( EME_Base64DecodeSize(EME_UTILS_STRLEN(str)) + 1 > smallLen)
            {
               strB64 = EME_UTILS_MALLOC(pvMemMgr,EME_Base64DecodeSize(EME_UTILS_STRLEN(str)) + 1);
               EME_UTILS_BREAKNULL(strB64,
                  iResult,
                  EME_NOMEMORY,
                  "In EME_Utf7ToUcs2(): strB64  malloc failed!!!");
               EME_UTILS_MEMSET(strB64,0x00,EME_Base64DecodeSize(EME_UTILS_STRLEN(str)) + 1);
            }
            else
            {
               strB64 = tmpB64;
            }
            pTravel = strB64;                
            
            sizeB64 = EME_FromBase64(strB64, str, EME_UTILS_STRLEN(str));
            /* sizeB64 must be n*2 */
            if (sizeB64 % 2 != 0)
            {
               if (str != tmpStr)
               {
                  EME_UTILS_FREEIF(pvMemMgr,str);
               }
               if (strB64 != tmpB64)
               {
                  EME_UTILS_FREEIF(pvMemMgr,strB64);
               }
               iResult = EME_FAILED;
               break;
            }
            
            while ( sizeB64 > 0 )
            {
               cH = *pTravel++;
               cL = *pTravel++ & 0x00ff;
               *strOut++ = (cH << 8) + cL;
               sizeB64 -= 2;
            }
            
            if (str != tmpStr)
            {
               EME_UTILS_FREEIF(pvMemMgr,str);
            }
            if (strB64 != tmpB64)
            {
               EME_UTILS_FREEIF(pvMemMgr,strB64);
            }
         }
        }
        
    }
    /*error handling*/
    if (EME_SUCCESS != iResult)
    {
       if (str != tmpStr)
       {
          EME_UTILS_FREEIF(pvMemMgr,str);
       }
       if (strB64 != tmpB64)
       {
          EME_UTILS_FREEIF(pvMemMgr,strB64);
       }
    }

    return iResult;
}


/* convert utf16 to utf8 */
WE_INT32 EME_Ucs2ToUtf8(const WE_UINT16* strIn, WE_UINT8* strOut)
{	
   WE_UINT32 i = 0;
   /* low 8 bit temp, and high 8 bit temp */
   WE_UINT8 lch = 0;
   WE_UINT8 hch = 0;
   if (NULL == strIn || NULL == strOut)
   {
      EME_UTILS_LOG_ERROR(("EME_Ucs2ToUtf8(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   for (i = 0; ;i++)
   {
      if (strIn[i] == 0)
      {
         break;
      }
      
      lch = strIn[i] & 0xff;
      hch = (strIn[i] >> 8) & 0xff;		
      if ( !hch && lch < 0x80) /* 0000 0000-0000 007F -> 0xxxxxxx */
      {
         *strOut++ = lch;
      }
      else
      {
         WE_UINT8 b = (hch << 2) | (lch >>6);
         if ( hch < 0x08) /* 0000 0080-0000 07FF -> 110xxxxx 10xxxxxx */
         {
            *strOut++ = 0xc0 | b;
         }
         else /* 0000 0800-0000 FFFF -> 1110xxxx 10xxxxxx 10xxxxxx */
         {
            *strOut++ = 0xe0 | (hch >> 4);
            *strOut++ = 0x80 | (b&0x3f);
         }
         *strOut++ = 0x80 | (lch&0x3f);
      }
   }
   *strOut = 0;
   return EME_SUCCESS;	
}


/* convert utf8 to utf16 */
WE_INT32 EME_Utf8ToUcs2(const WE_UINT8* strIn, WE_UINT16* strOut)
{
   WE_INT32 i = 0;
   WE_INT32 need = 0;
   WE_UINT16 uc = 0;
   WE_UINT8 ch = 0;
   WE_INT32 iResult = EME_SUCCESS; /*must be init to success*/
   if (NULL == strIn || NULL == strOut)
   {
      EME_UTILS_LOG_ERROR(("EME_Utf8ToUcs2(),input parameters is invalid!!!!"));
      return EME_FAILED;
   }
   
   for (i = 0; ; i++,strIn++)
   {
      ch = *strIn;
      if (ch == 0)
      {
         break;
      }
      if (need)
      {
         if ( (ch&0xc0) == 0x80 )
         {
            uc = (uc << 6) | (ch & 0x3f);
            need--;
            if ( !need ) 
            {
               *strOut++ = uc;
            }
         }
         else
         {
            /* error */ 
            *strOut++ = 0xfffd;
            need = 0;
            iResult = EME_FAILED;/*PB*/
            EME_UTILS_LOG_ERROR(("MAY be problem!!!!!!!!!!!"));
         }
      }
      else
      {
         if (ch < 128)
         {
            *strOut++ = ch;
         }
         else if ( (ch&0xe0) == 0xc0 )
         {
            uc = ch &0x0f;
            need = 1;
         }
         else if ( (ch&0xf0) == 0xe0 )
         {
            uc = ch & 0x0f;
            need = 2;
         }
      }
   }

   *strOut = 0;
   return iResult;	
}



