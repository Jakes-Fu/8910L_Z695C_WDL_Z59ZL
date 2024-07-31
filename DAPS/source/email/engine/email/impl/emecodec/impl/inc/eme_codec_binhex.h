
#ifndef _EME_CODEC_BINHEXH_H_
#define _EME_CODEC_BINHEXH_H_

#include "eme_utils_api.h"
#include "we_def.h"

typedef WE_UINT8 byte;

typedef WE_UINT16 word;

typedef WE_INT32 (*GET)(WE_VOID*);

typedef WE_VOID (*WRITE)(WE_VOID* p,WE_INT32 c);

typedef WE_VOID (* BH_NOTIFY)(WE_VOID *p, WE_BOOL bSuccess);

WE_BOOL parseBinHex(WE_VOID* pvCBMgr, GET from, WE_VOID* pf,WRITE to,WE_VOID* pt,BH_NOTIFY notify,WE_VOID* pNotify);

#endif