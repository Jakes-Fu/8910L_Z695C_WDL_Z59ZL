/******************************************************************************
 ** File Name:      lcmif_drv.h                                               *
 ** Author:         hongwei.xu                                                *
 ** DATE:           10/15/2007                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contains driver for LCM I/F controller. 
 **                                                          
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE             NAME          DESCRIPTION                                *
 ** 10/15/2007     hongwei xu       Create.                                    *
 ** 11/02/2007     hongwei xu      eliminate all the strategy                 *
 **  
 ******************************************************************************/

#ifndef _LCMIF_DRV_H_
#define _LCMIF_DRV_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "dal_lcd.h"
#include "lcm_drv.h"
#include "kd.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/******************************************************************************/
//LCM INTERFACE DECLARE
/******************************************************************************/

/******************************************************************************/
// Description:  Set the related registers to the default value, and initialize
// Global resource dependence: 
// Author: hongwei xu
// Note:
/******************************************************************************/
KDboolean LCMIF_Init(KDboolean is_reset);

/******************************************************************************/
// Description:   Close the LCMIF. Delete the mutex. 
// Global resource dependence: 
// Author:  hongwei xu
// Note:
/******************************************************************************/
KDboolean LCMIF_Close(void);

/******************************************************************************/
// Description:    set lcm parameter
// Global resource dependence: 
// Author:  hongwei xu
// Note: 
/******************************************************************************/
void LCMIF_SetParam(KDuint32 r_ahb_clk, //rocky ahb clock
                    KDuint32 s_ahb_clk  //seine ahb clock
                    );

/******************************************************************************/
// Description:    Read a KDuint32 value from the 'read_addr'.
// Global resource dependence: 
// Author: hongwei xu
// Note:  The 'read_addr' must be devided exactly by 4.
/******************************************************************************/
KDboolean LCMIF_Read(// 
                     KDuint32* read_data,//save the data read from 'read_addr'
                     KDuint32 read_addr// the addr of the value want to read
                     );

/******************************************************************************/
// Description:    Write a KDuint32 value 'write_data' to 'write_addr'.
// Global resource dependence: 
// Author:  hongwei xu
// Note: The 'write_addr' must be devided exactly by 2.
/******************************************************************************/
KDboolean LCMIF_Write(KDuint32 write_data, KDuint32 write_addr);

/******************************************************************************/
// Description:    Read 'read_len' bytes from 'read_from_addr' and save to 'read_ptr'.
//         Before read the data, the programme will get the MUTEX of the LCMIF first.
// Global resource dependence: 
// Author:  hongwei xu
// Note: The 'read_from_addr' must be devided exactly by 4.
/******************************************************************************/
KDuint32 LCMIF_Read_Data(KDuint8* read_ptr,KDuint32 read_len, KDuint32 read_from_addr);

/******************************************************************************/
// Description:    Write 'write_len' bytes data from 'write_ptr' to 'write_to_addr'.
//           Before write, the programme will get the MUTEX of the LCMIF first.
// Global resource dependence: 
// Author:  hongwei xu
// Note: The 'write_to_addr' must be devided exactly by 2.
/******************************************************************************/
KDuint32 LCMIF_Write_Data(KDuint8* write_ptr,KDuint32 write_len,KDuint32 write_to_addr);

/******************************************************************************/
// Description:  Set addr cs_index=0~5
//                            cd_index=0~5
// Global resource dependence: 
// Author: hongwei xu
// Note:
/******************************************************************************/
void LCMIF_SetAddress(KDuint16 cs_index,KDuint16 cd_index);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // LCMIF_CONTROLLER_H

/* End Of File  */

