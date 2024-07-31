#ifndef EME_SERVERCONF_API_H
#define EME_SERVERCONF_API_H

#include "we_def.h"
#include "EMS_Store_API.h"

WE_INT EME_SerConf_GetData(FILE_HANDLE hMgr, WE_UINT32 *puiNum, EMS_ServerConf *pstSerConf);

#endif