/******************************************************************************
 ** File Name:      dm_log.c                                                    *
 ** Author:                                                           *
 ** DATE:           05/05/2008                                               *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file is for dm update agent to trace log                                               *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/05/2008     kelly.li          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "log.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif
#define	UART1_BASE_ADDR  	(0x84000000)
#define	UART_NUM			(1)
/**---------------------------------------------------------------------------*
 **             The const data define                                         *
 **---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//#define UART_DEBUG

#define SIO_TX_EMPTY(s)    	    ((s) & 0xFF00)
#define WAIT_FIFO_EMPTY                                          \
    {                                                            \
        volatile uint32 i;                                      \
        for(i=0;i<0x2000;i++);                                   \
        {                                                        \
           while( SIO_TX_EMPTY(*(volatile uint32*)(UART1_BASE_ADDR+0x0c)));\
        }                                                        \
    }

#define TRACE_LOG_MSG \
        va_start(args, x_format);\
        nBuf = vsprintf(format_str, x_format, args);\
        /* was there an error? */\
        /* Was the expanded string too long? */\
        va_end(args);\
        /* Send message to serial buffer! */ \
        SendMsgOut(format_str, nBuf + 1);

/**---------------------------------------------------------------------------*
 **             Function Definitions                                          *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : write char info 
//	Global resource dependence : 
//  Author:kelly.li
//	Note:
/*****************************************************************************/
LOCAL void WriteChar (char c)
{
	while ((((*(volatile uint32*)UART1_BASE_ADDR+0x0c) >> 8 )&0xFF) >= 64 ) {};
	
	*(volatile uint32*)UART1_BASE_ADDR = c;
}
  
/*****************************************************************************/
// 	Description : send trace msg out
//	Global resource dependence : 
//  Author:kelly.li
//	Note:
/*****************************************************************************/      
LOCAL void SendMsgOut(char * buf_ptr, int size)
{
  while (size --)
  {
   WriteChar (*(buf_ptr++));
  }
    WriteChar('\n');
}

/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_LOW. 
//                  This function put a message to the trace buffer. 
//                  1.msg_trace_info.buf_is_send == TRACE_BUF_SEND
//                    Send the messages to the application tool when buffer 
//                      is full.
//                  2.msg_trace_info.buf_is_send == TRACE_BUF_COVER
//                    Always cover the oldest message when buffer is full.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_TraceLow(
    const char *x_format, ...)
{
	char       format_str[1024];
	
	va_list    args;
	int        nBuf;
    WAIT_FIFO_EMPTY
    memset (format_str,0,1024);
    TRACE_LOG_MSG    /*assert verified*/
    WAIT_FIFO_EMPTY
    return 1;
}
/**---------------------------------------------------------------------------*
 **             Compiler Flag                                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
