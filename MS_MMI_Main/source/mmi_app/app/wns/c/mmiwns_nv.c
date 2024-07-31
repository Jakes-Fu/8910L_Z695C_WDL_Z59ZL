/***************************************************************************
** File Name:      mmiwns_nv.c                                                                                                                  *
** Author:          grant peng                                                                                                                    *
** Date:           16/12/2014                                                                                                                    *
** Copyright:      2014 Spreatrum, Incoporated. All Rights Reserved.                                             *
** Description:     This file defines the function about nv                                                                   *
****************************************************************************
**                         Important Edit History                                                                                                 *
** ------------------------------------------------------------------------ *
** DATE                 NAME             DESCRIPTION                                                                                    *
** 12/2014          grant.peng              Create                                                                                         *
**                                                                                                                                                                 *
****************************************************************************/
#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmiwns_export.h"
#include "mmiwns_manager.h"
#include "mmiwns_nv.h"

const uint16 mmiwns_nv_len[] =
{
    sizeof(uint16),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T),
    sizeof(PNS_MGR_APP_NV_INFO_T) 
};

/*****************************************************************************/
//Description : register test module nv
//Global resource dependence : none
//  Author: grant.peng
//Note:
/*****************************************************************************/
PUBLIC void MMI_RegWnsNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_WNS, mmiwns_nv_len, sizeof(mmiwns_nv_len)/sizeof(uint16));
}

