/*
 * Copyright 2005 Aplix Corporation. All rights reserved.
 */

#ifndef KJAVA_SYS_SERIAL_HEADER
#define KJAVA_SYS_SERIAL_HEADER

/**
 * @file kjava_sys_serial.h
 * TCK Serial KSI
 */

#ifdef __cplusplus
extern "C"{
#endif

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup tck_ksi
 * @{
 */
 
/** 
 * @addtogroup kjava_sys_serial Serial IO Adatpter KSI
 * @{
 */

/** \brief The operation processed successfully */
#define CONST_KJAVA_SERIAL_OK               0

/** \brief The serial device is now busy or without pending data for reading */
#define CONST_KJAVA_SERIAL_NOT_AVAILABLE    -1

/** \brief The serial device reports an error while processing */
#define CONST_KJAVA_SERIAL_ERROR            -2


/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/
/**
 *  \brief Write 1 character to the TCK serial communications device.
 *  @param c
 *        The character to be written. The low 8 bits are valid.
 *  @return
 *        Return 0 if the operation succeeds.<br>
 *        Return -1 if writing failed because the device was busy.<br>
 *        Return a negative value other than -1 if writing failed because of
 *        communication device trouble.
 *
 *  Write 1 character (8 bits) to the serial communications device used for the TCK. <br>
 *  If the communications device is busy (suppressed by flow control, buffer full,
 *  etc.), return -1 indicating the busy state and return immediately to the caller.
 *  <dl>
 *   <dt>Note carefully!</dt>
 *   <dd>
 *    Implement so that no other serial output (#kjava_sys_log_printOut() log output, etc.)
 *    takes place at the same time.
 *   </dd>
 *  </dl>
 *
 * @endif
 * @see kjava_serial_getc()
 */
int kjava_serial_putc( int c );

/** \brief Read 1 character from the TCK serial communications device.
 *  @return
 *        If the read operation succeeds, return a positive value (0 to 255) as the
 *        read character.<br>
 *        Return -1 if there is no character to be read.<br>
 *        Return a negative value other than -1 if writing failed because of
 *        communication device trouble.
 *
 *  Read 1 character (8 bits) from the serial communications device used for the TCK. <br>
 *  If there is no character to be read from the communications device (none in the
 *  receive buffer, etc.) when the read operation is performed, return -1 indicating
 *  no character to be read and return immediately to the caller.
 *
 * @endif
 * @see kjava_serial_putc()
 */
int kjava_serial_getc(void);

//***********************************************************************************************
//  Description:ini
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_serial_init(void);

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_SERIAL_HEADER */
