/****************************************************************************
** File Name:      pcm_record_demo.c                                                                  *
** Author:                                                                                                                     *
** Date:                                                                                                    *
** Copyright:                                     *
** Description:    the file PCM RECORD PLAY     
*****************************************************************************
**                         Important Edit History                                                                          *
** --------------------------------------------------------------------------*
** DATE            NAME             DESCRIPTION                                                                    *
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                                                                        *
 **--------------------------------------------------------------------------*/
#include "mmi_app_eng_trc.h"
#include "std_header.h"
//#include "sig_code.h"
#include "sfs.h"
#include "ffs.h"
#include "dsp_codec_adp.h"
#include "dal_audio.h"
#include "aud_gen.h"
#include "audio_api.h"
#include "asm.h"
#include "audio_config.h"
#include "wav_adp.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                                                            *
 **--------------------------------------------------------------------------*/
#define IDLE             0
#define DECODING         1
#define PAUSE            2

/*buffer size*/
#define PCM_RECORD_BUFFER_SIZE                  (1024*15)

#define PCM_DRIVER_BUF_SIZE        (80*150)

/*******************************************************************************
 *  type declare
 *******************************************************************************/
typedef struct 
{
    HAUDIO  pcm_handle; /*device handle*/
    int32  i_state;
} pcm_dec_ctx_t;

/*******************************************************************************
 *  static variable
 *******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                                                           *
 **--------------------------------------------------------------------------*/
LOCAL uint32 amr_count=0;
uint8 pcm_buffer[PCM_RECORD_BUFFER_SIZE];
LOCAL uint8 *pcm_datastream, *pcm_datastream_init;
LOCAL uint16 samr_temp[160];
LOCAL pcm_dec_ctx_t s_dec_ctx = {0};
LOCAL HAUDIO hPCMRecord;
uint8* pcm_demo_source = SCI_NULL;
LOCAL uint32 source_size=0;
LOCAL uint32 long_offset_source=0;
LOCAL WAV_STREAM_BUFFER_T wav_header_info = {0};
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                                                            *
 **--------------------------------------------------------------------------*/
extern uint32 VBDecodeOuputBuf[];
extern HAUDIODEV hARMVBDualChn;
extern HAUDIODEV hARMVBDualChnRecord;
extern uint16 GUI_GB2UCS(uint16 *, const uint8 *, uint16);
extern void WAV_Demo_Start(void);
extern void WAV_Demo_Stop(void);

/*****************************************************************************/
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void pcm_dec_dummy_callbak(
    HAUDIO hAudio, 
    uint32 notify_info, 
    uint32 affix_info
)
{
       return; //do nothing
}

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                                 */
/*---------------------------------------------------------------------------*/
LOCAL AUDIO_RESULT_E AUD_EnableVoiceRecord(
    BOOLEAN is_enable,
    AUD_REC_FORMAT_E recorder_format,
    RECORD_CALLBACK callback_fun    
    )
{
    uint32 temp;
    AUDIO_RESULT_E status;
    DSP_CODEC_EXT_OPE_START_RECORD_T ptPara = {0}; 

    if (is_enable)
    {
        ptPara.recorder_format = recorder_format;
        ptPara.callback_fun = callback_fun;
        ptPara.hAudioHandle = hPCMRecord;

        status = AUDIO_ExeCodecExtOpe(hPCMRecord,
                        ANSI2UINT16("EXT_START_RECORD"),
                        DSP_CODEC_EXT_OPE_START_RECORD_PARA_COUNT,
                        &ptPara,
                        &temp);
    }
    else
    {
        ptPara.recorder_format = recorder_format;
        ptPara.callback_fun = callback_fun;
        ptPara.hAudioHandle = hPCMRecord;
        status = AUDIO_ExeCodecExtOpe(hPCMRecord, 
                        ANSI2UINT16("EXT_STOP_RECORD"), 
                        DSP_CODEC_EXT_OPE_START_RECORD_PARA_COUNT,
                        &ptPara,
                        &temp); 
    }

    if(AUDIO_NO_ERROR == status)
    {
        return AUDIO_NO_ERROR;
    }
    else
    {
        return AUDIO_ERROR;
    }
}


LOCAL void AUD_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length )
{
    DSP_CODEC_EXT_OPE_VOICE_DATA_T ptPara;
    uint32 temp;
    ptPara.source_buf_ptr = destination_buf_ptr;
    ptPara.data_length = data_length;
    
    AUDIO_ExeCodecExtOpe(hPCMRecord, 
                            ANSI2UINT16("READ_VOICE_DATA"), 
                            DSP_CODEC_EXT_OPE_VOICE_DATA_PARA_COUNT, 
                            &ptPara,
                            &temp); 
    return;
}



/*****************************************************************************/
//  Description : Create amr record file.
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PCM_REOCED_File(uint8 *data_ptr, uint32 data_len)
{
    uint8  file_name[40] = {0};
    uint16 ucs2_file_name[40] = {0};
    uint32 tmp = 0;
    SFS_HANDLE file_handle_r = NULL;

    sprintf((char*)file_name,"E:\\pcm_Record.pcm");

    GUI_GB2UCS(ucs2_file_name, file_name, (uint16)(sizeof(file_name)));

    file_handle_r = SFS_CreateFile((const uint16 *)ucs2_file_name, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE,NULL,NULL);
    if(file_handle_r > 0)
    {
        SFS_WriteFile(file_handle_r, data_ptr, data_len, (uint32*)&tmp, NULL);
    }
    SFS_CloseFile(file_handle_r);

    return;
}

/*****************************************************************************/
//  Description : Create record callback function.
//                for read data from codec.
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void pcmenc_CallBack(uint16 i_len)
{
    uint16 *p16_tmp;
    int32 i;

    AUD_ReadVoiceData((uint16 *)samr_temp, i_len);

    p16_tmp = (uint16 *)samr_temp;

    for (i=0; i<(i_len*2); i++)
    {
        *(pcm_datastream+i) = (uint8)(((uint8 *)p16_tmp)[i]);
    }
    pcm_datastream += (i_len*2);

    if ((pcm_datastream - pcm_datastream_init) > PCM_RECORD_BUFFER_SIZE)
    {
        pcm_datastream = pcm_datastream_init;
    }
    return;
}

/*****************************************************************************/
//  Description : API of start record 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void PCM_Record_Start(void)
{
    SCI_TRACE_LOW(" PCM_Record_Start start");
    pcm_datastream = pcm_buffer;
    pcm_datastream_init = pcm_buffer;

    hPCMRecord = AUDIO_CreateNormalHandle(hDSPCodec, SCI_NULL, hARMVBDualChnRecord, SCI_NULL, pcm_dec_dummy_callbak);
    //start record
    AUD_EnableVoiceRecord(SCI_TRUE, AUD_REC_PCM, pcmenc_CallBack);

    return;
}

/*****************************************************************************/
//  Description : API of stop record and save record data to a file
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void PCM_Record_Stop(void)
{
    SCI_TRACE_LOW(" PCM_Record_Stop start");
    //stop record
    AUD_EnableVoiceRecord(SCI_FALSE, AUD_REC_PCM, pcmenc_CallBack);
    AUDIO_CloseHandle(hPCMRecord);
    //save record data to file
    //PCM_REOCED_File((uint8 *)pcm_buffer, PCM_RECORD_BUFFER_SIZE);

    return;
}

LOCAL AUDIO_STREAM_GET_SRC_RESULT_E pcm_callback(
    HAUDIO hAudio, 
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo
    )
{
    uint32 puiDataLength;

    SCI_ASSERT(ptSrcDataInfo!=PNULL); /*assert verified*/

    //SCI_TRACE_LOW:"wav_dec, callback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_85_112_2_18_2_18_25_309,(uint8*)"");

    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = PNULL;
    puiDataLength = 0;

    if (s_dec_ctx.i_state != DECODING)
    {
        //SCI_TRACE_LOW:"wav_dec, callback, not in palying status."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_92_112_2_18_2_18_25_310,(uint8*)"");
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
        return AUDIO_STREAM_GET_SRC_ERROR;
    }

    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = (uint8 *)(pcm_demo_source+long_offset_source);	

    if(long_offset_source+2048 >source_size)
    {
        puiDataLength = source_size-long_offset_source;    
        long_offset_source=source_size; 
    }
    else
    {
        puiDataLength = 2048;
        long_offset_source+=2048;
    }

    if(long_offset_source >=source_size)
    {
        long_offset_source=0;

        if(puiDataLength==0)
        {
            ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = (uint8 *)(pcm_demo_source+long_offset_source);    
            puiDataLength =2048; 
            long_offset_source+=2048;	
        }
    }
    ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =puiDataLength;

    //SCI_TRACE_LOW:"wav_dec, i_len %d, offset=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_134_112_2_18_2_18_25_311,(uint8*)"dd", puiDataLength,long_offset_source );
    return AUDIO_STREAM_GET_SRC_SUCCESS;
}

/*****************************************************************************/
//  Description : API of start pcm stream play
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int PCM_Play_Start(void)
{
    //SCI_TRACE_LOW:"wav_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_155_112_2_18_2_18_25_312,(uint8*)"");

    if(s_dec_ctx.i_state == DECODING)
    {
        return 0;
    }
    s_dec_ctx.i_state = DECODING;
    AUDIO_Play(s_dec_ctx.pcm_handle, 0);
    return 0;
}

/*****************************************************************************/
//  Description : API of start pcm stream open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int PCM_Play_Open(void)
{

    WAV_STREAM_FORMATINFO_T* wav_format_info_ptr = PNULL;
    //SCI_TRACE_LOW:"wav_dec, open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_174_112_2_18_2_18_25_313,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        //SCI_TRACE_LOW:"wav_dec, open, last operation first not stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_178_112_2_18_2_18_25_314,(uint8*)"");
        return SCI_ERROR;
    }

    wav_header_info.bType  = SCI_TRUE;
    wav_format_info_ptr = &(wav_header_info.unWavStreamData.tWavFormatInfo);
    wav_format_info_ptr->eSubtype = WAV_PCM;
    wav_format_info_ptr->uiBitPerSample  = 16;
    wav_format_info_ptr->uiBlockAlign    = 4;
    wav_format_info_ptr->uiChannelNum    = 1;
    wav_format_info_ptr->uiDataSize      = PCM_RECORD_BUFFER_SIZE;
    wav_format_info_ptr->uiSampleRate    = 8000;

    //SCI_TRACE_LOW:"wav_dec, open, uiBitPerSample[%d],uiBlockAlign[%d],uiChannelNum[%d],uiDataSize[%d],uiSampleRate[%d],eSubtype[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_203_112_2_18_2_18_25_315,(uint8*)"dddddd",wav_format_info_ptr->uiBitPerSample, wav_format_info_ptr->uiBlockAlign, wav_format_info_ptr->uiChannelNum,wav_format_info_ptr->uiDataSize, wav_format_info_ptr->uiSampleRate, wav_format_info_ptr->eSubtype);

      s_dec_ctx.pcm_handle = AUDIO_CreateStreamBufferHandle(
                                                            hWAVCodec,
                                                            PNULL,
                                                            hARMVBDualChn,
                                                            PNULL,
                                                            (uint8 *)&wav_header_info,
                                                            sizeof(WAV_STREAM_BUFFER_T),
                                                            (uint32 *)(VBDecodeOuputBuf),
                                                            PCM_DRIVER_BUF_SIZE,
                                                            pcm_dec_dummy_callbak,
                                                            pcm_callback);

    if ( !s_dec_ctx.pcm_handle)
    {
        //SCI_TRACE_LOW:"wav_dec, open(), create handle fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_219_112_2_18_2_18_25_316,(uint8*)"");
        return SCI_ERROR;
    }

    return 0;
}

/*****************************************************************************/
//  Description : API of  pcm play demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void PCM_Demo_Start(void)
{
    //SCI_TRACE_LOW:"wav_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_231_112_2_18_2_18_25_317,(uint8*)"");
    pcm_demo_source = pcm_buffer;
    source_size=PCM_RECORD_BUFFER_SIZE; 

    memset(&s_dec_ctx, 0, sizeof(pcm_dec_ctx_t));

    long_offset_source=0;

    //SCI_TRACE_LOW:"wav_dec,source_len= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_242_112_2_18_2_18_25_318,(uint8*)"d",source_size);
    AUDIO_SetVolume(5);
    PCM_Play_Open();
    PCM_Play_Start();

    return;
}

/*****************************************************************************/
//  Description : API of  pcm stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void PCM_Demo_Stop(void)
{
    //SCI_TRACE_LOW:"wav_dec, close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_253_112_2_18_2_18_25_319,(uint8*)"");

    if (s_dec_ctx.i_state != IDLE)
    {
        AUDIO_Stop(s_dec_ctx.pcm_handle);
        s_dec_ctx.i_state = IDLE;
    }

    if (s_dec_ctx.pcm_handle)
    {
        AUDIO_CloseHandle(s_dec_ctx.pcm_handle);
        s_dec_ctx.pcm_handle = NULL;
    }

    return;
}

/*****************************************************************************/
//  Description : API of  pcm play and record demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void PCM_Play_Record_Start(void)
{
    WAV_Demo_Start();
    PCM_Record_Start();
    return;
}

/*****************************************************************************/
//  Description : API of  pcm play and record  demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void PCM_Play_Record_Stop(void)
{
    PCM_Record_Stop();
    WAV_Demo_Stop();
    return;
}
