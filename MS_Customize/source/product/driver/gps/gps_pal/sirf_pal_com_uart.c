/**
 * @addtogroup platform_src_sirf_pal_posix
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006-2009 by SiRF Technology, Inc.  All rights reserved.
 *
 *    This Software is protected by United States copyright laws and
 *    international treaties.  You may not reverse engineer, decompile
 *    or disassemble this Software.
 *
 *    WARNING:
 *    This Software contains SiRF Technology Inc.'s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.'s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 *
 *  Keywords for Perforce.  Do not modify.
 *
 *  $File: //customs/customer/spreadtrum/6800h/Software/sirf/pal/Threadx_Syn/sirf_pal_com_uart.c $
 *
 *  $DateTime: 2010/09/10 02:27:27 $
 *
 *  $Revision: #8 $
 */
/**
 * @file   sirf_pal_io_uart.c
 *
 * @brief  UART COM for the POSIX version of the SiRF PAL
 *
 * UART COM for the POSIX version of the SiRF PAL
 */

/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */
/* library includes */
#include "os_api.h"

/* public includes */
#include "sirf_pal.h"
#include "sirf_types.h"
#include "csr_util.h"

/* private includes */

/* local includes */

/* ----------------------------------------------------------------------------
 *   Definitions
 * ------------------------------------------------------------------------- */
#define INVALID_FILE_DESCRIPTOR                  (-1)

#define PORT_IS_STARTED(_uart_settings)                                        \
         ((_uart_settings)->general_settings.port_opened != SIRF_FALSE)


/* ----------------------------------------------------------------------------
 *    Types, Enums, and Structs
 * ------------------------------------------------------------------------- */
typedef struct
{
    int          fd;
    tSIRF_MUTEX  read_mutex;
    tSIRF_MUTEX  write_mutex;
} platform_settings_type;

typedef struct
{
    /* general I/O settings */
    tSIRF_COM_COMMON_SETTINGS general_settings;

    /* uart-specific settings */
    tSIRF_BOOL             flow_control;
    tSIRF_UINT32           baud_rate;

    /* platform-specific (posix) settings */
    platform_settings_type platform_settings;
} com_uart_settings_type;


/* ----------------------------------------------------------------------------
 *    Global Variables
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */
static com_uart_settings_type my_uart_settings[SIRF_COM_UART_MAX_INSTANCES];
static tSIRF_MUTEX create_delete_mutex;


/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */
static void init_port_settings(com_uart_settings_type * const uart_settings,
                               tSIRF_HANDLE               logical_handle);

static tSIRF_RESULT  set_baud_rate_and_flow_control(
                               com_uart_settings_type * const uart_settings);

extern int GPS_ComInit(unsigned long int BaudRate); /*return:         0--ok, others--error.*/

extern int GPS_ComClose(void);  /*return:         0--ok, others--error.*/

extern int GPS_SetBaudRate(unsigned long int baudrate_bps); /*return:         0--ok, others--error.*/

extern int GPS_ComRead(unsigned char *buf, unsigned long int len); /*return:         actual bytes read*/

extern int GPS_ComWrite(unsigned char *buf, unsigned long int len);
	
int CSRGPS_UART_Open(unsigned long int baudrate_bps)/*return: 0 --- Successful, Others --- error*/
{
	return GPS_ComInit(baudrate_bps);
}

int CSRGPS_UART_Close(void) /*return: 0 --- Successful, Others --- error*/
{
	return GPS_ComClose();
}

int CSRGPS_UART_SetBaudRate(unsigned long int baudrate_bps) /*return: 0 --- Successful, Others --- error*/
{
	return GPS_SetBaudRate(baudrate_bps);
}

int CSRGPS_UART_Write(void *pBuf, unsigned long int bytes_to_write) /*return: Non Negative Value --- actual bytes written to UART, Negative Value --- error*/
{
	return GPS_ComWrite(pBuf, bytes_to_write);
}

int CSRGPS_UART_Read(void *pBuf, unsigned long int bytes_to_read) /*return: Non Negative Value --- actual bytes read from UART, Negative Value --- error*/ /* We will Read data from UART every 100 milliseconds.*/
{
	return GPS_ComRead(pBuf, bytes_to_read);
}

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 *
 *    Function:   init_port_settings
 *
 *    Note:       Initialize the UART settings structure for a given port.
 *
 * ------------------------------------------------------------------------- */
static void init_port_settings(com_uart_settings_type * const uart_settings,
                               tSIRF_HANDLE               logical_handle)
{
    CsrMemSet(uart_settings, 0, sizeof(com_uart_settings_type));

    /* initialize the general settings */
    uart_settings->general_settings.logical_handle     = logical_handle;
    uart_settings->general_settings.read_byte_timeout  = 10;
    uart_settings->general_settings.read_total_timeout = 100;
    uart_settings->general_settings.read_wait_timeout  = 100;
    uart_settings->general_settings.port_opened        = SIRF_FALSE;

    /* initialize the uart-specific settings */
    uart_settings->baud_rate    = SIRF_COM_UART_BAUD_DEFAULT;
    uart_settings->flow_control = SIRF_COM_UART_FC_DEFAULT;

    /* initialize the platform-specific settings */
    uart_settings->platform_settings.fd          = INVALID_FILE_DESCRIPTOR;
    uart_settings->platform_settings.read_mutex  = NULL;
    uart_settings->platform_settings.write_mutex = NULL;

    return;
}  /* init_port_settings() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   set_baud_rate_and_flow_control
 *
 *    Note:       Set the baud rate and flow control.
 *
 * ------------------------------------------------------------------------- */
static tSIRF_RESULT  set_baud_rate_and_flow_control(com_uart_settings_type * const uart_settings)
{
    CSRGPS_UART_SetBaudRate(uart_settings->baud_rate);
    return SIRF_SUCCESS;
}


/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Init
 *
 *    Note:       Initialize UART port settings.
 *
 * ------------------------------------------------------------------------- */
void SIRF_PAL_COM_UART_Init(void)
{
    /* local variables */
    int port_counter;
    tSIRF_RESULT mutex_result;

    /* create the create/delete mutex */
    mutex_result = SIRF_PAL_OS_MUTEX_Create(&create_delete_mutex);
    if (SIRF_SUCCESS == mutex_result)
    {
        for(port_counter = 0; port_counter < SIRF_COM_UART_MAX_INSTANCES; port_counter++)
        {
            init_port_settings(&my_uart_settings[port_counter], SIRF_INVALID_HANDLE);
        }
    }
    else
    {
        DEBUGMSG(1, (DEBUGTXT("SIRF_PAL_COM_UART_Init: creation create_delete_mutex failed\n")));
    }

    return;
}  /* SIRF_PAL_COM_UART_Init() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Close
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Close_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Close(tSIRF_HANDLE com_handle)
{
    /* local variables */
    tSIRF_RESULT           return_value = SIRF_COM_RET_FAILURE_GENERIC;
    com_uart_settings_type *uart_settings;
    int result;

    /* Validate input parameters */
    if((SIRF_INVALID_HANDLE != com_handle) &&
        (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
    {
        /* initialize local variables */
        uart_settings = &my_uart_settings[(int)com_handle];

        result = CSRGPS_UART_Close();
        if(result == 0)
        {
            return_value = SIRF_SUCCESS;
        }

        uart_settings->platform_settings.fd         = INVALID_FILE_DESCRIPTOR;
        uart_settings->general_settings.port_opened = SIRF_FALSE;
    }

    return(return_value);
}  /* SIRF_PAL_COM_UART_Close() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Control
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Control_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Control(
      tSIRF_HANDLE               com_handle,
      tSIRF_COM_CONTROL          com_control,
      void               * const com_value)
{
    /* local variables */
    tSIRF_RESULT               return_value = SIRF_COM_RET_FAILURE_GENERIC;
    com_uart_settings_type     *uart_settings;

    /* Validate input parameters */
    if((SIRF_INVALID_HANDLE != com_handle) &&
        (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
    {
        uart_settings = &my_uart_settings[(int)com_handle];

        switch(com_control)
        {
            /* ----------------------------------------------------------------------
            *   General Settings
            * ------------------------------------------------------------------- */
        case SIRF_COM_CTRL_READ_BYTE_TIMEOUT:
            uart_settings->general_settings.read_byte_timeout = (tSIRF_INT32)com_value;
            return_value = SIRF_SUCCESS;
            break;

        case SIRF_COM_CTRL_READ_TOTAL_TIMEOUT:
            uart_settings->general_settings.read_total_timeout = (tSIRF_INT32)com_value;
            return_value = SIRF_SUCCESS;
            break;

        case SIRF_COM_CTRL_READ_WAIT_TIMEOUT:
            uart_settings->general_settings.read_wait_timeout = (tSIRF_INT32)com_value;
            return_value = SIRF_SUCCESS;
            break;

            /* ----------------------------------------------------------------------
            *   UART-Specific Settings
            * ------------------------------------------------------------------- */
        case SIRF_COM_CTRL_UART_BAUD_RATE:
            uart_settings->baud_rate = (tSIRF_UINT32)com_value;
            if(PORT_IS_STARTED(uart_settings))
            {
                return_value = set_baud_rate_and_flow_control(uart_settings);
            }
            else
            {
                return_value = SIRF_SUCCESS;
            }
            break;

        case SIRF_COM_CTRL_UART_FLOW_CONTROL:
            uart_settings->flow_control = (tSIRF_UINT32)com_value;
            if(PORT_IS_STARTED(uart_settings))
            {
                return_value = set_baud_rate_and_flow_control(uart_settings);
            }
            else
            {
                return_value = SIRF_SUCCESS;
            }
            break;

        default:
            /* an unhandled/unknown setting was commanded.  do nothing */
            DEBUGMSG(1, (DEBUGTXT("Unhandled UART control setting: %d\n", (tSIRF_UINT32)com_control)));
            break;
        }
    }

    return(return_value);

}  /* SIRF_PAL_COM_UART_Control() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Create
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Create_Fcn().
 *
 *    Warning:    As written, this function is not thread-safe.
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Create(
      tSIRF_HANDLE       * const com_handle)
{
    /* local variables */
    tSIRF_RESULT return_value = SIRF_COM_RET_FAILURE_GENERIC;
    int          port_counter;
    tSIRF_RESULT mutex_result;

    if(NULL == com_handle)
    {
        DEBUGMSG(1, (DEBUGTXT("SIRF_PAL_COM_UART_Create: invalid input parameters\n")));
        return(SIRF_COM_RET_FAILURE_GENERIC);
    }

    *com_handle = SIRF_INVALID_HANDLE;

    /* ----------------------------------------------------------------------
    *   1. lock the create/delete mutex
    * ------------------------------------------------------------------- */
    mutex_result = SIRF_PAL_OS_MUTEX_Enter(create_delete_mutex);
    if(SIRF_SUCCESS != mutex_result)
    {
        DEBUGMSG(1, (DEBUGTXT("Uart Create Mutex Error on line %d\n", __LINE__)));
        return_value = SIRF_COM_RET_FAILURE_MUTEX_FAILED;
    }
    else
    {
        /* ----------------------------------------------------------------------
        *   2. search for an unused port
        * ------------------------------------------------------------------- */
        for(port_counter = 0; port_counter < SIRF_COM_UART_MAX_INSTANCES; port_counter++)
        {
            if(SIRF_INVALID_HANDLE == my_uart_settings[port_counter].general_settings.logical_handle)
            {
                /* we found an unused port, reserve it */
                init_port_settings(&my_uart_settings[port_counter], (tSIRF_HANDLE)port_counter);

                /* create the read and write mutexes */
                mutex_result = SIRF_PAL_OS_MUTEX_Create(&my_uart_settings[port_counter].platform_settings.read_mutex);

                if(SIRF_SUCCESS == mutex_result)
                {
                    mutex_result = SIRF_PAL_OS_MUTEX_Create(&my_uart_settings[port_counter].platform_settings.write_mutex);

                    if(SIRF_SUCCESS == mutex_result)
                    {
                        *com_handle  = my_uart_settings[port_counter].general_settings.logical_handle;
                        return_value = SIRF_SUCCESS;
                    }
                    else
                    {
                        /* creating write mutex failed, delete the read */
                        (void)SIRF_PAL_OS_MUTEX_Delete(&my_uart_settings[port_counter].platform_settings.read_mutex);
                    }
                }

                break;
            }
        } /* for */

        /* ----------------------------------------------------------------------
        *   3. unlock the create/delete mutex
        * ------------------------------------------------------------------- */
        (void)SIRF_PAL_OS_MUTEX_Exit(create_delete_mutex);
    } /* else */

    return(return_value);

}  /* SIRF_PAL_COM_UART_Create() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Delete
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Delete_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Delete(
      tSIRF_HANDLE       * const com_handle)
{
    /* local variables */
    tSIRF_RESULT            return_value = SIRF_COM_RET_FAILURE_GENERIC;
    com_uart_settings_type  *uart_settings;
    tSIRF_RESULT            result_read;
    tSIRF_RESULT            result_write;
    tSIRF_RESULT            mutex_result;

    /* Validate input parameters */
    if((NULL != com_handle) &&
        (SIRF_INVALID_HANDLE != *com_handle) &&
        (SIRF_COM_UART_MAX_INSTANCES > (int)*com_handle))
    {
        uart_settings = &my_uart_settings[(int)*com_handle];

        /* ----------------------------------------------------------------------
        *   1. lock the create/delete mutex
        * ------------------------------------------------------------------- */
        mutex_result = SIRF_PAL_OS_MUTEX_Enter(create_delete_mutex);
        if(SIRF_SUCCESS != mutex_result)
        {
            DEBUGMSG(1, (DEBUGTXT("Uart Create Mutex Error on line %d\n", __LINE__)));
            return_value = SIRF_COM_RET_FAILURE_MUTEX_FAILED;
        }
        else
        {
            /* ----------------------------------------------------------------------
            *   2. delete the read/write mutexes
            * ------------------------------------------------------------------- */
            result_read = SIRF_PAL_OS_MUTEX_Delete(uart_settings->platform_settings.read_mutex);
            result_write = SIRF_PAL_OS_MUTEX_Delete(uart_settings->platform_settings.write_mutex);

            /* invalidate the platform settings */
            init_port_settings(uart_settings, SIRF_INVALID_HANDLE);
            *com_handle = SIRF_INVALID_HANDLE;

            if ((SIRF_SUCCESS == result_read) && (SIRF_SUCCESS == result_write))
            {
                return_value = SIRF_SUCCESS;
            }

            /* ----------------------------------------------------------------------
            *   3. unlock the create/delete mutex
            * ------------------------------------------------------------------- */
            (void)SIRF_PAL_OS_MUTEX_Exit(create_delete_mutex);
        }
    }

    return(return_value);

}  /* SIRF_PAL_COM_UART_Delete() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Open
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Open_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Open(
      tSIRF_HANDLE               com_handle,
      char         const * const com_port)
{
    /* local variables */
    tSIRF_RESULT            return_value = SIRF_COM_RET_FAILURE_GENERIC;
    com_uart_settings_type  *uart_settings;
    int                     fd;

    if((SIRF_INVALID_HANDLE != com_handle) &&
        (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
    {
        uart_settings = &my_uart_settings[(int)com_handle];

        if(PORT_IS_STARTED(uart_settings))
        {
            DEBUGMSG(1, (DEBUGTXT("SIRF_PAL_COM_UART_Open: port is already started\n")));
            return_value = SIRF_COM_RET_FAILURE_GENERIC_OPEN;
        }
        else
        {
            /* open the port: */
            fd = CSRGPS_UART_Open(uart_settings->baud_rate);
            if ( fd != 0 )
            {
                DEBUGMSG(1, (DEBUGTXT("error: could not open port; errno\n")));
                return_value = SIRF_COM_RET_FAILURE_GENERIC_OPEN;
            }
            else
            {
                uart_settings->platform_settings.fd = fd;
                return_value = set_baud_rate_and_flow_control (uart_settings);
                if (SIRF_SUCCESS == return_value)
                {
                    uart_settings->general_settings.port_opened = SIRF_TRUE;
                }
            }
        }
    }

    return(return_value);

}  /* SIRF_PAL_COM_UART_Open() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Read
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Read_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Read(
      tSIRF_HANDLE               com_handle,
      tSIRF_UINT8        * const read_ptr,
      tSIRF_UINT32               requested_bytes_to_read,
      tSIRF_UINT32       * const actual_bytes_read)
{
    /* local variables */
    com_uart_settings_type  * uart_settings;
    tSIRF_UINT32            bytes_read = 0;
    tSIRF_RESULT            return_value = SIRF_COM_RET_FAILURE_GENERIC;
    tSIRF_UINT32            byte_timeout_ms;
    tSIRF_UINT32            start_time_ms;
    tSIRF_UINT32            now_time_ms;
    tSIRF_BOOL              timeout = SIRF_FALSE;
    tSIRF_BOOL			skip_read = SIRF_FALSE;
    tSIRF_RESULT            pal_result;
    int                     os_result = 0;

#define READ_TIMEOUT_SLEEP_INCREMENT (10)

    /* Validate input parameters */
    if((SIRF_INVALID_HANDLE != com_handle) &&
        (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle) &&
        (NULL != read_ptr) &&
        (0 < requested_bytes_to_read) &&
        (NULL != actual_bytes_read))
    {
        uart_settings    = &my_uart_settings[(int)com_handle];
        start_time_ms    = SIRF_PAL_OS_TIME_SystemTime();
        byte_timeout_ms  = start_time_ms;

        /* -------------------------------------------------------------------------
        *   1.  ensure that the port has been started
        * ---------------------------------------------------------------------- */
        if(!(PORT_IS_STARTED(uart_settings)))
        {
            return_value = SIRF_COM_RET_FAILURE_GENERIC_OPEN;
        }
        else
        {
            /* ----------------------------------------------------------------------
            *   2.  lock the read mutex
            * ------------------------------------------------------------------- */

            pal_result = SIRF_PAL_OS_MUTEX_Enter(uart_settings->platform_settings.read_mutex);
            if(SIRF_SUCCESS != pal_result)
            {
                DEBUGMSG(1, (DEBUGTXT("Uart Read Mutex Error on line %d\n", __LINE__)));
                return_value = SIRF_COM_RET_FAILURE_MUTEX_FAILED;
            }
            else
            {
                /* -------------------------------------------------------------------
                *   3.  wait for a byte to be received,
                *       or wait timeout exceeded,
                *       or the port to be closed
                * ---------------------------------------------------------------- */
                if(0 > uart_settings->platform_settings.fd)
                {
                    return_value = SIRF_COM_RET_FAILURE_GENERIC;
                }
                else
                {

                    while ((os_result == 0) && (PORT_IS_STARTED(uart_settings)) && (timeout == SIRF_FALSE))
                    {
                        os_result = CSRGPS_UART_Read(read_ptr, requested_bytes_to_read);
                        if (os_result > 0)
                        {
                            skip_read = SIRF_TRUE;
                            break;
                        }
                        else
                        {
                            SIRF_PAL_OS_THREAD_Sleep(READ_TIMEOUT_SLEEP_INCREMENT);

                            now_time_ms = SIRF_PAL_OS_TIME_SystemTime();

                            /* check for wait timeout */
                            if ((now_time_ms - start_time_ms) >= ((uint32)uart_settings->general_settings.read_wait_timeout))
                            {
                                timeout = SIRF_TRUE;
                                break;
                            }

                        }
                    }

                    while(((bytes_read < requested_bytes_to_read)
                        && (SIRF_TRUE != timeout)
                        && PORT_IS_STARTED(uart_settings)) || (skip_read == SIRF_TRUE))
                    {
                        if (skip_read == SIRF_TRUE)
                        {
                            skip_read = SIRF_FALSE;
                        }
                        else
                        {
                            os_result = CSRGPS_UART_Read(&read_ptr[bytes_read], (requested_bytes_to_read - bytes_read));
                        }

                        if(os_result < 0)
                        {
                            DEBUGMSG(1, (DEBUGTXT("SIRF_PAL_COM_UART_Read: read failed with errno\n")));
                            return_value = SIRF_COM_RET_FAILURE_UART_ERROR;
                            *actual_bytes_read = 0;
                        }
                        else if (os_result == 0)
                        {
                            /* if byte timeout has occurred, return timeout */
                            now_time_ms = SIRF_PAL_OS_TIME_SystemTime();
                            if((now_time_ms - byte_timeout_ms) >= ((uint32)uart_settings->general_settings.read_byte_timeout))
                            {
                                timeout = SIRF_TRUE;
                                break;
                            }

                            /* queue empty, sleep for a bit */
                            SIRF_PAL_OS_THREAD_Sleep(READ_TIMEOUT_SLEEP_INCREMENT);
                        }
                        else
                        {
                            byte_timeout_ms = SIRF_PAL_OS_TIME_SystemTime();

                            return_value = SIRF_SUCCESS;
                            bytes_read += (uint32)os_result;

                        }

                        /* check for read total timeout */
                        now_time_ms = SIRF_PAL_OS_TIME_SystemTime();
                        if ((now_time_ms - start_time_ms) >= ((uint32)uart_settings->general_settings.read_total_timeout))
                        {
                            timeout = SIRF_TRUE;
                        }

                    }

                    /* -------------------------------------------------------------------
                    *   5.  unlock the mutex
                    * ---------------------------------------------------------------- */
                    (void)SIRF_PAL_OS_MUTEX_Exit(uart_settings->platform_settings.read_mutex);
                }
            }
        }
    }
    else
   	{
        return_value = SIRF_COM_RET_FAILURE_GENERIC;
   	}

    *actual_bytes_read = bytes_read;/*lint !e613*/

    return(return_value);

}  /* SIRF_PAL_COM_UART_Read() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Write
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Write_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Write(
      tSIRF_HANDLE               com_handle,
      tSIRF_UINT8  const * const write_ptr,
      tSIRF_UINT32               requested_bytes_to_write,
      tSIRF_UINT32       * const actual_bytes_written)
{
    /* local variables */
    com_uart_settings_type  *uart_settings;
    tSIRF_RESULT            return_value = SIRF_COM_RET_FAILURE_GENERIC;
    tSIRF_UINT32            bytes_written = 0;
    tSIRF_RESULT            pal_result;
    int                     os_result;

    /* Validate input paramters */
    if((SIRF_INVALID_HANDLE != com_handle) &&
        (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle) &&
        (NULL != write_ptr) &&
        (0 < requested_bytes_to_write) &&
        (NULL != actual_bytes_written))
    {
        uart_settings    = &my_uart_settings[(int)com_handle];

        /* ----------------------------------------------------------------------
        *   1.  ensure that the port has been started
        * ------------------------------------------------------------------- */
        if(!(PORT_IS_STARTED(uart_settings)))
        {
            return_value = SIRF_COM_RET_FAILURE_GENERIC_OPEN;
        }
        else
        {
            /* ----------------------------------------------------------------------
            *   2.  lock the write mutex
            * ------------------------------------------------------------------- */
            pal_result = SIRF_PAL_OS_MUTEX_Enter(uart_settings->platform_settings.write_mutex);
            if(SIRF_SUCCESS != pal_result)
            {
                DEBUGMSG(1, (DEBUGTXT("Uart Write Mutex Error on line %d\n"), __LINE__));
                return_value = SIRF_COM_RET_FAILURE_MUTEX_FAILED;
            }
            else
            {
                if(0 > uart_settings->platform_settings.fd)
                {
                    return_value = SIRF_COM_RET_FAILURE_GENERIC;
                }
                else
                {
                    do
                    {
                        os_result = CSRGPS_UART_Write(write_ptr+bytes_written, requested_bytes_to_write-bytes_written);/*lint !e124 !e605*/
                        if(0 > os_result)
                        {
                            DEBUGMSG(1, (DEBUGTXT("SIRF_PAL_COM_UART_Write: write failed with errno\n")));
                            return_value = SIRF_COM_RET_FAILURE_GENERIC_WRITE;
                            break;
                        }
                        else
                        {
                            bytes_written += (uint32)os_result;
                        }
			 /* Have a short break to allow transmission from the UART buffer if it seems to be filled */
			   if (bytes_written < requested_bytes_to_write)
			   {
			   	SIRF_PAL_OS_THREAD_Sleep(10);
			   }
                    }while ( bytes_written < requested_bytes_to_write && PORT_IS_STARTED(uart_settings) );

                    if(bytes_written == requested_bytes_to_write)
                    {
                        return_value = SIRF_SUCCESS;
                    }
                }

                /* -------------------------------------------------------------------
                *   5.  unlock the mutex
                * ---------------------------------------------------------------- */
                (void)SIRF_PAL_OS_MUTEX_Exit(uart_settings->platform_settings.write_mutex);
            }
        }
    }

    *actual_bytes_written = bytes_written;/*lint !e613*/

    return(return_value);

}  /* SIRF_PAL_COM_UART_Write() */

/* ============================================================================
 * Functions below are used to reset a GSD4t tracker only.
 * They are not used for any communication handshake. */

/**
 * @brief Clear serial port RTS.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_UART_ClrRTS( tSIRF_HANDLE com_handle )
{
    tSIRF_RESULT return_value = SIRF_SUCCESS;

    return return_value;
} /* SIRF_PAL_COM_UART_ClrRTS() */

/**
 * @brief Set serial port RTS.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_UART_SetRTS( tSIRF_HANDLE com_handle )
{
    tSIRF_RESULT return_value = SIRF_SUCCESS;

    return return_value;
} /* SIRF_PAL_COM_UART_SetRTS() */

/**
 * @brief Clear serial port DTR.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_UART_ClrDTR( tSIRF_HANDLE com_handle )
{
    tSIRF_RESULT return_value = SIRF_SUCCESS;

    return return_value;
} /* SIRF_PAL_COM_UART_ClrDTR() */

/**
 * @brief Set serial port DTR.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_UART_SetDTR( tSIRF_HANDLE com_handle )
{
    tSIRF_RESULT return_value = SIRF_SUCCESS;

    return return_value;
} /* SIRF_PAL_COM_UART_SetDTR() */

/**
 * @}
 * End of file.
 */
