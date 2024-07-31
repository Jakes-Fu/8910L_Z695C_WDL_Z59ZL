#ifndef EME_ACTINDEX_API_H
#define EME_ACTINDEX_API_H

#include "we_def.h"
#include "eme_file_api.h"
#include "eme_dataaccess_api.h"

WE_INT32 EME_ActIndex_GetAccountId(FILE_HANDLE hMgr, WE_UINT16 *pdwActName, WE_UINT32 *pdwActId);

WE_INT32 EME_ActIndex_CreateAccount(FILE_HANDLE hMgr, WE_UINT16 *pdwActName, WE_UINT32 *pdwActId);

WE_INT32 EME_ActIndex_DeleteAccount(FILE_HANDLE hMgr, WE_UINT32 dwActId);

WE_INT32 EME_ActIndex_GetAccountNum(FILE_HANDLE hMgr, WE_UINT32 *pdwActNum);

WE_INT32 EME_ActIndex_GetActInfo(FILEMGR_HANDLE hMgr, WE_UINT32 dwNum, EMS_ActInfo *pActInfo);

WE_INT32 EME_ActIndex_SetActGlobalCfgItem(FILEMGR_HANDLE hMgr, WE_UINT32 uiItem,  WE_VOID *pItemVal);

WE_INT32 EME_ActIndex_SetActGlobalCfgItemEx(FILEMGR_HANDLE hMgr, WEMsgOpt *pstOpt);

WE_INT32 EME_ActIndex_SetActCfgItem(FILEMGR_HANDLE hMgr, WE_UINT32 uiActId, WE_UINT32 uiItem,  WE_VOID *pItemVal);

WE_INT32 EME_ActIndex_SetActCfgItemEx(FILEMGR_HANDLE hMgr, WE_UINT32 uiActId, WEMsgOpt *pstOpt);

WE_INT32 EME_ActIndex_GetActGlobalCfgItem(FILEMGR_HANDLE hMgr, WE_UINT32 uiItem,  WE_VOID **ppItemVal);

WE_INT32 EME_ActIndex_GetActCfgItem(FILEMGR_HANDLE hMgr, WE_UINT32 uiActId, WE_UINT32 uiItem,  WE_VOID **ppItemVal);

WE_INT32 EME_ActIndex_CreateEmptyActIndex(FILE_HANDLE hMgr);

WE_INT32 EME_ActIndex_CheckAndFixedActIndex(FILE_HANDLE hMgr);

WE_INT32 EME_ActIndex_FactoryReset(FILE_HANDLE hMgr);

WE_INT32 EME_ActIndex_SetActiveActId(FILE_HANDLE hMgr, WE_UINT32 dwActId);

WE_INT32 EME_ActIndex_GetActiveActId(FILE_HANDLE hMgr, WE_UINT32 *pdwActId);

WE_BOOL  EME_ActIndex_CheckActountOK(FILE_HANDLE hMgr, WE_UINT32 dwActId);

#endif