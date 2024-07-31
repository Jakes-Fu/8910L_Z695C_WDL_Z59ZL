/****************************************************************************
** File Name:      autotest_audio.c                                             *
** Author:                                                                   *
** Date:           06/12/2011                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    the file autotest audio     
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        xudong zhang           Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "audio_api.h"
#include "asm.h"
#include "wav_adp.h"
#include "dsp_codec_adp.h"
#include "audio_api.h"
#include "audio_prod.h"
#include "aud_gen.h"
#include "audio_config.h"
#include "adpcmrecord_adp.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define IDLE          0
#define DECODING      1
#define PAUSE         2

/*buffer size*/
#define AMR_BUF_SIZE   (160*75)
#define DATA_BUF_SIZE  (15840)  //(180*44*2)

#define PCM_BUFFER_SIZE  (320)
#define MIC_GET_BUFFLEN  (1600*2) 

typedef enum
{
    AUD_INPUT_TYPE_MAINMIC,
    AUD_INPUT_TYPE_AUXMIC,
    AUD_INPUT_TYPE_DUALMIC,
    AUD_INPUT_TYPE_HPMIC_USE_L,
    AUD_INPUT_TYPE_HPMIC_USE_R,
} AUD_INPUT_TYPE_T;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern uint32 VBDecodeOuputBuf[];
extern void LAYER1_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length );


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*******************************************************************************
 *  type declare
 *******************************************************************************/
typedef struct
{
    HAUDIO          pcm_handle;				/*device handle*/
    int              i_state;
}pcm_dec_ctx_t;

/*******************************************************************************
 *  static variable
 *******************************************************************************/
//static uint32 long_offset_source=0;
LOCAL uint32 source_size=0;
LOCAL pcm_dec_ctx_t s_dec_ctx = {0};
LOCAL WAV_STREAM_BUFFER_T autotest_wav_header_info = {0};
LOCAL uint16 *pcm_data = PNULL;
LOCAL uint32 buff_alloc_flag = 0;
LOCAL SFS_HANDLE s_catmv_file_handle = SCI_NULL;
LOCAL HAUDIO hDspMp4Record = NULL; 
LOCAL uint32 pcm_current = 0;
LOCAL uint32 pcm_file_len = 0;
LOCAL uint32 autotest_mic_flag = 0;
LOCAL uint16 *MIC_pcm_temp = NULL;
LOCAL uint16 *MIC_pcm_data = NULL;
LOCAL HAUDIO hDspRecord = NULL;
/*xin.xu1 BBAT Test*/
LOCAL BOOLEAN bAudioDeviceUsedFlag = SCI_FALSE;
#define DEVICE_MODE_INVALIDE (0xFF)
uint8 ucDeviceMode = DEVICE_MODE_INVALIDE;

LOCAL BOOLEAN AudioDeviceGetUsedFlag(void)
{
    return bAudioDeviceUsedFlag;
}

LOCAL void AudioDeviceSetUsedFlag(BOOLEAN bUsedFlag)
{
    bAudioDeviceUsedFlag = bUsedFlag;
}

#ifdef CHIP_ENDIAN_LITTLE 
const unsigned char sin1k_44k[] = {
0x92,0x02,0xcb,0x0b,0xd0,0x14,0x1d,0x1d,0xfc,0x24,0x17,0x2c,0x4a,0x32,0x69,0x37,  
0x92,0x3b,0x4e,0x3e,0x22,0x40,0x56,0x40,0x92,0x3f,0x12,0x3d,0x88,0x39,0x10,0x35,  
0xf0,0x2e,0x51,0x28,0xce,0x20,0x7f,0x18,0xd5,0x0f,0xdA,0x06,0xdf,0xfd,0xa4,0xf4,  
0xa2,0xeb,0x39,0xe3,0x57,0xdb,0x3d,0xd4,0x1f,0xce,0xe2,0xc8,0xb1,0xc4,0xc0,0xc1,  
0xec,0xbf,0xc1,0xbf,0xa4,0xc0,0xf2,0xc2,0x18,0xc6,0xc2,0xca,0xc8,0xd0,0x36,0xd7,  
0xbb,0xde,0xe6,0xe6,0xa5,0xef,0xa6,0xf8};                                           
#else
const unsigned char sin1k_44k[] = {
0x02,0x92,0x0b,0xcb,0x14,0xd0,0x1d,0x1d,0x24,0xfc,0x2c,0x17,0x32,0x4a,0x37,0x69,
0x3b,0x92,0x3e,0x4e,0x40,0x22,0x40,0x56,0x3f,0x92,0x3d,0x12,0x39,0x88,0x35,0x10,
0x2e,0xf0,0x28,0x51,0x20,0xce,0x18,0x7f,0x0f,0xd5,0x06,0xdA,0xfd,0xdf,0xf4,0xa4,
0xeb,0xa2,0xe3,0x39,0xdb,0x57,0xd4,0x3d,0xce,0x1f,0xc8,0xe2,0xc4,0xb1,0xc1,0xc0,
0xbf,0xec,0xbf,0xc1,0xc0,0xa4,0xc2,0xf2,0xc6,0x18,0xca,0xc2,0xd0,0xc8,0xd7,0x36,
0xde,0xbb,0xe6,0xe6,0xef,0xa5,0xf8,0xa6};
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 extern void ARMVB_TestModeStart_Direct_Pcm (AUDIO_DEVICE_MODE_TYPE_E eMode, AUD_INPUT_TYPE_T input_type, uint32 uiVol, BOOLEAN start);

BOOLEAN AutoTestLoopDirectPcm(uint8 xx, uint8 yy, BOOLEAN bStart)
{
    static uint8 staticxx = 0xff;
    static uint8 staticyy = 0xff;
    AUDIO_DEVICE_MODE_TYPE_E eMode;
    AUD_INPUT_TYPE_T input_type;

    if(!bStart && (staticxx == 0xff) && (staticyy == 0xff))
    {
        return SCI_FALSE;
    }

    if(bStart && (staticxx != 0xff) && (staticyy != 0xff))
    {
        return SCI_FALSE;
    }

    if(!bStart)
    {
        xx = staticxx;
        yy = staticyy;
    }

    switch(xx)
    {
        case 0:
            input_type = AUD_INPUT_TYPE_MAINMIC;
            break;
        case 1:
            input_type = AUD_INPUT_TYPE_HPMIC_USE_L;
            break;
        default:
            return SCI_FALSE;
    }

    switch(yy)
    {
        case 0:
            eMode = AUDIO_DEVICE_MODE_HANDFREE;
            break;
        case 1:
            eMode = AUDIO_DEVICE_MODE_HANDHOLD;
            break;
        case 2:
            eMode = AUDIO_DEVICE_MODE_EARPHONE;
            break;
        default:
            return SCI_FALSE;
    }
#if 0
    if((AUD_INPUT_TYPE_MAINMIC == input_type) && (AUDIO_DEVICE_MODE_EARPHONE == eMode))
    {
        /*Not Support*/
        return SCI_FALSE;
    }

    if((AUD_INPUT_TYPE_HPMIC_USE_L == input_type) && (AUDIO_DEVICE_MODE_EARPHONE != eMode))
    {
        /*Not Support*/
        return SCI_FALSE;
    }
#endif
    ARMVB_TestModeStart_Direct_Pcm(eMode,input_type,0x09,bStart);
    if(!bStart)
    {
        staticxx = 0xff;
        staticyy = 0xff;
    }
    else
    {
        staticxx = xx;
        staticyy = yy;
    }
    
    return SCI_TRUE;
}

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
PUBLIC void *AUTOTEST_Mem16Cpy(      // None
    uint8  *dest_ptr,          // New buffer
    const uint8 *src_ptr,      // Buffer to copy from
    uint16  count               // Number of characters in bytes to copy
    )
{
    uint16  i         = 0;
    uint8  *temp_ptr = dest_ptr;

    SCI_ASSERT(SCI_NULL != dest_ptr);
    SCI_ASSERT(0 == (((uint32) dest_ptr) % 2));

    for (i = 0; i < count; i++)
    {
	    *dest_ptr++ = *src_ptr++;
    }

    return temp_ptr;
}

/*****************************************************************************/
//  Description :call back function for get pcm data
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/

LOCAL AUDIO_STREAM_GET_SRC_RESULT_E Speaker_callback(    
HAUDIO hAudio, 
AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
void *pvOtherInfo)
{
     //SCI_TRACE_LOW("[AUTOTEST] callback");    
	if (s_dec_ctx.i_state != DECODING)
	{
		SCI_TRACE_LOW("[AUTOTEST] callback, not in palying status.");         
		return AUDIO_STREAM_GET_SRC_ERROR;
	}	

    ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = source_size;
    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr =(uint8*)pcm_data;
    ptSrcDataInfo->tStreamCurSrcDataInfo.tOffsetTimeInfo.eOffsetType = AUDIO_SRCDATA_INFO_OFFSET_TIME;
    ptSrcDataInfo->tStreamCurSrcDataInfo.tOffsetTimeInfo.uOffsetTime.uiCurTime = 0;

	return AUDIO_STREAM_GET_SRC_SUCCESS;    
}


 static void amr_dec_dummy_callbak(
	HAUDIO hAudio, 
	uint32 notify_info, 
	uint32 affix_info
)
{
       return;
}
 

/*****************************************************************************/
//  Description : API of start autotest stream play
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int Speaker_Play_Start(void)
{

	SCI_TRACE_LOW("[AUTOTEST] Speaker_Play_Start,start");

	if(s_dec_ctx.i_state == DECODING)
	{
		return TRUE;
	}
	s_dec_ctx.i_state = DECODING;
	AUDIO_Play(s_dec_ctx.pcm_handle, 0);
	return TRUE;
}


/*****************************************************************************/
//  Description : API of start AMR stream open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/


static int Speaker_Play_Open()
{
    WAV_STREAM_FORMATINFO_T* wav_format_info_ptr = PNULL;	
    SCI_TRACE_LOW("[AUTOTEST] Speaker_Play_Open open");
	if (s_dec_ctx.i_state == DECODING)
	{
		SCI_TRACE_LOW("[AUTOTEST] Speaker_Play_Open open, last operation first not stop");
		return SCI_ERROR;
	}	

     autotest_wav_header_info.bType                   = SCI_TRUE;
     wav_format_info_ptr = &(autotest_wav_header_info.unWavStreamData.tWavFormatInfo);

      wav_format_info_ptr->eSubtype = WAV_PCM;
      wav_format_info_ptr->uiBitPerSample  = 16;
      wav_format_info_ptr->uiBlockAlign      =  2;
      wav_format_info_ptr->uiChannelNum   =  1;
      wav_format_info_ptr->uiDataSize         = DATA_BUF_SIZE;
      wav_format_info_ptr->uiSampleRate    = 44100;	 
    //SCI_Sleep(100);  //conflict with key voice!
	 s_dec_ctx.pcm_handle = AUDIO_CreateStreamBufferHandle(
								        hWAVCodec,
								        PNULL,
								        hARMVB,
								        PNULL,
								        (uint8 *)&autotest_wav_header_info,
								        sizeof(WAV_STREAM_BUFFER_T),
								        (uint32 *)(VBDecodeOuputBuf),
								        AMR_BUF_SIZE,
								        amr_dec_dummy_callbak,
								        Speaker_callback
								        );
    if ( !s_dec_ctx.pcm_handle)
    {
     	SCI_TRACE_LOW("[AUTOTEST] Speaker_Play_Open open(), create handle fail!");
    	return SCI_ERROR;
    }
           
	return TRUE;
}

LOCAL uint32 Autotest_Databuf_Set( uint8 *src,uint32 datalen)
{
    uint8 *src_ptr;
    uint8 *dest_ptr;
    uint32 need_to_copy;
    uint32 *pdatalen = NULL;

    SCI_ASSERT(PNULL != src);
    //SCI_ASSERT(0 <= datalen);//coverirty warning: unsigned datalen is always  biger-than-or-equal-to-zero 

    if(buff_alloc_flag != 1)
    {
        pcm_data = (uint16 *)SCI_ALLOC(DATA_BUF_SIZE+sizeof(uint32));
        if(PNULL == pcm_data)
        {     	
            return 0;
        }     	
        buff_alloc_flag = 1;
    }
    else
    {
        return 0;
    }
    pdatalen = (uint32 *)pcm_data;
    *pdatalen = 88 * 8;
    dest_ptr = (uint8 *)((uint32)pcm_data+sizeof(uint32));

#if 0

    SCI_MEMCPY((uint8 *)dest_ptr, (uint8*)pcmdata3, *pdatalen);

#else
    
    src_ptr = src;
    need_to_copy = *pdatalen;

    while(need_to_copy >= datalen)
    {
        AUTOTEST_Mem16Cpy(dest_ptr, src_ptr, datalen);
        dest_ptr += datalen;
        need_to_copy -= datalen;			
    }
    if(need_to_copy > 0)
    {
        AUTOTEST_Mem16Cpy(dest_ptr, src_ptr, need_to_copy);
    }
#endif
    return 1;
}
/*****************************************************************************/
//  Description : API of  AMR play demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC uint32 Autotest_Speaker_Start( uint16 *srcdata, uint32 datalen,uint8 dev_mode)
{
    SCI_TRACE_LOW("[AUTOTEST] Autotest_Speaker_Start start");
    
    if(PNULL == srcdata) return 0;
	if(datalen == 0) return 0;	
    
	if( 1 == Autotest_Databuf_Set(srcdata,datalen>>1))
	{
		source_size = DATA_BUF_SIZE;		
	}
	else
	{
		return 0;
	}


	switch(dev_mode)
	{
	    case 0x00:
			AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
		break;
		case 0x01:
			AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDHOLD);
		break;
		case 0x02:
			AUDIO_SetDevMode(AUDIO_DEVICE_MODE_EARPHONE);
		break;
		default:
			AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
		break;		
	}
    //AUDIO_SetVolume(1);
    Speaker_Play_Open();	
	Speaker_Play_Start();	
	return 1;
}
extern AUDIO_RESULT_E bbat_ARMVB_Play (AUDIO_DEVICE_MODE_TYPE_E eDev_mode, void *pPcmStream);
extern AUDIO_RESULT_E bbat_ARMVB_Stop (AUDIO_DEVICE_MODE_TYPE_E eDev_mode);

PUBLIC uint32 Autotest_Speaker_Start_Local(uint8 dev_mode)
{
    AUDIO_DEVICE_MODE_TYPE_E eDevMode = AUDIO_DEVICE_MODE_HANDHOLD;

    SCI_TRACE_LOW("[AUTOTEST] Autotest_Speaker_Start start");

	if( 1 == Autotest_Databuf_Set((uint16*)sin1k_44k,44))
	{
		source_size = DATA_BUF_SIZE;
	}
	else
	{
		return 0;
	}


    
    switch(dev_mode)
    {
        case 0x00:
            eDevMode = (AUDIO_DEVICE_MODE_HANDFREE);
        break;
        case 0x01:
            eDevMode = (AUDIO_DEVICE_MODE_HANDHOLD);
        break;
        case 0x02:
            eDevMode = (AUDIO_DEVICE_MODE_EARPHONE);
        break;
        default:
            eDevMode = (AUDIO_DEVICE_MODE_HANDFREE);
        break;		
    }
	
    //AUDIO_SetVolume(1);	
   // Speaker_Play_Open();	
	//Speaker_Play_Start();
    bbat_ARMVB_Play(eDevMode, (void *)pcm_data);
	
	return 1;
}

PUBLIC uint32 Autotest_rpc_Speaker_Start_local( uint8 *srcdata, uint32 datalen,uint8 dev_mode)
{
    AUDIO_DEVICE_MODE_TYPE_E eDevMode = AUDIO_DEVICE_MODE_HANDHOLD;

    switch(dev_mode)
    {
        case 0x00:
            eDevMode = (AUDIO_DEVICE_MODE_HANDFREE);
        break;
        case 0x01:
            eDevMode = (AUDIO_DEVICE_MODE_HANDHOLD);
        break;
        case 0x02:
            eDevMode = (AUDIO_DEVICE_MODE_EARPHONE);
        break;
        default:
            eDevMode = (AUDIO_DEVICE_MODE_HANDFREE);
        break;		
    }
    if( 1 == Autotest_Databuf_Set(srcdata,datalen))
    {
        source_size = DATA_BUF_SIZE;		
    }
    else
    {
        return 0;
    }
    bbat_ARMVB_Play(eDevMode, (void *)pcm_data);
    
    return 1;
}

PUBLIC uint32 Autotest_rpc_Speaker_Start( uint8 *srcdata, uint32 datalen,uint8 dev_mode)
{
    uint32 ret = 0;
    if(DEVICE_MODE_INVALIDE != ucDeviceMode)
    {
        return 0;
    }
    ret = Autotest_rpc_Speaker_Start_local(srcdata,datalen,dev_mode);
    if(ret == 1)
    {
        ucDeviceMode = dev_mode;
    }
    return ret;
}

PUBLIC void Autotest_rpc_Speaker_Stop_local(uint8 dev_mode)
{
    AUDIO_DEVICE_MODE_TYPE_E eDevMode = AUDIO_DEVICE_MODE_HANDHOLD;

    if (pcm_data == PNULL)
    {
        return;
    }
    switch(dev_mode)
    {
        case 0x00:
            eDevMode = (AUDIO_DEVICE_MODE_HANDFREE);
        break;
        case 0x01:
            eDevMode = (AUDIO_DEVICE_MODE_HANDHOLD);
        break;
        case 0x02:
            eDevMode = (AUDIO_DEVICE_MODE_EARPHONE);
        break;
        default:
            eDevMode = (AUDIO_DEVICE_MODE_HANDFREE);
        break;		
    }
    bbat_ARMVB_Stop(eDevMode);
    
    if (pcm_data != PNULL)
    {
        SCI_FREE(pcm_data);
        pcm_data = PNULL;
        buff_alloc_flag = 0;
    }
   // return 1;
}

PUBLIC void Autotest_rpc_Speaker_Stop(uint8 dev_mode)
{
    if(DEVICE_MODE_INVALIDE == ucDeviceMode)
    {
        return;
    }

    if(dev_mode != ucDeviceMode)
    {
        return;
    }
    
    Autotest_rpc_Speaker_Stop_local(dev_mode);
    ucDeviceMode = DEVICE_MODE_INVALIDE;
}



/*****************************************************************************/
//  Description : API of  AMR stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void Autotest_Speaker_Stop(void)
{
	SCI_TRACE_LOW("[AUTOTEST] Autotest_Speaker_Stop close");

    if (pcm_data != PNULL)
    {
    SCI_FREE(pcm_data);
    pcm_data = PNULL;
	buff_alloc_flag = 0;
   	}
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
}

extern uint16 ARMVB_getRecordLenth(void);


static uint32 pcm_cap_read_data(uint32 buf_addr,uint32 buf_len,uint32 data_len)
{
    AUDIO_RESULT_E aud_result = AUDIO_NO_ERROR;
    uint32 sample_len_getted = 0;

    SCI_TRACE_LOW("[ljf] func[%s] line[%d] in buf_addr:0x%x, buf_len:%d, data_len:%d\n", __FUNCTION__, __LINE__, buf_addr, buf_len, data_len);
    if( INVALID_HANDLE == hDspRecord)
    {
        return 0;
    }


    {
        DSP_CODEC_EXT_OPE_VOICE_DATA_T ptPara;
        uint32 temp;

        ptPara.source_buf_ptr = (uint16 *)buf_addr;
        ptPara.data_length = (uint16)(data_len>>1);// 2bytes per sample.

        aud_result = AUDIO_ExeCodecExtOpe(hDspRecord, 
                        ANSI2UINT16("READ_VOICE_DATA"), 
                        DSP_CODEC_EXT_OPE_VOICE_DATA_PARA_COUNT, 
                        &ptPara,
                        &temp); 
        sample_len_getted = (AUDIO_NO_ERROR != aud_result)? 0 : (data_len>>1);
    }

    return (sample_len_getted<<1);
}


uint32 tmppcmlen = 0;

LOCAL void pcmenc_CallBack(uint16 i_len)
{	
	uint16 *p16_tmp = PNULL;
	uint16 *tmp = PNULL;

    uint32 write_point = 0;
    uint16 lenth = 0;
    uint32 pdata =tmppcmlen%pcm_file_len;
    
        SCI_TRACE_LOW("[AUTOTEST] pcmenc_CallBack: tmppcmlen = %d, pcm_file_len =%d",tmppcmlen,pcm_file_len);
//	SCI_ASSERT( 160 >= i_len ); 

    if(tmppcmlen/pcm_file_len)
    {
        autotest_mic_flag = 1;
    }


    lenth = ARMVB_getRecordLenth();
        SCI_TRACE_LOW("[AUTOTEST] pcmenc_CallBack:  ARMVB_getRecordLenth = %d",lenth);
    pcm_cap_read_data((uint32)MIC_pcm_data+pdata,pcm_file_len-pdata,lenth);
    tmppcmlen += lenth;
    //_dsp_codec_callback(lenth);
    return;

#if 0

		
	p16_tmp = MIC_pcm_temp;
	
	LAYER1_ReadVoiceData((uint16 *)p16_tmp, i_len);
	
	tmp = (uint16 *)(MIC_pcm_data + pcm_current);
	
	AUTOTEST_Mem16Cpy((uint16 *)tmp,(uint16 *)p16_tmp,i_len);
	
	if(pcm_current + i_len*2 <= pcm_file_len)
	{
		pcm_current = pcm_current + i_len;
	}
	else
	{
		autotest_mic_flag = 1;
		pcm_current = 0;
	}

#endif
}



static void AudioDummyNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}

void Audio_Dumpdata_callback1(int16 *psBuf, uint32 uLength)
{
    SCI_TRACE_LOW("[AUTOTEST] Audio_Dumpdata_callback1 uLength= %d",uLength);

    SCI_MEMCPY((uint8 *)MIC_pcm_data, (uint8*)psBuf, 1600*2);
		autotest_mic_flag = 1;
    return;
}

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                                 */
/*---------------------------------------------------------------------------*/
LOCAL AUDIO_RESULT_E AUD_EnableVoiceRecord(  // If succeed, return ERR_AUD_NONE,
                                        // else return error value
    BOOLEAN is_enable,                  // SCI_TRUE: Enable,   SCI_FALSE: Disable
    AUD_REC_FORMAT_E recorder_format,
    RECORD_CALLBACK callback_fun    
    )
{
    uint32 temp = 0;     
    DSP_CODEC_EXT_OPE_START_RECORD_T ptPara = {0}; 
    AUDIO_RESULT_E status = AUDIO_NO_ERROR;
    //RECORD_PARAM_T record_param1 = {AUD_REC_ADPCM, RECORD_OUTPUT_BUFFER, 0, 160, 2,1,0,16000,0};

    RECORD_UPLOAD_PARAM_T record_param = {AUD_REC_PCM, RECORD_OUTPUT_BUFFER,PNULL,0};
	static char *ptr = "bbat";

    if (is_enable)
    {      
        SCI_TRACE_LOW("[AUTOTEST] AUD_EnableVoiceRecord: &MIC_pcm_data[0] = 0x%x",MIC_pcm_data);
        if(!hDspRecord)
        {
            hDspRecord     = AUDIO_CreateRecordBufferHandle(hRecordCodec,  SCI_NULL, hARMVBRECORD,     
                SCI_NULL, MIC_pcm_data,pcm_file_len,NULL,NULL,amr_dec_dummy_callbak);
        }
        record_param.output_channel = RECORD_OUTPUT_ARM_STREAM;
        record_param.stream_upload_calback= Audio_Dumpdata_callback1;

        
       /* AUDIO_ExeCodecExtOpe(hDspRecord,
                ANSI2UINT16("SET_RECORDPARAM"),
                RECORD_PARAM_SET_COUNT,
                &record_param1, &temp);*/
        AUDIO_ExeCodecExtOpe(hDspRecord,
                ANSI2UINT16("SET_RECORDPARAM"),
                RECORD_PARAM_SET_STRAM_OUT_COUNT,
                &record_param, &temp);


        status = AUDIO_Play(hDspRecord,0);
    }
    else
    {
        if(hDspRecord)
        {
            status = AUDIO_Stop(hDspRecord);
            hDspRecord = 0;
         }
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


PUBLIC int Autotest_Mic_Start(uint8 dev_mode)
{
    SCI_TRACE_LOW("[AUTOTEST] AUTOTEST_MIC_Start: start reoced");
	pcm_current = 0;	
	MIC_pcm_temp = (uint16*)SCI_ALLOC(PCM_BUFFER_SIZE);
	MIC_pcm_data = (uint16*)SCI_ALLOC(MIC_GET_BUFFLEN);
	pcm_file_len = MIC_GET_BUFFLEN>>1;

    autotest_mic_flag = 0;
	
	switch(dev_mode)
	{
	    case 0x00:
		AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
		break;
		case 0x01:
		AUDIO_SetDevMode(AUDIO_DEVICE_MODE_EARPHONE);
		break;		
		default:
		AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
		break;		
	}
	
	AUD_EnableVoiceRecord(SCI_TRUE,
		AUD_REC_PCM,
		pcmenc_CallBack);	
	
    return SCI_TRUE;
}

PUBLIC int Autotest_Mic_Stop(void)
{
   SCI_TRACE_LOW("[AUTOTEST] : stop reoced");		
    Autotest_rpc_Speaker_Stop(ucDeviceMode);
    
   if(1 == autotest_mic_flag)
   {
       //stop_recode
    	AUD_EnableVoiceRecord(
    		SCI_FALSE,
    		AUD_REC_PCM,
    		NULL
    		);
    		//BBAT_DSPVB_PcmDumpStop();
	tmppcmlen = 0;
        autotest_mic_flag = 0;
        if(MIC_pcm_temp != PNULL)
        {
    	  SCI_FREE(MIC_pcm_temp);
    	  MIC_pcm_temp = PNULL;
        }	
        if(MIC_pcm_data != PNULL)
        {
    	  SCI_FREE(MIC_pcm_data);
    	  MIC_pcm_data = PNULL;
        }
	    return SCI_TRUE;
   }
   else
   {
        return SCI_FALSE;
   }
}

PUBLIC uint8 Autotest_GetMicState(void)
{
    return (uint8)autotest_mic_flag;
}

PUBLIC uint16* Autotest_GetDataAddr(void)
{
    return MIC_pcm_data;
}



