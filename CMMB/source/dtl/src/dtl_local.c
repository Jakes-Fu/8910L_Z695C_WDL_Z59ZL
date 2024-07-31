/******************************************************************************
 ** File Name:      dtl.c                                                     *
 ** Author:         Hanbing.Zhang                                             *
 ** DATE:           1/11/2007                                                 *
 ** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation of data transfer    *
 **                 layer                                                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME                 DESCRIPTION                           *
 ** 1/11/2007      Hanbing.Zhang        Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "cmmb_trc.h"
#include "dtl.h"
#include "os_api.h"
#include "tb_hal.h"
#include "oscfg.h"
#include "demod.h"
#include "cmmbdmx_comm.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
        
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
//#define DEBUG_DTL

//#ifdef DEBUG_DTL
#ifndef SCREEN_TEST
#define DTL_PRINT   SCI_TRACE_LOW
#else
#define DTL_PRINT  
#endif


//命令优先级掩码[15-12]优先级为 F-0 为优先级从高到低
#define DTL_CMD_PRI_MASK 0XF000

#define DTL_DELAY_TIME   10 //ms

/**************************************************************************/
//DTL Service task

//service task消息
typedef struct dtl_svc_sig_tag
{
    SIGNAL_VARS
    DTL_RESPQ_CMD_T resp_cmd;
}DTL_SVC_SIG_T;

//定义DTL_SERVICE处理的MSG
typedef enum dtl_svc_msg_tag
{
    //DTL_MSG_START    = (P_DTLSVC << 8) | 1,  //可以从0开始
    DTL_MSG_DATA,
    DTL_MSG_EXIT,
    DTL_MSG_MAX
}DTL_SVC_MSG_E;

//dtl service send msg num
#define DTL_SVC_MSG_NUM  30 //command num//(VDTL_MSG_MAX & 0xff)

/**************************************************************************/

/**************************************************************************/
//DTL DATA TASK

//定义DTL_DATA处理的MSG
typedef enum dtl_data_msg_e_tag
{
    //L_DATA_MSG_START    = (P_DTLDATA << 8) | 1,
    DTL_DATA_POST_CMD,
    DTL_DATA_POST_DATA,
    DTL_DATA_FETCH_DATA,
    DTL_DATA_MSG_MAX
}DTL_DATA_MSG_E;

//数据传输任务消息附加参数
typedef struct dtl_data_msg_tag
{
    uint8*                data_ptr;
    uint32                data_len;
    uint32                data_addr;
    DTL_DATA_CALLBACK     callback_func;
}DTL_DATA_MSG_T;

//dtl data task msg
typedef struct dtl_data_sig_tag
{
    SIGNAL_VARS
    DTL_DATA_MSG_T data_msg;
}DTL_DATA_SIG_T;


/**************************************************************************/
//DTL CMD TASK simulate int,direct call int handle that is seine cmd handle

//dtl cmd task id
LOCAL BLOCK_ID g_dtl_cmd_task_id = 0;

// stack and queue define
#define   P_DTLCMD_STACK_SIZE      4000
/* Modified queue size to 30. Ref ms232456 to see detail.
*/
#define   P_DTLCMD_QUEUE_NUM       30//10
#define   P_DTLCMD_TASK_PRIORITY   20

//dtl data task id
LOCAL BLOCK_ID s_dtl_cmd_task_id = 0;
#define P_DTLCMD g_dtl_cmd_task_id

//定义DTL_CMD处理的MSG
typedef enum dtl_cmd_msg_e_tag
{
    //DTL_CMD_MSG_START    = (P_DTLDCMD << 8) | 1,
    DTL_CMD_DIRECT,
    DTL_CMD_EXIT,
    DTL_CMD_MSG_MAX
}DTL_CMD_MSG_E;

//dtl data task msg
typedef struct dtl_cmd_sig_tag
{
    SIGNAL_VARS
    DTL_CMDQ_CMD_T dir_cmd;
}DTL_CMD_SIG_T;

LOCAL BOOLEAN s_cmd_task_is_active = FALSE;

/**************************************************************************/
//需要访问的response queue addr, (queue size,in, out,data,)
#define DTL_CMDQ_SIZE             CMD_BB_CMDQ_SIZE
#define DTL_CMDQ_DATA_START_ADDR  CMD_BB_ARM_CMDQ_BASE
#define DTL_CMDQ_IN_ADDR          (CMD_BB_ARM_CMDQ_BASE+(sizeof(CMDQ_BB_ARM_CMD_T)*CMD_BB_CMDQ_SIZE))
#define DTL_CMDQ_OUT_ADDR         (DTL_CMDQ_IN_ADDR + sizeof(uint32))


//需要访问的response queue addr, (queue size,in, out,data,)
#define DTL_RESPQ_SIZE             CMD_BB_RESPQ_SIZE
#define DTL_RESPQ_DATA_START_ADDR  (DTL_CMDQ_OUT_ADDR+sizeof(uint32))
#define DTL_RESPQ_IN_ADDR          (DTL_RESPQ_DATA_START_ADDR+(sizeof(CMDQ_BB_ARM_RESP_T)*CMD_BB_RESPQ_SIZE))
#define DTL_RESPQ_OUT_ADDR         (DTL_RESPQ_IN_ADDR+sizeof(uint32))

//锁
LOCAL SCI_MUTEX_PTR s_dtl_mutex_ptr = PNULL;

//当数据量大时，分批读写
#define DTL_DATA_BLOCK_SIZE 10240 //10K

#define DTL_LCM_AFTER_DESHAKING_TIME 20

//command module map (because the service only support register uint8 event)
#define DTL_CMDQ_MODULE_NUM 10
typedef struct cmdq_module_map_tag
{
    uint8  cmd_moudule_id;
    uint32 module;
}DTL_CMDQ_MODULE_MAP_T;

LOCAL DTL_CMDQ_MODULE_MAP_T s_dtl_cmd_module[DTL_CMDQ_MODULE_NUM] = 
{
    {DTL_CMDQ_DSP_COMMON,     ARM_CMDQ_DSP_COMMON_BASE_ID},
    {DTL_CMDQ_DSP_AUDIO,      ARM_CMDQ_DSP_AUDIO_BASE_ID},
    {DTL_CMDQ_DSP_DEMUX,      ARM_CMDQ_DSP_DEMUX_BASE_ID},
    {DTL_CMDQ_DSP_DEMODULATE, ARM_CMDQ_DSP_DEMODULATE_BASE_ID},
    {DTL_CMDQ_ARM,            ARM_CMDQ_ARM_BASE_ID},
    {DTL_CMDQ_LCDC,           ARM_CMDQ_LCDC_BASE_ID},
    {DTL_CMDQ_VIDEO,          ARM_CMDQ_VIDEO_BASE_ID},
    {DTL_CMDQ_DSP_RF_DVT,     ARM_CMDQ_DSP_RF_DVT_BASE_ID},
    {DTL_CMDQ_ARM_SCREEN_TEST,ARM_CMDQ_SCREEN_TEST_BASE_ID},
    //{DTL_CMDQ_CMD_MTVAPI,     CMD_MTVAPI_BASE_ID},
//    {DTL_CMDQ_ACK_MTVAPI,     ACK_MTVAPI_BASE_ID},
    {DTL_CMDQ_ARM_SIM,	      ARM_CMDQ_SIM_BASE_ID}

};


//reg&unreg service msg
#define DTL_SVC_MSG_REG_MAX_NUM  128
typedef struct dtl_svc_msg_reg_tag
{
    //uint8       start_event;
    //uint8       end_event;
    uint32        event;
    REG_CALLBACK  reg_func_ptr;
    uint32        virtual_task_id;
}DTL_SVC_MSG_REG_T;

LOCAL DTL_SVC_MSG_REG_T s_dtl_svc_msg_reg[DTL_SVC_MSG_REG_MAX_NUM] = {0};
#define DTL_MAX_RECV_RESP_CMD_NUM 20
LOCAL DTL_RESPQ_CMD_T s_dtl_resp_cmd_que[DTL_MAX_RECV_RESP_CMD_NUM];



uint8 share_memory_array[SHARED_MEMORY_SIZE]={0};

#define MAX_SERVICE_RUN_NUM   2
typedef struct service_tag
{
    uint32 service_idx;
    uint32 service_handle;
    uint32 subframe_idx;
}DEMOD_SERVICE_T;

LOCAL DEMOD_SERVICE_T s_service[MAX_SERVICE_RUN_NUM];

#define DTL_SALT_KEY_LEN 8

LOCAL uint8 s_vid_salt_key[DTL_SALT_KEY_LEN] = {0};
LOCAL uint8 s_aud_salt_key[DTL_SALT_KEY_LEN] = {0};

LOCAL DEMOD_DESCARMBLE_T s_descramble_key[MAX_DESCRAMBLE_KEY_NUM] = {0};

/**---------------------------------------------------------------------------*
 **                      Function Declaration                                 *
 **---------------------------------------------------------------------------*/

/************************************************************************
* \fn  LOCAL void DTL_SortCmd(
*                      DTL_RESPQ_CMD_T *resp_ptr,
*                      uint16 cmd_num
*                      )
* \param  resp_ptr     respond command pointer
* \param  cmd_num      command num
* \return command pointer 
* \brief  sort command
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void DTL_SortCmd(
                       DTL_RESPQ_CMD_T *resp_ptr,
                       uint16 cmd_num
                       );


/************************************************************************
* \fn  LOCAL void DTLCMD_Task(
*                       uint32 argc,
*                       void * argv
*                       )
* \param  void
* \return void 
* \brief  dtl cmd task
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void DTLCMD_Task(
                        uint32 argc,
                        void * argv
                        );

/************************************************************************
* \fn  LOCAL uint8 DTL_Get_ModuleID(
*                            uint32 cmd_id
*                            )
* \param  cmd_id      command ID
* \return command module ID 
* \brief  Get map module ID of the command 
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL uint8 DTL_Get_ModuleID(
                             uint32 cmd_id
                             );

/************************************************************************
* \fn  LOCAL void DTL_InitRegMsg(
*                             uint32 dyn_task_id
*                             )
* \param dyn_task_id
* \return void 
* \brief  init DTL_SERVICE reg&unreg msg 
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void DTL_InitRegMsg(
                          uint32 dyn_task_id
                          );

/************************************************************************
* \fn  LOCAL void DTL_SendDirectCMDMsg(
*                               DTL_CMDQ_CMD_T *cmd_to_send
*                               )
* \param  cmd_to_send
* \return void 
* \brief  dtl direct send cmd to cmd handle task for simulate int
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void DTL_SendDirectCMDMsg(
                                DTL_CMDQ_CMD_T *cmd_to_send
                                );

/************************************************************************
* \fn  LOCAL uint8 DTL_Get_ModuleID(
*                            uint32 cmd_id
*                            )
* \param  cmd_id      command ID
* \return command module ID 
* \brief  Get map module ID of the command 
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL uint8 DTL_Get_ModuleID(
                             uint32 cmd_id
                             )
{
    uint8 i = 0;
    uint8 module_id = 0;
    uint32 mask_module = 0xF0F00;

    if ( cmd_id < s_dtl_cmd_module[1].module )
    {
        module_id = s_dtl_cmd_module[0].cmd_moudule_id;
    }
    else
    {        
        for ( i = 1; i < DTL_CMDQ_MODULE_NUM; i ++ )
        {
            if ( s_dtl_cmd_module[i].module == (mask_module & cmd_id) )
            {
                module_id = s_dtl_cmd_module[i].cmd_moudule_id;
                break;
            }
        }
    }    
    return module_id;
}

/************************************************************************
* \fn  LOCAL void DTL_SvcMsgHandler( 
*                            BLOCK_ID id, 
*                            uint32   argc,
*                            void     *argv 
*                            )
* \param  id
* \param  argc
* \param  argv
* \return void 
* \brief  service default callback function
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void DTL_SvcMsgHandler( 
                             BLOCK_ID  id,
                             uint32    argc,
                             void      *argv 
                             )
{
    //do nothing
}


#define INT_SWAP1(data) _INT_SWAP(data, &(data))

#ifdef MTV_MIDDLEWARE_ON_BB
LOCAL int Demod_CallBack(unsigned int ack_id, unsigned int para0, unsigned int para1, unsigned int para2)
{
    DTL_RESPQ_CMD_T RespQ;
    //SCI_TRACE_LOW:"Demod_CallBack ack_id=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_LOCAL_361_112_2_17_23_9_5_25,(uint8*)"d",ack_id);
    RespQ.resp_id = ack_id;
    RespQ.parameter0 = para0;
    RespQ.parameter1 = para1;
    RespQ.parameter2 = para2;    
    SCI_SendEventToClient(DTL_SERVICE, (uint32)(DTL_Get_ModuleID(ack_id)),(void*)&RespQ);
    return 1;
}

PUBLIC void Demod_dmx_callback(uint8* argv)
{
    CMDQ_DSP_ARM_RESP_T* resp_ptr = (CMDQ_DSP_ARM_RESP_T*)argv;
    DTL_RESPQ_CMD_T cmd = {0};

    //SCI_TRACE_LOW:"Demod_dmx_callback resp_ptr->resp_id=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_LOCAL_375_112_2_17_23_9_5_26,(uint8*)"d",resp_ptr->resp_id);
/*lint -save -e506 -e774 */     
    INT_SWAP1(resp_ptr->resp_id);
    INT_SWAP1(resp_ptr->parameter0);
    INT_SWAP1(resp_ptr->parameter1);
    INT_SWAP1(resp_ptr->parameter2);
    INT_SWAP1(resp_ptr->parameter3);           
/*lint -restore */
    
    cmd.resp_id = resp_ptr->resp_id;
    cmd.parameter0 = (resp_ptr->parameter0<<16)|(resp_ptr->parameter1);
    cmd.parameter1 = (resp_ptr->parameter2<<16)|(resp_ptr->parameter3);
    cmd.parameter2 = 0;
    
    SCI_SendEventToClient(DTL_SERVICE, (uint32)(DTL_Get_ModuleID(cmd.resp_id)),(void*)&cmd);
}
#endif


#if 0
LOCAL void Demod_CallBack(uint32 ack_id, uint32 para0, uint32 para1, uint32 para2)
{
    switch (ack_id)
    {
        case BB_CMDQ_SIGNAL_STATUS:
            {
                DTL_RESPQ_CMD_T cmd = {0};
                cmd.resp_id = BB_CMDQ_SIGNAL_STATUS;
                cmd.parameter0 = para0;
                ProcessStimiSetTunerRespCmd(cmd);
            }
            break;
        case BB_CMDQ_DATA_READY:
            cmmbdmx(para0,(uint8*)para1,para2);
            break;
        case BB_CMDQ_DISABLE_CHANNEL_RESP:
            ProcessStimiDisableChannelCmd(ack_id,para0,para1,para2);;
            break;
        default:
            break;
    }
}
#endif


LOCAL void DTL_InitServiceIdx(void)
{
    SCI_MEMSET(&s_service,0,sizeof(DEMOD_SERVICE_T)*MAX_SERVICE_RUN_NUM);
}

PUBLIC uint32 DTL_GetServiceHandle(uint32 service_idx)
{
    uint8 i = 0;
    for ( i = 0; i < MAX_SERVICE_RUN_NUM;i++)
    {
        if ( s_service[i].service_idx == service_idx )
        {
            return s_service[i].service_handle;
        }
    }
    return 0;
}

PUBLIC uint32 DTL_GetServiceSubframeIdx(uint32 service_idx)
{
    uint8 i = 0;
    for ( i = 0; i < MAX_SERVICE_RUN_NUM;i++)
    {
        if ( s_service[i].service_idx == service_idx )
        {
            return s_service[i].subframe_idx;
        }
    }
    return 0;
}

/************************************************************************
* \fn  PUBLIC void DTL_Initialize(void)
* \param  void
* \return void 
* \brief  initialize the data transfer layer
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC void DTL_Initialize(void)
{

    //Create service
    SCI_CreateClientList( DTL_SERVICE, DTL_SVC_MSG_NUM,
                            (REG_CALLBACK)DTL_SvcMsgHandler);

    //创建锁
    if ( s_dtl_mutex_ptr )
    {
        SCI_ASSERT(SCI_SUCCESS == SCI_DeleteMutex(s_dtl_mutex_ptr)); /*assert verified*/ 
        s_dtl_mutex_ptr = PNULL;
    }
    s_dtl_mutex_ptr = SCI_CreateMutex("dtl_mutex", SCI_INHERIT);
    SCI_ASSERT(PNULL != s_dtl_mutex_ptr); /*assert verified*/ 
    //init dtl_cmd task
    g_dtl_cmd_task_id = SCI_CreateThread( 
                            "T_P_DTLCMD", 
                            "Q_P_DTLCMD", 
                            DTLCMD_Task, 
                            0, 
                            0, 
                            P_DTLCMD_STACK_SIZE, 
                            P_DTLCMD_QUEUE_NUM, 
                            P_DTLCMD_TASK_PRIORITY, 
                            SCI_PREEMPT, 
                            SCI_AUTO_START);
    SCI_ASSERT(SCI_INVALID_BLOCK_ID != g_dtl_cmd_task_id); /*assert verified*/ 
    s_cmd_task_is_active = TRUE;

    Demod_Init(PNULL,Demod_CallBack);

    DTL_InitServiceIdx();
       
}

/************************************************************************
* \fn  PUBLIC void DTL_Close(void)
* \param  void
* \return void 
* \brief  finish the data transfer layer
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC void DTL_Close(void)
{    
    DTL_CMD_SIG_T  *cmd_sig_ptr = PNULL;
        
    //delete service
    SCI_DeleteClientList(DTL_SERVICE);
/*    
    //释放互斥量
    if ( s_dtl_mutex_ptr )
    {
        SCI_ASSERT(SCI_SUCCESS == SCI_DeleteMutex(s_dtl_mutex_ptr));
        s_dtl_mutex_ptr = PNULL;
    }
*/
    
    //delete dtl direct send cmd task
    if ( g_dtl_cmd_task_id )
    {
        s_cmd_task_is_active = FALSE;
        
        cmd_sig_ptr = (DTL_CMD_SIG_T *)SCI_ALLOC(sizeof(DTL_CMD_SIG_T));
        SCI_ASSERT(SCI_NULL != cmd_sig_ptr); /*assert verified*/ 
        
        cmd_sig_ptr->SignalSize = sizeof(DTL_CMD_SIG_T);
        cmd_sig_ptr->Sender     = SCI_IdentifyThread();
        cmd_sig_ptr->SignalCode = DTL_CMD_EXIT;
        SCI_SendSignal((xSignalHeader)cmd_sig_ptr, P_DTLCMD);
        
        //g_dtl_cmd_task_id = 0;
    }

}

//temp 
/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_SendData(	
*                          uint8 *data_to_send,
*                          uint32 data_len,
*                          uint32 dest_addr
*                          )
* \param  *data_to_send  data pointer
* \param  data_len       data length
* \param  dest_addr      dest address
* \return BOOLEAN 
* \brief  send data
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC BOOLEAN DTL_SendData(
                            uint8 *data_to_send,
                            uint32 data_len,
                            uint32 dest_addr
                            )
{
    SCI_ASSERT(dest_addr != 0); /*assert verified*/ 

    SCI_MEMCPY((void*)dest_addr, (void*)data_to_send, data_len);    
     
    return TRUE;
}

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_GetData(
*                  uint8 *data_buf,
*                  uint32 data_len,
*                  uint32 src_addr
*                  )
* \param  *data_buf      data pointer
* \param  data_len       data length
* \param  src_addr       dest address
* \return BOOLEAN 
* \brief  get data synchronous
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC BOOLEAN DTL_GetData(
                   uint8 *data_buf,
                   uint32 data_len,
                   uint32 src_addr
                   )
{
     SCI_MEMCPY(data_buf, (uint8*)src_addr, data_len);
    
     return TRUE;
}

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_SendCMD(
*                          DTL_CMDQ_CMD_T *cmd_to_send
*                          )
* \param  cmd_to_send  command to send
* \return TRUE success; FALSE fail 
* \brief  send command synchronous
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC BOOLEAN DTL_SendCMD(
                           DTL_CMDQ_CMD_T *cmd_to_send
                           )
{    

    //获取锁 进入临界区
    SCI_ASSERT(SCI_SUCCESS == SCI_GetMutex (s_dtl_mutex_ptr, SCI_WAIT_FOREVER)); /*assert verified*/ 
    
    DTL_SendDirectCMDMsg(cmd_to_send);

    //释放锁 出临界区
    SCI_ASSERT(SCI_SUCCESS == SCI_PutMutex(s_dtl_mutex_ptr)); /*assert verified*/ 
    
    return TRUE;
    
}

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_SendCMD_NOINT(
*                                 DTL_CMDQ_CMD_T *cmd_to_send
*                                 )
* \param  cmd_to_send  command to send
* \return TRUE success; FALSE fail 
* \brief  send command synchronous
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC BOOLEAN DTL_SendCMD_NOINT(
                                 DTL_CMDQ_CMD_T *cmd_to_send
                                 )
{
    //获取锁 进入临界区
    SCI_ASSERT(SCI_SUCCESS == SCI_GetMutex (s_dtl_mutex_ptr, SCI_WAIT_FOREVER)); /*assert verified*/ 
    
    DTL_SendDirectCMDMsg(cmd_to_send);

    //释放锁 出临界区
    SCI_ASSERT(SCI_SUCCESS == SCI_PutMutex(s_dtl_mutex_ptr)); /*assert verified*/ 
    
    return TRUE;
    
}

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_SendCMD_And_MsgParam(
*                                       uint8*          msg_data_ptr,
*                                       uint32          msg_data_len,
*                                       DTL_CMDQ_CMD_T* cmd_to_send
*                                       )
* \param  msg_data_ptr command message data which is large and stored in SRAM
* \param  msg_data_len command message data length
* \param  cmd_to_send  command to send
* \return TRUE success; FALSE fail 
* \brief  send command and command message data synchronous
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC BOOLEAN DTL_SendCMD_And_MsgParam(
                                        uint8*          msg_data_ptr,
                                        uint32          msg_data_len,
                                        DTL_CMDQ_CMD_T* cmd_to_send
                                        )
{
    //获取锁 进入临界区
    SCI_ASSERT(SCI_SUCCESS == SCI_GetMutex (s_dtl_mutex_ptr, SCI_WAIT_FOREVER)); /*assert verified*/ 
    
    DTL_SendDirectCMDMsg(cmd_to_send);

    //释放锁 出临界区
    SCI_ASSERT(SCI_SUCCESS == SCI_PutMutex(s_dtl_mutex_ptr)); /*assert verified*/ 
    
    return TRUE;
}

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_SendCMD_NOMutex(
*                                 DTL_CMDQ_CMD_T *cmd_to_send
*                                 )
* \param  cmd_to_send  command to send
* \return TRUE success; FALSE fail 
* \brief  send command synchronous
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC BOOLEAN DTL_SendCMD_NOMutex(
                                 DTL_CMDQ_CMD_T *cmd_to_send
                                 )
{

    DTL_SendDirectCMDMsg(cmd_to_send);

    return TRUE;
    
}

/************************************************************************
* \fn  PUBLIC DTL_RESPQ_CMD_T* DTL_GetCmd(
*                                 uint16 *cmd_num_ptr
*                                 )
* \param  cmd_num_ptr  command num
* \return command pointer 
* \brief  get all commands
*
* \author Hanbing.Zhang
* \note   
************************************************************************/
PUBLIC DTL_RESPQ_CMD_T* DTL_GetCmd(
                                   uint16 *cmd_num_ptr
                                   )
{

    uint32 total_cmd_num = 0;
    
    
    //获取锁 进入临界区
    SCI_ASSERT(SCI_SUCCESS == SCI_GetMutex (s_dtl_mutex_ptr, SCI_WAIT_FOREVER)); /*assert verified*/ 
        
    //释放锁 出临界区
    SCI_ASSERT(SCI_SUCCESS == SCI_PutMutex(s_dtl_mutex_ptr)); /*assert verified*/ 
    
    *cmd_num_ptr = total_cmd_num;
    return (DTL_RESPQ_CMD_T*)s_dtl_resp_cmd_que;
    
}

/************************************************************************
* \fn  LOCAL void DTL_SortCmd(
*                      DTL_RESPQ_CMD_T *resp_ptr,
*                      uint16 cmd_num
*                      )
* \param  resp_ptr     respond command pointer
* \param  cmd_num      command num
* \return command pointer 
* \brief  sort command
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void DTL_SortCmd(
                       DTL_RESPQ_CMD_T *resp_ptr,
                       uint16 cmd_num
                       )
{
    DTL_RESPQ_CMD_T resp_cmd = {0};
    uint16 i = 0;
    uint16 j = 0;

    SCI_ASSERT( PNULL != resp_ptr ); /*assert verified*/ 

    for ( i = 1; i < cmd_num; i ++ )
    {
        for ( j = cmd_num-1;j>=i; j --)
        {
            if ( (resp_ptr[j].resp_id & DTL_CMD_PRI_MASK) 
                > (resp_ptr[j-1].resp_id & DTL_CMD_PRI_MASK) )
            {
                resp_cmd = resp_ptr[j];
                resp_ptr[j] = resp_ptr[j-1];
                resp_ptr[j-1] = resp_cmd;
            }
        }
    }
}

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_IsErrorSerious(void)
* \param void
* \return BOOLEAN 
* \brief  is error seirous
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC BOOLEAN DTL_IsErrorSerious(void)
{
    return 0;
}

/************************************************************************
* \fn  LOCAL void DTL_InitRegMsg(
*                             uint32 dyn_task_id
*                             )
* \param dyn_task_id
* \return void 
* \brief  init DTL_SERVICE reg&unreg msg 
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void DTL_InitRegMsg(
                          uint32 dyn_task_id
                          )
{
    uint16 i = 0;
    uint32 vid = dyn_task_id - 0x1000 + 1; //0x1000 == SCI_MIN_DYNAMIC_THREAD_ID os_apiext.h
    
    for ( i = 0; i< DTL_SVC_MSG_REG_MAX_NUM; i ++ )
    {
        s_dtl_svc_msg_reg[i].reg_func_ptr = PNULL;
        s_dtl_svc_msg_reg[i].virtual_task_id = vid + i;
    }
}

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_RegisterMsg(
*                           uint32 service_id,
*                           uint8  start_event, // Start number of event
*                           uint8  end_event,   // End number of event
*                           REG_CALLBACK  fun   // Callback function user defined
*                           )
* \param service_id
* \param start_event
* \param end_event
* \param func callback function pointer
* \return BOOLEAN 
* \brief  reg DTL_SERVICE msg 
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC BOOLEAN DTL_RegisterMsg(
                               uint32 service_id,
                               uint8  start_event,
                               uint8  end_event,
                               REG_CALLBACK  fun
                               )
{
    uint16 i = 0;
    uint16 j = 0;
    
    SCI_ASSERT(start_event<=end_event);/*assert to do*/
    SCI_ASSERT(PNULL != fun); /*assert verified*/ 
    
    for ( j = start_event;j<=end_event;j++)
    {
        for ( i = 0;i < DTL_SVC_MSG_REG_MAX_NUM; i ++ )
        {
            if ( fun == s_dtl_svc_msg_reg[i].reg_func_ptr
                && j == s_dtl_svc_msg_reg[i].event )
            {
                SCI_PASSERT(0,("DTL_RegisterMsg duplicate"));/*assert verified*/
            }
            
            if ( PNULL == s_dtl_svc_msg_reg[i].reg_func_ptr )
            {
                s_dtl_svc_msg_reg[i].event    = j;
                s_dtl_svc_msg_reg[i].reg_func_ptr = fun;
                
                //SCI_TRACE_LOW:"DTL_RegisterMsg,event=%d,i=%d,task_id=0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_LOCAL_840_112_2_17_23_9_6_27,(uint8*)"ddd",j,i,s_dtl_svc_msg_reg[i].virtual_task_id);
                
                SCI_RegisterTaskMsg(service_id,start_event,end_event,
                        s_dtl_svc_msg_reg[i].virtual_task_id,fun );
                
                break;
            }
        }
        
        if ( DTL_SVC_MSG_REG_MAX_NUM == i )
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_UnregisterMsg(
*                           uint32 service_id,
*                           uint8  start_event, // Start number of event
*                           uint8  end_event,   // End number of event
*                           REG_CALLBACK  fun          // Callback function user defined
*                           )
* \param service_id
* \param start_event
* \param end_event
* \param func callback function pointer
* \return BOOLEAN 
* \brief  unreg DTL_SERVICE msg 
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC BOOLEAN DTL_UnregisterMsg(
                                 uint32 service_id,
                                 uint8  start_event,
                                 uint8  end_event,
                                 REG_CALLBACK  fun
                                 )
{
    uint16 i = 0;
    uint16 j = 0;
    
    SCI_ASSERT(start_event<=end_event); /*assert to do*/
    SCI_ASSERT(PNULL != fun); /*assert verified*/ 
    
    for ( j = start_event;j<=end_event;j++ )
    {
        for ( i = 0;i < DTL_SVC_MSG_REG_MAX_NUM; i ++ )
        {
            if ( fun == s_dtl_svc_msg_reg[i].reg_func_ptr
                && j == s_dtl_svc_msg_reg[i].event )
            {
                s_dtl_svc_msg_reg[i].reg_func_ptr = PNULL;
                
                //SCI_TRACE_LOW:"DTL_UnregisterMsg,event=%d,i=%d,task_id=0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_LOCAL_897_112_2_17_23_9_6_28,(uint8*)"ddd",j,i,s_dtl_svc_msg_reg[i].virtual_task_id);
                
                SCI_UnregisterTaskMsg(service_id,start_event,end_event,
                        s_dtl_svc_msg_reg[i].virtual_task_id,fun );
                
                break;
            }
        }
        
        if ( DTL_SVC_MSG_REG_MAX_NUM == i )
        {
            return FALSE;
        }
    }
    
    return TRUE;
}


/************************************************************************
* \fn  LOCAL void DTL_SendDirectCMDMsg(
*                               DTL_CMDQ_CMD_T *cmd_to_send
*                               )
* \param  cmd_to_send
* \return void 
* \brief  dtl direct send cmd to cmd handle task for simulate int
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void DTL_SendDirectCMDMsg(
                                DTL_CMDQ_CMD_T *cmd_to_send
                                )
{
    DTL_CMD_SIG_T* data_sig_ptr = PNULL;

    if (s_cmd_task_is_active)
    {
        data_sig_ptr = (DTL_CMD_SIG_T *)SCI_ALLOC(sizeof(DTL_CMD_SIG_T));
        SCI_ASSERT(SCI_NULL != data_sig_ptr); /*assert verified*/ 
        
        data_sig_ptr->SignalSize = sizeof(DTL_CMD_SIG_T);
        data_sig_ptr->Sender     = SCI_IdentifyThread();
        data_sig_ptr->SignalCode = DTL_CMD_DIRECT;
        data_sig_ptr->dir_cmd = *cmd_to_send;
        SCI_SendSignal((xSignalHeader)data_sig_ptr, P_DTLCMD);
    }
}

/************************************************************************
* \fn  LOCAL void DTLCMD_Task(
*                       uint32 argc,
*                       void * argv
*                       )
* \param  void
* \return void 
* \brief  dtl cmd task
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void DTLCMD_Task(
                       uint32 argc,
                       void * argv
                       )
{
    DTL_CMDQ_CMD_T cmd;
    DTL_CMD_SIG_T *data_sig_ptr = PNULL;

    BOOLEAN is_cmd_task_exist = TRUE;
    BLOCK_ID dtl_task_id = SCI_IdentifyThread();
    BOOLEAN is_finish = FALSE;    

    DTL_InitRegMsg(dtl_task_id);
    
    while(is_cmd_task_exist)
    {
        //从消息队列中获取MSG
        data_sig_ptr = (DTL_CMD_SIG_T*)SCI_GetSignal(dtl_task_id);
        cmd = data_sig_ptr->dir_cmd;
        switch(data_sig_ptr->SignalCode)
        {
        case DTL_CMD_DIRECT: 
            //CMDQ_BB_DirectCMD((CMDQ_BB_ARM_CMD_T*)&(data_sig_ptr->dir_cmd));
            switch (cmd.cmd_id )
            {
                case DSP_CMDQ_SET_TUNNER:
                        Demod_SetTuner(PNULL, cmd.parameter0, cmd.parameter1);
                    break;
                case DSP_CMDQ_ENABLE_CHANNEL:
                case DSP_CMDQ_CHANNEL_2_ENABLE_CHANNEL:
                        //SCI_TRACE_LOW:"enable channel %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_LOCAL_986_112_2_17_23_9_7_29,(uint8*)"d",cmd.parameter0);
                        DTL_UpdateServiceIdx(cmd.parameter0,TRUE);
                        Demod_EnableChannel(PNULL, cmd.parameter0,cmd.parameter1,cmd.parameter2);
                    break;
                case DSP_CMDQ_DISABLE_CHANNEL:
                case DSP_CMDQ_CHANNEL_2_DISABLE_CHANNEL:
                        //SCI_TRACE_LOW:"disable channel %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_LOCAL_992_112_2_17_23_9_7_30,(uint8*)"d",cmd.parameter0);
                        Demod_DisableChannel(PNULL, DTL_GetServiceHandle(cmd.parameter0));                    
                        DTL_UpdateServiceIdx(cmd.parameter0,FALSE);
                    break;
                case DSP_CMDQ_TS0_ENABLE_CHANNEL:
                        Demod_EnableChannel(PNULL, cmd.parameter0,cmd.parameter1,cmd.parameter2);
                    break;
                case DSP_CMDQ_TS0_DISABLE_CHANNEL:
                        Demod_DisableChannel(PNULL, cmd.parameter0);   
                    break;
                case DSP_CMDQ_SET_DESCRAM_KEY:
                	  //SCI_TRACE_LOW:"SET DESCRAM KEY"
                	  SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_LOCAL_1003_112_2_17_23_9_7_31,(uint8*)"");
                    DTL_SetDescramKey(cmd.parameter1,(uint8*)cmd.parameter2);
                    Demod_SetCaCtrlWords(PNULL,cmd.parameter0,cmd.parameter1,(uint8*)cmd.parameter2);
                    break;
                case DSP_CMDQ_SET_ISMA_RTP_PARAM:
                	  //SCI_TRACE_LOW:"SET ISMA"
                	  SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_LOCAL_1008_112_2_17_23_9_7_32,(uint8*)"");
                    Demod_SetCaSaltKey(PNULL,cmd.parameter0,(uint8*)cmd.parameter1,(uint8*)cmd.parameter2);
                    break;
                default:
                    break;
            }
            break;
        case DTL_MSG_EXIT:
            is_cmd_task_exist = FALSE;
            break;
        default:
            //SCI_TRACE_LOW("DTLCMD_Task()%p", data_sig_ptr->SignalCode);
            break;
        }
        SCI_FREE(data_sig_ptr);
    }
    
    // Empty the signal queue.
    while (!is_finish)
    {
        data_sig_ptr = (void*)SCI_PeekSignal(g_dtl_cmd_task_id);
        if (data_sig_ptr)
        {
            SCI_FREE(data_sig_ptr);
        }
        else
        {
            is_finish = TRUE;
        }
    }

    Demod_Exit(PNULL);
    
    SCI_ThreadExit();
}
    
PUBLIC void DTL_UpdateServiceIdx(uint32 service_idx,BOOLEAN is_start)
{
    uint8 i = 0;

    if ( is_start )
    {
        for ( i = 0; i < MAX_SERVICE_RUN_NUM;i++)
        {
            if ( 0 == s_service[i].service_idx )
            {
                s_service[i].service_idx = service_idx;
                break;
            }
        }
    }
    else 
    {
        for ( i = 0; i < MAX_SERVICE_RUN_NUM;i++)
        {
            if ( service_idx == s_service[i].service_idx )
            {
                s_service[i].service_idx = 0;
                s_service[i].service_handle = 0;
                s_service[i].subframe_idx = 0;
                break;
            }
        }        
    }

}
    
PUBLIC void DTL_UpdateServiceHandle(uint32 service_handle,uint32 subframe_idx)
{
    uint8 i = 0;
    for ( i = 0; i < MAX_SERVICE_RUN_NUM;i++)
    {
        if ( s_service[i].service_idx != 0 )
        {
            s_service[i].service_handle = service_handle;
            s_service[i].subframe_idx = subframe_idx;            
            break;
        }
    }
}

PUBLIC void DTL_SetSaltKey(uint8* vid_salt,uint8* aud_salt)
{
    SCI_MEMCPY(s_vid_salt_key,vid_salt, DTL_SALT_KEY_LEN);
    SCI_MEMCPY(s_aud_salt_key,aud_salt, DTL_SALT_KEY_LEN);
}

PUBLIC void DTL_GetSaltKey(uint8** vid_salt,uint8** aud_salt)
{
    *vid_salt = s_vid_salt_key;
    *aud_salt = s_aud_salt_key;
}

PUBLIC void DTL_SetDescramKey(uint16 key_id,uint8* cw)
{
    uint8 i = 0;

    //first,0-255
    if ( 0xffff == s_descramble_key[0].key_id 
        || ((abs(key_id - s_descramble_key[0].key_id ) >2) && s_descramble_key[0].key_id != 255))
    {
        for ( i = 0; i < MAX_DESCRAMBLE_KEY_NUM; i ++ )
        {
            s_descramble_key[i].key_id = key_id;
            SCI_MEMCPY(s_descramble_key[i].key,cw,MAX_DESCRAMBLE_KEY_LEN);
        }
    }
    else
    {
        if ( key_id %2 )
        {            
            s_descramble_key[0].key_id = key_id;
            SCI_MEMCPY(s_descramble_key[0].key,cw,MAX_DESCRAMBLE_KEY_LEN);
        }
        else
        {
            s_descramble_key[1].key_id = key_id;
            SCI_MEMCPY(s_descramble_key[1].key,cw,MAX_DESCRAMBLE_KEY_LEN);            
        }
    }
    
}

PUBLIC DEMOD_DESCARMBLE_T* DTL_GetDescramKey(void)
{
    return (DEMOD_DESCARMBLE_T*)s_descramble_key;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
