/******************************************************************************
 ** File Name:      cmd_common.h                                              *
 ** Author:         Gang.Li                                                   *
 ** DATE:           11/23/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic macro definition of command   *
 **                 ID and data struct.                                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/23/2007     Gang.Li          Create.                                   *
 ** 12/07/2007     Gang.Li          modify.                                   *
 ******************************************************************************/

#ifndef _CMD_COMMON_H_
#define _CMD_COMMON_H_


//#define _SC6600V_WQVGA_  1

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mtv_cfg.h"
 /**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
 
/*****************************DSP COMMON COMMAND ID***********************************/
//6600V ARM->6600V DSP
#define ARM_CMDQ_DSP_COMMON_BASE_ID             (0x0000U)
#define DSP_CMDQ_RESET	                        (ARM_CMDQ_DSP_COMMON_BASE_ID+0x00)
#define DSP_CMDQ_TURN_OFF	                    (ARM_CMDQ_DSP_COMMON_BASE_ID+0x01)
#define DSP_CMDQ_SWITCH_AUDIO_DECODER	        (ARM_CMDQ_DSP_COMMON_BASE_ID+0x02)

//6600V DSP->6600V ARM
#define DSP_CMDQ_RESET_ACK	                    (ARM_CMDQ_DSP_COMMON_BASE_ID+0x80) 
#define DSP_CMDQ_TURN_OFF_ACK	                (ARM_CMDQ_DSP_COMMON_BASE_ID+0x81) 
#define DSP_CMDQ_SWITCH_AUDIO_DECODER_FINISHED	(ARM_CMDQ_DSP_COMMON_BASE_ID+0x82) 
#define DSP_CMDQ_DSP_ASSERT						(ARM_CMDQ_DSP_COMMON_BASE_ID+0x83)

 
/*****************************DSP AUDIO COMMAND ID***********************************/
//BB->6600V ARM->6600V DSP
#define ARM_CMDQ_DSP_AUDIO_BASE_ID      (0x0100U)
#define DSP_CMDQ_AUDIO_PLAY	            (ARM_CMDQ_DSP_AUDIO_BASE_ID+0x00)
#define DSP_CMDQ_AUDIO_STOP	            (ARM_CMDQ_DSP_AUDIO_BASE_ID+0x01)
#define DSP_CMDQ_AUDIO_SET_PARAM		(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x02)
#define DSP_CMDQ_AUDIO_PAUSE	        (ARM_CMDQ_DSP_AUDIO_BASE_ID+0x03)
#define DSP_CMDQ_AUDIO_RESUME	        (ARM_CMDQ_DSP_AUDIO_BASE_ID+0x04)
#define DSP_CMDQ_AUDIO_DATA_REQUEST	    (ARM_CMDQ_DSP_AUDIO_BASE_ID+0x05)
#define DSP_CMDQ_AUDIO_CHANNEL_CHANGE	(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x06)
#define DSP_CMDQ_AUDIO_PCM_MIX			(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x07)
#define DSP_CMDQ_AUDIO_SET_VOLUME		(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x08)
#define DSP_CMDQ_AUDIO_GET_VOLUME		(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x09)
//6600V DSP->6600V ARM->BB
#define BB_CMDQ_AUDIO_DATA_REQUEST_ACK	(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x80)
#define BB_CMDQ_AUDIO_STOP_ACK			(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x81)
#define BB_CMDQ_AUDIO_FIRST_FRAME_OK	(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x82)
#define BB_CMDQ_AUDIO_GET_VOLUME_ACK	(ARM_CMDQ_DSP_AUDIO_BASE_ID+0x83)
 
 
/*****************************DSP DEMUX COMMAND ID***********************************/
#ifndef MTV_MIDDLEWARE_ON_BB
//BB->6600V ARM->6600V DSP
#define ARM_CMDQ_DSP_DEMUX_BASE_ID              (0x0200U)
#define DSP_CMDQ_ENABLE_MFHEADER_FILTER	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x00)
#define DSP_CMDQ_DISABLE_MFHEADER_FILTER	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x01)
#define DSP_CMDQ_ENABLE_TABLE_FILTER	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x02)
#define DSP_CMDQ_DISABLE_TABLE_FILTER	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x03)
#define DSP_CMDQ_SET_DEMUX_PARAMS	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x04)
#define DSP_CMDQ_ENABLE_MSFHEADER_FILTER	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x05)
#define DSP_CMDQ_DISABLE_MSFHEADER_FILTER	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x06)
#define DSP_CMDQ_GET_STC	                    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x07)
#define DSP_CMDQ_SET_STC	                    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x08)
#define DSP_CMDQ_ENABLE_STC_COUNT	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x09)
#define DSP_CMDQ_DISABLE_STC_COUNT	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0A)
#define DSP_CMDQ_ENABLE_VIDEO_OUTPUT	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0B)
#define DSP_CMDQ_DISABLE_VIDEO_OUTPUT	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0C)
#define DSP_CMDQ_ENABLE_AUDIO_OUTPUT	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0D)
#define DSP_CMDQ_DISABLE_AUDIO_OUTPUT	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0E)
#define DSP_CMDQ_ENABLE_DATA_FILTER	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0F)
#define DSP_CMDQ_DISABLE_DATA_FILTER	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x10)
#define DSP_CMDQ_START_RECORD		            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x11)
#define DSP_CMDQ_STOP_RECORD			        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x12)
#define DSP_CMDQ_DEMUX_START_PLAYBACK	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x13)
#define DSP_CMDQ_DEMUX_STOP_PLAYBACK	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x14)
#define DSP_CMDQ_DEMUX_START_BYPASS	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x15)
#define DSP_CMDQ_SET_DESCRAM_KEY	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x16)
#define DSP_CMDQ_SET_ISMA_RTP_PARAM	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x17)
#define DSP_CMDQ_ENABLE_MBBMS             	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x18)

//6600V DSP->6600V ARM->BB
#define BB_CMDQ_DATA_READY_DEMUX                (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x80)
#define BB_CMDQ_DISABLE_MFHEADER_RECEIVE_RESP	(ARM_CMDQ_DSP_DEMUX_BASE_ID+0x81) 
#define BB_CMDQ_DISABLE_TABLE_FILTER_RESP	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x82)
#define BB_CMDQ_DISABLE_MSFHEADER_RECEIVE_RESP  (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x83) 
#define BB_CMDQ_STC_READY	                    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x84)
#define BB_CMDQ_DISABLE_VIDEO_OUTPUT_RESP	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x85)
#define BB_CMDQ_DISABLE_AUDIO_OUTPUT_RESP	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x86)
#define BB_CMDQ_DISABLE_DATA_FILTER_RESP	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x87)  
#define BB_CMDQ_STOP_RECORD_RESP			    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x88)
#define BB_CMDQ_DEMUX_STOP_PLAYBACK_RESP		(ARM_CMDQ_DSP_DEMUX_BASE_ID+0x89) 
#define BB_CMDQ_PLAYBACK_DATA_REQUEST           (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8A)
#define BB_CMDQ_ENABLE_DATA_FILTER_RESP         (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8B)
#define BB_CMDQ_SET_DESCRAM_KEY_RESP            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8C)
#define BB_CMDQ_DESCRAM_KEY_REQUEST             (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8D)
#define BB_CMDQ_SET_ISMA_RTP_PARAM_RESP         (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8E)
#define BB_CMDQ_ENABLE_DATA_FILTER_RESP         (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8B)
#define BB_CMDQ_SET_DESCRAM_KEY_RESP            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8C)
#define BB_CMDQ_DESCRAM_KEY_REQUEST             (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8D)

#else

#define ARM_CMDQ_DSP_DEMUX_BASE_ID              (0x0200U)
#define DSP_CMDQ_ENABLE_MFHEADER_FILTER	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x00)
#define DSP_CMDQ_DISABLE_MFHEADER_FILTER	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x01)
#define DSP_CMDQ_ENABLE_TABLE_FILTER	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x02)
#define DSP_CMDQ_DISABLE_TABLE_FILTER	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x03)
#define DSP_CMDQ_SET_DEMUX_PARAMS	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x04)
#define DSP_CMDQ_ENABLE_MSFHEADER_FILTER	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x05)
#define DSP_CMDQ_DISABLE_MSFHEADER_FILTER	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x06)
#define DSP_CMDQ_ENABLE_SERVICE                 (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x07)
#define DSP_CMDQ_DISABLE_SERVICE	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x08)
//#define DSP_CMDQ_ENABLE_STC_COUNT	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x09)
//#define DSP_CMDQ_DISABLE_STC_COUNT            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0A)
#define DSP_CMDQ_ENABLE_VA_OUTPUT		        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0B)
#define DSP_CMDQ_DISABLE_VA_OUTPUT				(ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0C)
//#define DSP_CMDQ_ENABLE_AUDIO_OUTPUT			(ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0D)
//#define DSP_CMDQ_DISABLE_AUDIO_OUTPUT	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0E)
#define DSP_CMDQ_ENABLE_DATA_FILTER	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x0F)
#define DSP_CMDQ_DISABLE_DATA_FILTER	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x10)
#define DSP_CMDQ_START_RECORD		            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x11)
#define DSP_CMDQ_STOP_RECORD			        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x12)
#define DSP_CMDQ_DEMUX_START_PLAYBACK	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x13)
#define DSP_CMDQ_DEMUX_STOP_PLAYBACK	        (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x14)
//#define DSP_CMDQ_DEMUX_START_BYPASS	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x15)
#define DSP_CMDQ_SET_DESCRAM_KEY	            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x16)
#define DSP_CMDQ_SET_ISMA_RTP_PARAM             (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x17)
//#define DSP_CMDQ_DEMUX_INIT                     (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x18)

//6600V DSP->6600V ARM->BB
#define BB_CMDQ_DATA_READY_DEMUX                (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x80)
#define BB_CMDQ_DISABLE_MFHEADER_RECEIVE_RESP	(ARM_CMDQ_DSP_DEMUX_BASE_ID+0x81)
#define BB_CMDQ_DISABLE_TABLE_FILTER_RESP	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x82)
#define BB_CMDQ_DISABLE_MSFHEADER_RECEIVE_RESP  (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x83)
#define BB_CMDQ_DISABLE_SERVICE_RESP            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x84)
#define BB_CMDQ_DISABLE_VA_OUTPUT_RESP		    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x85)
//#define BB_CMDQ_DISABLE_AUDIO_OUTPUT_RESP	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x86)
#define BB_CMDQ_DISABLE_DATA_FILTER_RESP	    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x87)
#define BB_CMDQ_STOP_RECORD_RESP			    (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x88)
#define BB_CMDQ_DEMUX_STOP_PLAYBACK_RESP		(ARM_CMDQ_DSP_DEMUX_BASE_ID+0x89)
#define BB_CMDQ_PLAYBACK_DATA_REQUEST           (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8A)
#define BB_CMDQ_ENABLE_DATA_FILTER_RESP         (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8B)
#define BB_CMDQ_SET_DESCRAM_KEY_RESP            (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8C)
#define BB_CMDQ_DESCRAM_KEY_REQUEST             (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8D)
#define BB_CMDQ_SET_ISMA_RTP_PARAM_RESP         (ARM_CMDQ_DSP_DEMUX_BASE_ID+0x8E)

#endif


/*****************************DSP DEMODULATE COMMAND ID***********************************/
//BB->6600V ARM->6600V DSP
#define ARM_CMDQ_DSP_DEMODULATE_BASE_ID     (0x0300U)
#define DSP_CMDQ_RELOCK	                    (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x00)
#define DSP_CMDQ_SET_TUNNER	                (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x01)
#define DSP_CMDQ_ENABLE_TUNNER	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x02)
#define DSP_CMDQ_DISABLE_TUNNER	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x03)
#define DSP_CMDQ_SET_PARAM	                (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x04)
#define DSP_CMDQ_ENABLE_CHANNEL	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x05)
#define DSP_CMDQ_DISABLE_CHANNEL	        (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x06)
#define DSP_CMDQ_LOCK_PARAMS	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x07)
#define DSP_CMDQ_UNLOCK_PARAMS	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x08)
#define DSP_CMDQ_ENABLE_FIC_FILTER	        (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x09)
#define DSP_CMDQ_DISABLE_FIC_FILTER	        (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x0A)
#define DSP_CMDQ_START_PLAYBACK	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x0B)
#define DSP_CMDQ_STOP_PLAYBACK	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x0C)
#define DSP_CMDQ_CHANNEL_2_SET_PARAM	    (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x0D)
#define DSP_CMDQ_CHANNEL_2_ENABLE_CHANNEL	(ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x0E)
#define DSP_CMDQ_CHANNEL_2_DISABLE_CHANNEL	(ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x0F)
#define DSP_CMDQ_TS0_ENABLE_CHANNEL			(ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x10)
#define DSP_CMDQ_TS0_DISABLE_CHANNEL		(ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x11)
#define DSP_CMDQ_DISABLE_CHANNEL_COMPLETE   (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x12)
#define DSP_CMDQ_START_IQ_CATCH     	 	(ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x20)

//6600V DSP->6600V ARM->BB
#define BB_CMDQ_DISABLE_TUNNER_RESP	        (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x80)
#define BB_CMDQ_DISABLE_CHANNEL_RESP	    (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x81) 
#define BB_CMDQ_LOCK_PARAMS_RESP	        (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x82)
#define BB_CMDQ_DISABLE_FIC_FILTER_RESP     (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x83) 
#define BB_CMDQ_STOP_PLAYBACK_RESP	        (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x84)
#define BB_CMDQ_DATA_REQUIRE	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x85)
#define BB_CMDQ_DATA_READY                  (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x86)
#define BB_CMDQ_SIGNAL_QUALITY	            (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x87)
#define BB_CMDQ_SIGNAL_STATUS				(ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x88)
#define DSP_CMDQ_DISABLE_CHANNEL_COMPLETE_RESP       (ARM_CMDQ_DSP_DEMODULATE_BASE_ID+0x89)  
 
/*****************************DSP RF DVT COMMAND ID***********************************/
//BB->6600V ARM->6600V DSP
#define ARM_CMDQ_DSP_RF_DVT_BASE_ID     	(0x0800U)
#define DSP_CMDQ_SET_FREQ	                (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x00)
#define DSP_CMDQ_GET_PLLSTATUS	            (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x01)
#define DSP_CMDQ_GET_MODUTYPE	            (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x02)
#define DSP_CMDQ_GET_FREQOFFSET	            (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x03)
#define DSP_CMDQ_GET_RSSI	                (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x04)
#define DSP_CMDQ_GET_BLER	            	(ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x05)
#define DSP_CMDQ_SET_TUNERREG	        	(ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x06)
#define DSP_CMDQ_GET_TUNERREG	            (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x07)

//6600V DSP->6600V ARM->BB
#define DSP_CMDQ_SET_FREQ_RESP	        	(ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x80)
#define DSP_CMDQ_GET_PLLSTATUS_RESP	    	(ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x81) 
#define DSP_CMDQ_GET_MODUTYPE_RESP	        (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x82)
#define DSP_CMDQ_GET_FREQOFFSET_RESP     	(ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x83) 
#define DSP_CMDQ_GET_RSSI_RESP	        	(ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x84)
#define DSP_CMDQ_GET_BLER_RESP	            (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x85)
#define DSP_CMDQ_SET_TUNERREG_RESP          (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x86)
#define DSP_CMDQ_GET_TUNERREG_RESP	        (ARM_CMDQ_DSP_RF_DVT_BASE_ID+0x87)  

/*****************************DSP SCREEN TEST COMMAND ID********************************/
//BB->6600V ARM->6600V DSP
#define DSP_CMDQ_DSP_SCREEN_TEST_BASE_ID    (0x0900U)
#define DSP_CMDQ_SCREEN_TEST_IRAMROM        (DSP_CMDQ_DSP_SCREEN_TEST_BASE_ID+0x00)

//6600V DSP->6600V ARM->BB
#define DSP_CMDQ_SCREEN_TEST_IRAMROM_RESP   (DSP_CMDQ_DSP_SCREEN_TEST_BASE_ID+0x80)
 
/*****************************ARM COMMON COMMAND ID**************************************/
//BB->6600V ARM
#define ARM_CMDQ_ARM_BASE_ID                        (0x10000UL)
#define BB_ARM_CMDQ_DOWNLOAD_ARM_CODE	            (ARM_CMDQ_ARM_BASE_ID+0x00)
#define BB_ARM_CMDQ_JUMP_TO_IRAM_RUN	            (ARM_CMDQ_ARM_BASE_ID+0x01)
#define BB_ARM_CMDQ_JUMP_TO_RUN	                    (ARM_CMDQ_ARM_BASE_ID+0x02)
#define BB_ARM_CMDQ_TURN_OFF_TV	                    (ARM_CMDQ_ARM_BASE_ID+0x03)
#define BB_ARM_CMDQ_SWITCH_VIDEO_DECODER	        (ARM_CMDQ_ARM_BASE_ID+0x04)
#define BB_ARM_CMDQ_SWITCH_AUDIO_DECODER	        (ARM_CMDQ_ARM_BASE_ID+0x05)
#define BB_ARM_CMDQ_RX_DATA_THROGH_SPI				(ARM_CMDQ_ARM_BASE_ID+0x06)
#define BB_ARM_CMDQ_TX_DATA_THROGH_SPI				(ARM_CMDQ_ARM_BASE_ID+0x07)
#define BB_ARM_CMDQ_TX_DATA_THROGH_IIS				(ARM_CMDQ_ARM_BASE_ID+0x08)
#define BB_ARM_CMDQ_TURN_OFF_SEINE_POWER            (ARM_CMDQ_ARM_BASE_ID+0x09)
#define BB_ARM_CMDQ_AUDIO_MUTE			            (ARM_CMDQ_ARM_BASE_ID+0x0A)
#define BB_ARM_CMDQ_AUDIO_UNMUTE            		(ARM_CMDQ_ARM_BASE_ID+0x0B)

#define BB_ARM_CMDQ_READ_DATA_ACK                   (ARM_CMDQ_ARM_BASE_ID+0x10)
#define BB_ARM_CMDQ_WRITE_DATA_ACK                  (ARM_CMDQ_ARM_BASE_ID+0x11)
#define BB_ARM_CMDQ_WRITE_ODD                            (ARM_CMDQ_ARM_BASE_ID+0x12)

//6600V ARM->BB
#define BB_ARM_STATUS_DOWNLOAD_ARM_CODE_FINISHED	(ARM_CMDQ_ARM_BASE_ID+0x80)
#define BB_ARM_STATUS_ARM_DSP_BOOT_FINISHED	        (ARM_CMDQ_ARM_BASE_ID+0x81) 
#define BB_ARM_STATUS_TURN_OFF_TV_FINISHED	        (ARM_CMDQ_ARM_BASE_ID+0x82)
#define BB_ARM_STATUS_SWITCH_VIDEO_DECODER_FINISHED	(ARM_CMDQ_ARM_BASE_ID+0x84)
#define BB_ARM_STATUS_SWITCH_AUDIO_DECODER_FINISHED	(ARM_CMDQ_ARM_BASE_ID+0x85) 
#define BB_ARM_STATUS_SPI_DMA_READY                 (ARM_CMDQ_ARM_BASE_ID+0x86)
#define BB_ARM_STATUS_TX_DATA_THROGH_IIS			(ARM_CMDQ_ARM_BASE_ID+0x87)
#define BB_ARM_STATUS_ASSERT			            (ARM_CMDQ_ARM_BASE_ID+0x88)

#define BB_ARM_STATUS_READ_DATA                     (ARM_CMDQ_ARM_BASE_ID+0x90)
#define BB_ARM_STATUS_WRITE_DATA                    (ARM_CMDQ_ARM_BASE_ID+0x91)
#define BB_ARM_CMDQ_WRITE_ODD_ACK                  (ARM_CMDQ_ARM_BASE_ID+0x93)
/* SPI DMA ready status used for CMD BB_ARM_STATUS_SPI_DMA_READY */
#define SPI_DMA_READY_OK                  	              0
#define SPI_DMA_READY_ERROR                                 1
 
 
/*****************************LCDC COMMAND ID**************************************/
//BB->6600V ARM
#define ARM_CMDQ_LCDC_BASE_ID           (0x20000UL)
#define ARM_CMDQ_LCDC_ENABLE	        (ARM_CMDQ_LCDC_BASE_ID+0x00)
#define ARM_CMDQ_LCDC_LAYER_ENABLE	    (ARM_CMDQ_LCDC_BASE_ID+0x01)
#define ARM_CMDQ_LCDC_SET_SPEC	        (ARM_CMDQ_LCDC_BASE_ID+0x02)
#define ARM_CMDQ_LCDC_SET_LAYER_SPEC	(ARM_CMDQ_LCDC_BASE_ID+0x03)
#ifdef _SC6600V_WQVGA_
#define ARM_CMDQ_LCDC_UPDATE_VIDEO	    (ARM_CMDQ_LCDC_BASE_ID+0x04)
#endif
#define ARM_CMDQ_LCDC_DISPLAY_OSD	    (ARM_CMDQ_LCDC_BASE_ID+0x04)
#define ARM_CMDQ_LCDC_UPDATE_OSD	    (ARM_CMDQ_LCDC_BASE_ID+0x05)
#define ARM_CMDQ_LCDC_SET_CONTRAST	    (ARM_CMDQ_LCDC_BASE_ID+0x06)
#define ARM_CMDQ_LCDC_SET_BRIGHTNESS	(ARM_CMDQ_LCDC_BASE_ID+0x07)
#define ARM_CMDQ_LCDC_GET_STATE	        (ARM_CMDQ_LCDC_BASE_ID+0x08)
#define ARM_CMDQ_LCDC_FREEZE	        (ARM_CMDQ_LCDC_BASE_ID+0x09)
#define ARM_CMDQ_LCDC_RESUME	        (ARM_CMDQ_LCDC_BASE_ID+0x0a)
#define ARM_CMDQ_LCDC_GET_LCD_INFO      (ARM_CMDQ_LCDC_BASE_ID+0x0b)
#define ARM_CMDQ_LCDC_SET_FRAME_RATE    (ARM_CMDQ_LCDC_BASE_ID+0x0c)
#define ARM_CMDQ_LCDC_SET_GAMMA_TABLE   (ARM_CMDQ_LCDC_BASE_ID+0x0d)
#define ARM_CMDQ_LCDC_SET_VIDEO_FETCHER   (ARM_CMDQ_LCDC_BASE_ID+0x0e)
#define ARM_CMDQ_LCDC_CHANGED_LCD       (ARM_CMDQ_LCDC_BASE_ID+0x0f)
#define ARM_CMDQ_LCDC_SET_GAMMA_TABLE   (ARM_CMDQ_LCDC_BASE_ID+0x0d)
#define ARM_CMDQ_LCDC_SET_VIDEO_FETCHER (ARM_CMDQ_LCDC_BASE_ID+0x0e)
#define ARM_CMDQ_LCDC_CHANGED_LCD       (ARM_CMDQ_LCDC_BASE_ID+0x0f)

//6600V ARM->BB
#define BB_CMDQ_LCDC_STATE	            (ARM_CMDQ_LCDC_BASE_ID+0x80)
#define BB_CMDQ_LCDC_FREEZE_DONE	    (ARM_CMDQ_LCDC_BASE_ID+0x81)

/*****************************VIDEO COMMAND ID*************************************/
//BB->6600V ARM
#define ARM_CMDQ_VIDEO_BASE_ID              (0x30000UL)
#define ARM_CMDQ_VIDEO_PLAY	                (ARM_CMDQ_VIDEO_BASE_ID+0x00)
#define ARM_CMDQ_VIDEO_STOP	                (ARM_CMDQ_VIDEO_BASE_ID+0x01)
#define ARM_CMDQ_VIDEO_PAUSE	            (ARM_CMDQ_VIDEO_BASE_ID+0x02)
#define ARM_CMDQ_VIDEO_RESUME	            (ARM_CMDQ_VIDEO_BASE_ID+0x03)
#define ARM_CMDQ_VIDEO_FF	                (ARM_CMDQ_VIDEO_BASE_ID+0x04)
#define ARM_CMDQ_VIDEO_UPDATE_PARAMS	    (ARM_CMDQ_VIDEO_BASE_ID+0x05)
#define ARM_CMDQ_VIDEO_PARAMS_CHANGE_ACK	(ARM_CMDQ_VIDEO_BASE_ID+0x06)
#define ARM_CMDQ_VIDEO_CHANNEL_CHANGE	    (ARM_CMDQ_VIDEO_BASE_ID+0x07)
#ifdef _SC6600V_WQVGA_
#define ARM_CMDQ_VIDEO_UPDATE_REF_CLOCK     (ARM_CMDQ_VIDEO_BASE_ID+0x08)
#define ARM_CMDQ_VIDEO_GET_INFO				(ARM_CMDQ_VIDEO_BASE_ID+0x09)
#endif

//6600V ARM->BB
#define BB_CMDQ_VIDEO_PARAMS_CHANGE	        (ARM_CMDQ_VIDEO_BASE_ID+0x80)
#define BB_CMDQ_VIDEO_PARAMS_REQ	        (ARM_CMDQ_VIDEO_BASE_ID+0x81)
#define BB_CMDQ_VIDEO_STOP_ACK	            (ARM_CMDQ_VIDEO_BASE_ID+0x82)
#define BB_CMDQ_VIDEO_FIRST_FRAME_OK	    (ARM_CMDQ_VIDEO_BASE_ID+0x83)
#ifdef _SC6600V_WQVGA_
#define BB_CMDQ_VIDEO_SEND_INFO				(ARM_CMDQ_VIDEO_BASE_ID+0x84)
#endif


/*****************************SCREEN TEST COMMAND ID********************************/
//BB->6600V ARM
#define ARM_CMDQ_SCREEN_TEST_BASE_ID        (0x40000UL)
#define ARM_CMDQ_SCREEN_TEST_IRAMROM        (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x00)
#define ARM_CMDQ_SCREEN_TEST_UART	        (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x01)
#define ARM_CMDQ_SCREEN_TEST_I2C            (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x02)
#define ARM_CMDQ_SCREEN_TEST_SPI            (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x03)
#define ARM_CMDQ_SCREEN_TEST_GPIO           (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x04)
#define ARM_CMDQ_SCREEN_TEST_I2S		    (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x05)
#define ARM_CMDQ_SCREEN_TEST_SIM			(ARM_CMDQ_SCREEN_TEST_BASE_ID+0x06)
#define ARM_CMDQ_SCREEN_TEST_BUSMON		    (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x07)
#define ARM_CMDQ_SCREEN_TEST_SLEEP		    (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x08)

//6600V ARM->BB
#define ARM_CMDQ_SCREEN_TEST_IRAMROM_RESP   (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x80)
#define ARM_CMDQ_SCREEN_TEST_UART_RESP      (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x81)
#define ARM_CMDQ_SCREEN_TEST_I2C_RESP       (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x82)
#define ARM_CMDQ_SCREEN_TEST_SPI_RESP       (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x83)
#define ARM_CMDQ_SCREEN_TEST_GPIO_RESP      (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x84)
#define ARM_CMDQ_SCREEN_TEST_I2S_RESP       (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x85)
#define ARM_CMDQ_SCREEN_TEST_SIM_RESP       (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x86)
#define ARM_CMDQ_SCREEN_TEST_BUSMON_RESP    (ARM_CMDQ_SCREEN_TEST_BASE_ID+0x87)
#define ARM_CMDQ_SCREEN_TEST_SLEEP_RESP    	(ARM_CMDQ_SCREEN_TEST_BASE_ID+0x88)


/*****************************SIM COMMAND ID********************************/
//BB->6600V ARM
#define ARM_CMDQ_SIM_BASE_ID		(0x70000UL)
#define ARM_CMDQ_SIM_INITIALIZE	(ARM_CMDQ_SIM_BASE_ID+0x00)
#define ARM_CMDQ_SIM_CLOSE		(ARM_CMDQ_SIM_BASE_ID+0x01)
#define ARM_CMDQ_SIM_REQUEST		(ARM_CMDQ_SIM_BASE_ID+0x02)

//6600V ARM->BB
#define ARM_CMDQ_SIM_INITIALIZE_CNF		(ARM_CMDQ_SIM_BASE_ID+0x80)
#define ARM_CMDQ_SIM_CLOSE_CNF			(ARM_CMDQ_SIM_BASE_ID+0x81)
#define ARM_CMDQ_SIM_REQUEST_CNF			(ARM_CMDQ_SIM_BASE_ID+0x82)
/*****************************SCREEN TEST COMMAND ID********************************/


typedef enum 
{
    UART0_UART0,
    RW_I2C,
    RW_SPI,
    RW_GPIOS,
    RW_SIM,
    BUS_MONITOR
}PERIPHERAL_TEST_ENUM;
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/*****************************COMMAND QUEUE DATA STRUCT******************/
#define CMD_BB_CMDQ_SIZE                        16
#define CMD_BB_RESPQ_SIZE                       16
#define CMD_DSP_CMDQ_SIZE                       16
#define CMD_DSP_RESPQ_SIZE                      16

/*****************************SHARED MEMORY MAP******************/
#ifndef MTV_MIDDLEWARE_ON_BB
#define SHARED_MEMORY_BASE_ADDRESS           	(0x10000000)
#else
#define SHARED_MEMORY_SIZE                                   (4096)
extern uint8 share_memory_array[SHARED_MEMORY_SIZE];
#define SHARED_MEMORY_BASE_ADDRESS           	((uint32)share_memory_array)
#endif

#define CMD_BB_ARM_CMDQ_BASE           			(SHARED_MEMORY_BASE_ADDRESS)
#define CMD_ARM_DSP_CMDQ_BASE        			(CMD_BB_ARM_CMDQ_BASE + sizeof(CMDQ_BB_ARM_CMD_QUEUE_T))
#define SHARE_MEM_ARM_WATCHDOG_BASE		(SHARED_MEMORY_BASE_ADDRESS+0x400)
#define SHARE_MEM_DEMUX_BASE                             (SHARED_MEMORY_BASE_ADDRESS+0x800)
//#define	SHARED_MEM_VIDEO_BASE					(0x10000a00)
#define SHARED_MEM_AUDIO_BASE        			(SHARED_MEMORY_BASE_ADDRESS+0xE00)
#define SHARED_MEM_STIMI_SLCH_BASE			(SHARED_MEMORY_BASE_ADDRESS+0xE40)
#define SHARED_CHANNEL_STATUS_BASE			(SHARED_MEMORY_BASE_ADDRESS+0xE60)
#define SHARED_MEM_STIMI_SLCH2_BASE			(SHARED_MEMORY_BASE_ADDRESS+0xEA0)
#define SHARED_CHANNEL_2_STATUS_BASE              (SHARED_MEMORY_BASE_ADDRESS+0xEC0)

/***********************Demux Share Memory Usage***********************************/
#define BUFFER_INDICATOR_BASE_ADDR                      (  SHARE_MEM_DEMUX_BASE  +  (0x0*2)  )
#define VIDEO_FIFO_CONF_BASE_ADDR                       (  SHARE_MEM_DEMUX_BASE  +  (0x100*2))
#define AUDIO_FIFO_CONF_BASE_ADDR                       (  SHARE_MEM_DEMUX_BASE  +  (0x110*2))
#define SLOT_CONF_BASE_ADDR                             (  SHARE_MEM_DEMUX_BASE  +  (0x140*2))
#define MF_FILTER_CONF_BASE_ADDR                        (  SHARE_MEM_DEMUX_BASE  +  (0x180*2))
#define MSF_FILTER_CONF_BASE_ADDR                       (  SHARE_MEM_DEMUX_BASE  +  (0x190*2))
#define TABLE_FILTER_CONF_BASE_ADDR                     (  SHARE_MEM_DEMUX_BASE  +  (0x1a0*2))
#define DATA_FILTER_CONF_BASE_ADDR                      (  SHARE_MEM_DEMUX_BASE  +  (0x220*2))
#define DESCRAM_KEY_BASE_ADDR                           (  SHARE_MEM_DEMUX_BASE  +  (0x2e0*2))
#define DEMUX_ISMA_CFG_BASE_ADDR                        (  SHARE_MEM_DEMUX_BASE  +  (0x2f0*2))



/*****************************SDRAM MEMORY MAP******************/
#define SDRAM_MEMORY_BASE_ADDRESS           	(0x90000000)

//ARM COMMON 
//ARM_CONFIG_SECTION 0x90000000-0x90000400 1K Bytes
#define ARM_CONFIG_SECTION_BASE_ADDR      		(SDRAM_MEMORY_BASE_ADDRESS)
#define ARM_CONFIG_SECTION_MAX_SIZE				(0x400)

#define ARM_ASSERT_INFO_BASE_ADDR               (ARM_CONFIG_SECTION_BASE_ADDR+sizeof(SC6600V_ARM_CONFIG_T))
#define MAX_ASSERT_INFO_LENGTH                  (100)

//ARM_CONFIG_LCD_COM_CFG 0x90000400-0x90004000 15K Bytes
#define ARM_CONFIG_LCD_COM_CFG_BASE_ADDR		(ARM_CONFIG_SECTION_BASE_ADDR + ARM_CONFIG_SECTION_MAX_SIZE)
#define ARM_CONFIG_LCD_COM_CFG_MAX_SIZE			(15*1024)

//ARM_COMMON_CODE 0x90004000-0x90020000 112K Bytes
#define ARM_COMMON_CODE_BASE_ADDR      			(ARM_CONFIG_LCD_COM_CFG_BASE_ADDR + ARM_CONFIG_LCD_COM_CFG_MAX_SIZE)
#define ARM_COMMON_CODE_MAX_SIZE				(0x1C000)

//VIDEO_H264_CODE 0x90020000-0x90030000 64K Bytes
#define VIDEO_H264_CODE_BASE_ADDR      			(ARM_COMMON_CODE_BASE_ADDR + ARM_COMMON_CODE_MAX_SIZE)
#define VIDEO_H264_CODE_MAX_SIZE				(0x10000)

//VIDEO_AVS_CODE 0x90030000-0x90040000 64K Bytes
#define VIDEO_AVS_CODE_BASE_ADDR      			(VIDEO_H264_CODE_BASE_ADDR + VIDEO_H264_CODE_MAX_SIZE)
#define VIDEO_AVS_CODE_MAX_SIZE					(0x10000)

//ARM_CODE_IN_SDRAM 0x90040000-0x90080000 256K Bytes
#define ARM_CODE_IN_SDRAM_BASE_ADDR      		(VIDEO_AVS_CODE_BASE_ADDR + VIDEO_AVS_CODE_MAX_SIZE)
#define ARM_CODE_IN_SDRAM_MAX_SIZE				(0x40000)

//DSP_COMMON_CODE_CMMB 0x90080000-0x900A0000 128K Bytes
#define DSP_COMMON_CODE_CMMB_BASE_ADDR      	(ARM_CODE_IN_SDRAM_BASE_ADDR + ARM_CODE_IN_SDRAM_MAX_SIZE)
#define DSP_COMMON_CODE_CMMB_MAX_SIZE			(0x20000)

//DSP_COMMON_CODE_TDMB 0x900A0000-0x900C0000 128K Bytes
#define DSP_COMMON_CODE_TDMB_BASE_ADDR      	(DSP_COMMON_CODE_CMMB_BASE_ADDR + DSP_COMMON_CODE_CMMB_MAX_SIZE)
#define DSP_COMMON_CODE_TDMB_MAX_SIZE			(0x20000)

//DSP_AUDIO_CODE_MP3 0x900C0000-0x900D0000 64K Bytes
#define DSP_AUDIO_CODE_MP3_BASE_ADDR      		(DSP_COMMON_CODE_TDMB_BASE_ADDR + DSP_COMMON_CODE_TDMB_MAX_SIZE)
#define DSP_AUDIO_CODE_MP3_MAX_SIZE				(0x10000)

//DSP_AUDIO_CODE_AAC 0x900D0000-0x900D7000 28K Bytes
#define DSP_AAC_PARAM_TABLE_BASE_ADDR      		(DSP_AUDIO_CODE_MP3_BASE_ADDR + DSP_AUDIO_CODE_MP3_MAX_SIZE)
#define DSP_AAC_PARAM_TABLE_MAX_SIZE			(0x7000)

//DSP_AUDIO_CODE_AAC 0x900D7000-0x900E0000 36K Bytes
#define DSP_AUDIO_CODE_AAC_BASE_ADDR      		(DSP_AAC_PARAM_TABLE_BASE_ADDR + DSP_AAC_PARAM_TABLE_MAX_SIZE)
#define DSP_AUDIO_CODE_AAC_MAX_SIZE				(0x9000)

//DSP_AUDIO_CODE_DRA 0x900E0000-0x900E7000 28K Bytes
#define DSP_DRA_PARAM_TABLE_BASE_ADDR      		(DSP_AUDIO_CODE_AAC_BASE_ADDR + DSP_AUDIO_CODE_AAC_MAX_SIZE)
#define DSP_DRA_PARAM_TABLE_MAX_SIZE			(0x7000)

//DSP_AUDIO_CODE_DRA 0x900E7000-0x900F0000 36K Bytes
#define DSP_AUDIO_CODE_DRA_BASE_ADDR      		(DSP_DRA_PARAM_TABLE_BASE_ADDR + DSP_DRA_PARAM_TABLE_MAX_SIZE)
#define DSP_AUDIO_CODE_DRA_MAX_SIZE				(0x9000)


//ARM_COMMON_DATA 0x900F0000-0x90100000 64K Bytes
#define ARM_COMMON_DATA_BASE_ADDRESS           	(0x900F0000)
#define ARM_COMMON_DATA_MAX_SIZE           		(0x10000)

#define CMD_BB_ARM_MSGQ_BASE           			(ARM_COMMON_DATA_BASE_ADDRESS)
#define CMD_MESSAGE_LEN                       	(512)
#define CMD_MESSAGE_MAX_SIZE					(CMD_MESSAGE_LEN * CMD_BB_CMDQ_SIZE)

#define	VIDEO_UPDATE_PARAMS_BASE_ADDR  			(0x900F2000)   
#define	VIDEO_UPDATE_PATAMS_MAX_SIZE   			(8*1024)

/******************************************************************/
/*
 * CODE_IN_SDRAM
 * use the spare space (~700KB) in this area for R2V project should be modified!!!!!!!
 */
#define CODE_IN_SDRAM_BASE_ADDR                 (0x90600000)
#define CODE_IN_SDRAM_MAX_SIZE                  (1536*1024)
/******************************************************************/

//VIDEO
//VIDEO 0x90100000-0x9025C000 1392K Bytes
#define	VIDEO_FRAME_BUFFER_BASE_ADDR  			(0x90100000)   
#define	VIDEO_FRAME_BUFFER_MAX_SIZE   			(1392*1024)

#define VIDEO_FRAME_BACK_BUFFER_Y_ADDR_0          (DSP_COMMON_CODE_CMMB_BASE_ADDR)
#define VIDEO_FRAME_BACK_BUFFER_Y0_SIZE          (256*1024)

#define VIDEO_FRAME_BACK_BUFFER_Y_ADDR_1          (DEFAULT_VIDEO_BUFFER_BASE_ADDR)
#define VIDEO_FRAME_BACK_BUFFER_Y1_SIZE          (256*1024)

//TS/ESG
//TS/ESG 0x9025C000-0x90300000 656K Bytes
#define	TS_ESG_BUFFER_BASE_ADDR  				(0x9025C000)   
#define	TS_ESG_BUFFER_MAX_SIZE   				(656*1024)

#define	ESG_BUFFER_BASE_ADDR 					TS_ESG_BUFFER_BASE_ADDR
#define	ESG_BUFFER_MAX_SIZE   					TS_ESG_BUFFER_MAX_SIZE

#define	TS_BUFFER_BASE_ADDR 					(0x9029C000)
#define	TS_BUFFER_MAX_SIZE   					(400*1024)

//DEMUX/VIDEO
//DEMUX/VIDEO 0x90300000-0x90340000 256K Bytes
#define	VIDEO_STREAM_BUFFER_BASE_ADDR  			(0x90300000)   
#define	VIDEO_STREAM_BUFFER_MAX_SIZE   			(255*1024)

#define	VIDEO_DESCRIPTOR_BUFFER_BASE_ADDR  		(0x9033FC00)   
#define	VIDEO_DESCRIPTOR_BUFFER_MAX_SIZE   		(1*1024)
//DEFAULT VIDEO 0x90340000-0x90380000 256K Bytes
#define	DEFAULT_VIDEO_BUFFER_BASE_ADDR  		(0x90340000)   
#define	DEFAULT_VIDEO_BUFFER_MAX_SIZE   		(256*1024)

//OVERLAP DSP code
#define VIDEO_FRAME_BACK_BUFFER_Y_ADDR_0          (DSP_COMMON_CODE_CMMB_BASE_ADDR)
#define VIDEO_FRAME_BACK_BUFFER_Y0_SIZE          (256*1024)

#define VIDEO_FRAME_BACK_BUFFER_Y_ADDR_1          (DEFAULT_VIDEO_BUFFER_BASE_ADDR)
#define VIDEO_FRAME_BACK_BUFFER_Y1_SIZE          (256*1024)

//BB/AUDIO
//BB/AUDIO 0x90380000-0x90400000 512K Bytes
#define	AUDIO_STREAM_BUFFER_BASE_ADDR  			(0x90380000)   
#define	AUDIO_STREAM_BUFFER_MAX_SIZE   			(64*1024)

#define	AUDIO_PCM_BUFFER_BASE_ADDR  			(0x90390000)   
#define	AUDIO_PCM_BUFFER_MAX_SIZE   			(448*1024)
//DEFAULT VIDEO
//DEFAULT VIDEO 0x90340000-0x90380000 256K Bytes
#define	DEFAULT_VIDEO_BUFFER_BASE_ADDR  		(0x90340000)   
#define	DEFAULT_VIDEO_BUFFER_MAX_SIZE   		(256*1024)

//BB/AUDIO
//BB/AUDIO 0x90380000-0x90400000 512K Bytes
#define	AUDIO_STREAM_BUFFER_BASE_ADDR  			(0x90380000)   
#define	AUDIO_STREAM_BUFFER_MAX_SIZE   			(64*1024)

#define	AUDIO_PCM_BUFFER_BASE_ADDR  			(0x90390000)   
#define	AUDIO_PCM_BUFFER_MAX_SIZE   			(448*1024)

//BB/DEMUX
//BB/DEMUX 0x90400000-0x90600000 2M Bytes
#define	CONTROL_INFO_BUFFER_BASE_ADDR  			(0x90400000)   
#define	CONTROL_INFO_BUFFER_MAX_SIZE   			(64*1024)

#define	OSD_LOOKUP_TABLE_BUFFER_BASE_ADDR  		(0x90410000)   
#define	OSD_LOOKUP_TABLE_BUFFER_MAX_SIZE   		(64*1024)

#define	VIDEO_SCALING_PARAM_BUFFER_BASE_ADDR  	(0x90420000)   
#define	VIDEO_SCALING_PARAM_BUFFER_MAX_SIZE   	(64*1024)

#define	ARM_LOG_BUFFER_BASE_ADDR  				(0x90430000)   
#define	ARM_LOG_BUFFER_MAX_SIZE   				(64*1024)

#define	DSP_LOG_BUFFER_BASE_ADDR  				(0x90440000)   
#define	DSP_LOG_BUFFER_MAX_SIZE   				(64*1024)

#define	ROTATION_BUFFER_BASE_ADDR  				(0x90450000)   
#define	ROTATION_BUFFER_MAX_SIZE   				(256*432/2*4*2)

#define YUV_CAPTURE_BUFFER_BASE_ADDR            (0x904BC000)
#define YUV_CAPTURE_BUFFER_SIZE                 (240*432*2)

#define VIDEO_FRAME_BACK_BUFFER_UV_ADDR_0        (0x904F0000)
#define VIDEO_FRAME_BACK_BUFFER_UV0_SIZE         (512*432/2)

#define VIDEO_FRAME_BACK_BUFFER_UV_ADDR_1         (0x90510000)
#define VIDEO_FRAME_BACK_BUFFER_UV1_SIZE         (512*432/2)

#define	OSD_BUFFER_BASE_ADDR  					(0x90530000)   
#define	OSD_BUFFER_MAX_SIZE   					(832*1024)

//DEMODULATOR
//DEMODULATOR 0x90600000-0x90FFFFFF 10M Bytes
#define	DEMODULATOR_BUFFER_BASE_ADDR  			(0x90600000)   
#define	DEMODULATOR_BUFFER_MAX_SIZE   			(10*1024*1024)

#ifdef _SC6600V_WQVGA_

#define MP4_STREAM_DESQ_BUFFER_BASE_ADDR          (0x10000E00)
#define MP4_STREAM_DESQ_BUFFER_MAX_SIZE                (0x100)

#define MP4_STREAM_BUFFER_BASE_ADDR               (0x90100000)
#define	MP4_STREAM_BUFFER_MAX_SIZE   				(256*1024)

#define MP4_CAPTURE_BUFFER_BASE_ADDR              (0x90140000)
#define	MP4_CAPTURE_BUFFER_MAX_SIZE   				(256*1024)

#define MP4_DECODER_BUFFER_BASE_ADDR              (0x90200000)
#define	MP4_DECODER_BUFFER_MAX_SIZE   			 (2*1024*1024)


#define	ROTATION_BUFFER_BASE_ADDR  				(0x90450000)   
#define	ROTATION_BUFFER_MAX_SIZE   				(896*1024)

#define	OSD_BASE_BUFFER_ADDR  					(0x90530000)   
#define	OSD_BASE_BUFFER_MAX_SIZE   				(200*1024)

#define OSD_QUEUE_BUFFER_ADDR                   (0x90562000)
#define OSD_QUEUE_BUFFER_MAX_SIZE               (600*1024)

#define VIDEO_QUEUE_BUFFER_ADDR                 (0x90600000)
#define VIDEO_QUEUE_BUFFER_MAX_SIZE             (600*1024)

#define VIDEO_DISP_QUEUE_BUFFER_ADDR            (0x906a0000-12)
#define VIDEO_DISP_BUFFER_MAX_SIZE              (1000*1024)


#endif

#define SC6600V_PACKED 

#define SC6600V_ARM_CFG_ID			0x20080327

/*typedef enum GPIO_INT_tag
{
    GPIO_INT_DISABLE = 0,   // Disable the relative gpio interrupt.
    GPIO_INT_LEVEL_HIGH,    // detect high level.
    GPIO_INT_LEVEL_LOW,     // detect low level.
    GPIO_INT_EDGES_BOTH,    // detect the rising edges and falling edges.
    GPIO_INT_EDGES_RISING,  // detect the rising edges.   
    GPIO_INT_EDGES_FALLING  // detect the falling edges.
}GPIO_INT_TYPE;*/

typedef enum LCD_TYPE_CONFIG_tag
{
    /*
    if need add new lcd type here, 
    it must be added between LCD_TYPE_COMM_LCD and LCD_TYPE_MAX
    */
    LCD_TYPE_S6D0139 = 0,   
    LCD_TYPE_R61505MCU,    
    LCD_TYPE_R61505RGB,     
    LCD_TYPE_R61509MCU,
    LCD_TYPE_LGDP4531MCU,
    LCD_TYPE_ILI9325MCU,
    LCD_TYPE_COMM_LCD,
    LCD_TYPE_MAX
}LCD_TYPE_CONFIG_E;

#if 0
typedef enum IIS_MODE_tag
{
    IIS_MODE_SLAVE = 0,
    IIS_MODE_MASTER
}IIS_MODE_E;

typedef enum AUDIO_MODE_tag
{
    AUDIO_MODE_SPI = 0,
    AUDIO_MODE_IIS
}AUDIO_MODE_E;

typedef struct iis_config
{
	IIS_MODE_E	mode;
	uint32		iis_clk;
	uint32		iis_port;
}IIS_CONFIG_T;
#endif
typedef struct{
	uint32 sc6600v_cfg_id;
    uint32 gpio_out_id;
    uint32 gpio_in_id;
    uint32 gpio_out_int_type;
    uint32 gpio_in_int_type;
    uint32 gpio_out_delay;
    LCD_TYPE_CONFIG_E lcd_type; 
    uint32 arm_log_uart_id;
    uint32 dsp_log_uart_id;
    uint32 arm_log_cfg;
    
    /* uart log rate */
    uint32 arm_log_uart_rate;

    /* SPI Config */
    uint32 arm_spi_port_id;
    BOOLEAN rx_shift_edge; //Write "1" Enable Rx data shift in at clock neg-edge
    BOOLEAN tx_shift_edge; //Write "1" Enable Tx data shift out at clock neg-edge
    uint8   tx_bit_length; //Transmit data bit number
    BOOLEAN lsb_first;     //Write "1" enable data transmit/receive from LSB
    uint32  freq;          //SPI bus clock
    IIS_CONFIG_T	iis_cfg;
    AUDIO_MODE_E audio_mode;
//#ifdef _SC6600V_WQVGA_
    BOOLEAN is_dc;         //1: dc application, 0:mp4 application
    BOOLEAN is_mix_audio_output; //0: output stereo; 1: output mixed
    BOOLEAN is_sc6600v_descramble;  //0 or 1: demode api mode
                                    //   1 - sc6600v descramble; 0 - host descramble
                                    //0xff: mtv api mode
    BOOLEAN res2;
//#endif 

}SC6600V_ARM_CONFIG_T;
SC6600V_ARM_CONFIG_T *SCI_GetSysCfg();

typedef SC6600V_PACKED struct{
    uint32 cmd_id;
    uint32 parameter0;
    uint32 parameter1;
    uint32 parameter2;
}CMDQ_BB_ARM_CMD_T;

typedef SC6600V_PACKED struct {
    uint32 resp_id;
    uint32 parameter0;
    uint32 parameter1;
    uint32 parameter2;
}CMDQ_BB_ARM_RESP_T;

typedef SC6600V_PACKED struct{
    uint16 cmd_id;
    uint16 parameter0;
    uint16 parameter1;
    uint16 parameter2;
    uint16 parameter3;
}CMDQ_DSP_ARM_CMD_T;

typedef SC6600V_PACKED struct {
    uint16 resp_id;
    uint16 parameter0;
    uint16 parameter1;
    uint16 parameter2;
    uint16 parameter3;
}CMDQ_DSP_ARM_RESP_T;


typedef SC6600V_PACKED struct {
	CMDQ_BB_ARM_CMD_T cmd_t_arr[CMD_BB_CMDQ_SIZE];
	uint32 command_queue_in;
	uint32 command_queue_out;

	CMDQ_BB_ARM_RESP_T resp_t_arr[CMD_BB_RESPQ_SIZE];
	uint32 response_queue_in;
	uint32 response_queue_out;
}CMDQ_BB_ARM_CMD_QUEUE_T;

typedef SC6600V_PACKED struct {
	uint32 cmd_msg[CMD_BB_CMDQ_SIZE][CMD_MESSAGE_LEN/4];
	uint32 resp_msg[CMD_BB_RESPQ_SIZE][CMD_MESSAGE_LEN/4];		
}CMDQ_BB_ARM_MSGQ_T;

typedef SC6600V_PACKED struct {
	CMDQ_DSP_ARM_CMD_T cmd_t_arr[CMD_DSP_CMDQ_SIZE];
	uint16 command_queue_in;
	uint16 command_queue_out;

	CMDQ_DSP_ARM_RESP_T resp_t_arr[CMD_DSP_RESPQ_SIZE];
	uint16 response_queue_in;
	uint16 response_queue_out;		
}CMDQ_DSP_ARM_CMD_QUEUE_T;

 
/*****************************AUDIO COMMAND DATA STRUCT******************/
typedef enum
{
    AUDIO_RESAMPLING_TYPE_NO = 0,
    AUDIO_RESAMPLING_TYPE_YES
}AUDIO_RESAMPLING_TYPE_E;

typedef enum
{
    AUDIO_STERE_TO_MONO_TYPE_NO = 0,
    AUDIO_STERE_TO_MONO_TYPE_YES
}AUDIO_STERE_TO_MONO_TYPE_E;

typedef enum
{
    AUDIO_STERE_TO_MONO_OUTPUT_TYPE_LEFT = 0,
    AUDIO_STERE_TO_MONO_OUTPUT_TYPE_RIGHT,
    AUDIO_STERE_TO_MONO_OUTPUT_TYPE_LEFT_AND_RIGHT
}AUDIO_STERE_TO_MONO_OUTPUT_TYPE_E;

typedef enum
{
    AUDIO_OUTPUT_TYPE_LRLR = 0,
    AUDIO_OUTPUT_TYPE_LLRR
}AUDIO_OUTPUT_TYPE_E;


typedef union _AUDIO_SET_OUTPUT_PARAM_U{
    struct AUDIO_SET_OUTPUT_PARAM_MAP {
        volatile uint  reserved        			:8;
        volatile uint  packed_format        		:3;	//bit [7:5]
        volatile uint	output_type				:1; //bit 4
        volatile uint	stere_to_mono_output	:2; //bit [3:2]
        volatile uint	stere_to_mono			:1; //bit 1
        volatile uint	resample_type			:1; //bit 0
        volatile uint16 resample_rate; 
        volatile uint8	orig_sample_rate_idx;		
        volatile uint8  codec_type;		
    }mBits;
    struct AUDIO_OUTPUT_PARAM{
        volatile uint32 param1;
        volatile uint16 param2;
    }dwValue;
}AUDIO_SET_OUTPUT_PARAM_U;


typedef union _AUDIO_DATA_REQUEST_U
{
	struct _audio_data_request_params
    {
		uint32 parameter0:16;
		uint32 parameter1:16;
    }audio_data_request_params_t;
	volatile uint32 dwValue ;	    
}AUDIO_DATA_REQUEST_U;

/*****************************DEMODULATOR COMMAND DATA STRUCT******************/
typedef enum
{
	BYTE_INTERLEAVE_MODE0 = 72,
	BYTE_INTERLEAVE_MODE1 = 144,
	BYTE_INTERLEAVE_MODE2 = 288,
	BYTE_INTERLEAVE_MODE3 = 108,
	BYTE_INTERLEAVE_MODE4 = 216,
	BYTE_INTERLEAVE_MODE5 = 432	
}STiMi_Byte_interleave_mode_E;


typedef struct
{
	uint32	Center_frequency;// 
	uint16	Bandwidth_mode;//=0: 8M mode; =1: 2M mode;
	uint16	Gain_ctrl_word;//
	uint16	Mux_frame_ID;// 
	uint16	RS_mode; //=0:TDMB mode; =1: RS(240,224); =2: RS(240,192); =3: RS(240,176);=4: RS(240,240), bupass mode
	//STiMi_Byte_interleave_mode_E	Byte_interleave_mode;
	uint16	Byte_interleave_mode;
	uint16	LDPC_mode; //=0: 1/2; =1: 3/4
	uint16	Mapping_mode; //=0:BPSK; =1:QPSK; =2:16QAM;
	uint16	Scramble_mode; //=0~7
	uint16	TS_number; //=0~10
	uint16	TS_start_index;
}STiMi_SLCH_Settings_S;

/*****************Demux Configuration in Share Memory Data Structure*************/

typedef struct {
    uint32    data_low_addr    ;
    uint32    data_up_addr     ;
    uint32    dscr_low_addr    ;
    uint32    dscr_up_addr     ;
}   DMUX_BUF_INDICATOR_S       ;

typedef struct {                //Only be configured when initialization
    uint32    data_low_addr    ;
    uint32    data_up_addr     ;
    uint32    data_wr_ptr      ;
    uint32    data_rd_ptr      ;
    uint32    dscr_low_addr    ;
    uint32    dscr_up_addr     ;
    uint32    dscr_wr_ptr      ;
    uint32    dscr_rd_ptr      ;
}   DMUX_FIFO_INDICATOR_S      ;

typedef struct {
    uint32    slot_en          :1  ;
    uint32    buf_idx_up       :7  ;        // < 32
    uint32    wr_tog           :1  ;        // Cannot be written when the slot is in use and demux is the writer
    uint32    wr_idx           :7  ;        // Cannot be written when the slot is in use and demux is the writer
    uint32    buf_idx_low      :8  ;        // < 32
    uint32    rd_tog           :1  ;        // Cannot be written when the slot is in use and demux is the reader
    uint32    rd_idx           :7  ;        // Cannot be written when the slot is in use and demux is the reader
}   DMUX_SLOT_CONF_S               ;

/*********************Demux Filter Configuration in Share Memory*******************/

typedef struct {                   //Used with MFH filter enable command
    uint8     match[6]             ;
    uint16    reserved0            ;
    uint8     equal_mask[6]        ;
    uint16    reserved1            ;
    uint8     unequal_mask[6]      ;
    uint16    reserved2            ;
}   DMUX_MFH_FILTER_CONF_S         ;

typedef struct {                   //Used with table filter enable command
    uint8     match[4]             ;
    uint8     unequal_mask[4]      ;
    uint8     equal_mask[4]        ;
    uint8     reserved[4]          ;
}   DMUX_TABLE_FILTER_CONF_S       ;

typedef struct {                   //Used with MSF filter enable command
    uint8     vid_para_match[9]    ;
    uint8     aud_para_match[7]    ;
    uint8     vid_para_mask[9]     ;
    uint8     aud_para_mask[7]     ;
}   DMUX_MSFH_FILTER_CONF_S        ;

typedef struct {                   //Used with data filter enable command
    uint8     reserved0            ;
    uint8     type_match           ;
    uint8     data_match[4]        ;
    uint16    reserved1            ;
    uint8     reserved2            ;
    uint8     type_mask            ;
    uint8     data_mask[4]         ;
    uint16    reserved3            ;
}   DMUX_DATA_FILTER_CONF_S        ;

/***************************Demux Command parameter Data Struct*************************/

typedef struct {
    uint16    mux_frame_sel        ;        //0 indicates TS0 MF, others 1
    uint16    filter_en            ;        //always set to 1
    uint16    unequal_filter_en    ;        //only use least significant bit
    uint16    equal_filter_en      ;        //only use least significant bit
}   DMUX_MFH_FILTER_ENABLE_CMD_S   ;

typedef struct {
    uint16    mux_frame_sel        ;        //0 indicates TS0 MF, others 1
    uint16    filter_en            ;        //always set to 1
    uint32    reserved             ;
}   DMUX_MFH_FILTER_DISABLE_CMD_S  ;

typedef struct {
    uint16    reserved             ;
    uint16    filter_sel           ;        //use least significant 8-bit
    uint16    unequal_filter_en    ;        //use least significant 8-bit
    uint16    equal_filter_en      ;        //use least significant 8-bit
}   DMUX_TABLE_FILTER_ENABLE_CMD_S ;

typedef struct {
    uint16    reserved0            ;
    uint16    filter_sel           ;        //use least significant 8-bit
    uint16    reserved1            ;
    uint16    reserved2            ;
}   DMUX_TABLE_FILTER_DISABLE_CMD_S;

typedef union demux_service_union_tag{  
    struct demux_service_tag{
        uint16    service_type    :3   ;
        uint16    service_num     :5   ;
        uint16    mf_id           :8   ;
    }demux_service;
    uint16 mBits;
}DEMUX_SERVICE_U;

typedef struct {
    DEMUX_SERVICE_U service;
    uint16    mux_subframe_id      ;        // < 16
    uint16    video_stream_id      ;        // < 8
    uint16    audio_stream_id      ;        // < 8 
}   DMUX_SET_PARAMS_CMD_S          ;

typedef struct {
    uint16    reserved0            ;
    uint16    filter_sel           ;        // always set to 1
    uint16    unequal_filter_en    ;
    uint16    reserved1            ;
}   DMUX_MSFH_FILTER_ENABLE_CMD_S  ;

typedef struct {
    uint16    reserved0            ;
    uint16    filter_sel           ;        // always set to 1
    uint16    reserved1            ;
    uint16    reserved2            ;
}   DMUX_MSFH_FILTER_DISABLE_CMD_S ;

typedef struct {
    uint32    reserved             ;
    uint32    stc                  ;
}   DMUX_STC_CMD_S                 ;

typedef struct {
    uint16    filter_id           ;
    uint16    data_info            ;
    uint16    equal_filter_en          ;
    uint16    unequal_filter_en            ;

}   DMUX_DATA_FILTER_ENABLE_CMD_S  ;

typedef struct {
    uint16    reserved0            ;
    uint16    filter_sel           ;
    uint16    reserved1            ;
    uint16    reserved2            ;
}   DMUX_DATA_FILTER_DISABLE_CMD_S ;

typedef struct {
    uint16    channel_frame_cnt    ;       // < 64
    uint16    slot_idx             ;       // < 8
    uint16    buffer_idx           ;       // < 32
    uint16    unit_num             ;
}   DMUX_DATA_RDY_CMD_S            ;

typedef struct {
    uint32    buffer_idx           ;       // < 32
    uint32    reserved             ;
}   DMUX_DATA_REQUEST_CMD_S        ;

/*****************************ARM COMMON COMMAND DATA STRUCT******************/
typedef enum
{
    VIDEO_DECODER_TYPE_H264 = 0,
    VIDEO_DECODER_TYPE_AVS
}VIDEO_DECODER_TYPE_E;

typedef enum
{
    AUDIO_DECODER_TYPE_MP3 = 0,
    AUDIO_DECODER_TYPE_AACLC,
    AUDIO_DECODER_TYPE_AACPLUS
}AUDIO_DECODER_TYPE_E;
 
/*****************************LCDC COMMAND DATA STRUCT************************/
#define LCDC_OSD_BUF_NUM  2
#define MAX_LCD_WIDTH  (240)
#define MAX_LCD_HEIGHT (432)

typedef enum
{
    LCDC_LAYER_VIDEO = 0,
    LCDC_LAYER_OSD
}LCDC_LAYER_E;

typedef enum
{
    LCDC_OSD_FORMAT_RGB565 = 0,
    LCDC_OSD_FORMAT_ARGB4444,
    LCDC_OSD_FORMAT_ARGB1555,
    LCDC_OSD_FORMAT_CLUT8
}LCDC_OSD_FORMAT_E;

typedef enum
{
    LCDC_LAYER_ORDER_VIDEO = 0,    //Video层在OSD层之上
    LCDC_LAYER_ORDER_OSD           //OSD层在Video层之上
}LCDC_LAYER_ORDER_E;

typedef enum
{
    LCDC_ANGLE_90 = 0,
    LCDC_ANGLE_270,
    LCDC_ANGLE_0
}LCDC_ANGLE_E;

typedef union lcdc_color_tag
{
    struct lcdc_color_map
    {
        volatile uint8 reserved;
        volatile uint8 red;
        volatile uint8 green;
        volatile uint8 blue;
    }mBits;
    volatile uint32 value;
}LCDC_COLOR_U;

typedef union lcdc_start_coordinate_tag
{
    struct lcdc_start_coordinate_map
    {
        volatile uint16 start_x;
        volatile uint16 start_y;
    }mBits;
    volatile uint32 value;
}LCDC_START_COORDINATE_U;

typedef union lcdc_end_coordinate_tag
{
    struct lcdc_end_coordinate_map
    {
        volatile uint16 end_x;
        volatile uint16 end_y;
    }mBits;
    volatile uint32 value;
}LCDC_END_COORDINATE_U;

typedef union lcdc_lcd_size_tag
{
    struct lcdc_lcd_size_map
    {
        volatile uint16 width;
        volatile uint16 height;
    }mBits;
    volatile uint32 value;
}LCDC_SIZE_U;

typedef struct lcdc_yuv2rgb_tag
{
    uint16 yuv2rgb_c0[4];   //数组中每个元素的低10位有效
    uint16 yuv2rgb_c1[4];   //数组中每个元素的低10位有效
    uint16 yuv2rgb_c2[4];   //数组中每个元素的低10位有效
}LCDC_YUV2RGB_T;

typedef struct lcdc_color_correction_tag
{
    int16 color_coeff_a[3];  //数组中每个元素的低12位有效
    int16 color_coeff_b[3];  //数组中每个元素的低12位有效
    int16 color_coeff_c[3];  //数组中每个元素的低12位有效
    int16 reserverd;
}LCDC_COLOR_CORRCTION_T;

#define LCDC_GAMMA_COEFFICIENT_NUM      (17)
#define LCDC_SCALLING_H_LUMA_NUM        (8)
#define LCDC_SCALLING_V_LUMA_NUM		(136)
#define LCDC_SCALLING_H_CHROMA_NUM	    (8)
#define LCDC_SCALLING_V_CHROMA_NUM	    (72)

typedef struct lcdc_video_scalling_tag
{
    uint8 h_luma_coeff_high[LCDC_SCALLING_H_LUMA_NUM];   //luma horizontal高位参数数组指针(8)
    uint32 h_luma_coeff_mid[LCDC_SCALLING_H_LUMA_NUM];    //luma horizontal中位参数数组指针(8)
    uint32 h_luma_coeff_low[LCDC_SCALLING_H_LUMA_NUM];    //luma horizontal低位参数数组指针(8)
    uint16 v_luma_coeff[LCDC_SCALLING_V_LUMA_NUM];        //luma vertical参数数组指针      (136)
    uint8 h_chroma_coeff_high[LCDC_SCALLING_H_CHROMA_NUM]; // (8)
    uint32  h_chroma_coeff_low[LCDC_SCALLING_H_CHROMA_NUM];  // (8)
    uint16 v_chroma_coeff[LCDC_SCALLING_V_CHROMA_NUM];      // (72)
}LCDC_VIDEO_SCALLING_T;

typedef struct lcdc_color_key_tag
{
    LCDC_COLOR_U colorkey_min;
    LCDC_COLOR_U colorkey_max;
}LCDC_COLOR_KEY_T;

typedef struct lcdc_cmd_spec_tag
{
    uint32                      reserved_0;
    LCDC_COLOR_U                back_color;
    uint32                      order;
    BOOLEAN                     color_correction_enable;
    BOOLEAN                     dithering_enable;
    uint8                       reserved_1;
    uint8                       reserved_2;
    LCDC_COLOR_CORRCTION_T      color_coeff;
    uint32                      *ex_ram_ptr;    //need space of 2*1024*1024 bytes in worst case.
                                                //64k bytes align
    uint32  *osd_base_queue_addr;
    uint32  *osd_pad_queue_addr;
#ifdef _SC6600V_WQVGA_
    uint32                      *rotate_buf_addr;
    uint32                      *video_queue_addr;
    uint32                      *video_disp_queue_addr;
    uint32                      *osd_base_addr;
    uint32                      *osd_queue_addr;
#endif    
}LCDC_CMD_SPEC_T;

typedef struct lcdc_cmd_osd_spec_tag
{
    LCDC_START_COORDINATE_U  lcd_start;
    LCDC_END_COORDINATE_U    lcd_end;
    LCDC_COLOR_KEY_T         colorkey;
    uint32                   osd_format;
    uint32                   *clut8_data_ptr;   //如果osd_format为CLUT8，则BB需在SDRAM中
                                                //划分一块256 WORD的空间，存放CLUT8 TABLE
                                                //当第一次设置或者CLUT8T TABL内容变化时
                                                //需将该字段设为CLUT8 TABLE首地址；否则，将该字段置为NULL。
    uint8                    alpha;             //layer alpha value
    BOOLEAN                  colorkey_enable;
    uint8                    reserved_0;
    uint8                    reserved_1;
}LCDC_CMD_OSD_SPEC_T;

typedef struct lcdc_cmd_video_spec_tag
{
    LCDC_SIZE_U             	video_size;
    LCDC_START_COORDINATE_U 	video_start;
    LCDC_END_COORDINATE_U   	video_end;
    LCDC_START_COORDINATE_U 	lcd_start;
    LCDC_END_COORDINATE_U   	lcd_end;
    LCDC_COLOR_KEY_T        	colorkey;
    LCDC_VIDEO_SCALLING_T       *scaler_coeff_ptr; 
    uint32 				        reserved_0;
    uint32                  	angle;
    BOOLEAN                 	deinterlace_enable;
    BOOLEAN                 	colorkey_enable;
    BOOLEAN                 	rotate_enable;
    BOOLEAN                 	gamma_enable;
    uint8                  		filter_tap;
    uint8                   	alpha;           //layer alpha value
    uint8                   	yuv2rgb_shift;
    uint8                  		reserved_2;
    LCDC_YUV2RGB_T              yuv2rgb_coeff;   
    uint32 autosize;
}LCDC_CMD_VIDEO_SPEC_T;


typedef struct lcdc_osd_pad_tag
{
    LCDC_START_COORDINATE_U start;
    LCDC_END_COORDINATE_U end;
    uint32 osd_buf[MAX_LCD_WIDTH*MAX_LCD_HEIGHT*2/4];
}LCDC_OSD_PAD_T;

typedef struct lcdc_osd_queue_tag
{
    uint32 write_pos;
    uint32 read_pos;
    LCDC_OSD_PAD_T osd_pad_t[LCDC_OSD_BUF_NUM];
}LCDC_OSD_QUEUE_T;
#ifdef _SC6600V_WQVGA_

#define LCDC_VIDEO_BUF_NUM  3
#define MAX_VIDEO_WIDTH     352
#define MAX_VIDEO_HEIGHT    288

typedef struct lcdc_video_queue_tag
{
    uint32 write_pos;
    uint32 read_pos;
    uint32 video_buf[LCDC_VIDEO_BUF_NUM][MAX_VIDEO_WIDTH*MAX_VIDEO_HEIGHT*2/4];
}LCDC_VIDEO_QUEUE_T;

#endif

#if 0
typedef enum
{
    LCD_CTRL_MODE_RGB = 0, 
    LCD_CTRL_MODE_MCU
}LCD_CTRL_MODE_E;

typedef enum
{
    LCD_MCU_SYNC_MODE_FMARK = 0, 
    LCD_MCU_SYNC_MODE_VSYNC
}LCD_MCU_SYNC_MODE_E;

typedef enum
{
    LCD_MCU_BUS_MODE_8080 = 0, 
    LCD_MCU_BUS_MODE_6800
}LCD_MCU_BUS_MODE_E;

typedef enum
{
    LCD_SIGNAL_POL_HIGH= 0,    //signal is valide when it's high level
    LCD_SIGNAL_POL_LOW         //signal is valide when it's low level
}LCD_SIGNAL_POL_E;

typedef enum
{
    LCD_MCU_DATA_TYPE_RGB565 = 0,
    LCD_MCU_DATA_TYPE_RGB666,
    LCD_MCU_DATA_TYPE_8,
    LCD_MCU_DATA_TYPE_9
}LCD_MCU_DATA_TYPE_E;
#endif

typedef enum
{
   LCD_RGB_OUT_SEQ_RB = 0,     //输出模式是RGB
   LCD_RGB_OUT_SEQ_BR          //输出模式是BGR
}LCD_RGB_OUT_SEQ_E;

typedef enum
{
   LCD_RGB_DATA_TYPE_24 = 0,
   LCD_RGB_DATA_TYPE_18,
   LCD_RGB_DATA_TYPE_16
}LCD_RGB_DATA_TYPE_E;

typedef enum
{
    LCD_RGB_OUT_CYCLE_1 = 0,    //one cycle per pixel
    LCD_RGB_OUT_CYCLE_3         //three cycle per pixel
}LCD_RGB_OUT_CYCLE_E;   


typedef enum
{
    LCDC_VIDEO_FETCHER_VIDEO = 0,
    LCDC_VIDEO_FETCHER_STATIC
}LCDC_VIDEO_FETCHER_E;

#if 0
typedef struct lcd_rgb_timing_param_tag
{
    uint8  bfw;             //begin of frame horizontal sync pluse-1, 
                            //that means, if you want 6, then bfw = 5
    uint8  efw;             //end of frame horizontal sync pluse-1
    uint8  vsw;             //vsync pluse width-1
    uint8  blw;             //begin of line pixel clock-1
    uint8  elw;             //end of line pixel clock-1
    uint8  hsw;             //horizontal sync pluse wait-1
    uint8  reserved_0;
    uint8  reserved_1;
}LCD_RGB_TIMING_PARAM_T;

typedef struct lcd_mcu_timing_param_tag
{
    uint16 vsync_width;     //vysnc width
    uint8  whpw;            //write high pulse width
    uint8  wlpw;            //write low pulse width
    uint8  wcss;            //write cs setup width
    uint8  rhpw;            //read high pulse width
    uint8  rlpw;            //read low pulse width
    uint8  rcss;            //read cs setup width
}LCD_MCU_TIMING_PARAM_T;

typedef struct lcd_mcu_mode_param_tag
{
    LCD_MCU_SYNC_MODE_E     sync_mode;
    LCD_MCU_BUS_MODE_E      bus_mode;
    LCD_SIGNAL_POL_E        vsync_pol;
    LCD_SIGNAL_POL_E        cd_cmd;    // 1(0): cd pin will be low(high)
                                       // when write cmd to lcd panel;
                                       // 1(0): cd pin will be high(low) 
                                       // when write data to lcd panel;
    LCD_MCU_DATA_TYPE_E     data_type;
}LCD_MCU_MODE_PARAM_T;

typedef struct lcd_rgb_mode_param_tag
{
    uint32        rb_seq;
    uint32        lcd_bpp;
    uint32        trans_cycle;
    uint32        enable_pol;
    uint32        vsync_pol;
    uint32        hsync_pol;
}LCD_RGB_MODE_PARAM_T;
typedef struct lcd_op_cell
{
    uint8  op_index;
    uint8  delay_time;
    uint16 reg_index;
    uint32 data;
}LCD_OP_CELL;

typedef struct lcd_com_cfg 
{
    LCD_CTRL_MODE_E        ctrl_mode;
    LCD_MCU_MODE_PARAM_T   mode_param;
    LCD_MCU_TIMING_PARAM_T timing_param;
    uint16 lcd_gamma_table[20];
    uint32 lcd_init_offset;
    uint32 lcd_enter_sleep_offset;
    uint32 lcd_set_contrast_offset;
    uint32 lcd_set_brightness_offset;
    uint32 lcd_set_display_window_offset;
    uint32 lcd_get_info_offset;
    uint32 lcd_get_set_frame_rate_offset;
    uint32 lcd_clear_offset;
    uint32 lcd_close_offset;
    uint32 lcd_open_offset;
    uint32 lcd_set_direction_offset;
    uint32 res1_offset;
    uint32 res2_offset;
    uint32 res3_offset;
}LCD_COM_CFG;
#endif
/*****************************VIDEO COMMAND DATA STRUCT************************/
typedef enum
{
    RADIO_PROTOCOL_TDMB = 0,
    RADIO_PROTOCOL_CMMB
}RADIO_PROTOCOL_E;

typedef enum
{
    ENCODER_PROTOCOL_AVS =0,
    ENCODER_PROTOCOL_H264
}ENCODER_PROTOCOL_E;

typedef enum
{
    AVS_RM52C = 0,
    AVS_RM52G,
    AVS_RM52G1,
    AVS_RM52J
}AVS_RM_E;

typedef enum
{
    FRATE_25FPS = 0,
    FRATE_30FPS,
    FRATE_12_5FPS,
    FRATE_15FPS,
    FRATE_UNKNOW = 0xff
}FRAME_RATE_E;

typedef union video_para_set_u
{
    struct video_parameter_set
    {
	    uint32    res			            :11;
	    uint32    refresh_rate	            :8;
	    uint32    frame_rate		        :8; 
	    uint32    avs_version		        :3;
	    uint32    encoder_protocol		    :1;
	    uint32    radio_protocol		    :1;

    }mBits;
   uint32 value;
}VIDEO_PARA_SET_U;

typedef struct video_init_para{
	VIDEO_PARA_SET_U	para;
    uint32    			reserve[2];
}VIDEO_INIT_PARA_T;

typedef struct video_para_set{
    uint32    frame_rate		    :8; 
    uint32    channel_frame_counter	:8;
    uint32    refresh_rate			:8;
    uint32    res			        :8; 
    uint32    address;
    uint32    length;
}VIDEO_PARA_SET_T;

typedef struct video_attribute{
    uint32   frame_rate    	    :8;
    uint32   picture_width	    :12;
    uint32   picture_height     :12;
}VIDEO_ATTRIBUTE_T;

#ifdef _SC6600V_WQVGA_

#define ES_DESC_SIZE			8
#define ES_DESC_QUEUE_BASE		MP4_STREAM_DESQ_BUFFER_BASE_ADDR

#define MPEG4_DEC_EXTERNAL_BUFFER_BASE		0x90800000
#define MPEG4_DEC_EXTERNAL_BUFFER_SIZE		0x100000

typedef  struct {
	uint32	PTS;           		//数据时间戳
	uint32	DataAddr;  			//数据的起始地址，byte地址
	uint32	PTSF:1;     		//时间戳字段是否有效
	uint32	Resync:1;   		//忽略接下来收到的数据，找下一个I帧开始解码
	uint32	DataLength:30;		//数据长度，字节数
	uint32	reserved;			
}Es_Desc_T;

typedef SC6600V_PACKED struct {
	Es_Desc_T 	es_desc[ES_DESC_SIZE];
	uint32 		desc_queue_in;
	uint32 		desc_queue_out;
}ES_DESC_QUEUE_T;

typedef struct{
	uint16 imgWidth;
	uint16 imgHeight;
	uint16 frameRate;
	uint16 bInfoValid;
}VIDEO_INFO_T;
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif/* _CMD_COMMON_H_  */

/* End Of File  */
