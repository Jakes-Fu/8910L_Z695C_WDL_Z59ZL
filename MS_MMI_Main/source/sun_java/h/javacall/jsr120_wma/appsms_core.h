/*****************************************************************************
 *  模块名  ：Java子系统WMA模块本地适配层
 *  文件名	：appsms_Core.h
 *  文件标识：
 *  相关文件：appsms_Core.c
 *  实现功能：定义APPSMS适配层内部使用的数据结构和函数接口
                               
 *  作者		：
 *  版本		：
 *  完成日期：
 *  其它说明：
 *****************************************************************************/
#include "javacall_defs.h"
#include "sci_types.h"

#ifndef	APPSMS_CORE_H
#define	APPSMS_CORE_H
/**************************************************************************
 *                        头文件包含                                      *
 **************************************************************************/
/* 标准库头文件 */


/* 非标准库头文件 */
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**************************************************************************
 *                         常量定义                                           *
 **************************************************************************/
 
/**************************************************************************
 *                         全局宏定义                                         *
 **************************************************************************/

/*受限的端口数*/
#define  APPSMS_RESTRICT_PORT_NUM   16

/*端口号列表的最大长度*/
#define  APPSMS_PORT_LIST_LEN         20

/*电话号码的长度*/
#define  APPSMS_MAX_ADD_LEN         20

/*短消息队列中支持的最大物理条数*/
#define  APPSMS_MAX_SMS_NUM         10

/*7-bit编码类型短消息UD的最大长度*/
#define  APPSMS_7BIT_SMS_LEN           160

/*8-bit编码类型短消息UD的最大长度*/
#define  APPSMS_8BIT_SMS_LEN           140

/*普通连接的WAP PUSH短消息的端口号*/
#define  APPSMS_PORT_2948              (uint16)2948

/*安全连接的WAP PUSH短消息的端口号*/
#define  APPSMS_PORT_2949              (uint16)2949


/**************************************************************************
 *                         全局数据类型定义                                       *
 **************************************************************************/
 /*短消息编码类型*/
typedef enum
{
    APPSMS_SMSENCODE_7_BIT = 0 ,   /*7-bit*/
    APPSMS_SMSENCODE_8_BIT,        /*8-bit*/
    APPSMS_SMSENCODE_UCS2          /*16-bit*/
} T_APPSMS_Encoding;


/*用于网络传输的短消息*/
typedef struct
{
    T_APPSMS_Encoding msgType;             /*编码类型*/
    uint8   address[APPSMS_MAX_ADD_LEN+1];    /*地址（因为没有区分mt和mo，可能是源地址，
                                                   也可能是目标地址）*/
    uint8   msgBuffer[APPSMS_8BIT_SMS_LEN+1];  /*用户数据头+有效载荷*/
    int32   msgBufferLen;                       /*用户数据头+有效载荷的长度*/
    uint32 timeStamp;                         /*时间戳，对于mo短消息为0*/
}T_APPSMS_MsgForNet;

/*用于APPSMS使用的短消息*/
typedef struct
{
    T_APPSMS_Encoding msgType;                /*编码类型*/
    uint8  address[APPSMS_MAX_ADD_LEN+1];       /*地址（因为没有区分mt和mo，可能是源地址，
                                                    也可能是目标地址）*/
    uint16 sourcePort;                          /*源端口号*/
    uint16  destPort;                            /*目标端口号*/
    
    /*短消息数据（串接后或折分前的数据）*/
    uint8   msgBuffer[APPSMS_7BIT_SMS_LEN* APPSMS_MAX_SMS_NUM];  
    int32   msgBufferLen;                         /*短消息数据的长度*/
    uint32 timeStamp;                          /*时间戳，对于mo短消息为0*/ 
}T_APPSMS_MsgForJava;

/*用于适配层短消息链表使用的短消息*/
typedef struct
{
    T_APPSMS_Encoding msgType;                  /*编码类型*/
    uint8 address[APPSMS_MAX_ADD_LEN+1];         /*地址（因为没有区分mt和mo，可能是源地址，
                                                      也可能是目标地址）*/
    uint16 sourcePort;                            /*源端口号*/
    uint16 destPort;                              /*目标端口号*/
    uint8   msgBuffer[APPSMS_7BIT_SMS_LEN];        /*有效载荷*/
    int32 msgBufferLen;                           /*有效载荷长度*/
    uint16   ref;                                   /*长短消息的参考码，可能会是16-bit*/
    uint8   max;                                   /*长短消息折分的短消息条数*/
    uint8   seq;                                   /*短消息的序列号*/
    uint32  timeStamp;                            /*时间戳，对于mo短消息为0*/
}T_APPSMS_MsgForList;

/*短消息的发送结果*/
typedef enum
{
    APPSMS_SMSSEND_SUCCESS = 0,       /*发送成功*/
    APPSMS_SMSSEND_FAIL,              /*发送失败*/
    APPSMS_SMSSEND_FAILENCODE,        /*短消息编码错误*/
    APPSMS_SMSSEND_UNKNOW,            /*未知错误*/
    APPSMS_SMSSEND_NETBUSY            /*网络忙*/
} T_APPSMS_SendResult;


/*接收的短消息缓存列表*/
typedef struct
{
    T_APPSMS_MsgForList  recvSms;
    BOOLEAN              useFlag;
}T_APPSMS_RecvSmsNode;



/**************************************************************************
 *                        全局函数原型                                     *
 **************************************************************************/
/*注册端口号*/
BOOLEAN APPSMS_AddPortNum(uint16 portNum);

/*计算长短消息的分段数*/
int32 APPSMS_GetSmsSegments (T_APPSMS_Encoding msgType,int32 msgBufferLen, BOOLEAN hasPort);


/*发送短消息*/
int32 APPSMS_SendSms(const T_APPSMS_MsgForJava *sendSms);

/*折分短消息*/
BOOLEAN APPSMS_MsgSegment(const T_APPSMS_MsgForJava *msg , 
                           uint8 pos ,
						   uint8 max,
						   uint8 ref,
						   T_APPSMS_MsgForList *sendSms,
						   BOOLEAN hasPort);


/*生成句柄*/
int32 APPSMS_GenHandle(void);

/*生成长短消息参考码*/
int32 APPSMS_GenReference(void);

/*短消息编码*/
BOOLEAN APPSMS_EncodeSms(const T_APPSMS_MsgForList *msg,T_APPSMS_MsgForNet *sendSms);

/*短消息解码*/
BOOLEAN APPSMS_DecodeSms(const T_APPSMS_MsgForNet *recvSms, T_APPSMS_MsgForList *msg);

/*上报短消息发送结果*/
void APPSMS_ReceiveSendResult(T_APPSMS_SendResult result);

/*接收短消息*/
void  APPSMS_ReceiveRecvSms(const T_APPSMS_MsgForNet *recvSms);


/*检查长短消息是否全部接收完成*/
BOOLEAN APPSMS_CheckLongRecvSms(const T_APPSMS_MsgForList *recvSms);

/*添加短消息到链表中*/
BOOLEAN APPSMS_AddRecvSmsToList(const T_APPSMS_MsgForList *recvSms);

/*串解长短消息*/
void APPSMS_MsgConcatenation (const T_APPSMS_MsgForList *recvSms,T_APPSMS_MsgForJava *msg);

/*根据长短消息的参考码删除接收短消息链表中的短消息*/
void APPSMS_DelRecvSmsByRef (const T_APPSMS_MsgForList *recvSms);

/*删除接收短消息链表中五天前的短消息*/
void APPSMS_DelRecvSmsByDate(void);

/*把年月日的日期格式转换成到2000 年1 月1 日的秒数*/
int32 APPSMS_ConvertTime2Long(uint8 year , uint8 month , uint8 day , uint8 hour , uint8 minute , uint8 second);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /* APPSMS_CORE_H */
