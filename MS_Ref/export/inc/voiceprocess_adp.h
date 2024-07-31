/******************************************************************************
** File Name:      voice_adp.h                                              
** Author:         shujing.dong                                              
** DATE:           12/13/2005                                                
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of voice_adp                     
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 12/13/2005     shujing.dong     Create.        
** 04/30/2008      Binggo.Zhou     Modify For Audio Service 2.0                                 *
******************************************************************************/
#ifndef _VOICEPROCESS_ADP_H
#define _VOICEPROCESS_ADP_H
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
#include "asm.h"
#include "audio_config.h"
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
#define VOICE_DUMP_PCM_DATA_POSITION_TOTAL_COUNT 10	
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **   downlink data process like voice boost:                                 **
 **---------------------------------------------------------------------------**/
// definition of downlink callback function to config
typedef int(*VOICE_DOWNLINK_PROCESS_CONFIG)(int16 *in,int16 *noise, int16 *out); 
// definition of downlink callback function to execution
typedef int(*VOICE_DOWNLINK_PROCESS_EXP)(void); 
// input parameter of extend operation of downlink data process
typedef struct 
{ 
        VOICE_DOWNLINK_PROCESS_CONFIG downlink_pro_config; 
        VOICE_DOWNLINK_PROCESS_EXP  downlink_pro_exe; 
}VOICE_DOWNLINK_PROCESS_T;

/**---------------------------------------------------------------------------**
 **   uplink data process like nxp :                                          **
 **---------------------------------------------------------------------------**/
// definition of uplink callback function to config
typedef int(*VOICE_UPLINK_PROCESS_CONFIG)(int16 *in1, int16 *in2, int16 *out); 
// definition of uplink callback function to execution
typedef int(*VOICE_UPLINK_PROCESS_EXP)(void); 

// input parameter of extend operation of uplink data process
typedef struct 
{ 
        VOICE_UPLINK_PROCESS_CONFIG uplink_pro_config; 
        VOICE_UPLINK_PROCESS_EXP  uplink_pro_exe; 
}VOICE_UPLINK_PROCESS_T;

/**---------------------------------------------------------------------------**
 **  set if it is in loopback, default be not in loopback                     **
 **---------------------------------------------------------------------------**/
//ANSI2UINT16("VOICE_IF_LOOPBACK");
typedef struct 
{ 
	BOOLEAN bIsLoopback; 
}VOICE_IF_LOOPBACK_T;


/**---------------------------------------------------------------------------**
 **  save pcm data of 4 positions to files                                    **
 **  position 1:  dl original pcm data:                                       ** 
 **			pcm data from net and transfered to arm from dsp                  **
 **  position 2:  dl handled pcm data                                         ** 
 **			pcm data handled by arm and transfered to dsp and then to da      **
 **  position 3:  ul handled pcm daa                                          **
 **			pcm data handled by arm and transfered to dsp and then to net     **
 **  position 4:  ul original pcm data                                        **
 **			pcm data from ad and transfered to arm from dsp                   ** 
 **  note: the extend operations as below can be executed before or after     **
 **        playing.
 **         ANSI2UINT16("VOICE_EnableRecordDebugData");
 **---------------------------------------------------------------------------**/
//ANSI2UINT16("VOICE_EnableRecordDebugData");
//argc: 
#define VOICE_ENABLE_RECORD_DEBUG_DATA_PARAM_COUNT 3
//argv: structure of
typedef struct 
{ 
	//bEnable:SCI_TRUE: start to record pcm dataof some position
	//bEnable:SCI_FALSE: stop recording pcm data of some position
	BOOLEAN bEnable;	
	//uiRecordPosIndex = 0, enable/disable recording pcm data of dl original 
	//uiRecordPosIndex = 1, enable/disable recording pcm data of dl handled
	//uiRecordPosIndex = 2, enable/disable recording pcm data of ul handled
	//uiRecordPosIndex = 3, enable/disable recording pcm data of ul original
	uint32 uiRecordPos;//value range:0-3
	uint8  *pucDebugFileName;//[AUDIO_SFS_DEVICE_LEN];
}VOICE_ENALBE_RECORD_DEBUG_DATA_T;

/**---------------------------------------------------------------------------**
 **  this extend operation is used to dump voice pcm data in arm  in modem project                **
1)voice audio handle
2)the samples send to ap once. And its least value is 160 and it has to be the tiple of 160.
3)the cach frame count, the size of one cach is the one the samples sent to ap once. 
The count is 2 at least. Because codec can't confirm when codec sends pcm data to ap, 
How long codec needs to wait to reuse the cach again. 
4)the callback function, which is used to send data to ap once the size of the data sent from 
dsp is equal to  the samples you demands by the first parameter.
**  note: the extend operations as below have to be executed before playing    
**---------------------------------------------------------------------------**/
 //ANSI2UINT16("VOICE_DUMP_PCM_DATA_CONFIG");
 #define VOICE_CODEC_CACH_FRAME_COUNT_MIN 2
typedef void (*VOICE_DUMP_PCM_DATA_CALLBACK)(
    uint32 uiPositionIndex,
    int16 *psBuf, 
    uint32 uiLength //its unit is bytes.
    );
typedef struct
{
	uint32 uiSampleOnce;
	uint32 uiCachFrameCount;
	VOICE_DUMP_PCM_DATA_CALLBACK pDumpPcmDataCallbackFunc;		
}VOICE_CODEC_EXT_OPE_CONFIG_DUMP_PCM_DATA_PARAM_T;


/*
sample code:
#include "voice_adp.h" 

1.create voice audio handle:hAudioHandle by AUDIO_CreateNormalHandle;

2.
1)
VOICE_DOWNLINK_PROCESS_T dlk_pro_info = (0);
VOICE_UPLINK_PROCESS_T ulk_pro_info = {0};
AUDIO_RESULT_E result = AUDIO_NO_ERROR;
//set the value of the element like this:
dlk_pro_info.downlink_pro_config = AppDownlinkConfig;
dlk_pro_info.downlink_pro_exe = AppDownlinkExe;

ulk_pro_info.uplink_pro_config = AppUplinkConfig;
ulk_pro_info.uplink_pro_exe = AppUplinkExe;

AUDIO_ExeCodecExtOpe(hAudioHandle, ANSI2UINT16("VOICE_DLK_PRO_REG"), 0, (void *)&dlk_pro_info, &result);
AUDIO_ExeCodecExtOpe(hAudioHandle, ANSI2UINT16("VOICE_ULK_PRO_REG"), 0, (void *)&ulk_pro_info, &result);

2)
//if loopback mode, please set like this;
//if not loopback mode, this sentence can not be set.
{
	VOICE_IF_LOOPBACK_T tLoopback = {0};
	tLoopback.bIsLoopback = SCI_TRUE;
	AUDIO_ExeCodecExtOpe(hAudioHandle, ANSI2UINT16("VOICE_IF_LOOPBACK"), 0, (void *)&tLoopback, &result);
}
3)
3.1)//in modem project: if hope to recording the pcm data of positon 0 and 1 to ap for debugging, do like:
3.1.1)//if this is modem project please execute this step first, then execute the step of 3.1.2).
{
    VOICE_CODEC_EXT_OPE_CONFIG_DUMP_PCM_DATA_PARAM_T tPara = {0};
    tPara.uiSampleOnce = 160*4;
    tPara.uiCachFrameCount = 3;
    tPara.pDumpPcmDataCallbackFunc = callback; //this is realized by app.
    AUDIO_ExeCodecExtOpe(hAudioHandle, ANSI2UINT16("VOICE_DUMP_PCM_DATA_CONFIG"), 0, (void *)&tPara, &result);
}
3.1.2)//
{
	VOICE_ENALBE_RECORD_DEBUG_DATA_T tEnableRecordDebugData = {0};
	
	tEnableRecordDebugData.bEnable = SCI_TRUE;
	tEnableRecordDebugData.uiRecordPos = 0;
	tEnableRecordDebugData.pucDebugFileName = PNULL;
		
	AUDIO_ExeCodecExtOpe(
		hAudioHandle, 
		ANSI2UINT16("VOICE_EnableRecordDebugData"), 
		VOICE_ENABLE_RECORD_DEBUG_DATA_PARAM_COUNT, 
		(void *)&tEnableRecordDebugData, 
		&result);

	tEnableRecordDebugData.bEnable = SCI_TRUE;
	tEnableRecordDebugData.uiRecordPos = 1;
	tEnableRecordDebugData.pucDebugFileName = PNULL;
	
	AUDIO_ExeCodecExtOpe(
		hAudioHandle, 
		ANSI2UINT16("VOICE_EnableRecordDebugData"), 
		VOICE_ENABLE_RECORD_DEBUG_DATA_PARAM_COUNT, 
		(void *)&tEnableRecordDebugData, 
		&result);	
}

3.2)//in non-modem project: if hope to recording the pcm data of positon 0 and 1 to files for debugging, do like:
{
	VOICE_ENALBE_RECORD_DEBUG_DATA_T tEnableRecordDebugData = {0};
	
	tEnableRecordDebugData.bEnable = SCI_TRUE;
	tEnableRecordDebugData.uiRecordPos = 0;
	tEnableRecordDebugData.pucDebugFileName = L"E:\\PCM\\testfile1.pcm";
		
	AUDIO_ExeCodecExtOpe(
		hAudioHandle, 
		ANSI2UINT16("VOICE_EnableRecordDebugData"), 
		VOICE_ENABLE_RECORD_DEBUG_DATA_PARAM_COUNT, 
		(void *)&tEnableRecordDebugData, 
		&result);

	tEnableRecordDebugData.bEnable = SCI_TRUE;
	tEnableRecordDebugData.uiRecordPos = 1;
	tEnableRecordDebugData.pucDebugFileName = L"E:\\PCM\\testfile2.pcm";
	
	AUDIO_ExeCodecExtOpe(
		hAudioHandle, 
		ANSI2UINT16("VOICE_EnableRecordDebugData"), 
		VOICE_ENABLE_RECORD_DEBUG_DATA_PARAM_COUNT, 
		(void *)&tEnableRecordDebugData, 
		&result);	
}

3.//play
AUDIO_Play(hAudioHandle, 0);   

1)
//if hope to stop recording the pcm data of positon 0, do like:
{
	VOICE_ENALBE_RECORD_DEBUG_DATA_T tEnableRecordDebugData = {0};
	tEnableRecordDebugData.bEnable = SCI_FALSE;
	tEnableRecordDebugData.uiRecordPos = 0;

	AUDIO_ExeCodecExtOpe(
		hAudioHandle, 
		ANSI2UINT16("VOICE_EnableRecordDebugData"), 
		VOICE_ENABLE_RECORD_DEBUG_DATA_PARAM_COUNT, 
		(void *)&tEnableRecordDebugData, 
		&result);
}
//if hope to stop recording the pcm data of positon 0 and 1, do like:
{
	VOICE_ENALBE_RECORD_DEBUG_DATA_T tEnableRecordDebugData = {0};
	tEnableRecordDebugData.bEnable = SCI_FALSE;
	tEnableRecordDebugData.uiRecordPos = 0;

	AUDIO_ExeCodecExtOpe(
		hAudioHandle, 
		ANSI2UINT16("VOICE_EnableRecordDebugData"), 
		VOICE_ENABLE_RECORD_DEBUG_DATA_PARAM_COUNT, 
		(void *)&tEnableRecordDebugData, 
		&result);

	tEnableRecordDebugData.bEnable = SCI_FALSE;
	tEnableRecordDebugData.uiRecordPos = 1;

	AUDIO_ExeCodecExtOpe(
		hAudioHandle, 
		ANSI2UINT16("VOICE_EnableRecordDebugData"), 
		VOICE_ENABLE_RECORD_DEBUG_DATA_PARAM_COUNT, 
		(void *)&tEnableRecordDebugData, 
		&result);
}

*/

/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/
	
	
/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/
	
/**---------------------------------------------------------------------------**
 **                          Function Declare                                 **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//  Description:    This function sent audio data to voice boost , which was processed by NXP Rx.
//  Author:          TH
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E Voice_SendDataToVoiceBoost(
    int16 *input_src_buf, //input source data to be processed.
    uint32 len //input buffer length unit by half-word
); 

/*****************************************************************************/
//  Description:    This function control the tx delay time before sending to dsp.
//  Author:          shujing.dong
//  Note:           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E VoiceProcess_Delay(uint16 *puiSrcData);

/*****************************************************************************/
//  Description:    This Function is used to initialize the audio server. 
//  Author:         Shujing.Dong
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC VoiceProcess_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo);          	
PUBLIC  void VoiceProcess_RegCallbackToGetPcmDataToDsp(RECORD_GETPCMDATA callback);
PUBLIC void VoiceProcess_GetDataFromUplink(uint16 *puiSrcData);
PUBLIC void VoiceProcess_SendDataToDownlink(uint16 *puiSrcData);
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _VOICEPROCESS_ADP_H

//end of file
