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
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "jblend_platform.h"
#include "dal_keypad.h"
#include "keypad.h"
#include "ajms_common.h"
#include "ajms_arc.h"
#include "ajms_ab.h"
#include "jwe_log.h"
#include "ajms_cfg_eime.h"
#include "udpresource.h"
#include "mmi_id.h"
#include "mmijava_internal.h"
#include "os_api.h"
#include "jblend_config.h"
#ifdef JAVA_SUPPORT_SHRINK_RAM
#include "kjava_shrink_ram.h"
#endif
#ifndef WIN32  
#ifdef PCLINK_SUPPORT
#include "tcpip_api.h"
#endif
#endif
#include "jblend_logfilter.h"
#include "priority_app.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define AJAVAKVE_TASK_NAME                  ("AJAVAKVE")                   
#define AJAVAKVE_TASK_QNAME                 ("AJAVAKVE_Q")                 
#define AJAVAKVE_TASK_STACK_SIZE            (16 * 1024)                
#define AJAVAKVE_TASK_QNUM                  (8)                          
#define AJAVAKVE_TASK_PRIORITY             PRI_AJAVAKVE_TASK //  (SCI_PRIORITY_IDLE)  
#define AJAVAKVE_TASK_PREEMPTIVE            (SCI_PREEMPT)              
#define AJAVAKVE_TASK_START_FLAG            SCI_DONT_START//    (SCI_AUTO_START)           


#define AJAVACOMM_TASK_NAME                 ("AJAVACOMM")
#define AJAVACOMM_TASK_QNAME                ("AJAVACOMM_Q")
#define AJAVACOMM_TASK_STACK_SIZE           (4 * 1024)
#define AJAVACOMM_TASK_QNUM                 (8)
#define AJAVACOMM_TASK_PRIORITY            PRI_AJAVACOMM_TASK  // (SCI_PRIORITY_LOWEST + 1)
#define AJAVACOMM_TASK_PREEMPTIVE           (SCI_PREEMPT)
#define AJAVACOMM_TASK_START_FLAG           SCI_DONT_START//    (SCI_AUTO_START)


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
BLOCK_ID      g_AKveId = 0;
BLOCK_ID      g_AComId = 0;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL AJMS_DEBUG_FilterCallback LogFilter;      // logging message filtering for jblendia

LOCAL BOOLEAN b_initializeJAVA = FALSE;
LOCAL BOOLEAN b_initializeJAVAUdp = FALSE;
LOCAL BOOLEAN b_initializeJAVACom = FALSE;
LOCAL BOOLEAN g_is_delete  = FALSE;

#ifdef USE_LOGFILE
SFS_HANDLE yuyulog_ptr=0;//
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef JAVA_SUPPORT_SHRINK_RAM
void* JkMjb2ShrinkRamMalloc(unsigned int size);
void JkMjb2ShrinkRamFree(void* p);
void Ajava_ShrinkRamInit(void);
void Ajava_ShrinkRamFini(void);
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Ajava_Initiliztion_ia
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
#ifdef JAVA_SUPPORT_SHRINK_RAM
void* JkMjb2ShrinkRamMalloc(unsigned int size){
 	return SCI_ALLOC_APP(size);
}

void JkMjb2ShrinkRamFree(void * p){
	SCI_FREE(p);
}

void Ajava_ShrinkRamInit(void){
	CALL_SHRINK_VAR_INIT(kjava_adv_fs)
	CALL_SHRINK_VAR_INIT(kjava_sys_gfx)
	CALL_SHRINK_VAR_INIT(kjava_sys_push)
	CALL_SHRINK_VAR_INIT(kjava_sys_sock)
	CALL_SHRINK_VAR_INIT(kjava_sys_wma)
	CALL_SHRINK_VAR_INIT(UDPResource)

}

void Ajava_ShrinkRamFini(void){
	CALL_SHRINK_VAR_FINI(kjava_adv_fs)
	CALL_SHRINK_VAR_FINI(kjava_sys_gfx)
	CALL_SHRINK_VAR_FINI(kjava_sys_push)
	CALL_SHRINK_VAR_FINI(kjava_sys_sock)
	CALL_SHRINK_VAR_FINI(kjava_sys_wma)
	CALL_SHRINK_VAR_FINI(UDPResource)
}
#endif
PUBLIC BOOLEAN Ajava_Initiliztion_ia(void)
{
    int32 ajms_ret  = 0;
    uint32 status = 0;
    
    //SCI_TRACE_LOW:"Ajava_Initiliztion_ia b_initializeJAVA = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JWE_83_112_2_18_2_23_4_344,(uint8*)"d",b_initializeJAVA);
    if(!b_initializeJAVA)
    {  
	#ifdef USE_LOGFILE
		uint16 wyuyufile[255];
		int wlength;//
		uint8  digit_name[64];//
		SCI_TM_T cur_time;//
		SCI_MEMSET(digit_name,0,sizeof(digit_name));
		TM_GetTime(&cur_time);//
		sprintf((char *)digit_name,"E:\\jblendia_filelog_%d_%d_%d_%d_%d_%d",cur_time.tm_year,cur_time.tm_mon,cur_time.tm_mday,cur_time.tm_hour,cur_time.tm_min,cur_time.tm_sec);//
		SCI_MEMSET((void *)wyuyufile,0,sizeof(wyuyufile));
		wlength = GUI_UTF8ToWstr((uint8 *)wyuyufile,255,(const uint8 *)digit_name,strlen((void *)digit_name));
		//wyuyufile[wlength] = 0;
		yuyulog_ptr = SFS_CreateFile((const uint16 *)wyuyufile,SFS_MODE_APPEND|SFS_MODE_READ|SFS_MODE_WRITE,0,0);
		if(yuyulog_ptr == 0){
			//SCI_TRACE_LOW:"Ajava_Initiliztion_ia yuyu log failed"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,JWE_99_112_2_18_2_23_4_345,(uint8*)"");
		}
		else{
			//SCI_TRACE_LOW:"Ajava_Initiliztion_ia yuyu log success"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,JWE_102_112_2_18_2_23_4_346,(uint8*)"");
		}
	#endif
	
        LogFilter = (AJMS_DEBUG_FilterCallback)jblendia_logFilter;
	SCI_TRACE_LOW("AmMjb2ShrinkRamInit enter");
#ifdef JAVA_SUPPORT_SHRINK_RAM
	 Ajava_ShrinkRamInit();
#endif
        ajms_ret = ajms_init(LogFilter);
        JWE_LOG(Ajava_Initiliztion_ia, ("Ajava_Initiliztion_ia -- ajms_ret = %d", ajms_ret));
        
        g_AKveId = SCI_CreateThread(AJAVAKVE_TASK_NAME, AJAVAKVE_TASK_QNAME, AJAVA_KVEMain, 0, NULL, AJAVAKVE_TASK_STACK_SIZE, 
            AJAVAKVE_TASK_QNUM, AJAVAKVE_TASK_PRIORITY, AJAVAKVE_TASK_PREEMPTIVE, AJAVAKVE_TASK_START_FLAG);
        //SCI_ASSERT(SCI_INVALID_BLOCK_ID != g_AKveId);
        
        if(SCI_INVALID_BLOCK_ID == g_AKveId)
        {
            JWE_LOG(Ajava_Initiliztion_ia, ("create kve thread failed"));
            return FALSE;
        }
        
        JWE_LOG(Ajava_Initiliztion_ia, ("create kve thread %08x", g_AKveId));
        status = SCI_ResumeThread(g_AKveId);
        JWE_LOG(Ajava_Initiliztion_ia, ("Resume kve thread, %d", status)); 
        b_initializeJAVA = TRUE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Ajava_Finalize_ia
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Ajava_Finalize_ia(void)
{
    uint32 status = 0;
    
    if( b_initializeJAVA)
    {
    	#ifdef USE_LOGFILE
		if(yuyulog_ptr!=0){
			SFS_CloseFile(yuyulog_ptr);
		}
		#endif
        JWE_LOG(Ajava_Finalize_ia, ("Ajava_Finalize_ia, Enter!"));
        
        status = SCI_TerminateThread(g_AKveId);
        JWE_LOG(Ajava_Finalize_ia, ("SCI_TerminateThread kve %d", status));
        
        status = SCI_DeleteThread(g_AKveId);
        JWE_LOG(Ajava_Finalize_ia, ("SCI_DeleteThread kve %d", status));
        
        g_AKveId = SCI_INVALID_BLOCK_ID;        

        ajms_fini();
        
        JWE_LOG(Ajava_Finalize_ia, ("Ajava_Finalize_ia, Leave!"));
        b_initializeJAVA = FALSE;
    }
#ifdef JAVA_SUPPORT_SHRINK_RAM
	Ajava_ShrinkRamFini();
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : Ajava_Initiliztion_ia
//  Global resource dependence : none
//  Author: huangjc
//  Note: 建立jvm系统文件和纪录文件，比如JAVA folder和suite.idx
/*****************************************************************************/
PUBLIC BOOLEAN Ajava_Initiliztion_Special(void)
{
    int32           ajms_ret  = 0;  
    wchar           full_path[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16          full_path_len = MMIFILE_FILE_NAME_MAX_LEN;    
    const wchar *   dev_name =  PNULL;
    uint16          dev_name_len = 0;
	MMIFILE_DEVICE_E device_type = MMIAPIFMM_GetFirstValidDevice();
    
    //SCI_TRACE_LOW:"Ajava_Initiliztion_Special b_initializeJAVA = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JWE_175_112_2_18_2_23_4_347,(uint8*)"d",b_initializeJAVA);
    if(!b_initializeJAVA&&MMK_IsOpenWin(MAIN_IDLE_WIN_ID))
    {        
        
        if (g_is_delete)
        { 
            if (device_type >= MMI_DEVICE_NUM)
            {
                dev_name = MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
                dev_name_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK);
            }
            SCI_MEMSET(full_path,0,((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar)));
            
            if(MMIAPIFMM_CombineFullPath(
             dev_name,
             dev_name_len,
             JAVA_DIR,
             JAVA_DIR_LEN,
             JAVA_DEFAULT_LIST_FILE,
             JAVA_DEFAULT_LIST_FILE_LEN,
             full_path,
             &full_path_len))
            {          
         
                //SCI_TRACE_LOW:"IsDefaultJavaFolder delete suite.idx"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,JWE_199_112_2_18_2_23_4_348,(uint8*)"");
                ajms_ret = MMIAPIFMM_DeleteFile(full_path,PNULL);
                //SCI_TRACE_LOW:"Ajava_Initiliztion_Special ajms_ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,JWE_201_112_2_18_2_23_4_349,(uint8*)"d",ajms_ret);
            }
        }
        LogFilter = (AJMS_DEBUG_FilterCallback)jblendia_logFilter;
        ajms_ret = ajms_init(LogFilter);
        MMIJAVA_initPreinstallID();
        JWE_LOG(Ajava_Initiliztion_Special, ("Ajava_Initiliztion_ia -- ajms_ret = %d", ajms_ret));        
        b_initializeJAVA = TRUE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : MMIJAVA_IsInitialize
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_IsInitialize(void)
{
    return b_initializeJAVA;
}

/*****************************************************************************/
//  Description : MMIJAVA_SetIsDelJava
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsDelJava(BOOLEAN is_deljava)
{
    g_is_delete = is_deljava;
}

/*****************************************************************************/
//  Description : Ajava_Finalize_ia
//  Global resource dependence : none
//  Author: huangjc
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Ajava_Finalize_Special(void)
{
   // uint32 status = 0;
    
    if( b_initializeJAVA)
    {     

        ajms_fini();
        JWE_LOG(Ajava_Finalize_Special, ("Ajava_Finalize_ia, Leave!"));
        b_initializeJAVA = FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : Ajava_Initiliztion_UDP
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
BOOLEAN Ajava_Initiliztion_UDP(void)
{
    if(!b_initializeJAVAUdp)
    {        
        
#ifndef WIN32
        UDPResourceInitialize();
#endif
        b_initializeJAVAUdp = TRUE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : Ajava_Finalize_UDP
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
BOOLEAN Ajava_Finalize_UDP(void)
{
	if(b_initializeJAVAUdp)
	{
#ifndef WIN32       
		UDPResourceFinalize();   
#ifdef PCLINK_SUPPORT
		if(TCPIPPCLINK_IsPclinkEnable())/*lint !e718 !e746*/
		{
			TCPIPPCLINK_ResetNetInterface();/*lint !e718 !e18*/
		}
		else
#endif
		{ 
			//MMIAPIJAVA_PDPDeactive();
             MMIAPIJAVA_SendSignal(MSG_JAVA_DEACTIVE_PDP,PNULL);		
		}
#endif    
		b_initializeJAVAUdp = FALSE;
	}
	return TRUE;
}

/*****************************************************************************/
//  Description : Ajava_Initiliztion_COM
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
BOOLEAN Ajava_Initiliztion_COM(void)
{
    uint32 status = 0;
    
    if(!b_initializeJAVACom )
    {
        
        g_AComId = SCI_CreateThread(AJAVACOMM_TASK_NAME, AJAVACOMM_TASK_QNAME, AJAVA_COMMain, 0, NULL, AJAVACOMM_TASK_STACK_SIZE, 
            AJAVACOMM_TASK_QNUM, AJAVACOMM_TASK_PRIORITY, AJAVACOMM_TASK_PREEMPTIVE, AJAVACOMM_TASK_START_FLAG);
        //SCI_ASSERT(SCI_INVALID_BLOCK_ID != g_AComId);
        if(SCI_INVALID_BLOCK_ID == g_AComId)
        {
            JWE_LOG(Ajava_Initiliztion_COM, ("create com thread failed"));
        }
        
        JWE_LOG(Ajava_Initiliztion_COM, ("create com thread %08x", g_AComId));
        
        status = SCI_ResumeThread(g_AComId);
        JWE_LOG(Ajava_Initiliztion_COM, ("Resume com thread, %d", status));
        
        b_initializeJAVACom = TRUE;
    }
    return TRUE;
}
/*****************************************************************************/
//  Description : Ajava_Finalize_COM
//  Global resource dependence : none
//   Author: huangjc
//  Note:
/*****************************************************************************/
BOOLEAN Ajava_Finalize_COM(void)
{
    uint32 status = 0;
    
    if(b_initializeJAVACom)
    {
        
        status = SCI_TerminateThread(g_AComId);
        JWE_LOG(Ajava_Finalize_COM, ("SCI_TerminateThread com %d", status));
        
        status = SCI_DeleteThread(g_AComId);
        JWE_LOG(Ajava_Finalize_COM, ("SCI_DeleteThread com %d", status));
        
        g_AComId = SCI_INVALID_BLOCK_ID;
        
        JWE_LOG(Ajava_Finalize_COM, ("Ajava_Finalize_COM, Leave!"));
        b_initializeJAVACom = FALSE;
    }
    return TRUE;
}

void* JkGenericConnectionGetSchemeInfo(void){
	return 0;
}
#endif

