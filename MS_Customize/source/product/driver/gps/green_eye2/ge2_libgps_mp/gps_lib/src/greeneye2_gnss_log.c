
#include "sci_types.h"
#include "sci_api.h"
#include "threadx_os.h"
#include "threadx_assert.h"
#include "os_api.h"
#include "priority_system.h"
#include "dal_time.h"
#include <stdio.h>
#include "sci_types.h" 
#include "greeneye2_common.h"
#include "greeneye2_gnss_log.h"
#include "greeneye2_tcard_log.h"

BLOCK_ID s_gps_log_thread_id = SCI_INVALID_BLOCK_ID;
#define GPS_LOG_THREAD_STACK_SIZE (5*1024)
#define s_gps_log_thread_task_priority     PRI_GPS_LOG_TASK    //31
#define GPS_LOG_THREAD_QUEUE_NUM    0x20

/**---------------------------------------------------------------------------*
 **                                                 Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

extern int         gLogType;
extern int         gWriteFlag;
extern int         gDropCount;
extern char ge2_logbuf[512];
extern uint32 ge2_loglen;
 LOCAL void gnss_log_entry(uint32 argc, void *argv);
LOCAL void gps_log_start_thread(void);
/*--------------------------------------------------------------------------
    Function:  savenmealog2Buffer

    Description:
        it only save nmea  log  to temp buffer when it need report callbacks.nmea_cb  
    Parameters:
     pdata: point the  nma log 
     len : the one nmea log datalen  

     notes:  the  input data length <= 2048  
     
    Return: void
--------------------------------------------------------------------------*/
void savenmealog2Buffer(const char*pdata, int len)
{
	GpsState*   s = _gps_state;
    int datalen = sizeof(ge2_logbuf);
    int count = GNSS_LOG_MAX_RETRY;
    
    GREENEYE2_LOGD(("[GE2]%s input datalen : %d ",__FUNCTION__,len));
    if((pdata == NULL)|| (len > datalen))
    {
        GREENEYE2_LOGE(("[GE2]%s input datalen : %d > 512",__FUNCTION__,len));
        return;
    }
    while(count)
    {
        if(gWriteFlag)
        {
            break;
        }
        else
        {
            gps_sendlogmsg(); 
            SCI_Sleep(5);//1ms 
            count--;
        }
    }
    if(0 == gWriteFlag)
    {
        gDropCount++;
        gWriteFlag = 1; 
        GREENEYE2_LOGD(("[GE2]%s will drop count:%d gnss log :%s",__FUNCTION__,gDropCount,ge2_logbuf));
    }
    //pthread_mutex_lock(&s->gnsslogmutex);
    SCI_MEMSET(ge2_logbuf,0,sizeof(ge2_logbuf));
    memcpy(ge2_logbuf,pdata,len);
    ge2_loglen = len;
    gLogType = GNSS_TYPE_LOG_NMEA;//it only save file 
    gWriteFlag = 0;
    //pthread_mutex_unlock(&s->gnsslogmutex);
    gps_sendlogmsg(); 
    SCI_Sleep(5);//release read thread 
}


/*--------------------------------------------------------------------------
    Function:  copygnsslog2Sendbuffer

    Description:
         it copy gnss log from tmep buffer to send buffer  
    Parameters: void
     
    Return: void
--------------------------------------------------------------------------*/
void copygnsslog2Sendbuffer()
{
    GpsState*   s = _gps_state;
    int dataLen = 0;
    
    SCI_MEMSET(s->gnsslogBuffer,0,GNSS_LOG_BUFFER_MAX);
    if(ge2_loglen < GNSS_LOG_BUFFER_MAX)   
    {
        //memcpy(s->gnsslogBuffer,ge2_logbuf,strlen(ge2_logbuf));
        memcpy(s->gnsslogBuffer,ge2_logbuf,ge2_loglen);
    }
    else
    {
        GREENEYE2_LOGD(("[GE2]copygnsslog2Sendbuffer dataLen > 512"));
    }
    gWriteFlag = 1;
    SCI_MEMSET(ge2_logbuf,0,sizeof(ge2_logbuf));
    //pthread_mutex_unlock(&s->gnsslogmutex);

}

 /*--------------------------------------------------------------------------
     Function:  gnss_rf_capture_data
 
     Description:
          it read rf data from /data/gnss/yyyy-mm-dd-mmin--so on.txt
     Parameters: void
      
     Return: void
 --------------------------------------------------------------------------*/
 void gnss_rf_capture_data(void )
{
    GpsState*   s = _gps_state;

    copygnsslog2Sendbuffer();

    s->callbacks.nmea_cb(GNSS_TYPE_LOG_RF,(const char*)s->gnsslogBuffer,ge2_loglen);

#ifdef GPS_TCARD_LOG_SUPPORT
    //saveLocallog((const char*)s->gnsslogBuffer,ge2_loglen);
#endif
}

/*--------------------------------------------------------------------------
    Function:  saveLocallog

    Description:
        it gnss  log  to local dir : /data/gnss/gnss.log 
    Parameters:
     data: point the  gnss  log 
     len : the one  gnss  log  datalen  

     notes:  the  input data length <= 2048  
     
    Return: void
--------------------------------------------------------------------------*/
void saveLocallog(const char  *data,short len)
{
#ifdef GPS_TCARD_LOG_SUPPORT
    //Gps_WriteCardLog((uint8 *)data,len);
#endif
}

/*--------------------------------------------------------------------------
    Function:  close_logsave

    Description:
        it closed the local gnss log file handle  
    Parameters:
     
    Return: void
--------------------------------------------------------------------------*/
void close_logsave(void)
{
#ifdef GPS_TCARD_LOG_SUPPORT
    //Gps_CardLogClose();
#endif
}


/*****************************************************************************/
//  Description:    gps_log_task_init
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_log_task_init(void)
{
    gps_log_start_thread();
}

/*****************************************************************************/
//  Description:    gps_log_task_deinit
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_log_task_deinit(void)
{
    if(s_gps_log_thread_id != SCI_INVALID_BLOCK_ID)
    {
        SCI_TerminateThread(s_gps_log_thread_id);
        SCI_DeleteThread(s_gps_log_thread_id);
        s_gps_log_thread_id = SCI_INVALID_BLOCK_ID;
    }    
}

/*****************************************************************************/
//  Description:    gps_log_start_thread
//  Author:         
//  Note:   
/*****************************************************************************/
LOCAL void gps_log_start_thread(void)
{
	
    s_gps_log_thread_id = SCI_CreateThread("T_GPS_LOG_SCHED", "Q_GPS_LOG_SCHED", gnss_log_entry, 0, NULL, GPS_LOG_THREAD_STACK_SIZE, 
									GPS_LOG_THREAD_QUEUE_NUM, s_gps_log_thread_task_priority, SCI_PREEMPT, SCI_AUTO_START);

    SCI_ASSERT(SCI_INVALID_BLOCK_ID != s_gps_log_thread_id);
    
}


/*--------------------------------------------------------------------------
    Function:  gnss_log_thread

    Description:
        it only save nmea  log  to temp buffer when it need report callbacks.nmea_cb  
    Parameters:
     pdata: point the  nma log 
     len : the one nmea log datalen  

     notes:  the  input data length <= 2048  
     
    Return: void
--------------------------------------------------------------------------*/
LOCAL void gnss_log_entry(uint32 argc, void *argv)
{
    GpsState*   s = _gps_state;
    xSignalHeaderRec *signal = PNULL;
    uint32 len = 0;
    GREENEYE2_LOGD(("[GE2]gnss_log_thread: enter"));

    do
    {
        signal = (xSignalHeaderRec *) SCI_GetSignal (s_gps_log_thread_id);
        //ret = WaitEvent(&(s->gnssLogsync), EVENT_WAIT_FOREVER);//wait signal

        GREENEYE2_LOGD(("[GE2]s->etuMode:%d,s->postlog:%d,slog_switch:%d ,s->logswitch:%d",s->etuMode,s->postlog,s->slog_switch,s->logswitch));

        if(s->etuMode)//gnss tool test 
        {
            if(s->rftool_enable &&(GNSS_TYPE_LOG_RF == gLogType))
            {
                gnss_rf_capture_data();
            }
            else
            {
                copygnsslog2Sendbuffer();
                GREENEYE2_LOGD(("[GE2]gnss_log_thread: etuMode datalen:%d", strlen((const char *)s->gnsslogBuffer)));
                GREENEYE2_LOGD(("[GE2][GE2ETULOG]%s",s->gnsslogBuffer));
                s->callbacks.nmea_cb(s->NmeaReader[0].fix.timestamp,(const char*)s->gnsslogBuffer,strlen((const char *)s->gnsslogBuffer));
            }
        }

        //it start ready gnss log or nmea and report the client 
        else if(s->postlog) //gnss tool or autotest  repoart  all gnss log to pc  
        {
            //if rf tool test ,it get data from file 
            copygnsslog2Sendbuffer();
            GREENEYE2_LOGD(("[GE2]gnss_log_thread: postlog datalen:%d", strlen((const char *)s->gnsslogBuffer)));
            s->callbacks.nmea_cb(s->NmeaReader[0].fix.timestamp,(const char*)s->gnsslogBuffer,strlen((const char *)s->gnsslogBuffer));
        }
        else if (s->slog_switch) //the modem slog open gnss log ,not pc tool 
        {
            //first it should transfer gnss log to modem log daemon 
            if(GNSS_TYPE_LOG_TRACE == gLogType)
            {
                copygnsslog2Sendbuffer();
                GREENEYE2_LOGD(("[GE2]gnss_log_thread: slog_switch trace  datalen:%d,data:%s", strlen((const char *)s->gnsslogBuffer),s->gnsslogBuffer));
                //writeGnsslogTokernel((unsigned char*)s->gnsslogBuffer, (unsigned short)strlen((const char *)s->gnsslogBuffer));
            }
            else if(GNSS_TYPE_LOG_NMEA == gLogType)//it need report nmea log to client when nmea log
            {
                copygnsslog2Sendbuffer();
                GREENEYE2_LOGD(("[GE2]gnss_log_thread: slog_switch nmea  datalen:%d", strlen((const char *)s->gnsslogBuffer)));
                s->callbacks.nmea_cb(s->NmeaReader[0].fix.timestamp,(const char*)s->gnsslogBuffer,strlen((const char *)s->gnsslogBuffer));
            }

        }
        else // the pc tool and modem slog can't configuration , 
        {
            if((0 == s->release) && (s->logswitch)&&(GNSS_TYPE_LOG_TRACE == gLogType))// only userdebug pac and log switch on 
            {
                copygnsslog2Sendbuffer();
                GREENEYE2_LOGD(("[GE2]gnss_log_thread: logswitch trace  datalen:%d",  ge2_loglen));
                //GREENEYE2_LOGD(("[GE2][GE2LOG]%s",s->gnsslogBuffer));

                //len = ge2_loglen;
                len = ge2_loglen;
                if(s->gnsslogBuffer[len-1] != 0x0A)
                {
                    s->gnsslogBuffer[len] = 0x0D;
                    s->gnsslogBuffer[len+1] = 0x0A;
                    len += 2;
                }
                s->callbacks.nmea_cb(GNSS_TYPE_LOG_TRACE,(const char*)s->gnsslogBuffer,len);
            }

            //it need report nmea log to client when nmea log 
            if(GNSS_TYPE_LOG_NMEA == gLogType)
            {
                copygnsslog2Sendbuffer();
                len = strlen((const char *)s->gnsslogBuffer);
                s->callbacks.nmea_cb(GNSS_TYPE_LOG_NMEA,(const char*)s->gnsslogBuffer,len);
            }

            saveLocallog((const char*)s->gnsslogBuffer,len);

        }
        SCI_FREE(signal);
        signal = NULL;
    }while(1);
    GREENEYE2_LOGD(("[GE2]gnss_log_thread: leave"));

    return;
}

/*****************************************************************************/
//  Description:    gps_sendmsg
//  Author:         
//  Note:   
/*****************************************************************************/
void gps_sendlogmsg()
{
    xSignalHeaderRec *signal = PNULL;

    SCI_CREATE_SIGNAL(
        signal,
        NULL,
        sizeof (xSignalHeaderRec),
        SCI_IDENTIFY_THREAD());
    
    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, s_gps_log_thread_id);   
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif 

