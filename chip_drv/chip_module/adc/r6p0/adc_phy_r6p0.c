/******************************************************************************
 ** File Name:      adc_phy_v5.c                                                 *
 ** Author:         Mingwei.Zhang                                                    *
 ** DATE:           05/28/2010                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                 Edit          History                                     *
 **---------------------------------------------------------------------------*
 ** DATE            NAME          DESCRIPTION                                 *
 ** 08/28/2011      lichd         Modify for sc6530                           *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "adc_reg_r6p0.h"
#include "adi_hal_internal.h"
#include "adc_drvapi.h"
#include "chip_internal.h"
#include "rda2720_glb.h"

#define _ADC_DEBUG
#ifdef _ADC_DEBUG
#include "sci_log.h"

#define ADC_ASSERT(_x_)                SCI_ASSERT(_x_)
#define ADC_VERIFY(_x_)                SCI_ASSERT(_x_)
#define ADC_ERR_TRACE(_x_)            SCI_TraceLow _x_
#define ADC_LOG_TRACE(_x_)            SCI_TraceLow _x_
#define ADC_INF_TRACE(_x_)            SCI_TraceLow _x_
#else
#define ADC_ASSERT(_x_)                ((void)0)
#define ADC_VERIFY(_x_)                ((void)(_x_))
#define ADC_ERR_TRACE(_x_)            ((void)0)
#define ADC_LOG_TRACE(_x_)            ((void)0)
#define ADC_INF_TRACE(_x_)            ((void)0)
#endif /* _ADC_DEBUG */

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define ADC_REG_OR   ANA_REG_OR
#define ADC_REG_AND  ANA_REG_AND
#define ADC_REG_GET  ANA_REG_GET
#define ADC_REG_SET  ANA_REG_SET
#define ADC_REG_MASK_OR  ANA_REG_MSK_OR

#define CAL_TYPR_NO 0
#define CAL_TYPE_NV 1
#define CAL_TYPE_EFUSE 2
/*
 * Struct
 */
typedef struct adc_para_struct
{
	uint32 small_adc_p0;
	uint32 small_adc_p1;
	uint32 small_vol_p0;
	uint32 small_vol_p1;
	uint32 small_cal_efuse_bit;

	uint32 big_adc_p0;
	uint32 big_adc_p1;
	uint32 big_vol_p0;
	uint32 big_vol_p1;
	uint32 big_cal_efuse_bit;

	uint32 adc_data_off;
}ADC_PARA_T;

typedef struct adc_efuse_struct
{
	uint32 p1_vol;
	uint32 p1_adc;
	uint32 p0_vol;
	uint32 p0_adc;
}ADC_EFUSE_T;

typedef struct adc_efuse_info_struct
{
	ADC_EFUSE_T big_scale_cal;
	ADC_EFUSE_T small_scale_cal;
}ADC_EFUSE_INFO_T;

typedef struct
{
    int32   x0_adc;
    int32   y0_vol;
    int32   slope;
    int32   zoom_in;
    uint32  cal_flag;
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UIX8910)
	//must be placed in this position
	int32   x0_adc_small_scale;
    int32   y0_vol_small_scale;
	int32	slope_small_scale;
#endif
}CHGMNG_ADC_CAL_T;

/*
 * LOCAL para
 */
LOCAL const ADC_PARA_T s_adc_cal_para = {// para from spec
	833,	80,		1000,	100,	19,
	856,	733,	4200,	3600,	18,
	128
};
LOCAL ADC_EFUSE_INFO_T s_adc_efuse_info; // info from efuse(ATE)
LOCAL CHGMNG_ADC_CAL_T adc_cal = {3505,4200,1207,1000,CAL_TYPR_NO};
/*
 * Function declaration
 */
LOCAL void _adc_get_big_scale_efuse(uint32 bit_index);
LOCAL void _adc_get_small_scale_efuse(uint32 bit_index);

/**---------------------------------------------------------------------------*
 **                         Local Define                                      *
 **---------------------------------------------------------------------------*/
// ADC device handle
LOCAL DEVICE_HANDLE s_dev_adc = SCI_NULL;

LOCAL void _adc_enable(uint32 is_en)
{
    if( is_en )
    {
        ADC_REG_OR(ADC_CTRL, ADC_CTRL_ADC_EN);
    }
    else
    {
        ADC_REG_AND(ADC_CTRL, ~ADC_CTRL_ADC_EN);
    }
}

LOCAL void _adc_int_enable(uint32 is_en)
{
    if( is_en )
    {
        ADC_REG_OR(ADC_CTRL, ADC_CTRL_ADC_EN);
    }
    else
    {
        ADC_REG_AND(ADC_CTRL, ~ADC_CTRL_ADC_EN);
    }
}

/*
    Set adc run , is cleared by HW.
*/
PUBLIC void ADC_Run( )
{
    ADC_REG_OR (ADC_CTRL, ADC_CTRL_SWCH_RUN);
}

PUBLIC void ADC_Set_Cal()
{
    ADC_REG_OR (ADC_CTRL, ADC_CTRL_OFFSET_CAL);
}

LOCAL void _adc_set_runnum( uint32 run_number )
{
    ADC_REG_AND (ADC_CTRL, ~ADC_CTRL_SWCH_RUN_NUM_MASK);
    ADC_REG_OR  (ADC_CTRL, run_number<<ADC_CTRL_SWCH_RUN_NUM_SHIFT);
}

/******************************************************************************
 *     ISR for adc in analog die
 ******************************************************************************/
PUBLIC void ADC_IsrHandler( uint32 data)
{
    uint32 adc_data=0;

    if( !(ADC_REG_GET(ADC_INT_RAW) & ADC_IRQ_MASK) )
        //SCI_TraceLow:"ADC_IsrHandler interrupt ERROR\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_92_112_2_17_23_2_29_11,(uint8*)"");

    //Get ADC result
    adc_data = ADC_REG_GET(ADC_DATA) & ADC_DATA_MSK;

    // clear interrupt status
    ADC_REG_OR( ADC_INT_CLR, ADC_IRQ_MASK);

    //SCI_TraceLow:"[ADC_PHY_V5]ADC_IsrHandler.adc_data=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_100_112_2_17_23_2_29_12,(uint8*)"d", adc_data);
}

/******************************************************************************
 * ADC_Set12Bit
 ******************************************************************************/
PUBLIC void ADC_Set12Bit( BOOLEAN is_en )
{
    if(is_en)
    {
        ADC_REG_OR(ADC_CTRL,  ADC_CTRL_MODE_12B);
    }
    else
    {
        ADC_REG_AND(ADC_CTRL,  ~ADC_CTRL_MODE_12B);
    }
}

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UIX8910)
/******************************************************************************
 * ADC_OpenTPC
 ******************************************************************************/
PUBLIC void ADC_OpenTPC (void)
{
    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCYD | ADC_HW_CH_CAPACITY_MODE); //

    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCXR | ADC_HW_CH_CAPACITY_MODE); //

    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCYU | ADC_HW_CH_CAPACITY_MODE); //

    ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ~ADC_HW_CH_ID_MASK);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_HW_CHANNEL_FOR_TPCXL | ADC_HW_CH_CAPACITY_MODE); //

    ADC_Set12Bit(SCI_TRUE);  //Default:10bit adc
}
#endif

/******************************************************************************
 * ADC_Init
 ******************************************************************************/
PUBLIC uint32 ADC_Init (void)
{
    /* Enable ADC device */
    SCI_ASSERT (DEVICE_Find ("ADC", &s_dev_adc) == CLOCK_SUCCESS); /*assert verified*/
    DEVICE_Open(s_dev_adc);

    // Reset and enable ADC module
    DEVICE_Reset(s_dev_adc);

    _adc_enable(SCI_TRUE);

#ifdef FORCECHANGE_SUPPORT
	/* Get efuse info */
	_adc_get_big_scale_efuse(
		s_adc_cal_para.big_cal_efuse_bit);
	_adc_get_small_scale_efuse(
		s_adc_cal_para.small_cal_efuse_bit);
#endif

    // Set HW_CH_DELAY[7:0] for TPC sampling
    ADC_REG_SET (ADC_HW_CH_DELAY, 0xe0);
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UIX8910)
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_HW_CH_DLY_EN);  //CHN_DLY_EN
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_HW_CH_DLY_EN);  //CHN_DLY_EN
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_HW_CH_DLY_EN);  //CHN_DLY_EN
    ADC_REG_OR  (ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_HW_CH_DLY_EN);  //CHN_DLY_EN

    ADC_OpenTPC();
#endif
    return SCI_SUCCESS;
}

/******************************************************************************
 * ADC_SetCs
 ******************************************************************************/
PUBLIC void ADC_SetCs (uint32 source)
{
    //SCI_PASSERT ( (source < ADC_CH_MAX_NUM), ("error: source < ADC_CH_MAX_NUM"));/*assert verified*/

    if (ADC_MAX <= source)
    {
        /* Assert verified: channel selected error! */
        ADC_ASSERT(("error: ADC_MAX <= source"));
    }
    ADC_REG_AND (ADC_SW_CH_CFG, ~ADC_SW_CH_ID_MASK);
    ADC_REG_OR  (ADC_SW_CH_CFG, source<<ADC_SW_CH_ID_SHIFT);

    //ADC_LOG_TRACE(("[ADC:ADC_SetCs]=cs%d", source));
}

/******************************************************************************
 * ADC_SetScale

 ******************************************************************************/
PUBLIC void ADC_SetScale (ADC_SCALE_TYPE_E scale)
{
#if 0
    if (scale == ADC_SCALE_1V2)
    {
        //Set ADC small scalel
        ADC_REG_AND (ADC_SW_CH_CFG, ~ADC_SW_CH_BIG_SCALE_EN);
    }
    else if ( (scale == ADC_SCALE_3V))
    {
        //Set ADC large scalel
        ADC_REG_OR (ADC_SW_CH_CFG, ADC_SW_CH_BIG_SCALE_EN);
    }
#else
    if ( scale < ADC_SCALE_MAX )
    {
        ADC_REG_MASK_OR (ADC_SW_CH_CFG, (scale<<ADC_SW_CH_SCALE_SHIFT), ADC_SW_CH_SCALE_MASK);
    }
#endif

     //Slow mode
    ADC_REG_OR  (ADC_SW_CH_CFG, ADC_SW_CH_SLOW_MODE_EN);

    //ADC_LOG_TRACE(("[ADC:ADC_SetScale]"));
}

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UIX8910)
PUBLIC void TPC_SetScale (BOOLEAN scale)
{
    if (scale == ADC_SCALE_1V2)
    {
        //Set ADC Little scalel
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ~ADC_SW_CH_BIG_SCALE_EN);
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ~ADC_SW_CH_BIG_SCALE_EN);

        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ~ADC_SW_CH_BIG_SCALE_EN);
        ADC_REG_AND(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ~ADC_SW_CH_BIG_SCALE_EN);
    }
    else if ( (scale == ADC_SCALE_3V))
    {
        //Set ADC Big scalel
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_SW_CH_BIG_SCALE_EN);
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_SW_CH_BIG_SCALE_EN);

        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_SW_CH_BIG_SCALE_EN);
        ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_SW_CH_BIG_SCALE_EN);
    }

    //Enable slow mode
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL), ADC_SW_CH_SLOW_MODE_EN);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL), ADC_SW_CH_SLOW_MODE_EN);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL), ADC_SW_CH_SLOW_MODE_EN);
    ADC_REG_OR(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL), ADC_SW_CH_SLOW_MODE_EN);

    //ADC_LOG_TRACE(("[ADC:TPC_SetScale]"));
}

/******************************************************************************
 * ADC_ConfigTPC
 ******************************************************************************/
PUBLIC void ADC_ConfigTPC (uint8 x, uint8 y)
{
    //ADC_LOG_TRACE:"[ADC:ADC_ConfigTPC] TPC_X,Y is bounded to ADC_SLOW_HW_CH0/CH1"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_217_112_2_17_23_2_29_13,(uint8*)"");
}

/******************************************************************************
 * ADC_CloseTPC
 ******************************************************************************/
PUBLIC void ADC_CloseTPC (void)
{
    //ADC_LOG_TRACE:"[ADC:ADC_CloseTPC] is null for sc6530."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_224_112_2_17_23_2_29_14,(uint8*)"");
}
#endif
/******************************************************************************
 * ADC_GetResultDirectly
 ******************************************************************************/
PUBLIC uint32 ADC_GetResultDirectly (ADC_E source, ADC_SCALE_TYPE_E scale)
{
    uint32 cur_tick, next_tick, result;
    uint32          ana_ctrl_reg32;
    uint32          rf_ana_26M_ctrl;
    uint32          status;
    uint32          start_tick;

#if defined(FPGA_VERIFICATION)
    return 0;
#endif

    //Disable irq
    SCI_DisableIRQ();

    start_tick = SLEEP_GetCur32kNum();

ADC_CHANNEL_RUN:

    // prepare
    CHIP_ADC_Sample_Prepare_Enter();

    //Clear int flag
    ADC_REG_OR (ADC_INT_CLR, BIT_0);

    //Set ADC Offset Calibration
    ADC_Set_Cal();

    //Set ADC cs
    ADC_SetCs (source);

    //Set ADC scale
    ADC_SetScale (scale);

    //Run ADC soft channel
    ADC_Run();

    //Get current tick
//    cur_tick = next_tick = SCI_GetTickCount();
    cur_tick = next_tick = SLEEP_GetCur32kNum();

    //Wait ADC run over
    while (! (ADC_REG_GET(ADC_INT_RAW) & ADC_IRQ_MASK))
    {
        //Timeout
        //SCI_PASSERT ((next_tick - cur_tick < 6),
        //             ("ADC CHN %d, timeout %dms", source, (next_tick - cur_tick)));

//        if (next_tick - cur_tick > 6)
        if ((next_tick - start_tick) > 64)   // 64/32.768 ~2ms
        {
            PMIC_26M_get_status(&ana_ctrl_reg32, &rf_ana_26M_ctrl, &status);
            SCI_PASSERT (0,("ADC CHN %d, ana_ctrl_reg32=%x, rf_ana_26M_ctrl=%x, status=%d",source, ana_ctrl_reg32, rf_ana_26M_ctrl, status));
        }

//        if ( (next_tick - cur_tick) > 1)
        if ( (next_tick - cur_tick) > 16)
        {
//            PMIC_26M_get_status(&ana_ctrl_reg32, &rf_ana_26M_ctrl, &status);;

            ADC_Init();

            //prepare restore
#ifdef BLUETOOTH_SUPPORT
			//SCI_TraceLow("BT_GetTestMode() = %d", BT_GetTestMode());

			if(BT_GetTestMode()!=1)
			{
				CHIP_ADC_Sample_Prepare_Exit();
			}
#else
			//SCI_TraceLow("BLUETOOTH_SUPPORT is NONE");

            CHIP_ADC_Sample_Prepare_Exit();
#endif
            //ANA_REG_OR (ADC_CTRL, (adc_ctrl&ADC_TPC_CH_ON_BIT));
//            SCI_RestoreIRQ();

            //ADC_LOG_TRACE:"[ADC:ADC_GetResultDirectly timeout...."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_262_112_2_17_23_2_29_15,(uint8*)"");
            goto ADC_CHANNEL_RUN;
            //return ADC_GetResultDirectly (source, scale);
        }

//        next_tick = SCI_GetTickCount();
        next_tick = SLEEP_GetCur32kNum();
    }

    //Get ADC result
    result = ADC_REG_GET(ADC_DATA) & ADC_DATA_MSK;


    //ADC_LOG_TRACE:"[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ADC_PHY_V5_273_112_2_17_23_2_29_16,(uint8*)"dd",source, result);

    // clear the intsts
    ADC_REG_OR (ADC_INT_CLR, ADC_DATA_MSK);

    //prepare restore
#ifdef BLUETOOTH_SUPPORT
	//SCI_TraceLow("BT_GetTestMode() = %d", BT_GetTestMode());
	if(BT_GetTestMode()!=1)
	{
		CHIP_ADC_Sample_Prepare_Exit();
	}

#else
	//SCI_TraceLow("BLUETOOTH_SUPPORT is NONE");
    CHIP_ADC_Sample_Prepare_Exit();
#endif

    //Enable irq
    SCI_RestoreIRQ();

    return result;
}

/*******************************************************************************
 * new func for UMS9118
 * use all 4 kinds of scale: 1.25/2.4/3.233/5
 * GetVol base on efuse calibration info
 ******************************************************************************/
/*
 * ADC_SetScale_Extend - set adc scale
 *
 * <Description of return value>
 * @scale: scale select
 *
 * <Detailed description if necessary>
 * set [10:09] bits of ADC_SW_CH_CFG
 */
PUBLIC void ADC_SetScale_Extend(
	ADC_SCALE_TYPE_E scale)
{
	/* Set ADC scalel */
	ADC_REG_AND(ADC_SW_CH_CFG, ~ADC_CH_SCALE_MASK);
	ADC_REG_OR(ADC_SW_CH_CFG, scale << ADC_CH_SCALE_SHIFT);

	/* Slow mode */
	//ADC_REG_OR(ADC_SW_CH_CFG, ADC_SW_CH_SLOW_MODE_EN);
}

/*
 * ADC_GetResultDirectly_Extend - get value from one adc channel directly
 * @channel_id: channel select
 * @scale: scale select
 *
 * <Description of return value>
 * return: 12-bit ADC value
 *
 * <Detailed description if necessary>
 */
PUBLIC uint32 ADC_GetResultDirectly_Extend(
	ADC_E channel_id,
	ADC_SCALE_TYPE_E scale)
{
	uint32 cur_tick, str_tick, result;

#ifdef FPGA_VERIFICATION
	return 0;
#endif

	/* Disable irq */
	SCI_DisableIRQ();
	/* Clear flag */
	ADC_REG_OR(ADC_INT_CLR, ADC_IRQ_MASK);
	ADC_Set_Cal();
	/* Set ADC cs */
	ADC_SetCs(channel_id);
	/* Set ADC scale */
	ADC_SetScale(scale);
	/* Run ADC soft channel */
	ADC_Run();

	/* Get start time (str_tick) */
	str_tick = SCI_GetTickCount();
	/* Wait ADC run over: 1.irq occured; 2.ADC_CTRL_SWCH_RUN clr */
	while(!(ADC_REG_GET(ADC_INT_RAW)&ADC_IRQ_MASK) &&
		(ADC_REG_GET(ADC_CTRL)&ADC_CTRL_SWCH_RUN)) {
		/* Get current time (cur_tick) */
		cur_tick = SCI_GetTickCount();
		/* Timeout verified*/
		if (cur_tick > str_tick + 6) {
			/* Stop Running */
			ADC_REG_AND(ADC_CTRL, ~ADC_CTRL_SWCH_RUN);
			/* Enable irq */
			SCI_RestoreIRQ();
			ADC_ERR_TRACE(("[ADC][ADC_GetResultDirectly_Extend] channle_%d, timeout",
				channel_id));
			SCI_PASSERT (0,("ADC CHN %d,timeout! ",channel_id));

			return 0;
		}
	}

	/* Get ADC result */
	result = ADC_REG_GET(ADC_DATA)&ADC_DATA_MSK;
	/* Clear the flag */
	ADC_REG_OR(ADC_INT_CLR, ADC_DATA_MSK);
	/* Enable irq */
	SCI_RestoreIRQ();

	ADC_LOG_TRACE(("[ADC][ADC_GetResultDirectly_Extend] chn=%d scale=%d val=%d",
		channel_id, scale, result));
	return result;
}

/*
 * ADC_GetVolRatio_Extend - get adc ratio based on adc scale
 * @channel_id: channel select
 * @scale: scale select
 * @Channel_DIV_P1: ratio 1
 * @Channel_DIV_P2: ratio 2
 *
 * <Detailed description if necessary>
 *
 */
void ADC_GetVolRatio_Extend(
	ADC_E channel_id,
	ADC_SCALE_TYPE_E scale,
	uint32 *Channel_DIV_P1,
	uint32 *Channel_DIV_P2)
{
	switch (channel_id) {
	case ADIN_14:
		switch (scale) {
		case ADC_SCALE_1V250:
			*Channel_DIV_P1 = 68;
			*Channel_DIV_P2 = 900;
			break;
		case ADC_SCALE_2V444:
			*Channel_DIV_P1 = 68;
			*Channel_DIV_P2 = 1760;
			break;
		case ADC_SCALE_3V233:
			*Channel_DIV_P1 = 68;
			*Channel_DIV_P2 = 2327;
			break;
		case ADC_SCALE_5V000:
			*Channel_DIV_P1 = 68;
			*Channel_DIV_P2 = 3654;
			break;
		default:
			*Channel_DIV_P1 = 1;
			*Channel_DIV_P2 = 1;
			break;
		}
		break;

	case ADIN_16:
		switch (scale) {
		case ADC_SCALE_1V250:
			*Channel_DIV_P1 = 48;
			*Channel_DIV_P2 = 100;
			break;
		case ADC_SCALE_2V444:
			*Channel_DIV_P1 = 480;
			*Channel_DIV_P2 = 1955;
			break;
		case ADC_SCALE_3V233:
			*Channel_DIV_P1 = 480;
			*Channel_DIV_P2 = 2586;
			break;
		case ADC_SCALE_5V000:
			*Channel_DIV_P1 = 48;
			*Channel_DIV_P2 = 406;
			break;
		default:
			*Channel_DIV_P1 = 1;
			*Channel_DIV_P2 = 1;
			break;
		}
		break;

	case ADIN_21:
	case ADIN_22:
	case ADIN_23:
		switch (scale) {
		case ADC_SCALE_1V250:
			*Channel_DIV_P1 = 3;
			*Channel_DIV_P2 = 8;
			break;
		case ADC_SCALE_2V444:
			*Channel_DIV_P1 = 375;
			*Channel_DIV_P2 = 1955;
			break;
		case ADC_SCALE_3V233:
			*Channel_DIV_P1 = 375;
			*Channel_DIV_P2 = 2586;
			break;
		case ADC_SCALE_5V000:
			*Channel_DIV_P1 = 300;
			*Channel_DIV_P2 = 3248;
			break;
		default:
			*Channel_DIV_P1 = 1;
			*Channel_DIV_P2 = 1;
			break;
		}
		break;

	default:
		switch (scale) {
		case ADC_SCALE_1V250:
			*Channel_DIV_P1 = 1;
			*Channel_DIV_P2 = 1;
			break;
		case ADC_SCALE_2V444:
			*Channel_DIV_P1 = 1000;
			*Channel_DIV_P2 = 1955;
			break;
		case ADC_SCALE_3V233:
			*Channel_DIV_P1 = 1000;
			*Channel_DIV_P2 = 2586;
			break;
		case ADC_SCALE_5V000:
			*Channel_DIV_P1 = 100;
			*Channel_DIV_P2 = 406;
			break;
		default:
			*Channel_DIV_P1 = 1;
			*Channel_DIV_P2 = 1;
			break;
		}
		break;
	}
}

LOCAL void _adc_get_big_scale_efuse(
	uint32 bit_index)
{
	uint32 deta, cal_data,res;

	/* get efuse */
	res = EFUSE_PHY_Read_Ex(bit_index,&deta);

	/* adc 3.6V */
	cal_data = ((deta >> 8) & 0x00FF) +
		s_adc_cal_para.big_adc_p1 -
		s_adc_cal_para.adc_data_off;
	s_adc_efuse_info.big_scale_cal.p1_vol =
		s_adc_cal_para.big_vol_p1;
	s_adc_efuse_info.big_scale_cal.p1_adc =
		(cal_data << 2) & 0xFFFF;

	/* adc 4.2V */
	cal_data = (deta & 0x00FF) +
		s_adc_cal_para.big_adc_p0 -
		s_adc_cal_para.adc_data_off;
	s_adc_efuse_info.big_scale_cal.p0_vol =
		s_adc_cal_para.big_vol_p0;
	s_adc_efuse_info.big_scale_cal.p0_adc =
		(cal_data << 2) & 0xFFFF;
}

LOCAL void _adc_get_small_scale_efuse(
	uint32 bit_index)
{
	uint32 deta, cal_data,res;

	/* get efuse */
	res = EFUSE_PHY_Read_Ex(bit_index,&deta);

	/* adc 0.1V */
	cal_data = ((deta >> 8) & 0x00FF) +
		s_adc_cal_para.small_adc_p1 -
		s_adc_cal_para.adc_data_off;
	s_adc_efuse_info.small_scale_cal.p1_vol =
		s_adc_cal_para.small_vol_p1;
	s_adc_efuse_info.small_scale_cal.p1_adc =
		(cal_data << 2) & 0xFFFF;

	/* adc 1.0V */
	cal_data = (deta & 0x00FF) +
		s_adc_cal_para.small_adc_p0 -
		s_adc_cal_para.adc_data_off;
	s_adc_efuse_info.small_scale_cal.p0_vol =
		s_adc_cal_para.small_vol_p0;
	s_adc_efuse_info.small_scale_cal.p0_adc =
		(cal_data << 2) & 0xFFFF;
}

LOCAL uint32 _adc_cal_vol(
	uint32 cal_denominators,
	uint32 cal_numerators,
	uint32 denominators,
	uint32 numerators,
	uint32 vol)
{
	uint32 result, m, n;

	n = cal_denominators * numerators;
	m = vol * cal_numerators * denominators;
	result = (m + n / 2) / n;
	return result;
}

LOCAL uint16 AdcvalueToVoltage (uint32 adc)
{
    int32 result;
    int32 adcvalue = (int32)adc;

    result = ((adcvalue - adc_cal.x0_adc)*adc_cal.slope)/adc_cal.zoom_in + adc_cal.y0_vol;

    if(result < 0)
    {
        result = 0;
    }

    return result;
}

LOCAL uint32 _adc_to_vol(
	uint16 p0_vol,
	uint16 p1_vol,
	uint16 p0_adc,
	uint16 p1_adc,
	uint16 adcval)
{
	int32 vol;

	vol = p0_vol - p1_vol;
	vol = vol * (adcval - p1_adc);
	vol = vol / (p0_adc - p1_adc);
	vol = vol + p1_vol;
	if (vol < 0)
		vol = 0;
	vol = (uint32)vol;

	return vol;
}

LOCAL uint32 _adc_big_scale_vol(
	ADC_E channel_id,
	ADC_SCALE_TYPE_E scale,
	uint32 adc_value)
{
	uint32 vol, cal_vol;
	uint32 cal_numerators, cal_denominators;
	uint32 numerators, denominators;

	vol = _adc_to_vol(
		s_adc_efuse_info.big_scale_cal.p0_vol,
		s_adc_efuse_info.big_scale_cal.p1_vol,
		s_adc_efuse_info.big_scale_cal.p0_adc,
		s_adc_efuse_info.big_scale_cal.p1_adc,
		adc_value);

	ADC_GetVolRatio_Extend(ADIN_5, ADC_SCALE_5V000, &cal_numerators, &cal_denominators);
	ADC_GetVolRatio_Extend(channel_id, scale, &numerators, &denominators);

	cal_vol = _adc_cal_vol(cal_denominators, cal_numerators,
		denominators, numerators, vol);

	return cal_vol;
}

LOCAL uint32 _adc_small_scale_vol(
	ADC_E channel_id,
	ADC_SCALE_TYPE_E scale,
	uint32 adc_value)
{
	uint32 vol, cal_vol;
	uint32 cal_numerators, cal_denominators;
	uint32 numerators, denominators;

	vol = _adc_to_vol(
		s_adc_efuse_info.small_scale_cal.p0_vol,
		s_adc_efuse_info.small_scale_cal.p1_vol,
		s_adc_efuse_info.small_scale_cal.p0_adc,
		s_adc_efuse_info.small_scale_cal.p1_adc,
		adc_value);

	ADC_GetVolRatio_Extend(ADIN_1, ADC_SCALE_1V250, &cal_numerators, &cal_denominators);
	ADC_GetVolRatio_Extend(channel_id, scale, &numerators, &denominators);

	cal_vol = _adc_cal_vol(cal_denominators, cal_numerators,
		denominators, numerators, vol);

	return cal_vol;
}

/*
 * ADC_GetCalVol_Extend - get adc voltage (calibration)
 * @channel_id: channel select
 * @scale: scale select
 *
 * return: voltage(mV)
 * <Detailed description if necessary>
 *
 */
PUBLIC uint32 ADC_GetCalVol_Extend(
	ADC_E channel_id,
	ADC_SCALE_TYPE_E scale)
{
	uint32 adc_value, vol;

	/* get adc original data */
	adc_value = ADC_GetResultDirectly(channel_id, scale);
	/* VBAT use big scale efuse calibration
	if (ADIN_5 == channel_id)
		vol = _adc_big_scale_vol(channel_id, scale, adc_value);
	else
		vol = _adc_small_scale_vol(channel_id, scale, adc_value);

	ADC_LOG_TRACE(("[ADC][ADC_GetCalVol_Extend] chn=%d scale=%d val=%d cal=%dmV",
		channel_id, scale, adc_value, vol));
	*/
	//vol = AdcvalueToVoltage(adc_value);
	vol =(adc_value * 1250/*1000 * 1.25*/) / 4096;
	return vol;
}
#if 1
/*for sc2731/sc2721/2720 if set 1.25uA, parameter isen is 125uA */
LOCAL uint32 _adc_isen_set(uint32 isen)
{
	uint32 isen_set = 0;
	switch (isen) {
	case 125:
		isen_set = 0x0;
		break;
	case 250:
		isen_set = 0x1;
		break;
	case 500:
		isen_set = 0x2;
		break;
	case 1000:
		isen_set = 0x3;
		break;
	case 2000:
		isen_set = 0x4;
		break;
	case 4000:
		isen_set = 0x5;
		break;
	case 8000:
		isen_set = 0x6;
		break;
	case 16000:
		isen_set = 0x7;
		break;
	default:
		isen_set = 0x0;
		break;
	}
	return isen_set;
}

LOCAL void _adc_set_isen(uint8 enable, uint32 isen)//xun zhao ti dai
{
	uint32 isen_bit;
	if (enable) {
		isen_bit = _adc_isen_set(isen);
		ADC_REG_AND(ANA_REG_GLB_IB_CTRL, ~BIT_IB_REX_EN);
		ADC_REG_OR(ANA_REG_GLB_IB_CTRL,BIT_BATDET_CUR_EN);
		ADC_REG_AND(ANA_REG_GLB_IB_CTRL,~(BITS_BATDET_CUR_I(0xF)));
		ADC_REG_OR(ANA_REG_GLB_IB_CTRL,BITS_BATDET_CUR_I(isen_bit));
		udelay(150);
	} else {
		ADC_REG_AND(ANA_REG_GLB_IB_CTRL,~(BITS_BATDET_CUR_I(0xF)));
		ADC_REG_AND(ANA_REG_GLB_IB_CTRL,~BIT_BATDET_CUR_EN);
	}
}

LOCAL void _adc_set_current(uint8 enable, uint32 isen)
{
	if (enable) {
		_adc_set_isen(enable, isen);
		ADC_REG_OR(ADC_CTL0, BIT_ADC_CUR_EN);

	} else {
		ADC_REG_AND(ADC_CTL0, ~BIT_ADC_CUR_EN);
		_adc_set_isen(enable, isen);
	}
}

PUBLIC uint32 ADC_GetVoltageByCurrent_Extend(
	ADC_E channel_id,
	ADC_SCALE_TYPE_E scale,
	uint32 isen,
	uint32 cnt)
{
	uint32 i = 0, value= 0;
	cnt = (cnt<50)?cnt:50;
	for (i=0; i<cnt; i++) {
		_adc_set_current(1, isen);
		value += ADC_GetCalVol_Extend(channel_id, scale);
	}
	value = (uint32)(value/cnt);

	_adc_set_current(0, 0);
	return value;
}
#endif
