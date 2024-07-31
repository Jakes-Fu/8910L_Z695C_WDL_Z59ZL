/***********************************************************************
**Author:    huangjc
**Date:      2007-4-18
**CopyRight:AMOI 2007
**Description:This file define the function of java nv
***********************************************************************/
/***********************************************************************
**                                                Edit History
**      Date                    Name               Description
***2007-4-18              huangjc                create
***********************************************************************/
#ifndef  _MMIJAVA_EXPORT_H_    
#define  _MMIJAVA_EXPORT_H_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef JAVA_SUPPORT_IA
#include "sci_types.h" 
#include "mmk_type.h"
#include "guiedit.h"//#include "guieditbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmifmm_export.h"
#include "java_mmi_text.h"
#include "ajms_ota.h"
#include "jblend_platform.h"
#include "ajms_arc.h"
#include "ajms_ab.h"
#include "dal_keypad.h"
#include "kjava_deprecated.h"
#include "jwe_log.h"
#include "ajms_arc.h"
#include "jktypeskeycode.h"
#include "amkeycode.h"
#include "ajms_pcs.h"
#include "ajutil.h"
#include "ajsc_platform.h"
#include "arm_reg.h"
#include "ajms_cfg_eime.h"
#include "jblend_config.h"
#include "java_menutable.h"
#include "java_id.h"

//#include "mmibrowser.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

 /**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef IA_JAVA_SUPPORT_LOW_MEMORY
#define     MMIJAVA_MAX_SIZE               ((1024+500)*1024)
#define     MMIJAVA_INSTALL_MAX_SIZE       ((1024+500)*1024)
#else
    #ifdef NANDBOOT_SUPPORT
        #define     MMIJAVA_MAX_SIZE               (3*1024*1024)
        #define     MMIJAVA_INSTALL_MAX_SIZE       (3*1024*1024)
    #else
        #define     MMIJAVA_MAX_SIZE               ((2*1024+500)*1024)//按照讨论的结果,8M SRAM的情况下，java HEAP 缩小到2.5M
        #define     MMIJAVA_INSTALL_MAX_SIZE       ((2*1024+500)*1024)
    #endif
#endif

#define     SPREADTRUM_LCD_SOFTKEY_WIDTH 			MMI_MAINSCREEN_WIDTH
#define     SPREADTRUM_LCD_SOFTKEY_HEIGHT 			25//MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT

#define     MMIJAVA_WIN_POINT_LEFT              0
#define     MMIJAVA_WIN_POINT_TOP               MMI_CLIENT_RECT_TOP

#define     MMIJAVA_CLIENT_LEFT                 (0)
#define		MMIJAVA_CLIENT_TOP					(MMI_TITLE_HEIGHT)
#define     MMIJAVA_CLIENT_RIGHT                (MMI_MAINSCREEN_RIGHT_MAX_PIXEL)
#define		MMIJAVA_CLIENT_BOTTOM				(MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)//139



#define     MMIJAVA_NAME_MAX_LEN	        AJMS_MAX_ENTRY_NAME_LENGTH
#define     MMIJAVA_DEFAULT_SUFFIX_LEN      8
#define     MMIJAVA_JAR_SUFFIX	            L".jar"
#define     MMIJAVA_JAD_SUFFIX	            L".jad"
//#define     MMIJAVA_DM_SUFFIX	            L".dm"
//#define     MMIJAVA_DCF_SUFFIX	            L".dcf"

#define     JAVA_DIR                        L"@java"
#define     JAVA_DIR_LEN                    5
#define     JAVA_DEFAULT_LIST_FILE          L"suite.idx"
#define     JAVA_DEFAULT_LIST_FILE_LEN      9

#define     MMIJAVA_MAX_DOWNLOAD_URL_NAME_LEN   16//huangjc add at 2007.3.12
#define     MMIJAVA_MAX_DOWNLOAD_URL_LEN        512//huangjc add at 2007.3.12

#define     MMIJAVA_MAX_DOWNLOAD_NUM        5

#define     MMIJAVA_VERSTR_MAX_LEN          12
#define     MMIJAVA_SIZESTR_MAX_LEN         12

//#define     MMIJAVA_JAVA_MAIN_DIR_UCS2		 JAVA_DIR
//#define     MMIJAVA_JAVA_MAIN_DIR_BYTE_LEN       JAVA_DIR_LEN

#define     MAXJAVALISTSIZE                 20
#define     MMIJAVA_PERMIT_MAX_NUM          (8)
#define     MMIJAVA_PERMIT_OPT_MAX_NUM      (4)

#define     MMISMS_JAVA_ADDR_MAX_LEN    32
#define     MMISMS_JAVACBF_MAX_NUM       10
#define     MMISMS_JAVA_MAX_UCS2_BYTE_LEN        134
#define     MMISMS_JAVA_MAX_UCS2_SPLIT_LEN        128

#define     MMISMS_JAVA_MAX_DEF_BYTE_LEN        154
#define     MMISMS_JAVA_MAX_DEF_SPLIT_LEN        147

#define     MMISMS_JAVA_MAX_8BIT_BYTE_LEN        134
#define     MMISMS_JAVA_MAX_8BIT_SPLIT_LEN        128
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef struct _JAVA_APP_SIGNAL {
    SIGNAL_VARS
    uint32    	    content;			// message content
} JAVA_APP_SIGNAL;

typedef struct
{
    uint32              file_len;  //current lyric file length
    uint8*              file_content_ptr; //buffer allocated
    SFS_HANDLE         file_handler;
    uint32              file_pos;  //position in file_content_ptr buffer
} MMIJAVA_JAR_FILE_INFO_T;

typedef struct
{
     uint32             file_len;
     uint8*             file_content_ptr;
     SFS_HANDLE         file_handler;
     uint32             file_pos;
}MMIJAVA_JAD_FILE_INFO_T;

typedef struct
{
	uint16			    name[MMIJAVA_NAME_MAX_LEN];	
	uint16				name_len;		    		
	uint16				store_pos;					//存储位置　0=> udisk, 1=> sdcard
    uint32				time;						//创建时间
    uint32			    memory;					    //所占内存
} MMIJAVA_DETAIL_INFO;

typedef struct
{
    uint32          jad_len; 
    uint8          *jad_ptr;
    SFS_HANDLE      jad_file_handler;
    uint32          jar_len; 
    uint8          *jar_ptr;
    SFS_HANDLE      jar_file_handler;
    uint8           *url;
    uint8           *jar_file_path;
    int32           old_id;
    BOOLEAN         is_ota; //对齐
} MMIJAVA_INSTALL_DATA_INFO_T;


typedef struct
{
    uint8	total;                      /*list total items*/
    MMI_TEXT_ID_T   item_text_id[MMIJAVA_PERMIT_OPT_MAX_NUM];/*list item's text id*/
    int32     mode[MMIJAVA_PERMIT_OPT_MAX_NUM];/*list item according to mode*/
    int32     cur_mode;                /*cur permit's mode*/
}MMIJAVA_PERMIT_OPT_INFO;


typedef struct _SMS_Message 
{
    // text message ?/ binary message ?
    int32   type; 
    /**
     * A URL indicating a message-related address.
     * <ul>
     *  <li>If a send message, the destination address.
     *  <li>If a receive message, the sender's address.
     *    If the receive message does not contain the senders's address, 
JK_NULL.
     * </ul>
     */
    char*   url;

    /**
     * The size in octets of the URL string pointed to by the <i>url</i> 
member.
     * <ul>
     *  <li>If a send message, the length of the destination address.
     *  <li>If a receive message, the length of the sender's address.
     *     If the receive message does not contain the senders's address, 0.
     * </ul>
     */
    int32 urlLength;

    /**
     * A flag indicating whether the time of sending is set in a receive 
message.
     * <ul>
     *  <li>If a send message, JK_FALSE.
     *  <li>If a receive message,
     *  then JK_TRUE if the time of sending is set in timeStampHigh and
     *  timeStampLow, else JK_FALSE.
     * </ul>
     */
    BOOLEAN isTimeStampAvailable;

    /**
     * The high 32 bits of the time stamp (time of sending, GMT, in 
milliseconds) set
     * in a receive message.
     *
     * This member is valid if the isTimeStampAvailable member is JK_TRUE.
     */
    int32 timeStampHigh;

    /**
     * The low 32 bits of the time stamp (time of sending, GMT, in 
milliseconds) set
     * in a receive message.
     *
     * This member is valid if the isTimeStampAvailable member is JK_TRUE.
     */
    int32 timeStampLow;

    /**
     * Length (in octets) of payload data in a binary message.
     *
     * This member is valid if the type member is #JK_WMA_MESSAGE_TYPE_BINARY.
     */
    int32 binaryDataLength;

    /**
     * A pointer to the memory location containing payload data in the case 
of a
     * binary message.
     *
     * This member is valid if the type member is #JK_WMA_MESSAGE_TYPE_BINARY.
     */
    char*   binaryData;

    /**
     * The number of characters of payload data in the case of a text message.
     *
     * This member is valid if the type member is #JK_WMA_MESSAGE_TYPE_TEXT.
     */
    int32 textDataLength;

    /**
     * A pointer to the memory location containing payload data in the case 
of a
     * text message.
     *
     * This member is valid if the type member is #JK_WMA_MESSAGE_TYPE_TEXT.
     */
    char*   textData;

     /**
     * 目的端口
     * huangjc add at 2007-5-30 for IaSolution 
     * 发送短信时，调用MMISMS_RegCallback会返回注册的端口号给该值
     */
     int32 dstPort;

     /**
     * 源端口
     * huangjc add at 2007-5-30 for IaSolution 
     * 接收短信时，调用MMISMS_RegCallback会返回注册的端口号给该值
     */
     int32 srcPort;

} SMS_Message;


typedef void (* SMS_CALLBACK) (int32  event, int/*SMS_Message **/argv);

typedef struct _JAVA_SMS_RegisterInfo 
{
	int32 port;
	SMS_CALLBACK callback;
}JAVA_SMS_RegisterInfo;

typedef struct 
{
    uint32 curmidletid;
    int32  cursuitid;
    //wchar  jar_filename[MMIFILE_FILE_NAME_MAX_LEN+1];
    wchar*   jar_filename_ptr;
    //wchar  jad_filename[MMIFILE_FILE_NAME_MAX_LEN+1];
    wchar*   jad_filename_ptr;
    uint16 is_flag;
}JAVA_INSTALL_INFO_T; 

typedef struct
{
    GUIEDIT_TYPE_E      type;           //类型
    wchar                   wstr[MMIFILE_FILE_NAME_MAX_LEN+1];       //字符串
    uint16     			    wstr_len;       //字符个数
    uint16                  max_str_len;    //最大长度
    uint16            max_ucs2_len;      // 非ascii字符的最大个数
    uint16            max_default_len;   // ascii字符的最大个数
    uint8             allow_inputmethod; //允许的输入法类型
    uint32            cur_inputmethod;   //当前的输入法类型
    wchar            title[MMIFILE_FILE_NAME_MAX_LEN+1];
    uint16            title_len;
}JAVA_EDITBOX_INIT_DATA_T;

typedef struct
{
    SIGNAL_VARS
	AJMS_EimeData *data_ptr;
} MMIJAVA_TEXTSIG_T;

typedef struct  
{
    BOOLEAN is_start_wap;
    uint8 *url;
}MMIJAVA_WAP_INFO;

typedef struct 
{
    //KJavaFileHandle    hFile;  
	char               fileName[MMIFILE_FILE_NAME_MAX_LEN];//but
    int 			   mode;
    SFS_HANDLE		   sfs_handle;
    BOOLEAN			   is_used;
} Kjava_ShareFileHandlerInfo;

typedef struct tagMMISMS_JAVA_T
{
    // ------------------- FOR IASOLUTION ---------------------
    int32   type;     
    BOOLEAN isTimeStampAvailable;    
    int32 timeStampHigh;    
    int32 timeStampLow;   
    int32 binaryDataLength;   
    char*   binaryData;
    int32 textDataLength;
    char*   textData;
    // address info, marked with '+' if international number
    uint8                       addr_buf[MMISMS_JAVA_ADDR_MAX_LEN];
    uint16                      addr_len;
   
    // port
    int32                       src_port;
    int32                       dst_port;

    // message type flag. equal to 1 if CBS message, 0 if SMS.
    int32                       is_cbs;         // not used in MT

    // ------------------- FOR LENOVO ---------------------
    BOOLEAN                     is_long;
    uint16                      ref_num;
    uint8                       total_num;
    uint8                       cur_num;

    struct tagMMISMS_JAVA_T*    previous_ptr;
    struct tagMMISMS_JAVA_T*    next_ptr;
}MMISMS_JAVA_DATA_T;

typedef struct
{
    SMS_CALLBACK  jsCBF;
    int32       port;
}MMISMS_JAVA_CBF_T;

typedef struct
{
    MMISMS_JAVA_CBF_T   java_cbf[MMISMS_JAVACBF_MAX_NUM];

    uint16              java_data_count;
    MMISMS_JAVA_DATA_T* java_data_root;

    BOOLEAN             is_sending_java;
    int32               src_port;
    int32               des_port;
}MMISMS_JAVA_GLOBAL_T;

typedef enum
{
    JAVA_SENDOK_CNF,
    JAVA_SENDFAIL_CNF,
    JAVA_NEW_MT_IND
}MMISMS_JAVA_EVENT_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//***************************************************************************************
//	Description:JAVA应用以普通短信发送短信接口
//	Author:huangjc
//	Date:2007-07-09
//	Note:当JAVA应用发送短信的目标地址为移动手机或者是格锐数码游
//		戏服务器时候调用
//***************************************************************************************
#ifndef MMI_MULTI_SIM_SYS_SINGLE
PUBLIC void MMIAPIJAVA_AppSendCommonSMS(
											MN_DUAL_SYS_E       		dual_sys,	
											uint8	*dest_addr_ptr,
											uint8	dest_addr_len,
											MMI_STRING_T	*message_content_ptr,
											int dstPort,//目的端口
											int srcPort//源端口
											);

#else
PUBLIC void MMIAPIJAVA_AppSendCommonSMS(
											uint8	*dest_addr_ptr,
											uint8	dest_addr_len,
											MMI_STRING_T	*message_content_ptr,
											int dstPort,//目的端口
											int srcPort//源端口
											);
#endif

/*****************************************************************************/
//  Description :MMIAPIJAVA_OpenMainMenu
//  Global resource dependence : none
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenMainMenu(void);
#if 0 //by murphy for API整理 
/******************* *******************************************************************************/
// Description:HandleWapCheckResult
// Date:2007-06-08
// Author:huangjc
// Note:OTA安装解析结果处理函数
//*************************************************************************************************/
PUBLIC void HandleWapCheckResult(
                                 AJMS_CheckResult *check_result_ptr
                                 );
#endif
/**************************************************************************************************/
// Description:MMIJAVA_Init
// Date:2007-06-08
// Author:huangjc
// Note:JAVA 模块初始化(NV与及其它相关数据结构)
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_Init(void);
/****************************************************************************
*函数定义:
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
PUBLIC void MMIAPIJAVA_InitModule(void);
/****************************************************************************
*函数定义:MMIAPIJAVA_JBlendiaAjmsInit
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
PUBLIC void MMIAPIJAVA_JBlendiaAjmsInit( void );
/****************************************************************************
*函数定义:MMIAPIJAVA_JBlendiaAjmsFinalize
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
PUBLIC void MMIAPIJAVA_JBlendiaAjmsFinalize( void );

/**
 * \brief starts the specified application with given suite name and vendor name.
 * @param suiteName
 *        A pointer to the UTF-8 string indicating the application suite name.
 * @param vendorName
 *        A pointer to the UTF-8 string indicating the application vendor name.
 * @return 
 *        - A result value: 
 *        - #0 non error
 *        - #-1 has error
 *        .
 *Author:hanker.zhou
 *Date:2010-6-30
 * The function is used to start the midlet by suitename and vendorname
 * application. According to the MIDP2.0 spec, the vendor name plus suite name
 * should be unique for each different midlet suite.
 */
int MMIAPIJAVA_StartMIDletByNameVendor( const char* suiteName, 
                                       const char* vendorName );

/**************************************************************************************************/
// Description:MMIAPIJAVA_StartLocalInstall
// Date:2007-06-08
// Author:huangjc
// Note:本地安装入口函数，供"资源管理器"调用
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_StartLocalInstall(const wchar *full_name,uint16 full_name_len, uint16 index);
//*********************************************************************************
//Description:JavaGameDirectStart
//Author:huangjc
//Date:2007-5-17
//Note:for java direct start
//        modify at 2007.7.1 for direct start bug
//在U盘上直接运行JAVA应用，而且应用退出时，会把安装记录删除掉
//*********************************************************************************
PUBLIC void MMIAPIJAVA_JavaGameDirectStart(const wchar *full_name,uint16 full_name_len, uint16 index);


/*****************************************************************************/
// 	Description : open text edit win
//	Global resource dependence : none
//  Author: applezhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenTextEditWindow(AJMS_EimeData * data);


/*****************************************************************************/
//  Description :exit java
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_ExitJAVA(void);
#ifdef JAVA_OTA_SUPPORT

/*****************************************************************************/
//  Description : open waiting window
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_OpenWapWin(void);


/*****************************************************************************/
//  Description : MMIAPIJAVA_StopOTAInstall
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOTAInstall(void);

#ifdef DRM_SUPPORT
PUBLIC void MMIAPIJAVA_OTAInstallDrm(wchar *fileName, int8 *url);
#endif
#endif

/*****************************************************************************/
//  Description:   to resume jbed installing processing when after mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Arthur.Peng 2006/11
//  Param
//       module: [in] module tag to resume java
//  Return:
//       TRUE : resuming complete.
//       FALSE: don't need to resume or has been resumed.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Resume(MMIJAVA_PAUSE_TYPE_E module);
/*****************************************************************************/
//  Description:   to pause jbed installing processing when mt call ,alarm 
//                 incoming or other system high interrupt event.
//  Author     :   Arthur.Peng 2006/11
//  Param
//       module: [in] module tag to pause java
//  Return:
//       TRUE : paused normally.
//       FALSE: don't need pause or has been paused.
//  Note: if the func is invoked to pause java, resume func must be invoked 
//        after high interuppt event.
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Pause(MMIJAVA_PAUSE_TYPE_E module);


/*****************************************************************************/
//  Description:   to resume jbed installing processing when after mt call ,alarm 
//                 incoming or other system high interrupt event.
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_ResumeBackgroundJVM( void );
/*****************************************************************************/
//  Description:   to pause jbed installing processing when mt call ,alarm 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PauseBackgroundJVM( void );


/****************************************************************************
*函数定义:MMIJAVA_HandleOTANotifyTimer
*函数说明:
*参数说明:
*返回值:
*huangjc add at 2007.4.20 for java module init
*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_HandleOTANotifyTimer(void * param);
/*****************************************************************************/
// 	Description : IsFocusJAVARunWin
//	Global resource dependence : 
//  Author:aoke.hu
//	Note:2007.8.15
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsFocusJAVARunWin(void);
/*****************************************************************************/
//  Description : java task send msg to app task
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SendSignal(uint32 sig_id, uint32 param);
/*****************************************************************************/
//  Description : auto run app 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_AutoRunApp(uint32 param);
//******************************************************************************
//  Description:  kjava_sock_link_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartLinkTimer(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_conn_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartConnectTimer(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_recv_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartRecvTimer(int32 slot);
//******************************************************************************
//  Description:  MMIAPIJAVA_StartSendTimer
//  Author:murphyxie
//  Date:2010-09-13
//  Note: start send polling
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartSendTimer(int32 slot);
//******************************************************************************
//  Description:  MMIAPIJAVA_StopSockTimer
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StopSockTimer(int32 slot);
//******************************************************************************
//  Description:  kjava_sock_link_polling
//  Author:huangjc
//  Date:2007-09-03
//  Note: 
//*********************************************************************************
PUBLIC void MMIAPIJAVA_StartUDPLink(int32 id);
/*****************************************************************************/
//  Description : MMIAPIJAVA_UDP_Timer_Process.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_UDP_Timer_Process(uint8 timer_id, uint32 param);
//  Description : MMIAPIJAVA_MediaStopTimer_Process
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_MediaStopTimer_Process(uint8 timerID, uint32 param);

//***********************************************************************************************
//  Description:jblendia_gfx_timer
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
PUBLIC void MMIAPIJAVA_CreateWinPaintTimer(void);

/*****************************************************************************/
//  Description : Initialization of connection resource control.
//  Global resource dependence :
//  Author: murphy.xie
//  Note: receive sms timer polling: g_java_sms_timer_id;
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_WMA_Timer_Process(int32 timerID);


/***************************************************************************/
// Description:free MMIAPIJAVA_socket_polling_timeOut_process
// Author:huangjc
// Note:
//*************************************************************************/
PUBLIC int32  MMIAPIJAVA_socket_polling_timeOut_process(int32 timerID);

/***************************************************************************/
// Description:free MMIAPIJAVA_Exit_FinalizeResource
// Author:huangjc
// Note:
//*************************************************************************/
PUBLIC void MMIAPIJAVA_Exit_FinalizeResource(void);

/***************************************************************************/
// Description:free MMIAPIJAVA_StartMediaStopTimer
// Author:huangjc
// Note: api for stop media timer
//*************************************************************************/
PUBLIC void MMIAPIJAVA_StartMediaStopTimer(int32 player_id);
#ifdef SELECT_INSTALL_DISK_SUPPORT

/**************************************************************************************************/
// Description:Set sys Disk C to on
// Date:2010-07-13
// Author:hanker
// Note:
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_SetSysDiskHiddenOn(void);
/**************************************************************************************************/
// Description:Set sys Disk C to on
// Date:2010-07-13
// Author:hanker
// Note:
//*************************************************************************************************/
PUBLIC void MMIAPIJAVA_SetSysDiskHiddenOff(void);
#endif

/*****************************************************************************/
//  Description : MMIAPIJAVA_Finalize_Special
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_Finalize_Special(void);

/*****************************************************************************/
//  Description:  stop current operation when start udisk 
//  Author     :  murphy.xie
//  Param :     void
//  Return:     void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOperaStartUDisk(void);

/*****************************************************************************/
//  Description:  stop current operation when sd plug
//  Author     :  murphy.xie
//  Param :     void
//  Return:     void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StopOperaSDPlug(void);
/*****************************************************************************/
//  Description : to install form file system
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystem(wchar *fullpathname,int fullpathname_len);
/*****************************************************************************/
// 	Description : MMIAPIJAVA_InstallFromFilesystemForDM
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:install function for dm
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_InstallFromFilesystemForDM(
                                             wchar *pathname, 
                                             int pathname_len, 
                                             wchar *filename, 
                                             int filename_len
                                             );

/*****************************************************************************/
// Description : MMIAPIJAVA_CheckIfJavaPort
// Global resource dependence : 
// Author:kelly.li
// Note:check if  the register java port
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_CheckIfJavaPort(int32 port, SMS_CALLBACK* javasms_func_ptr, BOOLEAN callback_flag);

/*****************************************************************************/
// Description : MMIAPIJAVA_GetSmsJavaData
// Global resource dependence : 
// Author:murphy.xie
// Note:get the local g_mmisms_javadata
/*****************************************************************************/
PUBLIC MMISMS_JAVA_GLOBAL_T* MMIAPIJAVA_GetSmsJavaData(void);

/*****************************************************************************/
// 	Description : MMIAPIJAVA_IsJavaRuning
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:jvm if running
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsJavaRuning( void );
/*****************************************************************************/
// 	Description : MMIAPIJAVA_IsJavaBackgroundRuning
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:jvm if running
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IsJavaBackgroundRuning( void );

/*****************************************************************************/
// 	Description : MMIAPIJAVA_StartCheckVmState
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:checke jvm state 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_StartCheckVmState(int32 msgVMState,DPARAM param);

/*****************************************************************************/
// 	Description : MMIAPIJAVA_CheckVmStateTimer_Process
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:check jvm state time out function 
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_CheckVmStateTimer_Process(uint8 timerID, uint32 param);

//***********************************************************************************************

/*****************************************************************************/
// 	Description : MMIAPIJAVA_SetJavaAutoTckOn
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:set tck flag on
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SetJavaAutoTckOn(void);
/*****************************************************************************/
// 	Description : MMIAPIJAVA_SetJavaAutoTckOff
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:set tck flag off
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_SetJavaAutoTckOff(void);

/*****************************************************************************/
// 	Description : MMIAPIJAVA_GetJavaAutoTckState
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:set tck flag off
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_GetJavaAutoTckState(void);

/*****************************************************************************/
// 	Description : MMIAPIJAVA_jwe_vmErrorHandle
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:show error pub win
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_jwe_vmErrorHandle(int32 eventType);
/*****************************************************************************/
// 	Description : MMIAPIJAVA_PDPDeactive
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:do deactive pdp 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_PDPDeactive(void);
/*****************************************************************************/
//  Description : MMIAPIJAVA_GetVersion
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC const  char* MMIAPIJAVA_GetVersion( void );

/*****************************************************************************/
//  Description :MMIAPIJAVA_AudioClientPlayAgain
//  Global resource dependence :
//  Author:
//  Note: to implement loop play
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_AudioClientPlayAgain(void);
/*****************************************************************************/
// 	Description : MMIAPIJAVA_PlatformRequest
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_PlatformRequest(uint32 type);
/*****************************************************************************/
//  Description : MMIAPIJAVA_StopAudio
//  Global resource dependence :stop java audio and also stop jvm
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_StopAudioAndJVM(void);

/*****************************************************************************/
//  Description :MMIAPIJAVA_stopVM_sync
//  Global resource dependence : none
//  Author: 
//  Note: make jvm to background state
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_stopVM_sync(void);

/*****************************************************************************/
//  Description : MMIAPIJAVA_IfUseAudioDevice
//  Global resource dependence :MMIAPIJAVA_IfUseAudioDevice
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIJAVA_IfUseAudioDevice(void);

/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef MMI_MULTI_SIM_SYS_SINGLE
#define MMIJAVA_AppSendCommonSMS				        MMIAPIJAVA_AppSendCommonSMS
#endif
#define MMIJAVA_StartMIDletByNameVendor				MMIAPIJAVA_StartMIDletByNameVendor
#define MMIJAVA_OpenTextEditWindow				    MMIAPIJAVA_OpenTextEditWindow
#define MMIJAVA_OpenWapWin				            MMIAPIJAVA_OpenWapWin
#ifdef JAVA_OTA_SUPPORT
#define MMIJAVA_StopOTAInstall				        MMIAPIJAVA_StopOTAInstall
#ifdef DRM_SUPPORT
#define MMIJAVA_OTAInstallDrm				            MMIAPIJAVA_OTAInstallDrm
#endif
#endif
#define MMIJAVA_SetJavaAutoTckOn				        MMIAPIJAVA_SetJavaAutoTckOn
#define MMIJAVA_SetJavaAutoTckOff				    MMIAPIJAVA_SetJavaAutoTckOff
#define MMIJAVA_GetJavaAutoTckState				    MMIAPIJAVA_GetJavaAutoTckState
#ifdef JAVA_EVENT_ERROR_PROMPT_WIN
#define MMIJAVA_jwe_vmErrorHandle				    MMIAPIJAVA_jwe_vmErrorHandle
#endif
#define MMIJAVA_PDPDeactive				            MMIAPIJAVA_PDPDeactive
#define MMIJAVA_AudioClientPlayAgain				    MMIAPIJAVA_AudioClientPlayAgain
#define MMIJAVA_stopVM_sync				            MMIAPIJAVA_stopVM_sync

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif

