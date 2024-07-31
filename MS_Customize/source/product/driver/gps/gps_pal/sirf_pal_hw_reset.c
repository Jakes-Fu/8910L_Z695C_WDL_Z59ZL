/**
 * @addtogroup platform_src_sirf_pal_win32
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
 */

/**
 * @file   sirf_pal_hw_reset.c
 *
 * @brief  Tracker reset control module.
 */

#include "sirf_types.h"
#include "sirf_pal.h"

extern void GPIO_GPS_Reset(unsigned char on_off); //GSD4t Reset Pin <---> BB GPIO 75

void CSRGPS_RESET_Pin_Output(unsigned char level) /*level: 1 --- high, 0 --- low*/
{
	GPIO_GPS_Reset(level);
}

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

/* A global handler for the reset port to control EXT_SRESET_N pin */
tSIRF_HANDLE ResetPortHandle = SIRF_INVALID_HANDLE;

/**
 * @brief Open and Init the reset port
 * 
 * @param[in] level           Initial line value
 * @return                    SIRF_SUCCESS or non-zero error code
 */
tSIRF_RESULT SIRF_PAL_HW_OpenRESET( tSIRF_UINT32 level )
{
   return SIRF_SUCCESS;
}

/**
* @brief Close the reset port.
* @return                       SIRF SUCCESS or non-zero error code
*/
tSIRF_RESULT SIRF_PAL_HW_CloseRESET( void )
{
   return SIRF_SUCCESS;
}

/**
* @brief Reset the tracker.
* @param[in] level              Non-zero to put into reset.
* @return                       Success code.
*/
tSIRF_RESULT SIRF_PAL_HW_WriteRESET( tSIRF_UINT32 level )
{
   if(level == SIRF_PAL_HW_RESET_HIGH)
   {
   	CSRGPS_RESET_Pin_Output(1);
   }
   else
   {
   	CSRGPS_RESET_Pin_Output(0);
   }
   
   return SIRF_SUCCESS;
} /* SIRF_PAL_HW_WriteRESET() */

/**
 * @}
 * End of file.
 */
