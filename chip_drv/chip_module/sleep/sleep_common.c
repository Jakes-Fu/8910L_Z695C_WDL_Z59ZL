/******************************************************************************
 ** File Name:      sleep_common.c                                                                                     *
 ** Author:         jiexia.yu                                                                                                  *
 ** DATE:           07/10/2007                                                                                             *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.                                    *
 ** Description:    This file defines the basic configuration for ldo management.                       *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                                                                          *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                                                     *
 ** 07/10/2007     jiexia.yu        Create.                                                                            *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                                                                       *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "sleep_phy.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "pin_sleep.h"
#include "../sys_mail/uix8910/mailbox_reg_uix8910.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                                                                       *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

extern volatile uint32  LpsDomainSt;

PUBLIC uint32 sleep_apb_active_modues = 0;
PUBLIC uint32 sleep_ahb_active_modules = 0;
PUBLIC uint32 sleep_active_modules = 0;
PUBLIC uint8  g_usb_plugout_flag = 1;

uint32  ahb_sleep_flag              = 0;
SLPCTL_PTR  s_slp_ctl_tab     		= NULL;

#define SLEEP_MCU_SLEEP             0
#define SLEEP_ARM_SLEEP             1
//#define LOCAL_VAR_DEF   uint32 reg_val;
//macro used to set register bit
#define REGBIT_SET(_reg_addr, _bit_mask, _bit_set)  do{reg_val = REG32(_reg_addr);   \
        reg_val &= ~(_bit_mask);    \
        reg_val |= ((_bit_set)&(_bit_mask)); \
        REG32(_reg_addr) = reg_val;}while(0)
//save pin register
#define REG_SAVE(_pin_sleep_element)  {(_pin_sleep_element).reg_save =    \
            REG32((_pin_sleep_element).reg_addr) &   \
            ((_pin_sleep_element).bit_mask);}
//set up new pin register value
#define REG_SETUP(_pin_sleep_element) REGBIT_SET((_pin_sleep_element).reg_addr,   \
        (_pin_sleep_element).bit_mask, (_pin_sleep_element).bit_set)
//restore pin register value
#define REG_RESTORE(_pin_sleep_element)   REGBIT_SET((_pin_sleep_element).reg_addr,   \
        (_pin_sleep_element).bit_mask, (_pin_sleep_element).reg_save)

#define SUSPEND_RESUME_END_FLAG 0xFFFFFFFF            /*用于标志链表尾*/
#define __LPS_ENABLE__

/*ARM域寄存器注册模块使用的结构体类型*/
struct HAL_IP_SUSPEND_RESUME_T            
{
    char module_name[10];
    void (*reg_suspend)();                       //func addr of reg save
    void (*reg_resume)();                        //func addr of reg resume
    struct HAL_IP_SUSPEND_RESUME_T *next;        //next node addr
};
/*ARM域寄存器注册模块链表表头*/
struct HAL_IP_SUSPEND_RESUME_T g_suspend_resume_head = 
	{"head", 0, 0, (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG};
/*ARM域寄存器注册模块链表表尾*/
struct HAL_IP_SUSPEND_RESUME_T g_suspend_resume_end = 
	{"end", 0, 0, (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG};


/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
//should be configured by customer
#if SLEEP_MODULE_CONFIG_SLEEP_PIN_FEATURE
extern PIN_SLEEP_T misc_pin_sleep_cfg[];
extern CONST uint32 misc_pin_sleep_cfg_size;
#endif


PUBLIC uint32 SLEEP_GetAPBModuleStatus (void)
{
    return  sleep_apb_active_modues;
}

PUBLIC uint32 SLEEP_GetAHBModuleStatus (void)
{
    return sleep_ahb_active_modules;
}

PUBLIC uint32 SCI_Getsleepmode (void)
{
    return sleep_active_modules;
}

#ifdef MODEM_PLATFORM
PUBLIC void SCI_MODEM_EnableDeepSleep(uint32 mode)
{
    if(mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_MODEM;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_MODEM;
    }

}
#endif

PUBLIC void SCI_L1_EnableDeepSleep (uint32 mode)
{
    sleep_active_modules &= ~ SLEEP_DRV_L1;
}

PUBLIC void SCI_MMI_EnableDeepSleep (uint32 mode)
{
#if SLEEP_SUPPORT_MMI

    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_MMI;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_MMI;
    }

#endif
}

PUBLIC void SCI_BT_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_BT;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_BT;
    }
}

PUBLIC void SCI_AUD_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_AUDIO;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_AUDIO;
    }

}

PUBLIC void SCI_LINEIN_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_LINEIN;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_LINEIN;
    }

}

PUBLIC void SCI_CHR_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_CHARGE;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_CHARGE;
    }

}


PUBLIC void SCI_ENGINEER_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_ENGINEER;
		SLEEP_SetCpSysSlpLock(FALSE);
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_ENGINEER;
		SLEEP_SetCpSysSlpLock(TRUE);
    }
}

PUBLIC BOOLEAN SCI_ENGINEER_GetEnableArmSleepFlag (void)
{
    return (sleep_active_modules & SLEEP_DRV_ENGINEER) ? TRUE : FALSE;
}

PUBLIC void SCI_UPM_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_UPM;
#if SLEEP_SUPPORT_USBD
        sleep_ahb_active_modules &= ~SLEEP_AHB_USBD;
#endif
		g_usb_plugout_flag = 0;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_UPM;
#if SLEEP_SUPPORT_USBD
        sleep_ahb_active_modules |= SLEEP_AHB_USBD;
#endif
		g_usb_plugout_flag = 1;
    }
}

PUBLIC void SCI_SDIO_EnableDeepSleep (uint32 mode)
{
#if SLEEP_SUPPORT_SDIO

    if (mode)
    {
        sleep_ahb_active_modules &= ~SLEEP_AHB_SDIO;
    }
    else
    {
        sleep_ahb_active_modules |= SLEEP_AHB_SDIO;
    }

#endif
}


PUBLIC void SCI_DMA_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_ahb_active_modules &= ~SLEEP_AHB_DMA;
    }
    else
    {
        sleep_ahb_active_modules |= SLEEP_AHB_DMA;
    }
}


PUBLIC void SCI_ROT_EnableDeepSleep (uint32 mode)
{
#if SLEEP_SUPPORT_ROT

    if (mode)
    {
        sleep_ahb_active_modules &= ~SLEEP_AHB_ROT;
    }
    else
    {
        sleep_ahb_active_modules |= SLEEP_AHB_ROT;
    }

#endif
}

PUBLIC void SCI_MIDI_EnableDeepSleep (uint32 mode)
{
#if SLEEP_SUPPORT_MIDI

    if (mode)
    {
        sleep_ahb_active_modules &= ~SLEEP_AHB_MIDI;
    }
    else
    {
        sleep_ahb_active_modules |= SLEEP_AHB_MIDI;
    }

#endif
}

PUBLIC void DPSLP_Lcdc_EnableAhbSleep (uint32 mode)
{
#if SLEEP_SUPPORT_LCDC

    if (mode)
    {
        sleep_ahb_active_modules &= ~SLEEP_AHB_LCDC;
    }
    else
    {
        sleep_ahb_active_modules |= SLEEP_AHB_LCDC;
    }

#endif
}

PUBLIC void DPSLP_DCAM_EnableAhbSleep (uint32 mode)
{
#if SLEEP_SUPPORT_DCAM

    if (mode)
    {
        sleep_ahb_active_modules &= ~SLEEP_AHB_DCAM;
    }
    else
    {
        sleep_ahb_active_modules |= SLEEP_AHB_DCAM;
    }

#endif
}

PUBLIC void SCI_DCAM_EnableDeepSleep (uint32 mode)
{
    DPSLP_DCAM_EnableAhbSleep (mode);
}

PUBLIC void DPSLP_ISP_EnableAhbSleep (uint32 mode)
{
#if SLEEP_SUPPORT_ISP

    if (mode)
    {
        sleep_ahb_active_modules &= ~SLEEP_AHB_ISP;
    }
    else
    {
        sleep_ahb_active_modules |= SLEEP_AHB_ISP;
    }

#endif
}

PUBLIC void DPSLP_IVSP_EnableAhbSleep (uint32 mode)
{
#if SLEEP_SUPPORT_IVSP

    if (mode)
    {
        sleep_ahb_active_modules &= ~SLEEP_AHB_IVSP;
    }
    else
    {
        sleep_ahb_active_modules |= SLEEP_AHB_IVSP;
    }

#endif
}

PUBLIC void SCI_BTCLK_EnableDeepSleep (uint32 mode)
{
#if SLEEP_SUPPORT_BTCLK

    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_BITCLK;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_BITCLK;
    }

#endif
}

PUBLIC void SCI_Calibration_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_CALIBRATION;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_CALIBRATION;
    }
}

PUBLIC void SCI_ATC_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_ATC;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_ATC;
    }
}

PUBLIC void SCI_TV_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_TV;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_TV;
    }
}
PUBLIC void SCI_LCDBacklightBrightless_EnableDeepSleep (uint32 mode)
{
#if SLEEP_SUPPORT_BKLIGHT

    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_LCD;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_LCD;
    }

#endif
}

PUBLIC void SCI_CMMB_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_CMMB;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_CMMB;
    }
}

PUBLIC void SCI_WIFI_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_WIFI;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_WIFI;
    }
}


PUBLIC void SCI_CPCPU_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_CPCPU;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_CPCPU;
    }
}


PUBLIC void SCI_PCM_EnableDeepSleep (uint32 mode)
{
    SLEEP_PHY_PCM_EnableSleep (mode);
}

PUBLIC void SCI_SIM_EnableDeepSleep (uint32 mode)
{
    SLEEP_PHY_SIM_EnableSleep (mode);
}

PUBLIC void SCI_TPC_EnableDeepSleep (uint32 mode)
{
    SLEEP_PHY_TPC_EnableSleep (mode);
}


PUBLIC void DPSLP_UART_EnableApbSleep (UART_PORT_ID_E port_id, uint32 mode)
{
    //sc8800h, 0,1,2,3.    sc6800h, 0, 1, 2.   sc6600l 0,1  sc8800g,0,1,2.
    SCI_ASSERT (port_id < UART_MAX);/*assert verified*/

    switch (port_id)
    {
        case UART0_E:
            {
                if (mode)
                {
                    sleep_apb_active_modues &= ~SLEEP_APB_UART0;
                }
                else
                {
                    sleep_apb_active_modues |= SLEEP_APB_UART0;
                }

                break;
            }
        case UART1_E:
            {
                if (mode)
                {
                    sleep_apb_active_modues &= ~SLEEP_APB_UART1;
                }
                else
                {
                    sleep_apb_active_modues |= SLEEP_APB_UART1;
                }

                break;
            }
        case UART2_E:
            {
                if (mode)
                {
                    sleep_apb_active_modues &= ~SLEEP_APB_UART2;
                }
                else
                {
                    sleep_apb_active_modues |= SLEEP_APB_UART2;
                }

                break;
            }
        case UART3_E:
            {
                if (mode)
                {
                    sleep_apb_active_modues &= ~SLEEP_APB_UART3;
                }
                else
                {
                    sleep_apb_active_modues |= SLEEP_APB_UART3;
                }

                break;
            }
        default:
            break;
    }
}

PUBLIC void DPSLP_PWM_EnableApbSleep (PWM_PORT_ID_E port_id, uint32 mode)
{
    //8800g:a,b,c, d,   sc6800h,a,  sc6600l, a,  sc8800h a,b,c,d,e.
    SCI_ASSERT (port_id < PWM_MAX);/*assert verified*/

    switch (port_id)
    {
        case PWMA_E:

            if (mode)
            {
                sleep_apb_active_modues &= ~SLEEP_APB_PWM;
            }
            else
            {
                sleep_apb_active_modues |= SLEEP_APB_PWM;
            }

            break;

        case PWMB_E:

            if (mode)
            {
                sleep_apb_active_modues &= ~SLEEP_APB_PWMB;
            }
            else
            {
                sleep_apb_active_modues |= SLEEP_APB_PWMB;
            }

            break;

        case PWMC_E:

            if (mode)
            {
                sleep_apb_active_modues &= ~SLEEP_APB_PWMC;
            }
            else
            {
                sleep_apb_active_modues |= SLEEP_APB_PWMC;
            }

            break;
        case PWMD_E:

            if (mode)
            {
                sleep_apb_active_modues &= ~SLEEP_APB_PWMD;
            }
            else
            {
                sleep_apb_active_modues |= SLEEP_APB_PWMD;
            }

            break;
        case PWME_E:

            if (mode)
            {
                sleep_apb_active_modues &= ~SLEEP_APB_PWME;
            }
            else
            {
                sleep_apb_active_modues |= SLEEP_APB_PWME;
            }

            break;
        default:
            break;
    }

}

PUBLIC void SCI_SPI_EnableDeepSleep (uint32 mode)
{
#if SLEEP_SUPPORT_SPI

    if (mode)
    {
        sleep_apb_active_modues &= ~SLEEP_APB_SPI;
    }
    else
    {
        sleep_apb_active_modues |= SLEEP_APB_SPI;
    }

#endif
}


PUBLIC void SCI_INTC_EnableDeepSleep (uint32 mode)
{

    if (mode)
    {
        sleep_apb_active_modues &= ~SLEEP_APB_INTC;
    }
    else
    {
        sleep_apb_active_modues |= SLEEP_APB_INTC;
    }

}

PUBLIC void SCI_FM_EnableDeepSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_FM;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_FM;
    }

}

PUBLIC void SCI_MP4_EnableDeepSleep (uint32 mode)
{
	if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_MP4;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_MP4;
    }
}

PUBLIC void SCI_RPC_EnableDeepSleep (uint32 mode)
{
	if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_RPC;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_RPC;
    }
}

/*****************************************************************************
 *  Description:    enable mcu sleep        
 *                                                                           
 *  Global Resource Dependence:                                              
 *  Author: bo.tang                                                          
 *  Note:   	                                         
******************************************************************************/

PUBLIC void SCI_Enter_SleepRoutine (SLP_AHB_APB_TYPE_E slp_type)
{

	// According sleep type enter different sleep routine.
	//if ( (slp_type == AHB_CAN_SLP_APB_CAN_SLP) || (slp_type == AHB_CAN_SLP_APB_NO_SLP))
	if((slp_type == AHB_CAN_SLP_APB_CAN_SLP) && (!sleep_active_modules)){
		ahb_sleep_flag = 1;
		// Enter mcu sleep routine
		//SYSMAIL20 与IPC传输IP地址重复使用，去掉测试稳定性
		//if(hwp_mailbox->SYSMAIL20 == 0x50000){
		       TM_SendTestPointRequest(0x445577,   LpsDomainSt);   
		#ifdef CARD_LOG
		       SIO_SetCardLogPauseFlag(SCI_TRUE);
		#endif
			s_slp_ctl_tab->slp_com_cfg.slp_handler(SLEEP_MCU_SLEEP);
		//}else{
		//	s_slp_ctl_tab->slp_com_cfg.slp_handler(SLEEP_ARM_SLEEP);
		//}
	}
	//else if ( (slp_type == AHB_NO_SLP_APB_CAN_SLP) || (slp_type == AHB_NO_SLP_APB_NO_SLP))
	else if((slp_type == AHB_NO_SLP_APB_CAN_SLP)
	|| (slp_type == AHB_NO_SLP_APB_NO_SLP)
	|| (slp_type == AHB_CAN_SLP_APB_NO_SLP)){
		ahb_sleep_flag = 0;
		// Enter arm sleep routine
		TM_SendTestPointRequest(0x445588,   LpsDomainSt);   
		s_slp_ctl_tab->slp_com_cfg.slp_handler(SLEEP_ARM_SLEEP);  //wfi
	}else{
		SCI_ASSERT (0);/*assert verified*/
	}

}



/*****************************************************************************/
//  Function name:  Slp_Get_Ahb_Status
//  Description  :  this function get AHB bus status
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SLP_AHB_TYPE_E Slp_Get_Ahb_Status (void)
{
    uint32         ahb_id=0;
    SLP_AHB_CTL_T  ahb_ctl;
    SLPCTL_PTR     slp_ctl_tab = NULL;

    slp_ctl_tab   = Slp_Get_Cfg();

    while (slp_ctl_tab->slp_com_cfg.ahb_ctrl[ahb_id].id != SLP_AHB_DEV_MAX)
    {
        ahb_ctl = slp_ctl_tab->slp_com_cfg.ahb_ctrl[ahb_id];

        if (!ahb_ctl.valid)
        {
            if (ahb_ctl.value != (REG32 (ahb_ctl.ahb_dev_reg) & ahb_ctl.mask))
            {
                break;
            }
        }

        ahb_id++;
    }

    if (slp_ctl_tab->slp_com_cfg.ahb_ctrl[ahb_id].id >= SLP_AHB_DEV_MAX)
    {
        if (!SLEEP_GetAHBModuleStatus())
        {
            return AHB_COULD_SLEEP;
        }
    }

    return AHB_NO_SLEEP;

}

/*****************************************************************************/
//  Function name:  Slp_Get_Apb_Status
//  Description  :  this function get APB bus status
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SLP_APB_TYPE_E Slp_Get_Apb_Status (void)
{
    uint32         apb_id=0;
    SLP_APB_CTL_T  apb_ctl;
    SLPCTL_PTR     slp_ctl_tab = NULL;

    slp_ctl_tab   = Slp_Get_Cfg();

    while (slp_ctl_tab->slp_com_cfg.apb_ctrl[apb_id].id < SLP_APB_DEV_MAX)
    {
        apb_ctl = slp_ctl_tab->slp_com_cfg.apb_ctrl[apb_id];

        if (!apb_ctl.valid)
        {
            apb_id++;
            continue;
        }
        else
        {
            if (apb_ctl.value != (REG32 (apb_ctl.apb_dev_reg) & apb_ctl.mask))
            {
                break;
            }
        }

        apb_id++;

    }

    if (slp_ctl_tab->slp_com_cfg.apb_ctrl[apb_id].id >= SLP_APB_DEV_MAX)
    {
        if (!SLEEP_GetAPBModuleStatus())
        {
            return APB_COULD_SLEEP;
        }
    }

    return APB_NO_SLEEP;

}

/*****************************************************************************/
//  Description  :  this function get chip sleep status according ahb and apb
//  bus status
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC SLP_AHB_APB_TYPE_E Slp_Get_Slp_Status (void)
{
    SLP_AHB_TYPE_E 		ahb_status;
    SLP_APB_TYPE_E 		apb_status;
    SLP_AHB_APB_TYPE_E	type = AHB_NO_SLP_APB_NO_SLP;

    ahb_status = Slp_Get_Ahb_Status();
    apb_status = Slp_Get_Apb_Status();

    if (ahb_status== AHB_COULD_SLEEP && apb_status== APB_COULD_SLEEP)
    {
        type = AHB_CAN_SLP_APB_CAN_SLP;
    }
    else if (ahb_status== AHB_COULD_SLEEP && apb_status== APB_NO_SLEEP)
    {
        type = AHB_CAN_SLP_APB_NO_SLP;
    }
    else if (ahb_status== AHB_NO_SLEEP && apb_status== APB_COULD_SLEEP)
    {
        type = AHB_NO_SLP_APB_CAN_SLP;
    }
    else if (ahb_status== AHB_NO_SLEEP && apb_status== APB_NO_SLEEP)
    {
        type = AHB_NO_SLP_APB_NO_SLP;
    }
    else
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    return type;
}






/*****************************************************************************/
//  Description  :  this function config the regs according different sleep status
//  bus status
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void Slp_Reg_Cfg (SLP_AHB_APB_TYPE_E slp_type,SLP_CFG_TYPE_E cfg_type)
{
    uint32         ctrlid      =0;
    SLPCTL_PTR     slp_ctl_tab = NULL;
    SLP_COM_CTL_T  *com_cfg    = NULL;

    slp_ctl_tab = Slp_Get_Cfg();

    switch (cfg_type)
    {
        case SLP_CFG_COM:
            {
                com_cfg = &slp_ctl_tab->slp_com_cfg;

                while (com_cfg->slp_bit_ctrl[ctrlid].id != SLP_CTRL_MAX)
                {
                    if (com_cfg->slp_bit_ctrl[ctrlid].valid)
                    {
                        if (com_cfg->slp_bit_ctrl[ctrlid].value == SLP_BIT_SET)
                        {
                            REG32 (com_cfg->slp_bit_ctrl[ctrlid].slp_bit_reg)
                            |= com_cfg->slp_bit_ctrl[ctrlid].mask  ;
                        }
                        else
                        {
                            REG32 (com_cfg->slp_bit_ctrl[ctrlid].slp_bit_reg)
                            &= ~com_cfg->slp_bit_ctrl[ctrlid].mask ;
                        }
                    }

                    ctrlid++;
                }

                break;
            }
        default:
            break;
    }
}


/*******************************************************************************
*   FUNCTION:       Slp_LpsRegsSave
*   PARAMETERS:     *Regs_Data: target Address;
*                   *RegsTable: source address;
*                    Data_Len : save data Byte length;
*   DESCRIPTION:    save register data
*   RETURNVALUE:    N/A 
*   AUTHOR:         bo.tang 2018/09/03
*******************************************************************************/
void Slp_LpsRegsSave(uint32 *regs_Data,uint32 *regstable, uint32 data_len) 
{
   
    uint8 	temp;
    uint32 *reg_table_addr;
    uint32 *pregs_data;
    
    reg_table_addr = regstable;
    pregs_data     = regs_Data;
    for(temp = 0; temp < data_len; temp++)
    {
        *pregs_data = (*(volatile unsigned long *)(*reg_table_addr));
        pregs_data++;
        reg_table_addr++;
    }
}

/*******************************************************************************
*   FUNCTION:       Slp_LpsRegsResume
*   PARAMETERS:     *Regs_Data: target Address;
*                   *RegsTable: source address;
*                    Data_Len : save data Byte length;
*   DESCRIPTION:    Restore register data
*   RETURNVALUE:    N/A 
*   AUTHOR:         bo.tang 2018/09/03
*******************************************************************************/
void Slp_LpsRegsResume(uint32 *regs_data,uint32 *regstable, uint32 data_len) 
{
    uint8 	temp;
    uint32 *ptable;
    uint32 *pregs_data;
    
    pregs_data = regs_data;
    ptable     = regstable;
    for(temp = 0; temp < data_len; temp++)
    {
        (*(volatile unsigned long *)(*ptable))= *pregs_data;
        pregs_data++;
        ptable++;
    }
}


/*******************************************************************************
*   FUNCTION:       Slp_LpsSuspendLogin
*   PARAMETERS:     
*   DESCRIPTION:    func login.
*   RETURNVALUE:    N/A 
*   AUTHOR:         bo.tang 2018/09/03
*******************************************************************************/
void Slp_LpsSuspendLogin(struct HAL_IP_SUSPEND_RESUME_T *node)
{
    struct HAL_IP_SUSPEND_RESUME_T *p_p0, *p_p1,*p_p2,*p_p3;
    
    p_p0 = node;
    p_p1 = (&g_suspend_resume_head);
    p_p2 = (&g_suspend_resume_end);

    while(p_p1->next != (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG)     
    {
        p_p1 = p_p1->next;
        if(p_p1 == p_p0)
        {
            return;
        }
    }
    if (p_p1->next == (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG)       /*如果g_suspend_resume_head是尾节点*/
    {
        p_p1->next = p_p0;
        p_p2->next = p_p0;
        p_p0->next = (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG;
    }
    else
    {
        p_p3 = p_p2->next;
        p_p3->next = p_p0;
        p_p2->next = p_p0;
        p_p0->next = (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG;        
    }     
}

/*******************************************************************************
*   FUNCTION:       Slp_LpsSuspendLogout
*   PARAMETERS:     
*   DESCRIPTION:    func login.
*   RETURNVALUE:    return 0, cancel success;
*					return -1,cancel fail. 
*   AUTHOR:         bo.tang 2018/09/03
*******************************************************************************/
int Slp_LpsSuspendLogout(struct HAL_IP_SUSPEND_RESUME_T *node)
{
    struct HAL_IP_SUSPEND_RESUME_T *p_p0, *p_p1,*p_p2;
    
    p_p0 = node;
    p_p1 = (&g_suspend_resume_head);
    p_p2 = (&g_suspend_resume_head);

    while ((p_p1->next != p_p0) && (p_p1->next != (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG))
    {
        p_p1 = p_p1->next;
        p_p2 = p_p1;
    }
    if (p_p1->next == (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG)
    {
        return -1;
    }
    else
    {
        p_p1 = p_p1->next;
        p_p2->next = p_p1->next;
        return 0;
    }
}


/*******************************************************************************
*   FUNCTION:       Slp_LpsSuspend
*   PARAMETERS:     
*   DESCRIPTION:    save poweroff domain register.
*   RETURNVALUE:    NA. 
*   AUTHOR:         bo.tang 2018/09/03
*******************************************************************************/
void Slp_LpsSuspend(void)
{
    struct HAL_IP_SUSPEND_RESUME_T *p_p0;
    
    p_p0 = (&g_suspend_resume_head);                  	//get linked list head
    if (p_p0->next != (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG)
    {        
        do 
        {
            p_p0 = p_p0->next;
            (*p_p0->reg_suspend)();                		//call-back save function
        }
        while(p_p0->next != (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG);
    }
}


/*******************************************************************************
*   FUNCTION:       Slp_LpsResume
*   PARAMETERS:     
*   DESCRIPTION:    resume poweroff domain register.
*   RETURNVALUE:    NA. 
*   AUTHOR:         bo.tang 2018/09/03
*******************************************************************************/
void Slp_LpsResume (void)                   
{
    struct HAL_IP_SUSPEND_RESUME_T *p_p0;
    
    p_p0 = (&g_suspend_resume_head);                    //get linked list head
    if (p_p0->next != (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG)
    {      
        do 
        {
            p_p0 = p_p0->next;
            (*p_p0->reg_resume)();                      //call-back resume function
        }
        while(p_p0->next != (struct HAL_IP_SUSPEND_RESUME_T *)SUSPEND_RESUME_END_FLAG);        
    }
}

#if SLEEP_MODULE_CONFIG_SLEEP_PIN_FEATURE
/*****************************************************************************/
//  Description:  setup pin config when system enters deepsleep
//  Global resource dependence:  misc_pin_sleep_cfg[]
//  Author:  jiexia.yu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN s_is_pin_setup = SCI_FALSE;
PUBLIC void PIN_EnterDeepSleep (void)
{
    LOCAL_VAR_DEF
    uint32 i;

    SCI_ASSERT (SCI_FALSE == s_is_pin_setup);/*assert verified*/

    for (i = 0; i < misc_pin_sleep_cfg_size; i++)
    {
        //setup miscellaneous pin in deepsleep
        REG_SAVE (misc_pin_sleep_cfg[i]);
        REG_SETUP (misc_pin_sleep_cfg[i]);
    }

    s_is_pin_setup = SCI_TRUE;
}

/*****************************************************************************/
//  Description:  restore pin config when system leaves deepsleep
//  Global resource dependence:  misc_pin_sleep_cfg[]
//  Author:  jiexia.yu
//  Note:
/*****************************************************************************/
PUBLIC void PIN_LeaveDeepSleep (void)
{
    LOCAL_VAR_DEF
    uint32 i;

    SCI_ASSERT (SCI_TRUE == s_is_pin_setup);/*assert verified*/

    //restore register value from end of array!
    for (i = 1; i <= misc_pin_sleep_cfg_size; i++)
    {
        REG_RESTORE (misc_pin_sleep_cfg[misc_pin_sleep_cfg_size - i]);
    }

    s_is_pin_setup = SCI_FALSE;
}

#endif

