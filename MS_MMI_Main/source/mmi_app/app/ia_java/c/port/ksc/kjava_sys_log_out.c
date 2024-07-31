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
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#include"os_api.h"
#ifdef JAVA_SUPPORT_IA

/*-----------------------------------------------------------------------------
                              * Local variables
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* External Function Prototypes
*----------------------------------------------------------------------------*/
#ifdef USE_LOGFILE
extern SFS_HANDLE yuyulog_ptr;//
#endif
/*-----------------------------------------------------------------------------
* Local Function Prototypes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Global functions
*----------------------------------------------------------------------------*/

void kjava_sys_log_printOut(char *message)
{   
#ifdef USE_LOGFILE
	uint32 written;
	if(yuyulog_ptr!=0){
		SFS_WriteFile(yuyulog_ptr,message,strlen(message),&written,0);
		SFS_WriteFile(yuyulog_ptr,"\n",1,&written,0);
	}
#endif
    //SCI_TRACE_LOW:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_LOG_OUT_49_112_2_18_2_23_23_398,(uint8*)"s", message);
    return;
}
#endif
/*-----------------------------------------------------------------------------
* Local functions
*----------------------------------------------------------------------------*/
