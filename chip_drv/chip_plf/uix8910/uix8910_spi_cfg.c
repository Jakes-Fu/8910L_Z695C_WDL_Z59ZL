/******************************************************************************
 ** File Name:      sc6531efm_spi_cfg.c                                                *
 ** Author:         liuhao                                                   *
 ** DATE:           12/20/2010                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the hal layer of spi device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "../../chip_module/pin/uix8910/pinmap.h"
#include "sci_types.h"
#include "uix8910_reg_base.h"
#include "../../chip_module/spi/spi_phy.h"
#include "../../chip_module/pin/SC6531efm/pin_reg_sc6531efm.h"
#include "uix8910_spi_cfg.h"
//#include "../../chip_module/spi/uix8910/spi_reg_uix8910.h"
#include "../../chip_plf/common/inc/chip_drv_common_io.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/

extern SPI_PHY_FUN spi_phy_fun_v5;

#ifdef GPIO_SIMULATE_SPI_SUPPORT
extern SPI_PHY_FUN spi_phy_fun_simu;
#endif
/*lint -save -e785 */	
const SPI_PHY_CFG __spi_phy_cfg[SPI_ID_MAX] =   //only one cs line
{
    /*logic id, controller id, cs id, method*/
    {SPI0_0_CS0, SPI_BUS_ID_0, SPI_CS_ID_0, SPI_PIN_GID_0, &spi_phy_fun_v5}, /*hw spi controller1, cs 0*/
    //{SPI0_0_CS1, SPI_BUS_ID_0, SPI_CS_ID_1, SPI_PIN_GID_0, &spi_phy_fun_v5}, /*hw spi controller1, cs 1*/
#ifdef GPIO_SIMULATE_SPI_SUPPORT
    {SPI1_0_CS0, SPI_BUS_ID_1, SPI_CS_ID_0, SPI_PIN_GID_0, &spi_phy_fun_v5}, /*hw spi controller1, cs 1*/
#else
    {SPI1_0_CS0, SPI_BUS_ID_1, SPI_CS_ID_0, SPI_PIN_GID_0, &spi_phy_fun_v5},
    {SPI1_0_CD,  SPI_BUS_ID_1, SPI_CS_ID_0, SPI_PIN_GID_1, &spi_phy_fun_v5},
#endif
};
/*lint -restore*/
PUBLIC SPI_BASE_INFO __spi_base_info[SPI_BUS_MAX] =
{
    /*hw controller id, base address, tx chanel, rx chanel*/
    {SPI_BUS_ID_0, SPI0_BASE, INVALID_U16, INVALID_U16, SPI0_TX_REQ_ID, SPI0_RX_REQ_ID},/*hw spi controller1, register base*/
    {SPI_BUS_ID_1, SPI1_BASE, INVALID_U16, INVALID_U16, SPI1_TX_REQ_ID, SPI1_RX_REQ_ID},/*hw spi controller1, register base*/
#ifdef GPIO_SIMULATE_SPI_SUPPORT
    {SPI_BUS_ID_1, INVALID_U32, INVALID_U16, INVALID_U16, INVALID_U16, INVALID_U16}, /*sw spi controller1, no register base*/
    {SPI_BUS_ID_2, INVALID_U32, INVALID_U16, INVALID_U16, INVALID_U16, INVALID_U16}, /*sw spi controller2, no register base*/
    {SPI_BUS_ID_3, INVALID_U32, INVALID_U16, INVALID_U16, INVALID_U16, INVALID_U16} /*sw spi controller3, no register base*/
#endif
};

const SPI_PAD_INFO __spi_pad_func_info[]=
{
    /*SPI_PAD_NAME_E, pin_addr, value_spi, value_gpio, gpio_id*/
    {PAD_SPI0_0_CLK,  PIN_GPIO_9_CFG_REG, PIN_FUNC_SEL(1), PIN_FUNC_SEL(0), 9 },
    {PAD_SPI0_0_DI,  PIN_GPIO_12_CFG_REG,  PIN_FUNC_SEL(1), PIN_FUNC_SEL(0), 12 },
    {PAD_SPI0_0_DO,  PIN_GPIO_11_CFG_REG,  PIN_FUNC_SEL(1), PIN_FUNC_SEL(0), 11 },
    {PAD_SPI0_0_CS0,  PIN_GPIO_10_CFG_REG, PIN_FUNC_SEL(1), PIN_FUNC_SEL(0), 10 },

    {PAD_SPI1_0_CLK,  PIN_SDMMC1_DATA_1_CFG_REG, PIN_FUNC_SEL(7), PIN_FUNC_SEL(1), 26 },
    {PAD_SPI1_0_DI,  PIN_SDMMC1_DATA_3_CFG_REG,  PIN_FUNC_SEL(7), PIN_FUNC_SEL(1), 28 },
    {PAD_SPI1_0_DO,  PIN_SDMMC1_DATA_2_CFG_REG,  PIN_FUNC_SEL(7), PIN_FUNC_SEL(1), 27 },
    {PAD_SPI1_0_CS0,  PIN_SDMMC1_CLK_CFG_REG,PIN_FUNC_SEL(7), PIN_FUNC_SEL(0), 25 },
    {INVALID_U32, INVALID_U32,      INVALID_U32,   INVALID_U32, INVALID_U32}
};


/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/*  End Of File */
