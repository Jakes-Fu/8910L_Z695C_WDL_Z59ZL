/******************************************************************************
 ** File Name:      dtl.h                                                     *
 ** Author:         Hanbing Zhang                                             *
 ** DATE:           1/11/2007                                                 *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define data transfer layer interface            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 1/11/2007     Hanbing.Zhang        create                                 *
 ******************************************************************************/

#ifndef _DTL_H
#define _DTL_H

#include "Sci_types.h"
#include "sci_service.h"
#include "cmd_common.h"
#include "sig_code.h"
#include "os_api.h"
#include "dtl_recovery.h"

#ifdef __cplusplus
extern   "C"
{
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/* Endianess stuff */
#define swab32(data32_ptr) \
do{\
    uint8* data8_ptr = (uint8*)data32_ptr;\
    uint8 tmp;\
    tmp = data8_ptr[3];\
    data8_ptr[3] = data8_ptr[0];\
    data8_ptr[0] = tmp;\
\
    tmp = data8_ptr[2];\
    data8_ptr[2] = data8_ptr[1];\
    data8_ptr[1] = tmp;\
}while(0)

#define swab16(data16_ptr)\
do{\
    uint8* data8_ptr = (uint8*)data16_ptr;\
    uint8 tmp;\
    tmp = data8_ptr[1];\
    data8_ptr[1] = data8_ptr[0];\
    data8_ptr[0] = tmp;\
}while(0)

#define _INT_SWAP(type, ptr) \
   do{ if(sizeof(type) == 4) {  \
            swab32(ptr); \
        }else if(sizeof(type) == 2) { \
        swab16(ptr);}   \
    }while(0)

#ifdef HOST_IS_LITTLEEND
#define INT_SWAP(data) _INT_SWAP(data, &(data))
#else
#define INT_SWAP(data)
#endif


//dtl service task id
//extern BLOCK_ID g_dtl_svc_task_id = 0;
#define P_DTLSVC g_dtl_svc_task_id

#define DTL_CMDQ_RESPQ_ORGINAL_SIZE 8
#define DTL_CMDQ_RESPQ_EXTEND_SIZE  16

 
//»Øµ÷º¯Êý
typedef void (*DTL_DATA_CALLBACK)(uint32 msg_type,uint8* data_ptr);

//send command 
#define DTL_CMDQ_CMD_T CMDQ_BB_ARM_CMD_T
//respond command
#define DTL_RESPQ_CMD_T CMDQ_BB_ARM_RESP_T

//register msg module
typedef enum dtl_cmdq_module_tag
{
    DTL_CMDQ_START,
    DTL_CMDQ_DSP_COMMON,
    DTL_CMDQ_DSP_AUDIO,
    DTL_CMDQ_DSP_DEMUX,
    DTL_CMDQ_DSP_DEMODULATE,
    DTL_CMDQ_ARM,
    DTL_CMDQ_LCDC,
    DTL_CMDQ_VIDEO,
    DTL_CMDQ_DSP_RF_DVT,
    DTL_CMDQ_ARM_SCREEN_TEST,
    DTL_CMDQ_ACK_MTVAPI,
    DTL_CMDQ_ACK_MTVAPI_APP,
    DTL_CMDQ_ARM_SIM,
    DTL_CMDQ_END
}DTL_CMDQ_MODULE_E;

#define MAX_DESCRAMBLE_KEY_NUM  2
#define MAX_DESCRAMBLE_KEY_LEN  16
typedef struct descramble_key_tag
{
    uint16 key_id;
    uint8 key[MAX_DESCRAMBLE_KEY_LEN];
}DEMOD_DESCARMBLE_T;


/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/

/************************************************************************
* \fn  PUBLIC void DTL_Initialize(void)
* \param  void
* \return void 
* \brief  initialize the data transfer layer
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC void DTL_Initialize(void);

/************************************************************************
* \fn  PUBLIC void DTL_Close(void)
* \param  void
* \return void 
* \brief  finish the data transfer layer
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
PUBLIC void DTL_Close(void);

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
                            );

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
                   );

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
                           );

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
                                 );

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
                                        );

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
                                 );

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
                                  );
/************************************************************************
* \fn  typedef void (*DTL_CALLBACK)(void);
* \return void 
* \brief  call it after boot ok
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
typedef void (*DTL_CALLBACK)(void);
                                  
/************************************************************************
* \fn  PUBLIC void DTL_SetCallback(void);
* \param callback
* \return void 
* \brief  call it after boot ok
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_SetCallback(DTL_CALLBACK callback);

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
                               );

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_UnregisterMsg(
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
                                 );
                                 
/************************************************************************
* \fn  PUBLIC void DTL_SetErrorInfo(DTL_ERR_INFO_E err_id)
* \param err_id error id
* \return void 
* \brief  set error info
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_SetErrorInfo(DTL_ERR_INFO_E err_id);

/************************************************************************
* \fn  PUBLIC DTL_ERR_INFO_E DTL_GetErrorInfo(void)
* \param void
* \return void 
* \brief  get error info
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC DTL_ERR_INFO_E DTL_GetErrorInfo(void);

/************************************************************************
* \fn  PUBLIC void DTL_SetLittleErrorInfo(DTL_ERR_INFO_E err_id)
* \param err_id error id
* \return void 
* \brief  set error info
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_SetLittleErrorInfo(DTL_ERR_INFO_E err_id);

/************************************************************************
* \fn  PUBLIC DTL_ERR_INFO_E DTL_GetLittleErrorInfo(void)
* \param err_id error id
* \return void 
* \brief  get error info
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC DTL_ERR_INFO_E DTL_GetLittleErrorInfo(void);

/************************************************************************
* \fn  PUBLIC BOOLEAN DTL_IsErrorSerious(void)
* \param void
* \return BOOLEAN 
* \brief  is error serious
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC BOOLEAN DTL_IsErrorSerious(void);

/************************************************************************
* \fn  PUBLIC void DTL_MTVSoftSwitchON(BOOLEAN is_on)
* \param is_on 
* \return void 
* \brief  turn on or off seine soft switch
*             
* \author Hanbing.Zhang
* \note   power on mobile,turn on soft switch
************************************************************************/
PUBLIC void DTL_MTVSoftSwitchON(BOOLEAN is_on);

/************************************************************************
* \fn  PUBLIC void DTL_SetCmdQSize(uint8 cmdq_size)
* \param cmdq_size 
* \return void 
* \brief  change cmdq size  when boot size is 8, boot ok size is 16
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_SetCmdQSize(uint8 cmdq_size);

PUBLIC uint32 DTL_GetServiceHandle(uint32 service_idx);
PUBLIC uint32 DTL_GetServiceSubframeIdx(uint32 service_idx);
PUBLIC void DTL_UpdateServiceIdx(uint32 service_idx,BOOLEAN is_start);
PUBLIC void DTL_UpdateServiceHandle(uint32 service_handle,uint32 subframe_idx);

PUBLIC void DTL_SetSaltKey(uint8* vid_salt,uint8* aud_salt);
PUBLIC void DTL_GetSaltKey(uint8** vid_salt,uint8** aud_salt);

PUBLIC void DTL_SetDescramKey(uint16 key_id,uint8* cw);
PUBLIC DEMOD_DESCARMBLE_T* DTL_GetDescramKey(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif
