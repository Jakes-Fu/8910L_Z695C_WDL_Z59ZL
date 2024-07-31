/******************************************************************************
 ** File Name:      dtl_cmdq.h                                                *
 ** Author:         Hanbing Zhang                                             *
 ** DATE:           6/11/2007                                                 *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define data transfer layer command interface    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 6/11/2007     Hanbing.Zhang        create                                 *
 ******************************************************************************/

#ifndef _DTL_CMDQ_H
#define _DTL_CMDQ_H

/**------------------------------------------------------------------------------------*
 **                         Dependencies           	                                   *
 **------------------------------------------------------------------------------------*/
#include "kd.h"
#include "dtl.h"
#include "cmd_common.h"
/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/

#define  INNO_CHAN_PARA(ts_start,ts_count,map_mode)   ((((ts_start)&0x0ff)<<16) | (((ts_count)&0x0ff) << 8) | ((map_mode) & 0x0ff))

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Play(void)
* \param  void
* \return KDboolean 
* \brief  send audio play command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Play(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Pcm_Mix(void)
* \param  void
* \return KDboolean 
* \brief  send audio pcm mix command
*
* \author Wangyq
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Pcm_Mix(KDboolean isMix);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Stop(void)
* \param  void
* \return KDboolean 
* \brief  send audio stop command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Stop(void);

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
                                         );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Pause(void)
* \param  void
* \return KDboolean 
* \brief  send audio pause command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Pause(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Resume(void)
* \param  void
* \return KDboolean 
* \brief  send audio resume command
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Resume(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Data_Request( void )
* \param  void
* \return KDboolean 
* \brief  send audio data request
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Data_Request( void );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Channel_Change(void)
* \param  void
* \return KDboolean 
* \brief  audio channel change
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Channel_Change(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Mute(void)
* \param  void
* \return BOOLEAN 
* \brief  iis mute
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Mute(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Unmute(void)
* \param  void
* \return BOOLEAN 
* \brief  iis unmute
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Unmute(void);

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
                                        );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Audio_Get_Volume(void)
* \param  void
* \return BOOLEAN 
* \brief  dsp get volume
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Audio_Get_Volume(void);

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
                                         );

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
                                          );
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
                                       );

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
                                       );

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
                                      );

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
                                       );

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
                                   );

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
                                          );

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
                                           );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Get_STC(void)
* \param  void
* \return KDboolean 
* \brief  get STC
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Get_STC(void);

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
                          );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Video_Output(void)
* \param  void
* \return KDboolean 
* \brief  Enable Video Output
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Video_Output(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Video_Output(void)
* \param  void
* \return KDboolean 
* \brief  Disable Video Output
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Video_Output(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Audio_Output(void)
* \param  void
* \return KDboolean 
* \brief  Enable Audio Output
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Audio_Output(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Audio_Output(void)
* \param  void
* \return KDboolean 
* \brief  Disable Audio Output
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Audio_Output(void);

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
                                     );

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
                                      );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Demux_Start_Playback(KDint is_setstc, KDint need_stcsuboffset, KDuint32 stc)
* \param  void
* \return KDboolean 
* \brief  Demux Start playback
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Demux_Start_Playback(KDint is_setstc, KDint need_stcsuboffset, KDuint32 stc);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Demux_Stop_Playback(void)
* \param  void
* \return KDboolean 
* \brief  Demux stop playback
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Demux_Stop_Playback(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_STC_Count(void)
* \param  void
* \return KDboolean 
* \brief  Enable STC count
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_STC_Count(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_STC_Count(void)
* \param  void
* \return KDboolean 
* \brief  Disable STC count
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_STC_Count(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Start_Record(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  Start Record
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Start_Record(uint16 service_idx);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Stop_Record(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  Stop Record
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Stop_Record(uint16 service_idx);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Relock(void)
* \param  void
* \return KDboolean 
* \brief  重新捕获
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Relock(void);

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
                            );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Tuner(void)
* \param  void
* \return KDboolean 
* \brief  启动Tunner
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Tuner(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Tuner(void)
* \param  void
* \return KDboolean 
* \brief  停止Tunner
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Tuner(void);

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
                            );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Channel(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  启动复用帧接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Channel(uint16 service_idx,KDuint32 p1,KDuint32 p2);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Channel(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  结束复用帧接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Channel(uint16 service_idx);

/************************************************************************
* \fn  KDboolean DTLCMDQ_TS0_Enable_Channel(void)
* \param  void
* \return KDboolean 
* \brief  启动TS0接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_TS0_Enable_Channel(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_TS0_Disable_Channel(void)
* \param  void
* \return KDboolean 
* \brief  结束TS0接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_TS0_Disable_Channel(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_Channel_Complete(void)
* \param  void
* \return KDboolean 
* \brief  
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_Channel_Complete(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Start_IQ_Catch(void)
* \param  void
* \return KDboolean 
* \brief  
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Start_IQ_Catch(void);

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
                                           );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Channel_2_Enable_Channel(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  启动复用帧接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Channel_2_Enable_Channel(uint16 service_idx,KDuint32 p1,KDuint32 p2);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Channel_2_Disable_Channel(uint16 service_idx)
* \param  service_idx
* \return KDboolean 
* \brief  结束复用帧接收
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Channel_2_Disable_Channel(uint16 service_idx);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Lock_Params(void)
* \param  void
* \return KDboolean 
* \brief  lock配置参数
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Lock_Params(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Unlock_Params(void)
* \param  void
* \return KDboolean 
* \brief  unlock配置参数
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Unlock_Params(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_FIC_Filter(void)
* \param  void
* \return KDboolean 
* \brief  enable FIC filter
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_FIC_Filter(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Disable_FIC_Filter(void)
* \param  void
* \return KDboolean 
* \brief  Disable FIC Filter
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Disable_FIC_Filter(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Demodulator_Start_Playback(void)
* \param  void
* \return KDboolean 
* \brief  Demodulator Start playback
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Demodulator_Start_Playback(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Demodulator_Stop_Playback(void)
* \param  void
* \return KDboolean 
* \brief  Demodulator stop playback
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Demodulator_Stop_Playback(void);

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
                             );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Stop(void)
* \param  void
* \return KDboolean 
* \brief  Video Stop
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Stop(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Pause(void)
* \param  void
* \return KDboolean 
* \brief  Video Pause
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Pause(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Resume(void)
* \param  void
* \return KDboolean 
* \brief  Video resume
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Resume(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_FF(void)
* \param  void
* \return KDboolean 
* \brief  Video fast forward
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_FF(void);

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
                                      );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Params_Change_Ack(void)
* \param  void
* \return KDboolean 
* \brief  Video params change ack
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Params_Change_Ack(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Video_Channel_Change(void)
* \param  void
* \return KDboolean 
* \brief  Video channel change
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Video_Channel_Change(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Enable(
                                KDboolean is_enable
                                )
* \param  is_enable SCI_TURE:Enable,SCI_FALSE,unable
* \return KDboolean 
* \brief  lcdc enable
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Enable(
                              KDboolean is_enable
                              );

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
                                    KDboolean     is_enable
                                    );

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
                                );

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
                                      );

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Display_OSD( void )
* \param  void
* \return KDboolean 
* \brief  lcdc display osd base image
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Display_OSD( void );

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Update_OSD( void )
* \param  void
* \return KDboolean 
* \brief  lcdc update osd increment image
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Update_OSD( void );

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
                                    );

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
                                      );

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
                              );

/************************************************************************
* \fn  KDboolean DTLCMDQ_LCDC_Resume(void)
* \param  void
* \return KDboolean 
* \brief  lcdc resume
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_LCDC_Resume(void);

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
                                    );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Jumpto_IRAM_Run(void)
* \param  void
* \return KDboolean 
* \brief  jump to IRAM run
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Jumpto_IRAM_Run(void);

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
                             );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Turn_Off_TV(void)
* \param  void
* \return KDboolean 
* \brief  turn off TV
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Turn_Off_TV(void);

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
                                                );

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
                                       );

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
                                       );

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
                                      );

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
                                      );

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
                               );

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
                                    );

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
                                   );

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
                                     );

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
                               );

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
                               );

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
                                   );

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
                                   );

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
                                      );

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
                                   );

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
                                  );

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
                                  );

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
                                   );

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
                                  );

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
                                  );

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
                                     );

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
                                    );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Turn_Off_Seine_POWER(void)
* \param  void
* \return KDboolean 
* \brief  turn off seine power
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Turn_Off_Seine_POWER(void);

/************************************************************************
* \fn  KDboolean DTLCMDQ_Read_Data_ACK( KDboolean is_success )
* \param  is_success
* \return KDboolean 
* \brief  write data ack for V read data from ap
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Read_Data_ACK( KDboolean is_success );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Write_Data_ACK( KDboolean is_success )
* \param  is_success
* \return KDboolean 
* \brief  write data ack for V write data to ap
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Write_Data_ACK( KDboolean is_success );

/************************************************************************
* \fn  KDboolean DTLCMDQ_Enable_Mbbms(KDboolean is_mbbms)
* \param  is_mbbms
* \return KDboolean 
* \brief  
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTLCMDQ_Enable_Mbbms(KDboolean is_mbbms);


#ifdef MTV_MIDDLEWARE_ON_BB

typedef union av_idx_union_tag{  
    struct av_idx_tag{
        uint16    reserved     :7;
        uint16    av_enable   :1;
        uint16    res               :4;
        uint16    av_id           :4;
    }av_idx;
    uint16 mBits;
}AV_IDX_U;

typedef struct dmx_service_tag
{
    DEMUX_SERVICE_U service;
    uint16                     msf_idx;
    AV_IDX_U               video;
    AV_IDX_U               audio;    
}DEMUX_SERVICE_T;

KDboolean DTLCMDQ_Enable_Service(DEMUX_SERVICE_T* service_ptr);

KDboolean DTLCMDQ_Disable_Service(uint16 service_idx);

KDboolean DTLCMDQ_Enable_AV_Output(uint16 service_idx,uint8 video_enable,uint8 audio_enable);

KDboolean DTLCMDQ_Disable_AV_Output(uint16 service_idx,uint8 video_enable,uint8 audio_enable);

KDboolean DTLCMDQ_Set_Demux_Params_Ext(
                                   uint8 is_mbbms
                                   );

#endif

#endif
