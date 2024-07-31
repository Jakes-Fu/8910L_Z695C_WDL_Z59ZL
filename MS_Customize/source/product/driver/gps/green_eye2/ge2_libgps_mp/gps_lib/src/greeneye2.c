/******************************************************************************
 ** File Name:      greeneye2.c                                                    *
 ** Author:                                                       *
 ** DATE:                                                         *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 bsc data. It manages bootloader write and read.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME             	DESCRIPTION                               *
 ** 										                                  *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_api.h"
#include "greeneye2_common.h"
#include "threadx_os.h"
#include "threadx_assert.h"
#include "os_api.h"
#include "priority_system.h"
#include "greeneye2_engine.h"
#include "greeneye2_cmd.h"
#include "greeneye2.h"
#include "greeneye2_common.h"
#include "gps_com.h"
#include "gps_drv.h"
#include "sc6531efm_int_cfg.h"
#include "chip_drv_common_inter.h"
#include "dal_time.h"
#include "doidleonpage.h"
#include <stdio.h>
#include "sci_types.h" 
#include "greeneye2_tcard_log.h"
#include "gps_interface.h"
#include "mn_gps_signal_def.h"
#include "greeneye2_gnss_log.h"
#include "nv_item_id.h"
#include "nvitem.h"
#include "deepsleep_drvapi.h"

#ifdef GPS_AGPS_SUPPORT_QX
#include "qx_supl_sdk.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 * Static Global Variables
 ******************************************************************************/
Ge2PositionParam_t gLocationInfo[1];
extern TGNSS_DataProcessor_t gGPSStream;

extern unsigned int g_startmode_flg;
extern char  g_llh_time_flg;

RawGpsSvInfo sv_rawinfo;
//int gSUPLRspType = 0;
char thread_status = thread_init; 
char fix_flag = 0;
GpsState  _gps_state[1];

char gps_log_enable = 1;

static char *cold_cmd = "$PCGDC,CS*55\r\n";
static char *warm_cmd = "$PCGDC,WS*55\r\n";
static char *hot_cmd = "$PCGDC,HS*55\r\n";
static char *fac_cmd = "$PCGDC,FAC*55\r\n";
static GpsStatus gstate;

static int gps_mode = 0;
static int days_per_month_no_leap[] =
    {31,28,31,30,31,30,31,31,30,31,30,31};
static int days_per_month_leap[] =
    {31,29,31,30,31,30,31,31,30,31,30,31};
static char cer_path[128];
extern int gpsTow;

char *gps_idle_on = "$PCGDC,IDLEON,1,*1\r\n";
char *gps_idle_off="$PCGDC,IDLEOFF,1,*1\r\n"; 
extern char *libgps_version;

#define GPS_TX_WATER_MARK	127
#define GPS_RX_WATER_MARK	127

BLOCK_ID s_gps_thread_id = SCI_INVALID_BLOCK_ID;
#define GPS_THREAD_STACK_SIZE (5*1024)
#define s_gps_thread_task_priority     PRI_GPS_TASK    //31
#define GPS_THREAD_QUEUE_NUM    0x20

static BOOLEAN s_testmode_enable = SCI_FALSE;

static GPS_RF_TEST_MODE_E gps_rf_testmode;

uint8 gps_start_flag;
uint8 gps_stop_flag;

extern BOOLEAN ge2_sendephdata;
extern BOOLEAN ge2_storeeph;

BOOLEAN ge2_watchdogassert = SCI_FALSE;

TGe2TimeStamp ge2fixtime;

#define APPEND_CHAR(p, character) \
do \
{ \
	*(p) = (character); \
	(p) ++; \
} while(0)

#define DELAY_CYCLE(n)  {\
                                            uint32 k=0;\
                                             for(k=0; k<n; k++){}\
                                         }

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void gps_task_init(void);
LOCAL void gps_start_thread(void);
LOCAL void gps_task_entry(uint32 argc, void *argv);
LOCAL void gps_task_deinit(void);
LOCAL void gps_uart_callback(uint32 event);

LOCAL BOOLEAN GPS_STATUS = SCI_FALSE;
LOCAL void gps_cmd_process( char cmd );
LOCAL int gps_com_init(int uart_port);
LOCAL uint32 gps_getposition(char **pbuf);
LOCAL void gps_sendmsg(GPSDATACMD_TYPE sig,int sig_param);
LOCAL uint32 watchdog(uint32 param);
/**---------------------------------------------------------------------------*
 **                     Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    is_leap_year
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int is_leap_year(int year)
{
    if ((year%400) == 0)
        return 1;
    if ((year%100) == 0)
        return 0;
    if ((year%4) == 0)
        return 1;
    return 0;
}

/*****************************************************************************/
//  Description:    number_of_leap_years_in_between
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int number_of_leap_years_in_between(int from, int to)
{
    int n_400y, n_100y, n_4y;
    n_400y = to/400 - from/400;
    n_100y = to/100 - from/100;
    n_4y = to/4 - from/4;
    return (n_4y - n_100y + n_400y);
}

/*****************************************************************************/
//  Description:    utc_mktime
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL time_t utc_mktime(struct tm *_tm)
{
    time_t t_epoch=0;
    int m; 
    int *days_per_month;
    if (is_leap_year(_tm->tm_year+1900))
        days_per_month = days_per_month_leap;
    else
        days_per_month = days_per_month_no_leap;
    t_epoch += (_tm->tm_year - 70)*SECONDS_PER_NORMAL_YEAR; 
    t_epoch += number_of_leap_years_in_between(1970,_tm->tm_year+1900-1) * SECONDS_PER_DAY;
    for (m=0; m<_tm->tm_mon; m++) {
        t_epoch += days_per_month[m]*SECONDS_PER_DAY;
    }
    t_epoch += (_tm->tm_mday-1)*SECONDS_PER_DAY;
    t_epoch += _tm->tm_hour*SECONDS_PER_HOUR;
    t_epoch += _tm->tm_min*SECONDS_PER_MIN;
    t_epoch += _tm->tm_sec;
    return t_epoch;

}

/*****************************************************************************/
//  Description:    str2int
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int str2int( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;

    for ( ; len > 0; len--, p++ )
    {
        int  c;

        if (p >= end)
            goto Fail;

        c = *p - '0';
        if ((unsigned)c >= 10)
            goto Fail;

        result = result*10 + c;
    }
    return  result;

Fail:
    return -1;
}

/*****************************************************************************/
//  Description:    strhex2int
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int strhex2int( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;

    for ( ; len > 0; len--, p++ )
    {
        int  c;

        if (p >= end)
            goto Fail;
        if ((*p >= 'a') && (*p <= 'f'))
            c = *p - 'a' + 10;
        else if ((*p >= 'A') && (*p <= 'F'))
            c = *p - 'A' + 10;
        else if ((*p >= '0') && (*p <= '9'))
            c = *p - '0';
        else
            goto Fail;

        result = result*0x10 + c;
    }
    return  result;

Fail:
    return -1;
}

/*****************************************************************************/
//  Description:    str2float
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL double str2float( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;
    char  temp[16];
    if (len >= (int)sizeof(temp))
        return 0.;

    memcpy( temp, p, len );
    temp[len] = 0;
    return strtod( temp, NULL );
}

/*****************************************************************************/
//  Description:    my_floor
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL double my_floor(double a)
{
    int b;

    if(a >0)
        b = (int)a;
    else
        b = (int)a - 1;

    return (double)b;
}

/*****************************************************************************/
//  Description:    convert_from_hhmm
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL double convert_from_hhmm( Token  tok )
{
    double  val     = str2float(tok.p, tok.end);
    int     degrees = (int)(my_floor(val) / 100);
    double  minutes = val - degrees*100.;
    double  dcoord  = degrees + minutes / 60.0;

    return dcoord;
}

/*****************************************************************************/
//  Description:    nmea_reader_init
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void  nmea_reader_init( NmeaReader*  r )
{
    SCI_MEMSET( r, 0, sizeof(*r) );

    r->pos      = 0;
    r->overflow = 0;
    r->utc_year = -1;
    r->utc_mon  = -1;
    r->utc_day  = -1;
    r->location_callback = NULL;
    r->sv_status_callback = NULL;
    r->svinfo_flags = 0;
    r->fix.size = sizeof(GpsLocation);
    r->svstatus.size = sizeof(GpsSvStatus);
}

/*****************************************************************************/
//  Description:    nmea_reader_set_callback
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void nmea_reader_set_callback( NmeaReader*  r, GpsCallbacks  *callbacks)
{
    if(callbacks != NULL)
    {
        r->location_callback = callbacks->location_cb;
        r->sv_status_callback = callbacks->sv_status_cb;
        GREENEYE2_LOGE(("[GE2]nmea_reader_set_callback is enter,fix is %d sv_status_callback =%p",r->fix.flags,r->sv_status_callback));
        r->fix.flags = 0;
        if (callbacks->location_cb != NULL && r->fix.flags != 0) 
        {
            GREENEYE2_LOGD(("[GE2]%s: sending latest fix to new callback", __FUNCTION__));
            r->location_callback( &r->fix );
            r->fix.flags = 0;
        }
    }
    else
    {
        r->location_callback  = NULL;
        r->sv_status_callback  = NULL;
    }
}

/*****************************************************************************/
//  Description:    nmea_tokenizer_init
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_tokenizer_init( NmeaTokenizer*  t, const char*  p, const char*  end )
{
    int    count = 0;

    // the initial '$' is optional
    if (p < end && p[0] == '$')
        p += 1;

    // remove trailing newline
    if (end > p && end[-1] == '\n') {
        end -= 1;
        if (end > p && end[-1] == '\r')
            end -= 1;
    }

    // get rid of checksum at the end of the sentecne
    if (end >= p+3 && end[-3] == '*') {
        end -= 3;
    }

    while (p < end) {
        const char*  q = p;

        q = memchr(p, ',', end-p);
        //q = gps_memchr(p, ',', end-p);
        if (q == NULL)
            q = end;

        if (q >= p) {
            if (count < MAX_NMEA_TOKENS) {
                t->tokens[count].p   = p;
                t->tokens[count].end = q;
                count += 1;
            }
        }
        if (q < end)
            q += 1;

        p = q;
    }

    t->count = count;
    return count;
}

/*****************************************************************************/
//  Description:    nmea_tokenizer_get
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL Token  nmea_tokenizer_get( NmeaTokenizer*  t, int  index )
{
    Token  tok;
    static const char*  dummy = "";

    if (index < 0 || index >= t->count) {
        tok.p = tok.end = dummy;
    } else
        tok = t->tokens[index];

    return tok;
}

/*****************************************************************************/
//  Description:    nmea_tokenizer_get
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_time( NmeaReader*  r, Token  tok )
{
    int        hour, minute;
    double     seconds;
    struct tm  tm;
    time_t     fix_time;
    SCI_DATE_T sys_data;

    if (tok.p + 6 > tok.end)
        return -1;

    if (r->utc_year < 0) {
        TM_GetSysDate(&sys_data);
        r->utc_year = sys_data.year;
        r->utc_mon  = sys_data.mon;
        r->utc_day  = sys_data.mday;
    }

    hour    = str2int(tok.p,   tok.p+2);
    minute  = str2int(tok.p+2, tok.p+4);
    seconds = str2float(tok.p+4, tok.end);

    tm.tm_hour = hour;
    tm.tm_min  = minute;
    tm.tm_sec  = (int) seconds;
    tm.tm_year = r->utc_year - 1900;
    tm.tm_mon  = r->utc_mon - 1;
    tm.tm_mday = r->utc_day;

    fix_time = utc_mktime( &tm );
    r->fix.timestamp = (long long)fix_time * 1000;
//will be update time stamp
    SCI_MEMSET(&gLocationInfo->time,0x0,sizeof(TGe2TimeStamp));
    gLocationInfo->time.wYear = tm.tm_year + 1900;
    gLocationInfo->time.wMonth = tm.tm_mon + 1;
    gLocationInfo->time.wDay = tm.tm_mday;
    gLocationInfo->time.wHour = tm.tm_hour;
    gLocationInfo->time.wMinute = tm.tm_min;
    gLocationInfo->time.wSecond = tm.tm_sec;		
    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_update_latlong
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_latlong( NmeaReader*  r,
     Token  latitude, char   latitudeHemi,Token   longitude, char   longitudeHemi )
{
    double   lat, lon;
    Token    tok;

    tok = latitude;
    if (tok.p + 6 > tok.end) {
        GREENEYE2_LOGD(("[GE2]latitude is too short: '%.*s'", (int)(tok.end-tok.p), tok.p));
        return -1;
    }
    lat = convert_from_hhmm(tok);
    if (latitudeHemi == 'S')
        lat = -lat;

    tok = longitude;
    if (tok.p + 6 > tok.end) {
        GREENEYE2_LOGD(("[GE2]longitude is too short: '%.*s'", (int)(tok.end-tok.p), tok.p));
        return -1;
    }
    lon = convert_from_hhmm(tok);
    if (longitudeHemi == 'W')
        lon = -lon;

    r->fix.flags    |= GPS_LOCATION_HAS_LAT_LONG;
    r->fix.latitude  = lat;
    r->fix.longitude = lon;
//will update pos 
    gLocationInfo->pos.latitude = (float)(r->fix.latitude);
    gLocationInfo->pos.longitude = (float)(r->fix.longitude);

    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_update_satellites
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_satellites( NmeaReader*  r,
     Token  satellites)
{
    double  alt;
    Token   tok = satellites;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_SATELLITES;
    r->fix.satellites = (uint8)str2int(tok.p, tok.end);
//will update satellites
    gLocationInfo->pos.satellites = r->fix.satellites;
    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_update_altitude
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_altitude( NmeaReader*  r,
           Token   altitude)
{
    double  alt;
    Token   tok = altitude;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_ALTITUDE;
    r->fix.altitude = str2float(tok.p, tok.end);
//will update altitue
    gLocationInfo->pos.altitude = (float)(r->fix.altitude);
    return 0;
}

/*****************************************************************************/
//  Description:    gps_srand_time
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL uint32 gps_srand_time()
{
    SCI_TM_T realtime;
    uint32 msec = 0;
    TM_GetTime(&realtime);
    msec += (realtime.tm_year - 1970) * 12 * 30 * 24 * 60 * 60;
    msec += realtime.tm_mon * 30 * 24 * 60 * 60;
    msec += realtime.tm_mday* 24 * 60 * 60;
    msec += realtime.tm_hour * 60;
    msec += realtime.tm_min *60;
    msec += realtime.tm_sec;

    return msec;
}

/*****************************************************************************/
//  Description:    nmea_reader_update_accuracy
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_accuracy ( NmeaReader*  r,
      Token    hdop)
{
    double  alt;
    Token   tok = hdop;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_ACCURACY;
    r->fix.accuracy = str2float(tok.p, tok.end)*10.0;
    
    srand(gps_srand_time()); 
    
    if(( r->fix.accuracy > 12) &&( r->fix.accuracy < 60))
    {
        r->fix.accuracy = 6 + rand()%7;
    }
    else if( r->fix.accuracy > 60)
    {
        r->fix.accuracy = 12 + rand()%6;
    }
    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_handle_pstat
//  Author:         
//  Note:           
/*****************************************************************************/
static int nmea_reader_handle_pstat(NmeaReader *r, NmeaTokenizer *tzer)
{
    GpsState* s = _gps_state;
    int prn;
    float snr;
    int nline;
    Token tok;

    GREENEYE2_LOGD(("[GE2]nmea_reader_handle_pstat enter,nmeareader point:%p\r\n",r));

    tok=nmea_tokenizer_get(tzer,1);
    nline = str2int(tok.p, tok.end);
    if(nline == 1)
    {
        SCI_MEMSET(&sv_rawinfo, 0, sizeof(RawGpsSvInfo));
    }

    tok=nmea_tokenizer_get(tzer,2);
    prn = str2int(tok.p, tok.end);

    tok=nmea_tokenizer_get(tzer,13);
    snr = str2float(tok.p, tok.end);
    if(sv_rawinfo.num_svs >= 49)
    {
        GREENEYE2_LOGE(("[GE2]sv_rawinfo.num_svs too large"));
        return 0;
    }
    sv_rawinfo.sv_list[sv_rawinfo.num_svs].prn = prn;
    sv_rawinfo.sv_list[sv_rawinfo.num_svs].snr = snr;
    sv_rawinfo.num_svs++;

    return 0;
}


/*****************************************************************************/
//  Description:    nmea_reader_handle_gsv
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_handle_gsv(NmeaReader *r, NmeaTokenizer *tzer)
{
    int lines;
    int nline;
    int nsat;
	int i = 0;
    Token tok;
    GpsSvInfo  *svinfo;

	
	GREENEYE2_LOGD(("[GE2] nmea_reader_handle_gsv enter\r\n"));
    
    tok=nmea_tokenizer_get(tzer,1);
    lines = str2int(tok.p, tok.end);

    tok=nmea_tokenizer_get(tzer,2);
    nline = str2int(tok.p, tok.end);

    tok=nmea_tokenizer_get(tzer,3);
    if (nline == 1)
        r->svstatus.num_svs = 0;
    for (nsat=1; nsat <= 4;nsat++) {
            tok=nmea_tokenizer_get(tzer,4*nsat);
            if (!TOKEN_LEN(tok))
                continue;
            //if (str2int(tok.p,tok.end)>32)
            //    continue;
			if(r->svstatus.num_svs >= GPS_MAX_SVS)
			{
				GREENEYE2_LOGE(("[GE2]svstatus num over or equal %d",GPS_MAX_SVS));
				break;
			}
            svinfo=&r->svstatus.sv_list[r->svstatus.num_svs++];
            svinfo->size = sizeof(GpsSvInfo);
            svinfo->prn=str2int(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+1);
            svinfo->elevation=str2float(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+2);
            svinfo->azimuth=str2float(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+3);
            svinfo->snr=str2float(tok.p,tok.end);
			if(sv_rawinfo.num_svs >= 1)
			{
				for(i = 0;i<sv_rawinfo.num_svs;i++)
				{
					if(svinfo->prn == sv_rawinfo.sv_list[i].prn)
					{
						svinfo->snr = sv_rawinfo.sv_list[i].snr;
						break;
					}
				}
			}
    }
    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_handle_glgsv
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_handle_glgsv(NmeaReader *r, NmeaTokenizer *tzer)
{
    int lines;
    int nline;
    int nsat;
	int i = 0;
    Token tok;
    GpsSvInfo  *svinfo;

	if(r->svstatus.num_svs >= GPS_MAX_SVS)
	{
		GREENEYE2_LOGE(("[GE2]num bds svs over %d",GPS_MAX_SVS));
		return 0;
	}  
  
    tok=nmea_tokenizer_get(tzer,1);
    lines = str2int(tok.p, tok.end);

    tok=nmea_tokenizer_get(tzer,2);
    nline = str2int(tok.p, tok.end);
	
    tok=nmea_tokenizer_get(tzer,3);

    for (nsat=1; nsat <= 4;nsat++) {
            tok=nmea_tokenizer_get(tzer,4*nsat);
            if (!TOKEN_LEN(tok))
                continue;
            //if (str2int(tok.p,tok.end)>32)
               // continue;
			if(r->svstatus.num_svs >= GPS_MAX_SVS)
			{
				GREENEYE2_LOGE(("[GE2]svstatus num over or equal  %d",GPS_MAX_SVS));
				break;
			}
            svinfo=&r->svstatus.sv_list[r->svstatus.num_svs++];
            svinfo->size = sizeof(GpsSvInfo);
            svinfo->prn=str2int(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+1);
            svinfo->elevation=str2float(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+2);
            svinfo->azimuth=str2float(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+3);
            svinfo->snr=str2float(tok.p,tok.end);
			if(sv_rawinfo.num_svs >= 1)
			{
				for(i = 0;i<sv_rawinfo.num_svs;i++)
				{
					if(svinfo->prn == sv_rawinfo.sv_list[i].prn)
					{
						svinfo->snr = sv_rawinfo.sv_list[i].snr;
						break;
					}
				}
			}
    }

    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_handle_glgsv
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_handle_bdgsv(NmeaReader *r, NmeaTokenizer *tzer)
{
    int lines;
    int nline;
    int nsat;
	int i = 0;
    Token tok;
    GpsSvInfo  *svinfo;
    
	if(r->svstatus.num_svs >= GPS_MAX_SVS)
	{
		GREENEYE2_LOGE(("[GE2]num bds svs over or equal %d",GPS_MAX_SVS));
		return 0;
	}  
  
    tok=nmea_tokenizer_get(tzer,1);
    lines = str2int(tok.p, tok.end);

    tok=nmea_tokenizer_get(tzer,2);
    nline = str2int(tok.p, tok.end);
	

    tok=nmea_tokenizer_get(tzer,3);

    for (nsat=1; nsat <= 4;nsat++) {
            tok=nmea_tokenizer_get(tzer,4*nsat);
            if (!TOKEN_LEN(tok))
                continue;
			if(r->svstatus.num_svs >= GPS_MAX_SVS)
			{
				GREENEYE2_LOGE(("[GE2]svstatus num over or equal %d",GPS_MAX_SVS));
				break;
			}
            svinfo=&r->svstatus.sv_list[r->svstatus.num_svs++];
            svinfo->size = sizeof(GpsSvInfo);
            svinfo->prn=str2int(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+1);
            svinfo->elevation=str2float(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+2);
            svinfo->azimuth=str2float(tok.p,tok.end);
            tok=nmea_tokenizer_get(tzer,4*nsat+3);
            svinfo->snr=str2float(tok.p,tok.end);
			if(sv_rawinfo.num_svs >= 1)
			{
				for(i = 0;i<sv_rawinfo.num_svs;i++)
				{
					if(svinfo->prn == sv_rawinfo.sv_list[i].prn)
					{
						svinfo->snr = sv_rawinfo.sv_list[i].snr;
						break;
					}
				}
			}
    }

    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_update_bearing
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_bearing( NmeaReader*  r, Token  bearing )
{
    double  alt;
    Token   tok = bearing;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_BEARING;
    r->fix.bearing  = str2float(tok.p, tok.end);
//update bearing
    gLocationInfo->velocity.bearing = (float)(r->fix.bearing);
    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_update_date
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_date( NmeaReader*  r, Token  date, Token  time )
{
    Token  tok = date;
    int    day, mon, year;

    if (tok.p + 6 != tok.end) {
        GREENEYE2_LOGE(("[GE2]date not properly formatted: '%.*s'", (int)(tok.end-tok.p), tok.p));
        return -1;
    }
    day  = str2int(tok.p, tok.p+2);
    mon  = str2int(tok.p+2, tok.p+4);
    year = str2int(tok.p+4, tok.p+6) + 2000;

    if ((day|mon|year) < 0) {
        GREENEYE2_LOGE(("[GE2]date not properly formatted: '%.*s'", (int)(tok.end-tok.p), tok.p));
        return -1;
    }

    r->utc_year  = year;
    r->utc_mon   = mon;
    r->utc_day   = day;

    return nmea_reader_update_time( r, time );
}

/*****************************************************************************/
//  Description:    nmea_reader_update_speed
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_speed( NmeaReader* r, Token speed )
{
    double  alt;
    Token   tok = speed;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_SPEED;
    r->fix.speed    = str2float(tok.p, tok.end);
//update vec horizon
    gLocationInfo->velocity.horizontalSpeed = (float)(r->fix.speed);
    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_update_satellites
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_update_fixmode( NmeaReader*  r,
     Token  fixStatus)
{
    double  alt;
    Token   tok = fixStatus;
    uint8 fix_mode = 0;

    if (tok.p >= tok.end)
        return -1;
    fix_mode = (uint8)str2int(tok.p, tok.end);

    gLocationInfo->fix_mode= fix_mode;
    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_handle_pglor_sat
//  Author:         
//  Note:           
/*****************************************************************************/
static int  nmea_reader_handle_pglor_sat(NmeaReader *r, NmeaTokenizer *tzer)
{
    Token tok;
    int indx;
    int prn;
    uint32 sv_flags;
    uint32 sv_mask;

    //r->svstatus.used_in_fix_mask = 0;
    //r->svstatus.ephemeris_mask   = 0;
    //r->svstatus.almanac_mask     = 0;
    for (indx=2;indx<tzer->count;indx+=3) {
        tok=nmea_tokenizer_get(tzer,indx);
        if (!TOKEN_LEN(tok))
            continue;
        prn = str2int(tok.p,tok.end);
        GREENEYE2_LOGD(("[GE2]prn str(%.*s) d(%d)",(int)(tok.end-tok.p),tok.p,prn));
        if (prn > 32)
            continue;
        sv_mask = 1 << (prn-1);
        tok=nmea_tokenizer_get(tzer,indx+2);
        sv_flags = strhex2int(tok.p,tok.end);
        GREENEYE2_LOGD(("[GE2]svflags str(%.*s) d(%X)",(int)(tok.end-tok.p),tok.p,sv_flags));
        //if (sv_flags&PGLOR_SAT_SV_USED_IN_POSITION)
        //    r->svstatus.used_in_fix_mask |= sv_mask;
        //D("used d(%X)", r->svstatus.used_in_fix_mask);
        switch(sv_flags&0x30) {
            case PGLOR_SAT_SV_EPH_SRC_BE:
            case PGLOR_SAT_SV_EPH_SRC_CBEE:
            case PGLOR_SAT_SV_EPH_SRC_SBEE:
               // r->svstatus.ephemeris_mask |= sv_mask;
                break;
            default:
                break;
        }
       // D("eph d(%X)", r->svstatus.ephemeris_mask);

    }
    r->svinfo_flags |= SVINFO_GOT_SV_USED_FLAG;
    r->svinfo_flags |= SVINFO_GOT_EPH_INFO_FLAG;
    GREENEYE2_LOGD(("[GE2]Set svinfo_flags SV_USED_FLAG & EPH_INFO_FLAG\r\n"));
    return 0;
}

/*****************************************************************************/
//  Description:    nmea_reader_handle_gsa
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int nmea_reader_handle_gsa(NmeaReader *r, NmeaTokenizer *tzer)
{
    int i,prn;
    Token tok_fixStatus,tok_accuracy,  tok_prn;

    tok_fixStatus   = nmea_tokenizer_get(tzer, 2);

    nmea_reader_update_fixmode(r,tok_fixStatus);

    if ((tok_fixStatus.p[0] != '\0' && tok_fixStatus.p[0] != '1') && (fix_flag == 1))
    {
        tok_accuracy = nmea_tokenizer_get(tzer, 15);
        nmea_reader_update_accuracy(r, tok_accuracy);   

        for(i = 3; i <=14; i++)
        {
            tok_prn  = nmea_tokenizer_get(tzer, i);
            prn = str2int(tok_prn.p, tok_prn.end);
            if((prn > 0) && (prn <= 32))
            {
                r->svstatus.used_in_fix_mask[0] |= (1 << (prn-1));
            }
            if((prn > 32) && (prn <= 64))
            {
                r->svstatus.used_in_fix_mask[1] |= (1 << (prn-33));
            }
            if((prn > 64) && (prn <= 96))
            {
                r->svstatus.used_in_fix_mask[2] |= (1 << (prn-65));
            }
            if((prn > 96) && (prn <= 128))
            {
                r->svstatus.used_in_fix_mask[3] |= (1 << (prn-97));
            }
            if((prn > 128) && (prn <= 160))
            {
                r->svstatus.used_in_fix_mask[4] |= (1 << (prn-129));
            }
            if((prn > 160) && (prn <= 192))
            {
                r->svstatus.used_in_fix_mask[5] |= (1 << (prn-161));
            }
            if((prn > 192) && (prn <= 224))
            {
                r->svstatus.used_in_fix_mask[6] |= (1 << (prn-193));
            }
        }
		GREENEYE2_LOGD(("[GE2]&&& gsa used_in_fix_masks = 0x%x\n",r->svstatus.used_in_fix_mask[0]));
    }
	else
	{
		SCI_MEMSET(r->svstatus.used_in_fix_mask,0,sizeof(r->svstatus.used_in_fix_mask));  
		SCI_MEMSET(r->svstatus.ephemeris_mask,0,sizeof(r->svstatus.ephemeris_mask)); 
		SCI_MEMSET(r->svstatus.almanac_mask,0,sizeof(r->svstatus.almanac_mask)); 
	}
    return 0;
}

/*****************************************************************************/
//  Description:    msa_update_latlong
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int msa_update_latlong( NmeaReader*  r,
     Token  latitude, char   latitudeHemi,Token   longitude, char   longitudeHemi )
{
    double   lat, lon;
    Token    tok;

    tok = latitude;
    lat = str2float(latitude.p,latitude.end);
    if (latitudeHemi == 'S')
        lat = -lat;

    tok = longitude;
    if (tok.p + 6 > tok.end) {
        GREENEYE2_LOGE(("[GE2]longitude is too short: '%.*s'", (int)(tok.end-tok.p), tok.p));
        return -1;
    }
    lon = str2float(longitude.p,longitude.end);
    if (longitudeHemi == 'W')
        lon = -lon;

    r->fix.flags    |= GPS_LOCATION_HAS_LAT_LONG;
    r->fix.latitude  = lat;
    r->fix.longitude = lon;
    return 0;
}

/*****************************************************************************/
//  Description:    parse_gga_token
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void parse_gga_token(NmeaTokenizer *tzer,NmeaReader*  r)
{
    Token  tok_time          = nmea_tokenizer_get(tzer,1);
    Token  tok_latitude      = nmea_tokenizer_get(tzer,2);
    Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,3);
    Token  tok_longitude     = nmea_tokenizer_get(tzer,4);
    Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,5);
    Token  tok_satellites = nmea_tokenizer_get(tzer,7);
    Token  tok_accuracy      = nmea_tokenizer_get(tzer,8);
    Token  tok_altitude      = nmea_tokenizer_get(tzer,9);

    nmea_reader_update_time(r, tok_time);
    nmea_reader_update_latlong(r, tok_latitude,
                              tok_latitudeHemi.p[0],
                              tok_longitude,
                              tok_longitudeHemi.p[0]);
    nmea_reader_update_satellites(r, tok_satellites);
    nmea_reader_update_altitude(r, tok_altitude);
    nmea_reader_update_accuracy(r, tok_accuracy);
}

/*****************************************************************************/
//  Description:    parse_rmc_token
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL int parse_rmc_token(NmeaTokenizer *tzer,NmeaReader*  r)
{
     int ret = 0;
//     char rsp = 2;
//     int sid = 1234;
     Token  tok_time          = nmea_tokenizer_get(tzer,1);
     Token  tok_fixStatus     = nmea_tokenizer_get(tzer,2);
     Token  tok_latitude      = nmea_tokenizer_get(tzer,3);
     Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,4);
     Token  tok_longitude     = nmea_tokenizer_get(tzer,5);
     Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,6);
     Token  tok_speed         = nmea_tokenizer_get(tzer,7);
     Token  tok_bearing       = nmea_tokenizer_get(tzer,8);
     Token  tok_date          = nmea_tokenizer_get(tzer,9);
     
	 SCI_MEMSET(r->svstatus.used_in_fix_mask,0,sizeof(r->svstatus.used_in_fix_mask));  
	 SCI_MEMSET(r->svstatus.ephemeris_mask,0,sizeof(r->svstatus.ephemeris_mask)); 
	 SCI_MEMSET(r->svstatus.almanac_mask,0,sizeof(r->svstatus.almanac_mask)); 
		
     GREENEYE2_LOGD(("[GE2]in RMC, fixStatus=%c", tok_fixStatus.p[0]));
     if (tok_fixStatus.p[0] == 'A')
     {

         nmea_reader_update_date( r, tok_date, tok_time );

         nmea_reader_update_latlong( r, tok_latitude,
                                   tok_latitudeHemi.p[0],
                                   tok_longitude,
                                   tok_longitudeHemi.p[0] );

         nmea_reader_update_bearing( r, tok_bearing );
         nmea_reader_update_speed  ( r, tok_speed );
         ret = 1;
     }
    return ret;
}

/*****************************************************************************/
//  Description:    parse_gds_token
//  Author:         
//  Note:           
/*****************************************************************************/
LOCAL void parse_gds_token(NmeaTokenizer *tzer,NmeaReader*  r)
{
    Token  mtok_msa;
    Token  mtok_latitude;
    Token  mtok_latitudeHemi;
    Token  mtok_longitude;
    Token  mtok_longitudeHemi;
    Token  maltitude;
    Token  mtok_speed;
    Token  mtok_bearing;
    
    //if(gSUPLRspType != RSP_MSA) 
    //    return;
    
    mtok_msa = nmea_tokenizer_get(tzer,1);
    
    if(memcmp(mtok_msa.p,"MSA",3)) 
        return;

    mtok_latitude = nmea_tokenizer_get(tzer,2);
    mtok_latitudeHemi = nmea_tokenizer_get(tzer,3);
    mtok_longitude = nmea_tokenizer_get(tzer,4);
    mtok_longitudeHemi = nmea_tokenizer_get(tzer,5);
    maltitude         = nmea_tokenizer_get(tzer,6);
    mtok_speed = nmea_tokenizer_get(tzer,7);
    mtok_bearing = nmea_tokenizer_get(tzer,8);

    GREENEYE2_LOGD(("[GE2]msa is enter now\n"));
    msa_update_latlong( r, mtok_latitude,
    mtok_latitudeHemi.p[0],
    mtok_longitude,
    mtok_longitudeHemi.p[0] );
    r->fix.flags   |= GPS_LOCATION_HAS_ALTITUDE;
    r->fix.altitude = str2float(maltitude.p, maltitude.end);
    nmea_reader_update_bearing( r, mtok_bearing );
    nmea_reader_update_speed  ( r, mtok_speed );
    GREENEYE2_LOGD(("[GE2]MSA lat is %d,long is %d,altitude is %d,bearing is %d,speed is %d\n",
    (int)(r->fix.latitude),(int)(r->fix.longitude),(int)(r->fix.altitude),(int)(r->fix.bearing),(int)(r->fix.speed)));

    r->location_callback( &r->fix );
    r->location_callback( &r->fix );
    r->location_callback( &r->fix );

}

/*****************************************************************************/
//  Description:    parse_lor_token
//  Author:         
//  Note:           
/*****************************************************************************/
static void parse_lor_token(NmeaTokenizer *tzer,NmeaReader*  r)
{
     Token tok;
     tok = nmea_tokenizer_get(tzer,1);
     GREENEYE2_LOGD(("[GE2]sentence id $PGLOR,%.*s ", (int)(tok.end-tok.p), tok.p));
     if (TOKEN_LEN(tok) >= 3) {
         if (!memcmp(tok.p,"SAT",3) )
             nmea_reader_handle_pglor_sat(r, tzer);
     }
}

/*****************************************************************************/
//  Description:    parse_pcgds_token
//  Author:         
//  Note:           
/*****************************************************************************/
static void parse_pcgds_token(NmeaTokenizer *tzer,NmeaReader*  r)
{
    Token  name = nmea_tokenizer_get(tzer,1);
    Token  value = nmea_tokenizer_get(tzer,2);
    GpsState*  s = _gps_state;    


    if( !memcmp(name.p,"CWCN0",5))
    {
    }
    else if( !memcmp(name.p,"TSXTEMP",7))
    {
    }
    else if( !memcmp(name.p,"TCXO",4))
    {
    }
}

/*****************************************************************************/
//  Description:    nmea_reader_parse
//  Author:         
//  Note:           
/*****************************************************************************/
void  nmea_reader_parse( NmeaReader*  r )
{
    NmeaTokenizer  tzer[1];
    Token          tok;
    GpsState*  s = _gps_state;    
    int nmea_end = 0;
    int sum_check = 0;
    GPSNMEA_TYPE type = GPS_TYPE_MAX;
    uint8 spttff[32] = {0};
    char *p = NULL;
    char TimeString[32] = {0};

    if (r->pos < 9) {
        GREENEYE2_LOGE(("[GE2]nmea lenth below 9,discarded."));
        return;
    }

	//===================nmea log read=====================//
	if(!((r->in[0] == '$') && (r->in[1] == 'P') && (r->in[2] == 'S')))
	{
		//s->callbacks.nmea_cb(r->fix.timestamp,r->in,r->pos);
#ifdef GPS_MINI_LOG_SUPPORT
                //savenmealog2Buffer(r->in,r->pos);
#endif
		//GREENEYE2_LOGD(("[GE2][LOG]nmea_reader_parse len=%d,data=%s",r->pos,r->in));
	}
	//====================end===========================//
	//if((r->in[0] != '$') || ((r->in[1] != 'G') && (r->in[1] != 'B')))
	if((r->in[0] != '$') || ((r->in[1] != 'G') && (r->in[1] != 'B') && (r->in[1] != 'P') && ((r->in[1] == 'P')||(r->in[2] != 'S'))))
	{
		//GREENEYE2_LOGE(("[GE2]int nmea log,token '$G' or '$B' not found"));
		return;
	}
    nmea_tokenizer_init(tzer, r->in, r->in + r->pos);

    tok = nmea_tokenizer_get(tzer, 0);
    if (tok.p + 5 > tok.end) {
        GREENEYE2_LOGD(("sentence id '%.*s' too short, ignored.", (int)(tok.end-tok.p), tok.p));
        return;
    }

    //GREENEYE2_LOGD(("[GE2]&&&&& p0= %c, p1= %c \r\n",*(tok.p),*(tok.p+1)));

    tok.p += 2;

    sum_check = *tok.p + *(tok.p + 1) + *(tok.p + 2);
    /*-------------------------------------
		GGA is 207
		GSV is 240
		GSA is 219
		RMC is 226
		WER is 238
		GDS is 222
		CMD is 212
		LOR is 237
		log is 322
		DMP is 225
    -------------------------------------*/
    switch(sum_check)
    {
        case ('G'+'G'+'A'):
            parse_gga_token(tzer,r);
            type = GPS_GGA_TYPE;
            break;
        case ('G'+'S'+'A'):
            nmea_reader_handle_gsa(r,tzer);
            type = GPS_GSA_TYPE;
            break;
        case ('G'+'S'+'V'):
			if(!memcmp(r->in,"$GLGSV",6))
			{				
				nmea_reader_handle_glgsv(r, tzer);
				break;
			}
			if(!memcmp(r->in,"$BDGSV",6))
			{
				nmea_reader_handle_bdgsv(r, tzer);
				break;
			}
            nmea_reader_handle_gsv(r, tzer);
            type = GPS_GSV_TYPE;
            break;
        case ('R'+'M'+'C'):
	     r->svstatus.num_svs = 0;   //clear svstatus
             fix_flag = parse_rmc_token(tzer,r);
             type = GPS_RMC_TYPE;
	     GE2GPS_LOGD("%d",fix_flag);
	     if((fix_flag) && (gps_start_flag))
	     {
	     	{
			GPS_FIX_SUCCESS_T *signal = PNULL;

		        SCI_CREATE_SIGNAL(
		        signal,
		        GPS_FIX_SUCCESS_REQ,
		        sizeof (GPS_FIX_SUCCESS_T),
		        SCI_IDENTIFY_THREAD());
		        SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, GPS_GetInterfaceThreadId()); 
		}
		 gps_start_flag = 0x0;
	     }
             break;
        case ('G'+'D'+'S'):
            parse_gds_token(tzer,r);
            break;
        case ('L'+'O'+'R'):
            parse_lor_token(tzer,r);
            break;
        case ('V'+'T'+'G'):
            nmea_end = 1;
            r->svinfo_flags |= SVINFO_GOT_SV_INFO_FLAG;
            type = GPS_VTG_TYPE;
            if(gLocationInfo->ttff > 0)
            {
		if(ge2fixtime.wYear == 0x0)
			SCI_MEMCPY((uint8 *)&ge2fixtime,(uint8 *)&(gLocationInfo->time),sizeof(TGe2TimeStamp));

		strcpy(spttff,"$SPTTFF");
		p = TimeString;
		APPEND_CHAR(p, ',');
		p = AppendInteger(p, ge2fixtime.wHour, 2);
		p = AppendInteger(p, ge2fixtime.wMinute, 2);
		p = AppendInteger(p, ge2fixtime.wSecond, 2);
		APPEND_CHAR(p, ',');
		p = AppendInteger(p, gLocationInfo->ttff, 0);
		strcat(spttff,TimeString);			
		NmeaTerminate(spttff);
		GE2GPS_LOGD("ttff=%d,%s", gLocationInfo->ttff,spttff);
		s->callbacks.nmea_cb(GNSS_TYPE_LOG_NMEA,spttff,strlen(spttff));
            }
            break;
	case ('T'+'A'+'T'):
            if(!memcmp(r->in,"$PSTAT",6))
            {
            	nmea_reader_handle_pstat(r, tzer);
            }
            break;
    	case ('G'+'L'+'L'):
            type = GPS_GLL_TYPE;
	    break;
        default:
            break;
    }

    if(s->logswitch)	
    {
        GREENEYE2_LOGD(("[GE2]r->svinfo_flags = 0x%x, \r\n",r->svinfo_flags));
    }
    if ((r->svinfo_flags & SVINFO_GOT_EPH_INFO_FLAG) ||
            (r->svinfo_flags & SVINFO_GOT_SV_INFO_FLAG) ||
            (r->svinfo_flags & SVINFO_GOT_SV_USED_FLAG)) {

        if (r->sv_status_callback) {
            GREENEYE2_LOGD(("[GE2]svstatus size =%d, numsvs =%d",r->svstatus.size, r->svstatus.num_svs));
            r->sv_status_callback( &r->svstatus );
            r->svinfo_flags = 0;
            SCI_MEMSET(&sv_rawinfo, 0, sizeof(RawGpsSvInfo));
        }
    }
    if((nmea_end == 1) && (r->fix.flags & GPS_LOCATION_HAS_LAT_LONG) &&
           (r->fix.flags & GPS_LOCATION_HAS_ALTITUDE) &&
           (r->fix.flags & GPS_LOCATION_HAS_SPEED) &&
           (r->fix.flags & GPS_LOCATION_HAS_BEARING)) {

	    GREENEYE2_LOGD(("[GE2]callback flag is %d",fix_flag));
        if ((r->location_callback) && (fix_flag == 1)) 
        {	
            r->fix.speed = r->fix.speed*0.51417;
            if(s->logswitch)	
            {   
                //GREENEYE2_LOGD(("[GE2]nmea_reader_parse [gSUPLRspType:%d][gPosMethod:%d]",gSUPLRspType,gPosMethod));
            }
            r->location_callback( &r->fix );
            r->fix.flags = 0;            
        }
        else {
            GREENEYE2_LOGD(("[GE2]no callback, keeping data until needed !"));
        }

    }

    s->callbacks.send_nmea_cb(type,r->in,r->pos);
    s->callbacks.nmea_cb(GNSS_TYPE_LOG_NMEA,r->in,r->pos);
}

/*****************************************************************************/
//  Description:    nmea_reader_addc
//  Author:         
//  Note:           
/*****************************************************************************/
void  nmea_reader_addc( NmeaReader*  r, int  c )
{
    if (r->overflow) {
        r->overflow = (c != '\n');
        return;
    }

    if (r->pos >= (int) sizeof(r->in)-1 ) {
        r->overflow = 1;
        r->pos      = 0;
        return;
    }

    r->in[r->pos] = (char)c;
    r->pos       += 1;

    if ((r->pos > 2) && (c == '\n') && (r->in[r->pos-2] == '\r')){
        nmea_reader_parse(r);
        r->pos = 0;
    }
}

/*****************************************************************************/
//  Description:    gps_test_mode
//  Author:         
//  Note:           
/*****************************************************************************/
void gps_test_mode(GpsState *s)
{
	int flag = COMMAND;
	int ret = 0;
	if(gps_mode > 0)
	{	
		SCI_Sleep(100);
		GE2GPS_LOGD("gps_mode=%d",gps_mode);
		switch(gps_mode)
		{
			case 1: //warm start 
			{
                                GREENEYE2_LOGD(("set gps_mode=1,warm start now"));
                                s->cmd_flag = WARM_START_MODE;
                                if(s->hotTestFlag)
                                {
                                    s->hotTestFlag = 0; 
                                }
                                gps_sendData(flag, warm_cmd); 
				break; 
			}

			case 125:
			{
                                GREENEYE2_LOGD(("set gps_mode=125,cold start now"));
                                s->cmd_flag = COLD_START_MODE;
                                if(0 == s->hotTestFlag)
                                {
                                    s->hotTestFlag = 1; 
                                }
                                g_startmode_flg = 1;
                                gps_sendData(flag, cold_cmd);
				break;
			}
			case 136:
			{
				GREENEYE2_LOGD(("enable log save"));
				gps_log_enable = 1; 
				break;
			}
			case 192:
			{
				break;
			}
			case 448:
			{
				break;
			}
			case 520:
			{
				gps_log_enable = 0; 
				break;
			}
			case 1024: 
			{
                                GREENEYE2_LOGD(("set gps_mode=1024,hot start now"));

                                s->cmd_flag = HOT_START_MODE;
                                if(s->hotTestFlag)
                                {
                                    s->hotTestFlag = 0; 
                                }
                                gps_sendData(flag, hot_cmd);
				break;
			}
			case 4100:
				break;
			case 4104:
				break;
			case 4108:
				break;
			case 4112:
				break;
			case 4116:
				break;
			case 34813:
				break;
			case 34815:
                                GREENEYE2_LOGD(("[GE2]set gps_mode=34815 fac test"));
                                s->cmd_flag = FAC_START_MODE;
                                if(s->hotTestFlag)
                                {
                                    s->hotTestFlag = 0; 
                                    GREENEYE2_LOGD(("[GE2]fact start get gnsslock now"));
                                }
                                gps_sendData(flag, fac_cmd);
                                break;
                        case 65535:
                                GREENEYE2_LOGD(("[GE2]set gps_mode=65535 fac test"));
                                s->cmd_flag = FAC_START_MODE;
                                if(s->hotTestFlag)
                                {
                                    s->hotTestFlag = 0; 
                                    GREENEYE2_LOGD(("[GE2]fact start get gnsslock now"));
                                }
                                gps_sendData(flag, fac_cmd);
                                break;
			#if 1//def TEST_MODE
			case 50001:
				GREENEYE2_LOGD(("[GE2]TCXO test mode"));
                                s->etuMode = 1;
				gps_sendData(EUT_TEST_TCXO,NULL); 
				break;
			case 50002:
				GREENEYE2_LOGD(("[GE2]CW CN0 test mode"));
                                s->etuMode = 1;
				s->cw_enable = 1;   // HW_CW
				s->watchdog = 0;
				gps_sendData(EUT_TEST_CW,NULL);
				break;
			case 50003:
				GREENEYE2_LOGD(("[GE2]rf data tool mode"));
                                s->etuMode = 1;
                                s->rftool_enable = 1;
				s->watchdog = 0;
				gps_sendData(RF_DATA_CAPTURE, NULL);
				break;
			case 50004:
				GREENEYE2_LOGD(("[GE2]tsx temp test mode"));
                                s->etuMode = 1;
                                s->tsx_enable = 0x1;
				gps_sendData(EUT_TEST_TSX,NULL);
				break;
                        case 50005:
                            GREENEYE2_LOGD(("[GE2]TCXO/TSX dia new test"));
                            s->etuMode = 1;
                            gps_sendData(EUT_TEST_TCXO_TSX, NULL);
                            break;
			#endif

			default:
				break;
		}
		gps_mode = 0;
		s->cmd_flag = 0;
	}	 
}

#ifdef GE2_ASSERT_DEBUG
typedef struct ge2_heart_test_record_debug_tag
{
    uint32 tick;
    uint32 data;
} ge2_heart_test_record_debug_t;

ge2_heart_test_record_debug_t ge2_heart_test_record_debug[256] = {0};
uint32 heart_test_record_index = 0;
#endif
LOCAL void gps_heart_test_record_debug(uint32 data)
{
#ifdef GE2_ASSERT_DEBUG
	ge2_heart_test_record_debug[heart_test_record_index].tick = SCI_GetTickCount();
	ge2_heart_test_record_debug[heart_test_record_index].data= data;
	heart_test_record_index++;
	if(heart_test_record_index >= 256)
	{
		heart_test_record_index = 0;
	}
#endif
}
/*****************************************************************************/
//  Description:    gps_sendHeartTest
//  Author:         
//  Note: it  encode and send  heart test request           
/*****************************************************************************/
static void  gps_sendHeartTest(GpsState* pGPsState)
{
	TCmdData_t packet;
	int len,writeLen ;

	SCI_MEMSET(&packet,0,sizeof(TCmdData_t));
	//first check it can send data ?  todo 	
	packet.type    = GNSS_LIBGPS_DATA_TYPE;
	packet.subType = GNSS_LIBGPS_TEST_SUBTYPE;
	packet.length = 0;
	len = GNSS_EncodeOnePacket( &packet, pGPsState->writeBuffer, SERIAL_WRITE_BUFFER_MAX);

	if(len > 0)
	{
	gps_heart_test_record_debug(UART_GetRxFifoCnt(pGPsState->uart_port));
	gps_heart_test_record_debug(UART_GetTxFifoCnt(pGPsState->uart_port));
	gps_heart_test_record_debug(UART_IsTxFifoBusy(pGPsState->uart_port));

	if(ge2_watchdogassert)
	{
		writeLen = 0;
		GE2GPS_LOGD("GE2 watchdog assert test,ge2_watchdogassert=%d",ge2_watchdogassert);
	}
	else
	{
		writeLen = gps_writeSerialData(pGPsState,len);
	}
		gps_heart_test_record_debug(writeLen);
		pGPsState->waitHeartRsp =1;
		pGPsState->HeartCount++;

		GREENEYE2_LOGD(("[GE2]gps_sendHeartTest HeartCount =%d\r\n",pGPsState->HeartCount));
	}
	else
	{
		GREENEYE2_LOGE(("[GE2]gps_sendHeartTest error in send heart tet \r\n "));
	}
	return ;
}

/*****************************************************************************/
//  Description:    gps_redownload
//  Author:         
//  Note:      
/*****************************************************************************/
void gps_redownload()
{
    GpsState*  s = _gps_state;
    GREENEYE2_LOGD(("[GE2]Greeneye2 createDump\n"));
    gps_com_close(s->uart_port);
    SCI_MEMSET(s,0,sizeof(GpsState));
    GPS_Init();
}

/*****************************************************************************/
//  Description:    createDump
//  Author:         
//  Note:      
/*****************************************************************************/
void createDump(GpsState* pState)
{
    GpsState*   s = _gps_state;
    GPS_CREATE_DUMP_T *signal = PNULL;
	
    //SCI_PASSERT(0, ("[GE2]Greeneye2 createDump!"));/*assert to do*/
    //s->started = 0;
    //s->cw_enable = 0;   // HW_CW     
    //s->first_idleoff = 0;
    //s->screen_off = 0;   // SCREENOFF
    //s->gps_flag = CMD_CLEAN;
    s->watchdogAssert = 0x1;
    //gLocationInfo->fix_mode = 0x0;
    DoIdle_UnRegisterCallback ( (DOIDLE_CALLBACK_FUNC) watchdog);
    //gps_task_deinit();
#ifdef GPS_MINI_LOG_SUPPORT
    //gps_log_task_deinit();
#endif
    //gps_com_close(s->uart_port);
   // s->first = 0;
   // s->init = 0;
    gps_uart_dmaclose();

    //s->callbacks.err_occurs_cb();
    SCI_CREATE_SIGNAL(
		signal,
		GPS_CREATE_DUMP_REQ,
		sizeof (GPS_CREATE_DUMP_T),
		SCI_IDENTIFY_THREAD());
		SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, GPS_GetInterfaceThreadId()); 
}

/*****************************************************************************/
//  Description:    recoverDevice
//  Author:         
//  Note:      
/*****************************************************************************/
int recoverDevice(GpsState* pState)
{
    int ret = 0;
    GpsState*   s = _gps_state;
    //SCI_PASSERT(0, ("Greeneye2 recoverDevice!"));/*assert to do*/
    s->started = 0;
    s->cw_enable = 0;   // HW_CW     
    s->first_idleoff = 0;
    s->screen_off = 0;   // SCREENOFF
    s->gps_flag = CMD_CLEAN;
    gLocationInfo->fix_mode = 0x0;
    DoIdle_UnRegisterCallback ( (DOIDLE_CALLBACK_FUNC) watchdog);
    gps_task_deinit();
    #ifdef GPS_MINI_LOG_SUPPORT
    //gps_log_task_deinit();
    #endif
    gps_com_close(s->uart_port);
    s->first = 0;
    s->init = 0;
    gps_uart_dmaclose();

    s->callbacks.err_occurs_cb();
    return ret;
}

/*****************************************************************************/
//  Description:    watchdog
//  Author:         
//  Note:   use for greeneye2 watchdog
/*****************************************************************************/
LOCAL uint32 watchdog(uint32 param)
{
    GpsState*   s = _gps_state;
    extern unsigned long send_length;
    extern unsigned long read_length;
    int dataLen = 0;
    int status = 0;
    int wifiold = 0;
    uint32 start_tick = 0;
    uint32 end_tick = 0;
    static BOOLEAN refresh_tick = SCI_TRUE;
    int watchdog_open = 1;
    GPS_WATCHDOG_T *signal = PNULL;

   static uint32 old_tick = 0x0;
   uint32 new_tick = 0x0;
   uint32 offset = 0x0;
   
   if(old_tick == 0)
   {
   	old_tick = SCI_GetTickCount();
   }
   else
   {
   	new_tick = SCI_GetTickCount();
	offset = new_tick - old_tick;
	if(offset > 15000)
	{
		SCI_PASSERT(0,("do idle timer(%d) is error",offset));
	}
	old_tick = new_tick;
   }

    GREENEYE2_LOGD(("[GE2]watchdog_open PowerState:%s,timeout = %d\r\n",gps_PowerStateName(s->powerState),offset));
    if((s->powerState > DEVICE_DOWNLOADED) && (s->powerState < DEVICE_POWEROFFING))//DEVICE_SLEEP
    {
        if(s->gps_flag == CMD_START)
        {
            GREENEYE2_LOGD(("[GE2]libgps monitor Work,send byte is %ld,read byte is %ld",send_length,read_length));
            s->keepIdleTime = 0; 

                if(DEVICE_RESET == s->powerState)
                {
                    if(0 == recoverDevice(s))
                    {
                        s->HeartCount = 0;
                        s->started = 0;
                        fix_flag = 0;
                        gps_mode = 1024;//hot-start
                        gps_state_start(s);
                    }
                    else
                    {
                        GREENEYE2_LOGD(("[GE2]recoverDevice fail,so return \r\n"));
                        return;
                    }
                }
            }
            else if(s->gps_flag == CMD_STOP)
            {
                if((s->sleepFlag)&& (DEVICE_IDLED == s->powerState))
                {

                    s->keepIdleTime++;
                    GREENEYE2_LOGD(("[GE2]libgps monitor, idltime =%d second\r\n",(s->keepIdleTime * GNSS_MONITOR_INTERVAL_TIME)));

                    if(s->keepIdleTime > (s->sleepTimer>>1))
                    {                        
                        if(DEVICE_SLEEP != s->powerState)
                        {
                            gps_notiySleepDevice(s);
                            //SCI_Sleep(10);
                        }
                        if(s->hotTestFlag)
                        {
                            s->hotTestFlag = 0;
                            GREENEYE2_LOGD(("[GE2]release_wake_lock GNSSLOCK "));
                        }
                        s->keepIdleTime = 0;
                    }

                    if(s->screen_off == 1)
                    {
                        if((s->postlog)||(s->cmcc))//auto test 
                        {
                            GREENEYE2_LOGD(("[GE2]auto test ,so don't send cp sleep"));
                        }
                        else
                        {
                            if(!s->hotTestFlag)
                            {
                                if(DEVICE_SLEEP != s->powerState)
                                {
                                    GREENEYE2_LOGD(("[GE2]it send sleep in  screen off "));
                                    gps_notiySleepDevice(s);
                                    //SCI_Sleep(10);
                                    s->keepIdleTime = 0;
                                }
                                s->keepIdleTime = 0;
                            }
                            else
                            {
                                GREENEYE2_LOGD(("[GE2]it can't send sleep in  screen off when last cold-start"));
                            }
                        }

                    }

            } 
            else if((s->sleepFlag)&& (DEVICE_RESET == s->powerState))
            {
                if(0== recoverDevice(s))
                {
                    s->HeartCount = 0;
                    s->keepIdleTime = 0;
                    fix_flag = 0;
                    gps_mode = 1024;//hot-start
                    if(s->postlog)
                    {
                        GREENEYE2_LOGD(("[GE2]auto test ,so don't send cp sleep"));
                    }
                    else
                    {
                        gps_notiySleepDevice(s);
                        SCI_Sleep(20);
                    }
                }
                else
                {
                    GREENEYE2_LOGD(("[GE2]recoverDevice fail,so return \r\n"));
                    return;
                }
            }
        }
        else
        {
            GREENEYE2_LOGD(("[GE2]libgps monitor, PowerState: %s \r\n",gps_PowerStateName(s->powerState)));
            s->keepIdleTime = 0;
        }
        if((s->watchdog)&& (!s->happenAssert)&&(DEVICE_SLEEP!= s->powerState)&&(!s->watchdogAssert))
        {
            if(s->HeartCount > HEART_COUNT_MAX)
            {
                if(!s->release) //userdebug version 
                {   
                    GREENEYE2_LOGD(("[GE2]the romcode send 0x55, ge2 crash!\r\n"));
		    //SCI_PASSERT(0, ("[GE2]Greeneye2 watchdog assert!"));
                    gps_setPowerState(DEVICE_ABNORMAL);
                    createDump(s);
                    //s->HeartCount = 0; 
                }
                else
                {
                    GREENEYE2_LOGD(("[GE2]%s,gps_wakeupDevice  failed ",__FUNCTION__));
                    gps_setPowerState(DEVICE_RESET);                      
                }
            }
	    else
	    {
		SCI_CREATE_SIGNAL(
			signal,
			GPS_WATCHDOG_REQ,
			sizeof (GPS_WATCHDOG_T),
			SCI_IDENTIFY_THREAD());
		SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, GPS_GetInterfaceThreadId()); 
	    }
        }
    }
	return;
}

/*****************************************************************************/
//  Description:    gps_state_init
//  Author:         
//  Note:   
/*****************************************************************************/
static void  gps_state_init( GpsState*  state )
{
    int  ret;
    char time = 0;
    state->first       =  1;
    state->recurrence = GPS_POSITION_RECURRENCE_PERIODIC;
    state->min_interval = 1;
    state->preferred_accuracy = 50;
    state->preferred_time = 1;
    state->sleepCount = 0;
    state->wakeupCount = 0;
    
    state->m_capabilities = GPS_CAPABILITY_MSA | GPS_CAPABILITY_MSB | GPS_CAPABILITY_ON_DEMAND_TIME | GPS_CAPABILITY_SCHEDULING;    // CTS_TEST add GPS_CAPABILITY_SCHEDULING
    GREENEYE2_LOGD(("[GE2]state->m_capabilities is %d\n", state->m_capabilities));
    state->callbacks.set_capabilities_cb(state->m_capabilities);
    
    state->lastreftime = 0;
    state->lastrefeph= 0;
}

/*****************************************************************************/
//  Description:    gps_sendephdata
//  Author:         
//  Note:   
/*****************************************************************************/
static void  gps_sendephdata()
{
   GpsState*  s = _gps_state;
   gps_send_eph_ge2(s);
}

/*****************************************************************************/
//  Description:    gps_sendwatchdog
//  Author:         
//  Note:   
/*****************************************************************************/
static void  gps_sendwatchdog()
{
   GpsState*  s = _gps_state;
   gps_sendHeartTest(s);
}

/*****************************************************************************/
//  Description:    gps_clearephdata
//  Author:         
//  Note:   
/*****************************************************************************/
static void  gps_clearephdata()
{
   GpsState*  s = _gps_state;
   if(s->cmcc)
   	return;
   gps_clear_ephdata(s);
}

/*****************************************************************************/
//  Description:    gps_clearalmdata
//  Author:         
//  Note:   
/*****************************************************************************/
static void  gps_clearalmdata()
{
   GpsState*  s = _gps_state;
   if(s->cmcc)
   	return;
   gps_clear_almdata(s);
}

/*****************************************************************************/
//  Description:    gps_clearalmdata
//  Author:         
//  Note:   
/*****************************************************************************/
static void  gps_setassert(uint8 assertmode)
{
   GpsState*  s = _gps_state;
   if(assertmode)
   {
   	gps_sendData(SET_ASSERT_STATUS, NULL);
   }
   else
   {
   	ge2_watchdogassert = SCI_TRUE;;
   }
}

/*****************************************************************************/
//  Description:    gps_clearalmdata
//  Author:         
//  Note:   
/*****************************************************************************/
static void  gps_lteopen(uint8 lte_open)
{
   GpsState*  s = _gps_state;
   s->lte_open = lte_open;
   GE2GPS_LOGD("lte_open=%d",s->lte_open);
}

/*****************************************************************************/
//  Description:    gps_init
//  Author:         
//  Note:   
/*****************************************************************************/
static int gps_init(GpsCallbacks* callbacks)
{
	GpsState*  s = _gps_state;
	int ret = 0;
        uint32 start_tick = 0;
        uint32 end_tick = 0;
        uint32 timeout = 0;
	GREENEYE2_LOGD(("[GE2]gps_init enter \r\n"));

    if (!s->first)
    {
        s->callbacks = *callbacks;

        s->hardware_id = GREENEYE2;
        s->uart_port = GPS_COM;
        s->cpmode = 0x0;  //101
        s->logswitch = REFPARAM_GetGpsLogValue();
        GREENEYE2_LOGD(("[GE2]s->logswitch:%d \r\n", s->logswitch));
        s->watchdog = 0x1;
        s->sleepFlag = 0x1;
        s->sleepTimer = 300;
        s->stoptimer = 1;
        s->lte_open = 0x1;
        s->phoneplatform = 0x1;
        s->etuMode = 0;
        s->cw_enable = 0;   // HW_CW
        s->rftool_enable= 0;
        gps_state_init(s);
        //GE2 auto enter sleep mode,so AP should set sleep status
        //download
        gps_setPowerState(DEVICE_SLEEP);
		
	gps_task_init();
#ifdef GPS_MINI_LOG_SUPPORT
	//gps_log_task_init();
#endif
#ifdef GPS_TCARD_LOG_SUPPORT
	Gps_Cardlog_task_init();
#endif
        DoIdle_RegisterCallback ( (DOIDLE_CALLBACK_FUNC) watchdog);

	if(s->dmamutex == NULL)
		s->dmamutex = SCI_CreateMutex ("GE2DmaMutex", SCI_INHERIT);
	
	if(s->ge2sleeptimer == NULL)
		s->ge2sleeptimer = SCI_CreatePeriodTimer("GE2SLEEPTIMER", gps_send_sleep, 0, GE2_TIMER_SLEEP_INTERVAL, SCI_NO_ACTIVATE);
    }
    
    if(s->init)//it avoid the higer layer error
    {
        GREENEYE2_LOGD(("[GE2]gps_init has init \r\n"));
        return -1;
    }

    ret = gps_com_init(s->uart_port);
    if(0 != ret)
    {
        GREENEYE2_LOGE(("[GE2]gps_init open com fail\r\n"));
        return -1;
    }

    PurgeSio (s->uart_port, PURGE_TX_CLEAR | PURGE_RX_CLEAR);
    
    gps_uart_dmastart(s->uart_port);
    
    ret = gps_wakeupDevice(s);
    if(ret)
    {
    	gps_com_close(s->uart_port);
	gps_uart_dmaclose(s->uart_port);
       GREENEYE2_LOGE(("[GE2]gps_wakeupDevice fail\r\n"));
       return -1;
    }

    if(s->logswitch)
    {
        s->outtype = 1;
    }
    else
    {
        s->outtype = 0;
    }
    
    gps_sendData(SET_OUTTYPE,NULL);

    gps_sendData(SET_PHONE_PLATFORM,NULL);
	
    gps_sendData(SET_BASEBAND_MODE,NULL);
    gps_sendData(REQUEST_VERSION,NULL);

    start_tick = SCI_GetTickCount();
    do
    {
        SCI_Sleep(5);
        if(s->cpmode > 0x0)
            break;
        gps_sendData(REQUEST_VERSION,NULL);
        end_tick = SCI_GetTickCount();
        timeout = end_tick - start_tick;
    }while(timeout < 3000);
    if(timeout >= 3000)
    {
        gps_com_close(s->uart_port);
	gps_uart_dmaclose(s->uart_port);
        GREENEYE2_LOGE(("[GE2]GPS_TaskInit fail\r\n"));
        return -1;
    }

    GREENEYE2_LOGD(("[GE2]GPS receive version success\r\n"));

    gps_send_assist_ge2(s);

    gps_sendData(SET_LTE_ENABLE,NULL);
    if((s->postlog) ||(s->cmcc))
    {
        GREENEYE2_LOGD(("[GE2]auto test ,so don't send cp sleep"));
    }
    else
    {
        if(s->sleepFlag&&(DEVICE_IDLED == s->powerState))
        {            
            gps_entersleep();
	    //gps_notiySleepDevice(s);
            SCI_Sleep(10);  
        }
    }
    
    //gps_mode = 125;

    s->ref = 0;
    s->init =  1;
    s->IdlOnCount = 0;
    s->IdlOffCount = 0;
    s->HeartCount = 0;
    s->keepIdleTime = 0;
    s->happenAssert = 0;
    s->hotTestFlag = 0;
    s->gps_flag = CMD_INIT;    // CMD_STOP
    s->etuMode = 0;
    ret = 0;
    return ret;
}

/*****************************************************************************/
//  Description:    gps_state_start
//  Author:         
//  Note:   
/*****************************************************************************/
void  gps_state_start(GpsState*  s)
{
    char cmd = CMD_START;

    gps_cmd_process(cmd);
}

/*****************************************************************************/
//  Description:    gps_start
//  Author:         
//  Note:   
/*****************************************************************************/
int gps_start(void)
{
    GpsState*  s = _gps_state;
    int ret = 0;

    if (!s->init)
    {
        GREENEYE2_LOGE(("[GE2]%s: called with uninitialized state", __FUNCTION__));
        return -1;
    }
    //reset cs mode
    g_startmode_flg = 0;
    g_llh_time_flg  = 0;
    
    s->ref++;
    
    GREENEYE2_LOGD(("[GE2]%s: called,ref is %d", __FUNCTION__,s->ref));
    
    if(s->ref == 1) 
    {
        gps_state_start(s);
		fix_flag = 0;
		gps_start_flag = 1;
	}
	//fix_flag = 0;
	gps_stop_flag = 0;
	return 0;
}

/*****************************************************************************/
//  Description:    gps_state_stop
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_state_stop( GpsState*  s )
{
    char  cmd = CMD_STOP;
    gps_start_flag = 0x0;
    gps_cmd_process(cmd);
}

/*****************************************************************************/
//  Description:    gps_delay_stop
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_delay_stop(GpsState*  s )
{
    
    GREENEYE2_LOGD(("[GE2]gps_delay_stop:s->ref is %d, %d",s->ref));
    if ((s->ref == 0) && (s->gps_flag == CMD_START))
    {
        GREENEYE2_LOGD(("[GE2]%s:send stop cmd",__FUNCTION__));
        gps_state_stop(s);
    }

}

/*****************************************************************************/
//  Description:    gps_stop
//  Author:         
//  Note:   
/*****************************************************************************/
int gps_stop(void)
{

    GpsState*  s = _gps_state;
    unsigned int delay = 0;

    if(s->ref == 0) 
        return 0;
    if (!s->init)
    {
        GREENEYE2_LOGE(("[GE2]%s: called with uninitialized state!!", __FUNCTION__));
        return -1;
    }
    s->ref--;
    GREENEYE2_LOGD(("[GE2]gps_stop,s->ref is %d",s->ref));

    	if ((s->ref == 0) && (s->gps_flag == CMD_START))
		gps_stop_flag = 1;

	if(s->stoptimer > 0)
	{
		SCI_Sleep(s->stoptimer *1000);
		gps_delay_stop(s);
	}
	else
	{
		if(s->ref == 0)
		{
			gps_stop_flag = 1;
			GREENEYE2_LOGD(("[GE2]before send stop command"));
			gps_state_stop(s);
		}
	}

    return 0;
}

/*****************************************************************************/
//  Description:    gps_state_clean
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_state_clean( GpsState*  s )
{
    char  cmd = CMD_CLEAN;
   
    gps_cmd_process(cmd);
}

/*****************************************************************************/
//  Description:    gps_cleanup
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void  gps_cleanup(void)
{
    GpsState*  s = _gps_state;
   
    GREENEYE2_LOGD(("[GE2]%s: called", __FUNCTION__));
	
    if (!s->init)
    {
        GREENEYE2_LOGE(("[GE2]: called with uninitialized state!!"));
        return;
    }
    s->ref = 0;
    s->init = 0;
    GREENEYE2_LOGD(("[GE2]%s: clear init \r\n",__FUNCTION__));
    gps_state_clean(s);
}

/*****************************************************************************/
//  Description:    gps_set_start_mode
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_set_rftestmode(GPS_RF_TEST_MODE_E rf_mode)
{
    gps_rf_testmode = rf_mode;
}

/*****************************************************************************/
//  Description:    gps_get_rftestmode
//  Author:         
//  Note:   
/*****************************************************************************/
GPS_RF_TEST_MODE_E gps_get_rftestmode()
{
    return  gps_rf_testmode;
}

/*****************************************************************************/
//  Description:    gps_set_restart
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_set_restart(uint32 mode)
{
    GpsState*  s = _gps_state;

    s->etuMode = 0;
    s->rftool_enable = 0;
    s->watchdog = 1;

    gps_stop();
    switch(mode)
    {
        case GPS_COLD_MODE:
            gps_mode = 125;
            break;
        case GPS_HOT_MODE:
            gps_mode = 1024;
            break;
        case GPS_WARM_MODE:
            gps_mode = 1;
            break;
        case GPS_TCXO_MODE:
            s->etuMode = 1;
            gps_mode = 50001;
            break;
        case GPS_CW_MODE:
            s->etuMode = 1;
            s->cw_enable = 1;   // HW_CW
            s->watchdog = 0;
            gps_mode = 50002;
        break;
        case GPS_RF_MODE:
            s->etuMode = 1;
            s->rftool_enable = 1;
            s->watchdog = 0;
            gps_mode = 50003;
        break;
        default:
            break;
    }
    gps_start();
    GREENEYE2_LOGD(("[GE2]%s: mode = %d,gps_mode = %d\r\n",__FUNCTION__,mode,gps_mode));
}

/*****************************************************************************/
//  Description:    gps_testmode_enable
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_testmode_enable(uint8 status)
{
    if(status)
    {
       s_testmode_enable = SCI_TRUE;
    }
    else
    {
        s_testmode_enable = SCI_FALSE;
    }
}

/*****************************************************************************/
//  Description:    gps_get_testmode
//  Author:         
//  Note:   
/*****************************************************************************/
BOOLEAN gps_get_testmode()
{
    return s_testmode_enable;
}

/*****************************************************************************/
//  Description:    gps_set_start_mode
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_set_timer_interval(uint32 timer_interval)
{

    GpsState*  s = _gps_state;

    if(timer_interval != s->min_interval)
    {
        s->min_interval = timer_interval;
        gps_sendData(SET_INTERVAL, NULL);
    }

    GREENEYE2_LOGD(("[GE2]%s: timer_interval = %d\r\n",__FUNCTION__,timer_interval));
}

/*****************************************************************************/
//  Description:    gps_writeagpsdata
//  Author:         
//  Note:   
/*****************************************************************************/
#ifdef GPS_AGPS_SUPPORT_QX
void gps_writeagpsdata(char *agps_data,uint32 length)
{
    QX_agnssdata_client_t *response = NULL;
    if(NULL == agps_data)
        return;
    response = (QX_agnssdata_client_t *)agps_data;
    CG_deliverGNSSAssData(agps_data);
}
#else
void gps_writeagpsdata(char *agps_data,uint32 length)
{
    
}
#endif

//the funcitons of _GpsInterface list  end 

static const GpsInterface  _GpsInterface = {
    sizeof(GpsInterface), 
    gps_init,
    gps_start,
    gps_stop,
    gps_cleanup,
    gps_getposition,
    gps_set_restart,
    gps_set_timer_interval,
    gps_writeagpsdata,
    gps_testmode_enable,
    gps_set_rftestmode,
    gps_sendephdata,
    gps_clearephdata,
    gps_sendwatchdog,
    gps_clearalmdata,
    gps_setassert,
    gps_entersleep,
    gps_lteopen,
};

/*****************************************************************************/
//  Description:    gps_get_hardware_interface
//  Author:         
//  Note:   
/*****************************************************************************/
const GpsInterface* gps_get_hardware_interface(struct gps_device_t* dev)
{
	return &_GpsInterface;
}

/*****************************************************************************/
//  Description:    gps_open
//  Author:         
//  Note:   
/*****************************************************************************/
int gps_open(struct gps_device_t **device)
{
    struct gps_device_t *gps_device = SCI_ALLOC(sizeof(struct gps_device_t));
    if (gps_device)
    {
        memset(gps_device, 0, sizeof(struct gps_device_t));
        gps_device->get_gps_interface = gps_get_hardware_interface;

        *device = (struct gps_device_t*)gps_device;
        return 0;
    }
    return 1; 
}

/*****************************************************************************/
//  Description:    gps_task_init
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_task_init(void)
{
    gps_start_thread();
}

/*****************************************************************************/
//  Description:    gps_task_deinit
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_task_deinit(void)
{
    if(s_gps_thread_id != SCI_INVALID_BLOCK_ID)
    {
        SCI_TerminateThread(s_gps_thread_id);
        SCI_DeleteThread(s_gps_thread_id);
        s_gps_thread_id = SCI_INVALID_BLOCK_ID;
    }    
}
#ifdef GE2_ASSERT_DEBUG
GE2_HANDER_TIME_T ge2_hander_time_record = {0};

PUBLIC uint32 ge2_send_read_time_record (void)
{
	GE2_HANDER_TIME_T*  ptr_record = &ge2_hander_time_record;
	uint32  index = 0;
	
	
	if(ptr_record->curr_index[GE2_PROCESS_SENDREAD] >= GE2_HANDER_TIME_MAX)
	{
		ptr_record->curr_index[GE2_PROCESS_SENDREAD] = 0;
	}
	index = ptr_record->curr_index[GE2_PROCESS_SENDREAD];
	ptr_record->send_read_time[index] = SCI_GetTickCount();

	return ptr_record->curr_index[GE2_PROCESS_SENDREAD]++;  
}
PUBLIC void ge2_read_time_record (uint32 index, uint32 process)
{
	GE2_HANDER_TIME_T*  ptr_record = &ge2_hander_time_record;
	uint32	ticket_value = 0;

	ptr_record->curr_index[process] = index;
	
	

	if(GE2_PROCESS_READ == process)
	{
		ptr_record->thread_read_time[index] = SCI_GetTickCount();
		if(ptr_record->thread_read_time[index] >= ptr_record->send_read_time[index])
		{
			ticket_value = ptr_record->thread_read_time[index] - ptr_record->send_read_time[index];
			if(ticket_value >= ptr_record->read_time_max_value)
			{
				ptr_record->read_time_max_value = ticket_value;
				ptr_record->read_time_max_ticket[GE2_PROCESS_SENDREAD] = ptr_record->send_read_time[index];
				ptr_record->read_time_max_ticket[GE2_PROCESS_READ] = ptr_record->thread_read_time[index];
			}
		}
		if(ticket_value > 50)
			ptr_record->read_timeout_count++;

	}
	else if(GE2_PROCESS_READEND == process)
	{
		ptr_record->thread_read_end_time[index] = SCI_GetTickCount();
		if(ptr_record->thread_read_end_time[index] >= ptr_record->send_read_time[index])
		{
			ticket_value = ptr_record->thread_read_end_time[index] - ptr_record->send_read_time[index];
			if(ticket_value >= ptr_record->read_end_time_max_value)
			{
				ptr_record->read_end_time_max_value = ticket_value;
				ptr_record->read_end_time_max_ticket[GE2_PROCESS_SENDREAD] = ptr_record->send_read_time[index];
				ptr_record->read_end_time_max_ticket[GE2_PROCESS_READ] = ptr_record->thread_read_time[index];
				ptr_record->read_end_time_max_ticket[GE2_PROCESS_READEND] = ptr_record->thread_read_end_time[index];
			}
		}
		if(ticket_value > 50)
			ptr_record->read_end_timeout_count++;

	}
	if(ticket_value > 300) //&& SCI_GetTickCount() > 3*60*1000)
	{
		SCI_PASSERT(0, ("[GE2]Greeneye2 receive uart timeout!"));
	}

}
#endif
/*****************************************************************************/
//  Description:    gps_task_entry
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_task_entry(uint32 argc, void *argv)
{
    uint32 u32_actual_event;
    GPS_SIG_T *	signal;
    GpsState*   s =  _gps_state;
    int ret = 0;
    nmea_reader_init( s->NmeaReader );
    nmea_reader_set_callback(s->NmeaReader,&s->callbacks); 
    GNSS_InitParsePacket(&gGPSStream);
    thread_status = thread_run;

    do
    {
        signal = (GPS_SIG_T *) SCI_GetSignal (s_gps_thread_id);
        TM_SendTestPointRequest(signal->SignalCode,96919); 
        switch(signal->SignalCode)
            {
                case GPS_DATA_READ:
#ifdef GE2_ASSERT_DEBUG
					ge2_read_time_record(signal->sig_param,GE2_PROCESS_READ);
                    gps_serialPortRead(s);
					ge2_read_time_record(signal->sig_param,GE2_PROCESS_READEND);
#else
					gps_serialPortRead(s);
#endif
                    UART_Rx_Int_Enable(s->uart_port, TRUE);
                    break;
                 case GPS_DATA_WRITE:
                     break;
                case GPS_DATA_START:
                    break;
                default:
                        break;
            }
        SCI_FREE(signal);
        signal = NULL;
    }while(1);
}

/*****************************************************************************/
//  Description:    gps_assert_entry
//  Author:         
//  Note:   
/*****************************************************************************/
PUBLIC uint32 gps_assert_entry(uint8* input, uint8* output, uint32 src_len)
{
    GpsState*   s =  _gps_state;
	int ret = 0;
	TGNSS_DataProcessor_t* pStream = &gGPSStream;
		
	if(s->happenAssert)
	{
		parse_assert_subtype_data(input, output, src_len, pStream);
		if(pStream->state == GNSS_RECV_SEARCH_FLAG)
		{
			memcpy(output, pStream->cmdData.buff,pStream->cmdData.length);
			ret = pStream->cmdData.length;
		}
	}
	else if(s->watchdogAssert)
	{
		//memcpy(output,input,src_len);
		ret = src_len;
	}
	return ret;

}

/*****************************************************************************/
//  Description:    gps_get_assert_mode
//  Author:         
//  Note:   
/*****************************************************************************/
PUBLIC int gps_get_assert_mode()
{
    GpsState*   s =  _gps_state;
    if(s->happenAssert)
	return 1;
    else if(s->watchdogAssert)
	return 2;
   else
   	return 0;
}

/*****************************************************************************/
//  Description:    gps_start_thread
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_start_thread(void)
{
	if((SCI_INVALID_BLOCK_ID == s_gps_thread_id) || (SCI_NULL == s_gps_thread_id))
	{
		s_gps_thread_id = SCI_CreateThread("T_GPS_SCHED", "Q_GPS_SCHED", gps_task_entry, 0, NULL, GPS_THREAD_STACK_SIZE, 
								GPS_THREAD_QUEUE_NUM, s_gps_thread_task_priority, SCI_PREEMPT, SCI_AUTO_START);

		SCI_ASSERT(SCI_INVALID_BLOCK_ID != s_gps_thread_id);
	}
    
}

/*****************************************************************************/
//  Description:    gps_cmd_process
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_cmd_process( char cmd )
{
    GpsState*  s = _gps_state;
    int i,ret;
    char error_time = 0;
    char sig_first = 0;

    extern unsigned long send_length;
    extern unsigned long read_length;

    switch(cmd)
    {
        case CMD_START:
            if(!s->started)
            {
            	GE2GPS_LOGD("gps start,stop sleep timer");
            	SCI_DeactiveTimer(s->ge2sleeptimer);
                GREENEYE2_LOGD(("[GE2]LIBGPS VERSION:%s",libgps_version));
                gLocationInfo->fix_mode = 0x0;
		gLocationInfo->ttff = 0x0;
		ge2_sendephdata = SCI_TRUE;
	    	ge2_storeeph = SCI_FALSE;
		SCI_MEMSET(&ge2fixtime,0,sizeof(ge2fixtime));
                #ifdef GPS_TCARD_LOG_SUPPORT
                Gps_CardLogOpen(GPS_TCARDLOG_TYPE_ALL);
                #endif
		s->requestephmode = 0x0;
		//gps_uart_dmastart(s->uart_port);
                s->gps_flag = CMD_START;
                ret = gps_wakeupDevice(s);
                if(ret)
                {
                    if(!s->release) //userdebug
                    {
                        gps_setPowerState(DEVICE_ABNORMAL);
                        createDump(s);
                        //continue;
                    }
                    else
                    {
                        GREENEYE2_LOGD(("[GE2]%s,gps_wakeupDevice  failed ",__FUNCTION__));
                        gps_setPowerState(DEVICE_RESET); 
                        //continue;
                    }
                }
                send_length = 0;
                read_length = 0;
                s->IdlOffCount++;
                s->started = 1;
		s->requestephmode = 0x0;
                //open LNA????

                //first check power state ,if should not keep sleep state 
                if(s->logswitch)
                {
                    s->outtype = 1;
                }
                else
                {
                    s->outtype = 0;
                }
		gps_sendData(SET_LTE_ENABLE,NULL);
                gps_sendData(SET_OUTTYPE,NULL); 
                GREENEYE2_LOGD(("[GE2]cpmode is:%d:",s->cpmode));
        	gps_sendData(SET_CPMODE,NULL);               
        	gps_sendData(SET_INTERVAL, NULL);	 // CTS_TEST
                GREENEYE2_LOGD(("[GE2]gps Real send IDLEOFF is:%d:",s->IdlOffCount));
                
                gps_test_mode(s);

                ret = 1;
                i = 0;
                gps_setPowerState(DEVICE_WORKING);
                while(ret && (i < 3))
                {
                    i++;
                    gps_sendData(IDLEOFF_STAT, gps_idle_off);
                    SCI_Sleep(200);
                    ret =gps_power_Statusync(IDLE_OFF_SUCCESS);
                }       
            }
            break;
        case CMD_STOP:
		ge2_sendephdata = SCI_FALSE;
                if(s->started)
                {
                    GREENEYE2_LOGD(("[GE2]=== cmd stop enter\n"));
                    ret = 1;
                    i = 0;
                    s->gps_flag = CMD_STOP;
                    s->started = 0;
                    s->IdlOnCount++;
                    s->cw_enable = 0;   // HW_CW             
                    GREENEYE2_LOGD(("[GE2]gps Real send IDLON is:%d:",s->IdlOnCount));
                    gps_setPowerState(DEVICE_IDLING);
                    while(ret &&(i < 3))
                    {
                        i++;
                        gps_sendData(IDLEON_STAT, gps_idle_on);
                        SCI_Sleep(200);
                        ret = gps_power_Statusync(IDLE_ON_SUCCESS);
                    }

                    s->etuMode = 0;
                    s->rftool_enable = 0;
                    s->watchdog = 1;

                    //disable LNA???

                    GREENEYE2_LOGD(("[GE2]gps_stop ,send cp sleep"));
                    gps_notiySleepDevice(s);
                    SCI_Sleep(10);

                    s->keepIdleTime = 0;  						
                    gLocationInfo->fix_mode = 0x0;
		    gps_saveEphToFlash();
		    //gps_uart_dmaclose();
                }
            break;
        case CMD_CLEAN:
                  if(s->started == 1)
                  {
                      GREENEYE2_LOGE(("[GE2]=========>>>>>>>>>>>note:gps_stop isn't run!!!"));                   
                      s->IdlOnCount++;
                      i = s->started = 0; 
                      ret = 1;
                      gps_setPowerState(DEVICE_IDLING);
                      while(ret &&(i < 3))
                      {
                          i++;
                          gps_sendData(IDLEON_STAT, gps_idle_on);
                          SCI_Sleep(200);
                          ret = gps_power_Statusync(IDLE_ON_SUCCESS);
                      }
                  }
            	  GE2GPS_LOGD("gps clean,stop sleep timer");
		  SCI_DeactiveTimer(s->ge2sleeptimer);
                  GREENEYE2_LOGD(("[GE2]cleanup  send cp sleep"));
                  if(DEVICE_SLEEP != s->powerState)
                  {
                      //gps_notiySleepDevice(s);
                  }
                  SCI_Sleep(40);//it dealy wait the cp ready 
                  sig_first = 0;
                  s->first_idleoff = 0;
                  s->screen_off = 0;   // SCREENOFF
                  s->gps_flag = CMD_CLEAN;
                  gLocationInfo->fix_mode = 0x0;
                  gps_saveEphToFlash();
                  DoIdle_UnRegisterCallback ( (DOIDLE_CALLBACK_FUNC) watchdog);
                  gps_task_deinit();
                  #ifdef GPS_MINI_LOG_SUPPORT
                  //gps_log_task_deinit();
                  #endif
                  //gps_com_close(s->uart_port);
                  s->first = 0;
                  s->init = 0;
				  if(s->dmamutex)
				  {
				       SCI_DeleteMutex(s->dmamutex);
					   s->dmamutex = NULL;
				  }
                    #ifdef GPS_TCARD_LOG_SUPPORT
                    Gps_CardLogClose(GPS_TCARDLOG_TYPE_ALL);
                    #endif
		   gps_uart_dmaclose();
                  //disable lan???
            break;
            case CMD_QUIT:
                break;
            default:
                s->gps_flag = CMD_QUIT;
                break;
    }  
}

/*****************************************************************************/
//  Description:    ge2_uart_callback
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_uart_callback(uint32 event)
{    
    GpsState*   s =  _gps_state;
    switch (event)
    {
        case EVENT_DATA_TO_READ:    
			{ 
				UART_Rx_Int_Enable(s->uart_port, FALSE);
#ifdef GE2_ASSERT_DEBUG
				gps_sendmsg(GPS_DATA_READ,ge2_send_read_time_record());
#else
				gps_sendmsg(GPS_DATA_READ,0);
#endif
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

/*****************************************************************************/
//  Description:    gps_com_init
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL int gps_com_init(int uart_port)
{
    int ret; 
    
    UART_INIT_PARA_T    uart_st;
    uart_st.tx_watermark  = GPS_TX_WATER_MARK; // 0~127B
    uart_st.rx_watermark  = GPS_RX_WATER_MARK; // 0~127B
    uart_st.baud_rate     = BAUD_3250000;
    uart_st.parity        = FALSE; 
    uart_st.parity_enable = PARITY_DISABLE;
    uart_st.byte_size     = EIGHT_BITS;
    uart_st.stop_bits     = ONE_STOP_BIT;
    uart_st.flow_control  = HW_FLOW_CONTROL;
    uart_st.ds_wakeup_en  = DS_WAKEUP_ENABLE;
    uart_st.ds_wakeup_type = DS_WAKEUP_BY_RXD; 

    ret = UART_Initilize(uart_port, &uart_st, gps_uart_callback);

    UART_Tx_Int_Enable(uart_port, FALSE);
    return ret;
}

/*****************************************************************************/
//  Description:    gps_com_reset
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_com_reset (int uart_port)
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

/*****************************************************************************/
//  Description:    gps_com_close
//  Author:         
//  Note:   
/*****************************************************************************/
int gps_com_close(int uart_port)
{
    UART_INIT_PARA_T    uart1_st;
        
    uart1_st.tx_watermark  = GPS_TX_WATER_MARK; // 0~127B
    uart1_st.rx_watermark  = GPS_RX_WATER_MARK; // 0~127B
    uart1_st.baud_rate     = BAUD_115200;
    uart1_st.parity        = FALSE; 
    uart1_st.parity_enable = PARITY_DISABLE;
    uart1_st.byte_size     = EIGHT_BITS;
    uart1_st.stop_bits     = ONE_STOP_BIT;
    uart1_st.flow_control  = NO_FLOW_CONTROL;
    uart1_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;

    gps_com_reset(uart_port);

    return UART_Close(uart_port);
}

/*****************************************************************************/
//  Description:    gps_getposition
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL uint32 gps_getposition(char **pbuf)
{
    uint32 ret = 0;
    GREENEYE2_LOGD(("[GE2]===>>>getposition:fixflag=%d",fix_flag));
    if(fix_flag == 1)
    {
        *pbuf = (char *)gLocationInfo;
        ret = sizeof(Ge2PositionParam_t);
    }
    return ret;
}

/*****************************************************************************/
//  Description:    gps_sendmsg
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_sendmsg(GPSDATACMD_TYPE sig,int sig_param)
{
    GPS_SIG_T *signal = PNULL;

    SCI_CREATE_SIGNAL(
        signal,
        sig,
        sizeof (GPS_SIG_T),
        SCI_IDENTIFY_THREAD());
    
    signal->sig_param = sig_param;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_gps_thread_id);   
}
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

