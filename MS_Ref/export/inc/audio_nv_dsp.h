/******************************************************************************
 ** File Name:      audio_nv_dsp.h                                              
 ** Author:         shujing.dong                                              
 ** DATE:           01/17/2011                                               
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:    This file defines the basic operation interfaces and data   
 **                 structure of audio device mode manager controlled by dsp.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 **  -----------------------------------------------------------------------  
 ** DATE           NAME             DESCRIPTION                               
 ** 01/17/2011     shujing.dong     Create.                                   
 ******************************************************************************/

/*! \file audio_nv_dsp.h
*  \author Shujing.Dong 
*  \date Jan 17, 2011
*  \brief This file defines the basic operation interfaces and data structure 
*          of audio device mode manager controlled by dsp.
*/ 

#ifndef _AUDIO_NV_DSP_H
#define _AUDIO_NV_DSP_H
/**---------------------------------------------------------------------------**
 **                         Dependencies                                      **
 **---------------------------------------------------------------------------**/
#include "sci_types.h"
#include "audio_api.h"
#include "admm_III.h"
/**---------------------------------------------------------------------------**
 **                        Debugging Flag                                     **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
#ifdef WIN32
#define PACKED
#else
#define PACKED __packed
#endif
#define AUDIO_NV_DSP_MODE_NAME_MAX_LEN (16)

#define AUDIO_NV_DSP_MODE_STRUCT_RESERVE (24)
#define AUDIO_NV_DSP_VOL_CFG_NUM (32)
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef enum
{
    AUDIO_NV_DSP_NO_ERROR = 0x0,
    AUDIO_NV_DSP_ALLOC_ERROR = BIT_0, /*<!alloc error.*/
    AUDIO_NV_DSP_INPUT_PARA_ERROR = BIT_1, /*<!input parameter error.*/
    AUDIO_NV_DSP_MODE_NO_EXIST = BIT_3,  /*<!the mode with this name is not exist.*/
    
    AUDIO_NV_DSP_ERROR    
}AUDIO_NV_DSP_RESULT_E;



typedef struct Audio_Nv_Dsp_Mode_Struct
{	 
    uint16 dac_set;
    uint16 adc_set;
    uint16 path_set;
    uint16 vol_cfg0[AUDIO_NV_DSP_VOL_CFG_NUM];      //adc
    uint16 vol_cfg1[AUDIO_NV_DSP_VOL_CFG_NUM];      //dac
    uint16 vol_cfg2[AUDIO_NV_DSP_VOL_CFG_NUM];
    uint16 reserve[AUDIO_NV_DSP_MODE_STRUCT_RESERVE];
}AUDIO_NV_DSP_MODE_STRUCT_T;


typedef struct Audio_Nv_Dsp_Mode_Info_struct
{
    uint8 ucModeName[AUDIO_NV_DSP_MODE_NAME_MAX_LEN];	//node name.
    AUDIO_NV_DSP_MODE_STRUCT_T tAudioNvDspModeStruct;	// Audio structure    
}AUDIO_NV_DSP_MODE_INFO_T;

typedef enum
{ 
        NV_AUDIO_DSP_CODEC_HANDSET_E,
        NV_AUDIO_DSP_CODEC_HANDSFREE_E, 
        NV_AUDIO_DSP_CODEC_HEADSET_E,
        NV_AUDIO_DSP_CODEC_MP4HFTR_E,
        NV_AUDIO_DSP_CODEC_RECORD_E,
        NV_AUDIO_DSP_CODEC_BTHS_E,
        NV_AUDIO_DSP_CODEC_MP4HFTRHEADSET_E,
        NV_AUDIO_DSP_CODEC_RECORDHEADSET_E,
        NV_AUDIO_DSP_CODEC_LOOPBHANDFREE_E,
        NV_AUDIO_DSP_CODEC_MAX
}NV_AUDIO_DSP_CODEC_E_;
typedef uint32 NV_AUDIO_DSP_CODEC_E;

/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Function Prototypes                               **
 **---------------------------------------------------------------------------**/
/**---------------------------------------------------------------------------**
 **                         Function Define                                   **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_InitModeManager(void) 
//! \param void No Parameter
//! \return Operation results. \sa AUDIO_NV_DSP_RESULT_E
//! \brief  Description:  This function is to init Mode DSP Manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_InitModeManager(void);


/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_DSP_MODE_INFO_T* AUDIONVDSP_GetFirstDevMode(void)
//! \param void No parameter.
//! \return The first device mode saved in device mode manager.
//! \brief  Description:  This function is to get the first device mode in the current 
//!                             device mode manager.
//! \author Author:  Jian.Chen
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_NV_DSP_MODE_INFO_T* AUDIONVDSP_GetFirstDevMode(void);
/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_DSP_MODE_INFO_T* AUDMOD_GetNextDevMode(
//!    AUDMOD_DEVMOD_T* ptPreDevMode
//!    )
//! \param ptPreDevMode Previous device mode pointer.
//! \return The device mode after ptPreDevMode.
//! \brief  Description:  This function is to get the address of info structure 
//!                         of next device mode
//! \author Author:  Jian.Chen
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_NV_DSP_MODE_INFO_T* AUDIONVDSP_GetNextDevMode(
    AUDIO_NV_DSP_MODE_INFO_T* ptPreDevMode);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIONVDSP_GetModeNumber(void)
//! \param void No parameter.
//! \return The total number of registered modes.
//! \brief  Description:  This function is to get the number of mode in the  
//!                       current mode manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDIONVDSP_GetModeNumber(void);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_AddMode(
//!    AUDIO_NV_DSP_MODE_INFO_T* ptMode
//!    )
//! \param ptMode Added mode.
//! \return Operation results. \sa AUDIO_NV_DSP_RESULT_E
//! \brief  Description:  This function is to add one mode into mode manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_AddMode(
    AUDIO_NV_DSP_MODE_INFO_T* ptMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_RemoveMode(
//!    const char* pucModeName
//!    )
//! \param pucModeName mode name which is needed to remove from this mode manager.
//! \return Operation results. \sa AUDIO_NV_DSP_RESULT_E
//! \brief  Description:  This function is to remove one mode from mode manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_RemoveMode(
    const char* pucModeName
    );

/*****************************************************************************/
//! \fn PUBLIC  AUDIO_NV_DSP_RESULT_E AUDIONVDSP_GetModeParam(
//!    const char* pucModeName, 
//!    AUDIO_NV_DSP_MODE_INFO_T* ptMode
//!    )
//! \param pucModeName Mode Name.
//! \param ptMode [OUT] Address which the mode parameter output.
//! \return Operation resules. \sa AUDIO_NV_DSP_RESULT_E
//! \brief  Description:  This function is to get the named mode information
//!                       from linked list managed by Device Mode Manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC  AUDIO_NV_DSP_RESULT_E AUDIONVDSP_GetModeParam(
    const char* pucModeName, 
    AUDIO_NV_DSP_MODE_INFO_T* ptMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_SetModeParam(
//!    const char* pucModeName, 
//!    AUDIO_NV_DSP_MODE_INFO_T* ptMode
//!    )
//! \param pucModeName Mode Name.
//! \param ptMode [IN] Address which the new mode parameter saved.
//! \return Operation resules. \sa AUDIO_NV_DSP_RESULT_E
//! \brief  Description: This function is used to set mode parameters to the 
//! \          appointed device mode managed by Mode Manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_SetModeParam(
    const char* pucModeName, 
    AUDIO_NV_DSP_MODE_INFO_T* ptMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_GetAudioModeName(
//!    AUDIO_DEVICE_MODE_TYPE_E nv_audio_e
//!    )
//! \param nv_audio_e Mode type.
//! \return mode name. \sa char*
//! \brief  Description:  This function is to get the mode name by mode type.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC char *AUDIONVDSP_GetAudioModeName (
	NV_AUDIO_DSP_CODEC_E dsp_codec_id
	); 

/*****************************************************************************/
//! \fn PUBLIC AUDIO_DEVICE_MODE_TYPE_E AUDIONVDSP_GetAudioModeType(
//!    char *mode_name_ptr
//!    )
//! \param mode name. \sa char*
//! \return nv_audio_e Mode type.
//! \brief  Description:  This function is to get the mode type by mode name.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_DEVICE_MODE_TYPE_E AUDIONVDSP_GetAudioModeType(
	char *mode_name_ptr
	);

/*****************************************************************************/
//! \fn void AUDIONVDSP_GetAudioDspCodecInfoFromRam(uint16 *audio_dsp_info)
//! \param audio_dsp_info indicates the information of dsp codec.
//! \brief  Description:  This function is to get the information of dsp codec.
//! \author Author:  Jian.Chen
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC void AUDIONVDSP_GetAudioDspCodecInfoFromRam(uint16 *audio_dsp_info);

/*****************************************************************************/
//! \fn void AUDIONVDSP_SetAudioDspCodecInfoToRam(uint16 audio_dsp_info)
//! \param audio_dsp_info indicates the information of dsp codec.
//! \brief  Description:  This function is to set the information of dsp codec.
//! \author Author:  Jian.Chen
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC void AUDIONVDSP_SetAudioDspCodecInfoToRam(uint16 audio_dsp_info);
	
/*****************************************************************************/
//! \fn void AUDIONVDSP_GetAudioDspCodecInfoFromFlash(uint16 *audio_dsp_info)
//! \param audio_dsp_info indicates the information of dsp codec.
//! \brief  Description:  This function is to get the information of dsp codec.
//! \author Author:  Jian.Chen
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC void AUDIONVDSP_GetAudioDspCodecInfoFromFlash(uint16 *audio_dsp_info);

/*****************************************************************************/
//! \fn BOOLEAN AUDIONVDSP_SetAudioDspCodecInfoToFlash(uint16 audio_dsp_info)
//! \param audio_dsp_info indicates the information of dsp codec.
//! \brief  Description:  This function is to set the information of dsp codec.
//! \author Author:  Jian.Chen
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDIONVDSP_SetAudioDspCodecInfoToFlash(uint16 audio_dsp_info);

/*****************************************************************************/
//! \fn PUBLIC  AUDIO_NV_DSP_RESULT_E AUDIONVDSP_ReadModeParamFromFlash(
//!    const char* pucModeName, 
//!    AUDIO_NV_DSP_MODE_INFO_T* ptMode
//!    )
//! \param pucModeName device mode name, its max length is not more than 16 bytes.
//! \param ptMode [OUT] Address which the mode parameter output.
//! \return Operation resules. \sa AUDIO_NV_DSP_RESULT_E
//! \brief  Description:  This function is to get the named mode information
//!                       from flash.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC  AUDIO_NV_DSP_RESULT_E AUDIONVDSP_ReadModeParamFromFlash(
    const char* pucModeName, 
    AUDIO_NV_DSP_MODE_INFO_T* ptMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_WriteModeParamToFlash(
//!    const char* pucModeName, 
//!    AUDIO_NV_DSP_MODE_INFO_T* ptMode
//!    )
//! \param pucModeName mode name, its max length is not more than 16 bytes.
//! \param ptMode [IN] Address which the new mode parameter saved.
//! \return Operation resules. \sa AUDIO_NV_DSP_RESULT_E
//! \brief  Description:  This function is to set the named mode information
//!                       to flash.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_NV_DSP_RESULT_E AUDIONVDSP_WriteModeParamToFlash(
	const char* pucModeName, 
    AUDIO_NV_DSP_MODE_INFO_T* ptMode
    );
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif

#endif  // _ADMM_H

// End of admm.h
