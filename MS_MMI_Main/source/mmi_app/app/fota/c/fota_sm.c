
#ifdef  FOTA_SUPPORT

#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "lcd_cfg.h"
#include "mmk_app.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"
#include "http_api.h"
#include "version.h"
#include "socket_api.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmieng_nv.h"
#include "mmiconnection_export.h"
#include "mmipdp_internal.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"

#include "fota_api.h"
#include "fota_header.h"

LOCAL FOTA_SM_ENUM s_fota_sm = FOTA_SM_NULL;

PUBLIC FOTA_SM_ENUM Fota_SM_Get(void)
{
    return s_fota_sm;
}

PUBLIC void Fota_SM_Set(FOTA_SM_ENUM sm)
{
    s_fota_sm = sm;
}

PUBLIC void Fota_SM_Process(void)
{
    switch(s_fota_sm)
    {
        case FOTA_SM_INIT:
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_INIT");
            Fota_SendLoginRequest();
            break;
        case FOTA_SM_REQUEST:
            //Tell MMI
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_REQUEST");
            CHECK_VERSION_RETURN(FOTA_RETURN_OK);
            break;
        case FOTA_SM_GET_URL:
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_GET_URL");
            Fota_SendGetDataRequest();
            break;
        case FOTA_SM_PRE_DOWNLOAD:
            //Tell MMI
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_PRE_DOWNLOAD");
            if (Fota_GetApnUrl() != NULL 
				|| Fota_GetCfgNvUrl() != NULL 
				|| Fota_GetIMSUrl() != NULL 
				|| Fota_GetIMSToggleUrl() != NULL)
            {
                GET_URL_RETURN(FOTA_RETURN_OK);
            }
            else
            {
                GET_URL_RETURN(FOTA_GET_URL_ERR);
            }
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_PRE_DOWNLOAD--END");
            break;
        case FOTA_SM_APN_DOWNLOAD:
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_APN_DOWNLOAD");
            if (MMI_RESULT_TRUE != Fota_SendDownloadRequest(Fota_GetApnUrl(), Fota_GetUpdatePacPath(), Fota_GetApnFileName()))
                DOWN_PACKAGE_RETURN(FOTA_GET_APN_ERR);
            break;
        case FOTA_SM_CNV_DOWNLOAD:
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_CNV_DOWNLOAD");
            if (MMI_RESULT_TRUE != Fota_SendDownloadRequest(Fota_GetCfgNvUrl(), Fota_GetUpdatePacPath(), Fota_GetCnvFileName()))
                DOWN_PACKAGE_RETURN(FOTA_GET_CNV_ERR);
            break;
        case FOTA_SM_IMS_DOWNLOAD:
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_IMS_DOWNLOAD");
            if (MMI_RESULT_TRUE != Fota_SendDownloadRequest(Fota_GetIMSUrl(), Fota_GetUpdatePacPath(), Fota_GetImsFileName()))
                DOWN_PACKAGE_RETURN(FOTA_GET_IMS_ERR);
            break;
        case FOTA_SM_TOG_DOWNLOAD:
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_TOG_DOWNLOAD");
            if (MMI_RESULT_TRUE != Fota_SendDownloadRequest(Fota_GetIMSToggleUrl(), Fota_GetUpdatePacPath(), Fota_GetToggleFileName()))
                DOWN_PACKAGE_RETURN(FOTA_GET_TOG_ERR);
            break;
        case FOTA_SM_UPDATING:
            //Tell MMI
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_UPDATING");
            DOWN_PACKAGE_RETURN(FOTA_RETURN_OK);
            break;
	case FOTA_SM_DOWN_ERR:
            //Tell MMI
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_DOWN_ERR");
            DOWN_PACKAGE_RETURN(FOTA_GET_URL_REQ_ERR);
            break;
        case FOTA_SM_FINISH:
            SCI_TraceLow("[FOTA Update] Fota_SM_Process FOTA_SM_FINISH");
            Fota_CloseRequest();
	    break;
        default:
            SCI_TraceLow("[FOTA Update] Fota_SM_Process default %d", s_fota_sm);
            break;
    }
}

#endif
