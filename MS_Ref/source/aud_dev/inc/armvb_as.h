/******************************************************************************
 ** File Name:      armvb_as.h                                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           01/07/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    Voiceband(ARM Controlled) audioservice adapter layer.* 
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/07/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/
  
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 

#ifndef _ARMVB_AS_H_
#define _ARMVB_AS_H_

#include "audio_api.h"
#include "audio_nv_arm.h"
#ifdef DSP_AUDIO_III
#include "admm_III.h"
#else 
#include "admm_II.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Register voiceband controlled by arm to audio service.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_RegAudioDevice(void);

/*****************************************************************************/
//  Description:    Register voiceband controlled by arm to audio service.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_Record_RegAudioDevice(void);

/*****************************************************************************/
//  Description:    Register linein logic device controlled by arm to play  
//                  from linein channel to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_Linein_RegAudioDevice (void);

/*****************************************************************************/
//  Description:    Register record from linein logic device controlled by arm   
//                  to record from linein to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_LineinAd_RegAudioDevice (void);

/*****************************************************************************/
//  Description:    Register linein logic device controlled by arm to play  
//                  from linein channel to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_FM_RegAudioDevice (void);
PUBLIC HAUDIODEV ARMVB_FM_RegExternalAudioDevice (void);
PUBLIC HAUDIODEV ARMVB_FM_RegBuildInAudioDevice (void);

/*****************************************************************************/
//  Description:    Register record from linein logic device controlled by arm   
//                  to record from linein to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_FMAd_RegAudioDevice (void);
PUBLIC HAUDIODEV ARMVB_FMAd_RegExternalAudioDevice (void);
PUBLIC HAUDIODEV ARMVB_FMAd_RegBuildInAudioDevice (void);

/*****************************************************************************/
//  Description:    ARM VB Sets paras for changes from nv 
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
PUBLIC AUDIO_RESULT_E ARMVB_SetDevModeParam (
    AUDIO_NV_ARM_MODE_INFO_T *ptDevModInfo);

/*****************************************************************************/
//  Description:    ARM VB Sets speaker pa and headset pa.
//  Author:         Johnson.sun
//  Param bEnable: TRUE=set, FALSE=off
//  Note:
//****************************************************************************/
PUBLIC void ARMVB_EnableAmplifier(BOOLEAN bEnable);

/*****************************************************************************/
//  Description:    ARM VB Sets speaker pa and headset pa in recording.
//  Author:         shujing.dong
//  Param bEnable: TRUE=set, FALSE=off
//  Note:
//****************************************************************************/
PUBLIC void ARMVB_RECORD_EnableAmplifier (BOOLEAN bEnable);

//PUBLIC void AUDIO_PMIC_26M_open (uint32 open_flag);
//PUBLIC void AUDIO_PMIC_26M_close (uint32 close_flag);

#ifdef PRODUCT_DRIVER_MTV_ENABLE
/*****************************************************************************/
//  Description:    Register voiceband controlled by MTV to audio service.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV MTVVB_RegAudioDevice(void);
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of armvb_as.h

#endif // End of _ARMVB_AS_H_
