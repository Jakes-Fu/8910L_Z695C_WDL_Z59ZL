/******************************************************************************
 ** File Name:      mtv_boot.h                                                *
 ** Author:          hongwei.xu                                               *
 ** DATE:           11/02/2007                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contains driver for LCM I/F controller. 
 **                                                          
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME               DESCRIPTION                         *
 ** 11/02/2007     hongwei xu	   Create.
 **  
 ******************************************************************************/

#ifndef _MTV_BOOT_H_
#define _MTV_BOOT_H_


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

/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                               Data Prototypte                             **
**----------------------------------------------------------------------------*/

/************************************************************************
* \fn  typedef void (* MTVBOOT_CALLBACK)(void)
* \param  data length
* \return void 
* \brief  call it after boot ok
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
typedef void (*MTVBOOT_CALLBACK)(void);


/******************************************************************************/
// Description:  Set the non-bypass mode, initialize the SDRAM, download the
//     ARM and DSP code, and register a callback function to process the messages
//     response from Seine.
// Global resource dependence: 
// Author:  hongweixu
// Note:
/******************************************************************************/
KDboolean MTVBOOT_TVOpen(
                              MTVBOOT_CALLBACK    callback,
                              KDuint8 *mem1,
                              KDuint8 *mem2
                              );
/******************************************************************************/
// Description:    
// Global resource dependence: 
// Author: 
// Note:
/******************************************************************************/
KDboolean MTVBOOT_TVOff(MTVBOOT_CALLBACK tv_off_callback);

/******************************************************************************/
// Description:  tv boot is ok
//     
// Global resource dependence: 
// Author:  hongweixu
// Note:   TRUE OK
/******************************************************************************/
KDboolean MTVBOOT_IsOK(void);

/******************************************************************************/
// Description:  set off flag for assert
//     
// Global resource dependence: 
// Author:  hongweixu
// Note:   
/******************************************************************************/
void MTVBOOT_SetOffFlagForAssert(void);

/******************************************************************************/
// Description:  download firmware to sharemem to get lcd id
// Global resource dependence: 
// Author:  hongweixu
// Note:
/******************************************************************************/
KDboolean MTVBOOT_IdentifyLCDId( KDuint32 lcdid_addr,KDuint32 *lcd_id1_ptr,KDuint32 *lcd_id2_ptr,KDuint32 *sim_smd_ptr);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //BOOT_H



