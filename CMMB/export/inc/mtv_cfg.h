/******************************************************************************
 ** File Name:      mtv_cfg.h                                                 *
 ** Author:         hanbing.zhang                                             *
 ** DATE:           05/08/2008                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic configuration for mtv         *
 **                 by customer.                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/08/2008     hanbing.zhang        Create.                               *
 ******************************************************************************/
#ifndef _MTV_CFG_H_
#define _MTV_CFG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "kd.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/

#define MTV_COMM_CFG_ID			0x20080327

typedef enum MTV_LCD_TYPE_CONFIG_tag
{
    MTV_LCD_TYPE_S6D0139 = 0,
    MTV_LCD_TYPE_R61505MCU,
    MTV_LCD_TYPE_R61505RGB,
    MTV_LCD_TYPE_R61509MCU,
    MTV_LCD_TYPE_LGDP4531MCU,
    MTV_LCD_TYPE_ILI9325MCU,
    MTV_LCD_TYPE_COMM,
    MTV_LCD_TYPE_MAX
}MTV_LCD_TYPE_CONFIG_E;

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
	KDuint32		mode;   //IIS_MODE_E
	KDuint32		iis_clk;
	KDuint32		iis_port;
}IIS_CONFIG_T;

//bb communicate setting
typedef struct{
	KDuint32 sc6600v_cfg_id;
    KDuint32 gpio_out_id;
    KDuint32 gpio_in_id;
    KDuint32 gpio_out_int_type;
    KDuint32 gpio_in_int_type;
    KDuint32 gpio_out_delay;
    KDuint32 lcd_type; 
    KDuint32 arm_log_uart_id;
    KDuint32 dsp_log_uart_id;
    KDuint32 arm_log_cfg;
    /* uart log rate */
    KDuint32 arm_log_uart_rate;

    /* SPI Config */
    KDuint32 arm_spi_port_id;
    KDboolean rx_shift_edge; //Write "1" Enable Rx data shift in at clock neg-edge
    KDboolean tx_shift_edge; //Write "1" Enable Tx data shift out at clock neg-edge
    KDuint8   tx_bit_length; //Transmit data bit number
    KDboolean lsb_first;     //Write "1" enable data transmit/receive from LSB
    KDuint32  freq;          //SPI bus clock
    IIS_CONFIG_T	iis_cfg;
    KDuint32  audio_mode;   //AUDIO_MODE_E
    KDboolean is_dc;         //1: dc application, 0:mp4 application
    KDboolean is_mix_audio_output; //0: output stereo; 1: output mixed
    KDboolean is_sc6600v_descramble;  //0 or 1: demode api mode
                                      //   1 - sc6600v descramble; 0 - host descramble
                                      //0xff: mtv api mode
    KDboolean res2;   
}MTV_COMM_CONFIG_T;



//ddr sdr memory
#define	ROW_SIZE_4Kb		(4*1024)
#define	ROW_SIZE_8Kb		(8*1024)
#define	ROW_SIZE_16Kb		(16*1024)
/* emc device type */
#define	EMC_DEV_DDR			0
#define	EMC_DEV_SDR			1

/* bank mode */
#define BM_2BANK			0
#define	BM_4BANK			1

/* sdram capacity */
#define	CAPACITY_008MBIT		0
#define	CAPACITY_016MBIT		1
#define	CAPACITY_032MBIT		2
#define	CAPACITY_064MBIT		3
#define	CAPACITY_128MBIT		4
#define	CAPACITY_256MBIT		5

#define	WQVGA_DC_ON				1
#define	WQVGA_MP4_ON			0

typedef struct
{
    KDuint32   value_type; //2:无意义；1:表示使用clk作为单位；0:则使用ns时间为单位。
    KDuint32   value;       //对应的值
}VALUE_UNIT_S;


typedef struct
{
    VALUE_UNIT_S rp;        //precharge to active delay, unit(ns)
    VALUE_UNIT_S rcdrd;		//active to read delay, unit(ns)
    VALUE_UNIT_S rcdwr;		//active to write delay,uint(ns)
    
    VALUE_UNIT_S wr;        //write recovery time, unit(ns)
    VALUE_UNIT_S ras;		//active to charge delay,unit(ns)
    VALUE_UNIT_S mrd;		//LOAD MODE REGISTER to other command delay,unit(ns)	
    VALUE_UNIT_S rrd;		//two active to different bank delay
    VALUE_UNIT_S rfc;       //auto refresh period, unit(ns)
    VALUE_UNIT_S wtr;		//write to read  delay

    unsigned int CAS_Latency;   //2 or 3

    unsigned int ref;           //refresh period time, unit(ms)
    unsigned int row_num;       //row number
    unsigned int row_size;       //row capacity
    unsigned int bank_num;	//bank number
    unsigned int capacity;      //unit:Mbit

    unsigned int dev_type;	//
}SDRAM_PARAM_S;

typedef void (*MTVCFG_LCDCFG)(KDuint32 **cfg_pptr, KDuint32 *len_ptr);
typedef struct mtvcfg_lcd_tag
{
    KDuint32 lcd_id;
    MTVCFG_LCDCFG cfg_func;
}MTVCFG_LCD_T;

/* @fool2 */
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

typedef enum
{
    LCD_CTRL_MODE_RGB = 0, 
    LCD_CTRL_MODE_MCU
}LCD_CTRL_MODE_E;

typedef struct lcd_mcu_mode_param_tag
{
    KDuint32     sync_mode;
    KDuint32     bus_mode;
    KDuint32     vsync_pol;
    KDuint32     cd_cmd;    // 1(0): cd pin will be low(high)
                                       // when write cmd to lcd panel;
                                       // 1(0): cd pin will be high(low) 
                                       // when write data to lcd panel;
    KDuint32     data_type;
}LCD_MCU_MODE_PARAM_T;

typedef struct lcd_rgb_timing_param_tag
{
    KDuint8  bfw;             //begin of frame horizontal sync pluse-1, 
                            //that means, if you want 6, then bfw = 5
    KDuint8  efw;             //end of frame horizontal sync pluse-1
    KDuint8  vsw;             //vsync pluse width-1
    KDuint8  blw;             //begin of line pixel clock-1
    KDuint8  elw;             //end of line pixel clock-1
    KDuint8  hsw;             //horizontal sync pluse wait-1
    KDuint8  reserved_0;
    KDuint8  reserved_1;
}LCD_RGB_TIMING_PARAM_T;

typedef struct lcd_mcu_timing_param_tag
{
    KDuint16 vsync_width;     //vysnc width
    KDuint8  whpw;            //write high pulse width
    KDuint8  wlpw;            //write low pulse width
    KDuint8  wcss;            //write cs setup width
    KDuint8  rhpw;            //read high pulse width
    KDuint8  rlpw;            //read low pulse width
    KDuint8  rcss;            //read cs setup width
}LCD_MCU_TIMING_PARAM_T;

typedef struct lcd_op_cell
{
    KDuint8  op_index;
    KDuint8  delay_time;
    KDuint16 reg_index;
    KDuint32 data;
}LCD_OP_CELL;

typedef struct lcd_com_cfg 
{
    KDuint32 ctrl_mode;
    LCD_MCU_MODE_PARAM_T   mode_param;
    LCD_MCU_TIMING_PARAM_T timing_param;
    KDuint16 lcd_gamma_table[20];
    KDuint32 lcd_init_offset;
    KDuint32 lcd_enter_sleep_offset;
    KDuint32 lcd_set_contrast_offset;
    KDuint32 lcd_set_brightness_offset;
    KDuint32 lcd_set_display_window_offset;
    KDuint32 lcd_get_info_offset;
    KDuint32 lcd_get_set_frame_rate_offset;
    KDuint32 lcd_clear_offset;
    KDuint32 lcd_close_offset;
    KDuint32 lcd_open_offset;
    KDuint32 lcd_set_direction_offset;
    KDuint32 res1_offset;
    KDuint32 res2_offset;
    KDuint32 res3_offset;
}LCD_COM_CFG;
/**/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: config 26M clock
//	Global resource dependence: 
//  Author: hanbing.zhang
//	Note:
/*****************************************************************************/
void MTVCFG_Open_26MClock(void);
 
/*****************************************************************************/
//  Description: 
//	Global resource dependence: 
//  Author:
//	Note:
/*****************************************************************************/
void MTVCFG_Close_26MClock(void);

/*****************************************************************************/
//  Description: Get MTV LCD Config
//	Global resource dependence: 
//  Author:
//	Note:
/*****************************************************************************/
void MTVCFG_GetLCDCfg(KDuint32 **cfg_pptr,
                             KDuint32 *len_ptr);

/*****************************************************************************/
//  Description: PowerDown Camera
//	Global resource dependence: 
//  Author: tom.wu
//	Note:
/*****************************************************************************/
void MTVCFG_CameraPowerDown(void);

/*****************************************************************************/
//  Description: Set WQVGA DC/MP4 ON
//	Global resource dependence: 
//  Author:
//	Note:
/*****************************************************************************/
void MTVCFG_SetDCMP4On(KDboolean is_on);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // _SLEEP_CFG_H_

