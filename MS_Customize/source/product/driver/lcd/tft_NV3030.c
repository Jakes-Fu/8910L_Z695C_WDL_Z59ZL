/******************************************************************************
 ** File Name:     tft_NV3030.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
 ** Author:         sky.wang                                                 *
 ** DATE:           20/07/2018                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/102010      lichd    	    Create.
 ******************************************************************************/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
#include "lcm_drvapi.h"
#include "spi_drvapi.h"
#include "timer_drvapi.h"

#include "hal_gouda.h"
#include "ldo_drvapi.h"
#include "lcm_cfg.h"


/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
uint32 _Read_NV3030_ID(void);
#define SCI_TraceLow_Test SCI_TraceLow

LOCAL uint16 NV3030_line_mode = 0;
LOCAL uint16 NV3030_farmk = 0;


#define LCM_WR_REG(Addr, Data)                              \
{                                                       \
    while (hal_GoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
    ;                                               \
}
#define LCD_DataWrite_NV3030(Data)                               \
{                                                  \
    while (hal_GoudaWriteData(Data) != HAL_ERR_NO) \
    ;                                          \
}
#define LCD_CtrlWrite_NV3030(Cmd)                              \
{                                                \
    while (hal_GoudaWriteCmd(Cmd) != HAL_ERR_NO) \
    ;                                        \
}

//#define LCD_CtrlData_NV3030(cmd, data)  LCD_CtrlWrite_NV3030( cmd ); LCD_DataWrite_NV3030( data );

/*lint -restore*/

#ifdef CHIP_ENDIAN_LITTLE
typedef union {
    struct {
        unsigned int resv   :2;
        unsigned int mh    :1;	// display data latch order
        unsigned int rgb    :1;	// rgb order
        unsigned int ml    :1;		// line address order
        unsigned int mv    :1;	// x/y exchanged
        unsigned int mx    :1;	// horizontal mirror
        unsigned int my    :1;	// vertical mirror 
    } bitsval;
    unsigned int dataval;
} MEM_DATA_ACCESS_CTRL_U;
#else
typedef union {
    struct {
        unsigned int my    :1;	// vertical mirror 
        unsigned int mx    :1;	// horizontal mirror
        unsigned int mv    :1;	// x/y exchanged
        unsigned int ml    :1;		// line address order
        unsigned int rgb    :1;	// rgb order
        unsigned int mh    :1;	// display data latch order
        unsigned int resv   :2;
    } bitsval;
    unsigned int dataval;
} MEM_DATA_ACCESS_CTRL_U;
#endif // CHIP_ENDIAN_LITTLE

LOCAL MEM_DATA_ACCESS_CTRL_U s_lcd_madc;


/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void NV3030_Close(void);

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NV3030_EnterSleep(BOOLEAN is_sleep);

/*********************************************************************/
//  Description:   Initialize color LCD : GC9304
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E NV3030_Init(void);

LOCAL ERR_LCD_E NV3030_Invalidate(void);
/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
LOCAL ERR_LCD_E NV3030_InvalidateRect(
        uint16 left, //the left value of the rectangel
        uint16 top, //top of the rectangle
        uint16 right, //right of the rectangle
        uint16 bottom //bottom of the rectangle
        );

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void NV3030_set_display_window(
        uint16 left, // start Horizon address
        uint16 right, // end Horizon address
        uint16 top, // start Vertical address
        uint16 bottom // end Vertical address
        );

LOCAL uint32 NV3030_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

LOCAL ERR_LCD_E NV3030_SetDirection(LCD_DIRECT_E direct_type)
{
   // gouda_event(0x90001101);

    SCI_TraceLow_Test("lcd: NV3030_SetDirection = %d",direct_type);
#if 0
    switch(direct_type)
    {
        case LCD_DIRECT_NORMAL:
            s_lcd_madc.bitsval.mx = 0;
            s_lcd_madc.bitsval.my = 0;
            s_lcd_madc.bitsval.mv = 0;
            break;
        case LCD_DIRECT_ROT_90:
            s_lcd_madc.bitsval.mx = 1;
            s_lcd_madc.bitsval.my = 0;
            s_lcd_madc.bitsval.mv = 1;
            break;
        case LCD_DIRECT_ROT_180:
            s_lcd_madc.bitsval.mx = 1;
            s_lcd_madc.bitsval.my = 1;
            s_lcd_madc.bitsval.mv = 0;
            break;
        case LCD_DIRECT_ROT_270:
            s_lcd_madc.bitsval.mx = 0;
            s_lcd_madc.bitsval.my = 1;
            s_lcd_madc.bitsval.mv = 1;
            break;
        case LCD_DIRECT_MIR_H:
            s_lcd_madc.bitsval.mx = 1;
            s_lcd_madc.bitsval.my = 0;
            s_lcd_madc.bitsval.mv = 0;
            break;
        case LCD_DIRECT_MIR_V:
            s_lcd_madc.bitsval.mx = 0;
            s_lcd_madc.bitsval.my = 1;
            s_lcd_madc.bitsval.mv = 0;
            break;
        case LCD_DIRECT_MIR_HV:
            s_lcd_madc.bitsval.mx = 1;
            s_lcd_madc.bitsval.my = 1;
            s_lcd_madc.bitsval.mv = 0;
            break;
        default:
            s_lcd_madc.bitsval.mx = 0;
            s_lcd_madc.bitsval.my = 0;
            s_lcd_madc.bitsval.mv = 0;
            break;
    }

    s_lcd_madc.bitsval.mx = ~s_lcd_madc.bitsval.mx;
    s_lcd_madc.bitsval.rgb = 1;
    LCD_CtrlWrite_NV3030(0x36);
    LCD_DataWrite_NV3030(s_lcd_madc.dataval);
    LCD_CtrlWrite_NV3030(0x2c);
	#endif
    return ERR_LCD_NONE;

}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//	Note:
/******************************************************************************/
LOCAL void NV3030_set_display_window(
        uint16 left, // start Horizon address
        uint16 top, // start Vertical address
        uint16 right, // end Horizon address
        uint16 bottom // end Vertical address
        )
{   
    uint16 newleft   = left;
    uint16 newtop    = top;
    uint16 newright  = right;
    uint16 newbottom = bottom;
    // SCI_TraceLow_Test("[DRV_LCD]NV3030_set_display_window L = %d, top = %d, R = %d, bottom = %d",left,top,right,bottom);
    gouda_event(0x90001102);
    //hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3);
    LCD_CtrlWrite_NV3030(0x2a);	// set hori start , end (left, right)
    LCD_DataWrite_NV3030((newleft>>8)&0xff); // left high 8 b
    LCD_DataWrite_NV3030(newleft&0xff); // left low 8 b
    LCD_DataWrite_NV3030((newright>>8)&0xff); // right high 8 b
    LCD_DataWrite_NV3030(newright&0xff); // right low 8 b

    LCD_CtrlWrite_NV3030(0x2b);	// set vert start , end (top, bot)
    LCD_DataWrite_NV3030((newtop>>8)&0xff); // top high 8 b
    LCD_DataWrite_NV3030(newtop&0xff); // top low 8 b
    LCD_DataWrite_NV3030((newbottom>>8)&0xff); // bot high 8 b
    LCD_DataWrite_NV3030(newbottom&0xff); // bot low 8 b
    LCD_CtrlWrite_NV3030(0x2c);	 // recover memory write mode
    //hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3_2_LANE);

}

extern void hal_GoudatResetLcdPin(void);
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E NV3030_Init(void)
{
    SCI_TraceLow_Test("lcd: NV3030_Init here");

    mon_Event(0x55555555);


#if 0 // use NV3030 spi org init code 
    LCD_CtrlWrite_NV3030(0xfe);
    LCD_CtrlWrite_NV3030(0xef);

    LCD_CtrlWrite_NV3030(0x35);
    LCD_DataWrite_NV3030(0x00);

    LCD_CtrlWrite_NV3030(0x36);
    LCD_DataWrite_NV3030(0x48);

    LCD_CtrlWrite_NV3030(0x3a);
    LCD_DataWrite_NV3030(0x05);

    LCD_CtrlWrite_NV3030(0xa4);
    LCD_DataWrite_NV3030(0x44);
    LCD_DataWrite_NV3030(0x44);

    LCD_CtrlWrite_NV3030(0xa5);
    LCD_DataWrite_NV3030(0x42);
    LCD_DataWrite_NV3030(0x42);

    LCD_CtrlWrite_NV3030(0xaa);
    LCD_DataWrite_NV3030(0x88);
    LCD_DataWrite_NV3030(0x88);

    LCD_CtrlWrite_NV3030(0xe8);
    LCD_DataWrite_NV3030(0x11);
    LCD_DataWrite_NV3030(0x77);

    LCD_CtrlWrite_NV3030(0xe3);
    LCD_DataWrite_NV3030(0x01);
    LCD_DataWrite_NV3030(0x18);

    LCD_CtrlWrite_NV3030(0xe1);
    LCD_DataWrite_NV3030(0x10);
    LCD_DataWrite_NV3030(0x0a);

    LCD_CtrlWrite_NV3030(0xac);
    LCD_DataWrite_NV3030(0x00);

    LCD_CtrlWrite_NV3030(0xaf);
    LCD_DataWrite_NV3030(0x67);

    LCD_CtrlWrite_NV3030(0xa6);
    LCD_DataWrite_NV3030(0x29);
    LCD_DataWrite_NV3030(0x29);

    LCD_CtrlWrite_NV3030(0xa7);
    LCD_DataWrite_NV3030(0x27);
    LCD_DataWrite_NV3030(0x27);

    LCD_CtrlWrite_NV3030(0xa8);
    LCD_DataWrite_NV3030(0x17);
    LCD_DataWrite_NV3030(0x17);

    LCD_CtrlWrite_NV3030(0xa9);
    LCD_DataWrite_NV3030(0x26);
    LCD_DataWrite_NV3030(0x26);

    //----gamma setting---------//
    LCD_CtrlWrite_NV3030(0xf0);
    LCD_DataWrite_NV3030(0x02);
    LCD_DataWrite_NV3030(0x02);
    LCD_DataWrite_NV3030(0x00);
    LCD_DataWrite_NV3030(0x02);
    LCD_DataWrite_NV3030(0x07);
    LCD_DataWrite_NV3030(0x0c);

    LCD_CtrlWrite_NV3030(0xf1);
    LCD_DataWrite_NV3030(0x01);
    LCD_DataWrite_NV3030(0x01);
    LCD_DataWrite_NV3030(0x00);
    LCD_DataWrite_NV3030(0x03);
    LCD_DataWrite_NV3030(0x07);
    LCD_DataWrite_NV3030(0x0f);

    LCD_CtrlWrite_NV3030(0xf2);
    LCD_DataWrite_NV3030(0x0D);
    LCD_DataWrite_NV3030(0x08);
    LCD_DataWrite_NV3030(0x37);
    LCD_DataWrite_NV3030(0x04);
    LCD_DataWrite_NV3030(0x04);
    LCD_DataWrite_NV3030(0x4b);

    LCD_CtrlWrite_NV3030(0xf3);
    LCD_DataWrite_NV3030(0x11);
    LCD_DataWrite_NV3030(0x0c);
    LCD_DataWrite_NV3030(0x37);
    LCD_DataWrite_NV3030(0x04);
    LCD_DataWrite_NV3030(0x04);
    LCD_DataWrite_NV3030(0x47);

    LCD_CtrlWrite_NV3030(0xf4);
    LCD_DataWrite_NV3030(0x0a);
    LCD_DataWrite_NV3030(0x15);
    LCD_DataWrite_NV3030(0x15);
    LCD_DataWrite_NV3030(0x24);
    LCD_DataWrite_NV3030(0x3a);
    LCD_DataWrite_NV3030(0x0F);

    LCD_CtrlWrite_NV3030(0xf5);
    LCD_DataWrite_NV3030(0x07);
    LCD_DataWrite_NV3030(0x0f);
    LCD_DataWrite_NV3030(0x0d);
    LCD_DataWrite_NV3030(0x17);
    LCD_DataWrite_NV3030(0x3a);
    LCD_DataWrite_NV3030(0x0F);

#else
#if  0

   hal_LcdDelay(120);

LCD_CtrlWrite_NV3030(0xfd);//private_access
LCD_DataWrite_NV3030(0x06);	
LCD_DataWrite_NV3030(0x08);


////osc 11.2K 45.87M 	
LCD_CtrlWrite_NV3030(0x60);//	
LCD_DataWrite_NV3030(0x01);//osc_user_adj[3:0] 07
LCD_DataWrite_NV3030(0x01);//
LCD_DataWrite_NV3030(0x01);//01

LCD_CtrlWrite_NV3030(0x61);//	
LCD_DataWrite_NV3030(0x07);//00
LCD_DataWrite_NV3030(0x04);//

//bias
LCD_CtrlWrite_NV3030(0x62);//bias setting	
LCD_DataWrite_NV3030(0x00);//01
LCD_DataWrite_NV3030(0x44);//04	44
LCD_DataWrite_NV3030(0x40);//44	65	40
LCD_DataWrite_NV3030(0x01);//06	vref_adj

LCD_CtrlWrite_NV3030(0x63);//
LCD_DataWrite_NV3030(0x41);//
LCD_DataWrite_NV3030(0x07);//
LCD_DataWrite_NV3030(0x12);//
LCD_DataWrite_NV3030(0x12);//

LCD_CtrlWrite_NV3030(0x64);//
LCD_DataWrite_NV3030(0x37);//

//VSP
LCD_CtrlWrite_NV3030(0x65);//Pump1=4.7MHz //PUMP1 VSP
LCD_DataWrite_NV3030(0x09);//D6-5:pump1_clk[1:0] clamp  28	2b
LCD_DataWrite_NV3030(0x10);//6.26	
LCD_DataWrite_NV3030(0x21);

//VSN
LCD_CtrlWrite_NV3030(0x66);	//pump=2 AVCL		  
LCD_DataWrite_NV3030(0x09); //clamp  08	 0b	 09
LCD_DataWrite_NV3030(0x10);	//10
LCD_DataWrite_NV3030(0x21);

//add source_neg_time
LCD_CtrlWrite_NV3030(0x67);//pump_sel
LCD_DataWrite_NV3030(0x20);//21	20
LCD_DataWrite_NV3030(0x40);

//gamma vap/van
LCD_CtrlWrite_NV3030(0x68);//gamma vap/van
LCD_DataWrite_NV3030(0x90);//78-90-af	 9f
LCD_DataWrite_NV3030(0x4c);//	
LCD_DataWrite_NV3030(0x34);//23 
LCD_DataWrite_NV3030(0x07);//07

LCD_CtrlWrite_NV3030(0xb1);//frame rate 	
LCD_DataWrite_NV3030(0x0F);//0x0f fr_h[5:0]   0F
LCD_DataWrite_NV3030(0x02);//0x02 fr_v[4:0]   02
LCD_DataWrite_NV3030(0x03);//0x04 fr_div[2:0] 03

LCD_CtrlWrite_NV3030(0xB4);	
LCD_DataWrite_NV3030(0x01);	//2dot 

////porch
LCD_CtrlWrite_NV3030(0xB5);	
LCD_DataWrite_NV3030(0x02);//0x02 vfp[6:0]	
LCD_DataWrite_NV3030(0x02);//0x02 vbp[6:0]	
LCD_DataWrite_NV3030(0x0a);//0x0A hfp[6:0]	
LCD_DataWrite_NV3030(0x14);//0x14 hbp[6:0]

LCD_CtrlWrite_NV3030(0xB6);	
LCD_DataWrite_NV3030(0x04);//
LCD_DataWrite_NV3030(0x01);//
LCD_DataWrite_NV3030(0x9f);//
LCD_DataWrite_NV3030(0x00);//
LCD_DataWrite_NV3030(0x02);//

////gamme sel 						
LCD_CtrlWrite_NV3030(0xdf);//	
LCD_DataWrite_NV3030(0x11);//gofc_gamma_en_sel=1

////gamma_test1	A1#_wangly
//3030b_gamma_new_12.18
//GAMMA---------------------------------/////////////
LCD_CtrlWrite_NV3030(0xe0); //gmama set 2.2
LCD_DataWrite_NV3030(0x0e); //PKP0[4:0]  
LCD_DataWrite_NV3030(0x0D); //PKP1[4:0]  		
LCD_DataWrite_NV3030(0x0b); //PKP2[4:0] 			
LCD_DataWrite_NV3030(0x0f); //PKP3[4:0] 			 
LCD_DataWrite_NV3030(0x13); //PKP4[4:0] 		
LCD_DataWrite_NV3030(0x14); //PKP5[4:0] 			 
LCD_DataWrite_NV3030(0x10); //PKP6[4:0] 	        
LCD_DataWrite_NV3030(0x13); //PKP6[4:0] 	
LCD_CtrlWrite_NV3030(0xe3);
LCD_DataWrite_NV3030(0x10); //PKN0[4:0] 	    
LCD_DataWrite_NV3030(0x0c); //PKN1[4:0] 		
LCD_DataWrite_NV3030(0x11); //PKN2[4:0] 	          
LCD_DataWrite_NV3030(0x13); //PKN3[4:0] 			
LCD_DataWrite_NV3030(0x12); //PKN4[4:0] 		 
LCD_DataWrite_NV3030(0x0f); //PKN5[4:0] 		
LCD_DataWrite_NV3030(0x0E); //PKN6[4:0] 		 
LCD_DataWrite_NV3030(0x07); //PKN6[4:0] 		 
LCD_CtrlWrite_NV3030(0xe1);
LCD_DataWrite_NV3030(0x41); //PRP0[6:0]   			
LCD_DataWrite_NV3030(0x6d); //PRP1[6:0]   			 
LCD_CtrlWrite_NV3030(0xe4);												 
LCD_DataWrite_NV3030(0x68); //PRN0[6:0]  	                 
LCD_DataWrite_NV3030(0x2e); //PRN1[6:0] 		
LCD_CtrlWrite_NV3030(0xe2);
LCD_DataWrite_NV3030(0x1e); //VRP0[5:0]    		 
LCD_DataWrite_NV3030(0x1C); //VRP1[5:0]       		 
LCD_DataWrite_NV3030(0x20); //VRP2[5:0]  		 
LCD_DataWrite_NV3030(0x1c); //VRP3[5:0]  		  
LCD_DataWrite_NV3030(0x36); //VRP4[5:0]   			 
LCD_DataWrite_NV3030(0x3B); //VRP5[5:0]  			
LCD_CtrlWrite_NV3030(0xe5);
LCD_DataWrite_NV3030(0x3B); //VRN0[5:0] 	   		    
LCD_DataWrite_NV3030(0x35); //VRN1[5:0]  				  
LCD_DataWrite_NV3030(0x23); //VRN2[5:0]  			  
LCD_DataWrite_NV3030(0x0a); //VRN3[5:0] 		  
LCD_DataWrite_NV3030(0x19); //VRN4[5:0]  	 			  
LCD_DataWrite_NV3030(0x10); //VRN5[5:0] 
//GAMMA---------------------------------/////////////



//source

LCD_CtrlWrite_NV3030(0xE6);	
LCD_DataWrite_NV3030(0x00);	
LCD_DataWrite_NV3030(0xff);//SC_EN_START[7:0]  f0	

LCD_CtrlWrite_NV3030(0xE7);	
LCD_DataWrite_NV3030(0x01);//CS_START[3:0]  01	
LCD_DataWrite_NV3030(0x04);//scdt_inv_sel cs_vp_en	
LCD_DataWrite_NV3030(0x03);//CS1_WIDTH[7:0]	12
LCD_DataWrite_NV3030(0x03);//CS2_WIDTH[7:0]	12
LCD_DataWrite_NV3030(0x00);//PREC_START[7:0]  06 	
LCD_DataWrite_NV3030(0x12);//PREC_WIDTH[7:0]  12


LCD_CtrlWrite_NV3030(0xE8);	//source
LCD_DataWrite_NV3030(0x00); //VCMP_OUT_EN	 81-vcmp/vref_output pad
LCD_DataWrite_NV3030(0x70);	//chopper_sel[6:4]
LCD_DataWrite_NV3030(0x00); //gchopper_sel[6:4]	 60

////gate 
LCD_CtrlWrite_NV3030(0xEc);	
LCD_DataWrite_NV3030(0x52);//47  43



LCD_CtrlWrite_NV3030(0xF1);	
LCD_DataWrite_NV3030(0x00);//te_pol	tem_extend

LCD_CtrlWrite_NV3030(0xfd);	
LCD_DataWrite_NV3030(0xfa);	
LCD_DataWrite_NV3030(0xfc);

	
LCD_CtrlWrite_NV3030(0x3a);	
LCD_DataWrite_NV3030(0x05);//SH 0x66

LCD_CtrlWrite_NV3030(0x35);	
LCD_DataWrite_NV3030(0x00);
							
LCD_CtrlWrite_NV3030(0x36);//bgr_[3]
LCD_DataWrite_NV3030(0x00);//c0
                                                                                                                     
                                                                                          
LCD_CtrlWrite_NV3030(0x11); // exit sleep                                                                                                
hal_LcdDelay(200);                                                                                                                     
LCD_CtrlWrite_NV3030(0x29); // display on        
hal_LcdDelay(10);
   #else

   	//hal_GoudatResetLcdPin();
	 hal_LcdDelay(120);


	// lxj ???? NV3030B + CTC 
	/*
	LCD_CtrlWrite_NV3030(0xfd);//private_access
	LCD_DataWrite_NV3030(0x06);
	LCD_DataWrite_NV3030(0x08);
	////osc
	//LCD_CtrlWrite_NV3030(0x60);//
	//LCD_DataWrite_NV3030(0x01);//osc_user_adj[3:0] 08
	//LCD_DataWrite_NV3030(0x01);//
	//LCD_DataWrite_NV3030(0x01);//01

	//LCD_CtrlWrite_NV3030(0x61);//
	//LCD_DataWrite_NV3030(0x07);//
	//LCD_DataWrite_NV3030(0x04);//
	//bias
	LCD_CtrlWrite_NV3030(0x62);//bias setting
	LCD_DataWrite_NV3030(0x00);//01
	LCD_DataWrite_NV3030(0x44);//04 44
	LCD_DataWrite_NV3030(0x40);//44 65 40
	//LCD_DataWrite_NV3030(0x01);//06 vref_adj 01

	LCD_CtrlWrite_NV3030(0x63);//VGL
	LCD_DataWrite_NV3030(0x41);//
	LCD_DataWrite_NV3030(0x07);//
	LCD_DataWrite_NV3030(0x12);//
	LCD_DataWrite_NV3030(0x12);//
	//VSP
	LCD_CtrlWrite_NV3030(0x65);//Pump1=4.7MHz //PUMP1 VSP
	LCD_DataWrite_NV3030(0x09);//D6-5:pump1_clk[1:0] clamp 28 2b
	LCD_DataWrite_NV3030(0x17);//6.26
	LCD_DataWrite_NV3030(0x21);
	//VSN
	LCD_CtrlWrite_NV3030(0x66); //pump=2 AVCL
	LCD_DataWrite_NV3030(0x09); //clamp 08 0b 09
	LCD_DataWrite_NV3030(0x17); //10
	LCD_DataWrite_NV3030(0x21);
	//add source_neg_time
	LCD_CtrlWrite_NV3030(0x67);//pump_sel
	LCD_DataWrite_NV3030(0x20);//21 20
	LCD_DataWrite_NV3030(0x40);
	//gamma vap/van
	LCD_CtrlWrite_NV3030(0x68);//gamma vap/van
	LCD_DataWrite_NV3030(0x40);//90 9f
	LCD_DataWrite_NV3030(0x20);//30
	LCD_DataWrite_NV3030(0x70);//77
	LCD_DataWrite_NV3030(0x70);//07

	LCD_CtrlWrite_NV3030(0xb1);//frame rate
	LCD_DataWrite_NV3030(0x0F);//0x0f fr_h[5:0] 0F
	LCD_DataWrite_NV3030(0x02);//0x02 fr_v[4:0] 02
	LCD_DataWrite_NV3030(0x05);//0x04 fr_div[2:0] 03

	LCD_CtrlWrite_NV3030(0xB4);
	LCD_DataWrite_NV3030(0x01); //00:column 01:dot
	////porch
	LCD_CtrlWrite_NV3030(0xB5);
	LCD_DataWrite_NV3030(0x02);//0x02 vfp[6:0]
	LCD_DataWrite_NV3030(0x02);//0x02 vbp[6:0]
	LCD_DataWrite_NV3030(0x0a);//0x0A hfp[6:0]
	LCD_DataWrite_NV3030(0x14);//0x14 hbp[6:0]

	LCD_CtrlWrite_NV3030(0xB6);
	LCD_DataWrite_NV3030(0x04);//04
	LCD_DataWrite_NV3030(0x01);//01
	LCD_DataWrite_NV3030(0x9f);//
	LCD_DataWrite_NV3030(0x00);//
	LCD_DataWrite_NV3030(0x02);//
	////gamme sel
	LCD_CtrlWrite_NV3030(0xdf);//
	LCD_DataWrite_NV3030(0x11);//gofc_gamma_en_sel=1
	//GAMMA---------------------------------/////////////
	LCD_CtrlWrite_NV3030(0xE2);
	LCD_DataWrite_NV3030(0x00);//vrp0[5:0] V0
	LCD_DataWrite_NV3030(0x02);//vrp1[5:0] V1
	LCD_DataWrite_NV3030(0x05);//vrp2[5:0] V2
	LCD_DataWrite_NV3030(0x1b);//vrp3[5:0] V61
	LCD_DataWrite_NV3030(0x19);//vrp4[5:0] V62
	LCD_DataWrite_NV3030(0x3f);//vrp5[5:0] V63

	LCD_CtrlWrite_NV3030(0xE5);
	LCD_DataWrite_NV3030(0x3f);//vrn0[5:0] V63
	LCD_DataWrite_NV3030(0x19);//vrn1[5:0] V62
	LCD_DataWrite_NV3030(0x1a);//vrn2[5:0] V61
	LCD_DataWrite_NV3030(0x05);//vrn3[5:0] V2
	LCD_DataWrite_NV3030(0x02);//vrn4[5:0] V1
	LCD_DataWrite_NV3030(0x00);//vrn5[5:0] V0

	LCD_CtrlWrite_NV3030(0xE1);
	LCD_DataWrite_NV3030(0x2b);//prp0[6:0] V15
	LCD_DataWrite_NV3030(0x5e);//prp1[6:0] V51

	LCD_CtrlWrite_NV3030(0xE4);
	LCD_DataWrite_NV3030(0x5f);//prn0[6:0] V51
	LCD_DataWrite_NV3030(0x20);//prn1[6:0] V15

	LCD_CtrlWrite_NV3030(0xE0);
	LCD_DataWrite_NV3030(0x08);//pkp0[4:0] V3
	LCD_DataWrite_NV3030(0x0c);//pkp1[4:0] V7
	LCD_DataWrite_NV3030(0x10);//pkp2[4:0] V21
	LCD_DataWrite_NV3030(0x16);//pkp3[4:0] V29
	LCD_DataWrite_NV3030(0x18);//pkp4[4:0] V37 //
	LCD_DataWrite_NV3030(0x10);//pkp5[4:0] V45
	LCD_DataWrite_NV3030(0x12);//pkp6[4:0] V56
	LCD_DataWrite_NV3030(0x15);//pkp7[4:0] V60

	LCD_CtrlWrite_NV3030(0xE3);
	LCD_DataWrite_NV3030(0x19);//pkn0[4:0] V60
	LCD_DataWrite_NV3030(0x11);//pkn1[4:0] V56
	LCD_DataWrite_NV3030(0x17);//pkn2[4:0] V45
	LCD_DataWrite_NV3030(0x16);//pkn3[4:0] V37 //
	LCD_DataWrite_NV3030(0x13);//pkn4[4:0] V29
	LCD_DataWrite_NV3030(0x10);//pkn5[4:0] V21
	LCD_DataWrite_NV3030(0x0d);//pkn6[4:0] V7
	LCD_DataWrite_NV3030(0x08);//pkn7[4:0] V3

	//GAMMA---------------------------------/////////////
	LCD_CtrlWrite_NV3030(0xE6);
	LCD_DataWrite_NV3030(0x00);
	LCD_DataWrite_NV3030(0xff);//SC_EN_START[7:0] f0

	LCD_CtrlWrite_NV3030(0xE7);
	LCD_DataWrite_NV3030(0x01);//CS_START[3:0] 01
	LCD_DataWrite_NV3030(0x04);//scdt_inv_sel cs_vp_en
	LCD_DataWrite_NV3030(0x03);//CS1_WIDTH[7:0] 12
	LCD_DataWrite_NV3030(0x03);//CS2_WIDTH[7:0] 12
	LCD_DataWrite_NV3030(0x00);//PREC_START[7:0] 06
	LCD_DataWrite_NV3030(0x12);//PREC_WIDTH[7:0] 12

	LCD_CtrlWrite_NV3030(0xE8); //source
	LCD_DataWrite_NV3030(0x00); //VCMP_OUT_EN 81-vcmp/vref_output pad
	LCD_DataWrite_NV3030(0x70); //chopper_sel[6:4]
	LCD_DataWrite_NV3030(0x00); //gchopper_sel[6:4] 60
	////gate
	LCD_CtrlWrite_NV3030(0xEc);
	LCD_DataWrite_NV3030(0x50);//50

	LCD_CtrlWrite_NV3030(0xF1);
	LCD_DataWrite_NV3030(0x00);//te_pol tem_extend


	LCD_CtrlWrite_NV3030(0xf6);   ////3Ïß2
	LCD_DataWrite_NV3030(0x01);//04
	LCD_DataWrite_NV3030(0x10);//01
	LCD_DataWrite_NV3030(0x00);//
	LCD_DataWrite_NV3030(0x40);//
	
	
	LCD_CtrlWrite_NV3030(0xfd);
	LCD_DataWrite_NV3030(0xfa);
	LCD_DataWrite_NV3030(0xfc);
*/
//20220704
LCD_CtrlWrite_NV3030(0xfd);//private_access
	LCD_DataWrite_NV3030(0x06);
	LCD_DataWrite_NV3030(0x08);
	////osc
	//LCD_CtrlWrite_NV3030(0x60);//
	//LCD_DataWrite_NV3030(0x01);//osc_user_adj[3:0] 08
//LCD_DataWrite_NV3030(0x01);//
//LCD_DataWrite_NV3030(0x01);//01
LCD_CtrlWrite_NV3030(0x61);//	
LCD_DataWrite_NV3030(0x06);//
LCD_DataWrite_NV3030(0x04);//

	//bias
	LCD_CtrlWrite_NV3030(0x62);//bias setting
	LCD_DataWrite_NV3030(0x00);//01
	LCD_DataWrite_NV3030(0x44);//04 44
	LCD_DataWrite_NV3030(0x40);//44 65 40
	//LCD_DataWrite_NV3030(0x01);//06 vref_adj 01

	LCD_CtrlWrite_NV3030(0x63);//VGL
	LCD_DataWrite_NV3030(0x41);//
	LCD_DataWrite_NV3030(0x07);//
	LCD_DataWrite_NV3030(0x12);//
	LCD_DataWrite_NV3030(0x12);//
	//VSP
	LCD_CtrlWrite_NV3030(0x65);//Pump1=4.7MHz //PUMP1 VSP
	LCD_DataWrite_NV3030(0x09);//D6-5:pump1_clk[1:0] clamp 28 2b
	LCD_DataWrite_NV3030(0x17);//6.26
	LCD_DataWrite_NV3030(0x21);
	//VSN
	LCD_CtrlWrite_NV3030(0x66); //pump=2 AVCL
	LCD_DataWrite_NV3030(0x09); //clamp 08 0b 09
	LCD_DataWrite_NV3030(0x17); //10
	LCD_DataWrite_NV3030(0x21);
	//add source_neg_time
	LCD_CtrlWrite_NV3030(0x67);//pump_sel
LCD_DataWrite_NV3030(0x20);//21 20
LCD_DataWrite_NV3030(0x40);
//gamma vap/van
LCD_CtrlWrite_NV3030(0x68);//gamma vap/van
LCD_DataWrite_NV3030(0x90);//90 9f
LCD_DataWrite_NV3030(0x30);//30
LCD_DataWrite_NV3030(0x7E);//5e 77
LCD_DataWrite_NV3030(0x70);//50 07

LCD_CtrlWrite_NV3030(0xb1);//frame rate
LCD_DataWrite_NV3030(0x0F);//0x0f fr_h[5:0] 0F
LCD_DataWrite_NV3030(0x02);//0x02 fr_v[4:0] 02
LCD_DataWrite_NV3030(0x01);//0x04 fr_div[2:0] 03
LCD_CtrlWrite_NV3030(0xB4);
LCD_DataWrite_NV3030(0x01); //00:column 01:dot
////porch
	LCD_CtrlWrite_NV3030(0xB5);
	LCD_DataWrite_NV3030(0x02);//0x02 vfp[6:0]
	LCD_DataWrite_NV3030(0x02);//0x02 vbp[6:0]
	LCD_DataWrite_NV3030(0x0a);//0x0A hfp[6:0]
	LCD_DataWrite_NV3030(0x14);//0x14 hbp[6:0]

	LCD_CtrlWrite_NV3030(0xB6);
	LCD_DataWrite_NV3030(0x04);//04
	LCD_DataWrite_NV3030(0x01);//01
	LCD_DataWrite_NV3030(0x9f);//
	LCD_DataWrite_NV3030(0x00);//
	LCD_DataWrite_NV3030(0x02);//
	////gamme sel
LCD_CtrlWrite_NV3030(0xdf);//
LCD_DataWrite_NV3030(0x11);//gofc_gamma_en_sel=1
//GAMMA---------------------------------/////////////
LCD_CtrlWrite_NV3030(0xE2);
LCD_DataWrite_NV3030(0x0f);//vrp0[5:0] V0
LCD_DataWrite_NV3030(0x0b);//vrp1[5:0] V1
LCD_DataWrite_NV3030(0x12);//vrp2[5:0] V2
LCD_DataWrite_NV3030(0x1b);//vrp3[5:0] V61
LCD_DataWrite_NV3030(0x1b);//vrp4[5:0] V62
LCD_DataWrite_NV3030(0x3f);//vrp5[5:0] V63
LCD_CtrlWrite_NV3030(0xE5);
LCD_DataWrite_NV3030(0x3f);//vrn0[5:0] V63
LCD_DataWrite_NV3030(0x1a);//vrn1[5:0] V62
LCD_DataWrite_NV3030(0x1d);//vrn2[5:0] V61
LCD_DataWrite_NV3030(0x12);//vrn3[5:0] V2
LCD_DataWrite_NV3030(0x0b);//vrn4[5:0] V1
LCD_DataWrite_NV3030(0x0f);//vrn5[5:0] V0

LCD_CtrlWrite_NV3030(0xE1);
LCD_DataWrite_NV3030(0x2f);//prp0[6:0] V15
LCD_DataWrite_NV3030(0x66);//prp1[6:0] V51
LCD_CtrlWrite_NV3030(0xE4);
LCD_DataWrite_NV3030(0x66);//prn0[6:0] V51
LCD_DataWrite_NV3030(0x30);//prn1[6:0] V15
LCD_CtrlWrite_NV3030(0xE0);
LCD_DataWrite_NV3030(0x09);//pkp0[4:0] V3
LCD_DataWrite_NV3030(0x0c);//pkp1[4:0] V7
LCD_DataWrite_NV3030(0x11);//pkp2[4:0] V21
LCD_DataWrite_NV3030(0x14);//pkp3[4:0] V29
LCD_DataWrite_NV3030(0x16);//pkp4[4:0] V37 //
LCD_DataWrite_NV3030(0x10);//pkp5[4:0] V45
LCD_DataWrite_NV3030(0x12);//pkp6[4:0] V56
LCD_DataWrite_NV3030(0x15);//pkp7[4:0] V60
LCD_CtrlWrite_NV3030(0xE3);
LCD_DataWrite_NV3030(0x16);//pkn0[4:0] V60
LCD_DataWrite_NV3030(0x12);//pkn1[4:0] V56
LCD_DataWrite_NV3030(0x17);//pkn2[4:0] V45
LCD_DataWrite_NV3030(0x13);//pkn3[4:0] V37 //
LCD_DataWrite_NV3030(0x13);//pkn4[4:0] V29
LCD_DataWrite_NV3030(0x12);//pkn5[4:0] V21
LCD_DataWrite_NV3030(0x0d);//pkn6[4:0] V7
LCD_DataWrite_NV3030(0x0a);//pkn7[4:0] V3
//GAMMA---------------------------------/////////////
LCD_CtrlWrite_NV3030(0xE6);
LCD_DataWrite_NV3030(0x00);
LCD_DataWrite_NV3030(0xff);//SC_EN_START[7:0] f0
LCD_CtrlWrite_NV3030(0xE7);
LCD_DataWrite_NV3030(0x01);//CS_START[3:0] 01
LCD_DataWrite_NV3030(0x04);//scdt_inv_sel cs_vp_en
LCD_DataWrite_NV3030(0x03);//CS1_WIDTH[7:0] 12
LCD_DataWrite_NV3030(0x03);//CS2_WIDTH[7:0] 12
LCD_DataWrite_NV3030(0x00);//PREC_START[7:0] 06
LCD_DataWrite_NV3030(0x12);//PREC_WIDTH[7:0] 12
LCD_CtrlWrite_NV3030(0xE8); //source
LCD_DataWrite_NV3030(0x00); //VCMP_OUT_EN 81-vcmp/vref_output pad
LCD_DataWrite_NV3030(0x70); //chopper_sel[6:4]
LCD_DataWrite_NV3030(0x00); //gchopper_sel[6:4] 60
////gate
LCD_CtrlWrite_NV3030(0xEc);
LCD_DataWrite_NV3030(0x50);//50

LCD_CtrlWrite_NV3030(0xF1);	
LCD_DataWrite_NV3030(0x01);//TE
LCD_DataWrite_NV3030(0x01);
LCD_DataWrite_NV3030(0x02);



LCD_CtrlWrite_NV3030(0xf6);   ////3Ïß2
LCD_DataWrite_NV3030(0x01);//04
LCD_DataWrite_NV3030(0x30);//01
LCD_DataWrite_NV3030(0x00);//
LCD_DataWrite_NV3030(0x40);//

LCD_CtrlWrite_NV3030(0xfd);
LCD_DataWrite_NV3030(0xfa);
LCD_DataWrite_NV3030(0xfc);



	LCD_CtrlWrite_NV3030(0x3a);
	LCD_DataWrite_NV3030(0x55);//SH 0x66

	LCD_CtrlWrite_NV3030(0x35);
	LCD_DataWrite_NV3030(0x00);

	LCD_CtrlWrite_NV3030(0x36);//bgr_[3]
	LCD_DataWrite_NV3030(0x00);//c0
    
	LCD_CtrlWrite_NV3030(0x11);
	hal_LcdDelay(200);//100
	LCD_CtrlWrite_NV3030(0x29);
	hal_LcdDelay(20);//50
   #endif
#endif
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NV3030_EnterSleep(
        BOOLEAN is_sleep //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
        )
{
    SCI_TraceLow("lcd: NV3030_EnterSleep, is_sleep = %d", is_sleep);
   // gouda_event(0x90001104);

    if(is_sleep)
    {
       // gouda_event(0x900f2dc4);
        LCD_CtrlWrite_NV3030(0x28); //display off
        hal_LcdDelay(120);
        LCD_CtrlWrite_NV3030(0x10); // enter sleep mode
    }
    else
    {

       // gouda_event(0x9f0f2ddf);
#if 0
        LCD_CtrlWrite_NV3030(0x11); // sleep out mode
        hal_LcdDelay(120);
        LCD_CtrlWrite_NV3030(0x29); // display on
#endif
        NV3030_Init();

	_Read_NV3030_ID();
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void NV3030_Close(void)
{
    SCI_TraceLow("lcd:  in GC9304_Close");
    gouda_event(0x90001105);

    NV3030_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E NV3030_Invalidate(void)
{
    gouda_event(0x90001106);

    NV3030_set_display_window (0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT -1);
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
PUBLIC ERR_LCD_E NV3030_InvalidateRect(
        uint16 left, //the left value of the rectangel
        uint16 top, //top of the rectangle
        uint16 right, //right of the rectangle
        uint16 bottom //bottom of the rectangle
        )
{
    gouda_event(0x90001107);

    left = (left >=LCD_WIDTH) ? LCD_WIDTH-1 : left;
    right = (right >= LCD_WIDTH) ? LCD_WIDTH-1 : right;
    top = (top >= LCD_HEIGHT) ? LCD_HEIGHT-1 : top;
    bottom = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;

    if ( ( right < left ) || ( bottom < top ) )
    {
        return ERR_LCD_OPERATE_FAIL;
    }     

    NV3030_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E NV3030_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    gouda_event(0x90001108);

    SCI_TraceLow("lcd:  NV3030_RotationInvalidateRect");
    switch(angle)
    {
        case LCD_ANGLE_0:
            NV3030_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            NV3030_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            NV3030_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            NV3030_set_display_window(left, top, bottom,right);
            break;
        default:
            NV3030_set_display_window(left, top, right, bottom);
            break;
    }
    return ERR_LCD_NONE;
}



#define LCD_DEFAULT_STATUS_09H 0x80530000  //0xa4530000

LOCAL void GcCheckDisplayStatus(void)
{
    uint8 id[4] = {0};
    uint32 lcdstatus =0;

    hal_GoudaReadData(0x09, id, 4);

    lcdstatus = ((id[3]<<24) | (id[2]<<16));
	
    SCI_TraceLow("lcd: NV3030 09=0x%x\n",lcdstatus);
    if(LCD_DEFAULT_STATUS_09H != lcdstatus)
    {
        hal_GoudatResetLcdPin();
        hal_LcdDelay(120);
        NV3030_Init();
        SCI_TraceLow("lcd: NV3030 ReadDisplayStatus: RESET  \n");
    }
    else
    {
        SCI_TraceLow("lcd: NV3030 LCD_DEFAULT_STATUS_09H   \n");
		
	LCD_CtrlWrite_NV3030(0xfd);//private_access
	LCD_DataWrite_NV3030(0x06);
	LCD_DataWrite_NV3030(0x08);
		
      	LCD_CtrlWrite_NV3030(0xfd);
	LCD_DataWrite_NV3030(0xfa);
	LCD_DataWrite_NV3030(0xfc);

	LCD_CtrlWrite_NV3030(0x3a);
	LCD_DataWrite_NV3030(0x55);//SH 0x66

	LCD_CtrlWrite_NV3030(0x35);
	LCD_DataWrite_NV3030(0x00);

	LCD_CtrlWrite_NV3030(0x36);//bgr_[3]
	LCD_DataWrite_NV3030(0x00);//c0

	LCD_CtrlWrite_NV3030(0x11);
	hal_LcdDelay(100);
	LCD_CtrlWrite_NV3030(0x29);
    }
}


LOCAL ERR_LCD_E   NV3030_SetContrast(uint16  contrast)
{
    gouda_event(0x90001109);

    SCI_TraceLow("lcd: NV3030_SetContrast");
   GcCheckDisplayStatus();
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 
/*****************************************************************************/
LOCAL ERR_LCD_E   NV3030_SetBrightness(uint16 brightness)
{
    SCI_TraceLow("lcd:  NV3030_SetBrightness");
   // gouda_event(0x9000110a);

    return ERR_LCD_FUNC_NOT_SUPPORT;
}

//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E NV3030_SetDisplayWindow(
        uint16 left, //left of the window
        uint16 top, //top of the window
        uint16 right, //right of the window
        uint16 bottom //bottom of the window
        )
{
    gouda_event(0x9000110b);

    SCI_TraceLow("lcd:  NV3030_SetDisplayWindow");
    NV3030_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL void NV3030_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
    SCI_TraceLow("lcd:  NV3030_InvalidatePixel");
    gouda_event(0x9000110c);

    NV3030_InvalidateRect(x,y,x,y);
    LCD_DataWrite_NV3030(data);
}

LOCAL void NV3030_GetConfig(void)
{
    uint32 count,k;
    LCM_CFG_INFO_T_PTR cfg_ptr = PNULL;
    cfg_ptr = LCM_GetCfgInfo(0,&count);
    for(k=0;k<count;k++)
    {
        if(cfg_ptr[k].dev_id==LCD_DRV_ID_NV3030)
        {
            NV3030_line_mode = cfg_ptr[k].adc_min;
            NV3030_farmk = cfg_ptr[k].adc_max;

            break;
        }
    }
    SCI_TraceLow("lcd: NV3030_GetConfig: NV3030_line_mode = %d , NV3030_farmk =%d\n",NV3030_line_mode,NV3030_farmk);  //0x80,0xf5,0x3e
}

uint32 _Read_NV3030_ID(void)
{
    HAL_ERR_T r_err;
    uint8 id[4] = {0};

    hal_LcdDelay(140);
    hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    hal_LcdDelay(10);
    r_err = hal_GoudaReadData(0x04,id,4);///0xD3
    hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    hal_LcdDelay(10); 
	    SCI_TraceLow("lcd: _Read_NV3030_ID: id1->0x%0x, id2->0x%x, id3->0x%0x\n",id[3], id[2], id[1] );
	//Trace_Log_Print("NV3030G_ReadID lcd_id=%d, dev_id=0x%x",lcd_id,lcm_dev_id);
    return ((id[3]<<16)|(id[2] <<8)| id[1]);

}


LOCAL uint32 NV3030_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 ret_id = LCD_DRV_ID_NV3030;

    NV3030_GetConfig();

    hal_gouda_sim_init(NV3030_line_mode,NV3030_farmk,0x2a000);

    ret_id = _Read_NV3030_ID();

    hal_gouda_term();

    return ret_id;
}


const LCD_OPERATIONS_T NV3030_operations = 
{
    NV3030_Init,
    NV3030_EnterSleep,
    NV3030_SetContrast,
    NV3030_SetBrightness,
    NV3030_SetDisplayWindow,
    NV3030_InvalidateRect,
    NV3030_Invalidate,
    NV3030_Close,
    NV3030_RotationInvalidateRect,
    NV3030_SetDirection,
    NULL, 
    NV3030_ReadID	
};

LOCAL const LCD_TIMING_U s_NV3030_lcmtiming =
{
    20,  // CS setup time for LCM read (optional)  
    300, // low pulse width for LCM read (according spec)	
    300, // high pulse width for LCM read (according spec)
    15,   // CS setup time for LCM write  (optional) 	
    50,  // low pulse width for LCM write (according spec)
    50,  // high pulse width for LCM write (according spec)	
    // LCM_CYCLE_U end(ns)
};

LOCAL const LCD_TIMING_U s_NV3030_spitiming =
{
#ifdef FPGA_VERIFICATION
    39000000,
#else
    52000000,
#endif
    0,0,
    /*    SPI_CLK_52MHZ,        // clk frequency support (unit:MHz)
          SPI_CLK_IDLE_LOW,     // CPOL: 0--SPI_CLK_IDLE_LOW, 1--SPI_CLK_IDLE_HIGH
          SPI_SAMPLING_RISING,  // CPHA: 0--SPI_SAMPLING_RISING,  1--SPI_SAMPLING_FALLING 
     */    
    8,                    // tx bit length: 8/16bits refer to lcm driver ic 
    0,
    0
};

#ifdef MAINLCM_INTERFACE_SPI
const LCD_SPEC_T g_lcd_NV3030 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    SCI_NULL, 
    SCI_NULL,
    WIDTH_8,
    (LCD_TIMING_U*)&s_NV3030_spitiming, 
    (LCD_OPERATIONS_T*)&NV3030_operations,
    1,
    0
};
#else // not verify lcm
const LCD_SPEC_T g_lcd_NV3030 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_NV3030_lcmtiming, 
    (LCD_OPERATIONS_T*)&NV3030_operations,
    1,
    0
};
#endif



