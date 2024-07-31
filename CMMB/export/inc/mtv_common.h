/******************************************************************************
 ** File Name:      mtv_common.h                                              *
 ** Author:         Spreadtrum                                                *
 ** DATE:           06/23/2009                                                *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    data structures and functions definitions of mtv common   *
 **                                                                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/23/2009     Yelianna         Create                                    *
 ******************************************************************************/
  
#ifndef _MTV_COMMON_H
#define _MTV_COMMON_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "kd.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern   "C"
{
#endif


typedef enum
{
    CA_INFO_TYPE_SYSTEM = 1664,
    CA_INFO_TYPE_SMARTCARD
}CAInfoType;

typedef enum
{
    CA_INFO_CAK_VERSION = 0,
    CA_INFO_IRD_SERIAL_NUMBER,
    CA_INFO_PROJECT_INFO
} CASystemInfoSubtype;

typedef enum
{
    CA_INFO_ECM_SYSTEM_ID = 0,
    CA_INFO_EMM_SYSTEM_ID,
    CA_INFO_VERSION,
    CA_INFO_SERIAL_NUMBER,
    CA_INFO_ID,
    CA_INFO_NUMBER,
    CA_INFO_EXPIRATION_DATE,
    CA_INFO_PROVIDER_ID,
    CA_INFO_SESSION_ID,
    CA_INFO_ZIP_CODE,
    CA_INFO_DELTA_TIME,
    CA_INFO_STATE
} CASmartcardInfoSubtype;




#ifdef __cplusplus
}
#endif

#endif  // _MTV_COMMON_H