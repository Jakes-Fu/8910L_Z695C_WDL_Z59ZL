/******************************************************************************
 ** File Name:      gps_cfg.c                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           07/26/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of GPS   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             		DESCRIPTION                       *
 ** 07/26/2007     Liangwen.Zhen/David.Jia    Create.                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 



/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#include "gps_cfg.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         extend Variables and function                     *
 **---------------------------------------------------------------------------*/
#ifdef GPS_CHIP_VER_GREENEYE2
extern GPS_OPERATIONS_T g_greeneye2_gps_operation;
#else
#ifndef PRODUCT_DRIVER_SC6800_DVB 
extern GPS_OPERATIONS_T g_sirf_gps_operation;
#endif
#endif
/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 // Customer need to add GPS probe into below table
const GPS_OPERATIONS_T * GPS_OPS_TAB[]=
{
#ifdef GPS_CHIP_VER_GREENEYE2
        &g_greeneye2_gps_operation,
#else
#ifndef PRODUCT_DRIVER_SC6800_DVB 	
	&g_sirf_gps_operation,
#endif
#endif
	PNULL					// END
};

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to get GPS information table pointer
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPS_OPERATIONS_T ** GPS_GetOperationTab(void)
{
	return (GPS_OPERATIONS_T **)GPS_OPS_TAB;
}

/*****************************************************************************/
//  Description:    This function is used to get GPS information table lenght 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 GPS_GetOperationTabLen(void)
{
	return (uint32)(sizeof(GPS_OPS_TAB)/sizeof(GPS_OPERATIONS_T*));
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of gps_cfg.c
