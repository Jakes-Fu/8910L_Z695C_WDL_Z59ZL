/******************************************************************************
 ** File Name:      armvb_as.c                                                                *
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


#include "ms_ref_aud_dev_trc.h"
#include "os_api.h"
#include "audio_config.h"
#include "vb_drv.h"
#include "aud_gen.h"
#include "adm.h"
#include "armvb_as.h"

#include "sc_reg.h"

#include "agc_exp.h"
#include "aud_proc_exp.h"
#include "aud_enha_exp.h"
#include "audio_nv_arm.h"
#include "admm_III.h"
#include "audio_drvapi.h"
#include "track_mixer.h"
#include "fm_drv.h"
#include "deepsleep_drvapi.h"
#ifdef AUD_ARM_RECORD_EXP
#include "aud_arm_record_exp.h"
#endif
#include "pmic_26M_clock.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
#ifndef __arm
#ifndef __MODULE__
#define __MODULE__ "armvb_as.c"
#endif
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define VB_MAX_DA_VALUE (32767)
#define VB_MIN_DA_VALUE (-32768) 
#define TMP_FIFO_SIZE         (160)    //half-word (8*1024/4)//
#define TMP_FIFO_SIZE_MAX    (160*6)  //half-word (7*1024)//
#define VB_CAPTURE_TRANS_SIZE    (1*TMP_FIFO_SIZE)
#define MULTITRACK_BUFFER_SIZE  (8*1024) 
//this value is related with nv configuration, and is note permitted to change at random.
#define ARMVB_LINEIN_RESERVER_VOL_LEVEL 15//(0->14, 0:MUTE, 1->14 volume level 1->14) 
#define ARMVB_LINEIN_VOL_LEVEL_INDEX_BASE  18
#define ARMVB_RECORD_NV_RESERVE     46
#define ARMVB_LINEINAD_NV_RESERVE   47
#define ARMVB_INNER_PA_GAIN_RESERVE  45
#define ARMVB_FM_INNER_PA_GAIN_RESERVE  48

#define VB_ARMVB_PA_LDO_V_SEL_MASK  0x000F
#define VB_ARMVB_PA_DTRI_F_SEL_MASK  0x0070
#define VB_ARMVB_PA_DTRI_F_SEL_SHIFT  4

#define ARMVB_USE_DM_TONE_INTERFACE
#define ARMVB_USE_PCIM_26M_CLOCK
#define ARMVB_USE_CP_AUDIO_INTERFACE
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef struct
{
     uint16 *data_ptr;
     uint16 *data_ptr_r;
    uint32 size;
    uint32 in;
    uint32 out;
}VB_ARM_RECORD_BUF;

typedef struct
{
    uint32 in;     /*!<Data in pointer.*/
    uint32 out;    /*!<Data out pointer.*/
    uint32 size;  /*!<Total data count.*/
    int16* data_buffer_ptr; /*!<Left channel buffer pointer.*/
    int16* data_buffer_ptr_r;   /*!<Right channel buffer pointer.*/
}VB_ARM_PLAYBACK_BUF;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
//lint -e551
LOCAL AUDIO_DEVICE_HANDLE_T *g_armvb_device_ptr;
LOCAL AUDIO_DEVICE_HANDLE_T *g_armvb_linein_device_ptr;
LOCAL AUDIO_DEVICE_HANDLE_T *g_armvb_fm_device_ptr;
LOCAL AUDIO_DEVICE_HANDLE_T *g_armvb_dualchn_device_ptr;
#ifdef PRODUCT_DRIVER_MTV_ENABLE
LOCAL AUDIO_DEVICE_HANDLE_T *g_mtvvb_device_ptr;
#endif
VB_ARM_PLAYBACK_BUF s_armvb_output_buf;
int32 s_vb_da0_buffer_multitrack[MULTITRACK_BUFFER_SIZE];
/*LOCAL int32 s_vb_da1_buffer_multitrack[MULTITRACK_BUFFER_SIZE];*/

volatile BOOLEAN  s_is_vb_mute = SCI_FALSE;
LOCAL volatile BOOLEAN  s_is_vb_linein_mute = SCI_FALSE;
volatile BOOLEAN  g_is_vb_linein_ad_mute = SCI_FALSE; 
LOCAL volatile BOOLEAN s_is_vb_stop = SCI_FALSE;

LOCAL BOOLEAN s_b_armvb_linein_playing = SCI_FALSE;

#define ARMVB_RECORD_OUTPUT_COUNT_MAX 2
LOCAL uint8 s_vb_volumn = 0;
LOCAL uint8 s_vb_linein_volumn = 0;
BOOLEAN s_b_armvb_recording = SCI_FALSE;
LOCAL BOOLEAN s_b_linein_ad_playing = SCI_FALSE;
LOCAL uint8 s_vb_record_volumn = 0;
volatile uint32 s_record_callback_count = 0;
volatile uint32 record_callback_FrameCount = 0;
uint32 s_ad_read_size = 0;
LOCAL uint32 s_armvb_hw_per_read = 0;   //half-word
VB_ARM_RECORD_BUF s_vb_arm_record_buf;
LOCAL volatile ADM_RECCALLBACK s_armrecord_callback[ARMVB_RECORD_OUTPUT_COUNT_MAX] = {0};  // default NULL
LOCAL uint8 sucDolphinLineinGain[10]= {0x1F,0x1F,0x0F,0x0A,0x08,0x06,0x04,0x02,0x01,0x00};
uint8 *g_zsp_share_memory_addr = NULL;
uint32 g_zsp_share_memory_lenth = 0;

//#define DECODE_IN_ZSP

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    ARMVB RECORD logic device from linein set device mode.
//  Author:
//  Note:
//****************************************************************************/
LOCAL void ARMVB_LineinAd_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode);
LOCAL int32 ARMVB_DataFmt(uint32 id,uint32 fmt, uint32 dir);
LOCAL AUDIO_RESULT_E ARMVB_SetNotchPara();

#ifdef FM_VBC
LOCAL AUDIO_RESULT_E ARMVB_FM_PreClose(void *ptParam);
LOCAL AUDIO_RESULT_E ARMVB_FM_PostOpen(void *ptParam);
#endif
/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                              *
 **---------------------------------------------------------------------------*/
PUBLIC char * _ARMVB_ArmvbNv_GetName(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode)
{
    NV_AUDIO_ARM_E arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
    switch(aud_dev_mode)
    {
        case      AUDIO_DEVICE_MODE_HANDHOLD:
        arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
        break;
        case      AUDIO_DEVICE_MODE_HANDFREE:
        arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
        break;
        case      AUDIO_DEVICE_MODE_EARPHONE:
        arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
        break;
        case      AUDIO_DEVICE_MODE_EARFREE:
        arm_nv_id = NV_AUDIO_ARM_EARFREE_E;
        break;
        case      AUDIO_DEVICE_MODE_TVOUT:
        arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
        break;
        case      AUDIO_DEVICE_MODE_BLUEPHONE:
        arm_nv_id = NV_AUDIO_ARM_BTHS_E;
        break;
        default:
        arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
        break;
    }
    return AUDIONVARM_GetAudioModeNameById(arm_nv_id);
}


/*****************************************************************************/
//  Description:    adjust the address of left channel and right channel to output .
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL int32 ARMVB_DrvOutSwitch(uint32 id,uint32 eMode,BOOLEAN is_on)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_SWITCH_T *aud_ctl_switch_ptr = PNULL;
    
    aud_ctl_switch_ptr = (AUDIO_CTL_SWITCH_T *)AUDIO_ALLOC(sizeof(AUDIO_CTL_SWITCH_T));
    if(PNULL==aud_ctl_switch_ptr)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_DrvOutSwitch, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_140_112_2_18_1_2_10_0,(uint8*)"d",sizeof(AUDIO_CTL_SWITCH_T));
        
        return AUDIO_HAL_ERROR;
    }
    aud_ctl_switch_ptr->id = id;
    aud_ctl_switch_ptr->chan = AUDIO_ALL_CHAN;
    aud_ctl_switch_ptr->mode = eMode;
    aud_ctl_switch_ptr->val = is_on;
    
    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DRV_OUT, (uint32)aud_ctl_switch_ptr);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_DrvOutSwitch, ioctl fail, eMode: %d, is_on: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_150_112_2_18_1_2_10_1,(uint8*)"dd",aud_ctl_switch_ptr->mode,aud_ctl_switch_ptr->val);
        SCI_FREE(aud_ctl_switch_ptr);
        
        return AUDIO_HAL_ERROR;
    }
    
    SCI_FREE(aud_ctl_switch_ptr);
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 ARMVB_DrvInSwitch(uint32 id,uint32 eMode,BOOLEAN is_on)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_SWITCH_T *aud_ctl_switch_ptr = PNULL;
    
    aud_ctl_switch_ptr = (AUDIO_CTL_SWITCH_T *)AUDIO_ALLOC(sizeof(AUDIO_CTL_SWITCH_T));
    if(PNULL==aud_ctl_switch_ptr)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_DrvInSwitch, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_165_112_2_18_1_2_10_2,(uint8*)"d",sizeof(AUDIO_CTL_SWITCH_T));
        return AUDIO_HAL_ERROR;
    }
    aud_ctl_switch_ptr->id = id;
    aud_ctl_switch_ptr->chan = AUDIO_ALL_CHAN;
    aud_ctl_switch_ptr->mode = eMode;
    aud_ctl_switch_ptr->val = is_on;
    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DRV_IN, (uint32)aud_ctl_switch_ptr);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_DrvInSwitch, ioctl fail, eMode: %d, is_on: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_175_112_2_18_1_2_10_3,(uint8*)"dd",aud_ctl_switch_ptr->mode,aud_ctl_switch_ptr->val);
        SCI_FREE(aud_ctl_switch_ptr);
        return AUDIO_HAL_ERROR;
    }
    SCI_FREE(aud_ctl_switch_ptr);    
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 ARMVB_SetDacBufInfo(void *pLHead,void *pRHead,uint32 cnt)
{
    return AUDIO_HAL_DAC_Set_Buf_Info(pLHead,pRHead,cnt);
}

LOCAL int32 ARMVB_SetDacWritePoint(uint32 pWrite)
{
    return AUDIO_HAL_DAC_Set_Write_Point(pWrite);
}

LOCAL int32 ARMVB_GetDacReadPoint(uint32* pRead)
{
    return AUDIO_HAL_DAC_Get_Read_Point(pRead);
}

LOCAL int32 ARMVB_SetPgaLevel(uint32 id,uint32 eChannel,uint32 pga_level,uint32 eCodecMode)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_PGA_T *aud_ctl_pga_ptr = PNULL;
    
    aud_ctl_pga_ptr = (AUDIO_CTL_PGA_T *)AUDIO_ALLOC(sizeof(AUDIO_CTL_PGA_T));
    if(PNULL==aud_ctl_pga_ptr)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_SetPgaLevel, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_207_112_2_18_1_2_10_4,(uint8*)"d",sizeof(AUDIO_CTL_PGA_T));
        return AUDIO_HAL_ERROR;
    }
    aud_ctl_pga_ptr->id = id;
    aud_ctl_pga_ptr->chan = eChannel;
    aud_ctl_pga_ptr->level = pga_level;
    aud_ctl_pga_ptr->mode = eCodecMode;
    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_PGA, (uint32)aud_ctl_pga_ptr);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_SetPgaLevel, ioctl fail, pga_level: %d eCodecMode: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_217_112_2_18_1_2_10_5,(uint8*)"dd",aud_ctl_pga_ptr->level,aud_ctl_pga_ptr->mode);
        SCI_FREE(aud_ctl_pga_ptr);
        return AUDIO_HAL_ERROR;
    }
    SCI_FREE(aud_ctl_pga_ptr);    
    return AUDIO_HAL_SUCCESS;
}

LOCAL uint32 ARMVB_GetSupportSamplerate(uint32 uiSampleRate)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    aud_result = AUDIO_HAL_Ioctl(AUDIO_GET_SUPPORT_FS, (uint32)&uiSampleRate);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_GetSupportSamplerate, ioctl fail, uiSampleRate: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_231_112_2_18_1_2_10_6,(uint8*)"d",uiSampleRate);
        return 0;
    }
    return uiSampleRate;
}

LOCAL void ARMVB_CalcBufAddr(int16 **const ppsLeftData,int16 **const ppsRightData,uint32 uiDataOffset)
{
    *ppsLeftData = (int16 *) (& (s_armvb_output_buf.data_buffer_ptr[uiDataOffset]));
    *ppsRightData = (int16 *) (& (s_armvb_output_buf.data_buffer_ptr_r[uiDataOffset]));
}

typedef unsigned int uint32_t;
#include "bt_abs.h"
#include "rpc_aud.h"
#include "rpc_aud_api.h"
#include "load_modem.h"
#include "hal_aud_zsp.h"
HAL_AIF_CH_NB_T g_channelNb = HAL_AIF_MONO;
SND_ITF_T g_cur_itf = SND_ITF_LOUD_SPEAKER;
HAL_AIF_STREAM_T g_aif_stream;
HAL_AIF_STREAM_T g_dual_record_aif_stream;
AUD_LEVEL_T g_aud_level_cfg = {0};
VOIS_AUDIO_CFG_T g_audio_cfg;
SND_BT_WORK_MODE_T g_btworkmode = SND_BT_WORK_MODE_NO;
UINT32 g_NRECflag = 0; // 0/1: BT has NREC function. 2: Audio driver remove depop delay
UINT32 tts_play_number = 0; // reuse BT NREC Flag for add depop delay in audio driver
SND_SPK_LEVEL_T g_volbeforemute = SND_SPK_MUTE;
uint32 g_uiCurrentSamplerate = 0;
uint32 g_shakingMode = 0;
int32 g_audio_dev_used = 0;
INT32 g_need_add_zsp_vibrate = 0;
SCI_SEMAPHORE_PTR audio_armvb_semaphore = SCI_NULL;
AUD_AP2CP_PARA_T g_zsp_para = {0};
uint32 g_toneMode = 0;
AUD_SPKPA_TYPE_T g_spkpatype = AUD_INPUT_TYPE_CLASSD;

PUBLIC void A2DP_GetAudioData(int16 *left_data, int16 *right_data, uint32 data_len);
PUBLIC uint32 A2DP_GetAudioDataCount(void);

/*begin xin.xu1 20190409 for bugid 1032174*/
LOCAL SCI_TIMER_PTR  pSwitchDeviceTimerHandle = SCI_NULL;
LOCAL BOOLEAN bSwitchDeviceFlag = SCI_FALSE;

LOCAL void SwitchDeviceTimerCallback(uint32 arg)
{
    AUD_ERR_T aud_ret = 0;

    SCI_TRACE_LOW("SwitchDeviceTimerCallback ");
    bSwitchDeviceFlag = SCI_FALSE;
}

LOCAL void SwitchDeviceStartTimer(void)
{
    bSwitchDeviceFlag = SCI_TRUE;
    SCI_SLEEP(500);
    
    SCI_TRACE_LOW("SwitchDeviceTimer start");
    if(SCI_NULL == pSwitchDeviceTimerHandle)
    {
        pSwitchDeviceTimerHandle = SCI_CreateTimer ("SwitchDeviceTimer", SwitchDeviceTimerCallback, 0, 500, SCI_NO_ACTIVATE);
        SCI_ASSERT(pSwitchDeviceTimerHandle);
    }
    if (!SCI_IsTimerActive(pSwitchDeviceTimerHandle))
    {
        SCI_ChangeTimer (pSwitchDeviceTimerHandle, SwitchDeviceTimerCallback, 500);
        SCI_ActiveTimer(pSwitchDeviceTimerHandle);
    }
    else
    {
        SCI_DeactiveTimer(pSwitchDeviceTimerHandle);
    }
}

PUBLIC BOOLEAN SwitchDeviceTimerGetFlag(void)
{
    return bSwitchDeviceFlag;
}
/*end xin.xu1 20190409 for bugid 1032174*/

/*****************************************************************************/
//  Description:    RingBufferIsFull
//  Author:           xin.xu1
//  Note:
//****************************************************************************/
BOOLEAN RingBufferIsFull(AUDIO_INPUT_PARAM_T *pAudInputPara)
{
    SCI_ASSERT(pAudInputPara != NULL);

    return (pAudInputPara->writeOffset == (pAudInputPara->inLenth + pAudInputPara->readOffset - 4)%pAudInputPara->inLenth);
}

/*****************************************************************************/
//  Description:    RingBufferIsEmpty
//  Author:           xin.xu1
//  Note:
//****************************************************************************/
BOOLEAN RingBufferIsEmpty(AUDIO_INPUT_PARAM_T *pAudInputPara)
{
    SCI_ASSERT(pAudInputPara != NULL);

    return (pAudInputPara->writeOffset == pAudInputPara->readOffset );
}

/*****************************************************************************/
//  Description:    RingBufferGetWriteBufferLen
//  Author:           xin.xu1
//  Note:
//****************************************************************************/
UINT32 RingBufferGetWriteBufferLen(AUDIO_INPUT_PARAM_T *pAudInputPara, UINT16 usInputLen)
{
    UINT16 usLen = 0;
    
    SCI_ASSERT(pAudInputPara != NULL);

    if(RingBufferIsEmpty(pAudInputPara))
    {
        return (pAudInputPara->inLenth - 4) > usInputLen? usInputLen:(pAudInputPara->inLenth - 4) ;
    }
    if(RingBufferIsFull(pAudInputPara))
    {
        return 0;
    }

    if(pAudInputPara->writeOffset > pAudInputPara->readOffset)
    {
        usLen = pAudInputPara->inLenth - (pAudInputPara->writeOffset - pAudInputPara->readOffset) -4;
    }
    else
    {
        usLen = pAudInputPara->readOffset - pAudInputPara->writeOffset - 4;
    }

    return usLen >  usInputLen? usInputLen:usLen;
}

/*****************************************************************************/
//  Description:    RingBufferGetWriteBufferLen
//  Author:           xin.xu1
//  Note:
//****************************************************************************/
UINT16 RingBufferWriteBuffer(AUDIO_INPUT_PARAM_T *pAudInputPara, INT16 *psOutBuffer, INT32 *psInBuffer, UINT16 usInBufferLen, UINT16 usActualDataLen)
{
    UINT32 uiWriteBufferLen = 0;
    UINT16 i = 0;
    UINT16 usWriteOffset = 0;
    
    SCI_ASSERT(pAudInputPara != NULL);
    SCI_ASSERT(psOutBuffer != NULL);
    SCI_ASSERT(psInBuffer != NULL);

    uiWriteBufferLen = RingBufferGetWriteBufferLen(pAudInputPara,usInBufferLen);
    if(0 == uiWriteBufferLen)
    {
        return 0;
    }

    usWriteOffset = pAudInputPara->writeOffset;

    for(i = 0; i < uiWriteBufferLen; i+=2)
    {
        psOutBuffer[((usWriteOffset+i) % pAudInputPara->inLenth)/2] = (INT16)psInBuffer[i/2];
    }

    pAudInputPara->writeOffset = (pAudInputPara->writeOffset+usActualDataLen) % pAudInputPara->inLenth; // Set writeoffset to actual data lenth

    return uiWriteBufferLen;

}

void ARMVB_PLAYBACK_Callback (void)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = NULL;
    uint32 data_count;
    uint32 copy_size = 0;
    int channelNb = 0;
    uint32 tick_cnt;
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d] 1time:[%d][%d]in", __FILE__, __FUNCTION__, __LINE__,tick_cnt/1000, tick_cnt%1000);
    
    zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    
    if (g_channelNb == HAL_AIF_MONO)
    {
        channelNb = 0;
    }
    else
    {
        channelNb = 1;
    }

    data_count = RingBufferGetWriteBufferLen(&zsp_sharemem->audInPara, zsp_sharemem->audInPara.inLenth);
    SCI_TRACE_LOW("[xx] file[%s] func[%s] line[%d] data_count = %d", __FILE__, __FUNCTION__, __LINE__,data_count);

    copy_size = _Track_Mixer(g_armvb_device_ptr->ptTrackList,
                                            s_vb_da0_buffer_multitrack, SCI_NULL/*s_vb_da1_buffer_multitrack*/,
                                            SCI_NULL, SCI_NULL,  data_count/2,   1,  channelNb,  s_is_vb_mute);
    SCI_TRACE_LOW("[xx] file[%s] func[%s] line[%d] copy_size = %d", __FILE__, __FUNCTION__, __LINE__,copy_size);
    RingBufferWriteBuffer(&zsp_sharemem->audInPara, 
                                        zsp_sharemem->audInput, 
                                        s_vb_da0_buffer_multitrack, data_count, (copy_size * 2)); // set data_count data for input 0 to enough

    s_armvb_output_buf.in += copy_size*2;
    if (copy_size < (data_count/2))
    {
        if(_Track_Mixer_End(g_armvb_device_ptr->ptTrackList))
        {
            zsp_sharemem->audInPara.fileEndFlag = 1;
        }
    }

    tick_cnt = SCI_GetTickCount();
}
LOCAL void ARMVB_SetDepopMute(
    BOOLEAN is_mute
)
{
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDepopMute ", __LINE__);
    return;
}
/*****************************************************************************/
//  Description:    Set Arm VB Downlink Mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void ARMVB_SetDownlinkMute (
    BOOLEAN is_mute
)
{    
    AUD_ERR_T aud_ret = 0;
    s_is_vb_mute = is_mute;
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDownlinkMute is_mute:0x%x", __LINE__, is_mute);
    if (TRUE == is_mute)
    {
        g_volbeforemute = g_aud_level_cfg.spkLevel;
        g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
    }
    else
    {
        g_aud_level_cfg.spkLevel = g_volbeforemute;
    }
    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_MUSIC;
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    aud_ret = aud_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDownlinkMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#else
    aud_ret = DM_AudSetup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDownlinkMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
#else
    aud_ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDownlinkMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description:    ARMVB logic device set device volume.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_SetVolumeType (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
#if 0
#endif
    return AUDIO_NO_ERROR;
}

LOCAL int32 ARMVB_SetInnerPA(uint16 pa_l_setting)
{
    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
//  Description:    ARMVB logic device set device mode.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void ARMVB_TestModeStart_Local (AUDIO_DEVICE_MODE_TYPE_E eMode, AUD_INPUT_TYPE_T input_type, uint32 uiVol, BOOL start,AUD_TEST_VOICE_MODE_T voismode)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ERR_T aud_ret = AUD_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    SND_ITF_T snd_cur_itf = SND_ITF_NONE;
    VOIS_AUDIO_CFG_T audio_cfg;
    AUD_LEVEL_T aud_level_cfg;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    HAL_AIF_STREAM_T hal_aif_stream;
    AUD_TEST_T onoff;
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_TestModeStart_Direct eMode:0x%x, input_type:0x%x, uiVol:0x%x, start:0x%x,voismode:%d", __LINE__, eMode, input_type, uiVol, start,voismode);

   	
    switch(eMode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        cur_itf = SND_ITF_RECEIVER;
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        cur_itf = SND_ITF_LOUD_SPEAKER;
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        cur_itf = SND_ITF_EAR_PIECE;
        break;
        case AUDIO_DEVICE_MODE_EARFREE:
        cur_itf = SND_ITF_LOUD_SPEAKER;
        break;
        case AUDIO_DEVICE_MODE_TVOUT:
        cur_itf = SND_ITF_TV;
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        cur_itf = SND_ITF_BLUETOOTH;
        break;

        default:
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_TestModeStart_Direct eMode is [0x%x] WRONG!!!", __LINE__, eMode);
        break;
    }
    audio_cfg.spkLevel = (AUD_SPK_LEVEL_T)uiVol;
    audio_cfg.micLevel = AUD_MIC_ENABLE;
    audio_cfg.sideLevel = AUD_SIDE_MUTE;
    audio_cfg.toneLevel = AUD_TONE_0dB;
    audio_cfg.encMute = 0;
    audio_cfg.decMute = 0;
    //
    device_cfg_ext.inputType = input_type;
    device_cfg_ext.inputCircuityType = AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath = AUD_INPUT_PATH1;
    device_cfg_ext.linePath = AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
    //
    snd_cur_itf = (SND_ITF_T)cur_itf;
    //
    hal_aif_stream.startAddress = NULL;
    hal_aif_stream.length = 0;
    hal_aif_stream.sampleRate = HAL_AIF_FREQ_8000HZ;
    hal_aif_stream.channelNb = HAL_AIF_MONO;
    hal_aif_stream.voiceQuality = 0;
    hal_aif_stream.playSyncWithRecord = 0;
    hal_aif_stream.halfHandler = NULL;
    hal_aif_stream.endHandler = NULL;
    //
    aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)uiVol;
    aud_level_cfg.micLevel = SND_MIC_ENABLE;
    aud_level_cfg.sideLevel = SND_SIDE_MUTE;
    aud_level_cfg.toneLevel = SND_TONE_0DB;
    aud_level_cfg.appMode = SND_APP_MODE_VOICE;
    //
    if (start ==  TRUE)
    {
        onoff = AUD_TEST_SIDE_TEST;
    }
    else
    {
        onoff = AUD_TEST_NO;
    }
#ifdef ARMVB_USE_PCIM_26M_CLOCK
    if (start ==  TRUE)
    {
        //PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
        AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_LocalTest);
    }
#endif
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    vois_ret = vois_SetCfg(cur_itf, &audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_TestModeStart_Direct vois_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#else
    vois_ret = DM_VoisSetCfg(cur_itf, &audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_TestModeStart_Direct DM_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    aud_ret = vois_TestModeSetup(snd_cur_itf, &hal_aif_stream, &aud_level_cfg, onoff, voismode);
    if (VOIS_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_TestModeStart_Direct vois_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#else
    aud_ret = DM_VoisTestModeSetup(snd_cur_itf, &hal_aif_stream, &aud_level_cfg, onoff, voismode);
    if (VOIS_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_TestModeStart_Direct DM_VoisTestModeSetup ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
#else
    vois_ret = AUDIO_CP_VoisSetCfg(cur_itf, &audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_TestModeStart AUDIO_CP_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
    aud_ret = AUDIO_CP_TestModeSet(snd_cur_itf, &hal_aif_stream, &aud_level_cfg, onoff, voismode);
    if (VOIS_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_TestModeStart AUDIO_CP_TestModeSet ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
#ifdef ARMVB_USE_PCIM_26M_CLOCK
    if (start !=  TRUE)
    {
        //PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
        AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_LocalTest);
    }
#endif
    return;
}
void ARMVB_TestModeStart_Direct (AUDIO_DEVICE_MODE_TYPE_E eMode, AUD_INPUT_TYPE_T input_type, uint32 uiVol, BOOL start)
{
    ARMVB_TestModeStart_Local(eMode,input_type,uiVol,start,AUD_TEST_VOICE_MODE_NB);
}

void ARMVB_TestModeStart_Direct_Pcm (AUDIO_DEVICE_MODE_TYPE_E eMode, AUD_INPUT_TYPE_T input_type, uint32 uiVol, BOOL start)
{
    ARMVB_TestModeStart_Local(eMode,input_type,uiVol,start,AUD_TEST_VOICE_MODE_PCM);
}


void ARMVB_SetShakingMode(BOOL shaking)
{
    SCI_TRACE_LOW("line[%d] ARMVB_SetShakingMode This function do nothing");
    if (TRUE == shaking)
    {
        g_shakingMode = 0; // No need any more for 1
    }
    else
    {
        g_shakingMode = 0;
    }
    AUDIO_CP_SetSharkingMode(g_shakingMode);
    return;
}

void ARMVB_SetTTSPlayNumberFlag(UINT32 flag)
{
    tts_play_number = flag;
    return;
}

void ARMVB_SetToneMode(UINT32 isToneMode)
{
    g_toneMode = isToneMode;
    
    SCI_TRACE_LOW("zgt line[%d] ARMVB_SetToneMode isToneMode:0x%x ", __LINE__, isToneMode);
    //AUDIO_CP_SetToneMode(isToneMode);
    return;
}

void ARMVB_SetZspVibratePara_Direct(BOOL shaking)
{
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetZspVibratePara_Direct shaking:%d", __LINE__, shaking);
    if (TRUE == shaking)
    {
        //ARMVB_SetShakingMode(TRUE);
        g_need_add_zsp_vibrate = 1;
    }
    else
    {
        //ARMVB_SetShakingMode(FALSE);
        g_need_add_zsp_vibrate = 0;
    }
    return;
}

void ARMVB_SetCompositionofRingandVibFlag(BOOL composition)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetCompositionofRingandVibFlag composition:%d", __LINE__, composition);
    if (TRUE == composition)
    {
        zsp_sharemem->audInPara.musicVibFlag = 1;
    }
    else
    {
        zsp_sharemem->audInPara.musicVibFlag = 0;
    }
    return;
}
void ARMVB_SetBtWorkMode_Direct (BT_HFG_WB_FLAG *btHfgWB)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetBtWorkMode_Direct btHfgWB:[0x%x][0x%x]", __LINE__, btHfgWB->is_wb_call_flag, btHfgWB->is_nrec_flag);
    //g_cur_itf = SND_ITF_BLUETOOTH;
    if (btHfgWB->is_wb_call_flag == TRUE)
    {
        g_btworkmode = SND_BT_WORK_MODE_WB;
    }
    else
    {
        g_btworkmode = SND_BT_WORK_MODE_NB;
    }
    //cur_itf = (AUD_ITF_T)g_cur_itf;
    cur_itf =(AUD_ITF_T)SND_ITF_BLUETOOTH;
    g_audio_cfg.spkLevel = g_aud_level_cfg.spkLevel;
    g_audio_cfg.micLevel = AUD_MIC_ENABLE;
    g_audio_cfg.sideLevel = AUD_SIDE_MUTE;
    g_audio_cfg.toneLevel = AUD_TONE_0dB;
    g_audio_cfg.encMute = 0;
    g_audio_cfg.decMute = 0;
    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType = AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath = AUD_INPUT_PATH1;
    device_cfg_ext.linePath = AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
    if (btHfgWB->is_nrec_flag == TRUE)
    {
        g_NRECflag = 1;
    }
    else
    {
        g_NRECflag = 0;
    }
#if 0
    vois_SetBtNRECFlag(g_NRECflag);
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    vois_ret = vois_SetCfg(g_cur_itf, &g_audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode_Direct vois_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetBtWorkMode_Direct vois_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#else
    vois_ret = DM_VoisSetCfg(g_cur_itf, &g_audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetBtWorkMode_Direct DM_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
#else
    AUDIO_CP_SetBtNRECFlag(g_NRECflag);
    vois_ret = AUDIO_CP_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetBtWorkMode_Direct DM_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
    return;
}

LOCAL void ARMVB_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    AUD_ERR_T aud_ret = 0;

    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetVolume uiVol:0x%x eAppType:0x%x", __LINE__, uiVol, eAppType);
    g_aud_level_cfg.spkLevel = uiVol;
    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_MUSIC;
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    aud_ret = aud_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#else
    aud_ret = DM_AudSetup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
#else
    aud_ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
    return;
}


/*****************************************************************************/
//  Description:    ARMVB logic device set device mode.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void ARMVB_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;

    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode current mode:0x%x, new eMode:0x%x", __LINE__, g_cur_itf,eMode);
    /*xin.xu1 20190409 for bugid 1032174*/
    SwitchDeviceStartTimer();
    if (AUDIO_DEVICE_MODE_BLUEPHONE == g_cur_itf)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioClose");
        //A2DP_audioClose();
    }
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call aud_StreamStop faile", __LINE__);
    }
#else
    ret = DM_AudStreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call DM_AudStreamStop faile", __LINE__);
    }
#endif

#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStop();
#else
    hal_ret = hal_zspMusicPlayStop();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("line[%d] ARMVB_SetDevMode call hal_zspMusicPlayStop faile", __LINE__);
    }
#endif
#else
    ret = AUDIO_CP_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call AUDIO_CP_StreamStop faile", __LINE__);
    }
    hal_ret = AUDIO_ZSP_STOP(&g_zsp_para);
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("line[%d] ARMVB_SetDevMode call hal_zspMusicPlayStop faile", __LINE__);
    }
#endif
    SCI_MEMSET(zsp_sharemem->Output.audOutput, 0, AUDIO_OUTPUT_BUF_SIZE);
    
    
    switch(eMode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;

        default:
            break;
    }
    if (AUDIO_DEVICE_MODE_BLUEPHONE ==eMode)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioStart");
        //A2DP_audioStart(g_uiCurrentSamplerate, A2DP_GetAudioData, A2DP_GetAudioDataCount);
    }
#if 0
#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStart();
#else
    hal_ret = hal_zspMusicPlayStart();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[zgt] line[%d] ARMVB_SetDevMode call hal_zspMusicPlayStart faile ret:0x%x", __LINE__, hal_ret);
    }
#endif
#if 1 // vibration mode
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode g_shakingMode:0x%x", __LINE__, g_shakingMode);
    aud_SetSharkingMode(g_shakingMode);
#else
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode g_shakingMode:0x%x", __LINE__, g_shakingMode);
    DM_AudSetSharkingMode(g_shakingMode);
#endif
    g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call aud_StreamStart faile ret:0x%x", __LINE__, ret);
    }
#else
    ret = DM_AudStreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call DM_AudStreamStart faile ret:0x%x", __LINE__, ret);
    }
#endif
#else
    hal_ret = AUDIO_ZSP_START(&g_zsp_para);
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[zgt] line[%d] ARMVB_SetDevMode call AUDIO_ZSP_START faile ret:0x%x", __LINE__, hal_ret);
    }
    AUDIO_CP_SetSharkingMode(g_shakingMode);
    ret = AUDIO_CP_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call AUDIO_CP_StreamStart faile ret:0x%x", __LINE__, ret);
    }
    if (AUDIO_DEVICE_MODE_EARFREE == eMode)
    {
        g_cur_itf = SND_ITF_LOUD_SPEAKER;
        ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
        if (0 != ret)
        {
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode call AUDIO_CP_Setup faile ret:0x%x", __LINE__, ret);
        }
    }
    SCI_TRACE_LOW("line[%d] ARMVB_SetDevMode done eMode:0x%x", __LINE__, eMode);
#endif
    return;
}

/*****************************************************************************/
//  Description:    get appropriate output samplerate.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL uint32 ARMVB_GetAppropriateOutputSamplerate (uint32 uiSampleRate)
{
    uint32 sr;

    if (uiSampleRate < 8800)
    {
        sr = 8000;
    }
    else if (uiSampleRate < 11512)
    {
        sr = 11025;
    }
    else if (uiSampleRate  < 14000)
    {
        sr = 12000;
    }
    else if (uiSampleRate < 19025)
    {
        sr = 16000;
    }
    else if (uiSampleRate < 23025)
    {
        sr = 22050;
    }
    else if (uiSampleRate < 28000)
    {
        sr = 24000;
    }
    else if (uiSampleRate < 38050)
    {
        sr = 32000;
    }
    else if (uiSampleRate < 46050)
    {
        sr = 44100;
    }
    else if (uiSampleRate < 72000)
    {
        sr = 48000;
    }
    else if (uiSampleRate <= 96000)
    {
        sr = 96000;
    }
    else
    {
        sr = 8000;
    }
    if ((g_need_add_zsp_vibrate != 0) && (g_uiCurrentSamplerate == 0))
    {
        SCI_TRACE_LOW("[%s] because need add zsp vibrate, so set sampleRate to [44100]", __FUNCTION__);
        sr = 44100;
    }
    // judge playing music or not
    if (g_uiCurrentSamplerate != 0)
    {
        SCI_TRACE_LOW("[%s] because playing music, so set sampleRate [%d]", __FUNCTION__, g_uiCurrentSamplerate);
        sr = g_uiCurrentSamplerate;
    }
    SCI_TRACE_LOW("[%s] sampleRate in=%d out=%d", __FUNCTION__, uiSampleRate,sr);
    return sr;
}

LOCAL uint32 ARMVB_Record_GetAppropriateOutputSamplerate (uint32 uiSampleRate)
{
    if(uiSampleRate==8000)
    {
        return uiSampleRate;
    }    
    else if(uiSampleRate==11025)
    {
        return uiSampleRate;
    }
    else if(uiSampleRate==12000)
    {
        return uiSampleRate;
    }
    else if(uiSampleRate==16000)
    {
        return uiSampleRate;
    }
    else if(uiSampleRate==22050)
    {
        return uiSampleRate;
    }
    else if(uiSampleRate==24000)
    {
        return uiSampleRate;
    }
    else if(uiSampleRate==32000)
    {
        return uiSampleRate;
    }    
    else if(uiSampleRate==44100)
    {
        return uiSampleRate;
    }    
    else if(uiSampleRate==48000)
    {
        return uiSampleRate;
    }    
    else
    {
        SCI_TRACE_LOW("ARMVB_Record samplerate is invalid = %ld", uiSampleRate);
        uiSampleRate = 0;
        return uiSampleRate;
    }
}

#define COF_VALUE (1024)
uint32 ARMVB_GetRemainedPcmCount(void)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = NULL;
    UINT16 readOffset;
    UINT16 writeOffset;
    UINT16 lenth;
    UINT32 remainedcount;

    zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    readOffset = zsp_sharemem->audInPara.readOffset;
    writeOffset = zsp_sharemem->audInPara.writeOffset;
    lenth = zsp_sharemem->audInPara.inLenth;
    if (readOffset <= writeOffset)
    {
        remainedcount = (UINT32)(writeOffset - readOffset);
    }
    else
    {
        remainedcount = (UINT32)(writeOffset + lenth - readOffset);
    }
    if(zsp_sharemem->audInPara.fileEndFlag == 1)
    {
        if(remainedcount > COF_VALUE)
        {
            remainedcount -= COF_VALUE;
        }
        else
        {
            remainedcount = 0;
        }
    }
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_GetRemainedPcmCount return remainedcount:%d",
         __LINE__, remainedcount);
    return (remainedcount);
}

uint32 ARMVB_GetRemainedPcmTime(void)
{
    uint32 ret_time = 0;
    uint32 pcm_num = 0;

    if (0 == g_uiCurrentSamplerate)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_GetRemainedPcmTime return 0", __LINE__);
        return 0;
    }
    if (8000 == g_uiCurrentSamplerate)
    {
        pcm_num = 0;//2304;
    }
    else
    {
        pcm_num = 1152;
    }
    ret_time = (((ARMVB_GetRemainedPcmCount() + pcm_num) / g_channelNb) * 1000 / g_uiCurrentSamplerate);
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_GetRemainedPcmTime 1 g_channelNb:[%d] return:%d ", __LINE__, g_channelNb, ret_time);
    return ret_time;
}

/*****************************************************************************/
//  Description:    Get LINEIN or LINEINAD device appropriate output samplerate.
//  Author:         TH
//  Note:
//****************************************************************************/
LOCAL uint32 ARMVB_GetLineinAppropriateOutputSamplerate (uint32 uiSampleRate)
{
    return ARMVB_GetSupportSamplerate(uiSampleRate);
}

/*****************************************************************************/
//  Description:    Open voiceband.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    uint32 tick_cnt;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    SND_ITF_T cur_itf = SND_ITF_LOUD_SPEAKER;
    SCI_TRACE_LOW("{xx} ARMVB_Play SCI_GetSemaphore....");
    if(SCI_SUCCESS != SCI_GetSemaphore(audio_armvb_semaphore, 10))
    {
        SCI_TRACE_LOW("{xx} ARMVB_Play SCI_GetSemaphore return");
        return AUDIO_CONFLICT;
    }
    SCI_TRACE_LOW("{xx} ARMVB_Play SCI_GetSemaphore OK");
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ptDevSetting->eDev_mode:0x%x g_audio_dev_used:[%d] time[%d][%d]", __LINE__, __FILE__, __FUNCTION__, ptDevSetting->eDev_mode, g_audio_dev_used, tick_cnt/1000, tick_cnt%1000);
    SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] uiSampleRate[%d] vol[%d] ", __FUNCTION__,__LINE__,uiSampleRate,ptDevSetting->uiVol);
    if (1 == g_audio_dev_used)
    {
        SCI_TRACE_LOW("[ljf] Because g_audio_dev_used is [%d], so ARMVB_Play return", g_audio_dev_used);
        return AUDIO_ERROR;
    }
    SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);

    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;

        default:
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_aif_stream.startAddress = (uint32 *)zsp_sharemem->Output.audOutput;
    if(ptDevSetting->usKeyTone)
    {
        //g_aif_stream.length = 1024*2;//AUDIO_OUTPUT_BUF_SIZE;
        g_aif_stream.length = 1024;//AUDIO_OUTPUT_BUF_SIZE;
        if (1 == tts_play_number)
    	{
	        AUDIO_CP_SetBtNRECFlag(2); // reuse BT NREC Flag for add depop delay in audio driver
	        ARMVB_SetToneMode(1);
    	}
    }
    else
    {
        g_aif_stream.length = AUDIO_OUTPUT_BUF_SIZE;
    }
    g_uiCurrentSamplerate = uiSampleRate;
    zsp_sharemem->audInPara.sampleRate = uiSampleRate;
    zsp_sharemem->audOutPara.samplerate = uiSampleRate;
    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL)
    {
        zsp_sharemem->audInPara.channelNb = HAL_AIF_STEREO;
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_STEREO;
    }
    else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) || (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT))
    {
        zsp_sharemem->audInPara.channelNb = HAL_AIF_MONO;
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_MONO;
    }
    else
    {
        SCI_TRACE_LOW("line[%d] ARMVB_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
    zsp_sharemem->audInPara.bitsPerSample = 16;
#ifdef DECODE_IN_ZSP
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_sharemem->musicMode =2;// 1:PCM mode;2:mp3 stream
#endif
    g_zsp_para.musicMode = 2;
#else
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_sharemem->musicMode = 1;
#endif
    g_zsp_para.musicMode = 1;
#endif
    g_zsp_para.sbcOutFlag = 0;
    if(ptDevSetting->usKeyTone)
    {
        //zsp_sharemem->audOutPara.length = 512;//AUDIO_OUTPUT_BUF_SIZE/8;//1152*2;
        zsp_sharemem->audOutPara.length = 256;//AUDIO_OUTPUT_BUF_SIZE/8;//1152*2;
    }
    else
    {
        zsp_sharemem->audOutPara.length = 1152*2;
    }

    //!!!can not memset all of zsp sharemem to zero. some status value will be keep with zsp!!! sheen
    //  do not init this para will result in assert but  it should not initialize here for zsp used !zgt
    zsp_sharemem->updateParaInd=0;//sheen
    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL)
    {
        g_aif_stream.channelNb = HAL_AIF_STEREO;
    }
    else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) || (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT))
    {
        g_aif_stream.channelNb = HAL_AIF_MONO;
    }
    else
    {
        SCI_TRACE_LOW("line[%d] ARMVB_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
    g_channelNb = g_aif_stream.channelNb;
    g_aif_stream.playSyncWithRecord = 0;
    g_aif_stream.sampleRate = uiSampleRate;
    g_aif_stream.voiceQuality = TRUE;
    g_aif_stream.halfHandler = NULL;
    g_aif_stream.endHandler = NULL;

    //for bug973551 973739 to fix noise in seek and stop.zgt 2018.12.10
    SCI_TRACE_LOW("line[%d] ARMVB_SetDownlinkMute ptDevSetting->bDownlink_mute:%d", __LINE__, ptDevSetting->bDownlink_mute);
    if(0==ptDevSetting->bDownlink_mute)
    {
        g_aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)ptDevSetting->uiVol;
    }
    else
    {
        g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
    }
    s_is_vb_mute = ptDevSetting->bDownlink_mute;

    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_MUSIC;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    zsp_sharemem->audInPara.fileEndFlag = 0;
    zsp_sharemem->audInPara.readOffset = 0;
    SCI_MEMSET(zsp_sharemem->Output.audOutput, 0, AUDIO_OUTPUT_BUF_SIZE);
    SCI_MEMSET(zsp_sharemem->audInput, 0, (zsp_sharemem->audInPara.inLenth -4));
#ifdef DECODE_IN_ZSP
    zsp_sharemem->audInPara.writeOffset =zsp_sharemem->audInPara.inLenth -4; // Init buffer
#else
    if(ptDevSetting->usKeyTone)
    {
    //zsp_sharemem->audInPara.writeOffset = 1024;//AUDIO_OUTPUT_BUF_SIZE/2;//4608; //zsp_sharemem->audInPara.inLenth -4; // Init buffer
    zsp_sharemem->audInPara.writeOffset = 512;//AUDIO_OUTPUT_BUF_SIZE/2;//4608; //zsp_sharemem->audInPara.inLenth -4; // Init buffer
    }
    else
    {
    zsp_sharemem->audInPara.writeOffset = 4608; //zsp_sharemem->audInPara.inLenth -4; // Init buffer
    }
#endif
    ipc_register_music_notify(ARMVB_PLAYBACK_Callback);

    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioStart");
       // A2DP_audioStart(uiSampleRate, A2DP_GetAudioData, A2DP_GetAudioDataCount);
    }
#if 0//ndef ARMVB_USE_CP_AUDIO_INTERFACE
#ifdef ARMVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
	AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_MUSIC);
#endif
#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStart();
#else
    hal_ret = hal_zspMusicPlayStart();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play call hal_zspMusicPlayStart faile ret:0x%x", __LINE__, hal_ret);
    }
#endif

#if 1 // vibration mode
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play g_shakingMode:0x%x", __LINE__, g_shakingMode);
    aud_SetSharkingMode(g_shakingMode);
#else
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play g_shakingMode:0x%x", __LINE__, g_shakingMode);
    DM_AudSetSharkingMode(g_shakingMode);
#endif
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play call aud_StreamStart faile ret:0x%x", __LINE__, ret);
    }
#else
    ret = DM_AudStreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play call DM_AudStreamStart faile ret:0x%x", __LINE__, ret);
    }
#endif
#else
    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType= AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath= AUD_INPUT_PATH1;
    device_cfg_ext.linePath= AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
    ret =AUDIO_CP_START(AUDIO_DEV_MUSIC,g_cur_itf, &g_aif_stream, &g_aud_level_cfg,0,device_cfg_ext,0, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("zgt line[%d] ARMVB_Play call AUDIO_CP_START faile ret:0x%x", __LINE__, ret);
        return ret;
    }
#endif
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_EARFREE)
    {
        g_cur_itf = SND_ITF_LOUD_SPEAKER;
        ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
        if (0 != ret)
        {
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play call AUDIO_CP_Setup faile ret:0x%x", __LINE__, ret);
        }
    }
	tts_play_number = 0;
	AUDIO_CP_SetBtNRECFlag(g_NRECflag);
    ARMVB_SetToneMode(0);//reset tonemode value
    return 0;
}

/*****************************************************************************/
//  Description:    Close voiceband
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_Stop (DEV_SETTING_T *ptDevSetting)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    INT8 delay_cnt = 10;
    uint32 tick_cnt;
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] eDev_mode:0x%x g_audio_dev_used:[%d], time[%d][%d]", __LINE__, __FILE__, __FUNCTION__, ptDevSetting->eDev_mode, g_audio_dev_used, tick_cnt/1000, tick_cnt%1000);
    if (1 == g_audio_dev_used)
    {
        SCI_TRACE_LOW("[ljf] Because g_audio_dev_used is [%d], so ARMVB_Stop return", g_audio_dev_used);
        return AUDIO_ERROR;
    }

    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER; // close lasted device.
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;

        default:
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_uiCurrentSamplerate = 0;
    zsp_sharemem->audInPara.fileEndFlag = 1;
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_sharemem->audInPara.sbcOutFlag = 0;
    zsp_sharemem->musicMode = 0;
#endif
    g_zsp_para.musicMode = 0;
    g_zsp_para.sbcOutFlag = 0;
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioClose");
        //A2DP_audioClose();
    }
#if 0///ndef ARMVB_USE_CP_AUDIO_INTERFACE
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call aud_StreamStop faile", __LINE__);
    }
#else
    ret = DM_AudStreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call DM_AudStreamStop faile", __LINE__);
    }
#endif
#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStop();
#else
    hal_ret = hal_zspMusicPlayStop();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call hal_zspMusicPlayStop faile", __LINE__);
    }
#endif
#if 1
    // just for close 26M after close codec
    while (delay_cnt > 0)
    {
    ret = DM_VoisSetBtNRECFlag(0);
    if (1 == ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call DM_VoisSetBtNRECFlag sleep 50ms ret:0x%x", __LINE__, ret);
        SCI_SLEEP(50);
    delay_cnt--;
    }
    else
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_Stop call DM_VoisSetBtNRECFlag succ and return ret:0x%x", __LINE__, ret);
        break;
    }
    }
#endif

#ifdef ARMVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
        AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_MUSIC);
#endif
#else
    //ARMVB_SetToneMode(0);
    ret = AUDIO_CP_STOP(AUDIO_DEV_MUSIC,g_cur_itf, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("zgt line[%d] ARMVB_Stop call AUDIO_CP_STOP faile ret:0x%x", __LINE__, ret);
        return ret;
    }
#endif

    SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    SCI_TRACE_LOW("{xx} ARMVB_Stop SCI_PutSemaphore....");
    SCI_PutSemaphore(audio_armvb_semaphore);
    SCI_TRACE_LOW("{xx} ARMVB_Stop SCI_PutSemaphore OK");

    return 0;
}
LOCAL AUDIO_RESULT_E ARMVB_SetNotchToneEnable(uint32 enable)
{
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetNotchToneEnable ", __LINE__);

    return 0;
}

/*****************************************************************************/
//  Description:    armvb set notch tone para.
//  Author:         ye.zhang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_SetNotchPara()
{
    NothPara para;
    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_HANDFREE;
    char *mode_name = NULL;
    AUDIO_NV_ARM_MODE_INFO_T *ptAudMode = PNULL;

    mode_name = _ARMVB_ArmvbNv_GetName(cur_dev_mode);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("get mode_name error!");
        return AUDIO_ERROR;
    }
    ptAudMode = (AUDIO_NV_ARM_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        SCI_TRACE_LOW("ARMVB_SetNotchpara alloc error!");
        return AUDIO_ERROR;
    }

    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudMode)))
    {
        SCI_FREE(ptAudMode);
        SCI_TRACE_LOW("AUDIONVARM_GetModeParam error!");
        return AUDIO_ERROR; 
    }    
    
    para.ptsinM= ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[0];
    para.ptsinL= ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[1];
    para.ptcosM= ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[2];
    para.ptcosL= ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[3];
    para.ptgl  = ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[4]; 
    para.ptgr  = ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[5];
    para.gd    = ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[6];
    para.gu    = ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[7];
    para.holdtime= ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[0].reserve[8];

    AUDIO_HAL_SetNothpara(&para);
    SCI_FREE(ptAudMode);
    return AUDIO_NO_ERROR;
}

#ifdef BT_DIALER_SET_VOLGAIN
/*****************************************************************************/
//  Description:    armvb set gain for special volume.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E _ARMVB_SetVolGain (AUDIO_ARM_APP_TYPE_E eAppType, void *param)
{
    SCI_TRACE_LOW("[ljf] line[%d] _ARMVB_SetVolGain ", __LINE__);

    return 0;
}
#endif

LOCAL void ARMVB_SetSampleRate (uint32 sample_rate)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return;
}
/*****************************************************************************/
//  Description:    Register voiceband controlled by arm to audio service.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d]", __FILE__, __FUNCTION__, __LINE__);
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB"), strlen ("ARMVB") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_TRUE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = ARMVB_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL; //ARMVB_SetDepopMute; // AUDIO_SM_Stop WILL call sleep(150), so set depopmute funcion to NULL
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=ARMVB_GetRemainedPcmCount;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=ARMVB_GetRemainedPcmTime;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetAppropriateOutputSamplerate;
    tDeviceInfo.tAudioDevOpe.pSetNotchton=ARMVB_SetNotchToneEnable;
#ifdef BT_DIALER_SET_VOLGAIN
    tDeviceInfo.tAudioDevOpe.pSetVolGain = _ARMVB_SetVolGain;
#endif    
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d] ", __FILE__, __FUNCTION__, __LINE__);
    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    g_armvb_device_ptr = AUDIO_DM_GetDeviceHandleStruct (hDevice);  // Record device structure
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_RegAudioDevice g_armvb_device_ptr:0x%x ", __LINE__, g_armvb_device_ptr);
#if 1 // TODO: LJF TEST
    //AUDIO_HAL_Register_Playback((AUDIO_HAL_CALLBACK_FUN)ARMVB_PLAYBACK_Callback);
    g_zsp_share_memory_addr = GET_MemoryStartAddressAndLen(MEM_AUDIO_SM_NAME, &g_zsp_share_memory_lenth);
    //ipc_register_music_notify(ARMVB_PLAYBACK_Callback);
    g_aud_level_cfg.spkLevel = SND_SPK_VOL_8;
    if(SCI_NULL == audio_armvb_semaphore)
    {
        audio_armvb_semaphore = SCI_CreateSemaphore("ARMVB_AUDIO_SEMAPHORE",1);
    }
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d] g_zsp_share_memory_addr:0x%x g_zsp_share_memory_lenth:0x%x", __FILE__, __FUNCTION__, __LINE__, g_zsp_share_memory_addr, g_zsp_share_memory_lenth);
#endif
    return hDevice;
}

/*****************************************************************************/
//  Description:    ARM VB Sets paras for changes from nv
//  Author:         Cherry.Liu
//  Note:           豕?nv dev mode?Darm?角1?米?2?那y?邦??,?辰米¯車?∩?o‘那y辰?那12?那y谷迆D∫!
//****************************************************************************/
PUBLIC AUDIO_RESULT_E ARMVB_SetDevModeParam (
    AUDIO_NV_ARM_MODE_INFO_T *ptDevModInfo)
{    
    AUDIO_RESULT_E  result = AUDIO_NO_ERROR;

#ifdef EQ_EXE_ON_CHIP
#ifndef EQ_HAS_MULTI_BANDS
    //mp3 & midi  lcf/agc/eq control related paras changed
    {
        AUD_PROC_EXP_PARA_T  aud_para = {0};
        aud_para.eParaType = AUD_PROC_PARA_EQ_SET;
        result = AUDIO_ExeExpSetParaOpe (
            hAUDPROCExp,
            PNULL,
            AUD_PROC_EXP_PARA_COUNT,
            &aud_para
            );

        //SCI_TRACE_LOW:"set aud proc explugger!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_876_112_2_18_1_2_12_35,(uint8*)"");
    }
#else
    {
    AUD_ENHA_EXP_PARA_T  mode_para = {0};

    mode_para.eParaType = AUD_ENHA_PARA_DEV_MODE;
    mode_para.unAudProcPara.devModeName = (char *)ptDevModInfo->ucModeName;

    result = AUDIO_ExeExpSetParaOpe (
        hAUDENHAExp, 
        PNULL,
        AUD_ENHA_EXP_PARA_COUNT,
        &mode_para
        );
    }
#endif
#endif
    //other arm related paras setting....
    return result;
}
/*****************************************************************************/
//  Description:    ARM VB Sets speaker pa and headset pa.
//  Author:         Johnson.sun
//  Param bEnable: TRUE=set, FALSE=off
//  Note:
//****************************************************************************/
PUBLIC void ARMVB_EnableAmplifier (BOOLEAN bEnable)
{
    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    BOOLEAN is_mute = SCI_FALSE;

    AUDIO_GetDownLinkMute (&is_mute);

    if (bEnable && !is_mute)
    {
        AUDMOD_DEVICECTL_T aud_mod_param_t = {0}; 
        AUDIO_NV_ARM_RESULT_E result = AUDIO_NV_ARM_NO_ERROR;
        char *mode_name = PNULL;
        
        AUDIO_GetDevMode (&cur_dev_mode);
        
        mode_name = _ARMVB_ArmvbNv_GetName(cur_dev_mode);
        SCI_ASSERT(mode_name != PNULL); /*assert verified*/

        result=AUDIONVARM_GetAudioModeDevCtrInfo (mode_name, AUDIO_DEVICE_MODE_PARAM_PLAY, (AUDMOD_DEVICECTL_T *)(&aud_mod_param_t));    
        SCI_ASSERT(AUDIO_NV_ARM_NO_ERROR==result);/*assert verified*/

        ARMVB_DrvOutSwitch(0,AUDIO_SPK,aud_mod_param_t.ucSpkAmpEn);
        
    }
    else
    {
        ARMVB_DrvOutSwitch(0,AUDIO_SPK,SCI_FALSE);
    }
}

/*****************************************************************************/
//  Description:    ARM VB Sets speaker pa and headset pa in recording.
//  Author:         shujing.dong
//  Param bEnable: TRUE=set, FALSE=off
//  Note:
//****************************************************************************/
PUBLIC void ARMVB_RECORD_EnableAmplifier (BOOLEAN bEnable)
{
    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    BOOLEAN is_mute = SCI_FALSE;    

    AUDIO_GetDownLinkMute (&is_mute);

    if (bEnable && !is_mute)
    {
        AUDMOD_DEVICECTL_T aud_mod_param_t = {0};  
        char *mode_name = NULL;
        AUDIO_NV_ARM_RESULT_E result = AUDIO_NV_ARM_NO_ERROR;  

        AUDIO_GetDevMode (&cur_dev_mode);

        mode_name = _ARMVB_ArmvbNv_GetName(cur_dev_mode);
        if(PNULL==mode_name)
        {
            //SCI_TRACE_LOW:"armvb_as.c ARMVB_RECORD_EnableAmplifier mode:%d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_962_112_2_18_1_2_12_36,(uint8*)"d", cur_dev_mode);
            return;
        }

        result=AUDIONVARM_GetAudioModeDevCtrInfo (mode_name, AUDIO_DEVICE_MODE_PARAM_RECORD, (AUDMOD_DEVICECTL_T *)(&aud_mod_param_t));    
        if(AUDIO_NV_ARM_NO_ERROR!=result)
        {
            //SCI_TRACE_LOW:"armvb_as.c ARMVB_RECORD_EnableAmplifier failed to get dev ctr info, result:%d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_969_112_2_18_1_2_12_37,(uint8*)"d", result);
            return;
        } 
        ARMVB_DrvOutSwitch(0,AUDIO_SPK,aud_mod_param_t.ucSpkAmpEn);
    }
    else
    {
        ARMVB_DrvOutSwitch(0,AUDIO_SPK,SCI_FALSE);
    }
}

#define ____BBAT_ARMVB_PLAY_START__


void bbat_ARMVB_PLAYBACK_Callback (void)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = NULL;
    uint32 data_count;
    uint32 copy_size = 0;
    int channelNb = 0;

    zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    
    if (g_channelNb == HAL_AIF_MONO)
    {
        channelNb = 0;
    }
    else
    {
        channelNb = 1;
    }

    data_count = RingBufferGetWriteBufferLen(&zsp_sharemem->audInPara, zsp_sharemem->audInPara.inLenth);
    
    copy_size = _Track_Mixer(g_armvb_device_ptr->ptTrackList,
                                            s_vb_da0_buffer_multitrack, SCI_NULL/*s_vb_da1_buffer_multitrack*/,
                                            SCI_NULL, SCI_NULL,  data_count/2,   1,  channelNb,  s_is_vb_mute);
    SCI_TRACE_LOW("[xx] file[%s] func[%s] line[%d] copy_size = %d", __FILE__, __FUNCTION__, __LINE__,copy_size);
    
    RingBufferWriteBuffer(&zsp_sharemem->audInPara, 
                                        zsp_sharemem->audInput, 
                                        s_vb_da0_buffer_multitrack, data_count, (copy_size * 2));

    s_armvb_output_buf.in += copy_size*2;
    if (copy_size < (data_count/2))
    {
        if(_Track_Mixer_End(g_armvb_device_ptr->ptTrackList))
        {
            zsp_sharemem->audInPara.fileEndFlag = 1;
        }
    }
}

LOCAL SND_ITF_T bbatsnditf = SND_ITF_NONE;
/*****************************************************************************/
//  Description:    Open voiceband.
//  Author:         xin.xu1
//  Note:
//****************************************************************************/
PUBLIC AUDIO_RESULT_E bbat_ARMVB_Play (AUDIO_DEVICE_MODE_TYPE_E eDev_mode, void *pPcmStream)
{
    AUD_ERR_T ret = 0;
    BBAT_PCM_STREAM_T * pbbatPcmStream = (BBAT_PCM_STREAM_T *)pPcmStream;

    SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);

    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;

        default:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
    }
    bbatsnditf = g_cur_itf;
#if 0
#ifdef ARMVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
	AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_MUSIC);
#endif
    ret = aud_bbatPcmBufferPlayStart(g_cur_itf, pbbatPcmStream);
    if (0 != ret)
    {
        SCI_TRACE_LOW("line[%d] bbat_ARMVB_Play call aud_bbatPcmBufferPlayStart faile ret:0x%x", __LINE__, ret);
    }
    SCI_TRACE_LOW("line[%d] bbat_ARMVB_Play call aud_bbatPcmBufferPlayStart faile ret:0x%x", __LINE__, ret);
#else
    ret = AUDIO_CP_bbatPcmPlayStart(g_cur_itf, pbbatPcmStream);
    if (AUD_ERR_NO!= ret)
    {
        SCI_TRACE_LOW("line[%d] bbat_ARMVB_Play call AUDIO_CP_bbatPcmPlayStart faile ret:0x%x", __LINE__, ret);
    }
    SCI_TRACE_LOW("line[%d] bbat_ARMVB_Play call AUDIO_CP_bbatPcmPlayStart  ret:0x%x", __LINE__, ret);
#endif
    
    return 0;
}


/*****************************************************************************/
//  Description:    Close voiceband
//  Author:         xin.xu1
//  Note:
//****************************************************************************/
PUBLIC AUDIO_RESULT_E bbat_ARMVB_Stop (AUDIO_DEVICE_MODE_TYPE_E eDev_mode)
{
    AUD_ERR_T ret = 0;

    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER; // close lasted device.
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;

        default:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
    }
#if 0
    ret = aud_bbatPcmBufferPlayStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("line[%d] aud_bbatPcmBufferPlayStop call aud_bbatPcmBufferPlayStop faile ret:0x%x", __LINE__, ret);
    }
    SCI_TRACE_LOW("line[%d] aud_bbatPcmBufferPlayStop call aud_bbatPcmBufferPlayStop faile ret:0x%x", __LINE__, ret);

#ifdef ARMVB_USE_PCIM_26M_CLOCK
    //PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
        AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_MUSIC);
#endif
#else
    ret = AUDIO_CP_bbatPcmPlayStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("line[%d] aud_armvb_STOP call AUDIO_CP_bbatPcmPlayStop faile ret:0x%x", __LINE__, ret);
    }
    SCI_TRACE_LOW("line[%d] aud_armvb_STOP call AUDIO_CP_bbatPcmPlayStop ret:0x%x", __LINE__, ret);
#endif
    SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    bbatsnditf = SND_ITF_NONE;

    return 0;
}

#define ____BBAT_ARMVB_PLAY_END__

#ifdef PRODUCT_DRIVER_MTV_ENABLE
LOCAL void VB_MOD_SetOutputMode (BOOLEAN bAuxon)
{
    if (bAuxon)
    {
        ARMVB_DrvOutSwitch(0,AUDIO_EAR,SCI_FALSE);        
    }
    else
    {
        ARMVB_DrvOutSwitch(0,AUDIO_EAR,SCI_TRUE);
    }
}

LOCAL void VB_MOD_SetStereo (BOOLEAN bStereoOn)
{
    if (bStereoOn)
    {
        ARMVB_DrvOutSwitch(0,AUDIO_HP,SCI_TRUE);
    }
    else
    {
        ARMVB_DrvOutSwitch(0,AUDIO_HP,SCI_FALSE);
    }
}

LOCAL void VB_MOD_SetVolume (uint32 uiVol, uint32 uiBase, AUDIO_ARM_APP_TYPE_E eAppType)
{
    uint32 gain = 0;

    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    char *mode_name = NULL;
    AUDIO_NV_ARM_MODE_INFO_T *ptAudMode = PNULL;

    s_vb_volumn = uiVol;

    AUDIO_GetDevMode (&cur_dev_mode);
    mode_name = _ARMVB_ArmvbNv_GetName(cur_dev_mode);
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"armvb_as.c VB_MOD_SetVolume mode:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1020_112_2_18_1_2_12_38,(uint8*)"d", cur_dev_mode);
        return;
    }

    //SCI_TRACE_LOW:"[VB_MOD_SetVolume] uiVol=%d, mode:%d, appType:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1024_112_2_18_1_2_12_39,(uint8*)"ddd", uiVol, cur_dev_mode, eAppType);

    ptAudMode = (AUDIO_NV_ARM_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        //SCI_TRACE_LOW:"armvb_as.c, VB_MOD_SetVolume, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1029_112_2_18_1_2_12_40,(uint8*)"d",sizeof(AUDIO_NV_ARM_MODE_INFO_T));
        return;
    }

    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudMode)))
    {
        SCI_FREE(ptAudMode);
        //SCI_TRACE_LOW:"armvb_as.c VB_MOD_SetVolume failed to get mode param."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1036_112_2_18_1_2_12_41,(uint8*)"");
        return;
    }       

    if (0 == uiVol)
    {
        //SCI_TRACE_LOW:"VB_MOD_SetVolume 0 == uiVol"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1042_112_2_18_1_2_12_42,(uint8*)"");
        s_is_vb_mute = SCI_TRUE;
    }
    else
    {
        s_is_vb_mute = SCI_FALSE;
    }

    gain = ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[eAppType].arm_volume[uiVol & 0xF];

    //SCI_TRACE_LOW:"[VB_MOD_SetVolume] gain= 0x%4x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1052_112_2_18_1_2_12_43,(uint8*)"d", gain);

    AUDDEV_SetPGA (0, gain);
    AUDDEV_SetPGA (1, gain);

    SCI_FREE(ptAudMode);
}

/*****************************************************************************/
//  Description:    MTV Open voiceband.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E MTVVB_Play (
    uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting
)
{
    uint32 volume=0;

    if (0 == g_mtvvb_device_ptr->tDeviceInfo.ucDevOpenCnt)
    {
        BOOLEAN bAudDevOpen = SCI_FALSE;
        AUDIO_RESULT_E vol_result = AUDIO_NO_ERROR;
        bAudDevOpen = AUDDEV_Open (AUDDEV_FUNC_EXT_IIS,
        SCI_TRUE,
        uiSampleRate);

        if(!bAudDevOpen)
        {
            //SCI_TRACE_LOW:"MTVVB_Play AUDDEV_Open failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1080_112_2_18_1_2_12_44,(uint8*)"");
            return AUDIO_ERROR;
        }
        VB_MOD_SetOutputMode (AUDMOD_GetOutputModeSetting_Dolphin());

        VB_MOD_SetStereo (AUDMOD_GetStereoSetting_Dolphin());
        vol_result = ARMVB_SetVolumeType (ptDevSetting->uiVol, ptDevSetting->eAppType);
        if(AUDIO_NO_ERROR!=vol_result)
        {
            return AUDIO_ERROR;
        }

        return AUDIO_NO_ERROR;
    }

    //SCI_TRACE_LOW:"MTVVB_Play has been opend, opencount:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1095_112_2_18_1_2_12_45,(uint8*)"d", g_mtvvb_device_ptr->tDeviceInfo.ucDevOpenCnt);
    return AUDIO_ERROR;
}

/*****************************************************************************/
//  Description:    MTV Close voiceband
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E MTVVB_Stop (DEV_SETTING_T *ptDevSetting)
{
    if (1 == g_mtvvb_device_ptr->tDeviceInfo.ucDevOpenCnt)
    {
        g_mtvvb_device_ptr->uiCurrentSamplerate = 0;

        AUDDEV_Close (AUDDEV_FUNC_EXT_IIS, SCI_TRUE);
        return AUDIO_NO_ERROR;
    }

    //SCI_TRACE_LOW:"MTVVB_Stop more opened:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1113_112_2_18_1_2_12_46,(uint8*)"d", g_mtvvb_device_ptr->tDeviceInfo.ucDevOpenCnt);
    return AUDIO_ERROR;
}

/*****************************************************************************/
//  Description:    MRV Pause voiceband.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E MTVVB_Pause (DEV_SETTING_T *ptDevSetting)
{
    if (1 == g_mtvvb_device_ptr->tDeviceInfo.ucDevOpenCnt)
    {
        AUDDEV_Close (AUDDEV_FUNC_EXT_IIS, SCI_TRUE);
        return AUDIO_NO_ERROR;
    }

    //SCI_TRACE_LOW:"MTVVB_Pause more opened:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1129_112_2_18_1_2_12_47,(uint8*)"d", g_mtvvb_device_ptr->tDeviceInfo.ucDevOpenCnt);
    return AUDIO_ERROR;
}
/*****************************************************************************/
//  Description:    MTV Resume voiceband.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E MTVVB_Resume (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    if (0 == g_mtvvb_device_ptr->tDeviceInfo.ucDevOpenCnt)
    {
        BOOLEAN bAudDevOpen = SCI_FALSE;
        bAudDevOpen = AUDDEV_Open (AUDDEV_FUNC_EXT_IIS,
                     SCI_TRUE,
                     uiSampleRate);

        if(!bAudDevOpen)
        {
            //SCI_TRACE_LOW:"MTVVB_Resume AUDDEV_Open failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1147_112_2_18_1_2_12_48,(uint8*)"");
            return AUDIO_ERROR;
        }
        VB_MOD_SetOutputMode (AUDMOD_GetOutputModeSetting_Dolphin());
        VB_MOD_SetStereo (AUDMOD_GetStereoSetting_Dolphin());
        VB_MOD_SetVolume (AUDMOD_GetVolumeSetting(), 0, AUDIO_ARM_APP_TYPE_0);

        return AUDIO_NO_ERROR;
    }

    //SCI_TRACE_LOW:"MTVVB_Resume has been opened, opencount:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1157_112_2_18_1_2_12_49,(uint8*)"d", g_mtvvb_device_ptr->tDeviceInfo.ucDevOpenCnt);
    return AUDIO_ERROR;

}

/*****************************************************************************/
//  Description:    Register voiceband controlled by MTV to audio service.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV MTVVB_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("MTVVB"), strlen ("MTVVB") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = MTVVB_Play;
    tDeviceInfo.tAudioDevOpe.pClose = MTVVB_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = MTVVB_Pause;
    tDeviceInfo.tAudioDevOpe.pResume = MTVVB_Resume;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetAppropriateOutputSamplerate;
#ifdef BT_DIALER_SET_VOLGAIN  
    tDeviceInfo.tAudioDevOpe.pSetVolGain = _ARMVB_SetVolGain;
#endif
    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    g_mtvvb_device_ptr = AUDIO_DM_GetDeviceHandleStruct (hDevice);  // Record device structure

    return hDevice;
}
#endif

/*****************************************************************************/
//  Description:    adjust the address of left channel and right channel to output .
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/


/*****************************************************************************/
//  Description:    Send data to voiceband DA.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL int32 ARMVB_Record_SetAdcBufInfo(void *pLHead,void *pRHead,uint32 cnt)
{
    return 0;
}

int32 ARMVB_Record_GetZspWritePoint(uint32* pWrite)
{
    int32 ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    *pWrite = zsp_sharemem->audInPara.writeOffset;

    return ret;
}

int32 ARMVB_Record_SetZspReadPoint(uint32 pRead)
{
    int32 ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    zsp_sharemem->audInPara.readOffset = pRead;
    //SCI_TRACE_LOW("[zgt]  fuc[%s] Read piont[%d] ", __FUNCTION__, zsp_sharemem->audInPara.readOffset );

    return ret;
}

/*****************************************************************************/
//  Description:    ARMVB logic device set device volume.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_Record_SetVolumeType (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{   
    return AUDIO_NO_ERROR;
}

LOCAL void ARMVB_Record_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);

    return ;
}

/*****************************************************************************/
//  Description:    ARM VB Sets paras for changes from nv
//  Author:         Cherry.Liu
//  Note:           豕?nv dev mode?Darm?角1?米?2?那y?邦??,?辰米¯車?∩?o‘那y辰?那12?那y谷迆D∫!
//****************************************************************************/
PUBLIC AUDIO_RESULT_E ARMVB_Record_SetDevModeExp (AUDIO_DEVICE_MODE_TYPE_E eMode)
{    
    AUDIO_RESULT_E  result = AUDIO_NO_ERROR;
    return result;
}

LOCAL AUDIO_RESULT_E ARMVB_Record_PostOpen(void *ptParam)
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
#ifdef AUD_ARM_RECORD_EXP
    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);
#endif
    return result;
}

LOCAL AUDIO_RESULT_E ARMVB_Record_PreClose(void *ptParam)
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    return result;
}
/*****************************************************************************/
//  Description:    ARMVB RECORD logic device set device mode.
//  Author:
//  Note:
//****************************************************************************/
LOCAL void ARMVB_Record_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);

    return;
}
/*****************************************************************************/
//  Description:    ARMVB RECORD logic device reg Rec callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL uint32 ARMVB_Record_RegRecCallback (
    ADM_RECCALLBACK pCallback
)
{
    int32 i = 0;
    
    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);
    SCI_PASSERT((pCallback != SCI_NULL), /*assert verified*/
        ("ARMVB_Record_RegRecCallback: Input Parameter(pCallback) should not be NULL!")); 

    // Find the vacant position and save it.
    for(i = 0; i < ARMVB_RECORD_OUTPUT_COUNT_MAX; i++)
    {
        if(s_armrecord_callback[i] == SCI_NULL)
        {
            //SCI_PASSERT((s_record_callback_count<ARMVB_RECORD_OUTPUT_COUNT_MAX),  /*assert verified*/
            //    ("count:%d, addr:0x%x.", s_record_callback_count, pCallback));
            s_record_callback_count++;
            s_armrecord_callback[i] = pCallback;
            return (uint32) s_armrecord_callback[i];
        }
    }
    /*20190228-xin.xu1 -code review*/
    return (uint32)SCI_NULL;
}

/*****************************************************************************/
//  Description:    ARMVB RECORD logic device unreg Rec callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL void ARMVB_Record_UnRegRecCallback (
    uint32 callback_vector
)
{
    int32 i = 0;
    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);

    // Find the occupied position by callback function and clear it.
    for(i = 0; i < ARMVB_RECORD_OUTPUT_COUNT_MAX; i++)
    {
        if((uint32)s_armrecord_callback[i] == callback_vector)
        {
            //SCI_PASSERT((s_record_callback_count>0),  /*assert verified*/
            //    ("count:%d, addr:0x%x.", s_record_callback_count, callback_vector));
            s_record_callback_count--;
            s_armrecord_callback[i] = SCI_NULL;
            break;
        }
    }
     return;
}

LOCAL void ARMVB_Record_CopyAudioData(uint16 *puiDestData,
            const uint16 *puiLeftData,
            const uint16 *puiRightData,
            uint32 uiOffset,
            uint32 uiSampleCount)
{
    if (!puiRightData)
    {
        SCI_MEMCPY(puiDestData, puiLeftData+uiOffset, uiSampleCount<<1);
    }
    else
    {
        int16 *left_ptr = (int16 *)puiLeftData + uiOffset;
        int16 *right_ptr = (int16 *)puiRightData + uiOffset;
        while (uiSampleCount)
        {
            int32 sample = ((*left_ptr + *right_ptr) /2);
            *puiDestData = (uint16)sample;
            puiDestData++;
            left_ptr++;
            right_ptr++;
            uiSampleCount--;
        }
    }
}

/*****************************************************************************/
//  Description:    ARMVB RECORD logic device read data interface
//  Author:
//  Note:
//****************************************************************************/
LOCAL void ARMVB_Record_ReadData_Internal(uint16 *puiDestData, uint32 uiCount, uint32 *puiDataLen)
{
    uint32 write_point = 0;
    uint32 byte_count_1 = 0 ,byte_count_2=0;
    SCI_ASSERT (PNULL != puiDestData);  /*assert verified*/

    if((0==uiCount) || (0 == s_ad_read_size))
    {
        SCI_TRACE_LOW("[zgt]  fuc[%s] need none. uiCount=[%d] ", __FUNCTION__,uiCount);

        *puiDataLen = 0;
        return;
    }
    ARMVB_Record_GetZspWritePoint(&write_point);

    if(write_point> (s_vb_arm_record_buf.out<<1))
    {
        byte_count_1 = write_point - (s_vb_arm_record_buf.out<<1);
        if(byte_count_1 >= (uiCount<<1))
            byte_count_1 = (uiCount<<1);            
        ARMVB_Record_CopyAudioData(puiDestData,
            s_vb_arm_record_buf.data_ptr,
            s_vb_arm_record_buf.data_ptr_r,
            s_vb_arm_record_buf.out,
            byte_count_1>>1);
    }
    else
    {
        byte_count_1 = (s_vb_arm_record_buf.size<<1)-(s_vb_arm_record_buf.out<<1) ;
        byte_count_2 = write_point;
        if(byte_count_1 >= (uiCount<<1))
        {
            byte_count_1 = (uiCount<<1);    
            byte_count_2 = 0;
        }
        else
        {
            if( byte_count_2 > ((uiCount<<1)-byte_count_1) )
            {
                byte_count_2 = (uiCount<<1)-byte_count_1;
            }
        }
        ARMVB_Record_CopyAudioData(puiDestData,
            s_vb_arm_record_buf.data_ptr,
            s_vb_arm_record_buf.data_ptr_r,
            s_vb_arm_record_buf.out,
            byte_count_1>>1);

        if( byte_count_2 )
        {
            ARMVB_Record_CopyAudioData((puiDestData+(byte_count_1>>1)),
                s_vb_arm_record_buf.data_ptr,
                s_vb_arm_record_buf.data_ptr_r,
                0,
                byte_count_2>>1);
        }
    }
    *puiDataLen = byte_count_1+byte_count_2;
    SCI_TRACE_LOW("[zgt]  fuc[%s] addr[0x%x] uiCount[%d]puiDataLen[%d] ", __FUNCTION__,puiDestData,uiCount,*puiDataLen);

}

#define RECORD_FADEIN_TIME 250

#define FADEIN_SHIFT_BITS 10
static int g_fadein_samples;
static int g_fadein_gain;
static int g_fadein_step;
LOCAL void _ARMVB_Record_Fadein_Prepare(int samplerate, int total_duration, int begin, int step)
{
    g_fadein_samples = total_duration * samplerate /1000;
    g_fadein_gain = begin;
    g_fadein_step = step;
    SCI_TRACE_LOW("record prepare %d %d %d, sr=%d", g_fadein_samples, g_fadein_gain, g_fadein_step, samplerate);
}

LOCAL void _ARMVB_Record_Fadein_Next(int process_len)
{
    g_fadein_samples -= process_len;
    g_fadein_gain += process_len * g_fadein_step;
}

LOCAL void _ARMVB_Record_Fadein_Process(int16 *p_data, int process_len)
{
    int gain;
    int step;

    if (g_fadein_gain <= 0)
    {
        SCI_MEMSET(p_data, 0, process_len * 2);
        return;
    }
    gain = g_fadein_gain;
    step = g_fadein_step;
    while (process_len > 0)
    {
        int32 sample = *p_data;
        sample *= gain;
        sample >>= FADEIN_SHIFT_BITS;
        process_len--;
        gain += step;
        *p_data++ = (int16)sample;
    }
}

LOCAL void ARMVB_Record_ReadData(uint16 *puiDestData, uint32 uiCount, uint32 *puiDataLen)
{
    int process_len;
    int read_len;    
    ARMVB_Record_ReadData_Internal(puiDestData, uiCount, puiDataLen);
    // fade in
    read_len = *puiDataLen >> 1;
    process_len = g_fadein_samples > read_len ? read_len : g_fadein_samples;
    if (process_len > 0)
    {
        SCI_TRACE_LOW("record process %d %d %d, len = %d(%d)", g_fadein_samples, g_fadein_gain, g_fadein_step, process_len, read_len);
        if(puiDestData)
        {
        	_ARMVB_Record_Fadein_Process((int16 *)puiDestData,process_len);
        }
    }
    return;
}

LOCAL void ARMVB_Record_MoveDataPointer (uint32 uiCount)
{
    uint32 write_point = 0;
    uint32 byte_count_1 = 0 ,byte_count_2=0;

    if((0==uiCount) || (0 == s_ad_read_size))
    {
        return;
    }

    ARMVB_Record_GetZspWritePoint(&write_point);

    if(write_point> (s_vb_arm_record_buf.out<<1))
    {
        byte_count_1 = write_point - (s_vb_arm_record_buf.out<<1);
        if(byte_count_1 >= (uiCount<<1))
            byte_count_1 = (uiCount<<1);
        s_vb_arm_record_buf.out =(s_vb_arm_record_buf.out+  (byte_count_1>>1))%( s_vb_arm_record_buf.size);
    }
    else
    {
        byte_count_1 = (s_vb_arm_record_buf.size<<1)-(s_vb_arm_record_buf.out<<1) ;
        byte_count_2 = write_point;
        if(byte_count_1 >= (uiCount<<1))
        {
            byte_count_1 = (uiCount<<1);    
            byte_count_2 = 0;
            s_vb_arm_record_buf.out =(s_vb_arm_record_buf.out+  (byte_count_1>>1))% (s_vb_arm_record_buf.size);

        }
        else
        {
            if( byte_count_2 > ((uiCount<<1)-byte_count_1) )
            {
                byte_count_2 = (uiCount<<1)-byte_count_1;
            }
            s_vb_arm_record_buf.out =(s_vb_arm_record_buf.out +  (byte_count_1>>1)
                +  (byte_count_2>>1))%( s_vb_arm_record_buf.size);
        }
    }

    ARMVB_Record_SetZspReadPoint(s_vb_arm_record_buf.out<<1);
    _ARMVB_Record_Fadein_Next(uiCount);
}

uint16 ARMVB_getRecordLenth(void)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    uint16 readoffset = zsp_sharemem->audInPara.readOffset;
    uint16 writeoffset = zsp_sharemem->audInPara.writeOffset;
    uint16 lenth = 0;
    
    SCI_TRACE_LOW("[zgt]  fuc[%s]  line[%d] readOffset[%d] writeOffset[%d], inLenth[%d]", __FUNCTION__,__LINE__, readoffset, writeoffset, zsp_sharemem->audInPara.inLenth);
    if (readoffset < writeoffset)
    {
        lenth = (writeoffset - readoffset);
    }
    else if (readoffset > writeoffset)
    {
        lenth = ((writeoffset + zsp_sharemem->audInPara.inLenth)  - readoffset) % zsp_sharemem->audInPara.inLenth;
    }
    else
    {
        lenth = 0;
    }

    SCI_TRACE_LOW("[zgt]  fuc[%s]  line[%d] lenth[%d]", __FUNCTION__,__LINE__, lenth);
    return lenth;
}

/*****************************************************************************/
//  Description:    Record DMA Callback Reg to drv
//  Author:
//  Note:
//****************************************************************************/
//void ARMVB_CAPTURE_Callback (uint32 count)
void ARMVB_CAPTURE_Callback (void)
{  
    uint32 write_point = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;

    if (s_record_callback_count>0)
    {
        uint32 i=0;
        uint32 j=0;
        
        s_ad_read_size=VB_CAPTURE_TRANS_SIZE;
        //the length of data to arm source is 160 samples one time but from zsp is according for the samplerate
        for(j=0; j<record_callback_FrameCount; j++)
        {
            for(i=0; i<s_record_callback_count; i++)
            {
                if(PNULL!=s_armrecord_callback[i])
                {
                    s_armrecord_callback[i](s_ad_read_size);
                }
            }
            SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);

            ARMVB_Record_MoveDataPointer(s_ad_read_size);
        }
    }
    else
    {
        SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);

        ARMVB_Record_GetZspWritePoint(&write_point);
        ARMVB_Record_SetZspReadPoint(write_point);
        s_vb_arm_record_buf.out = write_point/2;
    }
}
LOCAL SND_ITF_T bbatrecorditf = SND_ITF_NONE;

/*****************************************************************************/
//  Description:    Open device to record.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_Record_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    // TODO:zgt setvol and open audiorecord codec
    if (!s_b_armvb_recording)
    {    
        SND_ITF_T cur_itf = SND_ITF_NONE;
        HAL_AIF_STREAM_T mic_stream;
        AUD_LEVEL_T aud_level_cfg;
        AUD_ERR_T ret = 0;
        HAL_ERR_T hal_ret = 0;
        AUD_DEVICE_CFG_EXT_T devicecfg;

        AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
        //!!!can not memset all of zsp sharemem to zero. some status value will be keep with zsp!!! sheen
        // if do not init this para will result in assert but  it should not initialize here for zsp used !zgt
        zsp_sharemem->updateParaInd=0;//sheen

        SCI_TRACE_LOW("{xx} ARMVB_Record_Play SCI_GetSemaphore....");
        if(SCI_SUCCESS != SCI_GetSemaphore(audio_armvb_semaphore, 10))
        {
            SCI_TRACE_LOW("{xx} ARMVB_Record_Play SCI_GetSemaphore return");
            return AUDIO_CONFLICT;
        }
        SCI_TRACE_LOW("{xx} ARMVB_Record_Play SCI_GetSemaphore OK");
        SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);
        _ARMVB_Record_Fadein_Prepare(uiSampleRate, RECORD_FADEIN_TIME, 0, 0);
        if (bbatsnditf == SND_ITF_NONE)
        {
            cur_itf = SND_ITF_LOUD_SPEAKER;
        }
        else
        {
            cur_itf = bbatsnditf;
            bbatrecorditf = bbatsnditf;
        }
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] uiSampleRate[%d]  ", __FUNCTION__,__LINE__,uiSampleRate);
        //init mic path
        devicecfg.inputType = AUD_INPUT_TYPE_MAINMIC;
        devicecfg.inputCircuityType= AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
        devicecfg.inputPath= AUD_INPUT_PATH1;
        devicecfg.linePath= AUD_LINE_PATH2;
        devicecfg.spkpaType= g_spkpatype;
        //aud_SetAudDeviceCFG(devicecfg);
        //AUDIO_CP_SetDevCFG(devicecfg);
        
        zsp_sharemem->audInPara.sampleRate = uiSampleRate;
        //_ARMVB_Record_Fadein_Prepare(uiSampleRate, RECORD_FADEIN_TIME, 0, 0);
        //init s_vb_arm_record_buf
        SCI_MEMSET(&s_vb_arm_record_buf,0,sizeof(VB_ARM_RECORD_BUF));
        //s_vb_arm_record_buf.data_ptr = (uint16 *)AUDIO_ALLOC(TMP_FIFO_SIZE<<4);    //160*8 half-word
        // s_vb_arm_record_buf.data_ptr =(uint16 *)(&(zsp_sharemem->audInput[0]));
        s_vb_arm_record_buf.data_ptr =(uint16 *)(zsp_sharemem->audInput);

        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] data_ptr[0x%x]  ", __FUNCTION__,__LINE__, s_vb_arm_record_buf.data_ptr );

        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] outaddr[0x%x]  ", __FUNCTION__,__LINE__,  zsp_sharemem->audOutPara.bitrate);

        s_vb_arm_record_buf.in = 0;
        s_vb_arm_record_buf.out = 0;
        s_vb_arm_record_buf.size = AUDIO_INPUT_BUF_SIZE>>1;//7k byte

        zsp_sharemem->audInPara.channelNb = HAL_AIF_MONO;//need to complet
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_MONO;
        zsp_sharemem->audInPara.bitsPerSample = 16;
#if 0 // AUDIO_CP_USE_ZSP_PARA
        zsp_sharemem->musicMode =5;//record wav MODE
#endif
        g_zsp_para.musicMode = 5;
        zsp_sharemem->audInPara.readOffset= 0;
        zsp_sharemem->audInPara.writeOffset= 0;
#if 0 // AUDIO_CP_USE_ZSP_PARA
        zsp_sharemem->audInPara.sbcOutFlag =0;//sheen
#endif
        g_zsp_para.sbcOutFlag = 0;
        zsp_sharemem->audInPara.fileEndFlag = 0;

        //init mic_Stream
        if(8000 ==  zsp_sharemem->audInPara.sampleRate)
        {
            mic_stream.startAddress = (zsp_sharemem->txPCM.txPcmBuffer.pcmBuf); 
            //mic_stream.startAddress = &(zsp_sharemem->txPCM.txPcmBuffer); 
            mic_stream.length = sizeof(HAL_SPEECH_PCM_BUF_T);     
        }
        else
        {
            mic_stream.startAddress = (zsp_sharemem->txPCM.txPcmVolte.pcmBuf); 
            mic_stream.length = sizeof(HAL_VOLTE_PCM_BUF_T);
        }
        //for 16k samplerate record.zgt
        if(8000 ==  zsp_sharemem->audInPara.sampleRate)
        {
            record_callback_FrameCount =1;     
        }
        else if (16000 ==  zsp_sharemem->audInPara.sampleRate)
        {
            record_callback_FrameCount =2; 
        }

        mic_stream.sampleRate = uiSampleRate;
        mic_stream.channelNb = HAL_AIF_MONO;
        mic_stream.voiceQuality = FALSE;
        mic_stream.playSyncWithRecord = 0;
        mic_stream.halfHandler = NULL;
        mic_stream.endHandler = NULL;
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] sampleRate[%d]  ", __FUNCTION__,__LINE__, mic_stream.sampleRate );

        //init cfg 
        //ARMVB_Record_SetDevMode (ptDevSetting->eDev_mode);
        //ARMVB_Record_SetVolume (ptDevSetting->uiVol, ptDevSetting->eAppType);
        aud_level_cfg.spkLevel = SND_SPK_VOL_7; 
        aud_level_cfg.micLevel = SND_MIC_ENABLE;
        aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
        aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
        aud_level_cfg.appMode = SND_APP_MODE_RECORD;

        if(PNULL == s_vb_arm_record_buf.data_ptr)
        {
             SCI_TRACE_LOW("[zgt] fuc[%s] line[%d]s_vb_arm_record_buf error ", __FUNCTION__,__LINE__);
            return AUDIO_ERROR;
        }
        ipc_register_music_notify(ARMVB_CAPTURE_Callback);
#if 0///ndef ARMVB_USE_CP_AUDIO_INTERFACE
#ifdef ARMVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
	AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_RECORD);
#endif
        //call zsp 
#ifdef ARMVB_USE_DM_TONE_INTERFACE
        DM_ZspMusicPlayStart();
#else
        hal_ret = hal_zspMusicPlayStart();
        if (0 != hal_ret)
        {
            SCI_TRACE_LOW("[zgt] line[%d] ARMVB_record call hal_zspMusicPlayStart faile", __LINE__);
        }
#endif
        //start record stream
#ifndef ARMVB_USE_DM_TONE_INTERFACE
        ret = aud_StreamRecord(cur_itf, &mic_stream, &aud_level_cfg);
        if (0 != ret) 
        {
            SCI_TRACE_LOW("[zgt] line[%d] ARMVB_record call aud_StreamStart faile", __LINE__);
        }
#else
        ret = DM_AudStreamRecord(cur_itf, &mic_stream, &aud_level_cfg);
        if (0 != ret) 
        {
            SCI_TRACE_LOW("[zgt] line[%d] ARMVB_record call DM_AudStreamRecord faile", __LINE__);
        }
#endif
#else
        ret =AUDIO_CP_START(AUDIO_DEV_RECORD,cur_itf, &mic_stream, &aud_level_cfg,0,devicecfg,0, &g_zsp_para);
        if (AUD_ERR_NO != ret)
        {
            SCI_TRACE_LOW("zgt line[%d] ARMVB_Record_Play call AUDIO_CP_START faile ret:0x%x", __LINE__, ret);
            return ret;
        }
#endif

        s_ad_read_size = 0;
        s_record_callback_count = 0;
        s_b_armvb_recording = SCI_TRUE;
    }
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Close device to stop record.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_Record_Stop (DEV_SETTING_T *ptDevSetting)
{
    SND_ITF_T cur_itf = SND_ITF_NONE;
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);
    if (s_b_armvb_recording)
    {
#if 0 // AUDIO_CP_USE_ZSP_PARA
        zsp_sharemem->musicMode = 0;
#endif
        g_zsp_para.musicMode = 0;
        if (bbatrecorditf == SND_ITF_NONE)
        {
            cur_itf = SND_ITF_LOUD_SPEAKER;
        }
        else
        {
            cur_itf = bbatrecorditf;
        }
        //SCI_ASSERT(0);
#if 0//ndef ARMVB_USE_CP_AUDIO_INTERFACE
#ifndef ARMVB_USE_DM_TONE_INTERFACE
        ret = aud_StreamRecordStop(cur_itf);
        if (0 != ret)
        {
            SCI_TRACE_LOW("[zgt] line[%d] ARMVB_Record_Stop call aud_StreamStop faile", __LINE__);
        }
#else
        ret = DM_AudStreamRecordStop(cur_itf);
        if (0 != ret)
        {
            SCI_TRACE_LOW("[zgt] line[%d] ARMVB_Record_Stop call DM_AudStreamRecordStop faile", __LINE__);
        }
#endif
#ifdef ARMVB_USE_DM_TONE_INTERFACE
        DM_ZspMusicPlayStop();
#else
        hal_ret = hal_zspMusicPlayStop();
        if (0 != hal_ret)
        {
            SCI_TRACE_LOW("[zgt] line[%d] ARMVB_Record_Stop call hal_zspMusicPlayStop faile", __LINE__);
        }
#endif
#ifdef ARMVB_USE_PCIM_26M_CLOCK
		//PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_RECORD);
#endif
#else
    ret = AUDIO_CP_STOP(AUDIO_DEV_RECORD,cur_itf, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("zgt line[%d] ARMVB_record_Stop call AUDIO_CP_STOP faile ret:0x%x", __LINE__, ret);
        return ret;
    }
#endif
        if(s_vb_arm_record_buf.data_ptr)
        {
            s_vb_arm_record_buf.data_ptr = PNULL;
        }
        SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);

        s_ad_read_size = 0;
        s_record_callback_count = 0;
        record_callback_FrameCount =0;  
        s_b_armvb_recording = SCI_FALSE;
        bbatrecorditf = SND_ITF_NONE;
    SCI_TRACE_LOW("{xx} ARMVB_Record_Stop SCI_PutSemaphore....");
    SCI_PutSemaphore(audio_armvb_semaphore);
    SCI_TRACE_LOW("{xx} ARMVB_Record_Stop SCI_PutSemaphore OK");
    }
    
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Register device controlled by arm to audio service.
//  Author:         
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_Record_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_RECORD"), strlen ("ARMVB_RECORD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_Record_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_Record_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_Record_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_Record_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_Record_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_Record_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = ARMVB_Record_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = ARMVB_Record_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = ARMVB_Record_ReadData;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_Record_GetAppropriateOutputSamplerate;
#ifdef AUD_ARM_RECORD_EXP
    tDeviceInfo.tAudioDevOpe.pPreClose = ARMVB_Record_PreClose;
    tDeviceInfo.tAudioDevOpe.pPostOpen = ARMVB_Record_PostOpen;
#endif

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    //AUDIO_HAL_Register_Capture((AUDIO_HAL_CALLBACK_FUN)ARMVB_CAPTURE_Callback);
    g_zsp_share_memory_addr = GET_MemoryStartAddressAndLen(MEM_AUDIO_SM_NAME, &g_zsp_share_memory_lenth);
    //ipc_register_music_notify(ARMVB_CAPTURE_Callback);

    return hDevice;
}

#ifndef BUILDIN_FM
/*****************************************************************************/
//  Description:    Set Arm dolphin mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void ARMVB_Linein_SetMute (
    BOOLEAN is_mute
)
{
    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;   
    AUDMOD_DEVICECTL_T aud_mod_param_t = {0};
    AUDIO_NV_ARM_MODE_INFO_T *ptAudMode = PNULL;
    
    AUDIO_NV_ARM_RESULT_E result = AUDIO_NV_ARM_NO_ERROR;
    char *mode_name = PNULL;
    
    AUDIO_GetDevMode (&cur_dev_mode);
    
    mode_name = _ARMVB_ArmvbNv_GetName(cur_dev_mode);
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_Linein_SetMute mode:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1620_112_2_18_1_2_13_63,(uint8*)"d", cur_dev_mode);
        return;
    }
   
    result=AUDIONVARM_GetAudioModeDevCtrInfo (mode_name, AUDIO_DEVICE_MODE_PARAM_PLAY, (AUDMOD_DEVICECTL_T *)(&aud_mod_param_t));    
    if(AUDIO_NV_ARM_NO_ERROR!=result)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_Linein_SetMute failed to get dev ctr info, result:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1627_112_2_18_1_2_13_64,(uint8*)"d", result);
        return;
    }

    //SCI_TRACE_LOW:"ARMVB_Linein_SetMute : %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1631_112_2_18_1_2_13_65,(uint8*)"d", is_mute);

    s_is_vb_linein_mute = is_mute;
    
    if(is_mute)
    {   
        ARMVB_DrvOutSwitch(0, AUDIO_EAR, SCI_FALSE);
        ARMVB_DrvOutSwitch(0, AUDIO_HP, SCI_FALSE);
        ARMVB_DrvOutSwitch(0,AUDIO_SPK,SCI_FALSE);
    }
    else
    {
        // Open or close the earphone according to current mode
        if (VB_DA_PRI_OUTPUT == aud_mod_param_t.ucDAChannel)
        {
                ARMVB_DrvOutSwitch(0, AUDIO_EAR, SCI_TRUE);        
        }
        else if (VB_DA_AUX_OUTPUT == aud_mod_param_t.ucDAChannel)
        {
                ARMVB_DrvOutSwitch(0, AUDIO_EAR, SCI_FALSE);
        }
        else
        {
            //SCI_TRACE_LOW:"ARMVB_Linein_SetMute : unknown ucDAChanne!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1654_112_2_18_1_2_13_66,(uint8*)"");
        }

        // Open or close the headphone according to current mode
        if (VB_AUD_STEREO == aud_mod_param_t.ucStereoEn)
        {
                ARMVB_DrvOutSwitch(0, AUDIO_HP, SCI_TRUE);
        }
        else if (VB_AUD_NON_STEREO == aud_mod_param_t.ucStereoEn)
        {
                ARMVB_DrvOutSwitch(0, AUDIO_HP, SCI_FALSE);
        }
        else
        {
            //SCI_TRACE_LOW:"ARMVB_Linein_SetMute : unknown ucStereoEn!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1668_112_2_18_1_2_13_67,(uint8*)"");
        }

        ARMVB_DrvOutSwitch(0,AUDIO_SPK,aud_mod_param_t.ucSpkAmpEn);

        ptAudMode = (AUDIO_NV_ARM_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
        if(PNULL==ptAudMode)
        {
            //SCI_TRACE_LOW:"armvb_as.c, ARMVB_Linein_SetMute, alloc fail, size: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1676_112_2_18_1_2_13_68,(uint8*)"d",sizeof(AUDIO_NV_ARM_MODE_INFO_T));
            return;
        }

        if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam(mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudMode)))
        {
            //SCI_TRACE_LOW:"armvb_as.c failed to get mode param."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1682_112_2_18_1_2_13_69,(uint8*)"");
            SCI_FREE(ptAudMode);
            return;
        }    

#ifdef FM_VBC
        AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DAC, (uint32)(!is_mute));
#endif

#ifdef EQ_EXE_ON_CHIP
        ARMVB_SetDevModeParam ((AUDIO_NV_ARM_MODE_INFO_T *)(ptAudMode));
#else
        AGC_ReInit_Param (cur_dev_mode);
#endif         
        SCI_FREE(ptAudMode);
    }
}

/*****************************************************************************/
//  Description:    ARMVB logic device set linein output volume.
//  Author:         Shujing.dong
//  Note:
//****************************************************************************/
LOCAL void ARMVB_Linein_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    uint32 gobl_gain=0;
    uint32 uiIndexBase = 0;
    char *mode_name = NULL;
    AUDIO_NV_ARM_MODE_INFO_T *ptAudMode = PNULL;
    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;

    AUDIO_GetDevMode (&cur_dev_mode);

    //SCI_TRACE_LOW:"ARMVB_Linein_SetVolume uiVol:%d, max:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1710_112_2_18_1_2_13_70,(uint8*)"dd", uiVol, ARMVB_LINEIN_RESERVER_VOL_LEVEL-1);

    if(uiVol>ARMVB_LINEIN_RESERVER_VOL_LEVEL)
    {
        uiVol=(ARMVB_LINEIN_RESERVER_VOL_LEVEL);
    }
    else if(uiVol<1)
    {
        uiVol = 1;
    }

    mode_name = _ARMVB_ArmvbNv_GetName(cur_dev_mode);

    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_Linein_SetVolume mode:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1721_112_2_18_1_2_13_71,(uint8*)"d", cur_dev_mode);
        return;
    }

    ptAudMode = (AUDIO_NV_ARM_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_Linein_SetVolume, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1728_112_2_18_1_2_13_72,(uint8*)"d",sizeof(AUDIO_NV_ARM_MODE_INFO_T));
        return;
    }

    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam(mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudMode)))
    {
        SCI_FREE(ptAudMode);
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_Linein_SetVolume failed to get mode param."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1735_112_2_18_1_2_13_73,(uint8*)"");
        return;
    }      

    s_vb_linein_volumn = uiVol;

    uiIndexBase = ARMVB_LINEIN_VOL_LEVEL_INDEX_BASE;

    SCI_PASSERT(((uiIndexBase+uiVol)<AUDIO_NV_ARM_PARA_RESERVE),      /*assert verified*/
        ("ARMVB_Linein_SetVolume overflow, indexbase:%d, vol:%d, indexMax:%d",
    uiIndexBase, uiVol, (ARMVB_LINEIN_RESERVER_VOL_LEVEL-1)));

    gobl_gain = ptAudMode->tAudioNvArmModeStruct.reserve[uiIndexBase+uiVol-1]; // 0:mute, 1-15: volume level 1-15 

    SCI_TRACE_LOW("armvb_as.c ARMVB_Linein_SetVolume gain:0x%x.", gobl_gain);

    if(AUDIO_DEVICE_MODE_HANDFREE == cur_dev_mode)
    {
        ARMVB_SetPgaLevel(0,AUDIO_ALL_CHAN,gobl_gain,AUDIO_LINEIN | AUDIO_SPK);    
    }else if(AUDIO_DEVICE_MODE_EARPHONE == cur_dev_mode)
    {
        ARMVB_SetPgaLevel(0,AUDIO_ALL_CHAN,gobl_gain,AUDIO_LINEIN | AUDIO_HP);    
    }else if(AUDIO_DEVICE_MODE_EARFREE == cur_dev_mode)
    {
        ARMVB_SetPgaLevel(0,AUDIO_ALL_CHAN,gobl_gain,AUDIO_LINEIN | AUDIO_SPK);    
        ARMVB_SetPgaLevel(0,AUDIO_ALL_CHAN,gobl_gain,AUDIO_LINEIN | AUDIO_HP);
    }

#ifdef FM_VBC
        {
            int32  dg_level = 0;

            gobl_gain = ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[eAppType].arm_volume[uiVol & 0xF];
#ifndef EQ_HAS_MULTI_BANDS
            dg_level = (int32) ( (int32) gobl_gain >> 16) ; // 16Bit signed
#else
            dg_level = (gobl_gain & 0xFFFF0000)>>16;// 16Bit unsigned  //cherry check here!
#endif

            AUD_SetDgGain (dg_level);
        }
#endif//FM_VBC

    SCI_FREE(ptAudMode);
    }

/*****************************************************************************/
//  Description:    ARMVB logic device set device mode.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL void ARMVB_Linein_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode ", __LINE__);
    return;
}

/*****************************************************************************/
//  Description:    Open device to start to play from linein.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_Linein_Play(uint32 uiSampleRate, DEV_SETTING_T * ptDevSetting) 
{
    AUDMOD_DEVICECTL_T aud_mod_param_t = {0};
    AUDIO_NV_ARM_RESULT_E result = AUDIO_NV_ARM_NO_ERROR;
    char *mode_name = PNULL;
    mode_name = _ARMVB_ArmvbNv_GetName(ptDevSetting->eDev_mode);
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_Play mode:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1875_112_2_18_1_2_14_81,(uint8*)"d", ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }
    
    result=AUDIONVARM_GetAudioModeDevCtrInfo (mode_name, AUDIO_DEVICE_MODE_PARAM_PLAY, (AUDMOD_DEVICECTL_T *)(&aud_mod_param_t));    
    if(AUDIO_NV_ARM_NO_ERROR!=result)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_Linein_Play failed to get dev ctr info, result:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1882_112_2_18_1_2_14_82,(uint8*)"d", result);
        return AUDIO_ERROR;
    }

    if (!s_b_armvb_linein_playing)
    {        
        
        uint32 aud_dev_open_return = AUDIO_HAL_SUCCESS;

#ifdef FM_VBC
        aud_dev_open_return = AUDIO_HAL_Open(AUDIO_FUN_LINEIN_DIGITAL|AUDIO_FUN_PLAYBACK,uiSampleRate);    
#else
        aud_dev_open_return = AUDIO_HAL_Open(AUDIO_FUN_LINEIN|AUDIO_FUN_PLAYBACK,uiSampleRate);    
#endif//FM_VBC
        if(AUDIO_HAL_SUCCESS != aud_dev_open_return)
        {
            //SCI_TRACE_LOW:"ARMVB_Linein_Play AUDDEV_Open failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1908_112_2_18_1_2_14_84,(uint8*)"");
            return AUDIO_ERROR;
        }

        ARMVB_Linein_SetMute (ptDevSetting->bDownlink_mute); 
        ARMVB_DrvInSwitch(0,AUDIO_LINEIN, SCI_TRUE);
        ARMVB_Linein_SetDevMode (ptDevSetting->eDev_mode);        
        ARMVB_Linein_SetVolume (ptDevSetting->uiVol, ptDevSetting->eAppType);
        s_b_armvb_linein_playing = SCI_TRUE;

        return AUDIO_NO_ERROR;
    }    

    return AUDIO_ERROR;
}

/*****************************************************************************/
//  Description:    Close device to stop play from linein.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_Linein_Stop (DEV_SETTING_T *ptDevSetting)
{
    AUDMOD_DEVICECTL_T aud_mod_param_t = {0};
    AUDIO_NV_ARM_RESULT_E result = AUDIO_NV_ARM_NO_ERROR;
    char *mode_name = PNULL;

    mode_name = _ARMVB_ArmvbNv_GetName(ptDevSetting->eDev_mode);
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_Linein_Stop mode:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1938_112_2_18_1_2_14_85,(uint8*)"d", ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }
    
    result=AUDIONVARM_GetAudioModeDevCtrInfo (mode_name, AUDIO_DEVICE_MODE_PARAM_PLAY, (AUDMOD_DEVICECTL_T *)(&aud_mod_param_t));    
    if(AUDIO_NV_ARM_NO_ERROR!=result)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_Linein_Stop failed to get dev ctr info, result:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1945_112_2_18_1_2_14_86,(uint8*)"d", result);
        return AUDIO_ERROR;
    }

    if (s_b_armvb_linein_playing) //uiTotalTrackOccupied
    {
#ifdef FM_VBC
        AUDIO_HAL_Close(AUDIO_FUN_LINEIN_DIGITAL|AUDIO_FUN_PLAYBACK);            
#else
        AUDIO_HAL_Close(AUDIO_FUN_LINEIN|AUDIO_FUN_PLAYBACK);
#endif
        s_b_armvb_linein_playing = SCI_FALSE;

        if(s_b_linein_ad_playing)
        {
            ARMVB_LineinAd_SetDevMode(ptDevSetting->eDev_mode);
        }

        return AUDIO_NO_ERROR;
    }
    else
    {
        //SCI_TRACE_LOW:"ARMVB_Linein_Stop failed."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_1964_112_2_18_1_2_14_87,(uint8*)"");
        return AUDIO_ERROR;
    }
}
#endif
/*****************************************************************************/
//  Description:    Register linein logic device controlled by arm to play  
//                  from linein channel to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_Linein_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
#ifndef BUILDIN_FM
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_LINEIN"), strlen ("ARMVB_LINEIN") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_Linein_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_Linein_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_Linein_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_Linein_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_Linein_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_Linein_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = ARMVB_Linein_SetMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetLineinAppropriateOutputSamplerate;
#ifdef FM_VBC
    tDeviceInfo.tAudioDevOpe.pPreClose = ARMVB_FM_PreClose;
    tDeviceInfo.tAudioDevOpe.pPostOpen = ARMVB_FM_PostOpen;
#endif//FM_VBC

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    g_armvb_linein_device_ptr = AUDIO_DM_GetDeviceHandleStruct (hDevice);  // Record device structure    
#endif
    return hDevice;
}

/*****************************************************************************/
//  Description:    Register linein logic device controlled by arm to play  
//                  from linein channel to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_FM_RegExternalAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
#ifndef BUILDIN_FM
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_FM"), strlen ("ARMVB_FM") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_Linein_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_Linein_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_Linein_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_Linein_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_Linein_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_Linein_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = ARMVB_Linein_SetMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetLineinAppropriateOutputSamplerate;
#ifdef FM_VBC
    tDeviceInfo.tAudioDevOpe.pPreClose = ARMVB_FM_PreClose;
    tDeviceInfo.tAudioDevOpe.pPostOpen = ARMVB_FM_PostOpen;
#endif//FM_VBC

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    g_armvb_fm_device_ptr = AUDIO_DM_GetDeviceHandleStruct (hDevice);  // Record device structure    
#endif
    return hDevice;
}

#ifdef FM_VBC
LOCAL AUDIO_RESULT_E ARMVB_FM_PostOpen(void *ptParam)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
    return 0;
}

LOCAL AUDIO_RESULT_E ARMVB_FM_PreClose(void *ptParam)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
    return 0;
}
#endif//FM_VBC
#ifdef BUILDIN_FM
LOCAL void ARMVB_FMAd_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode);

LOCAL uint32 ARMVB_GetFMAppropriateOutputSamplerate (uint32 uiSampleRate)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
    return uiSampleRate;
}

/*****************************************************************************/
//  Description:    Set Arm dolphin mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void ARMVB_FM_SetMute (
    BOOLEAN is_mute
)
{
    AUD_ERR_T aud_ret = 0;

    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetMute is_mute:0x%x g_cur_itf:0x%x", __LINE__, is_mute, g_cur_itf);
    if (TRUE == is_mute)
    {
        g_volbeforemute = g_aud_level_cfg.spkLevel;
        g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
    }
    else
    {
        g_aud_level_cfg.spkLevel = g_volbeforemute;
    }
    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_FMPLAY;
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    aud_ret = aud_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#else
    aud_ret = DM_AudSetup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
#else
    aud_ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetMute aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description:    ARMVB logic device set linein output volume.
//  Author:         Shujing.dong
//  Note:
//****************************************************************************/
LOCAL void ARMVB_FM_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    AUD_ERR_T aud_ret = 0;

    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetVolume uiVol:0x%x eAppType:0x%x g_cur_itf:0x%x", __LINE__, uiVol, eAppType, g_cur_itf);
    g_aud_level_cfg.spkLevel = uiVol;
    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_FMPLAY;
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    aud_ret = aud_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#else
    aud_ret = DM_AudSetup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
#else
    aud_ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
    if (AUD_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetVolume aud_ret is [0x%x] WRONG!!!", __LINE__, aud_ret);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description:    ARMVB logic device set device mode.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL void ARMVB_FM_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    AUD_ERR_T ret = 0;
    INT8 delay_cnt = 10;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)GET_MemoryStartAddressAndLen(MEM_AUDIO_SM_NAME, &g_zsp_share_memory_lenth);
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode eMode:0x%x", __LINE__, eMode);
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call aud_StreamStop faile", __LINE__);
    }
#else
    ret = DM_AudStreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call DM_AudStreamStop faile", __LINE__);
    }
#endif
#if 1
    //  just for clean zsp buffer after close codec
    while (delay_cnt > 0)
    {
        ret = DM_VoisSetBtNRECFlag(0);
        if (1 == ret)
        {
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call DM_VoisSetBtNRECFlag sleep 50ms ret:0x%x", __LINE__, ret);
            SCI_SLEEP(50);
            delay_cnt--;
        }
        else
        {
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call DM_VoisSetBtNRECFlag succ and return ret:0x%x", __LINE__, ret);
            break;
        }
    }
#endif
#else
    ret = AUDIO_CP_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call AUDIO_CP_StreamStop faile", __LINE__);
    }
    ret = AUDIO_CP_GetCodecStatus(0);
    if( CODEC_PLAYING==ret)
    {
        SCI_TRACE_LOW("zgt ARMVB_FM_SetDevMode get CODEC ERROR =%d", ret);
    }
#endif
    SCI_MEMSET(zsp_sharemem->Output.audOutput, 0, (9 * 1024));

    switch(eMode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;

        default:
            break;
    }

    g_aif_stream.startAddress = (uint32 *)zsp_sharemem->Output.audOutput;
    g_aif_stream.length = AUDIO_INPUT_BUF_SIZE;
    g_aud_level_cfg.appMode = SND_APP_MODE_FMPLAY;
    #if 0
#if 1 // vibration mode
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode g_shakingMode:0x%x", __LINE__, g_shakingMode);
    aud_SetSharkingMode(g_shakingMode);
#else
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode g_shakingMode:0x%x", __LINE__, g_shakingMode);
    DM_AudSetSharkingMode(g_shakingMode);
#endif
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call aud_StreamStart faile", __LINE__);
    }
#else
    ret = DM_AudStreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call DM_AudStreamStart faile", __LINE__);
    }
#endif
#else
    AUDIO_CP_SetSharkingMode(g_shakingMode);
    ret = AUDIO_CP_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call AUDIO_CP_StreamStart faile", __LINE__);
    }
    if (eMode == AUDIO_DEVICE_MODE_EARFREE)
    {
        g_cur_itf = SND_ITF_LOUD_SPEAKER;
        ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
        if (0 != ret)
        {
            SCI_TRACE_LOW("zgt line[%d] ARMVB_FM_SetDevMode call AUDIO_CP_Setup faile ret:0x%x", __LINE__, ret);
        }
    }
#endif
    g_aif_stream.startAddress = (uint32 *)zsp_sharemem->audInput;
    g_aif_stream.length = AUDIO_INPUT_BUF_SIZE;
    g_aud_level_cfg.appMode = SND_APP_MODE_FMRECORD;
#if 0
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamRecord(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call aud_StreamStart faile", __LINE__);
    }
#else
    ret = DM_AudStreamRecord(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call DM_AudStreamRecord faile", __LINE__);
    }
#endif
#else
    ret = AUDIO_CP_RecordStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_SetDevMode call AUDIO_CP_RecordStart faile", __LINE__);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description:    Open device to start to play from linein.
//  Author:
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_FM_Play(uint32 uiSampleRate, DEV_SETTING_T * ptDevSetting)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_DEVICE_CFG_EXT_T devicecfg;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)GET_MemoryStartAddressAndLen(MEM_AUDIO_SM_NAME, &g_zsp_share_memory_lenth);
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d] zsp_sharemem:0x%x g_zsp_share_memory_lenth:0x%x", __FILE__, __FUNCTION__, __LINE__, zsp_sharemem, g_zsp_share_memory_lenth);
    SCI_TRACE_LOW("[ljf] func[%s] line[%d] eDev_mode:0x%x uiVol:0x%x bDownlink_mute:0x%x", __FUNCTION__, __LINE__, ptDevSetting->eDev_mode, ptDevSetting->uiVol, ptDevSetting->bDownlink_mute);

    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;

        default:
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_aif_stream.startAddress = (uint32 *)zsp_sharemem->Output.audOutput;
    g_aif_stream.length = AUDIO_INPUT_BUF_SIZE;
    zsp_sharemem->audInPara.sampleRate = 48000;
    zsp_sharemem->audOutPara.samplerate = 48000;
    zsp_sharemem->audInPara.channelNb = HAL_AIF_STEREO;
    zsp_sharemem->audOutPara.channelNb = HAL_AIF_STEREO;
    zsp_sharemem->audInPara.bitsPerSample = 16;
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_sharemem->musicMode =8; // 8 FM_INIT
#endif
    g_zsp_para.musicMode = 8;
    zsp_sharemem->audOutPara.length = ((AUDIO_INPUT_BUF_SIZE / 2) / zsp_sharemem->audInPara.channelNb);

    g_aif_stream.channelNb = HAL_AIF_STEREO;
    g_aif_stream.playSyncWithRecord = 0;
    g_aif_stream.sampleRate = 48000;
    g_aif_stream.voiceQuality = TRUE;
    g_aif_stream.halfHandler = NULL;
    g_aif_stream.endHandler = NULL;
    if(0==ptDevSetting->bDownlink_mute)
    {
        g_aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)ptDevSetting->uiVol;
    }
    else
    {
        g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
    }
    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_FMPLAY;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    SCI_MEMSET(zsp_sharemem->Output.audOutput, 0, AUDIO_OUTPUT_BUF_SIZE);
    SCI_MEMSET(zsp_sharemem->audInput, 0, AUDIO_INPUT_BUF_SIZE);


#if 0//ndef ARMVB_USE_CP_AUDIO_INTERFACE
#ifdef ARMVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
	AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_FM);
#endif
#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStart();
#else
    hal_ret = hal_zspMusicPlayStart();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play call hal_zspMusicPlayStart faile", __LINE__);
    }
#endif
#if 1 // vibration mode
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play g_shakingMode:0x%x", __LINE__, g_shakingMode);
    aud_SetSharkingMode(g_shakingMode);
#else
    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play g_shakingMode:0x%x", __LINE__, g_shakingMode);
    DM_AudSetSharkingMode(g_shakingMode);
#endif
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play call aud_StreamStart faile", __LINE__);
    }
#else
    ret = DM_AudStreamStart(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play call DM_AudStreamStart faile", __LINE__);
    }
#endif
    g_aif_stream.startAddress = (uint32 *)zsp_sharemem->audInput;
    g_aif_stream.length = AUDIO_INPUT_BUF_SIZE;
    g_aud_level_cfg.appMode = SND_APP_MODE_FMRECORD;
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamRecord(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play call aud_StreamStart faile", __LINE__);
    }
#else
    ret = DM_AudStreamRecord(g_cur_itf, &g_aif_stream, &g_aud_level_cfg);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play call DM_AudStreamRecord faile", __LINE__);
    }
#endif
#else
    devicecfg.inputType = AUD_INPUT_TYPE_MAINMIC;
    devicecfg.inputCircuityType= AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    devicecfg.inputPath= AUD_INPUT_PATH1;
    devicecfg.linePath= AUD_LINE_PATH2;
    devicecfg.spkpaType = g_spkpatype;
    ret =AUDIO_CP_START(AUDIO_DEV_FM,g_cur_itf, &g_aif_stream, &g_aud_level_cfg,0,devicecfg,0, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("zgt line[%d] ARMVB_FM_Play call AUDIO_CP_START faile ret:0x%x", __LINE__, ret);
        return ret;
    }
   if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_EARFREE)
    {    
        SCI_TRACE_LOW("line[%d] ARMVB_FM_Play need call AUDIO_CP_Setup appMode:0x%x", __LINE__, g_aud_level_cfg.appMode);
        g_cur_itf = SND_ITF_LOUD_SPEAKER;
        g_aud_level_cfg.appMode = SND_APP_MODE_FMPLAY;
        ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
        if (0 != ret)
        {
            SCI_TRACE_LOW("zgt line[%d] ARMVB_FM_Play call AUDIO_CP_Setup faile ret:0x%x", __LINE__, ret);
        }
    }
#endif
    SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);

    return 0;
    }

/*****************************************************************************/
//  Description:    Close device to stop play from linein.
//  Author:
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_FM_Stop (DEV_SETTING_T *ptDevSetting)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    INT8 delay_cnt = 10;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] eDev_mode:0x%x", __LINE__, __FILE__, __FUNCTION__, ptDevSetting->eDev_mode);

    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;

        default:
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
#if 0 // AUDIO_CP_USE_ZSP_PARA
    zsp_sharemem->musicMode = 0;
#endif
    g_zsp_para.musicMode = 0;
#if 0//ndef ARMVB_USE_CP_AUDIO_INTERFACE
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Stop call aud_StreamStop faile", __LINE__);
    }
#else
    ret = DM_AudStreamStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Stop call DM_AudStreamStop faile", __LINE__);
    }
#endif
#ifndef ARMVB_USE_DM_TONE_INTERFACE
    ret = aud_StreamRecordStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Stop call aud_StreamRecordStop faile", __LINE__);
    }
#else
    ret = DM_AudStreamRecordStop(g_cur_itf);
    if (0 != ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Stop call DM_AudStreamRecordStop faile", __LINE__);
    }
#endif
#ifdef ARMVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStop();
#else
    hal_ret = hal_zspMusicPlayStop();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Stop call hal_zspMusicPlayStop faile", __LINE__);
    }
#endif
    // just for close 26M after close codec
    while (delay_cnt > 0)
    {
        ret = DM_VoisSetBtNRECFlag(0);
        if (1 == ret)
        {
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Stop call DM_VoisSetBtNRECFlag sleep 50ms ret:0x%x", __LINE__, ret);
            SCI_SLEEP(50);
            delay_cnt--;
        }
        else
        {
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_FM_Stop call DM_VoisSetBtNRECFlag succ and return ret:0x%x", __LINE__, ret);
            break;
        }
    }

#ifdef ARMVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_FM);
#endif
#else
    ret = AUDIO_CP_STOP(AUDIO_DEV_FM,g_cur_itf, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("zgt line[%d] ARMVB_FM_Stop call AUDIO_CP_STOP faile ret:0x%x", __LINE__, ret);
        return ret;
    }
#endif

    SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    return 0;
}
#endif
/*****************************************************************************/
//  Description:    Register linein logic device controlled by arm to play  
//                  from linein channel to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_FM_RegBuildInAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
#ifdef BUILDIN_FM

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_FM"), strlen ("ARMVB_FM") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_FM_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_FM_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_FM_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_FM_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_FM_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_FM_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = ARMVB_FM_SetMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetFMAppropriateOutputSamplerate;
#ifdef FM_VBC
    tDeviceInfo.tAudioDevOpe.pPreClose = ARMVB_FM_PreClose;
    tDeviceInfo.tAudioDevOpe.pPostOpen = ARMVB_FM_PostOpen;
#endif//FM_VBC

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    g_armvb_fm_device_ptr = AUDIO_DM_GetDeviceHandleStruct (hDevice);  // Record device structure    
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
#endif

    return hDevice;
}
#ifndef BUILDIN_FM
/*****************************************************************************/
//  Description:    ARMVB logic device set linein and lineinAd output volume.
//  Author:         TH
//  Note:  
//****************************************************************************/
LOCAL void ARMVB_LineinAd_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    uint32 gain = 0;

    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    char *mode_name = NULL;
    AUDIO_NV_ARM_MODE_INFO_T *ptAudMode = PNULL;
    //Set Linein volume.
    ARMVB_Linein_SetVolume(uiVol, eAppType);
    AUDIO_GetDevMode (&cur_dev_mode);

    mode_name = _ARMVB_ArmvbNv_GetName(cur_dev_mode);
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_LineinAd_SetVolume mode:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2022_112_2_18_1_2_14_88,(uint8*)"d", cur_dev_mode);
        return;
    }

    //SCI_TRACE_LOW:"[ARMVB_LineinAd_SetVolume] uiVol=%d, mode:%d, appType:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2026_112_2_18_1_2_14_89,(uint8*)"ddd", uiVol, cur_dev_mode, eAppType);

    ptAudMode = (AUDIO_NV_ARM_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        //SCI_TRACE_LOW:"armvb_as.c, ARMVB_LineinAd_SetVolume, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2031_112_2_18_1_2_14_90,(uint8*)"d",sizeof(AUDIO_NV_ARM_MODE_INFO_T));
        return;
    }

    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudMode)))
    {
        SCI_FREE(ptAudMode);
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_LineinAd_SetVolume failed to get mode param."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2038_112_2_18_1_2_14_91,(uint8*)"");
        return;
    }    

    //    gain = ptAudMode->tAudioNvArmModeStruct.app_config_info_set.app_config_info[eAppType].arm_volume[uiVol & 0xF];
    gain = ptAudMode->tAudioNvArmModeStruct.reserve[ARMVB_LINEINAD_NV_RESERVE];
    //SCI_TRACE_LOW:"[ARMVB_LineinAd_SetVolume] gain= 0x%4x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2044_112_2_18_1_2_14_92,(uint8*)"d", gain);

    ARMVB_SetPgaLevel(0,AUDIO_ALL_CHAN,gain,AUDIO_LINEIN|AUDIO_MIC);

    SCI_FREE(ptAudMode);    


    }

/*****************************************************************************/
//  Description:    Set Arm linein ad mute or Cancel the mute_set.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
LOCAL void ARMVB_LineinAd_SetMute (
    BOOLEAN is_mute
)
{
    ARMVB_Linein_SetMute(is_mute);
    g_is_vb_linein_ad_mute = is_mute;
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2064_112_2_18_1_2_14_93,(uint8*)"d", g_is_vb_linein_ad_mute);
}

/*****************************************************************************/
//  Description:    ARMVB RECORD logic device from linein set device mode.
//  Author:
//  Note:
//****************************************************************************/
LOCAL void ARMVB_LineinAd_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    AUDMOD_DEVICECTL_T aud_mod_param_record_t;
    char *mode_name = NULL;
    AUDIO_NV_ARM_RESULT_E result = AUDIO_NV_ARM_NO_ERROR;

    AUDIO_ARM_APP_TYPE_E eAppType = AUDIO_ARM_APP_TYPE_MAX;
    uint32 uiVol = 0;

    AUDIO_RESULT_E audio_result = AUDIO_NO_ERROR;

    audio_result = AUDIO_GetVolumeAndType((uint32 *)&uiVol, (AUDIO_ARM_APP_TYPE_E *)&eAppType);
    if(AUDIO_NO_ERROR != audio_result)
    {
        //SCI_TRACE_LOW:"armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2085_112_2_18_1_2_14_94,(uint8*)"d", audio_result);
        return;
    }

    mode_name = _ARMVB_ArmvbNv_GetName(eMode);
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_LineinAd_SetDevMode mode:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2092_112_2_18_1_2_14_95,(uint8*)"d", eMode);
        return;
    }

    result=AUDIONVARM_GetAudioModeDevCtrInfo (mode_name, AUDIO_DEVICE_MODE_PARAM_RECORD_LINEIN, (AUDMOD_DEVICECTL_T *)(&aud_mod_param_record_t));    
    if(AUDIO_NV_ARM_NO_ERROR!=result)
    {
        //SCI_TRACE_LOW:"armvb_as.c ARMVB_LineinAd_SetDevMode failed to get dev ctr info, result:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2099_112_2_18_1_2_14_96,(uint8*)"d", result);
        return;
    }  

    if(!s_b_armvb_linein_playing)
    {
        //        AUDDEV_RegPACtlFunc (ARMVB_RECORD_EnableAmplifier);

        // Open or close the earphone according to current mode
        if (VB_DA_PRI_OUTPUT == aud_mod_param_record_t.ucDAChannel)
        {
            ARMVB_DrvOutSwitch(0,AUDIO_EAR, SCI_TRUE);
        }
        else if (VB_DA_AUX_OUTPUT == aud_mod_param_record_t.ucDAChannel)
        {
            ARMVB_DrvOutSwitch(0,AUDIO_EAR, SCI_FALSE);
        }
        else
        {
            //SCI_TRACE_LOW:"ARMVB_LineinAd_SetDevMode : unknown ucDAChanne!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2132_112_2_18_1_2_14_98,(uint8*)"");
        }

        // Open or close the headphone according to current mode
        if (VB_AUD_STEREO == aud_mod_param_record_t.ucStereoEn)
        {
            ARMVB_DrvOutSwitch(0,AUDIO_HP, SCI_TRUE);
        }
        else if (VB_AUD_NON_STEREO == aud_mod_param_record_t.ucStereoEn)
        {
            ARMVB_DrvOutSwitch(0,AUDIO_HP, SCI_FALSE);
        }
        else
        {
            //SCI_TRACE_LOW:"ARMVB_LineinAd_SetDevMode : unknown ucStereoEn!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2146_112_2_18_1_2_14_99,(uint8*)"");
        }     
        return;
    }
}
/*****************************************************************************/
//  Description:    Open linein device.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_LineinAd_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    if (!s_b_linein_ad_playing)
    {
        uint32 aud_dev_open_return = AUDIO_HAL_SUCCESS;
        _ARMVB_Record_Fadein_Prepare(uiSampleRate, RECORD_FADEIN_TIME, 0, 0);
#ifdef FM_VBC
        ARMVB_DataFmt(0,AUDIO_DATA_FMT_LL_RR,AUDIO_CAPTRUE);
        aud_dev_open_return = AUDIO_HAL_Open(AUDIO_FUN_CAPTRUE |AUDIO_FUN_PLAYBACK | AUDIO_FUN_LINEIN_DIGITAL,uiSampleRate);                
#else
        aud_dev_open_return = AUDIO_HAL_Open(AUDIO_FUN_ADC0 |AUDIO_FUN_PLAYBACK | AUDIO_FUN_LINEIN,uiSampleRate);    
#endif
        if(AUDIO_HAL_SUCCESS != aud_dev_open_return)
        {
            //SCI_TRACE_LOW:"ARMVB_LineinAd_Play AUDDEV_Open failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2163_112_2_18_1_2_14_100,(uint8*)"");
            return AUDIO_ERROR;
        }
        SCI_MEMSET(&s_vb_arm_record_buf,0,sizeof(VB_ARM_RECORD_BUF));
        s_vb_arm_record_buf.data_ptr = (uint16 *)AUDIO_ALLOC(TMP_FIFO_SIZE<<4);    //2560 bytes
#ifdef FM_VBC
        s_vb_arm_record_buf.data_ptr_r = (uint16 *)AUDIO_ALLOC(TMP_FIFO_SIZE<<4);    //2560 bytes
        if (PNULL == s_vb_arm_record_buf.data_ptr || PNULL == s_vb_arm_record_buf.data_ptr_r)
#else
        if (PNULL == s_vb_arm_record_buf.data_ptr)
#endif
        {
            //SCI_TRACE_LOW:"ARMVB_Record_Play alloc fail, no memory."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ARMVB_AS_2170_112_2_18_1_2_14_101,(uint8*)"");
            if(s_vb_arm_record_buf.data_ptr)
            {
                SCI_FREE(s_vb_arm_record_buf.data_ptr);
            }
            if(s_vb_arm_record_buf.data_ptr_r)
            {
                SCI_FREE(s_vb_arm_record_buf.data_ptr_r);
            }
            return AUDIO_ERROR;
        }
        s_vb_arm_record_buf.in = 0;
        s_vb_arm_record_buf.out = 0;
        s_vb_arm_record_buf.size = TMP_FIFO_SIZE<<3;

#ifdef FM_VBC
        ARMVB_Record_SetAdcBufInfo(s_vb_arm_record_buf.data_ptr, s_vb_arm_record_buf.data_ptr_r, s_vb_arm_record_buf.size<<1);
#else
        ARMVB_Record_SetAdcBufInfo(s_vb_arm_record_buf.data_ptr,NULL,s_vb_arm_record_buf.size<<1);
#endif
        //        AUDIO_HAL_Ioctl(AUDIO_CTL_SET_BUFFER_DIV, 10);
        ARMVB_LineinAd_SetDevMode (ptDevSetting->eDev_mode);
        ARMVB_LineinAd_SetVolume (ptDevSetting->uiVol, ptDevSetting->eAppType);
        ARMVB_LineinAd_SetMute(ptDevSetting->bDownlink_mute);
        s_ad_read_size = 0;
        s_record_callback_count = 0;
        s_b_linein_ad_playing = SCI_TRUE;
    }

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Close linein device.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_LineinAd_Stop (DEV_SETTING_T *ptDevSetting)
{
    SCI_TRACE_LOW("[ljf] line[%d] _ARMVB_SetVolGain ", __LINE__);

    return 0;
}
#endif
/*****************************************************************************/
//  Description:    Register record from linein logic device controlled by arm   
//                  to record from linein to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_LineinAd_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

#ifndef BUILDIN_FM
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_LINEIN_AD"), strlen ("ARMVB_LINEIN_AD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_LineinAd_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_LineinAd_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_LineinAd_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_LineinAd_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_LineinAd_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_LineinAd_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = ARMVB_LineinAd_SetMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = ARMVB_Record_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = ARMVB_Record_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = ARMVB_Record_ReadData;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetLineinAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    AUDIO_HAL_Register_Capture((AUDIO_HAL_CALLBACK_FUN)ARMVB_CAPTURE_Callback);
#endif
    return hDevice;
}

PUBLIC HAUDIODEV ARMVB_FMAd_RegExternalAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
#ifndef BUILDIN_FM
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_FM_AD"), strlen ("ARMVB_FM_AD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_LineinAd_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_LineinAd_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_LineinAd_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_LineinAd_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_LineinAd_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_LineinAd_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = ARMVB_LineinAd_SetMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = ARMVB_Record_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = ARMVB_Record_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = ARMVB_Record_ReadData;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetLineinAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    AUDIO_HAL_Register_Capture((AUDIO_HAL_CALLBACK_FUN)ARMVB_CAPTURE_Callback);
#endif
    return hDevice;
}

LOCAL int32 ARMVB_DataFmt(uint32 id,uint32 fmt, uint32 dir)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_DAT_FMT_T aud_data_fmt;
    aud_data_fmt.id = id;
    aud_data_fmt.chan = AUDIO_ALL_CHAN;
    aud_data_fmt.fmt = fmt;
    aud_data_fmt.dir = dir;

    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_SET_DATA_FMT, (uint32)&aud_data_fmt);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        SCI_TRACE_LOW("armvb_as.c, ARMVB_DataFmt, ioctl fail, format: %d, dir: %d ",aud_data_fmt.fmt,aud_data_fmt.dir);
        return AUDIO_HAL_ERROR;
    }
    return AUDIO_HAL_SUCCESS;
}

#ifdef BUILDIN_FM
/*****************************************************************************/
//  Description:    ARMVB logic device set FM output volume.
//  Author:         KW
//  Note:  
//****************************************************************************/
LOCAL void ARMVB_FMAd_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
    return;
}

/*****************************************************************************/
//  Description:    Set Arm linein ad mute or Cancel the mute_set.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
LOCAL void ARMVB_FMAd_SetMute (
    BOOLEAN is_mute
)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
    return;
}

/*****************************************************************************/
//  Description:    ARMVB RECORD logic device from linein set device mode.
//  Author:
//  Note:
//****************************************************************************/
LOCAL void ARMVB_FMAd_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
    return;
}
/*****************************************************************************/
//  Description:    Open linein device.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_FMAd_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] uiSampleRate:0x%x", __LINE__, __FILE__, __FUNCTION__, uiSampleRate);

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Close linein device.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_FMAd_Stop (DEV_SETTING_T *ptDevSetting)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
    return AUDIO_NO_ERROR;
}
#endif
/*****************************************************************************/
//  Description:    Register record from linein logic device controlled by arm   
//                  to record from linein to audio service.
//  Author:         shujing.dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_FMAd_RegBuildInAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;
#ifdef BUILDIN_FM
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_FM_AD"), strlen ("ARMVB_FM_AD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_FMAd_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_FMAd_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_FMAd_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_FMAd_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_FMAd_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_FMAd_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = ARMVB_FMAd_SetMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = ARMVB_Record_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = ARMVB_Record_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = ARMVB_Record_ReadData;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetFMAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    //AUDIO_HAL_Register_Capture((AUDIO_HAL_CALLBACK_FUN)ARMVB_CAPTURE_Callback);
#endif

    return hDevice;
}

PUBLIC HAUDIODEV ARMVB_FM_RegAudioDevice (void)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
    return ARMVB_FM_RegBuildInAudioDevice();
}

PUBLIC HAUDIODEV ARMVB_FMAd_RegAudioDevice (void)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] ", __LINE__, __FILE__, __FUNCTION__);
#ifndef FM_SUPPORT_NONE
    if (FM_GetFmType() == FM_TYPE_EXTERNAL)
        return ARMVB_FMAd_RegExternalAudioDevice();
    else
        return ARMVB_FMAd_RegBuildInAudioDevice();
#else
    return INVALID_HANDLE;
#endif
}

typedef enum
{
    AUDDEV_POC_MODE_RECORD = (1 << 8),
    AUDDEV_POC_MODE_PLAY = (1 << 12),
    AUDDEV_POC_MODE_PLAY_RECORD = ((1 << 8) | (1 << 12)),
} AUDDEVPOCMODE_T;
AUDDEVPOCMODE_T g_armvb_dualchn_status = 0;

LOCAL void ARMVB_DualChn_Record_ReadData_Internal(uint16 *puiDestData, uint32 uiCount, uint32 *puiDataLen)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    uint32 write_point = zsp_sharemem->audOutPara.writeOffset;
    uint32 byte_count_1 = 0, byte_count_2=0;
    SCI_ASSERT (PNULL != puiDestData);  /*assert verified*/
    //SCI_ASSERT (PNULL != puiDataLen);  /*assert verified*/

    if((0==uiCount) || (0 == s_ad_read_size))
    {
        SCI_TRACE_LOW("[audio dev]  fuc[%s] need none. uiCount=[%d] ", __FUNCTION__,uiCount);

        *puiDataLen = 0;
        return;
    }

    SCI_TRACE_LOW("[audio dev]  fuc[%s] uiCount=[%d] s_vb_arm_record_buf.out:%d, write_point:%d, size:%d", __FUNCTION__,uiCount, (s_vb_arm_record_buf.out<<1), write_point, (s_vb_arm_record_buf.size<<1));
    if(write_point> (s_vb_arm_record_buf.out<<1))
    {
        byte_count_1 = write_point - (s_vb_arm_record_buf.out<<1);
        if(byte_count_1 >= (uiCount<<1))
            byte_count_1 = (uiCount<<1);            
        ARMVB_Record_CopyAudioData(puiDestData,
            s_vb_arm_record_buf.data_ptr,
            s_vb_arm_record_buf.data_ptr_r,
            s_vb_arm_record_buf.out,
            byte_count_1>>1);
    }
    else
    {
        byte_count_1 = (s_vb_arm_record_buf.size<<1)-(s_vb_arm_record_buf.out<<1) ;
        byte_count_2 = write_point;
        if(byte_count_1 >= (uiCount<<1))
        {
            byte_count_1 = (uiCount<<1);    
            byte_count_2 = 0;
        }
        else
        {
            if( byte_count_2 > ((uiCount<<1)-byte_count_1) )
            {
                byte_count_2 = (uiCount<<1)-byte_count_1;
            }
        }
        ARMVB_Record_CopyAudioData(puiDestData,
            s_vb_arm_record_buf.data_ptr,
            s_vb_arm_record_buf.data_ptr_r,
            s_vb_arm_record_buf.out,
            byte_count_1>>1);

        if( byte_count_2 )
        {
            ARMVB_Record_CopyAudioData((puiDestData+(byte_count_1>>1)),
                s_vb_arm_record_buf.data_ptr,
                s_vb_arm_record_buf.data_ptr_r,
                0,
                byte_count_2>>1);
        }
    }
    *puiDataLen = byte_count_1+byte_count_2;
    SCI_TRACE_LOW("[audio dev]  fuc[%s] addr[0x%x] uiCount[%d]puiDataLen[%d] ", __FUNCTION__,puiDestData,uiCount,*puiDataLen);
    return;
}

LOCAL void ARMVB_DualChn_Record_MoveDataPointer (uint32 uiCount, uint32 write_point, uint32 read_point)
{
    uint32 byte_count_1 = 0 ,byte_count_2=0;

    if((0==uiCount) || (0 == s_ad_read_size))
    {
        return;
    }

    if(write_point> (s_vb_arm_record_buf.out<<1))
    {
        byte_count_1 = write_point - (s_vb_arm_record_buf.out<<1);
        if(byte_count_1 >= (uiCount<<1))
            byte_count_1 = (uiCount<<1);
        s_vb_arm_record_buf.out =(s_vb_arm_record_buf.out+  (byte_count_1>>1))%( s_vb_arm_record_buf.size);
    }
    else
    {
        byte_count_1 = (s_vb_arm_record_buf.size<<1)-(s_vb_arm_record_buf.out<<1) ;
        byte_count_2 = write_point;
        if(byte_count_1 >= (uiCount<<1))
        {
            byte_count_1 = (uiCount<<1);    
            byte_count_2 = 0;
            s_vb_arm_record_buf.out =(s_vb_arm_record_buf.out+  (byte_count_1>>1))% (s_vb_arm_record_buf.size);

        }
        else
        {
            if( byte_count_2 > ((uiCount<<1)-byte_count_1) )
            {
                byte_count_2 = (uiCount<<1)-byte_count_1;
            }
            s_vb_arm_record_buf.out =(s_vb_arm_record_buf.out +  (byte_count_1>>1)
                +  (byte_count_2>>1))%( s_vb_arm_record_buf.size);
        }
    }

    return;
}

void ARMVB_DualChn_CAPTURE_Callback (void)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    uint32 write_point = zsp_sharemem->audOutPara.writeOffset;
    uint32 read_point = zsp_sharemem->audOutPara.readOffset;
    uint16 inLenth = AUDIO_OUTPUT_BUF_SIZE;
    uint32 i=0;
    uint32 j=0;

    SCI_TRACE_LOW("[audio dev]  fuc[%s]  line[%d], write_point:%d read_point:%d", __FUNCTION__,__LINE__, write_point, read_point);
    if (s_record_callback_count == 0)
    {
        //ARMVB_Record_GetZspWritePoint(&write_point);
        //ARMVB_Record_SetZspReadPoint(write_point);
        zsp_sharemem->audOutPara.readOffset = (read_point + VB_CAPTURE_TRANS_SIZE*2) % inLenth;
        //s_vb_arm_record_buf.out = write_point/2;
        return;
    }

    s_ad_read_size=VB_CAPTURE_TRANS_SIZE;
    //the length of data to arm source is 160 samples one time but from zsp is according for the samplerate
    for(j=0; j<record_callback_FrameCount; j++)
    {
        for(i=0; i<s_record_callback_count; i++)
        {
            if(PNULL!=s_armrecord_callback[i])
            {
                s_armrecord_callback[i](s_ad_read_size);
            }
        }
        SCI_TRACE_LOW("[audio dev]  fuc[%s]  %d", __FUNCTION__,__LINE__);

        ARMVB_DualChn_Record_MoveDataPointer(s_ad_read_size, write_point, read_point);
        zsp_sharemem->audOutPara.readOffset = (read_point + s_ad_read_size*2) % inLenth;
    }
    return;
}

void ARMVB_DualChn_Callback (void)
{
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = NULL;
    uint32 data_count;
    uint32 copy_size = 0;
    int channelNb = 0;
    uint32 tick_cnt;
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[audio dev] func[%s] line[%d] 1time:[%d][%d]in", __FUNCTION__, __LINE__,tick_cnt/1000, tick_cnt%1000);

    zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    
    if (g_channelNb == HAL_AIF_MONO)
    {
        channelNb = 0;
    }
    else
    {
        channelNb = 1;
    }

    if ((g_armvb_dualchn_status & AUDDEV_POC_MODE_PLAY) == AUDDEV_POC_MODE_PLAY)
    {
        data_count = RingBufferGetWriteBufferLen(&zsp_sharemem->audInPara, zsp_sharemem->audInPara.inLenth);
        SCI_TRACE_LOW("[audio dev] func[%s] line[%d] data_count = %d", __FUNCTION__, __LINE__,data_count);
    
        copy_size = _Track_Mixer(g_armvb_dualchn_device_ptr->ptTrackList,
                                                s_vb_da0_buffer_multitrack, SCI_NULL/*s_vb_da1_buffer_multitrack*/,
                                                SCI_NULL, SCI_NULL,  data_count/2,   1,  channelNb,  s_is_vb_mute);
        SCI_TRACE_LOW("[audio dev] func[%s] line[%d] copy_size = %d", __FUNCTION__, __LINE__,copy_size);
        RingBufferWriteBuffer(&zsp_sharemem->audInPara, 
                                            zsp_sharemem->audInput, 
                                            s_vb_da0_buffer_multitrack, data_count, (copy_size * 2)); // set data_count data for input 0 to enough
    }
    if ((g_armvb_dualchn_status & AUDDEV_POC_MODE_RECORD) == AUDDEV_POC_MODE_RECORD)
    {
        // record data process
        ARMVB_DualChn_CAPTURE_Callback();
    }
    SCI_TRACE_LOW("[audio dev] func[%s] line[%d] samplerate = %d, traceDataFlag:0x%x, inLenth:%d", __FUNCTION__, __LINE__, zsp_sharemem->audOutPara.samplerate, zsp_sharemem->traceDataFlag, zsp_sharemem->audInPara.inLenth);
#if 0
    //s_armvb_output_buf.in += copy_size*2;
    if (copy_size < (data_count/2))
    {
        if(_Track_Mixer_End(g_armvb_device_ptr->ptTrackList))
        {
            zsp_sharemem->audInPara.fileEndFlag = 1;
        }
    }
    tick_cnt = SCI_GetTickCount();
#endif
    return;
}
/*****************************************************************************/
//  Description:    ARMVB logic device set device mode.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void ARMVB_DualChn_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
	AUD_DEVICE_CFG_EXT_T device_cfg_ext;

    SCI_TRACE_LOW("[ljf] line[%d] ARMVB_DualChn_SetDevMode current mode:0x%x, new eMode:0x%x", __LINE__, g_cur_itf,eMode);
    /*xin.xu1 20190409 for bugid 1032174*/
    SwitchDeviceStartTimer();
    if (AUDIO_DEVICE_MODE_BLUEPHONE == g_cur_itf)
    {
        SCI_TRACE_LOW("[ljf] ARMVB_DualChn_SetDevMode is bluephone, call A2DP_audioClose");
        //A2DP_audioClose();
    }


    ret = AUDIO_CP_STOP(AUDIO_DEV_DUALCHN_PLAYRECORD, g_cur_itf, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Stop call AUDIO_CP_STOP failed ret:0x%x", __LINE__, ret);
        return ;
    }
	    zsp_sharemem->traceDataFlag &= (!AUDDEV_POC_MODE_PLAY);
    g_armvb_dualchn_status &= (!AUDDEV_POC_MODE_PLAY);
	
    
    
    switch(eMode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;

        default:
            break;
    }
    if (AUDIO_DEVICE_MODE_BLUEPHONE ==eMode)
    {
        SCI_TRACE_LOW("[ljf] eDev_mode is bluephone, call A2DP_audioStart");
        //A2DP_audioStart(g_uiCurrentSamplerate, A2DP_GetAudioData, A2DP_GetAudioDataCount);
    }
  g_zsp_para.musicMode = 10;
    g_zsp_para.sbcOutFlag = 0;
#if USE_FOR_WALKIE_TALKIE
    zsp_sharemem->traceDataFlag |= AUDDEV_POC_MODE_PLAY;
#else
    zsp_sharemem->traceDataFlag =  AUDDEV_POC_MODE_PLAY_RECORD;
#endif

    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType= AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath= AUD_INPUT_PATH1;
    device_cfg_ext.linePath= AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
	ret =AUDIO_CP_START(AUDIO_DEV_DUALCHN_PLAYRECORD,g_cur_itf, &g_aif_stream, &g_aud_level_cfg,0,device_cfg_ext,0, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_SetDevMode call AUDIO_CP_START failed ret:0x%x", __LINE__, ret);
        return ;
    }
    AUDIO_CP_RecordStart(g_cur_itf, &g_dual_record_aif_stream, &g_aud_level_cfg);
    g_armvb_dualchn_status |= AUDDEV_POC_MODE_PLAY;
    if (eMode == AUDIO_DEVICE_MODE_EARFREE)
    {
        g_cur_itf = SND_ITF_LOUD_SPEAKER;
        ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
        if (0 != ret)
        {
            SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Play call AUDIO_CP_Setup failed ret:0x%x", __LINE__, ret);
        }
    }
    ARMVB_SetToneMode(0); //reset tonemode value

    SCI_TRACE_LOW("line[%d] ARMVB_SetDevMode done eMode:0x%x", __LINE__, eMode);
    return;
}

LOCAL void ARMVB_DualChn_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    SCI_TRACE_LOW("[audio dev] fuc[%s] line[%d] uiVol[%d] eAppType[%d]", __FUNCTION__,__LINE__, uiVol, eAppType);

    g_aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)uiVol;

    cur_itf = (AUD_ITF_T)g_cur_itf;
    g_audio_cfg.spkLevel = g_aud_level_cfg.spkLevel;
    g_audio_cfg.micLevel = AUD_MIC_ENABLE;
    g_audio_cfg.sideLevel = AUD_SIDE_MUTE;
    g_audio_cfg.toneLevel = AUD_TONE_0dB;
    g_audio_cfg.encMute = 0;
    g_audio_cfg.decMute = 0;
    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType = AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath = AUD_INPUT_PATH1;
    device_cfg_ext.linePath = AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
    SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_SetVolume g_audio_cfg.spkLevel:0x%x", __LINE__, g_audio_cfg.spkLevel);

    vois_ret = AUDIO_CP_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_SetVolume AUDIO_CP_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }

    return;
}

/*****************************************************************************/
//  Description:    Open voiceband.
//  Author:         Jinfeng.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_DualChn_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    uint32 tick_cnt;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    SND_ITF_T cur_itf = SND_ITF_LOUD_SPEAKER;

    if (g_armvb_dualchn_status == AUDDEV_POC_MODE_RECORD)
    {
        g_armvb_dualchn_status |= AUDDEV_POC_MODE_PLAY;
        zsp_sharemem->traceDataFlag |= AUDDEV_POC_MODE_PLAY;
        return AUDIO_NO_ERROR;
    }
    else if (g_armvb_dualchn_status == AUDDEV_POC_MODE_PLAY_RECORD)
    {
        return AUDIO_NO_ERROR;
    }

    SCI_TRACE_LOW("{xx} ARMVB_DualChn_Play SCI_GetSemaphore....");
    if(SCI_SUCCESS != SCI_GetSemaphore(audio_armvb_semaphore, 10))
    {
        SCI_TRACE_LOW("{xx} ARMVB_DualChn_Play SCI_GetSemaphore return");
        return AUDIO_CONFLICT;
    }
    SCI_TRACE_LOW("{xx} ARMVB_DualChn_Play SCI_GetSemaphore OK");
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[audio dev] line[%d] func[%s] ptDevSetting->eDev_mode:0x%x g_audio_dev_used:[%d] time[%d][%d]", __LINE__, __FUNCTION__, ptDevSetting->eDev_mode, g_audio_dev_used, tick_cnt/1000, tick_cnt%1000);
    SCI_TRACE_LOW("[audio dev] fuc[%s] line[%d] uiSampleRate[%d] vol[%d] zsp_sharemem:0x%x", __FUNCTION__,__LINE__,uiSampleRate,ptDevSetting->uiVol, zsp_sharemem);
    if (1 == g_audio_dev_used)
    {
        SCI_TRACE_LOW("[audio dev] Because g_audio_dev_used is [%d], so ARMVB_DualChn_Play return", g_audio_dev_used);
        return AUDIO_ERROR;
    }
    SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);

    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;

        default:
            SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_aif_stream.startAddress = (uint32 *)zsp_sharemem->rxPCM.rxPcmBuffer.pcmBuf;
    if(ptDevSetting->usKeyTone)
    {
        g_aif_stream.length = 320*2;
    }
    else
    {
        g_aif_stream.length = 320*2;
    }
    g_uiCurrentSamplerate = uiSampleRate;
    zsp_sharemem->audInPara.sampleRate = uiSampleRate;
    zsp_sharemem->audOutPara.samplerate = uiSampleRate;
    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL)
    {
        zsp_sharemem->audInPara.channelNb = HAL_AIF_STEREO;
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_STEREO;
    }
    else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) || (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT))
    {
        zsp_sharemem->audInPara.channelNb = HAL_AIF_MONO;
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_MONO;
    }
    else
    {
        SCI_TRACE_LOW("line[%d] ARMVB_DualChn_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
    zsp_sharemem->audInPara.channelNb = HAL_AIF_MONO;
    zsp_sharemem->audOutPara.channelNb = HAL_AIF_MONO;
    zsp_sharemem->audInPara.bitsPerSample = 16;

    g_zsp_para.musicMode = 10;
    g_zsp_para.sbcOutFlag = 0;
#if USE_FOR_WALKIE_TALKIE
    zsp_sharemem->traceDataFlag  |= AUDDEV_POC_MODE_PLAY;
#else
    zsp_sharemem->traceDataFlag =  AUDDEV_POC_MODE_PLAY_RECORD;
#endif
    if(ptDevSetting->usKeyTone)
    {
        zsp_sharemem->audOutPara.length = 160; //256; // TODO: LJF
    }
    else
    {
        zsp_sharemem->audOutPara.length = 160;
    }

    //!!!can not memset all of zsp sharemem to zero. some status value will be keep with zsp!!! sheen
    //  do not init this para will result in assert but  it should not initialize here for zsp used !zgt
    zsp_sharemem->updateParaInd=0;//sheen
    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL)
    {
        g_aif_stream.channelNb = HAL_AIF_STEREO;
    }
    else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) || (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT))
    {
        g_aif_stream.channelNb = HAL_AIF_MONO;
    }
    else
    {
        SCI_TRACE_LOW("line[%d] ARMVB_DualChn_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
    g_aif_stream.channelNb = HAL_AIF_MONO;
    g_channelNb = g_aif_stream.channelNb;
    g_aif_stream.playSyncWithRecord = 1;
    g_aif_stream.sampleRate = uiSampleRate;
    g_aif_stream.voiceQuality = TRUE;
    g_aif_stream.halfHandler = NULL;
    g_aif_stream.endHandler = NULL;

    //for bug973551 973739 to fix noise in seek and stop.zgt 2018.12.10
    SCI_TRACE_LOW("line[%d] ARMVB_DualChn_Play ptDevSetting->bDownlink_mute:%d", __LINE__, ptDevSetting->bDownlink_mute);
    if(0==ptDevSetting->bDownlink_mute)
    {
        g_aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)ptDevSetting->uiVol;
    }
    else
    {
        g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
    }
    s_is_vb_mute = ptDevSetting->bDownlink_mute;

    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_VOICE;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    zsp_sharemem->audInPara.fileEndFlag = 0;
    zsp_sharemem->audInPara.readOffset = 0;
    SCI_MEMSET(zsp_sharemem->Output.audOutput, 0, AUDIO_OUTPUT_BUF_SIZE);
    SCI_MEMSET(zsp_sharemem->audInput, 0, (zsp_sharemem->audInPara.inLenth -4));
    if(ptDevSetting->usKeyTone)
    {
        zsp_sharemem->audInPara.writeOffset = 512; // Init buffer
    }
    else
    {
        zsp_sharemem->audInPara.writeOffset = 4608;
    }

#if USE_FOR_WALKIE_TALKIE
    zsp_sharemem->audOutPara.writeOffset = 0;
    zsp_sharemem->audOutPara.readOffset = 0;
#endif
    SCI_TRACE_LOW("ARMVB_DualChn_Play audInPara.readOffset:%d,audInPara.writeOffset:%d,audOutPara.writeOffset:%d,audOutPara.readOffset:%d", 
        zsp_sharemem->audInPara.readOffset, zsp_sharemem->audInPara.writeOffset,zsp_sharemem->audOutPara.writeOffset,zsp_sharemem->audOutPara.readOffset);

    ipc_register_music_notify(ARMVB_DualChn_Callback);

    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        SCI_TRACE_LOW("[audio dev] eDev_mode is bluephone");
    }

    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType= AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath= AUD_INPUT_PATH1;
    device_cfg_ext.linePath= AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
    ret =AUDIO_CP_START(AUDIO_DEV_DUALCHN_PLAYRECORD,g_cur_itf, &g_aif_stream, &g_aud_level_cfg,0,device_cfg_ext,0, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Play call AUDIO_CP_START failed ret:0x%x", __LINE__, ret);
        return ret;
    }

    g_dual_record_aif_stream.channelNb = HAL_AIF_MONO;
    g_dual_record_aif_stream.playSyncWithRecord = 1;
    g_dual_record_aif_stream.sampleRate = uiSampleRate;
    g_dual_record_aif_stream.voiceQuality = TRUE;
    g_dual_record_aif_stream.halfHandler = NULL;
    g_dual_record_aif_stream.endHandler = NULL;
    g_dual_record_aif_stream.startAddress = (uint32 *)zsp_sharemem->txPCM.txPcmBuffer.pcmBuf;
    if(ptDevSetting->usKeyTone)
    {
        g_dual_record_aif_stream.length = 320*2;
    }
    else
    {
        g_dual_record_aif_stream.length = 320*2;
    }
    AUDIO_CP_RecordStart(g_cur_itf, &g_dual_record_aif_stream, &g_aud_level_cfg);
    g_armvb_dualchn_status |= AUDDEV_POC_MODE_PLAY;
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_EARFREE)
    {
        g_cur_itf = SND_ITF_LOUD_SPEAKER;
        ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
        if (0 != ret)
        {
            SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Play call AUDIO_CP_Setup failed ret:0x%x", __LINE__, ret);
        }
    }
    ARMVB_SetToneMode(0); //reset tonemode value

    return 0;
}

/*****************************************************************************/
//  Description:    Close voiceband
//  Author:         Jinfeng.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_DualChn_Stop (DEV_SETTING_T *ptDevSetting)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    INT8 delay_cnt = 10;
    uint32 tick_cnt;
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[audio dev] line[%d] func[%s] eDev_mode:0x%x g_audio_dev_used:[%d], time[%d][%d]", __LINE__, __FUNCTION__, ptDevSetting->eDev_mode, g_audio_dev_used, tick_cnt/1000, tick_cnt%1000);
    if (g_armvb_dualchn_status == AUDDEV_POC_MODE_RECORD)
    {
        return AUDIO_NO_ERROR;
    }
    else if (g_armvb_dualchn_status == AUDDEV_POC_MODE_PLAY_RECORD)
    {
        zsp_sharemem->traceDataFlag &= (~AUDDEV_POC_MODE_PLAY);
        g_armvb_dualchn_status &= (~AUDDEV_POC_MODE_PLAY);
        return AUDIO_NO_ERROR;
    }

    if (1 == g_audio_dev_used)
    {
        SCI_TRACE_LOW("[audio dev] Because g_audio_dev_used is [%d], so ARMVB_DualChn_Stop return", g_audio_dev_used);
        return AUDIO_ERROR;
    }

    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER; // close lasted device.
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;

        default:
            SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Stop eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_uiCurrentSamplerate = 0;
    zsp_sharemem->audInPara.fileEndFlag = 1;
    g_zsp_para.musicMode = 0;
    g_zsp_para.sbcOutFlag = 0;
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        SCI_TRACE_LOW("[audio dev] eDev_mode is bluephone");
    }

    ret = AUDIO_CP_STOP(AUDIO_DEV_DUALCHN_PLAYRECORD, g_cur_itf, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Stop call AUDIO_CP_STOP failed ret:0x%x", __LINE__, ret);
        return ret;
    }

    zsp_sharemem->traceDataFlag &= (~AUDDEV_POC_MODE_PLAY);
    g_armvb_dualchn_status &= (~AUDDEV_POC_MODE_PLAY);
    SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    SCI_TRACE_LOW("{xx} ARMVB_DualChn_Stop SCI_PutSemaphore....");
    SCI_PutSemaphore(audio_armvb_semaphore);
    SCI_TRACE_LOW("{xx} ARMVB_DualChn_Stop SCI_PutSemaphore OK");

    return 0;
}



PUBLIC HAUDIODEV ARMVB_DualChn_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("[audio dev] func[%s] line[%d]", __FUNCTION__, __LINE__);
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_DualChn"), strlen ("ARMVB_DualChn") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_TRUE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_DualChn_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_DualChn_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_DualChn_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_DualChn_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_DualChn_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_DualChn_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = ARMVB_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute= SCI_NULL; //ARMVB_SetDepopMute;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=ARMVB_GetRemainedPcmCount;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=ARMVB_GetRemainedPcmTime;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_GetAppropriateOutputSamplerate;
    tDeviceInfo.tAudioDevOpe.pSetNotchton=ARMVB_SetNotchToneEnable;
#ifdef BT_DIALER_SET_VOLGAIN
    tDeviceInfo.tAudioDevOpe.pSetVolGain = _ARMVB_SetVolGain;
#endif    
    SCI_TRACE_LOW("[audio dev] func[%s] line[%d]", __FUNCTION__, __LINE__);
    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    g_armvb_dualchn_device_ptr = AUDIO_DM_GetDeviceHandleStruct (hDevice);  // Record device structure
    SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_RegAudioDevice g_armvb_dualchn_device_ptr:0x%x ", __LINE__, g_armvb_dualchn_device_ptr);

    return hDevice;
}

/*****************************************************************************/
//  Description:    Open device to dualchn record.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_DualChn_Record_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    uint32 tick_cnt;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    SND_ITF_T cur_itf = SND_ITF_LOUD_SPEAKER;
	
    SCI_TRACE_LOW("[audio dev] fuc[%s] line[%d] uiSampleRate[%d]  g_armvb_dualchn_status[0x%x]", __FUNCTION__,__LINE__,uiSampleRate, g_armvb_dualchn_status);
    if (g_armvb_dualchn_status == AUDDEV_POC_MODE_PLAY)
    {
        g_armvb_dualchn_status |= AUDDEV_POC_MODE_RECORD;
        zsp_sharemem->traceDataFlag |= AUDDEV_POC_MODE_RECORD;
        SCI_MEMSET(&s_vb_arm_record_buf,0,sizeof(VB_ARM_RECORD_BUF));
        s_vb_arm_record_buf.data_ptr =(uint16 *)(zsp_sharemem->Output.audOutput);
        s_vb_arm_record_buf.in = 0;
        s_vb_arm_record_buf.out = 0;
        s_vb_arm_record_buf.size = AUDIO_OUTPUT_BUF_SIZE>>1;//9k byte
        record_callback_FrameCount =1;
        return AUDIO_NO_ERROR;
    }
    else if (g_armvb_dualchn_status == AUDDEV_POC_MODE_PLAY_RECORD)
    {
        s_vb_arm_record_buf.data_ptr =(uint16 *)(zsp_sharemem->Output.audOutput);
        return AUDIO_NO_ERROR;
    }

    if (s_b_armvb_recording)
    {
        return AUDIO_CONFLICT;
    }
    SCI_TRACE_LOW("{xx} ARMVB_DualChn_Record_Play SCI_GetSemaphore....");
    if(SCI_SUCCESS != SCI_GetSemaphore(audio_armvb_semaphore, 10))
    {
        SCI_TRACE_LOW("{xx} ARMVB_DualChn_Record_Play SCI_GetSemaphore return");
        return AUDIO_CONFLICT;
    }
    SCI_TRACE_LOW("{xx} ARMVB_DualChn_Record_Play SCI_GetSemaphore OK");
    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[audio dev] line[%d] func[%s] ptDevSetting->eDev_mode:0x%x g_audio_dev_used:[%d] time[%d][%d]", __LINE__, __FUNCTION__, ptDevSetting->eDev_mode, g_audio_dev_used, tick_cnt/1000, tick_cnt%1000);
    SCI_TRACE_LOW("[audio dev] fuc[%s] line[%d] uiSampleRate[%d] vol[%d] zsp_sharemem:0x%x", __FUNCTION__,__LINE__,uiSampleRate,ptDevSetting->uiVol, zsp_sharemem);
    if (1 == g_audio_dev_used)
    {
        SCI_TRACE_LOW("[audio dev] Because g_audio_dev_used is [%d], so ARMVB_DualChn_Play return", g_audio_dev_used);
        return AUDIO_ERROR;
    }
    SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);

    g_armvb_dualchn_status |= AUDDEV_POC_MODE_RECORD;
    zsp_sharemem->traceDataFlag |= AUDDEV_POC_MODE_RECORD;
    SCI_MEMSET(&s_vb_arm_record_buf,0,sizeof(VB_ARM_RECORD_BUF));
    s_vb_arm_record_buf.data_ptr =(uint16 *)(zsp_sharemem->Output.audOutput);
    s_vb_arm_record_buf.in = 0;
    s_vb_arm_record_buf.out = 0;
    s_vb_arm_record_buf.size = AUDIO_OUTPUT_BUF_SIZE>>1;//9k byte
    record_callback_FrameCount =1;
    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;

        default:
            SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Record_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_aif_stream.startAddress = (uint32 *)zsp_sharemem->rxPCM.rxPcmBuffer.pcmBuf;
    if(ptDevSetting->usKeyTone)
    {
        g_aif_stream.length = 320*2;
    }
    else
    {
        g_aif_stream.length = 320*2;
    }
    g_uiCurrentSamplerate = uiSampleRate;
    zsp_sharemem->audInPara.sampleRate = uiSampleRate;
    zsp_sharemem->audOutPara.samplerate = uiSampleRate;
    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL)
    {
        zsp_sharemem->audInPara.channelNb = HAL_AIF_STEREO;
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_STEREO;
    }
    else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) || (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT))
    {
        zsp_sharemem->audInPara.channelNb = HAL_AIF_MONO;
        zsp_sharemem->audOutPara.channelNb = HAL_AIF_MONO;
    }
    else
    {
        SCI_TRACE_LOW("line[%d] ARMVB_DualChn_Record_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
    zsp_sharemem->audInPara.channelNb = HAL_AIF_MONO;
    zsp_sharemem->audOutPara.channelNb = HAL_AIF_MONO;
    zsp_sharemem->audInPara.bitsPerSample = 16;

    g_zsp_para.musicMode = 10;
    g_zsp_para.sbcOutFlag = 0;
#if USE_FOR_WALKIE_TALKIE
    zsp_sharemem->traceDataFlag |= AUDDEV_POC_MODE_RECORD;
#else
    zsp_sharemem->traceDataFlag =  AUDDEV_POC_MODE_PLAY_RECORD;
#endif
    if(ptDevSetting->usKeyTone)
    {
        zsp_sharemem->audOutPara.length = 160; //256; // TODO: LJF
    }
    else
    {
        zsp_sharemem->audOutPara.length = 160;
    }

    //!!!can not memset all of zsp sharemem to zero. some status value will be keep with zsp!!! sheen
    //  do not init this para will result in assert but  it should not initialize here for zsp used !zgt
    zsp_sharemem->updateParaInd=0;//sheen
    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL)
    {
        g_aif_stream.channelNb = HAL_AIF_STEREO;
    }
    else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) || (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT))
    {
        g_aif_stream.channelNb = HAL_AIF_MONO;
    }
    else
    {
        SCI_TRACE_LOW("line[%d] ARMVB_DualChn_Record_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
    g_aif_stream.channelNb = HAL_AIF_MONO;
    g_channelNb = g_aif_stream.channelNb;
    g_aif_stream.playSyncWithRecord = 1;
    g_aif_stream.sampleRate = uiSampleRate;
    g_aif_stream.voiceQuality = TRUE;
    g_aif_stream.halfHandler = NULL;
    g_aif_stream.endHandler = NULL;

    //for bug973551 973739 to fix noise in seek and stop.zgt 2018.12.10
    SCI_TRACE_LOW("line[%d] ARMVB_DualChn_Record_Play ptDevSetting->bDownlink_mute:%d", __LINE__, ptDevSetting->bDownlink_mute);
    if(0==ptDevSetting->bDownlink_mute)
    {
        g_aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)ptDevSetting->uiVol;
    }
    else
    {
        g_aud_level_cfg.spkLevel = SND_SPK_MUTE;
    }
    s_is_vb_mute = ptDevSetting->bDownlink_mute;

    g_aud_level_cfg.micLevel = SND_MIC_ENABLE;
    g_aud_level_cfg.sideLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.toneLevel = SND_SIDE_VOL_15;
    g_aud_level_cfg.appMode = SND_APP_MODE_VOICE;
    zsp_sharemem->audInPara.inLenth = AUDIO_INPUT_BUF_SIZE; // 7K
    zsp_sharemem->audInPara.fileEndFlag = 0;
    zsp_sharemem->audInPara.readOffset = 0;
    SCI_MEMSET(zsp_sharemem->Output.audOutput, 0, AUDIO_OUTPUT_BUF_SIZE);
    SCI_MEMSET(zsp_sharemem->audInput, 0, (zsp_sharemem->audInPara.inLenth -4));
    if(ptDevSetting->usKeyTone)
    {
        zsp_sharemem->audInPara.writeOffset = 512; // Init buffer
    }
    else
    {
        zsp_sharemem->audInPara.writeOffset = 4608;
    }
#if USE_FOR_WALKIE_TALKIE
    zsp_sharemem->audOutPara.writeOffset = 0;
    zsp_sharemem->audOutPara.readOffset = 0;
#endif
    SCI_TRACE_LOW("ARMVB_DualChn_Record_Play audInPara.readOffset:%d,audInPara.writeOffset:%d,audOutPara.writeOffset:%d,audOutPara.readOffset:%d", 
        zsp_sharemem->audInPara.readOffset, zsp_sharemem->audInPara.writeOffset,zsp_sharemem->audOutPara.writeOffset,zsp_sharemem->audOutPara.readOffset);

    ipc_register_music_notify(ARMVB_DualChn_Callback);

    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        SCI_TRACE_LOW("[audio dev] eDev_mode is bluephone");
    }

    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType= AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath= AUD_INPUT_PATH1;
    device_cfg_ext.linePath= AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
    ret =AUDIO_CP_START(AUDIO_DEV_MUSIC,g_cur_itf, &g_aif_stream, &g_aud_level_cfg,0,device_cfg_ext,0, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Record_Play call AUDIO_CP_START failed ret:0x%x", __LINE__, ret);
        return ret;
    }

    g_dual_record_aif_stream.channelNb = HAL_AIF_MONO;
    g_dual_record_aif_stream.playSyncWithRecord = 1;
    g_dual_record_aif_stream.sampleRate = uiSampleRate;
    g_dual_record_aif_stream.voiceQuality = TRUE;
    g_dual_record_aif_stream.halfHandler = NULL;
    g_dual_record_aif_stream.endHandler = NULL;
    g_dual_record_aif_stream.startAddress = (uint32 *)zsp_sharemem->txPCM.txPcmBuffer.pcmBuf;
    if(ptDevSetting->usKeyTone)
    {
        g_dual_record_aif_stream.length = 320*2;
    }
    else
    {
        g_dual_record_aif_stream.length = 320*2;
    }
    AUDIO_CP_RecordStart(g_cur_itf, &g_dual_record_aif_stream, &g_aud_level_cfg);
    g_armvb_dualchn_status |= AUDDEV_POC_MODE_RECORD;
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_EARFREE)
    {
        g_cur_itf = SND_ITF_LOUD_SPEAKER;
        ret = AUDIO_CP_Setup(g_cur_itf, &g_aud_level_cfg);
        if (0 != ret)
        {
            SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Record_Play call AUDIO_CP_Setup failed ret:0x%x", __LINE__, ret);
        }
    }
    ARMVB_SetToneMode(0); //reset tonemode value

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Close device to stop dualchn record.
//  Author:         
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E ARMVB_DualChn_Record_Stop (DEV_SETTING_T *ptDevSetting)
{
    AUD_ERR_T ret = 0;
    HAL_ERR_T hal_ret = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    INT8 delay_cnt = 10;
    uint32 tick_cnt;

    tick_cnt = SCI_GetTickCount();
    SCI_TRACE_LOW("[audio dev] line[%d] func[%s] eDev_mode:0x%x g_audio_dev_used:[%d], time[%d][%d]", __LINE__, __FUNCTION__, ptDevSetting->eDev_mode, g_audio_dev_used, tick_cnt/1000, tick_cnt%1000);

    if (g_armvb_dualchn_status == AUDDEV_POC_MODE_PLAY)
    {
        return AUDIO_NO_ERROR;
    }
    else if (g_armvb_dualchn_status == AUDDEV_POC_MODE_PLAY_RECORD)
    {
        zsp_sharemem->traceDataFlag &= (~AUDDEV_POC_MODE_RECORD);
        g_armvb_dualchn_status &= (~AUDDEV_POC_MODE_RECORD);
        return AUDIO_NO_ERROR;
    }

    if (1 == g_audio_dev_used)
    {
        SCI_TRACE_LOW("[audio dev] Because g_audio_dev_used is [%d], so ARMVB_DualChn_Record_Stop return", g_audio_dev_used);
        return AUDIO_ERROR;
    }

    switch(ptDevSetting->eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            g_cur_itf = SND_ITF_RECEIVER;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            g_cur_itf = SND_ITF_EAR_PIECE;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER; // close lasted device.
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;

        default:
            SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Stop eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }
    g_uiCurrentSamplerate = 0;
    zsp_sharemem->audInPara.fileEndFlag = 1;
    g_zsp_para.musicMode = 0;
    g_zsp_para.sbcOutFlag = 0;
    if (ptDevSetting->eDev_mode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        SCI_TRACE_LOW("[audio dev] eDev_mode is bluephone");
    }

    ret = AUDIO_CP_STOP(AUDIO_DEV_MUSIC, g_cur_itf, &g_zsp_para);
    if (AUD_ERR_NO != ret)
    {
        SCI_TRACE_LOW("[audio dev] line[%d] ARMVB_DualChn_Stop call AUDIO_CP_STOP failed ret:0x%x", __LINE__, ret);
        return ret;
    }

    zsp_sharemem->traceDataFlag &= (~AUDDEV_POC_MODE_RECORD);
    g_armvb_dualchn_status &= (~AUDDEV_POC_MODE_RECORD);
    SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    SCI_TRACE_LOW("{xx} ARMVB_DualChn_Stop SCI_PutSemaphore....");
    SCI_PutSemaphore(audio_armvb_semaphore);
    SCI_TRACE_LOW("{xx} ARMVB_DualChn_Stop SCI_PutSemaphore OK");

    return AUDIO_NO_ERROR;
}


/*****************************************************************************/
//  Description:    Register device controlled by arm to audio service.
//  Author:         
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV ARMVB_DualChn_Record_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("[audio dev] func[%s] line[%d]", __FUNCTION__, __LINE__);
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("ARMVB_DualChn_RECORD"), strlen ("ARMVB_DualChn_RECORD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = ARMVB_DualChn_Record_Play;
    tDeviceInfo.tAudioDevOpe.pClose = ARMVB_DualChn_Record_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = ARMVB_DualChn_Record_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = ARMVB_DualChn_Record_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = ARMVB_DualChn_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = ARMVB_Record_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = ARMVB_Record_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = ARMVB_Record_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = ARMVB_DualChn_Record_ReadData_Internal;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = ARMVB_Record_GetAppropriateOutputSamplerate;
#ifdef AUD_ARM_RECORD_EXP
    tDeviceInfo.tAudioDevOpe.pPreClose = SCI_NULL; //ARMVB_Record_PreClose;
    tDeviceInfo.tAudioDevOpe.pPostOpen = SCI_NULL; //ARMVB_Record_PostOpen;
#endif

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of asm_hc.c





