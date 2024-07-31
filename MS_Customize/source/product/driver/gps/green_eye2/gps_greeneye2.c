/******************************************************************************
 ** File Name:      Gps_SiRF.c                                                *
 ** Author:         David.Jia                                                 *
 ** DATE:           7/31/2007                                                 *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    SiRF GPS interface.                                       *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE            NAME            DESCRIPTION                               * 
 ** 07/31/2007      David.Jia       SiRF GPS interface base on Gps_drv.       *
 ** 08/02/2007      David.Jia       Change names of COM_Init/COM_Close/Map_Read/Map_Write 
 **     to GPS_ComInit/GPS_ComClose/GPS_ComRead/GPS_ComWrite.
 ** 08/14/2007      David.Jia       Add gpio control.                         *
 ** 09/04/2007      David.Jia       Modify Srf_Open for download firmware.    *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "gps_drv.h"
#include "nv_productionparam_type.h"
#include "sci_types.h"
#include "GPS_COM.h"
#include "gpio_prod_api.h"
#include "gps_nmea.h"
#include "greeneye2.h"
#include "greeneye2_common.h"
#include "gps_interface.h"
#include "env_gps_signal.h"
#include "mn_gps_signal_def.h"
#include "greeneye2_gnss_log.h"
#include "ldo_drvapi.h"
#include "global_ctl_drvapi.h"

#ifdef GPS_TCARD_LOG_SUPPORT
#include "greeneye2_tcard_log.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/******************************************************************************
 * Static Global Variables
 ******************************************************************************/

GpsCallbacks         callbacks;

GpsInterface *gpsInterface = NULL;

#if 0
GPSNMEA_DATA nmea_data[GPS_TYPE_MAX];
#endif

uint32 nmea_output_mode = 0;

BLOCK_ID usr_send_id = SCI_INVALID_BLOCK_ID;

static uint8 s_nmealog_enable = 0x0;          //only use ouput to diag
static uint8 s_debuglog_enable = 0x0;          //only use ouput to diag

static GpsSvStatus SvState;
/**---------------------------------------------------------------------------*
 **                     Function Prototypes                             *
 **---------------------------------------------------------------------------*/
#if 0
/*****************************************************************************/
//  Description:    Greeneye2_Partition
//  Author:        
//  Note:           
/*****************************************************************************/
LOCAL int Greeneye2_Partition(GpsSvInfo *data,int low,int high)
{
    GpsSvInfo cell_data;
    cell_data = data[low];
    while(low < high)
    { 
	while(low < high && data[high].snr<= cell_data.snr)
		high--;
	
	data[low] = data[high];
	
	while(low < high && data[low].snr >= cell_data.snr)
		low++;
	
	data[high] = data[low];
    }
    data[low] = cell_data;
    return low;
}

/*****************************************************************************/
//  Description:    Greeneye2_Quicksort
//  Author:        
//  Note: //快排每趟进行时的枢轴要重新确定，由此进 //一步确定每个待排小记录的low及high的值
/*****************************************************************************/
LOCAL void Greeneye2_Quicksort(GpsSvInfo *data,int low,int high) 
{
    int pivotloc = 0;

    if(low >= high)
	return ;
    pivotloc = Greeneye2_Partition(data,low,high);
    Greeneye2_Quicksort(data,low,pivotloc-1);
    Greeneye2_Quicksort(data,pivotloc+1,high);
}
#endif

/*****************************************************************************/
//  Description:    Greeneye2_GetSvInfo
//  Author:         
//  Note: Callback with location information          
/*****************************************************************************/
void Greeneye2_GetSvInfo(GpsSvStatus *svinfo)
{
    //Greeneye2_Quicksort(SvState.sv_list,0, SvState.num_svs-1);

    memcpy(svinfo,&SvState,sizeof(GpsSvStatus));
}

/*****************************************************************************/
//  Description:    GreenEye2_location_callback
//  Author:         
//  Note: Callback with location information          
/*****************************************************************************/
void GreenEye2_location_callback(GpsLocation* location)
{
    //GREENEYE2_LOGD(("[GE2]gps_location_callback"));
}

/*****************************************************************************/
//  Description:    GreenEye2_status_callback
//  Author:         
//  Note: Callback with status information.          
/*****************************************************************************/
void GreenEye2_status_callback(GpsStatus* status)
{
    //GREENEYE2_LOGD(("[GE2]gps_status_callback"));
}

/*****************************************************************************/
//  Description:    GreenEye2_err_occurs
//  Author:         
//  Note: Callback with status information.          
/*****************************************************************************/
void GreenEye2_err_occurs()
{
    xSignalHeaderRec *signal = NULL;
    GREENEYE2_LOGD(("[GE2]GreenEye2_err_occurs"));
    SCI_CREATE_SIGNAL(
        signal,
        MN_GPS_ERR_IND,
        sizeof (xSignalHeaderRec),
        SCI_IDENTIFY_THREAD());
    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, usr_send_id);
    //SCI_PASSERT(0, ("GreenEye2_err_occurs!"));
}

/*****************************************************************************/
//  Description:    GreenEye2_status_callback
//  Author:         
//  Note: Callback with SV status information.         
/*****************************************************************************/
void GreenEye2_sv_status_callback(GpsSvStatus* sv_info)
{
    //GREENEYE2_LOGD(("[GE2]gps_sv_status_callback"));
    memcpy(&SvState,sv_info,sizeof(GpsSvStatus));

}

/*****************************************************************************/
//  Description:    GreenEye2_send_nmea_callback
//  Author:         
//  Note: Callback for reporting NMEA sentences.      
/*****************************************************************************/
GPS_NMEALOG_OUTPUT_T nmealog_output;
void GreenEye2_nmea_callback(GpsUtcTime timestamp, const char* nmea, int length)
{
    char gnsslog_begin[8] = "GLB@";
    char gnsslog_end[8] = "GLE@";
    static uint32 rflog_output_len = 0;
    
    if(length <= 0)
        return;

    if(s_debuglog_enable)
    {
	TM_SendTestPointRequest(timestamp,924198);
        if(GNSS_TYPE_LOG_RF == timestamp)
        {
            if(!rflog_output_len)
                SCI_MEMSET(&nmealog_output,0x0,sizeof(nmealog_output));

            SCI_MEMCPY(&(nmealog_output.nmealog[rflog_output_len]),nmea,length);
            rflog_output_len += length;

            if(nmealog_output.nmealog[rflog_output_len-1] != 0x0A)
            {
                return;
            }
            else
            {
                length = rflog_output_len;
                rflog_output_len = 0;
            }
            nmealog_output.msg_head.subtype = 0x5;
        }
        else if(GNSS_TYPE_LOG_NMEA == timestamp)
        {
            SCI_MEMSET(&nmealog_output,0x0,sizeof(nmealog_output));
            SCI_MEMCPY(nmealog_output.nmealog,nmea,length);
            if(s_nmealog_enable)
                nmealog_output.msg_head.subtype = 0x5;
            else
                nmealog_output.msg_head.subtype = 0x6;
        }
        else if(GNSS_TYPE_MINILOG_TRACE == timestamp)
        {   
            SCI_MEMSET(&nmealog_output,0x0,sizeof(nmealog_output));
            SCI_MEMCPY(nmealog_output.nmealog,gnsslog_begin,strlen(gnsslog_begin));
            SCI_MEMCPY(&(nmealog_output.nmealog[strlen(gnsslog_begin)]),nmea,length);
            length += strlen(gnsslog_begin);
            if(nmealog_output.nmealog[length-1] != 0x0A)
            {
                nmealog_output.nmealog[length] = 0x0D;
                nmealog_output.nmealog[length+1] = 0x0A;
                length += 2;
            }
            SCI_MEMCPY(&(nmealog_output.nmealog[length]),gnsslog_end,strlen(gnsslog_end));
            length += strlen(gnsslog_end);
            nmealog_output.msg_head.subtype = 0x6;
        }
        else if(GNSS_TYPE_LOG_TRACE == timestamp)
        {
            SCI_MEMSET(&nmealog_output,0x0,sizeof(nmealog_output));
            SCI_MEMCPY(nmealog_output.nmealog,nmea,length);
            nmealog_output.msg_head.subtype = 0x6;
        }
        else
        {
            return;
        }

        nmealog_output.msg_head.seq_num = 0x0;
        nmealog_output.msg_head.type = 0x3A;

        nmealog_output.msg_head.len = sizeof (MSG_HEAD_T)+length;
        SIO_SendPacket((uint8 *)(&nmealog_output),length+sizeof(MSG_HEAD_T));
#ifdef GPS_TCARD_LOG_SUPPORT
	if(2 == SCI_GetGpsLogValue())
	{
		Gps_CardLogWrite(GPS_TCARDLOG_TYPE_ALL,nmealog_output.nmealog,length);
	}
#endif
    }
}

/*****************************************************************************/
//  Description:    GreenEye2_send_nmea_callback
//  Author:         
//  Note: Callback for reporting NMEA sentences.      
/*****************************************************************************/
void GreenEye2_send_nmea_callback(GPSNMEA_TYPE type, const char* nmea, int length)
{
    MN_GPS_OUTPUT_INFO_IND_T *signal = PNULL;
    GPS_DIAG_OUTPUT_NMEALOG_T *diag_signal = PNULL;


    //GREENEYE2_LOGD(("[GE2]GreenEye2_send_nmea_callback"));
    #if 0
    SCI_MEMSET(nmea_data[type].data,0,NMEA_MAX_SIZE+1);
    SCI_MEMCPY(nmea_data[type].data,nmea,length);
    GREENEYE2_LOGD(("[GE2]gps_nmea_callback,nmea_data[%d].data=%s\n",type,nmea_data[type].data));
    #endif

    if(nmea_output_mode != 0)
    {
        SCI_CREATE_SIGNAL(
        signal,
        MN_GPS_OUTPUT_INFO_IND,
        sizeof (MN_GPS_OUTPUT_INFO_IND_T),
        SCI_IDENTIFY_THREAD());

        switch(type)
        {
            case GPS_GGA_TYPE:
                if(nmea_output_mode & BIT_0)
                {
                    signal->length = length;
                    SCI_MEMCPY(signal->nmea_data,nmea,length);
                    signal->type = BIT_0;
                }
                break;
            case GPS_GLL_TYPE:
                if(nmea_output_mode & BIT_1)
                {
                    signal->length = length;
                    SCI_MEMCPY(signal->nmea_data,nmea,length);
                    signal->type = BIT_1;
                }
                break;
            case GPS_GSA_TYPE:
                if(nmea_output_mode & BIT_2)
                {
                    signal->length = length;
                    SCI_MEMCPY(signal->nmea_data,nmea,length);
                    signal->type = BIT_2;
                }
                break;
            case GPS_GSV_TYPE:
                if(nmea_output_mode & BIT_3)
                {
                    signal->length = length;
                    SCI_MEMCPY(signal->nmea_data,nmea,length);
                    signal->type = BIT_3;
                }
                break;
            case GPS_RMC_TYPE:
                if(nmea_output_mode & BIT_4)
                {
                    signal->length = length;
                    SCI_MEMCPY(signal->nmea_data,nmea,length);
                    signal->type = BIT_4;
                }
                break;
            case GPS_VTG_TYPE:
                if(nmea_output_mode & BIT_5)
                {
                    signal->length = length;
                    SCI_MEMCPY(signal->nmea_data,nmea,length);
                    signal->type = BIT_5;
                }
                break;
            default:
                break;
        }
        
        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, usr_send_id);   
    }
}

/*****************************************************************************/
//  Description:    GreenEye2_status_callback
//  Author:         
//  Note: Callback to inform framework of the GPS engine's capabilities.
//            Capability parameter is a bit field of GPS_CAPABILITY_* flags.      
/*****************************************************************************/
void GreenEye2_set_capabilities(uint32 capabilities)
{
    //GREENEYE2_LOGD(("[GE2]gps_set_capabilities"));
}

/*****************************************************************************/
//  Description:    GreenEye2_status_callback
//  Author:         
//  Note: Callback for requesting NTP time.      
/*****************************************************************************/
void GreenEye2_request_utc_time()
{
    //GREENEYE2_LOGD(("[GE2]gps_request_utc_time"));
}

/*****************************************************************************/
//  Description:    Control GPS's power
//                  is_open = SCI_TRUE, 	power on
//                  is_open = SCI_FALSE, 	power off
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void GreenEye2_GPIO_PowerOn(BOOLEAN is_open)
{
#if defined(HW_ALPHA_CUSTOMER)
	//Turn on LDO
	if(is_open == TRUE){
		//LDO_SetVoltValue(LDO_LDO_CAMA,1800);//1.8V
		//LDO_TurnOnLDO(LDO_LDO_CAMA);//1,8V

		LDO_SetVoltValue(LDO_LDO_CAMD0,1800);//1.8V
		LDO_TurnOnLDO(LDO_LDO_CAMD0);//1,8V

		//LDO_SetVoltValue(LDO_LDO_SDIO,1600);//1.6V
		//LDO_TurnOnLDO(LDO_LDO_SDIO);
	}else{
		//LDO_TurnOffLDO(LDO_LDO_CAMA);
		LDO_TurnOffLDO(LDO_LDO_CAMD0);
		//LDO_TurnOffLDO(LDO_LDO_SDIO);
	}
    SCI_Sleep(10);
    GPIO_SetValue( GPIO_GPS_VDDWCN_EN, is_open);
#endif
    GPIO_SetValue( GPIO_GPS_CHIP_EN, is_open);
    SCI_Sleep(10);
}

/*****************************************************************************/
//  Description:    config greeneye2 pinmap
//  Author:         
//  Note:           this config is used uart1 port,if you use other port pls modify.
/*****************************************************************************/
LOCAL void GreenEye2_GPIO_PinCfg(BOOLEAN is_gpio)
{
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_SIRF_127_112_2_18_0_33_22_1692,(uint8*)"d", is_gpio);
	if(GPS_COM==0)
	{
		CHIP_REG_SET(PIN_SPI0_CLK_REG,(PIN_SLP_Z|PIN_SPX_EN|PIN_FUNC_1|PIN_FPX_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//HZ-TXD
		CHIP_REG_SET(PIN_SPI0_CS0_REG,(PIN_SLP_Z|PIN_SPX_EN|PIN_FUNC_3|PIN_FPU_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//HZ-RXD

		CHIP_REG_SET(PIN_U0CTS_REG, (PIN_SLP_Z|PIN_SPD_EN|PIN_FUNC_0|PIN_FPU_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//BB_U0CTS
		CHIP_REG_SET(PIN_U0RTS_REG, (PIN_SLP_Z|PIN_SPD_EN|PIN_FUNC_0|PIN_FPX_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//BB_U0RTS
		CHIP_REG_SET(PIN_U0RXD_REG, (PIN_SLP_Z|PIN_SPD_EN|PIN_FUNC_0|PIN_FPD_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//BB_U0RXD
		CHIP_REG_SET(PIN_U0TXD_REG, (PIN_SLP_Z|PIN_SPD_EN|PIN_FUNC_0|PIN_FPD_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//BB_U0TXD
		GlB_CTL_PORT_SEL(UART0,0);

		CHIP_REG_SET(PIN_PWM_REG,     (PIN_SLP_Z|PIN_SPX_EN|PIN_FUNC_2|PIN_FPX_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//32K clock            
		//CHIP_REG_SET(PIN_IISLRCK_REG, (PIN_SLP_OE|PIN_SPX_EN|PIN_FUNC_3|PIN_FPD_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//gpio19
		GPIO_SetDataMask (GPIO_GPS_CHIP_EN,SCI_TRUE);
		GPIO_SetDirection (GPIO_GPS_CHIP_EN,SCI_TRUE);
		GPIO_DisableIntCtl (GPIO_GPS_CHIP_EN);
		GPIO_SetValue (GPIO_GPS_CHIP_EN,SCI_FALSE); 
	}
	else
	{
		CHIP_REG_SET(PIN_SPI0_DI_REG,         (PIN_SLP_IE|PIN_SPX_EN|PIN_FUNC_2|PIN_FPU_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//RX
		CHIP_REG_SET(PIN_SPI0_DO_REG,       (PIN_SLP_OE|PIN_SPX_EN|PIN_FUNC_2|PIN_FPU_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//TX
		CHIP_REG_SET(PIN_SPI0_CLK_REG,        (PIN_SLP_Z|PIN_SPX_EN|PIN_FUNC_2|PIN_FPU_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//rts
		CHIP_REG_SET(PIN_SPI0_CS0_REG,        (PIN_SLP_Z|PIN_SPX_EN|PIN_FUNC_2|PIN_FPU_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//cts

		CHIP_REG_SET(PIN_PWM_REG,            (PIN_SLP_Z|PIN_SPX_EN|PIN_FUNC_2|PIN_FPX_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN)); //GPS CLK 32K
		CHIP_REG_SET(PIN_EXTINT0_REG,        (PIN_SLP_OE|PIN_SPD_EN|PIN_FUNC_3|PIN_FPD_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//gpio68 GPS_CHIP_EN

#if defined(HW_ALPHA_CUSTOMER)
		CHIP_REG_SET(PIN_IISDO_REG, (PIN_SLP_OE|PIN_SPX_EN|PIN_FUNC_3|PIN_FPD_EN|PIN_BSR_WPX|PIN_DS_0|PIN_DSLP_EN));//gpio17 GPS_VDDWCN_EN
#endif

#if defined(HW_ALPHA_CUSTOMER)
		GPIO_SetDataMask (GPIO_GPS_VDDWCN_EN,SCI_TRUE);
		GPIO_SetDirection (GPIO_GPS_VDDWCN_EN,SCI_TRUE);
		GPIO_DisableIntCtl (GPIO_GPS_VDDWCN_EN);
		GPIO_SetValue (GPIO_GPS_VDDWCN_EN,SCI_FALSE); 
#endif

		GPIO_SetDataMask (GPIO_GPS_CHIP_EN,SCI_TRUE);
		GPIO_SetDirection (GPIO_GPS_CHIP_EN,SCI_TRUE);
		GPIO_DisableIntCtl (GPIO_GPS_CHIP_EN);
		GPIO_SetValue (GPIO_GPS_CHIP_EN,SCI_FALSE);

		*((volatile uint32 *)(APB_PIN_CTL0)) &= (~UART1_SEL_MSK); //select UART1_0
		*((volatile uint32 *)(APB_PIN_CTL0)) |= (0x4 << UART1_SEL_SHIFT); //select UART1_2
		UART1ArbitrateSwitch (UART_CONTROL_BY_MCU);

		
	}
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Download
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
LOCAL GPS_ERR_E GreenEye2_Download(void)
{
    GPS_MODE_E mode;
    GreenEye2_GPIO_PinCfg(SCI_TRUE); 
    GreenEye2_GPIO_PowerOn(SCI_TRUE);
#ifdef GPS_MODE_GPS_BD
    mode = GPS_MODE_BD;
#else 
    mode = GPS_MODE_GLONASS;
#endif

    gps_setPowerState(DEVICE_DOWNLOADING);
    if(GPS_DLGreenEye2(mode))
    {
        gps_setPowerState(DEVICE_DOWNLOADED);
        return GPS_ERR_NONE;
    }
    GreenEye2_GPIO_PowerOn(SCI_FALSE);
    GreenEye2_GPIO_PinCfg(SCI_FALSE); 
    GREENEYE2_LOGD(("[GE2] greeneye2 download fail!\n"));
    return GPS_ERR_NO_MODULE;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_DownloadFdl
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
LOCAL GPS_ERR_E GreenEye2_DownloadFdl(void)
{
	if(!GPS_DLGreenEye2FDL())
		return GPS_ERR_NO_MODULE;
	return GPS_ERR_NONE;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Init
//  Description:    hardware initialization, called on cell power on.
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           u1rxd/u1txd,  pin select.
/*****************************************************************************/
LOCAL GPS_ERR_E GreenEye2_Init(void)
{
    struct gps_device_t *gps_device = NULL;
    int ret = 0;

#if 0
    nmea_data[GPS_GGA_TYPE].mode = GPS_GGA_MODE;
    nmea_data[GPS_GLL_TYPE].mode = GPS_GLL_MODE;  
    nmea_data[GPS_GSA_TYPE].mode = GPS_GSA_MODE;
    nmea_data[GPS_GSV_TYPE].mode = GPS_GSV_MODE;
    nmea_data[GPS_RMC_TYPE].mode = GPS_RMC_MODE;
    nmea_data[GPS_VTG_TYPE].mode = GPS_VTG_MODE;
#endif
    gps_open(&gps_device);

    gpsInterface = gps_device->get_gps_interface(gps_device);

    callbacks.size = sizeof(GpsCallbacks);
    callbacks.location_cb = GreenEye2_location_callback;
    callbacks.status_cb = GreenEye2_status_callback;
    callbacks.sv_status_cb = GreenEye2_sv_status_callback;
    callbacks.nmea_cb = GreenEye2_nmea_callback;
    callbacks.set_capabilities_cb = GreenEye2_set_capabilities;
    callbacks.request_utc_time_cb = GreenEye2_request_utc_time;
    callbacks.err_occurs_cb = GreenEye2_err_occurs;
    callbacks.send_nmea_cb = GreenEye2_send_nmea_callback;
    
    if(PNULL != gpsInterface)
    {
        ret = gpsInterface->init(&callbacks);
        if(ret < 0)
        {
            GREENEYE2_LOGD(("[GE2] greeneye2 init fail!\n"));
            return GPS_ERR_NO_MODULE;
        }

        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Open
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
LOCAL GPS_ERR_E GreenEye2_Open(BLOCK_ID sender_id)
{
    int ret = 0;
#if 0
    uint8 i = 0;
    for(i = 0;i < GPS_TYPE_MAX;i++)
    {
        SCI_MEMSET(nmea_data[i].data,0,NMEA_MAX_SIZE+1);
    }
#endif
    usr_send_id = sender_id;
    if(PNULL != gpsInterface)
    {
        ret = gpsInterface->start();
        if(ret < 0)
        {
            GREENEYE2_LOGD(("[GE2] greeneye2 start fail!\n"));
            return GPS_ERR_PARAM;
        }
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Close
//  Description:    called after GPS work.
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
LOCAL GPS_ERR_E GreenEye2_Close(void)
{
    int ret = 0;
    if(PNULL != gpsInterface)
    {
        ret = gpsInterface->stop();
        if(ret < 0)
        {
            GREENEYE2_LOGD(("[GE2] greeneye2 init fail!\n"));
            return GPS_ERR_PARAM;
        }
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_ReadData
//  Description:    call Map_Read
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
uint32 GreenEye2_ReadData(uint8* read_buf_ptr, uint32 byte_to_read)
{
    uint32 ret_val = 0;

    return ret_val;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_ReadInfoData
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
uint32 GreenEye2_ReadInfoData(char** read_buf_ptr, uint32 byte_to_read)
{
    uint32 ret_val = 0;
    
    if(PNULL != gpsInterface)
    {
        ret_val = gpsInterface->getposition(read_buf_ptr);
    }
    
    return ret_val;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_WriteData
//  Description:    call Map_Write
//  return:         
//  INPUT:          
//  Author:        
//  date:           
//	Note:           
/*****************************************************************************/
uint32 GreenEye2_WriteData(uint8* read_buf_ptr, uint32 byte_to_read)
{
    return 0;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_WriteAgpsData
//  Description:    
//  return:         
//  INPUT:          
//  Author:        
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_WriteAgpsData(char* agps_data, uint32 length)
{
    if(PNULL != gpsInterface)
    {
	gpsInterface->writeagpsdata(agps_data,length);
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_SetStartMode
//  Description:    call Map_Write
//  return:         
//  INPUT:          
//  Author:        
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_SetRestart(uint32 mode)
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->setrestart(mode);
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Identify(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
BOOLEAN GreenEye2_Identify(void)
{
    return SCI_TRUE;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_GetNmeaData(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_GetNmeaData(uint32 mode,uint8 *buff)
{
    #if 0
    uint8 i = 0;
    for(i = 0;i < GPS_TYPE_MAX;i++)
    {
        if(nmea_data[i].mode == mode)
        {
            SCI_MEMCPY(buff,nmea_data[i].data,strlen(nmea_data[i].data));
        }
    }
    GREENEYE2_LOGD(("[GE2] GreenEye2_GetNmeaData,mode = %d,buff=%s\n",mode,buff));
    if(GPS_TYPE_MAX == i)
        return GPS_ERR_PARAM;
    #endif
    return GPS_ERR_NONE;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_SetOutputMode(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_SetOutputMode(uint32 mode,uint32 time_interval,BLOCK_ID sender_id)
{
    nmea_output_mode = mode;
    usr_send_id = sender_id;
    if(PNULL != gpsInterface)
    {
        gpsInterface->settimerinterval(time_interval);
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_PowerOff(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_PowerOff()
{
    unsigned char readBuffer[256];
    int readLen = 0;

    if(PNULL != gpsInterface)
    {
        gpsInterface->cleanup();
	gpsInterface = PNULL;
        nmea_output_mode = 0x0;
        GreenEye2_GPIO_PowerOn(SCI_FALSE);
	while(1)
	{
		readLen = UART_ReadData(GPS_COM,readBuffer,255);
		if(0 == readLen)
			break;
		else
			SCI_Sleep(5);
	}
	gps_com_close(GPS_COM);	
        //GreenEye2_GPIO_PinCfg(SCI_FALSE); 
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_SendEphData(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_SendEphData()
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->sendephdata();
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_SendWatchdog(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_SendWatchdog()
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->sendwatchdog();
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_ClearEphData(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_ClearEphData()
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->clearephdata();
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_ClearAlmData(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_ClearAlmData()
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->clearalmdata();
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_PowerOff(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           only use output to diag
/*****************************************************************************/
GPS_ERR_E GreenEye2_DebugLog_Enable(uint8 status)
{
    s_debuglog_enable = status;
    return GPS_ERR_NONE;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_NmeaLog_Enable(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           only use output to diag
/*****************************************************************************/
GPS_ERR_E GreenEye2_NmeaLog_Enable(uint8 status)
{
    s_nmealog_enable = status;
    return GPS_ERR_NONE;
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_PowerOff(void)
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           only use output to diag
/*****************************************************************************/
GPS_ERR_E GreenEye2_TestMode_Enable(uint8 status)
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->testmodeenable(status);
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Set_Assert
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_Set_Assert(uint8 assertmode)
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->setassert(assertmode);
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Set_RfTestMode
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           only use output to diag
/*****************************************************************************/
GPS_ERR_E GreenEye2_Set_RfTestMode(GPS_RF_TEST_MODE_E rf_mode)
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->setrftestmode(rf_mode);
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Enter_Sleep
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_Enter_Sleep()
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->entersleep();
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/*****************************************************************************/
//  FUNCTION:       GreenEye2_Lte_Open
//  Description:    
//  return:         
//  INPUT:          
//  Author:         
//  date:           
//	Note:           
/*****************************************************************************/
GPS_ERR_E GreenEye2_Lte_Open(uint8 lte_open)
{
    if(PNULL != gpsInterface)
    {
        gpsInterface->lteopen(lte_open);
        return GPS_ERR_NONE;
    }
    else
    {
        return GPS_ERR_NO_MODULE;
    }
}

/**************************************************************************************/
// GPS interface
/**************************************************************************************/
PUBLIC GPS_OPERATIONS_T g_greeneye2_gps_operation = 
{
    GreenEye2_Init,
    GreenEye2_Open,
    GreenEye2_Close,
    GreenEye2_ReadData,           
    GreenEye2_WriteData,
    GreenEye2_Identify,
    GreenEye2_SetRestart,
    GreenEye2_GetNmeaData,
    GreenEye2_SetOutputMode,
    GreenEye2_Download,
    GreenEye2_PowerOff,
    GreenEye2_ReadInfoData,
    GreenEye2_WriteAgpsData,
    GreenEye2_DebugLog_Enable,
    GreenEye2_NmeaLog_Enable,
    Greeneye2_GetSvInfo,
    GreenEye2_TestMode_Enable,
    GreenEye2_Set_RfTestMode,
    GreenEye2_SendEphData,
    GreenEye2_ClearEphData,
    GreenEye2_SendWatchdog,
    GreenEye2_ClearAlmData,
    GreenEye2_DownloadFdl,
    GreenEye2_Set_Assert,
    GreenEye2_Enter_Sleep,
    GreenEye2_Lte_Open,
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

// End of Gps_SIRF.c
