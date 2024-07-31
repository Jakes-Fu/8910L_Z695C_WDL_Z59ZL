/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "ajms_common.h"
#include "kjava_sys_serial.h"
#include "jkcommconnection.h"

//***********************************************************************************************
//  Description:ini
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_serial_init()
{
	CommInit();
	return 0;
}

//***********************************************************************************************
//  Description:Write 1 character to the TCK serial communications device
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_serial_putc( int c )
{
	return CommPutChar((uint8)c);
}

//***********************************************************************************************
//  Description:Read 1 character from the TCK serial communications device
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int kjava_serial_getc(void)
{
	return CommGetChar();
}

#endif
