/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#ifdef  JAVA_SUPPORT_IA

/* JBlendia interface */
#include "jkpimcommon.h"
#include "ampimutil.h"
#include "ampimevent.h"
#include "pim_common.h"
#include "jwe_log.h"
#include "ajutil.h"
/* Spreatrum interface */
#include "os_api.h"
#include "gui_ucs2b_converter.h"

/*-----------------------------------------------------------------------------
 * Marcos, Types, and Structures
 *----------------------------------------------------------------------------*/
#define ASSERT_LOG(_x)  JWE_LOG_ERR(assert,_x)

#ifndef _AJSC_DEBUG
    #undef  JWE_LOG
    #define JWE_LOG(function,parameters)
#endif

PimListInfo gm_pimList[3] ={0};
//modify in 12.14
void dumpbuf(void *buf, int len)
{
#ifdef _AJSC_DEBUG
	int i;
	unsigned char *p = (unsigned char*)buf;
	JWE_LOG(dumpbuf, ("dumpbuf %08x, %d\n", buf, len))
	if(len==0){
	JWE_LOG(dumpbuf, ("since len=0 to see the value first p NULL the value should be 0x00")	)	
	JWE_LOG(dumpbuf, ("0x%02x ", (unsigned char)(*p)))
	}
	else{

	for( i = 0; i < len; i++)
	{
		JWE_LOG(dumpbuf, ("0x%02x ", (unsigned char)p[i]))
	}
	
	}	
	
#endif	
}

JKSint getListIdByName(JKWChar *name, JKSint32 length)
{
    int i, len;
    // char* p1= NULL;
    // char* p2= NULL;
    
	//p2 = (char *)name;
	//p2[length * 2 + 1] = '\0';

    for ( i = 0; i < PIM_CONTACT_LIST_MAX; i++ )
    {
        
        JWE_LOG(getListIdByName,("gm_pimList[%d] length = %d",i,strlen((char *)gm_pimList[i].listName)));
        JWE_LOG(getListIdByName,("length = %d",length));
        len = PIM_GetUcs2Len((JKWChar *)gm_pimList[i].listName );
        if ( len != length )
        {
           JWE_LOG(getListIdByName,("len != length"));
            continue;
        }
       /* if( !gm_pimList[i].listLength )
        	continue;
        	
        p1 = (char *)gm_pimList[i].listName,     
        p1[gm_pimList[i].listLength + 1] = '\0';
     
        
        JWE_LOG(getListIdByName,("dumpbuf1"));
        dumpbuf(name,length*2);
        JWE_LOG(getListIdByName,("dumpbuf2,length is: %d",gm_pimList[i].listLength));
        dumpbuf(gm_pimList[i].listName,gm_pimList[i].listLength);
        */
        if ( !memcmp((char*) gm_pimList[i].listName,(char*)name,length* sizeof(JKWChar)) )/*lint !e737*/
        {
            JWE_LOG(getListIdByName,("get list name"));
            return gm_pimList[i].listID;
        }
    }
	JWE_LOG(getListIdByName,("err get listname"));
    return (-1);
}
JKSint32 PIM_GetUcs2Len( const JKWChar *wcs )
{
    int i = 0;

    while ( wcs[i] != L'\0' )
        i++;

    return i;
}
/*
	return the wstring number
	*/
uint16 PIM_MakeIntoUcs2(
					uint8 *ucsPtr,//ucs2 big endian string pointer
					uint16 MaxUcs2Len,//ucs2 big endian string buffer len int byte
					BOOLEAN isUcs2,
					const uint8 *utf8Ptr,//utf8 string pongiter
					uint16 utf8Len//utf8 string len int byte
					)
{
	uint16 uscBufLen = 0;
	SCI_MEMSET(ucsPtr,0,MaxUcs2Len);
	
	JWE_LOG(PIM_MakeIntoUcs2,("utf8Len=(%d)", utf8Len));
	if(!isUcs2)
	{
		uscBufLen = GUI_UTF8ToWstr((wchar*)ucsPtr,MaxUcs2Len,utf8Ptr,utf8Len);			
		JWE_LOG(PIM_MakeIntoUcs2,("uscBufLen1=(%d)", uscBufLen));
		
		uscBufLen = PIM_GetUcs2Len((JKWChar *)ucsPtr);
		JWE_LOG(PIM_MakeIntoUcs2,("uscBufLen2=(%d)", uscBufLen));
	}
	else
	{
		SCI_MEMCPY(ucsPtr,utf8Ptr,utf8Len);
		uscBufLen = PIM_GetUcs2Len((JKWChar *)ucsPtr);
		JWE_LOG(PIM_MakeIntoUcs2,("uscBufLen3=(%d)", uscBufLen));
	}
	return uscBufLen;
}

#endif
