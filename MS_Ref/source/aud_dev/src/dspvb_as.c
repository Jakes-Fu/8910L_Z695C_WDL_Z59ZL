#include "ms_ref_aud_dev_trc.h"
#include "os_api.h"
#include "audio_config.h"
#include "vb_drv.h"
#include "aud_gen.h"
#include "adm.h"
#include "dspvb_as.h"
#include "armvb_as.h"
#include "audio_config.h"
#include "audio_nv_dsp.h"

#include "sc_reg.h"

#ifdef DSP_AUDIO_III
#include "admm_III.h"
#else
#include "admm_II.h"
#endif

#include "layer1_audio.h"

#ifdef DSP_CTRL_PCM
#include "pcm_drvapi.h"
#endif

#ifdef   __cplusplus
extern   "C"
{
#endif
#ifndef __arm
#ifndef __MODULE__
#define __MODULE__ "dspvb_as.c"
#endif
#endif
#include "deepsleep_drvapi.h"
#include "pmic_26M_clock.h"
#include "rpc_aud.h"
/*lint -save -e752 */

#define RESERVED (0)
#define DSPVB_EXTRA_VOL_GAIN_MIN 0X1000

#define VB_DSP_VOL_L_MASK        0xFF    
#define VB_DSP_VOL_R_MASK        0xFF00
#define VB_DSP_VOL_SHIFT        8

#define VB_DSP_PA_LDO_V_SEL_MASK  0x000F
#define VB_DSP_PA_DTRI_F_SEL_MASK  0x0070
#define VB_DSP_PA_DTRI_F_SEL_SHIFT  4

#define VB_DSP_OPEN_FLAG        (AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP)

#define DSPVB_USE_DM_TONE_INTERFACE
#define DSPVB_USE_PCIM_26M_CLOCK
#define DSPVB_USE_CP_AUDIO_INTERFACE
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint8 s_vb_volumn = 0;
LOCAL BOOLEAN s_b_dspvb_playing = SCI_FALSE;
LOCAL BOOLEAN s_b_layer1_playing  = SCI_FALSE;
LOCAL BOOLEAN s_b_dspdata_playing = SCI_FALSE;
extern SCI_SEMAPHORE_PTR audio_armvb_semaphore;

extern AUD_SPKPA_TYPE_T g_spkpatype;

//extern uint32 s_pcm_on_count;
extern uint16 g_mode_flag;
#ifdef BT_DIALER_SUPPORT 
BOOLEAN bt_dialer_uplink_mute_flag = 0;
#endif
LOCAL void DSPVB_Layer1SetVolume(uint32  uiVol);
LOCAL void DSPVB_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType, NV_AUDIO_DSP_CODEC_E dsp_codec_id);
LOCAL void _LAYER1_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType);
LOCAL void _RECORD_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType);
LOCAL void _MP4RECORD_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType);
LOCAL void _LOOPBACK_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType);
LOCAL AUDIO_NV_DSP_RESULT_E DSPVB_Codec_SetVolume(uint32  uiVol,  NV_AUDIO_DSP_CODEC_E dsp_codec_id);

SFS_HANDLE      g_PcmDumpFileHandle = NULL;

LOCAL NV_AUDIO_DSP_CODEC_E _LAYER1_GetDspCodecId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_E;
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        break;
        default:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
            SCI_TRACE_LOW ("[_LAYER1_GetDspCodecId]: mode:%d", eDev_mode);
            break;
    }    
    return dsp_codec_id;
}

LOCAL NV_AUDIO_DSP_CODEC_E _RECORD_GetDspCodecId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_E;
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORDHEADSET_E;
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        break;
        default:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
            SCI_TRACE_LOW ("[_RECORD_GetDspCodecId]: mode:%d", eDev_mode);
            break;
    }    
    return dsp_codec_id;
}

LOCAL NV_AUDIO_DSP_CODEC_E _MP4RECORD_GetDspCodecId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_E;
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTRHEADSET_E;
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        break;
        default:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
            SCI_TRACE_LOW ("[_MP4RECORD_GetDspCodecId]: mode:%d", eDev_mode);
            break;
    }    
    return dsp_codec_id;
}

LOCAL NV_AUDIO_DSP_CODEC_E _LOOPBACK_GetDspCodecId(AUDIO_DEVICE_MODE_TYPE_E  eDev_mode)
{
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    switch(eDev_mode)
    {
        case AUDIO_DEVICE_MODE_HANDHOLD:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        break;
        case AUDIO_DEVICE_MODE_HANDFREE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_LOOPBHANDFREE_E;
        break;
        case AUDIO_DEVICE_MODE_EARPHONE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_E;
        break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        break;
        default:
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
            SCI_TRACE_LOW ("[_LOOPBACK_GetDspCodecId]: mode:%d", eDev_mode);
            break;
    }    
    return dsp_codec_id;
}

LOCAL AUDIO_RESULT_E  Dspvb_PreClose(void *ptParam)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return AUDIO_NO_ERROR;
#if 0
    AUDIO_HAL_Ioctl(AUDIO_CTL_DAI_BY_ARM,0);
    return AUDIO_NO_ERROR;
#endif
}   
LOCAL int32 DSPVB_DrvOutSwitch(uint32 id,uint32 eMode,BOOLEAN is_on)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_SWITCH_T *aud_ctl_switch_ptr = PNULL;
    aud_ctl_switch_ptr = (AUDIO_CTL_SWITCH_T *)AUDIO_ALLOC(sizeof(AUDIO_CTL_SWITCH_T));
    if(PNULL==aud_ctl_switch_ptr)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_DrvOutSwitch, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_75_112_2_18_1_2_15_102,(uint8*)"d",sizeof(AUDIO_CTL_SWITCH_T));
        return AUDIO_HAL_ERROR;
    }
    aud_ctl_switch_ptr->id = id;
    aud_ctl_switch_ptr->chan = AUDIO_ALL_CHAN;
    aud_ctl_switch_ptr->mode = eMode;
    aud_ctl_switch_ptr->val = is_on;
    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DRV_OUT, (uint32)aud_ctl_switch_ptr);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_DrvOutSwitch, ioctl fail, eMode: %d, is_on: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_85_112_2_18_1_2_15_103,(uint8*)"dd",aud_ctl_switch_ptr->mode,aud_ctl_switch_ptr->val);
        SCI_FREE(aud_ctl_switch_ptr);
        return AUDIO_HAL_ERROR;
    }
    SCI_FREE(aud_ctl_switch_ptr);    
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 DSPVB_DrvInSwitch(uint32 id,uint32 eMode,BOOLEAN is_on)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_SWITCH_T *aud_ctl_switch_ptr = PNULL;
    aud_ctl_switch_ptr = (AUDIO_CTL_SWITCH_T *)AUDIO_ALLOC(sizeof(AUDIO_CTL_SWITCH_T));
    if(PNULL==aud_ctl_switch_ptr)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_DrvInSwitch, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_100_112_2_18_1_2_15_104,(uint8*)"d",sizeof(AUDIO_CTL_SWITCH_T));
        return AUDIO_HAL_ERROR;
    }
    aud_ctl_switch_ptr->id = id;
    aud_ctl_switch_ptr->chan = AUDIO_ALL_CHAN;
    aud_ctl_switch_ptr->mode = eMode;
    aud_ctl_switch_ptr->val = is_on;
    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DRV_IN, (uint32)aud_ctl_switch_ptr);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_DrvInSwitch, ioctl fail, eMode: %d, is_on: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_110_112_2_18_1_2_15_105,(uint8*)"dd",aud_ctl_switch_ptr->mode,aud_ctl_switch_ptr->val);
        SCI_FREE(aud_ctl_switch_ptr);
        return AUDIO_HAL_ERROR;
    }
    SCI_FREE(aud_ctl_switch_ptr);    
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 DSPVB_SetInnerPA(uint16 pa_l_setting)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_PA_T *aud_ctl_pa_ptr = PNULL;
    aud_ctl_pa_ptr = (AUDIO_CTL_PA_T *)AUDIO_ALLOC(sizeof(AUDIO_CTL_PA_T));
    if(PNULL == aud_ctl_pa_ptr)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_SetInnerPA, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_125_112_2_18_1_2_15_106,(uint8*)"d",sizeof(AUDIO_CTL_PA_T));
        return AUDIO_HAL_ERROR;
    }
//    SCI_TRACE_LOW("dspvb_as.c, DSPVB_SetInnerPA, pa_l_setting:0x%x ",pa_l_setting);
    aud_ctl_pa_ptr->chan = AUDIO_ALL_CHAN;
    aud_ctl_pa_ptr->id = 0;
    aud_ctl_pa_ptr->LDO_V_sel  = (pa_l_setting & VB_DSP_PA_LDO_V_SEL_MASK);
    aud_ctl_pa_ptr->DTRI_F_sel = (pa_l_setting & VB_DSP_PA_DTRI_F_SEL_MASK)>>VB_DSP_PA_DTRI_F_SEL_SHIFT;
    aud_ctl_pa_ptr->is_DEMI     = (pa_l_setting & BIT_7)>>7;
    aud_ctl_pa_ptr->is_classD = (pa_l_setting & BIT_8)>>8;
    aud_ctl_pa_ptr->is_LDO = (pa_l_setting & BIT_9)>>9;
    aud_ctl_pa_ptr->is_auto_LDO = (pa_l_setting & BIT_10)>>10;
    aud_result = AUDIO_HAL_Ioctl(AUDIO_PA_SETTING,(uint32)aud_ctl_pa_ptr);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_SetInnerPA, ioctl fail, pa_l_setting: 0x%x  "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_138_112_2_18_1_2_15_107,(uint8*)"d",pa_l_setting);
        SCI_FREE(aud_ctl_pa_ptr);
        return AUDIO_HAL_ERROR;
    }
    SCI_FREE(aud_ctl_pa_ptr);    
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 DSPVB_SetPgaLevel(uint32 id,uint32 eChannel,uint32 pga_level,uint32 eCodecMode)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_PGA_T *aud_ctl_pga_ptr = PNULL;
    aud_ctl_pga_ptr = (AUDIO_CTL_PGA_T *)AUDIO_ALLOC(sizeof(AUDIO_CTL_PGA_T));
    if(PNULL==aud_ctl_pga_ptr)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_SetPgaLevel, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_153_112_2_18_1_2_15_108,(uint8*)"d",sizeof(AUDIO_CTL_PGA_T));
        return AUDIO_HAL_ERROR;
    }
    aud_ctl_pga_ptr->id = id;
    aud_ctl_pga_ptr->chan = eChannel;
    aud_ctl_pga_ptr->level = pga_level;
    aud_ctl_pga_ptr->mode = eCodecMode;
    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_PGA, (uint32)aud_ctl_pga_ptr);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_SetPgaLevel, ioctl fail, pga_level: %d eCodecMode: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_163_112_2_18_1_2_15_109,(uint8*)"dd",aud_ctl_pga_ptr->level,aud_ctl_pga_ptr->mode);
        SCI_FREE(aud_ctl_pga_ptr);
        return AUDIO_HAL_ERROR;
    }
    SCI_FREE(aud_ctl_pga_ptr);    
    return AUDIO_HAL_SUCCESS;
}

/*****************************************************************************/
//  Description:    DSP VB Sets speaker pa and headset pa.
//  Author:         Johnson.sun
//  Param bEnable: TRUE=set, FALSE=off
//  Note:
//****************************************************************************/
PUBLIC void DSPVB_EnableAmplifier (BOOLEAN bEnable, NV_AUDIO_DSP_CODEC_E dsp_codec_id)
{
    BOOLEAN is_mute = SCI_FALSE;

    AUDIO_GetDownLinkMute (&is_mute);

    //SCI_TRACE_LOW("[DSPVB_EnableAmplifier] cur device mode = %d, bEnable=%d, s_is_vb_mute:%d.", cur_dev_mode, bEnable, s_is_vb_mute);

    if (bEnable && !is_mute)
    {
        AUDIO_NV_DSP_MODE_INFO_T aud_mod_param_t = {0}; 
        char *mode_name = PNULL;
        
        mode_name = AUDIONVDSP_GetAudioModeName(dsp_codec_id);        
        SCI_ASSERT(mode_name != PNULL); /*assert verified*/
        AUDIONVDSP_GetModeParam(mode_name,(AUDIO_NV_DSP_MODE_INFO_T*)(&aud_mod_param_t));
        DSPVB_DrvOutSwitch(0,AUDIO_SPK,aud_mod_param_t.tAudioNvDspModeStruct.path_set & BIT_4);  //left
        DSPVB_DrvOutSwitch(1,AUDIO_SPK,aud_mod_param_t.tAudioNvDspModeStruct.path_set & BIT_5);  //right
    }
    else
    {
        DSPVB_DrvOutSwitch(0,AUDIO_SPK,SCI_FALSE);
        DSPVB_DrvOutSwitch(1,AUDIO_SPK,SCI_FALSE);
    }

}

LOCAL AUDIO_NV_DSP_RESULT_E DSPVB_Codec_SetMode(AUDIO_NV_DSP_MODE_INFO_T* ptMode, NV_AUDIO_DSP_CODEC_E dsp_codec_id)
{
    uint32 cur_vol = 0;
    AUDIO_ARM_APP_TYPE_E cur_app_type = 0;
    if(SCI_NULL == ptMode)
        {
            //AUDIO_PRINT:"dspvb_as.c:DSPVB_Codec_SetMode:error:Mode:0x%x "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_210_112_2_18_1_2_15_110,(uint8*)"d", ptMode);
            ASM_ASSERT_WARNING(SCI_FALSE);        
            return AUDIO_NV_DSP_INPUT_PARA_ERROR;
        }
    
    DSPVB_SetInnerPA(ptMode->tAudioNvDspModeStruct.reserve[0]);      //inner-pa-gain:dsp_codec_cfg-reserve[0]
    // Open or close the earphone according to current mode
    if (ptMode->tAudioNvDspModeStruct.path_set & BIT_6)
    {
        DSPVB_DrvOutSwitch(0,AUDIO_EAR,SCI_TRUE);
    }
    else
    {
        DSPVB_DrvOutSwitch(0,AUDIO_EAR,SCI_FALSE);
    } 

    // Set the volume of audio device
    AUDIO_GetVolumeAndType(&cur_vol,&cur_app_type);
    DSPVB_Codec_SetVolume(cur_vol, dsp_codec_id);
    // Open or close the headphone according to current mode
    if (ptMode->tAudioNvDspModeStruct.path_set & BIT_0)
    {
        DSPVB_DrvOutSwitch(0,AUDIO_HP,SCI_TRUE);
    }
    else
    {
        DSPVB_DrvOutSwitch(0,AUDIO_HP,SCI_FALSE);
    }

    // Open or close the speaker according to current mode
    if (ptMode->tAudioNvDspModeStruct.path_set & BIT_3)
    {
        DSPVB_DrvOutSwitch(0,AUDIO_SPK,SCI_TRUE);
    }
    else
    {        
        DSPVB_DrvOutSwitch(0,AUDIO_SPK,SCI_FALSE);
    }    

    if (ptMode->tAudioNvDspModeStruct.path_set & BIT_10)
    {
        DSPVB_DrvInSwitch(0,AUDIO_MIC,SCI_TRUE);
    }
    if (ptMode->tAudioNvDspModeStruct.path_set & BIT_12)
    {
        DSPVB_DrvInSwitch(0,AUDIO_HP_MIC,SCI_TRUE);
    }
    if (ptMode->tAudioNvDspModeStruct.path_set & BIT_11)
    {
        DSPVB_DrvInSwitch(1,AUDIO_MIC,SCI_TRUE);
    }
    
    if (!(ptMode->tAudioNvDspModeStruct.path_set & BIT_10))
    {
        DSPVB_DrvInSwitch(0,AUDIO_MIC,SCI_FALSE);
    }
    if (!(ptMode->tAudioNvDspModeStruct.path_set & BIT_11))
    {
        DSPVB_DrvInSwitch(1,AUDIO_MIC,SCI_FALSE);
    } 
    if (!(ptMode->tAudioNvDspModeStruct.path_set & BIT_12))
    {
        DSPVB_DrvInSwitch(0,AUDIO_HP_MIC,SCI_FALSE);
    }


    return AUDIO_NV_DSP_NO_ERROR;
}

LOCAL AUDIO_NV_DSP_RESULT_E DSPVB_Codec_SetVolume(uint32  uiVol,  NV_AUDIO_DSP_CODEC_E dsp_codec_id)
{
    char *mode_name = NULL;
    uint16 gain = 0;
    uint8 pga_ad_gain_l = 0;
    uint8 pga_ad_gain_r = 0;
    uint8 pga_da_gain_l = 0;
    uint8 pga_da_gain_r = 0;
    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    AUDIO_NV_DSP_MODE_INFO_T* ptAudMode = PNULL;

    if(    uiVol < AUDMOD_MIN_VOL)
    {
        uiVol = AUDMOD_MIN_VOL;
    }
    
    ptAudMode = (AUDIO_NV_DSP_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_DSP_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_Codec_SetVolume, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_299_112_2_18_1_2_15_111,(uint8*)"d",sizeof(AUDIO_NV_DSP_MODE_INFO_T));
        return AUDIO_NV_DSP_ERROR;
    }
//    s_vb_volumn = uiVol;
    AUDIO_GetDevMode (&cur_dev_mode);

    mode_name = AUDIONVDSP_GetAudioModeName(dsp_codec_id);   //"handset"...
    if(PNULL==mode_name)
    {
        //SCI_TRACE_LOW:"dspvb_as.c DSPVB_Codec_SetVolume mode:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_307_112_2_18_1_2_15_112,(uint8*)"d", cur_dev_mode);
        return AUDIO_NV_DSP_ERROR;
    }
    if(cur_dev_mode != AUDIONVDSP_GetAudioModeType(mode_name))
    {
        SCI_TRACE_LOW("DSPVB_Codec_SetVolume, not appropriate, mode: %d, name: %s", cur_dev_mode, mode_name);
    }
    if(AUDIO_NV_DSP_NO_ERROR != AUDIONVDSP_GetModeParam(mode_name,(AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode))
    {
        SCI_FREE(ptAudMode);
        //SCI_TRACE_LOW:"dspvb_as.c DSPVB_Codec_SetVolume failed to get mode param."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_313_112_2_18_1_2_15_113,(uint8*)"");
            return AUDIO_NV_DSP_ERROR; 
    }

    gain = ptAudMode->tAudioNvDspModeStruct.vol_cfg0[uiVol & 0xFFFFFFFF];  //adc
    pga_ad_gain_l = gain & VB_DSP_VOL_L_MASK;
    pga_ad_gain_r = (gain & VB_DSP_VOL_R_MASK) >> VB_DSP_VOL_SHIFT ;
//    AUDDEV_SetADPGA(pga_ad_gain_l);
    if(AUDIO_DEVICE_MODE_HANDHOLD == cur_dev_mode)
    {
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_L,pga_ad_gain_l,AUDIO_MIC);        
    }else if(AUDIO_DEVICE_MODE_HANDFREE == cur_dev_mode)
    {
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_L,pga_ad_gain_l,AUDIO_MIC);    
    }else if(AUDIO_DEVICE_MODE_EARPHONE == cur_dev_mode)
    {    
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_L,pga_ad_gain_l,AUDIO_HP_MIC);    
    }else if(AUDIO_DEVICE_MODE_EARFREE == cur_dev_mode)
    {
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_L,pga_ad_gain_l,AUDIO_HP_MIC);    
    }

    //SCI_TRACE_LOW:"DSPVB_Codec_SetVolume vol_cfg0, 0x%x, 0x%x, 0x%x, 0x%x."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_335_112_2_18_1_2_15_114,(uint8*)"dddd", uiVol, gain, pga_ad_gain_l, pga_ad_gain_r);

    gain = ptAudMode->tAudioNvDspModeStruct.vol_cfg1[uiVol & 0xFFFFFFFF];  //dac
    pga_da_gain_l = gain & VB_DSP_VOL_L_MASK;
    pga_da_gain_r = (gain & VB_DSP_VOL_R_MASK) >> VB_DSP_VOL_SHIFT ;
//    AUDDEV_SetPGA (0, pga_da_gain_l);
//    AUDDEV_SetPGA (1, pga_da_gain_r);
    if(AUDIO_DEVICE_MODE_HANDHOLD == cur_dev_mode)
    {
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_L,pga_da_gain_l,AUDIO_EAR);
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_R,pga_da_gain_r,AUDIO_EAR);        
    }else if(AUDIO_DEVICE_MODE_HANDFREE == cur_dev_mode)
    {
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_L,pga_da_gain_l,AUDIO_SPK);
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_R,pga_da_gain_r,AUDIO_SPK);    
    }else if(AUDIO_DEVICE_MODE_EARPHONE == cur_dev_mode)
    {    
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_L,pga_da_gain_l,AUDIO_HP);
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_R,pga_da_gain_r,AUDIO_HP);    
    }else if(AUDIO_DEVICE_MODE_EARFREE == cur_dev_mode)
    {
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_L,pga_da_gain_l,AUDIO_SPK | AUDIO_HP);
        DSPVB_SetPgaLevel(0,AUDIO_CHAN_R,pga_da_gain_r,AUDIO_SPK | AUDIO_HP);    
    }

    //SCI_TRACE_LOW:"DSPVB_Codec_SetVolume vol_cfg1, 0x%x, 0x%x, 0x%x, 0x%x."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_360_112_2_18_1_2_15_115,(uint8*)"dddd", uiVol, gain, pga_ad_gain_l, pga_ad_gain_r);
    SCI_FREE(ptAudMode);
    return AUDIO_NV_DSP_NO_ERROR;
}

/*****************************************************************************/
// Description :    Open the audio device by calling the driver interface. And
//                  wait the driver openning the device and switch it to DSP
//                  control.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
LOCAL BOOLEAN _DSPVB_OpenDev (uint32 uiSampleRate,AUDIO_NV_DSP_MODE_INFO_T *aud_dsp_mod_param,NV_AUDIO_DSP_CODEC_E dsp_codec_id)
{
    uint8 bSwitchDspCtl = 0;
    uint16 dsp_codec_info = 0;
    uint32 aud_dev_open_return = AUDIO_HAL_SUCCESS;    
//    DSPVB_DrvOutSwitch(0,AUDIO_HP,is_turn_on_slowly);
    DSPVB_Codec_SetMode(aud_dsp_mod_param,dsp_codec_id);
    AUDIONVDSP_GetAudioDspCodecInfoFromRam(&dsp_codec_info); //check whether controlled by dsp for debuging, default:0
    bSwitchDspCtl = ((dsp_codec_info >> 8) & BIT_0);
    if(bSwitchDspCtl)
    {
        aud_dev_open_return = AUDIO_HAL_Open( AUDIO_FUN_DSP_CTL_CODEC | VB_DSP_OPEN_FLAG,uiSampleRate);
    }else
    {
        aud_dev_open_return = AUDIO_HAL_Open(VB_DSP_OPEN_FLAG,uiSampleRate);
    }
    if(AUDIO_HAL_SUCCESS != aud_dev_open_return)
    {
        //SCI_TRACE_LOW:"_DSPVB_OpenDev AUDIO_HAL_Open failed."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_385_112_2_18_1_2_15_116,(uint8*)"");
        return SCI_FALSE;
    }


    return SCI_TRUE;
}

/*****************************************************************************/
//  Description:    system delay
//  Author:
//  Note:
//****************************************************************************/
LOCAL void DSPVB_SystemDelay (uint32 time)
{
    uint32  temp = 0;

    temp = SCI_GetTickCount();

    while ( (uint32) (SCI_GetTickCount()-temp) < time)
    {
        ;
    }
}

#include "rpc_aud.h"
#include "rpc_aud_api.h"
extern SND_ITF_T g_cur_itf;
extern AUD_LEVEL_T g_aud_level_cfg;
extern VOIS_AUDIO_CFG_T g_audio_cfg;
extern SND_BT_WORK_MODE_T g_btworkmode;
extern int32 g_audio_dev_used;
extern BOOLEAN g_tone_playing;
extern AUD_AP2CP_PARA_T g_zsp_para;
SND_BT_WORK_MODE_T g_lastbtworkmode = SND_BT_WORK_MODE_NO;
/*****************************************************************************/
//  Description:    Set DSP VB Uplink Mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void DSPVB_SetUplinkMute (
    BOOLEAN is_mute
)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] is_mute:0x%x", __LINE__, __FILE__, __FUNCTION__, is_mute);

    cur_itf = (AUD_ITF_T)g_cur_itf;
    if (TRUE == is_mute)
    {
        g_audio_cfg.micLevel = AUD_MIC_MUTE;
    }
    else
    {
        g_audio_cfg.micLevel = AUD_MIC_ENABLE;
    }
    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType = AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath = AUD_INPUT_PATH1;
    device_cfg_ext.linePath = AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
#if 0
#ifndef DSPVB_USE_DM_TONE_INTERFACE
    vois_ret = vois_SetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Play vois_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#else
    vois_ret = DM_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] DM_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
#else
    vois_ret = AUDIO_CP_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] AUDIO_CP_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return;
#if 0
    //SCI_TRACE_LOW:"[DSPVB_SetUplinkMute]: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_417_112_2_18_1_2_16_117,(uint8*)"d", is_mute);
#ifdef BT_DIALER_SUPPORT 
    if(DOWNLINK_GetWorkMode())
    bt_dialer_uplink_mute_flag = is_mute;
#endif
    LAYER1_SetUlnkMute (is_mute, 0);
#endif
}
/*****************************************************************************/
//  Description:    
//  Author:         
//  Note:
//****************************************************************************/
LOCAL void DSPVB_SetExtraVolume (
    BOOLEAN bExtraVolFlag,
    uint16 usExtraVolGain
    )
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return;
#if 0
    SCI_TRACE_LOW ("dspvb_as.c [DSPVB_SetExtraVolume] info:0x%x, 0x%x", bExtraVolFlag, usExtraVolGain);

    if(usExtraVolGain<DSPVB_EXTRA_VOL_GAIN_MIN)
    {
        SCI_TRACE_LOW ("dspvb_as.c [DSPVB_SetExtraVolume] gain min:%x", DSPVB_EXTRA_VOL_GAIN_MIN);

        usExtraVolGain = DSPVB_EXTRA_VOL_GAIN_MIN;
    }

    // Set DSP side volume
    AUDMOD_SetExtraVolume(bExtraVolFlag, usExtraVolGain);
#endif
}

/*****************************************************************************/
//  Description:    Set DSP VB volume.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void DSPVB_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType, NV_AUDIO_DSP_CODEC_E dsp_codec_id)
{
    //SCI_TRACE_LOW:"[DSPVB_SetVolume] uiVol= 0x%4x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_428_112_2_18_1_2_16_118,(uint8*)"d", uiVol);


    // Set DSP side volume
    DSPVB_Layer1SetVolume (uiVol);
    DSPVB_Codec_SetVolume(uiVol, dsp_codec_id);
}

LOCAL void DSPVB_Layer1SetVolume(uint32  uiVol)
{
    if (uiVol < AUDMOD_MIN_VOL)
    {
        uiVol = AUDMOD_MIN_VOL;
    }
#ifdef BT_DIALER_SUPPORT
     AUDMOD_SetVolume(uiVol);//add by EE
#else
     LAYER1_SetDevVolume (uiVol);
#endif
}


LOCAL void _LAYER1_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    SCI_TRACE_LOW("[ljf] line[%d] _LAYER1_SetVolume uiVol:0x%x", __LINE__, uiVol);

    g_aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)uiVol;

    cur_itf = (AUD_ITF_T)g_cur_itf;
    g_audio_cfg.spkLevel = g_aud_level_cfg.spkLevel;
    //g_audio_cfg.micLevel = AUD_MIC_ENABLE;
    g_audio_cfg.sideLevel = AUD_SIDE_MUTE;
    g_audio_cfg.toneLevel = AUD_TONE_0dB;
    g_audio_cfg.encMute = 0;
    g_audio_cfg.decMute = 0;
    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType = AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath = AUD_INPUT_PATH1;
    device_cfg_ext.linePath = AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
    SCI_TRACE_LOW("[ljf] line[%d] _LAYER1_SetVolume g_audio_cfg.spkLevel:0x%x", __LINE__, g_audio_cfg.spkLevel);
#if 0
#ifndef DSPVB_USE_DM_TONE_INTERFACE
    vois_ret = vois_SetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] _LAYER1_SetVolume vois_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#else
    vois_ret = DM_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] _LAYER1_SetVolume DM_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
#else
    vois_ret = AUDIO_CP_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] _LAYER1_SetVolume AUDIO_CP_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
    return;
}


LOCAL void _RECORD_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = (AUDIO_DEVICE_MODE_TYPE_E)0;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_GetDevMode (&cur_dev_mode);

    dsp_codec_id = _RECORD_GetDspCodecId(cur_dev_mode);    
    
    SCI_TRACE_LOW("RECORD_SetVolume %d, %d", uiVol, eAppType);
    
    DSPVB_SetVolume(uiVol, eAppType, dsp_codec_id);
}

LOCAL void _MP4RECORD_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
#if 1
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return;
#else
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = (AUDIO_DEVICE_MODE_TYPE_E)0;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_GetDevMode (&cur_dev_mode);

    dsp_codec_id = _MP4RECORD_GetDspCodecId(cur_dev_mode);    
    
    SCI_TRACE_LOW(" MP4RECORD_SetVolume%d, %d", uiVol, eAppType);
    
    DSPVB_SetVolume(uiVol, eAppType, dsp_codec_id);
#endif
}

LOCAL void _LOOPBACK_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
#if 1
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d]", __FILE__, __FUNCTION__, __LINE__);
    return;
#else
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = (AUDIO_DEVICE_MODE_TYPE_E)0;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_GetDevMode (&cur_dev_mode);

    dsp_codec_id = _LOOPBACK_GetDspCodecId(cur_dev_mode);    
    
    SCI_TRACE_LOW("LOOPBACK_SetVolume %d, %d", uiVol, eAppType);
    
    DSPVB_SetVolume(uiVol, eAppType, dsp_codec_id);
#endif
}


LOCAL uint32 DSPVB_GetSupportFs(uint32 uiSampleRate)
{
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    aud_result = AUDIO_HAL_Ioctl(AUDIO_GET_SUPPORT_FS, (uint32)&uiSampleRate);
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, DSPVB_GetSupportFs, ioctl fail, uiSampleRate: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_446_112_2_18_1_2_16_119,(uint8*)"d",uiSampleRate);
        return 0;
    }
    return uiSampleRate;
}

/*****************************************************************************/
//  Description:    get appropriate output samplerate.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL uint32 DSPVB_GetAppropriateOutputSamplerate (uint32 uiSampleRate)
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
    else if(uiSampleRate==96000)
    {
        return uiSampleRate;
    }
    else
    {
        SCI_TRACE_LOW("DSPVB samplerate is invalid = %ld", uiSampleRate);
        uiSampleRate = 0;
        return uiSampleRate;
    }
#if 0
    return DSPVB_GetSupportFs (uiSampleRate);
#endif
}
LOCAL uint32 DSPVB_Record_GetAppropriateOutputSamplerate (uint32 uiSampleRate)
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
        SCI_TRACE_LOW("DSPVB_Record samplerate is invalid = %ld", uiSampleRate);
        uiSampleRate = 0;
        return uiSampleRate;
    }
#if 0
    return DSPVB_GetSupportFs (uiSampleRate);
#endif
}

/*****************************************************************************/
//  Description:    Set DSP VB Downlink Mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void LAYER1_SetDownlinkMute (
    BOOLEAN is_mute
)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return;
#if 0
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = (AUDIO_DEVICE_MODE_TYPE_E)0;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_GetDevMode (&cur_dev_mode);

    dsp_codec_id = _LAYER1_GetDspCodecId(cur_dev_mode);

    //SCI_TRACE_LOW:"[LAYER1_SetDownlinkMute]: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_470_112_2_18_1_2_16_120,(uint8*)"d", is_mute);
        
    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DAC, (uint32)(!is_mute));
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, LAYER1_SetDownlinkMute, ioctl fail, is_mute: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_475_112_2_18_1_2_16_121,(uint8*)"d",is_mute);
    }

    if(is_mute)
    {
        DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
    }
    else
    {
        DSPVB_EnableAmplifier(SCI_TRUE, dsp_codec_id);
    }     

    // Set DSP side downlink mute
    LAYER1_SetDlnkMute (is_mute, 0);
#endif
}

/*****************************************************************************/
//  Description:    Set DSP VB Downlink Mute or Cancel the mute_set.
//  Author:         shujing.dong
//  Note:  
//****************************************************************************/
LOCAL void LAYER1_SetDownlinkLocalNetMute(
    BOOLEAN isLocalMute,
    BOOLEAN isNetMute 
    )
{ 
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return;
#if 0
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = (AUDIO_DEVICE_MODE_TYPE_E)0;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_GetDevMode (&cur_dev_mode);

    dsp_codec_id = _LAYER1_GetDspCodecId(cur_dev_mode);

    //SCI_TRACE_LOW:"[LAYER1_SetDownlinkLocalNetMute]: local:%d, net:%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_491_112_2_18_1_2_16_122,(uint8*)"dd", isLocalMute, isNetMute);
    
    if(isLocalMute)
    {
        DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
    }
    else
    {
        DSPVB_EnableAmplifier(SCI_TRUE, dsp_codec_id);
    }     
    
    // Set DSP side downlink mute
    LAYER1_SetDlnkMute(isNetMute, 0);
#endif
}   


/*****************************************************************************/
//  Description:    Set DSP VB Downlink Mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void RECORD_SetDownlinkMute (
    BOOLEAN is_mute
)
{
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = (AUDIO_DEVICE_MODE_TYPE_E)0;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_GetDevMode (&cur_dev_mode);

    dsp_codec_id = _LAYER1_GetDspCodecId(cur_dev_mode);

    //SCI_TRACE_LOW:"[RECORD_SetDownlinkMute]: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_515_112_2_18_1_2_16_123,(uint8*)"d", is_mute);

    if(is_mute)
    {
//        MP4RECORD_EnableAmplifier(SCI_FALSE);
            DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
    }
    else
    {
//        MP4RECORD_EnableAmplifier(SCI_TRUE);
            DSPVB_EnableAmplifier(SCI_TRUE, dsp_codec_id);
    }    


    // Set DSP side downlink mute
    LAYER1_SetDlnkMute (is_mute, 0);
}

/*****************************************************************************/
//  Description:    Set DSP VB Downlink Mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void MP4RECORD_SetDownlinkMute (
    BOOLEAN is_mute
)
{
#if 1
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return;
#else
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = (AUDIO_DEVICE_MODE_TYPE_E)0;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_GetDevMode (&cur_dev_mode);

    dsp_codec_id = _MP4RECORD_GetDspCodecId(cur_dev_mode);

    //SCI_TRACE_LOW:"[MP4RECORD_SetDownlinkMute]: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_541_112_2_18_1_2_16_124,(uint8*)"d", is_mute);

    if(is_mute)
    {
//        MP4RECORD_EnableAmplifier(SCI_FALSE);
            DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
    }
    else
    {
//        MP4RECORD_EnableAmplifier(SCI_TRUE);
            DSPVB_EnableAmplifier(SCI_TRUE, dsp_codec_id);
    }    


    // Set DSP side downlink mute
    LAYER1_SetDlnkMute (is_mute, 0);
#endif
}

/*****************************************************************************/
//  Description:    Set DSP VB Downlink Mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void LOOPBACK_SetDownlinkMute (
    BOOLEAN is_mute
)
{
#if 1
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d]", __FILE__, __FUNCTION__, __LINE__);
    return;
#else
    uint32 aud_result = AUDIO_HAL_SUCCESS;
    //SCI_TRACE_LOW:"[LOOPBACK_SetDownlinkMute]: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_568_112_2_18_1_2_16_125,(uint8*)"d", is_mute);
        
    aud_result = AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DAC, (uint32)(!is_mute));
    if(AUDIO_HAL_SUCCESS != aud_result)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, LOOPBACK_SetDownlinkMute, ioctl fail, is_mute: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_573_112_2_18_1_2_16_126,(uint8*)"d",is_mute);
    }

    // Set DSP side downlink mute
    LAYER1_SetDlnkMute (is_mute, 0);
#endif
}

/*****************************************************************************/
//  Description:    Layer1 logic device Set device mode.
//  Author:         Johnson.sun
//  Note:  Audio struct param all come from NV for Calibration.
//****************************************************************************/
LOCAL void LAYER1_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
#if 0
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] eMode:0x%x", __LINE__, __FILE__, __FUNCTION__, eMode);
#else
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    SCI_TRACE_LOW("[ljf] line[%d] LAYER1_SetDevMode eMode:0x%x", __LINE__, eMode);

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
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;

        default:
            SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode eMode is [0x%x] WRONG!!!", __LINE__, eMode);
            break;
    }

    if (eMode != AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        g_lastbtworkmode = SND_BT_WORK_MODE_NO;
    }
        else
        {
            g_lastbtworkmode = g_btworkmode;
        }
    cur_itf = (AUD_ITF_T)g_cur_itf;
    g_audio_cfg.spkLevel = g_aud_level_cfg.spkLevel;
    SCI_TRACE_LOW("[ljf] line[%d] LAYER1_SetDevMode g_audio_cfg.spkLevel:0x%x g_lastbtworkmode:0x%x", __LINE__, g_audio_cfg.spkLevel, g_lastbtworkmode);
    //g_audio_cfg.micLevel = AUD_MIC_ENABLE;
    g_audio_cfg.sideLevel = AUD_SIDE_MUTE;
    g_audio_cfg.toneLevel = AUD_TONE_0dB;
    g_audio_cfg.encMute = 0;
    g_audio_cfg.decMute = 0;
    device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    device_cfg_ext.inputCircuityType = AUD_INPUT_MIC_CIRCUITY_TYPE_DIFFERENTIAL;
    device_cfg_ext.inputPath = AUD_INPUT_PATH1;
    device_cfg_ext.linePath = AUD_LINE_PATH2;
    device_cfg_ext.spkpaType = g_spkpatype;
#if 0
#ifndef DSPVB_USE_DM_TONE_INTERFACE
    vois_ret = vois_SetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode_Direct vois_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#else
    vois_ret = DM_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode_Direct ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
#else
    vois_ret = AUDIO_CP_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] ARMVB_SetDevMode AUDIO_CP_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
#endif
    return;
}

/*****************************************************************************/
//  Description:    Record  logic device Set device mode.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void RECORD_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    AUDIO_NV_DSP_MODE_INFO_T* ptAudMode = PNULL;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    uint32 cur_vol = 0;
    AUDIO_ARM_APP_TYPE_E cur_app_type = 0;
    ptAudMode = (AUDIO_NV_DSP_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_DSP_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, RECORD_SetDevMode, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_683_112_2_18_1_2_16_132,(uint8*)"d",sizeof(AUDIO_NV_DSP_MODE_INFO_T));
        return;
    }
    

#ifdef CUSTOMER_SAMSUNG
    //SCI_TRACE_LOW:"[RECORD_SetDevMode]: enter  mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_689_112_2_18_1_2_16_133,(uint8*)"d", eMode);
#endif

#ifdef MRAPP_SUPPORT
/* ---- SKY_MULTIMEDIA start ----*/ 
#if defined (__SKY_MM_STREAMLINED__)
{
    extern uint8 gMPchatMMIState;

    //if(1 == gMPchatMMIState)
    {
        eMode = AUDIO_DEVICE_MODE_HANDFREE;
    }
}
#endif
/* ---- SKY_MULTIMEDIA end ----*/
#endif

#ifdef DEV_MODE_EX_SUPPORT
    if (eMode == AUDIO_DEVICE_MODE_HANDFREE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_E;
        AUDMOD_SetDevModeEx ( (const uint8 *) "Record", PNULL, (const uint8 *) "Record");
        AUDIONVDSP_GetModeParam( (const uint8 *) "Record", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else if (eMode == AUDIO_DEVICE_MODE_EARPHONE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORDHEADSET_E;
        AUDMOD_SetDevModeEx ( (const uint8 *) "RecordHeadset", PNULL, (const uint8 *) "RecordHeadset");
        AUDIONVDSP_GetModeParam( (const uint8 *) "RecordHeadset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_E;
       AUDMOD_SetDevModeEx ( (const uint8 *) "Record", PNULL, (const uint8 *) "Record");
    AUDIONVDSP_GetModeParam( (const uint8 *) "Record", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        //SCI_TRACE_LOW:"[RECORD_SetDevMode]:Wrong mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_705_112_2_18_1_2_16_134,(uint8*)"d", eMode);
    }
#else
    if (eMode == AUDIO_DEVICE_MODE_HANDFREE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_E;
        AUDMOD_SetDevMode ( (const uint8 *) "Record", PNULL);
    AUDIONVDSP_GetModeParam( (const char*) "Record", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else if (eMode == AUDIO_DEVICE_MODE_EARPHONE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORDHEADSET_E;
        AUDMOD_SetDevMode ( (const uint8 *) "RecordHeadset", PNULL);
    AUDIONVDSP_GetModeParam( (const char*) "RecordHeadset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_RECORD_E;
    AUDMOD_SetDevMode ( (const uint8 *) "Record", PNULL);
    AUDIONVDSP_GetModeParam( (const char*) "Record", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        //SCI_TRACE_LOW:"[RECORD_SetDevMode]: mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_720_112_2_18_1_2_16_135,(uint8*)"d", eMode);
    }
#endif
    DSPVB_Codec_SetMode((AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode, dsp_codec_id);
    AUDIO_GetVolumeAndType(&cur_vol,&cur_app_type);
    DSPVB_Layer1SetVolume (cur_vol);
    SCI_FREE(ptAudMode);
    return;
}

/*****************************************************************************/
//  Description:    MP4Record  logic device Set device mode.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void MP4RECORD_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
#if 1
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return;
#else
    AUDIO_NV_DSP_MODE_INFO_T* ptAudMode = PNULL;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    uint32 cur_vol = 0;
    AUDIO_ARM_APP_TYPE_E cur_app_type = 0;
    ptAudMode = (AUDIO_NV_DSP_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_DSP_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, MP4RECORD_SetDevMode, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_740_112_2_18_1_2_16_136,(uint8*)"d",sizeof(AUDIO_NV_DSP_MODE_INFO_T));
        return;
    }
    
#ifdef CUSTOMER_SAMSUNG
    //SCI_TRACE_LOW:"[MP4RECORD_SetDevMode]: enter  mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_745_112_2_18_1_2_16_137,(uint8*)"d", eMode);
#endif

#ifdef MRAPP_SUPPORT
/* ---- SKY_MULTIMEDIA start ----*/ 
#if defined (__SKY_MM_STREAMLINED__)
{
        extern uint8 gMPchatMMIState;

        if(1 == gMPchatMMIState 
             && (eMode != AUDIO_DEVICE_MODE_HANDFREE
               && eMode != AUDIO_DEVICE_MODE_EARPHONE
               && eMode != AUDIO_DEVICE_MODE_TVOUT))
        {
            eMode = AUDIO_DEVICE_MODE_HANDFREE;
        }
}
#endif
/* ---- SKY_MULTIMEDIA end ---- */ 
#endif
    
#ifdef DEV_MODE_EX_SUPPORT

    if (eMode == AUDIO_DEVICE_MODE_HANDFREE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_E;
        AUDMOD_SetDevModeEx ( (const uint8 *) "MP4HFTR", PNULL, (const uint8 *) "MP4HFTR");
        AUDIONVDSP_GetModeParam( (const uint8 *) "MP4HFTR", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else if (eMode == AUDIO_DEVICE_MODE_EARPHONE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTRHEADSET_E;
        AUDMOD_SetDevModeEx ( (const uint8 *) "MP4HFTRHeadset", PNULL, (const uint8 *) "MP4HFTRHeadset");
        AUDIONVDSP_GetModeParam( (const uint8 *) "MP4HFTRHeadset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_E;
       AUDMOD_SetDevModeEx ( (const uint8 *) "MP4HFTR", PNULL, (const uint8 *) "MP4HFTR");
    AUDIONVDSP_GetModeParam( (const uint8 *) "MP4HFTR", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        //SCI_TRACE_LOW:"[MP4RECORD_SetDevMode]:Wrong mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_762_112_2_18_1_2_16_138,(uint8*)"d", eMode);
    }
#else
    if (eMode == AUDIO_DEVICE_MODE_HANDFREE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_E;
        AUDMOD_SetDevMode ( (const uint8 *) "MP4HFTR", PNULL);
    AUDIONVDSP_GetModeParam( (const char*) "MP4HFTR", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else if (eMode == AUDIO_DEVICE_MODE_EARPHONE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTRHEADSET_E;
        AUDMOD_SetDevMode ( (const uint8 *) "MP4HFTRHeadset", PNULL);
    AUDIONVDSP_GetModeParam( (const char*) "MP4HFTRHeadset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_MP4HFTR_E;
        AUDMOD_SetDevMode ( (const uint8 *) "MP4HFTR", PNULL);
    AUDIONVDSP_GetModeParam( (const char*) "MP4HFTR", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        //SCI_TRACE_LOW:"[MP4RECORD_SetDevMode]:Wrong mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_777_112_2_18_1_2_16_139,(uint8*)"d", eMode);
    }
#endif
    DSPVB_Codec_SetMode((AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode, dsp_codec_id);
    AUDIO_GetVolumeAndType(&cur_vol,&cur_app_type);
    DSPVB_Layer1SetVolume (cur_vol);
    SCI_FREE(ptAudMode);
    return;
#endif
}

/*****************************************************************************/
//  Description:    Layer1 logic device Set device mode.
//  Author:         
//  Note:  Audio struct param all come from NV for Calibration.
//****************************************************************************/
LOCAL void LOOPBACK_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
#if 1
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d]", __FILE__, __FUNCTION__, __LINE__);
    return;
#else
    AUDIO_NV_DSP_MODE_INFO_T* ptAudMode = PNULL;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    uint32 cur_vol = 0;
    AUDIO_ARM_APP_TYPE_E cur_app_type = 0;
    ptAudMode = (AUDIO_NV_DSP_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_DSP_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        //SCI_TRACE_LOW:"dspvb_as.c, LOOPBACK_SetDevMode, alloc fail, size: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_795_112_2_18_1_2_16_140,(uint8*)"d",sizeof(AUDIO_NV_DSP_MODE_INFO_T));
        return ;
    }
    
#ifdef DEV_MODE_EX_SUPPORT

    if (eMode == AUDIO_DEVICE_MODE_HANDHOLD)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        AUDMOD_SetDevModeEx ( (const uint8 *) "Handset", SCI_NULL, (const uint8 *) "HandsetTD");
        AUDIONVDSP_GetModeParam( (const uint8 *) "Handset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else if (eMode == AUDIO_DEVICE_MODE_HANDFREE)
    {    
        dsp_codec_id =NV_AUDIO_DSP_CODEC_HANDSFREE_E ;
        AUDMOD_SetDevModeEx ( (const uint8 *) "LoopBHandfree", SCI_NULL, (const uint8 *) "LoopBHandfree");
        AUDIONVDSP_GetModeParam( (const uint8 *) "Handsfree", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);        
    }
    else if (eMode == AUDIO_DEVICE_MODE_EARPHONE)
    {
/*        if (s_pcm_on_count>0)
        {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
            DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
            AUDMOD_SetDevModeEx ( (const uint8 *) "BTHS", SCI_NULL, (const uint8 *) "BTHS");
            AUDIONVDSP_GetModeParam( (const uint8 *) "BTHS", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
            //SCI_TRACE_LOW:"LOOPBACK_SetDevMode s_pcm_on_count>0."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_819_112_2_18_1_2_16_141,(uint8*)"");
        }
        else*/
        {
            dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
            AUDMOD_SetDevModeEx ( (const uint8 *) "Headset", SCI_NULL, (const uint8 *) "HeadsetTD");
        AUDIONVDSP_GetModeParam( (const uint8 *) "Headset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        }
    }
    else if(eMode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
        AUDMOD_SetDevModeEx ( (const uint8 *) "BTHS", SCI_NULL, (const uint8 *) "BTHS");
        AUDIONVDSP_GetModeParam( (const uint8 *) "BTHS", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        //SCI_TRACE_LOW:"LOOPBACK_SetDevMode s_pcm_on_count>0."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_819_112_2_18_1_2_16_141,(uint8*)"");
    }
    else if (eMode == AUDIO_DEVICE_MODE_EARFREE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
        AUDMOD_SetDevModeEx ( (const uint8 *) "Headfree", SCI_NULL, (const uint8 *) "HeadfreeTD");
        AUDIONVDSP_GetModeParam( (const uint8 *) "Headfree", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);        
    }
    else
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
    AUDMOD_SetDevModeEx ( (const uint8 *) "Headset", SCI_NULL, (const uint8 *) "HeadsetTD");
    AUDIONVDSP_GetModeParam( (const uint8 *) "Headset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        //SCI_TRACE_LOW:"[LOOPBACK_SetDevMode]:Wrong mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_834_112_2_18_1_2_16_142,(uint8*)"d", eMode);
    }
#else

    if (eMode == AUDIO_DEVICE_MODE_HANDHOLD)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSET_E;
        AUDMOD_SetDevMode ( (const uint8 *) "Handset", SCI_NULL);
    AUDIONVDSP_GetModeParam( (const char*) "Handset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else if (eMode == AUDIO_DEVICE_MODE_HANDFREE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
        AUDMOD_SetDevMode ( (const uint8 *) "Handsfree", SCI_NULL);
    AUDIONVDSP_GetModeParam( (const char*) "Handsfree", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else if (eMode == AUDIO_DEVICE_MODE_EARPHONE)
    {
/*        if (s_pcm_on_count > 0)
        {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
            DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
            AUDMOD_SetDevMode ( (const uint8 *) "BTHS", SCI_NULL);
        AUDIONVDSP_GetModeParam( (const char*) "BTHS", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        }
        else*/
        {
                dsp_codec_id = NV_AUDIO_DSP_CODEC_HEADSET_E;
            AUDMOD_SetDevMode ( (const uint8 *) "Headset", SCI_NULL);
        AUDIONVDSP_GetModeParam( (const char*) "Headset", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        }
    }
    else if (eMode == AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_BTHS_E;
        DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
        AUDMOD_SetDevMode ( (const uint8 *) "BTHS", SCI_NULL);
        AUDIONVDSP_GetModeParam( (const char*) "BTHS", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);        
    }
    else if (eMode == AUDIO_DEVICE_MODE_EARFREE)
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
        AUDMOD_SetDevMode ( (const uint8 *) "Headfree", SCI_NULL);
    AUDIONVDSP_GetModeParam( (const char*) "Headfree", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
    }
    else
    {
        dsp_codec_id = NV_AUDIO_DSP_CODEC_HANDSFREE_E;
        AUDMOD_SetDevMode ( (const uint8 *) "Handsfree", SCI_NULL);
    AUDIONVDSP_GetModeParam( (const char*) "Handsfree", (AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode);
        //SCI_TRACE_LOW:"[LOOPBACK_SetDevMode]:Wrong mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_869_112_2_18_1_2_16_143,(uint8*)"d", eMode);
    }
#endif
    DSPVB_Codec_SetMode((AUDIO_NV_DSP_MODE_INFO_T *)ptAudMode, dsp_codec_id);
    AUDIO_GetVolumeAndType(&cur_vol,&cur_app_type);
    DSPVB_Layer1SetVolume (cur_vol);
    SCI_Sleep (30);
    DSPVB_EnableAmplifier(SCI_TRUE, dsp_codec_id);
    SCI_FREE(ptAudMode);
    return;
#endif
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E LAYER1_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{   
#if 0
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return 0;
#else
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    int32 dm_ret = 0;
    SCI_TRACE_LOW("{xx} LAYER1_Play SCI_GetSemaphore....");
    if(SCI_SUCCESS != SCI_GetSemaphore(audio_armvb_semaphore, 10))
    {
        SCI_TRACE_LOW("{xx} ARMVB_Play SCI_GetSemaphore return");
        return AUDIO_CONFLICT;
    }
    SCI_TRACE_LOW("{xx} LAYER1_Play SCI_GetSemaphore OK");
	SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s] g_audio_dev_used:%d", __LINE__, __FILE__, __FUNCTION__, g_audio_dev_used);
	SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Play eMode:0x%x spkLevel:%d ", __LINE__, ptDevSetting->eDev_mode, ptDevSetting->uiVol);
    if( (!s_b_dspvb_playing)&&(!g_tone_playing))
    {
        g_audio_dev_used = 1;
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
            g_cur_itf = SND_ITF_LOUD_SPEAKER;
            break;
        case AUDIO_DEVICE_MODE_TVOUT:
            g_cur_itf = SND_ITF_TV;
            break;
        case AUDIO_DEVICE_MODE_BLUEPHONE:
            g_cur_itf = SND_ITF_BLUETOOTH;
            break;

        default:
            SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Play eMode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
            break;
    }

    if (ptDevSetting->eDev_mode != AUDIO_DEVICE_MODE_BLUEPHONE)
    {
        g_lastbtworkmode = SND_BT_WORK_MODE_NO;
    }
        else
        {
            g_lastbtworkmode = g_btworkmode;
        }
    cur_itf = (AUD_ITF_T)g_cur_itf;
    g_aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)ptDevSetting->uiVol;

    g_audio_cfg.spkLevel = g_aud_level_cfg.spkLevel;
    SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Play g_audio_cfg.spkLevel:0x%x", __LINE__, g_audio_cfg.spkLevel);
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
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);

#if 0//ndef DSPVB_USE_CP_AUDIO_INTERFACE
#ifdef DSPVB_USE_PCIM_26M_CLOCK
        AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_LAYER1);
    //PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
#endif
#ifndef DSPVB_USE_DM_TONE_INTERFACE
    vois_ret = vois_SetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Play vois_ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#else
    vois_ret = DM_VoisSetCfg(cur_itf, &g_audio_cfg, device_cfg_ext, g_lastbtworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Play DM_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
#endif
    dm_ret = DM_StartAudioEx();
    if (0 != dm_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Play call DM_StartAudioEx dm_ret is [0x%x] WRONG!!!", __LINE__, dm_ret);
    }
#else
    vois_ret = AUDIO_CP_START(AUDIO_DEV_LAYER1,cur_itf, 0,0,&g_audio_cfg, device_cfg_ext, g_lastbtworkmode, &g_zsp_para);
    if (AUD_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("zgt line[%d] layer1_Play call AUDIO_CP_START faile ret:0x%x", __LINE__, vois_ret);
        return vois_ret;
    }
#endif
        s_b_dspvb_playing  = SCI_TRUE;
}else{
          SCI_TRACE_LOW("[zgt] line[%d] LAYER1_Play err dsp_playing=%d,toneplay=%d", __LINE__, s_b_dspvb_playing,g_tone_playing);
        }
        
    return AUDIO_NO_ERROR;
#endif

}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E LAYER1_Stop (DEV_SETTING_T *ptDevSetting)
{
    int32 dm_ret = 0;
    int8 delay_cnt = 10;
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    if (s_b_dspvb_playing)
    {
#if 0//ndef DSPVB_USE_CP_AUDIO_INTERFACE
        dm_ret = DM_StopAudioEx();
        if (0 != dm_ret)
        {
        	SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Play call DM_StopAudioEx dm_ret is [0x%x] WRONG!!!", __LINE__, dm_ret);
        }
#if 1
    // just for close 26M after close codec
    while (delay_cnt > 0)
    {
        dm_ret = DM_VoisSetBtNRECFlag(0);
        if (1 == dm_ret)
        {
            SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Stop call DM_VoisSetBtNRECFlag sleep 50ms ret:0x%x", __LINE__, dm_ret);
            SCI_SLEEP(50);
            delay_cnt--;
        }
        else
        {
            SCI_TRACE_LOW("[ljf] line[%d] LAYER1_Stop call DM_VoisSetBtNRECFlag succ and return ret:0x%x", __LINE__, dm_ret);
            break;
        }
    }
#endif
#ifdef DSPVB_USE_PCIM_26M_CLOCK
        //PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
        AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_LAYER1);
#endif
#else
        dm_ret = AUDIO_CP_STOP(AUDIO_DEV_LAYER1,SND_ITF_NONE, &g_zsp_para);
        if (AUD_ERR_NO != dm_ret)
        {
            SCI_TRACE_LOW("zgt line[%d] LAYER1_Stop call AUDIO_CP_STOP faile ret:0x%x", __LINE__, dm_ret);
            return dm_ret;
        }
#endif
        if(NULL!= g_PcmDumpFileHandle)
        {
            SFS_CloseFile(g_PcmDumpFileHandle);
        }
        SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
        g_audio_dev_used = 0;
        s_b_dspvb_playing = SCI_FALSE;
     }
    else{
           SCI_TRACE_LOW("[zgt] line[%d] LAYER1_stop not in playing state =%d", __LINE__, s_b_dspvb_playing);
     }
        SCI_TRACE_LOW("{xx} LAYER1_Stop SCI_PutSemaphore....");
        SCI_PutSemaphore(audio_armvb_semaphore);
        SCI_TRACE_LOW("{xx} LAYER1_Stop SCI_PutSemaphore OK");
    return AUDIO_NO_ERROR;
#if 0
    //SCI_TRACE_LOW:"[LAYER1_Stop]: s_b_dspvb_playing:%d,s_b_layer1_playing:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_934_112_2_18_1_2_17_147,(uint8*)"dd",s_b_dspvb_playing,s_b_layer1_playing);

    if (s_b_dspvb_playing)
    {
        NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;

        dsp_codec_id = _LAYER1_GetDspCodecId(ptDevSetting->eDev_mode);            
#ifdef DSP_CTRL_PCM
        PCM_Ctrl_Switch(TO_ARM_CTRL);
#endif
        if (!s_b_dspdata_playing) //dspdata stopped
        {

//            AUDDEV_Close (AUDDEV_FUNC_DSP_CTL, SCI_TRUE);
            AUDIO_HAL_Close(AUDIO_FUN_DSP_CTL_CODEC | AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP);
            s_b_dspvb_playing = SCI_FALSE;
        }
        else
        {
            DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
            //SCI_TRACE_LOW:"[LAYER1_Stop]: dspdata is using vb,so we should not close vb!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_951_112_2_18_1_2_17_148,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[LAYER1_Stop]: stop exception!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_956_112_2_18_1_2_17_149,(uint8*)"");
    }


    s_b_layer1_playing = SCI_FALSE;

    return AUDIO_NO_ERROR;
#endif
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E LAYER1_Pause (DEV_SETTING_T *ptDevSetting)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return AUDIO_NO_ERROR;
#if 0
    if (s_b_dspvb_playing)
    {
#ifdef DSP_CTRL_PCM
        PCM_Ctrl_Switch(TO_ARM_CTRL);
#endif
        if (!s_b_dspdata_playing) //dspdata stopped
        {
            AUDIO_HAL_Close(AUDIO_FUN_DSP_CTL_CODEC | AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP);
            s_b_dspvb_playing = SCI_FALSE;
        }
        else
        {
            DSPVB_DrvOutSwitch(0,AUDIO_SPK,SCI_FALSE);
            //SCI_TRACE_LOW:"[LAYER1_Pause]: dspdata is using vb,so we should not close vb!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_984_112_2_18_1_2_17_150,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[LAYER1_Pause]: pause exception!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_989_112_2_18_1_2_17_151,(uint8*)"");
    }

    s_b_layer1_playing = SCI_FALSE;

    return AUDIO_NO_ERROR;
#endif
}


/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E LAYER1_Resume (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return AUDIO_NO_ERROR;
#if 0
    char *mode_name= PNULL;
    AUDIO_NV_DSP_RESULT_E result = AUDIO_NV_DSP_NO_ERROR;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_NV_DSP_MODE_INFO_T aud_dsp_mod_param = {0};

    dsp_codec_id = _LAYER1_GetDspCodecId(ptDevSetting->eDev_mode); 
    mode_name =  AUDIONVDSP_GetAudioModeName(dsp_codec_id);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("dspvb_as.c LAYER1_Resume mode:%d.",ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }    
    result=AUDIONVDSP_GetModeParam (mode_name, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param));    
    if(AUDIO_NV_DSP_NO_ERROR!=result)
    {
        SCI_TRACE_LOW("dspvb_as.c LAYER1_Resume failed to get dev ctr info, result:%d.",result);
        return AUDIO_ERROR;
    }    

    if (!s_b_dspvb_playing)
    {
#ifdef DSP_CTRL_PCM
        PCM_Ctrl_Switch(TO_DSP_CTRL);
#endif
        LAYER1_PreSetAudioEnable(SCI_TRUE);
        if(!(_DSPVB_OpenDev (uiSampleRate,(AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param),dsp_codec_id)))
        {
            LAYER1_PreSetAudioEnable(SCI_FALSE);
            //SCI_TRACE_LOW:"[LAYER1_Resume] _DSPVB_OpenDev failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1014_112_2_18_1_2_17_152,(uint8*)"");
            return AUDIO_ERROR;
        }

        s_b_dspvb_playing = SCI_TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[LAYER1_Resume]: dsp vb is already opened by other device!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1022_112_2_18_1_2_17_153,(uint8*)"");
    }

    /*After DSP VB control operation, we have to set device mode and volume again.*/
    LAYER1_SetDevMode (ptDevSetting->eDev_mode);
    LAYER1_SetDownlinkMute (ptDevSetting->bDownlink_mute);
    DSPVB_SetUplinkMute (ptDevSetting->bUplink_mute);

    s_b_layer1_playing = SCI_TRUE;

    return AUDIO_NO_ERROR;
#endif
}
#if 1
#include "load_modem.h"
#include "hal_aud_zsp.h"
typedef struct
{
     uint16 *data_ptr;
     uint16 *data_ptr_r;
    uint32 size;
    uint32 in;
    uint32 out;
}VB_ARM_RECORD_BUF;
extern uint8 *g_zsp_share_memory_addr;
extern BOOLEAN s_b_armvb_recording;
extern VB_ARM_RECORD_BUF s_vb_arm_record_buf;
extern uint32 s_ad_read_size;
extern BOOLEAN s_b_armvb_recording;
extern uint32 s_record_callback_count;
typedef struct
{
    uint16 *data_ptr;
    uint16 *data_ptr_r;
    uint32 size;
    uint32 in;
    uint32 out;
}VB_DSP_RECORD_BUF;
RECORD_CALLBACK VoiceRec_Callback;
uint16 *VoiceRec_data_ptr;
#define VOICEREC_ZSP_TRANS_SIZE  (160)//160
LOCAL uint32 s_zsp_read_size = 0;
LOCAL VB_DSP_RECORD_BUF s_vb_dsp_record_buf;
LOCAL BOOLEAN s_b_dspvb_recording = SCI_FALSE;
extern uint8 *g_zsp_share_memory_addr;
extern uint32 g_zsp_share_memory_lenth;
LOCAL volatile uint32 zsp_callback_framecount = 0;
LOCAL BOOLEAN s_dspvb_pcmdumping = SCI_FALSE;
#define PCMDUMP_DEFAULT_TASK_ID 0xFFFFFFF
LOCAL BLOCK_ID tPcmDumpThreadId = PCMDUMP_DEFAULT_TASK_ID;
uint16  *pcm_dump_name= L"e:\\PcmDump.pcm";
//SFS_HANDLE      g_PcmDumpFileHandle = NULL;
#define PCM_DUMP_WRITE_DATA_TO_FILE_SIG 0
int8* PcmDumpOutBuf =NULL;
uint32 PcmDump_Buffer_lenth =0;//bytes
uint32 Zsp_ReadDataLen=0;
uint32 Pcmdump_file_len=0;
LOCAL volatile ADM_RECCALLBACK s_mp4record_callback;
//volatile uint32 s_mp4record_callback_count = 0;
//int8* savebuffer =NULL;
typedef struct 
{
    uint8   sec;        // secondsafter the minute - [0,59] 
    uint8   min;        // minutesafter the hour - [0,59] 
    uint8   hour;       // hours since midnight - [0,23] 
}PCMDUMP_TIME_T;
#define PCMDUMP_NAMESTR_MAX_NUM  60 


LOCAL void Dspvb_PcmDump_Thread_Entry(uint32 argc, void *argv )
{
#if 1
    BLOCK_ID    this_thread_id = SCI_IdentifyThread();    

    xSignalHeader  sig_in_ptr = PNULL;
    SCI_TRACE_LOW("[zgt]  fuc[%s]  line[%d] ", __FUNCTION__,__LINE__);
    while(s_dspvb_pcmdumping)
    {
        sig_in_ptr = (xSignalHeader)SCI_GetSignal(this_thread_id);
        switch (sig_in_ptr->SignalCode)
        {
            case PCM_DUMP_WRITE_DATA_TO_FILE_SIG:
            {
                SCI_TRACE_LOW("zgt:pcmdump bufferlen=%d,readlen=%d",PcmDump_Buffer_lenth,Zsp_ReadDataLen);

                if(Zsp_ReadDataLen>0)
                {
                       SFS_WriteFile( g_PcmDumpFileHandle, PcmDumpOutBuf,  PcmDump_Buffer_lenth, &Pcmdump_file_len, PNULL);
                }
                Zsp_ReadDataLen =0;
                SCI_TRACE_LOW("zgt:pcmdump bufferlen=%d,writed len=%d",PcmDump_Buffer_lenth,Pcmdump_file_len);
                //SCI_Sleep(1);
            }
            break;

            default:
            break;
        }
        SCI_FREE(sig_in_ptr);
    }
    #endif
}
LOCAL int32 DSPVB_Record_GetZspWritePoint(uint32* pWrite)
{
        int32 ret = 0;
        AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
        *pWrite = zsp_sharemem->audInPara.writeOffset;
        SCI_TRACE_LOW("[zgt]  fuc[%s] Write piont[%d] ", __FUNCTION__,*pWrite);
    
       return ret;

}

LOCAL int32 DSPVB_Record_SetZspReadPoint(uint32 pRead)
{

        int32 ret = 0;
        AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
        zsp_sharemem->audInPara.readOffset = pRead;
        SCI_TRACE_LOW("[zgt]  fuc[%s] Read piont[%d] ", __FUNCTION__, zsp_sharemem->audInPara.readOffset );

       return ret;
}
 /*****************************************************************************/
//  Description:    This function transmit the voice recorded data to uplayer buffer
//  Author:         guangtao.zhi
//    Note:           
/*****************************************************************************/
void DSPVB_Record_ReadData(uint16 *puiDestData, uint32 uiCount, uint32 *puiDataLen)
{
    uint32 write_point = 0;
    uint32 byte_count_1 = 0 ,byte_count_2=0;
    SCI_ASSERT (PNULL != puiDestData);  /*assert verified*/

    if((0==uiCount) || (0 == s_zsp_read_size))
    {
        SCI_TRACE_LOW("[zgt]  fuc[%s] need none. uiCount=[%d] ", __FUNCTION__,uiCount);

        *puiDataLen = 0;
        return;
    }
//    *puiDataLen = s_ad_read_size;
//    SCI_PASSERT ((s_ad_read_size <= uiCount), ("%d, %d.", s_ad_read_size, uiCount));  /*assert verified*/
    DSPVB_Record_GetZspWritePoint(&write_point);
    SCI_TRACE_LOW("[zgt]  fuc[%s] write_point[%d]s_vb_arm_record_buf.out[%d] ", __FUNCTION__,write_point,s_vb_dsp_record_buf.out);

    if(write_point> (s_vb_dsp_record_buf.out<<1))
    {
        byte_count_1 = write_point - (s_vb_dsp_record_buf.out<<1);
        if(byte_count_1 >= (uiCount<<1))
            byte_count_1 = (uiCount<<1);            
        SCI_MEMCPY(puiDestData,
            ((s_vb_dsp_record_buf.data_ptr)+(s_vb_dsp_record_buf.out)),byte_count_1);
    }else
    {
        byte_count_1 = (s_vb_dsp_record_buf.size<<1)-(s_vb_dsp_record_buf.out<<1) ;
        byte_count_2 = write_point;
        if(byte_count_1 >= (uiCount<<1))
        {
            byte_count_1 = (uiCount<<1);    
            byte_count_2 = 0;
        }else
        {
          if( byte_count_2 > ((uiCount<<1)-byte_count_1) )
          {
                byte_count_2 = (uiCount<<1)-byte_count_1;
          }
        }
        SCI_MEMCPY(puiDestData,
            ((s_vb_dsp_record_buf.data_ptr)+(s_vb_dsp_record_buf.out)),byte_count_1);

        #if 1
        if( byte_count_2 )
        {
            SCI_MEMCPY((puiDestData+(byte_count_1>>1)),(s_vb_dsp_record_buf.data_ptr),byte_count_2);
        }
        #endif
    }
        *puiDataLen = byte_count_1+byte_count_2;
        SCI_TRACE_LOW("[zgt]  fuc[%s] addr[0x%x] uiCount[%d]puiDataLen[%d] ", __FUNCTION__,puiDestData,uiCount,*puiDataLen);

}
 LOCAL void DSPVB_Record_MoveDataPointer (uint32 uiCount)
{
    uint32 write_point = 0;
    uint32 byte_count_1 = 0 ,byte_count_2=0;

    if((0==uiCount) || (0 == s_zsp_read_size))
    {
        return;
    }
 
    SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] uiCount[%d] ", __FUNCTION__,__LINE__,uiCount);
    DSPVB_Record_GetZspWritePoint(&write_point);
    SCI_TRACE_LOW("[zgt] fuc[%s] out[%d] uiCount[%d] ", __FUNCTION__,s_vb_dsp_record_buf.out<<1,uiCount);

    if(write_point> (s_vb_dsp_record_buf.out<<1))
    {
        byte_count_1 = write_point - (s_vb_dsp_record_buf.out<<1);
        if(byte_count_1 >= (uiCount<<1))
            byte_count_1 = (uiCount<<1);
        s_vb_dsp_record_buf.out =(s_vb_dsp_record_buf.out+  (byte_count_1>>1))%( s_vb_dsp_record_buf.size);
        SCI_TRACE_LOW("[zgt] fuc[%s] out[%d] uiCount[%d] ", __FUNCTION__,s_vb_dsp_record_buf.out<<1,uiCount);
    }else
    {
        byte_count_1 = (s_vb_dsp_record_buf.size<<1)-(s_vb_dsp_record_buf.out<<1) ;
        byte_count_2 = write_point;
        if(byte_count_1 >= (uiCount<<1))
        {
            byte_count_1 = (uiCount<<1);    
            byte_count_2 = 0;
            s_vb_dsp_record_buf.out =(s_vb_dsp_record_buf.out+  (byte_count_1>>1))% (s_vb_dsp_record_buf.size);

        }else
        {
          if( byte_count_2 > ((uiCount<<1)-byte_count_1) )
          {
            byte_count_2 = (uiCount<<1)-byte_count_1;
          }
            s_vb_dsp_record_buf.out =(s_vb_dsp_record_buf.out +  (byte_count_1>>1)
                    +  (byte_count_2>>1))%( s_vb_dsp_record_buf.size);
        }

    }

    DSPVB_Record_SetZspReadPoint(s_vb_dsp_record_buf.out<<1);

}

/*****************************************************************************/
//  Description:  Voice  Record ipc Callback Reg to drv
//  Author:
//  Note:
//****************************************************************************/
void DSPVB_VOICE_REC_Callback (void)
{  
    uint32 write_point = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    if(zsp_callback_framecount >0)
    {
        uint32 i=0;
        s_zsp_read_size=VOICEREC_ZSP_TRANS_SIZE;
        for(i=0; i<zsp_callback_framecount; i++)
        {
            if(PNULL!=VoiceRec_Callback)
            {
                VoiceRec_Callback(s_zsp_read_size);
                DSPVB_Record_MoveDataPointer(s_zsp_read_size);
            }
        }
        //DSPVB_Record_MoveDataPointer(s_zsp_read_size);
    }
}

/*****************************************************************************/
//  Description:    This function is used to start voice record.
//  Author:        guangtao.zhi
//  Note:
//****************************************************************************/
void DSPVB_StartRecord(uint32 uiSampleRate, RECORD_CALLBACK callback_fun)
{
    HAL_ERR_T hal_ret = 0;
    if (!s_b_dspvb_recording)
    {
        AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
        zsp_sharemem->audInPara.readOffset= 0;
        zsp_sharemem->audInPara.writeOffset= 0;

        zsp_sharemem->audInPara.sampleRate= uiSampleRate;
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] uiSampleRate[%d]  ", __FUNCTION__,__LINE__, uiSampleRate );
        //voice record type: pcm:0;amr:1;pcm dump:2;
        zsp_sharemem->voiceRecFormat= 0;
        SCI_MEMSET(&s_vb_dsp_record_buf,0,sizeof(VB_DSP_RECORD_BUF));
        s_vb_dsp_record_buf.data_ptr =(uint16 *)(zsp_sharemem->audInput);
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] data_ptr[0x%x]  ", __FUNCTION__,__LINE__, s_vb_dsp_record_buf.data_ptr );
        SCI_TRACE_LOW("readOffset[%d]  writeOffset[%d]  ", zsp_sharemem->audInPara.readOffset, zsp_sharemem->audInPara.writeOffset );

        //savebuffer=(uint16 *)AUDIO_ALLOC(1000*1024);
        //savebuffer1=(uint16 *)AUDIO_ALLOC(200*1024);
        //savebuffer2=(uint16 *)AUDIO_ALLOC(200*1024);
        
        //savebuffer_share=(uint16 *)AUDIO_ALLOC(100*1024); 
        //zsp_sharemem->audOutPara.bitrate = savebuffer;
       //zsp_sharemem->audOutPara.streamStatus= savebuffer1;
       //SCI_TRACE_LOW("[zgt] savebuffer[0x%x] savebuffer1[0x%x] savebuffer2[0x%x]  ", savebuffer, savebuffer1,savebuffer2);
       //SCI_TRACE_LOW("[zgt] savebuffer[0x%x] ", savebuffer);

        s_vb_dsp_record_buf.in = 0;
        s_vb_dsp_record_buf.out = 0;
        s_vb_dsp_record_buf.size = AUDIO_INPUT_BUF_SIZE>>1;//7k byte

        
        VoiceRec_Callback = callback_fun;
        g_zsp_share_memory_addr = GET_MemoryStartAddressAndLen(MEM_AUDIO_SM_NAME, &g_zsp_share_memory_lenth);


        ipc_register_music_notify(DSPVB_VOICE_REC_Callback);

        //NOTIFY  zsp VOICE START RECORD 
        // hal_ret = hal_zspVoiceRecordStart();
        hal_ret = AUDIO_ZSP_VoiceRecordStart();
        if (0 != hal_ret)
        {
            SCI_TRACE_LOW("[zgt] line[%d] DSPVB_record call hal_zspMusicPlayStart faile", __LINE__);
        }
        s_zsp_read_size = 0;
        s_b_dspvb_recording = SCI_TRUE;
        zsp_callback_framecount = 2;
    }
}
/*****************************************************************************/
//  Description:    This function is used to stop voice record.
//  Author:        guangtao.zhi
//  Note:
//****************************************************************************/
void DSPVB_StopRecord()
{
    if (s_b_dspvb_recording)
    {
        SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);
        VoiceRec_Callback = SCI_NULL;
        if(s_vb_dsp_record_buf.data_ptr)
        {
            s_vb_dsp_record_buf.data_ptr = PNULL;
        }
        //hal_zspVoiceRecordStop();
        AUDIO_ZSP_VoiceRecordStop();
        s_zsp_read_size = 0;
        s_b_dspvb_recording = SCI_FALSE;
        zsp_callback_framecount = 0;
        //SCI_ASSERT(0);
        //SCI_FREE(savebuffer);
    }
}
/*****************************************************************************/
//  Description: thread 
//  Author:
//  Note:           
/*****************************************************************************/
LOCAL void _datadump_cstowcs(uint16 *p_dst, const char *p_src, int i_len)
{
    int i = 0;
    while(i<i_len && p_src[i])
    {
        p_dst[i] = (int16)(p_src[i]);//lint !e571
        i++;
    }
    if (i<i_len)
    {
        p_dst[i] = 0;
    }
}

/*****************************************************************************/
//  Description:  Voice  Record ipc Callback Reg to drv
//  Author:
//  Note:
//****************************************************************************/
void DSPVB_PCM_DUMP_Callback (void)
{  
    uint32 write_point = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);

    if(zsp_callback_framecount >0)
    {
        uint32 i=0;
        xSignalHeader   signal;
        
        DSPVB_Record_ReadData((uint16*)PcmDumpOutBuf, PcmDump_Buffer_lenth/2,&Zsp_ReadDataLen);
        
        signal = SCI_ALLOC (sizeof (*signal));
        signal->SignalCode = PCM_DUMP_WRITE_DATA_TO_FILE_SIG;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();

        SCI_SendSignal (signal, tPcmDumpThreadId);
        signal= PNULL;
        s_zsp_read_size=VOICEREC_ZSP_TRANS_SIZE;
    //send copy data event to thread for write data to file

        DSPVB_Record_MoveDataPointer(s_zsp_read_size*zsp_callback_framecount);
    }
}
/*****************************************************************************/
//  Description:    This function is used to start voice pcm dump.
//  Author:        guangtao.zhi
//  Note:
//****************************************************************************/
void DSPVB_PcmDumpEnable()
{
    HAL_ERR_T hal_ret = 0;
    PCMDUMP_TIME_T    sys_time = {0};
    uint8  time_str[PCMDUMP_NAMESTR_MAX_NUM+1]      = {0};
    char    char_file_name[64]= {0};
    uint16 pcmdump_filename[64] ={0};
    if (!s_dspvb_pcmdumping)
    {
        AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
        zsp_sharemem->audInPara.readOffset= 0;
        zsp_sharemem->audInPara.writeOffset= 0;
        //voice record type: pcm:0;amr:1;pcmdump=2;
        zsp_sharemem->voiceRecFormat= 2;
        SCI_MEMSET(&s_vb_dsp_record_buf,0,sizeof(VB_DSP_RECORD_BUF));
        s_vb_dsp_record_buf.data_ptr =(uint16 *)(zsp_sharemem->audInput);
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] data_ptr[0x%x]  ", __FUNCTION__,__LINE__, s_vb_dsp_record_buf.data_ptr );
        SCI_TRACE_LOW("readOffset[%d]  writeOffset[%d]  ", zsp_sharemem->audInPara.readOffset, zsp_sharemem->audInPara.writeOffset );

        //savebuffer=(uint16 *)AUDIO_ALLOC(1000*1024);
        //savebuffer1=(uint16 *)AUDIO_ALLOC(200*1024);
        //savebuffer2=(uint16 *)AUDIO_ALLOC(200*1024);
        
        //savebuffer_share=(uint16 *)AUDIO_ALLOC(100*1024); 
        //zsp_sharemem->audOutPara.bitrate = savebuffer;
       //zsp_sharemem->audOutPara.streamStatus= savebuffer1;
       //SCI_TRACE_LOW("[zgt] savebuffer[0x%x] savebuffer1[0x%x] savebuffer2[0x%x]  ", savebuffer, savebuffer1,savebuffer2);
       //SCI_TRACE_LOW("[zgt] savebuffer[0x%x] ", savebuffer);

        s_vb_dsp_record_buf.in = 0;
        s_vb_dsp_record_buf.out = 0;
        s_vb_dsp_record_buf.size = AUDIO_INPUT_BUF_SIZE>>1;//7k byte

        g_zsp_share_memory_addr = GET_MemoryStartAddressAndLen(MEM_AUDIO_SM_NAME, &g_zsp_share_memory_lenth);
        s_dspvb_pcmdumping = SCI_TRUE;
       //for 4 channnel dump data 
        zsp_callback_framecount = 6*2;

        PcmDump_Buffer_lenth =zsp_callback_framecount*VOICEREC_ZSP_TRANS_SIZE*2;
        PcmDumpOutBuf = SCI_ALLOCA(PcmDump_Buffer_lenth);
        SCI_MEMSET(PcmDumpOutBuf,0,PcmDump_Buffer_lenth);

        tPcmDumpThreadId = SCI_CreateAppThread(
        "pcm_dump_thread",                               
        "pcm_dump_queue",                            
        Dspvb_PcmDump_Thread_Entry,                    // Entry function of the decode thread
        0,                                // First parameter for thread entry function,
        NULL,                                  // Second parameter for thread entry function,
        1024*2,                  // Size of the thread stack in bytes
        50,                  // Number of messages which can be enqueued
        78,//31,              
        SCI_PREEMPT,                                    
        SCI_AUTO_START
        );
        SCI_ASSERT( SCI_INVALID_BLOCK_ID != tPcmDumpThreadId);

        if(NULL !=g_PcmDumpFileHandle)
        {
            SFS_CloseFile(g_PcmDumpFileHandle);
        }
    // Ê±¼ä

        TM_GetSysTime(&sys_time);   
        //sprintf((char*)time_str,"%02d%02d%02d",sys_time.hour,sys_time.min,sys_time.sec); 
        sprintf(char_file_name, "e:\\PcmDump%02d%02d%02d.pcm", sys_time.hour,sys_time.min,sys_time.sec);
        _datadump_cstowcs(pcmdump_filename, char_file_name, (uint16)strlen(char_file_name));
        SCI_TRACE_LOW("[zgt]pcmdump fileName:%s, pcmdump_filename:%s", char_file_name, pcmdump_filename);

        g_PcmDumpFileHandle = SFS_CreateFile((const uint16 *)pcmdump_filename, SFS_MODE_WRITE|SFS_MODE_CREATE_NEW|SFS_MODE_APPEND, NULL, NULL);    

        ipc_register_music_notify(DSPVB_PCM_DUMP_Callback);

        //NOTIFY  zsp VOICE START pcm dump 
        //hal_ret = hal_zspVoiceRecordStart();
        hal_ret = AUDIO_ZSP_VoiceRecordStart();
        if (0 != hal_ret)
        {
            SCI_TRACE_LOW("[zgt] line[%d] DSPVB_record call hal_zspMusicPlayStart faile", __LINE__);
        }
        s_zsp_read_size = 0;
    }
}
/*****************************************************************************/
//  Description:    This function is used to stop pcm dump.
//  Author:        guangtao.zhi
//  Note:
//****************************************************************************/
void DSPVB_PcmDumpStop()
{
    if (s_dspvb_pcmdumping)
    {
        SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);
        if(s_vb_dsp_record_buf.data_ptr)
        {
            s_vb_dsp_record_buf.data_ptr = PNULL;
        }
        //hal_zspVoiceRecordStop();
        AUDIO_ZSP_VoiceRecordStop();
        s_zsp_read_size = 0;

        zsp_callback_framecount = 0;
        if(PCMDUMP_DEFAULT_TASK_ID != tPcmDumpThreadId)
        {
            SCI_DeleteThread(tPcmDumpThreadId);  // Delete the pcm dump task. 
            tPcmDumpThreadId = PCMDUMP_DEFAULT_TASK_ID;
        }
        SFS_CloseFile(g_PcmDumpFileHandle);
        SCI_FREE(PcmDumpOutBuf);
        //SCI_ASSERT(0);
        //SCI_FREE(savebuffer);
        s_dspvb_pcmdumping = SCI_FALSE;
    }
}
#endif
/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E RECORD_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    char* mode_name = PNULL;
    AUDIO_NV_DSP_RESULT_E result = AUDIO_NV_DSP_NO_ERROR;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_NV_DSP_MODE_INFO_T aud_dsp_mod_param = {0};
    dsp_codec_id = _RECORD_GetDspCodecId(ptDevSetting->eDev_mode); 
    mode_name =  AUDIONVDSP_GetAudioModeName(dsp_codec_id);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("dspvb_as.c RECORD_Play mode:%d.",ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }    
    result=AUDIONVDSP_GetModeParam (mode_name, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param));    
    if(AUDIO_NV_DSP_NO_ERROR!=result)
    {
        SCI_TRACE_LOW("dspvb_as.c RECORD_Play failed to get dev ctr info, result:%d.",result);
        return AUDIO_ERROR;
    }    
        
    if (!s_b_dspvb_playing)
    {
        LAYER1_PreSetAudioEnable(SCI_TRUE);
        if(!(_DSPVB_OpenDev (uiSampleRate, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param),dsp_codec_id)))
        {
            LAYER1_PreSetAudioEnable(SCI_FALSE);
            //SCI_TRACE_LOW:"[RECORD_Play] _DSPVB_OpenDev failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1048_112_2_18_1_2_17_154,(uint8*)"");
            return AUDIO_ERROR;
        }

        RECORD_SetDevMode (ptDevSetting->eDev_mode);
        LAYER1_SetAudioEnable (SCI_TRUE);

        s_b_dspvb_playing = SCI_TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"dspvb_as.c:RECORD_Play: dsp is playing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1062_112_2_18_1_2_17_155,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
}

void MP4RECORD_Callback (void)
{  
#if 1
    //ARMVB_Record_GetZspWritePoint(&write_point);
    //ARMVB_Record_SetZspReadPoint(write_point);
    uint32 write_point = 0;
    uint16 lenth = 0;
    int loop_count = 0;
    SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);
#if 1
    lenth = ARMVB_getRecordLenth();
    
    if(PNULL!= s_mp4record_callback)
    {
	//hch, limit 320
	while( lenth > 0 )
	{
		if( lenth > 320 )
		{
    		    s_mp4record_callback( 320 );
		    SCI_TRACE_LOW("zgt, ARMVB_getRecordLenth lenth lenth >320");
		}
		else
			s_mp4record_callback( lenth );
		lenth = lenth - 320;
		loop_count++;
		if( loop_count >= 4 )
		{ 
			SCI_TRACE_LOW("zgt, lenth is too long , discard the data.");
			break;
		}
	}
    }
    return;
#else
    if(PNULL!= s_mp4record_callback)
    {
       SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);
        s_mp4record_callback(0);
    }
#endif
#else
    uint32 write_point = 0;
    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
       SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);

    if (s_record_callback_count>0)
    {
        uint32 i=0;
        s_ad_read_size=VB_CAPTURE_TRANS_SIZE;
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
    else
    {
        SCI_TRACE_LOW("[zgt]  fuc[%s]  %d", __FUNCTION__,__LINE__);

        ARMVB_Record_GetZspWritePoint(&write_point);
        ARMVB_Record_SetZspReadPoint(write_point);
      s_vb_arm_record_buf.out = write_point/2;
    }
#endif
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E RECORD_Stop (DEV_SETTING_T *ptDevSetting)
{


    if (s_b_dspvb_playing)
    {
        AUDIO_HAL_Close(AUDIO_FUN_DSP_CTL_CODEC | AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP);

        LAYER1_SetAudioEnable (SCI_FALSE);
        s_b_dspvb_playing = SCI_FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"dspvb_as.c:RECORD_Stop: dsp is playing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1085_112_2_18_1_2_17_156,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:        Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E RECORD_Pause (DEV_SETTING_T *ptDevSetting)
{
    if (s_b_dspvb_playing)
    {
        AUDIO_HAL_Close(AUDIO_FUN_DSP_CTL_CODEC | AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP);

        LAYER1_SetAudioEnable (SCI_FALSE);

        s_b_dspvb_playing = SCI_FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"dspvb_as.c:RECORD_Pause: dsp is playing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1107_112_2_18_1_2_17_157,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
}


/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E RECORD_Resume (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    char *mode_name = PNULL;
    AUDIO_NV_DSP_RESULT_E result = AUDIO_NV_DSP_NO_ERROR;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_NV_DSP_MODE_INFO_T aud_dsp_mod_param = {0};

    dsp_codec_id = _RECORD_GetDspCodecId(ptDevSetting->eDev_mode); 
    mode_name =  AUDIONVDSP_GetAudioModeName(dsp_codec_id);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("dspvb_as.c RECORD_Resume mode:%d.",ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }    
    result=AUDIONVDSP_GetModeParam (mode_name, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param));    
    if(AUDIO_NV_DSP_NO_ERROR!=result)
    {
        SCI_TRACE_LOW("dspvb_as.c RECORD_Resume failed to get dev ctr info, result:%d.",result);
        return AUDIO_ERROR;
    }    
    
    if (!s_b_dspvb_playing)
    {
        LAYER1_PreSetAudioEnable(SCI_TRUE);
        if(!(_DSPVB_OpenDev (uiSampleRate, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param),dsp_codec_id )))
        {
            LAYER1_PreSetAudioEnable(SCI_FALSE);
            //SCI_TRACE_LOW:"[RECORD_Resume] _DSPVB_OpenDev failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1126_112_2_18_1_2_17_158,(uint8*)"");
            return AUDIO_ERROR;
        }

        RECORD_SetDevMode (ptDevSetting->eDev_mode);
        LAYER1_SetAudioEnable (SCI_TRUE);

        s_b_dspvb_playing = SCI_TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"dspvb_as.c:RECORD_Resume: dsp is playing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1141_112_2_18_1_2_17_159,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E MP4RECORD_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
#if 1
        SND_ITF_T cur_itf = SND_ITF_NONE;
        HAL_AIF_STREAM_T mic_stream;
        AUD_LEVEL_T aud_level_cfg;
        AUD_ERR_T ret = 0;
        HAL_ERR_T hal_ret = 0;
        AUD_DEVICE_CFG_EXT_T devicecfg;

        AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
        SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
        SCI_TRACE_LOW("{xx} MP4RECORD_Play SCI_GetSemaphore....");
        if(SCI_SUCCESS != SCI_GetSemaphore(audio_armvb_semaphore, 10))
        {
            SCI_TRACE_LOW("{xx} MP4RECORD_Play SCI_GetSemaphore return");
            return AUDIO_CONFLICT;
        }
        SCI_TRACE_LOW("{xx} MP4RECORD_Play SCI_GetSemaphore OK");
        SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);

        if (!s_b_armvb_recording)
        {    
    //!!!can not memset all of zsp sharemem to zero. some status value will be keep with zsp!!! sheen
       // if do not init this para will result in assert but  it should not initialize here for zsp used !zgt
       zsp_sharemem->updateParaInd=0;//sheen

        //if (ptDevSetting->eDev_mode != 0)
    {
        cur_itf = SND_ITF_LOUD_SPEAKER;
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
#if 0
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
        SCI_TRACE_LOW("line[%d] ARMVB_Record_Play ptDevSetting->eChannelChoice:%d", __LINE__, ptDevSetting->eChannelChoice);
    }
#endif
        //init s_vb_arm_record_buf
        SCI_MEMSET(&s_vb_arm_record_buf,0,sizeof(VB_ARM_RECORD_BUF));
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
        zsp_sharemem->audInPara.sbcOutFlag =0;//sheen
#endif
        g_zsp_para.musicMode = 5;
        g_zsp_para.sbcOutFlag = 0;
        zsp_sharemem->audInPara.readOffset= 0;
        zsp_sharemem->audInPara.writeOffset= 0;
        zsp_sharemem->audInPara.fileEndFlag = 0;
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] inchannelNb[%d]  ", __FUNCTION__,__LINE__,zsp_sharemem->audInPara.channelNb);
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] outchannelNb[%d]  ", __FUNCTION__,__LINE__,zsp_sharemem->audOutPara.channelNb);
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] bitsPerSample[%d]  ", __FUNCTION__,__LINE__,zsp_sharemem->audInPara.bitsPerSample);
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] musicMode[%d]  ", __FUNCTION__,__LINE__,zsp_sharemem->musicMode);
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] readOffset[%d]  ", __FUNCTION__,__LINE__,zsp_sharemem->audInPara.readOffset);

        //init mic_Stream
        if(8000 ==  zsp_sharemem->audInPara.sampleRate)
        {
            mic_stream.startAddress = (zsp_sharemem->txPCM.txPcmBuffer.pcmBuf); 
            mic_stream.length = sizeof(HAL_SPEECH_PCM_BUF_T);     
        }
        else
        {
            mic_stream.startAddress = (zsp_sharemem->txPCM.txPcmVolte.pcmBuf); 
            mic_stream.length = sizeof(HAL_VOLTE_PCM_BUF_T);
        }

        mic_stream.sampleRate = uiSampleRate;
        mic_stream.channelNb = HAL_AIF_MONO;
        mic_stream.voiceQuality = FALSE;
        mic_stream.playSyncWithRecord = 0;
        mic_stream.halfHandler = NULL;
        mic_stream.endHandler = NULL;
        SCI_TRACE_LOW("[zgt] fuc[%s] line[%d] sampleRate[%d]  ", __FUNCTION__,__LINE__, mic_stream.sampleRate );

        //init cfg 
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
        ipc_register_music_notify(MP4RECORD_Callback); // 

#if 0//ndef DSPVB_USE_CP_AUDIO_INTERFACE
    //ipc_register_music_notify(MP4RECORD_Callback); // TODO: LJF CAREMA
#ifdef DSPVB_USE_PCIM_26M_CLOCK
	//PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
	AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_RECORD);
#endif
         //call zsp 
#ifdef DSPVB_USE_DM_TONE_INTERFACE
    DM_ZspMusicPlayStart();
#else
    hal_ret = hal_zspMusicPlayStart();
    if (0 != hal_ret)
    {
        SCI_TRACE_LOW("[zgt] line[%d] ARMVB_record call hal_zspMusicPlayStart faile", __LINE__);
    }
#endif
        //start record stream
#ifndef DSPVB_USE_DM_TONE_INTERFACE
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
    ret =AUDIO_CP_START(AUDIO_DEV_MP4RECORD,cur_itf, &mic_stream, &aud_level_cfg,0,devicecfg,0, &g_zsp_para);
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
#else
    char *mode_name = PNULL;
    AUDIO_NV_DSP_RESULT_E result = AUDIO_NV_DSP_NO_ERROR;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_NV_DSP_MODE_INFO_T aud_dsp_mod_param = {0};
    
    dsp_codec_id = _MP4RECORD_GetDspCodecId(ptDevSetting->eDev_mode); 
    mode_name =  AUDIONVDSP_GetAudioModeName(dsp_codec_id);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("dspvb_as.c MP4RECORD_Play mode:%d.",ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }    
    result=AUDIONVDSP_GetModeParam (mode_name, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param));    
    if(AUDIO_NV_DSP_NO_ERROR!=result)
    {
        SCI_TRACE_LOW("dspvb_as.c MP4RECORD_Play failed to get dev ctr info, result:%d.",result);
        return AUDIO_ERROR;
    }    

    if (!s_b_dspvb_playing)
    {
         LAYER1_PreSetAudioEnable(SCI_TRUE);
        if(!(_DSPVB_OpenDev (uiSampleRate, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param),dsp_codec_id)))
        {
            LAYER1_PreSetAudioEnable(SCI_FALSE);
            //SCI_TRACE_LOW:"[MP4RECORD_Play] _DSPVB_OpenDev failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1159_112_2_18_1_2_17_160,(uint8*)"");
            return AUDIO_ERROR;
        }

        MP4RECORD_SetDevMode (ptDevSetting->eDev_mode);
        LAYER1_SetAudioEnable (SCI_TRUE);
        DSPVB_SystemDelay (100); //delay 100 tickcount to solve cr142121

        s_b_dspvb_playing = SCI_TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"dspvb_as.c:MP4RECORD_Play: dsp is playing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1175_112_2_18_1_2_17_161,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
#endif
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E MP4RECORD_Stop (DEV_SETTING_T *ptDevSetting)
{
#if 1
        SND_ITF_T cur_itf = SND_ITF_NONE;
        AUD_ERR_T ret = 0;
        HAL_ERR_T hal_ret = 0;
        SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);
        if (s_b_armvb_recording)
        {
            //if (ptDevSetting->eDev_mode != 0)
            {
                cur_itf = SND_ITF_LOUD_SPEAKER;
            }
            //SCI_ASSERT(0);
#if 0//ndef DSPVB_USE_CP_AUDIO_INTERFACE            
#ifdef DSPVB_USE_DM_TONE_INTERFACE
            ret = DM_AudStreamRecordStop(cur_itf);
            if (0 != ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] ARMVB_Record_Stop call DM_AudStreamRecordStop faile", __LINE__);
            }
#else
            ret = aud_StreamRecordStop(cur_itf);
            if (0 != ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] ARMVB_Record_Stop call aud_StreamStop faile", __LINE__);
            }
#endif
#ifdef DSPVB_USE_DM_TONE_INTERFACE
            DM_ZspMusicPlayStop();
#else
            hal_ret = hal_zspMusicPlayStop();
            if (0 != hal_ret)
            {
                SCI_TRACE_LOW("[zgt] line[%d] ARMVB_Record_Stop call hal_zspMusicPlayStop faile", __LINE__);
            }
#endif
#ifdef DSPVB_USE_PCIM_26M_CLOCK
		//PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
            AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_RECORD);
#endif
#else
            g_zsp_para.musicMode = 5;
            g_zsp_para.sbcOutFlag = 0;
            ipc_register_music_notify(SCI_NULL);
            ret = AUDIO_CP_STOP(AUDIO_DEV_MP4RECORD,cur_itf, &g_zsp_para);
            if (AUD_ERR_NO != ret)
            {
                SCI_TRACE_LOW("zgt line[%d] mp4record_Stop call AUDIO_CP_STOP faile ret:0x%x", __LINE__, ret);
                return ret;
            }
#endif
            if(s_vb_arm_record_buf.data_ptr)
            {
                s_vb_arm_record_buf.data_ptr = PNULL;
            }
            s_ad_read_size = 0;
            s_record_callback_count = 0;
            s_b_armvb_recording = SCI_FALSE;
        }
    SCI_TRACE_LOW("{xx} MP4RECORD_Stop SCI_PutSemaphore....");
    SCI_PutSemaphore(audio_armvb_semaphore);
    SCI_TRACE_LOW("{xx} MP4RECORD_Stop SCI_PutSemaphore OK");
    SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    return AUDIO_NO_ERROR;
#else

    if (s_b_dspvb_playing)
    {
//        AUDDEV_Close (AUDDEV_FUNC_DSP_CTL, SCI_FALSE);
        AUDIO_HAL_Close(AUDIO_FUN_DSP_CTL_CODEC | AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP);

        LAYER1_SetAudioEnable (SCI_FALSE);

        s_b_dspvb_playing = SCI_FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"dspvb_as.c:MP4RECORD_Stop: dsp is playing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1200_112_2_18_1_2_17_162,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
#endif
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:        Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E MP4RECORD_Pause (DEV_SETTING_T *ptDevSetting)
{
#if 1
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return AUDIO_NO_ERROR;
#else
    if (s_b_dspvb_playing)
    {
//        AUDDEV_Close (AUDDEV_FUNC_DSP_CTL, SCI_FALSE);
        AUDIO_HAL_Close(AUDIO_FUN_DSP_CTL_CODEC | AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP);


        LAYER1_SetAudioEnable (SCI_FALSE);

        s_b_dspvb_playing = SCI_FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"dspvb_as.c:MP4RECORD_Pause: dsp is playing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1225_112_2_18_1_2_17_163,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
#endif
}


/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E MP4RECORD_Resume (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
#if 1
    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return AUDIO_NO_ERROR;
#else
    char *mode_name=PNULL;
    AUDIO_NV_DSP_RESULT_E result = AUDIO_NV_DSP_NO_ERROR;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_NV_DSP_MODE_INFO_T aud_dsp_mod_param = {0};
    dsp_codec_id = _MP4RECORD_GetDspCodecId(ptDevSetting->eDev_mode); 
    mode_name =  AUDIONVDSP_GetAudioModeName(dsp_codec_id);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("dspvb_as.c MP4RECORD_Resume mode:%d.",ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }    
    result=AUDIONVDSP_GetModeParam (mode_name, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param));    
    if(AUDIO_NV_DSP_NO_ERROR!=result)
    {
        SCI_TRACE_LOW("dspvb_as.c MP4RECORD_Resume failed to get dev ctr info, result:%d.",result);
        return AUDIO_ERROR;
    }    

    if (!s_b_dspvb_playing)
    {
       LAYER1_PreSetAudioEnable(SCI_TRUE);
        if(!(_DSPVB_OpenDev (uiSampleRate, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param),dsp_codec_id)))
        {
            LAYER1_PreSetAudioEnable(SCI_FALSE);
            //SCI_TRACE_LOW:"[MP4RECORD_Resume] _DSPVB_OpenDev failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1246_112_2_18_1_2_17_164,(uint8*)"");
            return AUDIO_ERROR;
        }

        MP4RECORD_SetDevMode (ptDevSetting->eDev_mode);
        LAYER1_SetAudioEnable (SCI_TRUE);

        DSPVB_SystemDelay (100); //delay 100 tickcount to solve cr142121

        s_b_dspvb_playing = SCI_TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"dspvb_as.c:MP4RECORD_Resume: dsp is playing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1261_112_2_18_1_2_17_165,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
#endif
}
/*****************************************************************************/
//  Description:    MP4RECORD logic device reg Rec callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL uint32 MP4RECORD_RegRecCallback (
    ADM_RECCALLBACK pCallback
)
{
    int32 i = 0;
    
    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);
    SCI_PASSERT((pCallback != SCI_NULL), /*assert verified*/
        ("MP4RECORD_RegRecCallback: Input Parameter(pCallback) should not be NULL!")); 

    if(s_mp4record_callback == SCI_NULL)
    {    
        SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);
        s_mp4record_callback = pCallback;
        return (uint32) s_mp4record_callback;
    }
    return (uint32)SCI_NULL;
}

/*****************************************************************************/
//  Description:   MP4RECORD logic device unreg Rec callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL void MP4RECORD_UnRegRecCallback (
    uint32 callback_vector
)
{
    SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);

    if((uint32)s_mp4record_callback == callback_vector)
    {
        SCI_TRACE_LOW("[zgt] file[%s] fuc[%s] line[%d] ", __FILE__,__FUNCTION__,__LINE__);
        s_mp4record_callback = SCI_NULL;
    }
     return;
}
LOCAL void MP4RECORD_ReadData(uint16 *puiDestData, uint32 uiCount, uint32 *puiDataLen)
{  

    AUD_ZSP_SHAREMEM_T *zsp_sharemem = (AUD_ZSP_SHAREMEM_T *)g_zsp_share_memory_addr;
    UINT16 writeoffset = zsp_sharemem->audInPara.writeOffset;
    UINT16 readoffset = zsp_sharemem->audInPara.readOffset;
    UINT16 lenth = 0;
    lenth = uiCount*2;
    SCI_TRACE_LOW("[zgt]  fuc[%s] readoffset[%d] writeoffset[%d] ", __FUNCTION__,readoffset,writeoffset);
    if (readoffset < writeoffset)
    {
        //SCI_TRACE_LOW("[ljf] line[%d] func[%s] lenth:%d", __LINE__, __FUNCTION__, lenth);
        //SCI_MEMSET(ptPara->source_buf_ptr, 0xDD, lenth);
        SCI_MEMCPY(puiDestData, &(zsp_sharemem->audInput[readoffset / 2]), lenth);
    }
    else if (readoffset >= writeoffset)
    {
        if ((readoffset + lenth) < zsp_sharemem->audInPara.inLenth)
        {
            //SCI_TRACE_LOW("[ljf] line[%d] func[%s] lenth:%d", __LINE__, __FUNCTION__, lenth);
            //SCI_MEMSET(ptPara->source_buf_ptr, 0xDD, lenth);
            SCI_MEMCPY(puiDestData, &(zsp_sharemem->audInput[readoffset / 2]), lenth);
        }
        else
        {
            //SCI_TRACE_LOW("[ljf] line[%d] func[%s] lenth:[%d][%d]", __LINE__, __FUNCTION__, ((zsp_sharemem->audInPara.inLenth - readoffset)), ((lenth - (zsp_sharemem->audInPara.inLenth - readoffset))));
            //SCI_MEMSET(ptPara->source_buf_ptr, 0xDD, lenth);
            SCI_MEMCPY(puiDestData, &(zsp_sharemem->audInput[readoffset / 2]), (zsp_sharemem->audInPara.inLenth - readoffset));
            SCI_MEMCPY(&(puiDestData[(zsp_sharemem->audInPara.inLenth - readoffset) / 2]), &(zsp_sharemem->audInput[0]), (lenth - (zsp_sharemem->audInPara.inLenth - readoffset)));
        }
    }
    // move readoffset
    zsp_sharemem->audInPara.readOffset = (zsp_sharemem->audInPara.readOffset + lenth) % zsp_sharemem->audInPara.inLenth;
    *puiDataLen = lenth;
    SCI_TRACE_LOW("[zgt]  fuc[%s] addr[0x%x]uiCount[%d] puiDataLen[%d] ", __FUNCTION__,puiDestData,uiCount,*puiDataLen);

    return;
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E LOOPBACK_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ERR_T aud_ret = AUD_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    SND_ITF_T snd_cur_itf = SND_ITF_NONE;
    VOIS_AUDIO_CFG_T audio_cfg;
    AUD_LEVEL_T aud_level_cfg;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    HAL_AIF_STREAM_T hal_aif_stream;
	
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d] uiSampleRate:%d ptDevSetting->eDev_mode:0x%x", __FILE__, __FUNCTION__, __LINE__, uiSampleRate, ptDevSetting->eDev_mode);
    SCI_TRACE_LOW("{xx} LOOPBACK_Play SCI_GetSemaphore....");
    SCI_GetSemaphore(audio_armvb_semaphore, SCI_WAIT_FOREVER);
    SCI_TRACE_LOW("{xx} LOOPBACK_Play SCI_GetSemaphore OK");
	
    switch(ptDevSetting->eDev_mode)
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
        SCI_TRACE_LOW("[ljf] line[%d] LOOPBACK_Play eDev_mode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
        break;
    }
    audio_cfg.spkLevel = (AUD_SPK_LEVEL_T)ptDevSetting->uiVol;
    audio_cfg.micLevel = AUD_MIC_ENABLE;
    audio_cfg.sideLevel = AUD_SIDE_MUTE;
    audio_cfg.toneLevel = AUD_TONE_0dB;
    audio_cfg.encMute = 0;
    audio_cfg.decMute = 0;
    //
    if (AUDIO_DEVICE_MODE_EARPHONE == ptDevSetting->eDev_mode)
    {
        device_cfg_ext.inputType = AUD_INPUT_TYPE_HPMIC_USE_L;
    }
    else
    {
        device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    }
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
    aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)ptDevSetting->uiVol;
    aud_level_cfg.micLevel = SND_MIC_ENABLE;
    aud_level_cfg.sideLevel = SND_SIDE_MUTE;
    aud_level_cfg.toneLevel = SND_TONE_0DB;
    aud_level_cfg.appMode = SND_APP_MODE_VOICE;
    //
    //PMIC_26M_open(PMIC_26M_USED_BY_AUDIO);
    AUDIO_PMIC_26M_open(AUDIO_26M_USED_BY_LocalTest);

    vois_ret = AUDIO_CP_VoisSetCfg(cur_itf, &audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] LOOPBACK_Play AUDIO_CP_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
    aud_ret = AUDIO_CP_TestModeSet(snd_cur_itf, &hal_aif_stream, &aud_level_cfg, AUD_TEST_SIDE_TEST, AUD_TEST_VOICE_MODE_NB);
    if (VOIS_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] LOOPBACK_Play AUDIO_CP_TestModeSet ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    This function is used to get noise-restrain process state.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E LOOPBACK_Stop (DEV_SETTING_T *ptDevSetting)
{
    VOIS_ERR_T vois_ret = VOIS_ERR_NO;
    AUD_ERR_T aud_ret = AUD_ERR_NO;
    AUD_ITF_T cur_itf = AUD_ITF_NONE;
    SND_ITF_T snd_cur_itf = SND_ITF_NONE;
    VOIS_AUDIO_CFG_T audio_cfg;
    AUD_LEVEL_T aud_level_cfg;
    AUD_DEVICE_CFG_EXT_T device_cfg_ext;
    HAL_AIF_STREAM_T hal_aif_stream;
	
    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d] ptDevSetting->eDev_mode:0x%x", __FILE__, __FUNCTION__, __LINE__, ptDevSetting->eDev_mode);
	
    switch(ptDevSetting->eDev_mode)
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
        SCI_TRACE_LOW("[ljf] line[%d] LOOPBACK_Play eDev_mode is [0x%x] WRONG!!!", __LINE__, ptDevSetting->eDev_mode);
        break;
    }
    audio_cfg.spkLevel = (AUD_SPK_LEVEL_T)ptDevSetting->uiVol;
    audio_cfg.micLevel = AUD_MIC_ENABLE;
    audio_cfg.sideLevel = AUD_SIDE_MUTE;
    audio_cfg.toneLevel = AUD_TONE_0dB;
    audio_cfg.encMute = 0;
    audio_cfg.decMute = 0;
    //
    if (AUDIO_DEVICE_MODE_EARPHONE == ptDevSetting->eDev_mode)
    {
        device_cfg_ext.inputType = AUD_INPUT_TYPE_HPMIC_USE_L;
    }
    else
    {
        device_cfg_ext.inputType = AUD_INPUT_TYPE_MAINMIC;
    }
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
    aud_level_cfg.spkLevel = (SND_SPK_LEVEL_T)ptDevSetting->uiVol;
    aud_level_cfg.micLevel = SND_MIC_ENABLE;
    aud_level_cfg.sideLevel = SND_SIDE_MUTE;
    aud_level_cfg.toneLevel = SND_TONE_0DB;
    aud_level_cfg.appMode = SND_APP_MODE_VOICE;
    //
    vois_ret = AUDIO_CP_VoisSetCfg(cur_itf, &audio_cfg, device_cfg_ext, g_btworkmode);
    if (VOIS_ERR_NO != vois_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] LOOPBACK_Stop AUDIO_CP_VoisSetCfg ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
    aud_ret = AUDIO_CP_TestModeSet(snd_cur_itf, &hal_aif_stream, &aud_level_cfg, AUD_TEST_NO, AUD_TEST_VOICE_MODE_NB);
    if (VOIS_ERR_NO != aud_ret)
    {
        SCI_TRACE_LOW("[ljf] line[%d] LOOPBACK_Stop AUDIO_CP_TestModeSet ret is [0x%x] WRONG!!!", __LINE__, vois_ret);
    }
    //PMIC_26M_close(PMIC_26M_USED_BY_AUDIO);
    AUDIO_PMIC_26M_close(AUDIO_26M_USED_BY_LocalTest);
    SCI_TRACE_LOW("{xx} LOOPBACK_Stop SCI_PutSemaphore....");
    SCI_PutSemaphore(audio_armvb_semaphore);
    SCI_TRACE_LOW("{xx} LOOPBACK_Stop SCI_PutSemaphore OK");
    return AUDIO_NO_ERROR;
}
#define TMP_FIFO_SIZE        (160)

/*****************************************************************************/
//  Description:    This function is used to register LAYER1 logical VB device.
//  Author:         Johnson.sun
//  Note:  used for calling condition.
//****************************************************************************/
PUBLIC HAUDIODEV LAYER1_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("LAYER1"), strlen ("LAYER1") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = LAYER1_Play;
    tDeviceInfo.tAudioDevOpe.pClose = LAYER1_Stop;
    tDeviceInfo.tAudioDevOpe.pPreClose = Dspvb_PreClose;
    tDeviceInfo.tAudioDevOpe.pPause = LAYER1_Pause;
    tDeviceInfo.tAudioDevOpe.pResume = LAYER1_Resume;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = _LAYER1_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetExtraVolume= DSPVB_SetExtraVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = LAYER1_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = DSPVB_SetUplinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = LAYER1_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkLocalNetMute= LAYER1_SetDownlinkLocalNetMute;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate =
        DSPVB_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    if(SCI_NULL == audio_armvb_semaphore)
    {
        audio_armvb_semaphore = SCI_CreateSemaphore("ARMVB_AUDIO_SEMAPHORE",1);
    }

    SCI_TRACE_LOW("[ljf] line[%d] file[%s] func[%s]", __LINE__, __FILE__, __FUNCTION__);
    return hDevice;
}

/*****************************************************************************/
//  Description:    This function is used to register RECORD logical VB device.
//  Author:         Johnson.sun
//  Note:   used for general record condition.
//****************************************************************************/
PUBLIC HAUDIODEV RECORD_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("RECORD"), strlen ("RECORD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = RECORD_Play;
    tDeviceInfo.tAudioDevOpe.pClose = RECORD_Stop;
    tDeviceInfo.tAudioDevOpe.pPreClose = Dspvb_PreClose;
    tDeviceInfo.tAudioDevOpe.pPause = RECORD_Pause;
    tDeviceInfo.tAudioDevOpe.pResume = RECORD_Resume;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = _RECORD_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = RECORD_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = DSPVB_SetUplinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = RECORD_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate =
        DSPVB_Record_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}

/*****************************************************************************/
//  Description:    This function is used to register MP4RECORD logical VB device.
//  Author:         Johnson.sun
//  Note:  used for mp4 photograph condition.
//****************************************************************************/
PUBLIC HAUDIODEV MP4RECORD_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("MP4RECORD"), strlen ("MP4RECORD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = MP4RECORD_Play;
    tDeviceInfo.tAudioDevOpe.pClose = MP4RECORD_Stop;
    tDeviceInfo.tAudioDevOpe.pPreClose = Dspvb_PreClose;
    tDeviceInfo.tAudioDevOpe.pPause = MP4RECORD_Pause;
    tDeviceInfo.tAudioDevOpe.pResume = MP4RECORD_Resume;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = _MP4RECORD_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = DSPVB_SetUplinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = MP4RECORD_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = MP4RECORD_SetDownlinkMute;
        tDeviceInfo.tAudioDevOpe.pRegRecCallback = MP4RECORD_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = MP4RECORD_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = MP4RECORD_ReadData;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate =
        DSPVB_Record_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}

/*****************************************************************************/
//  Description:    This function is used to register LOOPBACK logical VB device.
//  Author:         
//  Note:  used for calling condition in loopback.
//****************************************************************************/
PUBLIC HAUDIODEV LOOPBACK_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("[ljf] file[%s] func[%s] line[%d]", __FILE__, __FUNCTION__, __LINE__);
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("LOOPBACKDEV"), strlen ("LAYER1") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = LOOPBACK_Play;
    tDeviceInfo.tAudioDevOpe.pClose = LOOPBACK_Stop;
    tDeviceInfo.tAudioDevOpe.pPreClose = Dspvb_PreClose;
    tDeviceInfo.tAudioDevOpe.pPause = LOOPBACK_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = LOOPBACK_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = _LOOPBACK_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = LOOPBACK_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = DSPVB_SetUplinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = LOOPBACK_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate =
        DSPVB_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}

/*****************************************************************************/
//  Description:    This function is used to open device used to get dsp data.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E DSPDATA_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    char *mode_name=PNULL;
    AUDIO_NV_DSP_RESULT_E result = AUDIO_NV_DSP_NO_ERROR;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_NV_DSP_MODE_INFO_T aud_dsp_mod_param = {0};
    dsp_codec_id = _LAYER1_GetDspCodecId(ptDevSetting->eDev_mode); 

    //SCI_TRACE_LOW:"[DSPDATA_Play]:s_b_dspvb_playing:%d,s_b_dspdata_playing:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1490_112_2_18_1_2_18_172,(uint8*)"dd",s_b_dspvb_playing,s_b_dspdata_playing);
    mode_name =  AUDIONVDSP_GetAudioModeName(dsp_codec_id);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("dspvb_as.c DSPDATA_Play mode:%d.",ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }    
    result=AUDIONVDSP_GetModeParam (mode_name, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param));    
    if(AUDIO_NV_DSP_NO_ERROR!=result)
    {
        SCI_TRACE_LOW("dspvb_as.c DSPDATA_Play failed to get dev ctr info, result:%d.",result);
        return AUDIO_ERROR;
    }    

    if (!s_b_dspvb_playing)
    {
        LAYER1_PreSetAudioEnable(SCI_TRUE);
        if(!(_DSPVB_OpenDev (uiSampleRate, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param), dsp_codec_id)))
        {
            LAYER1_PreSetAudioEnable(SCI_FALSE);
            //SCI_TRACE_LOW:"[DSPDATA_Play] _DSPVB_OpenDev failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1498_112_2_18_1_2_18_173,(uint8*)"");
            return AUDIO_ERROR;
        }
        LAYER1_SetDevMode (ptDevSetting->eDev_mode);
         DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);
        s_b_dspvb_playing = SCI_TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[DSPDATA_Play]: dsp vb is already opened by other device!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1509_112_2_18_1_2_18_174,(uint8*)"");
    }

    s_b_dspdata_playing = SCI_TRUE;

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    This function is used to stop device used to get dsp data.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E DSPDATA_Stop (DEV_SETTING_T *ptDevSetting)
{
    //SCI_TRACE_LOW:"[DSPDATA_Stop]:s_b_dspvb_playing:%d,s_b_dspdata_playing:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1524_112_2_18_1_2_18_175,(uint8*)"dd",s_b_dspvb_playing,s_b_dspdata_playing);

    if (s_b_dspvb_playing)
    {
        if (!s_b_layer1_playing) //layer1 is stopped
        {

            AUDIO_HAL_Close(AUDIO_FUN_DSP_CTL_CODEC | AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP);
            s_b_dspvb_playing = SCI_FALSE;
        }
        else
        {
            //SCI_TRACE_LOW:"[DSPDATA_Stop]: layer1 is using vb,so we should not close vb!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1536_112_2_18_1_2_18_176,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[DSPDATA_Stop]: stop exception!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1541_112_2_18_1_2_18_177,(uint8*)"");
    }

    s_b_dspdata_playing = SCI_FALSE;

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    This function is used to pause device used to get dsp data.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E DSPDATA_Pause (DEV_SETTING_T *ptDevSetting)
{
    if (s_b_dspvb_playing)
    {
        if (!s_b_layer1_playing) //layer1 is stopped
        {
            AUDIO_HAL_Close(AUDIO_FUN_DSP_CTL_CODEC | AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE | AUDIO_FUN_DSP);
            s_b_dspvb_playing = SCI_FALSE;
        }
        else
        {
            //SCI_TRACE_LOW:"[DSPDATA_Pause]: layer1 is using vb,so we should not close vb!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1564_112_2_18_1_2_18_178,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[DSPDATA_Pause]: pause exception!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1569_112_2_18_1_2_18_179,(uint8*)"");
    }

    s_b_dspdata_playing = SCI_FALSE;

    return AUDIO_NO_ERROR;

}

/*****************************************************************************/
//  Description:    This function is used to resume device used to get dsp data.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E DSPDATA_Resume (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    char *mode_name=PNULL;
    AUDIO_NV_DSP_RESULT_E result = AUDIO_NV_DSP_NO_ERROR;
    NV_AUDIO_DSP_CODEC_E dsp_codec_id = (NV_AUDIO_DSP_CODEC_E)0;
    AUDIO_NV_DSP_MODE_INFO_T aud_dsp_mod_param = {0};
    dsp_codec_id = _LAYER1_GetDspCodecId(ptDevSetting->eDev_mode); 
    mode_name =  AUDIONVDSP_GetAudioModeName(dsp_codec_id);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("dspvb_as.c DSPDATA_Resume mode:%d.",ptDevSetting->eDev_mode);
        return AUDIO_PARAM_ERROR;
    }    
    result=AUDIONVDSP_GetModeParam (mode_name, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param));    
    if(AUDIO_NV_DSP_NO_ERROR!=result)
    {
        SCI_TRACE_LOW("dspvb_as.c DSPDATA_Resume failed to get dev ctr info, result:%d.",result);
        return AUDIO_ERROR;
    }    
        
    if (!s_b_dspvb_playing)
    {
        if(!(_DSPVB_OpenDev (uiSampleRate, (AUDIO_NV_DSP_MODE_INFO_T *)(&aud_dsp_mod_param), dsp_codec_id)))
        {
            //SCI_TRACE_LOW:"[DSPDATA_Resume] _DSPVB_OpenDev failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1588_112_2_18_1_2_18_180,(uint8*)"");
            return AUDIO_ERROR;
        }
        
        LAYER1_SetDevMode (ptDevSetting->eDev_mode);
         DSPVB_EnableAmplifier(SCI_FALSE, dsp_codec_id);

        s_b_dspvb_playing = SCI_TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[DSPDATA_Play]: dsp vb is already opened by other device!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DSPVB_AS_1600_112_2_18_1_2_18_181,(uint8*)"");
    }

    s_b_dspdata_playing = SCI_TRUE;

    return AUDIO_NO_ERROR;

}

/*****************************************************************************/
//  Description:    This function is used to reg device used to get dsp data.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV DSPDATA_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("DSPDATA"), strlen ("DSPDATA") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.tAudioDevOpe.pOpen = DSPDATA_Play;
    tDeviceInfo.tAudioDevOpe.pClose = DSPDATA_Stop;
    tDeviceInfo.tAudioDevOpe.pPreClose = Dspvb_PreClose;
    tDeviceInfo.tAudioDevOpe.pPause = DSPDATA_Pause;
    tDeviceInfo.tAudioDevOpe.pResume = DSPDATA_Resume;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetVolume = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate =
        DSPVB_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}

#ifdef   __cplusplus
}
#endif  // End of dspvb_as.c




