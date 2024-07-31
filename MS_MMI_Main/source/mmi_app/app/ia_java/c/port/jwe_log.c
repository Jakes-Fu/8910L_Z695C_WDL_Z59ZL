/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */


/*-----------------------------------------------------------------------------
* Include Files
*----------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "os_api.h"
#include <stdarg.h>

#include "jwe_log.h"
#include "kjava_sys_log_out.h"

/*-----------------------------------------------------------------------------
* Marcos, Types, and Structures
*----------------------------------------------------------------------------*/
#define MAX_FORMAT_STRING_BUFFER_LENGTH         1024
#define MAX_MESSAGE_BUFFER_LENGTH                2048
#define DELAY_TIME                                 500

#define FN_PRINT_OUT(parameters)                                                \
    SCI_MEMSET(messageBuffer, 0, MAX_MESSAGE_BUFFER_LENGTH);                \
    jwe_log_message parameters ;                                  \
    kjava_sys_log_printOut(messageBuffer);                              \
    

/*
if(jwe_log_msg_delay)                                         \
{                                                                   \
kjava_sleep(DELAY_TIME);                                        \
}
*/



/*-----------------------------------------------------------------------------
* Local variables
*----------------------------------------------------------------------------*/

static char stringBuffer[MAX_FORMAT_STRING_BUFFER_LENGTH];
static char messageBuffer[MAX_MESSAGE_BUFFER_LENGTH];

static char jwe_log_enableOutputToAJSLCommander=0;
//static char jwe_log_msg_delay=0;

/*-----------------------------------------------------------------------------
* External Function Prototypes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Local Function Prototypes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Global functions
*----------------------------------------------------------------------------*/

/*
* Create the prefix part of logging message.
*/
void jwe_log_prefix(const char* moduleName, const char* functionName, int line)
{
    int     i;
    int     length          = strlen(moduleName);
    char*   newModuleName   = NULL;
    
    SCI_MEMSET(messageBuffer, 0, MAX_MESSAGE_BUFFER_LENGTH);
    
    // Clean up the directory part in the fullpath of source file //
    for ( i = length - 1 ; i >= 0 ; i-- )
    {
        if ( moduleName[i] == '\\' || moduleName[i] == '/' ) break;
    }
    newModuleName = i < (length - 1) ?
        ((char *) (moduleName + i + 1)) :
    (char *) moduleName;
    
    if(jwe_log_enableOutputToAJSLCommander)
        sprintf(messageBuffer, "<DEBUG>[%17s#%4d][%24s] ", newModuleName, line, functionName);
    else
        //sprintf(messageBuffer, "[%4s][%20s#%4d][%24s] ", "GOD", newModuleName, line, functionName);
        sprintf(messageBuffer, "[%15s#%4d][%25s] ", newModuleName, line, functionName);
}


/*
* Construct the formatted part of logging message.
*/
void jwe_log_message(const char* formatString, ...)
{
    va_list     ap;

    //lint -esym(628, __va_*)
    va_start(ap, formatString); /*lint !e718  !e746 */
    
    SCI_MEMSET(stringBuffer, 0, MAX_FORMAT_STRING_BUFFER_LENGTH);
    vsprintf(stringBuffer, formatString, ap);
    
    strcat(messageBuffer, stringBuffer);
    va_end(ap);
}



/*
* Output the logging message.
*/
void jwe_log_postfix(void)
{
    if(jwe_log_enableOutputToAJSLCommander)
        strcat(messageBuffer, "</DEBUG>");
        /*
        else
        strcat(messageBuffer, "\r\n");
    */
    
    kjava_sys_log_printOut(messageBuffer);
    
    /*
    if(jwe_log_msg_delay)
    {
    kjava_sleep(DELAY_TIME);
    }
    */
}


/*
* Dump the text data in a buffer
*/
void jwe_log_dumpText(char* buffer, int size)
{
    int i = 0;
    int charCount = 0;
    int times = 0;
    
    FN_PRINT_OUT((""))
        FN_PRINT_OUT(("*** DUMP TEXT , size = %d *** ",size))
        FN_PRINT_OUT(("--------------------------------------------"))
        
        while ( i < size )
        {
            {
                times++;
                if (times > 3)
                {
                    times = 0;
                }
            }
            
            SCI_MEMSET(messageBuffer, 0, MAX_MESSAGE_BUFFER_LENGTH);
            strcat(messageBuffer, "   ");
            charCount = 3;
            
            while ((i < size) && (charCount < (MAX_MESSAGE_BUFFER_LENGTH -2)))//out-of-bounds pointer,add 0 end
            {
                if ( buffer[i] == '\r' )
                {
                    messageBuffer[charCount++] = '\\';
                    messageBuffer[charCount++] = 'r';
                    i++;
                }
                else if ( buffer[i] == '\n' )
                {
                    messageBuffer[charCount++] = '\\';
                    messageBuffer[charCount++] = 'n';
                    i++;
                    break;
                }
                else
                {
                    messageBuffer[charCount++] = buffer[i++];
                }
            }
            
            kjava_sys_log_printOut(messageBuffer);
            
            /*
            if(jwe_log_msg_delay)
            {
            kjava_sleep(DELAY_TIME);
            }
            */
        }
        
        FN_PRINT_OUT(("--------------------------------------------"))
            FN_PRINT_OUT((""))
}

#endif
/*-----------------------------------------------------------------------------
* Local functions
*----------------------------------------------------------------------------*/



