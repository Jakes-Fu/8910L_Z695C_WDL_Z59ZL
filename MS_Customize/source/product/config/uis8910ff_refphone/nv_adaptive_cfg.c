/******************************************************************************
 ** File Name:      nv_adaptive_cfg.c                                         *
 ** Author:         abe.yue                                          	      *
 ** DATE:           04/07/2020                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic configure about get adaptive  *
					version.												  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/07/2020     abe.yue			Create.                                                           *
 ******************************************************************************/

 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "nv_adaptive_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

 LOCAL VERSION_GPIO_CFG_T bard_id_cfg[] = {
	{9, BIT_0},				//GPIO9  crystal_type  0: DCXO   1 : TCXO
	{11, BIT_1},			//GPIO119 PA BOARD ID Bit0
	{12, BIT_2},			//GPIO117 PA BOARD ID Bit1  00: RPM6743 & RTM7916
	{0xFFFF,0}
};

LOCAL const uint8 adc2deltanv_table[] = {
	-1, /* reserved */
	-1, /* reserved */
	-1, /* reserved */
	-1, /* reserved */
	-1, /* reserved */
	2, /* hw_ver02.nv */
	1, /* hw_ver01.nv */
	0 /* hw_ver00.nv */
};

/**---------------------------------------------------------------------------*
 **                           Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function get adaptive value from gpio configuration
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
LOCAL uint8 PH_GetAdaptiveGpioValue(void)
{
	uint8 i = 0;
	uint8 result = 0;
	for(i = 0; bard_id_cfg[i].version_bit != 0; i++) {
		GPIO_SetDirection(bard_id_cfg[i].gpio_num, 0);
		if(GPIO_GetValue(bard_id_cfg[i].gpio_num)) {
			result |= bard_id_cfg[i].version_bit;
		}
	}
	SCI_TRACE_LOW("PH_GetAdaptiveGpioValue : GPIO value is %d\n", result);
	return result;
}

/*****************************************************************************/
//  Description:    This function get adaptive value from ADC configuration
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
LOCAL uint8 PH_GetAdaptiveADCValue(void)
{
	uint32 vol= 0 ,level= -1;

	vol = ADC_GetVoltageByCurrent_Extend(ADIN_1, ADC_SCALE_1V250, 2000, 10);    //set isne is 20uA,get val

	SCI_TRACE_LOW("PH_GetAdaptiveADCValue :The voltage is = %d\n",vol);
	if (vol <= 0 || vol >= 1200) {
		SCI_TRACE_LOW("PH_GetAdaptiveADCValue: vol is out of ranger [0~1200]\n");
		return -1;
	}
	if (vol >= 5 && vol <= 24)
		level = 0;
	if (vol >= 37 && vol <= 51)
		level = 1;
	if (vol >= 115 && vol <= 157)
		level = 2;
	if (vol >= 170 && vol <= 230)
		level = 3;
	if (vol >= 306 && vol <= 414)
		level = 4;
	if (vol >= 459 && vol <= 621)
		level = 5;
	if (vol >= 663 && vol <= 897)
		level = 6;
	if (vol >= 952 && vol <= 1230)
		level = 7;

	if(level < 0 || level > 7) {
		SCI_TRACE_LOW("Adc value of rf band if is wrong, the rfboard.id will be set to -1\n");
		return -1;
	}

	return adc2deltanv_table[level];
}

/*****************************************************************************/
//  Description:    This function get hw version from gpio and adc
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_Get_RF_VERSION(void)
{
	uint32 hw_version = 0;
#ifdef FORCECHANGE_SUPPORT
	hw_version = PH_GetAdaptiveADCValue();
#endif
	return hw_version;
}

PUBLIC uint32 SCI_Get_PA_VERSION(void)
{
	uint32 PA_version = 0;
#ifdef FORCECHANGE_SUPPORT
	PA_version = (PH_GetAdaptiveGpioValue() & ( bard_id_cfg[1].version_bit | bard_id_cfg[2].version_bit )) >>  bard_id_cfg[1].version_bit;
#endif
	return PA_version;
}

PUBLIC crystal_type_t SCI_Get_Crystal_Type(void)
{
	uint32 result = 0;
	crystal_type_t crystal_type = 0;
#ifdef FORCECHANGE_SUPPORT
	result = PH_GetAdaptiveGpioValue();
	if(result == 0) {
		crystal_type = DCXO;
	} else if(result == 1) {
		crystal_type = TSX;
	}
#endif
	return crystal_type;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of nv_adaptive_cfg.c
