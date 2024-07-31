/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "ajevent.h"
#include "jwe_log.h"
//#include "alldefine.h"



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/***************************************************************************/
// Description:AJEventCreate
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
AJEvent AJEventCreate(void)
{
    AJEvent hEvent = SCI_CreateEvent("ajevent");
    JWE_LOG(AJEventCreate, ("hEvent %08x", hEvent))
    return hEvent;
}

/***************************************************************************/
// Description:AJEventSet
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
int32 AJEventSet(AJEvent event)
{
    uint32 status = 0;
    
    JWE_LOG(AJEventSet, ("Enter"))
    status = SCI_SetEvent(event, 0x01, SCI_OR);
    JWE_LOG(AJEventSet, ("stasus %d", status))
        
    return status;
}

/***************************************************************************/
// Description:AJEventReset
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
int32 AJEventReset(AJEvent event)
{
    uint32 status = 0;
    
    JWE_LOG(AJEventReset, ("Enter"))
    status = SCI_SetEvent(event, 0x00, SCI_AND);
    JWE_LOG(AJEventReset, ("status %d", status))        
    return status;
}

/***************************************************************************/
// Description:AJEventWait
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
int32 AJEventWait(AJEvent event, uint32 time)
{
    uint32 status = 0;
    uint32 event_value = 0;
    
    JWE_LOG(AJEventWait,("(time)=(%d)",time))
        
    status = SCI_GetEvent(event, 0x01, SCI_OR_CLEAR, &event_value, time);
    if(status == SCI_SUCCESS)
    {
        JWE_LOG(AJEventWait,("KJAVA_EVENT_OK"))
        return AJ_EVENT_SIGNAL;
    }
    else
    {
        JWE_LOG(AJEventWait,("KJAVA_EVENT_TIMEOUT"))
        return AJ_EVENT_TIMEOUT;
    }
}

/***************************************************************************/
// Description:AJEventDelete
// Date:2007-06-02
// Author:huangjc
// Note:J
//*************************************************************************/
int32 AJEventDelete(AJEvent event)
{
    uint32 ret = SCI_DeleteEvent(event);
    JWE_LOG(AJEventDelete, ("ret %d", ret))
    return ret;
}

#endif

