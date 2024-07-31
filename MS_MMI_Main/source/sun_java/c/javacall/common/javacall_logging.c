/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */
 

#include "mmi_sun_java_trc.h"
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "javacall_time.h"
#include "javacall_logging.h"
#include "os_api.h"
#include "mmifmm_export.h"

#ifdef __cplusplus
extern "C" {
#endif

//show java log time, and counter to know whether some log are lost
#define LOG_COUNTER_AND_TIME 1
static char* sbuf[1024];

void javacall_prints(const char* s) {
    strcat(sbuf, s);/*lint !e64 */
}

void javacall_flushs() {
    javacall_print(sbuf);/*lint !e64 */
    sbuf[0] = 0;
}

#ifdef JAVA_LOGGING_FILE_ENABLE
static BOOLEAN log_to_file=FALSE;
static MMIFILE_HANDLE log_fd = 0;
#define LOGGING_FILENAME   L"D:/jvm.log"

void javacall_logging_file_start()
{
    //SCI_TRACE_LOW:"[ JAVA ][%5d:%10d]: javacall_logging_file_start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_LOGGING_58_112_2_18_3_26_37_4,(uint8*)"dd", log_counter, javacall_time_get_clock_milliseconds());
    log_to_file=TRUE;
    if (log_fd == 0)
    {
        log_fd = MMIAPIFMM_CreateFile(LOGGING_FILENAME, (SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE), NULL, NULL);
        //SCI_TRACE_LOW:"[ JAVA ] create log file handle=%d when start"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_LOGGING_63_112_2_18_3_26_37_5,(uint8*)"d", log_fd);
    }
}

void javacall_logging_file_end()
{
    //SCI_TRACE_LOW:"[ JAVA ][%5d:%10d]: javacall_logging_file_end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_LOGGING_69_112_2_18_3_26_37_6,(uint8*)"dd", log_counter, javacall_time_get_clock_milliseconds());
    log_to_file=FALSE;
    if (log_fd != 0)
    {
        MMIAPIFMM_CloseFile(log_fd);
        log_fd=0;
    }
}
#endif
/**
 * Prints out a string to a system specific output stream
 *
 * @param s a NULL terminated character buffer to be printed
*/
void javacall_print(const char *s) {
#ifdef JAVA_LOGGING_FILE_ENABLE
    char buf[30];
    uint32 transmitted = 0;
#ifndef LOG_COUNTER_AND_TIME
#define LOG_COUNTER_AND_TIME 1
#endif
#endif
#ifdef LOG_COUNTER_AND_TIME
    static int log_counter=0;
    log_counter++;
    if (log_counter > 10000)
        log_counter=1;
    //SCI_TRACE_LOW:"[ JAVA ][%5d:%10d]:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_LOGGING_95_112_2_18_3_26_37_7,(uint8*)"dds", log_counter, javacall_time_get_clock_milliseconds(), s);
#else
    //SCI_TRACE_LOW:"[ JAVA ]:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_LOGGING_97_112_2_18_3_26_37_8,(uint8*)"s", s);
#endif
#ifdef JAVA_LOGGING_FILE_ENABLE
    if (log_to_file == TRUE) {
        if (log_fd == 0)
        {
            log_fd = MMIAPIFMM_CreateFile(LOGGING_FILENAME, (SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE), NULL, NULL);
            //SCI_TRACE_LOW:"[ JAVA ] create log file handle=%d when print"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAVACALL_LOGGING_104_112_2_18_3_26_37_9,(uint8*)"d", log_fd);
        }
        if (log_fd !=0)
        {
            sprintf(buf, "\n[ JAVA ][%5d:%10d]:", log_counter, javacall_time_get_clock_milliseconds());
            MMIAPIFMM_WriteFile(log_fd, (uint8*)buf, 28, &transmitted, NULL);
            MMIAPIFMM_WriteFile(log_fd, s, strlen(s), &transmitted, NULL);
        }
    }
#endif
}

#define BUFFERSIZE 500 //(1 << 7)
static char buffer[BUFFERSIZE];
static char wstrbuffer[BUFFERSIZE];

void javacall_printf(const char *format, ...) {
    va_list ap;
    if (NULL == format)
        return;
    
    //lint -esym(628, __va_*)
    va_start(ap, format);    /*lint !e718 !e64 !e628 */
    #ifdef _MSC_VER
    _vsnprintf(buffer, sizeof(buffer) - 1,format, ap);
    #else
    vsnprintf(buffer, sizeof(buffer) - 1, format, ap);
    #endif
    javacall_print(buffer);
    va_end(ap);             /*lint !e718 !e64 !e628 */
    
}

void javacall_printfv(const char *format, va_list ap) {
    if (NULL == format)
        return;
    
    #ifdef _MSC_VER
    _vsnprintf(buffer, sizeof(buffer) - 1,format, ap);
    #else
    vsnprintf(buffer, sizeof(buffer) - 1, format, ap);
    #endif
    javacall_print(buffer);
}

char* javacall_wstr2str(const short* src)
{
    int i;

    if (((int)src % 2) == 1) {
        javacall_print("!!!!!!!!!!!!!!!! odd address of wchar !!!!!!!!!!!!!!!!");
        src=NULL;
    }

    if (src == NULL) {
        wstrbuffer[0]='n';
        wstrbuffer[1]='u';
        wstrbuffer[2]='l';
        wstrbuffer[3]='l';
        wstrbuffer[4]=0;
        return wstrbuffer;
    }
        
    for (i=0; i<BUFFERSIZE-1; i++) {
        wstrbuffer[i] = (char)src[i];
        if (src[i] == 0)
            break;
    }
    wstrbuffer[BUFFERSIZE-1]=0;

    return wstrbuffer;
}

char* javacall_nwstr2str(const short* src, int len)
{
    int i;

    if (((int)src % 2) == 1) {
        javacall_print("!!!!!!!!!!!!!!!! odd address of wchar !!!!!!!!!!!!!!!!");
        src=NULL;
    }

    if (src == NULL) {
        wstrbuffer[0]='n';
        wstrbuffer[1]='u';
        wstrbuffer[2]='l';
        wstrbuffer[3]='l';
        wstrbuffer[4]=0;
        return wstrbuffer;
    }
    if (len > BUFFERSIZE-2)
        len=BUFFERSIZE-2;
    for (i=0; i<len; i++) {
        wstrbuffer[i] = (char)src[i];
    }
    wstrbuffer[len]=0;

    return wstrbuffer;
}

/*lint --e{48} */ //isalnum pclint
void javacall_printbinary(char* buf, int size)
{
#ifndef WIN32
    int i, index;

    SCI_MEMSET(buffer, 0, (4*16+4));
    for (i=0; i<size; i++)
    {
        index = i % 16;
        if (index ==0)
        {
            buffer[48]=' ';
            buffer[3*16]=':';
            javacall_print(buffer);
            SCI_MEMSET(buffer, ' ', (4*16+2));
        }
        //lint -esym(628, __rt_ctype_table*)  
        //lint -esym(718, __rt_ctype_table*) 
        //lint -esym(746, __rt_ctype_table*)        
        sprintf(buffer+index*3, "%02x", 0xff& buf[i]);
        buffer[index*3+2]=' ';
        if (isalnum(buf[i]) || buf[i]==' ' || buf[i]=='/' || buf[i]=='\\' 
            || buf[i]==':' || buf[i]==',' || buf[i]=='=' || buf[i]=='_' || buf[i]=='-')
             buffer[3*16+2+index]=0xff & buf[i];
        else
             buffer[3*16+2+index]='.';
    }
    javacall_print(buffer);
#endif
}

char* javacall_log_get_file( const char* file )
{
    char*  Log_file = (char*)file;
    char*  Log_current = Log_file;

    while ( *Log_current != '\0' )
    {
        if ( *Log_current == '\\' )
        {
            Log_file = Log_current + 1;
        }

        Log_current++;
    }

    return Log_file;
}
void javacall_assert(int a)     
{
    SCI_ASSERT(a); /*assert verified*/
}                                               

#ifdef __cplusplus
}
#endif
