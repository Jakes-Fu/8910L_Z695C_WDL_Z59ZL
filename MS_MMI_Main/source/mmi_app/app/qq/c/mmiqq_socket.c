/****************************************************************************
** File Name:      mmiqq_socket.c(Qsocket.c)                                                *
** Author:                                                                 *
** Date:           05/11/2010                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the socket  of QQ             *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2010       fen.xie         Create
** 
****************************************************************************/
#include "mmi_app_qq_trc.h"
#ifdef QQ_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "qsystem.h"
#include "datatype.h"
#include "os_api.h"
#include "qsocket.h"
//#include "sig_code.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

#ifdef WIN32
#include <winsock.h>
#else
#include "app_tcp_if.h"
#include "mmiphone_export.h"
#include "mmk_timer.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  	QQ_GPRS_RECONNECT_MAX	 				5//the max times of reconnect for activing gprs 


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
enum
{
    QQ_GPRS_STATUS_UNCONNECT = 0,
    QQ_GPRS_STATUS_CONNECTING,
    QQ_GPRS_STATUS_CONNECTED,
    //QQ_GPRS_STATUS_BLOCK,
    QQ_GPRS_STATUS_MAX
};

enum _SOCKET_STATE
{
    ISOCKET_NOTCONNECTED = 0,
    ISOCKET_CONNETCTING,
    ISOCKET_INPROGRESS,
    ISOCKET_EXCEPTION,
    ISOCKET_MAX
}SOCKET_STATE;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL unsigned int s_qq_net_id = 0;//pdp并发记录qq模块的 net id

#ifdef WIN32
LOCAL fd_set s_fd_read;
LOCAL fd_set s_fd_write;
LOCAL fd_set s_fd_exception;
#else
LOCAL sci_fd_set s_fd_read;
LOCAL sci_fd_set s_fd_write;
LOCAL sci_fd_set s_fd_exception;
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
//extern void* s_udpconn;
//extern int	g_qq_sim_select;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 //用于QQ 连接的socket 链表
Sockets  GSockets = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleQQPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);
/*****************************************************************************/
//  Description: ActiveGprsCallBack
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  ActiveGprsCallBack(uint16 msgid);
/*****************************************************************************/
//  Discription: This function get net setting index
//  Global resource dependence: None
//  Author: fen.xie
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL uint8 GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: QSOCKET_Init --初始化GSockets
//  Global resource dependence : GSockets 
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QSOCKET_Init(void)
{
    //SCI_TRACE_LOW:"[QQ]:QSOCKET_Init! **** "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_114_112_2_18_2_49_9_14,(uint8*)"");
    SCI_MEMSET(&GSockets, 0, sizeof(Sockets));
    GSockets.head = PNULL;
    GSockets.size = 0;
    GSockets.gprs_status = QQ_GPRS_STATUS_UNCONNECT;
}

/*****************************************************************************/
//  Description: QSOCKET_Free--是否GSockets的链表头
//  Global resource dependence :  GSockets
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QSOCKET_Free(void)
{
    ISocket* socket_ptr = PNULL;
   
    //SCI_TRACE_LOW:"[QQ]:QSOCKET_Free! GSockets.size = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_129_112_2_18_2_49_9_15,(uint8*)"d", GSockets.size);
    if ((GSockets.size != 0) && (GSockets.head != PNULL))
    {
        socket_ptr = GSockets.head;
        ISOCKET_Release(socket_ptr);
        GSockets.head = PNULL;
    }
}

/*****************************************************************************/
//  Description: QSOCKET_Insert--在GSockets插入一个socket
//  Global resource dependence :  GSockets
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QSOCKET_Insert(ISocket* socket_ptr)
{
    ISocket* temp_socket_ptr = PNULL;

    //SCI_TRACE_LOW:"[QQ]:QSOCKET_Insert! **** "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_146_112_2_18_2_49_9_16,(uint8*)"");
    if(PNULL == socket_ptr)
    {
        return;
    }
    
    if(PNULL == GSockets.head)
    {
        GSockets.size++;
        GSockets.head = socket_ptr;
        return;
    }
    
    temp_socket_ptr = GSockets.head;
    while(temp_socket_ptr->pNext != PNULL)
    {
        temp_socket_ptr = temp_socket_ptr->pNext;
    }
    temp_socket_ptr->pNext = socket_ptr;
    GSockets.size++;
}

/*****************************************************************************/
//  Description: QSocket_Remove--从GSockets 链表中移除一个socket
//  Global resource dependence :  GSockets
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QSocket_Remove(ISocket* socket_ptr)
{
    ISocket* temp_socket_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[QQ]:QSocket_Remove! **** "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_176_112_2_18_2_49_9_17,(uint8*)"");
    if ((PNULL == socket_ptr) || (PNULL == GSockets.head) || (0 == GSockets.size))
    {
        return;
    }
    
    temp_socket_ptr = GSockets.head;
    if(temp_socket_ptr->socketid == socket_ptr->socketid)
    {
        GSockets.head = socket_ptr->pNext;
        GSockets.size--;
        return;
    }
    
    while(temp_socket_ptr->pNext != PNULL)
    {
        if(temp_socket_ptr->pNext->socketid == socket_ptr->socketid)//删除指定的socket 节点
        {	
            temp_socket_ptr->pNext = socket_ptr->pNext;
            GSockets.size--;
            break;
        }
        temp_socket_ptr = temp_socket_ptr->pNext;
    }
    
}

/*****************************************************************************/
//  Description: ISOCKET_Writeable--设置socket 的WriteCB
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void ISOCKET_Writeable(ISocket* me_ptr, WriteCB func_ptr, void* user_ptr)
{
    //SCI_TRACE_LOW("[QQ]:ISOCKET_Writeable! **** ");
    if((me_ptr != PNULL) && (func_ptr != PNULL) && (user_ptr != PNULL))
    {
        me_ptr->pfnwrite = func_ptr;
        me_ptr->pUser = user_ptr;
    }
}

/*****************************************************************************/
//  Description: ISOCKET_Readable--设置socket 的ReadCB
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void ISOCKET_Readable(ISocket* me_ptr, ReadCB func_ptr, void* user_ptr)
{
    //SCI_TRACE_LOW("[QQ]:ISOCKET_Readable! **** ");
    if((me_ptr != PNULL) && (func_ptr != PNULL) && (user_ptr != PNULL))
    {
        me_ptr->pfnRead = func_ptr;
        me_ptr->pUser = user_ptr;
    }
}

/*****************************************************************************/
//  Description : QQ拨号链接状态是否已准备好
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
BOOLEAN MMIQQ_IsGprsStatusReady(void)
{
    if (QQ_GPRS_STATUS_UNCONNECT == GSockets.gprs_status)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifndef WIN32
//@fen.xie begin
/*****************************************************************************/
//  Discription: This function get net setting index
//  Global resource dependence: None
//  Author: fen.xie
//  Note : 
/*****************************************************************************/
LOCAL uint8 GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    uint8 index = 0;

    //待有统一get setting 的接口后再修正
#ifdef BROWSER_SUPPORT
    index = MMIAPIBROWSER_GetNetSettingIndex(dual_sys);               
#endif

    return index;
}

/*****************************************************************************/
//  Description : active pdp for QQ 
//  Global resource dependence : none
//  Author: fen.xie
//  Note:QSOCKET_PDPActive
/*****************************************************************************/
PUBLIC void QSOCKET_ActiveGprs(void* user_ptr)
{
    BOOLEAN return_val = FALSE;
    uint8 setting_index = 0;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;
    MN_DUAL_SYS_E sim_select = MMIQQ_GetSimSelect();
        
    //SCI_TRACE_LOW:"[QQ]:QSOCKET_ActiveGprs GSockets.gprs_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_277_112_2_18_2_49_9_18,(uint8*)"d", GSockets.gprs_status);
    //SCI_TRACE_LOW:"[QQ] QSOCKET_ActiveGprs sim_select = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_278_112_2_18_2_49_9_19,(uint8*)"d", sim_select);

    //已经连接成功
    if((QQ_GPRS_STATUS_CONNECTED == GSockets.gprs_status) || (QQ_GPRS_STATUS_CONNECTING == GSockets.gprs_status))
    {
        return;
    }
 
    //SCI_TRACE_LOW("[QQ]:QSOCKET_ActiveGprs TickCount=%d",SCI_GetTickCount());
    if (!MMIQQ_GetIsUseWifi())
    {
        setting_index = GetNetSettingIndex(sim_select);    
        setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_select, setting_index);
        
        if(setting_item_ptr != PNULL)
        {        
            app_info.app_handler = MMI_MODULE_QQ;
            app_info.auth_type = setting_item_ptr->auth_type;
            app_info.apn_ptr = (char*)setting_item_ptr->apn;
            app_info.user_name_ptr = (char*)setting_item_ptr->username;
            app_info.psw_ptr = (char*)setting_item_ptr->password;
            app_info.dual_sys = sim_select;
            app_info.priority = 3;
            app_info.ps_service_rat = MN_UNSPECIFIED;
            app_info.handle_msg_callback = HandleQQPdpMsg;
            app_info.ps_service_type = IM_E;//BROWSER_E
            app_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
            app_info.ip_type   = setting_item_ptr->ip_type;
#endif
            GSockets.gprs_status = QQ_GPRS_STATUS_CONNECTING;

            return_val = MMIAPIPDP_Active(&app_info);
            //SCI_TRACE_LOW:"[QQ]: QSOCKET_ActiveGprs() active pdp context apn = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_308_112_2_18_2_49_9_20,(uint8*)"s",  setting_item_ptr->apn);
        }
            
    }
#ifdef WIFI_SUPPORT
    else
    {
        app_info.app_handler = MMI_MODULE_QQ;
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
        app_info.handle_msg_callback = HandleQQPdpMsg;   
        return_val = MMIAPIPDP_Active(&app_info);
        GSockets.gprs_status = QQ_GPRS_STATUS_CONNECTING;

        //SCI_TRACE_LOW:"[QQ]: QSOCKET_ActiveGprs() use WIFI!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_321_112_2_18_2_49_9_21,(uint8*)"");
    }
#endif    

        if(return_val)//设置激活gprs的超时时间为10s
        {        
#ifndef WIFI_SUPPORT//wifi情况下,不需要设置该timeout
            QQTimer_AddTask((void*)0x2222FFFF, QSOCKET_ActiveGprsTimeOut, 10000, 0);
#endif
        }
        else/*这种情况下不再重试*/
        {
            //使用QQ的timeout方式较好
            //ActiveGprsCallBack(QQ_GPRS_STATUS_UNCONNECT);
            QQTimer_AddTask((void*)0x2222FFFF, QSOCKET_ActiveGprsTimeOut, 500, 0);
        }


    //SCI_TRACE_LOW:"[QQ]: QSOCKET_ActiveGprs() active pdp return_val = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_339_112_2_18_2_49_9_22,(uint8*)"d",  return_val);

//    return return_val;
}
/*****************************************************************************/
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void HandleQQPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    //SCI_TRACE_LOW:"[QQ]:HandleQQPdpMsg msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_353_112_2_18_2_49_9_23,(uint8*)"d",msg_ptr->msg_id);
    //SCI_TRACE_LOW:"[QQ]:HandleQQPdpMsg result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_354_112_2_18_2_49_9_24,(uint8*)"d",msg_ptr->result);
    //SCI_TRACE_LOW:"[QQ]:HandleQQPdpMsg GSockets.gprs_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_355_112_2_18_2_49_9_25,(uint8*)"d", GSockets.gprs_status);
    //SCI_TRACE_LOW:"[QQ]:HandleQQPdpMsg ps_interface=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_356_112_2_18_2_49_9_26,(uint8*)"d", msg_ptr->ps_interface);

    switch (msg_ptr->ps_interface)//protect 避免:gprs和wifi的消息错误交互
    {
    case MMIPDP_INTERFACE_WIFI:
        if (!MMIQQ_GetIsUseWifi())
        {
            return;
        }
        break;
        
    case MMIPDP_INTERFACE_GPRS:
        if (MMIQQ_GetIsUseWifi())
        {
            return;
        }
        break;
        
    default:
        return;
    }

    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            GSockets.retry_cnt = 0;
            s_qq_net_id= msg_ptr->nsapi; 
            ActiveGprsCallBack(QQ_GPRS_STATUS_CONNECTED);
            //SCI_TRACE_LOW:"[QQ]:HandleQQPdpMsg Active GPRS OK! **** "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_386_112_2_18_2_49_9_27,(uint8*)"");
        }
        else/*pdp支持重试,此处无需重试*/
        {
            GSockets.retry_cnt = 0;
            ActiveGprsCallBack(QQ_GPRS_STATUS_UNCONNECT);
        }
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        ActiveGprsCallBack(QQ_GPRS_STATUS_UNCONNECT);
        break;
        
    case MMIPDP_DEACTIVE_IND:
        ActiveGprsCallBack(QQ_GPRS_STATUS_UNCONNECT);
        //SCI_TRACE_LOW:"[QQ]:HandleQQPdpMsg  MMIPDP_DEACTIVE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_401_112_2_18_2_49_9_28,(uint8*)"");
        break;
        
    default:
        break;
    }
    //SCI_FREE(msg_ptr);
}

//@fen.xie end

/*****************************************************************************/
//  Description: QSOCKET_ConnectCB--进行socket 连接
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QSOCKET_ConnectCB(void* user_ptr)
{
    struct sci_sockaddr  addr = {0};
    int  status = AEE_NET_SUCCESS;
    ISocket* socket_ptr = PNULL;
    
    //SCI_TRACE_LOW("[QQ]:QSOCKET_ConnectCB ");
    
    if (PNULL == user_ptr)
    {
        return;
    }

    socket_ptr = (ISocket*)user_ptr;
    if(ISOCKET_CONNETCTING == socket_ptr->iState)
    {
        return;
    }

    SCI_MEMSET(&addr, 0, sizeof(struct sci_sockaddr));

    addr.family = AF_INET;
    addr.ip_addr = socket_ptr->ip;
    addr.port = socket_ptr->port;

    status = sci_sock_connect(socket_ptr->socketid, (struct sci_sockaddr*)&addr, sizeof(struct sci_sockaddr));

    //当error为INPROGRESS是,2s后轮询再尝试连接
    if((AEE_NET_SUCCESS != status) && (EINPROGRESS == sci_sock_errno(socket_ptr->socketid)))
    {
        QQTimer_AddTask(socket_ptr, QSOCKET_ConnectCB, 2000, 0);	
    }
    
    //SCI_TRACE_LOW:"[QQ]:QSOCKET_ConnectCB status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_449_112_2_18_2_49_9_29,(uint8*)"d", status);
 
}

/*****************************************************************************/
//  Description: QSOCKET_ConnectTimeOut--处理socket的连接超时
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QSOCKET_ConnectTimeOut(void* user_ptr)
{
    ISocket* socket_ptr = PNULL;

    //SCI_TRACE_LOW("[QQ]:QSOCKET_ConnectTimeOut");

    if (PNULL == user_ptr)
    {
        return;
    }

    socket_ptr = (ISocket*)user_ptr;
    QQTimer_CancelTask(socket_ptr, QSOCKET_ConnectCB);
    socket_ptr->pfnconnect(socket_ptr->pUser, 1);
}

/*****************************************************************************/
//  Description: ActiveGprsCallBack--处理gprs激活的结果
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
LOCAL void  ActiveGprsCallBack(uint16 msgid)
{
    ISocket* socket_ptr = PNULL;
    ISocket* temp_socket_ptr = PNULL;

    //SCI_TRACE_LOW:"[QQ]:ActiveGprsCallBack msgid=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_482_112_2_18_2_49_9_30,(uint8*)"d", msgid);

    switch (msgid)
    {	
    case QQ_GPRS_STATUS_CONNECTED:
        {
            //开始进行所有sockets 的连接
            GSockets.gprs_status = QQ_GPRS_STATUS_CONNECTED;
            socket_ptr = GSockets.head;
            while(socket_ptr != PNULL)
            {
                QSOCKET_ConnectCB(socket_ptr);
                //设置每个socket的连接超时时间为10s
                QQTimer_AddTask(socket_ptr, QSOCKET_ConnectTimeOut, 10000, 0);
                socket_ptr = socket_ptr->pNext;
            }

        }
        break;
        
    case QQ_GPRS_STATUS_UNCONNECT:
        {
            //释放所有的sockets
            GSockets.gprs_status = QQ_GPRS_STATUS_UNCONNECT;
            if(GSockets.size > 0)
            {
                socket_ptr = GSockets.head;
                while(socket_ptr != PNULL)/*释放后,size是否应该同步递减?*/
                {
                    temp_socket_ptr = socket_ptr->pNext;
                    socket_ptr->iState = ISOCKET_NOTCONNECTED;
                    socket_ptr->pfnconnect(socket_ptr->pUser, 2);
                    socket_ptr = temp_socket_ptr;
                }
            }
        }
        break;
        
    default:
        break;
    }

    QQTimer_CancelTask((void*)0x2222FFFF, QSOCKET_ActiveGprsTimeOut);
    QQTimer_CancelTask((void*)0x1111FFFF, QSOCKET_ActiveGprs);
}

/*****************************************************************************/
//  Description : deactive pdp for qq 
//  Global resource dependence : none
//  Author: fen.xie
//  Note:QSOCKET_PDPDeactive
/*****************************************************************************/
PUBLIC BOOLEAN QSOCKET_DeActiveGprs(void)
{
    BOOLEAN return_val = FALSE;
    //SCI_TRACE_LOW("[QQ]:QSOCKET_DeActiveGprs TickCount=%d",SCI_GetTickCount());

    //是否需要增加对GSockets.gprs_status的判断?
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_QQ);
    s_qq_net_id = 0;

    if (MMIQQ_GetIsUseWifi() || !return_val)//确保gprs状态的正确;wifi的deactive未发出MMIPDP_DEACTIVE_CNF消息
    {
        ActiveGprsCallBack(QQ_GPRS_STATUS_UNCONNECT);
    }

    //SCI_TRACE_LOW:"[QQ]:QSOCKET_DeActiveGprs return_val=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_546_112_2_18_2_49_10_31,(uint8*)"d",return_val);
    
    return return_val;
}

/*****************************************************************************/
//  Description: QSOCKET_ActiveGprsTimeOut--激活gprs 失败
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QSOCKET_ActiveGprsTimeOut(void* user_ptr)
{
    //SCI_TRACE_LOW("[QQ]:QSOCKET_ActiveGprsTimeOut");

    ActiveGprsCallBack(QQ_GPRS_STATUS_UNCONNECT);	
}

/*****************************************************************************/
//  Description: QSCOKET_OnTimer
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QSCOKET_OnTimer(Sockets* head_sockets_ptr)
{
    ISocket* socket_ptr = PNULL;
    ISocket* temp_socket_ptr = PNULL;

    //SCI_TRACE_LOW("[QQ]:QSCOKET_OnTimer");
    if(QQ_GPRS_STATUS_CONNECTED != GSockets.gprs_status)
    {
        return;
    }

    if(0 == head_sockets_ptr->size)
    {
        return;
    }

    socket_ptr = head_sockets_ptr->head;

    if(PNULL == socket_ptr)
    {
        return;
    }

    SCI_FD_ZERO(&s_fd_read);
    SCI_FD_ZERO(&s_fd_write);
    SCI_FD_ZERO(&s_fd_exception);

    while(socket_ptr != PNULL)
    {
        SCI_FD_SET(socket_ptr->socketid, &s_fd_read);
        SCI_FD_SET(socket_ptr->socketid, &s_fd_write);
        SCI_FD_SET(socket_ptr->socketid, &s_fd_exception);
        socket_ptr = socket_ptr->pNext;
    }
	
    socket_ptr = head_sockets_ptr->head;

    if(sci_sock_select(&s_fd_read, &s_fd_write, &s_fd_exception, 0))
    {	
        while(socket_ptr != PNULL)
        {
            temp_socket_ptr = socket_ptr->pNext;
            if(SCI_FD_ISSET(socket_ptr->socketid, &s_fd_read))
            {
                if(ISOCKET_CONNETCTING == socket_ptr->iState)
                {
                    socket_ptr->pfnRead(socket_ptr->pUser);
                }
            }
            else if(SCI_FD_ISSET(socket_ptr->socketid, &s_fd_write))
            {
                if(ISOCKET_CONNETCTING == socket_ptr->iState)
                {
                    socket_ptr->pfnwrite(socket_ptr->pUser);
                }
                else
                {
                    QQTimer_CancelTask(socket_ptr, QSOCKET_ConnectTimeOut);
                    QQTimer_CancelTask(socket_ptr, QSOCKET_ConnectCB);
                    socket_ptr->iState = ISOCKET_CONNETCTING;
                    socket_ptr->pfnconnect(socket_ptr->pUser, 0);
                }

            }
            else if(SCI_FD_ISSET(socket_ptr->socketid, &s_fd_exception))
            {
                socket_ptr->iState = ISOCKET_NOTCONNECTED;
                socket_ptr->pfnconnect(socket_ptr->pUser, 1);
            }
            socket_ptr = temp_socket_ptr;
        }
    }
}

/*****************************************************************************/
//  Description: INETMGR_OpenSocket--打开一个socket
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC ISocket* INETMGR_OpenSocket(INetMgr* net_mgr_ptr, int stream)
{
    ISocket* socket_ptr = PNULL;
    int error = 0;

    //SCI_TRACE_LOW:"[QQ]:INETMGR_OpenSocket s_qq_net_id =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_649_112_2_18_2_49_10_32,(uint8*)"d", s_qq_net_id);
    //SCI_TRACE_LOW:"[QQ]:INETMGR_OpenSocket GSockets.gprs_status =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_650_112_2_18_2_49_10_33,(uint8*)"d", GSockets.gprs_status);
    
    if ((QQ_GPRS_STATUS_CONNECTED != GSockets.gprs_status) && (QQ_GPRS_STATUS_CONNECTING != GSockets.gprs_status))
    {
        //gprs尚未active,先active gprs
        QSOCKET_ActiveGprs(NULL);
        return PNULL;
    }

    socket_ptr = (ISocket*)QMalloc(sizeof(ISocket));
    if(PNULL == socket_ptr)
    {
        return PNULL;
    }
    SCI_MEMSET(socket_ptr, 0, sizeof(ISocket));

    socket_ptr->socketid = sci_sock_socket(AF_INET, SOCK_STREAM, 0, s_qq_net_id);

    //SCI_TRACE_LOW:"QSOCKET_Create() socketid=0x%08x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_668_112_2_18_2_49_10_34,(uint8*)"d", socket_ptr->socketid);

    if(socket_ptr->socketid  < 0)
    {
        QFree(socket_ptr);  /*lint !e774*/
        return PNULL;
    }
    
    error = sci_sock_setsockopt(socket_ptr->socketid, SO_NBIO, PNULL);//设置socket 为非阻塞模式
    if(error != 0)//失败
    {
        error = sci_sock_errno(socket_ptr->socketid);
        //SCI_TRACE_LOW:"setnbio error ***************%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_680_112_2_18_2_49_10_35,(uint8*)"d", error);
        QFree(socket_ptr);   /*lint !e774*/
        return PNULL;
    }
    
    QSOCKET_Insert(socket_ptr);//成功则将新建的socket插入到sockets链表中	
    return socket_ptr;
}

/*****************************************************************************/
//  Description: ISOCKET_Connect--开始连接socket
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void ISOCKET_Connect(ISocket* me_ptr, INAddr ip, unsigned short port, ConnectCB func, void* user_ptr)
{
    //SCI_TRACE_LOW:"[QQ]:ISOCKET_Connect "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_695_112_2_18_2_49_10_36,(uint8*)"");
    
    if((me_ptr != PNULL) && (func != PNULL) && (user_ptr != PNULL))
    {
        me_ptr->ip = CHANGEIP(ip);
        me_ptr->port = AEE_htons(port);

        me_ptr->pfnconnect = func;
        me_ptr->pUser = user_ptr;

        if ((QQ_GPRS_STATUS_CONNECTED == GSockets.gprs_status) || (QQ_GPRS_STATUS_CONNECTING == GSockets.gprs_status))
        {
            QSOCKET_ConnectCB(me_ptr);	
            //设置socket连接 10s 超时
            QQTimer_AddTask(me_ptr, QSOCKET_ConnectTimeOut, 10000, 0);
        }
        else//gprs尚未active,先active gprs
        {
            QSOCKET_ActiveGprs(NULL);
        }
    }
}

/*****************************************************************************/
//  Description: ISOCKET_Read--从socket 接收数据
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int ISOCKET_Read(ISocket* me_ptr, char* buf_ptr, int size)
{
    int error = 0;
    int len = 0; 

    if((PNULL == me_ptr) || (0 == me_ptr->socketid))
    {
        return 0;
    }

    len = sci_sock_recv(me_ptr->socketid, buf_ptr, size, 0);
    //SCI_TRACE_LOW:"QSOCKET_Read: len = *********************%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_733_112_2_18_2_49_10_37,(uint8*)"d", len);
    if(len == -1)
    {
        error = sci_sock_errno(me_ptr->socketid);
        //SCI_TRACE_LOW:"QSOCKET_Read: error = *********************%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_737_112_2_18_2_49_10_38,(uint8*)"d", error);
        if(EWOULDBLOCK == error)
        {
        	return AEE_NET_WOULDBLOCK;
        }
    }
    
    return len;
}

/*****************************************************************************/
//  Description: ISOCKET_Write--向socket 传送数据
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int  ISOCKET_Write(ISocket* me_ptr, char* buf_ptr, int size)
{
    int error = 0;
    int len = 0;

    if (PNULL == me_ptr)
    {
        return 0;
    }

    len = sci_sock_send(me_ptr->socketid, buf_ptr, size, 0);
    //SCI_TRACE_LOW:"QSOCKET_Write: len = **************%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_762_112_2_18_2_49_10_39,(uint8*)"d", len);
    if(len < size)
    {
        //SCI_TRACE_LOW:"QSOCKET_Write: error = **************%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_765_112_2_18_2_49_10_40,(uint8*)"d", error);
        error = sci_sock_errno(me_ptr->socketid);
        if(EWOULDBLOCK == error)
        {
            return AEE_NET_WOULDBLOCK;
        }
    }
    return len;
}

/*****************************************************************************/
//  Description: ISOCKET_Release--释放某个socket
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void ISOCKET_Release(ISocket* me_ptr)
{
    //SCI_TRACE_LOW:"[QQ]:ISOCKET_Release "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQQ_SOCKET_781_112_2_18_2_49_10_41,(uint8*)"");

    if (PNULL == me_ptr)
    {
        return;
    }

    if(me_ptr->socketid > 0)
    {
        sci_sock_socketclose(me_ptr->socketid);
    }

    QQTimer_CancelTask(me_ptr, QSOCKET_ConnectCB);
    QQTimer_CancelTask(me_ptr, QSOCKET_ConnectTimeOut);

    QSocket_Remove(me_ptr);
    QFree(me_ptr);

    return;
}

#else

/*****************************************************************************/
//  Description: QSCOKET_OnTimer
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QSCOKET_OnTimer(Sockets* head_sockets_ptr)
{
    ISocket* socket_ptr = PNULL;
    ISocket* temp_socket_ptr = PNULL;
    struct timeval timeout = {0};	
    timeout.tv_sec = 0;
    timeout.tv_usec = 200;

    if(PNULL == head_sockets_ptr)
    {
        return;
    }
    
    if(0 == head_sockets_ptr->size)
    {
        return;
    }

    FD_ZERO(&s_fd_read);
    FD_ZERO(&s_fd_write);
    FD_ZERO(&s_fd_exception);
    socket_ptr = head_sockets_ptr->head;
    while(socket_ptr != PNULL)
    {
        FD_SET(socket_ptr->socketid, &s_fd_read);
        FD_SET(socket_ptr->socketid, &s_fd_write);
        FD_SET(socket_ptr->socketid, &s_fd_exception);
        socket_ptr= socket_ptr->pNext;
    }
	
    if(select(0, &s_fd_read, &s_fd_write, &s_fd_exception, &timeout))
    {
        socket_ptr = head_sockets_ptr->head;
        while(socket_ptr != PNULL)
        {
            temp_socket_ptr = socket_ptr->pNext;
            if(FD_ISSET(socket_ptr->socketid, &s_fd_read))
            {
                if(socket_ptr->iState == ISOCKET_CONNETCTING)
                {
                    socket_ptr->pfnRead(socket_ptr->pUser);
                }
            }
            else if(FD_ISSET(socket_ptr->socketid, &s_fd_write))
            {
                if(socket_ptr->iState == ISOCKET_NOTCONNECTED)
                {
                    socket_ptr->iState = ISOCKET_CONNETCTING;
                    socket_ptr->pfnconnect(socket_ptr->pUser, 0);
                }
                
                if(socket_ptr->iState == ISOCKET_CONNETCTING)
                {
                    if(socket_ptr->pfnwrite != 0)
                    {
                        socket_ptr->pfnwrite(socket_ptr->pUser);
                    }
                }
            }
            else if(FD_ISSET(socket_ptr->socketid, &s_fd_exception))
            {
                if(socket_ptr->iState != ISOCKET_EXCEPTION)
                {
                    socket_ptr->iState = ISOCKET_EXCEPTION;
                    socket_ptr->pfnconnect(socket_ptr->pUser, 1);
                }
            }
            socket_ptr = temp_socket_ptr;
        }
    }
}

/*****************************************************************************/
//  Description: INETMGR_OpenSocket
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC ISocket* INETMGR_OpenSocket(INetMgr* net_mgr_ptr, int stream)
{
    ISocket* socket_ptr = PNULL;
    unsigned long arg = 0;

    socket_ptr = (ISocket*)QMalloc(sizeof(ISocket));
    if(PNULL == socket_ptr)
    {
        return PNULL;
    }
    memset(socket_ptr,0,sizeof(ISocket));

    socket_ptr->socketid = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_ptr->socketid < 0)
    {
        QFree(socket_ptr);
        return PNULL;
    }
    QSOCKET_Insert(socket_ptr);
    //设置为非阻
    arg = 1;
    ioctlsocket(socket_ptr->socketid, FIONBIO, &arg);
    return socket_ptr;

}

/*****************************************************************************/
//  Description: HandleUnConnect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleUnConnect(void* user_ptr)
{
    ISocket* me_ptr = PNULL;
    if (PNULL == user_ptr)
    {
        return;
    }
    me_ptr = (ISocket*)user_ptr;
    me_ptr->pfnconnect(me_ptr->pUser, 1);
}

/*****************************************************************************/
//  Description: ISOCKET_Connect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void ISOCKET_Connect(ISocket* me_ptr, INAddr ip, unsigned short port, ConnectCB func, void* user_ptr)
{
    struct sockaddr_in  destSockAddr = {0};
    int status = 0;
    
    if((me_ptr != PNULL) && (func != PNULL) && (user_ptr != PNULL))
    {
        memcpy(&destSockAddr.sin_addr, &ip, 4);
        destSockAddr.sin_port = port;
        destSockAddr.sin_family = AF_INET;
        memset(destSockAddr.sin_zero,0,sizeof(destSockAddr.sin_zero));

        status = connect(me_ptr->socketid, (struct sockaddr *)&destSockAddr, sizeof(destSockAddr));

        me_ptr->pfnconnect = func;
        me_ptr->pUser = user_ptr;

        QQTimer_AddTask(me_ptr, HandleUnConnect, 15000, 0);
    }
}

/*****************************************************************************/
//  Description: ISOCKET_Read
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int ISOCKET_Read(ISocket* me_ptr, char* buf_ptr, int size)
{
    int status = 0;
    
    if((PNULL == me_ptr) || (0 == me_ptr->socketid))
    {
        return 0;
    }

    status = recv(me_ptr->socketid, buf_ptr, size, 0);
    
    if(SOCKET_ERROR == status)
    {
        status = WSAGetLastError();
        if(WSAEWOULDBLOCK == status)
        {
            return AEE_NET_WOULDBLOCK;
        }
    }
    return status;
}


/*****************************************************************************/
//  Description: ISOCKET_Write
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int  ISOCKET_Write(ISocket* me_ptr, char* buf_ptr, int size)
{
    int status = 0;
    
    if (PNULL == me_ptr)
    {
        return 0;
    }

    status = send(me_ptr->socketid, buf_ptr, size, 0);
    
    if(SOCKET_ERROR == status)
    {
        status = WSAGetLastError();
        if(WSAEWOULDBLOCK == status)
        {
            return AEE_NET_WOULDBLOCK;
        }
    }
    return status;
}


/*****************************************************************************/
//  Description: ISOCKET_Release
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void ISOCKET_Release(ISocket* me_ptr)
{
    int result = 0;
    
    if (PNULL == me_ptr)
    {
        return;
    }
    result = closesocket(me_ptr->socketid);
    QSocket_Remove(me_ptr);
    QFree(me_ptr);
    
    /*
    if(result == 0)
        return AEE_NET_SUCCESS;
    else
        return AEE_NET_ERROR;
    */
}

/*****************************************************************************/
//  Description: QSOCKET_UDP_Close
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QSOCKET_UDP_Close()
{

}
#endif
#endif




