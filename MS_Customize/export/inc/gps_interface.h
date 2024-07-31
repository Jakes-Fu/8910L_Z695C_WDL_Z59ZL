/******************************************************************************
 ** File Name:      gps_interface.h                                                 *
 ** Author:                                                      *
 ** DATE:                                                           *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of GPS   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/26/2007     Liangwen.Zhen/David.Jia    Create.                         *
 ******************************************************************************/
#ifndef _GPS_INTERFACE_H_
#define _GPS_INTERFACE_H_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "os_api.h"
#include "cmddef.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Enum Definition                                   *
 **---------------------------------------------------------------------------*/
 #if 0
typedef enum
{
    GPS_DOWNLOAD_CNF,
    GPS_INIT_CNF,
    GPS_START_CNF,
    GPS_STOP_CNF ,
    GPS_RESET_CNF,
    GPS_READ_STATUS_CNF,
    GPS_READ_INFO_CNF,
    GPS_OUTPUT_CNF,
    GPS_OUTPUT_INFO_IND,
    GPS_POWEROFF_CNF,
    GPS_END_CNF_SIGNAL
}GPS_CNF_SIGNAL_T;

typedef enum
{
    GPS_DOWNLOAD_REQ,
    GPS_INIT_REQ,
    GPS_START_REQ,
    GPS_STOP_REQ,
    GPS_RESET_REQ,
    GPS_READ_STATUS_REQ,
    GPS_READ_INFO_REQ,
    GPS_OUTPUT_REQ,
    GPS_POWEROFF_REQ,
    GPS_END_REQ_SIGNAL 
}GPS_REQ_SIGNAL_T;

typedef enum
{
    GPS_NOT_RUN,   
    GPS_NOT_FIX,      
    GPS_2D_FIX,    
    GPS_3D_FIX
} GPS_FIX_STATUS_E;

typedef struct
{
    SIGNAL_VARS
    GPS_FIX_STATUS_E     status;
} GPS_STATUS_CNF_T;

typedef enum 
{
    GPS_COLD_MODE,      //冷启动
    GPS_HOT_MODE,       //热启动
    GPS_WARM_MODE    //温启动
} GPS_RESET_MODE_E;

// following signal only have signal header
typedef xSignalHeaderRec  SIGNALS_NO_PARA_T;

typedef struct 
{
    SIGNAL_VARS
    GPS_RESET_MODE_E     reset_mode;
} GPS_RESET_REQ_T;


typedef struct 
{
    SIGNAL_VARS
    uint16     type;
    uint32          time_interval;
} GPS_OUTPUT_REQ_T;


typedef struct
{
    SIGNAL_VARS
    BOOLEAN     is_ok;
} GPS_DOWNLOAD_CNF_T,
GPS_INIT_CNF_T,
GPS_START_CNF_T, 
GPS_STOP_CNF_T, 
GPS_RESET_CNF_T,
GPS_OUTPUT_CNF_T,
GPS_POWEROFF_CNF_T;

typedef struct {
    uint16      wYear;
    uint16      wMonth;
    uint16      wDay;
    uint16      wHour;
    uint16      wMinute;
    uint16      wSecond;
} TimeStamp;

typedef struct 
{
    double      longitude;  //经度
    double      latitude;   //纬度
    TimeStamp      utc_time;  // 世界时间
    uint32       ttff_time; //第一次定位所用时间，单位s
    uint8         satellite_num; //卫星数目
    double       speed;    //对地速度
    double      course;   //对地方向
} GPS_FIX_INFO_T;

typedef struct
{
    SIGNAL_VARS
    GPS_FIX_INFO_T       gps_info;
} GPS_READ_INFO_CNF_T;

typedef struct 
{
    SIGNAL_VARS
    uint16     length;
    uint8       type; // 2--GGA, 4--GLL, 8--GSA, 16--GSV, 32--RMC, 64--VTG, //128--ZDA
    uint8       nmea_data[512]; //NMEA格式数据
} GPS_OUTPUT_INFO_IND_T;
#endif

typedef enum 
{
    GPS_COLD_MODE,      //冷启动
    GPS_HOT_MODE,       //热启动
    GPS_WARM_MODE,    //温启动
    GPS_TRACE_MODE,
    GPS_FACTORY_MODE,
    GPS_TCXO_MODE,
    GPS_TSX_MODE,
    GPS_CW_MODE,
    GPS_RF_MODE,
    GPS_MAX_MODE
} GPS_RESET_MODE_E;

typedef enum 
{
    GPS_RF_GPS,
    GPS_RF_GLO,
    GPS_RF_BDS,
    GPS_RF_MAX_MODE
} GPS_RF_TEST_MODE_E;

typedef struct
{
    MSG_HEAD_T  msg_head;
    uint8      nmealog[1024]; //NMEA格式数据
}GPS_NMEALOG_OUTPUT_T;

typedef struct 
{
    SIGNAL_VARS
    GPS_NMEALOG_OUTPUT_T nmealog;
} GPS_DIAG_OUTPUT_NMEALOG_T;

#pragma pack(1)
typedef struct 
{
    uint8 num_svs;
    uint8 snr[256];
}GPS_OUTPUT_SVINFO_T;
#pragma pack()

typedef struct 
{
    MSG_HEAD_T  msg_head;
    GPS_OUTPUT_SVINFO_T svinfo;
} GPS_DIAG_OUTPUT_SVINFO_T;

typedef struct
{
    SIGNAL_VARS
} GPS_GET_AGPSDATA_T,
GPS_FIX_SUCCESS_T,
GPS_GET_EPHDATA_T,
GPS_WATCHDOG_T,
GPS_CREATE_DUMP_T,
GPS_ENTER_SLEEP_T,
GPS_REPORT_AGPSDATA_T,
GPS_AGPSDATA_INIT_T;

typedef enum 
{
    GPS_GET_AGPSDATA_REQ = 0xA0,      //冷启动
    GPS_FIX_SUCCESS_REQ,       //热启动
    GPS_GET_EPHDATA_REQ,    //温启动
    GPS_WATCHDOG_REQ,
    GPS_CREATE_DUMP_REQ,
    GPS_ENTER_SLEEP_REQ,
    GPS_REPORT_AGPSDATA_REQ,
    GPS_AGPSDATA_INIT_REQ,
    GPS_FACTORY_INIT_REQ,
    GPS_FACTORY_RESTART_REQ,
    GPS_MAX_REQ
} GPS_INTERFACE_REQ_E;

PUBLIC BLOCK_ID GPS_GetInterfaceThreadId();
PUBLIC uint8* GPS_GetAgpsNVInfo(void);
PUBLIC uint32 GPS_WriteAgpsNVInfo(uint8* nv_info_ptr);
PUBLIC int GPS_Get_Uartport();
PUBLIC int gps_get_assert_mode();
PUBLIC int GPS_Lte_Open(uint8 lte_open);
#ifdef GPS_AGPS_SUPPORT_QX
PUBLIC BLOCK_ID AGPS_GetInterfaceThreadId();
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of gsp_interface.h

#endif // end _GPS_INTERFACE_H_
