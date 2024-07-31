/*****************************************************************************
 *  模块名      ：Java子系统WMA模块本地适配层
 *  文件名      ：appsms_4Spread.h
 *  文件标识：
 *  相关文件：appsms_4Spread.c
 *  实现功能：定义与展讯平台相关的接口函数（7API ）
 *  作者	      ：
 *  版本	      ：
 *  完成日期：
 *  其它说明：
 *****************************************************************************/


#ifndef	APPSMS_4SPREAD_H
#define	APPSMS_4SPREAD_H
/**************************************************************************
 *                        头文件包含                                      *
 **************************************************************************/
/* 标准库头文件 */


/* 非标准库头文件 */
#include "appsms_core.h"
#include "mn_type.h"
#include "mmisms_app.h"
#include "mn_api.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**************************************************************************
 *                        全局函数原型                                     *
 **************************************************************************/
 /*处理MN层APP_MN_SMS_READY_IND消息*/
void APPSMS_SmsReady_CallBack();

BOOLEAN APPSMS_SendSmsNet(const T_APPSMS_MsgForList *msg,T_APPSMS_MsgForNet *sendSms);

/*处理短消息的发送结果*/
void APPSMS_SendResult_CallBack(MN_SMS_CAUSE_E result,MN_SMS_TP_MR_T tp_mr);

/*处理MT短消息*/
void APPSMS_RecvSms_CallBack(const MN_SMS_MT_SMS_T *recvSms);

/*获取互斥量*/
BOOLEAN  APPSMS_GetMutex(uint32 timeOut);

/*释放互斥量*/
BOOLEAN  APPSMS_PutMutex(void);

/*获取当前时间*/
int32 APPSMS_GetCurrentTime(void);

/*判断是否是JAVA  短消息*/
BOOLEAN APPSMS_IsAppSms_CallBack(MN_DUAL_SYS_E dual_sys,APP_SMS_USER_DATA_T    *user_data_ptr);

BOOLEAN APPSMS_IsSmsForJava(
                            MN_SMS_USER_HEAD_T         user_head_ptr_t,
                            int32                    *des_port_ptr,
                            int32                    *org_port_ptr
                            );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /* APPSMS_4SPREAD_H */
