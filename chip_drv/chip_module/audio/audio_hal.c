/******************************************************************************
 ** File Name:      audio_hal.c                                               *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of audio device. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang        Create.                                   *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "priority_system.h"
#include "chip_plf_export.h"

#include "chg_drvapi.h"
#include "audio_drvapi.h"

#include "audio_dai.h"
#include "audio_codec.h"
#include "audio_hal.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
#define AUDIO_HAL_MUTEX_NAME        "AUDIO_HAL_MUTEX"
#define AUDIO_HAL_TASK_NAME         "AUDIO_HAL_TASK"
#define AUDIO_HAL_QNAME_NAME        "Q_AUDIO_HAL_TASK"
#define AUDIO_HAL_STACK_SIZE        (2*1024)
#define AUDIO_HAL_QUEUE_NUM         (2*10)
#define AUDIO_HAL_TASK_PRIV         (PRI_AUDIO_HAL_TASK)  //maybe use high pirv(SMALL VAL).

#ifdef AUDIO_HAL_DEBUG
#define AUDIO_HAL_DBG       SCI_TRACE_LOW
#define AUDIO_HAL_DBG_ASS   SCI_ASSERT
#else
#define AUDIO_HAL_DBG(...)
#define AUDIO_HAL_DBG_ASS(x)
#endif

#define AUDIO_HAL_PRT       SCI_TRACE_LOW

#define AUDIO_HAL_CALL(cb,rt) do{                           \
    if( (cb) ){                                             \
        if( AUDIO_HAL_SUCCESS != (cb)() )                   \
            rt = AUDIO_HAL_ERROR;                           \
    }else{rt = AUDIO_HAL_ERROR;}                            \
}while(0)

#define AUDIO_HAL_CALL1(cb,param,rt) do{                    \
    if( (cb) ){                                             \
        if( AUDIO_HAL_SUCCESS != (cb)(param) )              \
            rt = AUDIO_HAL_ERROR;                           \
    }else{rt = AUDIO_HAL_ERROR;}                            \
}while(0)

#define AUDIO_HAL_CALL2(cb,parm1,parm2,rt) do{              \
    if( (cb) ){                                             \
        if( AUDIO_HAL_SUCCESS != (cb)(parm1,parm2) )        \
            rt = AUDIO_HAL_ERROR;                           \
    }else{rt = AUDIO_HAL_ERROR;}                            \
}while(0)

#define AUDIO_HAL_CALL3(cb,parm1,parm2,parm3,rt) do{        \
    if( (cb) ){                                             \
        if( AUDIO_HAL_SUCCESS != (cb)(parm1,parm2,parm3) )  \
            rt = AUDIO_HAL_ERROR;                           \
    }else{rt = AUDIO_HAL_ERROR;}                            \
}while(0)

#define AUDIO_HAL_IRQ_ENTRY() do{ SCI_DisableIRQ(); }while(0)
#define AUDIO_HAL_IRQ_LEAVE() do{ SCI_RestoreIRQ(); }while(0)

#define AUDIO_HAL_MUTEX_ENTRY(mutex)  do{\
    uint32 local_ret = SCI_ERROR;\
    if(SCI_InThreadContext())\
    {\
        if( NULL != mutex )\
        {\
            local_ret = SCI_GetMutex(mutex, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);\
        }\
    }\
    AUDIO_HAL_DBG_ASS( local_ret == SCI_SUCCESS );/*assert verified*/\
}while(0)

#define AUDIO_HAL_MUTEX_LEAVE(mutex)  do{\
    uint32 local_ret = SCI_ERROR;\
    if(SCI_InThreadContext())\
    {\
        if( NULL != mutex )\
        {\
            local_ret = SCI_PutMutex( mutex );\
        }\
    }\
    AUDIO_HAL_DBG_ASS( local_ret == SCI_SUCCESS );/*assert verified*/\
}while(0)



#define AUDIO_HAL_DSP_EVENT         (0x00000001)
#define AUDIO_HAL_DSP_WAIT_TIMEOUT  (5000) // 5 sec need complete.


#define AUDIO_HAL_CB_TASK 0 //1--> 0 for ref request.

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

enum AUDIO_HAL_TASK_MSG_E{
    AUDIO_HAL_REGISTER_CB   = 1,
    AUDIO_HAL_CALL_CB       = 2,
    AUDIO_HAL_CODEC_NOTIFY  = 4,
    AUDIO_HAL_UPDATE_LDO_VOL= 8,
    AUDIO_HAL_MAX_MSG
};

typedef struct
{
	SIGNAL_VARS
    AUDIO_HAL_CALLBACK_FUN  pFun;
    uint32                  param;
    uint32                  path;
}AUDIO_HAL_CALLBACK_T;

typedef struct
{
	SIGNAL_VARS
    uint32                  param;
}AUDIO_HAL_CODEC_NOTIFY_T;

typedef struct{
    char                    magic_str[AUDIO_MAGIC_STR_MAX_LEN];
    
    AUD_CODEC_OPS_T         *codec_ops;
    AUD_DAI_OPS_T           *dai_ops;
    uint32                  data_fmt;
    AUDIO_HAL_CALLBACK_FUN  cb[2];
    int16                   ref_count[32][2];

    SCI_MUTEX_PTR           mutex;
    BLOCK_ID                task_id;
    SCI_EVENT_GROUP_PTR     wait_dsp_event;
    uint32                  is_wait_dsp;
    
    uint32                  magic_num;
    uint32                  fs;
}AUDIO_HAL_T;

/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL AUDIO_HAL_T s_audio_hal;
/**----------------------------------------------------------------------------*
**                   LOCAL  Function Prototype                                **
**----------------------------------------------------------------------------*/
PUBLIC int32 AUDIO_HAL_notch_enable(uint32 enable);
LOCAL int32 _AUDIO_HAL_Parse_Fun_Flg(uint32 *fun_flg,uint32 oc);
LOCAL int32 _AUDIO_HAL_Get_Data_FMT(uint32 *data_fmt);
LOCAL int32 _AUDIO_HAL_Create_Task(void);
LOCAL int32 _AUDIO_HAL_Register_Msg(AUDIO_HAL_CALLBACK_FUN pFun,uint32 path);
LOCAL void _AUDIO_HAL_Update_LDO_VOL(
                              uint32    taskid,     // the client's task id
                              uint32    event_id,   // the event type
                              void      *param_ptr  // data follow the even
                              );

/**----------------------------------------------------------------------------*
**                           PUBLIC Function Prototype                        **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    Audio Driver Init.
//                  include Codec Init, DAI Init etc.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Init(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]init magic_num=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_186_112_2_17_23_2_35_29,(uint8*)"d",s_audio_hal.magic_num);
    if( AUDIO_MAGIC_NUM != s_audio_hal.magic_num )
    {
        SCI_MEMSET(&s_audio_hal,0,sizeof(s_audio_hal));
        s_audio_hal.magic_num = AUDIO_MAGIC_NUM;
        SCI_MEMCPY(s_audio_hal.magic_str,AUDIO_MAGIC_STR,AUDIO_MAGIC_STR_MAX_LEN);
        s_audio_hal.mutex = SCI_CreateMutex(AUDIO_HAL_MUTEX_NAME, SCI_INHERIT);
        AUDIO_HAL_DBG_ASS(s_audio_hal.mutex);/*assert verified*/

        if( 0==(s_audio_hal.dai_ops = AUDIO_DAI_Get_OPS(0)) )
        {
            ret = AUDIO_HAL_ERROR;
            //AUDIO_HAL_PRT:"[AUDDRV][HAL]Init DAI Failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_198_112_2_17_23_2_35_30,(uint8*)"");
        }
        else
        {
            AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_init,0,ret);
            _AUDIO_HAL_Get_Data_FMT(0);
            
        }
        
        if( 0==(s_audio_hal.codec_ops = AUDIO_CODEC_Get_OPS(0)) )
        {
            ret = AUDIO_HAL_ERROR;
            //AUDIO_HAL_PRT:"[AUDDRV][HAL]Init CODEC Failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_210_112_2_17_23_2_35_31,(uint8*)"");
        }
        else
        {
            AUDIO_HAL_CALL1(s_audio_hal.codec_ops->codec_init,0,ret);
        }
        if( AUDIO_HAL_SUCCESS != _AUDIO_HAL_Create_Task() )
        {
            ret = AUDIO_HAL_ERROR;
            AUDIO_HAL_DBG_ASS(0 && "Create Audio hal task Failed!");/*assert verified*/
        }
        
        if( 0==s_audio_hal.wait_dsp_event )
        {
            s_audio_hal.wait_dsp_event = SCI_CreateEvent("_WAIT_DSP_EVENT");
        }
#if !defined(FPGA_VERIFICATION) && !defined(MODEM_PLATFORM)
        //Register CHARGE event
        if( SCI_SUCCESS!= SCI_RegisterMsg( CHR_SERVICE, 
              (uint8)((CHR_CAP_IND)),(uint8)((CHR_CAP_IND)),_AUDIO_HAL_Update_LDO_VOL)
        )
        {
            ret = AUDIO_HAL_ERROR;
            AUDIO_HAL_DBG_ASS(0 && "Register Update Codec LDO VOL Failed!");/*assert verified*/
        }
#endif        
    }
    return ret;
}

/*****************************************************************************/
// Description :    Audio Driver Open.
//                  include Codec Power On, DAI Enable.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Open(uint32 fun_flg,uint32 fs)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 old_fun_flg = fun_flg;

    AUDIO_HAL_TICK_DBG("[AUDTICK] open %d",SCI_GetTickCount());

    //AUDIO_HAL_DBG:"[AUDDRV][HAL]open fun_flg=0x%x,fs=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_251_112_2_17_23_2_35_32,(uint8*)"dd",fun_flg,fs);
    s_audio_hal.fs=fs;
    AUDIO_HAL_DBG_ASS(fun_flg);/*assert verified*/

    AUDIO_HAL_MUTEX_ENTRY(s_audio_hal.mutex);
    
    _AUDIO_HAL_Parse_Fun_Flg(&fun_flg,AUDIO_OPEN);
    
    //AUDIO_HAL_PRT:"[AUDDRV][HAL]real open 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_259_112_2_17_23_2_35_33,(uint8*)"d",fun_flg);

    
    if( 0==s_audio_hal.dai_ops )
    {
        s_audio_hal.dai_ops = AUDIO_DAI_Get_OPS(0);
        SCI_ASSERT(s_audio_hal.dai_ops); /*assert verified*/
        AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_init,0,ret);
        _AUDIO_HAL_Get_Data_FMT(0);
    }
    AUDIO_HAL_CALL2(s_audio_hal.dai_ops->dai_open,fun_flg,fs,ret);
    if( AUDIO_HAL_SUCCESS!=ret )
    {
        //AUDIO_HAL_PRT:"[AUDDRV][HAL]open dai failed! 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_272_112_2_17_23_2_35_34,(uint8*)"d",ret);
        AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
        AUDIO_HAL_Close(old_fun_flg);
        return ret;
    }
    
    if( 0==s_audio_hal.codec_ops )
    {
        s_audio_hal.codec_ops = AUDIO_CODEC_Get_OPS(0);
        SCI_ASSERT(s_audio_hal.codec_ops); /*assert verified*/
        AUDIO_HAL_CALL1(s_audio_hal.codec_ops->codec_init,0,ret);
    }
    AUDIO_HAL_CALL2(s_audio_hal.codec_ops->codec_open,fun_flg,fs,ret);
    if( AUDIO_HAL_SUCCESS!=ret )
    {
        //AUDIO_HAL_PRT:"[AUDDRV][HAL]open codec failed! 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_287_112_2_17_23_2_35_35,(uint8*)"d",ret);
        AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
        AUDIO_HAL_Close(old_fun_flg);
        return ret;
    }

    if( (AUDIO_FUN_DSP & fun_flg) )
    {
        s_audio_hal.is_wait_dsp = 1;
        ret = __AUDIO_HAL_Wait_Event(s_audio_hal.wait_dsp_event,AUDIO_HAL_DSP_EVENT,AUDIO_HAL_DSP_WAIT_TIMEOUT);
        AUDIO_HAL_DBG_ASS(AUDIO_HAL_SUCCESS==ret);/*assert verified*/
        s_audio_hal.is_wait_dsp = 0;
    }
    AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);

    AUDIO_HAL_TICK_DBG("[AUDTICK] open end %d",SCI_GetTickCount());

    return ret;
}

/*****************************************************************************/
// Description :    Audio Driver Close.
//                  include Codec Power Off, DAI Disable.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Close(uint32 fun_flg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    int32 t_ret = AUDIO_HAL_SUCCESS;
    AUDIO_HAL_TICK_DBG("[AUDTICK] close %d",SCI_GetTickCount());
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]close fun_flg=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_315_112_2_17_23_2_35_36,(uint8*)"d",fun_flg);

    AUDIO_HAL_DBG_ASS(fun_flg);/*assert verified*/

    AUDIO_HAL_MUTEX_ENTRY(s_audio_hal.mutex);
    
    _AUDIO_HAL_Parse_Fun_Flg(&fun_flg,AUDIO_CLOSE);

    //AUDIO_HAL_PRT:"[AUDDRV][HAL]real close 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_323_112_2_17_23_2_35_37,(uint8*)"d",fun_flg);
    
    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_close,fun_flg,t_ret);
    if( AUDIO_HAL_SUCCESS!=t_ret )
    {
        //AUDIO_HAL_PRT:"[AUDDRV][HAL]close dai failed! 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_328_112_2_17_23_2_35_38,(uint8*)"d",ret);
        ret = t_ret;
    }
    
    AUDIO_HAL_CALL1(s_audio_hal.codec_ops->codec_close,fun_flg,t_ret);
    if( AUDIO_HAL_SUCCESS!=t_ret )
    {
        //AUDIO_HAL_PRT:"[AUDDRV][HAL]close codec failed! 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_335_112_2_17_23_2_35_39,(uint8*)"d",ret);
        ret = t_ret;
    }

    AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);

    AUDIO_HAL_TICK_DBG("[AUDTICK] close end %d",SCI_GetTickCount());
    
    return ret;
}

/*****************************************************************************/
// Description :    Audio Driver Ioctl.
//                  include Codec Ioctl, DAI Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use 0 if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]ioctl cmd=%d,arg=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_355_112_2_17_23_2_35_40,(uint8*)"dd",cmd,arg);
    AUDIO_HAL_DBG_ASS(cmd);/*assert verified*/
    
    //AUDIO_HAL_MUTEX_ENTRY(s_audio_hal.mutex);
    
    if( (cmd >= AUDIO_CTL_CODEC_BEGIN)
      &&(cmd < AUDIO_CTL_CODEC_END) )
    {
        AUDIO_HAL_CALL2(s_audio_hal.codec_ops->codec_ioctl,cmd,arg,ret);
    }
    else if( (cmd >= AUDIO_CTL_DAI_BEGIN)
      &&(cmd < AUDIO_CTL_DAI_END) )
    {
        AUDIO_HAL_CALL2(s_audio_hal.dai_ops->dai_ioctl,cmd,arg,ret);
        if( (AUDIO_CTL_SET_DATA_FMT==cmd) && (AUDIO_HAL_SUCCESS==ret) )
        {
            s_audio_hal.data_fmt = ((AUDIO_CTL_ARG_U*)(arg))->dwVal;
        }
    }
    else
    {
        AUDIO_HAL_DBG_ASS(0 && "ioctl cmd not defined");/*assert verified*/
    }
    
    //AUDIO_HAL_MUTEX_LEAVE(s_audio_hal.mutex);
    
    return ret;
}


/*****************************************************************************/
// Description :    Audio Driver Set Playback Buffer Info.
//                  
// Return :         AUDIO_HAL_SUCCESS
// Param :          pLHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Left channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR , the data start point.
//                  pRHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Right channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR , will ignore.
//                  cnt : the max size of pLHead. 
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_DAC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 cnt)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]dac_set_buf_info pLHead=0x%x,pRHead=0x%x,cnt=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_401_112_2_17_23_2_35_41,(uint8*)"ddd",pLHead,pRHead,cnt);
    AUDIO_HAL_DBG_ASS(pLHead);/*assert verified*/
    if( AUDIO_DATA_FMT_LL_RR == s_audio_hal.data_fmt )
    {
        AUDIO_HAL_DBG_ASS(pRHead);/*assert verified*/
    }
    AUDIO_HAL_DBG_ASS(cnt);/*assert verified*/
    
    AUDIO_HAL_CALL3(s_audio_hal.dai_ops->dai_dac_set_buf_info,pLHead,pRHead,cnt,ret);
    return ret;
}
PUBLIC int32 AUDIO_HAL_DAC_Set_Write_Point(uint32 pWrite)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_dac_set_write_point,pWrite,ret);
    return ret;
}
PUBLIC int32 AUDIO_HAL_DAC_Get_Read_Point(uint32* pRead)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_dac_get_read_point,pRead,ret);
    return ret;
}

/*****************************************************************************/
// Description :    Audio Driver Will Trigger Playback immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_DAC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //AUDIO_HAL_DBG("[AUDDRV][HAL]dac_trigger");
    AUDIO_HAL_CALL(s_audio_hal.dai_ops->dai_dac_trigger,ret);
    return ret;
}

/*****************************************************************************/
// Description :    Audio Driver Capture Buffer Info.
//                  
// Return :         AUDIO_HAL_SUCCESS
// Param :          pLHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Left channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR , the data start point.
//                  pRHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Right channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR , will ignore.
//                  cnt : the max size of pLHead. 
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_ADC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 cnt)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]adc_set_buf_info pLHead=0x%x,pRHead=0x%x,cnt=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_456_112_2_17_23_2_35_42,(uint8*)"ddd",pLHead,pRHead,cnt);
    AUDIO_HAL_DBG_ASS(pLHead);/*assert verified*/
    if( AUDIO_DATA_FMT_LL_RR == s_audio_hal.data_fmt )
    {
        AUDIO_HAL_DBG_ASS(pRHead);/*assert verified*/
    }
    AUDIO_HAL_DBG_ASS(cnt);/*assert verified*/
    
    AUDIO_HAL_CALL3(s_audio_hal.dai_ops->dai_adc_set_buf_info,pLHead,pRHead,cnt,ret);
    return ret;
}

PUBLIC int32 AUDIO_HAL_ADC_Set_Read_Point(uint32 pRead)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_adc_set_read_point,pRead,ret);
    return ret;
}

PUBLIC int32 AUDIO_HAL_ADC_Get_Write_Point(uint32* pWrite)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_adc_get_write_point,pWrite,ret);
    return ret;
}

/*****************************************************************************/
// Description :    Audio Driver Will Trigger Captrue immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_ADC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]adc_trigger"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_492_112_2_17_23_2_35_43,(uint8*)"");
    AUDIO_HAL_CALL(s_audio_hal.dai_ops->dai_adc_trigger,ret);
    return ret;
}

/*****************************************************************************/
// Description :    Audio Driver Register Playback Callback.
//                  Use to Notify User the Driver fetch data gone.
//                  User can do something like start decode audio data.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Register_Playback(AUDIO_HAL_CALLBACK_FUN pFun)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]register_playback 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_509_112_2_17_23_2_35_44,(uint8*)"d",pFun);
    _AUDIO_HAL_Register_Msg(pFun,AUDIO_PLAYBACK);
    return ret;
}

/*****************************************************************************/
// Description :    Audio Driver Register Capture Callback.
//                  Use to Notify User for fetch data please.
//                  User should be move audio data ASAP.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
PUBLIC int32 AUDIO_HAL_Register_Capture(AUDIO_HAL_CALLBACK_FUN pFun)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]register_capture 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_526_112_2_17_23_2_36_45,(uint8*)"d",pFun);
    _AUDIO_HAL_Register_Msg(pFun,AUDIO_CAPTRUE);
    return ret;
}


/**----------------------------------------------------------------------------*
**                           LOCAL Function Prototype                         **
**----------------------------------------------------------------------------*/

LOCAL uint32 _AUDIO_HAL_Inc(uint32 *fun_flg,uint32 path,uint32 index)
{
    uint32 ret = 0;
    AUDIO_HAL_IRQ_ENTRY();
    if( s_audio_hal.ref_count[index][path] )
    {
        *fun_flg &= ~((uint32)(1<<index));
        ret = 1;
    }
    (s_audio_hal.ref_count[index][path])++;
    AUDIO_HAL_IRQ_LEAVE();
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]inc 0x%x,0x%x,ref %d,ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_547_112_2_17_23_2_36_46,(uint8*)"dddd",*fun_flg,path,(s_audio_hal.ref_count[index][path]),ret);
    return ret;
}

LOCAL uint32 _AUDIO_HAL_Dec(uint32 *fun_flg,uint32 path,uint32 index)
{
    uint32 ret = 0;
    AUDIO_HAL_IRQ_ENTRY();
    if( s_audio_hal.ref_count[index][path]>0 )
    {
        (s_audio_hal.ref_count[index][path])--;
    }
    if( s_audio_hal.ref_count[index][path] )
    {
        *fun_flg &= ~((uint32)(1<<index));
        ret = 1;
    }
    AUDIO_HAL_IRQ_LEAVE();
    //AUDIO_HAL_DBG:"[AUDDRV][HAL]dec 0x%x,0x%x,ref %d,ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_566_112_2_17_23_2_36_47,(uint8*)"dddd",*fun_flg,path,(s_audio_hal.ref_count[index][path]),ret);
    return ret;
}

LOCAL void _AUDIO_HAL_Refcount(uint32 *fun_flg,uint32 index,uint32 oc)
{
    uint32 fun_deal= (*fun_flg);
    if( AUDIO_OPEN==oc )
    {
        if(((uint32)AUDIO_FUN_PLAYBACK) & fun_deal)
        {
            if( _AUDIO_HAL_Inc(fun_flg,AUDIO_PLAYBACK,index) )
            {
                *fun_flg &= ~(AUDIO_FUN_PLAYBACK);
            }
        }
        /*DO NOT dispel the function bit*/
        if( *fun_flg & (AUDIO_FUN_CAPTRUE) )
        {
            *fun_flg |= (fun_deal & (~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE)));
        }
        if(((uint32)AUDIO_FUN_CAPTRUE) & fun_deal)
        {
            if( _AUDIO_HAL_Inc(fun_flg,AUDIO_CAPTRUE,index) )
            {
                *fun_flg &= ~(AUDIO_FUN_CAPTRUE);
            }
        }
    }
    else if( AUDIO_CLOSE==oc )
    {
        if(((uint32)AUDIO_FUN_PLAYBACK) & fun_deal)
        {
            if( _AUDIO_HAL_Dec(fun_flg,AUDIO_PLAYBACK,index) )
            {
                *fun_flg &= ~(AUDIO_FUN_PLAYBACK);
            }
        }
        /*DO NOT dispel the function bit*/
        if( *fun_flg & (AUDIO_FUN_CAPTRUE) )
        {
            *fun_flg |= (fun_deal & (~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE)));
        }
        if(((uint32)AUDIO_FUN_CAPTRUE) & fun_deal)
        {
            if( _AUDIO_HAL_Dec(fun_flg,AUDIO_CAPTRUE,index) )
            {
                *fun_flg &= ~(AUDIO_FUN_CAPTRUE);
            }
        }
    }
    /*DO NOT dispel the function bit*/
    if( *fun_flg & (AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE) )
    {
        *fun_flg |= (fun_deal & (~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE)));
    }
}

/*****************************************************************************/
// Description :    open/close will be use ref count.
//                  if some fun opening ,and ref count not equal 0,inc ref count,
//           then opened,else need call real open fun,and then inc ref count.
//                  if some fun closing ,that dec ref count first,
//           if ref count not equal 0,then closed,else need call real close fun.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _AUDIO_HAL_Parse_Fun_Flg(uint32 *fun_flg,uint32 oc)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    int i;
    uint32 fun_deal= (*fun_flg);
    if( (~(uint32)(AUDIO_FUN_PLAYBACK | AUDIO_FUN_CAPTRUE)) & fun_deal )
    {//DSP/LINEIN/CUSTOM etc FUN.
        for(i=0;i<AUDIO_FUN_MAX_CNT;i++)
        {
            if( fun_deal & ((uint32)(1<<i)) )
            {
                _AUDIO_HAL_Refcount(fun_flg,i,oc);
            }
        }
    }
    else
    {
        for(i=AUDIO_FUN_MAX_CNT;i<32;i++)
        {
            if( fun_deal & ((uint32)(1<<i)) )
            {
                _AUDIO_HAL_Refcount(fun_flg,i,oc);
            }
        }
    }
    return ret;
}


LOCAL int32 _AUDIO_HAL_Get_Data_FMT(uint32 * data_fmt)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( 0==s_audio_hal.data_fmt )
    {
        AUDIO_CTL_ARG_U ioctl_arg={0};
        AUDIO_HAL_CALL2(s_audio_hal.dai_ops->dai_ioctl,AUDIO_CTL_GET_DATA_FMT,(uint32)&ioctl_arg,ret);
        if( AUDIO_HAL_SUCCESS==ret )
        {
            s_audio_hal.data_fmt = ioctl_arg.dwVal;
        }
    }
    if(&s_audio_hal.data_fmt != data_fmt)
    {
        if( data_fmt )
        {
            *data_fmt = s_audio_hal.data_fmt;
        }
    }
    return ret;
}


LOCAL int32 _AUDIO_HAL_Send_Msg(uint32 msg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    xSignalHeader   signal = 0;

    SCI_CREATE_SIGNAL(
        signal,
        msg,
        sizeof(*signal),
        SCI_IDENTIFY_THREAD());

    if( signal ) //lint !e774
    {
        SCI_SEND_SIGNAL(signal,s_audio_hal.task_id);
    }
    else
    {
        ret = AUDIO_HAL_ERROR;
    }

    return ret;
}


LOCAL int32 _AUDIO_HAL_CB_Send_Msg(uint32 msg
    ,AUDIO_HAL_CALLBACK_FUN pFun,uint32 path,uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_HAL_CALLBACK_T   *signal = 0;

    SCI_CREATE_SIGNAL(
        signal,
        msg,
        sizeof(*signal),
        SCI_IDENTIFY_THREAD());

    if( signal ) //lint !e774
    {
        signal->pFun    = pFun;
        signal->param   = param;
        signal->path    = path;
        SCI_SEND_SIGNAL(signal,s_audio_hal.task_id);
    }
    else
    {
        ret = AUDIO_HAL_ERROR;
    }

    return ret;
}

LOCAL int32 _AUDIO_HAL_Register(uint32 path);


LOCAL int32 _AUDIO_HAL_Register_Msg(AUDIO_HAL_CALLBACK_FUN pFun,uint32 path)
{
#if (AUDIO_HAL_CB_TASK)
    return _AUDIO_HAL_CB_Send_Msg(AUDIO_HAL_REGISTER_CB,pFun,path,0);
#else
    int32 ret = AUDIO_HAL_SUCCESS;
    s_audio_hal.cb[(path)] = pFun;
    if( pFun )
    {
        ret = _AUDIO_HAL_Register((path));
    }
    return ret;
#endif
}

LOCAL int32 _AUDIO_HAL_Call_Msg(uint32 path,uint32 param)
{
#if (AUDIO_HAL_CB_TASK)
    return _AUDIO_HAL_CB_Send_Msg(AUDIO_HAL_CALL_CB,0,path,param);
#else
    int32 ret = AUDIO_HAL_SUCCESS;
    if( s_audio_hal.cb[(path)] )
    {
        ret = s_audio_hal.cb[(path)]((param));
    }
    return ret;
#endif
}


LOCAL int32 _AUDIO_HAL_Playback_CB(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    ret = _AUDIO_HAL_Call_Msg(AUDIO_PLAYBACK,param);
    return ret;
}

LOCAL int32 _AUDIO_HAL_Capture_CB(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    ret = _AUDIO_HAL_Call_Msg(AUDIO_CAPTRUE,param);
    return ret;
}

PUBLIC int32 __AUDIO_HAL_Codec_Notify_Msg(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_HAL_CODEC_NOTIFY_T   *signal = 0;

    if( (param == AUDIO_HAL_CODEC_NONE )
     || (param >= AUDIO_HAL_CODEC_MAX )
    )
    {
        return AUDIO_HAL_ERROR;
    }

    SCI_CREATE_SIGNAL(
        signal,
        AUDIO_HAL_CODEC_NOTIFY,
        sizeof(*signal),
        SCI_IDENTIFY_THREAD());

    if( signal ) //lint !e774
    {
        signal->param   = param;
        SCI_SEND_SIGNAL(signal,s_audio_hal.task_id);
    }
    else
    {
        ret = AUDIO_HAL_ERROR;
    }

    return ret;
}

LOCAL void _AUDIO_HAL_thread (uint32 argc, void *argv);

LOCAL int32 _AUDIO_HAL_Create_Task(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    static uint32 do_this = 1;
    if( do_this )
    {
        s_audio_hal.task_id  = SCI_CreateThread(AUDIO_HAL_TASK_NAME,
            AUDIO_HAL_QNAME_NAME,
            _AUDIO_HAL_thread,
            0,
            0,
            AUDIO_HAL_STACK_SIZE ,
            AUDIO_HAL_QUEUE_NUM,
            AUDIO_HAL_TASK_PRIV,
            SCI_PREEMPT,
            SCI_AUTO_START);
        do_this = 0;
    }
    if( 0==s_audio_hal.task_id )
    {
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _AUDIO_HAL_Register(uint32 path)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( AUDIO_PLAYBACK == path )
    {
        AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_register_playback,_AUDIO_HAL_Playback_CB,ret);
    }
    else if( AUDIO_CAPTRUE == path )
    {
        AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_register_capture,_AUDIO_HAL_Capture_CB,ret);
    }
    return ret;
}

LOCAL void _AUDIO_HAL_thread (uint32 argc, void *argv)
{
    xSignalHeader	signal;

    for(;;)
    {
        SCI_RECEIVE_SIGNAL(signal, s_audio_hal.task_id);
        switch(signal->SignalCode)
        {
#if (AUDIO_HAL_CB_TASK)
            case AUDIO_HAL_REGISTER_CB:
            {
                AUDIO_HAL_CALLBACK_T *cb = (AUDIO_HAL_CALLBACK_T *)signal;
                if( (AUDIO_PLAYBACK == (cb->path)) || (AUDIO_CAPTRUE == (cb->path)) )
                {
                    s_audio_hal.cb[(cb->path)] = cb->pFun;
                    if( cb->pFun )
                    {
                        _AUDIO_HAL_Register((cb->path));
                    }
                }
            }break;
            
            case AUDIO_HAL_CALL_CB:
            {
                AUDIO_HAL_CALLBACK_T *cb = (AUDIO_HAL_CALLBACK_T *)signal;
                //AUDIO_HAL_DBG("[AUDDRV][HAL]cb %d,0x%x",cb->path,s_audio_hal.cb[(cb->path)]);
                if( (AUDIO_PLAYBACK == (cb->path)) || ( AUDIO_CAPTRUE == (cb->path) ))
                {
                    if( s_audio_hal.cb[(cb->path)] )
                    {
                        s_audio_hal.cb[(cb->path)]((cb->param));
                    }
                }
            }break;
#endif
            case AUDIO_HAL_CODEC_NOTIFY:
            {
                int32 ret;
                AUDIO_HAL_CODEC_NOTIFY_T *op = (AUDIO_HAL_CODEC_NOTIFY_T *)signal;
                //AUDIO_HAL_DBG:"[AUDDRV][HAL]codec notify %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_883_112_2_17_23_2_36_48,(uint8*)"d",op->param);
                AUDIO_HAL_CALL1(s_audio_hal.dai_ops->dai_codec_notify,op->param,ret);
                if( s_audio_hal.is_wait_dsp && (AUDIO_HAL_CODEC_OPENED==op->param) )
                {
                    __AUDIO_HAL_Set_Event(s_audio_hal.wait_dsp_event,AUDIO_HAL_DSP_EVENT);
                }
            }break;
            case AUDIO_HAL_UPDATE_LDO_VOL:
            {
                int32 ret;
                CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
                //AUDIO_HAL_DBG("[AUDDRV][HAL]codec update ldo");
                ret = AUDIO_HAL_Ioctl(AUDIO_UPDTAE_LDO_VOL,p_chgmng_info->bat_cur_vol);
            }break;
            ///TODO: add msg here!
            default:
            {
                SCI_PASSERT (0,("Error msg!!!"));   /*assert verified*/
            }break;

        }
        SCI_FREE_SIGNAL(signal);
        signal = NULL;
    }
}

LOCAL void _AUDIO_HAL_Update_LDO_VOL(
                              uint32    taskid,     // the client's task id
                              uint32    event_id,   // the event type
                              void      *param_ptr  // data follow the even
                              )
{
    switch (event_id)
    {
        case CHR_CAP_IND :
        {
            //AUDIO_HAL_DBG:"AUDIO CODEC CHR_CAP_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_HAL_919_112_2_17_23_2_36_49,(uint8*)"");
            _AUDIO_HAL_Send_Msg(AUDIO_HAL_UPDATE_LDO_VOL);
        }break;
        default:break;
    }
}
    
PUBLIC int32 __AUDIO_HAL_Wait_Event(SCI_EVENT_GROUP_PTR h_event,uint32 event,uint32 timeval)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 actual_event;
    if( h_event )
    {
        ret = SCI_GetEvent(h_event,
            event,
            SCI_OR_CLEAR,
            &actual_event,
            timeval /*SCI_WAIT_FOREVER*/);
        if( SCI_SUCCESS != ret )
        {
            ret = AUDIO_HAL_ERROR;
        }
    }
    return ret;
}

PUBLIC void __AUDIO_HAL_Set_Event(SCI_EVENT_GROUP_PTR h_event,uint32 event)
{
    if( h_event )
    {
        SCI_SetEvent(h_event, event, SCI_OR);
    }
}

PUBLIC int32 AUDIO_HAL_notch_enable(uint32 enable)
{
    int32 ret=0;
    if(s_audio_hal.dai_ops->dai_notch_enable)
    {
        s_audio_hal.dai_ops->dai_notch_enable(enable,s_audio_hal.fs);
    }
    return ret;
}

PUBLIC void AUDIO_HAL_SetNothpara(NothPara *para)
{
    s_audio_hal.dai_ops->dai_notch_setpara(para);
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
