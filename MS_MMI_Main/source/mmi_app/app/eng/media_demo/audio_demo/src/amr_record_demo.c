/****************************************************************************
** File Name:      _AMR_RECORD_DEMO_C_.c                                                                  *
** Author:                                                                                                                     *
** Date:           11/10/2010                                                                                            *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.                               *
** Description:    the file AMR STREM PLAY     
*****************************************************************************
**                         Important Edit History                                                                          *
** --------------------------------------------------------------------------*
** DATE            NAME             DESCRIPTION                                                                    *
** 10/2010        hai.li               Create
******************************************************************************/

#define _AMR_RECORD_DEMO_C_


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
//#include "amr_adp.h"
#include "vb_drv.h"
#include "audio_config.h"
#include "adpcmrecord_adp.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                                                            *
 **--------------------------------------------------------------------------*/
#define MAX_AMR_TESTBUF  32

#define IDLE             0
#define DECODING         1
#define PAUSE            2

/*buffer size*/
#define AMR_RECORD_BUFFER_SIZE                  (1024*15)
#define AMRSTRING_TRACKBUFSIZE_APP       (80 * 150)


/*******************************************************************************
 *  type declare
 *******************************************************************************/
typedef struct 
{
    HAUDIO  pcm_handle; /*device handle*/
    int32  i_state;
}amr_dec_ctx_t;

/*******************************************************************************
 *  static variable
 *******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                                                           *
 **--------------------------------------------------------------------------*/
LOCAL uint8 AMR_HEADER2[6] = {'#', '!', 'A', 'M', 'R', '\n'};

LOCAL uint32 amr_count=0;
LOCAL uint32 source_size=0;
LOCAL amr_dec_ctx_t s_dec_ctx = {0};
//__align(4) unsigned char amr_buffer[1024*6+6];
uint8 amr_buffer[1024*15+6];
LOCAL uint8 *amr_datastream, *amr_datastream_init;
LOCAL uint16 frame_size[16]={ 12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0 };
LOCAL uint16 samr_temp[40];

LOCAL HAUDIO hAudRecord = SCI_NULL;
LOCAL void AUD_Amr_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length );
LOCAL AUDIO_RESULT_E AUD_EnableVoiceRecord(  // If succeed, return ERR_AUD_NONE,
                                        // else return error value
    BOOLEAN is_enable,                  // SCI_TRUE: Enable,   SCI_FALSE: Disable
    AUD_REC_FORMAT_E recorder_format,
    RECORD_CALLBACK callback_fun    
    );
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                                                            *
 **--------------------------------------------------------------------------*/
extern uint32               VBDecodeOuputBuf[];
//extern HAUDIOCODEC hAMRCodec;
//extern HAUDIODEV      hARMVB;

extern uint16 GUI_GB2UCS(uint16 *, const uint8 *, uint16);

/*****************************************************************************/
//  Description :call back function for amr driver
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E AMR_string_callback(//HAUDIO hAudio, uint8** const ppucDataAddr, uint32* const puiDataLength) 
    HAUDIO hAudio, 
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo
    )
{
    uint32 puiDataLength;
    
    SCI_ASSERT(ptSrcDataInfo!=PNULL);/*assert verified*/

    //SCI_TRACE_LOW:"amr_dec, callback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_98_112_2_18_2_18_18_248,(uint8*)"");

    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = PNULL;

    puiDataLength = 0;
    
    if (s_dec_ctx.i_state != DECODING)
    {
        //SCI_TRACE_LOW:"amr_dec, callback, not in palying status."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_106_112_2_18_2_18_18_249,(uint8*)"");
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;

        return AUDIO_STREAM_GET_SRC_ERROR;
    }
/*
    if ( 0 )
    {
        //get source error process
        return AUDIO_STREAM_GET_SRC_WAIT;
    }
    else*/
    {

        ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = amr_buffer+amr_count;	

        if(amr_count+512 >source_size)
        {
            //SCI_TRACE_LOW:"amr_dec, amr_count=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_123_112_2_18_2_18_18_250,(uint8*)"d", amr_count);
            puiDataLength = source_size-amr_count;    
            amr_count=source_size;
 
            if(puiDataLength==0)
            {
                amr_count=6;
                ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = amr_buffer+amr_count;	
                puiDataLength = 512;
                amr_count+=512;
            }
             // if(amr_count >=source_size)amr_count=6; 
        }
        else
        {
            puiDataLength = 512;
            amr_count+=512;	
        }
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =puiDataLength;

    }

    //SCI_TRACE_LOW:"amr_dec, *puiDataLength=%d,amr_count=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_145_112_2_18_2_18_18_251,(uint8*)"dd",puiDataLength,amr_count);

    return AUDIO_STREAM_GET_SRC_SUCCESS;
}

/*****************************************************************************/
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void amr_dec_dummy_callbak(
    HAUDIO hAudio, 
    uint32 notify_info, 
    uint32 affix_info
)
{
       return; //do nothing
}
 

/*****************************************************************************/
//  Description : API of start AMR stream play
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int AMR_Record_Play_Start(void)
{
    /*dumming*/
    //SCI_TRACE_LOW:"wav_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_171_112_2_18_2_18_18_252,(uint8*)"");

    if(s_dec_ctx.i_state == DECODING)
    {
        return TRUE;
    }
    s_dec_ctx.i_state = DECODING;

    AUDIO_Play(s_dec_ctx.pcm_handle, 0);
    return TRUE;
}


extern HAUDIODEV hARMVBDualChn;
/*****************************************************************************/
//  Description : API of start AMR stream open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int AMR_Record_Play_Open(void)
{
    //SCI_TRACE_LOW:"amr_dec, open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_190_112_2_18_2_18_18_253,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        //SCI_TRACE_LOW:"amr_dec, open, last operation first not stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_194_112_2_18_2_18_18_254,(uint8*)"");
        return SCI_ERROR;
    }

    s_dec_ctx.pcm_handle = AUDIO_CreateStreamBufferHandle(
                                        hAMRCodec,
                                        PNULL,
                                        hARMVBDualChn,
                                        PNULL,
                                        (uint8 *)PNULL,
                                        0,
                                        (uint32 *)(VBDecodeOuputBuf),
                                        AMR_RECORD_BUFFER_SIZE,
                                        amr_dec_dummy_callbak,
                                        AMR_string_callback
                                        );
    if ( !s_dec_ctx.pcm_handle)
    {
        //SCI_TRACE_LOW:"wav_dec, open(), create handle fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_212_112_2_18_2_18_19_255,(uint8*)"");
        return SCI_ERROR;
    }
           
    return TRUE;

}


DSP_CODEC_EXT_OPE_START_RECORD_T g_ptPara = {0}; 
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

    uint32 temp; 
    AUDIO_RESULT_E status;
             
    if (is_enable)
    {        
        g_ptPara.recorder_format = recorder_format;
        g_ptPara.callback_fun = callback_fun;
        g_ptPara.hAudioHandle = hAudRecord;
        
        status = AUDIO_ExeCodecExtOpe(hAudRecord, 
                        ANSI2UINT16("EXT_START_RECORD"), 
                        DSP_CODEC_EXT_OPE_START_RECORD_PARA_COUNT, 
                        &g_ptPara,
                        &temp);  
                    
    }
    else
    {
        status = AUDIO_ExeCodecExtOpe(hAudRecord, 
                        ANSI2UINT16("EXT_STOP_RECORD"), 
                        1, 
                        (void *)&hAudRecord,
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


LOCAL void AUD_Amr_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length )
{
    DSP_CODEC_EXT_OPE_VOICE_DATA_T ptPara;
    uint32 temp;
    ptPara.source_buf_ptr = destination_buf_ptr;
    ptPara.data_length = data_length;
    
    AUDIO_ExeCodecExtOpe(hAudRecord, 
                            ANSI2UINT16("READ_VOICE_DATA"), 
                            DSP_CODEC_EXT_OPE_VOICE_DATA_PARA_COUNT, 
                            &ptPara,
                            &temp); 

}


// for AMR_Recode test

/*****************************************************************************/
//  Description : Create amr record file.
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AMR_REOCED_File(uint8 *data_ptr, uint32 data_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    uint8  iis_file_name[40] = {0};
    uint16 ucs2_iis_file_name[40] = {0};  
    uint32 tmp = 0;
     
    SFS_HANDLE file_handle_r = NULL;
    sprintf((char*)iis_file_name,"E:\\AMR_Recode-1.amr");

    GUI_GB2UCS(ucs2_iis_file_name, iis_file_name, (uint16)(sizeof(iis_file_name)));

    file_handle_r = SFS_CreateFile((const uint16 *)ucs2_iis_file_name, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE,NULL,NULL);

    if(file_handle_r > 0)
    {
        SFS_WriteFile(file_handle_r, data_ptr, data_len, (uint32*)&tmp, NULL);

    }    
    
    SFS_CloseFile(file_handle_r);
    
    return;
}

/*****************************************************************************/
//  Description : Create amr record callback funcation.
//                for read arm_reoced data from codec.
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void amrenc_CallBack(int16 *psBuf, uint32 uLength)
{
    uint16 *p16_tmp;
    int32 i;
  
    p16_tmp = (uint16 *)psBuf;
    SCI_TRACE_LOW("amrenc_CallBack uLength:0x%x", uLength);
    for (i=0; i<uLength; i++)
    {
        *(amr_datastream+i) = (uint8)(((uint8 *)p16_tmp)[i]);
    }
    amr_datastream += uLength;	
   
    if ((amr_datastream - amr_datastream_init) > 1024*15)
    {
        amr_datastream = amr_datastream_init;
    }
}


extern HAUDIODEV hARMVBDualChnRecord;
/*****************************************************************************/
//  Description : API of start amr record 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void AMR_Recode_Start(void)
{
    int32 i;
    uint32 temp = 0;
    RECORD_UPLOAD_PARAM_T record_param = {AUD_REC_AMR_SW, RECORD_OUTPUT_ARM_STREAM, amrenc_CallBack, 0};

    //SCI_TRACE_LOW:"AMR_Record_Test: start reoced"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_360_112_2_18_2_18_19_256,(uint8*)"");
    // Add amr head for saving recode amr_data to amr_file
    amr_datastream=amr_buffer+6;
    amr_datastream_init=amr_buffer+6;
    
    for(i=0;i<6;i++)
    {
        amr_buffer[i]=AMR_HEADER2[i];
    }

    if(!hAudRecord)
    {
        hAudRecord     = AUDIO_CreateRecordBufferHandle(hRecordCodec,  SCI_NULL, hARMVBDualChnRecord,     
            SCI_NULL, amr_datastream,(1024*15),NULL,NULL,amr_dec_dummy_callbak);
    }
    AUDIO_ExeCodecExtOpe(hAudRecord,
            ANSI2UINT16("SET_RECORDPARAM"),
            4,
            &record_param, &temp);

       //start recoeding
    AUDIO_Play(hAudRecord,0);
 
}

/*****************************************************************************/
//  Description : API of stop amr record and save record data to a file
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void AMR_Recode_Stop(void)
{
    //SCI_TRACE_LOW:"AMR_Record_Test: stop reoced"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_388_112_2_18_2_18_19_257,(uint8*)"");
    //stop_recode
    AUDIO_Stop(hAudRecord);
    AUDIO_CloseHandle(hAudRecord);
    hAudRecord = SCI_NULL;
    //save reoced data to file	
    AMR_REOCED_File((uint8 *)amr_buffer, 4096+6); 

}

/*****************************************************************************/
//  Description : API of  amr record ¡¢ play demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AMR_Recode_Demo_Start(void)
{
    AMR_Recode_Start();
}

/*****************************************************************************/
//  Description : API of  amr record ¡¢ play demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AMR_Recode_Demo_Stop(void)
{
         AMR_Recode_Stop();

}

PUBLIC void AMR_Recode_Demo_PlayBack(void)
{
    //SCI_TRACE_LOW:"amr_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_419_112_2_18_2_18_19_258,(uint8*)"");
    source_size=sizeof(amr_buffer); 

    memset(&s_dec_ctx, 0, sizeof(amr_dec_ctx_t));

    amr_count=0x6;

    //SCI_TRACE_LOW:"amr_dec,source_len= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_426_112_2_18_2_18_19_259,(uint8*)"d",source_size);

    AUDIO_SetVolume(5);
    AMR_Record_Play_Open();
    AMR_Record_Play_Start();
}

/*****************************************************************************/
//  Description : API of  AMR stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AMR_Recode_Demo_PlayBack_Stop(void)
{
    //SCI_TRACE_LOW:"amr_dec, close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_RECORD_DEMO_438_112_2_18_2_18_19_260,(uint8*)"");

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


