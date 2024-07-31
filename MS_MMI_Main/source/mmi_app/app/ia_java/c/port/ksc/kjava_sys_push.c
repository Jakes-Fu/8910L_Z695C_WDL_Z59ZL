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
#include "jblend_config.h"
#ifdef JAVA_SUPPORT_IA
#ifdef JAVA_SUPPORT_SHRINK_RAM
#include "kjava_shrink_ram.h"
#endif
#include "kjava_sys_push.h"
#include "kjava_sys_core.h"
#include "jblend_platform.h"
#include "mmijava_internal.h"
#include "mmipub.h"
#include "mmi_image.h"
//extern BOOLEAN MMIAPIFMM_GetDeviceTypeStatus(MMIFILE_DEVICE_E memory_type);

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define KJAVA_PUSH_DB_MAX_SIZE 12
#define JAVA_PUSHNAME "push.dat"
#define JAVA_PUSHNAME_LEN 8
/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct _KJava_Push_ConnRecord
{
    int32                      used; 
    /**
    * \brief The connection protocol;
    */
    uint8   protocol[20];
    /**
    * \brief The connection port number     
    */
    int32 port;
    /**
    * \brief The URL filter string, in UTF-8 characters.
    */
    uint8  filter[60]; 
    int32 suiteId;
    int32 midletId;
} KJava_Push_ConnRecord;


typedef struct _KJava_Push_AlarmRecord
{
    int32     used; 
    /**
    * \brief The higher 32 bits of the activating time.
    */
    uint32 high;
    /**
    * \brief The lower 32 bits of the activating time.
    */
    uint32 low;
    int32 suiteId;
    int32 midletId;
} KJava_Push_AlarmRecord;


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef JAVA_SUPPORT_SHRINK_RAM
static KJava_Push_ConnRecord    s_java_pushConnRecords1[ KJAVA_PUSH_DB_MAX_SIZE ] = {0};
// This is the database of connection records

static KJava_Push_AlarmRecord   s_kjava_pushAlarmRecords1[ KJAVA_PUSH_DB_MAX_SIZE ] = {0};
// This is the database of alarms
#else
DEFINE_STATIC_SHRINK_VAR_1_DIM(KJava_Push_ConnRecord,s_java_pushConnRecords1)
DEFINE_STATIC_SHRINK_VAR_1_DIM(KJava_Push_AlarmRecord,s_kjava_pushAlarmRecords1)
#endif
LOCAL BOOLEAN s_is_initialized = FALSE;
LOCAL SCI_TIMER_PTR s_push_timer = PNULL;

LOCAL BOOLEAN                       s_is_needContinuePush = FALSE;
LOCAL KJava_Push_AlarmRecord*       s_java_push_info_ptr = PNULL;    

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : push_alarm_func
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void push_alarm_func(uint32 param);

/*****************************************************************************/
//  Description : pushSMScallback
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void pushSMScallback(int32 event, SMS_Message *argv);
/*****************************************************************************/
//  Description : SavePushDataBase
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SavePushDataBase(void);
/*****************************************************************************/
//  Description : LoadPushDataBase
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void LoadPushDataBase(void);


#ifdef JAVA_SUPPORT_SHRINK_RAM
BEGIN_DEFINE_SHRINK_VAR_INIT(kjava_sys_push)
	SHRINK_VAR_MEMORY_1_DIM(KJava_Push_ConnRecord,s_java_pushConnRecords1,KJAVA_PUSH_DB_MAX_SIZE)
	SHRINK_VAR_MEMORY_1_DIM(KJava_Push_AlarmRecord,s_kjava_pushAlarmRecords1,KJAVA_PUSH_DB_MAX_SIZE)
END_DEFINE_SHRINK_VAR_INIT

BEGIN_DEFINE_SHRINK_VAR_FINI(kjava_sys_push)
	FREE_SHRINK_VAR_MEMORY(s_java_pushConnRecords1)
	FREE_SHRINK_VAR_MEMORY(s_kjava_pushAlarmRecords1)
END_DEFINE_SHRINK_VAR_FINI
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : kjava_push_init
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void kjava_push_init(void)
{
    int32 i = 0;
    
    JWE_LOG(kjava_push_init, ("kjava_push_init enter"));    
    Ajava_Initiliztion_Special();
    if ( !s_is_initialized ) 
    { 
        SCI_MEMSET(s_java_pushConnRecords1, 0, (sizeof(KJava_Push_ConnRecord)*KJAVA_PUSH_DB_MAX_SIZE));
        SCI_MEMSET(s_kjava_pushAlarmRecords1, 0, (sizeof(KJava_Push_AlarmRecord)*KJAVA_PUSH_DB_MAX_SIZE));

        LoadPushDataBase();

        for ( i = 0; i < KJAVA_PUSH_DB_MAX_SIZE; i++ ) 
        {
            if (s_java_pushConnRecords1[i].used )
            {
                MMIJAVA_RegSMSPortCallback(s_java_pushConnRecords1[i].port, pushSMScallback);/*lint !e64*/
            }
        }
        s_is_initialized  = TRUE;
    }
    
    JWE_LOG(kjava_push_init, ("leave"));
}


/**
* \brief Start an alarm.
*
* The function informs the native system to start an alarm at the given time.
*
* The time is stored in the structure as elapsed milliseconds from
* 00:00:00.000 GMT, January 1, 1970.
*
* The native system should be informed when the alarm expires, and uses AJMS
* functions to launch the MIDlet associated with the alarm.
*
* \param high The higher 32 bits of the time information.
* \param low  The lower 32 bits of the time information.
*
* The following operations will occur: 
* 1. verify that the alarm is valid time, i.e., (alarm time - current time) > 0
* 2. add alarm to alarm database    
* 3. if 1 and 2 are ok, start the alarm 
* 4. if 1-3 are ok, add the alarm to file 
* 
* \return If the operation is successful, return 1. Otherwise, return 0.
*/
int kjava_push_startAlarm(int suiteId, int midletId, unsigned long high, unsigned long low)
{
    uint32  syshigh = 0;
    uint32  syslow = 0;
    uint32  waittime = 0;
    int64_t alarm_millisecond = 0;
    int64_t cur_millisecond = 0;
    int32   i = 0;
    
    JWE_LOG(kjava_push_startAlarm, ("kjava_push_startAlarm enter, high = %d,low = %d", high, low) );
    JWE_LOG(kjava_push_startAlarm,("enter, suiteId = %d, midletId = %d", suiteId, midletId));
    
    kjava_push_init();
    kjava_core_getCurrentTime(&syshigh ,&syslow);
    JWE_LOG(kjava_push_startAlarm, ("kjava_push_startAlarm current time: syshigh = %d, syslow = %d",syshigh, syslow));
    
    alarm_millisecond = (int64_t)((int64_t)high << 32) + (int64_t)low;
    cur_millisecond = (int64_t)((int64_t)syshigh << 32) + (int64_t)syslow;
    
    waittime = (uint32)((alarm_millisecond - cur_millisecond)+1441);
    JWE_LOG(kjava_push_startAlarm,("waittime is 0x%08x", waittime));
    
    if(waittime == 0 || waittime >= 0x7fffffff)
    {
        JWE_LOG(kjava_push_startAlarm,("alarm time is older than current time"));
        JWE_LOG(kjava_push_startAlarm,("Failed"));
        return 0;
    }
    
    while (i < KJAVA_PUSH_DB_MAX_SIZE)  
    {
        if ( ! s_kjava_pushAlarmRecords1[i].used )
        {
            s_kjava_pushAlarmRecords1[i].high = high;
            s_kjava_pushAlarmRecords1[i].low = low;
            s_kjava_pushAlarmRecords1[i].suiteId = suiteId;
            s_kjava_pushAlarmRecords1[i].midletId = midletId;
            s_kjava_pushAlarmRecords1[i].used = 1;
            break;
        }
        i++;
    }
    if(i == KJAVA_PUSH_DB_MAX_SIZE)
    {
        JWE_LOG(kjava_push_startAlarm,("no space for new alarm, over flow"));
        JWE_LOG(kjava_push_startAlarm,("Failed"));
        return 0;
    }
    
    JWE_LOG(kjava_push_startAlarm,("create alarm timer"));
    if (PNULL == s_push_timer)
    {
        s_push_timer = SCI_CreateTimer("JavaPushTimer", push_alarm_func, (int)&s_kjava_pushAlarmRecords1[i], waittime, SCI_AUTO_ACTIVATE);
        if(PNULL == s_push_timer)
        {   
            JWE_LOG(kjava_push_startAlarm,("timer fail"));
            JWE_LOG(kjava_push_startAlarm,("Failed"));
            return 0;
        }
        else
        {
            JWE_LOG(kjava_push_startAlarm,("create timer OK"));
        }
    }
    else
    {
        if (!SCI_IsTimerActive(s_push_timer))
        {
             SCI_ActiveTimer(s_push_timer);
        }
       
    }
    
    return 1;
    

}

/**
* \brief Stop an Push connection.
*
* The function informs the native system to stop the Push connection of the
* given URL (protocol + port).
*
* \param protocol       The connection type (sms, socket, datagram)
* \param port           The local port number associated with the protocol type
*
* The following operations will occur: 
* 1. verify that <connection> is a valid URL
* 2. remove <connection> from push registry database (kjava_pushConnRecords1)
* 3. if 1 and 2 are ok, stop the listener   
* 4. remove the <connection> from file
*
* \return If the operation is successful, return 1. Otherwise, return 0.
*/ 
int kjava_push_stopConn( unsigned char* protocol, int port )
{
    int32 i=0;
    
    JWE_LOG(kjava_push_stopConn, ("Enter: protocol %s, port %d", protocol, port) );
    
    kjava_push_init();
    
    // 1. verify that <connection> is a valid URL
    if ( strncmp( (char *)protocol, "sms://", 6 ) != 0 )
    {
        JWE_LOG(kjava_push_stopConn, ("it is not a legal URL") );
        return 0;
    }
    else
    {
        JWE_LOG(kjava_push_stopConn, ("it is a legal URL") );
    }
    
    while (i < KJAVA_PUSH_DB_MAX_SIZE)  
    {   
        if (s_java_pushConnRecords1[i].used) 
        {               
            if (strcmp((char *)s_java_pushConnRecords1[i].protocol, (char *)protocol) == 0  
                && s_java_pushConnRecords1[i].port == port )
            {
                // 2. remove <connection> from push registry database
                SCI_MEMSET(&s_java_pushConnRecords1[i], 0, sizeof(KJava_Push_ConnRecord));
                
                // 3. if 1 and 2 are ok, stop the listener  
                MMIJAVA_UnRegSMSPortCallback(port);
                
                // 4. remove the <connection> from file
                SavePushDataBase(); 
                JWE_LOG(kjava_push_stopConn,("kjava_push_stopConn OK"));
                return 1;
            }       
        }
        i++;
    }
    
    JWE_LOG(kjava_push_stopConn,("kjava_push_stopConn Failed"));    
    return 0;
}

/**
* \brief Start an Push connection.
*
* The function informs the native system to start a Push connection
* monitoring of the given URL (given by the protocol and port number).
*
* The native system should be informed when an incoming connection is created
* from the specified URL, and uses AJMS functions to launch the MIDlet
* associated with the URL.
*
* A filter string is also specified so native system could use it to filter
* wanted inbound connections. The syntax and semantic is protocol depedent. 
* But wildcard characters should be supported. Please refer to the
* spec of <code>javax.microedition.io.PushRegistry</code> for detail
* information.
*
* \param protocol         The connection type (sms, socket, datagram)
* \param port             The local port number associated with the protocol type
* \param filter           The URL filter string, in UTF-8 characters. 
*
* The following operations will occur: 
* 1. parse <connection> and verify that it is a legal URL that is supported by target platform
* 2. verify that <connection> is not part of the URL list that is being filtered by <filter>
* 3. if 1 and 2 are ok, add to push registry database
* 4. if 1-3 are ok, start listener
* 5. if 1-4 are ok, add connection to file
*  
* \return If the operation is successful, return 1. Otherwise, return 0.
*/   
int kjava_push_startConn(int            suiteId, 
                         int            midletId, 
                         unsigned char* protocol, 
                         int            port,
                         unsigned char* filter)
{
    int32 i=0;
    
    JWE_LOG(kjava_push_startConn, ("Enter, suiteId %d, midletId %d", suiteId, midletId) );
    JWE_LOG(kjava_push_startConn, ("protocol %s, port %d, filter %s", protocol, port, filter) );
    
    kjava_push_init();
    
    /* 1. parse <connection> and verify that it is a legal URL that is supported by target platform */
    if(!kjava_push_isAvailable(protocol, port))
    {
        JWE_LOG(kjava_push_startConn, ("it is not a legal URL") );
        return 0;
    }
    else
    {
        JWE_LOG(kjava_push_startConn, ("it is a legal URL") );
    }
    
    /* 2. verify that <connection> is not part of the URL list that is being filtered by <filter> */
    if(strstr( (char *)filter, (char *)protocol) == 0)
    {
        JWE_LOG(kjava_push_startConn, ("it is not part of the URL list that is being filtered") );
    }
    else 
    {
        JWE_LOG(kjava_push_startConn, ("it is part of the URL list that is being filtered") );      
        return 0;
    }
    
    while (i < KJAVA_PUSH_DB_MAX_SIZE)  
    {
        if ( ! s_java_pushConnRecords1[i].used )
        {
            // 3. add to push registry database
            SCI_MEMSET(&s_java_pushConnRecords1[i], 0, sizeof(KJava_Push_ConnRecord));
            
            s_java_pushConnRecords1[i].used      = 1;    
            s_java_pushConnRecords1[i].port      = port;
            s_java_pushConnRecords1[i].suiteId   = suiteId;
            s_java_pushConnRecords1[i].midletId  = midletId;
            SCI_STRCPY((char*)s_java_pushConnRecords1[i].protocol, (char*)protocol);
            SCI_STRCPY((char*)s_java_pushConnRecords1[i].filter, (char*)filter);
            
            // 4. open a connection listener for that <connection>  and listen  
            MMIJAVA_RegSMSPortCallback(port, pushSMScallback);/*lint !e64*/
            
            // 5. add connection to file
            JWE_LOG(kjava_push_startConn, ("add to push registry database") );      
            SavePushDataBase();
            JWE_LOG(kjava_push_startConn, ("kjava_push_startConn OK")); 
            return 1;
        }       
        i++;        
    }
    JWE_LOG(kjava_push_startConn, ("kjava_push_startConn Failed")); 
    return 0;
}


/**
* \brief Check if the specified URL has inbound data available.
*
* The function asks the native system whether the given URL (protocol + port number) 
* has inbound data buffered and is available to be read.
*
* \param protocol       The connection type (sms, socket, datagram)
* \param port           The local port number associated with the protocol type
*
* The following operations will occur: 
* 1. verify that <connection> is a legal URL
* 2. check that  <connection> has inbound data
* 
* \return If the URL is available, return 1. Otherwise, return 0.
*/
int kjava_push_isAvailable(unsigned char* protocol, unsigned int port)
{
    int32 i = 0;
    
    JWE_LOG(kjava_push_isAvailable,("Enter,port=%d",port));
    JWE_LOG(kjava_push_isAvailable,("protocol=%s",protocol));
    if ( strncmp( (char *)protocol, "sms://", 6 ) != 0 )
    {
        return 0;
    }
    
    for(i = 0; i < KJAVA_PUSH_DB_MAX_SIZE; i++)
    {
        if( s_java_pushConnRecords1[i].used == 1 && s_java_pushConnRecords1[i].port == (int32)port)
        {
            JWE_LOG(kjava_push_isAvailable, ("port is not available!"));
            return 0;
        }
    }
    JWE_LOG(kjava_push_isAvailable,("protocol and port ok!"));
    
    return 1;
}

/*****************************************************************************/
//  Description : push_alarm_func
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void push_alarm_func(uint32 param)
{
    MMIAPIJAVA_SendSignal(MSG_JAVA_AUTORUNAPP,param);       
    return; 
}

/*****************************************************************************/
//  Description : pushSMScallback
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void pushSMScallback(int32 event, SMS_Message *argv)
{
    int32 index = 0;
    // TODO: check the port
    index = MMIJAVA_getConnByPort(argv->dstPort);
    if(index == -1)
    {
        JWE_LOG(pushSMScallback, ("unregisted port"));
		return;
    } 
	if(event != JAVA_NEW_MT_IND)
	{
 		JWE_LOG(pushSMScallback, ("not JAVA_NEW_MT_IND event"));
		return;
	}

    JWE_LOG(pushSMScallback, ("MMIDEFAULT_SetBackLight"));
    MMIDEFAULT_SetBackLight(TRUE);
    
	MMIJAVA_Add2SMSReceiveQue(argv);
    MMIJAVA_ASA_autorunApp(s_java_pushConnRecords1[index].suiteId, s_java_pushConnRecords1[index].midletId);
	return ;
}

/*****************************************************************************/
//  Description : SavePushDataBase
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SavePushDataBase(void)
{
    SFS_HANDLE          handle = NULL;
    uint32              written = 0;//written is not used
    wchar               full_path[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    KJavaPathMappingEntry javafolderpathmap;
    char                push_data_file[MMIFILE_FULL_PATH_MAX_LEN + 1]={0};

    kjava_fs_getPath(0, &javafolderpathmap);//获得系统目录的路径
    sprintf(push_data_file,"%s%s",javafolderpathmap.path,JAVA_PUSHNAME);/*lint !e606*/

    JWE_LOG(SavePushDataBase,("%s",push_data_file));

    GUI_UTF8ToWstr(full_path, MMIFILE_FULL_PATH_MAX_LEN,(uint8*)push_data_file,MMIFILE_FULL_PATH_MAX_LEN);

    handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE, 0, 0);
    if(handle != 0)
    { 
    	MMIAPIFMM_WriteFile((SFS_HANDLE)handle, s_java_pushConnRecords1,  (uint32)(sizeof(KJava_Push_ConnRecord)*KJAVA_PUSH_DB_MAX_SIZE), (uint32 *)&written,0/*&overlapped_ptr*/);
    	JWE_LOG(SavePushDataBase,("write %d bytes", written))
    		MMIAPIFMM_CloseFile(handle);
    }
    else
    {
    	JWE_LOG(SavePushDataBase,("open file failed!"))	
    }
    
}

/*****************************************************************************/
//  Description : LoadPushDataBase
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void LoadPushDataBase(void)
{
    SFS_HANDLE          handle = NULL;
    uint32              read = 0;//written is not used
    wchar               full_path[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    KJavaPathMappingEntry javafolderpathmap;
    char                push_data_file[MMIFILE_FULL_PATH_MAX_LEN + 1]={0};

    kjava_fs_getPath(0, &javafolderpathmap);//获得系统目录的路径
    sprintf(push_data_file,"%s%s",javafolderpathmap.path,JAVA_PUSHNAME);/*lint !e606*/

    JWE_LOG(LoadPushDataBase,("%s",push_data_file));

    GUI_UTF8ToWstr(full_path, MMIFILE_FULL_PATH_MAX_LEN,(uint8*)push_data_file,MMIFILE_FULL_PATH_MAX_LEN);

    handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, 0, 0);
    if(handle != 0)
    {
        MMIAPIFMM_ReadFile((SFS_HANDLE)handle, s_java_pushConnRecords1, (uint32)(sizeof(KJava_Push_ConnRecord)*KJAVA_PUSH_DB_MAX_SIZE), (uint32 *)&read, 0);
        JWE_LOG(LoadPushDataBase,("read %d bytes", read))
        MMIAPIFMM_CloseFile(handle);
    }
    else
    {
        JWE_LOG(LoadPushDataBase,("open file failed!"))	
    }    
}

/*****************************************************************************/
//  Description : LoadPushDataBase
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_getConnByPort(int32 port)
{
    int32 i = 0;
    for(i = 0;i<KJAVA_PUSH_DB_MAX_SIZE;i++)
    {
        if(s_java_pushConnRecords1[i].used ==1&&s_java_pushConnRecords1[i].port == port)
            return i;
    }
    return -1;
}

/*****************************************************************************/
//  Description :HandleJavaQueryPushInRunningWinMsg
//  Global resource dependence : none
//  Author: 
//  Note: continue push after jvm exit 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleJavaQueryPushInRunningWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
	
    JWE_LOG(HandleJavaQueryPushInRunningWinMsg, ("enter"));
    switch(msg_id)
    {
	case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
		JWE_LOG(HandleJavaQueryPushInRunningWinMsg, ("OK, Quit Java and push"));
		s_is_needContinuePush= TRUE;
		MMK_CloseWin(win_id);
		MMIJAVA_stopVM_external(); 	
		break;

	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
	case MSG_KEYDOWN_RED:
		MMK_CloseWin(win_id);
		break;
	
	default:
		recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
		break;
    }
    
    return recode;

}

/*****************************************************************************/
//  Description :MMIJAVA_ContinuePush
//  Global resource dependence : none
//  Author: 
//  Note: continue push after jvm exit 
/*****************************************************************************/
PUBLIC void MMIJAVA_ContinuePush(void)
{
	if(s_is_needContinuePush)
    {
        SCI_Sleep(1000);
        JWE_LOG(MMIAPIJAVA_AutoRunApp, ("suitid %d, midletId %d", s_java_push_info_ptr->suiteId, s_java_push_info_ptr->midletId));    
        if(PNULL!=s_java_push_info_ptr)
        {
            MMIJAVA_SetJavaIfRunningByMMI(TRUE);
            MMIJAVA_ASA_autorunApp(s_java_push_info_ptr->suiteId, s_java_push_info_ptr->midletId);
        }
        s_is_needContinuePush= FALSE;
    }
}


/*****************************************************************************/
//  Description : auto run app 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_AutoRunApp(uint32 param)
{
    JWE_LOG(MMIAPIJAVA_AutoRunApp, ("enter timer"));
  	if(PNULL != s_push_timer)
	{
		SCI_DeleteTimer( s_push_timer	);
		s_push_timer = PNULL;
	}  
 

    s_java_push_info_ptr = (KJava_Push_AlarmRecord*)param;
    JWE_LOG(MMIAPIJAVA_AutoRunApp, ("suitid %d, midletId %d", s_java_push_info_ptr->suiteId, s_java_push_info_ptr->midletId));    
  
    if(PNULL != s_java_push_info_ptr)
    {  
	   	if(!MMIJAVA_GetJavaIfRunningByMMI())
		{
			 MMIJAVA_ASA_autorunApp(s_java_push_info_ptr->suiteId, s_java_push_info_ptr->midletId);	 
		}
		else
		{
			MMIPUB_OpenQueryWinByTextId( TXT_JAVA_PUSH_IN_RUNNING, IMAGE_PUBWIN_QUERY,PNULL,HandleJavaQueryPushInRunningWinMsg ); 
	
		}		        
        s_java_push_info_ptr->used = 0;    
   }
    return; 
}

#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
