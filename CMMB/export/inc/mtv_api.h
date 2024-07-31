/******************************************************************************
 ** File Name:      mtv_api.h                                                 *
 ** Author:         Spreadtrum                                                *
 ** DATE:           09/29/2008                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    data structures and functions definitions of mtv api      *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/29/2008     Geng.Ren         Create                                    *
 ******************************************************************************/
  
#ifndef _MTV_API_H
#define _MTV_API_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "kd.h"
#include "mtv_common.h"
#include "dal_player.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
// network name length
#define MTV_STIMI_NET_NAME_LEN             256 
#define MTV_STIMI_CONT_SERVICE_MAX_NUM     64
#define MTV_STIMI_SHORT_SERVICE_MAX_NUM    64

#define MTV_STIMI_SERVICE_NAME_MAX_LEN       32

// max data length for hint
#define MTV_STIMI_HINT_MAX_DATA_LEN        256

// max record size for hint
#define MTV_STIMI_HINT_MAX_RECORD_NUM      2

// max language length for hint
#define MTV_STIMI_HINT_MAX_LANG_NUM        3
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum 
{
    LCDC_VIDEO_CAP_DATA_YUYV = 0,   //capture data is in YUYV yuv422 format
    LCDC_VIDEO_CAP_DATA_Y_UV,       //capture data is in Y and UV two plane, yuv422 format
    LCDC_VIDEO_CAP_DATA_Y_U_V,       //capture data is in Y, U, V three plane, yuv422 format
    MAX_LCDC_VIDEO_CAP_DATA_TYPE
}LCDC_VIDEO_CAP_DATA_TYPE_E; 

typedef enum
{
    MTV_MODE_TDMB,
    MTV_MODE_CMMB,
    MTV_MODE_NUM
} MTV_MODE_E;

typedef enum
{
    MTV_ENABLE_RECORD,
    MTV_DISABLE_RECORD
} MTV_ENABLE_RECORD_E;

typedef void (* MTV_CALLBACK) (
    KDuint32 ack_id, 
    KDuint32 para0,
    KDuint32 para1,
    KDuint32 para2
);

//return value
typedef enum
{
    MTV_STIMI_ERR_NONE =0x0,                // 0: success
    MTV_STIMI_CTLTAB_UPDATING,              // 1: updating control table 
    MTV_STIMI_GET_CTLTAB_FOR_ESG,           // 2: get control table for searching ESG
    MTV_STIMI_NO_SERVICE_MCI_SEARCH_CTRTAB, // 3: no service mci,to search control table
    MTV_STIMI_NO_SERVICE,                   // 4: no service
    MTV_STIMI_NO_MCI,                       // 5: no mci

    MTV_STIMI_UPDATING_NIT,         // 6: updating NIT
    MTV_STIMI_UPDATING_CMCT,        // 7: updating CMCT
    MTV_STIMI_UPDATING_CSCT,        // 8: updating CSCT
    MTV_STIMI_UPDATING_SMCT,        // 9: updating SMCT
    MTV_STIMI_UPDATING_SSCT,        // 10: updating SSCT
    MTV_STIMI_UPDATING_ESGDT,       // 11: updating NIT

    MTV_STIMI_NO_NIT,               // 12: no NIT
    MTV_STIMI_NO_CMCT,              // 13: no CMCT
    MTV_STIMI_NO_CSCT,              // 14: no CSCT
    MTV_STIMI_NO_SMCT,              // 15: no SMCT
    MTV_STIMI_NO_SSCT,              // 16: no SSCT
    MTV_STIMI_NO_ESGDT,             // 17: no ESGDT
    MTV_STIMI_NO_CTRLT,             // 18: no Control Table

    MTV_STIMI_FAIL_CREATE_NIT,      // 19: fail to create NIT file    
    MTV_STIMI_FAIL_CREATE_CMCT,     // 20: fail to create CMCT file
    MTV_STIMI_FAIL_CREATE_CSCT,     // 21: fail to create CSCT file
    MTV_STIMI_FAIL_CREATE_SMCT,     // 22: in order to facility coding,short time service multiplex configuration Tab 
    MTV_STIMI_FAIL_CREATE_SSCT,     // 23: in order to facility coding,short time service  configuration Tab 
    MTV_STIMI_FAIL_CREATE_ESGDT,    // 24: fail to create ESGDT file

    MTV_STIMI_FAIL_OPEN_NIT,        // 25: fail to open NIT file    
    MTV_STIMI_FAIL_OPEN_CMCT,       // 26: fail to open CMCT file
    MTV_STIMI_FAIL_OPEN_CSCT,       // 27: fail to open CSCT file
    MTV_STIMI_FAIL_OPEN_SMCT,       // 28: in order to facility coding,short time service multiplex configuration Tab 
    MTV_STIMI_FAIL_OPEN_SSCT,       // 29: in order to facility coding,short time service  configuration Tab 
    MTV_STIMI_FAIL_OPEN_ESGDT,      // 30: fail to open ESGDT file

    MTV_STIMI_FAIL_WRITE_NIT,       // 31: fail to write NIT file
    MTV_STIMI_FAIL_WRITE_CMCT,      // 32: fail to write CMCT file
    MTV_STIMI_FAIL_WRITE_CSCT,      // 33: fail to write CSCT file   
    MTV_STIMI_FAIL_WRITE_SMCT,      // 34: in order to facility coding,short time service multiplex configuration Tab 
    MTV_STIMI_FAIL_WRITE_SSCT,      // 35: in order to facility coding,short time service  configuration Tab 
    MTV_STIMI_FAIL_WRITE_ESGDT,     // 36: fail to write ESGDT file

    MTV_STIMI_NO_SERVICE_FRQ_NUM,   // 37: no service frequency number
    MTV_STIMI_NO_FRQ_NUM_FRQ,       // 38: no frequency number correspondence frequency
    MTV_STIMI_NO_MEMORY,            // 39: no memory
    MTV_STIMI_NO_SPACE,             // 40: no space
    MTV_STIMI_FILE_SYSTEM_ERR,      // 41: file system error 
    MTV_STIMI_ONLY_UPDATED_MCT,     // 42: only CMCT
    MTV_STIMI_UPDATED_CTRL_TAB,     // 43: NIT ,or CSCT, or ESGD  is updated at least
    MTV_STIMI_LACKOF_CONTROL_INFO,  // 44: lack of control info , need searching program. 
    MTV_STIMI_INVALID_CENTER_FRQ,   // 45: invalid center frequency 
    MTV_STIMI_SEARCHING_PRG,        // 46: is searching program 
    MTV_STIMI_SEARCHING_ESG,        // 47: is searching esd data 
    MTV_STATE_ERROR,                // 48: take action at wrong state 
    MTV_SG_ERROR_GETISMA,           // 49: fail to get isma from sg(mbbms only)
    MTV_TAKE_PICTURE_ERROR = 0x100  // larger than all output type refer to VIDEO_DISP_DATATYP_E in video_display.h
}MTV_STIMI_ERR_NUM_E;

typedef enum
{
    MTV_STIMI_LATIN_TABLE,   // default ASCII
    MTV_STIMI_GBT_15273_1,   // ASCII
    MTV_STIMI_GBT_15273_2,   // ASCII
    MTV_STIMI_GBT_15273_3,   // ASCII
    MTV_STIMI_GBT_15273_4,   // ASCII
    MTV_STIMI_GBT_15273_7,   // ASCII 
    MTV_STIMI_GB_13000_1,    // unicode
    MTV_STIMI_KSC_5601,      // KSC 5601
    MTV_STIMI_GB_2312,       // GB 2312
    MTV_STIMI_GB_18030,      // GB 18030
    MTV_STIMI_UTF8           // UTF8
    
}
MTV_STIMI_CODE_TABLE_E;

// ServiceClass
typedef enum
{
    MTV_STIMI_SERVICE_CLASEE_RESERVED,        // 0 — 保留
    MTV_STIMI_SERVICE_CLASEE_TV,              // 1 — 电视
    MTV_STIMI_SERVICE_CLASEE_AUDIO_BC,        // 2 — 音频广播
    MTV_STIMI_SERVICE_CLASEE_SHORT_TV,        // 3 — 短格式视频节目片断
    MTV_STIMI_SERVICE_CLASEE_FILE_DOWNLOAD,   // 4 — 文件下载
    MTV_STIMI_SERVICE_CLASEE_SOFTWARE_MANAGE, // 5 — 软件管理
    MTV_STIMI_SERVICE_CLASEE_ESG              // 6 — ESG
}
MTV_STIMI_SERVICE_CLASS_E;

// erviceGenre：
typedef enum
{
    MTV_STIMI_SERVICE_GENRE_TV = 0x01,        // 0x01, 数字电视业务
    MTV_STIMI_SERVICE_GENRE_AUDIO_BC = 0x02,  // 0x02, 数字音频广播业务
    MTV_STIMI_SERVICE_GENRE_TELETEXT = 0x03,  // 0x03, 图文电视业务
    MTV_STIMI_SERVICE_GENRE_FM_BC = 0x0a,     // 0x0a, 调频广播
    MTV_STIMI_SERVICE_GENRE_DATA_BC = 0x0c    // 0x0c, 数据广播业务
}
MTV_STIMI_SERVICE_GENRE_E;


/*SIGNAL CODE DEFINITION*/
typedef enum{
    MTV_STIMI_SEARCH_PROG_END_CNF = ((0x0A8 << 8)|1),
    MTV_STIMI_GET_NETWORK_IND,
    MTV_STIMI_GET_NEXT_NET_INFO,
    MTV_STIMI_SEARCH_PROG_ERR_IND,
    MTV_STIMI_COMPLETE_FREQUENCY_IND,
    MTV_STIMI_LOCK_FRQ_IND,
    MTV_STIMI_LOCK_EXPIRE_IND,
    MTV_STIMI_SEARCH_ESG_END_CNF ,
    MTV_STIMI_SEARCH_ESG_ERR_IND,
    MTV_STIMI_EMERGENCY_BROADCAST_IND ,  
    MTV_STIMI_PROGRAM_HINT_IND ,                     
    MTV_STIMI_GET_CA_KEY_IND,
    MTV_STIMI_STOP_SEARCH_ESG,
    MTV_STIMI_CTLTAB_UPDATE_IND,
    MTV_STIMI_RCV_CTLTAB_ERR_IND,
    MTV_STIMI_GET_NETWORK_TIME_IND,
    MTV_STIMI_SET_NETWORK_INFO_IND,
    MTV_STIMI_STOP_SEARCH_PROG_SIGNAL,
    MTV_STIMI_TERMINATE_IND,
    MTV_CONFIG_DATA_FILTER,
    MTV_STIMI_SEARCH_PROG_END_IND,    
//    MTV_INIT_FINISH,
//    MTV_CLEANUP_FINISH,
    MTV_PLAY_ACK,
    MTV_PLAY_ERROR,
    MTV_PLAY_ERROR_FOR_CA,
    MTV_PLAY_ERROR_FOR_NOAV,
    
    MTV_STOP_ACK,
    MTV_STOP_ERROR,

    MTV_PLAY_ERROR_IND,
    MTV_CA_NOT_GRANTED_IND,
    MTV_CA_CARD_NOT_PRESENT_IND,
    
    MTV_CA_GRANTED_IND,
    
    MTV_CA_IND,
    MTV_PLAY_AV_IND,
    MTV_PLAY_DRA_IND,
    MTV_PLAY_STREAM_END,
    MTV_PLAY_TIME_IND,
    MTV_RECORD_FILE_SAVED,
    MTV_RECORD_FILE_TOOSMALL,

    MTV_DISK_FULL,
    MTV_DISK_WRITE_ERROR,
    MTV_DISK_READ_ERROR,
    MTV_DTL_POST_ERROR,
    MTV_DMA_READ_ERROR,
    MTV_NEED_SEARCH_AGAIN,

    MTV_RFVT_RESET_MODE,
    MTV_DTL_ERROR,

    MTV_RSSI_CHANGED,
    MTV_BAD_SIGNAL,
    MTV_SIGNAL_RECOVERED,
    MTV_RELOCKING,
    MTV_CHANNEL_ERROR,

    MTV_CA_INFO_OK,
    MTV_CA_INFO_ERROR,

    MTV_SIM_INIT_CNF,    
    MTV_SIM_CLOSE_CNF,    
    MTV_SIM_REQUEST_CNF,
    MTV_CA_DATA_READY_IND,    
    MTV_ESG_DATA_READY_IND,
    MTV_ESG_START_PARSE_IND,
    MTV_ESG_PARSE_RESULT_IND,
    MTV_RECORD_MF_TOOLARGE,
    MTV_CA_GET_ISMA_ERROR_IND,

    MTV_SIGNAL_MAX

}MTV_MIDDLEWARE_SIGNALCODE_E;

typedef enum{
    MTV_STIMI_OPERATE_RESULT =0x0,
    MTV_STIMI_BROADCAST_MSG,
    MTV_STIMI_BROADCAST_PROMPT,
    MTV_STIMI_NETWORK_TIME,
    MTV_STIMI_SET_NETWORK_INFO
}MTV_STIMI_SIG_DATA_TYPE_E;


// network information
typedef struct mtv_stimi_network_info_tag
{
    KDuint16    network_id;        // network level + network id
    KDuint8     frequency_no;      // frequency number
    KDuint8     bandwidth;         // bandwidth                    
    KDuint32    center_frequency;  // center frequency
} MTV_STIMI_NETWORK_INFO_T;

// network information for MMI
typedef struct mtv_stimi_net_info_tag
{
    KDuint8                   network_name_len;  // network name length
    KDuint8                   pad1;
    KDuint16                  pad2;
    KDuint32                  code_table;        // coding table
    KDuint8                   network_name_arr[MTV_STIMI_NET_NAME_LEN];
    MTV_STIMI_NETWORK_INFO_T  network_info;
}MTV_STIMI_NET_INFO_T;

// network information table(NIT)
typedef struct mtv_stimi_net_info_table_tag
{
    KDuint16               network_number;    // network total 
    KDuint16               pad;
    MTV_STIMI_NET_INFO_T  *network_info_ptr;  // network information data
}MTV_STIMI_NET_INFO_TAB_T;

// service under appointed network
typedef struct mtv_stim_network_service_tab_tag
{
    KDuint16    network_id;                                  // network level + network id
    KDuint16    pad;
    KDuint32    service_number;                              // continual service total
    KDuint16    service_arr[MTV_STIMI_CONT_SERVICE_MAX_NUM]; // continual service data
    KDuint32    shortservice_number;                         // short service total
    KDuint16    shortservice_arr[MTV_STIMI_CONT_SERVICE_MAX_NUM]; // short service data
}MTV_STIMI_NETWORK_SERVICE_TAB_T;

typedef struct mtv_stimi_service_info_tag
{
    KDuint16 service_id;                          // service id
    KDuint8  ServiceClass;                        // service clase
    KDuint8  ServiceGenre;                       // service genre
    KDuint16 padding;                               // padding
    KDboolean    is_free;                           // for free
    KDuint8   service_name_len;              // service name length
    KDuint8  service_name_arr[MTV_STIMI_SERVICE_NAME_MAX_LEN];  // service name
}MTV_STIMI_SERVICE_INFO_T;

typedef struct mtv_stim_network_service_list_tag
{
   
    KDuint32   char_set;                             // code
    KDuint16   service_number;                       // continual service total
    KDuint16   shortservice_number;                  // short service total
    MTV_STIMI_SERVICE_INFO_T   service_arr[MTV_STIMI_CONT_SERVICE_MAX_NUM];                                // continual service data
    MTV_STIMI_SERVICE_INFO_T   shortservice_arr[MTV_STIMI_CONT_SERVICE_MAX_NUM];//short service data

}MTV_STIMI_NETWORK_SERVICE_LIST_T;

//network information for MMI
typedef  struct mtv_stimi_net_list_tag
{
    KDuint8   network_name_len;               // network name length
    KDuint8   pad1;
    KDuint16  pad2;
    KDuint32  code_table;                     // coding table
    KDuint8   network_name_arr[MTV_STIMI_NET_NAME_LEN];  // network information
    MTV_STIMI_NETWORK_INFO_T     network_info;
    MTV_STIMI_NETWORK_SERVICE_LIST_T service_list;
}MTV_STIMI_NET_LIST_T;

//network information table(NIT)
typedef struct mtv_stimi_net_info_list_tag
{
    KDuint16   network_number;                // network total 
    KDuint16   pad;
    MTV_STIMI_NET_LIST_T  *network_info_ptr;  //network information data
}MTV_STIMI_NET_INFO_LIST_T;

// auxiliary data in broadcast MSG
typedef struct mtvstimi_broadcast_aux_data_tag
{
    KDuint8     aux_data_type; //auxiliary data type
    KDuint8     pad;
    KDuint16    aux_data_len ; //auxiliary data length
    KDuint8*    data_ptr;      //auxiliary data
}MTVSTIMI_BROADCAST_AUX_DATA_T;

// network propmt in broadcast MSG. It prompts some broadcast msg under other network. 
typedef struct mtvstimi_broadcast_net_prompt_tag
{
    KDuint8 trigger_flag;//trigger flag
    KDuint8 type;//broadcast type
    KDuint8 level;//broadcast level
    KDuint8  bandwidth;//bandwidth
    KDuint16  frq_num;//frequency number
    KDuint16 net_id; // network level + network id
    KDuint32    center_frq;     // center frequency
}MTVSTIMI_BROADCAST_NET_PROMPT_T;

// language content in broadcast MSG
typedef struct mtvstimi_broadcast_language_content_tag
{
    KDuint32  language_type;//language type
    KDuint8  issue_organization_flag;//issue organization flag
    KDuint8  reference_service_flag;//reference service flag
    KDuint16 reference_service_id;	//reference service id 
    KDuint8  issue_organization_name_len;//issue organization name length
    KDuint8   padding;
    KDuint16 text_len;//text length    
    KDuint8    *text_ptr;                    // text data
    KDuint8    *issue_organization_name_ptr; // issue organization name
}MTVSTIMI_BROADCAST_LANGUAGE_CONTENT_T;

typedef struct 
{
    KDuint8   mday;  // day of the month - [1,31] 
    KDuint8   mon;   // months  - [1,12] 
    KDuint16  year;  // years [2000,2049] 
    KDuint8   wday;  // days since Sunday - [0,6], (Sunday : 0, Monday : 1)
}MTV_DATE_T;

typedef struct 
{
    KDuint8   sec;  // secondsafter the minute - [0,59] 
    KDuint8   min;  // minutesafter the hour - [0,59] 
    KDuint8   hour; // hours since midnight - [0,23] 
}MTV_TIME_T;

// broadcast msg
typedef struct mtvstimi_broadcast_msg_tag
{
    KDuint8  trigger_flag ;//trigger flag
    KDuint8  type;//broadcast type
    KDuint8  level;//broadcast level
    KDuint8  code_char_set;//code character set
    KDuint8  language_count;//language type total
    KDuint8  aux_data_acount;//auxiliary data total
    KDuint16  padding;
    MTV_DATE_T    date;              // date
    MTV_TIME_T    time;              // time
    KDuint32        duration;
    MTVSTIMI_BROADCAST_LANGUAGE_CONTENT_T    *lang_content_ptr; // language content
    MTVSTIMI_BROADCAST_AUX_DATA_T            *aux_data_ptr;     // auxiliary data
}MTVSTIMI_BROADCAST_MSG_T;

// broadcast information
typedef union  mtvstimi_broadcast_tag
{
    MTVSTIMI_BROADCAST_MSG_T           msg_content; // broadcast message
    MTVSTIMI_BROADCAST_NET_PROMPT_T    net_prompt;  // network prompt
}MTV_STIMI_BROADCAST_U;

// date and time
typedef struct esg_date_time_tag
{
    KDuint16    year: 12;            
    KDuint16    month:4;
    KDuint8     day;
    KDuint8     hour;
    KDuint8     minute;
    KDuint8     second;
    KDuint16    milisecond;
}ESG_DATE_TIME_T;

// program hint information
typedef struct esg_hint_info_tag
{
     ESG_DATE_TIME_T    date_time;                             // start time
     KDuint16           duration;                              // duration
     KDuint16           padding;
     KDuint8            lang[MTV_STIMI_HINT_MAX_LANG_NUM];     // language code
     KDuint8            hint_len;                              // hint length
     KDuint8            hint_arr[MTV_STIMI_HINT_MAX_DATA_LEN]; // hint
}ESG_HINT_INFO_T;

// program hint information
typedef struct esg_prog_hint_info_tag
{
     KDuint8           num;            // record number,  0，- current， 1 ， next;  
     KDuint8           code_char_set;  //  char set, 0 - GB2312, 1 - GB18030; 2 - BG13000.1 ; 3 - UTF-8
     KDuint16          padding;
     ESG_HINT_INFO_T   hint_info_arr[MTV_STIMI_HINT_MAX_RECORD_NUM];  // hint
}ESG_PROG_HINT_INFO_T;

// program hint language
typedef enum
{
     ESG_PROG_HINT_LANG_CHI,  // chinese
     ESG_PROG_HINT_LANG_ENG,  // english
     ESG_PROG_HINT_LANG_MAX
}ESG_PROG_HINT_LANG_E;

/* ESG相关定义 */
typedef enum
{
    MTV_ASCII,  // ascii编码
    MTV_UTF8,   // UTF8编码
    MTV_UCS2,   // UTF16编码
    MTV_ENCODE_UNKNOWN
}MTV_ENCODE_E;

typedef struct mtv_string_tag
{
    KDuint16        length;  // 字符串长度(字节)
    KDuint32        encode;  // 编码类型
    KDuint8        *str_ptr; // 字符串指针
}MTV_STRING_T;

typedef enum
{
    ESG_SUCCESS,        // 成功
    ESG_INVALID_PARAM,  // 无效的参数
    ESG_PARSE_PENDING,  // 解析未完成
    ESG_NO_DATA,        // 没有找到对应的数据
    ESG_PARSE_FAIL,     // 解析失败
    ESG_ERROR_UNKNOWN   // 未知错误
}ESG_ERROR_E;

typedef enum
{
    LANG_ZHO,    // 汉语
    LANG_ENG,    // 英语
    LANG_UNKNOWN // 未知
}LANGUAGE_E;

typedef struct timepoint_tag
{
    KDuint32    years;
    KDuint32    months;
    KDuint32    days;
    KDuint32    hours;
    KDuint32    minutes;
    KDuint32    seconds;
}TIMEPOINT_T;

typedef struct mmimtv_progdetail_tag
{
    MTV_STRING_T    Title;       // 标题
    MTV_STRING_T    Digestinfo;  // 内容摘要
    MTV_STRING_T    AudioLang;   // 内容伴音语言指示字符串
    MTV_STRING_T    SubtitleLang;// 字幕语言指示字符串
    KDuint32        lang;        // 语言种类(缺省为中文)
}MMIMTV_PROGDETAIL_T;

typedef struct mmimtv_programme_tag
{
    MTV_STRING_T           Title;     // 节目名称
    KDuint32               lang;      // 节目名称的语言种类(缺省为中文)
    TIMEPOINT_T            StartTime; // 节目开始时间
    KDuint16               Duration;  // 节目播放时间长度，单位为秒
    KDuint16               pad1;
    MMIMTV_PROGDETAIL_T    detail;    // 节目介绍
}MMIMTV_PROGRAMME_T;

typedef struct mmimtv_service_tag
{
    KDuint32              ServiceID;  // 业务标识符
    MTV_STRING_T          ServiceName;// 业务名称描述
    KDuint32              NumOfProg;  // 有效节目数量
    KDuint32              ProgTotal;  // 所有节目数量
    MMIMTV_PROGRAMME_T   *prog_ptr;   // 节目信息列表
}MMIMTV_SERVICE_T;

typedef struct mmimtv_esg_id_tag
{
    KDuint16    networkID;
    KDuint16    serviceID;
}MMIMTV_ESG_ID_T;

typedef struct mmimtv_esg_filter_tag
{
    KDuint16       ServiceID; // 业务标识符
    TIMEPOINT_T    time;      // 节目时间信息
}MMIMTV_ESG_FILTER_T;

typedef struct mmimtv_service_addinfo_tag
{
    KDuint8      ServiceClass; // 业务类型
    KDuint8      ServiceGenre; // 业务分类
    KDboolean    is_free;      // 是否免费节目
}MMIMTV_SERVICE_ADDINFO;

typedef struct mtv_channel_status_tag
{
    /* Additional channel status. */
    KDuint32     freqency;   /* current frequency */
    KDuint32     BER;         /* block error ratio */  
    KDuint32     SNR;	     /* signal-noise ratio  */
    KDint32      frequency_offset;
    KDuint16     sync_stat;  /* synchronous state */
    KDuint16     heartbeat;
    KDuint16     block_count;
    KDuint16     error_block_count;
    KDint16      rssi;
    KDint16      padding;
} MTV_CHANNEL_STATUS_T;


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//! \fn void MTV_GetSWVersionNum (void)
//! \param void No parameter.
//! \return version string pointer
//! \brief  Description: This Function is used to get version string
//! \author Author: Geng Ren
//! \note   Note:Nothing.
/*****************************************************************************/
const KDchar * MTV_GetSWVersionNum (void);

/*****************************************************************************/
//! \fn KDuint32 MTV_Initialize(MTV_MODE_E mode, MTV_CALLBACK callback);
//! \param mode Initial Mobile TV mode, currently only CMMB supported.
//! \return 0 on success, others on error.
//! \brief  Description: This Function is used to initialize the Mobile TV 
//!         module.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 MTV_Initialize(MTV_MODE_E mode, MTV_CALLBACK callback, MTV_CALLBACK dac_callback);

/*****************************************************************************/
//! \fn void MTV_Close (void)
//! \param void No parameter.
//! \return void
//! \brief  Description: This Function is used to deinitialize the Mobile TV 
//!         module.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
void MTV_Close(void);

/*****************************************************************************/
//! \fn void CMMB_Init (void)
//! \param void No parameter.
//! \return void
//! \brief  Description: This Function is used to init CMMB system
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_Init(KDuint32 ca_type);

/*****************************************************************************/
//! \fn void CMMB_LogInit(KDboolean  is_sd_card)
//! \param is_sd_card.
//! \return void
//! \brief  Description: This Function is used to init CMMB Log save to BB
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_LogInit(KDboolean  is_sd_card);

/*****************************************************************************/
//! \fn void CMMB_Cleanup (void)
//! \param void No parameter.
//! \return void
//! \brief  Description: This Function is used to close CMMB system
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_Cleanup(void);

/*****************************************************************************/
//! \fn void CMMB_PlayNetwork(KDuint32 net_eid, KDuint32 service_id, KDuint32 audio_language)
//! \param net_eid network id.
//! \param service_id service id.
//! \param audio_language expected audio language to play
//! \return void
//! \brief  Description: This Function is used to play network programs.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_PlayNetwork(KDuint32 net_eid, KDuint32 service_id, KDuint32 audio_language);

/*****************************************************************************/
//! \fn void CMMB_PlayNetworkByFrq(KDuint32 frequency_num, KDuint32 service_id, KDuint32 audio_language)
//! \param frequency  frequency num.
//! \param service_id service id.
//! \param audio_language expected audio language to play
//! \return void
//! \brief  Description: This Function is used to play network programs.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_PlayNetworkByFrq(KDuint32 frequency_num, KDuint32 service_id, KDuint32 audio_language);

/*****************************************************************************/
//! \fn void CMMB_Stop (void)
//! \param void
//! \return void
//! \brief  Description: This Function is used to stop the playing process.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_Stop(void);

/*****************************************************************************/
//! \fn KDuint32 CMMB_EnumAudioLanguages(KDuint32 *language_code)
//! \param language_code available audio languages list.
//! \return KDuint32 language list length.
//! \brief  Description: This Function is used to get available audio languages
//!                list for current playing stream.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_EnumAudioLanguages(KDuint32 *language_code);

/*****************************************************************************/
//! \fn KDuint32 CMMB_GetAudioLanguage(void)
//! \param void.
//! \return KDuint32 current audio language.
//! \brief  Description: This Function is used to get current audio language.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_GetAudioLanguage(void);

/*****************************************************************************/
//! \fn void CMMB_SetAudioLanguage(KDuint32 language_code)
//! \param language_code language to set.
//! \return void No return.
//! \brief  Description: This Function is used to set audio language.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
void CMMB_SetAudioLanguage(KDuint32 language_code);

/*****************************************************************************/
//! \fn KDuint32 CMMB_Mute(void)
//! \param void.
//! \return 0 on success, others on error.
//! \brief  Description: This Function is used to enable mute 
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_Mute(void);

/*****************************************************************************/
//! \fn KDuint32 CMMB_UnMute(void)
//! \param void.
//! \return 0 on success, others on error.
//! \brief  Description: This Function is used to disable mute 
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_UnMute(void);

/*****************************************************************************/
//! \fn KDuint32 CMMB_GetVolume(void)
//! \param void.
//! \return KDuint32 current volume.
//! \brief  Description: This Function is used to get current volume setting 
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_GetVolume(KDuint32 *dsp_volume, KDuint32 *dac_volume);

/*****************************************************************************/
//! \fn KDuint32 CMMB_SetVolume(KDuint32 volume)
//! \param volume new volume setting.
//! \return 0 on success, others on error.
//! \brief  Description: This Function is used to set volume setting
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_SetVolume(KDuint32 dsp_volume, KDuint32 dac_volume);

/*****************************************************************************/
//! \fn KDuint32 CMMB_PlayStream(const KDuint16 *file_name_ptr,
//!                 KDuint32 audio_language)
//! \param file_name_ptr file name.
//! \param audio_language expected audio language to play
//! \param sub_language expected subtitle language to play
//! \return uint32 SCI_SUCCESS or SCI_ERROR.
//! \brief  Description: This Function is used to play program in file.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_PlayStream(const KDuint16 *file_name_ptr, KDuint32 audio_language);

/*****************************************************************************/
//! \fn KDuint32 CMMB_Pause()
//! \param void.
//! \return uint32 SCI_SUCCESS or SCI_ERROR.
//! \brief  Description: This Function is used to pause the file playing process.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_Pause(void);

/*****************************************************************************/
//! \fn KDuint32 CMMB_Resume()
//! \param void.
//! \return uint32 SCI_SUCCESS or SCI_ERROR
//! \brief  Description: This Function is used to resume the playing for local play.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_Resume(void);

/*****************************************************************************/
//! \fn KDuint32 CMMB_GetStreamLength(const uint16 *file_name_ptr)
//! \param file_name_ptr file name.
//! \return uint32 stream length, in seconds.
//! \brief  Description: This Function is used to get local file play length.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_GetStreamLength(const KDuint16 *file_name_ptr);

/*****************************************************************************/
//! \fn KDuint32 CMMB_SetStreamPos(KDuint32 pos)
//! \param pos set play position.
//! \return uint32 SCI_SUCCESS or SCI_ERROR.
//! \brief  Description: This Function is used to set current play position.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_SetStreamPos(KDuint32 pos);

/*****************************************************************************/
//! \fn KDuint32 CMMB_EnableRecord(MTV_ENABLE_RECORD_E param, 
//!                                     const KDuint16 *file_name_ptr)
//! \param param enable or disable.
//! \param file_name_ptr file name.
//! \return uint32 SCI_SUCCESS or SCI_ERROR.
//! \brief  Description: This Function is used to enable/disable record.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_EnableRecord(MTV_ENABLE_RECORD_E param, 
            const KDuint16 *file_name_ptr);

/*****************************************************************************/
//! \fn KDuint32 CMMB_IdentifyStream (const KDuint16 *file_name_ptr)
//! \param file_name_ptr file name.
//! \return uint32 SCI_TRUE for a correct stream, SCI_FALSE for an incorrect one.
//! \brief  Description: This Function is used to check if a stream can be played
//!         by the operation.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_IdentifyStream (const KDuint16 *file_name_ptr);

/*****************************************************************************/
//! \fn KDuint32 CMMB_SearchProgram (void)
//! \param void.
//! \return MTV_STIMI_SEARCHING_PRG,   MTV_STIMI_ERR_NONE..
//! \brief  Description: This Function is used to search programs automaticaly
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_SearchProgram(void);

/*****************************************************************************/
//! \fn KDuint32 CMMB_SearchCenterFrq (KDuint32 center_frq)
//! \param center frequency.
//! \return MTV_STIMI_SEARCHING_PRG,   MTV_STIMI_INVALID_CENTER_FRQ
//             MTV_STIMI_ERR_NONE..
//! \brief  Description: This Function is used to search programs at a given
//!          center frequency.
//! \author Author: Robert Lu
//! \note   Note:unit kz;            
/*****************************************************************************/
KDuint32 CMMB_SearchCenterFrq(KDuint32 center_frq);

/*****************************************************************************/
//! \fn void CMMB_StopSearchProgram (void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_StopSearchProgram(void);

/*****************************************************************************/
//! \fn KDuint32 CMMB_SearchESGData (KDuint16 net_id)
//! \param network ID.
//! \return void.
//! \brief  Description: This Function is used to search ESG date of a given
//!          network.
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_SearchESGData(KDuint16 net_id);

/*****************************************************************************/
//! \fn void CMMB_StopSearchESGData (void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_StopSearchESGData(void);

/*****************************************************************************/
//! \fn KDboolean CMMB_GetNetworkInfoTab (void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_GetNetworkInfoTab(MTV_STIMI_NET_INFO_TAB_T  *network_info_tab_ptr);

/*****************************************************************************/
//! \fn KDboolean CMMB_GetNetworkInfoList (void)
//! \param network_info_list_ptr.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_GetNetworkInfoList(MTV_STIMI_NET_INFO_LIST_T  *network_info_list_ptr);

/*****************************************************************************/
//! \fn void CMMB_FreeNetworkInfoList()
//! \param network_info_list_ptr.
//! \return void.
//! \brief  Description: This function is used to free memory ,which network
//                information table allocated.
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_FreeNetworkInfoList(MTV_STIMI_NET_INFO_LIST_T * network_info_list_ptr);
                              
/*****************************************************************************/
//! \fn KDboolean CMMB_GetNetworkServiceTab (void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_GetNetworkServiceTab(KDuint16                          network_id,
                                    MTV_STIMI_NETWORK_SERVICE_TAB_T  *network_service_tab_ptr);
/*****************************************************************************/
//! \fn KDboolean CMMB_GetControlTabData (void)
//! \param void.
//! \return KDuint32; actual data size, 0 expresses none data.
//! \brief  Description: This Function is used to get control table data
//! \author Author: Robert Lu
//! \note   Note:the size of buff_ptr >= 800
/*****************************************************************************/
KDuint32 CMMB_GetControlTabData(KDuint16    network_id,     //  network id
                                 KDuint16    buff_len,       // buffer len
                                 KDuint8    *buff_ptr        // buffer pointer                               
                                 );
                               
/*****************************************************************************/
//! \fn KDboolean CMMB_SetControlTabData (void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to set control table data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_SetControlTabData(KDuint16    network_id,     //  network id
                                 KDuint16    buff_len,       // buffer len
                                 KDuint8    *buff_ptr        // buffer pointer                               
                                 );
                               
/*****************************************************************************/
//! \fn KDboolean CMMB_SetProgHintInfo(void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_SetProgHintInfo(KDboolean               is_display,
                               ESG_PROG_HINT_LANG_E  lang);

/*****************************************************************************/
//! \fn KDboolean CMMB_GetProgHintInfo(void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_GetProgHintInfo(ESG_PROG_HINT_INFO_T *hint_ptr);

/*****************************************************************************/
//! \fn KDboolean CMMB_SetEmergBroadcast (void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_SetEmergBroadcast(KDboolean   is_accept);  

/*****************************************************************************/
//! \fn KDboolean CMMB_GetEmergBroadcast (void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_GetEmergBroadcast(KDuint32                trigger_flag,
                                 KDuint32                msg_id,
                                 MTV_STIMI_BROADCAST_U  *bc_ptr);

/*****************************************************************************/
//! \fn KDboolean CMMB_RemvEmergBroadcast (void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to remove emergency broadcast
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
KDboolean CMMB_RemvEmergBroadcast(KDuint32                trigger_flag,
                                                                            KDuint32                msg_id);

/*****************************************************************************/
//! \fn void CMMB_FreeBroadcastMsg(void)
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to stop searching ESG data
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_FreeBroadcastMsg(MTV_STIMI_BROADCAST_U *bc_ptr);  

/*****************************************************************************/
//! \fn void CMMB_SetPublicCallback()
//! \param void.
//! \return void.
//! \brief  Description: This Function is used to set public callbak function
//! \author Author: Robert Lu
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_SetPublicCallback(MTV_CALLBACK callback);

/*****************************************************************************/
//! \fn KDuint32 CMMB_ParseESG
//! \param network_id.
//! \return KDuint32.
//! \brief  Description: This Function is used to parse all ESG data
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_ParseESG(KDuint16  network_id);

/*****************************************************************************/
//! \fn KDuint32 CMMB_ParseService
//! \param network_id.
//! \return KDuint32.
//! \brief  Description: This Function is used to parse service information
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_ParseService(KDuint16  network_id);

/*****************************************************************************/
//! \fn void CMMB_GetServiceName
//! \param network_id, service_id, name_ptr
//! \return ESG_ERROR_E.
//! \brief  Description: This Function is used to get service name
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_GetServiceName(KDuint16       network_id,
                             KDuint16       service_id,
                             MTV_STRING_T  *name_ptr);

/*****************************************************************************/
//! \fn KDuint32 CMMB_GetAllProg
//! \param network_id, service_id, proglist_ptr
//! \return ESG_ERROR_E.
//! \brief  Description: This Function is used to get program data
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_GetAllProg(KDuint16           network_id,
                         KDuint16           service_id,
                         MTV_DATE_T         date,
                         MMIMTV_SERVICE_T  *proglist_ptr);

/*****************************************************************************/
//! \fn void CMMB_FreeString
//! \param str_ptr
//! \return void.
//! \brief  Description: This Function is used to free string struct
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_FreeString(MTV_STRING_T  *str_ptr);

/*****************************************************************************/
//! \fn void CMMB_FreeAllProg
//! \param proglist_ptr
//! \return void.
//! \brief  Description: This Function is used to free all program
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_FreeAllProg(MMIMTV_SERVICE_T  *proglist_ptr);

/*****************************************************************************/
//! \fn void CMMB_EsgCleanup
//! \param void
//! \return void.
//! \brief  Description: This Function is used to cleanup ESG dat
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_EsgCleanup(void);

/*****************************************************************************/
//! \fn void CMMB_SetEsgFilter
//! \param void
//! \return void.
//! \brief  Description: 设置ESG过滤数据
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_SetEsgFilter(MMIMTV_ESG_FILTER_T  filter);

/*****************************************************************************/
//! \fn void CMMB_GetChannelStatus
//! \param cs: channel status pointer
//! \return 0 on success, non-zero on failed.
//! \brief  Description: This Function is used to get channel status.
//! \author Author: geng.ren
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_GetChannelStatus(MTV_CHANNEL_STATUS_T *cs);


/*****************************************************************************/
//! \fn KDuint32 CMMB_GetCAInfo(KDint type, KDint subtype, void *result)
//! \param file_name_ptr file name.
//! \return uint32 SCI_TRUE for a correct stream, SCI_FALSE for an incorrect one.
//! \brief  Description: This Function is used to check if a stream can be played
//!         by the operation.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_GetCAInfo(KDint type, KDint subtype, void *result);

/*****************************************************************************/
//! \fn KDuint32 CMMB_ReqCAInfo(KDint type, KDint subtype)
//! \param file_name_ptr file name.
//! \return uint32 SCI_TRUE for a correct stream, SCI_FALSE for an incorrect one.
//! \brief  Description: This Function is used to check if a stream can be played
//!         by the operation.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_ReqCAInfo(KDint type, KDint subtype);

/*****************************************************************************/
//! \fn KDuint32 CMMB_TakePicture  (KDuint32 buffer_addr,KDuint32 mode,
//! \  KDuint32 *wide,KDuint32 *high)
//! \param center frequency.
//! \return 0: success,   otherwise  fail to take picture.
//! \brief  Description: This Function is used to take picture while watch tv
//! 
//! \author Author: Robert Lu
//! \note   Note:none        
/*****************************************************************************/
KDuint32 CMMB_TakePicture  (KDuint8 *buffer_addr,LCDC_VIDEO_CAP_DATA_TYPE_E mode,KDuint32 *wide,KDuint32 *high);

/*****************************************************************************/
//! \fn void CMMB_ResumePicture  ()
//! \param center frequency.
//! \return none
//! \brief  Description: This Function is used to  resume to play video after take picure
//! \author Author: Robert Lu
//! \note   Note:call afterCMMB_TakePicture.        
/*****************************************************************************/
void  CMMB_ResumePicture  (void);

/*****************************************************************************/
//! \fn void CMMB_SetBackGround  (KDuint8* luma_data_ptr, KDuint8 *chroma_data_ptr,
//! \   KDuint32 wide, KDuint32 high)
//! \param :   luma_data_ptr: Y data of picture. chroma_data_ptr: UV data of picture
//! \               wide: width of picture ; high: hight of picture.
//! \return none           
//! \brief  Description: This Function is used to set background picture
//!          
//! \author Author: Robert Lu
//! \note   Note:none           
/*****************************************************************************/
void  CMMB_SetBackGround  (KDuint8* luma_data_ptr, KDuint8 *chroma_data_ptr, KDuint32 wide, KDuint32 high);

/*****************************************************************************/
//! \fn KDuint32 CMMB_SetEsgDataPath(KDuint16    esg_path_len,  KDuint8     *esg_path_ptr ); 
//! \param esg_path_len   esg path byte len
//! \param esg_path_ptr    esg path pointer
//! \return 0 : on success, non-zero on failed.
//! \brief  Description: This Function is used to set esg data path
//! \author Author: Robert Lu
//! \note   Note: path is existing; esg_path_ptr uses ascii , format according to "D:\MTV",
//!               default "D:\\MTV", max len 20 bytes.
/*****************************************************************************/
 KDuint32 CMMB_SetEsgDataPath(KDuint16    esg_path_len,  KDuint8     *esg_path_ptr );      


/*****************************************************************************/
//! \fn CMMB_DoIQTest(KDuint32 freq)
//! \param void
//! \return 0 on success.
//! \brief  Description: This Function is used to do the IQ Test.
//! \author Author: Geng Ren
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_DoIQTest(KDuint32 freq);

/*****************************************************************************/
//! \fn CMMB_GetIQTestDatLen(void)
//! \param void
//! \return length of IQ Test data catched
//! \brief  Description: This Function is used to do the IQ Test.
//! \author Author: Geng Ren
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_GetIQTestDatLen(void);

/*****************************************************************************/
//! \fn KDuint32 CMMB_GetIQTestDat(KDuint8 * buf_ptr, KDuint32 start, KDuint32 lens)
//! \param buf_ptr the buffer used to hold the IQ Test data
//! \param start the start offset of the data
//! \param len   the length of data you want to get
//! \return 0 on success
//! \brief  Description: This Function is used to get iq test data.
//! \author Author: Geng Ren
//! \note   Note:Nothing.           
/*****************************************************************************/
KDuint32 CMMB_GetIQTestDat(KDuint8 * buf_ptr, KDuint32 start, KDuint32 len);

/*****************************************************************************/
//! \fn KDboolean CMMB_GetCenterFrequency(KDuint32 freq_no,  KDuint32 *center_freq_ptr);
//! \param freq_no[IN], frequency number.
//! \param center_freq_ptr[OUT], center frequency.
//! \return KD_TRUE , get a center frequency, or not .
//! \brief  Description: This Function is used to get center frequency.
//! \author Author: Robert Lu
//! \note   Note: unit kHz.
/*****************************************************************************/
KDboolean CMMB_GetCenterFrequency(KDuint32 freq_no,  KDuint32 *center_freq_ptr);

/*****************************************************************************/
//! \fn KDboolean CMMB_GetFrequency(KDuint16 service_id, KDuint32 *freq_no_ptr,  KDuint32 *center_freq_ptr;
//! \param service_id[IN], service id 
//! \param freq_no_ptr[IN], frequency number.
//! \param center_freq_ptr[OUT], center frequency.
//! \return KD_TRUE , get a center frequency, or not .
//! \brief  Description: This Function is used to get center frequency.
//! \author Author: Robert Lu
//! \note   Note: unit kHz.
/*****************************************************************************/
KDboolean CMMB_GetFrequencyByServid(KDuint16 service_id, KDuint32 *freq_no_ptr,  KDuint32 *center_freq_ptr);

/*****************************************************************************/
//! \fn CMMB_SetSearchMode(KDboolean is_mbbms);
//! \param is_mbbms[IN], is mbbms.
//! \return none .
//! \brief  Description: This Function is used to set searching mode.
//! \author Author: Robert Lu
//! \note   Note: .
/*****************************************************************************/
void CMMB_SetSearchMode(KDboolean is_mbbms);

/*****************************************************************************/
//! \fn CMMB_AbortESG
//! \param none.
//! \return none.
//! \brief  Description: This Function is used to abort ESG parse process.
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_AbortESG(void);

/*****************************************************************************/
//! \fn void CMMB_GetEsgDateRange
//! \param  network_id, service_id, sdate_ptr, edate_ptr
//! \return ESG_ERROR_E.
//! \brief  Description: 获取指定节目频道节目指南的日期范围
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
KDuint32 CMMB_GetEsgDateRange(KDuint16     network_id,
                              KDuint16     service_id,
                              MTV_DATE_T  *sdate_ptr,
                              MTV_DATE_T  *edate_ptr);

/*****************************************************************************/
//! \fn CMMB_AbortESG
//! \param none.
//! \return none.
//! \brief  Description: This Function is used to initialize CMMB tasks.
//! \author Author: Bill Ji
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_Task_Init(void);

/*****************************************************************************/
//! \fn CMMB_SetDisplayParam
//! \param none.
//! \return none.
//! \brief  Description: This Function is used to set display parameter.
//! \author Author: Hao Zhang
//! \note   Note:Nothing.
/*****************************************************************************/
void CMMB_SetDisplayParam(DPLAYER_DISPLAY_PARAM_T *param_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _MTV_API_H

