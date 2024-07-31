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

/**
 * @file
 *
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "sig_code.h"
#include "tasks_id.h"
#include "mmijava_main.h"
#include "mmi_appmsg.h"
#include "mmifilearray_export.h"
#include "guirichtext.h"
#include "sci_types.h"

#include "mmijava_main.h"
#include "javacall_lcd.h"
#include "javacall_lifecycle.h"
#include "javacall_memory.h"
#include "javacall_logging.h"
#include "javacall_sms.h"
#include "mmicc_export.h"
#include "mmiphone_export.h"
#ifdef BROWSER_SUPPORT
#include"mmibrowser_export.h"
#endif

#include"caf.h"
#include"mmi_common.h"
#include "mmi_nv.h"

#ifdef CMCC_UI_STYLE
#include "gpio_prod_api.h"
#endif

extern void javanotify_set_tck_mode_property(void);
typedef struct {
    javacall_utf16 *fileUrl;
    int *fileUrlLength;
} FILE_BROWSER_URL_T;

typedef struct {
    char *url;
} WAP_BROWSER_URL_T;

extern javacall_int32 getCurrentMIDletSuiteId(void);
LOCAL FILE_BROWSER_URL_T s_fileUrlData;
LOCAL WAP_BROWSER_URL_T s_wapUrlData = {0};
LOCAL int s_amsAutoStart = 0;
LOCAL javacall_lifecycle_state s_vmState = JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN;
LOCAL javacall_lifecycle_ground_state s_vmGroundState = JAVACALL_LIFECYCLE_MIDLET_FOREGROUND;
// The variables just for demo
#define MAX_SUITE_INFO_LEN      128
typedef struct {
    javacall_utf16 content[MAX_SUITE_INFO_LEN];
    int contentLen;
} SUITE_INFO_T;
LOCAL SUITE_INFO_T s_version;
LOCAL SUITE_INFO_T s_name;

/**
 * Starts a new process to handle the given URL. The new process executes
 * the value of the <tt>com.sun.midp.midlet.platformRequestCommand</tt>
 * system property. The URL is passed as this process' sole command-line
 * argument.
 *
 * If the platform has the appropriate capabilities and resources available, 
 * it SHOULD bring the appropriate application to the foreground and let the 
 * user interact with the content, while keeping the MIDlet suite running in 
 * the background. If the platform does not have appropriate capabilities or 
 * resources available, it MAY wait to handle the URL request until after the 
 * MIDlet suite exits. In this case, when the requesting MIDlet suite exits, 
 * the platform MUST then bring the appropriate application (if one exists) to 
 * the foreground to let the user interact with the content.
 * 
 * This is a non-blocking method. In addition, this method does NOT queue multiple 
 * requests. On platforms where the MIDlet suite must exit before the request 
 * is handled, the platform MUST handle only the last request made. On platforms 
 * where the MIDlet suite and the request can be handled concurrently, each
 * request that the MIDlet suite makes MUST be passed to the platform software 
 * for handling in a timely fashion.
 *
 * If the URL specified is of the form tel:<number>, as specified in RFC2806 
 * (http://www.ietf.org/rfc/rfc2806.txt), then the platform MUST interpret this as 
 * a request to initiate a voice call. The request MUST be passed to the phone 
 * application to handle if one is present in the platform. The phone application, 
 * if present, MUST be able to set up local and global phone calls and also perform 
 * DTMF post dialing. Not all elements of RFC2806 need be implemented, especially 
 * the area-specifier or any other requirement on the terminal to know its context. 
 * The isdn-subaddress, service-provider and future-extension may also be ignored. 
 * Pauses during dialing are not relevant in some telephony services. Devices MAY 
 * choose to support additional URL schemes beyond the requirements listed above.
 *
 * @param pszUrl An ascii URL string 
 *
 * @return <tt>JAVACALL_OK</tt> if the platform request is configured, and the MIDlet
 *                             suite MUST first exit before the content can be fetched.
 *         <tt>JAVACALL_FAIL</tt> if the platform request is configured, and the MIDlet
 *                             suite don't need to exit while the content can be fetched.
 *         <tt>JAVACALL_CONNECTION_NOT_FOUND</tt> if the platform request URL is not supported.
 */
javacall_result javacall_lifecycle_platform_request(char* urlString)
{
    char* pszTel = PNULL;
#ifdef BROWSER_SUPPORT    
    javacall_utf16 * dst = PNULL;
    javacall_int32 length = 0;
#endif

    javacall_printf(LOG_STR("urlString = %s"),urlString);  
    
    if(!strncmp("tel://", urlString, 6))
    {
        pszTel = (char *)(urlString+6);     
        MMIAPICC_MakeCall(MN_DUAL_SYS_1,(uint8*)pszTel,strlen(pszTel),PNULL,PNULL,CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,PNULL);      
        javacall_printf(LOG_STR("tel:// call ok"));
    }   
    else  if(!strncmp("tel:", urlString, 4))
    {
        pszTel = (char *)(urlString+4);     
        MMIAPICC_MakeCall(MN_DUAL_SYS_1,(uint8*)pszTel,strlen(pszTel),PNULL,PNULL,CC_CALL_SIM_MAX,CC_CALL_NORMAL_CALL,PNULL);      
        javacall_printf(LOG_STR("tel: call ok"));
    }   
    else if(!strncmp("http://", (char *)urlString, 7))
    {
 #ifdef BROWSER_SUPPORT      
        dst = (javacall_uint8*)SCI_ALLOCA(strlen(urlString)*sizeof(javacall_utf16));
        if(NULL == dst)
        {
            javacall_printf(LOG_STR_LEAVE("SCI_ALLOCA failed"));
            return JAVACALL_OUT_OF_MEMORY;
        }
        length = strlen(urlString);
	SCI_MEMSET(dst, 0, length*sizeof(wchar));   /*lint !e737 */
        MMIAPICOM_StrToWstr((uint8*)urlString, dst);
        javacall_lifecycle_launchNativeWebBrowser(dst,MMIAPICOM_Wstrlen(dst));
        MMIJAVA_SendJVMMessage2MMI(APP_JAVA_END_IND, LAUNCH_WAP_BROWSER);

        SCI_FREE(dst);
        javacall_printf(LOG_STR("http:// call ok"));

	 //MMIAPIBROWSER_Entry(&brw_parm);
#endif
    }
    
    return JAVACALL_OK;
}

javacall_result javacall_get_string_from_local_file_browser(
    /*IN-OUT*/javacall_utf16* fileUrl,
    /*IN-OUT*/int* fileUrlLength) {
    s_fileUrlData.fileUrl = fileUrl;
    s_fileUrlData.fileUrlLength = fileUrlLength;
    *s_fileUrlData.fileUrlLength = 0;

    MMIJAVA_SendJVMMessage2MMI(APP_JAVA_LAUNCH_FILE_BROWSER_IND, LAUNCH_FILE_BROWSER);
    return JAVACALL_WOULD_BLOCK;
}

void javacall_get_file_browser_data(FILEARRAY file_array) {     /*lint -e{765}*/
    uint32  file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    int tmpFileNameLen = 0;

    file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
    if (file_num > 0) {
        //read user selected dir
        MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data);

        if (file_array_data.name_len <= MMIFILE_FULL_PATH_MAX_LEN) {
            SCI_MEMCPY(s_fileUrlData.fileUrl,L"file:///",8*sizeof(wchar));
            tmpFileNameLen += 8;

            MMIAPICOM_Wstrcpy(s_fileUrlData.fileUrl+tmpFileNameLen, file_array_data.filename);

            tmpFileNameLen = MMIAPICOM_Wstrlen(s_fileUrlData.fileUrl);
        } else {
            javacall_printf("[javacall_get_file_browser_data]>> file name length > 255");
        }
    }

    *s_fileUrlData.fileUrlLength = tmpFileNameLen;
}

javacall_result javacall_lifecycle_launchNativeWebBrowser(/*IN*/javacall_utf16* url,
    /*IN*/int urlLength) {
    
    uint16 available_sim_index = 0;
    uint16 max_sim_num = 1;
    uint32 available_sim_num = 0;
    
    available_sim_num = MMIAPIPHONE_GetSimAvailableNum(&available_sim_index, max_sim_num);
    if(available_sim_num > 0)
    {
        s_wapUrlData.url = javacall_malloc(urlLength + 1);
        MMI_WSTRNTOSTR((uint8*)(s_wapUrlData.url),  GUIRICHTEXT_URL_MAX_LEN, url, GUIRICHTEXT_URL_MAX_LEN, urlLength);
        javacall_printf(LOG_STR("s_wapUrlData.url = %s"),s_wapUrlData.url);
        return JAVACALL_OK;
    }
    else
    {
        javacall_printf(LOG_STR("no sim "));
        return JAVACALL_FAIL;
    }
}

char * javacall_lifecycle_get_wapUrlPointer() {
    return s_wapUrlData.url;
}

void javacall_lifecycle_release_url_memory() {
    if (s_wapUrlData.url != NULL) {
        javacall_free((void *)s_wapUrlData.url);
        s_wapUrlData.url = NULL;
    }
}

void javacall_lifecycle_set_ams_auto_start(int autoStart) {
    s_amsAutoStart = autoStart;
}

int javacall_lifecycle_is_ams_auto_start() {
    return s_amsAutoStart;
}

void javacall_notify_suiteID(int suiteID)
{
    JAVAMMI_javacall_notify_suiteID(suiteID);
}
/**
 * Inform on change of the lifecycle status of the VM
 *
 * Java will invoke this function whenever the lifecycle status of the running
 * MIDlet is changes, for example when the running MIDlet has entered paused
 * status, the MIDlet has shut down etc.
 *
 * @param state new state of the running MIDlet. can be either,
 *        <tt>JAVACALL_LIFECYCLE_MIDLET_STARTED</tt>
 *        <tt>JAVACALL_LIFECYCLE_MIDLET_PAUSED</tt>
 *        <tt>JAVACALL_LIFECYCLE_MIDLET_RESUMED</tt>
 *        <tt>JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN</tt>
 *        <tt>JAVACALL_LIFECYCLE_MIDLET_INSTALL_COMPLETED</tt>
 * @param status return code of the state change
 *        If state is JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN, the status
 *        param will be set to <tt>JAVACALL_OK</tt> if MIDlet closed
 *        gracefully or <tt>JAVACALL_FAIL</tt> if MIDlet was killed
 *        If state is JAVACALL_LIFECYCLE_MIDLET_INSTALL_COMPLETED,
 *        status param will be set to <tt>JAVACALL_OK</tt> if MIDlet
 *        was installed successfully, or <tt>JAVACALL_FAIL</tt> if
 *        installation failed
 *        For states other than JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN and
 *        JAVACALL_LIFECYCLE_MIDLET_INSTALL_COMPLETED the parameter
 *        status is not used.
 */
extern void javacall_i18n_init(void);
extern void javacall_time_finalize_all(void);
//extern BOOLEAN g_java_paused_when_background;/*!e526*/
void javacall_lifecycle_state_changed(javacall_lifecycle_state state,
                                      javacall_result status){
    javacall_lifecycle_state oldState=s_vmState;                                      
#ifdef JAVA_LOGGING_FILE_ENABLE
    if (oldState == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN && state == JAVACALL_LIFECYCLE_MIDLET_CREATING)
        javacall_logging_file_start();
    if (state == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN)
        javacall_logging_file_end();
#endif
    s_vmState = state;

    javacall_printf("javacall_lifecycle_state_changed state is %d",state);    
    switch(state) {
    case  JAVACALL_LIFECYCLE_MIDLET_CREATING:
#ifdef CMCC_UI_STYLE
        GPIO_SetLcdBackLight(TRUE);
#endif
        if (oldState == JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN) {
	        javanotify_set_tck_mode_property();
    	    javacall_i18n_init();
        }
        break;
    case  JAVACALL_LIFECYCLE_MIDLET_STARTED:
        MMIJAVA_javacall_lifecycle_midlet_started();    
        break;
    case  JAVACALL_LIFECYCLE_MIDLET_PAUSED:
        break;
    case  JAVACALL_LIFECYCLE_MIDLET_PAUSING:
        break;
    case JAVACALL_LIFECYCLE_MIDLET_RESUMED:
       	javacall_lcd_flush(0);
        break;
    case  JAVACALL_LIFECYCLE_MIDLET_INTERNAL_PAUSED:
        break;
    case JAVACALL_LIFECYCLE_MIDLET_INTERNAL_RESUMED:
        //javacall_lcd_flush(0);
        break;
    case JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN:
        javacall_lifecycle_set_ground_state(JAVACALL_LIFECYCLE_MIDLET_FOREGROUND);
        //MMIJAVA_CloseAllMediaPlayer();
        javacall_printf("javacall_lifecycle_state_changed >> JAVACALL_LIFECYCLE_MIDLET_SHUTDOWN, s_amsAutoStart=%d", s_amsAutoStart);
        javacall_time_finalize_all();

        if (s_amsAutoStart != 0) {	
            s_amsAutoStart = 0;
            MMIJAVA_SendJVMMessage2MMI(APP_JAVA_START_AMS_IND, START_AMS);
			break;
        } 
        JAVAMMI_REMOVE_TMP_DOWNLOAD_FILE();//if s_amsAutoStart is 1,don't execute JAVAMMI_REMOVE_TMP_DOWNLOAD_FILE
		if (s_wapUrlData.url == NULL) {
            MMIJAVA_SendJVMMessage2MMI(APP_JAVA_END_IND, SHUTDOWN_JAVA);
        } else {
            javacall_printf(LOG_STR("send LAUNCH_WAP_BROWSER"));
            MMIJAVA_SendJVMMessage2MMI(APP_JAVA_LAUNCH_WAP_BROWSER_IND, LAUNCH_WAP_BROWSER);
        }
        break;
    case JAVACALL_LIFECYCLE_MIDLET_INSTALL_COMPLETED:
        javacall_print("event is JAVACALL_LIFECYCLE_MIDLET_INSTALL_COMPLETED ");
        break;
    default:
        javacall_print("event is UNKNOWN ");
    }
}

void javacall_lifecycle_operation_complete(javacall_operation operation,        /*lint -e{765}*/
                                      javacall_result status,
                                      struct javacall_lifecycle_push_entry_info *info) {

    javacall_printf(LOG_STR_ENTER("operation=%d, status=%d"), operation, status);

    switch(operation) {
    case JAVACALL_OP_GET_PUSH_ENTRY: {
        if(status == JAVACALL_OK) {
            Prepare_Launch_Midlet_Parameter(atoi(info->suiteId), info->className);
            MMIJAVA_OpenMainMenu();
        } else {
                javacall_sms_cleanup_pending_sms();
        }
        break;
    }
    default:
        break;  
    }

}
javacall_lifecycle_state javacall_lifecycle_get_vm_state() {
    return s_vmState;
}

javacall_lifecycle_ground_state javacall_lifecycle_get_ground_state() {
    return s_vmGroundState;
}

void javacall_lifecycle_set_ground_state(javacall_lifecycle_ground_state stat) {
    s_vmGroundState=stat;
}
void javacall_lifecycle_set_vm_state(javacall_lifecycle_state state) {
    s_vmState = state;
} 

javacall_result javacall_lifecycle_set_suite_info(int suiteId, javacall_utf16* version, int versionLength, 
                                                  javacall_utf16* name, int nameLength) 
{
    s_version.contentLen = versionLength>MAX_SUITE_INFO_LEN? MAX_SUITE_INFO_LEN:versionLength;
    if (s_version.contentLen > 0) {
        SCI_MEMCPY(s_version.content, version, (uint16)s_version.contentLen * sizeof(javacall_utf16));
    }
    
    s_name.contentLen = nameLength>MAX_SUITE_INFO_LEN? MAX_SUITE_INFO_LEN:nameLength;
    if (s_name.contentLen > 0) {
        SCI_MEMCPY(s_name.content, name, (uint16)s_name.contentLen * sizeof(javacall_utf16));
    }
    JAVAMMI_javacall_lifecycle_set_suite_info(suiteId,s_version.content,s_version.contentLen,s_name.content,s_name.contentLen);
    return JAVACALL_OK;
}

javacall_int32 javacall_lifecycle_get_running_suiteId()
{
    return getCurrentMIDletSuiteId();   /*lint !e746 */
}
void jvm_scheduler_callback()
{
    //sleep 10 ms, give up cpu. This time should be more tested.
    if ((s_vmState == JAVACALL_LIFECYCLE_MIDLET_PAUSED)
        ||(s_vmGroundState == JAVACALL_LIFECYCLE_MIDLET_BACKGROUND))
      SCI_Sleep(10);
}
#ifdef __cplusplus
}
#endif


