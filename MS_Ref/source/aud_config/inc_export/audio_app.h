/******************************************************************************
 ** File Name:      audio_app.h                                               *
 ** Author:         shujing.dong                                              *
 ** DATE:           06/30/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines audio general application interface.    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/30/2010     shujing.dong     Create.                                   *
 ******************************************************************************/

 /*! \file audio_app.h
  *  \author shujing.dong
  *  \date june 30, 2010
  *  \brief This file defines audio general application interface.
  */

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "audio_api.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

typedef enum 
{
    DATA_DUMP_TO_FILE = 0,
    DATA_DUMP_TO_ARM_LOG = 1,
}AUD_DUMP_TO_FLAG_E;

typedef enum 
{
    DUMP_SAVE_TO_RAM = 0,
    DUMP_SAVE_TO_FLASH = 1,
}AUD_DUMP_SAVE_FLAG_E;
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC void AUDIO_APP_EnableDataFetch(void)
//! \param  is_enable
//! \return AUDIO_RESULT_E
//! \brief  Description: This function is used to set data_fetch enable/disable.
//! \author Author: shujing.dong
//! \note   Note:Nothing.           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDataFetch(  // If succeed, return AUDIO_NO_ERROR,
                                        // else return error value
    BOOLEAN is_enable                   // SCI_TRUE: Enable,   SCI_FALSE: Disable
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDatadump(
//!    BOOLEAN bOn,             //start/stop
//!    AUD_DUMP_SAVE_FLAG_E uidump_save, // 1:change NV; 0: active untill reopen phone
//!    AUD_DUMP_TO_FLAG_E uidump_path, // 0:DUMP_TO_FILE; 1:DUMP_TO_ARM_LOG
//!    uint32 uiposflag         //posflag:bit_0:pos_0; bit_1:pos_1;.....bit_n:pos_n
//!    )
//! \param bOn              start/stop
//! \param bwrite_to_flash  TRUE:change NV; FALSE: not change nv, active untill reopen phone.
//! \param bdump_to_log     TRUE:DUMP_TO_ARM_LOG  FALSE:DUMP_TO_FILE
//! \param uiposflag        uint32:32 posisions.bit_0:pos_0;bit_1:pos_1....bit_31:pos_31;
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is dynamic open/close dump pcm in audio.
//! \author Author: yaye.jiang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDatadump(
    BOOLEAN bOn,             //start/stop
    AUD_DUMP_SAVE_FLAG_E uidump_save, // 1:change NV; 0: active untill reopen phone
    AUD_DUMP_TO_FLAG_E uidump_path, // 0:DUMP_TO_FILE; 1:DUMP_TO_ARM_LOG
    uint32 uiposflag         //posflag:bit_0:pos_0; bit_1:pos_1;.....bit_n:pos_n
    );

PUBLIC void AUDIO_EnableTxClarity(BOOLEAN bEnable);

PUBLIC BOOLEAN AUDIO_GetTxClarityEnable(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of audio_api.c

