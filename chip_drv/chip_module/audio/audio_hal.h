/******************************************************************************
 ** File Name:      audio_hal.h                                               *
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

#ifndef _AUDIO_HAL_H_
#define _AUDIO_HAL_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"
#include "vb_drvapi.h"

#include "task_monitor.h"

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
#define AUDIO_MAGIC_STR_MAX_LEN     (10)
#define AUDIO_MAGIC_STR             "AUDIO_HAL"
#define AUDIO_MAGIC_NUM             (0x41554449)

#define TEST_POINT_FLG  (2124785061) //0x7EA5A5A5
#define AUDIO_HAL_PRTMEM(P)   TM_SendTestPointRequest(P,TEST_POINT_FLG)

// for debug load time.
//#define AUDIO_HAL_TICK_DBG    SCI_TRACE_LOW
#define AUDIO_HAL_TICK_DBG(...)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
enum AUDIO_HAL_OC_E{
    AUDIO_CLOSE = 0,
    AUDIO_OPEN
};

enum AUDIO_HAL_CODEC_NOTIFY_E{
//CODEC NOTIFY
    AUDIO_HAL_CODEC_NONE        =   (0),
    AUDIO_HAL_CODEC_OPENED,
    AUDIO_HAL_CODEC_CLOSEED,
    AUDIO_HAL_CODEC_SEND_READY,
    AUDIO_HAL_CODEC_SEND_Z,
    
    AUDIO_HAL_CODEC_MAX
};

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

PUBLIC void VB_PHY_HPFSwitch (BOOLEAN is_enable);
PUBLIC void VB_PHY_SetHPFMode (BOOLEAN is_reset_mode);
PUBLIC void VB_PHY_SetHPFParas (
    int16 filter_index,
    int16 B0,
    int16 B1,
    int16 B2,
    int16 A0,
    int16 minusA1,
    int16 minusA2
);

PUBLIC void VB_PHY_SetHPFGain (
    int16 gain_index,//gain index:0,1,2,3,4,5,6
    int16 gain_value
);

PUBLIC uint32 VB_PHY_GetHPFGain (
    int16 gain_index
);
PUBLIC void VB_PHY_SetHPFLimit (
    int8 rLimit //0~127
);
PUBLIC void VB_PHY_SetHPFWidth (
    uint16  width //width = 16 or 24 (bits)
);

PUBLIC void VB_PHY_ALCSwitch (
    BOOLEAN is_enable
);

PUBLIC void VB_PHY_SetALCParas (
    VBC_ALC_PARAS_T *ptAlcPara
);


PUBLIC void VB_PHY_SetFMMixMode (
    int16 da_index,//0,1
    int16 mix_mode  //0,1,2
);

PUBLIC void VB_PHY_DGSwitch (
    int16 da_index,//0,1
    BOOLEAN is_enable
);


PUBLIC void VB_PHY_SetDG (
    int16 da_index,//0,1
    int16 dg_value
);


PUBLIC int32    __AUDIO_HAL_Codec_Notify_Msg(uint32 param);
PUBLIC int32    __AUDIO_HAL_Wait_Event(SCI_EVENT_GROUP_PTR h_event,uint32 event,uint32 timeval);
PUBLIC void     __AUDIO_HAL_Set_Event(SCI_EVENT_GROUP_PTR h_event,uint32 event);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_AUDIO_HAL_H_
// End
