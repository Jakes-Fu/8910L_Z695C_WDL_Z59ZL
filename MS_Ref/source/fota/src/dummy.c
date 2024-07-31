/******************************************************************************
 ** File Name:      dummy.c                                                   *
 ** Description:                                                              *
 **                                                                           *
 ** Author:         Rui.Zhang                                                 *
 ** DATE:           21/07/2009                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************


 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME             DESCRIPTION                              *
 ** 11/23/2006      Rui.Zhang        Create                                   *
 ******************************************************************************/


#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "sci_types.h"
#include "log.h"
#include "fiq_drvapi.h"
#include "lcd_cfg.h"


/**---------------------------------------------------------------------------*
**                         Compiler Flag
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

//hisr begin
typedef uint32  CHIPDRV_HISR_PRI_E;


const uint32 tx_pri_PRI_HISR_PRIO_0 = 0;
const uint32 tx_pri_PRI_HISR_PRIO_1 = 1;
const uint32 tx_pri_PRI_HISR_PRIO_2 = 2;

typedef void hisr_func_t (uint32 count, void *data);

typedef enum
{
    ISR_DONE = 0x0,
    CALL_HISR = 0xe05a05a5//magic number

} ISR_EXE_T;

// ISR function pointer prototype.
typedef ISR_EXE_T (* TB_ISR) (uint32);
//hisr end


typedef enum
{
    NORMAL_MODE = 0,
    LAYER1_TEST_MODE,
    ASSERT_BACK_MODE,
    CALIBRATION_MODE,
    DSP_CODE_DOWNLOAD_BACK,
    DSP_CODE_DOWNLOAD_BACK_CALIBRATION,
    BOOT_RESET_MODE,
    PRODUCTION_MODE,

    RESET_MODE,


    CALIBRATION_POST_MODE,
    PIN_TEST_MODE,
    IQC_TEST_MODE

}
MCU_MODE_E;

typedef enum{
TYPE_RESET = 0,           //bit[4:0]
TYPE_BACKLIGHT,         //bit[5]
TYPE_DSPEXCEPTION,  //bit[6]
TYPE_USB,                    //bit[7]
TYPE_MAX
}WDG_HW_FLAG_T;


#define KPD_BASE                 0x87000000
#ifdef PLATFORM_SC8800H
#define SYS_CNT0                 (KPD_BASE + 0x001C)
#define GR_HWRST                 (0x8b000020)
#else
#define SYS_CNT0                 (KPD_BASE + 0x3004)
#endif

#define SYSTEM_CURRENT_CLOCK (*((volatile uint32 *)SYS_CNT0) & 0xFFFFFFFF)
#define DSP_MCU_CMD_BASE_ADDR       0x00412600//0x01F01DE0

#define TB_REG_OR(reg_addr, value)    *(volatile uint32 *)(reg_addr) |= (uint32)value
#define TB_REG_AND(reg_addr, value)   *(volatile uint32 *)(reg_addr) &= (uint32)value

CONST uint32    g_mem_limit             = (uint32) (0x4000000- (64 * 1024));


__align (32) uint16 s_main_lcd_buffer[LCD_WIDTH * LCD_HEIGHT];
__align (32) uint16 s_sub_lcd_buffer[SUBLCD_WIDTH * SUBLCD_HEIGHT];

//volatile DSP_MCU_CMD_T *cmd_ptr    = (DSP_MCU_CMD_T *) DSP_MCU_CMD_BASE_ADDR;


/************************   LCD dummy ****************************************/

/*****************************************************************************/
//  Description:    MEM_GetMainLCDBuffer
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           This function is provided from product mem config .
/*****************************************************************************/
PUBLIC void *MEM_GetMainLCDBuffer (void)
{
    return (void *) s_main_lcd_buffer;
}

/*****************************************************************************/
//  Description:    MEM_GetSubLCDBuffer
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           This function is provided from product mem config .
/*****************************************************************************/
PUBLIC void *MEM_GetSubLCDBuffer (void)
{
    return (void *) s_sub_lcd_buffer;
}

/*****************************************************************************/
//  Description:    The function used to get mcu reset mode.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void POWER_GetResetMode (void)
{

}

/*****************************************************************************/
//  Description:    The function is used to create a counting semaphore.
//  Global resource dependence:
//  Author:         Zhemin.Lin
//  Note:
/*****************************************************************************/
PUBLIC void SCI_CreateSemaphore (//if successful, return semaphore
    //pointer, else return SCI_NULL
    const char *name_ptr, //name of the semaphore
    uint32 initial_count //initial value of semaphore counter
)
{

}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Younger.yang
// Note :           6600M ARM_CLK/AHB_CLK = 1
/*****************************************************************************/
//PUBLIC uint32 CHIP_GetAhbClk(void)
//{

//}

PUBLIC BOOLEAN SCI_InThreadContext (void)
{
    return TRUE;
}

/*****************************************************************************/
//  Description:    This function turn on/off the LCD1 backlight
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//  Author:         Zhemin.Lin
//  Note:           LCD1 backlight is controlled by GPIO7 on SM5100B EVB1.0
/*****************************************************************************/
//PUBLIC void GPIO_SetLcdBackLight(BOOLEAN b_on)
//{

//}


/*****************************************************************************/
//  Description:    Get the the value of system counter register.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 TIMER_GetSystemCounterReg (void) // return the value of system count.
{
    volatile uint32 tmp_tick1;
    volatile uint32 tmp_tick2;

    tmp_tick1 = SYSTEM_CURRENT_CLOCK;
    tmp_tick2 = SYSTEM_CURRENT_CLOCK;

    while (tmp_tick1 != tmp_tick2)
    {
        tmp_tick1 = tmp_tick2;
        tmp_tick2 = SYSTEM_CURRENT_CLOCK;
    }

    return tmp_tick1;
}

PUBLIC uint32 SCI_GetTickCount (void)
{
    return TIMER_GetSystemCounterReg();
}
/*****************************************************************************/
// Description :    This function delay some ticks .
// Global resource dependence :
// Author :         Daniel.ding
// Note :
/*****************************************************************************/
PUBLIC uint32 OS_TickDelay (uint32 ticks)
{

    volatile uint32  des_tick = TIMER_GetSystemCounterReg() + ticks;

    while (TIMER_GetSystemCounterReg() < des_tick);

    return 0;

}

/*****************************************************************************/
//  Description:    Suspends the execution of the current thread for a
//                  specified interval.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_Sleep (
    uint32  time_in_ms      // Number of milliseconds to sleep.
)

{

    volatile uint32 end = TIMER_GetSystemCounterReg() + time_in_ms;

    while (TIMER_GetSystemCounterReg() < end);

}

void TM_SendTestPointRequest (uint32 param1, uint32 param2)
{

}

/*****************************************************************************/
//  Description:    The function is used to puts an instance into the specified
//                  counting semaphore, which in reality increments the counting
//                  semaphore by one.
//  Global resource dependence:
//  Author:         Zhemin.Lin
//  Note:
/*****************************************************************************/
uint32 SCI_PutSemaphore ( //if successful return SCI_SUCCESS,
    //else return SCI_ERROR
    void//SCI_SEMAPHORE_PTR sem_ptr //semaphore pointer
)
{
    return 0;
}

/*****************************************************************************/
//  Description:    The function is used to retrieve an instance from the specified
//                  counting semaphore. As a result, the specified semaphore's count
//                  is decreased by one.
//  Global resource dependence:
//  Author:         Zhemin.Lin
//  Note:
/*****************************************************************************/
uint32 SCI_GetSemaphore (//if successful return SCI_SUCCESS,
    //else return SCI_ERROR
    void//SCI_SEMAPHORE_PTR sem_ptr,//semaphore pointer
    //uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
    //             1~0x0FFFFFFFE wait time(ms)
)
{
    return 0;
}

/*****************************************************************************/
//  Description:    This function get peripheral nv parameter.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint16 PROD_GetPeripheralNVParam (uint32 prod_nv_id)
{
    if (0 == prod_nv_id)
    {
        return 0xFC;
    }

    return 0;
}

/*****************************************************************************/
//  Description:    This function set peripheral nv parameter.
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN PROD_SetPeripheralNVParam (
    void//PROD_NV_PERIPHERAL_ID_E id,
    //uint16                    param
)
{
    return TRUE;
}

PUBLIC uint32   ADC_GetResultDirectly (    // If success, return the relative adc result
    int           adcSource,      // ADC source, see ADC_E  define to get more information.
    BOOLEAN       scale)
{
    return 0;
}

/*****************************************************************************/
//  Description:    dma hw channel config
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           channel : dma channel no
//                  dma_type: hard dma/soft dma
//                  int_type: burst/alltransfer/none
//                  chn_param: pointer to the dma channel param
/*****************************************************************************/
PUBLIC uint32 DMA_ChannelCfg (void)
{
    return 0;
}


/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC  uint32  DMA_RegCallBack (void)
{
    return 0;
}

/*****************************************************************************/
//  Description:    dma request enable/disable
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           channel :  channel No
//                  is_enable: enable/disable
/*****************************************************************************/
PUBLIC uint32 DMA_ChannelEnable (uint32 channel, BOOLEAN is_enable)
{
    return 0;
}

/*****************************************************************************/
// Description :    _tx_thread_interrupt_control
// Global resource dependence :
// Author :         Younger.yang
// Note :
/*****************************************************************************/
void   _tx_thread_interrupt_control (void)
{

}


/*****************************************************************************/
//  Description:    This function control lcd backlight
//                  is_high = SCI_TRUE, gpio in high level
//                  is_high = SCI_FALSE, gpio in low level
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
//PUBLIC void GPIO_SetLcdBackLightness(BOOLEAN is_high)
//{
//   LCD_SetBackLightBrightness(50);
//}
typedef uint32 (*DOIDLE_CALLBACK_FUNC) (uint32 param);
PUBLIC BOOLEAN DoIdle_RegisterCallback (DOIDLE_CALLBACK_FUNC func)
{
    return (BOOLEAN) 0;
}
/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
//uint32 CHIP_GetArmClk(void)
//{

//}
uint32 CHR_GetCurVoltage (void)
{
    return 0;
}

/*****************************************************************************/
// Description :    This function is used to get current ARM clock.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :           The valid clock is: ARM_CLK_13M     ARM_CLK_39M
/*****************************************************************************/
//uint32 CHIP_GetAPBClk(void)
//{

//}

/*****************************************************************************/
//  Description:    The function is used to disable IRQ and save old status to
//                  stack.
//  Global resource dependence:
//  Author:         lin.liu
//  Note:
/*****************************************************************************/
PUBLIC void SCI_DisableIRQ (void)
{

}

/*****************************************************************************/
//  Description:    The function is used to restore the old IRQ status.
//  Global resource dependence:
//  Author:         lin.liu
//  Note:
/*****************************************************************************/
PUBLIC void SCI_RestoreIRQ (void)
{

}

PUBLIC void DPSLP_PWM_EnableApbSleep (void)
{

}
/*****************************************************************************/
//  Description:    The function is used to disable IRQ and save old status to
//                  stack.
//  Global resource dependence:
//  Author:         lin.liu
//  Note:
/*****************************************************************************/
PUBLIC void SCI_DisableFIQ (void)
{

}

/*****************************************************************************/
//  Description:    The function is used to disable IRQ and save old status to
//                  stack.
//  Global resource dependence:
//  Author:         lin.liu
//  Note:
/*****************************************************************************/
PUBLIC void SCI_RestoreFIQ (void)
{

}

/*****************************************************************************/
//  Description:    The function change the core voltage
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Note:           when set core voltage,please refer to the core ref voltage you
//                  use now,,and then set relative vdd core voltage value.
/*****************************************************************************/
PUBLIC void PWRMNG_ChangeCoreVoltage (void)
{

}


PUBLIC uint32 SCI_GetAssertFlag (void)
{
    return 0;
}

/*****************************************************************************/
//  Description:    The function change the analog voltage
//  Global resource dependence:
//  Author:         Nick.Zhao
//  Note:
/*****************************************************************************/
PUBLIC void PWRMNG_ChangeAnalogVoltage (BOOLEAN bAnaVol)
{

}

PUBLIC void VB_SetVBSamplerate (void)
{

}

/*****************************************************************************/
//  Description:    This function set VB clock dividor according sample rate needed
//  Author:         Jimmy.Jia
//  Note:
/*****************************************************************************/
PUBLIC void VB_SetVBClk (uint32 sample_rate)
{

}

/*****************************************************************************/
// Description :    This function is used to turn off the power.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
PUBLIC void PWRMNG_TurnOffPower (void)
{

}

PUBLIC void PWRMNG_SetApllPdSrc (void)
{

}

PUBLIC void PWRMNG_SetVpllPdSrc (void)
{

}
/*****************************************************************************/
// Description :    This function is used to force powerdown VPLL clock.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
PUBLIC void PWRMNG_ForcePowerDownVPll (void)
{

}

/*****************************************************************************/
// Description :    This function is used to poweron VPLL clock.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
PUBLIC void PWRMNG_ForcePowerOnVPll (void)
{

}

/*****************************************************************************/
// Description :    This function is used to poweron APLL clock.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
PUBLIC void PWRMNG_ForcePowerOnAPll (void)
{

}

/*****************************************************************************/
// Description :    This function is used to init the initial power manager
//                  module, mainly clk src select for all device.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           select MPLL or VPLL or APLL to be the clock source.
/*****************************************************************************/
PUBLIC void PWRMNG_Init (void)
{

}

void mcu_nand_reboot (void)
{
}
void mcu_uart_reboot (void)
{
}

/*****************************************************************************/
// Description :    This function is to register a pll device request block to
//                  the PLL device request list.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
PUBLIC void PWRMNG_RegPllDeviceReq (void)
{

}

PUBLIC void PWRMNG_LCDC_ClkSwtch (BOOLEAN b_clk_on)
{

}

/*****************************************************************************/
//  Description:    This function dot lcd reset
//  before call this function,you must config the gpio output low in pinmap_**.c.
//  Author:
//  Note:           pulse_level: pin level,
//                  delay_ms:  reset pulse time
/*****************************************************************************/
//PUBLIC void GPIO_ResetLcd( BOOLEAN pulse_level, uint32  delay_ms )
//{

//}

/*****************************************************************************/
// Description :    This function is used to get escaped seconds form power on.
// Global resource dependence :
// Author :         hanjun.liu
// Note :
/*****************************************************************************/

PUBLIC uint32 OS_GetCurrentTime (void)
{

    return (TIMER_GetSystemCounterReg() / 1000);

}

PUBLIC void SCI_LCDBacklightBrightless_EnableDeepSleep (uint32 mode)
{

}

PUBLIC void DPSLP_Lcdc_EnableAhbSleep (uint32 mode)
{

}

/*****************************************************************************/
// Description :    This function is used to clear an event which specify this event
//                  forbids to change frequency. before this event will begin to access
//                  sdram, clear the event.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           sys_mode: when you call this function in an isr handler,
//                          use SYS_MODE_INT.
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_Event_Clr (void)
{
    return 0;
}

/*****************************************************************************/
// Description :    This function is used to set an event which specify this event
//                  permits to change frequency. after this event finished accessing
//                  sdram, set the event.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           sys_mode: when you call this function in an isr handler,
//                          use SYS_MODE_INT.
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_Event_Set (void)
{
    return 0;
}

PUBLIC void ISR_RegHandler (void)
{

}

PUBLIC void *SCI_MallocEx (   // Returns a void pointer to the allocated space,
    // or SCI_NULL if there is insufficient memory
    // available.
    uint32 size,        // Bytes to allocate.
    uint32 type,
    const char *file,   // The name of the file which call this function.
    uint32 line         // The line number in the file which call this function.
)
{
    return NULL;
}

/*****************************************************************************/
//  Description:    This function is used to set output extend GPIO value
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_SetValue (uint16 gpio_num, BOOLEAN b_on)
{
    return 0;
}

/*****************************************************************************/
//  Description:    This function is used to get input extend GPIO value
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_EXT_GetValue (uint16 gpio_num)
{
    return TRUE;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void UPM_CharageIntCallback (BOOLEAN state)
{

}

/*****************************************************************************/
//  Description:    This function is used to handle extend GPIO interrupt
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/

PUBLIC void GPIO_EXT_ISR (uint32 gpio_id) // system gpio, not extend gpio
{

}

PUBLIC void SCM_PlugIn (void)
{

}

PUBLIC void SCM_PlugOut (void)
{

}
void DPSLP_SetBklight_Sts (BOOLEAN bklight_on)
{
}
/*****************************************************************************/
//  Description:    This function is used to register extend GPIO
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_EXT_RegGpio (
    void
)
{
    return 0;
}

/*****************************************************************************/
//  Description:    This function is used to initialize extend GPIO
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_EXT_InitCfgTab (void)
{

}

/*****************************************************************************/
//  Description:    This function is called to do works defined by upper layer.
//  Dependency:     isr_array
//  Author:         Richard.Yang
//  Note:           If OK, Returns TB_SUCCESS;
//                  if int_num is an invalid number, returns TB_INT_ERROR;
//                  if msg is NULL, returns TB_PTR_ERROR ;
//                  if no callback function registered, returns TB_NO_CALLBACK
/*****************************************************************************/
PUBLIC uint32 ISR_Callback (void)
{
    return 0;
}

/*****************************************************************************/
//  Description:    This function used to set the value of a gpio pin
//  Author:           Lin.Liu
//  Note:
/*****************************************************************************/
PUBLIC void SCI_DeactiveTimer (void)
{

}

/*****************************************************************************/
//  Description:    This function used to get the value of a gpio pin
//  Author:           Lin.Liu
//  Note:
/*****************************************************************************/
PUBLIC void  SCI_CreatePeriodTimer (void)
{

}

/*****************************************************************************/
//  Description:    This function used to set gpio Interrupt sense type.
//  Author:           Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void SCI_ChangeTimer (void)
{

}

/*****************************************************************************/
//  Description:    This function used to set the direction of a gpio pin
//  Author:           Benjamin.Wang
// retread by:      Yiyue.He
//  Note:              0 - Input
//                        1 - Output
/*****************************************************************************/
PUBLIC void SCI_ActiveTimer (void)
{

}

/*****************************************************************************/
//  Description:    This function add an callback function to gpio_int_table
//  Author:
//  Retread by: Yiyue.He
//  Note:            This function and GPIO_AddToIntTable() can't be called simultaneously.
/*****************************************************************************/
PUBLIC void SCI_DeleteTimer (void)
{

}

PUBLIC void SCI_IsTimerActive (void)
{

}
/*****************************************************************************/
//  Description:    When charger pulgin/unplug interrupt happened, this function is called.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_ChargerPlugInHandler (uint32 gpio_id, uint32 gpio_state)
{

}
PUBLIC void CHGMNG_SetNormalChargerType(BOOLEAN standard_flag)
{
   
}

void SCI_Free (void)
{
}
void SCI_Assert (void)
{
}
void SCI_CreateMutex (void)
{
}
void SCI_GetMutex (void)
{
}
void SCI_PutMutex (void)
{
}
void SCI_IdentifyThread (void)
{
}
void SCI_GetSignal (void)
{
}
void SCI_SendSignal (void)
{
}
void SCI_CreateStaticThread (void)
{
}
void SCI_DeleteMutex (void)
{
}
uint32 *l1c_get_dsp_log_adr (void)
{
    return NULL;
}
void L1_Assert_Dsp (void)
{
}
void dsp_soft_reset (uint16  flag)
{

}

void DL_reset_tddsp (void)
{
    return;
}

void GetWatchdogMode()
{
}

PUBLIC void WDG_TimerStop()
{
}

PUBLIC void WDG_ResetMCU (void)
{
}

PUBLIC void DPSLP_UART_EnableApbSleep (uint32 port_id,uint32 mode)
{
}

PUBLIC void DPSLP_XTLOpenInSleep (BOOLEAN is_open)
{
}

PUBLIC void kdMemset (void *buf, int byte, uint32 len)
{
}

PUBLIC void kdMemcpy (void *buf, const void *src, uint32 len)
{
}

PUBLIC void LCD_ILI9325_GetSeineCfg (uint32 **cfg_pptr, uint32 *len_ptr)
{
}

PUBLIC void AUDDEV_HPDetectRoutine (BOOLEAN status)
{
}

uint32 GetHwRstAddr (void)
{
    return 0;
}

uint32 GetHwRst1Addr (void)
{
    return 0;
}
/*****************************************************************************/
//  Description:    The SCI_PAssert function supports printing message and variables
//                  when asserting.
//  Author:     Bin.Lu
//  Note:
/*****************************************************************************/
PUBLIC void SCI_PAssert (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line,      // Line number where assert
    char    *assert_info_ptr
)
{

}
PUBLIC char *SCI_SaveAssertInfo (
    const char *x_format, ...)
{
    return NULL;
}
/*****************************************************************************/
//  Description:    Enable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_IRQEnable (uint32 logic_num)
{
}


uint32 ISR_RegHandler_Ex (
    uint32 logic_num,      // Interrupt number
    TB_ISR isr_handler,   // Interrupt service routine pointer.

    hisr_func_t *pHisrFunc,
    CHIPDRV_HISR_PRI_E hisrPri,
    void              *pHisrData
)
{
    return 0;
}

/*****************************************************************************/
//  Description:    Disable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_IRQDisable (uint32 logic_num)
{
}

PUBLIC uint32 CLOCK_Config (void)
{
}

PUBLIC void DMA_init()
{
}

PUBLIC void LDO_Init (void)
{
}

PUBLIC uint32 ADC_Init (void)
{
}

typedef void (*DMA_HAL_ISR_HANDLER) (uint32 inttype);

PUBLIC void DMA_HAL_ChnIntConfig (uint32 channel, uint32 inttype, DMA_HAL_ISR_HANDLER callback)
{
}

typedef void (*DMA_CHNCFG_PTR) (uint32 inttype);
PUBLIC void DMA_HAL_ChnConfig (uint32 channel, DMA_CHNCFG_PTR chnparam)
{
}

PUBLIC void DMA_HAL_ChnEnable (uint32 channel, BOOLEAN is_enable)
{
}
PUBLIC void DMA_HAL_Init (void)
{
}
PUBLIC void bm_disable (void)
{
}
/*****************************************************************************/
//  Description:  Shut down all LDO when system poweroff
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LDO_TurnOffAllLDO (void)
{

}
#ifdef PLATFORM_SC8800H
void ADI_init()
{
}
/*****************************************************************************/
//  Description:  Shut down all LDO when system poweroff
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 ADI_Analogdie_reg_read (uint32 addr)
{
    return 0;
}
/*****************************************************************************/
//  Description:  Shut down all LDO when system poweroff
//  Global resource dependence:
//  Author: Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void ADI_Analogdie_reg_write (uint32 addr, uint16 data)
{
}
/*****************************************************************************/
//  Description:    Set analog value for analog device
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/

PUBLIC void ANA_SetDevValule (void)
{

}
#endif

/*****************************************************************************/
//  Description:    Before watchdog reset, writting HW_RST flag is uesed to j-
//                  udge differrent watchdog reset conditions between MCU reset
//                  and system-halted.
//  Author:         Jeff.Li
//  Modified by:  Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetResetMode (uint32 val)
{
    CHIP_PHY_SetWDGHWFlag(TYPE_RESET, val);
}

/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC void BOOT_SetBLStatus(uint32 val)
{
    CHIP_PHY_SetWDGHWFlag (TYPE_BACKLIGHT, val);
}

/*****************************************************************************/
//  Description:   Get HW Flag
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC uint32 BOOT_GetBLStatus(void)
{
    return CHIP_PHY_GetWDGHWFlag (TYPE_BACKLIGHT);
}


PUBLIC void MMU_EnableICache(){}
PUBLIC void MMU_CacheOn (BOOLEAN i, BOOLEAN d){}
PUBLIC void MMU_CacheOff (BOOLEAN i, BOOLEAN d){}
PUBLIC void MMU_IcacheSyncAll(void){}
PUBLIC void MMU_DcacheSync (void){}
PUBLIC void MMU_Init_Phase1(){}

void SetResetFlag (void)
{
#ifndef PLATFORM_SC8800H
    CHIP_PHY_ResetHWFlag (0x0);
#endif
    BOOT_SetResetMode (RESET_MODE);
}

void DemodSpiIsr(void* context)
{
}

PUBLIC uint32 OSPARAM_GetAssertMode(// If set successfully, return SCI_SUCCESS; 
                                    // else return SCI_ERROR
    uint8  *assert_mode_ptr     // Pointer to a uint8, 
                                // which used to save assert mode.
    )
{
    return 0;
}

uint32 MicroUSB_Interrupt(BOOLEAN is_on)
{
    return 0;
}

PUBLIC uint32 WIFI_IrqCallback(uint32 param)
{
	
}
void INT_HAL_FIQDisableAll()
{
}

void INT_HAL_IRQDisableAll()
{
}
void CHIPDRV_DisableIRQINT(uint32 int_num)
{

}
void CHIPDRV_EnableIRQINT(uint32 int_num)
{

}
void Gx_UDC_USB_Resume(void)
{
	
}
PUBLIC void DBAT_SendMsgToDBatTask (uint32 msg)
{
    
}

PUBLIC uint32* CHGMNG_GetModuleState(void)
{
    return 0;
}

PUBLIC uint32 LCM_Read (uint32 lcd_id, uint32 cd_level)
{
    return 0;
}

void _lcm_set_buswidth ( uint32 bus_width)
{
}

void EICD_DbncIsr( uint32 data )
{
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
