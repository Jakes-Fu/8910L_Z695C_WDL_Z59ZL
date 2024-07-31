/******************************************************************************
 ** File Name:      audio_app.c                                               *
 ** Author:         shujing.dong                                              *
 ** DATE:           06/30/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines audio general application interface.    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/30/2010     shujing.dong     Create.                                   *
 ******************************************************************************/

 /*! \file audio_app.c
  *  \author shujing.dong
  *  \date june 30, 2010
  *  \brief This file defines audio general application interface.
  */

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "ms_ref_aud_config_trc.h"
#include "audio_api.h"
#include "audio_config.h" 
#include "audio_app.h" 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_AUDIO_APP

#ifdef DEBUG_AUDIO_APP
    #ifndef AUDIO_PRINT_APP
    #define AUDIO_PRINT_APP( _format_string )   SCI_TRACE_LOW _format_string
    #endif
#else
    #ifndef AUDIO_PRINT_APP
    #define AUDIO_PRINT_APP( _format_string ) 
    #endif
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define AUDIO_APP_PCM_BUF_SIZE 640
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
BOOLEAN s_tx_clarity_enable = FALSE;
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Definitions                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: Only one dummy function.
// Author: 
// Note:           
/*****************************************************************************/
static void audio_app_Dummy_Notify(
    HAUDIO hAudio, 
    uint32 notify_info, 
    uint32 affix_info
)
{

}
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC void AUDIO_APP_EnableDataFetch(void)
//! \param  is_enable
//! \return AUDIO_RESULT_E
//! \brief  Description: This function is used to set data_fetch enable/disable.
//! \author Author: shujing.dong
//! \note   Note:Nothing.           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDataFetch(  // If succeed, return AUDIO_NO_ERROR,
                                        // else return error value
    BOOLEAN is_enable                   // SCI_TRUE: Enable,   SCI_FALSE: Disable
    )
{     
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    static HAUDIO h_data_play=INVALID_HANDLE, h_data_record=INVALID_HANDLE;
    static uint32 *ui_pcm_buffer=PNULL;
    AUDIO_PRINT_APP(("AUDIO_EnablePcm: %s", is_enable?"enable":"disable"));
        
    if(is_enable)
    {  
        if(PNULL!=ui_pcm_buffer)
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm] be enable buf:0x%x."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_89_112_2_18_1_1_59_7,(uint8*)"d", ui_pcm_buffer);
            return AUDIO_ERROR;
        }
        if((INVALID_HANDLE!=h_data_play) || (INVALID_HANDLE!=h_data_record))
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm] be enable handle:0x%x, 0x%x."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_94_112_2_18_1_1_59_8,(uint8*)"dd", h_data_play, h_data_record);
            return AUDIO_ERROR;
        }
        ui_pcm_buffer = (uint32 *)SCI_ALLOC_APP( AUDIO_APP_PCM_BUF_SIZE*4);
        if(PNULL == ui_pcm_buffer )
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]ui_pcm_buffer alloc failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_100_112_2_18_1_1_59_9,(uint8*)"");
            return AUDIO_ERROR;
        }
#ifndef WIN32        
        h_data_play = AUDIO_CreateBufferHandle(hDownlinkCodec, SCI_NULL, hPCMDev, SCI_NULL, 
            SCI_NULL, 0, ui_pcm_buffer, AUDIO_APP_PCM_BUF_SIZE, audio_app_Dummy_Notify);
#endif
        if(INVALID_HANDLE==h_data_play)
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_play create failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_109_112_2_18_1_2_0_10,(uint8*)"");
            return AUDIO_ERROR;
        }
#ifndef WIN32
        h_data_record = AUDIO_CreateNormalHandle(hUplinkCodec, SCI_NULL, hPCMDev, SCI_NULL,
             audio_app_Dummy_Notify); 
#endif
        if(INVALID_HANDLE==h_data_record)
        {
            //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_record create failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_118_112_2_18_1_2_0_11,(uint8*)"");
            AUDIO_CloseHandle(h_data_play);
            return AUDIO_ERROR;
        } 

        /* start audio */
        result = AUDIO_Play(h_data_play, 0);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_play play failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_127_112_2_18_1_2_0_12,(uint8*)"d", result);
          AUDIO_CloseHandle(h_data_play);
          AUDIO_CloseHandle(h_data_record);
          return result;
        }
        
        result = AUDIO_Play(h_data_record, 0);  
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_record play failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_136_112_2_18_1_2_0_13,(uint8*)"d", result);
          AUDIO_Stop(h_data_play);
          AUDIO_CloseHandle(h_data_play);
          AUDIO_CloseHandle(h_data_record);
          return result;
        }
    }
    else
    {
        result = AUDIO_Stop(h_data_play);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_play stop failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_148_112_2_18_1_2_0_14,(uint8*)"d", result);
          return result;
        }
        result = AUDIO_Stop(h_data_record);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_record stop failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_154_112_2_18_1_2_0_15,(uint8*)"d", result);
          return result;
        }
        result = AUDIO_CloseHandle(h_data_play);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_play close failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_160_112_2_18_1_2_0_16,(uint8*)"d", result);
          return result;
        }
        result = AUDIO_CloseHandle(h_data_record);
        if(AUDIO_NO_ERROR != result)
        {
          //AUDIO_PRINT_APP:"[AUDIO_EnablePcm]h_data_record close failed. result=0x%x."
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_APP_166_112_2_18_1_2_0_17,(uint8*)"d", result);
          return result;
        }   
        h_data_play = INVALID_HANDLE;
        h_data_record = INVALID_HANDLE;

        if(PNULL != ui_pcm_buffer )
        {
            SCI_FREE(ui_pcm_buffer);
        }
    }
    
    return result;    
}

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDatadump(
//!    BOOLEAN bOn,             //start/stop
//!    AUD_DUMP_SAVE_FLAG_E uidump_save, // 1:change NV; 0: active untill reopen phone
//!    AUD_DUMP_TO_FLAG_E uidump_path, // 0:DUMP_TO_FILE; 1:DUMP_TO_ARM_LOG
//!    uint32 uiposflag         //posflag:bit_0:pos_0; bit_1:pos_1;.....bit_n:pos_n
//!    )
//! \param bOn              start/stop
//! \param bwrite_to_flash  TRUE:change NV; FALSE: not change nv, active untill reopen phone.
//! \param bdump_to_log     TRUE:DUMP_TO_ARM_LOG  FALSE:DUMP_TO_FILE
//! \param uiposflag        uint32:32 posisions.bit_0:pos_0;bit_1:pos_1....bit_31:pos_31;
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is dynamic open/close dump pcm in audio.
//! \author Author: yaye.jiang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDatadump(
    BOOLEAN bOn,             //start/stop
    AUD_DUMP_SAVE_FLAG_E uidump_save, // 1:change NV; 0: active untill reopen phone
    AUD_DUMP_TO_FLAG_E uidump_path, // 0:DUMP_TO_FILE; 1:DUMP_TO_ARM_LOG
    uint32 uiposflag         //posflag:bit_0:pos_0; bit_1:pos_1;.....bit_n:pos_n
    )
{
    AUDMOD_RESULT_E result = AUDIO_NO_ERROR;
    AUDIO_NV_ARM_MODE_INFO_T    *ptmpMode = PNULL;      
    char *mode_name = NULL;
    uint16 usNvConfigToFiles=0;

    ptmpMode = (AUDIO_NV_ARM_MODE_INFO_T *)SCI_ALLOCA(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptmpMode)
    {
        SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump, alloc fail, size: %d", sizeof(AUDIO_NV_ARM_MODE_INFO_T));

        return AUDIO_NO_ENOUGH_MEMORY;
    }
    mode_name = AUDIONVARM_GetAudioModeNameById(NV_AUDIO_ARM_HANDSFREE_E);
    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptmpMode)))
    {
        SCI_FREE(ptmpMode);
        SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump failed to get mode param.");
        return AUDIO_ERROR;
    }
SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump bOn=%d,uidump_save=%d,uidump_path=%d,uiposflag=0x%x",
    bOn, uidump_save, uidump_path, uiposflag);

SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump reserve[63]=0x%x,reserve[61]=0x%x", 
    ptmpMode->tAudioNvArmModeStruct.reserve[63], ptmpMode->tAudioNvArmModeStruct.reserve[61]);

    usNvConfigToFiles = ptmpMode->tAudioNvArmModeStruct.reserve[63];
    if(bOn)
    {
        //如果nv中pos对应bit有数据，则说明已经开启dump，此时输出方式不能修改
        if((usNvConfigToFiles&0x0FFF)||(ptmpMode->tAudioNvArmModeStruct.reserve[61]&0xFFFF))
        {
            AUD_DUMP_TO_FLAG_E dump_path = (usNvConfigToFiles&0x3000)>>12;

            if(uidump_path != dump_path)
            {
                SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump fail!! It already dumping to %d.", dump_path);
                SCI_FREE(ptmpMode);
                return AUDIO_PARAM_ERROR;
            }
        }
        else
        {
            if(DATA_DUMP_TO_ARM_LOG == uidump_path)
            {
                usNvConfigToFiles |= 0x1000;  
            }
            else if(DATA_DUMP_TO_FILE == uidump_path)
            {
                usNvConfigToFiles &= 0xCFFF;  
            }
        }
        usNvConfigToFiles |= (uiposflag&0x0FFF);
        
        ptmpMode->tAudioNvArmModeStruct.reserve[63] = usNvConfigToFiles;
        if(uiposflag>>12)
        {
            usNvConfigToFiles = ptmpMode->tAudioNvArmModeStruct.reserve[61];
            usNvConfigToFiles |= (uiposflag>>12);
            ptmpMode->tAudioNvArmModeStruct.reserve[61] = usNvConfigToFiles;
        }
        AUDIONVARM_SetModeParam(mode_name, ptmpMode);

        Datadump_Enable(bOn, uiposflag);
    }
    else
    {
        Datadump_Enable(bOn, uiposflag);
        //if(bwrite_to_flash)
        {
            usNvConfigToFiles &= ~(uiposflag&0x0FFF);
            ptmpMode->tAudioNvArmModeStruct.reserve[63] = usNvConfigToFiles;
            if(uiposflag>>12)
            {
                usNvConfigToFiles = ptmpMode->tAudioNvArmModeStruct.reserve[61];
                usNvConfigToFiles &= ~(uiposflag>>12);
                ptmpMode->tAudioNvArmModeStruct.reserve[61] = usNvConfigToFiles;
            }
            AUDIONVARM_SetModeParam(mode_name, ptmpMode);
        }
    }
    

SCI_TRACE_LOW("audio_app.c AUDIO_APP_EnableDatadump reserve[63]=0x%x,reserve[61]=0x%x", 
    ptmpMode->tAudioNvArmModeStruct.reserve[63], ptmpMode->tAudioNvArmModeStruct.reserve[61]);
    if(DUMP_SAVE_TO_FLASH == uidump_save)
    {
        result = AUDIONVARM_WriteModeParamToFlash(mode_name, ptmpMode);
    }

    SCI_FREE(ptmpMode);
    return result;
}

PUBLIC void AUDIO_EnableTxClarity(BOOLEAN bEnable)
{
    s_tx_clarity_enable = bEnable;
}

PUBLIC BOOLEAN AUDIO_GetTxClarityEnable(void)
{
    return s_tx_clarity_enable;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of audio_api.c

