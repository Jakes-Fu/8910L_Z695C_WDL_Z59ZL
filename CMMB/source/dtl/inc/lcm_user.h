/******************************************************************************
 ** File Name:      lcm_user.h                                                *
 ** Author:         hongwei.xu                                                *
 ** DATE:           10/15/2007                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file for user define LCM controller                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE             NAME          DESCRIPTION                                *
 ** 10/15/2007     hongwei xu       Create.                                    *
 ** 11/02/2007     hongwei xu      eliminate all the strategy                 *
 **  
 ******************************************************************************/

#ifndef _LCM_USER_H_
#define _LCM_USER_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
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
KDboolean LCMUD_Init(KDboolean is_reset);

/******************************************************************************/
// Description:   Close the LCMIF. 
// Global resource dependence: 
// Author:  hongwei xu
// Note:
/******************************************************************************/
KDboolean LCMUD_Close(void);

/******************************************************************************/
// Description:    set lcm parameter
// Global resource dependence: 
// Author:  hongwei xu
// Note: 
/******************************************************************************/
void LCMUD_SetParam(KDuint32 r_ahb_clk, //rocky ahb clock
                    KDuint32 s_ahb_clk  //seine ahb clock
                    );

/******************************************************************************/
// Description:  write command
// Global resource dependence: 
// Author: hongwei xu
// Note:
/******************************************************************************/
void LCMUD_Write_Cmd(KDuint16 cmd);

/******************************************************************************/
// Description:  write data
// Global resource dependence: 
// Author: hongwei xu
// Note:
/******************************************************************************/
void LCMUD_Write_Data(KDuint16 data);

/******************************************************************************/
// Description:  read cmd
// Global resource dependence: 
// Author: hongwei xu
// Note: that is read rs status
/******************************************************************************/
KDuint16 LCMUD_Read_CMD(void);

/******************************************************************************/
// Description:  write data
// Global resource dependence: 
// Author: hongwei xu
// Note:
/******************************************************************************/
KDuint16 LCMUD_Read_Data(void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // LCMIF_CONTROLLER_H

/* End Of File  */

