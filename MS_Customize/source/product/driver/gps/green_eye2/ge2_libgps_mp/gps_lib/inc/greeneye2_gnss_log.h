/******************************************************************************
 ** File Name:   greeneye2_gnss_log.h                                                *
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

#ifndef __GREENEYE2_GNSS_LOG_H
#define __GREENEYE2_GNSS_LOG_H

/**---------------------------------------------------------------------------*
 **                                                 Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define GNSS_LOG_MAX_RETRY       (5)
#define MAXLINE                  (80)

typedef struct GPS_GNSSLOG_SIG_tag
{
    SIGNAL_VARS
}GPS_GNSSLOG_SIG_T;

typedef enum
{
    GNSS_TYPE_LOG_UNKOWN = 0,
    GNSS_TYPE_LOG_TRACE,
    GNSS_TYPE_LOG_NMEA,
    GNSS_TYPE_LOG_RF,
    GNSS_TYPE_MINILOG_TRACE,
    GNSS_TYPE_LOG_MAX,
}GNSS_TYPE_LOG_E;

typedef struct GPS_GNSSMINILOG_SIG_tag
{
    SIGNAL_VARS
    unsigned short length;
    unsigned char  buff[MAX_MSG_BUFF_SIZE];
}GPS_GNSSMINILOG_SIG_T;


void savenmealog2Buffer(const char*pdata, int len);
void gps_log_task_init(void);
void gps_log_task_deinit(void);
void gps_sendlogmsg();
#endif //__GREENEYE2_GNSS_LOG_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif 

