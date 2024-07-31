/**
 * @addtogroup sirf_pal_win32
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved.
 *
 *    This Software is protected by United States copyright laws and
 *    international treaties.  You may not reverse engineer, decompile
 *    or disassemble this Software.
 *
 *    WARNING:
 *    This Software contains SiRF Technology Inc.’s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.’s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 *
 *  Keywords for Perforce.  Do not modify.
 *
 *  $File: //customs/customer/spreadtrum/6800h/Software/sirf/pal/Threadx_Syn/sirf_pal_hw_on.c $
 *
 *  $DateTime: 2010/08/11 07:24:07 $
 *
 *  $Revision: #5 $
 *
 */

/**
 * @file   sirf_pal_hw_on.c
 *
 * @brief  Tracker on control module.
 */
#include "sirf_types.h"
#include "sirf_pal.h"
#include "sirf_pal_hw_on.h"

extern void GPIO_GPS_On(unsigned char on_off); //GSD4t OnOff Pin <---> BB GPIO 76

void CSRGPS_ONOFF_Pin_Output(unsigned char level) /*level: 1 --- high, 0 --- low*/
{
	GPIO_GPS_On(level);
}

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

tSIRF_HANDLE OnOffPortHandle = NULL;

/**
 * @brief Open and Init the ON_OFF port handle
 * 
 * @param[in] level           Initial line value
 * @return                    SIRF_FAILURE or SIRF_SUCCESS.
 */
tSIRF_RESULT SIRF_PAL_HW_OpenON_OFF( tSIRF_UINT32 level )
{
   return SIRF_SUCCESS;
}

/**
 * @brief Close the ON_OFF port handle
 * 
 * @return                    SIRF_FAILURE or SIRF_SUCCESS.
 */
tSIRF_RESULT SIRF_PAL_HW_CloseON_OFF( void )
{
   /* This just returns a SUCCESS as RESET and ON_OFF lines use the same COM port */
   /* Do not need to check line usage as we are returning SUCCESS */
   return SIRF_SUCCESS;
}

/**
 * @brief Set the ON_OFF line
 * @param[in] level              SIRF_PAL_HW_ON_HIGH or SIRF_PAL_HW_ON_LOW
 * @return                       SIRF_SUCCESS or SIRF_FAILURE
 */
tSIRF_RESULT SIRF_PAL_HW_WriteON_OFF( tSIRF_UINT32 level )
{
    if(level == SIRF_PAL_HW_RESET_HIGH)
   {
   	CSRGPS_ONOFF_Pin_Output(1);
   }
   else
   {
   	CSRGPS_ONOFF_Pin_Output(0);
   }
   
   return SIRF_SUCCESS;
} /* SIRF_PAL_HW_WriteON_OFF() */

/**
 * @}
 * End of file.
 */
