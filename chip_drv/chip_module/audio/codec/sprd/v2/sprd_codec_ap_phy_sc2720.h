/*
 * <sprd_codec_ap_phy_sc2720.h> - <codec ap control.>
 *
 * Copyright (c) 2019 Unisoc Communications Inc.
 * History
 *      <15/04/2021> <yuzhen.long> <create> *      
 *      codec ap control for sc2720.
 */
#ifndef __SPRD_CODEC_AP_PHY_SC2720_H___
#define __SPRD_CODEC_AP_PHY_SC2720_H___

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_plf_export.h"

/**---------------------------------------------------------------------------*
 **                       A Die codec analog part                             *
 **                         Register description                              *
 **---------------------------------------------------------------------------*/

#define MASK_1_BIT 0x1
#define MASK_2_BITS 0x3
#define MASK_3_BITS 0x7
#define MASK_4_BITS 0xf
#define MASK_5_BITS 0x1f
#define MASK_6_BITS 0x3f
#define MASK_7_BITS 0x7f
#define MASK_8_BITS 0xff
#define MASK_16_BITS 0xffff

#define CHIP_ID_2720 0x2720
#define CHIP_ID_2721 0x2721

/// ANA_PMU0 0x00 ///

/* RG_AUD_VB_EN [15] RW 0 Audio LDO VB enable signal
 * RG_AUD_VB_NLEAK_PD [14] RW 0 Audio LDO VB prevent reverse flow back power down signal
                                        0 = power up        1 = power down
 * RG_AUD_VB_HDMC_SP_PD     [13]    RW  0x1 Audio LDO VB SLEEP MODE PD signal
                                        0 = EN   1 = PD
 * RG_AUD_BG_EN             [12]    RW  0   Audio BG EN
                                        0 = disable         1 = enable
 * RG_AUD_BIAS_EN           [11]    RW  0   Audio BIAS EN
                                        0 = disable         1 = enable
 * RG_AUD_MICBIAS_EN        [10]    RW  0   Audio Microphone bias enable signal
                                        0 = disable         1 = enable
 * RG_AUD_HMIC_BIAS_EN      [9]     RW  0   Audio Headset Micbias enable signal
                                        0 = disable         1 = enable
 * RG_AUD_HMIC_SLEEP_EN     [8]     RW  0   Audio HeadMic SLEEP MODE EN signal
                                        0 = disable         1 = enable
 * RG_AUD_MIC_SLEEP_EN      [7]     RW  0   Audio  MIC SLEEP MODE EN signal
                                        0 = disable         1 = enable
 * RG_AUD_VBG_SEL           [6]     RW  0   Audio BG Voltage
                                        0 :BG=1.55V         1: BG=1.5V
 * RG_AUD_VBG_TEMP_BIASTUNE [5]     RW  0   Audio BG Bias option
                                        0 = normal          1 = debug mode
 * RG_AUD_VBG_TEMP_TUNE     [4:3]   RW  0   Audio BG tune TC option
                                        00: normal          01: TC reduce
                                        10: TC reduce more  11: TC enhance
 * RG_AUD_MICBIAS_PLGB      [2]     RW  0x1 Audio MICBIAS power down signal (do not control discharge circuit)
                                        0 = power up        1 = power down
 * RG_AUD_HMICBIAS_VREF_SEL [1]     RW  0   Audio Headmic VREF
                                        0 = main-BG         1 = AUD_BG
 * RG_HMIC_COMP_MODE1_EN    [0]     RW  0   HMIC_COMP_MODE_EN
                                        0 = disable         1 = headmicbias filter RC integrated in chip */
#define VB_EN				(BIT_15)
#define VB_NLEAK_PD			(BIT_14)
#define VB_HDMC_SP_PD		(BIT_13)
#define BG_EN				(BIT_12)
#define BIAS_EN				(BIT_11)
#define MICBIAS_EN			(BIT_10)
#define HMIC_BIAS_EN		(BIT_9)/////
#define HMIC_SLEEP_EN		(BIT_8)
#define MIC_SLEEP_EN		(BIT_7)
#define VBG_SEL				(BIT_6)
#define VBG_TEMP_BIASTUNE	(BIT_5)

#define VBG_TEMP_TUNE		(3)
#define VBG_TEMP_TUNE_MASK	(MASK_2_BITS << VBG_TEMP_TUNE)
#define VBG_TEMP_TUNE_NORMAL			(0x0)
#define VBG_TEMP_TUNE_TC_REDUCE			(0x1)
#define VBG_TEMP_TUNE_TC_REDUCE_MORE	(0x2)
#define VBG_TEMP_TUNE_TC_ENHANCE		(0x3)

#define MICBIAS_PLGB		(BIT_2)
#define HMICBIAS_VREF_SEL	(BIT_1)
#define HMIC_COMP_MODE1_EN	(BIT_0)

/// ANA_PMU1 0x04 ///
/* RG_AUD_VB_CAL [15: 11] RW 0x10 Audio LDO_VB output voltage calibration signal
									00000 = -13.3%  00001 = -12.5%
									00010 = -11.7%  00011 = -10.8%
									00100 = -10%    00101 = -9.2%
									00110 = -8.4%   00111 = -7.5%
									01000 = -6.7%   01001 = -5.8%
									01010 = -5%     01011 = -4.2%
									01100 = -3.3%   01101 = -2.5%
									01110 = -1.67%  01111 = -0.83%
									10000 = 0       10001 = 0.83%
									10010 = 1.67%   10011 = 2.5%
									10100 = 3.3%    10101 = 4.2%
									10110 = 5%      10111 = 5.8%
									11000 = 6.7%    11001 = 7.5%
									11010 = 8.4%    11011 = 9.2%
									11100 = 10%     11101 = 10.8%
									11110 = 11.7%   11111 = 12.5%
 * RG_AUD_VB_V [10: 6]  RW  0x10 Audio ADC/DAC/DRV VCM & LDO VB output voltage control bit
									(VB should be set larger than 3.0V)
									00000 -00011 = forbidden
									00100 = 3.0V        00101 = 3.025V
									00110 = 3.05V        00111 = 3.075V
									01000 = 3.1V        01001 = 3.125V
									01010 = 3.15V        01011 = 3.175V
									01100 = 3.2V        01101 = 3.225V
									01110 = 3.25V        01111 = 3.275V
									10000 = 3.3V        10001 = 3.325V
									10010 = 3.35V        10011 = 3.375V
									10100 = 3.4V        10101 = 3.425V
									10110 = 3.45V        10111 = 3.475V
									11000 = 3.5V        11001 = 3.525V
									11010 = 3.55V        11011 = 3.575V
									11100 =3.6V 11101-11111 = forbidden
 * RG_AUD_HMIC_BIAS_V   [5: 3]   RW  0x5     Audio headmicbias output voltage control bit
									000 = 2.2V        001 = 2.4V
									010 = 2.5V        011 = 2.6V
									100 = 2.7V        101 = 2.8V
									110 = 2.9V        111 = 3.0V
 * RG_AUD_MICBIAS_V     [2: 0]   RW  0x2     Audio MICBIAS output voltage select signal
									000 = 2.2V        001 = 2.4V
									010 = 2.5V        011 = 2.6V
									100 = 2.7V        101 = 2.8V
									110 = 2.9V        111 = 3.0V */
#define VB_CAL				(11)
#define VB_CAL_MASK			(MASK_5_BITS << VB_CAL)
#define VB_V				(6)
#define VB_V_MASK			(MASK_5_BITS << VB_V)
#define HMIC_BIAS_V			(3)
#define HMIC_BIAS_V_MASK	(MASK_3_BITS << HMIC_BIAS_V)
#define MICBIAS_V			(0)
#define MICBIAS_V_MASK		(MASK_3_BITS << MICBIAS_V)

/// ANA_PMU2 0x08 ///
/* RG_AUD_HP_IB     [15: 14]    RW  0   AUD HP-PGA BIAS current:
										00:X1  11:X2
 * RG_AUD_HP_IBCUR3 [13: 11]    RW  0x2 AUD HP-PGA 3rd stage BIAS current:
                                        000: 5uA   001:7.5uA
                                        010:10uA   011: 12.5uA
                                        100: 15uA  101: 17.5uA
                                        110:20uA   111: 22.5uA
 * RG_AUD_PA_AB_I   [10: 9]     RW  0x1 Audio PA class-AB mode Quiescent current decreasing level
                                        00=3.5mA, 01=2.5mA, 10=1.9mA, 11=1.6mA
 * RG_AUD_ADPGA_IBIAS_SEL   [8: 5]      RW  0   Audio ADC & PGA ibias current control bit
                                        <3:2> control the ibias of the PGA
                                        00=10uA, 01=7.5uA, 10=5uA, 11=5uA
                                        <1:0> control the ibias of the modulato
                                        00 = 5uA 01 = 3.75uA
                                        10 = 2.5uA 11 = 2.5uA
 * RG_AUD_DA_IG     [4: 3]      RW  0   Audio DACL & DACR & DACS output gain control bit
                                        00 = 0dB
                                        01 = -0.75dB
                                        1x=-1.5dB
 * RG_AUD_DRV_PM_SEL [2: 1]     RW  0x3 HP&RCV DRV SEL
                                        00:  input gm/2, miller cap cut
                                        01:  input gm/2, miller cap normal
                                        10:  input gm normal, miller cap cut
                                        11:  input gm normal, miller cap normal
 * Reserved          [0] */

#define HP_IB           (14)
#define HP_IB_MASK      (MASK_2_BITS << HP_IB)
#define HP_IB_X1        (0x0)
#define HP_IB_X2        (0x3)

#define HP_IBCUR3       (11)
#define HP_IBCUR3_MASK  (MASK_3_BITS << HP_IBCUR3)
#define HP_IBCUR3_5UA   (0x0)
#define HP_IBCUR3_7P5UA (0x1)
#define HP_IBCUR3_10UA  (0x2)
#define HP_IBCUR3_11UA  (0x3)

#define PA_AB_I         (9)
#define PA_AB_I_MASK    (MASK_2_BITS << PA_AB_I)

#define ADPGA_IBIAS_SEL (5)
#define ADPGA_IBIAS_SEL_MASK    (MASK_4_BITS << ADPGA_IBIAS_SEL)
#define ADPGA_IBIAS_SEL_PGA_7P5_ADC_3P75 (5)

#define DA_IG (3)
#define DA_IG_MASK      (MASK_2_BITS << DA_IG)

#define DRV_PM_SEL      (1)
#define DRV_PM_SEL_MASK (MASK_2_BITS << DRV_PM_SEL)
#define DRV_PM_SEL_1    (0x1)
#define DRV_PM_SEL_3    (0x3)

/// ANA_PMU3 0x0C ///

/* RG_AUD_PA_OTP_PD    [15]     RW  0x1 Audio PA over temperature protection circuit power down signal
                                        0 = power up          1 = power down
 * RG_AUD_PA_OTP_T     [14: 12] RW  0x6 Audio PA over temperature protection circuit temperature select
                                        000: 4C -> -14C       001: 25C -> 8C
                                        010: 47C -> 31C       011: 68C -> 52C
                                        100: 89C -> 74C       101: 110C -> 95C
                                        110: 130C -> 115C     111: 150C -> 135C
 * RG_AUD_PA_OVP_PD    [11]     RW  0x1 Audio VBAT_PA over voltage protection circuit power down signal
                                        0 = power up          1 = power down
 * RG_AUD_PA_OVP_THD   [10]     RW  0   Audio VBAT_PA over voltage protection circuit threshold select
                                        0 = 0.3V                1 = 0.6V
 * RG_AUD_PA_OVP_V     [9:7]    RW  0   Audio VBAT_PA over voltage protection circuit voltage select
                                        RG_AUD_PA_OVP_THD = 0/1
                                        000 = 5.87/5.93 -> 5.55/5.37
                                        001 = 6.10/6.16 -> 5.76/5.57
                                        010 = 6.32/6.38 -> 6.01/5.78
                                        011 = 6.54/6.61 -> 6.22/6.02
                                        100 = 6.76/6.83 -> 6.44/6.23
                                        101 = 6.99/7.06 -> 6.65/6.43
                                        110 = 7.21/7.28 -> 6.86/6.64
                                        111 = 7.43/7.51 -> 7.07/6.85
 * RG_AUD_PA_OCP_PD    [6]      RW  0x1 Audio PA over current protection circuit power down signal
                                        0 = power up,1 = power down
 * RG_AUD_PA_OCP_S     [5]      RW  0x1 Audio PA class-AB mode over current protection circuit current select
                                        0=800mA  1=1000mA
 * RG_AUD_DRV_OCP_PD   [4]      RW  0x1 Audio Driver over current protection circuit power down signal
                                        0 = power up,1 = power down
 * RG_AUD_DRV_OCP_MODE [3:2]    RW  0   Audio Driver over current protection current select
                                        HP mode:  00--108mA
                                        01--150mA    10 --156mA     11--195mA
                                        RCV mode: 00--209mA
                                        01-- 300mA   10 -- 310mA    11 -- 400mA
 * RG_AUD_PA_VCM_V     [1:0]    RW  0x1 Audio PA VCOM voltage control bit
                                        00 = 0.55xVDD  01 = 0.5xVDD
                                        10 = 0.45xVDD  11 = 0.4xVDD */

#define PA_OTP_PD			(BIT_15)

#define PA_OTP_T			(12)
#define PA_OTP_T_MASK		(MASK_3_BITS << PA_OTP_T)

#define PA_OVP_PD			(BIT_11)
#define PA_OVP_THD			(BIT_10)

#define PA_OVP_V			(7)
#define PA_OVP_V_MASK	    (MASK_3_BITS << PA_OVP_V)
#define PA_OVP_V_5P8		(0)
#define PA_OVP_V_6P0		(1)
#define PA_OVP_V_6P2		(2)
#define PA_OVP_V_6P4		(3)
#define PA_OVP_V_6P6		(4)
#define PA_OVP_V_6P8		(5)
#define PA_OVP_V_7P0		(6)
#define PA_OVP_V_7P2		(7)

#define PA_OCP_PD			(BIT_6)
#define PA_OCP_S			(BIT_5)
#define DRV_OCP_PD			(BIT_4)

#define DRV_OCP_MODE		(2)
#define DRV_OCP_MODE_MASK	(MASK_2_BITS << DRV_OCP_MODE)
#define DRV_OCP_MODE_156MA	(0x2)

#define PA_VCM_V			(0)
#define PA_VCM_V_MASK		(MASK_2_BITS << PA_VCM_V)

/*bits definitions for register ANA_PMU4  0x10 */

/* RG_AUD_PA_KSEL    [14:13]   RW  0x2      Audio PA class-D mode PWM Gain select
                                            00 = 1                 01 = 1.5
                                            10 = 1.67              11 = 2
/* RG_AUD_PA_DEGSEL  [12:11]   RW  0x1      Audio PA class-D mode PWM logic delay time select
                                            00 = 7ns               01 = 14ns
                                            10 = 24ns              11 = 29ns
/* RG_AUD_PA_DEGSEL  [10:8]    RW  0x0      Audio PA class-D output edge slew rate control
                                            000 = 2ns      001 = 4ns     010 = 6ns       011 = 8ns
                                            100 = 10ns     101 = 12ns    110 = 14ns      111 = 16ns
/* RG_AUD_PA_SS_EN   [7]       RW  0x0      Audio PA class-D mode spread spectrum enable signal
                                            0 = disable          1 = enable
/* RG_AUD_PA_SS_RST  [6]       RW  0x0      Audio PA class-D mode spread spectrum reset enable signal
                                            0 = disable          1 = enable
/* RG_AUD_PA_SS_F    [5:4]     RW  0x0      Audio PA class-D mode spread spectrum dither level select signal
                                            when PA_DTRI_F<1:0> = 00/01/10/11
                                            00 = 3.2%/1.6%/0.8%/0.4%
                                            01 =   9%/4.7%/2.3%/1.2%
                                            10 =  22%/ 11%/5.5%/2.7%
                                            11 =  47%/ 23%/ 12%/  6%
/* RG_AUD_PA_SS_32K_EN  [3]    RW  0x0      Audio PA class-D mode spread spectrum 32k dither clock select signal
                                            0 = disable           1 = enable
/* RG_AUD_PA_SS_T    [2:0]     RW  0x0      Audio PA class-D mode spread spectrum dither clock divider select signal
                                            000 = 1      001 = 1/2    010 = 1/4    011 = 1/8
                                            100 = 1/16   101 = 1/32   110 = 1/64   111 = 1/128 */

#define PA_KSEL				(13)
#define PA_KSEL_MASK		(MASK_2_BITS << PA_KSEL)

#define PA_DEGSEL			(11)
#define PA_DEGSEL_MASK		(MASK_2_BITS << PA_DEGSEL)

#define PA_EMI_L			(8)
#define PA_EMI_L_MASK		(MASK_3_BITS << PA_EMI_L)

#define PA_SS_EN			(BIT_7)
#define PA_SS_RST			(BIT_6)

#define PA_SS_F				(4)
#define PA_SS_F_MASK		(MASK_2_BITS << PA_SS_F)

#define PA_SS_32K_EN		(BIT_3)

#define PA_SS_T				(0)
#define PA_SS_T_MASK		(MASK_3_BITS << PA_SS_T)


/*bits definitions for register ANA_PMU5  0x14 */
/* RG_AUD_PA_D_EN         [14]    RW  0   Audio Speaker PA class-D mode enable signal
                                          0 = disable (CLASS-AB mode)      1 = enable(CLASS-D mode)
 * RG_AUD_PA_DFLCK_EN     [13]    RW  0   Audio Speaker PA class-D mode switching frequency locking enable signal
                                          0 = disable          1 = enable
 * RG_AUD_PA_DFLCK_RSL    [12]    RW  0   Audio Speaker PA class-D mode switching frequency locking resolution select
                                          0 = 1X                 1 = 2X
 * RG_AUD_PA_DTRI_FC      [11:9]  RW  0x2 Audio Speaker PA class-D mode switching frequency select
                                          000 = 330kHz    001 = 490kHz     010 = 650KHz     011 = 810KHz
                                          100 = 970kHz    101 = 1.12MHz    110 = 1.27MHz    111 = 1.42MHz
 * RG_AUD_PA_DTRI_FF      [8:3]   RW  0   Audio PA class-D mode Switching frequency hopping level
                                            000000=0Hz          000001=1*2.5KHz     000010=2*2.5KHz     000011=3*2.5KHz
                                            000100=4*2.5KHz     000101=5*2.5KHz     000110=6*2.5KHz     000111=7*2.5KHz
                                            001000=8*2.5KHz     001001=9*2.5KHz     001010=10*2.5KHz    001011=11*2.5KHz
                                            001100=12*2.5KHz    001101=13*2.5KHz    001110=14*2.5KHz    001111=15*2.5KHz
                                            010000=16*2.5KHz    010001=17*2.5KHz    010010=18*2.5KHz    010011=19*2.5KHz
                                            010100=20*2.5KHz    010101=21*2.5KHz    010110=22*2.5KHz    010111=23*2.5KHz
                                            011000=24*2.5KHz    011001=25*2.5KHz    011010=26*2.5KHz    011011=27*2.5KHz
                                            011100=28*2.5KHz    011101=29*2.5KHz    011110=30*2.5KHz    011111=31*2.5KHz
                                            100000=32*2.5KHz    100001=33*2.5KHz    100010=34*2.5KHz    100011=35*2.5KHz
                                            100100=36*2.5KHz    100101=37*2.5KHz    100110=38*2.5KHz    100111=39*2.5KHz
                                            101000=40*2.5KHz    101001=41*2.5KHz    101010=42*2.5KHz    101011=43*2.5KHz
                                            101100=44*2.5KHz    101101=45*2.5KHz    101110=46*2.5KHz    101111=47*2.5KHz
                                            110000=48*2.5KHz    110001=49*2.5KHz    110010=50*2.5KHz    110011=51*2.5KHz
                                            110100=52*2.5KHz    110101=53*2.5KHz    110110=54*2.5KHz    110111=55*2.5KHz
                                            111000=56*2.5KHz    111001=57*2.5KHz    111010=58*2.5KHz    111011=59*2.5KHz
                                            111100=60*2.5KHz    111101=61*2.5KHz    111110=62*2.5KHz    111111=63*2.5KHz
 * RG_AUD_PA_STOP_EN       [2]     RW  0  Audio PA Driver stop output enable signal
                                          0 = disable,  1 = enable
 * RG_AUD_PA_SH_DET_EN     [1]     RW  0  Audio PA output short to VBAT detect enable signal
                                          0 = disable          1 = enable
 * RG_AUD_PA_SL_DET_EN     [0]     RW  0  Audio PA output short to GND detect enable signal
                                          0 = disable          1 = enable */

#define PA_D_EN				(BIT_14)
#define PA_DFLCK_EN			(BIT_13)
#define PA_DFLCK_RSL		(BIT_12)

#define PA_DTRI_FC			(9)
#define PA_DTRI_FC_MASK		(MASK_3_BITS << PA_DTRI_FC)

#define PA_DTRI_FF			(3)
#define PA_DTRI_FF_MASK		(MASK_6_BITS << PA_DTRI_FF)

#define PA_STOP_EN			(BIT_2)
#define PA_SH_DET_EN		(BIT_1)
#define PA_SL_DET_EN		(BIT_0)

/// ANA_CLK0 0x18 ///

/* RG_AUD_DIG_CLK_6P5M_EN  [14]    RW  0   Audio digital core clcok input enable signal
 * RG_AUD_DIG_CLK_LOOP_EN  [13]    RW  0   Audio digital loop clcok input enable signal
 * RG_AUD_ANA_CLK_EN       [12]    RW  0   Audio analog core clcok input enable signal
 * RG_AUD_AD_CLK_EN        [11]    RW  0   Audio analog ADC clock input enable signal
 * RG_AUD_AD_CLK_RST       [10]    RW  1   Audio analog ADC clock reset enable signal
 * RG_AUD_DA_CLK_EN        [9]     RW  0   Audio DAC clock input enable signal
 * RG_AUD_DRV_CLK_EN       [8]     RW  0   Audio DRV clock input enable signal
 * RG_AUD_DCDCGEN_CLK_EN   [7]     RW  0   Audio DCDC GEN clock input enable signal
                                           0 = disable          1 = enable
 * RG_AUD_DCDCMEM_CLK_EN   [6]     RW  0   Audio DCDC MEM clock input enable signal
                                           0 = disable          1 = enable
 * RG_AUD_DCDCCORE_CLK_EN  [5]     RW  0   Audio DCDC CORE clock input enable signal
                                           0 = disable          1 = enable
 * RG_AUD_DCDCCHG_CLK_EN   [4]     RW  0   Audio VAD enable signal
                                           0 = disable          1 = enable
 * RG_AUD_AD_CLK_F         [3:2]   RW  0   Audio ADC clock frequency select (based on Fclk=6.5MHz)
                                            00 = Fclk              01/11 = Fclk / 3
                                            10 = Fclk / 2
 * RG_AUD_DA_CLK_F         [1:0]   RW  0   Audio DAC clock frequency select (based on Fclk=6.5MHz)
                                            00 = Fclk              01/11 = Fclk x 2
                                            10 = Fclk / 2 */
#define DIG_CLK_6P5M_EN		(BIT_14)
#define DIG_CLK_LOOP_EN		(BIT_13)
#define ANA_CLK_EN		    (BIT_12)
#define AD_CLK_EN		    (BIT_11)
#define AD_CLK_RST		    (BIT_10)
#define DA_CLK_EN		    (BIT_9)
#define DRV_CLK_EN		    (BIT_8)
#define DCDCGEN_CLK_EN		(BIT_7)
#define DCDCMEM_CLK_EN		(BIT_6)
#define DCDCCORE_CLK_EN		(BIT_5)
#define DCDCCHG_CLK_EN		(BIT_4)

#define AD_CLK_F		    (2)
#define AD_CLK_F_MASK		(MASK_2_BITS << AD_CLK_F)
#define AD_CLK_F_HALF		(2)

#define DA_CLK_F		    (0)
#define DA_CLK_F_MASK		(MASK_2_BITS << DA_CLK_F)

/// ANA_CDC0 0x1c ///

/* RG_AUD_ADPGA_IBIAS_EN  [15]    RW  0     Audio PGA&ADC BIAS en signal
                                            0 = disable          1 = enable
 * RG_AUD_ADPGA_IBUF_EN   [14]    RW  0     Audio PGA & ADC VCM buffer enable signal
                                            0 = disable          1 = enable
 * RG_AUD_ADPGAL_EN       [13]    RW  0     Audio ADC PGAL enable signal
                                            0 = disable          1 = enable
 * RG_AUD_ADPGAR_EN       [12]    RW  0     Audio ADC PGAR enable signal
                                            0 = disable          1 = enable

 * RG_AUD_ADPGAL_BYP      [11:10] RW  0     Audio ADC PGAL bypass select signal
                                            00 = normal input                 01 = HEADMIC to ADCL
                                            10/11 = All disconnected
 * RG_AUD_ADPGAR_BYP      [9:8]   RW  0     Audio ADC PGAR bypass select signal
                                            00 = normal input                 01 = HEADMIC to ADCR
                                            10/11 = All disconnected
 * RG_AUD_ADL_EN          [7]    RW  0     Audio ADCL enable signal
                                            0 = disable          1 = enable
 * RG_AUD_ADL_RST         [6]    RW  0      Audio ADCL reset enable signal
                                            0 = disable          1 = enable
 * RG_AUD_ADR_EN          [5]    RW  0      Audio ADCR enable signal
                                            0 = disable          1 = enable
 * RG_AUD_ADR_RST         [4]    RW  0      Audio ADCR reset enable signal
                                            0 = disable          1 = enable
 * RG_AUD_VREF_SFCUR      [3]    RW  0      Audio ADC VREF current drv increasing by 1.3 times enable signal
                                            0 = disable          1 = enable
 * RG_AUD_SHMIC_DPOP      [2]    RW  0      Headmic button release depop signal
                                            0 = disable    1 = depop
 * RG_AUD_SHMIC_DPOPVCM_EN [1]   RW  0      Headmic button release depop signal to VCM enable
                                            0 = disable    1 = depop
 * RG_AUD_ADC_BULKSW      [0]    RW  0      Reserved */

#define ADPGA_IBIAS_EN		(BIT_15)
#define ADPGA_IBUF_EN		(BIT_14)
#define ADPGAL_EN		    (BIT_13)
#define ADPGAR_EN		    (BIT_12)

#define ADPGAL_BYP		    (10)
#define ADPGAL_BYP_MASK		(MASK_2_BITS << ADPGAL_BYP)

#define ADPGAR_BYP		        (8)
#define ADPGAR_BYP_MASK		    (MASK_2_BITS << ADPGAR_BYP)
#define ADPGAR_BYP_NORMAL	    (0)
#define ADPGAR_BYP_HDST2ADC     (1)
#define ADPGAR_BYP_ALL_DISCON	(2)

#define ADL_EN			    (BIT_7)
#define ADL_RST			    (BIT_6)
#define ADR_EN			    (BIT_5)
#define ADR_RST			    (BIT_4)
#define VREF_SFCUR		    (BIT_3)
#define SHMIC_DPOP		    (BIT_2)
#define SHMIC_DPOPVCM_EN	(BIT_1)
#define ADC_BULKSW	        (BIT_0) //Reserved

/// ANA_CDC1 0x20 ///

/* RG_AUD_ADPGA_RESERVED [15:14] RW  0x1    ADC_REF SETTING
                                            00: 1.55    01:1.6    10:1.65     11:1.7
 * RG_AUD_ADVCMI_INT_SEL [13:12] RW  0x1    ADPGA_Internal common voltage select
                                            00: 0.5*VB  01: 0.45*VB  10: 0.425*VB  11:0.4*VB
 * RG_AUD_ADPGAL_G [11:9]   RW  0x1         Audio ADC PGAL Gain control
                                            000 = 0dB   001 = 3dB   010 = 6dB   011 = 12dB
                                            100 = 18dB  101 = 24dB  110 = 30dB  111 = 36dB
 * RG_AUD_ADPGAR_G [8:6]    RW  0x1         Audio ADC PGAR Gain control
                                            000 = 0dB   001 = 3dB   010 = 6dB   011 = 12dB
                                            100 = 18dB  101 = 24dB  110 = 30dB  111 = 36dB
 * RG_AUD_DALR_OS_D [5: 3]  RW  0           Audio DACL/R dc offset trim bit
                                            000 = 0  001 = +1/20*VFS 010 = +2/20*VFS  011 = +1/20*VFS
                                            100 = 0  101 = -1/20*VFS 110 = -2/20*VFS  111 = -1/20*VFS
 * RG_AUD_DAS_OS_D  [2: 0]  RW  0           Audio DACS dc offset trim bit
                                            000 = 0  001 = +1/20*VFS 010 = +2/20*VFS  011 = +1/20*VFS
                                            100 = 0  101 = -1/20*VFS 110 = -2/20*VFS  111 = -1/20*VFS */
#define ADVCMI_INT_SEL		(12)
#define ADVCMI_INT_SEL_MASK	(MASK_2_BITS << ADVCMI_INT_SEL)

#define ADPGAL_G		(9)
#define ADPGAL_G_MASK	(MASK_3_BITS << ADPGAL_G)

#define ADPGAR_G		(6)
#define ADPGAR_G_MASK	(MASK_3_BITS << ADPGAR_G)

#define DALR_OS_D		(3)
#define DALR_OS_D_MASK	(MASK_3_BITS << DALR_OS_D)
#define DALR_OS_D_0		(0)
#define DALR_OS_D_2		(0x2)

#define DAS_OS_D		(0)
#define DAS_OS_D_MASK   (MASK_3_BITS << DAS_OS_D)
#define DAS_OS_D_0		(0)
#define DAS_OS_D_2		(0x2)

/// ANA_CDC2 0x24 ///

/* RG_AUD_DAS_EN   [13] RW  0   Audio DACS enable signal
 * RG_AUD_DAL_EN   [12] RW  0   Audio DACL enable signal
 * RG_AUD_DAR_EN   [11] RW  0   Audio DACR enable signal
 * HPL_FLOOPEN     [10] RW  0   Audio Driver HPL dummy loop enable signal
 * HPL_FLOOP_END   [9]  RW  0   Audio Driver HPL dummy loop end enable signal
 * HPR_FLOOPEN     [8]  RW  0   Audio Driver HPR dummy loop enable signal
 * HPR_FLOOP_END   [7]  RW  0   Audio Driver HPR dummy loop end enable signal
 * RCV_FLOOPEN     [6]  RW  0   Audio Driver RCV dummy loop enable signal
 * RCV_FLOOP_END   [5]  RW  0   Audio Driver RCV dummy loop end enable signal
 * RG_AUD_HPL_EN   [4]  RW  0   Audio Driver HPL output enable signal
 * RG_AUD_HPR_EN   [3]  RW  0   Audio Driver HPR output enable signal
 * RG_AUD_HPBUF_EN [2]  RW  0   Audio Driver vcm buffer enable signal
 * RG_AUD_RCV_EN   [1]  RW  0   Audio Receiver/Earpiece Driver
                                RCV_P/RCV_N output enable signal (RG_AUD_CP_EN[1:0]=1)
 * RG_AUD_PA_EN    [0]  RW  0   Audio Speaker PA (Driver SPKL) enable signal */
#define DAS_EN          (BIT_13)
#define DAL_EN          (BIT_12)
#define DAR_EN          (BIT_11)
#define HPL_FLOOPEN     (BIT_10)
#define HPL_FLOOP_END   (BIT_9)
#define HPR_FLOOPEN     (BIT_8)
#define HPR_FLOOP_END   (BIT_7)
#define RCV_FLOOPEN     (BIT_6)
#define RCV_FLOOP_END   (BIT_5)
#define HPL_EN          (BIT_4)
#define HPR_EN          (BIT_3)
#define HPBUF_EN        (BIT_2)
#define RCV_EN          (BIT_1)
#define PA_EN           (BIT_0)

/// ANA_CDC3 0x28 ///

/* RG_AUD_DALR_OS_EN    [12] RW  0   Audio DACL/R dc offset enable signal
 * RG_AUD_DAS_OS_EN     [11] RW  0   Audio DACS dc offset enable signal
 * RG_AUD_NG_PA_EN      [10] RW  0   NG_PA enable control
 * RG_AUD_SDALHPL       [9]  RW  0   Audio DACL to HPL enable signal
 * RG_AUD_SDARHPR       [8]  RW  0   Audio DACR to HPR enable signal
 * RG_AUD_SDALRCV       [7]  RW  0   Audio DACL to Receiver/Earpiece enable signal
 * RG_AUD_SDAPA         [6]  RW  0   Audio DACS to PA enable signal
 * RG_AUD_SHMICPA_DEBUG [5]  RW  0   Audio HMIC to PA enable signal
                                     0 = disable          1 = enable
                                     when debug=1, HMIC to PA path on, no matter "RG_AUD_SDAPA"
                                     when debug=0, HMIC to PA path off, "RG_AUD_SDAPA" is enable
 * RG_AUD_SMICDRV_DEBUG [4]  RW  0   Audio MIC to LDRV enable signal
                                     0 = disable          1 = enable
                                     when debug=1, MIC to LDRV path on , "RG_AUD_SDALHPL"/"RG_AUD_SDALRCV" is dis-enable
                                     when debug=0, MIC to LDRV path off , "RG_AUD_SDALHPL"/"RG_AUD_SDALRCV" is enable
 * RG_AUD_SMIC1PGAL     [3]  RW  0   MIC1 to Audio ADC PGAL enable signal
 * RG_AUD_SMIC2PGAR     [2]  RW  0   MIC2 to Audio ADC PGAR enable signal
 * RG_AUD_SHMICPGAL     [1]  RW  0   HEADMIC to Audio ADC PGAL enable signal
 * RG_AUD_SHMICPGAR     [0]  RW  0   HEADMIC to Audio ADC PGAR enable signal */
#define DALR_OS_EN	(BIT_12)
#define DAS_OS_EN	(BIT_11)
#define PA_NG_EN	(BIT_10)
#define SDALHPL		(BIT_9)
#define SDARHPR		(BIT_8)
#define SDALRCV		(BIT_7)
#define SDAPA		(BIT_6)
#define SHMICPA_DEBUG	(BIT_5)
#define SMICDRV_DEBUG	(BIT_4)
#define SMIC1PGAL	(BIT_3)
#define SMIC2PGAR	(BIT_2)
#define SHMICPGAL	(BIT_1)
#define SHMICPGAR	(BIT_0)

/// ANA_CDC4 0x2c ///

/* RG_AUD_SPK_G [15:12] RW  0x1 Audio Speaker Driver PGA Gain control
                                <3:2>For Class-D PGA, dft=00
                                00 = 0dB        01 = 1.5dB        10 = 3dB        11 = 3dB
                                <1:0>For Class-AB PGA
                                00 = -3dB(20K)  01 = 0dB(28K)  10 = 1.16dB(32K)  11 = 1.16dB(32K)
 * RG_AUD_RCV_G [11:8]  RW  0x2 Audio Receiver/Earpiece Driver RCV_P/RCV_N PGA Gain control
                                0010 =  6dB     0011 =  3dB
                                0100 =  0dB     0101 = -3dB
                                0110 = -6dB     0111 = -9dB
                                1000 = -12dB    1001 = -18dB
                                1010 = -18dB    1111 = mute
 * RG_AUD_HPL_G [7:4]   RW  0x4 Audio Headphone left channel Gain control
                                0100 =  0dB   0101 =  -3dB  0110 = -6dB   0111 = -9dB
                                1000 = -12dB  1001 = -18dB  1010 = -18dB  1111 = mute
 * RG_AUD_HPR_G [3:0]   RW  0x4 Audio Headphone right channel Gain control
                                0100 =  0dB   0101 =  -3dB  0110 = -6dB   0111 = -9dB
                                1000 = -12dB  1001 = -18dB  1010 = -18dB  1111 = mute */
#define PA_G		(12)
#define PA_G_MASK	(MASK_4_BITS << PA_G)
#define PA_AB_G_MASK	(MASK_2_BITS << PA_G)
#define PA_AB_G_0DB	(0X1)
#define PA_D_G		(14)

#define RCV_G		(8)
#define RCV_G_MASK	(MASK_4_BITS << RCV_G)
#define RCV_G_MUTE	(0xF)

#define HPL_G		(4)
#define HPL_G_MASK	(MASK_4_BITS << HPL_G)
#define HPL_G_MUTE	(0xF)

#define HPR_G		(0)
#define HPR_G_MASK	(MASK_4_BITS << HPR_G)
#define HPR_G_MUTE	(0xF)

/// ANA_HDT0 0x30 ///
/* RG_AUD_HEDET_MUX2ADC_SEL_PD   [15]  RW 0 MUX2ADC SEL PD
                                            0 = power up      1 = power down
 * RG_AUD_HEDET_BUF_EN           [14]  RW 0 Audio signal input to AuxADC enable signal
 * RG_AUD_HEDET_BDET_EN          [13]  RW 0 Audio headset button detect circuit enable signal
 * RG_AUD_HEDET_V2I_EN           [12]  RW 0 Audio headset detect signal RG_HP_DRIVER_EN software control enable signal
                                            0 = DG_HP_DRIVER_EN work       1 = RG_HP_DRIVER_EN work
 * RG_AUD_HEDET_VREF_EN          [11]  RW 0 Audio headset detect reference voltage circuit enable signal
 * RG_AUD_HEDET_MICDET_EN        [10]  RW 0 Audio headset mic detect circuit power enable signal
 * RG_AUD_HEDET_V2AD_SCALE       [9]   RW 0 Audio signal input to AuxADC scale select signal
                                            0 =little scale       1 =large scale(4:1)
 * RG_AUD_HEDET_LDET_L_FILTER    [8]   RW 0 Audio headset_LINT low detect filter enable signal
                                            0 = no filter        1 = filter
 * RG_AUD_HEDET_BUF_CHOP         [7]   RW 0 Audio signal input to AuxADC buffer chop signal (1kHz)
 * RG_AUD_HEDET_MUX2ADC_SEL      [6:4] RW 0 Audio signal input to AuxADC select
                                            000 = HEADMIC_IN_DET
                                            001 =  HEADSET_L_INT
                                            010 = HP_L
                                            011 = HP_R            100 = AVDD_VB
                                            101 = VDDPA           110 = MICBIAS
                                            111 = HEADMIC_BIAS
 * RG_AUD_HEDET_V2I_SEL         [3:0] RW 0  Audio headset detect circuit current select signal
                                            0000 =0   0001 = 0.5u  0010 =1u  0011 = 1.5u
                                            0100 =2u  0101 = 2.5u  0110 =3u  0111 = 3.5u
                                            1000 =4u  1001 = 4.5u  1010 =5u  1011 = 5.5u
                                            1100 =6u  1101 = 6.5u  1110 =7u  1111 = 7.5u */
#define HEDET_MUX2ADC_SEL_P	(BIT_15)
#define HEDET_BUF_EN		(BIT_14)
#define HEDET_BDET_EN		(BIT_13)
#define HEDET_V21_EN		(BIT_12)
#define HEDET_VREF_EN		(BIT_11)
#define HEDET_MICDET_EN		(BIT_10)
#define HEDET_V2AD_SCALE	(BIT_9)
#define HEDET_LDET_L_FILTER	(BIT_8)
#define HEDET_BUF_CHOP		(BIT_7)

#define HEDET_MUX2ADC_SEL			(4)
#define HEDET_MUX2ADC_SEL_MASK		(MASK_3_BITS << HEDET_MUX2ADC_SEL)
#define HEDET_MUX2ADC_SEL_HEADMIC_IN_DETECT	(0)
#define HEDET_MUX2ADC_SEL_HEADSET_L_INT		(1)

#define HEDET_V21_SEL				(0)
#define HEDET_V21_SEL_MASK			(MASK_4_BITS << HEDET_V21_SEL)
#define HEDET_V21_SEL_1U			(0x2)

/* ANA_HDT1  0x34*/
#define HEDET_MICDET_REF_SEL		(13)
#define HEDET_MICDET_REF_SEL_MASK	(MASK_3_BITS << HEDET_MICDET_REF_SEL)
#define HEDET_MICDET_REF_SEL_2P6	(0x6)

#define HEDET_MICDET_HYS_SEL		(11)
#define HEDET_MICDET_HYS_SEL_MASK	(MASK_2_BITS << HEDET_MICDET_HYS_SEL)
#define HEDET_MICDET_HYS_SEL_20MV	(0x1)

#define HEDET_LDET_REFL_SEL		    (8)
#define HEDET_LDET_REFL_SEL_MASK	(MASK_3_BITS << HEDET_LDET_REFL_SEL)
#define HEDET_LDET_REFL_SEL_300MV	(0x6)
#define HEDET_LDET_REFL_SEL_50MV	(0x1)

#define HEDET_LDET_REFH_SEL		    (6)
#define HEDET_LDET_REFH_SEL_MASK	(MASK_2_BITS << HEDET_LDET_REFH_SEL)
#define HEDET_LDET_REFH_SEL_1P7		(0)
#define HEDET_LDET_REFH_SEL_1P9		(2)

#define HEDET_LDET_PU_PD		    (4)
#define HEDET_LDET_PU_PD_MASK		(MASK_2_BITS << HEDET_LDET_PU_PD)
#define HEDET_LDET_PU_PD_PU		    (0)

#define HEDET_LDET_L_HYS_SEL		(2)
#define HEDET_LDET_L_HYS_SEL_MASK	(MASK_2_BITS << HEDET_LDET_L_HYS_SEL)
#define HEDET_LDET_L_HYS_SEL_20MV	(0x1)

#define HEDET_LDET_H_HYS_SEL		(0)
#define HEDET_LDET_H_HYS_SEL_MASK	(MASK_2_BITS << HEDET_LDET_H_HYS_SEL)
#define HEDET_LDET_H_HYS_SEL_20MV	(0x1)


/* ANA_HDT2  0x38-> 0x534
RG_AUD_HEDET_BDET_REF_SEL [10:7]
Audio head microphone button pressed detect voltage select signal (VDDIO=2.8V)
0000 = 0.35V         0001 = 0.4V
0010 = 0.45V         0011 = 0.5V
0100 = 0.55V         0101 = 0.6V
0110 = 0.65V         0111 = 0.7V
1000 = 0.75V         1001 = 0.8V
1010 = 0.85V         1011 = 0.9V
1100 = 0.95V         1101 = 1V
1110/1111 = forbidden
*/
#define CHG_PROC_STS_BYPASS		(BIT_13)

#define HEDET_JACK_TYPE			(11)
#define HEDET_JACK_TYPE_MASK	(MASK_2_BITS << HEDET_JACK_TYPE)

#define HEDET_BDET_REF_SEL		(7)
#define HEDET_BDET_REF_SEL_MASK	(MASK_4_BITS << HEDET_BDET_REF_SEL)
#define HEDET_BDET_REF_SEL_850MV	(0xA)

#define HEDET_BDET_HYS_SEL		(5)
#define HEDET_BDET_HYS_SEL_MASK	(MASK_2_BITS << HEDET_BDET_HYS_SEL)
#define HEDET_BDET_HYS_SEL_20MV	(0x1)

#define PLGPD_EN			    (BIT_4)
#define HP_DRVIVER_EN			(BIT_3)
#define HPL_EN_D2HDT_EN			(BIT_2)

#define HPL_EN_D2HDT_T			(0)
#define HPL_EN_D2HDT_T_MASK		(MASK_2_BITS << HPL_EN_D2HDT_T)

/// ANA_DCL0 0x3C ///

/* RG_AUD_DCL_EN        [6]    RW  0   Audio digital control logic enable signal
 * RG_AUD_DCL_RST       [5]    RW  0x1 Audio digital control logic reset enable signal
 * RG_AUD_DRV_SOFT_T    [4:2]  RW  0x1 Audio DRV delay timer control signal
                                       000 = 0us          001 = 30us
                                       010 = 60us         011 = 90us
                                       100 = 120us        101 = 150us
                                       110 = 180us        111 = 210us
 * RG_AUD_DRV_SOFT_EN   [1]    RW  0   Audio DRV soft start enable signal
 * RG_AUD_DPOP_AUTO_RST [0]    RW  0   Soft reset dpop module . 0:disable , 1:enable
 */

#define DCL_EN		    (BIT_6)
#define DCL_RST		    (BIT_5)

#define DRV_SOFT_T	    (2)
#define DRV_SOFT_T_MASK	(MASK_3_BITS << DRV_SOFT_T)

#define DRV_SOFT_EN	    (BIT_1)
#define DPOP_AUTO_RST	(BIT_0)

/// ANA_DCL1 0x40 ///
/*
RG_AUD_PACAL_EN
RG_AUD_PACAL_DIV
RG_AUD_PA_OVP_ABMOD_PD
RG_AUD_PA_OVP_ABMOD_T
RG_AUD_PA_OVP_DEG_EN
RG_AUD_PA_OVP_DEG_T
RG_AUD_PACAL_EN        [10]   RW  0   "Audio PA calibration clock input enable
                                        signal0 = disable          1 = enable"
RG_AUD_PACAL_DIV       [9:8]  RW  0   "Audio PA PWM clock divider select signal
                                        00 = 1/128          01 = 1/6410 = 1/256          11 = 1/1"
RG_AUD_PA_OVP_ABMOD_PD [7]    RW  0   "Audio VBAT_PA over voltage protection circuit  mode change signal
                                        0 = enable Class-AB mode         1 = keep the previous mode"
RG_AUD_PA_OVP_ABMOD_T  [6:4]  RW  0   "Audio PA over current protection circuit mute timer control signal
                                        000 = 0ms   001 = 1ms010 = 4ms    011 = 16ms100 = 64ms
                                        101 = 256ms110 = 1s        111 = 4s"
RG_AUD_PA_OVP_DEG_EN   [3]    RW  0   "Audio VBAT_PA over voltage protection circuit  alert deglitch enable signal
                                        0 = disable          1 = enable"
RG_AUD_PA_OVP_DEG_T    [9:8]  RW  0   "Audio VBAT_PA over voltage protection circuit  alert deglitch timer control signal
                                        000 = 0ms  001 = 0.06ms 010 = 0.24ms 011 = 1ms100 = 4ms              101 = 16ms110 = 64ms             111 = 256ms"
*/
#define PACAL_EN		    (BIT_10)

#define PACAL_DIV		    (8)
#define PACAL_DIV_MASK		(MASK_2_BITS << PACAL_DIV)

#define PA_OVP_ABMOD_PD		(BIT_7)

#define PA_OVP_ABMOD_T		(4)
#define PA_OVP_ABMOD_T_MASK	(MASK_2_BITS << PA_OVP_ABMOD_T)

#define PA_OVP_DEG_EN		(BIT_3)

#define PA_OVP_DEG_T		(0)
#define PA_OVP_DEG_T_MASK	(MASK_3_BITS << PA_OVP_DEG_T)

/// ANA_DCL2 0x44 ///

/* RG_AUD_PA_OTP_DEG_EN  [12]   RW 0  Audio PA over temperature protection circuit
                                      alert deglitch enable signal
 * RG_AUD_PA_OTP_DEG_T   [11:9] RW 0  Audio PA over temperature protection circuit
                                      alert deglitch timer control signal
                                      000 = 0ms             001 = 0.06ms
                                      010 = 0.24ms          011 = 1ms
                                      100 = 4ms             101 = 16ms
                                      110 = 64ms            111 = 256ms
 * RG_AUD_PA_OTP_MUTE_EN [8]    RW 0  Audio PA over temperature protection circuit
                                      mute enable signal
 * RG_AUD_PA_OCP_DEG_EN  [7]    RW 0  Audio PA over current protection circuit
                                      alert deglitch enable signal
 * RG_AUD_PA_OCP_DEG_T   [6:4]  RW 0  Audio PA over current protection circuit
                                      alert deglitch timer control signal
                                      000 = 0ms             001 = 0.06ms
                                      010 = 0.24ms          011 = 1ms
                                      100 = 4ms             101 = 16ms
                                      110 = 64ms            111 = 256ms
 * RG_AUD_PA_OCP_MUTE_EN [3]    RW 0  Audio PA over current protection circuit mute power down signal
                                      0 = enable mute        1 = disable mute
 * RG_AUD_PA_OCP_MUTE_T  [2:0]  RW 0  Audio PA over current protection circuit mute timer control signal
                                      000 = 0ms             001 = 1ms
                                      010 = 4ms             011 = 16ms
                                      100 = 64ms            101 = 256ms
                                      110 = 1s              111 = 4s */
#define PA_OTP_DEG_EN       (BIT_12)
#define PA_OTP_DEG_T        (9)
#define PA_OTP_DEG_T_MASK   (MASK_3_BITS << PA_OTP_DEG_T)
#define PA_OTP_MUTE_EN      (BIT_8)
#define PA_OCP_DEG_EN       (BIT_7)
#define PA_OCP_DEG_T        (4)
#define PA_OCP_DEG_T_MASK   (MASK_3_BITS << PA_OCP_DEG_T)
#define PA_OCP_MUTE_EN      (BIT_3)
#define PA_OCP_MUTE_T       (0)
#define PA_OCP_MUTE_T_MASK  (MASK_3_BITS << PA_OCP_MUTE_T)

/// ANA_DCL3 0x48 ///

/// ANA_DCL4 0x4c ///
/* RG_HPL_DEPOP_CHG_CURSEL  [15:8] RW 0xFF  HPL depop DAC current setting
 * RG_HPR_DEPOP_CHG_CURSEL  [7:0]  RW 0xFF  HPR depop DAC current setting */
#define HPL_DEPOP_CHG_CURSEL        (8)
#define HPL_DEPOP_CHG_CURSEL_MASK   (MASK_8_BITS << HPL_DEPOP_CHG_CURSEL)
#define HPR_DEPOP_CHG_CURSEL        (0)
#define HPR_DEPOP_CHG_CURSEL_MASK   (MASK_8_BITS << HPR_DEPOP_CHG_CURSEL)

/// ANA_DCL5 0x50 ///
/* RG_CALDC_WAIT_T         [15]    RW 1     always on, value = 1
 * RG_AUD_HPL_RDAC_STS     [14]    RW 1     always on, value = 1
 * RG_AUD_HPL_DPOP_CLKN1   [13]    RW 1     Audio HP de-pop fade in function enable signal
                                            0 = disable              1 = enable
 * RG_AUD_HPL_DPOP_N1      [12]    RW 1     Audio HP de-pop fade out function enable signal
                                            0 = disable              1 = enable
 * RG_AUD_HPL_DPOP_VAL1    [11:9]  RW 4     Audio HP de-pop gain step (RG_AUD_HP_DPOP_RES_PD=0)
                                            000 = 1             001 = 2
                                            010 = 4             011 = 8
                                            100 = 16            101 = 32
                                            110 = 64            111 = 128
 * RG_AUD_HPL_DPOP_CLKN2   [8:6]   RW 0     Audio HP de-pop gain step (RG_AUD_HP_DPOP_RES_PD=0)
                                            000 = 1              001 = 2
                                            010 = 3              011 = 4
                                            100 = 5              101 = 6
                                            110 = 7              111 = 8
 * RG_AUD_HPL_DPOP_N2      [5:3]   RW 0     Audio HP de-pop gain time step (RG_AUD_HP_DPOP_RES_PD=0)
                                            000 = 30us           001 = 60us
                                            010 = 120us         011 = 250us
                                            100 = 500us         101 = 1ms
                                            110 = 2ms           111 = 4ms
 * RG_AUD_HPL_RDAC_STS     [2]     RW 0     Audio HPL_RDAC status signal
                                            0 = unfinish/have never done       1 = finish
 * RG_AUD_HPR_RDAC_STS     [1]     RW 0     Audio HPR_RDAC status signal
                                            0 = unfinish/have never done       1 = finish
 * Reserved                [0]  */

#define HPL_RDAC_START			(BIT_15)
#define HPR_RDAC_START			(BIT_14)
#define HP_DPOP_FDIN_EN			(BIT_13)
#define HP_DPOP_FDOUT_EN		(BIT_12)

#define HP_DPOP_GAIN_N1			(9)
#define HP_DPOP_GAIN_N1_MASK	(MASK_3_BITS << HP_DPOP_GAIN_N1)

#define HP_DPOP_GAIN_N2			(6)
#define HP_DPOP_GAIN_N2_MASK	(MASK_3_BITS << HP_DPOP_GAIN_N2)

#define HP_DPOP_GAIN_T			(3)
#define HP_DPOP_GAIN_T_MASK		(MASK_3_BITS << HP_DPOP_GAIN_T)

#define HPL_RDAC_STS			(BIT_2)
#define HPR_RDAC_STS			(BIT_1)

/// ANA_DCL6 0x54 ///
/* RG_CALDC_WAIT_T          [14:12] RW 0    Audio dc-calibration waiting time, every data change
                                            000 = 2Tclk    001 = 3Tclk    010 = 4Tclk    011 = 5Tclk
                                            100 = 6Tclk    101 = 7Tclk    110 = 8Tclk    111 = 9Tclk
 * RG_AUD_HPL_DPOP_CLKN1    [11:10] RW 0    Audio DePOP HPL DAC clock(start-up)
                                            00 = 1Tclk    01 = 2Tclk    10 = 4Tclk    11 = 8Tclk
 * RG_AUD_HPL_DPOP_N1       [9:8]   RW 0    Audio DePOP HPL DAC data increase step(start-up)
                                            00 = +1    01 = +2    10 = +4    11 = +8
 * RG_AUD_HPL_DPOP_VAL1     [7:5]   RW 0    Audio DePOP HPL DAC data final value(start-up)
                                            000 = 2   001 = 4   010 = 8   011 = 16
                                            100 = 32  101 = 64  110 = 128 111 = 256
 * RG_AUD_HPL_DPOP_CLKN2    [4:3]   RW 0    Audio DePOP HPL DAC clock(rising/falling)
                                            00 = 1Tclk    01 = 2Tclk    10 = 4Tclk    11 = 8Tclk
 * RG_AUD_HPL_DPOP_CLKN2    [2:1]   RW 0    Audio DePOP HPL DAC data increase step(rising/falling)
                                            00 = +1    01 = +2    10 = +4    11 = +8
 */
#define CALDC_WAIT_T		(12)
#define CALDC_WAIT_T_MASK	(MASK_3_BITS << CALDC_WAIT_T)

#define HPL_DPOP_CLKN1		(10)
#define HPL_DPOP_CLKN1_MASK	(MASK_2_BITS << HPL_DPOP_CLKN1)

#define HPL_DPOP_N1		    (8)
#define HPL_DPOP_N1_MASK	(MASK_2_BITS << HPL_DPOP_N1)

#define HPL_DPOP_VAL1		(5)
#define HPL_DPOP_VAL1_MASK	(MASK_3_BITS << HPL_DPOP_VAL1)

#define HPL_DPOP_CLKN2		(3)
#define HPL_DPOP_CLKN2_MASK	(MASK_2_BITS << HPL_DPOP_CLKN2)

#define HPL_DPOP_N2		    (1)
#define HPL_DPOP_N2_MASK	(MASK_2_BITS << HPL_DPOP_N2)


/// ANA_DCL7 0x58 ///
/* RG_DEPOPL_PCUR_OPT     [14:13] RW 1  depop_hpl_current_sel
                                        00: X2   01:X1   10:X2/3  11:X1/2
 * RG_DEPOPR_PCUR_OPT     [12:11] RW 1  depop_hpr_current_sel
                                        00: X2   01:X1   10:X2/3  11:X1/2
 * RG_AUD_HPR_DPOP_CLKN1  [10:9]  RW 0  Audio DePOP HPR DAC clock(start-up)
                                        00 = 1Tclk    01 = 2Tclk    10 = 4Tclk    11 = 8Tclk
 * RG_AUD_HPR_DPOP_N1     [8:7]   RW 0  Audio DePOP HPR DAC data increase step(start-up)
                                        00 = +1    01 = +2    10 = +4    11 = +8
 * RG_AUD_HPR_DPOP_VAL1   [6:4]   RW 6  Audio DePOP HPR DAC data final value(start-up)
                                        000 = 2   001 = 4   010 = 8   011 = 16
                                        100 = 32  101 = 64  110 = 128 111 = 256
 * RG_AUD_HPR_DPOP_CLKN2  [3:2]   RW 0  Audio DePOP HPR DAC clock(rising/falling)
                                        00 = 1Tclk    01 = 2Tclk    10 = 4Tclk    11 = 8Tclk
 * RG_AUD_HPR_DPOP_N2     [1:0]   RW 0  Audio DePOP HPR DAC data increase step(rising/falling)
                                        00 = +1    01 = +2    10 = +4    11 = +8 */

#define DEPOPL_PCUR_OPT		    (13)
#define DEPOPL_PCUR_OPT_MASK	(MASK_2_BITS << DEPOPL_PCUR_OPT)

#define DEPOPR_PCUR_OPT		    (11)
#define DEPOPR_PCUR_OPT_MASK	(MASK_2_BITS << DEPOPR_PCUR_OPT)

#define HPR_DPOP_CLKN1		(9)
#define HPR_DPOP_CLKN1_MASK	(MASK_2_BITS << HPR_DPOP_CLKN1)

#define HPR_DPOP_N1		    (7)
#define HPR_DPOP_N1_MASK	(MASK_2_BITS << HPR_DPOP_N1)

#define HPR_DPOP_VAL1		(4)
#define HPR_DPOP_VAL1_MASK	(MASK_3_BITS << HPR_DPOP_VAL1)

#define HPR_DPOP_CLKN2		(2)
#define HPR_DPOP_CLKN2_MASK	(MASK_2_BITS << HPR_DPOP_CLKN2)

#define HPR_DPOP_N2		    (0)
#define HPR_DPOP_N2_MASK	(MASK_2_BITS << HPR_DPOP_N2)

/// ANA_STS0 0x5c ///

/* RG_HP_DEPOP_WAIT_T1 [15:13] RW 0x4 depop_runing time
                                      000: 10ms   001: 20ms   010: 40ms  011: 80ms
                                      100: 160ms  101: 320ms  110: 640ms 111: 1280ms
 * RG_HP_DEPOP_WAIT_T2 [12:10] RW 0x4 depop_finish waiting time
                                      000: 10ms   001: 20ms   010: 40ms
                                      011: 80ms   100: 160ms  101: 320ms
                                      110: 640ms  111: 1280ms
 * RG_HP_DEPOP_WAIT_T3 [9:8]   RW 0x2 CHG_EN_Delay time
                                      00:  1Tclk  01: 2Tclk  10: 4Tclk  11: 8Tclk
 * RG_HP_DEPOP_WAIT_T4 [7:6]   RW 0x2 depop path on delay time
                                      00:  1Tclk   01: 2Tclk  10: 4Tclk  11: 8Tclk
 * RG_DC_CALI_IDACVAL  [5:3]   RW 0x4 DCCALI_IDAC_repeat_goal
                                      000: 8  001:9   010:10  011:11
                                      100:12  101:13  110:14  111:7
 * DC_CALI_IDAC_CURSEL [2:1]   RW 0   IDAC LSB SETTING:
                                      00: 10nA  01:15nA  10:5nA  11:10nA
 * RG_DCCALI_RDACI_ADJ [0]     RW 0   RDAC current enhancement
                                      0 = X1       1 = X2 */
#define HP_DEPOP_WAIT_T1        (13)
#define HP_DEPOP_WAIT_T1_MASK   (MASK_3_BITS << HP_DEPOP_WAIT_T1)
#define HP_DEPOP_WAIT_T2        (10)
#define HP_DEPOP_WAIT_T2_MASK   (MASK_3_BITS << HP_DEPOP_WAIT_T2)
#define HP_DEPOP_WAIT_T3        (8)
#define HP_DEPOP_WAIT_T3_MASK   (MASK_2_BITS << HP_DEPOP_WAIT_T3)
#define HP_DEPOP_WAIT_T4        (6)
#define HP_DEPOP_WAIT_T4_MASK   (MASK_2_BITS << HP_DEPOP_WAIT_T4)
#define DC_CALI_IDACVAL         (3)
#define DC_CALI_IDACVAL_MASK    (MASK_3_BITS << DC_CALI_IDACVAL)
#define DC_CALI_IDAC_CURSEL     (1)
#define DC_CALI_IDAC_CURSEL_MASK (MASK_2_BITS << DC_CALI_IDAC_CURSEL)
#define DCCALI_RDACI_ADJ        (BIT_0)

/// ANA_STS1 0x60 ///

/// ANA_STS2 0x64 ///
/* RG_CALDC_START       [15] RW  0  DC-calibraion start signal
                                    0 ---> 1 start calibration
 * RG_CALDC_EN          [14] RW  0  DC-calibraion enable signal (digital)
                                    0: disable      1: enable
 * RG_CALDC_ENO         [13] RW  0  DC-calibraion enable signal (analog)
                                    0: disable      1: enable
 * RG_AUD_DCCAL_STS     [12] RO  0  Audio DC-calibration status signal
                                    0 = unfinish/have never done       1 = finish
 * RG_DCCALI_STS_BYPASS [11] RW  0  DCCALI_STS_BYPASS=0, not bypass DCCALI_process
                                    DCCALI_STS_BYPASS=1, bypass DCCALI_process
 * RG_AUD_HP_DPOP_DVLD  [10] RO  0  Audio DC-calibration finish insert signal
                                    0 = unfinish        1 = finish
 * RG_DEPOP_CHG_START   [9]  RW  0  depop start signal
                                    0 ---> 1 start calibration
 * RG_DEPOP_CHG_EN      [8]  RW  0  depop charge en
                                    0: disable  1:enable
 * RG_AUD_PLUGIN        [7]  RW  0  plug_in=1, headphone has been inserted
 * RG_DEPOP_EN          [6]  RW  0  depop_ana_en
                                    0: disable  1:enable
 * RG_DEPOP_CHG_STS     [5]  RO  0  Audio plug-in depop status signal
                                    0 = depop not finish        1 = depop finish
 * RG_AUD_RCV_DPOP_DVLD [4]  RO  0  Audio plug-in depop charge finish insert signal
                                    0 = unfinish        1 = finish
 * RG_HPL_PU_ENB        [3]  RW  0  HPL_pull_up enable
                                    0: pull up enable   1: pull up disable
 * RG_HPR_PU_ENB        [2]  RW  0  HPR_pull_up enable
                                    0: pull up enable   1: pull up disable
 * RG_INSBUF_EN         [1]  RW  0  INSBUF_EN
                                    0: disable   1:enable  */

#define CALDC_START		    (BIT_15)
#define CALDC_EN		    (BIT_14)
#define CALDC_ENO		    (BIT_13)
#define DCCAL_STS		    (BIT_12)
#define DCCALI_STS_BYPASS	(BIT_11)
#define HP_DPOP_DVLD		(BIT_10)
#define DEPOP_CHG_START		(BIT_9)
#define DEPOP_CHG_EN		(BIT_8)
#define PLUGIN			    (BIT_7)
#define DEPOP_EN		    (BIT_6)
#define DEPOP_CHG_STS		(BIT_5)
#define RCV_DPOP_DVLD		(BIT_4)
#define HPL_PU_ENB		    (BIT_3)
#define HPR_PU_ENB	    	(BIT_2)
#define INSBUF_EN	    	(BIT_1)


/// ANA_STS3 0x68 ///

/* RG_DEPOP_BIAS_SEL [9: 8] RW 0    AUD_DRV_DEPOP_BIAS_CURRENT SEL
                                    00: 1.25uA   01:2.5uA   10:3.75uA   11:5uA
 * RG_DEPOP_OPA_SEL  [7: 6] RW 0    AUD_DRV_DEPOP_OPA_CURRENT SEL
                                    00: LS_I=5uA,  LS_R=100K
                                    01: LS_I=10uA, LS_R=100K
                                    10: LS_I=5uA,  LS_R=50K
                                    11: LS_I=10uA, LS_R=50K
 * RG_AUD_HWSW_SEL   [5: 0] RW 0    Hardware control/software control sel
                                    <3>: 0: depend on analog comp value
                                            1: bypass analog comp value
                                    <2>: 0: RG_AUD_VCMI_SEL change, repeat dccali
                                            1: change RG_AUD_VCMI_SEL, get two dccali value, choose any according RG_AUD_VCMI_SEL =0/=1
                                    <1>: 0: hw control  DC_CALI_IDAC_CURSEL,  1: sw control DC_CALI_IDAC_CURSEL
                                    <0>: 0: hw control  RG_HPL_PU_ENB, RG_HPR_PU_ENB, RG_INSBUF_EN
                                             1:  sw control  RG_HPL_PU_ENB, RG_HPR_PU_ENB, RG_INSBUF_EN */

#define DEPOP_BIAS_SEL		    (8)
#define DEPOP_BIAS_SEL_MASK	    (MASK_2_BITS << DEPOP_BIAS_SEL)
#define DEPOP_BIAS_SEL_2P5UA	(0X1)

#define DEPOP_OPA_SEL		    (6)
#define DEPOP_OPA_SEL_MASK	    (MASK_2_BITS << DEPOP_OPA_SEL)

#define HWSW_SEL		        (0)
#define HWSW_SEL_MASK		    (0x3F << HWSW_SEL)
#define HWSW_SEL_0		        (0)
/// ANA_STS4 0x6C ///
/* HPL_DCCAL_RDACL      [15: 8] RO 0   HPR_DCCALI_RDAC_VALUE
 * HPR_DCCAL_RDACL      [7: 4]  RO 0   HPR_DCCALI_RDAC_VALUE
 * HPL_DCCAL_IDACL_SEL  [3: 2]  RO 0   HPL_DCCALI_IDAC_path
 * HPR_DCCAL_IDACL_SEL  [1: 0]  RO 0   HPR_DCCALI_IDAC_path */


/// ANA_STS5 0x70 ///

/// ANA_STS6 0x74 ///
/* RG_AUD_PACAL_DVLD    [2]    RO  0    Audio PA clock calibration data valid signal
                                        0 = not valid         1 = data valid*/
#define PACAL_DVLD		(BIT_2)

/// ANA_STS7 0x78 ///
/* RG_AUD_HEAD_INSERT_ALL   [12]    RO  0   Audio headset insert alert signal (need software anti-dither)
                                            0 = normal          1 = plug in
 * RG_AUD_HEAD_INSERT3      [11]    RO  0   Audio headset-H insert alert signal (need software anti-dither)
                                            0 = normal          1 = plug in
 * RG_AUD_HEAD_INSERT2      [10]    RO  0   Audio headset-L insert alert signal (need software anti-dither)
                                            0 = normal          1 = plug in
 * RG_AUD_HEAD_INSERT       [9]     RO  0   Audio headset microphone insert alert signal (need software anti-dither)
                                            0 = normal          1 = plug in
 * RG_AUD_HEAD_BUTTON       [8]     RO  0   Audio headset microphone button press alert signal (need software anti-dither)
                                            0 = normal          1 = button press
 * RG_AUD_PA_SH_FLAG        [7]     RO  0   Audio PA output short to VBAT detect ALERT signal
                                            0 = normal          1 = short
 * RG_AUD_PA_SL_FLAG        [6]     RO  0   Audio PA output short to GND detect ALERT signal
                                            0 = normal          1 = short
 * RG_AUD_PA_OVP_FLAG       [5]     RO  0   Audio PA over voltage protection circuit alert signal
                                            0 = normal          1 = over temperature
 * RG_AUD_PA_OTP_FLAG       [4]     RO  0   Audio PA over temperature protection circuit alert signal
                                            0 = normal          1 = over temperature
 * RG_AUD_DRV_OCP_FLAG      [3:0]   RO  0   Audio Driver over current protection circuit alert signal
                                            <3:2> for SPK       <1:0> for Headphone/Earpiece */
#define HEAD_INSERT_ALL		(BIT_12)
#define HEAD_INSERT3		(BIT_11)
#define HEAD_INSERT2		(BIT_10)
#define HEAD_INSERT		    (BIT_9)
#define HEAD_BUTTON		    (BIT_8)
#define PA_SH_FLAG		    (BIT_7)
#define PA_SL_FLAG		    (BIT_6)
#define PA_OVP_FLAG		    (BIT_5)
#define PA_OTP_FLAG		    (BIT_4)
#define DRV_OCP_FLAG_SPK	(BIT_2)
#define DRV_OCP_FLAG_HPRCV	(0)
#define DRV_OCP_FLAG_MASK	(MASK_2_BITS << DRV_OCP_FLAG_HPRCV)

/// ANA_CLK1 0x7C ///
/* RG_AUD_DCDCGEN_CLK_F   [15: 14]  RW 0    DCDC GEN/MEM clock frequency select (based on Fclk=6.5MHz)
                                            00 = Fclk/4              01 = Fclk / 3
                                            10/11 = Fclk / 2
 * RG_AUD_DCDCCORE_CLK_F  [13: 12]  RW 0    DCDC CORE clock frequency select (based on Fclk=6.5MHz)
                                            00 = Fclk/4              01 = Fclk / 3
                                            10/11 = Fclk / 2
 * RG_AUD_DCDCCHG_CLK_F   [11: 10]  RW 0    DCDC CHG clock frequency select (based on Fclk=6.5MHz)
                                            00 = Fclk/4              01 = Fclk / 3
                                            10/11 = Fclk / 2
 * RG_AUD_PA_CLK_F        [9: 8]    RW 0    Audio PA clock frequency select (based on ADC Clock)
                                            00 = 1/2                  01 = 1/4
                                            10 = 1/8                  11 = 1/16
 * RG_AUD_CLK_PN_SEL      [7: 0]    RW 4    Audio clock PN select (If RG_AUD_AD_CLK_F[1:0]=00 or 10 & RG_AUD_DA_CLK_F[1:0]=00, RG_AUD_CLK_PN_SEL)
                                            <0> CLK_AUD_DIG_LOOP       <1> CLK_AUD_DIG_6P5M
                                            <2> CLK_AUD_DAC                 <3> CLK_AUD_ADC
                                            <4> CLK_AUD_DCDCGEN       <5> CLK_AUD_VAD_CLK_SEL
                                            <6> CLK_AUD_DCDCCORE     <7> RG_AUD_VB_V<5> */

#define DCDCGEN_CLK_F		(14)
#define DCDCGEN_CLK_F_MASK	(MASK_2_BITS << DCDCGEN_CLK_F)

#define DCDCCORE_CLK_F		(12)
#define DCDCCORE_CLK_F_MASK	(MASK_2_BITS << DCDCCORE_CLK_F)

#define DCDCCHG_CLK_F		(10)
#define DCDCCHG_CLK_F_MASK	(MASK_2_BITS << DCDCCHG_CLK_F)

#define PA_CLK_F		    (8)
#define PA_CLK_F_MASK		(MASK_2_BITS << PA_CLK_F)

#define CLK_PN_SEL		    (0)
#define CLK_PN_SEL_MASK		(MASK_8_BITS << CLK_PN_SEL)

/**---------------------------------------------------------------------------*
 **                         register mapping definition                       *
 **---------------------------------------------------------------------------*/

#define SC2720_AP_REG_BASE  (0x00000700)

typedef struct
{
	uint32  ana_pmu0;           //0x00
	uint32  ana_pmu1;           //0x04
	uint32  ana_pmu2;           //0x08
	uint32  ana_pmu3;           //0x0c
	uint32  ana_pmu4;           //0x10
	uint32  ana_pmu5;           //0x14

	uint32  ana_clk0;           //0x18

	uint32  ana_cdc0;           //0x1c
	uint32  ana_cdc1;           //0x20
	uint32  ana_cdc2;           //0x24
	uint32  ana_cdc3;           //0x28
	uint32  ana_cdc4;           //0x2c

	uint32  ana_hdt0;           //0x30
	uint32  ana_hdt1;           //0x34
	uint32  ana_hdt2;           //0x38

	uint32  ana_dcl0;           //0x3c
	uint32  ana_dcl1;           //0x40
	uint32  ana_dcl2;           //0x44
	uint32  ana_dcl3;           //0x48
	uint32  ana_dcl4;           //0x4c
	uint32  ana_dcl5;           //0x50
	uint32  ana_dcl6;           //0x54
	uint32  ana_dcl7;           //0x58

	uint32  ana_sts0;           //0x5C
	uint32  ana_sts1;           //0x60
	uint32  ana_sts2;           //0x64
	uint32  ana_sts3;           //0x68
	uint32  ana_sts4;           //0x6C
	uint32  ana_sts5;           //0x70
	uint32  ana_sts6;           //0x74
	uint32  ana_sts7;           //0x78

	uint32  ana_clk1;           //0x7C
} SPRD_CODEC_AP_REG_CTL_T;

/**---------------------------------------------------------------------------*
 **                       A Die codec digital part                            *
 **                         Register description                              *
 **---------------------------------------------------------------------------*/

/// AUD_CFGA_CLR 0x00 ///
/// aud_cfgal clr
/* ear_shutdown_clr     [10]    WC	0	ear shut down clear
 * hp_shutdown_clr      [9]     WC	0	hp shut down clear
 * pa_shutdown_clr      [8]     WC	0	pa shut down clear
 * aud_int_clr	        [7:0]	WC	0	[7:0]={audio_rcv_depop,audio_pacal_irq,audio_hp_dpop_irq,
                                        ovp_irq,otp_irq,pa_ocp_irq,ear_ocp_irq,hp_ocp_irq}*/
#define EAR_SHUTDOWN_CLR                (BIT_10)
#define HP_SHUTDOWN_CLR                 (BIT_9)
#define PA_SHUTDOWN_CLR                 (BIT_8)
#define AUD_INT_CLR_MASK			    (MASK_8_BITS)
#define AUD_INT_CLR_ALL                 (AUD_INT_CLR_MASK)
#define AUD_INT_CLR_AUDIO_RCV_DPOP_IRQ  (BIT_7)
#define AUD_INT_CLR_AUDIO_PACAL_IRQ     (BIT_6)
#define AUD_INT_CLR_AUDIO_HP_DPOP_IRQ   (BIT_5)
#define AUD_INT_CLR_OVP_IRQ             (BIT_4)
#define AUD_INT_CLR_OTP_IRQ             (BIT_3)
#define AUD_INT_CLR_PA_OCP_IRQ          (BIT_2)
#define AUD_INT_CLR_EAR_OCP_IRQ         (BIT_1)
#define AUD_INT_CLR_HP_OCP_IRQ          (BIT_0)


/// AUD_CFGA_HID_CFG0 0x04 ///
/// audio head insert detect status
/* UD_DBNC_EN           [2: 1]  RW  0   [0]: audio HEAD_INSERT debounce enable
                                        [1]: audio HEAD_BUTTON_OUT debounce enable
 * HID_EN               [0]     RW  0 */
#define AUD_HEAD_BUTTON_OUT_DEBOUNCE_EN   (BIT_2)
#define AUD_HEAD_INSERT_DEBOUNCE_EN       (BIT_1)
#define HID_EN                            (BIT_0)

// TODO: low or high ??
/// AUD_CFGA_HID_CFG1 0x08 ///
/// audio head insert detect status
/* HID_HIGH_DBNC_THD0	[15: 8]	RW	0	low debounce threshold(clock 1k), for HEAD_INSERT signal
 * HID_LOW_DBNC_THD0	[7: 0]	RW	0	high debounce threshold(clock 1k), for HEAD_INSERT signal */
#define HID_HIGH_DBNC_THD0       (8)
#define HID_HIGH_DBNC_THD0_MASK  (MASK_8_BITS << HID_HIGH_DBNC_THD0)
#define HID_LOW_DBNC_THD0        (0)
#define HID_LOW_DBNC_THD0_MASK   (MASK_8_BITS << HID_LOW_DBNC_THD0)

/// AUD_CFGA_HID_CFG2 0x0c ///
/// audio head insert detect status
/* HID_TMR_T1T2_STEP	[9: 5]	RW	0	head insert detect T1/T2 timer step (clock 1k)
 * HID_TMR_T0	        [4: 0]	RW	0	head insert detect T0 timer , (clock 1k) */
#define HID_TMR_T1T2_STEP       (5)
#define HID_TMR_T1T2_STEP_MASK  (MASK_5_BITS << HID_TMR_T1T2_STEP)
#define HID_TMR_T0              (0)
#define HID_TMR_T0_MASK         (MASK_5_BITS << HID_TMR_T0)

/// AUD_CFGA_HID_CFG3 0x10 ///
/// audio head insert detect status
/* HID_TMR_T1           [15: 0]	RW	0	head insert detect T1 timer ,step: HID_TMR_T1T2_STEP */
#define HID_TMR_T1          (0)
#define HID_TMR_T1_MASK     (MASK_16_BITS << HID_TMR_T1)

/// AUD_CFGA_HID_CFG4 0x14 ///
/// audio head insert detect status
/* HID_TMR_T2           [15: 0]	RW	0	head insert detect T2 timer ,step: HID_TMR_T1T2_STEP */
#define HID_TMR_T2          (0)
#define HID_TMR_T2_MASK     (MASK_16_BITS << HID_TMR_T2)

/// AUD_CFGA_HID_CFG5 0x18 ///
/// audio head insert detect status
/* HID_HIGH_DBNC_THD1	[15: 8]	RW	0	low debounce threshold(clock 1k), for HEAD_BUTTON_OUT signal
 * HID_LOW_DBNC_THD1	[7: 0]	RW	0	high debounce threshold(clock 1k), for HEAD_BUTTON_OUT signal */
#define HID_HIGH_DBNC_THD1              (8)
#define HID_HIGH_DBNC_THD1_MASK         (MASK_8_BITS << HID_HIGH_DBNC_THD1)
#define HID_LOW_DBNC_THD1               (0)
#define HID_LOW_DBNC_THD1_MASK          (MASK_8_BITS << HID_LOW_DBNC_THD1)

/// AUD_CFGA_HID_STS0 0x1c ///
/// audio head insert detect status
/* EAR_SHUTDOWN	            [10]	RO	0	ear_shutdown
 * HP_SHUTDOWN	            [9]	    RO	0	hp_shutdown
 * PA_SHUTDOWN	            [8]	    RO	0	pa_shutdown
 * AUDIO_HEAD_INSERT_OUT	[7]	    RO	0	head insert detect out: AUDIO_HEAD_INSERT_OUT state
 * AUDIO_HEAD_BUTTON_OUT	[6]	    RO	0	head button detect out: AUDIO_HEAD_BUTTON_OUT state
 * AUD_DBNC_STS1	        [5: 3]	RO	0	u1 debounce state machine  status
 * AUD_DBNC_STS0	        [2: 0]	RO	0	u0 debounce state machine  status */
#define EAR_SHUTDOWN            (BIT_10)
#define HP_SHUTDOWN             (BIT_9)
#define PA_SHUTDOWN             (BIT_8)
#define AUDIO_HEAD_INSERT_OUT   (BIT_7)
#define AUDIO_HEAD_BUTTON_OUT   (BIT_6)
#define AUD_DBNC_STS1           (3)
#define AUD_DBNC_STS1_MASK      (MASK_3_BITS << AUD_DBNC_STS1)
#define AUD_DBNC_STS0           (0)
#define AUD_DBNC_STS0_MASK      (MASK_3_BITS << AUD_DBNC_STS0)

/// AUD_CFGA_PRT_CFG_0 0x20 ///
/// aud protect module configuration
/* EAR_SHUTDOWN_EN      [9]     RW	0	ear_shutdown_enable
 * HP_SHUTDOWN_EN       [8]     RW	0	hp_shutdown_enable
 * PA_SHUTDOWN_EN2      [7]     RW	0	pa_shutdown_enable2
 * PA_SHUTDOWN_EN1      [6]     RW	0	pa_shutdown_enable1
 * PA_SHUTDOWN_EN0	    [5]     RW	0	pa_shutdown_enable0
 * AUD_CLK_SEL	        [4]     RW	0x1	1:32k_clk 0:1k_clk
 * AUD_PRT_EN	        [3]     RW	0	protect enable
 * OTP_PD_THD	        [2:0]	RW	0x3	over-temperature protection threshold */
#define EAR_SHUTDOWN_EN	    (BIT_9)
#define HP_SHUTDOWN_EN	    (BIT_8)
#define PA_SHUTDOWN_EN2	    (BIT_7)
#define PA_SHUTDOWN_EN1	    (BIT_6)
#define PA_SHUTDOWN_EN0	    (BIT_5)
#define AUD_CLK_SEL	        (BIT_4)
#define AUD_PRT_EN	        (BIT_3)
#define OTP_PD_THD	        (0)
#define OTP_PD_THD_MASK	    (MASK_3_BITS << OTP_PD_THD)

/// AUD_CFGA_PRT_CFG_1 0x24 ///
/// aud protect module configuration
/* OTP_PRECIS	[14: 12]	RW	0x2	over-temperature protection precis
 * OVP_PD_THD	[11: 9]     RW	0x2	overvoltage protection threshold
 * OVP_PRECIS	[8: 6]      RW	0	overvoltage protection precis
 * OCP_PD_THD	[5: 3]      RW	0x3	overcurrent protection threshold
 * OCP_PRECIS	[2: 0]      RW	0x2	overcurrent protection precis */
#define OTP_PRECIS          (12)
#define OTP_PRECIS_MASK     (MASK_3_BITS << OTP_PRECIS)
#define OVP_PD_THD          (9)
#define OVP_PD_THD_MASK     (MASK_3_BITS << OVP_PD_THD)
#define OVP_PRECIS          (6)
#define OVP_PRECIS_MASK     (MASK_3_BITS << OVP_PRECIS)
#define OCP_PD_THD          (3)
#define OCP_PD_THD_MASK     (MASK_3_BITS << OCP_PD_THD)
#define OCP_PRECIS          (0)
#define OCP_PRECIS_MASK     (MASK_3_BITS << OCP_PRECIS)

/// AUD_CFGA_RD_STS 0x28 ///
/// aud read states0&1
/* aud_irq_raw	[15: 8]	RO	0	int status: [7:0]={audio_rcv_depop,audio_pacal_irq,
                                audio_hp_dpop_irq,ovp_irq,otp_irq,pa_ocp_irq,ear_ocp_irq,hp_ocp_irq}
 * aud_irq_msk	[7: 0]	RO	0	int mask = aud_irq_raw & aud_int_en */
#define AUDIO_IRQ_RAW                  (8)
#define AUDIO_IRQ_RAW_MASK             (MASK_8_BITS << AUDIO_IRQ_RAW)
#define AUDIO_RCV_DEPOP_IRQ_RAW_STATUS (BIT_15)
#define AUDIO_PACAL_IRQ_RAW_STATUS     (BIT_14) // TODO: Maybe BIT_7
#define AUDIO_HP_DPOP_IRQ_RAW_STATUS   (BIT_13)
#define OVP_IRQ_RAW_STATUS             (BIT_12)
#define OTP_IRQ_RAW_STATUS             (BIT_11)
#define PA_OCP_IRQ_RAW_STATUS          (BIT_10)
#define EAR_OCP_IRQ_RAW_STATUS         (BIT_9)
#define HP_OCP_IRQ_RAW_STATUS          (BIT_8)

#define AUDIO_IRQ                        (0)
#define AUDIO_IRQ_MASK                   (MASK_8_BITS << AUDIO_IRQ)
#define AUDIO_RCV_DEPOP_IRQ_MASK_STATUS  (BIT_7)
#define AUDIO_PACAL_IRQ_MASK_STATUS      (BIT_6) // TODO: Maybe BIT_0
#define AUDIO_HP_DPOP_IRQ_MASK_STATUS    (BIT_5)
#define OVP_IRQ_MASK_STATUS              (BIT_4)
#define OTP_IRQ_MASK_STATUS              (BIT_3)
#define PA_OCP_IRQ_MASK_STATUS           (BIT_2)
#define EAR_OCP_IRQ_MASK_STATUS          (BIT_1)
#define HP_OCP_IRQ_MASK_STATUS           (BIT_0)

/// AUD_CFGA_INT_MODULE_CTRL 0x2c ///
/// aud interrupt module control
/* AUD_INT_EN	[7: 0]	RW	0	int enable: {audio_rcv_depop,audio_pacal_irq,audio_hp_dpop_irq,ovp_irq,otp_irq,pa_ocp_irq,ear_ocp_irq,hp_ocp_irq} */
#define AUDIO_IRQ_EN_MASK       (MASK_7_BITS)
#define AUDIO_RCV_DEPOP_IRQ_EN  (BIT_7)
#define AUDIO_PACAL_IRQ_EN      (BIT_6) // TODO: Maybe BIT_0
#define AUDIO_HP_DPOP_IRQ_EN    (BIT_5)
#define OVP_IRQ_EN              (BIT_4)
#define OTP_IRQ_EN              (BIT_3)
#define PA_OCP_IRQ_EN           (BIT_2)
#define EAR_OCP_IRQ_EN          (BIT_1)
#define HP_OCP_IRQ_EN           (BIT_0)

/// AUD_CFGA_LP_MODULE_CTRL 0x30 ///
/// aud loop module control
/* ADC_EN_R             [5]	RW	0	adc right enable
 * DAC_EN_R             [4]	RW	0	dac right enable
 * ADC_EN_L             [3]	RW	0	adc left enable
 * DAC_EN_L             [2]	RW	0	adc right enable
 * AUDIO_LOOP_MAP_SEL	[1]	RW	0x1
 * AUDIO_ADIE_LOOP_EN	[0]	RW	0 */
#define ADC_EN_R            (BIT_5)
#define DAC_EN_R            (BIT_4)
#define ADC_EN_L            (BIT_3)
#define DAC_EN_L            (BIT_2)
#define AUDIO_LOOP_MAP_SEL  (BIT_1)
#define AUDIO_ADIE_LOOP_EN  (BIT_0)

/// ANA_ET2 0x34 ///
/// Audio DACL & DACR output mixer select
/* DEM_BYPASS           [4]     RW	0	bypass DEM
 * RG_AUD_DALR_MIX_SEL	[3:2]	RW	0	Audio DACL & DACR output mixer select
                                        00 = R/L         01 = R/0
                                        10 = 0/L         11 = 0/0
 * RG_AUD_DAS_MIX_SEL	[1:0]	RW	0	Audio DACL & DACR output mixer select
                                        00 = L + R       01 = 2 x L
                                        10 = 2 x R       11 = 0 */
#define DEM_BYPASS              (4)
#define AUD_DALR_MIX_SEL        (2)
#define AUD_DALR_MIX_SEL_MASK   (MASK_2_BITS << AUD_DALR_MIX_SEL)
#define AUD_DALR_MIX_RL_MODE    (0x00)
#define AUD_DALR_MIX_R0_MODE    (0x01)
#define AUD_DALR_MIX_0L_MODE    (0x10)
#define AUD_DALR_MIX_00_MODE	(0x11)
#define AUD_DAS_MIX_SEL			(0)
#define AUD_DAS_MIX_SEL_MASK	(MASK_2_BITS << AUD_DALR_MIX_SEL)
#define AUD_DAS_MIX_RL_MODE		(0x00)
#define AUD_DAS_MIX_2L_MODE		(0x01)
#define AUD_DAS_MIX_2R_MODE		(0x10)
#define AUD_DAS_MIX_00_MODE		(0x11)

/// CLK_EN 0x38 ///
/// clock enable
/* CLK_AUD_LOOP_INV_EN	[5]	RW	0   // invalid
 * CLK_AUD_6P5M_EN      [4]	RW	0   // invalid
 * CLK_AUD_LOOP_EN      [3]	RW	0
 * CLK_AUD_HID_EN       [2]	RW	0
 * CLK_AUD_1K_EN        [1]	RW	0
 * CLK_AUD_32K_EN       [0]	RW	0 */
#define CLK_AUD_MASK		(MASK_6_BITS)
#define CLK_AUD_LOOP_INV	(BIT_5) // invalid
#define CLK_AUD_6P5M		(BIT_4) // invalid
#define CLK_AUD_LOOP		(BIT_3)
#define CLK_AUD_HID			(BIT_2)
#define CLK_AUD_1K			(BIT_1)
#define CLK_AUD_32K			(BIT_0)

/// SOFT_RST 0x3c ///
/// audio soft reset
/* AUD_DAC_POST_SOFT_RST    [5]	WC	0
 * AUD_DIG_6P5M_SOFT_RST    [4]	WC	0
 * AUD_DIG_LOOP_SOFT_RST    [3]	WC	0
 * AUD_HID_SOFT_RST	        [2]	WC	0
 * AUD_1K_SOFT_RST	        [1]	WC	0
 * AUD_32K_SOFT_RST	        [0]	WC	0 */
#define AUD_DAC_POST_SOFT_RST	(BIT_5)
#define AUD_DIG_6P5M_SOFT_RST	(BIT_4)
#define AUD_DIG_LOOP_SOFT_RST	(BIT_3)
#define AUD_HID_SOFT_RST		(BIT_2)
#define AUD_1K_SOFT_RST			(BIT_1)
#define AUD_32K_SOFT_RST		(BIT_0)
/**---------------------------------------------------------------------------*                            *
 **---------------------------------------------------------------------------*/
#define SC2720_AUD_CFGA_REG_BASE (0x00000800)

typedef struct
{
	uint32 aud_cfgal_clr;			//0x00

	uint32 aud_cfga_hid_cfg0;		//0x04
	uint32 aud_cfga_hid_cfg1;		//0x08
	uint32 aud_cfga_hid_cfg2;		//0x0c
	uint32 aud_cfga_hid_cfg3;		//0x10
	uint32 aud_cfga_hid_cfg4;		//0x14
	uint32 aud_cfga_hid_cfg5;		//0x18

	uint32 aud_cfga_hid_sts0;		//0x1c

	uint32 aud_cfga_prt_cfg_0;		//0x20
	uint32 aud_cfga_prt_cfg_1;		//0x24

	uint32 aud_cfga_rd_sts;			//0x28

	uint32 aud_cfga_int_module_ctrl;//002c
	uint32 aud_cfga_lp_module_ctrl;	//0030

	uint32 ana_et2;					//0034
	uint32 clk_en;					//0038
	uint32 soft_rst;				//003c
} SPRD_AUD_CFGA_REG_CTL_T;

typedef enum
{
	SPRD_CODEC_MAIN_MIC_NONE_OCCUPIED = 0,
	SPRD_CODEC_MAIN_MIC_HP_OCCUPIED = BIT_0,
	SPRD_CODEC_MAIN_MIC_MIC_OCCUPIED = BIT_1,
	SPRD_CODEC_MAIN_MIC_AUXMIC_OCCUPIED = BIT_2
}MAIN_MIC_OCCUPIED_E;

#endif//__SPRD_CODEC_AP_PHY_SC1161_H___
