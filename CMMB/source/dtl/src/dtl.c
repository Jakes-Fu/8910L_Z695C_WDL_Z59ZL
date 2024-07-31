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
#include "user_tasks_id.h"
#include "oscfg.h"
#include "lcmif_drv.h"
#include "lcd.h"
//#include "boot.h"
#include "lcd_tv.h"
#include "gpio_prod_api.h"
#include "ref_outport.h"
#include "mtv_cfg.h"
#include "dtl_cmdq.h"
//#include "sc_reg.h"
#include "kd.h"
#include "mtv_api_cmd.h"
#include "mtv_ctrl.h"
#include "dtl_export.h"
#include "mtv_boot.h"
#include "mtv_api.h"
#include "mtv_setting.h"

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
#define DTL_PRINT   KD_TRACE_LOW
#else
#define DTL_PRINT  KD_TRACE_LOW
#endif

#define USE_OPENKODE  1

//命令优先级掩码[15-12]优先级为 F-0 为优先级从高到低
#define DTL_CMD_PRI_MASK 0XF000

#define DTL_DELAY_TIME   10 //ms

/**************************************************************************/
//DTL Service task

#ifdef USE_OPENKODE
KDThread* g_dtl_svc_task_id = KD_NULL;
KDThreadSem* s_dtl_svc_thread_sem_ptr = KD_NULL;
//when v rw ap odd data,delete the sem,because it takes dead lock
//del sem,If useing dtl_senddata send odd data and useing dtl_getdata soon, it can't sure read it right.
//beacause the system don't know the odd data write handle finished.
//KDThreadSem* s_dtl_send_odd_sem_ptr = KD_NULL;  
//BLOCK_ID g_dtl_svc_task_id = 0;
#else
//dtl service task id
BLOCK_ID g_dtl_svc_task_id = 0;
#endif

// stack and queue define
#define   P_DTLDATA_STACK_SIZE      4000
#define   P_DTLDATA_QUEUE_NUM       8
#define   P_DTLDATA_TASK_PRIORITY   29

//service task消息
typedef struct dtl_svc_sig_tag
{
    SIGNAL_VARS
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

LOCAL BOOLEAN s_svc_task_is_active = FALSE;

#ifdef USE_OPENKODE
LOCAL KDThreadAttr* s_svc_thread_attr_ptr = PNULL;
#endif

/**************************************************************************/

/**************************************************************************/
//DTL DATA TASK

// stack and queue define
#define   P_DTLSVC_STACK_SIZE      4000
#define   P_DTLSVC_QUEUE_NUM       10
#define   P_DTLSVC_TASK_PRIORITY   20

//dtl data task id
LOCAL BLOCK_ID s_dtl_data_task_id = 0;
#define P_DTLDATA s_dtl_data_task_id

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

static uint8 cmdq_respq_cmdq_size = DTL_CMDQ_RESPQ_ORGINAL_SIZE;

/**************************************************************************/
//需要访问的response queue addr, (queue size,in, out,data,)
//#define DTL_CMDQ_SIZE             CMD_BB_CMDQ_SIZE
#define DTL_CMDQ_SIZE             cmdq_respq_cmdq_size
#define DTL_CMDQ_DATA_START_ADDR  CMD_BB_ARM_CMDQ_BASE
#define DTL_CMDQ_IN_ADDR          (DTL_CMDQ_DATA_START_ADDR+(sizeof(CMDQ_BB_ARM_CMD_T)*DTL_CMDQ_SIZE))
#define DTL_CMDQ_OUT_ADDR         (DTL_CMDQ_IN_ADDR + sizeof(uint32))


//需要访问的response queue addr, (queue size,in, out,data,)
//#define DTL_RESPQ_SIZE             CMD_BB_RESPQ_SIZE
#define DTL_RESPQ_SIZE             cmdq_respq_cmdq_size
#define DTL_RESPQ_DATA_START_ADDR  (DTL_CMDQ_OUT_ADDR+sizeof(uint32))
#define DTL_RESPQ_IN_ADDR          (DTL_RESPQ_DATA_START_ADDR+(sizeof(CMDQ_BB_ARM_RESP_T)*DTL_RESPQ_SIZE))
#define DTL_RESPQ_OUT_ADDR         (DTL_RESPQ_IN_ADDR+sizeof(uint32))

LOCAL KDThreadMutex* s_dtl_mutex_ptr = PNULL;

//当数据量大时，分批读写
#define DTL_DATA_BLOCK_SIZE 10240 //10K

#define DTL_LCM_AFTER_DESHAKING_TIME 20

//command module map (because the service only support register uint8 event)
#define DTL_CMDQ_MODULE_NUM 12

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
    {DTL_CMDQ_ACK_MTVAPI,     ACK_MTVAPI_BASE_ID},
    {DTL_CMDQ_ACK_MTVAPI_APP, ACK_MTVAPI_APP_BASE_ID},
    {DTL_CMDQ_ARM_SIM,        ARM_CMDQ_SIM_BASE_ID}
};

//send int to seine gpio
LOCAL DTL_CALLBACK s_dtl_callback = PNULL;

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

LOCAL DTL_ERR_INFO_E s_dtl_error_id = DTL_ERR_MAX;
LOCAL DTL_ERR_INFO_E s_dtl_little_error_id = DTL_ERR_MAX;
LOCAL BOOLEAN    s_is_dtl_init = FALSE;

LOCAL DTL_RECOVER_CALLBACK s_dtl_recover_callback = PNULL;
LOCAL uint8 s_dtl_is_debug_mode = MTV_DEBUG_MODE;
   
#define DTL_LCM_RET_POS(ret_val,cmd_pos,err_type) \
do{\
    if(MTV_RELEASE_MODE == s_dtl_is_debug_mode){\
        if (!ret_val || cmd_pos >= DTL_RESPQ_SIZE) \
        {\
            kdThreadMutexUnlock(s_dtl_mutex_ptr);\
            DTL_SetErrorInfo(err_type);\
            s_dtl_recover_callback = DTLR_GetCallback();\
            if(s_dtl_recover_callback)\
            {\
                    s_dtl_recover_callback(DTLR_GetParam(),err_type,0,0);\
            }\
            return FALSE;\
        }\
    }\
else{\
    kdAssert(TRUE == ret_val);\
    kdAssert(cmd_pos<DTL_RESPQ_SIZE);\
    }\
}\
while(0);

#define DTL_LCM_RET(ret_val,err_type)\
do{\
    if(MTV_RELEASE_MODE == s_dtl_is_debug_mode){\
        if (!ret_val) \
       {\
        kdThreadMutexUnlock(s_dtl_mutex_ptr);;\
        DTL_SetErrorInfo(err_type);\
        s_dtl_recover_callback = DTLR_GetCallback();\
        if(s_dtl_recover_callback)\
        {\
            s_dtl_recover_callback(DTLR_GetParam(),err_type,0,0);\
        }\
        return FALSE;\
        }\
    }\
    else{\
        kdAssert(TRUE == ret_val);\
    }\
}\
while(0);

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
* \fn  LOCAL void DTLDATA_Task(
                        uint32 argc, 
                        void * argv
                        )
* \param  argc
* \param  argv
* \return void 
* \brief  asynchronous data transfer task
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void DTLDATA_Task(
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
* \fn  LOCAL void TurnoffSeineLdo(
*                          BOOLEAN is_r_poweron
*                          )
* \param is_r_poweron is rocky power on,
*  TRUE: put battery,need close seine ldo 
*  FALSE tv off,close seine ldo 
* \return void 
* \brief  power down seine some ldo
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void TurnoffSeineLdo(
                           BOOLEAN is_r_poweron
                           );

/************************************************************************
* \fn  PUBLIC void DTL_RecvInt(void)
* \param  void
* \return void
* \brief  handle dtl receive int
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC void DTL_RecvInt(void)
{
    if( s_svc_task_is_active )
    {
        kdThreadSemPost(s_dtl_svc_thread_sem_ptr);
    }
}

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
    //uint32 mask_module = 0xF0F00;
    uint32 mask_module = 0xFFF00;
    
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

/************************************************************************
* \fn  PUBLIC void DTLSVC_Init(void)
* \param  void
* \return void 
* \brief  init for create dtlsvc task 
*
* \author Hanbing.Zhang
* \note   for static task
************************************************************************/
PUBLIC void DTLSVC_Init(void)
{

    /* ALLEN: changed for TYLT temporarily
    s_dtl_svc_thread_sem_ptr = kdThreadSemCreate(0);
    kdAssert(KD_NULL != s_dtl_svc_thread_sem_ptr);
    */
    
    s_svc_thread_attr_ptr = kdThreadAttrCreate();
    /*
    s_svc_thread_attr_ptr->thread_name = KD_NULL;
    s_svc_thread_attr_ptr->queue_name = KD_NULL;
    s_svc_thread_attr_ptr->stack_size = P_DTLSVC_STACK_SIZE;
    s_svc_thread_attr_ptr->queue_num = P_DTLSVC_QUEUE_NUM;
    s_svc_thread_attr_ptr->priority = P_DTLSVC_TASK_PRIORITY;
    s_svc_thread_attr_ptr->preempt = SCI_PREEMPT;
    s_svc_thread_attr_ptr->auto_start = SCI_AUTO_START;
*/
    kdThreadAttrSetStackSize(s_svc_thread_attr_ptr,P_DTLSVC_STACK_SIZE);
    kdThreadAttrSetPriority(s_svc_thread_attr_ptr,P_DTLSVC_TASK_PRIORITY);
    s_svc_task_is_active = TRUE;
    g_dtl_svc_task_id = kdThreadCreate(s_svc_thread_attr_ptr,DTLSVC_Task,KD_NULL);
    kdAssert(SCI_INVALID_BLOCK_ID != (BLOCK_ID)g_dtl_svc_task_id);
    kdThreadAttrFree(s_svc_thread_attr_ptr);

}

/************************************************************************
* \fn  PUBLIC void DTLSVC_Close(void)
* \param  void
* \return void 
* \brief  exit dtlsvc task 
*
* \author Hanbing.Zhang
* \note   for static task
************************************************************************/
PUBLIC void DTLSVC_Close(void)
{
    void *ret_val;
    KDint status;

    //delete dtl data task
    if ( g_dtl_svc_task_id )
    {
        s_svc_task_is_active = FALSE;
        kdThreadSemPost(s_dtl_svc_thread_sem_ptr);
    }

    status = kdThreadJoin(g_dtl_svc_task_id,&ret_val);
    kdAssert(0 == status);
    g_dtl_svc_task_id = KD_NULL;
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
     
    KDThread* svc_thread_ptr = kdThreadSelf();
    s_dtl_is_debug_mode = MTV_GetAssertMode();
    if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
    {
        DTL_SetErrorInfo(DTL_ERR_MAX);
        s_dtl_recover_callback = DTLR_GetCallback();
    }


    DTL_SetCmdQSize(DTL_CMDQ_RESPQ_ORGINAL_SIZE);

    MTVCFG_Open_26MClock();
    
    //tv on 
    GPIO_MTVReset(TRUE);
    GPIO_MTVStandby(FALSE);
    kdSleep(DTL_DELAY_TIME);
    GPIO_MTVLCDBypass(FALSE);

    kdSleep(DTL_DELAY_TIME);
    GPIO_MTVReset(FALSE);
    
    kdSleep(DTL_DELAY_TIME);
    
    //lcmif初始化
    LCMIF_Init(FALSE);

    //Create service
    SCI_CreateClientList( DTL_SERVICE, DTL_SVC_MSG_NUM,
                            (REG_CALLBACK)DTL_SvcMsgHandler);


    MTVCtrlOpen_Dummy();

    DTL_InitRegMsg((uint32)svc_thread_ptr);

    s_is_dtl_init = TRUE;
    
    //spi open
    //SPITV_Open();
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
    MTVCtrlClose_Dummy();

    DTLCMDQ_Turn_Off_Seine_POWER();
    kdMsDelay(DTL_DELAY_TIME);

    //lcm close
    LCMIF_Close();
    //spi close
    //SPITV_Close();
    
    //delete service
    SCI_DeleteClientList(DTL_SERVICE);

    //power down seine
    GPIO_MTVLCDBypass(TRUE);
    kdSleep(DTL_DELAY_TIME);
    GPIO_MTVStandby(TRUE);
    
    MTVCFG_Close_26MClock();

    //temp
    MTVBOOT_SetOffFlagForAssert();
#ifdef LCD_CONNECTED_TO_V
    LCDTV_SetLCDNeedReset(FALSE);
#endif

    s_is_dtl_init = FALSE;
}

/************************************************************************
* \fn  LOCAL void DTL_DirectRWData(
*                           DTL_RESPQ_CMD_T *RespQ_ptr
*                           )
* \param  DTL_RESPQ_CMD_T *RespQ_ptr 
* \return void 
* \brief  direct handle v read and write ap data
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void DTL_DirectRWData(
                            DTL_RESPQ_CMD_T *RespQ_ptr
                            )
{
    KDboolean is_ok = KD_FALSE;
    kdAssert(RespQ_ptr!= KD_NULL);
    
    if ( RespQ_ptr->resp_id == BB_ARM_STATUS_READ_DATA )
    {
        is_ok = DTL_SendData((uint8*)RespQ_ptr->parameter0,RespQ_ptr->parameter1,RespQ_ptr->parameter2);
        DTLCMDQ_Read_Data_ACK(is_ok);
    }
    else if ( RespQ_ptr->resp_id == BB_ARM_STATUS_WRITE_DATA)
    {
        is_ok = DTL_GetData((uint8*)RespQ_ptr->parameter0,RespQ_ptr->parameter1,RespQ_ptr->parameter2);
        DTLCMDQ_Write_Data_ACK(is_ok);
    }
    else
    {
        //DTL_PRINT:"DTL_DirectRWData RespQ_ptr->resp_id=%x error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_602_112_2_17_23_8_56_5,(uint8*)"d",RespQ_ptr->resp_id);
    }
    
}

/************************************************************************
* \fn  PUBLIC void* DTLSVC_Task(
*                       void * argv
*                       )
* \param  argv
* \return void 
* \brief  dtl service task
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void* DTLSVC_Task(
                        void    *argv
                        )
{
    DTL_RESPQ_CMD_T *RespQ_ptr = PNULL;
    uint16 cmd_num = 0;
    uint16 i = 0;
        
    /* ALLEN: changed for TYLT temporarily */
    if ( s_dtl_svc_thread_sem_ptr )
    {
        /* if you have API to fill the kdThreadSemFree() interface, return 0
         * after invoking your API, otherwise, just return a non-zero number, say 1 */
        if( 0 == kdThreadSemFree(s_dtl_svc_thread_sem_ptr))
        {
            s_dtl_svc_thread_sem_ptr = KD_NULL;
        }
        
    }    
    if(NULL == s_dtl_svc_thread_sem_ptr)
    {
        s_dtl_svc_thread_sem_ptr = kdThreadSemCreate(0);
    }
    kdAssert(KD_NULL != s_dtl_svc_thread_sem_ptr);
#if 0
    if ( s_dtl_send_odd_sem_ptr )
    {
        if( 0 == kdThreadSemFree(s_dtl_send_odd_sem_ptr))
        {
            s_dtl_send_odd_sem_ptr = KD_NULL;
        }        
    }    
    if(NULL == s_dtl_send_odd_sem_ptr)
    {
        s_dtl_send_odd_sem_ptr = kdThreadSemCreate(0);
    }
    kdAssert(KD_NULL != s_dtl_send_odd_sem_ptr);
#endif    
    s_svc_task_is_active = TRUE;
    /* ALLEN: end */
    
    if ( s_dtl_mutex_ptr )
    {
        /* if you have API to fill the kdThreadMutexFree() interface, return 0
         * after invoking your API, otherwise, just return a non-zero number, say 1 */
        if( 0 == kdThreadMutexFree(s_dtl_mutex_ptr))
        {
            s_dtl_mutex_ptr = KD_NULL;
        }
        
    }
    if ( NULL == s_dtl_mutex_ptr)
    {
        s_dtl_mutex_ptr = kdThreadMutexCreate(KD_NULL);
    }
    kdAssert(PNULL != s_dtl_mutex_ptr);
        
    while(s_svc_task_is_active)
    {
        kdThreadSemWait(s_dtl_svc_thread_sem_ptr);
        
        if (!s_svc_task_is_active)
        {
            break;
        }
        
        RespQ_ptr = DTL_GetCmd(&cmd_num);
        
        //GPIO_EnableIntCtl(DTL_LCM_GPIO);
        GPIO_MTVIntEnable(TRUE);
        
        if ( RespQ_ptr )
        {
            DTL_SortCmd(RespQ_ptr,cmd_num);
            
            for ( i = 0; i < cmd_num; i ++ )
            {
                //DTL_PRINT:"DTLSVC_Task()RespQ_ptr[%d].resp_id=%x,parameter0=%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_694_112_2_17_23_8_56_6,(uint8*)"ddd",i,RespQ_ptr[i].resp_id,RespQ_ptr[i].parameter0);
                if ( RespQ_ptr[i].resp_id == BB_ARM_STATUS_READ_DATA
                    || RespQ_ptr[i].resp_id == BB_ARM_STATUS_WRITE_DATA
                    )
                {
                    DTL_DirectRWData(&(RespQ_ptr[i]));
                }
#if 0                
                else if (BB_ARM_CMDQ_WRITE_ODD_ACK== RespQ_ptr[i].resp_id)
                {
                     kdThreadSemPost(s_dtl_send_odd_sem_ptr);
                }
#endif                
                else
                {
                    SCI_SendEventToClient(DTL_SERVICE, (uint32)(DTL_Get_ModuleID(RespQ_ptr[i].resp_id)),(void*)&RespQ_ptr[i]);
                }
            }
        }
    }
    //g_dtl_svc_task_id = KD_NULL;
    //SCI_ThreadExit();
    //kdThreadSemFree(s_dtl_svc_thread_sem_ptr);
    
    kdThreadExit((void*)-1);
    return 0;
}

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
    uint16 i = 0;
    uint16 block_num = 0;
    uint16 tail_size = 0;
    uint16 comp_data = 0;
    
    kdAssert(dest_addr != 0);
    kdAssert(data_len!= 0);

    if ( !s_is_dtl_init )
    {
        return FALSE;
    }
    
    if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
    {
        if( DTL_IsErrorSerious())
        {
            return FALSE;
        }
    }

    //odd address
    if ( dest_addr %2 )
    {
        comp_data = data_to_send[0]&0xff;
        comp_data = comp_data<<8;
        //even length
        if ( 0 == (data_len % 2 )) 
        {
            comp_data |= *(data_to_send+data_len-1);
            DTLCMDQ_Write_Odd(dest_addr,dest_addr+data_len-1,comp_data);
            //kdThreadSemWait(s_dtl_send_odd_sem_ptr);
            data_len -=2;
        }
        else
        {
            DTLCMDQ_Write_Odd(dest_addr,dest_addr,comp_data);
            //kdThreadSemWait(s_dtl_send_odd_sem_ptr);
            data_len -=1;
        }
        dest_addr += 1;
        data_to_send ++;        
    }
    else
    {
        if ( data_len % 2  )
        {
            comp_data = data_to_send[data_len-1]&0xff;
            comp_data = comp_data<<8;
            DTLCMDQ_Write_Odd(dest_addr+data_len-1,dest_addr+data_len-1,comp_data);
            //kdThreadSemWait(s_dtl_send_odd_sem_ptr);
            data_len -=1;            
        }
    }

    if ( 0 == data_len )
    {
        return TRUE;
    }

    block_num = data_len/DTL_DATA_BLOCK_SIZE;
    tail_size = data_len%DTL_DATA_BLOCK_SIZE;
    
    //DTL_PRINT:"DTL_SendData() block_num=%d,tail_size=%d, addr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_805_112_2_17_23_8_56_7,(uint8*)"ddd",block_num,tail_size, dest_addr);
    
    for ( i = 0; i < block_num; i ++ )
    {
        //获取锁 进入临界区
        kdThreadMutexLock(s_dtl_mutex_ptr);

        if ( DTL_DATA_BLOCK_SIZE != LCMIF_Write_Data(data_to_send+i*DTL_DATA_BLOCK_SIZE,
                DTL_DATA_BLOCK_SIZE,dest_addr+i*DTL_DATA_BLOCK_SIZE))
        {
            //DTL_PRINT:"DTL_SendData() block_num=%d error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_815_112_2_17_23_8_56_8,(uint8*)"d",i);
            //释放锁 出临界区
            kdThreadMutexUnlock(s_dtl_mutex_ptr);
            if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
            {
                DTL_SetErrorInfo(DTL_ERR_SENDDATA);
                s_dtl_recover_callback = DTLR_GetCallback();
                if(s_dtl_recover_callback)
                {
                    s_dtl_recover_callback(DTLR_GetParam(),DTL_ERR_SENDDATA,0,0);
                }
            }
            return FALSE;
        }
        //释放锁 出临界区
        kdThreadMutexUnlock(s_dtl_mutex_ptr);
    }

    //获取锁 进入临界区
    kdThreadMutexLock(s_dtl_mutex_ptr);

    if ( tail_size )
    {
        if ( tail_size != LCMIF_Write_Data(data_to_send+i*DTL_DATA_BLOCK_SIZE,
                tail_size,dest_addr+i*DTL_DATA_BLOCK_SIZE))
        {
            //DTL_PRINT:"DTL_SendData() tail_size=%d error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_841_112_2_17_23_8_56_9,(uint8*)"d",tail_size);
            //释放锁 出临界区
            kdThreadMutexUnlock(s_dtl_mutex_ptr);
            if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
            {
                DTL_SetErrorInfo(DTL_ERR_SENDDATA);
                s_dtl_recover_callback = DTLR_GetCallback();
                if(s_dtl_recover_callback)
                {
                    s_dtl_recover_callback(DTLR_GetParam(),DTL_ERR_SENDDATA,0,0);
                }
            }
            return FALSE;
        }
    }
    
    //释放锁 出临界区
    kdThreadMutexUnlock(s_dtl_mutex_ptr);
    
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
    uint16 i = 0;
    uint16 block_num = 0;
    uint16 tail_size = 0;
    uint16 comp_data = 0;
    
    kdAssert(src_addr != 0);
    kdAssert(data_len!= 0);
    
    if ( !s_is_dtl_init )
    {
        return FALSE;
    }
    
    if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
    {
        if( DTL_IsErrorSerious())
        {
            return FALSE;
        }            
    }

    if ( src_addr % 2 )
    {
        //获取锁 进入临界区
        kdThreadMutexLock(s_dtl_mutex_ptr);

        LCMIF_Read_Data((uint8 *)&comp_data,2,src_addr-1);

        kdThreadMutexUnlock(s_dtl_mutex_ptr);

        INT_SWAP(comp_data);
        *data_buf++ = comp_data;
        data_len -= 1;
        src_addr += 1; 
    }
    
    if ( 0 == data_len )
    {

        return TRUE;
    }
    
    block_num = data_len/DTL_DATA_BLOCK_SIZE;
    tail_size = data_len%DTL_DATA_BLOCK_SIZE;

    //DTL_PRINT:"DTL_GetData() block_num=%d,tail_size=%d,src_addr=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_928_112_2_17_23_8_57_10,(uint8*)"ddd",block_num,tail_size,src_addr);
            
    for ( i = 0; i < block_num; i ++ )
    {
        //获取锁 进入临界区
        kdThreadMutexLock(s_dtl_mutex_ptr);
        if ( DTL_DATA_BLOCK_SIZE != LCMIF_Read_Data(data_buf+i*DTL_DATA_BLOCK_SIZE,
                DTL_DATA_BLOCK_SIZE,src_addr+i*DTL_DATA_BLOCK_SIZE))
        {
            //DTL_PRINT:"DTL_GetData() block_num=%d error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_937_112_2_17_23_8_57_11,(uint8*)"d",i);
            kdThreadMutexUnlock(s_dtl_mutex_ptr);
            if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
            {
                DTL_SetErrorInfo(DTL_ERR_GETDATA);
                s_dtl_recover_callback = DTLR_GetCallback();
                if(s_dtl_recover_callback)
                {
                    s_dtl_recover_callback(DTLR_GetParam(),DTL_ERR_GETDATA,0,0);
                }
            }
            return FALSE;
        }
        kdThreadMutexUnlock(s_dtl_mutex_ptr);
    }
    //获取锁 进入临界区
    kdThreadMutexLock(s_dtl_mutex_ptr);    
    if ( tail_size )
    {
        kdAssert(i*DTL_DATA_BLOCK_SIZE+tail_size == data_len); //delete pclint error662
        if ( tail_size != LCMIF_Read_Data(data_buf+i*DTL_DATA_BLOCK_SIZE,
                tail_size,src_addr+i*DTL_DATA_BLOCK_SIZE))
        {
            //DTL_PRINT:"DTL_GetData() tail_size=%d error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_960_112_2_17_23_8_57_12,(uint8*)"d",tail_size);
            kdThreadMutexUnlock(s_dtl_mutex_ptr);
            if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
            {
                DTL_SetErrorInfo(DTL_ERR_GETDATA);
                s_dtl_recover_callback = DTLR_GetCallback();
                if(s_dtl_recover_callback)
                {
                    s_dtl_recover_callback(DTLR_GetParam(),DTL_ERR_GETDATA,0,0);
                }
            }
            return FALSE;
        }
    }

    //释放锁 出临界区
    kdThreadMutexUnlock(s_dtl_mutex_ptr);
    
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
    KDuint32 next_cmd_in = 0;
    KDuint32 cmd_out = 0;
    BOOLEAN rw_result = FALSE;
    uint32 cmd_size = sizeof(DTL_CMDQ_CMD_T);
    uint8  poll_num = 0;

    if ( !s_is_dtl_init )
    {
        return FALSE;
    }
    
    if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
    {
        if( DTL_IsErrorSerious())
        {
            return FALSE;
        }
    }
    
    //获取锁 进入临界区
    kdThreadMutexLock(s_dtl_mutex_ptr);
    
    while(1){
        rw_result = LCMIF_Read(&next_cmd_in,DTL_CMDQ_IN_ADDR);
        INT_SWAP(next_cmd_in);
        DTL_LCM_RET_POS(rw_result,next_cmd_in,DTL_ERR_SENDCMD)
        next_cmd_in = ( next_cmd_in + 1 )%DTL_CMDQ_SIZE;

        rw_result = LCMIF_Read(&cmd_out,DTL_CMDQ_OUT_ADDR);
        INT_SWAP(cmd_out);
        DTL_LCM_RET_POS(rw_result,cmd_out,DTL_ERR_SENDCMD)
    
        if ( next_cmd_in != cmd_out )
        {
            break;
        }
        
        if ( poll_num++ > 100)
        {
            //DTL_PRINT:"write cmd error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1034_112_2_17_23_8_57_13,(uint8*)"");
            break;
        }
    }
        
    if ( next_cmd_in == cmd_out )
    {
        //error
        rw_result = LCMIF_Write(0,DTL_CMDQ_IN_ADDR);
        DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)
        rw_result = LCMIF_Write(0,DTL_CMDQ_OUT_ADDR);
        DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)
        next_cmd_in = 1;
    }

    //DTL_PRINT:"DTL_SendCMD()next_cmd_in=%d,cmd_to_send->cmd_id=%x,p0=%x,p1=%x (%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1049_112_2_17_23_8_57_14,(uint8*)"ddddd",next_cmd_in,cmd_to_send->cmd_id,cmd_to_send->parameter0,cmd_to_send->parameter1, kdGetTimeUST());

    /* swap byte order */
    INT_SWAP(cmd_to_send->cmd_id);
    INT_SWAP(cmd_to_send->parameter0);
    INT_SWAP(cmd_to_send->parameter1);
    INT_SWAP(cmd_to_send->parameter2);
    
    if ( cmd_size != LCMIF_Write_Data((uint8*)cmd_to_send,cmd_size,
            (uint32)(DTL_CMDQ_DATA_START_ADDR +(next_cmd_in * cmd_size)) ))
    {
        //DTL_PRINT:"DTL_SendCMD() write_data error!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1060_112_2_17_23_8_57_15,(uint8*)"");
        kdThreadMutexUnlock(s_dtl_mutex_ptr);
        return FALSE;
    }

    INT_SWAP(next_cmd_in);
    rw_result = LCMIF_Write(next_cmd_in,DTL_CMDQ_IN_ADDR);
    DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)

    GPIO_MTVSendInt();

    //释放锁 出临界区
    kdThreadMutexUnlock(s_dtl_mutex_ptr);

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
    KDuint32 next_cmd_in = 0;
    KDuint32 cmd_out = 0;
    BOOLEAN rw_result = FALSE;
    uint32 cmd_size = sizeof(DTL_CMDQ_CMD_T);

    if ( !s_is_dtl_init )
    {
        return FALSE;
    }
    
    if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
    {
        if( DTL_IsErrorSerious())
        {
            return FALSE;
        }
    }

    //获取锁 进入临界区
    kdThreadMutexLock(s_dtl_mutex_ptr);
    
    rw_result = LCMIF_Read(&next_cmd_in,(uint32)DTL_CMDQ_IN_ADDR);
    INT_SWAP(next_cmd_in);
    DTL_LCM_RET_POS(rw_result,next_cmd_in,DTL_ERR_SENDCMD)
    next_cmd_in = ( next_cmd_in + 1 )%DTL_CMDQ_SIZE;
    
    rw_result = LCMIF_Read(&cmd_out,DTL_CMDQ_OUT_ADDR);
    INT_SWAP(cmd_out);
    DTL_LCM_RET_POS(rw_result,cmd_out,DTL_ERR_SENDCMD)
    if ( next_cmd_in == cmd_out )
    {
        //error
        rw_result = LCMIF_Write(0,DTL_CMDQ_IN_ADDR);
        DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)
        rw_result = LCMIF_Write(0,DTL_CMDQ_OUT_ADDR);
        DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)
        next_cmd_in = 1;
    }
    
    //DTL_PRINT:"DTL_SendCMD_NOINT()next_cmd_in=%d,cmd_out=%d,cmd_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1131_112_2_17_23_8_57_16,(uint8*)"ddd",next_cmd_in,cmd_out,cmd_to_send->cmd_id);
    
    /* swap byte order */
    INT_SWAP(cmd_to_send->cmd_id);
    INT_SWAP(cmd_to_send->parameter0);
    INT_SWAP(cmd_to_send->parameter1);
    INT_SWAP(cmd_to_send->parameter2);
    
    if ( cmd_size != LCMIF_Write_Data((uint8*)cmd_to_send,cmd_size,
        (uint32)(DTL_CMDQ_DATA_START_ADDR +(next_cmd_in * cmd_size)) ))
    {
        //DTL_PRINT:"DTL_SendCMD() write_data error!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1142_112_2_17_23_8_57_17,(uint8*)"");
        kdThreadMutexUnlock(s_dtl_mutex_ptr);
        return FALSE;
    }
    
    INT_SWAP(next_cmd_in);
    rw_result = LCMIF_Write(next_cmd_in,DTL_CMDQ_IN_ADDR);
    DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)
    
    //    DTL_SendInt();
    
    //释放锁 出临界区
    kdThreadMutexUnlock(s_dtl_mutex_ptr);
    
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
    uint16 i = 0;
    uint16 block_num = 0;
    uint16 tail_size = 0;
    KDuint32 next_cmd_in = 0;
    KDuint32 cmd_out = 0;
    BOOLEAN rw_result = FALSE;
    uint32 cmd_size = sizeof(DTL_CMDQ_CMD_T);
    uint32 dest_addr = CMD_BB_ARM_MSGQ_BASE;// message queue base addr
    uint8  poll_num = 0;
    
    if ( !s_is_dtl_init )
    {
        return FALSE;
    }
        
    if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
    {
        if( DTL_IsErrorSerious())
        {
            return FALSE;
        }
    }
    
    //获取锁 进入临界区
    kdThreadMutexLock(s_dtl_mutex_ptr);
    
    while(1){
        //get write command position
        rw_result = LCMIF_Read(&next_cmd_in,(uint32)DTL_CMDQ_IN_ADDR);
        INT_SWAP(next_cmd_in);
        DTL_LCM_RET_POS(rw_result,next_cmd_in,DTL_ERR_SENDCMD)
    
        next_cmd_in = ( next_cmd_in + 1 )%DTL_CMDQ_SIZE;

        rw_result = LCMIF_Read(&cmd_out,DTL_CMDQ_OUT_ADDR);
        INT_SWAP(cmd_out);
        DTL_LCM_RET_POS(rw_result,cmd_out,DTL_ERR_SENDCMD)
    
        if ( next_cmd_in != cmd_out )
        {
            break;
        }
        
        if ( poll_num++ > 100)
        {
            //DTL_PRINT:"write cmd error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1225_112_2_17_23_8_57_18,(uint8*)"");
            break;
        }
    }

    if ( next_cmd_in == cmd_out )
    {
        //error
        rw_result = LCMIF_Write(0,DTL_CMDQ_IN_ADDR);
        DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)

        rw_result = LCMIF_Write(0,DTL_CMDQ_OUT_ADDR);
        DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)

        next_cmd_in = 1;
    }

    dest_addr += next_cmd_in * CMD_MESSAGE_LEN;

    //send message data
    block_num = msg_data_len/DTL_DATA_BLOCK_SIZE;
    tail_size = msg_data_len%DTL_DATA_BLOCK_SIZE;
    
    for ( i = 0; i < block_num; i ++ )
    {
        if ( DTL_DATA_BLOCK_SIZE != LCMIF_Write_Data(msg_data_ptr+i*DTL_DATA_BLOCK_SIZE,
                DTL_DATA_BLOCK_SIZE,dest_addr+i*(DTL_DATA_BLOCK_SIZE/sizeof(uint32))))
        {
            //DTL_PRINT:"DTL_SendCMD_And_MsgParam() data_size = %d error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1253_112_2_17_23_8_57_19,(uint8*)"d",DTL_DATA_BLOCK_SIZE);
            kdThreadMutexUnlock(s_dtl_mutex_ptr);
            return FALSE;
        }
    }
    if ( tail_size )
    {
        if ( tail_size != LCMIF_Write_Data(msg_data_ptr+i*DTL_DATA_BLOCK_SIZE,
                tail_size,dest_addr+i*(DTL_DATA_BLOCK_SIZE/sizeof(uint32))))
        {
            //DTL_PRINT:"DTL_SendCMD_And_MsgParam() tail_size = %d error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1263_112_2_17_23_8_57_20,(uint8*)"d",tail_size);
            kdThreadMutexUnlock(s_dtl_mutex_ptr);
            return FALSE;
        }
    }

    /* swap byte order */
    INT_SWAP(cmd_to_send->cmd_id);
    INT_SWAP(cmd_to_send->parameter0);
    INT_SWAP(cmd_to_send->parameter1);
    INT_SWAP(cmd_to_send->parameter2);

    if ( cmd_size != LCMIF_Write_Data((uint8*)cmd_to_send,cmd_size,
            (uint32)(DTL_CMDQ_DATA_START_ADDR +(next_cmd_in * cmd_size)) ))
    {
        //DTL_PRINT:"DTL_SendCMD_And_MsgParam() write cmd error!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1278_112_2_17_23_8_57_21,(uint8*)"");
        kdThreadMutexUnlock(s_dtl_mutex_ptr);
        return FALSE;
    }

    INT_SWAP(next_cmd_in);
    rw_result = LCMIF_Write(next_cmd_in,DTL_CMDQ_IN_ADDR);
    DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)

    GPIO_MTVSendInt();

    //释放锁 出临界区
    kdThreadMutexUnlock(s_dtl_mutex_ptr);
    
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
    KDuint32 next_cmd_in = 0;
    KDuint32 cmd_out = 0;
    BOOLEAN rw_result = FALSE;
    uint32 cmd_size = sizeof(DTL_CMDQ_CMD_T);
    
    if ( !s_is_dtl_init )
    {
        return FALSE;
    }    
    if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
    {
        if( DTL_IsErrorSerious())
        {
            return FALSE;
        }
    }
    
    rw_result = LCMIF_Read(&next_cmd_in,(uint32)DTL_CMDQ_IN_ADDR);
    INT_SWAP(next_cmd_in);
    //DTL_LCM_RET_POS(rw_result,next_cmd_in,DTL_ERR_SENDCMD)
    next_cmd_in = ( next_cmd_in + 1 )%DTL_CMDQ_SIZE;
    
    rw_result = LCMIF_Read(&cmd_out,DTL_CMDQ_OUT_ADDR);
    INT_SWAP(cmd_out);
    //DTL_LCM_RET_POS(rw_result,cmd_out,DTL_ERR_SENDCMD)
    if ( next_cmd_in == cmd_out )
    {
        //error
        rw_result = LCMIF_Write(0,DTL_CMDQ_IN_ADDR);
        //DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)
        rw_result = LCMIF_Write(0,DTL_CMDQ_OUT_ADDR);
        //DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)
        next_cmd_in = 1;
    }
    
    //DTL_PRINT:"DTL_SendCMD_NOINT()next_cmd_in=%d,cmd_out=%d,cmd_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1344_112_2_17_23_8_57_22,(uint8*)"ddd",next_cmd_in,cmd_out,cmd_to_send->cmd_id);
    
    /* swap byte order */
    INT_SWAP(cmd_to_send->cmd_id);
    INT_SWAP(cmd_to_send->parameter0);
    INT_SWAP(cmd_to_send->parameter1);
    INT_SWAP(cmd_to_send->parameter2);
    
    if ( cmd_size != LCMIF_Write_Data((uint8*)cmd_to_send,cmd_size,
        (uint32)(DTL_CMDQ_DATA_START_ADDR +(next_cmd_in * cmd_size)) ))
    {
        //DTL_PRINT:"DTL_SendCMD() write_data error!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1355_112_2_17_23_8_57_23,(uint8*)"");
        return FALSE;
    }
    
    INT_SWAP(next_cmd_in);
    rw_result = LCMIF_Write(next_cmd_in,DTL_CMDQ_IN_ADDR);
    //DTL_LCM_RET(rw_result,DTL_ERR_SENDCMD)
    
    //    DTL_SendInt();
        
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
    KDuint32 cmd_in = 0;
    KDuint32 cmd_out = 0;
    uint16 cmd_num = 0;
    uint16 total_cmd_num = 0;
    uint32 read_data_size = 0;
    BOOLEAN is_continue = FALSE;
    BOOLEAN rw_result   = FALSE;
    DTL_RESPQ_CMD_T* RespQ_ptr = PNULL;
    DTL_RESPQ_CMD_T* Cmd_RespQ_ptr = (DTL_RESPQ_CMD_T*)s_dtl_resp_cmd_que;

    DTL_RESPQ_CMD_T* tmp_ptr;
    uint32 cmd_num_tmp;
    
    *cmd_num_ptr = 0;
    
    if ( !s_is_dtl_init )
    {
        return PNULL;
    }    
    
    if ( MTV_RELEASE_MODE == s_dtl_is_debug_mode )
    {
        if( DTL_IsErrorSerious())
        {
            return PNULL;
        }
    }
    
    //获取锁 进入临界区
    kdThreadMutexLock(s_dtl_mutex_ptr);
    
    rw_result = LCMIF_Read(&cmd_in,DTL_RESPQ_IN_ADDR);
    INT_SWAP(cmd_in);
    DTL_LCM_RET_POS(rw_result,cmd_in,DTL_ERR_GETCMD)
    rw_result = LCMIF_Read(&cmd_out,DTL_RESPQ_OUT_ADDR);
    INT_SWAP(cmd_out);
    DTL_LCM_RET_POS(rw_result,cmd_out,DTL_ERR_GETCMD)
    
    //计算要读取的command 个数
    while(cmd_in != cmd_out)
    {
        //计算要读取的command 个数
        //if ( cmd_in == cmd_out )
        //{
        //    SCI_ASSERT(SCI_SUCCESS == SCI_PutMutex(s_dtl_mutex_ptr));
        //    return PNULL;
        //}
        if ( cmd_in > cmd_out )
        {
            cmd_num = cmd_in-cmd_out;
            is_continue = TRUE;
        }
        else if ( cmd_in < cmd_out )
        {
            cmd_num = cmd_in + DTL_RESPQ_SIZE - cmd_out;
            kdAssert(cmd_num <= DTL_RESPQ_SIZE );
        }
        
        cmd_out = (cmd_out+1)%DTL_RESPQ_SIZE;
                
        //读command
        RespQ_ptr = (DTL_RESPQ_CMD_T*)kdMalloc(sizeof(DTL_RESPQ_CMD_T)*cmd_num);
        kdAssert(PNULL != RespQ_ptr);
        
        if ( is_continue )
        {
            read_data_size = cmd_num * sizeof(DTL_RESPQ_CMD_T);
            if ( read_data_size != LCMIF_Read_Data((uint8*)RespQ_ptr,read_data_size,
                DTL_RESPQ_DATA_START_ADDR + cmd_out * sizeof(DTL_RESPQ_CMD_T)))
            {
                kdFree(RespQ_ptr);
                kdThreadMutexUnlock(s_dtl_mutex_ptr);
                return PNULL;
            }
        }
        else
        {
            if ( cmd_out )
            {
                read_data_size = (DTL_RESPQ_SIZE - cmd_out) * sizeof(DTL_RESPQ_CMD_T);
                if ( read_data_size != LCMIF_Read_Data((uint8*)RespQ_ptr,read_data_size,
                    DTL_RESPQ_DATA_START_ADDR + cmd_out * sizeof(DTL_RESPQ_CMD_T)))
                {
                    kdFree(RespQ_ptr);
                    kdThreadMutexUnlock(s_dtl_mutex_ptr);
                    return PNULL;
                }
                
                read_data_size = (cmd_in+1) * sizeof(DTL_RESPQ_CMD_T);
                if ( read_data_size != LCMIF_Read_Data((uint8*)RespQ_ptr+( (DTL_RESPQ_SIZE - cmd_out) * sizeof(DTL_RESPQ_CMD_T)),
                    read_data_size,DTL_RESPQ_DATA_START_ADDR ))
                {
                    kdFree(RespQ_ptr);
                    kdThreadMutexUnlock(s_dtl_mutex_ptr);
                    return PNULL;
                }
            }
            else
            {
                read_data_size = cmd_num * sizeof(DTL_RESPQ_CMD_T);
                if ( read_data_size != LCMIF_Read_Data((uint8*)RespQ_ptr,read_data_size,DTL_RESPQ_DATA_START_ADDR ))//(cmd_in+1)
                {
                    kdFree(RespQ_ptr);
                    kdThreadMutexUnlock(s_dtl_mutex_ptr);
                    return PNULL;
                }
            }
        }
        
        //写cmd_out 标志
        INT_SWAP(cmd_in);
        rw_result = LCMIF_Write(cmd_in,DTL_RESPQ_OUT_ADDR);
        DTL_LCM_RET(rw_result,DTL_ERR_GETCMD)
        kdMemcpy((uint8*)Cmd_RespQ_ptr,(uint8*)RespQ_ptr,sizeof(DTL_RESPQ_CMD_T)*cmd_num);

        tmp_ptr = Cmd_RespQ_ptr;
        cmd_num_tmp = cmd_num;
        while(cmd_num_tmp--)
        {
            /* swap byte order */
             INT_SWAP(tmp_ptr->resp_id);
             //DTL_PRINT:"DTL_GetCmd() tmp_ptr->resp_id=%x"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,DTL_1504_112_2_17_23_8_58_24,(uint8*)"d",tmp_ptr->resp_id);
             INT_SWAP(tmp_ptr->parameter0);
             INT_SWAP(tmp_ptr->parameter1);
             INT_SWAP(tmp_ptr->parameter2);
             tmp_ptr++;
        }
        kdFree(RespQ_ptr);
        
        
        
        Cmd_RespQ_ptr += cmd_num;
        total_cmd_num+=cmd_num;
        kdAssert(total_cmd_num<DTL_MAX_RECV_RESP_CMD_NUM);
        
        //SCI_TRACE_LOW("total_cmd_num=%d",total_cmd_num);
        rw_result = LCMIF_Read(&cmd_in,DTL_RESPQ_IN_ADDR);
        INT_SWAP(cmd_in);
        DTL_LCM_RET_POS(rw_result,cmd_in,DTL_ERR_GETCMD)
        rw_result = LCMIF_Read(&cmd_out,DTL_RESPQ_OUT_ADDR);
        INT_SWAP(cmd_out);
        DTL_LCM_RET_POS(rw_result,cmd_out,DTL_ERR_GETCMD)
        
    }
    
    //释放锁 出临界区
    kdThreadMutexUnlock(s_dtl_mutex_ptr);
    
    //*cmd_num_ptr = cmd_num;
    //return RespQ_ptr;
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

    kdAssert( PNULL != resp_ptr );

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
* \fn  LOCAL void DTLDATA_Task(
                        uint32 argc, 
                        void * argv
                        )
* \param  argc
* \param  argv
* \return void 
* \brief  asynchronous data transfer task
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void DTLDATA_Task(
                        uint32 argc, 
                        void * argv
                        )
{
    DTL_DATA_SIG_T *data_sig_ptr = PNULL;
    
    while(1)
    {
        //从消息队列中获取消息
        data_sig_ptr = (DTL_DATA_SIG_T*)SCI_GetSignal(P_DTLDATA);
        
        switch(data_sig_ptr->SignalCode)
        {
        case DTL_DATA_FETCH_DATA:
            if ( DTL_GetData(data_sig_ptr->data_msg.data_ptr, data_sig_ptr->data_msg.data_len,
                    data_sig_ptr->data_msg.data_addr))
            {
                if ( data_sig_ptr->data_msg.callback_func )
                {
                    data_sig_ptr->data_msg.callback_func(data_sig_ptr->SignalCode,data_sig_ptr->data_msg.data_ptr);
                }
            }
            break;
            
        case DTL_DATA_POST_DATA:
            if ( DTL_SendData(data_sig_ptr->data_msg.data_ptr,data_sig_ptr->data_msg.data_len,
                    data_sig_ptr->data_msg.data_addr))
            {
                if ( data_sig_ptr->data_msg.callback_func )
                {
                    data_sig_ptr->data_msg.callback_func(data_sig_ptr->SignalCode,data_sig_ptr->data_msg.data_ptr);
                }
            }
            break;
            
        case DTL_DATA_POST_CMD:
            if ( DTL_SendCMD((DTL_CMDQ_CMD_T*)data_sig_ptr->data_msg.data_ptr))
            {
                if ( data_sig_ptr->data_msg.callback_func )
                {
                    data_sig_ptr->data_msg.callback_func(data_sig_ptr->SignalCode,data_sig_ptr->data_msg.data_ptr);
                }
            }
            break;
        default:
            break;
        }
        kdFree(data_sig_ptr);
    }
}

/************************************************************************
* \fn  PUBLIC void DTL_SetCallback(void);
* \param callback
* \return void 
* \brief  call it after boot ok
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_SetCallback(DTL_CALLBACK callback)
{
    if ( callback )
    {
        s_dtl_callback = callback;
    }
    
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
    
    kdAssert(start_event<=end_event);
    kdAssert(PNULL != fun);
    
    for ( j = start_event;j<=end_event;j++)
    {
        for ( i = 0;i < DTL_SVC_MSG_REG_MAX_NUM; i ++ )
        {
            if ( fun == s_dtl_svc_msg_reg[i].reg_func_ptr
                && j == s_dtl_svc_msg_reg[i].event )
            {
                kdAssert(0); /*lint !e506*/
            }
            
            if ( PNULL == s_dtl_svc_msg_reg[i].reg_func_ptr )
            {
                s_dtl_svc_msg_reg[i].event    = j;
                s_dtl_svc_msg_reg[i].reg_func_ptr = fun;
                
                KD_TRACE_LOW("DTL_RegisterMsg,event=%d,i=%d,task_id=0x%x",j,i,
                                        s_dtl_svc_msg_reg[i].virtual_task_id);
                
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
    
    kdAssert(start_event<=end_event);
    kdAssert(PNULL != fun);
    
    for ( j = start_event;j<=end_event;j++ )
    {
        for ( i = 0;i < DTL_SVC_MSG_REG_MAX_NUM; i ++ )
        {
            if ( fun == s_dtl_svc_msg_reg[i].reg_func_ptr
                && j == s_dtl_svc_msg_reg[i].event )
            {
                s_dtl_svc_msg_reg[i].reg_func_ptr = PNULL;
                
                KD_TRACE_LOW("DTL_UnregisterMsg,event=%d,i=%d,task_id=0x%x",j,i,
                    s_dtl_svc_msg_reg[i].virtual_task_id);
                
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
* \fn  LOCAL void TurnoffSeineLdo(
*                          BOOLEAN is_r_poweron
*                          )
* \param is_r_poweron is rocky power on,
*  TRUE: put battery,need close seine ldo 
*  FALSE tv off,close seine ldo 
* \return void 
* \brief  power down seine some ldo
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void TurnoffSeineLdo(
                           BOOLEAN is_r_poweron
                           )
{
    uint32  reg_data1  =  0x20;
    uint32  reg_data2  =  0x20202000;
    uint32  reg_data3  =  0x20;
    
    INT_SWAP(reg_data1);
    INT_SWAP(reg_data2);
    INT_SWAP(reg_data3);

   	if  ( is_r_poweron )
    {
        LCMIF_Write(reg_data1,0X8400003c);
        LCMIF_Write(reg_data2,0X84000038);
        LCMIF_Write(reg_data3,0X84000034);
    }
    else
    {
        DTL_SendData((uint8*)&reg_data1,sizeof(uint32),0x8400003c);
        DTL_SendData((uint8*)&reg_data2,sizeof(uint32),0x84000038);
        DTL_SendData((uint8*)&reg_data3,sizeof(uint32),0x84000034);
	}    
 
}

/************************************************************************
* \fn  PUBLIC void DTL_TurnoffSeineLdo(void)
* \param void
* \return void 
* \brief  power down seine some ldo
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_TurnoffSeineLdo(void)
{
    MTVCFG_Open_26MClock();
    //power on seine
    GPIO_MTVReset(TRUE);
    GPIO_MTVStandby(FALSE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVLCDBypass(FALSE);
    //kdMsDelay(5);
    
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVReset(FALSE);
    //for ( i =0; i< 1000000;i ++ );
    kdMsDelay(DTL_DELAY_TIME);
    
    LCMIF_Init(TRUE);
    
    TurnoffSeineLdo(TRUE);
    
    LCMIF_Close();
    
    //power down seine
    GPIO_MTVLCDBypass(TRUE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVStandby(TRUE);
    
    MTVCFG_Close_26MClock();
    //lcm write mode
    //*(volatile uint32 *)(0x60001C64) = 0x0;


}

/************************************************************************
* \fn  PUBLIC void DTL_SetErrorInfo(DTL_ERR_INFO_E err_id)
* \param err_id error id
* \return void 
* \brief  set error info
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_SetErrorInfo(DTL_ERR_INFO_E err_id)
{
    s_dtl_error_id = err_id;
    KD_TRACE_LOW("DTL_SetErrorInfo() ERR_ID=%d",s_dtl_error_id);
}

/************************************************************************
* \fn  PUBLIC DTL_ERR_INFO_E DTL_GetErrorInfo(void)
* \param err_id error id
* \return void 
* \brief  get error info
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC DTL_ERR_INFO_E DTL_GetErrorInfo(void)
{
    return s_dtl_error_id;
}

/************************************************************************
* \fn  PUBLIC void DTL_SetLittleErrorInfo(DTL_ERR_INFO_E err_id)
* \param err_id error id
* \return void 
* \brief  set error info
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_SetLittleErrorInfo(DTL_ERR_INFO_E err_id)
{
    s_dtl_little_error_id = err_id;
    KD_TRACE_LOW("DTL_SetLittleErrorInfo() ERR_ID=%d",s_dtl_error_id);
}

/************************************************************************
* \fn  PUBLIC DTL_ERR_INFO_E DTL_GetLittleErrorInfo(void)
* \param err_id error id
* \return void 
* \brief  get error info
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC DTL_ERR_INFO_E DTL_GetLittleErrorInfo(void)
{
    return s_dtl_little_error_id;
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
/*
    if( DTL_ERR_GETCMD == DTL_GetErrorInfo()
        || DTL_ERR_SENDCMD == DTL_GetErrorInfo()
        || DTL_ERR_GETDATA == DTL_GetErrorInfo()
        || DTL_ERR_SENDDATA == DTL_GetErrorInfo()
        || DTL_ERR_SEINE_ARM_ASSERT == DTL_GetErrorInfo()
        || DTL_ERR_SEINE_DSP_ASSERT == DTL_GetErrorInfo()
        )
        */
    if ( DTL_ERR_SEINE_DSP_ASSERT >= DTL_GetErrorInfo() )
    {
        return TRUE;
    }
    return FALSE;
}

/************************************************************************
* \fn  KDboolean DTL_IdentifyLCDId(KDuint32  lcdid_addr,
*                                  KDuint32 *lcd_id1_ptr,
*                                  KDuint32 *lcd_id2_ptr
*                                  );
* \param uint32 lcdid_addr lcd id address
* \param uint32 *lcd_id1_ptr when cd high,read  it
* \param uint32 *lcd_id2_ptr when cd low,read it
* \param uint32 sim_smd_ptr smd 1:found,0:no
* \return void 
* \brief  get lcd id
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTL_IdentifyLCDId(KDuint32  lcdid_addr,
                            KDuint32 *lcd_id1_ptr,
                            KDuint32 *lcd_id2_ptr,
                            KDuint32  *sim_smd_ptr
                            )
{
    BOOLEAN is_read = FALSE;

    kdAssert(lcd_id1_ptr!= PNULL);
    kdAssert(lcd_id2_ptr!= PNULL);

    MTVCFG_Open_26MClock();
    //power on seine
    GPIO_MTVReset(TRUE);

    GPIO_MTVStandby(FALSE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVLCDBypass(FALSE);
    //kdMsDelay(5);
    
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVReset(FALSE);
    //for ( i =0; i< 1000000;i ++ );
    kdMsDelay(DTL_DELAY_TIME);
    
    LCMIF_Init(TRUE);
    s_is_dtl_init = TRUE;
    is_read = MTVBOOT_IdentifyLCDId(lcdid_addr,lcd_id1_ptr,lcd_id2_ptr,sim_smd_ptr);
	s_is_dtl_init = FALSE;

    LCMIF_Close();
    
    //power down seine
    GPIO_MTVLCDBypass(TRUE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVStandby(TRUE);
    
    MTVCFG_Close_26MClock();
    //lcm write mode
//    *(volatile uint32 *)(0x60001C64) = 0x0;

    return is_read;
}

/************************************************************************
* \fn  PUBLIC void DTL_MTVSoftSwitchON(BOOLEAN is_on)
* \param is_on 
* \return void 
* \brief  turn on or off seine soft switch
*             
* \author Hanbing.Zhang
* \note   power on mobile,turn on soft switch
************************************************************************/
PUBLIC void DTL_MTVSoftSwitchON(BOOLEAN is_on)
{
    GPIO_MTVSoftSwitchON(is_on);
}

/************************************************************************
* \fn  PUBLIC void DTL_ClearRWPointer(void)
* \param void
* \return void 
* \brief  clear queue rw pointer
*             
* \author Hanbing.Zhang
* \note   clear command write queue pointer
************************************************************************/
PUBLIC void DTL_ClearRWPointer(void)
{
    uint32 init_data = 0;
    DTL_SendData((uint8*)&init_data,4,DTL_CMDQ_IN_ADDR);
    DTL_SendData((uint8*)&init_data,4,DTL_CMDQ_OUT_ADDR);
}

/************************************************************************
* \fn  PUBLIC void DTL_SetCmdQSize(uint8 cmdq_size)
* \param cmdq_size 
* \return void 
* \brief  change cmdq size  when boot size is 8, boot ok size is 16
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_SetCmdQSize(uint8 cmdq_size)
{
    cmdq_respq_cmdq_size = cmdq_size;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
