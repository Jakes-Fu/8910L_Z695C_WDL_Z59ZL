/******************************************************************************
 ** File Name:      ldo_cfg.c                                                 *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           07/10/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic configuration for ldo management.*
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/10/2007     jiexia.yu        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "adc_drvapi.h"
#include "ldo_drvapi.h"
#include "ldo_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
#if defined(PLATFORM_SC6800H)

LDO_CTL_T ldo_ctl_data[] =
{
    {
        LDO_LDO_CORE,      NULL,          NULL,    NULL,   GR_LDO_VCFG0,   BIT_0,  BIT_1,
        GR_LDO_VCFG0,   BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL0,     NULL
    },
    {
        LDO_LDO_CORE_SLP,  NULL,          NULL,    NULL,   GR_SLP_CTL,        BIT_0,   BIT_1,
        GR_SLP_CTL,     BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL0,        NULL
    },
    {
        LDO_LDO_ABB,   GR_LDO_PD,         BIT_18,  BIT_19,     GR_LDO_VCFG1,   BIT_4,  BIT_5,
        GR_LDO_VCFG1,   BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_IO18,   GR_LDO_PD,         BIT_4,  BIT_5,  GR_LDO_VCFG0,   BIT_8,  BIT_9,
        GR_LDO_VCFG0,   BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_IO28,   GR_LDO_PD,         BIT_2,  BIT_3,  GR_LDO_VCFG0,   BIT_4,  BIT_5,
        GR_LDO_VCFG0,   BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_MEM,   NULL,                    NULL,  NULL,   NULL,               NULL,   NULL,
        NULL,               NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_AUX1,  GR_LDO_PD,         BIT_14,  BIT_15,     GR_LDO_VCFG0,   BIT_20,     BIT_21,
        GR_LDO_VCFG0,   BIT_22,     BIT_23, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_AUX2,  GR_LDO_PD,         BIT_12,  BIT_13,     GR_LDO_VCFG0,   BIT_16,     BIT_17,
        GR_LDO_VCFG0,   BIT_18,     BIT_19, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RF,    GR_LDO_PD,         BIT_16,  BIT_17,     GR_LDO_VCFG0,   BIT_24,     BIT_25,
        GR_LDO_VCFG0,   BIT_26, BIT_27, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_USB,   GR_LDO_PD,         BIT_10,  BIT_11,     NULL,   NULL,   NULL,
        NULL,                 NULL,     NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_AVBO,  GR_LDO_PD,         BIT_20,  BIT_21,     GR_LDO_VCFG1,   BIT_8,  BIT_9,
        GR_LDO_VCFG1,   BIT_10,     BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RTC,   NULL,                   NULL,   NULL,   GR_LDO_VCFG1,   BIT_0,  BIT_1,
        GR_LDO_VCFG1,   BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SIM0,  NULL,                   NULL,   NULL,   GR_LDO_VCFG0,   BIT_28,     BIT_29,
        GR_LDO_VCFG0,   BIT_28, BIT_29, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SIM1,  NULL,                   NULL,   NULL,   GR_LDO_VCFG0,   BIT_30,     BIT_31,
        GR_LDO_VCFG0,   BIT_30,     BIT_31, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SIM2,  NULL,                   NULL,   NULL,   GR_LDO_VCFG1,   BIT_16,     BIT_17,
        GR_LDO_VCFG1,   BIT_16,     BIT_17, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SIM3,  NULL,                   NULL,   NULL,   GR_LDO_VCFG1,   BIT_18,     BIT_19,
        GR_LDO_VCFG1,   BIT_18,     BIT_19, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SDIO,  GR_LDO_PD,      BIT_8,  BIT_9,  GR_LDO_VCFG0,   BIT_12,     BIT_13,
        GR_LDO_VCFG0,   BIT_14,     BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_MAX,   NULL,           NULL,   NULL,   NULL,           NULL,   NULL,
        NULL,           NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    }
};

LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
    {SLP_LDO_COREV, GR_SLP_CTL, BIT_4,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_ABB,   GR_SLP_CTL, BIT_8,  SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_VBO,   GR_SLP_CTL, BIT_9,  SLP_BIT_SET,    TRUE,   NULL},
    //{SLP_LDO_AUX1,  GR_SLP_CTL, BIT_11, SLP_BIT_SET,    TRUE,   NULL},
    //{SLP_LDO_AUX2,  GR_SLP_CTL, BIT_10, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_RF,    GR_SLP_CTL, BIT_12, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_USB,   GR_SLP_CTL, BIT_13, SLP_BIT_SET,    TRUE,   NULL},
    //{SLP_LDO_SDIO,    GR_SLP_CTL, BIT_14,     SLP_BIT_SET,    TRUE,   NULL},
    //{SLP_LDO_IO18,    GR_SLP_CTL, BIT_15,     SLP_BIT_SET,    TRUE,   NULL},
    //{SLP_LDO_IO28,    GR_SLP_CTL, BIT_16,     SLP_BIT_SET,    TRUE,   NULL},
    //{SLP_LDO_BG,      GR_SLP_CTL, BIT_17,     SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_MAX,   NULL,       NULL,   SLP_BIT_SET,    TRUE,   NULL}
};
#endif

#ifdef PLATFORM_SC6600L
LDO_CTL_T ldo_ctl_data[] =
{
    {
        LDO_LDO_CORE,  GR_LDO_CTL0,    NULL,   NULL,   GR_LDO_CTL1,    BIT_0,  BIT_1,
        GR_LDO_CTL1,    BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_ABB,   GR_LDO_CTL0,      BIT_2,    BIT_3,  GR_LDO_CTL1,    BIT_16,     BIT_17,
        GR_LDO_CTL1,    BIT_18,     BIT_19, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_IO,      GR_LDO_CTL0,      NULL,   NULL,   NULL,               NULL,   NULL,
        NULL,           NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_MEM,   GR_LDO_CTL0,      NULL, NULL,   NULL,               NULL,   NULL,
        NULL,               NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_AUX1,  GR_LDO_CTL0,      BIT_8,    BIT_9,  GR_LDO_CTL1,    BIT_8,  BIT_9,
        GR_LDO_CTL1,    BIT_10,     BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_AUX2,  GR_LDO_CTL0,      BIT_10,   BIT_11,     GR_LDO_CTL1,    BIT_12,     BIT_13,
        GR_LDO_CTL1,    BIT_14,     BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RF,    GR_LDO_CTL0,      BIT_12,   BIT_13,     GR_LDO_CTL3,    BIT_0,  BIT_1,
        GR_LDO_CTL3,    BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_USB,   GR_LDO_CTL0,      BIT_14,   BIT_15,     GR_LDO_CTL3,    BIT_27,     BIT_28,
        GR_LDO_CTL3,    BIT_29,     BIT_30, NULL,   LDO_VOLT_LEVEL2,        NULL
    },
    {
        LDO_LDO_AVBO,  GR_LDO_CTL0,      BIT_16,   BIT_17,     GR_LDO_CTL3,    BIT_12,     BIT_13,
        GR_LDO_CTL3,    BIT_14,     BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RTC,   NULL,                   NULL,   NULL,   GR_LDO_CTL1,    BIT_4,  BIT_5,
        GR_LDO_CTL1,    BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL0,        NULL
    },
    {
        LDO_LDO_SIM0,  NULL,                   NULL,   NULL,   GR_LDO_CTL3,    BIT_4,  BIT_5,
        GR_LDO_CTL3,    BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL0,        NULL
    },
    {
        LDO_LDO_SIM1,  NULL,                   NULL,   NULL,   GR_LDO_CTL3,    BIT_8,  BIT_9,
        GR_LDO_CTL3,    BIT_10,     BIT_11, NULL,   LDO_VOLT_LEVEL0,        NULL
    },
    {
        LDO_LDO_MAX,   NULL,           NULL,   NULL,   NULL,           NULL,   NULL,
        NULL,           NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    }
};

LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
    {SLP_LDO_ABB,   GR_LDO_CTL0,    BIT_22, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_VBO,   GR_LDO_CTL0,    BIT_23, SLP_BIT_SET,    TRUE,   NULL},
    //{SLP_LDO_AUX1,    GR_LDO_CTL0,    BIT_24,     SLP_BIT_SET,    TRUE,   NULL},
    //{SLP_LDO_AUX2,    GR_LDO_CTL0,    BIT_25,     SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_RF,    GR_LDO_CTL0,    BIT_26,     SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_USB,   GR_LDO_CTL0,    BIT_27,     SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_COREV, GR_LDO_CTL1,    BIT_25,     SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_MAX,   NULL,           NULL,   SLP_BIT_SET,    TRUE,   NULL}
};
#endif

#ifdef PLATFORM_SC8800G
LDO_CTL_T ldo_ctl_data[] =
{
    {
        LDO_LDO_ABB,   NULL,           NULL,   NULL,   ANA_LDO_VCTL0,  BIT_12, BIT_13,
        ANA_LDO_VCTL0,  BIT_14, BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RF1,   NULL,           NULL,   NULL,   ANA_LDO_VCTL0,  BIT_8,  BIT_9,
        ANA_LDO_VCTL0,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RF0,   NULL,           NULL,   NULL,   ANA_LDO_VCTL0,  BIT_4,  BIT_5,
        ANA_LDO_VCTL0,  BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_RTC,   NULL,           NULL,   NULL,   ANA_LDO_VCTL0,  BIT_0,  BIT_1,
        ANA_LDO_VCTL0,  BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SDIO,  ANA_LDO_PD_CTL, BIT_2,  BIT_3,  ANA_LDO_VCTL1,  BIT_12,BIT_13,
        ANA_LDO_VCTL1,  BIT_14, BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_AVBO,  ANA_LDO_PD_CTL, BIT_14, BIT_15, ANA_LDO_VCTL1,  BIT_8,  BIT_9,
        ANA_LDO_VCTL1,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SIM1,  ANA_LDO_PD_CTL, BIT_6,  BIT_7,  ANA_LDO_VCTL1,  BIT_4,  BIT_5,
        ANA_LDO_VCTL1,  BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_SIM0,  ANA_LDO_PD_CTL, BIT_4,  BIT_5,  ANA_LDO_VCTL1,  BIT_0,  BIT_1,
        ANA_LDO_VCTL1,  BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_USB,   ANA_LDO_PD_CTL, BIT_0,  BIT_1,  ANA_LDO_VCTL2,  BIT_12, BIT_13,
        ANA_LDO_VCTL2,  BIT_14, BIT_15, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_USBD,  NULL,           NULL,   NULL,   NULL,           NULL,   NULL,
        NULL,           NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_CAMA,  ANA_LDO_PD_CTL, BIT_12, BIT_13, ANA_LDO_VCTL2,  BIT_8,  BIT_9,
        ANA_LDO_VCTL2,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_CAMD1, ANA_LDO_PD_CTL, BIT_10, BIT_11, ANA_LDO_VCTL2,  BIT_4,  BIT_5,
        ANA_LDO_VCTL2,  BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_CAMD0, ANA_LDO_PD_CTL, BIT_8,  BIT_9,  ANA_LDO_VCTL2,  BIT_0,  BIT_1,
        ANA_LDO_VCTL2,  BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_VDD25, NULL,           NULL,   NULL,   ANA_LDO_VCTL3,  BIT_8,  BIT_9,
        ANA_LDO_VCTL3,  BIT_10, BIT_11, NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_VDD18, NULL,           NULL,   NULL,   ANA_LDO_VCTL3,  BIT_4,  BIT_5,
        ANA_LDO_VCTL3,  BIT_6,  BIT_7,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_VDD28, NULL,           NULL,   NULL,   ANA_LDO_VCTL3,  BIT_0,  BIT_1,
        ANA_LDO_VCTL3,  BIT_2,  BIT_3,  NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    },
    {
        LDO_LDO_MAX,   NULL,           NULL,   NULL,   NULL,           NULL,   NULL,
        NULL,           NULL,   NULL,   NULL,   LDO_VOLT_LEVEL_MAX,     NULL
    }
};

LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
    {SLP_LDO_PA,        ANA_LDO_SLP,    BIT_15, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_DVDD18,    ANA_LDO_SLP,    BIT_14, SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_VDD25,     ANA_LDO_SLP,    BIT_13, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_VDD18,     ANA_LDO_SLP,    BIT_12, SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_VDD28,     ANA_LDO_SLP,    BIT_11, SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_ABB,       ANA_LDO_SLP,    BIT_10, SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_SDIO,      ANA_LDO_SLP,    BIT_9,  SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_VBO,       ANA_LDO_SLP,    BIT_8,  SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_CAMA,      ANA_LDO_SLP,    BIT_7,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_CAMD1,     ANA_LDO_SLP,    BIT_6,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_CAMD0,     ANA_LDO_SLP,    BIT_5,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_USB,       ANA_LDO_SLP,    BIT_4,  SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_SIM1,      ANA_LDO_SLP,    BIT_3,  SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_SIM0,      ANA_LDO_SLP,    BIT_2,  SLP_BIT_CLR,    TRUE,   NULL},
    {SLP_LDO_RF1,       ANA_LDO_SLP,    BIT_1,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_RF0,       ANA_LDO_SLP,    BIT_0,  SLP_BIT_SET,    TRUE,   NULL},
    {SLP_LDO_MAX,       NULL,           NULL,   SLP_BIT_SET,    TRUE,   NULL}
};

#endif

#if defined(PLATFORM_SC6530)

#ifdef CHIP_VER_6531

LDO_CTL_T ldo_ctl_data[] =
{
    {LDO_LDO_CORE,      ANA_LDO_PD_SET0,    BIT_1,  ANA_LDO_PD_RST0,    BIT_1,  ANA_LDO_VCTL0,  BIT_0,  BIT_1,  ANA_LDO_VCTL0,  BIT_2,  BIT_3,  ANA_LDO_VCTL0,           BIT_4,   BIT_5,   NULL,        LDO_VOLT_LEVEL_MAX, NULL},                                  
    {LDO_LDO_VDD28,     ANA_LDO_PD_SET0,    BIT_3,  ANA_LDO_PD_RST0,    BIT_3,  ANA_LDO_VCTL0,  BIT_8,  BIT_9,  ANA_LDO_VCTL0,  BIT_10, BIT_11, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_ABB,       ANA_LDO_PD_SET0,    BIT_4,  ANA_LDO_PD_RST0,    BIT_4,  ANA_LDO_VCTL0,  BIT_12, BIT_13, ANA_LDO_VCTL0,  BIT_14, BIT_15, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_CAMD0,     ANA_LDO_PD_SET0,    BIT_5,  ANA_LDO_PD_RST0,    BIT_5,  ANA_LDO_VCTL1,  BIT_4,  BIT_5,  ANA_LDO_VCTL1,  BIT_6,  BIT_7,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_CAMD1,     ANA_LDO_PD_SET0,    BIT_6,  ANA_LDO_PD_RST0,    BIT_6,  ANA_LDO_VCTL1,  BIT_8,  BIT_9,  ANA_LDO_VCTL1,  BIT_10, BIT_11, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_CAMA,      ANA_LDO_PD_SET0,    BIT_7,  ANA_LDO_PD_RST0,    BIT_7,  ANA_LDO_VCTL1,  BIT_12, BIT_13, ANA_LDO_VCTL1,  BIT_14, BIT_15, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_RF,        ANA_LDO_PD_SET0,    BIT_8,  ANA_LDO_PD_RST0,    BIT_8,  ANA_LDO_VCTL2,  BIT_0,  BIT_1,  ANA_LDO_VCTL2,  BIT_2,  BIT_3,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_VDD18,     ANA_LDO_PD_SET0,    BIT_9,  ANA_LDO_PD_RST0,    BIT_9,  ANA_LDO_VCTL3,  BIT_0,  BIT_1,  ANA_LDO_VCTL3,  BIT_2,  BIT_3,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_VDD33,     ANA_LDO_PD_SET0,    BIT_10, ANA_LDO_PD_RST0,    BIT_10, ANA_LDO_VCTL3,  BIT_8,  BIT_9,  ANA_LDO_VCTL3,  BIT_10, BIT_11, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_USB,       ANA_LDO_PD_SET1,    BIT_4,  ANA_LDO_PD_RST1,    BIT_4,  ANA_LDO_VCTL3,  BIT_4,  BIT_5,  ANA_LDO_VCTL3,  BIT_6,  BIT_7,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_SIM2,      ANA_LDO_PD_SET1,    BIT_3,  ANA_LDO_PD_RST1,    BIT_3,  ANA_LDO_VCTL2,  BIT_12, BIT_13, ANA_LDO_VCTL2,  BIT_14, BIT_15, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_SIM1,      ANA_LDO_PD_SET1,    BIT_2,  ANA_LDO_PD_RST1,    BIT_2,  ANA_LDO_VCTL2,  BIT_8,  BIT_9,  ANA_LDO_VCTL2,  BIT_10, BIT_11, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_SIM0,      ANA_LDO_PD_SET1,    BIT_1,  ANA_LDO_PD_RST1,    BIT_1,  ANA_LDO_VCTL2,  BIT_4,  BIT_5,  ANA_LDO_VCTL2,  BIT_6,  BIT_7,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_SDIO,      ANA_LDO_PD_SET1,    BIT_0,  ANA_LDO_PD_RST1,    BIT_0,  ANA_LDO_VCTL1,  BIT_0,  BIT_1,  ANA_LDO_VCTL1,  BIT_2,  BIT_3,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_MAX,       NULL,               NULL,   NULL,               NULL,   NULL,           NULL,   NULL,   NULL,           NULL,   NULL,   NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL}
};

LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
#ifndef VDDCORE_SUPPLY_LDO
    {SLP_LDO_COREV,         ANA_SLP_LDO_CTL2,   SLP_LDOCORE_V_EN,       SLP_BIT_CLR,        TRUE,   NULL},
#endif
    {SLP_LDO_MEMS,          ANA_SLP_LDO_CTL0,   SLP_LDOMEMS_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDD28,         ANA_SLP_LDO_CTL0,   SLP_LDOVDD28_PD_EN,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDD33,         ANA_SLP_LDO_CTL0,   SLP_LDOVDD33_PD_EN,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_CAMD0,         ANA_SLP_LDO_CTL0,   SLP_LDOCAMD0_PD_EN,     SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMD1,         ANA_SLP_LDO_CTL0,   SLP_LDOCAMD1_PD_EN,     SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMA,          ANA_SLP_LDO_CTL0,   SLP_LDOCAMA_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_RF,            ANA_SLP_LDO_CTL0,   SLP_LDORF_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_VDD18,         ANA_SLP_LDO_CTL0,   SLP_LDOVDD18_PD_EN,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_RF_AUTO_ISO,   ANA_SLP_LDO_CTL0,   SLP_RF_AUTO_ISO_EN,     SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_DCXO,          ANA_SLP_LDO_CTL0,   SLP_LDODCXO_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SDIO,          ANA_SLP_LDO_CTL0,   SLP_LDOSDIO_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_USB,           ANA_SLP_LDO_CTL0,   SLP_LDOUSB_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SIM0,          ANA_SLP_LDO_CTL0,   SLP_LDOSIM0_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SIM1,          ANA_SLP_LDO_CTL0,   SLP_LDOSIM1_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SIM2,          ANA_SLP_LDO_CTL0,   SLP_LDOSIM2_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_AUDIO_MICBIAS, ANA_SLP_LDO_CTL1,   SLP_AUDIO_MICBIAS_PD_EN,SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_AUDIO_BG,      ANA_SLP_LDO_CTL1,   SLP_AUDIO_BG_PD_EN,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VBO,           ANA_SLP_LDO_CTL1,   SLP_AUDIO_VBO_PD_EN,    SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VB,            ANA_SLP_LDO_CTL1,   SLP_AUDIO_VB_PD_EN,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_AUDIO_VCMBUF,  ANA_SLP_LDO_CTL1,   SLP_AUDIO_VCMBUF_PD_EN, SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_AUDIO_VCM,     ANA_SLP_LDO_CTL1,   SLP_AUDIO_VCM_PD_EN,    SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_XTL_AUDIO,     ANA_SLP_LDO_CTL1,   XTL_AUDIO_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},  
    {SLP_LDO_VDD18,         ANA_SLP_LDO_CTL2,   SLP_LDOVDD18_V_EN,      SLP_BIT_CLR,        TRUE,   NULL},  
    {SLP_LDO_PWR_SET,       ANA_SLP_LDO_CTL2,   SLP_PWR_SET_EN,         SLP_BIT_SET,        TRUE,   NULL},  
    {SLP_DCDC_V,            ANA_SLP_LDO_CTL2,   SLP_DCDC_V_EN,         SLP_BIT_CLR,         TRUE,   NULL},  
    {SLP_LDO_LOWPOWER,            ANA_SLP_LDO_CTL2,   SLP_LDO_LP_AUTO_EN,     SLP_BIT_CLR,         TRUE,   NULL},      
    {SLP_LDO_MAX,           NULL,               NULL,                   SLP_BIT_SET,        TRUE,   NULL}
};

#else

LDO_CTL_T ldo_ctl_data[] =
{
    {LDO_LDO_CORE,      ANA_LDO_PD_SET0,    BIT_1,  ANA_LDO_PD_RST0,    BIT_1,  ANA_LDO_VCTL0,  BIT_0,  BIT_1,  ANA_LDO_VCTL0,  BIT_2,  BIT_3,  ANA_LDO_VCTL0,           BIT_4,   BIT_5,   NULL,        LDO_VOLT_LEVEL_MAX, NULL},                                  
    {LDO_LDO_VDD28,     ANA_LDO_PD_SET0,    BIT_3,  ANA_LDO_PD_RST0,    BIT_3,  ANA_LDO_VCTL0,  BIT_8,  BIT_9,  ANA_LDO_VCTL0,  BIT_10, BIT_11, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_ABB,       ANA_LDO_PD_SET0,    BIT_4,  ANA_LDO_PD_RST0,    BIT_4,  ANA_LDO_VCTL0,  BIT_12, BIT_13, ANA_LDO_VCTL0,  BIT_14, BIT_15, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_CAMD0,     ANA_LDO_PD_SET0,    BIT_5,  ANA_LDO_PD_RST0,    BIT_5,  ANA_LDO_VCTL1,  BIT_4,  BIT_5,  ANA_LDO_VCTL1,  BIT_6,  BIT_7,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_CAMD1,     ANA_LDO_PD_SET0,    BIT_6,  ANA_LDO_PD_RST0,    BIT_6,  ANA_LDO_VCTL1,  BIT_8,  BIT_9,  ANA_LDO_VCTL1,  BIT_10, BIT_11, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_CAMA,      ANA_LDO_PD_SET0,    BIT_7,  ANA_LDO_PD_RST0,    BIT_7,  ANA_LDO_VCTL1,  BIT_12, BIT_13, ANA_LDO_VCTL1,  BIT_14, BIT_15, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_RF,        ANA_LDO_PD_SET0,    BIT_8,  ANA_LDO_PD_RST0,    BIT_8,  ANA_LDO_VCTL2,  BIT_0,  BIT_1,  ANA_LDO_VCTL2,  BIT_2,  BIT_3,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_VDD18,     ANA_LDO_PD_SET0,    BIT_9,  ANA_LDO_PD_RST0,    BIT_9,  ANA_LDO_VCTL3,  BIT_0,  BIT_1,  ANA_LDO_VCTL3,  BIT_2,  BIT_3,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_VDD25,     ANA_LDO_PD_SET0,    BIT_10, ANA_LDO_PD_RST0,    BIT_10, ANA_LDO_VCTL3,  BIT_8,  BIT_9,  ANA_LDO_VCTL3,  BIT_10, BIT_11, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_USB,       ANA_LDO_PD_SET1,    BIT_4,  ANA_LDO_PD_RST1,    BIT_4,  ANA_LDO_VCTL3,  BIT_4,  BIT_5,  ANA_LDO_VCTL3,  BIT_6,  BIT_7,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_SIM2,      ANA_LDO_PD_SET1,    BIT_3,  ANA_LDO_PD_RST1,    BIT_3,  ANA_LDO_VCTL2,  BIT_12, BIT_13, ANA_LDO_VCTL2,  BIT_14, BIT_15, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_SIM1,      ANA_LDO_PD_SET1,    BIT_2,  ANA_LDO_PD_RST1,    BIT_2,  ANA_LDO_VCTL2,  BIT_8,  BIT_9,  ANA_LDO_VCTL2,  BIT_10, BIT_11, NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_SIM0,      ANA_LDO_PD_SET1,    BIT_1,  ANA_LDO_PD_RST1,    BIT_1,  ANA_LDO_VCTL2,  BIT_4,  BIT_5,  ANA_LDO_VCTL2,  BIT_6,  BIT_7,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_SDIO,      ANA_LDO_PD_SET1,    BIT_0,  ANA_LDO_PD_RST1,    BIT_0,  ANA_LDO_VCTL1,  BIT_0,  BIT_1,  ANA_LDO_VCTL1,  BIT_2,  BIT_3,  NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL},
    {LDO_LDO_MAX,       NULL,               NULL,   NULL,               NULL,   NULL,           NULL,   NULL,   NULL,           NULL,   NULL,   NULL,                     NULL,    NULL,     NULL,      LDO_VOLT_LEVEL_MAX, NULL}
};

LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
#ifndef VDDCORE_SUPPLY_LDO
    {SLP_LDO_COREV,         ANA_SLP_LDO_CTL2,   BIT_12,     SLP_BIT_CLR,        TRUE,   NULL},
#endif
    {SLP_LDO_MEMS,          ANA_SLP_LDO_CTL0,   BIT_0,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDD28,         ANA_SLP_LDO_CTL0,   BIT_1,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_CAMD0,         ANA_SLP_LDO_CTL0,   BIT_3,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_CAMD1,         ANA_SLP_LDO_CTL0,   BIT_4,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_CAMA,          ANA_SLP_LDO_CTL0,   BIT_5,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_RF,            ANA_SLP_LDO_CTL0,   BIT_6,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_VDD18,         ANA_SLP_LDO_CTL0,   BIT_7,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDD25,         ANA_SLP_LDO_CTL0,   BIT_8,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_ABB,           ANA_SLP_LDO_CTL0,   BIT_9,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SDIO,          ANA_SLP_LDO_CTL0,   BIT_11,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_USB,           ANA_SLP_LDO_CTL0,   BIT_12,     SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SIM0,          ANA_SLP_LDO_CTL0,   BIT_13,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SIM1,          ANA_SLP_LDO_CTL0,   BIT_14,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SIM2,          ANA_SLP_LDO_CTL0,   BIT_15,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_AUDIO_MICBIAS, ANA_SLP_LDO_CTL1,   BIT_9,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_AUDIO_BG,      ANA_SLP_LDO_CTL1,   BIT_8,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_VBO,           ANA_SLP_LDO_CTL1,   BIT_7,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_VB,            ANA_SLP_LDO_CTL1,   BIT_6,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_AUDIO_VCMBUF,  ANA_SLP_LDO_CTL1,   BIT_5,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_AUDIO_VCM,     ANA_SLP_LDO_CTL1,   BIT_4,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_BG,            ANA_SLP_LDO_CTL1,   BIT_3,      SLP_BIT_CLR,        TRUE,   NULL},  
    {SLP_LDO_MAX,           NULL,               NULL,       SLP_BIT_SET,        TRUE,   NULL}
};
#endif

#endif

#if defined(PLATFORM_SC6531EFM)
LDO_CTL_T ldo_ctl_data[] =
{
	{LDO_LDO_CORE,      ANA_POWER_PD_SW,      BIT_4, NULL},
	{LDO_LDO_VDD28,     ANA_POWER_PD_SW,      BIT_1, NULL},
	{LDO_LDO_DCXO,      ANA_POWER_PD_SW,      BIT_5, NULL},
	{LDO_LDO_CAMD0,     ANA_LDO_CAMD_REG0,    BIT_0, NULL},
	{LDO_LDO_RF,        ANA_LDO_RF_REG0,      BIT_0, NULL},
	{LDO_LDO_VDD18,     ANA_POWER_PD_SW,      BIT_2, NULL},
	{LDO_LDO_USB,       ANA_LDO_USB_REG0,     BIT_0, NULL},
	{LDO_LDO_SIM1,      ANA_LDO_SIM1_PD_REG,  BIT_0, NULL},
	{LDO_LDO_SIM0,      ANA_LDO_SIM0_REG2,    BIT_0, NULL},
	{LDO_LDO_SDIO,      ANA_LDO_SD_REG2,      BIT_0, NULL},
	{LDO_LDO_CAMA,      ANA_LDO_CAMA_REG0,    BIT_0, NULL},
	{LDO_LDO_BTPA,      ANA_LDO_BTPA_REG0,    BIT_0, NULL},
	{LDO_LDO_VIBR,      ANA_VIBR_CTRL,        BIT_8, NULL},
	{LDO_LDO_SF,        ANA_POWER_PD_SW,      BIT_3, NULL},
	{LDO_LDO_MAX,       NULL,                 NULL,  NULL}
};

LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
    {SLP_LDO_RF,            ANA_SLP_LDO_PD_CTRL0,   SLP_LDORF_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_DCXO,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDODCXO_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDD28,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOVDD28_PD_EN,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SDIO,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSD_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_USB,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOUSB_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMD0,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMD_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMA,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMA_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SIM1,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSIM1_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SIM0,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSIM0_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDDSF,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSF_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDDBTPA,       ANA_SLP_LDO_PD_CTRL0,   SLP_LDOBTPA_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_VDDVIB,        ANA_VIBR_CTRL,          SLP_LDOVIBR_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_MAX,           NULL,                   NULL,                   SLP_BIT_CLR,        TRUE,   NULL}
};

LOCAL CAL_LDO_T cal_ldo_data[] = {
	{LDO_LDO_CORE,  CAL_LDO_GROUP_B, CAL_LDO_CORE,  8,   20, ANA_LDO_CORE_REG1,  LDO_CORE_V_MSK,  LDO_CORE_V_SHIFT,  VDD1V1_DEFAULT, 0x23, 750,  1350, 0x0,  0x3C, VDD1V1_DEFAULT, FALSE, FALSE, FALSE}, //6  LDO_CORE
	{LDO_LDO_RF,    CAL_LDO_GROUP_B, CAL_LDO_RF,    8,   13, ANA_LDO_RF_REG1,    LDO_RF_V_MSK,    LDO_RF_V_SHIFT,    VDD1V8_DEFAULT, 0x60, 1200, 2000, 0x0,  0x7F, VDD1V8_DEFAULT, FALSE, FALSE,  FALSE}, //15 LDO_RF
	{LDO_LDO_BTPA,  CAL_LDO_GROUP_B, CAL_LDO_BTPA,  8,   20, ANA_LDO_BTPA_REG1,  LDO_BTPA_V_MSK,  LDO_BTPA_V_SHIFT,  VDD3V3_DEFAULT, 0xd2, 1200, 3300, 0x0,  0xd2, VDD3V3_DEFAULT, TRUE,  FALSE,  FALSE}, //42 LDO_BTPA
	{LDO_LDO_VDD18, CAL_LDO_GROUP_B, CAL_LDO_VDD18, 8,   20, ANA_LDO_VDD18_REG1, LDO_VDD18_V_MSK, LDO_VDD18_V_SHIFT, VDD1V8_DEFAULT, 0x3c, 1800, 3300, 0x3c, 0xd2, VDD1V8_DEFAULT, FALSE, FALSE,  FALSE}, //33 LDO_VDD18
	{LDO_LDO_VDD28, CAL_LDO_GROUP_B, CAL_LDO_VDD28, 8,   20, ANA_LDO_VDD28_REG1, LDO_VDD28_V_MSK, LDO_VDD28_V_SHIFT, VDD2V8_DEFAULT, 0xa0, 1800, 3300, 0x3c, 0xd2, VDD2V8_DEFAULT, FALSE, FALSE,  FALSE}, //35 LDO_VDD28
	{LDO_LDO_DCXO,  CAL_LDO_GROUP_B, CAL_LDO_DCXO,  8,   20, ANA_LDO_DCXO_REG1,  LDO_DCXO_V_MSK,  LDO_DCXO_V_SHIFT,  VDD1V8_DEFAULT, 0x3c, 1800, 3300, 0x3c, 0xd2, VDD1V8_DEFAULT, FALSE, FALSE,  FALSE}, //46 LDO_DCXO
	{LDO_LDO_SDIO,  CAL_LDO_GROUP_A, CAL_LDO_SD,    7,   20, ANA_LDO_SD_REG1,    LDO_SD_V_MSK,    LDO_SD_V_SHIFT,    VDD3V0_DEFAULT, 0xb4, 1800, 3300, 0x3c, 0xd2, VDD3V0_DEFAULT, TRUE,  FALSE,  FALSE}, //29 LDO_SDIO
	{LDO_LDO_CAMA,  CAL_LDO_GROUP_A, CAL_LDO_CAMA,  7,   20, ANA_LDO_CAMA_REG1,  LDO_CAMA_V_MSK,  LDO_CAMA_V_SHIFT,  VDD2V8_DEFAULT, 0xa0, 1800, 3300, 0x3c, 0xd2, VDD2V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
	{LDO_LDO_CAMD0, CAL_LDO_GROUP_A, CAL_LDO_CAMD,  7,   20, ANA_LDO_CAMD_REG1,  LDO_CAMD_V_MSK,  LDO_CAMD_V_SHIFT,  VDD1V8_DEFAULT, 0x3c, 1800, 3300, 0x3c, 0xd2, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //31 LDO_CAMD0
	{LDO_LDO_VIBR,  CAL_LDO_GROUP_A, CAL_LDO_VIBR,  7,   20, ANA_VIBR_CTRL,      LDO_VIBR_V_MSK,  LDO_VIBR_V_SHIFT,  VDD3V0_DEFAULT, 0xb4, 1200, 3300, 0x0,  0xd2, VDD3V0_DEFAULT, TRUE,  FALSE,  FALSE}, //40 LDO_VIBR
	{LDO_LDO_SIM0,  CAL_LDO_GROUP_A, CAL_LDO_SIM0,  7,   20, ANA_LDO_SIM0_REG1,  LDO_SIM0_V_MSK,  LDO_SIM0_V_SHIFT,  VDD1V8_DEFAULT, 0x3c, 1800, 3300, 0x3c, 0xd2, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //10 LDO_SIM0
	{LDO_LDO_SIM1,  CAL_LDO_GROUP_A, CAL_LDO_SIM1,  7,   20, ANA_LDO_SIM1_REG1,  LDO_SIM1_V_MSK,  LDO_SIM1_V_SHIFT,  VDD1V8_DEFAULT, 0x3c, 1800, 3300, 0x3c, 0xd2, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //11 LDO_SIM1
	{LDO_LDO_USB,   CAL_LDO_GROUP_A, CAL_LDO_USB,   7,   20, ANA_LDO_USB_REG1,   LDO_USB_V_MSK,   LDO_USB_V_SHIFT,   VDD3V3_DEFAULT, 0xd2, 1800, 3300, 0x3c, 0xd2, VDD3V3_DEFAULT, FALSE, FALSE,  FALSE}, //14 LDO_USB
	{LDO_LDO_SF,    CAL_LDO_GROUP_A, CAL_LDO_SF,    7,   20, ANA_LDO_SF_REG1,    LDO_SF_V_MSK,    LDO_SF_V_SHIFT,    VDD1V8_DEFAULT, 0x3c, 1800, 3300, 0x3c, 0xd2, VDD1V8_DEFAULT, FALSE, FALSE,  FALSE}, //41 LDO_SF
	{LDO_LDO_MAX,   0,               0,             0,   0,  0,                  0,               0,                 0,              0,    0,    0,    0,    0,    0,              0,     0,     0}
};
#endif

#if defined(PLATFORM_UIX8910)
#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UIS8910FF) 
LDO_CTL_T ldo_ctl_data[] =
{
	{LDO_LDO_CAMA,      ANA_LDO_CAMA_REG0,    BIT_0, NULL},
	{LDO_LDO_CAMIO,     ANA_LDO_CAMIO_REG0,   BIT_0, NULL},
	{LDO_LDO_LCD,       ANA_LDO_LCD_REG0,     BIT_0, NULL},
	{LDO_LDO_SD,        ANA_LDO_SD_REG2,      BIT_0, NULL},    /*sd EMMCCORE in 2720m_global*/
	{LDO_LDO_SDIO,      ANA_LDO_SD_REG2,      BIT_0, NULL},    /*sd EMMCCORE in 2720m_global*/
	{LDO_LDO_BTPHY,     ANA_LDO_BTPHY_REG0,   BIT_0, NULL},
	{LDO_LDO_VDD33,     ANA_LDO_USB_PD_REG,   BIT_0, NULL},
	{LDO_LDO_VIBR,      ANA_VIBR_CTRL,        BIT_8, NULL},
	{LDO_LDO_WPA,       ANA_DCDC_WPA_REG2,    BIT_0, NULL},
	{LDO_LDO_DDR12,     ANA_LDO_VDDR12_PD_REG,BIT_0, NULL},
    {LDO_LDO_MAX,       NULL,                 NULL,  NULL},
};
LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
#if 0
    {SLP_LDO_RF,            ANA_SLP_LDO_PD_CTRL0,   SLP_LDORF_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_DCXO,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDODCXO_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDD28,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOVDD28_PD_EN,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SDIO,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSD_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_USB,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOUSB_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMD0,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMD_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMA,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMA_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SIM1,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSIM1_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SIM0,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSIM0_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDDSF,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSF_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDDBTPA,       ANA_SLP_LDO_PD_CTRL0,   SLP_LDOBTPA_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_VDDVIB,        ANA_VIBR_CTRL,          SLP_LDOVIBR_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
#else
    {SLP_LDO_RF,            ANA_SLP_LDO_PD_CTRL0,   SLP_LDORF_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_ANA,           ANA_SLP_LDO_PD_CTRL1,   SLP_LDOANA_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    ////test
    {SLP_LDO_EMMC,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOMMC_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
#if !defined(POP_MDY_SUPPORT)
    {SLP_LDO_VDD28,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOVDD28_PD_EN,     SLP_BIT_SET,        TRUE,   NULL},
#endif
    {SLP_LDO_DDR12,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDODDR12_PD_EN,     SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_USB,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOUSB_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_LCD,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOLCD_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMD1,         ANA_SLP_LDO_PD_CTRL0,   0x10,                   SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMD0,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMD_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMA,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMA_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SIM2,          ANA_SLP_LDO_PD_CTRL0,   0x2,                    SLP_BIT_SET,        TRUE,   NULL},
    //{SLP_LDO_SIM1,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSIM1_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    //{SLP_LDO_SIM0,          ANA_SLP_LDO_PD_CTRL1,   SLP_LDOSIM0_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CP,            ANA_SLP_LDO_PD_CTRL1,   SLP_LDOCP_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    //{SLP_LDO_BTPHY,         ANA_SLP_LDO_PD_CTRL1,   SLP_LDOCON_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    //{SLP_LDO_SPIMEM,            ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSPIMEM_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
#if defined(POP_MDY_SUPPORT)
    {SLP_LDO_KB,            ANA_KPLED_CTRL,   BIT_9,        SLP_BIT_SET,        TRUE,   NULL},
#endif
#endif
    {SLP_LDO_MAX,           NULL,                   NULL,                   SLP_BIT_CLR,        TRUE,   NULL}
};

LOCAL CAL_LDO_T 	cal_ldo_data[] = {
	//{LDO_LDO_RF,    CAL_LDO_GROUP_B, CAL_LDO_RF,    8,   13, ANA_LDO_RF_REG1,    LDO_RF_V_MSK,    LDO_RF_V_SHIFT,    VDD1V8_DEFAULT, 0x60, 1200, 2000, 0x0,  0x7F, VDD1V8_DEFAULT, FALSE, FALSE,  FALSE}, //15 LDO_RF
	//{LDO_LDO_BTPA,  CAL_LDO_GROUP_B, CAL_LDO_BTPA,  8,   20, ANA_LDO_BTPA_REG1,  LDO_BTPA_V_MSK,  LDO_BTPA_V_SHIFT,  VDD3V3_DEFAULT, 0xd2, 1200, 3300, 0x0,  0xd2, VDD3V3_DEFAULT, TRUE,  FALSE,  FALSE}, //42 LDO_BTPA
	{LDO_LDO_SD,  CAL_LDO_GROUP_A, CAL_LDO_SD,    7,   25, ANA_LDO_SD_REG1,    LDO_SD_V_MSK,    LDO_SD_V_SHIFT,    VDD3V1_DEFAULT, 0x77, 1612, 3200, 0x0,  0x7F, VDD3V1_DEFAULT, TRUE,  FALSE,  FALSE}, //29 LDO_SDIO
	{LDO_LDO_CAMA,  CAL_LDO_GROUP_A, CAL_LDO_CAMA,  7,   25, ANA_LDO_CAMA_REG1,  LDO_CAMA_V_MSK,  LDO_CAMA_V_SHIFT,  VDD1V8_DEFAULT, 0x0f, 1612, 3200, 0x0,  0x7F, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
	{LDO_LDO_CAMD,  CAL_LDO_GROUP_A, CAL_LDO_CAMD,  7,   25, ANA_LDO_CAMD_REG1,  LDO_CAMD_V_MSK,  LDO_CAMD_V_SHIFT,  VDD1V2_DEFAULT, 0x20, 800,  1587, 0x0,  0x3F, VDD1V2_DEFAULT, TRUE,  FALSE,  FALSE}, //31 LDO_CAMD0
	{LDO_LDO_CAMIO, CAL_LDO_GROUP_A, CAL_LDO_CAMIO, 7,   25, ANA_LDO_CAMIO_REG1, LDO_CAMIO_V_MSK, LDO_CAMIO_V_SHIFT, VDD1V8_DEFAULT, 0x20, 1400, 2187, 0x0,  0x3F, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
	{LDO_LDO_LCD,   CAL_LDO_GROUP_A, CAL_LDO_LCD,   7,   25, ANA_LDO_LCD_REG1,   LDO_LCD_V_MSK,   LDO_LCD_V_SHIFT,   VDD1V8_DEFAULT, 0x0f, 1612, 3200, 0x0,  0x7F, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
    {LDO_LDO_VIBR,  CAL_LDO_GROUP_A, CAL_LDO_VIBR,  7,   200, ANA_VIBR_CTRL,      LDO_VIBR_V_MSK,  LDO_VIBR_V_SHIFT,  VDD3V0_DEFAULT, 0x02, 2800, 3300, 0x0,  0x07, VDD3V0_DEFAULT, TRUE,  FALSE,  FALSE}, //40 LDO_VIBR
    {LDO_LDO_BTPHY, CAL_LDO_GROUP_A, CAL_LDO_BTPHY, 7,   25, ANA_LDO_BTPHY_REG1, LDO_BTPHY_V_MSK, LDO_BTPHY_V_SHIFT, VDD1V5_DEFAULT, 0x20, 1100, 1887, 0x0,  0x3F, VDD1V5_DEFAULT, TRUE,  FALSE,  FALSE}, //40 LDO_VIBR
    {LDO_LDO_MAX,   0,               0,             0,   0,  0,                  0,               0,                 0,              0,    0,    0,    0,    0,    0,              0,     0,     0}
};

#else
LDO_CTL_T ldo_ctl_data[] =
{
	{LDO_LDO_SDIO,      ANA_LDO_SDIO_REG2,    BIT_1, NULL},
	{LDO_LDO_CAMA,      ANA_LDO_CAMA_REG0,    BIT_0, NULL},
	{LDO_LDO_CAMIO,     ANA_LDO_CAMIO_REG0,   BIT_0, NULL},
	//{LDO_LDO_CAMMOT,    ANA_LDO_CAMMOT_REG0,  BIT_0, NULL},
	{LDO_LDO_SD,        ANA_LDO_SD_REG2,      BIT_1, NULL},
	{LDO_LDO_CAMD,    ANA_LDO_CAMD_REG0,    BIT_0, NULL},
	{LDO_LDO_CAMIO,    ANA_LDO_CAMD_REG0,    BIT_0, NULL},
	{LDO_LDO_MAX,       NULL,                 NULL,  NULL},
};

LOCAL const SLP_LDO_CTL_T slp_ldo_ctl_data[] =
{
#if 0
    {SLP_LDO_RF,            ANA_SLP_LDO_PD_CTRL0,   SLP_LDORF_PD_EN,        SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_DCXO,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDODCXO_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDD28,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOVDD28_PD_EN,     SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SDIO,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSD_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_USB,           ANA_SLP_LDO_PD_CTRL0,   SLP_LDOUSB_PD_EN,       SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMD0,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMD_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_CAMA,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOCAMA_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_SIM1,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSIM1_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_SIM0,          ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSIM0_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDDSF,         ANA_SLP_LDO_PD_CTRL0,   SLP_LDOSF_PD_EN,        SLP_BIT_CLR,        TRUE,   NULL},
    {SLP_LDO_VDDBTPA,       ANA_SLP_LDO_PD_CTRL0,   SLP_LDOBTPA_PD_EN,      SLP_BIT_SET,        TRUE,   NULL},
    {SLP_LDO_VDDVIB,        ANA_VIBR_CTRL,          SLP_LDOVIBR_PD_EN,      SLP_BIT_CLR,        TRUE,   NULL},
#endif
    {SLP_LDO_MAX,           NULL,                   NULL,                   SLP_BIT_CLR,        TRUE,   NULL}
};

LOCAL CAL_LDO_T cal_ldo_data[] = {
	//{LDO_LDO_RF,    CAL_LDO_GROUP_B, CAL_LDO_RF,    8,   13, ANA_LDO_RF_REG1,    LDO_RF_V_MSK,    LDO_RF_V_SHIFT,    VDD1V8_DEFAULT, 0x60, 1200, 2000, 0x0,  0x7F, VDD1V8_DEFAULT, FALSE, FALSE,  FALSE}, //15 LDO_RF
	//{LDO_LDO_BTPA,  CAL_LDO_GROUP_B, CAL_LDO_BTPA,  8,   20, ANA_LDO_BTPA_REG1,  LDO_BTPA_V_MSK,  LDO_BTPA_V_SHIFT,  VDD3V3_DEFAULT, 0xd2, 1200, 3300, 0x0,  0xd2, VDD3V3_DEFAULT, TRUE,  FALSE,  FALSE}, //42 LDO_BTPA
	{LDO_LDO_SDIO,  CAL_LDO_GROUP_A, CAL_LDO_SD,    7,   20, ANA_LDO_SD_REG1,    LDO_SD_V_MSK,    LDO_SD_V_SHIFT,    VDD2V0_DEFAULT, 0x50, 2000, 3587, 0x0,  0x7F, VDD3V0_DEFAULT, TRUE,  FALSE,  FALSE}, //29 LDO_SDIO
	{LDO_LDO_CAMA,  CAL_LDO_GROUP_A, CAL_LDO_CAMA,  7,   20, ANA_LDO_CAMA_REG1,  LDO_CAMA_V_MSK,  LDO_CAMA_V_SHIFT,  VDD2V8_DEFAULT, 0x5f, 1612, 3200, 0x0,  0x7F, VDD2V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
	{LDO_LDO_CAMD,  CAL_LDO_GROUP_A, CAL_LDO_CAMD,  7,   20, ANA_LDO_CAMD_REG1,  LDO_CAMD_V_MSK,  LDO_CAMD_V_SHIFT,  VDD1V2_DEFAULT, 0x20, 800,  1587, 0x0,  0x3F, VDD1V2_DEFAULT, TRUE,  FALSE,  FALSE}, //31 LDO_CAMD0
	{LDO_LDO_CAMIO, CAL_LDO_GROUP_A, CAL_LDO_CAMIO, 7,   20, ANA_LDO_CAMIO_REG1, LDO_CAMIO_V_MSK, LDO_CAMIO_V_SHIFT, VDD1V8_DEFAULT, 0x20, 1800, 3300, 0x0,  0x3F, VDD1V8_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
	{LDO_LDO_CAMMOT,CAL_LDO_GROUP_A, CAL_LDO_CAMMOT,7,   20, ANA_LDO_CAMMOT_REG1,LDO_CAMMOT_V_MSK,LDO_CAMMOT_V_SHIFT,VDD3V0_DEFAULT, 0x50, 2000, 3587, 0x0,  0x7F, VDD3V0_DEFAULT, TRUE,  FALSE,  FALSE}, //30 LDO_CAMA
    //{LDO_LDO_VIBR,  CAL_LDO_GROUP_A, CAL_LDO_VIBR,  7,   20, ANA_VIBR_CTRL,      LDO_VIBR_V_MSK,  LDO_VIBR_V_SHIFT,  VDD3V0_DEFAULT, 0xb4, 1200, 3300, 0x0,  0xd2, VDD3V0_DEFAULT, TRUE,  FALSE,  FALSE}, //40 LDO_VIBR
	{LDO_LDO_MAX,   0,               0,             0,   0,  0,                  0,               0,                 0,              0,    0,    0,    0,    0,    0,              0,     0,     0}
};

#endif
#endif


/*****************************************************************************/
//  Description:  Ldo_Get_Cfg
//  Global resource dependence: NONE
//  Author:
//  Note:    Ldo_Get_Cfg
/*****************************************************************************/
PUBLIC LDO_CTL_PTR Ldo_Get_Cfg (void)
{
    LDO_CTL_PTR  ldo_ctl_tab = NULL;

    ldo_ctl_tab = ldo_ctl_data;

    return ldo_ctl_tab;
}

/*****************************************************************************/
//  Description:  Slp_Ldo_Get_Cfg
//  Global resource dependence: NONE
//  Author:
//  Note:    Slp_Ldo_Get_Cfg
/*****************************************************************************/
PUBLIC SLP_LDO_CTL_PTR Slp_Ldo_Get_Cfg (void)
{
    SLP_LDO_CTL_PTR  slp_ldo_ctl_tab = NULL;

    slp_ldo_ctl_tab = (SLP_LDO_CTL_PTR)slp_ldo_ctl_data;

    return slp_ldo_ctl_tab;
}

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UIX8910)
PUBLIC CAL_LDO_PTR Cal_Ldo_Get_Cfg (void)
{
    CAL_LDO_PTR  cal_ldo_data_tab = NULL;

    cal_ldo_data_tab = (CAL_LDO_PTR)cal_ldo_data;

    return cal_ldo_data_tab;
}

PUBLIC uint32 Cal_Ldo_Get_Tab_Size (void)
{
    uint32 cal_ldo_tab_size = 0;

    cal_ldo_tab_size = sizeof(cal_ldo_data)/sizeof(cal_ldo_data[0]);

    return cal_ldo_tab_size;
}
#endif

/**---------------------------------------------------------------------------*
 **                         Function Definitions                                                                               *
 **---------------------------------------------------------------------------*/

