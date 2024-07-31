/******************************************************************************
 ** File Name:   greeneye2_comm.c                                                *
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
#include "greeneye2.h"
#include "greeneye2_common.h"
#include "greeneye2_cmd.h"
#include "greeneye2_engine.h"
#include "dal_time.h"
#include "gps_interface.h"
#include "sci_types.h"
#include "sci_api.h"
#include "threadx_os.h"
#include "threadx_assert.h"
#include "os_api.h"
#include <stdio.h>
#include "greeneye2_gnss_log.h"

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
#define JAN61980      44244
#define JAN11901      15385  

extern char gps_log_enable;

extern char *gps_idle_on ;
extern char *gps_idle_off;
extern Ge2PositionParam_t gLocationInfo[1];
extern uint8 gps_stop_flag;
extern uint8 gps_start_flag;
static char *gps_to_nmea;
char *libgps_version = "GE2_15A_W17.13.04,2017.03.30\r\n";

char ge2_logbuf[512];
uint32 ge2_loglen;
uint8 assertbuf[1024+1];
uint32 assertpos = 0;
int         gLogType = 0;
int         gWriteFlag = 1;
int         gDropCount = 0;
static char data_capture_flag = 0;
char data_exist_flag = 0;

typedef struct SCgTime
{
        unsigned short year;		/*< year */
        unsigned short month;		/*< month (0-11) */
        unsigned short day;		/*< day (0-31) */
        unsigned short hour;		/*< hour (0-23) */
        unsigned short minute;		/*< minute (0-59) */
        unsigned short second;		/*< second (0-59) */
        unsigned int  microsec;	/*< microseconds */
} TCgTime;

static const long month_day[2][13] = {
       {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
       {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};

GE2_EPHEMERIS_FLASH ge2_eph_data;
BOOLEAN ge2_saveflash = SCI_FALSE;
BOOLEAN ge2_savebdflash = SCI_FALSE;
BOOLEAN ge2_savegloflash = SCI_FALSE;
BOOLEAN ge2_sendephdata = SCI_FALSE;
BOOLEAN ge2_storeeph = SCI_FALSE;

#ifdef GPS_MODE_GPS_BD
GE2_BD_EPHEMERIS_FLASH ge2_bdeph_data;
#else
GE2_GLO_EPHEMERIS_FLASH ge2_gloeph_data;
#endif

int gps_weekno_diff = 0x0;
int gps_mscount_diff = 0x0;

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    TimeSystemToGps
//  Author:         
//  Note:          
/*****************************************************************************/
static int TimeSystemToGps(TCgTime *aTime, gpstime *pGpsTime)
{
    unsigned int yday, mjd, leap, totalDay;
    double in_seconds = 0.0, sec_per_week = 0.0, fmjd = 0.0;
    double daysFromEpoc = 0.0;


    if((aTime->year<2000) || (aTime->year>2099) || (aTime->month<1) || (aTime->month>12))
        return 1; // not valid

    in_seconds = (double)aTime->second + ((double)aTime->microsec / (double)1000000.0) ;

    leap = (aTime->year%4 == 0);

    yday = month_day[leap][aTime->month-1] + aTime->day - 1; //for 01/01 is 0 day
    totalDay = (aTime->year-1901) * 365 + (aTime->year-1900) / 4 + yday;

    mjd = totalDay + JAN11901;
    fmjd = ((in_seconds/(double)60 + aTime->minute)/(double)60 + aTime->hour)/(double)24;

    pGpsTime->weekno = (unsigned short)((mjd - JAN61980)/7);

    daysFromEpoc = (mjd - JAN61980) - (pGpsTime->weekno*7.0) + fmjd;
    sec_per_week  = daysFromEpoc * 86400;

    pGpsTime->mscount = (unsigned int)(sec_per_week * 1000);
  
    return 0;
}


/*****************************************************************************/
//  Description:    OsTimeSystemGet
//  Author:         
//  Note:          
/*****************************************************************************/
int OsTimeSystemGet( gpstime *aGpsTime)
{
    SCI_TIME_T t = {0};
    SCI_DATE_T tm1 = {0};
    TCgTime time = {0};

    TM_GetSysDate(&tm1);
        
    time.year  = tm1.year;
    time.month = tm1.mon;
    time.day = tm1.mday;

    TM_GetSysTime(&t);

    time.hour  = t.hour;
    time.minute = t.min;
    time.second = t.sec;
    time.microsec = 0;

    GE2GPS_LOGD("Systime:%.4d/%.2d/%.2d,%.2d:%.2d:%.2d,%ld",time.year,time.month,time.day,time.hour,time.minute,time.second,time.microsec);

    TimeSystemToGps(&time, aGpsTime);

    return 0;
}

/*****************************************************************************/
//  Description:    get_sys_time
//  Author:         
//  Note:          
/*****************************************************************************/
void get_sys_time(void)
{
    SCI_TIME_T t = {0};
    SCI_DATE_T tm1 = {0};
    TCgTime time;
	
    TM_GetSysDate(&tm1);
        
    time.year  = tm1.year;
    time.month = tm1.mon;
    time.day = tm1.mday;

    TM_GetSysTime(&t);

    time.hour  = t.hour;
    time.minute = t.min;
    time.second = t.sec;
}

/*****************************************************************************/
//  Description:    gps_power_Statusync
//  Author:         
//  Note:          
/*****************************************************************************/
int  gps_power_Statusync(int value)
{
    GpsState* s = _gps_state;
    int ret = 0;

    GREENEYE2_LOGD(("[GE2]gps_power_Statusync enter"));

    switch(value)
    {
        case IDLE_ON_SUCCESS:
            {
                if(s->powerState == DEVICE_IDLED)
                {
                    GREENEYE2_LOGD(("[GE2]idle on success! \r\n"));
                    s->powerErrorNum = 0;
                }
                else
                {
                    if(s->powerState == DEVICE_IDLING)
                    {
                        SCI_Sleep(2000);
                    }
                    //it need to repeat it again when powererrornum < 3
                    if((s->powerState == DEVICE_IDLING)&& (s->powerErrorNum < POWER_ERROR_NUMBER_MAX))
                    {
                        ret = 1; 
                        s->powerErrorNum++;
                        gps_sendData(IDLEON_STAT, gps_idle_on);
                        GREENEYE2_LOGE(("[GE2]idle on fail,maybe some wrong \r\n"));
                    }
                }       
            break;
            }
        case IDLE_OFF_SUCCESS:
            {
                if(s->powerState == DEVICE_WORKED)
                {
                    GREENEYE2_LOGD(("[GE2]idle off success \r\n"));
                    s->powerErrorNum = 0;
                }
                else
                {
                    if(s->powerState == DEVICE_WORKING)
                    {
                        SCI_Sleep(200);
                    }  
                    //it need to repeat it again when powererrornum < 3
                    if((s->powerState == DEVICE_WORKING)&& (s->powerErrorNum < POWER_ERROR_NUMBER_MAX))
                    {
                        ret = 1;
                        s->powerErrorNum++;
                        GREENEYE2_LOGE(("[GE2]idle off fail,maybe some wrong \r\n"));
                    }  
                }
                break;
            }
        case POWER_WAKEUP:
            {
                if(s->powerState == DEVICE_IDLED)
                {
                    GREENEYE2_LOGD(("[GE2]it wakeup success"));
                    s->powerErrorNum = 0;
                }
                else
                {
                    if(s->powerState == DEVICE_WAKEUPING)
                    {
                        if(4 != ((s->powerErrorNum)%5))
                        {
                            SCI_Sleep(10);
                        }
                        else
                        {
                            SCI_Sleep(2000);
                        }
                    }

                    if(s->powerState == DEVICE_WAKEUPING)
                    {
                        ret = 1;
                        s->powerErrorNum++;
                        if(0 == ((s->powerErrorNum)%100))
                            GREENEYE2_LOGE(("[GE2]wake up fail %d,so try again",s->powerErrorNum));
                    }
                }
                break;
            }
        case POWER_SLEEPING:
            if(s->powerState == DEVICE_SLEEP)
            {
                GREENEYE2_LOGD(("[GE2]it sleep success"));
                s->powerErrorNum = 0;
            }
            else
            {
                if(s->powerState == DEVICE_SLEEPING)
                {
                    SCI_Sleep(200);
                }  
                //it need to repeat it again when powererrornum < 3
                if((s->powerState == DEVICE_SLEEPING)&& (s->powerErrorNum < POWER_ERROR_NUMBER_MAX))
                {
                    ret = 1;
                    s->powerErrorNum++;
                    GREENEYE2_LOGE(("[GE2]it sleep  fail,maybe some wrong \r\n"));
                }  
            }
            break;

        default:// we dont't care the  power on event 
            GREENEYE2_LOGE(("[GE2]the gps_power_Statusync value =%d \r\n",value));
            break;

    }

    return ret;
}

/*****************************************************************************/
//  Description:    gps_getPowerResponse
//  Author:         
//  Note:          
/*****************************************************************************/
static void  gps_getPowerResponse(TCmdData_t* pPacket)
{
    GpsState* s = _gps_state;

	
	GREENEYE2_LOGD(("[GE2]gps_getPowerResponse	subtype = %d\r\n",pPacket->subType));
	TM_SendTestPointRequest(pPacket->subType,10101809); 

    switch(pPacket->subType)
    {
  	    case GNSS_LIBGPS_POWERON_STATUS_SUCCESS:
		gps_setPowerState(DEVICE_POWERONED);
		break;

  	    case GNSS_LIBGPS_IDLON_STATUS_SUCCESS:
		gps_setPowerState(DEVICE_IDLED);
		break;

  	    case GNSS_LIBGPS_IDLOFF_STATUS_SUCCESS:
		gps_setPowerState(DEVICE_WORKED);
		break;

        case GNSS_LIBGPS_SHUTDOWN_STATUS_SUCCESS:
		gps_setPowerState(DEVICE_POWEROFFED);
		break;

            case GNSS_LIBGPS_RESET_STATUS_SUCCESS:
		gps_setPowerState(DEVICE_POWERONED);
		break;

  	    case GNSS_LIBGPS_POWERON_STATUS_FAILED:
	    case GNSS_LIBGPS_IDLON_STATUS_FAILED:
        case GNSS_LIBGPS_IDLOFF_STATUS_FAILED:
        case GNSS_LIBGPS_RESET_STATUS_FAILED:
        case GNSS_LIBGPS_SHUTDOWN_STATUS_FAILED:
		s->powerErrorNum++;
		break;

	    default:
		GREENEYE2_LOGE(("[GE2]gps_getPowerResponse  subtype = %d\r\n",pPacket->subType));
		break;
    }
}

/*****************************************************************************/
//  Description:    gps_getNotified
//  Author:         
//  Note:          
/*****************************************************************************/
static void  gps_getNotified(TCmdData_t* pPacket)
{
    GpsState* s = _gps_state;
    unsigned char  state;
    
    if(GNSS_NOTIFIED_DEVICE_STATE_SUBTYPE == pPacket->subType)
    {
        if(pPacket->length == 2)
        {
            state = pPacket->buff[0];
            if(state == GNSS_DEVICE_READY_STATE)
            {
                GREENEYE2_LOGD(("[GE2]gps_getNotified Device %d Ready ",pPacket->buff[1]));
                gps_setPowerState(DEVICE_IDLED);
                GREENEYE2_LOGD(("[GE2] gps_getNotified %d",SCI_GetTickCount()));
		TM_SendTestPointRequest(pPacket->subType,10101810); 
            }
            else if(state == GNSS_DEVICE_SLEEP_STATE)
            {
                GREENEYE2_LOGD(("[GE2]gps_getNotified sleep %d success,%d",pPacket->buff[1],SCI_GetTickCount()));
                gps_setPowerState(DEVICE_SLEEP);
		TM_SendTestPointRequest(pPacket->subType,10101811); 
            }
        }
        else
        {
            GREENEYE2_LOGE(("[GE2]gps_getNotified data len error"));
	    TM_SendTestPointRequest(pPacket->subType,10101812); 
        }
    }
}

/*****************************************************************************/
//  Description:    get_one_nmea
//  Author:         
//  Note:          
/*****************************************************************************/
int get_one_nmea(GpsState* s,unsigned char *buf,int len)
{
    int pos = 0;
    int i = 0;
    int temp = len;

    while(!((buf[i] == '\r') && (buf[i+1] == '\n')))
    {
        s->NmeaReader[0].in[i] = buf[i];
        i++;	
        temp--;	
        if(temp <= 0)
        {
            return 0;
        }
    }
    s->NmeaReader[0].in[i] = buf[i];
    if(s->logswitch)
    {
        GREENEYE2_LOGD(("[GE2]get one nmea,buf[i]=%d",s->NmeaReader[0].in[i]));
    }
    s->NmeaReader[0].in[i+1] = buf[i+1];
    return i+2;
}

/*****************************************************************************/
//  Description:    parseNmea
//  Author:         
//  Note:          
/*****************************************************************************/
void parseNmea(unsigned char *buf,int len)
{
    GpsState*  s = _gps_state;  
    unsigned char *temp = buf;

    if(0 == s->postlog)
    {
        GREENEYE2_LOGD(("[GE2]the nmea log len = %d\r\n",len));
        //preprocessGnssLog(buf,len,GNSS_LIBGPS_NMEA_SUBTYPE);
    }

    while(len > 9)
    {
        //GREENEYE2_LOGD(("[GE2]parse nmea enter"));
        SCI_MEMSET(s->NmeaReader[0].in,0,(NMEA_MAX_SIZE+1));
        s->NmeaReader[0].pos = get_one_nmea(s,temp,len);
        if(s->NmeaReader[0].pos == 0)
        {
            GREENEYE2_LOGE(("[GE2]read last nema log is not complete"));
            break;
        }
        temp = temp + s->NmeaReader[0].pos;
        len = len - s->NmeaReader[0].pos;
        GREENEYE2_LOGD(("[GE2]%s \r\n",s->NmeaReader[0].in));
        nmea_reader_parse(s->NmeaReader);
    }
    return;
}

/*****************************************************************************/
//  Description:    GPS_HeartCountRsp
//  Author:         
//  Note:          
/*****************************************************************************/
void GPS_HeartCountRsp(GpsState* pGPSState)
{
    if(pGPSState->waitHeartRsp)
    {
        pGPSState->waitHeartRsp = 0;
        pGPSState->HeartCount = 0;
    }
    GREENEYE2_LOGD(("[GE2]&&& GPS_HeartCountRsp  HeartCount %d\r\n",pGPSState->HeartCount));
}

/*****************************************************************************/
//  Description:    saveRFData
//  Author:         
//  Note:          
/*****************************************************************************/
void saveRFData(unsigned char  *data,unsigned short len)
{
        GpsState*  s = _gps_state;

    #if 0
    int bufferLen = sizeof(ge2_logbuf);
    int recount = GNSS_LOG_MAX_RETRY;

    if(len == 0)
        return;

    if(len > bufferLen)
    {
        GREENEYE2_LOGE(("[GE2]%s datalen:%d > 512",__FUNCTION__,len));
    }

    while(recount)
    {
        if(gWriteFlag)
        {
            break;
        }
        else
        {
            gps_sendlogmsg();
            SCI_Sleep(5);//1ms 
            recount--;
        }
    }
    
    if(0 == gWriteFlag)
    {
        gDropCount++;
        gWriteFlag = 1; 
        GREENEYE2_LOGD(("[GE2]%s will drop count:%d gnss log :%s",__FUNCTION__,gDropCount,ge2_logbuf));
    }

    if(len == 0)
        return;

    if(len > bufferLen)
    {
        GREENEYE2_LOGE(("[GE2]%s datalen:%d > 512",__FUNCTION__,len));
    }

    SCI_MEMSET(ge2_logbuf,0,bufferLen);
    memcpy(ge2_logbuf,data,len);
    ge2_loglen = len;
    gLogType = GNSS_TYPE_LOG_RF;//it only save file 
    gWriteFlag = 0;
    //pthread_mutex_unlock(&pState->gnsslogmutex);
    gps_sendlogmsg();
    SCI_Sleep(5);
    #else
    s->callbacks.nmea_cb(GNSS_TYPE_LOG_RF,(const char*)data,len);
    #endif
}

#if 0
void parsePstat(unsigned char *buf,int len)
{
    GpsState*  s = _gps_state;
	unsigned char *temp;
	unsigned char tempdata[128];
	int length = 120;

	if(memcmp(buf,"$PSTAT",6) != 0)
	{
		return;
	}

	D("the pstat log len = %d\r\n",len);

	memcpy(tempdata, buf, length);
	tempdata[length++] = '\r';
	tempdata[length++] = '\n';
	len = length;

	temp = tempdata;

	//s->callbacks.nmea_cb(s->NmeaReader[0].fix.timestamp, (const char*)temp, (int)len);
    savenmealog2Buffer((const char*)temp, len);
	while(len > 9)
	{
		memset(s->NmeaReader[0].in,0,(NMEA_MAX_SIZE+1));
		s->NmeaReader[0].pos = get_one_nmea(s,temp,len);
		if(s->NmeaReader[0].pos == 0)
		{
			E("read last pstat log is not complete");
			break;
		}
		temp = temp + s->NmeaReader[0].pos;
		len = len - s->NmeaReader[0].pos;
		if(s->logswitch)
		{
		    D("%s \r\n",s->NmeaReader[0].in);
		}
		nmea_reader_parse(s->NmeaReader);
	}
    return;
}
#endif

/*****************************************************************************/
//  Description:    gps_getRFData
//  Author:         
//  Note:          
/*****************************************************************************/
static void gps_getRFData(TCmdData_t *pPacket)
{
	char response[256];
	int i = 0;
	time_t t;
	struct tm *p;
    static int rawcount = 0; 
    GpsState*  s = _gps_state;
	//char fname[64] = {0};

	if(NULL == pPacket)
	{
		return;
	}

	//GREENEYE2_LOGD(("[GE2]####data_capture_flag=%d",data_capture_flag));

	if(data_capture_flag == 1)
	{
	    rawcount++;
        saveRFData(pPacket->buff, pPacket->length);
        if( rawcount > 16384)
        {
          
            if(!memcmp(&pPacket->buff[i],"DATA_CAPTURE_END",strlen("DATA_CAPTURE_END")))
            {
                GREENEYE2_LOGD(("[GE2]####data capture end."));
                data_capture_flag = 0;
                data_exist_flag = 1;
                 rawcount = 0;
                //fclose(datafp);
                //datafp = NULL; 
                //gLogType = GNSS_TYPE_LOG_RF;
                //SignalEvent(&s->gnssLogsync);
               // break;
            }
            else
            {
                GREENEYE2_LOGE(("[GE2]####data capture  should end, but not get the DATA_CAPTURE_END"));
            }
        }

	}

	if(data_capture_flag == 0)
	{
		for(i = 0; i < pPacket->length; i++)
		{
			if(!memcmp(&pPacket->buff[i],"DATA_CAPTURE_BEGIN",strlen("DATA_CAPTURE_BEGIN")))
			{
				GREENEYE2_LOGD(("[GE2]####data capture begin."));
				data_capture_flag = 1;
				data_exist_flag = 0;

				//t = time(NULL);
				//p = gmtime(&t);

				//memset(fname,0,64);
				//sprintf(fname, "data/gnss/%d-%d-%d_%d-%d-%d.txt",1900+p->tm_year,1+p->tm_mon, p->tm_mday, 8+p->tm_hour,p->tm_min,p->tm_sec);
				//D("%s\n", fname);
                                rawcount = 0;

				if(data_capture_flag == 1)
				{
					saveRFData(pPacket->buff, pPacket->length);
				}
				break;
			}
		}
	}

}


/*****************************************************************************/
//  Description:    gps_getData
//  Author:         
//  Note:          
/*****************************************************************************/
static void  gps_getData(TCmdData_t* pPacket)
{
	GpsState* s = _gps_state;
	int i = 0,ret = 0;
	static int flagassert =0;
	static char fia = 0;
	//static EphExt ephext;
	TCmdData_t Pak;
	int len = 0;


    if(fia == 0)
    {
        fia = 1;
    }
    if(NULL == pPacket)
    {
        GREENEYE2_LOGE(("[GE2]get data,packet is null"));
        return;
    }

    switch(pPacket->subType)
    {
        case GNSS_LIBGPS_LOG_SUBTYPE:
                /*if(s->float_cn_enable == 1)
                {
                    parsePstat(pPacket->buff, pPacket->length);
                }*/
                if(s->rftool_enable == 1)
                {
                    gps_getRFData(pPacket);
                }
#if 0
                else
                {
                    if((s->postlog == 1) || (s->cw_enable == 1))	 // HW_CW
                    {
                        savenmealog2Buffer((const char*)pPacket->buff, (int)pPacket->length);
                    }
                }
#endif
        	break;
        case GNSS_LIBGPS_NMEA_SUBTYPE:
			if (gps_stop_flag == 0)
            parseNmea(pPacket->buff,pPacket->length);
			else
				GE2GPS_LOGD("recevied stop signel, so don't need to parse nmea data, gps_stop_flag = %d\n", gps_stop_flag);
            break;
        case GNSS_LIBGPS_SOFTBIT_SEND_SUBTYPE:
            break;
        case GNSS_LIBGPS_SOFTBIT_CLEAR_SUBTYPE:
            break;
        case GNSS_LIBGPS_ASSERT_SUBTYPE:
            //save assert file todo 
            GREENEYE2_LOGE(("[GE2]$$$$ assert length=%d,buffer=%s\r\n",pPacket->length,pPacket->buff));	
            break;
        case GNSS_LIBGPS_MEMDUMP_DATA_SUBTYPE:
            break;
        case GNSS_LIBGPS_MEMDUMP_CODE_SUBTYPE:
			SCI_MEMCPY(&assertbuf[assertpos],pPacket->buff,pPacket->length);
			assertpos += pPacket->length;
            break;
        case GNSS_LIBGPS_MEMDUMP_PCHANNEL_SUBTYPE:
            break;
        case GNSS_LIBGPS_MSA_MEASUREMENT_SUBTYPE:
            break;
        case GNSS_LIBGPS_OBS_SUBTYPE:
            break;
        case GNSS_LIBGPS_TEST_SUBTYPE:
        {
            assertpos = 0;
            if(s->watchdog)
            {
                GPS_HeartCountRsp(s);
            }
            else
            {
                GREENEYE2_LOGE(("[GE2]it should not recv the heart response!"));
            }
            break;
        }
		case GNSS_LIBGPS_SAVE_TSX_TEMP:
			break;
		case GNSS_LIBGPS_CALI_TSX_TEMP:
			break;
		case GNSS_LIBGPS_ELLIPES_SUBTYPE:
			break;
		case GNSS_LIBGPS_TTFF_SUBTYPE:
			{
				SCI_MEMCPY(&(gLocationInfo->ttff),pPacket->buff,pPacket->length);
			}
			break;
        default:
            break;
    }
    return;
}

/*****************************************************************************/
//  Description:    gps_getAdiData
//  Author:         
//  Note:          
/*****************************************************************************/
static void gps_getAdiData(TCmdData_t* pPacket)
{
    GpsState* s = _gps_state;
    int ret = 0;

    if(NULL == pPacket)
	return;

    switch(pPacket->subType)
    {
	case GNSS_LIBGPS_AIDING_ASSISTANCE_SUBTYPE:
		GREENEYE2_LOGD(("[GE2]start agps si session"));
		if(pPacket->length == sizeof(int))
		{
			/*SCI_MEMCPY(&agpsdata->assistflag,pPacket->buff,sizeof(int));
			GREENEYE2_LOGD(("[GE2]assist request flag from firmware is 0x%x",agpsdata->assistflag));
			if(agpsdata->assistflag == 0x7)
			{
				agpsdata->assistflag = 0x134;
			}*/
			{
				#ifdef GPS_AGPS_SUPPORT_QX
				GPS_GET_AGPSDATA_T *signal = PNULL;

	                        SCI_CREATE_SIGNAL(
	                        signal,
	                        GPS_GET_AGPSDATA_REQ,
	                        sizeof (GPS_GET_AGPSDATA_T),
	                        SCI_IDENTIFY_THREAD());
	                        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, AGPS_GetInterfaceThreadId()); 
				#endif
			}
		}
		else
		{
			//agpsdata->assistflag = 0x134;
		}
		break;
	case GNSS_LIBGPS_AIDING_SET_SENSOR_SUBTYPE:
		GREENEYE2_LOGD(("[GE2]recive sensor control command"));
		/*SCI_MEMCPY(&agpsdata->SensorStatus,pPacket->buff,sizeof(int));
		sem_post(&agpsdata->JavaEvent);*/
		break;
	default:
		break;
    }
}

/*--------------------------------------------------------------------------
	Function:  gps_getResponse

	Description:
		it  parse response from ge2,these response is answer for libgps request. 
	Parameters:
 		packet in uart: 
	Return:void
--------------------------------------------------------------------------*/
static void gps_getResponse(TCmdData_t *pPacket)
{
	GpsState*  s = _gps_state;
	char response[256];
	//char log_string[16];
	int cpmode = 0,i = 0;
    unsigned int len = 0;
	//gpstime TT;
	SCI_MEMSET(response,0,sizeof(response));
	GREENEYE2_LOGE(("[GE2]getresponse subtype is %d",pPacket->subType));
	if(NULL == pPacket)
	{
        return;
	}
	switch(pPacket->subType)
	{
		case GNSS_LIBGPS_RESPONSE_VERSION_TYPE:
		    for(i = 0; i < pPacket->length; i++)
		    {
		        if(!memcmp(&pPacket->buff[i],"VERSION:HW",strlen("VERSION:HW")))
		        {
		            i = i + strlen("VERSION:HW") + 1;
		            memcpy(response,&pPacket->buff[i],pPacket->length-i-2);
                    GREENEYE2_LOGD(("[GE2]gnss version get: %s,strlen is %d,%d",response,strlen(response),SCI_GetTickCount()));
					len = strlen(response);
					response[len] = 'O';
					response[len+1] = '\0';
                    if(strstr(response, "GLO") != NULL)
                    {
                        s->cpmode = 5;
                        s->workmode = GPS_GLONASS;
                    }
                    else
                    {
                        s->cpmode = 3;
                         s->workmode = GPS_BDS;
                    }
                    break;
		        }
		    }
		    break;
		case GNSS_LIBGPS_RESPONSE_CHIP_SUBTYPE:
			break;
		case GNSS_LIBGPS_RESPONSE_GPSTIME_SUBTYPE:
			break;
		case GNSS_LIBGPS_RESPONSE_CONSTELLATION_SUBTYPE:
			break;
		default:
			break;

	}
}

#ifdef GPS_MINI_LOG_SUPPORT
/*--------------------------------------------------------------------------
	Function:  preprocessGnssLog

	Description:
		
	Parameters:
 		 
	Return:void
--------------------------------------------------------------------------*/
void preprocessGnssLog(unsigned char  *data,unsigned short len,unsigned char subType)
{
   // struct stat log_stat;
    char linebuf[300];
    char *ptemp = NULL;
    int num = 0,i;
    int recount = GNSS_LOG_MAX_RETRY;
    GpsState*  pState = _gps_state; 
    int bufferLen = sizeof(ge2_logbuf);

    if(len == 0)
        return;

    if(len > bufferLen)
    {
        GREENEYE2_LOGE(("[GE2]%s datalen:%d > 512",__FUNCTION__,len));
    }

#if 0
    while(recount)
    {
        if(gWriteFlag)
        {
            break;
        }
        else
        {
            gps_sendlogmsg();
            SCI_Sleep(5);//1ms 
            recount--;
        }
    }
    
    if(0 == gWriteFlag)
    {
        gDropCount++;
        gWriteFlag = 1; 
        GREENEYE2_LOGD(("[GE2]%s will drop count:%d gnss log :%s",__FUNCTION__,gDropCount,ge2_logbuf));
    }
#endif
    if((GNSS_LIBGPS_ASSERT_SUBTYPE == subType) || (GNSS_LIBGPS_LOG_SUBTYPE == subType)||(GNSS_LIBGPS_NMEA_SUBTYPE == subType)||(GNSS_LIBGPS_FLASH_TYPE == subType))
    {
	//writeGnsslogTokernel(data,len);

        //GREENEYE2_LOGD(("[GE2]preprocessGnssLog 1 len =%d type :%d",len,subType));
        GREENEYE2_LOGD(("[GE2]preprocessGnssLog 2 data= %s",data));
        //pthread_mutex_lock(&pState->gnsslogmutex);
	//SCI_MEMSET(ge2_logbuf,0,bufferLen);
        //memcpy(ge2_logbuf,data,len);
        //ge2_loglen = len;
        //gLogType = GNSS_TYPE_LOG_TRACE;//it only save file 
        //gWriteFlag = 0;
        //pthread_mutex_unlock(&pState->gnsslogmutex);
        //gps_sendlogmsg();
        pState->callbacks.nmea_cb(GNSS_TYPE_LOG_TRACE,data,len);
    }
    else if(GNSS_LIBGPS_MINILOG_SUBTYPE == subType)
    {
        
        memcpy(&num,data,sizeof(int));
        //GREENEYE2_LOGD(("[GE2]preprocessGnssLog 2 len =%d,num=%d",len,num));

        if(num > LINE_MAX_NUM)
        {
            GREENEYE2_LOGE(("[GE2]preprocessGnssLog 2 line num:%d is too large,return",num));
            return;
        }

        SCI_MEMSET(ge2_logbuf,0,bufferLen);
        memcpy(ge2_logbuf,data,len);
        ge2_loglen = len;
        gLogType = GNSS_TYPE_LOG_TRACE;
        gWriteFlag = 0;
        //pthread_mutex_unlock(&pState->gnsslogmutex);
        //gps_sendlogmsg();
        pState->callbacks.nmea_cb(GNSS_TYPE_MINILOG_TRACE,data,len);
    }
    else
    {
        TGNSS_Baseband_Log_t* pBasebandlog = (TGNSS_Baseband_Log_t*)data;
        int count = len/GNSS_LOG_BB_LEN;
        GREENEYE2_LOGD(("[GE2]preprocessGnssLog 3  len =%d",len));
        //pthread_mutex_lock(&pState->gnsslogmutex);            
        SCI_MEMSET(ge2_logbuf,0,bufferLen);
        sprintf(ge2_logbuf,"GNSS Baseband Log start len =%d,count=%d\r\n",len,count);
        while(count--)
        {
            sprintf(ge2_logbuf+strlen(ge2_logbuf),"type:%d,svid:%d,bbtime:%d,%d,%d,%d,%d,%d,%d,%d\r\n",
                        pBasebandlog->type,pBasebandlog->svid,
                        pBasebandlog->bbtime,pBasebandlog->data4,
                        pBasebandlog->data5,pBasebandlog->data6,
                        pBasebandlog->data7,pBasebandlog->data1,
                        pBasebandlog->data2,pBasebandlog->data3);
                        pBasebandlog++;
        }

        sprintf(ge2_logbuf+strlen(ge2_logbuf),"GNSS Baseband Log End\r\n");
        ge2_loglen = strlen(ge2_logbuf);
        gLogType = GNSS_TYPE_LOG_TRACE;
        gWriteFlag = 0;
        //pthread_mutex_unlock(&pState->gnsslogmutex);
        //gps_sendlogmsg();
        //writeGnsslogTokernel((unsigned char*)logbuf, (unsigned short)strlen(logbuf));
            pState->callbacks.nmea_cb(GNSS_TYPE_LOG_TRACE,ge2_logbuf,ge2_loglen);

    }

    TM_SendTestPointRequest(subType,92430); 
    //SCI_Sleep(5);//release read thread 
       
}
#endif

#if 1
/*****************************************************************************/
//  Description:    gps_clear_ephdata
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_clear_ephdata(GpsState* s)
{
	uint8 count = 0;
	uint32 status = 0;
	uint32 length;
	
	for(count = 1;count < 32;count ++)
	{
		switch(count)
		{
		         case GNSS_LIBGPS_FLASH_GPS_EPH1_SUBTYPE:
				length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsEph[0]),0,length);
		            break;
		        case GNSS_LIBGPS_FLASH_GPS_EPH2_SUBTYPE:
				length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsEph[1]),0,length);
		            break;
		        case GNSS_LIBGPS_FLASH_GPS_EPH3_SUBTYPE:
				length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsEph[2]),0,length);
				break;
		        case GNSS_LIBGPS_FLASH_GPS_EPH4_SUBTYPE:
				length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsEph[3]),0,length);
				break;
		        case GNSS_LIBGPS_FLASH_QZS_EPH_SUBTYPE:
				length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_eph_data.qzsEph),0,length);
		            break;
#ifdef GPS_MODE_GPS_BD
		         case GNSS_LIBGPS_FLASH_BD2_EPH1_SUBTYPE:
				length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_bdeph_data.bdEph[0]),0,length);
		            break;
		        case GNSS_LIBGPS_FLASH_BD2_EPH2_SUBTYPE:
				length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_bdeph_data.bdEph[1]),0,length);
		            break;
#else
		        case GNSS_LIBGPS_FLASH_GLO_EPH1_SUBTYPE:
				length = sizeof(GE2_BLOCK_GLO_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_gloeph_data.gloEph[0]),0,length);
		            break;
		        case GNSS_LIBGPS_FLASH_GLO_EPH2_SUBTYPE:
				length = sizeof(GE2_BLOCK_GLO_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_gloeph_data.gloEph[1]),0,length);
		            break;
		        case GNSS_LIBGPS_FLASH_GLO_EPH3_SUBTYPE:
				length = sizeof(GE2_BLOCK_GLO_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_gloeph_data.gloEph[2]),0,length);
		            break;
		        case GNSS_LIBGPS_FLASH_GLO_EPH4_SUBTYPE:
				length = sizeof(GE2_BLOCK_GLO_EPHEMERIS_FLASH);
				SCI_MEMSET((uint8 *)&(ge2_gloeph_data.gloEph[3]),0,length);
#endif
		        default:
		            break;
		}

	}

	status = EFS_NvitemWrite(NV_GPS_EPHEMERIS_PARAM, sizeof(GE2_EPHEMERIS_FLASH), (uint8 *)(&ge2_eph_data), 1);
	GE2GPS_LOGD("gps  clear eph ,status=%d!",status);

#ifdef GPS_MODE_GPS_BD
	status = EFS_NvitemWrite(NV_BD_EPHEMERIS_PARAM, sizeof(GE2_BD_EPHEMERIS_FLASH), (uint8 *)(&ge2_bdeph_data), 1);
	GE2GPS_LOGD("bd  clear eph ,status=%d!",status);
#else
	status = EFS_NvitemWrite(NV_GLO_EPHEMERIS_PARAM, sizeof(GE2_GLO_EPHEMERIS_FLASH), (uint8 *)(&ge2_gloeph_data), 1);
	GE2GPS_LOGD("glo  clear eph ,status=%d!",status);
#endif
        //if(NVERR_NONE != status)
            //SCI_PASSERT(0,("[GE2]%s:gps_saveFlashCfg fail,status=%d",__FUNCTION__,status));

}

/*****************************************************************************/
//  Description:    gps_clear_almdata
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_clear_almdata(GpsState* s)
{
	uint8 count = 0;
	uint32 status = 0;
	uint32 length;
	
	for(count = 5;count < 32;count ++)
	{
	    switch(count)
	    {
	            case GNSS_LIBGPS_FLASH_GPS_ALM_SUBTYPE:
			length = sizeof(GE2_BLOCK_GPS_ALMANAC_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsAlmanac),0,length);
			break;
	            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY1_SUBTYPE:
			length = sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsAmcopy[0]),0,length);
			break;
	            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY2_SUBTYPE:
			length = sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsAmcopy[1]),0,length);
	                break;
	            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY3_SUBTYPE:
			length = sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsAmcopy[2]),0,length);
	                break;
	            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY4_SUBTYPE:
			length = sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsAmcopy[3]),0,length);
	                break;
	            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY5_SUBTYPE:
			length = sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsAmcopy[4]),0,length);
	                break;
	            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY6_SUBTYPE:
			length = sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsAmcopy[5]),0,length);
	                break;
	            case GNSS_LIBGPS_FLASH_QZS_ALM_SUBTYPE:
	                break;
#ifdef GPS_MODE_GPS_BD
	            case GNSS_LIBGPS_FLASH_BD2_ALM_SUBTYPE:
			length = sizeof(GE2_BLOCK_BD2_ALMANAC_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_bdeph_data.bdAlmanac),0,length);
	                break;
#else
	            case GNSS_LIBGPS_FLASH_GLO_ALM1_SUBTYPE:
			length = sizeof(GE2_BLOCK_GLO_ALMANAC_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_gloeph_data.gloAlmanac[0]),0,length);
	                break;
	            case GNSS_LIBGPS_FLASH_GLO_ALM2_SUBTYPE:
			length = sizeof(GE2_BLOCK_GLO_ALMANAC_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_gloeph_data.gloAlmanac[1]),0,length);
	                break;
#endif
	            case GNSS_LIBGPS_FLASH_ALMCPY_COMM_SUBTYPE:
			length = sizeof(GE2_BLOCK_GPS_ALMCOM_FLASH);
			SCI_MEMSET((uint8 *)&(ge2_eph_data.gpsAlmcom),0,length);
	                break;
	            default:
	                break;
	   }
        }

	status = EFS_NvitemWrite(NV_GPS_EPHEMERIS_PARAM, sizeof(GE2_EPHEMERIS_FLASH), (uint8 *)(&ge2_eph_data), 1);
	GE2GPS_LOGD("gps  clear alm ,status=%d!",status);

#ifdef GPS_MODE_GPS_BD
	status = EFS_NvitemWrite(NV_BD_EPHEMERIS_PARAM, sizeof(GE2_BD_EPHEMERIS_FLASH), (uint8 *)(&ge2_bdeph_data), 1);
	GE2GPS_LOGD("bd  clear eph ,status=%d!",status);
#else
	status = EFS_NvitemWrite(NV_GLO_EPHEMERIS_PARAM, sizeof(GE2_GLO_EPHEMERIS_FLASH), (uint8 *)(&ge2_gloeph_data), 1);
	GE2GPS_LOGD("glo  clear eph ,status=%d!",status);
#endif
        //if(NVERR_NONE != status)
            //SCI_PASSERT(0,("[GE2]%s:gps_saveFlashCfg fail,status=%d",__FUNCTION__,status));

}

/*****************************************************************************/
//  Description:    gps_saveFlashCfg
//  Author:         
//  Note:   
/*****************************************************************************/
static void gps_saveFlashCfg(TCmdData_t * pPacket)
{
    //GpsState* s = _gps_state;
    uint32 status = 0;
    BOOLEAN flashcfg = SCI_TRUE;
    gpstime Gtime;    

    if(NULL == pPacket)
    {
        return;
    }

    if(pPacket->subType < GNSS_LIBGPS_FLASH_MAX_LEN_SUBTYPE)
    {
        GREENEYE2_LOGD(("[GE2]begin save flash data,subtype is %d,length is %d",pPacket->subType,pPacket->length));

        switch(pPacket->subType)
        {
            case GNSS_LIBGPS_FLASH_PvtMisc_SUBTYPE:  //ok
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.pvttype),pPacket->buff,pPacket->length);
		OsTimeSystemGet(&Gtime);
		if(Gtime.weekno > 0)
		{
			gps_weekno_diff =  ge2_eph_data.pvttype.PvtPkg.week - Gtime.weekno;
			gps_mscount_diff =  ge2_eph_data.pvttype.PvtPkg.msCount - Gtime.mscount;
		}
                break;
             case GNSS_LIBGPS_FLASH_GPS_EPH1_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsEph[0]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_EPH2_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsEph[1]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_EPH3_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsEph[2]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_EPH4_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsEph[3]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsAlmanac),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY1_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsAmcopy[0]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY2_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsAmcopy[1]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY3_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsAmcopy[2]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY4_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsAmcopy[3]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY5_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsAmcopy[4]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY6_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsAmcopy[5]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_QZS_EPH_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.qzsEph),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_QZS_ALM_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
#ifdef GPS_MODE_GPS_BD
             case GNSS_LIBGPS_FLASH_BD2_EPH1_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_bdeph_data.bdEph[0]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_BD2_EPH2_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_bdeph_data.bdEph[1]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_bdeph_data.bdAlmanac),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_CPY1_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_CPY2_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_CPY3_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_CPY4_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
#else
	    case GNSS_LIBGPS_FLASH_GLO_EPH1_SUBTYPE:
                 SCI_MEMCPY((uint8 *)&(ge2_gloeph_data.gloEph[0]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GLO_EPH2_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_gloeph_data.gloEph[1]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GLO_EPH3_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_gloeph_data.gloEph[2]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GLO_EPH4_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_gloeph_data.gloEph[3]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GLO_ALM1_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_gloeph_data.gloAlmanac[0]),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_GLO_ALM2_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_gloeph_data.gloAlmanac[1]),pPacket->buff,pPacket->length);
                break;
#endif				
            case GNSS_LIBGPS_FLASH_IONO_UTC_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.ionoutc),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_SYS_CFG_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_ALMCPY_COMM_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.gpsAlmcom),pPacket->buff,pPacket->length);
                break;
           case GNSS_LIBGPS_FLASH_PVT_SEQLSQ_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.seqlsq),pPacket->buff,pPacket->length);
                break;
            case GNSS_LIBGPS_FLASH_PVT_BLUNDER_SUBTYPE:
                SCI_MEMCPY((uint8 *)&(ge2_eph_data.blunder),pPacket->buff,pPacket->length);
                break;
            default:
                break;
        }
        if(flashcfg)
        {
            //status = EFS_NvitemWrite(NV_GPS_EPHEMERIS_PARAM, sizeof(GE2_EPHEMERIS_FLASH), (uint8 *)(&ge2_eph_data), 1);
	    //GE2GPS_LOGD("save eph data,status=%d",status);		
            /*if(NVERR_NONE != status)
                SCI_PASSERT(0,("[GE2]gps_saveFlashCfg fail,status=%d",status));*/
	    ge2_saveflash = SCI_TRUE;
	    ge2_savebdflash = SCI_TRUE;
	    ge2_savegloflash = SCI_TRUE;
	    ge2_storeeph = SCI_TRUE;
        }

     }
    else
    {
        GREENEYE2_LOGE(("[GE2]the save flash cfg index error\r\n"));
    }

}

void gps_saveEphToFlash()
{
    	uint32 status = 0;

	if(!ge2_storeeph)
	{
		GE2GPS_LOGD("eph data no need save,save_cfg=%d",ge2_storeeph);
		return;
	}

	status = EFS_NvitemWrite(NV_GPS_EPHEMERIS_PARAM, sizeof(GE2_EPHEMERIS_FLASH), (uint8 *)(&ge2_eph_data), 1);
	GE2GPS_LOGD("save gps eph,status=%d",status);
	if(NVERR_NONE != status)
	{
		//SCI_PASSERT(0,("[GE2]gps_saveFlashCfg fail,status=%d,ge2_storeeph=%d",status,ge2_storeeph));
	}
	SCI_Sleep(100);
#ifdef GPS_MODE_GPS_BD
	status = EFS_NvitemWrite(NV_BD_EPHEMERIS_PARAM, sizeof(GE2_BD_EPHEMERIS_FLASH), (uint8 *)(&ge2_bdeph_data), 1);
	GE2GPS_LOGD("save bd eph ,status=%d",status);
#else
	status = EFS_NvitemWrite(NV_GLO_EPHEMERIS_PARAM, sizeof(GE2_GLO_EPHEMERIS_FLASH), (uint8 *)(&ge2_gloeph_data), 1);
	GE2GPS_LOGD("save glo eph ,status=%d",status);
#endif
	
}

/*****************************************************************************/
//  Description:    GET_UBITS_TMP
//  Author:         
//  Note:   
/*****************************************************************************/
int GET_UBITS_TMP(unsigned int data, unsigned int pos, unsigned int len)
{
	unsigned int tempL = 0,tempR = 0;
	tempR = (data >> pos)&ONES(len);
	
	return tempR;
}

/*****************************************************************************/
//  Description:    gps_send_eph
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_send_eph(GpsState* s)
{
    TCmdData_t Pak;
    int i = 0,j = 0,k = 0;
    uint32 status = 0;

//g_GlonassAlmanac
    if(s->workmode == GPS_GLONASS)
    {
#ifdef GPS_MODE_GPS_GLONASS

        if(!ge2_savegloflash)
        {
	        SCI_MEMSET(&ge2_gloeph_data,0,sizeof(GE2_GLO_EPHEMERIS_FLASH));

		status = EFS_NvitemRead(NV_GLO_EPHEMERIS_PARAM, sizeof(GE2_GLO_EPHEMERIS_FLASH), (uint8 *)(&ge2_gloeph_data));    
		// If the initial values don't exist.
		if (NVERR_NOT_EXIST == status)
		{
			GREENEYE2_LOGD(("[GE2]%s:read glo flash fail",__FUNCTION__));
			return;
		}
		ge2_savegloflash = SCI_TRUE;
        }

        memset(&Pak,0,sizeof(TCmdData_t));	
        Pak.type = GNSS_LIBGPS_FLASH_TYPE;
        Pak.subType = GNSS_LIBGPS_FLASH_GLO_EPH1_SUBTYPE + s->requestephmode -1;
        Pak.length = sizeof(GE2_GLONASS_EPHEMERIS)*7+4;  //for glonass flag
        GREENEYE2_LOGD(("[GE2]glonass ephemeris[%d] length is %d",(s->requestephmode-1),Pak.length));
        memcpy(Pak.buff+4,(uint8 *)&(ge2_gloeph_data.gloEph[s->requestephmode -1]),sizeof(GE2_GLONASS_EPHEMERIS)*7);
        gps_adingData(&Pak); 
        SCI_Sleep(10);

	s->requestephmode += 1;
	if(s->requestephmode > 4)
		s->requestephmode = 0;
#endif

    }
    else
    {
#ifdef GPS_MODE_GPS_BD
	GREENEYE2_LOGD(("[GE2]update bds assist data"));
	if(!ge2_savebdflash)
	{
	        SCI_MEMSET(&ge2_bdeph_data,0,sizeof(GE2_BD_EPHEMERIS_FLASH));

		status = EFS_NvitemRead(NV_BD_EPHEMERIS_PARAM, sizeof(GE2_BD_EPHEMERIS_FLASH), (uint8 *)(&ge2_bdeph_data));    
		// If the initial values don't exist.
		if (NVERR_NOT_EXIST == status)
		{
			GREENEYE2_LOGD(("[GE2]%s:read bd flash fail",__FUNCTION__));
			return;
		}
		ge2_savebdflash = SCI_TRUE;
	}
	memset(&Pak,0,sizeof(TCmdData_t));	
	Pak.type = GNSS_LIBGPS_FLASH_TYPE;
	Pak.subType = GNSS_LIBGPS_FLASH_BD2_EPH1_SUBTYPE + s->requestephmode - 1;
	Pak.length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
	GREENEYE2_LOGD(("[GE2]bds ephemeris length[%d] is %d",(s->requestephmode - 1),Pak.length));
	memcpy(Pak.buff,(uint8 *)&(ge2_bdeph_data.bdEph[s->requestephmode - 1]),sizeof(GE2_BLOCK_EPHEMERIS_FLASH));
	gps_adingData(&Pak);
	SCI_Sleep(10);
	s->requestephmode += 1;
	if(s->requestephmode > 2)
		s->requestephmode = 0;
#endif
    }
}

/*****************************************************************************/
//  Description:    gps_send_gpseph
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_send_gpseph(GpsState* s)
{
    TCmdData_t Pak;
    int i = 0,j = 0,k = 0;
    int week,toe;
    long diff = 0;

//GPSEPH
    memset(&Pak,0,sizeof(TCmdData_t));

    Pak.length = 4;   //send gps eph agps mode

    /*
    GNSS_LIBGPS_FLASH_GPS_EPH1_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH2_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH3_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH4_SUBTYPE
    */
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 8; j++)
        {
            //D("said:flag-----%d,svid-----%d",tempEph.Ephemeris[j].flag,tempEph.Ephemeris[j].svid);
            if((ge2_eph_data.gpsEph[i].Ephemeris[j].flag & 0x1) && (ge2_eph_data.pvttype.PvtPkg.week > 0))
            {
                week = GET_UBITS_TMP(ge2_eph_data.gpsEph[i].Ephemeris[j].word[7],20,10);
                toe = GET_UBITS_TMP(ge2_eph_data.gpsEph[i].Ephemeris[j].word[10],14,16);
                diff = (ge2_eph_data.pvttype.PvtPkg.week - 1024 - week )*604800 + (ge2_eph_data.pvttype.PvtPkg.msCount/1000 - toe*16 + 16);	
                //D("update,week is %d,toe is %d,diff is %ld",week,toe,diff);
                if((diff < -14400) || (diff > 14400))
                {
                    GREENEYE2_LOGE(("[GE2]ephemeris is too old"));
                    break;
                }

                memcpy(Pak.buff+Pak.length,&(ge2_eph_data.gpsEph[i].Ephemeris[j]),sizeof(GE2_GPS_EPHEMERIS_PACKED));
                Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
                k++;
                if(k > 11)
                {
                    i = 5;
                    break;
                }
            }
        }
    }

    //GNSS_LIBGPS_FLASH_QZS_EPH_SUBTYPE
    for(j = 0; j < 8; j++)
    {
        //D("said:flag-----%d,svid-----%d",tempEph.Ephemeris[j].flag,tempEph.Ephemeris[j].svid);
        if((ge2_eph_data.qzsEph.Ephemeris[j].flag & 0x1) && (ge2_eph_data.pvttype.PvtPkg.week > 0))
        {
	        week = GET_UBITS_TMP(ge2_eph_data.qzsEph.Ephemeris[j].word[7],20,10);
	        toe = GET_UBITS_TMP(ge2_eph_data.qzsEph.Ephemeris[j].word[10],14,16);
	        diff = (ge2_eph_data.pvttype.PvtPkg.week - 1024 - week )*604800 + (ge2_eph_data.pvttype.PvtPkg.msCount/1000 - toe*16 + 16);	
	        //D("update,week is %d,toe is %d,diff is %ld",week,toe,diff);
	        if((diff < -14400) || (diff > 14400))
	        {
	            GREENEYE2_LOGE(("[GE2]ephemeris is too old"));
	            break;
	        }
	        if(k < 13)
	        {
                    memcpy(Pak.buff+Pak.length,&(ge2_eph_data.qzsEph.Ephemeris[j]),sizeof(GE2_GPS_EPHEMERIS_PACKED));
                    Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
		}
	        k++;
			
        }
    }
		
    GREENEYE2_LOGD(("[GE2]will be update gps eph num is %d,length = %d",k,Pak.length));	
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_GPSEPHEMERIS_SUBTYPE;
    gps_adingData(&Pak);	
    //SCI_Sleep(200);
}



/*****************************************************************************/
//  Description:    gps_send_eph_ge2
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_send_eph_ge2(GpsState* s)
{
    int i = 0,j = 0,k = 0;
    TCmdData_t Pak;
    int week,toe;
    long diff = 0;
    uint32  status = 0;

    GREENEYE2_LOGD(("[GE2]%s:send initial data,eph_size=%d",__FUNCTION__,sizeof(GE2_EPHEMERIS_FLASH)));

   if(!ge2_sendephdata)
   	return;

   //send eph data only once
   //ge2_sendephdata = SCI_FALSE;
   
    if(!ge2_saveflash)
    {
        SCI_MEMSET(&ge2_eph_data,0,sizeof(GE2_EPHEMERIS_FLASH));

	status = EFS_NvitemRead(NV_GPS_EPHEMERIS_PARAM, sizeof(GE2_EPHEMERIS_FLASH), (uint8 *)(&ge2_eph_data));    
	// If the initial values don't exist.
	if (NVERR_NOT_EXIST == status)
	{
		GREENEYE2_LOGD(("[GE2]%s:read flash fail",__FUNCTION__));
		return;
	}
	ge2_saveflash = SCI_TRUE;
    }


	if(0 == s->requestephmode)
	{
		gps_send_gpseph(s);
		s->requestephmode += 1;
	}
	else
	{
	        gps_send_eph(s);
	}

    return;
}


/*****************************************************************************/
//  Description:    gps_send_assist_ge2
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_send_assist_ge2(GpsState* s)
{
    int i = 0,j = 0,k = 0;
    TCmdData_t Pak;
    int week,toe;
    long diff = 0;
    uint32  status = 0;

    GREENEYE2_LOGD(("[GE2]%s:send initial data,eph_size=%d,ge2_saveflash=%d",__FUNCTION__,sizeof(GE2_EPHEMERIS_FLASH),ge2_saveflash));

    if(s->cmcc)
    {
   	GE2GPS_LOGD("cmcc test,not send assist data to ge2!");
   	return;
    }

    if(!ge2_saveflash)
    {
        SCI_MEMSET(&ge2_eph_data,0,sizeof(GE2_EPHEMERIS_FLASH));

	status = EFS_NvitemRead(NV_GPS_EPHEMERIS_PARAM, sizeof(GE2_EPHEMERIS_FLASH), (uint8 *)(&ge2_eph_data));    
	// If the initial values don't exist.
	if (NVERR_NOT_EXIST == status)
	{
		GREENEYE2_LOGD(("[GE2]%s:read flash fail",__FUNCTION__));
		return;
	}
	ge2_saveflash = SCI_TRUE;
    }

//SCI_PASSERT(0,("gps_send_assist_ge2,status=%d!",status));
    //PVTSTAUS
    memset(&Pak,0,sizeof(TCmdData_t));

    GREENEYE2_LOGD(("[GE2]send assist data to ge2"));
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_RCVSTATUS_SUBTYPE;
    Pak.length = sizeof(GE2_PVTMISC_FLASH);
    GREENEYE2_LOGD(("[GE2]location to ge2,lat=%d,lon=%d,length=%d",(int)(ge2_eph_data.pvttype.PvtPkg.ReceiverPos.lat),(int)(ge2_eph_data.pvttype.PvtPkg.ReceiverPos.lon),Pak.length));
    memcpy(Pak.buff,(uint8 *)&(ge2_eph_data.pvttype),sizeof(GE2_PVTMISC_FLASH));
    if((ge2_eph_data.pvttype.PvtPkg.ReceiverPos.lat != 0) || (ge2_eph_data.pvttype.PvtPkg.ReceiverPos.lon != 0))
    {
        GREENEYE2_LOGD(("[GE2]location is not null,send to gnss set"));
        gps_adingData(&Pak);
        SCI_Sleep(10);
    }

//GPSEPH
    memset(&Pak,0,sizeof(TCmdData_t));

    Pak.length = 4;   //send gps eph agps mode

    /*
    GNSS_LIBGPS_FLASH_GPS_EPH1_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH2_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH3_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH4_SUBTYPE
    */
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 8; j++)
        {
            //D("said:flag-----%d,svid-----%d",tempEph.Ephemeris[j].flag,tempEph.Ephemeris[j].svid);
            if((ge2_eph_data.gpsEph[i].Ephemeris[j].flag & 0x1) && (ge2_eph_data.pvttype.PvtPkg.week > 0))
            {
                week = GET_UBITS_TMP(ge2_eph_data.gpsEph[i].Ephemeris[j].word[7],20,10);
                toe = GET_UBITS_TMP(ge2_eph_data.gpsEph[i].Ephemeris[j].word[10],14,16);
                diff = (ge2_eph_data.pvttype.PvtPkg.week - 1024 - week )*604800 + (ge2_eph_data.pvttype.PvtPkg.msCount/1000 - toe*16 + 16);	
                //D("update,week is %d,toe is %d,diff is %ld",week,toe,diff);
                if((diff < -14400) || (diff > 14400))
                {
                    GREENEYE2_LOGE(("[GE2]ephemeris is too old"));
                    break;
                }

                memcpy(Pak.buff+Pak.length,&(ge2_eph_data.gpsEph[i].Ephemeris[j]),sizeof(GE2_GPS_EPHEMERIS_PACKED));
                Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
                k++;
                if(k > 11)
                {
                    i = 5;
                    break;
                }
            }
        }
    }

    //GNSS_LIBGPS_FLASH_QZS_EPH_SUBTYPE
    for(j = 0; j < 8; j++)
    {
        //D("said:flag-----%d,svid-----%d",tempEph.Ephemeris[j].flag,tempEph.Ephemeris[j].svid);
        if((ge2_eph_data.qzsEph.Ephemeris[j].flag & 0x1) && (ge2_eph_data.pvttype.PvtPkg.week > 0))
        {
	        week = GET_UBITS_TMP(ge2_eph_data.qzsEph.Ephemeris[j].word[7],20,10);
	        toe = GET_UBITS_TMP(ge2_eph_data.qzsEph.Ephemeris[j].word[10],14,16);
	        diff = (ge2_eph_data.pvttype.PvtPkg.week - 1024 - week )*604800 + (ge2_eph_data.pvttype.PvtPkg.msCount/1000 - toe*16 + 16);	
	        //D("update,week is %d,toe is %d,diff is %ld",week,toe,diff);
	        if((diff < -14400) || (diff > 14400))
	        {
	            GREENEYE2_LOGE(("[GE2]ephemeris is too old"));
	            break;
	        }
	        if(k < 13)
	        {
                    memcpy(Pak.buff+Pak.length,&(ge2_eph_data.qzsEph.Ephemeris[j]),sizeof(GE2_GPS_EPHEMERIS_PACKED));
                    Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
		}
	        k++;
			
        }
    }
		
    GREENEYE2_LOGD(("[GE2]will be update gps eph num is %d,length = %d",k,Pak.length));	
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_GPSEPHEMERIS_SUBTYPE;
    gps_adingData(&Pak);	
    SCI_Sleep(10);

//IONOUTC	
    memset(&Pak,0,sizeof(TCmdData_t));	
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_IONOUTC_SUBTYPE;
    Pak.length = sizeof(GE2_IONOUTC_PARAM_FLASH);
    GREENEYE2_LOGD(("[GE2]begin send almcpy data,length=%d",Pak.length));
    memcpy(Pak.buff,(uint8 *)&(ge2_eph_data.ionoutc),sizeof(GE2_IONOUTC_PARAM_FLASH));
    gps_adingData(&Pak);    

//GPSALM	
    memset(&Pak,0,sizeof(TCmdData_t));	
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_GPSALMANAC_SUBTYPE;
    Pak.length = sizeof(GE2_BLOCK_GPS_ALMANAC_FLASH)+4;
    GREENEYE2_LOGD(("[GE2]begin send almanac data,length=%d",Pak.length));
    memcpy(Pak.buff+4,(uint8 *)&(ge2_eph_data.gpsAlmanac),sizeof(GE2_BLOCK_GPS_ALMANAC_FLASH));
    gps_adingData(&Pak); 
    SCI_Sleep(10);

//GPSALMCOM
    memset(&Pak,0,sizeof(TCmdData_t));	
    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
    Pak.subType = GNSS_LIBGPS_FLASH_ALMCPY_COMM_SUBTYPE;
    Pak.length = sizeof(GE2_BLOCK_GPS_ALMCOM_FLASH);
    GREENEYE2_LOGD(("[GE2]begin send almcomm data,length=%d",Pak.length));
    memcpy(Pak.buff,(uint8 *)&(ge2_eph_data.gpsAlmcom),sizeof(GE2_BLOCK_GPS_ALMCOM_FLASH));
    gps_adingData(&Pak); 
    SCI_Sleep(10);

//SEQLSQ
    memset(&Pak,0,sizeof(TCmdData_t));	
    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
    Pak.subType = GNSS_LIBGPS_FLASH_PVT_SEQLSQ_SUBTYPE;
    Pak.length = sizeof(GE2_SEQLSQ_PARAM_FLASH);
    GREENEYE2_LOGD(("[GE2]begin send seqlsq data,length=%d",Pak.length));
    memcpy(Pak.buff,(uint8 *)&(ge2_eph_data.seqlsq),sizeof(GE2_SEQLSQ_PARAM_FLASH));
    gps_adingData(&Pak); 
    SCI_Sleep(10);

//BLUNDER_INFO
    memset(&Pak,0,sizeof(TCmdData_t));	
    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
    Pak.subType = GNSS_LIBGPS_FLASH_PVT_BLUNDER_SUBTYPE;
    Pak.length = sizeof(GE2_BLUNDER_INFO_FLASH);
    GREENEYE2_LOGD(("[GE2]begin send blunderinfo data,length=%d",Pak.length));
    memcpy(Pak.buff,(uint8 *)&(ge2_eph_data.blunder),sizeof(GE2_BLUNDER_INFO_FLASH));
    gps_adingData(&Pak); 
    SCI_Sleep(10);

//GPSALMCPY
    for(i = 0; i < 6; i++)
    {
	    memset(&Pak,0,sizeof(TCmdData_t));	
	    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
	    Pak.subType = GNSS_LIBGPS_FLASH_GPS_ALM_CPY1_SUBTYPE + i;
	    Pak.length = sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH);
            GREENEYE2_LOGD(("[GE2]begin send almcpy data,length[%d]=%d",i,Pak.length));
            memcpy(Pak.buff,(uint8 *)&(ge2_eph_data.gpsAmcopy[i]),sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH));
            gps_adingData(&Pak); 
            SCI_Sleep(10);
    }

//g_GlonassAlmanac
    if(s->workmode == GPS_GLONASS)
    {
#ifdef GPS_MODE_GPS_GLONASS
	if(!ge2_savegloflash)
	{
		status = EFS_NvitemRead(NV_GLO_EPHEMERIS_PARAM, sizeof(GE2_GLO_EPHEMERIS_FLASH), (uint8 *)(&ge2_gloeph_data));    
		// If the initial values don't exist.
		if (NVERR_NOT_EXIST == status)
		{
			GREENEYE2_LOGD(("[GE2]%s:read glo flash fail",__FUNCTION__));
			return;
		}
		ge2_savegloflash = SCI_TRUE;
	}

        memset(&Pak,0,sizeof(TCmdData_t));	
        Pak.type = GNSS_LIBGPS_FLASH_TYPE;
        Pak.subType = GNSS_LIBGPS_FLASH_GLO_ALM1_SUBTYPE;
        Pak.length = sizeof(GE2_GLONASS_ALMANAC)*14;
        GREENEYE2_LOGD(("[GE2]glonass almanac1 lenth is %d",Pak.length));
        memcpy(Pak.buff,(uint8 *)&(ge2_gloeph_data.gloAlmanac[0]),sizeof(GE2_GLONASS_ALMANAC)*14);
        gps_adingData(&Pak); 
        SCI_Sleep(10);

        memset(&Pak,0,sizeof(TCmdData_t));	
        Pak.type = GNSS_LIBGPS_FLASH_TYPE;
        Pak.subType = GNSS_LIBGPS_FLASH_GLO_ALM2_SUBTYPE;
        Pak.length = sizeof(GE2_GLONASS_ALMANAC)*14;
        GREENEYE2_LOGD(("[GE2]glonass almanac2 lenth is %d",Pak.length));
        memcpy(Pak.buff,(uint8 *)&(ge2_gloeph_data.gloAlmanac[1]),sizeof(GE2_GLONASS_ALMANAC)*14);
        gps_adingData(&Pak); 
        SCI_Sleep(10);

    //g_GlonassEphemeris
        memset(&Pak,0,sizeof(TCmdData_t));	
        Pak.type = GNSS_LIBGPS_FLASH_TYPE;
        Pak.subType = GNSS_LIBGPS_FLASH_GLO_EPH1_SUBTYPE;
        Pak.length = sizeof(GE2_GLONASS_EPHEMERIS)*7+4;  //for glonass flag
        GREENEYE2_LOGD(("[GE2]glonass ephemeris1 length is %d",Pak.length));
        memcpy(Pak.buff+4,(uint8 *)&(ge2_gloeph_data.gloEph[0]),sizeof(GE2_GLONASS_EPHEMERIS)*7);
        gps_adingData(&Pak); 
        SCI_Sleep(10);

        memset(&Pak,0,sizeof(TCmdData_t));	
        Pak.type = GNSS_LIBGPS_FLASH_TYPE;
        Pak.subType = GNSS_LIBGPS_FLASH_GLO_EPH2_SUBTYPE;	
        Pak.length = sizeof(GE2_GLONASS_EPHEMERIS)*7+4;
	GREENEYE2_LOGD(("[GE2]glonass ephemeris2 length is %d",Pak.length));
        memcpy(Pak.buff+4,(uint8 *)&(ge2_gloeph_data.gloEph[1]),sizeof(GE2_GLONASS_EPHEMERIS)*7);
        gps_adingData(&Pak); 
        SCI_Sleep(10);

        memset(&Pak,0,sizeof(TCmdData_t));	
        Pak.type = GNSS_LIBGPS_FLASH_TYPE;
        Pak.subType = GNSS_LIBGPS_FLASH_GLO_EPH3_SUBTYPE;
        Pak.length = sizeof(GE2_GLONASS_EPHEMERIS)*7+4;
	GREENEYE2_LOGD(("[GE2]glonass ephemeris3 length is %d",Pak.length));
        memcpy(Pak.buff+4,(uint8 *)&(ge2_gloeph_data.gloEph[2]),sizeof(GE2_GLONASS_EPHEMERIS)*7);
        gps_adingData(&Pak); 
        SCI_Sleep(10);

        memset(&Pak,0,sizeof(TCmdData_t));	
        Pak.type = GNSS_LIBGPS_FLASH_TYPE;
        Pak.subType = GNSS_LIBGPS_FLASH_GLO_EPH4_SUBTYPE;
        Pak.length = sizeof(GE2_GLONASS_EPHEMERIS)*7+4;
	GREENEYE2_LOGD(("[GE2]glonass ephemeris4 length is %d",Pak.length));
        memcpy(Pak.buff+4,(uint8 *)&(ge2_gloeph_data.gloEph[3]),sizeof(GE2_GLONASS_EPHEMERIS)*7);
        gps_adingData(&Pak); 
        SCI_Sleep(50);
#endif
    }
    else
    {
#ifdef GPS_MODE_GPS_BD
        GREENEYE2_LOGD(("[GE2]update bds assist data"));
	if(!ge2_savebdflash)
	{
		status = EFS_NvitemRead(NV_BD_EPHEMERIS_PARAM, sizeof(GE2_BD_EPHEMERIS_FLASH), (uint8 *)(&ge2_bdeph_data));    
		// If the initial values don't exist.
		if (NVERR_NOT_EXIST == status)
		{
			GREENEYE2_LOGD(("[GE2]%s:read bd flash fail",__FUNCTION__));
			return;
		}
		ge2_savebdflash = SCI_TRUE;
	}

        for(i = 0; i < 2; i++)
        {
            memset(&Pak,0,sizeof(TCmdData_t));	
            Pak.type = GNSS_LIBGPS_FLASH_TYPE;
            Pak.subType = GNSS_LIBGPS_FLASH_BD2_EPH1_SUBTYPE + i;
            Pak.length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
	    GREENEYE2_LOGD(("[GE2]bds ephemeris length[%d] is %d",i,Pak.length));
            memcpy(Pak.buff,(uint8 *)&(ge2_bdeph_data.bdEph[i]),sizeof(GE2_BLOCK_EPHEMERIS_FLASH));
            gps_adingData(&Pak);
            SCI_Sleep(10);
        }

        memset(&Pak,0,sizeof(TCmdData_t));	
        Pak.type = GNSS_LIBGPS_FLASH_TYPE;
        Pak.subType = GNSS_LIBGPS_FLASH_BD2_ALM_SUBTYPE;
        Pak.length = sizeof(GE2_BLOCK_BD2_ALMANAC_FLASH);
	GREENEYE2_LOGD(("[GE2]bds almanac length is %d",Pak.length));
        memcpy(Pak.buff,(uint8 *)&(ge2_bdeph_data.bdAlmanac),sizeof(GE2_BLOCK_BD2_ALMANAC_FLASH));
        gps_adingData(&Pak); 
        SCI_Sleep(10); 
#endif
    }

    return;
}
#else

/*****************************************************************************/
//  Description:    gps_saveFlashCfg
//  Author:         
//  Note:   
/*****************************************************************************/
static void gps_saveFlashCfg(TCmdData_t * pPacket)
{
    //GpsState* s = _gps_state;
    uint32 status = 0;
    BOOLEAN flashcfg = SCI_TRUE;
    uint16 eph_itemid = 0;

    if(NULL == pPacket)
    {
        return;
    }

    if(pPacket->subType < GNSS_LIBGPS_FLASH_MAX_LEN_SUBTYPE)
    {
        GREENEYE2_LOGD(("[GE2]%s:begin save flash data,subtype is %d,length is %d",__FUNCTION__,pPacket->subType,pPacket->length));

        switch(pPacket->subType)
        {
            case GNSS_LIBGPS_FLASH_PvtMisc_SUBTYPE:  //ok
                 eph_itemid = NV_GPS_PVTMISC_PARAM;
                break;
             case GNSS_LIBGPS_FLASH_GPS_EPH1_SUBTYPE:
                 eph_itemid = NV_GPS_GPSEPH1_PARAM;
                break;
            case GNSS_LIBGPS_FLASH_GPS_EPH2_SUBTYPE:
		eph_itemid = NV_GPS_GPSEPH2_PARAM;
                break;
            case GNSS_LIBGPS_FLASH_GPS_EPH3_SUBTYPE:
		eph_itemid = NV_GPS_GPSEPH3_PARAM;                
		break;
            case GNSS_LIBGPS_FLASH_GPS_EPH4_SUBTYPE:
		eph_itemid = NV_GPS_GPSEPH4_PARAM;                
		break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_SUBTYPE:
		eph_itemid = NV_GPS_GPSALM_PARAM;                
		break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY1_SUBTYPE:
		eph_itemid = NV_GPS_GPSCPY1_PARAM;                
		break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY2_SUBTYPE:
		eph_itemid = NV_GPS_GPSCPY2_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY3_SUBTYPE:
		eph_itemid = NV_GPS_GPSCPY3_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY4_SUBTYPE:
		eph_itemid = NV_GPS_GPSCPY4_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY5_SUBTYPE:
		eph_itemid = NV_GPS_GPSCPY5_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GPS_ALM_CPY6_SUBTYPE:
		eph_itemid = NV_GPS_GPSCPY5_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_QZS_EPH_SUBTYPE:
		eph_itemid = NV_GPS_QZSEPH_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_QZS_ALM_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
             case GNSS_LIBGPS_FLASH_BD2_EPH1_SUBTYPE:
		eph_itemid = NV_GPS_BDEPH1_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_BD2_EPH2_SUBTYPE:
		eph_itemid = NV_GPS_BDEPH2_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_SUBTYPE:
		eph_itemid = NV_GPS_BDALM_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_CPY1_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_CPY2_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_CPY3_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_BD2_ALM_CPY4_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_GLO_EPH1_SUBTYPE:
		eph_itemid = NV_GPS_GLOEPH1_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GLO_EPH2_SUBTYPE:
		eph_itemid = NV_GPS_GLOEPH2_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GLO_EPH3_SUBTYPE:
		eph_itemid = NV_GPS_GLOEPH3_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GLO_EPH4_SUBTYPE:
		eph_itemid = NV_GPS_GLOEPH4_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GLO_ALM1_SUBTYPE:
		eph_itemid = NV_GPS_GLOALM1_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_GLO_ALM2_SUBTYPE:
		eph_itemid = NV_GPS_GLOALM2_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_IONO_UTC_SUBTYPE:
		eph_itemid = NV_GPS_IONO_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_SYS_CFG_SUBTYPE:
                flashcfg = SCI_FALSE;
                break;
            case GNSS_LIBGPS_FLASH_ALMCPY_COMM_SUBTYPE:
		eph_itemid = NV_GPS_ALMCPY_PARAM;                
                break;
           case GNSS_LIBGPS_FLASH_PVT_SEQLSQ_SUBTYPE:
		eph_itemid = NV_GPS_SEQLSQ_PARAM;                
                break;
            case GNSS_LIBGPS_FLASH_PVT_BLUNDER_SUBTYPE:
		eph_itemid = NV_GPS_BLUNDER_PARAM;                
                break;
            default:
		flashcfg = SCI_FALSE;
                break;
        }
        if(flashcfg)
        {
            status = EFS_NvitemWrite(eph_itemid, pPacket->length, pPacket->buff, 1);
            if(NVERR_NONE != status)
                SCI_PASSERT(0,("[GE2]%s:gps_saveFlashCfg fail,status=%d",__FUNCTION__,status));
        }

     }
    else
    {
        GREENEYE2_LOGE(("[GE2]the save flash cfg index error\r\n"));
    }
	TM_SendTestPointRequest(status,925941);
	//flag++;

	//if(flag >= 32)
		//SCI_PASSERT(0,("SAVE EPH FLASH FAIL!"));
}

/*****************************************************************************/
//  Description:    GET_UBITS_TMP
//  Author:         
//  Note:   
/*****************************************************************************/
int GET_UBITS_TMP(unsigned int data, unsigned int pos, unsigned int len)
{
	unsigned int tempL = 0,tempR = 0;
	tempR = (data >> pos)&ONES(len);
	
	return tempR;
}

/*****************************************************************************/
//  Description:    gps_send_eph
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_send_eph(GpsState* s)
{
    TCmdData_t Pak;
    uint32  status = 0x0;
    uint8 *eph_ptr = PNULL;
    uint32 eph_size = 2048;
    uint8 *temp_ptr = PNULL;

    eph_ptr = SCI_ALLOC(eph_size);
    if((NULL == eph_ptr))
    {
	GE2GPS_LOGD("gps eph alloc fail!");
	return;
    }

//g_GlonassAlmanac
    if(s->workmode == GPS_GLONASS)
    {
	GREENEYE2_LOGD(("[GE2]%s:send glonass data to ge2",__FUNCTION__));

	//g_GlonassEphemeris	  
	memset(&Pak,0,sizeof(TCmdData_t));	
	memset(eph_ptr,0,eph_size);
	status = EFS_NvitemRead((NV_GPS_GLOEPH1_PARAM + s->requestephmode - 1), (sizeof(GE2_GLONASS_EPHEMERIS) * 7), eph_ptr);    
	if(NVERR_NONE == status)
	{  
		Pak.type = GNSS_LIBGPS_FLASH_TYPE;
		Pak.subType = GNSS_LIBGPS_FLASH_GLO_EPH1_SUBTYPE + s->requestephmode - 1;
		Pak.length = sizeof(GE2_GLONASS_EPHEMERIS)*7+4;  //for glonass flag
		GREENEYE2_LOGD(("[GE2]%s:glonass ephemeris[%d] length is %d",__FUNCTION__,s->requestephmode - 1,Pak.length));
		memcpy(Pak.buff+4,eph_ptr,(sizeof(GE2_GLONASS_EPHEMERIS) * 7));
		gps_adingData(&Pak); 
		SCI_Sleep(10);
	}
	else
	{
	    GREENEYE2_LOGD(("[GE2]%s:read glonass eph[%d] fail",__FUNCTION__,s->requestephmode - 1));
	}
	s->requestephmode += 1;

	if(s->requestephmode >  4)
		s->requestephmode = 0;
    }
    else
    {
        GREENEYE2_LOGD(("[GE2]%s:send bds data to ge2",__FUNCTION__));
	memset(&Pak,0,sizeof(TCmdData_t));	
	memset(eph_ptr,0,eph_size);
	status = EFS_NvitemRead((NV_GPS_BDEPH1_PARAM + s->requestephmode - 1), sizeof(GE2_BLOCK_EPHEMERIS_FLASH), eph_ptr);    
	if(NVERR_NONE == status)
	{
	            Pak.type = GNSS_LIBGPS_FLASH_TYPE;
	            Pak.subType = GNSS_LIBGPS_FLASH_BD2_EPH1_SUBTYPE + s->requestephmode - 1;
	            Pak.length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
	            GREENEYE2_LOGD(("[GE2]%s:bd eph[%d] length is %d",__FUNCTION__,s->requestephmode - 1,Pak.length));
		    memcpy(Pak.buff,eph_ptr,sizeof(GE2_BLOCK_EPHEMERIS_FLASH));
		    gps_adingData(&Pak); 
		    SCI_Sleep(10);
	}
	else
	{
	        GREENEYE2_LOGD(("[GE2]%s:read bd eph[%d] fail",__FUNCTION__,s->requestephmode - 1));
	}
	s->requestephmode += 1;

	if(s->requestephmode > 2)
		s->requestephmode = 0;
    }

   SCI_FREE(eph_ptr);
}

/*****************************************************************************/
//  Description:    gps_send_gpseph
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_send_gpseph(GpsState* s)
{
    TCmdData_t Pak;
    int i = 0,j = 0,k = 0;
    int week,toe;
    long diff = 0;
    uint32  status = 0x0;
    uint8 *eph_ptr = PNULL;
    uint32 eph_size = 2048;
    uint8 *temp_ptr = PNULL;
    GE2_PVTMISC_FLASH *pvttype = PNULL;
    uint16 pvtsize = sizeof(GE2_PVTMISC_FLASH);

    eph_ptr = SCI_ALLOC(eph_size);
    pvttype = SCI_ALLOC(pvtsize);
    if((NULL == eph_ptr) || (NULL == pvttype))
    {
	GE2GPS_LOGD("gps eph alloc fail!");
	return;
    }

    GREENEYE2_LOGD(("[GE2]%s:send gps data to ge2",__FUNCTION__));

    //PVTSTAUS
    memset(&Pak,0,sizeof(TCmdData_t));
    memset(pvttype,0,pvtsize);
    status = EFS_NvitemRead(NV_GPS_PVTMISC_PARAM, sizeof(GE2_PVTMISC_FLASH), pvttype);    
    if (NVERR_NOT_EXIST == status)
    {
	GREENEYE2_LOGD(("[GE2]%s:read pvtmisc flash fail",__FUNCTION__));
	return;
    }
    GREENEYE2_LOGD(("[GE2]location to ge2,lat=%f,lon=%f,length=%d",pvttype->PvtPkg.ReceiverPos.lat,pvttype->PvtPkg.ReceiverPos.lon,Pak.length));
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_RCVSTATUS_SUBTYPE;
    Pak.length = sizeof(GE2_PVTMISC_FLASH);
    memcpy(Pak.buff,pvttype,pvtsize);
    if((pvttype->PvtPkg.ReceiverPos.lat != 0) || (pvttype->PvtPkg.ReceiverPos.lon != 0))
    {
        GREENEYE2_LOGD(("[GE2]%s:location is not null,send to gnss set",__FUNCTION__));
        //gps_adingData(&Pak);
        //SCI_Sleep(10);
    }

//GPSEPH
    memset(&Pak,0,sizeof(TCmdData_t));
    Pak.length = 4;   //send gps eph agps mode

    /*
    GNSS_LIBGPS_FLASH_GPS_EPH1_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH2_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH3_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH4_SUBTYPE
    */
    for(i = 0; i < 4; i++)
    {
        memset(eph_ptr,0,eph_size);
        status = EFS_NvitemRead((NV_GPS_GPSEPH1_PARAM + i), sizeof(GE2_BLOCK_EPHEMERIS_FLASH), eph_ptr);    
	if(NVERR_NONE == status)
	{
	        for(j = 0; j < 8; j++)
	        {
	            //D("said:flag-----%d,svid-----%d",tempEph.Ephemeris[j].flag,tempEph.Ephemeris[j].svid);
	            if((((GE2_BLOCK_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].flag & 0x1) && (pvttype->PvtPkg.week > 0))
	            {
	                week = GET_UBITS_TMP(((GE2_BLOCK_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].word[7],20,10);
	                toe = GET_UBITS_TMP(((GE2_BLOCK_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].word[10],14,16);
	                diff = (pvttype->PvtPkg.week - 1024 - week )*604800 + (pvttype->PvtPkg.msCount/1000 - toe*16 + 16);	
	                //D("update,week is %d,toe is %d,diff is %ld",week,toe,diff);
	                if((diff < -14400) || (diff > 14400))
	                {
	                    GREENEYE2_LOGE(("[GE2]%s:eph[%d] ephemeris[%d] is too old",__FUNCTION__,i,j));
	                    break;
	                }

	                memcpy(Pak.buff+Pak.length,&(((GE2_BLOCK_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j]),sizeof(GE2_GPS_EPHEMERIS_PACKED));
	                Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
	                k++;
	                if(k > 11)
	                {
	                    i = 5;
	                    break;
	                }
	            }
	        }
	}
	else
    	{
		GREENEYE2_LOGD(("[GE2]%s:read gps eph[%d] flash fail",__FUNCTION__,i));
    	}
    }

    //GNSS_LIBGPS_FLASH_QZS_EPH_SUBTYPE
    memset(eph_ptr,0,eph_size);
    status = EFS_NvitemRead((NV_GPS_QZSEPH_PARAM + i), sizeof(GE2_QZSS_EPHEMERIS_FLASH), eph_ptr);    
    if(NVERR_NONE == status)
    {
	    for(j = 0; j < 8; j++)
	    {
	        //D("said:flag-----%d,svid-----%d",tempEph.Ephemeris[j].flag,tempEph.Ephemeris[j].svid);
	        if((((GE2_QZSS_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].flag & 0x1) && (pvttype->PvtPkg.week > 0))
	        {
		        week = GET_UBITS_TMP(((GE2_QZSS_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].word[7],20,10);
		        toe = GET_UBITS_TMP(((GE2_QZSS_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].word[10],14,16);
		        diff = (pvttype->PvtPkg.week - 1024 - week )*604800 + (pvttype->PvtPkg.msCount/1000 - toe*16 + 16);	
		        //D("update,week is %d,toe is %d,diff is %ld",week,toe,diff);
		        if((diff < -14400) || (diff > 14400))
		        {
		            GREENEYE2_LOGE(("[GE2]%s:ephemeris is too old",__FUNCTION__));
		            break;
		        }
		        if(k < 13)
		        {
	                    memcpy(Pak.buff+Pak.length,&(((GE2_QZSS_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j]),sizeof(GE2_GPS_EPHEMERIS_PACKED));
	                    Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
			}
		        k++;
				
	        }
	    }
   }
   else
   {
        GREENEYE2_LOGD(("[GE2]%s:read qzs eph flash fail",__FUNCTION__));
   }
		
    GREENEYE2_LOGD(("[GE2]%s:will be update gps eph num is %d,length = %d",__FUNCTION__,k,Pak.length));	
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_GPSEPHEMERIS_SUBTYPE;
    gps_adingData(&Pak);
    //SCI_Sleep(200);

    SCI_FREE(pvttype);
    SCI_FREE(eph_ptr);
}


/*****************************************************************************/
//  Description:    gps_send_eph_ge2
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_send_eph_ge2(GpsState* s)
{
    if(0 == s->requestephmode)
    {
	 gps_send_gpseph(s);
	 s->requestephmode += 1;
    }
    else
    {
	 gps_send_eph(s);
    }
    return;
}

/*****************************************************************************/
//  Description:    gps_send_assist_ge2
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_send_assist_ge2(GpsState* s)
{
    int i = 0,j = 0,k = 0;
    TCmdData_t Pak;
    int week,toe;
    long diff = 0;
    uint32  status = 0x0;
    uint8 *eph_ptr = PNULL;
    uint32 eph_size = 2048;
    uint8 *temp_ptr = PNULL;
    GE2_PVTMISC_FLASH *pvttype = PNULL;
    uint16 pvtsize = sizeof(GE2_PVTMISC_FLASH);

    eph_ptr = SCI_ALLOC(eph_size);
    pvttype = SCI_ALLOC(pvtsize);
    if((NULL == eph_ptr) || (NULL == pvttype))
    {
	GE2GPS_LOGD("gps eph alloc fail!");
	return;
    }

    GREENEYE2_LOGD(("[GE2]%s:send assist data to ge2",__FUNCTION__));

    //PVTSTAUS
    memset(&Pak,0,sizeof(TCmdData_t));
    memset(pvttype,0,pvtsize);
    status = EFS_NvitemRead(NV_GPS_PVTMISC_PARAM, sizeof(GE2_PVTMISC_FLASH), pvttype);    
    if (NVERR_NOT_EXIST == status)
    {
	GREENEYE2_LOGD(("[GE2]%s:read pvtmisc flash fail",__FUNCTION__));
	SCI_FREE(pvttype);
   	SCI_FREE(eph_ptr);
	return;
    }
    GREENEYE2_LOGD(("[GE2]location to ge2,lat=%f,lon=%f,length=%d",pvttype->PvtPkg.ReceiverPos.lat,pvttype->PvtPkg.ReceiverPos.lon,Pak.length));
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_RCVSTATUS_SUBTYPE;
    Pak.length = sizeof(GE2_PVTMISC_FLASH);
    memcpy(Pak.buff,pvttype,pvtsize);
    if((pvttype->PvtPkg.ReceiverPos.lat != 0) || (pvttype->PvtPkg.ReceiverPos.lon != 0))
    {
        GREENEYE2_LOGD(("[GE2]%s:location is not null,send to gnss set",__FUNCTION__));
        gps_adingData(&Pak);
        SCI_Sleep(10);
    }

//GPSEPH
    memset(&Pak,0,sizeof(TCmdData_t));
    Pak.length = 4;   //send gps eph agps mode

    /*
    GNSS_LIBGPS_FLASH_GPS_EPH1_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH2_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH3_SUBTYPE
    GNSS_LIBGPS_FLASH_GPS_EPH4_SUBTYPE
    */
    for(i = 0; i < 4; i++)
    {
        memset(eph_ptr,0,eph_size);
        status = EFS_NvitemRead((NV_GPS_GPSEPH1_PARAM + i), sizeof(GE2_BLOCK_EPHEMERIS_FLASH), eph_ptr);    
	if(NVERR_NONE == status)
	{
	        for(j = 0; j < 8; j++)
	        {
	            //D("said:flag-----%d,svid-----%d",tempEph.Ephemeris[j].flag,tempEph.Ephemeris[j].svid);
	            if((((GE2_BLOCK_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].flag & 0x1) && (pvttype->PvtPkg.week > 0))
	            {
	                week = GET_UBITS_TMP(((GE2_BLOCK_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].word[7],20,10);
	                toe = GET_UBITS_TMP(((GE2_BLOCK_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].word[10],14,16);
	                diff = (pvttype->PvtPkg.week - 1024 - week )*604800 + (pvttype->PvtPkg.msCount/1000 - toe*16 + 16);	
	                //D("update,week is %d,toe is %d,diff is %ld",week,toe,diff);
	                if((diff < -14400) || (diff > 14400))
	                {
	                    GREENEYE2_LOGE(("[GE2]%s:ephemeris is too old",__FUNCTION__));
	                    break;
	                }

	                memcpy(Pak.buff+Pak.length,&(((GE2_BLOCK_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j]),sizeof(GE2_GPS_EPHEMERIS_PACKED));
	                Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
	                k++;
	                if(k > 11)
	                {
	                    i = 5;
	                    break;
	                }
	            }
	        }
	}
	else
    	{
		GREENEYE2_LOGD(("[GE2]%s:read gps eph[%d] flash fail",__FUNCTION__,i));
    	}
    }

    //GNSS_LIBGPS_FLASH_QZS_EPH_SUBTYPE
    memset(eph_ptr,0,eph_size);
    status = EFS_NvitemRead((NV_GPS_QZSEPH_PARAM + i), sizeof(GE2_QZSS_EPHEMERIS_FLASH), eph_ptr);    
    if(NVERR_NONE == status)
    {
	    for(j = 0; j < 8; j++)
	    {
	        //D("said:flag-----%d,svid-----%d",tempEph.Ephemeris[j].flag,tempEph.Ephemeris[j].svid);
	        if((((GE2_QZSS_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].flag & 0x1) && (pvttype->PvtPkg.week > 0))
	        {
		        week = GET_UBITS_TMP(((GE2_QZSS_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].word[7],20,10);
		        toe = GET_UBITS_TMP(((GE2_QZSS_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j].word[10],14,16);
		        diff = (pvttype->PvtPkg.week - 1024 - week )*604800 + (pvttype->PvtPkg.msCount/1000 - toe*16 + 16);	
		        //D("update,week is %d,toe is %d,diff is %ld",week,toe,diff);
		        if((diff < -14400) || (diff > 14400))
		        {
		            GREENEYE2_LOGE(("[GE2]%s:ephemeris is too old",__FUNCTION__));
		            break;
		        }
		        if(k < 13)
		        {
	                    memcpy(Pak.buff+Pak.length,&(((GE2_QZSS_EPHEMERIS_FLASH *)eph_ptr)->Ephemeris[j]),sizeof(GE2_GPS_EPHEMERIS_PACKED));
	                    Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
			}
		        k++;
				
	        }
	    }
   }
   else
   {
        GREENEYE2_LOGD(("[GE2]%s:read qzs eph flash fail",__FUNCTION__));
   }
		
    GREENEYE2_LOGD(("[GE2]%s:will be update gps eph num is %d,length = %d",__FUNCTION__,k,Pak.length));	
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_GPSEPHEMERIS_SUBTYPE;
    gps_adingData(&Pak);	
    SCI_Sleep(10);

//IONOUTC	
    memset(&Pak,0,sizeof(TCmdData_t));	
    memset(eph_ptr,0,eph_size);
    status = EFS_NvitemRead(NV_GPS_IONO_PARAM, sizeof(GE2_IONOUTC_PARAM_FLASH), eph_ptr);    
    if(NVERR_NONE == status)
    {
	    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
	    Pak.subType = GNSS_LIBGPS_AIDING_IONOUTC_SUBTYPE;
	    Pak.length = sizeof(GE2_IONOUTC_PARAM_FLASH);
	    GREENEYE2_LOGD(("[GE2]%s:begin send almcpy data,length=%d",__FUNCTION__,Pak.length));
	    memcpy(Pak.buff,eph_ptr,sizeof(GE2_IONOUTC_PARAM_FLASH));
	    gps_adingData(&Pak);
    }
   else
   {
        GREENEYE2_LOGD(("[GE2]%s:read ionoutc flash fail",__FUNCTION__));
  }

//GPSALM
    memset(&Pak,0,sizeof(TCmdData_t));	
    memset(eph_ptr,0,eph_size);
    status = EFS_NvitemRead(NV_GPS_GPSALM_PARAM, sizeof(GE2_BLOCK_GPS_ALMANAC_FLASH), eph_ptr);    
    if(NVERR_NONE == status)
    {
	    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
	    Pak.subType = GNSS_LIBGPS_AIDING_GPSALMANAC_SUBTYPE;
	    Pak.length = sizeof(GE2_BLOCK_GPS_ALMANAC_FLASH)+4;
	    GREENEYE2_LOGD(("[GE2]%s:begin send almanac data,length=%d",__FUNCTION__,Pak.length));
	    memcpy(Pak.buff+4,eph_ptr,sizeof(GE2_BLOCK_GPS_ALMANAC_FLASH));
	    gps_adingData(&Pak); 
	    SCI_Sleep(10);
    }
   else
   {
        GREENEYE2_LOGD(("[GE2]%s:read gps almanac flash fail",__FUNCTION__));
  }

//GPSALMCOM
    memset(&Pak,0,sizeof(TCmdData_t));	
    memset(eph_ptr,0,eph_size);
    status = EFS_NvitemRead(NV_GPS_ALMCPY_PARAM, sizeof(GE2_BLOCK_GPS_ALMCOM_FLASH), eph_ptr);    
    if(NVERR_NONE == status)
    {
	    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
	    Pak.subType = GNSS_LIBGPS_FLASH_ALMCPY_COMM_SUBTYPE;
	    Pak.length = sizeof(GE2_BLOCK_GPS_ALMCOM_FLASH);
	    GREENEYE2_LOGD(("[GE2]%s:begin send almcomm data,length=%d",__FUNCTION__,Pak.length));
	    memcpy(Pak.buff,eph_ptr,sizeof(GE2_BLOCK_GPS_ALMCOM_FLASH));
	    gps_adingData(&Pak); 
	    SCI_Sleep(10);
    }
   else
   {
        GREENEYE2_LOGD(("[GE2]%s:read almcom flash fail",__FUNCTION__));
   }

//SEQLSQ
    memset(&Pak,0,sizeof(TCmdData_t));	
    memset(eph_ptr,0,eph_size);
    status = EFS_NvitemRead(NV_GPS_SEQLSQ_PARAM, sizeof(GE2_SEQLSQ_PARAM_FLASH), eph_ptr);    
    if(NVERR_NONE == status)
    {
	    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
	    Pak.subType = GNSS_LIBGPS_FLASH_PVT_SEQLSQ_SUBTYPE;
	    Pak.length = sizeof(GE2_SEQLSQ_PARAM_FLASH);
	    GREENEYE2_LOGD(("[GE2]%s:begin send seqlsq data,length=%d",__FUNCTION__,Pak.length));
	    memcpy(Pak.buff,eph_ptr,sizeof(GE2_SEQLSQ_PARAM_FLASH));
	    gps_adingData(&Pak); 
	    SCI_Sleep(10);
    }
   else
   {
        GREENEYE2_LOGD(("[GE2]%s:read seqlsq flash fail",__FUNCTION__));
   }


//BLUNDER_INFO
    memset(&Pak,0,sizeof(TCmdData_t));	
    memset(eph_ptr,0,eph_size);
    status = EFS_NvitemRead(NV_GPS_BLUNDER_PARAM, sizeof(GE2_BLUNDER_INFO_FLASH), eph_ptr);    
    if(NVERR_NONE == status)
    {
	    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
	    Pak.subType = GNSS_LIBGPS_FLASH_PVT_BLUNDER_SUBTYPE;
	    Pak.length = sizeof(GE2_BLUNDER_INFO_FLASH);
	    GREENEYE2_LOGD(("[GE2]%s:begin send blunderinfo data,length=%d",__FUNCTION__,Pak.length));
	    memcpy(Pak.buff,eph_ptr,sizeof(GE2_BLUNDER_INFO_FLASH));
	    gps_adingData(&Pak); 
	    SCI_Sleep(10);
    }
   else
   {
        GREENEYE2_LOGD(("[GE2]%s:read blunderinfo fail",__FUNCTION__));
   }

//GPSALMCPY
    for(i = 0; i < 6; i++)
    {
	    memset(&Pak,0,sizeof(TCmdData_t));	
	    memset(eph_ptr,0,eph_size);
	    status = EFS_NvitemRead((NV_GPS_GPSCPY1_PARAM + i), sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH), eph_ptr);    
	    if(NVERR_NONE == status)
	    {
		    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
		    Pak.subType = GNSS_LIBGPS_FLASH_GPS_ALM_CPY1_SUBTYPE + i;
		    Pak.length = sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH);
	            GREENEYE2_LOGD(("[GE2]%s:begin send almcpy data,length[%d]=%d",__FUNCTION__,i,Pak.length));
		    memcpy(Pak.buff,eph_ptr,sizeof(GE2_BLOCK_GPS_ALMCOPY_FLASH));
		    gps_adingData(&Pak); 
		    SCI_Sleep(10);
	    }
	   else
	   {
	        GREENEYE2_LOGD(("[GE2]%s:read gps almcpy[%d] fail",__FUNCTION__,i));
	   }
    }

//g_GlonassAlmanac
    if(s->workmode == GPS_GLONASS)
    {
      for(i = 0; i < 2; i++)
      {
	    memset(&Pak,0,sizeof(TCmdData_t));	
	    memset(eph_ptr,0,eph_size);
	    status = EFS_NvitemRead((NV_GPS_GLOALM1_PARAM + i), (sizeof(GE2_GLONASS_ALMANAC)*14), eph_ptr);    
	    if(NVERR_NONE == status)
	    {
   	            Pak.type = GNSS_LIBGPS_FLASH_TYPE;
   	            Pak.subType = GNSS_LIBGPS_FLASH_GLO_ALM1_SUBTYPE;
   	            Pak.length = sizeof(GE2_GLONASS_ALMANAC)*14;
	            GREENEYE2_LOGD(("[GE2]%s:begin send glonass almanac[%d] data,length=%d",__FUNCTION__,i,Pak.length));
		    memcpy(Pak.buff,eph_ptr,(sizeof(GE2_GLONASS_ALMANAC)*14));
		    gps_adingData(&Pak); 
		    SCI_Sleep(10);
	    }
	   else
	   {
	        GREENEYE2_LOGD(("[GE2]%s:read glonass almanac[%d] fail",__FUNCTION__,i));
	   }
      }

    //g_GlonassEphemeris	  
      for(i = 0; i < 4; i++)
      {
	    memset(&Pak,0,sizeof(TCmdData_t));	
	    memset(eph_ptr,0,eph_size);
	    status = EFS_NvitemRead((NV_GPS_GLOEPH1_PARAM + i), (sizeof(GE2_GLONASS_EPHEMERIS) * 7), eph_ptr);    
	    if(NVERR_NONE == status)
	    {
		     Pak.type = GNSS_LIBGPS_FLASH_TYPE;
		     Pak.subType = GNSS_LIBGPS_FLASH_GLO_EPH1_SUBTYPE + i;
		     Pak.length = sizeof(GE2_GLONASS_EPHEMERIS)*7+4;  //for glonass flag
                    GREENEYE2_LOGD(("[GE2]%s:glonass ephemeris[%d] length is %d",__FUNCTION__,i,Pak.length));
		    memcpy(Pak.buff+4,eph_ptr,(sizeof(GE2_GLONASS_EPHEMERIS) * 7));
		    gps_adingData(&Pak); 
		    SCI_Sleep(10);
	    }
	   else
	   {
	        GREENEYE2_LOGD(("[GE2]%s:read glonass eph[%d] fail",__FUNCTION__,i));
	   }
      }
    }
    else
    {
        GREENEYE2_LOGD(("[GE2]update bds assist data"));
        for(i = 0; i < 2; i++)
        {
	    memset(&Pak,0,sizeof(TCmdData_t));	
	    memset(eph_ptr,0,eph_size);
	    status = EFS_NvitemRead((NV_GPS_BDEPH1_PARAM + i), sizeof(GE2_BLOCK_EPHEMERIS_FLASH), eph_ptr);    
	    if(NVERR_NONE == status)
	    {
                    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
                    Pak.subType = GNSS_LIBGPS_FLASH_BD2_EPH1_SUBTYPE + i;
                    Pak.length = sizeof(GE2_BLOCK_EPHEMERIS_FLASH);
                    GREENEYE2_LOGD(("[GE2]%s:bd eph[%d] length is %d",__FUNCTION__,i,Pak.length));
		    memcpy(Pak.buff,eph_ptr,sizeof(GE2_BLOCK_EPHEMERIS_FLASH));
		    gps_adingData(&Pak); 
		    SCI_Sleep(10);
	    }
	   else
	   {
	        GREENEYE2_LOGD(("[GE2]%s:read bd eph[%d] fail",__FUNCTION__,i));
	   }

        }

        memset(&Pak,0,sizeof(TCmdData_t));	
        memset(eph_ptr,0,eph_size);
        status = EFS_NvitemRead(NV_GPS_BDALM_PARAM, sizeof(GE2_BLOCK_BD2_ALMANAC_FLASH), eph_ptr);    
        if(NVERR_NONE == status)
        {
	    Pak.type = GNSS_LIBGPS_FLASH_TYPE;
	    Pak.subType = GNSS_LIBGPS_FLASH_BD2_ALM_SUBTYPE;
	    Pak.length = sizeof(GE2_BLOCK_BD2_ALMANAC_FLASH);
	    GREENEYE2_LOGD(("[GE2]%s:bd almanac length is %d",__FUNCTION__,Pak.length));
	    memcpy(Pak.buff,eph_ptr,sizeof(GE2_BLOCK_BD2_ALMANAC_FLASH));
	    gps_adingData(&Pak); 
	    SCI_Sleep(10);
        }
        else
        {
           GREENEYE2_LOGD(("[GE2]%s:read bd almanac fail",__FUNCTION__));
        }	
    }

   SCI_FREE(pvttype);
   SCI_FREE(eph_ptr);

    return;
}
#endif

/*--------------------------------------------------------------------------
	Function:  gps_getParaData

	Description:
		it  parse lte and some other para set and request 
	Parameters:
	 void: 
	Return: SUCCESS:0 ; FAILED : others 
--------------------------------------------------------------------------*/

static void gps_getParaData(TCmdData_t *pPacket)
{
    GpsState*  s = _gps_state;
	static uint8 eph_count = 0;
	if(NULL == pPacket)
	{
        return;
	}
	switch(pPacket->subType)
	{
		case GNSS_LIBGPS_SET_LTE_SUBTYPE:
			if((s->lte_open == 1) && (s->lte_running == 1))
			{
				GREENEYE2_LOGD(("[GE2]lte request is received"));
				//parse_lte_request((char *)pPacket->buff,pPacket->length);
			}
                        else
                        {
                        	{
					GPS_GET_EPHDATA_T *signal = PNULL;

					SCI_CREATE_SIGNAL(
						signal,
						GPS_GET_EPHDATA_REQ,
						sizeof (GPS_GET_EPHDATA_T),
						SCI_IDENTIFY_THREAD());
						SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, GPS_GetInterfaceThreadId()); 
                        	}
                        }
			break;
		default:
			break;
    }
}

/*--------------------------------------------------------------------------
	Function:  gps_dispatch

	Description:
		it  parse all dispatch from ge2
	Parameters:
	 packet in uart: 
	Return: void 
--------------------------------------------------------------------------*/
void  gps_dispatch(TCmdData_t* pPacket)
{
    GpsState*  state = _gps_state; 
	if((NULL == pPacket) || (state->happenAssert))
	{
        return;
	}

	TM_SendTestPointRequest(pPacket->subType,9261004); 

	GE2GPS_LOGD("type=%d,subType=%d,length=%d",pPacket->type,pPacket->subType,pPacket->length);
    //ap recevie the data ,so it should set heartcount 0 whatever response of  hardcount 
    state->HeartCount = 0;
	TM_SendTestPointRequest(pPacket->type,924726); 
#ifdef GPS_MINI_LOG_SUPPORT
    if( pPacket->length &&(GNSS_LIBGPS_DATA_TYPE == pPacket->type)
        &&((GNSS_LIBGPS_LOG_SUBTYPE == pPacket->subType)||
          (GNSS_LIBGPS_ASSERT_SUBTYPE == pPacket->subType)||
          (GNSS_LIBGPS_MINILOG_SUBTYPE == pPacket->subType)||
          (GNSS_LIBGPS_LOG_BB_SUBTYPE == pPacket->subType)))
	{
		if(0 == state->rftool_enable)
		{
			preprocessGnssLog(pPacket->buff,pPacket->length,pPacket->subType);
		}	
	}
#endif
	switch(pPacket->type)
	{
		case GNSS_LIBGPS_SET_PARA_TYPE:
		{
                        GREENEYE2_LOGD(("[GE2]===>>>>set para type"));
			gps_getParaData(pPacket);
			break;
		}
		case GNSS_LIBGPS_RESPONSE_TYPE:
		{
			gps_getResponse(pPacket);
			break;
		}
		case GNSS_LIBGPS_NOTIFIED_TYPE:                     
		{
			gps_getNotified(pPacket);
			break;
		}
		case GNSS_LIBGPS_DATA_TYPE:
		{
			gps_getData(pPacket); 
			break;
		}
		case GNSS_LIBGPS_AIDING_DATA_TYPE:
		{
            if(state->etuMode)
            {
    			GREENEYE2_LOGE(("[GE2]in ETU work ,so don't send agps request"));
            }
            else
            {
    			GREENEYE2_LOGE(("[GE2]agps aiding data received"));
    			gps_getAdiData(pPacket);
            }
			break;
		}
		case GNSS_LIBGPS_POWER_RESPONSE_TYPE:
		{
			gps_getPowerResponse(pPacket);
			break;
		}
		case GNSS_LIBGPS_FLASH_TYPE:
                {
                        if(state->etuMode)
                        {
                            GREENEYE2_LOGD(("[GE2]in ETU work ,so don't save flash information "));
                        }
                        else
                        {            
                            GREENEYE2_LOGD(("[GE2]gps save flash information "));
                            gps_saveFlashCfg(pPacket);
                        }
			break;
		}
		case GNSS_LIBGPS_NMEA_TYPE:
		{
			if (gps_stop_flag == 0)
			{
				GREENEYE2_LOGD(("[GE2]====>>>>>nmea data is received"));
				nmea_parse_cp(pPacket->buff,pPacket->length);
			}
			else
			{
				GE2GPS_LOGD("we have enter stop process, so don't parse NMEA data\n");
			}
			break;
		}
		#ifdef GNSS_REG_WR
		case GNSS_LIBGPS_REG_WR_TYPE:
		{
			GREENEYE2_LOGD(("[GE2]obtain gnss register value from firmware\n"));
			break;
		}
		#endif
		default:
		{
			GREENEYE2_LOGE(("[GE2]the gps_dispatch  type error = %d\r\n",pPacket->type));
			break;
		}
	
	}
	
}
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

