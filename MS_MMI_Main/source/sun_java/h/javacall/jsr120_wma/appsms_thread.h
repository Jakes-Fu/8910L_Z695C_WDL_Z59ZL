/*****************************************************************************
 *  模块名  ：Java子系统WMA模块本地适配层
 *  文件名	：appsms_Thread.h
 *  文件标识：
 *  相关文件：appsms_Thread.c
 *  实现功能：定义全局变量端口号链表、待发短消息链表，
                                并从不同的线程对它们进行操作。
 *  作者		：
 *  版本		：
 *  完成日期：
 *  其它说明：
 *****************************************************************************/


#ifndef	APPSMS_THREAD_H
#define	APPSMS_THREAD_H
/**************************************************************************
 *                        头文件包含                                      *
 **************************************************************************/
/* 标准库头文件 */


/* 非标准库头文件 */

#include "appsms_core.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**************************************************************************
 *                         全局数据类型定义                                       *
 **************************************************************************/
/*待发送短消息列表中短消息节点*/
typedef struct
{
    T_APPSMS_MsgForList sendSms;           /*待发送的短消息*/
    int32              handle;            /*短消息句柄*/
    BOOLEAN             useFlag;           /*以后扩展使用*/
}T_APPSMS_SendSmsNode; 

/**************************************************************************
 *                        全局函数原型                                     *
 **************************************************************************/
/*检查指定的端口号是否存在于端口号链表中*/
BOOLEAN APPSMS_ExistPortNum(uint16 portNum);

 /*向端口号链表中添加新的端口号*/
BOOLEAN APPSMS_AddPortNumToList(uint16 portNum);

/*从端口号链表中删除指定的端口号*/
BOOLEAN APPSMS_RemovePortNum(uint16 portNum);

/*检查短消息服务的状态*/
BOOLEAN APPSMS_CheckSmsService(void);

/*设置短消息服务的状态*/
void APPSMS_SetSmsService(BOOLEAN state);

 /*向待发送短消息链表中增加待发送短消息*/
BOOLEAN APPSMS_AddSendSmsMsg(const T_APPSMS_MsgForList *sendSms ,int32 handle);

/*检查网络层状态：忙、闲*/
BOOLEAN APPSMS_CheckNetState(void);

/*设置网络层状态*/
void APPSMS_SetNetState(BOOLEAN state);

/*获取第一条待发送短消息*/
BOOLEAN APPSMS_GetFirstSendSms(T_APPSMS_MsgForList * *sendSms, int32 *handle);

/*通过消息句柄删除待发送消息链表中的消息*/
void APPSMS_DelSendSmsByHandle(int32 handle);

/*删除待发送短消息链表中的第一条短消息*/
void APPSMS_DelFirstSendSms(void);

void APPSMS_InitPortNumList(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /* APPSMS_THREAD_H */

