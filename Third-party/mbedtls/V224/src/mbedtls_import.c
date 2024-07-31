#include "dal_time.h"
#include "mbedtls/mbedtls_import.h"

//modified by cheng.du of sprd
unsigned int  HAL_time(void)
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


