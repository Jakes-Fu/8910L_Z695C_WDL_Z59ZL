#ifndef __FOTA_H__
#define __FOTA_H__

#ifdef FOTA_SUPPORT
#include "sci_types.h"

typedef enum
{
    FOTA_RETURN_OK,
    FOTA_STATUS_ERR,
    FOTA_INIT_REQ_ERR,
    FOTA_INIT_CNF_ERR,
    FOTA_LOGIN_REQ_ERR,
    FOTA_LOGIN_ERR,
    FOTA_GET_URL_REQ_ERR,
    FOTA_GET_URL_ERR,
    FOTA_GET_APN_ERR,
    FOTA_GET_CNV_ERR,
    FOTA_GET_IMS_ERR,
    FOTA_GET_TOG_ERR,
} FOTA_SEVER_RESULT;

typedef void (*RESULT_FUNC)(FOTA_SEVER_RESULT result);

BOOLEAN MMIFOTA_ConnectPDP(void);

PUBLIC void MMIFOTA_InitModule(void);

PUBLIC void MMIFOTA_Check_Version(RESULT_FUNC call_back);

PUBLIC void MMIFOTA_DownLoad_Package(RESULT_FUNC call_back);


#endif // FOTA_SUPPORT

#endif // __FOTA_H__
