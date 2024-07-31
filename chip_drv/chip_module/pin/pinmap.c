/******************************************************************************
 ** File Name:      pinmap.c                                                  *
 ** Author:         Richard.Yang                                              *
 ** DATE:           03/08/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the pin contrl interface for 6600 chip  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/08/2004     Richard.Yang     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "pinmap_drvapi.h"
#include "gpio_drvapi.h"
#if defined(PLATFORM_UIX8910)
#include "../../../../../chip_drv/chip_module/pin/uix8910/pinmap.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define     KBPIN_FRC_OUT_HIGH     (PIN_OUT_FRC_HIGH | PIN_PULL_FRC_NONE | 0xF)
#define     KBPIN_FRC_OUT_LOW      (PIN_OUT_FRC_LOW | PIN_PULL_FRC_NONE | 0xF)
#define     SCI_FAILURE       0x01

/**---------------------------------------------------------------------------*
 **                         Data                                              *
 **---------------------------------------------------------------------------*/
LOCAL void PM_GPIOInit (void);


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Return the INT sense type according to GPIO number in
//                  pm_gpio_default_map[]
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC PM_IS_E PM_GetGPIOIntType (uint32 gpio_num)
{
    uint32 i = 0;

    for (;;)
    {
        if (PM_INVALID_SHORT_VAL == pm_gpio_default_map[i].gpio_num)
        {
            break;
        }

        if (PM_INPUT == pm_gpio_default_map[i].dir)
        {
            if (gpio_num == pm_gpio_default_map[i].gpio_num)
            {
                return pm_gpio_default_map[i].int_sense;
            }
        }

        i++;
    }

    return PM_INVALID_INT;
}

PUBLIC void PM_Init (void)
{
    int i = 0;
#if defined(PLATFORM_UIX8910)
    uint32 value = 0;
    uint32 default_value = 0;
    uint8 bit_at_reg;
    T_ANA_PIN_FUN_BIT t_an_pin_fun_bit;
#else
    uint32 value = 0;
#endif

#ifdef PLATFORM_SC8800G
    CHIP_REG_OR ( (GR_GEN0), (GEN0_PIN_EN));
    ANA_REG_OR (ANA_AGEN, (AGEN_PINREG_EN)); // enable gpio base romcode
#endif

    for (;;)
    {
        // Check if search to end
        if (pm_func[i].addr == PM_INVALID_VAL)
        {
            break;
        }

#if defined(PLATFORM_SC8800G) || defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM)

        // Write the value to chip
        //if (0x02000000 != (pm_func[i].addr & 0x02000000))  //d-die 0x8c000000; a-die 0x82000000
        if (!ANA_IS_ANA_REG (pm_func[i].addr))
        {
            value = pm_func[i].value;
             //when spiflash voltage is 3V,the lowest spiflash pin driver strength is PIN_DS_4,PIN_DS_0 is apply to spiflash voltage is 1.8V.
            if((PIN_ESMD0_REG <= pm_func[i].addr) && (PIN_ESMCSN0_REG >= pm_func[i].addr))
            {
                if(LDO_SF_VOLT_3000 == (ANA_REG_GET(ANA_LDO_SF_REG0)&LDO_SF_OPTION_O))
                {
                	value |= (PIN_DS_4);
                }
				else //1.8V
				{
					if(SC6531EFM == CHIP_GetChipType())
                    {
                        //before metal fix set,only use for chip test.
                       value  |= PIN_DS_5;
                    }
				}
            }
             //UART0/UART1 pad have electric leakage
            if(((PIN_U0RXD_REG <= pm_func[i].addr) && (PIN_U0RTS_REG >= pm_func[i].addr)) || (PIN_U0TXD_REG == pm_func[i].addr))
            {
                if(CHIP_TYPE_EX_AA_AB == CHIP_GetChipTypeEx())
                {
                	value &= ~(PIN_SPD_EN |PIN_SPU_EN | PIN_SLP_IE | PIN_SLP_OE);
                	value |= (PIN_SPD_EN | PIN_SLP_Z);//Pull down and High-Z enable for sleep mode
                }
            }
            CHIP_REG_SET (pm_func[i].addr ,value);
        }
        else
        {
            ANA_REG_SET (pm_func[i].addr ,pm_func[i].value);
        }
#elif defined(PLATFORM_UIX8910)
#if 0
        if (ANA_IS_ANA_REG (pm_func[i].addr))
        {
            value = pm_func[i].value;
             //when spiflash voltage is 3V,the lowest spiflash pin driver strength is PIN_DS_4,PIN_DS_0 is apply to spiflash voltage is 1.8V.
            if((PIN_ESMD0_REG <= pm_func[i].addr) && (PIN_ESMCSN0_REG >= pm_func[i].addr))
            {
                if(LDO_SF_VOLT_3000 == (ANA_REG_GET(ANA_LDO_SF_REG0)&LDO_SF_OPTION_O))
                {
                	value |= (PIN_DS_4);
                }
				else //1.8V
				{
					if(SC6531EFM == CHIP_GetChipType())
                    {
                        //before metal fix set,only use for chip test.
                       value  |= PIN_DS_5;
                    }
				}
            }
             //UART0/UART1 pad have electric leakage
            if(((PIN_U0RXD_REG <= pm_func[i].addr) && (PIN_U0RTS_REG >= pm_func[i].addr)) || (PIN_U0TXD_REG == pm_func[i].addr))
            {
                if(CHIP_TYPE_EX_AA_AB == CHIP_GetChipTypeEx())
                {
                	value &= ~(PIN_SPD_EN |PIN_SPU_EN | PIN_SLP_IE | PIN_SLP_OE);
                	value |= (PIN_SPD_EN | PIN_SLP_Z);//Pull down and High-Z enable for sleep mode
                }
            }
            CHIP_REG_SET (pm_func[i].addr ,value);
        }
        else
#endif
        {
            value = pm_func[i].value;
	     if ((pm_func[i].addr >= ANALOG_PIN_RESETB_CFG) && (pm_func[i].addr <= ANALOG_PIN_UART_2_CFG) )
	     {
	         default_value = CHIP_REG_GET(pm_func[i].addr);
		   bit_at_reg = 0;
		   if (((pm_func[i].addr >= ANALOG_PIN_SDMMC1_OTHERS_CFG) && (pm_func[i].addr <= ANALOG_PIN_SPI_LCD_CFG2)) || (pm_func[i].addr == ANALOG_PIN_LCD_CFG))
		   {
			t_an_pin_fun_bit.pad_drv_bitmask = 0x78;
			t_an_pin_fun_bit.pad_whole_bitmask = 0x7F;
			t_an_pin_fun_bit.pad_whole_bitcnt = 0x7;
		   }
		   else
		   {
			t_an_pin_fun_bit.pad_drv_bitmask = 0x18;
			t_an_pin_fun_bit.pad_whole_bitmask = 0x1F;
			t_an_pin_fun_bit.pad_whole_bitcnt = 0x5;
		   }
		   while (bit_at_reg < 31)
		   {
			   if (value & t_an_pin_fun_bit.pad_whole_bitmask)
		         {
				default_value &= ~(t_an_pin_fun_bit.pad_whole_bitmask << bit_at_reg); 
				default_value |= ((value & t_an_pin_fun_bit.pad_whole_bitmask) << bit_at_reg); 
				break;
			   }
			   else    /*use default value*/
			   {
				value = value >> t_an_pin_fun_bit.pad_whole_bitcnt;
				bit_at_reg += t_an_pin_fun_bit.pad_whole_bitcnt;
			   }
		   }
		   value = default_value;
	     }
            CHIP_REG_SET (pm_func[i].addr ,value);
        }
#else
        CHIP_REG_SET (pm_func[i].addr ,pm_func[i].value);
#endif
        i++;
    }

    PM_GPIOInit();
    
#if defined(PLATFORM_UIX8910)
    hwp_idle->PAD_CTRL &= ~(CP_IDLE_PAD_CHIP_PD_PULL_UP |CP_IDLE_PAD_CHIP_PD_PULL_DOWE | CP_IDLE_PAD_GPIO_6_PULL_UP | CP_IDLE_PAD_GPIO_6_PULL_DOWE);  //chip_pd /gpio pull up/down disable 
    hwp_idle->PAD_CTRL |= (CP_IDLE_PAD_GPIO_6_PULL_FRC |CP_IDLE_PAD_CHIP_PD_PULL_FRC);  //chip_pd /gpio pull up/down disable 
    hwp_idle->PAD_CTRL_UART_TXD &= ~(CP_IDLE_PAD_UART_1_TXD_PULL_UP | CP_IDLE_PAD_UART_1_TXD_PULL_DOWE);  //uart1_tx pull up/down disable 
    hwp_idle->PAD_CTRL_UART_TXD |= CP_IDLE_PAD_UART_1_TXD_PULL_FRC;  //uart1_tx pull up/down disable 
#endif
}

LOCAL void PM_GPIOInit (void)
{
    int i = 0;
    uint16 gpio_num;

    // Enable all GPIO pages
#if defined (PLATFORM_SC8800G)
    CHIP_REG_OR ( (GR_GEN0), (GEN0_GPIO_EN | GEN0_GPIO_RTC_EN));
    ANA_REG_OR (ANA_AGEN,AGEN_RTC_GPIO_EN);
    SCI_Sleep (5);
    ANA_REG_OR (ANA_AGEN,AGEN_GPIO_EN);
#endif

    for (;;)
    {
        if (pm_gpio_default_map[i].gpio_num == PM_INVALID_SHORT_VAL)
        {
            break;
        }

        gpio_num  = pm_gpio_default_map[i].gpio_num;

        if (pm_gpio_default_map[i].dir == PM_OUTPUT)
        {
            // first Set default value
            if (0xFFFF != pm_gpio_default_map[i].default_val)
            {
                GPIO_SetValue (gpio_num,pm_gpio_default_map[i].default_val);
            }

            // Output only
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            // Config it to be OUTPUT.(1)
            GPIO_SetDirection (gpio_num,SCI_TRUE);
            // MASK it.(0)
            GPIO_DisableIntCtl (gpio_num);
        }
        else
        {
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            // Config it to be INPUT.(0)
            GPIO_SetDirection (gpio_num,SCI_FALSE);
        }

        i++;
    }
}

uint8 PM_GPIOKeyPinCtrl(uint8 dir, uint32 io_index, uint8 *pvalue)
{
    uint32 gpioex_kbpin_addr;
    uint32 gpioex_kbpin_value[2] = {KBPIN_FRC_OUT_LOW, KBPIN_FRC_OUT_HIGH};
    const uint32 gpioex_kb_set_addr[]=
    {
        PIN_KEYIN_0_CFG_REG,
        PIN_KEYIN_1_CFG_REG,
        PIN_KEYIN_2_CFG_REG,
        PIN_KEYIN_3_CFG_REG,
        PIN_KEYIN_4_CFG_REG,
        PIN_KEYIN_5_CFG_REG,
        NULL,
        NULL,
        PIN_KEYOUT_0_CFG_REG,
        PIN_KEYOUT_1_CFG_REG,
        PIN_KEYOUT_2_CFG_REG,
        PIN_KEYOUT_3_CFG_REG,
        PIN_KEYOUT_4_CFG_REG,
        PIN_KEYOUT_5_CFG_REG,
        NULL,
        NULL
    };    
    uint8 result = SCI_SUCCESS;

    if (0 == dir)    //out
    {
        if ((io_index >= (sizeof(gpioex_kb_set_addr) / sizeof(uint32))) || (*pvalue > 1))
        {
            result = SCI_FAILURE; 
        }
        else
        {
            if (NULL != gpioex_kb_set_addr[io_index])
            {
                CHIP_REG_SET (gpioex_kb_set_addr[io_index] ,gpioex_kbpin_value[*pvalue]);
            }
            else
            {
                result = SCI_FAILURE; 
            }
        }    
    }
    else    //in
    {
         result = SCI_FAILURE; 
    }

    return result;
}

#ifdef PLATFORM_SC8800G
//******************************************************************************
//  Description:  PM_PHY_DSPJTAGSet
//  Global resource dependence:
//  Author:  andy.chen
//  para:  void
//  Note:  this funcition set pin function to dsp jtag
//******************************************************************************
PUBLIC void PM_PHY_DSPJTAGEn (void)
{
    CHIP_REG_SET (PIN_IISDI_REG, (PIN_DS_1 | PIN_FPD_EN | PIN_FUNC_1 | PIN_SPD_EN | PIN_I_EN));
    CHIP_REG_SET (PIN_IISDO_REG, (PIN_DS_1 | PIN_FPX_EN | PIN_FUNC_1 | PIN_SPU_EN | PIN_I_EN));
    CHIP_REG_SET (PIN_IISCLK_REG, (PIN_DS_1 | PIN_FPD_EN | PIN_FUNC_1 | PIN_SPU_EN | PIN_I_EN));
    CHIP_REG_SET (PIN_IISLRCK_REG, (PIN_DS_1 | PIN_FPD_EN | PIN_FUNC_1 | PIN_SPU_EN | PIN_I_EN));
    CHIP_REG_SET (PIN_IISMCK_REG, (PIN_DS_1 | PIN_FPD_EN | PIN_FUNC_1 | PIN_SPD_EN | PIN_I_EN));
}

//======================================
// note: this funtion should be put in dsp relative file later
//
PUBLIC void DSP_JTAGEnable (uint32 id)
{
    PM_PHY_DSPJTAGEn();
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

// End of pinmap.c
