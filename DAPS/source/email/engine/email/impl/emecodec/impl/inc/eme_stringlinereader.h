

#ifndef _EME_STRING_LINE_READER_H_
#define _EME_STRING_LINE_READER_H_
/*
#include "AEE.h"
#include "AEEStdLib.h"
*/

#include "eme_codec_api.h"
#include "eme_utils_api.h"

typedef struct
{
	WE_INT32				iCurrentIndex;
	WE_CHAR					*pContent;
	WE_INT32				nLength;
	WE_VOID					*pvMemHdr;
} StringLineReader;

WE_UINT32 StringLineReader_Create(StringLineReader **ppo, const WE_CHAR* pSrc, WE_VOID* pvMemHdr);

WE_CHAR*  StringLineReader_GetNextLine(StringLineReader* pMe);

WE_VOID   StringLineReader_Reset(StringLineReader* pMe);

WE_VOID   StringLineReader_Release(StringLineReader* pMe);

#endif
