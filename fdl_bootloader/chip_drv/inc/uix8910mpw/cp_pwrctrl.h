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


#ifndef _CP_PWRCTRL_H_
#define _CP_PWRCTRL_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'cp_pwrctrl'."
#endif

#include "global_macros.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CP_PWRCTRL_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_PWRCTRL_BASE           0x050A0000
#else
#define REG_PWRCTRL_BASE           0x500A0000
#endif

typedef volatile struct
{
    REG32                          PWR_HWEN;                     //0x00000000
    REG32                          AP_PWR_CTRL;                  //0x00000004
    REG32                          ZSP_PWR_CTRL;                 //0x00000008
    REG32                          LTE_PWR_CTRL;                 //0x0000000C
    REG32                          GGE_PWR_CTRL;                 //0x00000010
    REG32                          RF_PWR_CTRL;                  //0x00000014
    REG32                          BTFM_PWR_CTRL;                //0x00000018
    REG32                          AP_PWR_STAT;                  //0x0000001C
    REG32                          ZSP_PWR_STAT;                 //0x00000020
    REG32                          LTE_PWR_STAT;                 //0x00000024
    REG32                          GGE_PWR_STAT;                 //0x00000028
    REG32                          RF_PWR_STAT;                  //0x0000002C
    REG32                          BTFM_PWR_STAT;                //0x00000030
    REG32                          STATE_DELAY;                  //0x00000034
    REG32                          PREPGC_DELAY;                 //0x00000038
    REG32                          ALLPGC_DELAY;                 //0x0000003C
    REG32                          DDR_HOLD_CTRL;                //0x00000040
    REG32                          ZSP_PD_POLL;                  //0x00000044
    REG32                          ZSP_PD_POLL_SET;              //0x00000048
    REG32                          ZSP_PD_POLL_CLR;              //0x0000004C
    REG32                          PD_PLL_SW;                    //0x00000050
    REG32                          PD_PLL_SW_SET;                //0x00000054
    REG32                          PD_PLL_SW_CLR;                //0x00000058
    REG32                          PD_PLL_SEL;                   //0x0000005C
    REG32                          PD_PLL_SEL_SET;               //0x00000060
    REG32                          PD_PLL_SEL_CLR;               //0x00000064
    REG32                          IDLE_CG_SW;                   //0x00000068
    REG32                          IDLE_CG_SW_SET;               //0x0000006C
    REG32                          IDLE_CG_SW_CLR;               //0x00000070
    REG32                          IDLE_CG_SEL;                  //0x00000074
    REG32                          IDLE_CG_SEL_SET;              //0x00000078
    REG32                          IDLE_CG_SEL_CLR;              //0x0000007C
    REG32                          RF_IDLE_ENABLE_SW;            //0x00000080
    REG32                          RF_IDLE_ENABLE_SEL;           //0x00000084
    REG32                          WCN_LPS;                      //0x00000088
    REG32                          ARM_SLP_REQ_SW;               //0x0000008C
    REG32                          ARM_SLP_ACK;                  //0x00000090
    REG32                          ARM_SLP_REQ_HWEN;             //0x00000094
    REG32                          ZSP_SLP_REQ_SW;               //0x00000098
    REG32                          ZSP_SLP_ACK;                  //0x0000009C
    REG32                          ZSP_SLP_REQ_HWEN;             //0x000000A0
    REG32                          DDR_SLP_REQ_SW;               //0x000000A4
    REG32                          DDR_SLP_ACK;                  //0x000000A8
    REG32                          DDR_SLP_REQ_HWEN;             //0x000000AC
    REG32                          TIMEOUT_FLAG;                 //0x000000B0
    REG32                          POWER_STATE;                  //0x000000B4
    REG32                          PWRCTRL_MODE;                 //0x000000B8
    REG32                          PWRCTRL_SW;                   //0x000000BC
    REG32                          PWRCTRL_SW_SET;               //0x000000C0
    REG32                          PWRCTRL_SW_CLR;               //0x000000C4
    REG32                          SLOW_SYS_CLK_SEL_HWEN;        //0x000000C8
    REG32                          SLOW_CLK_SEL_HWEN;            //0x000000CC
} HWP_CP_PWRCTRL_T;

#define hwp_pwrctrl                ((HWP_CP_PWRCTRL_T*) REG_ACCESS_ADDRESS(REG_PWRCTRL_BASE))


//PWR_HWEN
#define CP_PWRCTRL_RESERVED_0(n)   (((n)&0x3FFFFFF)<<6)
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

//PD_PLL_SW
#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_DSIPLL          (1<<6)
#define CP_PWRCTRL_MEMPLL          (1<<5)
#define CP_PWRCTRL_USBPLL          (1<<4)
#define CP_PWRCTRL_AUDIOPLL        (1<<3)
#define CP_PWRCTRL_APLL            (1<<2)
#define CP_PWRCTRL_BBPLL2          (1<<1)
#define CP_PWRCTRL_BBPLL1          (1<<0)

//PD_PLL_SW_SET
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_PDPLLSWSET(n)   (((n)&0x7F)<<0)

//PD_PLL_SW_CLR
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_PDPLLSWCLR(n)   (((n)&0x7F)<<0)

//PD_PLL_SEL
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
//#define CP_PWRCTRL_DSIPLL        (1<<6)
//#define CP_PWRCTRL_MEMPLL        (1<<5)
//#define CP_PWRCTRL_USBPLL        (1<<4)
//#define CP_PWRCTRL_AUDIOPLL      (1<<3)
//#define CP_PWRCTRL_APLL          (1<<2)
//#define CP_PWRCTRL_BBPLL2        (1<<1)
//#define CP_PWRCTRL_BBPLL1        (1<<0)

//PD_PLL_SEL_SET
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_PDPLLSELSET(n)  (((n)&0x7F)<<0)

//PD_PLL_SEL_CLR
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_PDPLLSELCLR(n)  (((n)&0x7F)<<0)

//IDLE_CG_SW
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
//#define CP_PWRCTRL_DSIPLL        (1<<6)
//#define CP_PWRCTRL_MEMPLL        (1<<5)
//#define CP_PWRCTRL_USBPLL        (1<<4)
//#define CP_PWRCTRL_AUDIOPLL      (1<<3)
//#define CP_PWRCTRL_APLL          (1<<2)
//#define CP_PWRCTRL_BBPLL2        (1<<1)
//#define CP_PWRCTRL_BBPLL1        (1<<0)

//IDLE_CG_SW_SET
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_IDLECGSWSET(n)  (((n)&0x7F)<<0)

//IDLE_CG_SW_CLR
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_IDLECGSWCLR(n)  (((n)&0x7F)<<0)

//IDLE_CG_SEL
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
//#define CP_PWRCTRL_DSIPLL        (1<<6)
//#define CP_PWRCTRL_MEMPLL        (1<<5)
//#define CP_PWRCTRL_USBPLL        (1<<4)
//#define CP_PWRCTRL_AUDIOPLL      (1<<3)
//#define CP_PWRCTRL_APLL          (1<<2)
//#define CP_PWRCTRL_BBPLL2        (1<<1)
//#define CP_PWRCTRL_BBPLL1        (1<<0)

//IDLE_CG_SEL_SET
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_IDLECGSELSET(n) (((n)&0x7F)<<0)

//IDLE_CG_SEL_CLR
//#define CP_PWRCTRL_RESERVED_0_X6(n) (((n)&0x1FFFFFF)<<7)
#define CP_PWRCTRL_IDLECGSELCLR(n) (((n)&0x7F)<<0)

//RF_IDLE_ENABLE_SW
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_RFIDLEENABLESW  (1<<0)

//RF_IDLE_ENABLE_SEL
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_RFIDLEENABLESEL (1<<0)

//WCN_LPS
#define CP_PWRCTRL_RESERVED_0_X7(n) (((n)&0xFFFFFFF)<<4)
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
#define CP_PWRCTRL_RESERVED_0_X8(n) (((n)&15)<<28)
#define CP_PWRCTRL_COREPOWERSTATE(n) (((n)&15)<<24)
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
#define CP_PWRCTRL_RESERVED_0_X9(n) (((n)&3)<<30)
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
//#define CP_PWRCTRL_RESERVED_0_X9(n) (((n)&3)<<30)
#define CP_PWRCTRL_SET(n)          (((n)&0x3FFFFFFF)<<0)

//PWRCTRL_SW_CLR
//#define CP_PWRCTRL_RESERVED_0_X9(n) (((n)&3)<<30)
#define CP_PWRCTRL_CLEAR(n)        (((n)&0x3FFFFFFF)<<0)

//SLOW_SYS_CLK_SEL_HWEN
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
#define CP_PWRCTRL_HWEN            (1<<0)

//SLOW_CLK_SEL_HWEN
//#define CP_PWRCTRL_RESERVED_0_X4(n) (((n)&0x7FFFFFFF)<<1)
//#define CP_PWRCTRL_HWEN          (1<<0)





#endif

