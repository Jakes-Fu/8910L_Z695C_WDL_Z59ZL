/******************************************************************************
 ** File Name:   greeneye2_hardware.c                                                *
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
#include "greeneye2_common.h"
#include "greeneye2_engine.h"
#include "greeneye2_cmd.h"
#include "greeneye2_type.h"
#include "greeneye2_gnss_libgps_api.h"
#include "dal_time.h"
#include "sci_types.h"
#include "sci_api.h"
#include "com_drvapi.h"
#include "deepsleep_drvapi.h"
#include "sc6531efm_dma_cfg.h"

#ifdef GPS_AGPS_SUPPORT_QX
#include "greeneye2_agps_type.h"
#endif

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
static const char *GPS_EPHE = "$PCAGPS,gpsephe,";
static const char *GPS_TIME = "$PCAGPS,gpstime,";
static const char *GPS_LOC  = "$PCAGPS,gpslocn,";
static const char *A_END  = "$PCSUPL,aend\r\n";
static const char *S_END  = "$PCSUPL,send\r\n";
static const char *E_ASSIST  = "$PCSUPL,assist,";
static const char *E_SUPL_READY  = "$PCSUPL,suplready,";
static const char *MSA_POS  = "$PCSUPL,niposition,";


static char *close_cmd ="$PCGDC,SHUTDOWN,1,*1\r\n"; 

extern TGNSS_DataProcessor_t gGPSStream;

unsigned long send_length = 0;
unsigned long read_length = 0;
unsigned long read_length_assert = 0;

#ifdef GPS_AGPS_SUPPORT_QX
static TGe2Ephemeris_t EPHval;
static GE2_GPS_EPHEMERIS_PACKED EphPacked;
static WLLH_TYPE PosTime;
static char set_init_cmcc = 0;
#endif

typedef struct gps_uart_tag
{
    uint8 port;
    uint32 tx_dma_channel;
    uint32 rx_dma_channel;   //Not use at present
    volatile uint32  base_addr;
} ge2_uart_t;
#if 0//def GE2_ASSERT_DEBUG
typedef struct ge2_assert_write_debug_tag
{
    uint8 serial_buf[128];
    uint32 ticket;   //Not use at present
    uint8  heart;
} ge2_assert_write_debug_t;

typedef struct ge2_assert_read_debug_tag
{
    uint8 serial_buf[32];
    uint32 ticket;   //Not use at present
    uint8  heart;
	uint8  type;
	uint8  subType;
} ge2_assert_read_debug_t;

ge2_assert_read_debug_t  g_serial_read_data[256] = {0};
ge2_assert_write_debug_t  g_serial_write_data[128] = {0};
uint32 g_serial_read_index = 0;
uint32 g_serial_write_index = 0;
#endif
ge2_uart_t    s_gps_uart = {0};

#define GE2_UART_TX_DMA_CHANNEL   s_gps_uart.tx_dma_channel
#define GE2_UART_BASE_ADDR        s_gps_uart.base_addr
#define GE2_UART_COM              s_gps_uart.port

BOOLEAN dma_gpssend_flag = SCI_TRUE;

extern int gps_weekno_diff;
extern int gps_mscount_diff;

LOCAL void gps_txdma_callBack(uint32 num);
LOCAL void gps_uartdma_writedata(uint8 *start_addr, uint32 length);
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------
	Function:  gps_serialPortRead

	Description:
		it  read  the serial port by asynchronous mothod 
	Parameters:
	 pGPSState: point the gps device 
	Return: void
--------------------------------------------------------------------------*/
#ifdef GE2_ASSERT_DEBUG
#define GPS_LOGTESTBUFF_SIZE  (10*1024)
uint8 gps_logtestbuff[GPS_LOGTESTBUFF_SIZE];
uint32 gps_logpos = 0x0;
BOOLEAN gps_full_flag = SCI_FALSE;
#define READ_DATA_RECORD_SIZE 256
typedef struct ge2_read_data_record_debug_tag
{
    uint32 tick;
    uint32 length;
} ge2_read_data_record_debug_t;

ge2_read_data_record_debug_t ge2_read_data_record_debug[READ_DATA_RECORD_SIZE] = {0};
uint32 read_data_record_index = 0;

LOCAL void gps_read_data_record_debug(uint32 length)
{
	ge2_read_data_record_debug[read_data_record_index].tick = SCI_GetTickCount();
	ge2_read_data_record_debug[read_data_record_index].length= length;
	read_data_record_index++;
	if(read_data_record_index >= READ_DATA_RECORD_SIZE)
	{
		read_data_record_index = 0;
	}
}
#endif

int gps_serialPortRead(GpsState *pGPSState)
{
    int readLen = 0;
    int i = 0;
    int ret = 0;
    GpsState *s = _gps_state;
    SCI_TIME_T  tvold;
    SCI_TIME_T  tvnew;

    //GREENEYE2_LOGD(("[GE2]gps_serialPortRead read before "));
    readLen = UART_ReadData(s->uart_port,pGPSState->readBuffer,SERIAL_READ_BUFFER_MAX-1);
    TM_SendTestPointRequest(readLen,924144); 
#ifdef GE2_ASSERT_DEBUG
    gps_read_data_record_debug(readLen);
    if((gps_logpos + readLen) > GPS_LOGTESTBUFF_SIZE)
    {
    	gps_full_flag = SCI_TRUE;
		
	gps_logpos = 0x0;
    }
    else
    {	
    	  gps_full_flag = SCI_FALSE;
    }

    SCI_MEMCPY(&(gps_logtestbuff[0])+gps_logpos,pGPSState->readBuffer,readLen);
    gps_logpos += readLen;
#endif
	
    if (readLen > 0) 
    {
        read_length = read_length + readLen;   //send length
        if(read_length > GPS_RECV_MAX_LEN) 
        {
            read_length = 0;
        }

        //GREENEYE2_LOGE(("[GE2]GNSS_ParseOnePacket  readlen=%d,buffer=%s \r\n",readLen,pGPSState->readBuffer));
        TM_GetSysTime(&tvold);
        ret =  GNSS_ParseOnePacket(&gGPSStream, pGPSState->readBuffer,readLen);
#if 0//def GE2_ASSERT_DEBUG
		if(gGPSStream.state == GNSS_RECV_SEARCH_FLAG)
		{
			memcpy(g_serial_read_data[g_serial_read_index].serial_buf,gGPSStream.cmdData.buff,31); 
			g_serial_read_data[g_serial_read_index].type = gGPSStream.cmdData.type;
			g_serial_read_data[g_serial_read_index].subType= gGPSStream.cmdData.subType;
			g_serial_read_data[g_serial_read_index].ticket = SCI_GetTickCount();
			g_serial_read_data[g_serial_read_index].heart = s->HeartCount;
			g_serial_read_index++;
			if(g_serial_read_index == 256)
			{
				g_serial_read_index = 0;
			}

		}
#endif
        TM_GetSysTime(&tvnew);
        i = tvnew.sec - tvold.sec;
        if( i > 2)
        {
            GREENEYE2_LOGE(("[GE2]%s   GNSS_ParseOnePacket  consume too times :%d \r\n",__FUNCTION__,i));
        }
    }
    else
    {	
        //SCI_Sleep(20); 
    }
    return readLen;
    //GREENEYE2_LOGD(([GE2]"gps_serialPortRead read len =%d",readLen));
}
int parse_assert_subtype_data(uint8* input, uint8* output, uint32 src_len,TGNSS_DataProcessor_t* stream_data)
{
    int ret = 0;

    if (src_len > 0) 
    {
        read_length_assert = read_length_assert + src_len;   //send length
        if(read_length_assert > GPS_RECV_MAX_LEN) 
        {
            read_length_assert = 0;
        }
        ret =  GNSS_ParseOnePacket(stream_data, input,src_len);
    }

    return ret;
}

/*--------------------------------------------------------------------------
    Function:  gps_writeSerialData

    Description:
        it   send data to device by sync method 
    Parameters:
     s : point the gps device 
     len : the send data len 
    Return: it real send data len  
--------------------------------------------------------------------------*/
int gps_writeSerialData(GpsState* s,int len)
{
    int length = 0;
    int i = 0;

    if(DEVICE_SLEEP == s->powerState)
    {
        GREENEYE2_LOGE(("[GE2]the power is not wakeup"));
        return length;
    }
	
    SCI_GetMutex(s->dmamutex, SCI_WAIT_FOREVER);
	
#if 0//def GE2_ASSERT_DEBUG
	memcpy(g_serial_write_data[g_serial_write_index].serial_buf,s->writeBuffer,127); 
	g_serial_write_data[g_serial_write_index].ticket = SCI_GetTickCount();
	g_serial_write_data[g_serial_write_index].heart = s->HeartCount;
	g_serial_write_index++;
	if(g_serial_write_index == 128)
	{
		g_serial_write_index = 0;
	}
#endif
   send_length = send_length + len;
   if(send_length >  GPS_SEND_MAX_LEN)
   	send_length = 0;

	if((len < UART_HAL_FifoSize(s->uart_port)) && (dma_gpssend_flag))
    {
    	length = UART_WriteDataEx(s->uart_port, s->writeBuffer,len,SCI_FALSE);
    }
    else
    {
    	gps_uartdma_writedata(s->writeBuffer,len);
		length = len;
    }
	
	SCI_PutMutex(s->dmamutex);

    return length;
}


/******************************************************************************
**  Description:    gps_txdma_callBack
**  Author:         
**  parameter:      
******************************************************************************/
typedef struct ge2_dma_record_debug_tag
{
    uint32 ticket;
    uint32 step;   //Not use at present
} ge2_dma_record_debug_t;

#ifdef GE2_ASSERT_DEBUG
ge2_dma_record_debug_t ge2_dma_record_debug[256] = {0};
uint32 dma_record_index = 0;
#endif
LOCAL void gps_dma_record_debug(uint32 step)
{
#ifdef GE2_ASSERT_DEBUG
	ge2_dma_record_debug[dma_record_index].ticket = SCI_GetTickCount();
	ge2_dma_record_debug[dma_record_index].step= step;
	dma_record_index++;
	if(dma_record_index >= 256)
	{
		dma_record_index = 0;
	}
#endif
}

LOCAL void gps_txdma_callBack(uint32 num)
{
	dma_gpssend_flag = SCI_TRUE;
    GREENEYE2_LOGD(("[GE2]gps_txdma_callBack\n"));
	//bm_disable();
	gps_dma_record_debug(6);

    DMA_HAL_ChnEnable(GE2_UART_TX_DMA_CHANNEL, SCI_FALSE);        // s_os_dma_int++;
	gps_dma_record_debug(7);

    //DPSLP_UART_EnableApbSleep (GE2_UART_COM, ENABLE_APB_SLEEP);
	TM_SendTestPointRequest(GE2_UART_TX_DMA_CHANNEL,927924);
	gps_dma_record_debug(8);
}

/******************************************************************************
**  Description:    GPS_UartDma_WriteData
**  Author:         
**  parameter:      
******************************************************************************/
LOCAL void gps_uartdma_writedata(uint8 *start_addr, uint32 length)
{
	while(1)
	{
		if(dma_gpssend_flag)
		{
			gps_dma_record_debug(1);

			DPSLP_UART_EnableApbSleep (GE2_UART_COM, DISABLE_APB_SLEEP);  //AHB Should not sleep when use DMA

			dma_gpssend_flag = SCI_FALSE;
			gps_dma_record_debug(2);

			//Enable Uart Dma
			UART_DmaModeEnable(GE2_UART_COM, TRUE);
			gps_dma_record_debug(3);

			//DMA_HAL_ChnEnable(GPS_UART_TX_DMA_CHANNEL, SCI_FALSE);
			DMA_HAL_ChnIntConfig(GE2_UART_TX_DMA_CHANNEL, DMA_CHN_INT_TYPE_TRANSDONE, gps_txdma_callBack);
			gps_dma_record_debug(4);
			UART_DmaWriteData(GE2_UART_COM, start_addr, length, GE2_UART_TX_DMA_CHANNEL);
			gps_dma_record_debug(5);
			break;
		}
		else
		{
			TM_SendTestPointRequest(length,926747);
			SCI_SLEEP(20);
		}
	}
}

/******************************************************************************
**  Description:    GPS_Uart_DmaStart
**  Author:         
**  parameter:      
******************************************************************************/
void gps_uart_dmastart(int uart_port)
{
    uint32 ret = 0xFFFFFFFF;
    s_gps_uart.port = uart_port;

    ret = DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_STANDARD);  
    UART_DMA_CONNECT(uart_port, UART_DMA_TX_MODE, ret);
    s_gps_uart.tx_dma_channel = ret;

   GE2GPS_LOGD("tx_dma_channel=%d",s_gps_uart.tx_dma_channel);
}

/******************************************************************************
**  Description:    gps_uart_dmaclose
**  Author:         
**  parameter:      
******************************************************************************/
void gps_uart_dmaclose()
{
    DMA_HAL_ChnEnable (GE2_UART_TX_DMA_CHANNEL,SCI_FALSE);
    UART_DMA_CONNECT(GE2_UART_COM, UART_DMA_TX_MODE, 0);	
    DMA_HAL_FreeChn(GE2_UART_TX_DMA_CHANNEL);  
    DPSLP_UART_EnableApbSleep (GE2_UART_COM, ENABLE_APB_SLEEP);
	GE2GPS_LOGD("tx_dma_channel=%d,close",s_gps_uart.tx_dma_channel);
}


/*--------------------------------------------------------------------------
    Function:  gps_sendCommnd

    Description:
        it  encode and send  start-module   request  
    Parameters:
     flag: start module  request flag 
    Return: the write data length  
--------------------------------------------------------------------------*/
static int gps_sendCommnd(int flag)
{
	GpsState*	s = _gps_state;
	TCmdData_t packet;
	int len,writeLen ;

	len = writeLen = 0;
	//E("send command enter,subtype is %d",flag);
	packet.type  = GNSS_LIBGPS_SET_PARA_TYPE;

	switch(flag)
	{
		case COMMAND:
		{
			packet.subType = GNSS_LIBGPS_SET_WORKMODE_SUBTYPE;
			packet.length = 4;
			memcpy(packet.buff,&(s->cmd_flag),sizeof(int));
			break;
		}
		case SET_CPMODE:
		{
			packet.subType = GNSS_LIBGPS_SET_GNSS_SUBTYPE;
			packet.length = 4;
                        GREENEYE2_LOGD(("[GE2]cpmode is:%d:",s->cpmode));
			memcpy(packet.buff,&(s->cpmode),sizeof(int));
			break;
		}
		case SET_OUTTYPE:
		{
			packet.subType = GNSS_LIBGPS_SET_OUTPUT_PROTOCOL_SUBTYPE;
			packet.length = 4;
			memcpy(packet.buff,&(s->outtype),sizeof(int));
			break;
		}
		case CLEAR_CONFIG:
		{
			packet.subType = GNSS_LIBGPS_CLEAR_FLASH_CONFIG_SUBTYPE;
			packet.length = 0;
			break;
		}
		case SET_INTERVAL:
		{	// CTS_TEST
			packet.subType = GNSS_LIBGPS_SET_REPORT_INTERVAL;
			packet.length = 4;
			memcpy(packet.buff,&(s->min_interval),sizeof(int));
			break;
		}
		case SET_LTE_ENABLE:
			packet.subType = GNSS_LIBGPS_SET_LTE_SWITCH;
			packet.length = 4;
			memcpy(packet.buff,&(s->lte_open),sizeof(int));
		    break;
		case SET_NET_MODE:
		    break;		
		case SET_CMCC_MODE:
		    break;
		case SET_REALEPH:
			
		    break;	
		case SET_WIFI_STATUS:
		    break;		
		case SET_ASSERT_STATUS:
			packet.subType = GNSS_LIBGPS_SET_ASSERT_STATUS;
			packet.length = 0;
		    break;
                case SET_BASEBAND_MODE:
			packet.subType = GNSS_LIBGPS_SETMODE_SUBTYPE;
			packet.length = 4;
			memcpy(packet.buff,&(s->baseband_mode),sizeof(int));
                    break;
                case SET_PHONE_PLATFORM:
			packet.subType = GNSS_LIBGPS_SETPLATFORM_SUBTYPE;
			packet.length = 4;
			GREENEYE2_LOGD(("[GE2]phoneplatform is:%d:",s->phoneplatform));
			memcpy(packet.buff,&(s->phoneplatform),sizeof(int));
                    break;
		default:
			break;
	}
    
	len = GNSS_EncodeOnePacket(&packet, s->writeBuffer, SERIAL_WRITE_BUFFER_MAX);

	if(len > 0)
	{
		writeLen = gps_writeSerialData(s,len);   //gps_writeData2Buffer
	}
	else
	{
		//E("gps_sendCommnd->GNSS_EncodeOnePacket error \r\n ");
	}
	return writeLen;
}

/*--------------------------------------------------------------------------
    Function:  gps_powerRequest

    Description:
        it  encode and send  power   request  
    Parameters:
     flag: power request flag 
    Return: the write data length  
--------------------------------------------------------------------------*/
static int gps_powerRequest(int flag)
{
	GpsState*	s = _gps_state;
	TCmdData_t packet;
	int len ,writeLen;
    gpstime Gtime;    
    
    len = writeLen = 0;

	//first check it can send data ?  todo 	
	packet.type = GNSS_LIBGPS_POWER_REQUEST_TYPE;
	packet.length = 0;
	
	switch(flag) //it should add POWERON to do 
	{
		case IDLEON_STAT:
		{
			packet.subType = GNSS_LIBGPS_POWER_IDLEON_REQ_SUBTYPE;
			break;
		}
		case IDLEOFF_STAT:
		{
			if(s->first_idleoff == 0)
		    {
		        s->first_idleoff = 1;
		        packet.length = sizeof(gpstime);
		        OsTimeSystemGet(&Gtime);
			if(Gtime.weekno > 0)
			{
				Gtime.weekno += gps_weekno_diff;
				Gtime.mscount += gps_mscount_diff;
			}
		        GREENEYE2_LOGD(("[GE2]first idleoff,time:weekno=%d,mscount=%d,weekno_diff=%d,mscount_diff=%d",Gtime.weekno,Gtime.mscount,gps_weekno_diff,gps_mscount_diff));
		        memcpy(packet.buff,&Gtime,packet.length);
		    }
			packet.subType = GNSS_LIBGPS_POWER_IDLEOFF_REQ_SUBTYPE;
			break;
		}
		case SHUTDOWN:
		{
			packet.subType = GNSS_LIBGPS_POWER_SHUTDOWN_REQ_SUBTYPE;
			break;
		}
		case POWERON:
		{
			packet.subType = GNSS_LIBGPS_POWER_ON_REQ_SUBTYPE;
			break;
		}
		case CHIPRESET:
		{
			packet.subType = GNSS_LIBGPS_POWER_RESET_REQ_SUBTYPE;
			break;
		}
		default:
			break;
	}

	len = GNSS_EncodeOnePacket( &packet, s->writeBuffer, SERIAL_WRITE_BUFFER_MAX);
	if(len > 0)
	{
		writeLen = gps_writeSerialData(s,len);
	}
	else
	{
		GREENEYE2_LOGE(("[GE2]gps_powerRequest->GNSS_EncodeOnePacket error \r\n "));
	}
	GREENEYE2_LOGD(("[GE2]gps_powerRequestsssss-> send power request =%d  writelen=%d",packet.subType,writeLen));

	return writeLen;
	
}


/*--------------------------------------------------------------------------
    Function:  gps_adingData

    Description:
        it  encode and send  ading  data 
        
    Parameters:
     pPak: point the  ading data information  
    Return: the write data length  
--------------------------------------------------------------------------*/
int gps_adingData(TCmdData_t*pPak)
{
    GpsState *s = _gps_state;
    int writeLen = 0, len = 0;
    TCmdData_t *packet = pPak;
    int ret =0;

    if(DEVICE_SLEEP == s->powerState)
    {
        GREENEYE2_LOGE(("[GE2]GE2 has sleep so drops data \r\n "));
        return ret; 
    }
    //ret= gps_wakeupDevice(s);
    if(ret)
    {
        if(!s->release) //userdebug
        {
            gps_setPowerState(DEVICE_ABNORMAL);
            createDump(s);
            return writeLen;
        }
        else
        {
            GREENEYE2_LOGD(("[GE2]%s,gps_wakeupDevice  failed ",__FUNCTION__));
            gps_setPowerState(DEVICE_RESET);  
            return writeLen;
        }
    }
    
    //pthread_mutex_lock(&s->writeMutex);
    memset(s->writeBuffer,0,SERIAL_WRITE_BUFFER_MAX);
    len = GNSS_EncodeOnePacket(packet, s->writeBuffer, SERIAL_WRITE_BUFFER_MAX);
    if(len > 0)
    {
	    writeLen = gps_writeSerialData(s,len);
    }
    else
    {
	    GREENEYE2_LOGE(("[GE2]gps_powerRequest->GNSS_EncodeOnePacket error \r\n "));
    }
    //pthread_mutex_unlock(&s->writeMutex);
    //gps_restorestate(s);
    return writeLen;
}

#ifdef GPS_AGPS_SUPPORT_QX
static int adjustAdingData(int type,const char *src, TCmdData_t *dest)
{
	GpsState *s = _gps_state;
    TCgAgpsEphemerisCollection_t *ephe;
	TCgRxNTime *time;
    TGe2GpsPosition *loc;
	unsigned int agpsmode = 1;
	int lenth = 0,i;

    if((src == NULL) || (dest == NULL))
    {
		GREENEYE2_LOGE(("[GE2]src or dest null,return"));
        return -1;
    }
    switch(type)
    {
		case EPHEMERIS:

				GREENEYE2_LOGE(("[GE2]SEND GE2 EPHEMERIS"));
				ephe = (TCgAgpsEphemerisCollection_t *)src;
				GREENEYE2_LOGE(("[GE2]ephe transform begin,svid count is %d",ephe->count));
				if(ephe->count > 13)
				{
					GREENEYE2_LOGE(("[GE2]ephe count is too large,maybe some wrong"));
					ephe->count = 13;
				}
				memcpy(dest->buff,&agpsmode,sizeof(int));
				for(i = 0; i < ephe->count; i++)
                		{
					memset(&EPHval,0,sizeof(TGe2Ephemeris_t));
					memset(&EphPacked,0,sizeof(GE2_GPS_EPHEMERIS_PACKED));
					Trans_Ephpacked(&(ephe->satellite[i]),&EPHval);
					EncodeSvData(&EPHval, &EphPacked);
					EphPacked.flag = 1;
					EphPacked.svid = ephe->satellite[i].SatID;		
					//E("svid i:word[2]=%d,word[10]=%d,word[11]=%d",EphPacked.svid,EphPacked.word[2],EphPacked.word[10],EphPacked.word[11]);	
	
	
					memcpy(dest->buff+i*sizeof(GE2_GPS_EPHEMERIS_PACKED)+4,&EphPacked,sizeof(GE2_GPS_EPHEMERIS_PACKED));
				}
				dest->length = sizeof(GE2_GPS_EPHEMERIS_PACKED)*ephe->count+4;
            	dest->type = GNSS_LIBGPS_AIDING_DATA_TYPE;
            	dest->subType = GNSS_LIBGPS_AIDING_GPSEPHEMERIS_SUBTYPE;
				GREENEYE2_LOGD(("[GE2]send gps data,lenth is %d",dest->length));
				//adjust data for ge2;
			
        	break;
		case TIME:
			if(s->hardware_id == CELLGUIDE)
            {
				dest->length = sizeof(TCgRxNTime);
				dest->type = GNSS_LIBGPS_AIDING_DATA_TYPE;
				dest->subType = GNSS_LIBGPS_AIDING_REFTIME_SUBTYPE;
				memcpy(dest->buff,src, dest->length);
			}
            else
            {
				GREENEYE2_LOGE(("[GE2]SEND GE2 TIME"));
				time = (TCgRxNTime*)src;
				PosTime.weekNo = time->weekNo;
				PosTime.second = time->second;
				PosTime.secondFrac = time->secondFrac;
				if((time->weekNo > 0) && (s->cmcc == 1) && (set_init_cmcc == 0))
				{
				    GREENEYE2_LOGD(("[GE2]now cmcc mode will set to ge2"));
				    set_init_cmcc = 1;
				    gps_sendData(SET_CMCC_MODE, NULL); 
				}
				//E("SEND TIME OVER,weekno is %d",PosTime.weekNo);
				return 1;
				//adjust data for ge2;
			}
        	break;
		case LOCATION:
			if(s->hardware_id == CELLGUIDE)
            {
				dest->length = sizeof(TGe2GpsPosition);
            	dest->type = GNSS_LIBGPS_AIDING_DATA_TYPE;
            	dest->subType = GNSS_LIBGPS_AIDING_REFPOSITION_SUBTYPE;
				memcpy(dest->buff,src, dest->length);
			}
            else
            {
				GREENEYE2_LOGD(("[GE2]SEND GE2:time:week=%d,second=%d,secondfrac=%d"
                  ,PosTime.weekNo,PosTime.second,PosTime.secondFrac));
				loc = (TGe2GpsPosition *)src;
				dest->length = sizeof(WLLH_TYPE);
            	dest->type = GNSS_LIBGPS_AIDING_DATA_TYPE;
            	dest->subType = GNSS_LIBGPS_AIDING_POSVELTIME_SUBTYPE;
			    PosTime.lat = loc->latitude;
			    PosTime.lon = loc->longitude;
			    PosTime.hae = loc->altitude;
			    GREENEYE2_LOGD(("[GE2]SEND GE2:location:lat=%f,lon=%f",PosTime.lat,PosTime.lon));
				memcpy(dest->buff,&PosTime,sizeof(WLLH_TYPE));
				//adjust data for ge2;
			}
        	break;
		default:
        	break;
    }
    return 0;
}
#else
static int adjustAdingData(int type,const char *src, TCmdData_t *dest)
{
}
#endif

/*--------------------------------------------------------------------------
    Function:  gps_SendRequest

    Description:
         
    Parameters:
     flag: 
    Return:  
--------------------------------------------------------------------------*/
int gps_SendRequest(GpsState *s,int flag)
{
	TCmdData_t packet;
	int len = 0, writeLen = 0;

	packet.type = GNSS_LIBGPS_REQUEST_TYPE;

	switch(flag)
	{
		case REQUEST_VERSION:
			packet.subType = GNSS_LIBGPS_REQUEST_VERSION_SUBTYPE;
			packet.length = 0;
			break;
		case REQUEST_CHIP:
			packet.subType = GNSS_LIBGPS_REQUEST_CHIP_SUBTYPE;
			packet.length = 0;
			break;
		case REQUEST_GPSTIME:
			packet.subType = GNSS_LIBGPS_REQUEST_GPSTIME_SUBTYPE;
			packet.length = 0;
			break;
		case REQUEST_CONSTELLATION:
			break;
		case REQUEST_TSXTEMP:
			break;
		default:
			break;
	}

	len = GNSS_EncodeOnePacket( &packet, s->writeBuffer, SERIAL_WRITE_BUFFER_MAX);
	if(len > 0)
	{
		writeLen = gps_writeSerialData(s,len); 
	}
	else
	{
		//E("gps_SendRequest->GNSS_EncodeOnePacket error \r\n ");
	}

	return writeLen;
}

/*--------------------------------------------------------------------------
    Function:  gps_SendNotify

    Description:
         
    Parameters:
     flag: 
    Return:  
--------------------------------------------------------------------------*/
int gps_SendNotify(GpsState *s,int flag)
{
    TCmdData_t packet;
    int len = 0;
    int writeLen = 0;

    //first check it can send data ?  todo 	
    packet.type = GNSS_LIBGPS_NOTIFIED_TYPE;
    packet.subType = GNSS_NOTIFIED_DEVICE_STATE_SUBTYPE;
    packet.length = 2;
    if(SLEEP_NOTIFY == flag)
    {
        if(DEVICE_SLEEP == s->powerState)
        {
            GREENEYE2_LOGD(("[GE2]the state is sleep ,so don't send sleep again"));
            return writeLen;
        }
        else
        {
            packet.buff[0] = GNSS_DEVICE_SLEEP_STATE;
            packet.buff[1] = s->sleepCount++;
        }
    }
    else if(WAKEUP_NOTIFY == flag)
    {   
        packet.buff[0] = GNSS_DEVICE_READY_STATE;
        packet.buff[1] = s->wakeupCount++;
    }
    else
    {
        GREENEYE2_LOGE(("[GE2]it not supply notify now"));
    }
    GREENEYE2_LOGD(("[GE2]request type %d,subtype %d",packet.type,packet.subType));
    len = GNSS_EncodeOnePacket( &packet, s->writeBuffer, SERIAL_WRITE_BUFFER_MAX);
    if(len > 0)
    {
        writeLen = gps_writeSerialData(s,len);     //gps_writeData2Buffer
    }
    else
    {
        GREENEYE2_LOGE(("[GE2]gps_SendNotify->GNSS_EncodeOnePacket error \r\n "));
    }
    return writeLen;
}

#if 1//def TEST_MODE
static int gps_eut_config(int flag)
{
	GpsState*	s = _gps_state;
	TCmdData_t packet;
	int len,writeLen ;
        int rf_testmode = 0;

	len = writeLen = 0;
	GREENEYE2_LOGD(("[GE2]config eut,subtype is %d",flag));
	packet.type  = GNSS_LIBGPS_EUT_TYPE;

	switch(flag)
	{
		case EUT_TEST_CW:
		{
			packet.subType = GNSS_LIBGPS_EUT_CW_SUBTYPE;
			packet.length = 0;
			break;
		}
		case EUT_TEST_TSX:
		{
			packet.subType = GNSS_LIBGPS_EUT_TSXTEMP_SUBTYPE;
			packet.length = 0;
			break;
		}
		case EUT_TEST_TCXO:
		{
			packet.subType = GNSS_LIBGPS_EUT_TCXO_SUBTYPE;
			packet.length = 0;
			break;
		}
		case EUT_TEST_MIDBAND:
		{
			packet.subType = GNSS_LIBGPS_EUT_MIDBAND_SUBTYPE;
			packet.length = 0;
			break;
		}
		case RF_DATA_CAPTURE:
			packet.subType = GNSS_LIBGPS_EUT_RFDATA_CAP_SUBTYPE;
			packet.length = 4;
                        rf_testmode = gps_get_rftestmode(); //gps:0x0  glonass:0x1  bds:0x2
                        memcpy(packet.buff,&(rf_testmode),sizeof(int));
			break;
                case EUT_TEST_TCXO_TSX:
                    {
                        packet.subType = GNSS_LIBGPS_EUT_TCXO_SUBTYPE;
                        packet.length = 1;
                        packet.buff[0] = 1; //set   TCXO_TSX
                        GREENEYE2_LOGD(("[GE2]gps_eut_config-> EUT_TEST_TCXO_TSX  \r\n "));
                        break;
                    }
		default:
			break;
	}
	
	len = GNSS_EncodeOnePacket(&packet, s->writeBuffer, SERIAL_WRITE_BUFFER_MAX);

	if(len > 0)
	{
		writeLen = gps_writeSerialData(s,len);   //gps_writeData2Buffer
	}
	else
	{
		GREENEYE2_LOGE(("[GE2]gps_eut_config->GNSS_EncodeOnePacket error \r\n "));
	}
	
	return writeLen;
}
#endif

#ifdef GNSS_REG_WR
static int gnss_register_wr(int flag)
{
	GpsState*	s = _gps_state;
	TCmdData_t packet;
	int len,writeLen ;

	len = writeLen = 0;
	GREENEYE2_LOGD(("[GE2]gnss_register_wr,subtype is %d",flag));
	packet.type  = GNSS_LIBGPS_REG_WR_TYPE;

	switch(flag)
	{
		case WRITE_REG:
		{
			packet.subType = GNSS_LIBGPS_REG_WRITE_SUBTYPE;
			packet.length = 8;
			memcpy(packet.buff,&(s->wRegAddr),sizeof(unsigned int));
			memcpy(packet.buff + sizeof(unsigned int), &(s->wRegValue), sizeof(unsigned int));
			break;
		}
		case READ_REG:
		{
			packet.subType = GNSS_LIBGPS_REG_READ_SUBTYPE;
			packet.length = 4;
			memcpy(packet.buff, &(s->rRegAddr), sizeof(unsigned int));
			break;
		}
		default:
			break;
	}
	
	len = GNSS_EncodeOnePacket(&packet, s->writeBuffer, SERIAL_WRITE_BUFFER_MAX);

	if(len > 0)
	{
		writeLen = gps_writeSerialData(s,len);   //gps_writeData2Buffer
	}
	else
	{
		GREENEYE2_LOGE(("[GE2]gnss_register_wr->GNSS_EncodeOnePacket error \r\n "));
	}
	
	return writeLen;
}
#endif

/*--------------------------------------------------------------------------
    Function:  gps_wakeupDevice

    Description:
        it  wakeup device before  send data to ge2 device   
    Parameters: gps device  
    Return: 
		 1 : it  wake up fail  0: wake up success 

--------------------------------------------------------------------------*/
int gps_wakeupDevice(GpsState *s )
{
    int wakeup_timeout=0;
    int wakeuping =1;
    int ret = 0;
    int i = 0;

    GREENEYE2_LOGD(("[GE2] wakeupdevice %d",SCI_GetTickCount()));

    //first wakeup GE2 device 
    if((s->sleepFlag)&&(DEVICE_SLEEP == s->powerState))
    {
    	DPSLP_UART_EnableApbSleep (GE2_UART_COM, DISABLE_APB_SLEEP);
        gps_setPowerState(DEVICE_WAKEUPING);
        while(wakeuping)
        {
            for(i = 0;i < 3; i++)
            {
                ret = gps_SendNotify(s,WAKEUP_NOTIFY);
                if(ret < 0)
                {
                    break;
                }
            }
            wakeuping = gps_power_Statusync(POWER_WAKEUP);
            if(wakeuping)
            {
                wakeup_timeout++;
            }
            if(wakeup_timeout == 30)
            {
                break;
            }
        }
        //it can't wakeup device,so must download firmware 
        if(wakeuping)
        {
            DPSLP_UART_EnableApbSleep (GE2_UART_COM, ENABLE_APB_SLEEP);
            ret = 1;
        }
        else
        {   
            s->powerErrorNum = 0;
            ret = 0;
        }
    }
    else
    {
        ret = 0;
    }

    return ret;
}

/*--------------------------------------------------------------------------
    Function:  gps_entersleep

    Description:
    	it  sleep device before  send data to ge2 device
    Parameters: gps device  
    Return: 

--------------------------------------------------------------------------*/
void gps_entersleep()
{
    GpsState*  s = _gps_state;
    int sleepCount = 3;
    int ret = 0;
    int i = 0;

    GE2GPS_LOGD("gps will enter sleep");

    if(DEVICE_IDLED != s->powerState)
    {
        GREENEYE2_LOGE(("[GE2]send sleep command  the powerState should DEVICE_IDLED ,but  powerState: %s",gps_PowerStateName(s->powerState)));
        return;
    }

    if((gps_get_testmode()) && (s->gps_flag != CMD_INIT))
    {
        //GREENEYE2_LOGE(("[GE2]gps test mode is enable,don`t allow  to sleep"));
        //return;
    }
    gps_setPowerState(DEVICE_SLEEPING);
    while(sleepCount)
    {
        ret = gps_SendNotify(s,SLEEP_NOTIFY);
        if(ret > 0)
        {
            SCI_Sleep(10);
            ret = gps_power_Statusync(POWER_SLEEPING);
            if(ret)
            {
                sleepCount--;
            }
            else
            {
                sleepCount = 0;
                break;
            } 
        }
    }

    if(ret)
    {
        GREENEYE2_LOGE(("[GE2]it's sleep device fail !!!!"));
	//	SCI_PASSERT(0,("it sleep fail"));
        if(s->release)
        {
            gps_setPowerState(DEVICE_RESET);
        }
        else
        {
            //gps_setPowerState(DEVICE_ABNORMAL);
        }
    }
    else
    {
    	//GPIO_SetGPS26MPower(SCI_FALSE);
    	DPSLP_UART_EnableApbSleep (GE2_UART_COM, ENABLE_APB_SLEEP);
    }
}

/*--------------------------------------------------------------------------
    Function:  gps_send_sleep

    Description:
    Parameters: gps device  
    Return: 

--------------------------------------------------------------------------*/
void gps_send_sleep(uint32 state)
{
    GpsState*  s = _gps_state;
    GPS_ENTER_SLEEP_T *signal = PNULL;

    GE2GPS_LOGD("send sleep request!");

    SCI_DeactiveTimer(s->ge2sleeptimer);
    SCI_CREATE_SIGNAL(
				signal,
				GPS_ENTER_SLEEP_REQ,
				sizeof (GPS_ENTER_SLEEP_T),
			SCI_IDENTIFY_THREAD());
			SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, GPS_GetInterfaceThreadId()); 
	
}

/*****************************************************************************/
//  Description:    gps_notiySleepDevice
//  Author:         
//  Note:   
/*****************************************************************************/
void  gps_notiySleepDevice(GpsState *s )
{
#if 0
    gps_entersleep();
#else
    if(SCI_IsTimerActive(s->ge2sleeptimer))
    {
    	GE2GPS_LOGD("GPS sleep timer is active!");
    	return;
    }
    else
    {
    	GE2GPS_LOGD("GPS active sleep timer !");
    	SCI_ChangeTimer(s->ge2sleeptimer, gps_send_sleep, GE2_TIMER_SLEEP_INTERVAL);
	SCI_ActiveTimer(s->ge2sleeptimer);
    }
#endif
}

/*--------------------------------------------------------------------------
    Function:  gps_sendData

    Description:
        it  send data in sync communication ,it should try 3 times if it send failed ,
        or it may be hardware error 
        
    Parameters:
     flag: it message type  
     pBuf: point the data buffer 
    Return: void 
--------------------------------------------------------------------------*/
void  gps_sendData(int flag,const char *pBuf )
{
	GpsState *s = _gps_state;
	int len = 0;
	TCmdData_t Pak;
    int ret = 0;
 
	if((s->gps_flag != CMD_START) && ((flag == EPHEMERIS) || (flag == TIME) || (flag == LOCATION)))
	{
		GREENEYE2_LOGE(("[GE2]agps data will not send,for gps is close"));
		return;
	}

    if(SLEEP_NOTIFY != flag)
    {
        ret = gps_wakeupDevice(s);	
        if(ret)
        {
            if(!s->release) //userdebug
            {
                gps_setPowerState(DEVICE_ABNORMAL);
                createDump(s);
                return ; 
            }
            else
            {
                GREENEYE2_LOGD(("[GE2]%s,gps_wakeupDevice  failed ",__FUNCTION__));
                gps_setPowerState(DEVICE_RESET);
            }
            return;
        }
    }
    
	GREENEYE2_LOGD(("[GE2]gps_sendData,flag is 0x%x",flag));
 
	switch(flag)
	{
		case COMMAND:
		case SET_CPMODE:
		case SET_OUTTYPE:
		case CLEAR_CONFIG:
		case SET_INTERVAL:		// CTS_TEST
		case SET_CMCC_MODE:
		case SET_LTE_ENABLE:
		case SET_NET_MODE:
		case SET_REALEPH:
		case SET_WIFI_STATUS:
		case SET_ASSERT_STATUS:
                case SET_BASEBAND_MODE:
                case SET_PHONE_PLATFORM:
		{
			len = gps_sendCommnd(flag);
			break;
		}
		case IDLEON_STAT:
		case IDLEOFF_STAT:
		case SHUTDOWN:
		case POWERON:
		case CHIPRESET:
		{
			len = gps_powerRequest(flag);
			break;	
		}
		case EPHEMERIS:
		case TIME:
		case LOCATION:
		{
			ret = 0;
	            if(EPHEMERIS == flag)
	            {
	                SCI_Sleep(100);
	            }
			ret = adjustAdingData(flag,pBuf,&Pak);
			if(!ret)
			{
				GE2GPS_LOGD("send %d\n", flag);
				len = gps_adingData(&Pak);  
                if(EPHEMERIS == flag)
                {
                    SCI_Sleep(200);
                }
			} 
			break;
		}
		//it should rewrite the agps coontrol plan 
		case ASSIST_END:
		case SUPL_END:
		case ASSIST : 
		case MSA_POSITION:	
		case POSITION:
			break;
		case SUPL_READY:
			break;
		case REQUEST_VERSION:
		case REQUEST_CHIP:
		case REQUEST_GPSTIME:
		case REQUEST_CONSTELLATION:
		case REQUEST_TSXTEMP:
			len = gps_SendRequest(s,flag);
			break; 
		case SLEEP_NOTIFY:
			len = gps_SendNotify(s,flag);
			break;
		case EUT_TEST_CW:
		case EUT_TEST_TSX:
		case EUT_TEST_TCXO:
		case EUT_TEST_MIDBAND:
		case RF_DATA_CAPTURE:
                case EUT_TEST_TCXO_TSX:
    			len = gps_eut_config(flag);
			break;
		#ifdef GNSS_REG_WR
		case WRITE_REG:
		case READ_REG:
    			len = gnss_register_wr(flag);
			break;
		#endif
		default:
			GREENEYE2_LOGE(("[GE2]gps_sendData flag = %d error \r\n",flag));
			break;
	}
	
     GREENEYE2_LOGD(("[GE2]gps_sendData,send datalen  %d",len));
}

/*--------------------------------------------------------------------------
    Function:  gps_setPowerState

    Description:
        it update the device power state 
        
    Parameters:
     state: it should change the device state 
     
    Return: 1 success others failed  
--------------------------------------------------------------------------*/
void gps_setPowerState(int state)
{
	GpsState*	s = _gps_state;
    int ret = 0;

	//to do check the power state is right ?
	GREENEYE2_LOGD(("[GE2]GNSS STATE :%s, set power: %s",gps_PowerStateName(s->powerState),gps_PowerStateName(state)));
    
	switch(state)
    {
        case DEVICE_DOWNLOADING: //inital or abornal
        {
            s->powerState = state;
            break;
        }
        case DEVICE_DOWNLOADED:
        {
            if(DEVICE_DOWNLOADING == s->powerState)
            {
                s->powerState = state;
            }
            else
            {
                ret =1;
            }
            break;
        }
        case DEVICE_WORKING:
        {
            if(DEVICE_IDLED == s->powerState)
            {
                s->powerState = state;
            }
            else
            {
                ret =1;
            }
            break;
        }
        case DEVICE_WORKED:
        {
            if(DEVICE_WORKING == s->powerState)
            {
                s->powerState = state;
            }
            else
            {
                ret =1;
            }
            break;
        }
        case DEVICE_IDLING:
        {
            if((DEVICE_WORKED == s->powerState)||
                (DEVICE_SLEEP == s->powerState))
            {
                s->powerState = state;
            }
            else
            {
                ret =1;
            }
            break;
        }
        case DEVICE_IDLED:
        {
            if((DEVICE_IDLING == s->powerState)||
                (DEVICE_DOWNLOADED== s->powerState)||
                (DEVICE_WAKEUPING == s->powerState))
            {
                s->powerState = state;
            }
            else
            {
                ret =1;
            }
            break;
        }
        case DEVICE_WAKEUPING:
        {
           s->powerState = state; 
        }
        case DEVICE_SLEEP:
        {
            if((DEVICE_WORKED != s->powerState)||
               (DEVICE_WORKING != s->powerState))
            {
                s->powerState = state;
            }
            else
            {
                ret =1;
            }
            break;
        }
        case DEVICE_POWEROFFING:
        {
            s->powerState = state;
            break;
        }
        case DEVICE_POWEROFFED:
        {
            if(DEVICE_POWEROFFING == s->powerState)
            {
                s->powerState = state;
            }
            else
            {
                ret =1;
            }
            break;
        }
        case DEVICE_RESET:
        {
            s->powerState = state;
            break;
        }
        case DEVICE_ABNORMAL:
        {
           s->powerState = state;
           break;
        }
        case DEVICE_SLEEPING:
        {
           s->powerState = state;
           break;
        }
        default: 
            break;
    }
	
    if(ret)
    {
        GREENEYE2_LOGE(("[GE2]GNSS STATE :%s, should not set: %s",\
          gps_PowerStateName(s->powerState),\
          gps_PowerStateName(state)));
    }
    else
    {
        GREENEYE2_LOGD(("[GE2]SET POWER State change : %s\r\n",gps_PowerStateName(state)));
    }
}

const char* gps_PowerStateName(int state)
{
    switch(state)
    {
        case DEVICE_DOWNLOADING: return "GE2 DOWNLOADING";
        case DEVICE_DOWNLOADED:  return "GE2 DOWNLOADED";
        case DEVICE_POWERONING:  return "GE2 POWERING";
        case DEVICE_POWERONED:   return "GE2 POWERONED";
        case DEVICE_WORKING:     return "GE2 CP WORKING";  
        case DEVICE_WORKED:      return "GE2 CP WORKED"; 
        case DEVICE_IDLING:      return "GE2 CP IDLING";  
        case DEVICE_IDLED:       return "GE2 CP IDLED"; 
        case DEVICE_SLEEPING:   return "GE2 CP DEVICE_SLEEPING";  
        case DEVICE_SLEEP:       return "GE2 CP SLEEP";  
        case DEVICE_WAKEUPING:   return "GE2 CP DEVICE_WAKEUPING";   
        case DEVICE_RESET:       return "GE2 CP DEVICE_RESET";
        case DEVICE_POWEROFFING: return "GE2 POWEROFFING";  
        case DEVICE_POWEROFFED:  return "GE2 POWEROFFED"; 
        case DEVICE_ABNORMAL:    return "GE2 ABNORMAL";   
        default:                 return  "GE2 UNKOWN STATE";
    }  
}
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
