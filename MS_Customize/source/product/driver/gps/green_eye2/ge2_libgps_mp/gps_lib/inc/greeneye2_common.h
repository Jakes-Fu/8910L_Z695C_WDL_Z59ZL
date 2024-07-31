/******************************************************************************
 ** File Name:      greeneye2_common.h                                                    *
 ** Author:                                                     *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE          	NAME             	DESCRIPTION                               *
 **                                   *                                *
 ******************************************************************************/

#ifndef GREENEYE2_COMMON_H
#define GREENEYE2_COMMON_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "greeneye2.h"
#include "greeneye2_type.h"
#include "greeneye2_gnss_libgps_api.h"
#include "nv_item_id.h"
#include "nvitem.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
#define  GPS_DEBUG   0    
#define  GPS_LTE     0
#define AGPS_PARAM_CONFIG
#define AGPS_LTE 1
#define GNSS_REG_WR

#define UPDATE_BIN  0
#define GNSS_DOWNLOAD_OPT 1


#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define GE2_TIMER_SLEEP_INTERVAL 30000

#define  LOG_TAG  "LIBGPS"

#define CTRL_MODE 1
#define SUPL_MODE 0

#define  MAX_NMEA_TOKENS  32
//#define  NMEA_MAX_SIZE  1023
#define TOKEN_LEN(tok)  (tok.end>tok.p?tok.end-tok.p:0)
#define SERIAL_READ_BUFFER_MAX       (1024) //todo 
#define SERIAL_WRITE_BUFFER_MAX      (2048) //todo 
#define GNSS_LOG_BUFFER_MAX          (512) //todo 

#define GPS_RECV_MAX_LEN                 (4294967200UL)
#define GPS_SEND_MAX_LEN                 (4294967200UL)

#define POWER_ERROR_NUMBER_MAX        (3)
#define SEND_REPEAT_COUNT_MAX         (3)

//for boot add by zxw
#define START_DOWNLOAD_FD1 0x01
#define START_DOWNLOAD_GE2 0x02
#define CLOSE_DEVICE       0x03
#define DEVICE_EXCEPTION   0x04
#define DOWNLOAD_FAIL      0x05
#define GET_GNSS_STATUS    0x06

#define GNSS_SOCKET_NAME "gnss_server"
#define WCND_SOCKET_NAME    "wcnd" 

#define GNSS_DOWNLOAD_TIMEOUT  (12) // 6 seconds 



#define WCN_CMD_REBOOT_GNSS		"rebootgnss"
#define WCN_CMD_DUMP_GNSS		"dumpgnss"
#define WCN_CMD_START_GNSS		"startgnss"
#define WCN_CMD_STOP_GNSS			"stopgnss"
#define WCN_RESP_REBOOT_GNSS		"rebootgnss-ok"
#define WCN_RESP_DUMP_GNSS		"dumpgnss-ok"
#define WCN_RESP_START_GNSS		"startgnss-ok"
#define WCN_RESP_STOP_GNSS		"stopgnss-ok"
#define WCN_RESP_STATE_GNSS		"gnssstate-ok"

#define GET_XML 0x00
#define SET_XML 0x01
#define SUPL_XML_PATH           "/data/gnss/supl/supl.xml"
#define CONFIG_XML_PATH         "/data/gnss/config/config.xml"
#define RAWOBS_PATH             "/data/gnss/lte/raw.obs"
#define GE2_FDL_PATH            "/data/gnss/bin/gnssfdl.bin"


#define SUPL_ETC_PATH          "/system/etc/supl.xml"
#define CONFIG_ETC_PATH        "/system/etc/config.xml"
#define SPREADORBIT_ETC_PATH   "/system/etc/jpleph.405"
#define RAWOBS_ETC_PATH         "/system/etc/raw.obs"
#define GE2_FIRMWARE_ETC_PATH   "/system/etc/gnssmodem.bin"
#define GE2_FDL_ETC_PATH        "/system/etc/gnssfdl.bin"
#define AGNSS_CER_ETC_PATH      "/system/etc/spirentroot.cer"

#define SUPL_TYPE         0
#define CONFIG_TYPE       1
#define SPREADORBIT_TYPE  2
#define FIRMWARE_TYPE     3
#define FDL_TYPE          4
#define RAWOBS_TYPE       5
#define CER_TYPE          6

#define GPS_GLONASS       0
#define GPS_BDS           1

#define PI 3.14159265358979323846
#define WGS_AXIS_A  6378137.0
#define WGS_E1_SQR  0.006694379990141317
#define EARTH_R 6371.004
#define ABS(x)  (((x) > 0 ? (x): -(x)))

//agps session protection macro and constaint define
#define AGPS_SESSION_START   (1)
#define AGPS_SESSION_STOP    (2)
#define AGPS_SESSION_TIMEOUT (3)

#define AGPS_GLONASS_EPH_MAX_COUNT  (28)   //28
#define AGPS_SESSION_TIMEOU_SECONDS (3600) //one hour
#define GPS_WEEK_NO_SECONDS         (604800) //one hour

#define TRACE_OFFSET       544*1024
#define FORMAT_MAX_LENTH   8
#define LINE_MAX_NUM       2000

typedef struct ttf{
	unsigned int weekno;
	unsigned int mscount;
}gpstime;

typedef struct {
    int     prn;
    float   snr;
} RawSvData;

typedef struct {
    int     	num_svs;
    RawSvData   sv_list[50];
} RawGpsSvInfo;
//RawGpsSvInfo sv_rawinfo;

typedef enum 
{
	NV_GPS_AGPS_PARAM = NV_GPS_GE2_BASE,
	NV_GPS_EPHEMERIS_PARAM,
	NV_GLO_EPHEMERIS_PARAM,
	NV_BD_EPHEMERIS_PARAM,
	#if 0
	NV_GPS_PVTMISC_PARAM,
	NV_GPS_GPSEPH1_PARAM,
	NV_GPS_GPSEPH2_PARAM,
	NV_GPS_GPSEPH3_PARAM,
	NV_GPS_GPSEPH4_PARAM,
	NV_GPS_GPSALM_PARAM,
	NV_GPS_GPSCPY1_PARAM,
	NV_GPS_GPSCPY2_PARAM,
	NV_GPS_GPSCPY3_PARAM,
	NV_GPS_GPSCPY4_PARAM,
	NV_GPS_GPSCPY5_PARAM,
	NV_GPS_GPSCPY6_PARAM,
	NV_GPS_QZSEPH_PARAM,
	NV_GPS_BDEPH1_PARAM,
	NV_GPS_BDEPH2_PARAM,
	NV_GPS_BDALM_PARAM,
	NV_GPS_GLOEPH1_PARAM,
	NV_GPS_GLOEPH2_PARAM,
	NV_GPS_GLOEPH3_PARAM,
	NV_GPS_GLOEPH4_PARAM,
	NV_GPS_GLOALM1_PARAM,
	NV_GPS_GLOALM2_PARAM,
	NV_GPS_IONO_PARAM,
	NV_GPS_ALMCPY_PARAM,
	NV_GPS_SEQLSQ_PARAM,
	NV_GPS_BLUNDER_PARAM,
	#endif
	NV_GPS_MAX_PARAM = NV_GPS_GE2_MAX     //must be less than 50
}GPS_FLASHPARAM_T;

//end
typedef enum DevicepowerState
{
	DEVICE_DOWNLOADING = 1,
	DEVICE_DOWNLOADED,
	DEVICE_POWERONING,
	DEVICE_POWERONED,
	DEVICE_WORKING,
	DEVICE_WORKED,
	DEVICE_IDLING,
	DEVICE_IDLED,
	DEVICE_SLEEPING,
	DEVICE_SLEEP,
    DEVICE_WAKEUPING,
    DEVICE_RESET,
	DEVICE_POWEROFFING,
	DEVICE_POWEROFFED,
	DEVICE_ABNORMAL
}TDevicepowerState;

typedef struct {
    const char*  p;
    const char*  end;
} Token;


typedef struct {
    int     count;
    Token   tokens[ MAX_NMEA_TOKENS ];
} NmeaTokenizer;

#if 0
struct timeout_actions_elem {
    int inuse;
    int id;
    time_t tout;
    void (*timeout_cb)(int, void *data);
    void *data;
}; 
#endif

typedef struct {
    int     pos;
    int     overflow;
    int     utc_year;
    int     utc_mon;
    int     utc_day;
    GpsLocation  fix;
    GpsSvStatus  svstatus; 
    int svinfo_flags;
    gps_location_callback  location_callback;
    gps_sv_status_callback  sv_status_callback;
    char    in[ NMEA_MAX_SIZE+1 ];
} NmeaReader;

typedef enum{
   LIBGPSTYPE,
   SUPLTYPE,
   CONTROLTYPE,
   ENGINETYPE
}GPSTYPE;

typedef enum
{
    DUMP_RECV_START_UNKOWN,
    DUMP_RECV_CODE_START,
    DUMP_RECV_CODE_END,//it qual data start
    DUMP_RECV_DATA_END,
    DUMP_RECV_MAX,
}DUMP_RecvState_e;


typedef struct {
	//global state value
	int      init;
	int      first;   
	int      first_idleoff;
	int      fd;
	int      powerctrlfd;
	int      gnsslogfd;
	int      slog_switch;
	int      slogmodem_fd;
	int      hardware_id;
    int      getetcconfig;
    unsigned char  sleepCount;
    unsigned char  wakeupCount;
	//char serialName[22];//fd  string name 
	int uart_port;
	//char binPath[128];
	//	int readBuffersize;
	unsigned char readBuffer[SERIAL_READ_BUFFER_MAX];
	unsigned char writeBuffer[SERIAL_WRITE_BUFFER_MAX];
	unsigned char gnsslogBuffer[512];
	int  writeoffsize;
	int  sendLen;
	int  sendFlag; 
//	TCircular_buffer serialWrite;
	//pthread_mutex_t  writeMutex; 
	NmeaReader  NmeaReader[1];
	int      ref;
	//for set mode param
	GpsPositionRecurrence   recurrence;
	int      min_interval;
	int      preferred_accuracy;
	int      preferred_time;
	int      fix_frequency; 
	//for supl server and port
	char     suplhost[80];
	int      suplport;
	//for callback
	GpsCallbacks         callbacks;
	//AGpsCallbacks        agps_callbacks;
	//GpsNiCallbacks       ni_callbacks;
	//AGpsRilCallbacks     agps_ril_callbacks;
	//GpsXtraCallbacks     xtra_callbacks;
	//GpsGeofenceCallbacks geofence_callbacks;
	//for data conn,zhouxw add
	int  connected;
	int  type;
	int  avaiable;
	int  wifi_ok;
	int  m_capabilities;
	int  gps_flag; 
	int  gps_open;
    int  started;
	int  fix_stat;
	int  logswitch;
	int  workmode;
	//control sock
	int     control[2];
	int     asock[2];
	//for print debug,can remove
	int     cmd_flag;
	int     cpmode;
	int     outtype;
	int     lte_open;
	int     lte_running;
	int     realeph;
	int     cmcc;
	int     IdlOnCount;
	int     IdlOffCount;
	int     HeartCount;
	int     waitHeartRsp;
	int     keepIdleTime;
	int     postlog;
	int     watchdog;
	int     imgmode;
	int     sleepFlag;
	int     sleepTimer ;
	int     stoptimer;
	int     happenAssert; 
	//it don't send sleep command in  5 minuter when laster cold-start
	int     hotTestFlag;
	//these is for power value
	TDevicepowerState powerState;// the device power state 
	int     powerErrorNum;
	//sem_t powersync;  //power state sync
	//FILE*  fFlashCfg;
	//GNSS_REG_WR
	unsigned int wRegAddr;
	unsigned int wRegValue;
	unsigned int rRegAddr;
	unsigned int rRegValue;
	int  screen_off;    // SCREENOFF
	int  release; 
    //AGPS Timeout protection
    int agps_session;
    uint32 lastreftime;
    uint32 lastrefeph;
	
	int tsx_enable;
	int tsxIsdeliver;
    int etuMode;
	double tsx_temp;   // TSXTEMP
	int    cw_enable;	// HW_CW
	int rftool_enable;
	int float_cn_enable;
    int baseband_mode;
    int phoneplatform;    //0x1:feature phone
    int requestephmode;
	SCI_MUTEX_PTR dmamutex;
   int watchdogAssert;
   SCI_TIMER_PTR  ge2sleeptimer;
} GpsState;

#define MSB_SI       0
#define MSB_NI       1
#define MSA_SI       2
#define MSA_NI       3
#define GEO_MSB_SI   4
#define GEO_MSB_NI   5
#define GEO_MSA_SI   6
#define GEO_MSA_NI   7
#define PEROID_SI    8
#define PEROID_NI    9
#define CP_SI        10
#define CP_NI        11

typedef struct LLC
{
	unsigned int   weekNo;         /*< week number (from beginning of current week cycle) */
	unsigned int    second;          /*<  seconds (from beginning of current week no) */
	unsigned int    secondFrac;      /*<  seconds fraction (from beginning of current second) units: 1ns */
	double lat;
	double lon;
	double hae;
} WLLH_TYPE;

typedef struct __TAgpsMsrElement_t 
{
    unsigned char  systemid;     //0-gps,1-glonass,2-beidou
    unsigned char  satelliteID;  // range: 0-63
    int            cNo;          // carrier noise ratio, range:(0..63)
    float          doppler;      // doppler, range:(-32768/5..32767/5)
    int            wholeChips;   // whole value of the code phase measurement, range:(0..1022)
    int            fracChips;    // fractional value of the code phase measurement, range:(0..1023),
    int            mpathInd;     // multipath indicator
    int            pseuRangeRMSErr; // index, range:(0..63)

	unsigned long long flags;
	long long	received_gps_tow_ns;
	long long	received_gps_tow_uncertainty_ns;
	double		pseudorange_m;
	double		pseudorange_uncertainty_m;
	float		carrier_frequency_hz; // default L1

	unsigned int carrier_count;
	double		carrier_phase;
	int 		bit_number; //Total bit number since midnight
	short		time_from_last_bit_ms; // how many ms in one bit
	double		elevation_deg;
	double		elevation_uncertainty_deg;
	double		azimuth_deg;
	double		azimuth_uncertainty_deg;
	int 		used_in_fix;
} TAgpsMsrElement_t;

typedef struct __TAgpsMsrSetList_t
{
    int                 gpsTOW;    // unit:ms
    unsigned char       listNums;  // range:0-16
    TAgpsMsrElement_t   msrElement[14];  // modify 16 to 14
}TAgpsMsrSetList_t;

typedef struct __TAgpsMsrElement_rawt
{
    unsigned char  systemid;     //0-gps,1-glonass,2-beidou
    unsigned char  satelliteID;  // range: 0-63
    int            cNo;          // carrier noise ratio, range:(0..63)
    float          doppler;      // doppler, range:(-32768/5..32767/5)
    int            wholeChips;   // whole value of the code phase measurement, range:(0..1022)
    int            fracChips;    // fractional value of the code phase measurement, range:(0..1023),
    int            mpathInd;     // multipath indicator
    int            pseuRangeRMSErr; // index, range:(0..63)
} TAgpsMsrElement_raw_t;

typedef struct __TAgpsMsrSetList_rawt
{
    int                 gpsTOW;    // unit:ms
    unsigned char       listNums;  // range:0-16
    TAgpsMsrElement_raw_t   msrElement[14]; // todo  change 14
}TAgpsMsrSetList_raw_t;

typedef struct {
	int  bdsURAI;
	int  bdsToe;   /* 2^3  sec*/
	unsigned int  bdsAPowerHalf;  /*2^-19   meters^1/2*/
	unsigned int  bdsE;  /*2^-33  */
	int  bdsW;  /*2^-31   semi-circles */
	int  bdsDeltaN; /*2^-43   semi-circles/sec */
	int  bdsM0;  /*2^-31   semi-circles */
	int  bdsOmega0;  /*2^-31   semi-circles */
	int  bdsOmegaDot; /*2^-43   semi-circles/sec */
	int  bdsI0;  /*2^-31   semi-circles */
	int  bdsIDot;  /*2^-43   semi-circles/sec */
	int  bdsCuc;  /*2^-31  radians*/
	int  bdsCus;  /*2^-31  radians*/
	int  bdsCrc;  /*2^-6  meters*/
	int  bdsCrs;  /*2^-6  meters*/
	int  bdsCic;  /*2^-31  radians*/
	int  bdsCis;  /*2^-31  radians*/
} *PBDS_EPHEMERIS,BDS_EPHEMERIS;

typedef struct{
	int  bdsToc;  /* 2^3  seconds */
	int  bdsA0;   /* 2^-33  sec/sec */
	int  bdsA1;   /* 2^-50  sec/sec^2 */
	int  bdsA2;    /* 2^-66  seconds */
	int  bdsTgd1;  /* 10^-10  seconds */
} *PBDS_CLOCK_EPHEMERIS,BDS_CLOCK_EPHEMERIS;

extern GpsState  _gps_state[1];
//extern AgpsData_t agpsdata[1]; 
//extern int sid_agps;

extern char data_exist_flag;
extern int   gWriteFlag;
extern int    gDropCount;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
void test_gps_mode(GpsState *s);
void  gps_state_start(GpsState*  s);
int gps_power_Statusync(int value);
int gps_writeSerialData(GpsState* s,int len);
void gps_sendData(int flag, const char* pBuf);
void gps_setPowerState(int state);
const char* gps_PowerStateName(int state);
int gps_SendNotify(GpsState *s,int flag);
void  gps_dispatch(TCmdData_t* pPacket);
void  nmea_reader_parse( NmeaReader*  r );
int OsTimeSystemGet( gpstime *aGpsTime);
int gps_wakeupDevice(GpsState* s);
void  gps_notiySleepDevice(GpsState *s );
//int gps_adingData(TCmdData_t *pPak);
int recoverDevice(GpsState* pState);
void createDump(GpsState* pState);
void parseNmea(unsigned char *buf,int len);
void nmea_parse_cp(unsigned char *buff,unsigned short lenth);
int s2int(char *p, char len);
void  nmea_reader_addc( NmeaReader*  r, int  c );

void gps_uart_dmastart(int uart_port);
void gps_uart_dmaclose();
void gps_send_assist_ge2(GpsState* s);
void gps_send_eph_ge2(GpsState* s);
void gps_saveEphToFlash();
void gps_entersleep();
void gps_send_sleep(uint32 state);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#endif //GREENEYE2_COMMON_H
