/******************************************************************************
 ** File Name:      chip_drv_init.c                                           *
 ** Author:         Jeff Li                                                   *
 ** DATE:           06/09/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces called during system and *
 **                 driver initialization, which export to init.c of refbase. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/09/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//lint -e766
#include "os_api.h"
#include "chip_plf_export.h"
#include "chg_drvapi.h"
#include "dma_drv_internal.h"
#include "clock_drvapi.h"
#include "watchdog.h"
#include "emc_drvapi.h"
#include "ldo_drvapi.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "rtc_drvapi.h"
#include "timer_drvapi.h"
#include "nlc_drvapi.h"
#include "gpio_drvapi.h"
#include "adc_drvapi.h"
#include "upm_api.h"
#include "analog_drvapi.h"
#include "mmu_drvapi.h"
#include "udc_api.h"
#include "usbservice_api.h"
#include "eic_hal.h"
#include "normcp_spec.h"
#include "flash_drvapi.h"
#include "lzma_hal.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
PUBLIC void UartDSlp_Init (void);
PUBLIC void gea_init (void);

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    Phase1 of chip driver intialization. It initializes criti-
//                  cal resource of chip before the refbase starts to boot, it
//                  contains global clock init and enable, pinmap config init etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase1 (void)
{
    MMU_Init_Phase1();
    //INT_PHY_Init();

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UIX8910)
#ifndef NOR_BOOTLOADER_SUPPORT
	//CHIP_LDOSFPDEnAfterRst();
#endif
#endif

#if defined(PLATFORM_UIX8910)
    /*the sysmail register not default to 0 when power on, so set to 0 manually*/
    CHIP_SYSMAIL_Reg_Init();
#endif

#ifdef FLASH_TYPE_SPI
    FLASH_SystemInit();
 #endif
    // Get current chip type
    CHIP_InitHWChipID();

    //Init chip
    CHIP_Init ();

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UIX8910)
    if (! (CHIP_DetectMemType()))
    {
        // Init flash device
        EMC_Init();
    }
#endif

    //LZMA_HAL_Init();
}

/*****************************************************************************/
// Description :    Phase2 of chip driver intialization. It initializes driver
//                  modules before refbase confirms powering on. it contains
//                  some necessary modules including RTC, timer, DMA, NLC etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase2 (void)
{
    TIMER_ID_E id;

    // Init interrupt controller
    ISR_Init ();

    LDO_Init();    /*ï¿½ï¿½ï¿½ï¿½ï¿½Î£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/

    SLEEP_Init();

    //Init clock system
    CLOCK_System_Init();

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UIX8910)
#if defined(LDO_CALIBRATION_SUPPORT)
	// ADC parameter init
    ADC_Init();
#endif

	//ldo calibration
	//LDO_Calibration();

	//vdddcxo rise to 1.95v
	//LDO_SetVoltValue(LDO_LDO_DCXO, 1950);
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UIX8910)
/* IB Calibration is used for LCD&KPD LED, LCD&KPD are not used in modem platform */
#if !defined(FPGA_VERIFICATION) && !defined(MODEM_PLATFORM)
    //IB calibration
    //IB_Calibration();
#endif    
#endif
	
    // Init RTC.
   RTC_Init ();

    // Init Timer
#if defined(PLATFORM_UIX8910)
    //for (id=TIMER_3; id<TIMER_4; id++)
    id = OS_TIMER;
#else
    for (id=TIMER_0; id<TIMER_MAX; id++)
#endif
    {
        TIMER_HAL_Open (id);
    }

#if defined(PLATFORM_UIX8910)
    TIMER_HAL_IOctl (OS_TIMER, TIMER_IOCTL_CMD_START, NULL);
#else
    TIMER_HAL_IOctl (TIMER_1, TIMER_IOCTL_CMD_START, NULL);
#endif
    // Init DMA
    DMA_HAL_Init();
	SLEEP_InitSoft();
#ifdef  NANDBOOT_SUPPORT
    // Nandflash controller Init
    NANDCTL_Init (CHIP_GetAhbClk());
#else //Nor Platform

#ifndef FPGA_VERIFICATION
#ifdef PLATFORM_SC6600L
    EMC_SetARMAHBClock (80);
#elif defined(PLATFORM_SC6800H)
    if(SC6800H == CHIP_GetChipType())
    {
        EMC_SetARMAHBClock (200);
    }
    else
    {
        EMC_SetARMAHBClock (180);
    }
#endif
#endif
#endif //NANDBOOT_SUPPORT

#ifndef NANDBOOT_SUPPORT
    //Init MMU
    MMU_Init ();
#endif

#ifdef PLATFORM_SC6530
#ifndef FPGA_VERIFICATION
	EIC_HAL_Init();
#endif
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UIX8910)
#ifndef FPGA_VERIFICATION
	//EIC_HAL_Init();
#endif
#endif

#if defined(PLATFORM_UIX8910) && defined(SECURE_BOOT_SUPPORT)
    SecureMode_WCNuartEnable();
#endif
}

/*****************************************************************************/
// Description :    Phase3 of chip driver intialization. It initializes driver
//                  modules after refbase confirms powering on. it contains
//                  the modules including GPIO, watchdog, analog, charge etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase3 (void)
{
//#ifndef FPGA_VERIFICATION
    // Initialize GPIO driver.
    GPIO_Init();
//#endif
#if defined(PLATFORM_UIX8910)
{
#ifndef CHIP_ECO_SUPPORT
    uint32 temp = ADI_Analogdie_reg_read(0xc08);
    temp |= (1<<2)|(1<<10);
    ADI_Analogdie_reg_write (0xc08, temp);
    ADI_Analogdie_reg_write (0x900, 0x454e);
    ADI_Analogdie_reg_write (0x948, 0x1);      /*vgenÉÏÓÐÂ©µçµ¼ÖÂ32K error£¬ÁÙÊ±°æ±¾È¥µô*/
    ADI_Analogdie_reg_write (0xe6c, 0x2ff);  /*vmem do not reset*/
    ADI_Analogdie_reg_write (0xe68, 0x80);
#endif
}
#endif
    // Init and  start the watch dog.
    WatchDogTimerInit();

    // Init the GEA
    gea_init();

    // uart deepsleep wakeup func init
    UartDSlp_Init();

#if !defined(PLATFORM_SC6531EFM) && !defined(LDO_CALIBRATION_SUPPORT) && !defined(PLATFORM_UIX8910)
    // ADC parameter init
    ADC_Init();
#endif

    //Init vbus thread
    USB_InitBottomHalf();

    //Init usb device controller and ISR
    UDC_Init();

    //Init main thread
    USB_InitTopHalf();

    // Init chng freq
    CHNG_FREQ_Init();

#if !defined(FPGA_VERIFICATION) && !defined(MODEM_PLATFORM)
    // It depends on GPIO, so must be called after GPIO has been initilized
    CHGMNG_Init();   //ljj stub0806
#endif


#ifndef FPGA_VERIFICATION	
    // Init Analog device
    ANA_Init();
#endif

    hal_PagespyInit();   //pagespy

}

/*****************************************************************************/
// Description :    Phase4 of chip driver intialization. It initializes MMU
//                  after refbase has copied the kernel image.
// Author :         Jeff.Li
// Note :           Before phase4, the MMU and virtual memory mapping is still
//                  not initialized, so the kernel image coping of refbase sho-
//                  uld make sure that the operating address is physical.
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase4 (void)
{

#ifdef NANDBOOT_SUPPORT
    //Init MMU
    MMU_Init ();
#endif

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
