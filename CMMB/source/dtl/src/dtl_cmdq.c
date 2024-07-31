/******************************************************************************
 ** File Name:      dtl_cmdq.c                                                *
 ** Author:         Hanbing.Zhang                                             *
 ** DATE:           6/11/2007                                                 *
 ** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the command interface  of data transfer *
 **                 layer                                                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME                 DESCRIPTION                           *
 ** 6/11/2007      Hanbing.Zhang        Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "dtl_cmdq.h"
#include "os_api.h"
#include "kd.h"
#include "bits.h"

#ifdef MTV_MIDDLEWARE_ON_BB
#include "demod.h"
#include "cmmbdmx_if.h"
#include "bits.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define DTLCMD_DWORD_LOW  0X0000FFFF
#define DTLCMD_DWORD_HIGH 0XFFFF0000

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Play(void)
* \param  void
* \return KDboolean 
* \brief  send audio play command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Play(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_AUDIO_PLAY;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Pcm_Mix(void)
* \param  void
* \return KDboolean 
* \brief  send audio pcm mix command
*
* \author Wangyq
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Pcm_Mix(KDboolean isMix)
{
    DTL_CMDQ_CMD_T cmd = {0};
    
   cmd.cmd_id      = DSP_CMDQ_AUDIO_PCM_MIX;			

   if (SCI_TRUE == isMix )
   {
   	 cmd.parameter0  = 1;
   }		
   else
   {
   	cmd.parameter0  = 0;
   }
	
    return DTL_SendCMD(&cmd);
}


/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Stop(void)
* \param  void
* \return KDboolean 
* \brief  send audio stop command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Stop(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_AUDIO_STOP;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Set_Output_Param(
*                                          AUDIO_SET_OUTPUT_PARAM_U *audio_param_ptr
*                                          )
* \param  audio_param_ptr
* \return KDboolean 
* \brief  send audio output param
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Set_Output_Param(
                                         AUDIO_SET_OUTPUT_PARAM_U *audio_param_ptr
                                         )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id      = DSP_CMDQ_AUDIO_SET_PARAM;
    cmd.parameter0  = audio_param_ptr->dwValue.param1;
    cmd.parameter1  = audio_param_ptr->dwValue.param2;
    cmd.parameter1  = (cmd.parameter1 << 16) & DTLCMD_DWORD_HIGH;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Pause(void)
* \param  void
* \return KDboolean 
* \brief  send audio pause command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Pause(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_AUDIO_PAUSE;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Resume(void)
* \param  void
* \return KDboolean 
* \brief  send audio resume command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Resume(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_AUDIO_RESUME;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Data_Request( void )
* \param  void
* \return KDboolean 
* \brief  send audio data request
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Data_Request( void )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id      = DSP_CMDQ_AUDIO_DATA_REQUEST;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Channel_Change(void)
* \param  void
* \return KDboolean 
* \brief  audio channel change
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Channel_Change(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_AUDIO_CHANNEL_CHANGE;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Mute(void)
* \param  void
* \return BOOLEAN 
* \brief  iis mute
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Mute(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = BB_ARM_CMDQ_AUDIO_MUTE;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Unmute(void)
* \param  void
* \return BOOLEAN 
* \brief  iis unmute
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Unmute(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = BB_ARM_CMDQ_AUDIO_UNMUTE;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Set_Volume(
*                                       KDboolean is_set,
*                                       KDuint32 left_volume,
*                                       KDuint32 right_volume
*                                       )
* \param  is_set
* \param  left_volume
* \param  right_volume
* \return BOOLEAN 
* \brief  dsp set volume
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Set_Volume(
                                        KDboolean is_set,
                                        KDuint32 left_volume,
                                        KDuint32 right_volume
                                        )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_AUDIO_SET_VOLUME;
    cmd.parameter0 = is_set & DTLCMD_DWORD_LOW;
    cmd.parameter0 = left_volume + (cmd.parameter0 << 16);
    cmd.parameter1 = right_volume & DTLCMD_DWORD_LOW;
    cmd.parameter1 = cmd.parameter1 << 16;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Get_Volume(void)
* \param  void
* \return BOOLEAN 
* \brief  dsp get volume
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Get_Volume(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_AUDIO_GET_VOLUME;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_MFHeader_Filter(
*                                          DMUX_MFH_FILTER_ENABLE_CMD_S* fen_cmd_ptr
*                                          )
* \param  DMUX_MFH_FILTER_ENABLE_CMD_S
* \return KDboolean 
* \brief  启动复用帧头部过滤
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_MFHeader_Filter(
                                         DMUX_MFH_FILTER_ENABLE_CMD_S *fen_cmd_ptr
                                         )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};

    SCI_ASSERT( PNULL != fen_cmd_ptr ); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_ENABLE_MFHEADER_FILTER;
    cmd.parameter0 = fen_cmd_ptr->mux_frame_sel & DTLCMD_DWORD_LOW;
    cmd.parameter0 = fen_cmd_ptr->filter_en + (cmd.parameter0 << 16);
    cmd.parameter1 = fen_cmd_ptr->unequal_filter_en & DTLCMD_DWORD_LOW;
    cmd.parameter1 = fen_cmd_ptr->equal_filter_en + (cmd.parameter1 << 16);

    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_ENABLE_MFHEADER_FILTER);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),fen_cmd_ptr->mux_frame_sel);
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),fen_cmd_ptr->filter_en);
    PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),fen_cmd_ptr->unequal_filter_en);
    PUT16(((KDuint8*)&cmd+4*sizeof(KDuint16)),fen_cmd_ptr->equal_filter_en);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;     
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_MFHeader_Filter(
*                                           DMUX_MFH_FILTER_DISABLE_CMD_S *fd_cmd_ptr
*                                           )
* \param  DMUX_MFH_FILTER_DISABLE_CMD_S
* \return KDboolean 
* \brief  结束复用帧头部过滤
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_MFHeader_Filter(
                                          DMUX_MFH_FILTER_DISABLE_CMD_S *fd_cmd_ptr
                                          )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT( PNULL != fd_cmd_ptr); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_DISABLE_MFHEADER_FILTER;
    cmd.parameter0 = fd_cmd_ptr->mux_frame_sel & DTLCMD_DWORD_LOW;
    cmd.parameter0 = fd_cmd_ptr->filter_en + (cmd.parameter0 << 16);

    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_DISABLE_MFHEADER_FILTER);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),fd_cmd_ptr->mux_frame_sel);
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),fd_cmd_ptr->filter_en);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;     
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Table_Filter(
*                                       DMUX_TABLE_FILTER_ENABLE_CMD_S *tbe_cmd_ptr
*                                       )
* \param  DMUX_TABLE_FILTER_ENABLE_CMD_S
* \return KDboolean 
* \brief  启动一路控制信息表过滤
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Table_Filter(
                                      DMUX_TABLE_FILTER_ENABLE_CMD_S *tbe_cmd_ptr
                                      )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT(PNULL != tbe_cmd_ptr ); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_ENABLE_TABLE_FILTER;
    cmd.parameter0 = tbe_cmd_ptr->reserved & DTLCMD_DWORD_LOW;
    cmd.parameter0 = tbe_cmd_ptr->filter_sel + (cmd.parameter0 << 16);
    cmd.parameter1 = tbe_cmd_ptr->unequal_filter_en & DTLCMD_DWORD_LOW;
    cmd.parameter1 = tbe_cmd_ptr->equal_filter_en + (cmd.parameter1 << 16);
    
    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_ENABLE_TABLE_FILTER);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),tbe_cmd_ptr->reserved);
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),tbe_cmd_ptr->filter_sel);
    PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),tbe_cmd_ptr->unequal_filter_en);
    PUT16(((KDuint8*)&cmd+4*sizeof(KDuint16)),tbe_cmd_ptr->equal_filter_en);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;     
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Table_Filter(
*                                        DMUX_TABLE_FILTER_DISABLE_CMD_S * tbd_cmd_ptr
*                                        )
* \param  DMUX_TABLE_FILTER_DISABLE_CMD_S
* \return KDboolean 
* \brief  结束一路控制信息表过滤
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Table_Filter(
                                       DMUX_TABLE_FILTER_DISABLE_CMD_S * tbd_cmd_ptr
                                       )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT(PNULL != tbd_cmd_ptr); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_DISABLE_TABLE_FILTER;
    cmd.parameter0 = tbd_cmd_ptr->reserved0 & DTLCMD_DWORD_LOW;
    cmd.parameter0 = tbd_cmd_ptr->filter_sel + (cmd.parameter0 << 16);
    
    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};    
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_DISABLE_TABLE_FILTER);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),tbd_cmd_ptr->filter_sel);
    
    cmmbdmx_command((unsigned char*)&cmd);
    return KD_TRUE;     
#endif
}


/************************************************************************
* \fn  KDboolean DTLCMDQ_Set_Demux_Params(
*                                    DMUX_SET_PARAMS_CMD_S *demux_cmd_ptr
*                                    )
* \param  DMUX_SET_PARAMS_CMD_S
* \return KDboolean 
* \brief  设置解复用器参数
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Set_Demux_Params(
                                   DMUX_SET_PARAMS_CMD_S *demux_cmd_ptr
                                   )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT (PNULL != demux_cmd_ptr); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_SET_DEMUX_PARAMS;
    cmd.parameter0 = demux_cmd_ptr->service.mBits & DTLCMD_DWORD_LOW;
    cmd.parameter0 = demux_cmd_ptr->mux_subframe_id + (cmd.parameter0 << 16);
    cmd.parameter1 = demux_cmd_ptr->video_stream_id & DTLCMD_DWORD_LOW;
    cmd.parameter1 = demux_cmd_ptr->audio_stream_id + (cmd.parameter1<<16);
    
    return DTL_SendCMD(&cmd);
#else
    return 0;
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_MSFHeader_Filter(
*                                           DMUX_MSFH_FILTER_ENABLE_CMD_S *msfhe_cmd_ptr
*                                           )
* \param  DMUX_MSFH_FILTER_ENABLE_CMD_S
* \return KDboolean 
* \brief  启动复用子帧头部过滤
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_MSFHeader_Filter(
                                          DMUX_MSFH_FILTER_ENABLE_CMD_S *msfhe_cmd_ptr
                                          )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT( PNULL != msfhe_cmd_ptr ); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_ENABLE_MSFHEADER_FILTER;
    cmd.parameter0 = msfhe_cmd_ptr->reserved0 & DTLCMD_DWORD_LOW;
    cmd.parameter0 = msfhe_cmd_ptr->filter_sel + (cmd.parameter0 << 16);
    cmd.parameter1 = msfhe_cmd_ptr->unequal_filter_en & DTLCMD_DWORD_LOW;
    cmd.parameter1 = msfhe_cmd_ptr->reserved1 + (cmd.parameter1 << 16);
    
    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_ENABLE_MSFHEADER_FILTER);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),msfhe_cmd_ptr->reserved0);
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),msfhe_cmd_ptr->filter_sel);
    PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),msfhe_cmd_ptr->unequal_filter_en);
    PUT16(((KDuint8*)&cmd+4*sizeof(KDuint16)),msfhe_cmd_ptr->reserved1);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;     
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_MSFHeader_Filter(
*                                            DMUX_MSFH_FILTER_DISABLE_CMD_S *msfhd_cmd_ptr
*                                            )
* \param  DMUX_MSFH_FILTER_DISABLE_CMD_S
* \return KDboolean 
* \brief  结束复用子帧头部过滤
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_MSFHeader_Filter(
                                           DMUX_MSFH_FILTER_DISABLE_CMD_S *msfhd_cmd_ptr
                                           )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT(PNULL!=msfhd_cmd_ptr); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_DISABLE_MSFHEADER_FILTER;
    cmd.parameter0 = msfhd_cmd_ptr->reserved0 & DTLCMD_DWORD_LOW;
    cmd.parameter0 = msfhd_cmd_ptr->filter_sel + (cmd.parameter0 << 16);
    
    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_DISABLE_MSFHEADER_FILTER);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),msfhd_cmd_ptr->reserved0);
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),msfhd_cmd_ptr->filter_sel);
    
    cmmbdmx_command((unsigned char*)&cmd);  
    return KD_TRUE; 
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Set_Descram_Key(
*                                      KDuint16 service_idx,
*                                      KDuint16 key_id,
*                                      KDboolean key_select
*                                      KDuint8* cw
*                                      )
* \param  key_id
* \param  key_select
* \return KDboolean 
* \brief  Set Descram Key
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Set_Descram_Key(
                                       KDuint16 service_idx,
                                       KDuint16 key_id,
                                       KDboolean key_select,
                                       KDuint8* cw
                                       )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_SET_DESCRAM_KEY;
    cmd.parameter0 = key_id;
    cmd.parameter1 = key_select;
    
    return DTL_SendCMD(&cmd);
#else
    DTL_CMDQ_CMD_T cmd = {0};

    DTL_SetDescramKey(key_id,cw);    
    
    cmd.cmd_id = DSP_CMDQ_SET_DESCRAM_KEY;
    cmd.parameter0 = service_idx;
    cmd.parameter1 = key_id;
    cmd.parameter2 =  (uint32)cw;

    return DTL_SendCMD(&cmd);
#endif    
}

/************************************************************************
* \fn KDboolean DTLCMDQ_Set_Isma_Rtp_Param(
*                                      KDuint16 service_idx,
*                                      KDuint16 stream_index
*                                       KDuint8* vid_salt,
*                                       KDuint8* aud_salt
*                                      )
* \param  stream_index
* \return KDboolean 
* \brief  Set isma rtp param
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Set_Isma_Rtp_Param(
                                       KDuint16 service_idx,
                                       KDuint16 stream_index,
                                       KDuint8* vid_salt,
                                       KDuint8* aud_salt
                                       )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_SET_ISMA_RTP_PARAM;
    cmd.parameter0 = stream_index;
    
    return DTL_SendCMD(&cmd);
#else
    DTL_CMDQ_CMD_T cmd = {0};

    DTL_SetSaltKey(vid_salt,aud_salt);    
    
    cmd.cmd_id = DSP_CMDQ_SET_ISMA_RTP_PARAM;
    cmd.parameter0 = service_idx;        
    cmd.parameter1 = (uint32)vid_salt;
    cmd.parameter2 = (uint32)aud_salt;
    return DTL_SendCMD(&cmd);
#endif
}

/************************************************************************
* \fn  PUBLIC KDboolean DTLCMDQ_Get_STC(void)
* \param  void
* \return KDboolean 
* \brief  get STC
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Get_STC(void)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_GET_STC;

    return DTL_SendCMD(&cmd);
#else
    return KD_TRUE;
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Set_STC(
*                           DMUX_STC_CMD_S *demux_stc_ptr
*                           )
* \param  DMUX_STC_CMD_S
* \return KDboolean 
* \brief  set STC
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Set_STC(
                          DMUX_STC_CMD_S *demux_stc_ptr
                          )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT(PNULL!= demux_stc_ptr); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_SET_STC;
    cmd.parameter0 = demux_stc_ptr->reserved;
    cmd.parameter1 = demux_stc_ptr->stc;
    
    return DTL_SendCMD(&cmd);
#else
    return KD_TRUE;
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Video_Output(void)
* \param  void
* \return KDboolean 
* \brief  Enable Video Output
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Video_Output(void)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_ENABLE_VIDEO_OUTPUT;

    return DTL_SendCMD(&cmd);
#else
    return 0;
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Video_Output(void)
* \param  void
* \return KDboolean 
* \brief  Disable Video Output
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Video_Output(void)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DISABLE_VIDEO_OUTPUT;

    return DTL_SendCMD(&cmd);
#else
    return 0;
#endif    
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Audio_Output(void)
* \param  void
* \return KDboolean 
* \brief  Enable Audio Output
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Audio_Output(void)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_ENABLE_AUDIO_OUTPUT;

    return DTL_SendCMD(&cmd);
#else
    return 0;
#endif    
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Audio_Output(void)
* \param  void
* \return KDboolean 
* \brief  Disable Audio Output
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Audio_Output(void)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DISABLE_AUDIO_OUTPUT;

    return DTL_SendCMD(&cmd);
#else
    return 0;
#endif    
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Data_Filter(
*                                      DMUX_DATA_FILTER_ENABLE_CMD_S *ddfe_cmd_ptr
*                                      )
* \param  DMUX_DATA_FILTER_ENABLE_CMD_S
* \return KDboolean 
* \brief  Enable Data Filter
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Data_Filter(
                                     DMUX_DATA_FILTER_ENABLE_CMD_S *ddfe_cmd_ptr
                                     )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT(PNULL != ddfe_cmd_ptr); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_ENABLE_DATA_FILTER;
    cmd.parameter0 = ddfe_cmd_ptr->filter_id & DTLCMD_DWORD_LOW;
    cmd.parameter0 = ddfe_cmd_ptr->data_info + (cmd.parameter0 << 16);
    cmd.parameter1 = ddfe_cmd_ptr->equal_filter_en & DTLCMD_DWORD_LOW;
    cmd.parameter1 = ddfe_cmd_ptr->unequal_filter_en + (cmd.parameter1 << 16);
    
    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};

    SCI_ASSERT(PNULL != ddfe_cmd_ptr); /*assert verified*/   
    
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_ENABLE_DATA_FILTER);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),ddfe_cmd_ptr->filter_id);
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),ddfe_cmd_ptr->data_info);
    PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),ddfe_cmd_ptr->equal_filter_en);
    PUT16(((KDuint8*)&cmd+4*sizeof(KDuint16)),ddfe_cmd_ptr->unequal_filter_en);
        
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;     
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Data_Filter(
*                                       DMUX_DATA_FILTER_DISABLE_CMD_S *ddfd_cmd_ptr
*                                       )
* \param  DMUX_DATA_FILTER_DISABLE_CMD_S
* \return KDboolean 
* \brief  Disable Data Filter
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Data_Filter(
                                      DMUX_DATA_FILTER_DISABLE_CMD_S *ddfd_cmd_ptr
                                      )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT( PNULL != ddfd_cmd_ptr); /*assert verified*/ 
    
    cmd.cmd_id = DSP_CMDQ_DISABLE_DATA_FILTER;
    cmd.parameter0 = ddfd_cmd_ptr->reserved0 & DTLCMD_DWORD_LOW;
    cmd.parameter0 = ddfd_cmd_ptr->filter_sel + (cmd.parameter0 << 16);
    
    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};

    SCI_ASSERT( PNULL != ddfd_cmd_ptr); /*assert verified*/ 
    
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_DISABLE_DATA_FILTER);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),ddfd_cmd_ptr->reserved0);
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),ddfd_cmd_ptr->filter_sel);
    PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),ddfd_cmd_ptr->reserved1);
    PUT16(((KDuint8*)&cmd+4*sizeof(KDuint16)),ddfd_cmd_ptr->reserved2);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;     
#endif    
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Demux_Start_Playback(KDint is_setstc, KDint need_stcsuboffset, KDuint32 stc)
* \param  void
* \return KDboolean 
* \brief  Demux Start playback
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Demux_Start_Playback(KDint is_setstc, KDint need_stcsuboffset, KDuint32 stc)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DEMUX_START_PLAYBACK;
    cmd.parameter0 = ((is_setstc ? 0x1 : 0) | (need_stcsuboffset ? 0x2 : 0)) << 16;
    cmd.parameter1 = stc;

    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};

    PUT16(((KDuint8*)&cmd),DSP_CMDQ_DEMUX_START_PLAYBACK);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),is_setstc);
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),need_stcsuboffset);
    PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),stc);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE; 
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Demux_Stop_Playback(void)
* \param  void
* \return KDboolean 
* \brief  Demux stop playback
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Demux_Stop_Playback(void)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DEMUX_STOP_PLAYBACK;

    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};    
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_DEMUX_STOP_PLAYBACK);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE; 
#endif    
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_STC_Count(void)
* \param  void
* \return KDboolean 
* \brief  Enable STC count
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_STC_Count(void)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_ENABLE_STC_COUNT;

    return DTL_SendCMD(&cmd);
#else
    return KD_TRUE;
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_STC_Count(void)
* \param  void
* \return KDboolean 
* \brief  Disable STC count
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_STC_Count(void)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DISABLE_STC_COUNT;

    return DTL_SendCMD(&cmd);
#else
    return KD_TRUE;
#endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Start_Record(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  Start Record
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Start_Record(uint16 service_idx)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_START_RECORD;
    
    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_START_RECORD);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),service_idx<<8);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE; 
#endif    
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Stop_Record(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  Stop Record
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Stop_Record(uint16 service_idx)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_STOP_RECORD;
    
    return DTL_SendCMD(&cmd);
#else
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_STOP_RECORD);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),service_idx<<8);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;    
#endif    
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Relock(void)
* \param  void
* \return KDboolean 
* \brief  重新捕获
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Relock(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_RELOCK;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Set_Tuner(
*                             KDuint32 frequency,
*                             KDuint32 bandwidth
*                             )
* \param  frequency
* \param  bandwidth mode=0; //=0: 8M mode; =1: 2M
* \return KDboolean 
* \brief  设置频点带宽
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Set_Tuner(
                            KDuint32 frequency,
                            KDuint32 bandwidth,
                            KDuint32 searchflag
                            )
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_SET_TUNNER;
    cmd.parameter0 = frequency;
    cmd.parameter1 = bandwidth;
    cmd.parameter1 = bandwidth | ((searchflag & 0x0ffff) << 16);
    
    return DTL_SendCMD(&cmd);
#else
    //Demod_SetTuner(PNULL, frequency, bandwidth);
    DTL_CMDQ_CMD_T cmd = {0};    
    cmd.cmd_id     = DSP_CMDQ_SET_TUNNER;
    cmd.parameter0 = frequency;
    cmd.parameter1 = bandwidth;
    return DTL_SendCMD(&cmd);
 #endif
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Tuner(void)
* \param  void
* \return KDboolean 
* \brief  启动Tunner
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Tuner(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_ENABLE_TUNNER;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Tuner(void)
* \param  void
* \return KDboolean 
* \brief  停止Tunner
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Tuner(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DISABLE_TUNNER;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Set_Param(
*                             STiMi_SLCH_Settings_S *setting_ptr
*                             )
* \param  STiMi_SLCH_Settings_S 
* \return KDboolean 
* \brief  set param
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Set_Param(
                            STiMi_SLCH_Settings_S *setting_ptr
                            )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_SET_PARAM;

    if( !DTL_SendData((KDuint8*)setting_ptr,sizeof(STiMi_SLCH_Settings_S),SHARED_MEM_STIMI_SLCH_BASE))
    {
        KD_TRACE_LOW("DTLCMDQ_Set_Param send data error");
        return FALSE;
    }

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Channel(void)
* \param  void
* \return KDboolean 
* \brief  启动复用帧接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Channel(uint16 service_idx,KDuint32 p1,KDuint32 p2)
{

    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_ENABLE_CHANNEL;
#ifdef MTV_MIDDLEWARE_ON_BB    
    cmd.parameter0 = service_idx;
    cmd.parameter1 = p1;
    cmd.parameter2 = p2;
#endif
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Channel(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  结束复用帧接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Channel(uint16 service_idx)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DISABLE_CHANNEL;
#ifdef MTV_MIDDLEWARE_ON_BB    
    cmd.parameter0 = service_idx;
#endif
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_TS0_Enable_Channel(void)
* \param  void
* \return KDboolean 
* \brief  启动TS0接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_TS0_Enable_Channel(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_TS0_ENABLE_CHANNEL;
#ifdef MTV_MIDDLEWARE_ON_BB    
    cmd.parameter0 = 0xffff;
#endif
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_TS0_Disable_Channel(void)
* \param  void
* \return KDboolean 
* \brief  结束TS0接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_TS0_Disable_Channel(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_TS0_DISABLE_CHANNEL;
#ifdef MTV_MIDDLEWARE_ON_BB    
    cmd.parameter0 = 0xffff;
#endif
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Channel_Complete(void)
* \param  void
* \return KDboolean 
* \brief  
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Channel_Complete(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DISABLE_CHANNEL_COMPLETE;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Start_IQ_Catch(void)
* \param  void
* \return KDboolean 
* \brief  
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Start_IQ_Catch(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_START_IQ_CATCH;

    return DTL_SendCMD(&cmd);
}


/************************************************************************
* \fn  KDboolean DTLCMDQ_Channel_2_Set_Param(
*                             STiMi_SLCH_Settings_S *setting_ptr
*                             )
* \param  STiMi_SLCH_Settings_S 
* \return KDboolean 
* \brief  set param
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Channel_2_Set_Param(
                                           STiMi_SLCH_Settings_S *setting_ptr
                                           )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_CHANNEL_2_SET_PARAM;
    
    if( !DTL_SendData((uint8*)setting_ptr,sizeof(STiMi_SLCH_Settings_S),SHARED_MEM_STIMI_SLCH2_BASE))
    {
        //SCI_TRACE_LOW("DTLCMDQ_Channel_2_Set_Param send data error");
        return FALSE;
    }
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Channel_2_Enable_Channel(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  启动复用帧接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Channel_2_Enable_Channel(uint16 service_idx,KDuint32 p1,KDuint32 p2)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_CHANNEL_2_ENABLE_CHANNEL;
#ifdef MTV_MIDDLEWARE_ON_BB
    cmd.parameter0 = service_idx;
    cmd.parameter1 = p1;
    cmd.parameter2 = p2;
#endif
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Channel_2_Disable_Channel(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  结束复用帧接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Channel_2_Disable_Channel(uint16 service_idx)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_CHANNEL_2_DISABLE_CHANNEL;
#ifdef MTV_MIDDLEWARE_ON_BB
    cmd.parameter0 = service_idx;
#endif    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Lock_Params(void)
* \param  void
* \return KDboolean 
* \brief  lock配置参数
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Lock_Params(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_LOCK_PARAMS;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Unlock_Params(void)
* \param  void
* \return KDboolean 
* \brief  unlock配置参数
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Unlock_Params(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_UNLOCK_PARAMS;

    return DTL_SendCMD(&cmd);
}


/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_FIC_Filter(void)
* \param  void
* \return KDboolean 
* \brief  enable FIC filter
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_FIC_Filter(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_ENABLE_FIC_FILTER;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_FIC_Filter(void)
* \param  void
* \return KDboolean 
* \brief  Disable FIC Filter
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_FIC_Filter(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_DISABLE_FIC_FILTER;

    return DTL_SendCMD(&cmd);
}


/************************************************************************
* \fn  KDboolean DTLCMDQ_Demodulator_Start_Playback(void)
* \param  void
* \return KDboolean 
* \brief  Demodulator Start playback
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Demodulator_Start_Playback(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_START_PLAYBACK; //duplicate with demux 

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Demodulator_Stop_Playback(void)
* \param  void
* \return KDboolean 
* \brief  Demodulator stop playback
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Demodulator_Stop_Playback(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_STOP_PLAYBACK; //duplicate with demux 

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Play(
*                              VIDEO_INIT_PARA_T *v_init_ptr
*                              )
* \param  VIDEO_INIT_PARA_T
* \return KDboolean 
* \brief  Video Play
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Play(
                             VIDEO_INIT_PARA_T *v_init_ptr
                             )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT( PNULL != v_init_ptr); /*assert verified*/ 
    
    cmd.cmd_id     = ARM_CMDQ_VIDEO_PLAY;
    cmd.parameter0 = v_init_ptr->para.value;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Stop(void)
* \param  void
* \return KDboolean 
* \brief  Video Stop
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Stop(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = ARM_CMDQ_VIDEO_STOP;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Pause(void)
* \param  void
* \return KDboolean 
* \brief  Video Pause
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Pause(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = ARM_CMDQ_VIDEO_PAUSE;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Resume(void)
* \param  void
* \return KDboolean 
* \brief  Video resume
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Resume(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = ARM_CMDQ_VIDEO_RESUME;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_FF(void)
* \param  void
* \return KDboolean 
* \brief  Video fast forward
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_FF(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = ARM_CMDQ_VIDEO_FF;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Update_Params(
*                                       KDuint8 *data_ptr,
*                                       VIDEO_PARA_SET_T *v_set_ptr
*                                       )
* \param  data_ptr  data 
* \param  VIDEO_PARA_SET_T
* \return KDboolean 
* \brief  Video update params
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Update_Params(
                                      KDuint8 *data_ptr,
                                      VIDEO_PARA_SET_T *v_set_ptr
                                      )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    SCI_ASSERT ( PNULL != v_set_ptr); /*assert verified*/ 
    
    if ( v_set_ptr->length > 0 )
    {
        if (!DTL_SendData(data_ptr,v_set_ptr->length,v_set_ptr->address))
        {
            KD_TRACE_LOW("DTLCMDQ_Video_Update_Params send data error");
            return FALSE;
        }
    }
    
    cmd.cmd_id     = ARM_CMDQ_VIDEO_UPDATE_PARAMS;
    //cmd.parameter0 = v_set_ptr->;
    cmd.parameter1 = v_set_ptr->address;
    cmd.parameter2 = v_set_ptr->length;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Params_Change_Ack(void)
* \param  void
* \return KDboolean 
* \brief  Video params change ack
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Params_Change_Ack(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = ARM_CMDQ_VIDEO_PARAMS_CHANGE_ACK;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Channel_Change(void)
* \param  void
* \return KDboolean 
* \brief  Video channel change
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Channel_Change(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = ARM_CMDQ_VIDEO_CHANNEL_CHANGE;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Enable(
*                               KDboolean is_enable
*                               )
* \param  is_enable SCI_TURE:Enable,SCI_FALSE,unable
* \return KDboolean 
* \brief  lcdc enable
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Enable(
                              KDboolean is_enable
                              )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id     = ARM_CMDQ_LCDC_ENABLE;
    cmd.parameter0 = is_enable;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Layer_Enable(
*                                     LCDC_LAYER_E layer,
*                                     KDboolean      is_enable
                                      )
* \param  layer 
* \param  SCI_TURE:Enable,SCI_FALSE,unable
* \return KDboolean 
* \brief  lcdc layer enable
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Layer_Enable(
                                    LCDC_LAYER_E layer,
                                    KDboolean      is_enable
                                    )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id     = ARM_CMDQ_LCDC_LAYER_ENABLE;
    cmd.parameter0 = layer;
    cmd.parameter1 = is_enable;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Set_Spec(
*                                 LCDC_CMD_SPEC_T* spec_ptr
*                                 )
* \param  spec_ptr spec data
* \return KDboolean 
* \brief  lcdc set lcd specification
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Set_Spec(
                                LCDC_CMD_SPEC_T* spec_ptr
                                )
{
    DTL_CMDQ_CMD_T cmd = {0};
    KDuint32 i;
    LCDC_CMD_SPEC_T lcdc_spec = *spec_ptr;

    INT_SWAP(lcdc_spec.reserved_0);
    INT_SWAP(lcdc_spec.back_color.value);
    INT_SWAP(lcdc_spec.order);
    INT_SWAP(lcdc_spec.ex_ram_ptr);
    INT_SWAP(lcdc_spec.osd_base_queue_addr);
    INT_SWAP(lcdc_spec.osd_pad_queue_addr);

    for(i=0; i<10; i++)
    {
        INT_SWAP(lcdc_spec.color_coeff.color_coeff_a[i]);
    }
    
    cmd.cmd_id = ARM_CMDQ_LCDC_SET_SPEC;
    
    return DTL_SendCMD_And_MsgParam((KDuint8*)&lcdc_spec,sizeof(LCDC_CMD_SPEC_T),&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Set_Layer_Spec(
*                                       KDuint8*       layer_data_ptr,
*                                       LCDC_LAYER_E layer_index
*                                       )
* \param  layer_data_ptr layer data
* \param  layer_index 
* \return KDboolean 
* \brief  lcdc set layer specification
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Set_Layer_Spec(
                                      KDuint8*       layer_data_ptr,
                                      LCDC_LAYER_E layer_index
                                      )
{
#if 0
    DTL_CMDQ_CMD_T cmd      = {0};
    KDuint32         data_len = 0;
    LCDC_CMD_OSD_SPEC_T* osd_ptr=KD_NULL;
    LCDC_CMD_VIDEO_SPEC_T* video_ptr = KD_NULL;
    
    switch(layer_index)
    {
    case LCDC_LAYER_OSD:
        {
        LCDC_CMD_OSD_SPEC_T osd_spec = *((LCDC_CMD_OSD_SPEC_T*)layer_data_ptr);
        layer_data_ptr = (KDuint8*)&osd_spec;        
        data_len = sizeof(LCDC_CMD_OSD_SPEC_T);
        
        INT_SWAP(osd_spec.lcd_start.mBits.start_x);
        INT_SWAP(osd_spec.lcd_start.mBits.start_y);
        INT_SWAP(osd_spec.lcd_end.mBits.end_x);
        INT_SWAP(osd_spec.lcd_end.mBits.end_y);

        INT_SWAP(osd_spec.colorkey.colorkey_min.value);
        INT_SWAP(osd_spec.colorkey.colorkey_max.value);

        INT_SWAP(osd_spec.osd_format);
        INT_SWAP(osd_spec.clut8_data_ptr);
        }
        break;
    case LCDC_LAYER_VIDEO:
        {
        KDuint32 i;
        LCDC_CMD_VIDEO_SPEC_T video_spec = *((LCDC_CMD_VIDEO_SPEC_T*)layer_data_ptr);
        layer_data_ptr= (KDuint8*)&video_spec;
        data_len = sizeof(LCDC_CMD_VIDEO_SPEC_T);

        
        INT_SWAP(video_spec.video_size.mBits.width);
        INT_SWAP(video_spec.video_size.mBits.height);

        INT_SWAP(video_spec.video_start.mBits.start_x);
        INT_SWAP(video_spec.video_start.mBits.start_y);
        INT_SWAP(video_spec.video_end.mBits.end_x);
        INT_SWAP(video_spec.video_end.mBits.end_y);
        
        INT_SWAP(video_spec.lcd_start.mBits.start_x);
        INT_SWAP(video_spec.lcd_start.mBits.start_y);
        INT_SWAP(video_spec.lcd_end.mBits.end_x);
        INT_SWAP(video_spec.lcd_end.mBits.end_y);

        INT_SWAP(video_spec.colorkey.colorkey_min.value);
        INT_SWAP(video_spec.colorkey.colorkey_max.value);

        INT_SWAP(video_spec.scaler_coeff_ptr);

        INT_SWAP(video_spec.reserved_0);
        INT_SWAP(video_spec.angle);
        INT_SWAP(video_spec.autosize);

        for(i=0; i<12; i++)
        {
            INT_SWAP(video_spec.yuv2rgb_coeff.yuv2rgb_c0[i]);
        }
        }
        break;
    default:
        kdAssert(0); /*assert verified*//*lint !e506*/
        break;

    }
    
    cmd.cmd_id     = ARM_CMDQ_LCDC_SET_LAYER_SPEC;
    cmd.parameter0 = layer_index;
    
    return DTL_SendCMD_And_MsgParam(layer_data_ptr,data_len,&cmd);
    #endif
    return 0;
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Display_OSD( void )
* \param  void
* \return KDboolean 
* \brief  lcdc display osd base image
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Display_OSD( void )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id     = ARM_CMDQ_LCDC_DISPLAY_OSD;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Update_OSD( void )
* \param  void
* \return KDboolean 
* \brief  lcdc update osd increment image
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Update_OSD( void )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id     = ARM_CMDQ_LCDC_UPDATE_OSD;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Set_Contrast(
*                                           KDuint32 contrast_val
*                                           )
* \param  contrast_val contrast value
* \return KDboolean 
* \brief  lcdc set contrast value
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Set_Contrast(
                                    KDuint32 contrast_val
                                    )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id     = ARM_CMDQ_LCDC_SET_CONTRAST;
    cmd.parameter0 = contrast_val;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Set_Brightness(
*                                       KDuint32 brightness_val
*                                       )
* \param  brightness_val brightness value
* \return KDboolean 
* \brief  lcdc set brightness value
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Set_Brightness(
                                      KDuint32 brightness_val
                                      )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id     = ARM_CMDQ_LCDC_SET_BRIGHTNESS;
    cmd.parameter0 = brightness_val;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Freeze(
*                               KDuint32 capture_buf_addr
*                               )
* \param  capture_buf_addr capture buffer address
* \return KDboolean 
* \brief  lcdc capture buffer address (seine save position)
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Freeze(
                              KDuint32 capture_buf_addr
                              )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id     = ARM_CMDQ_LCDC_FREEZE;
    cmd.parameter0 = capture_buf_addr;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Resume(void)
* \param  void
* \return KDboolean 
* \brief  lcdc resume
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Resume(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = ARM_CMDQ_LCDC_RESUME;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Download_Arm_Code(
*                                     KDuint32 dest_addr,
*                                     KDuint32 src_addr,
*                                     KDuint32 code_size
*                                     )
* \param  dest_addr dest address
* \param  src_addr  source address
* \param  code_size 
* \return KDboolean 
* \brief  send download arm code command 
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Download_Arm_Code(
                                    KDuint32 dest_addr,
                                    KDuint32 src_addr,
                                    KDuint32 code_size
                                    )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = BB_ARM_CMDQ_DOWNLOAD_ARM_CODE;
    cmd.parameter0 = dest_addr;
    cmd.parameter1 = src_addr;
    cmd.parameter2 = code_size;
    
    return DTL_SendCMD_NOINT(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Jumpto_IRAM_Run(void)
* \param  void
* \return KDboolean 
* \brief  jump to IRAM run
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Jumpto_IRAM_Run(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = BB_ARM_CMDQ_JUMP_TO_IRAM_RUN;
    
    return DTL_SendCMD_NOINT(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Jumpto_Run(
*                              KDuint32 run_addr
*                              )
* \param  run_addr run address
* \return KDboolean 
* \brief  jump to run_addr to run
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Jumpto_Run(
                             KDuint32 run_addr
                             )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id     = BB_ARM_CMDQ_JUMP_TO_RUN;
    cmd.parameter0 = run_addr;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Turn_Off_TV(void)
* \param  void
* \return KDboolean 
* \brief  turn off TV
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Turn_Off_TV(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = BB_ARM_CMDQ_TURN_OFF_TV;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Write_Odd(
*                                               KDuint32 start_addr,
*                                               KDuint32 end_addr,
*                                               KDuint32 write_data
*                                               )
* \param  odd start_addr
* \param  ood end_addr
* \param  write_data high8->start low8->end
* \return KDboolean 
* \brief  
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Write_Odd(
                                                KDuint32 start_addr,
                                                KDuint32 end_addr,
                                                KDuint32 write_data
                                                )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = BB_ARM_CMDQ_WRITE_ODD;
    cmd.parameter0 = start_addr;
    cmd.parameter1 = end_addr;
    cmd.parameter2 = write_data;

    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Switch_Video_Decoder(
*                                        VIDEO_DECODER_TYPE_E vd_type
*                                        )
* \param  vd_type 0: H.264;1:AVS
* \return KDboolean 
* \brief  switch video decoder
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Switch_Video_Decoder(
                                       VIDEO_DECODER_TYPE_E vd_type
                                       )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = BB_ARM_CMDQ_SWITCH_VIDEO_DECODER;
    cmd.parameter0 = vd_type;

    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Switch_Audio_Decoder(
*                                        AUDIO_DECODER_TYPE_E ad_type
*                                        )
* \param  ad_type 0: MP3;1:AAC-LC;2:AAC+
* \return KDboolean 
* \brief  switch audio decoder
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Switch_Audio_Decoder(
                                       AUDIO_DECODER_TYPE_E ad_type
                                       )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = BB_ARM_CMDQ_SWITCH_AUDIO_DECODER;
    cmd.parameter0 = ad_type;
    
    return DTL_SendCMD(&cmd);
}


/************************************************************************
* \fn  KDboolean DTLCMDQ_TX_Data_Through_SPI(
*                                        KDuint32 src_addr,
*                                        KDuint32 data_size
*                                        )
* \param  src_addr write data address
* \param  data_size data size
* \return KDboolean 
* \brief  send write data command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_TX_Data_Through_SPI(
                                      KDuint32 src_addr,
                                      KDuint32 data_size
                                      )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = BB_ARM_CMDQ_TX_DATA_THROGH_SPI;
    cmd.parameter0 = src_addr;
    cmd.parameter1 = data_size;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_RX_Data_Through_SPI(
*                                        KDuint32 src_addr,
*                                        KDuint32 data_size
*                                        )
* \param  src_addr source address
* \param  data_size data size
* \return KDboolean 
* \brief  send request data command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_RX_Data_Through_SPI(
                                      KDuint32 src_addr,
                                      KDuint32 data_size
                                      )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = BB_ARM_CMDQ_RX_DATA_THROGH_SPI;
    cmd.parameter0 = src_addr;
    cmd.parameter1 = data_size;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_DSP_SET_FREQ(
*                                KDuint32 parameter0,
*                                KDuint32 parameter1,
*                                KDuint32 parameter2
*                                )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  dsp set freq
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_DSP_SET_FREQ(
                               KDuint32 parameter0,
                               KDuint32 parameter1,
                               KDuint32 parameter2
                               )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_SET_FREQ;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_DSP_GET_PLLSTATUS(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  dsp get pllstatus
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_DSP_GET_PLLSTATUS(
                                    KDuint32 parameter0,
                                    KDuint32 parameter1,
                                    KDuint32 parameter2
                                    )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_GET_PLLSTATUS;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_DSP_GET_MODUTYPE(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  dsp get modutype
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_DSP_GET_MODUTYPE(
                                   KDuint32 parameter0,
                                   KDuint32 parameter1,
                                   KDuint32 parameter2
                                   )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_GET_MODUTYPE;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_DSP_GET_FREQOFFSET(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  dsp get FREQOFFSET
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_DSP_GET_FREQOFFSET(
                                     KDuint32 parameter0,
                                     KDuint32 parameter1,
                                     KDuint32 parameter2
                                     )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_GET_FREQOFFSET;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_DSP_GET_RSSI(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  dsp get RSSI
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_DSP_GET_RSSI(
                               KDuint32 parameter0,
                               KDuint32 parameter1,
                               KDuint32 parameter2
                               )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_GET_RSSI;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_DSP_GET_RSSI(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  dsp get BLER
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_DSP_GET_BLER(
                               KDuint32 parameter0,
                               KDuint32 parameter1,
                               KDuint32 parameter2
                               )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_GET_BLER;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_DSP_SET_TUNERREG(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  dsp set TUNERREG
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_DSP_SET_TUNERREG(
                                   KDuint32 parameter0,
                                   KDuint32 parameter1,
                                   KDuint32 parameter2
                                   )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_SET_TUNERREG;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_DSP_GET_TUNERREG(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  dsp get TUNERREG
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_DSP_GET_TUNERREG(
                                   KDuint32 parameter0,
                                   KDuint32 parameter1,
                                   KDuint32 parameter2
                                   )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = DSP_CMDQ_GET_TUNERREG;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_IRAMROM(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test IRAMROM
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_IRAMROM(
                                      KDuint32 parameter0,
                                      KDuint32 parameter1,
                                      KDuint32 parameter2
                                      )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_IRAMROM;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_UART(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test UART
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_UART(
                                   KDuint32 parameter0,
                                   KDuint32 parameter1,
                                   KDuint32 parameter2
                                   )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_UART;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_I2C(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test I2C
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_I2C(
                                  KDuint32 parameter0,
                                  KDuint32 parameter1,
                                  KDuint32 parameter2
                                  )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_I2C;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_SPI(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test SPI
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_SPI(
                                  KDuint32 parameter0,
                                  KDuint32 parameter1,
                                  KDuint32 parameter2
                                  )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_SPI;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_GPIO(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test GPIO
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_GPIO(
                                   KDuint32 parameter0,
                                   KDuint32 parameter1,
                                   KDuint32 parameter2
                                   )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_GPIO;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_I2S(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test I2S
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_I2S(
                                  KDuint32 parameter0,
                                  KDuint32 parameter1,
                                  KDuint32 parameter2
                                  )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_I2S;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_SIM(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test SIM
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_SIM(
                                  KDuint32 parameter0,
                                  KDuint32 parameter1,
                                  KDuint32 parameter2
                                  )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_SIM;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_BUSMON(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test BUSMON
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_BUSMON(
                                     KDuint32 parameter0,
                                     KDuint32 parameter1,
                                     KDuint32 parameter2
                                     )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_BUSMON;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_SCREEN_TEST_SLEEP(
*                                     KDuint32 parameter0,
*                                     KDuint32 parameter1,
*                                     KDuint32 parameter2
*                                     )
* \param  parameter0
* \param  parameter1
* \param  parameter2
* \return KDboolean 
* \brief  arm screen test SLEEP
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_SCREEN_TEST_SLEEP(
                                    KDuint32 parameter0,
                                    KDuint32 parameter1,
                                    KDuint32 parameter2
                                    )
{
    DTL_CMDQ_CMD_T cmd = {0};
    
    cmd.cmd_id     = ARM_CMDQ_SCREEN_TEST_SLEEP;
    cmd.parameter0 = parameter0;
    cmd.parameter1 = parameter1;
    cmd.parameter2 = parameter2;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Turn_Off_Seine_POWER(void)
* \param  void
* \return KDboolean 
* \brief  turn off seine power
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Turn_Off_Seine_POWER(void)
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = BB_ARM_CMDQ_TURN_OFF_SEINE_POWER;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Read_Data_ACK( KDboolean is_success )
* \param  is_success
* \return KDboolean 
* \brief  write data ack for V read data from ap
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Read_Data_ACK( KDboolean is_success )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = BB_ARM_CMDQ_READ_DATA_ACK;
    cmd.parameter0 = is_success;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Write_Data_ACK( KDboolean is_success )
* \param  is_success
* \return KDboolean 
* \brief  write data ack for V write data to ap
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Write_Data_ACK( KDboolean is_success )
{
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = BB_ARM_CMDQ_WRITE_DATA_ACK;
    cmd.parameter0 = is_success;
    
    return DTL_SendCMD(&cmd);
}

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Mbbms(KDboolean is_mbbms)
* \param  is_mbbms
* \return KDboolean 
* \brief  
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Mbbms(KDboolean is_mbbms)
{
#ifndef MTV_MIDDLEWARE_ON_BB
    DTL_CMDQ_CMD_T cmd = {0};
    cmd.cmd_id = DSP_CMDQ_ENABLE_MBBMS;
    cmd.parameter0 = is_mbbms;

    return DTL_SendCMD(&cmd);
#else
    return KD_TRUE;
#endif
}

#ifdef MTV_MIDDLEWARE_ON_BB
KDboolean DTLCMDQ_Enable_Service(DEMUX_SERVICE_T* service_ptr)
{
    CMDQ_DSP_ARM_CMD_T cmd = {0};

    SCI_ASSERT(PNULL!= service_ptr); /*assert verified*/ 

/*lint -save -e502 -e506 -e774*/      
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_ENABLE_SERVICE);

    BIT_PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),0,8,service_ptr->service.demux_service.mf_id);    
    BIT_PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),8,5,service_ptr->service.demux_service.service_num);
    BIT_PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),13,3,service_ptr->service.demux_service.service_type);
    
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),service_ptr->msf_idx);

    BIT_PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),0,4,service_ptr->video.av_idx.av_id);
    BIT_PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),8,1,service_ptr->video.av_idx.av_enable);

    BIT_PUT16(((KDuint8*)&cmd+4*sizeof(KDuint16)),0,4,service_ptr->audio.av_idx.av_id);
    BIT_PUT16(((KDuint8*)&cmd+4*sizeof(KDuint16)),8,1,service_ptr->audio.av_idx.av_enable);
/*lint -restore */
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;
}

KDboolean DTLCMDQ_Disable_Service(uint16 service_idx)
{
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_DISABLE_SERVICE);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),service_idx);
    
    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;    
}

KDboolean DTLCMDQ_Enable_AV_Output(uint16 service_idx,uint8 video_enable,uint8 audio_enable)
{
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_ENABLE_VA_OUTPUT);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),service_idx<<8);
/*lint -save -e572*/          
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),video_enable);
    PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),audio_enable);
/*lint -restore */

    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;        
}

KDboolean DTLCMDQ_Disable_AV_Output(uint16 service_idx,uint8 video_enable,uint8 audio_enable)
{
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_DISABLE_VA_OUTPUT);
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),service_idx<<8);
/*lint -save -e572*/       
    PUT16(((KDuint8*)&cmd+2*sizeof(KDuint16)),video_enable);
    PUT16(((KDuint8*)&cmd+3*sizeof(KDuint16)),audio_enable);
/*lint -restore */

    cmmbdmx_command((unsigned char*)&cmd);    
    return KD_TRUE;        
}

KDboolean DTLCMDQ_Set_Demux_Params_Ext(
                                   uint8 is_mbbms
                                   )
{
    CMDQ_DSP_ARM_CMD_T cmd = {0};
    PUT16(((KDuint8*)&cmd),DSP_CMDQ_SET_DEMUX_PARAMS);
/*lint -save -e572*/    
    PUT16(((KDuint8*)&cmd+sizeof(KDuint16)),is_mbbms);
/*lint -restore */    
    cmmbdmx_command((unsigned char*)&cmd);
    
    return KD_TRUE;
}


#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
