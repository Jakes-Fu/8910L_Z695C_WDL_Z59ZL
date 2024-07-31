/******************************************************************************
 ** File Name:      sprd_codec_ap_phy_sc2720_headset.c                                    *
 ** Author:         yuzhen.long                                               *
 ** DATE:           15/04/2021                                                *
 ** Copyright:      2021 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec headset related operate                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 15/04/2021    yuzhen.long     Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"

#include "audio_drvapi.h"
//#include "sprd_codec_phy.h"
#include "sprd_codec_ap_phy_sc2720.h"
#include "sprd_codec_ap_phy_sc2720_headset.h"
#include "ldo_drvapi.h"
#include "adc_drvapi.h"
#include "rda2720_glb.h"
//#include "efuse_phy_uix8910_pmic.h"

#define SPRD_CODEC_DP_PRINT    SCI_TRACE_LOW
#define  SC2720_HEADSET_AUXADC_CAL_NO 0
#define  SC2720_HEADSET_AUXADC_CAL_DO 1
#define SC2720_DELTA1_BLOCK20 20
#define SC2720_DELTA2_BLOCK22 22
#define SC2720_BITCOUNT 16
#define SC2720_BLK_WIDTH 16
#define SC2720_PROTECT_BIT (0)

#define SPRD_CODEC_AP_REG_BF_SET_VAL(_Reg, _Value, _Lsb, _Mask)\
	SPRD_CODEC_AP_WRITE_BITS(_Reg,(_Value<<_Lsb),_Mask)

LOCAL SPRD_CODEC_AP_REG_CTL_T * aud_codec_ctrl_ptr = (SPRD_CODEC_AP_REG_CTL_T *)SC2720_AP_REG_BASE;
LOCAL SPRD_AUD_CFGA_REG_CTL_T * aud_cfga_ctrl_ptr = (SPRD_AUD_CFGA_REG_CTL_T *)SC2720_AUD_CFGA_REG_BASE;


LOCAL HEADSET_INFO_T s_headset_cfg=
{
        3357,	// 2.65v large scale 3.233v
        491,//0.15V
        2948,//0.9V
        164,//0.05V
        328,// 0.1v
        0,
        0,
        0,
        0,
        0,
        0,
        939836,
};

#ifdef HP_JACK_NORMAL_CLOSED
#define HEADSET_NO_MIC_MIN_ADC     0
#define HEADSET_NO_MIC_MAX_ADC     112
#define HEADSET_4POLE_NORMAL_MIN_ADC    1118
#define HEADSET_4POLE_NORMAL_MAX_ADC    3020
#define HEADSET_4POLE_ABNORMAL_MIN_ADC  335
#define HEADSET_4POLE_ABNORMAL_MAX_ADC  1006
#endif

#define SC2720_HEADMIC_ADC_CHANNEL (20)

#define SC2720_SCI_ADC_GET_VALUE_COUNT (10)

LOCAL T_HEADPHONE_STATUS g_HeadPhoneStatus = {0,UNKNOWN_MIC_TYPE};
/* summer for calculation usage */
static SC2720_HEADSET_AUXADC_CAL_T g_SC2720_ADC_CAL_HEADSET = {
    0, 0, 0, 0,  SC2720_HEADSET_AUXADC_CAL_NO,
};

//extern BOOLEAN g_ap_status;
/*****************************************************************************/
//  Description:    This function is used to get headset configure information
//  Author:         brady.wang
//  Note:
/*****************************************************************************/
PUBLIC void _sprd_codec_ap_en_op(uint32 en, void* reg, uint32 bit)
{
	if (en)
		SPRD_CODEC_AP_WRITE_BITS((uint32)reg, (bit), (bit));
	else
		SPRD_CODEC_AP_WRITE_BITS((uint32)reg, ~(bit), (bit));
}

LOCAL void _sprd_codec_ap_drv_delay(uint32 delay)
{
	SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_dcl0, delay,
		DRV_SOFT_T, \
		DRV_SOFT_T_MASK \
		);
}

/// ANA_PMU0

//ANA_PMU0 start
LOCAL void _sprd_codec_ap_LDO_vb_en(uint32 en)
{
	_sprd_codec_ap_en_op( en,&aud_codec_ctrl_ptr->ana_pmu0, VB_EN);
}

LOCAL void _sprd_codec_ap_drv_soft_start_en(uint32 en)
{
	_sprd_codec_ap_en_op( en,&aud_codec_ctrl_ptr->ana_dcl0, DRV_SOFT_EN);
}

 LOCAL void _sprd_codec_ap_dcl_en(uint32 en)
{
	_sprd_codec_ap_en_op( en,&aud_codec_ctrl_ptr->ana_dcl0, DCL_EN);
}

LOCAL void _sprd_codec_ap_vbg_sel(uint32 voltage_sel)
{
	_sprd_codec_ap_en_op( voltage_sel,&aud_codec_ctrl_ptr->ana_pmu0, VBG_SEL);
}

LOCAL void _sprd_codec_ap_mic_bias_power_down(uint32 down)
{
	_sprd_codec_ap_en_op( down,&aud_codec_ctrl_ptr->ana_pmu0, MICBIAS_PLGB);
}

LOCAL void _sprd_codec_ap_vbg_temp_tune(uint32 option)
{
	SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_pmu0, option,\
		VBG_TEMP_TUNE,  VBG_TEMP_TUNE_MASK);
}

PUBLIC HEADSET_INFO_T_PTR SPRD_CODEC_HEADSET_GetInfo(void)
{
	HEADSET_INFO_T_PTR cfg_ptr = PNULL;

	cfg_ptr = &s_headset_cfg;
	return cfg_ptr;
}
PUBLIC void _sprd_codec_ap_aud_hid_en(uint32 en)
{
	_sprd_codec_ap_en_op( en,&aud_cfga_ctrl_ptr->clk_en, CLK_AUD_HID);
}

PUBLIC void _sprd_codec_ap_LDO_vb_sleep_mode_power_down(uint32 down)
{
	_sprd_codec_ap_en_op( down,&aud_codec_ctrl_ptr->ana_pmu0, VB_HDMC_SP_PD);
}

PUBLIC void _sprd_codec_ap_bg_en(uint32 en)
{
	_sprd_codec_ap_en_op( en,&aud_codec_ctrl_ptr->ana_pmu0, BG_EN);
}


PUBLIC void _sprd_codec_ap_aud_bias_en(uint32 en)
{
	_sprd_codec_ap_en_op( en,&aud_codec_ctrl_ptr->ana_pmu0, BIAS_EN);
}

PUBLIC void _sprd_codec_ap_headmic_bias_en(uint32 en)
{
	_sprd_codec_ap_en_op( en,&aud_codec_ctrl_ptr->ana_pmu0, HMIC_BIAS_EN);
}


LOCAL void _sprd_codec_headmic_adc_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&aud_codec_ctrl_ptr->ana_hdt0,HEDET_BUF_EN);
    _sprd_codec_ap_en_op(en,&aud_codec_ctrl_ptr->ana_hdt0,HEDET_V21_EN);
}
LOCAL void _sprd_codec_headmic_detect_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&aud_codec_ctrl_ptr->ana_hdt0,HEDET_MICDET_EN);
    //_sprd_codec_ap_en_op(en,&aud_codec_ctrl_ptr->ana_hdt0,HEDET_V21_EN);
}

PUBLIC void _sprd_codec_headset_lint_low_det_filter_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&aud_codec_ctrl_ptr->ana_hdt0,HEDET_LDET_L_FILTER);
}

LOCAL void _sprd_codec_set_auxadc_to_headmic(uint32 is_headmic)
{
    if (is_headmic)
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt0, \
    	HEDET_MUX2ADC_SEL_HEADMIC_IN_DETECT,HEDET_MUX2ADC_SEL, \
    	HEDET_MUX2ADC_SEL_MASK );
    else
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt0, \
    	HEDET_MUX2ADC_SEL_HEADSET_L_INT,HEDET_MUX2ADC_SEL, \
    	HEDET_MUX2ADC_SEL_MASK );
}

LOCAL void _sprd_codec_headset_button_detect_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_hdt0, HEDET_BDET_EN);
}

LOCAL void _sprd_codec_set_auxadc_scale(uint32 is_big_scale)
{
    _sprd_codec_ap_en_op(is_big_scale, &aud_codec_ctrl_ptr->ana_hdt0, HEDET_V2AD_SCALE);
}
LOCAL void _sprd_codec_hedet_vref_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_hdt0, HEDET_VREF_EN);
}

LOCAL void _sprd_codec_headset_plugout_det_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_hdt2, PLGPD_EN);
}

LOCAL void _sprd_codec_hpl_det_delay_en(uint32 en)
{
    _sprd_codec_ap_en_op(en, &aud_codec_ctrl_ptr->ana_hdt2, HPL_EN_D2HDT_EN);
}

LOCAL void _sprd_codec_headset_adc_cal_from_efuse(void)
{
	uint8 delta[4] = {0};
	uint32 block0_bit7 = 128;
	uint32 test[2] = {0};
	unsigned int adie_chip_id;

	SCI_TRACE_LOW("to get calibration data from efuse ...\n");
	if (g_SC2720_ADC_CAL_HEADSET.cal_type != SC2720_HEADSET_AUXADC_CAL_NO) {
		SCI_TRACE_LOW("efuse A,B,E has been calculated already!\n");
		return;
	}

	test[0] = sprd_pmic_efuse_read_bits(SC2720_DELTA1_BLOCK20*SC2720_BLK_WIDTH, SC2720_BITCOUNT);
	test[1] = sprd_pmic_efuse_read_bits(SC2720_DELTA2_BLOCK22*SC2720_BLK_WIDTH, SC2720_BITCOUNT);

	delta[0] = test[0] & 0xFF;
	delta[1] = (test[0] & 0xFF00) >> 8;
	delta[2] = test[1] & 0xFF;
	delta[3] = (test[1] & 0xFF00) >> 8;

	SCI_TRACE_LOW("test[0] 0x%x %d, test[1] 0x%x %d\n",
		test[0], test[0], test[1], test[1]);

	SCI_TRACE_LOW("d[0] %#x %d d[1] %#x %d d[2] %#x %d d[3] %#x %d\n",
			delta[0], delta[0], delta[1], delta[1],
			delta[2], delta[2],  delta[3], delta[3]);

	block0_bit7 = sprd_pmic_efuse_read_bits(0, SC2720_BITCOUNT);
	SCI_TRACE_LOW("block_7 0x%08x\n", block0_bit7);
	if (!(block0_bit7&(1<<SC2720_PROTECT_BIT))) {
		SCI_TRACE_LOW("block 0 bit 7 set 1 no efuse data\n");
		return;
	}

	g_SC2720_ADC_CAL_HEADSET.cal_type = SC2720_HEADSET_AUXADC_CAL_DO;
	SCI_TRACE_LOW("block 0 bit 7 set 0 have efuse data\n");
	g_SC2720_ADC_CAL_HEADSET.A = (delta[0] - 128 + 80) * 4;
	g_SC2720_ADC_CAL_HEADSET.B =  (delta[1] - 128 + 833) * 4;
	g_SC2720_ADC_CAL_HEADSET.E1 = delta[2] * 2 + 2500;
	g_SC2720_ADC_CAL_HEADSET.E2 = delta[3] * 4 + 1300;

        SCI_TRACE_LOW("A %d, B %d E1 %d E2 %d\n",
		g_SC2720_ADC_CAL_HEADSET.A, g_SC2720_ADC_CAL_HEADSET.B,
		g_SC2720_ADC_CAL_HEADSET.E1, g_SC2720_ADC_CAL_HEADSET.E2);
}

LOCAL uint32 _sprd_codec_ap_adc_get(uint32 is_big_scale)
{
    int32 count = 0;
    int32 average = 0;
    int32 val = 0;
    int32 ret = 0;

    _sprd_codec_set_auxadc_scale(is_big_scale);

    while (count < SC2720_SCI_ADC_GET_VALUE_COUNT)
    {
        val= ADC_GetResultDirectly(SC2720_HEADMIC_ADC_CHANNEL,!is_big_scale);
        average += val;
        count++;
    }
    average /= SC2720_SCI_ADC_GET_VALUE_COUNT;
    SCI_TRACE_LOW("_sprd_codec_ap_adc_get:average adc: %d\n", average);

    return average;
}

PUBLIC uint32 _sprd_codec_ap_sc2720_headmic_adcget(uint32 is_big_scale)
{
    uint32 adc_Value = 0;

    _sprd_codec_ap_en_op(0,&aud_codec_ctrl_ptr->ana_hdt0,HEDET_BUF_CHOP);
    adc_Value = _sprd_codec_ap_adc_get(is_big_scale);
    SCI_TRACE_LOW("adc_value is %d\n", adc_Value);
    _sprd_codec_ap_en_op(1,&aud_codec_ctrl_ptr->ana_hdt0,HEDET_BUF_CHOP);
    adc_Value = _sprd_codec_ap_adc_get(is_big_scale) + adc_Value;
    SCI_TRACE_LOW("adc_value is %d\n", adc_Value/2);
    return adc_Value/2;
}

LOCAL int _sprd_codec_ap_headmic_get_adcideal(uint32 adc_mic, uint32 coefficient)
{
    int64 numerator = 0;
    int64 denominator = 0;
    uint32 adc_ideal = 0;
    uint32 a, b, e1, e2;
    int64 exp1, exp2, exp3, exp4;
    uint64 dividend;
    uint32 divisor;
    if (g_SC2720_ADC_CAL_HEADSET.cal_type != SC2720_HEADSET_AUXADC_CAL_DO)
    {
        SCI_TRACE_LOW("headset: efuse A,B,E hasn't been calculated!\n");
        return adc_mic;
    }

    a = g_SC2720_ADC_CAL_HEADSET.A;
    b = g_SC2720_ADC_CAL_HEADSET.B;
    e1 = g_SC2720_ADC_CAL_HEADSET.E1;
    e2 = g_SC2720_ADC_CAL_HEADSET.E2;

    if (9*adc_mic + b < 10*a)
        return adc_mic;
// TODO:need check with ASIC
    /*1.22v new calibration need*/
    exp1 =  ((int64)e1 - (int64)e2);
    exp2 = (9 * (int64)adc_mic - 10 * (int64)a + (int64)b);
    exp3 = 24 * (int64)e1 * ((int64)b - (int64)a);
    exp4 = ((int64)e1 - (int64)e2 - 1200) * exp2;

    SCI_TRACE_LOW("exp1=%d, exp2=%d, exp3=%d, exp4=%d\n",
        exp1, exp2, exp3, exp4);
    denominator = exp3 + exp4;
    numerator = coefficient * (exp1 + 1200) * exp2;
    SCI_TRACE_LOW("denominator=%d, numerator=%d\n",
            denominator, numerator);
    //do_div(numerator, 100);
    numerator = numerator / 100;
    SCI_TRACE_LOW("denominator=%d, numerator=%d\n",
            denominator, numerator);
    /*enable the denominator * 0.01
    *  numerator * 0.01 at the same time
    *  for do_div() argument divisor is u32
    *  and dividend is u64 but divsor is over 32bit
    */
    //do_div(denominator, 100);
    //do_div(numerator, 100);
    denominator = denominator / 100;
    numerator = numerator / 100;
    SCI_TRACE_LOW("denominator=%lld, numerator=%lld\n",
            denominator, numerator);

    divisor = (uint32)(denominator);
    dividend = (uint64)(numerator);
    SCI_TRACE_LOW("divisor=%u, dividend=%llu\n", divisor, dividend);

    //do_div(dividend, divisor);
    dividend = dividend / divisor;
    adc_ideal = (uint64)dividend;
    SCI_TRACE_LOW("adc_mic=%d, adc_ideal=%d\n", adc_mic, adc_ideal);

    return adc_ideal;
}

PUBLIC uint32 __sprd_codec_ap_get_headbutton_value(void )
{
    uint32 adc_mic_average = 0;
    uint32 adc_mic_ideal = 0;
    int val;

    HEADSET_INFO_T_PTR cfg_ptr = PNULL;

    if (!g_HeadPhoneStatus.Connected)
    {
        SCI_TRACE_LOW("__sprd_codec_ap_get_headbutton_value headphone not connect");
        return 0;
    }
    cfg_ptr = SPRD_CODEC_HEADSET_GetInfo();
    SCI_TRACE_LOW("__sprd_codec_ap_get_headbutton_value entry");
    /* change to little scale */
    _sprd_codec_set_auxadc_scale(0);
        /* Get adc value of headmic in. */
    _sprd_codec_set_auxadc_to_headmic(1);
    _sprd_codec_headmic_adc_en(1);
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt0);
    SCI_TRACE_LOW("__sprd_codec_ap_get_headbutton_value %d: ana_hdt0: %x\n", __LINE__, val);
    adc_mic_average = _sprd_codec_ap_sc2720_headmic_adcget(0);
    adc_mic_ideal = _sprd_codec_ap_headmic_get_adcideal(adc_mic_average,
                            cfg_ptr->coefficient);
    SCI_TRACE_LOW("headset button:little scale adc_mic_average= %d adc_mic_ideal = %d\n", adc_mic_average,adc_mic_ideal);

    return adc_mic_ideal;
}


#if 1
PUBLIC void _sprd_codec_headset_detect_reg_init(void)
{
    uint32 val;

    //headset_detect_clk_en();
    _sprd_codec_ap_en_op( 1,ANA_REG_GLB_MODULE_EN0, BIT_AUD_EN);

    //BIT4 AUD_EN.  MODULE_EN0
    _sprd_codec_ap_aud_hid_en(1);

    /* default ANA_HDT0=0a02*/
    /* must set in initial phase */
    _sprd_codec_hedet_vref_en(1);
    _sprd_codec_set_auxadc_scale(1);
    /* headset_reg_set_bits(ANA_HDT0, BIT(HEDET_BUF_CHOP)); */
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt0, HEDET_V21_SEL_1U,\
    HEDET_V21_SEL,  HEDET_V21_SEL_MASK);
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt0);
    SCI_TRACE_LOW("_sprd_codec_headset_detect_reg_init %d: ana_hdt0: %x\n", __LINE__, val);

    /* default ANA_HDT1 = 0xce05 */
    /* bit15-13 = 110 */
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_MICDET_REF_SEL_2P6,\
    HEDET_MICDET_REF_SEL,  HEDET_MICDET_REF_SEL_MASK);

    /* bit12-11 = 01 */
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_MICDET_HYS_SEL_20MV,\
    HEDET_MICDET_HYS_SEL,  HEDET_MICDET_HYS_SEL_MASK);

    /* bit10-8 = 110 */
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_LDET_REFL_SEL_300MV,\
    HEDET_LDET_REFL_SEL,  HEDET_LDET_REFL_SEL_MASK);

    /* bit7-6 = 0 */
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_LDET_REFH_SEL_1P7,\
    HEDET_LDET_REFH_SEL,  HEDET_LDET_REFH_SEL_MASK);

    /* bit5-4 = 0 must set in initial phase */
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_LDET_PU_PD_PU,\
    HEDET_LDET_PU_PD,  HEDET_LDET_PU_PD_MASK);

    /* bit3-2 = 01 */
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_LDET_L_HYS_SEL_20MV,\
    HEDET_LDET_L_HYS_SEL,  HEDET_LDET_L_HYS_SEL_MASK);

    /* bit1-0 = 01 */
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_LDET_H_HYS_SEL_20MV,\
    HEDET_LDET_H_HYS_SEL,  HEDET_LDET_H_HYS_SEL_MASK);

    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt1);
    SCI_TRACE_LOW("_sprd_codec_headset_detect_reg_init %d: ana_hdt1: %x\n", __LINE__, val);

    /* default ANA_HDT2 = 0x0534*/
    //val = HEDET_BDET_HYS_SEL_20MV << HEDET_BDET_HYS_SEL;
    //msk = HEDET_BDET_HYS_SEL_MASK << HEDET_BDET_HYS_SEL;
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt2, HEDET_BDET_HYS_SEL_20MV,\
    HEDET_BDET_HYS_SEL,  HEDET_BDET_HYS_SEL_MASK);
       /* bit10-7 = 1010 bdet_ref_v=0.85v*/
    SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt2, HEDET_BDET_REF_SEL_850MV,\
    HEDET_BDET_REF_SEL,  HEDET_BDET_REF_SEL_MASK);

#ifdef HP_JACK_NORMAL_CLOSED        
        _sprd_codec_headset_plugout_det_en(0);
#else        
        _sprd_codec_headset_plugout_det_en(1);
#endif
    _sprd_codec_hpl_det_delay_en(1);
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt2);
    SCI_TRACE_LOW("_sprd_codec_headset_detect_reg_init %d: ana_hdt2: %x\n", __LINE__, val);

    /*ANA_DCL0 = 0x46
    headset_reg_write(ANA_DCL0, 0x46, 0xFFFF);
    headset_reg_read(ANA_DCL0, &val);
    pr_info("%s: ANA_DCL0: %#x\n", __func__, val);*/
    _sprd_codec_ap_drv_delay(1);
    _sprd_codec_ap_drv_soft_start_en(1);
    _sprd_codec_ap_dcl_en(1);
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_dcl0);
    SCI_TRACE_LOW("_sprd_codec_headset_detect_reg_init %d: ana_dcl0: %x\n", __LINE__, val);

    /*ANA_PMU0 init config*/
    _sprd_codec_ap_vbg_sel(1);// 1: BG=1.5V 0:1.55v
    _sprd_codec_ap_mic_bias_power_down(0);// 0:power up
    _sprd_codec_ap_vbg_temp_tune(VBG_TEMP_TUNE_TC_REDUCE);
	
	_sprd_codec_ap_LDO_vb_sleep_mode_power_down(1);
	_sprd_codec_ap_LDO_vb_en(1);

    _sprd_codec_ap_en_op( 1,&aud_codec_ctrl_ptr->ana_pmu0, HMIC_COMP_MODE1_EN);
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_pmu0);
    SCI_TRACE_LOW("_sprd_codec_headset_detect_reg_init %d: ANA_PMU0: %x\n", __LINE__, val);

}


PUBLIC void _sprd_codec_sc2720_init(void)
{
	int32  val;
	_sprd_codec_headset_detect_reg_init();
	SCI_TRACE_LOW("_sprd_codec_sc2720_init finish%d:", __LINE__);
}

#endif

PUBLIC uint32 _sprd_codec_head_mic_insert_sts()
{
	return SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_sts7)
		&(HEAD_INSERT);
}
#if 0
PUBLIC void __sprd_codec_headset_type_related_oper(SC2720_HEADSET_TYPE_T headset_type)
{
	g_HeadPhoneStatus.MicType = headset_type;

	_sprd_codec_headmic_adc_en(0);
	_sprd_codec_headset_lint_low_det_filter_en(1);
	if(g_ap_status)
	{
		_sprd_codec_ap_LDO_vb_sleep_mode_power_down(1);
	}else
	{
		_sprd_codec_ap_bg_en(0);
		_sprd_codec_ap_LDO_vb_sleep_mode_power_down(0);
	}
	switch (headset_type)
 	{
	case HEADSET_TYPE_ERR:
            _sprd_codec_ap_headmic_bias_en(0);
            //_sprd_codec_ap_vbg_sel(0);
            //_sprd_codec_ap_vbg_temp_tune(VBG_TEMP_TUNE_NORMAL);
            _sprd_codec_headmic_detect_en(0);
            _sprd_codec_hpl_det_delay_en(0);
      		SCI_TRACE_LOW("headset_type = %d (HEADSET_TYPE_ERR)\n",headset_type);
		break;
	case HEADSET_4POLE_NOT_NORMAL:
            _sprd_codec_ap_headmic_bias_en(0);
           // _sprd_codec_ap_vbg_sel(0);
            //_sprd_codec_ap_vbg_temp_tune(VBG_TEMP_TUNE_NORMAL);
            _sprd_codec_headmic_detect_en(0);
            _sprd_codec_hpl_det_delay_en(0);
		SCI_TRACE_LOW("headset_type = %d (HEADSET_4POLE_NOT_NORMAL)\n",headset_type);
		break;
	case HEADSET_4POLE_NORMAL:
             _sprd_codec_headset_button_detect_en(1);
             _sprd_codec_headmic_detect_en(1);


		SCI_TRACE_LOW("headset_type = %d (HEADSET_4POLE_NORMAL)\n",headset_type);
		break;
	case HEADSET_NO_MIC:
            _sprd_codec_ap_headmic_bias_en(0);
            //_sprd_codec_ap_vbg_sel(0);
           // _sprd_codec_ap_vbg_temp_tune(VBG_TEMP_TUNE_NORMAL);
            _sprd_codec_headmic_detect_en(0);
            _sprd_codec_hpl_det_delay_en(0);
		SCI_TRACE_LOW("headset_type = %d (HEADSET_NO_MIC)\n",headset_type);
		break;
	case HEADSET_APPLE:
		SCI_TRACE_LOW("headset_type = %d (HEADSET_APPLE)\n",headset_type);
		break;
	default:
		SCI_TRACE_LOW("headset_type = %d (HEADSET_UNKNOWN)\n",headset_type);
		break;
	}

}
#endif

PUBLIC SC2720_HEADSET_TYPE_T __sprd_codec_ap_headmic_type_detect(void)
{
    uint32 adc_mic_average = 0;
    uint32 adc_left_average = 0;
    uint32 adc_mic_ideal = 0;
    uint32 retry_times = 10;
    int val;

    HEADSET_INFO_T_PTR cfg_ptr = PNULL;

    cfg_ptr = SPRD_CODEC_HEADSET_GetInfo();
    SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect entry");
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt0);
    SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_hdt0: %x\n", __LINE__, val);
#ifdef HP_JACK_NORMAL_CLOSED
    SCI_TRACE_LOW("headset: now get adc value  in little scale for HP_JACK_NORMAL_CLOSED\n");
       _sprd_codec_headmic_adc_en(1);
    SCI_Sleep( 10);

    _sprd_codec_ap_aud_hid_en(1);

    while(retry_times>0)
    {
        SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect  entry");
        /* set large scale */
        _sprd_codec_set_auxadc_scale(1);
        _sprd_codec_set_auxadc_to_headmic(1);
        SCI_Sleep( 10);
        adc_mic_average =  _sprd_codec_ap_sc2720_headmic_adcget(1);
        adc_mic_average = _sprd_codec_ap_headmic_get_adcideal(adc_mic_average,
                            cfg_ptr->coefficient);
        if ((adc_mic_average > cfg_ptr->headset_stable_max_value) ||(adc_mic_average == -1))// 2.65v
        {
                retry_times--;
                SCI_Sleep( 100);
                SCI_TRACE_LOW("headset: retry time is %d adc_mic_average %d\n",
                    retry_times, adc_mic_average);
                if(0==retry_times)
            return HEADSET_TYPE_ERR;
        }else
        {
            SCI_TRACE_LOW("headset: retry time is %d adc_mic_average=%d<headset_stable_max_value ",retry_times,adc_mic_average);
            break;
        }
    }
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt0);
    SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_hdt0: %x\n", __LINE__, val);

    //SCI_TRACE_LOW("headset: adc_mic_average = %d\n", adc_mic_average);
    SCI_TRACE_LOW("headset: now get adc value  in little scale\n");
    /* change to little scale */
    _sprd_codec_set_auxadc_scale(0);
    /* get adc value of left */
    _sprd_codec_set_auxadc_to_headmic(0);
    SCI_Sleep( 10);
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt0);
    SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_hdt0: %x\n", __LINE__, val);

    adc_left_average = _sprd_codec_ap_sc2720_headmic_adcget(0);
    SCI_TRACE_LOW("headset: adc_left_average = %d\n", adc_left_average);

    /* Get adc value of headmic in. */
    _sprd_codec_set_auxadc_to_headmic(1);
    adc_mic_average = _sprd_codec_ap_sc2720_headmic_adcget(0);
    SCI_TRACE_LOW("headset:little scale adc_mic_average1 = %d\n", adc_mic_average);

    adc_mic_ideal = _sprd_codec_ap_headmic_get_adcideal(adc_mic_average,
                            cfg_ptr->coefficient);
    //if (adc_mic_ideal >= 0)
        adc_mic_average = adc_mic_ideal;
    SCI_TRACE_LOW("headset: adc_mic_average2 = %d\n", adc_mic_average);

       if (adc_mic_average > HEADSET_4POLE_NORMAL_MIN_ADC)// 0.9v
        {
                    /* bit10-8 change from 110 to 001 HDT1*/
            //SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_LDET_REFL_SEL_50MV,\
            //HEDET_LDET_REFL_SEL,  HEDET_LDET_REFL_SEL_MASK);
            //_sprd_codec_headset_button_detect_en(1);
            SCI_TRACE_LOW("headset: HEADSET_4POLE_NORMAL\n");
            return HEADSET_4POLE_NORMAL;
        }else{
            if(_sprd_codec_head_mic_insert_sts()==0)
            {
                SCI_TRACE_LOW("headset: 4pole HEADSET  mic abnormal \n");
                return HEADSET_NO_MIC;
            }
            SCI_TRACE_LOW("headset: HEADSET_NO_MIC\n");
            return HEADSET_NO_MIC;
        }
#else
    _sprd_codec_headmic_adc_en(1);
    SCI_Sleep( 10);

    _sprd_codec_ap_aud_hid_en(1);

    while(retry_times>0)
    {
            SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect  entry");
        /* set large scale */
        _sprd_codec_set_auxadc_scale(1);
        _sprd_codec_set_auxadc_to_headmic(1);
        SCI_Sleep( 10);
        adc_mic_average =  _sprd_codec_ap_sc2720_headmic_adcget(1);
        adc_mic_average = _sprd_codec_ap_headmic_get_adcideal(adc_mic_average,
                            cfg_ptr->coefficient);
        if ((adc_mic_average > cfg_ptr->headset_stable_max_value) ||(adc_mic_average == -1))// 2.65v
        {
                retry_times--;
                SCI_Sleep( 100);
                SCI_TRACE_LOW("headset: retry time is %d adc_mic_average %d\n",
                    retry_times, adc_mic_average);
                if(0==retry_times)
            return HEADSET_TYPE_ERR;
        }else
        {
            SCI_TRACE_LOW("headset: retry time is %d adc_mic_average=%d<headset_stable_max_value ",retry_times,adc_mic_average);
            break;
        }
    }
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt0);
    SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_hdt0: %x\n", __LINE__, val);

    //SCI_TRACE_LOW("headset: adc_mic_average = %d\n", adc_mic_average);
   SCI_TRACE_LOW("headset: now get adc value  in little scale\n");
    /* change to little scale */
    _sprd_codec_set_auxadc_scale(0);
    /* get adc value of left */
    _sprd_codec_set_auxadc_to_headmic(0);
     SCI_Sleep( 10);
     val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt0);
	SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_hdt0: %x\n", __LINE__, val);

    adc_left_average = _sprd_codec_ap_sc2720_headmic_adcget(0);
     SCI_TRACE_LOW("headset: adc_left_average = %d\n", adc_left_average);

    /* Get adc value of headmic in. */
    _sprd_codec_set_auxadc_to_headmic(1);
    adc_mic_average = _sprd_codec_ap_sc2720_headmic_adcget(0);
    SCI_TRACE_LOW("headset:little scale adc_mic_average1 = %d\n", adc_mic_average);

    adc_mic_ideal = _sprd_codec_ap_headmic_get_adcideal(adc_mic_average,
                            cfg_ptr->coefficient);
    //if (adc_mic_ideal >= 0)
        adc_mic_average = adc_mic_ideal;
    SCI_TRACE_LOW("headset: adc_mic_average2 = %d\n", adc_mic_average);
#if 0
    val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt0);
	SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_hdt0: %x\n", __LINE__, val);
        val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt1);
	SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_hdt1: %x\n", __LINE__, val);
        val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_hdt2);
	SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_hdt2: %x\n", __LINE__, val);
        val = SPRD_CODEC_AP_REG_GET(&aud_codec_ctrl_ptr->ana_pmu0);
	SCI_TRACE_LOW("__sprd_codec_ap_headmic_type_detect %d: ana_pmu0: %x\n", __LINE__, val);
#endif
    if (adc_left_average < cfg_ptr->headset_one_half_adc_gnd) // 0.15v
    {
        if (adc_mic_average < cfg_ptr->headset_adc_threshold_3pole_detect)// 0.9v
        {
            SCI_TRACE_LOW("headset: HEADSET_NO_MIC\n");
            return HEADSET_NO_MIC;
        }
#if 0
	if(_sprd_codec_head_mic_insert_sts()==0)
	{
		SCI_TRACE_LOW("headset: 4pole HEADSET  mic abnormal \n");
		return HEADSET_NO_MIC;
	}
#endif
        if (adc_left_average < cfg_ptr->headset_half_adc_gnd) // 0.05v
        {
            /* bit10-8 change from 110 to 001 HDT1*/
            SPRD_CODEC_AP_REG_BF_SET_VAL(&aud_codec_ctrl_ptr->ana_hdt1, HEDET_LDET_REFL_SEL_50MV,\
            HEDET_LDET_REFL_SEL,  HEDET_LDET_REFL_SEL_MASK);
        }
        _sprd_codec_headset_button_detect_en(1);
        SCI_TRACE_LOW("headset: HEADSET_4POLE_NORMAL\n");
        return HEADSET_4POLE_NORMAL;
    }
    else if  (ABS(adc_mic_average - adc_left_average) < cfg_ptr->headset_adc_gnd)// 0.1v
    {
        SCI_TRACE_LOW("headset: HEADSET_4POLE_NOT_NORMAL\n");
        return HEADSET_4POLE_NOT_NORMAL;
    }
    else
    {
        SCI_TRACE_LOW("headset: HEADSET_TYPE_ERR\n");
        return HEADSET_TYPE_ERR;
    }
#endif
}

PUBLIC void __sprd_codec_set_SC2720_headset_connection_status(uint32 status)
{

    SCI_TRACE_LOW("__sprd_codec_set_headset_connection_status line%d: status: %x\n", __LINE__, status);
    if (status) {
	_sprd_codec_headset_lint_low_det_filter_en(1);
        _sprd_codec_headset_detect_reg_init();
        _sprd_codec_headmic_detect_en(1);
        _sprd_codec_ap_aud_bias_en(1);
        _sprd_codec_ap_headmic_bias_en(1);
         if (g_SC2720_ADC_CAL_HEADSET.cal_type ==  SC2720_HEADSET_AUXADC_CAL_NO)
        {
            _sprd_codec_headset_adc_cal_from_efuse();
        }
    }else{
		_sprd_codec_headset_lint_low_det_filter_en(0);
#ifdef HP_JACK_NORMAL_CLOSED
            _sprd_codec_headset_plugout_det_en(1);
#endif
		_sprd_codec_ap_headmic_bias_en(0);
		_sprd_codec_headmic_adc_en(0);
		_sprd_codec_headset_button_detect_en(0);
		_sprd_codec_headmic_detect_en(0);
#ifdef HP_JACK_NORMAL_CLOSED
            _sprd_codec_headset_plugout_det_en(0);
#else
            _sprd_codec_headset_plugout_det_en(0);
#endif
		SPRD_CODEC_AP_REG_SET(&aud_codec_ctrl_ptr->ana_sts0,0x0000);
    }
    g_HeadPhoneStatus.Connected = status;
    g_HeadPhoneStatus.MicType = UNKNOWN_MIC_TYPE;
}


