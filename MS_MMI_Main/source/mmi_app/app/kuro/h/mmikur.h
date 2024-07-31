/*****************************************************************************
** File Name:      mmikur.h                                                  *
** Author:                                                                   *
** Date:           08/2007                                                *
** Description:    This file is used to mmikur_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       cmzs           Create
******************************************************************************/

#ifndef _MMKUR_H_
#define _MMKUR_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "mmi_appmsg.h"
#include "mmibt_a2dp.h"
#include "mmibt_app.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIKUR_NAME_MAX_LEN         (FMM_SEARCH_FILENAME_MAXLEN + 1)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : open kur player
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OpenKurPlayer(void);

/*****************************************************************************/
//  Description : stop kur play and close play window
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurAndCloseWin(void);

/*****************************************************************************/
//  Description : close kuro play window
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_CloseWin(
                void
                );

/*****************************************************************************/
//  Description : when format udisk,reset global variabel
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_ResetAllKur(void);

/*****************************************************************************/
//  Description : 跳转到前一首或者下一首Kur。
//              根据播放模式重新显示进度条和button和是否继续播放其他mp3等
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_GoToPreOrNextKur(
                   BOOLEAN      is_prev
                   );

/*****************************************************************************/
//  Description : set the volume of kur
//  Global resource dependence : none
//  Author: cmzs
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_SetKurVolume(//volume is change
                      BOOLEAN  is_increase
                      );

/*****************************************************************************/
//  Description : call when udisk exit
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OnUdiskShutDown(void);

/*****************************************************************************/
//  Description : call when udisk start up
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OnUdiskStartup(void);

/*****************************************************************************/
//  Description : MMIAPIKUR_Getmp3SampleRate
//  Global resource dependence : 
//  Author:Juan.Zhao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetkurSampleRate(void);
/*****************************************************************************/
//  Description : MMIAPIKUR_Recordmp3SampleRate
//  Global resource dependence : 
//  Author:Juan.Zhao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_RecordkurSampleRate(uint32 sample_rate);

/*****************************************************************************/
//  Description : MMIAPIKUR_IsklrUsed
//  Global resource dependence : 
//  Author:Juan.Zhao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsKlrUsed(uint16 * file_full_path, uint16 file_path_len);   

/*****************************************************************************/
//  Description : Whether the kuro file is the current play file.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsKurUsed(uint16 * file_full_path,uint16 file_path_len);

/*****************************************************************************/
//  Description : Whether the file can be deleted.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsFileUsed(
                        uint16              *file_path_ptr,
                        uint16              file_path_len
                        );

/*****************************************************************************/
//  Description : Whether the file is internal data of kuro player.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsInternalDataFile(
                        uint16              *file_path_ptr,
                        uint16              file_path_len
                        );

/*****************************************************************************/
//     Description : get sum size of all kur nv itme
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetSumNvSize(void);

/*****************************************************************************/
//  Description:process blue tooth a2dp avrcp of play/pause mp3
//  Global resource dependence:
//  Author: kelly.li
//  Note:
/*****************************************************************************/
#ifdef  BLUETOOTH_SUPPORT
PUBLIC void MMIAPIKUR_HandleA2dpMsg(
                                    MMIBT_FUNCTION_TYPE_E action_type
                );
#endif

/*****************************************************************************/
//  Description : Initialize kuro module when app initializes.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_Init(void);

/*****************************************************************************/
//  Description : Register kuro module
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_InitModule(void);

/*****************************************************************************/
//  Description : Get the head size of kur.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetKuroHeadSize(
                    void
                    );

/*****************************************************************************/
//  Description : initialize kuro dir
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_InitKurFolder(void);

/*****************************************************************************/
//  Description : whether the kuro player is playing
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerPlaying(void);

/*****************************************************************************/
//  Description : whether the kuro player is paused
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerPaused(void);

/*****************************************************************************/
//  Description : whether the kuro player is stopped
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerStopped(void);

/*****************************************************************************/
//  Description : whether the kuro player is not stopped
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerActive(void);

/*****************************************************************************/
//  Description : Get the delay timer, for playing kur in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIKUR_GetDelayPlayTimer(
                    void
                    );

/*****************************************************************************/
//  Description : Play kur in a delayed manner, handle delay timer,begin to play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_HandleDelayPlayTimer(
                    uint8 timer_id,
                    uint32 param
                    );

/*****************************************************************************/
//  Description : Start play kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PlayKurPlayer(
                    void
                    );

/*****************************************************************************/
//  Description : Pause kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PauseKurPlayer(
                    BOOLEAN  is_by_user
                    );

/*****************************************************************************/
//  Description : Resume kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_ResumeKurPlayer(
                    BOOLEAN  is_by_user
                    );

/*****************************************************************************/
//  Description : Stop kur player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurPlayer(
                    void
                    );

/*****************************************************************************/
//  Description : play music from my document
//  Global resource dependence : 
//  Author: xingdongli
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_PlayMyDocumentMusic(
                    MMIFILE_FILE_INFO_T   *music_file_info_ptr
                    );

/*****************************************************************************/
//  Description : Get the current kuro file name.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_GetCurKurName(
                    MMI_STRING_T   *kuro_name_ptr,
                    uint16         max_name_len
                    );

/*****************************************************************************/
//  Description : mp3 play main win is focus
//  Global resource dependence :
//  Author: Jasmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerFocus(void);

/*****************************************************************************/
//  Description : KURO 正常播放结束的回调函数。
//                根据播放模式重新显示进度条和button和是否继续播放其他 KURO 等
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_PlayKurCallBack(void);

/*****************************************************************************/
//  Description : Start play mp3
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PlayKurForBt(
                    void
                    );

/*****************************************************************************/
//  Description : Pause the KURO BG player
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_PauseKurForBt(
                    BOOLEAN  is_by_user
                    );

/*****************************************************************************/
//  Description : Resume KURO BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_ResumeKurForBt(
                    void
                    );

/*****************************************************************************/
//  Description : Stop the Kuro BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurForBt(
                    void
                    );

/*****************************************************************************/
//  Description : Handle Fatal Error.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_HandleFatalError(void);

/*****************************************************************************/
//  Description : get mp3 file sample rate
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_GetSampleRateBeforePlay(uint32*    sample_rate_ptr);
/*****************************************************************************/
//  Description : Pause the KURO BG player
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PauseKurForBt(
                    BOOLEAN  is_by_user
                    );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

                           
#ifdef   __cplusplus
    }
#endif

#endif

