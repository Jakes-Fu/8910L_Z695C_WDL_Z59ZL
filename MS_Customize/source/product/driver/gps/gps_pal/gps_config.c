/******************************************************************************
 ** File Name:      Gps_SiRF.c                                                *
 ** Author:         David.Jia                                                 *
 ** DATE:           7/31/2007                                                 *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    SiRF GPS interface.                                       *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE            NAME            DESCRIPTION                               * 
 ** 07/31/2007      David.Jia       SiRF GPS interface base on Gps_drv.       *
 ** 08/02/2007      David.Jia       Change names of COM_Init/COM_Close/Map_Read/Map_Write 
 **     to GPS_ComInit/GPS_ComClose/GPS_ComRead/GPS_ComWrite.
 ** 08/14/2007      David.Jia       Add gpio control.                         *
 ** 09/04/2007      David.Jia       Modify Srf_Open for download firmware.    *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "GPS_COM.h"
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


#define SIRF_DEBUG

#ifdef  SIRF_DEBUG
#define GPS_LOG     SCI_TraceLow
#endif


#define     GPIO_GPS_ON                 85      //GPS On/Off
#define     GPIO_GPS_BOOT_INTER         16      //GPS boot from Internal(Reflash)/External
#define     GPIO_GPS_RESET              80      //GPS Reset
#define     GPIO_GPS_POWER              13      //GPS Power On/Off

#ifdef _SP7560_P1_
 #define     GPIO_GPS_WAKEUP             4       //@David.Jia 2007.9.25 GPS Wakeup/Sleep
#else
 #define     GPIO_GPS_WAKEUP                   // no wakeup pin @P2 7560
#endif

extern void GPIO_SetValue (uint32 gpio_id, BOOLEAN b_on);


/*****************************************************************************/
//  Description:    Control GPS's power
//                  is_open = SCI_TRUE, 	power on
//                  is_open = SCI_FALSE, 	power off
//  Author:         David.Jia
//  Note:           
/*****************************************************************************/
LOCAL void GPIO_GPS_PowerOn(BOOLEAN is_open)
{
    //SCI_TraceLow:"GPIO_GPS_PowerOn: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_CONFIG_58_112_2_18_0_33_12_1619,(uint8*)"d", is_open);

    GPIO_SetValue( GPIO_GPS_POWER, is_open);

}

/*****************************************************************************/
//  Description:    Control GPS On/Off
//                  is_open = SCI_TRUE, 	on
//                  is_open = SCI_FALSE, 	off
//  Author:         David.Jia
//  Note:           
/*****************************************************************************/
void GPIO_GPS_On(BOOLEAN is_open)
{
    //SCI_TraceLow:"GPIO_GPS_On: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_CONFIG_71_112_2_18_0_33_12_1620,(uint8*)"d", is_open);

    GPIO_SetValue( GPIO_GPS_ON, is_open);
  

}

/*****************************************************************************/
//  Description:    Control GPS boot from internal(reflash)/external
//                  is_open = SCI_TRUE, 	internal (reflash)
//                  is_open = SCI_FALSE, 	external (normal)
//  Author:         David.Jia
//  Note:           
/*****************************************************************************/
LOCAL void GPIO_GPS_BootInter(BOOLEAN is_open)
{
    //SCI_TraceLow:"GPIO_GPS_BootInter: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_CONFIG_85_112_2_18_0_33_12_1621,(uint8*)"d", is_open);

    GPIO_SetValue( GPIO_GPS_BOOT_INTER, is_open);

}

/*****************************************************************************/
//  Description:    Control GPS reset
//                  is_open = SCI_TRUE, 	reset high
//                  is_open = SCI_FALSE, 	reset low
//  Author:         David.Jia
//  Note:           
/*****************************************************************************/
void GPIO_GPS_Reset(BOOLEAN is_open)
{
    //SCI_TraceLow:"GPIO_GPS_ResetLow: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_CONFIG_98_112_2_18_0_33_12_1622,(uint8*)"d", is_open);

    GPIO_SetValue( GPIO_GPS_RESET, is_open);       //low to reset    

}

/*****************************************************************************/
//  Description:    Control GPS wakeup/sleep
//                  is_open = SCI_TRUE, 	sleep
//                  is_open = SCI_FALSE, 	wakeup
//  Author:         David.Jia
//  Note:           
/*****************************************************************************/
LOCAL void GPIO_GPS_Wakeup(BOOLEAN is_open)
{
    //SCI_TraceLow:"GPIO_GPS_Wakeup: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_CONFIG_111_112_2_18_0_33_12_1623,(uint8*)"d", is_open);
   
      #ifdef _SP7560_P1_
       GPIO_SetValue( GPIO_GPS_WAKEUP, !is_open);      //low to wakeup
      #endif 
}



/*****************************************************************************/
//  FUNCTION:       Srf_Read
//  Description:    call Map_Read
//  return:         
//  INPUT:          
//  Author:         David.Jia
//  date:           2007.7.31
//	Note:           
/*****************************************************************************/
uint32 Srf_ReadData(uint8* read_buf_ptr, uint32 byte_to_read)
{
	uint32 ret_val = 0;
    //GPS_LOG:"Srf_ReadData: buf=%x, len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_CONFIG_130_112_2_18_0_33_12_1624,(uint8*)"dd", (uint32)read_buf_ptr, byte_to_read);
    ret_val = (uint32)GPS_ComRead(read_buf_ptr, byte_to_read);

	return ret_val;
}

/*****************************************************************************/
//  FUNCTION:       Srf_WriteData
//  Description:    call Map_Write
//  return:         
//  INPUT:          
//  Author:         David.Jia
//  date:           2007.7.31
//	Note:           
/*****************************************************************************/
uint32 Srf_WriteData(uint8* read_buf_ptr, uint32 byte_to_read)
{
    //GPS_LOG:"Srf_WriteData: buf=%x, len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_CONFIG_145_112_2_18_0_33_12_1625,(uint8*)"dd", (uint32)read_buf_ptr, byte_to_read);
    return (uint32)GPS_ComWrite(read_buf_ptr, byte_to_read);
}


/*****************************************************************************/
//  FUNCTION:       Srf_Identify(void)
//  Description:    true for SiRF GPS
//  return:         
//  INPUT:          
//  Author:         David.Jia
//  date:           2007.7.31
//	Note:           
/*****************************************************************************/
BOOLEAN Srf_Identify(void)
{
    //GPS_LOG:"Srf_Identify"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_CONFIG_159_112_2_18_0_33_12_1626,(uint8*)"");
    return TRUE;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

// End of Gps_SIRF.c
