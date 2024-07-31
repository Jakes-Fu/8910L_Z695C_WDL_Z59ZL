#include "mta_tracescan_nosme.h"

#include "sci_log.h"
#include <stdarg.h>

#ifdef WIN32

#ifdef MAX_STR_SIZE
#undef MAX_STR_SIZE
#endif

#define MAX_STR_SIZE 3000

#endif

enum{
	Trace_TSC_Nosme=0x910c
};

extern int SIO_SendTracePacket (
    uint32 *src,  // Message head
    int    len   // Message head len
);
//lint -e628
int mta_traceid_data( uint32 traceid)
{
	char trace_info[sizeof(TRACE_LOG_HEAD_T)+sizeof(uint32)];
	TRACE_LOG_HEAD_T * pHead=(TRACE_LOG_HEAD_T*)trace_info;
#ifndef PSIT_SUPPORT
    if(SCI_FALSE == SCI_GetArmLogFlagTemp()) return -1;				
#endif
	pHead->SignalCode=Trace_TSC_Nosme;
	pHead->SignalSize=sizeof(trace_info);
	*(uint32*)(trace_info+sizeof(TRACE_LOG_HEAD_T))=traceid;
    return SIO_SendTracePacket((void*)trace_info,sizeof(trace_info));	
}

int mta_traceid_vdata( uint32 traceid, const int8 *fmt, ...)
{
	va_list         arg_list;
    char	trace_info[sizeof(TRACE_LOG_HEAD_T)+sizeof(uint32)+MAX_STR_SIZE+1];
	char* 	arg_str_buff=trace_info+sizeof(TRACE_LOG_HEAD_T)+sizeof(uint32);
	int 	arg_str_len=0;
	TRACE_LOG_HEAD_T* pHead=(TRACE_LOG_HEAD_T*)trace_info;
	
#ifndef PSIT_SUPPORT
    /* Check switch */
	if (SCI_FALSE == SCI_GetArmLogFlagTemp()) return -1;
#endif

    /* Get args */
    va_start(arg_list, fmt);/*lint !e718*//*lint !e64*/
    /*arg_str_len = vsprintf(arg_str_buff, fmt, arg_list);*/
    arg_str_len = (uint32)_vsnprintf(arg_str_buff, MAX_STR_SIZE, fmt, arg_list);
    if((arg_str_len>MAX_STR_SIZE)  || (arg_str_len == (uint32)-1))//lint !e650 !e737
    {
        arg_str_len = MAX_STR_SIZE;
    }
    SCI_ASSERT(arg_str_len <= MAX_STR_SIZE);
    va_end(arg_list);
	arg_str_buff[arg_str_len++]=0;

	pHead->SignalCode=Trace_TSC_Nosme;
	pHead->SignalSize=sizeof(TRACE_LOG_HEAD_T)+sizeof(uint32)+arg_str_len;//lint !e737
	*(uint32*)(trace_info+sizeof(TRACE_LOG_HEAD_T))=traceid;
    return SIO_SendTracePacket((void*)trace_info,pHead->SignalSize);	
}





