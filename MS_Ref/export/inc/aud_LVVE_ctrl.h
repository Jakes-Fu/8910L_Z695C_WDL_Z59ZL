/******************************************************************************
 ** File Name:      aud_nxp_ctl.h                                             *
 ** Author:         TH                                                   *
 ** DATE:           2011.02.28                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:     Application layer interface of NXP Life Vibes Voice Experience engine       *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2011.02.28   TH               Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _AUD_LVVE_CTRL_H_
#define _AUD_LVVE_CTRL_H_

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "asm.h"
//#include "LVVE.h"
#include "lvve_ctrl_param_nv.h"
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
#define LVVE_DEBUG
#ifdef LVVE_DEBUG
#define LVVE_TRACE SCI_TRACE_LOW
#else
#define LVVE_TRACE
#endif /* LVVE_DEBUG */

typedef enum SRC_DATA_TYPE
{
    SRC_DATA_MIC0,
    SRC_DATA_MIC1,
    SRC_DATA_VB, //voice boost
    SRC_DATA_NET
}SRC_DATA_E;

//Config LVVE control parameters while initialization or running status. 
typedef struct CONFIG_LVVE_PARAM
{
    //uint16 *test_file_name_ptr;
    LVVE_CTRL_PARAM_NV_T lvve_ctrl_param_nv_t;
}CONFIG_LVVE_PARAM_T;

/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                             Variables                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function start to construct  LVVE control thread.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E Aud_LVVE_Construct(void);

/*****************************************************************************/
//  Description:    This function start to init resource what the LVVE needs.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E Aud_LVVE_Start(void);

/*****************************************************************************/
//  Description:    This function config LVVE control parameters.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E Aud_LVVE_ConfigParam(CONFIG_LVVE_PARAM_T *config_param_ptr);

/*****************************************************************************/
//  Description:    This function sent audio data to be processed.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E Aud_LVVE_SendData(
    int16 *input_src_buf, //input source data to be processed.
    uint32 len, //input buffer length unit by half-word
    SRC_DATA_E src_data_type //where data from
); 

/*****************************************************************************/
//  Description:    This function stop LVVE resource.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E Aud_LVVE_Stop(void);


/*****************************************************************************/
//  Description:    This function deconstruct the LVVE control thread resource.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E Aud_LVVE_Deconstruct(void);


/*****************************************************************************/
//  Description:    GET nv parameters for alc module
//  Author:         Cherry.Liu
//  Note:           the paras in lvve structure
//****************************************************************************/  
PUBLIC void Aud_LVVE_ALCGetParaFromLvve(
    LVVE_CTRL_PARAM_NV_T* audio_lvve_parm_ptr
);

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif //#ifndef _AUD_LVVE_CTRL_H_
