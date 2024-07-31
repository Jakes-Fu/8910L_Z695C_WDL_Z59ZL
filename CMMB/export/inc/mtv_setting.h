/******************************************************************************
 ** File Name:      mtv_setting.h                                                     *
 ** Author:         Hao Zhang                                                 *
 ** DATE:           10/27/2008                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    The file declares the DAC interface.                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/27/2008     Hao Zhang        Create                                    *
 ******************************************************************************/

  
#ifndef _MTV_SETTING_H
#define _MTV_SETTING_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mtv_cfg.h"
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
#define MTV_DEBUG_MODE       0
#define MTV_RELEASE_MODE     1

//MTV log forder
#define MTV_LOG_FOLDER                    "mtvlog"
#define MTV_DSP_LOG_FILE_POSTFIX          "_dsp.log"
#define MTV_ARM_LOG_FILE_POSTFIX          "_arm.txt"
#define MTV_SD_DEVICE_NAME                "\x00\x45\x00\x3a\x00\x00"
#define MTV_UDISK_DEVICE_NAME             "\x00\x44\x00\x3a\x00\x00"

#define MTV_LOGTYPE_ARM     0
#define MTV_LOGTYPE_DSP     1
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
uint8 MTV_GetAssertMode(void);

/*****************************************************************************/
//  Description :根据NV中的seine log项初始化V log to BB开关
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
PUBLIC void MTV_InitVLogFlag(void);

/*****************************************************************************/
//  Description :This Function is used to get mtv log flag
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
PUBLIC KDboolean MTV_GetVLogFlag(void);

/*****************************************************************************/
//  Description :This Function is used to set mtv log flag
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
PUBLIC void MTV_SetVLogFlag(KDboolean  flag);

/*****************************************************************************/
//  Description :This Function is used to get mtv log to bb flag
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
PUBLIC KDboolean MTV_GetVLogToBBFlag(void);

/*****************************************************************************/
//  Description :This Function is used to set mtv log to bb flag
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
PUBLIC void MTV_SetVLogToBBFlag(KDboolean  flag);

/*****************************************************************************/
//  Description :This Function is used to get mtv log storage
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
PUBLIC KDboolean MTV_GetVLogStorage(void);

/*****************************************************************************/
//  Description :This Function is used to set mtv log storage
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
PUBLIC void MTV_SetVLogStorage(KDboolean  is_sdcard);

/*****************************************************************************/
//  Description : Get&Set MTV_COMM_CONFIG_T for init V
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
#ifndef WIN32
/*****************************************************************************/
//  Description : Get&Set MTV_COMM_CONFIG_T for init V
//  Global resource dependence : none
//  Author:Bill Ji
//  Note:
/*****************************************************************************/
PUBLIC void MTV_SetCommConfig(MTV_COMM_CONFIG_T  *in_cfg);

/*****************************************************************************/
// Description: Create log folder
// Global resource dependence:
// Author:
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MTV_CreateLogfolder(BOOLEAN  is_sd_plug_in);

/*****************************************************************************/
// Description: Create log file.
// Global resource dependence:
// Author:
// Note:
/*****************************************************************************/
PUBLIC uint32 MTV_CreateLogFile(BOOLEAN  is_sd_plug_in);
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _DAC_H
