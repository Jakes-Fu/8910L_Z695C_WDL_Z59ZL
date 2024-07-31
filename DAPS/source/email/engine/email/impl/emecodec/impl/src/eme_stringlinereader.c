
#include "eme_stringlinereader.h"

WE_UINT32 StringLineReader_Create(StringLineReader **ppo, const WE_CHAR* pSrc, WE_VOID* pvMemHdr)
{	
	StringLineReader	*pMe = NULL;

	if(!pSrc || !ppo)
	{
		return EME_FAILED;
	}

	pMe = (StringLineReader*)EME_UTILS_MALLOC(pvMemHdr,sizeof(StringLineReader));
	if(!pMe)
	{
		return EME_FAILED;
	}
   EME_UTILS_MEMSET(pMe, 0x00, sizeof(StringLineReader));

	pMe->pContent = (WE_CHAR*)pSrc;
	pMe->iCurrentIndex = 0;
	pMe->nLength = EME_UTILS_STRLEN(pSrc);
	pMe->pvMemHdr = pvMemHdr;
	*ppo = pMe;

	return EME_SUCCESS;
}

WE_CHAR*  StringLineReader_GetNextLine(StringLineReader* pMe)
{
	WE_CHAR				*pStrResult = NULL;
	WE_INT32			iOldIndex = 0;
	WE_BOOL				bGotLF = FALSE;

	if(!pMe)
	{
		return NULL;
	}
	
	iOldIndex = pMe->iCurrentIndex;
	if(iOldIndex > pMe->nLength - 1)
	{
		return NULL;
	}

	while( pMe->iCurrentIndex < pMe->nLength - 1 )
	{
		if(pMe->pContent[pMe->iCurrentIndex] == '\n' || pMe->pContent[pMe->iCurrentIndex] == '\r')
		{
			bGotLF = TRUE;
			break;
		}
		pMe->iCurrentIndex ++;
	}

	if(bGotLF == FALSE)
	{
		pMe->iCurrentIndex ++;
	}

	if(pMe->iCurrentIndex == iOldIndex)
	{
		return EME_UTILS_STRDUP(pMe->pvMemHdr,"");
	}
	
	pStrResult = (WE_CHAR*)EME_UTILS_MALLOC(pMe->pvMemHdr, pMe->iCurrentIndex - iOldIndex + 1);
	if(!pStrResult)
	{
		return NULL;
	}
   EME_UTILS_MEMSET(pStrResult, 0x00, pMe->iCurrentIndex - iOldIndex + 1);

	EME_UTILS_MEMCPY(pStrResult, &pMe->pContent[iOldIndex], pMe->iCurrentIndex - iOldIndex);
	pMe->iCurrentIndex ++;
	if(pMe->iCurrentIndex < pMe->nLength)
	{
		if(pMe->pContent[pMe->iCurrentIndex] == '\n' 
			&& pMe->pContent[pMe->iCurrentIndex - 1] == '\r')
		{
			pMe->iCurrentIndex ++;
		}
	}

	return pStrResult;
}

WE_VOID   StringLineReader_Reset(StringLineReader* pMe)
{
	pMe->iCurrentIndex = 0;
}

WE_VOID   StringLineReader_Release(StringLineReader* pMe)
{
	if(pMe)
		EME_UTILS_FREEIF(pMe->pvMemHdr, pMe);
}
