/******************************************************************************
 ** File Name:      boot.c                                                    *
 ** Author:          hongwei.xu                                               *
 ** DATE:           11/02/2007                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contains driver for LCM I/F controller. 
 **                                                          
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/02/2007     hongwei xu       Create.
 **  
 ******************************************************************************/

/**----------------------------------------------------------------------------*
 **                         Dependencies                                       *
 **----------------------------------------------------------------------------*/

#include "cmmb_trc.h"
#include "os_api.h"
#include "sci_service.h"
#include "user_tasks_id.h"
#include "mtv_boot.h"
#include "dtl.h"
#include "dtl_cmdq.h"
#include "seine_sdram_drv.h"
#include "lcd.h"
/*@fool2 #include "spi_tv.h" */
#include "lcmif_drv.h"
#include "gpio_drv.h"
#include "lcd_tv.h"
#include "mtv_cfg.h"
/*@fool2 #include "dtl_rfvt.h" */
#include "dtl_recovery.h"
#include "kd.h"
#include "tv_firmware.h"
#include "nv_type.h"
#include "nvitem.h"
#include "ref_param.h"
#include "mtv_setting.h"

/**----------------------------------------------------------------------------*
 **                        Compiler Flag                                       *
 **----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
**                               Debug flag                                **
**----------------------------------------------------------------------------*/
#define DEBUG_BOOT

#ifdef DEBUG_BOOT
    #define BOOT_TRACE KD_TRACE_LOW
#else
//    #define BOOT_TRACE
    #define BOOT_TRACE KD_TRACE_LOW /* @fool2 */
#endif

#define BOOT_ASSERT kdAssert

/**---------------------------------------------------------------------------*
** static value
*/
#define S_ASSERT_INFO_MAX_LEN  100 //seine assert info data length

#define TVBOOT_MAX_WAIT_TIME   100

//tv boot ok flag
LOCAL KDboolean s_mtv_boot_ok = KD_FALSE;

LOCAL MTVBOOT_CALLBACK s_tv_callback = PNULL;
LOCAL MTVBOOT_CALLBACK s_tv_off_callback = PNULL;

extern KDuint8 video_decoder_h264[];
extern KDuint32 video_decoder_h264_len;
extern KDuint8 arm_common_code[];
extern KDuint32 arm_common_code_len;
extern KDuint8 video_decoder_avs[];
extern KDuint32 video_decoder_avs_len;
extern KDuint8 code_in_sdram[];
extern KDuint32 code_in_sdram_len;
extern KDuint8 dsp_common_code_cmmb[];
extern KDuint32 dsp_common_code_cmmb_len;
extern KDuint8  aac_table[];
extern KDuint32 aac_table_len;
extern KDuint8  scatte_pilot[];
extern KDuint32 scatte_pilot_len;
extern KDuint8 aac_decoder_code[];
extern KDuint32 aac_decoder_code_len;
extern KDuint8 dra_decoder_code[];
extern KDuint32 dra_decoder_code_len;
extern KDuint8  dra_table[];
extern KDuint32 dra_table_len;
extern KDuint8 dsp_acq_track_code[];
extern KDuint32 dsp_acq_track_code_len;

#ifdef _SC6600V_WQVGA_
extern uint8 arm_common_code_wdm[];
extern uint32 arm_common_code_wdm_len;

extern uint8 code_in_sdram_wdm[];
extern uint32 code_in_sdram_wdm_len;
#endif

extern BOOLEAN REFPARAM_GetEnableArmLogFlag(void);

__ALIGN(4) const KDuint8 mtv_identify_lcd[]= 
#include "identify_lcd.h"
KDuint32 mtv_identify_lcd_len = sizeof(mtv_identify_lcd);

#define EndianChange INT_SWAP

void LCD_DspCodeEndianChange(uint16 *code_ptr, uint32 len)
{
    uint32 i;
    len /= 2;
    for(i=0; i<len; i++)
    {
        EndianChange(code_ptr[i]);
    }
}


void LCD_CfgEndianChange(uint32 *cfg_ptr, uint32 len)
{
    uint32 i;
    LCD_COM_CFG *lcd_cfg_ptr      = (LCD_COM_CFG*)cfg_ptr;
    LCD_OP_CELL *lcd_cell_ptr = (LCD_OP_CELL *)&lcd_cfg_ptr[1];

    KD_TRACE_LOW("len = %u, cfg_len = %u", len, sizeof(LCD_COM_CFG));

    len -= sizeof(LCD_COM_CFG);
    len /= sizeof(LCD_OP_CELL);

    EndianChange(lcd_cfg_ptr->ctrl_mode);
    EndianChange(lcd_cfg_ptr->mode_param.sync_mode);
    EndianChange(lcd_cfg_ptr->mode_param.bus_mode);
    EndianChange(lcd_cfg_ptr->mode_param.vsync_pol);
    EndianChange(lcd_cfg_ptr->mode_param.cd_cmd);
    EndianChange(lcd_cfg_ptr->mode_param.data_type);
    EndianChange(lcd_cfg_ptr->timing_param.vsync_width);

    for(i=0; i<20; i++)
    {
        EndianChange(lcd_cfg_ptr->lcd_gamma_table[i]);
    }
    EndianChange(lcd_cfg_ptr->lcd_init_offset);
    EndianChange(lcd_cfg_ptr->lcd_enter_sleep_offset);
    EndianChange(lcd_cfg_ptr->lcd_set_contrast_offset);
    EndianChange(lcd_cfg_ptr->lcd_set_brightness_offset);
    EndianChange(lcd_cfg_ptr->lcd_set_display_window_offset);
    EndianChange(lcd_cfg_ptr->lcd_get_info_offset);
    EndianChange(lcd_cfg_ptr->lcd_get_set_frame_rate_offset);
    EndianChange(lcd_cfg_ptr->lcd_clear_offset);
    EndianChange(lcd_cfg_ptr->lcd_close_offset);
    EndianChange(lcd_cfg_ptr->lcd_open_offset);
    EndianChange(lcd_cfg_ptr->lcd_set_direction_offset);
    EndianChange(lcd_cfg_ptr->res1_offset);
    EndianChange(lcd_cfg_ptr->res2_offset);
    EndianChange(lcd_cfg_ptr->res3_offset);

    for(i=0; i<len; i++)
    {
        EndianChange(lcd_cell_ptr[i].reg_index);
        EndianChange(lcd_cell_ptr[i].data);
    }
}

void SC6600V_CfgEndianChange(uint32 *cfg_ptr)
{
    MTV_COMM_CONFIG_T *cfg_tmp_ptr = (MTV_COMM_CONFIG_T*)cfg_ptr;
    EndianChange(cfg_tmp_ptr->sc6600v_cfg_id);
    EndianChange(cfg_tmp_ptr->gpio_out_id);
    EndianChange(cfg_tmp_ptr->gpio_in_id);
    EndianChange(cfg_tmp_ptr->gpio_out_int_type);
    EndianChange(cfg_tmp_ptr->gpio_in_int_type);
    EndianChange(cfg_tmp_ptr->gpio_out_delay);
    EndianChange(cfg_tmp_ptr->lcd_type);
    EndianChange(cfg_tmp_ptr->arm_log_uart_id);
    EndianChange(cfg_tmp_ptr->dsp_log_uart_id);
    EndianChange(cfg_tmp_ptr->arm_log_cfg);
    EndianChange(cfg_tmp_ptr->arm_log_uart_rate);
    EndianChange(cfg_tmp_ptr->arm_spi_port_id);
    EndianChange(cfg_tmp_ptr->freq);

    EndianChange(cfg_tmp_ptr->iis_cfg.mode);
    EndianChange(cfg_tmp_ptr->iis_cfg.iis_clk);    
    EndianChange(cfg_tmp_ptr->iis_cfg.iis_port);
    EndianChange(cfg_tmp_ptr->audio_mode);    
}

/******************************************************************************/
// Description: 
//     Registed by MTVBOOT_TVOPEN() and will be called when receive some msg
//   from Seine.
//       .
// Global resource dependence: 
// Author:  hongwei xu
// Note:      id:    Task id of the client which register the event
//                  The specific funtion of argc and argv, please see each
//                  server's definition, according to function
//                  PUBLIC void SCI_SendEventToClient(
//                  KDuint32 server_ID,     // Server ID, EXAMPLE: KEYPAD_SERVICE
//                  KDuint32 ui_event_type, // Message ID of the registered event
//                  void * signal_ptr     // Signal Sent to Client
//                  );
//                  Where argc equals ui_event_type, argv equals to signal_ptr
/******************************************************************************/
LOCAL void MTVBOOT_Callback(uint32 id, uint32 argc, void *argv);

/******************************************************************************/
// Description: 
//     Registed by TVBOOT_TVOPEN() and will be called when receive some msg
//   from Seine.
//       .
// Global resource dependence: 
// Author:  hongwei xu
/******************************************************************************/
LOCAL void MTVBOOT_DSPCommonCallback(uint32 id, uint32 argc, void *argv);

/*
static unsigned char pcm_buffer[] = 
#include "pcm.h"
#define    PCM_BUFFER_SIZE (sizeof(pcm_buffer))
*/

/******************************************************************************/
// Description:  Set the non-bypass mode, initialize the SDRAM, download the
//     ARM and DSP code, and register a callback function to process the messages
//     response from Seine.
// Global resource dependence: 
// Author:  hongweixu
// Note:
/******************************************************************************/
KDboolean MTVBOOT_TVOpen(
                              MTVBOOT_CALLBACK    callback,
                              uint8 *mem1,
                              uint8 *mem2
                              )
{
    KDuint32  dsp_hold_data = 0;
    KDuint8    event         = DTL_CMDQ_ARM;
    uint32      data_len      = 0;
    KDuint8   *send_buffer=NULL;
    KDboolean is_ok         = KD_FALSE;
    KDuint16  cmd_num       = 0;
    KDuint16  wait_times    = 0;
    DTL_RESPQ_CMD_T *RespQ_ptr = PNULL;
    KDuint32  *lcd_cfg_data_ptr = PNULL;
    KDuint32  data_src_addr = 0;
    KDuint32 read_cmd_id = 0;
    KDuint32 arm_common_code_length;
    MTV_COMM_CONFIG_T cfg_tmp = {0};

    BOOT_TRACE("MTVBOOT_TVOpen before SDRAM Init"); /* @fool2 */

    /* initialize the SDRAM */
    SDRAM_Init();

    BOOT_TRACE("MTVBOOT_TVOpen SDRAM Init OK");   

    send_buffer = NULL;

    MTV_SetCommConfig(&cfg_tmp);
    SC6600V_CfgEndianChange((uint32*)&cfg_tmp);
    DTL_SendData((KDuint8*)&cfg_tmp,sizeof(MTV_COMM_CONFIG_T),ARM_CONFIG_SECTION_BASE_ADDR);

    /*
    *  register a callback function for the BOOT module
    */
    DTL_RegisterMsg(DTL_SERVICE, event, event, MTVBOOT_Callback);
    
    event = DTL_CMDQ_DSP_COMMON;
    DTL_RegisterMsg(DTL_SERVICE, event, event, MTVBOOT_DSPCommonCallback);
    
    // ARM COMMON CODE
    //BOOT_TRACE("download_arm_common_code!!! 0x%x",OS_GetTickCount());
    download_arm_common_code(ARM_COMMON_CODE_BASE_ADDR,&data_len,mem1,mem2);
    arm_common_code_length = data_len;
    
    //VIDEO_H264_CODE
    //BOOT_TRACE("download_video_decoder_h264!!! 0x%x",OS_GetTickCount());
    download_video_decoder_h264(VIDEO_H264_CODE_BASE_ADDR,&data_len,mem1,mem2);
    
    //ARM_CODE_IN_SDRAM                       
    //BOOT_TRACE("download_code_in_sdram!!! 0x%x",OS_GetTickCount());
    download_code_in_sdram(0x90600000,&data_len,mem1,mem2);
    
    //ARM_DSP_COMMON_CODE
    //BOOT_TRACE("download_dsp_common_code!!! 0x%x",OS_GetTickCount());
    download_dsp_common_code(DSP_COMMON_CODE_CMMB_BASE_ADDR,&data_len,mem1,mem2);   


    //DSP_AUDIO_CODE_AAC
    //BOOT_TRACE("download_dsp_decoder_aac!!! 0x%x",OS_GetTickCount());
    download_dsp_decoder_aac(DSP_AUDIO_CODE_AAC_BASE_ADDR,&data_len,mem1,mem2);
    
    //DSP_AUDIO_CODE_DRA
    //BOOT_TRACE("download_dsp_decoder_dra!!! 0x%x",OS_GetTickCount());
    download_dsp_decoder_dra(DSP_AUDIO_CODE_DRA_BASE_ADDR,&data_len,mem1,mem2);
   
    //aac table
    data_len = aac_table_len;
    is_ok = DTL_SendData((KDuint8*)aac_table,data_len,DSP_AAC_PARAM_TABLE_BASE_ADDR);//0x91b00000);
    if( !is_ok )
    {
        BOOT_TRACE("TVBOOT_TVOpen()download aac table error!\n");
        return KD_FALSE;
    }
    //dra table
    data_len = dra_table_len;
    is_ok = DTL_SendData((uint8*)dra_table,data_len,DSP_DRA_PARAM_TABLE_BASE_ADDR);//0x91b00000);
    if( !is_ok )
    {
        BOOT_TRACE("TVBOOT_TVOpen()download dra table error!\n");
        return FALSE;
    }
    
    //dsp_acq_track_code
    //BOOT_TRACE("download_dsp_acq_track!!! 0x%x",OS_GetTickCount());
    download_dsp_acq_track(VIDEO_AVS_CODE_BASE_ADDR,&data_len,mem1,mem2);
    
    //scatte_pilot
    data_len = scatte_pilot_len;
    is_ok = DTL_SendData((KDuint8*)scatte_pilot,data_len,0x90f80000);
    //DEMODULATOR_BUFFER_BASE_ADDR+DEMODULATOR_BUFFER_MAX_SIZE-data_len);
    if( !is_ok )
    {
        BOOT_TRACE("TVBOOT_TVOpen()download scatte_pilot table error!\n");
        return KD_FALSE;
    }
    MTVCFG_GetLCDCfg(&lcd_cfg_data_ptr, (KDuint32*)&data_len);
    LCD_CfgEndianChange((uint32*)lcd_cfg_data_ptr, data_len);
    BOOT_TRACE("lcd_cfg_data_ptr=0x%p, data len=%u", lcd_cfg_data_ptr, data_len);
    is_ok = DTL_SendData((KDuint8*)lcd_cfg_data_ptr,data_len,ARM_CONFIG_LCD_COM_CFG_BASE_ADDR);
    if( !is_ok )
    {
        BOOT_TRACE("TVBOOT_TVOpen()download LCD COMMON config error!\n");
        return KD_FALSE;
    }
        
    //hold dsp 0x20300000 is seine register
    if( !DTL_GetData((KDuint8*)&dsp_hold_data,sizeof(KDuint32),0x20300000))
    {
        return KD_FALSE;
    }
    INT_SWAP(dsp_hold_data);
    dsp_hold_data = dsp_hold_data|0x8;
    INT_SWAP(dsp_hold_data);
    if ( !DTL_SendData((KDuint8*)&dsp_hold_data,sizeof(KDuint32),0x20300000))
    {
        return KD_FALSE;
    }

/*
    data_len = PCM_BUFFER_SIZE;
    is_ok = DTL_SendData((KDuint8*)pcm_buffer,data_len,
        0x90905664);
    if( !is_ok )
    {
        BOOT_TRACE("PCM data download ARM_COMMON_CODE error!\n");
        return KD_FALSE;
    }
    
*/


    BOOT_TRACE("TVBOOT_TVOpen()download code finish!!!");
    
    /* tell Seine to download the code */
    DTLCMDQ_Download_Arm_Code(0x08000000,ARM_COMMON_CODE_BASE_ADDR,arm_common_code_length);
    
    while(1)
    {
        uint32 get_data;
        uint32 lg_i;
        kdMsDelay(1);
        RespQ_ptr = DTL_GetCmd(&cmd_num);

        if ( cmd_num )
        {
            kdAssert(1==cmd_num); /*assert verified*/
            
            if (BB_ARM_STATUS_DOWNLOAD_ARM_CODE_FINISHED == RespQ_ptr[0].resp_id)
            {
                DTLCMDQ_Jumpto_IRAM_Run();
                DTL_SetCmdQSize(DTL_CMDQ_RESPQ_EXTEND_SIZE); //CMD_BB_CMDQ_SIZE = 16
            }
            else
            {
                kdAssert(0); /*lint !e506*/ /*assert verified*/
            }
            //SCI_FREE(RespQ_ptr);
            break;
        }
        
        wait_times++;
        if ( wait_times > TVBOOT_MAX_WAIT_TIME )
        {
            KD_TRACE_LOW("MTVBOOT_TVOpen() not read BB_ARM_STATUS_DOWNLOAD_ARM_CODE_FINISHED command");
            return KD_FALSE;
        }
    }
        
    if (callback)
    {
        s_tv_callback = callback;
    }
    return KD_TRUE;
}

/******************************************************************************/
// Description:    Cut off the TV. Send a message to Seine to tell it to stop and
//     the others processing will be done in the TVBOOT_Callback().
// Global resource dependence: 
// Author:  hongwei.xu
// Note:
/******************************************************************************/
KDboolean MTVBOOT_TVOff(MTVBOOT_CALLBACK tv_off_callback)
{
    if (MTVBOOT_IsOK())
    {
        DTLCMDQ_Turn_Off_TV();
    }

    if ( tv_off_callback )
    {
        s_tv_off_callback = tv_off_callback;
    }
    
    return KD_TRUE;
}

/******************************************************************************/
// Description: 
//     Registed by MTVBOOT_TVOPEN() and will be called when receive some msg
//   from Seine.
//       .
// Global resource dependence: 
// Author:  hongwei xu
// Note:      id:    Task id of the client which register the event
//                  The specific funtion of argc and argv, please see each
//                  server's definition, according to function
//                  PUBLIC void SCI_SendEventToClient(
//                  KDuint32 server_ID,     // Server ID, EXAMPLE: KEYPAD_SERVICE
//                  KDuint32 ui_event_type, // Message ID of the registered event
//                  void * signal_ptr     // Signal Sent to Client
//                  );
//                  Where argc equals ui_event_type, argv equals to signal_ptr
/******************************************************************************/
LOCAL void MTVBOOT_Callback(uint32 id, uint32 argc, void *argv)
{
    DTL_RESPQ_CMD_T *resp_cmd_ptr = PNULL;
    KDuint16          assert_len    = 0;
    KDuint8           assert_info[S_ASSERT_INFO_MAX_LEN] = {0};
    DTL_RECOVER_CALLBACK tvboot_recover_callback = PNULL;

    kdAssert( PNULL != argv ); /*assert verified*/
    resp_cmd_ptr = (DTL_RESPQ_CMD_T*)argv;
    
    switch(resp_cmd_ptr->resp_id)
    {
    case BB_ARM_STATUS_ARM_DSP_BOOT_FINISHED:
        BOOT_TRACE("MTVBOOT_Callback boot finished");
        s_mtv_boot_ok = KD_TRUE;

        if (s_tv_callback)
        {
            s_tv_callback();
        }        
        break;
    case BB_ARM_STATUS_TURN_OFF_TV_FINISHED:
        
#ifdef LCD_CONNECTED_TO_V
        LCDTV_SetLCDNeedReset(KD_FALSE);
#endif

        //DTL_UnregisterMsg(DTL_SERVICE, DTL_CMDQ_ARM, DTL_CMDQ_ARM,TVBOOT_Callback);
        
        //DTL_UnregisterMsg(DTL_SERVICE, DTL_CMDQ_DSP_COMMON, DTL_CMDQ_DSP_COMMON,TVBOOT_DSPCommonCallback);

        s_mtv_boot_ok = KD_FALSE;

        if (s_tv_off_callback)
        {
            s_tv_off_callback();
        }

        break;
    case BB_ARM_STATUS_SPI_DMA_READY:
        //SPITV_Data_Ready(resp_cmd_ptr->parameter0);
        BOOT_TRACE("BB_ARM_STATUS_SPI_DMA_READY");
        break;
    case BB_ARM_STATUS_ASSERT:
        if ( MTV_RELEASE_MODE == MTV_GetAssertMode() )        
        {
            DTL_SetErrorInfo(DTL_ERR_SEINE_ARM_ASSERT);
            tvboot_recover_callback = DTLR_GetCallback();
            if(tvboot_recover_callback)
            {
                tvboot_recover_callback(DTLR_GetParam(),DTL_ERR_SEINE_ARM_ASSERT,0,0);
            }
        }
        else
        {
            assert_len = MIN(resp_cmd_ptr->parameter0,S_ASSERT_INFO_MAX_LEN);
            DTL_GetData((KDuint8*)assert_info,assert_len,resp_cmd_ptr->parameter1);
            BOOT_TRACE((char*)assert_info);
            kdAssert(0); /*lint !e506*/ /*assert verified*/
        }
        break;
    default:
        BOOT_TRACE("it is not ARM COMMON message!\n");
        break;
    }
    
    return;
}

/******************************************************************************/
// Description: 
//     Registed by MTVBOOT_TVOPEN() and will be called when receive some msg
//   from Seine.
//       .
// Global resource dependence: 
// Author:  hongwei xu
/******************************************************************************/
LOCAL void MTVBOOT_DSPCommonCallback(uint32 id, uint32 argc, void *argv)
{
    DTL_RESPQ_CMD_T *resp_cmd_ptr = PNULL;
    KDuint8           assert_info[S_ASSERT_INFO_MAX_LEN] = {0};
    KDuint16          p0 = 0;
    KDuint16          p1 = 0;
    DTL_RECOVER_CALLBACK tvboot_recover_callback = PNULL;
    
    kdAssert( PNULL != argv ); /*assert verified*/
    resp_cmd_ptr = (DTL_RESPQ_CMD_T*)argv;
    
    switch(resp_cmd_ptr->resp_id)
    {
    case DSP_CMDQ_DSP_ASSERT:
        if ( MTV_RELEASE_MODE == MTV_GetAssertMode() )       
        {
            DTL_SetErrorInfo(DTL_ERR_SEINE_DSP_ASSERT);
            tvboot_recover_callback = DTLR_GetCallback();
            if(tvboot_recover_callback)
            {
                tvboot_recover_callback(DTLR_GetParam(),DTL_ERR_SEINE_DSP_ASSERT,0,0);
            }
        }
        else
        {
            p0 = (KDuint16)(resp_cmd_ptr->parameter0);
            p1 = (KDuint16)(resp_cmd_ptr->parameter1);

            sprintf((char*)assert_info,"SeineDSPASSERT:fileno=0x%x,lineno=0x%x,p1=0x%x,p2=0x%x",
                    (KDuint16)((resp_cmd_ptr->parameter0)>>16),p0,
                    (KDuint16)((resp_cmd_ptr->parameter1)>>16),p1);
            
            kdAssert(0); /*lint !e506*/ /*assert verified*/
        }
        break;
    default:
        BOOT_TRACE("it is not DSP ASSERT message!\n");
        break;
    }
    
    return;
}

/******************************************************************************/
// Description:  tv boot is ok
//     
// Global resource dependence: 
// Author:  hongweixu
// Note:   KD_TRUE OK
/******************************************************************************/
KDboolean MTVBOOT_IsOK(void)
{
    return s_mtv_boot_ok;
}

/******************************************************************************/
// Description:  set off flag for assert
//     
// Global resource dependence: 
// Author:  hongweixu
// Note:   
/******************************************************************************/
void MTVBOOT_SetOffFlagForAssert(void)
{
    s_mtv_boot_ok = KD_FALSE;

}

/******************************************************************************/
// Description:  download firmware to sharemem to get lcd id
// Global resource dependence: 
// Author:  hongweixu
// Note:
/******************************************************************************/
KDboolean MTVBOOT_IdentifyLCDId( KDuint32 lcdid_addr,KDuint32 *lcd_id1_ptr,KDuint32 *lcd_id2_ptr,KDuint32 *sim_smd_ptr)
{
    KDuint32 write_len = 0;
    KDuint32 dsp_hold_data = 0;
    KDboolean is_ok = KD_FALSE;
    
    DTL_CMDQ_CMD_T cmd = {0};

    kdAssert(lcd_id1_ptr!= PNULL); /*assert verified*/
    kdAssert(lcd_id2_ptr!= PNULL); /*assert verified*/

    kdMsDelay(50);

    //hold dsp 0x20300000 is seine register
    if( !LCMIF_Read((KDuint32*)&dsp_hold_data,0x20300000))
    {
        return KD_FALSE;
    }
    INT_SWAP(dsp_hold_data);
    dsp_hold_data = dsp_hold_data|0x8;
   
    INT_SWAP(dsp_hold_data);    
    if ( !LCMIF_Write(dsp_hold_data,0x20300000))
    {
        return KD_FALSE;
    }

    //write lcd id address
    INT_SWAP(lcdid_addr);
    if ( !LCMIF_Write(lcdid_addr,0x100003fc))
    {
        return KD_FALSE;
    }

    //download firmware
    write_len = LCMIF_Write_Data((KDuint8*)mtv_identify_lcd,mtv_identify_lcd_len,0x10000400);
    if ( write_len != mtv_identify_lcd_len)
    {
        return KD_FALSE;
    }
    
    //send cmd
    cmd.cmd_id     = BB_ARM_CMDQ_DOWNLOAD_ARM_CODE;
    cmd.parameter0 = 0x08000000;
    cmd.parameter1 = 0x10000400;
    cmd.parameter2 = mtv_identify_lcd_len;
    is_ok = DTL_SendCMD_NOMutex((DTL_CMDQ_CMD_T*)&cmd);
    if (!is_ok)
    {
        return KD_FALSE;
    }

    kdMemset(&cmd,0,sizeof(DTL_CMDQ_CMD_T));
    cmd.cmd_id     = BB_ARM_CMDQ_JUMP_TO_RUN;
    cmd.parameter0 = 0x08000000;    
    is_ok = DTL_SendCMD_NOMutex(&cmd);
    if (!is_ok)
    {
        return KD_FALSE;
    }

    kdMsDelay(100);

    LCMIF_Read(lcd_id1_ptr,0x10000020);
    INT_SWAP(*lcd_id1_ptr);
    LCMIF_Read(lcd_id2_ptr,0x10000024);
    INT_SWAP(*lcd_id2_ptr);
    LCMIF_Read(sim_smd_ptr,0x10000028);
    INT_SWAP(*sim_smd_ptr);
    
    BOOT_TRACE("TVBOOT_IdentifyLCDId() lcd_id1=0x%x,lcd_id2=0x%x,sim_smd_ptr=0x%x",*lcd_id1_ptr,*lcd_id2_ptr,*sim_smd_ptr);
    
    return KD_TRUE;
}

#ifdef _SC6600V_WQVGA_
/******************************************************************************/
// Description:  Set the non-bypass mode, initialize the SDRAM, only download the
//     ARM code, and register a callback function to process the messages
//     response from Seine.
// Global resource dependence: 
// Author:  hongweixu
// Note:
/******************************************************************************/
PUBLIC BOOLEAN TVBOOT_TVOpenWqvga(
                             uint32 standard, //the DC/MP4 type
                             TVBOOT_CALLBACK    callback
                             )
{
    uint32  dsp_hold_data = 0;
    uint32  event         = DTL_CMDQ_ARM;
    uint32  data_len      = 0;
    BOOLEAN is_ok         = FALSE;
    uint16  cmd_num       = 0;
    uint16  wait_times    = 0;
    DTL_RESPQ_CMD_T *RespQ_ptr = PNULL;
    uint32  *lcd_cfg_data_ptr = PNULL;
    MTV_COMM_CONFIG_T cfg_tmp = {0};
    
    //SCI_ASSERT( standard < TV_STANDARD_INVALID ); /*assert verified*/


    /* initialize the SDRAM */
    SDRAM_Init();
    
    MTVCFG_SetDCMP4On(standard);

    MTV_SetCommConfig(&cfg_tmp);
    SC6600V_CfgEndianChange((uint32*)&cfg_tmp);
    DTL_SendData((KDuint8*)&cfg_tmp,sizeof(MTV_COMM_CONFIG_T),ARM_CONFIG_SECTION_BASE_ADDR);

    /*
    *  register a callback function for the BOOT module
    */
    DTL_RegisterMsg(DTL_SERVICE, event, event, TVBOOT_Callback);
    
    event = DTL_CMDQ_DSP_COMMON;
    DTL_RegisterMsg(DTL_SERVICE, event, event, TVBOOT_DSPCommonCallback);
    
    //ARM_COMMON_CODE

    data_len = arm_common_code_wdm_len;
    is_ok = DTL_SendData((uint8*)arm_common_code_wdm,data_len,
                                ARM_COMMON_CODE_BASE_ADDR);
    if( !is_ok )
    {
        BOOT_TRACE("TVBOOT_TVOpen()download ARM_COMMON_CODE error!\n");
        return FALSE;
    }

    //ARM_CODE_IN_SDRAM
    data_len = code_in_sdram_wdm_len;
    is_ok = DTL_SendData((uint8*)code_in_sdram_wdm,data_len,
                                ARM_CODE_IN_SDRAM_BASE_ADDR);
    if( !is_ok )
    {
        BOOT_TRACE("TVBOOT_TVOpen()download ARM_CODE_IN_SDRAM error!\n");
        return FALSE;
    }
    
   
    //scatte_pilot
    data_len = scatte_pilot_len;
    is_ok = DTL_SendData((uint8*)scatte_pilot,data_len,0x90f80000);
        //DEMODULATOR_BUFFER_BASE_ADDR+DEMODULATOR_BUFFER_MAX_SIZE-data_len);
    if( !is_ok )
    {
        BOOT_TRACE("TVBOOT_TVOpen()download scatte_pilot table error!\n");
        return FALSE;
    }
    MTVCFG_GetLCDCfg(&lcd_cfg_data_ptr, &data_len);
    BOOT_TRACE("lcd_cfg_data_ptr=0x%p, data len=%u", lcd_cfg_data_ptr, data_len);
    is_ok = DTL_SendData((uint8*)lcd_cfg_data_ptr,data_len,ARM_CONFIG_LCD_COM_CFG_BASE_ADDR);
    if( !is_ok )
    {
        BOOT_TRACE("TVBOOT_TVOpen()download LCD COMMON config error!\n");
        return FALSE;
    }

    //hold dsp 0x20300000 is seine register
    if( !DTL_GetData((uint8*)&dsp_hold_data,sizeof(uint32),0x20300000))
    {
        return FALSE;
    }
    dsp_hold_data = dsp_hold_data|0x8;
    if ( !DTL_SendData((uint8*)&dsp_hold_data,sizeof(uint32),0x20300000))
    {
        return FALSE;
    }
    
    /* tell Seine to download the code */
    //DTLCMDQ_Download_Arm_Code(0x08000000,0x90000000,arm_common_code_len);
    DTLCMDQ_Download_Arm_Code(0x08000000,ARM_COMMON_CODE_BASE_ADDR,arm_common_code_wdm_len);
    
    while(1)
    {
        SCI_Sleep(1);
        RespQ_ptr = DTL_GetCmd(&cmd_num);
        if ( cmd_num )
        {
            SCI_ASSERT(1==cmd_num); /*assert verified*/
            
            if (BB_ARM_STATUS_DOWNLOAD_ARM_CODE_FINISHED == RespQ_ptr[0].resp_id)
            {
                DTLCMDQ_Jumpto_IRAM_Run();
            }
            else
            {
                SCI_ASSERT(0); /*assert verified*/
            }
            //SCI_FREE(RespQ_ptr);
            break;
        }

        wait_times++;
        if ( wait_times > TVBOOT_MAX_WAIT_TIME )
        {
            //SCI_TRACE_LOW:"TVBOOT_TVOpenWqvga() not read BB_ARM_STATUS_DOWNLOAD_ARM_CODE_FINISHED command"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MTV_BOOT_775_112_2_17_23_9_16_36,(uint8*)"");
            return FALSE;
        }
    }
    
    //s_tv_cur_standard = standard;
    
    if (callback)
    {
        s_tv_callback = callback;
    }
    return TRUE;
}

PUBLIC BOOLEAN TVBOOT_TVOffWqvga(TVBOOT_CALLBACK tv_off_callback)
{
    if (TVBOOT_IsOK())
    {
        DTLCMDQ_Turn_Off_TV();
    }

    if ( tv_off_callback )
    {
        s_tv_off_callback = tv_off_callback;
    }
    
    return TRUE;
}

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

