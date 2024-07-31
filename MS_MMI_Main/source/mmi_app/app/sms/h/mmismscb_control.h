/******************************************************************************
**
**
**
**  小区广播模块
**
**
**
**
**
**
*******************************************************************************/
#if defined(MMI_SMSCB_SUPPORT)

#ifndef  _MMISMSCB_CONTROL_H_    
#define  _MMISMSCB_CONTROL_H_   
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mn_type.h"
#include "dal_time.h"
#include "mmisms_export.h"
//#include "mmismscb_wintab.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


#define MMI_MAX_CB_LANGUAGE_NUM (MN_CB_LANGUAGE_ID_MAX_NUM + 1)               //最大语言数
#ifdef MMI_ETWS_SUPPORT
#define MMI_CB_CHANNEL_NAME_LEN 60               //最大信道列表数
#else
#define MMI_CB_CHANNEL_NAME_LEN 25               //最大信道列表数
#endif
#define MMI_MAX_CB_CHANNEL_NUM  MN_CB_MSG_ID_MAX_NUM               //最大信道数

#ifdef MMI_ETWS_SUPPORT
#define MMI_MAX_CB_MSG_NUM      10     //最大广播信息存储数
#else
#define MMI_MAX_CB_MSG_NUM      (10 * MMI_DUAL_SYS_MAX)     //最大广播信息存储数
#endif
#define MMI_CB_ALERT_NUM 0             //CB Alert的信道个数
#ifdef MMI_ETWS_SUPPORT
#define MMI_CB_ETWS_TITLE_NAME_MAX_LEN  25
#endif

#ifdef MMI_ETWS_SUPPORT
#define TXT_CMAS_CHILE_4370_TITLE {'C', 'L', '-', 'A', 'l', 'e', 'r', 't', 'a', ' ', 'L', 'o', 'c', 'a', 'l', 0}
#define TXT_CMAS_CHILE_4380_TITLE {'C', 'L', '-', 'P', 'r', 'u', 'e', 'b', 'a', 0}

#define TXT_CMAS_CHILE_4370_POPUP_TITLE {'A', 'l', 'e', 'r', 't', 'a', ' ', 'd', 'e', ' ', 'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'i', 'a', 0}
#define TXT_CMAS_CHILE_4380_POPUP_TITLE {'M', 'e', 'n', 's', 'a', 'j', 'e', ' ', 'd', 'e', ' ', 'P', 'r', 'u', 'e', 'b', 'a', 0}

#define TXT_CMAS_UAE_4370_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0x0020, 0x0648, 0x0637, 0x0646, 0x064A, 0}
#define TXT_CMAS_UAE_4371_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0}
#define TXT_CMAS_UAE_4372_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0}
#define TXT_CMAS_UAE_4373_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0}
#define TXT_CMAS_UAE_4374_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0}
#define TXT_CMAS_UAE_4375_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0}
#define TXT_CMAS_UAE_4376_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0}
#define TXT_CMAS_UAE_4377_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0}
#define TXT_CMAS_UAE_4378_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x0020, 0x0637, 0x0627, 0x0631, 0x0626, 0}
#define TXT_CMAS_UAE_4379_TITLE {0x0631, 0x0633, 0x0627, 0x0644, 0x0629, 0x0020, 0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x064A, 0x0629, 0}
#define TXT_CMAS_UAE_4380_TITLE {0x0631, 0x0633, 0x0627, 0x0644, 0x0629, 0x0020, 0x062A, 0x062C, 0x0631, 0x064A, 0x0628, 0x064A, 0x0629, 0}
#define TXT_CMAS_UAE_4381_TITLE {0x062A, 0x0645, 0x0631, 0x064A, 0x0646, 0}
#define TXT_CMAS_UAE_4383_TITLE {'N', 'a', 't', 'i', 'o', 'n', 'a', 'l', ' ', 'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4384_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4385_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4386_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4387_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4388_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4389_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4390_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4391_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4392_TITLE {'W', 'a', 'r', 'n', 'i', 'n', 'g', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4393_TITLE {'T', 'e', 's', 't', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4394_TITLE {'E', 'x', 'e', 'r', 'c', 'i', 's', 'e', 0}
#define TXT_CMAS_UAE_4396_TITLE {0x062A, 0x062D, 0x0630, 0x064A, 0x0631, 0x003A, 0x0020, 0x0627, 0x0644, 0x0633, 0x0644, 0x0627, 0x0645, 0x0629, 0x0020, 0x0627, 0x0644, 0x0639, 0x0627, 0x0645, 0x0629, 0}
#define TXT_CMAS_UAE_4397_TITLE {'P', 'u', 'b', 'l', 'i', 'c', ' ', 'S', 'a', 'f', 'e', 't', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_UAE_4398_TITLE {0x0631, 0x0633, 0x0627, 0x0644, 0x0629, 0x0020, 0x062A, 0x062C, 0x0631, 0x064A, 0x0628, 0x064A, 0x0629, 0}
#define TXT_CMAS_UAE_4399_TITLE {'T', 'e', 's', 't', ' ', 'A', 'l', 'e', 'r', 't', 0}

#define TXT_CMAS_PWS_911_TITLE {0x8B66, 0x8A0A, 0x901A, 0x77E5, 0}
#define TXT_CMAS_PWS_919_TITLE {'A', 'l', 'e', 'r', 't', ' ', 'M', 'e', 's', 's', 'a', 'g', 'e', 0}
#define TXT_CMAS_PWS_4370_TITLE {0x570B, 0x5BB6, 0x7D1A, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4371_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4372_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4373_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4374_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4375_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4376_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4377_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4378_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4379_TITLE {0x7DCA, 0x6025, 0x8B66, 0x5831, 0}
#define TXT_CMAS_PWS_4380_TITLE {0x6BCF, 0x6708, 0x6E2C, 0x8A66, 0x7528, 0x8A0A, 0x606F, 0}
#define TXT_CMAS_PWS_4383_TITLE {'P', 'r', 'e', 's', 'i', 'd', 'e', 'n', 't', 'i', 'a', 'l', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4384_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4385_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4386_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4387_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4388_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4389_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4390_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4391_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4392_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_PWS_4393_TITLE {'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd', ' ', 'M', 'o', 'n', 't', 'h', 'l', 'y', ' ', 'T', 'e', 's', 't', 0}

#define TXT_CMAS_EU_ALERT_NL_4371_TITLE {'(', '4', '3', '7', '1', ')', 0}

#define TXT_CMAS_NZ_4370_TITLE {'E', 'm', 'e', 'r', 'g', 'e', 'n', 'c', 'y', ' ', 'A', 'l', 'e', 'r', 't', 0}

#define TXT_CMAS_RO_TITLE {'R', 'O', '-', 'A', 'L', 'E', 'R', 'T', 0}
#define TXT_CMAS_RO_4370_TITLE {'P', 'r', 'e', 's', 'i', 'd', 'e', 'n', 't', 'i', 'a', 'l', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_RO_4371_TITLE {'E', 'x', 't', 'r', 'e', 'm', 'e', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_RO_4375_TITLE {'S', 'e', 'v', 'e', 'r', 'e', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_RO_4379_TITLE {'O', 'r', 'a', 'n', 'g', 'e', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_RO_4381_TITLE {'E', 'x', 'e', 'r', 'c', 'i', 's', 'e', ' ','A', 'l', 'e', 'r', 't',0}
#define TXT_CMAS_RO_919_TITLE {'E', 'x', 't', 'r', 'e', 'm', 'e', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_RO_918_TITLE {'S', 'e', 'v', 'e', 'r', 'e', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_RO_917_TITLE {'O', 'r', 'a', 'n', 'g', 'e', ' ', 'A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_RO_916_TITLE {'E', 'x', 'e', 'r', 'c', 'i', 's', 'e', ' ','A', 'l', 'e', 'r', 't',0}

#define TXT_CMAS_IL_4370_TITLE {'E','m','e','r','g','e','n','c','y',' ','A', 'l', 'e', 'r', 't', 0}
#define TXT_CMAS_IL_4371_TITLE {'W','a','r','n','i','n','g',' ','N','o','t','i','f','i','c','a','t','i','o','n', 0}
#define TXT_CMAS_IL_4373_TITLE {'I','n','f','o','r','m','a','t','i','o','n','a','l', 0}
#define TXT_CMAS_IL_4379_TITLE {'A','p','p','l','y',' ','f','o','r',' ','P','u','b','l','i','c',' ','a','s','s','i','s','t','a','n','c','e', 0}
#define TXT_CMAS_IL_4380_TITLE {'T','e','s','t', 0}
#define TXT_CMAS_IL_4381_TITLE {'E','x','e','r','c','i','s','e', 0}

#define TXT_CMAS_GR_TITLE {'G', 'R', '-', 'A', 'L', 'E', 'R', 'T', 0}

#endif

#ifdef MMI_ETWS_SUPPORT
typedef struct
{
    uint16 channel_id;
    uint16 primary_channel;
    uint16 secondary_channel;
    uint16 associated_channel;
    wchar title_id[MMI_CB_ETWS_TITLE_NAME_MAX_LEN];
    MMI_TEXT_ID_T title_string_id;
    MMI_TEXT_ID_T popup_string_id;
    uint16 is_default_enabled;
    uint16 can_be_disabled;
    uint16 can_be_visible;
    uint16 alert_level;
    uint16 alert_tone_type;
    uint16 vibrate_type;
    uint16 is_continued;
    uint16 alert_period;
} CMAS_CHANNEL_INFO_T;

typedef struct
{
    uint16 mcc;
    CMAS_CHANNEL_INFO_T *channel_info;
} CMAS_CONTROL_LIST_T;

typedef enum
{
    MMI_CMAS_CHILE_ID = 0x00, 
    MMI_CMAS_UAE_ID,
    MMI_CMAS_TAIWAN_PWS_ID,
    MMI_CMAS_EU_ALERT_NL_ID,
    MMI_CMAS_EU_ALERT_LT_ID,
    MMI_CMAS_MAX_ID
} CMAS_CB_SPEC_T;

extern CMAS_CHANNEL_INFO_T s_cmas_chile_channel_info[];
extern CMAS_CHANNEL_INFO_T s_cmas_uae_channel_info[];
extern CMAS_CHANNEL_INFO_T s_cmas_pws_channel_info[];
extern CMAS_CHANNEL_INFO_T s_cmas_ue_alert_nl_channel_info[];
#endif

//------------------------------------------------------------------------------------------
//
// 小区广播语言选择列表结构
//
//------------------------------------------------------------------------------------------
typedef struct{

    BOOLEAN selected_arr[MMI_MAX_CB_LANGUAGE_NUM];                       //语言选择列表数组

}MMISMS_CB_LANGUAGE_T;


//------------------------------------------------------------------------------------------
//
// 小区广播信道名称结构
//
//------------------------------------------------------------------------------------------
typedef struct{

        wchar    wstr[MMI_CB_CHANNEL_NAME_LEN + 1];                           //信道名称字符串数组
        uint16   wstr_len;                                                //信道名称长度
}MMISMS_CB_CHANNEL_NAME_T;

//------------------------------------------------------------------------------------------
//
// 小区广播信道选项结构
//
//------------------------------------------------------------------------------------------
typedef struct{

        MN_CB_MSG_ID_T           msg_id;                                 //信道索引号
        MMISMS_CB_CHANNEL_NAME_T name;                                   //信道名称数组
        BOOLEAN is_enabled;                                              //是否开启       
}MMISMS_CB_CHANNEL_ITEM_T;

//------------------------------------------------------------------------------------------
//
// 小区广播信道列表结构
//
//------------------------------------------------------------------------------------------
typedef struct{

        uint16 num;                                                      //信道总数
        MMISMS_CB_CHANNEL_ITEM_T channel_arr[MMI_MAX_CB_CHANNEL_NUM];    //信道选项数组

}MMISMS_CB_CHANNEL_T;

//------------------------------------------------------------------------------------------
//
// 小区广播消息选项结构
//
//------------------------------------------------------------------------------------------
typedef struct mmisms_cbmsg_tag
{
    wchar             context[MN_CB_MACRO_MSG_LEN + 1];                   //小区广播消息内容
    uint16            len;                                            //小区广播消息内容长度
    SCI_TIME_T        time;                                           //接受时间
    SCI_DATE_T        date;                                           //接受日期
    MN_CB_MSG_ID_T    msg_id;                                         //信道索引号
    BOOLEAN           is_read;                                        //是否已被阅读
    MN_DUAL_SYS_E     dual_sys;
    struct mmisms_cbmsg_tag * msg_next_ptr;
}MMISMS_CB_MESSAGE_ITEM_T;

#ifdef MMI_ETWS_SUPPORT
#if 0
typedef struct mmisms_cbsmsg_tag
{
    wchar             context[MN_CB_MACRO_MSG_LEN + 1];                   //小区广播消息内容
    uint16            len;                                            //小区广播消息内容长度
    SCI_TIME_T        time;                                           //接受时间
    SCI_DATE_T        date;                                           //接受日期
    MN_CB_MSG_ID_T    msg_id;                                         //信道索引号
    BOOLEAN           is_read;                                        //是否已被阅读
    MN_DUAL_SYS_E     dual_sys;
}MMISMS_CB_STORAGE_MSG_ITEM_T;
#endif

typedef struct
{
    wchar             context[MN_CB_MACRO_MSG_LEN + 1];                   //小区广播消息内容
    uint16            len;                                            //小区广播消息内容长度
    SCI_TIME_T        time;                                           //接受时间
    SCI_DATE_T        date;                                           //接受日期
    MN_CB_MSG_ID_T    msg_id;                                         //信道索引号
    BOOLEAN           is_read;                                        //是否已被阅读
    MN_DUAL_SYS_E     dual_sys;
}MMISMS_CB_MESSAGE_ARRAY_T;

#endif

//------------------------------------------------------------------------------------------
//
// 小区广播消息列表结构
//
//------------------------------------------------------------------------------------------
typedef struct{
        uint16 num;                                                      //信息列表总数
        MMISMS_CB_MESSAGE_ITEM_T *msg_ptr;                                 //信息列表数组
}MMISMS_CB_MESSAGE_T;

//------------------------------------------------------------------------------------------
//
// 小区广播重要CB Channel 结构体
//
//------------------------------------------------------------------------------------------
typedef struct{
    uint16 type;
    MN_CB_MSG_ID_T cb_alert_id;
#ifdef MMI_ETWS_SUPPORT
	BOOLEAN is_enabled;
	MMISMS_CB_CHANNEL_NAME_T name;
#endif		
}MN_CB_ALERT_CONTENT_T;

#ifdef MMI_ETWS_SUPPORT
typedef struct{
    uint16 type;
    MN_CB_MSG_ID_T cb_alert_id;
}MN_CB_ETWS_CONTENT_T;
#endif

typedef struct{
    uint16 emegence_num;
    uint16 sim_cbmi_num;
    uint16 soft_conf_num;
#ifdef MMI_ETWS_SUPPORT
//    uint16 etws_num;
#endif	
    MN_CB_ALERT_CONTENT_T cb_alert_content[MMI_MAX_CB_CHANNEL_NUM];
#ifdef MMI_ETWS_SUPPORT
//    MN_CB_ETWS_CONTENT_T cb_etws_content[MMI_MAX_CB_CHANNEL_NUM];
#endif
    MN_CB_MSG_ID_T soft_conf_id[5];
}MN_CB_ALERT_MSG_T;

typedef enum
{
    MMI_CB_EMERGENCE_ID = 0x00, 
#ifdef MMI_ETWS_SUPPORT
		MMI_CB_ETWS_ID,
#endif		
		MMI_CB_SIM_CBMI_ID,
		MMI_CB_USER_CONF_ID,
}MMI_CB_ALERT_ID_T;

#ifdef MMI_ETWS_SUPPORT
typedef enum
{
    CB_CONFIG_OFF = 0x00, 
    CB_CONFIG_ON,
    CB_CONFIG_DISABLE,
    CB_CONFIG_MAX
}MMISMS_CB_CONFIG_T;
#endif

/******************************************************************************************************************/
// 
//
//                        小区广播模块对外接口函数声明
//
//
/******************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_GetCBMsgType
//  功能描述           :        
//  全局变量使用:    s_is_alert_type_msg
//  返回值                 :    TRUE    : Alert Message
//                                          FALSE  : Normal Message 
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMIAPISMSCB_GetCBMsgType(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_IsCBDefaultEnable
//  功能描述:        根据配置化软件判断是否打开CB功能NV_VARIANT_CONFIG_TELE/CELL_BROADCAST
//  全局变量使用:    cell_broadcast
//  返回值:          TRUE 默认打开CB，FALSE 默认关闭CB
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMSCB_IsCBDefaultEnable(void);

 /***************************************************************************/
//     Description : 获得静态结构体数组变量s_cb_cbmis 的地址
//    Global resource dependence : none
//    Author:Tong Yongli
//    Note:
/***************************************************************************/
PUBLIC MN_CB_ALERT_MSG_T * MMIAPISMSCB_GetCbmisValOfAddr(MN_DUAL_SYS_E dual_sys);

 /*****************************************************************************/
//  Description : get cbmi from sim
//  Global resource dependence : none
//  Author: CBK
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetCBMIFromSim(DPARAM param);

 /*****************************************************************************/
//  Description : get g_cb_alert_id[index]
//  Global resource dependence : none
//  Author: CBK
//  Note: 
/*****************************************************************************/
PUBLIC MN_CB_MSG_ID_T MMIAPISMSCB_GetCBAlertID(uint8 index);

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description :
//    Global resource dependence : 
//  Author:Candice.Chen
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetETWSNum(void);

/*****************************************************************************/
//     Description :
//    Global resource dependence : 
//  Author:Candice.Chen
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetETWSNum(uint16 num);
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_InitCBAlertInfo
//  功能描述           :        
//  全局变量使用:    s_cb_alert_msg
//  返回值                 :    void 
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMIAPISMSCB_InitCBAlertInfo(MN_DUAL_SYS_E dual_sys);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_GetCBAllAlertInfo
//  功能描述           :        
//  全局变量使用:    s_cb_all_alert_info
//  返回值                 :    
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MN_CB_ALERT_MSG_T  * MMIAPISMSCB_GetCBAllAlertInfo(MN_DUAL_SYS_E dual_sys);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMsgNodeToCBMsgList
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            CBK
//
//----------------------------------------------------------------------------------------------------------------

PUBLIC void AddNewMsgNodeToCBMsgList(MMISMS_CB_MESSAGE_ITEM_T* cur_msg_ptr);

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Num
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:           Candice
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC void MMISMS_Set_CB_Message_Num( uint16 num );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Ptr
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_MESSAGE_T *MMISMS_Get_CB_Message_Parent_Ptr( void );
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Num
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC uint16 MMISMS_Get_CB_Message_Num( void );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Ptr
//  功能描述:        
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC MMISMS_CB_MESSAGE_ITEM_T *MMISMS_Get_CB_Message_Ptr( void );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_DeleteMsgFromCBMsgList
//  功能描述:        从小区广播收件箱列表中删除一条记录
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            liqing.peng
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_DeleteMsgFromCBMsgList(
                                  uint16      index
                                  );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         IsCBMsgListEmpty
//  功能描述:        判断广播收件箱是否为空
//  全局变量使用:    s_cb_msg
//  返回值:          TRUE 为空，FALSE 不为空
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMS_IsCBMsgListEmpty(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         UpdateCBMsgList
//  功能描述:        刷新小区广播收件箱列表
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_UpdateCBMsgList(
                           MMI_CTRL_ID_T    ctrl_id        //IN:
                           );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_Init
//  功能描述:        初始化小区广播接口函数(在手机初始化时调用, MMIMAIN.c 中调用)
//  全局变量使用:    无
//  返回值:          MN_RETURN_SUCCESS 初始化成功，MN_RETURN_FAILURE 初始化失败
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
MN_RETURN_RESULT_E MMISMSCB_Init(MN_DUAL_SYS_E dual_sys);


/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
void MMISMSCB_OpenMainMenu(void);


//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_SwitchSmsCBStatus
//  功能描述:        切换小区广播启动状态
//  全局变量使用:    无
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMISMSCB_SwitchSmsCBStatus(
                                BOOLEAN cb_enabled      //要切换的状态 (输入参数)
                                );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_SwitchCB50Channel
//  功能描述:        切换50频段小区广播
//  全局变量使用:    无
//  返回值:          无
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
void MMISMSCB_SwitchCB50Channel(
                                BOOLEAN cb_enabled      //要切换的状态 (输入参数)
                                );

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
void MMISMSCB_OpenSettingWin(void);

/*****************************************************************************/
//  Description : set setting card
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_SetSelectCard(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : get setting card
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E  MMISMSCB_GetSelectCard( void );

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         InitCB_ETWSChannel
//  功能描述:       
//  全局变量使用:    cell_broadcast_channel
//  返回值:          
//  作者:   
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void InitCB_ETWSChannel(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Current Channel table according the MCC
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC CMAS_CHANNEL_INFO_T* GetCMASChannelTable( MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Current Channel info according the channel ID
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC CMAS_CHANNEL_INFO_T* GetCMASChannelInfo( MN_DUAL_SYS_E dual_sys, uint16 msg_id);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
#endif //#if defined(MMI_SMSCB_SUPPORT)

