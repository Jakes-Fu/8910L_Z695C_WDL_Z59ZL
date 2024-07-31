
#ifndef _EME_CODEC_UU_H_
#define _EME_CODEC_UU_H_


#include "eme_codec_api.h"
#include "eme_utils_api.h"
#include "eme_stringlinereader.h"

typedef	struct  
{
	WE_CHAR			*pFileName;
	
	WE_CHAR			*pBuf;
	WE_UINT32      uiSize;
} UU_Decode_Buf;

//char* EME_UU_Encode(const char* pSrc, const int len, int mode, char* pStrName);

UU_Decode_Buf	*EME_UU_Decode(const WE_CHAR* pSrc, const WE_INT32 len, WE_VOID* pvMemHdr);

WE_VOID			UU_Decode_Buf_Release(UU_Decode_Buf* pBuf, WE_VOID* pvMemHdr);

#endif