/****************************************************************************
** File Name:      mmibt_export.h                                         *
** Author:         Wenming feng                                            *
** Date:           Dec/22/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** Dec/22/2005    Wenming feng     Create                                  *
**                                                                         *
****************************************************************************/

#ifndef _MMI_NPS_HTTP_H_    
#define  _MMI_NPS_HTTP_H_        

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: yanxian.zhou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMINPS_PdpActive(void);

/*****************************************************************************/
//  Description : Deactive Pdp
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMINPS_PdpDeactive(void);

/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleNPSHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Handle weather app Msg
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleNPSAppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
#ifdef ENABLE_HTTPC//dc_debug
PUBLIC void StartHttpRequest(void);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif
