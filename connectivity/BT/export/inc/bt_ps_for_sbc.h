/******************************************************************************
** File Name:      bt_ps_for_sbc.h
** Author:
** DATE:           2012/1/4
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.
** Description:
******************************************************************************/

#ifndef _BT_PS_FOR_SBC_H
#define _BT_PS_FOR_SBC_H

#include "bt_sbc_pal.h"
#include "a2dp_api.h"

extern uint16 A2DP_SbcConfig(SbcChannelMode_t channel_mode,
                          SbcAllocMethod_t alloc_method,
                          uint16 sample_freq,
                          uint8 nrof_blocks,
                          uint8 nrof_subbands,
                          uint8 bitpool);


extern uint8 SBC_CalcBitPool(uint8 *bitPoolAlt,
                              uint8 *togglePeriod,
                              SbcChannelMode_t channel_mode,
                              uint16 sample_freq,
                              uint8 nrof_blocks,
                              uint8 nrof_subbands,
                              uint16 bitrate);

#endif
