/******************************************************************************
 ** File Name:      lvve_control_param_nv.h                                   *
 ** Author:         Ting.Hu                                                   *
 ** DATE:           2011.03.30                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2011.03.30   Ting.Hu               Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _LVVE_CONTROL_PARAM_NV_H_
#define _LVVE_CONTROL_PARAM_NV_H_

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "LVVE.h"
#include "admm_III.h"
#include "audio_api.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define LVVE_EQ_COEF_SIZE                 (32) 
#define LVVE_MAX_MODE_NAME_LEN (16)
#define LVVE_CONFIG_RESERVE_LEN (64)
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
typedef struct LVVE_CTRL_PARAM_
{
    LVVE_Rx_ControlParams_st    ControlParams_Rx;
    int16  RxEqCoefs[LVVE_EQ_COEF_SIZE];
    LVVE_Tx_ControlParams_st    ControlParams_Tx;
    int16  TxEqCoefs[LVVE_EQ_COEF_SIZE];
}LVVE_CTRL_PARAM_T;


typedef struct LVVE_CTRL_PARAM_NV_
{
    uint8 ucModeName[LVVE_MAX_MODE_NAME_LEN];
    LVVE_CTRL_PARAM_T lvve_ctrl_param_t;
    uint16 lvve_reserve[LVVE_CONFIG_RESERVE_LEN];//reserve[1-9] denote the volume level: 1-9, correspondent with LVVE_CTRL_PARAM_T->ControlParams_Rx->VC_ControlParams->Noise_Sensitivity
}LVVE_CTRL_PARAM_NV_T;


typedef struct LVVE_CTRL_PARAM_NV_LIST_
{
    LVVE_CTRL_PARAM_NV_T  lvve_ctrl_nv_params;
    struct LVVE_CTRL_PARAM_NV_LIST_ *next_ptr;
}LVVE_CTRL_PARAM_NV_LIST_T;

typedef enum
{        
        NV_AUDIO_NXP_HANDSET_E,
        NV_AUDIO_NXP_HANDSFREE_E,
        NV_AUDIO_NXP_HEADSET_E,        
        NV_AUDIO_NXP_BTHS_E,
        NV_AUDIO_NXP_LOOPBHANDSFREE_E,
        NV_AUDIO_NXP_MAX
}NV_AUDIO_NXP_E_;
typedef uint32 NV_AUDIO_NXP_E;
/**---------------------------------------------------------------------------*
 **                             Variables                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description:    This function gets device mode count.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC uint32 LVVE_CTRL_PARAM_GetDevModeCount(void);
 
 /*****************************************************************************/
 //  Description:    This function gets nxp mode name by the appointed nxp index.
 //  Author:          TH
 //  Note:             This function is for audiotester tool. index is the one in the ram list in add lvve mode.
 /*****************************************************************************/
 PUBLIC char * LVVE_CTRL_PARAM_GetDevModeName(
     uint32 index
 );
 
 /*****************************************************************************/
 //  Description:    This function gets nxp mode name by the appointed nxp mode.
 //  Author:          TH
 //  Note:           This function is for upper app layer like mmi/voiceprocess_adp.c. 
 /*****************************************************************************/
 PUBLIC char * LVVE_CTRL_PARAM_GetDevModeNameById(
     NV_AUDIO_NXP_E nxp_mode_e
 );

/*****************************************************************************/
//  Description:    This function gets device mode parameters by device mode name.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC int32 LVVE_CTRL_PARAM_GetDevModeParam(
 uint8 *dev_mode_name_ptr,
 LVVE_CTRL_PARAM_NV_T *lvve_ctrl_param_nv_ptr
);

/*****************************************************************************/
//  Description:    This function sets device mode parameters by device mode name.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC int32 LVVE_CTRL_PARAM_SetDevModeParam(
uint8 *dev_mode_name_ptr,
LVVE_CTRL_PARAM_NV_T *lvve_ctrl_param_nv_ptr
);

/*****************************************************************************/
//  Description:    This function reads device mode parameters from flash.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC int32 LVVE_CTRL_PARAM_ReadModeParamFromFlash(
uint8 * dev_mode_name_ptr,
LVVE_CTRL_PARAM_NV_T *lvve_ctrl_param_nv_ptr
);

/*****************************************************************************/
//  Description:    This function writes device mode parameters to flash.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC int32 LVVE_CTRL_PARAM_WriteModeParamToFlash(
uint8 * dev_mode_name_ptr,
LVVE_CTRL_PARAM_NV_T *lvve_ctrl_param_nv_ptr
);

/*****************************************************************************/
//  Description:    This function creates device mode link list.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC int32 LVVE_CTRL_PARAM_CreateDevModeList(void);

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif //#ifndef _LVVE_CONTROL_PARAM_NV_H_
