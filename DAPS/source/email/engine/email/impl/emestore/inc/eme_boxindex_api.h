#ifndef EME_BOXINDEX_API_H
#define EME_BOXINDEX_API_H

#include "we_def.h"
#include "eme_file_api.h"
#include "eme_msgopts_api.h"
#include "eme_dataaccess_api.h"

/*Interface define*/
WE_INT32 EME_BoxIndex_GetBoxId(FILEMGR_HANDLE hMgr,  WE_UINT32 dwActId, WE_UINT16 *pwszBoxName, WE_UINT32 *pdwBoxId);

WE_INT32 EME_BoxIndex_UpdateBox(FILEMGR_HANDLE hMgr,WE_UINT32 dwActId, WE_UINT32 dwBoxId, MsgOpt *pstOpts);

WE_INT32 EME_BoxIndex_CreateBox(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT16 *pwcBoxName, WE_UINT32 dwBoxState, WE_UINT32 *pdwBoxId);

WE_INT32 EME_BoxIndex_CreateEmptyBoxIndex(FILEMGR_HANDLE hMgr, WE_UINT32 dwActId);

WE_INT32 EME_BoxIndex_DeleteBox(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 dwBoxId);

WE_INT32 EME_BoxIndex_GetBoxNum(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 *pdwNum);

WE_INT32 EME_BoxIndex_GetBoxInfo(FILE_HANDLE hMgr, WE_UINT32 dwActId, WE_UINT32 uiNum, EMS_BoxInfo *pastBoxInfo);

WE_INT32 EME_BoxIndex_SynBoxInfo(FILE_HANDLE hMgr, WE_UINT32 dwActId);

#endif