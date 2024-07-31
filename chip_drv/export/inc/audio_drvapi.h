/******************************************************************************
 ** File Name:      audio_drvapi.h                                            *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of audio device. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang        Create.                                   *
 ******************************************************************************/

#ifndef _AUDIO_DRVAPI_H_
#define _AUDIO_DRVAPI_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
enum CODEC_CMD_E{
    AUDIO_CTL_BEGIN         = 0,

    AUDIO_CTL_CODEC_BEGIN,

    AUDIO_CTL_PGA           = AUDIO_CTL_CODEC_BEGIN,
    AUDIO_CTL_SWITCH_DAC,
    AUDIO_CTL_SWITCH_DRV_OUT,
    AUDIO_CTL_SWITCH_DRV_IN,
    AUDIO_GET_SUPPORT_FS,
    AUDIO_UPDTAE_LDO_VOL,
    AUDIO_PA_SETTING,
    AUDIO_HP_UNMUTE,
    
    ///TODO: add here.
    AUDIO_CTL_CODEC_END,
    AUDIO_CODEC_ENUM_MAX    = 0xFFFFFFFF //force this enum use uint32
};

enum DAI_CMD_E{
    AUDIO_CTL_DAI_BEGIN     = AUDIO_CTL_CODEC_END,
    AUDIO_GET_SUPPORT_DATA_FMT =AUDIO_CTL_DAI_BEGIN,
    AUDIO_CTL_SET_DATA_FMT,
    AUDIO_CTL_GET_DATA_FMT,
    
    AUDIO_CTL_DAI_BY_DSP,
    AUDIO_CTL_DAI_BY_ARM,
    
    AUDIO_CTL_SET_BUFFER_DIV,
    AUDIO_CTL_SET_AD_BUFFER_DIV,
    
    AUDIO_CTL_DAI_END,
    AUDIO_CTL_END           =AUDIO_CTL_DAI_END,
    AUDIO_DAI_ENUM_MAX      = 0xFFFFFFFF //force this enum use uint32
};

//enum CODEC_MODE_TYPE_E{
#define AUDIO_SPK           (0x0001)
#define AUDIO_EAR           (0x0002)
#define AUDIO_HP            (0x0004)
#define AUDIO_MIC           (0x0008)
#define AUDIO_LINEIN        (0x0010)
#define AUDIO_HP_MIC        (0x0020)
//};

/**
When play MP3, just use AUDIO_FUN_PLAYBACK .
When use for call, need AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP .
the AUDIO_FUN_DSP special tell DAI switch to DSP control.
Yet, AUDIO_FUN_PLAYBACK stand for DAC path except open HP/PA/EAR what open by IOCTL.
Yet, AUDIO_FUN_CAPTRUE stand for ADC path except select MIC/AI  what select by IOCTL.
**/
//enum AUDIO_FUN_E{
#define AUDIO_FUN_DAC0           (0x80000000)
#define AUDIO_FUN_DAC1           (0x40000000)
#define AUDIO_FUN_PLAYBACK       (AUDIO_FUN_DAC0 | AUDIO_FUN_DAC1)
#define AUDIO_FUN_ADC0           (0x20000000)
#define AUDIO_FUN_ADC1           (0x10000000)
#define AUDIO_FUN_CAPTRUE        (AUDIO_FUN_ADC0 | AUDIO_FUN_ADC1)

#define AUDIO_FUN_DSP            (0x00008000)
#define AUDIO_FUN_LINEIN         (0x00004000)
#define AUDIO_FUN_DSP_CTL_CODEC  (0x00002000)
#define AUDIO_FUN_AD_FROM_DA     (0x00001000)
#define AUDIO_FUN_LINEIN_DIGITAL (0x00000800)   

#define AUDIO_FUN_INT_FM         (0x00000080)
    
#define AUDIO_FUN_CUSTOM1        (0x00000008)
#define AUDIO_FUN_CUSTOM2        (0x00000004)
#define AUDIO_FUN_CUSTOM3        (0x00000002)
#define AUDIO_FUN_CUSTOM4        (0x00000001)
//};
#define AUDIO_FUN_MAX_CNT 28


enum AUDIO_ERR_E{
    AUDIO_HAL_SUCCESS       = 0,
    AUDIO_HAL_ERROR         = -1,
    AUDIO_ERR_ENUM_MAX      = 0xFFFFFFFF //force this enum use uint32
};


/**
AUDIO_DATA_FMT_LL_RR : Like VBC , Left channel and Right channel buffer is independence
                then , Ref give audio data should be independence
AUDIO_DATA_FMT_LR_LR : Like I2S , Left channel and Right channel buffer is mix together
                then , Ref give audio data should be mix together
some platform DMA support fragment move data,will support tow DATA FORMAT
like this AUDIO_DATA_FMT_LL_RR | AUDIO_DATA_FMT_LR_LR .
then Ref can choise one format give data but DO NOT change DATA FORMAT in transmission.
AUDIO_DATA_FMT_MONO  : Mono playback or captrue.
**/
//enum AUDIO_DATA_FORMAT_E{
#define AUDIO_DATA_FMT_LL_RR    (0x00000001)
#define AUDIO_DATA_FMT_LR_LR    (0x00000002)
#define AUDIO_DATA_FMT_MONO     (0x00000004)
///TODO: add FMT here. then MUST modify AUDIO_DATA_FMT_ALL
#define AUDIO_DATA_FMT_ALL      (0x00000007)
//};

//enum AUDIO_CHANNEL_E{
#define AUDIO_CHAN_L            (0x1)
#define AUDIO_CHAN_R            (0x2)
#define AUDIO_ALL_CHAN          (0x8)
//};

#define AUDIO_IS_CHAN_L(chan) ((AUDIO_ALL_CHAN==(chan&0xF)) || (AUDIO_CHAN_L & (chan)) )
#define AUDIO_IS_CHAN_R(chan) ((AUDIO_ALL_CHAN==(chan&0xF)) || (AUDIO_CHAN_R & (chan)) )

#define AUDIO_IS_CHAN_L_ONLY(chan)  ((AUDIO_CHAN_L==(chan&0xF)))
#define AUDIO_IS_CHAN_R_ONLY(chan)  ((AUDIO_CHAN_R==(chan&0xF)))

enum AUDIO_MUTE_E{
    AUDIO_MUTE  = 0,
    AUDIO_UNMUTE,
    AUDIO_NEXT_ENABLED = 0x80000000
};

enum AUDIO_DIRECTION_E{
    AUDIO_PLAYBACK  = 0,
    AUDIO_CAPTRUE,
};


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef struct{
    uint32              id:4;
    uint32              chan:4;
    uint32              level:16;
    uint32              mode:8;
}AUDIO_CTL_PGA_T;

typedef struct{
    uint32              id:4;
    uint32              chan:4;
    uint32              val:16;
    uint32              mode:8;
}AUDIO_CTL_SWITCH_T;

typedef struct{
    uint32              id:4;
    uint32              chan:4;
    uint32              fmt:16;
    uint32              dir:8;
}AUDIO_CTL_DAT_FMT_T;

typedef struct{
    uint32              id:4;
    uint32              chan:4;
    uint32              is_classD:1;
    uint32              is_LDO:1;
    uint32              is_DEMI:1;
    uint32              is_auto_LDO:1;
    uint32              LDO_V_sel:4;
    uint32              DTRI_F_sel:3;
    uint32              RESV:13;
}AUDIO_CTL_PA_T;

typedef union{
    uint32              dwVal;
    AUDIO_CTL_PGA_T     pga;
    AUDIO_CTL_SWITCH_T  sw;
    AUDIO_CTL_DAT_FMT_T dat_fmt;
    AUDIO_CTL_PA_T      PA_Setting;
}AUDIO_CTL_ARG_U;

typedef int32 (*AUDIO_HAL_CALLBACK_FUN)(uint32);


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
// Description :    Audio Driver Init.
//                  include Codec Init, DAI Init etc.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Init(void);

/*****************************************************************************/
// Description :    Audio Driver Open.
//                  include Codec Power On, DAI Enable.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Open(uint32 fun_flg,uint32 fs);

/*****************************************************************************/
// Description :    Audio Driver Close.
//                  include Codec Power Off, DAI Disable.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Close(uint32 fun_flg);

/*****************************************************************************/
// Description :    Audio Driver Ioctl.
//                  include Codec Ioctl, DAI Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Ioctl(uint32 cmd, uint32 arg);


/*****************************************************************************/
// Description :    Audio Driver Set Playback Buffer Info.
//                  
// Return :         AUDIO_HAL_SUCCESS
// Param :          pLHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Left channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO
//                      , the data start point.
//                  pRHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Right channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO 
//                      , will ignore.
//                  size : the max size of pLHead. 
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_DAC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 size);
PUBLIC int32 AUDIO_HAL_DAC_Set_Write_Point(uint32 pWrite);
PUBLIC int32 AUDIO_HAL_DAC_Get_Read_Point(uint32* pRead);

/*****************************************************************************/
// Description :    Audio Driver Will Trigger Playback immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_DAC_Trigger(void);

/*****************************************************************************/
// Description :    Audio Driver Capture Buffer Info.
//                  
// Return :         AUDIO_HAL_SUCCESS
// Param :          pLHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Left channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO
//                      , the data start point.
//                  pRHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Right channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO 
//                      , will ignore.
//                  size : the max size of pLHead. 
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_ADC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 size);
PUBLIC int32 AUDIO_HAL_ADC_Set_Read_Point(uint32 pRead);
PUBLIC int32 AUDIO_HAL_ADC_Get_Write_Point(uint32* pWrite);

/*****************************************************************************/
// Description :    Audio Driver Will Trigger Captrue immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_ADC_Trigger(void);

/*****************************************************************************/
// Description :    Audio Driver Register Playback Callback.
//                  Use to Notify User the Driver fetch data gone.
//                  User can do something like start decode audio data.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Register_Playback(AUDIO_HAL_CALLBACK_FUN pFun);

/*****************************************************************************/
// Description :    Audio Driver Register Capture Callback.
//                  Use to Notify User for fetch data please.
//                  User should be move audio data ASAP.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Register_Capture(AUDIO_HAL_CALLBACK_FUN pFun);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_AUDIO_DRVAPI_H_
// End
