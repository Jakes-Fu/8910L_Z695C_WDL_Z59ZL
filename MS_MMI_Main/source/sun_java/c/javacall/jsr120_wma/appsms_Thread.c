/*****************************************************************************
 *  模块名  ：Java子系统WMA模块本地适配层
 *  文件名	：appsms_Thread.c
 *  文件标识：
 *  相关文件：appsms_Thread.h
 *  实现功能：对全局变量端口号链表、待发短消息链表，
                                进行跨线程操作接口的实现。
 *  作者		：
 *  版本		：
 *  完成日期：
 *  其它说明：
 *
 *  修改记录1：
 *    日  期：
 *    修改记录号:	
 *    修改人：              
 *    修改内容：
 *  修改记录2：
 *****************************************************************************/

/**************************************************************************
 *                        头文件包含                                      *
 **************************************************************************/
/* 标准库头文件 */


/* 非标准库头文件 */

#include "appsms_thread.h"
#include "appsms_4spread.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**************************************************************************
 *                         全局变量声明                                    *
 **************************************************************************/
/* 端口号列表 */
uint16 g_wappsms_portNumList[APPSMS_PORT_LIST_LEN];

/*待发送短消息列表*/
T_APPSMS_SendSmsNode g_tappsms_sendSmsList[APPSMS_MAX_SMS_NUM];
int32 nStart=-1;
int32 nEnd=0;
 
/* MN层状态
*  TRUE：表示忙
*  FALSE：表示闲
*  默认为闲 */
BOOLEAN g_bappsms_netIsBusy=0;

/* 短消息服务的状况
* TRUE：表示正常
* FALSE：表示异常
* 默认为异常 */
BOOLEAN g_bappsms_smsAvailable=0;

/**************************************************************************
 *                     全局函数实现                                      *
 **************************************************************************/
void APPSMS_InitPortNumList() {
    int i;

    for (i=0; i<APPSMS_PORT_LIST_LEN; i++) {
        g_wappsms_portNumList[i] = 0;
    }
}
/*****************************************************************************
 *	函数名		：APPSMS_ExistPortNum
 *	功能		  ：检查指定的端口号是否存在于端口号列表中
 *	输入参数	：portNum端口号
 *	输出参数	：无
 *	返回值说明：指定的端口号在端口号列表中是否存在
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_ExistPortNum(uint16 portNum){
    uint8 i;

    for(i=0;i<APPSMS_PORT_LIST_LEN;i++){
        if(g_wappsms_portNumList[i]==portNum){
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************
 *	函数名		：APPSMS_AddPortNumToList
 *	功能		  ：向端口号列表中增中端口号
 *	输入参数	：portNum端口号
 *	输出参数	：无
 *	返回值说明：成功或者失败
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_AddPortNumToList(uint16 portNum){
    uint8 i;

    for(i=0;i<APPSMS_PORT_LIST_LEN;i++){
        if(g_wappsms_portNumList[i]==0){
            g_wappsms_portNumList[i]=portNum;
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************
 *	函数名		：APPSMS_RemovePortNum
 *	功能		  ：从端口号列表中删除指定的端口号
 *	输入参数	：portNum端口号
 *	输出参数	：无
 *	返回值说明：成功或者失败
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_RemovePortNum(uint16 portNum){
    uint8 i;

    for(i=0;i<APPSMS_PORT_LIST_LEN;i++){
        if(g_wappsms_portNumList[i]==portNum){
            g_wappsms_portNumList[i]=0;
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************
 *	函数名		：APPSMS_CheckSmsService
 *	功能		  ：检查短消息服务的状态
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：正常或者异常
 *	其他说明	：TRUE表示正常；FALSE表示异常
 *****************************************************************************/
BOOLEAN APPSMS_CheckSmsService(void)
{   
    return g_bappsms_smsAvailable;
}

/*****************************************************************************
 *	函数名		：APPSMS_SetSmsService
 *	功能		  ：设置短消息服务的状态
 *	输入参数	：state短消息服务的状态
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：TRUE表示正常；FALSE表示异常
 *****************************************************************************/
void APPSMS_SetSmsService(BOOLEAN state)
{
    g_bappsms_smsAvailable = state;
}

/*****************************************************************************
 *	函数名		：APPSMS_AddSendSmsMsg
 *	功能		       ：向待发送短消息链表中增加待发送短消息
 *	输入参数	：sendSms待发送的短消息;handle：短消息的句柄
 *	输出参数	：无
 *	返回值说明：成功或者失败
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_AddSendSmsMsg(const T_APPSMS_MsgForList *sendSms ,int32 handle){

    if(nStart == nEnd){/*队列已满*/
        return FALSE;
    }else{/*未满*/
        /*队列为空*/
        if(nStart==-1){
            nStart = nEnd;
        }
        
        g_tappsms_sendSmsList[nEnd].handle = handle;
        SCI_MEMCPY(&(g_tappsms_sendSmsList[nEnd].sendSms),sendSms,sizeof(T_APPSMS_MsgForList));
        nEnd++;

        if(nEnd == APPSMS_MAX_SMS_NUM){
            nEnd = 0;
        }

        return TRUE;
    }
}

/*****************************************************************************
 *	函数名		：APPSMS_CheckNetState
 *	功能		       ：检查网络层状态：忙、闲
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：忙或者闲
 *	其他说明	：TRUE：闲；FALSE：忙
 *****************************************************************************/
BOOLEAN APPSMS_CheckNetState(void){
    return g_bappsms_netIsBusy;
}


/*****************************************************************************
 *	函数名		：APPSMS_SetMNState
 *	功能		       ：设置MN层状态
 *	输入参数	：忙或者闲
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：TRUE：忙；FALSE：闲//suozhiling改过了
 *****************************************************************************/
void APPSMS_SetNetState(BOOLEAN state){
    g_bappsms_netIsBusy = state;
    //javacall_printf("---Thread APPSMS_SetNetState g_bappsms_netIsBusy:%d",g_bappsms_netIsBusy);
}

/*****************************************************************************
 *	函数名		：APPSMS_GetFirstSendSms
 *	功能		       ：获取第一条待发送短消息
 *	输入参数	：无
 *	输出参数	：待发送短消息
 *	返回值说明：成功或者失败
 *	其他说明	：
 *****************************************************************************/
BOOLEAN APPSMS_GetFirstSendSms(T_APPSMS_MsgForList * *sendSms, int32 *handle){
    /*队列为空*/
    if(nStart==-1){
        return FALSE;
    }else{
        *sendSms = &(g_tappsms_sendSmsList[nStart].sendSms);
        *handle = g_tappsms_sendSmsList[nStart].handle;
        return TRUE;
    }
}


/*****************************************************************************
 *	函数名		：APPSMS_DelSendSmsByHandle
 *	功能		       ：通过消息句柄删除待发送消息链表中的消息
 *	输入参数	：短消息句柄
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：
 *****************************************************************************/
void APPSMS_DelSendSmsByHandle(int32 handle){
     while(nStart != -1 && g_tappsms_sendSmsList[nStart].handle == handle){
         APPSMS_DelFirstSendSms();
     }
}

/*****************************************************************************
 *	函数名		：APPSMS_DelFirstSendSms
 *	功能		       ：删除待发送短消息链表中的第一条短消息
 *	输入参数	：无
 *	输出参数	：无
 *	返回值说明：无
 *	其他说明	：
 *****************************************************************************/
void APPSMS_DelFirstSendSms(){
    if(nStart != -1){
        nStart++;
        if(nStart == APPSMS_MAX_SMS_NUM){
            nStart=0;
        }
        if(nStart == nEnd){
            nStart = -1;
        }
    }
    return;
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


