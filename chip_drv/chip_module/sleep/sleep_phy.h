/******************************************************************************
 ** File Name:      sleep_phy.h                                             *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           07/09/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function for ldo management.  *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/09/2007     jiexia.yu        Create.                                   *
 ******************************************************************************/
#ifndef _SLEEP_PHY_H_
#define _SLEEP_PHY_H_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#define SLEEP_APB_SIM  		BIT_0
#define SLEEP_APB_TPC  		BIT_1
#define SLEEP_APB_PWM  		BIT_2
#define SLEEP_APB_UART0  	BIT_3
#define SLEEP_APB_UART1  	BIT_4
#define SLEEP_APB_UART2  	BIT_5
#define SLEEP_APB_UART3  	BIT_6
#define SLEEP_APB_IIS       BIT_7
#define SLEEP_APB_PWMB   	BIT_8//SC8800G
#define SLEEP_APB_PWMC   	BIT_9//SC8800G
#define SLEEP_APB_PWMD   	BIT_10//SC8800G
#define SLEEP_APB_PWME   	BIT_11//SC8800H
#define SLEEP_APB_SPI   	BIT_12//SC8800G
#define SLEEP_APB_INTC   	BIT_13//SC6531
#define SLEEP_APB_OSTIMER   BIT_14

#define SLEEP_AHB_DMA    	BIT_0
#define SLEEP_AHB_DCAM  	BIT_1
#define SLEEP_AHB_SDIO   	BIT_2
#define SLEEP_AHB_LCDC   	BIT_3
#define SLEEP_AHB_ISP     	BIT_4
#define SLEEP_AHB_USBD   	BIT_5//SC6600L
#define SLEEP_AHB_TPC      	BIT_6//sc8800h
#define SLEEP_AHB_IVSP    	BIT_7//SC8800H
#define SLEEP_AHB_ROT     	BIT_8
#define SLEEP_AHB_MIDI     	BIT_9//SC6530

#define SLEEP_DRV_NONE			0
#define SLEEP_DRV_L1			BIT_0
#define SLEEP_DRV_MMI			BIT_1
#define SLEEP_DRV_SIM			BIT_2
#define SLEEP_DRV_AUDIO			BIT_3
#define SLEEP_DRV_LCD			BIT_4
#define SLEEP_DRV_CHARGE		BIT_5
#define SLEEP_DRV_ENGINEER		BIT_6
#define SLEEP_DRV_BT			BIT_7
#define SLEEP_DRV_PCM			BIT_8
#define SLEEP_DRV_BTCLK			BIT_9
#define SLEEP_DRV_UPM			BIT_10
#define SLEEP_DRV_CALIBRATION	BIT_11
#define SLEEP_DRV_ATC			BIT_12
#define SLEEP_DRV_AP			BIT_13
#define SLEEP_DRV_BITCLK		BIT_14//SC8800H
#define SLEEP_DRV_TV            BIT_15
#define SLEEP_DRV_LINEIN        BIT_16
#define SLEEP_DRV_CMMB        	BIT_17
#define SLEEP_DRV_WIFI        	BIT_18
#define SLEEP_DRV_MODEM       	BIT_19
#define SLEEP_DRV_FM	      	BIT_20
#define SLEEP_DRV_MP4         	BIT_21
#define SLEEP_DRV_CPCPU         BIT_22
#define SLEEP_DRV_LPSTIMER      BIT_23
#define SLEEP_DRV_WCN_OSC       BIT_24
#define SLEEP_DRV_RPC                BIT_25

extern uint32 sleep_apb_active_modues;
extern uint32 sleep_ahb_active_modules;
extern uint32 sleep_active_modules;

extern uint8  g_usb_plugout_flag;

extern SLPCTL_PTR  s_slp_ctl_tab;

#if defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910FF) 
#define LPS_CTRL_TYPE_PSRAM
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Function name:  Slp_Get_Apb_Status
//  Description  :  this function get chip sleep status according ahb and apb
//  bus status
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC SLP_AHB_APB_TYPE_E  Slp_Get_Slp_Status (void);
/*****************************************************************************/
//  Description  :  this function config the regs according different sleep status
//  bus status
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void Slp_Reg_Cfg (SLP_AHB_APB_TYPE_E slp_type,SLP_CFG_TYPE_E cfg_type);
PUBLIC int  tx_enter_deep_sleep (uint32 level);
PUBLIC void RECOVERY_ARM_REG(void);
PUBLIC void Slp_Pm0_wfi (void);
PUBLIC void SCI_Enter_SleepRoutine (SLP_AHB_APB_TYPE_E slp_type);
PUBLIC void UNI_WAKEUP_ENTRY(void);
/*****************************************************************************/
//  Description:  setup pin config when system enters deepsleep
//  Global resource dependence:  misc_pin_sleep_cfg[]
//  Author:  jiexia.yu
//  Note:
/*****************************************************************************/
PUBLIC void PIN_EnterDeepSleep (void);

/*****************************************************************************/
//  Description:  restore pin config when system leaves deepsleep
//  Global resource dependence:  misc_pin_sleep_cfg[]
//  Author:  jiexia.yu
//  Note:
/*****************************************************************************/
PUBLIC void PIN_LeaveDeepSleep (void);

PUBLIC uint32 SCI_Getsleepmode (void);



PUBLIC void SLEEP_PHY_PCM_EnableSleep (uint32 mode);
PUBLIC void SLEEP_PHY_SIM_EnableSleep (uint32 mode);
PUBLIC void SLEEP_PHY_TPC_EnableSleep (uint32 mode);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _LDO_MANAGER_H_
