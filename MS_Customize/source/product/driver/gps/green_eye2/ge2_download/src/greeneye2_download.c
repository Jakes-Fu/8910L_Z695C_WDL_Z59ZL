/******************************************************************************
 ** File Name:   greeneye2_download.c                                                *
 ** Author:                                                     *
 ** DATE:                                                         *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                                                                           *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_api.h"
#include "com_drvapi.h"
#include "gps_com.h"
#include "greeneye2_download.h"
#include "gps_drv.h"
#ifdef GPS_MODE_GPS_BD
#include "greeneye2_bd.h"
#else
#include "greeneye2_glonass.h"
#endif
#include "busmonitor_drvapi.h"
#include "sc6531efm_int_cfg.h"
#include "chip_drv_common_inter.h"
#include "greeneye2_packet.h"
#include "threadx_os.h"
#include "threadx_assert.h"
#include "os_api.h"
#include "priority_system.h"
#include "uart_drvapi.h"
#include "greeneye2_gnssfdl.h"

#include "com_drv.h"
#include "uartcom_drv.h"
#include "uart_drvapi.h"
#include "sc_reg.h"
#include "tb_drv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         variable section                                  *
 **---------------------------------------------------------------------------*/
pmanager_t pmanager;

#define BSL_UART_PACKET 0
#define BSL_SPI_PACKET 1

typedef enum 
{
	GPS_DL_BD = 0x0,
	GPS_DL_GLONASS,
	GPS_DL_FDL,
	GPS_DL_MAX
}GPS_DL_E;

struct image_info download_image_info[] = {
    {
        // BD
        GPS_DL_BD,
        NULL,
        //0x1400, 0x200120, 0x0
        0x57800, 0x200120, 0x0,
    },
    {
        // GLONASS
        GPS_DL_GLONASS,
        NULL,
        0x57800, 0x200120, 0x0,
    },
    {
    	GPS_DL_FDL,
	NULL,
	0x1400,0x200120,0x0,
     },
};

typedef enum _GPSCMD_TYPE
{
    GPS_CMD_TYPE_MIN,
    GPS_CMD_TYPE_CONNENT_DEVICE = 0x0,
    GPS_CMD_TYPE_SET_BAUDRATE,
    GPS_CMD_TYPE_SEND_MSG,
    GPS_CMD_TYPE_SEND_START_DATA,
    GPS_CMD_TYPE_SEND_MIDST_DATA,
    GPS_CMD_TYPE_SEND_END_DATA,
    GPS_CMD_TYPE_SEND_EXEC_DATA,
    GPS_CMD_TYPE_MAX
} GPSCMD_TYPE;

typedef int (*GPSCMDPROC) (GPS_PACKET_T *, void *);

typedef struct _CMDPROC_TAB
{
    GPSCMDPROC    proc;
    void      *arg;
} GPSCMDPROC_TAB, *PGPSCMDPROC_TAB;

GPSCMDPROC_TAB gps_proctab[GPS_CMD_TYPE_MAX - GPS_CMD_TYPE_MIN] = {0, 0};/*lint !e656*/

#define IS_VALID_CMD(cmd)           ((cmd >= GPS_CMD_TYPE_MIN) && (cmd < GPS_CMD_TYPE_MAX))
#define CMD_IND(cmd)                ((cmd) - GPS_CMD_TYPE_MIN)
#define CMD_PROC(cmd)               (gps_proctab[CMD_IND(cmd)].proc)
#define CMD_ARG(cmd)                (gps_proctab[CMD_IND(cmd)].arg)
#define CALL_PROC(cmd, packet)      (CMD_PROC(cmd)((packet), CMD_ARG(cmd)))

GPS_PACKET_T gps_packet;

#define GPS_DOWNLOAD_SUCCESS    1
#define GPS_DOWNLOAD_FAILURE    0

#define DELAY_CYCLE(n)  {\
                                            uint32 k=0;\
                                             for(k=0; k<n; k++){}\
                                         }

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    
// Global resource dependence : 
// Author :         
// Note :
/*****************************************************************************/
static int GPS_DLEntry(GPS_MODE_E mode); 
static void GPS_DLPowerOn(BOOLEAN enable);
static void GPS_DLChipEn(BOOLEAN enable);
static int GPS_DLComClose(int uart_port);
static int GPS_DLImage(int uart_port, struct image_info *info);
static void GPS_DL_UartCallBack(uint32 event);
static void GPS_DlInit (int uart_port,GPS_MODE_E mode);
static unsigned char GPS_DlReg (GPSCMD_TYPE cmd, GPSCMDPROC proc, void *arg);
static  int GPS_DLComInit(uint32 port,uint32 BaudRate);
void StartGpsThread(void);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    GPS_DlInit
//  Author:         
//  Note:          
/*****************************************************************************/
static void GPS_DlInit (int uart_port,GPS_MODE_E mode)
{
    char *data_buffer = NULL;
    uint32 image_size = 0;
    
    memset (gps_proctab, 0, sizeof (gps_proctab));
    
    gps_packet.uart_port = uart_port;
    gps_packet.flag = 0;
    gps_packet.uart_speed = 2000000;
    gps_packet.gps_image_info = &download_image_info[0];
    
    if(GPS_MODE_BD == mode)
        gps_packet.mode = GPS_DL_BD;
    else if(GPS_MODE_GLONASS == mode)
        gps_packet.mode = GPS_DL_GLONASS;
    else
	gps_packet.mode = GPS_DL_FDL;
    
    image_size = GPS_DLGetImage(&(gps_packet.gps_image_info[gps_packet.mode].image_data),gps_packet.mode);
    gps_packet.gps_image_info[gps_packet.mode].image_size = image_size;
    
    gps_packet.data_size = DL_PACKET_SIZE;
    gps_packet.translated_size = 0;
    gps_packet.gps_cmd_type = GPS_CMD_TYPE_CONNENT_DEVICE;

}

/*****************************************************************************/
//  Description:    GPS_DlReg
//  Author:         
//  Note:          
/*****************************************************************************/
static unsigned char GPS_DlReg (GPSCMD_TYPE cmd, GPSCMDPROC proc, void *arg)
{
    if (!IS_VALID_CMD (cmd))
    {
        return 0;
    }

    CMD_PROC (cmd) = proc; /*lint !e656*/
    CMD_ARG (cmd) = arg;   /*lint !e656*/
    return 1;
}

/*****************************************************************************/
//  Description:    GPS_DLGreenEye2
//  Author:         
//  Note:          
/*****************************************************************************/
int GPS_DLGreenEye2(GPS_MODE_E mode)
{
    int ret = 0;
    uint32 loop_count = 0;
    GPS_PACKET_T *packet = &gps_packet;
    int image_size;
    char *data_buffer;
    int size = 0;
    int count = 0;
    int timeout_count = 0;
    uint32 i,tmp;
    BOOLEAN flag = SCI_TRUE;
    uint32 tick_begin = 0;
    uint32 tick_end = 0;

    gps_packet.download_state = GPS_DL_INIT;

    GPS_DlInit(GPS_COM,mode);
    send_data_init(flag);
    flag = SCI_FALSE;
    GPS_DlReg (GPS_CMD_TYPE_CONNENT_DEVICE,    send_to_connect_device,  0);
    GPS_DlReg (GPS_CMD_TYPE_SET_BAUDRATE, send_uart_speed_message,   0);
    GPS_DlReg (GPS_CMD_TYPE_SEND_MSG, send_connect_message,   0);
    GPS_DlReg (GPS_CMD_TYPE_SEND_START_DATA,  send_start_message,    0);
    GPS_DlReg (GPS_CMD_TYPE_SEND_MIDST_DATA,  send_data_message,    0);
    GPS_DlReg (GPS_CMD_TYPE_SEND_END_DATA,  send_end_message,    0);
    GPS_DlReg (GPS_CMD_TYPE_SEND_EXEC_DATA,  send_exec_message,    0);

    ret = GPS_DLComInit(GPS_COM, 115200);
    if (ret != 0) 
    {
        DOWNLOAD_LOGE(("[GPS_DL]open_uart_device fail\n"));
         return GPS_DOWNLOAD_FAILURE;
    }

    GPS_Uart_DmaStart(GPS_COM);
    
    image_size = gps_packet.gps_image_info[gps_packet.mode].image_size;
    data_buffer = gps_packet.gps_image_info[gps_packet.mode].image_data;
    gps_packet.download_state = GPS_DL_START;

    SCI_Sleep(100);
    if(GPS_COM)
        CHIPDRV_EnableIRQINT (TB_UART1_INT);
    else
        CHIPDRV_EnableIRQINT (TB_UART0_INT);

    tick_begin = SCI_GetTickCount();

    do {
            switch(packet->download_state)
            {
                case GPS_DL_START:
                        if(!gps_packet.gps_cmd_type)
                        {
                            DOWNLOAD_LOGD(("[GPS_DL]gnss_download start\n"));
                            gps_packet.download_state = GPS_DL_RUNNING;
                            PurgeSio (GPS_COM, PURGE_TX_CLEAR | PURGE_RX_CLEAR);
                            ret = CALL_PROC(GPS_CMD_TYPE_CONNENT_DEVICE, &gps_packet);
                            if (ret != GPS_DL_SUCCESS) 
                            {
                                DOWNLOAD_LOGE(("[GPS_DL]send_to_connect_device fail\n"));
                                gps_packet.download_state = GPS_DL_FAILURE;
                            }
                        }
                        break;
                case GPS_DL_FINISH:
                        DOWNLOAD_LOGD(("[GPS_DL]gnss_download success\n"));
                        GPS_DLComClose(GPS_COM);
                        GPS_Uart_DmaClose();
                        ret = send_data_init(flag);
                        if(ret)
                            return GPS_DOWNLOAD_SUCCESS;
                        else
                            return GPS_DOWNLOAD_FAILURE;
                        break;
                case GPS_DL_FAILURE:
                        if(count < 3)
                        {
                            DOWNLOAD_LOGD(("[GPS_DL]gnss_download fail,try again,count = %d\n",count));
                            GPS_DLComClose(GPS_COM);
                            GPS_DLChipEn(SCI_FALSE);
                            SCI_Sleep(100);
                            count++;
                            gps_packet.download_state = GPS_DL_START;
                            gps_packet.gps_cmd_type = 0;
                            GPS_DLChipEn(SCI_TRUE);
                            ret = GPS_DLComInit(GPS_COM, 115200);
                            if (ret != 0) 
                            {
                                DOWNLOAD_LOGE(("[GPS_DL]open_uart_device fail\n"));
                                gps_packet.download_state = GPS_DL_FAILURE;
                            }
                            SCI_Sleep(100);
                            if(GPS_COM)
                                CHIPDRV_EnableIRQINT (TB_UART1_INT);
                            else
                                CHIPDRV_EnableIRQINT (TB_UART0_INT);
                            tick_begin = SCI_GetTickCount();
                        }
                        else
                        {
                            DOWNLOAD_LOGD(("[GPS_DL]gnss_download fail\n"));
			    SCI_PASSERT(0,("[GPS_DL]gnss_download fail\n"));
                            GPS_DLComClose(GPS_COM);
                            GPS_Uart_DmaClose();
                            send_data_init(flag);
                            return GPS_DOWNLOAD_FAILURE;
                        }
                        break;
                default:
                        break;
            }
            tick_end = SCI_GetTickCount();
            if((tick_end - tick_begin) > 10000)
            {

		if(timeout_count < 3)
                {
                            DOWNLOAD_LOGD(("[GPS_DL]gnss_download timeout fail,try again,count = %d\n",timeout_count));
                            GPS_DLComClose(GPS_COM);
                            GPS_DLChipEn(SCI_FALSE);
                            SCI_Sleep(100);
                            timeout_count++;
                            gps_packet.download_state = GPS_DL_START;
                            gps_packet.gps_cmd_type = 0;
                            GPS_DLChipEn(SCI_TRUE);
                            ret = GPS_DLComInit(GPS_COM, 115200);
                            if (ret != 0) 
                            {
                                DOWNLOAD_LOGE(("[GPS_DL]open_uart_device fail\n"));
                                gps_packet.download_state = GPS_DL_FAILURE;
                            }
                            SCI_Sleep(100);
                            if(GPS_COM)
                                CHIPDRV_EnableIRQINT (TB_UART1_INT);
                            else
                                CHIPDRV_EnableIRQINT (TB_UART0_INT);
                            tick_begin = SCI_GetTickCount();
                 }
                else
                 {
		            DOWNLOAD_LOGD(("[GPS_DL]gnss_download timeout fail\n"));
			    SCI_PASSERT(0,("[GPS_DL]gnss_download timeout fail\n"));
		            GPS_DLComClose(GPS_COM);
		            GPS_Uart_DmaClose();
		            send_data_init(flag);
		            return GPS_DOWNLOAD_FAILURE;
                  }	

            }
            SCI_Sleep(500);
    } while (SCI_TRUE);

}

/*****************************************************************************/
//  Description:    GPS_DLChipEn
//  Author:         
//  Note:          
/*****************************************************************************/
static void GPS_DLChipEn(BOOLEAN enable) 
{
    if(enable)
    {
        GPIO_SetValue( GPIO_GPS_CHIP_EN, enable);
        SCI_Sleep(100);
    }
    else
    {
        GPIO_SetValue( GPIO_GPS_CHIP_EN, enable);
        SCI_Sleep(100);
    }
}

/*****************************************************************************/
//  Description:    GPS_DLComInit
//  Author:         
//  Note:          
/*****************************************************************************/
static int GPS_DLComInit(uint32 port,uint32 BaudRate)
{
    int ret = 0; 
    
    UART_INIT_PARA_T    uart_st;
    
    DOWNLOAD_LOGD(("[GPS_DL]GPS_ComInit.BaudRate=%d\n",BaudRate));
    
    uart_st.tx_watermark  = GPS_DL_TX_WATER_MARK; // 0~127B
    uart_st.rx_watermark  = GPS_DL_RX_WATER_MARK;//GPS_DL_RX_WATER_MARK; // 0~127B
    switch(BaudRate)
    {
        case 38400:
        	uart_st.baud_rate     = BAUD_38400;
        break;
        case 57600:
        	uart_st.baud_rate     = BAUD_57600;
        break;	
        case 115200:
        	uart_st.baud_rate     = BAUD_115200;
        break;
        default:
        uart_st.baud_rate     = BAUD_115200;
        break;
    }
    uart_st.parity        = FALSE; 
    uart_st.parity_enable = PARITY_DISABLE;
    uart_st.byte_size     = EIGHT_BITS;
    uart_st.stop_bits     = ONE_STOP_BIT;
    uart_st.flow_control  = NO_FLOW_CONTROL;
    uart_st.ds_wakeup_en  = DS_WAKEUP_ENABLE;
    uart_st.ds_wakeup_type = DS_WAKEUP_BY_RXD;    
    ret = UART_Initilize(port, &uart_st, GPS_DL_UartCallBack);

    UART_Tx_Int_Enable(port, FALSE);
    if(port)
        CHIPDRV_DisableIRQINT (TB_UART1_INT);
    else
        CHIPDRV_DisableIRQINT (TB_UART0_INT);
    return ret;
}

/*****************************************************************************/
//  Description:    GPS_DLComClose
//  Author:         
//  Note:          
/*****************************************************************************/
static int GPS_DLComClose(int uart_port)
{
    UART_INIT_PARA_T    uart1_st;
    
    DOWNLOAD_LOGD(("[GPS_DL]GPS_ComClose.\n"));
    
    uart1_st.tx_watermark  = GPS_DL_TX_WATER_MARK; // 0~127B
    uart1_st.rx_watermark  = GPS_DL_RX_WATER_MARK; // 0~127B
    uart1_st.baud_rate     = BAUD_115200;
    //uart1_st.baud_rate     = BAUD_57600;
    uart1_st.parity        = FALSE; 
    uart1_st.parity_enable = PARITY_DISABLE;
    uart1_st.byte_size     = EIGHT_BITS;
    uart1_st.stop_bits     = ONE_STOP_BIT;
    uart1_st.flow_control  = NO_FLOW_CONTROL;
    uart1_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
        
    //assume u1rxd/u1txd have selected in pinmap    
    return UART_Close(uart_port);
}

/*****************************************************************************/
//  Description:    GPS_DLGetImage
//  Author:         
//  Note:          
/*****************************************************************************/
unsigned int GPS_DLGetImage(unsigned char **p_gps_image_ptr,unsigned char image_type) 
{
    unsigned int size = 0;

    if(GPS_DL_FDL == image_type)
    {
        *p_gps_image_ptr = (unsigned char *)gps_gnss_fdl;
    	size = sizeof(gps_gnss_fdl);
	return size;
    }
	
#ifdef GPS_MODE_GPS_BD
    *p_gps_image_ptr = (unsigned char *)gps_bd_midi;
    size = sizeof(gps_bd_midi);
#else
    *p_gps_image_ptr = (unsigned char *)gps_glonass_midi;
    size = sizeof(gps_glonass_midi);
#endif

    return size;
}

/*****************************************************************************/
//  Description:    GPS_DL_UartCallBack
//  Author:         
//  Note:          
/*****************************************************************************/
static void GPS_DL_UartCallBack(uint32 event)
{
    int size = 0;
    int ret = 0;
    int image_size = gps_packet.gps_image_info[gps_packet.mode].image_size;
    char *data_buffer = gps_packet.gps_image_info[gps_packet.mode].image_data;
    uint32 i = 0;
    uint32 tmp = 0;
    static int flag = 0;
    
    switch (event)
    {
        case EVENT_DATA_TO_READ:    
            {
                if((gps_packet.download_state == GPS_DL_FAILURE) || (gps_packet.download_state == GPS_DL_FINISH) || (gps_packet.download_state != GPS_DL_RUNNING))
                {
                    PurgeSio (gps_packet.uart_port, PURGE_TX_CLEAR | PURGE_RX_CLEAR);
                    flag = 0;
                    return;
                }

                ret = reivce_data_process(gps_packet.uart_port,gps_packet.gps_cmd_type);
                if(ret == GPS_DL_SUCCESS)
                {
                    PurgeSio (gps_packet.uart_port, PURGE_TX_CLEAR | PURGE_RX_CLEAR);
                    if(GPS_CMD_TYPE_SEND_EXEC_DATA != gps_packet.gps_cmd_type)
                    {
                        gps_packet.gps_cmd_type++;

                        if(GPS_CMD_TYPE_SEND_MIDST_DATA == gps_packet.gps_cmd_type)
                        {
                            if(!flag)
                            {
                                gps_packet.uart_raw_buffer = SCI_ALLOC(DL_MALLOC_BUFFSER_SIZE);
                                if(NULL == gps_packet.uart_raw_buffer)
                                {
                                    gps_packet.download_state = GPS_DL_FAILURE;
                                    flag = 0;
                                    return;
                                }
                                gps_packet.translated_size = 0;
                                gps_packet.data_size = DL_PACKET_SIZE;
                                flag = 1;
                            }
                            size = image_size -gps_packet.translated_size;
                            gps_packet.data_buffer = (char *)(data_buffer + gps_packet.translated_size);
                            DOWNLOAD_LOGD(("[GPS_DL]image info : address=0x%p,size=0x%x,translated_size=0x%x,data_buffer=0x%p\n", data_buffer, size,gps_packet.translated_size,gps_packet.data_buffer));
                            if(size > DL_PACKET_SIZE)
                            {
                                ret = CALL_PROC(gps_packet.gps_cmd_type, &gps_packet);
                                if(ret != GPS_DL_SUCCESS)
                                {
                                    SCI_FREE(gps_packet.uart_raw_buffer);
                                    gps_packet.download_state = GPS_DL_FAILURE;
                                    flag = 0;
                                    return;
                                }
                                gps_packet.translated_size += gps_packet.data_size;
                                gps_packet.gps_cmd_type--;
                            }
                            else
                            {
                                gps_packet.data_size = size;
                                ret = CALL_PROC(gps_packet.gps_cmd_type, &gps_packet);
                                SCI_FREE(gps_packet.uart_raw_buffer);
                                if(ret != GPS_DL_SUCCESS)
                                {
                                    gps_packet.download_state = GPS_DL_FAILURE;
                                    flag = 0;
                                    return;
                                }
                            }
                        }
                        else
                        {
                            ret = CALL_PROC(gps_packet.gps_cmd_type, &gps_packet);
                            if(ret != GPS_DL_SUCCESS)
                            {
                                gps_packet.download_state = GPS_DL_FAILURE;
                                flag = 0;
                                return;
                            }
                        }
                    }
                    else
                    {
                        gps_packet.download_state = GPS_DL_FINISH;
                        flag = 0;
                    }
                }
                else
                {
                    gps_packet.download_state = GPS_DL_FAILURE;
                    flag = 0;
                    return;
                }
                break;
         }
        
        case EVENT_WRITE_COMPLETE:  
        {
            break;    
        }
        
        case EVENT_INIT_COMPLETE:
            break;
        
        case EVENT_SHUTDOWN_COMPLETE:
            break;
        
        default:
            break;            
    }
}


LOCAL int gps_DLFdlCominit(int uart_port)
{
    int ret; 
    
    UART_INIT_PARA_T    uart_st;
    uart_st.tx_watermark  = GPS_DL_TX_WATER_MARK; // 0~127B
    uart_st.rx_watermark  = GPS_DL_RX_WATER_MARK; // 0~127B
    uart_st.baud_rate     = BAUD_115200;
    uart_st.parity        = FALSE; 
    uart_st.parity_enable = PARITY_DISABLE;
    uart_st.byte_size     = EIGHT_BITS;
    uart_st.stop_bits     = ONE_STOP_BIT;
    uart_st.flow_control  = HW_FLOW_CONTROL;
    uart_st.ds_wakeup_en  = DS_WAKEUP_ENABLE;
    uart_st.ds_wakeup_type = DS_WAKEUP_BY_RXD; 

    ret = UART_PHY_Initilize(uart_port, &uart_st);

    UART_Tx_Int_Enable(uart_port, FALSE);
    if(uart_port)
        CHIPDRV_DisableIRQINT (TB_UART1_INT);
    else
        CHIPDRV_DisableIRQINT (TB_UART0_INT);
    return ret;
}


/*------------------------------------------------------------
    Function:  GPS_DLGreenEye2FDL 

    Description:
 
    Parameters: none
--------------------------------------------------------------*/
#if 0//def GE2_ASSERT_DEBUG
#define GPS_WHDTESTBUFF_SIZE  (20*1024)
uint8 gps_whdtestbuff[GPS_WHDTESTBUFF_SIZE];
uint32 gps_whdpos = 0x0;
BOOLEAN gps_whdfull_flag = SCI_FALSE;
#define READ_WHD_RECORD_SIZE 256
typedef struct ge2_read_whd_record_debug_tag
{
    uint32 tick;
    uint32 length;
} ge2_read_whd_record_debug_t;

ge2_read_whd_record_debug_t ge2_read_whd_record_debug[READ_WHD_RECORD_SIZE] = {0};
uint32 read_whd_record_index = 0;

LOCAL void gps_read_whd_record_debug(uint32 length)
{
	ge2_read_whd_record_debug[read_whd_record_index].tick = SCI_GetTickCount();
	ge2_read_whd_record_debug[read_whd_record_index].length= length;
	read_whd_record_index++;
	if(read_whd_record_index >= READ_WHD_RECORD_SIZE)
	{
		read_whd_record_index = 0;
	}
}
#endif

void GPS_UartReset (int uart_port)
{
	if(!uart_port)
	{
	    /* Reset uart0 clock */
	    CHIP_REG_SET (APB_RST0_SET,  UART0_SOFT_RST_SET);
	    DELAY_CYCLE(1000)

	    CHIP_REG_SET (APB_RST0_CLR,  UART0_SOFT_RST_CLR);
	    DELAY_CYCLE(1000)
	}
	else
	{
	    /* Reset uart1 clock */
	    CHIP_REG_SET (APB_RST0_SET,  UART1_SOFT_RST_SET_MCU);
	    DELAY_CYCLE(1000)

	    CHIP_REG_SET (APB_RST0_CLR,  UART1_SOFT_RST_CLR_MCU);
	    DELAY_CYCLE(1000)
	}
}

int GPS_DLGreenEye2FDL()
{
	int ret = 0;
	int dataLen = 0;
	char readBuffer[1024];
	uint32	tick_old, tick_new;
	uint32 count = 0x0;

	pmanager.flag_dump = SCI_TRUE;

	tick_old = tick_new = SCI_GetTickCount();
	 // Rx fifo count
	while( (tick_new-tick_old) < 5000)
	{
		dataLen = UART_ReadData(GPS_COM,readBuffer,1023);
		if(dataLen > 0)
		{
			#if 0//def GE2_ASSERT_DEBUG
			gps_read_whd_record_debug(dataLen);
			if((gps_whdpos + dataLen) > GPS_WHDTESTBUFF_SIZE)
			{
				gps_whdfull_flag = SCI_TRUE;
				
				gps_whdpos = 0x0;
			}
			else
			{	
				  gps_whdfull_flag = SCI_FALSE;
			}

			SCI_MEMCPY(&(gps_whdtestbuff[0])+gps_whdpos,readBuffer,dataLen);
			gps_whdpos += dataLen;
			#endif
			tick_old = SCI_GetTickCount();
			count++;
			if(count >= 600)
				SCI_PASSERT(0,("GE2 Watchdog timeout,buffer not read finish!"));
		}
		tick_new = SCI_GetTickCount();
		SCI_Sleep(5);
	}

	GPS_UartReset(GPS_COM);
	
	GPS_DLComClose(GPS_COM);
	
	ret = GPS_DLGreenEye2(GPS_DL_FDL);
	if(GPS_DOWNLOAD_SUCCESS == ret)
	{
		gps_DLFdlCominit(GPS_COM);

		if(gps_packet.uart_port)
		*((volatile uint32 *)(ARM_UART1_BASE + ARM_UART_CLKD0))= (0xD & 0x0000ffff);
		else
		*((volatile uint32 *)(ARM_UART0_BASE + ARM_UART_CLKD0))= (0xD & 0x0000ffff);
		SCI_PASSERT(0,("GE2 Watchdog timeout!"));
		return GPS_DOWNLOAD_SUCCESS;
	}
	else
	{
		SCI_PASSERT(0,("GE2 fdl download fail!"));
		return GPS_DOWNLOAD_FAILURE;
	}
}

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/*  End Of File */

