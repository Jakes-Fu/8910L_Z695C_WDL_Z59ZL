/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _CP_PWRCTRL_ASM_ARM_H_
#define _CP_PWRCTRL_ASM_ARM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !



//==============================================================================
// cp_pwrctrl
//------------------------------------------------------------------------------
///
//==============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_PWRCTRL_BASE           0x050A0000
#else
#define REG_PWRCTRL_BASE           0x500A0000
#endif

#define REG_PWRCTRL_PWR_HWEN          0x00000000
#define REG_PWRCTRL_AP_PWR_CTRL       0x00000004
#define REG_PWRCTRL_ZSP_PWR_CTRL      0x00000008
#define REG_PWRCTRL_LTE_PWR_CTRL      0x0000000C
#define REG_PWRCTRL_GGE_PWR_CTRL      0x00000010
#define REG_PWRCTRL_RF_PWR_CTRL       0x00000014
#define REG_PWRCTRL_BTFM_PWR_CTRL     0x00000018
#define REG_PWRCTRL_AON_LP_PWR_CTRL    0x0000001C
#define REG_PWRCTRL_AP_PWR_STAT       0x00000020
#define REG_PWRCTRL_ZSP_PWR_STAT      0x00000024
#define REG_PWRCTRL_LTE_PWR_STAT      0x00000028
#define REG_PWRCTRL_GGE_PWR_STAT      0x0000002C
#define REG_PWRCTRL_RF_PWR_STAT       0x00000030
#define REG_PWRCTRL_BTFM_PWR_STAT     0x00000034
#define REG_PWRCTRL_AON_LP_PWR_STAT    0x00000038
#define REG_PWRCTRL_STATE_DELAY       0x0000003C
#define REG_PWRCTRL_PREPGC_DELAY      0x00000040
#define REG_PWRCTRL_ALLPGC_DELAY      0x00000044
#define REG_PWRCTRL_DDR_HOLD_CTRL     0x00000048
#define REG_PWRCTRL_ZSP_PD_POLL       0x0000004C
#define REG_PWRCTRL_ZSP_PD_POLL_SET    0x00000050
#define REG_PWRCTRL_ZSP_PD_POLL_CLR    0x00000054
#define REG_PWRCTRL_WCN_LPS           0x00000090
#define REG_PWRCTRL_ARM_SLP_REQ_SW    0x00000094
#define REG_PWRCTRL_ARM_SLP_ACK       0x00000098
#define REG_PWRCTRL_ARM_SLP_REQ_HWEN    0x0000009C
#define REG_PWRCTRL_ZSP_SLP_REQ_SW    0x000000A0
#define REG_PWRCTRL_ZSP_SLP_ACK       0x000000A4
#define REG_PWRCTRL_ZSP_SLP_REQ_HWEN    0x000000A8
#define REG_PWRCTRL_DDR_SLP_REQ_SW    0x000000AC
#define REG_PWRCTRL_DDR_SLP_ACK       0x000000B0
#define REG_PWRCTRL_DDR_SLP_REQ_HWEN    0x000000B4
#define REG_PWRCTRL_TIMEOUT_FLAG      0x000000B8
#define REG_PWRCTRL_POWER_STATE       0x000000BC
#define REG_PWRCTRL_PWRCTRL_MODE      0x000000C0
#define REG_PWRCTRL_PWRCTRL_SW        0x000000C4
#define REG_PWRCTRL_PWRCTRL_SW_SET    0x000000C8
#define REG_PWRCTRL_PWRCTRL_SW_CLR    0x000000CC
#define REG_PWRCTRL_PWRCTRL_SW1       0x000000D0
#define REG_PWRCTRL_PWRCTRL_SW1_SET    0x000000D4
#define REG_PWRCTRL_PWRCTRL_SW1_CLR    0x000000D8

//PWR_HWEN
#define CP_PWRCTRL_RESERVED_0(n)   (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_AON_LP_PON_EN   (1<<6)
#define CP_PWRCTRL_BTFM_PON_EN     (1<<5)
#define CP_PWRCTRL_RF_PON_EN       (1<<4)
#define CP_PWRCTRL_GGE_PON_EN      (1<<3)
#define CP_PWRCTRL_LTE_PON_EN      (1<<2)
#define CP_PWRCTRL_ZSP_PON_EN      (1<<1)
#define CP_PWRCTRL_AP_PWR_EN       (1<<0)

//AP_PWR_CTRL
#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_AP_PON          (1<<1)
#define CP_PWRCTRL_AP_POFF         (1<<0)

//ZSP_PWR_CTRL
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_ZSP_PON         (1<<1)
#define CP_PWRCTRL_ZSP_POFF        (1<<0)

//LTE_PWR_CTRL
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_LTE_PON         (1<<1)
#define CP_PWRCTRL_LTE_POFF        (1<<0)

//GGE_PWR_CTRL
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_GGE_PON         (1<<1)
#define CP_PWRCTRL_GGE_POFF        (1<<0)

//RF_PWR_CTRL
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_RF_PON          (1<<1)
#define CP_PWRCTRL_RF_POFF         (1<<0)

//BTFM_PWR_CTRL
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_BTFM_PON        (1<<1)
#define CP_PWRCTRL_BTFM_POFF       (1<<0)

//AON_LP_PWR_CTRL
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_AON_LP_PON      (1<<1)
#define CP_PWRCTRL_AON_LP_POFF     (1<<0)

//AP_PWR_STAT
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_AP_STABLE       (1<<1)
#define CP_PWRCTRL_AP_PSTAT        (1<<0)

//ZSP_PWR_STAT
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_ZSP_STABLE      (1<<1)
#define CP_PWRCTRL_ZSP_PSTAT       (1<<0)

//LTE_PWR_STAT
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_LTE_STABLE      (1<<1)
#define CP_PWRCTRL_LTE_PSTAT       (1<<0)

//GGE_PWR_STAT
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_GGE_STABLE      (1<<1)
#define CP_PWRCTRL_GGE_PSTAT       (1<<0)

//RF_PWR_STAT
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_RF_STABLE       (1<<1)
#define CP_PWRCTRL_RF_PSTAT        (1<<0)

//BTFM_PWR_STAT
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_BTFM_STABLE     (1<<1)
#define CP_PWRCTRL_BTFM_PSTAT      (1<<0)

//AON_LP_PWR_STAT
//#define CP_PWRCTRL_RESERVED_0_X1(n) (((n)&0x3FFFFFFF)<<2)
#define CP_PWRCTRL_AON_LP_STABLE   (1<<1)
#define CP_PWRCTRL_AON_LP_PSTAT    (1<<0)

//STATE_DELAY
#define CP_PWRCTRL_RESERVED_0_X2(n) (((n)&0xFFFFFF)<<8)
#define CP_PWRCTRL_DELAY(n)        (((n)&0xFF)<<0)

//PREPGC_DELAY
#define CP_PWRCTRL_RESERVED_0_X3(n) (((n)&0xFFFF)<<16)
#define CP_PWRCTRL_DELAY_X1(n)     (((n)&0xFFFF)<<0)

//ALLPGC_DELAY
//#define CP_PWRCTRL_RESERVED_0_X3(n) (((n)&0xFFFF)<<16)
//#define CP_PWRCTRL_DELAY_X1(n)   (((n)&0xFFFF)<<0)

//DDR_HOLD_CTRL
#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_HOLD_CTRL       (1<<0)

//ZSP_PD_POLL
#define CP_PWRCTRL_RESERVED_0_X5(n) (((n)&0x1FFFFFFF)<<3)
#define CP_PWRCTRL_ZSP_POLL        (1<<2)
#define CP_PWRCTRL_CP_POLL         (1<<1)
#define CP_PWRCTRL_AP_POLL         (1<<0)

//ZSP_PD_POLL_SET
//#define CP_PWRCTRL_RESERVED_0_X5(n) (((n)&0x1FFFFFFF)<<3)
#define CP_PWRCTRL_ZSPPOLLSET(n)   (((n)&7)<<0)

//ZSP_PD_POLL_CLR
//#define CP_PWRCTRL_RESERVED_0_X5(n) (((n)&0x1FFFFFFF)<<3)
#define CP_PWRCTRL_ZSPPOLLCLR(n)   (((n)&7)<<0)

//WCN_LPS
#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0xFFFFFFF)<<4)
#define CP_PWRCTRL_WCN2SYS_SLEEP   (1<<3)
#define CP_PWRCTRL_WCN2SYS_OSC_EN  (1<<2)
#define CP_PWRCTRL_WCN2SYS_WAKEUP  (1<<1)
#define CP_PWRCTRL_SYS2WCN_WAKEUP  (1<<0)

//ARM_SLP_REQ_SW
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_ARMSLPREQ       (1<<0)

//ARM_SLP_ACK
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_ARMSLPACK       (1<<0)

//ARM_SLP_REQ_HWEN
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_ARMSLPREQ_HWEN  (1<<0)

//ZSP_SLP_REQ_SW
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_ZSPSLPREQ       (1<<0)

//ZSP_SLP_ACK
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_ZSPSLPACK       (1<<0)

//ZSP_SLP_REQ_HWEN
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_ZSPSLPREQ_HWEN  (1<<0)

//DDR_SLP_REQ_SW
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_DDRSLPREQ       (1<<0)

//DDR_SLP_ACK
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_DDRSLPACK       (1<<0)

//DDR_SLP_REQ_HWEN
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_DDRSLPREQ_HWEN  (1<<0)

//TIMEOUT_FLAG
//#define CP_PWRCTRL_RESERVED_0_X5(n) (((n)&0x1FFFFFFF)<<3)
#define CP_PWRCTRL_DDRSLPTIMEOUT   (1<<2)
#define CP_PWRCTRL_ZSPBUSSLPTIMEOUT (1<<1)
#define CP_PWRCTRL_ARMBUSSLPTIMEOUT (1<<0)

//POWER_STATE
#define CP_PWRCTRL_COREPOWERSTATE(n) (((n)&15)<<28)
#define CP_PWRCTRL_AONLPPOWERSTATE(n) (((n)&15)<<24)
#define CP_PWRCTRL_BTFMPOWERSTATE(n) (((n)&15)<<20)
#define CP_PWRCTRL_RFPOWERSTATE(n) (((n)&15)<<16)
#define CP_PWRCTRL_GGEPOWERSTATE(n) (((n)&15)<<12)
#define CP_PWRCTRL_LTEPOWERSTATE(n) (((n)&15)<<8)
#define CP_PWRCTRL_ZSPPOWERSTATE(n) (((n)&15)<<4)
#define CP_PWRCTRL_ARMPOWERSTATE(n) (((n)&15)<<0)

//PWRCTRL_MODE
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_PWR_MODE        (1<<0)

//PWRCTRL_SW
#define CP_PWRCTRL_RESERVED_0_X7(n) (((n)&3)<<30)
#define CP_PWRCTRL_BTFM_PWR_CTRL   (1<<29)
#define CP_PWRCTRL_BTFM_PWR_CTRL_PRE (1<<28)
#define CP_PWRCTRL_BTFM_HOLD       (1<<27)
#define CP_PWRCTRL_BTFM_RST_CTRL   (1<<26)
#define CP_PWRCTRL_BTFM_CLK_CTRL   (1<<25)
#define CP_PWRCTRL_RF_PWR_CTRL     (1<<24)
#define CP_PWRCTRL_RF_PWR_CTRL_PRE (1<<23)
#define CP_PWRCTRL_RF_HOLD         (1<<22)
#define CP_PWRCTRL_RF_RST_CTRL     (1<<21)
#define CP_PWRCTRL_RF_CLK_CTRL     (1<<20)
#define CP_PWRCTRL_GGE_PWR_CTRL    (1<<19)
#define CP_PWRCTRL_GGE_PWR_CTRL_PRE (1<<18)
#define CP_PWRCTRL_GGE_HOLD        (1<<17)
#define CP_PWRCTRL_GGE_RST_CTRL    (1<<16)
#define CP_PWRCTRL_GGE_CLK_CTRL    (1<<15)
#define CP_PWRCTRL_LTE_PWR_CTRL    (1<<14)
#define CP_PWRCTRL_LTE_PWR_CTRL_PRE (1<<13)
#define CP_PWRCTRL_LTE_HOLD        (1<<12)
#define CP_PWRCTRL_LTE_RST_CTRL    (1<<11)
#define CP_PWRCTRL_LTE_CLK_CTRL    (1<<10)
#define CP_PWRCTRL_ZSP_PWR_CTRL    (1<<9)
#define CP_PWRCTRL_ZSP_PWR_CTRL_PRE (1<<8)
#define CP_PWRCTRL_ZSP_HOLD        (1<<7)
#define CP_PWRCTRL_ZSP_RST_CTRL    (1<<6)
#define CP_PWRCTRL_ZSP_CLK_CTRL    (1<<5)
#define CP_PWRCTRL_AP_PWR_CTRL     (1<<4)
#define CP_PWRCTRL_AP_PWR_CTRL_PRE (1<<3)
#define CP_PWRCTRL_AP_HOLD         (1<<2)
#define CP_PWRCTRL_AP_RST_CTRL     (1<<1)
#define CP_PWRCTRL_AP_CLK_CTRL     (1<<0)

//PWRCTRL_SW_SET
//#define CP_PWRCTRL_RESERVED_0_X7(n) (((n)&3)<<30)
#define CP_PWRCTRL_SET(n)          (((n)&0x3FFFFFFF)<<0)

//PWRCTRL_SW_CLR
//#define CP_PWRCTRL_RESERVED_0_X7(n) (((n)&3)<<30)
#define CP_PWRCTRL_CLEAR(n)        (((n)&0x3FFFFFFF)<<0)

//PWRCTRL_SW1
#define CP_PWRCTRL_RESERVED_0_X8(n) (((n)&0x7FFFFFF)<<5)
#define CP_PWRCTRL_AON_LP_PWR_CTRL (1<<4)
#define CP_PWRCTRL_AON_LP_PWR_CTRL_PRE (1<<3)
#define CP_PWRCTRL_AON_LP_HOLD     (1<<2)
#define CP_PWRCTRL_AON_LP_RST_CTRL (1<<1)
#define CP_PWRCTRL_AON_LP_CLK_CTRL (1<<0)

//PWRCTRL_SW1_SET
//#define CP_PWRCTRL_RESERVED_0_X8(n) (((n)&0x7FFFFFF)<<5)
#define CP_PWRCTRL_SET_X1(n)       (((n)&31)<<0)

//PWRCTRL_SW1_CLR
//#define CP_PWRCTRL_RESERVED_0_X8(n) (((n)&0x7FFFFFF)<<5)
#define CP_PWRCTRL_CLEAR_X1(n)     (((n)&31)<<0)




#endif
