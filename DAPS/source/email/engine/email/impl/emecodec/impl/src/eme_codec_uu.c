#include "eme_codec_uu.h"

const WE_INT32 PERLINE_COUNT2 = 45;

const WE_INT32 PADDING_DIGEST = 1;

#define pCRLF "\n"

#define DEFAULT_FILENAME "default.dat"

#include "eme_stringlinereader.h"

//mode "unix line mode 3 octet digests"

WE_VOID	DecodePrefix(const WE_CHAR* pSrc, WE_CHAR** ppFileName, WE_VOID* pvMemHdr)
{
	WE_CHAR			*pStart = NULL;	

	if(!pSrc || !ppFileName)
	{
		return;
	}

	pStart = EME_UTILS_STRRCHR(pSrc, (WE_INT32)' ');

	if(pStart != NULL)
	{
		*ppFileName = EME_UTILS_STRDUP(pvMemHdr, pStart + 1);		
		
		return;
	}

	*ppFileName = NULL;
}


WE_UINT32 StringBuffer_AppendStr2(WE_CHAR** ppstr, WE_CHAR* psrc,WE_UINT32 nSize,WE_INT nLen, WE_VOID* pvMemHdr)
{
	WE_CHAR			*pTmp = NULL;

	if(!ppstr || !psrc)
	{
		return EME_FAILED;
	}	

	pTmp = (WE_CHAR*)EME_UTILS_REALLOC(pvMemHdr, *ppstr, nSize + nLen + 1);
	if(!pTmp)
	{
		return EME_FAILED;
	}

	EME_UTILS_MEMCPY(pTmp + nSize, psrc, nLen);
	pTmp[nSize + nLen] = '\0';
	*ppstr = pTmp;
	return EME_SUCCESS;
}

UU_Decode_Buf* EME_UU_Decode(const WE_CHAR* pSrc, const WE_INT32 len, WE_VOID* pvMemHdr)
{
	WE_UINT32			uiResult = EME_FAILED;
	WE_INT32			i = 0;
	UU_Decode_Buf		*pBuf = NULL;
	WE_CHAR				*pStrBuf = NULL;
	WE_BOOL				bGotEnd = FALSE;

	WE_INT32			lineCount = 0;
	WE_INT32			lineNeed = 0;
	StringLineReader	*pLineReader = NULL;	

	if(!pSrc || len <= 0 )
	{
		return NULL;
	}
	
	//uiResult = StringBuffer_Create(&pStrBuf, NULL);
   
	uiResult = StringLineReader_Create(&pLineReader, pSrc, pvMemHdr);
	if(uiResult != EME_SUCCESS)
	{
		return NULL;
	}
	pBuf = (UU_Decode_Buf*)EME_UTILS_MALLOC(pvMemHdr, sizeof(UU_Decode_Buf));
	if(!pBuf)
	{
		StringLineReader_Release(pLineReader);
		return NULL;	
	}
	EME_UTILS_MEMSET(pBuf, 0, sizeof(UU_Decode_Buf));

	do 
	{
		WE_CHAR		*pLineBuf = NULL;

		if(EME_UTILS_STRNCMP(pSrc, "begin ", 6) != 0)
		{
			uiResult = EME_FAILED;
			break;
		}

		pLineBuf = StringLineReader_GetNextLine(pLineReader);
		DecodePrefix(pLineBuf, &pBuf->pFileName, pvMemHdr);
		EME_UTILS_FREEIF(pvMemHdr, pLineBuf);
		//Skip -begin-
      pStrBuf = (WE_CHAR*)EME_UTILS_MALLOC(pvMemHdr,1);
      if(!pStrBuf)
      {
         uiResult = EME_FAILED;
         break;
      }
      EME_UTILS_MEMSET(pStrBuf, 0x00, 1);

		while ((pLineBuf = StringLineReader_GetNextLine(pLineReader)) != NULL)
		{
			if(EME_UTILS_STRNCMP(pLineBuf, "end", 3) == 0)
			{
				bGotEnd = TRUE;
				EME_UTILS_FREE(pvMemHdr,pLineBuf);	
				break;
			}
			
			if(pLineBuf[0] < ' ')
			{
				uiResult = EME_FAILED;
				EME_UTILS_FREE(pvMemHdr, pLineBuf);
				break;
			}
			lineCount = (WE_INT32)pLineBuf[0];
			lineCount = (lineCount - ' ') & 0X3f;
			if(lineCount > 0)
			{
				lineNeed = (lineCount * 8 + 5)/6;
				if((WE_INT32)EME_UTILS_STRLEN(pLineBuf) >= lineNeed + 1)
				{					
					WE_UINT8		a, b;
					WE_INT32		bufsize = 0;
					WE_CHAR			*buffer = NULL;
					WE_UINT8		uFlag = 0;
					i = 1;
					/*
					* A correct uuencoder always encodes 3 characters at a time, even
					* if there aren't 3 characters left.  But since some people out
					* there have broken uuencoders we handle the case where they
					* don't include these "unnecessary" characters.
					*/
					buffer = (WE_CHAR*)EME_UTILS_MALLOC(pvMemHdr, lineCount + 1);
					if(buffer == NULL)
					{
						uiResult = EME_FAILED;
						EME_UTILS_FREE(pvMemHdr, pLineBuf);
						break;
					}
               EME_UTILS_MEMSET(buffer, 0, lineCount + 1);

					while (bufsize < lineCount) 
					{
						++uFlag;
						// continue decoding until we get 'count' decoded chars
						a = (WE_UINT8)((pLineBuf[i++] - ' ') & 0x3f);
						b = (WE_UINT8)((pLineBuf[i++] - ' ') & 0x3f);
						buffer[bufsize++] = (WE_UINT8)(((a << 2) & 0xfc) | ((b >> 4) & 3));
						
						if (bufsize < lineCount) 
						{
							a = b;
							b = (WE_UINT8)((pLineBuf[(i++)] - ' ') & 0x3f);
							buffer[bufsize++] =
								(WE_UINT8)(((a << 4) & 0xf0) | ((b >> 2) & 0xf));
								++uFlag;
						}
						
						if (bufsize < lineCount) 
						{
							a = b;
							b = (WE_UINT8)((pLineBuf[(i++)] - ' ') & 0x3f);
							buffer[bufsize++] = (WE_UINT8)(((a << 6) & 0xc0) | (b & 0x3f));
							++uFlag;
						}
					}
					
					if(uFlag > 0)
					{
						uiResult = StringBuffer_AppendStr2(&pStrBuf, buffer,pBuf->uiSize,(WE_INT)uFlag,pvMemHdr);
                  pBuf->uiSize += (WE_UINT32)uFlag;
					}
						
					if(uiResult != SUCCESS)
					{
						EME_UTILS_FREE(pvMemHdr, pLineBuf);
						break;
					}
					EME_UTILS_FREE(pvMemHdr, buffer);
				}
			}

			EME_UTILS_FREE(pvMemHdr, pLineBuf);
		}

		if(bGotEnd == TRUE)
		{
			uiResult = EME_SUCCESS;
		}
		else
		{
			uiResult = EME_FAILED;
		}
		
	} while(0);
	
	StringLineReader_Release(pLineReader);

	if(uiResult != EME_SUCCESS)
	{
		EME_UTILS_FREEIF(pvMemHdr, pStrBuf);
		UU_Decode_Buf_Release(pBuf,pvMemHdr);

		return NULL;
	}

	pBuf->pBuf = (pStrBuf);
	//StringBuffer_Release(pStrBuf);

	return pBuf;
}


WE_VOID UU_Decode_Buf_Release(UU_Decode_Buf* pBuf,WE_VOID* pvMemHdr)
{	
	if(!pBuf)
	{
		return;
	}

	EME_UTILS_FREEIF(pvMemHdr, pBuf->pBuf);
	EME_UTILS_FREEIF(pvMemHdr, pBuf->pFileName);
	EME_UTILS_FREE(pvMemHdr, pBuf);
}
