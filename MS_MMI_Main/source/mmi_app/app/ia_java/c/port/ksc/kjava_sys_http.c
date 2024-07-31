/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/*****************************************************************************
** File Name:                                    *
** Author:         figo.feng                                                 *
** Date:           06/09/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe digit camera                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        figo.feng	       Create
******************************************************************************/


/*-----------------------------------------------------------------------------
* Include Files
*----------------------------------------------------------------------------*/
#ifdef JAVA_SUPPORT_IA
#include "kjava_sys_http.h"



/*-----------------------------------------------------------------------------
* Marcos, Types, and Structures
*----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
* Local variables
*----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
* External Function Prototypes                                              *
*---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Local Function Prototypes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Global functions
*----------------------------------------------------------------------------*/
KJavaHttpHandle kjava_http_open( const char* url, int urlLength, int method, int param, int timeout, KJavaHttpCallbackProc callback )
{
    return (KJavaHttpHandle)0;
}

int kjava_http_write_header( KJavaHttpHandle connection, KJava_Http_Header header[], int headerCount )
{
    return 0;
}

int kjava_http_write( KJavaHttpHandle connection, const unsigned char* data, int dataLength )
{
    return 0;
}

int kjava_http_flush( KJavaHttpHandle connection )
{
    return 0;
}

int kjava_http_finish_request( KJavaHttpHandle connection )
{
    return 0;
}

int kjava_http_read( KJavaHttpHandle connection, unsigned char* data, int dataLength )
{
    return 0;
}

int kjava_http_available( KJavaHttpHandle connection )
{
    return 0;
}

int kjava_http_close( KJavaHttpHandle connection )
{
    return 0;
}

int kjava_http_get_secureinfo( KJavaHttpHandle connection, KJavaSecureInfo *secureInfo )
{
    return 0;
}
#endif